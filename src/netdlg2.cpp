/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "netdlg2.h"

#include "_rules.h"
#include "_timer.h"
#include "addon.h"
#include "conquer.h"
#include "data.h"
#include "dbgprint.h"
#include "globals.h"
#include "narrow.h" // RA2: Narrow_Text, ra2-decomp 61ba2ad
#include "goptions.h"
#include "houstype.h"
#include "init.h"
#include "ipxmgr.h"
#include "language\language.h"
#include "mapgen.h"
#include "mplayer.h"
#include "msgbox.h"
#include "multiplayer.h" // RA2: the includes this file needs
#include "netdlg.h"
#include "netshare.h"
#include "newmenu.h"
#include "ownrdraw.h"
#include "rules.h"
#include "scenario.h"
#include "sendfile.h"
#include "srfcache.h"
#include "stimer.h"
#include "timer.h"
#include "voc.h" // RA2: the includes this file needs
#include "random.h" // RA2: the includes this file needs
#include "_rand.h"
#include "msgloop.h" // RA2: the includes this file needs
#include "windlg.h"
#include "winstub.h"
#include "wonline.h"
#include "wsproto.h"
#include "wstring.h" // RA2: the includes this file needs
#include "widewstring.h" // RA2: the includes this file needs

#undef LoadString
#include <stddef.h> // RA2: the includes this file needs
#include <wchar.h> // RA2: the includes this file needs
#include "stringtable.h" // RA2: the includes this file needs

char * __fastcall Wide_To_Narrow(wchar_t const * text); // RA2: 0x6FA120, static buffer
wchar_t * __fastcall Narrow_To_Wide(char const * narrow); // RA2: 0x6FA180, static buffer
void __cdecl Lobby_Chat_Printf(int color, wchar_t const * format, ...); // RA2: 0x005C2C80, prints a line in the lobby's chat list
unsigned int __fastcall Compute_Name_CRC(wchar_t const * name); // RA2: 0x005BBA50, the wide form of netdlg.h's
void __fastcall Net2DisplayUsersFull(bool full); // RA2: 0x005BBDD0
void __fastcall Refresh_Slot_Settings(HWND dialog, bool full); // RA2: 0x005CA000, the slot rows of the lobby dialogs
Surface * __fastcall Country_Flag(int country); // RA2: the flag picture of a country
int Dialog_ID_Of_Window(HWND window); // RA2: 0x005EAB00, the dialog number a window was created for
namespace UI { bool __fastcall Is_Host_Player(int player); } // RA2: 0x005C8680
int Net2_g_Col_Name; // RA2: the columns of the users list
int Net2_g_Col_House;
int Net2_g_Col_Accept;
void __fastcall Free_Lobby_Entries(void); // RA2: 0x005C8950
void __fastcall Fill_Slots(void); // RA2: 0x005C8A10
void __fastcall Init_Lobby_Entries(void); // RA2: 0x005C86E0
void __fastcall Clear_Slot_Lists(void); // RA2: 0x005C8A70
void __fastcall Reset_Slots(void); // RA2: 0x005C8A90
void __fastcall Get_Slot_Names(wchar_t * list); // RA2: 0x005C8CF0, fills the slot names the host sends
bool __fastcall Lobby_Update_Dialog(HWND dialog, int count, bool flag1, bool flag2); // RA2: 0x005CBDB0
void __fastcall Dialog_On_WM_PAINT(HWND dialog); // RA2: 0x005FEE70, before a dialog paints
void __fastcall Dialog_On_WM_DRAWITEM(long item); // RA2: 0x005FE380
WideWstring __fastcall Limit_Name_Text(HWND control); // RA2: 0x005CCF50, the text of a name edit, cut to what the lobby allows
void __fastcall Save_Observer_Mode(void); // RA2: 0x005CD3A0, writes MultiPlayer ObserverMode to RA2.INI
void __fastcall Update_Observer_Buttons(HWND dialog); // RA2: 0x005CD540, the observer radio buttons of the lobby dialogs
namespace OwnerDraw { char const * __fastcall Control_Tooltip_Label(unsigned int key, HWND window); void __fastcall Register_Window_Classes(void); bool __fastcall WS_Save_Dialog_Values(HWND dialog); } // RA2: ownrdraw.cpp's 0x005E22B0 (the label of a control's tool tip, or NULL), 0x005EAC30 and 0x005E5FC0
bool __fastcall Focus_Next_Tab_Item(long lparam, bool flag); // RA2: 0x005DEF80, message 0x4DB
void __fastcall Dialog_On_OD_4EC(HWND dialog); // RA2: 0x005E8710, message 0x4EC
#include "progress.h" // RA2: the includes this file needs
#include "preview.h" // RA2: the includes this file needs
int __fastcall Side_Combo_Player(int id); // RA2: 0x004D13B0, the player a side combo box belongs to, or -1
int __fastcall Color_Combo_Player(int id); // RA2: 0x004D1D10, the player a color combo box belongs to, or -1
int __fastcall Player_Side_Combo_ID(int player); // RA2: 0x004D1350
int __fastcall Player_Color_Combo_ID(int player); // RA2: 0x004D1CB0
int __fastcall Side_Combo_Data(HWND dialog, int id, int index); // RA2: 0x004D1C70
int __fastcall Color_Combo_Data(HWND dialog, int id, int index); // RA2: 0x004D2900
wchar_t const * __fastcall Color_Name(int color); // RA2: 0x004D1D80
wchar_t const * __fastcall Side_Name(int side); // RA2: 0x004D1420
void __fastcall Fill_Side_Combos(HWND dialog); // RA2: 0x004D16D0
void __fastcall Init_Color_Entries(void); // RA2: 0x004D1EA0
void __fastcall Fill_Color_Combos(HWND dialog); // RA2: 0x004D2300
void __fastcall Select_Player_Side(HWND dialog, int player); // RA2: 0x004D1820
void __fastcall Select_Player_Color(HWND dialog, int player); // RA2: 0x004D23C0
void __fastcall Set_Slot_Country(HWND dialog, int id); // RA2: 0x005C9A10
void __fastcall Set_Slot_Color(HWND dialog, int id); // RA2: 0x005C9AB0
void __fastcall Lock_Slots(HWND dialog, int slot); // RA2: 0x005C9DA0
int __fastcall Control_Slot(int id); // RA2: 0x005C9800, the slot a control belongs to, or -1
wchar_t const * __fastcall Entry_Type_Name(NodeNameType * entry); // RA2: 0x005C9870
int __fastcall Combo_Selected_Data(HWND dialog, int id, int index); // RA2: 0x005C99D0
void __fastcall Send_Player_Choice(char const * name, char const * text); // RA2: 0x005C3D90, sends "R<side>,<color>" to the host
void __fastcall Send_Host_Option(char * text); // RA2: 0x005C3C60, sends an option command to the host
void __fastcall Send_Lobby_Options(bool force, bool now); // RA2: 0x005C3810, sends the game options when one changed
void __fastcall Enable_Player_Combos(HWND dialog, bool value); // RA2: 0x005CD030, enables the first player's side and color boxes unless the player is the observer
void __fastcall Fill_Options_Dialog(HWND dialog, int initial); // RA2: 0x005C3550
bool __fastcall Is_Lobby_Name(Wstring const & name); // RA2: 0x005CD2F0, one of the seven fixed lobby entry names
Wstring __fastcall Wstring_FromWide(WideWstring const & text); // RA2: 0x00771450, keeps the 7 bit characters
void __fastcall Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, as msgbox.cpp
LRESULT CALLBACK Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x005BF9B0
bool __fastcall Is_All_Slots_Filled(void); // RA2: 0x005C9440
bool __fastcall Is_Observer_Slot_Full(void); // RA2: 0x005C9490, no slot is left for an observer
void __fastcall Fill_Observer_Combo(HWND dialog, int id); // RA2: 0x004D1640
void __fastcall Fill_Random_Color_Combo(HWND dialog, int id); // RA2: 0x004D2250
void __fastcall Select_Color_Combo(HWND dialog, int id, int color); // RA2: 0x004D2480
void __fastcall Fill_Side_Combo(HWND dialog, int id); // RA2: 0x004D1560
void __fastcall Fill_Color_Combo(HWND dialog, int id); // RA2: 0x004D2080
int __fastcall Get_Slot_Control_Id(int slot); // RA2: 0x005C97A0, the id of the control of a slot
void __fastcall Load_Lobby_Options(HWND dialog, SessionOptionsClass * options); // RA2: 0x005CCA30, the options are a SessionOptionsClass (netshare.cpp reads their game mode, map, slots and settings)
void __fastcall Save_Lobby_Options(HWND dialog, SessionOptionsClass * options); // RA2: 0x005CC890, the executable passes the dialog and the options (the earlier declaration had no parameters)
void __fastcall Update_Slot_Controls(HWND dialog, int slot, bool flag); // RA2: 0x005C9B90
void __fastcall Change_Scenario_Slots(HWND dialog, int old_scenario, int scenario); // RA2: 0x005CB8D0
void __fastcall Refresh_Slot_Images(HWND dialog); // RA2: 0x005C9EC0
void __fastcall Set_Item_Text(HWND dialog, MultiplayerGameMode * mode); // RA2: 0x005C3450
void __fastcall Set_Skirmish_Dialog_Text(HWND dialog); // RA2: 0x005C34B0
void __fastcall Load_Random_Map_Preview(HWND dialog); // RA2: 0x005CC800
void __fastcall Clear_Observer_Slots(HWND dialog); // RA2: 0x005CB560
bool __fastcall Reset_AI_Slots(HWND dialog); // RA2: 0x005CACB0
void __fastcall Change_Slot_Type(HWND dialog, int id, NodeNameType * node, int mode); // RA2: 0x005CBF20
void __fastcall Seat_First_Player(int slot); // RA2: 0x005C8CD0
void __fastcall Set_Player_Slot(int slot, NodeNameType * node); // RA2: 0x005C8CC0
void __fastcall Set_Slot_Names(wchar_t const * list); // RA2: 0x005C8DD0
bool __fastcall Is_Host(void); // RA2: 0x005C86A0
void __fastcall Enable_Start_Button(void); // RA2: 0x005C9700
bool __fastcall Is_Color_Taken(int color); // RA2: 0x005C95D0
void __fastcall Release_Slot_By_Address(IPXAddressClass & address); // RA2: 0x005C91C0, the address is compared with IPXAddressClass::operator== (0x005235D0), which takes a non-const reference
int __fastcall Assign_Slot(NodeNameType * node); // RA2: 0x005C9170
bool __fastcall Net2DecodeGameopt(char const * options, char const * name); // RA2: 0x005C3FF0, returns true when the scenario changed (the executable ends with sete al on its same flag), TibSun's DecodePubGameopt returns bool too
void __fastcall Handle_Start_Reminder(void); // RA2: 0x005C3C20, the host's start message reached the joiner
void __fastcall Handle_Mode_Selection(void); // RA2: 0x005C34F0
void __fastcall Dispatch_Game_Mode(void); // RA2: 0x005C3B70
void __fastcall Show_Dialog(HWND dialog, int id); // RA2: 0x005FF7E0
void __fastcall Send_Map_Preview(MapPreviewClass * preview); // RA2: 0x005C7EF0, answers a preview request
void __fastcall Receive_Random_Map_Preview(void); // RA2: 0x005C7970
LRESULT CALLBACK MPlayer_Game_List_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x005BE6B0
LRESULT CALLBACK MPlayer_Host_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x005BEFE0
LRESULT CALLBACK MPlayer_Guest_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x005C2330

// RA2: the network dialogs' state; the executable keeps these in the order below (0x00A71E8C-0x00A71EA4)
int _netresponse; // RA2: 0x00A71E8C
int CurGame; // RA2: 0x00A71E90
JoinStateType JoinState; // RA2: 0x00A71E94
bool Net2OptionsLocked; // RA2: 0x00A71E98
bool Net2ObserverMode; // RA2: 0x00A71E99
int RulesID; // RA2: 0x00A71E9C
int ArtID; // RA2: 0x00A71EA0
int AIID; // RA2: 0x00A71EA4
char SerialNumber[23]; // RA2: 0x00A71BC4

/*
******************************** Prototypes *********************************
*/
int Request_To_Join(int join_index, bool observer);
void Unjoin_Game(int game_index);
static void Send_Join_Queries(int gamenow, int playernow, int chatnow, int init = 0);
void Get_Join_Responses(void); // RA2: 0x005C08A0
void Send_Chat_Message(HWND dialog, int color, int recipients);


bool Net2Callback(void)
{
	Call_Back();
	Get_Join_Responses();
	return(false);
}


void Send_Slot_Names(void) // RA2: 0x005BBD20; sends the lobby's slot names to every other player
{
	GlobalPacketType packet;
	memset(&packet, 0, sizeof(packet));
	packet.Command = NET_COMMAND_16;
	strcpy(packet.Name, Session.Handle);
	Get_Slot_Names(packet.Slots.Names);
	int count = Session.Players.Count();
	for (int i = 1; i < count; i++) {
		Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address);
		Call_Back();
	}
}


/// <summary>
/// Redisplays the list of users.
/// This routine is the entry point the network code reaches for whenever a change to the
/// user list has to find its way onto the screen.
/// </summary>
void Net2DisplayUsers(void)
{
	Net2DisplayUsersFull(false);
}


/***********************************************************************************************
 * Init_Network -- initializes network stuff                                                   *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = Initialization OK, false = error                                                *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
/// <summary>
/// Services the network while something else is waiting.
/// </summary>
/// <returns>bool; Should the caller abandon what it is waiting on?</returns>
static bool Find_Active_Game_Dialog(HWND & active)
{
	active = WS_Top_Window();
	int dialog_id = WS_Top_Window_ID();
	if (dialog_id == IDD_MPLAYER_HOST || dialog_id == IDD_MPLAYER_GUEST) {
		return true;
	}
	active = WS_Next_Lower_Dialog(active);
	while (active) {
		int kind = Dialog_ID_Of_Window(active);
		if (kind == IDD_MPLAYER_HOST || kind == IDD_MPLAYER_GUEST) {
			return true;
		}
		active = WS_Next_Lower_Dialog(active);
	}
	return false;
}


/// <summary>
/// Redisplays the users of the lobby: the chat players, or the players of the game that is selected.
/// </summary>
void __fastcall Net2DisplayUsersFull(bool full) // RA2: 0x005BBDD0; with a game lobby dialog open its slot rows are refreshed instead
{
	HWND active;
	if (Find_Active_Game_Dialog(active)) {
		Refresh_Slot_Settings(active, full);
		return;
	}
	{
		HWND dialog = WS_Top_Window();
		HWND list = GetDlgItem(dialog, IDC_USERS);
		if (dialog && list) {
			HWND users = GetDlgItem(dialog, IDC_USERS);
			OwnerDraw::CellData cell;
			int top = SendDlgItemMessage(dialog, IDC_USERS, LB_GETTOPINDEX, 0, 0);
			SendDlgItemMessage(dialog, IDC_USERS, OD_DISABLEPAINT, 0, 1);
			Dictionary<WideWstring, bool> selections(Wstring_Hash);
			LBSaveSelections(list, selections);
			SendDlgItemMessage(dialog, IDC_USERS, LB_RESETCONTENT, 0, 0);
		if (CurGame == 0) {
				SendMessage(users, 0x4CC, 0, (LPARAM)Narrow_To_Wide(Session.Handle));
				for (int chat = 1; chat < Session.Chat.Count(); ++chat) {
					SendMessage(users, 0x4CC, chat, (LPARAM)Session.Chat[chat]);
			}
		} else {
				for (int player = 0; player < Session.Players.Count(); ++player) {
					int state = 0;
					if (UI::Is_Host_Player(player)) {
						state = 2;
						Session.Players[player]->Player.Status = 1;
					} else if (Session.Players[player]->Player.Status != 0) {
						state = 1;
				}
					wchar_t info[128];
					wchar_t country_name[128];
					swprintf(info, L"%s", Session.Players[player]);
					int country = Session.Players[player]->Get_Country();
					Surface * flag = Country_Flag(country);
					switch (country) {
						case 0: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 383)); break;
						case 1: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 386)); break;
						case 2: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 389)); break;
						case 3: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 392)); break;
						case 4: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 395)); break;
						case 5: swprintf(country_name, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\netdlg2.cpp", 398)); break;
						case 6: swprintf(country_name, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\netdlg2.cpp", 401)); break;
						case 7: swprintf(country_name, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\netdlg2.cpp", 404)); break;
						case 8: swprintf(country_name, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\netdlg2.cpp", 407)); break;
				}
					SendMessage(users, 0x4CC, -1, (LPARAM)info);
					int color = PlayerColorTable[Session.Players[player]->Player.Color];
					cell.type = OwnerDraw::CellData::PRIMARY;
					cell.color = color;
				cell.hint.set(L""); // RA2: set, not operator=
					SendDlgItemMessage(dialog, IDC_USERS, OD_SETCELL, MAKEWPARAM(Net2_g_Col_Name, player), (LPARAM)&cell);
					cell.type = OwnerDraw::CellData::SURFACE;
					cell.hint.set(country_name);
					cell.surf = flag;
					SendDlgItemMessage(dialog, IDC_USERS, OD_SETCELL, MAKEWPARAM(Net2_g_Col_House, player), (LPARAM)&cell);
				cell.hint.set(L""); // RA2: set, not operator=
					cell.type = OwnerDraw::CellData::SURFACE;
					if (state == 2) {
						cell.surf = SurfaceCache.GetSurface("wolhost.pcx", false);
					} else if (state) {
						cell.surf = SurfaceCache.GetSurface("wolacpt.pcx", false);
				} else {
						cell.type = OwnerDraw::CellData::INVALID;
				}
					SendDlgItemMessage(dialog, IDC_USERS, OD_SETCELL, MAKEWPARAM(Net2_g_Col_Accept, player), (LPARAM)&cell);
			}
		}
			LBRestoreSelections(list, selections);
			SendDlgItemMessage(dialog, IDC_USERS, LB_SETTOPINDEX, top, 0);
			SendDlgItemMessage(dialog, IDC_USERS, OD_DISABLEPAINT, 0, 0);
			InvalidateRect(list, NULL, 0);
			UpdateWindow(list);
	}
	}
}


void __fastcall Reject_Join(NodeNameType * node) // RA2: 0x005BC3E0; turns a joiner down
{
	memset(&Session.GPacket, 0, sizeof(Session.GPacket));
	Session.GPacket.Command = NET_REJECT_JOIN;
	Session.GPacket.Reject.Why = REJECT_BY_OWNER;
	Ipx.Send_Global_Message(&Session.GPacket, sizeof(Session.GPacket), 1, &node->Address);
}


bool __fastcall Update_Lobby_Count(int count, bool flag1, bool flag2) // RA2: 0x005BC430
{
	HWND dialog = WS_Find_Dialog(IDD_MPLAYER_HOST);
	return Lobby_Update_Dialog(dialog, count, flag1, flag2);
}


bool Net2Init_Network(void)
{
	NetNumType net;
	NetNodeType node;

	if (!Ipx.Init()) {
		return(false);
	}
	if (Session.Type != GAME_INTERNET && Session.IsBridge) {
		Session.BridgeNet.Get_Address(net, node);
		Ipx.Set_Bridge(net);
	}
	Init_Lobby_Entries(); // RA2: the fixed lobby entries (open, closed, computers)
	Clear_Slot_Lists();
	Reset_Slots();
	return(true);

}	/* end of Init_Network */


/***********************************************************************************************
 * Shutdown_Network -- shuts down network stuff                                                *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void Net2Shutdown_Network(void)
{
//
// Note: The thought behind this section of code was that if the program
// terminates early, without an EventClass::EXIT event, it still needs to
// tell the other systems that it's gone, so it would send a SIGN_OFF packet.
// BUT, this causes a sync bug if the systems are running slow and this system
// is running ahead of the others; it will send the NET_SIGN_OFF >>before<<
// the other system execute their EventClass::EXIT event, and the other systems
// will kill the connection at some random Frame # & turn my stuff over to
// the computer possibly at different times.
// BRR, 10/29/96
//
	Free_Lobby_Entries(); // RA2: no sign-off packet, the lobby entries are freed instead
	Session.GameName[0] = 0;
	if (PacketTransport != NULL) {
		delete PacketTransport;
	}
	PacketTransport = NULL;

}	/* end of Shutdown_Network */


/// <summary>
/// Ages out the games and chatters that have gone quiet.
/// This routine is called regularly while the lobby is up. A host that has stopped
/// advertising has its game dropped from the list, and a silent chatter is prodded once
/// before it too is forgotten.
/// </summary>
void Net2ServiceGameList(void)
{
	int i;

	for (i = 1; i < Session.Games.Count(); i++) {
		if (wcscmp(Session.Games[i]->Name, Session.GameName) != 0) {
			if (TickCount - Session.Games[i]->Game.LastTime > 300) {
				DebugString("Game '%ls' has timed out\n", Session.Games[i]->Name);
				delete Session.Games[i];
				Session.Games.Delete(Session.Games[i]);

				Net2DisplayGameList();
				if (i <= CurGame) {
					Net2DisplayUsersFull(false);
					Send_Join_Queries(0, 1, 0, 0);
				}
			}
		}
	}

	for (i = 1; i < Session.Chat.Count(); i++) {
		if (Session.Chat[i]->Chat.LastTime == 0) {
			Session.Chat[i]->Chat.LastTime = TickCount;
		}
		if (TickCount - Session.Chat[i]->Chat.LastTime > 360) {
			delete Session.Chat[i];
			Session.Chat.Delete(Session.Chat[i]);
			Net2DisplayUsersFull(false);
		} else if (TickCount - Session.Chat[i]->Chat.LastTime > 300 && Session.Chat[i]->Chat.LastChance == 0) {
			GlobalPacketType packet;
			memset(&packet, 0, sizeof(packet));
			strcpy(packet.Name, Session.Handle);
			packet.Command = NET_CHAT_REQUEST;
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.Chat[i]->Address);
			Call_Back();
			Session.Chat[i]->Chat.LastChance = 1;
		}
	}
}


/// <summary>
/// Redisplays the list of games available to join.
/// The lobby heads the list and the games other hosts are advertising follow it. The
/// current selection is clamped and re-applied, since a game can vanish out from under
/// the player at any moment.
/// </summary>
void Net2DisplayGameList(void)
{
	wchar_t text[80];

	if (WS_Top_Window_ID() == IDD_MPLAYER_GAME_LIST) {
		HWND dialog = WS_Top_Window();
		HWND list = GetDlgItem(dialog, IDC_GAMELIST);
		if (CurGame >= Session.Games.Count()) {
			CurGame = Session.Games.Count() - 1;
			Send_Join_Queries(0, 1, 0, 0);
		}
		if (CurGame < 0) {
			CurGame = 0;
		}
		int top = SendDlgItemMessage(dialog, IDC_GAMELIST, LB_GETTOPINDEX, 0, 0);
		SendDlgItemMessage(dialog, IDC_GAMELIST, OD_DISABLEPAINT, 0, 1);
		SendMessage(list, LB_RESETCONTENT, 0, 0);
		SendMessage(list, OD_ADDSTRINGW, 0, (LPARAM)StringTable::LoadString("TXT_LOBBY", NULL, "D:\\Ra2\\netdlg2.cpp", 490));
		for (int i = 1; i < Session.Games.Count(); i++) {
			if (Session.Games[i]->Game.IsOpen) {
				swprintf(text, StringTable::LoadString("TXT_THATGUYS_GAME", NULL, "D:\\Ra2\\netdlg2.cpp", 496), Session.Games[i]->Name);
			} else {
				swprintf(text, StringTable::LoadString("TXT_THATGUYS_GAME_BRACKET", NULL, "D:\\Ra2\\netdlg2.cpp", 498), Session.Games[i]->Name);
			}
			SendMessage(list, OD_ADDSTRINGW, 0, (LPARAM)text);
		}
		SendDlgItemMessage(dialog, IDC_GAMELIST, LB_SETCURSEL, CurGame, 0);
		SendDlgItemMessage(dialog, IDC_GAMELIST, LB_SETTOPINDEX, top, 0);
		SendDlgItemMessage(dialog, IDC_GAMELIST, OD_DISABLEPAINT, 0, 0);
		list = GetDlgItem(dialog, IDC_GAMELIST);
		InvalidateRect(list, NULL, 0);
		UpdateWindow(list);
	}
}


/// <summary>
/// Encodes the current game options into a text string.
/// This routine builds the blob the host broadcasts to the guests, so that every machine
/// agrees on the rules, the scenario, and who is playing what.
/// </summary>
/// <param name="out">Buffer to build the encoded option string within.</param>
/// <remarks>Be sure the destination buffer is big enough for the options and an entry for
/// every player in the game.</remarks>
void Net2EncodeGameopt(char *out)
{
	static char useroptions[512];

	Wstring name = Session.Options.ScenarioDescription ? Wstring_FromWide(Session.Options.ScenarioDescription) : ""; // RA2: only the 7 bit characters of the map name are sent
	int value = Session.ScenarioMaxPlayers; // RA2: not recovered
	sprintf(out, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%d,%s,%s:", Seed, Options.GameSpeed, Session.Options.GameSpeed, Session.Options.Credits, Session.Options.UnitCount, // RA2: the older option string; as above
		Session.Options.ShortGame, Session.Options.SWAllowed, Session.Options.BuildOffAlly, Session.Options.MCVRedeploy, Session.Options.Goodies, Session.Options.MPModeIndex,
		name.get(), value, Session.ScenarioIsOfficial, Session.ScenarioFileLength, Session.ScenarioFileName, Session.ScenarioDigest);

	memset(useroptions, 0, sizeof(useroptions));

	for (int i = 0, count = Session.Players.Count(); i < count; i++) {
		NodeNameType * entry = Session.Players[i];
		int country = entry->Get_Country(); // RA2: country and color of each player, three more numbers that are zero
		int color = entry->Get_Color();
		wchar_t const * player_name = Session.Players[i]->Name;
		sprintf(useroptions + strlen(useroptions), "%s,%d,%d,%x,%x,%x,", Wide_To_Narrow(player_name), country, color, 0, 0, 0);
	}
	useroptions[strlen(useroptions) - 1] = ':'; // RA2: the player list ends in a colon
	strcat(useroptions, "@:"); // RA2: then the computer players
	for (int k = 0; k < 8; k++) {
		sprintf(useroptions + strlen(useroptions), "%d,%d,%d,", Session.Options.AISlots.Difficulties[k], Session.Options.AISlots.Countries[k] == -3 ? -1 : Session.Options.AISlots.Countries[k], Session.Options.AISlots.Colors[k]);
	}
	DebugString("Size of options string is %d bytes\n", strlen(useroptions)); // RA2: logged
	DebugString("Options: %s\n", useroptions);
	strcat(out, useroptions);
}


/// <summary>
/// Sets the accept status of a player.
/// The player list is redisplayed afterwards, so that the ready markers beside the names
/// keep up with what the other machines have reported.
/// </summary>
/// <param name="who">The name of the player to adjust. NULL means this machine's own
/// player.</param>
/// <param name="status">The accept status to record against the player.</param>
void Net2SetAccept(char const * who, int status)
{
	if (who == NULL) {
		who = Session.Handle;
		if (status == 0) {
			HWND dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST); // RA2: the player's own side and color boxes follow the status
			Enable_Player_Combos(dialog, true);
		}
	}
	if (wcscmp(Narrow_To_Wide(who), Session.GameName) == 0) { // RA2: the host's own entry is always accepted
		status = 1;
	}

	int offset = -1;

	for (int i = 0; i < Session.Players.Count(); i++) {
		if (_stricmp(Wide_To_Narrow(Session.Players[i]->Name), who) == 0) { // RA2: names are wide, compared without case
			offset = i;
			break; // RA2: the first match
		}
	}

	if (offset != -1) {
		Session.Players[offset]->Player.Status = status;
	}
	Net2DisplayUsersFull(false); // RA2: the full refresh of the lobby
}


/// <summary>
/// Fetches the accept status of a player.
/// </summary>
/// <param name="who">The name of the player to examine. NULL means this machine's own
/// player.</param>
/// <returns>Returns with the player's accept status, or -1 if no such player is in the
/// game.</returns>
int Net2GetAccept(char const * who)
{
	if (who == NULL) {
		who = Session.Handle;
	}
	if (wcscmp(Narrow_To_Wide(who), Session.GameName) == 0) { // RA2: the host is always accepted
		return(1);
	}

	int offset = -1;

	for (int i = 0; i < Session.Players.Count(); i++) {
		if (_stricmp(Wide_To_Narrow(Session.Players[i]->Name), who) == 0) { // RA2: names are wide, compared without case
			offset = i;
		}
	}

	if (offset != -1) {
		return (Session.Players[offset]->Player.Status);
	}
	return (-1);
}


/// <summary>
/// Sets the house and color of a player in the game.
/// This routine is called as the option packets arrive from the other machines. The
/// player list is redisplayed, and when it is this machine's own entry that changed, the
/// color combo box is nudged along to agree with it.
/// </summary>
/// <param name="who">The name of the player to adjust.</param>
/// <returns>Returns with TRUE if the player's house actually changed.</returns>
int Net2SetHouseAndColor(char const * who, int house, int color) // RA2: 0x005BCCF0; a player's side and color changed (by the host's message): the lobby dialog's boxes follow
{
	int offset = -1;
	NodeNameType * node;
	for (int i = 0; i < 8; i++) {
		char * name = Narrow_Text(Session.Slots[i]->Name); // RA2: the twin's statement, with its Narrow_Text (narrow.h) where the twin calls the conversion directly
		if (_stricmp(name, who) == 0) {
			offset = i;
			break;
		}
	}

	int retval = 0;
	if (offset == -1) {
		return(0);
	}

	HWND dialog = WS_Find_Dialog(0xbc);
	if (dialog == NULL) {
		dialog = WS_Find_Dialog(0xbd);
	}

	node = Session.Slots[offset];
	if (node->Get_Country() != house) {
		retval = 1;
		node->Set_Country(house);
	}

	int id = Player_Side_Combo_ID(offset);
	bool random = Side_Combo_Data(dialog, id, -1) == -3;
	if (house == -3) {
		if (!random) {
			Fill_Observer_Combo(dialog, id);
			int box = Get_Slot_Control_Id(offset);
			SendMessage(GetDlgItem(dialog, box), 0x4f1, 0, 1);
			id = Player_Color_Combo_ID(offset);
			Fill_Random_Color_Combo(dialog, id);
			Select_Color_Combo(dialog, id, 8);
		}
	} else if (random) {
		Fill_Side_Combo(dialog, id);
		int box = Get_Slot_Control_Id(offset);
		SendMessage(GetDlgItem(dialog, box), 0x4f1, 0, 0);
		id = Player_Color_Combo_ID(offset);
		Fill_Color_Combo(dialog, id);
		Select_Color_Combo(dialog, id, -2);
	}

	if (Session.Slots[offset]->Get_Color() != color) {
		node->Set_Color(color);
	}
	if (Session.Slots[offset] == Session.Players[0]) {
		Session.PrefColor = color;
	}
	Net2DisplayUsersFull(false);
	return(retval);
}


/// <summary>
/// Fetches the serial number recorded in the registry.
/// A key that is missing, or that cannot be opened, simply leaves the buffer as it was
/// found -- the caller is expected to have primed it with something harmless.
/// </summary>
/// <param name="serial">Buffer to fill in with the serial number found.</param>
/// <param name="reg_key">The registry key, beneath the local machine hive, to read from.</param>
/// <remarks>Be sure the buffer is big enough to hold an entire encrypted serial number.</remarks>
static void Get_Serial_From_Registry(char * serial, char const * reg_key)
{
	if (reg_key && strlen(reg_key) != 0) {
		HKEY rKey;
		char keyname[256];
		strcpy(keyname, reg_key);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyname, 0, KEY_READ, &rKey) == ERROR_SUCCESS) {
			DWORD type;
			DWORD sizeOfBuffer = ENCRYPTION_STRING_LENGTH;
			RegQueryValueEx(rKey, "Serial", NULL, &type, (BYTE *)serial, &sizeOfBuffer);
			RegCloseKey(rKey);
		}
	}
	serial[SERIAL_MAX-1] = 0;
}


/// <summary>
/// Fetches the decrypted serial number of this installation.
/// The scrambled serial is pulled out of the registry and then unpicked with the key file
/// that shipped alongside the game. This routine is used to identify the player to the
/// online service.
/// </summary>
/// <param name="buffer">Buffer to fill in with the decrypted serial number.</param>
/// <returns>bool; Was the serial number recovered? Failure means the key file was not
/// available.</returns>
/// <remarks>Be sure the destination buffer is big enough to hold an entire serial number.</remarks>
bool Decrypt_Serial(char * buffer)
{
	char serial[ENCRYPTION_STRING_LENGTH];

	bool encrypt = false;

	memset(serial, '0', SERIAL_MAX-1);
	serial[SERIAL_MAX-1] = 0;
	strcpy(buffer, serial);
	memset(serial, 0, sizeof(serial));

	Get_Serial_From_Registry(serial, "SOFTWARE\\Westwood\\Red Alert 2"); // RA2: Red Alert 2's key

	strcpy(buffer, serial);

	int sign = encrypt ? 1 : -1;

	int number;
	int temp;
	int pos = 0;

	FILE *in = fopen("woldata.key", "r");

	if (in == NULL) {
		return(false);
	}

	while ((number = fgetc(in)) != EOF) {
		temp = serial[pos] - '0';
		temp %= 10;
		number *= sign;
		temp += number;
		temp += 1000;
		temp %= 10;
		temp += '0';
		serial[pos] = temp;

		pos++;
		if (pos == (int)strlen(serial)) {
			pos = 0;
		}
	}

	fclose(in);

	strcpy(buffer, serial);

	return(true);
}


/***********************************************************************************************
 * Remote_Connect -- handles connecting this user to others                                    *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = connections established; false = not                                            *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
/// <summary>
/// Starts the game the player has joined.
/// </summary>
bool Net_Join_Dialog(void)
{
	RulesID = Rule->Get_Rule_Unique_ID();
	Rule->Load_Art_INI();
	ArtID = Rule->Get_Art_Unique_ID();
	AIID = Rule->Get_AI_Unique_ID();
	Decrypt_Serial(SerialNumber);
	Ipx.Set_Timing(30, -1, 600, true);
	Session.NetStealth = false;
	Session.GameName[0] = 0;
	Session.NetOpen = true;
	Net2OptionsLocked = false;
	OwnerDraw::Register_Window_Classes();
	HWND dialog = (HWND)WS_Create_Dialog(ProgramInstance, IDD_MPLAYER_GAME_LIST, MainWindow, (DLGPROC)MPlayer_Game_List_Dialog_Proc, 0);
	Show_Dialog(dialog, IDD_MPLAYER_GAME_LIST);
	ShowWindow(dialog, SW_SHOWNORMAL);
	Net2DisplayUsersFull(false);
	CurGame = 0;
	_netresponse = 0;
	JoinState = JOIN_NOTHING;
	Net2JoinListEnabled = true;

	while (true) {
		//.....................................................................
		// Pop up the network Join/New dialog
		//.....................................................................
		while (_netresponse == 0) {
			Ipx.Service();
			Sleep(0);
			Call_Back();
			Ipx.Service();
			Title_Screen_Restore(false);
			MessageLoopCount++;
			MSG msg;
			while (Peek_Windows_Message(&msg, 0, 0, 0, 1)) {
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}

			Call_Back();
			if (_netresponse != 0) {
				break;
			}

			if (WS_Top_Window()) {
				Send_Join_Queries(0, 0, 0, 0);
				Get_Join_Responses();
				if (WS_Top_Window_ID() == IDD_MPLAYER_HOST) {
					Send_Lobby_Options(false, false);
				}
				Net2ServiceGameList();
			}
		}

		//.....................................................................
		//	-1 = user selected Cancel
		//.....................................................................
		if (_netresponse == IDC_CANCEL || _netresponse == IDCANCEL) {
			Session.Write_MultiPlayer_Settings();
			if (WS_Top_Window_ID() == IDD_MPLAYER_GAME_LIST) {
				if (JoinState > JOIN_NOTHING) {
					Unjoin_Game(CurGame);
					Ipx.Service();
				}
				WS_Destroy_Dialog(0, 0);
				Clear_Slot_Lists();
				Fill_Slots();
				Clear_Vector(&Session.Players);
				Clear_Vector(&Session.Games);
				Clear_Vector(&Session.Chat);
				Session.NetOpen = false;
				Ipx.Service();
				return(false);
			}
			if (WS_Top_Window_ID() == IDD_MPLAYER_HOST) {
				Unjoin_Game(CurGame);
				JoinState = JOIN_NOTHING;
				WS_Destroy_Dialog(WS_Top_Window(), 0);
				Reset_Slots();
				dialog = (HWND)WS_Create_Dialog(ProgramInstance, IDD_MPLAYER_GAME_LIST, MainWindow, (DLGPROC)MPlayer_Game_List_Dialog_Proc, 0);
				Show_Dialog(dialog, IDD_MPLAYER_GAME_LIST);
				ShowWindow(dialog, SW_SHOWNORMAL);
				Send_Join_Queries(0, 0, 1, 0);
			} else if (WS_Top_Window_ID() == IDD_MPLAYER_GUEST) {
				if (JoinState == JOIN_CONFIRMED) {
					Unjoin_Game(CurGame);
					JoinState = JOIN_NOTHING;
				} else {
					//...............................................................
					// If I'm not joined to a game, send a SIGN_OFF to all players
					// in my Chat vector (but not to myself, index 0)
					//...............................................................
					GlobalPacketType packet;
					memset(&packet, 0, sizeof(packet));
					packet.Command = NET_SIGN_OFF;
					strcpy(packet.Name, Session.Handle);
					for (int i = 1; i < Session.Chat.Count(); i++) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Chat[i]->Address);
						Call_Back();
					}
					Ipx.Send_Global_Message(&packet, sizeof(packet), 0, 0);
					if (Session.IsBridge) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
					}
				}
				CDTimerClass<SystemTimerClass> timeout = 600;
				while (Ipx.Global_Num_Send() > 0) {
					if (Ipx.Service() == 0) {
						break;
					}
					if (timeout == 0) {
						break;
					}
					Call_Back();
				}
				Session.GameName[0] = 0;
				JoinState = JOIN_NOTHING;
				WS_Destroy_Dialog(0, 0);
				_netresponse = 0;
				CurGame = 0;
				Clear_Vector(&Session.Players);
				Clear_Slot_Lists();
				Reset_Slots();
				dialog = (HWND)WS_Create_Dialog(ProgramInstance, IDD_MPLAYER_GAME_LIST, MainWindow, (DLGPROC)MPlayer_Game_List_Dialog_Proc, 0);
				Show_Dialog(dialog, IDD_MPLAYER_GAME_LIST);
				ShowWindow(dialog, SW_SHOWNORMAL);
			} else {
				if (JoinState == JOIN_CONFIRMED) {
					Unjoin_Game(CurGame);
					JoinState = JOIN_NOTHING;
				} else {
					GlobalPacketType packet;
					memset(&packet, 0, sizeof(packet));
					packet.Command = NET_SIGN_OFF;
					strcpy(packet.Name, Session.Handle);
					for (int i = 1; i < Session.Chat.Count(); i++) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Chat[i]->Address);
						Call_Back();
					}
					Ipx.Send_Global_Message(&packet, sizeof(packet), 0, 0);
					if (Session.IsBridge) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
					}
				}
				CDTimerClass<SystemTimerClass> timeout = 600;
				while (Ipx.Global_Num_Send() > 0) {
					if (Ipx.Service() == 0) {
						break;
					}
					if (timeout == 0) {
						break;
					}
					Call_Back();
				}
				Session.GameName[0] = 0;
				JoinState = JOIN_NOTHING;
				_netresponse = 0;
				CurGame = 0;
				Clear_Vector(&Session.Players);
				Clear_Slot_Lists();
				Reset_Slots();
				dialog = (HWND)WS_Create_Dialog(ProgramInstance, IDD_MPLAYER_GAME_LIST, MainWindow, (DLGPROC)MPlayer_Game_List_Dialog_Proc, 0);
				Show_Dialog(dialog, IDD_MPLAYER_GAME_LIST);
				ShowWindow(dialog, SW_SHOWNORMAL);
			}
		}

		//.....................................................................
		//	0 = user has joined an existing game; save values & return
		//.....................................................................
		if (_netresponse == 0x6CD) {
			Session.NetStealth = false;
			Session.Write_MultiPlayer_Settings();
			if (Request_To_Join(CurGame, ObserverMode != 0)) {
				JoinState = JOIN_WAIT_CONFIRM;
			} else {
				HWND top = WS_Top_Window();
				if (top) {
					EnableWindow(top, TRUE);
					HWND button = GetDlgItem(top, IDC_YOURNAME);
					if (button) {
						EnableWindow(button, TRUE);
					}
				}
			}
			if (Session.MPGameMode) {
				Session.MPGameMode->vt_entry_24();
			}
		}

		//.....................................................................
		//	1 = user requests New Network Game
		//.....................................................................
		if (_netresponse == 0x6CE) {
			Session.NetStealth = false;
			Session.Write_MultiPlayer_Settings();
			if (Request_To_Join(CurGame, true)) {
				JoinState = JOIN_WAIT_CONFIRM;
			} else {
				HWND top = WS_Top_Window();
				if (top) {
					EnableWindow(top, TRUE);
					HWND button = GetDlgItem(top, IDC_YOURNAME);
					if (button) {
						EnableWindow(button, TRUE);
					}
				}
			}
			if (Session.MPGameMode) {
				Session.MPGameMode->vt_entry_24();
			}
		}

		if (_netresponse == 0x6CC) {
			bool ok = true;

			//...............................................................
			// Force user to enter a name
			//...............................................................
			if (strlen(Session.Handle) < 1) {
				ODMessageBox(StringTable::LoadString("TXT_NAME_BLANK", NULL, "D:\\Ra2\\netdlg2.cpp", 1571), 0, Net2Callback, false);
				ok = false;
			}

			//...............................................................
			// Ensure name is unique
			//...............................................................
			for (int i = 0; i < Session.Games.Count(); i++) {
				if (ok && !_stricmp(Wide_To_Narrow(Session.Games[i]->Name), Session.Handle)) {
					ODMessageBox(StringTable::LoadString("TXT_GAMENAME_MUSTBE_UNIQUE", NULL, "D:\\Ra2\\netdlg2.cpp", 1581), 0, Net2Callback, false);
					ok = false;
					break;
				}
			}

			if (ok) {
				//...............................................................
				// Save player & game name
				//...............................................................
				wcscpy(Session.GameName, Narrow_To_Wide(Session.Handle));
				Session.NetOpen = true;
				Session.NetStealth = false;
				Session.Options.ScenarioIndex = 0;
				Session.PlayingAgainstVersion = VerNum.Version_Number();
				Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex);
				WS_Destroy_Dialog(0, 0);
				_netresponse = 0;

				//------------------------------------------------------------------------
				// Clear the list of players
				//------------------------------------------------------------------------
				Clear_Vector(&Session.Players);

				//------------------------------------------------------------------------
				// Add myself to the list, and to the Players vector.
				//------------------------------------------------------------------------
				NodeNameType * who = new NodeNameType;
				memset(who, 0, sizeof(*who));
				wcscpy(who->Name, Narrow_To_Wide(Session.Handle));
				strcpy(who->Slot.SerialPad, SerialNumber);
				who->Set_Country(Session.Chosen_House());
				who->Set_Color(Session.Chosen_Color());
				who->Player.Status = 1;
				who->Slot.SpectatorFlag = 0;
				LobbyObserverSlot = 7;
				if (Net2ObserverMode) {
					LobbyObserverSlot = 0;
					who->Slot.SpectatorFlag = -1;
				} else if (who->Get_Country() == -3) {
					who->Set_Country(-2);
				}
				Session.Players.Add(who);
				if (Session.MPGameMode) {
					Session.MPGameMode->vt_entry_24();
				}
				Clear_Slot_Lists();
				Reset_Slots();
				Seat_First_Player(0);
				if (LobbyObserverSlot == 0) {
					Session.Slots[0]->Set_Country(-3);
					Session.Slots[0]->Slot.SpectatorFlag = -1;
				}
				JoinState = JOIN_CONFIRMED;

				NodeNameType * game = new NodeNameType;
				wcscpy(game->Name, Narrow_To_Wide(Session.Handle));
				game->Address = Session.GAddress;
				game->Game.IsOpen = true;
				game->Game.LastTime = TickCount();
				Session.Games.Add(game);

				CurGame = Session.Games.Count() - 1;

				//..................................................................
				//	Pop up the New Network Game dialog; if user selects OK, return
				//	'true'; otherwise, return to the Join Dialog.
				//..................................................................
				HWND host = (HWND)WS_Create_Dialog(ProgramInstance, IDD_MPLAYER_HOST, MainWindow, (DLGPROC)MPlayer_Host_Dialog_Proc, 0);
				Show_Dialog(host, IDD_MPLAYER_HOST);
				SendMessageA(host, OD_SETTOP, 0, 1);
				ShowWindow(host, SW_SHOWNORMAL);
			} else {
				HWND top = WS_Top_Window();
				if (top) {
					EnableWindow(top, TRUE);
					HWND button = GetDlgItem(top, IDC_YOURNAME);
					if (button) {
						EnableWindow(button, TRUE);
					}
				}
			}
		}

		if (_netresponse == 1 && WS_Top_Window_ID() == IDD_MPLAYER_GUEST) {
			while (WS_Destroy_Dialog(0, 0) == true) {
			}
			if (JoinState == JOIN_GAME_START) {
				if (!Net2ReadyToGo(0)) {
					_netresponse = IDCANCEL;
					Net2OptionsLocked = false;
					continue;
				}
				Net2OptionsLocked = true;
			} else if (JoinState == JOIN_GAME_START_LOAD) {
				Net2ReadyToGo(1);
			}
			_netresponse = 0;

			PregameSetup();
			Session.FrameSendRate = 5;
			Session.LatencyFudge = 0;
			Session.PrecalcMaxAhead = 0;
			Session.PrecalcDesiredFrameRate = 0;
			if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
				Session.MaxAhead = ((Ipx.Global_Response_Time() / 8 + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate > Session.FrameSendRate * 2 ? ((Ipx.Global_Response_Time() / 8 + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate : Session.FrameSendRate * 2;
			} else {
				Session.MaxAhead = (int)Ipx.Global_Response_Time() / 8 > 2 ? (int)Ipx.Global_Response_Time() / 8 : 2;
			}
			Session.NetOpen = false;
			Session.Write_MultiPlayer_Settings();
			return(true);
		}

		if (_netresponse == IDC_GO) {
			Net2OptionsLocked = false;
			Session.Write_MultiPlayer_Settings();
		}
		int count = Session.Players.Count();
		int observers = 0;
		for (int i = 0; i < count; i++) {
			if (Session.Players[i]->Slot.SpectatorFlag == -1) {
				observers++;
			}
		}
		if (_netresponse == IDC_GO) {
			if (count - observers <= 1) {
				Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_ONLY_ONE", NULL, "D:\\Ra2\\netdlg2.cpp", 1760));
				_netresponse = 0;
				EnableWindow(GetDlgItem(WS_Top_Window(), IDC_GO), TRUE);
			}
			if (_netresponse == IDC_GO) {
				bool nomap = false;
				for (int i = 1; i < count; i++) {
					NodeNameType * player = Session.Players[i];
					if (player->Slot.Unknown7F == 0) {
						Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostNoMap", NULL, "D:\\Ra2\\netdlg2.cpp", 1772), player->Name, Session.Options.ScenarioDescription);
						nomap = true;
					}
				}
				if (nomap) {
					_netresponse = 0;
					EnableWindow(GetDlgItem(WS_Top_Window(), IDC_GO), TRUE);
				}
			}
		}
		MultiplayerGameMode * mode = Session.MPGameMode;
		if (_netresponse == IDC_GO) {
			int map_players = RandomMapWaypointCount(Session.Options.ScenarioIndex);
			if (map_players < Session.Options.AIPlayers - observers + count) {
				Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_SCENARIO_TOO_SMALL", NULL, "D:\\Ra2\\netdlg2.cpp", 1791), map_players);
				EnableWindow(GetDlgItem(WS_Top_Window(), IDC_GO), TRUE);
				_netresponse = 0;
			}
			{
				WideWstring message;
				if (!mode->CheckStartConditions(&message)) {
					Lobby_Chat_Printf(-1, message.get());
					EnableWindow(GetDlgItem(WS_Top_Window(), IDC_GO), TRUE);
					_netresponse = 0;
				}
			}
			if (_netresponse == IDC_GO) {
				for (int i = 0; i < count; i++) {
					if (Session.Players[i]->Player.Status == 0) {
						Dispatch_Game_Mode();
						Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostGameStartHost", NULL, "D:\\Ra2\\netdlg2.cpp", 1817));
						_netresponse = 0;
						EnableWindow(GetDlgItem(WS_Top_Window(), IDC_GO), TRUE);
						break;
					}
				}
				if (_netresponse == IDC_GO) {
					Net2OptionsLocked = true;
					int i;
					for (i = 0; i < Session.Players.Count(); i++) {
						if (Session.Players[i]->Slot.SpectatorFlag == -1) {
							Session.Players[i]->Set_Color(-2);
						}
					}
					mode->vt_entry_58();
					Session.Assign_Random_Choices();
					Send_Lobby_Options(true, true);
					{
						GlobalPacketType packet;
						memset(&packet, 0, sizeof(packet));
						packet.Command = NET_COMMAND_16;
						strcpy(packet.Name, Session.Handle);
						Get_Slot_Names(packet.Slots.Names);
						int players = Session.Players.Count();
						for (i = 1; i < players; i++) {
							Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address);
							Call_Back();
						}
					}
					mode->vt_entry_54();
					if (MultiplayerMapPreview != 0) {
						delete MultiplayerMapPreview;
						MultiplayerMapPreview = 0;
					}
					PregameSetup();
					Session.FrameSendRate = 5;
					Session.LatencyFudge = 0;
					Session.PrecalcMaxAhead = 0;
					Session.PrecalcDesiredFrameRate = 0;
					if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
						Session.MaxAhead = ((Ipx.Global_Response_Time() / 8 + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate > Session.FrameSendRate * 2 ? ((Ipx.Global_Response_Time() / 8 + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate : Session.FrameSendRate * 2;
					} else {
						Session.MaxAhead = (int)Ipx.Global_Response_Time() / 8 > 2 ? (int)Ipx.Global_Response_Time() / 8 : 2;
					}
					Ipx.Set_Timing(30 > Ipx.Global_Response_Time() + 2 ? 30 : Ipx.Global_Response_Time() + 2, -1, 600, true);

				/*
				 * Not the GO button -- there is nothing to do this pass, so reset
				 * the response and fall back through the main message loop.
				 */
					GlobalPacketType packet;
					memset(&packet, 0, sizeof(packet));
					packet.Command = NET_GO;
					packet.Progress.Percent = Session.MaxAhead;   // NET_GO carries MaxAhead at +0x2F
					for (i = 1; i < Session.Players.Count(); i++) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address);
					}

					CDTimerClass<SystemTimerClass> timeout = 1200;
					while (Ipx.Global_Num_Send() > 0 && !timeout) {
						Call_Back();
					}

					int responses[8];
					memset(responses, 0, sizeof(responses));
					int num_responses = 0;
					bool send_scenario = false;
					DebugString("About to wait for 'GO' response.\n");
				CDTimerClass<SystemTimerClass> response_timer;    // timeout timer for waiting for responses
				response_timer = 600;
					do {
						Call_Back();
					if (Ipx.Get_Global_Message(&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID)
							&& Session.GProductID == 0xAA03) {
							for (i = 1; i < Session.Players.Count(); i++) {
								if (Session.Players[i]->Address == Session.GAddress) {
									if (!responses[i]) {
										if (Session.GPacket.Command == NET_REQ_SCENARIO) {
											DebugString("Received REQ_SCENARIO packet.\n");
											responses[i] = Session.GPacket.Command;
											send_scenario = true;
											num_responses++;
										}
										if (Session.GPacket.Command == NET_READY_TO_GO) {
											DebugString("Received READY_TO_GO packet.\n");
											responses[i] = Session.GPacket.Command;
											num_responses++;
										}
									}
								}
							}
						}
					} while (num_responses < Session.Players.Count() - 1 && response_timer);

					WS_Destroy_Dialog(0, 0);
					Darken_Surface(NULL, true);
					if (send_scenario) {
						memset(Session.ScenarioRequests, 0, sizeof(Session.ScenarioRequests));
						Session.RequestCount = 0;
						for (i = 1; i < Session.Players.Count(); i++) {
							if (responses[i] == 0x10) {
								Session.ScenarioRequests[Session.RequestCount] = i;
								Session.RequestCount++;
							}
						}
						Send_Remote_File(Scen->ScenarioName, 3, false, true);
					}
					Ipx.Set_Timing(Ipx.Global_Response_Time() + 2 > 30 ? Ipx.Global_Response_Time() + 2 : 30,
						-1, 120 > Ipx.Global_Response_Time() * 8 ? 120 : Ipx.Global_Response_Time() * 8, true);
					break;
				}
			}
		}
		_netresponse = 0;
	}

	Session.NetOpen = false;
	Session.Write_MultiPlayer_Settings();
	return(true);
}


bool Net2ReadyToGo(int load_game)
{
	Ipx.Set_Timing(Ipx.Global_Response_Time() + 2 > 30 ? Ipx.Global_Response_Time() + 2 : 30, -1, 1000, true);
	if (Find_Local_Scenario(Session.ScenarioFileName, Session.ScenarioFileLength, Session.ScenarioDigest, Session.ScenarioIsOfficial, true) == true) {
		DebugString("Found local scenario, file name is %d\n", Session.ScenarioFileName);
		GlobalPacketType packet;
		memset(&packet, 0, sizeof(packet));
		packet.Command = NET_READY_TO_GO;
		Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.HostAddress);
		while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) {
			Call_Back();
		}

	} else {
		//.....................................................................
		// If the other guys are playing a scenario I don't have (sniff), I can't
		// play.  Try to bail gracefully.
		//.....................................................................
		DebugString("Failed to find local scenario, file name is %d\n", Session.ScenarioFileName);
		if ((Session.ScenarioIsOfficial && !Session.Is_Random_Map()) || !Get_File_From_Host(Session.ScenarioFileName, 3, true)) {
			Session.Options.ScenarioIndex = -1;
			WWMessageBox(L" ").Process(StringTable::LoadString("TXT_UNABLE_PLAY_WAAUGH", NULL, "D:\\Ra2\\netdlg2.cpp", 678),
				StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\netdlg2.cpp", 678), NULL, NULL, false);
			GlobalPacketType packet;
			memset(&packet, 0, sizeof(packet));
			packet.Command = NET_SIGN_OFF;
			strcpy(packet.Name, Session.Handle);
			for (int i = 1; i < Session.Chat.Count(); i++) {
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Chat[i]->Address);
				Call_Back();
			}
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL);
			if (Session.IsBridge) {
				Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
			}
			while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) {
				Call_Back();
			}
			return(false);
		}
	}

	//---------------------------------------------------------------------
	// Prepare to load the scenario.
	//---------------------------------------------------------------------
	strcpy(Scen->ScenarioName, Session.ScenarioFileName);
	int response = Ipx.Global_Response_Time();
	if (response < 20) {
		DebugString("IPX.Global_Response_Time() == %d. Adjusting to 20\n", response);
		response = 20;
	}
	Ipx.Set_Timing(response, -1, response * 8 < 120 ? 120 : response * 8, true);
	Ipx.Set_External_Timing(60, -1, 600);
	return(true);
}


/// <summary>
/// Handles the multiplayer guest dialog.
/// This is the setup dialog a player works in after joining somebody else's game. The
/// guest picks a side and a color here, chats with the rest of the players, and tells the
/// host when it is happy for the game to begin.
/// </summary>
/// <returns>Returns with TRUE if the message was consumed by this dialog.</returns>
LRESULT CALLBACK MPlayer_Game_List_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005BE6B0; the join dialog of the LAN lobby, ra2-decomp's twin text
{
	switch (message) {
		case OD_SUBCLASSED: { // RA2: the dialog's set-up message
		CurGame = 0;
			Net2JoinListEnabled = true;
			Update_Observer_Buttons(window);

			SendDlgItemMessage(window, IDC_YOURNAME, EM_SETLIMITTEXT, 16, 0);
			SendDlgItemMessage(window, IDC_YOURNAME, 0x4eb, 0, (LPARAM)L",");
			SendDlgItemMessage(window, IDC_YOURNAME, 0x4d1, 1, 0);
			wchar_t * handle = Narrow_To_Wide(Session.Handle);
			HWND edit = GetDlgItem(window, IDC_YOURNAME);
			SendMessage(edit, OD_SETTEXTW, 0, (LPARAM)handle);

			Session.Options.ScenarioDescription[0] = 0;
			int color = Session.Color;
			int color2 = Session.Color2;
			Game::PlayerColor = color;
			Session.ColorIdx = color2;

		Clear_Vector(&Session.Games);
		Clear_Vector(&Session.Players);
		Clear_Vector(&Session.Chat);

			NodeNameType * who = new NodeNameType;
			wcscpy(who->Name, Narrow_To_Wide(Session.Handle));
			who->Chat.LastTime = 0;
			who->Chat.LastChance = 0;
			who->Chat.Color = Session.GPacket.Chat.Color;
			Session.Chat.Add(who);

			NodeNameType * game = new NodeNameType;
			wcscpy(game->Name, L"");
			game->Game.IsOpen = 0;
			game->Game.LastTime = 0;
			Session.Games.Add(game);

			Send_Join_Queries(1, 0, 1, 1);
		Net2DisplayGameList();
			Net2DisplayUsersFull(false);
			Dialog_On_WM_PAINT(window);
			SetFocus(GetDlgItem(window, IDC_INPUT));
		return(0);
	}

	case WM_COMMAND:
		switch (LOWORD(wparam)) {
			case IDC_YOURNAME: { // the name edit
				if (!IsWindowEnabled(window)) {
					return(0);
				}
				if (_netresponse != 0) {
					return(0);
				}
				wchar_t name[20];
				name[0] = 0;
				HWND edit = GetDlgItem(window, IDC_YOURNAME);
				SendMessage(edit, 0x4b3, 20, (LPARAM)name);
				wchar_t first = name[0];
				wchar_t * p = name;
				while (first != 0 && *p == L' ') {
					p++;
					if (*p == 0) {
						break;
					}
				}
				if (*p == 0 && first != 0) {
					name[0] = 0;
					edit = GetDlgItem(window, IDC_YOURNAME);
					SendMessage(edit, OD_SETTEXTW, 0, (LPARAM)name);
				}
				if (strcmp(Wide_To_Narrow(name), Session.Handle) != 0) {
					strcpy(Session.Handle, Wide_To_Narrow(name));
					Send_Join_Queries(0, 0, 1, 0);
					Net2DisplayUsersFull(false);
				}
				return(0);
			}

			case IDC_GAMELIST: { // the game list
				if (JoinState > JOIN_NOTHING) {
					return(0);
				}
				int old_game = CurGame;
				int sel = SendDlgItemMessage(window, IDC_GAMELIST, LB_GETCURSEL, 0, 0);
				if (sel >= 0 && Net2JoinListEnabled) {
					CurGame = sel;
					wcscpy(Session.GameName, Session.Games[sel]->Name);
				}
				if (HIWORD(wparam) == LBN_SELCHANGE) {
					Clear_Vector(&Session.Players);
					if (old_game != CurGame) {
						Send_Join_Queries(1, 1, 1, 0);
					}
					Net2DisplayUsersFull(false);
					return(0);
			}
				if (HIWORD(wparam) == LBN_DBLCLK) {
					_netresponse = (ObserverMode == 1) ? 0x6ce : 0x6cd;
				return(0);
			}
			return(0);
			}

			case IDC_INPUT: { // the chat line
				Limit_Name_Text((HWND)lparam);
				if (HIWORD(wparam) == EN_MAXTEXT) {
					Send_Chat_Message(window, ColorMe, ColorUser);
				}
				return(0);
			}

			case IDC_CANCEL:
				_netresponse = IDC_CANCEL;
				Save_Observer_Mode();
			return(0);

			case 0x6cc:
				_netresponse = LOWORD(wparam);
				Net2ObserverMode = (ObserverMode == 1);
				return(0);

			case 0x6cd:
			case 0x6ce: {
				EnableWindow(window, FALSE);
				HWND name = GetDlgItem(window, IDC_YOURNAME);
				if (name) {
					EnableWindow(name, FALSE);
			}
				_netresponse = LOWORD(wparam);
			return(0);
		}

			case 0x75f:
			case 0x760: { // the observer radio buttons
				int state = 0;
				HWND item = GetDlgItem(window, 0x760);
				if (item) {
					state = SendMessage(item, BM_GETCHECK, 0, 0);
			}
				if (LOWORD(wparam) == 0x75f) {
					state = (state == BST_UNCHECKED);
			}
				ObserverMode = state;
				Update_Observer_Buttons(NULL);
			return(0);
		}
		}
		return(0);

	case WM_MOUSEMOVE: { // the tool tip of the control under the cursor
		HWND tip = GetDlgItem(window, 0x695);
		if (tip == NULL) {
			return(0);
		}
		POINT pt;
		pt.x = LOWORD(lparam);
		pt.y = HIWORD(lparam);
		RECT rect;
		GetWindowRect(window, &rect);
		pt.x -= rect.left;
		pt.y -= rect.top;
		wchar_t const * text = L"";
		HWND child = ChildWindowFromPointEx(window, pt, CWP_SKIPINVISIBLE);
		if (child) {
			char const * label = OwnerDraw::Control_Tooltip_Label((unsigned int)window, child);
			if (label) {
				text = StringTable::LoadString(label, NULL, "D:\\Ra2\\netdlg2.cpp", 2285);
			}
		}
		SendMessage(tip, OD_SETTEXTW, 0, (LPARAM)text);
		return(0);
	}

	case 0x4db:
		Focus_Next_Tab_Item(lparam, wparam != 0);
		return(0);

	case 0x4ec:
		Dialog_On_OD_4EC(window);
		return(0);

	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(1);

	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(0);

	case WM_ERASEBKGND:
		return(1);
	}

	return(0);
}


void Send_Chat_Message(HWND dialog, int color, int recipients) // RA2: 0x005BEE20; sends the text of the chat line to the lobby
{
	wchar_t text[112];

	HWND edit = GetDlgItem(dialog, IDC_INPUT);
	SendMessage(edit, 0x4B3, 0x6F, (LPARAM)text);
	text[0x6F] = 0;
	int length = wcslen(text);
	edit = GetDlgItem(dialog, IDC_INPUT);
	SendMessage(edit, OD_SETTEXTW, 0, (LPARAM)L"");
	InvalidateRect(dialog, NULL, 1);
	if (length > 2) {
		Lobby_Chat_Printf(color, L"[%s] %s", Narrow_To_Wide(Session.Handle), text);
		GlobalPacketType packet;
		memset(&packet, 0, sizeof(packet));
		packet.Command = NET_MESSAGE;
		strcpy(packet.Name, Session.Handle);
		wcscpy((wchar_t *)packet.Message.Buf, text);
		packet.Message.Color = recipients;
		packet.Message.NameCRC = Compute_Name_CRC(Session.GameName);
		if (JoinState == JOIN_CONFIRMED) {
			int count = Session.Players.Count();
			for (int i = 1; i < count; i++) {
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address);
				Call_Back();
			}
		} else {
			int count = Session.Chat.Count();
			for (int i = 1; i < count; i++) {
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Chat[i]->Address);
				Call_Back();
			}
		}
	}
}


#ifdef RA2_NON_MATCHING // not exact: 61 of 2507 bytes differ (image-derived bound 005befe0-005bf9ab): ra2-decomp's twin text (Yuri's Revenge 0x005DE520) with RA2's statements; message and lparam take each other's registers (ebx, edi) and nothing else differs; the pair turns with lparam's use in the set-up case's Options_Dialog_Proc call and with the order of pt.x/pt.y (lane F33, 2026-11-23); RobinDigitalPro's 1.001 text of the function (src/ra2/netdlg2.cpp Host_Dialog_Proc: positive tests that return inside each case, a named command id, `message` in the set-up call) compiles to these same bytes (lane F103)
LRESULT CALLBACK MPlayer_Host_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005BEFE0; the lobby dialog of the host
{
	if (message != OD_SUBCLASSED) {
		Options_Dialog_Proc(window, message, wparam, lparam);
	}

	switch (message) {
		case WM_NCHITTEST: { // RA2: the tool tip of the control under the cursor
			HWND tip = GetDlgItem(window, 0x695);
			if (tip == NULL) {
				break;
				}
			POINT pt;
			pt.x = LOWORD(lparam);
			pt.y = HIWORD(lparam);
			RECT rect;
			GetWindowRect(window, &rect);
			pt.x -= rect.left;
			pt.y -= rect.top;
			wchar_t const * text = L"";
			HWND child = ChildWindowFromPointEx(window, pt, CWP_SKIPINVISIBLE);
			if (child) {
				char const * label = OwnerDraw::Control_Tooltip_Label((unsigned int)window, child);
				if (label) {
					text = StringTable::LoadString(label, NULL, "D:\\Ra2\\netdlg2.cpp", 2704);
					}
				}
				SendMessage(tip, OD_SETTEXTW, 0, (LPARAM)text);
			break;
			}

		case WM_DRAWITEM:
			Dialog_On_WM_DRAWITEM(lparam);
			return(1);

		case WM_PAINT:
			Dialog_On_WM_PAINT(window);
			if (MultiplayerMapPreview) {
				MultiplayerMapPreview->Blit_Preview(window);
			}
			if (Progress.Has_Dialog()) {
				Progress.Display_Progress();
			}
			Slide_Pending_Dialog(window);
			ValidateRect(window, NULL);
			break;

		case WM_DESTROY:
			if (Progress.Has_Dialog()) {
				Progress.Clear_Dialog();
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case IDCANCEL:
					if (Net2OptionsLocked) {
						break;
					}
					if (MultiplayerMapPreview) {
						delete MultiplayerMapPreview;
						MultiplayerMapPreview = NULL;
					}
					Save_Lobby_Options(window, &Session.LAN);
					_netresponse = IDCANCEL;
					break;

				case 0x411: case 0x412: case 0x413: case 0x414: case 0x415: case 0x416: case 0x417: case 0x418:
					if (HIWORD(wparam) != 1) {
						break;
					}
					if (Net2OptionsLocked) {
						break;
					}
					if (Is_Host()) {
						int node = Combo_Selected_Data(window, LOWORD(wparam), -1);
						Change_Slot_Type(window, LOWORD(wparam), (NodeNameType *)node, 2);
					}
					break;

				case 0x540:
					Limit_Name_Text((HWND)lparam);
					if (HIWORD(wparam) == EN_MAXTEXT) {
						Send_Chat_Message(window, ColorMe, ColorUser);
					}
					break;

				case 0x588:
					EnableWindow(GetDlgItem(window, 0x588), FALSE);
					Net2OptionsLocked = true;
					Save_Lobby_Options(window, &Session.LAN);
					_netresponse = 0x588;
					break;

				case 0x510: case 0x513: case 0x514:
				case 0x51e: case 0x51f: case 0x520: case 0x521:
				case 0x6a1:
					if (HIWORD(wparam) == 1 && !Net2OptionsLocked) {
						Set_Slot_Country(window, LOWORD(wparam));
						Send_Lobby_Options(true, false);
						Net2DisplayUsersFull(false);
					}
					break;

				case 0x522: case 0x523: case 0x524: case 0x525: case 0x526: case 0x527: case 0x528:
				case 0x6a2:
					if (HIWORD(wparam) == 1 && !Net2OptionsLocked) {
						Set_Slot_Color(window, LOWORD(wparam));
						Send_Lobby_Options(true, false);
						Net2DisplayUsersFull(false);
					}
					break;

				case 0x5aa: { // RA2: choose another map
					if (Net2OptionsLocked) {
						break;
					}
					int scenario = Session.Options.ScenarioIndex;
					bool was_skirmish = (Session.MPGameMode == NULL) || Session.MPGameMode->IsSkirmishAllowed();
					bool was_observer = (Session.MPGameMode == NULL) || Session.MPGameMode->vt_entry_A0();
					OwnerDraw::WS_Save_Dialog_Values(window);
					ShowWindow(window, SW_HIDE);
					IsRandomMap = false;
					if (Scenario_Dialog(MainWindow) == 2) {
						Session.Options.ScenarioIndex = scenario;
						Set_Scenario_Info_From_Index(scenario);
						Update_Network_Dialog_Preview(window);
						IsRandomMap = true;
						ShowWindow(window, SW_SHOW);
						Load_Random_Map_Preview(window);
						Send_Lobby_Options(true, false);
						InvalidateRect(window, NULL, FALSE);
						break;
					}
					if (!Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex)) {
						Session.Options.ScenarioIndex = scenario;
					} else {
						Change_Scenario_Slots(window, scenario, Session.Options.ScenarioIndex);
					}
					IsRandomMap = true;
					ShowWindow(window, SW_SHOW);
					Set_Skirmish_Dialog_Text(window);
					Load_Random_Map_Preview(window);
					bool is_skirmish = (Session.MPGameMode == NULL) || Session.MPGameMode->IsSkirmishAllowed();
					bool is_observer = (Session.MPGameMode == NULL) || Session.MPGameMode->vt_entry_A0();
					if (was_observer && !is_observer) {
						Clear_Observer_Slots(window);
					}
					if (was_skirmish && !is_skirmish && Reset_AI_Slots(window)) {
						Session.Options.AIPlayers = 0;
						Send_Slot_Names();
						Net2DisplayUsersFull(false);
						Send_Lobby_Options(true, false);
						break;
					}
					Net2DisplayUsersFull(true);
					break;
				}

				default:
					return(0);
			}
			break;

		case OD_SUBCLASSED: {
			VerNum.Init_Clipping();
			srand(NonCriticalRandomNumber(1, 0x7fff));
			Seed = rand();
			Center_Window_Within_Window(window);
			Fill_Side_Combos(window);
			Init_Color_Entries();
			Fill_Color_Combos(window);
			Select_Player_Side(window, Session.LocalSlot);
			Select_Player_Color(window, Session.LocalSlot);
			Net2DisplayUsersFull(false);
			Load_Lobby_Options(window, &Session.LAN);
			if (Session.MPGameMode && !Session.MPGameMode->vt_entry_A0()) {
				LobbyObserverSlot = -1;
				ObserverMode = 0;
				Update_Observer_Buttons(NULL);
				Session.Slots[0]->Slot.SpectatorFlag = 0;
				Session.Slots[0]->Set_Country(Session.MPGameMode->DefaultCountry());
				Session.Slots[0]->Set_Color(-2);
				Fill_Color_Combos(window);
			}
			Update_Slot_Controls(window, 0, true);
			Change_Scenario_Slots(window, -1, Session.Options.ScenarioIndex);
			if (LobbyObserverSlot == 0 && Session.MPGameMode && Session.MPGameMode->vt_entry_A0()) {
				Session.Slots[0]->Set_Country(-3);
				Session.Slots[0]->Slot.SpectatorFlag = -1;
			}
			Refresh_Slot_Images(window);
			Set_Item_Text(window, Session.MPGameMode);
			Set_Skirmish_Dialog_Text(window);
			Load_Random_Map_Preview(window);
			Options_Dialog_Proc(window, OD_SUBCLASSED, wparam, lparam);
			Net2DisplayGameList();
			break;
			}

		case WM_HSCROLL:
		case WM_VSCROLL:
			if (Net2OptionsLocked) {
				break;
			}
			Session.Options.GameSpeed = 6 - SendDlgItemMessage(window, IDC_GAME_SPEED_SLIDER, TBM_GETPOS, 0, 0);
			Session.Options.Credits = SendDlgItemMessage(window, IDC_CREDITS, TBM_GETPOS, 0, 0);
			Session.Options.UnitCount = SendDlgItemMessage(window, IDC_UNITCOUNT, TBM_GETPOS, 0, 0);
			break;

		case 0x49b: { // RA2: the text of the edit control that was asked for
			HWND item = GetDlgItem(window, wparam);
			SendMessage(item, 0x4b3, 0x7f, lparam);
			return(0);
			}

		default:
			return(0);

		case 0x4ec:
			Dialog_On_OD_4EC(window);
			break;

		case 0x4e9: { // RA2: the tool tip text of a combo box item
			ToolTipTextRequest * request = (ToolTipTextRequest *)lparam;
			request->Text = (wchar_t const *)NULL;
			if (request->Window && request->Item != -1) {
				int id = GetDlgCtrlID(request->Window);
				if (Control_Slot(id) == -1) {
					if (Side_Combo_Player(id) == -1) {
						if (Color_Combo_Player(id) != -1) {
							request->Text = Color_Name(Color_Combo_Data(window, id, request->Item));
						}
					} else {
						int side = Side_Combo_Data(window, id, request->Item);
						if (side != -1) {
							request->Text = Side_Name(side);
						}
					}
				} else {
					request->Text = Entry_Type_Name((NodeNameType *)Combo_Selected_Data(window, id, request->Item));
				}
			}
			}

		case WM_ERASEBKGND:
			return(1);
	}

	return(0);
}
#endif


LRESULT CALLBACK Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005BF9B0; the game options of the lobby dialog
{
	if (!Net2OptionsLocked) {
		switch (message) {
			case OD_SUBCLASSED:
				Fill_Options_Dialog(window, 1);
			return(0);
			case WM_HSCROLL:
				if (GetDlgItem(window, IDC_GAME_SPEED_SLIDER) == (HWND)lparam) {
					Session.Options.GameSpeed = 6 - SendDlgItemMessage(window, IDC_GAME_SPEED_SLIDER, TBM_GETPOS, 0, 0);
			}
				if (GetDlgItem(window, IDC_CREDITS) == (HWND)lparam) {
					Session.Options.Credits = SendDlgItemMessage(window, IDC_CREDITS, TBM_GETPOS, 0, 0);
			}
				if (GetDlgItem(window, IDC_UNITCOUNT) == (HWND)lparam) {
					Session.Options.UnitCount = SendDlgItemMessage(window, IDC_UNITCOUNT, TBM_GETPOS, 0, 0);
			}
			return(0);
			case WM_COMMAND:
				switch ((unsigned short)wparam) {
					case IDC_SHORT_GAME:
				Session.Options.ShortGame = false;
						if (SendDlgItemMessage(window, IDC_SHORT_GAME, BM_GETCHECK, 0, 0) == BST_CHECKED) {
					Session.Options.ShortGame = true;
					if (!Session.Options.Bases) {
						Session.Options.Bases = true;
								SendDlgItemMessage(window, IDC_BASES, BM_SETCHECK, BST_CHECKED, 0);
					}
				}
				return(0);
					case IDC_CRATES:
				Session.Options.Goodies = false;
						if (SendDlgItemMessage(window, IDC_CRATES, BM_GETCHECK, 0, 0) == BST_CHECKED) {
					Session.Options.Goodies = true;
				}
				return(0);
					case IDC_REDEPLOY_MCV:
				Session.Options.MCVRedeploy = false;
						if (SendDlgItemMessage(window, IDC_REDEPLOY_MCV, BM_GETCHECK, 0, 0) == BST_CHECKED) {
					Session.Options.MCVRedeploy = true;
				}
				return(0);
					case IDC_BRIDGE_DESTROY:
				Session.Options.BridgeDestruction = false;
						if (SendDlgItemMessage(window, IDC_BRIDGE_DESTROY, BM_GETCHECK, 0, 0) == BST_CHECKED) {
					Session.Options.BridgeDestruction = true;
				}
				return(0);
					case 0x69D:
						Session.Options.BuildOffAlly = false;
						if (SendDlgItemMessage(window, 0x69D, BM_GETCHECK, 0, 0) == BST_CHECKED) {
							Session.Options.BuildOffAlly = true;
				}
				return(0);
					case 0x69A:
						Session.Options.SWAllowed = false;
						if (SendDlgItemMessage(window, 0x69A, BM_GETCHECK, 0, 0) == BST_CHECKED) {
							Session.Options.SWAllowed = true;
				}
				return(0);
			}
		}
	}
	return(0);
}


/***********************************************************************************************
 * Request_To_Join -- Sends a JOIN request packet to game owner                                *
 *                                                                                             *
 * Regardless of the return code, the Join Dialog will need to be redrawn after calling this   *
 * routine.                                                                                    *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      1 = Packet sent, 0 = wasn't                                                            *
 *=============================================================================================*/
int Request_To_Join(int join_index, bool observer)
{
	if (join_index < 1) {
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_MUST_SELECT_GAME", NULL, "D:\\Ra2\\netdlg2.cpp", 3423));
		VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL);
		return(0);
	}
	if (Session.Games.Count() <= 1 || join_index > Session.Games.Count()) {
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_NOTHING_TO_JOIN", NULL, "D:\\Ra2\\netdlg2.cpp", 3430));
		VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL);
		return(0);
	}
	if (strlen(Session.Handle) == 0) {
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_NAME_ERROR", NULL, "D:\\Ra2\\netdlg2.cpp", 3438));
		VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL);
		return(0);
	}
	if (strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[0])) == 0
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[1])) == 0
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[2])) == 0
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[3])) == 0
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[4])) == 0
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[5])) == 0
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[6])) == 0
		|| Is_Lobby_Name(Session.Handle)) {
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("ERROR:NameIsReservedWord", NULL, "D:\\Ra2\\netdlg2.cpp", 3453));
		VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL);
		return(0);
	}
	if (!Session.Games[join_index]->Game.IsOpen) {
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_GAME_IS_CLOSED", NULL, "D:\\Ra2\\netdlg2.cpp", 3461));
		VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL);
		return(0);
	}
	GlobalPacketType packet;
	memset(&packet, 0, sizeof(packet));
	packet.Command = NET_QUERY_JOIN;
	strcpy(packet.Name, Session.Handle);
	strcpy(packet.Serial, SerialNumber);
	packet.PlayerInfo.House = Session.Chosen_House();
	packet.PlayerInfo.Color = Session.Chosen_Color();
	packet.PlayerInfo.MinVersion = VerNum.Min_Version();
	packet.PlayerInfo.MaxVersion = VerNum.Max_Version();
	packet.PlayerInfo.CheatCheck = RulesID;
	packet.PlayerInfo.ArtCheatCheck = ArtID;
	packet.PlayerInfo.AICheatCheck = AIID;
	packet.PlayerInfo.BuildNumber = Build_Number();
	packet.PlayerInfo.Flag = observer;
	Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Games[join_index]->Address);
	return(1);
}


/***********************************************************************************************
 * Unjoin_Game -- Sends a SIGN_OFF to the game's owner and the other players                   *
 *=============================================================================================*/
void Unjoin_Game(int game_index)
{
	GlobalPacketType packet;
	memset(&packet, 0, sizeof(packet));
	packet.Command = NET_SIGN_OFF;
	strcpy(packet.Name, Session.Handle);
	for (int i = 1; i < Session.Players.Count(); i++) {
		Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address);
		Call_Back();
	}
	if (JoinState == JOIN_WAIT_CONFIRM || JoinState == JOIN_CONFIRMED) {
		if (!Session.Games[game_index]->Address.Is_Broadcast()) {
			Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Games[game_index]->Address);
		}
	}
	Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL);
	Clear_Slot_Lists(); // RA2: the lobby's slots are cleared
	Fill_Slots();
	Clear_Vector(&Session.Players);
	Session.GameName[0] = 0;
}


/***********************************************************************************************
 * Send_Join_Queries -- sends queries for the Join Dialog                                      *
 *=============================================================================================*/
static void Send_Join_Queries(int gamenow, int playernow, int chatnow, int init)
{
	static CDTimerClass<SystemTimerClass> gametimer;
	static CDTimerClass<SystemTimerClass> playertimer;
	static CDTimerClass<SystemTimerClass> chattimer;
	GlobalPacketType packet;

	if (init) {
		gametimer = 120;
		playertimer = 35;
		chattimer = 83;
	}

	if (!gametimer || gamenow) {
		gametimer = 120;
		if (WS_Top_Window_ID() != IDD_MPLAYER_HOST || gamenow) {
			memset(&packet, 0, sizeof(packet));
			packet.Command = NET_QUERY_GAME;
			strcpy(packet.Name, Session.Handle);
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL);
			if (Session.IsBridge) {
				Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
			}
		}
	}

	if ((CurGame > 0 && CurGame < Session.Games.Count() && !playertimer) || playernow) {
		playertimer = 35;
		memset(&packet, 0, sizeof(packet));
		packet.Command = NET_QUERY_PLAYER;
		strcpy(packet.Name, Wide_To_Narrow(Session.Games[CurGame]->Name));
		Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL);
		if (Session.IsBridge) {
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
		}
	}

	if ((!chattimer && JoinState < JOIN_CONFIRMED) || chatnow) {
		chattimer = 83;
		memset(&packet, 0, sizeof(packet));
		packet.Command = NET_CHAT_ANNOUNCE;
		strcpy(packet.Name, Session.Handle);
		packet.Chat.ID = Session.UniqueID;
		packet.Chat.Color = Session.PrefColor;
		Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL);
		if (Session.IsBridge) {
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
		}
	}
}


/// <summary>
/// Answers the queries other machines send to this one.
/// </summary>
bool Process_Global_Packet(GlobalPacketType * packet, IPXAddressClass * address)
{
	if (packet->Command == NET_QUERY_GAME && !Session.NetStealth) {
		if (wcslen(Session.GameName) > 0) {
			if (!Session.NetOpen || _stricmp(Session.Handle, Wide_To_Narrow(Session.GameName)) == 0) {
				if (_stricmp(Session.Handle, Wide_To_Narrow(Session.GameName)) != 0 && !ScenarioActive) {
					return(true);
				}
				GlobalPacketType answer;
				memset(&answer, 0, sizeof(answer));
				answer.Command = NET_ANSWER_GAME;
				strcpy(answer.Name, Wide_To_Narrow(Session.GameName));
				answer.GameInfo.IsOpen = Session.NetOpen;
				Ipx.Send_Global_Message(&answer, sizeof(answer), 1, address);
			}
		}
		return(true);
	}
	if (packet->Command == NET_QUERY_PLAYER) {
		if (_stricmp(packet->Name, Wide_To_Narrow(Session.GameName)) == 0
			&& wcslen(Session.GameName) > 0 && !Session.NetStealth && JoinState >= JOIN_CONFIRMED) {
			GlobalPacketType answer;
			memset(&answer, 0, sizeof(answer));
			answer.Command = NET_ANSWER_PLAYER;
			strcpy(answer.Name, Session.Handle);
			strcpy(answer.Serial, SerialNumber);
			answer.PlayerInfo.House = Session.Chosen_House();
			answer.PlayerInfo.Color = Session.Chosen_Color();
			answer.PlayerInfo.NameCRC = Compute_Name_CRC(Session.GameName);
			answer.PlayerInfo.Slot = Session.LocalSlot;
			answer.PlayerInfo.Flag = Session.LocalSlot == LobbyObserverSlot;
			if (Session.Players.Count() == 0) {
				return(true);
			}
			if (Session.Players[0]->Slot.SpectatorFlag != -1) {
				answer.PlayerInfo.Flag = 0;
			}
			Ipx.Send_Global_Message(&answer, sizeof(answer), 1, address);
			if (_stricmp(packet->Name, Wide_To_Narrow(Session.Players[0]->Name)) == 0) {
				Send_Lobby_Options(true, true);
				if (Session.MPGameMode) {
					Session.MPGameMode->vt_entry_50();
				}
			}
			return(true);
		}
	}
	return(false);
}


void Get_Join_Responses(void)
{
	NodeNameType * who;
	int i;
	int found;
	int why;
	wchar_t txt[80];
	bool display_users = false;
	bool display_games = false;

	while (_netresponse == 0) {
		Call_Back();
		if (!Ipx.Get_Global_Message(&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID)) {
			break;
		}
		if (Session.GProductID != 0xAA03) {
			continue;
		}
		if (Process_Global_Packet(&Session.GPacket, &Session.GAddress)) {
			continue;
		}

		// The commands of TibSun's Get_Join_Responses, in its order (RA2 adds the map preview,
		// slot and observer packets).
		if (Session.GPacket.Command == NET_PREVIEW_MODE) {
			if (WS_Top_Window_ID() == IDD_MPLAYER_GUEST) {
				Receive_Random_Map_Preview();
			}
		} else if (Session.GPacket.Command == NET_REQ_PREVIEW) {
			if (WS_Top_Window_ID() == IDD_MPLAYER_HOST) {
				Enable_Start_Button();
				display_users = true;
				Send_Map_Preview(MultiplayerMapPreview);
			}
		} else if (Session.GPacket.Command == NET_ANSWER_GAME) {
			// NET_ANSWER_GAME
			found = 0;
			for (i = 1; i < Session.Games.Count(); i++) {
				if (!_stricmp(Wide_To_Narrow(Session.Games[i]->Name), Session.GPacket.Name)) {
					found = 1;
					Session.Games[i]->Game.LastTime = TickCount();
					if (Session.Games[i]->Game.IsOpen != Session.GPacket.GameInfo.IsOpen) {
						display_games = true;
						Session.Games[i]->Game.IsOpen = Session.GPacket.GameInfo.IsOpen;
						if (Session.Games[i]->Game.IsOpen) {
							Session.Games[i]->Address = Session.GAddress;
						}
						if (JoinState < JOIN_CONFIRMED) {
							if (Session.Games[i]->Game.IsOpen) {
								swprintf(txt, StringTable::LoadString("TXT_S_FORMED_NEW_GAME", NULL, "D:\\Ra2\\netdlg2.cpp", 3940), Narrow_To_Wide(Session.GPacket.Name));
								VocClass::PlayGlobal(Rule->GameForming, 1.0f, 0x2000, 0);
							} else {
								swprintf(txt, StringTable::LoadString("TXT_GAME_NOW_IN_PROGRESS", NULL, "D:\\Ra2\\netdlg2.cpp", 3945), Narrow_To_Wide(Session.GPacket.Name));
								VocClass::PlayGlobal(Rule->GameClosed, 1.0f, 0x2000, 0);
							}
							Lobby_Chat_Printf(ColorSystem, txt);
						}
					}
					break;
				}
			}
			if (found == 0) {
				who = new NodeNameType;
				wcscpy(who->Name, Narrow_To_Wide(Session.GPacket.Name));
				who->Address = Session.GAddress;
				who->Game.IsOpen = Session.GPacket.GameInfo.IsOpen;
				who->Game.LastTime = TickCount();
				Session.Games.Add(who);
				for (i = 1; i < Session.Chat.Count(); i++) {
					if (Session.Chat[i]->Address == Session.GAddress) {
						delete Session.Chat[i];
						Session.Chat.Delete(Session.Chat[i]);
						break;
					}
				}
				if (Session.GPacket.GameInfo.IsOpen && JoinState < JOIN_CONFIRMED) {
					swprintf(txt, StringTable::LoadString("TXT_S_FORMED_NEW_GAME", NULL, "D:\\Ra2\\netdlg2.cpp", 3986), Narrow_To_Wide(Session.GPacket.Name));
					Lobby_Chat_Printf(ColorSystem, txt);
					VocClass::PlayGlobal(Rule->GameForming, 1.0f, 0x2000, 0);
				}
				display_users = true;
				display_games = true;
			}
		} else if (Session.GPacket.Command == NET_ANSWER_PLAYER) {
			// NET_ANSWER_PLAYER
			found = 0;
			for (i = 0; i < Session.Players.Count(); i++) {
				if (Session.Players[i]->Address == Session.GAddress) {
					found = 1;
					break;
				}
			}
			if (Session.Games.Count() && Session.GPacket.PlayerInfo.NameCRC != (int)Compute_Name_CRC(Session.GameName)) {
				found = 1;
			}
			if (!_stricmp(Session.Handle, Session.GPacket.Name)) {
				found = 1;
			}
			if (found == 0) {
				who = new NodeNameType;
				memset(who, 0, sizeof(*who));
				wcscpy(who->Name, Narrow_To_Wide(Session.GPacket.Name));
				strcpy(who->Slot.SerialPad, Session.GPacket.Serial);
				who->Address = Session.GAddress;
				who->Set_Country(Session.GPacket.PlayerInfo.House);
				who->Set_Color(Session.GPacket.PlayerInfo.Color);
				if (Session.GPacket.PlayerInfo.Flag) {
					who->Slot.SpectatorFlag = -1;
				}
				Session.Players.Add(who);
				Set_Player_Slot(Session.GPacket.PlayerInfo.Slot, who);
				for (i = 1; i < Session.Chat.Count(); i++) {
					if (Session.Chat[i]->Address == Session.GAddress) {
						delete Session.Chat[i];
						Session.Chat.Delete(Session.Chat[i]);
						break;
					}
				}
				Enable_Start_Button();
				HWND dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST);
				Enable_Player_Combos(dialog, true);
				DebugString("New Player");
				display_users = true;
				if (JoinState >= JOIN_CONFIRMED) {
					VocClass::PlayGlobal(Rule->PlayerJoined, 1.0f, 0x2000, 0);
				}
			}
		} else if (Session.GPacket.Command == NET_COMMAND_16) {
			if (!Is_Host()) {
				Set_Slot_Names(Session.GPacket.Slots.Names);
				display_users = true;
			}
		} else if (Session.GPacket.Command == NET_COMMAND_28) {
			if (!Is_Host()) {
				Handle_Start_Reminder();
			}
		} else if (Session.GPacket.Command == NET_CONFIRM_JOIN) {
			// NET_CONFIRM_JOIN
			if (JoinState != JOIN_CONFIRMED) {
				wcscpy(Session.GameName, Narrow_To_Wide(Session.GPacket.Name));
				Session.Set_Side(Session.GPacket.PlayerInfo.House, false);
				Session.Set_Color(Session.GPacket.PlayerInfo.Color, false);
				JoinState = JOIN_CONFIRMED;
				Session.HostAddress = Session.GAddress;
				Clear_Vector(&Session.Players);
				who = new NodeNameType;
				memset(who, 0, sizeof(*who));
				wcscpy(who->Name, Narrow_To_Wide(Session.Handle));
				who->Set_Country(Session.Chosen_House());
				who->Set_Color(Session.Chosen_Color());
				LobbyObserverSlot = Session.GPacket.PlayerInfo.Flag;
				if (Session.GPacket.PlayerInfo.Slot == (int)Session.GPacket.PlayerInfo.Flag && Session.GPacket.PlayerInfo.House == -3) {
					who->Slot.SpectatorFlag = -1;
				}
				Session.Players.Add(who);
				Net2JoinListEnabled = false;
				WS_Destroy_Dialog(0, 0);
				_netresponse = 0;
				Clear_Slot_Lists();
				Reset_Slots();
				Seat_First_Player(Session.GPacket.PlayerInfo.Slot);
				HWND dialog = (HWND)WS_Create_Dialog(ProgramInstance, IDD_MPLAYER_GUEST, MainWindow, (DLGPROC)MPlayer_Guest_Dialog_Proc, 0);
				Show_Dialog(dialog, IDD_MPLAYER_GUEST);
				ShowWindow(dialog, SW_SHOWNORMAL);
				display_users = true;
				Send_Join_Queries(1, 1, 1, 0);
			}
		} else if (Session.GPacket.Command == NET_REJECT_JOIN) {
			// NET_REJECT_JOIN
			why = REJECT_DUPLICATE_NAME;
			if (JoinState == JOIN_CONFIRMED) {
				GlobalPacketType packet;
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_SIGN_OFF;
				strcpy(packet.Name, Session.Handle);
				for (i = 1; i < Session.Players.Count(); i++) {
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address);
					Call_Back();
				}
				Ipx.Send_Global_Message(&packet, sizeof(packet), 0, 0);
				if (Session.IsBridge) {
					Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
				}
				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) {
					Call_Back();
				}
				Session.GameName[0] = 0;
				Clear_Slot_Lists();
				Fill_Slots();
				Clear_Vector(&Session.Players);
				Clear_Vector(&Session.Chat);
				CurGame = 0;
				Net2JoinListEnabled = true;
				JoinState = JOIN_REJECTED;
				why = REJECT_BY_OWNER;
				display_users = true;
			} else if (JoinState == JOIN_WAIT_CONFIRM) {
				why = Session.GPacket.Reject.Why;
				JoinState = JOIN_REJECTED;
			}
			if (JoinState == JOIN_REJECTED) {
				Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_REQUEST_DENIED", NULL, "D:\\Ra2\\netdlg2.cpp", 4239));
				VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, 0);
				wchar_t const * item = 0;
				if (why == REJECT_DUPLICATE_NAME) {
					item = StringTable::LoadString("TXT_NAME_MUSTBE_UNIQUE", NULL, "D:\\Ra2\\netdlg2.cpp", 4245);
				} else if (why == REJECT_GAME_FULL) {
					if (ObserverMode == 0) {
						item = StringTable::LoadString("TXT_GAME_FULL", NULL, "D:\\Ra2\\netdlg2.cpp", 4247);
					} else {
						item = StringTable::LoadString("TXT_OBSERVER_FULL", NULL, "D:\\Ra2\\netdlg2.cpp", 4249);
					}
				} else if (why == REJECT_VERSION_TOO_OLD) {
					item = StringTable::LoadString("TXT_YOURGAME_OUTDATED", NULL, "D:\\Ra2\\netdlg2.cpp", 4251);
				} else if (why == REJECT_VERSION_TOO_NEW) {
					item = StringTable::LoadString("TXT_DESTGAME_OUTDATED", NULL, "D:\\Ra2\\netdlg2.cpp", 4253);
				} else if (why == REJECT_MISMATCH) {
					item = StringTable::LoadString("TXT_MISMATCH", NULL, "D:\\Ra2\\netdlg2.cpp", 4255);
				} else if (why == REJECT_DISBANDED) {
					item = StringTable::LoadString("TXT_GAME_CANCELLED", NULL, "D:\\Ra2\\netdlg2.cpp", 4257);
				} else if (why == REJECT_DUPLICATE_SERIAL) {
					item = StringTable::LoadString("TXT_SERIAL_DUP", NULL, "D:\\Ra2\\netdlg2.cpp", 4259);
				}
				if (item) {
					ODMessageBox(item, 0, Net2Callback, false);
				}
				if (WS_Top_Window_ID() == IDD_MPLAYER_GAME_LIST) {
					HWND window = WS_Top_Window();
					if (window) {
						EnableWindow(window, TRUE);
						HWND button = GetDlgItem(window, IDC_YOURNAME);
						if (button) {
							EnableWindow(button, TRUE);
						}
					}
				} else {
					_netresponse = IDCANCEL;
				}
				Send_Join_Queries(0, 0, 1, 0);
			}
		} else if (Session.GPacket.Command == NET_PUB_GAMEOPT) {
			// NET_PUB_GAMEOPT
			for (i = 0; i < Session.Players.Count(); i++) {
				if (!_stricmp(Wide_To_Narrow(Session.Players[i]->Name), Session.GPacket.Name)) {
					if (i != -1 && !Net2OptionsLocked) {
						Net2DecodeGameopt(Session.GPacket.Options.Buf, Session.GPacket.Name);
					}
					break;
				}
			}
		} else if (Session.GPacket.Command == NET_PRIV_GAMEOPT) {
			// NET_PRIV_GAMEOPT
			char * opts = strdup(&Session.GPacket.Options.Buf[1]);
			for (i = 0; i < 8; i++) {
				char * name = Wide_To_Narrow(Session.Slots[i]->Name);
				if (!_stricmp(name, Session.GPacket.Name)) {
					if (i != -1 && !Net2OptionsLocked) {
						char * tok = strtok(opts, ",");
						int newhouse = 0;
						if (tok) {
							newhouse = atol(tok);
						}
						tok = strtok(0, ",");
						int newcolor = 0;
						if (tok) {
							newcolor = atol(tok);
						}
						HWND dialog = WS_Find_Dialog(IDD_MPLAYER_HOST);
						if (!dialog) {
							dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST);
						}
						bool house_changed = Session.Slots[i]->Get_Country() != newhouse;
						if (house_changed) {
							Session.Slots[i]->Set_Country(newhouse);
						}
						bool color_changed = Session.Slots[i]->Get_Color() != newcolor;
						if (color_changed) {
							if (Session.Is_Color_Used(newcolor)) {
								newcolor = -2;
							}
							Session.Slots[i]->Set_Color(newcolor);
						}
						if (house_changed || color_changed) {
							display_users = true;
							Send_Lobby_Options(true, false);
						}
					}
					break;
				}
			}
			free(opts);
		} else if (Session.GPacket.Command == NET_SIGN_OFF) {
			// NET_SIGN_OFF
			for (i = 1; i < Session.Games.Count(); i++) {
				if (!_stricmp(Wide_To_Narrow(Session.Games[i]->Name), Session.GPacket.Name) && Session.Games[i]->Address == Session.GAddress) {
					if (i == CurGame) {
						Clear_Slot_Lists();
						Fill_Slots();
						Clear_Vector(&Session.Players);
						if (WS_Top_Window_ID() != IDD_MPLAYER_GAME_LIST && WS_Top_Window_ID() == IDD_MPLAYER_GUEST) {
							_netresponse = IDCANCEL;
						}
					}
					if (JoinState > JOIN_NOTHING && i == CurGame) {
						JoinState = JOIN_REJECTED;
						why = REJECT_DISBANDED;
					}
					delete Session.Games[i];
					Session.Games.Delete(Session.Games[i]);
					CurGame = i - 1;
					display_games = true;
					Send_Join_Queries(0, 1, 1, 0);
				}
			}
			bool player_removed = false;
			Release_Slot_By_Address(Session.GAddress);
			for (i = 0; i < Session.Players.Count(); i++) {
				if (Session.Players[i]->Address == Session.GAddress) {
					delete Session.Players[i];
					Session.Players.Delete(Session.Players[i]);
					player_removed = true;
					if (JoinState >= JOIN_CONFIRMED) {
						VocClass::PlayGlobal(Rule->PlayerLeft, 1.0f, 0x2000, 0);
					}
				}
			}
			for (i = 1; i < Session.Chat.Count(); i++) {
				if (Session.Chat[i]->Address == Session.GAddress) {
					delete Session.Chat[i];
					Session.Chat.Delete(Session.Chat[i]);
				}
			}
			if (player_removed) {
				Enable_Start_Button();
				HWND dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST);
				Enable_Player_Combos(dialog, true);
				if (WS_Top_Window_ID() == 0x6B) {
					HWND window = WS_Top_Window();
					int index = SendDlgItemMessageA(window, 0x6EB, 0x188, 0, 0);
					if (index != -1) {
						MultiplayerGameMode * row = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6EB, 0x199, index, 0);
						row->vt_entry_78();
					}
				} else {
					Session.MPGameMode->vt_entry_78();
				}
			}
			display_users = true;
		} else if (Session.GPacket.Command == NET_GO || Session.GPacket.Command == NET_LOADGAME) {
			// NET_GO, NET_LOADGAME
			if (JoinState == JOIN_CONFIRMED) {
				Session.MaxAhead = Session.GPacket.Progress.Percent;
				Session.HostAddress = Session.GAddress;
				Session.NumPlayers = Session.Players.Count();
				_netresponse = 1;
				if (Session.GPacket.Command == NET_GO) {
					JoinState = JOIN_GAME_START;
				} else if (Session.GPacket.Command == NET_LOADGAME) {
					JoinState = JOIN_GAME_START_LOAD;
				}
			}
		} else if (Session.GPacket.Command == NET_CHAT_ANNOUNCE) {
			// NET_CHAT_ANNOUNCE
			if (Session.GPacket.Chat.ID == Session.UniqueID) {
				continue;
			}
			found = 0;
			for (i = 0; i < Session.Chat.Count(); i++) {
				if (Session.Chat[i]->Address == Session.GAddress) {
					wcscpy(Session.Chat[i]->Name, Narrow_To_Wide(Session.GPacket.Name));
					Session.Chat[i]->Chat.LastTime = TickCount();
					Session.Chat[i]->Chat.LastChance = 0;
					Session.Chat[i]->Chat.Color = Session.GPacket.Chat.Color;
					found = 1;
					if (CurGame == 0) {
						display_users = true;
					}
					break;
				}
			}
			if (!found) {
				who = new NodeNameType;
				wcscpy(who->Name, Narrow_To_Wide(Session.GPacket.Name));
				who->Address = Session.GAddress;
				who->Chat.LastTime = TickCount();
				who->Chat.LastChance = 0;
				who->Chat.Color = Session.GPacket.Chat.Color;
				Session.Chat.Add(who);
				if (CurGame == 0) {
					display_users = true;
				}
			}
		} else if (Session.GPacket.Command == NET_CHAT_REQUEST) {
			// NET_CHAT_REQUEST
			if (JoinState != JOIN_WAIT_CONFIRM && JoinState != JOIN_CONFIRMED) {
				GlobalPacketType packet;
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_CHAT_ANNOUNCE;
				strcpy(packet.Name, Session.Handle);
				packet.Chat.ID = Session.UniqueID;
				packet.Chat.Color = Session.PrefColor;
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
				Call_Back();
			}
		} else if (Session.GPacket.Command == NET_MESSAGE) {
			// NET_MESSAGE
			int color = Session.GPacket.Message.Color;
			if (JoinState == JOIN_CONFIRMED) {
				if (Session.GPacket.Message.NameCRC == (int)Compute_Name_CRC(Session.GameName)) {
					Lobby_Chat_Printf(color, L"[%s] %s", Narrow_To_Wide(Session.GPacket.Name), Session.GPacket.Message.Buf);
					VocClass::PlayGlobal(Rule->IncomingMessage, 1.0f, 0x2000, 0);
				}
			} else {
				Lobby_Chat_Printf(color, L"[%s] %s", Narrow_To_Wide(Session.GPacket.Name), Session.GPacket.Message.Buf);
				VocClass::PlayGlobal(Rule->IncomingMessage, 1.0f, 0x2000, 0);
			}
		} else if (Session.GPacket.Command == NET_QUERY_JOIN) {
			// NET_QUERY_JOIN
			GlobalPacketType packet;
			if (!Session.Players.Count()) {
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_REJECT_JOIN;
				packet.Reject.Why = REJECT_DISBANDED;
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
			} else {
			found = 0;
			int resend = 0;
			for (i = 1; i < Session.Players.Count(); i++) {
				if (!_stricmp(Wide_To_Narrow(Session.Players[i]->Name), Session.GPacket.Name)) {
					if (Session.Players[i]->Address != Session.GAddress) {
						found = 1;
					} else {
						resend = 1;
					}
					break;
				}
			}
			if (!_stricmp(Wide_To_Narrow(Session.Players[0]->Name), Session.GPacket.Name)) {
				found = 1;
			}
			if (found) {
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_REJECT_JOIN;
				packet.Reject.Why = REJECT_DUPLICATE_NAME;
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
				continue;
			}
			if (!resend) {
				if (Session.GPacket.PlayerInfo.Flag != 1 && Is_All_Slots_Filled()) {
					memset(&packet, 0, sizeof(packet));
					packet.Command = NET_REJECT_JOIN;
					packet.Reject.Why = REJECT_GAME_FULL;
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
					continue;
				}
				if (Session.GPacket.PlayerInfo.Flag == 1 && Is_Observer_Slot_Full()) {
					memset(&packet, 0, sizeof(packet));
					packet.Command = NET_REJECT_JOIN;
					packet.Reject.Why = REJECT_GAME_FULL;
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
					continue;
				}
			}
			bool match = true;
			if (!resend) {
				if (Session.GPacket.PlayerInfo.CheatCheck != RulesID) {
					match = false;
				}
				if (Session.GPacket.PlayerInfo.AICheatCheck != Rule->Get_AI_Unique_ID()) {
					match = false;
				}
				if (Session.GPacket.PlayerInfo.ArtCheatCheck != ArtID) {
					match = false;
				}
				if (Session.GPacket.PlayerInfo.BuildNumber != Build_Number()) {
					match = false;
				}
			}
			int dups = 0;
			for (i = 0; i < Session.Players.Count(); i++) {
				if (!strcmp(Session.Players[i]->Slot.SerialPad, Session.GPacket.Serial)) {
					dups++;
				}
			}
			if (!match) {
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_REJECT_JOIN;
				packet.Reject.Why = REJECT_MISMATCH;
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
				continue;
			}
			if (!resend) {
				unsigned long version = VerNum.Clip_Version(Session.GPacket.PlayerInfo.MinVersion, Session.GPacket.PlayerInfo.MaxVersion);
				Session.PlayingAgainstVersion = version;
				if (version == 0) {
					memset(&packet, 0, sizeof(packet));
					packet.Command = NET_REJECT_JOIN;
					packet.Reject.Why = REJECT_VERSION_TOO_OLD;
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
					continue;
				}
				if (version == 0xFFFFFFFF) {
					memset(&packet, 0, sizeof(packet));
					packet.Command = NET_REJECT_JOIN;
					packet.Reject.Why = REJECT_VERSION_TOO_NEW;
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
					continue;
				}
				Session.CommProtocol = VerNum.Version_Protocol(version);
				who = new NodeNameType;
				memset(who, 0, sizeof(*who));
				wcscpy(who->Name, Narrow_To_Wide(Session.GPacket.Name));
				who->Address = Session.GAddress;
				if (Session.MPGameMode) {
					who->Set_Country(!Session.MPGameMode->IsCountryAllowed(Session.GPacket.PlayerInfo.House) ? Session.MPGameMode->DefaultCountry() : Session.GPacket.PlayerInfo.House);
				}
				strcpy(who->Slot.SerialPad, Session.GPacket.Serial);
				int color = Session.GPacket.PlayerInfo.Color;
				if (Is_Color_Taken(color)) {
					color = -2;
				}
				if (Session.GPacket.PlayerInfo.Flag == 1) {
					color = -2;
					who->Set_Country(-3);
					who->Slot.SpectatorFlag = -1;
				} else {
					who->Slot.SpectatorFlag = 0;
					if (Session.GPacket.PlayerInfo.House == -3) {
						if (Session.MPGameMode) {
							who->Set_Country(Session.MPGameMode->DefaultCountry());
						} else {
							who->Set_Country(-2);
						}
					}
				}
				who->Set_Color(color);
				Session.Players.Add(who);
				int slot = Assign_Slot(who);
				Enable_Start_Button();
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_CONFIRM_JOIN;
				strcpy(packet.Name, Session.Handle);
				display_users = true;
				packet.PlayerInfo.House = who->Get_Country();
				packet.PlayerInfo.Color = who->Get_Color();
				packet.PlayerInfo.Flag = LobbyObserverSlot;
				packet.PlayerInfo.Slot = slot;
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress);
				VocClass::PlayGlobal(Rule->PlayerJoined, 1.0f, 0x2000, 0);
				if (Session.MPGameMode) {
					int rows = Session.MPGameMode->vt_entry_80();
					if (rows != -1) {
						Session.ScenarioMaxPlayers = rows;
					}
				}
				Send_Lobby_Options(true, false);
				GlobalPacketType slots;
				memset(&slots, 0, sizeof(slots));
				slots.Command = NET_COMMAND_16;
				strcpy(slots.Name, Session.Handle);
				Get_Slot_Names(slots.Slots.Names);
				int count = Session.Players.Count();
				for (i = 1; i < count; i++) {
					Ipx.Send_Global_Message(&slots, sizeof(slots), 1, &Session.Players[i]->Address);
					Call_Back();
				}
				Handle_Mode_Selection();
			}
			}
		}
	}
	if (display_users) {
		Net2DisplayUsersFull(false);
	}
	if (display_games) {
		Net2DisplayGameList();
	}
}


LRESULT CALLBACK MPlayer_Guest_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005C2330; the lobby dialog of a joined player
{
	switch (message) {
	case WM_DESTROY:
		if (Progress.Has_Dialog()) {
			Progress.Clear_Dialog();
		}
		if (MultiplayerMapPreview != NULL) {
			delete MultiplayerMapPreview;
			MultiplayerMapPreview = NULL;
			return(0);
		}
		break;
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		// fall through
	case WM_ERASEBKGND:
		return(1);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		if (MultiplayerMapPreview != NULL) {
			MultiplayerMapPreview->Blit_Preview(window);
		}
		if (Progress.Has_Dialog()) {
			Progress.Display_Progress(Point2D(-1, -1));
		}
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(0);
	case WM_NCHITTEST: { // the tool tip of the control under the cursor
		HWND tip = GetDlgItem(window, 0x695);
		if (tip != NULL) {
			POINT point;
			point.x = lparam & 0xFFFF;
			point.y = (unsigned long)lparam >> 16;
			RECT rect;
			GetWindowRect(window, &rect);
			point.x -= rect.left;
			point.y -= rect.top;
			wchar_t const * text = L"";
			HWND control = ChildWindowFromPointEx(window, point, CWP_SKIPINVISIBLE);
			if (control != NULL) {
				char const * label = OwnerDraw::Control_Tooltip_Label((unsigned int)window, control);
				if (label != NULL) {
					text = StringTable::LoadString(label, NULL, "D:\\Ra2\\netdlg2.cpp", 3143);
				}
			}
			SendMessage(tip, OD_SETTEXTW, 0, (LPARAM)text);
			return(0);
		}
		break;
	}
	case WM_COMMAND:
		switch ((unsigned short)wparam) {
			case IDCANCEL:
				if (!Net2OptionsLocked) {
				_netresponse = IDCANCEL;
				return(0);
			}
			break;
			case 0x510:
			case 0x513:
			case 0x514:
			case 0x51E:
			case 0x51F:
			case 0x520:
			case 0x521:
			case 0x522:
			case 0x523:
			case 0x524:
			case 0x525:
			case 0x526:
			case 0x527:
			case 0x528:
			case 0x6A1:
			case 0x6A2:
				if ((unsigned short)(wparam >> 16) == CBN_SELCHANGE) {
					int id = (unsigned short)wparam;
					int side_player = Side_Combo_Player(id);
					int color_player = Color_Combo_Player(id);
					int player = side_player > color_player ? side_player : color_player;
					int side_id = Player_Side_Combo_ID(player);
					int color_id = Player_Color_Combo_ID(player);
					if (player == side_player) {
						Set_Slot_Country(window, side_id);
				} else {
						Set_Slot_Color(window, color_id);
				}
					char text[64];
					int side = Side_Combo_Data(window, side_id, -1);
					int color = Color_Combo_Data(window, color_id, -1);
					sprintf(text, "R%d,%d", side, color);
					Send_Player_Choice(Wide_To_Narrow(Session.GameName), text);
				return(0);
			}
			break;
			case IDC_INPUT:
				Limit_Name_Text((HWND)lparam);
				if ((unsigned short)(wparam >> 16) == EN_MAXTEXT) {
					Send_Chat_Message(window, ColorMe, ColorUser);
				return(0);
			}
			break;
			case IDC_ACCEPT:
				if (Session.Players.Count() != 0) {
				Session.Players[0]->Player.Status = 1;
					char text[64];
					sprintf(text, "A1");
					Send_Host_Option(text);
					Enable_Player_Combos(window, false);
					HWND button = GetDlgItem(window, IDC_ACCEPT);
					EnableWindow(button, FALSE);
					InvalidateRect(button, NULL, FALSE);
					SendMessage(button, 0x4DC, 0, 0);
					Net2DisplayUsersFull(false);
				return(0);
			}
			break;
		}
		break;
	case OD_SUBCLASSED:
		Fill_Side_Combos(window);
		Init_Color_Entries();
		Fill_Color_Combos(window);
		Lock_Slots(window, Session.LocalSlot);
		Select_Player_Side(window, Session.LocalSlot);
		Select_Player_Color(window, Session.LocalSlot);
		EnableWindow(GetDlgItem(window, IDC_ACCEPT), FALSE);
		LobbyDialogFlag = false;
		Net2SetAccept(NULL, 0);
		Net2DisplayUsersFull(false);
		Net2DisplayGameList();
		Fill_Options_Dialog(window, 1);
		Set_Scenario_Info_From_Index(-1);
		Session.ScenarioMaxPlayers = 8;
		return(0);
	case 0x4E9: { // the tool tip text of a combo box item
		ToolTipTextRequest * request = (ToolTipTextRequest *)lparam;
		request->Text = NULL;
		if (request->Window != NULL && request->Item != -1) {
			int id = GetDlgCtrlID(request->Window);
			if (Control_Slot(id) == -1) {
				if (Side_Combo_Player(id) == -1) {
					if (Color_Combo_Player(id) != -1) {
						request->Text = Color_Name(Color_Combo_Data(window, id, request->Item));
						return(1);
					}
				} else {
					int side = Side_Combo_Data(window, id, request->Item);
					if (side != -1) {
						request->Text = Side_Name(side);
						return(1);
					}
				}
			} else {
				request->Text = Entry_Type_Name((NodeNameType *)Combo_Selected_Data(window, id, request->Item));
			}
		}
		return(1);
	}
	case 0x4EC:
		Dialog_On_OD_4EC(window);
		break;
	}
	return(0);
}



void Join_Lobby_Start_Reminder(void) // RA2: 0x005C28E0; reminds the joiner that the host starts the game
{
	HWND dialog = WS_Top_Window();
	HWND item = GetDlgItem(dialog, IDC_ACCEPT);
	if (Net2GetAccept(NULL) != 1) {
		SendMessage(item, 0x4DC, 0, 1);
		Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostGameStartJoiner", NULL, "D:\\Ra2\\netdlg2.cpp", 4958));
	}
}


void __cdecl Join_Lobby_Reset_Ready(void) // RA2: 0x005C2940; sends message 0x4DC to the ready button of the joiner's lobby
{
	HWND dialog = WS_Top_Window();
	HWND button = GetDlgItem(dialog, IDC_ACCEPT);
	SendMessage(button, 0x4DC, 0, 0);
}
