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
// @ref ?DSurface_RedRight@@3HA=00852fb8
// @ref ?DSurface_RedLeft@@3HA=00852fbc
// @ref ?DSurface_BlueRight@@3HA=00852fc0
// @ref ?DSurface_BlueLeft@@3HA=00852fc4
// @ref ?DSurface_GreenRight@@3HA=00852fc8
// @ref ?DSurface_GreenLeft@@3HA=00852fcc
