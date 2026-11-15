/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// From mischa85/RedAlert2 392e904 src/tag.cpp (matching RA2 1.006 decomp; TibSun tag.cpp): global names and
// Parameter names from mischa85/RedAlert2 392e904 src/tag.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/tag.cpp where both functions declare the same local types (names only).
// function comments; modified (our module form and header state, Route A unit map
// manifests/original_target/modules/tag.json).

// Header-static dynamic initializers of the unit at 0x006B3600 and the
// unit's TagClass list; the other drafts of this plan are emitted by this file.
// Unit-wide (Tag.cpp 0x006B3600): tag.hpp's guarded Tag.cpp members.
#define RA2_TAG_UNIT_MEMBERS
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/object.hpp"

// The live tags (0x00ABFE48).
DynamicVectorClass<TagClass *> Tags;


// Dynamic initializer (0x006B3630, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B3650, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B3670, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B3690, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B36B0, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B36D0, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B3710, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B3740, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B3770, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B37A0, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B37B0, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B37D0, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B37F0, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


// Dynamic initializer (0x006B3830, _$E) emitted by HeaderStatics_diagonal_init_006b3600.cpp; no source of its own.


/// <summary>
/// Creates a tag from the specified tag type.
/// The new tag joins the global tag list and builds a trigger for every trigger type
/// that its tag type names, so it is ready to spring the moment it is attached to
/// something.
/// </summary>
/// <param name="type">The tag type that this tag is built from.</param>
// (0x006B3870)

// The trigger tracker list (0x00AC0D48; tracker.cpp).
extern DynamicVectorClass<AbstractClass *> TriggerPtrTracker;

TagClass::TagClass(TagTypeClass *type) :
    Class(type),
    Trigger(0),
    AttachCount(0),
    CellID(unknown_geometry_k),
    IsToDie(false),
    IsCurrentlySprung(false)
{
    Tags.Add(this);
    AbstractTypePtrTracker.Add((ObjectClass *)this);
    TriggerPtrTracker.Add(this);
    if (type != 0) {
        for (TriggerTypeClass *trigger = type->FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
            TriggerClass *made = new TriggerClass(trigger);
            made->LinkedTo = Trigger;
            Trigger = made;
        }
    }
}


/// <summary>
/// Destroys this tag and severs everything that pointed at it.
/// The triggers hanging off this tag are queued for destruction, the tag is pulled from
/// the global tag list, and if it was one of the allow-win tags then its house is given
/// back the victory it was holding up.
/// </summary>
// (0x006B39F0)
#include "shared/house.hpp"

// The trigger tracker list (0x00AC0D48; tracker.cpp).
extern DynamicVectorClass<AbstractClass *> TriggerPtrTracker;
// The logic and map tag lists and the index each is being processed at (0x00866160 / 0x00A35D44
// for types with flag 0x10, 0x00866238 / 0x00A35E24 for flag 4).
extern DynamicVectorClass<TagClass *> LogicTags;
extern int LogicTriggerID;
extern DynamicVectorClass<TagClass *> MapTags;
extern int MapTriggerID;
extern bool GameActive;
extern bool ScenarioActive;

TagClass::~TagClass()
{
    Unknown_006ECB90(this, true);
    if (GameActive && ScenarioActive && Class != 0 && (Class->Attaches_To() & 0x10)) {
        int index = LogicTags.ID(this);
        if (LogicTriggerID >= index) {
            if (--LogicTriggerID < 0 && LogicTags.Count() == 0) {
                LogicTriggerID = 0;
            }
        }
    }
    if (GameActive && ScenarioActive && Class != 0 && (Class->Attaches_To() & 4)) {
        int index = MapTags.ID(this);
        if (MapTriggerID >= index) {
            if (--MapTriggerID < 0 && MapTags.Count() == 0) {
                MapTriggerID = 0;
            }
        }
    }
    if (GameActive && ScenarioActive && Trigger != 0 && Trigger->Class != 0 &&
        Trigger->Class->House != 0 && Class->Is_Allow_Win()) {
        HouseClass *house = HouseClass::As_Pointer(((HouseTypeClass *)Trigger->Class->House)->House);
        if (house->Blockage != 0) {
            house->Blockage--;
        }
        house->BorrowedTime = 60;
    }
    for (TriggerClass *trigger = Trigger; trigger != 0; trigger = trigger->LinkedTo) {
        ObjectsToDelete.Add(trigger);
    }
    AbstractTypePtrTracker.Delete((ObjectClass *)this);
    TriggerPtrTracker.Delete(this);
    Tags.Delete(this);
    Unknown_006ECB90(this, true);
}


/// <summary>
/// Condemns this tag to destruction.
/// The tag is not deleted on the spot; it is queued so that it goes away at a point in
/// the game logic where nothing is still walking the tag list.
/// </summary>
// (0x006B3CC0)

void TagClass::Mark_To_Delete()
{
    IsToDie = true;
    ObjectsToDelete.Add(this);
}


/// <summary>
/// Has this tag been condemned?
/// </summary>
/// <returns>bool; Is this tag waiting to be destroyed?</returns>
// (0x006B3D20)

bool TagClass::Is_Marked_To_Delete() const
{
    return IsToDie;
}


/// <summary>
/// Fetches the tag of the specified type, creating one if need be.
/// Use this routine when something needs a tag of a given type and does not care whether
/// the scenario has already brought one into existence.
/// </summary>
/// <param name="type">The tag type that the tag should be built from.</param>
/// <returns>Returns with a pointer to the tag of that type. It is created if none
/// existed.</returns>
// (0x006B3D30)

TagClass *__fastcall TagClass::Find_Or_Make(TagTypeClass *type)
{
    for (int index = 0; index < Tags.Count(); index++) {
        TagClass *tag = Tags[index];
        if (tag->Class == type) {
            return tag;
        }
    }
    return new TagClass(type);
}


/// <summary>
/// Fetches the map cell that this tag rides on.
/// </summary>
/// <returns>Returns with the cell the tag is attached to. CELL_NONE means it is not on
/// the map.</returns>
// (0x006B3D80)

Cell TagClass::Get_Position() const
{
    return CellID;
}


/// <summary>
/// Is this tag watching for a vertical line crossing?
/// </summary>
/// <returns>bool; Was the tag type declared as a vertical crossing tag?</returns>
// (0x006B3D90)

bool TagClass::Is_Cross_Vertical() const
{
    return Class->Is_Cross_Vertical();
}


/// <summary>
/// Is this tag watching for a horizontal line crossing?
/// </summary>
/// <returns>bool; Was the tag type declared as a horizontal crossing tag?</returns>
// (0x006B3DA0)

bool TagClass::Is_Cross_Horizontal() const
{
    return Class->Is_Cross_Horizontal();
}


/// <summary>
/// Is this tag watching for something entering its zone?
/// </summary>
/// <returns>bool; Was the tag type declared as an enters-zone tag?</returns>
// (0x006B3DB0)

bool TagClass::Is_Enters_Zone() const
{
    return Class->Is_Enters_Zone();
}


/// <summary>
/// Is this one of the tags that gates the player's victory?
/// A house cannot declare victory while any of its allow-win tags is still waiting to
/// be sprung.
/// </summary>
/// <returns>bool; Does this tag hold up its house from winning?</returns>
// (0x006B3DC0)

bool TagClass::Is_Allow_Win() const
{
    return Class->Is_Allow_Win();
}


/// <summary>
/// Is the specified trigger hanging off this tag?
/// </summary>
/// <returns>bool; Is the trigger a member of this tag's chain?</returns>
// (0x006B3DD0)

bool TagClass::Is_Trigger_Attached(TriggerClass *trigger) const
{
    for (TriggerClass *trigptr = Trigger; trigptr != 0; trigptr = trigptr->LinkedTo) {
        if (trigptr == trigger) {
            return true;
        }
    }
    return false;
}


/// <summary>
/// Springs this tag in response to a game event.
/// Every trigger hanging off this tag is offered the event and any that recognize it
/// will fire their actions. The tag type's persistence decides what becomes of the tag
/// afterwards -- it may detach itself from whatever it was riding on and queue itself
/// for destruction, or it may stay behind to fire again.
/// </summary>
/// <param name="event">The event that has occurred.</param>
/// <param name="object">The object the event happened to. This may be NULL.</param>
/// <param name="cell">The cell the event happened at, or CELL_NONE if it was not cell
/// based.</param>
/// <param name="forced">Should the triggers fire without regard to their own event tests?</param>
/// <param name="source">The object that caused the event, if there was one.</param>
/// <returns>bool; Did any attached trigger fire?</returns>
/// <remarks>A tag will refuse to spring while it is already in the middle of springing.</remarks>
// (0x006B3DF0)
#include "shared/cell.hpp"
#include "shared/map_view.hpp"

bool TagClass::Spring(int event, ObjectClass *object, Cell cell, bool forced, int source)
{
    bool res = false;
    bool die = false;
    bool det = false;
    if (IsCurrentlySprung) {
        return false;
    }
    IsCurrentlySprung = true;
    for (TriggerClass *trigger = Trigger; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Should_Spring(event, object, forced, Class->Persistence == 2, source)) {
            switch (Class->Persistence) {
            case 0:
                trigger->Spring(object, cell);
                trigger->Mark_To_Delete();
                res = true;
                die = true;
                det = true;
                break;
            case 1:
                if (AttachCount == 1) {
                    trigger->Spring(object, cell);
                    trigger->Mark_To_Delete();
                    die = true;
                    res = true;
                } else {
                    det = true;
                }
                break;
            case 2:
                trigger->Spring(object, cell);
                res = true;
                break;
            }
        }
    }
    IsCurrentlySprung = false;
    if (det) {
        if (object != 0 && object->unknown_34 == this) {
            object->Attach_Tag(0);
        }
        if (cell != unknown_geometry_k) {
            Map[cell].Attach_Tag(0);
        }
    }
    if (die) {
        Unknown_006ECB90(this, true);
        ObjectsToDelete.Add(this);
    }
    return res;
}


/// <summary>
/// Records the map cell that this tag rides on.
/// </summary>
/// <param name="cell">The cell the tag is attached to, or CELL_NONE if it rides on an
/// object.</param>
// (0x006B3F90)

void TagClass::Set_Position(Cell cell)
{
    CellID = cell;
}


/// <summary>
/// Destroys every tag in the game.
/// This routine is called when a scenario is torn down so that no tag from the previous
/// mission survives into the next one.
/// </summary>
// (0x006B3FA0)

void TagClass::Clear_All()
{
    while (Tags.Count() > 0) {
        delete Tags[0];
    }
}


/// <summary>
/// Restarts this tag's elapsed time events tied to a global flag.
/// </summary>
/// <param name="global">The global flag that just changed.</param>
// (0x006B3FD0)

void TagClass::Timer_Global_Reset(int global)
{
    Trigger->Reset_Global_Linked_Timed_Events(global);
}


/// <summary>
/// Restarts this tag's elapsed time events tied to a local variable.
/// </summary>
/// <param name="local">The local variable that just changed.</param>
// (0x006B3FE0)

void TagClass::Timer_Local_Reset(int local)
{
    Trigger->Reset_Local_Linked_Timed_Events(local);
}


/// <summary>
/// Adds a trigger to this tag's trigger chain.
/// </summary>
/// <param name="trigger">The trigger to attach to this tag.</param>
// (0x006B3FF0)

void TagClass::Link(TriggerClass *trigger)
{
    trigger->LinkedTo = Trigger;
    Trigger = trigger;
}


/// <summary>
/// Removes a trigger from this tag's trigger chain.
/// The trigger itself is left intact -- only its membership in this tag is severed.
/// </summary>
/// <param name="trigger">The trigger to detach from this tag.</param>
/// <returns>bool; Was the trigger found and unlinked?</returns>
// (0x006B4000)

bool TagClass::Unlink(TriggerClass *trigger)
{
    if (trigger != 0) {
        if (Trigger == trigger) {
            Trigger = trigger->LinkedTo;
            return true;
        }
        TriggerClass *trigptr = Trigger;
        while (trigptr != 0) {
            if (trigptr->LinkedTo == trigger) {
                trigptr->LinkedTo = trigger->LinkedTo;
                return true;
            }
            trigptr = trigptr->LinkedTo;
        }
    }
    return false;
}


/// <summary>
/// Removes all references to the object specified.
/// This routine is called when an object is leaving the game. Should the tag lose the
/// tag type it was built from, it marks itself to die as well, since a tag with no type
/// has nothing left to do.
/// </summary>
/// <param name="target">The object that is going away.</param>
/// <param name="all">Is the target disappearing for good rather than merely hiding?</param>
// (0x006B4040)

void TagClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Class == target) {
        Class = 0;
    }
    if ((AbstractClass *)Trigger == target) {
        Trigger = Trigger->LinkedTo;
    }
    if (Class == 0) {
        Mark_To_Delete();
    }
}


/// <summary>
/// Fetches the class identifier that this tag persists under.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x006B40D0)
#include "shared/tag.hpp"

HRESULT __stdcall TagClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_TagClass;
    return S_OK;
}


/// <summary>
/// Submits this tag's state to the game CRC.
/// This routine is used by the network sync check to prove that every machine agrees
/// about the tags currently in play.
/// </summary>
/// <param name="crc">The CRC engine to feed this tag's values into.</param>
// (0x006B4110)

void TagClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(Class->Fetch_ID());
    if (Trigger != 0) {
        crc(Trigger->Fetch_ID());
    }
    crc(IsToDie);
}


/// <summary>
/// Loads this tag from the save game stream.
/// The virtual table is rebuilt in place and the tag type and trigger references are
/// swizzled back into live pointers.
/// </summary>
/// <param name="stream">The stream to read this tag from.</param>
/// <returns>Returns with the result code of the load attempt.</returns>
// (0x006B4160)

HRESULT __stdcall TagClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TagClass(NoInitClass());
    Swizzler.Swizzle((void **)&Class);
    Swizzler.Swizzle((void **)&Trigger);
    return S_OK;
}


/// <summary>
/// Saves this tag to the save game stream.
/// The base class writes the raw object out; this routine exists so that the tag layer
/// gets its chance at the stream.
/// </summary>
/// <param name="stream">The stream to write this tag to.</param>
/// <param name="cleardirty">Should the object be considered clean after being written?</param>
/// <returns>Returns with the result code of the save attempt.</returns>
// (0x006B41D0)

HRESULT __stdcall TagClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = AbstractClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Determines if this tag may be handed over to another object.
/// A tag is transferable if any of the triggers hanging off it was marked as such by
/// its trigger type. Such a tag will be passed along to the new owner when an object
/// is captured, and to the crew that survives a destroyed vehicle, rather than being
/// lost with the object it started out on.
/// </summary>
/// <returns>bool; May this tag be handed over to another object?</returns>
// (0x006B41F0)

bool TagClass::Is_To_Inherit() const
{
    for (TriggerClass *trigger = Trigger; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Class->IsToInherit) {
            return true;
        }
    }
    return false;
}


/// <summary>
/// Restarts the elapsed time events tied to a global flag.
/// This routine is called when a global flag changes value, so that every tag in the
/// scenario gets a chance to restart the timers that were waiting on it.
/// </summary>
/// <param name="global">The global flag that just changed.</param>
// (0x006B4220)

void __fastcall TagClass::All_Timer_Global_Reset(int global)
{
    for (int index = 0; index < Tags.Count(); index++) {
        Tags[index]->Trigger->Reset_Global_Linked_Timed_Events(global);
    }
}


/// <summary>
/// Restarts the elapsed time events tied to a local variable.
/// This routine is called when a local variable changes value, so that every tag in the
/// scenario gets a chance to restart the timers that were waiting on it.
/// </summary>
/// <param name="local">The local variable that just changed.</param>
// (0x006B4250)

void __fastcall TagClass::All_Timer_Local_Reset(int local)
{
    for (int index = 0; index < Tags.Count(); index++) {
        Tags[index]->Trigger->Reset_Local_Linked_Timed_Events(local);
    }
}


/// <summary>
/// Is this the only tag built from its tag type?
/// This routine is used when something needs to know whether the tag type it is looking
/// at has been instantiated more than once during the scenario.
/// </summary>
/// <returns>bool; Is this the sole tag of its type?</returns>
// (0x006B4280)

bool TagClass::Is_One_Of_A_Kind() const
{
    for (int index = 0; index < Tags.Count(); index++) {
        TagClass *tag = Tags[index];
        if (tag != this && tag->Class == Class) {
            return false;
        }
    }
    return true;
}


// int TagClass::Size_Of() const (0x006B42C0)

int TagClass::Size_Of() const
{
    return sizeof(TagClass);
}


// int TagClass::Kind_Of() const (0x006B42D0)

int TagClass::Kind_Of() const
{
    return 44;
}


// TagClass scalar deleting destructor (0x006B42E0): compiler generated; no source of its own.
