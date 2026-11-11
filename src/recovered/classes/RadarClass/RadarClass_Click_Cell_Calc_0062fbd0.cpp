// Cell RadarClass::Click_Cell_Calc(Point2D const &xy) (0x0062FBD0, slot 51)
// The cell under a screen point: the cached cell (0x00AB6330) when the point
// maps to the cached coordinate (0x00AB6370), else the tactical view's cell.
// Both branches on the flag at 0x00A40D13 compile to the same call.
#include "shared/radar.hpp"
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?Pixel_To_Coord@Tactical@@QAE?AUCoordStruct@@ABUPoint2D@@@Z=006a2b10
// @ref ?Pixel_To_Cell@Tactical@@QAE?AVCell@@ABUPoint2D@@@Z=006a6bd0
// @ref ?Debug_Map@@3_NA=00a40d13
// @ref ?unknown_cell_ab6330@RadarClass@@2VCell@@A=00ab6330
// @ref ?unknown_coord_ab6370@RadarClass@@2UCoordStruct@@A=00ab6370

Cell RadarClass::Click_Cell_Calc(Point2D const &xy)
{
    Point2D point = xy;
    CoordStruct coord = unknown_tactical_83996c->Pixel_To_Coord(point);
    if (coord != unknown_coord_ab6370) {
        if (Debug_Map) {
            return unknown_tactical_83996c->Pixel_To_Cell(point);
        }
        return unknown_tactical_83996c->Pixel_To_Cell(point);
    }
    return unknown_cell_ab6330;
}
