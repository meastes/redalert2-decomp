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
#include <string.h>
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
    LONG style = 0, exstyle = 0, userdata = 0, id = 0;
    bool visible = false, enabled = true;
    std::string cls, title;
    void *host_window = nullptr;
    // Dialogs built from a template: the controls in template order, the
    // DWL_USER slot (index 8; pointer-sized natively), the modal result.
    std::vector<HWND> children;
    bool is_dialog = false, ended = false, checked = false;
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
int cursor_count = 0;
int dialogs_shown = 0;   // dialogs from templates shown so far (--run-to `menu`)
POINT cursor = {0, 0};
// The screen size: the desktop until the game sets a DirectDraw display mode.
int screen_width = 800, screen_height = 600;

struct Timer { HWND window; uintptr_t id; UINT period; std::chrono::steady_clock::time_point next; void *proc; };
std::vector<Timer> timers;

Window *win(HWND h) { return (Window *)handle_get(h, H_WINDOW); }

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

WORD RegisterClassA(const WNDCLASSA_PORT *wc)
{
    if (!wc || !wc->lpszClassName) return 0;
    classes[wc->lpszClassName] = wc->lpfnWndProc;
    return (WORD)(classes.size());
}

HWND CreateWindowExA(DWORD exstyle, const char *cls, const char *title, DWORD style, int x, int y, int w, int h,
                     HWND parent, void *menu, HINSTANCE instance, void *param)
{
    (void)menu; (void)instance; (void)param;
    Window *wnd = new Window;
    wnd->kind = H_WINDOW;
    wnd->cls = cls ? cls : "";
    wnd->title = title ? title : "";
    wnd->style = (LONG)style;
    wnd->exstyle = (LONG)exstyle;
    wnd->parent = parent;
    auto it = classes.find(wnd->cls);
    wnd->proc = it != classes.end() ? it->second : nullptr;
    if (w <= 0 || w > 4096) w = 800;
    if (h <= 0 || h > 4096) h = 600;
    wnd->rect = {x < 0 || x > 4096 ? 0 : x, y < 0 || y > 4096 ? 0 : y, 0, 0};
    wnd->rect.right = wnd->rect.left + w;
    wnd->rect.bottom = wnd->rect.top + h;
    HWND hwnd = handle_new(wnd);
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
        wnd->proc(hwnd, 0x0001, 0, 0);   // WM_CREATE (CREATESTRUCT not provided)
    }
    return hwnd;
}

BOOL DestroyWindow(HWND hwnd)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    if (w->proc) w->proc(hwnd, 0x0002, 0, 0);   // WM_DESTROY
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
    // --run-to `menu`: the first dialog built from a game.exe template that is
    // shown (Select_Game's main menu on the start-up path; the detail names
    // the template so a run log shows which one).
    if (!was && w->visible && w->is_dialog) {
        char detail[96];
        snprintf(detail, sizeof detail, "dialog #%u shown (%zu controls)", w->template_id, w->children.size());
        progress_count("dialogs shown", detail);
        if (dialogs_shown++ == 0) progress("menu", detail);
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
BOOL UpdateWindow(HWND hwnd) { return win(hwnd) != nullptr; }
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
    wnd->rect = {x, y, x + w, y + h};
    return 1;
}
BOOL SetWindowPos(HWND hwnd, HWND after, int x, int y, int w, int h, UINT flags)
{
    (void)after;
    Window *wnd = win(hwnd);
    if (!wnd) return 0;
    if (!(flags & 0x0002)) { int cw = wnd->rect.right - wnd->rect.left, ch = wnd->rect.bottom - wnd->rect.top; wnd->rect = {x, y, x + cw, y + ch}; }
    if (!(flags & 0x0001)) { wnd->rect.right = wnd->rect.left + w; wnd->rect.bottom = wnd->rect.top + h; }
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
    *r = w->rect;
    return 1;
}
BOOL AdjustWindowRect(RECT *r, DWORD style, BOOL menu) { (void)r; (void)style; (void)menu; return 1; }
BOOL AdjustWindowRectEx(RECT *r, DWORD style, BOOL menu, DWORD ex) { (void)r; (void)style; (void)menu; (void)ex; return 1; }
BOOL ClientToScreen(HWND hwnd, POINT *p) { Window *w = win(hwnd); if (w && p) { p->x += w->rect.left; p->y += w->rect.top; } return 1; }
BOOL ScreenToClient(HWND hwnd, POINT *p) { Window *w = win(hwnd); if (w && p) { p->x -= w->rect.left; p->y -= w->rect.top; } return 1; }
HWND GetParent(HWND hwnd) { Window *w = win(hwnd); return w ? w->parent : nullptr; }
HWND GetWindow(HWND hwnd, UINT cmd) { (void)hwnd; (void)cmd; return nullptr; }
HWND GetTopWindow(HWND hwnd) { (void)hwnd; return nullptr; }
HWND WindowFromPoint(POINT p) { (void)p; return main_hwnd; }
HWND ChildWindowFromPoint(HWND hwnd, POINT p) { (void)p; return hwnd; }
HWND ChildWindowFromPointEx(HWND hwnd, POINT p, UINT flags) { (void)p; (void)flags; return hwnd; }
HWND GetActiveWindow(void) { return main_hwnd; }
HWND SetActiveWindow(HWND hwnd) { HWND old = main_hwnd; (void)hwnd; return old; }
HWND GetForegroundWindow(void) { return main_hwnd; }
BOOL SetForegroundWindow(HWND hwnd) { return win(hwnd) != nullptr; }
BOOL BringWindowToTop(HWND hwnd) { return win(hwnd) != nullptr; }
HWND GetFocus(void) { return focus_hwnd; }
HWND SetFocus(HWND hwnd) { HWND old = focus_hwnd; focus_hwnd = hwnd; return old; }
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

// LONG is 32 bits as on Win32: a window procedure stored through here would be
// truncated, so GWL_WNDPROC is refused and logged.
LONG GetWindowLongA(HWND hwnd, int index)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    switch (index) {
    case -12: return w->id;
    case -16: return w->style;
    case -20: return w->exstyle;
    case -21: return w->userdata;
    case 8: return (LONG)w->dialog_user;     // DWL_USER (pointer stores use ra2port_set_dialog_user)
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
    case -16: w->style = value; break;
    case -20: w->exstyle = value; break;
    case -21: w->userdata = value; break;
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
    if (w->proc) return w->proc(hwnd, message, wparam, lparam);
    if (w->dlgproc) return w->dlgproc(hwnd, message, wparam, lparam);
    return 0;
}

BOOL PostMessageA(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    return post_message(hwnd, message, wparam, lparam);
}

void PostQuitMessage(int code) { post_message(nullptr, 0x0012, (WPARAM)code, 0); }   // WM_QUIT

BOOL PeekMessageA(MSG *msg, HWND hwnd, UINT first, UINT last, UINT remove)
{
    (void)hwnd;
    if (host() && host()->pump_events) host()->pump_events();
    fire_timers();
    std::lock_guard<std::mutex> g(queue_lock);
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        if (first || last) {
            if (it->message < first || it->message > last) continue;
        }
        *msg = *it;
        if (remove & 1) queue.erase(it);
        return 1;
    }
    return 0;
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
    return SendMessageA(target, msg->message, msg->wParam, msg->lParam);
}

BOOL IsDialogMessageA(HWND dialog, MSG *msg) { (void)dialog; (void)msg; return 0; }

uintptr_t SetTimer(HWND hwnd, uintptr_t id, UINT period, void *proc)
{
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
    GetClientRect(hwnd, &ps->rcPaint);
    ps->hdc = (HDC)(uintptr_t)0x7000;
    return ps->hdc;
}
BOOL EndPaint(HWND hwnd, const PAINTSTRUCT_PORT *ps) { (void)hwnd; (void)ps; return 1; }
HDC GetDC(HWND hwnd) { (void)hwnd; return (HDC)(uintptr_t)0x7000; }
int ReleaseDC(HWND hwnd, HDC dc) { (void)hwnd; (void)dc; return 1; }
BOOL InvalidateRect(HWND hwnd, const RECT *r, BOOL erase) { (void)hwnd; (void)r; (void)erase; return 1; }
BOOL ValidateRect(HWND hwnd, const RECT *r) { (void)hwnd; (void)r; return 1; }
BOOL RedrawWindow(HWND hwnd, const RECT *r, void *rgn, UINT flags) { (void)hwnd; (void)r; (void)rgn; (void)flags; return 1; }
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
int ToAscii(UINT vk, UINT scan, const BYTE *state, WORD *out, UINT flags)
{
    (void)scan; (void)state; (void)flags;
    if ((vk >= '0' && vk <= '9') || (vk >= 'A' && vk <= 'Z') || vk == ' ') { *out = (WORD)(vk >= 'A' && vk <= 'Z' ? vk + 32 : vk); return 1; }
    return 0;
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
    if (style & 0x40) {                              // DS_SETFONT (DS_SHELLFONT includes it)
        r.word();
        if (ex) { r.word(); r.p += 2; }
        r.sz_or_ord();
    }
    Window *dlg = new Window;
    dlg->kind = H_WINDOW;
    dlg->cls = cls.empty() ? "#32770" : cls;
    dlg->title = title;
    dlg->style = (LONG)style;
    dlg->exstyle = (LONG)exstyle;
    dlg->parent = parent;
    dlg->dlgproc = proc;
    dlg->is_dialog = true;
    dlg->template_id = id;
    dlg->rect = {dialog_units_x(x), dialog_units_y(y), dialog_units_x(x + cx), dialog_units_y(y + cy)};
    HWND hdlg = handle_new(dlg);
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
        c->proc = it != classes.end() ? it->second : nullptr;
        c->rect = {dialog_units_x(ix), dialog_units_y(iy), dialog_units_x(ix + icx), dialog_units_y(iy + icy)};
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
