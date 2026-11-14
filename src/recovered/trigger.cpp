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


// TriggerClass::TriggerClass(TriggerTypeClass *type) (0x006ED220)
// A live trigger; disabled when its type is, or for the current difficulty.

// The abstract list 0x00AC0D48 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0d48;

TriggerClass::TriggerClass(TriggerTypeClass *type) :
    Class(type),
    LinkedTo(0),
    IsToDelete(false),
    IsTripped(0),
    IsActive(true)
{
    unknown_objects_ac0cb0.Add((ObjectClass *)this);
    unknown_AbstractClass_list_00ac0d48.Add(this);
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


// bool TriggerClass::Is_Cross_Horizontal() const (0x006ED4D0)
// TriggerTypeClass::Is_Cross_Horizontal of this trigger's type or of any later one.

bool TriggerClass::Is_Cross_Horizontal() const
{
    if (LinkedTo != 0) {
        return Class->Is_Cross_Horizontal() || LinkedTo->Is_Cross_Horizontal();
    }
    return Class->Is_Cross_Horizontal();
}


// bool TriggerClass::Is_Cross_Vertical() const (0x006ED510)
// TriggerTypeClass::Is_Cross_Vertical of this trigger's type or of any later one.

bool TriggerClass::Is_Cross_Vertical() const
{
    if (LinkedTo != 0) {
        return Class->Is_Cross_Vertical() || LinkedTo->Is_Cross_Vertical();
    }
    return Class->Is_Cross_Vertical();
}


// bool TriggerClass::Is_Enters_Zone() const (0x006ED550)
// TriggerTypeClass::Is_Enters_Zone of this trigger's type or the next one's.

bool TriggerClass::Is_Enters_Zone() const
{
    if (LinkedTo != 0) {
        return Class->Is_Enters_Zone() || LinkedTo->Class->Is_Enters_Zone();
    }
    return Class->Is_Enters_Zone();
}


// bool TriggerClass::Is_Allow_Win() const (0x006ED590)
// TriggerTypeClass::Is_Allow_Win of this trigger's type or the next one's.

bool TriggerClass::Is_Allow_Win() const
{
    if (LinkedTo != 0) {
        return Class->Is_Allow_Win() || LinkedTo->Class->Is_Allow_Win();
    }
    return Class->Is_Allow_Win();
}


// bool TriggerClass::Is_Linked_To_Global(int value) const (0x006ED5D0)

bool TriggerClass::Is_Linked_To_Global(int value) const
{
    if (LinkedTo != 0) {
        return Class->Is_Linked_To_Global(value) || LinkedTo->Class->Is_Linked_To_Global(value);
    }
    return Class->Is_Linked_To_Global(value);
}


// void TriggerClass::Reset_Global_Linked_Timed_Events(int value) (0x006ED620)
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


// void TriggerClass::Reset_Local_Linked_Timed_Events(int value) (0x006ED650)
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


// void TriggerClass::Reset_All_Timed_Events() (0x006ED680)
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


// bool TriggerClass::Should_Spring(int event, ObjectClass *object, bool forced, bool persistent, int source) (0x006ED740)
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


// bool TriggerClass::Spring(ObjectClass *object, Cell cell) (0x006ED830)
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


// TriggerClass *__fastcall TriggerClass::Find_Or_Make(TriggerTypeClass *type) (0x006ED8A0)
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


// void TriggerClass::Detach(AbstractClass *target, bool all) (0x006ED900)

void TriggerClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Class == target) {
        Class = 0;
    }
    if ((AbstractClass *)LinkedTo == target) {
        LinkedTo = LinkedTo->LinkedTo;
    }
}


// void TriggerClass::Flag_Event_Tripped(int index) (0x006ED930)

void TriggerClass::Flag_Event_Tripped(int index)
{
    IsTripped |= 1 << index;
}


// void TriggerClass::Flag_Event_Untripped(int index) (0x006ED950)

void TriggerClass::Flag_Event_Untripped(int index)
{
    IsTripped &= ~(1 << index);
}


// bool TriggerClass::Is_Event_Tripped(int index) const (0x006ED970)

bool TriggerClass::Is_Event_Tripped(int index) const
{
    return (IsTripped & (1 << index)) != 0;
}


// void TriggerClass::Mark_To_Delete() (0x006ED990)
// Marks the trigger and queues it in the abstract list 0x00AC0CD8.

void TriggerClass::Mark_To_Delete()
{
    IsToDelete = true;
    unknown_objects_ac0cd8.Add(this);
}


// bool TriggerClass::Is_Marked_To_Delete() const (0x006ED9F0)

bool TriggerClass::Is_Marked_To_Delete() const
{
    return IsToDelete;
}


// void TriggerClass::Compute_CRC(WWCRCEngine &crc) const (0x006EDA00)

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


// HRESULT __stdcall TriggerClass::GetClassID(GUID *clsid) (0x006EDA90)

HRESULT __stdcall TriggerClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TriggerClass;
    return S_OK;
}


// HRESULT __stdcall TriggerClass::Load(IStream *stream) (0x006EDAD0)

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


// HRESULT __stdcall TriggerClass::Save(IStream *stream, BOOL clear_dirty) (0x006EDB40)

HRESULT __stdcall TriggerClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// void TriggerClass::Enable() (0x006EDB60)

void TriggerClass::Enable()
{
    IsActive = true;
    Reset_All_Timed_Events();
}


// void TriggerClass::Disable() (0x006EDB70)

void TriggerClass::Disable()
{
    IsActive = false;
}


// int TriggerClass::Kind_Of() const (0x006EDB90)

int TriggerClass::Kind_Of() const
{
    return 38;
}


// TriggerClass scalar deleting destructor (0x006EDBA0): compiler generated,
// with the destructor expanded inline. The destructor itself (0x006ED3C0) is
// defined here: its frozen inventory bound is split at 0x006ED49A, so it is
// bound by reference until the bound is refined.

// The abstract list 0x00AC0D48 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0d48;

TriggerClass::~TriggerClass()
{
    Unknown_006ECB90(this, true);
    Triggers.Delete(this);
    unknown_objects_ac0cb0.Delete((ObjectClass *)this);
    unknown_AbstractClass_list_00ac0d48.Delete(this);
}
