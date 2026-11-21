// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3 partial, written out by lane N23): planning mode's mouse
// handling, unmatched in the YR tree. Written from gamemd.exe's instructions (yr_dis.py) in the
// shape of Red Alert 2's twins (src/recovered/planmgr.cpp: Planning_Mouse_Move_00617900 and
// Planning_Left_Down_00617340). gamemd.exe inlines the hovered node's hit test
// (PlanningNodeClass::Hit_Test, the lead's 8x8 box grown by 4) and the node search
// (Find_Node_At over PlanningNodeClass::Unknown1), and shows each refusal once
// (DAT_00AC4C08) with the scold sound, as the tree's other planning refusals.
// Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------

// The hovered node (DAT_00AC4CCC) when the point is on it, else NULL.
static inline PlanningNodeClass * Hovered_Node_At(int x, int y)
{
	PlanningNodeClass * hover = DAT_00AC4CCC;
	if (hover != NULL && hover->Hit_Test(x, y)) {
		return(hover);
	}
	return(NULL);
}

static inline void Planning_Refuse_Once(char const * name)
{
	if (!DAT_00AC4C08) {
		DAT_00AC4C08 = true;
		wchar_t const * text = StringTable::LoadString(name, NULL, "D:\\ra2mdpost\\PlanMgr.cpp", 0xca1);
		FUN_00730A90(text, 0x1e0);
		VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000);
	}
}

// FUN_0063AB60 (YR 0x0063AB60, 252 bytes): the mouse over the map (ScrollClass::Scroll_AI). In
// planning mode with no button held, over the hovered node or with a node picked up, the cursor
// is the normal one (small when over the picked node or nothing is picked) and the map's own
// cursor is skipped.
bool __stdcall FUN_0063AB60(int x, int y)
{
	if (!PlanningNodeClass::PlanningModeActive) {
		return(false);
	}
	if (DAT_00AC4CB0) {
		return(false);
	}
	PlanningNodeClass * selected = DAT_00AC4C38;
	PlanningNodeClass * hit = Hovered_Node_At(x, y);
	if (selected == NULL && hit == NULL) {
		return(false);
	}
	Map.Set_Default_Mouse(MOUSE_NORMAL, selected == NULL || hit == selected);
	return(true);
}

// FUN_0063A5A0 (YR 0x0063A5A0, 818 bytes): a left button press on the map
// (ScrollClass::Message_Handler). In planning mode a press on the hovered node picks it up
// (FUN_0063A8E0, the release, then connects the selected units' paths to it); a press on another
// planned node that cannot be taken (FUN_0063AC70 not -1) is refused with "InvalidNodeX"; a
// press elsewhere with a selection planning mode cannot plan for (FUN_00639DA0: aircraft,
// buildings, other objects) is refused. True when planning mode took the press.
bool __stdcall FUN_0063A5A0(int x, int y)
{
	if (!PlanningNodeClass::PlanningModeActive) {
		return(false);
	}
	PlanningNodeClass * selected = DAT_00AC4C38;
	Point2D point(x, y);
	PlanningNodeClass * hit = Hovered_Node_At(x, y);
	if (hit != NULL && hit != selected) {
		Map.Set_Default_Mouse(MOUSE_NORMAL, true);
		DAT_00AC4C38 = hit;
	} else {
		PlanningNodeClass * picked = Find_Node_At(PlanningNodeClass::Unknown1, point);
		if (picked != NULL) {
			if (FUN_0063AC70(picked) != -1) {
				Planning_Refuse_Once("MSG:PlanningModeInvalidNodeX");
				return(true);
			}
		} else {
			int kind = FUN_00639DA0();
			if (kind != -1) {
				if (kind == 0) {
					Planning_Refuse_Once("MSG:PlanningModeNoAircraft");
				} else if (kind == 1) {
					Planning_Refuse_Once("MSG:PlanningModeNoBuildings");
				} else if (kind == 2) {
					Planning_Refuse_Once("MSG:PlanningModeNoUnknowns");
				}
				return(true);
			}
		}
	}
	return(selected != NULL || hit != NULL);
}
