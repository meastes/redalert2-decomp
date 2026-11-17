// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

#include "vector.h"
#include "event.h"

class TechnoClass;
class HouseClass;
class PlanningNodeClass;

class PlanningMemberClass
{
	public:
		PlanningMemberClass(void);
		void Set_Owner(TechnoClass * owner);
		void Set_Packet(EventClass * packet);
		void Set_Branch(int branch);
		void Set_Active(bool active);
		TechnoClass * Get_Owner(void);
		EventClass * Get_Packet(void);
		int Get_Branch(void);
		bool Get_Active(void);

		TechnoClass * Owner;
		EventClass * Packet;
		int Branch;
		bool Active;
};

class PlanningBranchClass
{
	public:
		PlanningBranchClass(void);
		void Set_Event(EventClass const & event);
		void Set_Count(int count);
		void Set_Color(int color);
		void Clear_Count(void);
		int Inc_Count(void);
		int Dec_Count(void);
		EventClass Get_Event(void);
		int Get_Count(void);
		int Get_Color(void);
		Coord Get_Coord(void);
		Point2D Get_Pixel(void);

		EventClass Event;
		int Count;
		int Color;
};

class PlanningNodeClass
{
	public:
		static DynamicVectorClass<PlanningNodeClass *> Unknown1;
		static DynamicVectorClass<PlanningNodeClass *> DrawnNodes;
		static DynamicVectorClass<PlanningNodeClass *> QueuedNodes;
		static bool PlanningModeActive;

		PlanningNodeClass(int id);
		~PlanningNodeClass();
		int Get_Id(void);
		int Get_Count(void);
		int Index_In(TechnoClass * techno);
		PlanningNodeClass * Prev_Node(TechnoClass * techno);
		PlanningNodeClass * Next_Node(TechnoClass * techno);
		void Add_Member(TechnoClass * techno, EventClass const * event);
		void Remove_Member(TechnoClass * techno);
		int Find_Member(TechnoClass * techno);
		TechnoClass * Member_Owner(int index);
		EventClass Get_Event(int index);
		EventClass Get_Event_For(TechnoClass * techno);
		bool Has_Member(TechnoClass * techno);
		HouseClass * Lead_House(void);
		Coord Lead_Coord(void);
		Point2D Lead_Pixel(void);
		bool Hit_Test(int x, int y);
		bool Lead_At(Coord coord);
		Rect Lead_Rect(void);
		Rect Lead_Box_Rect(void);
		Rect Lead_Box_Rect2(void);
		bool Is_Past_Start(void);
		void Promote_Events(void);
		PlanningMemberClass * Get_Member(TechnoClass * techno);
		TechnoClass * Owner_At_Branch(int branch);
		TechnoClass * Active_Owner(void);
		PlanningBranchClass * Get_Branch(int index);
		PlanningBranchClass * Branch_Of(TechnoClass * techno);
		EventClass * Loop_Event(void);
		Coord Branch_Coord(int index);
		Coord Loop_Coord(void);
		Point2D Branch_Pixel(int index);
		int Branch_Count(void);
		int Has_Loop(void);
		int Is_Loop_Alone(void);
		Point2D Loop_Pixel(void);
		void Set_Queued(void);
		bool Get_Queued(void);
		void Send_Delete_Event(void); // RA2: 0x614AA0; asks all players to delete the planned node of the object the first order points at
		void Dump(void); // RA2: 0x6118E0; prints every branch and who follows it to the debug log
		void Rebuild(void);
		void Update_Active(void);
		int Find_Branch(TechnoClass * techno);
		int Assign_Branch(PlanningMemberClass * member); // RA2: 0x611BD0; counts the member in the branch it follows (a new branch when none matches yet)
		inline void Detach(PlanningMemberClass * member); // RA2: 0x611E20; its expansion stands in Remove_Member
		void Clear(void);
		bool Is_Loop_Start(TechnoClass * techno);
		void Deactivate(PlanningMemberClass * member);
		void Locate_Loop_Branch(void);
		void Select_All(void);
		void Unselect_All(void);
		void Draw_Lead_Box(int color);
		void Fill_Lead_Box(int color);
		void Frame_Lead_Box(int color);
		void Frame_Lead_Box_Outer(int color); // RA2: 0x612420; the same three nested frames around the lead box grown by another 4
		int Node_Color(void);
		int Lead_Color(void);
		int Branch_Color(int index);
		int Loop_Color(void);
		void Reset_Colors(bool all);

		DynamicVectorClass<PlanningMemberClass *> PlanningMembers;
		int Id;
		bool Queued;
		DynamicVectorClass<PlanningBranchClass *> PlanningBranches;
		PlanningBranchClass LoopBranch;
		int LoopBranchIndex;
		int Color;
};

class PlanningTokenClass
{
	public:
		static DynamicVectorClass<PlanningTokenClass *> Array;
		static DynamicVectorClass<EventClass *> Events;
		static DynamicVectorClass<TechnoClass *> Technos;

		PlanningTokenClass(TechnoClass * owner);
		~PlanningTokenClass();
		TechnoClass * Get_Owner(void);
		inline void Cancel_Owner(void); // RA2: 0x612EC0; its expansion stands in the destructor
		void Destroy(void);
		void Set_F1C(void);
		void Clear_F1C(void);
		bool Get_F1C(void);
		bool Owner_Is_Mission(void);
		bool Has_Started(void);
		EventClass Pop_Front(void);
		void Share_Range(TechnoClass * techno, int start, int count);
		void Share_From(TechnoClass * techno, int start);
		void Remove_Node_At(int index); // RA2: 0x6138F0; drops a node of the path and moves the current and the loop marks with it
		void Reset_Node_Colors(void); // RA2: 0x613A80; puts the colors of the nodes from the current one on back to unset
		int Get_F8C(void);
		bool Reached(int index);
		int Node_Count(void);
		int Current_Or_Count(void);
		int Remaining(void);
		void Add_Node(PlanningNodeClass * node);
		PlanningNodeClass * Node_At(int index);
		PlanningNodeClass * Last_Node(void);
		bool Has_Node(PlanningNodeClass * node);
		int Node_Index(PlanningNodeClass * node);
		EventClass Event_At(int index);
		EventClass Last_Popped(void);
		PlanningNodeClass * Loop_Node(void);
		void Set_Loop(int count, int start);
		int Get_Loop_Count(void);
		int Get_Loop_Start(void);
		bool Loop_Start_Valid(void);
		bool Loop_Start_Positive(void);
		bool Loop_Start_Zero(void);
		bool Get_F98(void);
		bool Get_F99(void);
		void Dec_Loop_Start(void);

		TechnoClass * OwnerUnit;
		DynamicVectorClass<PlanningNodeClass *> PlanningNodes;
		bool F1C;
		EventClass Popped;
		int F8C;
		int ClosedLoopNodeCount;
		int StepsToClosedLoop;
		bool F98;
		bool F99;
};

struct PlanningEventTargets : public EventClass // RA2: a view of an event for the planning helpers: its mission and its first target
{
	int CurrentMission(void) const {return (signed char)Data.MegaMission.Mission;}
	TargetClass const & Target(void) const {return *(TargetClass const *)&Data.MegaMission.Target;}
};
bool __fastcall Planning_Add_Order(TechnoClass * techno, EventClass * event); // RA2: 0x614EC0; plans an order for a techno
void __fastcall Planning_Execute(EventClass * event); // RA2: 0x614BA0; carries out a planning event

class RGBClass; // RA2: the helpers below and the globals of the planning module

struct PlanningPoint {
	int X;
	int Y;
	PlanningPoint & operator=(Point2D const & point) { *(Point2D *)this = point; return *this; }
}; // RA2: a pair of plain ints (the planning module runs no initializer for the mouse position at 0xA76728)
class INoticeSink;

void Planning_Set_Sink(INoticeSink * sink); // RA2: 0x613E00
void __cdecl Planning_Reset(void); // RA2: 0x613E10; leaves planning mode and forgets the picked nodes
void __cdecl Planning_Reset_Wrapper(void); // RA2: 0x613E40
void __cdecl Planning_Reset_State(void); // RA2: 0x613E50; deletes every token and clears every list and flag
void __fastcall Planning_Update(void); // RA2: 0x614010; the planning mode's per frame update
void __fastcall Planning_Free_Nodes(void); // RA2: 0x6142F0; runs the planned events still held and frees them
void __fastcall Planning_Store_Event(EventClass const & event); // RA2: 0x614350
void __cdecl Planning_Process_Queue(void); // RA2: 0x6146C0
void __cdecl Planning_Begin(void); // RA2: 0x614760
void __cdecl Planning_End(void); // RA2: 0x6147B0
void __fastcall Commit_Plans(int house); // RA2: 0x614850
bool __fastcall Planning_Allows_Event_Type(int type); // RA2: 0x614B50
void __stdcall Planning_Send_Event(EventClass event); // RA2: 0x614B70
void __cdecl Planning_Hover(void); // RA2: 0x615C00
void __fastcall Planning_Refuse_Command(int kind); // RA2: 0x617250
bool __fastcall Planning_Mission_Allows(TechnoClass * techno); // RA2: 0x615A10
int __fastcall Planning_Order_Kind(TechnoClass * techno, PlanningEventTargets const * event); // RA2: 0x615A80; how an order relates to the path of the techno: 0 refused outright, 1 and 2 past a terminating or a continual order, -1 taken
void __fastcall Planning_Path_Append(TechnoClass * techno, PlanningNodeClass * node); // RA2: 0x6165A0; gives the techno the rest of the path of the node's first member from the node on
void __fastcall Planning_Copy_Members(PlanningNodeClass * node, PlanningNodeClass * from, EventClass event); // RA2: 0x6167F0; adds every member of one node to another, each with a copy of the event aimed at its techno
void __fastcall Planning_Remove_Node(PlanningNodeClass * node); // RA2: 0x616960; takes a node out of the paths of its members
void __fastcall Planning_Convert_Loop_Moves(TechnoClass * techno); // RA2: 0x616A50; in the path of the techno turns the move orders after the loop start into the other move mission (event mission 2 into 0x29)
Rect __fastcall Planning_Centered_Rect(Point2D center, int size); // RA2: 0x6112D0; a size by size rectangle centered on a pixel

inline void __fastcall Planning_Uncount(int house); // RA2: 0x616D20 (expanded where it is used); lowers the count of a house, never below 0

extern bool PlanningTearDown; // RA2: 0xA767A5; set while the planning lists are being emptied
extern int PlanningUnitCounts[8]; // RA2: 0xA76684; per house, the number of units with a planned path
extern INoticeSink * PlanningSink; // RA2: 0xA76680; told when planning mode starts and ends
extern int PlanningHelpCounts[5]; // RA2: 0xA766B4; how often each help message was shown
extern bool PlanningRefusalShown; // RA2: 0xA766C8; a refusal was already shown this frame
extern int PlanningBlockedNode; // RA2: 0xA766FC; the node (as a number) the mouse is over but may not use
extern PlanningPoint PlanningMouseDown; // RA2: 0xA76728; the screen point the mouse button went down at, (-1, -1) for none
extern int PlanningPendingRefusal; // RA2: 0xA76750; the refusal to show at the end of the frame, -1 for none
extern bool PlanningDragging; // RA2: 0xA76770
extern bool PlanningLoopClosed; // RA2: 0xA76788; a loop was closed this frame
extern bool PlanningCommandFlag; // RA2: 0xA76790
extern bool PlanningBusy; // RA2: 0xA767A6; guards Select_Planning_Members against re-entry
extern PlanningNodeClass * PlanningHoverNode; // RA2: 0xA7678C; the node under the mouse
extern PlanningNodeClass * PlanningPickedNode; // RA2: 0xA766F8; the node a path is being drawn from
extern PlanningNodeClass * PlanningSelectedNode; // RA2: 0xA766B0; the node whose members are shown selected

/*
**	RA2: the helpers planmgr.cpp defines beside the classes (free fastcall functions here; YR has some of
**	them as static members).
*/
void __fastcall Planning_Empty(void); // RA2: 0x610760; does nothing, first of the planning module
int __fastcall Planning_House(TechnoClass * techno); // RA2: 0x610770; the house a techno plans for (the player's when its house is human controlled)
Point2D __fastcall Planning_Coord_Pixel(Coord const & coord); // RA2: 0x6107A0
Coord __fastcall Planning_Event_Coord(EventClass const & event); // RA2: 0x610980; where the order of an event points, or (-1, -1, -1)
void __fastcall Planning_Inflate(Rect & rect, int amount); // RA2: 0x612570
void __fastcall Planning_Unlist_Node(PlanningNodeClass * node); // RA2: 0x6143E0
void __fastcall Planning_Queue_Node(PlanningNodeClass * node); // RA2: 0x614650
void __fastcall Planning_Unlist_Token(PlanningTokenClass * token); // RA2: 0x6144F0
void __fastcall Planning_Unlist_Techno(TechnoClass * techno); // RA2: 0x614550
int __fastcall Planning_Bright_Color(TechnoClass * techno); // RA2: 0x619D50 (also expanded where it is used); the color of the techno's house, brightened
int __fastcall Planning_Adjusted_Pixel(RGBClass const & color, RGBClass const & target, int ratio); // RA2: 0x619E00
