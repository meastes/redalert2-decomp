// SPDX-License-Identifier: GPL-3.0-or-later
// Native host entry (SDL3, as historical/native-reconstruction chose). It owns
// the process, the window and the event pump, then will hand control to the
// recovered WinMain (0x0068E280) once the tree links; the recovered code keeps
// its own main loop and calls back into the platform through the Win32 /
// DirectDraw / DirectSound seams in native/port. Until WinMain links, this
// opens the 800x600 window the original creates and pumps events, so the
// platform layer can be exercised alone.
//
// Game data comes from the user's installation at run time (--data-dir or
// RA2_DATA_DIR); nothing from it is built in.
#include <SDL3/SDL.h>
// On iOS SDL's own main starts UIKit and calls this file's main (as SDL_main) on
// the main thread; elsewhere main stays the program's entry (Scout1247). On
// Windows SDL would supply WinMain, the recovered game's own entry point: main
// stays the entry there too.
#ifdef _WIN32
#define SDL_MAIN_HANDLED 1
#endif
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif
#include "ra2port/win32_port.hpp"
#include "mod_import.hpp"
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <algorithm>

#ifdef RA2_NATIVE_GAME
// The recovered entry point (0x0068E280). Not yet in the tree: with
// RA2_NATIVE_LINK_CLOSURE it resolves to a trap that names it.
struct HINSTANCE__;
extern "C" char const launch_mutex_name_007d515c[];
extern char const launcher_event_007d51f0[];
extern char const launcher_expected_message_007d52b8[];   // 0x007D52B8 (game.exe data, loaded at run time)
extern "C" void *CreateMutexA(void *security, int owned, const char *name);
extern "C" void *CreateEventA(void *security, int manual, int initial, const char *name);
// C linkage, as the original's entry and the draft declare it.
extern "C" int WinMain(HINSTANCE__ *instance, HINSTANCE__ *previous, char *command_line, int show);
// The data the original stores, read from the installed game.exe at run time
// (native/data/original_data.cpp, port/src/original_image.cpp).
struct ra2_original_global;
struct ra2_original_place;
extern "C" const ra2_original_global ra2_original_globals[];
extern "C" const ra2_original_place ra2_original_storage[];
namespace ra2port { int load_original_data(const ra2_original_global *table, const ra2_original_place *places); }
// Records with layouts original_data.py leaves out, filled after it (link/behaviour_m23.cpp).
void ra2_native_original_records();
#endif
void ra2_install_sdl_host();
#ifdef RA2_NATIVE_GAME
void ra2_sdl_host_options(bool app, bool fullscreen, void (*on_quit)());
bool ra2_sdl_host_fullscreen();
void ra2_sdl_host_use_window(SDL_Window *window);
void ra2_sdl_host_touch_options(int checkpoint_seconds);
#endif
extern "C" int ra2port_initterm_last(int *units);
extern "C" void ra2port_run_to(const char *target, int seconds);
extern "C" void ra2port_run_end(const char *why);
extern "C" void ra2port_set_skirmish(const char *map, int frames);
extern "C" void ra2port_set_skirmish_order(const char *order);
extern "C" void ra2port_set_frames(int frames);
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
// The app mode needs the game (host.cpp is linked with it only). Its platform
// services come from host_os.hpp (Scout1247).
#if defined(RA2_NATIVE_GAME)
#define RA2_APP_MODE 1
#include "app_mode.hpp"
#endif

#ifdef RA2_APP_MODE
namespace {
app::Settings app_settings;
SDL_Window *app_window = nullptr;     // iOS: the window made before the game (setup_imported)
// The fullscreen state is remembered for the next start, however the game ends
// (WinMain returning, the game's own exit, the host's quit fallback).
void save_app_settings()
{
    app_settings.values["fullscreen"] = ra2_sdl_host_fullscreen() ? "1" : "0";
    app_settings.save();
}
// The enhancements' settings (platform/sdl/enhancements.cpp): enh_* keys of this file,
// never RA2.INI.
std::string enhancement_get(const char *key) { return app_settings.get(key); }
// Mods (lane C16, level 2): the game folder (for the Mods folder in it), this process's arguments
// (a restart passes them on without --mod: the saved choice is used) and a restart asked for.
std::string mods_data_dir;
std::vector<std::string> restart_args;
bool restart_wanted = false;
std::vector<std::string> app_mods();
void restart_if_wanted()
{
    if (!restart_wanted) return;
    restart_wanted = false;
    char self[2048];
    snprintf(self, sizeof self, "%s", app::exe_path().c_str());
    // RA2_NATIVE_NO_RELAUNCH=1 (tests): the choice is saved and the app ends, nothing started.
    if (getenv("RA2_NATIVE_NO_RELAUNCH")) {
        fprintf(stderr, "app: restarting for the mod: not started (RA2_NATIVE_NO_RELAUNCH)\n");
        fflush(stderr);
        return;
    }
    bool ok = hostos::relaunch(self, restart_args);
    fprintf(stderr, "app: restarting for the mod: %s\n", ok ? "started" : "could not start");
    fflush(stderr);
}
// The host's quit (_exit after this, no atexit handlers): the settings saved, then a restart asked for.
void save_and_restart()
{
    save_app_settings();
    restart_if_wanted();
}
// The Mod row's choice: saved, then the app starts again with it (where it can; iOS: at the next start).
void switch_mod(const char *name)
{
    app_settings.values["mod"] = name ? name : "";
    app_settings.save();
    if (hostos::relaunch_supported()) {
        restart_wanted = true;
        SDL_Event quit = {};
        quit.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quit);
        fprintf(stderr, "app: mod %s chosen: restarting\n", name && *name ? name : "none");
    } else {
        fprintf(stderr, "app: mod %s chosen: used at the next start\n", name && *name ? name : "none");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Mod",
                                 "The mod you chose is used the next time you open Red Alert 2.", app_window);
    }
}
void enhancement_set(const char *key, const char *value)
{
    app_settings.values[key] = value;
    app_settings.save();
}
}
#endif
#ifdef RA2_NATIVE_GAME
#include <string>
extern "C" int ra2_enh_sidebar_height(int screen_height);   // platform/sdl/enhancements.cpp
void ra2_enhancements_store(std::string (*get)(const char *), void (*set)(const char *, const char *));
void ra2_enhancements_mods(std::vector<std::string> (*list)(), const char *running, void (*switch_to)(const char *),
                           bool restarts);
void ra2_enhancements_mod_info(bool (*info)(const char *folder, std::string *name, std::string *description));
#endif

#ifdef RA2_NATIVE_GAME
extern "C" int ra2_native_draw_selftest(void);   // link/selftest_draw.cpp (lane C17, tests)
extern "C" int ra2_native_vqa_selftest(void);    // link/selftest_vqa.cpp (lane C16, tests)
#endif
// The folder of a mod named on the command line (lane C16): the name as a folder, else
// Mods/<name> in the app's support folder (the app) or in the game folder, folder names in any
// case; "" when there is none.
static bool is_dir(const std::string &p)
{
    struct stat st;
    return stat(p.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}
static std::string child_any_case(const std::string &dir, const std::string &name)
{
    if (is_dir(dir + "/" + name)) return dir + "/" + name;
    if (DIR *d = opendir(dir.c_str())) {
        while (struct dirent *e = readdir(d)) {
            if (!strcasecmp(e->d_name, name.c_str()) && is_dir(dir + "/" + e->d_name)) {
                std::string found = dir + "/" + e->d_name;
                closedir(d);
                return found;
            }
        }
        closedir(d);
    }
    return std::string();
}
// The mods to choose from (lane C16, level 2: the Enhancements row "Mod"): the folders in Mods in
// the app's support folder and in the game folder, by name (the first spelling of a name), sorted.
static std::vector<std::string> mods_in(const std::vector<std::string> &roots)
{
    std::vector<std::string> names;
    for (const std::string &root : roots) {
        std::string mods = child_any_case(root, "Mods");
        DIR *d = mods.empty() ? nullptr : opendir(mods.c_str());
        if (!d) continue;
        while (struct dirent *e = readdir(d)) {
            if (e->d_name[0] == '.' || !is_dir(mods + "/" + e->d_name)) continue;
            bool listed = false;
            for (const std::string &n : names) listed = listed || !strcasecmp(n.c_str(), e->d_name);
            if (!listed) names.push_back(e->d_name);
        }
        closedir(d);
    }
    std::sort(names.begin(), names.end(), [](const std::string &a, const std::string &b) { return strcasecmp(a.c_str(), b.c_str()) < 0; });
    return names;
}
// The folders holding a Mods folder (lane C16, level 3): the app's support folder and, on iOS, its
// Documents (the Files app shows Documents/Mods), then the game folder.
static std::vector<std::string> mod_roots(const char *data)
{
    std::vector<std::string> roots;
#ifdef RA2_APP_MODE
    roots.push_back(app::support_dir());
    if (hostos::imports_game_files()) {
        std::string writes = app::game_write_dir();
        size_t slash = writes.find_last_of('/');
        if (slash != std::string::npos) roots.push_back(writes.substr(0, slash));
    }
#endif
    if (data) roots.push_back(data);
    return roots;
}
static std::string find_mod(const char *mod, const char *data)
{
    std::string m = mod;
    if (m.find('/') != std::string::npos || m.find('\\') != std::string::npos) return is_dir(m) ? m : std::string();
    std::vector<std::string> roots = mod_roots(data);
    for (const std::string &root : roots) {
        std::string mods = child_any_case(root, "Mods");
        if (mods.empty()) continue;
        std::string found = child_any_case(mods, m);
        if (!found.empty()) return found;
    }
    return std::string();
}

#ifdef RA2_APP_MODE
namespace {
std::vector<std::string> app_mods()
{
    return mods_in(mod_roots(mods_data_dir.empty() ? nullptr : mods_data_dir.c_str()));
}
// The Mod row's name and description of a mod folder (its mod.ini, lane C16 level 3).
bool app_mod_info(const char *folder, std::string *name, std::string *description)
{
    std::string found = find_mod(folder, mods_data_dir.empty() ? nullptr : mods_data_dir.c_str());
    return !found.empty() && mods::manifest(found, name, description);
}
}
#endif

int main(int argc, char **argv)
{
#ifdef RA2_NATIVE_GAME
    if (getenv("RA2_NATIVE_DRAW_SELFTEST")) return ra2_native_draw_selftest();
    if (getenv("RA2_NATIVE_VQA_SELFTEST")) return ra2_native_vqa_selftest();
#endif
#ifdef RA2_APP_MODE
    // The crash dialog of RA2.app (app_mode.hpp): a fresh process.
    if (argc == 3 && !strcmp(argv[1], "--crash-report")) return app::crash_report(argv[2]);
#endif
    const char *data = getenv("RA2_DATA_DIR");
    bool smoke = false;
    // Test runs (README "Start-up test runs"): --run-to=<milestone> starts the
    // game, logs each start-up milestone and ends by itself at that milestone,
    // the first unrecovered function, a fault or --run-seconds (default 5).
    // The installation is never written: writes go to --write-dir (default a
    // fresh directory under $TMPDIR). Dialogs and message boxes are never shown.
    const char *run_to = nullptr;
    const char *write_dir = nullptr;
    int run_seconds = 5;
    // Skirmish harness (README "Skirmish harness"): Main_Game starts a skirmish
    // on <map> without the menu dialogs and runs N Main_Loop frames.
    const char *skirmish = nullptr;
    int skirmish_frames = 1;
    // The game's own command line (WinMain's lpCmdLine, e.g. "-noaudio").
    const char *game_args = "";
    // The native game skips the CD check (user decision 2026-11-08): unless the
    // game's command line names a CD path itself (-CD<path>) or --cd-check asks
    // for the original check, it gets Parse_Command_Line's -CD. (every file is
    // read from the installation; CD::Force_Available and Get_CD_Index pass).
    bool cd_check = false;
    // --play=<map>: an interactive skirmish on <map> (as --skirmish names it).
    const char *play = nullptr;
    // Mac app mode (README "Mac app"): RA2.app launched from Finder, or --app.
    // First-run setup, the settings, the log file and the crash dialog.
    bool app_flag = false, setup_flag = false;
    const char *mod = getenv("RA2_NATIVE_MOD");
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--data-dir") && i + 1 < argc) data = argv[++i];
        else if (!strcmp(argv[i], "--smoke-test")) smoke = true;
        else if (!strncmp(argv[i], "--run-to=", 9)) run_to = argv[i] + 9;
        else if (!strncmp(argv[i], "--run-seconds=", 14)) run_seconds = atoi(argv[i] + 14);
        else if (!strncmp(argv[i], "--write-dir=", 12)) write_dir = argv[i] + 12;
        else if (!strncmp(argv[i], "--skirmish=", 11)) skirmish = argv[i] + 11;
        else if (!strncmp(argv[i], "--game-args=", 12)) game_args = argv[i] + 12;
        else if (!strncmp(argv[i], "--skirmish-frames=", 18)) skirmish_frames = atoi(argv[i] + 18);
        else if (!strncmp(argv[i], "--skirmish-order=", 17)) ra2port_set_skirmish_order(argv[i] + 17);
        else if (!strcmp(argv[i], "--cd-check")) cd_check = true;
        else if (!strncmp(argv[i], "--play=", 7)) play = argv[i] + 7;
        else if (!strncmp(argv[i], "--frames=", 9)) ra2port_set_frames(atoi(argv[i] + 9));
        else if (!strncmp(argv[i], "--input-script=", 15)) setenv("RA2_NATIVE_INPUT_SCRIPT", argv[i] + 15, 1);
        else if (!strcmp(argv[i], "--app")) app_flag = true;
        else if (!strcmp(argv[i], "--setup")) setup_flag = true;
        // --load-save=<file> (lane C15, a test run's): the menus go to Load Saved Game and
        // load <file> as Resume at start does (ra2port::request_resume); with
        // --run-to=frames --frames=N the run ends N Main_Loop frames into the loaded game.
        else if (!strncmp(argv[i], "--load-save=", 12)) ra2port::request_resume(argv[i] + 12);
        // --mod=<name or folder> (lane C16, mod support level 1; RA2_NATIVE_MOD the same): the
        // mod's folder is read ahead of the installation (ra2port::set_mod_dir).
        else if (!strncmp(argv[i], "--mod=", 6)) mod = argv[i] + 6;
#ifdef RA2_APP_MODE
        // --after-pid=<pid> (lane C16, mods level 2): a restart for a new mod waits for the app
        // that started it to end (hostos::relaunch).
        else if (!strncmp(argv[i], "--after-pid=", 12)) hostos::wait_for_process(atol(argv[i] + 12));
        if (strncmp(argv[i], "--mod=", 6) && strncmp(argv[i], "--after-pid=", 12)) restart_args.push_back(argv[i]);
#endif
    }
#ifdef RA2_APP_MODE
    bool app_mode = app_flag || (app::in_bundle() && !run_to && !skirmish && !smoke);
    if (app_mode && (run_to || skirmish || smoke)) { fprintf(stderr, "--app is the interactive game: not with --run-to, --skirmish or --smoke-test\n"); return 64; }
    if (app_mode) {
        app::install_crash_handler();
        if (!getenv("RA2_NATIVE_LOG_STDERR")) {
            fprintf(stderr, "app: logging to %s/ra2-native.log\n", app::log_dir().c_str());
            app::open_log();
        }
        SDL_SetAppMetadata("Red Alert 2", nullptr, "io.github.ra2native.RA2");
        // Control-click stays a left click with Control held: RA2's force fire is Ctrl+click
        // (Alt+click force move, Shift+click add to selection).
        // The macOS Control-click-is-right-click emulation turned force fire into a right click,
        // which deselects (Scout1209 r13); a trackpad's right click is the two-finger click.
    }
#else
    bool app_mode = false;
    (void)app_flag;
    (void)setup_flag;
#endif
    if (skirmish && !run_to) { fprintf(stderr, "--skirmish is a test harness: use it with --run-to=<milestone> (e.g. frame)\n"); return 64; }
    if (play && (skirmish || run_to)) { fprintf(stderr, "--play is interactive: not with --skirmish or --run-to\n"); return 64; }
    if (skirmish) ra2port_set_skirmish(skirmish, skirmish_frames);
    // Interactive skirmish (README "Playing"): the harness's start (no menu
    // dialogs yet), no frame limit, no scripted orders; the player's mouse
    // and keyboard drive the game until it is closed.
    if (play) ra2port_set_skirmish(play, 0);
    static char write_template[1024];
    if (run_to && !write_dir) {
        const char *tmp = getenv("TMPDIR");
        snprintf(write_template, sizeof write_template, "%s/ra2-native-run-XXXXXX", tmp && *tmp ? tmp : "/tmp");
        write_dir = mkdtemp(write_template);
    }
    if (write_dir) {
        mkdir(write_dir, 0755);     // a named --write-dir may not exist yet (writes below it create their parents)
        ra2port::set_write_dir(write_dir);
        fprintf(stderr, "run-to: writes go to %s\n", write_dir);
    }
    if (run_to) {
        if (smoke) { fprintf(stderr, "--run-to and --smoke-test exclude each other\n"); return 64; }
        if (!data) { fprintf(stderr, "--run-to needs --data-dir or RA2_DATA_DIR (the game installation, read only)\n"); return 64; }
        setenv("RA2_PORT_QUIET", "1", 0);
        // Test runs play into SDL's dummy device (consumed in real time, silent)
        // unless SDL_AUDIO_DRIVER names one.
        setenv("SDL_AUDIO_DRIVER", "dummy", 0);
        ra2port_run_to(run_to, run_seconds);
    }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
#ifdef RA2_APP_MODE
    if (app_mode) {
        // The installation and the supported game.exe: saved, or asked for.
        app_settings.load();
        std::string folder, exe;
        bool force = setup_flag || (getenv("RA2_NATIVE_SETUP") && *getenv("RA2_NATIVE_SETUP") == '1');
        if (data && !force) app_settings.values["data_dir"] = data;    // --data-dir / RA2_DATA_DIR win
        bool ready;
        if (hostos::imports_game_files()) {
            // iOS (Scout1247): the game's window first (the picker and the messages are shown
            // over it); the game's CreateWindowExA takes it over.
            SDL_Window *window = SDL_CreateWindow("Red Alert 2", 800, 600, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIGH_PIXEL_DENSITY);
            app_window = window;
            if (!window) fprintf(stderr, "app: window: %s\n", SDL_GetError());
            ra2_sdl_host_use_window(window);
            void *native = window ? SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER, nullptr) : nullptr;
            ready = app::setup_imported(native, force, folder, exe);
        } else {
            ready = app::setup(app_settings, force, folder, exe);
        }
        if (!ready) {
            fprintf(stderr, "app: setup cancelled\n");
            SDL_Quit();
            return 0;
        }
        static std::string data_dir_kept;
        data_dir_kept = folder;
        data = data_dir_kept.c_str();
        setenv("RA2_GAME_EXE", exe.c_str(), 1);
        fprintf(stderr, "app: game folder %s, game.exe %s\n", folder.c_str(), exe.c_str());
        // Mods (lane C16, level 2): without --mod or RA2_NATIVE_MOD, the one chosen on the
        // Enhancements page (the settings' "mod"); its saves and RA2.INI have their own folder.
        mods_data_dir = folder;
        // Level 3 (lane C16): on iOS the Files app's Documents/Mods is made, and a .zip put in a
        // Mods folder is unpacked into a folder of its name.
        if (hostos::imports_game_files()) {
            std::string documents = mod_roots(nullptr).back() + "/Mods";
            app::make_dirs(documents);
        }
        for (const std::string &root : mod_roots(data)) {
            std::string dir = child_any_case(root, "Mods");
            if (!dir.empty()) mods::import_zips(dir);
        }
        static std::string saved_mod, mod_found, mod_name;
        if (!mod || !*mod) {
            saved_mod = app_settings.get("mod");
            mod = saved_mod.c_str();
        }
        if (mod && *mod) {
            mod_found = find_mod(mod, data);
            if (mod_found.empty()) {
                fprintf(stderr, "mod: %s not found (a folder, or Mods/%s in the app's folder or the game folder); none used\n", mod, mod);
            } else {
                size_t slash = mod_found.find_last_of('/');
                mod_name = slash == std::string::npos ? mod_found : mod_found.substr(slash + 1);
                ra2port::set_mod_dir(mod_found.c_str());
                fprintf(stderr, "mod: %s from %s\n", mod_name.c_str(), mod_found.c_str());
            }
            mod = nullptr;      // resolved here
        }
        ra2_enhancements_mods(app_mods, mod_name.c_str(), switch_mod, hostos::relaunch_supported());
        ra2_enhancements_mod_info(app_mod_info);
        atexit(restart_if_wanted);
        // Writes (RA2.INI, saved games) beside the settings, never into the installation; a
        // mod's in Mods/<name> below them.
        if (!write_dir) {
            static std::string game_writes;
            game_writes = app::game_write_dir();
            if (!mod_name.empty()) game_writes += "/Mods/" + mod_name;
            app::make_dirs(game_writes);
            ra2port::set_write_dir(game_writes.c_str());
            fprintf(stderr, "app: writes go to %s\n", game_writes.c_str());
        }
        ra2_sdl_host_options(true, app_settings.get("fullscreen") == "1", save_and_restart);
        ra2_enhancements_store(enhancement_get, enhancement_set);
        atexit(save_app_settings);
        if (hostos::imports_game_files() && app_window) {
            // The touch app (Scout1248): the in-game resolution from the screen's size
            // (720 lines on a large screen, 600 on a phone-sized one: lane C14); checkpoints in the background and every 3 minutes. (The toolbar's
            // interface scale, "ui_scale", is gone (lane C10): Game interface size on the
            // Enhancements page sizes the interface; a scale saved before is not applied.)
            for (int i = 0; i < 10; ++i) SDL_PumpEvents();     // the safe area settles with the window
            void *native = SDL_GetPointerProperty(SDL_GetWindowProperties(app_window), SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER, nullptr);
            hostos::Rect r;
            int ww = 0, wh = 0;
            SDL_GetWindowSize(app_window, &ww, &wh);
            if (!native || !hostos::usable_rect(native, &r)) r = hostos::Rect{0, 0, (double)ww, (double)wh};
            if (r.w < r.h) std::swap(r.w, r.h);     // started upright or folded: the landscape inner screen
            // Tests (Scout1274): RA2_NATIVE_USABLE_POINTS=<w>x<h> picks the resolution as if that
            // much were clear (the Duo's 944x634 on a Simulator that reports 594x432).
            if (const char *v = getenv("RA2_NATIVE_USABLE_POINTS")) {
                double fw = 0, fh = 0;
                if (sscanf(v, "%lfx%lf", &fw, &fh) == 2 && fw > 0 && fh > 0) r.w = fw, r.h = fh;
            }
            int gw = 0, gh = 0;
            app::game_resolution(r.w, r.h, gw, gh);
            // Game interface size (enhancements, C3): a sidebar column shown larger than the
            // map takes more of the width, so the game's width gives that back (never below 800).
            {
                int sidebar = ra2_enh_sidebar_height(gh);
                if (sidebar > 0 && sidebar < gh) {
                    double ratio = (double)gh / sidebar;
                    int narrower = ((int)(gw - 168 * (ratio - 1) + 0.5)) & ~7;
                    gw = narrower < 800 ? 800 : narrower;
                }
            }
            fprintf(stderr, "app: usable %.0fx%.0f points (window %dx%d): %s screen, play %dx%d\n", r.w, r.h, ww, wh,
                    app::large_screen(r.w, r.h) ? "large" : "phone-sized", gw, gh);
            app::set_ini_resolution(ra2port::write_dir() ? ra2port::write_dir() : app::game_write_dir().c_str(), folder, gw, gh);
            ra2_sdl_host_touch_options(180);
            // Resume at start (lane I): the system ended the app in the background after a
            // checkpoint (host.cpp's marker). Offer it once; Resume goes through the menus to
            // the load (ra2port::request_resume). RA2_NATIVE_RESUME_ANSWER=resume|menu
            // answers for tests.
            std::string marker = hostos::settings_dir() + "/resume.pending";
            std::string checkpoint = std::string(ra2port::write_dir() ? ra2port::write_dir() : app::game_write_dir().c_str()) + "/CHECKPT.SAV";
            FILE *found = fopen(marker.c_str(), "r");
            if (found) {
                fclose(found);
                remove(marker.c_str());
                FILE *save = fopen(checkpoint.c_str(), "rb");
                if (save) {
                    fclose(save);
                    int answer = 0;
                    if (const char *a = getenv("RA2_NATIVE_RESUME_ANSWER")) {
                        answer = !strcmp(a, "resume") ? 1 : 0;
                    } else {
                        const SDL_MessageBoxButtonData buttons[] = {
                            {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Main Menu"},
                            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Resume"},
                        };
                        const SDL_MessageBoxData box = {SDL_MESSAGEBOX_INFORMATION, app_window, "Resume your game?",
                            "Red Alert 2 was closed while a game was running. Resume from the checkpoint saved then?",
                            2, buttons, nullptr};
                        if (!SDL_ShowMessageBox(&box, &answer)) answer = 0;
                    }
                    fprintf(stderr, "app: resume offered: %s\n", answer == 1 ? "resume" : "main menu");
                    if (answer == 1) ra2port::request_resume("CHECKPT.SAV");
                }
            }
        }
        // The Network (LAN) button opens the LAN lobby by default (lane C17w); lan=0 in the settings
        // file turns it off, as RA2_NATIVE_LAN=0 does for a test run: a Finder-launched app has no
        // environment of its own (packaging/README.md). lan=1, the old switch, changes nothing.
        if (app_settings.get("lan") == "0" && !getenv("RA2_NATIVE_LAN")) setenv("RA2_NATIVE_LAN", "0", 1);
        // save_format=original in the settings file writes saves in the original game's format
        // (lane C17; RA2_NATIVE_SAVE_FORMAT, storage.cpp), as the environment does for a test run:
        // the iOS app has no environment. A temporary switch until that format is the default.
        if (app_settings.get("save_format") == "original" && !getenv("RA2_NATIVE_SAVE_FORMAT")) {
            setenv("RA2_NATIVE_SAVE_FORMAT", "original", 1);
            fprintf(stderr, "app: saves are written in the original game's format (save_format=original)\n");
        }
    }
#endif
    if (data) ra2port::set_data_dir(data);
    if (mod && *mod) {
        // A mod (lane C16): a folder given as it is, else Mods/<name> beside the app's settings
        // (the app) or in the installation (any case for the folder names).
        std::string found = find_mod(mod, data);
        if (found.empty()) {
            fprintf(stderr, "mod: %s not found (a folder, or Mods/%s in the app's folder or the game folder); none used\n", mod, mod);
        } else {
            ra2port::set_mod_dir(found.c_str());
            fprintf(stderr, "mod: %s from %s\n", mod, found.c_str());
        }
    }
#ifdef RA2_NATIVE_GAME
    {
        // Every static initialiser has run when main is reached.
        int units = 0;
        int last = ra2port_initterm_last(&units);
        printf("initterm: main reached; %d initialiser units ran, the last starting at original entry #%d\n",
               units, last);
    }
    if (!smoke && data) {
        // Before WinMain: the globals the original stores in game.exe (file
        // names, tables, constants). The static initialisers ran before main
        // and saw them zero.
        ra2port::load_original_data(ra2_original_globals, ra2_original_storage);
        ra2_native_original_records();
    }
    if (!smoke) {
        // Hand over to the game: it creates its window through CreateWindowExA
        // (the host hook opens the SDL window) and runs its own message loop.
        ra2_install_sdl_host();
        static char command_line[1024];
        bool has_cd = false;
        for (const char *p = game_args; *p; ++p) {
            if ((p == game_args || p[-1] == ' ') && p[0] == '-' && (p[1] == 'C' || p[1] == 'c') && (p[2] == 'D' || p[2] == 'd')) has_cd = true;
        }
        snprintf(command_line, sizeof command_line, "%s%s%s", game_args,
                 cd_check || has_cd ? "" : (*game_args ? " " : ""), cd_check || has_cd ? "" : "-CD.");
        // The native game is its own launcher: RA2.EXE created this mutex and
        // event before starting GAME.EXE, which checks the first (0x004918F0)
        // and signals the second (0x00491930). Same names (data/startup_winmain.cpp).
        CreateMutexA(nullptr, 0, launch_mutex_name_007d515c);
        CreateEventA(nullptr, 0, 0, launcher_event_007d51f0);
        // ...and answered the event with its message (0xBEEF), the text
        // Validate_Launcher 0x00491A10 expects (ra2port::set_launcher).
        ra2port::set_launcher(launcher_event_007d51f0, launcher_expected_message_007d52b8);
        ra2port::progress("main");
        ra2port::progress("winmain");
        int code = WinMain((HINSTANCE__ *)(uintptr_t)0x400000, nullptr, command_line, 1);
        {
            char why[64];
            snprintf(why, sizeof why, "WinMain returned %d", code);
            ra2port_run_end(why);
        }
        SDL_Quit();
#if defined(__APPLE__) && TARGET_OS_IPHONE
        // iOS (Scout1281; user report: Exit Game left a black screen). SDL's UIKit start-up
        // calls this main and, when it returns, keeps the UIKit run loop going without
        // ending the process (SDL_uikitappdelegate: the app may only have set up in main),
        // so the window stayed black. The main menu's Exit Game ends WinMain as on Windows;
        // the process then ends here, as the Outpost 2 port's Exit does (its callbacks
        // return SDL_APP_SUCCESS and SDL ends the app). exit() runs the atexit handlers
        // (save_app_settings). The Mac's main returns and the process ends as before.
        fprintf(stderr, "app: WinMain returned %d: exit\n", code);
        fflush(stderr);
        exit(code);
#endif
        return code;
    }
#endif
    SDL_Window *window = SDL_CreateWindow("Red Alert 2 (native)", 800, 600, SDL_WINDOW_HIGH_PIXEL_DENSITY);
    SDL_Renderer *renderer = window ? SDL_CreateRenderer(window, nullptr) : nullptr;
    if (!renderer) {
        fprintf(stderr, "window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    printf("data directory: %s\n", data ? data : "(none: pass --data-dir or set RA2_DATA_DIR)");
    bool running = true;
    int frames = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
        if (smoke && ++frames == 10) running = false;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
