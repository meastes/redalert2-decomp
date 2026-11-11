// native-asm-check: the behaviour rewrites of the handwritten-assembly
// functions (native/link/behaviour_asm_*.cpp, native/behaviour_rewrites.json)
// run on synthetic inputs and are compared with references built here from
// each algorithm: encoders whose decoding is known, plain per-pixel formulas,
// the textbook IMA ADPCM decoder. The x86 originals do not run natively; no
// value here comes from the original executable. The ADPCM tables are also
// compared with the adopted archive text (src/recovered/asm/*.inc).
#include "shared/vqa_player.hpp"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// The VC6 dialect (-fno-wchar, the forced prelude) does not admit the C++
// standard library headers, so a minimal growable array stands in for vector.
template <class T>
struct Vec {
    T *p = 0;
    size_t n = 0, cap = 0;
    Vec() {}
    explicit Vec(size_t count, T fill = T()) { resize(count, fill); }
    Vec(Vec const &o) { for (size_t i = 0; i < o.n; ++i) push_back(o.p[i]); }
    Vec &operator=(Vec const &o) { if (this != &o) { n = 0; for (size_t i = 0; i < o.n; ++i) push_back(o.p[i]); } return *this; }
    ~Vec() { free(p); }
    void reserve(size_t c) { if (c > cap) { cap = c < 16 ? 16 : c; p = (T *)realloc(p, cap * sizeof(T)); } }
    void push_back(T v) { if (n == cap) reserve(cap * 2 + 16); p[n++] = v; }
    void resize(size_t c, T fill = T()) { reserve(c); while (n < c) p[n++] = fill; n = c; }
    void append(size_t count, T v) { for (size_t i = 0; i < count; ++i) push_back(v); }
    size_t size() const { return n; }
    bool empty() const { return n == 0; }
    T *data() { return p; }
    T const *data() const { return p; }
    T &operator[](size_t i) { return p[i]; }
    T const &operator[](size_t i) const { return p[i]; }
    T *begin() { return p; }
    T *end() { return p + n; }
    bool operator==(Vec const &o) const { return n == o.n && (n == 0 || memcmp(p, o.p, n * sizeof(T)) == 0); }
};

static int failures = 0;
static int checks = 0;
#define CHECK(c) do { ++checks; if (!(c)) { printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// The rewrites under test (C linkage names and native port types).
extern "C" {
void Brighten_Color_555(void *, void *, int, int, int, int);
void Brighten_Color_556(void *, void *, int, int, int, int);
void Brighten_Color_565(void *, void *, int, int, int, int);
void Brighten_Color_655(void *, void *, int, int, int, int);
void MMX_Brighten_Color_555(void *, void *, int, int, int, int, void *);
void MMX_Brighten_Color_556(void *, void *, int, int, int, int, void *);
void MMX_Brighten_Color_565(void *, void *, int, int, int, int, void *);
void MMX_Brighten_Color_655(void *, void *, int, int, int, int, void *);
void Asm_Interpolate(unsigned char *, unsigned char *, int, int, int);
void Asm_Interpolate_Line_Double(unsigned char *, unsigned char *, int, int, int);
void Asm_Interpolate_Line_Interpolate(unsigned char *, unsigned char *, int, int, int);
void ra2asm_sos_tables(int32_t const **diff, uint16_t const **next);
void ra2asm_trans_xlat_forward16(void *dest, void const *source, int len, unsigned short const *xlator);
unsigned char PaletteInterpolationTable[256][256];
}
int LCW_Comp(void const *source, void *dest, int datasize);
unsigned int Get_CPU_Clock(unsigned int &high);
int Get_RDTSC_CPU_Speed();
void RDTSC();
extern unsigned int TSC_Low, TSC_High;
// The port's game clock the virtual TSC reads (port/src/win32_time.cpp; the check
// links the behaviour file without the port): the monotonic clock here.
extern "C" uint64_t ra2port_game_ns(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint64_t)t.tv_sec * 1000000000u + (uint64_t)t.tv_nsec;
}

static uint32_t rng_state = 0x12345678;
static uint32_t rnd() { rng_state = rng_state * 1103515245u + 12345u; return rng_state >> 8; }

// ---------------------------------------------------------------- AudioUnzap
// Reference: an encoder that emits each code kind from a known sample list.
struct ZapStream {
    Vec<uint8_t> code;
    Vec<uint8_t> samples;   // expected output
    int prev = 0x80;
};
static int clamp255(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }
static void zap_raw(ZapStream &z, int n)           // n 1..32 raw samples
{
    z.code.push_back((uint8_t)(0x80 | (n - 1)));
    for (int i = 0; i < n; ++i) { uint8_t v = (uint8_t)rnd(); z.code.push_back(v); z.samples.push_back(v); z.prev = v; }
}
static void zap_delta5(ZapStream &z, int delta)    // -16..15, wraps
{
    z.code.push_back((uint8_t)(0x80 | 0x20 | (delta & 0x1F)));
    z.prev = (z.prev + delta) & 0xFF;
    z.samples.push_back((uint8_t)z.prev);
}
static void zap_zero(ZapStream &z, int n)          // n 1..64
{
    z.code.push_back((uint8_t)(0xC0 | (n - 1)));
    for (int i = 0; i < n; ++i) z.samples.push_back((uint8_t)z.prev);
}
static void zap_4bit(ZapStream &z, int bytes)
{
    static int const t4[16] = {-9, -8, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 8};
    z.code.push_back((uint8_t)(0x40 | (bytes - 1)));
    for (int i = 0; i < bytes; ++i) {
        int lo = rnd() & 15, hi = rnd() & 15;
        z.code.push_back((uint8_t)(lo | hi << 4));
        z.prev = clamp255(z.prev + t4[lo]); z.samples.push_back((uint8_t)z.prev);
        z.prev = clamp255(z.prev + t4[hi]); z.samples.push_back((uint8_t)z.prev);
    }
}
static void zap_2bit(ZapStream &z, int bytes)
{
    static int const t2[4] = {-2, -1, 0, 1};
    z.code.push_back((uint8_t)(bytes - 1));
    for (int i = 0; i < bytes; ++i) {
        int b = rnd() & 0xFF;
        z.code.push_back((uint8_t)b);
        for (int k = 0; k < 4; ++k) { z.prev = clamp255(z.prev + t2[(b >> (2 * k)) & 3]); z.samples.push_back((uint8_t)z.prev); }
    }
}
static void test_audio_unzap()
{
    for (int round = 0; round < 200; ++round) {
        ZapStream z;
        int parts = 1 + rnd() % 12;
        for (int p = 0; p < parts; ++p) {
            switch (rnd() % 5) {
            case 0: zap_raw(z, 1 + rnd() % 32); break;
            case 1: zap_delta5(z, (int)(rnd() % 32) - 16); break;
            case 2: zap_zero(z, 1 + rnd() % 64); break;
            case 3: zap_4bit(z, 1 + rnd() % 64); break;
            case 4: zap_2bit(z, 1 + rnd() % 64); break;
            }
        }
        Vec<uint8_t> out(z.samples.size() + 300, 0xEE);
        long size = (long)z.samples.size();
        RA2_LONG read = AudioUnzap(z.code.data(), out.data(), (RA2_LONG)size);
        CHECK(read == (RA2_LONG)z.code.size());
        CHECK(memcmp(out.data(), z.samples.data(), z.samples.size()) == 0);
    }
    // Clamping at both ends and the 5-bit delta wrapping without clamp.
    {
        uint8_t code[] = {0x80, 0xFE, 0x40, 0xDF, 0x80, 0x01, 0x00, 0x00, 0xA1};
        // raw FE; 4-bit DF: +8 (clamp 255), +5 (255); raw 01; 2-bit 00: four -2 (clamp
        // at 0); delta5 0x21 = +1 -> 01.
        uint8_t out[16];
        memset(out, 0xEE, sizeof(out));
        RA2_LONG read = AudioUnzap(code, out, 9);
        uint8_t expect[] = {0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01};
        CHECK(read == 9);
        CHECK(memcmp(out, expect, 9) == 0);
        uint8_t wrap[] = {0x80, 0xFF, 0xA1};   // raw FF then +1 wraps to 00 (no clamp)
        AudioUnzap(wrap, out, 2);
        CHECK(out[0] == 0xFF && out[1] == 0x00);
    }
    CHECK(AudioUnzap(0, (void *)1, 4) == 0);
    CHECK(AudioUnzap((void *)1, 0, 4) == 0);
}

// --------------------------------------------------------------------- LCW
// Reference decoder for absolute LCW, written from the format.
static Vec<uint8_t> lcw_reference_decode(uint8_t const *s)
{
    Vec<uint8_t> d;
    for (;;) {
        uint8_t c = *s++;
        if (!(c & 0x80)) {
            int n = (c >> 4) + 3, off = ((c & 15) << 8) | *s++;
            size_t from = d.size() - off;
            for (int i = 0; i < n; ++i) d.push_back(d[from + i]);
        } else if (!(c & 0x40)) {
            if (c == 0x80) break;
            for (int i = 0; i < (c & 0x3F); ++i) d.push_back(*s++);
        } else if (c == 0xFE) {
            int n = s[0] | s[1] << 8; uint8_t v = s[2]; s += 3;
            d.append(n, v);
        } else {
            int n = (c & 0x3F) + 3;
            if (c == 0xFF) { n = s[0] | s[1] << 8; s += 2; }
            int off = s[0] | s[1] << 8; s += 2;
            for (int i = 0; i < n; ++i) d.push_back(d[off + i]);
        }
    }
    return d;
}
static void test_lcw()
{
    // Hand vectors traced through the register program.
    {
        uint8_t src[] = {'A', 'B', 'C'};
        uint8_t out[16];
        int n = LCW_Comp(src, out, 3);
        uint8_t expect[] = {0x83, 'A', 'B', 'C', 0x80};
        CHECK(n == 5 && memcmp(out, expect, 5) == 0);
    }
    {
        Vec<uint8_t> src(100, 'x');
        uint8_t out[64];
        int n = LCW_Comp(src.data(), out, 100);
        // 81 'x', fill FE 98 'x' (the run to the end counts one short), literal 'x', end.
        uint8_t expect[] = {0x81, 'x', 0xFE, 98, 0, 'x', 0x81, 'x', 0x80};
        CHECK(n == 9 && memcmp(out, expect, 9) == 0);
    }
    {
        // "abcdabcdabcd": literal abcd, then a short run of 8 from distance 4.
        char const *src = "abcdabcdabcd";
        uint8_t out[32];
        int n = LCW_Comp(src, out, 12);
        uint8_t expect[] = {0x84, 'a', 'b', 'c', 'd', 0x50, 0x04, 0x80};
        CHECK(n == 8 && memcmp(out, expect, 8) == 0);
    }
    // Round trips: random data with repeats, long matches, runs, far matches.
    for (int round = 0; round < 60; ++round) {
        int size = 1 + rnd() % 6000;
        Vec<uint8_t> src(size);
        int alphabet = 1 + rnd() % 255;
        for (int i = 0; i < size; ++i) {
            int mode = rnd() % 10;
            if (mode < 3 && i > 10) { int back = 1 + rnd() % i; src[i] = src[i - back]; }
            else if (mode < 5 && i > 0) src[i] = src[i - 1];
            else src[i] = (uint8_t)(rnd() % alphabet);
        }
        if (round % 7 == 0 && size > 300) memset(&src[size / 3], 7, 200);
        Vec<uint8_t> comp(size * 2 + 64);
        int n = LCW_Comp(src.data(), comp.data(), size);
        CHECK(n > 0 && n <= size + size / 63 + 4);
        CHECK(lcw_reference_decode(comp.data()) == src);
        Vec<uint8_t> back(size + 16, 0xEE);
        RA2_ULONG got = VQA_LCW_Uncompress((char const *)comp.data(), (char *)back.data(), size);
        CHECK(got == (RA2_ULONG)size && memcmp(back.data(), src.data(), size) == 0);
        // Output clamp: a shorter length stops exactly there.
        if (size > 10) {
            Vec<uint8_t> part(size, 0xEE);
            RA2_ULONG cut = size / 2;
            got = VQA_LCW_Uncompress((char const *)comp.data(), (char *)part.data(), cut);
            CHECK(got == cut && memcmp(part.data(), src.data(), cut) == 0 && part[cut] == 0xEE);
        }
    }
    // Every command of the decoder, absolute and relative.
    {
        uint8_t abs_stream[] = {0x83, 1, 2, 3,          // literal 1 2 3
                                0x10, 0x03,             // short: 4 bytes from 3 back -> 1 2 3 1
                                0xC1, 0x01, 0x00,       // med run: 4 bytes from start+1 -> 2 3 1 2
                                0xFE, 0x05, 0x00, 9,    // fill 5 x 9
                                0xFF, 0x06, 0x00, 0x00, 0x00,  // long: 6 bytes from start -> 1 2 3 1 2 3
                                0x80};
        uint8_t expect[] = {1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 9, 9, 9, 9, 9, 1, 2, 3, 1, 2, 3};
        uint8_t out[32];
        RA2_ULONG got = VQA_LCW_Uncompress((char const *)abs_stream, (char *)out, 32);
        CHECK(got == sizeof(expect) && memcmp(out, expect, sizeof(expect)) == 0);
        uint8_t rel_stream[] = {0x00, 0x83, 1, 2, 3,
                                0xC1, 0x02, 0x00,       // med run: 4 bytes from 2 back -> 2 3 2 3
                                0xFF, 0x05, 0x00, 0x07, 0x00,  // long: 5 bytes from 7 back
                                0x80};
        uint8_t rexpect[] = {1, 2, 3, 2, 3, 2, 3, 1, 2, 3, 2, 3};
        got = VQA_LCW_Uncompress((char const *)rel_stream, (char *)out, 32);
        CHECK(got == sizeof(rexpect) && memcmp(out, rexpect, sizeof(rexpect)) == 0);
        CHECK(VQA_LCW_Uncompress((char const *)abs_stream, (char *)out, 0) == 0);
    }
}

// ------------------------------------------------------------- SOS ADPCM
static int const ima_steps[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97,
    107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428,
    4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350,
    22385, 24623, 27086, 29794, 32767};
struct ImaState { int pred = 0, index = 0; };
static int16_t ima_decode(ImaState &s, int nyb)
{
    static int const adj[16] = {-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};
    int step = ima_steps[s.index];
    int diff = step >> 3;
    if (nyb & 4) diff += step;
    if (nyb & 2) diff += step >> 1;
    if (nyb & 1) diff += step >> 2;
    s.pred += (nyb & 8) ? -diff : diff;
    if (s.pred > 32767) s.pred = 32767;
    if (s.pred < -32768) s.pred = -32768;
    s.index += adj[nyb];
    if (s.index < 0) s.index = 0;
    if (s.index > 88) s.index = 88;
    return (int16_t)s.pred;
}
static Vec<char *> read_lines(char const *name)
{
    Vec<char *> lines;
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", RA2_ASM_DIR, name);
    FILE *f = fopen(path, "r");
    if (!f) return lines;
    char buf[512];
    while (fgets(buf, sizeof(buf), f)) lines.push_back(strdup(buf));
    fclose(f);
    return lines;
}
static void test_sos_tables()
{
    int32_t const *diff;
    uint16_t const *next;
    ra2asm_sos_tables(&diff, &next);
    // difftb.inc: "DD <value> ; Index = i Token = t"; indextb.inc: "DW <n> * 2 ; ...".
    Vec<char *> dl = read_lines("difftb.inc"), il = read_lines("indextb.inc");
    CHECK(!dl.empty() && !il.empty());
    int nd = 0, ni = 0;
    for (char *l : dl) {
        char const *p = strstr(l, "DD");
        if (l[0] == ';' || p == 0) continue;
        int v = atoi(p + 2);
        if (nd < 89 * 16) CHECK(diff[nd] == v);
        ++nd;
    }
    for (char *l : il) {
        char const *p = strstr(l, "DW");
        if (l[0] == ';' || p == 0) continue;
        int v = atoi(p + 2) * 2;
        if (ni < 89 * 16) CHECK(next[ni] == v);
        ++ni;
    }
    CHECK(nd == 89 * 16 && ni == 89 * 16);
}
static void test_sos()
{
    test_sos_tables();
    VQASOS info;
    memset(&info, 0x5A, sizeof(info));
    VQA_sosCODECInitStream(&info);
    CHECK(info.Predicted == 0 && info.Index == 0 && info.Predicted2 == 0 && info.Index2 == 0);
    // Mono: every size 0..80 bytes of output, every source alignment.
    for (int bytes = 0; bytes <= 80; ++bytes) {
        for (int align = 0; align < 4; ++align) {
            uint8_t srcbuf[64];
            for (int i = 0; i < 64; ++i) srcbuf[i] = (uint8_t)rnd();
            uint8_t *src = srcbuf + align;
            Vec<uint8_t> out(bytes + 8, 0xEE);
            VQA_sosCODECInitStream(&info);
            VQA_sosCODECDecompressData(src, out.data(), 16, 1, bytes, &info);
            ImaState ref;
            bool ok = true;
            for (int i = 0; i < bytes / 2; ++i) {
                int16_t s = ima_decode(ref, (src[i >> 1] >> ((i & 1) * 4)) & 15);
                int16_t got;
                memcpy(&got, &out[2 * i], 2);
                ok = ok && got == s;
            }
            CHECK(ok);
            CHECK(out[(bytes / 2) * 2] == 0xEE);
            CHECK(info.Predicted == ref.pred && info.Index == ref.index * 32);
        }
    }
    // Large signal hitting both clamps; continuity across calls.
    {
        Vec<uint8_t> src(4096);
        for (size_t i = 0; i < src.size(); ++i) src[i] = (uint8_t)((i / 64) % 2 ? 0x77 : 0xFF);
        Vec<uint8_t> a(16384), b(16384);
        VQA_sosCODECInitStream(&info);
        VQA_sosCODECDecompressData(src.data(), a.data(), 16, 1, 16384, &info);
        VQA_sosCODECInitStream(&info);
        VQA_sosCODECDecompressData(src.data(), b.data(), 16, 1, 6000, &info);
        VQA_sosCODECDecompressData(src.data() + 1500, b.data() + 6000, 16, 1, 16384 - 6000, &info);
        CHECK(a == b);
        ImaState ref;
        bool ok = true, hit_hi = false, hit_lo = false;
        for (int i = 0; i < 8192; ++i) {
            int16_t s = ima_decode(ref, (src[i >> 1] >> ((i & 1) * 4)) & 15);
            int16_t got;
            memcpy(&got, &a[2 * i], 2);
            ok = ok && got == s;
            hit_hi |= s == 32767; hit_lo |= s == -32768;
        }
        CHECK(ok && hit_hi && hit_lo);
    }
    // Stereo: left from the first size/8 bytes into even samples, right from the next into odd ones.
    for (int bytes = 0; bytes <= 96; bytes += 4) {
        uint8_t src[64];
        for (int i = 0; i < 64; ++i) src[i] = (uint8_t)rnd();
        Vec<uint8_t> out(bytes + 8, 0xEE);
        VQA_sosCODECInitStream(&info);
        info.Predicted2 = 100; info.Index2 = 10 * 32;
        VQA_sosCODECDecompressData(src, out.data(), 16, 2, bytes, &info);
        ImaState l, r;
        r.pred = 100; r.index = 10;
        bool ok = true;
        for (int i = 0; i < bytes / 4; ++i) {
            int16_t sl = ima_decode(l, (src[i >> 1] >> ((i & 1) * 4)) & 15);
            int16_t sr = ima_decode(r, (src[bytes / 8 + (i >> 1)] >> ((i & 1) * 4)) & 15);
            int16_t gl, gr;
            memcpy(&gl, &out[4 * i], 2);
            memcpy(&gr, &out[4 * i + 2], 2);
            ok = ok && gl == sl && gr == sr;
        }
        CHECK(ok);
        CHECK(info.Predicted == l.pred && info.Index == l.index * 32);
        CHECK(info.Predicted2 == r.pred && info.Index2 == r.index * 32);
    }
    // Unsupported formats leave the output alone.
    {
        uint8_t src[8] = {1, 2, 3, 4, 5, 6, 7, 8}, out[8];
        memset(out, 0xEE, 8);
        VQA_sosCODECDecompressData(src, out, 8, 1, 8, &info);
        VQA_sosCODECDecompressData(src, out, 16, 3, 8, &info);
        bool same = true;
        for (int i = 0; i < 8; ++i) same = same && out[i] == 0xEE;
        CHECK(same);
    }
}

// -------------------------------------------------------------------- UnVQ
static void test_unvq()
{
    int const bpr = 5, rows = 3, entries = bpr * rows;
    Vec<uint8_t> codebook(300 * 32);
    for (size_t i = 0; i < codebook.size(); ++i) codebook[i] = (uint8_t)rnd();
    Vec<uint8_t> ptrs(2 * entries);
    Vec<int> value(entries);
    for (int i = 0; i < entries; ++i) {
        bool one = (rnd() % 3) == 0;
        int v = one ? (0xFF00 | (rnd() & 0xFF)) : (int)(rnd() % 300);
        value[i] = v;
        ptrs[i] = (uint8_t)v;
        ptrs[entries + i] = (uint8_t)(v >> 8);
    }
    // C0 4x2 and 4x4 into a buffer wider than the image (pitch 40).
    for (int h = 2; h <= 4; h += 2) {
        int pitch = 40;
        Vec<uint8_t> out(pitch * rows * h + 64, 0xEE);
        if (h == 2) ASM_UnVQ_4x2(codebook.data(), ptrs.data(), out.data(), bpr, rows, pitch);
        else ASM_UnVQ_4x4(codebook.data(), ptrs.data(), out.data(), bpr, rows, pitch);
        bool ok = true;
        for (int y = 0; y < rows * h; ++y) {
            for (int x = 0; x < pitch; ++x) {
                uint8_t got = out[y * pitch + x];
                if (x >= bpr * 4) { ok = ok && got == 0xEE; continue; }
                int v = value[(y / h) * bpr + x / 4];
                uint8_t expect = (v >> 8) == 0xFF ? (uint8_t)v : codebook[v * 4 * h + (y % h) * 4 + x % 4];
                ok = ok && got == expect;
            }
        }
        CHECK(ok);
    }
    // C0 4x4 half: pixel (x, y) of each block's 2x2 is codeword byte 8*y + 2*x.
    {
        int pitch = 16;
        Vec<uint8_t> out(pitch * rows * 2 + 64, 0xEE);
        ASM_UnVQ_4x4_HALF(codebook.data(), ptrs.data(), out.data(), bpr, rows, pitch);
        bool ok = true;
        for (int y = 0; y < rows * 2; ++y) {
            for (int x = 0; x < pitch; ++x) {
                uint8_t got = out[y * pitch + x];
                if (x >= bpr * 2) { ok = ok && got == 0xEE; continue; }
                int v = value[(y / 2) * bpr + x / 2];
                uint8_t expect = (v >> 8) == 0xFF ? (uint8_t)v : codebook[v * 16 + (y % 2) * 8 + (x % 2) * 2];
                ok = ok && got == expect;
            }
        }
        CHECK(ok);
    }
    // C1 4x4: 16-bit pixels, one-colour when bit 15 of the pointer is set (value & 0x7FFF).
    {
        Vec<uint8_t> p2(2 * entries);
        Vec<int> v2(entries);
        for (int i = 0; i < entries; ++i) {
            int v = (rnd() % 2) ? (int)(0x8000 | (rnd() & 0x7FFF)) : (int)(rnd() % 300);
            v2[i] = v; p2[i] = (uint8_t)v; p2[entries + i] = (uint8_t)(v >> 8);
        }
        int pitch = 24;   // pixels
        Vec<uint16_t> out(pitch * rows * 4 + 32, 0xEEEE);
        ASM_UnVQ1_C1_4x4(codebook.data(), p2.data(), (unsigned char *)out.data(), bpr, rows, pitch);
        bool ok = true;
        for (int y = 0; y < rows * 4; ++y) {
            for (int x = 0; x < pitch; ++x) {
                uint16_t got = out[y * pitch + x];
                if (x >= bpr * 4) { ok = ok && got == 0xEEEE; continue; }
                int v = v2[(y / 4) * bpr + x / 4];
                uint16_t expect;
                if (v & 0x8000) expect = (uint16_t)(v & 0x7FFF);
                else memcpy(&expect, &codebook[v * 32 + (y % 4) * 8 + (x % 4) * 2], 2);
                ok = ok && got == expect;
            }
        }
        CHECK(ok);
    }
}

// --------------------------------------------------------------- Brighten
struct Fmt { int rpos, rbits, gpos, gbits, bpos, bbits; };
static int scale(int c8, int s) { int v = c8 + (c8 * s >> 8); return v > 255 ? 255 : v; }
static uint16_t ref_brighten(Fmt f, uint16_t p, int s)
{
    int r = (p >> f.rpos) & ((1 << f.rbits) - 1), g = (p >> f.gpos) & ((1 << f.gbits) - 1),
        b = (p >> f.bpos) & ((1 << f.bbits) - 1);
    r = scale(r << (8 - f.rbits), s) >> (8 - f.rbits);
    g = scale(g << (8 - f.gbits), s) >> (8 - f.gbits);
    b = scale(b << (8 - f.bbits), s) >> (8 - f.bbits);
    return (uint16_t)(r << f.rpos | g << f.gpos | b << f.bpos);
}
typedef void (*BrightenFn)(void *, void *, int, int, int, int);
typedef void (*MMXFn)(void *, void *, int, int, int, int, void *);
static void test_brighten()
{
    struct { BrightenFn fn; Fmt f; } const cases[] = {
        {Brighten_Color_565, {11, 5, 5, 6, 0, 5}}, {Brighten_Color_555, {10, 5, 5, 5, 0, 5}},
        {Brighten_Color_655, {10, 6, 5, 5, 0, 5}}, {Brighten_Color_556, {11, 5, 6, 5, 0, 6}}};
    int const w = 37, h = 9, spitch = 256, dpitch = 2 * 50;
    Vec<uint8_t> mul(spitch * h);
    for (auto &m : mul) m = (rnd() % 4 == 0) ? 0 : (uint8_t)rnd();
    for (auto const &c : cases) {
        Vec<uint16_t> pix(50 * h), orig;
        for (auto &p : pix) p = (uint16_t)rnd();
        orig = pix;
        c.fn(mul.data(), pix.data(), spitch, dpitch, w, h);
        bool ok = true;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < 50; ++x) {
                uint16_t o = orig[y * 50 + x];
                int s = mul[y * spitch + x];
                uint16_t expect = (x >= w || s == 0) ? o : ref_brighten(c.f, o, s);
                ok = ok && pix[y * 50 + x] == expect;
            }
        CHECK(ok);
    }
    // MMX: the table holds R << 16 | G << 8 | B (8-bit components) per pixel, as
    // SpotLight builds it; components saturate the same way, then pack per format.
    Vec<uint32_t> table(65536);
    for (int p = 0; p < 65536; ++p) {
        int r = (p >> 11) << 3, g = ((p >> 5) & 63) << 2, b = (p & 31) << 3;
        table[p] = (uint32_t)(r << 16 | g << 8 | b);
    }
    struct { MMXFn fn; int kind; } const mcases[] = {
        {MMX_Brighten_Color_565, 0}, {MMX_Brighten_Color_555, 1}, {MMX_Brighten_Color_556, 2},
        {MMX_Brighten_Color_655, 3}};
    for (auto const &c : mcases) {
        Vec<uint16_t> pix(50 * h), orig;
        for (auto &p : pix) p = (uint16_t)rnd();
        orig = pix;
        c.fn(mul.data(), pix.data(), spitch, dpitch, w, h, table.data());
        bool ok = true;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < 50; ++x) {
                uint16_t o = orig[y * 50 + x];
                int s = mul[y * spitch + x];
                uint16_t expect = o;
                if (x < w && s != 0) {
                    uint32_t e = table[o];
                    int R = scale((e >> 16) & 255, s), G = scale((e >> 8) & 255, s), B = scale(e & 255, s);
                    switch (c.kind) {
                    case 0: expect = (uint16_t)((B >> 3) | (G >> 2) << 5 | (R >> 3) << 11); break;
                    case 1: expect = (uint16_t)((B >> 3) | (G >> 3) << 5 | (R >> 3) << 10); break;
                    // 556 as the archive procedure packs it: G keeps six bits at bit 6
                    // (its top bit lands on R's lowest), B six bits, R five at 11.
                    case 2: expect = (uint16_t)((B >> 2) | (G >> 2) << 6 | (((R >> 2) << 10) & 0xF800)); break;
                    // 655 as the archive procedure packs it: G << 4 masked by 0x423A0A60.
                    case 3: expect = (uint16_t)((B >> 3) | (((G >> 2) << 4) & 0x423A0A60) | (R >> 2) << 10); break;
                    }
                }
                ok = ok && pix[y * 50 + x] == expect;
            }
        CHECK(ok);
    }
}

// ------------------------------------------------------------ Interpolate
static uint8_t T(int a, int b) { return PaletteInterpolationTable[b][a]; }
static void ref_line(uint8_t const *s, int w, uint8_t *d)
{
    for (int i = 0; i < w; i += 2) {
        d[2 * i] = s[i];
        d[2 * i + 1] = T(s[i], s[i + 1]);
        d[2 * i + 2] = s[i + 1];
        d[2 * i + 3] = (i + 2 < w) ? T(s[i + 1], s[i + 2]) : 0;
    }
}
static void test_interpolate()
{
    for (int a = 0; a < 256; ++a)
        for (int b = 0; b < 256; ++b) PaletteInterpolationTable[a][b] = (uint8_t)(a * 7 + b * 13 + 1);
    int const w = 20, lines = 6, pitch = 2 * w + 6, dw = 2 * pitch;
    Vec<uint8_t> src(w * (lines + 1));
    for (auto &s : src) s = (uint8_t)rnd();
    Vec<uint8_t> line(2 * w), line2(2 * w);
    // Asm_Interpolate: line y at row 2y; odd rows untouched.
    {
        Vec<uint8_t> out(pitch * 2 * lines + 64, 0xEE);
        Asm_Interpolate(src.data(), out.data(), lines, w, dw);
        bool ok = true;
        for (int y = 0; y < 2 * lines; ++y) {
            if (y & 1) { for (int x = 0; x < pitch; ++x) ok = ok && out[y * pitch + x] == 0xEE; continue; }
            ref_line(&src[(y / 2) * w], w, line.data());
            ok = ok && memcmp(&out[y * pitch], line.data(), 2 * w) == 0 && out[y * pitch + 2 * w] == 0xEE;
        }
        CHECK(ok);
    }
    // Line_Double: each line twice.
    {
        Vec<uint8_t> out(pitch * 2 * lines + 64, 0xEE);
        Asm_Interpolate_Line_Double(src.data(), out.data(), lines, w, dw);
        bool ok = true;
        for (int y = 0; y < 2 * lines; ++y) {
            ref_line(&src[(y / 2) * w], w, line.data());
            ok = ok && memcmp(&out[y * pitch], line.data(), 2 * w) == 0 && out[y * pitch + 2 * w] == 0xEE;
        }
        CHECK(ok);
    }
    // Line_Interpolate: line k at row 2k, blend(line k, line k+1) at row 2k+1 for k < lines-1;
    // the last row is the interpolation of the source line after the last (line `lines`).
    {
        Vec<uint8_t> out(pitch * 2 * lines + 64, 0xEE);
        Asm_Interpolate_Line_Interpolate(src.data(), out.data(), lines, w, dw);
        bool ok = true;
        for (int k = 0; k < lines - 1; ++k) {
            ref_line(&src[k * w], w, line.data());
            ref_line(&src[(k + 1) * w], w, line2.data());
            ok = ok && memcmp(&out[2 * k * pitch], line.data(), 2 * w) == 0;
            for (int i = 0; i < 2 * w; ++i) ok = ok && out[(2 * k + 1) * pitch + i] == T(line[i], line2[i]);
        }
        ref_line(&src[lines * w], w, line.data());
        ok = ok && memcmp(&out[(2 * lines - 2) * pitch], line.data(), 2 * w) == 0;
        ok = ok && out[(2 * lines - 1) * pitch] == 0xEE;
        CHECK(ok);
    }
}

// ------------------------------------------------- BlitForward, the clock
static void test_blit_and_clock()
{
    unsigned short pal[256];
    for (int i = 0; i < 256; ++i) pal[i] = (unsigned short)(i * 257 ^ 0x1234);
    uint8_t src[50];
    unsigned short dst[52], orig[52];
    for (int i = 0; i < 50; ++i) src[i] = (i % 3 == 0) ? 0 : (uint8_t)rnd();
    for (int i = 0; i < 52; ++i) dst[i] = orig[i] = (unsigned short)rnd();
    ra2asm_trans_xlat_forward16(dst, src, 50, pal);
    bool ok = true;
    for (int i = 0; i < 52; ++i) ok = ok && dst[i] == ((i < 50 && src[i]) ? pal[src[i]] : orig[i]);
    CHECK(ok);
    ra2asm_trans_xlat_forward16(dst, src, 0, pal);

    CHECK(Get_RDTSC_CPU_Speed() == 1000);
    unsigned int hi0, hi1;
    unsigned int lo0 = Get_CPU_Clock(hi0);
    usleep(20000);
    unsigned int lo1 = Get_CPU_Clock(hi1);
    uint64_t t0 = (uint64_t)hi0 << 32 | lo0, t1 = (uint64_t)hi1 << 32 | lo1;
    // 20 ms at Get_RDTSC_CPU_Speed() MHz: at least 20,000,000 ticks, and not wildly more.
    CHECK(t1 - t0 >= 20000000ull && t1 - t0 < 2000000000ull);
    RDTSC();
    uint64_t t2 = (uint64_t)TSC_High << 32 | TSC_Low;
    CHECK(t2 >= t1);
}

int main()
{
    test_audio_unzap();
    test_lcw();
    test_sos();
    test_unvq();
    test_brighten();
    test_interpolate();
    test_blit_and_clock();
    printf("native-asm-check: %d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
