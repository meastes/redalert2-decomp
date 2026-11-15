/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/target.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// target.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/target.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/object_index.hpp"
#include "shared/building.hpp"
#include "shared/unit.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/aircraft.hpp"
#include "shared/infantry.hpp"
#include "shared/terrain.hpp"
#include "shared/bullet.hpp"
#include "shared/anim.hpp"
#include "shared/team.hpp"
#include "shared/team_type.hpp"
#include "shared/trigger.hpp"
#include "shared/trigger_type.hpp"
#include "shared/tag.hpp"
#include "shared/tag_type.hpp"
#include "shared/house.hpp"
#include "shared/techno_type.hpp"
#include "shared/target.hpp"


// Header-static dynamic initializers of the unit at 0x006B5250:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006B5250-0x006B5436); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006B5280, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B52A0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B52C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B52E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5300, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5320, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5360, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5390, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B53C0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B53F0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5400, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5420, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer of the object index (0x006B5440, _$E): zeroes
// unknown_object_index_abff58 (0x00ABFF58) and registers its atexit destructor
// (0x006B5470, the following _$E draft, emitted by this definition).


IndexClass<long, AbstractClass *> unknown_object_index_abff58;


// atexit destructor of unknown_object_index_abff58 (0x006B5470, _$E): emitted by
// the definition in unknown_object_index_abff58_init_006b5440.cpp; no source of its own.


// TargetClass (Target.cpp, 0x006B54A0-0x006B6654; layout: shared/target.hpp):
// a reference to a game object by kind and ID. Kind 11 is a cell (ID = X + Y *
// 1000); kind 0x34 an object in the object index (shared/object_index.hpp).

/// <summary>
/// Creates a target that refers to a game object.
/// This routine is the usual way a target is made. A cell is recorded by its location so
/// that it survives being reallocated; everything else is recorded by its tracker ID. A
/// NULL pointer produces an empty target.
/// </summary>
/// <param name="ptr">Pointer to the object the target should refer to.</param>
// (0x006B54A0)
// position, any other object by its ID; null gives no target.

TargetClass::TargetClass(AbstractClass *object)
{
    if (object != 0) {
        int kind = object->Kind_Of();
        if (kind == 11) {
            Kind = kind;
            Cell cell = ((CellClass *)object)->MapCoords;
            ID = cell.X + cell.Y * 1000;
        } else {
            Kind = 0x34;
            ID = object->Fetch_ID();
        }
    } else {
        Kind = 0;
        ID = 0;
    }
}


/// <summary>
/// Creates a target that refers to a map cell.
/// A cell of CELL_NONE produces an empty target, so the caller need not screen out the
/// no-cell case before building one.
/// </summary>
/// <param name="cell">The cell to build the target from.</param>
// (0x006B5510)
// cell (shared/unknown_geometry.hpp, this unit's copy) gives no target.

TargetClass::TargetClass(Cell const &cell)
{
    if (cell == unknown_geometry_k) {
        Kind = 0;
    } else {
        Kind = 11;
        ID = cell.X + cell.Y * 1000;
    }
}


/// <summary>
/// Creates a target that refers to the cell under a coordinate.
/// Use this routine when a piece of logic has a location in hand but wants to aim at the
/// map rather than at whatever happens to be standing there.
/// </summary>
/// <param name="coord">The coordinate to build the target from.</param>
// (0x006B5560)
// holding the coordinate.

TargetClass::TargetClass(CoordStruct const &coord)
{
    Kind = 11;
    ID = coord.X / 256 + coord.Y / 256 * 1000;
}


// AbstractTypeClass *TargetClass::As_AbstractType() const (0x006B55A0): the indexed object if it is a AbstractTypeClass, or null.

AbstractTypeClass *TargetClass::As_AbstractType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<AbstractTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/// <summary>
/// Converts a target into a tag pointer.
/// This routine is used to convert the target object into a pointer to the tag it
/// represents. If it represents something else, then NULL is returned.
/// </summary>
/// <returns>Returns with a pointer to the tag that this target represents, or NULL if it
/// doesn't represent one.</returns>
// (0x006B5670)

TagClass *TargetClass::As_Tag() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TagClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/// <summary>
/// Converts a target into a tag type pointer.
/// This routine is used to convert the target object into a pointer to the tag type it
/// represents. If it represents something else, then NULL is returned.
/// </summary>
/// <returns>Returns with a pointer to the tag type that this target represents, or NULL if
/// it doesn't represent one.</returns>
// (0x006B5740)

TagTypeClass *TargetClass::As_TagType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TagTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * TargetClass::As_Abstract -- Converts a target into an abstract object pointer.              *
 *                                                                                             *
 *    If the target represents an object of some type, then this routine will return a         *
 *    pointer to the object. Otherwise it will return NULL.                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the object that this target represents or NULL if it     *
 *          doesn't represent a target.                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B5810)
// object or the cell, or null.

AbstractClass *TargetClass::As_Abstract() const
{
    if (Kind == 0x34) {
        return dynamic_cast<AbstractClass *>(unknown_object_index_abff58[ID]);
    }
    if (Kind == 11) {
        return &Map[Cell(ID % 1000, ID / 1000)];
    }
    return 0;
}


/***********************************************************************************************
 * TargetClass::As_Techno -- Converts a target into a techno object pointer.                   *
 *                                                                                             *
 *    This routine is used to convert the target object into a pointer to a techno class       *
 *    object. If the target doesn't specify a techno class object, then NULL is returned.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the techno class object that this target represents or   *
 *          else it returns NULL.                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B5910)
// as a techno (AbstractClass::As_Techno), or null.

TechnoClass *TargetClass::As_Techno() const
{
    if (Kind == 0x34) {
        return unknown_object_index_abff58[ID]->As_Techno();
    }
    return 0;
}


/***********************************************************************************************
 * TargetClass::As_Object -- Converts a target into an object pointer.                         *
 *                                                                                             *
 *    If the target represents an object of some type, then this routine will return a         *
 *    pointer to the object. Otherwise it will return NULL.                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the object that this target represents or NULL if it     *
 *          doesn't represent a target.                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B59E0)

ObjectClass *TargetClass::As_Object() const
{
    if (Kind == 0x34) {
        return dynamic_cast<ObjectClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/// <summary>
/// Converts a target into a foot object pointer.
/// This routine is used when only a mobile object will do. If the target represents a
/// building or something that isn't an object at all, then NULL is returned.
/// </summary>
/// <returns>Returns with a pointer to the mobile object that this target represents, or
/// NULL if it doesn't represent one.</returns>
// (0x006B5AB0)

FootClass *TargetClass::As_Foot() const
{
    if (Kind == 0x34) {
        return dynamic_cast<FootClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Trigger -- Converts specified target into a trigger pointer.                             *
 *                                                                                             *
 *    This routine will convert the specified target number into a trigger pointer.            *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the trigger pointer that the specified target number represents. If   *
 *          it doesn't represent a legal trigger object, then NULL is returned.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B5B80)

TriggerClass *TargetClass::As_Trigger() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TriggerClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/// <summary>
/// Converts a target into a house pointer.
/// This routine is used to convert the target object into a pointer to the house class
/// object it represents. If the target isn't a house, then NULL is returned.
/// </summary>
/// <returns>Returns with a pointer to the house that this target represents, or NULL if it
/// doesn't represent one.</returns>
// (0x006B5C50)

HouseClass *TargetClass::As_House() const
{
    if (Kind == 0x34) {
        return dynamic_cast<HouseClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_TechnoType -- Convert the target number into a techno type class pointer.                *
 *                                                                                             *
 *    This routine will conver the specified target number into a pointer to the techno        *
 *    type class that it represents.                                                           *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the TechnoTypeClass object that the target number        *
 *          represents. If it doesn't represent that kind of object, then NULL is returned.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B5D20)

TechnoTypeClass *TargetClass::As_TechnoType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TechnoTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_TriggerType -- Convert the specified target into a trigger type.                         *
 *                                                                                             *
 *    This routine will conver the target number into a pointer to the trigger type it         *
 *    represents.                                                                              *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert into a trigger type pointer.               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the trigger type object that the specified target value  *
 *          represents. If it doesn't represent a trigger type, then NULL is returned.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B5DF0)

TriggerTypeClass *TargetClass::As_TriggerType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TriggerTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_TeamType -- Converts a target into a team type pointer.                                  *
 *                                                                                             *
 *    This routine will convert the specified target number into a team type pointer.          *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type represented by the target number. If the   *
 *          target number doesn't represent a legal team type, then NULL is returned.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B5EC0)

TeamTypeClass *TargetClass::As_TeamType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TeamTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/// <summary>
/// Converts a target into a terrain object pointer.
/// This routine is used to convert the target object into a pointer to a terrain class
/// object. If the target doesn't specify a terrain object, then NULL is returned.
/// </summary>
/// <returns>Returns with a pointer to the terrain object that this target represents, or
/// NULL if it doesn't represent one.</returns>
// (0x006B5F90)

TerrainClass *TargetClass::As_Terrain() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TerrainClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Bullet -- Converts the target into a bullet pointer.                                     *
 *                                                                                             *
 *    This routine will convert the specified target number into a bullet pointer.             *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the bullet it specifies. If the target doesn't refer to  *
 *          a legal bullet, then NULL is returned.                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B6060)

BulletClass *TargetClass::As_Bullet() const
{
    if (Kind == 0x34) {
        return dynamic_cast<BulletClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Animation -- Converts target value into animation pointer.                               *
 *                                                                                             *
 *    This routine will convert the specified target number into an animation pointer.         *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert into an animation pointer.                *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the legal animation that this target represents. If it   *
 *          doesn't represent a legal animation, then NULL is returned.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B6130)

AnimClass *TargetClass::As_Anim() const
{
    if (Kind == 0x34) {
        return dynamic_cast<AnimClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Team -- Converts a target number into a team pointer.                                    *
 *                                                                                             *
 *    This routine will convert the specified target number into a team pointer.               *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the team object that the specified target number represents. If it    *
 *          doesn't represent a legal team then NULL is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B6200)

TeamClass *TargetClass::As_Team() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TeamClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Infantry -- If the target is infantry, return a pointer to it.                           *
 *                                                                                             *
 *    This routine will translate the specified target value into an infantry pointer if the   *
 *    target actually represents an infantry object.                                           *
 *                                                                                             *
 * INPUT:   target   -- The target to convert to a pointer.                                    *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to the infantry object that this target value represents. If     *
 *          the target doesn't represent an infantry object, then return NULL.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B62D0)

InfantryClass *TargetClass::As_Infantry() const
{
    if (Kind == 0x34) {
        return dynamic_cast<InfantryClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Unit -- Converts a target value into a unit pointer.                                     *
 *                                                                                             *
 *    This routine is used to convert the target value specified into a pointer to a unit      *
 *    object.                                                                                  *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert into a unit pointer.                       *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the unit the target value represents or NULL if not      *
 *          a unit.                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B63A0)

UnitClass *TargetClass::As_Unit() const
{
    if (Kind == 0x34) {
        return dynamic_cast<UnitClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Building -- Converts a target value into a building object pointer.                      *
 *                                                                                             *
 *    This routine is used to convert the target value specified into a building pointer.      *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert from.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building object that the target value represents.    *
 *          If it doesn't represent a building, then return NULL.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B6470)

BuildingClass *TargetClass::As_Building() const
{
    if (Kind == 0x34) {
        return dynamic_cast<BuildingClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Aircraft -- Converts the target value into an aircraft pointer.                          *
 *                                                                                             *
 *    This routine will convert the specified target value into an aircraft object pointer.    *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the aircraft that this target value represents. If the   *
 *          specified target value doesn't represent an aircraft, then NULL is returned.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B6540)

AircraftClass *TargetClass::As_Aircraft() const
{
    if (Kind == 0x34) {
        return dynamic_cast<AircraftClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


/***********************************************************************************************
 * As_Cell -- Converts a target value into a cell number.                                      *
 *                                                                                             *
 *    This routine is used to convert the target value specified, into a cell value. This is   *
 *    necessary for find path and other procedures that need a cell value.                     *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert to a cell value.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the target value expressed as a cell location.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006B6610)

CellClass *TargetClass::As_Cell() const
{
    if (Kind == 11) {
        return &Map[Cell(ID % 1000, ID / 1000)];
    }
    return 0;
}
