// USER32 on the SDL3 host: window objects (handle tokens) with their window
// procedures, the thread message queue fed by the host's event pump, timers,
// input state and the small rectangle/string helpers. Dialog boxes need the
// templates from the user's game executable; until those are read at run time
// they are logged failures (CreateDialog* return NULL, DialogBox* return -1).
#include "ra2port/win32_port.hpp"

#include <chrono>
#include <deque>
#include <map>
#include <mutex>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <string>
#include <thread>
#include <vector>

using namespace ra2port;

namespace {

struct Window : HandleObject {
    WNDPROC proc = nullptr;
    DLGPROC dlgproc = nullptr;
    HWND parent = nullptr;
    RECT rect = {0, 0, 0, 0};
    LONG style = 0, exstyle = 0, id = 0;
    intptr_t userdata = 0;            // GWL_USERDATA, pointer-sized (ra2port_set_window_ptr)
    bool dirty = false;               // needs WM_PAINT (dialogs and controls)
    // Predefined control state (Scout1119, M14): button check/push, list and
    // combo items (text and item data), selection, top index, trackbar/progress
    // position and range, tab items.
    bool pushed = false, captured_inside = false, dropped = false;
    struct Item { std::string text; intptr_t data = 0; };
    std::vector<Item> items;
    int cursel = -1, top = 0, item_height = 16, pos = 0, range_min = 0, range_max = 100;
    bool visible = false, enabled = true;
    std::string cls, title;
    void *host_window = nullptr;
    // Dialogs built from a template: the controls in template order, the
    // DWL_USER slot (index 8; pointer-sized natively), the modal result.
    std::vector<HWND> children;
    bool is_dialog = false, ended = false, checked = false;
    HWND focus_child = nullptr;   // a dialog: the control that last had the focus (SetFocus)
    // A drop-down combo box (Scout1121): the window is its selection field
    // (sel_height + 6 high, as Windows sizes it); the size the template or a
    // move gives is the dropped size (CB_GETDROPPEDCONTROLRECT).
    int sel_height = 16, drop_height = 0;
    intptr_t dialog_user = 0, result = 0;
    unsigned template_id = 0;
};

struct WNDCLASSA_PORT {
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra, cbWndExtra;
    HINSTANCE hInstance;
    void *hIcon, *hCursor, *hbrBackground;
    const char *lpszMenuName, *lpszClassName;
};

std::mutex queue_lock;
std::deque<MSG> queue;
std::map<std::string, WNDPROC> classes;
HWND main_hwnd = nullptr, focus_hwnd = nullptr, capture_hwnd = nullptr;
std::vector<HWND> dialog_windows;   // dialogs in creation order (mouse routing: the last visible one is on top)
int cursor_count = 0;
int dialogs_shown = 0;   // dialogs from templates shown so far (--run-to `menu`)
std::vector<DWORD> dialog_show_times;   // when each was shown (scripted input "d<k>+<ms>")
POINT cursor = {0, 0};
// The screen size: the desktop until the game sets a DirectDraw display mode.
int screen_width = 800, screen_height = 600;

struct Timer { HWND window; uintptr_t id; UINT period; std::chrono::steady_clock::time_point next; void *proc; };
std::vector<Timer> timers;

Window *win(HWND h) { return (Window *)handle_get(h, H_WINDOW); }

// A window's client origin on the screen: child rectangles are relative to
// their parent's client area (the main window and dialogs are top-level).
POINT screen_origin(HWND h)
{
    POINT p = {0, 0};
    for (Window *w = win(h); w; w = win(w->parent)) {
        p.x += w->rect.left;
        p.y += w->rect.top;
        if (!w->parent || w->parent == main_hwnd) break;
    }
    return p;
}

// Marks a dialog or control (and its children) for WM_PAINT.
void invalidate(HWND h)
{
    Window *w = win(h);
    if (!w) return;
    if (w->is_dialog || w->parent) w->dirty = true;
    for (HWND c : w->children) invalidate(c);
}

// A CBS_DROPDOWN or CBS_DROPDOWNLIST combo box: its height is the selection field's.
bool closed_combo(const Window *w)
{
    return w && (w->style & 3) != 1 && !strcasecmp(w->cls.c_str(), "ComboBox");
}
int combo_height(Window *w, int h)
{
    if (!closed_combo(w)) return h;
    w->drop_height = h;
    return w->sel_height + 6;
}

DWORD now_ms()
{
    return (DWORD)std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

void fire_timers()
{
    auto now = std::chrono::steady_clock::now();
    for (auto &t : timers) {
        if (now >= t.next) {
            t.next = now + std::chrono::milliseconds(t.period);
            post_message(t.window, 0x0113, t.id, (LPARAM)t.proc);   // WM_TIMER
        }
    }
}

}  // namespace

namespace ra2port {
HWND main_window() { return main_hwnd; }
// DirectDraw changed the display mode (exclusive full screen): as on Windows,
// the screen takes that size and the game's top-level window (a full-screen
// popup) covers it; the window gets WM_MOVE and WM_SIZE, whose handler
// recomputes the mouse's confining rectangle (Windows_Procedure 0x007375A0,
// WWMouseClass::Calc_Confining_Rect 0x00771C50). Without it the mouse stayed
// clamped to the 800x600 window the game created (Scout1115, M13).
void display_mode_changed(int width, int height)
{
    screen_width = width;
    screen_height = height;
    Window *w = win(main_hwnd);
    if (!w) return;
    w->rect = {0, 0, width, height};
    post_message(main_hwnd, 0x0003, 0, 0);                                                  // WM_MOVE (0, 0)
    post_message(main_hwnd, 0x0005, 0, (LPARAM)((height & 0xFFFF) << 16 | (width & 0xFFFF)));   // WM_SIZE
    // Windows repaints every window after a display change: the open dialogs
    // (the Options page under the mode confirmation, moved by Set_Display_Mode
    // 0x005434E0's relayout loop) get WM_PAINT again (Scout1187, M23).
    for (HWND d : dialog_windows) invalidate(d);
}
BOOL post_message(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    std::lock_guard<std::mutex> g(queue_lock);
    MSG m = {window, message, wparam, lparam, now_ms(), cursor};
    queue.push_back(m);
    if (message == 0x0200 || message == 0x0201 || message == 0x0202 || message == 0x0204 || message == 0x0205) {
        cursor.x = (LONG)(int16_t)(lparam & 0xFFFF);
        cursor.y = (LONG)(int16_t)((lparam >> 16) & 0xFFFF);
    }
    return 1;
}
}  // namespace ra2port

extern "C" {

// ---- classes and windows -------------------------------------------------

}  // extern "C"
namespace { WNDPROC predefined_proc(const std::string &cls); }
extern "C" {
WORD RegisterClassA(const WNDCLASSA_PORT *wc)
{
    if (!wc || !wc->lpszClassName) return 0;
    classes[wc->lpszClassName] = wc->lpfnWndProc;
    return (WORD)(classes.size());
}

HWND CreateWindowExA(DWORD exstyle, const char *cls, const char *title, DWORD style, int x, int y, int w, int h,
                     HWND parent, void *menu, HINSTANCE instance, void *param)
{
    Window *wnd = new Window;
    wnd->kind = H_WINDOW;
    wnd->cls = cls ? cls : "";
    wnd->title = title ? title : "";
    wnd->style = (LONG)style;
    wnd->exstyle = (LONG)exstyle;
    wnd->parent = parent;
    auto it = classes.find(wnd->cls);
    wnd->proc = it != classes.end() ? it->second : predefined_proc(wnd->cls);
    if (parent && win(parent)) win(parent)->children.push_back(nullptr);   // placeholder, set below
    if (w <= 0 || w > 4096) w = 800;
    if (h <= 0 || h > 4096) h = 600;
    wnd->rect = {x < 0 || x > 4096 ? 0 : x, y < 0 || y > 4096 ? 0 : y, 0, 0};
    wnd->rect.right = wnd->rect.left + w;
    wnd->rect.bottom = wnd->rect.top + combo_height(wnd, h);
    HWND hwnd = handle_new(wnd);
    if (parent && win(parent)) win(parent)->children.back() = hwnd;
    wnd->visible = (style & 0x10000000u) != 0;
    wnd->id = parent ? (LONG)(intptr_t)menu : 0;
    {
        char detail[256];
        snprintf(detail, sizeof detail, "class \"%s\" title \"%s\" %dx%d", wnd->cls.c_str(), wnd->title.c_str(), w, h);
        progress_count("windows created", detail);
        if (!parent) progress("window", detail);
    }
    if (!parent && !main_hwnd) {
        main_hwnd = hwnd;
        focus_hwnd = hwnd;
        if (host() && host()->create_window) wnd->host_window = host()->create_window(wnd->title.c_str(), w, h);
    }
    if (wnd->proc) {
        // WM_CREATE with the CREATESTRUCTA (pointer-sized fields natively):
        // lpCreateParams first, as the combo drop list 0x005EAD20 reads its
        // owner combo box from it (Scout1121).
        struct { void *params; HINSTANCE instance; void *menu; HWND parent; int cy, cx, y, x; LONG style;
                 const char *name; const char *cls; DWORD exstyle; } cs =
            {param, instance, menu, parent, h, w, wnd->rect.top, wnd->rect.left, (LONG)style, title, cls, exstyle};
        wnd->proc(hwnd, 0x0001, 0, (LPARAM)&cs);   // WM_CREATE
    }
    return hwnd;
}

BOOL DestroyWindow(HWND hwnd)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    if (w->proc) w->proc(hwnd, 0x0002, 0, 0);   // WM_DESTROY
    w = win(hwnd);
    if (!w) return 1;
    // Children after their parent's WM_DESTROY, then WM_NCDESTROY (the
    // owner-draw subclass restores its tables there).
    std::vector<HWND> children = w->children;
    for (HWND c : children) DestroyWindow(c);
    w = win(hwnd);
    if (!w) return 1;
    if (w->proc) w->proc(hwnd, 0x0082, 0, 0);   // WM_NCDESTROY
    if (Window *parent = win(w->parent)) {
        for (size_t i = 0; i < parent->children.size(); ++i)
            if (parent->children[i] == hwnd) { parent->children.erase(parent->children.begin() + (long)i); break; }
    }
    bool uncovers = false;
    for (size_t i = 0; i < dialog_windows.size(); ++i)
        if (dialog_windows[i] == hwnd) { uncovers = w->visible; dialog_windows.erase(dialog_windows.begin() + (long)i); break; }
    // Windows invalidates what a closed window uncovers: the dialogs below a
    // closed visible dialog that it overlapped get WM_PAINT (the Options page
    // under the mode confirmation, whose screen Main_Options_Proc 0x00542590
    // refills after a resolution change; Scout1187, M23).
    if (uncovers) {
        for (HWND d : dialog_windows) {
            Window *o = win(d);
            if (o && o->visible && o->rect.left < w->rect.right && w->rect.left < o->rect.right
                && o->rect.top < w->rect.bottom && w->rect.top < o->rect.bottom) invalidate(d);
        }
    }
    if (capture_hwnd == hwnd) capture_hwnd = nullptr;
    if (focus_hwnd == hwnd) focus_hwnd = main_hwnd;
    if (hwnd == main_hwnd) main_hwnd = nullptr;
    handle_close(hwnd);
    return 1;
}

BOOL CloseWindow(HWND hwnd) { return win(hwnd) != nullptr; }
LRESULT SendMessageA(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL ShowWindow(HWND hwnd, int cmd)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool was = w->visible;
    w->visible = cmd != 0;
    if (w->visible && !was) invalidate(hwnd);
    // WM_SHOWWINDOW on a change, as Windows sends it (dialogs and controls;
    // the owner-draw layer resets its slide-in state there).
    if (w->visible != was && (w->is_dialog || w->parent) && w->proc) w->proc(hwnd, 0x0018, w->visible ? 1 : 0, 0);
    // --run-to `menu`: the first dialog built from a game.exe template that is
    // shown (Select_Game's main menu on the start-up path; the detail names
    // the template so a run log shows which one).
    if (!was && w->visible && w->is_dialog) {
        char detail[96];
        snprintf(detail, sizeof detail, "dialog #%u shown (%zu controls)", w->template_id, w->children.size());
        progress_count("dialogs shown", detail);
        if (dialogs_shown++ == 0) progress("menu", detail);
        dialog_show_times.push_back(now_ms());
        if (getenv("RA2_NATIVE_MSG_LOG") || getenv("RA2_NATIVE_DIALOG_LOG")) fprintf(stderr, "dialog shown #%d (template %u) t%llu\n", dialogs_shown, w->template_id, (unsigned long long)now_ms());
        // The main menu (its single-player button 0x683) shown again after a
        // game ran: the menu-to-game-to-menu loop closed (Scout1119, M14).
        extern int ra2port_last_frame(void);
        bool main_menu = false;
        for (HWND c : w->children) if (win(c) && win(c)->id == 0x683) main_menu = true;
        if (ra2port_last_frame() > 0 && main_menu) progress("menu-return", detail);
        if (getenv("RA2_NATIVE_MSG_LOG")) {
            POINT o = screen_origin(hwnd);
            fprintf(stderr, "dialog %p tmpl %u at %d,%d %dx%d\n", (void *)hwnd, w->template_id, (int)o.x, (int)o.y,
                    (int)(w->rect.right - w->rect.left), (int)(w->rect.bottom - w->rect.top));
            for (HWND c : w->children) {
                Window *cw = win(c);
                if (cw) fprintf(stderr, "  control %d %s style %x %s at %d,%d %dx%d \"%s\"\n", (int)cw->id, cw->cls.c_str(), (unsigned)cw->style,
                                cw->visible ? "shown" : "hidden", (int)(o.x + cw->rect.left), (int)(o.y + cw->rect.top),
                                (int)(cw->rect.right - cw->rect.left), (int)(cw->rect.bottom - cw->rect.top), cw->title.c_str());
            }
        }
    }
    // Showing the application's first top-level window makes it the active,
    // foreground window, as Windows does for a newly started program: the
    // window receives WM_ACTIVATEAPP(TRUE) and WM_ACTIVATE(WA_ACTIVE) then.
    // (Later focus changes come from the host's SDL focus events.)
    static bool activated = false;
    if (!was && w->visible && hwnd == main_hwnd && !activated) {
        activated = true;
        SendMessageA(hwnd, 0x001C, 1, 0);   // WM_ACTIVATEAPP
        SendMessageA(hwnd, 0x0006, 1, 0);   // WM_ACTIVATE, WA_ACTIVE
    }
    return was;
}
BOOL UpdateWindow(HWND hwnd)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    if (w->dirty && w->visible && w->proc) { w->dirty = false; w->proc(hwnd, 0x000F, 0, 0); }   // WM_PAINT
    for (HWND c : std::vector<HWND>(w->children)) UpdateWindow(c);
    return 1;
}
BOOL IsWindowVisible(HWND hwnd) { Window *w = win(hwnd); return w && w->visible; }
BOOL IsWindowEnabled(HWND hwnd) { Window *w = win(hwnd); return w && w->enabled; }
BOOL EnableWindow(HWND hwnd, BOOL enable)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool was_disabled = !w->enabled;
    w->enabled = enable;
    return was_disabled;
}
BOOL MoveWindow(HWND hwnd, int x, int y, int w, int h, BOOL repaint)
{
    (void)repaint;
    Window *wnd = win(hwnd);
    if (!wnd) return 0;
    wnd->rect = {x, y, x + w, y + combo_height(wnd, h)};
    return 1;
}
BOOL SetWindowPos(HWND hwnd, HWND after, int x, int y, int w, int h, UINT flags)
{
    (void)after;
    Window *wnd = win(hwnd);
    if (!wnd) return 0;
    if (!(flags & 0x0002)) { int cw = wnd->rect.right - wnd->rect.left, ch = wnd->rect.bottom - wnd->rect.top; wnd->rect = {x, y, x + cw, y + ch}; }
    if (!(flags & 0x0001)) { wnd->rect.right = wnd->rect.left + w; wnd->rect.bottom = wnd->rect.top + combo_height(wnd, h); }
    if (flags & 0x0040) wnd->visible = true;
    if (flags & 0x0080) wnd->visible = false;
    return 1;
}
BOOL GetClientRect(HWND hwnd, RECT *r)
{
    Window *w = win(hwnd);
    if (!w || !r) return 0;
    *r = {0, 0, w->rect.right - w->rect.left, w->rect.bottom - w->rect.top};
    return 1;
}
BOOL GetWindowRect(HWND hwnd, RECT *r)
{
    Window *w = win(hwnd);
    if (!w || !r) return 0;
    POINT o = screen_origin(hwnd);            // screen coordinates, as on Windows
    *r = {o.x, o.y, o.x + w->rect.right - w->rect.left, o.y + w->rect.bottom - w->rect.top};
    return 1;
}
BOOL AdjustWindowRect(RECT *r, DWORD style, BOOL menu) { (void)r; (void)style; (void)menu; return 1; }
BOOL AdjustWindowRectEx(RECT *r, DWORD style, BOOL menu, DWORD ex) { (void)r; (void)style; (void)menu; (void)ex; return 1; }
BOOL ClientToScreen(HWND hwnd, POINT *p) { if (win(hwnd) && p) { POINT o = screen_origin(hwnd); p->x += o.x; p->y += o.y; } return 1; }
BOOL ScreenToClient(HWND hwnd, POINT *p) { if (win(hwnd) && p) { POINT o = screen_origin(hwnd); p->x -= o.x; p->y -= o.y; } return 1; }
HWND GetParent(HWND hwnd) { Window *w = win(hwnd); return w ? w->parent : nullptr; }
HWND GetWindow(HWND hwnd, UINT cmd) { (void)hwnd; (void)cmd; return nullptr; }
HWND GetTopWindow(HWND hwnd) { (void)hwnd; return nullptr; }
HWND ra2port_window_at(POINT screen, bool enabled_only);
HWND WindowFromPoint(POINT p) { return ra2port_window_at(p, false); }
HWND ChildWindowFromPoint(HWND hwnd, POINT p)
{
    Window *w = win(hwnd);
    if (!w) return nullptr;
    for (auto it = w->children.rbegin(); it != w->children.rend(); ++it) {
        Window *c = win(*it);
        if (c && c->visible && p.x >= c->rect.left && p.x < c->rect.right && p.y >= c->rect.top && p.y < c->rect.bottom) return *it;
    }
    return hwnd;
}
HWND ChildWindowFromPointEx(HWND hwnd, POINT p, UINT flags) { (void)flags; return ChildWindowFromPoint(hwnd, p); }
HWND GetActiveWindow(void) { return main_hwnd; }
HWND SetActiveWindow(HWND hwnd) { HWND old = main_hwnd; (void)hwnd; return old; }
HWND GetForegroundWindow(void) { return main_hwnd; }
BOOL SetForegroundWindow(HWND hwnd) { return win(hwnd) != nullptr; }
BOOL BringWindowToTop(HWND hwnd) { return win(hwnd) != nullptr; }
HWND GetFocus(void) { return focus_hwnd; }
// The focus is in a visible edit control (the lobby's name field, a dialog's text
// box): the host shows the on-screen keyboard (iOS, Scout1248).
extern "C" int ra2port_text_input_wanted(void)
{
    Window *w = win(focus_hwnd);
    return w && w->visible && !strcasecmp(w->cls.c_str(), "Edit");
}
// The focus moves to a window, as on Windows: a control's dialog remembers it, and a dialog
// given the focus passes it on to the control that had it last (DefDlgProc's WM_SETFOCUS).
// Focus_Restore 0x00737400 calls SetFocus(dialog) at every activation: the lobby's name field
// lost the focus to its dialog and took no more keys after the window had been in the
// background (a macOS prompt or another app in front; user report, Scout1223).
void set_focus(HWND hwnd)
{
    Window *w = win(hwnd);
    if (w && w->is_dialog) {
        Window *c = win(w->focus_child);
        if (c && c->parent == hwnd && c->visible && c->enabled) hwnd = w->focus_child;
    } else if (w && w->parent) {
        if (Window *d = win(w->parent)) if (d->is_dialog) d->focus_child = hwnd;
    }
    // WM_KILLFOCUS to the window losing it and WM_SETFOCUS to the one gaining it, as Windows
    // sends them: the NewEdit control starts its caret blink on WM_SETFOCUS (0x005F22A0), and the
    // lobby's name field showed no caret (user report, Scout1223).
    HWND old = focus_hwnd;
    focus_hwnd = hwnd;
    if (old == hwnd) return;
    if (old && win(old)) SendMessageA(old, 0x0008, (WPARAM)hwnd, 0);         // WM_KILLFOCUS
    if (hwnd && win(hwnd) && focus_hwnd == hwnd) SendMessageA(hwnd, 0x0007, (WPARAM)old, 0);   // WM_SETFOCUS
}
HWND SetFocus(HWND hwnd) { HWND old = focus_hwnd; set_focus(hwnd); return old; }
HWND GetCapture(void) { return capture_hwnd; }
HWND SetCapture(HWND hwnd) { HWND old = capture_hwnd; capture_hwnd = hwnd; return old; }
BOOL ReleaseCapture(void) { capture_hwnd = nullptr; return 1; }
BOOL LockWindowUpdate(HWND hwnd) { (void)hwnd; return 1; }
void *GetMenu(HWND hwnd) { (void)hwnd; return nullptr; }

// GWL_WNDPROC (-4), GWL_ID (-12), GWL_STYLE (-16), GWL_EXSTYLE (-20), GWL_USERDATA (-21).
// A dialog's DWL_USER slot (index 8), pointer-sized: the native view stores and
// reads the pointers the recovered dialogs keep there through these
// (native_units.py rewrites SetWindowLongA(w, 8, (LONG)p) / GetWindowLongA(w, 8)).
extern "C" intptr_t ra2port_set_dialog_user(const volatile void *window, const volatile void *value)
{
    Window *w = win((HWND)window);
    if (!w) return 0;
    intptr_t old = w->dialog_user;
    w->dialog_user = (intptr_t)value;
    return old;
}
extern "C" intptr_t ra2port_get_dialog_user(const volatile void *window)
{
    Window *w = win((HWND)window);
    return w ? w->dialog_user : 0;
}

// GWL_WNDPROC (-4) and GWL_USERDATA (-21), pointer-sized (native_units.py
// rewrites the recovered Set/GetWindowLongA calls with a pointer to these;
// Win64's Set/GetWindowLongPtr). Subclassing replaces the window's procedure
// and returns the previous one (a predefined control's, or the dialog
// manager's for a dialog), which the subclass calls through CallWindowProcA.
extern "C" intptr_t ra2port_set_window_ptr(const volatile void *window, int index, intptr_t value)
{
    Window *w = win((HWND)window);
    if (!w) return 0;
    intptr_t old = 0;
    if (index == -4) { old = (intptr_t)w->proc; w->proc = (WNDPROC)value; }
    else if (index == -21) { old = w->userdata; w->userdata = value; }
    else if (index == 8) { old = w->dialog_user; w->dialog_user = value; }
    return old;
}
extern "C" intptr_t ra2port_get_window_ptr(const volatile void *window, int index)
{
    Window *w = win((HWND)window);
    if (!w) return 0;
    if (index == -4) return (intptr_t)w->proc;
    if (index == -21) return w->userdata;
    if (index == 8) return w->dialog_user;
    return 0;
}

// LONG is 32 bits as on Win32: a window procedure stored through here would be
// truncated, so GWL_WNDPROC is refused and logged.
LONG GetWindowLongA(HWND hwnd, int index)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    switch (index) {
    case -12: return w->id;
    // GWL_HWNDPARENT: the parent (owner) window; port handles fit a LONG. The
    // layout pass 0x00735BC0 subtracts its rectangle (was 0: GetWindowRect(0)
    // left the stack RECT unset and the menu buttons at random x, Scout1121).
    case -8: return (LONG)(intptr_t)w->parent;
    case -16: return (w->style & ~(LONG)0x18000000) | (w->visible ? 0x10000000 : 0) | (w->enabled ? 0 : 0x08000000);   // GWL_STYLE with the current WS_VISIBLE / WS_DISABLED
    case -20: return w->exstyle;
    case -21: return (LONG)w->userdata;
    case 8: return (LONG)w->dialog_user;     // DWL_USER (pointer stores use ra2port_set_dialog_user)
    // DWL_DLGPROC: the owner-draw control procedure (draft 0x005EE460) tests it
    // to tell a dialog from a control. Natively the pointer does not fit a LONG:
    // nonzero for a dialog with a procedure (only ever tested), Scout1119.
    case 4: return w->is_dialog && w->dlgproc ? (LONG)((intptr_t)w->dlgproc | 1) : 0;
    case -4: unimplemented("GetWindowLongA(GWL_WNDPROC)"); return 0;
    }
    return 0;
}
LONG SetWindowLongA(HWND hwnd, int index, LONG value)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    LONG old = GetWindowLongA(hwnd, index);
    switch (index) {
    case -12: w->id = value; break;
    case -16:                       // GWL_STYLE: WS_VISIBLE and WS_DISABLED are the window's state, as on
        w->style = value;           // Windows (the LAN message boxes show their OK button this way,
        w->visible = (value & 0x10000000) != 0;          // Message_Box 0x005C3230; user report: no button,
        w->enabled = (value & 0x08000000) == 0;          // the box could not be closed; Scout1209 r17)
        invalidate(hwnd);
        break;
    case -20: w->exstyle = value; break;
    case -21: w->userdata = value; break;   // a pointer store goes through ra2port_set_window_ptr
    case 8: w->dialog_user = value; break;
    case -4: unimplemented("SetWindowLongA(GWL_WNDPROC)"); break;
    }
    return old;
}
int GetWindowTextA(HWND hwnd, char *text, int size)
{
    Window *w = win(hwnd);
    if (!w || size <= 0) return 0;
    snprintf(text, (size_t)size, "%s", w->title.c_str());
    return (int)strlen(text);
}
int GetClassNameA(HWND hwnd, char *text, int size)
{
    Window *w = win(hwnd);
    if (!w || size <= 0) return 0;
    snprintf(text, (size_t)size, "%s", w->cls.c_str());
    return (int)strlen(text);
}
DWORD GetWindowContextHelpId(HWND hwnd) { (void)hwnd; return 0; }
int GetDlgCtrlID(HWND hwnd) { Window *w = win(hwnd); return w ? w->id : 0; }

// ---- messages ------------------------------------------------------------

LRESULT DefWindowProcA(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    (void)wparam; (void)lparam;
    if (message == 0x0010) DestroyWindow(hwnd);   // WM_CLOSE
    return 0;
}

LRESULT CallWindowProcA(WNDPROC proc, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    return proc ? proc(hwnd, message, wparam, lparam) : 0;
}

LRESULT SendMessageA(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    // RA2_NATIVE_MSG_LOG=1: messages to dialogs and controls (test runs).
    static int log_messages = getenv("RA2_NATIVE_MSG_LOG") ? atoi(getenv("RA2_NATIVE_MSG_LOG")) : 0;
    if (log_messages && (w->is_dialog || w->parent) && (log_messages > 1 || (message != 0x0200 && message != 0x0113 && message != 0x000F)))
        fprintf(stderr, "msg %p %s id %d tmpl %u: 0x%x %lx %lx\n", (void *)hwnd, w->cls.c_str(), (int)w->id, w->template_id,
                message, (unsigned long)wparam, (unsigned long)lparam);
    if (w->proc) return w->proc(hwnd, message, wparam, lparam);
    if (w->dlgproc) return w->dlgproc(hwnd, message, wparam, lparam);
    return 0;
}

BOOL PostMessageA(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    return post_message(hwnd, message, wparam, lparam);
}

void PostQuitMessage(int code) { post_message(nullptr, 0x0012, (WPARAM)code, 0); }   // WM_QUIT

static bool paint_message(MSG *msg, UINT first, UINT last, bool remove);
static bool peek_queue(MSG *msg, UINT first, UINT last, UINT remove);
static void exposure_check(const MSG *found, std::chrono::steady_clock::time_point now);
static void idle_spin_reset();
static void idle_backoff(std::chrono::steady_clock::time_point now, std::chrono::steady_clock::time_point next_pump);
BOOL PeekMessageA(MSG *msg, HWND hwnd, UINT first, UINT last, UINT remove)
{
    (void)hwnd;
    // RA2_NATIVE_PERF_LOG=1 (diagnostics, Scout1209): a main-thread stretch without
    // PeekMessage of 100 ms or more (the menus' slide loops, loading) on stderr.
    static int perf_log = -1;
    if (perf_log < 0) perf_log = getenv("RA2_NATIVE_PERF_LOG") ? 1 : 0;
    if (perf_log) {
        static auto last_peek = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        double gap = std::chrono::duration<double, std::milli>(now - last_peek).count();
        if (gap >= 100) fprintf(stderr, "perf: t%u no PeekMessage for %.0f ms\n", (unsigned)now_ms(), gap);
        last_peek = now;
    }
    // The host's event pump runs at most once a millisecond (Scout1209). The menus'
    // dialog loop (0x005B5EE0 through 0x00522ED0) peeks back to back, about 470,000
    // times a second, and each Cocoa pump cost about 2 us: most of the main
    // thread's time went to pumping an empty event queue. Messages already posted
    // are still returned at once; new host input waits at most 1 ms.
    using clock = std::chrono::steady_clock;
    static clock::time_point next_pump;
    clock::time_point now = clock::now();
    if (now >= next_pump) {
        if (host() && host()->pump_events) host()->pump_events();
        next_pump = now + std::chrono::milliseconds(1);
    }
    fire_timers();
    bool found = peek_queue(msg, first, last, remove);
    exposure_check(found ? msg : nullptr, now);
    if (found) {
        idle_spin_reset();
        return 1;
    }
    idle_backoff(now, next_pump);
    // The host pause (Scout1247) holds the game here, where it found nothing to do.
    hold_if_paused();
    return 0;
}

static bool peek_queue(MSG *msg, UINT first, UINT last, UINT remove)
{
    std::lock_guard<std::mutex> g(queue_lock);
    // Posted input and messages first; then WM_PAINT; WM_TIMER last (Windows'
    // order: paint and timer messages are generated when nothing else waits).
    for (int pass = 0; pass < 2; ++pass) {
        if (pass == 1 && paint_message(msg, first, last, remove & 1)) return true;
        for (auto it = queue.begin(); it != queue.end(); ++it) {
            if (first || last) {
                if (it->message < first || it->message > last) continue;
            }
            if ((it->message == 0x0113) != (pass == 1)) continue;
            *msg = *it;
            if (remove & 1) queue.erase(it);
            return true;
        }
    }
    return false;
}

// A loop that only peeks an empty queue (the menus' dialog loop between events)
// kept a core busy. After 64 empty peeks in a row, each within 200 us of the
// last, an empty peek waits until the next pump or the next due timer (at most
// 1 ms) before it returns. Messages, their order and the timers' periods are
// unchanged; a game frame's few peeks are never in such a run (Scout1209).
// The game window became active again (WM_ACTIVATEAPP 1). On Windows the
// system then repaints what the other application covered: the dialogs and
// their controls get WM_PAINT once the switch has finished, after the game has
// reacted to the activation. The game redraws its menu background over the
// whole screen when it regains the focus (0x00517370 and Select_Game's loop,
// flag 0x008523DC) and leaves the controls to that repaint. Natively nothing
// repainted them: the main menu's buttons, title and version stayed missing
// after the window lost and regained the focus, also during start-up when the
// menu was not shown yet (user capture 2026-11-09; on the Mac the window often
// loses the focus while the game starts). The port repaints the shown dialogs
// once the main thread has been reading its queue for 250 ms with no work in
// between (the game's redraw and any menu slide are done), as the system's
// repaint arrives after the game's own (Scout1209).
// The game also redraws when it loses the focus, and while inactive the menus sleep 500 ms
// between their message loops (MSEngine::Check_Focus 0x005B3670): the dialogs are then
// repainted too, and those gaps do not count as work, or the controls stayed missing while
// the window was in the background (user report, Scout1223).
// From the activation until that repaint the host holds its presents (the window keeps the
// last picture): the game's redraw first clears the screen, and the menu flashed black, then
// without its buttons, at every focus change (user report, Scout1223). See ra2port_presents_held for how long.
static bool exposure_pending = false;
static bool exposure_restart = false;
static bool app_active = true;
static std::chrono::steady_clock::time_point hold_since;
static bool hold_presents = false;
static bool dialogs_dirty()
{
    for (HWND d : dialog_windows) {
        Window *w = win(d);
        if (!w || !w->visible) continue;
        if (w->dirty) return true;
        for (HWND h : w->children) if (Window *c = win(h)) if (c->dirty && c->visible) return true;
    }
    return false;
}
// While the game is in the background with a menu shown, the window keeps the last picture it
// showed in front, as an inactive Windows game keeps its last frame: the paused menu redraws
// its background without the buttons and the video (they are paused animations), and that was
// what the window showed whenever another app was in front (user report, Scout1223). In a game
// (no dialog shown) presents go on.
static bool menu_seen_active = false;
static bool dialog_shown()
{
    for (HWND d : dialog_windows) if (Window *w = win(d)) if (w->visible) return true;
    return false;
}
// A menu dialog is shown (the host's touch gestures: no sidebar then; Scout1248).
extern "C" int ra2port_menu_shown(void) { return dialog_shown() ? 1 : 0; }
extern "C" bool ra2port_presents_held(void)
{
    bool menu = dialog_shown();
    if (!app_active && menu && menu_seen_active) return true;
    if (!hold_presents) {
        if (app_active && menu) menu_seen_active = true;
        return false;
    }
    // With a menu shown the hold lasts at least 800 ms: after a reactivation the menu also
    // redraws its buttons in again over several frames, after the dialogs' repaint.
    auto held = std::chrono::steady_clock::now() - hold_since;
    if (held > std::chrono::milliseconds(1500)
        || (!exposure_pending && !dialogs_dirty() && (!menu || held > std::chrono::milliseconds(800))))
        hold_presents = false;
    return hold_presents;
}
static void exposure_check(const MSG *found, std::chrono::steady_clock::time_point now)
{
    using namespace std::chrono;
    static steady_clock::time_point idle_since, last_peek;
    static bool animating = false;   // the previous peek returned a WM_PAINT or WM_TIMER
    // Input or a posted message, or work between two peeks, restarts the wait;
    // the generated WM_PAINT and WM_TIMER of the menus' animations and their
    // short handling do not (a menu slide, which a WM_PAINT can start, does).
    bool activity = found && found->message != 0x000F && found->message != 0x0113;
    // A focus change starts the wait afresh: the idle time counted while inactive let the
    // repaint run before the game's redraw after reactivation, which then stayed on screen.
    if (exposure_restart) { exposure_restart = false; activity = true; }
    if (activity || (app_active && now - last_peek > milliseconds(animating ? 50 : 3))) idle_since = now;
    animating = found && !activity;
    last_peek = steady_clock::now();
    if (!exposure_pending) return;
    bool shown = false;
    for (HWND d : dialog_windows) if (Window *w = win(d)) shown = shown || w->visible;
    if (!shown) { idle_since = now; return; }   // nothing to repaint yet: wait for the next dialog
    if (now - idle_since < milliseconds(250)) return;
    exposure_pending = false;
    for (HWND d : dialog_windows) invalidate(d);
}

static int idle_spins = 0;
static std::chrono::steady_clock::time_point idle_last_empty;
static void idle_spin_reset() { idle_spins = 0; }
static void idle_backoff(std::chrono::steady_clock::time_point now, std::chrono::steady_clock::time_point next_pump)
{
    using namespace std::chrono;
    idle_spins = now - idle_last_empty < microseconds(200) ? idle_spins + 1 : 0;
    idle_last_empty = now;
    if (idle_spins < 64) return;
    steady_clock::time_point until = next_pump;
    for (auto &t : timers) if (t.next < until) until = t.next;
    if (until > now + milliseconds(1)) until = now + milliseconds(1);
    if (until > now) std::this_thread::sleep_until(until);
    idle_last_empty = steady_clock::now();
}
static bool paint_message(MSG *msg, UINT first, UINT last, bool remove)
{
    // No queued message: a dialog or control that needs painting gets WM_PAINT
    // (Windows generates it the same way, after the queue).
    if (!first || (first <= 0x000F && last >= 0x000F)) {
        for (HWND d : dialog_windows) {
            Window *w = win(d);
            if (!w || !w->visible) continue;
            std::vector<HWND> all = w->children;
            all.insert(all.begin(), d);
            for (HWND h : all) {
                Window *c = win(h);
                if (c && c->dirty && c->visible) {
                    // A peek without PM_REMOVE leaves it pending (Windows keeps
                    // WM_PAINT until the window validates): the menus' message
                    // loop peeks first, which lost the dialogs' own WM_PAINT and
                    // with it the owner-draw paint and slide-in (Scout1121).
                    if (remove) c->dirty = false;
                    *msg = {h, 0x000F, 0, 0, now_ms(), cursor};
                    return true;
                }
            }
        }
    }
    return false;
}

BOOL TranslateMessage(const MSG *msg) { (void)msg; return 0; }   // WM_CHAR comes from host text input
int TranslateAcceleratorA(HWND hwnd, void *table, MSG *msg) { (void)hwnd; (void)table; (void)msg; return 0; }

LRESULT DispatchMessageA(const MSG *msg)
{
    if (msg->message == 0x0113 && msg->lParam) {   // WM_TIMER with a TIMERPROC
        typedef void (*TIMERPROC)(HWND, UINT, uintptr_t, DWORD);
        ((TIMERPROC)msg->lParam)(msg->hwnd, msg->message, msg->wParam, msg->time);
        return 0;
    }
    HWND target = msg->hwnd ? msg->hwnd : main_hwnd;
    if (msg->message == 0x001C && target == main_hwnd) {
        exposure_pending = true;
        exposure_restart = true;
        app_active = msg->wParam != 0;
        hold_presents = true;
        hold_since = std::chrono::steady_clock::now();
    }
    // Mouse input goes to the window under the cursor (or the capture), as on
    // Windows: the host posts it to the main window in screen pixels; over a
    // shown dialog's control it is that control's, in its client coordinates
    // (Scout1119, M14: the menus' owner-draw buttons, lists and combos).
    if (target == main_hwnd && msg->message >= 0x0200 && msg->message <= 0x0209 && !dialog_windows.empty()) {
        POINT p = {(LONG)(int16_t)(msg->lParam & 0xFFFF), (LONG)(int16_t)((msg->lParam >> 16) & 0xFFFF)};
        HWND at = capture_hwnd ? capture_hwnd : ra2port_window_at(p, true);
        if (msg->message == 0x0201 && getenv("RA2_NATIVE_MSG_LOG")) {
            fprintf(stderr, "click %d,%d -> %p id %d\n", (int)p.x, (int)p.y, (void *)at, win(at) ? (int)win(at)->id : -1);
            for (HWND d : dialog_windows) {
                Window *dw = win(d);
                if (!dw || !dw->visible) continue;
                POINT o = screen_origin(d);
                for (HWND c : dw->children) {
                    Window *cw = win(c);
                    if (cw && cw->visible) fprintf(stderr, "  %d %s at %d,%d %dx%d %s\n", (int)cw->id, cw->cls.c_str(), (int)(o.x + cw->rect.left),
                                                   (int)(o.y + cw->rect.top), (int)(cw->rect.right - cw->rect.left), (int)(cw->rect.bottom - cw->rect.top), cw->title.c_str());
                }
            }
        }
        if (at && at != main_hwnd) {
            // The main window still sees the motion (the game's mouse); the
            // control gets it in client coordinates.
            if (msg->message == 0x0200) SendMessageA(main_hwnd, msg->message, msg->wParam, msg->lParam);
            POINT o = screen_origin(at);
            LPARAM lp = (LPARAM)((((int)p.y - (int)o.y) & 0xFFFF) << 16 | (((int)p.x - (int)o.x) & 0xFFFF));
            return SendMessageA(at, msg->message, msg->wParam, lp);
        }
    }
    // Keys go to the focus window, as on Windows: the host posts them to the
    // main window; while a shown dialog or one of its controls has the focus
    // (SetFocus: a combo box's drop, the dialog itself on WM_INITDIALOG) it
    // gets them instead (Scout1121, M14b: keyboard selection in the menus).
    if (target == main_hwnd && msg->message >= 0x0100 && msg->message <= 0x0109 && focus_hwnd && focus_hwnd != main_hwnd) {
        HWND top = focus_hwnd;
        bool shown = true;
        // Up to the control's dialog: a dialog owned by the game window has the game window
        // as its parent, and the walk stopped there (no key reached the lobby's name field,
        // a NewEdit list box; user report, Scout1209 r17).
        while (top && win(top) && !win(top)->is_dialog && win(top)->parent) { shown = shown && win(top)->visible; top = win(top)->parent; }
        Window *tw = win(top);
        if (tw && tw->is_dialog && tw->visible && shown) return SendMessageA(focus_hwnd, msg->message, msg->wParam, msg->lParam);
    }
    return SendMessageA(target, msg->message, msg->wParam, msg->lParam);
}

BOOL IsDialogMessageA(HWND dialog, MSG *msg) { (void)dialog; (void)msg; return 0; }

uintptr_t SetTimer(HWND hwnd, uintptr_t id, UINT period, void *proc)
{
    // Windows raises a period below USER_TIMER_MINIMUM (10 ms) to it. The owner-draw
    // animations ask for 1 ms (0x005E1490's frame delay); unclamped, a timer was due
    // at every PeekMessage and the dispatch loop 0x005B5EE0 (which runs until the
    // queue is empty) never returned from the movie list dialog (Scout1177, M22).
    if (period < 10) period = 10;
    for (auto &t : timers) {
        if (t.window == hwnd && t.id == id) { t.period = period; t.proc = proc; return id; }
    }
    if (!hwnd && !id) id = 0x1000 + timers.size();
    timers.push_back({hwnd, id, period, std::chrono::steady_clock::now() + std::chrono::milliseconds(period), proc});
    return id;
}

BOOL KillTimer(HWND hwnd, uintptr_t id)
{
    for (size_t i = 0; i < timers.size(); ++i) {
        if (timers[i].window == hwnd && timers[i].id == id) { timers.erase(timers.begin() + (long)i); return 1; }
    }
    return 0;
}

BOOL RegisterHotKey(HWND hwnd, int id, UINT mods, UINT vk) { (void)hwnd; (void)id; (void)mods; (void)vk; return 1; }
DWORD WaitForInputIdle(HANDLE process, DWORD ms) { (void)process; (void)ms; return 0; }

// ---- painting --------------------------------------------------------------

struct PAINTSTRUCT_PORT { HDC hdc; BOOL fErase; RECT rcPaint; BOOL fRestore, fIncUpdate; BYTE rgbReserved[32]; };
HDC BeginPaint(HWND hwnd, PAINTSTRUCT_PORT *ps)
{
    memset(ps, 0, sizeof *ps);
    if (Window *w = win(hwnd)) w->dirty = false;
    GetClientRect(hwnd, &ps->rcPaint);
    ps->hdc = (HDC)(uintptr_t)0x7000;
    return ps->hdc;
}
BOOL EndPaint(HWND hwnd, const PAINTSTRUCT_PORT *ps) { (void)hwnd; (void)ps; return 1; }
HDC GetDC(HWND hwnd) { (void)hwnd; return (HDC)(uintptr_t)0x7000; }
int ReleaseDC(HWND hwnd, HDC dc) { (void)hwnd; (void)dc; return 1; }
BOOL InvalidateRect(HWND hwnd, const RECT *r, BOOL erase) { (void)r; (void)erase; invalidate(hwnd); return 1; }
BOOL ValidateRect(HWND hwnd, const RECT *r) { (void)r; Window *w = win(hwnd); if (w) w->dirty = false; return 1; }
BOOL RedrawWindow(HWND hwnd, const RECT *r, void *rgn, UINT flags)
{
    (void)r; (void)rgn;
    if (flags & 0x0001) invalidate(hwnd);                 // RDW_INVALIDATE
    if (flags & 0x0100) UpdateWindow(hwnd);               // RDW_UPDATENOW
    return 1;
}
BOOL GetUpdateRect(HWND hwnd, RECT *r, BOOL erase) { (void)hwnd; (void)erase; if (r) *r = {0, 0, 0, 0}; return 0; }
DWORD GetSysColor(int index) { (void)index; return 0; }
int GetSystemMetrics(int index)
{
    switch (index) {
    case 0: return screen_width;    // SM_CXSCREEN
    case 1: return screen_height;   // SM_CYSCREEN
    case 19: return 1;     // SM_MOUSEPRESENT
    case 43: return 3;     // SM_CMOUSEBUTTONS
    }
    return 0;
}

// ---- input ---------------------------------------------------------------

static BYTE key_state[256];

}  // extern "C"

namespace ra2port {
void set_key_state(int vk, bool down) { if (vk >= 0 && vk < 256) key_state[vk] = down ? 0x80 : 0; }
}

extern "C" {

short GetAsyncKeyState(int vk) { return (vk >= 0 && vk < 256 && key_state[vk]) ? (short)0x8000 : 0; }
short GetKeyState(int vk) { return (vk >= 0 && vk < 256 && key_state[vk]) ? (short)0x8000 : 0; }
BOOL GetCursorPos(POINT *p) { if (p) *p = cursor; return 1; }
BOOL SetCursorPos(int x, int y) { cursor = {x, y}; return 1; }
int ShowCursor(BOOL show)
{
    cursor_count += show ? 1 : -1;
    if (host() && host()->set_relative_cursor) host()->set_relative_cursor(cursor_count >= 0);
    return cursor_count;
}
void *SetCursor(void *cursor_handle) { (void)cursor_handle; return nullptr; }
void *LoadCursorA(HINSTANCE instance, const char *name) { (void)instance; (void)name; return (void *)(uintptr_t)0x7100; }
void *LoadIconA(HINSTANCE instance, const char *name) { (void)instance; (void)name; return (void *)(uintptr_t)0x7200; }
UINT MapVirtualKeyA(UINT code, UINT type)
{
    (void)type;
    return code;
}
int GetKeyNameTextA(LONG lparam, char *text, int size)
{
    if (size <= 0) return 0;
    int scan = (lparam >> 16) & 0xFF;
    snprintf(text, (size_t)size, (scan >= 0x20 && scan < 0x7F) ? "%c" : "Key %d", scan);
    return (int)strlen(text);
}
// The character of a virtual key on the US layout, as Windows' ToAscii answers it
// from the caller's key state (WWKeyboardClass::To_ASCII sets shift, control and
// alt there from the queued key's bits): letters follow shift and caps lock, or
// give their control character under Ctrl; the control keys give their own
// codes (Return '\r', Backspace 8, Tab 9, Escape 0x1B), which the in-game chat
// line's opening test in Message_Input 0x00540C20 reads (Return, Backspace and
// backslash; Scout1236: the port answered 0 for them, so the chat never opened).
int ToAscii(UINT vk, UINT scan, const BYTE *state, WORD *out, UINT flags)
{
    (void)scan; (void)flags;
    bool shift = state && (state[0x10] & 0x80);
    bool control = state && (state[0x11] & 0x80);
    bool caps = state && (state[0x14] & 0x01);
    int ch = 0;
    if (vk >= 'A' && vk <= 'Z') {
        if (control) ch = (int)vk & 0x1F;
        else ch = (shift != caps) ? (int)vk : (int)vk + 32;
    } else if (vk >= '0' && vk <= '9') {
        if (control) return 0;
        ch = shift ? ")!@#$%^&*("[vk - '0'] : (int)vk;
    } else if (vk >= 0x60 && vk <= 0x69) {           // VK_NUMPAD0..9
        if (control) return 0;
        ch = '0' + (int)(vk - 0x60);
    } else {
        struct Pair { UINT vk; char plain, shifted, controlled; };
        static Pair const table[] = {
            {0x08, 8, 8, 0x7F},  {0x09, 9, 9, 9},     {0x0D, '\r', '\r', '\n'}, {0x1B, 0x1B, 0x1B, 0x1B},
            {0x20, ' ', ' ', ' '}, {0x6A, '*', '*', 0}, {0x6B, '+', '+', 0},   {0x6D, '-', '-', 0},
            {0x6E, '.', '.', 0},  {0x6F, '/', '/', 0},
            {0xBA, ';', ':', 0},  {0xBB, '=', '+', 0}, {0xBC, ',', '<', 0},   {0xBD, '-', '_', 0x1F},
            {0xBE, '.', '>', 0},  {0xBF, '/', '?', 0}, {0xC0, '`', '~', 0},   {0xDB, '[', '{', 0x1B},
            {0xDC, '\\', '|', 0x1C}, {0xDD, ']', '}', 0x1D}, {0xDE, '\'', '"', 0},
        };
        for (Pair const &p : table) {
            if (p.vk != vk) continue;
            ch = control ? p.controlled : shift ? p.shifted : p.plain;
            break;
        }
    }
    if (!ch) return 0;
    *out = (WORD)(unsigned char)ch;
    return 1;
}
BOOL CharToOemBuffA(const char *src, char *dst, DWORD n) { if (src != dst) memmove(dst, src, n); return 1; }

// ---- rectangles and strings ----------------------------------------------

BOOL SetRect(RECT *r, int l, int t, int rr, int b) { if (!r) return 0; *r = {l, t, rr, b}; return 1; }
BOOL IntersectRect(RECT *out, const RECT *a, const RECT *b)
{
    RECT r = {a->left > b->left ? a->left : b->left, a->top > b->top ? a->top : b->top,
              a->right < b->right ? a->right : b->right, a->bottom < b->bottom ? a->bottom : b->bottom};
    if (r.left >= r.right || r.top >= r.bottom) { *out = {0, 0, 0, 0}; return 0; }
    *out = r;
    return 1;
}

int wsprintfA(char *out, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int n = vsnprintf(out, 1024, format, ap);   // wsprintf's documented 1024-byte limit
    va_end(ap);
    return n;
}

int MessageBoxA(HWND hwnd, const char *text, const char *caption, UINT type);

// One native instance: no other game window exists to find.
HWND FindWindowA(const char *cls, const char *title) { (void)cls; (void)title; return nullptr; }

struct MSGBOXPARAMSA_PORT {
    UINT cbSize;
    HWND hwndOwner;
    HINSTANCE hInstance;
    const char *lpszText;
    const char *lpszCaption;
    DWORD dwStyle;
};
int MessageBoxIndirectA(const MSGBOXPARAMSA_PORT *params)
{
    if (!params) return 0;
    return MessageBoxA(params->hwndOwner, params->lpszText, params->lpszCaption, params->dwStyle);
}

int MessageBoxA(HWND hwnd, const char *text, const char *caption, UINT type)
{
    (void)hwnd; (void)type;
    fprintf(stderr, "MessageBox [%s]: %s\n", caption ? caption : "", text ? text : "");
    progress_count("message boxes", text ? text : "");
    return 1;   // IDOK
}

BOOL WinHelpA(HWND hwnd, const char *file, UINT cmd, uintptr_t data) { (void)hwnd; (void)file; (void)cmd; (void)data; return 0; }

// ---- dialogs (templates are resources of the user's game executable) -------
// The templates (RT_DIALOG) are read from the supported game.exe at run time
// (ra2port::original_resource; AGENTS.md "Native phase", user decision
// 2026-11-08), never copied. Both forms are parsed: DLGTEMPLATE and
// DLGTEMPLATEEX. Each control becomes a child window (its class: the registered
// window class of that name, or a predefined control kept as data: Button,
// Edit, Static, ListBox, ScrollBar, ComboBox); dialog units scale by the
// MS Sans Serif 8 base units (6 x 13). The dialog procedure receives
// WM_INITDIALOG; a modal box pumps messages until EndDialog.

} // extern "C"
namespace ra2port { const void *original_resource(uintptr_t type, uintptr_t name, size_t *size); }
namespace {


// ---- predefined controls (Scout1119, M14) ------------------------------------
// The window procedures Windows supplies for its control classes, as the
// game's owner-draw layer (0x005ED180) finds them under its subclass and
// calls them through CallWindowProcA for every message it does not paint
// itself: text, check state, items and selection, and the click that becomes
// WM_COMMAND/BN_CLICKED for the dialog. Behaviour of the Win32 controls only
// (platform layer); the drawing is the game's own.
LRESULT notify_parent(HWND hwnd, int code)
{
    Window *w = win(hwnd);
    if (!w || !w->parent) return 0;
    return SendMessageA(w->parent, 0x0111, (WPARAM)((w->id & 0xFFFF) | (code << 16)), (LPARAM)hwnd);   // WM_COMMAND
}

LRESULT text_messages(Window *w, UINT message, WPARAM wparam, LPARAM lparam, bool *handled)
{
    *handled = true;
    switch (message) {
    case 0x000C: w->title = lparam ? (const char *)lparam : ""; return 1;                       // WM_SETTEXT
    case 0x000D: {                                                                            // WM_GETTEXT
        if (!wparam || !lparam) return 0;
        size_t n = w->title.size() < wparam - 1 ? w->title.size() : wparam - 1;
        memcpy((char *)lparam, w->title.data(), n);
        ((char *)lparam)[n] = 0;
        return (LRESULT)n;
    }
    case 0x000E: return (LRESULT)w->title.size();                                              // WM_GETTEXTLENGTH
    case 0x000A: w->enabled = wparam != 0; return 0;                                           // WM_ENABLE
    case 0x000F: w->dirty = false; return 0;                                                   // WM_PAINT
    case 0x0014: return 1;                                                                     // WM_ERASEBKGND
    case 0x0007: case 0x0008: return 0;                                                        // WM_SETFOCUS/KILLFOCUS
    }
    *handled = false;
    return 0;
}

bool inside(Window *w, LPARAM lparam)
{
    int x = (int16_t)(lparam & 0xFFFF), y = (int16_t)((lparam >> 16) & 0xFFFF);
    return x >= 0 && y >= 0 && x < w->rect.right - w->rect.left && y < w->rect.bottom - w->rect.top;
}

LRESULT button_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool handled;
    LRESULT r = text_messages(w, message, wparam, lparam, &handled);
    if (handled) return r;
    int type = w->style & 0xF;
    switch (message) {
    case 0x0201: case 0x0203:                                  // WM_LBUTTONDOWN / DBLCLK
        if (!w->enabled) return 0;
        capture_hwnd = hwnd;
        set_focus(hwnd);
        w->pushed = true;
        invalidate(hwnd);
        return 0;
    case 0x0200:                                               // WM_MOUSEMOVE
        if (capture_hwnd == hwnd) { bool in = inside(w, lparam); if (in != w->pushed) { w->pushed = in; invalidate(hwnd); } }
        return 0;
    case 0x0202: {                                             // WM_LBUTTONUP
        if (capture_hwnd != hwnd) return 0;
        capture_hwnd = nullptr;
        bool in = inside(w, lparam);
        w->pushed = false;
        invalidate(hwnd);
        if (in) {
            if (type == 3 || type == 6) w->checked = !w->checked;      // BS_AUTOCHECKBOX / BS_AUTO3STATE
            if (type == 9) w->checked = true;                          // BS_AUTORADIOBUTTON
            notify_parent(hwnd, 0);                                    // BN_CLICKED
        }
        return 0;
    }
    case 0x00F0: return w->checked ? 1 : 0;                    // BM_GETCHECK
    case 0x00F1: w->checked = wparam != 0; invalidate(hwnd); return 0;   // BM_SETCHECK
    case 0x00F2: return (w->checked ? 1 : 0) | (w->pushed ? 4 : 0);      // BM_GETSTATE
    case 0x00F3: w->pushed = wparam != 0; invalidate(hwnd); return 0;    // BM_SETSTATE
    case 0x00F4: w->style = (LONG)((w->style & ~0xFFFF) | (wparam & 0xFFFF)); return 0;   // BM_SETSTYLE
    case 0x00F5:                                               // BM_CLICK
        if (type == 3 || type == 6) w->checked = !w->checked;
        if (type == 9) w->checked = true;
        notify_parent(hwnd, 0);
        return 0;
    }
    return 0;
}

LRESULT static_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool handled;
    LRESULT r = text_messages(w, message, wparam, lparam, &handled);
    if (handled) return r;
    if (message == 0x0202 && (w->style & 0x100)) notify_parent(hwnd, 0);   // SS_NOTIFY: STN_CLICKED
    return 0;
}

LRESULT edit_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool handled;
    LRESULT r = text_messages(w, message, wparam, lparam, &handled);
    if (handled) {
        if (message == 0x000C) notify_parent(hwnd, 0x0300);   // EN_CHANGE
        return r;
    }
    switch (message) {
    case 0x0201: set_focus(hwnd); return 0;
    case 0x0102:                                               // WM_CHAR
        if (wparam == 8) { if (!w->title.empty()) w->title.pop_back(); }
        else if (wparam >= 0x20 && (w->pos <= 0 || (int)w->title.size() < w->pos)) w->title += (char)wparam;
        else return 0;
        invalidate(hwnd);
        notify_parent(hwnd, 0x0300);
        return 0;
    case 0x00C5: w->pos = (int)wparam; return 0;               // EM_LIMITTEXT
    case 0x00D5: return w->pos ? w->pos : 30000;               // EM_GETLIMITTEXT
    case 0x00B0: if (wparam) *(DWORD *)wparam = 0; if (lparam) *(DWORD *)lparam = (DWORD)w->title.size(); return 0;   // EM_GETSEL
    case 0x00BA: return 1;                                     // EM_GETLINECOUNT
    }
    return 0;
}

// List boxes and combo boxes keep items: strings (LBS_HASSTRINGS / CBS_HASSTRINGS
// or no owner-draw style) or 32-bit-per-item data (owner-draw without strings).
bool list_has_strings(Window *w, bool combo)
{
    if (combo) return (w->style & 0x200) || !(w->style & 0x30);   // CBS_HASSTRINGS, CBS_OWNERDRAW*
    return (w->style & 0x40) || !(w->style & 0x30);               // LBS_HASSTRINGS, LBS_OWNERDRAW*
}
LRESULT list_insert(Window *w, bool combo, int at, LPARAM lparam)
{
    Window::Item item;
    if (list_has_strings(w, combo)) item.text = lparam ? (const char *)lparam : "";
    else item.data = (intptr_t)lparam;
    bool sort = combo ? (w->style & 0x100) != 0 : (w->style & 0x2) != 0;   // CBS_SORT / LBS_SORT
    if (at < 0 || at > (int)w->items.size()) at = (int)w->items.size();
    if (sort && at == (int)w->items.size() && list_has_strings(w, combo)) {
        at = 0;
        while (at < (int)w->items.size() && strcasecmp(w->items[at].text.c_str(), item.text.c_str()) <= 0) ++at;
    }
    w->items.insert(w->items.begin() + at, item);
    if (w->cursel >= at) ++w->cursel;
    return at;
}
LRESULT list_find(Window *w, int start, LPARAM lparam, bool exact)
{
    const char *text = (const char *)lparam;
    int n = (int)w->items.size();
    for (int k = 1; k <= n; ++k) {
        int i = (start + k + n) % n;
        if (start < 0) i = k - 1;
        if (!text) continue;
        const std::string &t = w->items[i].text;
        if (exact ? !strcasecmp(t.c_str(), text) : !strncasecmp(t.c_str(), text, strlen(text))) return i;
    }
    return -1;
}
LRESULT list_get_text(Window *w, bool combo, int i, LPARAM lparam)
{
    if (i < 0 || i >= (int)w->items.size()) return -1;
    if (list_has_strings(w, combo)) {
        if (lparam) strcpy((char *)lparam, w->items[i].text.c_str());
        return (LRESULT)w->items[i].text.size();
    }
    if (lparam) memcpy((void *)lparam, &w->items[i].data, sizeof(intptr_t));
    return sizeof(intptr_t);
}

LRESULT listbox_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool handled;
    LRESULT r = text_messages(w, message, wparam, lparam, &handled);
    if (handled) return r;
    int count = (int)w->items.size();
    switch (message) {
    case 0x0180: return list_insert(w, false, -1, lparam);                     // LB_ADDSTRING
    case 0x0181: return list_insert(w, false, (int)wparam, lparam);            // LB_INSERTSTRING
    case 0x0182:                                                               // LB_DELETESTRING
        if ((int)wparam < 0 || (int)wparam >= count) return -1;
        w->items.erase(w->items.begin() + (long)wparam);
        if (w->cursel == (int)wparam) w->cursel = -1; else if (w->cursel > (int)wparam) --w->cursel;
        return (LRESULT)w->items.size();
    case 0x0184: w->items.clear(); w->cursel = -1; w->top = 0; invalidate(hwnd); return 0;   // LB_RESETCONTENT
    case 0x0185: if ((int)lparam >= -1 && (int)lparam < count) w->cursel = wparam ? (int)lparam : -1; return 0;  // LB_SETSEL
    case 0x0186: w->cursel = ((int)wparam >= 0 && (int)wparam < count) ? (int)wparam : -1; invalidate(hwnd); return w->cursel == (int)wparam ? (LRESULT)wparam : -1;   // LB_SETCURSEL
    case 0x0187: return (int)wparam == w->cursel && w->cursel >= 0 ? 1 : 0;    // LB_GETSEL
    case 0x0188: return w->cursel;                                             // LB_GETCURSEL
    case 0x0189: return list_get_text(w, false, (int)wparam, lparam);          // LB_GETTEXT
    case 0x018A: return ((int)wparam >= 0 && (int)wparam < count) ? (list_has_strings(w, false) ? (LRESULT)w->items[wparam].text.size() : (LRESULT)sizeof(intptr_t)) : -1;   // LB_GETTEXTLEN
    case 0x018B: return count;                                                 // LB_GETCOUNT
    case 0x018C: { LRESULT i = list_find(w, (int)wparam, lparam, false); if (i >= 0) w->cursel = (int)i; return i; }   // LB_SELECTSTRING
    case 0x018E: return w->top;                                                // LB_GETTOPINDEX
    case 0x018F: return list_find(w, (int)wparam, lparam, false);              // LB_FINDSTRING
    case 0x0190: return w->cursel >= 0 ? 1 : 0;                                // LB_GETSELCOUNT
    case 0x0197: w->top = (int)wparam; invalidate(hwnd); return 0;             // LB_SETTOPINDEX
    case 0x0198: {                                                             // LB_GETITEMRECT
        if (!lparam) return -1;
        RECT *rc = (RECT *)lparam;
        int y = ((int)wparam - w->top) * w->item_height;
        *rc = {0, y, w->rect.right - w->rect.left, y + w->item_height};
        return 0;
    }
    case 0x0199: return ((int)wparam >= 0 && (int)wparam < count) ? (LRESULT)w->items[wparam].data : -1;   // LB_GETITEMDATA
    case 0x019A: if ((int)wparam < 0 || (int)wparam >= count) return -1; w->items[wparam].data = (intptr_t)lparam; return 0;   // LB_SETITEMDATA
    case 0x01A0: if (lparam > 0) w->item_height = (int)lparam; return 0;      // LB_SETITEMHEIGHT
    case 0x01A1: return w->item_height;                                        // LB_GETITEMHEIGHT
    case 0x01A2: return list_find(w, (int)wparam, lparam, true);               // LB_FINDSTRINGEXACT
    case 0x01A9: {                                                             // LB_ITEMFROMPOINT
        int i = w->top + (int16_t)((lparam >> 16) & 0xFFFF) / (w->item_height > 0 ? w->item_height : 16);
        return i >= 0 && i < count ? i : (LRESULT)(count | 0x10000);
    }
    case 0x0201: case 0x0203: {                                                // WM_LBUTTONDOWN / DBLCLK
        set_focus(hwnd);
        int i = w->top + (int16_t)((lparam >> 16) & 0xFFFF) / (w->item_height > 0 ? w->item_height : 16);
        if (i < 0 || i >= count) return 0;
        bool changed = i != w->cursel;
        w->cursel = i;
        invalidate(hwnd);
        if (w->style & 0x1) {                                                  // LBS_NOTIFY
            if (changed) notify_parent(hwnd, 1);                               // LBN_SELCHANGE
            if (message == 0x0203) notify_parent(hwnd, 2);                     // LBN_DBLCLK
        }
        return 0;
    }
    case 0x020A: {                                                             // WM_MOUSEWHEEL
        int d = (int16_t)((wparam >> 16) & 0xFFFF);
        w->top += d > 0 ? -1 : 1;
        if (w->top > count - 1) w->top = count - 1;
        if (w->top < 0) w->top = 0;
        invalidate(hwnd);
        return 0;
    }
    }
    return 0;
}

LRESULT combobox_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool handled;
    LRESULT r = text_messages(w, message, wparam, lparam, &handled);
    if (handled) return r;
    int count = (int)w->items.size();
    switch (message) {
    case 0x0143: return list_insert(w, true, -1, lparam);                      // CB_ADDSTRING
    case 0x014A: return list_insert(w, true, (int)wparam, lparam);             // CB_INSERTSTRING
    case 0x0144:                                                               // CB_DELETESTRING
        if ((int)wparam < 0 || (int)wparam >= count) return -1;
        w->items.erase(w->items.begin() + (long)wparam);
        if (w->cursel == (int)wparam) w->cursel = -1; else if (w->cursel > (int)wparam) --w->cursel;
        return (LRESULT)w->items.size();
    case 0x0146: return count;                                                 // CB_GETCOUNT
    case 0x0147: return w->cursel;                                             // CB_GETCURSEL
    case 0x0148: return list_get_text(w, true, (int)wparam, lparam);           // CB_GETLBTEXT
    case 0x0149: return ((int)wparam >= 0 && (int)wparam < count) ? (list_has_strings(w, true) ? (LRESULT)w->items[wparam].text.size() : (LRESULT)sizeof(intptr_t)) : -1;   // CB_GETLBTEXTLEN
    case 0x014B: w->items.clear(); w->cursel = -1; invalidate(hwnd); return 0;   // CB_RESETCONTENT
    case 0x014C: return list_find(w, (int)wparam, lparam, false);              // CB_FINDSTRING
    case 0x014D: { LRESULT i = list_find(w, (int)wparam, lparam, false); if (i >= 0) { w->cursel = (int)i; invalidate(hwnd); } return i; }   // CB_SELECTSTRING
    case 0x014E:                                                               // CB_SETCURSEL
        w->cursel = ((int)wparam >= 0 && (int)wparam < count) ? (int)wparam : -1;
        if (w->cursel >= 0 && list_has_strings(w, true)) w->title = w->items[w->cursel].text;
        invalidate(hwnd);
        return w->cursel == (int)wparam ? (LRESULT)wparam : -1;
    case 0x014F: w->dropped = wparam != 0; invalidate(hwnd); return 1;         // CB_SHOWDROPDOWN
    case 0x0150: return ((int)wparam >= 0 && (int)wparam < count) ? (LRESULT)w->items[wparam].data : -1;   // CB_GETITEMDATA
    case 0x0151: if ((int)wparam < 0 || (int)wparam >= count) return -1; w->items[wparam].data = (intptr_t)lparam; return 0;   // CB_SETITEMDATA
    case 0x0152:                                                               // CB_GETDROPPEDCONTROLRECT
        if (lparam) {
            GetWindowRect(hwnd, (RECT *)lparam);
            if (closed_combo(w) && w->drop_height > 0) ((RECT *)lparam)->bottom = ((RECT *)lparam)->top + w->drop_height;
        }
        return 1;
    case 0x0153:                                                               // CB_SETITEMHEIGHT (-1: the selection field)
        if ((int)lparam <= 0) return 0;
        if ((int)wparam == -1) {
            w->sel_height = (int)lparam;
            if (closed_combo(w)) w->rect.bottom = w->rect.top + w->sel_height + 6;
        } else {
            w->item_height = (int)lparam;
        }
        return 0;
    case 0x0154: return (int)wparam == -1 ? w->sel_height : w->item_height;   // CB_GETITEMHEIGHT
    case 0x0157: return w->dropped ? 1 : 0;                                    // CB_GETDROPPEDSTATE
    case 0x0158: return list_find(w, (int)wparam, lparam, true);               // CB_FINDSTRINGEXACT
    case 0x015B: return 1;                                                     // CB_LIMITTEXT
    case 0x0100: {                                                             // WM_KEYDOWN: arrows, Home/End move the selection
        int at = w->cursel;
        if (wparam == 0x28 || wparam == 0x27) at = at < 0 ? 0 : at + 1;        // VK_DOWN, VK_RIGHT
        else if (wparam == 0x26 || wparam == 0x25) at = at < 0 ? 0 : at - 1;   // VK_UP, VK_LEFT
        else if (wparam == 0x24) at = 0;                                       // VK_HOME
        else if (wparam == 0x23) at = count - 1;                               // VK_END
        else return 0;
        if (at < 0) at = 0;
        if (at >= count) at = count - 1;
        if (at < 0 || at == w->cursel) return 0;
        w->cursel = at;
        if (list_has_strings(w, true)) w->title = w->items[at].text;
        invalidate(hwnd);
        notify_parent(hwnd, 1);                                                // CBN_SELCHANGE
        return 0;
    }
    }
    return 0;
}

// Common controls the game's dialogs use (trackbar, progress bar, tab, hotkey).
LRESULT common_control_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool handled;
    LRESULT r = text_messages(w, message, wparam, lparam, &handled);
    if (handled) return r;
    if (w->cls == "msctls_trackbar32") {
        switch (message) {
        case 0x0400: return w->pos;                                            // TBM_GETPOS
        case 0x0401: return w->range_min;                                      // TBM_GETRANGEMIN
        case 0x0402: return w->range_max;                                      // TBM_GETRANGEMAX
        case 0x0405: w->pos = (int)lparam; if (w->pos < w->range_min) w->pos = w->range_min; if (w->pos > w->range_max) w->pos = w->range_max; invalidate(hwnd); return 0;   // TBM_SETPOS
        case 0x0406: w->range_min = (int16_t)(lparam & 0xFFFF); w->range_max = (int16_t)((lparam >> 16) & 0xFFFF); return 0;   // TBM_SETRANGE
        case 0x0407: w->range_min = (int)lparam; return 0;                     // TBM_SETRANGEMIN
        case 0x0408: w->range_max = (int)lparam; return 0;                     // TBM_SETRANGEMAX
        }
    } else if (w->cls == "msctls_progress32") {
        switch (message) {
        case 0x0401: { LRESULT old = (w->range_max << 16) | (w->range_min & 0xFFFF); w->range_min = (int16_t)(lparam & 0xFFFF); w->range_max = (int16_t)((lparam >> 16) & 0xFFFF); return old; }   // PBM_SETRANGE
        case 0x0402: { LRESULT old = w->pos; w->pos = (int)wparam; invalidate(hwnd); return old; }   // PBM_SETPOS
        case 0x0408: return w->pos;                                            // PBM_GETPOS
        }
    } else if (w->cls == "SysTabControl32") {
        switch (message) {
        case 0x1304: return (LRESULT)w->items.size();                          // TCM_GETITEMCOUNT
        case 0x1307: {                                                         // TCM_INSERTITEMA
            Window::Item item;
            int at = (int)wparam;
            if (at < 0 || at > (int)w->items.size()) at = (int)w->items.size();
            w->items.insert(w->items.begin() + at, item);
            if (w->cursel < 0) w->cursel = 0;
            return at;
        }
        case 0x1308: if ((int)wparam < (int)w->items.size()) w->items.erase(w->items.begin() + (long)wparam); return 1;   // TCM_DELETEITEM
        case 0x1309: w->items.clear(); w->cursel = -1; return 1;               // TCM_DELETEALLITEMS
        case 0x130B: return w->cursel;                                         // TCM_GETCURSEL
        case 0x130C: { LRESULT old = w->cursel; w->cursel = (int)wparam; invalidate(hwnd); return old; }   // TCM_SETCURSEL
        }
    } else if (w->cls == "msctls_hotkey32") {
        if (message == 0x0401) { w->pos = (int)wparam; return 0; }             // HKM_SETHOTKEY
        if (message == 0x0402) return w->pos;                                  // HKM_GETHOTKEY
    }
    return 0;
}

WNDPROC predefined_proc(const std::string &cls)
{
    if (!strcasecmp(cls.c_str(), "Button")) return button_proc;
    if (!strcasecmp(cls.c_str(), "Static")) return static_proc;
    if (!strcasecmp(cls.c_str(), "Edit")) return edit_proc;
    if (!strcasecmp(cls.c_str(), "ListBox")) return listbox_proc;
    if (!strcasecmp(cls.c_str(), "ComboBox")) return combobox_proc;
    if (!strcasecmp(cls.c_str(), "msctls_trackbar32") || !strcasecmp(cls.c_str(), "msctls_progress32") ||
        !strcasecmp(cls.c_str(), "SysTabControl32") || !strcasecmp(cls.c_str(), "msctls_hotkey32")) return common_control_proc;
    return nullptr;
}

// The dialog manager's procedure (DefDlgProc): the dialog procedure first,
// then the default handling of text messages.
LRESULT dialog_manager_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    intptr_t r = w->dlgproc ? w->dlgproc(hwnd, message, wparam, lparam) : 0;
    if (r) return (LRESULT)r;
    bool handled;
    LRESULT t = text_messages(w, message, wparam, lparam, &handled);
    return handled ? t : 0;
}

struct TemplateReader {
    const unsigned char *p;
    uint16_t word() { uint16_t v = (uint16_t)(p[0] | p[1] << 8); p += 2; return v; }
    uint32_t dword() { uint32_t v = (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24; p += 4; return v; }
    void align4(const unsigned char *base) { while ((p - base) & 3) ++p; }
    // sz_Or_Ord: 0 none, 0xFFFF + ordinal, else a NUL-terminated UTF-16 string (kept as Latin-1).
    std::string sz_or_ord(unsigned *ordinal = nullptr)
    {
        uint16_t first = word();
        if (first == 0) return std::string();
        if (first == 0xFFFF) { uint16_t o = word(); if (ordinal) *ordinal = o; return std::string(); }
        std::string out(1, (char)(first < 256 ? first : '?'));
        for (uint16_t c; (c = word()) != 0;) out += (char)(c < 256 ? c : '?');
        return out;
    }
};

const char *predefined_class(unsigned atom)
{
    switch (atom) {
    case 0x80: return "Button";
    case 0x81: return "Edit";
    case 0x82: return "Static";
    case 0x83: return "ListBox";
    case 0x84: return "ScrollBar";
    case 0x85: return "ComboBox";
    }
    return "";
}

int dialog_units_x(int x) { return x * 6 / 4; }
int dialog_units_y(int y) { return y * 13 / 8; }


HWND build_dialog(const void *templ, unsigned id, HWND parent, DLGPROC proc, LPARAM init, const char *api)
{
    if (!templ) return nullptr;
    const unsigned char *base = (const unsigned char *)templ;
    TemplateReader r{base};
    bool ex = base[2] == 0xFF && base[3] == 0xFF && base[0] == 1 && base[1] == 0;
    uint32_t style, exstyle;
    if (ex) { r.word(); r.word(); r.dword(); exstyle = r.dword(); style = r.dword(); }
    else { style = r.dword(); exstyle = r.dword(); }
    int count = r.word();
    int x = (int16_t)r.word(), y = (int16_t)r.word(), cx = (int16_t)r.word(), cy = (int16_t)r.word();
    r.sz_or_ord();                                   // menu
    unsigned cls_atom = 0;
    std::string cls = r.sz_or_ord(&cls_atom);        // window class (none: the dialog class)
    std::string title = r.sz_or_ord();
    int point_size = 0;
    std::string face;
    if (style & 0x40) {                              // DS_SETFONT (DS_SHELLFONT includes it)
        point_size = r.word();
        if (ex) { r.word(); r.p += 2; }
        face = r.sz_or_ord();
    }
    if (getenv("RA2_NATIVE_MSG_LOG")) fprintf(stderr, "template %u: %d,%d %dx%d DLU, font %d \"%s\", style %x\n", id, x, y, cx, cy, point_size, face.c_str(), style);
    Window *dlg = new Window;
    dlg->kind = H_WINDOW;
    dlg->cls = cls.empty() ? "#32770" : cls;
    dlg->title = title;
    dlg->style = (LONG)style;
    dlg->exstyle = (LONG)exstyle;
    dlg->parent = parent;
    dlg->dlgproc = proc;
    dlg->proc = dialog_manager_proc;
    dlg->is_dialog = true;
    dlg->template_id = id;
    dlg->rect = {dialog_units_x(x), dialog_units_y(y), dialog_units_x(x + cx), dialog_units_y(y + cy)};
    HWND hdlg = handle_new(dlg);
    dialog_windows.push_back(hdlg);
    for (int i = 0; i < count; ++i) {
        r.align4(base);
        uint32_t cstyle, cex, cid;
        if (ex) { r.dword(); cex = r.dword(); cstyle = r.dword(); }
        else { cstyle = r.dword(); cex = r.dword(); }
        int ix = (int16_t)r.word(), iy = (int16_t)r.word(), icx = (int16_t)r.word(), icy = (int16_t)r.word();
        cid = ex ? r.dword() : r.word();
        unsigned atom = 0;
        std::string ccls = r.sz_or_ord(&atom);
        if (ccls.empty()) ccls = predefined_class(atom);
        std::string text = r.sz_or_ord();
        uint16_t extra = r.word();
        r.p += extra;
        Window *c = new Window;
        c->kind = H_WINDOW;
        c->cls = ccls;
        c->title = text;
        c->style = (LONG)cstyle;
        c->exstyle = (LONG)cex;
        c->id = (LONG)cid;
        c->parent = hdlg;
        c->visible = (cstyle & 0x10000000u) != 0;
        c->enabled = (cstyle & 0x08000000u) == 0;
        auto it = classes.find(ccls);
        c->proc = it != classes.end() ? it->second : predefined_proc(ccls);
        c->rect = {dialog_units_x(ix), dialog_units_y(iy), dialog_units_x(ix + icx), dialog_units_y(iy + icy)};
        c->rect.bottom = c->rect.top + combo_height(c, c->rect.bottom - c->rect.top);
        HWND hc = handle_new(c);
        dlg->children.push_back(hc);
        if (c->proc) c->proc(hc, 0x0001, 0, 0);     // WM_CREATE
    }
    char detail[160];
    snprintf(detail, sizeof detail, "%s #%u: %d controls, %dx%d%s", api, id, count,
             dlg->rect.right - dlg->rect.left, dlg->rect.bottom - dlg->rect.top, ex ? " (extended template)" : "");
    progress_count("dialogs created", detail);
    if (proc) proc(hdlg, 0x0110, (WPARAM)(dlg->children.empty() ? nullptr : dlg->children[0]), init);   // WM_INITDIALOG
    if (style & 0x10000000u) ShowWindow(hdlg, 5);    // WS_VISIBLE
    return hdlg;
}

const void *dialog_template(const char *name)
{
    return ra2port::original_resource(5, (uintptr_t)name, nullptr);   // RT_DIALOG
}

}  // namespace
extern "C" {

// Logs a dialog request (--run-to): the resource id or name, and the "dialog" milestone.
static void note_dialog(const char *api, const void *name)
{
    char detail[128];
    if ((uintptr_t)name < 0x10000) snprintf(detail, sizeof detail, "%s #%u", api, (unsigned)(uintptr_t)name);
    else if (name) snprintf(detail, sizeof detail, "%s \"%.80s\"", api, (const char *)name);
    else snprintf(detail, sizeof detail, "%s (indirect)", api);
    progress_count("dialogs requested", detail);
    progress("dialog", detail);
}

static unsigned dialog_id(const char *name) { return (uintptr_t)name < 0x10000 ? (unsigned)(uintptr_t)name : 0; }

HWND CreateDialogParamA(HINSTANCE instance, const char *name, HWND parent, DLGPROC proc, LPARAM init)
{
    (void)instance;
    note_dialog("CreateDialogParamA", name);
    const void *templ = dialog_template(name);
    if (!templ) { unimplemented("CreateDialogParamA (no template: game.exe not supported or id missing)"); return nullptr; }
    return build_dialog(templ, dialog_id(name), parent, proc, init, "CreateDialogParamA");
}
HWND CreateDialogIndirectParamA(HINSTANCE instance, const void *templ, HWND parent, DLGPROC proc, LPARAM init)
{
    (void)instance;
    note_dialog("CreateDialogIndirectParamA", nullptr);
    return build_dialog(templ, 0, parent, proc, init, "CreateDialogIndirectParamA");
}
static intptr_t run_modal(HWND hdlg)
{
    Window *d = win(hdlg);
    if (!d) return -1;
    if (!d->visible) ShowWindow(hdlg, 5);
    while (win(hdlg) && !win(hdlg)->ended) {
        MSG msg;
        if (PeekMessageA(&msg, nullptr, 0, 0, 1)) {
            if (msg.message == 0x0012) { PostQuitMessage((int)msg.wParam); break; }   // WM_QUIT
            DispatchMessageA(&msg);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    Window *w = win(hdlg);
    intptr_t result = w ? w->result : -1;
    if (w) DestroyWindow(hdlg);
    return result;
}
intptr_t DialogBoxParamA(HINSTANCE instance, const char *name, HWND parent, DLGPROC proc, LPARAM init)
{
    (void)instance;
    note_dialog("DialogBoxParamA", name);
    const void *templ = dialog_template(name);
    if (!templ) { unimplemented("DialogBoxParamA (no template: game.exe not supported or id missing)"); return -1; }
    return run_modal(build_dialog(templ, dialog_id(name), parent, proc, init, "DialogBoxParamA"));
}
intptr_t DialogBoxIndirectParamA(HINSTANCE instance, const void *templ, HWND parent, DLGPROC proc, LPARAM init)
{
    (void)instance;
    note_dialog("DialogBoxIndirectParamA", nullptr);
    return run_modal(build_dialog(templ, 0, parent, proc, init, "DialogBoxIndirectParamA"));
}
BOOL EndDialog(HWND dialog, intptr_t result)
{
    Window *d = win(dialog);
    if (!d || !d->is_dialog) return 0;
    d->ended = true;
    d->result = result;
    d->visible = false;
    return 1;
}
HWND GetDlgItem(HWND dialog, int id)
{
    Window *d = win(dialog);
    if (!d) return nullptr;
    for (HWND c : d->children) {
        Window *w = win(c);
        if (w && w->id == id) return c;
    }
    return nullptr;
}
LRESULT SendDlgItemMessageA(HWND dialog, int id, UINT message, WPARAM wparam, LPARAM lparam)
{
    return SendMessageA(GetDlgItem(dialog, id), message, wparam, lparam);
}
BOOL SetDlgItemTextA(HWND dialog, int id, const char *text)
{
    Window *c = win(GetDlgItem(dialog, id));
    if (!c) return 0;
    c->title = text ? text : "";
    return 1;
}
BOOL CheckDlgButton(HWND dialog, int id, UINT check)
{
    Window *c = win(GetDlgItem(dialog, id));
    if (!c) return 0;
    c->checked = check != 0;
    return 1;
}
UINT IsDlgButtonChecked(HWND dialog, int id) { Window *c = win(GetDlgItem(dialog, id)); return c && c->checked ? 1 : 0; }
HWND GetNextDlgTabItem(HWND dialog, HWND control, BOOL previous)
{
    Window *d = win(dialog);
    if (!d || d->children.empty()) return nullptr;
    size_t n = d->children.size(), at = n;
    for (size_t i = 0; i < n; ++i) if (d->children[i] == control) at = i;
    for (size_t step = 1; step <= n; ++step) {
        size_t i = at == n ? (previous ? n - step : step - 1) : (previous ? (at + n - step) % n : (at + step) % n);
        Window *w = win(d->children[i]);
        if (w && w->visible && w->enabled && (w->style & 0x00010000)) return d->children[i];   // WS_TABSTOP
    }
    return nullptr;
}
typedef BOOL (*WNDENUMPROC)(HWND, LPARAM);
BOOL EnumChildWindows(HWND parent, WNDENUMPROC proc, LPARAM lparam)
{
    Window *d = win(parent);
    if (!d || !proc) return 0;
    std::vector<HWND> children = d->children;
    for (HWND c : children) if (win(c) && !proc(c, lparam)) break;
    return 1;
}

}  // extern "C"

// The window under a screen point for mouse input: the topmost shown dialog
// (the last created) containing it, and its enabled, shown control there;
// otherwise the main window (Scout1119, M14).
extern "C" HWND ra2port_window_at(POINT p, bool enabled_only)
{
    for (auto it = dialog_windows.rbegin(); it != dialog_windows.rend(); ++it) {
        Window *d = win(*it);
        if (!d || !d->visible) continue;
        POINT o = screen_origin(*it);
        int w = d->rect.right - d->rect.left, h = d->rect.bottom - d->rect.top;
        if (p.x < o.x || p.y < o.y || p.x >= o.x + w || p.y >= o.y + h) continue;
        // Template order is the Z order (the first control on top); static
        // controls without SS_NOTIFY and group boxes answer HTTRANSPARENT, so
        // the click goes to what lies under them, as on Windows.
        for (auto c = d->children.begin(); c != d->children.end(); ++c) {
            Window *cw = win(*c);
            if (!cw || !cw->visible || (enabled_only && !cw->enabled)) continue;
            if (!strcasecmp(cw->cls.c_str(), "Static") && !(cw->style & 0x100)) continue;
            if (!strcasecmp(cw->cls.c_str(), "Button") && (cw->style & 0xF) == 7) continue;
            int x = p.x - o.x, y = p.y - o.y;
            if (x >= cw->rect.left && x < cw->rect.right && y >= cw->rect.top && y < cw->rect.bottom) return *c;
        }
        return *it;
    }
    return main_hwnd;
}

// Scripted input (platform/sdl/host.cpp): the time the k-th dialog (1-based)
// was shown, in the port's millisecond clock, and that clock's now; -1 while
// fewer have been shown (Scout1119, M14).
extern "C" long long ra2port_dialog_shown_ms(int k)
{
    return k >= 1 && k <= (int)dialog_show_times.size() ? (long long)dialog_show_times[k - 1] : -1;
}
extern "C" long long ra2port_now_ms(void) { return (long long)now_ms(); }
