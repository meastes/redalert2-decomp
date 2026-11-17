// NON-EXACT DRAFT 0x68CCE0 FUN_0068cce0
// entry 0x68CCE0; exact-match score 99.37 (misses_index best); plan "draft as-is"
// origin lane: Scout782 (build/class-work/Scout782/w/bw_9937.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// bool SurfaceCacheClass::Draw_0068cce0(Rect const &, Surface *, Surface *, int, int) (0x0068CCE0):
// tiles the 16-bit image over rect of destination, centred when the image is the larger, from the
// source offset.
#include "shared/pcx_cache.hpp"
#include "shared/surface.hpp"

bool SurfaceCacheClass::Draw_0068cce0(Rect const &rect, Surface *destination, Surface *image, int source_x, int source_y)
{
    unsigned short *target = (unsigned short *)destination->Lock();
    if (target == 0) {
        return false;
    }
    unsigned short *source = (unsigned short *)image->Lock();
    if (source == 0) {
        destination->Unlock();
        return false;
    }
    int target_pitch = destination->Stride() / 2;
    int source_pitch = image->Stride() / 2;
    Rect image_rect = image->Get_Rect();
    Rect destination_rect = destination->Get_Rect();
    int x_margin = (image_rect.Width - rect.Width) / 2;
    x_margin = x_margin > 0 ? x_margin : 0;
    int y_margin = (image_rect.Height - rect.Height) / 2;
    y_margin = y_margin > 0 ? y_margin : 0;
    for (int row = 0; row < rect.Height; row++) {
        int offset = (row + rect.Y) * target_pitch + rect.X;
        for (int column = 0; column < rect.Width; column++) {
            target[offset + column] = source[(column + x_margin + source_x) % image_rect.Width + (row + y_margin + source_y) % image_rect.Height * source_pitch];
        }
    }
    image->Unlock();
    destination->Unlock();
    return true;
}
