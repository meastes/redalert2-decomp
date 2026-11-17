// void Tactical::AI() -- 0x006A2DD0 (slot 23)
// Purpose: each new frame, advances the scroll animation from unknown_c8 to
// unknown_d0 (unknown_dc is the progress, unknown_d8 the step), moves the
// view position through the clamp 0x006A8C80, restarts the update-count timer
// at Rules->WaypointAnimationSpeed once it ran out, and applies a requested
// view position (unknown_d74) when the animation did not move it.
// Assumptions: unknown_a40d04 / Debug_Map are game flags of unknown
// meaning; the comparison with 0x00ABE540 is by value.
#include "shared/tactical.hpp"
#include "shared/rules.hpp"
#include "shared/unknown_globals.hpp"
// @ref ?Clamp_To_Tactical_Rect@Tactical@@QAE_NPAUPoint2D@@@Z=006a8c80
// @ref ?Update_Visible_Cells@Tactical@@QAEXXZ=006a9170
// @ref ?Lerp@@YI?AUPoint2D@@ABU1@0M@Z=00722140
// @ref ?unknown_abe540@@3UPoint2D@@A=00abe540/00abe544

extern Point2D unknown_abe540;    // 0x00ABE540 (the header static unknown_geometry_l, read as a point)
Point2D __fastcall Lerp(Point2D const &from, Point2D const &to, float t);

void Tactical::AI()
{
    bool moved = false;
    if (unknown_flags_a3f600 & 2) {
        return;
    }
    if (LastAIFrame != Frame) {
        if (ScenarioActive && MoveTo != unknown_abe540 && MoveSpeed != 0.0f) {
            MoveFactor = MoveFactor + MoveSpeed;
            if (MoveFactor > 1.0) {
                MoveFactor = 1.0f;
            }
            Point2D pos = Lerp(MoveFrom, MoveTo, MoveFactor);
            if (MoveFactor >= 1.0) {
                MoveTo = Point2D(0, 0);
                MoveFrom = Point2D(0, 0);
                MoveSpeed = 0;
                MoveFactor = 0;
            }
            Point2D point = pos;
            int x;
            int y;
            if (Clamp_To_Tactical_Rect(&point) && !Debug_Map) {
                x = point.X;
                y = point.Y;
            } else {
                x = pos.X;
                y = pos.Y;
            }
            TacticalCoord = Point2D(x, y);
            DesiredTacticalCoord = Point2D(x, y);
            Update_Visible_Cells();
            IsToRedraw = true;
            moved = true;
        }
        if (WaypointAnimTimer.Value() == 0) {
            WaypointAnimTimer = CDTimerClass<SystemTimerClass>(Rules->WaypointAnimationSpeed);
            WaypointAnimCounter++;
        }
    }
    LastAIFrame = Frame;
    if (DesiredTacticalCoord != TacticalCoord && !moved) {
        Point2D point = DesiredTacticalCoord;
        int x;
        int y;
        if (Clamp_To_Tactical_Rect(&point) && !Debug_Map) {
            x = point.X;
            y = point.Y;
        } else {
            x = DesiredTacticalCoord.X;
            y = DesiredTacticalCoord.Y;
        }
        TacticalCoord = Point2D(x, y);
        DesiredTacticalCoord = Point2D(x, y);
        Update_Visible_Cells();
        IsToRedraw = true;
    }
}
