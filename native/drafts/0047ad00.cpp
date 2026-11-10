// void CellClass::Place_Veins() (0x0047AD00): TibSun Place_Veins.
// NATIVE (Scout1209 r16): Scout1166 header-overlay names mapped to the current src/shared names
// (unknown_47a670 Can_Place_Veins, unknown_474eb0 Overlay_Render_Rect, unknown_475100
// Overlay_Shadow_Render_Rect, MapClass::Unknown_00631570 Radar_Background).
#include "shared/cell.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/overlay_type.hpp"
#include "shared/id_source.hpp"

// @ref ?OverlayTypes@@3V?$DynamicVectorClass@PAVOverlayTypeClass@@@@A=00a35de8

inline bool Is_Solid_Vein(CellClass const &cell)
{
    return cell.OverlayIndex == 0x7E && cell.OverlayData >= 0x30;
}

void CellClass::Place_Veins()
{
    Rect rect(0, 0, 0, 0);

    if (Can_Place_Veins()) {
        rect = Union(rect, Union(Overlay_Render_Rect(), Overlay_Shadow_Render_Rect()));

        Map.Radar_Background(MapCoords);
        OverlayIndex = 0x7E;

        if (unknown_11c == 0) {
            OverlayData = abs(unknown_id_source_a3d290->Random()()) % 3 + 0x30;

            int dir;
            for (dir = 0; dir < 8; dir += 2) {
                CellClass &adjacent = (CellClass &)((CellClass const *)this)->Adjacent_Cell(dir);

                if (Is_Solid_Vein(adjacent)) {
                    continue;
                }
                if (adjacent.OverlayIndex != 0x7E && adjacent.OverlayIndex != -1 && OverlayTypes[adjacent.OverlayIndex]->IsVeins) {
                    continue;
                }

                if (adjacent.unknown_11c == 0) {
                    int frame = adjacent.Get_Vein_Frame();
                    if (adjacent.OverlayIndex == -1 || (frame <= 15 && adjacent.OverlayData / 3 != frame)) {
                        Rect adjrect(0, 0, 0, 0);
                        adjrect = Union(adjrect, Union(adjacent.Overlay_Render_Rect(), adjacent.Overlay_Shadow_Render_Rect()));
                        adjacent.OverlayIndex = 0x7E;
                        adjacent.OverlayData = 3 * frame + abs(unknown_id_source_a3d290->Random()() % 3);
                        adjacent.Recalc_Attributes(-1);
                        RectPoint2D origin = unknown_rect_839630.Top_Left();
                        adjrect = Union(adjrect, Union(adjacent.Overlay_Render_Rect(), adjacent.Overlay_Shadow_Render_Rect())) - origin;
                        unknown_tactical_83996c->Register_Dirty_Area(adjrect, 0);
                    }
                } else {
                    Rect adjrect(0, 0, 0, 0);
                    adjrect = Union(adjrect, Union(adjacent.Overlay_Render_Rect(), adjacent.Overlay_Shadow_Render_Rect()));
                    adjacent.OverlayIndex = 0x7E;
                    adjacent.OverlayData = 2 * adjacent.unknown_11c + (abs(unknown_id_source_a3d290->Random()()) & 1) + 0x33;
                    adjrect = Union(adjrect, Union(adjacent.Overlay_Render_Rect(), adjacent.Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
                    unknown_tactical_83996c->Register_Dirty_Area(adjrect, 0);
                }

                Map.Radar_Background(adjacent.MapCoords);
            }

            unknown_47bb30();
        } else {
            OverlayData = 2 * unknown_11c + (abs(unknown_id_source_a3d290->Random()()) & 1) + 0x33;
        }

        rect = Union(rect, Union(Overlay_Render_Rect(), Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(rect, 0);
        Recalc_Attributes(-1);
    }
}
