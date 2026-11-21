/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

/*
**	RA2: the skirmish setup dialog (Skirmish.cpp in the executable's debug text, resource 0x102): seven
**	computer player rows with a type, side and color box each, the game options, the map list and the preview.
**	TibSun's skirmish.cpp of the same name has the same entry point and dialog procedure, but its dialog is
**	another one (sliders for the number of computer players); the code here is the executable's from 0x00682600.
*/

#include "always.h"
#include "rect.h"
#include "geometry.h"
#include <string.h>
#include <wchar.h>
#include "_rules.h"
#include "goptions.h"
#include "init.h"
#include "mplayer.h"
#include "msgbox.h"
#include "netshare.h"
#include "multiplayer.h"
#include "ownrdraw.h"
#include "rules.h"
#include "session.h"
#include "houstype.h"
#undef LoadString
#include "stringtable.h"
#include "skirmish.h"
#include "gdlgsupp.h"


#include "geometry.h"



HWND SkirmishDialog;

int __fastcall Skirmish_Player_Combo_ID(int index);
int __fastcall Skirmish_Player_Index(int id);
wchar_t const * __fastcall Skirmish_Player_Type_Name(int type);
bool __fastcall Skirmish_Has_Computer_After(int player);
void __fastcall Skirmish_On_WM_COMMAND(HWND dialog, int id, HWND control, int code);
BOOL __fastcall Skirmish_Is_Computer(HWND dialog, int id);
void __fastcall Skirmish_Show_Players(HWND dialog, int first, int count);
void __fastcall Skirmish_Hide_Players(HWND dialog, int first);
int __stdcall Skirmish_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int __fastcall Skirmish_On_WM_INITDIALOG(HWND dialog, WPARAM wparam, LPARAM lparam);


void __fastcall Set_Item_Text(HWND dialog, MultiplayerGameMode * mode);
void __fastcall Set_Skirmish_Dialog_Text(HWND dialog);
MultiplayerGameMode * Get_Current_Game_Mode(void);
void __fastcall Darken_Surface(Surface * surface, bool update);
namespace OwnerDraw { bool __fastcall WS_Save_Dialog_Values(HWND dialog); bool __fastcall Is_Control_Disabled(HWND control); void __fastcall WideToCharString(char * dest, wchar_t const * src, int count); }
wchar_t * __fastcall Narrow_To_Wide(char const * narrow);
MultiplayerGameMode * First_Game_Mode(void);

/// <summary>
/// The id of the player type combo box of a computer player.
/// </summary>
int __fastcall Skirmish_Player_Combo_ID(int index)
{
	if (index == 0) {
		return(0x50B);
	}
	if (index == 1) {
		return(0x50E);
	}
	if (index == 2) {
		return(0x516);
	}
	if (index == 3) {
		return(0x51A);
	}
	if (index == 4) {
		return(0x51B);
	}
	if (index == 5) {
		return(0x51C);
	}
	if (index == 6) {
		return(0x51D);
	}
	return(-1);
}

/// <summary>
/// The index of the computer player a player type combo box belongs to.
/// </summary>
int __fastcall Skirmish_Player_Index(int id)
{
	if (id == 0x50B) {
		return(0);
	}
	if (id == 0x50E) {
		return(1);
	}
	if (id == 0x516) {
		return(2);
	}
	if (id == 0x51A) {
		return(3);
	}
	if (id == 0x51B) {
		return(4);
	}
	if (id == 0x51C) {
		return(5);
	}
	if (id == 0x51D) {
		return(6);
	}
	return(-1);
}

/// <summary>
/// The text of a player type.
/// </summary>
wchar_t const * __fastcall Skirmish_Player_Type_Name(int type)
{
	if (type == -1) {
		return(StringTable::LoadString("STT:PlayerNone", NULL, "D:\\Ra2\\Skirmish.cpp", 0x85));
	}
	if (type == 2) {
		return(StringTable::LoadString("STT:PlayerDumbAI", NULL, "D:\\Ra2\\Skirmish.cpp", 0x87));
	}
	if (type == 1) {
		return(StringTable::LoadString("STT:PlayerSmartAI", NULL, "D:\\Ra2\\Skirmish.cpp", 0x89));
	}
	if (type == 0) {
		return(StringTable::LoadString("STT:PlayerGeniusAI", NULL, "D:\\Ra2\\Skirmish.cpp", 0x8B));
	}
	return(NULL);
}

/// <summary>
/// Whether a player from this one on is a computer player.
/// </summary>
bool __fastcall Skirmish_Has_Computer_After(int player)
{
	HWND dialog = SkirmishDialog;
	for (int index = player; index < 8; index++) {
		if (Skirmish_Is_Computer(dialog, Skirmish_Player_Combo_ID(index - 1))) {
			return(true);
		}
	}
	return(false);
}

/// <summary>
/// The commands of the skirmish dialog.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 2819 bytes differ (draft structure)
void __fastcall Skirmish_On_WM_COMMAND(HWND dialog, int id, HWND control, int code)
{
	int * result = (int *)GetWindowLong(dialog, DWL_USER);
	switch (id) {
		case 0x50B:
		case 0x50E:
		case 0x516:
		case 0x51A:
		case 0x51B:
		case 0x51C:
		case 0x51D:
		{
			int row = Skirmish_Player_Index(id) + 1;
			int row_color = Player_Color_Combo_ID(row);
			int row_side = Player_Side_Combo_ID(row);
			BOOL enable = Skirmish_Is_Computer(dialog, id);
			if (!enable) {
				Select_Color_Combo(dialog, row_color, -2);
				Select_Side_Combo(dialog, row_side, -2);
			}
			HWND color_box = GetDlgItem(dialog, row_color);
			HWND side_box = GetDlgItem(dialog, row_side);
			EnableWindow(color_box, enable);
			EnableWindow(side_box, enable);
		}
		return;
		case 0x5AA: {
			int old_mode = Session.Options.MPModeIndex;
			int old_scenario = Session.Options.ScenarioIndex;
			char old_file[0x200];
			strcpy(old_file, Session.ScenarioFileName);
			wchar_t old_name[0x100];
			wcscpy(old_name, Session.Options.ScenarioDescription);
			OwnerDraw::WS_Save_Dialog_Values(dialog);
			ShowWindow(dialog, SW_HIDE);
		if (Scenario_Dialog(MainWindow) == IDCANCEL) {
				Session.Options.MPModeIndex = old_mode;
				Session.Options.ScenarioIndex = old_scenario;
				Set_Scenario_Info_From_Index(old_scenario);
				Update_Network_Dialog_Preview(dialog);
				ShowWindow(dialog, SW_SHOW);
				if (Session.Scenarios[Session.Options.ScenarioIndex]->Is_Random_Map()) {
				if (MultiplayerMapPreview != NULL) {
					delete MultiplayerMapPreview;
				}
				MultiplayerMapPreview = new MapPreviewClass;
				MultiplayerMapPreview->Read_PCX_Preview("RandMap.img");
				if (MultiplayerMapPreview->Get_Preview_Surface() == NULL) {
						Update_Network_Dialog_Preview(dialog);
				}
					InvalidateRect(dialog, NULL, FALSE);
			} else {
					Update_Network_Dialog_Preview(dialog);
			}
				InvalidateRect(dialog, NULL, FALSE);
			return;
		}
			int scenario = Session.Options.ScenarioIndex;
			int old_players;
			if (old_scenario == -1) {
				old_players = 8;
		} else {
				old_players = RandomMapWaypointCount(old_scenario);
		}
			int players = RandomMapWaypointCount(scenario);
			int extra = players - old_players;
			if (old_scenario == scenario) {
				Skirmish_Hide_Players(dialog, 1);
				Skirmish_Show_Players(dialog, 1, players - 1);
		}
			if (extra > 0) {
				Skirmish_Show_Players(dialog, old_players, extra);
			} else if (extra < 0) {
				Skirmish_Hide_Players(dialog, players);
		}
			ShowWindow(dialog, SW_SHOW);
			if (Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex)) {
				Set_Item_Text(dialog, MultiplayerGameMode::Get(Session.Options.MPModeIndex));
				Set_Skirmish_Dialog_Text(dialog);
				if (Session.Scenarios[Session.Options.ScenarioIndex]->Is_Random_Map()) {
				if (MultiplayerMapPreview != NULL) {
					delete MultiplayerMapPreview;
					MultiplayerMapPreview = new MapPreviewClass;
					MultiplayerMapPreview->Read_PCX_Preview("RandMap.img");
				}
				if (MultiplayerMapPreview->Get_Preview_Surface() == NULL) {
						Update_Network_Dialog_Preview(dialog);
				}
					InvalidateRect(dialog, NULL, FALSE);
			} else {
					Update_Network_Dialog_Preview(dialog);
			}
		} else {
				Session.Options.ScenarioIndex = old_scenario;
				Session.Options.MPModeIndex = old_mode;
		}
		return;
	}
		case 0x5C0:
		case 0x617: {
			if (code != 0) {
			break;
		}
			if (id == 0x617) {
				EnableWindow(GetDlgItem(dialog, 0x617), FALSE);
		}
			int players = RandomMapWaypointCount(Session.Options.ScenarioIndex);
			int computers = 0;
			int index;
			for (index = 0; index < 7; index++) {
				if (Skirmish_Is_Computer(dialog, Skirmish_Player_Combo_ID(index))) {
					computers++;
			}
		}
			int total = computers + 1;
			Session.Options.AIPlayers = computers;
			if (id == 0x617) {
				wchar_t text[0x100];
				if (players < total) {
					swprintf(text, StringTable::LoadString("TXT_SCENARIO_TOO_SMALL", NULL, "D:\\Ra2\\Skirmish.cpp", 0x334), players);
					WWMessageBox(L" ").Process(text, StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\Skirmish.cpp", 0x335), NULL, NULL, false);
					EnableWindow(GetDlgItem(dialog, 0x617), TRUE);
				return;
			}
				if (total <= 1) {
					swprintf(text, StringTable::LoadString("TXT_NEED_AT_LEAST_TWO_PLAYERS", NULL, "D:\\Ra2\\Skirmish.cpp", 0x33C), players);
					WWMessageBox(L" ").Process(text, StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\Skirmish.cpp", 0x33D), NULL, NULL, false);
					EnableWindow(GetDlgItem(dialog, 0x617), TRUE);
				return;
			}
		}
			WideWstring message;
			if (!Session.MPGameMode->CheckStartConditions(&message)) {
				if (id == 0x617) {
					WWMessageBox(L" ").Process(message.get(), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\Skirmish.cpp", 0x34B), NULL, NULL, false);
					EnableWindow(GetDlgItem(dialog, 0x617), TRUE);
				return;
			}
				First_Game_Mode();
		}
			Session.Skirmish.MPGameMode = Session.Options.MPModeIndex;
			Session.Skirmish.ScenIndex = Session.Options.ScenarioIndex;
			if (Session.Options.ScenarioIndex >= Session.Scenarios.Count()) {
				Session.Skirmish.ScenIndex = 0;
		}
			wchar_t name[20];
			HWND edit = GetDlgItem(dialog, 0x6A0);
			SendMessage(edit, 0x4B3, 20, (LPARAM)name);
			OwnerDraw::WideToCharString(Session.Handle, name, -1);
			int local_side = Side_Combo_Data(dialog, 0x6A1, -1);
			Session.Set_Side(local_side, true);
			for (index = 0; index < 7; index++) {
				int player = index + 1;
				int combo = Skirmish_Player_Combo_ID(index);
				int side_id = Player_Side_Combo_ID(player);
				int color_id = Player_Color_Combo_ID(player);
				if (Skirmish_Is_Computer(dialog, combo)) {
					int difficulty = SendDlgItemMessage(dialog, combo, CB_GETCURSEL, 0, 0);
					Session.Options.AISlots.Difficulties[player] = SendDlgItemMessage(dialog, combo, CB_GETITEMDATA, difficulty, 0);
					Session.Options.AISlots.Countries[player] = Side_Combo_Data(dialog, side_id, -1);
					Session.Options.AISlots.Colors[player] = Color_Combo_Data(dialog, color_id, -1);
			}
		}
			for (int player = 1; player < 8; player++) {
				int combo = Skirmish_Player_Combo_ID(player - 1);
				int side_id = Player_Side_Combo_ID(player);
				int color_id = Player_Color_Combo_ID(player);
				int selected = SendDlgItemMessage(dialog, combo, CB_GETCURSEL, 0, 0);
				int type = SendDlgItemMessage(dialog, combo, CB_GETITEMDATA, selected, 0);
				int side = Side_Combo_Data(dialog, side_id, -1);
				int color = Color_Combo_Data(dialog, color_id, -1);
				int value;
				if (type == -1) {
					value = 1;
				} else if (type == 0) {
					value = 4;
				} else if (type == 1) {
					value = 5;
			} else {
					value = type == 2 ? 6 : 0;
			}
				Session.Skirmish.SlotData[player][0] = value;
				Session.Skirmish.SlotData[player][1] = side;
				Session.Skirmish.SlotData[player][2] = color;
		}
		Session.Set_Color(Color_Combo_Data(dialog, 0x6A2, -1), true);
			NodeNameType * node = new NodeNameType;
			if (node != NULL) {
				wcscpy(node->Name, Narrow_To_Wide(Session.Handle));
				node->Slot.Country = Session.House;
				node->Slot.ColorIdx = Session.PrefColor;
				node->Slot.Time = -1;
				Session.Players.Add(node);
		}
			Session.Assign_Random_Choices();
			HWND item = GetDlgItem(dialog, 0x529);
			if (item != NULL) {
				Session.Options.GameSpeed = 6 - SendMessage(item, TBM_GETPOS, 0, 0);
			Options.GameSpeed = Session.Options.GameSpeed;
		}
			item = GetDlgItem(dialog, 0x511);
			if (item != NULL) {
				Session.Options.Credits = SendMessage(item, TBM_GETPOS, 0, 0);
		}
			item = GetDlgItem(dialog, 0x50C);
			if (item != NULL) {
				Session.Options.UnitCount = SendMessage(item, TBM_GETPOS, 0, 0);
		}
			Session.Skirmish.GameSpeed = Session.Options.GameSpeed;
			Session.Skirmish.Credits = Session.Options.Credits;
			Session.Skirmish.UnitCount = Session.Options.UnitCount;
			item = GetDlgItem(dialog, 0x54E);
			if (item != NULL) {
				Session.Options.ShortGame = SendMessage(item, BM_GETCHECK, 0, 0) == 1;
		}
			item = GetDlgItem(dialog, 0x69A);
			if (item != NULL) {
				Session.Options.SWAllowed = SendMessage(item, BM_GETCHECK, 0, 0) == 1;
		}
			item = GetDlgItem(dialog, 0x69D);
			if (item != NULL) {
				Session.Options.BuildOffAlly = SendMessage(item, BM_GETCHECK, 0, 0) == 1;
		}
			item = GetDlgItem(dialog, 0x693);
			if (item != NULL) {
				Session.Options.MCVRedeploy = SendMessage(item, BM_GETCHECK, 0, 0) == 1;
		}
			item = GetDlgItem(dialog, 0x696);
			if (item != NULL) {
				Session.Options.Goodies = SendMessage(item, BM_GETCHECK, 0, 0) == 1;
		}
			Session.Skirmish.ShortGame = Session.Options.ShortGame;
			Session.Skirmish.SuperWeaponsAllowed = Session.Options.SWAllowed;
			Session.Skirmish.BuildOffAlly = Session.Options.BuildOffAlly;
			Session.Skirmish.MCVRepacks = Session.Options.MCVRedeploy;
			Session.Skirmish.CratesAppear = Session.Options.Goodies;
			Session.Options.FogOfWar = false;
			Session.Options.BridgeDestruction = true;
		Session.Options.HarvTruce = false;
			Session.Options.CrapEngineers = false;
		if (MultiplayerMapPreview != NULL) {
			delete MultiplayerMapPreview;
			MultiplayerMapPreview = NULL;
		}
		*result = id;
		return;
	}
		case 0x510:
		case 0x513:
		case 0x514:
		case 0x51E:
		case 0x51F:
		case 0x520:
		case 0x521:
		case 0x6A1:
			Show_Player_Flag(dialog, Side_Combo_Player(id));
		break;
		case 0x522:
		case 0x523:
		case 0x524:
		case 0x525:
		case 0x526:
		case 0x527:
		case 0x528:
		case 0x6A2:
			if (code == 1) {
				Color_Combo_Changed(dialog, id);
		}
		break;
	}
}
#endif


/// <summary>
/// Whether the player type combo box selects a computer player.
/// </summary>
BOOL __fastcall Skirmish_Is_Computer(HWND dialog, int id)
{
	int selection = SendDlgItemMessage(dialog, id, CB_GETCURSEL, 0, 0);
	int type = SendDlgItemMessage(dialog, id, CB_GETITEMDATA, selection, 0);
	if (type != 0 && type != 1 && type != 2) {
		return(FALSE);
	}
	return(TRUE);
}

/// <summary>
/// Shows the rows of some players.
/// </summary>
void __fastcall Skirmish_Show_Players(HWND dialog, int first, int count)
{
	for (int player = first; player < first + count; player++) {
		int combo = Skirmish_Player_Combo_ID(player - 1);
		int row_id = Player_Control_ID(player);
		int color_id = Player_Color_Combo_ID(player);
		int side_id = Player_Side_Combo_ID(player);
		HWND combo_window = GetDlgItem(dialog, combo);
		HWND row_window = GetDlgItem(dialog, row_id);
		HWND color_window = GetDlgItem(dialog, color_id);
		HWND side_window = GetDlgItem(dialog, side_id);
		ShowWindow(combo_window, SW_SHOW);
		ShowWindow(row_window, SW_SHOW);
		ShowWindow(color_window, SW_SHOW);
		ShowWindow(side_window, SW_SHOW);
		int row = Skirmish_Player_Index(combo) + 1;
		int row_color = Player_Color_Combo_ID(row);
		int row_side = Player_Side_Combo_ID(row);
		int selection = SendDlgItemMessage(dialog, combo, CB_GETCURSEL, 0, 0);
		int type = SendDlgItemMessage(dialog, combo, CB_GETITEMDATA, selection, 0);
		bool enable = type == 0 || type == 1 || type == 2;
		if (!enable) {
			Select_Color_Combo(dialog, row_color, -2);
			Select_Side_Combo(dialog, row_side, -2);
		}
		HWND color_box = GetDlgItem(dialog, row_color);
		HWND side_box = GetDlgItem(dialog, row_side);
		EnableWindow(color_box, enable);
		EnableWindow(side_box, enable);
	}
}


/// <summary>
/// Resets and hides the rows of the players from this one on.
/// </summary>
void __fastcall Skirmish_Hide_Players(HWND dialog, int first)
{
	int player;
	for (player = first; player < 8; player++) {
		int combo = Skirmish_Player_Combo_ID(player - 1);
		int count = SendDlgItemMessage(dialog, combo, CB_GETCOUNT, 0, 0);
		for (int item = 0; item < count; item++) {
			if (SendDlgItemMessage(dialog, combo, CB_GETITEMDATA, item, 0) == -1) {
				SendDlgItemMessage(dialog, combo, CB_SETCURSEL, item, 0);
				break;
			}
		}
		int row = Skirmish_Player_Index(combo) + 1;
		int row_color = Player_Color_Combo_ID(row);
		int row_side = Player_Side_Combo_ID(row);
		int selection = SendDlgItemMessage(dialog, combo, CB_GETCURSEL, 0, 0);
		int type = SendDlgItemMessage(dialog, combo, CB_GETITEMDATA, selection, 0);
		bool enable = type == 0 || type == 1 || type == 2;
		if (!enable) {
			Select_Color_Combo(dialog, row_color, -2);
			Select_Side_Combo(dialog, row_side, -2);
		}
		HWND color_box = GetDlgItem(dialog, row_color);
		HWND side_box = GetDlgItem(dialog, row_side);
		EnableWindow(color_box, enable);
		EnableWindow(side_box, enable);
	}
	for (player = first; player < 8; player++) {
		int combo = Skirmish_Player_Combo_ID(player - 1);
		int row_id = Player_Control_ID(player);
		int side_id = Player_Side_Combo_ID(player);
		int color_id = Player_Color_Combo_ID(player);
		HWND combo_window = GetDlgItem(dialog, combo);
		HWND row_window = GetDlgItem(dialog, row_id);
		HWND side_window = GetDlgItem(dialog, side_id);
		HWND color_window = GetDlgItem(dialog, color_id);
		ShowWindow(combo_window, SW_HIDE);
		ShowWindow(row_window, SW_HIDE);
		ShowWindow(side_window, SW_HIDE);
		ShowWindow(color_window, SW_HIDE);
	}
}


/// <summary>
/// Runs the skirmish dialog.
/// </summary>
bool Skirmish_Mode_Dialog(void)
{
	int result = -1;
	Rule->Do_HouseTypes(*RuleINI);
	Rule->Do_Sides(*RuleINI);
	for (int index = 0; index < HouseTypes.Count(); index++) {
		HouseTypes[index]->Read_INI(*RuleINI);
	}
	HWND dialog = OwnerDraw::Begin_Dialog(0x102, Skirmish_Dialog_Proc, 0);
	if (dialog != NULL) {
		SkirmishDialog = dialog;
		SetWindowLong(dialog, 8, (LONG)&result);
		OwnerDraw::Display_Dialog(dialog);
		while (result != 0x617 && result != 0x5C0 && OwnerDraw::Dialog_Message_Handler() != true) {
			Title_Screen_Restore(false);
		}
		OwnerDraw::End_Dialog(dialog);
		SkirmishDialog = NULL;
	}
	if (MultiplayerMapPreview != NULL) {
		delete MultiplayerMapPreview;
		MultiplayerMapPreview = NULL;
	}
	Session.Write_MultiPlayer_Settings();
	if (result == 0x5C0) {
		Darken_Surface(NULL, true);
	}
	if (result == 0x617) {
		return(true);
	}
	return(false);
}

/// <summary>
/// The dialog procedure of the skirmish dialog.
/// </summary>
int __stdcall Skirmish_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result != 0) {
		return(result);
	}
	switch (message) {
	case WM_PAINT:
		if (MultiplayerMapPreview != NULL && !OwnerDraw::Is_Control_Disabled(GetDlgItem(window, 0x468))) {
			MultiplayerMapPreview->Blit_Preview(window);
		}
		ValidateRect(window, NULL);
		return(0);
	case WM_COMMAND:
		Skirmish_On_WM_COMMAND(window, wparam & 0xFFFF, (HWND)lparam, wparam >> 16);
		return(1);
	case 0x497:
		return(Skirmish_On_WM_INITDIALOG(window, wparam, lparam));
	case 0x4E9: {
		ToolTipTextRequest * request = (ToolTipTextRequest *)lparam;
		request->Text = NULL;
		if (request->Window == NULL || request->Item == -1) {
			return(1);
		}
		int id = GetDlgCtrlID(request->Window);
		if (Skirmish_Player_Index(id) == -1) {
			if (Side_Combo_Player(id) == -1) {
				if (Color_Combo_Player(id) == -1) {
					return(1);
				}
				request->Text = Color_Name(Color_Combo_Data(window, id, request->Item));
				return(1);
			}
			int side = Side_Combo_Data(window, id, request->Item);
			if (side == -1) {
				return(1);
			}
			request->Text = Side_Name(side);
			return(1);
		}
		int item = request->Item;
		if (item == -1) {
			item = SendDlgItemMessage(window, id, CB_GETCURSEL, 0, 0);
		}
		request->Text = Skirmish_Player_Type_Name(SendDlgItemMessage(window, id, CB_GETITEMDATA, item, 0));
		return(1);
	}
	}
	return(0);
}


/// <summary>
/// Fills the skirmish dialog from the saved settings.
/// </summary>
int __fastcall Skirmish_On_WM_INITDIALOG(HWND dialog, WPARAM wparam, LPARAM lparam)
{
	Session.Options.Reset_AI();
	HWND item = GetDlgItem(dialog, 0x6A0);
	if (item != NULL) {
		SendMessage(item, 0xC5, 19, 0);
		SendMessage(item, 0x4B2, 0, (LPARAM)Narrow_To_Wide(Session.Handle));
		SendMessage(item, 0x4D1, 1, 0);
	}
	for (int i = 0; i < 7; i++) {
		item = GetDlgItem(dialog, Skirmish_Player_Combo_ID(i));
		SendMessage(item, CB_RESETCONTENT, 0, 0);
		int index = SendMessage(item, 0x4C2, 0, (LPARAM)StringTable::LoadString("GUI:None", NULL, "D:\\Ra2\\Skirmish.cpp", 0x9C));
		SendMessage(item, CB_SETITEMDATA, index, -1);
		index = SendMessage(item, 0x4C2, 0, (LPARAM)StringTable::LoadString("GUI:AIEasy", NULL, "D:\\Ra2\\Skirmish.cpp", 0x9F));
		SendMessage(item, CB_SETITEMDATA, index, 2);
		index = SendMessage(item, 0x4C2, 0, (LPARAM)StringTable::LoadString("GUI:AINormal", NULL, "D:\\Ra2\\Skirmish.cpp", 0xA2));
		SendMessage(item, CB_SETITEMDATA, index, 1);
		index = SendMessage(item, 0x4C2, 0, (LPARAM)StringTable::LoadString("GUI:AIHard", NULL, "D:\\Ra2\\Skirmish.cpp", 0xA5));
		SendMessage(item, CB_SETITEMDATA, index, 0);
		SendMessage(item, CB_SETCURSEL, 0, 0);
	}
	Fill_Side_Combos(dialog);
	Init_Color_Entries();
	Fill_Color_Combos(dialog);
	Select_Player_Side(dialog, 0);
	Select_Player_Color(dialog, 0);
	for (int player = 1; player < 8; player++) {
		int combo = Skirmish_Player_Combo_ID(player - 1);
		int type = Session.Skirmish.SlotData[player][0];
		int selected;
		if (type == 1) {
			selected = -1;
		} else if (type == 4) {
			selected = 0;
		} else if (type == 5) {
			selected = 1;
		} else if (type == 6) {
			selected = 2;
		} else {
			selected = -1;
		}
		int count = SendDlgItemMessage(dialog, combo, CB_GETCOUNT, 0, 0);
		for (int row = 0; row < count; row++) {
			if (SendDlgItemMessage(dialog, combo, CB_GETITEMDATA, row, 0) == selected) {
				SendDlgItemMessage(dialog, combo, CB_SETCURSEL, row, 0);
				break;
			}
		}
		int side = Player_Side_Combo_ID(player);
		int color = Player_Color_Combo_ID(player);
		if (selected == -1) {
			Select_Side_Combo(dialog, side, -2);
			Select_Color_Combo(dialog, color, -2);
			HWND side_window = GetDlgItem(dialog, side);
			HWND color_window = GetDlgItem(dialog, color);
			EnableWindow(side_window, FALSE);
			EnableWindow(color_window, FALSE);
		} else {
			Select_Side_Combo(dialog, side, Session.Skirmish.SlotData[player][1]);
			Select_Color_Combo(dialog, color, Session.Skirmish.SlotData[player][2]);
		}
	}
	if (Session.Skirmish.MPGameMode < 0 || Session.Skirmish.MPGameMode >= MultiplayerGameMode::GetModeCount()) {
		Session.Skirmish.MPGameMode = First_Game_Mode()->MPModeIndex;
	}
	Session.Options.MPModeIndex = Session.Skirmish.MPGameMode;
	MultiplayerGameMode * mode = Get_Current_Game_Mode();
	Session.Skirmish.MPGameMode = mode->MPModeIndex;
	Session.Options.MPModeIndex = mode->MPModeIndex;
	if (Session.Skirmish.ScenIndex < 0 || Session.Skirmish.ScenIndex >= Session.Scenarios.Count()) {
		Session.Skirmish.ScenIndex = 0;
	}
	Session.Options.ScenarioIndex = Set_Scenario_Info_From_Index(Session.Skirmish.ScenIndex) ? Session.Skirmish.ScenIndex : 0;
	MultiMission * map = Session.Scenarios[Session.Options.ScenarioIndex];
	if (!mode->IsMapAllowed(map)) {
		int selection = mode->FirstValidMapIndex();
		if (selection == -1) {
			mode = First_Game_Mode();
			Session.Skirmish.MPGameMode = mode->MPModeIndex;
			Session.Options.MPModeIndex = mode->MPModeIndex;
			selection = mode->FirstValidMapIndex();
		}
		map = Session.Scenarios[selection];
		Session.Skirmish.ScenIndex = selection;
		Session.Options.ScenarioIndex = selection;
	}
	int scenario = Session.Options.ScenarioIndex;
	int players = RandomMapWaypointCount(scenario);
	if (scenario == -1) {
		Skirmish_Hide_Players(dialog, 1);
		Skirmish_Show_Players(dialog, 1, players - 1);
	}
	int extra = players - 8;
	if (extra > 0) {
		Skirmish_Show_Players(dialog, 8, extra);
	} else if (extra < 0) {
		Skirmish_Hide_Players(dialog, players);
	}
	Session.MPGameMode = mode;
	mode->vt_entry_20();
	Set_Item_Text(dialog, mode);
	Set_Skirmish_Dialog_Text(dialog);
	Clear_Vector(&Session.Players);
	Session.Options.GameSpeed = Session.Skirmish.GameSpeed;
	Session.Options.Credits = Session.Skirmish.Credits;
	Session.Options.UnitCount = Session.Skirmish.UnitCount;
	item = GetDlgItem(dialog, 0x529);
	if (item != NULL) {
		int speed = 6 - Session.Options.GameSpeed;
		SendMessage(item, TBM_SETRANGE, 1, 0x60000);
		SendMessage(item, TBM_SETPOS, 1, speed);
	}
	item = GetDlgItem(dialog, 0x511);
	if (item != NULL) {
		SendMessage(item, TBM_SETRANGE, 1, ((unsigned int)(unsigned short)Rule->MPMaxMoney << 16) | (unsigned short)Rule->MinMoney);
		SendMessage(item, TBM_SETPOS, 1, Session.Options.Credits);
		SendMessage(item, 0x4AB, 0, Rule->MoneyIncrement);
	}
	item = GetDlgItem(dialog, 0x50C);
	if (item != NULL) {
		SendMessage(item, TBM_SETRANGE, 1, ((unsigned int)(unsigned short)Rule->MaxUnitCount << 16) | (unsigned short)Rule->MinUnitCount);
		SendMessage(item, TBM_SETPOS, 1, Session.Options.UnitCount);
	}
	Session.Options.ShortGame = Session.Skirmish.ShortGame;
	Session.Options.SWAllowed = Session.Skirmish.SuperWeaponsAllowed;
	Session.Options.BuildOffAlly = Session.Skirmish.BuildOffAlly;
	Session.Options.MCVRedeploy = Session.Skirmish.MCVRepacks;
	Session.Options.Goodies = Session.Skirmish.CratesAppear;
	item = GetDlgItem(dialog, 0x54E);
	if (item != NULL) {
		SendMessage(item, BM_SETCHECK, Session.Options.ShortGame != false, 0);
	}
	item = GetDlgItem(dialog, 0x69A);
	if (item != NULL) {
		SendMessage(item, BM_SETCHECK, Session.Options.SWAllowed != false, 0);
	}
	item = GetDlgItem(dialog, 0x69D);
	if (item != NULL) {
		SendMessage(item, BM_SETCHECK, Session.Options.BuildOffAlly != false, 0);
	}
	item = GetDlgItem(dialog, 0x693);
	if (item != NULL) {
		SendMessage(item, BM_SETCHECK, Session.Options.MCVRedeploy != false, 0);
	}
	item = GetDlgItem(dialog, 0x696);
	if (item != NULL) {
		SendMessage(item, BM_SETCHECK, Session.Options.Goodies != false, 0);
	}
	if (map->Is_Random_Map()) {
		if (MultiplayerMapPreview != NULL) {
			delete MultiplayerMapPreview;
		}
		MultiplayerMapPreview = new MapPreviewClass;
		MultiplayerMapPreview->Read_PCX_Preview("RandMap.img");
		if (MultiplayerMapPreview->Get_Preview_Surface() == NULL) {
			Update_Network_Dialog_Preview(dialog);
		}
	} else {
		Update_Network_Dialog_Preview(dialog);
	}
	return(0);
}

