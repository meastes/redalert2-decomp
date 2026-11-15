// Behaviour rewrites of the YR tree's assembly for the native build (lane Y1). NOT EXACT and not
// original code: the RA2 native tree's C++ for the same TibSun routines (native/link/
// behaviour_asm_lib.cpp, written from the TibSun archive's inline assembly; GPL-3.0-or-later with
// EA's section 7 terms, THIRD_PARTY.md), with the YR tree's names and signatures. The functions
// they replace are cut from the generated units (native/yr/fixups.json "cut").
#include <stdint.h>
#include <stddef.h>

// LCW_Comp, YR 0x00551E50 (lcw.cpp: TibSun lcw.cpp's inline assembly, Westwood's LCWCOMP.ASM).
// The RA2 tree's transliteration of the register program (RA2 0x00534D40), unchanged.
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

// RDTSC, YR 0x005CAFB0, and Get_RDTSC_CPU_Speed, YR 0x005CAFC0 (mpu.cpp: TibSun mpu.cpp). Natively
// the time-stamp counter is the port's game clock in nanoseconds (a virtual 1 GHz counter, the
// clock the port's QueryPerformanceCounter reads), so the measured rate is 1000 MHz; the RA2 tree
// does the same (behaviour_asm_lib.cpp).
extern "C" uint64_t ra2port_game_ns(void);
namespace {
unsigned int yr_tsc_low, yr_tsc_high;
}
void RDTSC(void)
{
    uint64_t t = ra2port_game_ns();
    yr_tsc_low = (unsigned int)t;
    yr_tsc_high = (unsigned int)(t >> 32);
}
int Get_RDTSC_CPU_Speed(void) { return 1000; }

// Detect_MMX_Availability / Detect_CMOV_Availability (detproc.asm, Westwood's handwritten
// assembly; the RA2 tree's 0x0077F14D / 0x0077F20F behaviour, native/link/behaviour_launch.cpp).
// arm64 has neither instruction: a CPUID-capable family-6 processor without MMX and CMOV, so
// every MMX/CMOV branch takes the plain path; the stored vendor text is left as it is.
extern "C" {
extern char CPUType;
extern unsigned char UseMMX;
extern unsigned char UseCMOV;
extern unsigned char HasCMOV;

bool Detect_MMX_Availability(void)
{
    CPUType = 6;
    UseMMX = 0;
    return false;
}

bool Detect_CMOV_Availability(void)
{
    UseCMOV = 0;
    HasCMOV = 0;
    return false;
}
}
