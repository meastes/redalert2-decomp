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
#include "host_os.hpp"
#include "touch.hpp"

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
// A network game runs (LAN lobby or game; native/link may define it): the host pause
// on focus loss would stall the other players, so the game goes on in the background
// there, as the original does on Windows. Weak default: no network game.
extern "C" __attribute__((weak)) int ra2_native_network_game(void) { return 0; }
// The touch app hides the game's own cursor, each mouse the game makes once (Scout1249;
// link/native_touch_cursor.cpp). Weak default: nothing to hide.
extern "C" __attribute__((weak)) int ra2_native_hide_game_cursor(void) { return 1; }
extern "C" int ra2port_last_frame(void);
extern "C" int ra2port_text_input_wanted(void);   // port/src/user32.cpp: an edit control has the focus
extern "C" int ra2port_menu_shown(void);          // port/src/user32.cpp: a menu dialog is shown (no sidebar)
extern "C" __attribute__((weak)) bool ra2port_presents_held(void);   // port/src/user32.cpp

namespace {
SDL_Window *window = nullptr;
// The activation state the game was last told (USER32 activates it when its first window shows).
bool game_active = true;
// The window's focus as SDL last reported it, and when (the game hears of a change once it has
// held for kFocusSettleMs; see pump_events).
bool window_focused = true;
Uint64 window_focus_ms = 0;
constexpr Uint64 kFocusSettleMs = 150;
SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;
int tex_w = 0, tex_h = 0;

// Mac app options (main.cpp, ra2_sdl_host_options): start in fullscreen; ask
// before quitting a game; what to save before a quit ends the process.
bool want_fullscreen = false;
bool app_mode = false;
// The host pause (ra2port::request_pause, Scout1247) when the window loses the focus:
// on in the app, off in test runs; RA2_NATIVE_FOCUS_PAUSE=1 or 0 overrides either.
bool pause_on_focus_loss = false;
// iOS: the app is in the background (nothing may be drawn there).
bool in_background = false;
// Why the host pause is wanted (Scout1248; update_pause): the window lost the focus
// (settled), the device is folded or turned so the usable part is too small, an
// audio-session interruption (a call).
bool focus_paused = false, folded = false, audio_interrupted = false;
// Checkpoint saves (ra2port::request_checkpoint): when the app goes to the background
// and every checkpoint_period_ms of play (0: never); on in the iOS app.
bool checkpoints = false;
Uint64 checkpoint_period_ms = 0, last_checkpoint_ms = 0;

// The layout (Scout1248): the usable part of the window in points (hostos::usable_rect:
// the iOS safe area less the hinge; elsewhere the whole window) and the game picture
// fitted into it, aspect kept, in points. Window coordinates convert to the game's
// pixels through it (the renderer draws in device pixels, no logical presentation).
hostos::Rect usable = {0, 0, 800, 600};
hostos::Rect picture = {0, 0, 800, 600};
int pic_w = 800, pic_h = 600;     // the game picture's size in game pixels
bool layout_dirty = true;

// The touch controls (Scout1248): on where the screen is touched (iOS), or with
// RA2_NATIVE_TOUCH_UI=1 (tests). A column of toggles at the usable part's left edge:
// force fire (Ctrl), force move (Alt), add to the selection (Shift) stay held while
// on; the keyboard; the interface scale (takes effect at the next start).
bool touch_ui = false;
bool system_toolbar = false;   // the iOS glass toolbar holds the toggles (hostos::toolbar_create; Scout1249)
struct Toggle { const char *label; int vk; bool on; };
Toggle toggles[] = {{"FIRE", 0x11, false}, {"MOVE", 0x12, false}, {"ADD", 0x10, false}, {"KEYS", 0, false}, {"SCALE", 0, false}};
constexpr int kToggleCount = 5, kKeysToggle = 3, kScaleToggle = 4;
double ui_scale = 1.0;
void (*scale_changed)(double) = nullptr;
bool text_input_on = false;
SDL_TouchID const script_touch = 0x5C5D;   // scripted fingers: coordinates in game pixels
SDL_FingerID toggle_finger = 0;            // the finger that pressed a toggle (its gesture is the toggle's)
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
    // The punctuation and keypad keys (Scout1236): the in-game chat line opens on
    // the backslash key too (Message_Input 0x00540C20), and ToAscii gives these
    // keys their characters.
    case SDLK_SEMICOLON: return 0xBA; case SDLK_EQUALS: return 0xBB; case SDLK_COMMA: return 0xBC;
    case SDLK_MINUS: return 0xBD; case SDLK_PERIOD: return 0xBE; case SDLK_SLASH: return 0xBF;
    case SDLK_GRAVE: return 0xC0; case SDLK_LEFTBRACKET: return 0xDB; case SDLK_BACKSLASH: return 0xDC;
    case SDLK_RIGHTBRACKET: return 0xDD; case SDLK_APOSTROPHE: return 0xDE;
    case SDLK_KP_ENTER: return 0x0D; case SDLK_KP_MULTIPLY: return 0x6A; case SDLK_KP_PLUS: return 0x6B;
    case SDLK_KP_MINUS: return 0x6D; case SDLK_KP_PERIOD: return 0x6E; case SDLK_KP_DIVIDE: return 0x6F;
    }
    if (k >= SDLK_KP_1 && k <= SDLK_KP_9) return 0x61 + (int)(k - SDLK_KP_1);   // SDL orders KP_1..KP_9, then KP_0
    if (k == SDLK_KP_0) return 0x60;
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

void sync_toolbar();
void toolbar_tapped(int i);
float pixel_density()
{
    int ww = 0, wh = 0, pw = 0, ph = 0;
    if (!window || !SDL_GetWindowSize(window, &ww, &wh) || !SDL_GetWindowSizeInPixels(window, &pw, &ph) || ww <= 0) return 1.0f;
    return (float)pw / (float)ww;
}
void update_layout()
{
    layout_dirty = false;
    if (!window) return;
    int ww = 0, wh = 0;
    SDL_GetWindowSize(window, &ww, &wh);
    usable = hostos::Rect{0, 0, (double)ww, (double)wh};
    void *native = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER, nullptr);
    hostos::Rect r;
    bool device = native && hostos::usable_rect(native, &r);
    if (device && r.w > 0 && r.h > 0) usable = r;
    // Folded (the iPhone Duo's outer display) or upright: too small for the game. (The
    // Duo's inner display in landscape leaves about 594x432 points clear in the Simulator.)
    bool was = folded;
    folded = device && (usable.w < 480 || usable.h < 320 || usable.w < usable.h);
    if (folded != was) fprintf(stderr, "host: usable area %.0fx%.0f points: %s\n", usable.w, usable.h, folded ? "too small (folded?): pause" : "fits");
    if (touch_ui && !system_toolbar && native) {
        static const hostos::ToolbarItem items[kToggleCount] = {
            {"scope", "Fire"}, {"arrow.up.and.down.and.arrow.left.and.right", "Move"},
            {"plus.square.on.square", "Add"}, {"keyboard", "Keys"}, {"textformat.size", "1x"}};
        system_toolbar = hostos::toolbar_create(native, items, kToggleCount, toolbar_tapped);
        if (system_toolbar) sync_toolbar();
    }
    if (system_toolbar) hostos::toolbar_place(usable);
    double scale = SDL_min(usable.w / pic_w, usable.h / pic_h);
    picture = hostos::Rect{usable.x + (usable.w - pic_w * scale) / 2, usable.y + (usable.h - pic_h * scale) / 2, pic_w * scale, pic_h * scale};
}
void window_to_game(float wx, float wy, float *gx, float *gy)
{
    *gx = picture.w > 0 ? (float)((wx - picture.x) * pic_w / picture.w) : wx;
    *gy = picture.h > 0 ? (float)((wy - picture.y) * pic_h / picture.h) : wy;
}
void game_to_window(float gx, float gy, float *wx, float *wy)
{
    *wx = (float)(picture.x + gx * picture.w / pic_w);
    *wy = (float)(picture.y + gy * picture.h / pic_h);
}
// A mouse event's window coordinates to the game's pixels (what the renderer's logical
// presentation did before Scout1248).
void convert_event(SDL_Event &e)
{
    double sx = picture.w > 0 ? pic_w / picture.w : 1, sy = picture.h > 0 ? pic_h / picture.h : 1;
    if (e.type == SDL_EVENT_MOUSE_MOTION) {
        window_to_game(e.motion.x, e.motion.y, &e.motion.x, &e.motion.y);
        e.motion.xrel = (float)(e.motion.xrel * sx);
        e.motion.yrel = (float)(e.motion.yrel * sy);
    } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        window_to_game(e.button.x, e.button.y, &e.button.x, &e.button.y);
    } else if (e.type == SDL_EVENT_MOUSE_WHEEL) {
        window_to_game(e.wheel.mouse_x, e.wheel.mouse_y, &e.wheel.mouse_x, &e.wheel.mouse_y);
    }
}
// The toggles' places in window points: a column at the usable part's left edge.
hostos::Rect toggle_rect(int i)
{
    double size = 44, gap = 6;
    return hostos::Rect{usable.x + 6, usable.y + 6 + i * (size + gap), size, size};
}
int toggle_at(float wx, float wy)
{
    if (!touch_ui || system_toolbar) return -1;
    for (int i = 0; i < kToggleCount; ++i) {
        hostos::Rect r = toggle_rect(i);
        if (wx >= r.x && wx < r.x + r.w && wy >= r.y && wy < r.y + r.h) return i;
    }
    return -1;
}

void *create_window(const char *title, int width, int height)
{
    if (!renderer) {
        // Resizable, HiDPI (the renderer draws in device pixels; the picture
        // keeps its aspect, letterboxed).
        SDL_SetHint(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0");
        // The click that brings the window to the front also reaches the game, as on Windows
        // (WM_MOUSEACTIVATE answered MA_ACTIVATE): a click on the lobby's name field with
        // another app in front was lost (Scout1223).
        SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
        // A window the app made already (iOS: the import's; ra2_sdl_host_use_window) is the game's.
        if (window) SDL_SetWindowTitle(window, title);
        else window = SDL_CreateWindow(title, width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
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
        // The game picture fills the usable part of the window, aspect kept (update_layout);
        // mouse positions convert to the game's pixels through it.
        pic_w = width;
        pic_h = height;
        layout_dirty = true;
        // Text input (WM_CHAR) from a keyboard; not where it would raise the on-screen
        // keyboard over the game (iOS, Scout1247).
        if (!SDL_HasScreenKeyboardSupport()) SDL_StartTextInput(window);
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
    // USER32 holds presents while the game redraws its menu after a focus change (until
    // the dialogs have repainted).
    if (ra2port_presents_held && ra2port_presents_held()) return;
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
bool button_down[2];  // the game was told the button is down (and not yet up)
void press_button(int b, float x, float y)
{
    button_down[b] = true;
    ra2port::set_key_state(b ? 0x02 : 0x01, true);
    ra2port::post_message(ra2port::main_window(), b ? 0x0204 : 0x0201, 0, mouse_lparam(x, y));   // WM_RBUTTONDOWN / WM_LBUTTONDOWN
}
void release_button(int b)
{
    held[b].release_pending = false;
    button_down[b] = false;
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

// RA2_NATIVE_NSEVENT_KEYS="<ms>:<text>[;<ms>:<text>...]" (test scaffolding, Scout1223): at that
// many milliseconds after start the host posts key-down and key-up events for each character
// ('<' is Backspace, '|' Return; a text "@X,Y" is a left click at game pixel X, Y) into the
// application's own event queue (hostos::post_system_keys/_click: AppKit events on the Mac), so
// they take the real keyboard's path (NSApplication sendEvent, SDL's key handling and text input)
// without the Accessibility permission that posting system keyboard events needs.
void post_nsevent_keys()
{
    struct Group { Uint64 ms; std::string text; };
    static std::vector<Group> groups;
    static bool parsed = false;
    if (!parsed) {
        parsed = true;
        const char *env = getenv("RA2_NATIVE_NSEVENT_KEYS");
        for (const char *p = env; p && *p;) {
            char *end = nullptr;
            Uint64 ms = strtoull(p, &end, 10);
            if (!end || *end != ':') break;
            const char *t = end + 1, *stop = strchr(t, ';');
            groups.push_back({ms, std::string(t, stop ? (size_t)(stop - t) : strlen(t))});
            p = stop ? stop + 1 : nullptr;
        }
    }
    if (groups.empty() || !window || SDL_GetTicks() < groups.front().ms) return;
    std::string text = groups.front().text;
    groups.erase(groups.begin());
    void *native = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    if (!native) return;
    if (text[0] == '@') {
        // "@X,Y": a left click at game pixel X, Y.
        float gx = 0, gy = 0, wx = 0, wy = 0;
        if (sscanf(text.c_str() + 1, "%f,%f", &gx, &gy) != 2 || !renderer) return;
        game_to_window(gx, gy, &wx, &wy);
        SDL_WarpMouseInWindow(window, wx, wy);   // AppKit reads the pointer's own position for the click
        if (hostos::post_system_click(native, wx, wy))
            fprintf(stderr, "nsevent-keys: clicked %g,%g (window %g,%g)\n", gx, gy, wx, wy);
        return;
    }
    if (hostos::post_system_keys(native, text.c_str())) fprintf(stderr, "nsevent-keys: posted \"%s\"\n", text.c_str());
}

void mouse_move(float x, float y)
{
    pointer_x = x;
    pointer_y = y;
    ra2port::post_message(ra2port::main_window(), 0x0200, 0, mouse_lparam(x, y));
}
void mouse_button(int b, bool down, float x, float y)
{
    if (down) {
        if (held[b].release_pending) release_button(b);   // a second tap before the first was seen
        held[b].down_frame = ra2port_last_frame();
        held[b].down_ms = SDL_GetTicks();
        press_button(b, x, y);
    } else if (!seen_held(b)) {
        // Released before the game saw the button held: kept down until it has.
        held[b].release_pending = true;
        held[b].x = x;
        held[b].y = y;
    } else {
        held[b].x = x;
        held[b].y = y;
        release_button(b);
    }
}

// The host pause's reasons combined (Scout1248): the background (iOS), the focus lost
// (settled), folded, an audio interruption.
void update_pause()
{
    bool want = in_background || focus_paused || folded || audio_interrupted;
    if (want != ra2port::pause_requested())
        fprintf(stderr, "host: pause %s (background %d focus %d folded %d audio %d)\n", want ? "on" : "off",
                in_background, focus_paused, folded, audio_interrupted);
    ra2port::request_pause(want);
}

// Touch gestures out (touch.hpp): the same mouse, pan and wheel paths as the mouse's.
void touch_pan(float dx, float dy, float x, float y)
{
    if (pan_last_ms && SDL_GetTicks() - pan_last_ms > 250) pan_dx = pan_dy = pan_wheel = 0;
    pointer_x = x;
    pointer_y = y;
    pan_dx += dx;
    pan_dy += dy;
    pan_last_ms = SDL_GetTicks();
}
void touch_wheel(int notches, float x, float y)
{
    int delta = notches * 120;
    ra2port::post_message(ra2port::main_window(), 0x020A, (WPARAM)((delta & 0xFFFF) << 16), mouse_lparam(x, y));
}
// A touch drag after its press (the selection box): the moves wait until the game has
// seen the button held (seen_held), as a mouse's press lasts frames before it moves;
// the game took a press and an immediate move as a click where the finger lifted.
bool touch_move_waiting = false;
float touch_move_x = 0, touch_move_y = 0;
void touch_moves_due();
void touch_button(int b, bool down, float x, float y)
{
    if (!down && touch_move_waiting) {          // the drag's last move before its release
        touch_move_waiting = false;
        mouse_move(touch_move_x, touch_move_y);
    }
    fprintf(stderr, "touch: %s %s at %.0f,%.0f\n", b ? "right" : "left", down ? "down" : "up", x, y);
    mouse_button(b, down, x, y);
}
void touch_pan_logged(float dx, float dy, float x, float y)
{
    static Uint64 last = 0;
    if (SDL_GetTicks() - last > 500) fprintf(stderr, "touch: pan from %.0f,%.0f\n", x, y);
    last = SDL_GetTicks();
    touch_pan(dx, dy, x, y);
}
void touch_wheel_logged(int notches, float x, float y)
{
    fprintf(stderr, "touch: wheel %+d at %.0f,%.0f\n", notches, x, y);
    touch_wheel(notches, x, y);
}
bool left_down_unseen()
{
    return button_down[0] && !seen_held(0);
}
void touch_move(float x, float y)
{
    if (left_down_unseen()) {
        touch_move_waiting = true;
        touch_move_x = x;
        touch_move_y = y;
        return;
    }
    touch_move_waiting = false;
    mouse_move(x, y);
}
void touch_moves_due()
{
    if (touch_move_waiting && !left_down_unseen()) touch_move(touch_move_x, touch_move_y);
}
const touch::Output touch_out = {touch_move, touch_button, touch_pan_logged, touch_wheel_logged};

void redraw();
// The glass toolbar shows the toggles' states and the scale (Scout1249).
void sync_toolbar()
{
    if (!system_toolbar) return;
    char scale[8];
    snprintf(scale, sizeof scale, "%gx", ui_scale);
    for (int i = 0; i < kToggleCount; ++i)
        hostos::toolbar_set(i, toggles[i].on || (i == kKeysToggle && text_input_on), i == kScaleToggle ? scale : nullptr);
}
void press_toggle(int i);
void toolbar_tapped(int i) { press_toggle(i); }
void press_toggle(int i)
{
    Toggle &t = toggles[i];
    HWND w = ra2port::main_window();
    if (i == kScaleToggle) {
        ui_scale = ui_scale < 1.1 ? 1.25 : ui_scale < 1.4 ? 1.5 : 1.0;
        fprintf(stderr, "host: interface scale %.2f (at the next start)\n", ui_scale);
        if (scale_changed) scale_changed(ui_scale);
    } else {
        t.on = !t.on;
        if (t.vk) {
            ra2port::set_key_state(t.vk, t.on);
            ra2port::post_message(w, t.on ? 0x0100 : 0x0101, (WPARAM)t.vk, (LPARAM)(t.on ? 1 : (1 | (3u << 30))));
        }
        fprintf(stderr, "host: toggle %s %s\n", t.label, t.on ? "on" : "off");
    }
    sync_toolbar();
    redraw();
}

// SDL's finger events: direct touches only (a Mac trackpad's touches are its mouse and
// wheel). Window-normalised coordinates to points, then to game pixels; scripted fingers
// (input script tdown/tmove/tup) are in game pixels already.
bool unreported_finger = false;   // a finger reached the game during a lifecycle pause (resume_unreported_foreground)
void finger_event(SDL_TouchFingerEvent const &f, Uint32 type)
{
    bool scripted = f.touchID == script_touch;
    if (!scripted && (!touch_ui || SDL_GetTouchDeviceType(f.touchID) != SDL_TOUCH_DEVICE_DIRECT)) return;
    float gx = f.x, gy = f.y;
    if (!scripted && type == SDL_EVENT_FINGER_DOWN && (in_background || (pause_on_focus_loss && !window_focused)))
        unreported_finger = true;     // resume_unreported_foreground
    if (!scripted) {
        int ww = 0, wh = 0;
        SDL_GetWindowSize(window, &ww, &wh);
        float wx = f.x * ww, wy = f.y * wh;
        if (type == SDL_EVENT_FINGER_DOWN) {
            int t = toggle_at(wx, wy);
            if (t >= 0) {
                toggle_finger = f.fingerID;
                press_toggle(t);
                return;
            }
        }
        if (toggle_finger && f.fingerID == toggle_finger) {
            if (type == SDL_EVENT_FINGER_UP || type == SDL_EVENT_FINGER_CANCELED) toggle_finger = 0;
            return;
        }
        window_to_game(wx, wy, &gx, &gy);
    }
    touch::set_screen(pic_w, pic_h, ra2port_menu_shown() ? 0 : pic_w - 168);
    // When the finger moved, not when the game read the queue (Scout1274): taps queued during
    // one long frame (menus pump seldom) were otherwise read at one time, and two separate
    // quick taps became a two-finger tap. SDL's event timestamps share SDL_GetTicks' clock
    // (nanoseconds); scripted fingers carry none.
    Uint64 now = SDL_GetTicks();
    if (f.timestamp && f.timestamp / 1000000 <= now) now = f.timestamp / 1000000;
    if (type == SDL_EVENT_FINGER_DOWN) touch::finger_down(f.fingerID, gx, gy, now, touch_out);
    else if (type == SDL_EVENT_FINGER_MOTION) touch::finger_move(f.fingerID, gx, gy, now, touch_out);
    else touch::finger_up(f.fingerID, gx, gy, now, touch_out);
}

// A lifecycle pause whose end SDL never reported (Scout1274; user report: on the iPhone
// Duo the game froze after the glass toolbar was closed). SDL turns the scene's resign
// active into WILL_ENTER_BACKGROUND and a keyboard focus loss, and only its become active
// into the foreground and the focus back. The Duo's logs show "host: background" and
// "focus lost: pause" with touches still reaching the game afterwards and no foreground:
// the pause held for good. When the app has been active again for half a second, or a
// finger reaches the game view (only an active app gets those), the host ends both
// itself, as a foreground would (the game then hears WM_ACTIVATEAPP once more).
// A finger whose lift never reached the gestures (Scout1274; user report: on the Duo the
// taps stopped working for good while the game ran on): touch.cpp then waited for it, a
// two-finger gesture never ended and every later touch was ignored. After the queue is
// drained SDL's own finger lists are current; a finger the gestures hold that no touch
// device has listed for 250 ms is forgotten. iOS only (system_toolbar): scripted fingers
// on the Mac are in no SDL list.
void forget_lost_fingers()
{
    static uint64_t missing_id = 0;
    static Uint64 missing_since = 0;
    if (!system_toolbar || !touch::active()) { missing_since = 0; return; }
    uint64_t held[10];
    int n = touch::finger_list(held, 10);
    int devices = 0;
    SDL_TouchID *ids = SDL_GetTouchDevices(&devices);
    uint64_t lost = 0;
    bool found_lost = false;
    for (int i = 0; i < n && !found_lost; ++i) {
        bool listed = false;
        for (int d = 0; d < devices && !listed; ++d) {
            int count = 0;
            SDL_Finger **fingers = SDL_GetTouchFingers(ids[d], &count);
            for (int k = 0; k < count && !listed; ++k) listed = fingers[k]->id == (SDL_FingerID)held[i];
            SDL_free(fingers);
        }
        if (!listed) { lost = held[i]; found_lost = true; }
    }
    SDL_free(ids);
    Uint64 now = SDL_GetTicks();
    if (!found_lost) { missing_since = 0; return; }
    if (!missing_since || missing_id != lost) { missing_id = lost; missing_since = now; return; }
    if (now - missing_since < 250) return;
    touch::forget_finger(lost, now, touch_out);
    missing_since = 0;
}

void resume_unreported_foreground()
{
    static Uint64 active_since = 0;
    bool stuck = in_background || (pause_on_focus_loss && !window_focused);
    if (!stuck || !touch_ui) { active_since = 0; unreported_finger = false; return; }
    Uint64 now = SDL_GetTicks();
    bool active = hostos::app_active();
    if (!active) active_since = 0;
    else if (!active_since) active_since = now;
    if (!unreported_finger && !(active_since && now - active_since >= 500)) return;
    fprintf(stderr, "host: active again with no foreground from SDL (%s): resume\n", unreported_finger ? "a touch" : "app active");
    unreported_finger = false;
    active_since = 0;
    if (in_background) {
        in_background = false;
        layout_dirty = true;
        update_pause();
    }
    if (!window_focused) {
        window_focused = true;
        window_focus_ms = now - kFocusSettleMs;
    }
}

void pump_events()
{
    post_nsevent_keys();
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
        if (renderer && !scripted) convert_event(e);
        switch (e.type) {
        case SDL_EVENT_QUIT:
            quit();
            break;
        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
            apply_mouse_grab();
            layout_dirty = true;
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
            if (e.motion.which == SDL_TOUCH_MOUSEID) break;     // touches have their own gestures (touch.cpp)
            clamp_to_picture(e.motion.x, e.motion.y);
            mouse_move(e.motion.x, e.motion.y);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN: case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (e.button.which == SDL_TOUCH_MOUSEID) break;
            clamp_to_picture(e.button.x, e.button.y);
            if (e.button.button != SDL_BUTTON_LEFT && e.button.button != SDL_BUTTON_RIGHT) break;
            mouse_button(e.button.button == SDL_BUTTON_RIGHT ? 1 : 0, e.type == SDL_EVENT_MOUSE_BUTTON_DOWN, e.button.x, e.button.y);
            break;
        }
        case SDL_EVENT_FINGER_DOWN: case SDL_EVENT_FINGER_MOTION: case SDL_EVENT_FINGER_UP: case SDL_EVENT_FINGER_CANCELED:
            finger_event(e.tfinger, e.type);
            break;
        case SDL_EVENT_WINDOW_RESIZED: case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
        case SDL_EVENT_DISPLAY_ORIENTATION:
            if (e.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) apply_mouse_grab();
            layout_dirty = true;
            break;
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
        // Each WM_ACTIVATEAPP makes the game redraw its menu background over the dialogs
        // (Windows_Procedure 0x007375A0 sets 0x008523DC), which USER32 repaints afterwards.
        // macOS reports short focus bounces (a click on another app's window that returns,
        // Cmd+Tab passing over the game, a focus report while the app activates): told of
        // each one the game redrew the menu without its buttons, and a loss right after a
        // gain left them missing while the window was in the background (user report,
        // Scout1223). The game hears of a focus change once it has held for 150 ms.
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            window_focused = e.type == SDL_EVENT_WINDOW_FOCUS_GAINED;
            window_focus_ms = SDL_GetTicks();
            break;
        }
    }
    resume_unreported_foreground();
    if (window_focused != game_active && SDL_GetTicks() - window_focus_ms >= kFocusSettleMs) {
        game_active = window_focused;
        ra2port::post_message(w, 0x001C, game_active ? 1 : 0, 0);   // WM_ACTIVATEAPP
        // The host pause (Scout1247): the game stops where it is while another app is in
        // front, its clocks with it (a mission's timers and the movies do not run on), once
        // it has read the WM_ACTIVATEAPP above (the pause takes hold at the next empty
        // queue read). Not in a network game, which the other players' games pace.
        if (pause_on_focus_loss) {
            bool pause = !game_active && !ra2_native_network_game();
            if (pause != focus_paused) fprintf(stderr, "host: focus %s: %s\n", game_active ? "back" : "lost", pause ? "pause" : "resume");
            focus_paused = pause;
            update_pause();
        }
    }
    // Touch: a finger held still becomes a hold; the on-screen keyboard follows an edit
    // control's focus or the KEYS toggle; the layout follows rotation, folding and the
    // safe area (Scout1248).
    touch::tick(SDL_GetTicks(), touch_out);
    forget_lost_fingers();
    touch_moves_due();
    static const bool keep_cursor = getenv("RA2_NATIVE_TOUCH_CURSOR") != nullptr;
    if (touch_ui && !keep_cursor) ra2_native_hide_game_cursor();
    if (SDL_HasScreenKeyboardSupport() && window) {
        bool want = toggles[kKeysToggle].on || ra2port_text_input_wanted();
        if (want != text_input_on) {
            text_input_on = want;
            if (want) SDL_StartTextInput(window); else SDL_StopTextInput(window);
            sync_toolbar();
        }
    }
    if (layout_dirty) {
        bool was = folded;
        update_layout();
        if (was != folded) update_pause();
        redraw();
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

// The touch controls over the picture (Scout1248): the toggles, and while the device
// is folded a note in place of the game.
void draw_overlay(float density)
{
    if (folded) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderScale(renderer, 2 * density, 2 * density);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDebugText(renderer, (float)(usable.x / 2 + 8), (float)(usable.y / 2 + usable.h / 4), "Open the phone to play.");
        SDL_SetRenderScale(renderer, 1, 1);
        return;
    }
    if (!touch_ui || system_toolbar) return;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (int i = 0; i < kToggleCount; ++i) {
        hostos::Rect r = toggle_rect(i);
        SDL_FRect box = {(float)(r.x * density), (float)(r.y * density), (float)(r.w * density), (float)(r.h * density)};
        if (toggles[i].on) SDL_SetRenderDrawColor(renderer, 200, 30, 20, 210);
        else SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_RenderFillRect(renderer, &box);
        SDL_SetRenderDrawColor(renderer, 230, 200, 60, 230);
        SDL_RenderRect(renderer, &box);
        char label[8];
        if (i == kScaleToggle) snprintf(label, sizeof label, "%gx", ui_scale);
        else snprintf(label, sizeof label, "%s", toggles[i].label);
        float text_scale = density * 1.25f;
        float tw = 8.0f * (float)strlen(label);
        SDL_SetRenderScale(renderer, text_scale, text_scale);
        SDL_RenderDebugText(renderer, (box.x + box.w / 2) / text_scale - tw / 2, (box.y + box.h / 2) / text_scale - 4, label);
        SDL_SetRenderScale(renderer, 1, 1);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void present_now(const void *pixels, int width, int height, int pitch)
{
    if (in_background || !renderer) return;     // iOS: no drawing in the background
    last_present_ms = SDL_GetTicks();
    if (!texture || tex_w != width || tex_h != height) {
        if (texture) SDL_DestroyTexture(texture);
        // The game's resolution (its DirectDraw display mode): the window
        // takes that size (in points; fullscreen keeps the display) and the
        // mouse maps onto those pixels.
        pic_w = width;
        pic_h = height;
        fit_window(width, height);
        update_layout();
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, width, height);
        // Sharp pixels at any scale (Retina 2x, fullscreen's fractional fit).
        if (texture) SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
        tex_w = width;
        tex_h = height;
    }
    if (layout_dirty) update_layout();
    if (pixels) SDL_UpdateTexture(texture, nullptr, pixels, pitch);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    float density = pixel_density();
    SDL_FRect dst = {(float)(picture.x * density), (float)(picture.y * density), (float)(picture.w * density), (float)(picture.h * density)};
    if (texture) SDL_RenderTexture(renderer, texture, nullptr, &dst);
    draw_overlay(density);
    // RA2_NATIVE_WINDOW_DUMP=<file> (test runs): every 2 s, the whole window
    // as drawn (device pixels: the scaling, letterbox and fullscreen) to
    // <file>.<n>.bmp; RA2_NATIVE_WINDOW_DUMP_MS=<ms> sets another period (0: every present).
    static const char *window_dump = getenv("RA2_NATIVE_WINDOW_DUMP");
    static const char *window_dump_period = getenv("RA2_NATIVE_WINDOW_DUMP_MS");
    static Uint64 window_dump_ms = 0;
    static int window_dump_count = 0;
    if (window_dump && *window_dump && (window_dump_count == 0 ||
            SDL_GetTicks() - window_dump_ms >= (Uint64)(window_dump_period ? atoi(window_dump_period) : 2000))) {
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

// The last picture again (a toggle changed, the layout moved, folded).
void redraw()
{
    if (SDL_IsMainThread() && texture) present_now(nullptr, tex_w, tex_h, 0);
}

void set_relative_cursor(bool show)
{
    if (show) SDL_ShowCursor(); else SDL_HideCursor();
}

// Audio: an SDL3 playback stream (stereo, signed 16-bit) that asks the port's
// DirectSound mixer for every block the device consumes.
void (*audio_mix)(short *out, int frames) = nullptr;
SDL_AudioStream *audio_stream = nullptr;
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
    audio_stream = stream;
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
        } else if (action == "tdown" || action == "tmove" || action == "tup") {
            // tdown|tmove|tup X Y [finger]: a touch at game pixel X, Y (finger 1 by default;
            // the touch gestures' tests, Scout1248).
            Point a = script_point(tokens, i);
            ok = a.ok && a.spec.empty();
            if (ok) {
                Scripted s = {frame, {}, std::string()};
                if (script_ms >= 0) { s.ms = script_ms; s.frame = 0x7FFFFFFF; s.dialog = script_dialog; }
                s.event.type = action == "tdown" ? SDL_EVENT_FINGER_DOWN : action == "tmove" ? SDL_EVENT_FINGER_MOTION : SDL_EVENT_FINGER_UP;
                s.event.tfinger.touchID = script_touch;
                s.event.tfinger.fingerID = i < tokens.size() ? (SDL_FingerID)atoi(tokens[i].c_str()) : 1;
                s.event.tfinger.x = a.x;
                s.event.tfinger.y = a.y;
                script.push_back(s);
            }
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
    // Every 30 s of play, the frame rate and (iOS) memory and thermal state in the log
    // (Scout1248).
    {
        static Uint64 since = 0;
        static int frames = 0;
        Uint64 now = SDL_GetTicks();
        if (!since) since = now;
        ++frames;
        if (now - since >= 30000) {
            std::string device = hostos::device_report();
            fprintf(stderr, "host: stats: %.1f frames/s%s%s\n", frames * 1000.0 / (double)(now - since), device.empty() ? "" : ", ", device.c_str());
            since = now;
            frames = 0;
        }
    }
    // Periodic checkpoints while a game runs (Scout1248): the game side saves at the end
    // of this Main_Loop pass.
    if (checkpoints && checkpoint_period_ms) {
        Uint64 now = SDL_GetTicks();
        if (!last_checkpoint_ms) last_checkpoint_ms = now;
        else if (now - last_checkpoint_ms >= checkpoint_period_ms) {
            last_checkpoint_ms = now;
            ra2port::request_checkpoint();
        }
    }
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
        } else if (e.type == SDL_EVENT_FINGER_DOWN || e.type == SDL_EVENT_FINGER_MOTION || e.type == SDL_EVENT_FINGER_UP) {
            e.tfinger.windowID = id;
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
                game_to_window(x, y, &wx, &wy);
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

// Host::held: the port's pause hold began or ended (Scout1247). The audio device stops
// with it (the mixer's play cursors stand still, as the game's clocks do) and starts again.
void game_held(bool on)
{
    if (audio_stream) {
        if (on) SDL_PauseAudioStreamDevice(audio_stream);
        else SDL_ResumeAudioStreamDevice(audio_stream);
    }
    fprintf(stderr, "host: game %s\n", on ? "held (paused)" : "running again");
}

const ra2port::Host sdl_host = {create_window, pump_events, present, set_relative_cursor, open_audio, frame, wait_begins, keep_alive, game_held};

// Lifecycle (Scout1248): SDL calls this as each event is queued, so going to the
// background takes effect before the game reads its queue again: a checkpoint is asked
// for, the pause is requested and nothing more is drawn.
bool SDLCALL lifecycle_watch(void *, SDL_Event *e)
{
    switch (e->type) {
    case SDL_EVENT_WILL_ENTER_BACKGROUND: case SDL_EVENT_DID_ENTER_BACKGROUND:
        if (!in_background) {
            fprintf(stderr, "host: background\n");
            in_background = true;
            if (checkpoints) ra2port::request_checkpoint();
            update_pause();
        }
        break;
    case SDL_EVENT_WILL_ENTER_FOREGROUND: case SDL_EVENT_DID_ENTER_FOREGROUND:
        if (in_background) {
            fprintf(stderr, "host: foreground\n");
            in_background = false;
            layout_dirty = true;
            update_pause();
        }
        break;
    default:
        break;
    }
    return true;
}
void audio_interruption(bool on)
{
    fprintf(stderr, "host: audio interruption %s\n", on ? "began" : "ended");
    audio_interrupted = on;
    update_pause();
}
}  // namespace

void ra2_install_sdl_host()
{
    ra2port::set_host(&sdl_host);
    if (const char *v = getenv("RA2_NATIVE_FOCUS_PAUSE")) pause_on_focus_loss = *v == '1';
    // Touches make their own gestures (touch.cpp), never SDL's mouse.
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    touch_ui = SDL_HasScreenKeyboardSupport();
    if (const char *v = getenv("RA2_NATIVE_TOUCH_UI")) touch_ui = *v == '1';
    if (const char *v = getenv("RA2_NATIVE_CHECKPOINT_SECONDS")) {
        checkpoint_period_ms = (Uint64)atoi(v) * 1000u;
        checkpoints = true;
    }
    SDL_AddEventWatch(lifecycle_watch, nullptr);
    hostos::watch_audio_interruptions(audio_interruption);
}
// Mac app (main.cpp): start in fullscreen; what to save before a quit ends the
// process.
void ra2_sdl_host_options(bool app, bool fullscreen, void (*on_quit)())
{
    app_mode = app;
    want_fullscreen = fullscreen;
    before_exit = on_quit;
    pause_on_focus_loss = app;
}
// The touch app (iOS, main.cpp; Scout1248): the interface scale shown on its toggle and
// where a new choice goes; checkpoints when it goes to the background and every
// `checkpoint_seconds` of play.
void ra2_sdl_host_touch_options(double scale, void (*on_scale)(double), int checkpoint_seconds)
{
    ui_scale = scale;
    scale_changed = on_scale;
    checkpoints = true;
    if (!getenv("RA2_NATIVE_CHECKPOINT_SECONDS")) checkpoint_period_ms = (Uint64)checkpoint_seconds * 1000u;
}
// The app's own window becomes the game's (iOS: made before the game starts, for
// the import; Scout1247).
void ra2_sdl_host_use_window(SDL_Window *made) { window = made; }
// The window's fullscreen state at exit (remembered in the settings).
bool ra2_sdl_host_fullscreen() { return is_fullscreen(); }
