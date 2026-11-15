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
// Function comment blocks from mischa85/RedAlert2 392e904 src/trigger.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// trigger.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/trigger.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x006ED030:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006ED030-0x006ED216); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x006ED060, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED080, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED0A0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED0C0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED0E0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED100, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED140, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED170, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED1A0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED1D0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED1E0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.


// Dynamic initializer (0x006ED200, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006ed030.cpp; no source of its own.




#include "shared/object.hpp"
#include "shared/random.hpp"
#include "shared/trigger.hpp"
#include "shared/house.hpp"
#include "shared/taction.hpp"


/***********************************************************************************************
 * TriggerClass::TriggerClass -- constructor                                                   *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
// (0x006ED220)
// A live trigger; disabled when its type is, or for the current difficulty.

// The abstract list 0x00AC0D48 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> TriggerPtrTracker;

TriggerClass::TriggerClass(TriggerTypeClass *type) :
    Class(type),
    LinkedTo(0),
    IsToDelete(false),
    IsTripped(0),
    IsActive(true)
{
    AbstractTypePtrTracker.Add((ObjectClass *)this);
    TriggerPtrTracker.Add(this);
    Triggers.Add(this);
    if (Class != 0) {
        Reset_All_Timed_Events();
        if (!Class->IsEnabled || (Scenario_Difficulty() == 0 && !Class->IsEnabledOnEasy) ||
            (Scenario_Difficulty() == 1 && !Class->IsEnabledOnMedium) || (Scenario_Difficulty() == 2 && !Class->IsEnabledOnHard)) {
            IsActive = false;
        }
    }
}


// TriggerClass::~TriggerClass (0x006ED3C0)
// Inline destructor defined in the shared header, emitted out of line by this unit.
// No source of its own: emitted by this unit (population-2418).


/// <summary>
/// Does this trigger watch for a horizontal line being crossed?
/// The linked trigger is consulted as well, so a chain of linked triggers answers as one.
/// </summary>
/// <returns>bool; Is this a horizontal crossing trigger?</returns>
// (0x006ED4D0)
// TriggerTypeClass::Is_Cross_Horizontal of this trigger's type or of any later one.

bool TriggerClass::Is_Cross_Horizontal() const
{
    if (LinkedTo != 0) {
        return Class->Is_Cross_Horizontal() || LinkedTo->Is_Cross_Horizontal();
    }
    return Class->Is_Cross_Horizontal();
}


/// <summary>
/// Does this trigger watch for a vertical line being crossed?
/// The linked trigger is consulted as well, so a chain of linked triggers answers as one.
/// </summary>
/// <returns>bool; Is this a vertical crossing trigger?</returns>
// (0x006ED510)
// TriggerTypeClass::Is_Cross_Vertical of this trigger's type or of any later one.

bool TriggerClass::Is_Cross_Vertical() const
{
    if (LinkedTo != 0) {
        return Class->Is_Cross_Vertical() || LinkedTo->Is_Cross_Vertical();
    }
    return Class->Is_Cross_Vertical();
}


/// <summary>
/// Does this trigger watch for something entering its zone?
/// The linked trigger is consulted as well, so a chain of linked triggers answers as one.
/// </summary>
/// <returns>bool; Is this an enters-zone trigger?</returns>
// (0x006ED550)
// TriggerTypeClass::Is_Enters_Zone of this trigger's type or the next one's.

bool TriggerClass::Is_Enters_Zone() const
{
    if (LinkedTo != 0) {
        return Class->Is_Enters_Zone() || LinkedTo->Class->Is_Enters_Zone();
    }
    return Class->Is_Enters_Zone();
}


/// <summary>
/// Must this trigger spring before the scenario can be won?
/// The victory check consults every allow-win trigger before it declares the mission a
/// success. The linked trigger is consulted as well, so a chain answers as one.
/// </summary>
/// <returns>bool; Is this an allow-win trigger?</returns>
// (0x006ED590)
// TriggerTypeClass::Is_Allow_Win of this trigger's type or the next one's.

bool TriggerClass::Is_Allow_Win() const
{
    if (LinkedTo != 0) {
        return Class->Is_Allow_Win() || LinkedTo->Class->Is_Allow_Win();
    }
    return Class->Is_Allow_Win();
}


/// <summary>
/// Is this trigger controlled by the specified global variable?
/// The linked trigger is consulted as well, so a chain of linked triggers answers as one.
/// </summary>
/// <param name="global">Index of the global variable to check against.</param>
/// <returns>bool; Is this trigger tied to that global variable?</returns>
// (0x006ED5D0)

bool TriggerClass::Is_Linked_To_Global(int value) const
{
    if (LinkedTo != 0) {
        return Class->Is_Linked_To_Global(value) || LinkedTo->Class->Is_Linked_To_Global(value);
    }
    return Class->Is_Linked_To_Global(value);
}


/// <summary>
/// Restarts the timed events of triggers tied to the specified global variable.
/// This routine is used when a global scenario variable changes state. The whole chain of
/// linked triggers is walked, since a linked trigger may be tied to the variable as well.
/// </summary>
/// <param name="global">Index of the global variable that changed.</param>
// (0x006ED620)
// Resets every trigger of the chain (from this one) whose type answers
// TriggerTypeClass::Is_Linked_To_Global(value).

void TriggerClass::Reset_Global_Linked_Timed_Events(int value)
{
    for (TriggerClass *trigger = this; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Class->Is_Linked_To_Global(value)) {
            trigger->Reset_All_Timed_Events();
        }
    }
}


/// <summary>
/// Restarts the timed events of triggers tied to the specified local variable.
/// This routine is used when a local scenario variable changes state. The whole chain of
/// linked triggers is walked, since a linked trigger may be tied to the variable as well.
/// </summary>
/// <param name="local">Index of the local variable that changed.</param>
// (0x006ED650)
// Resets every trigger of the chain (from this one) whose type answers
// TriggerTypeClass::Is_Linked_To_Local(value).

void TriggerClass::Reset_Local_Linked_Timed_Events(int value)
{
    for (TriggerClass *trigger = this; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Class->Is_Linked_To_Local(value)) {
            trigger->Reset_All_Timed_Events();
        }
    }
}


/// <summary>
/// Restarts this trigger's timed events.
/// This routine is used whenever a trigger is enabled or rearmed. Any time based event it
/// carries begins its countdown again from the delay the trigger type specifies.
/// </summary>
// (0x006ED680)
// Restarts the timed events: elapsed time (13) waits Data seconds, random
// delay (51) Data / 2 plus a Random_Pick in [0, Data]; their fired bits
// clear (TibSun Reset_All_Timed_Events shape).

void TriggerClass::Reset_All_Timed_Events()
{
    int index = 0;
    TEventClass *event = Class->FirstEvent;
    while (event != 0) {
        if (event->Type == 0x0D) {
            Timer = event->Data * 15;
            IsTripped &= ~(1 << index);
        }
        if (event->Type == 0x33) {
            Timer = (event->Data / 2 + Random_Pick(0, event->Data)) * 15;
            IsTripped &= ~(1 << index);
        }
        index++;
        event = event->Next;
    }
}


/***********************************************************************************************
 * TriggerClass::Spring -- Spring the trigger (possibly).                                      *
 *                                                                                             *
 *    This routine is called when a potential trigger even has occurred. The event is matched  *
 *    with the trigger event needed by this trigger. If the condition warrants, the trigger    *
 *    action is performed.                                                                     *
 *                                                                                             *
 * INPUT:   event    -- The event that is occurring.                                           *
 *                                                                                             *
 *          obj      -- If the trigger is attached to an object, this points to the object.    *
 *                                                                                             *
 *          cell     -- If the trigger is attached to a cell, this is the cell number.         *
 *                                                                                             *
 *          forced   -- Should the trigger be forced to execute regardless of the event?       *
 *                                                                                             *
 * OUTPUT:  bool; Was the trigger sprung?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1996 JLB : Created.                                                                 *
 *   08/13/1996 JLB : Linked triggers supported.                                               *
 *=============================================================================================*/
// (0x006ED740)
// Unless disabled or already sprung: each event not yet marked must fire for event (in the trigger
// type's house, with the trigger's timer); events that fire and hold are marked. Any event that does
// not fire keeps the trigger from springing; forced skips the tests. A persistent spring restarts the
// timed events.

bool TriggerClass::Should_Spring(int event, ObjectClass *object, bool forced, bool persistent, int source)
{
    if (!IsActive || IsToDelete) {
        return false;
    }
    bool sprung = true;
    if (!forced) {
        int index = 0;
        for (TEventClass *e = Class->FirstEvent; e != 0; e = e->Next) {
            if (!Is_Event_Tripped(index) && !e->unknown_func_6e6050(event, HouseClass::As_Pointer(Class->House->ArrayIndex),
                                                             object, &Timer, &persistent, source)) {
                sprung = false;
            } else if (persistent && e->unknown_func_6e6cf0() && e->unknown_func_6e6d50()) {
                Flag_Event_Tripped(index);
            }
            index++;
        }
    }
    if (sprung && persistent) {
        Reset_All_Timed_Events();
    }
    return sprung;
}


/// <summary>
/// Performs this trigger's actions.
/// This routine is called once the trigger's events have all been satisfied. Every action
/// attached to the trigger type is given its chance to run.
/// </summary>
/// <param name="object">The object the trigger is attached to, if it is attached to one.</param>
/// <param name="cell">The cell the trigger is attached to, if it is attached to one.</param>
/// <returns>bool; Did any of the actions actually do something?</returns>
// (0x006ED830)
// Unless disabled or already sprung, performs every action of the type in the type's house;
// true when any of them acted.

bool TriggerClass::Spring(ObjectClass *object, Cell cell)
{
    if (!IsActive || IsToDelete) {
        return false;
    }
    bool acted = false;
    for (TActionClass *action = Class->FirstAction; action != 0; action = action->Next) {
        if (action->Execute(HouseClass::As_Pointer(Class->House->ArrayIndex), object, this, &cell)) {
            acted = true;
        }
    }
    return acted;
}


/***********************************************************************************************
 * Find_Or_Make -- Find or create a trigger of the type specified.                             *
 *                                                                                             *
 *    This routine is used when, given a trigger type, an actual trigger object is needed.     *
 *    In this case, an existing trigger of the correct type must be located, or a trigger      *
 *    object must be created. In either case, this routine will return a trigger object that   *
 *    corresponds to the trigger type class specified.                                         *
 *                                                                                             *
 * INPUT:   trigtype -- Pointer to the trigger type to find (or create) a matching trigger     *
 *                      object.                                                                *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to a matching trigger object. If no more triggers could be       *
 *          allocated and no matching trigger could be found, then this routine will return    *
 *          NULL (a very rare case).                                                           *
 *                                                                                             *
 * WARNINGS:   This routine could return NULL.                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006ED8A0)
// The live trigger of a type, created when missing; 0 without a type.

TriggerClass *__fastcall TriggerClass::Find_Or_Make(TriggerTypeClass *type)
{
    if (type != 0) {
        for (int index = 0; index < Triggers.Count(); index++) {
            if (type == Triggers[index]->Class) {
                return Triggers[index];
            }
        }
        return new TriggerClass(type);
    }
    return 0;
}


/***********************************************************************************************
 * TriggerClass::Detach -- Detach specified target from this trigger.                          *
 *                                                                                             *
 *    This routine is called when the specified trigger MUST be detached from all references   *
 *    to it. The only reference maintained by a trigger is the reference to the trigger        *
 *    type class it is modeled after.                                                          *
 *                                                                                             *
 * INPUT:   target   -- The target identifier to remove all attachments to.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   You must never detach the trigger type class from a trigger. Such a process     *
 *             will leave the trigger orphan and in a 'crash the game immediately if used'     *
 *             state. As such, this routine will throw an assertion if this is tried.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006ED900)

void TriggerClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Class == target) {
        Class = 0;
    }
    if ((AbstractClass *)LinkedTo == target) {
        LinkedTo = LinkedTo->LinkedTo;
    }
}


/// <summary>
/// Records that the specified event has been satisfied.
/// This routine is used by the persistent trigger logic so that an event which has already
/// occurred does not have to occur a second time.
/// </summary>
/// <param name="event">Index of the event that was satisfied.</param>
// (0x006ED930)

void TriggerClass::Flag_Event_Tripped(int index)
{
    IsTripped |= 1 << index;
}


/// <summary>
/// Clears the sprung record for the specified event.
/// This routine is used when a trigger is rearmed, so that the event must be satisfied
/// all over again before the trigger will spring.
/// </summary>
/// <param name="event">Index of the event to rearm.</param>
// (0x006ED950)

void TriggerClass::Flag_Event_Untripped(int index)
{
    IsTripped &= ~(1 << index);
}


/// <summary>
/// Has the specified event of this trigger already been satisfied?
/// A persistent trigger remembers which of its events have fired, so that the ones still
/// outstanding can be waited on without re-testing those already met.
/// </summary>
/// <param name="event">Index of the event to examine.</param>
/// <returns>bool; Has the event already sprung?</returns>
// (0x006ED970)

bool TriggerClass::Is_Event_Tripped(int index) const
{
    return (IsTripped & (1 << index)) != 0;
}


/// <summary>
/// Marks this trigger for deletion.
/// The trigger stops springing right away, but it is not destroyed until the deferred deletion
/// list is processed. This keeps a trigger from vanishing out from under the logic that
/// is still walking it.
/// </summary>
// (0x006ED990)
// Marks the trigger and queues it in the abstract list 0x00AC0CD8.

void TriggerClass::Mark_To_Delete()
{
    IsToDelete = true;
    ObjectsToDelete.Add(this);
}


/// <summary>
/// Has this trigger been marked for deletion?
/// </summary>
/// <returns>bool; Is this trigger waiting on deferred deletion list?</returns>
// (0x006ED9F0)

bool TriggerClass::Is_Marked_To_Delete() const
{
    return IsToDelete;
}


/// <summary>
/// Adds this trigger's state to the CRC calculation.
/// This routine is used by the network sync check to prove that every machine in the game
/// agrees about the triggers currently in play.
/// </summary>
/// <param name="crc">The CRC engine to feed this trigger's values into.</param>
// (0x006EDA00)

void TriggerClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(Class->Fetch_ID());
    if (LinkedTo != 0) {
        crc(LinkedTo->Fetch_ID());
    }
    crc(IsToDelete);
    crc(IsActive);
    crc(Timer.Value());
    crc((long)IsTripped);
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is part of the persistence support. The save game system uses the
/// identifier to work out what kind of object to build when the stream is read back in.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x006EDA90)

HRESULT __stdcall TriggerClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TriggerClass;
    return S_OK;
}


/// <summary>
/// Reads this trigger back in from the save game stream.
/// The virtual table is rebuilt in place and the trigger type and linked trigger
/// references are swizzled back into live pointers.
/// </summary>
/// <returns>Returns with the result code of the load attempt.</returns>
// (0x006EDAD0)

HRESULT __stdcall TriggerClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TriggerClass(NoInitClass());
    Swizzler.Swizzle((void **)&Class);
    Swizzler.Swizzle((void **)&LinkedTo);
    return S_OK;
}


/// <summary>
/// Writes this trigger out to the save game stream.
/// This routine is part of the persistence chain the save game system walks. The trigger
/// keeps nothing beyond its own members, so the base class does all of the real work.
/// </summary>
/// <returns>Returns with the result code of the save attempt.</returns>
// (0x006EDB40)

HRESULT __stdcall TriggerClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Enables this trigger.
/// The trigger's timed events are restarted as it comes on, so a trigger switched on part
/// way through a scenario counts its delays from that moment rather than from the start.
/// </summary>
// (0x006EDB60)

void TriggerClass::Enable()
{
    IsActive = true;
    Reset_All_Timed_Events();
}


/// <summary>
/// Disables this trigger.
/// A disabled trigger ignores every event that comes its way. Use this routine when a
/// trigger must be held in reserve until the scenario decides to switch it on.
/// </summary>
// (0x006EDB70)

void TriggerClass::Disable()
{
    IsActive = false;
}


// int TriggerClass::Kind_Of() const (0x006EDB90)

int TriggerClass::Kind_Of() const
{
    return 38;
}


/***********************************************************************************************
 * TriggerClass::~TriggerClass -- Destructor for trigger objects.                              *
 *                                                                                             *
 *    This destructor will update the house blockage value if necessary. No other action need  *
 *    be performed on trigger destruction.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EDBA0)
// with the destructor expanded inline. The destructor itself (0x006ED3C0) is
// defined here: its frozen inventory bound is split at 0x006ED49A, so it is
// bound by reference until the bound is refined.

// The abstract list 0x00AC0D48 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> TriggerPtrTracker;

TriggerClass::~TriggerClass()
{
    Unknown_006ECB90(this, true);
    Triggers.Delete(this);
    AbstractTypePtrTracker.Delete((ObjectClass *)this);
    TriggerPtrTracker.Delete(this);
}
