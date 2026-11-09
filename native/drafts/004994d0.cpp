// NON-EXACT DRAFT 0x4994D0 DisplayClass::Unknown_004994D0 (TibSun Set_Cursor_Pos)
// entry 0x4994D0; exact-match score 78.48 (misses_index best, Scout620/Scout1075).
// plan "draft, verify". Origin: build/class-work/Scout1075/keep/scp_k6_natural_7848.cpp.
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by Scout1115
// (native/drafts.json). Not original bytes. Reached by the placement cursor once a finished
// building is picked from the sidebar (native M13 interactive input).
// Native adaptation (Scout1115): the radar's cell under the mouse (RadarClass slot 51, which this
// unit's light MouseClass does not declare) through native/link/behaviour_m13_commands.cpp's
// Native_Radar_Cell_At; unknown_a40d13 is HEAD's Debug_Map (0x00A40D13). Marked NATIVE.
// Cell DisplayClass::Unknown_004994D0(Cell const &xpos) (0x004994D0)
// TibSun DisplayClass::Set_Cursor_Pos: moves the placement cursor (zone cell +0x1170) to a cell, or to the
// cell under the mouse for the null cell, re-marks the cursor list (+0x1178) and recomputes the proximity
// (+0x117C) and shroud (+0x117D) checks for the pending object (+0x11A4) and house (+0x11A8). Returns the old
// zone cell (the new one when it did not move).
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/mouse.hpp"
#include "shared/rect.hpp"
#include "shared/unknown_globals.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?unknown_cell_852610@@3VCell@@A=00852610
// @ref ?WWMouse@@3PAVMouse@@A=00839c68
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?unknown_a40d13@@3_NA=00a40d13
// @ref ?unknown_slot51@RadarClass@@QAE?AVCell@@ABUPoint2D@@@Z=0062fbd0
// @ref ?Unknown_006A6BD0@Tactical@@QAE?AVCell@@ABUPoint2D@@@Z=006a6bd0
// @ref ?Clamp_Cursor_To_Tactical@Tactical@@QAE?AVCell@@ABV2@PBV2@@Z=006aa970
// @ref ?Unknown_004998C0@DisplayClass@@QAEXABVCell@@_N@Z=004998c0
// @ref ?Passes_Proximity_Check_004991D0@DisplayClass@@QBE_NPBVObjectTypeClass@@HPBVCell@@ABV3@@Z=004991d0
// @ref ?Unknown_00499390@DisplayClass@@QAE_NPAVBuildingTypeClass@@HHPBVCell@@@Z=00499390

Cell Native_Radar_Cell_At(Point2D const &xy);   // NATIVE: RadarClass slot 51 on Map
Cell DisplayClass::Unknown_004994D0(Cell const &xpos)
{
    Cell prevpos;
    Cell pos = xpos;

    if (pos == unknown_cell_852610) {
        Point2D xy = WWMouse->Get_Mouse_XY();
        Point2D tl = Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);
        if (xy.X >= tl.X && xy.X < tl.X + unknown_rect_839630.Width && xy.Y >= tl.Y && xy.Y < tl.Y + unknown_rect_839630.Height) {
            Point2D mouse = WWMouse->Get_Mouse_XY();
            Cell click = Native_Radar_Cell_At(mouse);   // NATIVE: Map.unknown_slot51(mouse)
            if (click != unknown_cell_852610) {
                pos = click;
            }
        } else {
            Point2D mouse = WWMouse->Get_Mouse_XY();
            pos = unknown_tactical_83996c->Unknown_006A6BD0(mouse - tl);
        }
    }

    if (unknown_1178 == 0) {
        prevpos = unknown_1170;
        unknown_1170 = pos;
        return prevpos;
    }

    if (!Debug_Map) {   // NATIVE: unknown_a40d13
        pos = unknown_tactical_83996c->Clamp_Cursor_To_Tactical(pos + unknown_1174, unknown_1178) - unknown_1174;
    }

    if (pos == unknown_1170) return pos;

    prevpos = unknown_1170;

    if (unknown_1178 != 0) {
        if (pos != unknown_1170 && unknown_1170 != unknown_cell_852610) {
            Unknown_004998C0(unknown_1170 + unknown_1174, false);
        }
        if (pos != unknown_cell_852610) {
            Unknown_004998C0(pos + unknown_1174, true);
        }
    }
    unknown_1170 = pos;
    unknown_117c = Passes_Proximity_Check_004991D0(unknown_11a4, unknown_11a8, unknown_1178, unknown_1170 + unknown_1174);
    Cell cell = unknown_1170 + unknown_1174;
    unknown_117d = Unknown_00499390((BuildingTypeClass *)unknown_11a4, unknown_11a8, (int)unknown_1178, &cell);

    return prevpos;
}
