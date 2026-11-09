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
#endif
void ra2_install_sdl_host();
extern "C" int ra2port_initterm_last(int *units);
extern "C" void ra2port_run_to(const char *target, int seconds);
extern "C" void ra2port_run_end(const char *why);
extern "C" void ra2port_set_skirmish(const char *map, int frames);
extern "C" void ra2port_set_skirmish_order(const char *order);
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
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
    }
    if (skirmish && !run_to) { fprintf(stderr, "--skirmish is a test harness: use it with --run-to=<milestone> (e.g. frame)\n"); return 64; }
    if (skirmish) ra2port_set_skirmish(skirmish, skirmish_frames);
    static char write_template[1024];
    if (run_to && !write_dir) {
        const char *tmp = getenv("TMPDIR");
        snprintf(write_template, sizeof write_template, "%s/ra2-native-run-XXXXXX", tmp && *tmp ? tmp : "/tmp");
        write_dir = mkdtemp(write_template);
    }
    if (write_dir) {
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
