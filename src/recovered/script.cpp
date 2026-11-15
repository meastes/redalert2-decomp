/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/script.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/script.cpp where both define the function (names only).

// script.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/script.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/script.hpp"


// Header-static dynamic initializers of the unit at 0x00668540:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00668540-0x00668726); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00668570, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x00668590, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x006685B0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x006685D0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x006685F0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x00668610, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x00668650, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x00668680, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x006686B0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x006686E0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x006686F0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


// Dynamic initializer (0x00668710, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00668540.cpp; no source of its own.


/// <summary>
/// Creates a running instance of the specified script type.
/// This routine is used when a team is formed and needs its own copy of the script's
/// execution state. The new script is registered with the master script list and starts
/// out positioned before the first mission of its type.
/// </summary>
/// <param name="type">Pointer to the script type that supplies the mission list.</param>
// (0x00668730)
// A script positioned before the first line of type; registered in Scripts.

ScriptClass::ScriptClass(ScriptTypeClass *type) :
    Class(type),
    unknown_28(0),
    CurrentMission(-1)
{
    Scripts.Add(this);
}


/// <summary>
/// Destroys the script.
/// This routine will sever any reference the game objects hold to this script before it
/// is dropped from the master script list.
/// </summary>
// (0x006687D0)
// Releases references to the script and removes it from Scripts.

ScriptClass::~ScriptClass()
{
    Unknown_006ECB90(this, true);
    Scripts.Delete(this);
}


/// <summary>
/// Adds this script's state to the game state checksum.
/// This routine is used by the multiplayer synchronization checker to prove that every
/// machine has its teams at the same point in the same script.
/// </summary>
/// <param name="crc">The checksum engine to submit the script state to.</param>
// (0x00668850)

void ScriptClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc((long)CurrentMission);
}


/// <summary>
/// Fetches the mission the script is presently on.
/// This routine is used by the team logic to discover what orders it should be carrying
/// out at this moment.
/// </summary>
/// <returns>Returns with the current team mission. If the script has not been started, a
/// TMISSION_NONE mission is returned.</returns>
// (0x00668870)
// The current line, or (-1, 0) before the first.

ScriptMissionClass ScriptClass::Get_Current_Mission() const
{
    if (CurrentMission == -1) {
        return ScriptMissionClass(-1, 0);
    }
    return Class->MissionList[CurrentMission];
}


/// <summary>
/// Fetches the mission that follows the current one.
/// Use this routine to peek ahead in the script so that a team can prepare for what it
/// will be asked to do next.
/// </summary>
/// <returns>Returns with the next team mission. If the script has nothing further to ask
/// for, a TMISSION_NONE mission is returned.</returns>
// (0x006688B0)
// The line after the current one, or (-1, 0) past the end.

ScriptMissionClass ScriptClass::Get_Next_Mission() const
{
    if (CurrentMission + 1 >= Class->MissionCount) {
        return ScriptMissionClass(-1, 0);
    }
    return Class->MissionList[CurrentMission + 1];
}


// bool ScriptClass::Reset() (0x00668900)

bool ScriptClass::Reset()
{
    CurrentMission = -1;
    return true;
}


// bool ScriptClass::Set_Mission(int mission) (0x00668910)

bool ScriptClass::Set_Mission(int mission)
{
    CurrentMission = mission;
    return true;
}


/// <summary>
/// Advances the script to the mission that follows.
/// This routine is called by the team logic once the current mission has been satisfied
/// and the team is ready to be given its next orders.
/// </summary>
/// <returns>bool; Is there a mission waiting at the new line?</returns>
// (0x00668920)
// Advances to the next line; true while it exists (unsigned compare).

bool ScriptClass::Next_Mission()
{
    return (unsigned)++CurrentMission < (unsigned)Class->MissionCount;
}


// bool ScriptClass::Has_Mission() const (0x00668940)
// True while the current line exists (unsigned compare).

bool ScriptClass::Has_Mission() const
{
    return (unsigned)CurrentMission < (unsigned)Class->MissionCount;
}


/// <summary>
/// Fetches the class identifier used to persist this script.
/// This routine is part of the IPersistStream contract that the save game system relies
/// on to recreate objects when a game is loaded.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x00668960)

HRESULT __stdcall ScriptClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_ScriptClass;
    return S_OK;
}


/// <summary>
/// Loads the script from a save game stream.
/// This routine will rebuild the virtual table pointers and remap the script type
/// reference onto whatever address that type occupies in the freshly loaded game.
/// </summary>
/// <param name="stream">The stream to read the script from.</param>
/// <returns>Returns with S_OK if the script was loaded, otherwise the failure code from
/// the base class.</returns>
// (0x006689A0)
// Restores the script in place and swizzles its type.

HRESULT __stdcall ScriptClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) ScriptClass(NoInitClass());
        Swizzler.Swizzle((void **)&Class);
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Saves the script to a save game stream.
/// </summary>
/// <param name="stream">The stream to write the script to.</param>
/// <param name="cleardirty">Should the object be marked as unmodified afterward?</param>
/// <returns>Returns with S_OK if the script was written, otherwise the failure code from
/// the base class.</returns>
// (0x00668A00)

HRESULT __stdcall ScriptClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = AbstractClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Creates a script type of the specified name.
/// This routine is used as the scenario's script list is read in. The new type is added
/// to the master script type heap and takes its heap identifier from it.
/// </summary>
/// <param name="name">The INI name to give this script type.</param>
// (0x00668A20)
// An empty script type, registered in ScriptTypes; ArrayIndex is its index.

ScriptTypeClass::ScriptTypeClass(char const *name) :
    AbstractTypeClass(name),
    IsGlobal(0),
    MissionCount(0)
{
    ScriptTypes.Add(this);
    ArrayIndex = ScriptTypes.ID(this);
}


/// <summary>
/// Destroys the script type.
/// This routine will sever any reference the game objects hold to this script type
/// before it is dropped from the master script type list.
/// </summary>
// (0x00668AE0)
// Releases references to the type and removes it from ScriptTypes.

ScriptTypeClass::~ScriptTypeClass()
{
    Unknown_006ECB90(this, true);
    ScriptTypes.Delete(this);
}


/// <summary>
/// Writes this script type's mission list to the INI database.
/// This routine is used as a scenario is saved. Any mission slot this script does not
/// use is cleared out, so that no stale lines are left behind from an earlier version of
/// the script.
/// </summary>
/// <param name="ini">The INI database to write the mission list to.</param>
/// <returns>bool; Was the script type written out?</returns>
// (0x00668B60)
// Writes the lines in use ("%d,%d") and clears the rest of "0=" .. "49=".

bool ScriptTypeClass::Write_INI(CCINIClass &ini) const
{
    char entry[16];
    char buffer[128];

    if (AbstractTypeClass::Write_INI(ini)) {
        for (int index = 0; index < 50; index++) {
            sprintf(entry, "%d", index);
            strtrim(entry);
            if (index < MissionCount) {
                MissionList[index].Build_INI_Entry(buffer);
                ini.Put_String(ID, entry, buffer);
            } else {
                ini.Clear(ID, entry);
            }
        }
        return true;
    }
    return false;
}


/// <summary>
/// Reads this script type's mission list from the INI database.
/// This routine is used as a scenario is loaded. The numbered entries in the script's own
/// section supply the missions the teams assigned this script will carry out.
/// </summary>
/// <param name="ini">The INI database to read the mission list from.</param>
/// <returns>bool; Was the script type read in?</returns>
// (0x00668C10)
// Reads the lines "0=" .. "49=" ("%d,%d"), keeping at most 50.

bool ScriptTypeClass::Read_INI(CCINIClass &ini)
{
    char entry[16];
    char buffer[128];

    if (AbstractTypeClass::Read_INI(ini)) {
        MissionCount = 0;
        for (int index = 0; index < 50; index++) {
            sprintf(entry, "%d", index);
            strtrim(entry);
            if (ini.Get_String(ID, entry, "", buffer, sizeof(buffer)) > 0) {
                ScriptMissionClass mission;
                mission.Read_INI_Entry(buffer);
                if (MissionCount < 50) {
                    MissionList[MissionCount] = mission;
                    MissionCount++;
                }
            }
        }
        return true;
    }
    return false;
}


// void ScriptTypeClass::Read_Scenario_INI(CCINIClass &ini, int is_global) (0x00668CE0)
// Creates (or finds) every script type listed in [ScriptTypes] and reads
// its own section.

void __fastcall ScriptTypeClass::Read_Scenario_INI(CCINIClass &ini, int is_global)
{
    char buffer[24];

    int count = ini.Entry_Count("ScriptTypes");
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry("ScriptTypes", index);
        if (ini.Get_String("ScriptTypes", entry, "", buffer, sizeof(buffer)) > 0) {
            ScriptTypeClass *script = Find_Or_Make(buffer);
            script->Read_INI(ini);
            script->IsGlobal = is_global;
        }
    }
}


class Leaf_668ea0 { public: void get(); };
void Leaf_668ea0::get() {  }


// ScriptTypeClass *ScriptTypeClass::Find_By_ID_Or_Name(char const *name) (0x00668EB0)
// The first script type whose ID or Name matches name (case-insensitive).

ScriptTypeClass *__fastcall ScriptTypeClass::Find_By_ID_Or_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < ScriptTypes.Count(); index++) {
            if (_stricmp(name, ScriptTypes[index]->ID) == 0 || _stricmp(name, ScriptTypes[index]->Name) == 0) {
                return ScriptTypes[index];
            }
        }
    }
    return 0;
}


// ScriptTypeClass *ScriptTypeClass::Find_By_Name(char const *name) (0x00668F20)
// The script type whose Name matches name (case-insensitive), or null.

ScriptTypeClass *__fastcall ScriptTypeClass::Find_By_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < ScriptTypes.Count(); index++) {
            if (_stricmp(name, ScriptTypes[index]->Name) == 0) {
                return ScriptTypes[index];
            }
        }
    }
    return 0;
}


/// <summary>
/// Fetches the script type of the specified name, creating it if necessary.
/// This routine is used while a scenario is being read, where a script can be referred
/// to before its own section has been reached.
/// </summary>
/// <param name="name">The name of the script type to find or create.</param>
/// <returns>Returns with a pointer to the script type.</returns>
// (0x00668F70)
// The script type with this ID, created when missing; null for "<none>"
// and "none".

ScriptTypeClass *__fastcall ScriptTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < ScriptTypes.Count(); index++) {
        if (_stricmp(ScriptTypes[index]->ID, name) == 0) {
            return ScriptTypes[index];
        }
    }
    return new ScriptTypeClass(name);
}


/// <summary>
/// Fetches the class identifier used to persist this script type.
/// This routine is part of the IPersistStream contract that the save game system relies
/// on to recreate objects when a game is loaded.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x006690C0)

HRESULT __stdcall ScriptTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_ScriptTypeClass;
    return S_OK;
}


/// <summary>
/// Loads the script type from a save game stream.
/// This routine will rebuild the virtual table pointers of the script type as it comes
/// back off the stream.
/// </summary>
/// <param name="stream">The stream to read the script type from.</param>
/// <returns>Returns with S_OK if the script type was loaded, otherwise the failure code
/// from the base class.</returns>
// (0x00669100)
// Restores the type in place.

HRESULT __stdcall ScriptTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) ScriptTypeClass(NoInitClass());
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Saves the script type to a save game stream.
/// </summary>
/// <param name="stream">The stream to write the script type to.</param>
/// <param name="cleardirty">Should the object be marked as unmodified afterward?</param>
/// <returns>Returns with S_OK if the script type was written, otherwise the failure code
/// from the base class.</returns>
// (0x00669150)

HRESULT __stdcall ScriptTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = AbstractClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Adds this script type's state to the game state checksum.
/// This routine is used by the multiplayer synchronization checker to prove that every
/// machine is running with the same script definitions.
/// </summary>
/// <param name="crc">The checksum engine to submit the script type state to.</param>
// (0x00669170)

void ScriptTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)MissionCount);
}


// void ScriptTypeClass::Detach(AbstractClass *target, bool all) (0x006691A0)
// When a team type goes away, lines with action 0x12 that name a later team
// type index move down by one; lines naming it fall back to 0.

void ScriptTypeClass::Detach(AbstractClass *target, bool all)
{
    if (target != 0 && target->Kind_Of() == 0x23) {
        int index = -1;
        for (int line = 0; line < MissionCount; line++) {
            if (MissionList[line].Mission == 0x12) {
                if (index == -1) {
                    index = TeamTypes.ID((TeamTypeClass *)target);
                    if (index == -1) {
                        return;
                    }
                }
                if (MissionList[line].Data > index) {
                    MissionList[line].Data--;
                } else if (MissionList[line].Data == index) {
                    MissionList[line].Data = 0;
                }
            }
        }
    }
}


// ScriptClass::Kind_Of (0x00669230)
// Compiler-emitted from shared/script.hpp in this unit; no source of its own.


// ScriptClass::Size_Of (0x00669240)
// Compiler-emitted from shared/script.hpp in this unit; no source of its own.


// ScriptClass::scalar_deleting_destructor (0x00669250)
// Compiler-emitted from shared/script.hpp in this unit; no source of its own.


// ScriptTypeClass::Kind_Of (0x006692E0)
// Compiler-emitted from shared/script.hpp in this unit; no source of its own.


// ScriptTypeClass::Size_Of (0x006692F0)
// Compiler-emitted from shared/script.hpp in this unit; no source of its own.


// ScriptTypeClass::Fetch_Heap_ID (0x00669300)
// Compiler-emitted from shared/script.hpp in this unit; no source of its own.


// ScriptTypeClass::scalar_deleting_destructor (0x00669310)
// Compiler-emitted from shared/script.hpp in this unit; no source of its own.
