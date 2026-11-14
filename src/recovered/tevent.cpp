// tevent.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tevent.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x006E5B70 and the
// unit's TEventClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/object.hpp"

// The unit's list of TEventClass objects (0x00AC0808; name is a hypothesis).
DynamicVectorClass<TEventClass *> unknown_TEventClass_list_00ac0808;


// Dynamic initializer (0x006E5BA0, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5BC0, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5BE0, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5C00, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5C20, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5C40, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5C80, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5CB0, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5CE0, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5D10, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5D20, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5D40, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5D60, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// Dynamic initializer (0x006E5DA0, _$E) emitted by HeaderStatics_diagonal_init_006e5b70.cpp; no source of its own.


// TEventClass::TEventClass() (0x006E5DE0)
// A new event, numbered by its place in TEvents.

// The abstract list 0x00AC0C98 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0c98;

TEventClass::TEventClass() :
    unknown_24(-1),
    Next(0),
    Type(0),
    unknown_30(0)
{
    unknown_TEventClass_list_00ac0808.Add(this);
    unknown_24 = unknown_TEventClass_list_00ac0808.ID(this);
    unknown_AbstractClass_list_00ac0c98.Add(this);
    unknown_objects_ac0cb0.Add((ObjectClass *)this);
    Data = 0;
}


// TEventClass::~TEventClass() (0x006E5F40)

// The abstract list 0x00AC0C98 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0c98;

TEventClass::~TEventClass()
{
    Unknown_006ECB90(this, true);
    unknown_AbstractClass_list_00ac0c98.Delete(this);
    unknown_objects_ac0cb0.Delete((ObjectClass *)this);
    unknown_TEventClass_list_00ac0808.Delete(this);
}


// bool TEventClass::unknown_func_6e6050(int event, HouseClass *house, ObjectClass *object,
//     CDTimerClass<FrameTimerClass> *timer, bool *tripped, int source) (0x006E6050)
// TS's TEventClass::operator(): whether this event is satisfied for event (time,
// global/local variable and mission-timer events first, then the events the caller
// reports, then the house and owner tests). Is_Time_Based 0x006E6CF0 inlines here.
#include "shared/object_type.hpp"
#include "shared/house.hpp"
#include "shared/team.hpp"
#include "shared/techno.hpp"
#include "shared/building_type_class.hpp"
#include "shared/id_source.hpp"
#include "shared/coord_distance.hpp"

bool TEventClass::unknown_func_6e6050(int event, HouseClass *house, ObjectClass *object,
                                      CDTimerClass<FrameTimerClass> *timer, bool *tripped, int source)
{
    switch (Type) {
    case 0x2D:
        return unknown_id_source_a3d290->unknown_22ac <= Data;
    case 0x2E:
        return unknown_id_source_a3d290->unknown_22ac >= Data;
    case 0x2F:
        if (Data > Frame / 15) {
            return false;
        } else {
            return true;
        }
    case 0x1B: {
        bool value;
        unknown_id_source_a3d290->Get_Global_Variable(Data, &value);
        return value;
    }
    case 0x1C: {
        bool value;
        unknown_id_source_a3d290->Get_Global_Variable(Data, &value);
        return !value;
    }
    case 0x24: {
        bool value;
        unknown_id_source_a3d290->Get_Local_Variable(Data, &value);
        return value;
    }
    case 0x25: {
        bool value;
        unknown_id_source_a3d290->Get_Local_Variable(Data, &value);
        return !value;
    }
    case 0x0E:
        if (!unknown_id_source_a3d290->MissionTimer.Is_Active() || unknown_id_source_a3d290->MissionTimer != 0) return false;
        return true;
    case 0x0D:
    case 0x33:
        if (*timer != 0) return false;
        return true;
    }

    if (Type == 0) {
        return false;
    }

    if (Type != 0x2C &&
        Type != 1 &&
        Type != 0x19 &&
        Type != 0x1A &&
        Type != 0x22 &&
        Type != 0x18 &&
        Type != 0x13 &&
        Type != 0x14 &&
        Type != 0x15 &&
        Type != 0x16 &&
        Type != 0x35 &&
        Type != 0x36 &&
        unknown_func_6e6cf0()) {
        if (event != Type || Debug_Map) {
            return false;
        }
    }

    if (Type == 1 || Type == 0x19 || Type == 0x1A || Type == 0x18) {
        if (event != Type) return false;
        if (Data != -1 && HouseClass::As_Pointer(Data) == 0) return false;
        if (!object || (Data != -1 && object->Owner() != HouseClass::As_Pointer(Data)->ArrayIndex)) return false;
        *tripped = true;
        return true;
    } else if (Type == 0x35) {
        if (event != Type) return false;
        if (!object || Data == -1 || HouseClass::As_Pointer(Data) == 0) return false;
        if (!object->unknown_slot51(true)) return false;
        if (object->unknown_slot51(true)->ArrayIndex != HouseClass::As_Pointer(Data)->ArrayIndex) return false;
        *tripped = true;
        return true;
    } else if (Type == 0x36) {
        if (event != Type) return false;
        if (!object) return false;
        ObjectTypeClass *type = object->unknown_slot50(true);
        if (type->Kind_Of() != 0x10 || Data == -1 || type->Fetch_Heap_ID() != Data) return false;
        *tripped = true;
        return true;
    } else if (Type == 0x22) {
        if (event != Type) return false;
        CoordStruct waypoint = unknown_id_source_a3d290->Waypoint_Coord(Data);
        if (Coord_Distance(object->Center_Coord(), waypoint) > 0x500) {
            return false;
        }
        return true;
    } else if (Type == 0x2C) {
        if (event != Type) return false;
        // source is the attacking TechnoClass (TS: TechnoClass *source; the int parameter is the old hypothesis).
        if (source == 0 || Data != ((TechnoClass *)source)->House->ArrayIndex) {
            return false;
        }
    }

    int index;
    if (house != 0) {
        switch (Type) {
        case 0x17:
            for (index = 0; index < Teams.Count(); index++) {
                TeamClass *ptr = Teams[index];
                if (ptr->Class == (TeamTypeClass *)unknown_30 && ptr->Member == 0 && ptr->TargetNotAssigned) {
                    *tripped = true;
                    break;
                }
            }
            if (index == Teams.Count()) return false;
            break;
        case 0x0C:
            if (house->Available_Money() < Data) return false;
            break;
        case 0x34:
            if (house->Available_Money() > Data) return false;
            break;
        case 0x11:
            if (house->CurBuildings > 0) {
                for (int index = 0; index < house->Buildings.Count(); index++) {
                    BuildingClass *ptr = house->Buildings[index];
                    if (ptr != 0 && !ptr->unknown_76 && ptr->Type->Factory != 0) {
                        return false;
                    }
                }
            }
            break;
        case 0x12:
            if (!house->unknown_141) return false;
            break;
        case 0x20:
            if (house->unknown_5460.unknown_func_491d10(Data) == 0) return false;
            *tripped = true;
            break;
        case 0x39:
            if (house->unknown_5460.unknown_func_491d10(Data) != 0) return false;
            *tripped = true;
            break;
        case 0x13:
            if (house->unknown_1b4 != Data) return false;
            *tripped = true;
            break;
        case 0x14:
            if (house->unknown_1bc != Data) return false;
            *tripped = true;
            break;
        case 0x15:
            if (house->unknown_1b8 != Data) return false;
            *tripped = true;
            break;
        case 0x16:
            if (house->unknown_1c0 != Data) return false;
            *tripped = true;
            break;
        case 0x0F:
            if (house->unknown_53b4 < Data) return false;
            break;
        case 0x10:
            if (house->unknown_5360 < Data) return false;
            break;
        default:
            break;
        }
    }

    house = HouseClass::As_Pointer(Data);
    if (house != 0) {
        switch (Type) {
        case 0x1E:
            if (house->Power_Fraction() >= 1) return false;
            break;
        case 0x03:
            if (!house->unknown_18b) return false;
            break;
        case 0x05:
            if (!house->unknown_13c) return false;
            break;
        case 0x0A:
            if (house->CurBuildings > 0) return false;
            break;
        case 0x09:
            if (house->unknown_228 > 0 || house->unknown_234 > 0) return false;
            break;
        case 0x37:
            if (house->unknown_22c > 0) return false;
            break;
        case 0x38:
            if (house->unknown_228 - house->unknown_22c > 0) return false;
            if (house->unknown_234 > 0) return false;
            break;
        case 0x0B:
            if (house->CurBuildings > 0 || house->unknown_228 > 0 || house->unknown_234 > 0) return false;
            break;
        default:
            break;
        }
    }
    return true;
}


// void TEventClass::Build_INI_Entry(char *buffer) const (0x006E68B0)

void TEventClass::Build_INI_Entry(char *buffer) const
{
    AbstractTypeClass *type = unknown_30;
    int data = Data;
    if (type != 0) {
        wsprintfA(buffer + strlen(buffer), "%d,%d,%s", Type, 1, (char const *)type->ID);
    } else {
        wsprintfA(buffer + strlen(buffer), "%d,%d,%d", Type, 0, data);
    }
}


// void TEventClass::Read_INI() (0x006E6900)
// Reads "type,kind,value" from the current strtok position: kind 0 keeps
// the value as a number, kind 1 as a named type.

void TEventClass::Read_INI()
{
    Data = 0;
    Type = atoi(strtok(0, ","));
    int kind = atoi(strtok(0, ","));
    char *token = strtok(0, ",");
    int value = atoi(token);
    switch (kind) {
    case 0:
        Data = value;
        break;
    case 1:
        unknown_30 = Unknown_006BF260(token);
        break;
    }
}


// int Event_Needs(int type) (0x006E6970)
// What data an event type needs (TibSun tevent.cpp's Event_Needs: house,
// number, global, local, structure, unit, infantry, aircraft, team and
// waypoint, in that order; the returned codes are RA2's need values, names
// from the TS order and hypotheses). No callers in the image.
#include "shared/tevent.hpp"

int __fastcall Event_Needs(int type)
{
    switch (type) {
        case 1:
        case 3:
        case 5:
        case 9:
        case 10:
        case 11:
        case 24:
        case 25:
        case 26:
        case 30:
        case 44:
        case 53:
        case 55:
        case 56:
            return 13;   // house

        case 12:
        case 13:
        case 15:
        case 16:
        case 45:
        case 46:
        case 47:
        case 51:
        case 52:
            return 10;   // number

        case 27:
        case 28:
            return 22;   // global

        case 36:
        case 37:
            return 29;   // local

        case 19:
        case 32:
        case 57:
            return 8;   // structure

        case 20:
            return 6;   // unit

        case 21:
        case 54:
            return 5;   // infantry

        case 22:
            return 7;   // aircraft

        case 23:
            return 12;   // team

        case 34:
            return 9;   // waypoint

        default:
            break;
    }
    return 0;
}


// static int __fastcall TEventClass::Event_Flags(int type) (0x006E6A30)
// What an event type attaches to, as bits (1, 2, 4, 8, 16; meanings are
// hypotheses: TriggerTypeClass 0x006EE410 merges them over a trigger's events).

int __fastcall TEventClass::Event_Flags(int type)
{
    int flags = 0;
    switch (type) {
        case 0:
        case 1:
        case 4:
        case 8:
        case 24:
        case 25:
        case 26:
        case 31:
        case 53:
        case 54:
            flags |= 1;
            break;
    }
    switch (type) {
        case 0:
        case 1:
        case 2:
        case 4:
        case 6:
        case 7:
        case 8:
        case 29:
        case 33:
        case 34:
        case 35:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 48:
        case 49:
            flags |= 2;
            break;
    }
    if (type == 8 || type == 24) {
        flags |= 4;
    }
    switch (type) {
        case 3:
        case 5:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 30:
        case 32:
        case 52:
        case 55:
        case 56:
        case 57:
            flags |= 8;
            break;
    }
    switch (type) {
        case 8:
        case 13:
        case 14:
        case 23:
        case 27:
        case 28:
        case 36:
        case 37:
        case 45:
        case 46:
        case 47:
        case 50:
        case 51:
            flags |= 16;
            break;
    }
    return flags;
}


// void TEventClass::Detach(AbstractClass *target, bool all) (0x006E6BA0)

void TEventClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Next == target) {
        Next = Next->Next;
    }
    if ((AbstractClass *)unknown_30 == target) {
        unknown_30 = 0;
    }
}


// void TEventClass::Compute_CRC(WWCRCEngine &crc) const (0x006E6BC0)

void TEventClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    if (Next != 0) {
        crc(Next->Fetch_ID());
    }
    crc((long)Type);
    if (unknown_30 != 0) {
        crc(unknown_30->Fetch_ID());
    }
    crc((long)Data);
}


// HRESULT __stdcall TEventClass::GetClassID(GUID *clsid) (0x006E6C20)

HRESULT __stdcall TEventClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TEventClass;
    return S_OK;
}


// HRESULT __stdcall TEventClass::Load(IStream *stream) (0x006E6C60)

HRESULT __stdcall TEventClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TEventClass(NoInitClass());
    Swizzler.Swizzle((void **)&Next);
    Swizzler.Swizzle((void **)&unknown_30);
    return S_OK;
}


// HRESULT __stdcall TEventClass::Save(IStream *stream, BOOL clear_dirty) (0x006E6CD0)

HRESULT __stdcall TEventClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// bool TEventClass::unknown_func_6e6cf0() const (0x006E6CF0)
// TS's TEventClass::Is_Time_Based: the events that fire only when the caller
// reports them (TriggerClass::Should_Spring 0x006ED740 marks them).

bool TEventClass::unknown_func_6e6cf0() const
{
    switch (Type) {
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x06:
    case 0x07:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1D:
    case 0x1F:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:
    case 0x2C:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x35:
    case 0x36:
        return true;
    }
    return false;
}


// bool TEventClass::unknown_func_6e6d50() const (0x006E6D50)

bool TEventClass::unknown_func_6e6d50() const
{
    switch (Type) {
    case 1:
    case 6:
    case 0x2C:
        return false;
    }
    return true;
}


// int TEventClass::Size_Of() const (0x006E6D70)

int TEventClass::Size_Of() const
{
    return sizeof(TEventClass);
}


// int TEventClass::Kind_Of() const (0x006E6D80)

int TEventClass::Kind_Of() const
{
    return 48;
}


// int TEventClass::Fetch_Heap_ID() const (0x006E6D90)

int TEventClass::Fetch_Heap_ID() const
{
    return unknown_24;
}


// TEventClass scalar deleting destructor (??_G) (0x006E6DA0)
// Compiler generated with the TEventClass vtable; the destructor is inlined into it.
// No source of its own: emitted by this unit (population-2526).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TEventClass *>::Clear (0x006E6EC0; DynamicVectorClass<TEventClass *> vtable 0x007ad694 slot 3).
// DynamicVectorClass<TEventClass *>::ID_pointer (0x006E6EF0; DynamicVectorClass<TEventClass *> vtable 0x007ad694 slot 5).
// VectorClass<TEventClass *>::Fetch (0x006E6F10; VectorClass<TEventClass *> vtable 0x007ad6b4 slot 6).
// VectorClass<TEventClass *>::operator== (0x006E6F20).
// VectorClass<TEventClass *>::Resize (0x006E6F60).
// VectorClass<TEventClass *>::Clear (0x006E7010; VectorClass<TEventClass *> vtable 0x007ad6b4 slot 3).
// VectorClass<TEventClass *>::ID_object (0x006E7040; VectorClass<TEventClass *> vtable 0x007ad6b4 slot 4).
// VectorClass<TEventClass *>::ID_pointer (0x006E7080; VectorClass<TEventClass *> vtable 0x007ad6b4 slot 5).
// DynamicVectorClass<TEventClass *>::Resize (0x006E70A0; DynamicVectorClass<TEventClass *> vtable 0x007ad694 slot 2).
// DynamicVectorClass<TEventClass *>::ID_object (0x006E7150; DynamicVectorClass<TEventClass *> vtable 0x007ad694 slot 4).


extern "C" unsigned char vtable_6e7180[]; extern "C" void __cdecl release_buffer_6e7180(void*); extern "C" void __cdecl release_object_6e7180(void*);
class Leaf_6e7180 { public: void* get(unsigned int flags); };
void* Leaf_6e7180::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6e7180; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6e7180((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6e7180(this); return this; }


extern "C" unsigned char vtable_6e71d0[]; extern "C" void __cdecl release_buffer_6e71d0(void*); extern "C" void __cdecl release_object_6e71d0(void*);
class Leaf_6e71d0 { public: void* get(unsigned int flags); };
void* Leaf_6e71d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6e71d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6e71d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6e71d0(this); return this; }
