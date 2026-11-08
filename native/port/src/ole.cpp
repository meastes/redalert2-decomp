// OLE32/OLEAUT32: the in-process COM the game uses. Its locomotors and other
// COM classes are created with CoCreateInstance through class factories it
// registers itself (CoRegisterClassObject), so those two form a real CLSID
// table here. Structured storage (save games) is the next step: logged
// E_NOTIMPL until then.
#include "ra2port/win32_port.hpp"

#include <map>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

using namespace ra2port;

namespace {
const HRESULT S_OK = 0, E_NOTIMPL = (HRESULT)0x80004001u, E_POINTER = (HRESULT)0x80004003u,
              REGDB_E_CLASSNOTREG = (HRESULT)0x80040154u, CO_E_CLASSSTRING = (HRESULT)0x800401F3u;

struct GUID { uint32_t d1; uint16_t d2, d3; uint8_t d4[8]; };
bool operator<(const GUID &a, const GUID &b) { return memcmp(&a, &b, sizeof a) < 0; }

struct IUnknownPort {
    virtual HRESULT QueryInterface(const GUID *iid, void **out) = 0;
    virtual ULONG AddRef() = 0;
    virtual ULONG Release() = 0;
};
struct IClassFactoryPort : IUnknownPort {
    virtual HRESULT CreateInstance(IUnknownPort *outer, const GUID *iid, void **out) = 0;
    virtual HRESULT LockServer(BOOL lock) = 0;
};

std::mutex com_lock;
std::map<GUID, IClassFactoryPort *> &factories() { static std::map<GUID, IClassFactoryPort *> f; return f; }
std::map<DWORD, GUID> &cookies() { static std::map<DWORD, GUID> c; return c; }
DWORD next_cookie = 1;

int format_guid(const GUID *g, char *out, size_t n)
{
    return snprintf(out, n, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", g->d1, g->d2, g->d3,
                    g->d4[0], g->d4[1], g->d4[2], g->d4[3], g->d4[4], g->d4[5], g->d4[6], g->d4[7]);
}
}  // namespace

extern "C" {

HRESULT OleInitialize(void *reserved) { (void)reserved; return S_OK; }
void OleUninitialize(void) {}
HRESULT OleRun(IUnknownPort *unknown) { (void)unknown; return S_OK; }
HRESULT CoDisconnectObject(IUnknownPort *unknown, DWORD reserved) { (void)unknown; (void)reserved; return S_OK; }

HRESULT CoRegisterClassObject(const GUID *clsid, IUnknownPort *unknown, DWORD context, DWORD flags, DWORD *cookie)
{
    (void)context; (void)flags;
    void *factory = nullptr;
    static const GUID IID_IClassFactory = {0x00000001, 0, 0, {0xC0, 0, 0, 0, 0, 0, 0, 0x46}};
    if (unknown->QueryInterface(&IID_IClassFactory, &factory) != S_OK || !factory) factory = unknown;
    std::lock_guard<std::mutex> g(com_lock);
    factories()[*clsid] = (IClassFactoryPort *)factory;
    DWORD c = next_cookie++;
    cookies()[c] = *clsid;
    if (cookie) *cookie = c;
    return S_OK;
}

HRESULT CoRevokeClassObject(DWORD cookie)
{
    std::lock_guard<std::mutex> g(com_lock);
    auto it = cookies().find(cookie);
    if (it == cookies().end()) return E_POINTER;
    auto f = factories().find(it->second);
    if (f != factories().end()) { f->second->Release(); factories().erase(f); }
    cookies().erase(it);
    return S_OK;
}

HRESULT CoCreateInstance(const GUID *clsid, IUnknownPort *outer, DWORD context, const GUID *iid, void **out)
{
    (void)context;
    IClassFactoryPort *factory = nullptr;
    {
        std::lock_guard<std::mutex> g(com_lock);
        auto it = factories().find(*clsid);
        if (it != factories().end()) factory = it->second;
    }
    if (!factory) {
        char text[64];
        format_guid(clsid, text, sizeof text);
        trace("CoCreateInstance %s: class not registered", text);
        if (out) *out = nullptr;
        return REGDB_E_CLASSNOTREG;
    }
    return factory->CreateInstance(outer, iid, out);
}

HRESULT CoFileTimeNow(FILETIME *ft)
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    uint64_t v = (uint64_t)t.tv_sec * 10000000ull + (uint64_t)t.tv_nsec / 100 + 116444736000000000ull;
    ft->dwLowDateTime = (DWORD)v;
    ft->dwHighDateTime = (DWORD)(v >> 32);
    return S_OK;
}

int StringFromGUID2(const GUID *guid, WCHAR *out, int max)
{
    char text[64];
    int n = format_guid(guid, text, sizeof text);
    if (max < n + 1) return 0;
    for (int i = 0; i <= n; ++i) out[i] = (unsigned char)text[i];
    return n + 1;
}

HRESULT StringFromCLSID(const GUID *clsid, WCHAR **out)
{
    WCHAR *buffer = (WCHAR *)malloc(39 * sizeof(WCHAR));
    StringFromGUID2(clsid, buffer, 39);
    *out = buffer;
    return S_OK;
}

HRESULT CLSIDFromString(const WCHAR *text, GUID *clsid)
{
    char narrow[64] = {};
    for (int i = 0; i < 63 && text && text[i]; ++i) narrow[i] = (char)text[i];
    unsigned int d1, d2, d3, b[8];
    if (sscanf(narrow, "{%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x}", &d1, &d2, &d3, &b[0], &b[1], &b[2], &b[3], &b[4],
               &b[5], &b[6], &b[7]) != 11)
        return CO_E_CLASSSTRING;
    clsid->d1 = d1;
    clsid->d2 = (uint16_t)d2;
    clsid->d3 = (uint16_t)d3;
    for (int i = 0; i < 8; ++i) clsid->d4[i] = (uint8_t)b[i];
    return S_OK;
}

// Structured storage and persistence (save games): not yet ported.
HRESULT StgCreateDocfile(const WCHAR *name, DWORD mode, DWORD reserved, void **out)
{ (void)name; (void)mode; (void)reserved; if (out) *out = nullptr; unimplemented("StgCreateDocfile"); return E_NOTIMPL; }
HRESULT StgOpenStorage(const WCHAR *name, void *priority, DWORD mode, void *exclude, DWORD reserved, void **out)
{ (void)name; (void)priority; (void)mode; (void)exclude; (void)reserved; if (out) *out = nullptr; unimplemented("StgOpenStorage"); return E_NOTIMPL; }
HRESULT OleSaveToStream(void *persist, void *stream) { (void)persist; (void)stream; unimplemented("OleSaveToStream"); return E_NOTIMPL; }
HRESULT OleLoadFromStream(void *stream, const GUID *iid, void **out) { (void)stream; (void)iid; if (out) *out = nullptr; unimplemented("OleLoadFromStream"); return E_NOTIMPL; }

// BSTR: a 32-bit byte length before the UTF-16 text, NUL-terminated.
WCHAR *SysAllocString(const WCHAR *text)
{
    if (!text) return nullptr;
    size_t n = wlen(text);
    uint32_t *block = (uint32_t *)malloc(4 + (n + 1) * 2);
    block[0] = (uint32_t)(n * 2);
    memcpy(block + 1, text, (n + 1) * 2);
    return (WCHAR *)(block + 1);
}
void SysFreeString(WCHAR *text) { if (text) free((uint32_t *)text - 1); }
HRESULT LoadTypeLib(const WCHAR *file, void **out) { (void)file; if (out) *out = nullptr; unimplemented("LoadTypeLib"); return E_NOTIMPL; }
HRESULT RevokeActiveObject(DWORD cookie, void *reserved) { (void)cookie; (void)reserved; return S_OK; }

}  // extern "C"
