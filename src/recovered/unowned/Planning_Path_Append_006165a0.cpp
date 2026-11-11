// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#define RA2_COORD_EQUAL_MEMBER
// void __fastcall Planning_Path_Append(TechnoClass *techno, PlanningNodeClass *node) (0x006165A0)
// Gives techno the rest of the path of the node's first member from the node
// on, and that path's loop: from the loop start when the node lies before
// it, else from the node with the stretch up to it shared too. Selects (or,
// with a loop, unselects) the members of techno's last node the first time
// a human house does this.
// from ra2-decomp 0x00639800 (FUN_00639800, src/planmgr.cpp)
#include "shared/planning.hpp"

void __fastcall Planning_Path_Append(TechnoClass *techno, PlanningNodeClass *node)
{
    TechnoClass *owner = node->Member_Owner(0);
    PlanningTokenClass *initial = Planning_Token(owner);
    int from = initial->Node_Index(node);
    PlanningTokenClass *source = Planning_Token(owner);
    source->Copy_Nodes_To(techno, from);

    int start = -1;
    int loop_count = -1;
    PlanningNodeClass *update = 0;
    PlanningTokenClass *target = Planning_Token(techno);
    if (source->Loop_Past_Start() || source->Loop_At_Start()) {
        int const source_loop_count = source->Get_LoopLength();
        int step = source->Get_LoopStart();
        if (source->Loop_Past_Start() && from < step) {
            PlanningNodeClass *loop = source->Node_At(step);
            start = target->Node_Index(loop);
            update = loop;
        } else {
            source->Copy_Nodes_To(techno, step, from - step);
            start = target->Node_Index(node);
            update = node;
        }
        loop_count = source_loop_count;
    }
    target->LoopLength = loop_count;
    target->LoopStart = start;
    target->unknown_98 = true;
    if (update) {
        update->unknown_611770();
    }

    if (!unknown_flag_a76790 && techno->House->Is_Player_Control()) {
        unknown_flag_a76790 = true;
        PlanningNodeClass *last = target->Last_Node();
        if (!update) {
            last->unknown_612090();
        } else {
            last->unknown_6120c0();
        }
    }
}
