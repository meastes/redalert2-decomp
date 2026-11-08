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
POINT cursor = {0, 0};

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
BOOL ShowWindow(HWND hwnd, int cmd)
{
    Window *w = win(hwnd);
    if (!w) return 0;
    bool was = w->visible;
    w->visible = cmd != 0;
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
    case 0: return 800;    // SM_CXSCREEN
    case 1: return 600;    // SM_CYSCREEN
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

int MessageBoxA(HWND hwnd, const char *text, const char *caption, UINT type)
{
    (void)hwnd; (void)type;
    fprintf(stderr, "MessageBox [%s]: %s\n", caption ? caption : "", text ? text : "");
    return 1;   // IDOK
}

BOOL WinHelpA(HWND hwnd, const char *file, UINT cmd, uintptr_t data) { (void)hwnd; (void)file; (void)cmd; (void)data; return 0; }

// ---- dialogs (templates are resources of the user's game executable) -------

HWND CreateDialogParamA(HINSTANCE instance, const char *name, HWND parent, DLGPROC proc, LPARAM init)
{
    (void)instance; (void)name; (void)parent; (void)proc; (void)init;
    unimplemented("CreateDialogParamA");
    return nullptr;
}
HWND CreateDialogIndirectParamA(HINSTANCE instance, const void *templ, HWND parent, DLGPROC proc, LPARAM init)
{
    (void)instance; (void)templ; (void)parent; (void)proc; (void)init;
    unimplemented("CreateDialogIndirectParamA");
    return nullptr;
}
intptr_t DialogBoxParamA(HINSTANCE instance, const char *name, HWND parent, DLGPROC proc, LPARAM init)
{
    (void)instance; (void)name; (void)parent; (void)proc; (void)init;
    unimplemented("DialogBoxParamA");
    return -1;
}
intptr_t DialogBoxIndirectParamA(HINSTANCE instance, const void *templ, HWND parent, DLGPROC proc, LPARAM init)
{
    (void)instance; (void)templ; (void)parent; (void)proc; (void)init;
    unimplemented("DialogBoxIndirectParamA");
    return -1;
}
BOOL EndDialog(HWND dialog, intptr_t result) { (void)dialog; (void)result; return 1; }
HWND GetDlgItem(HWND dialog, int id) { (void)dialog; (void)id; return nullptr; }
LRESULT SendDlgItemMessageA(HWND dialog, int id, UINT message, WPARAM wparam, LPARAM lparam)
{
    return SendMessageA(GetDlgItem(dialog, id), message, wparam, lparam);
}
BOOL SetDlgItemTextA(HWND dialog, int id, const char *text) { (void)dialog; (void)id; (void)text; return 0; }
BOOL CheckDlgButton(HWND dialog, int id, UINT check) { (void)dialog; (void)id; (void)check; return 0; }
UINT IsDlgButtonChecked(HWND dialog, int id) { (void)dialog; (void)id; return 0; }
HWND GetNextDlgTabItem(HWND dialog, HWND control, BOOL previous) { (void)dialog; (void)control; (void)previous; return nullptr; }
typedef BOOL (*WNDENUMPROC)(HWND, LPARAM);
BOOL EnumChildWindows(HWND parent, WNDENUMPROC proc, LPARAM lparam) { (void)parent; (void)proc; (void)lparam; return 1; }

}  // extern "C"
