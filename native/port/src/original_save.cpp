// Reading the original game's saves (lane C6, roadmap item (d)).
//
// The original's CONTENTS stream holds i686 data: each pointer and pointer-sized
// integer is 4 bytes, objects have VC6 layouts. While StgOpenStorage has a
// compound file (an original save) open, ra2port_original_save_mode is set and
// every typed IStream::Read (com_persist.hpp's overlay) comes here with the kind
// of its destination (ra2port/prelude.hpp):
//  - pointers, intptr_t/uintptr_t: 4 bytes per value read and widened (zero-
//    extended, sign-extended for intptr_t). They are swizzle ids and saved
//    pointer fields: original heap and image addresses, all below 2 GB.
//  - objects: converted from the i686 layout to the native one (field maps,
//    original_layouts: arm64, or x86_64 Windows in the Windows build, lane C15);
//    a type without a map is read as is and reported once.
//  - everything else: read as is.
#include "ra2port/win32_port.hpp"
#include "ra2port/original_layouts.hpp"

#include <cxxabi.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <mach-o/loader.h>
#endif
#include <map>
#include <mutex>
#include <set>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <vector>

extern "C" int ra2port_original_save_mode = 0;
#if defined(_WIN32)
extern "C" char __ImageBase;   // the linker's: the exe's IMAGE_DOS_HEADER
#endif

// No maps when the build has none (tools/original_layouts.py writes the strong
// definitions into gen/original_layouts.cpp for the game).
extern "C" __attribute__((weak)) const OriginalLayoutOp ra2port_original_ops[1] = {};
extern "C" __attribute__((weak)) const OriginalLayout ra2port_original_layouts[1] = {};
extern "C" __attribute__((weak)) const unsigned ra2port_original_layout_count = 0;
extern "C" __attribute__((weak)) const char *const ra2port_original_vtable_names[1] = {nullptr};

typedef int32_t (*ra2port_stream_read_fn)(void *stream, void *buffer, uint32_t size, uint32_t *read);
typedef int32_t (*ra2port_stream_write_fn)(void *stream, void const *buffer, uint32_t size, uint32_t *written);

namespace {
enum { RAW, UNSIGNED_WIDE, SIGNED_WIDE, OBJECT, POINTER };

bool log_on()
{
    static int on = -1;
    if (on < 0) on = getenv("RA2_NATIVE_ORIGINAL_LOG") != nullptr;
    return on != 0;
}
void olog(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void olog(const char *fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    fprintf(stderr, "original-save: ");
    vfprintf(stderr, fmt, a);
    fprintf(stderr, "\n");
    va_end(a);
}

// RA2_NATIVE_SAVE_INDEX=<file> (lane C17, comparing saves): one line per typed read or
// write of a save's CONTENTS stream: R|W, its position in the decompressed stream (the
// CStreamClass byte counts, cstream.cpp fixups), the bytes in the file, the kind and map.
std::mutex index_lock;
std::map<std::pair<void *, int>, uint64_t> &index_positions()   // (stream, writing) -> bytes
{
    static std::map<std::pair<void *, int>, uint64_t> positions;
    return positions;
}
FILE *index_file()
{
    static FILE *f = nullptr;
    static bool tried = false;
    if (!tried) {
        tried = true;
        const char *path = getenv("RA2_NATIVE_SAVE_INDEX");
        if (path && *path) f = fopen(path, "w");
    }
    return f;
}
void index_note(char rw, void *stream, uint32_t bytes, const char *kind, const char *name, uint32_t count)
{
    FILE *f = index_file();
    if (!f) return;
    std::lock_guard<std::mutex> g(index_lock);
    auto &positions = index_positions();
    auto it = positions.find(std::make_pair(stream, rw == 'W' ? 1 : 0));
    uint64_t at = it == positions.end() ? 0 : it->second;
    fprintf(f, "%c %p %llu %u %s %s %u\n", rw, stream, (unsigned long long)at, bytes, kind, name, count);
    fflush(f);
}

// "UnitClass", "DynamicVectorClass<int>": without class/struct keywords and spaces.
std::string canonical(std::string s)
{
    static const char *const words[] = {"class ", "struct ", "union ", "enum "};
    for (const char *w : words) {
        size_t n = strlen(w), at;
        while ((at = s.find(w)) != std::string::npos) s.erase(at, n);
    }
    std::string out;
    for (char c : s) if (c != ' ') out.push_back(c);
    return out;
}

// The static type of original_type_name<T>: "... [T = Name]".
std::string static_name(const char *pretty)
{
    if (!pretty) return std::string();
    const char *at = strstr(pretty, "[T = ");
    if (!at) return canonical(pretty);
    std::string s(at + 5);
    if (!s.empty() && s.back() == ']') s.pop_back();
    return canonical(s);
}

uint64_t load_unit(const uint8_t *p, unsigned bytes)
{
    uint64_t v = 0;
    for (unsigned i = 0; i < bytes; ++i) v |= (uint64_t)p[i] << (8 * i);
    return v;
}

// Whether p lies in the game's own image (its vtables, RTTI records and their
// names). A load may read an object over raw memory (VeinholeMonsterClass's
// load reads one into new char[]): its vtable pointer is then garbage.
bool in_image(void const *p)
{
#if defined(__APPLE__)
    static uintptr_t lo = 0, hi = 0;
    if (!hi) {
        // The image this code is in (the executable, or an engine bundle of the combined app,
        // lane Y5), not dyld's image 0.
        Dl_info self = {};
        if (!dladdr((const void *)&in_image, &self) || !self.dli_fbase) return false;
        const struct mach_header_64 *h = (const struct mach_header_64 *)self.dli_fbase;
        intptr_t slide = 0;
        {
            const uint8_t *c = (const uint8_t *)(h + 1);
            for (uint32_t i = 0; i < h->ncmds; ++i) {
                const struct load_command *lc = (const struct load_command *)c;
                if (lc->cmd == LC_SEGMENT_64 && !strcmp(((const struct segment_command_64 *)c)->segname, "__TEXT"))
                    slide = (intptr_t)((uintptr_t)h - ((const struct segment_command_64 *)c)->vmaddr);
                c += lc->cmdsize;
            }
        }
        const uint8_t *c = (const uint8_t *)(h + 1);
        uintptr_t l = UINTPTR_MAX, e = 0;
        for (uint32_t i = 0; i < h->ncmds; ++i) {
            const struct load_command *lc = (const struct load_command *)c;
            if (lc->cmd == LC_SEGMENT_64) {
                const struct segment_command_64 *s = (const struct segment_command_64 *)c;
                if (s->vmsize && strcmp(s->segname, "__PAGEZERO") && strcmp(s->segname, "__LINKEDIT")) {
                    l = std::min(l, (uintptr_t)(s->vmaddr + slide));
                    e = std::max(e, (uintptr_t)(s->vmaddr + s->vmsize + slide));
                }
            }
            c += lc->cmdsize;
        }
        lo = l;
        hi = e;
    }
    return (uintptr_t)p >= lo && (uintptr_t)p < hi;
#elif defined(_WIN32)
    // The exe's image from its PE headers (lane C15): IMAGE_DOS_HEADER.e_lfanew at
    // 0x3C, the PE32+ optional header's SizeOfImage at 0x50 of the NT headers.
    const uint8_t *base = (const uint8_t *)&__ImageBase;
    uint32_t nt = (uint32_t)load_unit(base + 0x3C, 4);
    uintptr_t size = (uintptr_t)load_unit(base + nt + 0x50, 4);
    return (uintptr_t)p >= (uintptr_t)base && (uintptr_t)p < (uintptr_t)base + size;
#else
    return p != nullptr;
#endif
}

// The most derived type of a polymorphic object, from its vtable's RTTI slot
// (empty when the object holds no vtable of the game's).
std::string dynamic_name(void const *vptr)
{
    static std::mutex lock;
    static std::map<void const *, std::string> known;
    std::lock_guard<std::mutex> g(lock);
    auto it = known.find(vptr);
    if (it != known.end()) return it->second;
    std::string &name = known[vptr];
    if (((uintptr_t)vptr & 7) || !in_image(vptr) || !in_image((char const *)vptr - 8)) return name;
    std::type_info const *info = ((std::type_info const *const *)vptr)[-1];
    if (((uintptr_t)info & 7) || !in_image(info)) return name;
    const char *raw = ((const char *const *)info)[1];   // the Itanium type_info's name
    if (!in_image(raw)) return name;
    int status = 0;
    char *d = abi::__cxa_demangle(info->name(), nullptr, nullptr, &status);
    std::string s = d && status == 0 ? d : info->name();
    free(d);
    name = canonical(s);
    return name;
}

int32_t read_widened(ra2port_stream_read_fn stream_read, void *stream, void *buffer, uint32_t size, uint32_t *read,
                     bool is_signed, bool pointers = false)
{
    uint32_t count = size == 4 ? 1 : size / 8;
    index_note('R', stream, count * 4, pointers ? "pointer" : is_signed ? "signed" : "wide", "-", count);
    std::vector<uint32_t> narrow(count ? count : 1);
    uint32_t got = 0;
    int32_t r = stream_read(stream, narrow.data(), count * 4, &got);
    uint64_t *out = (uint64_t *)buffer;
    for (uint32_t i = 0; i < got / 4; ++i) {
        out[i] = is_signed ? (uint64_t)(int64_t)(int32_t)narrow[i] : (uint64_t)narrow[i];
    }
    if (read) *read = got == count * 4 ? size : got * 2;
    return r;
}
OriginalLayout const *layout(std::string const &name)
{
    static std::map<std::string, OriginalLayout const *> *index = nullptr;
    static std::mutex lock;
    std::lock_guard<std::mutex> g(lock);
    if (!index) {
        index = new std::map<std::string, OriginalLayout const *>;
        for (unsigned i = 0; i < ra2port_original_layout_count; ++i) {
            (*index)[ra2port_original_layouts[i].name] = &ra2port_original_layouts[i];
        }
    }
    auto it = index->find(name);
    return it == index->end() ? nullptr : it->second;
}

void store_unit(uint8_t *p, unsigned bytes, uint64_t v)
{
    for (unsigned i = 0; i < bytes; ++i) p[i] = (uint8_t)(v >> (8 * i));
}

// The object's i686 image from the stream, spread over the arm64 object by its
// field map. Vtable pointers and members only the port has keep their values.
void convert(uint8_t *out, const uint8_t *in, OriginalLayout const &l)
{
    for (uint32_t i = 0; i < l.count; ++i) {
        OriginalLayoutOp const &o = ra2port_original_ops[l.first + i];
        switch (o.kind) {
        case ORIGINAL_OP_COPY:
            memcpy(out + o.dst, in + o.src, o.n);
            break;
        case ORIGINAL_OP_WIDEN:
        case ORIGINAL_OP_POINTER:
        case ORIGINAL_OP_WIDEN_S:
            for (uint32_t k = 0; k < o.n; ++k) {
                uint32_t v = (uint32_t)load_unit(in + o.src + 4 * k, 4);
                uint64_t w = o.kind == ORIGINAL_OP_WIDEN_S ? (uint64_t)(int64_t)(int32_t)v : (uint64_t)v;
                store_unit(out + o.dst + 8 * k, 8, w);
            }
            break;
        case ORIGINAL_OP_BITS: {
            uint64_t mask = o.width >= 64 ? ~0ull : ((1ull << o.width) - 1);
            uint64_t v = (load_unit(in + o.src, o.unit) >> o.sbit) & mask;
            uint64_t d = load_unit(out + o.dst, o.dunit);
            d = (d & ~(mask << o.dbit)) | (v << o.dbit);
            store_unit(out + o.dst, o.dunit, d);
            break;
        }
        default:
            break;
        }
    }
}

// count objects (an array of them when a read covers several).
int32_t read_object(ra2port_stream_read_fn stream_read, void *stream, void *buffer, uint32_t size, uint32_t *read,
                    OriginalLayout const &l, uint32_t count)
{
    uint32_t bytes = l.i686_size * count;
    index_note('R', stream, bytes, "object", l.name, count);
    std::vector<uint8_t> image(bytes);
    uint32_t got = 0;
    int32_t r = stream_read(stream, image.data(), bytes, &got);
    if (got < bytes) std::fill(image.begin() + got, image.end(), 0);
    for (uint32_t k = 0; k < count; ++k) {
        convert((uint8_t *)buffer + (size_t)k * l.arm64_size, image.data() + (size_t)k * l.i686_size, l);
    }
    if (log_on()) olog("%s: %u x %u i686 bytes to %u (read asked %u)", l.name, count, l.i686_size, l.arm64_size, size);
    if (read) *read = got == bytes ? size : got;
    return r;
}
}  // namespace

extern "C" int32_t ra2port_original_read(ra2port_stream_read_fn stream_read, void *stream, void *buffer, uint32_t size,
                                         uint32_t *read, int kind, void const *vptr, char const *type_name)
{
    switch (kind) {
    case POINTER:
    case UNSIGNED_WIDE:
    case SIGNED_WIDE:
        if (size == 4 || (size && size % 8 == 0)) {
            return read_widened(stream_read, stream, buffer, size, read, kind == SIGNED_WIDE, kind == POINTER);
        }
        break;
    case OBJECT: {
        // The destination's own type when the read is exactly one of it (a load's
        // sizeof(*this); also an object read over raw memory, whose vtable pointer is
        // garbage), else the most derived type when the read covers it
        // (AbstractClass::Load's Size_Of()), else an array of the destination's type.
        OriginalLayout const *s = layout(static_name(type_name));
        if (s && (size == s->arm64_size || size == s->i686_size)) {
            return read_object(stream_read, stream, buffer, size, read, *s, 1);
        }
        std::string dynamic = vptr ? dynamic_name(vptr) : std::string();
        OriginalLayout const *d = dynamic.empty() ? nullptr : layout(dynamic);
        if (d && (size == d->arm64_size || size == d->i686_size)) {
            return read_object(stream_read, stream, buffer, size, read, *d, 1);
        }
        if (s && s->arm64_size && size % s->arm64_size == 0) {
            return read_object(stream_read, stream, buffer, size, read, *s, size / s->arm64_size);
        }
        std::string names[2] = {dynamic, static_name(type_name)};
        static std::mutex lock;
        static std::set<std::string> reported;
        std::lock_guard<std::mutex> g(lock);
        std::string name = names[0].empty() ? names[1] : names[0];
        if (reported.insert(name + "/" + std::to_string(size)).second) {
            olog("no layout for %s (%u bytes): read as is", name.c_str(), size);
        }
        break;
    }
    default:
        break;
    }
    return stream_read(stream, buffer, size, read);
}

// ---- Writing the original's saves (lane C17; every save the port writes since lane C20) ----
// While a storage written in the original's format is open (storage.cpp sets
// ra2port_original_write_mode), every typed IStream::Write comes here, the mirror of
// the read above: pointers are written as 4-byte swizzle ids (the id an object had in
// the original save it was loaded from, else a new one; the game's globals and the
// mapped image as the original's own addresses), pointer-width integers as 4 bytes,
// objects as their i686 image through the field maps run backwards, with the
// original's vtable addresses in their vtable pointers and padding zeroed.
extern "C" int ra2port_original_write_mode = 0;

namespace ra2port {
uint32_t original_address_of(const void *p);                        // original_image.cpp
uint32_t original_vtable(uint32_t primary, uint32_t object_offset);
uint32_t original_vtable_named(const char *type_descriptor_name, uint32_t object_offset);
}

namespace {
std::mutex id_lock;
std::map<uintptr_t, uint32_t> &pointer_ids()
{
    static std::map<uintptr_t, uint32_t> ids;
    return ids;
}
std::set<uint32_t> &used_ids()
{
    static std::set<uint32_t> used;
    return used;
}

// Fresh ids: 8-aligned values above any original heap address seen in the saves (the
// original's swizzle manager needs only distinct non-zero ids).
uint32_t pointer_id(uint64_t value)
{
    if (value == 0) return 0;
    if (uint32_t va = ra2port::original_address_of((const void *)(uintptr_t)value)) return va;
    std::lock_guard<std::mutex> g(id_lock);
    auto &ids = pointer_ids();
    auto it = ids.find((uintptr_t)value);
    if (it != ids.end()) return it->second;
    static uint32_t next = 0x10000000u;
    auto &used = used_ids();
    while (used.count(next)) next += 8;
    uint32_t id = next;
    next += 8;
    ids[(uintptr_t)value] = id;
    used.insert(id);
    return id;
}

void report_once(std::string const &what)
{
    static std::mutex lock;
    static std::set<std::string> seen;
    std::lock_guard<std::mutex> g(lock);
    if (seen.insert(what).second) olog("%s", what.c_str());
}

// The i686 image of one native object (the read's convert run backwards).
void convert_back(uint8_t *out, const uint8_t *in, OriginalLayout const &l)
{
    memset(out, 0, l.i686_size);
    for (uint32_t i = 0; i < l.count; ++i) {
        OriginalLayoutOp const &o = ra2port_original_ops[l.first + i];
        switch (o.kind) {
        case ORIGINAL_OP_COPY:
            memcpy(out + o.src, in + o.dst, o.n);
            break;
        case ORIGINAL_OP_WIDEN:
        case ORIGINAL_OP_WIDEN_S:
            for (uint32_t k = 0; k < o.n; ++k) store_unit(out + o.src + 4 * k, 4, (uint32_t)load_unit(in + o.dst + 8 * k, 8));
            break;
        case ORIGINAL_OP_POINTER:
            for (uint32_t k = 0; k < o.n; ++k) store_unit(out + o.src + 4 * k, 4, pointer_id(load_unit(in + o.dst + 8 * k, 8)));
            break;
        case ORIGINAL_OP_BITS: {
            uint64_t mask = o.width >= 64 ? ~0ull : ((1ull << o.width) - 1);
            uint64_t v = (load_unit(in + o.dst, o.dunit) >> o.dbit) & mask;
            uint64_t d = load_unit(out + o.src, o.unit);
            d = (d & ~(mask << o.sbit)) | (v << o.sbit);
            store_unit(out + o.src, o.unit, d);
            break;
        }
        case ORIGINAL_OP_VPTR: {
            uint32_t va = ra2port::original_vtable(o.n, o.dst);
            if (!va) report_once(std::string("write: no original vtable for ") + l.name + " +" + std::to_string(o.src));
            store_unit(out + o.src, 4, va);
            break;
        }
        case ORIGINAL_OP_VPTR_NAMED: {
            const char *td = ra2port_original_vtable_names[o.n];
            uint32_t va = td ? ra2port::original_vtable_named(td, o.dst) : 0;
            if (!va) report_once(std::string("write: no original vtable for ") + l.name + " +" + std::to_string(o.src) +
                                 " (" + (td ? td : "?") + " +" + std::to_string(o.dst) + ")");
            store_unit(out + o.src, 4, va);
            break;
        }
        default:
            break;
        }
    }
}

int32_t write_object(ra2port_stream_write_fn stream_write, void *stream, void const *buffer, uint32_t size,
                     uint32_t *written, OriginalLayout const &l, uint32_t count)
{
    std::vector<uint8_t> image((size_t)l.i686_size * count);
    index_note('W', stream, (uint32_t)image.size(), "object", l.name, count);
    for (uint32_t k = 0; k < count; ++k) {
        convert_back(image.data() + (size_t)k * l.i686_size, (const uint8_t *)buffer + (size_t)k * l.arm64_size, l);
    }
    uint64_t put = 0;   // the stream's Write stores an unsigned long (8 bytes on LP64)
    int32_t r = stream_write(stream, image.data(), (uint32_t)image.size(), (uint32_t *)&put);
    if (log_on()) olog("write %s: %u x %u bytes to %u i686 bytes", l.name, count, l.arm64_size, l.i686_size);
    if (written) *written = (uint32_t)put == image.size() ? size : (uint32_t)put;
    return r;
}

int32_t write_narrowed(ra2port_stream_write_fn stream_write, void *stream, void const *buffer, uint32_t size,
                       uint32_t *written, bool pointers, bool kind_signed = false)
{
    uint32_t count = size / 8;
    index_note('W', stream, count * 4, pointers ? "pointer" : kind_signed ? "signed" : "wide", "-", count);
    std::vector<uint32_t> narrow(count);
    for (uint32_t i = 0; i < count; ++i) {
        uint64_t v = load_unit((const uint8_t *)buffer + 8 * i, 8);
        narrow[i] = pointers ? pointer_id(v) : (uint32_t)v;
    }
    uint64_t put = 0;
    int32_t r = stream_write(stream, narrow.data(), count * 4, (uint32_t *)&put);
    if (written) *written = (uint32_t)put == count * 4 ? size : (uint32_t)put * 2;
    return r;
}
}  // namespace

// The id an object had in the original save being read (SwizzleManagerClass::Here_I_Am,
// port/fixups.json): written again for it, so a save written after loading one keeps them.
extern "C" void ra2port_original_note_id(intptr_t id, void *pointer)
{
    if (!ra2port_original_save_mode || !pointer || id <= 0 || (uint64_t)id > 0xFFFFFFFFu) return;
    std::lock_guard<std::mutex> g(id_lock);
    pointer_ids()[(uintptr_t)pointer] = (uint32_t)id;
    used_ids().insert((uint32_t)id);
}

extern "C" int32_t ra2port_original_write(ra2port_stream_write_fn stream_write, void *stream, void const *buffer,
                                          uint32_t size, uint32_t *written, int kind, void const *vptr,
                                          char const *type_name)
{
    switch (kind) {
    case POINTER:
    case UNSIGNED_WIDE:
    case SIGNED_WIDE:
        if (size && size % 8 == 0) {
            return write_narrowed(stream_write, stream, buffer, size, written, kind == POINTER, kind == SIGNED_WIDE);
        }
        if (size == 4) {
            // One value written with VC6's 4-byte size (Yuri's Revenge's LayerClass::Save writes each
            // object pointer with sizeof(int)): its 4-byte form, as the read takes size 4 (lane Y10).
            uint32_t got = 0;
            int32_t r = write_narrowed(stream_write, stream, buffer, 8, &got, kind == POINTER, kind == SIGNED_WIDE);
            if (written) *written = got == 8 ? 4 : got;
            return r;
        }
        break;
    case OBJECT: {
        OriginalLayout const *s = layout(static_name(type_name));
        if (s && size == s->arm64_size) return write_object(stream_write, stream, buffer, size, written, *s, 1);
        std::string dynamic = vptr ? dynamic_name(vptr) : std::string();
        OriginalLayout const *d = dynamic.empty() ? nullptr : layout(dynamic);
        if (d && size == d->arm64_size) return write_object(stream_write, stream, buffer, size, written, *d, 1);
        if (s && s->arm64_size && size % s->arm64_size == 0) {
            return write_object(stream_write, stream, buffer, size, written, *s, size / s->arm64_size);
        }
        report_once("write: no layout for " + (dynamic.empty() ? static_name(type_name) : dynamic) + " (" +
                    std::to_string(size) + " bytes): written as is");
        break;
    }
    default:
        break;
    }
    return stream_write(stream, buffer, size, written);
}

extern "C" void ra2port_save_index_bytes(void *stream, unsigned long size, int writing)
{
    if (!index_file()) return;
    std::lock_guard<std::mutex> g(index_lock);
    index_positions()[std::make_pair(stream, writing ? 1 : 0)] += size;
}
