// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// WideString WorldDominationTour::MapSizeGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const (0x00728AB0)
// Adapted from ra2-decomp src/wdtgameoptions.cpp MapSizeGameOption::Get_String (gamemd twin 0x00767740,
// matched there); see THIRD_PARTY.md.
// @ref ??0WideString@@QAE@PBG@Z=0076f9c0
// @ref ??1WideString@@QAE@XZ=0076fa50
// @ref ??H@YI?AVWideString@@ABV0@0@Z=00771460
// @ref ??HWideString@@QBE?AV0@PBG@Z=0076fed0
// @ref ??_C@_15MHMA@?$AA?0?$AA?5?$AA?$AA@=007f6520
// @ref ?Length@WideString@@QBEIXZ=007704a0
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ?MapSize_Labels@WorldDominationTour@@3QBQBDB=007af140
#include "shared/wdt_game_options.hpp"

namespace WorldDominationTour {

WideString MapSizeGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const
{
    if (prefix.Length() > 0) {
        return operator+(prefix, L", ") +
               StringTable::Load_String(MapSize_Labels[territory.Get()->unknown_12], 0, "D:\\Ra2\\WDTGameOptions.cpp", 129);
    }
    return WideString(
        StringTable::Load_String(MapSize_Labels[territory.Get()->unknown_13], 0, "D:\\Ra2\\WDTGameOptions.cpp", 131));
}

}
