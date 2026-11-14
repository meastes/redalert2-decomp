// tag.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tag.json carries the addresses and the rows that
// stay in their own files.


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

// The unit's list of TagClass objects (0x00ABFE48; name is a hypothesis).
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


// TagClass::TagClass(TagTypeClass *type) (0x006B3870)
// A live tag with one trigger per trigger type of its type.

// The abstract list 0x00AC0D48 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0d48;

TagClass::TagClass(TagTypeClass *type) :
    Class(type),
    Trigger(0),
    AttachCount(0),
    CellID(unknown_geometry_k),
    IsToDie(false),
    IsCurrentlySprung(false)
{
    Tags.Add(this);
    unknown_objects_ac0cb0.Add((ObjectClass *)this);
    unknown_AbstractClass_list_00ac0d48.Add(this);
    if (type != 0) {
        for (TriggerTypeClass *trigger = type->FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
            TriggerClass *made = new TriggerClass(trigger);
            made->LinkedTo = Trigger;
            Trigger = made;
        }
    }
}


// TagClass::~TagClass() (0x006B39F0)
// Drops the tag from the scenario's tag lists and its house's counters,
// queues its triggers for removal and leaves the abstract lists.
#include "shared/house.hpp"

// The abstract list 0x00AC0D48 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0d48;
// Two tag lists and their positions (0x00866160 / 0x00A35D44 for types
// with flag 0x10, 0x00866238 / 0x00A35E24 for flag 4; hypotheses).
extern DynamicVectorClass<TagClass *> unknown_tags_866160;
extern int unknown_tag_position_a35d44;
extern DynamicVectorClass<TagClass *> unknown_tags_866238;
extern int unknown_tag_position_a35e24;
extern bool unknown_flag_a40958;
extern bool unknown_flag_a40d04;

TagClass::~TagClass()
{
    Unknown_006ECB90(this, true);
    if (unknown_flag_a40958 && unknown_flag_a40d04 && Class != 0 && (Class->Attaches_To() & 0x10)) {
        int index = unknown_tags_866160.ID(this);
        if (unknown_tag_position_a35d44 >= index) {
            if (--unknown_tag_position_a35d44 < 0 && unknown_tags_866160.Count() == 0) {
                unknown_tag_position_a35d44 = 0;
            }
        }
    }
    if (unknown_flag_a40958 && unknown_flag_a40d04 && Class != 0 && (Class->Attaches_To() & 4)) {
        int index = unknown_tags_866238.ID(this);
        if (unknown_tag_position_a35e24 >= index) {
            if (--unknown_tag_position_a35e24 < 0 && unknown_tags_866238.Count() == 0) {
                unknown_tag_position_a35e24 = 0;
            }
        }
    }
    if (unknown_flag_a40958 && unknown_flag_a40d04 && Trigger != 0 && Trigger->Class != 0 &&
        Trigger->Class->House != 0 && Class->Is_Allow_Win()) {
        HouseClass *house = HouseClass::As_Pointer(((HouseTypeClass *)Trigger->Class->House)->House);
        if (house->unknown_1c4 != 0) {
            house->unknown_1c4--;
        }
        house->unknown_1e0 = 60;
    }
    for (TriggerClass *trigger = Trigger; trigger != 0; trigger = trigger->LinkedTo) {
        unknown_objects_ac0cd8.Add(trigger);
    }
    unknown_objects_ac0cb0.Delete((ObjectClass *)this);
    unknown_AbstractClass_list_00ac0d48.Delete(this);
    Tags.Delete(this);
    Unknown_006ECB90(this, true);
}


// void TagClass::Mark_To_Delete() (0x006B3CC0)
// Marks the tag and queues it in the abstract list 0x00AC0CD8.

void TagClass::Mark_To_Delete()
{
    IsToDie = true;
    unknown_objects_ac0cd8.Add(this);
}


// bool TagClass::Is_Marked_To_Delete() const (0x006B3D20)

bool TagClass::Is_Marked_To_Delete() const
{
    return IsToDie;
}


// TagClass *__fastcall TagClass::Find_Or_Make(TagTypeClass *type) (0x006B3D30)
// The live tag of a type, created when missing.

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


// Cell TagClass::Get_Cell() const (0x006B3D80)

Cell TagClass::Get_Cell() const
{
    return CellID;
}


// bool TagClass::Is_Cross_Vertical() const (0x006B3D90)

bool TagClass::Is_Cross_Vertical() const
{
    return Class->Is_Cross_Vertical();
}


// bool TagClass::Is_Cross_Horizontal() const (0x006B3DA0)

bool TagClass::Is_Cross_Horizontal() const
{
    return Class->Is_Cross_Horizontal();
}


// bool TagClass::Is_Enters_Zone() const (0x006B3DB0)

bool TagClass::Is_Enters_Zone() const
{
    return Class->Is_Enters_Zone();
}


// bool TagClass::Is_Allow_Win() const (0x006B3DC0)

bool TagClass::Is_Allow_Win() const
{
    return Class->Is_Allow_Win();
}


// bool TagClass::Is_Trigger_Attached(TriggerClass *trigger) const (0x006B3DD0)

bool TagClass::Is_Trigger_Attached(TriggerClass *trigger) const
{
    for (TriggerClass *attached = Trigger; attached != 0; attached = attached->LinkedTo) {
        if (attached == trigger) {
            return true;
        }
    }
    return false;
}


// bool TagClass::Spring(int event, ObjectClass *object, Cell cell, bool forced, int source) (0x006B3DF0)
// Offers the event to each attached trigger. A sprung trigger acts on object and cell; by the tag
// type's persistence (0 volatile, 1 semi-persistent, 2 persistent) the trigger and the tag are then
// queued for removal and the object and cell drop the tag. Returns whether any trigger acted.
#include "shared/cell.hpp"
#include "shared/map_view.hpp"

bool TagClass::Spring(int event, ObjectClass *object, Cell cell, bool forced, int source)
{
    bool sprung = false;
    bool remove = false;
    bool detach = false;
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
                sprung = true;
                remove = true;
                detach = true;
                break;
            case 1:
                if (AttachCount == 1) {
                    trigger->Spring(object, cell);
                    trigger->Mark_To_Delete();
                    remove = true;
                    sprung = true;
                } else {
                    detach = true;
                }
                break;
            case 2:
                trigger->Spring(object, cell);
                sprung = true;
                break;
            }
        }
    }
    IsCurrentlySprung = false;
    if (detach) {
        if (object != 0 && object->unknown_34 == this) {
            object->Attach_Tag(0);
        }
        if (cell != unknown_geometry_k) {
            Map[cell].Attach_Tag(0);
        }
    }
    if (remove) {
        Unknown_006ECB90(this, true);
        unknown_objects_ac0cd8.Add(this);
    }
    return sprung;
}


// void TagClass::Set_Cell(Cell cell) (0x006B3F90)

void TagClass::Set_Cell(Cell cell)
{
    CellID = cell;
}


// void TagClass::Clear_All() (0x006B3FA0)
// Deletes every tag (each destructor removes it from Tags).

void TagClass::Clear_All()
{
    while (Tags.Count() > 0) {
        delete Tags[0];
    }
}


// void TagClass::Timer_Global_Reset(int value) (0x006B3FD0)

void TagClass::Timer_Global_Reset(int value)
{
    Trigger->Reset_Global_Linked_Timed_Events(value);
}


// void TagClass::Timer_Local_Reset(int value) (0x006B3FE0)
// Passes value to the attached chain's 0x006ED650 reset (the companion of 0x006B3FD0).

void TagClass::Timer_Local_Reset(int value)
{
    Trigger->Reset_Local_Linked_Timed_Events(value);
}


// void TagClass::Link(TriggerClass *trigger) (0x006B3FF0): puts the trigger at the head of the chain
// (TibSun Link).

void TagClass::Link(TriggerClass *trigger)
{
    trigger->LinkedTo = Trigger;
    Trigger = trigger;
}


// bool TagClass::Unlink(TriggerClass *trigger) (0x006B4000): takes the trigger out of the chain;
// false when it was not attached (TibSun Unlink).

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


// void TagClass::Detach(AbstractClass *target, bool all) (0x006B4040)
// A tag that loses its type queues itself for removal.

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


// HRESULT __stdcall TagClass::GetClassID(GUID *clsid) (0x006B40D0)
#include "shared/tag.hpp"

HRESULT __stdcall TagClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TagClass;
    return S_OK;
}


// void TagClass::Compute_CRC(WWCRCEngine &crc) const (0x006B4110)

void TagClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(Class->Fetch_ID());
    if (Trigger != 0) {
        crc(Trigger->Fetch_ID());
    }
    crc(IsToDie);
}


// HRESULT __stdcall TagClass::Load(IStream *stream) (0x006B4160)

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


// HRESULT __stdcall TagClass::Save(IStream *stream, BOOL clear_dirty) (0x006B41D0)

HRESULT __stdcall TagClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// bool TagClass::Is_To_Inherit() const (0x006B41F0)
// Whether any attached trigger's type has +0xA0 set.

bool TagClass::Is_To_Inherit() const
{
    for (TriggerClass *trigger = Trigger; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Class->IsToInherit) {
            return true;
        }
    }
    return false;
}


// void __fastcall TagClass::All_Timer_Global_Reset(int value) (0x006B4220)
// Passes value to the first trigger of every tag.

void __fastcall TagClass::All_Timer_Global_Reset(int value)
{
    for (int index = 0; index < Tags.Count(); index++) {
        Tags[index]->Trigger->Reset_Global_Linked_Timed_Events(value);
    }
}


// void __fastcall TagClass::All_Timer_Local_Reset(int value) (0x006B4250)
// Passes value to the first trigger of every tag.

void __fastcall TagClass::All_Timer_Local_Reset(int value)
{
    for (int index = 0; index < Tags.Count(); index++) {
        Tags[index]->Trigger->Reset_Local_Linked_Timed_Events(value);
    }
}


// bool TagClass::Is_One_Of_A_Kind() const (0x006B4280)
// True when no other tag has the same type.

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
