// void DisplayClass::unknown_slot45(Point2D const &xy) (0x0049C490, slot 45)
// The left mouse button went down at xy (hypothesis: Westwood's Mouse_Left_Press):
// with no mode active (+0x11AC, +0x11AE, +0x11AD clear, +0x11B4 -1) and nothing
// waiting for placement (+0x11A4), a selection band is anchored there (+0x11CC,
// its start +0x11D0 and last point +0x11D8).
#include "shared/display.hpp"

void DisplayClass::unknown_slot45(Point2D const &xy)
{
    if (!unknown_11ac && !unknown_11ae && !unknown_11ad && unknown_11b4 == -1 && unknown_11a4 == 0) {
        unknown_11cc = true;
        unknown_11d0 = xy;
        unknown_11d8 = xy;
    }
}
