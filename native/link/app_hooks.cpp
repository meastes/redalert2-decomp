// Mac app hooks (platform/sdl/host.cpp declares weak defaults; these strong
// definitions replace them). SUGGESTION from the platform lane (Scout1195), not
// part of the platform patch: native/link belongs to the link lane.
//   ra2_native_scroll_map: trackpad panning over the tactical view. The view
//     position is in pixels (Tactical +0xD64): read it (0x006A67B0), move it by
//     the fingers' pixels and set it back clamped to the map (0x006A6640, which
//     also flags the redraw).
//   ra2_native_save_settings: Cmd+Q saves RA2.INI as the main menu's Exit Game
//     does (Select_Game 0x005135F0: Options.Save_Settings 0x005D91B0).
#include "shared/tactical.hpp"
#include "shared/options.hpp"

extern "C" int ra2_native_scroll_map(int, int, int dx, int dy)
{
    Tactical *tactical = unknown_tactical_83996c;
    if (!tactical) return 0;
    Point2D view = tactical->Get_Tactical_Position();
    view.X += dx;
    view.Y += dy;
    tactical->Set_Tactical_Position_006a6640(view);
    return 1;
}

extern "C" void ra2_native_save_settings(void)
{
    Options.Save_Settings();
}
