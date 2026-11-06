// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#define RA2_COORD_EQUAL_MEMBER
// int PlanningNodeClass::Branch_Color(int index) (0x006127E0)
// The colour of branch index (cached in the branch): the house colour of the
// first member counted in it that has not reached this node, moved toward
// white; once every such member has, a shade closer to white.
// from ra2-decomp 0x00635A40 (PlanningNodeClass::Branch_Color, src/planmgr.cpp)
#include "shared/planning.hpp"
#include "shared/hicolor.hpp"
// @ref ?unknown_rgb_ab6841@@3VRGBClass@@A=00ab6841
// @ref ?unknown_rgb_ab6849@@3VRGBClass@@A=00ab6849

int PlanningNodeClass::Branch_Color(int index)
{
    PlanningBranchClass *branch = Branches[index];
    if (branch->unknown_74 != -1) return branch->unknown_74;
    TechnoClass *last_owner = 0;
    int count = Members.Count();
    for (int i = 0; i < count; ++i) {
        PlanningMemberClass *member = Members[i];
        if (member->BranchIndex == index) {
            TechnoClass *owner = member->Owner;
            PlanningTokenClass *token = Planning_Token(owner);
            if (!token->Reached(token->Nodes.ID(this))) {
                RGBClass color = Planning_House_Color(owner);
                color.Adjust(0x50, unknown_rgb_ab6841);
                return branch->unknown_74 = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
            }
            last_owner = owner;
        }
    }
    if (last_owner == 0) {
        return -1;
    } else {
        RGBClass color = Planning_House_Color(last_owner);
        color.Adjust(0x64, unknown_rgb_ab6849);
        return branch->unknown_74 = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
    }
}
