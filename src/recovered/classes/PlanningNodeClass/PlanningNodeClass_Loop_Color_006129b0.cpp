// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#define RA2_COORD_EQUAL_MEMBER
// int PlanningNodeClass::Loop_Color() (0x006129B0)
// The colour of the node's own branch (cached in its unknown_74): the house
// colour of the first active member still short of the loop, moved toward
// white; once every active member is past it, a shade closer to white.
// from ra2-decomp 0x00635C10 (PlanningNodeClass::Loop_Color, src/planmgr.cpp)
#include "shared/planning.hpp"
#include "shared/hicolor.hpp"
// @ref ?unknown_rgb_ab6841@@3VRGBClass@@A=00ab6841
// @ref ?unknown_rgb_ab6849@@3VRGBClass@@A=00ab6849

int PlanningNodeClass::Loop_Color()
{
    if (Branch.unknown_74 != -1) return Branch.unknown_74;
    TechnoClass *last_owner = 0;
    int count = Members.Count();
    for (int i = 0; i < count; ++i) {
        PlanningMemberClass *member = Members[i];
        if (member->unknown_0c) {
            TechnoClass *owner = member->Owner;
            PlanningTokenClass *token = Planning_Token(owner);
            if (!token->unknown_98) {
                RGBClass color = Planning_House_Color(owner);
                color.Adjust(0x50, unknown_rgb_ab6841);
                return Branch.unknown_74 = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
            }
            last_owner = owner;
        }
    }
    if (last_owner == 0) {
        return -1;
    } else {
        RGBClass color = Planning_House_Color(last_owner);
        color.Adjust(0x64, unknown_rgb_ab6849);
        return Branch.unknown_74 = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
    }
}
