// Cell Tactical::Unknown_006A6BD0(Point2D const &point) (0x006A6BD0): the cell under a
// screen point: scans up to 180 pixels below the point for the first cell whose
// lifted top lies at or above it, allowing for bridges (TibSun Tactical::Pixel_To_Cell).
// Unlike TibSun's early returns, the found cell is stored into the fallback and the scan
// breaks to one shared return: the original copies every result through one register.
#include "shared/tactical.hpp"
#include "shared/cell.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ??D@YI?AUVector3@@ABVMatrix3D@@ABU0@@Z=00591380

Cell Tactical::Unknown_006A6BD0(Point2D const &point)
{
    Point2D lepton = Unknown_006A2900(Point2D(TacPixelX, TacPixelY) + point - Point2D(unknown_rect_839630.X, unknown_rect_839630.Y));

    Cell fallback(lepton.X / 256, lepton.Y / 256);

    // The pixel of the point's offset within its cell (TibSun Coord_Fraction); never used.
    Point2D abspixel;
    Coord_To_Pixel(CoordStruct(lepton.X & 255, lepton.Y & 255, 0), abspixel);
    Point2D xy = point - Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);

    Point2D scan = xy;
    scan.Y += 180;

    int count = 0;
    while (count < 180) {
        abspixel = scan + Point2D(TacPixelX, TacPixelY);

        Point2D scanlepton = Unknown_006A2900(abspixel);
        Cell cell(scanlepton.X / 256, scanlepton.Y / 256);

        CellClass *cellptr = &Map[cell];
        int celly = scan.Y - cellptr->unknown_11b * 15;

        if (cellptr->unknown_140 & 0x100) {
            CellClass &east = cellptr->Adjacent_Cell(2);
            CellClass &south = cellptr->Adjacent_Cell(4);

            bool bridge1 = (cellptr->unknown_140 & 0x800) && !(cellptr->Adjacent_Cell(0).unknown_140 & 0x100);
            bool bridge2 = !(cellptr->unknown_140 & 0x800) && !(cellptr->Adjacent_Cell(6).unknown_140 & 0x100);
            bool bridge3 = (cellptr->unknown_140 & 0x800) && !(south.unknown_140 & 0x100);
            bool bridge4 = !(cellptr->unknown_140 & 0x800) && !(east.unknown_140 & 0x100);
            bool bridge5 = (cellptr->unknown_140 & 0x800) && abs(cellptr->unknown_11b - east.unknown_11b) <= 1 && !(east.unknown_140 & 0x100);
            bool bridge6 = !(cellptr->unknown_140 & 0x800) && abs(cellptr->unknown_11b - south.unknown_11b) <= 1 && !(south.unknown_140 & 0x100);

            CoordStruct topcoord = Cell_Coord(cellptr->MapCoords) - CoordStruct(128, 128, 0);
            Point2D top;
            Coord_To_Pixel(topcoord, top);
            top.Y += 30 * cellptr->unknown_11b / -2;

            if (top.Y <= xy.Y) {
                if (bridge3 || bridge6) {
                    fallback = Cell(cell.X, cell.Y + 1);
                    break;
                }
                if (bridge4 || bridge5) {
                    fallback = Cell(cell.X + 1, cell.Y);
                    break;
                }
            }

            if (bridge1 || bridge2) {
                int dx = xy.X - top.X;
                int dy = xy.Y - top.Y;
                if ((bridge1 && dy - dx / 2 > 15) || (bridge2 && dx / 2 + dy > 15)) {
                    celly -= ((cellptr->unknown_140 & 0x100) ? 60 : 0);
                }
            } else {
                celly -= ((cellptr->unknown_140 & 0x100) ? 60 : 0);
            }
        }

        if (celly <= xy.Y) {
            fallback = cell;
            break;
        }

        scan.Y--;
        count++;
    }

    return fallback;
}
