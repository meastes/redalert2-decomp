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
#undef LoadString // RA2: as above
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
	HouseClass * house = techno->House; // RA2: as above
	int index = house->HeapID; // RA2: as above
	if (house->Is_Player_Control() && PlayerPtr) { // RA2: as above
		index = PlayerPtr->HeapID; // RA2: as above
	}
	return index; // RA2: as above
}

Point2D __fastcall Planning_Coord_Pixel(Coord const & coord) // RA2: 0x006107A0; a coordinate as a pixel of the tactical view moved to the screen
{
	Point2D pixel; // RA2: as above
	TacticalMap->Coord_To_Pixel(coord, pixel); // RA2: as above
	pixel.X += TacticalRect.X; // RA2: as above
	pixel.Y += TacticalRect.Y; // RA2: as above
	return pixel; // RA2: as above
}

PlanningMemberClass::PlanningMemberClass(void) : Owner(0), Packet(0), Branch(-1), Active(false) // RA2: 0x006107E0
{
}

void PlanningMemberClass::Set_Owner(TechnoClass * owner) // RA2: 0x00610800
{
	Owner = owner; // RA2: as above
}

void PlanningMemberClass::Set_Packet(EventClass * packet) // RA2: 0x00610810
{
	Packet = packet; // RA2: as above
}

void PlanningMemberClass::Set_Branch(int branch) // RA2: 0x00610820
{
	Branch = branch; // RA2: as above
}

void PlanningMemberClass::Set_Active(bool active) // RA2: 0x00610830
{
	Active = active; // RA2: as above
}

TechnoClass * PlanningMemberClass::Get_Owner(void) // RA2: 0x00610840
{
	return Owner; // RA2: as above
}

EventClass * PlanningMemberClass::Get_Packet(void) // RA2: 0x00610850
{
	return Packet; // RA2: as above
}

int PlanningMemberClass::Get_Branch(void) // RA2: 0x00610860
{
	return Branch; // RA2: as above
}

bool PlanningMemberClass::Get_Active(void) // RA2: 0x00610870
{
	return Active; // RA2: as above
}

PlanningBranchClass::PlanningBranchClass(void) : Count(0), Color(-1) // RA2: 0x00610880
{
}

void PlanningBranchClass::Set_Event(EventClass const & event) // RA2: 0x006108A0
{
	Event = event; // RA2: as above
}

void PlanningBranchClass::Set_Count(int count) // RA2: 0x006108C0
{
	Count = count; // RA2: as above
}

void PlanningBranchClass::Set_Color(int color) // RA2: 0x006108D0
{
	Color = color; // RA2: as above
}

void PlanningBranchClass::Clear_Count(void) // RA2: 0x006108E0
{
	Count = 0; // RA2: as above
}

int PlanningBranchClass::Inc_Count(void) // RA2: 0x006108F0
{
	return ++Count; // RA2: as above
}

int PlanningBranchClass::Dec_Count(void) // RA2: 0x00610900
{
	return --Count; // RA2: as above
}

EventClass PlanningBranchClass::Get_Event(void) // RA2: 0x00610910
{
	return Event; // RA2: as above
}

int PlanningBranchClass::Get_Count(void) // RA2: 0x00610930
{
	return Count; // RA2: as above
}

int PlanningBranchClass::Get_Color(void) // RA2: 0x00610940
{
	return Color; // RA2: as above
}

Coord PlanningBranchClass::Get_Coord(void) // RA2: 0x00610950
{
	return Coord(Planning_Event_Coord(Event)); // RA2: as above
}

Coord __fastcall Planning_Event_Coord(EventClass const & event) // RA2: 0x00610980; where the order of a mission event points (target, destination or follow), (-1, -1, -1) for none
{
	if (event.Type == EventClass::MEGAMISSION) { // RA2: as above
		AbstractClass * object; // RA2: as above
		if (event.Data.MegaMission.Target.Is_Valid()) { // RA2: as above
			object = event.Data.MegaMission.Target.As_Abstract(); // RA2: as above
			if (object) { // RA2: as above
				return object->As_Coord(); // RA2: as above
			}
		} else if (event.Data.MegaMission.Follow.Is_Valid()) { // RA2: as above
			object = event.Data.MegaMission.Follow.As_Abstract(); // RA2: as above
			if (object) { // RA2: as above
				return object->As_Coord(); // RA2: as above
			}
		} else if (event.Data.MegaMission.Destination.Is_Valid()) { // RA2: as above
			object = event.Data.MegaMission.Destination.As_Abstract(); // RA2: as above
			if (object) { // RA2: as above
				return object->As_Coord(); // RA2: as above
			}
		}
	}
	return Coord(-1, -1, -1); // RA2: as above
}

Point2D PlanningBranchClass::Get_Pixel(void) // RA2: 0x00610A20
{
	Coord coord = Planning_Event_Coord(Event); // RA2: as above
	return Planning_Coord_Pixel(coord); // RA2: as above
}

PlanningNodeClass::PlanningNodeClass(int id) : Id(id), Queued(false), LoopBranchIndex(-1), Color(-1) // RA2: 0x00610A80
{
}

PlanningNodeClass::~PlanningNodeClass() // RA2: 0x00610AF0
{
	Clear(); // RA2: as above
	Planning_Unlist_Node(this); // RA2: as above
}

int PlanningNodeClass::Get_Id(void) // RA2: 0x00610C10
{
	return Id; // RA2: as above
}

int PlanningNodeClass::Get_Count(void) // RA2: 0x00610C20
{
	return PlanningMembers.Count(); // RA2: as above
}

int PlanningNodeClass::Index_In(TechnoClass * techno) // RA2: 0x00610C30
{
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	return token->PlanningNodes.ID(this); // RA2: as above
}

PlanningNodeClass * PlanningNodeClass::Prev_Node(TechnoClass * techno) // RA2: 0x00610C60
{
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	int index = token->PlanningNodes.ID(this); // RA2: as above
	if (index == -1) return 0; // RA2: as above
	int count = token->PlanningNodes.Count(); // RA2: as above
	if (count == 1) return this; // RA2: as above
	index--; // RA2: as above
	if (index < 0) index = count - 1; // RA2: as above
	return token->Node_At(index); // RA2: as above
}

PlanningNodeClass * PlanningNodeClass::Next_Node(TechnoClass * techno) // RA2: 0x00610CE0
{
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	int index = token->PlanningNodes.ID(this); // RA2: as above
	if (index == -1) return 0; // RA2: as above
	int count = token->PlanningNodes.Count(); // RA2: as above
	if (count == 1) return this; // RA2: as above
	index++; // RA2: as above
	if (index >= count) index = 0; // RA2: as above
	return token->Node_At(index); // RA2: as above
}

void PlanningNodeClass::Add_Member(TechnoClass * techno, EventClass const * event) // RA2: 0x00610D60
{
	PlanningMemberClass * member = new PlanningMemberClass; // RA2: as above
	member->Set_Owner(techno); // RA2: as above
	EventClass * packet = new EventClass; // RA2: as above
	*packet = *event;
	member->Set_Packet(packet); // RA2: as above
	PlanningMembers.Add(member); // RA2: as above
	techno->Get_Planning_Token()->Add_Node(this); // RA2: as above
	Assign_Branch(member); // RA2: as above
}

void PlanningNodeClass::Remove_Member(TechnoClass * techno) // RA2: 0x00610E70
{
	PlanningMemberClass * member = Get_Member(techno); // RA2: as above
	if (member) { // RA2: as above
		Detach(member); // RA2: as above
		delete member->Get_Packet(); // RA2: as above
		PlanningMembers.Delete(member); // RA2: as above
		delete member; // RA2: as above
		if (Get_Count() == 0) { // RA2: as above
			delete this; // RA2: as above
		}
	}
}

int PlanningNodeClass::Find_Member(TechnoClass * techno) // RA2: 0x00611050
{
	int result = -1; // RA2: as above
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		if (PlanningMembers[i]->Get_Owner() == techno) { // RA2: as above
			result = i; // RA2: as above
			break; // RA2: as above
		}
	}
	return result; // RA2: as above
}

TechnoClass * PlanningNodeClass::Member_Owner(int index) // RA2: 0x00611090
{
	PlanningMemberClass * member = PlanningMembers[index];
	TechnoClass * owner = member->Owner;
	return owner;
}

Rect __fastcall Planning_Centered_Rect(Point2D center, int size); // RA2: as above

EventClass PlanningNodeClass::Get_Event(int index) // RA2: 0x006110A0
{
	PlanningMemberClass * member = PlanningMembers[index]; // RA2: as above
	return *member->Get_Packet(); // RA2: as above
}

EventClass PlanningNodeClass::Get_Event_For(TechnoClass * techno) // RA2: 0x006110D0
{
	return *Get_Member(techno)->Packet; // RA2: as above
}

bool PlanningNodeClass::Has_Member(TechnoClass * techno) // RA2: 0x00611120
{
	return Find_Member(techno) != -1; // RA2: as above
}

HouseClass * PlanningNodeClass::Lead_House(void) // RA2: 0x00611170
{
	return PlanningMembers[0]->Owner->House; // RA2: as above
}

Coord PlanningNodeClass::Lead_Coord(void) // RA2: 0x00611180
{
	Coord coord = Planning_Event_Coord(*PlanningMembers[0]->Get_Packet()); // RA2: as above
	return coord; // RA2: as above
}

Point2D PlanningNodeClass::Lead_Pixel(void) // RA2: 0x006111C0
{
	Coord coord = Lead_Coord(); // RA2: as above
	Point2D pixel = Planning_Coord_Pixel(coord); // RA2: as above
	return pixel; // RA2: as above
}

bool PlanningNodeClass::Hit_Test(int x, int y) // RA2: 0x00611230
{
	return Lead_Box_Rect().Is_Point_Within(Point2D(x, y)); // RA2: as above
}

Rect __fastcall Planning_Centered_Rect(Point2D center, int size) // RA2: 0x006112D0; a size by size rectangle centered on a pixel
{
	int offset = -(size / 2); // RA2: as above
	Point2D corner; // RA2: as above
	corner = center; // RA2: as above
	corner.X += offset; // RA2: as above
	corner.Y += offset; // RA2: as above
	Rect rect(corner.X, corner.Y, size, size); // RA2: as above
	return rect; // RA2: as above
}

bool PlanningNodeClass::Lead_At(Coord coord) // RA2: 0x00611310
{
	Coord c = Planning_Event_Coord(*PlanningMembers[0]->Packet); // RA2: as above
	return c.X == coord.X && c.Y == coord.Y && c.Z == coord.Z; // RA2: as above
}

Rect PlanningNodeClass::Lead_Rect(void) // RA2: 0x00611360
{
	Point2D pixel; // RA2: as above
	pixel = Lead_Pixel(); // RA2: as above
	return Planning_Centered_Rect(pixel, 8); // RA2: as above
}

bool PlanningNodeClass::Is_Past_Start(void) // RA2: 0x006113F0
{
	if (PlanningMembers.Count() == 0) return false; // RA2: as above
	PlanningTokenClass * token = PlanningMembers[0]->Owner->Get_Planning_Token(); // RA2: as above
	int index = token->PlanningNodes.ID(this); // RA2: as above
	int start = token->StepsToClosedLoop; // RA2: as above
	return start != -1 && index >= start; // RA2: as above
}

void PlanningNodeClass::Promote_Events(void) // RA2: 0x00611440
{
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		EventClass * packet = PlanningMembers[i]->Packet; // RA2: as above
		if (packet->Data.MegaMission.Mission == MISSION_MOVE) packet->Data.MegaMission.Mission = MISSION_ATTACK_MOVE; // RA2: as above
	}
}

PlanningMemberClass * PlanningNodeClass::Get_Member(TechnoClass * techno) // RA2: 0x00611470
{
	int index = Find_Member(techno); // RA2: as above
	if (index == -1) { // RA2: as above
		return NULL; // RA2: as above
	}
	return PlanningMembers[index]; // RA2: as above
}

int PlanningNodeClass::Branch_Count(void) // RA2: 0x006114C0
{
	return PlanningBranches.Count(); // RA2: as above
}

int PlanningNodeClass::Has_Loop(void) // RA2: 0x006114D0
{
	return LoopBranch.Count > 0; // RA2: as above
}

int PlanningNodeClass::Is_Loop_Alone(void) // RA2: 0x006114E0
{
	if (LoopBranch.Count > 0 && LoopBranchIndex == -1) return 1; // RA2: as above
	return 0; // RA2: as above
}

TechnoClass * PlanningNodeClass::Owner_At_Branch(int branch) // RA2: 0x00611500
{
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningMemberClass * m = PlanningMembers[i]; // RA2: as above
		if (m->Branch == branch) return m->Owner; // RA2: as above
	}
	return 0; // RA2: as above
}

TechnoClass * PlanningNodeClass::Active_Owner(void) // RA2: 0x00611530
{
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningMemberClass * m = PlanningMembers[i]; // RA2: as above
		if (m->Active) return m->Owner; // RA2: as above
	}
	return 0; // RA2: as above
}

PlanningBranchClass * PlanningNodeClass::Get_Branch(int index) // RA2: 0x00611560
{
	return PlanningBranches[index]; // RA2: as above
}

PlanningBranchClass * PlanningNodeClass::Branch_Of(TechnoClass * techno) // RA2: 0x00611570
{
	PlanningMemberClass * member = Get_Member(techno); // RA2: as above
	int branch = member->Branch; // RA2: as above
	PlanningBranchClass * result = 0; // RA2: as above
	if (branch != -1) result = PlanningBranches[branch]; // RA2: as above
	return result; // RA2: as above
}

EventClass * PlanningNodeClass::Loop_Event(void) // RA2: 0x006115C0
{
	return &LoopBranch.Event; // RA2: as above
}

Coord PlanningNodeClass::Branch_Coord(int index) // RA2: 0x006115D0
{
	Coord coord = Get_Branch(index)->Get_Coord(); // RA2: as above
	return coord; // RA2: as above
}

Coord PlanningNodeClass::Loop_Coord(void) // RA2: 0x00611610
{
	Coord coord = LoopBranch.Get_Coord(); // RA2: as above
	return coord; // RA2: as above
}

Point2D PlanningNodeClass::Branch_Pixel(int index) // RA2: 0x00611640
{
	return PlanningBranches[index]->Get_Pixel(); // RA2: as above
}

Point2D PlanningNodeClass::Loop_Pixel(void) // RA2: 0x006116B0
{
	return LoopBranch.Get_Pixel(); // RA2: as above
}

void PlanningNodeClass::Set_Queued(void) // RA2: 0x00611710
{
	Queued = true; // RA2: as above
}

bool PlanningNodeClass::Get_Queued(void) // RA2: 0x00611720
{
	return Queued; // RA2: as above
}

void PlanningNodeClass::Rebuild(void) // RA2: 0x00611730
{
	Clear(); // RA2: as above
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		Assign_Branch(PlanningMembers[i]); // RA2: as above
	}
	Update_Active(); // RA2: as above
	Queued = false; // RA2: as above
}

void PlanningNodeClass::Update_Active(void) // RA2: 0x00611770
{
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningMemberClass * member = PlanningMembers[i]; // RA2: as above
		TechnoClass * owner = member->Owner; // RA2: as above
		if (Is_Loop_Start(owner)) { // RA2: as above
			member->Active = true; // RA2: as above
			if (LoopBranch.Inc_Count() == 1) { // RA2: as above
				LoopBranch.Set_Event(owner->Get_Planning_Token()->Last_Node()->Get_Event_For(owner)); // RA2: as above
			}
		}
	}
	LoopBranchIndex = -1; // RA2: as above
	if (LoopBranch.Get_Count() != 0) { // RA2: as above
		EventClass self = LoopBranch.Get_Event(); // RA2: as above
		int branches = PlanningBranches.Count(); // RA2: as above
		int b = 0; // RA2: as above
		if (branches > 0) { // RA2: as above
			do { // RA2: as above
				EventClass other = PlanningBranches[b]->Get_Event(); // RA2: as above
				Coord a = Planning_Event_Coord(self); // RA2: as above
				Coord c = Planning_Event_Coord(other); // RA2: as above
				if (a == c) { // RA2: as above
					LoopBranchIndex = b; // RA2: as above
					return; // RA2: as above
				}
				b++; // RA2: as above
			} while (b < branches); // RA2: as above
		}
	}
}

void PlanningNodeClass::Dump(void) // RA2: 0x006118E0
{
	int branch_count = PlanningBranches.Count(); // RA2: as above
	for (int branch_index = 0; branch_index < branch_count; ++branch_index) { // RA2: as above
		PlanningBranchClass * branch = PlanningBranches[branch_index]; // RA2: as above
		int count = branch->Get_Count(); // RA2: as above
		EventClass event = branch->Get_Event(); // RA2: as above
		Coord origin = Planning_Event_Coord(event); // RA2: as above
		DebugString("Branch [%d];  Member count[%d];  Origin event location[%d, %d, %d]\n", // RA2: as above
			branch_index, count, origin.X, origin.Y, origin.Z); // RA2: as above
		int member_count = PlanningMembers.Count(); // RA2: as above
		for (int member_index = 0; member_index < member_count; ++member_index) { // RA2: as above
			PlanningMemberClass * member = PlanningMembers[member_index]; // RA2: as above
			if (member->Get_Branch() == branch_index) { // RA2: as above
				TechnoClass * owner = member->Get_Owner(); // RA2: as above
				char const * name = owner->Class_Of()->IniName; // RA2: as above
				Cell cell = owner->Get_Cell(); // RA2: as above
				DebugString("  Member [%s];  Cell location[%d, %d]\n", name, cell.X, cell.Y); // RA2: as above
			}
		}
	}
	int count = LoopBranch.Get_Count(); // RA2: as above
	if (count > 0) { // RA2: as above
		EventClass event = LoopBranch.Get_Event(); // RA2: as above
		Coord origin = Planning_Event_Coord(event); // RA2: as above
		DebugString("Loop Branch [%d];  Member count[%d];  Origin event location[%d, %d, %d]\n", // RA2: as above
			LoopBranchIndex, count, origin.X, origin.Y, origin.Z); // RA2: as above
		int member_count = PlanningMembers.Count(); // RA2: as above
		for (int member_index = 0; member_index < member_count; ++member_index) { // RA2: as above
			PlanningMemberClass * member = PlanningMembers[member_index]; // RA2: as above
			if (member->Get_Active()) { // RA2: as above
				TechnoClass * owner = member->Get_Owner(); // RA2: as above
				char const * name = owner->Class_Of()->IniName; // RA2: as above
				Cell cell = owner->Get_Cell(); // RA2: as above
				DebugString("  Member [%s];  Cell location[%d, %d]\n", name, cell.X, cell.Y); // RA2: as above
			}
		}
	}
}

int PlanningNodeClass::Find_Branch(TechnoClass * techno) // RA2: 0x00611A80
{
	PlanningNodeClass * prev = Prev_Node(techno); // RA2: as above
	if (prev == techno->Get_Planning_Token()->Last_Node()) return -1; // RA2: as above
	EventClass event = prev->Get_Event_For(techno); // RA2: as above
	int count = PlanningBranches.Count(); // RA2: as above
	if (count == 0) return -1; // RA2: as above
	Coord origin = Planning_Event_Coord(event); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningBranchClass * branch = PlanningBranches[i]; // RA2: as above
		if (branch->Get_Count()) { // RA2: as above
			EventClass other = branch->Get_Event(); // RA2: as above
			Coord c = Planning_Event_Coord(other); // RA2: as above
			if (origin == c) return i; // RA2: as above
		}
	}
	return -1; // RA2: as above
}

static inline void Find_Loop_Branch(PlanningNodeClass * node) // RA2: no function of its own; expanded in Assign_Branch and Locate_Loop_Branch
{
	node->LoopBranchIndex = -1; // RA2: as above
	if (node->LoopBranch.Get_Count()) { // RA2: as above
		EventClass self = node->LoopBranch.Get_Event(); // RA2: as above
		int branches = node->PlanningBranches.Count(); // RA2: as above
		for (int b = 0; b < branches; b++) { // RA2: as above
			EventClass other = node->PlanningBranches[b]->Get_Event(); // RA2: as above
			Coord a = Planning_Event_Coord(self); // RA2: as above
			Coord c = Planning_Event_Coord(other); // RA2: as above
			if (a == c) { // RA2: as above
				node->LoopBranchIndex = b; // RA2: as above
				return; // RA2: as above
			}
		}
	}
}

int PlanningNodeClass::Assign_Branch(PlanningMemberClass * member) // RA2: 0x00611BD0
{
	TechnoClass * owner = member->Get_Owner(); // RA2: as above
	int index = Find_Branch(owner); // RA2: as above
	if (index == -1) { // RA2: as above
		PlanningNodeClass * previous = Prev_Node(owner); // RA2: as above
		PlanningNodeClass * last = owner->Get_Planning_Token()->Last_Node(); // RA2: as above
		if (previous == last) { // RA2: as above
			member->Set_Branch(-1); // RA2: as above
			return -1; // RA2: as above
		}
		EventClass event = previous->Get_Event_For(owner); // RA2: as above
		PlanningBranchClass * branch = new PlanningBranchClass; // RA2: as above
		branch->Set_Event(event); // RA2: as above
		branch->Inc_Count(); // RA2: as above
		PlanningBranches.Add(branch); // RA2: as above
		int count = PlanningBranches.Count() - 1; // RA2: as above
		if (Is_Loop_Alone()) { // RA2: as above
			Find_Loop_Branch(this); // RA2: as above
		}
		Reset_Colors(false); // RA2: as above
		member->Set_Branch(count); // RA2: as above
		return count; // RA2: as above
	} else { // RA2: as above
		PlanningBranchClass * branch = PlanningBranches[index]; // RA2: as above
		branch->Set_Color(-1); // RA2: as above
		Reset_Colors(false); // RA2: as above
		branch->Inc_Count(); // RA2: as above
		member->Set_Branch(index); // RA2: as above
		return index; // RA2: as above
	}
}

inline void PlanningNodeClass::Detach(PlanningMemberClass * member) // RA2: 0x00611E20; expanded in Remove_Member and kept out of line too
{
	if (member->Get_Active()) { // RA2: as above
		Deactivate(member); // RA2: as above
	}
	if (member->Get_Branch() != -1) { // RA2: as above
		PlanningBranchClass * branch = PlanningBranches[member->Get_Branch()]; // RA2: as above
		branch->Set_Color(-1); // RA2: as above
		Reset_Colors(false); // RA2: as above
		if (branch->Dec_Count() == 0) { // RA2: as above
			Planning_Queue_Node(this); // RA2: as above
		}
	}
	member->Set_Branch(-1); // RA2: as above
}

void PlanningNodeClass::Clear(void) // RA2: 0x00611EE0
{
	int branches = PlanningBranches.Count(); // RA2: as above
	for (int i = 0; i < branches; i++) { // RA2: as above
		delete PlanningBranches[i]; // RA2: as above
	}
	PlanningBranches.Clear(); // RA2: as above
	int members = PlanningMembers.Count(); // RA2: as above
	for (int j = 0; j < members; j++) { // RA2: as above
		PlanningMemberClass * member = PlanningMembers[j]; // RA2: as above
		member->Set_Branch(-1); // RA2: as above
		member->Set_Active(false); // RA2: as above
	}
	LoopBranch.Clear_Count(); // RA2: as above
	LoopBranchIndex = -1; // RA2: as above
}

bool PlanningNodeClass::Is_Loop_Start(TechnoClass * techno) // RA2: 0x00611F60
{
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	return token->PlanningNodes.ID(this) == token->StepsToClosedLoop; // RA2: as above
}

void PlanningNodeClass::Deactivate(PlanningMemberClass * member) // RA2: 0x00611FA0
{
	member->Set_Active(false); // RA2: as above
	if (LoopBranch.Dec_Count() == 0) { // RA2: as above
		LoopBranchIndex = -1; // RA2: as above
	}
}

void PlanningNodeClass::Locate_Loop_Branch(void) // RA2: 0x00611FD0
{
	Find_Loop_Branch(this); // RA2: as above
}

void PlanningNodeClass::Select_All(void) // RA2: 0x00612090
{
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		TechnoClass * owner = PlanningMembers[i]->Owner; // RA2: as above
		if (!owner->IsSelected) { // RA2: as above
			owner->Select(); // RA2: as above
		}
	}
}

void PlanningNodeClass::Unselect_All(void) // RA2: 0x006120C0
{
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		TechnoClass * owner = PlanningMembers[i]->Owner; // RA2: as above
		if (owner->IsSelected) { // RA2: as above
			owner->Unselect(); // RA2: as above
		}
	}
}


void __fastcall Planning_Add_To_Drawn(PlanningNodeClass * node); // RA2: 0x006157C0, defined below

void PlanningNodeClass::Draw_Lead_Box(int color) // RA2: 0x006120F0; the marker is listed among the drawn nodes when it is on the view
{
	Rect clip = Get_Title_Screen_Area(); // RA2: as above
	Rect const & rect = Lead_Rect(); // RA2: as above
	CompositeSurface->Draw_Rect(clip, rect, color); // RA2: as above
	if (clip.Is_Overlapping(rect)) { // RA2: as above
		Planning_Add_To_Drawn(this); // RA2: as above
	}
}

// RA2: the one-pixel shrink is ra2-decomp's unit-local twin helper (no function of its own; expanded in the lead box members)
static inline void Shrink_Rect(Rect & r)
{
	if (r.Width <= -2) goto none; // RA2: as above
	if (r.Height <= -2) { // RA2: as above
	none:
		r.Set(0, 0, 0, 0); // RA2: as above
	} else {
		r.X += 1; // RA2: as above
		r.Y += 1; // RA2: as above
		r.Width -= 2; // RA2: as above
		r.Height -= 2; // RA2: as above
	}
}

void PlanningNodeClass::Fill_Lead_Box(int color) // RA2: 0x00612230
{
	Rect rect; // RA2: as above
	Rect clip = Get_Title_Screen_Area(); // RA2: as above
	rect = Lead_Rect(); // RA2: as above
	Shrink_Rect(rect); // RA2: as above
	CompositeSurface->Fill_Rect(clip, rect, color); // RA2: as above
}

void PlanningNodeClass::Frame_Lead_Box(int color) // RA2: 0x00612300
{
	Rect clip = Get_Title_Screen_Area(); // RA2: as above
	Rect rect = Lead_Box_Rect(); // RA2: as above
	for (int i = 0; i < 3; i++) { // RA2: as above
		CompositeSurface->Draw_Rect(clip, rect, color); // RA2: as above
		Shrink_Rect(rect); // RA2: as above
	}
}

void PlanningNodeClass::Frame_Lead_Box_Outer(int color) // RA2: 0x00612420
{
	Rect clip = Get_Title_Screen_Area(); // RA2: as above
	Rect rect = Lead_Box_Rect2(); // RA2: as above
	for (int i = 0; i < 3; i++) { // RA2: as above
		CompositeSurface->Draw_Rect(clip, rect, color); // RA2: as above
		Shrink_Rect(rect); // RA2: as above
	}
}

void __fastcall Planning_Inflate(Rect & rect, int amount) // RA2: 0x00612570; grows a rectangle by amount on every side; one that would vanish becomes empty
{
	int twice = amount * 2; // RA2: as above
	if (twice < 0 && (twice >= rect.Width || twice >= rect.Height)) { // RA2: as above
		rect.X = 0; // RA2: as above
		rect.Y = 0; // RA2: as above
		rect.Width = 0; // RA2: as above
		rect.Height = 0; // RA2: as above
		return; // RA2: as above
	}
	rect.X -= amount; // RA2: as above
	rect.Y -= amount; // RA2: as above
	rect.Width += twice; // RA2: as above
	rect.Height += twice; // RA2: as above
}

static inline RGBClass Planning_House_Color(TechnoClass * techno) // RA2: no function of its own; the remap color of the house a techno plans for
{
	return Houses[Planning_House(techno)]->RemapColorRGB; // RA2: as above
}

// RA2: the path tint is ra2-decomp's twin text
enum WaypointTint { WAYPOINT_NORMAL, WAYPOINT_BRIGHT };

static inline int Unit_Path_Tint(TechnoClass * techno, WaypointTint tint)
{
	int slot = Planning_House(techno); // RA2: as above
	RGBClass color = Houses[slot]->RemapColorRGB; // RA2: as above
	if (tint == WAYPOINT_NORMAL) { // RA2: as above
		return Planning_Adjusted_Pixel(color, BlackColor, 80); // RA2: as above
	}
	return Planning_Adjusted_Pixel(color, WhiteColor, 100); // RA2: as above
}

int PlanningNodeClass::Node_Color(void) // RA2: 0x006125D0; the color of a planning node
{
	int result = Color; // RA2: as above
	if (result == -1) { // RA2: as above
		TechnoClass * owner = PlanningMembers[0]->Owner; // RA2: as above
		result = Unit_Path_Tint(owner, WAYPOINT_NORMAL); // RA2: as above
		Color = result; // RA2: as above
		int count = PlanningBranches.Count(); // RA2: as above
		for (int i = 0; i < count; i++) { // RA2: as above
			if (PlanningBranches[i]->Get_Color() == result) // RA2: as above
				goto done; // RA2: as above
		}
		if (count == 0) { // RA2: as above
			PlanningTokenClass * token = owner->Get_Planning_Token(); // RA2: as above
			bool past = token->Reached(token->Node_Index(this)); // RA2: as above
			if (past) { // RA2: as above
				Color = Unit_Path_Tint(owner, WAYPOINT_BRIGHT); // RA2: as above
			}
		} else {
			Color = Unit_Path_Tint(owner, WAYPOINT_BRIGHT); // RA2: as above
		}
		result = Color; // RA2: as above
	}
done:
	return result; // RA2: as above
}

int PlanningNodeClass::Branch_Color(int index) // RA2: 0x006127E0
{
	PlanningBranchClass * branch = PlanningBranches[index]; // RA2: as above
	if (branch->Color != -1) return branch->Color; // RA2: as above
	TechnoClass * last_owner = NULL; // RA2: as above
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; ++i) { // RA2: as above
		PlanningMemberClass * member = PlanningMembers[i]; // RA2: as above
		if (member->Branch == index) { // RA2: as above
			TechnoClass * owner = member->Owner; // RA2: as above
			PlanningTokenClass * token = owner->Get_Planning_Token(); // RA2: as above
			if (!token->Reached(token->Node_Index(this))) { // RA2: as above
				RGBClass color = Houses[Planning_House(owner)]->RemapColorRGB; // RA2: as above
				color.Adjust(0x50, BlackColor); // RA2: as above
				return branch->Color = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue()); // RA2: as above
			}
			last_owner = owner; // RA2: as above
		}
	}
	if (last_owner == NULL) { // RA2: as above
		return -1; // RA2: as above
	} else { // RA2: as above
		RGBClass color = Houses[Planning_House(last_owner)]->RemapColorRGB; // RA2: as above
		color.Adjust(0x64, WhiteColor); // RA2: as above
		return branch->Color = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue()); // RA2: as above
	}
}

int PlanningNodeClass::Loop_Color(void) // RA2: 0x006129B0
{
	if (LoopBranch.Color != -1) return LoopBranch.Color; // RA2: as above
	TechnoClass * last_owner = NULL; // RA2: as above
	int count = PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; ++i) { // RA2: as above
		PlanningMemberClass * member = PlanningMembers[i]; // RA2: as above
		if (member->Active) { // RA2: as above
			TechnoClass * owner = member->Owner; // RA2: as above
			PlanningTokenClass * token = owner->Get_Planning_Token(); // RA2: as above
			if (!token->Get_F98()) { // RA2: as above
				RGBClass color = Houses[Planning_House(owner)]->RemapColorRGB; // RA2: as above
				color.Adjust(0x50, BlackColor); // RA2: as above
				return LoopBranch.Color = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue()); // RA2: as above
			}
			last_owner = owner; // RA2: as above
		}
	}
	if (last_owner == NULL) { // RA2: as above
		return -1; // RA2: as above
	} else { // RA2: as above
		RGBClass color = Houses[Planning_House(last_owner)]->RemapColorRGB; // RA2: as above
		color.Adjust(0x64, WhiteColor); // RA2: as above
		return LoopBranch.Color = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue()); // RA2: as above
	}
}

void PlanningNodeClass::Reset_Colors(bool all) // RA2: 0x00612B50
{
	Color = -1; // RA2: as above
	if (all) { // RA2: as above
		int count = PlanningBranches.Count(); // RA2: as above
		int i = 0; // RA2: as above
		if (count > 0) do { // RA2: as above
			PlanningBranches[i]->Set_Color(-1); // RA2: as above
			i++; // RA2: as above
		} while (i < count); // RA2: as above
		LoopBranch.Set_Color(-1); // RA2: as above
	}
}

Rect PlanningNodeClass::Lead_Box_Rect(void) // RA2: 0x00612B90
{
	Rect rect = Lead_Rect(); // RA2: as above
	Planning_Inflate(rect, 4); // RA2: as above
	return rect; // RA2: as above
}

Rect PlanningNodeClass::Lead_Box_Rect2(void) // RA2: 0x00612C30
{
	Rect rect = Lead_Box_Rect(); // RA2: as above
	Planning_Inflate(rect, 4); // RA2: as above
	return rect; // RA2: as above
}

PlanningTokenClass::PlanningTokenClass(TechnoClass * owner) : F1C(false), F8C(-1), ClosedLoopNodeCount(-1), StepsToClosedLoop(-1), F98(false), F99(false) // RA2: 0x00612CC0
{
	OwnerUnit = owner; // RA2: as above
}

PlanningTokenClass::~PlanningTokenClass() // RA2: 0x00612D20
{
	if (OwnerUnit) { // RA2: as above
		OwnerUnit->Set_Planning_Token(0); // RA2: as above
	}
	Planning_Unlist_Token(this); // RA2: as above
	Cancel_Owner(); // RA2: as above
}

TechnoClass * PlanningTokenClass::Get_Owner(void) // RA2: 0x00610840
{
	return OwnerUnit; // RA2: as above
}

inline void PlanningTokenClass::Cancel_Owner(void) // RA2: 0x00612EC0; expanded in the destructor and kept out of line too
{
	int count = PlanningNodes.Count(); // RA2: as above
	bool none = (count == 0); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningNodeClass * node = PlanningNodes[i]; // RA2: as above
		node->Remove_Member(Get_Owner()); // RA2: as above
	}
	if (!none) { // RA2: as above
		Planning_Unlist_Techno(OwnerUnit); // RA2: as above
	}
	PlanningNodes.Clear(); // RA2: as above
	F8C = -1; // RA2: as above
	ClosedLoopNodeCount = -1; // RA2: as above
	StepsToClosedLoop = -1; // RA2: as above
	F98 = false; // RA2: as above
	F99 = false; // RA2: as above
}

void PlanningTokenClass::Destroy(void) // RA2: 0x006130B0
{
	Cancel_Owner(); // RA2: as above
	OwnerUnit = 0; // RA2: as above
	delete this; // RA2: as above
}

void PlanningTokenClass::Set_F1C(void) // RA2: 0x00611710
{
	F1C = true; // RA2: as above
}

void PlanningTokenClass::Clear_F1C(void) // RA2: 0x006132D0
{
	F1C = false; // RA2: as above
}

bool PlanningTokenClass::Get_F1C(void) // RA2: 0x00611720
{
	return F1C; // RA2: as above
}

bool PlanningTokenClass::Owner_Is_Mission(void) // RA2: 0x006132F0
{
	return OwnerUnit->CurrentMission == MISSION_WAIT || OwnerUnit->MissionQueue == MISSION_WAIT; // RA2: as above
}

bool PlanningTokenClass::Has_Started(void) // RA2: 0x00613310
{
	return PlanningNodes.Count() > 0 && (F8C == -1 || F8C > 0); // RA2: as above
}

static inline void Finish_Consumed_Step(PlanningTokenClass * token) // RA2: no function of its own; the end of Pop_Front
{
	token->F8C = max(-1, token->F8C - 1); // RA2: as above
	if (token->Node_Count() == 0) { // RA2: as above
		Planning_Unlist_Techno(token->Get_Owner()); // RA2: as above
	}
}

EventClass PlanningTokenClass::Pop_Front(void) // RA2: 0x00613330
{
	PlanningNodeClass * node = PlanningNodes[0]; // RA2: as above
	EventClass packet = Event_At(0); // RA2: as above
	Popped = packet; // RA2: as above
	PlanningNodes.Delete_Index(0); // RA2: as above
	if (Loop_Start_Zero()) { // RA2: as above
		F99 = true; // RA2: as above
		PlanningNodes.Add(node); // RA2: as above
		Planning_Queue_Node(node); // RA2: as above
	} else { // RA2: as above
		node->Remove_Member(OwnerUnit); // RA2: as above
		if (Loop_Start_Positive()) { // RA2: as above
			Dec_Loop_Start(); // RA2: as above
		}
	}
	Finish_Consumed_Step(this); // RA2: as above
	return packet; // RA2: as above
}

void PlanningTokenClass::Share_Range(TechnoClass * techno, int start, int count) // RA2: 0x00613550
{
	if (techno->Get_Planning_Token()) { // RA2: as above
		for (int i = start; i < count + start; i++) { // RA2: as above
			PlanningNodeClass * node = PlanningNodes[i]; // RA2: as above
			EventClass event = node->Get_Event_For(OwnerUnit); // RA2: as above
			event.Data.Target.Whom = TargetClass(techno); // RA2: as above
			node->Add_Member(techno, &event); // RA2: as above
		}
	}
}

void PlanningTokenClass::Share_From(TechnoClass * techno, int start) // RA2: 0x00613720
{
	int count = PlanningNodes.Count() - start; // RA2: as above
	Share_Range(techno, start, count); // RA2: as above
}

void PlanningTokenClass::Remove_Node_At(int index) // RA2: 0x006138F0
{
	if (PlanningNodes.Count() == 1) { // RA2: as above
		F8C = -1; // RA2: as above
	} else if (F8C > 0 && index < F8C) { // RA2: as above
		F8C--; // RA2: as above
	}
	if (StepsToClosedLoop >= 0) { // RA2: as above
		if (index < StepsToClosedLoop) { // RA2: as above
			StepsToClosedLoop--; // RA2: as above
		} else { // RA2: as above
			ClosedLoopNodeCount--; // RA2: as above
			if (ClosedLoopNodeCount < 3) { // RA2: as above
				StepsToClosedLoop = -1; // RA2: as above
				ClosedLoopNodeCount = 0; // RA2: as above
			}
		}
	}
	PlanningNodeClass * node = PlanningNodes[index]; // RA2: as above
	node->Remove_Member(OwnerUnit); // RA2: as above
	PlanningNodes.Delete_Index(index); // RA2: as above
}

void PlanningTokenClass::Reset_Node_Colors(void) // RA2: 0x00613A80
{
	if (F98) { // RA2: as above
		PlanningNodeClass * node = Loop_Node(); // RA2: as above
		if (node != NULL) node->Reset_Colors(true); // RA2: as above
		F98 = false; // RA2: as above
	}
	int start = F8C == -1 ? 0 : F8C; // RA2: as above
	int count = PlanningNodes.Count(); // RA2: as above
	for (int index = start; index < count; index++) { // RA2: as above
		PlanningNodeClass * node = PlanningNodes[index]; // RA2: as above
		if (node != NULL) node->Reset_Colors(true); // RA2: as above
	}
	F8C = -1; // RA2: as above
}

int PlanningTokenClass::Get_F8C(void) // RA2: 0x00613B30
{
	return F8C; // RA2: as above
}

bool PlanningTokenClass::Reached(int index) // RA2: 0x00613B40
{
	if (F8C == -1) return false; // RA2: as above
	return index >= F8C; // RA2: as above
}

int PlanningTokenClass::Node_Count(void) // RA2: 0x00613B60
{
	return PlanningNodes.Count(); // RA2: as above
}

int PlanningTokenClass::Current_Or_Count(void) // RA2: 0x00613B70
{
	if (F8C == -1) return PlanningNodes.Count(); // RA2: as above
	return F8C; // RA2: as above
}

int PlanningTokenClass::Remaining(void) // RA2: 0x00613B80
{
	if (F8C == -1) return 0; // RA2: as above
	return PlanningNodes.Count() - F8C; // RA2: as above
}

void PlanningTokenClass::Add_Node(PlanningNodeClass * node) // RA2: 0x00613BA0
{
	PlanningNodes.Add(node); // RA2: as above
	if (F8C == -1) { // RA2: as above
		F8C = PlanningNodes.Count() - 1; // RA2: as above
	}
}

PlanningNodeClass * PlanningTokenClass::Node_At(int index) // RA2: 0x00613C00
{
	int count = PlanningNodes.Count(); // RA2: as above
	if (count == 0) return 0; // RA2: as above
	if (index >= count) { // RA2: as above
		int start = StepsToClosedLoop; // RA2: as above
		if (!Loop_Start_Valid()) return 0; // RA2: as above
		index = (index - start) % ClosedLoopNodeCount + start; // RA2: as above
	}
	if (index < 0 || index >= count) return 0; // RA2: as above
	return PlanningNodes[index]; // RA2: as above
}

PlanningNodeClass * PlanningTokenClass::Last_Node(void) // RA2: 0x00613C50
{
	int index = PlanningNodes.Count() - 1; // RA2: as above
	if (index < 0) return 0; // RA2: as above
	return PlanningNodes[index]; // RA2: as above
}

bool PlanningTokenClass::Has_Node(PlanningNodeClass * node) // RA2: 0x00613C60
{
	return PlanningNodes.ID(node) != -1; // RA2: as above
}

int PlanningTokenClass::Node_Index(PlanningNodeClass * node) // RA2: 0x00613C80
{
	return PlanningNodes.ID(node); // RA2: as above
}

EventClass PlanningTokenClass::Event_At(int index) // RA2: 0x00613CA0
{
	PlanningNodeClass * node = PlanningNodes[index]; // RA2: as above
	TechnoClass * owner = OwnerUnit; // RA2: as above
	return *node->Get_Member(owner)->Packet; // RA2: as above
}

EventClass PlanningTokenClass::Last_Popped(void) // RA2: 0x00613D00
{
	return Popped; // RA2: as above
}

PlanningNodeClass * PlanningTokenClass::Loop_Node(void) // RA2: 0x00613D20
{
	if (StepsToClosedLoop == -1) return 0; // RA2: as above
	return PlanningNodes[StepsToClosedLoop]; // RA2: as above
}

void PlanningTokenClass::Set_Loop(int count, int start) // RA2: 0x00613D40
{
	ClosedLoopNodeCount = count; // RA2: as above
	StepsToClosedLoop = start; // RA2: as above
	F98 = true; // RA2: as above
}

int PlanningTokenClass::Get_Loop_Count(void) // RA2: 0x00613D60
{
	return ClosedLoopNodeCount; // RA2: as above
}

int PlanningTokenClass::Get_Loop_Start(void) // RA2: 0x00613D70
{
	return StepsToClosedLoop; // RA2: as above
}

bool PlanningTokenClass::Loop_Start_Valid(void) // RA2: 0x00613D80
{
	return StepsToClosedLoop > 0 || StepsToClosedLoop == 0; // RA2: as above
}

bool PlanningTokenClass::Loop_Start_Positive(void) // RA2: 0x00613DA0
{
	return StepsToClosedLoop > 0; // RA2: as above
}

bool PlanningTokenClass::Loop_Start_Zero(void) // RA2: 0x00613DB0
{
	return StepsToClosedLoop == 0; // RA2: as above
}

bool PlanningTokenClass::Get_F98(void) // RA2: 0x00613DC0
{
	return F98; // RA2: as above
}

bool PlanningTokenClass::Get_F99(void) // RA2: 0x00613DD0
{
	return F99; // RA2: as above
}

void PlanningTokenClass::Dec_Loop_Start(void) // RA2: 0x00613DE0
{
	StepsToClosedLoop--; // RA2: as above
	StepsToClosedLoop = (StepsToClosedLoop <= 0) ? 0 : StepsToClosedLoop; // RA2: as above
}

void Planning_Set_Sink(INoticeSink * sink) // RA2: 0x00613E00
{
	PlanningSink = sink; // RA2: as above
}

void __cdecl Planning_Reset(void) // RA2: 0x00613E10
{
	PlanningNodeClass::PlanningModeActive = false; // RA2: as above
	PlanningTearDown = false; // RA2: as above
	PlanningHoverNode = NULL; // RA2: as above
	PlanningPickedNode = NULL; // RA2: as above
	PlanningSelectedNode = NULL; // RA2: as above
	PlanningMouseDown = Point2D(0, 0); // RA2: as above
	PlanningDragging = false; // RA2: as above
}

void __cdecl Planning_Reset_Wrapper(void) // RA2: 0x00613E40
{
	Planning_Reset_State(); // RA2: as above
}

static inline void Reset_Planning_Command_Flags(void)
{
	PlanningCommandFlag = false; // RA2: as above
	PlanningLoopClosed = false; // RA2: as above
}

void __cdecl Planning_Reset_State(void) // RA2: 0x00613E50; deletes every token and clears every list and flag
{
	int count = PlanningTokenClass::Array.Count(); // RA2: as above
	PlanningTearDown = true; // RA2: as above
	PlanningNodeClass::PlanningModeActive = false; // RA2: as above
	PlanningBusy = false; // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningTokenClass * token = PlanningTokenClass::Array[i]; // RA2: as above
		delete token; // RA2: as above
	}
	memset(PlanningUnitCounts, 0, sizeof(PlanningUnitCounts)); // RA2: as above
	PlanningTokenClass::Technos.Clear(); // RA2: as above
	PlanningTokenClass::Array.Clear(); // RA2: as above
	PlanningNodeClass::Unknown1.Clear(); // RA2: as above
	PlanningNodeClass::DrawnNodes.Clear(); // RA2: as above
	PlanningHoverNode = NULL; // RA2: as above
	Reset_Planning_Command_Flags(); // RA2: as above
	PlanningBlockedNode = 0; // RA2: as above
	PlanningRefusalShown = false; // RA2: as above
	PlanningTearDown = false; // RA2: as above
	PlanningPendingRefusal = -1; // RA2: as above
	memset(PlanningHelpCounts, 0, sizeof(PlanningHelpCounts)); // RA2: as above
}

void __fastcall Planning_Update(void) // RA2: 0x00614010; the per frame update: drops the steps whose target is gone and shows a pending refusal
{
	Planning_Hover(); // RA2: as above
	if (PlanningSelectedNode && CurrentObject.Count() > 0) { // RA2: as above
		PlanningSelectedNode = 0; // RA2: as above
	}
	int count = PlanningTokenClass::Array.Count(); // RA2: as above
	int i = 0; // RA2: as above
	if (count > 0) { // RA2: as above
		do { // RA2: as above
			PlanningTokenClass * token = PlanningTokenClass::Array[i]; // RA2: as above
			TechnoClass * owner = token->OwnerUnit; // RA2: as above
			for (int j = token->PlanningNodes.Count() - 1; j >= 0; --j) { // RA2: as above
				EventClass event = token->Event_At(j); // RA2: as above
				bool remove = true; // RA2: as above
				if (event.Type == EventClass::MEGAMISSION) { // RA2: as above
					if (event.Data.MegaMission.Target.Is_Valid()) { // RA2: as above
						remove = event.Data.MegaMission.Target.As_Abstract() == 0; // RA2: as above
					} else if (event.Data.MegaMission.Destination.Is_Valid()) { // RA2: as above
						remove = event.Data.MegaMission.Destination.As_Abstract() == 0; // RA2: as above
					}
				}
				if (!remove) { // RA2: as above
					continue; // RA2: as above
				}
				PlanningNodeClass * node = token->Node_At(j); // RA2: as above
				PlanningNodeClass * next = node->Next_Node(owner); // RA2: as above
				Planning_Queue_Node(next); // RA2: as above
				token->Remove_Node_At(j); // RA2: as above
			}
		} while (count > ++i); // RA2: as above
	}
	Planning_Process_Queue(); // RA2: as above
	int refusal = PlanningPendingRefusal; // RA2: as above
	if (refusal != -1) { // RA2: as above
		Planning_Refuse_Command(refusal); // RA2: as above
		PlanningPendingRefusal = -1; // RA2: as above
	}
	Reset_Planning_Command_Flags(); // RA2: as above
	PlanningRefusalShown = false; // RA2: as above
}

void __fastcall Planning_Free_Nodes(void) // RA2: 0x006142F0; runs the planned events still held and frees them
{
	for (int i = PlanningTokenClass::Events.Count() - 1; i >= 0; i--) { // RA2: as above
		EventClass * event = PlanningTokenClass::Events[i]; // RA2: as above
		Enqueue_Event(event); // RA2: as above
		PlanningTokenClass::Events.Delete_Index(i); // RA2: as above
		delete event; // RA2: as above
	}
}

void __fastcall Planning_Store_Event(EventClass const & source) // RA2: 0x00614350
{
	EventClass * event = new EventClass; // RA2: as above
	*event = source;
	if (event->Type == EventClass::MEGAMISSION) { // RA2: as above
		event->Data.MegaMission.IsPlanningEvent = 2; // RA2: as above
	}
	PlanningTokenClass::Events.Add(event); // RA2: as above
}

void __fastcall Planning_Unlist_Node(PlanningNodeClass * node) // RA2: 0x006143E0
{
	PlanningNodeClass::QueuedNodes.Delete(node); // RA2: as above
	PlanningNodeClass::Unknown1.Delete(node); // RA2: as above
	PlanningNodeClass::DrawnNodes.Delete(node); // RA2: as above
	if (PlanningHoverNode == node) { // RA2: as above
		PlanningHoverNode = NULL; // RA2: as above
	}
	if (PlanningPickedNode == node) { // RA2: as above
		PlanningPickedNode = NULL; // RA2: as above
	}
	if (PlanningSelectedNode == node) { // RA2: as above
		PlanningSelectedNode = NULL; // RA2: as above
	}
}

void __fastcall Planning_Unlist_Token(PlanningTokenClass * token) // RA2: 0x006144F0
{
	if (!PlanningTearDown) { // RA2: as above
		PlanningTokenClass::Array.Delete(token); // RA2: as above
	}
}

void __fastcall Planning_Unlist_Techno(TechnoClass * techno) // RA2: 0x00614550
{
	int house = Planning_House(techno); // RA2: as above
	Planning_Uncount(house); // RA2: as above
	PlanningTokenClass::Technos.Delete(techno); // RA2: as above
}

void TechnoClass::Select_Planning_Members(void) // RA2: 0x006145E0
{
	if (!PlanningBusy) { // RA2: as above
		PlanningBusy = true; // RA2: as above
		if (PlanningNodeClass::PlanningModeActive) { // RA2: as above
			PlanningTokenClass * token = Get_Planning_Token(); // RA2: as above
			if (token) { // RA2: as above
				PlanningNodeClass * node = token->Last_Node(); // RA2: as above
				if (node) { // RA2: as above
					node->Select_All(); // RA2: as above
				}
			}
		}
		PlanningBusy = false; // RA2: as above
	}
}

void __fastcall Planning_Queue_Node(PlanningNodeClass * node) // RA2: 0x00614650
{
	if (node && !node->Get_Queued()) { // RA2: as above
		node->Set_Queued(); // RA2: as above
		PlanningNodeClass::QueuedNodes.Add(node); // RA2: as above
	}
}

void __cdecl Planning_Process_Queue(void) // RA2: 0x006146C0
{
	int count = PlanningNodeClass::QueuedNodes.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningNodeClass * node = PlanningNodeClass::QueuedNodes[i]; // RA2: as above
		node->Rebuild(); // RA2: as above
	}
	PlanningNodeClass::QueuedNodes.Clear(); // RA2: as above
}

void __cdecl Planning_Begin(void) // RA2: 0x00614760; starts planning mode: sound and a notice to the sink
{
	if (!PlanningNodeClass::PlanningModeActive) { // RA2: as above
		PlanningNodeClass::PlanningModeActive = true; // RA2: as above
		VocClass::PlayGlobal(Rule->StartPlanningModeSound, 1.0f, 0x2000); // RA2: as above
		if (PlanningSink) { // RA2: as above
			PlanningSink->Unknown_Virtual_0(0x11C7, 0); // RA2: as above
		}
	}
}

void __cdecl Planning_End(void) // RA2: 0x006147B0; ends planning mode: sends the commit event, sound and a notice to the sink
{
	if (PlanningNodeClass::PlanningModeActive) { // RA2: as above
		EventClass event(PlayerPtr->HeapID, EventClass::PLAN_COMMIT); // RA2: as above
		Add_Outgoing_Event(event); // RA2: as above
		PlanningHoverNode = NULL; // RA2: as above
		PlanningPickedNode = NULL; // RA2: as above
		PlanningSelectedNode = NULL; // RA2: as above
		PlanningNodeClass::PlanningModeActive = false; // RA2: as above
		VocClass::PlayGlobal(Rule->EndPlanningModeSound, 1.0f, 0x2000); // RA2: as above
		if (PlanningSink) { // RA2: as above
			PlanningSink->Unknown_Virtual_0(0x11C8, 0); // RA2: as above
		}
	}
}

bool Planning_Is_Active(void) // RA2: 0x00614840
{
	return PlanningNodeClass::PlanningModeActive; // RA2: as above
}

void __fastcall Commit_Plans(int house) // RA2: 0x00614850; for every object of a house: starts the path again when it is not under way and its mission allows it; the expansion of TechnoClass::TryNextPlanningTokenNode (0x00615360) and of Planning_Mission_Allows stand in it
{
	int count = Technos.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		TechnoClass * techno = Technos[i]; // RA2: as above
		if (house == Planning_House(techno)) { // RA2: as above
			PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
			if (token != NULL) { // RA2: as above
				bool started = token->Has_Started(); // RA2: as above
				token->Reset_Node_Colors(); // RA2: as above
				bool permitted = Planning_Mission_Allows(techno); // RA2: as above
				if (!started && permitted) { // RA2: as above
					techno->TryNextPlanningTokenNode(); // RA2: as above
				}
			}
		}
	}
}

void PlanningNodeClass::Send_Delete_Event(void) // RA2: 0x00614AA0
{
	EventClass event = *PlanningMembers[0]->Packet; // RA2: as above
	xTargetClass * target; // RA2: as above
	if (event.Data.MegaMission.Target.Is_Valid()) { // RA2: as above
		target = &event.Data.MegaMission.Target; // RA2: as above
	} else if (event.Data.MegaMission.Follow.Is_Valid()) { // RA2: as above
		target = &event.Data.MegaMission.Follow; // RA2: as above
	} else if (event.Data.MegaMission.Destination.Is_Valid()) { // RA2: as above
		target = &event.Data.MegaMission.Destination; // RA2: as above
	} else { // RA2: as above
		return; // RA2: as above
	}
	AbstractClass * object = target->As_Abstract(); // RA2: as above
	if (object) { // RA2: as above
		EventClass remove(PlayerPtr->HeapID, EventClass::PLAN_NODE_DELETE, TargetClass(object)); // RA2: as above
		Add_Outgoing_Event(remove); // RA2: as above
	}
}

bool __fastcall Planning_Allows_Event_Type(int type) // RA2: 0x00614B50; whether an event type may be sent now: no mission events while planning mode is on
{
	if (PlanningNodeClass::PlanningModeActive && type == 4) { // RA2: as above
		return false; // RA2: as above
	}
	return true; // RA2: as above
}

void __stdcall Planning_Send_Event(EventClass event) // RA2: 0x00614B70; sends a mission event marked as planned
{
	if (event.Type == EventClass::MEGAMISSION) { // RA2: as above
		event.Data.MegaMission.IsPlanningEvent = true; // RA2: as above
		Add_Outgoing_Event(event); // RA2: as above
	}
}


inline bool __fastcall Planning_Target_Fits(int mission, TargetClass target) // RA2: no function of its own; whether an attack or an area guard order aims at a cell or (for the area guard) at nothing
{
	if (mission == 1 || mission == 11) { // RA2: as above
		do { // RA2: as above
			if (RTTIType(target) == RTTI_CELL) { // RA2: as above
				break; // RA2: as above
			}
			if (mission != 11 || target.Is_Valid()) { // RA2: as above
				return false; // RA2: as above
			}
		} while (false); // RA2: as above
		return true; // RA2: as above
	}
	return false; // RA2: as above
}

inline bool Planning_Valid_House(int house) // RA2: no function of its own; whether a house number indexes the counters
{
	return house >= 0 && house < 8; // RA2: as above
}

inline bool Planning_Token_Looped(PlanningTokenClass * token) // RA2: no function of its own; the path of the token has closed a loop
{
	return token->Loop_Start_Positive() || token->Loop_Start_Zero(); // RA2: as above
}

inline void __fastcall Planning_Attach_Order(PlanningNodeClass * const & node, PlanningTokenClass * token, TechnoClass * techno, EventClass * event) // RA2: no function of its own; puts the techno on the node: with the members of the last node of its path, or alone
{
	PlanningNodeClass * last = token->Last_Node(); // RA2: as above
	if (last) { // RA2: as above
		Planning_Copy_Members(node, last, *event); // RA2: as above
	} else { // RA2: as above
		node->Add_Member(techno, event); // RA2: as above
	}
}

inline int Planning_Token_Index(PlanningTokenClass * token, PlanningNodeClass * node) // RA2: no function of its own; the index of a node in the path of a token
{
	return token->Node_Index(node); // RA2: as above
}

int __fastcall Planning_Count(int house); // RA2: 0x00616D50
inline PlanningNodeClass * __fastcall Planning_Find_Lead_Node(int house, int id, Coord coord) // RA2: no function of its own; the planned node whose lead order is at a coordinate (of an id and a house unless -1)
{
	int count = PlanningNodeClass::Unknown1.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i]; // RA2: as above
		if (node->Lead_At(Coord(coord)) && (id == -1 || id == node->Id)) { // RA2: as above
			int owner = Planning_House(node->Member_Owner(0));
			if (house == -1 || house == owner) { // RA2: as above
				return node; // RA2: as above
			}
		}
	}
	return NULL; // RA2: as above
}

PlanningNodeClass * __fastcall Planning_Pick_Drawn(Point2D point); // RA2: 0x00616140
int __fastcall Planning_Selection_Node_State(PlanningNodeClass * node); // RA2: 0x00617A10
void __fastcall Planning_Message(char const * label); // RA2: 0x00616FB0
void __fastcall Planning_Refuse(char const * label); // RA2: 0x00616FD0
bool __fastcall Planning_Terminal_Mission(int mission); // RA2: 0x006159C0
bool __fastcall Planning_Cell_Mission(int mission, TargetClass target); // RA2: 0x006159E0
void __fastcall Planning_Refuse_Order(EventClass const * event); // RA2: 0x00617020
int __fastcall Planning_Event_Result(PlanningTokenClass * token, EventClass const * event); // RA2: 0x006171E0
void __fastcall Planning_Recount(int house); // RA2: 0x00616D00
void __fastcall Planning_Set_Pending_Refusal(int value); // RA2: 0x00617240
PlanningTokenClass * __fastcall Planning_New_Token(TechnoClass * techno); // RA2: 0x00615820
void __fastcall Planning_Close_Loop(TechnoClass * techno, PlanningNodeClass * node); // RA2: 0x006164E0
void __fastcall Planning_Path_Add(TechnoClass * techno, PlanningNodeClass * node); // RA2: 0x00616350
PlanningNodeClass * __fastcall Planning_Find_Node(int house, int id, CoordStruct coord); // RA2: 0x00615FB0
void __fastcall Planning_Cancel_Token(TechnoClass * techno); // RA2: 0x006155E0
bool __fastcall Planning_Same_Last_Node(PlanningTokenClass * first, PlanningTokenClass * second); // RA2: 0x00616BD0

void __fastcall Planning_Execute(EventClass * event); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00614ba0 has no row); main: best 89.13, tried 3 times (inline)
void __fastcall Planning_Execute(EventClass * event) // RA2: 0x00614BA0; carries out a planning event: plan commit restarts the paths of the event's house, plan connect adds the node at the second target's coordinate to the first target's path, plan node delete takes the node at the target's coordinate out of every path (remembering the next node for the player); any other event is an order for its techno
{
	if (event->Type == EventClass::PLAN_COMMIT) { // RA2: as above
		Commit_Plans(event->ID); // RA2: as above
	} else if (event->Type == EventClass::PLAN_CONNECT) { // RA2: as above
		TechnoClass * techno = event->Data.NavCom.Whom.As_Techno(); // RA2: as above
		if (techno) { // RA2: as above
			AbstractClass * object = event->Data.NavCom.Where.As_Abstract(); // RA2: as above
			if (object) { // RA2: as above
				Coord coord = object->As_Coord(); // RA2: as above
				PlanningNodeClass * node = Planning_Find_Node(event->ID, -1, coord); // RA2: as above
				if (node) { // RA2: as above
					Planning_Path_Add(techno, node); // RA2: as above
				}
			}
		}
	} else if (event->Type == EventClass::PLAN_NODE_DELETE) { // RA2: as above
		AbstractClass * object = event->Data.Target.Whom.As_Abstract(); // RA2: as above
		if (object) { // RA2: as above
			Coord coord = object->As_Coord(); // RA2: as above
			int house = event->ID; // RA2: as above
			PlanningNodeClass * node = NULL; // RA2: as above
			int count = PlanningNodeClass::Unknown1.Count(); // RA2: as above
			for (int i = 0; i < count; i++) { // RA2: as above
				PlanningNodeClass * lead = PlanningNodeClass::Unknown1[i]; // RA2: as above
				if (lead->Lead_At(coord) && (house == -1 || house == Planning_House(lead->PlanningMembers[0]->Owner))) { // RA2: as above
					node = lead; // RA2: as above
					break; // RA2: as above
				}
			}
			if (node) { // RA2: as above
				TechnoClass * owner = node->PlanningMembers[0]->Owner; // RA2: as above
				int index = owner->Get_Planning_Token()->Node_Index(node); // RA2: as above
				PlanningNodeClass * next = owner->Get_Planning_Token()->Node_At(index + 1); // RA2: as above
				Planning_Remove_Node(node); // RA2: as above
				if (next && house == PlayerPtr->HeapID) { // RA2: as above
					PlanningSelectedNode = next; // RA2: as above
				}
			}
		}
	} else { // RA2: as above
		TechnoClass * techno = event->Data.Target.Whom.As_Techno(); // RA2: as above
		Planning_Add_Order(techno, event); // RA2: as above
	}
}
#endif

bool __fastcall Planning_Add_Order(TechnoClass * techno, EventClass * event) // RA2: 0x00614EC0; plans an order for a techno: adds it to the node at the order's coordinate, or to a new node, unless the order or the techno's path refuses it
{
	if (!techno) { // RA2: as above
		return false; // RA2: as above
	}
	bool player = event->ID == PlayerPtr->HeapID; // RA2: as above
	Coord const & coord = Planning_Event_Coord(*event); // RA2: as above
	if (coord == Coord(-1, -1, -1)) { // RA2: as above
		return false; // RA2: as above
	}
	int id = event->Frame; // RA2: as above
	PlanningNodeClass * node = Planning_Find_Lead_Node(event->ID, id, coord); // RA2: as above
	if (node && node->Has_Member(techno)) { // RA2: as above
		return true; // RA2: as above
	}
	int kind = Planning_Order_Kind(techno, (EventClass const *)event); // RA2: as above
	if (kind != -1) { // RA2: as above
		if (!player) { // RA2: as above
			return false; // RA2: as above
		}
		if (kind == 0) { // RA2: as above
			Planning_Refuse_Order(event); // RA2: as above
		} else if (kind == 1) { // RA2: as above
			Planning_Refuse("MSG:PostTerminatingCommand"); // RA2: as above
		} else if (kind == 2) { // RA2: as above
			Planning_Refuse("MSG:PostContinualCommand"); // RA2: as above
		}
		return false; // RA2: as above
	}
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	if (!token) { // RA2: as above
		token = Planning_New_Token(techno); // RA2: as above
	}
	if (player) { // RA2: as above
		int result = Planning_Event_Result(token, (EventClass const *)event); // RA2: as above
		if (result != -1) { // RA2: as above
			Planning_Set_Pending_Refusal(result); // RA2: as above
		}
	}
	if (Planning_Token_Looped(token)) { // RA2: as above
		if (player) { // RA2: as above
			Planning_Refuse("MSG:PostLoop"); // RA2: as above
		}
		return false; // RA2: as above
	}
	if (token->Node_Count() == 0) { // RA2: as above
		PlanningTokenClass::Technos.Add(techno); // RA2: as above
		Planning_Recount(event->ID); // RA2: as above
	} else if (token->Node_Count() == 16) { // RA2: as above
		if (player) { // RA2: as above
			Planning_Refuse("MSG:NodeMaximum"); // RA2: as above
		}
		return false; // RA2: as above
	}
	if (!node) { // RA2: as above
		node = new PlanningNodeClass(event->Frame); // RA2: as above
		PlanningNodeClass::Unknown1.Add(node); // RA2: as above
	}
	Planning_Attach_Order(node, token, techno, event); // RA2: as above
	if (player) { // RA2: as above
		VocClass::PlayGlobal(Rule->AddPlanningModeCommandSound, 1.0f, 0x2000); // RA2: as above
	}
	return true; // RA2: as above
}

bool TechnoClass::TryNextPlanningTokenNode(void) // RA2: 0x00615360; starts carrying out the techno's planned path once: queues the planning mission, holds the token's first event and marks the token started
{
	PlanningTokenClass * token = Get_Planning_Token(); // RA2: as above
	if (token != NULL && token->Node_Count() > 0 && (token->Get_F8C() == -1 || token->Get_F8C() > 0) && !token->Get_F1C()) { // RA2: as above
		QueueMission(MISSION_WAIT, true); // RA2: as above
		EventClass event = token->Pop_Front(); // RA2: as above
		Planning_Store_Event(event); // RA2: as above
		token->Set_F1C(); // RA2: as above
		return true; // RA2: as above
	}
	return false; // RA2: as above
}

inline void __fastcall Planning_Uncount(int house) // RA2: 0x00616D20 (outside this part); lowers the count of a house, never below 0
{
	if (house >= 0 && house < 8) { // RA2: as above
		int value = PlanningUnitCounts[house] - 1; // RA2: as above
		PlanningUnitCounts[house] = (value < 0) ? 0 : value; // RA2: as above
	}
}

void __fastcall Planning_Clear_Techno(TechnoClass * techno, EventClass * event) // RA2: 0x00615480; after an order that is not a planned one, lets go of the planning token of a techno that has nodes, and lets the token run again
{
	if (techno) { // RA2: as above
		PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
		if (token) { // RA2: as above
			int planned = event ? event->Data.MegaMission.IsPlanningEvent : 0; // RA2: as above
			if (!planned && token->Node_Count() > 0) { // RA2: as above
				Planning_Cancel_Token(techno); // RA2: as above
			}
			token->Clear_F1C(); // RA2: as above
		}
	}
}

void __fastcall Planning_Cancel_Token(TechnoClass * techno) // RA2: 0x006155E0; lets go of the planning token of a techno, when it has one
{
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	if (token != NULL) { // RA2: as above
		token->Cancel_Owner(); // RA2: as above
	}
}

void __fastcall Planning_Add_To_Drawn(PlanningNodeClass * node) // RA2: 0x006157C0; puts a node on the list of the nodes drawn on the tactical view
{
	PlanningNodeClass::DrawnNodes.Add(node); // RA2: as above
}

PlanningTokenClass * __fastcall Planning_New_Token(TechnoClass * techno) // RA2: 0x00615820; makes the planning token of a techno and lists it
{
	PlanningTokenClass * token = new PlanningTokenClass(techno); // RA2: as above
	PlanningTokenClass::Array.Add(token); // RA2: as above
	techno->Set_Planning_Token(token); // RA2: as above
	return token; // RA2: as above
}

bool __fastcall Planning_Stop_Type(char type) // RA2: 0x006158F0; the order types planning mode refuses: deploy, stop and scatter
{
	return type == 9 || type == 6 || type == 7; // RA2: as above
}

bool __fastcall Planning_Order_Stays_Put(TechnoClass * techno, int mission, TargetClass target) // RA2: 0x00615910; an area guard order that aims at nothing, or at the cell the techno stands on already
{
	if (mission == 11) { // RA2: as above
		if (!target.Is_Valid()) { // RA2: as above
			return true; // RA2: as above
		}
		if (techno) { // RA2: as above
			Cell here = techno->Get_Cell(); // RA2: as above
			Cell cell = techno->Get_Cell(); // RA2: as above
			CellClass * target_cell = target.As_Cell(); // RA2: as above
			if (target_cell) { // RA2: as above
				cell = target_cell->CellID; // RA2: as above
			} else { // RA2: as above
				TechnoClass * target_techno = target.As_Techno(); // RA2: as above
				if (target_techno) { // RA2: as above
					cell = target_techno->Get_Cell(); // RA2: as above
				}
			}
			if (here == cell) { // RA2: as above
				return true; // RA2: as above
			}
		}
	}
	return false; // RA2: as above
}

bool __fastcall Planning_Terminal_Mission(int mission) // RA2: 0x006159C0 (no function of its own); the missions that end a path
{
	return mission == 7 || mission == 8 || mission == 9; // RA2: as above
}

bool __fastcall Planning_Cell_Mission(int mission, TargetClass target) // RA2: 0x006159E0; an attack or an area guard aimed at a cell, or for the area guard at nothing
{
	if (mission == 1 || mission == 11) { // RA2: as above
		if (RTTIType(target) == RTTI_CELL) { // RA2: as above
			return true; // RA2: as above
		}
		if (mission == 11 && !target.Is_Valid()) { // RA2: as above
			return true; // RA2: as above
		}
	}
	return false; // RA2: as above
}

bool __fastcall Planning_Mission_Allows(TechnoClass * techno) // RA2: 0x00615A10; whether the current mission of a techno lets planning mode take over: guarding, or attacking or guarding an area with a cell or (for the area) nothing as the target
{
	if (techno) { // RA2: as above
		int mission = techno->Get_Mission(); // RA2: as above
		if (mission == 5) { // RA2: as above
			return true; // RA2: as above
		}
		return Planning_Target_Fits(mission, TargetClass(techno->TarCom)); // RA2: as above
	}
	return false; // RA2: as above
}

int __fastcall Planning_Order_Kind(TechnoClass * techno, EventClass const * event) // RA2: 0x00615A80; how an order relates to the path of the techno: 0 refused outright, 1 and 2 past a terminating or a continual order, -1 taken
{
	if (Planning_Stop_Type(event->Type)) { // RA2: as above
		return 0; // RA2: as above
	}
	int order_mission = event->CurrentMission(); // RA2: as above
	TargetClass const & target = event->Target(); // RA2: as above
	if (Planning_Order_Stays_Put(techno, order_mission, target)) { // RA2: as above
		return 0; // RA2: as above
	}
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	if (token != NULL && token->Node_Count() > 0) { // RA2: as above
		EventClass last_event = token->Event_At(token->Node_Count() - 1); // RA2: as above
		EventClass const * targets = (EventClass const *)&last_event; // RA2: as above
		if (Planning_Terminal_Mission(targets->CurrentMission())) { // RA2: as above
			return 1; // RA2: as above
		}
		if (Planning_Cell_Mission(targets->CurrentMission(), targets->Target())) { // RA2: as above
			return 2; // RA2: as above
		}
	}
	return -1; // RA2: as above
}

void __fastcall Planning_Add_To_Nodes(PlanningNodeClass * node) // RA2: 0x00615B60; puts a node on the list of all nodes
{
	PlanningNodeClass::Unknown1.Add(node); // RA2: as above
}

void __cdecl Planning_Clear_Drawn(void) // RA2: 0x00615BC0; empties the list of the nodes drawn on the tactical view
{
	PlanningNodeClass::DrawnNodes.Clear(); // RA2: as above
}

// RA2: the node searches below are ra2-decomp's twin text
static inline PlanningNodeClass * Find_Node_At(DynamicVectorClass<PlanningNodeClass *> const & nodes, Point2D const & point)
{
	int count = nodes.Count(); // RA2: as above
	int i = 0; // RA2: as above
	if (count > 0) { // RA2: as above
		do {
			PlanningNodeClass * node = nodes[i]; // RA2: as above
			if (node->Hit_Test(point.X, point.Y)) return node; // RA2: as above
			++i; // RA2: as above
		} while (i < count);
	}
	return NULL; // RA2: as above
}

void __cdecl Planning_Hover(void) // RA2: 0x00615C00; the planning mode's mouse hover: the drawn node under the mouse, unless it is not the player's to use
{
	if (!PlanningDragging) { // RA2: as above
		int x = MouseCursor->Get_Mouse_X(); // RA2: as above
		int y = MouseCursor->Get_Mouse_Y(); // RA2: as above
		PlanningNodeClass * node = Find_Node_At(PlanningNodeClass::DrawnNodes, Point2D(x, y)); // RA2: as above
		if (node != PlanningHoverNode) { // RA2: as above
			PlanningBlockedNode = 0; // RA2: as above
			if (node) { // RA2: as above
				if (node->PlanningMembers[0]->Owner->House == PlayerPtr // RA2: as above
					&& (PlanningPickedNode == 0 || PlanningPickedNode == node)
					&& Planning_Selection_Node_State(node) == -1) {
				} else {
					PlanningBlockedNode = (int)node; // RA2: as above
					node = 0; // RA2: as above
				}
			}
			PlanningHoverNode = node; // RA2: as above
			if (node) { // RA2: as above
				node->Dump(); // RA2: as above
			}
		}
	}
}

static inline PlanningNodeClass * Hover_Node_At(Point2D point)
{
	PlanningNodeClass * hover = PlanningHoverNode; // RA2: as above
	PlanningNodeClass * hit; // RA2: as above
	if (!hover) { // RA2: as above
		hit = NULL; // RA2: as above
	} else {
		if (hover->Hit_Test(point.X, point.Y)) { // RA2: as above
			hit = PlanningHoverNode; // RA2: as above
		} else {
			hit = NULL; // RA2: as above
		}
	}

	return hit; // RA2: as above
}

PlanningNodeClass * __fastcall Planning_Node_At(Point2D point) // RA2: 0x00615D30; the node under the mouse when the point is inside its box
{
	return Hover_Node_At(point); // RA2: as above
}

bool __fastcall Is_Planning_Selection_Valid(void); // RA2: the definition is not exact yet
static inline bool Planning_Is_Techno(ObjectClass const * object)
{
	if (object == 0) return false;
	unsigned char flag = (object->AbstractFlags & ABSTRACT_TECHNO);
	return flag != 0;
}

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
		if (Planning_Is_Techno(object)) {
			PlanningTokenClass * first = ((TechnoClass *)object)->Get_Planning_Token();
			for (i = 0; i < count; i++) {
				ObjectClass * other = CurrentObject[i];
				if (!Planning_Is_Techno(other)) goto fail;
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
	if (PlanningNodeClass::PlanningModeActive) { // RA2: as above
		int total = Planning_Count(PlayerPtr->HeapID); // RA2: as above
		int added = 0; // RA2: as above
		int count = CurrentObject.Count(); // RA2: as above
		for (int i = 0; i < count; i++) { // RA2: as above
			ObjectClass * object = CurrentObject[i]; // RA2: as above
			bool techno; // RA2: as above
			if (object != NULL) { // RA2: as above
				techno = object->AbstractFlags & ABSTRACT_TECHNO; // RA2: as above
			}
			if (object != NULL && techno) { // RA2: as above
				PlanningTokenClass * token = ((TechnoClass *)object)->Get_Planning_Token(); // RA2: as above
				if (token == NULL) { // RA2: as above
					added++; // RA2: as above
				} else if (token->Node_Count() == 0) { // RA2: as above
					added++; // RA2: as above
				}
			}
		}
		if (total + added > 128) { // RA2: as above
			Planning_Refuse("MSG:PlannerMaximum"); // RA2: as above
			return false; // RA2: as above
		}
	}
	return true; // RA2: as above
}

PlanningNodeClass * __fastcall Planning_Find_Node(int house, int id, CoordStruct coord) // RA2: 0x00615FB0; the planned node whose lead order aims at the coordinate (of an id and a house unless -1)
{
	int count = PlanningNodeClass::Unknown1.Count();
	for (int i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i];
		if (node->Lead_At(Coord(coord)) && (id == -1 || id == node->Id)) {
			int owner = Planning_House(node->Member_Owner(0));
			if (house == -1 || house == owner) {
				return node;
			}
		}
	}
	return NULL;
}


PlanningNodeClass * __fastcall Planning_Pick_Node(Point2D point) // RA2: 0x00616070; the node (of all nodes) whose box holds the point
{
	return Find_Node_At(PlanningNodeClass::Unknown1, point); // RA2: as above
}

PlanningNodeClass * __fastcall Planning_Pick_Drawn(Point2D point) // RA2: 0x00616140; the drawn node whose box holds the point
{
	return Find_Node_At(PlanningNodeClass::DrawnNodes, point); // RA2: as above
}

PlanningNodeClass * __stdcall Planning_Find_Drawn(Coord coord) // RA2: 0x00616210; the first drawn node whose lead order aims at the coordinate
{
	int count = PlanningNodeClass::DrawnNodes.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		PlanningNodeClass * node = PlanningNodeClass::DrawnNodes[i]; // RA2: as above
		if (node->Lead_Coord() == coord) { // RA2: as above
			return node; // RA2: as above
		}
	}
	return NULL; // RA2: as above
}



void __fastcall Planning_Send_Pair(AbstractClass * source, PlanningNodeClass * node) // RA2: 0x00616280; asks all players to connect the object to the node of the target the first order of the node points at
{
	EventClass event = node->Get_Event(0); // RA2: as above
	AbstractClass * object = event.First_Target_Object(); // RA2: as above
	if (object) { // RA2: as above
		EventClass pair(PlayerPtr->HeapID, EventClass::PLAN_CONNECT, TargetClass(source), TargetClass(object)); // RA2: as above
		Add_Outgoing_Event(pair); // RA2: as above
	}
}

void __fastcall Planning_Path_Add(TechnoClass * techno, PlanningNodeClass * node) // RA2: 0x00616350; puts a node on the path of a techno, closing the path into a loop when it holds the node already
{
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	if (!token) { // RA2: as above
		token = Planning_New_Token(techno); // RA2: as above
	}
	if (token->Node_Index(node) != -1) { // RA2: as above
		Planning_Close_Loop(techno, node); // RA2: as above
	} else { // RA2: as above
		Planning_Path_Append(techno, node); // RA2: as above
	}
}

static inline void Planning_Unselect_Members(PlanningNodeClass * last) // RA2: no function of its own; unselects the members of the last node of a path that just closed into a loop (its expansion stands in Planning_Close_Loop)
{
	int count = last->PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		TechnoClass * member = last->PlanningMembers[i]->Owner; // RA2: as above
		if (member->IsSelected) { // RA2: as above
			member->Unselect(); // RA2: as above
		}
	}
}

void __fastcall Planning_Close_Loop(TechnoClass * techno, PlanningNodeClass * node) // RA2: 0x006164E0; makes the path of a techno loop back to a node
{
	int start = Planning_Token_Index(techno->Get_Planning_Token(), node); // RA2: as above
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	token->ClosedLoopNodeCount = token->PlanningNodes.Count() - start; // RA2: as above
	token->StepsToClosedLoop = start; // RA2: as above
	token->F98 = true; // RA2: as above
	if (!PlanningLoopClosed) { // RA2: as above
		if (techno->House->Is_Player_Control()) { // RA2: as above
			PlanningLoopClosed = true; // RA2: as above
			Planning_Unselect_Members(token->Last_Node()); // RA2: as above
		}
		Planning_Convert_Loop_Moves(techno); // RA2: as above
	}
	node->Update_Active(); // RA2: as above
}

void __fastcall Planning_Path_Append(TechnoClass * techno, PlanningNodeClass * node) // RA2: 0x006165A0; gives the techno the rest of the path of the node's first member from the node on
{
	TechnoClass * owner = node->Member_Owner(0); // RA2: as above
	PlanningTokenClass * initial = owner->Get_Planning_Token(); // RA2: as above
	int from = initial->Node_Index(node); // RA2: as above
	PlanningTokenClass * source = owner->Get_Planning_Token(); // RA2: as above
	source->Share_From(techno, from); // RA2: as above

	int start = -1; // RA2: as above
	int loop_count = -1; // RA2: as above
	PlanningNodeClass * update = NULL; // RA2: as above
	PlanningTokenClass * target = techno->Get_Planning_Token(); // RA2: as above
	if (source->Loop_Start_Positive() || source->Loop_Start_Zero()) { // RA2: as above
		int const source_loop_count = source->Get_Loop_Count(); // RA2: as above
		int step = source->Get_Loop_Start(); // RA2: as above
		if (source->Loop_Start_Positive() && from < step) { // RA2: as above
			PlanningNodeClass * loop = source->Node_At(step); // RA2: as above
			start = target->Node_Index(loop); // RA2: as above
			update = loop; // RA2: as above
		} else { // RA2: as above
			source->Share_Range(techno, step, from - step); // RA2: as above
			start = target->Node_Index(node); // RA2: as above
			update = node; // RA2: as above
		}
		loop_count = source_loop_count; // RA2: as above
	}
	target->ClosedLoopNodeCount = loop_count; // RA2: as above
	target->StepsToClosedLoop = start; // RA2: as above
	target->F98 = true; // RA2: as above
	if (update) { // RA2: as above
		update->Update_Active(); // RA2: as above
	}

	if (!PlanningCommandFlag && techno->House->Is_Player_Control()) { // RA2: as above
		PlanningCommandFlag = true; // RA2: as above
		PlanningNodeClass * last = target->Last_Node(); // RA2: as above
		if (!update) { // RA2: as above
			last->Select_All(); // RA2: as above
		} else { // RA2: as above
			last->Unselect_All(); // RA2: as above
		}
	}
}

void __fastcall Planning_Copy_Members(PlanningNodeClass * node, PlanningNodeClass * from, EventClass event) // RA2: 0x006167F0; adds every member of one node to another, each with a copy of the event aimed at its techno
{
	int count = from->PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		TechnoClass * techno = from->PlanningMembers[i]->Owner; // RA2: as above
		event.Data.Target.Whom = TargetClass(techno); // RA2: as above
		PlanningMemberClass * member = new PlanningMemberClass; // RA2: as above
		member->Owner = techno; // RA2: as above
		EventClass * copy = new EventClass; // RA2: as above
		*copy = event; // RA2: as above
		member->Packet = copy; // RA2: as above
		node->PlanningMembers.Add(member); // RA2: as above
		PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
		token->Add_Node(node); // RA2: as above
		node->Assign_Branch(member); // RA2: as above
	}
}

void __fastcall Planning_Remove_Node(PlanningNodeClass * node) // RA2: 0x00616960; takes a node out of the paths of its members
{
	if (node != NULL) { // RA2: as above
		TechnoClass * owner = node->PlanningMembers[0]->Owner; // RA2: as above
		PlanningNodeClass * next = node->Next_Node(owner); // RA2: as above
		for (int i = node->PlanningMembers.Count() - 1; i >= 0; i--) { // RA2: as above
			TechnoClass * member = node->PlanningMembers[i]->Owner; // RA2: as above
			PlanningTokenClass * token = member->Get_Planning_Token(); // RA2: as above
			token->Remove_Node_At(token->Node_Index(node)); // RA2: as above
		}
		if (next != NULL && next != node) { // RA2: as above
			next->Clear(); // RA2: as above
			int count = next->PlanningMembers.Count(); // RA2: as above
			for (int j = 0; j < count; j++) { // RA2: as above
				next->Assign_Branch(next->PlanningMembers[j]); // RA2: as above
			}
			next->Update_Active(); // RA2: as above
			next->Queued = false; // RA2: as above
		}
	}
}

#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00616a50 has no row); main: best 77.83, tried 5 times (register, inline, layout)
void __fastcall Planning_Convert_Loop_Moves(TechnoClass * techno) // RA2: 0x00616A50; in the path of the techno turns the move orders after the loop start into the other move mission (event mission 2 into 0x29)
{
	PlanningTokenClass * token = techno->Get_Planning_Token(); // RA2: as above
	int count = token->Node_Count(); // RA2: as above
	int start = token->StepsToClosedLoop; // RA2: as above
	PlanningNodeClass * first = token->Node_At(0); // RA2: as above
	int members = first->PlanningMembers.Count(); // RA2: as above
	for (int i = 0; i < members; i++) { // RA2: as above
		if (first->PlanningMembers[i]->Owner->CanAttackOnTheMove()) { // RA2: as above
			if (start < count) { // RA2: as above
				for (int index = start; index < count; index++) { // RA2: as above
					PlanningNodeClass * node = token->Node_At(index); // RA2: as above
					int total = node->PlanningMembers.Count(); // RA2: as above
					for (int k = 0; k < total; k++) { // RA2: as above
						EventClass * event = node->PlanningMembers[k]->Packet; // RA2: as above
						if (event->Data.MegaMission.Mission == 2) { // RA2: as above
							event->Data.MegaMission.Mission = 0x29; // RA2: as above
						}
					}
				}
			}
			return; // RA2: as above
		}
	}
}
#endif

static inline int Planning_Object_Kind(TechnoClass * techno) // RA2: no function of its own; 0 for an aircraft, 1 for a building, 2 for the rest
{
	if (techno != NULL) { // RA2: as above
		if (techno->Fetch_RTTI() == RTTI_AIRCRAFT) { // RA2: as above
			return 0; // RA2: as above
		}
		if (techno->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
			return 1; // RA2: as above
		}
	}
	return 2; // RA2: as above
}

int __cdecl Planning_Selection_Kind(void) // RA2: 0x00616B40; what the selected technos have in common: -1 for none that cannot use a waypoint, else the kind of the first of them
{
	int kind = -1; // RA2: as above
	int count = CurrentObject.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		ObjectClass * object = CurrentObject[i]; // RA2: as above
		bool techno; // RA2: as above
		if (object != NULL) { // RA2: as above
			techno = object->AbstractFlags & ABSTRACT_TECHNO; // RA2: as above
		}
		TechnoClass * candidate = (object != NULL && techno) ? (TechnoClass *)object : NULL; // RA2: as above
		if (candidate == NULL || !candidate->CanUseWaypoint()) { // RA2: as above
			if (candidate != NULL && candidate->Fetch_RTTI() == RTTI_AIRCRAFT) { // RA2: as above
				kind = 0; // RA2: as above
			} else if (candidate != NULL && candidate->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
				kind = 1; // RA2: as above
			} else { // RA2: as above
				kind = 2; // RA2: as above
			}
			break; // RA2: as above
		}
	}
	return kind; // RA2: as above
}

bool __fastcall Planning_Same_Last_Node(PlanningTokenClass * first, PlanningTokenClass * second) // RA2: 0x00616BD0; whether both paths (or none) end at nodes holding the owners of both
{
	if (first == NULL && second == NULL) { // RA2: as above
		return true; // RA2: as above
	}
	int first_count = 0; // RA2: as above
	if (first != NULL) { // RA2: as above
		first_count = first->Node_Count(); // RA2: as above
	}
	int second_count = 0; // RA2: as above
	if (second != NULL) { // RA2: as above
		second_count = second->Node_Count(); // RA2: as above
	}
	if (first_count == 0 && second_count == 0) { // RA2: as above
		return true; // RA2: as above
	}
	if (first_count == 0 || second_count == 0) { // RA2: as above
		return false; // RA2: as above
	}
	PlanningNodeClass * first_last = first->Node_At(first_count - 1); // RA2: as above
	PlanningNodeClass * second_last = second->Node_At(second_count - 1); // RA2: as above
	TechnoClass * first_owner = first->OwnerUnit; // RA2: as above
	TechnoClass * second_owner = second->OwnerUnit; // RA2: as above
	if (!first_last->Has_Member(second_owner)) { // RA2: as above
		return false; // RA2: as above
	}
	if (!second_last->Has_Member(first_owner)) { // RA2: as above
		return false; // RA2: as above
	}
	return true; // RA2: as above
}
void __fastcall Planning_Recount(int house) // RA2: 0x00616D00; raises the count of a house
{
	if (Planning_Valid_House(house)) { // RA2: as above
		PlanningUnitCounts[house]++; // RA2: as above
	}
}

int __fastcall Planning_Count(int house) // RA2: 0x00616D50 (also expanded where it is used); the number of units of a house with a planned path, 0 for a number that is no house
{
	bool valid = Planning_Valid_House(house); // RA2: as above
	int count = 0; // RA2: as above
	if (valid) { // RA2: as above
		count = PlanningUnitCounts[house]; // RA2: as above
	}
	return count; // RA2: as above
}

void __fastcall Planning_Refuse_Event(EventClass * event) // RA2: 0x00616D70; says why planning mode refuses an event of this type, then scolds
{
	if (event->Type == 9) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoDeploy"); // RA2: as above
	} else if (event->Type == 6) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoStop"); // RA2: as above
	} else if (event->Type == 7) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoScatter"); // RA2: as above
	}
	VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000); // RA2: as above
}

void __fastcall Planning_Refuse_Area_Guard(void) // RA2: 0x00616E70; refuses the guard command while planning mode is on
{
	Planning_Refuse("MSG:PlanningModeNoGuardArea"); // RA2: as above
}

void __fastcall Planning_Show_Help(bool sound, bool enable) // RA2: 0x00616EC0; shows the help text of planning mode (when it is turned on or off), the number of times per kind it allows
{
	int kind; // RA2: as above
	if (enable) { // RA2: as above
		kind = !sound; // RA2: as above
	} else { // RA2: as above
		kind = 2; // RA2: as above
	}
	int limit; // RA2: as above
	if (sound) { // RA2: as above
		limit = PlanningHelpCounts[kind] + 1; // RA2: as above
	} else { // RA2: as above
		limit = 1; // RA2: as above
	}
	if (PlanningHelpCounts[kind] < limit) { // RA2: as above
		if (enable) { // RA2: as above
			if (sound) { // RA2: as above
				Planning_Message("MSG:PlanningModeIntro1Button"); // RA2: as above
			} else { // RA2: as above
				Planning_Message("MSG:PlanningModeIntro1Key"); // RA2: as above
			}
		} else { // RA2: as above
			Planning_Message("MSG:PlanningModeIntro3"); // RA2: as above
		}
		PlanningHelpCounts[kind]++; // RA2: as above
	}
}

void __fastcall Planning_Help(int kind) // RA2: 0x00616F50; shows the help text of a kind (loop, delete) once
{
	if (PlanningHelpCounts[kind] < 1) { // RA2: as above
		if (kind == 3) { // RA2: as above
			Planning_Message("MSG:PlanningModeLoopHelp"); // RA2: as above
		} else if (kind == 4) { // RA2: as above
			Planning_Message("MSG:PlanningModeDeleteHelp"); // RA2: as above
		}
		PlanningHelpCounts[kind]++; // RA2: as above
	}
}

void __fastcall Planning_Message(char const * label) // RA2: 0x00616FB0 (expanded in Planning_Refuse); shows a text of the string table
{
	wchar_t const * text = StringTable::LoadString(label, NULL, "D:\\Ra2\\PlanMgr.cpp", 3231); // RA2: as above
	Show_Player_Message(text, 0x1E0); // RA2: as above
}

void __fastcall Planning_Refuse(char const * label) // RA2: 0x00616FD0; shows a refusal once per frame and scolds
{
	if (!PlanningRefusalShown) { // RA2: as above
		PlanningRefusalShown = true; // RA2: as above
		Planning_Message(label); // RA2: as above
		VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000); // RA2: as above
	}
}

void __fastcall Planning_Refuse_Order(EventClass const * event) // RA2: 0x00617020; says why planning mode refuses an order of this type
{
	if (event->Type == 9) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoDeploy"); // RA2: as above
	} else if (event->Type == 6) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoStop"); // RA2: as above
	} else if (event->Type == 7) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoScatter"); // RA2: as above
	}
}

void __fastcall Planning_Refuse_Selection(int kind) // RA2: 0x00617100; says why planning mode refuses a selection of this kind
{
	if (kind == 0) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoAircraft"); // RA2: as above
	} else if (kind == 1) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoBuildings"); // RA2: as above
	} else if (kind == 2) { // RA2: as above
		Planning_Refuse("MSG:PlanningModeNoUnknowns"); // RA2: as above
	}
}

int __fastcall Planning_Event_Result(PlanningTokenClass * token, EventClass const * event) // RA2: 0x006171E0; what planning mode shows for an order: 0 for one that ends the path, 2 at the last free node, -1 for none
{
	int mission = event->CurrentMission(); // RA2: as above
	TargetClass const & target = event->Target(); // RA2: as above
	if (Planning_Terminal_Mission(mission)) { // RA2: as above
		return 0; // RA2: as above
	}
	if (Planning_Cell_Mission(mission, target)) { // RA2: as above
		return 0; // RA2: as above
	}
	if (token->Node_Count() == 15) { // RA2: as above
		return 2; // RA2: as above
	}
	return -1; // RA2: as above
}

void __fastcall Planning_Set_Pending_Refusal(int value) // RA2: 0x00617240; sets the refusal to show at the end of the frame
{
	PlanningPendingRefusal = value; // RA2: as above
}

void __fastcall Planning_Refuse_Command(int kind) // RA2: 0x00617250; drops the selection and says why a command is refused
{
	TacticalMap->Unselect_All(); // RA2: as above
	if (kind == 0) { // RA2: as above
		Planning_Refuse("MSG:PostTerminatingCommand"); // RA2: as above
	} else if (kind == 1) { // RA2: as above
		Planning_Refuse("MSG:PostContinualCommand"); // RA2: as above
	} else if (kind == 2) { // RA2: as above
		Planning_Refuse("MSG:NodeMaximum"); // RA2: as above
	}
}

void __cdecl Planning_Connect_Selection(void); // RA2: 0x00617AF0
void __fastcall Planning_Set_Picked_Node(PlanningNodeClass * node); // RA2: 0x00617AD0
PlanningNodeClass * __fastcall Planning_Get_Picked_Node(void); // RA2: 0x00617AE0
void __fastcall Planning_Set_Selected_Node(PlanningNodeClass * node); // RA2: 0x00617E20
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

bool __stdcall Planning_Left_Press(int x, int y); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00617340 has no row); main: best 38.59, tried 7 times (register, layout, frame)
bool __stdcall Planning_Left_Press(int x, int y) // RA2: 0x00617340; planning mode's left button press on the tactical view: takes the node under the mouse as the picked one (the path starts there), else refuses a selection or a node planning mode cannot use; true when it took the input
{
	if (!PlanningNodeClass::PlanningModeActive) { // RA2: as above
		return false; // RA2: as above
	}
	PlanningNodeClass * previous = Planning_Get_Picked_Node(); // RA2: as above
	PlanningNodeClass * node = Planning_Node_At(Point2D(x, y)); // RA2: as above
	if (node != NULL && node != previous) { // RA2: as above
		Map.Set_Default_Mouse(MOUSE_NORMAL, true); // RA2: as above
		Planning_Set_Picked_Node(node); // RA2: as above
	} else { // RA2: as above
		PlanningNodeClass * picked = Planning_Pick_Node(Point2D(x, y)); // RA2: as above
		if (picked != NULL) { // RA2: as above
			if (Planning_Selection_Node_State(picked) != -1) { // RA2: as above
				Planning_Refuse("MSG:PlanningModeInvalidNodeX"); // RA2: as above
				return true; // RA2: as above
			}
		} else { // RA2: as above
			int kind = Planning_Selection_Kind(); // RA2: as above
			if (kind != -1) { // RA2: as above
				Planning_Refuse_Selection(kind); // RA2: as above
				return true; // RA2: as above
			}
		}
	}
	return previous != NULL || node != NULL; // RA2: as above
}
#endif

bool __stdcall Planning_Left_Release(Point2D point) // RA2: 0x00617680; planning mode's left button release: connects the selected objects to the node when it is the picked one, else drops the picked node; true when it took the input
{
	if (!Planning_Is_Active()) return false; // RA2: as above

	PlanningNodeClass * selected = Planning_Get_Picked_Node(); // RA2: as above
	PlanningNodeClass * hit = Hover_Node_At(point); // RA2: as above

	if (hit && hit == selected) { // RA2: as above
		Map.Set_Default_Mouse(MOUSE_NORMAL, true); // RA2: as above
		Planning_Connect_Selection(); // RA2: as above
	} else {
		PlanningNodeClass * node = Find_Node_At(PlanningNodeClass::Unknown1, point); // RA2: as above
		if (node && Planning_Selection_Node_State(node) != -1) return true; // RA2: as above
		Planning_Set_Picked_Node(NULL); // RA2: as above
	}
	return selected || hit; // RA2: as above
}

void __stdcall Planning_Right_Press(int x, int y) // RA2: 0x00617860; planning mode's right button press: starts a drag, remembering the point when nothing is selected
{
	PlanningDragging = true; // RA2: as above
	PlanningMouseDown = Point2D(-1, -1); // RA2: as above
	if (PlanningNodeClass::PlanningModeActive && CurrentObject.Count() <= 0) { // RA2: as above
		PlanningMouseDown = Point2D(x, y); // RA2: as above
	}
}

void __stdcall Planning_Right_Release(int x, int y) // RA2: 0x006178A0; planning mode's right button release: a click (not a drag) with nothing selected lets go of the selected node
{
	PlanningDragging = false; // RA2: as above
	if (PlanningNodeClass::PlanningModeActive && CurrentObject.Count() <= 0 && PlanningSelectedNode != NULL && abs(x - PlanningMouseDown.X) <= 3 && abs(y - PlanningMouseDown.Y) <= 3) { // RA2: as above
		PlanningSelectedNode = NULL; // RA2: as above
	}
}

bool __stdcall Planning_Mouse_AI(int x, int y); // RA2: the definition is not exact yet
bool __stdcall Planning_Mouse_AI(int x, int y) // RA2: 0x00617900; planning mode's mouse cursor: the plain one over the picked node or when none is picked, else the small one; true when planning mode set it
{
	if (!PlanningNodeClass::PlanningModeActive) { // RA2: as above
		return false; // RA2: as above
	}
	if (PlanningDragging) { // RA2: as above
		return false; // RA2: as above
	}
	PlanningNodeClass * remembered = PlanningPickedNode; // RA2: as above
	PlanningNodeClass * node = Planning_Node_At(Point2D(x, y)); // RA2: as above
	if (remembered == NULL && node == NULL) { // RA2: as above
		return false; // RA2: as above
	}
	if (remembered == NULL || node == remembered) { // RA2: as above
		Map.Set_Default_Mouse(MOUSE_NORMAL, true); // RA2: as above
	} else { // RA2: as above
		Map.Set_Default_Mouse(MOUSE_NORMAL, false); // RA2: as above
	}
	return true; // RA2: as above
}

PlanningNodeClass * __fastcall Planning_Selected_Node(void) // RA2: 0x00617A00; the planned node the player has selected, if any
{
	return PlanningSelectedNode; // RA2: as above
}

int __fastcall Planning_Selection_Node_State(PlanningNodeClass * node) // RA2: 0x00617A10; how the selected objects relate to a node: 1 for an object whose path has closed a loop, 5 for one whose path lacks the node, 2 and 3 for the last and the second last node of a path, -1 for none
{
	int count = CurrentObject.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		ObjectClass * object = CurrentObject[i]; // RA2: as above
		bool techno; // RA2: as above
		if (object != NULL) { // RA2: as above
			techno = object->AbstractFlags & ABSTRACT_TECHNO; // RA2: as above
		}
		if (object != NULL && techno) { // RA2: as above
			PlanningTokenClass * token = ((TechnoClass *)object)->Get_Planning_Token(); // RA2: as above
			if (token != NULL) { // RA2: as above
				if (Planning_Token_Looped(token)) { // RA2: as above
					return 1; // RA2: as above
				}
				int index = token->Node_Index(node); // RA2: as above
				if (index == -1) { // RA2: as above
					return 5; // RA2: as above
				}
				if (index == token->PlanningNodes.Count() - 1) { // RA2: as above
					return 2; // RA2: as above
				}
				if (index == token->PlanningNodes.Count() - 2) { // RA2: as above
					return 3; // RA2: as above
				}
			}
		}
	}
	return -1; // RA2: as above
}

void __fastcall Planning_Set_Picked_Node(PlanningNodeClass * node) // RA2: 0x00617AD0; sets the node a path is being drawn from
{
	PlanningPickedNode = node; // RA2: as above
}

PlanningNodeClass * __fastcall Planning_Get_Picked_Node(void) // RA2: 0x00617AE0; the node a path is being drawn from
{
	return PlanningPickedNode; // RA2: as above
}

void __cdecl Planning_Connect_Selection(void) // RA2: 0x00617AF0 (called Planning_Connect_Selection by us); lets go of the picked node and asks all players to connect each selected object (and the owners of the last node of its path) to it, or with nothing selected keeps the node as the selected one
{
	PlanningNodeClass * selected = Planning_Get_Picked_Node(); // RA2: as above
	Planning_Set_Picked_Node(NULL); // RA2: as above
	int count = CurrentObject.Count(); // RA2: as above
	if (count > 0) { // RA2: as above
		DynamicVectorClass<TechnoClass *> units; // RA2: as above
		for (int i = 0; i < count; i++) { // RA2: as above
			ObjectClass * object = CurrentObject[i]; // RA2: as above
			if (Planning_Is_Techno(object)) {
				TechnoClass * unit = (TechnoClass *)object; // RA2: as above
				PlanningTokenClass * token = unit->Get_Planning_Token(); // RA2: as above
				if (token != NULL) { // RA2: as above
					PlanningNodeClass * last = token->Last_Node(); // RA2: as above
					if (last == NULL) { // RA2: as above
						units.Add(unit); // RA2: as above
					} else { // RA2: as above
						int members = last->PlanningMembers.Count(); // RA2: as above
						for (int j = 0; j < members; j++) { // RA2: as above
							TechnoClass * owner = last->Member_Owner(j); // RA2: as above
							if (units.ID(owner) == -1) { // RA2: as above
								units.Add(owner); // RA2: as above
							}
						}
					}
				} else { // RA2: as above
					units.Add(unit); // RA2: as above
				}
			}
		}
		int total = units.Count(); // RA2: as above
		for (int k = 0; k < total; k++) { // RA2: as above
			Planning_Send_Pair(units[k], selected); // RA2: as above
		}
		if (total > 0) { // RA2: as above
			Planning_Help(3); // RA2: as above
		}
	} else { // RA2: as above
		Planning_Set_Selected_Node(selected); // RA2: as above
		Planning_Help(4); // RA2: as above
	}
}

AbstractClass * EventClass::First_Target_Object(void) const // RA2: 0x00617DF0
{
	AbstractClass * object = NULL; // RA2: as above
	if (Data.MegaMission.Target.Is_Valid()) { // RA2: as above
		object = Data.MegaMission.Target.As_Abstract(); // RA2: as above
	} else if (Data.MegaMission.Follow.Is_Valid()) { // RA2: as above
		object = Data.MegaMission.Follow.As_Abstract(); // RA2: as above
	} else if (Data.MegaMission.Destination.Is_Valid()) { // RA2: as above
		object = Data.MegaMission.Destination.As_Abstract(); // RA2: as above
	}
	return object; // RA2: as above
}

void __fastcall Planning_Set_Selected_Node(PlanningNodeClass * node) // RA2: 0x00617E20; sets the node whose members are shown selected
{
	PlanningSelectedNode = node; // RA2: as above
}

void __cdecl Planning_Clear_Selected_Node(void) // RA2: 0x00617E30; forgets the selected node
{
	PlanningSelectedNode = NULL; // RA2: as above
}

void __cdecl Planning_Redraw(void) // RA2: 0x00617E40; rebuilds the list of the drawn nodes: draws the nodes and the paths of the technos that are allies of the player
{
	Planning_Clear_Drawn(); // RA2: as above
	int count = PlanningNodeClass::Unknown1.Count(); // RA2: as above
	int i; // RA2: as above
	for (i = 0; i < count; i++) { // RA2: as above
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i]; // RA2: as above
		if (node->Lead_House()->Is_Ally(PlayerPtr)) { // RA2: as above
			Planning_Draw_Node(node); // RA2: as above
		}
	}
	count = Technos.Count(); // RA2: as above
	for (i = 0; i < count; i++) { // RA2: as above
		TechnoClass * techno = Technos[i]; // RA2: as above
		HouseClass * house = techno->House; // RA2: as above
		if (house->Is_Ally(PlayerPtr)) { // RA2: as above
			Planning_Draw_Techno(techno); // RA2: as above
		}
	}
}

void __cdecl Planning_Draw_All(void) // RA2: 0x00617EF0; draws the filled version of the nodes and the paths of the technos that are allies of the player
{
	int count = PlanningNodeClass::Unknown1.Count(); // RA2: as above
	int i; // RA2: as above
	for (i = 0; i < count; i++) { // RA2: as above
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i]; // RA2: as above
		if (node->Lead_House()->Is_Ally(PlayerPtr)) { // RA2: as above
			Planning_Draw_Filled_Node(node); // RA2: as above
		}
	}
	for (i = 0; i < count; i++) { // RA2: as above
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i]; // RA2: as above
		if (node->Lead_House()->Is_Ally(PlayerPtr)) { // RA2: as above
			node->Fill_Lead_Box(node->Node_Color()); // RA2: as above
		}
	}
	int technos = Technos.Count(); // RA2: as above
	for (i = 0; i < technos; i++) { // RA2: as above
		TechnoClass * techno = Technos[i]; // RA2: as above
		if (techno->House->Is_Ally(PlayerPtr)) { // RA2: as above
			Planning_Draw_Filled_Techno(techno); // RA2: as above
		}
	}
}

void __cdecl Planning_Draw_Remembered(void) // RA2: 0x00618090; outlines the node under the mouse and the selected node in white
{
	int color = Planning_Pixel(&WhiteColor); // RA2: as above
	if (PlanningHoverNode != NULL) { // RA2: as above
		PlanningHoverNode->Frame_Lead_Box(color); // RA2: as above
	}
	if (PlanningSelectedNode != NULL) { // RA2: as above
		PlanningSelectedNode->Frame_Lead_Box_Outer(color); // RA2: as above
	}
}


// RA2: the two waypoint renderers use these predicates from the ra2-decomp filled-waypoint text.
static inline bool Has_Planned_Nodes(PlanningTokenClass * token)
{
	int count; // RA2: as above
	if (!token) count = 0; // RA2: as above
	else count = token->Node_Count(); // RA2: as above
	return count > 0; // RA2: as above
}

static inline bool Should_Draw_Active_Action(TechnoClass * techno, bool has_nodes) // RA2: no function of its own; whether the techno has an order of its own to draw: it is not guarding or idle, and has planned nodes or is selected
{
	return techno->Get_Mission() != MISSION_GUARD && !Planning_Idle_Mover(techno) // RA2: as above
		&& (has_nodes || techno->IsSelected);
}

#ifdef RA2_NON_MATCHING // not exact: 1665 bytes differ (both waypoint renderers use the same node and active-order predicates; all 50 direct calls agree and every instruction is the image's but for its stack offsets: the frame is 0x178 against 0x174, the attack-move flag not beside has_nodes)
void __fastcall Planning_Draw_Techno(TechnoClass * techno)
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	Coord current_coord = techno->As_Coord();
	Point2D start = Planning_Coord_Pixel(current_coord);
	int color = Planning_Techno_Color(techno);
	bool has_nodes = Has_Planned_Nodes(token);
	bool current_order = Should_Draw_Active_Action(techno, has_nodes);
	int thickness = 4;
	if (token && token->Owner_Is_Mission()) {
		EventClass event = token->Popped;
		Coord coord = Planning_Event_Or_Object_Coord(event, techno);
		Point2D end = Planning_Coord_Pixel(coord);
		Planning_Draw_Dot(start, color);
		Planning_Draw_Thick_Line(start, end, 3, color);
		if (event.Get_Event_Frame() != -1) Planning_Draw_Midpoint(start, end, color);
		Planning_Draw_Dot(end, color);
		return;
	}
	if (current_order) {
		bool alternate = techno->HaveMegaMission();
		bool secondary = techno->HaveAttackMoveTarget();
		Coord coord = techno->GetAttackCoordinates();
		Point2D end = Planning_Coord_Pixel(coord);
		if (techno->IsInLimbo) {
			Planning_Draw_Dot(end, color);
			return;
		}
		Planning_Draw_Dot(start, color);
		Planning_Draw_Thick_Line(start, end, 3, color);
		int action = -1;
		if (!alternate || secondary) {
			if (techno->Get_Mission() == MISSION_GUARD_AREA) action = Cursor_Start_Frame_76();
		} else if (techno->GetMegaMission() == MISSION_ATTACK_MOVE) action = Cursor_Start_Frame_77();
		if (action != -1) Planning_Draw_Midpoint(start, end, color);
		Planning_Draw_Dot(end, color);
		if (secondary) {
			Coord next_coord = techno->GetAttackMoveCoords();
			Point2D next = Planning_Coord_Pixel(next_coord);
			Planning_Draw_Thick_Line(end, next, 3, color);
			if (techno->GetMegaMission() == MISSION_ATTACK_MOVE && Cursor_Start_Frame_77() != -1)
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
		PlanningNodeClass * node = token->Node_At(0);
		Point2D end = node->Lead_Pixel();
		PlanningBranchClass * branch = node->Branch_Of(techno);
		if (branch) end = Planning_Event_Pixel(&branch->Event, techno);
		Planning_Draw_Dot(start, color);
		Planning_Draw_Thick_Line(start, end, thickness, color);
		if (branch) Planning_Draw_Dot(end, color);
		EventClass event = node->Get_Event_For(techno);
		if (event.Get_Event_Frame() != -1)
			Planning_Draw_Midpoint(start, end, color);
	}
}
#endif

bool __fastcall Planning_Idle_Mover(TechnoClass * techno) // RA2: 0x00618BB0; whether a techno is on its move mission with nothing to move to or to attack
{
	if (techno->Get_Mission() == MISSION_GUARD_AREA) { // RA2: as above
		if (techno->Is_Foot()) { // RA2: as above
			FootClass * foot = (FootClass *)techno; // RA2: as above
			if (foot->NavCom != 0 || foot->TarCom != 0) { // RA2: as above
				return false; // RA2: as above
			}
		}
		return true; // RA2: as above
	}
	return false; // RA2: as above
}

// RA2: the filled waypoints and their helpers are ra2-decomp's twin text
int __fastcall Planning_Mega_Frame(TechnoClass * techno);
void __fastcall Planning_Draw_Line(Point2D start, Point2D end, int offset, int color, bool dashed);

static inline int RGB_Pixel(RGBClass const & color)
{
	return DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue()); // RA2: as above
}

static inline int Waypoint_Fill_Color(TechnoClass * techno)
{
	int normal = RGB_Pixel(BlackColor); // RA2: as above
	int result; // RA2: as above
	if (techno) { // RA2: as above
		int highlight = Planning_Pixel(&WhiteColor); // RA2: as above
		if (PlanningSelectedNode) { // RA2: as above
			if (PlanningSelectedNode->Find_Member(techno) != -1) result = highlight; // RA2: as above
			else result = normal; // RA2: as above
		} else if (techno->IsSelected) { // RA2: as above
			result = highlight; // RA2: as above
		} else {
			result = normal; // RA2: as above
		}
	} else {
		result = normal; // RA2: as above
	}
	return result; // RA2: as above
}

static inline int Active_Waypoint_Frame(TechnoClass * techno, bool mega_mission, bool attack_move)
{
	int frame; // RA2: as above
	if (mega_mission && !attack_move) frame = Planning_Mega_Frame(techno); // RA2: as above
	else frame = techno->Get_Mission() == MISSION_GUARD_AREA ? Cursor_Start_Frame_76() : -1; // RA2: as above
	return frame; // RA2: as above
}

static inline int Planned_Waypoint_Frame(EventClass order)
{
	return order.Get_Event_Frame(); // RA2: as above
}

void __fastcall Planning_Draw_Filled_Techno(TechnoClass * techno)
{
	PlanningTokenClass * token = techno->Get_Planning_Token();
	Coord origin = techno->As_Coord(); // RA2: as above
	Point2D center = Planning_Coord_Pixel(origin); // RA2: as above
	Point2D source; // RA2: as above
	source = center; // RA2: assigned whole, as the family's other points and rectangles are (ra2-decomp's twin copies the two fields)
	int color = Unit_Path_Tint(techno, WAYPOINT_NORMAL); // RA2: as above
	int bright = Unit_Path_Tint(techno, WAYPOINT_BRIGHT); // RA2: as above
	bool has_nodes = Has_Planned_Nodes(token); // RA2: as above
	bool draw_action = Should_Draw_Active_Action(techno, has_nodes); // RA2: as above
	int width = 4; // RA2: as above
	int fill = Waypoint_Fill_Color(techno); // RA2: as above
	if (token && token->Owner_Is_Mission()) { // RA2: as above
		EventClass packet(token->Popped); // RA2: as above
		Coord coord = Planning_Event_Or_Object_Coord(packet, techno); // RA2: as above
		Point2D target = Planning_Coord_Pixel(coord); // RA2: as above
		Planning_Fill_Dot(center, color); // RA2: as above
		Planning_Draw_Dashed_Thick_Line(center, target, 3, color); // RA2: as above
		int frame = packet.Get_Event_Frame(); // RA2: as above
		if (frame != -1) Planning_Draw_Midpoint_Shape(center, target, color, fill, frame); // RA2: as above
		Planning_Fill_Dot(target, color); // RA2: as above
		return;
	}

	if (draw_action) { // RA2: as above
		bool mega_mission = techno->HaveMegaMission(); // RA2: as above
		bool attack_move = techno->HaveAttackMoveTarget(); // RA2: as above
		Coord coord = techno->GetAttackCoordinates();
		Point2D action = Planning_Coord_Pixel(coord); // RA2: as above
		if (techno->IsInLimbo) {
			Planning_Fill_Dot(action, color); // RA2: as above
			return;
		}
		Planning_Fill_Dot(center, color); // RA2: as above
		Planning_Draw_Dashed_Thick_Line(center, action, 3, color); // RA2: as above
		int frame = Active_Waypoint_Frame(techno, mega_mission, attack_move); // RA2: as above
		if (frame != -1) Planning_Draw_Midpoint_Shape(center, action, color, fill, frame); // RA2: as above
		Planning_Fill_Dot(action, color); // RA2: as above
		if (attack_move) { // RA2: as above
			Coord attack_coord = techno->GetAttackMoveCoords(); // RA2: as above
			Point2D dest = Planning_Coord_Pixel(attack_coord); // RA2: as above
			Planning_Draw_Line(action, dest, 0, color, true); // RA2: as above
			frame = techno->GetMegaMission() == MISSION_ATTACK_MOVE ? Cursor_Start_Frame_77() : -1; // RA2: as above
			if (frame != -1) Planning_Draw_Midpoint_Shape(action, dest, color, fill, frame); // RA2: as above
			Planning_Fill_Dot(dest, color); // RA2: as above
			return;
		}
		if (!has_nodes) return;
		source = action; // RA2: as above
		width = 3; // RA2: as above
	} else if (!has_nodes) {
		return;
	}

	PlanningNodeClass * node = token->Node_At(0); // RA2: as above
	Point2D target = node->Lead_Pixel(); // RA2: as above
	PlanningBranchClass * branch = node->Branch_Of(techno); // RA2: as above
	if (branch) target = Planning_Event_Pixel(&branch->Event, techno); // RA2: as above
	int path_color = token->Reached(0) ? bright : color; // RA2: as above
	Planning_Fill_Dot(source, path_color); // RA2: as above
	Planning_Draw_Dashed_Thick_Line(source, target, width, path_color); // RA2: as above
	if (branch) Planning_Fill_Dot(target, path_color); // RA2: as above
	int frame = Planned_Waypoint_Frame(node->Get_Event_For(techno)); // RA2: as above
	if (frame != -1) Planning_Draw_Midpoint_Shape(source, target, path_color, fill, frame); // RA2: as above
}

void __fastcall Planning_Draw_Node(PlanningNodeClass * node) // RA2: 0x00619420; draws the lines from the nodes before this one (and from its own loop) to it as solid lines, and its box
{
	Point2D pixel = node->Lead_Pixel(); // RA2: as above
	int frame = -1; // RA2: as above
	if (node->PlanningMembers.Count() > 0) { // RA2: as above
		EventClass event = node->Get_Event(0); // RA2: as above
		frame = event.Get_Event_Frame(); // RA2: as above
	}
	int color = node->Lead_Color(); // RA2: as above
	int count = node->PlanningBranches.Count(); // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		Coord coord = node->Branch_Coord(index); // RA2: as above
		Point2D start = Planning_Coord_Pixel(coord); // RA2: as above
		if (coord == Coord(-1, -1, -1)) { // RA2: as above
			EventClass * lead = &node->PlanningBranches[index]->Event; // RA2: as above
			start = Planning_Event_Pixel(lead, node->Owner_At_Branch(index)); // RA2: as above
		}
		Planning_Draw_Thick_Line(start, pixel, 4, color); // RA2: as above
		if (frame != -1) { // RA2: as above
			Planning_Draw_Midpoint(start, pixel, color); // RA2: as above
		}
	}
	if (node->Is_Loop_Alone()) { // RA2: as above
		Coord coord = node->Loop_Coord(); // RA2: as above
		Point2D start = Planning_Coord_Pixel(coord); // RA2: as above
		if (coord == Coord(-1, -1, -1)) { // RA2: as above
			EventClass * lead = &node->LoopBranch.Event; // RA2: as above
			start = Planning_Event_Pixel(lead, node->Active_Owner()); // RA2: as above
		}
		Planning_Draw_Thick_Line(start, pixel, 4, color); // RA2: as above
		if (frame != -1) { // RA2: as above
			Planning_Draw_Midpoint(start, pixel, color); // RA2: as above
		}
	}
	node->Draw_Lead_Box(color); // RA2: as above
}

Coord __fastcall Planning_Event_Or_Object_Coord(EventClass const & event, AbstractClass * object) // RA2: 0x00619810; where the order of an event points, or where the object is when it points nowhere
{
	Coord coord = Planning_Event_Coord(event); // RA2: as above
	if (coord == Coord(-1, -1, -1) && object != NULL) { // RA2: as above
		coord = object->As_Coord(); // RA2: as above
	}
	return coord; // RA2: as above
}

void __fastcall Planning_Draw_Filled_Node(PlanningNodeClass * node) // RA2: 0x00619870; draws the lines from the nodes before this one (and from its own loop) to it as dashed lines, and fills its box
{
	Point2D pixel = node->Lead_Pixel(); // RA2: as above
	int frame = -1; // RA2: as above
	if (node->PlanningMembers.Count() > 0) { // RA2: as above
		EventClass event = node->Get_Event(0); // RA2: as above
		frame = event.Get_Event_Frame(); // RA2: as above
	}
	int color = node->Lead_Color(); // RA2: as above
	int count = node->PlanningBranches.Count(); // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		int branch_color = node->Branch_Color(index); // RA2: as above
		Coord coord = node->Branch_Coord(index); // RA2: as above
		Point2D start = Planning_Coord_Pixel(coord); // RA2: as above
		if (coord == Coord(-1, -1, -1)) { // RA2: as above
			PlanningBranchClass * branch = node->PlanningBranches[index];
			start = Planning_Event_Pixel(&branch->Event, node->Owner_At_Branch(index)); // RA2: as above
		}
		Planning_Draw_Dashed_Thick_Line(start, pixel, 4, branch_color); // RA2: as above
		if (frame != -1) { // RA2: as above
			Planning_Draw_Midpoint_Shape(start, pixel, branch_color, color, frame); // RA2: as above
		}
	}
	if (node->Is_Loop_Alone()) { // RA2: as above
		int loop_color = node->Loop_Color(); // RA2: as above
		Coord coord = node->Loop_Coord(); // RA2: as above
		Point2D start = Planning_Coord_Pixel(coord); // RA2: as above
		if (coord == Coord(-1, -1, -1)) { // RA2: as above
			TechnoClass * owner = node->Active_Owner();
			start = Planning_Event_Pixel(&node->LoopBranch.Event, owner); // RA2: as above
		}
		Planning_Draw_Dashed_Thick_Line(start, pixel, 4, loop_color); // RA2: as above
		if (frame != -1) { // RA2: as above
			Planning_Draw_Midpoint_Shape(start, pixel, loop_color, color, frame); // RA2: as above
		}
	}
	node->Fill_Lead_Box(node->Node_Color()); // RA2: as above
}


void __fastcall Planning_Fill_Node(PlanningNodeClass * node) // RA2: 0x00619BD0; fills the box of a node in its color
{
	node->Fill_Lead_Box(node->Node_Color()); // RA2: as above
}

int __fastcall Planning_Dim_Color(TechnoClass * techno) // RA2: 0x00619CA0; the pixel of the house color of a techno brought 80/256 toward black
{
	RGBClass color = Planning_House_Color(techno); // RA2: as above
	color.Adjust(0x50, BlackColor); // RA2: as above
	return ((color.Get_Green() >> DSurface::GreenLeft) << DSurface::GreenRight) | ((color.Get_Red() >> DSurface::RedLeft) << DSurface::RedRight) | ((color.Get_Blue() >> DSurface::BlueLeft) << DSurface::BlueRight); // RA2: as above
}

int __fastcall Planning_Bright_Color(TechnoClass * techno) // RA2: 0x00619D50; the pixel of the house color of a techno brought 100/256 toward white
{
	RGBClass color = Planning_House_Color(techno); // RA2: as above
	color.Adjust(100, WhiteColor); // RA2: as above
	return ((color.Get_Green() >> DSurface::GreenLeft) << DSurface::GreenRight) | ((color.Get_Red() >> DSurface::RedLeft) << DSurface::RedRight) | ((color.Get_Blue() >> DSurface::BlueLeft) << DSurface::BlueRight); // RA2: as above
}

int __fastcall Planning_Techno_Color(TechnoClass * techno) // RA2: 0x00619E80; black, or white for a techno that is selected (or a member of the selected node)
{
	int plain = Planning_Pixel(&BlackColor); // RA2: as above
	if (techno == NULL) { // RA2: as above
		return plain; // RA2: as above
	}
	int lighter = Planning_Pixel(&WhiteColor); // RA2: as above
	if (PlanningSelectedNode != NULL) { // RA2: as above
		if (PlanningSelectedNode->Find_Member(techno) != -1) { // RA2: as above
			return lighter; // RA2: as above
		}
		return plain; // RA2: as above
	}
	if (techno->IsSelected) { // RA2: as above
		return lighter; // RA2: as above
	}
	return plain; // RA2: as above
}

Point2D __fastcall Planning_Event_Pixel(EventClass const * event, AbstractClass * object) // RA2: 0x00619FA0; the pixel an order points at, or where the object is when it points nowhere; (-1, -1) for no order
{
	Point2D pixel(-1, -1); // RA2: as above
	if (event != NULL) { // RA2: as above
		EventClass copy = *event; // RA2: as above
		Coord coord = Planning_Event_Or_Object_Coord(copy, object); // RA2: as above
		pixel = Planning_Coord_Pixel(coord); // RA2: as above
	}
	return pixel; // RA2: as above
}

int PlanningNodeClass::Lead_Color(void) // RA2: 0x0061A060 (a member of the node in YR); black, or white for a node whose lead owner is selected (or a member of the selected node)
{
	return Planning_Techno_Color(Member_Owner(0)); // RA2: both the owner lookup and its selection color match the YR method
}

void __fastcall Planning_Draw_Dot(Point2D center, int color) // RA2: 0x0061A190; draws a 6 by 6 frame around a pixel
{
	Rect view; // RA2: as above
	view = Get_Title_Screen_Area(); // RA2: as above
	Rect square; // RA2: as above
	square = Planning_Centered_Rect(center, 6); // RA2: as above
	CompositeSurface->Draw_Rect(view, square, color); // RA2: as above
}

void __fastcall Planning_Fill_Dot(Point2D center, int color) // RA2: 0x0061A210; fills a 6 by 6 square around a pixel, less its edge
{
	Rect clip = Get_Title_Screen_Area(); // RA2: as above
	Rect rect = Planning_Centered_Rect(center, 6); // RA2: as above
	Planning_Inflate(rect, -1); // RA2: as above
	CompositeSurface->Fill_Rect(clip, rect, color); // RA2: as above
}

int __fastcall Planning_Guard_Frame(TechnoClass * techno); // RA2: the definition is not exact yet
int __fastcall Planning_Guard_Frame(TechnoClass * techno) // RA2: 0x0061A290; the first cursor frame of the guard order of a techno on guard, else -1
{
	if (techno->Get_Mission() == MISSION_GUARD_AREA) { // RA2: as above
		return Cursor_Start_Frame_76(); // RA2: as above
	}
	return -1; // RA2: as above
}

int __fastcall Planning_Mega_Frame(TechnoClass * techno); // RA2: the definition is not exact yet
int __fastcall Planning_Mega_Frame(TechnoClass * techno) // RA2: 0x0061A2B0; the first cursor frame of the attack move order of a techno, else -1
{
	if (techno->GetMegaMission() == MISSION_ATTACK_MOVE) { // RA2: as above
		return Cursor_Start_Frame_77(); // RA2: as above
	}
	return -1; // RA2: as above
}

int EventClass::Get_Event_Frame(void) const // RA2: 0x0061A2D0
{
	int mission = (signed char)Data.MegaMission.Mission; // RA2: as above
	if (mission == MISSION_GUARD_AREA) { // RA2: as above
		return Cursor_Start_Frame_76(); // RA2: as above
	}
	if (mission == MISSION_ATTACK_MOVE) { // RA2: as above
		return Cursor_Start_Frame_77(); // RA2: as above
	}
	return -1; // RA2: as above
}

void __fastcall Planning_Draw_Midpoint(Point2D start, Point2D end, int color) // RA2: 0x0061A2F0; draws a 21 by 21 frame halfway along a line when it is at least 37 pixels long
{
	if (Distance(start, end) >= 37) { // RA2: as above
		int x = (start.X + end.X) / 2 - 10; // RA2: as above
		int y = (start.Y + end.Y) / 2 - 10; // RA2: as above
		Rect view = Get_Title_Screen_Area(); // RA2: as above
		Rect square(x, y, 21, 21); // RA2: as above
		CompositeSurface->Draw_Rect(view, square, color); // RA2: as above
	}
}

void __fastcall Planning_Draw_Midpoint_Shape(Point2D start, Point2D end, int color, int fill, int frame) // RA2: 0x0061A3C0; draws a framed and filled 21 by 21 box halfway along a line when it is at least 37 pixels long, with a cursor shape in it
{
	if (start.Distance_To(end) >= 37) { // RA2: as above
		int x = (start.X + end.X) / 2; // RA2: as above
		int y = (start.Y + end.Y) / 2; // RA2: as above
		int left = x - 10; // RA2: as above
		int top = y - 10; // RA2: as above
		Rect view = Get_Title_Screen_Area(); // RA2: as above
		Rect square(left, top, 21, 21); // RA2: as above
		Planning_Inflate(square, -1); // RA2: as above
		CompositeSurface->Draw_Rect(view, square, color); // RA2: as above
		Planning_Inflate(square, -1); // RA2: as above
		CompositeSurface->Fill_Rect(view, square, fill); // RA2: as above
		Point2D point(left, top); // RA2: as above
		Rect window = view; // RA2: as above
		ShapeSet const * shape = MouseClass::MouseShapes; // RA2: as above
		int width = shape->Get_Width(); // RA2: as above
		int height = shape->Get_Height(); // RA2: as above
		point.X = x - width / 2 - 1; // RA2: as above
		point.Y = y - height / 2; // RA2: as above
		Draw_Shape(*CompositeSurface, *MouseDrawer, shape, frame, point, window, SHAPE_WIN_REL, NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
	}
}

static bool const PlanningDashes[16] = {true, true, true, true, false, false, false, false, true, true, true, true, false, false, false, false}; // RA2: 0x007EB9F8; the dash pattern of the dashed lines

void __fastcall Planning_Draw_Line(Point2D start, Point2D end, int offset, int color, bool dashed); // RA2: the definition is not exact yet
void __fastcall Planning_Draw_Line(Point2D start, Point2D end, int offset, int color, bool dashed) // RA2: 0x0061A5A0; draws a line on the tactical view moved sideways by offset (clipped to it), dashed or solid
{
	bool steep = abs(end.X - start.X) > abs(end.Y - start.Y); // RA2: as above
	int sideways = steep ? 0 : offset; // RA2: as above
	int lengthways = steep ? offset : 0; // RA2: as above
	start.X += sideways; // RA2: as above
	start.Y += lengthways; // RA2: as above
	end.X += sideways; // RA2: as above
	end.Y += lengthways; // RA2: as above
	Rect area = TacticalRect; // RA2: as above
	if (Clip_Line_To_Rect(start, end, area)) { // RA2: as above
		if (dashed) { // RA2: as above
			int phase = (-(int)timeGetTime() >> 5) & 0xF; // RA2: as above
			CompositeSurface->Draw_Dashed_Line(start, end, color, (bool *)PlanningDashes, phase); // RA2: as above
		} else { // RA2: as above
			CompositeSurface->Draw_Line(start, end, color); // RA2: as above
		}
	}
}

void __fastcall Planning_Draw_Thick_Line(Point2D start, Point2D end, int thickness, int color) // RA2: 0x0061A6A0; draws a solid line thickness pixels wide
{
	int last = thickness / 2; // RA2: as above
	for (int offset = last - thickness + 1; offset <= last; offset++) { // RA2: as above
		Planning_Draw_Line(start, end, offset, color, false); // RA2: as above
	}
}

void __fastcall Planning_Draw_Dashed_Thick_Line(Point2D start, Point2D end, int thickness, int color) // RA2: 0x0061A6F0; draws a dashed line thickness less 2 pixels wide
{
	int count = thickness - 2; // RA2: as above
	int first; // RA2: as above
	int last; // RA2: as above
	if (count <= 0) { // RA2: as above
		first = 0; // RA2: as above
		last = 0; // RA2: as above
	} else { // RA2: as above
		last = count / 2; // RA2: as above
		first = last - count + 1; // RA2: as above
	}
	for (int offset = first; offset <= last; offset++) { // RA2: as above
		Planning_Draw_Line(start, end, offset, color, true); // RA2: as above
	}
}

int __fastcall Planning_Pixel(RGBClass const * color) // RA2: 0x0061A860
{
	unsigned char rr = DSurface::RedLeft; // RA2: as above
	unsigned char gr = DSurface::GreenLeft; // RA2: as above
	unsigned char br = DSurface::BlueLeft; // RA2: as above
	return (color->Get_Red() >> rr << DSurface::RedRight) // RA2: as above
		| (color->Get_Green() >> gr << DSurface::GreenRight)
		| (color->Get_Blue() >> br << DSurface::BlueRight);
}


int __fastcall Planning_Adjusted_Pixel(RGBClass const & color, RGBClass const & other, int ratio) // RA2: 0x00619E00; blends a planning color toward another and packs it as a pixel (ra2-decomp's twin)
{
	RGBClass mixed = color; // RA2: as above
	mixed.Adjust(ratio, other); // RA2: as above
	return (mixed.Get_Red() >> DSurface::RedLeft << DSurface::RedRight) // RA2: as above
		| (mixed.Get_Green() >> DSurface::GreenLeft << DSurface::GreenRight)
		| (mixed.Get_Blue() >> DSurface::BlueLeft << DSurface::BlueRight);
}
