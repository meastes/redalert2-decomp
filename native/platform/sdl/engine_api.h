// SPDX-License-Identifier: GPL-3.0-or-later
// One game, two engines (lane Y5; native/yr/README.md "One game"): the C interface between the
// combined app's thin host (native/yr/combined/one_game.cpp) and an engine bundle (ra2-engine,
// yr-engine: a whole game, the SDL host compiled with RA2_NATIVE_ENGINE). The bundle exports
// only ra2_engine_main; everything else in it is private to it (ld64 -exported_symbol), so the
// two games' identical names (WinMain, MapClass, the port) never meet.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define RA2_ENGINE_API_VERSION 1

enum ra2_engine_game { RA2_ENGINE_RA2 = 0, RA2_ENGINE_YR = 1 };

struct ra2_engine_host {
    int version;              // RA2_ENGINE_API_VERSION
    int game;                 // ra2_engine_game this engine is (checked against the bundle)
    void *window;             // in: the SDL_Window the previous engine used (null: make one);
                              // out: the window this engine leaves for the next
    int skip_intro;           // 1: a swap: no start-up movies (the logos, the intro)
    const char *toggle_label; // the main menu's toggle text: the other game's name
    int swap;                 // out: 1 when the engine ended for the toggle (the other game next)
};

// Runs the whole game (the SDL host's main) and returns its exit code. The engine leaves the
// window and SDL running; its renderer, audio stream and event watch are gone when it returns.
int ra2_engine_main(int argc, char **argv, struct ra2_engine_host *host);
typedef int (*ra2_engine_main_fn)(int argc, char **argv, struct ra2_engine_host *host);

#ifdef __cplusplus
}
#endif
