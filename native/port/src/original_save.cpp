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
//  - objects: converted from the i686 layout to the arm64 one (field maps,
//    original_layouts); a type without a map is read as is and reported once.
//  - everything else: read as is.
#include "ra2port/win32_port.hpp"
#include "ra2port/original_layouts.hpp"

#include <cxxabi.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
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

// No maps when the build has none (tools/original_layouts.py writes the strong
// definitions into gen/original_layouts.cpp for the game).
extern "C" __attribute__((weak)) const OriginalLayoutOp ra2port_original_ops[1] = {};
extern "C" __attribute__((weak)) const OriginalLayout ra2port_original_layouts[1] = {};
extern "C" __attribute__((weak)) const unsigned ra2port_original_layout_count = 0;

typedef int32_t (*ra2port_stream_read_fn)(void *stream, void *buffer, uint32_t size, uint32_t *read);

namespace {
enum { RAW, UNSIGNED_WIDE, SIGNED_WIDE, OBJECT };

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

// Whether p lies in the game's own image (its vtables, RTTI records and their
// names). A load may read an object over raw memory (VeinholeMonsterClass's
// load reads one into new char[]): its vtable pointer is then garbage.
bool in_image(void const *p)
{
#if defined(__APPLE__)
    static uintptr_t lo = 0, hi = 0;
    if (!hi) {
        const struct mach_header_64 *h = (const struct mach_header_64 *)_dyld_get_image_header(0);
        intptr_t slide = _dyld_get_image_vmaddr_slide(0);
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
                     bool is_signed)
{
    uint32_t count = size == 4 ? 1 : size / 8;
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

uint64_t load_unit(const uint8_t *p, unsigned bytes)
{
    uint64_t v = 0;
    for (unsigned i = 0; i < bytes; ++i) v |= (uint64_t)p[i] << (8 * i);
    return v;
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
    case UNSIGNED_WIDE:
    case SIGNED_WIDE:
        if (size == 4 || (size && size % 8 == 0)) {
            return read_widened(stream_read, stream, buffer, size, read, kind == SIGNED_WIDE);
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
