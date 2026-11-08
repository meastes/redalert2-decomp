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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

namespace {

const uint64_t kTargetSize = 4485392;
const char kTargetSha256[] = "e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f";

// FIPS 180-4 SHA-256.
struct Sha256 {
    uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    unsigned char block[64];
    size_t used = 0;
    uint64_t bits = 0;
    static uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
    void compress(const unsigned char *p)
    {
        static const uint32_t k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) w[i] = (uint32_t)p[4 * i] << 24 | (uint32_t)p[4 * i + 1] << 16 | (uint32_t)p[4 * i + 2] << 8 | p[4 * i + 3];
        for (int i = 16; i < 64; ++i) {
            uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
        for (int i = 0; i < 64; ++i) {
            uint32_t t1 = hh + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) + ((e & f) ^ (~e & g)) + k[i] + w[i];
            uint32_t t2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
            hh = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }
    void update(const unsigned char *p, size_t n)
    {
        bits += (uint64_t)n * 8;
        while (n) {
            size_t take = 64 - used < n ? 64 - used : n;
            memcpy(block + used, p, take);
            used += take; p += take; n -= take;
            if (used == 64) { compress(block); used = 0; }
        }
    }
    void hex(char out[65])
    {
        uint64_t total = bits;
        unsigned char pad = 0x80;
        update(&pad, 1);
        unsigned char zero = 0;
        while (used != 56) update(&zero, 1);
        unsigned char len[8];
        for (int i = 0; i < 8; ++i) len[i] = (unsigned char)(total >> (56 - 8 * i));
        update(len, 8);
        for (int i = 0; i < 8; ++i) snprintf(out + 8 * i, 9, "%08x", h[i]);
    }
};

uint32_t rd32(const unsigned char *p) { return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24; }
uint16_t rd16(const unsigned char *p) { return (uint16_t)(p[0] | p[1] << 8); }

struct Image {
    bool loaded = false, tried = false;
    uint32_t base = 0, size = 0;
    std::vector<unsigned char> mem;
    struct Range { uint32_t start, end, raw_end; bool exec; };
    std::vector<Range> sections;
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
    Sha256 sha;
    sha.update(file.data(), file.size());
    sha.hex(digest);
    if (file.size() != kTargetSize || strcmp(digest, kTargetSha256) != 0) {
        fprintf(stderr, "ra2_port: %s is not the supported game.exe (size %zu, SHA-256 %s): its stored data is not read\n",
                path, file.size(), digest);
        return img;
    }
    const unsigned char *p = file.data();
    uint32_t pe = rd32(p + 0x3C);
    uint16_t nsec = rd16(p + pe + 6), opt = rd16(p + pe + 20);
    const unsigned char *o = p + pe + 24;
    img.base = rd32(o + 28);
    img.size = rd32(o + 56);
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
struct ra2_original_place { uint32_t address; uint32_t extent; void *storage; };

namespace {
const ra2_original_place *g_places = nullptr;
size_t g_place_count = 0;
}

namespace ra2port {

// The native object for the original's address va: the storage of the global
// there (offset kept), else the mapped image's initialised, non-executable data
// (string literals, tables), else null (code, uninitialised data, outside).
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
        if (va < p.address + p.extent) return (unsigned char *)p.storage + (va - p.address);
    }
    for (const Image::Range &r : img.sections)
        if (va >= r.start && va < r.end) return r.exec || va >= r.raw_end ? nullptr : img.mem.data() + (va - img.base);
    return nullptr;
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
