/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// From mischa85/RedAlert2 392e904 src/tracker.cpp (matching RA2 1.006 decomp; TibSun tracker.cpp): the tracker
// list names, function names and comments; modified (our module form and header state, Route A unit map
// manifests/original_target/modules/tracker.json).

/* $Header: /CounterStrike/TRACKER.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TRACKER.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/14/96                                                     *
 *                                                                                             *
 *                  Last Update : June 14, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Detach_This_From_All -- Detaches this object from all others.                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Header-static dynamic initializers of the unit at 0x006EC3A0 and the unit's twelve
// tracker lists (each an initializer and an atexit destructor, 0x006EC610-0x006ECB50; every
// vtable store is DynamicVectorClass<AbstractClass *> 0x007A177C); the other drafts of this
// plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class AbstractClass;

DynamicVectorClass<AbstractClass *> ObjectPtrTracker;   // 0x00AC0D60
DynamicVectorClass<AbstractClass *> AbstractTypePtrTracker;   // 0x00AC0CB0
DynamicVectorClass<AbstractClass *> TagPtrTracker;   // 0x00AC0C58
DynamicVectorClass<AbstractClass *> TriggerPtrTracker;   // 0x00AC0D48
DynamicVectorClass<AbstractClass *> AnimPtrTracker;   // 0x00AC0BF8
DynamicVectorClass<AbstractClass *> FactoryPtrTracker;   // 0x00AC0C80
DynamicVectorClass<AbstractClass *> EventActionPtrTracker;   // 0x00AC0C98
DynamicVectorClass<AbstractClass *> WaypointPathPtrTracker;   // 0x00AC0C30
DynamicVectorClass<AbstractClass *> TeamPtrTracker;   // 0x00AC0C18
DynamicVectorClass<AbstractClass *> HousePtrTracker;   // 0x00AC0D08
DynamicVectorClass<AbstractClass *> NeuronPtrTracker;   // 0x00AC0D30

DynamicVectorClass<AbstractClass *> ObjectsToDelete;   // 0x00AC0CD8
// The later drafts of this unit include headers that declare these lists (shared/object.hpp,
// shared/object_lists.hpp): they skip the declarations here.
#define RA2_TRACKER_LISTS_DEFINED


// Dynamic initializer (0x006EC3D0, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC3F0, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC410, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC430, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC450, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC470, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC4B0, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC4E0, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC510, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC540, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC550, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC570, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.




#define MAP_VIEW_DECLARE_DETACH
#include "shared/map_view.hpp"
#include "shared/object_lists.hpp"
#include "shared/abstract_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/logic.hpp"
#include "shared/rules.hpp"
#include "shared/tactical.hpp"
#include "shared/saveload_lists.hpp"
#include "shared/kamikaze_tracker.hpp"
#include "shared/foot.hpp"
#include "shared/object.hpp"


// Dynamic initializer (0x006EC590, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC5D0, _$E) emitted by HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC610, _$E) of AbstractTypePtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006EC650, _$E) of AbstractTypePtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC690, _$E) of TagPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006EC6D0, _$E) of TagPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC710, _$E) of TriggerPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006EC750, _$E) of TriggerPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC790, _$E) of AnimPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006EC7D0, _$E) of AnimPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC810, _$E) of FactoryPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006EC850, _$E) of FactoryPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC890, _$E) of EventActionPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006EC8D0, _$E) of EventActionPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC910, _$E) of WaypointPathPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006EC950, _$E) of WaypointPathPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006EC990, _$E) of TeamPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006EC9D0, _$E) of TeamPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006ECA10, _$E) of HousePtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006ECA50, _$E) of HousePtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006ECA90, _$E) of NeuronPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006ECAD0, _$E) of NeuronPtrTracker: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// Dynamic initializer (0x006ECB10, _$E) of ObjectsToDelete: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


// atexit destructor (0x006ECB50, _$E) of ObjectsToDelete: emitted by the definition in
// HeaderStatics_diagonal_init_006ec3a0.cpp; no source of its own.


void __fastcall Detach_From_Command_List(void *item);

/***********************************************************************************************
 * Detach_This_From_All -- Detaches this object from all others.                               *
 *                                                                                             *
 *    This routine sweeps through all game objects and makes sure that it is no longer         *
 *    referenced by them. Typically, this is called in preparation for the object's death      *
 *    or limbo state.                                                                          *
 *                                                                                             *
 * INPUT:   target   -- This object expressed as a target number.                              *
 *                                                                                             *
 *          all      -- Is this object really in truly being removed from the game? The        *
 *                      answer would be false if the target was actually a stealth             *
 *                      tank that is cloaking. In such a case, the object should be removed    *
 *                      from all non-friendly tracking systems, but otherwise left alone.      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/

void __fastcall Detach_This_From_All(AbstractClass *target, bool all)
{
    int rtti = target->Kind_Of();
    int index = 0;

    if (Map.PendingObjectPtr == target) {
        Map.PendingObjectPtr = 0;
    }

    if (Map.PendingObject == target) {
        Map.PendingObject = 0;
        Map.Unknown_00498F10(0);
    }

    if (rtti == 0x18 && target == GasSystem) {
        GasSystem = 0;
    }

    if (rtti == 0x0D) {
        for (index = 0; index < HousePtrTracker.Count(); index++) {
            HousePtrTracker[index]->Detach(target, all);
        }
        Logic.Detach(target, all);
        /*
         * nothing else needs to be done
         */
        return;
    }

    if (rtti == 0x04) {
        for (index = 0; index < AnimPtrTracker.Count(); index++) {
            AnimPtrTracker[index]->Detach(target, all);
        }
        /*
         * nothing else needs to be done
         */
        return;
    }

    bool is_object;
    if (target != 0) {
        is_object = target->AbstractFlags.IsObject;
    }
    if (target != 0 && is_object) {
        for (index = 0; index < ObjectPtrTracker.Count(); index++) {
            ObjectPtrTracker[index]->Detach(target, all);
        }
        BombList.Detach(target);
        KamikazeTracker.Detach(target);
        if (rtti == 0x0F || rtti == 0x01 || rtti == 0x02) {
            AircraftTracker.Detach(target);
        }
        Detach_From_Command_List(target);
        if (TacticalMap != 0) {
            TacticalMap->Detach(target, true);
        }
        Logic.Detach(target, all);
        /*
         * nothing else needs to be done
         */
        return;
    }

    if (dynamic_cast<AbstractTypeClass *>(target) != 0) {
        for (index = 0; index < AbstractTypePtrTracker.Count(); index++) {
            AbstractTypePtrTracker[index]->Detach(target, all);
        }
        Rules->Detach(target, all);
        /*
         * nothing else needs to be done
         */
        return;
    }

    switch (rtti) {
    case 0x2C:
        for (index = 0; index < TagPtrTracker.Count(); index++) {
            TagPtrTracker[index]->Detach(target, all);
        }
        Map.Detach(target, true);
        Logic.Detach(target, all);
        break;

    case 0x26:
        for (index = 0; index < TriggerPtrTracker.Count(); index++) {
            TriggerPtrTracker[index]->Detach(target, all);
        }
        break;

    case 0x0C:
        for (index = 0; index < FactoryPtrTracker.Count(); index++) {
            FactoryPtrTracker[index]->Detach(target, all);
        }
        Map.Detach(target, true);
        break;

    case 0x2F:
    case 0x30:
        for (index = 0; index < EventActionPtrTracker.Count(); index++) {
            EventActionPtrTracker[index]->Detach(target, all);
        }
        break;

    case 0x33:
        for (index = 0; index < WaypointPathPtrTracker.Count(); index++) {
            WaypointPathPtrTracker[index]->Detach(target, all);
        }
        break;

    case 0x22:
        for (index = 0; index < TeamPtrTracker.Count(); index++) {
            TeamPtrTracker[index]->Detach(target, all);
        }
        break;

    case 0x3C:
        for (index = 0; index < NeuronPtrTracker.Count(); index++) {
            NeuronPtrTracker[index]->Detach(target, all);
        }
        break;
    }
}


/// <summary>
/// Disposes of the objects that are waiting on deferred deletion list.
/// This routine is called by the main game loop once the logic has finished with the
/// objects it destroyed. Any object that has gone inactive is taken off the list and
/// released; one that is still active is left behind for a later pass.
/// </summary>
class type_info {
public:
    virtual ~type_info();
    int operator==(type_info const &rhs) const;
    int operator!=(type_info const &rhs) const;
private:
    void *_m_data;
    char _m_d_name[1];
};

class BuildingClass;
class UnitClass;
class InfantryClass;
class AircraftClass;

void Process_Deferred_Deletion()
{
    int index = 0;
    while (index < ObjectsToDelete.Count()) {
        AbstractClass *obj = ObjectsToDelete[index];
        if (obj->Is_Inactive()) {
            while (true) {
                if (!ObjectsToDelete.Delete(obj)) {
                    break;
                }
            }
            if (obj->Release()) {
                if (typeid(BuildingClass) == typeid(*obj)
                 || typeid(UnitClass) == typeid(*obj)
                 || typeid(InfantryClass) == typeid(*obj)
                 || typeid(AircraftClass) == typeid(*obj)) {
                    ((ObjectClass *)obj)->IsActive = true;
                }
                delete obj;
            }
        } else {
            ++index;
        }
    }
}
