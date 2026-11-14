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


// ScriptClass::ScriptClass(ScriptTypeClass *type) (0x00668730)
// A script positioned before the first line of type; registered in Scripts.

ScriptClass::ScriptClass(ScriptTypeClass *type) :
    Class(type),
    unknown_28(0),
    CurrentMission(-1)
{
    Scripts.Add(this);
}


// ScriptClass::~ScriptClass() (0x006687D0)
// Releases references to the script and removes it from Scripts.

ScriptClass::~ScriptClass()
{
    Unknown_006ECB90(this, true);
    Scripts.Delete(this);
}


// void ScriptClass::Compute_CRC(WWCRCEngine &crc) const (0x00668850)

void ScriptClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc((long)CurrentMission);
}


// ScriptMissionClass ScriptClass::Get_Current_Mission() const (0x00668870)
// The current line, or (-1, 0) before the first.

ScriptMissionClass ScriptClass::Get_Current_Mission() const
{
    if (CurrentMission == -1) {
        return ScriptMissionClass(-1, 0);
    }
    return Class->MissionList[CurrentMission];
}


// ScriptMissionClass ScriptClass::Get_Next_Mission() const (0x006688B0)
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


// bool ScriptClass::Next_Mission() (0x00668920)
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


// HRESULT __stdcall ScriptClass::GetClassID(GUID *clsid) (0x00668960)

HRESULT __stdcall ScriptClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_ScriptClass;
    return S_OK;
}


// HRESULT __stdcall ScriptClass::Load(IStream *stream) (0x006689A0)
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


// HRESULT __stdcall ScriptClass::Save(IStream *stream, BOOL clear_dirty) (0x00668A00)

HRESULT __stdcall ScriptClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// ScriptTypeClass::ScriptTypeClass(char const *ini_name) (0x00668A20)
// An empty script type, registered in ScriptTypes; ArrayIndex is its index.

ScriptTypeClass::ScriptTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    IsGlobal(0),
    MissionCount(0)
{
    ScriptTypes.Add(this);
    ArrayIndex = ScriptTypes.ID(this);
}


// ScriptTypeClass::~ScriptTypeClass() (0x00668AE0)
// Releases references to the type and removes it from ScriptTypes.

ScriptTypeClass::~ScriptTypeClass()
{
    Unknown_006ECB90(this, true);
    ScriptTypes.Delete(this);
}


// bool ScriptTypeClass::Write_INI(CCINIClass &ini) const (0x00668B60)
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


// bool ScriptTypeClass::Read_INI(CCINIClass &ini) (0x00668C10)
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


// ScriptTypeClass *ScriptTypeClass::Find_Or_Make(char const *name) (0x00668F70)
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


// HRESULT __stdcall ScriptTypeClass::GetClassID(GUID *clsid) (0x006690C0)

HRESULT __stdcall ScriptTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_ScriptTypeClass;
    return S_OK;
}


// HRESULT __stdcall ScriptTypeClass::Load(IStream *stream) (0x00669100)
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


// HRESULT __stdcall ScriptTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x00669150)

HRESULT __stdcall ScriptTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// void ScriptTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x00669170)

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
