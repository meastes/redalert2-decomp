// bool Tactical::Add_To_Selectables(ObjectClass *object, Point2D point) (0x006AA510): lists an
// object with its pixel (point plus the view position) in the table at 0x00ABE5F8
// when the table has room and the point is within 32 pixels of the tactical area
// (hypothesis; 0x006AA300 expands it).
#include "shared/tactical.hpp"
#include "shared/display.hpp"
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?unknown_tactical_pixels_abe5f8@@3PAUTacticalObjectPixel@@A=00abe5f8

bool Tactical::Add_To_Selectables(ObjectClass *object, Point2D point)
{
    if (unknown_db0 != 500 && point.X >= -32 && point.X <= unknown_rect_839630.Width + 32 && point.Y >= -32 && point.Y <= unknown_rect_839630.Height + 32) {
        unknown_tactical_pixels_abe5f8[unknown_db0].Object = object;
        Point2D view(TacPixelX, TacPixelY);
        unknown_tactical_pixels_abe5f8[unknown_db0].Pixel = Point2D(point.X + view.X, point.Y + view.Y);
        unknown_db0++;
        return true;
    }
    return false;
}
