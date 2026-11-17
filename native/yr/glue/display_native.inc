
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/display.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

void DisplayClass::Set_Copy_Cursor_Shape(Cell const * list)
{
	if (CursorSize) {
		Cursor_Mark(ZoneCell+ZoneOffset, false);
	}
	ZoneOffset = CELL_NONE;

	if (list) {
		//int	w,h;
		static Cell _list[120];

		memcpy(_list, list, sizeof(_list));
		CursorSize = _list;
		Cell dim = Get_Occupy_Dimensions (CursorSize);
		dim -= Cell(1,1);
		ZoneOffset = -dim / 2;
		Cursor_Mark(ZoneCell+ZoneOffset, true);
	} else {
		CursorSize = 0;
	}
}

Cell DisplayClass::Set_Cursor_Pos(Cell const & xpos)
{
	Cell	prevpos;			// Last position of cursor (for jump-back reasons).
	Cell	pos = xpos;

	/*
	**	Follow the mouse position if no cell number is provided.
	*/
	if (pos == CELL_NONE) {
		Point2D tl = TacticalRect.TopLeft;
		if (TacticalRect.Is_Point_Within(MouseCursor->Get_Mouse_Point())) {
			Point2D mouse = MouseCursor->Get_Mouse_Point();
			Cell click = Map.Click_Cell_Calc(mouse);
			if (click != CELL_NONE) {
				pos = click;
			}
		} else {
			Point2D mouse = MouseCursor->Get_Mouse_Point();
			mouse -= tl;
			pos = TacticalMap->Pixel_To_Cell(mouse);
		}

	}

	if (CursorSize == NULL) {
		prevpos = ZoneCell;
		ZoneCell = pos;
		return(prevpos);
	}

	/*
	**	Adjusts the position so that the placement cursor is never part way off the
	**	tactical map.
	*/
	if (!Debug_Map) {
		pos = TacticalMap->Clamp_Cursor_To_Tactical(pos + ZoneOffset, CursorSize) - ZoneOffset;
	}

	/*
	**	This checks to see if NO animation or drawing is to occur and, if so,
	**	exits.
	*/
	if (pos == ZoneCell) return(pos);

	prevpos = ZoneCell;

	/*
	**	If the cursor is visible, then handle the graphic update.
	**	Otherwise, just update the global position of the cursor.
	*/
	if (CursorSize != NULL) {

		/*
		**	Erase the old cursor (if it exists) AND the cursor is moving.
		*/
		if (pos != ZoneCell && ZoneCell != CELL_NONE) {
			Cursor_Mark(ZoneCell+ZoneOffset, false);
		}

		/*
		**	Render the cursor (could just be animation).
		*/
		if (pos != CELL_NONE) {
			Cursor_Mark(pos+ZoneOffset, true);
		}
	}
	ZoneCell = pos;
	ProximityCheck = Passes_Proximity_Check(PendingObject, PendingHouse, CursorSize, ZoneCell+ZoneOffset);
	ShroudCheck = Passes_Shroud_Check(PendingObject, PendingHouse, CursorSize, ZoneCell+ZoneOffset);

	return(prevpos);
}

// NATIVE: TibSun's cursor cases for Tiberian Sun's own actions (chemical bomb, EM pulse, ion
// cannon, drop pod, rally point) are left out; YR has no such ActionType values.
void DisplayClass::Mouse_Left_Up(Cell const & cell, bool shadow, ObjectClass * object, ActionType action, bool wsmall)
{
	IsTentative = false;

	AbstractClass * target = NULL;
	if (object != NULL) {
		target = object;
	} else {
		if (cell != CELL_NONE) {
			target = &Map[cell];
		}
	}

	if (IsWaypointMode && DraggedWaypoint != NULL) {
		if (PlayerPtr->Waypoint_At(cell) == NULL && Map.In_Local_Radar(cell)) {
			DraggedWaypoint->Location = cell.As_Coord();
			DraggedWaypoint->Location.Z = Map.Get_Height_GL(DraggedWaypoint->Location) + (Map[cell].IsUnderBridge ? BRIDGE_LEPTON_HEIGHT : 0);
		}
	}

	ActionType waypoint_action = Action_To_Waypoint_Action(action, cell);

	if (WaypointColor == RGBClass(0, 0, 0)) {
		unsigned short color = ((unsigned short *)MouseDrawer->Get_Translate_Table())[1];
		WaypointColor = RGBClass((color >> DSurface::RedRight) << DSurface::RedLeft,
								(color >> DSurface::GreenRight) << DSurface::GreenLeft,
								(color >> DSurface::BlueRight) << DSurface::BlueLeft
								);
	}

	switch (waypoint_action) {
		case ACTION_PLACE_WAYPOINT:
		case ACTION_NO_PLACE_WAYPOINT:
		case ACTION_LOOP_WAYPOINT_PATH:
			Update_Waypoint_Color(PlayerPtr->SelectedPath);
			break;

		case ACTION_ENTER_WAYPOINT_MODE:
		case ACTION_FOLLOW_WAYPOINT:
		case ACTION_SELECT_WAYPOINT:
		case ACTION_ATTACK_WAYPOINT:
		case ACTION_ENTER_WAYPOINT:
		case ACTION_PATROL_WAYPOINT: {
			PathType path = PATH_NONE;
			char waypt_id;
			WaypointClass * waypt = PlayerPtr->Waypoint_At(cell);
			PlayerPtr->Fetch_Waypoint_Data(waypt, path, waypt_id);
			if (!IsWaypointMode) {
				PlayerPtr->SelectedPath = path;
			}
			Update_Waypoint_Color(path);
			break;
		}

		default: {
			unsigned short * table = (unsigned short *)MouseDrawer->Get_Translate_Table();
			table[1] = DSurface::Build_Hicolor_Pixel(WaypointColor.Get_Red(), WaypointColor.Get_Green(), WaypointColor.Get_Blue());
			Update_Waypoint_Color(-1);
			if (!IsWaypointMode) {
				PlayerPtr->SelectedPath = PATH_NONE;
			}
			break;
		}
	}

	/*
	**	Don't allow selection of an object that is located in shadowed terrain.
	**	In fact, just show the normal move cursor in order to keep the shadowed
	**	terrain a mystery.
	*/
	if (shadow) {
		switch (waypoint_action) {
			case ACTION_TOTE:
				Set_Default_Mouse(MOUSE_NO_TOTE, wsmall);
				break;

			case ACTION_NO_ENTER:
			case ACTION_NO_ENTER_TUNNEL:
				Set_Default_Mouse(MOUSE_NO_ENTER, wsmall);
				break;

			case ACTION_DAMAGE:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;

			case ACTION_GREPAIR:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;

			case ACTION_NO_DEPLOY:
				Set_Default_Mouse(MOUSE_NO_DEPLOY, wsmall);
				break;

			case ACTION_GUARD_AREA:
				Set_Default_Mouse(MOUSE_AREA_GUARD, wsmall);
				break;

			case ACTION_NONE:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;

			case ACTION_NO_SELL:
			case ACTION_SELL:
			case ACTION_SELL_UNIT:
				Set_Default_Mouse(MOUSE_NO_SELL_BACK, wsmall);
				break;

			case ACTION_NO_GREPAIR:
			case ACTION_NO_REPAIR:
			case ACTION_REPAIR:
				Set_Default_Mouse(MOUSE_NO_REPAIR, wsmall);
				break;

			case ACTION_NUKE_BOMB:
				Set_Default_Mouse(MOUSE_NUCLEAR_BOMB, wsmall);
				break;

			case ACTION_TOGGLE_POWER:
			case ACTION_NO_TOGGLE_POWER:
				Set_Default_Mouse(MOUSE_NO_TOGGLE_POWER, wsmall);
				break;

			case ACTION_HEAL:
				Set_Default_Mouse(MOUSE_HEAL, wsmall);
				break;

			case ACTION_NOMOVE:
				if (CurrentObject.Count() && CurrentObject[0]->Is_Techno() && ((TechnoClass *)CurrentObject[0])->TClass->IsMoveToShroud) {
					Set_Default_Mouse(MOUSE_CAN_MOVE, wsmall);
					break;
				}
				Set_Default_Mouse(MOUSE_NO_MOVE, wsmall);
				break;
				// Fall into next case for non aircraft object types.

			case ACTION_MOVE:
			case ACTION_ATTACK:
				Set_Default_Mouse(MOUSE_CAN_MOVE, wsmall);
				break;

			case ACTION_PLACE_WAYPOINT:
				Set_Default_Mouse(MOUSE_PLACE_WAYPOINT, wsmall);
				break;

			case ACTION_NO_PLACE_WAYPOINT:
				Set_Default_Mouse(MOUSE_NO_PLACE_WAYPOINT, wsmall);
				break;

			case ACTION_ENTER_WAYPOINT_MODE:
				Set_Default_Mouse(MOUSE_ENTER_WAYPOINT_MODE, wsmall);
				break;

			case ACTION_SELECT_WAYPOINT:
				Set_Default_Mouse(MOUSE_SELECT_WAYPOINT, wsmall);
				break;

			case ACTION_LOOP_WAYPOINT_PATH:
				Set_Default_Mouse(MOUSE_LOOP_WAYPOINT_PATH, wsmall);
				break;

			case ACTION_ATTACK_WAYPOINT:
				Set_Default_Mouse(MOUSE_ATTACK_WAYPOINT, wsmall);
				break;

			case ACTION_PATROL_WAYPOINT:
				Set_Default_Mouse(MOUSE_PATROL_WAYPOINT, wsmall);
				break;

			case ACTION_FOLLOW_WAYPOINT:
				Set_Default_Mouse(MOUSE_FOLLOW_WAYPOINT, wsmall);
				break;

			case ACTION_ENTER_WAYPOINT:
				Set_Default_Mouse(MOUSE_ENTER_WAYPOINT, wsmall);
				break;

			default:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;
		}
	} else {

		/*
		**	Change the mouse shape according to the default action that will occur
		**	if the mouse button were clicked at this location.
		*/
		switch (waypoint_action) {
			case ACTION_TOTE:
				Set_Default_Mouse(MOUSE_TOTE, wsmall);
				break;

			case ACTION_NO_ENTER:
				Set_Default_Mouse(MOUSE_NO_ENTER, wsmall);
				break;

			case ACTION_GREPAIR:
				Set_Default_Mouse(MOUSE_GREPAIR, wsmall);
				break;

			case ACTION_TOGGLE_SELECT:
			case ACTION_SELECT:
				Set_Default_Mouse(MOUSE_CAN_SELECT, wsmall);
				break;

			case ACTION_NO_DEPLOY:
				Set_Default_Mouse(MOUSE_NO_DEPLOY, wsmall);
				break;

			case ACTION_GUARD_AREA:
				Set_Default_Mouse(MOUSE_AREA_GUARD, wsmall);
				break;

			case ACTION_MOVE:
				Set_Default_Mouse(MOUSE_CAN_MOVE, wsmall);
				break;

			case ACTION_ATTACK:
				if (target != NULL && CurrentObject.Count() == 1 && CurrentObject[0]->Is_Techno() && ((TechnoClass *)CurrentObject[0])->In_Range(target, 0)) {
					Set_Default_Mouse(MOUSE_STAY_ATTACK, wsmall);
					break;
				}
				// fall into next case.

			case ACTION_HARVEST:
				Set_Default_Mouse(MOUSE_CAN_ATTACK, wsmall);
				break;

			case ACTION_SABOTAGE:
				Set_Default_Mouse(MOUSE_DEMOLITIONS, wsmall);
				break;

			case ACTION_ENTER:
			case ACTION_CAPTURE:
			case ACTION_ENTER_TUNNEL:
				Set_Default_Mouse(MOUSE_ENTER, wsmall);
				break;

			case ACTION_NOMOVE:
				Set_Default_Mouse(MOUSE_NO_MOVE, wsmall);
				break;

			case ACTION_NO_SELL:
				Set_Default_Mouse(MOUSE_NO_SELL_BACK, wsmall);
				break;

			case ACTION_NO_REPAIR:
			case ACTION_NO_GREPAIR:
				Set_Default_Mouse(MOUSE_NO_REPAIR, wsmall);
				break;

			case ACTION_SELF:
				Set_Default_Mouse(MOUSE_DEPLOY, wsmall);
				break;

			case ACTION_REPAIR:
				Set_Default_Mouse(MOUSE_REPAIR, wsmall);
				break;

			case ACTION_SELL_UNIT:
				Set_Default_Mouse(MOUSE_SELL_UNIT, wsmall);
				break;

			case ACTION_NO_TOGGLE_POWER:
				Set_Default_Mouse(MOUSE_NO_TOGGLE_POWER, wsmall);
				break;

			case ACTION_TOGGLE_POWER:
				Set_Default_Mouse(MOUSE_TOGGLE_POWER, wsmall);
				break;

			case ACTION_SELL:
				Set_Default_Mouse(MOUSE_SELL_BACK, wsmall);
				break;

			case ACTION_NUKE_BOMB:
				Set_Default_Mouse(MOUSE_NUCLEAR_BOMB, wsmall);
				break;

			case ACTION_HEAL:
				Set_Default_Mouse(MOUSE_HEAL, wsmall);
				break;

			case ACTION_PLACE_WAYPOINT:
				Set_Default_Mouse(MOUSE_PLACE_WAYPOINT, wsmall);
				break;

			case ACTION_NO_PLACE_WAYPOINT:
				Set_Default_Mouse(MOUSE_NO_PLACE_WAYPOINT, wsmall);
				break;

			case ACTION_ENTER_WAYPOINT_MODE:
				Set_Default_Mouse(MOUSE_ENTER_WAYPOINT_MODE, wsmall);
				break;

			case ACTION_SELECT_WAYPOINT:
				Set_Default_Mouse(MOUSE_SELECT_WAYPOINT, wsmall);
				break;

			case ACTION_LOOP_WAYPOINT_PATH:
				Set_Default_Mouse(MOUSE_LOOP_WAYPOINT_PATH, wsmall);
				break;

			case ACTION_ATTACK_WAYPOINT:
				Set_Default_Mouse(MOUSE_ATTACK_WAYPOINT, wsmall);
				break;

			case ACTION_PATROL_WAYPOINT:
				Set_Default_Mouse(MOUSE_PATROL_WAYPOINT, wsmall);
				break;

			case ACTION_FOLLOW_WAYPOINT:
				Set_Default_Mouse(MOUSE_FOLLOW_WAYPOINT, wsmall);
				break;

			case ACTION_ENTER_WAYPOINT:
				Set_Default_Mouse(MOUSE_ENTER_WAYPOINT, wsmall);
				break;

			default:
				Set_Default_Mouse(MOUSE_NORMAL, wsmall);
				break;
		}
	}
}

void DisplayClass::All_To_Look(bool units_only, bool is_fog)
{
	for (int index = 0; index < Layer[LAYER_GROUND].Count(); index++) {
		TechnoClass * tech = Dynamic_Cast<TechnoClass *>(Layer[LAYER_GROUND][index]);
		if (tech != NULL) {
			if (tech->RTTI == RTTI_BUILDING && units_only) continue;

			if (tech->House->Is_Player_Control()) {
				if (tech->IsDiscoveredByPlayer) {
					tech->Look(false, is_fog);
				}
			} else {
				if (tech->RTTI == RTTI_BUILDING && Rule->IsAllyReveal && tech->House->Is_Ally(PlayerPtr)) {
					tech->Look(is_fog, false);
				}
			}
		}
	}
}
