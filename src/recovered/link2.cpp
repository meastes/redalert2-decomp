// link2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/link2.json carries the addresses and the rows that
// stay in their own files.


#define RA2_DSURFACE_DRAW_TRAIL_LINE
#define VECTOR_ADD_NAMED_LENGTH
// Header-static dynamic initializers of the unit at 0x00539170 and the
// unit's LineTrail list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/line_trail.hpp"

// The unit's list of LineTrail objects (0x00A6E970; name is a hypothesis).
DynamicVectorClass<LineTrail *> unknown_LineTrail_list_00a6e970;


// Dynamic initializer (0x005391A0, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x005391C0, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x005391E0, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x00539200, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x00539220, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x00539240, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x00539280, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x005392B0, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x005392E0, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x00539310, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x00539320, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x00539340, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x00539360, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// Dynamic initializer (0x005393A0, _$E) emitted by HeaderStatics_diagonal_init_00539170.cpp; no source of its own.


// LineTrail constructor: 0x005393E0, complete native body164 bytes +12 NOP.

LineTrail::LineTrail() : Color(128,128,128), Owner(0), Decrement(16), ActiveSlot(0)
{
    for (int index=0;index<32;++index) {
        Trails[index].Position=unknown_geometry_l;
        Trails[index].Value=0;
    }
    unknown_LineTrail_list_00a6e970.Add(this);
}


// LineTrail::~LineTrail() (0x00539490)

LineTrail::~LineTrail()
{
    Clear_Owner();
    unknown_LineTrail_list_00a6e970.Delete(this);
}


// void LineTrail::Clear_Owner() (0x005394F0)
#include "shared/object.hpp"

void LineTrail::Clear_Owner()
{
    if (Owner != 0) {
        Owner->unknown_94 = 0;
        Owner = 0;
    }
}


// void LineTrail::Set_Decrement(int decrement) (0x00539510)
#include "shared/unknown_globals.hpp"

void LineTrail::Set_Decrement(int decrement)
{
    if (!unknown_a40b2c) {
        decrement <<= 1;
    }
    Decrement = decrement;
}


// void LineTrail::AI() (0x00539530)
// Starts a new point at full intensity when the owner has moved since the newest point, then fades
// every point by Decrement (not below zero), newest first. The slot step goes through a reference
// (`slot`), which makes VC6 reload ActiveSlot instead of reusing the compare's load.

void LineTrail::AI()
{
    if (Owner != 0) {
        CoordStruct coord = Owner->Coord;
        if (coord != Trails[ActiveSlot].Position) {
            int &slot = ActiveSlot;     // newest slot steps back, wrapping to 31
            if (--slot < 0) {
                slot = 31;
            }
            LineTrailNode &node = Trails[ActiveSlot];
            node.Position = coord;
            node.Value = 255;
        }
    }

    int index = ActiveSlot;
    do {
        LineTrailNode &node = Trails[index];
        node.Value -= Decrement;
        if (node.Value < 0) {
            node.Value = 0;
        }
        index++;
        if (index >= 32) {
            index = 0;
        }
    } while (index != ActiveSlot);
}


// LineTrail::Draw() (0x005395C0): from the newest point on, draws a line to each next live point
// with the older point's intensity, at the depths of both points, until a faded or unused point or
// the ring wraps back to the newest.
// From ra2-decomp 0x00556C00 (YR identical twin, src/link2.cpp LineTrail::Draw):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#include "shared/dsurface.hpp"
#include "shared/tactical.hpp"
#include "shared/tool_tip.hpp"

void LineTrail::Draw()
{
    int slot = ActiveSlot + 1;
    if (slot >= 32) slot = 0;

    LineTrailNode *to = &Trails[ActiveSlot];
    if (to->Position == unknown_geometry_l) return;

    for (;;) {
        LineTrailNode *from = to;
        to = &Trails[slot];
        if (from->Value == 0 || to->Position == unknown_geometry_l) break;

        Point2D start;
        Point2D end;
        unknown_tactical_83996c->Coord_To_Pixel(from->Position, start);
        unknown_tactical_83996c->Coord_To_Pixel(to->Position, end);
        int start_height = -2 - Unknown_006A2970(from->Position.Z);
        int end_height = -2 - Unknown_006A2970(to->Position.Z);
        ((DSurface *)unknown_surface_839964)->Draw_Trail_Line_4AE9C0(unknown_rect_839630, start, end, Color,
                                                                     from->Value, start_height, end_height);

        slot++;
        if (slot >= 32) slot = 0;
        if (slot == ActiveSlot) break;
    }
}


// int LineTrail::Is_Dead() const (0x005396E0)

int LineTrail::Is_Dead() const
{
    return Owner == 0 && Trails[ActiveSlot].Value == 0;
}


// void LineTrail::Update_All() (0x00539700)

void LineTrail::Update_All()
{
    for (int index = unknown_LineTrail_list_00a6e970.Count() - 1; index >= 0; index--) {
        LineTrail *trail = unknown_LineTrail_list_00a6e970[index];
        trail->AI();
        if (trail->Is_Dead()) {
            delete trail;
        } else {
            trail->Draw();
        }
    }
}


// void LineTrail::Delete_All() (0x005397B0)

void LineTrail::Delete_All()
{
    for (int index = 0; index < unknown_LineTrail_list_00a6e970.Count(); index++) {
        delete unknown_LineTrail_list_00a6e970[index];
    }
    unknown_LineTrail_list_00a6e970.Clear();
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<LineTrail *>::Clear (0x00539870; DynamicVectorClass<LineTrail *> vtable 0x007a5604 slot 3).
// DynamicVectorClass<LineTrail *>::ID_pointer (0x005398A0; DynamicVectorClass<LineTrail *> vtable 0x007a5604 slot 5).
// VectorClass<LineTrail *>::Fetch (0x005398C0; VectorClass<LineTrail *> vtable 0x007a5624 slot 6).
// VectorClass<LineTrail *>::operator equal (0x005398D0; VectorClass<LineTrail *> vtable 0x007a5624 slot 1).
// VectorClass<LineTrail *>::Resize (0x00539910; VectorClass<LineTrail *> vtable 0x007a5624 slot 2).
// VectorClass<LineTrail *>::Clear (0x005399C0; VectorClass<LineTrail *> vtable 0x007a5624 slot 3).
// VectorClass<LineTrail *>::ID_object (0x005399F0; VectorClass<LineTrail *> vtable 0x007a5624 slot 4).
// VectorClass<LineTrail *>::ID_pointer (0x00539A30; VectorClass<LineTrail *> vtable 0x007a5624 slot 5).
// DynamicVectorClass<LineTrail *>::Resize (0x00539A50; DynamicVectorClass<LineTrail *> vtable 0x007a5604 slot 2).
// DynamicVectorClass<LineTrail *>::ID_object (0x00539B00; DynamicVectorClass<LineTrail *> vtable 0x007a5604 slot 4).


extern "C" unsigned char vtable_539b30[]; extern "C" void __cdecl release_buffer_539b30(void*); extern "C" void __cdecl release_object_539b30(void*);
class Leaf_539b30 { public: void* get(unsigned int flags); };
void* Leaf_539b30::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_539b30; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_539b30((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_539b30(this); return this; }


extern "C" unsigned char vtable_539b80[]; extern "C" void __cdecl release_buffer_539b80(void*); extern "C" void __cdecl release_object_539b80(void*);
class Leaf_539b80 { public: void* get(unsigned int flags); };
void* Leaf_539b80::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_539b80; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_539b80((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_539b80(this); return this; }
