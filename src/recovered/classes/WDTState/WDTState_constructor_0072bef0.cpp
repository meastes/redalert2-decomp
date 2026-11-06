// WDTState::WDTState() (0x0072BEF0): an empty state (map 0, cycle 0, a 1,000,000 tick time, no
// territories). The territory list is reset by assigning an empty temporary: the vector's
// assignment is expanded and calls VectorClass::operator= 0x0072CAC0, which needs the unit's
// RA2_VECTOR_SIGNED_SIZE profile (defined in WDTTerritory::Create 0x0072BDC0).
// From ra2-decomp 0x0076ABE0 (YR identical twin, src/wdtnet.cpp WDTState::WDTState):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// @ref ??_7WDTState@@6B@=007af308
// @ref ??_7?$DynamicVectorClass@V?$rc_ptr@VWDTTerritory@@@@@@6B@=007af318
// @ref ??_7?$VectorClass@V?$rc_ptr@VWDTTerritory@@@@@@6B@=007af2e8
// @ref ??4?$VectorClass@V?$rc_ptr@VWDTTerritory@@@@@@QAEAAV0@ABV0@@Z=0072cac0
#include "shared/wdt_state.hpp"
#include "shared/wdt_territory.hpp"

WDTState::WDTState()
{
    unknown_14 = 0;
    unknown_18 = 0;
    unknown_20 = 0;
    ConflictCount = 0;
    unknown_28 = 1000000;
    Territories = DynamicVectorClass<rc_ptr<WDTTerritory> >();
    unknown_44 = 0;
}
