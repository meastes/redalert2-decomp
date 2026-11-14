// Behaviour rewrites of the library rows whose accepted source is (evidenced)
// inline assembly, and of the TSC helper in detproc.asm, for the native tree
// only. NOT EXACT and not original code: portable C++ written from the
// accepted text (TibSun archive inline assembly, adopted: GPL-3.0-or-later
// with EA's section 7 terms, THIRD_PARTY.md) and the archive procedure
// src/recovered/asm/detproc.asm. native_units.py skips the __asm rows; these
// definitions replace their link-closure traps.
// Tests: native/tests/asm_check.cpp (native-asm-check). Manifest:
// native/behaviour_rewrites.json.
#include "shared/lcw.hpp"
#include "shared/bench.hpp"
#include "shared/cpu_info.hpp"
#include "shared/basic_xlat_blitters.hpp"
#include <stdint.h>
#include <stddef.h>
#include <time.h>

// ---------------------------------------------------------------------------
// LCW_Comp 0x00534D40 (src/recovered/lcw/LCW_Comp.cpp: TibSun
// lcw.cpp's inline assembly, Westwood's LCWCOMP.ASM). A transliteration of
// the register program: the same greedy search, so the output bytes equal the
// original's for any input.
//   - The output starts with a one-byte literal (0x81, byte).
//   - At each position: if the byte 64 ahead is the same, the run of equal
//     bytes is measured (repe scasb to the end of the data, minus one: a run
//     reaching the end is counted one short); 65 or more emits a fill code
//     FE len16 byte and the position moves past it.
//   - Otherwise every earlier position (from the start of the data) holding
//     the current byte is tried: the candidate is compared only if it also
//     matches at offset count-1 (the best length so far); the longest match
//     wins, the first one on ties.
//   - Length <= 2: the byte is appended to the open literal (80+n), opening a
//     new one when none is open or it holds 63 bytes. 3..10 with a distance
//     <= 0xFFF: 0LLLdddd dddddddd (relative). Up to 64: 11LLLLLL abs16.
//     Longer: FF len16 abs16 (absolute offsets from the start of the data).
//   - The data ends with 0x80; returns the compressed size.
// The original compares the byte 64 ahead even when it lies past the end of
// the data; a run there is always shorter than 65 (it is bounded by the end),
// so skipping that read when it is out of range gives the same output.
// ---------------------------------------------------------------------------
int LCW_Comp(void const *source, void *dest, int datasize)
{
    uint8_t const *esi = (uint8_t const *)source;
    uint8_t *out = (uint8_t *)dest;
    uint8_t const *const end = esi + datasize;
    uint8_t *const a1stdest = out;
    uint8_t const *const a1stsrc = esi;
    uint8_t *lenoff = out;
    bool inlen = true;
    uint8_t const *matchoff = esi;

    *out++ = 0x81;
    *out++ = *esi++;
    while (esi < end) {
        uint8_t const *search = a1stsrc;       // edi while searching
        uint32_t count = 1;
        uint8_t al;
    searchloop:
        al = esi[0];
        if (esi + 64 < end && al == esi[64]) {
            uint8_t const *p = esi;            // repe scasb over end - esi bytes
            for (ptrdiff_t n = end - esi; n != 0; ) {
                --n;
                if (*p++ != al) break;
            }
            --p;
            uint32_t run = (uint32_t)(p - esi);
            if (run >= 65) {
                inlen = false;
                esi = p;
                out[0] = 0xFE;
                out[1] = (uint8_t)run;
                out[2] = (uint8_t)(run >> 8);
                out[3] = al;
                out += 4;
                if (esi >= end) break;         // (the asm's next loop sees nothing left)
                goto searchloop;
            }
        }
        for (;;) {                             // oploop
            ptrdiff_t n = esi - search;
            if (n == 0) break;
            bool found = false;                 // repne scasb
            while (n != 0) {
                --n;
                if (*search++ == al) { found = true; break; }
            }
            if (!found) break;
            if (esi[(ptrdiff_t)count - 1] != search[(ptrdiff_t)count - 2]) continue;
            uint8_t const *saved = search;
            uint8_t const *q = search - 1;
            uint8_t const *s = esi;
            bool equal = true;                   // repe cmpsb over end - esi bytes
            for (ptrdiff_t m = end - esi; m != 0; ) {
                --m;
                equal = (*s++ == *q++);
                if (!equal) break;
            }
            if (equal) ++q;
            uint32_t length = (uint32_t)(q - saved);
            search = saved;
            if (length < count) goto searchloop;
            count = length;
            matchoff = saved - 1;
            goto searchloop;
        }
        // searchdone
        if (count <= 2) {
            if (!inlen || *lenoff == 0xBF) {
                do {
                    lenoff = out;
                    *out++ = 0x80;
                } while (*lenoff == 0xBF);
            }
            (*lenoff)++;
            *out++ = *esi++;
            inlen = true;
            continue;
        }
        uint32_t distance = (uint32_t)(esi - matchoff);
        if (count <= 10 && distance <= 0xFFF) {
            out[0] = (uint8_t)(((count - 3) << 4) + (distance >> 8));
            out[1] = (uint8_t)distance;
            out += 2;
        } else {
            if (count <= 64) {
                *out++ = (uint8_t)((count - 3) | 0xC0);
            } else {
                out[0] = 0xFF;
                out[1] = (uint8_t)count;
                out[2] = (uint8_t)(count >> 8);
                out += 3;
            }
            uint32_t offset = (uint32_t)(matchoff - a1stsrc);
            out[0] = (uint8_t)offset;
            out[1] = (uint8_t)(offset >> 8);
            out += 2;
        }
        esi += count;
        inlen = false;
    }
    *out++ = 0x80;
    return (int)(out - a1stdest);
}

// ---------------------------------------------------------------------------
// The time-stamp counter. Get_CPU_Clock 0x0077F274 (detproc.asm: rdtsc, the
// high half stored through the reference, the low half returned), RDTSC
// 0x005ABF60 (TibSun mpu.cpp: rdtsc into TSC_Low/TSC_High) and Get_RDTSC_CPU_Speed
// 0x005ABF70 (TibSun mpu.cpp: the TSC rate in MHz, measured against the
// performance counter). Natively the TSC is a virtual 1 GHz counter (the
// port's game clock in nanoseconds, the same clock the port's
// QueryPerformanceCounter reads), so its rate is exactly 1000 MHz:
// Get_RDTSC_CPU_Speed returns the value the original's measurement converges to
// (its rounding steps leave an exact rate unchanged) without the busy loop.
// ---------------------------------------------------------------------------
// The port's game clock (port/src/win32_time.cpp): it stands still while the host
// pause holds the game (Scout1247), and keeps Apple's clock out of this layer.
extern "C" uint64_t ra2port_game_ns(void);
namespace {
uint64_t virtual_tsc() { return ra2port_game_ns(); }
}

unsigned int Get_CPU_Clock(unsigned int &high)
{
    uint64_t t = virtual_tsc();
    high = (unsigned int)(t >> 32);
    return (unsigned int)t;
}

// RDTSC's two globals (0x00A71780 / 0x00A7177C, zero-initialised .bss); no
// other native unit names them.
unsigned int TSC_Low;
unsigned int TSC_High;

void RDTSC()
{
    uint64_t t = virtual_tsc();
    TSC_Low = (unsigned int)t;
    TSC_High = (unsigned int)(t >> 32);
}

// Get_RDTSC_CPU_Speed 0x005ABF70 (M25, Scout1202): the original's measurement (TibSun mpu.cpp,
// src/recovered/mpu.cpp) with the virtual TSC for its two
// rdtsc reads and the port's performance counter (the same nanosecond clock, frequency
// 10^9): up to 20 samples of 1,000 counter ticks until three agree within 1 MHz, then
// the rounding to a tenth. NOT EXACT only in the rdtsc source; it returns 1000 here,
// as before, now by measuring.
// (The port's QueryPerformanceCounter reads mach_absolute_time in nanoseconds, the clock
// CLOCK_UPTIME_RAW gives: read here directly, so native-asm-check links without the port.)
namespace {
int QueryPerformanceFrequency(int64_t *frequency) { *frequency = 1000000000; return 1; }
int QueryPerformanceCounter(int64_t *counter) { *counter = (int64_t)virtual_tsc(); return 1; }
}

int Get_RDTSC_CPU_Speed()
{
    int64_t t0, t1, count_freq;
    unsigned int freq = 0, freq2 = 0, freq3 = 0, total = 0;
    int tries = 0;
    unsigned int total_cycles = 0, cycles, stamp0, stamp1, total_ticks = 0, ticks;
    if (!QueryPerformanceFrequency(&count_freq)) return 0;
    unsigned int freq_low = (unsigned int)count_freq;
    auto spread = [](unsigned int a, unsigned int t) { int d = (int)(3 * a - t); return d < 0 ? -d : d; };
    do {
        tries++;
        freq3 = freq2;
        freq2 = freq;
        QueryPerformanceCounter(&t0);
        t1 = t0;
        while ((unsigned int)t1 - (unsigned int)t0 < 50) QueryPerformanceCounter(&t1);
        stamp0 = (unsigned int)virtual_tsc();
        t0 = t1;
        while ((unsigned int)t1 - (unsigned int)t0 < 1000) QueryPerformanceCounter(&t1);
        stamp1 = (unsigned int)virtual_tsc();
        cycles = stamp1 - stamp0;
        ticks = (unsigned int)t1 - (unsigned int)t0;
        ticks = ticks * 100000;
        ticks = ticks / (freq_low / 10);
        total_ticks += ticks;
        total_cycles += cycles;
        if (ticks % freq_low > freq_low / 2) ticks++;
        if (ticks == 0) ticks = 1;                 // native guard (the original divides by it)
        freq = cycles / ticks;
        if (cycles % ticks > ticks / 2) freq++;
        total = freq + freq2 + freq3;
    } while (tries < 3 || (tries < 20 && (spread(freq, total) > 3 || spread(freq2, total) > 3 || spread(freq3, total) > 3)));
    if (total_ticks == 0) return 0;
    freq3 = (total_cycles * 10) / total_ticks;
    freq2 = (total_cycles * 100) / total_ticks;
    if (freq2 - freq3 * 10 >= 6) freq3++;
    int norm_freq = total_cycles / total_ticks;
    freq = norm_freq * 10;
    if (freq3 - freq >= 6) norm_freq++;
    return norm_freq;
}

// ---------------------------------------------------------------------------
// BlitTransXlat<unsigned short>::BlitForward 0x004861C0 (src/recovered/
// classes/BlitTransXlat_unsigned_short/: TibSun blitblit.h's inline assembly).
// For each of length source bytes: zero is transparent (the target pixel is
// left as is), any other index is written through the 16-bit palette.
// length <= 0 draws nothing (the original's counter would wrap below zero).
// ---------------------------------------------------------------------------
extern "C" void ra2asm_trans_xlat_forward16(void *dest, void const *source, int len, unsigned short const *xlator)
{
    unsigned short *d = (unsigned short *)dest;
    unsigned char const *s = (unsigned char const *)source;
    for (int i = 0; i < len; ++i) {
        if (s[i] != 0) d[i] = xlator[s[i]];
    }
}

template <>
void BlitTransXlat<unsigned short>::BlitForward(void *dest, void const *source, int len, int, void *, void *, int, int) const
{
    ra2asm_trans_xlat_forward16(dest, source, len, Palette);
}
