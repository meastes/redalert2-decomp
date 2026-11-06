// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#define RA2_COORD_EQUAL_MEMBER
// bool __fastcall Unknown_Planning_00615DE0() (0x00615DE0)
// While planning: whether every selected object is a techno whose path ends
// at the same node as the first selected techno's; refuses
// (MSG:PlanningModeHeteroSel) a mixed selection. True outside planning or
// with nothing selected.
// from ra2-decomp 0x00639040 (FUN_00639040, src/planmgr.cpp)
#include "shared/planning.hpp"
// @ref ??_C@_0BK@INKB@MSG?3PlanningModeHeteroSel?$AA@=007ebdc0

// AbstractClass::Is_Techno as ra2-decomp writes it (not in this unit's
// ObjectClass declaration).
static inline bool Is_Techno(ObjectClass const *object)
{
    if (object == 0) return false;
    unsigned char flag = object->AbstractFlags.IsTechno;
    return flag != 0;
}

bool __fastcall Planning_Same_Last_Node(PlanningTokenClass *token1, PlanningTokenClass *token2);

bool __fastcall Unknown_Planning_00615DE0()
{
    if (!unknown_flag_a767a4) {
        return true;
    }
    int count = unknown_objects_a40c60.Count();
    if (count == 0) {
        return true;
    }
    int i;
    for (i = 0; i < count; i++) {
        ObjectClass *object = unknown_objects_a40c60[i];
        if (Is_Techno(object)) {
            PlanningTokenClass *first = Planning_Token((TechnoClass *)object);
            for (i = 0; i < count; i++) {
                ObjectClass *other = unknown_objects_a40c60[i];
                if (!Is_Techno(other)) goto fail;
                PlanningTokenClass *token = Planning_Token((TechnoClass *)other);
                if (!Planning_Same_Last_Node(first, token)) goto fail;
            }
            return true;
        }
    }
    return false;
fail:
    Planning_Refuse("MSG:PlanningModeHeteroSel");
    return false;
}
