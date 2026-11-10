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
#include <stdio.h>
#include "ra2port/win32_port.hpp"

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
// The Mac app mode needs the game (host.cpp is linked with it only).
#if defined(__APPLE__) && defined(RA2_NATIVE_GAME)
#define RA2_APP_MODE 1
#include "app_mode.hpp"
#endif

#ifdef RA2_APP_MODE
namespace {
app::Settings app_settings;
// The fullscreen state is remembered for the next start, however the game ends
// (WinMain returning, the game's own exit, the host's quit fallback).
void save_app_settings()
{
    app_settings.values["fullscreen"] = ra2_sdl_host_fullscreen() ? "1" : "0";
    app_settings.save();
}
}
#endif

int main(int argc, char **argv)
{
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
        if (!app::setup(app_settings, force, folder, exe)) {
            fprintf(stderr, "app: setup cancelled\n");
            SDL_Quit();
            return 0;
        }
        static std::string data_dir_kept;
        data_dir_kept = folder;
        data = data_dir_kept.c_str();
        setenv("RA2_GAME_EXE", exe.c_str(), 1);
        fprintf(stderr, "app: game folder %s, game.exe %s\n", folder.c_str(), exe.c_str());
        // Writes (RA2.INI, saved games) beside the settings, never into the installation.
        if (!write_dir) {
            static std::string game_writes;
            game_writes = app::support_dir() + "/Game";
            app::make_dirs(game_writes);
            ra2port::set_write_dir(game_writes.c_str());
            fprintf(stderr, "app: writes go to %s\n", game_writes.c_str());
        }
        ra2_sdl_host_options(true, app_settings.get("fullscreen") == "1", save_app_settings);
        atexit(save_app_settings);
        // lan=1 in the settings file turns on the Network (LAN) button, as RA2_NATIVE_LAN=1 does
        // for a test run: a Finder-launched app has no environment of its own (packaging/README.md).
        if (app_settings.get("lan") == "1" && !getenv("RA2_NATIVE_LAN")) setenv("RA2_NATIVE_LAN", "1", 1);
    }
#endif
    if (data) ra2port::set_data_dir(data);
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
