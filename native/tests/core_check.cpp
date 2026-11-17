// Native proof: recovered RandomClass / Random2Class / CRCEngine / Matrix3D
// code built for arm64 from src/recovered through native_units.py, run here.
// Checks are behavioural invariants and independent reference formulas, not
// values taken from the original executable.
#include "shared/random.hpp"
#include "shared/fast_math.hpp"
#include <stdio.h>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// VC6 string literals are writable (.data); recovered code writes through
// pointers callers pass literals to (MixFileClass::Offset's _strupr, wcstrim on
// L""). The dialect's -fwritable-strings keeps that meaning (README).
static void __attribute__((noinline)) upcase_in_place(char const *text)
{
    for (char *p = (char *)text; *p; ++p) {
        if (*p >= 'a' && *p <= 'z') *p = (char)(*p - 32);
    }
}
static void __attribute__((noinline)) clear_wide(unsigned short const *text) { *(unsigned short *)text = 0; }

int main()
{
    // RandomClass: Westwood LCG (TibSun random.cpp): seed = seed*0x41C64E6D + 0x3039,
    // result = (seed >> 10) & 0x7FFF, all in 32-bit unsigned arithmetic.
    RandomClass r(12345);
    uint32_t ref = 12345;
    for (int i = 0; i < 1000; ++i) {
        ref = ref * 0x41C64E6Du + 0x3039u;
        int expect = (int)((ref >> 10) & 0x7FFF);
        int got = r();
        CHECK(got == expect);
        if (got != expect) break;
    }
    CHECK(sizeof(RandomClass) == 4);
    RandomClass a(7), b(7);
    for (int i = 0; i < 100; ++i) {
        int x = a(3, 9), y = b(3, 9);
        CHECK(x == y && x >= 3 && x <= 9);
    }
    // Random3Class: Mix1/Mix2 are defined by the random module (src/recovered/random.cpp). The
    // reference below is TibSun random.cpp's algorithm in 32-bit arithmetic.
    {
        static const uint32_t m1[4] = {0xbaa96887u, 0x1e17d32cu, 0x03bcdc3cu, 0x0f33d1b2u};
        static const uint32_t m2[4] = {0x4b0f3b58u, 0xe874f0c3u, 0x6955c5a6u, 0x55a7ca46u};
        Random3Class g(42, 0);
        int32_t seed = 42, index = 0;
        for (int n = 0; n < 1000; ++n) {
            int32_t lo = seed, hi = index++;
            for (int i = 0; i < 4; ++i) {
                int32_t hold = hi;
                int32_t t = (int32_t)((uint32_t)hold ^ m1[i]);
                int32_t l = t & 0xffff, h = t >> 16;
                t = (int32_t)((uint32_t)(l * l) + ~(uint32_t)(h * h));
                t = (t >> 16) | (int32_t)((uint32_t)t << 16);  // arithmetic shift, as the int source
                hi = lo ^ (int32_t)(((uint32_t)t ^ m2[i]) + (uint32_t)(l * h));
                lo = hold;
            }
            int got = g();
            CHECK(got == hi);
            if (got != hi) break;
        }
    }
    {
        char const *name = "rules.ini";
        upcase_in_place(name);
        CHECK(name[0] == 'R' && name[4] == 'S' && name[5] == '.');
        unsigned short const *empty = L"";
        clear_wide(empty);
        CHECK(empty[0] == 0);
    }
    {
        // VC6's _ftol conversion (ra2port::x87_ftol, prelude.hpp): fistp qword, low 32 bits;
        // the integer indefinite (low half 0) outside the 64-bit range and for a NaN.
        volatile double big = 3000000000.0, huge = 1e30, nan = __builtin_nan("");
        CHECK(ra2port::x87_ftol(big) == (int)(3000000000LL - 4294967296LL));
        CHECK(ra2port::x87_ftol(-big) == (int)(4294967296LL - 3000000000LL));
        CHECK(ra2port::x87_ftol(-2147483648.0) == (int)0x80000000u);
        CHECK(ra2port::x87_ftol(huge) == 0 && ra2port::x87_ftol(-huge) == 0 && ra2port::x87_ftol(nan) == 0);
        CHECK(ra2port::x87_ftol(-2.9) == -2 && ra2port::x87_ftol(2.9) == 2);
        // Fast_Atan2 0x004BA600 (C5): a pitch with a horizontal speed near 0 (a projectile
        // falling straight down) divides to beyond -2^31; the original reads entry 0 (the
        // 32-bit address wraps) where arm64's saturating conversion read 8 GB before the table.
        volatile double tiny = 1e-30;
        CHECK(Fast_Atan2(-1.0, tiny) == -unknown_atan_table_813df4[0]);
        CHECK(Fast_Atan2(1.0, tiny) == unknown_atan_table_813df4[0]);
        CHECK(Fast_Atan(-1e30) == -unknown_atan_table_813df4[0]);
        float diagonal = Fast_Atan2(1.0, 1.0);
        CHECK(diagonal > 0.76f && diagonal < 0.79f);   // table entry 40: atan(40 x tan(pi/2 - 0.01) / 4096)
    }
    printf("RandomClass ok, first value %d\n", RandomClass(1)());
    printf("%s (%d failures)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
