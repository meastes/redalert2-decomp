// Bink 1 video for the native binkw32 (Scout1153, native M19). NOT EXACT: the
// SDK's decoder is a third-party library, not part of the original program;
// this is a clean-room decoder written from the format's description (no
// third-party code). Called by port/src/bink.cpp (BinkDoFrame decodes,
// BinkCopyToBuffer converts to the destination surface).
//
// Format (revision 'i', what every RA2 movie uses): a frame's video data
// (after its audio packets) starts with a 32-bit word, then the planes Y, V
// (Cr), U (Cb), each 32-bit aligned at its end. A plane is coded in 8x8 blocks
// (chroma planes at half size); per plane the nine value streams ("bundles":
// block types, sub-block types, colours, patterns, x and y motion, intra and
// inter DC, runs) first read their Huffman tree choice, then per block row
// each bundle refills when its decoded values are used up (a count of
// ceil(log2(...)) bits, 0 ends the bundle for the plane). Block types: 0 skip
// (copy from the previous frame), 1 scaled (an 8x8 sub-block of type run,
// intra, fill, pattern or raw doubled to 16x16), 2 motion (copy at an offset),
// 3 run (pattern-ordered runs of colours), 4 residue (motion plus bit-plane
// coded corrections), 5 intra (DCT), 6 fill, 7 inter (motion plus DCT), 8
// pattern (two colours, one bit per pixel), 9 raw (64 colours).
//
// Fixed tables: the DCT scan order, the run lengths of block types and the
// Huffman code tables of trees 0..9 are written below from the format
// description; each was compared privately with the tables in the user's
// installed BINKW32.DLL (all equal; no bytes copied). The code lengths are not
// a table: they follow from the codes (the shortest lengths that make each
// tree a complete prefix code). Trees 10..15, the 16 pattern scan orders and
// the intra/inter quantizer matrices could not be written from the
// description: they are read at run time from the user's installed BINKW32.DLL
// (located by the verified tables next to them), like the game data the port
// reads from game.exe (user ruling 2026-11-09, AGENTS.md "Native phase": the
// port may read them from the installed BINKW32.DLL; Scout1161, M20 made it
// the default). Without the DLL, or when its tables are not found, one line on
// stderr says so and movies play their sound over a black picture.
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

namespace ra2port {
const char *data_dir();
void trace(const char *format, ...);
}

namespace {

// --- Tables -----------------------------------------------------------------

// Zig-zag of 2x2 quads: the order DCT coefficients are coded in.
const unsigned char dct_order[64] = {
    0, 1, 8, 9, 2, 3, 10, 11, 4, 5, 12, 13, 6, 7, 14, 15,
    20, 21, 28, 29, 22, 23, 30, 31, 16, 17, 24, 25, 32, 33, 40, 41,
    34, 35, 42, 43, 48, 49, 56, 57, 50, 51, 58, 59, 18, 19, 26, 27,
    36, 37, 44, 45, 38, 39, 46, 47, 52, 53, 60, 61, 54, 55, 62, 63,
};
// Block type codes 12..15 repeat the last type this many times.
const int type_repeat[4] = {4, 8, 12, 32};

// Codes (read least significant bit first) of the sixteen symbols of trees 0..9.
const unsigned char known_codes[10][16] = {
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
    {0x00, 0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F, 0x13, 0x15, 0x17, 0x19, 0x1B, 0x1D, 0x1F},
    {0x00, 0x02, 0x01, 0x09, 0x05, 0x15, 0x0D, 0x1D, 0x03, 0x13, 0x0B, 0x1B, 0x07, 0x17, 0x0F, 0x1F},
    {0x00, 0x02, 0x06, 0x01, 0x09, 0x05, 0x0D, 0x1D, 0x03, 0x13, 0x0B, 0x1B, 0x07, 0x17, 0x0F, 0x1F},
    {0x00, 0x04, 0x02, 0x06, 0x01, 0x09, 0x05, 0x0D, 0x03, 0x13, 0x0B, 0x1B, 0x07, 0x17, 0x0F, 0x1F},
    {0x00, 0x04, 0x02, 0x0A, 0x06, 0x0E, 0x01, 0x09, 0x05, 0x0D, 0x03, 0x0B, 0x07, 0x17, 0x0F, 0x1F},
    {0x00, 0x02, 0x0A, 0x06, 0x0E, 0x01, 0x09, 0x05, 0x0D, 0x03, 0x0B, 0x1B, 0x07, 0x17, 0x0F, 0x1F},
    {0x00, 0x01, 0x05, 0x03, 0x13, 0x0B, 0x1B, 0x3B, 0x07, 0x27, 0x17, 0x37, 0x0F, 0x2F, 0x1F, 0x3F},
    {0x00, 0x01, 0x03, 0x13, 0x0B, 0x2B, 0x1B, 0x3B, 0x07, 0x27, 0x17, 0x37, 0x0F, 0x2F, 0x1F, 0x3F},
    {0x00, 0x01, 0x05, 0x0D, 0x03, 0x13, 0x0B, 0x1B, 0x07, 0x27, 0x17, 0x37, 0x0F, 0x2F, 0x1F, 0x3F},
};

struct Tables {
    bool ready = false;
    unsigned char codes[16][16];
    unsigned char lengths[16][16];
    unsigned char patterns[16][64];
    int intra[16][64];     // natural coefficient order, 11 fraction bits
    int inter[16][64];
};
Tables tables;

// The shortest lengths that make a tree's codes a complete prefix code.
bool derive_lengths(unsigned char const codes[16], unsigned char lengths[16])
{
    double kraft = 0;
    for (int i = 0; i < 16; ++i) {
        int common = 0;
        for (int k = 0; k < 16; ++k) {
            if (k == i) continue;
            int n = 0;
            while (n < 16 && ((codes[i] >> n) & 1) == ((codes[k] >> n) & 1)) ++n;
            if (n > common) common = n;
        }
        int length = common + 1, bits = 0;
        while (codes[i] >> bits) ++bits;
        if (bits > length) length = bits;
        lengths[i] = (unsigned char)length;
        kraft += ldexp(1.0, -length);
    }
    return fabs(kraft - 1.0) < 1e-9;
}

bool is_permutation64(unsigned char const *p)
{
    bool seen[64] = {};
    for (int i = 0; i < 64; ++i) {
        if (p[i] > 63 || seen[p[i]]) return false;
        seen[p[i]] = true;
    }
    return true;
}

long find(std::vector<unsigned char> const &data, void const *what, size_t size)
{
    if (data.size() < size) return -1;
    for (size_t i = 0; i + size <= data.size(); ++i)
        if (memcmp(&data[i], what, size) == 0) return (long)i;
    return -1;
}

// Reads the tables the description does not give from the installed
// BINKW32.DLL in the data directory. Every read is checked: the codes of
// trees 10..15 must form complete prefix codes, every pattern must be a
// permutation of 0..63, the quantizers must start at 1.0 (0x10000 >> 5... the
// DC step) as trees 0..9 and the scan order next to them do.
bool load_tables()
{
    static int state = 0;   // 0 not tried, 1 ready, 2 unavailable
    if (state) return state == 1;
    state = 2;
    const char *dir = ra2port::data_dir();
    if (!dir || !*dir) { fprintf(stderr, "ra2_port: Bink video: no data directory: movies play without video\n"); return false; }
    std::string path = std::string(dir) + "/BINKW32.DLL";
    FILE *f = fopen(path.c_str(), "rb");
    if (!f) {
        path = std::string(dir) + "/binkw32.dll";
        f = fopen(path.c_str(), "rb");
    }
    if (!f) {
        fprintf(stderr, "ra2_port: Bink video: no BINKW32.DLL in %s: movies play without video\n", dir);
        return false;
    }
    std::vector<unsigned char> dll;
    unsigned char buffer[65536];
    size_t n;
    while ((n = fread(buffer, 1, sizeof buffer, f)) > 0) dll.insert(dll.end(), buffer, buffer + n);
    fclose(f);
    long codes_at = find(dll, known_codes, sizeof known_codes);
    long scan_at = find(dll, dct_order, sizeof dct_order);
    unsigned char first_pattern[16] = {0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
                                       0x39, 0x31, 0x29, 0x21, 0x19, 0x11, 0x09, 0x01};
    long patterns_at = find(dll, first_pattern, sizeof first_pattern);
    int const quant_head[2] = {0x10000, 0x16315};
    long intra_at = find(dll, quant_head, sizeof quant_head);
    if (codes_at < 0 || scan_at < 0 || patterns_at < 0 || intra_at < 0
        || (size_t)codes_at + 256 > dll.size() || (size_t)patterns_at + 1024 > dll.size()
        || (size_t)intra_at + 3 * 4096 > dll.size()) {
        fprintf(stderr, "ra2_port: Bink video: tables not found in %s: movies play without video\n", path.c_str());
        return false;
    }
    memcpy(tables.codes, &dll[codes_at], 256);
    memcpy(tables.patterns, &dll[patterns_at], 1024);
    memcpy(tables.intra, &dll[intra_at], 4096);
    memcpy(tables.inter, &dll[intra_at + 2 * 4096], 4096);   // a 16x64 table of another kind lies between
    for (int t = 0; t < 16; ++t) {
        if (!derive_lengths(tables.codes[t], tables.lengths[t])) {
            fprintf(stderr, "ra2_port: Bink video: tree %d of %s is not a complete prefix code: movies play without video\n", t, path.c_str());
            return false;
        }
        if (!is_permutation64(tables.patterns[t])) {
            fprintf(stderr, "ra2_port: Bink video: pattern %d of %s is not a permutation: movies play without video\n", t, path.c_str());
            return false;
        }
    }
    if (tables.inter[0][0] != 0x10000) {
        fprintf(stderr, "ra2_port: Bink video: inter quantizer of %s not where expected: movies play without video\n", path.c_str());
        return false;
    }
    tables.ready = true;
    state = 1;
    ra2port::trace("Bink video: tables read from %s", path.c_str());
    return true;
}

// --- Bit reading ------------------------------------------------------------

struct Bits {
    unsigned char const *data;
    size_t size;      // bits
    size_t pos = 0;
    unsigned get(int n)
    {
        unsigned v = 0;
        for (int i = 0; i < n; ++i, ++pos)
            if (pos < size && (data[pos >> 3] >> (pos & 7) & 1)) v |= 1u << i;
        return v;
    }
    int bit() { return (int)get(1); }
    bool over() const { return pos > size; }
};

// A tree choice: one of the sixteen code tables and the order its symbols map to.
struct Tree {
    int table = 0;
    unsigned char symbol[16];
};

int read_symbol(Bits &bits, Tree const &tree)
{
    unsigned char const *codes = tables.codes[tree.table];
    unsigned char const *lengths = tables.lengths[tree.table];
    unsigned value = 0;
    for (int length = 1; length <= 16; ++length) {
        value |= (unsigned)bits.bit() << (length - 1);
        for (int s = 0; s < 16; ++s)
            if (lengths[s] == length && codes[s] == value) return tree.symbol[s];
    }
    return 0;
}

void read_tree(Bits &bits, Tree &tree)
{
    tree.table = (int)bits.get(4);
    if (tree.table == 0) {
        for (int i = 0; i < 16; ++i) tree.symbol[i] = (unsigned char)i;
        return;
    }
    if (bits.bit()) {
        // An explicit list of the first symbols; the unused ones follow in order.
        int count = (int)bits.get(3) + 1;
        bool used[16] = {};
        int n = 0;
        for (; n < count; ++n) {
            tree.symbol[n] = (unsigned char)bits.get(4);
            used[tree.symbol[n]] = true;
        }
        for (int s = 0; s < 16 && n < 16; ++s)
            if (!used[s]) tree.symbol[n++] = (unsigned char)s;
        return;
    }
    // Merge passes: runs of 1, 2, 4, 8 symbols are merged pairwise, one bit per
    // output choosing the left or the right run.
    int passes = (int)bits.get(2) + 1;
    unsigned char a[16], b[16];
    for (int i = 0; i < 16; ++i) a[i] = (unsigned char)i;
    unsigned char *from = a, *to = b;
    for (int pass = 0; pass < passes; ++pass) {
        int run = 1 << pass;
        for (int start = 0; start < 16; start += 2 * run) {
            unsigned char *left = from + start, *right = from + start + run, *out = to + start;
            int nl = run, nr = run;
            while (nl && nr) {
                if (bits.bit()) { *out++ = *right++; --nr; }
                else { *out++ = *left++; --nl; }
            }
            while (nl--) *out++ = *left++;
            while (nr--) *out++ = *right++;
        }
        unsigned char *t = from; from = to; to = t;
    }
    memcpy(tree.symbol, from, 16);
}

// --- Bundles ----------------------------------------------------------------

enum { TYPES, SUB_TYPES, COLOURS, PATTERNS, X_MOTION, Y_MOTION, INTRA_DC, INTER_DC, RUNS, BUNDLES };

struct Bundle {
    int count_bits = 0;
    Tree tree;
    std::vector<int> values;
    size_t filled = 0, used = 0;
    bool ended = false;
    void reset() { values.clear(); filled = used = 0; ended = false; }
    // Values may be read when everything decoded so far was used.
    bool wants() const { return !ended && filled <= used; }
    int next() { return used < values.size() ? values[used++] : 0; }
};

struct Plane {
    int width = 0, height = 0, stride = 0;
    std::vector<unsigned char> pixels;
};

int log2_floor(unsigned v) { int n = -1; while (v) { v >>= 1; ++n; } return n; }

}  // namespace

namespace ra2port {

struct BinkVideo {
    int width, height;
    Plane frames[2][3];    // [current/previous][Y, U, V]
    int current = 0;
    Bundle bundle[BUNDLES];
    Tree colour_high[16];
    int colour_last = 0;
    bool decoded_any = false;
};

bool bink_video_available() { return load_tables(); }

BinkVideo *bink_video_open(int width, int height)
{
    if (!load_tables() || width <= 0 || height <= 0 || width > 4096 || height > 4096) return nullptr;
    BinkVideo *v = new BinkVideo;
    v->width = width;
    v->height = height;
    for (int f = 0; f < 2; ++f) {
        for (int p = 0; p < 3; ++p) {
            Plane &plane = v->frames[f][p];
            int shift = p ? 1 : 0;
            // Allocated to whole 16x16 macroblocks (a scaled block may reach past the edge).
            plane.width = ((width + 15) & ~15) >> shift;
            plane.height = ((height + 15) & ~15) >> shift;
            plane.stride = plane.width + 16;
            plane.pixels.assign((size_t)plane.stride * (plane.height + 16), p ? 128 : 0);
        }
    }
    return v;
}

void bink_video_close(BinkVideo *v) { delete v; }

}  // namespace ra2port

namespace {
using ra2port::BinkVideo;

int signed_value(Bits &bits, int v)
{
    if (v && bits.bit()) v = -v;
    return v;
}

void refill(BinkVideo *v, Bits &bits, int which)
{
    Bundle &b = v->bundle[which];
    if (!b.wants()) return;
    int count = (int)bits.get(b.count_bits);
    if (count == 0) { b.ended = true; return; }
    std::vector<int> &out = b.values;
    size_t end = out.size() + (size_t)count;
    switch (which) {
    case TYPES: case SUB_TYPES:
        if (bits.bit()) {
            int t = (int)bits.get(4);
            out.insert(out.end(), (size_t)count, t);
        } else {
            int last = 0;
            while (out.size() < end && !bits.over()) {
                int s = read_symbol(bits, b.tree);
                if (s < 12) { last = s; out.push_back(s); }
                else out.insert(out.end(), (size_t)type_repeat[s - 12], last);
            }
        }
        break;
    case COLOURS:
        if (bits.bit()) {
            v->colour_last = read_symbol(bits, v->colour_high[v->colour_last]);
            int c = v->colour_last << 4 | read_symbol(bits, b.tree);
            out.insert(out.end(), (size_t)count, c);
        } else {
            while (out.size() < end && !bits.over()) {
                v->colour_last = read_symbol(bits, v->colour_high[v->colour_last]);
                out.push_back(v->colour_last << 4 | read_symbol(bits, b.tree));
            }
        }
        break;
    case PATTERNS:
        while (out.size() < end && !bits.over()) {
            int low = read_symbol(bits, b.tree);
            out.push_back(low | read_symbol(bits, b.tree) << 4);
        }
        break;
    case X_MOTION: case Y_MOTION:
        if (bits.bit()) {
            int m = signed_value(bits, (int)bits.get(4));
            out.insert(out.end(), (size_t)count, m);
        } else {
            while (out.size() < end && !bits.over()) out.push_back(signed_value(bits, read_symbol(bits, b.tree)));
        }
        break;
    case INTRA_DC: case INTER_DC: {
        bool has_sign = which == INTER_DC;
        int dc = (int)bits.get(has_sign ? 10 : 11);
        if (has_sign) dc = signed_value(bits, dc);
        out.push_back(dc);
        int left = count - 1;
        while (left > 0 && !bits.over()) {
            int group = left < 8 ? left : 8;
            int width = (int)bits.get(4);
            for (int k = 0; k < group; ++k) {
                if (width) dc += signed_value(bits, (int)bits.get(width));
                out.push_back(dc);
            }
            left -= group;
        }
        break;
    }
    case RUNS:
        if (bits.bit()) {
            int r = (int)bits.get(4);
            out.insert(out.end(), (size_t)count, r);
        } else {
            while (out.size() < end && !bits.over()) out.push_back(read_symbol(bits, b.tree));
        }
        break;
    }
    if (out.size() > end) out.resize(end);
    b.filled = out.size();
}

// The 8-point inverse transform of the format: integer butterflies with
// constants in 1/2048 (sqrt 2, and the rotation terms), applied to columns
// then rows; the row pass rounds and divides by 256.
void inverse_1d(int const *in, int step, int *out, int out_step, bool final_pass)
{
    int x0 = in[0], x1 = in[step], x2 = in[2 * step], x3 = in[3 * step];
    int x4 = in[4 * step], x5 = in[5 * step], x6 = in[6 * step], x7 = in[7 * step];
    int even_sum = x0 + x4, even_diff = x0 - x4;
    int pair26 = x2 + x6;
    int rot26 = (2896 * (x2 - x6)) >> 11;
    int s53 = x5 + x3, d53 = x5 - x3;
    int s17 = x1 + x7, d17 = x1 - x7;
    int odd0 = s53 + s17;
    int r = (3784 * (d53 + d17)) >> 11;
    int odd1 = ((-5352 * d53) >> 11) - odd0 + r;
    int odd2 = ((2896 * (s17 - s53)) >> 11) - odd1;
    int odd3 = ((2217 * d17) >> 11) + odd2 - r;
    int y[8];
    y[0] = even_sum + pair26 + odd0;
    y[1] = even_diff + rot26 - pair26 + odd1;
    y[2] = even_diff - rot26 + pair26 + odd2;
    y[3] = even_sum - pair26 - odd3;
    y[4] = even_sum - pair26 + odd3;
    y[5] = even_diff - rot26 + pair26 - odd2;
    y[6] = even_diff + rot26 - pair26 - odd1;
    y[7] = even_sum + pair26 - odd0;
    for (int i = 0; i < 8; ++i) out[i * out_step] = final_pass ? (y[i] + 0x7F) >> 8 : y[i];
}

void inverse_dct(int block[64], int result[64])
{
    int temp[64];
    for (int c = 0; c < 8; ++c) {
        bool dc_only = true;
        for (int r = 1; r < 8; ++r) if (block[r * 8 + c]) { dc_only = false; break; }
        if (dc_only) for (int r = 0; r < 8; ++r) temp[r * 8 + c] = block[c];
        else inverse_1d(block + c, 8, temp + c, 8, false);
    }
    for (int r = 0; r < 8; ++r) inverse_1d(temp + r * 8, 1, result + r * 8, 1, true);
}

// Coefficients after the DC, coded by bit planes from the highest down: a list
// of 4-coefficient groups and single coefficients refines which are non-zero.
// Returns false on overrun. Dequantizes with the matrix the coder names.
bool read_coefficients(Bits &bits, int block[64], int const (*matrices)[64])
{
    int list_value[128], list_mode[128];
    int head = 64, tail = 64;
    int seeds[6][2] = {{4, 0}, {24, 0}, {44, 0}, {1, 3}, {2, 3}, {3, 3}};
    for (int i = 0; i < 6; ++i) { list_value[tail] = seeds[i][0]; list_mode[tail++] = seeds[i][1]; }
    int coded[64], coded_count = 0;
    auto read_level = [&](int plane) {
        if (plane == 0) return bits.bit() ? -1 : 1;
        int t = (int)bits.get(plane) | 1 << plane;
        return bits.bit() ? -t : t;
    };
    for (int plane = (int)bits.get(4) - 1; plane >= 0; --plane) {
        int at = head;
        while (at < tail) {
            if (bits.over()) return false;
            if ((list_mode[at] == 0 && list_value[at] == 0) || !bits.bit()) { ++at; continue; }
            int coef = list_value[at], mode = list_mode[at];
            if (mode == 1) {
                // A group of 16 splits into three more groups of 4 after its first 4.
                list_mode[at] = 2;
                for (int k = 0; k < 3; ++k) {
                    coef += 4;
                    if (tail >= 128) return false;
                    list_value[tail] = coef;
                    list_mode[tail++] = 2;
                }
                continue;
            }
            if (mode == 3) {
                block[dct_order[coef]] = read_level(plane);
                coded[coded_count++] = coef;
                list_value[at] = 0;
                list_mode[at++] = 0;
                continue;
            }
            // mode 0 (a group of 16 not yet split) or 2 (a group of 4).
            if (mode == 0) { list_value[at] = coef + 4; list_mode[at] = 1; }
            else { list_value[at] = 0; list_mode[at++] = 0; }
            for (int k = 0; k < 4; ++k, ++coef) {
                if (bits.bit()) {
                    if (head <= 0) return false;
                    list_value[--head] = coef;
                    list_mode[head] = 3;
                } else {
                    block[dct_order[coef]] = read_level(plane);
                    coded[coded_count++] = coef;
                }
            }
        }
    }
    int q = (int)bits.get(4);
    int const *m = matrices[q];
    block[0] = (int)(((long long)block[0] * m[0]) >> 11);
    for (int i = 0; i < coded_count; ++i) {
        int n = dct_order[coded[i]];
        block[n] = (int)(((long long)block[n] * m[n]) >> 11);
    }
    return true;
}

// Residue: corrections by bit planes of a magnitude mask (inter blocks).
bool read_residue(Bits &bits, int block[64], int budget)
{
    int list_value[128], list_mode[128];
    int head = 64, tail = 64;
    int seeds[4][2] = {{4, 0}, {24, 0}, {44, 0}, {0, 2}};
    for (int i = 0; i < 4; ++i) { list_value[tail] = seeds[i][0]; list_mode[tail++] = seeds[i][1]; }
    int nonzero[64], nonzero_count = 0;
    for (int mask = 1 << bits.get(3); mask; mask >>= 1) {
        for (int i = 0; i < nonzero_count; ++i) {
            if (!bits.bit()) continue;
            int &c = block[nonzero[i]];
            c += c < 0 ? -mask : mask;
            if (--budget < 0) return true;
        }
        int at = head;
        while (at < tail) {
            if (bits.over()) return false;
            if ((list_mode[at] == 0 && list_value[at] == 0) || !bits.bit()) { ++at; continue; }
            int coef = list_value[at], mode = list_mode[at];
            if (mode == 1) {
                list_mode[at] = 2;
                for (int k = 0; k < 3; ++k) {
                    coef += 4;
                    if (tail >= 128) return false;
                    list_value[tail] = coef;
                    list_mode[tail++] = 2;
                }
                continue;
            }
            if (mode == 3) {
                int n = dct_order[coef];
                nonzero[nonzero_count++] = n;
                block[n] = bits.bit() ? -mask : mask;
                list_value[at] = 0;
                list_mode[at++] = 0;
                if (--budget < 0) return true;
                continue;
            }
            if (mode == 0) { list_value[at] = coef + 4; list_mode[at] = 1; }
            else { list_value[at] = 0; list_mode[at++] = 0; }
            for (int k = 0; k < 4; ++k, ++coef) {
                if (bits.bit()) {
                    if (head <= 0) return false;
                    list_value[--head] = coef;
                    list_mode[head] = 3;
                } else {
                    int n = dct_order[coef];
                    nonzero[nonzero_count++] = n;
                    block[n] = bits.bit() ? -mask : mask;
                    if (--budget < 0) return true;
                }
            }
        }
    }
    return true;
}

unsigned char clamp255(int v) { return (unsigned char)(v < 0 ? 0 : v > 255 ? 255 : v); }

// An 8x8 block of values in pixel order, written at dst (scale 2: each value 2x2).
void put_block(unsigned char *dst, int stride, int const *values, int scale)
{
    for (int y = 0; y < 8 * scale; ++y)
        for (int x = 0; x < 8 * scale; ++x)
            dst[y * stride + x] = clamp255(values[(y / scale) * 8 + x / scale]);
}

void copy_block(unsigned char *dst, unsigned char const *src, int stride)
{
    for (int y = 0; y < 8; ++y) memcpy(dst + y * stride, src + y * stride, 8);
}

// One 8x8 (or doubled) block of a run, pattern, raw, fill or intra kind into values.
bool decode_simple(BinkVideo *v, Bits &bits, int type, int values[64])
{
    switch (type) {
    case 3: {   // runs along one of 16 scan patterns
        unsigned char const *order = tables.patterns[bits.get(4)];
        int i = 0;
        while (i < 63) {
            int run = v->bundle[RUNS].next() + 1;
            if (i + run > 64) return false;
            if (bits.bit()) {
                int c = v->bundle[COLOURS].next();
                for (int k = 0; k < run; ++k) values[order[i++]] = c;
            } else {
                for (int k = 0; k < run; ++k) values[order[i++]] = v->bundle[COLOURS].next();
            }
        }
        if (i == 63) values[order[i]] = v->bundle[COLOURS].next();
        return true;
    }
    case 5: {   // intra DCT
        int block[64] = {};
        block[0] = v->bundle[INTRA_DC].next();
        if (!read_coefficients(bits, block, tables.intra)) return false;
        inverse_dct(block, values);
        return true;
    }
    case 6: {   // fill
        int c = v->bundle[COLOURS].next();
        for (int i = 0; i < 64; ++i) values[i] = c;
        return true;
    }
    case 8: {   // two colours, one bit per pixel, a byte per row
        int c0 = v->bundle[COLOURS].next(), c1 = v->bundle[COLOURS].next();
        for (int y = 0; y < 8; ++y) {
            int row = v->bundle[PATTERNS].next();
            for (int x = 0; x < 8; ++x, row >>= 1) values[y * 8 + x] = row & 1 ? c1 : c0;
        }
        return true;
    }
    case 9:     // raw
        for (int i = 0; i < 64; ++i) values[i] = v->bundle[COLOURS].next();
        return true;
    }
    return false;
}

bool decode_plane(BinkVideo *v, Bits &bits, int index, bool chroma)
{
    Plane &plane = v->frames[v->current][index];
    Plane &prev = v->frames[v->current ^ 1][index];
    int bw = chroma ? (v->width + 15) >> 4 : (v->width + 7) >> 3;
    int bh = chroma ? (v->height + 15) >> 4 : (v->height + 7) >> 3;
    int width = v->width >> (chroma ? 1 : 0);
    if (width < 8) width = 8;
    width = (width + 7) & ~7;
    int wide = log2_floor((unsigned)(width >> 3) + 511) + 1;
    v->bundle[TYPES].count_bits = wide;
    v->bundle[SUB_TYPES].count_bits = log2_floor((unsigned)(width >> 4) + 511) + 1;
    v->bundle[COLOURS].count_bits = log2_floor((unsigned)bw * 64 + 511) + 1;
    v->bundle[INTRA_DC].count_bits = v->bundle[INTER_DC].count_bits = wide;
    v->bundle[X_MOTION].count_bits = v->bundle[Y_MOTION].count_bits = wide;
    v->bundle[PATTERNS].count_bits = log2_floor((unsigned)bw * 8 + 511) + 1;
    v->bundle[RUNS].count_bits = log2_floor((unsigned)bw * 48 + 511) + 1;
    for (int i = 0; i < BUNDLES; ++i) {
        v->bundle[i].reset();
        if (i == COLOURS) {
            for (int k = 0; k < 16; ++k) read_tree(bits, v->colour_high[k]);
            v->colour_last = 0;
        }
        if (i != INTRA_DC && i != INTER_DC) read_tree(bits, v->bundle[i].tree);
    }
    int const stride = plane.stride;
    for (int by = 0; by < bh; ++by) {
        for (int i = 0; i < BUNDLES; ++i) refill(v, bits, i);
        if (bits.over()) return false;
        for (int bx = 0; bx < bw; ++bx) {
            unsigned char *dst = &plane.pixels[(size_t)by * 8 * stride + bx * 8];
            unsigned char const *ref = &prev.pixels[(size_t)by * 8 * stride + bx * 8];
            int type = v->bundle[TYPES].next();
            if ((by & 1) && type == 1) { ++bx; continue; }   // the lower half of a scaled block above
            int values[64];
            switch (type) {
            case 0:
                copy_block(dst, ref, stride);
                break;
            case 1: {
                int sub = v->bundle[SUB_TYPES].next();
                if (!decode_simple(v, bits, sub, values)) return false;
                put_block(dst, stride, values, 2);
                ++bx;
                break;
            }
            case 2: case 4: case 7: {
                int dx = v->bundle[X_MOTION].next(), dy = v->bundle[Y_MOTION].next();
                int sx = bx * 8 + dx, sy = by * 8 + dy;
                if (sx < 0 || sy < 0 || sx + 8 > plane.width + 16 || sy + 8 > plane.height + 16) return false;
                copy_block(dst, &prev.pixels[(size_t)sy * stride + sx], stride);
                if (type == 4) {
                    int block[64] = {};
                    int budget = (int)bits.get(7);
                    if (!read_residue(bits, block, budget)) return false;
                    for (int y = 0; y < 8; ++y)
                        for (int x = 0; x < 8; ++x) dst[y * stride + x] = clamp255(dst[y * stride + x] + block[y * 8 + x]);
                } else if (type == 7) {
                    int block[64] = {};
                    block[0] = v->bundle[INTER_DC].next();
                    if (!read_coefficients(bits, block, tables.inter)) return false;
                    int delta[64];
                    inverse_dct(block, delta);
                    for (int y = 0; y < 8; ++y)
                        for (int x = 0; x < 8; ++x) dst[y * stride + x] = clamp255(dst[y * stride + x] + delta[y * 8 + x]);
                }
                break;
            }
            case 3: case 5: case 6: case 8: case 9:
                if (!decode_simple(v, bits, type, values)) return false;
                put_block(dst, stride, values, 1);
                break;
            default:
                return false;
            }
        }
    }
    bits.pos = (bits.pos + 31) & ~(size_t)31;
    return true;
}

}  // namespace

namespace ra2port {

// Decodes one frame's video data; false (and the frame unchanged) on bad data.
bool bink_video_decode(BinkVideo *v, unsigned char const *data, size_t size)
{
    if (!v || !data || size < 4) return false;
    v->current ^= 1;
    Bits bits = {data, size * 8};
    bits.get(32);
    static int const plane_order[3] = {0, 2, 1};   // Y, then V (Cr), then U (Cb)
    bool ok = true;
    for (int p = 0; p < 3 && ok; ++p) {
        ok = decode_plane(v, bits, plane_order[p], p != 0);
        if (bits.pos >= bits.size) break;
    }
    if (!ok) {
        static int reported = 0;
        if (reported++ < 4) trace("Bink video: bad frame data (plane stopped at bit %zu of %zu)", bits.pos, bits.size);
    }
    v->decoded_any = true;
    return ok;
}

// The last decoded frame as 16-bit 5:6:5 (BT.601 YCbCr) at x, y of dest.
void bink_video_copy_565(BinkVideo *v, void *dest, int pitch, unsigned height, unsigned x, unsigned y)
{
    if (!v || !v->decoded_any) return;
    Plane const &py = v->frames[v->current][0], &pu = v->frames[v->current][1], &pv = v->frames[v->current][2];
    for (int row = 0; row < v->height && y + row < height; ++row) {
        unsigned short *out = (unsigned short *)((unsigned char *)dest + (size_t)(y + row) * (unsigned)pitch) + x;
        unsigned char const *ly = &py.pixels[(size_t)row * py.stride];
        unsigned char const *lu = &pu.pixels[(size_t)(row >> 1) * pu.stride];
        unsigned char const *lv = &pv.pixels[(size_t)(row >> 1) * pv.stride];
        for (int col = 0; col < v->width; ++col) {
            int luma = (ly[col] - 16) * 298;
            int cb = lu[col >> 1] - 128, cr = lv[col >> 1] - 128;
            int r = clamp255((luma + 409 * cr + 128) >> 8);
            int g = clamp255((luma - 100 * cb - 208 * cr + 128) >> 8);
            int b = clamp255((luma + 516 * cb + 128) >> 8);
            out[col] = (unsigned short)((r >> 3) << 11 | (g >> 2) << 5 | (b >> 3));
        }
    }
}

}  // namespace ra2port
