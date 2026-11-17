// Expand a screen-format pixel into RGB channels,00536ED0.
#include "shared/hicolor.hpp"
RGBClass __fastcall Pixel_To_RGB_00536ed0(unsigned short pixel)
{
    RGBClass color;
    color.Red = (pixel >> DSurface_RedRight) << DSurface_RedLeft;
    color.Green = (pixel >> DSurface_GreenRight) << DSurface_GreenLeft;
    color.Blue = (pixel >> DSurface_BlueRight) << DSurface_BlueLeft;
    return color;
}
