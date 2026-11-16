// unsigned __fastcall SurfaceCacheConvertPixel(int green, int red, int blue) (0x0068BC70)
// Packs a color into a screen pixel through the channel shifts of shared/hicolor.hpp: red is scaled by 1.5
// (clamped to 255) and blue is divided by 4 first (hypothesis: a palette-fade or tint helper).
#include "shared/hicolor.hpp"

unsigned __fastcall SurfaceCacheConvertPixel(int green, int red, int blue)
{
    int boosted = red * 15 / 10;
    int red_clamped = boosted > 255 ? 255 : boosted;
    return (red_clamped >> DSurface_RedLeft) << DSurface_RedRight
         | (green >> DSurface_GreenLeft) << DSurface_GreenRight
         | ((blue / 4) >> DSurface_BlueLeft) << DSurface_BlueRight;
}
