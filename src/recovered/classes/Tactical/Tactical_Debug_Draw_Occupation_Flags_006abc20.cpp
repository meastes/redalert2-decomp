// void Tactical::Debug_Draw_Occupation_Flags() (0x006ABC20): for every cell in view, a 4x4 block in yellow for each
// occupation bit it carries, the ground's (Occupy_124) beside the cell centre and the bridge deck's (Occupy_128) 60
// pixels higher (TibSun Tactical::Debug_Draw_Occupation_Flags). The centre goes through the inlined Coord_To_Pixel,
// as in Debug_Draw_Occupier_Links: its Coord_To_Pixel_Absolute 0x006A27A0 stays out of line one level down.
#include "shared/tactical.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/surface.hpp"
#include "shared/hicolor.hpp"
#include "shared/unknown_globals.hpp"
extern Rect unknown_rect_839630;
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?Intersect@@YI?AVRect@@ABV1@0PAH1@Z=0041fdf0
// @ref ??HRectPoint2D@@QBE?BU0@ABU0@@Z=00435540
// @ref ??0Rect@@QAE@ABURectPoint2D@@HH@Z=00453e40
// @ref ?Coord_To_Pixel_Absolute@Tactical@@QAE?AUPoint2D@@ABUCoordStruct@@@Z=006a27a0

void Tactical::Debug_Draw_Occupation_Flags()
{
    Map.Unknown_0055A550();
    CellClass *cell = Map.Unknown_0055A490();
    int color = Build_Hicolor_Pixel(255, 255, 0);
    while (cell) {
        Rect render = cell->unknown_4752b0();
        Rect inter = Intersect(render, unknown_rect_839630);
        if (inter.Is_Valid()) {
            CoordStruct coord = cell->unknown_475d60();
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            Rect rect = Rect(pixel.X - 2, unknown_rect_839630.Y + pixel.Y - 2, 4, 4);
            if (cell->Occupy_124.NE) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(10, 0), color);
            }
            if (cell->Occupy_124.SE) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(0, 6), color);
            }
            if (cell->Occupy_124.SW) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(-10, 0), color);
            }
            if (cell->Occupy_124.Building || cell->Occupy_124.Monolith || cell->Occupy_124.Vehicle) {
                unknown_surface_83995c->Fill_Rect(rect, color);
            }
            if (cell->Occupy_128.NE) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(10, -60), color);
            }
            if (cell->Occupy_128.SE) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(0, -66), color);
            }
            if (cell->Occupy_128.SW) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(-10, -60), color);
            }
            if (cell->Occupy_128.Building || cell->Occupy_128.Monolith || cell->Occupy_128.Vehicle) {
                unknown_surface_83995c->Fill_Rect(rect + Point2D(0, -60), color);
            }
        }
        cell = Map.Unknown_0055A490();
    }
}
