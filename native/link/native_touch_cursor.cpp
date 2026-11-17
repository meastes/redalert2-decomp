// SPDX-License-Identifier: GPL-3.0-or-later
// The touch app hides the game's own cursor (Scout1249, not original code): on a
// touch screen the pointer only marks where the last touch was. Whenever the
// cursor is shown, one extra WWMouseClass::Hide_Mouse (0x00772CA0) leaves its
// nesting count below zero, so the game's balanced hide/show pairs keep it hidden;
// where the game forces it visible (shows until no longer hidden) the next call
// hides it again. Nothing else changes (the pointer position, the cursor shapes the
// game picks). The host (platform/sdl/host.cpp) calls it as it pumps events.
#include "shared/wwmouse.hpp"

extern "C" int ra2_native_hide_game_cursor(void)
{
    if (!_Mouse) return 0;
    if (!_Mouse->Is_Hidden()) _Mouse->Hide_Mouse();
    return 1;
}
