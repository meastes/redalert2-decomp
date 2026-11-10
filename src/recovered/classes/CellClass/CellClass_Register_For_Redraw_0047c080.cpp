// cell's three screen rectangles (0x004752B0, 0x00474EB0, 0x00475100) moved
// back by the tactical area origin (0x00839630, through Rect::operator- 0x0047D000), and marks the cell for the
// radar (RadarClass 0x00631570).
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/terrain.hpp"
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?Cell_Render_Rect@CellClass@@QBE?AVRect@@XZ=004752b0
// @ref ?Overlay_Render_Rect@CellClass@@QBE?AVRect@@XZ=00474eb0
// @ref ?Overlay_Shadow_Render_Rect@CellClass@@QBE?AVRect@@XZ=00475100

void CellClass::Register_For_Redraw()
{
    Rect rect = Union(Union(Cell_Render_Rect(), Overlay_Render_Rect()), Overlay_Shadow_Render_Rect());
    unknown_tactical_83996c->Register_Dirty_Area(rect - unknown_rect_839630.Top_Left(), 0);
    Map.Radar_Background(MapCoords);
}
