// planmgr.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/planmgr.json carries the addresses and the rows that
// stay in their own files.


#define RA2_PLANNING_COMMIT_PLANS
#define RA2_PLANNING_PICK_ROWS
#define RA2_COORD_EQUAL_MEMBER
// Header-static dynamic initializers of the unit at 0x006101B0 and its
// list globals (EventClass, PlanningNodeClass, PlanningTokenClass, TechnoClass); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis
// (shared/planning.hpp, shared/event.hpp, shared/techno.hpp).
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/planning.hpp"

DynamicVectorClass<EventClass *> PlanningLists::unknown_list_00a76648;
DynamicVectorClass<PlanningNodeClass *> PlanningNodeClass::unknown_list_00a76758;
DynamicVectorClass<TechnoClass *> PlanningLists::unknown_list_00a76700;
DynamicVectorClass<PlanningNodeClass *> PlanningNodeClass::unknown_list_00a76628;
DynamicVectorClass<PlanningNodeClass *> PlanningNodeClass::unknown_list_00a766d8;
DynamicVectorClass<PlanningTokenClass *> PlanningTokenClass::unknown_list_00a76738;


// Dynamic initializer (0x006101E0, _$E5) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610200, _$E8) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610220, _$E11) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610240, _$E14) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610260, _$E17) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610280, _$E20) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x006102C0, _$E23) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x006102F0, _$E26) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610320, _$E29) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610350, _$E32) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610360, _$E35) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610380, _$E38) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x006103A0, _$E50) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// atexit destructor (0x006103E0, _$E47) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610440, _$E56) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// atexit destructor (0x00610480, _$E53) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x006104E0, _$E62) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// atexit destructor (0x00610520, _$E59) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610580, _$E68) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// atexit destructor (0x006105C0, _$E65) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x00610620, _$E73) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// atexit destructor (0x00610660, _$E71) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// Dynamic initializer (0x006106C0, _$E78) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


// atexit destructor (0x00610700, _$E76) emitted by HeaderStatics_diagonal_init_006101b0.cpp; no source of its own.


class Leaf_610760 { public: void get(); };
void Leaf_610760::get() {  }


// int __fastcall Planning_House(TechnoClass *techno) (0x00610770)
// The house a techno plans for: its own, or the player's when its side is
// human-controlled.

int __fastcall Planning_House(TechnoClass *techno)
{
    HouseClass *side = techno->House;
    int house = side->ArrayIndex;
    if (side->Is_Player_Control() && unknown_player_a35db4) {
        house = unknown_player_a35db4->ArrayIndex;
    }
    return house;
}


// Point2D __fastcall Planning_Coord_Pixel(CoordStruct const &coord) (0x006107A0)
// The tactical pointer and the screen offset are named locals; this body is big
// enough that callers whose inline allowance runs short call it (Is_At_Pixel,
// Draw_Marker, Draw_All) while Pixel(), Rect(), Outer_Rect() and Fill_Marker
// still expand it, as in the original.

Point2D __fastcall Planning_Coord_Pixel(CoordStruct const &coord)
{
    Point2D pixel;
    Tactical *tactical = unknown_tactical_83996c;
    tactical->Coord_To_Pixel(coord, pixel);
    int x = pixel.X;
    int y = pixel.Y;
    int offset_x = unknown_int_839630;
    int offset_y = unknown_int_839634;
    pixel.X = x + offset_x;
    pixel.Y = y + offset_y;
    return pixel;
}


// PlanningMemberClass::PlanningMemberClass() (0x006107E0)

PlanningMemberClass::PlanningMemberClass() :
    Owner(0),
    Event(0),
    BranchIndex(-1),
    unknown_0c(false)
{
}


// void PlanningMemberClass::Set_Owner(TechnoClass *owner) (0x00610800)

void PlanningMemberClass::Set_Owner(TechnoClass *owner)
{
    Owner = owner;
}


// void PlanningMemberClass::Set_Event(EventClass *event) (0x00610810)

void PlanningMemberClass::Set_Event(EventClass *event)
{
    Event = event;
}


// void PlanningMemberClass::Set_BranchIndex(int index) (0x00610820)

void PlanningMemberClass::Set_BranchIndex(int index)
{
    BranchIndex = index;
}


// void PlanningMemberClass::Set_unknown_0c(bool value) (0x00610830)

void PlanningMemberClass::Set_unknown_0c(bool value)
{
    unknown_0c = value;
}


// TechnoClass *PlanningMemberClass::Get_Owner() const (0x00610840)

TechnoClass *PlanningMemberClass::Get_Owner() const
{
    return Owner;
}


// EventClass *PlanningMemberClass::Get_Event() const (0x00610850)

EventClass *PlanningMemberClass::Get_Event() const
{
    return Event;
}


// int PlanningMemberClass::Get_BranchIndex() const (0x00610860)

int PlanningMemberClass::Get_BranchIndex() const
{
    return BranchIndex;
}


// bool PlanningMemberClass::Get_unknown_0c() const (0x00610870)

bool PlanningMemberClass::Get_unknown_0c() const
{
    return unknown_0c;
}


// PlanningBranchClass::PlanningBranchClass() : (0x00610880)

PlanningBranchClass::PlanningBranchClass() :
    Count(0),
    unknown_74(-1)
{
}


// void PlanningBranchClass::Set_Event(EventClass const &event) (0x006108A0)

void PlanningBranchClass::Set_Event(EventClass const &event)
{
    Event = event;
}


// void PlanningBranchClass::Set_Count(int value) (0x006108C0)

void PlanningBranchClass::Set_Count(int value)
{
    Count = value;
}


// void PlanningBranchClass::Set_unknown_74(int value) (0x006108D0)

void PlanningBranchClass::Set_unknown_74(int value)
{
    unknown_74 = value;
}


// void PlanningBranchClass::Clear_Count() (0x006108E0)

void PlanningBranchClass::Clear_Count()
{
    Count = 0;
}


// int PlanningBranchClass::Increment_Count() (0x006108F0)

int PlanningBranchClass::Increment_Count()
{
    return ++Count;
}


// int PlanningBranchClass::Decrement_Count() (0x00610900)

int PlanningBranchClass::Decrement_Count()
{
    return --Count;
}


// EventClass PlanningBranchClass::Get_Event() const (0x00610910)

EventClass PlanningBranchClass::Get_Event() const
{
    return Event;
}


// int PlanningBranchClass::Get_Count() const (0x00610930)

int PlanningBranchClass::Get_Count() const
{
    return Count;
}


// int PlanningBranchClass::Get_unknown_74() const (0x00610940)

int PlanningBranchClass::Get_unknown_74() const
{
    return unknown_74;
}


// CoordStruct PlanningBranchClass::Coord() const (0x00610950)

CoordStruct PlanningBranchClass::Coord() const
{
    CoordStruct coord = Planning_Event_Coord(Event);
    return coord;
}


// CoordStruct __fastcall Planning_Event_Coord(EventClass const &event) (0x00610980)
// For a type-4 event, the slot-22 coordinate of the first set reference
// (+0x0E, then +0x18, then +0x13); (-1, -1, -1) otherwise.

CoordStruct __fastcall Planning_Event_Coord(EventClass const &event)
{
    if (event.Type == 4) {
        PlanningEventTargets const &targets = (PlanningEventTargets const &)event;
        if (targets.Target_0e.Kind) {
            AbstractClass *object = targets.Target_0e.As_Abstract();
            if (object) {
                return object->unknown_slot22();
            }
        } else if (targets.Target_18.Kind) {
            AbstractClass *object = targets.Target_18.As_Abstract();
            if (object) {
                return object->unknown_slot22();
            }
        } else if (targets.Target_13.Kind) {
            AbstractClass *object = targets.Target_13.As_Abstract();
            if (object) {
                return object->unknown_slot22();
            }
        }
    }
    return CoordStruct(-1, -1, -1);
}


// Point2D PlanningBranchClass::Pixel() const (0x00610A20)
// The screen pixel of the branch event.

Point2D PlanningBranchClass::Pixel() const
{
    CoordStruct coord = Coord();
    return Planning_Coord_Pixel(coord);
}


// PlanningNodeClass::PlanningNodeClass(int unknown_18) (0x00610A80)

PlanningNodeClass::PlanningNodeClass(int unknown_18) :
    unknown_18(unknown_18),
    IsQueued(false),
    unknown_b0(-1),
    unknown_b4(-1)
{
}


// PlanningNodeClass::~PlanningNodeClass() (0x00610AF0)
// Resets the node's branches and takes it out of the planning lists.

PlanningNodeClass::~PlanningNodeClass()
{
    unknown_611ee0();
    Planning_Unlist_Node(this);
}


// 0x00610c10: function not yet identified
//
// Body: 4 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_get_18_00610c10(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00610c10 {

unsigned int __fastcall unknown_get_18_00610c10(int self)
{
    return *(unsigned int *)(self + 0x18);
}

}  // namespace seed_00610c10


// 0x00610c20: function not yet identified
//
// Body: 4 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_get_10_00610c20(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00610c20 {

unsigned int __fastcall unknown_get_10_00610c20(int self)
{
    return *(unsigned int *)(self + 0x10);
}

}  // namespace seed_00610c20


// int PlanningNodeClass::Index_For(TechnoClass *techno) (0x00610C30)

int PlanningNodeClass::Index_For(TechnoClass *techno)
{
    PlanningTokenClass *token = Planning_Token(techno);
    return token->Nodes.ID(this);
}


// PlanningNodeClass *PlanningNodeClass::Previous(TechnoClass *techno) (0x00610C60)

PlanningNodeClass *PlanningNodeClass::Previous(TechnoClass *techno)
{
    PlanningTokenClass *token = Planning_Token(techno);
    int index = token->Nodes.ID(this);
    if (index == -1) {
        return 0;
    }
    int count = token->Nodes.Count();
    if (count == 1) {
        return this;
    }
    if (--index < 0) {
        index = count - 1;
    }
    return token->Node_At(index);
}


// PlanningNodeClass *PlanningNodeClass::Next(TechnoClass *techno) (0x00610CE0)

PlanningNodeClass *PlanningNodeClass::Next(TechnoClass *techno)
{
    PlanningTokenClass *token = Planning_Token(techno);
    int index = token->Nodes.ID(this);
    if (index == -1) {
        return 0;
    }
    int count = token->Nodes.Count();
    if (count == 1) {
        return this;
    }
    if (++index >= count) {
        index = 0;
    }
    return token->Node_At(index);
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void PlanningNodeClass::Add_Member(TechnoClass *techno, EventClass const &event) (0x00610D60)
// Gives techno a member in this node with its own copy of the order and
// appends the node to techno's path.
// Shape from ra2-decomp 0x00633FA0 (PlanningNodeClass::Add_Member, src/planmgr.cpp).

void PlanningNodeClass::Add_Member(TechnoClass *techno, EventClass const &event)
{
    PlanningMemberClass *member = new PlanningMemberClass;
    member->Set_Owner(techno);
    EventClass *copy = new EventClass;
    *copy = event;
    member->Set_Event(copy);
    Members.Add(member);
    Planning_Token(techno)->Add_Node(this);
    unknown_611bd0(member);
}


// void PlanningNodeClass::Remove_Member(TechnoClass *techno) (0x00610E70)
// Takes techno's member out of the node: out of its branches, then frees its
// order copy and the member; a node left without members deletes itself.

void PlanningNodeClass::Remove_Member(TechnoClass *techno)
{
    PlanningMemberClass *member = Member_For(techno);
    if (member != 0) {
        unknown_611e20(member);
        delete member->Get_Event();
        Members.Delete(member);
        delete member;
        if (Members.Count() == 0) {
            delete this;
        }
    }
}


// int PlanningNodeClass::Member_Index(TechnoClass *techno) const (0x00611050)

int PlanningNodeClass::Member_Index(TechnoClass *techno) const
{
    int result = -1;
    for (int index = 0; index < Members.Count(); index++) {
        if (Members[index]->Owner == techno) {
            result = index;
            break;
        }
    }
    return result;
}


// TechnoClass *PlanningNodeClass::Member_Owner(int index) const (0x00611090)

TechnoClass *PlanningNodeClass::Member_Owner(int index) const
{
    PlanningMemberClass *member = Members[index];
    TechnoClass *owner = member->Owner;
    return owner;
}


// EventClass PlanningNodeClass::Member_Event(int index) const (0x006110A0)

EventClass PlanningNodeClass::Member_Event(int index) const
{
    return *Members[index]->Event;
}


// EventClass PlanningNodeClass::Event_For(TechnoClass *techno) const (0x006110D0)

EventClass PlanningNodeClass::Event_For(TechnoClass *techno) const
{
    return *Member_For(techno)->Event;
}


// bool PlanningNodeClass::Has_Member(TechnoClass *techno) const (0x00611120)

bool PlanningNodeClass::Has_Member(TechnoClass *techno) const
{
    return Member_Index(techno) != -1;
}


// HouseClass *PlanningNodeClass::House() const (0x00611170)

HouseClass *PlanningNodeClass::House() const
{
    return Members[0]->Owner->House;
}


// CoordStruct PlanningNodeClass::Coord() const (0x00611180)

CoordStruct PlanningNodeClass::Coord() const
{
    CoordStruct coord = Planning_Event_Coord(*Members[0]->Event);
    return coord;
}


// Point2D PlanningNodeClass::Pixel() const (0x006111C0)

Point2D PlanningNodeClass::Pixel() const
{
    CoordStruct coord = Coord();
    Point2D pixel = Planning_Coord_Pixel(coord);
    return pixel;
}


// int PlanningNodeClass::Is_At_Pixel(Point2D point) const (0x00611230)
// Whether a screen point lies inside the grown node marker.

int PlanningNodeClass::Is_At_Pixel(Point2D point) const
{
    ::Rect rect = Outer_Rect();
    if (rect.Is_Point_Within(point)) {
        return 1;
    }
    return 0;
}


// Rect __fastcall Planning_Centered_Rect(Point2D center, int size) (0x006112D0)
// The corner and size are named before the Rect is built (field locals); this
// larger body keeps the call out of line where Rect() sits two inline levels
// deep (Is_At_Pixel, Outer_Rect2, Draw_All) and inlined in Rect(), Outer_Rect()
// and Fill_Marker, as in the original.

Rect __fastcall Planning_Centered_Rect(Point2D center, int size)
{
    int half = -(size / 2);
    Point2D corner = center;
    corner.X += half;
    corner.Y += half;
    int x = corner.X;
    int y = corner.Y;
    int width = size;
    int height = size;
    Rect rect(x, y, width, height);
    return rect;
}


// bool PlanningNodeClass::Is_At(CoordStruct coord) const (0x00611310)
// Natural form (Scout1165 v9): the node's coordinate is fetched into a local and compared.

bool PlanningNodeClass::Is_At(CoordStruct coord) const
{
    CoordStruct here;
    here = Coord();
    return here == coord;
}


// Rect PlanningNodeClass::Rect() const (0x00611360)

Rect PlanningNodeClass::Rect() const
{
    ::Rect rect;
    rect = Planning_Centered_Rect(Pixel(), 8);
    return rect;
}


// bool PlanningNodeClass::Is_In_Loop() (0x006113F0)
// True when this node lies in the closed loop of its first member's planning
// token: the token has a loop start and the node's index in the token's path
// is at or past it.

bool PlanningNodeClass::Is_In_Loop()
{
    if (!Members.Count()) {
        return false;
    }
    PlanningTokenClass *token = Planning_Token(Members[0]->Owner);
    int index = token->Nodes.ID(this);
    return token->LoopStart != -1 && index >= token->LoopStart;
}


// void PlanningNodeClass::unknown_611440() (0x00611440)

void PlanningNodeClass::unknown_611440()
{
    int count = Members.Count();
    for (int index = 0; index < count; index++) {
        PlanningEventTargets *event = (PlanningEventTargets *)Members[index]->Event;
        if (event->unknown_0c == 2) {
            event->unknown_0c = 0x1D;
        }
    }
}


// PlanningMemberClass *PlanningNodeClass::Member_For(TechnoClass *techno) const (0x00611470)

PlanningMemberClass *PlanningNodeClass::Member_For(TechnoClass *techno) const
{
    int index = Member_Index(techno);
    if (index == -1) {
        return 0;
    }
    return Members[index];
}


// int PlanningNodeClass::Branch_Count() const (0x006114C0)

int PlanningNodeClass::Branch_Count() const
{
    return Branches.Count();
}


// bool PlanningNodeClass::Has_Branch_Members() const (0x006114D0)

bool PlanningNodeClass::Has_Branch_Members() const
{
    return Branch.Count > 0;
}


// int PlanningNodeClass::unknown_6114e0() const (0x006114E0)

int PlanningNodeClass::unknown_6114e0() const
{
    if (Branch.Count > 0 && unknown_b0 == -1) {
        return 1;
    }
    return 0;
}


// TechnoClass *PlanningNodeClass::Branch_Owner(int index) const (0x00611500)

TechnoClass *PlanningNodeClass::Branch_Owner(int index) const
{
    for (int i = 0; i < Members.Count(); i++) {
        PlanningMemberClass *member = Members[i];
        if (member->BranchIndex == index) {
            return member->Owner;
        }
    }
    return 0;
}


// TechnoClass *PlanningNodeClass::First_Counted_Owner() const (0x00611530)

TechnoClass *PlanningNodeClass::First_Counted_Owner() const
{
    for (int i = 0; i < Members.Count(); i++) {
        PlanningMemberClass *member = Members[i];
        if (member->unknown_0c) {
            return member->Owner;
        }
    }
    return 0;
}


// PlanningBranchClass *PlanningNodeClass::Branch_At(int index) const (0x00611560)

PlanningBranchClass *PlanningNodeClass::Branch_At(int index) const
{
    return Branches[index];
}


// PlanningBranchClass *PlanningNodeClass::Branch_For(TechnoClass *techno) const (0x00611570)

PlanningBranchClass *PlanningNodeClass::Branch_For(TechnoClass *techno) const
{
    int index = Member_For(techno)->BranchIndex;
    PlanningBranchClass *branch = 0;
    if (index != -1) {
        branch = Branches[index];
    }
    return branch;
}


// PlanningBranchClass *PlanningNodeClass::Own_Branch() (0x006115C0)

PlanningBranchClass *PlanningNodeClass::Own_Branch()
{
    return &Branch;
}


// CoordStruct PlanningNodeClass::Branch_Coord(int index) const (0x006115D0)

CoordStruct PlanningNodeClass::Branch_Coord(int index) const
{
    PlanningBranchClass *branch = Branches[index];
    CoordStruct coord = branch->Coord();
    return coord;
}


// CoordStruct PlanningNodeClass::Own_Branch_Coord() const (0x00611610)

CoordStruct PlanningNodeClass::Own_Branch_Coord() const
{
    CoordStruct coord = Branch.Coord();
    return coord;
}


// Point2D PlanningNodeClass::Branch_Pixel(int index) const (0x00611640)
// The screen pixel of a branch event.

Point2D PlanningNodeClass::Branch_Pixel(int index) const
{
    return Branches[index]->Pixel();
}


// Point2D PlanningNodeClass::Own_Branch_Pixel() const (0x006116B0)
// The screen pixel of the node's own branch event.

Point2D PlanningNodeClass::Own_Branch_Pixel() const
{
    return Branch.Pixel();
}


// void PlanningNodeClass::Set_Queued() (0x00611710)

void PlanningNodeClass::Set_Queued()
{
    IsQueued = true;
}


// bool PlanningNodeClass::Is_Queued() const (0x00611720)

bool PlanningNodeClass::Is_Queued() const
{
    return IsQueued;
}


// void PlanningNodeClass::unknown_611730() (0x00611730)

void PlanningNodeClass::unknown_611730()
{
    unknown_611ee0();
    int count = Members.Count();
    for (int i = 0; i < count; i++) {
        unknown_611bd0(Members[i]);
    }
    unknown_611770();
    IsQueued = false;
}


// void PlanningNodeClass::unknown_611770() (0x00611770)
// For each member whose path loops back to this node (Node_Index == the
// token's LoopStart), marks the member (+0x0C) and counts it on the node's own
// branch, which takes the event of the member's last node on the first count;
// then records in +0xB0 the branch whose event lies at the coordinate of that
// loop event (-1 if none). Names are hypotheses. The token is fetched again for
// Last_Node; the loop event is read through Get_Count/Get_Event and the branch
// event is copied into a named local before both coordinates are taken.

void PlanningNodeClass::unknown_611770()
{
    int count = Members.Count();
    for (int index = 0; index < count; index++) {
        PlanningMemberClass *member = Members[index];
        TechnoClass *owner = member->Owner;
        PlanningTokenClass *token = Planning_Token(owner);
        if (token->Node_Index(this) == token->LoopStart) {
            member->unknown_0c = true;
            if (Branch.Increment_Count() == 1) {
                Branch.Set_Event(Planning_Token(owner)->Last_Node()->Event_For(owner));
            }
        }
    }
    unknown_b0 = -1;
    if (Branch.Get_Count() != 0) {
        int branches = Branches.Count();
        EventClass event = Branch.Get_Event();
        for (int i = 0; i < branches; i++) {
            EventClass branch_event = Branches[i]->Get_Event();
            CoordStruct coord = Planning_Event_Coord(event);
            CoordStruct branch_coord = Planning_Event_Coord(branch_event);
            if (coord == branch_coord) {
                unknown_b0 = i;
                return;
            }
        }
    }
}


// void PlanningNodeClass::Dump() const (0x006118E0)
// Debug output: every branch and the loop branch with their members.

void PlanningNodeClass::Dump() const
{
    int branches = Branches.Count();
    for (int b = 0; b < branches; b++) {
        PlanningBranchClass *branch = Branches[b];
        int count = branch->Count;
        EventClass event = branch->Event;
        CoordStruct coord = Planning_Event_Coord(event);
        Debug_Printf("Branch [%d];  Member count[%d];  Origin event location[%d, %d, %d]\n", b, count, coord.X, coord.Y, coord.Z);
        int members = Members.Count();
        for (int m = 0; m < members; m++) {
            PlanningMemberClass *member = Members[m];
            if (member->BranchIndex == b) {
                TechnoClass *techno = member->Owner;
                char const *name = techno->Class_Of()->ID;
                Cell cell = techno->unknown_slot104();
                Debug_Printf("  Member [%s];  Cell location[%d, %d]\n", name, cell.X, cell.Y);
            }
        }
    }
    int loop_count = Branch.Count;
    if (loop_count > 0) {
        EventClass event = Branch.Event;
        CoordStruct coord = Planning_Event_Coord(event);
        Debug_Printf("Loop Branch [%d];  Member count[%d];  Origin event location[%d, %d, %d]\n", unknown_b0, loop_count, coord.X, coord.Y, coord.Z);
        int members = Members.Count();
        for (int m = 0; m < members; m++) {
            PlanningMemberClass *member = Members[m];
            if (member->unknown_0c) {
                TechnoClass *techno = member->Owner;
                char const *name = techno->Class_Of()->ID;
                Cell cell = techno->unknown_slot104();
                Debug_Printf("  Member [%s];  Cell location[%d, %d]\n", name, cell.X, cell.Y);
            }
        }
    }
}


// int PlanningNodeClass::unknown_611a80(TechnoClass *techno) (0x00611A80)
// The index of the branch whose event lies at the coordinate of the
// techno's event on the previous node of its path, or -1 (also -1 when the
// previous node is the path's last node). Names are hypotheses.
// Previous (0x00610C60), the token's Last_Node and Event_For (Member_For with
// Member_Index 0x00611050 called) are expanded inline; the branch coordinate
// is a named local compared with operator== (0x0061A8B0, expanded).

int PlanningNodeClass::unknown_611a80(TechnoClass *techno)
{
    PlanningNodeClass *previous = Previous(techno);
    if (previous != Planning_Token(techno)->Last_Node()) {
        EventClass event = previous->Event_For(techno);
        int count = Branches.Count();
        if (count != 0) {
            CoordStruct coord = Planning_Event_Coord(event);
            for (int index = 0; index < count; index++) {
                PlanningBranchClass *branch = Branches[index];
                if (branch->Get_Count() != 0) {
                    CoordStruct branch_coord = Planning_Event_Coord(branch->Get_Event());
                    if (coord == branch_coord) {
                        return index;
                    }
                }
            }
        }
    }
    return -1;
}


// Counts one member in its existing branch, or adds the previous node's order.

int PlanningNodeClass::unknown_611bd0(PlanningMemberClass *member)
{
    TechnoClass *owner = member->Get_Owner();
    int index = unknown_611a80(owner);
    if (index == -1) {
        PlanningNodeClass *previous = Previous(owner);
        PlanningNodeClass *last = Planning_Token(owner)->Last_Node();
        if (previous == last) {
            member->Set_BranchIndex(-1);
            return -1;
        }
        EventClass event = previous->Event_For(owner);
        PlanningBranchClass *branch = new PlanningBranchClass;
        branch->Set_Event(event);
        branch->Increment_Count();
        Branches.Add(branch);
        int count = Branches.Count() - 1;
        if (unknown_6114e0())
            unknown_611fd0();
        unknown_612b50(false);
        member->Set_BranchIndex(count);
        return count;
    } else {
        PlanningBranchClass *branch = Branches[index];
        branch->Set_unknown_74(-1);
        unknown_612b50(false);
        branch->Increment_Count();
        member->Set_BranchIndex(index);
        return index;
    }
}


// void PlanningNodeClass::unknown_611e20(PlanningMemberClass *member) (0x00611E20)
// Takes a member out of its branches: out of the loop branch (0x00611FA0),
// then drops its count from the branch it was filed under (forgetting the
// node's computed index); the last member out queues the node.

void PlanningNodeClass::unknown_611e20(PlanningMemberClass *member)
{
    if (member->unknown_0c) {
        unknown_611fa0(member);
    }
    if (member->BranchIndex != -1) {
        PlanningBranchClass *branch = Branches[member->BranchIndex];
        branch->unknown_74 = -1;
        unknown_612b50(false);
        if (--branch->Count == 0) {
            Planning_Queue_Node(this);
        }
    }
    member->BranchIndex = -1;
}


// void PlanningNodeClass::unknown_611ee0() (0x00611EE0)
// Resets the node's branches: deletes them all and takes every member out
// of its branch and the loop branch.

void PlanningNodeClass::unknown_611ee0()
{
    int branches = Branches.Count();
    for (int b = 0; b < branches; b++) {
        delete Branches[b];
    }
    Branches.Clear();
    int members = Members.Count();
    for (int m = 0; m < members; m++) {
        PlanningMemberClass *member = Members[m];
        member->BranchIndex = -1;
        member->unknown_0c = false;
    }
    Branch.Clear_Count();
    unknown_b0 = -1;
}


// bool PlanningNodeClass::Is_Loop_Start(TechnoClass *techno) (0x00611F60)
// Whether this node is where the techno's path loops back to.

bool PlanningNodeClass::Is_Loop_Start(TechnoClass *techno)
{
    PlanningTokenClass *token = Planning_Token(techno);
    return token->Nodes.ID(this) == token->LoopStart;
}


// void PlanningNodeClass::unknown_611fa0(PlanningMemberClass *member) (0x00611FA0)
// Takes a member out of the loop branch (the last one resets its index).
// Written through the member and branch accessors: the accessor form's size
// makes /Ob2 charge this body where it is expanded, as the original's
// inlining decisions in 0x00610E70 require.

void PlanningNodeClass::unknown_611fa0(PlanningMemberClass *member)
{
    member->Set_unknown_0c(false);
    Branch.Set_Count(Branch.Get_Count() - 1);
    if (Branch.Get_Count() == 0) {
        unknown_b0 = -1;
    }
}


// void PlanningNodeClass::unknown_611fd0() (0x00611FD0)
// Finds the loop branch: the branch with the same origin coordinate as the
// node's own (loop) branch, or -1.

void PlanningNodeClass::unknown_611fd0()
{
    unknown_b0 = -1;
    if (Branch.Get_Count()) {
        EventClass event = Branch.Event;
        int branches = Branches.Count();
        for (int b = 0; b < branches; b++) {
            EventClass other = Branches[b]->Event;
            CoordStruct here = Planning_Event_Coord(event);
            CoordStruct there = Planning_Event_Coord(other);
            if (here == there) {
                unknown_b0 = b;
                return;
            }
        }
    }
}


// void PlanningNodeClass::unknown_612090() (0x00612090)

void PlanningNodeClass::unknown_612090()
{
    int count = Members.Count();
    for (int i = 0; i < count; i++) {
        TechnoClass *techno = Members[i]->Owner;
        if (!techno->unknown_77) {
            techno->unknown_slot80();
        }
    }
}


// void PlanningNodeClass::unknown_6120c0() (0x006120C0)

void PlanningNodeClass::unknown_6120c0()
{
    int count = Members.Count();
    for (int i = 0; i < count; i++) {
        TechnoClass *techno = Members[i]->Owner;
        if (techno->unknown_77) {
            techno->Unselect();
        }
    }
}


// void PlanningNodeClass::Draw_Marker(int color) (0x006120F0)
// Outlines the node marker; a marker on the tactical view is listed among
// the drawn nodes.
#include "shared/surface.hpp"

void PlanningNodeClass::Draw_Marker(int color)
{
    ::Rect view = Get_Title_Screen_Area_006f1f30();
    ::Rect marker = Rect();
    unknown_surface_839964->Draw_Rect(view, marker, color);
    if (view.Is_Overlapping(marker)) {
        unknown_list_00a766d8.Add(this);
    }
}


// void PlanningNodeClass::Fill_Marker(int color) (0x00612230)
// Fills the inside of the node's marker square.

void PlanningNodeClass::Fill_Marker(int color)
{
    ::Rect view = Get_Title_Screen_Area_006f1f30();
    ::Rect marker = Rect();
    marker.X += 1;
    marker.Y += 1;
    marker.Width -= 2;
    marker.Height -= 2;
    unknown_surface_839964->Fill_Rect(view, marker, color);
}


// void PlanningNodeClass::Draw_Outer_Outline(int color) (0x00612300)
// Outlines the grown node marker three times, each one pixel further in.

void PlanningNodeClass::Draw_Outer_Outline(int color)
{
    ::Rect view = Get_Title_Screen_Area_006f1f30();
    ::Rect rect = Outer_Rect();
    for (int ring = 0; ring < 3; ring++) {
        unknown_surface_839964->Draw_Rect(view, rect, color);
        Planning_Inflate(rect, -1);
    }
}


// void PlanningNodeClass::Draw_Marker_Outline(int color) (0x00612420)
// Outlines the marker grown by 8 pixels three times, each one pixel further in.

void PlanningNodeClass::Draw_Marker_Outline(int color)
{
    ::Rect view = Get_Title_Screen_Area_006f1f30();
    ::Rect rect = Outer_Rect2();
    for (int ring = 0; ring < 3; ring++) {
        unknown_surface_839964->Draw_Rect(view, rect, color);
        Planning_Inflate(rect, -1);
    }
}


// void __fastcall Planning_Inflate(Rect &rect, int amount) (0x00612570)

void __fastcall Planning_Inflate(Rect &rect, int amount)
{
    int twice = amount * 2;
    if (twice < 0 && (twice >= rect.Width || twice >= rect.Height)) {
        rect.X = 0;
        rect.Y = 0;
        rect.Width = 0;
        rect.Height = 0;
        return;
    }
    rect.X -= amount;
    rect.Y -= amount;
    rect.Width += twice;
    rect.Height += twice;
}


// int PlanningNodeClass::Color() (0x006125D0)
// The node's screen colour (cached in unknown_b4): its first member's house
// colour, moved toward white; a node whose pixel is taken by one of its
// branches, or that starts a loop, is drawn a shade closer to white.
#include "shared/hicolor.hpp"

int PlanningNodeClass::Color()
{
    if (unknown_b4 != -1) {
        return unknown_b4;
    }
    TechnoClass *owner = Members[0]->Owner;
    unknown_b4 = Planning_Adjusted_Pixel(Planning_House_Color(owner), unknown_rgb_ab6841, 0x50);
    int branches = Branches.Count();
    for (int i = 0; i < branches; i++) {
        if (Branches[i]->unknown_74 == unknown_b4) {
            return unknown_b4;
        }
    }
    if (branches == 0) {
        PlanningTokenClass *token = Planning_Token(owner);
        if (token->Reached(token->Nodes.ID(this))) {
            RGBClass loop_color = Planning_House_Color(owner);
            loop_color.Adjust(100, unknown_rgb_ab6849);
            unknown_b4 = Build_Hicolor_Pixel(loop_color.Red, loop_color.Green, loop_color.Blue);
        }
    } else {
        RGBClass branch_color = Planning_House_Color(owner);
        branch_color.Adjust(100, unknown_rgb_ab6849);
        unknown_b4 = Build_Hicolor_Pixel(branch_color.Red, branch_color.Green, branch_color.Blue);
    }
    return unknown_b4;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int PlanningNodeClass::Branch_Color(int index) (0x006127E0)
// The colour of branch index (cached in the branch): the house colour of the
// first member counted in it that has not reached this node, moved toward
// white; once every such member has, a shade closer to white.
// from ra2-decomp 0x00635A40 (PlanningNodeClass::Branch_Color, src/planmgr.cpp)

int PlanningNodeClass::Branch_Color(int index)
{
    PlanningBranchClass *branch = Branches[index];
    if (branch->unknown_74 != -1) return branch->unknown_74;
    TechnoClass *last_owner = 0;
    int count = Members.Count();
    for (int i = 0; i < count; ++i) {
        PlanningMemberClass *member = Members[i];
        if (member->BranchIndex == index) {
            TechnoClass *owner = member->Owner;
            PlanningTokenClass *token = Planning_Token(owner);
            if (!token->Reached(token->Nodes.ID(this))) {
                RGBClass color = Planning_House_Color(owner);
                color.Adjust(0x50, unknown_rgb_ab6841);
                return branch->unknown_74 = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
            }
            last_owner = owner;
        }
    }
    if (last_owner == 0) {
        return -1;
    } else {
        RGBClass color = Planning_House_Color(last_owner);
        color.Adjust(0x64, unknown_rgb_ab6849);
        return branch->unknown_74 = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int PlanningNodeClass::Loop_Color() (0x006129B0)
// The colour of the node's own branch (cached in its unknown_74): the house
// colour of the first active member still short of the loop, moved toward
// white; once every active member is past it, a shade closer to white.
// from ra2-decomp 0x00635C10 (PlanningNodeClass::Loop_Color, src/planmgr.cpp)

int PlanningNodeClass::Loop_Color()
{
    if (Branch.unknown_74 != -1) return Branch.unknown_74;
    TechnoClass *last_owner = 0;
    int count = Members.Count();
    for (int i = 0; i < count; ++i) {
        PlanningMemberClass *member = Members[i];
        if (member->unknown_0c) {
            TechnoClass *owner = member->Owner;
            PlanningTokenClass *token = Planning_Token(owner);
            if (!token->unknown_98) {
                RGBClass color = Planning_House_Color(owner);
                color.Adjust(0x50, unknown_rgb_ab6841);
                return Branch.unknown_74 = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
            }
            last_owner = owner;
        }
    }
    if (last_owner == 0) {
        return -1;
    } else {
        RGBClass color = Planning_House_Color(last_owner);
        color.Adjust(0x64, unknown_rgb_ab6849);
        return Branch.unknown_74 = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
    }
}


// void PlanningNodeClass::unknown_612b50(bool all) (0x00612B50)
// Forgets the node's computed index (and, with all, those of its branches).

void PlanningNodeClass::unknown_612b50(bool all)
{
    unknown_b4 = -1;
    if (all) {
        int count = Branches.Count();
        for (int i = 0; i < count; i++) {
            Branches[i]->unknown_74 = -1;
        }
        Branch.unknown_74 = -1;
    }
}


// Rect PlanningNodeClass::Outer_Rect() const (0x00612B90)

::Rect PlanningNodeClass::Outer_Rect() const
{
    ::Rect rect = Rect();
    Planning_Inflate(rect, 4);
    return rect;
}


// ::Rect PlanningNodeClass::Outer_Rect2() const (0x00612C30)
// The grown node marker grown by 4 more pixels on every side.

::Rect PlanningNodeClass::Outer_Rect2() const
{
    ::Rect rect = Outer_Rect();
    Planning_Inflate(rect, 4);
    return rect;
}


// PlanningTokenClass::PlanningTokenClass(TechnoClass *owner) (0x00612CC0)

PlanningTokenClass::PlanningTokenClass(TechnoClass *owner)
    : unknown_1c(false)
{
    unknown_8c = -1;
    LoopLength = -1;
    LoopStart = -1;
    unknown_98 = false;
    unknown_99 = false;
    Owner = owner;
}


// PlanningTokenClass::~PlanningTokenClass() (0x00612D20)
// Clears the techno's token slot, unlists the token and drops its path.

PlanningTokenClass::~PlanningTokenClass()
{
    if (Owner != 0) {
        Owner->unknown_6d1420(0);
    }
    Planning_Unlist_Token(this);
    Cancel_Owner();
}


// 0x00612eb0: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_get_00_00612eb0(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00612eb0 {

unsigned int __fastcall unknown_get_00_00612eb0(unsigned int *self)
{
    return *self;
}

}  // namespace seed_00612eb0


// void PlanningTokenClass::Cancel_Owner() (0x00612EC0)
// Drops the whole path: takes the owner out of every node (nodes left empty
// delete themselves), unlists the owner and forgets the loop.

void PlanningTokenClass::Cancel_Owner()
{
    int count = Nodes.Count();
    bool none = count == 0;
    for (int i = 0; i < count; i++) {
        PlanningNodeClass *node = Nodes[i];
        node->Remove_Member(Owner);
    }
    if (!none) {
        Planning_Unlist_Techno(Owner);
    }
    Nodes.Clear();
    unknown_8c = -1;
    LoopLength = -1;
    LoopStart = -1;
    unknown_98 = false;
    unknown_99 = false;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void PlanningTokenClass::Destroy() (0x006130B0)
// From ra2-decomp (YR 0x00636310).
// Drops the path, forgets the owner and deletes the token (TechnoClass's
// destructor 0x006C20B0 calls it on the techno's token). Cancel_Owner and the
// destructor (with Planning_Unlist_Token and a second Cancel_Owner) expand inline.

void PlanningTokenClass::Destroy()
{
    Cancel_Owner();
    Owner = 0;
    delete this;
}


// 0x006132c0: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_set_1c_006132c0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006132c0 {

void __fastcall unknown_set_1c_006132c0(int self)
{
    *(unsigned char *)(self + 0x1c) = 1;
    return;
}

}  // namespace seed_006132c0


// 0x006132d0: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_clear_1c_006132d0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006132d0 {

void __fastcall unknown_clear_1c_006132d0(int self)
{
    *(unsigned char *)(self + 0x1c) = 0;
    return;
}

}  // namespace seed_006132d0


// 0x006132e0: function not yet identified
//
// Body: 4 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 __fastcall unknown_get_1c_006132e0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006132e0 {

unsigned char __fastcall unknown_get_1c_006132e0(int self)
{
    return *(unsigned char *)(self + 0x1c);
}

}  // namespace seed_006132e0


// 0x006132f0: function not yet identified
//
// Body: 32 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_is_kind_1c_006132f0(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006132f0 {

unsigned int __fastcall unknown_is_kind_1c_006132f0(int *self)
{
    if ((*(int *)(*self + 0x98) != 0x1c) && (*(int *)(*self + 0xa0) != 0x1c)) {
        return 0;
    }
    return 1;
}

}  // namespace seed_006132f0


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool PlanningTokenClass::unknown_613310() const (0x00613310)
// Adopted from ra2-decomp PlanningTokenClass::Has_Started (YR 0x00636570).
// Whether the path has nodes and its first-node index is -1 or above 0.

bool PlanningTokenClass::unknown_613310() const
{
    return Nodes.Count() > 0 && (unknown_8c == -1 || unknown_8c > 0);
}


// EventClass PlanningTokenClass::unknown_613330() (0x00613330)
// Consumes the first planned event: the first node leaves the path and either
// rejoins it at the end (a loop at its start) or loses the owner, then the
// first-node index steps down and an emptied path unlists the owner.

template<class T> inline T Max(T a, T b) { return a > b ? a : b; }

// The owner's event at a node, kept as the token's current event.
static inline EventClass Take_Event(PlanningNodeClass *node, TechnoClass *owner, EventClass &current)
{
    EventClass event = node->Event_For(owner);
    current = event;
    return event;
}

// Steps the first-node index down and unlists an owner with no path left.
static inline void Finish_Step(PlanningTokenClass *token)
{
    token->unknown_8c = Max(-1, token->unknown_8c - 1);
    if (token->Nodes.Count() == 0) {
        Planning_Unlist_Techno(token->Owner);
    }
}

EventClass PlanningTokenClass::unknown_613330()
{
    PlanningNodeClass *node = Nodes[0];
    EventClass event = Take_Event(node, Owner, Event);
    Nodes.Delete(0);
    if (LoopStart == 0) {
        unknown_99 = true;
        Nodes.Add(node);
        Planning_Queue_Node(node);
    } else {
        node->Remove_Member(Owner);
        if (LoopStart > 0) {
            --LoopStart;
            LoopStart = Max(Get_LoopStart(), 0);
        }
    }
    Finish_Step(this);
    return event;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void PlanningTokenClass::Copy_Nodes_To(TechnoClass *techno, int start, int count) (0x00613550)
// Adds the nodes start..start+count-1 of this path to techno's path: each
// node gets a member for techno with a copy of this owner's order there,
// aimed at techno.
// Shape from ra2-decomp 0x006367B0 (PlanningTokenClass::Share_Range, src/planmgr.cpp).

void PlanningTokenClass::Copy_Nodes_To(TechnoClass *techno, int start, int count)
{
    if (Planning_Token(techno) == 0) {
        return;
    }
    for (int i = start; i < start + count; i++) {
        PlanningNodeClass *node = Nodes[i];
        EventClass event = node->Event_For(Owner);
        event.TargetAt(0x07) = TargetClass(techno);
        node->Add_Member(techno, event);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void PlanningTokenClass::Copy_Nodes_To(TechnoClass *techno, int start) (0x00613720)
// Copy_Nodes_To(techno, start, Nodes.Count() - start): adds the nodes from
// start of this path to techno's path.
// Shape from ra2-decomp 0x00636980 (PlanningTokenClass::Share_From, src/planmgr.cpp).

void PlanningTokenClass::Copy_Nodes_To(TechnoClass *techno, int start)
{
    int count = Nodes.Count() - start;
    Copy_Nodes_To(techno, start, count);
}


// void PlanningTokenClass::unknown_6138f0(int index) (0x006138F0)
// Takes the node at index out of the path: the first-node index (+0x8C) and
// the loop (LoopStart / LoopLength; a loop shorter than three nodes is
// dropped) are moved down, the owner leaves the node (Remove_Member, expanded
// inline; the node is destroyed when it has no members left) and the entry is
// deleted. Name hypothesis.

void PlanningTokenClass::unknown_6138f0(int index)
{
    if (Nodes.Count() == 1) {
        unknown_8c = -1;
    } else if (unknown_8c > 0 && index < unknown_8c) {
        unknown_8c--;
    }
    if (LoopStart >= 0) {
        if (index < LoopStart) {
            LoopStart--;
        } else if (--LoopLength < 3) {
            LoopStart = -1;
            LoopLength = 0;
        }
    }
    PlanningNodeClass *node = Nodes[index];
    node->Remove_Member(Owner);
    Nodes.Delete(index);
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void PlanningTokenClass::unknown_613a80() (0x00613A80)
// Clears the loop marks of the loop start (once) and of every node from the
// first one on. Statement shape from ra2-decomp PlanningTokenClass::FUN_00636CE0
// (YR 0x00636CE0).

void PlanningTokenClass::unknown_613a80()
{
    if (unknown_98) {
        PlanningNodeClass *node = Loop_Node();
        if (node != 0) node->unknown_612b50(true);
        unknown_98 = false;
    }
    int start = unknown_8c == -1 ? 0 : unknown_8c;
    int count = Nodes.Count();
    for (int index = start; index < count; index++) {
        PlanningNodeClass *node = Nodes[index];
        if (node != 0) node->unknown_612b50(true);
    }
    unknown_8c = -1;
}


// 0x00613b30: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_get_8c_00613b30(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613b30 {

unsigned int __fastcall unknown_get_8c_00613b30(int self)
{
    return *(unsigned int *)(self + 0x8c);
}

}  // namespace seed_00613b30


// bool PlanningTokenClass::Reached(int slot) const (0x00613B40)
// Whether the node at slot is at or past the first node of the path.

bool PlanningTokenClass::Reached(int slot) const
{
    if (unknown_8c == -1) {
        return false;
    }
    return slot >= unknown_8c;
}


class Leaf_613b60 { public: unsigned int get(); };
unsigned int Leaf_613b60::get() { return *(unsigned int*)((char*)this + 0x14); }


// 0x00613b70: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: int __fastcall unknown_get_8c_or_14_00613b70(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613b70 {

int __fastcall unknown_get_8c_or_14_00613b70(int self)
{
    int value;

    value = (int)(*(int *)(self + 0x8c));
    if (value == -1) {
        value = (int)(*(int *)(self + 0x14));
    }
    return value;
}

}  // namespace seed_00613b70


// 0x00613b80: function not yet identified
//
// Body: 20 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: int __fastcall unknown_span_8c_14_00613b80(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613b80 {

int __fastcall unknown_span_8c_14_00613b80(int self)
{
    if (*(int *)(self + 0x8c) == -1) {
        return 0;
    }
    return *(int *)(self + 0x14) - *(int *)(self + 0x8c);
}

}  // namespace seed_00613b80


// void PlanningTokenClass::Add_Node(PlanningNodeClass *node) (0x00613BA0)
// Appends a node to the path; the first one added is where the path starts.

void PlanningTokenClass::Add_Node(PlanningNodeClass *node)
{
    Nodes.Add(node);
    if (unknown_8c == -1) {
        unknown_8c = Nodes.Count() - 1;
    }
}


// PlanningNodeClass *PlanningTokenClass::Node_At(int index) const (0x00613C00)
// The node at index, wrapping into the loop past the end; 0 outside.

PlanningNodeClass *PlanningTokenClass::Node_At(int index) const
{
    int count = Nodes.Count();
    if (!count) {
        return 0;
    }
    if (index >= count) {
        if (LoopStart <= 0 && LoopStart != 0) {
            return 0;
        }
        index = (index - LoopStart) % LoopLength + LoopStart;
    }
    if (index < 0 || index >= count) {
        return 0;
    }
    return Nodes[index];
}


// PlanningNodeClass *PlanningTokenClass::Last_Node() const (0x00613C50)
// Defined inline in shared/planning.hpp; the out-of-line copy is emitted by the unit's refused
// site in Planning_Add_Order 0x00614EC0.


// bool PlanningTokenClass::Has_Node(PlanningNodeClass *node) (0x00613C60)

bool PlanningTokenClass::Has_Node(PlanningNodeClass *node)
{
    return Nodes.ID(node) != -1;
}


// void PlanningTokenClass::unknown_613c80(PlanningNodeClass *node) (0x00613C80)

void PlanningTokenClass::unknown_613c80(PlanningNodeClass *node)
{
    Nodes.ID(node);
}


// EventClass PlanningTokenClass::Event_At(int index) (0x00613CA0)
// The event of the token's techno in the node at index.

EventClass PlanningTokenClass::Event_At(int index)
{
    PlanningNodeClass *node = Nodes[index];
    return node->Event_For(Owner);
}


// EventClass PlanningTokenClass::Get_Event() const (0x00613D00)

EventClass PlanningTokenClass::Get_Event() const
{
    return Event;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// PlanningNodeClass *PlanningTokenClass::Loop_Node() (0x00613D20)
// Adopted from ra2-decomp PlanningTokenClass::Loop_Node (YR 0x00636F80).
// The node the path loops back to, or 0 without a loop.

PlanningNodeClass *PlanningTokenClass::Loop_Node()
{
    if (LoopStart == -1) {
        return 0;
    }
    return Nodes[LoopStart];
}


// 0x00613d40: function not yet identified
//
// Body: 30 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall unknown_set_90_94_00613d40(int param_1,undefined4 param_2,undefined4 param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613d40 {

struct Owner_00613d40 { void unknown_set_90_94_00613d40(unsigned int first, unsigned int second); };

void Owner_00613d40::unknown_set_90_94_00613d40(unsigned int first, unsigned int second)
{
    int self = (int)this;
    *(unsigned int *)(self + 0x90) = first;
    *(unsigned int *)(self + 0x94) = second;
    *(unsigned char *)(self + 0x98) = 1;
    return;
}

}  // namespace seed_00613d40


// 0x00613d60: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_get_90_00613d60(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613d60 {

unsigned int __fastcall unknown_get_90_00613d60(int self)
{
    return *(unsigned int *)(self + 0x90);
}

}  // namespace seed_00613d60


// 0x00613d70: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_get_94_00613d70(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613d70 {

unsigned int __fastcall unknown_get_94_00613d70(int self)
{
    return *(unsigned int *)(self + 0x94);
}

}  // namespace seed_00613d70


// int PlanningTokenClass::unknown_613d80() const (0x00613D80)
// False while the loop start is negative (no loop).

int PlanningTokenClass::unknown_613d80() const
{
    if (LoopStart <= 0 && LoopStart != 0) {
        return 0;
    }
    return 1;
}


// 0x00613da0: function not yet identified
//
// Body: 14 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __fastcall unknown_has_94_00613da0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613da0 {

bool __fastcall unknown_has_94_00613da0(int self)
{
    return 0 < *(int *)(self + 0x94);
}

}  // namespace seed_00613da0


// 0x00613db0: function not yet identified
//
// Body: 14 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __fastcall unknown_no_94_00613db0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613db0 {

bool __fastcall unknown_no_94_00613db0(int self)
{
    return *(int *)(self + 0x94) == 0;
}

}  // namespace seed_00613db0


// 0x00613dc0: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 __fastcall unknown_get_98_00613dc0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613dc0 {

unsigned char __fastcall unknown_get_98_00613dc0(int self)
{
    return *(unsigned char *)(self + 0x98);
}

}  // namespace seed_00613dc0


// 0x00613dd0: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 __fastcall unknown_get_99_00613dd0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00613dd0 {

unsigned char __fastcall unknown_get_99_00613dd0(int self)
{
    return *(unsigned char *)(self + 0x99);
}

}  // namespace seed_00613dd0


// void PlanningTokenClass::unknown_613de0() (0x00613DE0)
// Steps the loop start back one, never below 0.

void PlanningTokenClass::unknown_613de0()
{
    LoopStart = LoopStart - 1;
    LoopStart = LoopStart > 0 ? LoopStart : 0;
}


class Leaf_613e00 { public: void get(); };
void Leaf_613e00::get() { unsigned int self = (unsigned int)this; unsigned int value; unsigned int spare; *(unsigned int*)(0xa76680) = self; }


// void Planning_Reset() (0x00613E10)
// Clears the planning mode's flags and remembered nodes.

void Planning_Reset()
{
    unknown_flag_a767a4 = false;
    unknown_flag_a767a5 = false;
    unknown_node_a7678c = 0;
    unknown_node_a766f8 = 0;
    unknown_node_a766b0 = 0;
    unknown_point_a76728 = Point2D(0, 0);
    unknown_flag_a76770 = false;
}


// void Unknown_00613E40() (0x00613E40)
// A Prog_End shutdown step of the planning mode: forwards to 0x00613E50 (a tail jump).

void Unknown_00613E50();

void Unknown_00613E40()
{
    Unknown_00613E50();
}


// void Unknown_00613E50() (0x00613E50; Planning_Shutdown 0x00613E40's tail jump)
// Tears the planning mode down: deletes every planning token (the lists are
// marked as being torn down so the tokens do not unlist themselves), empties
// the planning lists and counters and forgets the remembered state.
#include "shared/crt_memory.hpp"

// The node lists, emptied together (expanded inline).
static inline void Clear_Node_Lists()
{
    PlanningNodeClass::unknown_list_00a76628.Clear();
    PlanningNodeClass::unknown_list_00a766d8.Clear();
}

void Unknown_00613E50()
{
    int count = PlanningTokenClass::unknown_list_00a76738.Count();
    unknown_flag_a767a5 = true;
    unknown_flag_a767a4 = false;
    unknown_flag_a767a6 = false;
    for (int i = 0; i < count; i++) {
        PlanningTokenClass *token = PlanningTokenClass::unknown_list_00a76738[i];
        delete token;
    }
    memset(unknown_counts_a76684, 0, sizeof(unknown_counts_a76684));
    PlanningLists::unknown_list_00a76700.Clear();
    PlanningTokenClass::unknown_list_00a76738.Clear();
    Clear_Node_Lists();
    unknown_node_a7678c = 0;
    unknown_flag_a76790 = false;
    unknown_flag_a76788 = false;
    unknown_a766fc = 0;
    unknown_flag_a766c8 = false;
    unknown_flag_a767a5 = false;
    unknown_a76750 = -1;
    memset(unknown_counts_a766b4, 0, 5 * sizeof(int));
}


// void __fastcall Unknown_00614010() (0x00614010)
// Per-frame planning update, called from the main loop: refreshes the hover,
// drops path nodes whose attack or move target has died, settles the queued
// nodes and shows any pending command refusal.

void __fastcall Planning_Hover();   // 0x00615C00

// The two wrappers group real statements. Each moves its statements out of
// the root: the first keeps Planning_Hover a call, the second refuses
// Planning_Refuse at the last refusal arm.
inline void Planning_Update_Hover_00614010()
{
    Planning_Hover();
    if (unknown_node_a766b0 && unknown_objects_a40c60.Count() > 0) {
        unknown_node_a766b0 = 0;
    }
}

inline void Planning_Update_Reset_Flags_00614010()
{
    unknown_flag_a76790 = false;
    unknown_flag_a76788 = false;
    unknown_flag_a766c8 = false;
}

void __fastcall Unknown_00614010()
{
    Planning_Update_Hover_00614010();
    int count = PlanningTokenClass::unknown_list_00a76738.Count();
    for (int index = 0; index < count; ++index) {
        PlanningTokenClass *token = PlanningTokenClass::unknown_list_00a76738[index];
        TechnoClass *owner = token->Owner;
        for (int path_index = token->Nodes.Count() - 1; path_index >= 0; --path_index) {
            PlanningNodeClass *event_node = token->Nodes[path_index];
            EventClass event = event_node->Event_For(token->Owner);
            bool remove = true;
            if (event.Type == 4) {
                if (event.TargetAt(0x0E).Is_Valid()) {
                    remove = event.TargetAt(0x0E).As_Abstract() == 0;
                } else if (event.TargetAt(0x13).Is_Valid()) {
                    remove = event.TargetAt(0x13).As_Abstract() == 0;
                }
            }
            if (!remove) {
                continue;
            }
            PlanningNodeClass *node = token->Node_At(path_index);
            PlanningNodeClass *next_node = node->Next(owner);
            Planning_Queue_Node(next_node);
            token->unknown_6138f0(path_index);
        }
    }
    Planning_Process_Queue();
    int refusal = unknown_a76750;
    if (refusal != -1) {
        Planning_Refuse_Command(refusal);
        unknown_a76750 = -1;
    }
    Planning_Update_Reset_Flags_00614010();
}


// void __fastcall Planning_Store_Event(EventClass const &event) (0x00614350)
// Lists a copy of an event among the planned events (0x00A76648); a copied
// mission event (type 4) is marked 2 at +0x1D. Name hypothesis; no callers.

void __fastcall Planning_Store_Event(EventClass const &event)
{
    EventClass *copy = new EventClass;
    *copy = event;
    if (copy->Type == 4) {
        ((PlanningEventTargets *)copy)->unknown_1d = 2;
    }
    PlanningLists::unknown_list_00a76648.Add(copy);
}


// void __fastcall Planning_Unlist_Node(PlanningNodeClass *node) (0x006143E0)
// Takes a node out of the three planning lists and forgets it if the
// planning mode was remembering it.

void __fastcall Planning_Unlist_Node(PlanningNodeClass *node)
{
    PlanningNodeClass::unknown_list_00a76758.Delete(node);
    PlanningNodeClass::unknown_list_00a76628.Delete(node);
    PlanningNodeClass::unknown_list_00a766d8.Delete(node);
    if (unknown_node_a7678c == node) {
        unknown_node_a7678c = 0;
    }
    if (unknown_node_a766f8 == node) {
        unknown_node_a766f8 = 0;
    }
    if (unknown_node_a766b0 == node) {
        unknown_node_a766b0 = 0;
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Planning_Unlist_Token(PlanningTokenClass *token) (0x006144F0)
// From ra2-decomp (YR 0x00637750).
// Removes a token from the static list unknown_list_00a76738 unless the
// planning lists are being torn down.

void __fastcall Planning_Unlist_Token(PlanningTokenClass *token)
{
    if (!unknown_flag_a767a5) {
        PlanningTokenClass::unknown_list_00a76738.Delete(token);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Planning_Unlist_Techno(TechnoClass *techno) (0x00614550)
// From ra2-decomp (YR 0x006377B0).
// Takes a techno out of the planning list, lowering its house's counter.

void __fastcall Planning_Unlist_Techno(TechnoClass *techno)
{
    int house = Planning_House(techno);
    Planning_Uncount(house);
    PlanningLists::unknown_list_00a76700.Delete(techno);
}


// void TechnoClass::unknown_6145e0() (0x006145E0)
// With planning active: tells every member of the last node on the techno's
// planned path to stop (their slot 80), unless they are already finished.
// The flag keeps it from re-entering.

void TechnoClass::unknown_6145e0()
{
    if (!unknown_flag_a767a6) {
        unknown_flag_a767a6 = true;
        if (unknown_flag_a767a4) {
            PlanningTokenClass *token = Planning_Token(this);
            if (token) {
                PlanningNodeClass *last = token->Last_Node();
                if (last) {
                    last->unknown_612090();
                }
            }
        }
        unknown_flag_a767a6 = false;
    }
}


// void __fastcall Planning_Queue_Node(PlanningNodeClass *node) (0x00614650)
// Marks a node as queued and appends it once to unknown_list_00a76758.

void __fastcall Planning_Queue_Node(PlanningNodeClass *node)
{
    if (node != 0 && !node->IsQueued) {
        node->IsQueued = true;
        PlanningNodeClass::unknown_list_00a76758.Add(node);
    }
}


// void Planning_Process_Queue() (0x006146C0)
// Settles every queued node (rebuilds its branches) and empties the queue.

void Planning_Process_Queue()
{
    DynamicVectorClass<PlanningNodeClass *> &queue = PlanningNodeClass::unknown_list_00a76758;
    int count = queue.Count();
    for (int i = 0; i < count; i++) {
        queue[i]->unknown_611730();
    }
    queue.DynamicVectorClass<PlanningNodeClass *>::Clear();
}


// void Planning_Begin() (0x00614760)
// Starts planning mode: sets the flag, plays the start sound and notifies the sink.

void Planning_Begin()
{
    if (!unknown_flag_a767a4) {
        unknown_flag_a767a4 = true;
        Voc_Play(Rules->unknown_1cc, 1.0f, 0x2000, 0);
        if (unknown_sink_a76680) {
            unknown_sink_a76680->Unknown_Slot0(0x11C7, 0);
        }
    }
}


// void Planning_End() (0x006147B0)
// Ends planning mode: sends the release event, forgets the remembered nodes,
// plays the end sound and notifies the sink.

void Planning_End()
{
    if (unknown_flag_a767a4) {
        EventClass event(unknown_player_a35db4->ArrayIndex, 0x2B);
        Unknown_0062E670(event);
        unknown_node_a7678c = 0;
        unknown_node_a766f8 = 0;
        unknown_node_a766b0 = 0;
        unknown_flag_a767a4 = false;
        Voc_Play(Rules->unknown_1d8, 1.0f, 0x2000, 0);
        if (unknown_sink_a76680) {
            unknown_sink_a76680->Unknown_Slot0(0x11C8, 0);
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Commit_Plans(int house) (0x00614850)
// Adopted from ra2-decomp Commit_Plans (YR 0x00637AB0).
// For every planning techno of a house: clears its path's loop marks, and
// when its path was not under way but its mission allows it, starts the
// path again. Planning_Execute (0x00614BA0) expands it inline for event 0x2B.

static inline void Commit_Unit_Orders(TechnoClass *techno)
{
    PlanningTokenClass *token = Planning_Token(techno);
    if (token != 0) {
        bool started = token->unknown_613310();
        token->unknown_613a80();
        bool permitted = Planning_Mission_Allows(techno);
        if (!started && permitted) {
            techno->unknown_615360();
        }
    }
}

void __fastcall Commit_Plans(int house)
{
    int count = unknown_list_a40c20.Count();
    for (int i = 0; i < count; i++) {
        TechnoClass *techno = unknown_list_a40c20[i];
        if (house == Planning_House(techno)) {
            Commit_Unit_Orders(techno);
        }
    }
}


// void PlanningNodeClass::unknown_614aa0() (0x00614AA0)
// Sends the player's type 0x2C event about the object the node's first
// order points at.

void PlanningNodeClass::unknown_614aa0()
{
    EventClass event = *Members[0]->Event;
    PlanningEventTargets const *view = (PlanningEventTargets const *)&event;
    TargetClass const *reference;
    if (view->Target_0e.Kind) {
        reference = &view->Target_0e;
    } else if (view->Target_18.Kind) {
        reference = &view->Target_18;
    } else if (view->Target_13.Kind) {
        reference = &view->Target_13;
    } else {
        return;
    }
    AbstractClass *object = reference->As_Abstract();
    if (object) {
        EventClass send(unknown_player_a35db4->ArrayIndex, 0x2C, TargetClass(object));
        Unknown_0062E670(send);
    }
}


// unsigned char __fastcall Infantry_Action_Allowed_00614b50(int action) (0x00614B50): an
// infantry action is allowed unless planning mode is on (0x00A767A4) and the action is 4
// (InfantryClass 0x00505390 and UnitClass slot 28 0x00703BD0 ask it).

unsigned char __fastcall Infantry_Action_Allowed_00614b50(int action)
{
    if (unknown_flag_a767a4 && action == 4) {
        return false;
    }
    return true;
}


// void __stdcall Planning_Send_Event(EventClass event) (0x00614B70)
// A mission event (type 4) is marked as planned and queued for sending.

void __stdcall Planning_Send_Event(EventClass event)
{
    if (event.Type == 4) {
        ((PlanningEventTargets *)&event)->unknown_1d = 1;
        Unknown_0062E670(event);
    }
}


// Not exact (RA2_NON_MATCHING): Planning_Execute at 0x00614BA0, best score 78.17, 2026-11-13; differs: 275 instructions in the original, 316 in the draft (shape 90.36)
#ifdef RA2_NON_MATCHING
// void __fastcall Planning_Execute(EventClass *event) (0x00614BA0)
// Carries out a planning event: 0x2B restarts the paths of the event's house,
// 0x2A adds the node at the 0x0C target's coordinate to the 0x07 techno's path,
// 0x2C takes the node at the 0x07 target's coordinate out of every path
// (remembering the next node for the player); any other event is an order for
// the 0x07 techno.

// For every planning techno of a house: clears its path's loop marks, and when
// its path was not under way but its mission allows it, starts the path again.
// The same steps as Commit_Plans (0x00614850), written without its
// Commit_Unit_Orders helper: this arm expands Planning_Token, Planning_House
// and the token test inline but calls the four steps.
inline void __fastcall Planning_Restart_House(int house)
{
    int count = unknown_list_a40c20.Count();
    for (int i = 0; i < count; i++) {
        TechnoClass *techno = unknown_list_a40c20[i];
        if (house == Planning_House(techno)) {
            PlanningTokenClass *token = Planning_Token(techno);
            if (token) {
                bool started = token->unknown_613310();
                token->unknown_613a80();
                bool allows = Planning_Mission_Allows(techno);
                if (!started && allows) {
                    techno->unknown_615360();
                }
            }
        }
    }
}

static inline PlanningNodeClass *Planning_Next_On_Path(PlanningNodeClass *node)
{
    TechnoClass *owner = node->Members[0]->Owner;
    PlanningTokenClass *token = Planning_Token(owner);
    int index = Token_Node_Index(token, node);
    PlanningTokenClass *path = Planning_Token(owner);
    PlanningNodeClass *next = path->Node_At(index + 1);
    return next;
}

void __fastcall Planning_Execute(EventClass *event)
{
    if (event->Type == 0x2B) {
        Planning_Restart_House(event->House);
    } else if (event->Type == 0x2A) {
        TechnoClass *techno = event->TargetAt(0x07).As_Techno();
        if (techno) {
            AbstractClass *object = event->TargetAt(0x0C).As_Abstract();
            if (object) {
                CoordStruct coord = object->unknown_slot22();
                PlanningNodeClass *node = Planning_Find_Node(event->House, -1, coord);
                if (node) {
                    Planning_Path_Add(techno, node);
                }
            }
        }
    } else if (event->Type == 0x2C) {
        AbstractClass *object = event->TargetAt(0x07).As_Abstract();
        if (object) {
            CoordStruct coord = object->unknown_slot22();
            int house = event->House;
            PlanningNodeClass *node = Planning_Find_Node(house, -1, coord);
            if (node) {
                PlanningNodeClass *next = Planning_Next_On_Path(node);
                Planning_Remove_Node(node);
                if (next && house == unknown_player_a35db4->ArrayIndex) {
                    unknown_node_a766b0 = next;
                }
            }
        }
    } else {
        TechnoClass *techno = event->TargetAt(0x07).As_Techno();
        Planning_Add_Order(techno, event);
    }
}
#endif

// bool __fastcall Planning_Add_Order(TechnoClass *techno, EventClass *event) (0x00614EC0)
// Plans an order for a techno: adds it to the node at the order's coordinate,
// or to a new node, unless the order or the techno's path refuses it.

// Raises a house's counter (0x00616D00).
inline void __fastcall Planning_Recount(int house)
{
    if (Planning_Valid_House(house)) {
        unknown_counts_a76684[house]++;
    }
}

// The path has closed a loop.
inline bool Planning_Token_Looped(PlanningTokenClass *token)
{
    return token->Loop_Past_Start() || token->Loop_At_Start();
}

// Puts the techno on the node: with the members of its path's last node, or alone.
inline void __fastcall Planning_Attach_Order(PlanningNodeClass *const &node, PlanningTokenClass *token, TechnoClass *techno, EventClass *event)
{
    PlanningNodeClass *last = token->Last_Node();
    if (last) {
        Planning_Copy_Members(node, last, *event);
    } else {
        node->Add_Member(techno, *event);
    }
}

bool __fastcall Planning_Add_Order(TechnoClass *techno, EventClass *event)
{
    if (!techno) {
        return false;
    }
    bool player = event->House == unknown_player_a35db4->ArrayIndex;
    CoordStruct const &coord = Planning_Event_Coord(*event);
    if (coord == CoordStruct(-1, -1, -1)) {
        return false;
    }
    int id = event->Frame;
    PlanningNodeClass *node = Planning_Find_Node(event->House, id, coord);
    if (node && node->Has_Member(techno)) {
        return true;
    }
    int kind = Planning_Order_Kind(techno, (PlanningEventTargets const *)event);
    if (kind != -1) {
        if (!player) {
            return false;
        }
        if (kind == 0) {
            Planning_Refuse_Order(event);
        } else if (kind == 1) {
            Planning_Refuse("MSG:PostTerminatingCommand");
        } else if (kind == 2) {
            Planning_Refuse("MSG:PostContinualCommand");
        }
        return false;
    }
    PlanningTokenClass *token = Planning_Token(techno);
    if (!token) {
        token = Planning_New_Token(techno);
    }
    if (player) {
        int result = Planning_Event_Result(token, (PlanningEventTargets const *)event);
        if (result != -1) {
            Planning_Set_a76750(result);
        }
    }
    if (Planning_Token_Looped(token)) {
        if (player) {
            Planning_Refuse("MSG:PostLoop");
        }
        return false;
    }
    if (token->Get_Node_Count() == 0) {
        PlanningLists::unknown_list_00a76700.Add(techno);
        Planning_Recount(event->House);
    } else if (token->Get_Node_Count() == 16) {
        if (player) {
            Planning_Refuse("MSG:NodeMaximum");
        }
        return false;
    }
    if (!node) {
        node = new PlanningNodeClass(event->Frame);
        PlanningNodeClass::unknown_list_00a76628.Add(node);
    }
    Planning_Attach_Order(node, token, techno, event);
    if (player) {
        Voc_Play(Rules->unknown_1d0, 1.0f, 0x2000, 0);
    }
    return true;
}


// bool TechnoClass::unknown_615360() (0x00615360, PlanMgr.cpp)
// Starts carrying out the techno's planned path once: when its planning
// token has nodes, a first-node index of -1 or above 0 and is not started
// yet, queues mission 0x1C, lists the token's 0x00613330 event
// (Planning_Store_Event) and marks the token started.

bool TechnoClass::unknown_615360()
{
    PlanningTokenClass *token = Planning_Token(this);
    if (token != 0 && token->Nodes.Count() > 0 && (token->unknown_8c == -1 || token->unknown_8c > 0) && !token->unknown_1c) {
        QueueMission(0x1C, true);
        EventClass event = token->unknown_613330();
        Planning_Store_Event(event);
        token->unknown_1c = true;
        return true;
    }
    return false;
}


// void __fastcall Planning_Techno_00615480(TechnoClass *techno, EventClass *event) (0x00615480)
// Unless the event was planned, takes the techno out of its whole path (0x006155E0),
// then clears the path's +0x1C mark. Name hypothesis kept: EventClass::Execute binds it.

void __fastcall Planning_Techno_00615480(TechnoClass *techno, EventClass *event)
{
    if (techno != 0) {
        PlanningTokenClass *token = Planning_Token(techno);
        if (token != 0) {
            int planned = event != 0 ? (char)event->ByteAt(0x1D) : 0;
            if (!planned && token->Get_Node_Count() > 0) {
                techno->unknown_6155e0();
            }
            token->Clear_1c();
        }
    }
}


// void TechnoClass::unknown_6155e0() (0x006155E0)
// Drops the techno's planned path: its token's members leave every node and
// the token is cleared (PlanningTokenClass::Cancel_Owner, expanded inline
// with Remove_Member and Planning_Unlist_Techno).

void TechnoClass::unknown_6155e0()
{
    PlanningTokenClass *token = Planning_Token(this);
    if (token != 0) {
        token->Cancel_Owner();
    }
}


// PlanningTokenClass *__fastcall Planning_New_Token(TechnoClass *techno) (0x00615820)
// Makes a techno's planning token, lists it and hands it to the techno.

PlanningTokenClass *__fastcall Planning_New_Token(TechnoClass *techno)
{
    PlanningTokenClass *token = new PlanningTokenClass(techno);
    PlanningTokenClass::unknown_list_00a76738.Add(token);
    techno->unknown_6d1420((UnknownTechno408 *)token);
    return token;
}


// 0x006158f0: function not yet identified
//
// Body: 24 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_is_kind_6_7_9_006158f0(char param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006158f0 {

unsigned int __fastcall unknown_is_kind_6_7_9_006158f0(char kind)
{
    if (((kind != '\t') && (kind != '\x06')) && (kind != '\a')) {
        return 0;
    }
    return 1;
}

}  // namespace seed_006158f0


// bool __fastcall Planning_Order_Stays_Put(TechnoClass *techno, int mission, TargetClass target) (0x00615910)
// Whether a mission 11 order leaves the techno where it is: no target, or a
// target cell (a cell, or the cell of a techno) that is the techno's own
// slot 104 cell. Name hypothesis.
#include "shared/cell.hpp"

bool __fastcall Planning_Order_Stays_Put(TechnoClass *techno, int mission, TargetClass target)
{
    if (mission == 11) {
        if (!target.Kind) {
            return true;
        }
        if (techno != 0) {
            Cell here = techno->unknown_slot104();
            Cell cell = techno->unknown_slot104();
            CellClass *target_cell = target.As_Cell();
            if (target_cell != 0) {
                cell = target_cell->MapCoords;
            } else {
                TechnoClass *target_techno = target.As_Techno();
                if (target_techno != 0) {
                    cell = target_techno->unknown_slot104();
                }
            }
            if (here == cell) {
                return true;
            }
        }
    }
    return false;
}


// 0x006159c0: function not yet identified
//
// Body: 24 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_is_kind_7_8_9_006159c0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006159c0 {

unsigned int __fastcall unknown_is_kind_7_8_9_006159c0(int kind)
{
    if (((kind != 7) && (kind != 8)) && (kind != 9)) {
        return 0;
    }
    return 1;
}

}  // namespace seed_006159c0


// bool __fastcall Planning_Cell_Mission(int mission, TargetClass target) (0x006159E0)
// Defined inline in shared/planning.hpp; the out-of-line copy is emitted by the unit's refused
// sites (Planning_Add_Order 0x00614EC0, through Planning_Event_Result).


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool __fastcall Planning_Mission_Allows(TechnoClass *techno) (0x00615A10)
// Whether the techno's current mission lets planning mode take over: mission
// 5, or mission 1 / 0xB whose target is a building (kind 0xB) or, for 0xB,
// absent. Statement shape from ra2-decomp FUN_00638C70 (YR 0x00638C70).

static inline bool Mission_Fits(int mission, TargetClass target)
{
    if (mission == 1 || mission == 0x0B) {
        if (target.Kind == 0x0B) goto fits;
        if (mission == 0x0B && target.Kind == 0) {
fits:
            return true;
        }
    }
    return false;
}

bool __fastcall Planning_Mission_Allows(TechnoClass *techno)
{
    if (techno) {
        int mission = techno->Get_Mission();
        if (mission == 5) {
            return true;
        }
        return Mission_Fits(mission, TargetClass(techno->unknown_228));
    }
    return false;
}


// int __fastcall Planning_Order_Kind(TechnoClass *techno, PlanningEventTargets const *event) (0x00615A80)
// How an order relates to the techno's planned path: 0 for event types 6, 7
// and 9 or an order that keeps the techno in place; when the path's last node
// holds a terminating order for the techno (missions 7-9) 1, a move/attack
// order onto a cell or (mission 11) nowhere 2; else -1. Name hypothesis.

int __fastcall Planning_Order_Kind(TechnoClass *techno, PlanningEventTargets const *event)
{
    if (Planning_Stop_Type(event->Type)) {
        return 0;
    }
    int order_mission = event->Mission();
    TargetClass const &target = event->Target();
    if (Planning_Order_Stays_Put(techno, order_mission, target)) {
        return 0;
    }
    PlanningTokenClass *token = Planning_Token(techno);
    if (token != 0 && token->Get_Node_Count() > 0) {
        EventClass last_event = token->Event_At(token->Get_Node_Count() - 1);
                PlanningEventTargets const *targets = (PlanningEventTargets const *)&last_event;
        int mission = targets->Mission();
        if (Planning_Terminal_Mission(mission)) {
            return 1;
        }
        if (Planning_Cell_Mission(mission, targets->Target())) {
            return 2;
        }
    }
    return -1;
}


// void Planning_Clear_Drawn() (0x00615BC0)
// Empties the list of nodes drawn on screen (unknown_list_00a766d8).

void Planning_Clear_Drawn()
{
    PlanningNodeClass::unknown_list_00a766d8.DynamicVectorClass<PlanningNodeClass *>::Clear();
}


// void __fastcall Planning_Hover() (0x00615C00)
// The node under the mouse becomes the highlighted node when the player may
// extend it; otherwise it is remembered in unknown_a766fc.
#include "shared/mouse.hpp"

inline void Planning_Set_a7678c(PlanningNodeClass *node) { unknown_node_a7678c = node; }

void __fastcall Planning_Hover()
{
    if (unknown_flag_a76770) {
        return;
    }
    int x = WWMouse->Get_Mouse_X();
    int y = WWMouse->Get_Mouse_Y();
    PlanningNodeClass *node = Planning_Pick_Drawn(x, y);
    if (node != unknown_node_a7678c) {
        unknown_a766fc = 0;
        if (node != 0) {
            if (node->Member_Owner(0)->House != unknown_player_a35db4
                || (unknown_node_a766f8 != 0 && unknown_node_a766f8 != node)
                || Planning_Selection_Node_State(node) != -1) {
                unknown_a766fc = (int)node;
                node = 0;
            }
        }
        Planning_Set_a7678c(node);
        if (node != 0) {
            node->Dump();
        }
    }
}


// PlanningNodeClass *__stdcall Planning_Node_At(int x, int y) (0x00615D30)
// The remembered node (0x00A7678C) when its grown marker contains the screen point, or 0.

PlanningNodeClass *__stdcall Planning_Node_At(int x, int y)
{
    if (!unknown_node_a7678c) {
        return 0;
    }
    Point2D point(x, y);
    if (unknown_node_a7678c->Hit_Rect().Is_Point_Within(point)) {
        return unknown_node_a7678c;
    }
    return 0;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool __fastcall Unknown_Planning_00615DE0() (0x00615DE0)
// While planning: whether every selected object is a techno whose path ends
// at the same node as the first selected techno's; refuses
// (MSG:PlanningModeHeteroSel) a mixed selection. True outside planning or
// with nothing selected.
// from ra2-decomp 0x00639040 (FUN_00639040, src/planmgr.cpp)

// AbstractClass::Is_Techno as ra2-decomp writes it (not in this unit's
// ObjectClass declaration).
static inline bool Is_Techno(ObjectClass const *object)
{
    if (object == 0) return false;
    unsigned char flag = object->AbstractFlags.IsTechno;
    return flag != 0;
}

bool __fastcall Planning_Same_Last_Node(PlanningTokenClass *token1, PlanningTokenClass *token2);

bool __fastcall Unknown_Planning_00615DE0()
{
    if (!unknown_flag_a767a4) {
        return true;
    }
    int count = unknown_objects_a40c60.Count();
    if (count == 0) {
        return true;
    }
    int i;
    for (i = 0; i < count; i++) {
        ObjectClass *object = unknown_objects_a40c60[i];
        if (Is_Techno(object)) {
            PlanningTokenClass *first = Planning_Token((TechnoClass *)object);
            for (i = 0; i < count; i++) {
                ObjectClass *other = unknown_objects_a40c60[i];
                if (!Is_Techno(other)) goto fail;
                PlanningTokenClass *token = Planning_Token((TechnoClass *)other);
                if (!Planning_Same_Last_Node(first, token)) goto fail;
            }
            return true;
        }
    }
    return false;
fail:
    Planning_Refuse("MSG:PlanningModeHeteroSel");
    return false;
}


// bool Planning_Can_Add() (0x00615ED0)
// While planning: whether the player's planned technos plus the selected
// technos without a token or with an empty path stay within 128; refuses
// (Planning_Refuse, MSG:PlannerMaximum) when they do not.

bool __fastcall Planning_Can_Add()
{
    if (unknown_flag_a767a4) {
        int total = Planning_Count(unknown_player_a35db4->ArrayIndex);
        int added = 0;
        int count = unknown_objects_a40c60.Count();
        for (int i = 0; i < count; i++) {
            ObjectClass *object = unknown_objects_a40c60[i];
            bool techno;
            if (object != 0) {
                techno = object->AbstractFlags.IsTechno;
            }
            if (object != 0 && techno) {
                PlanningTokenClass *token = Planning_Token((TechnoClass *)object);
                if (token == 0) {
                    added++;
                } else if (token->Nodes.Count() == 0) {
                    added++;
                }
            }
        }
        if (total + added > 128) {
            Planning_Refuse("MSG:PlannerMaximum");
            return false;
        }
    }
    return true;
}


// PlanningNodeClass *__fastcall Planning_Find_Node(int house, int id, CoordStruct coord) (0x00615FB0)
// The node of unknown_list_00a76628 at a coordinate with the given id
// (-1: any) and house (-1: any).

PlanningNodeClass *__fastcall Planning_Find_Node(int house, int id, CoordStruct coord)
{
    int count = PlanningNodeClass::unknown_list_00a76628.Count();
    for (int i = 0; i < count; i++) {
        PlanningNodeClass *node = PlanningNodeClass::unknown_list_00a76628[i];
        if (node->Is_At(coord) && (id == -1 || id == node->unknown_18)) {
            int owner = Planning_House(node->Member_Owner(0));
            if (house == -1 || house == owner) {
                return node;
            }
        }
    }
    return 0;
}


// PlanningNodeClass *__stdcall Planning_Pick_Node(int x, int y) (0x00616070)
// The first drawn node (unknown_list_00a76628) whose grown marker contains
// the screen point, or 0.

PlanningNodeClass *__stdcall Planning_Pick_Node(int x, int y)
{
    int count = PlanningNodeClass::unknown_list_00a76628.Count();
    for (int i = 0; i < count; i++) {
        PlanningNodeClass *node = PlanningNodeClass::unknown_list_00a76628[i];
        Point2D point(x, y);
        if (node->Hit_Rect().Is_Point_Within(point)) {
            return node;
        }
    }
    return 0;
}


// PlanningNodeClass *__stdcall Planning_Pick_Drawn(int x, int y) (0x00616140)
// The first drawn node (unknown_list_00a766d8) whose grown marker contains
// the screen point, or 0.

PlanningNodeClass *__stdcall Planning_Pick_Drawn(int x, int y)
{
    int count = PlanningNodeClass::unknown_list_00a766d8.Count();
    for (int i = 0; i < count; i++) {
        PlanningNodeClass *node = PlanningNodeClass::unknown_list_00a766d8[i];
        Point2D point(x, y);
        if (node->Hit_Rect().Is_Point_Within(point)) {
            return node;
        }
    }
    return 0;
}


// PlanningNodeClass *__stdcall Planning_Find_Drawn(CoordStruct coord) (0x00616210)
// The node on screen (unknown_list_00a766d8) at a coordinate, or 0.

PlanningNodeClass *__stdcall Planning_Find_Drawn(CoordStruct coord)
{
    int count = PlanningNodeClass::unknown_list_00a766d8.Count();
    for (int i = 0; i < count; i++) {
        PlanningNodeClass *node = PlanningNodeClass::unknown_list_00a766d8[i];
        CoordStruct here = node->Coord();
        if (here == coord) {
            return node;
        }
    }
    return 0;
}


// void __fastcall Planning_Send_Pair(AbstractClass *source, PlanningNodeClass *node) (0x00616280)
// Sends the player's type 0x2A event naming the source and the object the
// node's first order points at.

void __fastcall Planning_Send_Pair(AbstractClass *source, PlanningNodeClass *node)
{
    EventClass event = *node->Members[0]->Event;
    PlanningEventTargets const *view = (PlanningEventTargets const *)&event;
    TargetClass const *reference;
    if (view->Target_0e.Kind) {
        reference = &view->Target_0e;
    } else if (view->Target_18.Kind) {
        reference = &view->Target_18;
    } else if (view->Target_13.Kind) {
        reference = &view->Target_13;
    } else {
        return;
    }
    AbstractClass *object = reference->As_Abstract();
    if (object) {
        EventClass send(unknown_player_a35db4->ArrayIndex, 0x2A, TargetClass(source), TargetClass(object));
        Unknown_0062E670(send);
    }
}


// void __fastcall Planning_Path_Add(TechnoClass *techno, PlanningNodeClass *node) (0x00616350)
// Gives the techno a planning token if it has none, then closes its path
// into a loop at the node when the path holds it already, else appends it.
// Planning_Close_Loop is defined here, `inline`: this function expands it, and so
// does Planning_Close_Loop_006164e0 (0x006164E0, ra2-decomp's FUN_00639740 form),
// the function Planning_Execute 0x00614BA0 calls.

// void __fastcall Planning_Close_Loop(TechnoClass *techno, PlanningNodeClass *node) (inline; expanded at
// 0x00616350 and 0x006164E0)
// Makes the techno's path loop back to a node: the loop runs from the node's
// index to the path's end. The first time, the members of the path's last
// node owned by a human house get slot 81; then 0x00616A50 runs.
inline void __fastcall Planning_Close_Loop(TechnoClass *techno, PlanningNodeClass *node)
{
    int start = Token_Node_Index(Planning_Token(techno), node);
    PlanningTokenClass *token = Planning_Token(techno);
    token->LoopLength = token->Nodes.Count() - start;
    token->LoopStart = start;
    token->unknown_98 = true;
    if (!unknown_flag_a76788) {
        if (techno->House->Is_Player_Control()) {
            unknown_flag_a76788 = true;
            PlanningNodeClass *last = token->Last_Node();
            int count = last->Members.Count();
            for (int i = 0; i < count; i++) {
                TechnoClass *member = last->Members[i]->Owner;
                if (member->unknown_77) {
                    member->Unselect();
                }
            }
        }
        Planning_Unknown_616a50(techno);
    }
    node->unknown_611770();
}


void __fastcall Planning_Path_Add(TechnoClass *techno, PlanningNodeClass *node)
{
    PlanningTokenClass *token = Planning_Token(techno);
    if (!token) {
        token = Planning_New_Token(techno);
    }
    if (token->Node_Index(node) != -1) {
        Planning_Close_Loop(techno, node);
    } else {
        Planning_Path_Append(techno, node);
    }
}


// void __fastcall Planning_Close_Loop_006164e0(TechnoClass *techno, PlanningNodeClass *node) (0x006164E0;
// Planning_Execute 0x00614BA0 calls it): closes the techno's path into a loop at the node through the inline
// Planning_Close_Loop (defined in Planning_Path_Add's file 0x00616350, which expands it too).
// From ra2-decomp 0x00639740 (YR identical twin, src/planmgr.cpp FUN_00639740):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

void __fastcall Planning_Close_Loop_006164e0(TechnoClass * techno, PlanningNodeClass * node)
{
	Planning_Close_Loop(techno, node);
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Planning_Path_Append(TechnoClass *techno, PlanningNodeClass *node) (0x006165A0)
// Gives techno the rest of the path of the node's first member from the node
// on, and that path's loop: from the loop start when the node lies before
// it, else from the node with the stretch up to it shared too. Selects (or,
// with a loop, unselects) the members of techno's last node the first time
// a human house does this.
// from ra2-decomp 0x00639800 (FUN_00639800, src/planmgr.cpp)

void __fastcall Planning_Path_Append(TechnoClass *techno, PlanningNodeClass *node)
{
    TechnoClass *owner = node->Member_Owner(0);
    PlanningTokenClass *initial = Planning_Token(owner);
    int from = initial->Node_Index(node);
    PlanningTokenClass *source = Planning_Token(owner);
    source->Copy_Nodes_To(techno, from);

    int start = -1;
    int loop_count = -1;
    PlanningNodeClass *update = 0;
    PlanningTokenClass *target = Planning_Token(techno);
    if (source->Loop_Past_Start() || source->Loop_At_Start()) {
        int const source_loop_count = source->Get_LoopLength();
        int step = source->Get_LoopStart();
        if (source->Loop_Past_Start() && from < step) {
            PlanningNodeClass *loop = source->Node_At(step);
            start = target->Node_Index(loop);
            update = loop;
        } else {
            source->Copy_Nodes_To(techno, step, from - step);
            start = target->Node_Index(node);
            update = node;
        }
        loop_count = source_loop_count;
    }
    target->LoopLength = loop_count;
    target->LoopStart = start;
    target->unknown_98 = true;
    if (update) {
        update->unknown_611770();
    }

    if (!unknown_flag_a76790 && techno->House->Is_Player_Control()) {
        unknown_flag_a76790 = true;
        PlanningNodeClass *last = target->Last_Node();
        if (!update) {
            last->unknown_612090();
        } else {
            last->unknown_6120c0();
        }
    }
}


// void __fastcall Planning_Copy_Members(PlanningNodeClass *node, PlanningNodeClass *from, EventClass event) (0x006167F0)
// Adds every member of one node to another, each with a copy of the event
// aimed at that member's techno.

void __fastcall Planning_Copy_Members(PlanningNodeClass *node, PlanningNodeClass *from, EventClass event)
{
    int count = from->Members.Count();
    for (int i = 0; i < count; i++) {
        TechnoClass *techno = from->Members[i]->Owner;
        event.TargetAt(0x07) = TargetClass(techno);
        PlanningMemberClass *member = new PlanningMemberClass;
        member->Owner = techno;
        EventClass *copy = new EventClass;
        *copy = event;
        member->Event = copy;
        node->Members.Add(member);
        PlanningTokenClass *token = Planning_Token(techno);
        token->Nodes.Add(node);
        if (token->unknown_8c == -1) {
            token->unknown_8c = token->Nodes.Count() - 1;
        }
        node->unknown_611bd0(member);
    }
}


// void __fastcall Planning_Remove_Node(PlanningNodeClass *node) (0x00616960)
// Takes a node out of the paths of all its members (PlanningTokenClass
// 0x006138F0 on each member's token, last member first); the node after it in
// the first member's path (PlanningNodeClass::Next, expanded inline) is then
// rebuilt (0x00611EE0, 0x00611BD0 per member, 0x00611770) and marked not
// queued. Name hypothesis.
// The member loop is a file-local inline helper (hypothesis): written in place
// it leaves one /Ob2 site too many after Next, and Next's PlanningTokenClass::
// Node_At 0x00613C00 is then called out of line, where the original expands it.

static inline void Planning_Unlink_Node(PlanningNodeClass *node)
{
    for (int i = node->Members.Count() - 1; i >= 0; i--) {
        PlanningTokenClass *member_token = Planning_Token(node->Members[i]->Owner);
        member_token->unknown_6138f0(member_token->Node_Index(node));
    }
}

void __fastcall Planning_Remove_Node(PlanningNodeClass *node)
{
    if (node != 0) {
        PlanningNodeClass *next = node->Next(node->Members[0]->Owner);
        Planning_Unlink_Node(node);
        if (next != 0 && next != node) {
            next->unknown_611ee0();
            int count = next->Members.Count();
            for (int j = 0; j < count; j++) {
                next->unknown_611bd0(next->Members[j]);
            }
            next->unknown_611770();
            next->IsQueued = false;
        }
    }
}


// Not exact (RA2_NON_MATCHING): Planning_Unknown_616a50 at 0x00616A50, best score 77.83, 2026-11-13; differs: 101 instructions in the original, 102 in the draft (shape 97.54)
#ifdef RA2_NON_MATCHING
// void __fastcall Planning_Unknown_616a50(TechnoClass *techno) (0x00616A50)
// When a member of the first node of the techno's path takes slot 275,
// every node of the loop (from LoopStart to the path's end) turns its
// members' kind-2 orders into kind 0x1D (PlanningNodeClass::unknown_611440).

void __fastcall Planning_Unknown_616a50(TechnoClass *techno)
{
    PlanningTokenClass *token = Planning_Token(techno);
    int count = token->Nodes.Count();
    int start = token->LoopStart;
    PlanningNodeClass *first = token->Node_At(0);
    int members = first->Members.Count();
    for (int i = 0; i < members; i++) {
        if (first->Members[i]->Owner->unknown_slot275()) {
            for (int index = start; index < count; index++) {
                token->Node_At(index)->unknown_611440();
            }
            return;
        }
    }
}
#endif

// int Planning_Selection_Kind() (0x00616B40)
// Classifies the selection for planning mode: -1 when slot 274 accepts every
// selected techno, else the first refused object's kind: aircraft (0),
// building (1) or anything else, non-technos included (2).

// What kind of refused object this is.
inline int Planning_Object_Kind(ObjectClass *object)
{
    if (object != 0) {
        if (object->Kind_Of() == 2) {
            return 0;
        }
        if (object->Kind_Of() == 6) {
            return 1;
        }
    }
    return 2;
}

int Planning_Selection_Kind()
{
    int kind = -1;
    int count = unknown_objects_a40c60.Count();
    for (int i = 0; i < count; i++) {
        ObjectClass *object = unknown_objects_a40c60[i];
        bool techno;
        if (object != 0) {
            techno = object->AbstractFlags.IsTechno;
        }
        TechnoClass *candidate = (object != 0 && techno) ? (TechnoClass *)object : 0;
        if (candidate == 0 || !candidate->unknown_slot274()) {
            kind = Planning_Object_Kind(candidate);
            break;
        }
    }
    return kind;
}


// bool __fastcall Planning_Same_Last_Node(PlanningTokenClass *token1, PlanningTokenClass *token2) (0x00616BD0)
// Whether two planning tokens end at the same node: both empty (or null), or
// each one's last node holds the other's owner. Name hypothesis.

bool __fastcall Planning_Same_Last_Node(PlanningTokenClass *token1, PlanningTokenClass *token2)
{
    if (token1 == 0 && token2 == 0) {
        return true;
    }
    int count1 = 0;
    if (token1 != 0) {
        count1 = token1->Nodes.Count();
    }
    int count2 = 0;
    if (token2 != 0) {
        count2 = token2->Nodes.Count();
    }
    if (count1 == 0 && count2 == 0) {
        return true;
    }
    if (count1 == 0 || count2 == 0) {
        return false;
    }
    PlanningNodeClass *last1 = token1->Node_At(count1 - 1);
    PlanningNodeClass *last2 = token2->Node_At(count2 - 1);
    TechnoClass *owner1 = token1->Owner;
    TechnoClass *owner2 = token2->Owner;
    if (!last1->Has_Member(owner2)) {
        return false;
    }
    if (!last2->Has_Member(owner1)) {
        return false;
    }
    return true;
}


// 0x00616d00: function not yet identified
//
// Body: 17 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_count_increment_00616d00(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00616d00 {

extern "C" unsigned int unknown_planning_counts_00a76684;

void __fastcall unknown_count_increment_00616d00(int index)
{
    if ((index >= 0) && (index < 8)) {
        (&unknown_planning_counts_00a76684)[index] = (&unknown_planning_counts_00a76684)[index] + 1;
    }
    return;
}

}  // namespace seed_00616d00


// void __fastcall Planning_Uncount(int house) (0x00616D20)
// Lowers a house's counter (never below 0).

void __fastcall Planning_Uncount(int house)
{
    if (house >= 0 && house < 8) {
        int value = unknown_counts_a76684[house] - 1;
        unknown_counts_a76684[house] = value < 0 ? 0 : value;
    }
}


// int __fastcall Planning_Count(int house) (0x00616D50)
// A house's counter (0 for a house number out of range).

int __fastcall Planning_Count(int house)
{
    bool valid;
    if (house >= 0 && house < 8) {
        valid = true;
    } else {
        valid = false;
    }
    int count = 0;
    if (valid) {
        count = unknown_counts_a76684[house];
    }
    return count;
}


// void __fastcall Planning_Refuse_Event(EventClass const *event) (0x00616D70)
// Shows the refusal for an order planning mode cannot take (deploy, stop or
// scatter, as 0x00617020) and plays the refused-action sound.

void __fastcall Planning_Refuse_Event(EventClass const *event)
{
    if (event->Type == 9) {
        Planning_Refuse("MSG:PlanningModeNoDeploy");
    } else if (event->Type == 6) {
        Planning_Refuse("MSG:PlanningModeNoStop");
    } else if (event->Type == 7) {
        Planning_Refuse("MSG:PlanningModeNoScatter");
    }
    Voc_Play(Rules->unknown_620, 1.0f, 0x2000, 0);
}


// void Planning_Refuse_No_Guard_Area() (0x00616E70)
// Planning mode cannot take an area guard order.

void __fastcall Planning_Refuse_No_Guard_Area()
{
    Planning_Refuse("MSG:PlanningModeNoGuardArea");
}


// void __fastcall Planning_Intro(bool forced, bool first) (0x00616EC0)
// Shows a planning-mode introduction message: the first introduction (by
// button when forced, else by key) or the third; unless forced, each kind is
// shown once.

void __fastcall Planning_Intro(bool forced, bool first)
{
    int kind;
    if (first) {
        kind = !forced;
    } else {
        kind = 2;
    }
    int limit;
    if (forced) {
        limit = unknown_counts_a766b4[kind] + 1;
    } else {
        limit = 1;
    }
    if (unknown_counts_a766b4[kind] < limit) {
        if (first) {
            if (forced) {
                Planning_Message("MSG:PlanningModeIntro1Button");
            } else {
                Planning_Message("MSG:PlanningModeIntro1Key");
            }
        } else {
            Planning_Message("MSG:PlanningModeIntro3");
        }
        unknown_counts_a766b4[kind]++;
    }
}


// void __fastcall Planning_Help(int kind) (0x00616F50)
// Shows a planning-mode help message the first time it is wanted: the loop
// help (kind 3) or the delete help (kind 4).

void __fastcall Planning_Help(int kind)
{
    if (unknown_counts_a766b4[kind] < 1) {
        if (kind == 3) {
            Planning_Message("MSG:PlanningModeLoopHelp");
        } else if (kind == 4) {
            Planning_Message("MSG:PlanningModeDeleteHelp");
        }
        unknown_counts_a766b4[kind]++;
    }
}


// void __fastcall Planning_Message(char const *label) (0x00616FB0)
// Shows a planning-mode help message (the string table label in ecx).
#include "shared/string_table.hpp"

void __fastcall Planning_Message(char const *label)
{
    wchar_t const *text = StringTable::Load_String(label, 0, "D:\\Ra2\\PlanMgr.cpp", 3231);
    Unknown_006F58E0(text, 0x1E0);
}


// void __fastcall Planning_Refuse(char const *label) (0x00616FD0)
// Once per planning session: shows the refusal message and plays the
// refused-action sound.

void __fastcall Planning_Refuse(char const *label)
{
    if (!unknown_flag_a766c8) {
        unknown_flag_a766c8 = true;
        Planning_Message(label);
        Voc_Play(Rules->unknown_620, 1.0f, 0x2000, 0);
    }
}


// void __fastcall Planning_Refuse_Order(EventClass const *event) (0x00617020)
// Shows the refusal for an order planning mode cannot take: deploy (event
// type 9), stop (6) or scatter (7). Name hypothesis; no callers.

void __fastcall Planning_Refuse_Order(EventClass const *event)
{
    if (event->Type == 9) {
        Planning_Refuse("MSG:PlanningModeNoDeploy");
    } else if (event->Type == 6) {
        Planning_Refuse("MSG:PlanningModeNoStop");
    } else if (event->Type == 7) {
        Planning_Refuse("MSG:PlanningModeNoScatter");
    }
}


// void __fastcall Planning_Refuse_Selection(int kind) (0x00617100)
// Shows the refusal for a selection planning mode cannot take: aircraft (0),
// buildings (1) or unknown objects (2) (the values of Planning_Selection_Kind).

void __fastcall Planning_Refuse_Selection(int kind)
{
    if (kind == 0) {
        Planning_Refuse("MSG:PlanningModeNoAircraft");
    } else if (kind == 1) {
        Planning_Refuse("MSG:PlanningModeNoBuildings");
    } else if (kind == 2) {
        Planning_Refuse("MSG:PlanningModeNoUnknowns");
    }
}


// int __fastcall Planning_Event_Result(PlanningTokenClass *token, PlanningEventTargets const *event) (0x006171E0)
// What planning mode does with an order (the kinds of Planning_Refuse_Command):
// 0 for a terminating order (missions 7-9, or a move/attack whose target
// reference is of kind 11 or, for mission 11, empty), 2 when the token's path
// is full (15 nodes), else -1 (taken).

int __fastcall Planning_Event_Result(PlanningTokenClass *token, PlanningEventTargets const *event)
{
    int mission = event->Mission();
    TargetClass const &target = event->Target();
    if (Planning_Terminal_Mission(mission)) {
        return 0;
    }
    if (Planning_Cell_Mission(mission, target)) {
        return 0;
    }
    if (token->Get_Node_Count() == 15) {
        return 2;
    }
    return -1;
}


// 0x00617240: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_set_a76750_00617240(undefined4 param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00617240 {

extern "C" unsigned int unknown_planning_00a76750;

void __fastcall unknown_set_a76750_00617240(unsigned int value)
{
    unknown_planning_00a76750 = value;
    return;
}

}  // namespace seed_00617240


// void __fastcall Planning_Refuse_Command(int kind) (0x00617250)
// Drops the pending command and shows the refusal: terminating command
// (kind 0), continual command (1) or node maximum (2).

void __fastcall Planning_Refuse_Command(int kind)
{
    unknown_tactical_83996c->Unknown_006AAD40();
    if (kind == 0) {
        Planning_Refuse("MSG:PostTerminatingCommand");
    } else if (kind == 1) {
        Planning_Refuse("MSG:PostContinualCommand");
    } else if (kind == 2) {
        Planning_Refuse("MSG:NodeMaximum");
    }
}


// The game screen as this unit sees it (the full screen headers, shared/mouse_class.hpp
// and shared/map_view.hpp, change the unit's declaration and initializer counts).
struct MouseShapeHeader {
    short unknown_00;
    short Width;
    short Height;
    short FrameCount;
};
class MouseClass {
public:
    virtual void Set_Default_Mouse(int mouse, bool wsmall = false);   // 0x0059F060
    static MouseShapeHeader const *MouseShapes;
};
extern MouseClass Map;                          // 0x008324E0

// Not exact (RA2_NON_MATCHING): Planning_Left_Down at 0x00617340, best score 38.59, 2026-11-13; differs: 234 instructions in the original, 248 in the draft (shape 64.32)
#ifdef RA2_NON_MATCHING
// bool __stdcall Planning_Left_Down_00617340(Point2D point) (0x00617340)

bool __stdcall Planning_Left_Down_00617340(Point2D point)
{
    if (!Planning_Is_Active()) {
        return false;
    }
    PlanningNodeClass *remembered = Planning_Get_a766f8();
    PlanningNodeClass *node = Planning_Node_At(point.X, point.Y);
    if (node != 0 && node != remembered) {
        Map.Set_Default_Mouse(0, true);
        Planning_Set_a766f8(node);
    } else {
        PlanningNodeClass *picked = Planning_Pick_Node(point.X, point.Y);
        if (picked != 0) {
            if (Planning_Selection_Node_State(picked) != -1) {
                Planning_Refuse("MSG:PlanningModeInvalidNode");
                return true;
            }
        } else {
            int kind = Planning_Selection_Kind();
            if (kind != -1) {
                Planning_Refuse_Selection(kind);
                return true;
            }
        }
    }
    return remembered != 0 || node != 0;
}
#endif

// bool __stdcall Planning_Left_Up_00617680(Point2D point) (0x00617680)
// The left button came up: over the remembered node it ends the drag; over a node of the
// selected paths it is taken; otherwise the remembered node is dropped.

void Planning_Unknown_617af0();

bool __stdcall Planning_Left_Up_00617680(Point2D point)
{
    if (!Planning_Is_Active()) {
        return false;
    }
    PlanningNodeClass *remembered = Planning_Get_a766f8();
    PlanningNodeClass *node = Planning_Node_At(point.X, point.Y);
    if (node != 0 && node == remembered) {
        Map.Set_Default_Mouse(0, true);
        Planning_Unknown_617af0();
    } else {
        PlanningNodeClass *picked = Planning_Pick_Node(point.X, point.Y);
        if (picked != 0 && Planning_Selection_Node_State(picked) != -1) {
            return true;
        }
        Planning_Set_a766f8(0);
    }
    return remembered != 0 || node != 0;
}


// void __stdcall Planning_Mouse_Down(int x, int y) (0x00617860)
// The mouse button went down: remembers the screen point when planning mode
// is on and nothing is selected.

void __stdcall Planning_Mouse_Down(int x, int y)
{
    unknown_flag_a76770 = true;
    unknown_point_a76728 = Point2D(-1, -1);
    if (unknown_flag_a767a4 && unknown_objects_a40c60.Count() <= 0) {
        unknown_point_a76728 = Point2D(x, y);
    }
}


// void __stdcall Planning_Mouse_Up(int x, int y) (0x006178A0)
// A click (the button came up within 4 pixels of where it went down) forgets
// the remembered node.

void __stdcall Planning_Mouse_Up(int x, int y)
{
    unknown_flag_a76770 = false;
    if (unknown_flag_a767a4 && unknown_objects_a40c60.Count() <= 0 && unknown_node_a766b0
        && abs(x - unknown_point_a76728.X) <= 3 && abs(y - unknown_point_a76728.Y) <= 3) {
        unknown_node_a766b0 = 0;
    }
}


// bool __stdcall Planning_Mouse_Move_00617900(int x, int y) (0x00617900)

bool __stdcall Planning_Mouse_Move_00617900(int x, int y)
{
    if (!unknown_flag_a767a4) {
        return false;
    }
    if (unknown_flag_a76770) {
        return false;
    }
    PlanningNodeClass *remembered = unknown_node_a766f8;
    PlanningNodeClass *node = Planning_Node_At(x, y);
    if (remembered == 0 && node == 0) {
        return false;
    }
    if (remembered == 0 || node == remembered) {
        Map.Set_Default_Mouse(0, true);
    } else {
        Map.Set_Default_Mouse(0, false);
    }
    return true;
}


class Leaf_617a00 { public: unsigned int get(); };
unsigned int Leaf_617a00::get() { unsigned int self = (unsigned int)this; unsigned int value; unsigned int spare; value = *(unsigned int*)(0xa766b0); return value; }


// int __fastcall Planning_Selection_Node_State(PlanningNodeClass *node) (0x00617A10)
// Where a node stands in the paths of the selected technos: 1 when a path
// is closed into a loop, 5 when a path lacks the node, 2 / 3 when it is the
// last / next to last node of a path, else -1. Name hypothesis.

int __fastcall Planning_Selection_Node_State(PlanningNodeClass *node)
{
    int count = unknown_objects_a40c60.Count();
    for (int i = 0; i < count; i++) {
        ObjectClass *object = unknown_objects_a40c60[i];
        bool techno;
        if (object != 0) {
            techno = object->AbstractFlags.IsTechno;
        }
        if (object != 0 && techno) {
            PlanningTokenClass *token = Planning_Token((TechnoClass *)object);
            if (token != 0) {
                bool looped = token->LoopStart > 0 || token->LoopStart == 0;
                if (looped) {
                    return 1;
                }
                int index = token->Node_Index(node);
                if (index == -1) {
                    return 5;
                }
                if (index == token->Nodes.Count() - 1) {
                    return 2;
                }
                if (index == token->Nodes.Count() - 2) {
                    return 3;
                }
            }
        }
    }
    return -1;
}


// 0x00617ad0: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_set_a766f8_00617ad0(undefined4 param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00617ad0 {

extern "C" unsigned int unknown_planning_00a766f8;

void __fastcall unknown_set_a766f8_00617ad0(unsigned int value)
{
    unknown_planning_00a766f8 = value;
    return;
}

}  // namespace seed_00617ad0


// 0x00617ae0: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 unknown_get_a766f8_00617ae0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00617ae0 {

extern "C" unsigned int unknown_planning_00a766f8;

unsigned int  unknown_get_a766f8_00617ae0(void)
{
    return unknown_planning_00a766f8;
}

}  // namespace seed_00617ae0


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Planning_Unknown_617af0() (0x00617AF0)
// Adopted from ra2-decomp FUN_0063AD50 (YR 0x0063AD50).
// Connects the remembered node to the selection: with objects selected,
// every selected techno without a path, and every member of the last node
// of each selected path, gets a connect event (0x2A) to the node's target
// object, then help message 3; with none, the node is kept in
// unknown_node_a766b0 and help message 4 shown.

static inline void Connect_Node_Target(PlanningNodeClass *node, TechnoClass *techno)
{
    EventClass packet = node->Member_Event(0);
    AbstractClass *target = ((PlanningEventTargets *)&packet)->First_Target_Object();
    if (target) {
        EventClass event(unknown_player_a35db4->ArrayIndex, 0x2A, TargetClass(techno), TargetClass(target));
        Unknown_0062E670(event);
    }
}

void Planning_Unknown_617af0()
{
    PlanningNodeClass *selected = Planning_Get_a766f8();
    Planning_Set_a766f8(0);
    int count = unknown_objects_a40c60.Count();
    if (count > 0) {
        DynamicVectorClass<TechnoClass *> units;
        for (int i = 0; i < count; ++i) {
            ObjectClass *object = unknown_objects_a40c60[i];
            if (Is_Techno(object)) {
                TechnoClass *techno = (TechnoClass *)object;
                PlanningTokenClass *token = Planning_Token(techno);
                if (token) {
                    PlanningNodeClass *last = token->Last_Node();
                    if (!last) {
                        units.Add(techno);
                    } else {
                        int members = last->Get_Member_Count();
                        for (int j = 0; j < members; ++j) {
                            TechnoClass *owner = last->Member_Owner(j);
                            if (units.ID(owner) == -1) units.Add(owner);
                        }
                    }
                } else {
                    units.Add(techno);
                }
            }
        }
        int total = units.Count();
        for (int j = 0; j < total; ++j) {
            TechnoClass *techno = units[j];
            Connect_Node_Target(selected, techno);
        }
        if (total > 0) Planning_Help(3);
    } else {
        unknown_node_a766b0 = selected;
        Planning_Help(4);
    }
}


// AbstractClass *PlanningEventTargets::First_Target_Object() const (0x00617DF0)
// The object of the first set reference (+0x0E, then +0x18, then +0x13), or 0.

AbstractClass *PlanningEventTargets::First_Target_Object() const
{
    AbstractClass *object = 0;
    if (Target_0e.Kind) {
        object = Target_0e.As_Abstract();
    } else if (Target_18.Kind) {
        object = Target_18.As_Abstract();
    } else if (Target_13.Kind) {
        object = Target_13.As_Abstract();
    }
    return object;
}


// 0x00617e20: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_set_a766b0_00617e20(undefined4 param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00617e20 {

extern "C" unsigned int unknown_planning_00a766b0;

void __fastcall unknown_set_a766b0_00617e20(unsigned int value)
{
    unknown_planning_00a766b0 = value;
    return;
}

}  // namespace seed_00617e20


// 0x00617e30: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_clear_a766b0_00617e30(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00617e30 {

extern "C" unsigned int unknown_planning_00a766b0;

void  unknown_clear_a766b0_00617e30(void)
{
    unknown_planning_00a766b0 = 0;
    return;
}

}  // namespace seed_00617e30


// void Planning_Redraw() (0x00617E40)
// Forgets the nodes drawn on screen, then draws every node and every
// planning techno that belongs to the player or an ally.

void __fastcall Planning_Draw_Techno(TechnoClass *techno);

void Planning_Redraw()
{
    Planning_Clear_Drawn();
    int count = PlanningNodeClass::unknown_list_00a76628.Count();
    int i;
    for (i = 0; i < count; i++) {
        PlanningNodeClass *node = PlanningNodeClass::unknown_list_00a76628[i];
        if (node->House()->Is_Ally(unknown_player_a35db4)) {
            Planning_Draw_Node_619420(node);
        }
    }
    count = unknown_list_a40c20.Count();
    for (i = 0; i < count; i++) {
        TechnoClass *techno = unknown_list_a40c20[i];
        HouseClass *house = techno->House;
        if (house->Is_Ally(unknown_player_a35db4)) {
            Planning_Draw_Techno(techno);
        }
    }
}


// void Planning_Draw_All() (0x00617EF0)
// Draws the planning mode for the player's allies: every node's paths, then
// every node's filled marker, then every planned techno.

// Fills a node marker shrunk by a pixel (file-static helper; hypothesis: the
// original expands one more charged level here, which keeps Planning_Centered_Rect
// and PlanningNodeClass::Coord out of line).
static inline void Planning_Fill_Inner_Marker(PlanningNodeClass *node, int color)
{
    ::Rect view = Get_Title_Screen_Area_006f1f30();
    ::Rect marker = node->Rect();
    Planning_Inflate(marker, -1);
    unknown_surface_839964->Fill_Rect(view, marker, color);
}

void Planning_Draw_All()
{
    int count = PlanningNodeClass::unknown_list_00a76628.Count();
    int i;
    for (i = 0; i < count; i++) {
        PlanningNodeClass *node = PlanningNodeClass::unknown_list_00a76628[i];
        if (node->House()->Is_Ally(unknown_player_a35db4)) {
            Planning_Draw_Node_619870(node);
        }
    }
    for (i = 0; i < count; i++) {
        PlanningNodeClass *node = PlanningNodeClass::unknown_list_00a76628[i];
        if (node->House()->Is_Ally(unknown_player_a35db4)) {
            Planning_Fill_Inner_Marker(node, node->Color());
        }
    }
    int technos = unknown_list_a40c20.Count();
    for (i = 0; i < technos; i++) {
        TechnoClass *techno = unknown_list_a40c20[i];
        if (techno->House->Is_Ally(unknown_player_a35db4)) {
            Planning_Draw_Techno_618c00(techno);
        }
    }
}


// void Planning_Draw_Remembered() (0x00618090; called from Tactical 0x6A503A, not yet recovered)
// Outlines the remembered nodes 0x00A7678C and 0x00A766B0 in the fade colour.

void Planning_Draw_Remembered()
{
    int color = Planning_Pixel(&unknown_rgb_ab6849);
    if (unknown_node_a7678c) {
        unknown_node_a7678c->Draw_Outer_Outline(color);
    }
    if (unknown_node_a766b0) {
        unknown_node_a766b0->Draw_Marker_Outline(color);
    }
}


// void __fastcall Planning_Draw_Techno(TechnoClass *techno) (0x006182F0)
// Draws one techno's current order and its first queued planning waypoint: dots,
// a thick line to the destination and a midpoint marker when the leg has an event.
#include "shared/tactical.hpp"
bool __fastcall Planning_Idle_Mover(TechnoClass *techno);

inline bool Planning_Current_Order(TechnoClass *techno, bool has_nodes)
{
    return techno->Get_Mission() != 5 && !Planning_Idle_Mover(techno) && (has_nodes || techno->unknown_77);
}

void __fastcall Planning_Draw_Techno(TechnoClass *techno)
{
    PlanningTokenClass *token = (PlanningTokenClass *)techno->unknown_6d1430();
    CoordStruct current_coord = techno->unknown_slot22();
    Point2D start = Planning_Coord_Pixel(current_coord);
    int color = Planning_Techno_Color(techno);
    bool has_nodes = (!token ? 0 : token->Nodes.Count()) > 0;
    bool current_order = Planning_Current_Order(techno, has_nodes);
    int thickness = 4;
    if (token && (token->Owner->Mission == 28 || token->Owner->MissionQueue == 28)) {
        EventClass event = token->Event;
        CoordStruct coord = Planning_Event_Or_Object_Coord(event, techno);
        Point2D end = Planning_Coord_Pixel(coord);
        Planning_Draw_Dot(start, color);
        Planning_Draw_Thick_Line(start, end, 3, color);
        if (((PlanningEventTargets const &)event).unknown_61a2d0() != -1) Planning_Draw_Midpoint(start, end, color);
        Planning_Draw_Dot(end, color);
        return;
    }
    if (current_order) {
        bool alternate = techno->unknown_slot270();
        bool secondary = techno->unknown_slot271();
        CoordStruct coord = techno->unknown_slot241();
        Point2D end = Planning_Coord_Pixel(coord);
        if (techno->unknown_76) {
            Planning_Draw_Dot(end, color);
            return;
        }
        Planning_Draw_Dot(start, color);
        Planning_Draw_Thick_Line(start, end, 3, color);
        int action = -1;
        if (!alternate || secondary) {
            if (techno->Get_Mission() == 11) action = PlanningEventTargets::Unknown_006F6760();
        } else if (techno->unknown_slot272() == 29) action = PlanningEventTargets::Unknown_006F6750();
        if (action != -1) Planning_Draw_Midpoint(start, end, color);
        Planning_Draw_Dot(end, color);
        if (secondary) {
            CoordStruct next_coord = techno->unknown_slot273();
            Point2D next = Planning_Coord_Pixel(next_coord);
            Planning_Draw_Thick_Line(end, next, 3, color);
            if (techno->unknown_slot272() == 29 && PlanningEventTargets::Unknown_006F6750() != -1)
                Planning_Draw_Midpoint(end, next, color);
            Planning_Draw_Dot(next, color);
            return;
        }
        if (!has_nodes) return;
        start = end;
        thickness = 3;
    } else if (!has_nodes) {
        return;
    }
    {
        PlanningNodeClass *node = token->Node_At(0);
        Point2D end = node->Pixel();
        PlanningBranchClass *branch = node->Branch_For(techno);
        if (branch) end = Planning_Event_Pixel(&branch->Event, techno);
        Planning_Draw_Dot(start, color);
        Planning_Draw_Thick_Line(start, end, thickness, color);
        if (branch) Planning_Draw_Dot(end, color);
        EventClass event = node->Event_For(techno);
        if (((PlanningEventTargets const &)event).unknown_61a2d0() != -1)
            Planning_Draw_Midpoint(start, end, color);
    }
}


// bool __fastcall Planning_Idle_Mover(TechnoClass *techno) (0x00618BB0)
// Whether a moving techno (mission 0xB) has no destination object yet.
#include "shared/foot.hpp"

bool __fastcall Planning_Idle_Mover(TechnoClass *techno)
{
    if (techno->Get_Mission() == 0x0B) {
        bool flagged;
        if (techno) {
            flagged = techno->AbstractFlags.IsFoot;
        }
        if (techno && flagged) {
            if (((FootClass *)techno)->unknown_494 || techno->unknown_228) {
                return false;
            }
        }
        return true;
    }
    return false;
}


// void __fastcall Planning_Draw_Techno_618c00(TechnoClass *techno) (0x00618C00)
// The dashed twin of Planning_Draw_Techno: filled dots, dashed lines and mouse-shape
// midpoint markers in the dim house colour; the first queued leg dims or brightens with the path.
void __fastcall Planning_Draw_Midpoint_Shape(Point2D start, Point2D end, int color, int fill, int frame);
struct UnknownTechno_00610770;
int __fastcall unknown_owner_color(UnknownTechno_00610770 *techno);

inline RGBClass Planning_Owner_House_Color(TechnoClass *techno)
{
    int house = unknown_owner_color((UnknownTechno_00610770 *)techno);
    return Houses[house]->unknown_55e9;
}

inline int Planning_House_Pixel(TechnoClass *techno, RGBClass const &target, int ratio)
{
    return Planning_Adjusted_Pixel(Planning_Owner_House_Color(techno), target, ratio);
}

inline bool Planning_Reached(PlanningTokenClass const *token, int slot)
{
    int first = token->unknown_8c;
    if (first == -1) return false;
    return slot >= first;
}

void __fastcall Planning_Draw_Techno_618c00(TechnoClass *techno)
{
    PlanningTokenClass *token = (PlanningTokenClass *)techno->unknown_6d1430();
    CoordStruct current_coord = techno->unknown_slot22();
    Point2D start = Planning_Coord_Pixel(current_coord);
    int dim = Planning_House_Pixel(techno, unknown_rgb_ab6841, 0x50);
    int bright = Planning_House_Pixel(techno, unknown_rgb_ab6849, 100);
    bool has_nodes = (!token ? 0 : token->Nodes.Count()) > 0;
    bool current_order = Planning_Current_Order(techno, has_nodes);
    int dot;
    int thickness = 4;
    int color = Planning_Techno_Color(techno);
    if (token && (token->Owner->Mission == 28 || token->Owner->MissionQueue == 28)) {
        EventClass event = token->Event;
        CoordStruct coord = Planning_Event_Or_Object_Coord(event, techno);
        Point2D end = Planning_Coord_Pixel(coord);
        dot = dim;
        Planning_Fill_Dot(start, dot);
        Planning_Draw_Dashed_Thick_Line(start, end, 3, dot);
        int frame = ((PlanningEventTargets const &)event).unknown_61a2d0();
        if (frame != -1) Planning_Draw_Midpoint_Shape(start, end, dot, color, frame);
        Planning_Fill_Dot(end, dot);
        return;
    }
    if (current_order) {
        bool alternate = techno->unknown_slot270();
        bool secondary = techno->unknown_slot271();
        CoordStruct coord = techno->unknown_slot241();
        Point2D end = Planning_Coord_Pixel(coord);
        if (techno->unknown_76) {
            Planning_Fill_Dot(end, dim);
            return;
        }
        Planning_Fill_Dot(start, dim);
        Planning_Draw_Dashed_Thick_Line(start, end, 3, dim);
        int frame = -1;
        if (!alternate || secondary) {
            if (techno->Get_Mission() == 11) frame = PlanningEventTargets::Unknown_006F6760();
        } else if (techno->unknown_slot272() == 29) frame = PlanningEventTargets::Unknown_006F6750();
        if (frame != -1) Planning_Draw_Midpoint_Shape(start, end, dim, color, frame);
        Planning_Fill_Dot(end, dim);
        if (secondary) {
            CoordStruct next_coord = techno->unknown_slot273();
            Point2D next = Planning_Coord_Pixel(next_coord);
            Planning_Draw_Dashed_Thick_Line(end, next, 3, dim);
            if (techno->unknown_slot272() == 29) {
                frame = PlanningEventTargets::Unknown_006F6750();
                if (frame != -1) Planning_Draw_Midpoint_Shape(end, next, dim, color, frame);
            }
            Planning_Fill_Dot(next, dim);
            return;
        }
        if (!has_nodes) return;
        start = end;
        thickness = 3;
    } else if (!has_nodes) {
        return;
    }
    {
        PlanningNodeClass *node = token->Node_At(0);
        Point2D end = node->Pixel();
        PlanningBranchClass *branch = node->Branch_For(techno);
        if (branch) end = Planning_Event_Pixel(&branch->Event, techno);
        dot = Planning_Reached(token, 0) ? bright : dim;
        Planning_Fill_Dot(start, dot);
        Planning_Draw_Dashed_Thick_Line(start, end, thickness, dot);
        if (branch) Planning_Fill_Dot(end, dot);
        EventClass event = node->Event_For(techno);
        int frame = ((PlanningEventTargets const &)event).unknown_61a2d0();
        if (frame != -1) Planning_Draw_Midpoint_Shape(start, end, dot, color, frame);
    }
}


// void __fastcall Planning_Draw_Node_619420(PlanningNodeClass *node) (0x00619420)
// Draws a highlighted node: thick lines to each branch target (a square on
// the midpoint of a long one), the thick loop line and the outlined marker.
// The coordinate an event without a target reports ((-1, -1, -1)). Hypothesis: a
// default-constructed type whose ctor forwards to CoordStruct's, so the ctor call
// 0x0041B630 stays one inline level down and == reads the temporary directly.
struct Planning_No_Coord : CoordStruct {
    Planning_No_Coord() : CoordStruct(-1, -1, -1) {}
};

// The house colour of a node's first member. Hypothesis: a level of its own, so
// Member_Owner and Techno_Color are depth-two calls at both Draw_Node rows.
inline int Planning_Node_Color(PlanningNodeClass *node)
{
    return Planning_Techno_Color(node->Member_Owner(0));
}

void __fastcall Planning_Draw_Node_619420(PlanningNodeClass *node)
{
    Point2D pixel = node->Pixel();
    int frame = -1;
    if (node->Members.Count() > 0) {
        EventClass event = node->Member_Event(0);
        frame = ((PlanningEventTargets &)event).unknown_61a2d0();
    }
    int color = Planning_Node_Color(node);
    int count = node->Branches.Count();
    for (int index = 0; index < count; index++) {
        CoordStruct coord = node->Branch_Coord(index);
        Point2D start = Planning_Coord_Pixel(coord);
        if (coord == Planning_No_Coord()) {
            PlanningBranchClass *branch = node->Branches[index];
            start = Planning_Event_Pixel(&branch->Event, node->Branch_Owner(index));
        }
        Planning_Draw_Thick_Line(start, pixel, 4, color);
        if (frame != -1) {
            Planning_Draw_Midpoint(start, pixel, color);
        }
    }
    if (node->unknown_6114e0()) {
        CoordStruct coord = node->Own_Branch_Coord();
        Point2D start = Planning_Coord_Pixel(coord);
        if (coord == Planning_No_Coord()) {
            start = Planning_Event_Pixel(&node->Branch.Event, node->First_Counted_Owner());
        }
        Planning_Draw_Thick_Line(start, pixel, 4, color);
        if (frame != -1) {
            Planning_Draw_Midpoint(start, pixel, color);
        }
    }
    node->Draw_Marker(color);
}


// CoordStruct __fastcall Planning_Event_Or_Object_Coord(EventClass const &event, AbstractClass *object) (0x00619810)
// The coordinate an event points at, or the object's when the event points
// nowhere.

CoordStruct __fastcall Planning_Event_Or_Object_Coord(EventClass const &event, AbstractClass *object)
{
    CoordStruct coord = Planning_Event_Coord(event);
    if (coord == UnknownCoord434820(-1, -1, -1) && object) {
        coord = object->unknown_slot22();
    }
    return coord;
}


// void __fastcall Planning_Draw_Node_619870(PlanningNodeClass *node) (0x00619870)
// Draws a node: the lines to each branch target, the loop line back to the
// first node and the marker; an order with a cursor shape marks each line.
// Uses Planning_No_Coord and Planning_Node_Color from the 0x00619420 row.
void __fastcall Planning_Draw_Midpoint_Shape(Point2D start, Point2D end, int color, int fill, int frame);

void __fastcall Planning_Draw_Node_619870(PlanningNodeClass *node)
{
    Point2D pixel = node->Pixel();
    int frame = -1;
    if (node->Members.Count() > 0) {
        EventClass event = node->Member_Event(0);
        frame = ((PlanningEventTargets &)event).unknown_61a2d0();
    }
    int color = Planning_Node_Color(node);
    int count = node->Branches.Count();
    for (int index = 0; index < count; index++) {
        int branch_color = node->Branch_Color(index);
        CoordStruct coord = node->Branch_Coord(index);
        Point2D start = Planning_Coord_Pixel(coord);
        if (coord == Planning_No_Coord()) {
            PlanningBranchClass *branch = node->Branches[index];
            start = Planning_Event_Pixel(&branch->Event, node->Branch_Owner(index));
        }
        Planning_Draw_Dashed_Thick_Line(start, pixel, 4, branch_color);
        if (frame != -1) {
            Planning_Draw_Midpoint_Shape(start, pixel, branch_color, color, frame);
        }
    }
    if (node->unknown_6114e0()) {
        int loop_color = node->Loop_Color();
        CoordStruct coord = node->Own_Branch_Coord();
        Point2D start = Planning_Coord_Pixel(coord);
        if (coord == Planning_No_Coord()) {
            TechnoClass *owner = node->First_Counted_Owner();
            start = Planning_Event_Pixel(&node->Branch.Event, owner);
        }
        Planning_Draw_Dashed_Thick_Line(start, pixel, 4, loop_color);
        if (frame != -1) {
            Planning_Draw_Midpoint_Shape(start, pixel, loop_color, color, frame);
        }
    }
    node->Fill_Marker(node->Color());
}


// void PlanningNodeClass::Fill_Marker_Colored() (0x00619BD0)
// Fills the node marker in the node's own colour.
// void __fastcall Planning_Fill_Node(PlanningNodeClass *node) (0x00619BD0; no direct callers, name a hypothesis)
// Fills the node's marker in its own colour.

void __fastcall Planning_Fill_Node(PlanningNodeClass *node)
{
    node->Fill_Marker(node->Color());
}


// int __fastcall Planning_Dim_Color(TechnoClass *techno) (0x00619CA0)
// The screen colour of a techno's house, moved 0x50/256 toward the fade colour.

int __fastcall Planning_Dim_Color(TechnoClass *techno)
{
    RGBClass color = Planning_House_Color(techno);
    color.Adjust(0x50, unknown_rgb_ab6841);
    return ((color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
         | ((color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
         | ((color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
}


// int __fastcall Planning_Bright_Color(TechnoClass *techno) (0x00619D50)
// The screen colour of a techno's house, moved 100/256 toward the fade colour.

int __fastcall Planning_Bright_Color(TechnoClass *techno)
{
    RGBClass color = Planning_House_Color(techno);
    color.Adjust(100, unknown_rgb_ab6849);
    return ((color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
         | ((color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
         | ((color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
}


// int __fastcall Planning_Adjusted_Pixel(RGBClass const &color, RGBClass const &target, int ratio) (0x00619E00)
// The screen pixel of a colour moved ratio/256 of the way toward target.
#include "shared/rgb_class.hpp"

#pragma auto_inline(off)
int __fastcall Planning_Adjusted_Pixel(RGBClass const &color, RGBClass const &target, int ratio)
{
    RGBClass adjusted = color;
    adjusted.Adjust(ratio, target);
    return Build_Hicolor_Pixel(adjusted.Red, adjusted.Green, adjusted.Blue);
}
#pragma auto_inline(on)


// int __fastcall Planning_Techno_Color(TechnoClass *techno) (0x00619E80)
// The colour a techno's marker is drawn in: the plain one, or the lighter
// one for the remembered node's members (or an unmoved selected techno).

int __fastcall Planning_Techno_Color(TechnoClass *techno)
{
    int plain = Planning_Pixel(&unknown_rgb_ab6841);
    if (!techno) return plain;
    int lighter = Planning_Pixel(&unknown_rgb_ab6849);
    if (unknown_node_a766b0) {
        if (unknown_node_a766b0->Member_Index(techno) != -1) return lighter;
        return plain;
    }
    if (techno->unknown_77) return lighter;
    return plain;
}


// Point2D __fastcall Planning_Event_Pixel(EventClass const *event, AbstractClass *object) (0x00619FA0)
// The screen pixel an event points at (the object's when it points nowhere);
// (-1, -1) without an event.

Point2D __fastcall Planning_Event_Pixel(EventClass const *event, AbstractClass *object)
{
    Point2D pixel(-1, -1);
    if (event) {
        EventClass copy = *event;
        CoordStruct coord = Planning_Event_Or_Object_Coord(copy, object);
        pixel = Planning_Coord_Pixel(coord);
    }
    return pixel;
}


// int PlanningNodeClass::Highlight_Color() (0x0061A060)
// The colour the node's marker is drawn in: the plain one, or the lighter
// one when the node's first member is the one the planning mode is about to
// click (the remembered node's member, or an unmoved selected techno).

int PlanningNodeClass::Highlight_Color()
{
    TechnoClass *owner = Members[0]->Owner;
    int result = Build_Hicolor_Pixel(unknown_rgb_ab6841.Red, unknown_rgb_ab6841.Green, unknown_rgb_ab6841.Blue);
    if (owner) {
        int lighter = Build_Hicolor_Pixel(unknown_rgb_ab6849.Red, unknown_rgb_ab6849.Green, unknown_rgb_ab6849.Blue);
        if (unknown_node_a766b0) {
            if (unknown_node_a766b0->Has_Member(owner)) {
                result = lighter;
            }
        } else if (owner->unknown_77) {
            result = lighter;
        }
    }
    return result;
}


// void __fastcall Planning_Draw_Dot(Point2D center, int color) (0x0061A190)
// Outlines a 6 x 6 square centred on a screen point.

void __fastcall Planning_Draw_Dot(Point2D center, int color)
{
    ::Rect view;
    view = Get_Title_Screen_Area_006f1f30();
    ::Rect square;
    square = Planning_Centered_Rect(center, 6);
    unknown_surface_839964->Draw_Rect(view, square, color);
}


// void __fastcall Planning_Fill_Dot(Point2D center, int color) (0x0061A210)
// Fills the inside of a 6 x 6 square centred on a screen point.

void __fastcall Planning_Fill_Dot(Point2D center, int color)
{
    Point2D point = center;
    ::Rect view;
    view = Get_Title_Screen_Area_006f1f30();
    ::Rect square;
    square = Planning_Centered_Rect(point, 6);
    Planning_Inflate(square, -1);
    Surface *surface = unknown_surface_839964;
    surface->Fill_Rect(view, square, color);
}


// int PlanningEventTargets::unknown_61a2d0() const (0x0061A2D0)

int PlanningEventTargets::unknown_61a2d0() const
{
    int kind = (signed char)unknown_0c;
    if (kind == 0x0B) {
        return Unknown_006F6760();
    }
    if (kind == 0x1D) {
        return Unknown_006F6750();
    }
    return -1;
}


// void __fastcall Planning_Draw_Midpoint(Point2D start, Point2D end, int color) (0x0061A2F0)
// When two screen points are more than 36 pixels apart, outlines a 21 x 21
// square centred on their midpoint.

inline int Point2D::Length() const
{
    int x = X;
    int y = Y;
    return (int)unknown_sqrt_4ba410((double)y * y + (double)x * x);
}

void __fastcall Planning_Draw_Midpoint(Point2D start, Point2D end, int color)
{
    int length = (start - end).Length();
    if (length < 37) return;
    int x = (start.X + end.X) / 2 - 10;
    int y = (start.Y + end.Y) / 2 - 10;
    ::Rect view = Get_Title_Screen_Area_006f1f30();
    ::Rect square(x, y, 21, 21);
    unknown_surface_839964->Draw_Rect(view, square, color);
}


// void __fastcall Planning_Draw_Midpoint_Shape(Point2D start, Point2D end, int color, int fill, int frame) (0x0061A3C0)
// When two screen points are more than 36 pixels apart, outlines and fills a
// square on their midpoint and draws a mouse shape frame centred in it.

class ConvertClass;
struct ShapeFileStruct;
extern ConvertClass *unknown_drawer_8323c8;
void __fastcall CC_Draw_Shape(Surface *surface, ConvertClass *drawer, ShapeFileStruct const *shape, int frame,
                              Point2D const &point, Rect const &window, int flags, unsigned char const *unknown8,
                              int unknown9, int unknown10, int brightness, ShapeFileStruct const *unknown12,
                              int unknown13, Point2D offset);

void __fastcall Planning_Draw_Midpoint_Shape(Point2D start, Point2D end, int color, int fill, int frame)
{
    if ((start - end).Length() >= 37) {
        int x = (start.X + end.X) / 2;
        int y = (start.Y + end.Y) / 2;
        int left = x - 10;
        int top = y - 10;
        ::Rect view = Get_Title_Screen_Area_006f1f30();
        ::Rect square(left, top, 21, 21);
        Planning_Inflate(square, -1);
        unknown_surface_839964->Draw_Rect(view, square, color);
        Planning_Inflate(square, -1);
        unknown_surface_839964->Fill_Rect(view, square, fill);
        Point2D point(left, top);
        ::Rect window = view;
        MouseShapeHeader const *shape = MouseClass::MouseShapes;
        int width = shape->Width;
        int height = shape->Height;
        point.X = x - width / 2 - 1;
        point.Y = y - height / 2;
        CC_Draw_Shape(unknown_surface_839964, unknown_drawer_8323c8, (ShapeFileStruct const *)shape, frame, point, window,
                      0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }
}


// void __fastcall Planning_Draw_Line(Point2D start, Point2D end, int offset, int color, bool dashed) (0x0061A5A0)
// Draws a line between two screen points, shifted sideways by offset pixels
// (in y for a mostly horizontal line, else in x) and clipped to the tactical
// area; dashed lines crawl with the clock.
#include "shared/rect.hpp"

extern int unknown_int_839638;
extern int unknown_int_83963c;
extern bool unknown_pattern_7eb9f8[16];

void __fastcall Planning_Draw_Line(Point2D start, Point2D end, int offset, int color, bool dashed)
{
    bool steep = abs(end.X - start.X) > abs(end.Y - start.Y);
    int sideways = steep ? 0 : offset;
    int lengthways = steep ? offset : 0;
    start.X += sideways;
    start.Y += lengthways;
    end.X += sideways;
    end.Y += lengthways;
    Rect area(unknown_int_839630, unknown_int_839634, unknown_int_839638, unknown_int_83963c);
    if (Clip_Line(start, end, area)) {
        if (dashed) {
            int phase = (-(int)timeGetTime() >> 5) & 0xF;
            unknown_surface_839964->Draw_Dashed_Line(start, end, color, unknown_pattern_7eb9f8, phase);
        } else {
            unknown_surface_839964->Draw_Line(start, end, color);
        }
    }
}


// void __fastcall Planning_Draw_Thick_Line(Point2D start, Point2D end, int thickness, int color) (0x0061A6A0)
// Draws thickness solid lines side by side, centred on the line from start to end.

void __fastcall Planning_Draw_Thick_Line(Point2D start, Point2D end, int thickness, int color)
{
    int last = thickness / 2;
    for (int offset = last - thickness + 1; offset <= last; offset++) {
        Planning_Draw_Line(start, end, offset, color, false);
    }
}


// void __fastcall Planning_Draw_Dashed_Thick_Line(Point2D start, Point2D end, int thickness, int color) (0x0061A6F0)
// Draws thickness - 2 dashed lines side by side (one at least), centred on the line from start to end.

void __fastcall Planning_Draw_Dashed_Thick_Line(Point2D start, Point2D end, int thickness, int color)
{
    int count = thickness - 2;
    int first;
    int last;
    if (count <= 0) {
        first = 0;
        last = 0;
    } else {
        last = count / 2;
        first = last - count + 1;
    }
    for (int offset = first; offset <= last; offset++) {
        Planning_Draw_Line(start, end, offset, color, true);
    }
}


// PlanningNodeClass scalar deleting destructor (0x0061A750)
// Compiler generated for the out-of-line `delete node` in
// ~PlanningTokenClass 0x00612D20 (destructor inlined).


// int __fastcall Planning_Pixel(RGBClass const *color) (0x0061A860)
// The screen pixel of a palette colour.
#include "shared/hicolor_pixel.hpp"
int __fastcall Planning_Pixel(RGBClass const *color)
{
    RGBClass const &rgb = *color;
    int pixel = RGB_To_Pixel(rgb.Red, rgb.Green, rgb.Blue);
    return pixel;
}


// bool CoordStruct::operator==(CoordStruct const &coord) const (0x0061A8B0)

bool CoordStruct::operator==(CoordStruct const &coord) const
{
    return X == coord.X && Y == coord.Y && Z == coord.Z;
}


// 0x0061a8e0: function not yet identified
//
// Body: 58 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: undefined4 __thiscall unknown_rect_contains_0061a8e0(int *param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_0061a8e0 {

struct Owner_0061a8e0 { unsigned int unknown_rect_contains_0061a8e0(int *point); };

unsigned int Owner_0061a8e0::unknown_rect_contains_0061a8e0(int *point)
{
    int * rect = (int *)this;
    if ((*point >= *rect) && (*point < rect[2] + *rect)) {
        if ((point[1] >= rect[1]) && (point[1] < rect[3] + rect[1])) {
            return 1;
        }
    }
    return 0;
}

}  // namespace seed_0061a8e0


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<PlanningMemberClass *>::Clear (0x0061A920; DynamicVectorClass<PlanningMemberClass *> vtable 0x007a82bc slot 3).
// DynamicVectorClass<PlanningMemberClass *>::ID_pointer (0x0061A950; DynamicVectorClass<PlanningMemberClass *> vtable 0x007a82bc slot 5).
// VectorClass<PlanningMemberClass *>::Fetch (0x0061A970; DynamicVectorClass<PlanningMemberClass *> vtable 0x007a82bc slot 6).
// DynamicVectorClass<PlanningBranchClass *>::Clear (0x0061A980; DynamicVectorClass<PlanningBranchClass *> vtable 0x007a829c slot 3).
// DynamicVectorClass<PlanningBranchClass *>::ID_pointer (0x0061A9B0; DynamicVectorClass<PlanningBranchClass *> vtable 0x007a829c slot 5).
// VectorClass<PlanningBranchClass *>::Fetch (0x0061A9D0; VectorClass<PlanningBranchClass *> vtable 0x007a82fc slot 6).
// DynamicVectorClass<PlanningNodeClass *>::Clear (0x0061A9E0; DynamicVectorClass<PlanningNodeClass *> vtable 0x007a821c slot 3).
// DynamicVectorClass<PlanningNodeClass *>::ID_pointer (0x0061AA10; DynamicVectorClass<PlanningNodeClass *> vtable 0x007a821c slot 5).
// VectorClass<PlanningNodeClass *>::Fetch (0x0061AA30; VectorClass<PlanningNodeClass *> vtable 0x007a823c slot 6).
// DynamicVectorClass<EventClass *>::Clear (0x0061AA40; DynamicVectorClass<EventClass *> vtable 0x007a81dc slot 3).
// DynamicVectorClass<EventClass *>::ID_pointer (0x0061AA70; DynamicVectorClass<EventClass *> vtable 0x007a81dc slot 5).
// VectorClass<EventClass *>::Fetch (0x0061AA90; VectorClass<EventClass *> vtable 0x007a81fc slot 6).
// DynamicVectorClass<PlanningTokenClass *>::Clear (0x0061AAA0; DynamicVectorClass<PlanningTokenClass *> vtable 0x007a825c slot 3).
// DynamicVectorClass<PlanningTokenClass *>::ID_pointer (0x0061AAD0; DynamicVectorClass<PlanningTokenClass *> vtable 0x007a825c slot 5).
// VectorClass<PlanningTokenClass *>::Fetch (0x0061AAF0; VectorClass<PlanningTokenClass *> vtable 0x007a827c slot 6).
// VectorClass<PlanningMemberClass *>::operator equal (0x0061AB00; VectorClass<PlanningMemberClass *> vtable 0x007a82dc slot 1).
// VectorClass<PlanningMemberClass *>::Resize (0x0061AB40; VectorClass<PlanningMemberClass *> vtable 0x007a82dc slot 2).


// 0x0061abf0: function not yet identified
//
// Owner: VectorClass<PlanningMemberClass *>, virtual slot 3 (vtable +0xc) (from RTTI/vtables).
// Body: 39 bytes (frozen inventory bound).
// Referenced from 3 sites (data, unconditional call).
// Ghidra prototype: void __fastcall Clear_0061abf0(int param_1)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0061abf0 {

extern void  operator_delete_00781d3b(unsigned int);

void __fastcall Clear_0061abf0(int self)
{
    if ((*(int *)(self + 4) != 0) && (*(char *)(self + 0xd) != '\0')) {
        operator_delete_00781d3b((unsigned int)(*(int *)(self + 4)));
        *(unsigned int *)(self + 4) = 0;
    }
    *(unsigned char *)(self + 0xd) = 0;
    *(unsigned int *)(self + 8) = 0;
    return;
}

}  // namespace seed_0061abf0


// VectorClass<PlanningMemberClass *>::ID(PlanningMemberClass * const &) (0x0061AC20)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (population-2430).


// 0x0061ac60: function not yet identified
//
// Body: 27 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: uint __thiscall unknown_index_of_0061ac60(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0061ac60 {

struct Owner_0061ac60 { unsigned int unknown_index_of_0061ac60(int item); };

unsigned int Owner_0061ac60::unknown_index_of_0061ac60(int item)
{
    int self = (int)this;
    if (*(char *)(self + 0xc) == '\0') {
        return 0;
    }
    return (unsigned int)(item - *(int *)(self + 4)) >> 2;
}

}  // namespace seed_0061ac60


// Original region: 0x0061ac80 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_0061ac80(unsigned int byte_count);
extern "C" unsigned char base_dispatch_0061ac80[];
extern "C" unsigned char derived_dispatch_0061ac80[];

class WordStorage_0061ac80 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_0061ac80* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_0061ac80_size_check[(sizeof(WordStorage_0061ac80) == 24) ? 1 : -1];

WordStorage_0061ac80* WordStorage_0061ac80::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_0061ac80;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_0061ac80(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_0061ac80;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


// Original region: 0x0061af90 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_0061af90(unsigned int byte_count);
extern "C" unsigned char base_dispatch_0061af90[];
extern "C" unsigned char derived_dispatch_0061af90[];

class WordStorage_0061af90 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_0061af90* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_0061af90_size_check[(sizeof(WordStorage_0061af90) == 24) ? 1 : -1];

WordStorage_0061af90* WordStorage_0061af90::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_0061af90;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_0061af90(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_0061af90;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


extern "C" unsigned char vtable_61ba10[]; extern "C" void __cdecl release_buffer_61ba10(void*); extern "C" void __cdecl release_object_61ba10(void*);
class Leaf_61ba10 { public: void* get(unsigned int flags); };
void* Leaf_61ba10::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61ba10; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61ba10((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61ba10(this); return this; }


extern "C" unsigned char vtable_61ba60[]; extern "C" void __cdecl release_buffer_61ba60(void*); extern "C" void __cdecl release_object_61ba60(void*);
class Leaf_61ba60 { public: void* get(unsigned int flags); };
void* Leaf_61ba60::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61ba60; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61ba60((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61ba60(this); return this; }


extern "C" unsigned char vtable_61bab0[]; extern "C" void __cdecl release_buffer_61bab0(void*); extern "C" void __cdecl release_object_61bab0(void*);
class Leaf_61bab0 { public: void* get(unsigned int flags); };
void* Leaf_61bab0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61bab0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61bab0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61bab0(this); return this; }


extern "C" unsigned char vtable_61bb00[]; extern "C" void __cdecl release_buffer_61bb00(void*); extern "C" void __cdecl release_object_61bb00(void*);
class Leaf_61bb00 { public: void* get(unsigned int flags); };
void* Leaf_61bb00::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61bb00; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61bb00((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61bb00(this); return this; }


extern "C" unsigned char vtable_61bb50[]; extern "C" void __cdecl release_buffer_61bb50(void*); extern "C" void __cdecl release_object_61bb50(void*);
class Leaf_61bb50 { public: void* get(unsigned int flags); };
void* Leaf_61bb50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61bb50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61bb50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61bb50(this); return this; }


extern "C" unsigned char vtable_61bba0[]; extern "C" void __cdecl release_buffer_61bba0(void*); extern "C" void __cdecl release_object_61bba0(void*);
class Leaf_61bba0 { public: void* get(unsigned int flags); };
void* Leaf_61bba0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61bba0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61bba0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61bba0(this); return this; }


extern "C" unsigned char vtable_61bbf0[]; extern "C" void __cdecl release_buffer_61bbf0(void*); extern "C" void __cdecl release_object_61bbf0(void*);
class Leaf_61bbf0 { public: void* get(unsigned int flags); };
void* Leaf_61bbf0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61bbf0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61bbf0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61bbf0(this); return this; }


extern "C" unsigned char vtable_61bc40[]; extern "C" void __cdecl release_buffer_61bc40(void*); extern "C" void __cdecl release_object_61bc40(void*);
class Leaf_61bc40 { public: void* get(unsigned int flags); };
void* Leaf_61bc40::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61bc40; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61bc40((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61bc40(this); return this; }


extern "C" unsigned char vtable_61bc90[]; extern "C" void __cdecl release_buffer_61bc90(void*); extern "C" void __cdecl release_object_61bc90(void*);
class Leaf_61bc90 { public: void* get(unsigned int flags); };
void* Leaf_61bc90::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61bc90; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61bc90((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61bc90(this); return this; }


extern "C" unsigned char vtable_61bce0[]; extern "C" void __cdecl release_buffer_61bce0(void*); extern "C" void __cdecl release_object_61bce0(void*);
class Leaf_61bce0 { public: void* get(unsigned int flags); };
void* Leaf_61bce0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_61bce0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_61bce0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_61bce0(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<PlanningNodeClass *>::Add (0x0061B300): emitted by this unit's use of its list.
// DynamicVectorClass<PlanningTokenClass *>::Add (0x0061B940): emitted by this unit's use of its list.


// DynamicVectorClass<PlanningMemberClass *>::Delete(int) (0x0061BD30): emitted with the planning
// unit's Delete(PlanningMemberClass * const &) 0x0061AD90 (shared/vector_class.hpp); no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<PlanningMemberClass *>::Resize (0x0061ACE0; DynamicVectorClass<PlanningMemberClass *> vtable 0x007a82bc slot 2).
// DynamicVectorClass<PlanningMemberClass *>::ID_object (0x0061ADE0; DynamicVectorClass<PlanningMemberClass *> vtable 0x007a82bc slot 4).
// VectorClass<PlanningBranchClass *>::operator equal (0x0061AE10; VectorClass<PlanningBranchClass *> vtable 0x007a82fc slot 1).
// VectorClass<PlanningBranchClass *>::Resize (0x0061AE50; VectorClass<PlanningBranchClass *> vtable 0x007a82fc slot 2).
// VectorClass<PlanningBranchClass *>::Clear (0x0061AF00; VectorClass<PlanningBranchClass *> vtable 0x007a82fc slot 3).
// VectorClass<PlanningBranchClass *>::ID_object (0x0061AF30; VectorClass<PlanningBranchClass *> vtable 0x007a82fc slot 4).
// VectorClass<PlanningBranchClass *>::ID_pointer (0x0061AF70; VectorClass<PlanningBranchClass *> vtable 0x007a82fc slot 5).
// DynamicVectorClass<PlanningBranchClass *>::Resize (0x0061AFF0; DynamicVectorClass<PlanningBranchClass *> vtable 0x007a829c slot 2).
// DynamicVectorClass<PlanningBranchClass *>::ID_object (0x0061B0A0; DynamicVectorClass<PlanningBranchClass *> vtable 0x007a829c slot 4).
// VectorClass<PlanningNodeClass *>::operator equal (0x0061B0D0; VectorClass<PlanningNodeClass *> vtable 0x007a823c slot 1).
// VectorClass<PlanningNodeClass *>::Resize (0x0061B110; VectorClass<PlanningNodeClass *> vtable 0x007a823c slot 2).
// VectorClass<PlanningNodeClass *>::Clear (0x0061B1C0; VectorClass<PlanningNodeClass *> vtable 0x007a823c slot 3).
// VectorClass<PlanningNodeClass *>::ID_object (0x0061B1F0; VectorClass<PlanningNodeClass *> vtable 0x007a823c slot 4).
// VectorClass<PlanningNodeClass *>::ID_pointer (0x0061B230; VectorClass<PlanningNodeClass *> vtable 0x007a823c slot 5).
// DynamicVectorClass<PlanningNodeClass *>::Resize (0x0061B250; DynamicVectorClass<PlanningNodeClass *> vtable 0x007a821c slot 2).
// DynamicVectorClass<PlanningNodeClass *>::Delete(int index) (0x0061B350).
// Instantiated for the planning code (0x00616xxx calls it out of line).
// DynamicVectorClass<PlanningNodeClass *>::Delete(PlanningNodeClass * const &object) (0x0061B390).
// Expands Delete(int); this unit's own copy of Delete(int) is the one at 0x0061B350.
// DynamicVectorClass<PlanningNodeClass *>::ID_object (0x0061B3E0; DynamicVectorClass<PlanningNodeClass *> vtable 0x007a821c slot 4).
// VectorClass<EventClass *>::operator equal (0x0061B410; VectorClass<EventClass *> vtable 0x007a81fc slot 1).
// VectorClass<EventClass *>::Resize (0x0061B450; VectorClass<EventClass *> vtable 0x007a81fc slot 2).
// VectorClass<EventClass *>::Clear (0x0061B500; VectorClass<EventClass *> vtable 0x007a81fc slot 3).
// VectorClass<EventClass *>::ID_object (0x0061B530; VectorClass<EventClass *> vtable 0x007a81fc slot 4).
// VectorClass<EventClass *>::ID_pointer (0x0061B570; VectorClass<EventClass *> vtable 0x007a81fc slot 5).
// DynamicVectorClass<EventClass *>::Resize (0x0061B590; DynamicVectorClass<EventClass *> vtable 0x007a81dc slot 2).
// DynamicVectorClass<EventClass *>::Add (0x0061B640).
// Layout and behaviour: shared/vector_class.hpp. The out-of-line copy of the planning unit
// (0x00614A6A appends to PlanningLists::unknown_list_00a76648).
// DynamicVectorClass<EventClass *>::ID_object (0x0061B690; DynamicVectorClass<EventClass *> vtable 0x007a81dc slot 4).
// bool DynamicVectorClass<TechnoClass *>::Delete(TechnoClass * const &object) (0x0061B6C0).
// Instantiated from shared/vector_class.hpp; called by the planning code (0x00612E46 and three more) for the PlanningLists TechnoClass list (0x00A76700).
// Expands Delete(int); this unit's own copy of Delete(int) is the one at 0x0061BDC0.
// VectorClass<PlanningTokenClass *>::operator equal (0x0061B710; VectorClass<PlanningTokenClass *> vtable 0x007a827c slot 1).
// VectorClass<PlanningTokenClass *>::Resize (0x0061B750; VectorClass<PlanningTokenClass *> vtable 0x007a827c slot 2).
// VectorClass<PlanningTokenClass *>::Clear (0x0061B800; VectorClass<PlanningTokenClass *> vtable 0x007a827c slot 3).
// VectorClass<PlanningTokenClass *>::ID_object (0x0061B830; VectorClass<PlanningTokenClass *> vtable 0x007a827c slot 4).
// VectorClass<PlanningTokenClass *>::ID_pointer (0x0061B870; VectorClass<PlanningTokenClass *> vtable 0x007a827c slot 5).
// DynamicVectorClass<PlanningTokenClass *>::Resize (0x0061B890; DynamicVectorClass<PlanningTokenClass *> vtable 0x007a825c slot 2).
// bool DynamicVectorClass<PlanningTokenClass *>::Delete(PlanningTokenClass * const &object) (0x0061B990).
// Instantiated from shared/vector_class.hpp; called at 0x00613EA5 for the PlanningTokenClass list (0x00A76738).
// Expands Delete(int); this unit's own copy of Delete(int) is the one at 0x0061BE00.
// DynamicVectorClass<PlanningTokenClass *>::ID_object (0x0061B9E0; DynamicVectorClass<PlanningTokenClass *> vtable 0x007a825c slot 4).
// VectorClass<PlanningNodeClass *>::VectorClass(unsigned size, PlanningNodeClass * const *array) (0x0061BD70; stores the
// VectorClass<PlanningNodeClass *> vtable 0x007a823c). Layout and behaviour: shared/vector_class.hpp.
// bool DynamicVectorClass<TechnoClass *>::Delete(int index) (0x0061BDC0).
// Instantiated from shared/vector_class.hpp; emitted identically by the TechnoClass unit, whose references bind it here.
// bool DynamicVectorClass<PlanningTokenClass *>::Delete(int index) (0x0061BE00).
// Instantiated from shared/vector_class.hpp; called at 0x006131FC for the PlanningTokenClass list (0x00A76738).
