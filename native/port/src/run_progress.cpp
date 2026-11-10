// Start-up progress for test runs (ra2-native --run-to=<milestone>). The port
// sees every file, window, DirectDraw object and dialog the game asks for, so
// it names how far start-up got without touching recovered code. A run ends
// by itself: at the requested milestone, at the first link-closure trap, on a
// fault, or at the time limit (--run-seconds, default 5), always printing the
// summary to stderr. Diagnostics only; the game never depends on it.
#include "ra2port/win32_port.hpp"
#include "os.hpp"

#include <chrono>
#include <execinfo.h>
#include <map>
#include <pthread.h>
#include <mutex>
#include <signal.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <thread>
#include <unistd.h>
#include <vector>

extern "C" long long ra2port_now_ms(void);           // user32.cpp
extern "C" thread_local uint64_t ra2port_slept_ns;   // win32_time.cpp: this thread's Sleep time
static const auto process_start = std::chrono::steady_clock::now();

namespace ra2port {

namespace {

// Milestones in start-up order; "furthest" is the latest one reached.
const char *const ORDER[] = {
    "main",       // host main: every static initialiser has run
    "winmain",    // the recovered WinMain is entered
    "ini",        // RA2.INI opened (WinMain's configuration)
    "mix",        // the first MIX file opened (LANGUAGE.MIX: keys, then mix init)
    "strings",    // the string table (ra2.csf) opened
    "window",     // the main window created
    "ddraw",      // DirectDrawCreate
    "primary",    // the primary surface created (first DirectDraw surface)
    "surfaces",   // an off-screen surface after the primary (Allocate_Surfaces)
    "dialog",     // the first dialog requested (owner-drawn menus: templates)
    "present",    // the first frame presented
    "init",       // Init_Game returned success (Main_Game goes on to Select_Game)
    "menu",       // the main menu shown (not detected yet: a dialog, blocked with the templates)
    "scenario",   // --skirmish: Start_Scenario loaded the map (Select_Game returned true)
    "frame",      // --skirmish: the first Main_Loop iteration returned
    "frames",     // --skirmish: --skirmish-frames=N iterations returned (--frames=N: a game from the menus)
    "menu-return",// the main menu shown again after a game (Scout1119, M14)
};
const int ORDER_COUNT = sizeof(ORDER) / sizeof(ORDER[0]);

struct State {
    std::mutex lock;
    bool active = false;
    std::string target;
    int seconds = 5;
    std::chrono::steady_clock::time_point start;
    std::vector<std::pair<std::string, std::string>> reached;   // milestone, detail (+ms)
    std::map<std::string, std::vector<std::string>> counters;
    bool done = false;
};
State &state() { static State s; return s; }

int order_of(const std::string &m)
{
    for (int i = 0; i < ORDER_COUNT; ++i)
        if (m == ORDER[i]) return i;
    return -1;
}

long elapsed_ms()
{
    return (long)std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - state().start).count();
}

void summary_locked(const char *stop)
{
    State &s = state();
    if (s.done) return;
    s.done = true;
    // A coverage build (-fsanitize-coverage=func,trace-pc-guard, linked with
    // -fsanitize=undefined; UBSAN_OPTIONS=coverage=1) writes its .sancov file at
    // exit, which the runs' _exit skips: dump it here (diagnostics only; looked
    // up at run time so ordinary builds reference no sanitizer symbol).
    if (void (*dump)(void) = (void (*)(void))dlsym(RTLD_DEFAULT, "__sanitizer_cov_dump")) dump();
    int furthest = -1;
    std::string last;
    for (auto &r : s.reached) {
        int o = order_of(r.first);
        if (o > furthest) { furthest = o; last = r.first; }
    }
    fprintf(stderr, "run-to: target %s; stopped: %s after %ld ms; furthest milestone: %s\n",
            s.target.c_str(), stop, elapsed_ms(), furthest >= 0 ? last.c_str() : "(none)");
    for (auto &r : s.reached)
        fprintf(stderr, "run-to:   milestone %-9s %s\n", r.first.c_str(), r.second.c_str());
    for (auto &c : s.counters) {
        fprintf(stderr, "run-to:   %s: %zu", c.first.c_str(), c.second.size());
        size_t shown = 0;
        for (auto &d : c.second) {
            if (shown++ == 40) { fprintf(stderr, " ..."); break; }
            fprintf(stderr, "%s%s", shown == 1 ? " (" : ", ", d.c_str());
        }
        fprintf(stderr, "%s\n", c.second.empty() ? "" : ")");
    }
    fflush(stderr);
}

pthread_t main_thread;

// Time limit: the main thread prints where it is (a stall, e.g. a wait loop).
void on_stall(int)
{
    void *frames[48];
    int n = backtrace(frames, 48);
    fprintf(stderr, "run-to: main thread at the time limit:\n");
    backtrace_symbols_fd(frames, n, 2);
}

void on_signal(int sig, siginfo_t *info, void *context)
{
    // Best effort from a signal handler: the run is ending anyway. The faulting
    // pc and address (a frameless leaf does not appear in the backtrace).
    char what[128];
    uintptr_t pc = 0, lr = 0, frame = 0;
    bool registers = os::fault_registers(context, &pc, &lr, &frame);
    Dl_info where = {};
    if (pc) dladdr((void *)pc, &where);
    snprintf(what, sizeof what, "signal %d at %s+%ld (fault address %p)", sig,
             where.dli_sname ? where.dli_sname : "?", where.dli_sname ? (long)(pc - (uintptr_t)where.dli_saddr) : 0L,
             info ? info->si_addr : nullptr);
    summary_locked(what);
    void *frames[48];
    int n = backtrace(frames, 48);
    backtrace_symbols_fd(frames, n, 2);
    // A call through a bad pointer leaves pc outside any image, and backtrace()
    // stops there: walk the faulting thread's frame chain from the context.
    if (registers && !where.dli_sname) {
        uintptr_t *fp = (uintptr_t *)frame;
        char line[256];
        for (int i = 0; i < 24; ++i) {
            Dl_info at = {};
            dladdr((void *)lr, &at);
            int len = snprintf(line, sizeof line, "fault frame %d: %s+%ld\n", i, at.dli_sname ? at.dli_sname : "?",
                               at.dli_sname ? (long)(lr - (uintptr_t)at.dli_saddr) : 0L);
            if (len > 0) write(2, line, (size_t)len < sizeof line ? (size_t)len : sizeof line - 1);
            if (!fp || ((uintptr_t)fp & 7)) break;
            lr = fp[1];
            fp = (uintptr_t *)fp[0];
        }
    }
    _exit(4);
}

}  // namespace

void progress(const char *milestone, const char *detail)
{
    State &s = state();
    if (!s.active) return;
    std::lock_guard<std::mutex> g(s.lock);
    bool primary = false;
    for (auto &r : s.reached) {
        if (r.first == milestone) return;
        if (r.first == "primary") primary = true;
    }
    // "surfaces" counts the off-screen surfaces made after the primary one.
    if (!strcmp(milestone, "surfaces") && !primary) return;
    char text[512];
    snprintf(text, sizeof text, "+%ld ms%s%s", elapsed_ms(), detail ? "  " : "", detail ? detail : "");
    s.reached.emplace_back(milestone, text);
    fprintf(stderr, "run-to: reached %s %s\n", milestone, text);
    if (s.target == milestone) {
        summary_locked("target reached");
        _exit(0);
    }
}

void progress_count(const char *counter, const char *detail)
{
    State &s = state();
    if (!s.active) return;
    {
        std::lock_guard<std::mutex> g(s.lock);
        auto &v = s.counters[counter];
        std::string d = detail ? detail : "";
        bool seen = false;
        for (auto &x : v) if (x == d) { seen = true; break; }
        if (!seen) v.push_back(d);
    }
    // File-name milestones.
    if (detail && !strcmp(counter, "files opened")) {
        const char *base = detail;
        for (const char *p = detail; *p; ++p) if (*p == '\\' || *p == '/') base = p + 1;
        size_t n = strlen(base);
        if (!strcasecmp(base, "ra2.ini")) progress("ini", detail);
        else if (n > 4 && !strcasecmp(base + n - 4, ".mix")) progress("mix", detail);
        else if (n > 4 && !strcasecmp(base + n - 4, ".csf")) progress("strings", detail);
    }
}

}  // namespace ra2port

extern "C" {

// --run-to=<milestone> [--run-seconds=N]: arms the progress log, the watchdog
// and the fault summary. Unknown milestone names are accepted (they are never
// reached, so the run ends at a trap, a fault or the time limit).
void ra2port_run_to(const char *target, int seconds)
{
    using namespace ra2port;
    State &s = state();
    s.active = true;
    s.target = target ? target : "";
    s.seconds = seconds > 0 ? seconds : 5;
    s.start = std::chrono::steady_clock::now();
    struct sigaction action = {};
    action.sa_sigaction = on_signal;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    for (int sig : {SIGSEGV, SIGBUS, SIGILL, SIGFPE, SIGABRT}) sigaction(sig, &action, nullptr);
    main_thread = pthread_self();
    signal(SIGUSR2, on_stall);
    std::thread([] {
        std::this_thread::sleep_for(std::chrono::seconds(state().seconds));
        pthread_kill(main_thread, SIGUSR2);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        {
            std::lock_guard<std::mutex> g(state().lock);
            char what[48];
            snprintf(what, sizeof what, "time limit (%d s)", state().seconds);
            summary_locked(what);
        }
        _exit(2);
    }).detach();
}

int ra2port_run_active(void) { return ra2port::state().active ? 1 : 0; }

// Milestones reported from the dialect side (native/link/harness_skirmish.cpp).
void ra2port_progress(const char *milestone, const char *detail) { ra2port::progress(milestone, detail); }

// --skirmish=<map> [--skirmish-frames=N] (platform/sdl/main.cpp): the test
// harness that starts a skirmish without the menu dialogs.
namespace { const char *skirmish_map = nullptr; int skirmish_frames = 1; }
void ra2port_set_skirmish(const char *map, int frames)
{
    skirmish_map = map;
    // 0: interactive play (--play): no frame limit.
    skirmish_frames = frames > 0 ? frames : frames == 0 ? 0x7FFFFFFF : 1;
}
const char *ra2port_skirmish_map(void) { return skirmish_map; }
// --skirmish-order=<kind>[@<frame>] (link/harness_orders.cpp).
namespace { const char *skirmish_order = nullptr; }
void ra2port_set_skirmish_order(const char *order) { skirmish_order = order; }
// Also for a game started from the menus (Scout1121: the hunt order of the
// menu-to-score loop).
const char *ra2port_skirmish_order(void) { return skirmish_order; }
int ra2port_skirmish_frames(void) { return skirmish_frames; }
// Scripted input points (platform/sdl/host.cpp): the skirmish harness
// registers the resolver of "@ID[:n][+dx,dy]" (link/harness_input.cpp).
namespace { int (*point_resolver)(const char *, int *, int *) = nullptr; }
void ra2port_set_point_resolver(int (*resolver)(const char *spec, int *x, int *y)) { point_resolver = resolver; }
int ra2port_resolve_point(const char *spec, int *x, int *y) { return point_resolver ? point_resolver(spec, x, y) : 0; }
// A Main_Loop frame began (link/harness_skirmish.cpp): the host plays the
// scripted input due at it (--input-script).
namespace { int last_frame = 0; }
int ra2port_last_frame(void) { return last_frame; }
// --frames=N without --skirmish (Scout1119, M14): a game started from the
// menus reports milestone `frames` after N Main_Loop frames.
void ra2port_set_frames(int frames) { if (!skirmish_map) skirmish_frames = frames > 0 ? frames : 0x7FFFFFFF; }
void ra2port_frame(int frame)
{
    last_frame = frame;
    // RA2_NATIVE_PERF_LOG=1 (diagnostics, Scout1209): when a game's first Main_Loop
    // frame runs (steady ms, the input script's clock; ms since the process
    // started) and the main thread's sleeps so far.
    static int perf_log = -1;
    if (perf_log < 0) perf_log = getenv("RA2_NATIVE_PERF_LOG") ? 1 : 0;
    if (perf_log) {
        static int perf_last = -1;
        if (perf_last < 0 || frame < perf_last)
            fprintf(stderr, "perf: Main_Loop frame %d at steady %lld ms (%.0f ms after start), main thread slept %.0f ms\n", frame,
                    ra2port_now_ms(), std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - process_start).count(),
                    ra2port_slept_ns / 1e6);
        if (perf_last < 0 || frame < perf_last)
            fprintf(stderr, "perf: file system so far: %lu stat, %lu directory reads, %lu opens, %lu reads, %lu seeks\n",
                    ra2port::fs_op_count(ra2port::FS_STAT), ra2port::fs_op_count(ra2port::FS_LIST), ra2port::fs_op_count(ra2port::FS_OPEN), ra2port::fs_op_count(ra2port::FS_READ), ra2port::fs_op_count(ra2port::FS_SEEK));
        perf_last = frame;
    }
    // RA2_NATIVE_FPS_LOG=1: Main_Loop frames per wall-clock second on stderr
    // (any run, --play too): the frame limiter's check (Sync_Delay 0x00540960).
    static int fps_log = -1;
    if (fps_log < 0) fps_log = getenv("RA2_NATIVE_FPS_LOG") ? 1 : 0;
    if (fps_log) {
        static auto start = std::chrono::steady_clock::now();
        static int start_frame = frame;
        double s = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        if (s >= 5.0) {
            fprintf(stderr, "fps: %.1f (frames %d..%d in %.2f s)\n", (frame - start_frame) / s, start_frame, frame, s);
            start = std::chrono::steady_clock::now();
            start_frame = frame;
        }
    }
    const ra2port::Host *h = ra2port::host();
    if (h && h->frame) h->frame(frame);
}

// The run ended another way (WinMain returned): print the summary.
void ra2port_run_end(const char *why)
{
    using namespace ra2port;
    if (!state().active) return;
    std::lock_guard<std::mutex> g(state().lock);
    summary_locked(why);
}

// Called by the link-closure traps (gen/link_closure.c): unrecovered code was
// reached. Under --run-to the run ends here with the summary.
void ra2port_trap_reached(const char *name)
{
    using namespace ra2port;
    State &s = state();
    if (!s.active) return;
    {
        std::lock_guard<std::mutex> g(s.lock);
        s.counters["traps"].push_back(name);
    }
    if (getenv("RA2_NATIVE_TRAP_CONTINUE")) return;
    std::lock_guard<std::mutex> g(s.lock);
    std::string what = std::string("unrecovered ") + name;
    summary_locked(what.c_str());
    void *frames[48];                       // the trap's callers
    int n = backtrace(frames, 48);
    backtrace_symbols_fd(frames, n, 2);
    _exit(3);
}

}  // extern "C"

// SwizzleManagerClass::Process_Tables 0x0069FC30 (port/fixups.json, Scout1133):
// a loaded pointer whose object never called Here_I_Am. The original faults
// there (1 / 0; arm64 integer division does not trap, so the native loop spun).
// Prints the unmatched ID (the saving process's address), the field, and the
// nearest registered objects (old ID below the unmatched one, new object below
// the field) with their vtable symbols, so the owning class can be named.
namespace {
struct SwizzleRow { intptr_t id; void *pointer; };
const char *vtable_name(const void *object)
{
    if (!object) return "-";
    Dl_info info = {};
    void *vptr = *(void * const *)object;
    if (dladdr(vptr, &info) && info.dli_sname) return info.dli_sname;
    return "?";
}
int swizzle_unmatched_count = 0;
std::map<void *, void *> &swizzle_callers() { static std::map<void *, void *> m; return m; }
int swizzle_trace = -1;
}
extern "C" void ra2port_swizzle_note(void *field, void *caller)
{
    if (swizzle_trace < 0) swizzle_trace = getenv("RA2_NATIVE_SWIZZLE_TRACE") ? 1 : 0;
    if (swizzle_trace) swizzle_callers()[field] = caller;
}
extern "C" void ra2port_swizzle_unmatched(intptr_t id, void *field, const void *swizzled, int count)
{
    const SwizzleRow *rows = (const SwizzleRow *)swizzled;
    const SwizzleRow *below_id = nullptr, *owner = nullptr;
    for (int i = 0; i < count; ++i) {
        if (rows[i].id <= id && (!below_id || rows[i].id > below_id->id)) below_id = &rows[i];
        if ((char *)rows[i].pointer <= (char *)field && (!owner || (char *)rows[i].pointer > (char *)owner->pointer)) owner = &rows[i];
    }
    ++swizzle_unmatched_count;
    if (swizzle_unmatched_count > 200) return;
    fprintf(stderr, "swizzle: unmatched id %#lx field %p", (long)id, field);
    if (below_id) fprintf(stderr, " | id-1 %#lx (+%#lx) %s", (long)below_id->id, (long)(id - below_id->id), vtable_name(below_id->pointer));
    if (owner) fprintf(stderr, " | field in %p (+%#lx) %s", owner->pointer, (long)((char *)field - (char *)owner->pointer), vtable_name(owner->pointer));
    auto c = swizzle_callers().find(field);
    if (c != swizzle_callers().end()) {
        Dl_info info = {};
        dladdr(c->second, &info);
        fprintf(stderr, " | caller %s+%#lx (image +%#lx)", info.dli_sname ? info.dli_sname : "?",
                info.dli_saddr ? (long)((char *)c->second - (char *)info.dli_saddr) : 0L,
                (long)((char *)c->second - (char *)info.dli_fbase));
    }
    fprintf(stderr, "\n");
}
// After the table: the original faulted at the first unmatched field.
// RA2_NATIVE_SWIZZLE_CONTINUE=1 nulls them and goes on (diagnostics only).
extern "C" void ra2port_swizzle_done(void)
{
    if (!swizzle_unmatched_count) return;
    fprintf(stderr, "swizzle: %d unmatched fields\n", swizzle_unmatched_count);
    int n = swizzle_unmatched_count;
    swizzle_unmatched_count = 0;
    if (!getenv("RA2_NATIVE_SWIZZLE_CONTINUE")) { (void)n; raise(SIGFPE); }
}

