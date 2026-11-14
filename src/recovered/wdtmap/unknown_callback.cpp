// bool __stdcall unknown_callback_6f3650(Surface *surface, Rect const &window) -- 0x00729200
// Purpose: while the flag at 0x007FBD04 is set, draws the interface frame
// (0x006F3650 with true) on the surface and window it is given; returns false.
// Assumptions: the flag is unidentified.
// @ref ?unknown_flag_7fbd04@@3_NA=007fbd04
// @ref ?Unknown_006F3650@@YIXPAVSurface@@ABVRect@@_N@Z=006f3650
#include "shared/rect.hpp"

extern bool unknown_flag_7fbd04;
class Surface;
void __fastcall Unknown_006F3650(Surface *surface, Rect const &window, bool arg3);  // 0x006F3650

bool __stdcall unknown_callback_6f3650(Surface *surface, Rect const &window)
{
    if (unknown_flag_7fbd04) {
        Unknown_006F3650(surface, window, true);
    }
    return false;
}
