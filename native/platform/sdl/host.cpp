// SPDX-License-Identifier: GPL-3.0-or-later
// The SDL3 side of the Win32 seam (ra2port::Host): the game's main window, the
// event pump that turns SDL input into the Win32 messages the recovered window
// procedure expects, and presentation of the DirectDraw primary surface.
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <atomic>

#include "ra2port/win32_port.hpp"

namespace ra2port { void set_key_state(int vk, bool down); }
// A scripted point "@ID..." as game pixels (port/src/run_progress.cpp asks the
// skirmish harness); 0 when it has no answer.
extern "C" int ra2port_resolve_point(const char *spec, int *x, int *y);

// Trackpad scrolling over the tactical view pans the map (README "Mac app"):
// the game side answers through this hook, called at the start of a Main_Loop
// frame with the pixels the fingers moved since the last frame (dx right,
// dy down: the view moves that way) and the cursor's game pixel. The weak
// default answers 0 (not handled): the host then posts the vertical part as
// WM_MOUSEWHEEL (the sidebar scroll), as a mouse wheel does. A strong
// definition in the game tree (native/link) replaces it.
extern "C" __attribute__((weak)) int ra2_native_scroll_map(int, int, int, int) { return 0; }
// Quitting (Cmd+Q, the app menu's Quit, the window's close button) saves the
// game's settings (RA2.INI) as the main menu's Exit Game does, through this
// hook (weak default: nothing to save; native/link may define it).
extern "C" __attribute__((weak)) void ra2_native_save_settings(void) {}
extern "C" int ra2port_last_frame(void);

namespace {
SDL_Window *window = nullptr;
// The activation state the game was last told (USER32 activates it when its first window shows).
bool game_active = true;
SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;
int tex_w = 0, tex_h = 0;

// Mac app options (main.cpp, ra2_sdl_host_options): start in fullscreen; ask
// before quitting a game; what to save before a quit ends the process.
bool want_fullscreen = false;
bool app_mode = false;
void (*before_exit)() = nullptr;     // the app's settings (main.cpp)
// The last pointer position in game pixels (the trackpad hook's cursor).
float pointer_x = 0, pointer_y = 0;
// Trackpad motion not yet handed to the game (pixels), applied at the next frame.
float pan_dx = 0, pan_dy = 0;
Uint64 pan_last_ms = 0;
// The vertical pan the game did not take (no hook), as wheel notches.
float pan_wheel = 0;

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

// A window point converted to the game's pixels lies outside the picture when
// it is over a letterbox bar (fullscreen on a display of another aspect). The
// game reads the cursor as the screen's, so it is held on the picture's edge:
// at the display's edge the game sees its own screen edge and edge-scrolls.
void clamp_to_picture(float &x, float &y)
{
    if (tex_w <= 0 || tex_h <= 0) return;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > tex_w - 1) x = (float)(tex_w - 1);
    if (y > tex_h - 1) y = (float)(tex_h - 1);
}

// Fullscreen (Cmd+Return, Cmd+F, Alt+Return; README "Mac app"): a borderless
// window over the whole display (no Spaces animation, the menu bar and Dock
// hidden, so the top edge scrolls the map), the picture scaled to fit with
// its aspect kept. The mouse is confined to the window in fullscreen so the
// display's edges reach the game's edge scroll.
void apply_mouse_grab()
{
    if (!window) return;
    bool fullscreen = (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) != 0;
    SDL_SetWindowMouseGrab(window, fullscreen);
}
void set_fullscreen(bool on)
{
    if (!window) return;
    SDL_SetWindowFullscreen(window, on);
    SDL_SyncWindow(window);
    apply_mouse_grab();
    fprintf(stderr, "host: %s\n", on ? "fullscreen" : "windowed");
}
bool is_fullscreen() { return window && (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) != 0; }

// The window in points for a game resolution: the game's pixels at one
// point each (two device pixels on a Retina display), smaller only when that
// does not fit the display's usable area.
void fit_window(int width, int height)
{
    if (!window || is_fullscreen()) return;
    int w = width, h = height;
    SDL_Rect usable;
    SDL_DisplayID display = SDL_GetDisplayForWindow(window);
    if (display && SDL_GetDisplayUsableBounds(display, &usable) && usable.w > 0 && usable.h > 0) {
        int max_w = usable.w, max_h = usable.h - 28;     // the title bar
        if (w > max_w || h > max_h) {
            double scale = SDL_min((double)max_w / w, (double)max_h / h);
            w = (int)(w * scale);
            h = (int)(h * scale);
        }
    }
    SDL_SetWindowSize(window, w, h);
}

struct Deferred { const void *pixels; int width, height, pitch; };
std::atomic<bool> deferred_pending{false};
Deferred deferred;
Uint64 last_present_ms = 0;
SDL_Mutex *deferred_lock = nullptr;

void *create_window(const char *title, int width, int height)
{
    if (!window) {
        // Resizable, HiDPI (the renderer draws in device pixels; the picture
        // keeps its aspect, letterboxed).
        SDL_SetHint(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0");
        window = SDL_CreateWindow(title, width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
        renderer = window ? SDL_CreateRenderer(window, nullptr) : nullptr;
        if (window) {
            SDL_SetWindowMinimumSize(window, 320, 240);
            fit_window(width, height);
            if (want_fullscreen) set_fullscreen(true);
            // Ask for the focus at once: a Windows program's first window is
            // active (user32 sends the game WM_ACTIVATEAPP when it is shown),
            // and the game's start-up waits while it is not (Scout1209).
            SDL_RaiseWindow(window);
        }
        if (!deferred_lock) deferred_lock = SDL_CreateMutex();
        // The logical size follows the presented surface (present below), so
        // mouse positions convert to the game's pixels.
        if (renderer) SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        SDL_StartTextInput(window);
    }
    return window;
}

// Cmd+Q, the app menu's Quit, the window's close button (README "Mac app").
// Not WM_CLOSE: the original ignores the close command (Windows_Procedure
// 0x007375A0 answers WM_SYSCOMMAND SC_CLOSE with 0) and a WM_CLOSE reaching
// DefWindowProc destroys the main window under the running game, which then
// waits for ever. The host ends the game itself: after a game has run, the app
// asks first; the settings are saved (ra2_native_save_settings, RA2.INI) and
// the process ends. Without the exit-time destructors: the game's threads (the
// mouse timer, audio) still run and used a destroyed mutex under exit(0)
// ("mutex lock failed", an abort); Windows' ExitProcess stops them first.
void quit()
{
    fprintf(stderr, "host: quit requested\n");
    // Scripted runs (--input-script) quit without asking.
    const char *scripted = getenv("RA2_NATIVE_INPUT_SCRIPT");
    if (app_mode && ra2port_last_frame() > 0 && !(scripted && *scripted)) {
        bool was_fullscreen = is_fullscreen();
        if (was_fullscreen) set_fullscreen(false);
        const SDL_MessageBoxButtonData buttons[] = {
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Quit"},
            {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Cancel"},
        };
        const SDL_MessageBoxData box = {SDL_MESSAGEBOX_WARNING, window, "Red Alert 2",
                                        "Quit Red Alert 2?\n\nA game in progress is not saved.", 2, buttons, nullptr};
        int pressed = 0;
        if (SDL_ShowMessageBox(&box, &pressed) && pressed != 0) {
            fprintf(stderr, "host: quit cancelled\n");
            if (was_fullscreen) set_fullscreen(true);
            return;
        }
    }
    ra2_native_save_settings();
    if (before_exit) before_exit();
    fprintf(stderr, "host: quitting\n");
    fflush(nullptr);
    _exit(0);
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
// A trackpad's tap (tap to click) sends the button down and up together, in
// one event pump: the game then read the press from its queue while
// WWKeyboardClass::Down (0x00532890, GetAsyncKeyState) already reported the
// button up, and the click was lost: no unit could be selected, moved or
// deployed with taps (user report 2026-11-09, trackpad). A mouse's click, or a
// trackpad's physical one, holds the button for several frames. A release that
// comes before the game has run two frames with the button down (or within
// 50 ms where no frames run, the menus) is held back until then (Scout1209).
struct HeldButton { int down_frame = 0; Uint64 down_ms = 0; bool release_pending = false; float x = 0, y = 0; };
HeldButton held[2];   // left, right
void press_button(int b, float x, float y)
{
    ra2port::set_key_state(b ? 0x02 : 0x01, true);
    ra2port::post_message(ra2port::main_window(), b ? 0x0204 : 0x0201, 0, mouse_lparam(x, y));   // WM_RBUTTONDOWN / WM_LBUTTONDOWN
}
void release_button(int b)
{
    held[b].release_pending = false;
    ra2port::set_key_state(b ? 0x02 : 0x01, false);
    ra2port::post_message(ra2port::main_window(), b ? 0x0205 : 0x0202, 0, mouse_lparam(held[b].x, held[b].y));   // WM_RBUTTONUP / WM_LBUTTONUP
}
// The game has seen the button held: two game frames ran since the press, or
// none ran for 50 ms (the menus read the queue only).
bool seen_held(int b)
{
    int frames = ra2port_last_frame() - held[b].down_frame;
    return frames >= 2 || (frames == 0 && SDL_GetTicks() - held[b].down_ms >= 50);
}
void release_held_buttons()
{
    for (int b = 0; b < 2; ++b)
        if (held[b].release_pending && seen_held(b)) release_button(b);
}

void pump_events()
{
    release_held_buttons();
    play_timed();
    if (SDL_IsMainThread()) present_deferred();
    HWND w = ra2port::main_window();
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        // Window coordinates to the game's: the presented surface letterboxed
        // into the window (scripted events are in game pixels already).
        bool scripted = (e.type == SDL_EVENT_MOUSE_MOTION && e.motion.which == script_mouse) ||
                        (e.type == SDL_EVENT_MOUSE_WHEEL && e.wheel.which == script_mouse) ||
                        ((e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP) &&
                         e.button.which == script_mouse);
        if (renderer && !scripted) SDL_ConvertEventToRenderCoordinates(renderer, &e);
        switch (e.type) {
        case SDL_EVENT_QUIT:
            quit();
            break;
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
            apply_mouse_grab();
            break;
        case SDL_EVENT_KEY_DOWN: case SDL_EVENT_KEY_UP: {
            // Command-key shortcuts are the Mac's, never the game's: Cmd+Return
            // and Cmd+F toggle fullscreen (Alt+Return too, as Windows ports do);
            // Cmd+Q reaches SDL_EVENT_QUIT through the app menu.
            bool down_key = e.type == SDL_EVENT_KEY_DOWN;
            if (e.key.mod & SDL_KMOD_GUI) {
                if (down_key && !e.key.repeat && (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER || e.key.key == SDLK_F))
                    set_fullscreen(!is_fullscreen());
                break;
            }
            if (down_key && !e.key.repeat && (e.key.mod & SDL_KMOD_ALT) && e.key.key == SDLK_RETURN) {
                set_fullscreen(!is_fullscreen());
                break;
            }
            int vk = virtual_key(e.key.key);
            if (!vk) break;
            bool down = e.type == SDL_EVENT_KEY_DOWN;
            ra2port::set_key_state(vk, down);
            LPARAM lp = (LPARAM)(1 | ((int)e.key.scancode << 16) | (down ? 0 : (3u << 30)));
            ra2port::post_message(w, down ? 0x0100 : 0x0101, (WPARAM)vk, lp);   // WM_KEYDOWN/UP
            // TranslateMessage's WM_CHAR for the control keys SDL's text input does not
            // type: Backspace, Tab, Return and Escape (VK codes equal to their characters);
            // the lobby's name field never deleted a character (user report, Scout1209 r17).
            if (down && (vk == 0x08 || vk == 0x09 || vk == 0x0D || vk == 0x1B))
                ra2port::post_message(w, 0x0102, (WPARAM)vk, lp);                 // WM_CHAR
            break;
        }
        case SDL_EVENT_TEXT_INPUT:
            for (const unsigned char *c = (const unsigned char *)e.text.text; *c; ++c)
                if (*c < 0x80) ra2port::post_message(w, 0x0102, *c, 1);   // WM_CHAR (ASCII)
            break;
        case SDL_EVENT_MOUSE_MOTION:
            clamp_to_picture(e.motion.x, e.motion.y);
            pointer_x = e.motion.x;
            pointer_y = e.motion.y;
            ra2port::post_message(w, 0x0200, 0, mouse_lparam(e.motion.x, e.motion.y));
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN: case SDL_EVENT_MOUSE_BUTTON_UP: {
            bool down = e.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
            clamp_to_picture(e.button.x, e.button.y);
            if (e.button.button != SDL_BUTTON_LEFT && e.button.button != SDL_BUTTON_RIGHT) break;
            int b = e.button.button == SDL_BUTTON_RIGHT ? 1 : 0;
            if (down) {
                if (held[b].release_pending) release_button(b);   // a second tap before the first was seen
                held[b].down_frame = ra2port_last_frame();
                held[b].down_ms = SDL_GetTicks();
                press_button(b, e.button.x, e.button.y);
            } else if (!seen_held(b)) {
                // Released before the game saw the button held: kept down until it has.
                held[b].release_pending = true;
                held[b].x = e.button.x;
                held[b].y = e.button.y;
            } else {
                held[b].x = e.button.x;
                held[b].y = e.button.y;
                release_button(b);
            }
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            // A trackpad (fractional, two-axis deltas) over the tactical view
            // pans the map: collected here, handed to the game at the next frame
            // (frame below). SDL's precise deltas are a tenth of the pixels the
            // fingers moved; natural scrolling is already in their sign.
            float wx = e.wheel.mouse_x, wy = e.wheel.mouse_y;
            clamp_to_picture(wx, wy);
            bool precise = e.wheel.x != (float)(int)e.wheel.x || e.wheel.y != (float)(int)e.wheel.y ||
                           (e.wheel.x != 0 && e.wheel.y != 0);
            bool over_sidebar = tex_w > 0 && wx >= tex_w - 168;
            if (precise && !over_sidebar) {
                // A new gesture (or one that ended in the menus, where no frame
                // runs) starts from nothing.
                if (pan_last_ms && SDL_GetTicks() - pan_last_ms > 250) pan_dx = pan_dy = pan_wheel = 0;
                pointer_x = wx;
                pointer_y = wy;
                pan_dx += e.wheel.x * 10.0f;
                pan_dy -= e.wheel.y * 10.0f;
                pan_last_ms = SDL_GetTicks();
                break;
            }
            // WM_MOUSEWHEEL: Windows_Procedure 0x007375A0 scrolls the sidebar
            // (wheel delta in the high word, 120 per notch; the cursor's screen point).
            if (precise && e.wheel.integer_y == 0) break;     // a trackpad over the sidebar: whole notches only
            int delta = (int)((precise ? (float)e.wheel.integer_y : e.wheel.y) * 120.0f);
            if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) delta = -delta;
            if (delta == 0) break;
            ra2port::post_message(w, 0x020A, (WPARAM)((delta & 0xFFFF) << 16), mouse_lparam(wx, wy));
            break;
        }
        // WM_ACTIVATEAPP on changes only. USER32 activates the game when its first window
        // is shown, as Windows does; macOS then reports the focus once more when the app
        // becomes active, and that second activation made the game redraw the main menu,
        // whose buttons vanished and slid in again (user report, Scout1209 r13).
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            if (!game_active) { game_active = true; ra2port::post_message(w, 0x001C, 1, 0); }
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            if (game_active) { game_active = false; ra2port::post_message(w, 0x001C, 0, 0); }
            break;
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
        // takes that size (in points; fullscreen keeps the display) and the
        // mouse maps onto those pixels.
        SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        fit_window(width, height);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, width, height);
        // Sharp pixels at any scale (Retina 2x, fullscreen's fractional fit).
        if (texture) SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
        tex_w = width;
        tex_h = height;
    }
    SDL_UpdateTexture(texture, nullptr, pixels, pitch);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    // RA2_NATIVE_WINDOW_DUMP=<file> (test runs): every 2 s, the whole window
    // as drawn (device pixels: the scaling, letterbox and fullscreen) to
    // <file>.<n>.bmp.
    static const char *window_dump = getenv("RA2_NATIVE_WINDOW_DUMP");
    static Uint64 window_dump_ms = 0;
    static int window_dump_count = 0;
    if (window_dump && *window_dump && SDL_GetTicks() - window_dump_ms >= 2000) {
        window_dump_ms = SDL_GetTicks();
        if (SDL_Surface *shot = SDL_RenderReadPixels(renderer, nullptr)) {
            char name[1024];
            snprintf(name, sizeof name, "%s.%d.bmp", window_dump, window_dump_count++);
            SDL_SaveBMP(shot, name);
            SDL_DestroySurface(shot);
        }
    }
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
    // "cmd+NAME", "alt+NAME": the key with that modifier (fullscreen and quit
    // shortcuts in test runs).
    SDL_Keymod mod = SDL_KMOD_NONE;
    for (;;) {
        if (!strncasecmp(name, "cmd+", 4)) { mod |= SDL_KMOD_LGUI; name += 4; }
        else if (!strncasecmp(name, "alt+", 4)) { mod |= SDL_KMOD_LALT; name += 4; }
        else break;
    }
    // '_' stands for a space in SDL key names ("Left_Ctrl"; M25: Ctrl+number groups).
    std::string spaced(name);
    for (char &c : spaced) if (c == '_') c = ' ';
    SDL_Keycode key = SDL_GetKeyFromName(spaced.c_str());
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
    s.event.key.mod = mod;
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
        if (action == "blur" || action == "focus") {
            // blur / focus: the window loses or regains the focus (another app
            // in front; test scaffolding, Scout1209).
            Scripted s = {frame, {}, std::string()};
            if (script_ms >= 0) { s.ms = script_ms; s.frame = 0x7FFFFFFF; s.dialog = script_dialog; }
            s.event.type = action == "blur" ? SDL_EVENT_WINDOW_FOCUS_LOST : SDL_EVENT_WINDOW_FOCUS_GAINED;
            script.push_back(s);
            continue;
        }
        if (action == "quit" || action == "wheel") {
            // quit: what Cmd+Q sends (SDL_EVENT_QUIT). wheel DX DY [X Y]: a
            // trackpad scroll of that many SDL units at the game pixel X, Y.
            Scripted s = {frame, {}, std::string()};
            if (script_ms >= 0) { s.ms = script_ms; s.frame = 0x7FFFFFFF; s.dialog = script_dialog; }
            if (action == "quit") s.event.type = SDL_EVENT_QUIT;
            else if (tokens.size() >= 4) {
                s.event.type = SDL_EVENT_MOUSE_WHEEL;
                s.event.wheel.which = script_mouse;
                s.event.wheel.x = strtof(tokens[2].c_str(), nullptr);
                s.event.wheel.y = strtof(tokens[3].c_str(), nullptr);
                s.event.wheel.mouse_x = tokens.size() >= 6 ? strtof(tokens[4].c_str(), nullptr) : 300.0f;
                s.event.wheel.mouse_y = tokens.size() >= 6 ? strtof(tokens[5].c_str(), nullptr) : 300.0f;
            } else ok = false;
            if (ok) script.push_back(s);
        } else if (action == "key" || action == "keydown" || action == "keyup") {
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
    release_held_buttons();
    // Trackpad panning collected since the last frame (pump_events).
    int dx = (int)pan_dx, dy = (int)pan_dy;
    if (pan_last_ms && SDL_GetTicks() - pan_last_ms > 250) {
        pan_dx = pan_dy = pan_wheel = 0;    // the remainder of a finished gesture
        pan_last_ms = 0;
    } else if (dx || dy) {
        pan_dx -= dx;
        pan_dy -= dy;
        if (!ra2_native_scroll_map((int)pointer_x, (int)pointer_y, dx, dy)) {
            // No map pan in this build: 40 pixels of vertical motion make one
            // wheel notch (the sidebar scroll), fingers up scrolling it up.
            pan_wheel -= dy;
            HWND w = ra2port::main_window();
            for (; pan_wheel >= 40 || pan_wheel <= -40; pan_wheel += pan_wheel > 0 ? -40 : 40) {
                int delta = pan_wheel > 0 ? 120 : -120;
                ra2port::post_message(w, 0x020A, (WPARAM)((delta & 0xFFFF) << 16), mouse_lparam(pointer_x, pointer_y));
            }
        }
    }
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
        fprintf(stderr, "input-script: t%llu event %u (steady %lld ms)\n", (unsigned long long)now, s.event.type, ra2port_now_ms());
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
        } else if (e.type == SDL_EVENT_WINDOW_FOCUS_LOST || e.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
            e.window.windowID = id;
        } else if (e.type == SDL_EVENT_QUIT) {
        } else if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            e.wheel.windowID = id;
        } else {
            float x = e.type == SDL_EVENT_MOUSE_MOTION ? e.motion.x : e.button.x;
            float y = e.type == SDL_EVENT_MOUSE_MOTION ? e.motion.y : e.button.y;
            if (!s.spec.empty()) {
                // @SPEC/dx,dy: dx, dy game pixels from the point (test scaffolding:
                // a click that moves a little, as a trackpad's does; Scout1209).
                std::string spec = s.spec;
                int ox = 0, oy = 0;
                size_t slash = spec.find('/');
                if (slash != std::string::npos) {
                    sscanf(spec.c_str() + slash + 1, "%d,%d", &ox, &oy);
                    spec.resize(slash);
                }
                int px = 0, py = 0;
                if (!ra2port_resolve_point(spec.c_str(), &px, &py)) {
                    fprintf(stderr, "input-script: frame %d: no point for @%s\n", number, s.spec.c_str());
                    return;
                }
                x = (float)(px + ox);
                y = (float)(py + oy);
            }
            // RA2_NATIVE_SCRIPT_AS_USER=1 (test scaffolding, M27): the event as a player's mouse
            // makes it, in window coordinates (converted back by the pump like a real event).
            static bool as_user = getenv("RA2_NATIVE_SCRIPT_AS_USER") != nullptr;
            if (as_user && renderer) {
                float wx = x, wy = y;
                SDL_RenderCoordinatesToWindow(renderer, x, y, &wx, &wy);
                x = wx;
                y = wy;
                if (e.type == SDL_EVENT_MOUSE_MOTION) e.motion.which = 0; else e.button.which = 0;
            }
            if (e.type == SDL_EVENT_MOUSE_MOTION) { e.motion.windowID = id; e.motion.x = x; e.motion.y = y; }
            else { e.button.windowID = id; e.button.x = x; e.button.y = y; }
        }
        SDL_PushEvent(&e);
        // As a player's keyboard: a printable key also types its character (SDL text input).
        static bool as_user_keys = getenv("RA2_NATIVE_SCRIPT_AS_USER") != nullptr;
        if (as_user_keys && e.type == SDL_EVENT_KEY_DOWN && e.key.key >= 0x20 && e.key.key < 0x7F) {
            static char text[2];
            text[0] = (char)e.key.key;
            text[1] = 0;
            SDL_Event t = {};
            t.type = SDL_EVENT_TEXT_INPUT;
            t.text.windowID = id;
            t.text.text = text;
            SDL_PushEvent(&t);
        }
    }
}

// Host::wait: the main thread is about to sleep; show the frame it left in the
// primary (presents from other threads stay deferred to the main thread).
void wait_begins()
{
    if (SDL_IsMainThread()) present_deferred();
}

// Host::keep_alive: SDL_PumpEvents moves the system's events into SDL's queue
// (the window answers, no spinning cursor) without translating them: the game
// reads them at its next PeekMessage, in order, as Windows queued input while
// the original was busy.
void keep_alive()
{
    if (SDL_IsMainThread()) SDL_PumpEvents();
}

const ra2port::Host sdl_host = {create_window, pump_events, present, set_relative_cursor, open_audio, frame, wait_begins, keep_alive};
}  // namespace

void ra2_install_sdl_host() { ra2port::set_host(&sdl_host); }
// Mac app (main.cpp): start in fullscreen; what to save before a quit ends the
// process.
void ra2_sdl_host_options(bool app, bool fullscreen, void (*on_quit)())
{
    app_mode = app;
    want_fullscreen = fullscreen;
    before_exit = on_quit;
}
// The window's fullscreen state at exit (remembered in the settings).
bool ra2_sdl_host_fullscreen() { return is_fullscreen(); }
