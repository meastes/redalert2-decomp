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

// Adjust_Color_565/555/556/655, YR 0x007DE200, 0x007DE53A, 0x007DE87A, 0x007DEBBA (winasm.asm's
// ADJUST_COLOR macro; lightcon.cpp calls them by address). The RA2 native tree's C for the same
// macro (native/link/behaviour_launch.cpp, RA2 0x00797440 and 0x0079777A; lane Y3): a 256-entry
// 16-bit table from the palette's RGB bytes, entry 0 transparent (0); entry i scales the palette's
// red, green and blue by red/green/blue where mask[i] is set, else all three by intensity (16.16
// fixed point, saturated to 255), packed by the macro's masks and shifts for each mode. The
// original's MMX and CMOV paths give the same table; this is the plain path.
static void Adjust_Color(void const *palette, void *translator, int red, int green, int blue, int intensity,
                         bool const *mask, unsigned r_mask, int r_shift, unsigned g_mask, int g_shift,
                         unsigned b_mask, int b_shift)
{
    uint8_t const *rgb = (uint8_t const *)palette;
    uint16_t *table = (uint16_t *)translator;
    table[0] = 0;
    for (int i = 1; i < 256; ++i) {
        unsigned r_scale = mask[i] ? (unsigned)red : (unsigned)intensity;
        unsigned g_scale = mask[i] ? (unsigned)green : (unsigned)intensity;
        unsigned b_scale = mask[i] ? (unsigned)blue : (unsigned)intensity;
        unsigned r = ((unsigned)rgb[3 * i] * r_scale) >> 16;
        unsigned g = ((unsigned)rgb[3 * i + 1] * g_scale) >> 16;
        unsigned b = ((unsigned)rgb[3 * i + 2] * b_scale) >> 16;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        table[i] = (uint16_t)(((r & r_mask) << r_shift) | ((g & g_mask) << g_shift) | ((b & b_mask) >> b_shift));
    }
}

extern "C" {
void FUN_007DE200(void *palette, void *translator, int red, int green, int blue, int intensity, bool *mask)
{
    Adjust_Color(palette, translator, red, green, blue, intensity, mask, 0xF8, 8, 0xFC, 3, 0xF8, 3);   // 565
}
void FUN_007DE53A(void *palette, void *translator, int red, int green, int blue, int intensity, bool *mask)
{
    Adjust_Color(palette, translator, red, green, blue, intensity, mask, 0xF8, 7, 0xF8, 2, 0xF8, 3);   // 555
}
void FUN_007DE87A(void *palette, void *translator, int red, int green, int blue, int intensity, bool *mask)
{
    Adjust_Color(palette, translator, red, green, blue, intensity, mask, 0xF8, 8, 0xF8, 3, 0xFC, 2);   // 556
}
void FUN_007DEBBA(void *palette, void *translator, int red, int green, int blue, int intensity, bool *mask)
{
    Adjust_Color(palette, translator, red, green, blue, intensity, mask, 0xFC, 8, 0xF8, 2, 0xF8, 3);   // 655
}
}
