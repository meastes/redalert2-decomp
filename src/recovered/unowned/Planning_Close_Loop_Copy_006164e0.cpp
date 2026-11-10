// void __fastcall Planning_Close_Loop_006164e0(TechnoClass *techno, PlanningNodeClass *node) (0x006164E0;
// Planning_Execute 0x00614BA0 calls it): closes the techno's path into a loop at the node through the inline
// Planning_Close_Loop (defined in Planning_Path_Add's file 0x00616350, which expands it too).
// From ra2-decomp 0x00639740 (YR identical twin, src/planmgr.cpp FUN_00639740):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#include "shared/planning.hpp"

void __fastcall Planning_Close_Loop_006164e0(TechnoClass * techno, PlanningNodeClass * node)
{
	Planning_Close_Loop(techno, node);
}
