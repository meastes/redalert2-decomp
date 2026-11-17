// void Tactical::Set_View_Dimensions_006a65a0(Rect const &rect) (0x006A65A0; DisplayClass slot 42)
// Sets the view rectangle (kept in the global at 0x00ABE560, read by the tactical drawing) and re-clamps the
// requested view position through 0x006A8C80, which becomes the view position.
// @ref ?Clamp_To_Tactical_Rect@Tactical@@QAE_NPAUPoint2D@@@Z=006a8c80
// @ref ?Update_Visible_Cells@Tactical@@QAEXXZ=006a9170
// @ref ?Debug_Map@@3_NA=00a40d13
// @ref ?TacticalDimensions@@3VRect@@A=00abe560
#include "shared/tactical.hpp"
#include "shared/unknown_globals.hpp"

extern Rect TacticalDimensions;

void Tactical::Set_View_Dimensions_006a65a0(Rect const &rect)
{
    TacticalDimensions = rect;
    Point2D clamped = DesiredTacticalCoord;
    int x;
    int y;
    if (Clamp_To_Tactical_Rect(&clamped) && !Debug_Map) {
        x = clamped.X;
        y = clamped.Y;
    } else {
        x = DesiredTacticalCoord.X;
        y = DesiredTacticalCoord.Y;
    }
    TacticalCoord = Point2D(x, y);
    DesiredTacticalCoord = Point2D(x, y);
    Update_Visible_Cells();
    IsToRedraw = true;
}
