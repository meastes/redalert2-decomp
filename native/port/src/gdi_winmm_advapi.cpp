// GDI32 (offscreen DIB sections and device-context state; text drawing is a
// logged no-op until the font path is ported), WINMM multimedia timers on a
// thread, and ADVAPI32's registry as a per-user store seeded from the data dir.
#include "ra2port/win32_port.hpp"

#include <atomic>
#include <chrono>
#include <map>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <thread>
#include <vector>

using namespace ra2port;

namespace {
struct GdiObject : HandleObject { std::vector<unsigned char> bits; int type = 0; };
}

extern "C" {

// ---- GDI32 -----------------------------------------------------------------

int GetDeviceCaps(HDC dc, int index)
{
    (void)dc;
    switch (index) {
    case 8: return 800;    // HORZRES
    case 10: return 600;   // VERTRES
    case 12: return 16;    // BITSPIXEL
    case 14: return 1;     // PLANES
    case 88: case 90: return 96;   // LOGPIXELSX/Y
    }
    return 0;
}

HGDIOBJ GetStockObject(int index) { return (HGDIOBJ)(uintptr_t)(0x7300 + index * 4); }
HGDIOBJ SelectObject(HDC dc, HGDIOBJ object) { (void)dc; (void)object; return (HGDIOBJ)(uintptr_t)0x7300; }
BOOL DeleteObject(HGDIOBJ object)
{
    if (handle_get(object, H_GDI)) handle_close(object);
    return 1;
}
HGDIOBJ CreateSolidBrush(DWORD color)
{
    GdiObject *o = new GdiObject;
    o->kind = H_GDI;
    o->type = 2;
    (void)color;
    return handle_new(o);
}
HGDIOBJ CreateFontIndirectA(const void *logfont)
{
    (void)logfont;
    GdiObject *o = new GdiObject;
    o->kind = H_GDI;
    o->type = 6;
    return handle_new(o);
}

// BITMAPINFOHEADER at the start of BITMAPINFO.
struct BITMAPINFOHEADER_PORT { DWORD biSize; LONG biWidth, biHeight; WORD biPlanes, biBitCount; DWORD biCompression, biSizeImage; LONG biXPelsPerMeter, biYPelsPerMeter; DWORD biClrUsed, biClrImportant; };
HGDIOBJ CreateDIBSection(HDC dc, const BITMAPINFOHEADER_PORT *info, UINT usage, void **bits, HANDLE section, DWORD offset)
{
    (void)dc; (void)usage; (void)section; (void)offset;
    if (!info) return nullptr;
    LONG w = info->biWidth, h = info->biHeight < 0 ? -info->biHeight : info->biHeight;
    size_t pitch = (((size_t)w * info->biBitCount + 31) / 32) * 4;
    GdiObject *o = new GdiObject;
    o->kind = H_GDI;
    o->type = 7;
    o->bits.assign(pitch * (size_t)h, 0);
    if (bits) *bits = o->bits.data();
    return handle_new(o);
}

DWORD SetBkColor(HDC dc, DWORD c) { (void)dc; (void)c; return 0; }
DWORD GetBkColor(HDC dc) { (void)dc; return 0; }
int SetBkMode(HDC dc, int m) { (void)dc; (void)m; return 1; }
int GetBkMode(HDC dc) { (void)dc; return 1; }
DWORD SetTextColor(HDC dc, DWORD c) { (void)dc; (void)c; return 0; }
DWORD GetTextColor(HDC dc) { (void)dc; return 0; }
int SaveDC(HDC dc) { (void)dc; return 1; }
BOOL RestoreDC(HDC dc, int n) { (void)dc; (void)n; return 1; }
int SetGraphicsMode(HDC dc, int m) { (void)dc; (void)m; return 1; }
BOOL ModifyWorldTransform(HDC dc, const void *x, DWORD mode) { (void)dc; (void)x; (void)mode; return 1; }
BOOL DPtoLP(HDC dc, POINT *p, int n) { (void)dc; (void)p; (void)n; return 1; }
BOOL SetViewportOrgEx(HDC dc, int x, int y, POINT *old) { (void)dc; (void)x; (void)y; if (old) *old = {0, 0}; return 1; }
BOOL SetWindowOrgEx(HDC dc, int x, int y, POINT *old) { (void)dc; (void)x; (void)y; if (old) *old = {0, 0}; return 1; }
BOOL GetTextMetricsA(HDC dc, void *metrics)
{
    (void)dc;
    unimplemented("GetTextMetricsA");
    memset(metrics, 0, 57);   // TEXTMETRICA is 56 bytes + padding
    return 0;
}
BOOL TextOutA(HDC dc, int x, int y, const char *text, int n)
{
    (void)dc; (void)x; (void)y; (void)text; (void)n;
    unimplemented("TextOutA");
    return 0;
}

// ---- WINMM -----------------------------------------------------------------

typedef void (*TIMECALLBACK)(UINT id, UINT msg, uintptr_t user, uintptr_t r1, uintptr_t r2);
namespace {
struct MMTimer { std::thread thread; std::atomic<bool> stop{false}; };
std::mutex timer_lock;
std::map<UINT, MMTimer *> mm_timers;
UINT next_timer = 1;
}

UINT timeBeginPeriod(UINT period) { (void)period; return 0; }   // TIMERR_NOERROR
UINT timeEndPeriod(UINT period) { (void)period; return 0; }

UINT timeSetEvent(UINT delay, UINT resolution, TIMECALLBACK callback, uintptr_t user, UINT flags)
{
    (void)resolution;
    std::lock_guard<std::mutex> g(timer_lock);
    UINT id = next_timer++;
    MMTimer *t = new MMTimer;
    bool periodic = flags & 1;   // TIME_PERIODIC
    t->thread = std::thread([t, id, delay, callback, user, periodic] {
        auto next = std::chrono::steady_clock::now();
        do {
            next += std::chrono::milliseconds(delay ? delay : 1);
            std::this_thread::sleep_until(next);
            if (t->stop) break;
            callback(id, 0, user, 0, 0);
        } while (periodic && !t->stop);
    });
    mm_timers[id] = t;
    return id;
}

UINT timeKillEvent(UINT id)
{
    MMTimer *t = nullptr;
    {
        std::lock_guard<std::mutex> g(timer_lock);
        auto it = mm_timers.find(id);
        if (it == mm_timers.end()) return 97;   // MMSYSERR_INVALPARAM
        t = it->second;
        mm_timers.erase(it);
    }
    t->stop = true;
    if (t->thread.get_id() == std::this_thread::get_id()) t->thread.detach();
    else t->thread.join();
    delete t;
    return 0;
}

// ---- ADVAPI32 registry -----------------------------------------------------
// Keys are paths; values are typed byte strings. The store starts with what
// the installer wrote that the game needs (InstallPath = the data dir) and is
// not persisted yet (a later step writes it under the user's settings).

namespace {
struct RegValue { DWORD type; std::vector<unsigned char> data; };
struct RegKey : HandleObject { std::string path; };
std::mutex reg_lock;
std::map<std::string, std::map<std::string, RegValue>> &registry()
{
    static std::map<std::string, std::map<std::string, RegValue>> r;
    return r;
}
std::string lower(std::string s) { for (auto &c : s) c = (char)tolower((unsigned char)c); return s; }
std::string key_path(HANDLE parent, const char *sub)
{
    std::string base;
    uintptr_t p = (uintptr_t)parent;
    if (p >= 0x80000000u && p <= 0x80000005u) base = "root" + std::to_string(p & 0xF);
    else if (RegKey *k = (RegKey *)handle_get(parent, H_REGKEY)) base = k->path;
    return lower(base + "\\" + (sub ? sub : ""));
}
void seed_registry()
{
    static bool done = false;
    if (done) return;
    done = true;
    std::string dir = data_dir();
    RegValue v{1, std::vector<unsigned char>(dir.begin(), dir.end())};
    v.data.push_back(0);
    registry()[lower("root2\\Software\\Westwood\\Red Alert 2")]["installpath"] = v;
}
}

LONG RegOpenKeyExA(HANDLE parent, const char *sub, DWORD options, DWORD access, HANDLE *out)
{
    (void)options; (void)access;
    std::lock_guard<std::mutex> g(reg_lock);
    seed_registry();
    std::string path = key_path(parent, sub);
    if (!registry().count(path)) return 2;   // ERROR_FILE_NOT_FOUND
    RegKey *k = new RegKey;
    k->kind = H_REGKEY;
    k->path = path;
    *out = handle_new(k);
    return 0;
}
LONG RegCreateKeyExA(HANDLE parent, const char *sub, DWORD reserved, char *cls, DWORD options, DWORD access,
                     void *security, HANDLE *out, DWORD *disposition)
{
    (void)reserved; (void)cls; (void)options; (void)access; (void)security;
    std::lock_guard<std::mutex> g(reg_lock);
    seed_registry();
    std::string path = key_path(parent, sub);
    bool existed = registry().count(path) != 0;
    registry()[path];
    RegKey *k = new RegKey;
    k->kind = H_REGKEY;
    k->path = path;
    *out = handle_new(k);
    if (disposition) *disposition = existed ? 2 : 1;
    return 0;
}
LONG RegCloseKey(HANDLE key) { handle_close(key); return 0; }
LONG RegQueryValueExA(HANDLE key, const char *name, DWORD *reserved, DWORD *type, BYTE *data, DWORD *size)
{
    (void)reserved;
    std::lock_guard<std::mutex> g(reg_lock);
    RegKey *k = (RegKey *)handle_get(key, H_REGKEY);
    if (!k) return 6;
    auto &values = registry()[k->path];
    auto it = values.find(lower(name ? name : ""));
    if (it == values.end()) return 2;
    if (type) *type = it->second.type;
    DWORD n = (DWORD)it->second.data.size();
    if (data) {
        if (!size || *size < n) { if (size) *size = n; return 234; }   // ERROR_MORE_DATA
        memcpy(data, it->second.data.data(), n);
    }
    if (size) *size = n;
    return 0;
}
LONG RegSetValueExA(HANDLE key, const char *name, DWORD reserved, DWORD type, const BYTE *data, DWORD size)
{
    (void)reserved;
    std::lock_guard<std::mutex> g(reg_lock);
    RegKey *k = (RegKey *)handle_get(key, H_REGKEY);
    if (!k) return 6;
    registry()[k->path][lower(name ? name : "")] = RegValue{type, std::vector<unsigned char>(data, data + size)};
    return 0;
}
LONG RegDeleteValueA(HANDLE key, const char *name)
{
    std::lock_guard<std::mutex> g(reg_lock);
    RegKey *k = (RegKey *)handle_get(key, H_REGKEY);
    if (!k) return 6;
    return registry()[k->path].erase(lower(name ? name : "")) ? 0 : 2;
}
LONG RegDeleteKeyA(HANDLE parent, const char *sub)
{
    std::lock_guard<std::mutex> g(reg_lock);
    return registry().erase(key_path(parent, sub)) ? 0 : 2;
}
LONG RegEnumKeyExA(HANDLE key, DWORD index, char *name, DWORD *size, DWORD *reserved, char *cls, DWORD *cls_size, FILETIME *t)
{
    (void)reserved; (void)cls; (void)cls_size; (void)t;
    std::lock_guard<std::mutex> g(reg_lock);
    RegKey *k = (RegKey *)handle_get(key, H_REGKEY);
    if (!k) return 6;
    std::string prefix = k->path + "\\";
    DWORD i = 0;
    for (auto &e : registry()) {
        if (e.first.compare(0, prefix.size(), prefix) != 0) continue;
        std::string rest = e.first.substr(prefix.size());
        if (rest.empty() || rest.find('\\') != std::string::npos) continue;
        if (i++ == index) {
            if (!size || *size <= rest.size()) return 234;
            memcpy(name, rest.c_str(), rest.size() + 1);
            *size = (DWORD)rest.size();
            return 0;
        }
    }
    return 259;   // ERROR_NO_MORE_ITEMS
}
LONG RegQueryInfoKeyA(HANDLE key, char *cls, DWORD *cls_size, DWORD *reserved, DWORD *subkeys, DWORD *max_subkey,
                      DWORD *max_class, DWORD *values, DWORD *max_value_name, DWORD *max_value, DWORD *security, FILETIME *t)
{
    (void)cls; (void)cls_size; (void)reserved; (void)max_class; (void)security; (void)t;
    std::lock_guard<std::mutex> g(reg_lock);
    RegKey *k = (RegKey *)handle_get(key, H_REGKEY);
    if (!k) return 6;
    if (subkeys) *subkeys = 0;
    if (max_subkey) *max_subkey = 0;
    auto &v = registry()[k->path];
    if (values) *values = (DWORD)v.size();
    if (max_value_name) *max_value_name = 64;
    if (max_value) *max_value = 1024;
    return 0;
}

}  // extern "C"
