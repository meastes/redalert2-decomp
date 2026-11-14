// empulse.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/empulse.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x004B4970 and its
// list globals (EMPulseClass); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

// Complete evidenced class layout for this sweep.
#include "shared/unknown_globals.hpp"
#include "shared/abstract.hpp"
#include "shared/empulse_class.hpp"

DynamicVectorClass<EMPulseClass *> EMPulseClass::Array;


// Dynamic initializer (0x004B49A0, _$E5) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B49C0, _$E8) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B49E0, _$E11) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4A00, _$E14) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4A20, _$E17) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4A40, _$E20) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4A80, _$E23) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4AB0, _$E26) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4AE0, _$E29) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4B10, _$E32) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4B20, _$E35) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4B40, _$E38) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// Dynamic initializer (0x004B4B60, _$E45) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.


// atexit destructor (0x004B4BA0, _$E42) emitted by HeaderStatics_diagonal_init_004b4970.cpp; no source of its own.



EMPulseClass::~EMPulseClass()
{
    if (unknown_flag_a40958) Clear_Cells_004B5210();
    Unknown_006ECB90(this, true);
    Array.Delete(this);
}
EMPulseClass::EMPulseClass(Cell position, int radius, int duration, TechnoClass *source)
    : Position(position), Radius(radius), CreatedFrame(Frame), Duration(duration)
{
    Create_ID();
    Array.Add(this);
    Apply_004B4E30(source);
}
EMPulseClass::EMPulseClass(NoInitClass const &noinit) : AbstractClass(noinit)
{
}
void EMPulseClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(Position.X);
    crc(Position.Y);
    crc((long)Radius);
    crc((long)CreatedFrame);
    crc((long)Duration);
}
HRESULT __stdcall EMPulseClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (result >= 0) {
        NoInitClass noinit;
        new(this) EMPulseClass(noinit);
        result = 0;
    }
    return result;
}
void EMPulseClass::Delete_All_004B4DC0()
{
    while (Array.Count()) delete Array[0];
}
void EMPulseClass::Expire_004B4DF0()
{
    for (int index=Array.Count()-1; index>=0; --index) {
        EMPulseClass *pulse=Array[index];
        if (Frame >= pulse->Duration + pulse->CreatedFrame) delete pulse;
    }
}


// Emitted by EMPulseClass_Construct_004b4c00.cpp, which defines the lifetime family in one translation unit.


// Emitted by EMPulseClass_Construct_004b4c00.cpp, which defines the lifetime family in one translation unit.


// Emitted by EMPulseClass_Construct_004b4c00.cpp, which defines the lifetime family in one translation unit.


// Emitted by EMPulseClass_Construct_004b4c00.cpp, which defines the lifetime family in one translation unit.


#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/building.hpp"
#include "shared/building_type.hpp"
#include "shared/unit.hpp"
#include "shared/unit_type.hpp"
#include "shared/aircraft.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/random.hpp"
#include "shared/unknown_vector3.hpp"
// Existing full map and shared object interfaces; no reserved headers edited.
// The source pointer is forwarded to Foot slot224 and then the existing
// TechnoClass::unknown_6d3040(TechnoClass *) (004CD4B0 evidence).

inline CoordStruct EMPulse_Cell_Center(Cell const &cell)
{
    return CoordStruct(cell.X*256+128,cell.Y*256+128,0);
}
inline UnknownVector3 EMPulse_Offset(CoordStruct const &position, CoordStruct const &center)
{
    return UnknownVector3(position.X-center.X,position.Y-center.Y,position.Z-center.Z);
}

void EMPulseClass::Apply_004B4E30(TechnoClass *source)
{
    Cell cell;
    if (CreatedFrame + Duration > Frame) {
        int radiusSquared = Radius * Radius;
        for (int index=0; index<Aircraft.Count(); ++index) {
            AircraftClass *aircraft=Aircraft[index];
            if (aircraft->unknown_74 && !aircraft->unknown_76 && !aircraft->In_Air() && aircraft->Strength>0) {
                UnknownVector3 delta;
                delta=EMPulse_Offset(aircraft->Center_Coord(),EMPulse_Cell_Center(Position));
                if (delta.Length() < Radius*256) aircraft->unknown_slot224((int)source);
            }
        }
        for (int y=-Radius; y<=Radius; ++y) {
            for (int x=-Radius; x<=Radius; ++x) {
                cell=Cell((short)(Position.X+x),(short)(Position.Y+y));
                if (cell.X>=0 && cell.X<512 && cell.Y>=0 && cell.Y<512 && Map.Has_Cell(cell)) {
                    int dx=cell.X-Position.X;
                    int dy=cell.Y-Position.Y;
                    if (dy*dy+dx*dx <= radiusSquared) {
                        CellClass *mapCell=&Map[cell];
                        BuildingClass *building=(BuildingClass *)mapCell->Cell_Building();
                        if (building) {
                            Cell building_cell=Coord_Cell(building->Center_Coord());
                            if (building_cell==cell && !building->Type->InvisibleInGame) {
                                building->unknown_44d6c0();
                                building->unknown_3fc=Duration;
                                if (building->Type->Radar) building->House->unknown_5669=true;
                            }
                        } else {
                            bool active=false;
                            ObjectClass *object=mapCell->Cell_Techno(Point2D(0,0),false,0);
                            if (object && (object->Kind_Of()==1 || object->Kind_Of()==2)) active=object->AbstractFlags.IsFoot;
                            if (active) {
                                FootClass *foot=(FootClass *)object;
                                if (foot->Locomotor && ((foot->Kind_Of()!=1 || !((UnitClass *)foot)->Type->LargeVisceroid)
                                    || !((UnitClass *)foot)->Type->SmallVisceroid)) {
                                    foot->Locomotor.Checked()->Power_Off();
                                    if (foot->Locomotor.Checked()->Is_Moving()) foot->Locomotor.Checked()->Stop_Moving();
                                    foot->unknown_3fc=Duration;
                                    AnimClass *effect=(AnimClass *)operator new(sizeof(AnimClass));
                                    if (effect) {
                                        int delay=unknown_id_source_a3d290->Random()(0,25);
                                        effect=new(effect) AnimClass(Rules->unknown_1438,foot->Center_Coord(),delay,1,0x600,0);
                                    }
                                    if (effect) effect->Attach_To(foot);
                                }
                            }

                        }
                    }
                }
            }
        }
    }
}


// Use existing canonical map declarations; no map-header proposal.

void EMPulseClass::Clear_Cells_004B5210()
{
    int y = -Radius;
    int radiusSquared = Radius * Radius;
    for (; y <= Radius; ++y) {
        for (int x = -Radius; x <= Radius; ++x) {
            Cell cell;
            cell = Cell((short)(Position.X + x), (short)(Position.Y + y));
            if (cell.X >= 0 && cell.X < 512 && cell.Y >= 0 && cell.Y < 512 && Map.Has_Cell(cell)) {
                int dx = cell.X - Position.X;
                int dy = cell.Y - Position.Y;
                if (dy * dy + dx * dx <= radiusSquared) {
                    CellClass *mapCell = &Map[cell];
                    mapCell->unknown_140 &= 0xfff7ffffU;
                }
            }
        }
    }
}


// Emitted by EMPulseClass_Construct_004b4c00.cpp, which defines the lifetime family in one translation unit.


// HRESULT EMPulseClass::GetClassID(GUID *clsid) (0x004B5340, slot 3)

HRESULT __stdcall EMPulseClass::GetClassID(GUID *clsid)
{
    if (!clsid) {
        return E_POINTER;
    }
    *clsid = CLSID_EMPulseClass;
    return S_OK;
}


// Emitted by EMPulseClass_Construct_004b4c00.cpp, which defines the lifetime family in one translation unit.


// Emitted by EMPulseClass_Construct_004b4c00.cpp, which defines the lifetime family in one translation unit.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// 004b5510: emitted by this unit's use of its list


// 0x004b53d0: function not yet identified
//
// Owner: EMPulseClass, virtual slot 6 (vtable +0x18) (from RTTI/vtables).
// Body: 29 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int Save_004b53d0(undefined4 param_1,undefined4 param_2,undefined4 param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004b53d0 {

extern unsigned int __stdcall Save_004100c0(int, int, int);

int __stdcall Save_004b53d0(unsigned int object_address, unsigned int stream_address, unsigned int clear_dirty)
{
    int result;

    result = (int)(Save_004100c0((int)(object_address), (int)(stream_address), (int)(clear_dirty)));
    if (result >= 0) {
        result = (int)(0);
    }
    return result;
}

}  // namespace seed_004b53d0


class Leaf_4b53f0 { public: unsigned int get(); };
unsigned int Leaf_4b53f0::get() { return 0x34; }


class Leaf_4b5400 { public: unsigned int get(); };
unsigned int Leaf_4b5400::get() { return 0x37; }


extern "C" void __cdecl release_4b54b0(void*);
class Cleanup_4b54b0 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_4b54b0::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_4b54b0(pointer); items=0; } owns=0; count=0; }


extern "C" void __cdecl release_4b5600(void*);
class Cleanup_4b5600 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; public: void get(); };
void Cleanup_4b5600::get() { unsigned int* pointer=items; if(pointer && owns) { release_4b5600(pointer); items=0; } owns=0; count=0; }


extern "C" unsigned char vtable_4b5770[]; extern "C" void __cdecl release_buffer_4b5770(void*); extern "C" void __cdecl release_object_4b5770(void*);
class Leaf_4b5770 { public: void* get(unsigned int flags); };
void* Leaf_4b5770::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4b5770; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4b5770((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4b5770(this); return this; }


// 0x004b57c0: function not yet identified
//
// Body: 73 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_004b57c0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004b57c0 {

extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a0d6c;

struct Owner_004b57c0 { unsigned int * FUN_004b57c0(unsigned char deletion_flags); };

unsigned int * Owner_004b57c0::FUN_004b57c0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a0d6c);
    if ((object_words[1] != 0) && (*(char *)((int)object_words + 0xd) != '\0')) {
        FUN_00781d3b((unsigned int)(object_words[1]));
        object_words[1] = 0;
    }
    *(unsigned char *)((int)object_words + 0xd) = 0;
    object_words[2] = 0;
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_004b57c0


// 0x004b5810: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_004b5810(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004b5810 {

void  FUN_004b5810(void)
{
    return;
}

}  // namespace seed_004b5810


// 0x004b5820: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_004b5820(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004b5820 {

void  FUN_004b5820(void)
{
    return;
}

}  // namespace seed_004b5820
