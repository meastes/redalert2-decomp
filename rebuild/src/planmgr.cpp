/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the planning mode (waypoint mode): the planned orders of selected objects are kept
**	as nodes along a path per object, drawn on the tactical view and sent together when
**	planning mode ends. TibSun has no such file; the code is the executable's from 0x006101B0.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs
#include "convert.h" // RA2: the includes this file needs
#include "draw.h" // RA2: the includes this file needs
#include "dsurface.h" // RA2: the includes this file needs
#include "rect.h" // RA2: the includes this file needs
#include "shapeset.h" // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs
#include "_tactica.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "house.h" // RA2: the includes this file needs
#include "tactical.h" // RA2: the includes this file needs
#include "techno.h" // RA2: the includes this file needs
#include "foot.h" // RA2: the includes this file needs
#include "event.h" // RA2: the includes this file needs
#include "target.h" // RA2: the includes this file needs
#include "planning.h" // RA2: the includes this file needs
#include <stddef.h> // RA2: the includes this file needs
#undef LoadString
#include "stringtable.h" // RA2: the includes this file needs
#include "voc.h" // RA2: the includes this file needs
#include "_rules.h" // RA2: the includes this file needs
#include "rules.h" // RA2: the includes this file needs
#include "queue.h" // RA2: the includes this file needs
#include "_surface.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include "planmgr.h" // RA2: the includes this file needs
#include "mission.h" // RA2: the includes this file needs
#include "cell.h" // RA2: the includes this file needs
#include "_map.h" // RA2: Map, the mouse of the tactical view
#include "_convert.h" // RA2: MouseDrawer
#include "mouse.h" // RA2: MOUSE_NORMAL
#include "txtlabel2.h"



DynamicVectorClass<EventClass *> PlanningTokenClass::Events; // RA2: 0x00A76648; the planned events held until the end of planning mode
DynamicVectorClass<PlanningNodeClass *> PlanningNodeClass::QueuedNodes; // RA2: 0x00A76758; the nodes queued to be rebuilt
DynamicVectorClass<TechnoClass *> PlanningTokenClass::Technos; // RA2: 0x00A76700; the objects with a planning token
DynamicVectorClass<PlanningNodeClass *> PlanningNodeClass::Unknown1; // RA2: 0x00A76628
DynamicVectorClass<PlanningNodeClass *> PlanningNodeClass::DrawnNodes; // RA2: 0x00A766D8; the nodes drawn on the tactical view
DynamicVectorClass<PlanningTokenClass *> PlanningTokenClass::Array; // RA2: 0x00A76738; every planning token
INoticeSink * PlanningSink; // RA2: 0x00A76680
int PlanningUnitCounts[8]; // RA2: 0x00A76684
PlanningNodeClass * PlanningSelectedNode; // RA2: 0x00A766B0
int PlanningHelpCounts[5]; // RA2: 0x00A766B4
bool PlanningRefusalShown; // RA2: 0x00A766C8
PlanningNodeClass * PlanningPickedNode; // RA2: 0x00A766F8
int PlanningBlockedNode; // RA2: 0x00A766FC
Point2D PlanningMouseDown; // RA2: 0x00A76728
int PlanningPendingRefusal; // RA2: 0x00A76750
bool PlanningDragging; // RA2: 0x00A76770
bool PlanningLoopClosed; // RA2: 0x00A76788
PlanningNodeClass * PlanningHoverNode; // RA2: 0x00A7678C
bool PlanningCommandFlag; // RA2: 0x00A76790
bool PlanningTearDown; // RA2: 0x00A767A5
bool PlanningBusy; // RA2: 0x00A767A6
bool PlanningNodeClass::PlanningModeActive; // RA2: 0x00A767A4



int __fastcall Planning_House(TechnoClass * techno) // RA2: 0x00610770; the house a techno plans for: its own, the player's when its house is human controlled
{
	HouseClass * house = techno->House;
	int index = house->HeapID;
	if (house->Is_Player_Control() && PlayerPtr) {
		index = PlayerPtr->HeapID;
	}
	return index;
}

Point2D __fastcall Planning_Coord_Pixel(Coord const & coord) // RA2: 0x006107A0; a coordinate as a pixel of the tactical view moved to the screen
{
	Point2D pixel;
	TacticalMap->Coord_To_Pixel(coord, pixel);
	pixel.X += TacticalRect.X;
	pixel.Y += TacticalRect.Y;
	return pixel;
}

PlanningMemberClass::PlanningMemberClass(void) : Owner(0), Packet(0), Branch(-1), Active(false) // RA2: 0x006107E0
{
}

void PlanningMemberClass::Set_Owner(TechnoClass * owner) // RA2: 0x00610800
{
	Owner = owner;
}

void PlanningMemberClass::Set_Packet(EventClass * packet) // RA2: 0x00610810
{
	Packet = packet;
}

void PlanningMemberClass::Set_Branch(int branch) // RA2: 0x00610820
{
	Branch = branch;
}

void PlanningMemberClass::Set_Active(bool active) // RA2: 0x00610830
{
	Active = active;
}

TechnoClass * PlanningMemberClass::Get_Owner(void) // RA2: 0x00610840
{
	return Owner;
}

EventClass * PlanningMemberClass::Get_Packet(void) // RA2: 0x00610850
{
	return Packet;
}

int PlanningMemberClass::Get_Branch(void) // RA2: 0x00610860
{
	return Branch;
}

bool PlanningMemberClass::Get_Active(void) // RA2: 0x00610870
{
	return Active;
}

PlanningBranchClass::PlanningBranchClass(void) : Count(0), Color(-1) // RA2: 0x00610880
{
}

void PlanningBranchClass::Set_Event(EventClass const & event) // RA2: 0x006108A0
{
	Event = event;
}

void PlanningBranchClass::Set_Count(int count) // RA2: 0x006108C0
{
	Count = count;
}

void PlanningBranchClass::Set_Color(int color) // RA2: 0x006108D0
{
	Color = color;
}

void PlanningBranchClass::Clear_Count(void) // RA2: 0x006108E0
{
	Count = 0;
}

int PlanningBranchClass::Inc_Count(void) // RA2: 0x006108F0
{
	return ++Count;
}

int PlanningBranchClass::Dec_Count(void) // RA2: 0x00610900
{
	return --Count;
}

EventClass PlanningBranchClass::Get_Event(void) // RA2: 0x00610910
{
	return Event;
}

int PlanningBranchClass::Get_Count(void) // RA2: 0x00610930
{
	return Count;
}

int PlanningBranchClass::Get_Color(void) // RA2: 0x00610940
{
	return Color;
}

Coord PlanningBranchClass::Get_Coord(void) // RA2: 0x00610950
{
	return Coord(Planning_Event_Coord(Event));
}

Coord __fastcall Planning_Event_Coord(EventClass const & event) // RA2: 0x00610980; where the order of a mission event points (target, destination or follow), (-1, -1, -1) for none
{
	if (event.Type == EventClass::MEGAMISSION) {
		AbstractClass * object;
		if (event.Data.MegaMission.Target.Is_Valid()) {
			object = event.Data.MegaMission.Target.As_Abstract();
			if (object) {
				return object->As_Coord();
			}
		} else if (event.Data.MegaMission.Follow.Is_Valid()) {
			object = event.Data.MegaMission.Follow.As_Abstract();
			if (object) {
				return object->As_Coord();
			}
		} else if (event.Data.MegaMission.Destination.Is_Valid()) {
			object = event.Data.MegaMission.Destination.As_Abstract();
			if (object) {
				return object->As_Coord();
			}
		}
	}
	return Coord(-1, -1, -1);
}

Point2D PlanningBranchClass::Get_Pixel(void) // RA2: 0x00610A20
{
	Coord coord = Planning_Event_Coord(Event);
	return Planning_Coord_Pixel(coord);
}

PlanningNodeClass::PlanningNodeClass(int id) : Id(id), Queued(false), LoopBranchIndex(-1), Color(-1) // RA2: 0x00610A80
{
}

PlanningNodeClass::~PlanningNodeClass() // RA2: 0x00610AF0
{
	Clear();
	Planning_Unlist_Node(this);
}

int PlanningNodeClass::Get_Id(void) // RA2: 0x00610C10
{
	return Id;
}

int PlanningNodeClass::Get_Count(void) // RA2: 0x00610C20
{
	return PlanningMembers.Count();
}

int PlanningNodeClass::Index_In(TechnoClass * techno) // RA2: 0x00610C30
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	return token->PlanningNodes.ID(this);
}

PlanningNodeClass * PlanningNodeClass::Prev_Node(TechnoClass * techno) // RA2: 0x00610C60
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	int index = token->PlanningNodes.ID(this);
	if (index == -1) return 0;
	int count = token->PlanningNodes.Count();
	if (count == 1) return this;
	index--;
	if (index < 0) index = count - 1;
	return token->Node_At(index);
}

PlanningNodeClass * PlanningNodeClass::Next_Node(TechnoClass * techno) // RA2: 0x00610CE0
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	int index = token->PlanningNodes.ID(this);
	if (index == -1) return 0;
	int count = token->PlanningNodes.Count();
	if (count == 1) return this;
	index++;
	if (index >= count) index = 0;
	return token->Node_At(index);
}

void PlanningNodeClass::Add_Member(TechnoClass * techno, EventClass const * event) // RA2: 0x00610D60
{
	PlanningMemberClass * member = new PlanningMemberClass;
	member->Set_Owner(techno);
	EventClass * packet = new EventClass;
	*packet = *event;
	member->Set_Packet(packet);
	PlanningMembers.Add(member);
	techno->Get_Planning_Token()->Add_Node(this);
	Assign_Branch(member);
}

void PlanningNodeClass::Remove_Member(TechnoClass * techno) // RA2: 0x00610E70
{
	PlanningMemberClass * member = Get_Member(techno);
	if (member) {
		Detach(member);
		delete member->Get_Packet();
		PlanningMembers.Delete(member);
		delete member;
		if (Get_Count() == 0) {
			delete this;
		}
	}
}

int PlanningNodeClass::Find_Member(TechnoClass * techno) // RA2: 0x00611050
{
	int result = -1;
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		if (PlanningMembers[i]->Get_Owner() == techno) {
			result = i;
			break;
		}
	}
	return result;
}

TechnoClass * PlanningNodeClass::Member_Owner(int index) // RA2: 0x00611090
{
	PlanningMemberClass * member = PlanningMembers[index];
	return member->Get_Owner();
}

Rect __fastcall Planning_Centered_Rect(Point2D center, int size);

EventClass PlanningNodeClass::Get_Event(int index) // RA2: 0x006110A0
{
	PlanningMemberClass * member = PlanningMembers[index];
	return *member->Get_Packet();
}

EventClass PlanningNodeClass::Get_Event_For(TechnoClass * techno) // RA2: 0x006110D0
{
	return *Get_Member(techno)->Packet;
}

bool PlanningNodeClass::Has_Member(TechnoClass * techno) // RA2: 0x00611120
{
	return Find_Member(techno) != -1;
}

HouseClass * PlanningNodeClass::Lead_House(void) // RA2: 0x00611170
{
	return PlanningMembers[0]->Owner->House;
}

Coord PlanningNodeClass::Lead_Coord(void) // RA2: 0x00611180
{
	Coord coord = Planning_Event_Coord(*PlanningMembers[0]->Get_Packet());
	return coord;
}

Point2D PlanningNodeClass::Lead_Pixel(void) // RA2: 0x006111C0
{
	Coord coord = Lead_Coord();
	Point2D pixel = Planning_Coord_Pixel(coord);
	return pixel;
}

bool PlanningNodeClass::Hit_Test(int x, int y) // RA2: 0x00611230
{
	return Lead_Box_Rect().Is_Point_Within(Point2D(x, y));
}

Rect __fastcall Planning_Centered_Rect(Point2D center, int size) // RA2: 0x006112D0; a size by size rectangle centered on a pixel
{
	int offset = -(size / 2);
	Point2D corner;
	corner = center;
	corner.X += offset;
	corner.Y += offset;
	Rect rect(corner.X, corner.Y, size, size);
	return rect;
}

bool PlanningNodeClass::Lead_At(Coord coord) // RA2: 0x00611310
{
	Coord c;
	c = Lead_Coord();
	return c == coord;
}

Rect PlanningNodeClass::Lead_Rect(void) // RA2: 0x00611360
{
	Point2D pixel;
	pixel = Lead_Pixel();
	return Planning_Centered_Rect(pixel, 8);
}

bool PlanningNodeClass::Is_Past_Start(void) // RA2: 0x006113F0
{
	if (PlanningMembers.Count() == 0) return false;
	PlanningTokenClass * token = PlanningMembers[0]->Owner->Get_Planning_Token();
	int index = token->PlanningNodes.ID(this);
	int start = token->StepsToClosedLoop;
	return start != -1 && index >= start;
}

void PlanningNodeClass::Promote_Events(void) // RA2: 0x00611440
{
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		EventClass * packet = PlanningMembers[i]->Packet;
		if (packet->Data.MegaMission.Mission == MISSION_MOVE) packet->Data.MegaMission.Mission = MISSION_ATTACK_MOVE;
	}
}

PlanningMemberClass * PlanningNodeClass::Get_Member(TechnoClass * techno) // RA2: 0x00611470
{
	int index = Find_Member(techno);
	if (index == -1) {
		return NULL;
	}
	return PlanningMembers[index];
}

int PlanningNodeClass::Branch_Count(void) // RA2: 0x006114C0
{
	return PlanningBranches.Count();
}

int PlanningNodeClass::Has_Loop(void) // RA2: 0x006114D0
{
	return LoopBranch.Count > 0;
}

int PlanningNodeClass::Is_Loop_Alone(void) // RA2: 0x006114E0
{
	if (LoopBranch.Count > 0 && LoopBranchIndex == -1) return 1;
	return 0;
}

TechnoClass * PlanningNodeClass::Owner_At_Branch(int branch) // RA2: 0x00611500
{
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		PlanningMemberClass * m = PlanningMembers[i];
		if (m->Branch == branch) return m->Owner;
	}
	return 0;
}

TechnoClass * PlanningNodeClass::Active_Owner(void) // RA2: 0x00611530
{
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		PlanningMemberClass * m = PlanningMembers[i];
		if (m->Active) return m->Owner;
	}
	return 0;
}

PlanningBranchClass * PlanningNodeClass::Get_Branch(int index) // RA2: 0x00611560
{
	return PlanningBranches[index];
}

PlanningBranchClass * PlanningNodeClass::Branch_Of(TechnoClass * techno) // RA2: 0x00611570
{
	PlanningMemberClass * member = Get_Member(techno);
	int branch = member->Branch;
	PlanningBranchClass * result = 0;
	if (branch != -1) result = PlanningBranches[branch];
	return result;
}

EventClass * PlanningNodeClass::Loop_Event(void) // RA2: 0x006115C0
{
	return &LoopBranch.Event;
}

Coord PlanningNodeClass::Branch_Coord(int index) // RA2: 0x006115D0
{
	Coord coord = Get_Branch(index)->Get_Coord();
	return coord;
}

Coord PlanningNodeClass::Loop_Coord(void) // RA2: 0x00611610
{
	Coord coord = LoopBranch.Get_Coord();
	return coord;
}

Point2D PlanningNodeClass::Branch_Pixel(int index) // RA2: 0x00611640
{
	return PlanningBranches[index]->Get_Pixel();
}

Point2D PlanningNodeClass::Loop_Pixel(void) // RA2: 0x006116B0
{
	return LoopBranch.Get_Pixel();
}

void PlanningNodeClass::Set_Queued(void) // RA2: 0x00611710
{
	Queued = true;
}

bool PlanningNodeClass::Get_Queued(void) // RA2: 0x00611720
{
	return Queued;
}

void PlanningNodeClass::Rebuild(void) // RA2: 0x00611730
{
	Clear();
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		Assign_Branch(PlanningMembers[i]);
	}
	Update_Active();
	Queued = false;
}

void PlanningNodeClass::Update_Active(void) // RA2: 0x00611770
{
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		PlanningMemberClass * member = PlanningMembers[i];
		TechnoClass * owner = member->Owner;
		if (Is_Loop_Start(owner)) {
			member->Active = true;
			if (LoopBranch.Inc_Count() == 1) {
				LoopBranch.Set_Event(owner->Get_Planning_Token()->Last_Node()->Get_Event_For(owner));
			}
		}
	}
	LoopBranchIndex = -1;
	if (LoopBranch.Get_Count() != 0) {
		EventClass self = LoopBranch.Get_Event();
		int branches = PlanningBranches.Count();
		int b = 0;
		if (branches > 0) {
			do {
				EventClass other = PlanningBranches[b]->Get_Event();
				Coord a = Planning_Event_Coord(self);
				Coord c = Planning_Event_Coord(other);
				if (a == c) {
					LoopBranchIndex = b;
					return;
				}
				b++;
			} while (b < branches);
		}
	}
}

void PlanningNodeClass::Dump(void) // RA2: 0x006118E0
{
	int branch_count = PlanningBranches.Count();
	for (int branch_index = 0; branch_index < branch_count; ++branch_index) {
		PlanningBranchClass * branch = PlanningBranches[branch_index];
		int count = branch->Get_Count();
		EventClass event = branch->Get_Event();
		Coord origin = Planning_Event_Coord(event);
		DebugString("Branch [%d];  Member count[%d];  Origin event location[%d, %d, %d]\n",
			branch_index, count, origin.X, origin.Y, origin.Z);
		int member_count = PlanningMembers.Count();
		for (int member_index = 0; member_index < member_count; ++member_index) {
			PlanningMemberClass * member = PlanningMembers[member_index];
			if (member->Get_Branch() == branch_index) {
				TechnoClass * owner = member->Get_Owner();
				char const * name = owner->Class_Of()->IniName;
				Cell cell = owner->Get_Cell();
				DebugString("  Member [%s];  Cell location[%d, %d]\n", name, cell.X, cell.Y);
			}
		}
	}
	int count = LoopBranch.Get_Count();
	if (count > 0) {
		EventClass event = LoopBranch.Get_Event();
		Coord origin = Planning_Event_Coord(event);
		DebugString("Loop Branch [%d];  Member count[%d];  Origin event location[%d, %d, %d]\n",
			LoopBranchIndex, count, origin.X, origin.Y, origin.Z);
		int member_count = PlanningMembers.Count();
		for (int member_index = 0; member_index < member_count; ++member_index) {
			PlanningMemberClass * member = PlanningMembers[member_index];
			if (member->Get_Active()) {
				TechnoClass * owner = member->Get_Owner();
				char const * name = owner->Class_Of()->IniName;
				Cell cell = owner->Get_Cell();
				DebugString("  Member [%s];  Cell location[%d, %d]\n", name, cell.X, cell.Y);
			}
		}
	}
}

int PlanningNodeClass::Find_Branch(TechnoClass * techno) // RA2: 0x00611A80
{
	PlanningNodeClass * prev = Prev_Node(techno);
	if (prev == techno->Get_Planning_Token()->Last_Node()) return -1;
	EventClass event = prev->Get_Event_For(techno);
	int count = PlanningBranches.Count();
	if (count == 0) return -1;
	Coord origin = Planning_Event_Coord(event);
	for (int i = 0; i < count; i++) {
		PlanningBranchClass * branch = PlanningBranches[i];
		if (branch->Get_Count()) {
			EventClass other = branch->Get_Event();
			Coord c = Planning_Event_Coord(other);
			if (origin == c) return i;
		}
	}
	return -1;
}

static inline void Find_Loop_Branch(PlanningNodeClass * node) // RA2: no function of its own; expanded in Assign_Branch and Locate_Loop_Branch
{
	node->LoopBranchIndex = -1;
	if (node->LoopBranch.Get_Count()) {
		EventClass self = node->LoopBranch.Get_Event();
		int branches = node->PlanningBranches.Count();
		for (int b = 0; b < branches; b++) {
			EventClass other = node->PlanningBranches[b]->Get_Event();
			Coord a = Planning_Event_Coord(self);
			Coord c = Planning_Event_Coord(other);
			if (a == c) {
				node->LoopBranchIndex = b;
				return;
			}
		}
	}
}

int PlanningNodeClass::Assign_Branch(PlanningMemberClass * member) // RA2: 0x00611BD0
{
	TechnoClass * owner = member->Get_Owner();
	int index = Find_Branch(owner);
	if (index == -1) {
		PlanningNodeClass * previous = Prev_Node(owner);
		PlanningNodeClass * last = owner->Get_Planning_Token()->Last_Node();
		if (previous == last) {
			member->Set_Branch(-1);
			return -1;
		}
		EventClass event = previous->Get_Event_For(owner);
		PlanningBranchClass * branch = new PlanningBranchClass;
		branch->Set_Event(event);
		branch->Inc_Count();
		PlanningBranches.Add(branch);
		int count = PlanningBranches.Count() - 1;
		if (Is_Loop_Alone()) {
			Find_Loop_Branch(this);
		}
		Reset_Colors(false);
		member->Set_Branch(count);
		return count;
	} else {
		PlanningBranchClass * branch = PlanningBranches[index];
		branch->Set_Color(-1);
		Reset_Colors(false);
		branch->Inc_Count();
		member->Set_Branch(index);
		return index;
	}
}

inline void PlanningNodeClass::Detach(PlanningMemberClass * member) // RA2: 0x00611E20; expanded in Remove_Member and kept out of line too
{
	if (member->Get_Active()) {
		Deactivate(member);
	}
	if (member->Get_Branch() != -1) {
		PlanningBranchClass * branch = PlanningBranches[member->Get_Branch()];
		branch->Set_Color(-1);
		Reset_Colors(false);
		if (branch->Dec_Count() == 0) {
			Planning_Queue_Node(this);
		}
	}
	member->Set_Branch(-1);
}

void PlanningNodeClass::Clear(void) // RA2: 0x00611EE0
{
	int branches = PlanningBranches.Count();
	for (int i = 0; i < branches; i++) {
		delete PlanningBranches[i];
	}
	PlanningBranches.Clear();
	int members = PlanningMembers.Count();
	for (int j = 0; j < members; j++) {
		PlanningMemberClass * member = PlanningMembers[j];
		member->Set_Branch(-1);
		member->Set_Active(false);
	}
	LoopBranch.Clear_Count();
	LoopBranchIndex = -1;
}

bool PlanningNodeClass::Is_Loop_Start(TechnoClass * techno) // RA2: 0x00611F60
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	return token->PlanningNodes.ID(this) == token->StepsToClosedLoop;
}

void PlanningNodeClass::Deactivate(PlanningMemberClass * member) // RA2: 0x00611FA0
{
	member->Set_Active(false);
	if (LoopBranch.Dec_Count() == 0) {
		LoopBranchIndex = -1;
	}
}

void PlanningNodeClass::Locate_Loop_Branch(void) // RA2: 0x00611FD0
{
	Find_Loop_Branch(this);
}

void PlanningNodeClass::Select_All(void) // RA2: 0x00612090
{
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		TechnoClass * owner = PlanningMembers[i]->Owner;
		if (!owner->IsSelected) {
			owner->Select();
		}
	}
}

void PlanningNodeClass::Unselect_All(void) // RA2: 0x006120C0
{
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		TechnoClass * owner = PlanningMembers[i]->Owner;
		if (owner->IsSelected) {
			owner->Unselect();
		}
	}
}


void __fastcall Planning_Add_To_Drawn(PlanningNodeClass * node); // RA2: 0x006157C0, defined below

void PlanningNodeClass::Draw_Lead_Box(int color) // RA2: 0x006120F0; the marker is listed among the drawn nodes when it is on the view
{
	Rect clip = Get_Title_Screen_Area();
	Rect const & rect = Lead_Rect();
	CompositeSurface->Draw_Rect(clip, rect, color);
	if (clip.Is_Overlapping(rect)) {
		Planning_Add_To_Drawn(this);
	}
}

// RA2: the one-pixel shrink is ra2-decomp's unit-local twin helper (no function of its own; expanded in the lead box members)
static inline void Shrink_Rect(Rect & r)
{
	if (r.Width <= -2) goto none;
	if (r.Height <= -2) {
	none:
		r.Set(0, 0, 0, 0);
	} else {
		r.X += 1;
		r.Y += 1;
		r.Width -= 2;
		r.Height -= 2;
	}
}

void PlanningNodeClass::Fill_Lead_Box(int color) // RA2: 0x00612230
{
	Rect rect;
	Rect clip = Get_Title_Screen_Area();
	rect = Lead_Rect();
	Shrink_Rect(rect);
	CompositeSurface->Fill_Rect(clip, rect, color);
}

void PlanningNodeClass::Frame_Lead_Box(int color) // RA2: 0x00612300
{
	Rect clip = Get_Title_Screen_Area();
	Rect rect = Lead_Box_Rect();
	for (int i = 0; i < 3; i++) {
		CompositeSurface->Draw_Rect(clip, rect, color);
		Shrink_Rect(rect);
	}
}

void PlanningNodeClass::Frame_Lead_Box_Outer(int color) // RA2: 0x00612420
{
	Rect clip = Get_Title_Screen_Area();
	Rect rect = Lead_Box_Rect2();
	for (int i = 0; i < 3; i++) {
		CompositeSurface->Draw_Rect(clip, rect, color);
		Shrink_Rect(rect);
	}
}

void __fastcall Planning_Inflate(Rect & rect, int amount) // RA2: 0x00612570; grows a rectangle by amount on every side; one that would vanish becomes empty
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

static inline RGBClass Planning_House_Color(TechnoClass * techno) // RA2: no function of its own; the remap color of the house a techno plans for
{
	return Houses[Planning_House(techno)]->RemapColorRGB;
}

// RA2: the path tint is ra2-decomp's twin text
enum WaypointTint { WAYPOINT_NORMAL, WAYPOINT_BRIGHT };

static inline int Unit_Path_Tint(TechnoClass * techno, WaypointTint tint)
{
	int slot = Planning_House(techno);
	RGBClass color = Houses[slot]->RemapColorRGB;
	if (tint == WAYPOINT_NORMAL) {
		return Planning_Adjusted_Pixel(color, BlackColor, 80);
	}
	return Planning_Adjusted_Pixel(color, WhiteColor, 100);
}

int PlanningNodeClass::Node_Color(void) // RA2: 0x006125D0; the color of a planning node
{
	int result = Color;
	if (result == -1) {
		TechnoClass * owner = PlanningMembers[0]->Owner;
		result = Unit_Path_Tint(owner, WAYPOINT_NORMAL);
		Color = result;
		int count = PlanningBranches.Count();
		for (int i = 0; i < count; i++) {
			if (PlanningBranches[i]->Get_Color() == result)
				goto done;
		}
		if (count == 0) {
			PlanningTokenClass * token = owner->Get_Planning_Token();
			bool past = token->Reached(token->Node_Index(this));
			if (past) {
				Color = Unit_Path_Tint(owner, WAYPOINT_BRIGHT);
			}
		} else {
			Color = Unit_Path_Tint(owner, WAYPOINT_BRIGHT);
		}
		result = Color;
	}
done:
	return result;
}

int PlanningNodeClass::Branch_Color(int index) // RA2: 0x006127E0
{
	PlanningBranchClass * branch = PlanningBranches[index];
	if (branch->Color != -1) return branch->Color;
	TechnoClass * last_owner = NULL;
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; ++i) {
		PlanningMemberClass * member = PlanningMembers[i];
		if (member->Branch == index) {
			TechnoClass * owner = member->Owner;
			PlanningTokenClass * token = owner->Get_Planning_Token();
			if (!token->Reached(token->Node_Index(this))) {
				RGBClass color = Houses[Planning_House(owner)]->RemapColorRGB;
				color.Adjust(0x50, BlackColor);
				return branch->Color = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue());
			}
			last_owner = owner;
		}
	}
	if (last_owner == NULL) {
		return -1;
	} else {
		RGBClass color = Houses[Planning_House(last_owner)]->RemapColorRGB;
		color.Adjust(0x64, WhiteColor);
		return branch->Color = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue());
	}
}

int PlanningNodeClass::Loop_Color(void) // RA2: 0x006129B0
{
	if (LoopBranch.Color != -1) return LoopBranch.Color;
	TechnoClass * last_owner = NULL;
	int count = PlanningMembers.Count();
	for (int i = 0; i < count; ++i) {
		PlanningMemberClass * member = PlanningMembers[i];
		if (member->Active) {
			TechnoClass * owner = member->Owner;
			PlanningTokenClass * token = owner->Get_Planning_Token();
			if (!token->Get_F98()) {
				RGBClass color = Houses[Planning_House(owner)]->RemapColorRGB;
				color.Adjust(0x50, BlackColor);
				return LoopBranch.Color = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue());
			}
			last_owner = owner;
		}
	}
	if (last_owner == NULL) {
		return -1;
	} else {
		RGBClass color = Houses[Planning_House(last_owner)]->RemapColorRGB;
		color.Adjust(0x64, WhiteColor);
		return LoopBranch.Color = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue());
	}
}

void PlanningNodeClass::Reset_Colors(bool all) // RA2: 0x00612B50
{
	Color = -1;
	if (all) {
		int count = PlanningBranches.Count();
		int i = 0;
		if (count > 0) do {
				PlanningBranches[i]->Set_Color(-1);
				i++;
			} while (i < count);
		LoopBranch.Set_Color(-1);
	}
}

Rect PlanningNodeClass::Lead_Box_Rect(void) // RA2: 0x00612B90
{
	Rect rect = Lead_Rect();
	Planning_Inflate(rect, 4);
	return rect;
}

Rect PlanningNodeClass::Lead_Box_Rect2(void) // RA2: 0x00612C30
{
	Rect rect = Lead_Box_Rect();
	Planning_Inflate(rect, 4);
	return rect;
}

PlanningTokenClass::PlanningTokenClass(TechnoClass * owner) : F1C(false), F8C(-1), ClosedLoopNodeCount(-1), StepsToClosedLoop(-1), F98(false), F99(false) // RA2: 0x00612CC0
{
	OwnerUnit = owner;
}

PlanningTokenClass::~PlanningTokenClass() // RA2: 0x00612D20
{
	if (OwnerUnit) {
		OwnerUnit->Set_Planning_Token(0);
	}
	Planning_Unlist_Token(this);
	Cancel_Owner();
}

TechnoClass * PlanningTokenClass::Get_Owner(void) // RA2: 0x00610840
{
	return OwnerUnit;
}

inline void PlanningTokenClass::Cancel_Owner(void) // RA2: 0x00612EC0; expanded in the destructor and kept out of line too
{
	int count = PlanningNodes.Count();
	bool none = (count == 0);
	for (int i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodes[i];
		node->Remove_Member(Get_Owner());
	}
	if (!none) {
		Planning_Unlist_Techno(OwnerUnit);
	}
	PlanningNodes.Clear();
	F8C = -1;
	ClosedLoopNodeCount = -1;
	StepsToClosedLoop = -1;
	F98 = false;
	F99 = false;
}

void PlanningTokenClass::Destroy(void) // RA2: 0x006130B0
{
	Cancel_Owner();
	OwnerUnit = 0;
	delete this;
}

void PlanningTokenClass::Set_F1C(void) // RA2: 0x00611710
{
	F1C = true;
}

void PlanningTokenClass::Clear_F1C(void) // RA2: 0x006132D0
{
	F1C = false;
}

bool PlanningTokenClass::Get_F1C(void) // RA2: 0x00611720
{
	return F1C;
}

bool PlanningTokenClass::Owner_Is_Mission(void) // RA2: 0x006132F0
{
	return OwnerUnit->CurrentMission == MISSION_WAIT || OwnerUnit->MissionQueue == MISSION_WAIT;
}

bool PlanningTokenClass::Has_Started(void) // RA2: 0x00613310
{
	return PlanningNodes.Count() > 0 && (F8C == -1 || F8C > 0);
}

static inline void Finish_Consumed_Step(PlanningTokenClass * token) // RA2: no function of its own; the end of Pop_Front
{
	token->F8C = max(-1, token->F8C - 1);
	if (token->Node_Count() == 0) {
		Planning_Unlist_Techno(token->Get_Owner());
	}
}

EventClass PlanningTokenClass::Pop_Front(void) // RA2: 0x00613330
{
	PlanningNodeClass * node = PlanningNodes[0];
	EventClass packet = Event_At(0);
	Popped = packet;
	PlanningNodes.Delete_Index(0);
	if (Loop_Start_Zero()) {
		F99 = true;
		PlanningNodes.Add(node);
		Planning_Queue_Node(node);
	} else {
		node->Remove_Member(OwnerUnit);
		if (Loop_Start_Positive()) {
			Dec_Loop_Start();
		}
	}
	Finish_Consumed_Step(this);
	return packet;
}

void PlanningTokenClass::Share_Range(TechnoClass * techno, int start, int count) // RA2: 0x00613550
{
	if (techno->Get_Planning_Token()) {
		for (int i = start; i < count + start; i++) {
			PlanningNodeClass * node = PlanningNodes[i];
			EventClass event = node->Get_Event_For(OwnerUnit);
			event.Data.Target.Whom = TargetClass(techno);
			node->Add_Member(techno, &event);
		}
	}
}

void PlanningTokenClass::Share_From(TechnoClass * techno, int start) // RA2: 0x00613720
{
	int count = PlanningNodes.Count() - start;
	Share_Range(techno, start, count);
}

void PlanningTokenClass::Remove_Node_At(int index) // RA2: 0x006138F0
{
	if (PlanningNodes.Count() == 1) {
		F8C = -1;
	} else if (F8C > 0 && index < F8C) {
		F8C--;
	}
	if (StepsToClosedLoop >= 0) {
		if (index < StepsToClosedLoop) {
			StepsToClosedLoop--;
		} else {
			ClosedLoopNodeCount--;
			if (ClosedLoopNodeCount < 3) {
				StepsToClosedLoop = -1;
				ClosedLoopNodeCount = 0;
			}
		}
	}
	PlanningNodeClass * node = PlanningNodes[index];
	node->Remove_Member(OwnerUnit);
	PlanningNodes.Delete_Index(index);
}

void PlanningTokenClass::Reset_Node_Colors(void) // RA2: 0x00613A80
{
	if (F98) {
		PlanningNodeClass * node = Loop_Node();
		if (node != NULL) node->Reset_Colors(true);
		F98 = false;
	}
	int start = F8C == -1 ? 0 : F8C;
	int count = PlanningNodes.Count();
	for (int index = start; index < count; index++) {
		PlanningNodeClass * node = PlanningNodes[index];
		if (node != NULL) node->Reset_Colors(true);
	}
	F8C = -1;
}

int PlanningTokenClass::Get_F8C(void) // RA2: 0x00613B30
{
	return F8C;
}

bool PlanningTokenClass::Reached(int index) // RA2: 0x00613B40
{
	if (F8C == -1) return false;
	return index >= F8C;
}

int PlanningTokenClass::Node_Count(void) // RA2: 0x00613B60
{
	return PlanningNodes.Count();
}

int PlanningTokenClass::Current_Or_Count(void) // RA2: 0x00613B70
{
	if (F8C == -1) return PlanningNodes.Count();
	return F8C;
}

int PlanningTokenClass::Remaining(void) // RA2: 0x00613B80
{
	if (F8C == -1) return 0;
	return PlanningNodes.Count() - F8C;
}

void PlanningTokenClass::Add_Node(PlanningNodeClass * node) // RA2: 0x00613BA0
{
	PlanningNodes.Add(node);
	if (F8C == -1) {
		F8C = PlanningNodes.Count() - 1;
	}
}

PlanningNodeClass * PlanningTokenClass::Node_At(int index) // RA2: 0x00613C00
{
	int count = PlanningNodes.Count();
	if (count == 0) return 0;
	if (index >= count) {
		int start = StepsToClosedLoop;
		if (Loop_Start_Valid()) {
			index = (index - start) % ClosedLoopNodeCount + start;
		} else {
			return 0;
		}
	}
	if (index < 0 || index >= count) return 0;
	return PlanningNodes[index];
}

PlanningNodeClass * PlanningTokenClass::Last_Node(void) // RA2: 0x00613C50
{
	int index = PlanningNodes.Count() - 1;
	if (index < 0) return 0;
	return PlanningNodes[index];
}

bool PlanningTokenClass::Has_Node(PlanningNodeClass * node) // RA2: 0x00613C60
{
	return PlanningNodes.ID(node) != -1;
}

int PlanningTokenClass::Node_Index(PlanningNodeClass * node) // RA2: 0x00613C80
{
	return PlanningNodes.ID(node);
}

EventClass PlanningTokenClass::Event_At(int index) // RA2: 0x00613CA0
{
	PlanningNodeClass * node = PlanningNodes[index];
	TechnoClass * owner = OwnerUnit;
	return *node->Get_Member(owner)->Packet;
}

EventClass PlanningTokenClass::Last_Popped(void) // RA2: 0x00613D00
{
	return Popped;
}

PlanningNodeClass * PlanningTokenClass::Loop_Node(void) // RA2: 0x00613D20
{
	if (StepsToClosedLoop == -1) return 0;
	return PlanningNodes[StepsToClosedLoop];
}

void PlanningTokenClass::Set_Loop(int count, int start) // RA2: 0x00613D40
{
	ClosedLoopNodeCount = count;
	StepsToClosedLoop = start;
	F98 = true;
}

int PlanningTokenClass::Get_Loop_Count(void) // RA2: 0x00613D60
{
	return ClosedLoopNodeCount;
}

int PlanningTokenClass::Get_Loop_Start(void) // RA2: 0x00613D70
{
	return StepsToClosedLoop;
}

bool PlanningTokenClass::Loop_Start_Valid(void) // RA2: 0x00613D80
{
	return StepsToClosedLoop > 0 || StepsToClosedLoop == 0;
}

bool PlanningTokenClass::Loop_Start_Positive(void) // RA2: 0x00613DA0
{
	return StepsToClosedLoop > 0;
}

bool PlanningTokenClass::Loop_Start_Zero(void) // RA2: 0x00613DB0
{
	return StepsToClosedLoop == 0;
}

bool PlanningTokenClass::Get_F98(void) // RA2: 0x00613DC0
{
	return F98;
}

bool PlanningTokenClass::Get_F99(void) // RA2: 0x00613DD0
{
	return F99;
}

void PlanningTokenClass::Dec_Loop_Start(void) // RA2: 0x00613DE0
{
	StepsToClosedLoop--;
	StepsToClosedLoop = (StepsToClosedLoop <= 0) ? 0 : StepsToClosedLoop;
}

void Planning_Set_Sink(INoticeSink * sink) // RA2: 0x00613E00
{
	PlanningSink = sink;
}

void __cdecl Planning_Reset(void) // RA2: 0x00613E10
{
	PlanningNodeClass::PlanningModeActive = false;
	PlanningTearDown = false;
	PlanningHoverNode = NULL;
	PlanningPickedNode = NULL;
	PlanningSelectedNode = NULL;
	PlanningMouseDown = Point2D(0, 0);
	PlanningDragging = false;
}

void __cdecl Planning_Reset_Wrapper(void) // RA2: 0x00613E40
{
	Planning_Reset_State();
}

static inline void Reset_Planning_Command_Flags(void)
{
	PlanningCommandFlag = false;
	PlanningLoopClosed = false;
}

void __cdecl Planning_Reset_State(void) // RA2: 0x00613E50; deletes every token and clears every list and flag
{
	int count = PlanningTokenClass::Array.Count();
	PlanningTearDown = true;
	PlanningNodeClass::PlanningModeActive = false;
	PlanningBusy = false;
	for (int i = 0; i < count; i++) {
		PlanningTokenClass * token = PlanningTokenClass::Array[i];
		delete token;
	}
	memset(PlanningUnitCounts, 0, sizeof(PlanningUnitCounts));
	PlanningTokenClass::Technos.Clear();
	PlanningTokenClass::Array.Clear();
	PlanningNodeClass::Unknown1.Clear();
	PlanningNodeClass::DrawnNodes.Clear();
	PlanningHoverNode = NULL;
	Reset_Planning_Command_Flags();
	PlanningBlockedNode = 0;
	PlanningRefusalShown = false;
	PlanningTearDown = false;
	PlanningPendingRefusal = -1;
	memset(PlanningHelpCounts, 0, sizeof(PlanningHelpCounts));
}

void __fastcall Planning_Update(void) // RA2: 0x00614010; the per frame update: drops the steps whose target is gone and shows a pending refusal
{
	Planning_Hover();
	if (PlanningSelectedNode && CurrentObject.Count() > 0) {
		PlanningSelectedNode = 0;
	}
	int count = PlanningTokenClass::Array.Count();
	int i = 0;
	if (count > 0) {
		do {
			PlanningTokenClass * token = PlanningTokenClass::Array[i];
			TechnoClass * owner = token->OwnerUnit;
			for (int j = token->PlanningNodes.Count() - 1; j >= 0; --j) {
				EventClass event = token->Event_At(j);
				bool remove = true;
				if (event.Type == EventClass::MEGAMISSION) {
					if (event.Data.MegaMission.Target.Is_Valid()) {
						remove = event.Data.MegaMission.Target.As_Abstract() == 0;
					} else if (event.Data.MegaMission.Destination.Is_Valid()) {
						remove = event.Data.MegaMission.Destination.As_Abstract() == 0;
					}
				}
				if (!remove) {
					continue;
				}
				PlanningNodeClass * node = token->Node_At(j);
				PlanningNodeClass * next = node->Next_Node(owner);
				Planning_Queue_Node(next);
				token->Remove_Node_At(j);
			}
		} while (count > ++i);
	}
	Planning_Process_Queue();
	int refusal = PlanningPendingRefusal;
	if (refusal != -1) {
		Planning_Refuse_Command(refusal);
		PlanningPendingRefusal = -1;
	}
	Reset_Planning_Command_Flags();
	PlanningRefusalShown = false;
}

void __fastcall Planning_Free_Nodes(void) // RA2: 0x006142F0; runs the planned events still held and frees them
{
	for (int i = PlanningTokenClass::Events.Count() - 1; i >= 0; i--) {
		EventClass * event = PlanningTokenClass::Events[i];
		Enqueue_Event(event);
		PlanningTokenClass::Events.Delete_Index(i);
		delete event;
	}
}

void __fastcall Planning_Store_Event(EventClass const & source) // RA2: 0x00614350
{
	EventClass * event = new EventClass;
	*event = source;
	if (event->Type == EventClass::MEGAMISSION) {
		event->Data.MegaMission.IsPlanningEvent = 2;
	}
	PlanningTokenClass::Events.Add(event);
}

void __fastcall Planning_Unlist_Node(PlanningNodeClass * node) // RA2: 0x006143E0
{
	PlanningNodeClass::QueuedNodes.Delete(node);
	PlanningNodeClass::Unknown1.Delete(node);
	PlanningNodeClass::DrawnNodes.Delete(node);
	if (PlanningHoverNode == node) {
		PlanningHoverNode = NULL;
	}
	if (PlanningPickedNode == node) {
		PlanningPickedNode = NULL;
	}
	if (PlanningSelectedNode == node) {
		PlanningSelectedNode = NULL;
	}
}

void __fastcall Planning_Unlist_Token(PlanningTokenClass * token) // RA2: 0x006144F0
{
	if (!PlanningTearDown) {
		PlanningTokenClass::Array.Delete(token);
	}
}

void __fastcall Planning_Unlist_Techno(TechnoClass * techno) // RA2: 0x00614550
{
	int house = Planning_House(techno);
	Planning_Uncount(house);
	PlanningTokenClass::Technos.Delete(techno);
}

void TechnoClass::Select_Planning_Members(void) // RA2: 0x006145E0
{
	if (!PlanningBusy) {
		PlanningBusy = true;
		if (PlanningNodeClass::PlanningModeActive) {
			PlanningTokenClass * token = Get_Planning_Token();
			if (token) {
				PlanningNodeClass * node = token->Last_Node();
				if (node) {
					node->Select_All();
				}
			}
		}
		PlanningBusy = false;
	}
}

void __fastcall Planning_Queue_Node(PlanningNodeClass * node) // RA2: 0x00614650
{
	if (node && !node->Get_Queued()) {
		node->Set_Queued();
		PlanningNodeClass::QueuedNodes.Add(node);
	}
}

void __cdecl Planning_Process_Queue(void) // RA2: 0x006146C0
{
	int count = PlanningNodeClass::QueuedNodes.Count();
	for (int i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodeClass::QueuedNodes[i];
		node->Rebuild();
	}
	PlanningNodeClass::QueuedNodes.Clear();
}

void __cdecl Planning_Begin(void) // RA2: 0x00614760; starts planning mode: sound and a notice to the sink
{
	if (!PlanningNodeClass::PlanningModeActive) {
		PlanningNodeClass::PlanningModeActive = true;
		VocClass::PlayGlobal(Rule->StartPlanningModeSound, 1.0f, 0x2000);
		if (PlanningSink) {
			PlanningSink->Unknown_Virtual_0(0x11C7, 0);
		}
	}
}

void __cdecl Planning_End(void) // RA2: 0x006147B0; ends planning mode: sends the commit event, sound and a notice to the sink
{
	if (PlanningNodeClass::PlanningModeActive) {
		EventClass event(PlayerPtr->HeapID, EventClass::PLAN_COMMIT);
		Add_Outgoing_Event(event);
		PlanningHoverNode = NULL;
		PlanningPickedNode = NULL;
		PlanningSelectedNode = NULL;
		PlanningNodeClass::PlanningModeActive = false;
		VocClass::PlayGlobal(Rule->EndPlanningModeSound, 1.0f, 0x2000);
		if (PlanningSink) {
			PlanningSink->Unknown_Virtual_0(0x11C8, 0);
		}
	}
}

bool Planning_Is_Active(void) // RA2: 0x00614840
{
	return PlanningNodeClass::PlanningModeActive;
}

void __fastcall Commit_Plans(int house) // RA2: 0x00614850; for every object of a house: starts the path again when it is not under way and its mission allows it; the expansion of TechnoClass::TryNextPlanningTokenNode (0x00615360) and of Planning_Mission_Allows stand in it
{
	int count = Technos.Count();
	for (int i = 0; i < count; i++) {
		TechnoClass * techno = Technos[i];
		if (house == Planning_House(techno)) {
			PlanningTokenClass * token = techno->Get_Planning_Token();
			if (token != NULL) {
				bool started = token->Has_Started();
				token->Reset_Node_Colors();
				bool permitted = Planning_Mission_Allows(techno);
				if (!started && permitted) {
					techno->TryNextPlanningTokenNode();
				}
			}
		}
	}
}

void PlanningNodeClass::Send_Delete_Event(void) // RA2: 0x00614AA0
{
	EventClass event = *PlanningMembers[0]->Packet;
	xTargetClass * target;
	if (event.Data.MegaMission.Target.Is_Valid()) {
		target = &event.Data.MegaMission.Target;
	} else if (event.Data.MegaMission.Follow.Is_Valid()) {
		target = &event.Data.MegaMission.Follow;
	} else if (event.Data.MegaMission.Destination.Is_Valid()) {
		target = &event.Data.MegaMission.Destination;
	} else {
		return;
	}
	AbstractClass * object = target->As_Abstract();
	if (object) {
		EventClass remove(PlayerPtr->HeapID, EventClass::PLAN_NODE_DELETE, TargetClass(object));
		Add_Outgoing_Event(remove);
	}
}

bool __fastcall Planning_Allows_Event_Type(int type) // RA2: 0x00614B50; whether an event type may be sent now: no mission events while planning mode is on
{
	if (PlanningNodeClass::PlanningModeActive && type == EventClass::MEGAMISSION) {
		return false;
	}
	return true;
}

void __stdcall Planning_Send_Event(EventClass event) // RA2: 0x00614B70; sends a mission event marked as planned
{
	if (event.Type == EventClass::MEGAMISSION) {
		event.Data.MegaMission.IsPlanningEvent = true;
		Add_Outgoing_Event(event);
	}
}


inline bool __fastcall Planning_Target_Fits(int mission, TargetClass target) // RA2: no function of its own; whether an attack or an area guard order aims at a cell or (for the area guard) at nothing
{
	if (mission == MISSION_ATTACK || mission == MISSION_GUARD_AREA) {
		if ((RTTIType)target == RTTI_CELL) goto fits;
		if (mission == MISSION_GUARD_AREA && (RTTIType)target == RTTI_NONE) {
		fits:
			return true;
		}
	}
	return false;
}

inline int Planning_Token_Index(PlanningTokenClass * token, PlanningNodeClass * node) // RA2: no function of its own; the index of a node in the path of a token
{
	return token->Node_Index(node);
}

int __fastcall Planning_Count(int house); // RA2: 0x00616D50
PlanningNodeClass * __fastcall Planning_Pick_Drawn(Point2D point); // RA2: 0x00616140
int __fastcall Planning_Selection_Node_State(PlanningNodeClass * node); // RA2: 0x00617A10
void __fastcall Planning_Message(char const * label); // RA2: 0x00616FB0
void __fastcall Planning_Refuse(char const * label); // RA2: 0x00616FD0
bool __fastcall Planning_Terminal_Mission(int mission); // RA2: 0x006159C0
bool __fastcall Planning_Cell_Mission(int mission, xTargetClass target); // RA2: 0x006159E0
void __fastcall Planning_Refuse_Order(EventClass const & event); // RA2: 0x00617020
int __fastcall Planning_Event_Result(PlanningTokenClass * token, EventClass const & event); // RA2: 0x006171E0
void __fastcall Planning_Recount(int house); // RA2: 0x00616D00
void __fastcall Planning_Set_Pending_Refusal(int value); // RA2: 0x00617240
PlanningTokenClass * __fastcall Planning_New_Token(TechnoClass * techno); // RA2: 0x00615820
void __fastcall Planning_Close_Loop(TechnoClass * techno, PlanningNodeClass * node); // RA2: 0x006164E0
void __fastcall Planning_Path_Add(TechnoClass * techno, PlanningNodeClass * node); // RA2: 0x00616350
PlanningNodeClass * __fastcall Planning_Find_Node(int house, Coord coord, int id); // RA2: 0x00615FB0
void __fastcall Planning_Cancel_Token(TechnoClass * techno); // RA2: 0x006155E0
bool __fastcall Planning_Same_Last_Node(PlanningTokenClass * first, PlanningTokenClass * second); // RA2: 0x00616BD0
void __fastcall Planning_Set_Selected_Node(PlanningNodeClass * node); // RA2: 0x00617E20

void __fastcall Planning_Execute(EventClass * event) // RA2: 0x00614BA0; carries out a planning event: plan commit restarts the paths of the event's house, plan connect adds the node at the second target's coordinate to the first target's path, plan node delete takes the node at the target's coordinate out of every path (remembering the next node for the player); any other event is an order for its techno
{
	if (event->Type == EventClass::PLAN_COMMIT) {
		Commit_Plans(event->ID);
	} else if (event->Type == EventClass::PLAN_CONNECT) {
		TechnoClass * techno = event->Data.NavCom.Whom.As_Techno();
		if (techno) {
			AbstractClass * object = event->Data.NavCom.Where.As_Abstract();
			if (object) {
				Coord coord = object->As_Coord();
				PlanningNodeClass * node = Planning_Find_Node(event->ID, coord, -1);
				if (node) {
					Planning_Path_Add(techno, node);
				}
			}
		}
	} else if (event->Type == EventClass::PLAN_NODE_DELETE) {
		AbstractClass * object = event->Data.Target.Whom.As_Abstract();
		if (object) {
			Coord coord = object->As_Coord();
			int house = event->ID;
			PlanningNodeClass * node = Planning_Find_Node(house, coord, -1);
			if (node) {
				TechnoClass * owner = node->Member_Owner(0);
				int index = node->Index_In(owner);
				PlanningTokenClass * token = owner->Get_Planning_Token();
				PlanningNodeClass * next = token->Node_At(index + 1);
				Planning_Remove_Node(node);
				if (next && house == PlayerPtr->HeapID) {
					Planning_Set_Selected_Node(next);
				}
			}
		}
	} else {
		TechnoClass * techno = event->Data.Target.Whom.As_Techno();
		Planning_Add_Order(techno, event);
	}
}

bool __fastcall Planning_Add_Order(TechnoClass * techno, EventClass * event) // RA2: 0x00614EC0; plans an order for a techno: adds it to the node at the order's coordinate, or to a new node, unless the order or the techno's path refuses it
{
	if (techno == NULL) return false;
	bool mine = event->ID == PlayerPtr->HeapID;
	Coord where = Planning_Event_Coord(*event);
	if (where == Coord(-1, -1, -1)) return false;
	PlanningNodeClass * node = Planning_Find_Node(event->ID, where, event->Frame);
	if (node && node->Has_Member(techno)) return true;
	int status = Planning_Order_Kind(techno, *event);
	if (status != -1) {
		if (mine) {
			if (status == 0) {
				Planning_Refuse_Order(*event);
			} else if (status == 1) {
				Planning_Refuse("MSG:PostTerminatingCommand");
			} else if (status == 2) {
				Planning_Refuse("MSG:PostContinualCommand");
			}
		}
		return false;
	}
	PlanningTokenClass * token = techno->Get_Planning_Token();
	if (!token) {
		token = Planning_New_Token(techno);
	}
	if (mine) {
		int refusal = Planning_Event_Result(token, *event);
		if (refusal != -1) {
			Planning_Set_Pending_Refusal(refusal);
		}
	}
	if (token->Loop_Start_Valid()) {
		if (mine) {
			Planning_Refuse("MSG:PostLoop");
		}
		return false;
	}
	int count = token->Node_Count();
	if (count == 0) {
		PlanningTokenClass::Technos.Add(techno);
		Planning_Recount(event->ID);
	} else if (count == 16) {
		if (mine) {
			Planning_Refuse("MSG:NodeMaximum");
		}
		return false;
	}
	if (!node) {
		node = new PlanningNodeClass(event->Frame);
		PlanningNodeClass::Unknown1.Add(node);
	}
	PlanningNodeClass * last = token->Last_Node();
	if (last) {
		Planning_Copy_Members(node, last, *event);
	} else {
		node->Add_Member(techno, event);
	}
	if (mine) {
		VocClass::PlayGlobal(Rule->AddPlanningModeCommandSound, 1.0f, 0x2000);
	}
	return true;
}

bool TechnoClass::TryNextPlanningTokenNode(void) // RA2: 0x00615360; starts carrying out the techno's planned path once: queues the planning mission, holds the token's first event and marks the token started
{
	PlanningTokenClass * token = Get_Planning_Token();
	if (token != NULL && token->Node_Count() > 0 && (token->Get_F8C() == -1 || token->Get_F8C() > 0) && !token->Get_F1C()) {
		QueueMission(MISSION_WAIT, true);
		EventClass event = token->Pop_Front();
		Planning_Store_Event(event);
		token->Set_F1C();
		return true;
	}
	return false;
}

inline void __fastcall Planning_Uncount(int house) // RA2: 0x00616D20 (outside this part); lowers the count of a house, never below 0
{
	if (house >= 0 && house < 8) {
		int value = PlanningUnitCounts[house] - 1;
		PlanningUnitCounts[house] = (value < 0) ? 0 : value;
	}
}

void __fastcall Planning_Clear_Techno(TechnoClass * techno, EventClass * event) // RA2: 0x00615480; after an order that is not a planned one, lets go of the planning token of a techno that has nodes, and lets the token run again
{
	if (techno) {
		PlanningTokenClass * token = techno->Get_Planning_Token();
		if (token) {
			int planned = event ? event->Data.MegaMission.IsPlanningEvent : 0;
			if (!planned && token->Node_Count() > 0) {
				Planning_Cancel_Token(techno);
			}
			token->Clear_F1C();
		}
	}
}

void __fastcall Planning_Cancel_Token(TechnoClass * techno) // RA2: 0x006155E0; lets go of the planning token of a techno, when it has one
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	if (token != NULL) {
		token->Cancel_Owner();
	}
}

void __fastcall Planning_Add_To_Drawn(PlanningNodeClass * node) // RA2: 0x006157C0; puts a node on the list of the nodes drawn on the tactical view
{
	PlanningNodeClass::DrawnNodes.Add(node);
}

PlanningTokenClass * __fastcall Planning_New_Token(TechnoClass * techno) // RA2: 0x00615820; makes the planning token of a techno and lists it
{
	PlanningTokenClass * token = new PlanningTokenClass(techno);
	PlanningTokenClass::Array.Add(token);
	techno->Set_Planning_Token(token);
	return token;
}

bool __fastcall Planning_Stop_Type(char type) // RA2: 0x006158F0; the order types planning mode refuses: deploy, stop and scatter
{
	return type == EventClass::DEPLOY || type == EventClass::IDLE || type == EventClass::SCATTER;
}

bool __fastcall Planning_Order_Stays_Put(TechnoClass * techno, int mission, TargetClass target) // RA2: 0x00615910; an area guard order that aims at nothing, or at the cell the techno stands on already
{
	if (mission == MISSION_GUARD_AREA) {
		if (!target.Is_Valid()) {
			return true;
		}
		if (techno) {
			Cell here = techno->Get_Cell();
			Cell cell = techno->Get_Cell();
			CellClass * target_cell = target.As_Cell();
			if (target_cell) {
				cell = target_cell->CellID;
			} else {
				TechnoClass * target_techno = target.As_Techno();
				if (target_techno) {
					cell = target_techno->Get_Cell();
				}
			}
			if (here == cell) {
				return true;
			}
		}
	}
	return false;
}

bool __fastcall Planning_Terminal_Mission(int mission) // RA2: 0x006159C0 (no function of its own); the missions that end a path
{
	return mission == MISSION_ENTER || mission == MISSION_CAPTURE || mission == MISSION_EATEN;
}

bool __fastcall Planning_Cell_Mission(int mission, xTargetClass target) // RA2: 0x006159E0; an attack or an area guard aimed at a cell, or for the area guard at nothing
{
	if (mission == MISSION_ATTACK || mission == MISSION_GUARD_AREA) {
		if (RTTIType(target) == RTTI_CELL) {
			return true;
		}
		if (mission == MISSION_GUARD_AREA && (RTTIType)target == RTTI_NONE) {
			return true;
		}
	}
	return false;
}

bool __fastcall Planning_Mission_Allows(TechnoClass * techno) // RA2: 0x00615A10; whether the current mission of a techno lets planning mode take over: guarding, or attacking or guarding an area with a cell or (for the area) nothing as the target
{
	if (techno) {
		int mission = techno->Get_Mission();
		if (mission == MISSION_GUARD) {
			return true;
		}
		return Planning_Target_Fits(mission, TargetClass(techno->TarCom));
	}
	return false;
}

static inline TargetClass Planning_Target(xTargetClass const & packed)
{
	TargetClass target;
	static_cast<xTargetClass &>(target) = packed;
	return target;
}

static inline EventClass Last_Planning_Event(PlanningTokenClass * token)
{
	return token->Event_At(token->Node_Count() - 1);
}

int __fastcall Planning_Order_Kind(TechnoClass * techno, EventClass const & event) // RA2: 0x00615A80; how an order relates to the path of the techno: 0 refused outright, 1 and 2 past a terminating or a continual order, -1 taken
{
	if (Planning_Stop_Type(event.Type)) {
		return 0;
	}
	int mission = event.Data.MegaMission.Mission;
	xTargetClass const & target = event.Data.MegaMission.Target;
	if (Planning_Order_Stays_Put(techno, mission, Planning_Target(target))) {
		return 0;
	}
	PlanningTokenClass * token = techno->Get_Planning_Token();
	if (token != NULL && token->Node_Count() > 0) {
		EventClass previous = Last_Planning_Event(token);
		int last = previous.Data.MegaMission.Mission;
		if (Planning_Terminal_Mission(last)) {
			return 1;
		}
		if (Planning_Target_Fits(last, Planning_Target(previous.Data.MegaMission.Target))) {
			return 2;
		}
	}
	return -1;
}

void __fastcall Planning_Add_To_Nodes(PlanningNodeClass * node) // RA2: 0x00615B60; puts a node on the list of all nodes
{
	PlanningNodeClass::Unknown1.Add(node);
}

void __cdecl Planning_Clear_Drawn(void) // RA2: 0x00615BC0; empties the list of the nodes drawn on the tactical view
{
	PlanningNodeClass::DrawnNodes.Clear();
}

// RA2: the node searches below are ra2-decomp's twin text (Find_Node_At as of d5cab17: a plain for loop)
static inline PlanningNodeClass * Find_Node_At(DynamicVectorClass<PlanningNodeClass *> const & nodes, Point2D const & point)
{
	int count = nodes.Count();
	for (int i = 0; i < count; ++i) {
		PlanningNodeClass * node = nodes[i];
		if (node->Hit_Test(point.X, point.Y)) return node;
	}
	return NULL;
}

void __cdecl Planning_Hover(void) // RA2: 0x00615C00; the planning mode's mouse hover: the drawn node under the mouse, unless it is not the player's to use
{
	if (!PlanningDragging) {
		int x = MouseCursor->Get_Mouse_X();
		int y = MouseCursor->Get_Mouse_Y();
		PlanningNodeClass * node = Find_Node_At(PlanningNodeClass::DrawnNodes, Point2D(x, y));
		if (node != PlanningHoverNode) {
			PlanningBlockedNode = 0;
			if (node) {
				if (node->PlanningMembers[0]->Owner->House == PlayerPtr
					&& (PlanningPickedNode == 0 || PlanningPickedNode == node)
					&& Planning_Selection_Node_State(node) == -1) {
				} else {
					PlanningBlockedNode = (int)node;
					node = 0;
				}
			}
			PlanningHoverNode = node;
			if (node) {
				node->Dump();
			}
		}
	}
}

static inline PlanningNodeClass * Hover_Node_At(Point2D point)
{
	PlanningNodeClass * hover = PlanningHoverNode;
	PlanningNodeClass * hit;
	if (!hover) {
		hit = NULL;
	} else {
		if (hover->Hit_Test(point.X, point.Y)) {
			hit = PlanningHoverNode;
		} else {
			hit = NULL;
		}
	}

	return hit;
}

PlanningNodeClass * __fastcall Planning_Node_At(Point2D point) // RA2: 0x00615D30; the node under the mouse when the point is inside its box
{
	return Hover_Node_At(point);
}

bool __fastcall Is_Planning_Selection_Valid(void); // RA2: the definition is not exact yet
bool __fastcall Is_Planning_Selection_Valid(void) // RA2: 0x00615DE0; whether planning mode accepts the selected objects: every selected techno has a path that ends at a node of the same members
{
	if (!PlanningNodeClass::PlanningModeActive) {
		return true;
	}
	int count = CurrentObject.Count();
	if (count == 0) {
		return true;
	}
	int i;
	for (i = 0; i < count; i++) {
		ObjectClass * object = CurrentObject[i];
		if (object->Is_Techno()) {
			PlanningTokenClass * first = ((TechnoClass *)object)->Get_Planning_Token();
			for (i = 0; i < count; i++) {
				ObjectClass * other = CurrentObject[i];
				if (!other->Is_Techno()) goto fail;
				PlanningTokenClass * token = ((TechnoClass *)other)->Get_Planning_Token();
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


bool __fastcall Planning_Can_Add(void) // RA2: 0x00615ED0; whether the selected objects that have no path yet fit under the limit of planned objects
{
	if (PlanningNodeClass::PlanningModeActive) {
		int total = Planning_Count(PlayerPtr->HeapID);
		int added = 0;
		int count = CurrentObject.Count();
		for (int i = 0; i < count; i++) {
			ObjectClass * object = CurrentObject[i];
			if (object->Is_Techno()) {
				PlanningTokenClass * token = ((TechnoClass *)object)->Get_Planning_Token();
				if (token == NULL) {
					added++;
				} else if (token->Node_Count() == 0) {
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

PlanningNodeClass * __fastcall Planning_Find_Node(int house, Coord coord, int id) // RA2: 0x00615FB0; the planned node whose lead order aims at the coordinate (of an id and a house unless -1); ra2-decomp's twin text (Yuri's Revenge 0x00639210), the coordinate before the id
{
	int count = PlanningNodeClass::Unknown1.Count();
	for (int i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i];
		if (node->Lead_At(coord)) {
			if (id == -1 || id == node->Get_Id()) {
				int owner = Planning_House(node->Member_Owner(0));
				if (house == -1 || house == owner) {
					return node;
				}
			}
		}
	}
	return 0;
}


PlanningNodeClass * __fastcall Planning_Pick_Node(Point2D point) // RA2: 0x00616070; the node (of all nodes) whose box holds the point
{
	return Find_Node_At(PlanningNodeClass::Unknown1, point);
}

PlanningNodeClass * __fastcall Planning_Pick_Drawn(Point2D point) // RA2: 0x00616140; the drawn node whose box holds the point
{
	return Find_Node_At(PlanningNodeClass::DrawnNodes, point);
}

PlanningNodeClass * __stdcall Planning_Find_Drawn(Coord coord) // RA2: 0x00616210; the first drawn node whose lead order aims at the coordinate
{
	int count = PlanningNodeClass::DrawnNodes.Count();
	for (int i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodeClass::DrawnNodes[i];
		if (node->Lead_Coord() == coord) {
			return node;
		}
	}
	return NULL;
}



void __fastcall Planning_Send_Pair(AbstractClass * source, PlanningNodeClass * node) // RA2: 0x00616280; asks all players to connect the object to the node of the target the first order of the node points at
{
	EventClass event = node->Get_Event(0);
	AbstractClass * object = event.First_Target_Object();
	if (object) {
		EventClass pair(PlayerPtr->HeapID, EventClass::PLAN_CONNECT, TargetClass(source), TargetClass(object));
		Add_Outgoing_Event(pair);
	}
}

void __fastcall Planning_Path_Add(TechnoClass * techno, PlanningNodeClass * node) // RA2: 0x00616350; puts a node on the path of a techno, closing the path into a loop when it holds the node already
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	if (!token) {
		token = Planning_New_Token(techno);
	}
	if (token->Has_Node(node)) { // RA2: as above; ra2-decomp's test (its src/planmgr.cpp Path_Add twin), the member's one use
		Planning_Close_Loop(techno, node);
	} else {
		Planning_Path_Append(techno, node);
	}
}

static inline void Planning_Unselect_Members(PlanningNodeClass * last) // RA2: no function of its own; unselects the members of the last node of a path that just closed into a loop (its expansion stands in Planning_Close_Loop)
{
	int count = last->PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		TechnoClass * member = last->PlanningMembers[i]->Owner;
		if (member->IsSelected) {
			member->Unselect();
		}
	}
}

void __fastcall Planning_Close_Loop(TechnoClass * techno, PlanningNodeClass * node) // RA2: 0x006164E0; makes the path of a techno loop back to a node
{
	int start = Planning_Token_Index(techno->Get_Planning_Token(), node);
	PlanningTokenClass * token = techno->Get_Planning_Token();
	token->ClosedLoopNodeCount = token->PlanningNodes.Count() - start;
	token->StepsToClosedLoop = start;
	token->F98 = true;
	if (!PlanningLoopClosed) {
		if (techno->House->Is_Player_Control()) {
			PlanningLoopClosed = true;
			Planning_Unselect_Members(token->Last_Node());
		}
		Planning_Convert_Loop_Moves(techno);
	}
	node->Update_Active();
}

void __fastcall Planning_Path_Append(TechnoClass * techno, PlanningNodeClass * node) // RA2: 0x006165A0; gives the techno the rest of the path of the node's first member from the node on
{
	TechnoClass * owner = node->Member_Owner(0);
	PlanningTokenClass * initial = owner->Get_Planning_Token();
	int from = initial->Node_Index(node);
	PlanningTokenClass * source = owner->Get_Planning_Token();
	source->Share_From(techno, from);

	int start = -1;
	int loop_count = -1;
	PlanningNodeClass * update = NULL;
	PlanningTokenClass * target = techno->Get_Planning_Token();
	if (source->Loop_Start_Positive() || source->Loop_Start_Zero()) {
		int const source_loop_count = source->Get_Loop_Count();
		int step = source->Get_Loop_Start();
		if (source->Loop_Start_Positive() && from < step) {
			PlanningNodeClass * loop = source->Node_At(step);
			start = target->Node_Index(loop);
			update = loop;
		} else {
			source->Share_Range(techno, step, from - step);
			start = target->Node_Index(node);
			update = node;
		}
		loop_count = source_loop_count;
	}
	target->ClosedLoopNodeCount = loop_count;
	target->StepsToClosedLoop = start;
	target->F98 = true;
	if (update) {
		update->Update_Active();
	}

	if (!PlanningCommandFlag && techno->House->Is_Player_Control()) {
		PlanningCommandFlag = true;
		PlanningNodeClass * last = target->Last_Node();
		if (!update) {
			last->Select_All();
		} else {
			last->Unselect_All();
		}
	}
}

void __fastcall Planning_Copy_Members(PlanningNodeClass * node, PlanningNodeClass * last, EventClass event) // RA2: 0x006167F0; adds every member of one node to another, each with a copy of the event aimed at its techno
{
	int count = last->PlanningMembers.Count();
	for (int i = 0; i < count; i++) {
		TechnoClass * owner = last->Member_Owner(i);
		event.Data.Target.Whom = TargetClass(owner);
		node->Add_Member(owner, &event);
	}
}

void __fastcall Planning_Remove_Node(PlanningNodeClass * node) // RA2: 0x00616960; takes a node out of the paths of its members
{
	if (node) {
		PlanningNodeClass * next = node->Next_Node(node->Member_Owner(0));
		for (int i = node->PlanningMembers.Count() - 1; i >= 0; i--) {
			TechnoClass * owner = node->Member_Owner(i);
			PlanningTokenClass * token = owner->Get_Planning_Token();
			token->Remove_Node_At(token->Node_Index(node));
		}
		if (next && next != node) {
			next->Rebuild();
		}
	}
}

void __fastcall Planning_Convert_Loop_Moves(TechnoClass * techno) // RA2: 0x00616A50; in the path of the techno turns the move orders after the loop start into the other move mission (event mission 2 into 0x29)
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	int count = token->Node_Count();
	int start = token->Get_Loop_Start();
	PlanningNodeClass * first = token->Node_At(0);
	int members = first->Get_Count();
	bool attack_move = false;
	for (int i = 0; i < members; ++i) {
		TechnoClass * owner = first->Member_Owner(i);
		if (owner->CanAttackOnTheMove()) {
			attack_move = true;
			break;
		}
	}
	if (attack_move) {
		for (int j = start; j < count; ++j) token->Node_At(j)->Promote_Events();
	}
}

static inline int Planning_Object_Kind(TechnoClass * techno) // RA2: no function of its own; 0 for an aircraft, 1 for a building, 2 for the rest
{
	if (techno != NULL) {
		if (techno->Fetch_RTTI() == RTTI_AIRCRAFT) {
			return 0;
		}
		if (techno->Fetch_RTTI() == RTTI_BUILDING) {
			return 1;
		}
	}
	return 2;
}

int __cdecl Planning_Selection_Kind(void) // RA2: 0x00616B40; what the selected technos have in common: -1 for none that cannot use a waypoint, else the kind of the first of them
{
	int type = -1;
	int count = CurrentObject.Count();
	for (int i = 0; i < count; i++) {
		ObjectClass * object = CurrentObject[i];
		if (!object->Is_Techno()) {
			type = 2;
			break;
		}
		if (!((TechnoClass *)object)->CanUseWaypoint()) {
			if (object != NULL && object->Fetch_RTTI() == RTTI_AIRCRAFT) {
				type = 0;
			} else if (object != NULL && object->Fetch_RTTI() == RTTI_BUILDING) {
				type = 1;
			} else {
				type = 2;
			}
			break;
		}
	}
	return(type);
}

bool __fastcall Planning_Same_Last_Node(PlanningTokenClass * first, PlanningTokenClass * second) // RA2: 0x00616BD0; whether both paths (or none) end at nodes holding the owners of both
{
	if (first == NULL && second == NULL) {
		return true;
	}
	int first_count = 0;
	if (first != NULL) {
		first_count = first->Node_Count();
	}
	int second_count = 0;
	if (second != NULL) {
		second_count = second->Node_Count();
	}
	if (first_count == 0 && second_count == 0) {
		return true;
	}
	if (first_count == 0 || second_count == 0) {
		return false;
	}
	PlanningNodeClass * first_last = first->Node_At(first_count - 1);
	PlanningNodeClass * second_last = second->Node_At(second_count - 1);
	TechnoClass * first_owner = first->OwnerUnit;
	TechnoClass * second_owner = second->OwnerUnit;
	if (!first_last->Has_Member(second_owner)) {
		return false;
	}
	if (!second_last->Has_Member(first_owner)) {
		return false;
	}
	return true;
}
void __fastcall Planning_Recount(int house) // RA2: 0x00616D00; raises the count of a house
{
	if (house >= 0 && house < 8) {
		PlanningUnitCounts[house] = PlanningUnitCounts[house] + 1;
	}
}

int __fastcall Planning_Count(int house) // RA2: 0x00616D50 (also expanded where it is used); the number of units of a house with a planned path, 0 for a number that is no house
{
	bool valid = house >= 0 && house < 8;
	int count = 0;
	if (valid) {
		count = PlanningUnitCounts[house];
	}
	return count;
}

void __fastcall Planning_Refuse_Event(EventClass * event) // RA2: 0x00616D70; says why planning mode refuses an event of this type, then scolds
{
	if (event->Type == EventClass::DEPLOY) {
		Planning_Refuse("MSG:PlanningModeNoDeploy");
	} else if (event->Type == EventClass::IDLE) {
		Planning_Refuse("MSG:PlanningModeNoStop");
	} else if (event->Type == EventClass::SCATTER) {
		Planning_Refuse("MSG:PlanningModeNoScatter");
	}
	VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000);
}

void __fastcall Planning_Refuse_Area_Guard(void) // RA2: 0x00616E70; refuses the guard command while planning mode is on
{
	Planning_Refuse("MSG:PlanningModeNoGuardArea");
}

void __fastcall Planning_Show_Help(bool sound, bool enable) // RA2: 0x00616EC0; shows the help text of planning mode (when it is turned on or off), the number of times per kind it allows
{
	int kind;
	if (enable) {
		kind = !sound;
	} else {
		kind = 2;
	}
	int limit;
	if (sound) {
		limit = PlanningHelpCounts[kind] + 1;
	} else {
		limit = 1;
	}
	if (PlanningHelpCounts[kind] < limit) {
		if (enable) {
			if (sound) {
				Planning_Message("MSG:PlanningModeIntro1Button");
			} else {
				Planning_Message("MSG:PlanningModeIntro1Key");
			}
		} else {
			Planning_Message("MSG:PlanningModeIntro3");
		}
		PlanningHelpCounts[kind]++;
	}
}

void __fastcall Planning_Help(int kind) // RA2: 0x00616F50; shows the help text of a kind (loop, delete) once
{
	if (PlanningHelpCounts[kind] < 1) {
		if (kind == 3) {
			Planning_Message("MSG:PlanningModeLoopHelp");
		} else if (kind == 4) {
			Planning_Message("MSG:PlanningModeDeleteHelp");
		}
		PlanningHelpCounts[kind]++;
	}
}

void __fastcall Planning_Message(char const * label) // RA2: 0x00616FB0 (expanded in Planning_Refuse); shows a text of the string table
{
	wchar_t const * text = StringTable::LoadString(label, NULL, "D:\\Ra2\\PlanMgr.cpp", 3231);
	Show_Player_Message(text, 0x1E0);
}

void __fastcall Planning_Refuse(char const * label) // RA2: 0x00616FD0; shows a refusal once per frame and scolds
{
	if (!PlanningRefusalShown) {
		PlanningRefusalShown = true;
		Planning_Message(label);
		VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000);
	}
}

void __fastcall Planning_Refuse_Order(EventClass const & event) // RA2: 0x00617020; says why planning mode refuses an order of this type
{
	if (event.Type == EventClass::DEPLOY) {
		Planning_Refuse("MSG:PlanningModeNoDeploy");
	} else if (event.Type == EventClass::IDLE) {
		Planning_Refuse("MSG:PlanningModeNoStop");
	} else if (event.Type == EventClass::SCATTER) {
		Planning_Refuse("MSG:PlanningModeNoScatter");
	}
}

void __fastcall Planning_Refuse_Selection(int kind) // RA2: 0x00617100; says why planning mode refuses a selection of this kind
{
	if (kind == 0) {
		Planning_Refuse("MSG:PlanningModeNoAircraft");
	} else if (kind == 1) {
		Planning_Refuse("MSG:PlanningModeNoBuildings");
	} else if (kind == 2) {
		Planning_Refuse("MSG:PlanningModeNoUnknowns");
	}
}

int __fastcall Planning_Event_Result(PlanningTokenClass * token, EventClass const & event) // RA2: 0x006171E0; what planning mode shows for an order: 0 for one that ends the path, 2 at the last free node, -1 for none
{
	int mission = event.Data.MegaMission.Mission;
	xTargetClass const & target = event.Data.MegaMission.Target;
	if (Planning_Terminal_Mission(mission)) {
		return 0;
	}
	if (Planning_Cell_Mission(mission, target)) {
		return 0;
	}
	if (token->Node_Count() == 15) {
		return 2;
	}
	return -1;
}

void __fastcall Planning_Set_Pending_Refusal(int value) // RA2: 0x00617240; sets the refusal to show at the end of the frame
{
	PlanningPendingRefusal = value;
}

void __fastcall Planning_Refuse_Command(int kind) // RA2: 0x00617250; drops the selection and says why a command is refused
{
	TacticalMap->Unselect_All();
	if (kind == 0) {
		Planning_Refuse("MSG:PostTerminatingCommand");
	} else if (kind == 1) {
		Planning_Refuse("MSG:PostContinualCommand");
	} else if (kind == 2) {
		Planning_Refuse("MSG:NodeMaximum");
	}
}

void __cdecl Planning_Connect_Selection(void); // RA2: 0x00617AF0
void __fastcall Planning_Set_Picked_Node(PlanningNodeClass * node); // RA2: 0x00617AD0
PlanningNodeClass * __fastcall Planning_Get_Picked_Node(void); // RA2: 0x00617AE0
void __cdecl Planning_Clear_Selected_Node(void); // RA2: 0x00617E30
PlanningNodeClass * __fastcall Planning_Selected_Node(void); // RA2: 0x00617A00
void __fastcall Planning_Draw_Node(PlanningNodeClass * node); // RA2: 0x00619420
void __fastcall Planning_Draw_Filled_Node(PlanningNodeClass * node); // RA2: 0x00619870
void __fastcall Planning_Draw_Techno(TechnoClass * techno); // RA2: 0x006182F0
void __fastcall Planning_Draw_Filled_Techno(TechnoClass * techno); // RA2: 0x00618C00
int __fastcall Planning_Pixel(RGBClass const * color); // RA2: 0x0061A860
void __fastcall Planning_Draw_Dot(Point2D center, int color); // RA2: 0x0061A190
void __fastcall Planning_Fill_Dot(Point2D center, int color); // RA2: 0x0061A210
void __fastcall Planning_Draw_Midpoint(Point2D start, Point2D end, int color); // RA2: 0x0061A2F0
void __fastcall Planning_Draw_Midpoint_Shape(Point2D start, Point2D end, int color, int fill, int frame); // RA2: 0x0061A3C0
void __fastcall Planning_Draw_Thick_Line(Point2D start, Point2D end, int thickness, int color); // RA2: 0x0061A6A0
void __fastcall Planning_Draw_Dashed_Thick_Line(Point2D start, Point2D end, int thickness, int color); // RA2: 0x0061A6F0
int __fastcall Planning_Dim_Color(TechnoClass * techno); // RA2: 0x00619CA0
int __fastcall Planning_Techno_Color(TechnoClass * techno); // RA2: 0x00619E80
Point2D __fastcall Planning_Event_Pixel(EventClass const * event, AbstractClass * object); // RA2: 0x00619FA0
Coord __fastcall Planning_Event_Or_Object_Coord(EventClass const & event, AbstractClass * object); // RA2: 0x00619810
bool __fastcall Planning_Idle_Mover(TechnoClass * techno); // RA2: 0x00618BB0

bool __fastcall Planning_Left_Press(Point2D xy) // RA2: 0x00617340; planning mode's left button press on the tactical view: takes the node under the mouse as the picked one (the path starts there), else refuses a selection or a node planning mode cannot take; true when planning mode used the press (ra2-decomp's twin text, Yuri's Revenge 0x0063A5A0)
{
	if (!Planning_Is_Active()) return false;

	PlanningNodeClass * selected = Planning_Get_Picked_Node();
	PlanningNodeClass * hit = Hover_Node_At(xy);
	if (hit && hit != selected) {
		Map.Set_Default_Mouse(MOUSE_NORMAL, true);
		Planning_Set_Picked_Node(hit);
	} else {
		PlanningNodeClass * node = Find_Node_At(PlanningNodeClass::Unknown1, xy);
		if (node) {
			if (Planning_Selection_Node_State(node) != -1) {
				Planning_Refuse("MSG:PlanningModeInvalidNodeX");
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
	return selected || hit;
}

bool __stdcall Planning_Left_Release(Point2D point) // RA2: 0x00617680; planning mode's left button release: connects the selected objects to the node when it is the picked one, else drops the picked node; true when it took the input
{
	if (!Planning_Is_Active()) return false;

	PlanningNodeClass * selected = Planning_Get_Picked_Node();
	PlanningNodeClass * hit = Hover_Node_At(point);

	if (hit && hit == selected) {
		Map.Set_Default_Mouse(MOUSE_NORMAL, true);
		Planning_Connect_Selection();
	} else {
		PlanningNodeClass * node = Find_Node_At(PlanningNodeClass::Unknown1, point);
		if (node && Planning_Selection_Node_State(node) != -1) return true;
		Planning_Set_Picked_Node(NULL);
	}
	return selected || hit;
}

void __stdcall Planning_Right_Press(int x, int y) // RA2: 0x00617860; planning mode's right button press: starts a drag, remembering the point when nothing is selected
{
	PlanningDragging = true;
	PlanningMouseDown = Point2D(-1, -1);
	if (PlanningNodeClass::PlanningModeActive && CurrentObject.Count() <= 0) {
		PlanningMouseDown = Point2D(x, y);
	}
}

void __stdcall Planning_Right_Release(int x, int y) // RA2: 0x006178A0; planning mode's right button release: a click (not a drag) with nothing selected lets go of the selected node
{
	PlanningDragging = false;
	if (PlanningNodeClass::PlanningModeActive && CurrentObject.Count() <= 0 && PlanningSelectedNode != NULL && abs(x - PlanningMouseDown.X) <= 3 && abs(y - PlanningMouseDown.Y) <= 3) {
		PlanningSelectedNode = NULL;
	}
}

bool __stdcall Planning_Mouse_AI(int x, int y); // RA2: the definition is not exact yet
bool __stdcall Planning_Mouse_AI(int x, int y) // RA2: 0x00617900; planning mode's mouse cursor: the plain one over the picked node or when none is picked, else the small one; true when planning mode set it
{
	if (!PlanningNodeClass::PlanningModeActive) {
		return false;
	}
	if (PlanningDragging) {
		return false;
	}
	PlanningNodeClass * remembered = PlanningPickedNode;
	PlanningNodeClass * node = Planning_Node_At(Point2D(x, y));
	if (remembered == NULL && node == NULL) {
		return false;
	}
	if (remembered == NULL || node == remembered) {
		Map.Set_Default_Mouse(MOUSE_NORMAL, true);
	} else {
		Map.Set_Default_Mouse(MOUSE_NORMAL, false);
	}
	return true;
}

PlanningNodeClass * __fastcall Planning_Selected_Node(void) // RA2: 0x00617A00; the planned node the player has selected, if any
{
	return PlanningSelectedNode;
}

int __fastcall Planning_Selection_Node_State(PlanningNodeClass * node) // RA2: 0x00617A10; how the selected objects relate to a node: 1 for an object whose path has closed a loop, 5 for one whose path lacks the node, 2 and 3 for the last and the second last node of a path, -1 for none
{
	int count = CurrentObject.Count();
	for (int i = 0; i < count; i++) {
		ObjectClass * object = CurrentObject[i];
		if (object && object->Is_Techno()) {
			TechnoClass * techno = (TechnoClass *)object;
			PlanningTokenClass * token = techno->Get_Planning_Token();
			if (token) {
				if (token->Loop_Start_Valid()) {
					return 1;
				}
				int index = token->Node_Index(node);
				if (index == -1) {
					return 5;
				}
				int last = token->PlanningNodes.Count();
				if (index == last - 1) {
					return 2;
				}
				if (index == last - 2) {
					return 3;
				}
			}
		}
	}
	return -1;
}

void __fastcall Planning_Set_Picked_Node(PlanningNodeClass * node) // RA2: 0x00617AD0; sets the node a path is being drawn from
{
	PlanningPickedNode = node;
}

PlanningNodeClass * __fastcall Planning_Get_Picked_Node(void) // RA2: 0x00617AE0; the node a path is being drawn from
{
	return PlanningPickedNode;
}

static inline void Connect_Node_Target(PlanningNodeClass * node, TechnoClass * techno)
{
	EventClass packet = node->Get_Event(0);
	AbstractClass * target = packet.First_Target_Object();
	if (target) {
		EventClass event(PlayerPtr->HeapID, EventClass::PLAN_CONNECT, TargetClass(techno), TargetClass(target));
		Add_Outgoing_Event(event);
	}
}

void __cdecl Planning_Connect_Selection(void) // RA2: 0x00617AF0 (called Planning_Connect_Selection by us); lets go of the picked node and asks all players to connect each selected object (and the owners of the last node of its path) to it, or with nothing selected keeps the node as the selected one
{
	PlanningNodeClass * selected = Planning_Get_Picked_Node();
	Planning_Set_Picked_Node(NULL);
	int count = CurrentObject.Count();
	if (count > 0) {
		DynamicVectorClass<TechnoClass *> units;
		for (int i = 0; i < count; i++) {
			ObjectClass * object = CurrentObject[i];
			if (object->Is_Techno()) {
				TechnoClass * unit = (TechnoClass *)object;
				PlanningTokenClass * token = unit->Get_Planning_Token();
				if (token != NULL) {
					PlanningNodeClass * last = token->Last_Node();
					if (last == NULL) {
						units.Add(unit);
					} else {
						int members = last->Get_Count();
						for (int j = 0; j < members; j++) {
							TechnoClass * owner = last->Member_Owner(j);
							if (units.ID(owner) == -1) {
								units.Add(owner);
							}
						}
					}
				} else {
					units.Add(unit);
				}
			}
		}
		int total = units.Count();
		for (int k = 0; k < total; k++) {
			TechnoClass * techno = units[k];
			Connect_Node_Target(selected, techno);
		}
		if (total > 0) {
			Planning_Help(3);
		}
	} else {
		PlanningSelectedNode = selected;
		Planning_Help(4);
	}
}

AbstractClass * EventClass::First_Target_Object(void) const // RA2: 0x00617DF0
{
	AbstractClass * object = NULL;
	if (Data.MegaMission.Target.Is_Valid()) {
		object = Data.MegaMission.Target.As_Abstract();
	} else if (Data.MegaMission.Follow.Is_Valid()) {
		object = Data.MegaMission.Follow.As_Abstract();
	} else if (Data.MegaMission.Destination.Is_Valid()) {
		object = Data.MegaMission.Destination.As_Abstract();
	}
	return object;
}

void __fastcall Planning_Set_Selected_Node(PlanningNodeClass * node) // RA2: 0x00617E20; sets the node whose members are shown selected
{
	PlanningSelectedNode = node;
}

void __cdecl Planning_Clear_Selected_Node(void) // RA2: 0x00617E30; forgets the selected node
{
	PlanningSelectedNode = NULL;
}

void __cdecl Planning_Redraw(void) // RA2: 0x00617E40; rebuilds the list of the drawn nodes: draws the nodes and the paths of the technos that are allies of the player
{
	Planning_Clear_Drawn();
	int count = PlanningNodeClass::Unknown1.Count();
	int i;
	for (i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i];
		if (node->Lead_House()->Is_Ally(PlayerPtr)) {
			Planning_Draw_Node(node);
		}
	}
	count = Technos.Count();
	for (i = 0; i < count; i++) {
		TechnoClass * techno = Technos[i];
		HouseClass * house = techno->House;
		if (house->Is_Ally(PlayerPtr)) {
			Planning_Draw_Techno(techno);
		}
	}
}

void __cdecl Planning_Draw_All(void) // RA2: 0x00617EF0; draws the filled version of the nodes and the paths of the technos that are allies of the player
{
	int count = PlanningNodeClass::Unknown1.Count();
	int i;
	for (i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i];
		if (node->Lead_House()->Is_Ally(PlayerPtr)) {
			Planning_Draw_Filled_Node(node);
		}
	}
	for (i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i];
		if (node->Lead_House()->Is_Ally(PlayerPtr)) {
			node->Fill_Lead_Box(node->Node_Color());
		}
	}
	int technos = Technos.Count();
	for (i = 0; i < technos; i++) {
		TechnoClass * techno = Technos[i];
		if (techno->House->Is_Ally(PlayerPtr)) {
			Planning_Draw_Filled_Techno(techno);
		}
	}
}

void __cdecl Planning_Draw_Remembered(void) // RA2: 0x00618090; outlines the node under the mouse and the selected node in white
{
	int color = Planning_Pixel(&WhiteColor);
	if (PlanningHoverNode != NULL) {
		PlanningHoverNode->Frame_Lead_Box(color);
	}
	if (PlanningSelectedNode != NULL) {
		PlanningSelectedNode->Frame_Lead_Box_Outer(color);
	}
}


// RA2: the two waypoint renderers use these predicates from the ra2-decomp filled-waypoint text.
static inline bool Has_Planned_Nodes(PlanningTokenClass * token)
{
	int count;
	if (!token) count = 0;
	else count = token->Node_Count();
	return count > 0;
}

static inline bool Should_Draw_Active_Action(TechnoClass * techno, bool has_nodes) // RA2: no function of its own; whether the techno has an order of its own to draw: it is not guarding or idle, and has planned nodes or is selected
{
	return techno->Get_Mission() != MISSION_GUARD && !Planning_Idle_Mover(techno)
		&& (has_nodes || techno->IsSelected);
}

bool __fastcall Planning_Idle_Mover(TechnoClass * techno) // RA2: 0x00618BB0; whether a techno is on its move mission with nothing to move to or to attack
{
	if (techno->Get_Mission() == MISSION_GUARD_AREA) {
		if (techno->Is_Foot()) {
			FootClass * foot = (FootClass *)techno;
			if (foot->NavCom != 0 || foot->TarCom != 0) {
				return false;
			}
		}
		return true;
	}
	return false;
}

// RA2: the filled waypoints and their helpers are ra2-decomp's twin text
int __fastcall Planning_Mega_Frame(TechnoClass * techno);
void __fastcall Planning_Draw_Line(Point2D start, Point2D end, int offset, int color, bool dashed);

static inline int RGB_Pixel(RGBClass const & color)
{
	return DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue());
}

static inline int Waypoint_Fill_Color(TechnoClass * techno)
{
	int normal = RGB_Pixel(BlackColor);
	int result;
	if (techno) {
		int highlight = Planning_Pixel(&WhiteColor);
		if (PlanningSelectedNode) {
			if (PlanningSelectedNode->Find_Member(techno) != -1) result = highlight;
			else result = normal;
		} else if (techno->IsSelected) {
			result = highlight;
		} else {
			result = normal;
		}
	} else {
		result = normal;
	}
	return result;
}

static inline int Active_Waypoint_Frame(TechnoClass * techno, bool mega_mission, bool attack_move)
{
	int frame;
	if (mega_mission && !attack_move) frame = Planning_Mega_Frame(techno);
	else frame = techno->Get_Mission() == MISSION_GUARD_AREA ? Cursor_Start_Frame_76() : -1;
	return frame;
}

static inline int Planned_Waypoint_Frame(EventClass order)
{
	return order.Get_Event_Frame();
}

void __fastcall Planning_Draw_Filled_Techno(TechnoClass * techno)
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	Coord origin = techno->As_Coord();
	Point2D center = Planning_Coord_Pixel(origin);
	Point2D source;
	source = center; // RA2: assigned whole, as the family's other points and rectangles are (ra2-decomp's twin copies the two fields)
	int color = Unit_Path_Tint(techno, WAYPOINT_NORMAL);
	int bright = Unit_Path_Tint(techno, WAYPOINT_BRIGHT);
	bool has_nodes = Has_Planned_Nodes(token);
	bool draw_action = Should_Draw_Active_Action(techno, has_nodes);
	int width = 4;
	int fill = Waypoint_Fill_Color(techno);
	if (token && token->Owner_Is_Mission()) {
		EventClass packet(token->Popped);
		Coord coord = Planning_Event_Or_Object_Coord(packet, techno);
		Point2D target = Planning_Coord_Pixel(coord);
		Planning_Fill_Dot(center, color);
		Planning_Draw_Dashed_Thick_Line(center, target, 3, color);
		int frame = packet.Get_Event_Frame();
		if (frame != -1) Planning_Draw_Midpoint_Shape(center, target, color, fill, frame);
		Planning_Fill_Dot(target, color);
		return;
	}

	if (draw_action) {
		bool mega_mission = techno->HaveMegaMission();
		bool attack_move = techno->HaveAttackMoveTarget();
		Coord coord = techno->GetAttackCoordinates();
		Point2D action = Planning_Coord_Pixel(coord);
		if (techno->IsInLimbo) {
			Planning_Fill_Dot(action, color);
			return;
		}
		Planning_Fill_Dot(center, color);
		Planning_Draw_Dashed_Thick_Line(center, action, 3, color);
		int frame = Active_Waypoint_Frame(techno, mega_mission, attack_move);
		if (frame != -1) Planning_Draw_Midpoint_Shape(center, action, color, fill, frame);
		Planning_Fill_Dot(action, color);
		if (attack_move) {
			Coord attack_coord = techno->GetAttackMoveCoords();
			Point2D dest = Planning_Coord_Pixel(attack_coord);
			Planning_Draw_Line(action, dest, 0, color, true);
			frame = techno->GetMegaMission() == MISSION_ATTACK_MOVE ? Cursor_Start_Frame_77() : -1;
			if (frame != -1) Planning_Draw_Midpoint_Shape(action, dest, color, fill, frame);
			Planning_Fill_Dot(dest, color);
			return;
		}
		if (!has_nodes) return;
		source = action;
		width = 3;
	} else if (!has_nodes) {
		return;
	}

	PlanningNodeClass * node = token->Node_At(0);
	Point2D target = node->Lead_Pixel();
	PlanningBranchClass * branch = node->Branch_Of(techno);
	if (branch) target = Planning_Event_Pixel(&branch->Event, techno);
	int path_color = token->Reached(0) ? bright : color;
	Planning_Fill_Dot(source, path_color);
	Planning_Draw_Dashed_Thick_Line(source, target, width, path_color);
	if (branch) Planning_Fill_Dot(target, path_color);
	int frame = Planned_Waypoint_Frame(node->Get_Event_For(techno));
	if (frame != -1) Planning_Draw_Midpoint_Shape(source, target, path_color, fill, frame);
}

int __fastcall Planning_Guard_Frame(TechnoClass * techno);

void __fastcall Planning_Draw_Techno(TechnoClass * techno) // RA2: 0x006182F0; draws the path of a techno: the order under way, the current action and the line to the first node; ra2-decomp's whole twin text (Yuri's Revenge 0x0063B550, the separate source point included), the popped order copied with = and the frame read through RA2's member
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	Coord origin = techno->As_Coord();
	Point2D center = Planning_Coord_Pixel(origin);
	Point2D source;
	source.X = center.X;
	source.Y = center.Y;
	int color = Planning_Techno_Color(techno);
	bool has_nodes = Has_Planned_Nodes(token);
	bool draw_action = Should_Draw_Active_Action(techno, has_nodes);
	int width = 4;
	if (token && token->Owner_Is_Mission()) {
		EventClass packet = token->Popped;
		Coord coord = Planning_Event_Or_Object_Coord(packet, techno);
		Point2D target = Planning_Coord_Pixel(coord);
		Planning_Draw_Dot(center, color);
		Planning_Draw_Thick_Line(center, target, 3, color);
		int frame = packet.Get_Event_Frame();
		if (frame != -1) Planning_Draw_Midpoint(center, target, color);
		Planning_Draw_Dot(target, color);
		return;
	}

	if (draw_action) {
		bool mega_mission = techno->HaveMegaMission();
		bool attack_move = techno->HaveAttackMoveTarget();
		Coord coord = techno->GetAttackCoordinates();
		Point2D action = Planning_Coord_Pixel(coord);
		if (techno->IsInLimbo) {
			Planning_Draw_Dot(action, color);
			return;
		}
		Planning_Draw_Dot(center, color);
		Planning_Draw_Thick_Line(center, action, 3, color);
		int frame = (mega_mission && !attack_move) ? Planning_Mega_Frame(techno) : Planning_Guard_Frame(techno);
		if (frame != -1) Planning_Draw_Midpoint(center, action, color);
		Planning_Draw_Dot(action, color);
		if (attack_move) {
			Coord attack_coord = techno->GetAttackMoveCoords();
			Point2D dest = Planning_Coord_Pixel(attack_coord);
			Planning_Draw_Thick_Line(action, dest, 3, color);
			frame = Planning_Mega_Frame(techno);
			if (frame != -1) Planning_Draw_Midpoint(action, dest, color);
			Planning_Draw_Dot(dest, color);
			return;
		}
		if (!has_nodes) return;
		source = action;
		width = 3;
	} else if (!has_nodes) {
		return;
	}

	PlanningNodeClass * node = token->Node_At(0);
	Point2D target = node->Lead_Pixel();
	PlanningBranchClass * branch = node->Branch_Of(techno);
	if (branch) target = Planning_Event_Pixel(&branch->Event, techno);
	Planning_Draw_Dot(source, color);
	Planning_Draw_Thick_Line(source, target, width, color);
	if (branch) Planning_Draw_Dot(target, color);
	int frame = Planned_Waypoint_Frame(node->Get_Event_For(techno));
	if (frame != -1) Planning_Draw_Midpoint(source, target, color);
}

void __fastcall Planning_Draw_Node(PlanningNodeClass * node) // RA2: 0x00619420; draws the lines from the nodes before this one (and from its own loop) to it as solid lines, and its box
{
	Point2D pixel = node->Lead_Pixel();
	int frame = -1;
	if (node->PlanningMembers.Count() > 0) {
		EventClass event = node->Get_Event(0);
		frame = event.Get_Event_Frame();
	}
	int color = node->Lead_Color();
	int count = node->PlanningBranches.Count();
	for (int index = 0; index < count; index++) {
		Coord coord = node->Branch_Coord(index);
		Point2D start = Planning_Coord_Pixel(coord);
		if (coord == Coord(-1, -1, -1)) {
			EventClass * lead = &node->PlanningBranches[index]->Event;
			start = Planning_Event_Pixel(lead, node->Owner_At_Branch(index));
		}
		Planning_Draw_Thick_Line(start, pixel, 4, color);
		if (frame != -1) {
			Planning_Draw_Midpoint(start, pixel, color);
		}
	}
	if (node->Is_Loop_Alone()) {
		Coord coord = node->Loop_Coord();
		Point2D start = Planning_Coord_Pixel(coord);
		if (coord == Coord(-1, -1, -1)) {
			EventClass * lead = &node->LoopBranch.Event;
			start = Planning_Event_Pixel(lead, node->Active_Owner());
		}
		Planning_Draw_Thick_Line(start, pixel, 4, color);
		if (frame != -1) {
			Planning_Draw_Midpoint(start, pixel, color);
		}
	}
	node->Draw_Lead_Box(color);
}

Coord __fastcall Planning_Event_Or_Object_Coord(EventClass const & event, AbstractClass * object) // RA2: 0x00619810; where the order of an event points, or where the object is when it points nowhere
{
	Coord coord = Planning_Event_Coord(event);
	if (coord == Coord(-1, -1, -1) && object != NULL) {
		coord = object->As_Coord();
	}
	return coord;
}

void __fastcall Planning_Draw_Filled_Node(PlanningNodeClass * node) // RA2: 0x00619870; draws the lines from the nodes before this one (and from its own loop) to it as dashed lines, and fills its box
{
	Point2D pixel = node->Lead_Pixel();
	int frame = -1;
	if (node->PlanningMembers.Count() > 0) {
		EventClass event = node->Get_Event(0);
		frame = event.Get_Event_Frame();
	}
	int color = node->Lead_Color();
	int count = node->PlanningBranches.Count();
	for (int index = 0; index < count; index++) {
		int branch_color = node->Branch_Color(index);
		Coord coord = node->Branch_Coord(index);
		Point2D start = Planning_Coord_Pixel(coord);
		if (coord == Coord(-1, -1, -1)) {
			PlanningBranchClass * branch = node->PlanningBranches[index];
			start = Planning_Event_Pixel(&branch->Event, node->Owner_At_Branch(index));
		}
		Planning_Draw_Dashed_Thick_Line(start, pixel, 4, branch_color);
		if (frame != -1) {
			Planning_Draw_Midpoint_Shape(start, pixel, branch_color, color, frame);
		}
	}
	if (node->Is_Loop_Alone()) {
		int loop_color = node->Loop_Color();
		Coord coord = node->Loop_Coord();
		Point2D start = Planning_Coord_Pixel(coord);
		if (coord == Coord(-1, -1, -1)) {
			TechnoClass * owner = node->Active_Owner();
			start = Planning_Event_Pixel(&node->LoopBranch.Event, owner);
		}
		Planning_Draw_Dashed_Thick_Line(start, pixel, 4, loop_color);
		if (frame != -1) {
			Planning_Draw_Midpoint_Shape(start, pixel, loop_color, color, frame);
		}
	}
	node->Fill_Lead_Box(node->Node_Color());
}


void __fastcall Planning_Fill_Node(PlanningNodeClass * node) // RA2: 0x00619BD0; fills the box of a node in its color
{
	node->Fill_Lead_Box(node->Node_Color());
}

int __fastcall Planning_Dim_Color(TechnoClass * techno) // RA2: 0x00619CA0; the pixel of the house color of a techno brought 80/256 toward black
{
	RGBClass color = Planning_House_Color(techno);
	color.Adjust(0x50, BlackColor);
	return ((color.Get_Green() >> DSurface::GreenLeft) << DSurface::GreenRight) | ((color.Get_Red() >> DSurface::RedLeft) << DSurface::RedRight) | ((color.Get_Blue() >> DSurface::BlueLeft) << DSurface::BlueRight);
}

int __fastcall Planning_Bright_Color(TechnoClass * techno) // RA2: 0x00619D50; the pixel of the house color of a techno brought 100/256 toward white
{
	RGBClass color = Planning_House_Color(techno);
	color.Adjust(100, WhiteColor);
	return ((color.Get_Green() >> DSurface::GreenLeft) << DSurface::GreenRight) | ((color.Get_Red() >> DSurface::RedLeft) << DSurface::RedRight) | ((color.Get_Blue() >> DSurface::BlueLeft) << DSurface::BlueRight);
}

int __fastcall Planning_Techno_Color(TechnoClass * techno) // RA2: 0x00619E80; black, or white for a techno that is selected (or a member of the selected node)
{
	int plain = Planning_Pixel(&BlackColor);
	if (techno == NULL) {
		return plain;
	}
	int lighter = Planning_Pixel(&WhiteColor);
	if (PlanningSelectedNode != NULL) {
		if (PlanningSelectedNode->Find_Member(techno) != -1) {
			return lighter;
		}
		return plain;
	}
	if (techno->IsSelected) {
		return lighter;
	}
	return plain;
}

Point2D __fastcall Planning_Event_Pixel(EventClass const * event, AbstractClass * object) // RA2: 0x00619FA0; the pixel an order points at, or where the object is when it points nowhere; (-1, -1) for no order
{
	Point2D pixel(-1, -1);
	if (event != NULL) {
		EventClass copy = *event;
		Coord coord = Planning_Event_Or_Object_Coord(copy, object);
		pixel = Planning_Coord_Pixel(coord);
	}
	return pixel;
}

int PlanningNodeClass::Lead_Color(void) // RA2: 0x0061A060 (a member of the node in YR); black, or white for a node whose lead owner is selected (or a member of the selected node)
{
	return Planning_Techno_Color(Member_Owner(0)); // RA2: both the owner lookup and its selection color match the YR method
}

void __fastcall Planning_Draw_Dot(Point2D center, int color) // RA2: 0x0061A190; draws a 6 by 6 frame around a pixel
{
	Rect clip = Get_Title_Screen_Area();
	Rect rect = Planning_Centered_Rect(center, 6);
	CompositeSurface->Draw_Rect(clip, rect, color);
}

void __fastcall Planning_Fill_Dot(Point2D center, int color) // RA2: 0x0061A210; fills a 6 by 6 square around a pixel, less its edge
{
	Rect clip = Get_Title_Screen_Area();
	Rect rect = Planning_Centered_Rect(center, 6);
	Planning_Inflate(rect, -1);
	CompositeSurface->Fill_Rect(clip, rect, color);
}

int __fastcall Planning_Guard_Frame(TechnoClass * techno); // RA2: the definition is not exact yet
int __fastcall Planning_Guard_Frame(TechnoClass * techno) // RA2: 0x0061A290; the first cursor frame of the guard order of a techno on guard, else -1
{
	if (techno->Get_Mission() == MISSION_GUARD_AREA) {
		return Cursor_Start_Frame_76();
	}
	return -1;
}

int __fastcall Planning_Mega_Frame(TechnoClass * techno); // RA2: the definition is not exact yet
int __fastcall Planning_Mega_Frame(TechnoClass * techno) // RA2: 0x0061A2B0; the first cursor frame of the attack move order of a techno, else -1
{
	if (techno->GetMegaMission() == MISSION_ATTACK_MOVE) {
		return Cursor_Start_Frame_77();
	}
	return -1;
}

int EventClass::Get_Event_Frame(void) const // RA2: 0x0061A2D0
{
	int mission = (signed char)Data.MegaMission.Mission;
	if (mission == MISSION_GUARD_AREA) {
		return Cursor_Start_Frame_76();
	}
	if (mission == MISSION_ATTACK_MOVE) {
		return Cursor_Start_Frame_77();
	}
	return -1;
}

void __fastcall Planning_Draw_Midpoint(Point2D start, Point2D end, int color) // RA2: 0x0061A2F0; draws a 21 by 21 frame halfway along a line when it is at least 37 pixels long
{
	if (Distance(start, end) < 37) {
		return;
	}
	int x = (start.X + end.X) / 2 - 10;
	int y = (start.Y + end.Y) / 2 - 10;
	Rect view = Get_Title_Screen_Area();
	Rect square(x, y, 21, 21);
	CompositeSurface->Draw_Rect(view, square, color);
}

void __fastcall Planning_Draw_Midpoint_Shape(Point2D start, Point2D end, int color, int fill, int frame) // RA2: 0x0061A3C0; draws a framed and filled 21 by 21 box halfway along a line when it is at least 37 pixels long, with a cursor shape in it
{
	if (start.Distance_To(end) >= 37) {
		int x = (start.X + end.X) / 2;
		int y = (start.Y + end.Y) / 2;
		int left = x - 10;
		int top = y - 10;
		Rect view = Get_Title_Screen_Area();
		Rect square(left, top, 21, 21);
		Planning_Inflate(square, -1);
		CompositeSurface->Draw_Rect(view, square, color);
		Planning_Inflate(square, -1);
		CompositeSurface->Fill_Rect(view, square, fill);
		Point2D point(left, top);
		Rect window = view;
		ShapeSet const * shape = MouseClass::MouseShapes;
		int width = shape->Get_Width();
		int height = shape->Get_Height();
		point.X = x - width / 2 - 1;
		point.Y = y - height / 2;
		Draw_Shape(*CompositeSurface, *MouseDrawer, shape, frame, point, window, SHAPE_WIN_REL, NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0));
	}
}

static bool const PlanningDashes[16] = {true, true, true, true, false, false, false, false, true, true, true, true, false, false, false, false}; // RA2: 0x007EB9F8; the dash pattern of the dashed lines

void __fastcall Planning_Draw_Line(Point2D start, Point2D end, int offset, int color, bool dashed); // RA2: the definition is not exact yet
void __fastcall Planning_Draw_Line(Point2D start, Point2D end, int offset, int color, bool dashed) // RA2: 0x0061A5A0; draws a line on the tactical view moved sideways by offset (clipped to it), dashed or solid
{
	bool steep = abs(end.X - start.X) > abs(end.Y - start.Y);
	int sideways = steep ? 0 : offset;
	int lengthways = steep ? offset : 0;
	start.X += sideways;
	start.Y += lengthways;
	end.X += sideways;
	end.Y += lengthways;
	Rect area = TacticalRect;
	if (Clip_Line_To_Rect(start, end, area)) {
		if (dashed) {
			int phase = (-(int)timeGetTime() >> 5) & 0xF;
			CompositeSurface->Draw_Dashed_Line(start, end, color, (bool *)PlanningDashes, phase);
		} else {
			CompositeSurface->Draw_Line(start, end, color);
		}
	}
}

void __fastcall Planning_Draw_Thick_Line(Point2D start, Point2D end, int thickness, int color) // RA2: 0x0061A6A0; draws a solid line thickness pixels wide
{
	int last = thickness / 2;
	for (int offset = last - thickness + 1; offset <= last; offset++) {
		Planning_Draw_Line(start, end, offset, color, false);
	}
}

void __fastcall Planning_Draw_Dashed_Thick_Line(Point2D start, Point2D end, int thickness, int color) // RA2: 0x0061A6F0; draws a dashed line thickness less 2 pixels wide
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

int __fastcall Planning_Pixel(RGBClass const * color) // RA2: 0x0061A860
{
	unsigned char rr = DSurface::RedLeft;
	unsigned char gr = DSurface::GreenLeft;
	unsigned char br = DSurface::BlueLeft;
	return (color->Get_Red() >> rr << DSurface::RedRight)
		| (color->Get_Green() >> gr << DSurface::GreenRight)
		| (color->Get_Blue() >> br << DSurface::BlueRight);
}


int __fastcall Planning_Adjusted_Pixel(RGBClass const & color, RGBClass const & other, int ratio) // RA2: 0x00619E00; blends a planning color toward another and packs it as a pixel (ra2-decomp's twin)
{
	RGBClass mixed = color;
	mixed.Adjust(ratio, other);
	return (mixed.Get_Red() >> DSurface::RedLeft << DSurface::RedRight)
		| (mixed.Get_Green() >> DSurface::GreenLeft << DSurface::GreenRight)
		| (mixed.Get_Blue() >> DSurface::BlueLeft << DSurface::BlueRight);
}
