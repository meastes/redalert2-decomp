/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the commands the keyboard and the interface give the player: deploy, stop, guard, the
**	control groups, attack move, selection by type, state or rank. TibSun has no such file; the
**	code is the executable's from 0x006F57E0.
*/

#include "always.h"
#include "rect.h"
#include "geometry.h"
#include <string.h>
#include "vector.h"
#include "_tactica.h"
#include "globals.h"
#include "house.h"
#include "tactical.h"
#include "techno.h"
#include "foot.h"
#include "infantry.h"
#include "building.h"
#include "event.h"
#include "target.h"
#include "msglist.h"
#include "session.h"
#include "scenario.h"
#include "voc.h"
#include "_rules.h"
#include "rules.h"
#include "queue.h"
#include "_map.h"
#include "display.h"
#include "planmgr.h"

#include "_keyboar.h"
#include "goptions.h"
#include "mouse.h"
#include "planning.h"
#include "stringtable.h"
#include "beacon.h"
#include "revent.h"
#include "cell.h"
#include <wchar.h>
#include <stdio.h>
#include "tab.h"

typedef DynamicVectorClass<TechnoClass *> TechnoList;
typedef bool(__fastcall * TechnoTest)(TechnoClass * techno, int argument);
typedef void(__fastcall * TechnoAction)(TechnoClass * techno, int argument);

TechnoList * TypeSelectList;


/// <summary>
/// Frees the list of objects a type, state or rank selection works on.
/// </summary>
void Free_Type_Select_List(void)
{
	if (TypeSelectList != NULL) {
		delete TypeSelectList;
		TypeSelectList = NULL;
	}
}

/// <summary>
/// Whether an object belongs to the player's side, which depends on the game type.
/// </summary>
static inline bool Is_Player_Side(TechnoClass * techno)
{
	if (Session.Type == GAME_NORMAL) {
		return techno->House->IsPlayerControl;
	}
	return techno->House->Is_Player_Control();
}

/// <summary>
/// Whether every object of the player's control group is selected.
/// </summary>
bool __fastcall Group_Is_All_Selected(int group)
{
	int count = Technos.Count();
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && !techno->IsInLimbo && Is_Player_Side(techno) && !techno->IsSelected && techno->Group == group - 1) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// Counts the player's objects in a control group.
/// </summary>
int __fastcall Group_Count(int group)
{
	int members = 0;
	int count = Technos.Count();
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && !techno->IsInLimbo && Is_Player_Side(techno) && techno->Group == group - 1) {
			members++;
		}
	}
	return members;
}

/// <summary>
/// Puts a text on the player's message list in the player's color.
/// </summary>
void __fastcall Show_Player_Message(wchar_t const * text, int delay)
{
	if (delay == -1) {
		delay = 240;
	}
	int color = 3;
	if (PlayerPtr != NULL) {
		color = PlayerPtr->Scheme;
	}
	Session.Messages.Add_Message(NULL, 0, text, color, TextPrintType(0x4046), delay, true);
}

/// <summary>
/// The style the player's messages are printed in.
/// </summary>
int Message_Style(void)
{
	return 0x4046;
}

/// <summary>
/// How many frames the player's messages stay by default.
/// </summary>
int Message_Delay(void)
{
	return 240;
}

/// <summary>
/// Deploys the selected objects that can deploy, or has an occupied building let its occupants out.
/// </summary>
void __cdecl Command_Deploy(void)
{
	bool deployed = false;
	if (CurrentObject.Count() <= 0) {
		return;
	}
	if (CurrentObject[0] != NULL) {
		BuildingClass * building = (BuildingClass *)CurrentObject[0];
		if (building != NULL && building->Fetch_RTTI() == RTTI_BUILDING && building->GetOccupantCount() > 0 && building->House->Is_Player_Control()) {
			ObjectClass * object = CurrentObject[0];
			if (object != NULL && (object->AbstractFlags & 1) && ((TechnoClass *)object)->ClickedEvent(EventClass::DEPLOY)) {
				VocClass::PlayGlobal(Rule->DeploySound, 1.0f, 0x2000);
			}
			return;
		}
	}
	int count = CurrentObject.Count();
	bool undeployed = false;
	int index = 0;
	bool deploying = false;
	for (; index < count; index++) {
		ObjectClass * object = CurrentObject[index];
		if (object != NULL && object->Fetch_RTTI() == RTTI_INFANTRY && ((InfantryClass *)object)->Is_Deployer()) {
			if (((InfantryClass *)object)->Is_Deploying()) {
				deploying = true;
			} else {
				undeployed = true;
			}
			if (deploying && undeployed) {
				break;
			}
		}
	}
	for (index = 0; index < count; index++) {
		ObjectClass * object = CurrentObject[index];
		if (object != NULL && (object->Can_Player_Move() || object->Can_Player_Fire()) && (object->AbstractFlags & 1) && ((TechnoClass *)object)->Can_Attack_Now() && ((TechnoClass *)object)->Can_Deploy_Now()) {
			if (object->Fetch_RTTI() == RTTI_INFANTRY && ((InfantryClass *)object)->Is_Deployer() && deploying && undeployed && ((InfantryClass *)object)->Is_Deploying()) {
				continue;
			}
			if (((TechnoClass *)object)->ClickedEvent(EventClass::DEPLOY)) {
				deployed = true;
			}
		}
	}
	if (deployed) {
		VocClass::PlayGlobal(Rule->DeploySound, 1.0f, 0x2000);
	}
}

/// <summary>
/// Orders the selected objects to guard the area they stand in.
/// </summary>
void __fastcall Command_Guard(void)
{
	if (Planning_Is_Active()) {
		Planning_Refuse_Area_Guard();
		return;
	}
	if (CurrentObject.Count() > 0) {
		bool agreed = true;
		int count = CurrentObject.Count();
		for (int index = 0; index < count; index++) {
			ObjectClass * object = CurrentObject[index];
			if (object != NULL && (object->AbstractFlags & ABSTRACT_TECHNO) && object->Can_Player_Move() && object->Can_Player_Fire()) {
				agreed = ((TechnoClass *)object)->Player_Assign_Mission(MISSION_GUARD_AREA, (AbstractClass *)object->Get_Target_Cell_Ptr(), NULL, NULL);
				if (!agreed) {
					return;
				}
			}
		}
		if (agreed) {
			VocClass::PlayGlobal(Rule->GuardSound, 1.0f, 0x2000);
		}
	}
}


/// <summary>
/// Orders the selected objects to stop.
/// </summary>
void __cdecl Command_Stop(void)
{
	if (CurrentObject.Count() > 0) {
		bool agreed = true;
		int count = CurrentObject.Count();
		for (int index = 0; index < count; index++) {
			ObjectClass * object = CurrentObject[index];
			if (object != NULL && (object->AbstractFlags & 1) && (object->Can_Player_Move() || object->Can_Player_Fire())) {
				agreed = ((TechnoClass *)object)->ClickedEvent(EventClass::IDLE);
				if (!agreed) {
					break;
				}
			}
		}
		if (agreed) {
			VocClass::PlayGlobal(Rule->StopSound, 1.0f, 0x2000);
		}
	}
}

int CheerFrame;

/// <summary>
/// Queues a cheer event once the cheer rate allows another one.
/// </summary>
void __cdecl Command_Cheer(void)
{
	if (Frame >= CheerFrame && MuteSWLaunches == 0) {
		OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::ALL_CHEER));
		CheerFrame = Rule->MaximumCheerRate + Frame;
	}
}


/// <summary>
/// Orders the selected objects that can move to scatter.
/// </summary>
void __cdecl Command_Scatter(void)
{
	if (CurrentObject.Count() > 0) {
		bool agreed = true;
		int count = CurrentObject.Count();
		for (int index = 0; index < count; index++) {
			ObjectClass * object = CurrentObject[index];
			if (object != NULL && (object->AbstractFlags & 1) && object->Can_Player_Move()) {
				agreed = ((TechnoClass *)object)->ClickedEvent(EventClass::SCATTER);
				if (!agreed) {
					break;
				}
			}
		}
		if (agreed) {
			VocClass::PlayGlobal(Rule->ScatterSound, 1.0f, 0x2000);
		}
	}
}

/// <summary>
/// Makes the selected objects the control group, taking them out of any other group first.
/// </summary>
void __fastcall Group_Assign(int group)
{
	int count = Technos.Count();
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && !techno->IsInLimbo && Is_Player_Side(techno)) {
			if (techno->Group == group - 1) {
				techno->Group = -1;
			}
			if (techno->IsSelected) {
				techno->Group = group - 1;
			}
		}
	}
}

/// <summary>
/// Takes every object of the player out of a control group.
/// </summary>
void __fastcall Group_Clear(int group)
{
	int count = Technos.Count();
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && !techno->IsInLimbo && Is_Player_Side(techno) && techno->Group == group - 1) {
			techno->Group = -1;
		}
	}
}


/// <summary>
/// Whether the selection is exactly the control group: every member selected and nothing else.
/// </summary>
bool __fastcall Group_Is_Selection(int group)
{
	for (int index = Technos.Count() - 1; index >= 0; index--) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && !techno->IsInLimbo && Is_Player_Side(techno)) {
			if (techno->Group == group - 1 && !techno->IsSelected) {
				return false;
			}
			if (techno->Group != group - 1 && techno->IsSelected) {
				return false;
			}
		}
	}
	return true;
}

int GroupSelectFrame = -100;
int GroupSelectGroup = -1;
int RadarJumpFrame = -100;

/// <summary>
/// Leaves the modes that wait for a click on the map (power, waypoint, repair, sell and beacon).
/// </summary>
static inline void Cancel_Click_Modes(void)
{
	Map.Power_Mode_Control(0);
	Map.Waypoint_Mode_Control(0, false);
	Map.Repair_Mode_Control(0);
	Map.Sell_Mode_Control(0);
	Map.Place_Beacon_Mode_Control(0);
}

/// <summary>
/// Selects the control group; a second press within 25 frames centers the view on it.
/// </summary>
void __fastcall Group_Select(int group)
{
	Cancel_Click_Modes();
	if (Frame - GroupSelectFrame < 25 && group == GroupSelectGroup && Group_Is_Selection(group)) {
		Group_Reselect(group);
		return;
	}
	GroupSelectFrame = Frame;
	GroupSelectGroup = group;
	TacticalMap->Unselect_All();
	AllowVoice = true;
	for (int index = 0; index < Technos.Count(); index++) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && !techno->IsInLimbo && techno->Group == group - 1 && Is_Player_Side(techno) && !techno->IsSelected) {
			techno->Select();
			techno->RadarFlashTimer = Rule->RadarCombatFlashTime;
			AllowVoice = false;
		}
	}
	AllowVoice = true;
	TechnoClass::Reset_Action_Line_Timer();
}

/// <summary>
/// Selects the control group on top of the selection and centers the view on it.
/// </summary>
void __fastcall Group_Reselect(int group)
{
	Cancel_Click_Modes();
	if (CurrentObject.Count() != 0) {
		ObjectClass * object = CurrentObject[0];
		if (!object->Is_Foot() || ((FootClass *)CurrentObject[0])->Group != group - 1) {
			TacticalMap->Unselect_All();
		}
	}
	for (int index = 0; index < Technos.Count(); index++) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && !techno->IsInLimbo && techno->Group == group - 1 && Is_Player_Side(techno) && !techno->IsSelected) {
			techno->Select();
			AllowVoice = false;
		}
	}
	Map.Center_Map();
	Map.Flag_To_Redraw((GScreenRedrawFlags)1);
	AllowVoice = true;
}

/// <summary>
/// Adds the control group to the selection.
/// </summary>
void __fastcall Group_Select_Members(int group)
{
	Cancel_Click_Modes();
	AllowVoice = true;
	for (int index = 0; index < Technos.Count(); index++) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && !techno->IsInLimbo && techno->Group == group - 1 && Is_Player_Side(techno) && !techno->IsSelected) {
			techno->Select();
			AllowVoice = false;
		}
	}
	AllowVoice = true;
}

/// <summary>
/// Sends an alliance event for a house in a multiplayer game that allows changing alliances.
/// </summary>
void __fastcall Send_Ally_Event(unsigned long house)
{
	if (Session.Type != GAME_NORMAL && !Scen->Special.IsAllianceFixed && Rule->AlliesAllowed) {
		OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::ALLY, house));
	}
}

/// <summary>
/// Asks for an alliance with the owner of the selected object when that owner is another human player.
/// </summary>
void __cdecl Command_Ally(void)
{
	if (CurrentObject.Count() != 0 && !PlayerPtr->IsDefeated) {
		if (CurrentObject[0]->Owner_HouseClass() != PlayerPtr && CurrentObject[0]->Owner_HouseClass()->IsHuman) {
			unsigned long owner = CurrentObject[0]->Owner();
			if (Session.Type != GAME_NORMAL && !Scen->Special.IsAllianceFixed && Rule->AlliesAllowed) {
				OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::ALLY, owner));
			}
		}
	}
}


/// <summary>
/// Deletes the planned node the player has selected, or else the player's beacon.
/// </summary>
void __cdecl Command_Delete_Plan_Or_Beacon(void)
{
	PlanningNodeClass * node = Planning_Selected_Node();
	if (node != NULL) {
		node->Send_Delete_Event();
	} else {
		BeaconManager.Remove_Beacon(-1, -1);
	}
}


/// <summary>
/// Starts placing a beacon.
/// </summary>
void __cdecl Command_Beacon(void)
{
	if (MuteSWLaunches == 0) {
		Map.Place_Beacon_Mode_Control(-1);
	}
}


/// <summary>
/// Turns planning mode on.
/// </summary>
void __fastcall Command_Planning_Mode_On(bool sound)
{
	if (MuteSWLaunches == 0) {
		Planning_Begin();
		Planning_Show_Help(sound, true);
	}
}


/// <summary>
/// Turns planning mode off.
/// </summary>
void __fastcall Command_Planning_Mode_Off(bool sound)
{
	Planning_End();
	if (MuteSWLaunches == 0) {
		Planning_Show_Help(sound, false);
	}
}


/// <summary>
/// Whether planning mode is on.
/// </summary>
bool __fastcall Is_Planning_Mode_On(void)
{
	return Planning_Is_Active();
}

/// <summary>
/// Whether every selected object can attack while moving.
/// </summary>
bool __fastcall Selection_Can_Attack_Move(void)
{
	int count = CurrentObject.Count();
	for (int index = 0; index < count; index++) {
		ObjectClass * object = CurrentObject[index];
		if (object != NULL && object->Is_Techno() && !((TechnoClass *)object)->CanAttackOnTheMove()) {
			return false;
		}
	}
	return true;
}

bool AttackMoveMode;

/// <summary>
/// Makes the next order an attack move, unless a selected object cannot attack while moving.
/// </summary>
void __cdecl Command_Attack_Move(void)
{
	if (CurrentObject.Count() == 0) {
		Show_Player_Message(StringTable::LoadString("MSG:NothingSelected", NULL, "D:\\Ra2\\UICmnds.cpp", 745), -1);
	} else {
		int count = CurrentObject.Count();
		for (int index = 0; index < count; index++) {
			ObjectClass * object = CurrentObject[index];
			if (object != NULL && object->Is_Techno() && !((TechnoClass *)object)->CanAttackOnTheMove()) {
				Show_Player_Message(StringTable::LoadString("MSG:AttackMoveUnsupported", NULL, "D:\\Ra2\\UICmnds.cpp", 751), -1);
				return;
			}
		}
	}
	AttackMoveMode = true;
}

/// <summary>
/// Ends the attack move mode.
/// </summary>
void __cdecl Cancel_Attack_Move(void)
{
	AttackMoveMode = false;
}

/// <summary>
/// Whether an order is given as a forced attack move: the mode is on, or both the force-fire and force-move keys are held and every selected object can attack while moving.
/// </summary>
bool __fastcall Can_Force_Order(void)
{
	if (AttackMoveMode) {
		return true;
	}
	bool fire = Keyboard->Down(Options.KeyForceAttack1) || Keyboard->Down(Options.KeyForceAttack2);
	bool select = Keyboard->Down(Options.KeySelect1) || Keyboard->Down(Options.KeySelect2);
	if (!fire || !select) {
		return false;
	}
	int count = CurrentObject.Count();
	for (int index = 0; index < count; index++) {
		ObjectClass * object = CurrentObject[index];
		if (object != NULL && object->Is_Techno() && !((TechnoClass *)object)->CanAttackOnTheMove()) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// A cursor number, 71, that the display class sets at 0x0049B924.
/// </summary>
MouseType Cursor_Type_71(void)
{
	return MouseType(71);
}

/// <summary>
/// The area guard cursor.
/// </summary>
MouseType Cursor_Area_Guard(void)
{
	return MouseType(22);
}

/// <summary>
/// The first frame of cursor 77.
/// </summary>
int Cursor_Start_Frame_77(void)
{
	return Map.Get_Mouse_Start_Frame(MouseType(77), false);
}

/// <summary>
/// The first frame of cursor 76.
/// </summary>
int Cursor_Start_Frame_76(void)
{
	return Map.Get_Mouse_Start_Frame(MouseType(76), false);
}

/// <summary>
/// Ends the attack move mode.
/// </summary>
void __cdecl Clear_Attack_Move_Flag(void)
{
	AttackMoveMode = false;
}

int TypeSelectMode;

/// <summary>
/// Sets how the next selection by type works and ends the attack move mode.
/// </summary>
void __fastcall Set_Type_Select_Mode(unsigned mode)
{
	TypeSelectMode = mode;
	AttackMoveMode = false;
}

bool SelectAcrossMap;

/// <summary>
/// Puts the message that says what the last selection by type or state covered in the buffer.
/// </summary>
void __fastcall Get_Selection_Scope_Text(wchar_t * buffer)
{
	if (CurrentObject.Count() == 0) {
		wcscpy(buffer, StringTable::LoadString("MSG:NothingSelected", NULL, "D:\\Ra2\\UICmnds.cpp", 898));
		return;
	}
	if (SelectAcrossMap) {
		wcscpy(buffer, StringTable::LoadString("MSG:SelAcrossMap", NULL, "D:\\Ra2\\UICmnds.cpp", 900));
		return;
	}
	wcscpy(buffer, StringTable::LoadString("MSG:SelAcrossScreen", NULL, "D:\\Ra2\\UICmnds.cpp", 902));
}

/// <summary>
/// Puts the message that says how many objects of a kind are selected and what they are worth in the buffer.
/// </summary>
void __fastcall Get_Selection_Info_Text(wchar_t * buffer, wchar_t const * name)
{
	if (TypeSelectList->Count() == 0 || name == NULL) {
		wcscpy(buffer, StringTable::LoadString("MSG:NavEmpty", NULL, "D:\\Ra2\\UICmnds.cpp", 911));
		return;
	}
	wchar_t upper[100];
	wcscpy(upper, name);
	_wcsupr(upper);
	int count = CurrentObject.Count();
	if (count == 0) {
		swprintf(buffer, StringTable::LoadString("MSG:NoUnitsSel", NULL, "D:\\Ra2\\UICmnds.cpp", 921), upper);
		return;
	}
	int total = 0;
	for (int index = 0; index < count; index++) {
		ObjectClass * object = CurrentObject[index];
		if (object != NULL && object->Is_Techno()) {
			TechnoClass * techno = (TechnoClass *)object;
			HouseClass * owner = techno->House;
			total += techno->Class_Of()->Cost_Of(owner);
		}
	}
	swprintf(buffer, StringTable::LoadString("MSG:UnitsWorth", NULL, "D:\\Ra2\\UICmnds.cpp", 941), count, upper, total);
}

/// <summary>
/// Collects every object of the game that passes the test.
/// </summary>
TechnoList * __fastcall Collect_Technos(TechnoTest test, int argument)
{
	TechnoList * list = new TechnoList(0, NULL);
	int count = Technos.Count();
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = Technos[index];
		if (test(techno, argument)) {
			list->Add(techno);
		}
	}
	return list;
}

/// <summary>
/// Collects every object on the screen that passes the test.
/// </summary>
TechnoList * __fastcall Collect_Visible_Technos(TechnoTest test, int argument)
{
	TechnoList * list = new TechnoList(0, NULL);
	int count = TacticalMap->Selectable_Count();
	for (int index = 0; index < count; index++) {
		ObjectClass * object = TacticalMap->Selectable_Object(index);
		if (object->Is_Techno()) {
			TechnoClass * techno = (TechnoClass *)object;
			if (test(techno, argument)) {
				list->Add(techno);
			}
		}
	}
	return list;
}

/// <summary>
/// Collects the selected objects that are technos.
/// </summary>
TechnoList * Collect_Selected_Technos(void)
{
	TechnoList * list = new TechnoList(0, NULL);
	int count = CurrentObject.Count();
	for (int index = 0; index < count; index++) {
		ObjectClass * object = CurrentObject[index];
		if (object != NULL && object->Is_Techno()) {
			list->Add((TechnoClass *)object);
		}
	}
	return list;
}

/// <summary>
/// Applies the action to every object of the list.
/// </summary>
void __fastcall For_Each_Techno(TechnoList * list, TechnoAction action, int argument)
{
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		action((*list)[index], argument);
	}
}

/// <summary>
/// Applies the action to every object of the list that passes the test.
/// </summary>
void __fastcall For_Each_Techno_If(TechnoList * list, TechnoTest test, TechnoAction action, int test_argument, int argument)
{
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = (*list)[index];
		if (test(techno, test_argument)) {
			action(techno, argument);
		}
	}
}

/// <summary>
/// Whether every object of the list passes the test.
/// </summary>
bool __fastcall All_Technos_Pass(TechnoList * list, TechnoTest test, int argument)
{
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		if (!test((*list)[index], argument)) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// Whether any object of the list passes the test.
/// </summary>
bool __fastcall Any_Techno_Passes(TechnoList * list, TechnoTest test, int argument)
{
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		if (test((*list)[index], argument)) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// Finds a name in a list of names; -1 when it is not there.
/// </summary>
int __fastcall Find_Name_Index(DynamicVectorClass<char const *> const & names, char const * name)
{
	for (int index = 0; index < names.Count(); index++) {
		if (strcmp(name, names[index]) == 0) {
			return index;
		}
	}
	return -1;
}

int __fastcall Is_Owned_Active(TechnoClass * techno, int argument);

/// <summary>
/// Whether the object is a combatant the select-all command takes: not a building, and of a type marked selectable.
/// </summary>
inline int __fastcall Is_Selectable_Combatant(TechnoClass * techno, int argument) // RA2: no copy in the executable (expanded at its one use); Yuri's Revenge keeps it out of line at 0x7325C0 (ra2-decomp's name)
{
	if (techno->Fetch_RTTI() != RTTI_BUILDING && techno->TClass->IsSelectableCombatant) {
		return true;
	}
	return false;
}

/// <summary>
/// Whether the object is such a combatant and not yet selected.
/// </summary>
inline int __fastcall Is_Unselected_Combatant(TechnoClass * techno, int argument) // RA2: no copy in the executable
{
	if (techno->Fetch_RTTI() != RTTI_BUILDING && techno->TClass->IsSelectableCombatant && !techno->IsSelected) {
		return true;
	}
	return false;
}

/// <summary>
/// Selects the object.
/// </summary>
inline void __fastcall Select_Techno(TechnoClass * techno, int argument) // RA2: no copy in the executable
{
	techno->Select();
}

/// <summary>
/// Selects the player's combatants on the screen, or all over the map when none on the screen is left to select.
/// </summary>
void __fastcall Select_All_Combatants(bool clear)
{
	if (MuteSWLaunches != 0) {
		return;
	}
	TacticalMap->Unselect_Foreign_Object();
	if (TypeSelectMode != 1) {
		SelectAcrossMap = false;
	}
	TechnoTest test = (TechnoTest)Is_Owned_Active;
	TechnoList * list;
	if (SelectAcrossMap) {
		list = Collect_Technos(test, 0);
	} else {
		list = Collect_Visible_Technos(test, 0);
		if (!Any_Techno_Passes(list, (TechnoTest)Is_Unselected_Combatant, 0)) {
			SelectAcrossMap = true;
		}
		if (SelectAcrossMap) {
			delete list;
			list = Collect_Technos(test, 0);
		}
	}
	if (clear) {
		TacticalMap->Unselect_All();
	}
	For_Each_Techno_If(list, (TechnoTest)Is_Selectable_Combatant, Select_Techno, 0, 0);
	wchar_t buffer[256];
	Get_Selection_Scope_Text(buffer);
	Show_Player_Message(buffer, -1);
	delete list;
	TypeSelectMode = 1;
	AttackMoveMode = false;
}


/// <summary>
/// Whether the object is active and belongs to the player's side.
/// </summary>
int __fastcall Is_Owned_Active(TechnoClass * techno, int argument)
{
	if (techno != NULL && techno->IsActive && Is_Player_Side(techno)) {
		return 1;
	}
	return 0;
}

int __fastcall Is_Player_Alive_Unit_Of(TechnoClass * techno, char const * name);

/// <summary>
/// Takes every object of a kind out of the selection: those on the screen, or on the whole map after a selection by type reached over it.
/// </summary>
void __fastcall Unselect_Type(char const * name)
{
	TechnoTest test = (TechnoTest)Is_Player_Alive_Unit_Of;
	TechnoList * list;
	if (SelectAcrossMap) {
		list = Collect_Technos(test, (int)name);
	} else {
		list = Collect_Visible_Technos(test, (int)name);
	}
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		(*list)[index]->Unselect();
	}
	delete list;
}

/// <summary>
/// Whether the object is active, the player's, and of the kind of the name.
/// </summary>
int __fastcall Is_Player_Alive_Unit_Of(TechnoClass * techno, char const * name)
{
	if (techno != NULL && techno->IsActive && Is_Player_Side(techno) && techno->Is_Type_Named(name)) {
		return 1;
	}
	return 0;
}

/// <summary>
/// Selects every object of a kind: those on the screen, or on the whole map after a selection by type reached over it.
/// </summary>
void __fastcall Select_Type(char const * name)
{
	bool voice = AllowVoice;
	AllowVoice = false;
	TechnoTest test = (TechnoTest)Is_Player_Alive_Unit_Of;
	TechnoList * list = SelectAcrossMap ? Collect_Technos(test, (int)name) : Collect_Visible_Technos(test, (int)name);
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		(*list)[index]->Select();
	}
	delete list;
	AllowVoice = voice;
}

/// <summary>
/// Whether the object is of any of the kinds in the list.
/// </summary>
bool __fastcall Matches_Any_Name(TechnoClass * techno, DynamicVectorClass<char const *> const & names)
{
	int count = names.Count();
	for (int index = 0; index < count; index++) {
		char const * name = names[index];
		if (techno->Is_Type_Named(name)) {
			return true;
		}
	}
	return false;
}


bool TypeSelectKeyHeld;
unsigned long TypeSelectKeyTime;

/// <summary>
/// The type select key went down: remembers the moment.
/// </summary>
void Type_Select_Key_Down(void)
{
	if (!TypeSelectKeyHeld) {
		TypeSelectKeyHeld = true;
		TypeSelectKeyTime = timeGetTime();
	}
}

/// <summary>
/// Releases the type select key; a quick tap selects the types and redraws the tactical map.
/// </summary>
void __fastcall Type_Select_Key_Up(void)
{
	if (TypeSelectKeyHeld) {
		TypeSelectKeyHeld = false;
		if (abs((int)(timeGetTime() - TypeSelectKeyTime)) <= 500) {
			Command_Type_Select();
			Map.Flag_To_Redraw(GS_REDRAW_TACTICAL);
		}
	}
}

/// <summary>
/// Whether the type select key is down.
/// </summary>
bool Is_Type_Select_Key_Held(void)
{
	return TypeSelectKeyHeld;
}

/// <summary>
/// Collects the type names of the selected objects, each once.
/// </summary>
DynamicVectorClass<char const *> * Collect_Selected_Type_Names(void)
{
	DynamicVectorClass<char const *> * names = new DynamicVectorClass<char const *>(0, NULL);
	int count = CurrentObject.Count();
	for (int index = 0; index < count; index++) {
		char const * name = CurrentObject[index]->Class_Of()->Name();
		if (Find_Name_Index(*names, name) == -1) {
			names->Add(name);
		}
	}
	return names;
}


/// <summary>
/// Applies the action to every object of a kind: those on the screen, or on the whole map after a selection by type reached over it.
/// </summary>
void __fastcall For_Each_Of_Type(char const * name, TechnoAction action, int argument)
{
	TechnoTest test = (TechnoTest)Is_Player_Alive_Unit_Of;
	TechnoList * list;
	if (SelectAcrossMap) {
		list = Collect_Technos(test, (int)name);
	} else {
		list = Collect_Visible_Technos(test, (int)name);
	}
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		action((*list)[index], argument);
	}
	delete list;
}

/// <summary>
/// Takes an object that is going away out of the list a type, state or rank selection works on.
/// </summary>
void __fastcall Detach_From_Command_List(AbstractClass * object)
{
	if (TypeSelectList != NULL) {
		for (int index = TypeSelectList->Count() - 1; index >= 0; index--) {
			if ((*TypeSelectList)[index] == object) {
				TypeSelectList->Delete_Index(index);
				return;
			}
		}
	}
}

int __fastcall Is_Selectable_Player_Techno(TechnoClass * techno, int argument);

/// <summary>
/// Makes the list a state or rank selection works on: the selected objects, or when none is a techno, the player's objects on the screen that are not buildings.
/// </summary>
void __cdecl Refresh_Type_Select_List(void)
{
	if (TypeSelectList != NULL) {
		delete TypeSelectList;
		TypeSelectList = NULL;
	}
	TypeSelectList = Collect_Selected_Technos();
	if (TypeSelectList->Count() == 0) {
		delete TypeSelectList;
		TypeSelectList = NULL;
		TypeSelectList = Collect_Visible_Technos((TechnoTest)Is_Selectable_Player_Techno, 0);
	}
}



/// <summary>
/// Whether the object is active, the player's, and not a building.
/// </summary>
int __fastcall Is_Selectable_Player_Techno(TechnoClass * techno, int argument)
{
	if (techno != NULL && techno->IsActive && Is_Player_Side(techno) && techno->Fetch_RTTI() != RTTI_BUILDING) {
		return 1;
	}
	return 0;
}

/// <summary>
/// The message that says which health state a selection by health picked.
/// </summary>
wchar_t const * __fastcall Health_Message(int kind)
{
	if (kind == 0) {
		return StringTable::LoadString("MSG:Critical", NULL, "D:\\Ra2\\UICmnds.cpp", 1454);
	}
	if (kind == 1) {
		return StringTable::LoadString("MSG:HeavilyDamaged", NULL, "D:\\Ra2\\UICmnds.cpp", 1456);
	}
	if (kind == 2) {
		return StringTable::LoadString("MSG:Healthy", NULL, "D:\\Ra2\\UICmnds.cpp", 1458);
	}
	return L"";
}

/// <summary>
/// The health state the next selection by health picks: the one after the last, -1 stands for none yet.
/// </summary>
int __fastcall Next_Health_Kind(int last)
{
	if (last == -1) {
		return 0;
	}
	if (last == 0) {
		return 1;
	}
	return last == 1 ? 2 : 0;
}


/// <summary>
/// The message that says which rank a selection by rank picked.
/// </summary>
wchar_t const * __fastcall Rank_Message(int kind)
{
	if (kind == 0) {
		return StringTable::LoadString("MSG:Elite", NULL, "D:\\Ra2\\UICmnds.cpp", 1528);
	}
	if (kind == 1) {
		return StringTable::LoadString("MSG:Veteran", NULL, "D:\\Ra2\\UICmnds.cpp", 1530);
	}
	if (kind == 2) {
		return StringTable::LoadString("MSG:LittleExperience", NULL, "D:\\Ra2\\UICmnds.cpp", 1532);
	}
	return L"";
}

/// <summary>
/// The rank the next selection by rank picks: the one after the last, -1 stands for none yet.
/// </summary>
int __fastcall Next_Rank_Kind(int last)
{
	if (last == -1) {
		return 0;
	}
	if (last == 0) {
		return 1;
	}
	return last == 1 ? 2 : 0;
}


// RA2: remembers the last health state and rank selected; -1 means no selection yet.
int LastHealthKind = -1;
int LastRankKind = -1;

/// <summary>
/// Selects the type-select list's objects in the next health state (the first, when the list is new), and reports
/// the state, or "mixed" when the current selection holds objects in other states.
/// </summary>
void __fastcall Select_By_Health(bool flag)
{
	wchar_t buffer[256];
	if (MuteSWLaunches != 0) {
		return;
	}
	int kind;
	if (TypeSelectMode != 3) {
		Refresh_Type_Select_List();
		TacticalMap->Unselect_All();
		kind = 0;
	} else {
		kind = Next_Health_Kind(LastHealthKind);
		if (flag) {
			TacticalMap->Unselect_All();
		}
	}
	TechnoList * list = TypeSelectList;
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = (*list)[index];
		if (techno->Health_Status() == kind) {
			techno->Select();
		}
	}
	wchar_t const * text = Health_Message(kind);
	SmartPtr<TechnoList> objects(Collect_Selected_Technos());
	int objects_count = objects->Count();
	for (int check = 0; check < objects_count; check++) {
		TechnoClass * item = (*objects)[check];
		if (item->Health_Status() != kind) {
			text = StringTable::LoadString("MSG:Mixed", NULL, "D:\\Ra2\\UICmnds.cpp", 0x599);
			break;
		}
	}
	delete objects;
	Get_Selection_Info_Text(buffer, text);
	Show_Player_Message(buffer, -1);
	LastHealthKind = kind;
	int commands = TypeSelectList->Count();
	TypeSelectMode = 0;
	if (commands != 0) {
		TypeSelectMode = 3;
	}
	AttackMoveMode = false;
}

/// <summary>
/// Selects the type-select list's objects of the next rank (the first, when the list is new), and reports the
/// rank, or "mixed" when the current selection holds objects of other ranks.
/// </summary>
void __fastcall Select_By_Rank(bool flag)
{
	wchar_t buffer[256];
	if (MuteSWLaunches != 0) {
		return;
	}
	int kind;
	if (TypeSelectMode != 4) {
		Refresh_Type_Select_List();
		TacticalMap->Unselect_All();
		kind = 0;
	} else {
		kind = Next_Rank_Kind(LastRankKind);
		if (flag) {
			TacticalMap->Unselect_All();
		}
	}
	TechnoList * list = TypeSelectList;
	int count = list->Count();
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = (*list)[index];
		if (techno->Veterancy.Remaining_Levels() == kind) {
			techno->Select();
		}
	}
	wchar_t const * text = Rank_Message(kind);
	SmartPtr<TechnoList> objects(Collect_Selected_Technos());
	int objects_count = objects->Count();
	for (int check = 0; check < objects_count; check++) {
		TechnoClass * item = (*objects)[check];
		if (item->Veterancy.Remaining_Levels() != kind) {
			text = StringTable::LoadString("MSG:Mixed", NULL, "D:\\Ra2\\UICmnds.cpp", 0x5e4);
			break;
		}
	}
	delete objects;
	Get_Selection_Info_Text(buffer, text);
	Show_Player_Message(buffer, -1);
	LastRankKind = kind;
	int commands = TypeSelectList->Count();
	TypeSelectMode = 0;
	if (commands != 0) {
		TypeSelectMode = 4;
	}
	AttackMoveMode = false;
}

/// <summary>
/// Collects the type names of a list of objects, each once.
/// </summary>
DynamicVectorClass<char const *> * __fastcall Collect_Type_Names(TechnoList const & objects)
{
	DynamicVectorClass<char const *> * names = new DynamicVectorClass<char const *>(0, NULL);
	int count = objects.Count();
	for (int index = 0; index < count; index++) {
		char const * name = objects[index]->Class_Of()->Name();
		if (Find_Name_Index(*names, name) == -1) {
			names->Add(name);
		}
	}
	return names;
}


/// <summary>
/// Picks the kind of the list's objects that is worth the most but still less than the limit, or the one worth the most; returns its name in a new buffer.
/// </summary>
char * __fastcall Pick_Type_By_Cost(TechnoList const & objects, char const * name, int limit, int * best)
{
	DynamicVectorClass<char const *> * names = Collect_Type_Names(objects);
	DynamicVectorClass<int> * costs = new DynamicVectorClass<int>(0, NULL);
	int count = names->Count();
	for (int index = 0; index < count; index++) {
		costs->Add(0);
	}
	int total = objects.Count();
	for (int item = 0; item < total; item++) {
		TechnoClass * object = objects[item];
		char const * id = object->Class_Of()->Name();
		int found = Find_Name_Index(*names, id);
		HouseClass * house = object->House;
		int sum = (*costs)[found];
		int cost = ((TechnoTypeClass const *)object->Class_Of())->Cost_Of(house);
		(*costs)[found] = sum + cost;
	}
	*best = 0;
	int pick = -1;
	if (name != NULL) {
		for (int entry = 0; entry < count; entry++) {
			if (strcmp(name, (*names)[entry]) != 0) {
				int cost = (*costs)[entry];
				if (cost < limit && cost > *best) {
					*best = cost;
					pick = entry;
				}
			}
		}
	}
	if (name == NULL || *best == 0) {
		int most = 0;
		int which = -1;
		for (int slot = 0; slot < count; slot++) {
			if ((*costs)[slot] > most) {
				most = (*costs)[slot];
				which = slot;
			}
		}
		*best = most;
		pick = which;
	}
	char const * selected = name;
	if (pick != -1) {
		selected = (*names)[pick];
	}
	char * result = new char[100];
	if (selected != NULL) {
		strcpy(result, selected);
	}
	delete names;
	delete costs;
	return result;
}


/// <summary>
/// Forgets the control group and the radar event the last selection came from.
/// </summary>
void Reset_Group_Select(void)
{
	GroupSelectGroup = -1;
	GroupSelectFrame = -100;
	RadarJumpFrame = -100;
}

TechnoClass * SidebarStructureTabObject;
TechnoClass * SidebarDefenseTabObject;

/// <summary>
/// Makes a building the one the structure tab or the defense tab shows, by the building type's build category.
/// </summary>
void __fastcall Set_Tab_Building(BuildingClass * building)
{
	if (building->Class->BuildCat == 5) {
		SidebarDefenseTabObject = building;
	} else {
		SidebarStructureTabObject = building;
	}
}

/// <summary>
/// Forgets an object in the two sidebar tab slots; all of them for NULL.
/// </summary>
void __fastcall Clear_Sidebar_Tab_Object(TechnoClass * techno)
{
	if (techno == NULL) {
		SidebarStructureTabObject = NULL;
		SidebarDefenseTabObject = NULL;
		return;
	}
	if (techno == SidebarStructureTabObject) {
		SidebarStructureTabObject = NULL;
		return;
	}
	if (techno == SidebarDefenseTabObject) {
		SidebarDefenseTabObject = NULL;
	}
}

/// <summary>
/// The building the structure tab shows.
/// </summary>
TechnoClass * Get_Structure_Tab_Object(void)
{
	return SidebarStructureTabObject;
}

/// <summary>
/// The building the defense tab shows.
/// </summary>
TechnoClass * Get_Defense_Tab_Object(void)
{
	return SidebarDefenseTabObject;
}

/// <summary>
/// Centers the view on the newest radar event, or the previous event after a repeated press.
/// </summary>
void Jump_To_Radar_Event(void)
{
	Cell cell;
	if (Frame - RadarJumpFrame < 50) {
		cell = Recall_Previous_Radar_Event_Cell();
	} else {
		cell = Recall_Radar_Event_Cell();
	}
	RadarJumpFrame = Frame;
	if (cell.X != CELL_NONE.X || cell.Y != CELL_NONE.Y) {
		Coord coord(cell.X * CELL_LEPTON_W + CELL_LEPTON_W / 2, cell.Y * CELL_LEPTON_H + CELL_LEPTON_H / 2, 0);
		int height = Map[coord].Height * CELL_LEPTON_H / 2;
		coord -= Coord(height, height, 0);
		TacticalMap->Set_Tactical_Position(coord);
		if (Map.PendingObject != 0) {
			Map.Set_Cursor_Pos(CELL_NONE);
		}
	}
}
