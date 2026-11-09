// OLE32 structured storage and stream persistence for save games (Scout1125,
// native M15): StgCreateDocfile/StgOpenStorage, IStorage, IStream,
// IPropertySetStorage/IPropertyStorage and OleSaveToStream/OleLoadFromStream,
// as far as the game's save code calls them (SaveLoad.cpp Save_Game 0x006554A0,
// Load_Game 0x00656890, SavegameInformation 0x00659530/0x00659A90; slot
// orders as in the Win32 SDK's objidl.h/propidl.h).
//
// A storage is one host file in the port's own container format (not the
// compound-file format: native save games are read by the native game only):
// "RA2NSTG1", the named streams, then the property sets. Streams live in
// memory; Commit (or the last Release of a writable storage) writes the file.
#include "ra2port/win32_port.hpp"

#include <map>
#include <memory>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

using namespace ra2port;

namespace {
const HRESULT S_OK = 0, S_FALSE = 1, E_NOTIMPL = (HRESULT)0x80004001u, E_NOINTERFACE = (HRESULT)0x80004002u,
              E_POINTER = (HRESULT)0x80004003u, E_FAIL = (HRESULT)0x80004005u,
              STG_E_FILENOTFOUND = (HRESULT)0x80030002u, STG_E_INVALIDFUNCTION = (HRESULT)0x80030001u,
              STG_E_READFAULT = (HRESULT)0x8003001Eu, STG_E_FILEALREADYEXISTS = (HRESULT)0x80030050u;

struct GUID { uint32_t d1; uint16_t d2, d3; uint8_t d4[8]; };
bool operator<(const GUID &a, const GUID &b) { return memcmp(&a, &b, sizeof a) < 0; }
bool same(const GUID *a, const GUID &b) { return a && !memcmp(a, &b, sizeof b); }
const GUID IID_IUnknown = {0, 0, 0, {0xC0, 0, 0, 0, 0, 0, 0, 0x46}};
const GUID IID_ISequentialStream = {0x0c733a30, 0x2a1c, 0x11ce, {0xad, 0xe5, 0x00, 0xaa, 0x00, 0x44, 0x77, 0x3d}};
const GUID IID_IStream = {0x0000000c, 0, 0, {0xC0, 0, 0, 0, 0, 0, 0, 0x46}};
const GUID IID_IStorage = {0x0000000b, 0, 0, {0xC0, 0, 0, 0, 0, 0, 0, 0x46}};
const GUID IID_IPropertySetStorage = {0x0000013A, 0, 0, {0xC0, 0, 0, 0, 0, 0, 0, 0x46}};
const GUID IID_IPropertyStorage = {0x00000138, 0, 0, {0xC0, 0, 0, 0, 0, 0, 0, 0x46}};
const GUID IID_IPersistStream = {0x00000109, 0, 0, {0xC0, 0, 0, 0, 0, 0, 0, 0x46}};

typedef std::basic_string<WCHAR> wstr;
wstr to_w(const WCHAR *s) { return s ? wstr(s, wlen(s)) : wstr(); }

// RA2_NATIVE_STG_LOG=1: one stderr line per storage call (diagnostics).
void stg_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void stg_log(const char *fmt, ...)
{
    static int on = -1;
    if (on < 0) on = getenv("RA2_NATIVE_STG_LOG") != nullptr;
    if (!on) return;
    va_list a;
    va_start(a, fmt);
    fprintf(stderr, "stg: ");
    vfprintf(stderr, fmt, a);
    fprintf(stderr, "\n");
    va_end(a);
}


struct IUnknownPort {
    virtual HRESULT QueryInterface(const GUID *iid, void **out) = 0;
    virtual ULONG AddRef() = 0;
    virtual ULONG Release() = 0;
};

// PROPVARIANT as the game declares it natively (shared/savegame_info.hpp: vt,
// three reserved words, then a union of a 32-bit long, two string pointers and
// a FILETIME): 16 bytes on arm64.
struct PropVariant {
    uint16_t vt, r1, r2, r3;
    union { int32_t lVal; WCHAR *pwszVal; char *pszVal; FILETIME filetime; };
};
struct PropSpec { uint32_t kind, propid; };
enum { VT_EMPTY = 0, VT_I2 = 2, VT_I4 = 3, VT_BOOL = 11, VT_UI4 = 19, VT_LPSTR = 30, VT_LPWSTR = 31, VT_FILETIME = 64 };
struct Prop { uint16_t vt = VT_EMPTY; int32_t value = 0; FILETIME time = {0, 0}; std::string narrow; wstr wide; };
typedef std::map<uint32_t, Prop> PropSet;

struct Data {
    std::map<wstr, std::shared_ptr<std::vector<uint8_t>>> streams;
    std::map<GUID, PropSet> sets;
};

// ---- IStream over a shared byte vector ----
struct Stream : IUnknownPort {
    ULONG refs = 1;
    std::shared_ptr<std::vector<uint8_t>> bytes;
    uint64_t pos = 0;
    explicit Stream(std::shared_ptr<std::vector<uint8_t>> b) : bytes(b) {}
    HRESULT QueryInterface(const GUID *iid, void **out) override
    {
        if (!out) return E_POINTER;
        if (same(iid, IID_IUnknown) || same(iid, IID_IStream) || same(iid, IID_ISequentialStream)) {
            *out = this;
            AddRef();
            return S_OK;
        }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG AddRef() override { return ++refs; }
    ULONG Release() override { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT Read(void *buffer, ULONG size, ULONG *read)
    {
        uint64_t avail = pos < bytes->size() ? bytes->size() - pos : 0;
        ULONG n = size < avail ? size : (ULONG)avail;
        if (n) memcpy(buffer, bytes->data() + pos, n);
        pos += n;
        if (read) *read = n;
        return n < size ? S_FALSE : S_OK;
    }
    virtual HRESULT Write(const void *buffer, ULONG size, ULONG *written)
    {
        if (pos + size > bytes->size()) bytes->resize(pos + size);
        if (size) memcpy(bytes->data() + pos, buffer, size);
        pos += size;
        if (written) *written = size;
        return S_OK;
    }
    virtual HRESULT Seek(int64_t move, DWORD origin, uint64_t *position)
    {
        int64_t base = origin == 0 ? 0 : origin == 1 ? (int64_t)pos : (int64_t)bytes->size();
        if (origin > 2 || base + move < 0) return STG_E_INVALIDFUNCTION;
        pos = (uint64_t)(base + move);
        if (position) *position = pos;
        return S_OK;
    }
    virtual HRESULT SetSize(uint64_t size) { bytes->resize(size); return S_OK; }
    virtual HRESULT CopyTo(IUnknownPort *to, uint64_t count, uint64_t *read, uint64_t *written)
    {
        uint64_t avail = pos < bytes->size() ? bytes->size() - pos : 0;
        uint64_t n = count < avail ? count : avail;
        ULONG done = 0;
        HRESULT r = ((Stream *)to)->Write(bytes->data() + pos, (ULONG)n, &done);
        pos += n;
        if (read) *read = n;
        if (written) *written = done;
        return r;
    }
    virtual HRESULT Commit(DWORD) { return S_OK; }
    virtual HRESULT Revert() { return S_OK; }
    virtual HRESULT LockRegion(uint64_t, uint64_t, DWORD) { return S_OK; }
    virtual HRESULT UnlockRegion(uint64_t, uint64_t, DWORD) { return S_OK; }
    virtual HRESULT Stat(void *stat, DWORD) { (void)stat; unimplemented("IStream::Stat"); return E_NOTIMPL; }
    virtual HRESULT Clone(void **out)
    {
        Stream *s = new Stream(bytes);
        s->pos = pos;
        *out = s;
        return S_OK;
    }
};

// ---- IPropertyStorage over one property set ----
struct PropertyStorage : IUnknownPort {
    ULONG refs = 1;
    PropSet *set;
    IUnknownPort *owner;
    PropertyStorage(PropSet *s, IUnknownPort *o) : set(s), owner(o) { owner->AddRef(); }
    ~PropertyStorage() { owner->Release(); }
    HRESULT QueryInterface(const GUID *iid, void **out) override
    {
        if (!out) return E_POINTER;
        if (same(iid, IID_IUnknown) || same(iid, IID_IPropertyStorage)) { *out = this; AddRef(); return S_OK; }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG AddRef() override { return ++refs; }
    ULONG Release() override { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT ReadMultiple(ULONG count, const PropSpec *specs, PropVariant *values)
    {
        bool any = false;
        for (ULONG i = 0; i < count; ++i) {
            PropVariant &v = values[i];
            memset(&v, 0, sizeof v);
            if (specs[i].kind != 1) continue;          // PRSPEC_PROPID only
            auto it = set->find(specs[i].propid);
            stg_log("read property %u: %s", (unsigned)specs[i].propid, it == set->end() ? "absent" : "found");
            if (it == set->end()) continue;
            Prop const &p = it->second;
            v.vt = p.vt;
            any = true;
            switch (p.vt) {
            case VT_LPWSTR: {
                WCHAR *w = (WCHAR *)malloc((p.wide.size() + 1) * sizeof(WCHAR));
                memcpy(w, p.wide.c_str(), (p.wide.size() + 1) * sizeof(WCHAR));
                v.pwszVal = w;
                break;
            }
            case VT_LPSTR: {
                char *s = (char *)malloc(p.narrow.size() + 1);
                memcpy(s, p.narrow.c_str(), p.narrow.size() + 1);
                v.pszVal = s;
                break;
            }
            case VT_FILETIME: v.filetime = p.time; break;
            default: v.lVal = p.value; break;
            }
        }
        return any ? S_OK : S_FALSE;
    }
    virtual HRESULT WriteMultiple(ULONG count, const PropSpec *specs, const PropVariant *values, ULONG)
    {
        for (ULONG i = 0; i < count; ++i) {
            if (specs[i].kind != 1) continue;
            Prop p;
            p.vt = values[i].vt;
            switch (p.vt) {
            case VT_LPWSTR: p.wide = to_w(values[i].pwszVal); break;
            case VT_LPSTR: p.narrow = values[i].pszVal ? values[i].pszVal : ""; break;
            case VT_FILETIME: p.time = values[i].filetime; break;
            default: p.value = values[i].lVal; break;
            }
            (*set)[specs[i].propid] = p;
        }
        return S_OK;
    }
    virtual HRESULT DeleteMultiple(ULONG count, const PropSpec *specs)
    {
        for (ULONG i = 0; i < count; ++i) set->erase(specs[i].propid);
        return S_OK;
    }
    virtual HRESULT ReadPropertyNames(ULONG, const uint32_t *, WCHAR **) { return E_NOTIMPL; }
    virtual HRESULT WritePropertyNames(ULONG, const uint32_t *, const WCHAR *const *) { return E_NOTIMPL; }
    virtual HRESULT DeletePropertyNames(ULONG, const uint32_t *) { return E_NOTIMPL; }
    virtual HRESULT Commit(DWORD) { return S_OK; }
    virtual HRESULT Revert() { return S_OK; }
};

// ---- IStorage (+ IPropertySetStorage as a second interface) ----
struct Storage;
struct PropertySetStorage : IUnknownPort {
    Storage *outer;
    explicit PropertySetStorage(Storage *s) : outer(s) {}
    HRESULT QueryInterface(const GUID *iid, void **out) override;
    ULONG AddRef() override;
    ULONG Release() override;
    virtual HRESULT Create(const GUID &fmtid, const GUID *clsid, DWORD flags, DWORD mode, void **out);
    virtual HRESULT Open(const GUID &fmtid, DWORD mode, void **out);
    virtual HRESULT Delete(const GUID &fmtid);
    virtual HRESULT Enum(void **out) { (void)out; return E_NOTIMPL; }
};

void put32(std::vector<uint8_t> &o, uint32_t v) { for (int i = 0; i < 4; ++i) o.push_back((uint8_t)(v >> (8 * i))); }
void putw(std::vector<uint8_t> &o, wstr const &s)
{
    put32(o, (uint32_t)s.size());
    for (WCHAR c : s) { o.push_back((uint8_t)c); o.push_back((uint8_t)(c >> 8)); }
}
struct Reader {
    const uint8_t *p, *end;
    bool ok = true;
    uint32_t u32()
    {
        if (end - p < 4) { ok = false; return 0; }
        uint32_t v = p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24;
        p += 4;
        return v;
    }
    wstr w()
    {
        uint32_t n = u32();
        wstr s;
        if ((uint64_t)(end - p) < (uint64_t)n * 2) { ok = false; return s; }
        for (uint32_t i = 0; i < n; ++i, p += 2) s.push_back((WCHAR)(p[0] | p[1] << 8));
        return s;
    }
    bool bytes(void *to, size_t n)
    {
        if ((size_t)(end - p) < n) { ok = false; return false; }
        memcpy(to, p, n);
        p += n;
        return true;
    }
};

struct Storage : IUnknownPort {
    ULONG refs = 1;
    std::string path;        // host path
    bool writable;
    bool dirty = false;
    Data data;
    PropertySetStorage props{this};
    Storage(std::string const &host, bool write) : path(host), writable(write) {}
    ~Storage() { if (writable && dirty) Save(); }
    bool Save()
    {
        std::vector<uint8_t> o;
        const char magic[8] = {'R', 'A', '2', 'N', 'S', 'T', 'G', '1'};
        o.insert(o.end(), magic, magic + 8);
        put32(o, (uint32_t)data.streams.size());
        for (auto &s : data.streams) {
            putw(o, s.first);
            put32(o, (uint32_t)s.second->size());
            o.insert(o.end(), s.second->begin(), s.second->end());
        }
        put32(o, (uint32_t)data.sets.size());
        for (auto &set : data.sets) {
            const uint8_t *g = (const uint8_t *)&set.first;
            o.insert(o.end(), g, g + sizeof(GUID));
            put32(o, (uint32_t)set.second.size());
            for (auto &p : set.second) {
                put32(o, p.first);
                put32(o, p.second.vt);
                put32(o, (uint32_t)p.second.value);
                put32(o, p.second.time.dwLowDateTime);
                put32(o, p.second.time.dwHighDateTime);
                wstr narrow(p.second.narrow.begin(), p.second.narrow.end());
                putw(o, narrow);
                putw(o, p.second.wide);
            }
        }
        FILE *f = fopen(path.c_str(), "wb");
        if (!f) return false;
        bool ok = fwrite(o.data(), 1, o.size(), f) == o.size();
        fclose(f);
        dirty = false;
        return ok;
    }
    bool Load()
    {
        FILE *f = fopen(path.c_str(), "rb");
        if (!f) return false;
        std::vector<uint8_t> in;
        uint8_t block[65536];
        size_t n;
        while ((n = fread(block, 1, sizeof block, f)) > 0) in.insert(in.end(), block, block + n);
        fclose(f);
        Reader r{in.data(), in.data() + in.size()};
        char magic[8];
        if (!r.bytes(magic, 8) || memcmp(magic, "RA2NSTG1", 8)) return false;
        uint32_t streams = r.u32();
        for (uint32_t i = 0; i < streams && r.ok; ++i) {
            wstr name = r.w();
            uint32_t size = r.u32();
            auto bytes = std::make_shared<std::vector<uint8_t>>(size);
            if (size) r.bytes(bytes->data(), size);
            data.streams[name] = bytes;
        }
        uint32_t sets = r.u32();
        for (uint32_t i = 0; i < sets && r.ok; ++i) {
            GUID g;
            r.bytes(&g, sizeof g);
            PropSet &set = data.sets[g];
            uint32_t count = r.u32();
            for (uint32_t k = 0; k < count && r.ok; ++k) {
                uint32_t id = r.u32();
                Prop p;
                p.vt = (uint16_t)r.u32();
                p.value = (int32_t)r.u32();
                p.time.dwLowDateTime = r.u32();
                p.time.dwHighDateTime = r.u32();
                wstr narrow = r.w();
                p.narrow.assign(narrow.begin(), narrow.end());
                p.wide = r.w();
                set[id] = p;
            }
        }
        return r.ok;
    }
    HRESULT QueryInterface(const GUID *iid, void **out) override
    {
        if (!out) return E_POINTER;
        if (same(iid, IID_IUnknown) || same(iid, IID_IStorage)) { *out = this; AddRef(); return S_OK; }
        if (same(iid, IID_IPropertySetStorage)) { *out = &props; AddRef(); return S_OK; }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG AddRef() override { return ++refs; }
    ULONG Release() override { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT CreateStream(const WCHAR *name, DWORD, DWORD, DWORD, void **out)
    {
        auto bytes = std::make_shared<std::vector<uint8_t>>();
        data.streams[to_w(name)] = bytes;
        dirty = true;
        *out = new Stream(bytes);
        return S_OK;
    }
    virtual HRESULT OpenStream(const WCHAR *name, void *, DWORD mode, DWORD, void **out)
    {
        auto it = data.streams.find(to_w(name));
        if (it == data.streams.end()) { *out = nullptr; return STG_E_FILENOTFOUND; }
        if (mode & 3) dirty = true;
        *out = new Stream(it->second);
        return S_OK;
    }
    virtual HRESULT CreateStorage(const WCHAR *, DWORD, DWORD, DWORD, void **out) { *out = nullptr; unimplemented("IStorage::CreateStorage"); return E_NOTIMPL; }
    virtual HRESULT OpenStorage(const WCHAR *, void *, DWORD, void *, DWORD, void **out) { *out = nullptr; unimplemented("IStorage::OpenStorage"); return E_NOTIMPL; }
    virtual HRESULT CopyTo(DWORD, const GUID *, void *, void *) { return E_NOTIMPL; }
    virtual HRESULT MoveElementTo(const WCHAR *, void *, const WCHAR *, DWORD) { return E_NOTIMPL; }
    virtual HRESULT Commit(DWORD) { return !writable || Save() ? S_OK : E_FAIL; }
    virtual HRESULT Revert() { return S_OK; }
    virtual HRESULT EnumElements(DWORD, void *, DWORD, void **out) { *out = nullptr; return E_NOTIMPL; }
    virtual HRESULT DestroyElement(const WCHAR *name) { data.streams.erase(to_w(name)); dirty = true; return S_OK; }
    virtual HRESULT RenameElement(const WCHAR *, const WCHAR *) { return E_NOTIMPL; }
    virtual HRESULT SetElementTimes(const WCHAR *, const FILETIME *, const FILETIME *, const FILETIME *) { return S_OK; }
    virtual HRESULT SetClass(const GUID *) { return S_OK; }
    virtual HRESULT SetStateBits(DWORD, DWORD) { return S_OK; }
    virtual HRESULT Stat(void *, DWORD) { unimplemented("IStorage::Stat"); return E_NOTIMPL; }
};

HRESULT PropertySetStorage::QueryInterface(const GUID *iid, void **out) { return outer->QueryInterface(iid, out); }
ULONG PropertySetStorage::AddRef() { return outer->AddRef(); }
ULONG PropertySetStorage::Release() { return outer->Release(); }
HRESULT PropertySetStorage::Create(const GUID &fmtid, const GUID *, DWORD, DWORD, void **out)
{
    PropSet &set = outer->data.sets[fmtid];
    set.clear();
    outer->dirty = true;
    *out = new PropertyStorage(&set, outer);
    return S_OK;
}
HRESULT PropertySetStorage::Open(const GUID &fmtid, DWORD mode, void **out)
{
    auto it = outer->data.sets.find(fmtid);
    stg_log("open property set %08x: %s", (unsigned)fmtid.d1, it == outer->data.sets.end() ? "absent" : "found");
    if (it == outer->data.sets.end()) { *out = nullptr; return STG_E_FILENOTFOUND; }
    if (mode & 3) outer->dirty = true;
    *out = new PropertyStorage(&it->second, outer);
    return S_OK;
}
HRESULT PropertySetStorage::Delete(const GUID &fmtid) { outer->data.sets.erase(fmtid); outer->dirty = true; return S_OK; }

std::string narrow_name(const WCHAR *name)
{
    std::string s;
    for (size_t i = 0; name && name[i]; ++i) s.push_back((char)name[i]);
    return s;
}

// IPersistStream as the game's objects implement it (IPersist::GetClassID slot 3).
struct PersistStreamPort : IUnknownPort {
    virtual HRESULT GetClassID(GUID *clsid) = 0;
    virtual HRESULT IsDirty() = 0;
    virtual HRESULT Load(void *stream) = 0;
    virtual HRESULT Save(void *stream, BOOL clear_dirty) = 0;
};
}  // namespace

extern "C" {
HRESULT CoCreateInstance(const GUID *clsid, IUnknownPort *outer, DWORD context, const GUID *iid, void **out);

// STGM_CREATE (0x1000) | STGM_READWRITE (2) | STGM_SHARE_EXCLUSIVE (0x10): the save code's 0x1012.
HRESULT StgCreateDocfile(const WCHAR *name, DWORD mode, DWORD reserved, void **out)
{
    (void)reserved;
    if (!out) return E_POINTER;
    char path[1024];
    std::string game = narrow_name(name);
    host_path_write(game.c_str(), path, sizeof path);
    if (!(mode & 0x1000)) {
        FILE *f = fopen(path, "rb");
        if (f) { fclose(f); *out = nullptr; return STG_E_FILEALREADYEXISTS; }
    }
    Storage *s = new Storage(path, true);
    s->dirty = true;
    stg_log("create %s -> %s", game.c_str(), path);
    *out = s;
    return S_OK;
}

HRESULT StgOpenStorage(const WCHAR *name, void *priority, DWORD mode, void *exclude, DWORD reserved, void **out)
{
    (void)priority; (void)exclude; (void)reserved;
    if (!out) return E_POINTER;
    char path[1024];
    std::string game = narrow_name(name);
    host_path(game.c_str(), path, sizeof path);
    Storage *s = new Storage(path, (mode & 3) != 0);
    stg_log("open %s -> %s mode %x", game.c_str(), path, (unsigned)mode);
    if (!s->Load()) {
        stg_log("open %s: not a native save storage", path);
        trace("StgOpenStorage %s -> %s: not a native save storage", game.c_str(), path);
        s->writable = false;
        s->Release();
        *out = nullptr;
        return STG_E_FILENOTFOUND;
    }
    *out = s;
    return S_OK;
}

// WriteClassStm then IPersistStream::Save(stream, TRUE) (ole32 OleSaveToStream).
HRESULT OleSaveToStream(void *persist, void *stream)
{
    if (!persist || !stream) return E_POINTER;
    PersistStreamPort *p = (PersistStreamPort *)persist;
    GUID clsid;
    HRESULT r = p->GetClassID(&clsid);
    if (r < 0) return r;
    r = ((Stream *)stream)->Write(&clsid, sizeof clsid, nullptr);
    if (r < 0) return r;
    static unsigned saved = 0;
    stg_log("save object %u class %08x", saved++, clsid.d1);
    return p->Save(stream, 1);
}

// ReadClassStm, CoCreateInstance, then IPersistStream::Load (ole32 OleLoadFromStream).
HRESULT OleLoadFromStream(void *stream, const GUID *iid, void **out)
{
    if (!out) return E_POINTER;
    *out = nullptr;
    GUID clsid;
    ULONG got = 0;
    HRESULT r = ((Stream *)stream)->Read(&clsid, sizeof clsid, &got);
    static unsigned loaded = 0;
    stg_log("load object %u class %08x%s", loaded++, clsid.d1, r < 0 || got != sizeof clsid ? " (read fault)" : "");
    if (r < 0 || got != sizeof clsid) return STG_E_READFAULT;
    void *object = nullptr;
    r = CoCreateInstance(&clsid, nullptr, 1, iid, &object);
    if (r < 0 || !object) stg_log("load object %u not created: %08x", loaded - 1, (unsigned)r);
    if (r < 0 || !object) return r < 0 ? r : E_FAIL;
    void *persist = nullptr;
    r = ((IUnknownPort *)object)->QueryInterface(&IID_IPersistStream, &persist);
    if (r < 0 || !persist) {
        ((IUnknownPort *)object)->Release();
        return r < 0 ? r : E_NOINTERFACE;
    }
    r = ((PersistStreamPort *)persist)->Load(stream);
    ((IUnknownPort *)persist)->Release();
    if (r < 0) stg_log("load object %u failed: %08x", loaded - 1, (unsigned)r);
    if (r < 0) {
        ((IUnknownPort *)object)->Release();
        return r;
    }
    *out = object;
    return S_OK;
}
}  // extern "C"
