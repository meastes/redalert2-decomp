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
// Function comment blocks from mischa85/RedAlert2 392e904 src/trigtype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// trigtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/trigtype.json carries the addresses and the rows that
// stay in their own files.


#define RA2_TRIGGER_TYPE_REMOVE_ACTION
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/object.hpp"
#include "shared/trigger_type.hpp"
#include "shared/taction.hpp"
#include "shared/house_type.hpp"


// Header-static dynamic initializers of the unit at 0x006EDCC0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006EDCC0-0x006EDEA6); the other
// drafts of this plan are emitted by these includes.
// TriggerTypeClass::Delete_Action is declared for this unit only (other units
// that include trigger_type.hpp are sensitive to the extra member).


// Dynamic initializer (0x006EDCF0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDD10, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDD30, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDD50, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDD70, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDD90, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDDD0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDE00, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDE30, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDE60, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDE70, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


// Dynamic initializer (0x006EDE90, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006edcc0.cpp; no source of its own.


/***********************************************************************************************
 * TriggerTypeClass::TriggerTypeClass -- Constructor for trigger class object.                 *
 *                                                                                             *
 *    This is the normal constructor for a trigger object. The trigger starts with no team     *
 *    members, no mission, and default values for all settings.                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EDEB0)
// Enabled on every difficulty, no house, next trigger, events or actions;
// listed in the abstract lists and numbered by its place in TriggerTypes.

TriggerTypeClass::TriggerTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    ArrayIndex(-1),
    IsEnabledOnEasy(true),
    IsEnabledOnMedium(true),
    IsEnabledOnHard(true),
    IsEnabled(true),
    IsToInherit(false),
    House(0),
    LinkedTo(0),
    FirstEvent(0),
    FirstAction(0)
{
    EventActionPtrTracker.Add(this);
    AbstractTypePtrTracker.Add(this);
    ArrayIndex = TriggerTypes.Count();
    TriggerTypes.Add(this);
}


/***********************************************************************************************
 * TriggerTypeClass::~TriggerTypeClass -- Deleting a trigger type deletes associated triggers. *
 *                                                                                             *
 *    When a trigger type is deleted, then all triggers that refer to that type must also      *
 *    be deleted as well. There can be no 'orphan' triggers in existence.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EE030)
// Detaches the type, leaves TriggerTypes, deletes its actions and events and
// leaves the abstract lists.

TriggerTypeClass::~TriggerTypeClass()
{
    Unknown_006ECB90(this, true);
    TriggerTypes.Delete(this);
    TActionClass *action = FirstAction;
    while (action != 0) {
        TActionClass *next = action->Next;
        delete action;
        action = next;
    }
    TEventClass *event = FirstEvent;
    while (event != 0) {
        TEventClass *next = event->Next;
        delete event;
        event = next;
    }
    AbstractTypePtrTracker.Delete(this);
    EventActionPtrTracker.Delete(this);
}


/// <summary>
/// Does this trigger watch for a vertical line crossing?
/// A trigger springs on any one of the events in its list. This routine picks the
/// vertical line crossing event out of that list.
/// </summary>
/// <returns>bool; Is one of this trigger's events a vertical line crossing?</returns>
// (0x006EE180)
// Whether the type has an Event of type 0x1A.

bool TriggerTypeClass::Is_Cross_Vertical() const
{
    bool result = false;
    for (TEventClass *item = FirstEvent; item != 0; item = item->Next) {
        if (item->Type == 0x1A) {
            result = true;
            break;
        }
    }
    return result;
}


/// <summary>
/// Does this trigger watch for a horizontal line crossing?
/// A trigger springs on any one of the events in its list. This routine picks the
/// horizontal line crossing event out of that list.
/// </summary>
/// <returns>bool; Is one of this trigger's events a horizontal line crossing?</returns>
// (0x006EE1B0)
// Whether the type has an Event of type 0x19.

bool TriggerTypeClass::Is_Cross_Horizontal() const
{
    bool result = false;
    for (TEventClass *item = FirstEvent; item != 0; item = item->Next) {
        if (item->Type == 0x19) {
            result = true;
            break;
        }
    }
    return result;
}


/// <summary>
/// Does this trigger watch for something entering its zone?
/// A trigger springs on any one of the events in its list. This routine picks the
/// enters zone event out of that list.
/// </summary>
/// <returns>bool; Is one of this trigger's events an enters zone event?</returns>
// (0x006EE1E0)
// Whether the type has an Event of type 0x18.

bool TriggerTypeClass::Is_Enters_Zone() const
{
    bool result = false;
    for (TEventClass *item = FirstEvent; item != 0; item = item->Next) {
        if (item->Type == 0x18) {
            result = true;
            break;
        }
    }
    return result;
}


/// <summary>
/// Does this trigger hold back the scenario victory?
/// The allow win action keeps the scenario from being won until every trigger that
/// carries it has sprung. This routine checks the trigger's action list for it.
/// </summary>
/// <returns>bool; Is one of this trigger's actions the allow win action?</returns>
// (0x006EE210)
// Whether the type has an Action of type 0xF.

bool TriggerTypeClass::Is_Allow_Win() const
{
    bool result = false;
    for (TActionClass *item = FirstAction; item != 0; item = item->Next) {
        if (item->Type == 0xF) {
            result = true;
            break;
        }
    }
    return result;
}


/// <summary>
/// Is this trigger tied to the specified global variable?
/// A trigger can watch for a global variable being set or cleared. This routine checks
/// the trigger's whole event list for either kind of watch on the variable given.
/// </summary>
/// <param name="global">The global variable number to check for.</param>
/// <returns>bool; Does one of this trigger's events watch that global variable?</returns>
// (0x006EE240)
// Whether the type has an event of type 0x1B or 0x1C with this data.

bool TriggerTypeClass::Is_Linked_To_Global(int value) const
{
    bool result = false;
    for (TEventClass *event = FirstEvent; event != 0; event = event->Next) {
        if ((event->Type == 0x1B || event->Type == 0x1C) && event->Data == value) {
            result = true;
            break;
        }
    }
    return result;
}


/// <summary>
/// Is this trigger tied to the specified local variable?
/// A trigger can watch for a local variable being set or cleared. This routine checks
/// the trigger's whole event list for either kind of watch on the variable given.
/// </summary>
/// <param name="local">The local variable number to check for.</param>
/// <returns>bool; Does one of this trigger's events watch that local variable?</returns>
// (0x006EE280)
// Whether the type has an event of type 0x24 or 0x25 with this data.

bool TriggerTypeClass::Is_Linked_To_Local(int value) const
{
    bool result = false;
    for (TEventClass *event = FirstEvent; event != 0; event = event->Next) {
        if ((event->Type == 0x24 || event->Type == 0x25) && event->Data == value) {
            result = true;
            break;
        }
    }
    return result;
}


/***********************************************************************************************
 * TriggerTypeClass::Detach -- Removes attachments to the target object specified.             *
 *                                                                                             *
 *    When an object disappears from the game, it must be detached from all other objects that *
 *    may be referring to it. This routine will detach the specified target object from any    *
 *    references to it in this trigger type class.                                             *
 *                                                                                             *
 * INPUT:   target   -- The target object to be detached from this trigger type.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EE2C0)
// Passes the detach on to the actions and events, then unlinks target.

void TriggerTypeClass::Detach(AbstractClass *target, bool all)
{
    if (LinkedTo != 0 && (AbstractClass *)LinkedTo == target) {
        LinkedTo = LinkedTo->LinkedTo;
    }
    for (TActionClass *action = FirstAction; action != 0; action = action->Next) {
        action->Detach(target, true);
    }
    if ((AbstractClass *)FirstAction == target) {
        FirstAction = FirstAction->Next;
    }
    for (TEventClass *event = FirstEvent; event != 0; event = event->Next) {
        event->Detach(target, true);
    }
    if ((AbstractClass *)FirstEvent == target) {
        FirstEvent = FirstEvent->Next;
    }
}


/***********************************************************************************************
 * TriggerTypeClass::From_Name -- Convert an ASCII name into a trigger type pointer.           *
 *                                                                                             *
 *    Given just an ASCII representation of the trigger type, this routine will return with    *
 *    a pointer to the trigger type it refers to. Typical use of this is when parsing          *
 *    scenario INI files.                                                                      *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the name to use to identify the trigger type class object to   *
 *                   be looked up.                                                             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the trigger type class object that matches the name      *
 *          specified. If no match could be found, then NULL is returned.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EE350)

TriggerTypeClass *__fastcall TriggerTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TriggerTypes.Count(); index++) {
            if (!_stricmp(TriggerTypes[index]->ID, name) ||
                !_stricmp(TriggerTypes[index]->Name, name)) {
                return TriggerTypes[index];
            }
        }
    }
    return 0;
}


/// <summary>
/// Converts a persistence name into a trigger persistence type.
/// The persistence decides whether a trigger dies after it springs, dies once all of
/// the objects it is attached to have sprung it, or lives on regardless.
/// </summary>
/// <returns>Returns with the persistence type of that name. A name that is not
/// recognized is taken to mean VOLATILE.</returns>
// (0x006EE3C0)
// The persistence named (0 when unknown).

int __fastcall TriggerTypeClass::Persistence_From_Name(char const *name)
{
    for (int persistence = 0; persistence < 3; persistence++) {
        if (!_stricmp(name, PersistenceNames[persistence])) {
            return persistence;
        }
    }
    return 0;
}


/// <summary>
/// Fetches the name of the specified trigger persistence.
/// This routine is used when a trigger is written back out to the scenario file.
/// </summary>
/// <returns>Returns with a pointer to the name of that persistence type.</returns>
// (0x006EE400)

char const *__fastcall TriggerTypeClass::Name_From_Persistence(int persistence)
{
    return PersistenceNames[persistence];
}


/***********************************************************************************************
 * TriggerTypeClass::Attaches_To -- Determines what trigger can attach to.                     *
 *                                                                                             *
 *    This routine will examine the trigger events and return with a composit bitfield that    *
 *    indicates what this trigger can be attached to. This is used for trigger placement       *
 *    and logic processing.                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with AttachType bitfield representing what this trigger can be attached    *
 *          to.                                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EE410)
// The combined flags of the events and actions of this and the later types.

int TriggerTypeClass::Attaches_To() const
{
    int flags = 0;
    for (TEventClass *event = FirstEvent; event != 0; event = event->Next) {
        flags |= TEventClass::Event_Flags(event->Type);
    }
    for (TActionClass *action = FirstAction; action != 0; action = action->Next) {
        flags |= TActionClass::Action_Flags(action->Type);
    }
    if (LinkedTo != 0) {
        return LinkedTo->Attaches_To() | flags;
    }
    return flags;
}


/***********************************************************************************************
 * TriggerTypeClass::Read_INI -- reads triggers from the INI file                              *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Triggername = Eventname, Actionname, Data, Housename, TeamName, IsPersistant           *
 *                                                                                             *
 * This routine reads in the triggers & creates them. Then, other classes can                  *
 * get pointers to the triggers they're linked to.                                             *
 *                                                                                             *
 * The routine relies on the TeamTypeClasses already being loaded so it can resolve            *
 * references to teams in this function.                                                       *
 *                                                                                             *
 * Cell Trigger pointers & IsTrigger flags are set in DisplayClass::Read_INI(),                *
 * and cleared in the Map::Init() routine (which clears all cell objects to 0's).              *
 *                                                                                             *
 * Object's pointers are set in:                                                               *
 *      InfantryClass::Read_INI()                                                              *
 *      BuildingClass::Read_INI()                                                              *
 *      UnitClass::Read_INI()                                                                  *
 *      TerrainClass::Read_INI()                                                               *
 * The object trigger pointers are cleared in the ObjectClass constructor.                     *
 *                                                                                             *
 * The House's EMSListOf triggers is set in this routine, and cleared in the                   *
 * HouseClass::Init() routine.                                                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      buffer      buffer to hold the INI data                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This function must be called before any other class's Read_INI.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
// (0x006EE470, slot 25)
// The trigger's [Triggers] line (house, next trigger, name, disabled, easy,
// normal, hard and the +0xA0 flag), then its event and action lists from
// [Events] and [Actions] (a count, then each entry's own fields). Events are
// pushed on the front of the list, actions appended in order.

bool TriggerTypeClass::Read_INI(CCINIClass &ini)
{
    char buffer[512];
    if (ini.Get_String(unknown_triggers_section_7ada50, ID, "", buffer, sizeof(buffer))) {
        char *token = strtok(buffer, ",");
        if (_stricmp(token, "<none>") == 0) {
            House = HouseTypes[0];
        } else {
            House = HouseTypes[HouseTypeClass::Find_Index(token)];
        }

        token = strtok(0, ",");
        LinkedTo = 0;
        if (_stricmp(token, "<none>") != 0) {
            LinkedTo = Find_Or_Make(token);
        }

        token = strtok(0, ",");
        Name = FixedString<49>(token);

        token = strtok(0, ",");
        if (token != 0 && atoi(token) == 0) {
            IsEnabled = true;
        } else {
            IsEnabled = false;
        }
        token = strtok(0, ",");
        if (token != 0 && atoi(token) != 0) {
            IsEnabledOnEasy = true;
        } else {
            IsEnabledOnEasy = false;
        }
        token = strtok(0, ",");
        if (token != 0 && atoi(token) != 0) {
            IsEnabledOnMedium = true;
        } else {
            IsEnabledOnMedium = false;
        }
        token = strtok(0, ",");
        if (token != 0 && atoi(token) != 0) {
            IsEnabledOnHard = true;
        } else {
            IsEnabledOnHard = false;
        }
        token = strtok(0, ",");
        if (token != 0 && atoi(token) != 0) {
            IsToInherit = true;
        }

        if (ini.Get_String(unknown_events_section_7ada58, ID, "", buffer, sizeof(buffer))) {
            int count = atoi(strtok(buffer, ","));
            for (; count != 0; count--) {
                TEventClass *event = new TEventClass();
                event->Read_INI();
                event->Next = FirstEvent;
                FirstEvent = event;
            }
        }

        if (ini.Get_String(unknown_actions_section_7ada54, ID, "", buffer, sizeof(buffer))) {
            int count = atoi(strtok(buffer, ","));
            TActionClass *last = 0;
            FirstAction = 0;
            for (; count != 0; count--) {
                TActionClass *action = new TActionClass();
                action->Read_INI();
                if (FirstAction == 0) {
                    FirstAction = action;
                } else {
                    last->Next = action;
                }
                last = action;
            }
        }
        return true;
    }
    return false;
}


// void __fastcall TriggerTypeClass::Read_Scenario_INI(CCINIClass &ini) (0x006EE7F0)
// Makes (or finds) the trigger type of every [Triggers] entry and reads it.

void __fastcall TriggerTypeClass::Read_Scenario_INI(CCINIClass &ini)
{
    int count = ini.Entry_Count(unknown_triggers_section_7ada50);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_triggers_section_7ada50, index);
        TriggerTypeClass *type = Find_Or_Make(entry);
        type->Read_INI(ini);
    }
}


/***********************************************************************************************
 * TriggerTypeClass::Write_INI -- Stores all trigger types to the INI database specified.      *
 *                                                                                             *
 *    This routine will write out all trigger type objects to the INI database. Any existing   *
 *    trigger types in the database will be cleared out.                                       *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to have the trigger types added.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EE8C0, slot 26)
// The inverse of Read_INI: the [Triggers] line, then the [Events] and
// [Actions] lines (count, then each entry appended after a comma).


extern "C" char *__cdecl strcat(char *dest, char const *source);
#pragma intrinsic(strcat)

extern char const *unknown_triggers_section_7ada50;
extern char const *unknown_actions_section_7ada54;
extern char const *unknown_events_section_7ada58;

bool TriggerTypeClass::Write_INI(CCINIClass &ini) const
{
    char buffer[512];
    sprintf(buffer, "%s,%s,%s,%d,%d,%d,%d,%d",
        House != 0 ? (char const *)House->ID : "<none>",
        LinkedTo != 0 ? (char const *)LinkedTo->ID : "<none>",
        (char const *)Name, IsEnabled ? 0 : 1, IsEnabledOnEasy ? 1 : 0, IsEnabledOnMedium ? 1 : 0, IsEnabledOnHard ? 1 : 0, IsToInherit ? 1 : 0);
    ini.Put_String(unknown_triggers_section_7ada50, ID, buffer);

    int count = 0;
    TEventClass *event;
    for (event = FirstEvent; event != 0; event = event->Next) {
        count++;
    }
    sprintf(buffer, "%d", count);
    for (event = FirstEvent; event != 0; event = event->Next) {
        strcat(buffer, ",");
        event->Build_INI_Entry(buffer);
    }
    ini.Put_String(unknown_events_section_7ada58, ID, buffer);

    count = 0;
    TActionClass *action;
    for (action = FirstAction; action != 0; action = action->Next) {
        count++;
    }
    sprintf(buffer, "%d", count);
    for (action = FirstAction; action != 0; action = action->Next) {
        strcat(buffer, ",");
        action->Build_INI_Entry(buffer);
    }
    ini.Put_String(unknown_actions_section_7ada54, ID, buffer);
    return true;
}


// void __fastcall TriggerTypeClass::Write_Scenario_INI(CCINIClass &ini) (0x006EEAA0)
// Clears the [Triggers], [Events] and [Actions] sections (and every section
// their entries name) and writes every trigger type. The entry values are
// always looked up under [Triggers].

void __fastcall TriggerTypeClass::Write_Scenario_INI(CCINIClass &ini)
{
    char buffer[32];
    int count = ini.Entry_Count(unknown_triggers_section_7ada50);
    int index;
    for (index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_triggers_section_7ada50, index);
        ini.Get_String(unknown_triggers_section_7ada50, entry, unknown_string_83c55c, buffer, sizeof(buffer));
        ini.Clear(buffer, 0);
    }
    ini.Clear(unknown_triggers_section_7ada50, 0);

    count = ini.Entry_Count(unknown_events_section_7ada58);
    for (index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_events_section_7ada58, index);
        ini.Get_String(unknown_triggers_section_7ada50, entry, unknown_string_83c55c, buffer, sizeof(buffer));
        ini.Clear(buffer, 0);
    }
    ini.Clear(unknown_events_section_7ada58, 0);

    count = ini.Entry_Count(unknown_actions_section_7ada54);
    for (index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_actions_section_7ada54, index);
        ini.Get_String(unknown_triggers_section_7ada50, entry, unknown_string_83c55c, buffer, sizeof(buffer));
        ini.Clear(buffer, 0);
    }
    ini.Clear(unknown_actions_section_7ada54, 0);

    for (index = 0; index < TriggerTypes.Count(); index++) {
        TriggerTypes[index]->Write_INI(ini);
    }
}


/// <summary>
/// Removes an action from this trigger.
/// The action is unlinked from the trigger's action list and destroyed. Handing over an
/// action that this trigger does not own is not an error -- the request is simply
/// refused.
/// </summary>
/// <param name="taction">Pointer to the action to remove. It is deleted if found.</param>
/// <returns>bool; Was the action found and removed?</returns>
// (0x006EEC00)
// Unlinks and deletes an action of the type.

bool TriggerTypeClass::Delete_Action(TActionClass *action)
{
    TActionClass *current = FirstAction;
    if (current == action) {
        FirstAction = action->Next;
        delete action;
        return true;
    }
    while (current != 0) {
        if (current->Next == action) {
            current->Next = current->Next->Next;
            delete action;
            return true;
        }
        current = current->Next;
    }
    return false;
}


/// <summary>
/// Removes an event from this trigger.
/// The event is unlinked from the trigger's event list and destroyed. Handing over an
/// event that this trigger does not own is not an error -- the request is simply
/// refused.
/// </summary>
/// <param name="tevent">Pointer to the event to remove. It is deleted if found.</param>
/// <returns>bool; Was the event found and removed?</returns>
// (0x006EEC60)
// Unlinks and deletes an event of the type.

bool TriggerTypeClass::Delete_Event(TEventClass *event)
{
    TEventClass *current = FirstEvent;
    if (current == event) {
        FirstEvent = event->Next;
        delete event;
        return true;
    }
    while (current != 0) {
        if (current->Next == event) {
            current->Next = current->Next->Next;
            delete event;
            return true;
        }
        current = current->Next;
    }
    return false;
}


/// <summary>
/// Fetches the trigger type of the specified name, creating it if necessary.
/// This routine is used while the scenario is being read, so that a trigger can be
/// referred to before its own declaration has been processed.
/// </summary>
/// <returns>Returns with a pointer to the trigger type that goes by that name.</returns>
// (0x006EECC0)
// The named trigger type, created when missing; 0 for "<none>" and "none".

TriggerTypeClass *__fastcall TriggerTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name) || !_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < TriggerTypes.Count(); index++) {
        if (!_stricmp(TriggerTypes[index]->ID, name)) {
            return TriggerTypes[index];
        }
    }
    return new TriggerTypeClass(name);
}


/// <summary>
/// Adds this trigger type's data to a running checksum.
/// A trigger is defined by the objects it hangs off, so the owning house, the linked
/// trigger and the heads of the event and action lists are submitted by identifier
/// rather than by pointer.
/// </summary>
/// <param name="crc">The checksum engine to submit the data to.</param>
// (0x006EED50)

void TriggerTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc(House->Fetch_ID());
    if (LinkedTo != 0) {
        crc(LinkedTo->Fetch_ID());
    }
    if (FirstEvent != 0) {
        crc(FirstEvent->Fetch_ID());
    }
    if (FirstAction != 0) {
        crc(FirstAction->Fetch_ID());
    }
}


/// <summary>
/// Fetches the class identifier of this object.
/// The save game system uses this identifier to know which kind of object to build
/// when the stream is read back in.
/// </summary>
/// <param name="retval">Pointer to the location to store the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no storage location was supplied.</returns>
// (0x006EEDD0)

HRESULT __stdcall TriggerTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TriggerTypeClass;
    return S_OK;
}


/// <summary>
/// Reads this trigger type in from the save game stream.
/// The object is rebuilt in place and its owning house, its linked trigger, and the
/// heads of its event and action lists are swizzled back into real pointers.
/// </summary>
/// <returns>Returns with S_OK if the trigger type was read, otherwise a failure
/// code.</returns>
// (0x006EEE10)

HRESULT __stdcall TriggerTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TriggerTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&LinkedTo);
    Swizzler.Swizzle((void **)&FirstEvent);
    Swizzler.Swizzle((void **)&FirstAction);
    Swizzler.Swizzle((void **)&House);
    return S_OK;
}


/// <summary>
/// Writes this trigger type out to the save game stream.
/// A trigger's events and actions are separate objects that are saved in their own
/// right, so nothing beyond the raw object image needs to be appended here.
/// </summary>
/// <param name="cleardirty">Should the object be marked as clean once written?</param>
/// <returns>Returns with S_OK if the trigger type was written, otherwise a failure
/// code.</returns>
// (0x006EEEA0)

HRESULT __stdcall TriggerTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// int TriggerTypeClass::Kind_Of() const (0x006EEEC0)

int TriggerTypeClass::Kind_Of() const
{
    return 39;
}


// int TriggerTypeClass::Size_Of() const (0x006EEED0)

int TriggerTypeClass::Size_Of() const
{
    return sizeof(TriggerTypeClass);
}


// int TriggerTypeClass::Fetch_Heap_ID() const (0x006EEEE0)

int TriggerTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// TriggerTypeClass scalar deleting destructor (0x006EEEF0): compiler generated; no source of its own.
