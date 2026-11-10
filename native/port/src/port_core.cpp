// ra2_port core: logging, last-error, the handle table, DOS path mapping and
// the host hooks shared by the Win32 seam files.
#include "ra2port/win32_port.hpp"
#include "os.hpp"

#include <ctype.h>
#include <dirent.h>
#include <execinfo.h>
#include <signal.h>
#include <map>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <set>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <vector>

namespace ra2port {

namespace {
std::mutex &table_lock() { static std::mutex m; return m; }
std::vector<HandleObject *> &table() { static std::vector<HandleObject *> t; return t; }
thread_local DWORD last_error = 0;
std::string &data_root() { static std::string d; return d; }
// Write overlay (--write-dir; always set by --run-to): files the game creates or
// writes land here instead of the user's installation, which stays read-only;
// reads look here first so the game sees its own writes.
std::string &write_root() { static std::string d; return d; }
const Host *current_host = nullptr;
const uintptr_t HANDLE_BASE = 0x100;
}

void unimplemented(const char *name)
{
    static std::mutex m;
    static std::set<std::string> seen;
    std::lock_guard<std::mutex> g(m);
    if (seen.insert(name).second && getenv("RA2_PORT_QUIET") == nullptr)
        fprintf(stderr, "ra2_port: %s not implemented (returns failure)\n", name);
    progress_count("unported imports", name);
}

void trace(const char *fmt, ...)
{
    if (getenv("RA2_PORT_TRACE") == nullptr) return;
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "ra2_port: ");
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}

void set_last_error(DWORD code) { last_error = code; }

HANDLE handle_new(HandleObject *object)
{
    std::lock_guard<std::mutex> g(table_lock());
    auto &t = table();
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i] == nullptr) {
            t[i] = object;
            return (HANDLE)(HANDLE_BASE + i * 4);
        }
    }
    t.push_back(object);
    return (HANDLE)(HANDLE_BASE + (t.size() - 1) * 4);
}

HandleObject *handle_get(HANDLE handle, int kind)
{
    uintptr_t v = (uintptr_t)handle;
    if (v < HANDLE_BASE || (v - HANDLE_BASE) % 4) return nullptr;
    std::lock_guard<std::mutex> g(table_lock());
    size_t i = (v - HANDLE_BASE) / 4;
    auto &t = table();
    if (i >= t.size() || t[i] == nullptr) return nullptr;
    if (kind && t[i]->kind != kind) return nullptr;
    return t[i];
}

bool handle_close(HANDLE handle)
{
    uintptr_t v = (uintptr_t)handle;
    if (v < HANDLE_BASE || (v - HANDLE_BASE) % 4) return false;
    HandleObject *object = nullptr;
    {
        std::lock_guard<std::mutex> g(table_lock());
        size_t i = (v - HANDLE_BASE) / 4;
        auto &t = table();
        if (i >= t.size() || t[i] == nullptr) return false;
        object = t[i];
        t[i] = nullptr;
    }
    delete object;
    return true;
}

std::string overlay_path(const std::string &host);
void set_data_dir(const char *dir) { data_root() = dir ? dir : ""; }
const char *data_dir() { return data_root().empty() ? "." : data_root().c_str(); }

// File-system operations on the game's behalf (Scout1209 diagnostics).
namespace {
std::atomic<unsigned long> fs_counts[FS_OP_KINDS];
}
void keep_alive()
{
    static std::atomic<uint64_t> next{0};
    uint64_t now = os::monotonic_ns();
    uint64_t due = next.load(std::memory_order_relaxed);
    if (now < due || !next.compare_exchange_strong(due, now + 50000000u)) return;
    const Host *h = host();
    if (h && h->keep_alive) h->keep_alive();
}

void fs_op(FsOp kind)
{
    keep_alive();
    fs_counts[kind].fetch_add(1, std::memory_order_relaxed);
    // Only the operations that are a round trip on a network volume (metadata
    // and opens); reads and seeks within an open file are served by the
    // client's cache.
    static long latency_us = [] { const char *v = getenv("RA2_NATIVE_FS_LATENCY_US"); return v ? atol(v) : 0L; }();
    if (latency_us > 0 && kind <= FS_OPEN) std::this_thread::sleep_for(std::chrono::microseconds(latency_us));
}
unsigned long fs_op_count(FsOp kind) { return fs_counts[kind].load(std::memory_order_relaxed); }

// Case-insensitive lookups from cached directory listings (Scout1209).
// The game probes thousands of names while it starts and loads a map: each
// lookup tries a loose file in the game folder before the MIX files, and most
// do not exist (about 16,000 opens before a skirmish's first frame). Each probe
// used to stat the name, read the whole folder (some 350 entries), stat the
// write folder's twin and try to open it: four file-system round trips, which
// on a network volume (the user's game folder) made start-up and loading take
// many times longer. A directory's listing, folded to lower case, now answers
// both "is it there" and "how is it spelled". A listing is checked against the
// directory's modification time at most every 2 s, and read again after the
// port resolved a path to write (host_path_write: it creates, deletes or
// renames a file), so the port's own changes are always seen.
namespace {
struct DirListing {
    bool valid = false;
    time_t mtime = 0;
    unsigned generation = 0;
    std::chrono::steady_clock::time_point checked;
    std::map<std::string, std::string> by_folded;   // lower-case name -> real name
};
std::mutex dir_cache_lock;
std::map<std::string, DirListing> dir_cache;
unsigned dir_generation = 1;
std::string folded(const std::string &name)
{
    std::string f = name;
    for (auto &c : f) c = (char)tolower((unsigned char)c);
    return f;
}
enum Lookup { FOUND, ABSENT, NO_DIR };
Lookup lookup(const std::string &dir, const std::string &name, std::string *real)
{
    *real = name;
    const std::string where = dir.empty() ? "/" : dir;
    auto now = std::chrono::steady_clock::now();
    std::lock_guard<std::mutex> g(dir_cache_lock);
    DirListing &l = dir_cache[where];
    if (!l.valid || l.generation != dir_generation || now - l.checked >= std::chrono::seconds(2)) {
        struct stat st;
        fs_op(FS_STAT);
        if (stat(where.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) { l.valid = false; return NO_DIR; }
        if (!l.valid || l.generation != dir_generation || l.mtime != st.st_mtime) {
            l.by_folded.clear();
            fs_op(FS_LIST);
            if (DIR *d = opendir(where.c_str())) {
                // The first spelling readdir returns wins, as the old scan did.
                while (struct dirent *e = readdir(d)) l.by_folded.emplace(folded(e->d_name), e->d_name);
                closedir(d);
            }
            l.mtime = st.st_mtime;
        }
        l.valid = true;
        l.generation = dir_generation;
        l.checked = now;
    }
    auto it = l.by_folded.find(folded(name));
    if (it == l.by_folded.end()) return ABSENT;
    *real = it->second;
    return FOUND;
}
// What the calling thread's last host_path found: 0 the file exists (or was not
// looked up), 2 the last component is missing, 3 a folder on the way is missing
// (Win32's ERROR_FILE_NOT_FOUND and ERROR_PATH_NOT_FOUND).
thread_local int last_lookup = 0;
}  // namespace

int host_path_missing() { return last_lookup; }

char *host_path(const char *game_path, char *out, size_t n)
{
    last_lookup = 0;
    std::string p = game_path ? game_path : "";
    for (auto &c : p) if (c == '\\') c = '/';
    // A drive-qualified path ("C:/...") is taken relative to the data root:
    // the game only names its own installation and save folders.
    if (p.size() >= 2 && p[1] == ':') p = p.substr(2);
    std::string base = data_dir();
    if (!p.empty() && p[0] == '/') {
        // An absolute host path that exists stays as it is.
        struct stat st;
        fs_op(FS_STAT);
        if (stat(p.c_str(), &st) == 0) { snprintf(out, n, "%s", p.c_str()); return out; }
        p = p.substr(1);
    }
    std::string result = base;
    size_t start = 0;
    while (start <= p.size()) {
        size_t slash = p.find('/', start);
        std::string part = p.substr(start, slash == std::string::npos ? std::string::npos : slash - start);
        if (!part.empty() && part != ".") {
            std::string real;
            Lookup found = last_lookup == 3 ? NO_DIR : lookup(result, part, &real);
            if (found != FOUND) last_lookup = (found == NO_DIR || slash != std::string::npos) ? 3 : 2;
            result += "/" + real;
        }
        if (slash == std::string::npos) break;
        start = slash + 1;
    }
    if (!write_root().empty()) {
        // The write folder's twin, when it exists, is the file the game sees.
        std::string w = overlay_path(result);
        size_t cut = w.rfind('/');
        std::string real;
        if (!w.empty() && cut != std::string::npos && lookup(w.substr(0, cut), w.substr(cut + 1), &real) == FOUND) {
            result = w.substr(0, cut + 1) + real;
            last_lookup = 0;
        }
    }
    snprintf(out, n, "%s", result.c_str());
    return out;
}

void set_write_dir(const char *dir) { write_root() = dir ? dir : ""; }
const char *write_dir() { return write_root().empty() ? nullptr : write_root().c_str(); }

// The overlay twin of a host path: paths under the data root keep their
// relative part, others their last component.
std::string overlay_path(const std::string &host)
{
    if (write_root().empty()) return std::string();
    const std::string &base = data_root();
    std::string rel;
    if (!base.empty() && host.compare(0, base.size(), base) == 0 &&
        (host.size() == base.size() || host[base.size()] == '/'))
        rel = host.substr(base.size());
    else if (host.compare(0, write_root().size(), write_root()) == 0)
        return host;
    else
        rel = "/" + host.substr(host.rfind('/') == std::string::npos ? 0 : host.rfind('/') + 1);
    return write_root() + rel;
}

char *host_path_write(const char *game_path, char *out, size_t n)
{
    host_path(game_path, out, n);
    {
        // The caller is about to create or change a file: listings read from here
        // on are read again (match_component).
        std::lock_guard<std::mutex> g(dir_cache_lock);
        ++dir_generation;
    }
    if (write_root().empty()) return out;
    std::string w = overlay_path(out);
    // Create the overlay's parent directories.
    for (size_t i = write_root().size() + 1; i < w.size(); ++i) {
        if (w[i] == '/') mkdir(w.substr(0, i).c_str(), 0755);
    }
    snprintf(out, n, "%s", w.c_str());
    return out;
}

size_t wlen(const WCHAR *s)
{
    size_t n = 0;
    while (s && s[n]) ++n;
    return n;
}

void set_host(const Host *h) { current_host = h; }
const Host *host() { return current_host; }

}  // namespace ra2port

extern "C" {
DWORD GetLastError(void) { return ra2port::last_error; }
void SetLastError(DWORD code) { ra2port::last_error = code; }
}

// Start-up progress (diagnostic). native_units.py puts a call at the head of
// every unit that owns original _initterm entries, with the index of its first
// one, so the last index recorded names the unit whose initialisers were
// running when a trap or crash stopped start-up. RA2_NATIVE_INIT_TRACE=1
// prints each mark as it is reached.
namespace {
int initterm_last = -1;
int initterm_units = 0;

// RA2_NATIVE_CRASH_TRACE=1: a fault prints the start-up position and a
// backtrace (diagnostics while the tree is still being closed).
void crash_trace(int sig)
{
    fprintf(stderr, "ra2-native: signal %d after initterm #%d (%d units)\n", sig, initterm_last, initterm_units);
    void *frames[48];
    int n = backtrace(frames, 48);
    backtrace_symbols_fd(frames, n, 2);
    signal(sig, SIG_DFL);
    raise(sig);
}
}

extern "C" int ra2port_initterm_mark(int index)
{
    if (initterm_units == 0 && getenv("RA2_NATIVE_CRASH_TRACE") != nullptr) {
        signal(SIGSEGV, crash_trace);
        signal(SIGBUS, crash_trace);
        signal(SIGABRT, crash_trace);
        signal(SIGILL, crash_trace);
    }
    initterm_last = index;
    ++initterm_units;
    if (getenv("RA2_NATIVE_INIT_TRACE") != nullptr)
        fprintf(stderr, "ra2-native: initterm #%d\n", index);
    return index;
}

extern "C" int ra2port_initterm_last(int *units)
{
    if (units) *units = initterm_units;
    return initterm_last;
}
