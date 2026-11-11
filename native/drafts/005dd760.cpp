// NON-EXACT DRAFT 0x5DD760 FUN_005dd760
// entry 0x5DD760; exact-match score 63.16 (misses_index best), this text 48.0; plan "review: rewrite likely"
// origin lane: Scout558 (build/class-work/Scout558/sp_loop2_fresh_ptr_48.00.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Builds the spot light shapes once: 64 discs of growing size (1, 3, ... 127) and ten of a
// fixed radius from Rules' spot light radius with fading strength, each drawn into a
// 256x256 surface and copied down to 256x128 every other line; then, with CPUID, the
// 64K-entry table from each high-colour pixel to its 24-bit RGB value (0x005DD760).
#include "shared/surface.hpp"
#include "shared/rules.hpp"
#include "shared/hicolor.hpp"
#include "shared/cpu_info.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref __real@8@bff6b6db6db6db6db800=007a7b00
// @ref ?unknown_spot_light_shapes_a731e0@@3PAPAVBSurface@@A=00a731e0
// @ref ?unknown_block_a73378@@3PAXA=00a73378
// @ref ??2@YAPAXI@Z=00782017
// @ref ??0Buffer@@QAE@PAXJ@Z=004382c0
// @ref ??1Buffer@@QAE@XZ=00438410
// @ref ??_7XSurface@@6B@=0079af8c
// @ref ??_7BSurface@@6B@=0079aef8
// @ref ??_7Surface@@6B@=0079b020
// @ref ??0Surface@@QAE@HH@Z=0049ed90
// @ref ?Fill@XSurface@@UAE_NH@Z=00774e20
// @ref ?Fill_Circle@XSurface@@QAEXUPoint2D@@HVRect@@H@Z=00774c90
// @ref ?Get_Unknown_7d6290@@YAHXZ=004abb90
// @ref ?Get_CPU_Info@@YIXPAHPAEPADI@Z=004d2a20
// @ref ?unknown_spot_light_shapes_ready_a73374@@3_NA=00a73374
// @ref ?unknown_spot_light_pixel_7e8c4c@@3HA=007e8c4c
// @ref ?unknown_shift_852fb8@@3HA=00852fb8
// @ref ?unknown_shift_852fbc@@3HA=00852fbc
// @ref ?unknown_shift_852fc0@@3HA=00852fc0
// @ref ?unknown_shift_852fc4@@3HA=00852fc4
// @ref ?unknown_shift_852fc8@@3HA=00852fc8
// @ref ?unknown_shift_852fcc@@3HA=00852fcc

// The spot light shapes (0x00A731E0): 64 growing discs, then ten fixed-radius ones.
extern BSurface *unknown_spot_light_shapes_a731e0[74];
// The high-colour pixel to RGB table (0x00A73378; Unknown_005DDB40 frees it).
extern void *unknown_block_a73378;
// Set once the shapes are built (0x00A73374).
bool unknown_spot_light_shapes_ready_a73374;
// A copy of Get_Unknown_7d6290() taken here (0x007E8C4C).
extern int unknown_spot_light_pixel_7e8c4c;

void Unknown_005DD760()
{
    if (!unknown_spot_light_shapes_ready_a73374) {
        BSurface **shape = &unknown_spot_light_shapes_a731e0[0];
        for (int size = 1; size < 0x81; size += 2) {
            if (*shape == 0) {
                *shape = new BSurface(256, 128, 1);
            }
            (*shape)->Fill(0);

            BSurface work(256, 256, 1);
            work.Fill(0);
            for (int radius = size, strength = 12; radius > 0; radius -= 2, strength += 4) {
                work.Fill_Circle(Point2D(0x80, 0x80), radius, Rect(0, 0, 255, 255), strength <= 255 ? strength : 255);
            }
            for (int y = 0; y < 0x80; y++) {
                Rect source = Rect(0, y * 2, 255, 1);
                Rect dest(0, y, 255, 1);
                (*shape)->Copy_From(dest, work, source, false, true);
            }
            shape++;
        }

        BSurface **fixed_shape = &unknown_spot_light_shapes_a731e0[64];
        for (int step = 0, strength = 0x80; strength > 0x44; step++, strength -= 6) {
            if (*fixed_shape == 0) {
                *fixed_shape = new BSurface(256, 128, 1);
            }
            (*fixed_shape)->Fill(0);

            BSurface work(256, 256, 1);
            work.Fill(0);
            // behaviour fix (codex4 r536): the original factor is -1/358.4 (literal 0x007A7B00), not -1/360.
            work.Fill_Circle(Point2D(0x80, 0x80), step - (int)(Rules->unknown_6d0 * 60 * (-1.0 / 358.4)),
                Rect(0, 0, 255, 255), strength);
            for (int y = 0; y < 0x80; y++) {
                Rect source = Rect(0, y * 2, 255, 1);
                Rect dest(0, y, 255, 1);
                (*fixed_shape)->Copy_From(dest, work, source, false, true);
            }
            fixed_shape++;
        }
        unknown_spot_light_shapes_ready_a73374 = true;
    }

    unknown_spot_light_pixel_7e8c4c = Get_Unknown_7d6290();
    int family = 5;
    unsigned char has_cpuid = 0;
    Get_CPU_Info(&family, &has_cpuid, 0, 0);
    if (has_cpuid && unknown_block_a73378 == 0) {
        unknown_block_a73378 = new int[0x10000];
        for (int index = 0; index < 0x10000; index++) {
            RGBClass color = Unpack_Hicolor_Pixel((unsigned short)index);
            ((int *)unknown_block_a73378)[index] = (color.Red << 16) | (color.Green << 8) | color.Blue;
        }
    }
}
