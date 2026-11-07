#define RA2_COORD_EQUAL_MEMBER
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Planning_Unlist_Techno(TechnoClass *techno) (0x00614550)
// From ra2-decomp (YR 0x006377B0).
// Takes a techno out of the planning list, lowering its house's counter.
#include "shared/planning.hpp"

void __fastcall Planning_Unlist_Techno(TechnoClass *techno)
{
    int house = Planning_House(techno);
    Planning_Uncount(house);
    PlanningLists::unknown_list_00a76700.Delete(techno);
}
