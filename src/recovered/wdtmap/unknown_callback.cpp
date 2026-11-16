// bool __stdcall unknown_callback_6f3650(Surface *surface, Rect const &window) -- 0x00729200
// Purpose: while the flag at 0x007FBD04 is set, draws the interface frame
// (0x006F3650 with true) on the surface and window it is given; returns false.
// Assumptions: the flag is unidentified.
// @ref ?FrameAdvanceDraws@@3_NA=007fbd04
// @ref ?Unknown_Draw_Interface_006F3650@@YIXPAVSurface@@ABVRect@@_N@Z=006f3650
#include "shared/rect.hpp"

extern bool FrameAdvanceDraws;
class Surface;
void __fastcall Unknown_Draw_Interface_006F3650(Surface *surface, Rect const &window, bool arg3);  // 0x006F3650

bool __stdcall unknown_callback_6f3650(Surface *surface, Rect const &window)
{
    if (FrameAdvanceDraws) {
        Unknown_Draw_Interface_006F3650(surface, window, true);
    }
    return false;
}
