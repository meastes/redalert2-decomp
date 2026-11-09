// SPDX-License-Identifier: GPL-3.0-or-later
// The SDL3 side of the Win32 seam (ra2port::Host): the game's main window, the
// event pump that turns SDL input into the Win32 messages the recovered window
// procedure expects, and presentation of the DirectDraw primary surface.
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <atomic>

#include "ra2port/win32_port.hpp"

namespace ra2port { void set_key_state(int vk, bool down); }
// A scripted point "@ID..." as game pixels (port/src/run_progress.cpp asks the
// skirmish harness); 0 when it has no answer.
extern "C" int ra2port_resolve_point(const char *spec, int *x, int *y);

namespace {
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;
int tex_w = 0, tex_h = 0;

// Mouse events the input script pushes carry this mouse id: their coordinates
// are already the game's (the presented surface's pixels).
SDL_MouseID const script_mouse = 0x5C5C;

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

struct Deferred { const void *pixels; int width, height, pitch; };
std::atomic<bool> deferred_pending{false};
Deferred deferred;
Uint64 last_present_ms = 0;
SDL_Mutex *deferred_lock = nullptr;

void *create_window(const char *title, int width, int height)
{
    if (!window) {
        window = SDL_CreateWindow(title, width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY);
        renderer = window ? SDL_CreateRenderer(window, nullptr) : nullptr;
        if (!deferred_lock) deferred_lock = SDL_CreateMutex();
        // The logical size follows the presented surface (present below), so
        // mouse positions convert to the game's pixels.
        if (renderer) SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        SDL_StartTextInput(window);
    }
    return window;
}

void play_timed();
void present_now(const void *pixels, int width, int height, int pitch);
// A present asked for off the main thread (the game's mouse timer thread:
// WWMouseClass::Low_Hide_Mouse unlocks the primary surface) is deferred to the
// main thread: SDL's renderer (Metal) is not thread-safe, and two threads
// encoding at once aborted with "commit command buffer with uncommitted
// encoder" (Scout1125, M15). The surface's pixels stay put, so the main thread
// presents them as they are then, at its next present or event pump.
void present_deferred()
{
    if (!deferred_pending.exchange(false)) return;
    Deferred d;
    SDL_LockMutex(deferred_lock);
    d = deferred;
    SDL_UnlockMutex(deferred_lock);
    present_now(d.pixels, d.width, d.height, d.pitch);
}
void pump_events()
{
    play_timed();
    if (SDL_IsMainThread()) present_deferred();
    HWND w = ra2port::main_window();
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        // Window coordinates to the game's: the presented surface letterboxed
        // into the window (scripted events are in game pixels already).
        bool scripted = (e.type == SDL_EVENT_MOUSE_MOTION && e.motion.which == script_mouse) ||
                        ((e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP) &&
                         e.button.which == script_mouse);
        if (renderer && !scripted) SDL_ConvertEventToRenderCoordinates(renderer, &e);
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
            if (e.button.button != SDL_BUTTON_LEFT && e.button.button != SDL_BUTTON_RIGHT) break;
            UINT msg = e.button.button == SDL_BUTTON_RIGHT ? (down ? 0x0204 : 0x0205) : (down ? 0x0201 : 0x0202);
            ra2port::set_key_state(e.button.button == SDL_BUTTON_RIGHT ? 0x02 : 0x01, down);
            ra2port::post_message(w, msg, 0, mouse_lparam(e.button.x, e.button.y));
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            // WM_MOUSEWHEEL: Windows_Procedure 0x007375A0 scrolls the sidebar
            // (wheel delta in the high word, 120 per notch; the cursor's screen point).
            int delta = (int)(e.wheel.y * 120.0f);
            if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) delta = -delta;
            if (delta == 0) break;
            ra2port::post_message(w, 0x020A, (WPARAM)((delta & 0xFFFF) << 16), mouse_lparam(e.wheel.mouse_x, e.wheel.mouse_y));
            break;
        }
        case SDL_EVENT_WINDOW_FOCUS_GAINED: ra2port::post_message(w, 0x001C, 1, 0); break;   // WM_ACTIVATEAPP
        case SDL_EVENT_WINDOW_FOCUS_LOST: ra2port::post_message(w, 0x001C, 0, 0); break;
        }
    }
}

// Presents are coalesced to the message pump (user report 2026-11-09, M20:
// the sidebar flickered while a building was produced). The game draws on the
// DirectDraw primary in steps: it hides the mouse (the mouse thread erases the
// cursor and unlocks the primary), redraws the sidebar or the tactical view,
// then shows the mouse again. On Windows the primary is the screen and those
// steps last microseconds; presenting each one held an intermediate state
// (the cursor erased, a cameo half drawn) on screen until the next present,
// every frame. Each primary update now records the frame; the main thread
// presents it at its next event pump (once per Main_Loop frame, as the game
// reads input) or at once when nothing was presented for 100 ms (loading
// screens and other loops that draw without pumping).
void present(const void *pixels, int width, int height, int pitch, int bits)
{
    if (!renderer || bits != 16) return;
    if (!deferred_lock) return;
    SDL_LockMutex(deferred_lock);
    deferred = Deferred{pixels, width, height, pitch};
    SDL_UnlockMutex(deferred_lock);
    deferred_pending = true;
    if (SDL_IsMainThread() && SDL_GetTicks() - last_present_ms >= 100) present_deferred();
}

void present_now(const void *pixels, int width, int height, int pitch)
{
    last_present_ms = SDL_GetTicks();
    if (!texture || tex_w != width || tex_h != height) {
        if (texture) SDL_DestroyTexture(texture);
        // The game's resolution (its DirectDraw display mode): the window
        // takes that size and the mouse maps onto those pixels.
        SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        SDL_SetWindowSize(window, width, height);
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

// Audio: an SDL3 playback stream (stereo, signed 16-bit) that asks the port's
// DirectSound mixer for every block the device consumes.
void (*audio_mix)(short *out, int frames) = nullptr;
void SDLCALL audio_callback(void *, SDL_AudioStream *stream, int additional, int)
{
    short block[2048 * 2];
    while (additional > 0) {
        int frames = additional / 4;
        if (frames > 2048) frames = 2048;
        if (frames <= 0) break;
        audio_mix(block, frames);
        SDL_PutAudioStreamData(stream, block, frames * 4);
        additional -= frames * 4;
    }
}
bool open_audio(int rate, void (*mix)(short *out, int frames))
{
    // One line on stderr either way (user report 2026-11-09, M20: no sound in
    // play): the SDL driver and device, or why none opened.
    if (!SDL_WasInit(SDL_INIT_AUDIO) && !SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        fprintf(stderr, "audio: SDL audio init failed: %s\n", SDL_GetError());
        return false;
    }
    audio_mix = mix;
    SDL_AudioSpec spec = {SDL_AUDIO_S16, 2, rate};
    SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, audio_callback, nullptr);
    if (!stream) {
        fprintf(stderr, "audio: no playback device (driver %s): %s\n", SDL_GetCurrentAudioDriver() ? SDL_GetCurrentAudioDriver() : "none", SDL_GetError());
        return false;
    }
    SDL_ResumeAudioStreamDevice(stream);
    const char *device = SDL_GetAudioDeviceName(SDL_GetAudioStreamDevice(stream));
    fprintf(stderr, "audio: playing on \"%s\" (driver %s, %d Hz stereo)\n", device ? device : "default", SDL_GetCurrentAudioDriver(), rate);
    return true;
}

// Scripted input (--input-script=<file>, RA2_NATIVE_INPUT_SCRIPT; test runs):
// one event per line, "<frame> <action> <args>", run when that Main_Loop
// frame begins, through the same SDL event path as a player's input. A point
// P is "X Y" in game pixels or "@ID[:n][+dx,dy]": the screen pixel of the
// human player's n-th object of type ID (default the first), moved by dx, dy
// cells (the skirmish harness answers, link/harness_input.cpp).
//   move P | ldown P | lup P | rdown P | rup P
//   click P        move, then left down and up on the next two frames
//   rclick P       the same with the right button
//   drag P0 P1     move, left down, move, left up on four frames
//   key NAME       key down and up (SDL key names: A, Return, Escape, F1...)
//   keydown NAME | keyup NAME
// '#' starts a comment. A first field "t<ms>" instead of a frame plays the
// event that many milliseconds after the window opened (menus and dialogs,
// where no Main_Loop frame runs; Scout1119, M14); the order of the lines is
// kept within each kind. "d<k>+<ms>" plays it that many milliseconds after
// the k-th dialog was shown (counting every dialog the game shows). Timed
// lines play in the order written.
struct Scripted { int frame; SDL_Event event; std::string spec; int ms = -1; int dialog = 0; };
extern "C" long long ra2port_dialog_shown_ms(int k);
extern "C" long long ra2port_now_ms(void);
std::vector<Scripted> script;
size_t script_next = 0;

int script_ms = -1;     // the line being read is timed ("t<ms>"): its events' offset
int script_dialog = 0;  // ... after the k-th dialog was shown ("d<k>+<ms>")
Uint64 script_start = 0;
void script_mouse_event(int frame, Uint32 type, float x, float y, std::string const &spec, Uint8 button)
{
    Scripted s = {frame, {}, spec};
    if (script_ms >= 0) { s.ms = script_ms + (frame - 1000000) * 60; s.frame = 0x7FFFFFFF; s.dialog = script_dialog; }
    s.event.type = type;
    if (type == SDL_EVENT_MOUSE_MOTION) {
        s.event.motion.which = script_mouse;
        s.event.motion.x = x;
        s.event.motion.y = y;
    } else {
        s.event.button.which = script_mouse;
        s.event.button.button = button;
        s.event.button.down = type == SDL_EVENT_MOUSE_BUTTON_DOWN;
        s.event.button.clicks = 1;
        s.event.button.x = x;
        s.event.button.y = y;
    }
    script.push_back(s);
}

void script_key_event(int frame, bool down, const char *name)
{
    SDL_Keycode key = SDL_GetKeyFromName(name);
    if (key == SDLK_UNKNOWN) {
        fprintf(stderr, "input-script: unknown key %s\n", name);
        return;
    }
    Scripted s = {frame, {}, std::string()};
    if (script_ms >= 0) { s.ms = script_ms + (frame - 1000000) * 60; s.frame = 0x7FFFFFFF; s.dialog = script_dialog; }
    s.event.type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
    s.event.key.key = key;
    s.event.key.scancode = SDL_GetScancodeFromKey(key, nullptr);
    s.event.key.down = down;
    script.push_back(s);
}

// A point at tokens[i]: "@spec" (one token) or "X Y" (two).
struct Point { float x = 0, y = 0; std::string spec; bool ok = false; };
Point script_point(std::vector<std::string> const &tokens, size_t &i)
{
    Point p;
    if (i < tokens.size() && tokens[i][0] == '@') {
        p.spec = tokens[i++].substr(1);
        p.ok = true;
    } else if (i + 1 < tokens.size()) {
        char *end1 = nullptr, *end2 = nullptr;
        p.x = strtof(tokens[i].c_str(), &end1);
        p.y = strtof(tokens[i + 1].c_str(), &end2);
        p.ok = *end1 == 0 && *end2 == 0;
        i += 2;
    }
    return p;
}

void load_script()
{
    static bool loaded = false;
    if (loaded) return;
    loaded = true;
    const char *path = getenv("RA2_NATIVE_INPUT_SCRIPT");
    if (!path || !*path) return;
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "input-script: cannot open %s\n", path);
        return;
    }
    char line[256];
    while (fgets(line, sizeof line, f)) {
        if (char *hash = strchr(line, '#')) *hash = 0;
        std::vector<std::string> tokens;
        for (char *t = strtok(line, " \t\r\n"); t; t = strtok(nullptr, " \t\r\n")) tokens.push_back(t);
        if (tokens.size() < 2) continue;
        // Timed lines keep the frame arithmetic below (click: +1, +2) as 60 ms steps.
        script_ms = tokens[0][0] == 't' ? atoi(tokens[0].c_str() + 1) : -1;
        script_dialog = 0;
        if (tokens[0][0] == 'd') {      // d<k>+<ms>: ms after the k-th dialog was shown
            script_dialog = atoi(tokens[0].c_str() + 1);
            const char *plus = strchr(tokens[0].c_str(), '+');
            script_ms = plus ? atoi(plus + 1) : 0;
        }
        int frame = script_ms >= 0 ? 1000000 : atoi(tokens[0].c_str());
        std::string const action = tokens[1];
        size_t i = 2;
        bool ok = true;
        if (action == "key" || action == "keydown" || action == "keyup") {
            if (tokens.size() < 3) ok = false;
            else {
                if (action != "keyup") script_key_event(frame, true, tokens[2].c_str());
                if (action != "keydown") script_key_event(action == "key" ? frame + 1 : frame, false, tokens[2].c_str());
            }
        } else if (action == "drag") {
            Point a = script_point(tokens, i), b = script_point(tokens, i);
            ok = a.ok && b.ok;
            if (ok) {
                script_mouse_event(frame, SDL_EVENT_MOUSE_MOTION, a.x, a.y, a.spec, 0);
                script_mouse_event(frame + 1, SDL_EVENT_MOUSE_BUTTON_DOWN, a.x, a.y, a.spec, SDL_BUTTON_LEFT);
                script_mouse_event(frame + 2, SDL_EVENT_MOUSE_MOTION, b.x, b.y, b.spec, 0);
                script_mouse_event(frame + 3, SDL_EVENT_MOUSE_BUTTON_UP, b.x, b.y, b.spec, SDL_BUTTON_LEFT);
            }
        } else {
            Point a = script_point(tokens, i);
            ok = a.ok;
            if (!ok) {
            } else if (action == "move") script_mouse_event(frame, SDL_EVENT_MOUSE_MOTION, a.x, a.y, a.spec, 0);
            else if (action == "ldown") script_mouse_event(frame, SDL_EVENT_MOUSE_BUTTON_DOWN, a.x, a.y, a.spec, SDL_BUTTON_LEFT);
            else if (action == "lup") script_mouse_event(frame, SDL_EVENT_MOUSE_BUTTON_UP, a.x, a.y, a.spec, SDL_BUTTON_LEFT);
            else if (action == "rdown") script_mouse_event(frame, SDL_EVENT_MOUSE_BUTTON_DOWN, a.x, a.y, a.spec, SDL_BUTTON_RIGHT);
            else if (action == "rup") script_mouse_event(frame, SDL_EVENT_MOUSE_BUTTON_UP, a.x, a.y, a.spec, SDL_BUTTON_RIGHT);
            else if (action == "click" || action == "rclick") {
                Uint8 button = action == "rclick" ? SDL_BUTTON_RIGHT : SDL_BUTTON_LEFT;
                script_mouse_event(frame, SDL_EVENT_MOUSE_MOTION, a.x, a.y, a.spec, 0);
                script_mouse_event(frame + 1, SDL_EVENT_MOUSE_BUTTON_DOWN, a.x, a.y, a.spec, button);
                script_mouse_event(frame + 2, SDL_EVENT_MOUSE_BUTTON_UP, a.x, a.y, a.spec, button);
            } else ok = false;
        }
        if (!ok) fprintf(stderr, "input-script: cannot read line %d %s\n", frame, action.c_str());
    }
    fclose(f);
    // Stable by frame: the order within a frame is the script's.
    for (size_t i = 1; i < script.size(); ++i) {
        for (size_t k = i; k > 0 && script[k - 1].frame > script[k].frame; --k) {
            Scripted t = script[k - 1];
            script[k - 1] = script[k];
            script[k] = t;
        }
    }
    script_ms = -1;
    fprintf(stderr, "input-script: %zu events from %s\n", script.size(), path);
}

void play(Scripted &s, int number);
std::vector<size_t> timed;      // indices of timed events, in order
size_t timed_next = 0;
void frame(int number)
{
    load_script();
    while (script_next < script.size() && script[script_next].frame <= number) {
        play(script[script_next++], number);
    }
}
// Timed events (pump_events: menus and dialogs as well as the game).
void play_timed()
{
    load_script();
    if (!script_start) {
        script_start = SDL_GetTicks();
        for (size_t i = 0; i < script.size(); ++i) if (script[i].ms >= 0) timed.push_back(i);
    }
    // Timed events play in script order, each when due.
    Uint64 now = SDL_GetTicks() - script_start;
    while (timed_next < timed.size()) {
        Scripted &s = script[timed[timed_next]];
        if (s.dialog) {
            long long shown = ra2port_dialog_shown_ms(s.dialog);
            if (shown < 0 || ra2port_now_ms() - shown < s.ms) break;
        } else if ((Uint64)s.ms > now) {
            break;
        }
        fprintf(stderr, "input-script: t%llu event %u\n", (unsigned long long)now, s.event.type);
        play(script[timed[timed_next++]], -1);
    }
}
void play(Scripted &s, int number)
{
    Uint32 id = window ? SDL_GetWindowID(window) : 0;
    {
        SDL_Event e = s.event;
        if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP) {
            e.key.windowID = id;
        } else {
            float x = e.type == SDL_EVENT_MOUSE_MOTION ? e.motion.x : e.button.x;
            float y = e.type == SDL_EVENT_MOUSE_MOTION ? e.motion.y : e.button.y;
            if (!s.spec.empty()) {
                int px = 0, py = 0;
                if (!ra2port_resolve_point(s.spec.c_str(), &px, &py)) {
                    fprintf(stderr, "input-script: frame %d: no point for @%s\n", number, s.spec.c_str());
                    return;
                }
                x = (float)px;
                y = (float)py;
            }
            if (e.type == SDL_EVENT_MOUSE_MOTION) { e.motion.windowID = id; e.motion.x = x; e.motion.y = y; }
            else { e.button.windowID = id; e.button.x = x; e.button.y = y; }
        }
        SDL_PushEvent(&e);
    }
}

const ra2port::Host sdl_host = {create_window, pump_events, present, set_relative_cursor, open_audio, frame};
}  // namespace

void ra2_install_sdl_host() { ra2port::set_host(&sdl_host); }
