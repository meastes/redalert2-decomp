// The user's installed game.exe as a data source (AGENTS.md "Native phase",
// user decision 2026-11-08): the native port may read data the original stores
// (file names, tables, constants) from the installation at run time, like other
// game files; nothing is copied into the repository or shipped. This maps the
// executable's sections at their virtual addresses into one native block (after
// checking it is the pinned target: size and SHA-256) and fills the globals that
// native/data/original_data.cpp lists (tools/original_data.py): byte data is
// copied over its extent, a 32-bit data pointer becomes a native pointer into the
// mapped image (a target in an executable section, or outside the image, stays
// null). No code is ever taken from it.
#include "ra2port/win32_port.hpp"
#include "ra2port/game_builds.hpp"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

namespace {


uint32_t rd32(const unsigned char *p) { return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24; }
uint16_t rd16(const unsigned char *p) { return (uint16_t)(p[0] | p[1] << 8); }

struct Image {
    bool loaded = false, tried = false;
    uint32_t base = 0, size = 0;
    std::vector<unsigned char> mem;
    struct Range { uint32_t start, end, raw_end; bool exec; };
    std::vector<Range> sections;
    uint32_t resource_rva = 0, resource_size = 0;   // data directory 2 (.rsrc)
};

Image &image()
{
    static Image img;
    if (img.tried) return img;
    img.tried = true;
    // The installation's game.exe, or RA2_GAME_EXE: the supported executable
    // kept elsewhere when the installation carries another build.
    char path[1024];
    const char *override_path = getenv("RA2_GAME_EXE");
    if (override_path && *override_path) snprintf(path, sizeof path, "%s", override_path);
    else ra2port::host_path("game.exe", path, sizeof path);
    FILE *f = fopen(path, "rb");
    if (!f) { fprintf(stderr, "ra2_port: game.exe not found in the installation (%s)\n", path); return img; }
    std::vector<unsigned char> file;
    unsigned char buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) file.insert(file.end(), buf, buf + n);
    fclose(f);
    char digest[65];
    ra2build::Sha256 sha;
    sha.update(file.data(), file.size());
    sha.hex(digest);
    // The target, or a build whose data sections are the target's (identity address map;
    // ra2port/game_builds.hpp). Other builds are not read.
    std::string why;
    ra2build::Kind kind = ra2build::classify(file.data(), file.size(), digest, [](const unsigned char *b, size_t len) {
        char h[65];
        ra2build::Sha256 part;
        part.update(b, len);
        part.hex(h);
        return std::string(h);
    }, &why);
    if (kind == ra2build::Kind::other) {
        fprintf(stderr, "ra2_port: %s is not a supported game.exe (size %zu, SHA-256 %s: %s): its stored data is not read; "
                "supported: %s\n", path, file.size(), digest, why.c_str(), ra2build::supported_builds_text());
        return img;
    }
    if (kind == ra2build::Kind::same_data)
        fprintf(stderr, "ra2_port: %s (size %zu, SHA-256 %.16s...) is another build with the target's data: read at the "
                "target's addresses\n", path, file.size(), digest);
    const unsigned char *p = file.data();
    uint32_t pe = rd32(p + 0x3C);
    uint16_t nsec = rd16(p + pe + 6), opt = rd16(p + pe + 20);
    const unsigned char *o = p + pe + 24;
    img.base = rd32(o + 28);
    img.size = rd32(o + 56);
    if (rd32(o + 92) > 2) {                         // NumberOfRvaAndSizes
        img.resource_rva = rd32(o + 96 + 8 * 2);
        img.resource_size = rd32(o + 96 + 8 * 2 + 4);
    }
    img.mem.assign(img.size, 0);
    const unsigned char *s = o + opt;
    for (int i = 0; i < nsec; ++i, s += 40) {
        uint32_t vsize = rd32(s + 8), va = rd32(s + 12), rawsize = rd32(s + 16), raw = rd32(s + 20), flags = rd32(s + 36);
        uint32_t copy = rawsize < vsize || vsize == 0 ? rawsize : vsize;
        if (va + copy > img.size || raw + copy > file.size()) continue;
        memcpy(img.mem.data() + va, p + raw, copy);
        img.sections.push_back({img.base + va, img.base + va + (vsize ? vsize : rawsize), img.base + va + copy, (flags & 0x20000000u) != 0});
    }
    img.loaded = true;
    return img;
}

}  // namespace

struct ra2_original_global { void *storage; uint32_t address; uint32_t extent; uint32_t shape; };
// pointers: the storage holds pointer slots (8 bytes natively for the original's 4).
struct ra2_original_place { uint32_t address; uint32_t extent; void *storage; uint32_t pointers; };

namespace {
const ra2_original_place *g_places = nullptr;
size_t g_place_count = 0;
}

namespace ra2port {

// The native object for the original's address va: the storage of the global
// there (offset kept), else the mapped image's non-executable data (string
// literals, tables; and, since M27, uninitialised data no native global names:
// the image's zeroed block there, so a stored pointer to an unnamed .bss area is
// not null, as in the original: unknown_ramp_corners_7f4174[0] -> 0x00ABCE9C,
// the flat ramp's corner table, faulted the random map's hills), else null
// (code, outside).
void *original_data_pointer(uint32_t va)
{
    Image &img = image();
    if (!img.loaded || va == 0) return nullptr;
    size_t lo = 0, hi = g_place_count;
    while (lo < hi) {                      // last place with address <= va
        size_t mid = (lo + hi) / 2;
        if (g_places[mid].address <= va) lo = mid + 1; else hi = mid;
    }
    if (lo > 0) {
        const ra2_original_place &p = g_places[lo - 1];
        if (va < p.address + p.extent) {
            uint32_t offset = va - p.address;
            if (!p.pointers) return (unsigned char *)p.storage + offset;
            if (offset % 4 == 0) return (unsigned char *)p.storage + 2 * offset;
        }
    }
    for (const Image::Range &r : img.sections)
        if (va >= r.start && va < r.end) return r.exec ? nullptr : img.mem.data() + (va - img.base);
    return nullptr;
}

// A resource the original stores (PE resource directory, data directory 2):
// type and name are integer ids (MAKEINTRESOURCE) or names (case-insensitive,
// as FindResource matches them); the first language. Returns the data in the
// mapped image and its size, or null. Dialog templates (RT_DIALOG 5), string
// and other resources: read at run time, never copied (AGENTS.md "Native phase").
namespace {
bool resource_name_matches(const Image &img, uint32_t entry_name, const char *want)
{
    if (!(entry_name & 0x80000000u)) return false;
    const unsigned char *str = img.mem.data() + img.resource_rva + (entry_name & 0x7FFFFFFFu);
    uint16_t len = rd16(str);
    if (strlen(want) != len) return false;
    for (uint16_t i = 0; i < len; ++i) {
        uint16_t c = rd16(str + 2 + 2 * i);
        int a = c < 128 ? toupper(c) : c, b = toupper((unsigned char)want[i]);
        if (a != b) return false;
    }
    return true;
}
// The entry of directory dir matching key (an id <= 0xFFFF, else a name); its offset field or 0.
uint32_t resource_entry(const Image &img, uint32_t dir, uintptr_t key)
{
    const unsigned char *d = img.mem.data() + img.resource_rva + dir;
    int named = rd16(d + 12), ids = rd16(d + 14);
    const unsigned char *e = d + 16;
    for (int i = 0; i < named + ids; ++i, e += 8) {
        uint32_t name = rd32(e), offset = rd32(e + 4);
        if (key <= 0xFFFF ? (!(name & 0x80000000u) && name == key)
                          : resource_name_matches(img, name, (const char *)key))
            return offset;
    }
    return 0;
}
}  // namespace

const void *original_resource(uintptr_t type, uintptr_t name, size_t *size)
{
    Image &img = image();
    if (!img.loaded || !img.resource_rva || img.resource_rva >= img.size) return nullptr;
    if (name > 0xFFFF && ((const char *)name)[0] == '#') name = (uintptr_t)strtoul((const char *)name + 1, nullptr, 10);
    uint32_t t = resource_entry(img, 0, type);
    if (!(t & 0x80000000u)) return nullptr;
    uint32_t n = resource_entry(img, t & 0x7FFFFFFFu, name);
    if (!(n & 0x80000000u)) return nullptr;
    const unsigned char *langs = img.mem.data() + img.resource_rva + (n & 0x7FFFFFFFu);
    if (rd16(langs + 12) + rd16(langs + 14) == 0) return nullptr;
    uint32_t leaf = rd32(langs + 16 + 4);
    if (leaf & 0x80000000u) return nullptr;
    const unsigned char *entry = img.mem.data() + img.resource_rva + leaf;
    uint32_t rva = rd32(entry), length = rd32(entry + 4);
    if ((uint64_t)rva + length > img.size) return nullptr;
    if (size) *size = length;
    return img.mem.data() + rva;
}

// Copies n bytes the original stores at va; false when unavailable.
bool original_bytes(uint32_t va, void *out, size_t n)
{
    Image &img = image();
    if (!img.loaded || va < img.base || (uint64_t)va - img.base + n > img.size) return false;
    memcpy(out, img.mem.data() + (va - img.base), n);
    return true;
}

// Fills the listed globals (native/data/original_data.cpp); places maps original
// addresses to native storage (sorted). Returns how many were filled.
int load_original_data(const ra2_original_global *table, const ra2_original_place *places)
{
    Image &img = image();
    if (!img.loaded || !table) return 0;
    g_places = places;
    for (g_place_count = 0; places && places[g_place_count].storage; ++g_place_count) {}
    int done = 0, dropped = 0;
    for (const ra2_original_global *g = table; g->storage; ++g) {
        if (g->shape == 0) {
            if (original_bytes(g->address, g->storage, g->extent)) ++done;
            continue;
        }
        if ((g->shape & 0xF) == 3) {
            // Records of 4-byte fields: bits 4-7 the field count, bits 8-22 the
            // pointer fields (8-byte native slots, natural alignment).
            uint32_t n = (g->shape >> 4) & 0xF, mask = g->shape >> 8, offsets[16], off = 0;
            for (uint32_t f = 0; f < n; ++f) {
                if (mask & (1u << f)) off = (off + 7) & ~7u;
                offsets[f] = off;
                off += (mask & (1u << f)) ? 8 : 4;
            }
            uint32_t align = mask ? 8 : 4, size = (off + align - 1) & ~(align - 1);
            for (uint32_t i = 0; n && i < g->extent / (4 * n); ++i) {
                for (uint32_t f = 0; f < n; ++f) {
                    uint32_t word = 0;
                    if (!original_bytes(g->address + 4 * (i * n + f), &word, 4)) break;
                    unsigned char *slot = (unsigned char *)g->storage + i * size + offsets[f];
                    if (mask & (1u << f)) {
                        void *ptr = original_data_pointer(word);
                        if (word && !ptr) ++dropped;
                        memcpy(slot, &ptr, sizeof ptr);
                    } else {
                        memcpy(slot, &word, 4);
                    }
                }
            }
            ++done;
            continue;
        }
        uint32_t count = g->shape == 1 ? 1 : g->extent / 4;
        for (uint32_t i = 0; i < count; ++i) {
            uint32_t va = 0;
            if (!original_bytes(g->address + 4 * i, &va, 4)) break;
            void *ptr = original_data_pointer(va);
            if (va && !ptr) ++dropped;
            ((void **)g->storage)[i] = ptr;
        }
        ++done;
    }
    char detail[96];
    snprintf(detail, sizeof detail, "%d globals from game.exe (%d pointers outside its data left null)", done, dropped);
    progress("original-data", detail);
    return done;
}

}  // namespace ra2port
