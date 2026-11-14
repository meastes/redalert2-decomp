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


// TagTypeClass::TagTypeClass(char const *ini_name) (0x006B4570)
// A volatile tag with no triggers, named after its ID; registered in the tag
// type list (its index) and the abstract list.

TagTypeClass::TagTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    ArrayIndex(-1),
    Persistence(0),
    FirstTrigger(0)
{
    Name = FixedString<49>(ID);
    ArrayIndex = TagTypes.Count();
    TagTypes.Add(this);
    unknown_objects_ac0cb0.Add(this);
}


// TagTypeClass::~TagTypeClass() (0x006B46B0)
// Deletes the type's trigger types and every tag of this type.

TagTypeClass::~TagTypeClass()
{
    Unknown_006ECB90(this, true);
    TriggerTypeClass *trigger = FirstTrigger;
    FirstTrigger = 0;
    while (trigger != 0) {
        TriggerTypeClass *next = trigger->LinkedTo;
        trigger->LinkedTo = 0;
        delete trigger;
        trigger = next;
    }
    for (int index = 0; index < Tags.Count(); index++) {
        if (Tags[index]->Class == this) {
            delete Tags[index];
            index--;
        }
    }
    unknown_objects_ac0cb0.Delete((ObjectClass *)this);
    TagTypes.Delete(this);
}


// bool TagTypeClass::Link(TriggerTypeClass *trigger) (0x006B47E0)

bool TagTypeClass::Link(TriggerTypeClass *trigger)
{
    if (trigger != 0) {
        trigger->LinkedTo = FirstTrigger;
        FirstTrigger = trigger;
        return true;
    }
    return false;
}


// bool TagTypeClass::Unlink(TriggerTypeClass *trigger) (0x006B4810)

bool TagTypeClass::Unlink(TriggerTypeClass *trigger)
{
    TriggerTypeClass *current = FirstTrigger;
    if (current == trigger) {
        FirstTrigger = trigger->LinkedTo;
        return true;
    }
    while (current != 0) {
        if (current->LinkedTo == trigger) {
            current->LinkedTo = trigger->LinkedTo;
            return true;
        }
        current = current->LinkedTo;
    }
    return false;
}


// void TagTypeClass::Detach(AbstractClass *target, bool all) (0x006B4860)

void TagTypeClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)FirstTrigger == target) {
        FirstTrigger = FirstTrigger->LinkedTo;
    }
}


// TagTypeClass *__fastcall TagTypeClass::From_Name(char const *name) (0x006B4880)
// The tag type whose ID or name matches, or 0.

TagTypeClass *__fastcall TagTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TagTypes.Count(); index++) {
            TagTypeClass *type = TagTypes[index];
            if (!_stricmp(type->ID, name) || !_stricmp(type->Name, name)) {
                return type;
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
    int count = ini.Entry_Count(unknown_tags_section_7ac770);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_tags_section_7ac770, index);
        char buffer[24];
        if (ini.Get_String(unknown_tags_section_7ac770, entry, unknown_string_83c55c, buffer, sizeof(buffer)) > 0) {
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
    int count = ini.Entry_Count(unknown_tags_section_7ac770);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        char const *entry = ini.Get_Entry(unknown_tags_section_7ac770, index);
        ini.Get_String(unknown_tags_section_7ac770, entry, unknown_string_83c55c, buffer, sizeof(buffer));
        ini.Clear(buffer, 0);
    }
    ini.Clear(unknown_tags_section_7ac770, 0);
    for (int i = 0; i < TagTypes.Count(); i++) {
        TagTypes[i]->Write_INI(ini);
    }
}


// bool TagTypeClass::Read_INI(CCINIClass &ini) (0x006B4A90)
// The [Tags] entry "persistence,name,trigger": the persistence, the
// display name and the first trigger type (made when missing).
#include "shared/trigger_type.hpp"

extern "C" char *__cdecl strtok(char *string, char const *delimiters);

bool TagTypeClass::Read_INI(CCINIClass &ini)
{
    char buffer[128];
    if (ini.Get_String(unknown_tags_section_7ac770, ID, unknown_string_83c55c, buffer, sizeof(buffer))) {
        strtok(buffer, ",");
        Persistence = atoi(buffer);
        Name = FixedString<49>(strtok(0, ","));
        FirstTrigger = TriggerTypeClass::Find_Or_Make(strtok(0, ","));
        return true;
    }
    return false;
}


// bool TagTypeClass::Write_INI(CCINIClass &ini) const (0x006B4B70)

bool TagTypeClass::Write_INI(CCINIClass &ini) const
{
    char buffer[128];
    if (FirstTrigger == 0) {
        wsprintfA(buffer, "%s,<none>", (char const *)Name);
        ini.Put_String(unknown_tags_section_7ac770, ID, buffer);
    } else {
        wsprintfA(buffer, "%d,%s,%s", Persistence, (char const *)Name, (char const *)FirstTrigger->ID);
        ini.Put_String(unknown_tags_section_7ac770, ID, buffer);
    }
    return true;
}


// int TagTypeClass::Attaches_To() const (0x006B4C00)
// The combined flags of the type's trigger types.

int TagTypeClass::Attaches_To() const
{
    int flags = 0;
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
        flags |= trigger->Attaches_To();
    }
    return flags;
}


// bool TagTypeClass::Is_Allow_Win() const (0x006B4C30)
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


// bool TagTypeClass::Is_Cross_Horizontal() const (0x006B4C60)
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


// bool TagTypeClass::Is_Cross_Vertical() const (0x006B4C90)
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


// bool TagTypeClass::Is_Enters_Zone() const (0x006B4CC0)
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


// bool TagTypeClass::Is_Linked(TriggerTypeClass *trigger) const (0x006B4CF0)

bool TagTypeClass::Is_Linked(TriggerTypeClass *trigger) const
{
    for (TriggerTypeClass *attached = FirstTrigger; attached != 0; attached = attached->LinkedTo) {
        if (attached == trigger) {
            return true;
        }
    }
    return false;
}


// TagTypeClass *__fastcall TagTypeClass::Find_Or_Make(char const *name) (0x006B4D20)
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


// HRESULT __stdcall TagTypeClass::GetClassID(GUID *clsid) (0x006B4DB0)

HRESULT __stdcall TagTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TagTypeClass;
    return S_OK;
}


// void TagTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x006B4DF0)

void TagTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)Persistence);
}


// HRESULT __stdcall TagTypeClass::Load(IStream *stream) (0x006B4E20)

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


// HRESULT __stdcall TagTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x006B4E80)

HRESULT __stdcall TagTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
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
