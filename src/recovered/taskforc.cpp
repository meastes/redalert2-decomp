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
// Function comment blocks from mischa85/RedAlert2 392e904 src/taskforc.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/taskforc.cpp where both define the function (names only).

// taskforc.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/taskforc.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/task_force.hpp"


// Header-static dynamic initializers of the unit at 0x006B6660:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006B6660-0x006B6846); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006B6690, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B66B0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B66D0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B66F0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B6710, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B6730, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B6770, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B67A0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B67D0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B6800, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B6810, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


// Dynamic initializer (0x006B6830, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006b6660.cpp; no source of its own.


/// <summary>
/// Creates a task force with the specified name.
/// The new force joins the global task force list and starts out empty, waiting for its
/// member list to arrive from the INI database.
/// </summary>
/// <param name="name">The INI name to give the new task force.</param>
// (0x006B6850)
// An empty task force named ini_name, registered in TaskForces.

TaskForceClass::TaskForceClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    Group(-1),
    CountEntries(0),
    IsGlobal(0)
{
    TaskForces.Add(this);
    for (int index = 0; index < 6; index++) {
        Entries[index] = TaskForceEntryStruct(0, 0);
    }
}


/// <summary>
/// Creates a task force without initializing it.
/// This constructor is used by the load process, which fills the object in from the save
/// game stream rather than building it up from scratch.
/// </summary>
// (0x006B6920)
// Load path: installs the vtables only.

TaskForceClass::TaskForceClass(NoInitClass const &noinit) :
    AbstractTypeClass(noinit)
{
}


/// <summary>
/// Destroys the task force.
/// Anything still pointing at this force is detached from it before the force drops out
/// of the global list.
/// </summary>
// (0x006B6950)
// Releases references to the task force and removes it from TaskForces.

TaskForceClass::~TaskForceClass()
{
    Unknown_006ECB90(this, true);
    TaskForces.Delete(this);
}


/// <summary>
/// Creates a copy of the specified task force.
/// </summary>
// (0x006B69D0)
// Copies the base type, the group, flag and the entries in use.

TaskForceClass::TaskForceClass(TaskForceClass const &that) :
    AbstractTypeClass(that)
{
    Group = that.Group;
    CountEntries = that.CountEntries;
    IsGlobal = that.IsGlobal;
    for (int index = 0; index < CountEntries; index++) {
        Entries[index] = that.Entries[index];
    }
}


// TaskForceClass TaskForceClass::operator=(TaskForceClass const &that) (0x006B6AC0)
// Copies the group, flag and entries (not the type's names) and returns a
// copy of the result by value.

TaskForceClass TaskForceClass::operator=(TaskForceClass const &that)
{
    if (this != &that) {
        Group = that.Group;
        CountEntries = that.CountEntries;
        IsGlobal = that.IsGlobal;
        for (int index = 0; index < CountEntries; index++) {
            Entries[index] = that.Entries[index];
        }
    }
    return *this;
}


// int TaskForceClass::Total_Amount() const (0x006B6B30)
// The number of objects the entries in use ask for.

int TaskForceClass::Total_Amount() const
{
    int total = 0;
    for (int index = 0; index < CountEntries; index++) {
        total += Entries[index].Amount;
    }
    return total;
}


// TaskForceClass *TaskForceClass::Find_By_ID(char const *name) (0x006B6B50)
// The task force whose ID matches name (case-insensitive), or null.

TaskForceClass *__fastcall TaskForceClass::Find_By_ID(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TaskForces.Count(); index++) {
            if (_stricmp(name, TaskForces[index]->ID) == 0) {
                return TaskForces[index];
            }
        }
    }
    return 0;
}


// TaskForceClass *TaskForceClass::Find_By_Name(char const *name) (0x006B6BA0)
// The task force whose Name matches name (case-insensitive), or null.

TaskForceClass *__fastcall TaskForceClass::Find_By_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TaskForces.Count(); index++) {
            if (_stricmp(name, TaskForces[index]->Name) == 0) {
                return TaskForces[index];
            }
        }
    }
    return 0;
}


// void TaskForceClass::Read_Scenario_INI(CCINIClass &ini, int is_global) (0x006B6BF0)
// Creates (or finds) every task force listed in [TaskForces], registers its
// saved ID and reads its own section.

void __fastcall TaskForceClass::Read_Scenario_INI(CCINIClass &ini, int is_global)
{
    char buffer[24];

    int count = ini.Entry_Count("TaskForces");
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry("TaskForces", index);
        if (ini.Get_String("TaskForces", entry, "", buffer, sizeof(buffer)) > 0) {
            TaskForceClass *taskforce = Find_Or_Make(buffer);
            Swizzler.Here_I_Am(ahtoi(buffer), taskforce);
            taskforce->Read_INI(ini);
            taskforce->IsGlobal = is_global;
        }
    }
}


// void TaskForceClass::Write_Scenario_INI(CCINIClass &ini, int is_global) (0x006B6D10)
// Clears the old [TaskForces] list and its sections, then lists and writes
// every task force whose IsGlobal matches.

void __fastcall TaskForceClass::Write_Scenario_INI(CCINIClass &ini, int is_global)
{
    char buffer[32];

    int count = ini.Entry_Count("TaskForces");
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry("TaskForces", index);
        ini.Get_String("TaskForces", entry, "", buffer, sizeof(buffer));
        ini.Clear(buffer);
    }
    ini.Clear("TaskForces");

    int written = 0;
    for (int tf = 0; tf < TaskForces.Count(); tf++) {
        TaskForceClass *taskforce = TaskForces[tf];
        if (taskforce->IsGlobal == is_global) {
            sprintf(buffer, "%d", written++);
            strtrim(buffer);
            ini.Put_String("TaskForces", buffer, taskforce->ID);
            taskforce->Write_INI(ini);
        }
    }
}


/// <summary>
/// Reads this task force from the INI database.
/// The member list is rebuilt from the numbered entries in the force's own section. A
/// member that names an object type the game does not recognize is quietly dropped.
/// </summary>
/// <returns>bool; Was the task force read in?</returns>
// (0x006B6DF0)
// Reads the entries "0=" .. "5=" (count,type; entries without a known type
// are dropped) and "Group=".

bool TaskForceClass::Read_INI(CCINIClass &ini)
{
    char entry[32];
    char buffer[128];

    if (AbstractTypeClass::Read_INI(ini)) {
        CountEntries = 0;
        for (int index = 0; index < 6; index++) {
            sprintf(entry, "%d", index);
            if (ini.Get_String(ID, entry, "", buffer, sizeof(buffer)) > 0) {
                Entries[CountEntries] = TaskForceEntryStruct(buffer);
                if (Entries[CountEntries].Type != 0) {
                    CountEntries++;
                }
            }
        }
        Group = ini.Get_Int(ID, "Group", Group);
        return true;
    }
    return false;
}


/// <summary>
/// Writes this task force out to the INI database.
/// Every member slot is dealt with, and the slots this force does not use are cleared,
/// so that no leftover member from an earlier version of the force survives.
/// </summary>
/// <returns>bool; Was the task force written out?</returns>
// (0x006B6ED0)
// Writes the entries in use, clears the unused ones, and "Group=".

bool TaskForceClass::Write_INI(CCINIClass &ini) const
{
    char entry[32];

    if (AbstractTypeClass::Write_INI(ini)) {
        for (int index = 0; index < 6; index++) {
            sprintf(entry, "%d", index);
            if (index < CountEntries) {
                ini.Put_String(ID, entry, Entries[index].Text());
            } else {
                ini.Clear(ID, entry);
            }
        }
        ini.Put_Int(ID, "Group", Group);
        return true;
    }
    return false;
}


// bool TaskForceClass::unknown_all_kind16_6b6f70() const (0x006B6F70)
// True when the type of every entry in use reports Kind_Of() == 16.

bool TaskForceClass::unknown_all_kind16_6b6f70() const
{
    for (int index = 0; index < CountEntries; index++) {
        if (Entries[index].Type->Kind_Of() != 16) {
            return false;
        }
    }
    return true;
}


/// <summary>
/// Fetches the task force of the specified name, creating it if need be.
/// This routine is used while reading a scenario, so that a task force can be referred
/// to before its own section has been reached.
/// </summary>
/// <returns>Returns with a pointer to the task force found or created.</returns>
// (0x006B6FB0)
// The task force with this ID, created when missing; null for "<none>" and
// "none".

TaskForceClass *__fastcall TaskForceClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < TaskForces.Count(); index++) {
        if (_stricmp(TaskForces[index]->ID, name) == 0) {
            return TaskForces[index];
        }
    }
    return new TaskForceClass(name);
}


/// <summary>
/// Saves this task force to the save game stream.
/// </summary>
/// <returns>Returns with the result code of the save operation.</returns>
// (0x006B7040)

HRESULT __stdcall TaskForceClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = AbstractClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Loads this task force from the save game stream.
/// The member object types are remembered as raw identifiers in the save file, so they
/// are handed to the swizzler to be turned back into pointers once the rest of the file
/// has been read in.
/// </summary>
/// <returns>Returns with the result code of the load operation.</returns>
// (0x006B7060)
// Restores the task force in place and swizzles the entry types.

HRESULT __stdcall TaskForceClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TaskForceClass(NoInitClass());
        for (int index = 0; index < 6; index++) {
            Swizzler.Swizzle((void **)&Entries[index].Type);
        }
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is used by the save game code so that an object of the right kind can
/// be created when the game is loaded back in.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x006B70D0)

HRESULT __stdcall TaskForceClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_TaskForceClass;
    return S_OK;
}


/// <summary>
/// Adds the state of this task force to the running game checksum.
/// This routine is used by the multiplayer sync checking code.
/// </summary>
// (0x006B7110)

void TaskForceClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)Group);
    crc((long)CountEntries);
}


// int TaskForceClass::Max_Tech_Level() const (0x006B7140)
// The highest TechLevel of the entry types; a type with -1 counts as 11 in
// a game mode other than 0.

int TaskForceClass::Max_Tech_Level() const
{
    int level = 0;
    for (int index = 0; index < CountEntries; index++) {
        int techlevel = Entries[index].Type->TechLevel;
        if (techlevel > level) {
            level = techlevel;
        } else if (techlevel == -1 && unknown_game_mode_a3d298 != 0) {
            level = 11;
        }
    }
    return level;
}


// int TaskForceClass::Kind_Of() const (0x006B7190)
// Compiler-emitted from shared/task_force.hpp in this unit; no source of its own.


// int TaskForceClass::Size_Of() const (0x006B71A0)
// Compiler-emitted from shared/task_force.hpp in this unit; no source of its own.


// void *TaskForceClass::`scalar deleting destructor'(unsigned int) (0x006B71B0)
// Compiler-emitted from shared/task_force.hpp in this unit; no source of its own.
