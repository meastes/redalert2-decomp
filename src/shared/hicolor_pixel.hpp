// Screen pixel <-> RGBClass helpers kept out of line as COMDATs (0x00432D90 after BitText.cpp; also called
// from the planning unit 0x00618C00/0x0061FC20). Names are placeholders.
#ifndef RA2_SHARED_HICOLOR_PIXEL_HPP
#define RA2_SHARED_HICOLOR_PIXEL_HPP

#include "shared/hicolor.hpp"

inline RGBClass Pixel_To_RGB(unsigned short pixel)
{
    return RGBClass((pixel >> DSurface_RedRight) << DSurface_RedLeft,
                    (pixel >> DSurface_GreenRight) << DSurface_GreenLeft,
                    (pixel >> DSurface_BlueRight) << DSurface_BlueLeft);
}

inline int __fastcall RGB_To_Pixel(int red, int green, int blue)
{
    return ((red >> DSurface_RedLeft) << DSurface_RedRight)
         | ((green >> DSurface_GreenLeft) << DSurface_GreenRight)
         | ((blue >> DSurface_BlueLeft) << DSurface_BlueRight);
}

#endif
