// 0043EBA0: replace the footprint with rubble and optionally leave a crate.
#include "shared/building.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
#include "shared/cell.hpp"
#include "shared/tactical.hpp"
extern Rect unknown_rect_839630; // existing declaration in shared/display.hpp
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ??0Rect@@QAE@ABURectPoint2D@@HH@Z=00453e40
static inline bool Is_List_End(Cell const *offset);
void BuildingClass::Leave_Rubble_0043eba0()
{
    void *image;
    int frame;
    if (Type->unknown_func_458e00(&image, &frame)) {
        Cell const *offset = unknown_slot64(0);
        Cell origin = Coord_Cell(unknown_slot42());
        while (!Is_List_End(offset)) {
            Cell position = *offset++ + origin;
            CellClass &cell = Map[position];
            cell.Register_For_Redraw();
            cell.OverlayIndex = 0xef;
            cell.unknown_40 = 0;
            cell.Recalc_Attributes(-1);
            Map.Update_Cell_Zone(cell.MapCoords);
            Map.Unknown_00566340(cell.MapCoords);
            Unknown_006D7C80(&cell);
        }
        CellClass &base = Map[origin];
        base.unknown_40 = Type;
        Rect area = Union(base.Overlay_Render_Rect(), base.Overlay_Shadow_Render_Rect());
        Point2D view_origin(unknown_rect_839630.X, unknown_rect_839630.Y);
        Point2D translated(area.X - view_origin.X, area.Y - view_origin.Y);
        unknown_tactical_83996c->Register_Dirty_Area(Rect(RectPoint2D(translated.X, translated.Y), area.Width, area.Height), 0);
        unknown_tactical_83996c->Flag_Cell(&Map[origin]);
    }
    if (Type->CrateBeneath) {
        Cell position = Coord_Cell(unknown_slot42());
        if (Type->CrateBeneathIsMoney) {
            Map.Unknown_0054E580(position, 0);
            return;
        }
        Map.Unknown_0054E580(position, 0x14);
    }
}
