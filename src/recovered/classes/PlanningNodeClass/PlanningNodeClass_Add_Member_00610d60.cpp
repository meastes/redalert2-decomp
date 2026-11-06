// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#define RA2_COORD_EQUAL_MEMBER
// void PlanningNodeClass::Add_Member(TechnoClass *techno, EventClass const &event) (0x00610D60)
// Gives techno a member in this node with its own copy of the order and
// appends the node to techno's path.
// Shape from ra2-decomp 0x00633FA0 (PlanningNodeClass::Add_Member, src/planmgr.cpp).
#include "shared/planning.hpp"

void PlanningNodeClass::Add_Member(TechnoClass *techno, EventClass const &event)
{
    PlanningMemberClass *member = new PlanningMemberClass;
    member->Set_Owner(techno);
    EventClass *copy = new EventClass;
    *copy = event;
    member->Set_Event(copy);
    Members.Add(member);
    Planning_Token(techno)->Add_Node(this);
    unknown_611bd0(member);
}
