// Which game.exe builds the native port can read its data from (AGENTS.md "Native phase",
// user decisions 2026-11-08: the port reads data the original stores from the user's installed
// game.exe at run time, from the rebuilt target or from another build, locating the data per
// build). No bytes of any executable are kept here: only the target's section layout and the
// SHA-256 digests of the sections the port reads.
//
// The port reads only the stored data: the globals of native/data/original_data.cpp (in .rdata
// and .data; pointers into code stay null) and the dialog/string resources (.rsrc). It never
// runs or reads code. So a build is readable at the target's addresses (an identity address
// map) when its section table is the target's and its .rdata, .data and .rsrc bytes are
// identical to the target's; its code section may differ.
//
// Known builds:
//   target    4,485,392 B, SHA-256 e6418111c2c36081... (base Red Alert 2 1.006 game.exe; the
//             exact-build acceptance target).
//   1.006 signed/no-CD  4,489,184 B, SHA-256 7bb2c7e1eeb5bb6b... (the same build with an
//             Authenticode signature appended, its header checksum and security directory set,
//             and four short code patches (file offsets 0x70197, 0x91A38, 0x983E0, 0x115439):
//             data, read-only data and resources identical): read through the identity map.
// Any other build (another version, a cracked or repacked executable with moved data) is
// refused, with a message naming the supported builds.
#ifndef RA2PORT_GAME_BUILDS_HPP
#define RA2PORT_GAME_BUILDS_HPP

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <stdio.h>
#include <string>

namespace ra2build {

// FIPS 180-4 SHA-256 (portable; shared by the port and the app).
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


const long long kTargetSize = 4485392;
const char kTargetSha256[] = "e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f";

struct SectionSig {
    const char *name;
    uint32_t va, vsize, raw, rawsize;
    const char *sha256;     // of the raw bytes; null: not read by the port (code)
};
const SectionSig kTargetSections[4] = {
    {".text", 0x1000, 0x3985cd, 0x1000, 0x399000, nullptr},
    {".rdata", 0x39a000, 0x2ec36, 0x39a000, 0x2f000, "d1f7a2a28320f0b075c8188410f331396308ed19c6bdeac8e5f9bd7900c7bd6e"},
    {".data", 0x3c9000, 0x3639c4, 0x3c9000, 0x68000, "0c95ff98b80c378d6eb26c6cad478591046a69e9a14fec15aee72e7a07e6c746"},
    {".rsrc", 0x72d000, 0x15fb8, 0x431000, 0x16000, "2ff34f0c7d65f9d8a896d99920a66064b6f31bd7a28032c521dc3ee356723d85"},
};
const uint32_t kTargetImageBase = 0x400000;

enum class Kind { target, same_data, other };

inline uint32_t rd32(const unsigned char *p) { return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24; }
inline uint16_t rd16(const unsigned char *p) { return (uint16_t)(p[0] | p[1] << 8); }

// file/size: the whole executable; whole_sha256: its digest (hex); hex(p, n): the SHA-256 hex
// digest of a byte range. `why` receives a short reason for `other`.
template <class Hex>
Kind classify(const unsigned char *file, size_t size, std::string const &whole_sha256, Hex hex, std::string *why = nullptr)
{
    if ((long long)size == kTargetSize && whole_sha256 == kTargetSha256) return Kind::target;
    auto fail = [&](std::string const &reason) { if (why) *why = reason; return Kind::other; };
    if (size < 0x400 || file[0] != 'M' || file[1] != 'Z') return fail("not a Windows executable");
    uint32_t pe = rd32(file + 0x3C);
    if ((size_t)pe + 24 > size || memcmp(file + pe, "PE\0\0", 4) != 0) return fail("not a Windows executable");
    uint16_t nsec = rd16(file + pe + 6), opt = rd16(file + pe + 20);
    const unsigned char *o = file + pe + 24;
    if ((size_t)(o - file) + opt + 40u * nsec > size) return fail("truncated header");
    if (rd32(o + 28) != kTargetImageBase || nsec != 4) return fail("a different section layout");
    const unsigned char *s = o + opt;
    for (int i = 0; i < 4; ++i, s += 40) {
        SectionSig const &t = kTargetSections[i];
        char name[9] = {0};
        memcpy(name, s, 8);
        if (strcmp(name, t.name) != 0 || rd32(s + 8) != t.vsize || rd32(s + 12) != t.va || rd32(s + 16) != t.rawsize ||
            rd32(s + 20) != t.raw)
            return fail("a different section layout");
        if (!t.sha256) continue;
        if ((size_t)t.raw + t.rawsize > size) return fail("truncated sections");
        if (hex(file + t.raw, t.rawsize) != t.sha256) return fail(std::string("different data in ") + t.name);
    }
    return Kind::same_data;
}

// For messages: what is supported.
inline const char *supported_builds_text()
{
    return "the base Red Alert 2 1.006 game.exe (4,485,392 bytes, SHA-256 e6418111c2c36081…), or a copy of that "
           "build whose data is unchanged (for example the signed 4,489,184-byte game.exe with no-CD patches, "
           "SHA-256 7bb2c7e1…)";
}

}  // namespace ra2build

#endif
