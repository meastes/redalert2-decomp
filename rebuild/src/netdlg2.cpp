/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
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

#undef LoadString // RA2: as above
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
int Net2_g_Col_House; // RA2: as above
int Net2_g_Col_Accept; // RA2: as above
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
void __fastcall Change_Slot_Type(HWND dialog, int id, int data, int mode); // RA2: 0x005CBF20
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
int Request_To_Join(int join_index, bool observer); // RA2: as above
void Unjoin_Game(int game_index); // RA2: as above
static void Send_Join_Queries(int gamenow, int playernow, int chatnow, int init = 0);
void Get_Join_Responses(void); // RA2: 0x005C08A0
void Send_Chat_Message(HWND dialog, int color, int recipients); // RA2: as above


bool Net2Callback(void)
{
	Call_Back();
	Get_Join_Responses();
	return(false);
}


void Send_Slot_Names(void) // RA2: 0x005BBD20; sends the lobby's slot names to every other player
{
	GlobalPacketType packet;
	memset(&packet, 0, sizeof(packet)); // RA2: as above
	packet.Command = NET_COMMAND_16; // RA2: as above
	strcpy(packet.Name, Session.Handle);
	Get_Slot_Names(packet.Slots.Names); // RA2: as above
	int count = Session.Players.Count(); // RA2: as above
	for (int i = 1; i < count; i++) { // RA2: as above
		Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address); // RA2: as above
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
	Net2DisplayUsersFull(false); // RA2: as above
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
static bool Find_Active_Game_Dialog(HWND & active) // RA2: as above
{
	active = WS_Top_Window(); // RA2: as above
	int dialog_id = WS_Top_Window_ID(); // RA2: as above
	if (dialog_id == 0xBC || dialog_id == 0xBD) { // RA2: as above
		return true; // RA2: as above
	}
	active = WS_Next_Lower_Dialog(active); // RA2: as above
	while (active) { // RA2: as above
		int kind = Dialog_ID_Of_Window(active); // RA2: as above
		if (kind == 0xBC || kind == 0xBD) { // RA2: as above
			return true; // RA2: as above
		}
		active = WS_Next_Lower_Dialog(active); // RA2: as above
	}
	return false; // RA2: as above
}


/// <summary>
/// Redisplays the users of the lobby: the chat players, or the players of the game that is selected.
/// </summary>
void __fastcall Net2DisplayUsersFull(bool full) // RA2: 0x005BBDD0; with a game lobby dialog open its slot rows are refreshed instead
{
	HWND active; // RA2: as above
	if (Find_Active_Game_Dialog(active)) { // RA2: as above
		Refresh_Slot_Settings(active, full); // RA2: as above
		return;
	}
	{
	HWND dialog = WS_Top_Window(); // RA2: as above
	HWND list = GetDlgItem(dialog, 0x53E); // RA2: as above
	if (dialog && list) { // RA2: as above
		HWND users = GetDlgItem(dialog, 0x53E); // RA2: as above
		OwnerDraw::CellData cell; // RA2: as above
		int top = SendDlgItemMessage(dialog, 0x53E, 0x18E, 0, 0); // RA2: as above
		SendDlgItemMessage(dialog, 0x53E, 0x49D, 0, 1); // RA2: as above
		Dictionary<WideWstring, bool> selections(Wstring_Hash); // RA2: as above
		LBSaveSelections(list, selections); // RA2: as above
		SendDlgItemMessage(dialog, 0x53E, 0x184, 0, 0); // RA2: as above
		if (CurGame == 0) {
			SendMessage(users, 0x4CC, 0, (LPARAM)Narrow_To_Wide(Session.Handle)); // RA2: as above
			for (int chat = 1; chat < Session.Chat.Count(); ++chat) { // RA2: as above
				SendMessage(users, 0x4CC, chat, (LPARAM)Session.Chat[chat]); // RA2: as above
			}
		} else {
			for (int player = 0; player < Session.Players.Count(); ++player) { // RA2: as above
				int state = 0; // RA2: as above
				if (UI::Is_Host_Player(player)) { // RA2: as above
					state = 2; // RA2: as above
					Session.Players[player]->Player.Status = 1; // RA2: as above
				} else if (Session.Players[player]->Player.Status != 0) { // RA2: as above
					state = 1; // RA2: as above
				}
				wchar_t info[128]; // RA2: as above
				wchar_t country_name[128]; // RA2: as above
				swprintf(info, L"%s", Session.Players[player]); // RA2: as above
				int country = Session.Players[player]->Get_Country(); // RA2: as above
				Surface * flag = Country_Flag(country); // RA2: as above
				switch (country) { // RA2: as above
				case 0: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 383)); break; // RA2: as above
				case 1: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 386)); break; // RA2: as above
				case 2: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 389)); break; // RA2: as above
				case 3: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 392)); break; // RA2: as above
				case 4: swprintf(country_name, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\netdlg2.cpp", 395)); break; // RA2: as above
				case 5: swprintf(country_name, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\netdlg2.cpp", 398)); break; // RA2: as above
				case 6: swprintf(country_name, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\netdlg2.cpp", 401)); break; // RA2: as above
				case 7: swprintf(country_name, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\netdlg2.cpp", 404)); break; // RA2: as above
				case 8: swprintf(country_name, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\netdlg2.cpp", 407)); break; // RA2: as above
				}
				SendMessage(users, 0x4CC, -1, (LPARAM)info); // RA2: as above
				int color = PlayerColorTable[Session.Players[player]->Player.Color]; // RA2: as above
				cell.type = OwnerDraw::CellData::PRIMARY; // RA2: as above
				cell.color = color; // RA2: as above
				cell.hint.set(L""); // RA2: set, not operator=
				SendDlgItemMessage(dialog, 0x53E, OD_SETCELL, MAKEWPARAM(Net2_g_Col_Name, player), (LPARAM)&cell); // RA2: as above
				cell.type = OwnerDraw::CellData::SURFACE; // RA2: as above
				cell.hint.set(country_name); // RA2: as above
				cell.surf = flag; // RA2: as above
				SendDlgItemMessage(dialog, 0x53E, OD_SETCELL, MAKEWPARAM(Net2_g_Col_House, player), (LPARAM)&cell); // RA2: as above
				cell.hint.set(L""); // RA2: set, not operator=
				cell.type = OwnerDraw::CellData::SURFACE; // RA2: as above
				if (state == 2) { // RA2: as above
					cell.surf = SurfaceCache.GetSurface("wolhost.pcx", false); // RA2: as above
				} else if (state) { // RA2: as above
					cell.surf = SurfaceCache.GetSurface("wolacpt.pcx", false); // RA2: as above
				} else {
					cell.type = OwnerDraw::CellData::INVALID; // RA2: as above
				}
				SendDlgItemMessage(dialog, 0x53E, OD_SETCELL, MAKEWPARAM(Net2_g_Col_Accept, player), (LPARAM)&cell); // RA2: as above
			}
		}
		LBRestoreSelections(list, selections); // RA2: as above
		SendDlgItemMessage(dialog, 0x53E, 0x197, top, 0); // RA2: as above
		SendDlgItemMessage(dialog, 0x53E, 0x49D, 0, 0); // RA2: as above
		InvalidateRect(list, NULL, 0); // RA2: as above
		UpdateWindow(list); // RA2: as above
	}
	}
}


void __fastcall Reject_Join(NodeNameType * node) // RA2: 0x005BC3E0; turns a joiner down
{
	memset(&Session.GPacket, 0, sizeof(Session.GPacket));
	Session.GPacket.Command = NET_REJECT_JOIN;
	Session.GPacket.Reject.Why = REJECT_BY_OWNER; // RA2: as above
	Ipx.Send_Global_Message(&Session.GPacket, sizeof(Session.GPacket), 1, &node->Address); // RA2: as above
}


bool __fastcall Update_Lobby_Count(int count, bool flag1, bool flag2) // RA2: 0x005BC430
{
	HWND dialog = WS_Find_Dialog(0xBC); // RA2: as above
	return Lobby_Update_Dialog(dialog, count, flag1, flag2); // RA2: as above
}


bool Net2Init_Network(void) // RA2: as above
{
	NetNumType net;
	NetNodeType node;

	if (!Ipx.Init()) {
		return(false);
	}
	if (Session.Type != GAME_INTERNET && Session.IsBridge) { // RA2: as above
		Session.BridgeNet.Get_Address(net, node); // RA2: as above
		Ipx.Set_Bridge(net);
	}
	Init_Lobby_Entries(); // RA2: the fixed lobby entries (open, closed, computers)
	Clear_Slot_Lists(); // RA2: as above
	Reset_Slots(); // RA2: as above
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
void Net2Shutdown_Network(void) // RA2: as above
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
	if (PacketTransport != NULL) { // RA2: as above
		delete PacketTransport; // RA2: as above
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
		if (wcscmp(Session.Games[i]->Name, Session.GameName) != 0) { // RA2: as above
			if (TickCount - Session.Games[i]->Game.LastTime > 300) { // RA2: as above
				DebugString("Game '%ls' has timed out\n", Session.Games[i]->Name); // RA2: as above
				delete Session.Games[i];
				Session.Games.Delete(Session.Games[i]);

				Net2DisplayGameList();
				if (i <= CurGame) {
					Net2DisplayUsersFull(false); // RA2: as above
					Send_Join_Queries(0, 1, 0, 0);
				}
			}
		}
	}

	for (i = 1; i < Session.Chat.Count(); i++) {
		if (Session.Chat[i]->Chat.LastTime == 0) {
			Session.Chat[i]->Chat.LastTime = TickCount;
		}
		if (TickCount - Session.Chat[i]->Chat.LastTime > 360) { // RA2: as above
			delete Session.Chat[i];
			Session.Chat.Delete(Session.Chat[i]);
			Net2DisplayUsersFull(false); // RA2: as above
		} else if (TickCount - Session.Chat[i]->Chat.LastTime > 300 && Session.Chat[i]->Chat.LastChance == 0) { // RA2: as above
			GlobalPacketType packet;
			memset(&packet, 0, sizeof(packet)); // RA2: as above
			strcpy(packet.Name, Session.Handle);
			packet.Command = NET_CHAT_REQUEST;
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.Chat[i]->Address); // RA2: as above
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
	wchar_t text[80]; // RA2: as above

	if (WS_Top_Window_ID() == 0xBB) { // RA2: as above
		HWND dialog = WS_Top_Window(); // RA2: as above
		HWND list = GetDlgItem(dialog, 0x549); // RA2: as above
		if (CurGame >= Session.Games.Count()) { // RA2: as above
			CurGame = Session.Games.Count() - 1;
			Send_Join_Queries(0, 1, 0, 0);
		}
		if (CurGame < 0) {
			CurGame = 0;
		}
		int top = SendDlgItemMessage(dialog, 0x549, 0x18E, 0, 0); // RA2: as above
		SendDlgItemMessage(dialog, 0x549, 0x49D, 0, 1); // RA2: as above
		SendMessage(list, 0x184, 0, 0); // RA2: as above
		SendMessage(list, 0x4CD, 0, (LPARAM)StringTable::LoadString("TXT_LOBBY", NULL, "D:\\Ra2\\netdlg2.cpp", 490)); // RA2: as above
		for (int i = 1; i < Session.Games.Count(); i++) {
			if (Session.Games[i]->Game.IsOpen) {
				swprintf(text, StringTable::LoadString("TXT_THATGUYS_GAME", NULL, "D:\\Ra2\\netdlg2.cpp", 496), Session.Games[i]->Name); // RA2: as above
			} else {
				swprintf(text, StringTable::LoadString("TXT_THATGUYS_GAME_BRACKET", NULL, "D:\\Ra2\\netdlg2.cpp", 498), Session.Games[i]->Name); // RA2: as above
			}
			SendMessage(list, 0x4CD, 0, (LPARAM)text); // RA2: as above
		}
		SendDlgItemMessage(dialog, 0x549, 0x186, CurGame, 0); // RA2: as above
		SendDlgItemMessage(dialog, 0x549, 0x197, top, 0); // RA2: as above
		SendDlgItemMessage(dialog, 0x549, 0x49D, 0, 0); // RA2: as above
		list = GetDlgItem(dialog, 0x549); // RA2: as above
		InvalidateRect(list, NULL, 0); // RA2: as above
		UpdateWindow(list); // RA2: as above
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
		Session.Options.ShortGame, Session.Options.SWAllowed, Session.Options.BuildOffAlly, Session.Options.MCVRedeploy, Session.Options.Goodies, Session.Options.MPModeIndex, // RA2: as above
		name.get(), value, Session.ScenarioIsOfficial, Session.ScenarioFileLength, Session.ScenarioFileName, Session.ScenarioDigest); // RA2: as above

	memset(useroptions, 0, sizeof(useroptions));

	for (int i = 0, count = Session.Players.Count(); i < count; i++) { // RA2: as above
		NodeNameType * entry = Session.Players[i]; // RA2: as above
		int country = entry->Get_Country(); // RA2: country and color of each player, three more numbers that are zero
		int color = entry->Get_Color(); // RA2: as above
		wchar_t const * player_name = Session.Players[i]->Name; // RA2: as above
		sprintf(useroptions + strlen(useroptions), "%s,%d,%d,%x,%x,%x,", Wide_To_Narrow(player_name), country, color, 0, 0, 0); // RA2: as above
	}
	useroptions[strlen(useroptions) - 1] = ':'; // RA2: the player list ends in a colon
	strcat(useroptions, "@:"); // RA2: then the computer players
	for (int k = 0; k < 8; k++) { // RA2: as above
		sprintf(useroptions + strlen(useroptions), "%d,%d,%d,", Session.Options.AISlots.Difficulties[k], Session.Options.AISlots.Countries[k] == -3 ? -1 : Session.Options.AISlots.Countries[k], Session.Options.AISlots.Colors[k]); // RA2: as above
	}
	DebugString("Size of options string is %d bytes\n", strlen(useroptions)); // RA2: logged
	DebugString("Options: %s\n", useroptions); // RA2: as above
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
void Net2SetAccept(char const * who, int status) // RA2: as above
{
	if (who == NULL) {
		who = Session.Handle;
		if (status == 0) { // RA2: as above
			HWND dialog = WS_Find_Dialog(0xBD); // RA2: the player's own side and color boxes follow the status
			Enable_Player_Combos(dialog, true); // RA2: as above
		}
	}
	if (wcscmp(Narrow_To_Wide(who), Session.GameName) == 0) { // RA2: the host's own entry is always accepted
		status = 1; // RA2: as above
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
int Net2GetAccept(char const * who) // RA2: as above
{
	if (who == NULL) {
		who = Session.Handle;
	}
	if (wcscmp(Narrow_To_Wide(who), Session.GameName) == 0) { // RA2: the host is always accepted
		return(1); // RA2: as above
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
	int slot = -1; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		if (_stricmp(Wide_To_Narrow(Session.Slots[i]->Name), who) == 0) { // RA2: as above
			slot = i; // RA2: as above
			break;
		}
	}
	int changed = 0; // RA2: as above
	if (slot == -1) { // RA2: as above
		return(0);
	}
	HWND dialog = WS_Find_Dialog(0xBC); // RA2: as above
	if (dialog == NULL) { // RA2: as above
		dialog = WS_Find_Dialog(0xBD); // RA2: as above
	}
	NodeNameType * node = Session.Slots[slot]; // RA2: as above
	if (node->Get_Country() != house) { // RA2: as above
		node->Set_Country(house); // RA2: as above
		changed = 1; // RA2: as above
	}
	int side_id = Player_Side_Combo_ID(slot); // RA2: as above
	bool observer_side = Side_Combo_Data(dialog, side_id, -1) == -3; // RA2: as above
	if (house == -3) { // RA2: as above
		if (!observer_side) { // RA2: as above
			Fill_Observer_Combo(dialog, side_id); // RA2: as above
			SendMessage(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 1); // RA2: as above
			int color_id = Player_Color_Combo_ID(slot); // RA2: as above
			Fill_Random_Color_Combo(dialog, color_id); // RA2: as above
			Select_Color_Combo(dialog, color_id, 8); // RA2: as above
		}
	} else if (observer_side) { // RA2: as above
		Fill_Side_Combo(dialog, side_id); // RA2: as above
		SendMessage(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 0); // RA2: as above
		int color_id = Player_Color_Combo_ID(slot); // RA2: as above
		Fill_Color_Combo(dialog, color_id); // RA2: as above
		Select_Color_Combo(dialog, color_id, -2); // RA2: as above
	}
	if (Session.Slots[slot]->Get_Color() != color) { // RA2: as above
		node->Set_Color(color); // RA2: as above
	}
	if (Session.Slots[slot] == Session.Players[0]) { // RA2: as above
		Session.PrefColor = color;
	}
	Net2DisplayUsers();
	return(changed); // RA2: as above
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
bool Net_Join_Dialog(void) // RA2: as above
{
	RulesID = Rule->Get_Rule_Unique_ID(); // RA2: as above
	Rule->Load_Art_INI(); // RA2: as above
	ArtID = Rule->Get_Art_Unique_ID(); // RA2: as above
	AIID = Rule->Get_AI_Unique_ID(); // RA2: as above
	Decrypt_Serial(SerialNumber);
	Ipx.Set_Timing(30, -1, 600, true); // RA2: as above
	Session.NetStealth = false;
	Session.GameName[0] = 0;
	Session.NetOpen = true;
	Net2OptionsLocked = false; // RA2: as above
	OwnerDraw::Register_Window_Classes(); // RA2: as above
	HWND dialog = (HWND)WS_Create_Dialog(ProgramInstance, 0xBB, MainWindow, (DLGPROC)MPlayer_Game_List_Dialog_Proc, 0); // RA2: as above
	Show_Dialog(dialog, 0xBB); // RA2: as above
	ShowWindow(dialog, 1); // RA2: as above
	Net2DisplayUsersFull(false); // RA2: as above
	CurGame = 0;
	_netresponse = 0;
	JoinState = JOIN_NOTHING;
	Net2JoinListEnabled = true; // RA2: as above

	while (true) {
		//.....................................................................
		// Pop up the network Join/New dialog
		//.....................................................................
		while (_netresponse == 0) {
			Ipx.Service();
			Sleep(0);
			Call_Back();
			Ipx.Service();
			Title_Screen_Restore(false); // RA2: as above
			MessageLoopCount++; // RA2: as above
			MSG msg;
			while (Peek_Windows_Message(&msg, 0, 0, 0, 1)) { // RA2: as above
				TranslateMessage(&msg);
				DispatchMessageA(&msg); // RA2: as above
			}

			Call_Back();
			if (_netresponse != 0) {
				break;
			}

			if (WS_Top_Window()) {
				Send_Join_Queries(0, 0, 0, 0); // RA2: as above
				Get_Join_Responses();
				if (WS_Top_Window_ID() == 0xBC) { // RA2: as above
					Send_Lobby_Options(false, false); // RA2: as above
				}
				Net2ServiceGameList();
			}
		}

		//.....................................................................
		//	-1 = user selected Cancel
		//.....................................................................
		if (_netresponse == 0x5C0 || _netresponse == 2) { // RA2: as above
			Session.Write_MultiPlayer_Settings();
			if (WS_Top_Window_ID() == 0xBB) { // RA2: as above
				if (JoinState > JOIN_NOTHING) {
					Unjoin_Game(CurGame);
					Ipx.Service();
				}
				WS_Destroy_Dialog(0, 0);
				Clear_Slot_Lists(); // RA2: as above
				Fill_Slots(); // RA2: as above
				Clear_Vector(&Session.Players);
				Clear_Vector(&Session.Games);
				Clear_Vector(&Session.Chat);
				Session.NetOpen = false;
				Ipx.Service();
				return(false);
			}
			if (WS_Top_Window_ID() == 0xBC) { // RA2: as above
				Unjoin_Game(CurGame);
				JoinState = JOIN_NOTHING;
				WS_Destroy_Dialog(WS_Top_Window(), 0);
				Reset_Slots(); // RA2: as above
				dialog = (HWND)WS_Create_Dialog(ProgramInstance, 0xBB, MainWindow, (DLGPROC)MPlayer_Game_List_Dialog_Proc, 0); // RA2: as above
				Show_Dialog(dialog, 0xBB); // RA2: as above
				ShowWindow(dialog, 1); // RA2: as above
				Send_Join_Queries(0, 0, 1, 0);
			} else if (WS_Top_Window_ID() == 0xBD) { // RA2: as above
				if (JoinState == JOIN_CONFIRMED) {
					Unjoin_Game(CurGame);
					JoinState = JOIN_NOTHING;
				} else {
					//...............................................................
					// If I'm not joined to a game, send a SIGN_OFF to all players
					// in my Chat vector (but not to myself, index 0)
					//...............................................................
					GlobalPacketType packet;
					memset(&packet, 0, sizeof(packet)); // RA2: as above
					packet.Command = NET_SIGN_OFF;
					strcpy(packet.Name, Session.Handle);
					for (int i = 1; i < Session.Chat.Count(); i++) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Chat[i]->Address); // RA2: as above
						Call_Back();
					}
					Ipx.Send_Global_Message(&packet, sizeof(packet), 0, 0); // RA2: as above
					if (Session.IsBridge) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
					}
				}
				CDTimerClass<SystemTimerClass> timeout = 600; // RA2: as above
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
				Clear_Slot_Lists(); // RA2: as above
				Reset_Slots(); // RA2: as above
				dialog = (HWND)WS_Create_Dialog(ProgramInstance, 0xBB, MainWindow, (DLGPROC)MPlayer_Game_List_Dialog_Proc, 0); // RA2: as above
				Show_Dialog(dialog, 0xBB); // RA2: as above
				ShowWindow(dialog, 1); // RA2: as above
			} else {
				if (JoinState == JOIN_CONFIRMED) {
					Unjoin_Game(CurGame);
					JoinState = JOIN_NOTHING;
				} else {
					GlobalPacketType packet;
					memset(&packet, 0, sizeof(packet)); // RA2: as above
					packet.Command = NET_SIGN_OFF;
					strcpy(packet.Name, Session.Handle);
					for (int i = 1; i < Session.Chat.Count(); i++) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Chat[i]->Address); // RA2: as above
						Call_Back();
					}
					Ipx.Send_Global_Message(&packet, sizeof(packet), 0, 0); // RA2: as above
					if (Session.IsBridge) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
					}
				}
				CDTimerClass<SystemTimerClass> timeout = 600; // RA2: as above
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
				Clear_Slot_Lists(); // RA2: as above
				Reset_Slots(); // RA2: as above
				dialog = (HWND)WS_Create_Dialog(ProgramInstance, 0xBB, MainWindow, (DLGPROC)MPlayer_Game_List_Dialog_Proc, 0); // RA2: as above
				Show_Dialog(dialog, 0xBB); // RA2: as above
				ShowWindow(dialog, 1); // RA2: as above
			}
		}

		//.....................................................................
		//	0 = user has joined an existing game; save values & return
		//.....................................................................
		if (_netresponse == 0x6CD) { // RA2: as above
			Session.NetStealth = false;
			Session.Write_MultiPlayer_Settings();
			if (Request_To_Join(CurGame, ObserverMode != 0)) { // RA2: as above
				JoinState = JOIN_WAIT_CONFIRM;
			} else {
				HWND top = WS_Top_Window(); // RA2: as above
				if (top) { // RA2: as above
					EnableWindow(top, TRUE); // RA2: as above
					HWND button = GetDlgItem(top, 0x541); // RA2: as above
					if (button) { // RA2: as above
						EnableWindow(button, TRUE); // RA2: as above
					}
				}
			}
			if (Session.MPGameMode) { // RA2: as above
				Session.MPGameMode->vt_entry_24(); // RA2: as above
			}
		}

		//.....................................................................
		//	1 = user requests New Network Game
		//.....................................................................
		if (_netresponse == 0x6CE) { // RA2: as above
			Session.NetStealth = false;
			Session.Write_MultiPlayer_Settings();
			if (Request_To_Join(CurGame, true)) { // RA2: as above
				JoinState = JOIN_WAIT_CONFIRM;
			} else {
				HWND top = WS_Top_Window(); // RA2: as above
				if (top) { // RA2: as above
					EnableWindow(top, TRUE); // RA2: as above
					HWND button = GetDlgItem(top, 0x541); // RA2: as above
					if (button) { // RA2: as above
						EnableWindow(button, TRUE); // RA2: as above
					}
				}
			}
			if (Session.MPGameMode) { // RA2: as above
				Session.MPGameMode->vt_entry_24(); // RA2: as above
			}
		}

		if (_netresponse == 0x6CC) { // RA2: as above
			bool ok = true;

			//...............................................................
			// Force user to enter a name
			//...............................................................
			if (strlen(Session.Handle) < 1) {
				ODMessageBox(StringTable::LoadString("TXT_NAME_BLANK", NULL, "D:\\Ra2\\netdlg2.cpp", 1571), 0, Net2Callback, false); // RA2: as above
				ok = false;
			}

			//...............................................................
			// Ensure name is unique
			//...............................................................
			for (int i = 0; i < Session.Games.Count(); i++) {
				if (ok && !_stricmp(Wide_To_Narrow(Session.Games[i]->Name), Session.Handle)) { // RA2: as above
					ODMessageBox(StringTable::LoadString("TXT_GAMENAME_MUSTBE_UNIQUE", NULL, "D:\\Ra2\\netdlg2.cpp", 1581), 0, Net2Callback, false); // RA2: as above
					ok = false;
					break;
				}
			}

			if (ok) {
				//...............................................................
				// Save player & game name
				//...............................................................
				wcscpy(Session.GameName, Narrow_To_Wide(Session.Handle)); // RA2: as above
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
				NodeNameType *who = new NodeNameType; // RA2: as above
				memset(who, 0, sizeof(*who));
				wcscpy(who->Name, Narrow_To_Wide(Session.Handle)); // RA2: as above
				strcpy(who->Slot.SerialPad, SerialNumber); // RA2: as above
				who->Set_Country(Session.Chosen_House()); // RA2: as above
				who->Set_Color(Session.Chosen_Color()); // RA2: as above
				who->Player.Status = 1; // RA2: as above
				who->Slot.SpectatorFlag = 0; // RA2: as above
				LobbyObserverSlot = 7; // RA2: as above
				if (Net2ObserverMode) { // RA2: as above
					LobbyObserverSlot = 0; // RA2: as above
					who->Slot.SpectatorFlag = -1; // RA2: as above
				} else if (who->Get_Country() == -3) { // RA2: as above
					who->Set_Country(-2); // RA2: as above
				}
				Session.Players.Add(who);
				if (Session.MPGameMode) { // RA2: as above
					Session.MPGameMode->vt_entry_24(); // RA2: as above
				}
				Clear_Slot_Lists(); // RA2: as above
				Reset_Slots(); // RA2: as above
				Seat_First_Player(0); // RA2: as above
				if (LobbyObserverSlot == 0) { // RA2: as above
					Session.Slots[0]->Set_Country(-3); // RA2: as above
					Session.Slots[0]->Slot.SpectatorFlag = -1; // RA2: as above
				}
				JoinState = JOIN_CONFIRMED;

				NodeNameType *game = new NodeNameType; // RA2: as above
				wcscpy(game->Name, Narrow_To_Wide(Session.Handle)); // RA2: as above
				game->Address = Session.GAddress;
				game->Game.IsOpen = true;
				game->Game.LastTime = TickCount(); // RA2: as above
				Session.Games.Add(game);

				CurGame = Session.Games.Count() - 1;

				//..................................................................
				//	Pop up the New Network Game dialog; if user selects OK, return
				//	'true'; otherwise, return to the Join Dialog.
				//..................................................................
				HWND host = (HWND)WS_Create_Dialog(ProgramInstance, 0xBC, MainWindow, (DLGPROC)MPlayer_Host_Dialog_Proc, 0); // RA2: as above
				Show_Dialog(host, 0xBC); // RA2: as above
				SendMessageA(host, 0x4A9, 0, 1); // RA2: as above
				ShowWindow(host, 1); // RA2: as above
			} else {
				HWND top = WS_Top_Window(); // RA2: as above
				if (top) { // RA2: as above
					EnableWindow(top, TRUE); // RA2: as above
					HWND button = GetDlgItem(top, 0x541); // RA2: as above
					if (button) { // RA2: as above
						EnableWindow(button, TRUE); // RA2: as above
					}
				}
			}
		}

		if (_netresponse == 1 && WS_Top_Window_ID() == 0xBD) { // RA2: as above
			while (WS_Destroy_Dialog(0, 0) == true) { // RA2: as above
			}
			if (JoinState == JOIN_GAME_START) { // RA2: as above
				if (!Net2ReadyToGo(0)) {
					_netresponse = 2;
					Net2OptionsLocked = false; // RA2: as above
					continue;
				}
				Net2OptionsLocked = true; // RA2: as above
			} else if (JoinState == JOIN_GAME_START_LOAD) { // RA2: as above
				Net2ReadyToGo(1);
			}
			_netresponse = 0;

			PregameSetup();
			Session.FrameSendRate = 5; // RA2: as above
			Session.LatencyFudge = 0;
			Session.PrecalcMaxAhead = 0;
			Session.PrecalcDesiredFrameRate = 0;
			if (Session.CommProtocol == 2) { // RA2: as above
				Session.MaxAhead = ((Ipx.Global_Response_Time() / 8 + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate > Session.FrameSendRate * 2 ? ((Ipx.Global_Response_Time() / 8 + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate : Session.FrameSendRate * 2; // RA2: as above
			} else {
				Session.MaxAhead = (int)Ipx.Global_Response_Time() / 8 > 2 ? (int)Ipx.Global_Response_Time() / 8 : 2; // RA2: as above
			}
			Session.NetOpen = false;
			Session.Write_MultiPlayer_Settings();
			return(true);
		}

		if (_netresponse == 0x588) { // RA2: as above
			Net2OptionsLocked = false; // RA2: as above
			Session.Write_MultiPlayer_Settings();
		}
		int count = Session.Players.Count(); // RA2: as above
		int observers = 0; // RA2: as above
		for (int i = 0; i < count; i++) { // RA2: as above
			if (Session.Players[i]->Slot.SpectatorFlag == -1) { // RA2: as above
				observers++; // RA2: as above
			}
		}
		if (_netresponse == 0x588) { // RA2: as above
			if (count - observers <= 1) { // RA2: as above
				Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_ONLY_ONE", NULL, "D:\\Ra2\\netdlg2.cpp", 1760)); // RA2: as above
				_netresponse = 0;
				EnableWindow(GetDlgItem(WS_Top_Window(), 0x588), TRUE); // RA2: as above
			}
			if (_netresponse == 0x588) { // RA2: as above
				bool nomap = false; // RA2: as above
				for (int i = 1; i < count; i++) { // RA2: as above
					NodeNameType *player = Session.Players[i]; // RA2: as above
					if (player->Slot.Unknown7F == 0) { // RA2: as above
						Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostNoMap", NULL, "D:\\Ra2\\netdlg2.cpp", 1772), player->Name, Session.Options.ScenarioDescription); // RA2: as above
						nomap = true; // RA2: as above
					}
				}
				if (nomap) { // RA2: as above
					_netresponse = 0;
					EnableWindow(GetDlgItem(WS_Top_Window(), 0x588), TRUE); // RA2: as above
				}
			}
		}
		MultiplayerGameMode *mode = Session.MPGameMode; // RA2: as above
		if (_netresponse == 0x588) { // RA2: as above
			int map_players = RandomMapWaypointCount(Session.Options.ScenarioIndex); // RA2: as above
			if (map_players < Session.Options.AIPlayers - observers + count) { // RA2: as above
				Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_SCENARIO_TOO_SMALL", NULL, "D:\\Ra2\\netdlg2.cpp", 1791), map_players); // RA2: as above
				EnableWindow(GetDlgItem(WS_Top_Window(), 0x588), TRUE); // RA2: as above
				_netresponse = 0;
			}
			{
				WideWstring message; // RA2: as above
				if (!mode->CheckStartConditions(&message)) { // RA2: as above
					Lobby_Chat_Printf(-1, message.get()); // RA2: as above
					EnableWindow(GetDlgItem(WS_Top_Window(), 0x588), TRUE); // RA2: as above
					_netresponse = 0;
				}
			}
			if (_netresponse == 0x588) { // RA2: as above
				for (int i = 0; i < count; i++) { // RA2: as above
					if (Session.Players[i]->Player.Status == 0) {
						Dispatch_Game_Mode(); // RA2: as above
						Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostGameStartHost", NULL, "D:\\Ra2\\netdlg2.cpp", 1817)); // RA2: as above
						_netresponse = 0;
						EnableWindow(GetDlgItem(WS_Top_Window(), 0x588), TRUE); // RA2: as above
						break;
					}
				}
				if (_netresponse == 0x588) { // RA2: as above
					Net2OptionsLocked = true; // RA2: as above
					int i;
					for (i = 0; i < Session.Players.Count(); i++) {
						if (Session.Players[i]->Slot.SpectatorFlag == -1) { // RA2: as above
							Session.Players[i]->Set_Color(-2); // RA2: as above
						}
					}
					mode->vt_entry_58(); // RA2: as above
					Session.Assign_Random_Choices(); // RA2: as above
					Send_Lobby_Options(true, true); // RA2: as above
					{
						GlobalPacketType packet;
						memset(&packet, 0, sizeof(packet)); // RA2: as above
						packet.Command = NET_COMMAND_16; // RA2: as above
						strcpy(packet.Name, Session.Handle);
						Get_Slot_Names(packet.Slots.Names); // RA2: as above
						int players = Session.Players.Count(); // RA2: as above
						for (i = 1; i < players; i++) { // RA2: as above
							Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address); // RA2: as above
							Call_Back();
						}
					}
					mode->vt_entry_54(); // RA2: as above
					if (MultiplayerMapPreview != 0) { // RA2: as above
						delete MultiplayerMapPreview;
						MultiplayerMapPreview = 0;
					}
					PregameSetup();
					Session.FrameSendRate = 5; // RA2: as above
					Session.LatencyFudge = 0;
					Session.PrecalcMaxAhead = 0;
					Session.PrecalcDesiredFrameRate = 0;
					if (Session.CommProtocol == 2) { // RA2: as above
						Session.MaxAhead = ((Ipx.Global_Response_Time() / 8 + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate > Session.FrameSendRate * 2 ? ((Ipx.Global_Response_Time() / 8 + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate : Session.FrameSendRate * 2; // RA2: as above
					} else {
						Session.MaxAhead = (int)Ipx.Global_Response_Time() / 8 > 2 ? (int)Ipx.Global_Response_Time() / 8 : 2; // RA2: as above
					}
					Ipx.Set_Timing(30 > Ipx.Global_Response_Time() + 2 ? 30 : Ipx.Global_Response_Time() + 2, -1, 600, true); // RA2: as above

				/*
				 * Not the GO button -- there is nothing to do this pass, so reset
				 * the response and fall back through the main message loop.
				 */
					GlobalPacketType packet;
					memset(&packet, 0, sizeof(packet)); // RA2: as above
					packet.Command = NET_GO; // RA2: as above
					packet.Progress.Percent = Session.MaxAhead;   // NET_GO carries MaxAhead at +0x2F // RA2: as above
					for (i = 1; i < Session.Players.Count(); i++) {
						Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address); // RA2: as above
					}

					CDTimerClass<SystemTimerClass> timeout = 1200; // RA2: as above
					while (Ipx.Global_Num_Send() > 0 && !timeout) {
						Call_Back();
					}

					int responses[8]; // RA2: as above
					memset(responses, 0, sizeof(responses));
					int num_responses = 0;
					bool send_scenario = false;
					DebugString("About to wait for 'GO' response.\n");
				CDTimerClass<SystemTimerClass> response_timer;    // timeout timer for waiting for responses
					response_timer = 600; // RA2: as above
					do {
						Call_Back();
						if (Ipx.Get_Global_Message(&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID) // RA2: as above
							&& Session.GProductID == 0xAA03) { // RA2: as above
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
					Darken_Surface(NULL, true); // RA2: as above
					if (send_scenario) {
						memset(Session.ScenarioRequests, 0, sizeof(Session.ScenarioRequests));
						Session.RequestCount = 0;
						for (i = 1; i < Session.Players.Count(); i++) {
							if (responses[i] == 0x10) { // RA2: as above
								Session.ScenarioRequests[Session.RequestCount] = i; // RA2: as above
								Session.RequestCount++; // RA2: as above
							}
						}
						Send_Remote_File(Scen->ScenarioName, 3, false, true); // RA2: as above
					}
					Ipx.Set_Timing(Ipx.Global_Response_Time() + 2 > 30 ? Ipx.Global_Response_Time() + 2 : 30, // RA2: as above
						-1, 120 > Ipx.Global_Response_Time() * 8 ? 120 : Ipx.Global_Response_Time() * 8, true); // RA2: as above
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
	Ipx.Set_Timing(Ipx.Global_Response_Time() + 2 > 30 ? Ipx.Global_Response_Time() + 2 : 30, -1, 1000, true); // RA2: as above
	if (Find_Local_Scenario(Session.ScenarioFileName, Session.ScenarioFileLength, Session.ScenarioDigest, Session.ScenarioIsOfficial, true) == true) { // RA2: as above
		DebugString("Found local scenario, file name is %d\n", Session.ScenarioFileName);
		GlobalPacketType packet;
		memset(&packet, 0, sizeof(packet)); // RA2: as above
		packet.Command = NET_READY_TO_GO;
		Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.HostAddress); // RA2: as above
		while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) {
			Call_Back();
		}

	} else {
		//.....................................................................
		// If the other guys are playing a scenario I don't have (sniff), I can't
		// play.  Try to bail gracefully.
		//.....................................................................
		DebugString("Failed to find local scenario, file name is %d\n", Session.ScenarioFileName);
		if ((Session.ScenarioIsOfficial && !Session.Is_Random_Map()) || !Get_File_From_Host(Session.ScenarioFileName, 3, true)) { // RA2: as above
			Session.Options.ScenarioIndex = -1;
			WWMessageBox(L" ").Process(StringTable::LoadString("TXT_UNABLE_PLAY_WAAUGH", NULL, "D:\\Ra2\\netdlg2.cpp", 678), // RA2: as above
				StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\netdlg2.cpp", 678), NULL, NULL, false); // RA2: as above
			GlobalPacketType packet;
			memset(&packet, 0, sizeof(packet)); // RA2: as above
			packet.Command = NET_SIGN_OFF;
			strcpy(packet.Name, Session.Handle);
			for (int i = 1; i < Session.Chat.Count(); i++) {
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Chat[i]->Address); // RA2: as above
				Call_Back();
			}
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL); // RA2: as above
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
	int response = Ipx.Global_Response_Time(); // RA2: as above
	if (response < 20) { // RA2: as above
		DebugString("IPX.Global_Response_Time() == %d. Adjusting to 20\n", response); // RA2: as above
		response = 20; // RA2: as above
	}
	Ipx.Set_Timing(response, -1, response * 8 < 120 ? 120 : response * 8, true); // RA2: as above
	Ipx.Set_External_Timing(60, -1, 600); // RA2: as above
	return(true);
}


/// <summary>
/// Handles the multiplayer guest dialog.
/// This is the setup dialog a player works in after joining somebody else's game. The
/// guest picks a side and a color here, chats with the rest of the players, and tells the
/// host when it is happy for the game to begin.
/// </summary>
/// <returns>Returns with TRUE if the message was consumed by this dialog.</returns>
#ifdef RA2_NON_MATCHING // not exact: no exact match yet (row 005be6b0)
LRESULT CALLBACK MPlayer_Game_List_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005BE6B0; the join dialog of the LAN lobby
{
	switch (message) {
	case WM_ERASEBKGND:
		return(1);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window); // RA2: as above
		Slide_Pending_Dialog(window); // RA2: as above
		ValidateRect(window, NULL);
		return(0);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam); // RA2: as above
		return(1);
	case WM_COMMAND:
		switch ((unsigned short)wparam) { // RA2: as above
		case 0x549: // the game list // RA2: as above
			if (JoinState <= 0) { // RA2: as above
				int old_index = CurGame; // RA2: as above
				int index = SendDlgItemMessage(window, 0x549, 0x188, 0, 0); // RA2: as above
				if (index >= 0 && Net2JoinListEnabled) { // RA2: as above
					CurGame = index; // RA2: as above
					wcscpy(Session.GameName, Session.Games[index]->Name); // RA2: as above
				}
				if ((unsigned short)(wparam >> 16) == 1) { // RA2: as above
					Clear_Vector(&Session.Players);
					if (old_index != CurGame) { // RA2: as above
						Send_Join_Queries(1, 1, 1, 0);
					}
					Net2DisplayUsersFull(false); // RA2: as above
					return(0);
				}
				if ((unsigned short)(wparam >> 16) == 2) { // RA2: as above
					_netresponse = ObserverMode == 1 ? 0x6CE : 0x6CD; // RA2: as above
					return(0);
				}
			}
			break;
		case 0x541: // the name edit // RA2: as above
			if (IsWindowEnabled(window) && _netresponse == 0) { // RA2: as above
				wchar_t name[20]; // RA2: as above
				name[0] = 0; // RA2: as above
				HWND edit = GetDlgItem(window, 0x541); // RA2: as above
				SendMessage(edit, 0x4B3, 20, (LPARAM)name); // RA2: as above
				wchar_t * text = name; // RA2: as above
				while (*text != 0 && *text == ' ') { // RA2: as above
					text++; // RA2: as above
				}
				if (*text == 0 && name[0] != 0) { // RA2: as above
					name[0] = 0; // RA2: as above
					edit = GetDlgItem(window, 0x541); // RA2: as above
					SendMessage(edit, 0x4B2, 0, (LPARAM)name); // RA2: as above
				}
				if (strcmp(Wide_To_Narrow(name), Session.Handle) != 0) { // RA2: as above
					strcpy(Session.Handle, Wide_To_Narrow(name)); // RA2: as above
					Send_Join_Queries(0, 0, 1, 0);
					Net2DisplayUsersFull(false); // RA2: as above
					return(0);
				}
			}
			break;
		case 0x540: // the chat line // RA2: as above
			Limit_Name_Text((HWND)lparam); // RA2: as above
			if ((unsigned short)(wparam >> 16) == 0x501) { // RA2: as above
				Send_Chat_Message(window, ColorMe, ColorUser); // RA2: as above
				return(0);
			}
			break;
		case 0x5C0: // RA2: as above
			_netresponse = 0x5C0; // RA2: as above
			Save_Observer_Mode(); // RA2: as above
			return(0);
		case 0x6CC: // RA2: as above
			_netresponse = (unsigned short)wparam; // RA2: as above
			Net2ObserverMode = ObserverMode == 1; // RA2: as above
			return(0);
		case 0x6CD: // RA2: as above
		case 0x6CE: { // RA2: as above
			EnableWindow(window, FALSE); // RA2: as above
			HWND edit = GetDlgItem(window, 0x541); // RA2: as above
			if (edit != NULL) { // RA2: as above
				EnableWindow(edit, FALSE); // RA2: as above
			}
			_netresponse = (unsigned short)wparam; // RA2: as above
			return(0);
		}
		case 0x75F: // RA2: as above
		case 0x760: { // the observer radio buttons // RA2: as above
			int checked = 0; // RA2: as above
			HWND button = GetDlgItem(window, 0x760); // RA2: as above
			if (button != NULL) { // RA2: as above
				checked = SendMessage(button, 0xF0, 0, 0); // RA2: as above
			}
			if ((unsigned short)wparam == 0x75F) { // RA2: as above
				checked = checked == 0; // RA2: as above
			}
			ObserverMode = checked; // RA2: as above
			Update_Observer_Buttons(NULL); // RA2: as above
			return(0);
		}
		}
		break;
	case WM_MOUSEMOVE: { // the tool tip of the control under the cursor // RA2: as above
		HWND tip = GetDlgItem(window, 0x695); // RA2: as above
		if (tip != NULL) { // RA2: as above
			POINT point; // RA2: as above
			point.x = lparam & 0xFFFF; // RA2: as above
			point.y = (unsigned long)lparam >> 16; // RA2: as above
			RECT rect; // RA2: as above
			GetWindowRect(window, &rect); // RA2: as above
			point.x -= rect.left; // RA2: as above
			point.y -= rect.top; // RA2: as above
			wchar_t const * text = L""; // RA2: as above
			HWND control = ChildWindowFromPointEx(window, point, 1); // RA2: as above
			if (control != NULL) { // RA2: as above
				char const * label = OwnerDraw::Control_Tooltip_Label((unsigned int)window, control); // RA2: as above
				if (label != NULL) { // RA2: as above
					text = StringTable::LoadString(label, NULL, "D:\\Ra2\\netdlg2.cpp", 2285); // RA2: as above
				}
			}
			SendMessage(tip, 0x4B2, 0, (LPARAM)text); // RA2: as above
			return(0);
		}
		break;
	}
	case 0x497: { // RA2: as above
		CurGame = 0;
		Net2JoinListEnabled = true; // RA2: as above
		Update_Observer_Buttons(window); // RA2: as above
		SendDlgItemMessage(window, 0x541, 0xC5, 16, 0); // RA2: as above
		SendDlgItemMessage(window, 0x541, 0x4EB, 0, (LPARAM)L","); // RA2: as above
		SendDlgItemMessage(window, 0x541, 0x4D1, 1, 0); // RA2: as above
		wchar_t * name = Narrow_To_Wide(Session.Handle); // RA2: as above
		HWND edit = GetDlgItem(window, 0x541); // RA2: as above
		SendMessage(edit, 0x4B2, 0, (LPARAM)name); // RA2: as above
		Session.Options.ScenarioDescription[0] = 0; // RA2: as above
		Session.PrefColor = Session.Color; // RA2: as above
		Session.ColorIdx = Session.Color2; // RA2: as above
		Clear_Vector(&Session.Games);
		Clear_Vector(&Session.Players);
		Clear_Vector(&Session.Chat);
		NodeNameType * node = new NodeNameType; // RA2: as above
		wcscpy(node->Name, Narrow_To_Wide(Session.Handle)); // RA2: as above
		node->Chat.LastTime = 0; // RA2: as above
		node->Chat.LastChance = 0; // RA2: as above
		node->Chat.Color = Session.GPacket.Chat.Color; // RA2: as above
		Session.Chat.Add(node); // RA2: as above
		node = new NodeNameType; // RA2: as above
		wcscpy(node->Name, L""); // RA2: as above
		node->Game.IsOpen = 0; // RA2: as above
		node->Game.LastTime = 0; // RA2: as above
		Session.Games.Add(node); // RA2: as above
		Send_Join_Queries(1, 0, 1, 1); // RA2: as above
		Net2DisplayGameList();
		Net2DisplayUsersFull(false); // RA2: as above
		Dialog_On_WM_PAINT(window); // RA2: as above
		SetFocus(GetDlgItem(window, 0x540)); // RA2: as above
		break;
	}
	case 0x4DB: // RA2: as above
		Focus_Next_Tab_Item(lparam, wparam != 0); // RA2: as above
		return(0);
	case 0x4EC: // RA2: as above
		Dialog_On_OD_4EC(window); // RA2: as above
		return(0);
	}
	return(0);
}
#endif


void Send_Chat_Message(HWND dialog, int color, int recipients) // RA2: 0x005BEE20; sends the text of the chat line to the lobby
{
	wchar_t text[112]; // RA2: as above

	HWND edit = GetDlgItem(dialog, 0x540); // RA2: as above
	SendMessage(edit, 0x4B3, 0x6F, (LPARAM)text); // RA2: as above
	text[0x6F] = 0; // RA2: as above
	int length = wcslen(text); // RA2: as above
	edit = GetDlgItem(dialog, 0x540); // RA2: as above
	SendMessage(edit, 0x4B2, 0, (LPARAM)L""); // RA2: as above
	InvalidateRect(dialog, NULL, 1); // RA2: as above
	if (length > 2) { // RA2: as above
		Lobby_Chat_Printf(color, L"[%s] %s", Narrow_To_Wide(Session.Handle), text); // RA2: as above
		GlobalPacketType packet;
		memset(&packet, 0, sizeof(packet)); // RA2: as above
		packet.Command = NET_MESSAGE; // RA2: as above
		strcpy(packet.Name, Session.Handle);
		wcscpy((wchar_t *)packet.Message.Buf, text); // RA2: as above
		packet.Message.Color = recipients; // RA2: as above
		packet.Message.NameCRC = Compute_Name_CRC(Session.GameName); // RA2: as above
		if (JoinState == JOIN_CONFIRMED) {
			int count = Session.Players.Count(); // RA2: as above
			for (int i = 1; i < count; i++) { // RA2: as above
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address); // RA2: as above
				Call_Back();
			}
		} else {
			int count = Session.Chat.Count(); // RA2: as above
			for (int i = 1; i < count; i++) { // RA2: as above
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Chat[i]->Address); // RA2: as above
				Call_Back();
			}
		}
	}
}


LRESULT CALLBACK MPlayer_Host_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005BEFE0; the lobby dialog of the host
{
	if (message != 0x497) { // RA2: as above
		Options_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
	}
	int scenario = Session.Options.ScenarioIndex; // RA2: as above
	switch (message) {
	case WM_DESTROY:
		if (Progress.Has_Dialog()) { // RA2: as above
			Progress.Clear_Dialog(); // RA2: as above
		}
		return(0);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window); // RA2: as above
		if (MultiplayerMapPreview != NULL) {
			MultiplayerMapPreview->Blit_Preview(window);
		}
		if (Progress.Has_Dialog()) { // RA2: as above
			Progress.Display_Progress(Point2D(-1, -1)); // RA2: as above
		}
		Slide_Pending_Dialog(window); // RA2: as above
		ValidateRect(window, NULL);
		return(0);
	case WM_ERASEBKGND:
		return(1);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam); // RA2: as above
		return(1);
	case WM_NCHITTEST: { // the tool tip of the control under the cursor // RA2: as above
		HWND tip = GetDlgItem(window, 0x695); // RA2: as above
		if (tip != NULL) { // RA2: as above
			RECT rect; // RA2: as above
			GetWindowRect(window, &rect); // RA2: as above
			POINT point; // RA2: as above
			point.x = (lparam & 0xFFFF) - rect.left; // RA2: as above
			point.y = ((unsigned long)lparam >> 16) - rect.top; // RA2: as above
			wchar_t const * text = L""; // RA2: as above
			HWND control = ChildWindowFromPointEx(window, point, 1); // RA2: as above
			if (control != NULL) { // RA2: as above
				char const * label = OwnerDraw::Control_Tooltip_Label((unsigned int)window, control); // RA2: as above
				if (label != NULL) { // RA2: as above
					text = StringTable::LoadString(label, NULL, "D:\\Ra2\\netdlg2.cpp", 2704); // RA2: as above
				}
			}
			SendMessage(tip, 0x4B2, 0, (LPARAM)text); // RA2: as above
		}
		return(0);
	}
	case 0x49B: // the text of the edit control that was asked for // RA2: as above
		SendMessage(GetDlgItem(window, wparam), 0x4B3, 0x7F, lparam); // RA2: as above
		return(0);
	case 0x114: // RA2: as above
	case 0x115: // RA2: as above
		if (!Net2OptionsLocked) { // RA2: as above
			Session.Options.GameSpeed = 6 - SendDlgItemMessage(window, 0x529, 0x400, 0, 0); // RA2: as above
			Session.Options.Credits = SendDlgItemMessage(window, 0x402, 0x400, 0, 0); // RA2: as above
			Session.Options.UnitCount = SendDlgItemMessage(window, 0x54F, 0x400, 0, 0); // RA2: as above
		}
		return(0);
	case 0x4E9: { // the tool tip text of a combo box item // RA2: as above
		ToolTipTextRequest * request = (ToolTipTextRequest *)lparam; // RA2: as above
		request->Text = NULL; // RA2: as above
		if (request->Window != NULL) { // RA2: as above
			if (request->Item == -1) { // RA2: as above
				return(1);
			}
			int id = GetDlgCtrlID(request->Window); // RA2: as above
			if (Control_Slot(id) == -1) { // RA2: as above
				if (Side_Combo_Player(id) == -1) { // RA2: as above
					if (Color_Combo_Player(id) == -1) { // RA2: as above
						return(1);
					}
					request->Text = Color_Name(Color_Combo_Data(window, id, request->Item)); // RA2: as above
				} else {
					int side = Side_Combo_Data(window, id, request->Item); // RA2: as above
					if (side == -1) { // RA2: as above
						return(1);
					}
					request->Text = Side_Name(side); // RA2: as above
				}
			} else {
				request->Text = Entry_Type_Name((NodeNameType *)Combo_Selected_Data(window, id, request->Item)); // RA2: as above
			}
		}
		return(1);
	}
	case 0x4EC: // RA2: as above
		Dialog_On_OD_4EC(window); // RA2: as above
		return(0);
	case 0x497: // RA2: as above
		VerNum.Init_Clipping();
		srand(NonCriticalRandomNumber(1, 0x7FFF));
		Seed = rand();
		Center_Window_Within_Window(window);
		Fill_Side_Combos(window); // RA2: as above
		Init_Color_Entries(); // RA2: as above
		Fill_Color_Combos(window); // RA2: as above
		Select_Player_Side(window, 0); // RA2: as above
		Select_Player_Color(window, 0); // RA2: as above
		Net2DisplayUsers();
		Load_Lobby_Options(window, &Session.LAN); // RA2: as above
		if (Session.MPGameMode != NULL && !Session.MPGameMode->vt_entry_A0()) { // RA2: as above
			LobbyObserverSlot = -1; // RA2: as above
			ObserverMode = 0; // RA2: as above
			Update_Observer_Buttons(NULL); // RA2: as above
			Session.Slots[0]->Slot.SpectatorFlag = 0; // RA2: as above
			Session.Slots[0]->Set_Country(Session.MPGameMode->DefaultCountry()); // RA2: as above
			Session.Slots[0]->Set_Color(-2); // RA2: as above
			Fill_Color_Combos(window); // RA2: as above
		}
		Update_Slot_Controls(window, 0, true); // RA2: as above
		Change_Scenario_Slots(window, -1, Session.Options.ScenarioIndex); // RA2: as above
		if (LobbyObserverSlot == 0 && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_A0()) { // RA2: as above
			Session.Slots[0]->Set_Country(-3); // RA2: as above
			Session.Slots[0]->Slot.SpectatorFlag = -1; // RA2: as above
		}
		Refresh_Slot_Images(window); // RA2: as above
		Set_Item_Text(window, Session.MPGameMode); // RA2: as above
		Set_Skirmish_Dialog_Text(window); // RA2: as above
		Load_Random_Map_Preview(window); // RA2: as above
		Options_Dialog_Proc(window, 0x497, wparam, lparam); // RA2: as above
		Net2DisplayGameList();
		return(0);
	case WM_COMMAND:
		switch ((unsigned short)wparam) { // RA2: as above
		case 2: // RA2: as above
			if (!Net2OptionsLocked) { // RA2: as above
				if (MultiplayerMapPreview != NULL) {
					delete MultiplayerMapPreview;
					MultiplayerMapPreview = NULL;
				}
				Save_Lobby_Options(window, &Session.LAN); // RA2: the dialog and the LAN options are passed (the function takes both)
				_netresponse = 2;
			}
			return(0);
		case 0x540: // RA2: as above
			Limit_Name_Text((HWND)lparam); // RA2: as above
			if ((unsigned short)(wparam >> 16) == 0x501) { // RA2: as above
				Send_Chat_Message(window, 0xF0F0, ColorUser); // RA2: as above
			}
			return(0);
		case 0x588: // RA2: as above
			EnableWindow(GetDlgItem(window, 0x588), FALSE); // RA2: as above
			Net2OptionsLocked = true; // RA2: as above
			Save_Lobby_Options(window, &Session.LAN); // RA2: the dialog and the LAN options are passed (the function takes both)
			_netresponse = 0x588; // RA2: as above
			return(0);
		case 0x5AA: { // choose another map // RA2: as above
			if (Net2OptionsLocked) { // RA2: as above
				return(0);
			}
			bool was_skirmish = Session.MPGameMode == NULL || Session.MPGameMode->IsSkirmishAllowed(); // RA2: as above
			bool was_observer = Session.MPGameMode == NULL || Session.MPGameMode->vt_entry_A0(); // RA2: as above
			OwnerDraw::WS_Save_Dialog_Values(window); // RA2: as above
			ShowWindow(window, SW_HIDE);
			IsRandomMap = false;
			if (Scenario_Dialog(MainWindow) == 2) {
				Session.Options.ScenarioIndex = scenario; // RA2: as above
				Set_Scenario_Info_From_Index(scenario); // RA2: as above
				Update_Network_Dialog_Preview(window);
				IsRandomMap = true;
				ShowWindow(window, SW_SHOW);
				Load_Random_Map_Preview(window); // RA2: as above
				Send_Lobby_Options(true, false); // RA2: as above
				InvalidateRect(window, NULL, FALSE);
				return(0);
			}
			if (Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex)) { // RA2: as above
				Change_Scenario_Slots(window, 0, Session.Options.ScenarioIndex); // RA2: as above
				scenario = Session.Options.ScenarioIndex; // RA2: as above
			}
			Session.Options.ScenarioIndex = scenario; // RA2: as above
			IsRandomMap = true;
			ShowWindow(window, SW_SHOW);
			Set_Skirmish_Dialog_Text(window); // RA2: as above
			Load_Random_Map_Preview(window); // RA2: as above
			bool is_skirmish = Session.MPGameMode == NULL || Session.MPGameMode->IsSkirmishAllowed(); // RA2: as above
			bool is_observer = Session.MPGameMode == NULL || Session.MPGameMode->vt_entry_A0(); // RA2: as above
			if (was_observer && !is_observer) { // RA2: as above
				Clear_Observer_Slots(window); // RA2: as above
			}
			if (!was_skirmish || is_skirmish || !Reset_AI_Slots(window)) { // RA2: as above
				Net2DisplayUsers();
				return(0);
			}
			Session.PrecalcMaxAhead = 0;
			GlobalPacketType packet;
			memset(&packet, 0, sizeof(packet)); // RA2: as above
			packet.Command = NET_COMMAND_16; // RA2: as above
			strcpy(packet.Name, Session.Handle);
			Get_Slot_Names(packet.Slots.Names); // RA2: as above
			for (int i = 1; i < Session.Players.Count(); i++) {
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address); // RA2: as above
				Call_Back();
			}
			Net2DisplayUsers();
			Send_Lobby_Options(true, false); // RA2: as above
			return(0);
		}
		case 0x510: // RA2: as above
		case 0x513: // RA2: as above
		case 0x514: // RA2: as above
		case 0x51E: // RA2: as above
		case 0x51F: // RA2: as above
		case 0x520: // RA2: as above
		case 0x521: // RA2: as above
		case 0x6A1: // RA2: as above
			if ((unsigned short)(wparam >> 16) == 1 && !Net2OptionsLocked) { // RA2: as above
				Set_Slot_Country(window, (unsigned short)wparam); // RA2: as above
				Send_Lobby_Options(true, false); // RA2: as above
				Net2DisplayUsers();
			}
			return(0);
		case 0x522: // RA2: as above
		case 0x523: // RA2: as above
		case 0x524: // RA2: as above
		case 0x525: // RA2: as above
		case 0x526: // RA2: as above
		case 0x527: // RA2: as above
		case 0x528: // RA2: as above
		case 0x6A2: // RA2: as above
			if ((unsigned short)(wparam >> 16) == 1 && !Net2OptionsLocked) { // RA2: as above
				Set_Slot_Color(window, (unsigned short)wparam); // RA2: as above
				Send_Lobby_Options(true, false); // RA2: as above
				Net2DisplayUsers();
			}
			return(0);
		case 0x411: // RA2: as above
		case 0x412: // RA2: as above
		case 0x413: // RA2: as above
		case 0x414: // RA2: as above
		case 0x415: // RA2: as above
		case 0x416: // RA2: as above
		case 0x417: // RA2: as above
		case 0x418: // RA2: as above
			if ((unsigned short)(wparam >> 16) == 1 && !Net2OptionsLocked && Is_Host()) { // RA2: as above
				Change_Slot_Type(window, (unsigned short)wparam, Combo_Selected_Data(window, (unsigned short)wparam, -1), 2); // RA2: as above
			}
			return(0);
		}
		return(0);
	}
	return(0);
}


LRESULT CALLBACK Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005BF9B0; the game options of the lobby dialog
{
	if (!Net2OptionsLocked) { // RA2: as above
		switch (message) {
		case 0x497: // RA2: as above
			Fill_Options_Dialog(window, 1); // RA2: as above
			return(0);
		case 0x114: // RA2: as above
			if (GetDlgItem(window, 0x529) == (HWND)lparam) { // RA2: as above
				Session.Options.GameSpeed = 6 - SendDlgItemMessage(window, 0x529, 0x400, 0, 0); // RA2: as above
			}
			if (GetDlgItem(window, 0x402) == (HWND)lparam) { // RA2: as above
				Session.Options.Credits = SendDlgItemMessage(window, 0x402, 0x400, 0, 0); // RA2: as above
			}
			if (GetDlgItem(window, 0x54F) == (HWND)lparam) { // RA2: as above
				Session.Options.UnitCount = SendDlgItemMessage(window, 0x54F, 0x400, 0, 0); // RA2: as above
			}
			return(0);
		case 0x111: // RA2: as above
			switch ((unsigned short)wparam) { // RA2: as above
			case 0x54E: // RA2: as above
				Session.Options.ShortGame = false;
				if (SendDlgItemMessage(window, 0x54E, 0xF0, 0, 0) == 1) { // RA2: as above
					Session.Options.ShortGame = true;
					if (!Session.Options.Bases) {
						Session.Options.Bases = true;
						SendDlgItemMessage(window, 0x54B, 0xF1, 1, 0); // RA2: as above
					}
				}
				return(0);
			case 0x54D: // RA2: as above
				Session.Options.Goodies = false;
				if (SendDlgItemMessage(window, 0x54D, 0xF0, 0, 0) == 1) { // RA2: as above
					Session.Options.Goodies = true;
				}
				return(0);
			case 0x3E8: // RA2: as above
				Session.Options.MCVRedeploy = false;
				if (SendDlgItemMessage(window, 0x3E8, 0xF0, 0, 0) == 1) { // RA2: as above
					Session.Options.MCVRedeploy = true;
				}
				return(0);
			case 0x624: // RA2: as above
				Session.Options.BridgeDestruction = false;
				if (SendDlgItemMessage(window, 0x624, 0xF0, 0, 0) == 1) { // RA2: as above
					Session.Options.BridgeDestruction = true;
				}
				return(0);
			case 0x69D: // RA2: as above
				Session.Options.BuildOffAlly = false; // RA2: as above
				if (SendDlgItemMessage(window, 0x69D, 0xF0, 0, 0) == 1) { // RA2: as above
					Session.Options.BuildOffAlly = true; // RA2: as above
				}
				return(0);
			case 0x69A: // RA2: as above
				Session.Options.SWAllowed = false; // RA2: as above
				if (SendDlgItemMessage(window, 0x69A, 0xF0, 0, 0) == 1) { // RA2: as above
					Session.Options.SWAllowed = true; // RA2: as above
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
#ifdef RA2_NON_MATCHING // not exact: 1034 bytes differ (adopted text)
int Request_To_Join(int join_index, bool observer) // RA2: as above
{
	if (join_index < 1) { // RA2: as above
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_MUST_SELECT_GAME", NULL, "D:\\Ra2\\netdlg2.cpp", 3423)); // RA2: as above
		VocClass::VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL); // RA2: as above
		return(0);
	}
	if (Session.Games.Count() <= 1 || join_index > Session.Games.Count()) { // RA2: as above
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_NOTHING_TO_JOIN", NULL, "D:\\Ra2\\netdlg2.cpp", 3430)); // RA2: as above
		VocClass::VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL); // RA2: as above
		return(0);
	}
	if (strlen(Session.Handle) == 0) { // RA2: as above
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_NAME_ERROR", NULL, "D:\\Ra2\\netdlg2.cpp", 3438)); // RA2: as above
		VocClass::VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL); // RA2: as above
		return(0);
	}
	if (strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[0])) == 0 // RA2: as above
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[1])) == 0 // RA2: as above
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[2])) == 0 // RA2: as above
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[3])) == 0 // RA2: as above
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[4])) == 0 // RA2: as above
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[5])) == 0 // RA2: as above
		|| strcmp(Session.Handle, Wide_To_Narrow(LobbyEntryTexts[6])) == 0 // RA2: as above
		|| Is_Lobby_Name(Session.Handle)) { // RA2: as above
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("ERROR:NameIsReservedWord", NULL, "D:\\Ra2\\netdlg2.cpp", 3453)); // RA2: as above
		VocClass::VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL); // RA2: as above
		return(0);
	}
	if (!Session.Games[join_index]->Game.IsOpen) { // RA2: as above
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_GAME_IS_CLOSED", NULL, "D:\\Ra2\\netdlg2.cpp", 3461)); // RA2: as above
		VocClass::VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, NULL); // RA2: as above
		return(0);
	}
	GlobalPacketType packet;
	memset(&packet, 0, sizeof(packet)); // RA2: as above
	packet.Command = NET_QUERY_JOIN; // RA2: as above
	strcpy(packet.Name, Session.Handle);
	strcpy(packet.Serial, SerialNumber); // RA2: as above
	packet.PlayerInfo.House = Session.Chosen_House(); // RA2: as above
	packet.PlayerInfo.Color = Session.Chosen_Color(); // RA2: as above
	packet.PlayerInfo.MinVersion = VerNum.Min_Version(); // RA2: as above
	packet.PlayerInfo.MaxVersion = VerNum.Max_Version(); // RA2: as above
	packet.PlayerInfo.CheatCheck = RulesID; // RA2: as above
	packet.PlayerInfo.ArtCheatCheck = ArtID; // RA2: as above
	packet.PlayerInfo.AICheatCheck = AIID; // RA2: as above
	packet.PlayerInfo.BuildNumber = Build_Number(); // RA2: as above
	packet.PlayerInfo.Flag = observer; // RA2: as above
	Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Games[join_index]->Address); // RA2: as above
	return(1);
}
#endif


/***********************************************************************************************
 * Unjoin_Game -- Sends a SIGN_OFF to the game's owner and the other players                   *
 *=============================================================================================*/
void Unjoin_Game(int game_index) // RA2: as above
{
	GlobalPacketType packet;
	memset(&packet, 0, sizeof(packet)); // RA2: as above
	packet.Command = NET_SIGN_OFF;
	strcpy(packet.Name, Session.Handle);
	for (int i = 1; i < Session.Players.Count(); i++) {
		Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address); // RA2: as above
		Call_Back();
	}
	if (JoinState == JOIN_WAIT_CONFIRM || JoinState == JOIN_CONFIRMED) {
		if (!Session.Games[game_index]->Address.Is_Broadcast()) {
			Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Games[game_index]->Address); // RA2: as above
		}
	}
	Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL); // RA2: as above
	Clear_Slot_Lists(); // RA2: the lobby's slots are cleared
	Fill_Slots(); // RA2: as above
	Clear_Vector(&Session.Players);
	Session.GameName[0] = 0;
}


/***********************************************************************************************
 * Send_Join_Queries -- sends queries for the Join Dialog                                      *
 *=============================================================================================*/
static void Send_Join_Queries(int gamenow, int playernow, int chatnow, int init)
{
	static CDTimerClass<SystemTimerClass> gametimer; // RA2: as above
	static CDTimerClass<SystemTimerClass> playertimer; // RA2: as above
	static CDTimerClass<SystemTimerClass> chattimer; // RA2: as above
	GlobalPacketType packet;

	if (init) {
		gametimer = 120; // RA2: as above
		playertimer = 35; // RA2: as above
		chattimer = 83; // RA2: as above
	}

	if (!gametimer || gamenow) { // RA2: as above
		gametimer = 120; // RA2: as above
		if (WS_Top_Window_ID() != 0xBC || gamenow) { // RA2: as above
			memset(&packet, 0, sizeof(packet)); // RA2: as above
			packet.Command = NET_QUERY_GAME;
			strcpy(packet.Name, Session.Handle);
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL); // RA2: as above
			if (Session.IsBridge) {
				Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
			}
		}
	}

	if ((CurGame > 0 && CurGame < Session.Games.Count() && !playertimer) || playernow) { // RA2: as above
		playertimer = 35; // RA2: as above
		memset(&packet, 0, sizeof(packet)); // RA2: as above
		packet.Command = NET_QUERY_PLAYER;
		strcpy(packet.Name, Wide_To_Narrow(Session.Games[CurGame]->Name)); // RA2: as above
		Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL); // RA2: as above
		if (Session.IsBridge) {
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
		}
	}

	if ((!chattimer && JoinState < JOIN_CONFIRMED) || chatnow) { // RA2: as above
		chattimer = 83; // RA2: as above
		memset(&packet, 0, sizeof(packet)); // RA2: as above
		packet.Command = NET_CHAT_ANNOUNCE;
		strcpy(packet.Name, Session.Handle);
		packet.Chat.ID = Session.UniqueID;
		packet.Chat.Color = Session.PrefColor; // RA2: as above
		Ipx.Send_Global_Message(&packet, sizeof(packet), 0, NULL); // RA2: as above
		if (Session.IsBridge) {
			Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
		}
	}
}


/// <summary>
/// Answers the queries other machines send to this one.
/// </summary>
bool Process_Global_Packet(GlobalPacketType * packet, IPXAddressClass * address) // RA2: as above
{
	if (packet->Command == NET_QUERY_GAME && !Session.NetStealth) { // RA2: as above
		if (wcslen(Session.GameName) > 0) { // RA2: as above
			if (!Session.NetOpen || _stricmp(Session.Handle, Wide_To_Narrow(Session.GameName)) == 0) { // RA2: as above
				if (_stricmp(Session.Handle, Wide_To_Narrow(Session.GameName)) != 0 && !ScenarioActive) { // RA2: as above
					return(true);
				}
				GlobalPacketType answer; // RA2: as above
				memset(&answer, 0, sizeof(answer)); // RA2: as above
				answer.Command = NET_ANSWER_GAME; // RA2: as above
				strcpy(answer.Name, Wide_To_Narrow(Session.GameName)); // RA2: as above
				answer.GameInfo.IsOpen = Session.NetOpen; // RA2: as above
				Ipx.Send_Global_Message(&answer, sizeof(answer), 1, address); // RA2: as above
			}
		}
		return(true);
	}
	if (packet->Command == NET_QUERY_PLAYER) { // RA2: as above
		if (_stricmp(packet->Name, Wide_To_Narrow(Session.GameName)) == 0 // RA2: as above
			&& wcslen(Session.GameName) > 0 && !Session.NetStealth && JoinState >= JOIN_CONFIRMED) { // RA2: as above
			GlobalPacketType answer; // RA2: as above
			memset(&answer, 0, sizeof(answer)); // RA2: as above
			answer.Command = NET_ANSWER_PLAYER; // RA2: as above
			strcpy(answer.Name, Session.Handle); // RA2: as above
			strcpy(answer.Serial, SerialNumber); // RA2: as above
			answer.PlayerInfo.House = Session.Chosen_House(); // RA2: as above
			answer.PlayerInfo.Color = Session.Chosen_Color(); // RA2: as above
			answer.PlayerInfo.NameCRC = Compute_Name_CRC(Session.GameName); // RA2: as above
			answer.PlayerInfo.unknown_24 = Session.LocalSlot; // RA2: as above
			answer.PlayerInfo.Flag = Session.LocalSlot == LobbyObserverSlot; // RA2: as above
			if (Session.Players.Count() == 0) { // RA2: as above
				return(true);
			}
			if (Session.Players[0]->Slot.SpectatorFlag != -1) { // RA2: as above
				answer.PlayerInfo.Flag = 0; // RA2: as above
			}
			Ipx.Send_Global_Message(&answer, sizeof(answer), 1, address); // RA2: as above
			if (_stricmp(packet->Name, Wide_To_Narrow(Session.Players[0]->Name)) == 0) { // RA2: as above
				Send_Lobby_Options(true, true); // RA2: as above
				if (Session.MPGameMode) { // RA2: as above
					Session.MPGameMode->vt_entry_50(); // RA2: as above
				}
			}
			return(true);
		}
	}
	return(false);
}


void Get_Join_Responses(void) // RA2: as above
{
	NodeNameType *who; // RA2: as above
	int i;
	int found;
	int why; // RA2: as above
	wchar_t txt[80]; // RA2: as above
	bool display_users = false;
	bool display_games = false;

	while (_netresponse == 0) {
		Call_Back();
		if (!Ipx.Get_Global_Message(&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID)) { // RA2: as above
			break;
		}
		if (Session.GProductID != 0xAA03) { // RA2: as above
			continue;
		}
		if (Process_Global_Packet(&Session.GPacket, &Session.GAddress)) { // RA2: as above
			continue;
		}

		// The commands of TibSun's Get_Join_Responses, in its order (RA2 adds the map preview,
		// slot and observer packets).
		if (Session.GPacket.Command == NET_PREVIEW_MODE) { // RA2: as above
			if (WS_Top_Window_ID() == 0xBD) { // RA2: as above
				Receive_Random_Map_Preview();
			}
		} else if (Session.GPacket.Command == NET_REQ_PREVIEW) { // RA2: as above
			if (WS_Top_Window_ID() == 0xBC) { // RA2: as above
				Enable_Start_Button(); // RA2: as above
				display_users = true;
				Send_Map_Preview(MultiplayerMapPreview); // RA2: as above
			}
		} else if (Session.GPacket.Command == NET_ANSWER_GAME) { // RA2: as above
			// NET_ANSWER_GAME
			found = 0;
			for (i = 1; i < Session.Games.Count(); i++) {
				if (!_stricmp(Wide_To_Narrow(Session.Games[i]->Name), Session.GPacket.Name)) { // RA2: as above
					found = 1;
					Session.Games[i]->Game.LastTime = TickCount(); // RA2: as above
					if (Session.Games[i]->Game.IsOpen != Session.GPacket.GameInfo.IsOpen) {
						display_games = true;
						Session.Games[i]->Game.IsOpen = Session.GPacket.GameInfo.IsOpen;
						if (Session.Games[i]->Game.IsOpen) {
							Session.Games[i]->Address = Session.GAddress;
						}
						if (JoinState < JOIN_CONFIRMED) {
							if (Session.Games[i]->Game.IsOpen) {
								swprintf(txt, StringTable::LoadString("TXT_S_FORMED_NEW_GAME", NULL, "D:\\Ra2\\netdlg2.cpp", 3940), Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
								VocClass::PlayGlobal(Rule->GameForming, 1.0f, 0x2000, 0); // RA2: as above
							} else {
								swprintf(txt, StringTable::LoadString("TXT_GAME_NOW_IN_PROGRESS", NULL, "D:\\Ra2\\netdlg2.cpp", 3945), Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
								VocClass::PlayGlobal(Rule->GameClosed, 1.0f, 0x2000, 0); // RA2: as above
							}
							Lobby_Chat_Printf(ColorSystem, txt); // RA2: as above
						}
					}
					break;
				}
			}
			if (found == 0) { // RA2: as above
				who = new NodeNameType;
				wcscpy(who->Name, Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
				who->Address = Session.GAddress;
				who->Game.IsOpen = Session.GPacket.GameInfo.IsOpen;
				who->Game.LastTime = TickCount(); // RA2: as above
				Session.Games.Add(who); // RA2: as above
				for (i = 1; i < Session.Chat.Count(); i++) {
					if (Session.Chat[i]->Address == Session.GAddress) { // RA2: as above
						delete Session.Chat[i];
						Session.Chat.Delete(Session.Chat[i]);
						break;
					}
				}
				if (Session.GPacket.GameInfo.IsOpen && JoinState < JOIN_CONFIRMED) {
					swprintf(txt, StringTable::LoadString("TXT_S_FORMED_NEW_GAME", NULL, "D:\\Ra2\\netdlg2.cpp", 3986), Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
					Lobby_Chat_Printf(ColorSystem, txt); // RA2: as above
					VocClass::PlayGlobal(Rule->GameForming, 1.0f, 0x2000, 0); // RA2: as above
				}
				display_users = true;
				display_games = true;
			}
		} else if (Session.GPacket.Command == NET_ANSWER_PLAYER) { // RA2: as above
			// NET_ANSWER_PLAYER
			found = 0;
			for (i = 0; i < Session.Players.Count(); i++) {
				if (Session.Players[i]->Address == Session.GAddress) {
					found = 1;
					break;
				}
			}
			if (Session.Games.Count() && Session.GPacket.PlayerInfo.NameCRC != (int)Compute_Name_CRC(Session.GameName)) { // RA2: as above
				found = 1;
			}
			if (!_stricmp(Session.Handle, Session.GPacket.Name)) { // RA2: as above
				found = 1;
			}
			if (found == 0) { // RA2: as above
				who = new NodeNameType;
				memset(who, 0, sizeof(*who));
				wcscpy(who->Name, Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
				strcpy(who->Slot.SerialPad, Session.GPacket.Serial); // RA2: as above
				who->Address = Session.GAddress;
				who->Set_Country(Session.GPacket.PlayerInfo.House); // RA2: as above
				who->Set_Color(Session.GPacket.PlayerInfo.Color); // RA2: as above
				if (Session.GPacket.PlayerInfo.Flag) { // RA2: as above
					who->Slot.SpectatorFlag = -1; // RA2: as above
				}
				Session.Players.Add(who);
				Set_Player_Slot(Session.GPacket.PlayerInfo.unknown_24, who); // RA2: as above
				for (i = 1; i < Session.Chat.Count(); i++) {
					if (Session.Chat[i]->Address == Session.GAddress) { // RA2: as above
						delete Session.Chat[i];
						Session.Chat.Delete(Session.Chat[i]);
						break;
					}
				}
				Enable_Start_Button(); // RA2: as above
				HWND dialog = WS_Find_Dialog(0xBD); // RA2: as above
				Enable_Player_Combos(dialog, true); // RA2: as above
				DebugString("New Player");
				display_users = true;
				if (JoinState >= JOIN_CONFIRMED) { // RA2: as above
					VocClass::PlayGlobal(Rule->PlayerJoined, 1.0f, 0x2000, 0); // RA2: as above
				}
			}
		} else if (Session.GPacket.Command == NET_COMMAND_16) { // RA2: as above
			if (!Is_Host()) { // RA2: as above
				Set_Slot_Names(Session.GPacket.Slots.Names); // RA2: as above
				display_users = true;
			}
		} else if (Session.GPacket.Command == NET_COMMAND_28) { // RA2: as above
			if (!Is_Host()) { // RA2: as above
				Handle_Start_Reminder(); // RA2: as above
			}
		} else if (Session.GPacket.Command == NET_CONFIRM_JOIN) { // RA2: as above
			// NET_CONFIRM_JOIN
			if (JoinState != JOIN_CONFIRMED) { // RA2: as above
				wcscpy(Session.GameName, Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
				Session.Set_Side(Session.GPacket.PlayerInfo.House, false); // RA2: as above
				Session.Set_Color(Session.GPacket.PlayerInfo.Color, false); // RA2: as above
				JoinState = JOIN_CONFIRMED;
				Session.HostAddress = Session.GAddress;
				Clear_Vector(&Session.Players);
				who = new NodeNameType;
				memset(who, 0, sizeof(*who));
				wcscpy(who->Name, Narrow_To_Wide(Session.Handle)); // RA2: as above
				who->Set_Country(Session.Chosen_House()); // RA2: as above
				who->Set_Color(Session.Chosen_Color()); // RA2: as above
				LobbyObserverSlot = Session.GPacket.PlayerInfo.Flag; // RA2: as above
				if (Session.GPacket.PlayerInfo.unknown_24 == (int)Session.GPacket.PlayerInfo.Flag && Session.GPacket.PlayerInfo.House == -3) { // RA2: as above
					who->Slot.SpectatorFlag = -1; // RA2: as above
				}
				Session.Players.Add(who);
				Net2JoinListEnabled = false; // RA2: as above
				WS_Destroy_Dialog(0, 0);
				_netresponse = 0;
				Clear_Slot_Lists(); // RA2: as above
				Reset_Slots(); // RA2: as above
				Seat_First_Player(Session.GPacket.PlayerInfo.unknown_24); // RA2: as above
				HWND dialog = (HWND)WS_Create_Dialog(ProgramInstance, 0xBD, MainWindow, (DLGPROC)MPlayer_Guest_Dialog_Proc, 0); // RA2: as above
				Show_Dialog(dialog, 0xBD); // RA2: as above
				ShowWindow(dialog, 1); // RA2: as above
				display_users = true;
				Send_Join_Queries(1, 1, 1, 0);
			}
		} else if (Session.GPacket.Command == NET_REJECT_JOIN) { // RA2: as above
			// NET_REJECT_JOIN
			why = REJECT_DUPLICATE_NAME;
			if (JoinState == JOIN_CONFIRMED) {
				GlobalPacketType packet;
				memset(&packet, 0, sizeof(packet)); // RA2: as above
				packet.Command = NET_SIGN_OFF;
				strcpy(packet.Name, Session.Handle);
				for (i = 1; i < Session.Players.Count(); i++) {
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address); // RA2: as above
					Call_Back();
				}
				Ipx.Send_Global_Message(&packet, sizeof(packet), 0, 0); // RA2: as above
				if (Session.IsBridge) {
					Ipx.Send_Global_Message(&packet, sizeof(packet), 0, &Session.BridgeNet);
				}
				while (Ipx.Global_Num_Send() > 0 && Ipx.Service() != 0) {
					Call_Back();
				}
				Session.GameName[0] = 0;
				Clear_Slot_Lists(); // RA2: as above
				Fill_Slots(); // RA2: as above
				Clear_Vector(&Session.Players);
				Clear_Vector(&Session.Chat);
				CurGame = 0;
				Net2JoinListEnabled = true; // RA2: as above
				JoinState = JOIN_REJECTED;
				why = REJECT_BY_OWNER;
				display_users = true;
			} else if (JoinState == JOIN_WAIT_CONFIRM) { // RA2: as above
				why = Session.GPacket.Reject.Why; // RA2: as above
				JoinState = JOIN_REJECTED;
			}
			if (JoinState == JOIN_REJECTED) {
				Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_REQUEST_DENIED", NULL, "D:\\Ra2\\netdlg2.cpp", 4239)); // RA2: as above
				VocClass::PlayGlobal(Rule->SystemError, 1.0f, 0x2000, 0); // RA2: as above
				wchar_t const *item = 0; // RA2: as above
				if (why == REJECT_DUPLICATE_NAME) { // RA2: as above
					item = StringTable::LoadString("TXT_NAME_MUSTBE_UNIQUE", NULL, "D:\\Ra2\\netdlg2.cpp", 4245); // RA2: as above
				} else if (why == REJECT_GAME_FULL) { // RA2: as above
					if (ObserverMode == 0) { // RA2: as above
						item = StringTable::LoadString("TXT_GAME_FULL", NULL, "D:\\Ra2\\netdlg2.cpp", 4247); // RA2: as above
					} else {
						item = StringTable::LoadString("TXT_OBSERVER_FULL", NULL, "D:\\Ra2\\netdlg2.cpp", 4249); // RA2: as above
					}
				} else if (why == REJECT_VERSION_TOO_OLD) { // RA2: as above
					item = StringTable::LoadString("TXT_YOURGAME_OUTDATED", NULL, "D:\\Ra2\\netdlg2.cpp", 4251); // RA2: as above
				} else if (why == REJECT_VERSION_TOO_NEW) { // RA2: as above
					item = StringTable::LoadString("TXT_DESTGAME_OUTDATED", NULL, "D:\\Ra2\\netdlg2.cpp", 4253); // RA2: as above
				} else if (why == REJECT_MISMATCH) { // RA2: as above
					item = StringTable::LoadString("TXT_MISMATCH", NULL, "D:\\Ra2\\netdlg2.cpp", 4255); // RA2: as above
				} else if (why == REJECT_DISBANDED) { // RA2: as above
					item = StringTable::LoadString("TXT_GAME_CANCELLED", NULL, "D:\\Ra2\\netdlg2.cpp", 4257); // RA2: as above
				} else if (why == REJECT_DUPLICATE_SERIAL) { // RA2: as above
					item = StringTable::LoadString("TXT_SERIAL_DUP", NULL, "D:\\Ra2\\netdlg2.cpp", 4259); // RA2: as above
				}
				if (item) {
					ODMessageBox(item, 0, Net2Callback, false); // RA2: as above
				}
				if (WS_Top_Window_ID() == 0xBB) { // RA2: as above
					HWND window = WS_Top_Window();
					if (window) { // RA2: as above
						EnableWindow(window, TRUE); // RA2: as above
						HWND button = GetDlgItem(window, 0x541); // RA2: as above
						if (button) { // RA2: as above
							EnableWindow(button, TRUE); // RA2: as above
						}
					}
				} else {
					_netresponse = 2;
				}
				Send_Join_Queries(0, 0, 1, 0);
			}
		} else if (Session.GPacket.Command == NET_PUB_GAMEOPT) { // RA2: as above
			// NET_PUB_GAMEOPT
			for (i = 0; i < Session.Players.Count(); i++) {
				if (!_stricmp(Wide_To_Narrow(Session.Players[i]->Name), Session.GPacket.Name)) { // RA2: as above
					if (i != -1 && !Net2OptionsLocked) { // RA2: as above
						Net2DecodeGameopt(Session.GPacket.Options.Buf, Session.GPacket.Name); // RA2: as above
					}
					break;
				}
			}
		} else if (Session.GPacket.Command == NET_PRIV_GAMEOPT) { // RA2: as above
			// NET_PRIV_GAMEOPT
			char *opts = strdup(&Session.GPacket.Options.Buf[1]); // RA2: as above
			for (i = 0; i < 8; i++) { // RA2: as above
				char *name = Wide_To_Narrow(Session.Slots[i]->Name); // RA2: as above
				if (!_stricmp(name, Session.GPacket.Name)) { // RA2: as above
					if (i != -1 && !Net2OptionsLocked) { // RA2: as above
						char *tok = strtok(opts, ","); // RA2: as above
						int newhouse = 0;
						if (tok) {
							newhouse = atol(tok);
						}
						tok = strtok(0, ","); // RA2: as above
						int newcolor = 0;
						if (tok) {
							newcolor = atol(tok); // RA2: as above
						}
						HWND dialog = WS_Find_Dialog(0xBC); // RA2: as above
						if (!dialog) { // RA2: as above
							dialog = WS_Find_Dialog(0xBD); // RA2: as above
						}
						bool house_changed = Session.Slots[i]->Get_Country() != newhouse; // RA2: as above
						if (house_changed) { // RA2: as above
							Session.Slots[i]->Set_Country(newhouse); // RA2: as above
						}
						bool color_changed = Session.Slots[i]->Get_Color() != newcolor; // RA2: as above
						if (color_changed) { // RA2: as above
							if (Session.Is_Color_Used(newcolor)) { // RA2: as above
								newcolor = -2; // RA2: as above
							}
							Session.Slots[i]->Set_Color(newcolor); // RA2: as above
						}
						if (house_changed || color_changed) { // RA2: as above
							display_users = true;
							Send_Lobby_Options(true, false); // RA2: as above
						}
					}
					break;
				}
			}
			free(opts);
		} else if (Session.GPacket.Command == NET_SIGN_OFF) { // RA2: as above
			// NET_SIGN_OFF
			for (i = 1; i < Session.Games.Count(); i++) {
				if (!_stricmp(Wide_To_Narrow(Session.Games[i]->Name), Session.GPacket.Name) && Session.Games[i]->Address == Session.GAddress) { // RA2: as above
					if (i == CurGame) { // RA2: as above
						Clear_Slot_Lists(); // RA2: as above
						Fill_Slots(); // RA2: as above
						Clear_Vector(&Session.Players);
						if (WS_Top_Window_ID() != 0xBB && WS_Top_Window_ID() == 0xBD) { // RA2: as above
							_netresponse = 2;
						}
					}
					if (JoinState > JOIN_NOTHING && i == CurGame) { // RA2: as above
						JoinState = JOIN_REJECTED;
						why = REJECT_DISBANDED;
					}
					delete Session.Games[i];
					Session.Games.Delete(Session.Games[i]);
					CurGame = i - 1; // RA2: as above
					display_games = true;
					Send_Join_Queries(0, 1, 1, 0);
				}
			}
			bool player_removed = false;
			Release_Slot_By_Address(Session.GAddress); // RA2: as above
			for (i = 0; i < Session.Players.Count(); i++) {
				if (Session.Players[i]->Address == Session.GAddress) {
					delete Session.Players[i];
					Session.Players.Delete(Session.Players[i]);
					player_removed = true;
					if (JoinState >= JOIN_CONFIRMED) { // RA2: as above
						VocClass::PlayGlobal(Rule->PlayerLeft, 1.0f, 0x2000, 0); // RA2: as above
					}
				}
			}
			for (i = 1; i < Session.Chat.Count(); i++) {
				if (Session.Chat[i]->Address == Session.GAddress) { // RA2: as above
					delete Session.Chat[i];
					Session.Chat.Delete(Session.Chat[i]);
				}
			}
			if (player_removed) {
				Enable_Start_Button(); // RA2: as above
				HWND dialog = WS_Find_Dialog(0xBD); // RA2: as above
				Enable_Player_Combos(dialog, true); // RA2: as above
				if (WS_Top_Window_ID() == 0x6B) { // RA2: as above
					HWND window = WS_Top_Window();
					int index = SendDlgItemMessageA(window, 0x6EB, 0x188, 0, 0); // RA2: as above
					if (index != -1) {
						MultiplayerGameMode *row = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6EB, 0x199, index, 0); // RA2: as above
						row->vt_entry_78(); // RA2: as above
					}
				} else {
					Session.MPGameMode->vt_entry_78(); // RA2: as above
				}
			}
			display_users = true;
		} else if (Session.GPacket.Command == NET_GO || Session.GPacket.Command == NET_LOADGAME) { // RA2: as above
			// NET_GO, NET_LOADGAME
			if (JoinState == JOIN_CONFIRMED) {
				Session.MaxAhead = Session.GPacket.Progress.Percent; // RA2: as above
				Session.HostAddress = Session.GAddress;
				Session.NumPlayers = Session.Players.Count();
				_netresponse = 1; // RA2: as above
				if (Session.GPacket.Command == NET_GO) { // RA2: as above
					JoinState = JOIN_GAME_START;
				} else if (Session.GPacket.Command == NET_LOADGAME) { // RA2: as above
					JoinState = JOIN_GAME_START_LOAD;
				}
			}
		} else if (Session.GPacket.Command == NET_CHAT_ANNOUNCE) { // RA2: as above
			// NET_CHAT_ANNOUNCE
			if (Session.GPacket.Chat.ID == Session.UniqueID) {
				continue;
			}
			found = 0;
			for (i = 0; i < Session.Chat.Count(); i++) {
				if (Session.Chat[i]->Address == Session.GAddress) { // RA2: as above
					wcscpy(Session.Chat[i]->Name, Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
					Session.Chat[i]->Chat.LastTime = TickCount(); // RA2: as above
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
				wcscpy(who->Name, Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
				who->Address = Session.GAddress;
				who->Chat.LastTime = TickCount(); // RA2: as above
				who->Chat.LastChance = 0;
				who->Chat.Color = Session.GPacket.Chat.Color;
				Session.Chat.Add(who);
				if (CurGame == 0) {
					display_users = true;
				}
			}
		} else if (Session.GPacket.Command == NET_CHAT_REQUEST) { // RA2: as above
			// NET_CHAT_REQUEST
			if (JoinState != JOIN_WAIT_CONFIRM && JoinState != JOIN_CONFIRMED) {
				GlobalPacketType packet;
				memset(&packet, 0, sizeof(packet)); // RA2: as above
				packet.Command = NET_CHAT_ANNOUNCE;
				strcpy(packet.Name, Session.Handle);
				packet.Chat.ID = Session.UniqueID;
				packet.Chat.Color = Session.PrefColor; // RA2: as above
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
				Call_Back();
			}
		} else if (Session.GPacket.Command == NET_MESSAGE) { // RA2: as above
			// NET_MESSAGE
			int color = Session.GPacket.Message.Color; // RA2: as above
			if (JoinState == JOIN_CONFIRMED) {
				if (Session.GPacket.Message.NameCRC == (int)Compute_Name_CRC(Session.GameName)) { // RA2: as above
					Lobby_Chat_Printf(color, L"[%s] %s", Narrow_To_Wide(Session.GPacket.Name), Session.GPacket.Message.Buf); // RA2: as above
					VocClass::PlayGlobal(Rule->IncomingMessage, 1.0f, 0x2000, 0); // RA2: as above
				}
			} else {
				Lobby_Chat_Printf(color, L"[%s] %s", Narrow_To_Wide(Session.GPacket.Name), Session.GPacket.Message.Buf); // RA2: as above
				VocClass::PlayGlobal(Rule->IncomingMessage, 1.0f, 0x2000, 0); // RA2: as above
			}
		} else if (Session.GPacket.Command == NET_QUERY_JOIN) { // RA2: as above
			// NET_QUERY_JOIN
			GlobalPacketType packet;
			if (!Session.Players.Count()) {
				memset(&packet, 0, sizeof(packet)); // RA2: as above
				packet.Command = NET_REJECT_JOIN;
				packet.Reject.Why = REJECT_DISBANDED; // RA2: as above
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
			} else {
			found = 0;
			int resend = 0; // RA2: as above
			for (i = 1; i < Session.Players.Count(); i++) {
				if (!_stricmp(Wide_To_Narrow(Session.Players[i]->Name), Session.GPacket.Name)) { // RA2: as above
					if (Session.Players[i]->Address != Session.GAddress) {
						found = 1;
					} else {
						resend = 1;
					}
					break;
				}
			}
			if (!_stricmp(Wide_To_Narrow(Session.Players[0]->Name), Session.GPacket.Name)) { // RA2: as above
				found = 1;
			}
			if (found) {
				memset(&packet, 0, sizeof(packet)); // RA2: as above
				packet.Command = NET_REJECT_JOIN;
				packet.Reject.Why = REJECT_DUPLICATE_NAME; // RA2: as above
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
				continue;
			}
			if (!resend) {
				if (Session.GPacket.PlayerInfo.Flag != 1 && Is_All_Slots_Filled()) { // RA2: as above
					memset(&packet, 0, sizeof(packet)); // RA2: as above
					packet.Command = NET_REJECT_JOIN;
					packet.Reject.Why = REJECT_GAME_FULL; // RA2: as above
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
					continue;
				}
				if (Session.GPacket.PlayerInfo.Flag == 1 && Is_Observer_Slot_Full()) { // RA2: as above
					memset(&packet, 0, sizeof(packet)); // RA2: as above
					packet.Command = NET_REJECT_JOIN;
					packet.Reject.Why = REJECT_GAME_FULL; // RA2: as above
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
					continue;
				}
			}
			bool match = true;
			if (!resend) {
				if (Session.GPacket.PlayerInfo.CheatCheck != RulesID) { // RA2: as above
					match = false; // RA2: as above
				}
				if (Session.GPacket.PlayerInfo.AICheatCheck != Rule->Get_AI_Unique_ID()) { // RA2: as above
					match = false; // RA2: as above
				}
				if (Session.GPacket.PlayerInfo.ArtCheatCheck != ArtID) { // RA2: as above
					match = false; // RA2: as above
				}
				if (Session.GPacket.PlayerInfo.BuildNumber != Build_Number()) { // RA2: as above
					match = false; // RA2: as above
				}
			}
			int dups = 0;
			for (i = 0; i < Session.Players.Count(); i++) {
				if (!strcmp(Session.Players[i]->Slot.SerialPad, Session.GPacket.Serial)) { // RA2: as above
					dups++;
				}
			}
			if (!match) {
				memset(&packet, 0, sizeof(packet)); // RA2: as above
				packet.Command = NET_REJECT_JOIN;
				packet.Reject.Why = REJECT_MISMATCH; // RA2: as above
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
				continue;
			}
			if (!resend) {
				unsigned long version = VerNum.Clip_Version(Session.GPacket.PlayerInfo.MinVersion, Session.GPacket.PlayerInfo.MaxVersion); // RA2: as above
				Session.PlayingAgainstVersion = version;
				if (version == 0) {
					memset(&packet, 0, sizeof(packet)); // RA2: as above
					packet.Command = NET_REJECT_JOIN;
					packet.Reject.Why = REJECT_VERSION_TOO_OLD; // RA2: as above
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
					continue;
				}
				if (version == 0xFFFFFFFF) { // RA2: as above
					memset(&packet, 0, sizeof(packet)); // RA2: as above
					packet.Command = NET_REJECT_JOIN;
					packet.Reject.Why = REJECT_VERSION_TOO_NEW; // RA2: as above
					Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
					continue;
				}
				Session.CommProtocol = VerNum.Version_Protocol(version);
				who = new NodeNameType;
				memset(who, 0, sizeof(*who));
				wcscpy(who->Name, Narrow_To_Wide(Session.GPacket.Name)); // RA2: as above
				who->Address = Session.GAddress;
				if (Session.MPGameMode) { // RA2: as above
					who->Set_Country(!Session.MPGameMode->IsCountryAllowed(Session.GPacket.PlayerInfo.House) ? Session.MPGameMode->DefaultCountry() : Session.GPacket.PlayerInfo.House); // RA2: as above
				}
				strcpy(who->Slot.SerialPad, Session.GPacket.Serial); // RA2: as above
				int color = Session.GPacket.PlayerInfo.Color; // RA2: as above
				if (Is_Color_Taken(color)) { // RA2: as above
					color = -2; // RA2: as above
				}
				if (Session.GPacket.PlayerInfo.Flag == 1) { // RA2: as above
					color = -2; // RA2: as above
					who->Set_Country(-3); // RA2: as above
					who->Slot.SpectatorFlag = -1; // RA2: as above
				} else {
					who->Slot.SpectatorFlag = 0; // RA2: as above
					if (Session.GPacket.PlayerInfo.House == -3) { // RA2: as above
						if (Session.MPGameMode) { // RA2: as above
							who->Set_Country(Session.MPGameMode->DefaultCountry()); // RA2: as above
						} else {
							who->Set_Country(-2); // RA2: as above
						}
					}
				}
				who->Set_Color(color); // RA2: as above
				Session.Players.Add(who);
				int slot = Assign_Slot(who); // RA2: as above
				Enable_Start_Button(); // RA2: as above
				memset(&packet, 0, sizeof(packet)); // RA2: as above
				packet.Command = NET_CONFIRM_JOIN;
				strcpy(packet.Name, Session.Handle);
				display_users = true;
				packet.PlayerInfo.House = who->Get_Country(); // RA2: as above
				packet.PlayerInfo.Color = who->Get_Color(); // RA2: as above
				packet.PlayerInfo.Flag = LobbyObserverSlot; // RA2: as above
				packet.PlayerInfo.unknown_24 = slot; // RA2: as above
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.GAddress); // RA2: as above
				VocClass::PlayGlobal(Rule->PlayerJoined, 1.0f, 0x2000, 0); // RA2: as above
				if (Session.MPGameMode) { // RA2: as above
					int rows = Session.MPGameMode->vt_entry_80(); // RA2: as above
					if (rows != -1) { // RA2: as above
						Session.ScenarioMaxPlayers = rows; // RA2: as above
					}
				}
				Send_Lobby_Options(true, false); // RA2: as above
				GlobalPacketType slots; // RA2: as above
				memset(&slots, 0, sizeof(slots)); // RA2: as above
				slots.Command = NET_COMMAND_16; // RA2: as above
				strcpy(slots.Name, Session.Handle); // RA2: as above
				Get_Slot_Names(slots.Slots.Names); // RA2: as above
				int count = Session.Players.Count(); // RA2: as above
				for (i = 1; i < count; i++) { // RA2: as above
					Ipx.Send_Global_Message(&slots, sizeof(slots), 1, &Session.Players[i]->Address); // RA2: as above
					Call_Back();
				}
				Handle_Mode_Selection(); // RA2: as above
			}
			}
		}
	}
	if (display_users) {
		Net2DisplayUsersFull(false); // RA2: as above
	}
	if (display_games) {
		Net2DisplayGameList();
	}
}


LRESULT CALLBACK MPlayer_Guest_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005C2330; the lobby dialog of a joined player
{
	switch (message) {
	case WM_DESTROY:
		if (Progress.Has_Dialog()) { // RA2: as above
			Progress.Clear_Dialog(); // RA2: as above
		}
		if (MultiplayerMapPreview != NULL) {
			delete MultiplayerMapPreview;
			MultiplayerMapPreview = NULL;
			return(0);
		}
		break;
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam); // RA2: as above
		// fall through
	case WM_ERASEBKGND:
		return(1);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window); // RA2: as above
		if (MultiplayerMapPreview != NULL) {
			MultiplayerMapPreview->Blit_Preview(window);
		}
		if (Progress.Has_Dialog()) { // RA2: as above
			Progress.Display_Progress(Point2D(-1, -1)); // RA2: as above
		}
		Slide_Pending_Dialog(window); // RA2: as above
		ValidateRect(window, NULL);
		return(0);
	case WM_NCHITTEST: { // the tool tip of the control under the cursor // RA2: as above
		HWND tip = GetDlgItem(window, 0x695); // RA2: as above
		if (tip != NULL) { // RA2: as above
			POINT point; // RA2: as above
			point.x = lparam & 0xFFFF; // RA2: as above
			point.y = (unsigned long)lparam >> 16; // RA2: as above
			RECT rect; // RA2: as above
			GetWindowRect(window, &rect); // RA2: as above
			point.x -= rect.left; // RA2: as above
			point.y -= rect.top; // RA2: as above
			wchar_t const * text = L""; // RA2: as above
			HWND control = ChildWindowFromPointEx(window, point, 1); // RA2: as above
			if (control != NULL) { // RA2: as above
				char const * label = OwnerDraw::Control_Tooltip_Label((unsigned int)window, control); // RA2: as above
				if (label != NULL) { // RA2: as above
					text = StringTable::LoadString(label, NULL, "D:\\Ra2\\netdlg2.cpp", 3143); // RA2: as above
				}
			}
			SendMessage(tip, 0x4B2, 0, (LPARAM)text); // RA2: as above
			return(0);
		}
		break;
	}
	case WM_COMMAND:
		switch ((unsigned short)wparam) { // RA2: as above
		case 2: // RA2: as above
			if (!Net2OptionsLocked) { // RA2: as above
				_netresponse = 2;
				return(0);
			}
			break;
		case 0x510: // RA2: as above
		case 0x513: // RA2: as above
		case 0x514: // RA2: as above
		case 0x51E: // RA2: as above
		case 0x51F: // RA2: as above
		case 0x520: // RA2: as above
		case 0x521: // RA2: as above
		case 0x522: // RA2: as above
		case 0x523: // RA2: as above
		case 0x524: // RA2: as above
		case 0x525: // RA2: as above
		case 0x526: // RA2: as above
		case 0x527: // RA2: as above
		case 0x528: // RA2: as above
		case 0x6A1: // RA2: as above
		case 0x6A2: // RA2: as above
			if ((unsigned short)(wparam >> 16) == 1) { // RA2: as above
				int id = (unsigned short)wparam; // RA2: as above
				int side_player = Side_Combo_Player(id); // RA2: as above
				int color_player = Color_Combo_Player(id); // RA2: as above
				int player = side_player > color_player ? side_player : color_player; // RA2: as above
				int side_id = Player_Side_Combo_ID(player); // RA2: as above
				int color_id = Player_Color_Combo_ID(player); // RA2: as above
				if (player == side_player) { // RA2: as above
					Set_Slot_Country(window, side_id); // RA2: as above
				} else {
					Set_Slot_Color(window, color_id); // RA2: as above
				}
				char text[64]; // RA2: as above
				int side = Side_Combo_Data(window, side_id, -1); // RA2: as above
				int color = Color_Combo_Data(window, color_id, -1); // RA2: as above
				sprintf(text, "R%d,%d", side, color); // RA2: as above
				Send_Player_Choice(Wide_To_Narrow(Session.GameName), text); // RA2: as above
				return(0);
			}
			break;
		case 0x540: // RA2: as above
			Limit_Name_Text((HWND)lparam); // RA2: as above
			if ((unsigned short)(wparam >> 16) == 0x501) { // RA2: as above
				Send_Chat_Message(window, ColorMe, ColorUser); // RA2: as above
				return(0);
			}
			break;
		case 0x59F: // RA2: as above
			if (Session.Players.Count() != 0) { // RA2: as above
				Session.Players[0]->Player.Status = 1;
				char text[64]; // RA2: as above
				sprintf(text, "A1"); // RA2: as above
				Send_Host_Option(text); // RA2: as above
				Enable_Player_Combos(window, false); // RA2: as above
				HWND button = GetDlgItem(window, 0x59F); // RA2: as above
				EnableWindow(button, FALSE); // RA2: as above
				InvalidateRect(button, NULL, FALSE); // RA2: as above
				SendMessage(button, 0x4DC, 0, 0); // RA2: as above
				Net2DisplayUsersFull(false); // RA2: as above
				return(0);
			}
			break;
		}
		break;
	case 0x497: // RA2: as above
		Fill_Side_Combos(window); // RA2: as above
		Init_Color_Entries(); // RA2: as above
		Fill_Color_Combos(window); // RA2: as above
		Lock_Slots(window, Session.LocalSlot); // RA2: as above
		Select_Player_Side(window, Session.LocalSlot); // RA2: as above
		Select_Player_Color(window, Session.LocalSlot); // RA2: as above
		EnableWindow(GetDlgItem(window, 0x59F), FALSE); // RA2: as above
		LobbyDialogFlag = false; // RA2: as above
		Net2SetAccept(NULL, 0); // RA2: as above
		Net2DisplayUsersFull(false); // RA2: as above
		Net2DisplayGameList();
		Fill_Options_Dialog(window, 1); // RA2: as above
		Set_Scenario_Info_From_Index(-1); // RA2: as above
		Session.ScenarioMaxPlayers = 8; // RA2: as above
		return(0);
	case 0x4E9: { // the tool tip text of a combo box item // RA2: as above
		ToolTipTextRequest * request = (ToolTipTextRequest *)lparam; // RA2: as above
		request->Text = NULL; // RA2: as above
		if (request->Window != NULL && request->Item != -1) { // RA2: as above
			int id = GetDlgCtrlID(request->Window); // RA2: as above
			if (Control_Slot(id) == -1) { // RA2: as above
				if (Side_Combo_Player(id) == -1) { // RA2: as above
					if (Color_Combo_Player(id) != -1) { // RA2: as above
						request->Text = Color_Name(Color_Combo_Data(window, id, request->Item)); // RA2: as above
						return(1);
					}
				} else {
					int side = Side_Combo_Data(window, id, request->Item); // RA2: as above
					if (side != -1) { // RA2: as above
						request->Text = Side_Name(side); // RA2: as above
						return(1);
					}
				}
			} else {
				request->Text = Entry_Type_Name((NodeNameType *)Combo_Selected_Data(window, id, request->Item)); // RA2: as above
			}
		}
		return(1);
	}
	case 0x4EC: // RA2: as above
		Dialog_On_OD_4EC(window); // RA2: as above
		break;
	}
	return(0);
}



void Join_Lobby_Start_Reminder(void) // RA2: 0x005C28E0; reminds the joiner that the host starts the game
{
	HWND dialog = WS_Top_Window(); // RA2: as above
	HWND item = GetDlgItem(dialog, 0x59F); // RA2: as above
	if (Net2GetAccept(NULL) != 1) { // RA2: as above
		SendMessage(item, 0x4DC, 0, 1); // RA2: as above
		Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostGameStartJoiner", NULL, "D:\\Ra2\\netdlg2.cpp", 4958)); // RA2: as above
	}
}


void __cdecl Join_Lobby_Reset_Ready(void) // RA2: 0x005C2940; sends message 0x4DC to the ready button of the joiner's lobby
{
	HWND dialog = WS_Top_Window(); // RA2: as above
	HWND button = GetDlgItem(dialog, 0x59F); // RA2: as above
	SendMessage(button, 0x4DC, 0, 0); // RA2: as above
}
