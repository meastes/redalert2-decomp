// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Unknown_00753F10() (0x00753F10)
// Clears the WOL channel state: the lobby, game and user lists, the current and pending channel.
// From ra2-decomp 0x00799CD0 (YR twin FUN_00799CD0, src/wonline.cpp).
#include "shared/wol_sink.hpp"
#include "shared/wol_main.hpp"
extern bool unknown_7fd354;
// @ref ?unknown_7fd354@@3_NA=007fd354

void Unknown_00753F10()
{
    unknown_lobbies_b26c88.setEmpty();
    memset(&unknown_channel_b299c0, 0, sizeof(WOL::Channel));
    memset(&unknown_pending_channel_b26a60, 0, sizeof(WOL::Channel));
    unknown_7fd354 = true;
    unknown_games_b29990.setEmpty();
    unknown_users_b2a728.setEmpty();
    unknown_b2abe8 = 0;
}
