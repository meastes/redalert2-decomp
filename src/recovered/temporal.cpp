// temporal.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/temporal.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x006E1C80 and the
// unit's TemporalClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/temporal.hpp"

// The unit's list of TemporalClass objects (0x00AC02E0; name is a hypothesis).
DynamicVectorClass<TemporalClass *> unknown_TemporalClass_list_00ac02e0;


// Dynamic initializer (0x006E1CB0, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1CD0, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1CF0, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1D10, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1D30, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1D50, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1D90, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1DC0, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1DF0, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1E20, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1E30, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1E50, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1E70, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// Dynamic initializer (0x006E1EB0, _$E) emitted by HeaderStatics_diagonal_init_006e1c80.cpp; no source of its own.


// ??0TemporalClass@@QAE@XZ (0x006E1EF0)
// Listed in the unit list; only the timer is set.

TemporalClass::TemporalClass()
{
    unknown_TemporalClass_list_00ac02e0.Add(this);
}


// ??0TemporalClass@@QAE@PAVTechnoClass@@@Z (0x006E1F80)
// Link for owner, listed in the unit list.

TemporalClass::TemporalClass(TechnoClass *owner)
{
    Owner = owner;
    Target = 0;
    unknown_2c = CDTimerClass<FrameTimerClass>();
    unknown_40 = 0;
    unknown_44 = 0;
    unknown_TemporalClass_list_00ac02e0.Add(this);
    unknown_48 = 0;
    unknown_4c = 0;
    unknown_38 = 0;
    unknown_3c = 0;
}


// ??0TemporalClass@@QAE@ABVNoInitClass@@@Z (0x006E2040)
// Load path: the vtables and a fresh timer.

TemporalClass::TemporalClass(NoInitClass const &noinit) : AbstractClass(noinit)
{
}


// ??1TemporalClass@@UAE@XZ (0x006E2080)
// Unlists the link.

TemporalClass::~TemporalClass()
{
    unknown_TemporalClass_list_00ac02e0.Delete(this);
}


// ?Compute_CRC@TemporalClass@@UBEXAAVWWCRCEngine@@@Z (0x006E2100)
// Only the AbstractClass part.

void TemporalClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
}


// ?Load@TemporalClass@@UAGJPAUIStream@@@Z (0x006E2110)
// AbstractClass load, vtables, pointer fixups.

HRESULT __stdcall TemporalClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TemporalClass(NoInitClass());
        Swizzler.Swizzle((void **)&Owner);
        Swizzler.Swizzle((void **)&Target);
        Swizzler.Swizzle((void **)&unknown_40);
        Swizzler.Swizzle((void **)&unknown_44);
        Swizzler.Swizzle((void **)&unknown_38);
        Swizzler.Swizzle((void **)&unknown_3c);
    }
    return result;
}


// ?Save@TemporalClass@@UAGJPAUIStream@@H@Z (0x006E21D0)
// Only the AbstractClass part.

HRESULT __stdcall TemporalClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}


// ?GetClassID@TemporalClass@@UAGJPAUGUID@@@Z (0x006E21F0)
// The class identifier.

HRESULT __stdcall TemporalClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_TemporalClass;
    return S_OK;
}


#include "shared/techno.hpp"
#include "shared/anim.hpp"
#include "shared/super.hpp"

void TemporalClass::AI()
{
    if(Target!=0 && Target->unknown_20c==this && unknown_40!=0) {
        Target->unknown_20c=0;
        Target->unknown_204=false;
        Just_Let_Go();
        return;
    }
    unknown_48-=Get_Warp_Per_Step(0);
    if(unknown_48<=0) {
        if(Target==0) {
            Target=0;unknown_44=0;unknown_40=0;unknown_3c=0;unknown_38=0;
        } else {
            new AnimClass(Rules->WarpAway,Target->Position_Coord_004f3f40(),0,1,0x600,0);
        }
        if(Owner->Techno_Type_Class()->Trainable) {
            TechnoTypeClass *target_type=Target->Techno_Type_Class();
            TechnoTypeClass *owner_type=Owner->Techno_Type_Class();
            Owner->unknown_118.Made_A_Kill(owner_type->Cost_Of(Owner->House),target_type->Cost_Of(Target->House));
        }
        if(Target!=0) {
            if(Target->Kind_Of()==6) {
                BuildingClass *building=(BuildingClass *)Target;
                if(building!=0) {
                    if(building->unknown_4532f0()>0) building->Unknown_004536D0(0);
                    if(unknown_a40774==0) {
                        while(building->Cargo.Object!=0) {
                            ObjectClass *passenger=building->Cargo.Detach_Object();
                            delete passenger;
                        }
                    }
                }
                if(unknown_3c!=0) {
                    ((SuperClass *)unknown_3c)->Suspend(false);
                    unknown_3c=0;
                }
                Target->unknown_slot216((int)Owner);
                Target->Record_The_Kill(Owner);
                Target->Delete_Me();
                building->House->unknown_144=true;
            } else {
                Target->unknown_slot216((int)Owner);
                Target->Record_The_Kill(Owner);
                Target->Delete_Me();
            }
        }
        Target=0;unknown_44=0;unknown_40=0;unknown_3c=0;unknown_38=0;
    }
}

// External references checked against complete original body operands.


// int TemporalClass::Get_Warp_Per_Step(int step) (0x006E2470)
// The owner's current weapon damage (kept in +0x4C) plus that of every helper chained after it
// (followed at most 50 deep).

int TemporalClass::Get_Warp_Per_Step(int step)
{
    int helpers = 0;
    if (unknown_44 != 0 && step <= 50) {
        helpers = unknown_44->Get_Warp_Per_Step(step + 1);
    }
    unknown_4c = Owner->unknown_slot231(Owner->unknown_slot174(0))->WeaponType->unknown_a4;
    return unknown_4c + helpers;
}


// void TemporalClass::Detach_Techno(TechnoClass *target) (0x006E24C0)
// The owner going away lets go of the target; the target going away just forgets it and the chain.

void TemporalClass::Detach_Techno(TechnoClass *target)
{
    if (target == Owner) {
        if (Target != 0) {
            Let_Go_Of_Target();
            Target = 0;
        }
    } else if (target == Target) {
        Target = 0;
        unknown_44 = 0;
        unknown_40 = 0;
        unknown_3c = 0;
        unknown_38 = 0;
    }
}


// Existing accepted BuildingClass helper's legacy integer-this interface.
namespace seed_0044d380 { void __fastcall unknown_temporal_thaw_0044d380(int building); }

void TemporalClass::Let_Go_Of_Target()
{
    if(unknown_40==0) {
        if(unknown_44!=0) {
            if(Target!=0) Target->unknown_20c=unknown_44;
            unknown_44->unknown_40=0;
            unknown_44->unknown_48=unknown_48;
        } else if(Target!=0) {
            Target->unknown_20c=0;
            Target->unknown_204=false;
            if(Target->Kind_Of()==6) {
                TechnoClass *building=Target;
                building->House->unknown_5668=true;
                seed_0044d380::unknown_temporal_thaw_0044d380((int)building);
                building->House->unknown_144=true;
            }
            Target->Mark(2);
        }
    } else {
        if(unknown_44!=0) {
            unknown_44->unknown_40=unknown_40;
            if(unknown_40!=0) unknown_40->unknown_44=unknown_44;
        } else unknown_40->unknown_44=0;
    }
    Target=0;
    unknown_44=0;
    unknown_40=0;
    unknown_3c=0;
    unknown_38=0;
}

// External references checked against complete original body operands.


// 0x006e25f0: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_006e25f0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006e25f0 {

void __fastcall FUN_006e25f0(int temporal_address)
{
    if (*(int *)(temporal_address + 0x28) != 0) {
        *(unsigned int *)(*(int *)(temporal_address + 0x28) + 0x20c) = 0;
        *(unsigned char *)(*(int *)(temporal_address + 0x28) + 0x204) = 0;
        *(unsigned int *)(temporal_address + 0x28) = 0;
    }
    return;
}

}  // namespace seed_006e25f0


// 0x006e2670: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_006e2670(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006e2670 {

void  FUN_006e2670(void)
{
    return;
}

}  // namespace seed_006e2670


// void TemporalClass::Let_Go() (0x006E2680)
// Releases the target and unchains the links before and after this one (each lets go too).

void TemporalClass::Let_Go()
{
    if (Target != 0) {
        Target->unknown_204 = false;
        Target->unknown_20c = 0;
        Target = 0;
    }
    if (unknown_44 != 0) {
        if (unknown_44->unknown_40 == this) {
            unknown_44->unknown_40 = 0;
        }
        unknown_44->Just_Let_Go();
        unknown_44 = 0;
    }
    if (unknown_40 != 0) {
        if (unknown_40->unknown_44 == this) {
            unknown_40->unknown_44 = 0;
        }
        unknown_40->Just_Let_Go();
        unknown_40 = 0;
    }
    unknown_44 = 0;
    unknown_40 = 0;
    unknown_3c = 0;
    unknown_38 = 0;
}


// 0x006e26f0: function not yet identified
//
// Body: 18 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_006e26f0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006e26f0 {

void __fastcall FUN_006e26f0(int temporal_address)
{
    *(unsigned int *)(temporal_address + 0x28) = 0;
    *(unsigned int *)(temporal_address + 0x44) = 0;
    *(unsigned int *)(temporal_address + 0x40) = 0;
    *(unsigned int *)(temporal_address + 0x3c) = 0;
    *(unsigned int *)(temporal_address + 0x38) = 0;
    return;
}

}  // namespace seed_006e26f0


// void TemporalClass::Just_Let_Go() (0x006E2710)
// Releases the target and recursively the links chained before and after this one.

void TemporalClass::Just_Let_Go()
{
    Target->unknown_204 = false;
    Target->unknown_20c = 0;
    Target = 0;
    if (unknown_44 != 0) {
        if (unknown_44->unknown_40 == this) {
            unknown_44->unknown_40 = 0;
        }
        unknown_44->Just_Let_Go();
    }
    if (unknown_40 != 0) {
        if (unknown_40->unknown_44 == this) {
            unknown_40->unknown_44 = 0;
        }
        unknown_40->Just_Let_Go();
    }
    unknown_44 = 0;
    unknown_40 = 0;
    unknown_3c = 0;
    unknown_38 = 0;
}


// bool TemporalClass::Can_Warp_Target(TechnoClass *target) const (0x006E2770)
// A target can be erased when its type is Warpable and slot 203 does not hold (its +0x154 timer
// has run out).
#include "shared/techno_type.hpp"

bool TemporalClass::Can_Warp_Target(TechnoClass *target) const
{
    if (target == 0) {
        return false;
    }
    if (!target->Techno_Type_Class()->Warpable) {
        return false;
    }
    if (target->unknown_slot203()) {
        return false;
    }
    return true;
}


#include "shared/unit.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/capture_manager.hpp"
#include "shared/sidebar_building.hpp"
#include "shared/radar_event.hpp"
#include "shared/vox_class.hpp"
namespace seed_0044d320 { void __fastcall unknown_temporal_freeze_0044d320(int building); }

void TemporalClass::Unknown_006E27B0(TechnoClass *target)
{
    if(target!=0) {
        if(target->SpawnManager!=0) target->SpawnManager->Kill_Nodes();
        if(target->unknown_230!=0) ((CaptureManagerClass *)target->unknown_230)->Release_4681e0();
    }
    TemporalClass *owner_link=(TemporalClass *)Owner->unknown_208;
    if(owner_link!=0 && owner_link->Target!=0) owner_link->Let_Go_Of_Target();
    if(Can_Warp_Target(target) && Owner->unknown_20c==0) {
        Target=target;
        if(Target->unknown_20c==0) {
            target->unknown_20c=this;
            unknown_48=Target->Techno_Type_Class()->MaxStrength*10;
            if(Target!=0 && Target->Kind_Of()==1) {
                TechnoClass *object=Target;
                UnitClass *unit;
                if(object==0) unit=0;
                else unit=object->Kind_Of()==1 ? (UnitClass *)object:0;
                if(unit->Type->Harvester && Target->House==unknown_player_a35db4) {
                    Cell cell(Target->unknown_slot19(0));
                    if(Unknown_0063B5B0(4,cell)) Speech_Play_Name("EVA_OreMinerUnderAttack",-1,-1);
                }
            }
            if(Target->Kind_Of()==6 && !Target->Techno_Type_Class()->unknown_202 && !Target->unknown_slot31())
                Target->House->Attacked(As_Building(Target));
        } else {
            unknown_40=(TemporalClass *)Target->unknown_20c;
            unknown_44=unknown_40->unknown_44;
            unknown_40->unknown_44=this;
            if(unknown_44!=0) unknown_44->unknown_40=this;
        }
        Target->unknown_204=true;
        if(Target->Kind_Of()==6) {
            TechnoClass *building=Target;
            building->House->unknown_5668=true;
            seed_0044d320::unknown_temporal_freeze_0044d320((int)building);
            building->House->unknown_144=true;
        }
        Target->Mark(2);
        TemporalClass *target_link=(TemporalClass *)Target->unknown_208;
        if(target_link!=0 && target_link->Target!=0) target_link->Let_Go_Of_Target();
    }
}

// External references checked against complete original body operands.


// ?Size_Of@TemporalClass@@UBEHXZ (0x006E2A00)
// Object size.

int TemporalClass::Size_Of() const
{
    return sizeof(TemporalClass);
}


// ?Kind_Of@TemporalClass@@UBEHXZ (0x006E2A10)
// RTTI kind 0x46.

int TemporalClass::Kind_Of() const
{
    return 0x46;
}


// ??_GTemporalClass@@UAEPAXI@Z (0x006E2A20)
// Compiler generated (the destructor is expanded inline).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TemporalClass *>::Clear (0x006E2AB0; DynamicVectorClass<TemporalClass *> vtable 0x007ad2b4 slot 3).
// DynamicVectorClass<TemporalClass *>::ID_pointer (0x006E2AE0; DynamicVectorClass<TemporalClass *> vtable 0x007ad2b4 slot 5).
// VectorClass<TemporalClass *>::Fetch (0x006E2B00; VectorClass<TemporalClass *> vtable 0x007ad2d4 slot 6).
// VectorClass<TemporalClass *>::operator equal (0x006E2B10; VectorClass<TemporalClass *> vtable 0x007ad2d4 slot 1).
// VectorClass<TemporalClass *>::Resize (0x006E2B50; VectorClass<TemporalClass *> vtable 0x007ad2d4 slot 2).
// VectorClass<TemporalClass *>::Clear (0x006E2C00; VectorClass<TemporalClass *> vtable 0x007ad2d4 slot 3).
// VectorClass<TemporalClass *>::ID_object (0x006E2C30; VectorClass<TemporalClass *> vtable 0x007ad2d4 slot 4).
// VectorClass<TemporalClass *>::ID_pointer (0x006E2C70; VectorClass<TemporalClass *> vtable 0x007ad2d4 slot 5).
// DynamicVectorClass<TemporalClass *>::Resize (0x006E2C90; DynamicVectorClass<TemporalClass *> vtable 0x007ad2b4 slot 2).
// DynamicVectorClass<TemporalClass *>::ID_object (0x006E2D40; DynamicVectorClass<TemporalClass *> vtable 0x007ad2b4 slot 4).


extern "C" unsigned char vtable_6e2d70[]; extern "C" void __cdecl release_buffer_6e2d70(void*); extern "C" void __cdecl release_object_6e2d70(void*);
class Leaf_6e2d70 { public: void* get(unsigned int flags); };
void* Leaf_6e2d70::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6e2d70; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6e2d70((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6e2d70(this); return this; }


extern "C" unsigned char vtable_6e2dc0[]; extern "C" void __cdecl release_buffer_6e2dc0(void*); extern "C" void __cdecl release_object_6e2dc0(void*);
class Leaf_6e2dc0 { public: void* get(unsigned int flags); };
void* Leaf_6e2dc0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6e2dc0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6e2dc0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6e2dc0(this); return this; }
