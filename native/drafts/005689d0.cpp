// NON-EXACT DRAFT 0x5689D0 FUN_005689d0
// entry 0x5689D0; exact-match score 60.0 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout664 (build/class-work/Scout664/w/keep/ce_6000.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Cell MapClass::Closest_Edge_Cell(Cell const &cell, bool inset) (0x005689D0)
// The cell on the local view's nearest edge to a cell (one cell inside it
// when inset; TibSun Closest_Edge_Cell): the cell's local view point is
// moved to the edge on the longer axis from the view's centre.
#include "shared/map.hpp"
extern "C" int __cdecl abs(int value);

Cell MapClass::Unknown_005689D0(Cell const &cell, bool inset)
{
    Point2D point = Unknown_00547DB0(Point2D(cell.X, cell.Y));
    point = point - MapLocalSize.Top_Left();

    Point2D center;
    center.Y = MapLocalSize.Height / 2;
    center.X = MapLocalSize.Width / 2;
    int x = point.X - center.X;
    int y = point.Y - center.Y;

    if (abs(x) < abs(y)) {
        if (point.X < center.X) {
            point.X = inset ? 1 : 0;
        } else {
            point.X = inset ? MapLocalSize.Width - 1 : MapLocalSize.Width;
        }
    } else {
        if (point.Y < center.Y) {
            point.Y = inset ? 1 : 0;
        } else {
            point.Y = inset ? MapLocalSize.Height - 1 : MapLocalSize.Height;
        }
    }

    Point2D result = Unknown_00547D70(point);
    return Cell(result.X, result.Y);
}
