// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#define RA2_COORD_EQUAL_MEMBER
// void PlanningTokenClass::Copy_Nodes_To(TechnoClass *techno, int start, int count) (0x00613550)
// Adds the nodes start..start+count-1 of this path to techno's path: each
// node gets a member for techno with a copy of this owner's order there,
// aimed at techno.
// Shape from ra2-decomp 0x006367B0 (PlanningTokenClass::Share_Range, src/planmgr.cpp).
#include "shared/planning.hpp"

void PlanningTokenClass::Copy_Nodes_To(TechnoClass *techno, int start, int count)
{
    if (Planning_Token(techno) == 0) {
        return;
    }
    for (int i = start; i < start + count; i++) {
        PlanningNodeClass *node = Nodes[i];
        EventClass event = node->Event_For(Owner);
        event.TargetAt(0x07) = TargetClass(techno);
        node->Add_Member(techno, event);
    }
}
