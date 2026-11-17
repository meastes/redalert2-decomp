// void Tactical::Set_Tactical_Position_006a6640(Point2D const &point) (0x006A6640)
// Sets the view position to a point clamped by 0x006A8C80 (the point itself while the clamp refuses or the
// flag at 0x00A40D13 is set) and flags the view for a redraw.
// @ref ?Clamp_To_Tactical_Rect@Tactical@@QAE_NPAUPoint2D@@@Z=006a8c80
// @ref ?Update_Visible_Cells@Tactical@@QAEXXZ=006a9170
// @ref ?Debug_Map@@3_NA=00a40d13
#include "shared/tactical.hpp"
#include "shared/unknown_globals.hpp"

void Tactical::Set_Tactical_Position_006a6640(Point2D const &point)
{
    Point2D clamped = point;
    int x;
    int y;
    if (Clamp_To_Tactical_Rect(&clamped) && !Debug_Map) {
        x = clamped.X;
        y = clamped.Y;
    } else {
        x = point.X;
        y = point.Y;
    }
    TacticalCoord = Point2D(x, y);
    DesiredTacticalCoord = Point2D(x, y);
    Update_Visible_Cells();
    IsToRedraw = true;
}
