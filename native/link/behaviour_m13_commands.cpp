// Native adapter for the M13 input path (Scout1115). The keyboard commands' small
// virtuals (UICmnds/Init.CPP 0x0051AE70-0x0051DA30) and TabClass slot 33 0x006A20B0
// that were behaviour rows here are accepted typed source since Scout1120 r2/r3.
#include "shared/mouse_class.hpp"
#include "shared/object.hpp"

extern MouseClass Map;                 // 0x008324E0 (as shared/map_view.hpp declares it)

// Native adapter (not a row): RadarClass slot 51 0x0062FBD0 (the cell under a screen point)
// on Map, for the Set_Cursor_Pos draft 0x004994D0 whose unit sees a light MouseClass.
Cell Native_Radar_Cell_At(Point2D const &xy)
{
    return Map.Click_Cell_Calc(xy);
}
