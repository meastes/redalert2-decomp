// SPDX-License-Identifier: GPL-3.0-or-later
// The SDL3 side of the Win32 seam (ra2port::Host): the game's main window, the
// event pump that turns SDL input into the Win32 messages the recovered window
// procedure expects, and presentation of the DirectDraw primary surface.
#include <SDL3/SDL.h>

#include "ra2port/win32_port.hpp"

namespace ra2port { void set_key_state(int vk, bool down); }

namespace {
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;
int tex_w = 0, tex_h = 0;

// SDL keycodes to Win32 virtual keys (the subset the game binds).
int virtual_key(SDL_Keycode k)
{
    if (k >= SDLK_A && k <= SDLK_Z) return 'A' + (int)(k - SDLK_A);
    if (k >= SDLK_0 && k <= SDLK_9) return '0' + (int)(k - SDLK_0);
    if (k >= SDLK_F1 && k <= SDLK_F12) return 0x70 + (int)(k - SDLK_F1);
    switch (k) {
    case SDLK_BACKSPACE: return 0x08; case SDLK_TAB: return 0x09; case SDLK_RETURN: return 0x0D;
    case SDLK_LSHIFT: case SDLK_RSHIFT: return 0x10; case SDLK_LCTRL: case SDLK_RCTRL: return 0x11;
    case SDLK_LALT: case SDLK_RALT: return 0x12; case SDLK_PAUSE: return 0x13; case SDLK_CAPSLOCK: return 0x14;
    case SDLK_ESCAPE: return 0x1B; case SDLK_SPACE: return 0x20; case SDLK_PAGEUP: return 0x21;
    case SDLK_PAGEDOWN: return 0x22; case SDLK_END: return 0x23; case SDLK_HOME: return 0x24;
    case SDLK_LEFT: return 0x25; case SDLK_UP: return 0x26; case SDLK_RIGHT: return 0x27; case SDLK_DOWN: return 0x28;
    case SDLK_INSERT: return 0x2D; case SDLK_DELETE: return 0x2E;
    }
    return 0;
}

LPARAM mouse_lparam(float x, float y) { return (LPARAM)(((int)y & 0xFFFF) << 16 | ((int)x & 0xFFFF)); }

void *create_window(const char *title, int width, int height)
{
    if (!window) {
        window = SDL_CreateWindow(title, width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY);
        renderer = window ? SDL_CreateRenderer(window, nullptr) : nullptr;
        if (renderer) SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        SDL_StartTextInput(window);
    }
    return window;
}

void pump_events()
{
    HWND w = ra2port::main_window();
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_EVENT_QUIT: ra2port::post_message(w, 0x0010, 0, 0); break;   // WM_CLOSE
        case SDL_EVENT_KEY_DOWN: case SDL_EVENT_KEY_UP: {
            int vk = virtual_key(e.key.key);
            if (!vk) break;
            bool down = e.type == SDL_EVENT_KEY_DOWN;
            ra2port::set_key_state(vk, down);
            LPARAM lp = (LPARAM)(1 | ((int)e.key.scancode << 16) | (down ? 0 : (3u << 30)));
            ra2port::post_message(w, down ? 0x0100 : 0x0101, (WPARAM)vk, lp);   // WM_KEYDOWN/UP
            break;
        }
        case SDL_EVENT_TEXT_INPUT:
            for (const unsigned char *c = (const unsigned char *)e.text.text; *c; ++c)
                if (*c < 0x80) ra2port::post_message(w, 0x0102, *c, 1);   // WM_CHAR (ASCII)
            break;
        case SDL_EVENT_MOUSE_MOTION:
            ra2port::post_message(w, 0x0200, 0, mouse_lparam(e.motion.x, e.motion.y));
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN: case SDL_EVENT_MOUSE_BUTTON_UP: {
            bool down = e.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
            UINT msg = e.button.button == SDL_BUTTON_RIGHT ? (down ? 0x0204 : 0x0205) : (down ? 0x0201 : 0x0202);
            ra2port::set_key_state(e.button.button == SDL_BUTTON_RIGHT ? 0x02 : 0x01, down);
            ra2port::post_message(w, msg, 0, mouse_lparam(e.button.x, e.button.y));
            break;
        }
        case SDL_EVENT_WINDOW_FOCUS_GAINED: ra2port::post_message(w, 0x001C, 1, 0); break;   // WM_ACTIVATEAPP
        case SDL_EVENT_WINDOW_FOCUS_LOST: ra2port::post_message(w, 0x001C, 0, 0); break;
        }
    }
}

void present(const void *pixels, int width, int height, int pitch, int bits)
{
    if (!renderer || bits != 16) return;
    if (!texture || tex_w != width || tex_h != height) {
        if (texture) SDL_DestroyTexture(texture);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, width, height);
        tex_w = width;
        tex_h = height;
    }
    SDL_UpdateTexture(texture, nullptr, pixels, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void set_relative_cursor(bool show)
{
    if (show) SDL_ShowCursor(); else SDL_HideCursor();
}

const ra2port::Host sdl_host = {create_window, pump_events, present, set_relative_cursor};
}  // namespace

void ra2_install_sdl_host() { ra2port::set_host(&sdl_host); }
