// Behaviour-written bodies of unmatched surface rows reached on the start-up
// path. NOT EXACT: the rows stay unrecovered in the exact tree. Evidence: TibSun
// code/xsurface.cpp (GPL-3.0-or-later, EA section 7 terms; THIRD_PARTY.md).
#include "shared/surface.hpp"
#include "shared/crt_memory.hpp"

// XSurface::Fill_Rect(cliprect, fillrect, color) (slot 4; its slot-5 sibling
// 0x00774390 is accepted and calls it). TibSun xsurface.cpp: clip the request
// to the surface, fill fillrect (relative to cliprect) row by row; one byte or
// one 16-bit pixel per point. TibSun's aligned two-pixel stores (its asm
// surface_quick_fill) only speed up the same writes.
bool XSurface::Fill_Rect(Rect const &cliprect, Rect const &fillrect, int color)
{
    if (!fillrect.Is_Valid()) return false;
    Rect wrect = cliprect.Intersect(Get_Rect());
    Rect biased(fillrect.X + cliprect.X, fillrect.Y + cliprect.Y, fillrect.Width, fillrect.Height);
    Rect crect = wrect.Intersect(biased);
    if (!crect.Is_Valid()) return false;
    int pitch = Stride();
    void *buffer = Lock(Point2D(crect.X, crect.Y));
    if (buffer == 0) return false;
    if (Bytes_Per_Pixel() == 1) {
        for (int y = 0; y < crect.Height; y++) {
            memset(buffer, color, crect.Width);
            buffer = (unsigned char *)buffer + pitch;
        }
    } else {
        for (int y = 0; y < crect.Height; y++) {
            for (int x = 0; x < crect.Width; x++) ((unsigned short *)buffer)[x] = (unsigned short)color;
            buffer = (unsigned char *)buffer + pitch;
        }
    }
    Unlock();
    return true;
}
