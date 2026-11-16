/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/tagtype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/tagtype.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/tagtype.cpp where both functions declare the same local types (names only).

// tagtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tagtype.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x006B4300 and the
// unit's TagTypeClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/object.hpp"

// The unit's list of TagTypeClass objects (0x00ABFEA0; name is a hypothesis).
DynamicVectorClass<TagTypeClass *> TagTypes;


// Dynamic initializer (0x006B4330, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B4350, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B4370, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B4390, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B43B0, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B43D0, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B4410, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B4440, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B4470, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B44A0, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B44B0, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B44D0, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B44F0, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


// Dynamic initializer (0x006B4530, _$E) emitted by HeaderStatics_diagonal_init_006b4300.cpp; no source of its own.


/// <summary>
/// Constructs a tag type of the name specified.
/// The new tag type is added to the master tag list and to the abstract type tracker,
/// so that the scenario reader and the save system can find it later.
/// </summary>
// (0x006B4570)
// A volatile tag with no triggers, named after its ID; registered in the tag
// type list (its index) and the abstract list.

TagTypeClass::TagTypeClass(char const *name) :
    AbstractTypeClass(name),
    ArrayIndex(-1),
    Persistence(0),
    FirstTrigger(0)
{
    Name = FixedString<49>(ID);
    ArrayIndex = TagTypes.Count();
    TagTypes.Add(this);
    AbstractTypePtrTracker.Add(this);
}


/// <summary>
/// Destroys this tag type.
/// This routine will delete the trigger types hanging off the tag as well as any tag
/// instances that were built from it, then take the tag type back out of the master
/// tag list.
/// </summary>
// (0x006B46B0)
// Deletes the type's trigger types and every tag of this type.

TagTypeClass::~TagTypeClass()
{
    Detach_This_From_All(this, true);
    TriggerTypeClass *trigtype = FirstTrigger;
    FirstTrigger = 0;
    while (trigtype != 0) {
        TriggerTypeClass *linked = trigtype->LinkedTo;
        trigtype->LinkedTo = 0;
        delete trigtype;
        trigtype = linked;
    }
    for (int index = 0; index < Tags.Count(); index++) {
        if (Tags[index]->Class == this) {
            delete Tags[index];
            index--;
        }
    }
    AbstractTypePtrTracker.Delete((ObjectClass *)this);
    TagTypes.Delete(this);
}


/// <summary>
/// Adds a trigger type to this tag's trigger list.
/// The tag fires every trigger hanging off it, so this is how a designer gives one
/// tag more than one thing to do.
/// </summary>
/// <returns>bool; Was the trigger attached?</returns>
// (0x006B47E0)

bool TagTypeClass::Link(TriggerTypeClass *trigger)
{
    if (trigger != 0) {
        trigger->LinkedTo = FirstTrigger;
        FirstTrigger = trigger;
        return true;
    }
    return false;
}


/// <summary>
/// Removes a trigger type from this tag's trigger list.
/// This routine is used by the scenario editor when the designer breaks the tie
/// between a tag and one of its triggers.
/// </summary>
/// <returns>bool; Was the trigger found and unhooked?</returns>
// (0x006B4810)

bool TagTypeClass::Unlink(TriggerTypeClass *trigger)
{
    TriggerTypeClass *trigtype = FirstTrigger;
    if (trigtype == trigger) {
        FirstTrigger = trigger->LinkedTo;
        return true;
    }
    while (trigtype != 0) {
        if (trigtype->LinkedTo == trigger) {
            trigtype->LinkedTo = trigger->LinkedTo;
            return true;
        }
        trigtype = trigtype->LinkedTo;
    }
    return false;
}


/// <summary>
/// Removes any reference this tag type holds to the object specified.
/// This routine is called when an object is being destroyed, so that the tag is not
/// left pointing at a corpse.
/// </summary>
// (0x006B4860)

void TagTypeClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)FirstTrigger == target) {
        FirstTrigger = FirstTrigger->LinkedTo;
    }
}


/// <summary>
/// Fetches the tag type that goes by the name specified.
/// Both the database name and the display name are considered, and the comparison
/// ignores case.
/// </summary>
/// <returns>Returns with a pointer to the tag type found, or NULL if there is no match.</returns>
// (0x006B4880)
// The tag type whose ID or name matches, or 0.

TagTypeClass *__fastcall TagTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TagTypes.Count(); index++) {
            TagTypeClass *ttptr = TagTypes[index];
            if (!_stricmp(ttptr->ID, name) || !_stricmp(ttptr->Name, name)) {
                return ttptr;
            }
        }
    }
    return 0;
}


// void __fastcall TagTypeClass::Read_Scenario_INI(CCINIClass &ini) (0x006B48E0)
// Makes (or finds) the tag type of every [Tags] entry, registers its ID for
// pointer fixups and reads it.

void __fastcall TagTypeClass::Read_Scenario_INI(CCINIClass &ini)
{
    int count = ini.Entry_Count(TagTypeClass_INI_NAME);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(TagTypeClass_INI_NAME, index);
        char buffer[24];
        if (ini.Get_String(TagTypeClass_INI_NAME, entry, unknown_string_83c55c, buffer, sizeof(buffer)) > 0) {
            TagTypeClass *type = Find_Or_Make(entry);
            Swizzler.Here_I_Am(ahtoi(entry), type);
            type->Read_INI(ini);
        }
    }
}


// void __fastcall TagTypeClass::Write_Scenario_INI(CCINIClass &ini) (0x006B49F0)
// Clears the [Tags] entries and the section, then writes every tag type.

void __fastcall TagTypeClass::Write_Scenario_INI(CCINIClass &ini)
{
    int count = ini.Entry_Count(TagTypeClass_INI_NAME);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        char const *entry = ini.Get_Entry(TagTypeClass_INI_NAME, index);
        ini.Get_String(TagTypeClass_INI_NAME, entry, unknown_string_83c55c, buffer, sizeof(buffer));
        ini.Clear(buffer, 0);
    }
    ini.Clear(TagTypeClass_INI_NAME, 0);
    for (int i = 0; i < TagTypes.Count(); i++) {
        TagTypes[i]->Write_INI(ini);
    }
}


/// <summary>
/// Reads this tag type's data from the INI database.
/// This routine fetches the persistence, the display name, and the trigger type that
/// the tag fires.
/// </summary>
/// <returns>bool; Was an entry for this tag found in the database?</returns>
// (0x006B4A90)
// The [Tags] entry "persistence,name,trigger": the persistence, the
// display name and the first trigger type (made when missing).
#include "shared/trigger_type.hpp"

extern "C" char *__cdecl strtok(char *string, char const *delimiters);

bool TagTypeClass::Read_INI(CCINIClass &ini)
{
    char buffer[128];
    if (ini.Get_String(TagTypeClass_INI_NAME, ID, unknown_string_83c55c, buffer, sizeof(buffer))) {
        strtok(buffer, ",");
        Persistence = atoi(buffer);
        Name = FixedString<49>(strtok(0, ","));
        FirstTrigger = TriggerTypeClass::Find_Or_Make(strtok(0, ","));
        return true;
    }
    return false;
}


/// <summary>
/// Writes this tag type out to the INI database.
/// The tag is stored as its persistence, its display name, and the trigger it is
/// linked to.
/// </summary>
// (0x006B4B70)

bool TagTypeClass::Write_INI(CCINIClass &ini) const
{
    char buffer[128];
    if (FirstTrigger == 0) {
        wsprintfA(buffer, "%s,<none>", (char const *)Name);
        ini.Put_String(TagTypeClass_INI_NAME, ID, buffer);
    } else {
        wsprintfA(buffer, "%d,%s,%s", Persistence, (char const *)Name, (char const *)FirstTrigger->ID);
        ini.Put_String(TagTypeClass_INI_NAME, ID, buffer);
    }
    return true;
}


/// <summary>
/// Fetches what this tag may be attached to.
/// This routine gathers the attachment types of every trigger hanging off the tag, so
/// that the scenario reader knows whether the tag belongs to a cell, an object, or the
/// house itself.
/// </summary>
/// <returns>Returns with the combined attachment flags of the attached triggers.</returns>
// (0x006B4C00)
// The combined flags of the type's trigger types.

int TagTypeClass::Attaches_To() const
{
    int flags = 0;
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
        flags |= trigger->Attaches_To();
    }
    return flags;
}


/// <summary>
/// Can this tag grant a victory?
/// This routine asks the triggers attached to the tag, since it is they that carry
/// the win action.
/// </summary>
/// <returns>bool; Is one of the attached triggers able to win the scenario?</returns>
// (0x006B4C30)
// Whether any trigger type answers TriggerTypeClass::Is_Allow_Win.

bool TagTypeClass::Is_Allow_Win() const
{
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Is_Allow_Win()) {
            return true;
        }
    }
    return false;
}


/// <summary>
/// Does this tag watch for a horizontal line crossing?
/// This routine asks the triggers attached to the tag, since it is they that carry
/// the events.
/// </summary>
/// <returns>bool; Is one of the attached triggers a horizontal crossing trigger?</returns>
// (0x006B4C60)
// Whether any trigger type answers TriggerTypeClass::Is_Cross_Horizontal.

bool TagTypeClass::Is_Cross_Horizontal() const
{
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Is_Cross_Horizontal()) {
            return true;
        }
    }
    return false;
}


/// <summary>
/// Does this tag watch for a vertical line crossing?
/// This routine asks the triggers attached to the tag, since it is they that carry
/// the events.
/// </summary>
/// <returns>bool; Is one of the attached triggers a vertical crossing trigger?</returns>
// (0x006B4C90)
// Whether any trigger type answers TriggerTypeClass::Is_Cross_Vertical.

bool TagTypeClass::Is_Cross_Vertical() const
{
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Is_Cross_Vertical()) {
            return true;
        }
    }
    return false;
}


/// <summary>
/// Does this tag watch for an object entering a zone?
/// This routine asks the triggers attached to the tag, since it is they that carry
/// the events.
/// </summary>
/// <returns>bool; Is one of the attached triggers an enters zone trigger?</returns>
// (0x006B4CC0)
// Whether any trigger type answers TriggerTypeClass::Is_Enters_Zone.

bool TagTypeClass::Is_Enters_Zone() const
{
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Is_Enters_Zone()) {
            return true;
        }
    }
    return false;
}


/// <summary>
/// Is the specified trigger type attached to this tag?
/// </summary>
/// <returns>bool; Was the trigger found in this tag's trigger list?</returns>
// (0x006B4CF0)

bool TagTypeClass::Is_Linked(TriggerTypeClass *trigger) const
{
    for (TriggerTypeClass *trigtype = FirstTrigger; trigtype != 0; trigtype = trigtype->LinkedTo) {
        if (trigtype == trigger) {
            return true;
        }
    }
    return false;
}


/// <summary>
/// Fetches the tag type of the name specified, creating it if need be.
/// This routine is used while reading a scenario, where a tag may be referred to
/// before the tag section itself has been processed.
/// </summary>
/// <returns>Returns with a pointer to the tag type, or NULL if one could not be made.</returns>
// (0x006B4D20)
// The named tag type, created when missing; 0 for "<none>" and "none".

TagTypeClass *__fastcall TagTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name) || !_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < TagTypes.Count(); index++) {
        if (!_stricmp(TagTypes[index]->ID, name)) {
            return TagTypes[index];
        }
    }
    return new TagTypeClass(name);
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is used by the save and load system so that a tag type can be
/// recognized when it is read back out of a stream.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x006B4DB0)

HRESULT __stdcall TagTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_TagTypeClass;
    return S_OK;
}


/// <summary>
/// Adds this tag type's data to the CRC calculation.
/// This routine is used by the multiplayer sync checker to prove that every machine
/// holds the same tag types.
/// </summary>
// (0x006B4DF0)

void TagTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)Persistence);
}


/// <summary>
/// Loads this tag type from the data stream.
/// This routine will rebuild the object in place so that its virtual table is valid,
/// then remap the saved trigger pointer so that the tag's trigger list refers to the
/// trigger types of this session.
/// </summary>
/// <returns>Returns with S_OK if the tag type was loaded, otherwise the failure code.</returns>
// (0x006B4E20)

HRESULT __stdcall TagTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TagTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&FirstTrigger);
    return S_OK;
}


/// <summary>
/// Saves this tag type to the data stream.
/// The base class performs the actual write; this routine exists so that the tag type
/// takes part in the object persistence chain.
/// </summary>
/// <param name="cleardirty">Should the object be marked as clean once written?</param>
/// <returns>Returns with S_OK if the tag type was saved, otherwise the failure code.</returns>
// (0x006B4E80)

HRESULT __stdcall TagTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = AbstractClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// int TagTypeClass::Kind_Of() const (0x006B4EA0)

int TagTypeClass::Kind_Of() const
{
    return 45;
}


// int TagTypeClass::Size_Of() const (0x006B4EB0)

int TagTypeClass::Size_Of() const
{
    return sizeof(TagTypeClass);
}


// int TagTypeClass::Fetch_Heap_ID() const (0x006B4EC0)

int TagTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// TagTypeClass scalar deleting destructor (0x006B4ED0): compiler generated; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TagTypeClass *>::Clear (0x006B4EF0; DynamicVectorClass<TagTypeClass *> vtable 0x007ac778 slot 3).
// DynamicVectorClass<TagTypeClass *>::ID_pointer (0x006B4F20; DynamicVectorClass<TagTypeClass *> vtable 0x007ac778 slot 5).
// VectorClass<TagTypeClass *>::Fetch (0x006B4F40; VectorClass<TagTypeClass *> vtable 0x007ac798 slot 6).
// VectorClass<TagTypeClass *>::operator equal (0x006B4F50; VectorClass<TagTypeClass *> vtable 0x007ac798 slot 1).
// VectorClass<TagTypeClass *>::Resize (0x006B4F90; VectorClass<TagTypeClass *> vtable 0x007ac798 slot 2).
// VectorClass<TagTypeClass *>::Clear (0x006B5040; VectorClass<TagTypeClass *> vtable 0x007ac798 slot 3).
// VectorClass<TagTypeClass *>::ID_object (0x006B5070; VectorClass<TagTypeClass *> vtable 0x007ac798 slot 4).
// VectorClass<TagTypeClass *>::ID_pointer (0x006B50B0; VectorClass<TagTypeClass *> vtable 0x007ac798 slot 5).
// DynamicVectorClass<TagTypeClass *>::Resize (0x006B50D0; DynamicVectorClass<TagTypeClass *> vtable 0x007ac778 slot 2).
// DynamicVectorClass<TagTypeClass *>::ID_object (0x006B5180; DynamicVectorClass<TagTypeClass *> vtable 0x007ac778 slot 4).


extern "C" unsigned char vtable_6b51b0[]; extern "C" void __cdecl release_buffer_6b51b0(void*); extern "C" void __cdecl release_object_6b51b0(void*);
class Leaf_6b51b0 { public: void* get(unsigned int flags); };
void* Leaf_6b51b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6b51b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6b51b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6b51b0(this); return this; }


extern "C" unsigned char vtable_6b5200[]; extern "C" void __cdecl release_buffer_6b5200(void*); extern "C" void __cdecl release_object_6b5200(void*);
class Leaf_6b5200 { public: void* get(unsigned int flags); };
void* Leaf_6b5200::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6b5200; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6b5200((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6b5200(this); return this; }
