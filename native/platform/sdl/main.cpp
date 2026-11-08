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
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    const char *data = getenv("RA2_DATA_DIR");
    bool smoke = false;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--data-dir") && i + 1 < argc) data = argv[++i];
        else if (!strcmp(argv[i], "--smoke-test")) smoke = true;
    }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
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
