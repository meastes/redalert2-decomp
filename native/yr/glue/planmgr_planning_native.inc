
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-INS (lane N3, 2026-11-19), appended to the planmgr unit: planning mode's functions
// unmatched in ra2-decomp. They were link-closure traps: the command bar's PlanningMode button with units
// selected stopped the game at once (FUN_0063B550, the first frame's draw), and a planned order or a commit
// would have reached the rest. Each is written from gamemd.exe's instructions (native/yr/tools/yr_dis.py)
// in the tree's names, with the matched helpers it calls or inlines, on the shape of Red Alert 2's drafts of
// the twins (src/recovered/planmgr.cpp: Planning_Execute 0x00614BA0, Planning_Add_Order 0x00614EC0,
// Planning_Draw_Techno 0x006182F0, Planning_Draw_Node 0x00619420 / 0x00619870) and of the tree's matched
// filled twin Draw_Filled_Unit_Waypoints (0x0063BE60). Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------

// The planning node at a coordinate (any id when id is -1; any house when house is -1), as FUN_00639210
// does but tested with Lead_At, as gamemd.exe's inlined loops in 0x00637E00 and 0x00638120 test it.
static PlanningNodeClass * Planning_Node_At(int house, int id, Coord const & coord)
{
	int count = PlanningNodeClass::Unknown1.Count();
	for (int i = 0; i < count; i++) {
		PlanningNodeClass * node = PlanningNodeClass::Unknown1[i];
		if (node->Lead_At(coord) && (id == -1 || id == node->Id)) {
			int owner = FUN_006339B0(node->Member_Owner(0));
			if (house == -1 || house == owner) {
				return node;
			}
		}
	}
	return NULL;
}

// A techno's planning token, made and listed when it has none (inlined in 0x00637E00 and 0x00638120).
static PlanningTokenClass * Planning_Token_Of(TechnoClass * techno)
{
	PlanningTokenClass * token = techno->FUN_00705D20();
	if (token == NULL) {
		token = new PlanningTokenClass(techno);
		PlanningTokenClass::Array.Add(token);
		techno->FUN_00705D10(token);
	}
	return token;
}

// gamemd.exe 0x00639CB0 (239 bytes): after a path is closed into a loop (Close_Loop), when a member of the
// path's first node can attack on the move, the loop's attack orders (mission 2) become attack-move orders
// (mission 0x1D).
void __fastcall FUN_00639CB0(TechnoClass * techno)
{
	PlanningTokenClass * token = techno->FUN_00705D20();
	int count = token->PlanningNodes.Count();
	int start = token->StepsToClosedLoop;
	PlanningNodeClass * first = token->Node_At(0);
	bool can = false;
	for (int i = 0; i < first->PlanningMembers.Count(); i++) {
		if (first->PlanningMembers[i]->Owner->CanAttackOnTheMove()) {
			can = true;
			break;
		}
	}
	if (!can) {
		return;
	}
	for (int n = start; n < count; n++) {
		PlanningNodeClass * node = token->Node_At(n);
		for (int k = 0; k < node->PlanningMembers.Count(); k++) {
			EventClass * packet = node->PlanningMembers[k]->Packet;
			if (packet->Data.MegaMission.Mission == 2) {
				packet->Data.MegaMission.Mission = 0x1D;
			}
		}
	}
}

// gamemd.exe 0x00634E10 (585 bytes): files a member of the node under its branch: the branch that already
// holds its owner (counted once more), else none when the node follows the owner's last node on its path,
// else a new branch from the owner's previous node's order; the loop branch's index is found again.
void PlanningNodeClass::FUN_00634E10(PlanningMemberClass * member)
{
	TechnoClass * owner = member->Owner;
	int found = Find_Branch(owner);
	if (found != -1) {
		PlanningBranchClass * branch = PlanningBranches[found];
		branch->Color = -1;
		Color = -1;
		branch->Count++;
		member->Branch = found;
		return;
	}
	PlanningTokenClass * token = owner->FUN_00705D20();
	int index = token->PlanningNodes.ID(this);
	PlanningNodeClass * previous;
	if (index == -1) {
		previous = NULL;
	} else if (token->PlanningNodes.Count() == 1) {
		previous = this;
	} else {
		index--;
		if (index < 0) index = token->PlanningNodes.Count() - 1;
		previous = token->Node_At(index);
	}
	if (previous == owner->FUN_00705D20()->Last_Node()) {
		member->Branch = -1;
		return;
	}
	EventClass event = *previous->Get_Member(owner)->Packet;
	PlanningBranchClass * branch = new PlanningBranchClass;
	branch->Event = event;
	branch->Count++;
	PlanningBranches.Add(branch);
	int result = PlanningBranches.Count() - 1;
	if (LoopBranch.Count > 0 && LoopBranchIndex == -1) {
		LoopBranchIndex = -1;
		EventClass loop = LoopBranch.Event;
		for (int i = 0; i < PlanningBranches.Count(); i++) {
			EventClass other = PlanningBranches[i]->Event;
			if (FUN_00633BC0(loop) == FUN_00633BC0(other)) {
				LoopBranchIndex = i;
				break;
			}
		}
	}
	Color = -1;
	member->Branch = result;
}

// gamemd.exe 0x00638120 (1,181 bytes; Red Alert 2's Planning_Add_Order 0x00614EC0): plans an order for a
// techno: adds it to the node at the order's coordinate, or to a new node, unless the order or the techno's
// path refuses it (the player hears and reads why).
static bool Planning_Add_Order_00638120(TechnoClass * techno, EventClass * event)
{
	if (techno == NULL) {
		return false;
	}
	bool player = event->ID == PlayerPtr->HeapID;
	Coord coord = FUN_00633BC0(*event);
	if (coord == Coord(-1, -1, -1)) {
		return false;
	}
	PlanningNodeClass * node = Planning_Node_At(event->ID, event->Frame, coord);
	if (node && node->Find_Member(techno) != -1) {
		return true;
	}
	int kind = FUN_00638CE0(techno, *event);
	if (kind != -1) {
		if (!player) {
			return false;
		}
		if (kind == 0) {
			FUN_0063A280((char const *)event);
		} else if (kind == 1) {
			FUN_0063A230("MSG:PostTerminatingCommand");
		} else if (kind == 2) {
			FUN_0063A230("MSG:PostContinualCommand");
		}
		return false;
	}
	PlanningTokenClass * token = Planning_Token_Of(techno);
	if (player) {
		int mission = event->Data.MegaMission.Mission;
		int result = -1;
		if (mission == 7 || mission == 8 || mission == 9) {
			result = 0;
		} else if (FUN_00638C40(mission, Planning_Target(event->Data.MegaMission.Target))) {
			result = 0;
		} else if (token->PlanningNodes.Count() == 15) {
			result = 2;
		}
		if (result != -1) {
			DAT_00AC4C90 = result;
		}
	}
	if (token->StepsToClosedLoop >= 0) {
		if (player) {
			FUN_0063A230("MSG:PostLoop");
		}
		return false;
	}
	if (token->PlanningNodes.Count() == 0) {
		PlanningTokenClass::Technos.Add(techno);
		if (event->ID >= 0 && event->ID < 24) {
			DAT_00AC4B84[event->ID]++;
		}
	} else if (token->PlanningNodes.Count() == 16) {
		if (player) {
			FUN_0063A230("MSG:NodeMaximum");
		}
		return false;
	}
	if (node == NULL) {
		node = new PlanningNodeClass(event->Frame);
		PlanningNodeClass::Unknown1.Add(node);
	}
	PlanningNodeClass * last = token->Last_Node();
	if (last) {
		FUN_00639A50(node, last, *event);
	} else {
		node->Add_Member(techno, event);
	}
	if (player) {
		VocClass::PlayGlobal(Rule->AddPlanningModeCommandSound, 1.0f, 0x2000);
	}
	return true;
}

// gamemd.exe 0x00637E00 (786 bytes; Red Alert 2's Planning_Execute 0x00614BA0): carries out a planning event:
// PLAN_COMMIT (0x2B) starts the paths of the event's house again; PLAN_CONNECT (0x2A) adds the node at the
// second target's coordinate to the first target's path (closing a loop when the path already has it);
// PLAN_NODE_DELETE (0x2C) takes the node at the target's coordinate out of every path (the player's next node
// is remembered); any other event is a planned order for its techno.
void __fastcall FUN_00637E00(EventClass * event)
{
	if (event->Type == 0x2B) {
		int house = event->ID;
		int count = Technos.Count();
		for (int i = 0; i < count; i++) {
			TechnoClass * techno = Technos[i];
			if (house == FUN_006339B0(techno)) {
				PlanningTokenClass * token = techno->FUN_00705D20();
				if (token) {
					bool started = token->Has_Started();
					token->FUN_00636CE0();
					bool allows = FUN_00638C70(techno);
					if (!started && allows) {
						techno->TryNextPlanningTokenNode();
					}
				}
			}
		}
	} else if (event->Type == 0x2A) {
		TechnoClass * techno = event->Data.NavCom.Whom.As_Techno();
		if (techno == NULL) return;
		AbstractClass * object = event->Data.NavCom.Where.As_Abstract();
		if (object == NULL) return;
		Coord coord = object->As_Coord();
		PlanningNodeClass * node = Planning_Node_At(event->ID, -1, coord);
		if (node == NULL) return;
		PlanningTokenClass * token = Planning_Token_Of(techno);
		if (token->PlanningNodes.ID(node) != -1) {
			FUN_00639740(techno, node);
		} else {
			FUN_00639800(techno, node);
		}
	} else if (event->Type == 0x2C) {
		AbstractClass * object = event->Data.Target.Whom.As_Abstract();
		if (object == NULL) return;
		Coord coord = object->As_Coord();
		int house = event->ID;
		PlanningNodeClass * node = Planning_Node_At(house, -1, coord);
		if (node == NULL) return;
		TechnoClass * owner = node->PlanningMembers[0]->Owner;
		int index = owner->FUN_00705D20()->PlanningNodes.ID(node);
		PlanningNodeClass * next = owner->FUN_00705D20()->Node_At(index + 1);
		FUN_00639BC0(node);
		if (next && house == PlayerPtr->HeapID) {
			DAT_00AC4BF0 = next;
		}
	} else {
		TechnoClass * techno = event->Data.MegaMission.Whom.As_Techno();
		Planning_Add_Order_00638120(techno, event);
	}
}

// gamemd.exe 0x0063B550 (2,239 bytes; Red Alert 2's Planning_Draw_Techno 0x006182F0): one techno's current
// order and its first planned leg in outline (the planning mode's view), as the tree's filled twin
// Draw_Filled_Unit_Waypoints (0x0063BE60) draws them: outlined dots (FUN_0063D400), solid thick lines
// (FUN_0063D910) and outlined midpoint squares (FUN_0063D560), all in one colour: the highlight when the
// techno is in the node under the mouse (or selected), else black.
void __fastcall FUN_0063B550(TechnoClass * techno)
{
	PlanningTokenClass * token = techno->FUN_00705D20();
	Coord origin = techno->As_Coord();
	Point2D center = FUN_006339E0(origin);
	Point2D source = center;
	int color = FUN_0063DAD0(DAT_00A83CD9);
	if (techno) {
		int highlight = FUN_0063DAD0(DAT_00A80220);
		if (DAT_00AC4BF0 ? DAT_00AC4BF0->Find_Member(techno) != -1 : techno->IsSelected) {
			color = highlight;
		}
	}
	bool has_nodes = Has_Planned_Nodes(token);
	bool draw_action = Should_Draw_Active_Action(techno, has_nodes);
	int width = 4;
	if (token && token->Owner_Is_Mission()) {
		EventClass packet(token->Popped);
		Coord coord = FUN_0063CA80(packet, techno);
		Point2D target = FUN_006339E0(coord);
		FUN_0063D400(center, color);
		FUN_0063D910(center, target, 3, color);
		if (FUN_0063D540(packet) != -1) FUN_0063D560(center, target, color);
		FUN_0063D400(target, color);
		return;
	}
	if (draw_action) {
		bool mega_mission = techno->HaveMegaMission();
		bool attack_move = techno->HaveAttackMoveTarget();
		Coord coord = techno->GetAttackCoordinates();
		Point2D action = FUN_006339E0(coord);
		if (techno->IsInLimbo) {
			FUN_0063D400(action, color);
			return;
		}
		FUN_0063D400(center, color);
		FUN_0063D910(center, action, 3, color);
		if (Active_Waypoint_Frame(techno, mega_mission, attack_move) != -1) FUN_0063D560(center, action, color);
		FUN_0063D400(action, color);
		if (attack_move) {
			Coord attack_coord = techno->GetAttackMoveCoords();
			Point2D dest = FUN_006339E0(attack_coord);
			FUN_0063D910(action, dest, 3, color);
			if (techno->GetMegaMission() == 0x1d && FUN_00731CD0() != -1) FUN_0063D560(action, dest, color);
			FUN_0063D400(dest, color);
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
	if (branch) target = FUN_0063D210(&branch->Event, techno);
	FUN_0063D400(source, color);
	FUN_0063D910(source, target, width, color);
	if (branch) FUN_0063D400(target, color);
	if (Planned_Waypoint_Frame(node->Get_Event_For(techno)) != -1) FUN_0063D560(source, target, color);
}

// The pixel a node line starts from: the branch's coordinate, or its event's (owner's) when it has none.
static Point2D Planning_Branch_Start(PlanningBranchClass * branch, TechnoClass * owner)
{
	Coord coord = branch->Get_Coord();
	Point2D start = FUN_006339E0(coord);
	if (coord == Coord(-1, -1, -1)) {
		start = FUN_0063D210(&branch->Event, owner);
	}
	return start;
}

static int Planning_Node_Frame(PlanningNodeClass * node)
{
	int frame = -1;
	if (node->PlanningMembers.Count() > 0) {
		EventClass event = node->Get_Event(0);
		frame = FUN_0063D540(event);
	}
	return frame;
}

// gamemd.exe 0x0063C690 (994 bytes; Red Alert 2's Planning_Draw_Node 0x00619420): a node in outline: solid
// thick lines from each branch (a square on the middle of a long one when the order has a cursor), the loop
// line and the outlined box, in the node's first member's colour (FUN_0063D0F0).
void PlanningNodeClass::FUN_0063C690(void)
{
	Point2D pixel = Lead_Pixel();
	int frame = Planning_Node_Frame(this);
	int color = FUN_0063D0F0(Member_Owner(0));
	int count = PlanningBranches.Count();
	for (int index = 0; index < count; index++) {
		Point2D start = Planning_Branch_Start(PlanningBranches[index], Owner_At_Branch(index));
		FUN_0063D910(start, pixel, 4, color);
		if (frame != -1) FUN_0063D560(start, pixel, color);
	}
	if (Is_Loop_Alone()) {
		Point2D start = Planning_Branch_Start(&LoopBranch, Active_Owner());
		FUN_0063D910(start, pixel, 4, color);
		if (frame != -1) FUN_0063D560(start, pixel, color);
	}
	Draw_Lead_Box(color);
}

// gamemd.exe 0x0063CAE0 (852 bytes; Red Alert 2's Planning_Draw_Node 0x00619870): a node filled: dashed thick
// lines from each branch in the branch's colour and the loop line in the loop's, cursor shapes on their middles
// (FUN_0063D630), and the filled box in the node's colour.
void PlanningNodeClass::FUN_0063CAE0(void)
{
	Point2D pixel = Lead_Pixel();
	int frame = Planning_Node_Frame(this);
	int color = FUN_0063D0F0(Member_Owner(0));
	int count = PlanningBranches.Count();
	for (int index = 0; index < count; index++) {
		int branch_color = Branch_Color(index);
		Point2D start = Planning_Branch_Start(PlanningBranches[index], Owner_At_Branch(index));
		FUN_0063D960(start, pixel, 4, branch_color);
		if (frame != -1) FUN_0063D630(start, pixel, branch_color, color, frame);
	}
	if (Is_Loop_Alone()) {
		int loop_color = Loop_Color();
		Point2D start = Planning_Branch_Start(&LoopBranch, Active_Owner());
		FUN_0063D960(start, pixel, 4, loop_color);
		if (frame != -1) FUN_0063D630(start, pixel, loop_color, color, frame);
	}
	Fill_Lead_Box(Node_Color());
}
