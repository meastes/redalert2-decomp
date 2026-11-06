// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#define RA2_COORD_EQUAL_MEMBER
// void PlanningTokenClass::Copy_Nodes_To(TechnoClass *techno, int start) (0x00613720)
// Copy_Nodes_To(techno, start, Nodes.Count() - start): adds the nodes from
// start of this path to techno's path.
// Shape from ra2-decomp 0x00636980 (PlanningTokenClass::Share_From, src/planmgr.cpp).
#include "shared/planning.hpp"

void PlanningTokenClass::Copy_Nodes_To(TechnoClass *techno, int start)
{
    int count = Nodes.Count() - start;
    Copy_Nodes_To(techno, start, count);
}
