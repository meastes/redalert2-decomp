// ebolt.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ebolt.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x004B1580 and the
// unit's EBolt list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/ebolt.hpp"

// The unit's list of EBolt objects (0x00853050; name is a hypothesis).
DynamicVectorClass<EBolt *> unknown_EBolt_list_00853050;


// Dynamic initializer (0x004B15B0, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B15D0, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B15F0, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B1610, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B1630, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B1650, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B1690, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B16C0, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B16F0, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B1720, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B1730, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B1750, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B1770, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


// Dynamic initializer (0x004B17B0, _$E) emitted by HeaderStatics_diagonal_init_004b1580.cpp; no source of its own.


EBolt::EBolt() : Start(CoordStruct(0,0,0)),End(CoordStruct(0,0,0)),unknown_18(0),unknown_28(65536),unknown_2c(false) { unknown_1c=0; Owner=0; unknown_24=0; }


#include "shared/tactical.hpp"
#include "shared/convert.hpp"
#include "shared/surface.hpp"
extern ConvertClass *unknown_drawer_8323c4;
extern Surface *unknown_surface_839964;
extern Rect unknown_rect_839630;
int __fastcall Unknown_006A2970(int height);
void EBolt::Draw_Line_004B1850(CoordStruct start,CoordStruct end,int color,int first_depth,int second_depth)
{
    Point2D first_pixel,second_pixel;
    unknown_tactical_83996c->Coord_To_Pixel(start,first_pixel);
    unknown_tactical_83996c->Coord_To_Pixel(end,second_pixel);
    int first_z=first_depth-Unknown_006A2970(start.Z)-2;
    int second_z=second_depth-Unknown_006A2970(end.Z)-2;
    int pixel=unknown_drawer_8323c4->Convert(color);
    unknown_surface_839964->Draw_Depth_Shaded_Line(unknown_rect_839630,first_pixel,second_pixel,
        pixel,first_z,second_z);
}


// 0x004b1830: function not yet identified
//
// Body: 19 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_004b1830(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004b1830 {

void __fastcall FUN_004b1830(int object_address)
{
    if (*(int *)(object_address + 0x20) != 0) {
        *(unsigned int *)(*(int *)(object_address + 0x20) + 0x5c4) = 0;
        *(unsigned int *)(object_address + 0x20) = 0;
    }
    return;
}

}  // namespace seed_004b1830


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void EBolt::Draw_Bolt_004B1900(CoordStruct start, CoordStruct end, int color) (0x004B1900): from
// ra2-decomp EBolt::FUN_004C1F20 (YR 0x004C1F20), adapted to our names. Splits the bolt into three
// jittered strands by repeated midpoint displacement (an explicit stack of eight segments) and draws
// each finished piece with Draw_Line_004B1850, the main strand in colour 15.
#include "shared/fast_math.hpp"
#include "shared/random2.hpp"
#include "shared/crt_memory.hpp"
#include "shared/unknown_vector3.hpp"
extern ConvertClass *unknown_drawer_8323c4;
extern Surface *unknown_surface_839964;
extern Rect unknown_rect_839630;
extern Random2Class unknown_random_839218;
int __fastcall Unknown_006A2970(int height);

// One pending piece of the bolt: the three strands' ends and the subdivision state.
struct BoltSegment {
    CoordStruct Start[3];
    CoordStruct End[3];
    int Length;
    int Deviation;
    int StartDepth;
    int EndDepth;
};

// The length of a lepton vector through the fast square root, summed y, z, x (as TPoint3D::Length_YZX).
static inline int Length_YZX(UnknownVector3 const &vector)
{
    double y = vector.Y;
    double z = vector.Z;
    double x = vector.X;
    return (int)Fast_Sqrt(y * y + (z * z + x * x));
}

void EBolt::Draw_Bolt_004B1900(CoordStruct start, CoordStruct end, int color)
{
    BoltSegment stack[8];
    CoordStruct mids[3];
    CoordStruct starts[3];
    CoordStruct ends[3];
    int offsets[6];
    bool first = true;
    int depth = 0;

    UnknownVector3 delta = UnknownVector3(start.X - end.X, start.Y - end.Y, start.Z - end.Z);
    int length = Length_YZX(delta);
    if (length != 0) {
        int threshold = (length * 102) >> 8;
        int deviation = (length * 23) >> 8;
        int start_z = unknown_18;
        BoltSegment *seg = stack;
        int end_z = 0;
        for (int i = 0; i < 3; i++) {
            starts[i] = start;
            ends[i] = end;
        }

        for (;;) {
            if (length > 64 && depth < 8) {
                for (int j = 0; j < 3; j++) {
                    mids[j].X = (starts[j].X + ends[j].X) >> 1;
                    mids[j].Y = (starts[j].Y + ends[j].Y) >> 1;
                    mids[j].Z = (starts[j].Z + ends[j].Z) >> 1;
                }
                if (first) {
                    first = false;
                    double dev = deviation;
                    for (int k = 0; k < 6; k++) {
                        offsets[k] = Fast_Sin(unknown_1c * 3.141592653589793 / (k + 7)) * dev;
                    }
                    int zoff = (offsets[2] + offsets[4] + deviation * 2) >> 1;
                    for (int m = 0; m < 3; m++) {
                        mids[m].X += offsets[0] + offsets[3];
                        mids[m].Y += offsets[1] + offsets[5];
                        mids[m].Z += zoff;
                    }
                }
                if (length <= 128) {
                    mids[0].X += unknown_random_839218(-1, 1) * deviation * 2;
                    mids[0].Y += unknown_random_839218(-1, 1) * deviation * 2;
                    mids[0].Z += unknown_random_839218(-1, 1) * deviation * 2;
                } else {
                    mids[0].X += unknown_random_839218(-deviation, deviation);
                    mids[0].Y += unknown_random_839218(-deviation, deviation);
                    mids[0].Z += unknown_random_839218(-deviation, deviation);
                }
                if (length > threshold) {
                    for (int n = 1; n < 3; n++) {
                        mids[n].X = mids[0].X + (unknown_random_839218(-deviation, deviation) >> 1);
                        mids[n].Y = mids[0].Y + (unknown_random_839218(-deviation, deviation) >> 1);
                        mids[n].Z = mids[0].Z + (unknown_random_839218(-deviation, deviation) >> 1);
                    }
                } else {
                    for (int n2 = 1; n2 < 3; n2++) {
                        mids[n2].X += unknown_random_839218(-deviation, deviation);
                        mids[n2].Y += unknown_random_839218(-deviation, deviation);
                        mids[n2].Z += unknown_random_839218(-deviation, deviation);
                    }
                }
                length >>= 1;
                deviation >>= 1;
                int mid_z = (end_z + start_z) >> 1;
                for (int p = 0; p < 3; p++) {
                    seg->Start[p] = mids[p];
                    seg->End[p] = ends[p];
                }
                seg->Length = length;
                seg->Deviation = deviation;
                seg->StartDepth = mid_z;
                seg->EndDepth = end_z;
                seg++;
                depth++;
                memcpy(ends, mids, sizeof(ends));
                end_z = mid_z;
            } else {
                Draw_Line_004B1850(starts[1], ends[1], color, start_z, end_z);
                Draw_Line_004B1850(starts[2], ends[2], color, start_z, end_z);
                Draw_Line_004B1850(starts[0], ends[0], 15, start_z, end_z);
                if (--depth < 0) break;
                seg--;
                memcpy(ends, seg->End, sizeof(ends));
                memcpy(starts, seg->Start, sizeof(starts));
                length = seg->Length;
                deviation = seg->Deviation;
                start_z = seg->StartDepth;
                end_z = seg->EndDepth;
            }
        }
    }
}


extern Rect unknown_rect_839630;
void EBolt::Draw_004B2170()
{
 if(unknown_28) {
  Point2D start,end;
  unknown_tactical_83996c->Coord_To_Pixel(Start,start);
  unknown_tactical_83996c->Coord_To_Pixel(End,end);
  if(Clip_Line(start,end,unknown_rect_839630)) Draw_Bolt_004B1900(Start,End,unknown_2c ? 5 : 10);
  ++unknown_1c;
 }
 unknown_28>>=1;
}


// The no-argument fastcall convention is an exact-code hypothesis.
void __fastcall EBolt::Update_All_004B2210()
{
 for(int i=unknown_EBolt_list_00853050.Count()-1;i>=0;--i) {
  EBolt *bolt=unknown_EBolt_list_00853050[i];
  if(bolt->Owner) {
   CoordStruct coord=bolt->Owner->unknown_slot43(bolt->unknown_24);
   if(coord!=unknown_geometry_l) bolt->Start=coord;
  }
  bolt->Draw_004B2170();
  if(!bolt->unknown_28) {
   unknown_EBolt_list_00853050.Delete(bolt);
   delete bolt;
  }
 }
}


void EBolt::Delete_All_004B23B0()
{
 for(int i=0;i<unknown_EBolt_list_00853050.Count();++i) delete unknown_EBolt_list_00853050[i];
 unknown_EBolt_list_00853050.Clear();
}


#include "shared/random.hpp"
#include "shared/particle_system.hpp"
void EBolt::Setup_004B2430(CoordStruct start,CoordStruct end,int value)
{
 Start=start;
 End=end;
 unknown_18=value;
 unknown_1c=unknown_random_839218(0,256);
 unknown_EBolt_list_00853050.Add(this);
 new ParticleSystemClass(Rules->unknown_ce0,end,0,0,unknown_geometry_l);
}


CoordStruct EBolt::Owner_Coord_004B2510()
{
 if(Owner) return Owner->unknown_slot43(unknown_24);
 return unknown_geometry_l;
}


void EBolt::Attach_004B2580(TechnoClass *owner,int index)
{
 if(owner && owner->Kind_Of()==1 && owner->unknown_7d && !owner->unknown_76) {
  Owner=static_cast<UnitClass *>(owner);
  unknown_24=index;
 }
}


// 0x004b25c0: function not yet identified
//
// Body: 19 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void __fastcall FUN_004b25c0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004b25c0 {

void __fastcall FUN_004b25c0(int object_address)
{
    if (*(int *)(object_address + 0x20) != 0) {
        *(unsigned int *)(*(int *)(object_address + 0x20) + 0x5c4) = 0;
        *(unsigned int *)(object_address + 0x20) = 0;
    }
    return;
}

}  // namespace seed_004b25c0


// CoordStruct::CoordStruct() (0x004B25E0)
// Inline empty constructor in shared/coord.hpp, emitted out of line by this unit: the vector
// constructor iterator of Draw_Bolt_004B1900's BoltSegment stack calls it.
// No source of its own: emitted by this unit (the EBolt unit).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<EBolt *>::Clear (0x004B25F0; DynamicVectorClass<EBolt *> vtable 0x007a0c9c slot 3).
// DynamicVectorClass<EBolt *>::ID_pointer (0x004B2620; DynamicVectorClass<EBolt *> vtable 0x007a0c9c slot 5).
// VectorClass<EBolt *>::Fetch (0x004B2640; VectorClass<EBolt *> vtable 0x007a0cbc slot 6).
// VectorClass<EBolt *>::operator equal (0x004B2650; VectorClass<EBolt *> vtable 0x007a0cbc slot 1).
// VectorClass<EBolt *>::Resize (0x004B2690; VectorClass<EBolt *> vtable 0x007a0cbc slot 2).
// VectorClass<EBolt *>::Clear (0x004B2740; VectorClass<EBolt *> vtable 0x007a0cbc slot 3).
// VectorClass<EBolt *>::ID_object (0x004B2770; VectorClass<EBolt *> vtable 0x007a0cbc slot 4).
// VectorClass<EBolt *>::ID_pointer (0x004B27B0; VectorClass<EBolt *> vtable 0x007a0cbc slot 5).
// DynamicVectorClass<EBolt *>::Resize (0x004B27D0; DynamicVectorClass<EBolt *> vtable 0x007a0c9c slot 2).
// DynamicVectorClass<EBolt *>::ID_object (0x004B2880; DynamicVectorClass<EBolt *> vtable 0x007a0c9c slot 4).


extern "C" unsigned char vtable_4b28b0[]; extern "C" void __cdecl release_buffer_4b28b0(void*); extern "C" void __cdecl release_object_4b28b0(void*);
class Leaf_4b28b0 { public: void* get(unsigned int flags); };
void* Leaf_4b28b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4b28b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4b28b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4b28b0(this); return this; }


extern "C" unsigned char vtable_4b2900[]; extern "C" void __cdecl release_buffer_4b2900(void*); extern "C" void __cdecl release_object_4b2900(void*);
class Leaf_4b2900 { public: void* get(unsigned int flags); };
void* Leaf_4b2900::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4b2900; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4b2900((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4b2900(this); return this; }
