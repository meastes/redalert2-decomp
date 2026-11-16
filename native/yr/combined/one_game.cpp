// SPDX-License-Identifier: GPL-3.0-or-later
// One game (lane Y5; native/yr/README.md "One game"): the combined app's thin host. It owns the
// process, SDL and the window, and runs one engine at a time: ra2-engine (Red Alert 2, built
// from src/recovered) or yr-engine (Yuri's Revenge, from the pinned ra2-decomp tree), each a
// bundle with the whole game and its own copy of the platform layer, exporting only
// ra2_engine_main (engine_api.h). The main menu's toggle ends the running game through its own
// Exit Game; this host then starts the other engine on the same window, without its start-up
// movies. The game chosen last is kept in the settings ("game=ra2|yr") and starts next time,
// with its movies.
//
// Each start of an engine maps a fresh copy of its bundle (a temporary file, unlinked once
// mapped): the game's globals start as the original's did, with its static initialisers. The
// ended engine's exit handlers run when it ends (its static destructors, Prog_End, the
// settings save); its image stays mapped (its threads may still be parked in it), never called.
//
// ra2yr-one (RA2_NATIVE_ONE_EXECUTABLE, lane Y8): the same host with both engines linked in (the
// design for iOS, where no bundle can be loaded): a start resets the game's own __data/__bss to
// the bytes they held before its first start and runs its initialisers again; an end runs the exit
// handlers registered by its code (its __text, bounded by the engine markers).
//
//   ra2yr-native [--game=ra2|yr] [--engine-dir=<dir>] [--engine-scripts=<s0>,<s1>,...] <the game's options...>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>      // iOS: SDL's UIKit start-up calls this file's main (ra2yr-one)
#if !RA2_NATIVE_ONE_EXECUTABLE
#include <copyfile.h>
#endif
#include <dlfcn.h>
#include <errno.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <map>
#include <string>
#include <vector>

#include "../../platform/sdl/engine_api.h"
#if RA2_NATIVE_ONE_EXECUTABLE
#include <mach-o/getsect.h>
#include <mach-o/ldsyms.h>
#endif

extern "C" {
// libSystem: the exit handlers registered by code in the given ranges (what dlclose runs).
struct __cxa_range_t { const void *addr; size_t length; };
void __cxa_finalize_ranges(const struct __cxa_range_t ranges[], unsigned int count);
}

namespace {

const char *game_key(int game) { return game == RA2_ENGINE_YR ? "yr" : "ra2"; }
const char *game_name(int game) { return game == RA2_ENGINE_YR ? "Yuri's Revenge" : "Red Alert 2"; }

// The settings file the engines' app mode uses (platform/sdl/app_mode.hpp settings_path).
std::string settings_path()
{
    const char *s = getenv("RA2_NATIVE_SETTINGS");
    if (s && *s) return s;
    const char *home = getenv("HOME");
    return std::string(home ? home : "") + "/Library/Application Support/RA2Native/settings";
}
std::string read_setting(const char *key)
{
    FILE *f = fopen(settings_path().c_str(), "r");
    if (!f) return "";
    char line[4096];
    std::string found;
    size_t n = strlen(key);
    while (fgets(line, sizeof line, f)) {
        if (!strncmp(line, key, n) && line[n] == '=') {
            found = line + n + 1;
            while (!found.empty() && (found.back() == '\n' || found.back() == '\r')) found.pop_back();
        }
    }
    fclose(f);
    return found;
}
// key=value, the file's other lines kept (the engines own them).
void write_setting(const char *key, const char *value)
{
    std::string path = settings_path();
    std::vector<std::string> lines;
    bool replaced = false;
    size_t n = strlen(key);
    if (FILE *f = fopen(path.c_str(), "r")) {
        char line[4096];
        while (fgets(line, sizeof line, f)) {
            std::string l = line;
            while (!l.empty() && (l.back() == '\n' || l.back() == '\r')) l.pop_back();
            if (!strncmp(l.c_str(), key, n) && l.size() > n && l[n] == '=') {
                l = std::string(key) + "=" + value;
                replaced = true;
            }
            lines.push_back(l);
        }
        fclose(f);
    } else {
        lines.push_back("# Red Alert 2 (native) settings: written by the app");
    }
    if (!replaced) lines.push_back(std::string(key) + "=" + value);
    size_t slash = path.rfind('/');
    if (slash != std::string::npos) {
        std::string dir = path.substr(0, slash);
        for (size_t i = 1; i <= dir.size(); ++i)
            if (i == dir.size() || dir[i] == '/') mkdir(dir.substr(0, i).c_str(), 0755);
    }
    std::string tmp = path + ".tmp";
    FILE *f = fopen(tmp.c_str(), "w");
    if (!f) return;
    for (auto const &l : lines) fprintf(f, "%s\n", l.c_str());
    fclose(f);
    rename(tmp.c_str(), path.c_str());
}

std::string executable_dir()
{
    char buf[4096];
    uint32_t size = sizeof buf;
    if (_NSGetExecutablePath(buf, &size) != 0) return ".";
    char real[PATH_MAX];
    std::string path = realpath(buf, real) ? real : buf;
    size_t slash = path.rfind('/');
    return slash == std::string::npos ? "." : path.substr(0, slash);
}

struct Engine {
    void *handle = nullptr;
    ra2_engine_main_fn main = nullptr;
    const void *base = nullptr;     // its Mach-O header
};

#if !RA2_NATIVE_ONE_EXECUTABLE
// A fresh image of the bundle: dyld maps a file once per process, so each start copies it.
bool load_engine(std::string const &dir, int game, int serial, Engine &out)
{
    std::string bundle = dir + "/" + (game == RA2_ENGINE_YR ? "yr-engine.so" : "ra2-engine.so");
    const char *tmpdir = getenv("TMPDIR");
    char copy[1024];
    snprintf(copy, sizeof copy, "%s/ra2yr-%d-%d-%s.so", tmpdir && *tmpdir ? tmpdir : "/tmp", (int)getpid(), serial, game_key(game));
    if (copyfile(bundle.c_str(), copy, nullptr, COPYFILE_DATA | COPYFILE_STAT) != 0) {
        fprintf(stderr, "one-game: cannot copy %s: %s\n", bundle.c_str(), strerror(errno));
        return false;
    }
    void *handle = dlopen(copy, RTLD_NOW | RTLD_LOCAL);
    unlink(copy);
    if (!handle) {
        fprintf(stderr, "one-game: %s: %s\n", bundle.c_str(), dlerror());
        return false;
    }
    auto entry = (ra2_engine_main_fn)dlsym(handle, "ra2_engine_main");
    Dl_info info = {};
    if (!entry || !dladdr((const void *)entry, &info)) {
        fprintf(stderr, "one-game: %s has no ra2_engine_main\n", bundle.c_str());
        return false;
    }
    out.handle = handle;
    out.main = entry;
    out.base = info.dli_fbase;
    return true;
}

// The ended engine's exit handlers (atexit and C++ static destructors registered by its code),
// as dlclose or exit would run them, now rather than at the process's exit over another game.
void finalize_engine(Engine const &e)
{
    const struct mach_header_64 *h = (const struct mach_header_64 *)e.base;
    std::vector<__cxa_range_t> ranges;
    const uint8_t *c = (const uint8_t *)(h + 1);
    intptr_t slide = 0;
    for (int pass = 0; pass < 2; ++pass) {
        c = (const uint8_t *)(h + 1);
        for (uint32_t i = 0; i < h->ncmds; ++i) {
            const struct load_command *lc = (const struct load_command *)c;
            if (lc->cmd == LC_SEGMENT_64) {
                const struct segment_command_64 *s = (const struct segment_command_64 *)c;
                if (pass == 0 && !strcmp(s->segname, "__TEXT")) slide = (intptr_t)((uintptr_t)h - s->vmaddr);
                if (pass == 1 && s->vmsize && strcmp(s->segname, "__LINKEDIT") && strcmp(s->segname, "__PAGEZERO"))
                    ranges.push_back({(const void *)(s->vmaddr + slide), (size_t)s->vmsize});
            }
            c += lc->cmdsize;
        }
    }
    __cxa_finalize_ranges(ranges.data(), (unsigned)ranges.size());
}
#endif


#if RA2_NATIVE_ONE_EXECUTABLE
// The engines linked in (yr/combined.cmake): their entries and their section markers.
extern "C" {
int ra2_engine_main_ra2(int, char **, struct ra2_engine_host *);
int ra2_engine_main_yr(int, char **, struct ra2_engine_host *);
extern int ra2_data_begin, ra2_data_end, ra2_bss_begin, ra2_bss_end, ra2_common_begin, ra2_common_end;
extern int yr_data_begin, yr_data_end, yr_bss_begin, yr_bss_end, yr_common_begin, yr_common_end;
void ra2_text_begin(void); void ra2_text_end(void);
void yr_text_begin(void); void yr_text_end(void);
}
struct Span { uint8_t *begin; uint8_t *end; std::vector<uint8_t> pristine; };
struct Linked {
    ra2_engine_main_fn main;
    const char *init_section;
    Span data, bss, common;
    const void *text_begin, *text_end;
    bool started = false;
};
Linked linked[2] = {
    {ra2_engine_main_ra2, "__ra2e_init", {(uint8_t *)&ra2_data_begin, (uint8_t *)(&ra2_data_end + 1), {}},
     {(uint8_t *)&ra2_bss_begin, (uint8_t *)(&ra2_bss_end + 1), {}},
     {(uint8_t *)&ra2_common_begin, (uint8_t *)(&ra2_common_end + 1), {}}, (const void *)ra2_text_begin, (const void *)ra2_text_end},
    {ra2_engine_main_yr, "__yre_init", {(uint8_t *)&yr_data_begin, (uint8_t *)(&yr_data_end + 1), {}},
     {(uint8_t *)&yr_bss_begin, (uint8_t *)(&yr_bss_end + 1), {}},
     {(uint8_t *)&yr_common_begin, (uint8_t *)(&yr_common_end + 1), {}}, (const void *)yr_text_begin, (const void *)yr_text_end},
};
// Before the game's first start its spans are as the image has them (dyld has bound them, no
// initialiser of the game has run): kept; each later start copies them back. Then its initialisers.
void start_linked(int game)
{
    Linked &l = linked[game];
    for (Span *s : {&l.data, &l.bss, &l.common}) {
        if (s->end <= s->begin) { fprintf(stderr, "one-game: %s: bad engine span\n", game_key(game)); abort(); }
        if (!l.started) s->pristine.assign(s->begin, s->end);
        else memcpy(s->begin, s->pristine.data(), s->pristine.size());
    }
    unsigned long size = 0;
    uint8_t *start = getsectiondata(&_mh_execute_header, "__DATA", l.init_section, &size);
    typedef void (*Initialiser)(void);
    for (unsigned long i = 0; i < size / sizeof(Initialiser); ++i) ((Initialiser *)start)[i]();
    fprintf(stderr, "one-game: %s engine %s: data %zu + bss %zu + common %zu bytes, %lu initialisers\n", game_key(game),
            l.started ? "reset" : "first start", l.data.pristine.size(), l.bss.pristine.size(), l.common.pristine.size(),
            size / sizeof(Initialiser));
    l.started = true;
}
void finalize_linked(int game)
{
    Linked const &l = linked[game];
    uintptr_t a = (uintptr_t)l.text_begin, b = (uintptr_t)l.text_end;
    __cxa_range_t range = {(const void *)(a < b ? a : b), (size_t)(a < b ? b - a : a - b) + 16};
    __cxa_finalize_ranges(&range, 1);
}
#endif
}  // namespace

int main(int argc, char **argv)
{
    int game = -1;
    std::string engine_dir = executable_dir();
    bool test_run = false;
    long run_seconds = 0;
    std::vector<std::string> scripts;
    std::vector<char *> args;
    args.push_back(argv[0]);
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--game=ra2")) game = RA2_ENGINE_RA2;
        else if (!strcmp(argv[i], "--game=yr")) game = RA2_ENGINE_YR;
        else if (!strncmp(argv[i], "--engine-dir=", 13)) engine_dir = argv[i] + 13;
        // Test runs: one input script per engine start, in order (the host reads its script
        // afresh in each engine: its lines count that engine's dialogs and frames).
        else if (!strncmp(argv[i], "--engine-scripts=", 17)) {
            std::string list = argv[i] + 17;
            for (size_t at = 0; at <= list.size();) {
                size_t comma = list.find(',', at);
                if (comma == std::string::npos) comma = list.size();
                scripts.push_back(list.substr(at, comma - at));
                at = comma + 1;
            }
        }
        else {
            if (!strncmp(argv[i], "--run-to=", 9)) test_run = true;
            if (!strncmp(argv[i], "--run-seconds=", 14)) run_seconds = atol(argv[i] + 14);
            args.push_back(argv[i]);
        }
    }
    args.push_back(nullptr);
    // The settings are the app's; a test run keeps its hands off the user's unless it names a file.
    bool persist = !test_run || (getenv("RA2_NATIVE_SETTINGS") && *getenv("RA2_NATIVE_SETTINGS"));
    if (game < 0) {
        std::string saved = persist ? read_setting("game") : "";
        game = saved == "yr" ? RA2_ENGINE_YR : RA2_ENGINE_RA2;
        fprintf(stderr, "one-game: starting %s (%s)\n", game_name(game), saved.empty() ? "default" : "the game chosen last");
    } else {
        fprintf(stderr, "one-game: starting %s (--game)\n", game_name(game));
    }
    if (persist) write_setting("game", game_key(game));
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    const char *dump = getenv("RA2_NATIVE_WINDOW_DUMP");
    std::string dump_prefix = dump ? dump : "";
    void *window = nullptr;
    bool first = true;
    Uint64 started_ms = SDL_GetTicks();
    for (int serial = 0;; ++serial) {
        Engine e;
#if RA2_NATIVE_ONE_EXECUTABLE
        (void)engine_dir;
        e.main = linked[game].main;     // (its reset and initialisers: below, once its environment is set)
#else
        if (!load_engine(engine_dir, game, serial, e)) return 71;
#endif
        ra2_engine_host host = {};
        host.version = RA2_ENGINE_API_VERSION;
        host.game = game;
        host.window = window;
        host.skip_intro = first ? 0 : 1;
        host.toggle_label = game == RA2_ENGINE_YR ? "Play Red Alert 2" : "Play Yuri's Revenge";
        if (!scripts.empty()) {
            std::string script = serial < (int)scripts.size() ? scripts[serial] : "";
            if (script.empty()) unsetenv("RA2_NATIVE_INPUT_SCRIPT");
            else setenv("RA2_NATIVE_INPUT_SCRIPT", script.c_str(), 1);
        }
        // Test runs' window dumps (RA2_NATIVE_WINDOW_DUMP=<prefix>): each engine numbers its own
        // from 0, so each start writes under <prefix>-e<n>.
        if (!dump_prefix.empty()) setenv("RA2_NATIVE_WINDOW_DUMP", (dump_prefix + "-e" + std::to_string(serial)).c_str(), 1);
        fprintf(stderr, "one-game: engine %d: %s%s\n", serial, game_name(game), first ? "" : " (after a swap: no start-up movies)");
        fflush(stderr);
        // A test run's time limit (--run-seconds) is the whole run's: each engine gets what is left.
        std::vector<char *> engine_args = args;
        char seconds_arg[64];
        if (run_seconds > 0) {
            long left = run_seconds - (long)((SDL_GetTicks() - started_ms) / 1000);
            snprintf(seconds_arg, sizeof seconds_arg, "--run-seconds=%ld", left < 1 ? 1 : left);
            for (char *&a : engine_args)
                if (a && !strncmp(a, "--run-seconds=", 14)) a = seconds_arg;
        }
#if RA2_NATIVE_ONE_EXECUTABLE
        // The game's data reset and initialisers run now, after this start's environment (input
        // script, window dump prefix) is set: initialisers read it, as at a process start.
        start_linked(game);
#endif
        int code = e.main((int)engine_args.size() - 1, engine_args.data(), &host);
        if (!host.swap) {
            fprintf(stderr, "one-game: %s ended (%d)\n", game_name(game), code);
            SDL_Quit();
            return code;
        }
        window = host.window;
#if RA2_NATIVE_ONE_EXECUTABLE
        finalize_linked(game);
#else
        finalize_engine(e);
#endif
        game = game == RA2_ENGINE_YR ? RA2_ENGINE_RA2 : RA2_ENGINE_YR;
        if (persist) write_setting("game", game_key(game));
        fprintf(stderr, "one-game: swap: %s next\n", game_name(game));
        first = false;
    }
}
