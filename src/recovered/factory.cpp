// factory.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/factory.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/factory_class.hpp"
#include "shared/object_lists.hpp"
#include "shared/house.hpp"
#include "shared/techno_type.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/debug_printf.hpp"
#include "shared/factory_class_decl.hpp"


// Header-static dynamic initializers of the unit at 0x004B8EB0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004B8EB0-0x004B9096); the other
// drafts of this plan are emitted by these includes.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


FactoryClass::FactoryClass() :
 Object(0),unknown_58(false),unknown_59(false),UnspentCost(0),unknown_60(0),unknown_64(-1),Owner(0),unknown_6c(false),unknown_6d(true)
{
    Create_ID();
    Factories.Add(this);
    Set_Rate(0);
    Stage = 0;
    unknown_AbstractClass_list_00ac0d60.Add(this);
}


extern bool unknown_flag_a40958;
FactoryClass::~FactoryClass()
{
    Unknown_006ECB90(this, true);
    Factories.Delete(this);
    if (unknown_flag_a40958) Abandon_Production_004B97C0();
    unknown_AbstractClass_list_00ac0d60.Delete(this);
}


// FactoryClass production frame, original282+6; primary RTTI virtualslot23.
inline bool Factory_Can_Advance_Production(FactoryClass const &factory)
{
    return !factory.unknown_6c && (factory.Object!=0 || factory.unknown_64!=0) && !factory.Is_Complete_004B9900();
}
void FactoryClass::AI()
{
    if(Factory_Can_Advance_Production(*this)) {
        if(StageClass::Advance_Stage_When_Ready()) {
            unknown_59=true;
            int amount=Cost_Per_Tick_004B9950();
            if(amount>=UnspentCost) amount=UnspentCost;
            if(amount>Owner->Available_Money()) {
                unknown_58=true;
                Stage=(int)((unsigned int)Stage-1U);
            } else {
                Owner->Unknown_004E5280(amount);
                unknown_58=false;
                UnspentCost=(int)((unsigned int)UnspentCost-(unsigned int)amount);
            }
            if(Stage==54) {
                unknown_6c=true;
                Set_Rate(0);
                Owner->Unknown_004E5280(UnspentCost);
                UnspentCost=0;
            }
        }
    }
}


bool FactoryClass::unknown_4b9440(TechnoTypeClass *type, HouseClass *house, unsigned char resume)
{
    if (type->Kind_Of()==7) Abandon_Production_004B97C0();
    if (type->Kind_Of()!=7 && ((Rate!=0 && !unknown_6c) || Queue.Count()>0 || (Object && unknown_6c)) && !resume) {
        if (Queue.Count()<Rules->MaximumQueuedObjects && !Owner->Is_Build_Limited_004F4F30(type)) {
            Queue.Add(type);
            return true;
        }
        if (house->Is_Player_Control()) Voc_Play(Rules->unknown_620,1.0f,0x2000,0);
        return false;
    }
    unknown_59=true;
    unknown_6c=true;
    Set_Rate(0);
    Stage=0;
    Object=static_cast<TechnoClass *>(type->Create_One_Of(house));
    if (!house->Is_Human_Player() && Object && Object->Kind_Of()==6)
        static_cast<BuildingClass *>(Object)->unknown_59a=true;
    if (Object) {
        Owner=Object->House;
        UnspentCost=type->Cost_Of(Owner);
        Object->unknown_258=UnspentCost;
    }
    return Object!=0;
}


// Original production-cancel operation; names of the factory flags and
// house construction indices remain unknown.
extern char const unknown_abandon_message_7d7f44[];

void FactoryClass::Set_Object_004B95E0(TechnoClass *object)
{
    Abandon_Production_004B97C0();
    Object = object;
    Owner = object->House;
    UnspentCost = 0;
    Set_Rate(0);
    Stage = 54;
    unknown_59 = true;
    unknown_6c = true;
}


bool FactoryClass::Suspend_004B9630(bool value)
{
    if (!unknown_6c) {
        unknown_6d = value;
        unknown_6c = true;
        Set_Rate(0);
        return true;
    }
    return false;
}


bool FactoryClass::Start_Production_004B9670(bool suspend)
{
 if((Object || unknown_64) && unknown_6c && (!Object || Stage!=54) && (unknown_64==-1 || Stage!=54)) {
  unknown_6c=false;
  Set_Rate(Production_Rate_004B9780());
  if(Owner->Available_Money() >= Cost_Per_Tick_004B9950()) {
   unknown_6d=true;
   if(suspend) Suspend_004B9630(true);
   return true;
  }
 }
 return false;
}


int FactoryClass::Production_Rate_004B9780() const
{
 int duration=0;
 if(Object) duration=Object->Unknown_006C22A0();
 int rate=duration/54;
 if(rate<1) rate=1;
 else if(rate>255) rate=255;
 return rate;
}


bool FactoryClass::Abandon_Production_004B97C0()
{
    if (Object) {
        Debug_Printf(unknown_abandon_message_7d7f44, (char const *)Object->Class_Of()->Name);
        Owner->Refund_Money(Object->Class_Of()->Cost_Of(Object->House) - UnspentCost);
        UnspentCost = 0;
        if (unknown_64) unknown_64 = -1;
        Set_Rate(0);
        Stage = 0;
        unknown_6c = true;
        unknown_59 = true;
        if (!Owner->Unknown_004F52F0()) {
            if (Object->Kind_Of() == 15) Owner->unknown_5544 = -1;
            if (Object->Kind_Of() == 1) Owner->unknown_5540 = -1;
            if (Object->Kind_Of() == 2) Owner->unknown_5548 = -1;
            if (Object->Kind_Of() == 6) Owner->unknown_553c = -1;
        }
        ++unknown_a40774;
        delete Object;
        Object = 0;
        --unknown_a40774;
        return true;
    }
    return false;
}


bool FactoryClass::Is_Complete_004B9900() const
{
    if (Object && Stage == 54) return true;
    if (unknown_64 != -1 && Stage == 54) return true;
    return false;
}


// Factory object getter, original004B9930; existing4body+12tail bytes.

TechnoClass *FactoryClass::Get_Object() const
{
    return Object;
}


int FactoryClass::Cost_Per_Tick_004B9950() const
{
    if (Object) {
        int remaining = 54 - Stage;
        if (remaining) return UnspentCost / remaining;
        return UnspentCost;
    }
    return 0;
}


bool FactoryClass::Release_Completed_004B9970()
{
    if (Object && Stage == 54) {
        Object = 0;
        unknown_6c = true;
        unknown_59 = true;
        Stage = 0;
        Set_Rate(0);
        return true;
    }
    if (unknown_64 && Stage == 54) {
        unknown_64 = -1;
        unknown_6c = true;
        unknown_59 = true;
        Stage = 0;
        Set_Rate(0);
        return true;
    }
    return false;
}


extern "C" GUID const CLSID_FactoryClass;
HRESULT __stdcall FactoryClass::GetClassID(GUID *id)
{
 if(!id) return 0x80004003;
 *id=CLSID_FactoryClass;
 return 0;
}


HRESULT __stdcall FactoryClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) FactoryClass(NoInitClass());
        int count = 0;
        result = stream->Read(&count, sizeof(count), 0);
        if (SUCCEEDED(result)) {
            for (int i = 0; i < count; ++i) {
                TechnoTypeClass *type = 0;
                result = stream->Read(&type, sizeof(type), 0);
                if (FAILED(result)) return result;
                Queue.Add(type);
            }
            for (i = 0; i < count; ++i) Swizzle_Pointer(Queue[i]);
            Swizzle_Pointer(Owner);
            Swizzle_Pointer(Object);
            return S_OK;
        }
    }
    return result;
}


// Original element-write failure returns S_OK; base/count-write errors propagate.
HRESULT __stdcall FactoryClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        int count = Queue.Count();
        result = stream->Write(&count, sizeof(count), 0);
        if (SUCCEEDED(result)) {
            for (int index = 0; index < count; ++index) {
                result = stream->Write(&Queue[index], sizeof(Queue[index]), 0);
                if (FAILED(result)) return S_OK;
            }
            return S_OK;
        }
    }
    return result;
}


extern "C" char const factory_message_007d7f64[];
extern "C" char const factory_message_007d7f70[];
extern "C" char const factory_message_007d7f88[];
extern "C" char const factory_message_007d7f9c[];
extern "C" char const factory_message_007d7fb4[];
extern "C" char const factory_message_007d7fc4[];
extern "C" char const factory_message_007d7fd8[];
extern "C" char const factory_message_007d7fec[];
extern "C" char const factory_message_007d8004[];
extern "C" char const factory_message_007d8018[];
extern "C" char const factory_message_007d8038[];
void FactoryClass::Compute_CRC(WWCRCEngine &crc) const
{
    Debug_Printf(factory_message_007d8038, Frame);
    Debug_Printf(factory_message_007d8018, Queue.Count());
    Debug_Printf(factory_message_007d8004, Object->Kind_Of());
    Debug_Printf(factory_message_007d7fec, Object->Fetch_Heap_ID());
    Debug_Printf(factory_message_007d7fd8, unknown_6c);
    Debug_Printf(factory_message_007d7fc4, unknown_59);
    Debug_Printf(factory_message_007d7fb4, UnspentCost);
    Debug_Printf(factory_message_007d7f9c, unknown_60);
    Debug_Printf(factory_message_007d7f88, unknown_64);
    Debug_Printf(factory_message_007d7f70, Owner->Fetch_ID());
    Debug_Printf(factory_message_007d7f64, const_cast<FactoryClass *>(this)->Fetch_ID());
    AbstractClass::Compute_CRC(crc);
    crc(unknown_6c);
    crc(unknown_59);
    crc((long)UnspentCost);
    crc((long)unknown_60);
    if (Object) crc(Object->Fetch_ID());
    crc((long)unknown_64);
    crc(Owner->Fetch_ID());
}


void FactoryClass::Detach(AbstractClass *target,bool all)
{
 if(target==Object) Object=0;
}


// Private typed experiment. Owner authorized fresh House call-interface copy;
// return and fourth-argument meanings remain explicit hypotheses.
// Full original body 114 bytes, then14 NOPs to independently used004B9DF0.
void FactoryClass::Start_Next_Queued_004B9D70()
{
    if (Queue.Count() && !Object && (!Rate || unknown_6c)) {
        TechnoTypeClass *type = Queue[0];
        Queue.Delete(0);
        int index = type->Fetch_Heap_ID();
        if (index >= 0) {
            Owner->Begin_Production(type->Kind_Of(), index, type->Naval, true);
        }
    }
}


bool FactoryClass::Delete_Queued_004B9DF0(TechnoTypeClass *type)
{
    return Queue.Delete(type);
}


int FactoryClass::Count_Type_004B9E40(TechnoTypeClass *type) const
{
    int total = 0;
    if (Object && Object->Techno_Type_Class() == type) total = 1;
    int count = Queue.Count();
    if (count > 0) {
        TechnoTypeClass *const *next = &Queue[0];
        do {
            if (*next == type) ++total;
            ++next;
        } while (--count);
    }
    return total;
}


bool FactoryClass::Has_Queued_004B9E80(TechnoTypeClass *type) const
{
    for (int index = 0; index < Queue.Count(); ++index) {
        if (type == Queue[index]) return true;
    }
    return false;
}


void __fastcall FactoryClass::Update_Rates(HouseClass *owner) {
 for (int i=0;i<Factories.Count();++i) {
  FactoryClass *factory=Factories[i];
  if(factory->Owner==owner) {
   int rate=0;
   if(factory->Object) rate=factory->Object->Unknown_006C22A0();
   rate/=54;
   if(rate<1) rate=1; else if(rate>255) rate=255;
   if(rate!=factory->Rate) factory->Rate=rate;
  }
 }
}


class Leaf_4b9420 { public: unsigned char get(); };
unsigned char Leaf_4b9420::get() { return *(unsigned char*)((char*)this + 0x58); }


// 0x004b9430: function not yet identified
//
// Body: 8 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined1 __fastcall FUN_004b9430(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004b9430 {

unsigned char __fastcall FUN_004b9430(int object_address)
{
    unsigned char previous_value;

    previous_value = (unsigned char)(*(unsigned char *)(object_address + 0x59));
    *(unsigned char *)(object_address + 0x59) = 0;
    return previous_value;
}

}  // namespace seed_004b9430


class Leaf_4b98f0 { public: unsigned int get(); };
unsigned int Leaf_4b98f0::get() { return *(unsigned int*)((char*)this + 0x24); }


class Leaf_4b9940 { public: unsigned int get(); };
unsigned int Leaf_4b9940::get() { return *(unsigned int*)((char*)this + 0x64); }


// int FactoryClass::Kind_Of() const (0x004B9F20, slot 11): RTTI_FACTORY (0x0C).

int FactoryClass::Kind_Of() const
{
    return 0x0C;
}


// int FactoryClass::Size_Of() const (0x004B9F30, slot 12): the object size (0x70).

int FactoryClass::Size_Of() const
{
    return sizeof(FactoryClass);
}


// Compiler-generated scalar deleting destructor emitted with FactoryClass destructor.


// 0x004ba060: function not yet identified
//
// Owner: DynamicVectorClass<TechnoTypeClass *>, virtual slot 3 (vtable +0xc) (from RTTI/vtables).
// Body: 42 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void __fastcall FUN_004ba060(int param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004ba060 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);

void __fastcall Clear_004ba060(int vector_address)
{
    *(unsigned int *)(vector_address + 0x10) = 0;
    if ((*(int *)(vector_address + 4) != 0) && (*(char *)(vector_address + 0xd) != '\0')) {
        operator_delete_00781d3b((unsigned int)(*(int *)(vector_address + 4)));
        *(unsigned int *)(vector_address + 4) = 0;
    }
    *(unsigned char *)(vector_address + 0xd) = 0;
    *(unsigned int *)(vector_address + 8) = 0;
    return;
}

}  // namespace seed_004ba060


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<TechnoTypeClass *>::Fetch (0x004BA0B0; VectorClass<TechnoTypeClass *> vtable 0x007a0f64 slot 6).
// VectorClass<TechnoTypeClass *>::operator equal (0x004BA0C0; VectorClass<TechnoTypeClass *> vtable 0x007a0f64 slot 1).
// VectorClass<TechnoTypeClass *>::Resize (0x004BA100; VectorClass<TechnoTypeClass *> vtable 0x007a0f64 slot 2).
// VectorClass<TechnoTypeClass *>::Clear (0x004BA1B0; VectorClass<TechnoTypeClass *> vtable 0x007a0f64 slot 3).
// VectorClass<TechnoTypeClass *>::ID_object (0x004BA1E0; VectorClass<TechnoTypeClass *> vtable 0x007a0f64 slot 4).
// VectorClass<TechnoTypeClass *>::ID_pointer (0x004BA220; VectorClass<TechnoTypeClass *> vtable 0x007a0f64 slot 5).
// DynamicVectorClass<TechnoTypeClass *>::Resize (0x004BA240; DynamicVectorClass<TechnoTypeClass *> vtable 0x007a0f44 slot 2).
// DynamicVectorClass<TechnoTypeClass *>::ID_object (0x004BA2F0; DynamicVectorClass<TechnoTypeClass *> vtable 0x007a0f44 slot 4).


extern "C" unsigned char vtable_4ba320[]; extern "C" void __cdecl release_buffer_4ba320(void*); extern "C" void __cdecl release_object_4ba320(void*);
class Leaf_4ba320 { public: void* get(unsigned int flags); };
void* Leaf_4ba320::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4ba320; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4ba320((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4ba320(this); return this; }


extern "C" unsigned char vtable_4ba370[]; extern "C" void __cdecl release_buffer_4ba370(void*); extern "C" void __cdecl release_object_4ba370(void*);
class Leaf_4ba370 { public: void* get(unsigned int flags); };
void* Leaf_4ba370::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4ba370; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4ba370((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4ba370(this); return this; }


extern "C" unsigned char table_4ba3c0[]; extern "C" void* __cdecl storage_4ba3c0(unsigned int);
class Storage_4ba3c0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4ba3c0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4ba3c0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4ba3c0(requested << 2); owns=1; } return this; }
