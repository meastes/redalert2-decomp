// NON-EXACT DRAFT 0x5FE9B0 FUN_005fe9b0
// entry 0x5FE9B0; exact-match score 22.54 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout1031 (build/class-work/Scout1031/w/g3.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
#include "shared/win32_api.hpp"
#include "shared/surface.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

// Halves the brightness of a left strip width pixels wide and a top strip height pixels high of rect.
void __fastcall unknown_darken_bevel_5fe9b0(Rect const &rect, Surface *surface, int width, int height)
{
    unsigned short *pixels = (unsigned short *)surface->Lock();
    if (pixels != 0) {
        int stride = surface->Stride() / 2;
        for (int y = rect.Y; y < rect.Y + rect.Height; ++y) {
            int index = y * stride + rect.X;
            for (int x = rect.X; x < width + rect.X; ++x) {
                unsigned short pixel = pixels[index];
                pixels[index] = (pixel >> 1) & (((unknown_red_mask_a763dc >> 1) & (unsigned int)unknown_red_mask_a763dc) | ((unknown_green_mask_a763de >> 1) & unknown_green_mask_a763de) | (unknown_blue_mask_a763e0 >> 1));
                index++;
            }
        }
        for (int y2 = rect.Y; y2 < rect.Y + height; ++y2) {
            int index = y2 * stride + rect.X + width;
            for (int x = rect.X + width; x < rect.Width + rect.X; ++x) {
                unsigned short pixel = pixels[index];
                pixels[index] = (pixel >> 1) & (((unknown_red_mask_a763dc >> 1) & (unsigned int)unknown_red_mask_a763dc) | ((unknown_green_mask_a763de >> 1) & unknown_green_mask_a763de) | (unknown_blue_mask_a763e0 >> 1));
                index++;
            }
        }
        surface->Unlock();
    }
}

}
