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
#include "rect.h"
#include "geometry.h"

#include "_keyboar.h"
#include "_map.h"
#include "_rules.h"
#include "arraylist.h"
#include "dict.h"
#include "conquer.h"
#include "data.h"
#include "dbgprint.h"
#include "globals.h"
#include "goptions.h"
#include "ini.h"
#include "rcptr.h"
#include "rules.h"
#include "init.h"
#include "msgloop.h"
#include "ipxmgr.h"
#include "language\language.h"
#include "netshare.h"
#include "ownrdraw.h"
#include "playerprofile.h"
#include "scenario.h"
#include "srfcache.h"
#include "session.h"
#include "theme.h"
#include "widewstring.h"
#include "windlg.h"
#include "winstub.h"
#include "wolapi.h"
#include "wonline.h"
#include "wstring.h"



#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <shellapi.h>
#include <windows.h>
#undef LoadString
#include "stringtable.h"





#define OD_INSERTSTRING (WM_USER + 199)
#define OD_FINDSTRING (WM_USER + 197)

#define OD_INSERTSTRINGW (WM_USER + 204)
#define OD_SETTEXT (WM_USER + 180)
#define APP_REG_KEY "SOFTWARE\\Westwood\\Red Alert 2"

void Spaces_To_Underscores(char * text)
{
	for (char * space = strchr(text, ' '); space != NULL; space = strchr(space, ' ')) {
		*space = '_';
	}
}

#if 0
bool Is_User_Message_Shown(User const * user)
{
	if (!user) return(false);
	if (user->flags & 1) return(true);
	if (WS_Find_Dialog(0xC2)) return(true);
	if (WS_Find_Dialog(0xC9)) return(true);
	return g_DisplayLatin != 0;
}
#endif


#if 0
bool Is_Text_Shown(unsigned char const * flags, WideWstring const & text)
{
	if (flags == NULL) {
		return(false);
	}
	if (*flags & 1) {
		return(true);
	}
	if (WS_Find_Dialog(0xC2) != NULL) {
		return(true);
	}
	if (WS_Find_Dialog(0xC9) != NULL) {
		return(true);
	}
	if (text.Is_Narrow()) {
		return(g_DisplayLatin != 0);
	}
	return(g_DisplayAsian != 0);
}
#endif


bool Wait_Callback_Thunk(void)
{
	return(WOL_Wait_Callback());
}

//	SKU, reported to WOLAPI for the purpose of finding patches.
IPXAddressClass User_To_IPX_Address(User const * user)
{
	NetNumType net;
	NetNodeType node;
	memset(net, 0, sizeof(net));
	memset(node, 0, sizeof(node));
	unsigned long ip = ntohl(user->ipaddr);
	node[3] = (unsigned char)ip;
	node[2] = (unsigned char)(ip >> 8);
	node[1] = (unsigned char)(ip >> 16);
	node[0] = (unsigned char)(ip >> 24);
	IPXAddressClass address(net, node, 0);
	return(address);
}

void Set_Lobby_Music(bool online)
{
	if (online) {
		if (!Theme.Still_Playing()) {
			Theme.Stop();
			Theme.Queue_Song((ThemeType)-2);
			return;
		}
		if (Theme.What_Is_Playing() == Theme.From_Name("INTRO")) {
			Theme.Stop();
			Theme.Queue_Song((ThemeType)-2);
		}
	} else {
		Theme.Queue_Song((ThemeType)-3);
	}
}


#if 0
inline void Set_Match_State(int state, int timeout, int result)
{
	g_MatchResult = result;
	g_MatchState = state;
	g_MatchTimeout = timeout;
	g_MatchStartTime = time(0);
}
#endif


/// <summary>
/// Opens the Find Page dialog, waits for it to close, then restores the input-ignore flag
/// and forces the tactical map to fully redraw.
/// </summary>
#if 0
void DoFindPage(void)
{
	HWND dlg = OwnerDraw::Begin_Dialog(0xCF, (DLGPROC)WOL_Find_Page_Dialog_Proc, 0);
	IgnoreInput = true;
	Keyboard->Clear();
	ShowWindow(dlg, SW_NORMAL);
	WS_Wait_Dialog(dlg, OwnerDraw::Dialog_Message_Handler, false, true, true);
	Keyboard->Clear();
	IgnoreInput = Scen->IsInputLocked;
	Map.Flag_To_Redraw(GS_REDRAW_ALL);
}
#endif


/// <summary>
/// Searches the global user list for an entry whose name matches the given name
/// case-insensitively.
/// </summary>
/// <param name="name">Name to search for.</param>
/// <returns>Index of the matching user in g_UserList, or -1 if no user was found.</returns>
void Open_Tournament_Page(void)
{
	ViewHTML("http://www.westwood.com/westwoodonline/tournaments/redalert2/", 0);
}

#if 0
void Open_Registration_Page(void)
{
	char url[256];
	sprintf(url, "http://games2.westwood.com/cgi-bin/cgiclient?register&request=reg_menu&LANGCODE=%d", g_LanguageCode);
	ViewHTML(url, 0);
}
#endif


#if 0
int Player_Name_To_Index(Wstring const & name)
{
	int i;
	User * user = 0;
	for (i = 0; i < g_UserList.length(); i++) {
		g_UserList.getPointer(&user, i);
		if (stricmp(name.text(), (char *)user->name) == 0) {
			return(i);
		}
	}
	return(-1);
}
#endif


#if 0
bool Is_Channel_Tournament(Channel const * channel)
{
	if (Session.Type != GAME_INTERNET) return(false);
	return(channel->tournament || Is_Channel_Flag_8(channel));
}
#endif


#if 0
bool Is_Current_Channel_Tournament(void)
{
	return(Is_Channel_Tournament(&g_CurrentChannel));
}
#endif


#if 0
bool Is_Current_Channel_Flag_8(void)
{
	return(Is_Channel_Flag_8(&g_CurrentChannel));
}
#endif


#if 0
bool Is_Channel_Flag_8(Channel const * channel)
{
	if (Session.Type != GAME_INTERNET) return(false);
	return((channel->reserved >> 8) & 1);
}
#endif


bool Is_Channel_Flag_11(Channel const * channel)
{
	if (Session.Type != GAME_INTERNET) return(false);
	return((channel->reserved >> 11) & 1);
}

bool Is_Channel_Flag_12(Channel const * channel)
{
	if (Session.Type != GAME_INTERNET) return(false);
	return((channel->reserved >> 12) & 1);
}




#if 0
void Send_Player_List(void)
{
	char text[256];
	strcpy(text, "L");
	for (int i = 0; i < 8; ++i) {
		NodeNameType * entry = Session.Slots[i];
		Wstring name;
		if (Is_Real_Player_Entry(entry)) {
			name = Wstring_Narrow(entry->Name);
		} else {
			name = Lobby_Entry_Name(entry);
		}
		strncat(text, name.get(), 256);
		strncat(text, ",", 256);
	}
	g_pChat->RequestPublicGameOptions(text);
}
#endif


/// <summary>
/// Displays a web page in the player's own browser.
/// This routine hands the page to whatever program the system has registered for HTML and
/// then waits on it, pumping the wait callback throughout so the Westwood Online connection
/// does not lapse while the player reads. The game window is brought back to the front once
/// the browser is finished with. If no browser is installed the player is told so.
/// </summary>
/// <param name="name">URL or page name passed to the browser as the "[open]" argument.</param>
/// <param name="no_ask">Should the browser be launched without asking the player first?</param>
void ViewHTML(const char * name, int no_ask)
{
	/*
	 * Create a throwaway HTML file so FindExecutable can tell us which browser handles it.
	 */
	char filename[160];
	tmpnam(filename);
	strcat(filename, ".html");

	FILE * file = fopen(filename, "w");
	fprintf(file, "<title>Hi there.</title>");
	fclose(file);

	char exeName[MAX_PATH + 1];
	int result = (int)FindExecutable(filename, NULL, exeName);

	_unlink(filename);

	/// Ask the user for confirmation (unless no_ask forces an immediate launch), then spawn the browser.
	if (result > 32) {
		if (!no_ask && ODMessageBox(StringTable::LoadString("TXT_LAUNCHBROWSER", NULL, "D:\\Ra2\\wonline.cpp", 1172), MB_YESNO, Wait_Callback_Thunk, false) != IDYES) {
			return;
		}

		char commandLine[MAX_PATH + 10];
		sprintf(commandLine, "[open] %s", name);

		STARTUPINFO startupInfo;
		memset(&startupInfo, 0, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);

		PROCESS_INFORMATION processInfo;

		BOOL createSuccess = CreateProcess(exeName, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);

		if (createSuccess) {
			HANDLE process = processInfo.hProcess;
			if (process) {
				int done_waiting = FALSE;

				WOL_Wait_Callback();

				WaitForInputIdle(process, 5000);

				/// Wait for the browser to exit, or for the game window to regain focus.
				do {
					WOL_Wait_Callback();
					Sleep(500);

					DWORD exitCode;
					GetExitCodeProcess(process, &exitCode);
					if (exitCode != STILL_ACTIVE) {
						done_waiting = TRUE;
					}

					if (GameInFocus || GetTopWindow(NULL) == MainWindow || GetForegroundWindow() == MainWindow) {
						done_waiting = TRUE;
					}
					WOL_Wait_Callback();
				} while (!done_waiting);

				/*
				 * Bring the game window back to the foreground now that the browser is done.
				 */
				WOL_Wait_Callback();

				if (GetTopWindow(NULL) != MainWindow) {
					WOL_Wait_Callback();
					SetForegroundWindow(MainWindow);
					ShowWindow(MainWindow, SW_RESTORE);
					WOL_Wait_Callback();
				}

				int waits = 10;
				while (waits) {
					Sleep(100);
					WOL_Wait_Callback();
					waits--;
				}
			}
		}
	} else {
		/*
		 * No handler found for this file type -- tell the user there's no browser installed.
		 */
		ODMessageBox(StringTable::LoadString("TXT_NOBROWSER", NULL, "D:\\Ra2\\wonline.cpp", 1240), MB_OK, Wait_Callback_Thunk, false);
	}
}

/// <summary>
/// Determines the sort order of two channel users.
/// Channel owners come first, then users with voice, then everyone else; equals are listed
/// alphabetically. This is the order the user list box is kept in.
/// </summary>
/// <returns>Returns with a negative value if the u1 user sorts first, a positive value if
/// the u2 one does, and zero if the two cannot be told apart.</returns>
#if 0
void Read_WOL_Settings(void)
{
	g_AllowPage = ConfigINI.Get_Int("WOnline", "AllowPage", TRUE);
	g_AllowFind = ConfigINI.Get_Int("WOnline", "AllowFind", TRUE);
	g_LangFilter = ConfigINI.Get_Int("WOnline", "LangFilter", TRUE);
	g_LobbyMusic = ConfigINI.Get_Int("WOnline", "LobMusic", TRUE);
	g_ShowAllChannels = ConfigINI.Get_Int("WOnline", "ShowAll", TRUE);
	g_DisplayAsian = ConfigINI.Get_Int("WOnline", "DisplayAsian", TRUE);
	g_DisplayLatin = ConfigINI.Get_Int("WOnline", "DisplayLatin", TRUE);
	Load_Observer_Mode();
}
#endif


/// <summary>
/// Writes the current WOL lobby settings (paging, finding, language filter, lobby music,
/// show-all) to the config INI and saves it to disk.
/// </summary>
#if 0
void Write_WOL_Settings(void)
{
	RawFileClass file("RA2.INI");
	ConfigINI.Put_Int("WOnline", "AllowPage", g_AllowPage);
	ConfigINI.Put_Int("WOnline", "AllowFind", g_AllowFind);
	ConfigINI.Put_Int("WOnline", "LangFilter", g_LangFilter);
	ConfigINI.Put_Int("WOnline", "LobMusic", g_LobbyMusic);
	ConfigINI.Put_Int("WOnline", "ShowAll", g_ShowAllChannels);
	ConfigINI.Put_Int("WOnline", "DisplayAsian", g_DisplayAsian);
	ConfigINI.Put_Int("WOnline", "DisplayLatin", g_DisplayLatin);
	ConfigINI.Save(file, false);
	Save_Observer_Mode();
}
#endif


/// <summary>
/// Applies the current WOL settings to the active chat session: pushes the language filter
/// and find/page flags to the chat client, and starts, continues, or fades out the lobby
/// music depending on the LobMusic setting.
/// </summary>
#if 0
void Apply_WOL_Settings(void)
{
	if (g_pChat) {
		g_pChat->SetLangFilter(g_LangFilter);
		g_pChat->SetFindPage(g_AllowFind, g_AllowPage);
	}
	Update_Observer_Buttons(NULL);

		/// Manage the lobby music according to the LobMusic setting.
	bool music = g_LobbyMusic != 0;
	if (music && !Theme.Still_Playing()) {
		Theme.Stop();
		Theme.Queue_Song((ThemeType)-2);
	} else if (music) {
		if (Theme.What_Is_Playing() == Theme.From_Name("INTRO")) {
			Theme.Stop();
			Theme.Queue_Song((ThemeType)-2);
		}
	} else if (!music) {
		Theme.Queue_Song((ThemeType)-3);
	}
}
#endif


/// <summary>
/// Sends a chat message on the player's behalf.
/// The message goes privately to whoever is selected in the user list box, or out to the
/// whole channel when nothing is selected.
/// </summary>
/// <param name="msg">Text of the message to send.</param>
/// <returns>Returns with 0 if the message went to the whole channel, or 1 if it went
/// privately to the selected users.</returns>
#if 0
int Send_Chat_Message(WideWstring const & msg)
{
	static int sel_user_indexes[128];
	HWND users = GetDlgItem(WS_Top_Window(), IDC_USERS);
	int count = ListBox_GetSelItems(users, (WPARAM)32, (LPARAM)sel_user_indexes);

	if (count == 0) {
		/*
		 * No users selected -- broadcast to the whole channel.
		 */
		g_pChat->RequestPublicMessageWide((unsigned short const *)msg.text());
		return(0);
	} else {
		/*
		 * Build a linked list of the selected users and send them a private message.
		 */
		User * last = NULL;
		User * head = NULL;
		User * item = NULL;
		User source;
		memset(&source, 0, sizeof(source));
		for (int i = 0; i < count; i++) {
			g_UserList.get(source, sel_user_indexes[i]);
			item = new User;
			*item = source;
			item->next = NULL;
			if (i == 0) head = item;
			if (last) last->next = item;
			last = item;
		}

		if (head) g_pChat->RequestPrivateMessageWide(head, (unsigned short const *)msg.text());

		/// Free the temporary user list.
		while (head) {
			item = head;
			head = head->next;
			delete (item);
		}
		return(1);
	}
}
#endif


/// <summary>
/// Sends a chat action on the player's behalf.
/// The action goes privately to whoever is selected in the user list box, or out to the
/// whole channel when nothing is selected.
/// </summary>
/// <param name="msg">Text of the action to send.</param>
/// <returns>Returns with 0 if the action went to the whole channel, or 1 if it went
/// privately to the selected users.</returns>
#if 0
int Send_Chat_Action(WideWstring const & msg)
{
	static int sel_user_indexes[128];
	HWND users = GetDlgItem(WS_Top_Window(), IDC_USERS);
	int count = ListBox_GetSelItems(users, (WPARAM)32, (LPARAM)sel_user_indexes);

	if (count == 0) {
		/*
		 * No users selected -- broadcast to the whole channel.
		 */
		g_pChat->RequestPublicActionWide((unsigned short const *)msg.text());
		return(0);
	} else {
		/*
		 * Build a linked list of the selected users and send them a private action.
		 */
		User * last = NULL;
		User * head = NULL;
		User * item = NULL;
		User source;
		memset(&source, 0, sizeof(source));
		for (int i = 0; i < count; i++) {
			g_UserList.get(source, sel_user_indexes[i]);
			item = new User;
			*item = source;
			item->next = NULL;
			if (i == 0) head = item;
			if (last) last->next = item;
			last = item;
		}

		if (head) g_pChat->RequestPrivateActionWide(head, (unsigned short const *)msg.text());

		/// Free the temporary user list.
		while (head) {
			item = head;
			head = head->next;
			delete (item);
		}
		return(1);
	}
}
#endif


#if 0
static inline bool Same_Player_Name(wchar_t const * first, wchar_t const * second)
{
	char * name = Wide_To_Narrow(first);
	return(stricmp(name, Wide_To_Narrow(second)) == 0);
}
#endif


/// <summary>
/// Encodes the current game options and scenario info, followed by each user's name,
/// house, and color, into a single comma-separated string for transmission over WOL.
/// </summary>
/// <param name="out">Buffer that receives the encoded options string.</param>
#if 0
void Encode_Game_Options(char * out)
{
	static char useroptions[512];

	Wstring name = Session.Options.ScenarioDescription ? Wstring_FromWide(Session.Options.ScenarioDescription) : "";

	/// Encode the game options and scenario info.
	sprintf(out, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%d,%s,%s:",
			Seed, Options.GameSpeed, Session.Options.GameSpeed, Session.Options.Credits, Session.Options.UnitCount,
			Session.Options.ShortGame, Session.Options.SWAllowed, Session.Options.BuildOffAlly, Session.Options.MCVRedeploy,
			Session.Options.Goodies, Session.Options.MPModeIndex, name.get(), Session.ScenarioMaxPlayers, Session.ScenarioIsOfficial,
			Session.ScenarioFileLength, Session.ScenarioFileName, Session.ScenarioDigest);

	/*
	 * Append each user's name, house, and color.
	 * Append each user's name, country, color, address, port and ping.
	 */
	memset(useroptions, 0, sizeof(useroptions));
	for (int i = 0, count = Session.Players.Count(); i < count; i++) {
		NodeNameType * entry = Session.Players[i];
		int country = entry->Get_Country();
		int color = entry->Get_Color();
		int address = 0;
		int port = 0;
		int ping = 0;
		for (int j = 0; j < 8; j++) {
			if (Same_Player_Name(Session.Players[i]->Name, Session.Slots[j]->Name)) {
				NetNumType net;
				NetNodeType node;
				unsigned short socket;
				Session.SlotAddresses[j].Get_Address(net, node, &socket);
				address = *(int *)node;
				port = Session.WolPlayerState[j];
				ping = Session.PlayerPortBase[j];
				break;
			}
		}
		Wstring text(Wstring_Narrow(Session.Players[i]->Name));
		sprintf(useroptions + strlen(useroptions), "%s,%d,%d,%x,%x,%x,", text.get(), country, color, address, port, ping);
	}
	useroptions[strlen(useroptions) - 1] = ':';
	strcat(useroptions, "@:");

	/// Append the computer players.
	for (int k = 0; k < 8; k++) {
		sprintf(useroptions + strlen(useroptions), "%d,%d,%d,", Session.Options.AISlots.Difficulties[k], Session.Options.AISlots.Countries[k], Session.Options.AISlots.Colors[k]);
	}

	strcat(out, useroptions);
}
#endif


/// <summary>
/// Combines the rules, AI, and art CRCs into a single hash used to verify that all players
/// are running matching game data.
/// </summary>
/// <returns>Combined CRC hash.</returns>
#if 0
int GetINIHash(void)
{
	return(g_RuleCRC ^ g_AICRC ^ g_ArtCRC);
}
#endif


/// <summary>
/// Encodes extended game/session info -- build number, data hash, options, and a flag
/// describing which house(s) still have room -- into a comma-separated string for
/// transmission over WOL.
/// </summary>
/// <param name="out">Buffer that receives the encoded info string.</param>
#if 0
void Encode_Channel_ExInfo(char * out)
{
	int i = 0;
	int house1_count = 0;
	int house0_count = 0;

	/// Count how many users are currently on each house.
	for (int j = 0; j < (int)g_CurrentChannel.currentUsers; j++) {
		int house = g_UserInfo[j].house;
		if (house == 0) {
			house0_count++;
		} else if (house == 1) {
			house1_count++;
		}
	}

	/// Flag which house(s) still have free slots.
	if (house1_count < (int)g_CurrentChannel.maxUsers / 2) {
		i = 2;
	}

	if (house0_count < (int)g_CurrentChannel.maxUsers / 2) {
		i++;
	}

	sprintf(out, "%d,%d,%d,%d,%d,%s", Build_Number(), GetINIHash(), Count_AI_Slots(), Is_Observer_Slot_Occupied(), !Is_Observer_Slot_Full(), Session.ScenarioFileName);
}
#endif


/// <summary>
/// Records whether the given user (or the local player if who is NULL) has accepted an
/// invitation/request, then refreshes the user list display.
/// </summary>
/// <param name="who">Name of the user to update, or NULL for the local player.</param>
void Parse_Channel_ExInfo(char * text, int * build, int * hash, int * ai_count, int * observer, int * not_full, Wstring * scenario)
{
	char * token = strtok(text, ",");
	*build = token != NULL ? atol(token) : 0;
	token = strtok(NULL, ",");
	*hash = token != NULL ? atol(token) : 0;
	token = strtok(NULL, ",");
	*ai_count = token != NULL ? atol(token) : 0;
	token = strtok(NULL, ",");
	*observer = token != NULL ? atol(token) : 0;
	token = strtok(NULL, ",");
	*not_full = token != NULL ? atol(token) : 0;
	*scenario = strtok(NULL, ",");
}



/// <summary>
/// Records whether the given user (or the local player if who is empty) has accepted the game
/// options, then refreshes the user list display. When the host is waiting for the only other
/// player of a two player game and that player has accepted, the game is started.
/// </summary>
/// <param name="who">Name of the user to update, empty for the local player.</param>
/// <param name="status">New accepted status to store.</param>
#if 0
void SetPlayerAccepted(Wstring const & who, int status)
{
	Wstring const nick(who.length() > 0 ? who : g_NickName);
	if (nick == g_NickName && status == 0) {
		HWND dialog = WS_Find_Dialog(201);
		Enable_Player_Combos(dialog, true);
	}

	User * user = 0;
	for (int index = 0; index < g_UserList.length(); ++index) {
		g_UserList.getPointer(&user, index);
		if (stricmp(nick.text(), (char *)user->name) == 0) {
			if (index != -1) {
				g_UserInfo[index].accepted = status;
				Draw_Player_List(0, false);
			}
			break;
		}
	}

	bool local = g_IsChannelCreator == 1;
	Wstring const lower = nick.toLower();
	int player_index;
	for (player_index = 0; player_index < Session.Players.Count(); ++player_index) {
		Wstring const player = Wstring_Narrow(Session.Players[player_index]->Name);
		if (player.toLower() == lower) break;
	}
	if (player_index >= Session.Players.Count()) return;
	Session.Players[player_index]->Slot.Unknown77 = status;

	if (local && (Online_Session_State == 2 || Online_Session_State == 1) && !g_GameStartRequested && player_index == 1 && g_UserList.length() == 2) {
		User * node;
		User * head = 0;
		User copy;
		memset(&copy, 0, sizeof(copy));
		for (int user_index = 0; user_index < g_UserList.length(); ++user_index) {
			g_UserList.get(copy, user_index);
			node = new User;
			*node = copy;
			node->next = head;
			head = node;
		}
		Set_Match_State(9, 90, 0);
		g_pChat->RequestGameStart(head);
		g_GameStartRequested = true;
		node = head;
		while (node) {
			head = node->next;
			delete node;
			node = head;
		}
	}
	Draw_Player_List(0, false);
}
#endif


/// <summary>
/// Looks up whether the given user (or the local player if who is NULL) has accepted an
/// invitation/request.
/// </summary>
/// <param name="who">Name of the user to query, or NULL for the local player.</param>
/// <returns>The user's accepted status, or -1 if the user could not be found.</returns>
#if 0
int GetPlayerAccepted(Wstring const & who)
{
	Wstring const nick(who.length() > 0 ? who : g_NickName);
	Wstring const lower = nick.toLower();
	if (Wstring_ToWide(lower) == static_cast<WideWstring const &>(WideWstring(Session.GameName)).toLower()) {
		return(1);
	}
	int found = -1;
	for (int index = 0; index < Session.Players.Count(); index++) {
		Wstring const player = Wstring_Narrow(Session.Players[index]->Name);
		if (player.toLower() == lower) found = index;
	}
	if (found != -1) {
		return(Session.Players[found]->Slot.Unknown77);
	}
	return(-1);
}
#endif


/// <summary>
/// Leaves the current channel (waiting for the leave to complete) and joins the lobby channel
/// selected by Pick_Lobby.
/// </summary>
/// <returns>The index of the joined lobby channel, or -1 if the leave timed out, the join
/// failed, or no lobby was available.</returns>
/// <param name="who">Name of the player whose country/color changed.</param>
/// <param name="house">New country selection.</param>
/// <param name="color">New color selection.</param>
/// <returns>1 if the player's country actually changed, 0 otherwise.</returns>
#if 0
int Assign_House_And_Color(Wstring const & who, int house, int color)
{
	int found = -1;
	Wstring const lower = who.toLower();
	for (int index = 0; index < 8; ++index) {
		Wstring const player_name = Wstring_Narrow(Session.Slots[index]->Name);
		if (lower == player_name.toLower()) {
			found = index;
			break;
		}
	}
	int changed = 0;
	if (found == -1) return(changed);

	HWND window = WS_Find_Dialog(0xC2);
	if (!window) window = WS_Find_Dialog(0xC9);
	NodeNameType * player = Session.Slots[found];
	if (player->Get_Country() != house) {
		changed = 1;
		player->Set_Country(house);
		int side = Player_Side_Combo_ID(found);
		int previous = Side_Combo_Data(window, side, -1);
		if (house == -3) {
			if (previous != -3) {
				Fill_Observer_Combo(window, side);
				int observer_control = Get_Slot_Control_Id(found);
				SendMessage(GetDlgItem(window, observer_control), 0x4F1, 0, 1);
				int color_control = Player_Color_Combo_ID(found);
				Fill_Random_Color_Combo(window, color_control);
				Select_Color_Combo(window, color_control, 8);
			}
		} else if (previous == -3) {
			Fill_Side_Combo(window, side);
			int observer_control = Get_Slot_Control_Id(found);
			SendMessage(GetDlgItem(window, observer_control), 0x4F1, 0, 0);
			int color_control = Player_Color_Combo_ID(found);
			Fill_Color_Combo(window, color_control);
			Select_Color_Combo(window, color_control, -2);
		}
		if (player->Get_Country() == -3) player->Slot.SpectatorFlag = -1;
	}
	if (Session.Slots[found]->Get_Color() != color) player->Set_Color(color);
	if (Session.Slots[found] == Session.Players[0]) Session.PrefColor = color;
	Draw_Player_List(0, false);
	return(changed);
}
#endif


/// <summary>
/// Fills a login dialog in from a remembered login.
/// This routine is used to spare the player from retyping credentials. If the nickname is
/// one Westwood Online has kept, its password and locale are recalled into the dialog's
/// controls along with it; if it is not, the dialog is left as it was.
/// </summary>
/// <param name="name">Nickname to search for among the stored logins, or NULL to skip
/// the lookup.</param>
/// Most names are exposed by debug prints in RA2, rest guessed
enum {
	EV_ABORT,
	EV_GOT_SERVERS,
	EV_CONNECTED,
	EV_CHAN_LIST,
	EV_CHAN_JOINED,
	EV_ERROR,
	EV_EXIT,
	EV_STARTGAME,
	EV_QUITGAME,
	EV_NETIGNORE,
	EV_CHAN_JOIN_FAIL,
	EV_11, // continue waiting?
	EV_12,
	EV_13,
	EV_LOCALE,
	EV_SKIRMISH,
	EV_16,
	EV_17,
	EV_COUNT,
};

/// <summary>
/// Saves a login's nickname, password, and locale into one of the 32 stored login slots.
/// Reuses whichever slot already stores this nickname if one exists, otherwise the first
/// empty slot, otherwise cycles to the slot after the session's last-used nickname slot.
/// </summary>
/// <param name="loginName">Nickname to store.</param>
/// <param name="password">Password to store alongside the nickname.</param>
/// <param name="passwordPlaintext">Whether the password is stored/sent as plaintext.</param>
/// <param name="locale">Locale value to associate with this login.</param>
/// <returns>The result of setting the nick's locale (from SetNickLocale), or the computed
/// slot number if no slot ended up being written to.</returns>
/// <param name="channel_type">0 for the chat channels, 0x21 for the game channels.</param>
/// <param name="autoping">Should the server ping the channels?</param>
/// <returns>True if the request was sent.</returns>
#if 0
bool Request_Channel_List(int channel_type, unsigned char autoping)
{
	DebugString("Requesting channel list type = %d, ping = %d\n", channel_type, autoping);
	if (g_pChat->RequestChannelList(channel_type, autoping) != S_OK) {
		return(false);
	}
	/// Not found -- use the first empty slot instead.
	unsigned int type = channel_type;
	g_ChanListQueue.addTail(type);
	return(true);
}
#endif


/// <summary>
/// Dialog procedure for the "New Game" creation dialog. Handles owner-draw
/// painting, sets up the player-count controls (a slider, or WDT 1on1/2on2
/// radio buttons for World Domination Tour games), enforces tournament and
/// battle-clan slider interlocks, and stores the result on OK.
/// </summary>
/// <param name="win">Dialog window handle.</param>
/// <param name="uMsg">Window message identifier.</param>
/// <param name="wParam">Message-specific first parameter.</param>
/// <param name="lParam">Message-specific second parameter.</param>
/// <returns>TRUE if the message was handled, FALSE otherwise.</returns>
/// <returns>True if the channel list arrived.</returns>
#if 0
bool Wait_Channel_List(void)
{
	{
		WideWstring title(StringTable::LoadString("TXT_REQ_CHANLIST", NULL, "D:\\Ra2\\wonline.cpp", 2042));
		Show_Wait_Window(1, title, false);
	}
	int elapsed = 0;
	for (;;) {
		DWORD event = WaitForMultipleObjects(18, g_WaitEventHandles, FALSE, 100);
		while (event == WAIT_TIMEOUT) {
			elapsed += 100;
			if (elapsed > 30000) {
				swprintf(g_LastErrorMessage, StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 2051));
				SetEvent(g_WaitEventHandles[EV_ABORT]);
				return(false);
			}
			WOL_Wait_Callback();
			event = WaitForMultipleObjects(18, g_WaitEventHandles, FALSE, 100);
		}
		if (event == WAIT_FAILED) {
			Close_Wait_Window(1);
			DebugString("WaitForMultipleObjects() failed in Get_Channel_List\n");
			return(false);
		}
		ResetEvent(g_WaitEventHandles[event]);
		if (event == EV_ERROR) {
			Close_Wait_Window(1);
			return(false);
		}
		if (event == EV_ABORT) {
			Close_Wait_Window(1);
			break;
		}
		if (event == EV_CHAN_LIST) {
			Close_Wait_Window(1);
			return(true);
		}
	}
	return(false);
}
#endif


/// <summary>
/// Nudges the host with a NET_PING packet while waiting in a live game channel as a guest.
/// Sets up the session's player/host address from the channel's first user, then sends a
/// single ping packet to the host address.
/// </summary>
#if 0
bool Queue_Channel_List_Request(int channel_type, unsigned char autoping, bool wait)
{
	if (g_ChanListQueue.length()) {
		bool result = Wait_Channel_List();
		if (!result) return(result);
	}
	Reset_Wait_Events();
	bool requested = Request_Channel_List(channel_type, autoping);
	if (!requested) {
		SetEvent(g_WaitEventHandles[EV_ABORT]);
		return(false);
	}
	if (wait) {
		bool result = Wait_Channel_List();
		return(result);
	}
	return(true);
}
#endif


/// <summary>
/// Shows the World Domination Tour "available servers" dialog and waits for it to close.
/// </summary>
/// <returns>Result code returned by the dialog (see WS_Wait_Dialog/WS_Destroy_Dialog).</returns>
#if 0
bool Request_Chat_Channel_List(bool wait)
{
	if (g_ChanListQueue.length()) {
		bool result = Wait_Channel_List();
		if (!result) return(result);
	}
	for (int i = 0; i < 18; ++i) ResetEvent(g_WaitEventHandles[i]);
	if (!Request_Channel_List(0, 0)) {
		SetEvent(g_WaitEventHandles[EV_ABORT]);
		return(false);
	}
	if (wait) return(Wait_Channel_List());
	return(true);
}
#endif


/// <summary>
/// Dialog procedure for the World Domination Tour "available servers" list. Populates the
/// list box on OD_SUBCLASSED (stripping any "region:" prefix from each server name and
/// pre-selecting the player's preferred server), and on OK stores the chosen server as the
/// new preferred server and saves the multiplayer settings.
/// </summary>
/// <param name="win">Handle of the dialog window.</param>
/// <param name="uMsg">Window message being processed.</param>
/// <param name="wParam">Message-specific parameter.</param>
/// <param name="lParam">Message-specific parameter.</param>
/// <returns>TRUE if the message was handled, FALSE otherwise.</returns>
#if 0
bool Request_Game_Channel_List(unsigned char autoping, bool wait)
{
	if (g_ChanListQueue.length()) {
		bool result = Wait_Channel_List();
		if (!result) return(result);
	}
	for (int i = 0; i < 18; ++i) ResetEvent(g_WaitEventHandles[i]);
	if (!Request_Channel_List(0x21, autoping)) {
		SetEvent(g_WaitEventHandles[EV_ABORT]);
		return(false);
	}
	if (wait) return(Wait_Channel_List());
	return(true);
}
#endif


#if 0
inline bool Is_Tournament_Session_Inline(void)
{
	return(Online_Session_State == 2 || Online_Session_State == 1);
}
#endif


#if 0
bool WOL_Wait_Callback(void)
{
	Title_Screen_Restore(false);

	static int prev_exinfo = 0;
	static int prev_ping = 0;

	MSG msg;
	int counter = 0;
	MessageLoopCount++;
	while (Peek_Windows_Message(&msg, NULL, 0, 0, 1)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		counter++;
		if (counter == 100) break;
	}


	if (Is_Tournament_Session_Inline() && g_MatchTimeout != -1 && g_MatchState != 1 && g_MatchState != 0 && g_MatchState != 11 && (unsigned)(time(NULL) - g_MatchStartTime) > (unsigned)g_MatchTimeout) {
		if (g_MatchState >= 2 && g_MatchState <= 5) {
			Set_Match_State(0, -1, 2);
		} else {
			Lobby_Chat_Printf(0x20BFFF, StringTable::LoadString("WOL:MatchTimeout", NULL, "D:\\Ra2\\wonline.cpp", 2155));
			Set_Match_State(2, 30, 0);
		}
		SetEvent(g_WaitEventHandles[EV_ERROR]);
	}

	WS_Top_Window_ID();

	if (g_WDTTeamLookupQueue.length() > 200) {
		g_WDTTeamLookupQueue.clear();
	}

	if (g_LastWDTUserTeamRequestTime < (unsigned)time(0) - 2 && g_WDTTeamLookupQueue.length() > 65 ||
		g_LastWDTUserTeamRequestTime < (unsigned)time(0) - 3 && g_WDTTeamLookupQueue.length() > 10 ||
		g_LastWDTUserTeamRequestTime < (unsigned)time(0) - 15 && g_WDTTeamLookupQueue.length() > 0) {

		User * list = NULL;
		User * last_user = NULL;
		for (int i = 0; i < 10; i++) {
			if (g_WDTTeamLookupQueue.length() > 0) {
				User * new_user = new User;
				memset(new_user, 0, sizeof(User));
				Wstring name;
				g_WDTTeamLookupQueue.removeTail(name);
				char * rawName = name.get();
				if (strlen(rawName) > 9) {
					rawName[9] = '\0';
				}
				strcpy((char *)new_user->name, rawName);
				if (last_user == NULL) {
					list = new_user;
				} else {
					last_user->next = new_user;
				}
				last_user = new_user;
			} else {
				break;
			}
		}

		if (g_pChat != NULL) {
			g_pChat->RequestUserTeam(list);
		}

		User * d = list;
		while (d != NULL) {
			User * next = d->next;
			delete d;
			d = next;
		}

		g_LastWDTUserTeamRequestTime = time(0);
	}

	if (g_UserLocaleLookupQueue.length() > 200) {
		g_UserLocaleLookupQueue.clear();
	}

	if (g_LastChannelLocaleRequestTime < (unsigned)time(0) - 2 && g_UserLocaleLookupQueue.length() > 65 ||
		g_LastChannelLocaleRequestTime < (unsigned)time(0) - 3 && g_UserLocaleLookupQueue.length() > 10 ||
		g_LastChannelLocaleRequestTime < (unsigned)time(0) - 15 && g_UserLocaleLookupQueue.length() > 0) {

		User * list = NULL;
		User * last_user = NULL;
		for (int i = 0; i < 10; i++) {
			if (g_UserLocaleLookupQueue.length() > 0) {
				User * new_user = new User;
				memset(new_user, 0, sizeof(User));
				Wstring name;
				g_UserLocaleLookupQueue.removeTail(name);
				char * rawName = name.get();
				if (strlen(rawName) > 9) {
					rawName[9] = '\0';
				}
				strcpy((char *)new_user->name, rawName);
				if (last_user == NULL) {
					list = new_user;
				} else {
					last_user->next = new_user;
				}
				last_user = new_user;
			} else {
				break;
			}
		}

		if (g_pChat != NULL) {
			g_pChat->RequestUserLocale(list);
		}

		User * d = list;
		while (d != NULL) {
			User * next = d->next;
			delete d;
			d = next;
		}

		g_LastChannelLocaleRequestTime = time(0);
	}

	if (Session.Type == GAME_INTERNET && Online_Session_State == 2 && time(0) > g_NextWDTStatePollTime) {
		g_NextWDTStatePollTime = time(0) + 30;
		Get_WDT_State_Silent();
	}

	if ((g_pChat) && (g_SuspendChatPump == 0)) g_pChat->PumpMessages();
	if (g_pNetUtil) g_pNetUtil->PumpMessages();
	if (g_pDownload) g_pDownload->PumpMessages();

	Call_Back();

	if ((g_CurrentChannel.name[0]) && (g_CurrentChannel.type == 0x21) && (g_IsChannelCreator)) Send_Lobby_Options(false, false);

	if ((g_pChat != NULL) && (g_CurrentChannel.type != 0) && (g_IsChannelCreator)) {
		if ((time(NULL) - prev_exinfo) > 10) {
			char exinfo_out[80];
			memset(exinfo_out, 0, 80);
			Encode_Channel_ExInfo(exinfo_out);
			exinfo_out[40] = 0;
			g_pChat->SetChannelExInfo(exinfo_out);
			prev_exinfo = time(NULL);
		}
	}

	if (WolOptionsLocked != 2) {
		if ((g_pChat != NULL) && (g_CurrentChannel.type != 0) && ((time(NULL) - prev_ping) > 10)) {
			User user;
			Wstring nick = ((Wstring const &)g_NickName).toLower();
			int handle;
			int i;
			in_addr addr;

			if (!Is_Tournament_Session_Inline()) {
				for (i = 0; i < g_UserList.length(); i++) {
					g_UserList.get(user, i);
					Wstring const user_name((char *)user.name);
					if (nick == user_name.toLower()) continue;
					addr.s_addr = user.ipaddr;
					if (user.ipaddr != 0) g_pNetUtil->RequestPing(inet_ntoa(addr), 1000, &handle);
				}
			}
			prev_ping = time(NULL);
		}
	}

	if (WolOptionsLocked == 2) {
		if (g_StartGameReceived && !g_StartGameRunning) {
			Start_Game_Now();
		}
	}

	Handle_Preview_Download();
	return(false);
}
#endif


#if 0
void Reset_WOL_Globals(void)
{
	g_PlayerProfile = PlayerProfile::Load_Default();

	g_UserChannels.clear();
	g_GameChannelList.clear();
	g_UserList.clear();

	g_WDTTeamLookupQueue.clear();
	g_WDTTeams.clear();

	g_UserLocaleLookupQueue.clear();
	g_UserLocales.clear();

	g_NextWDTStatePollTime = time(0) + 86400;
	g_LastChannelLocaleRequestTime = 0;
	g_LastWDTUserTeamRequestTime = 0;

	g_BattleClansLadderDict.clear();
	g_WDTLadderDict.clear();
	g_BuddyChannels.clear();
	g_BuddyList.clear();
	g_BuddyRequests.clear();
	g_BuddyStatus.clear();

	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
		int sku = 0x2300;
		g_InstalledLadderSKU = sku;
		g_SelectedLadderSKU = sku;
		g_ActiveLadder = &g_WDTLadderDict;
	} else {
		int sku = g_GameSKU;
		g_InstalledLadderSKU = sku;
		g_SelectedLadderSKU = sku;
		g_ActiveLadder = &g_GeneralLadderDict;
	}

	memset(&g_CurrentChannel, 0, sizeof(g_CurrentChannel));
	memset(&g_PendingChannel, 0, sizeof(g_PendingChannel));
	memset(&g_GameServer, 0, sizeof(g_GameServer));
	memset(&g_WDTServer, 0, sizeof(g_WDTServer));

	g_LastErrorMessage[0] = 0;

	g_MatchResult = 0;
	g_MatchState = 1;
	g_MatchTimeout = -1;
	g_MatchStartTime = time(0);

	g_CurrentServerIndex = 0;
	g_TargetServerIndex = 0;

	g_IsChannelCreator = 0;
	g_ActiveWaitFlags = 0;

	g_GameStartRequested = false;

	g_IgnoreNetStatus = false;
	g_ShowMOTD = true;
	g_Motd.clear();

	memset(&g_WaitEventHandles, 0, sizeof(g_WaitEventHandles));

	g_PlayingNetGame = false;
	g_ConnectionLost = false;

	g_ServerType = -1;

	g_SuspendChatPump = 0;

	g_LastChatListTime = 0xFFFFFFFF;
	g_LastGameListTime = 0xFFFFFFFF;

	g_Lobbies[0] = StringTable::LoadString("TXT_LOB_1", NULL, "D:\\Ra2\\wonline.cpp", 2485);
	g_Lobbies[1] = StringTable::LoadString("TXT_LOB_2", NULL, "D:\\Ra2\\wonline.cpp", 2486);
	g_Lobbies[2] = StringTable::LoadString("TXT_LOB_3", NULL, "D:\\Ra2\\wonline.cpp", 2487);
	g_Lobbies[3] = StringTable::LoadString("TXT_LOB_4", NULL, "D:\\Ra2\\wonline.cpp", 2488);
	g_Lobbies[4] = StringTable::LoadString("TXT_LOB_5", NULL, "D:\\Ra2\\wonline.cpp", 2489);
	g_Lobbies[5] = StringTable::LoadString("TXT_LOB_6", NULL, "D:\\Ra2\\wonline.cpp", 2490);
	g_Lobbies[6] = StringTable::LoadString("TXT_LOB_7", NULL, "D:\\Ra2\\wonline.cpp", 2491);
	g_Lobbies[7] = StringTable::LoadString("TXT_LOB_8", NULL, "D:\\Ra2\\wonline.cpp", 2492);

	memset(WDT_TERRITORY_LOBBY_INDEXES, 0xFFFFFFFF, sizeof(WDT_TERRITORY_LOBBY_INDEXES));

	g_pChat = NULL;
	g_pNetUtil = NULL;
	g_pDownload = NULL;

	g_dwChatAdvise = 0;
	g_dwNetUtilAdvise = 0;
	g_dwDownloadAdvise = 0;

	g_SquadInfoRequests = 0;
	g_RequestingOwnSquadInfo = 0;
	g_OwnSquadID = 0;
	memset(g_OwnSquadName, 0, 41);

	g_ChannelCount = 0;

	g_LadderPos = 1;

	g_JoinLobbyNow = 0;

	g_ChanListQueue.setEmpty();

	WolOptionsLocked = 0;

	g_UserAge = 0;
	g_UserConsent = 0;

	g_RuleCRC = RulesClass::Get_Rule_Unique_ID();
	RulesClass::Load_Art_INI();
	g_ArtCRC = RulesClass::Get_Art_Unique_ID();
	g_AICRC = RulesClass::Get_AI_Unique_ID();

	ManglerIndex = 0;
}
#endif


#if 0
void Refresh_Lobby_List(void)
{
	HWND dialog = WS_Find_Dialog(0x113);
	if (!dialog) return;
	HWND list = GetDlgItem(dialog, 0x53D);
	if (!list) return;

	Wstring selected;
	int top_row = SendMessageA(list, LB_GETTOPINDEX, 0, 0);
	if (top_row != -1) Get_Window_Item_Text(list, top_row, selected);
	SendMessageA(list, OD_DISABLEPAINT, 0, 1);
	Clear_Listbox(list);

	int count = g_UserChannels.length();
	for (int i = 0; i < count; ++i) {
		Channel channel;
		g_UserChannels.get(channel, i);
		if (strncmp((char *)channel.name, "Lob_33_", strlen("Lob_33_")) != 0) continue;
		int number = atol((char *)channel.name + strlen("Lob_33_"));
		bool current = strcmp((char *)channel.name, (char *)g_CurrentChannel.name) == 0;
		wchar_t title[80];
		int page = number / 8;
		int name_index = number % 8;
		if (page == 0) {
			swprintf(title, L"%s  (%d)", (wchar_t const *)g_Lobbies[name_index], channel.currentUsers);
		} else {
			swprintf(title, L"%s_%d  (%d)", (wchar_t const *)g_Lobbies[name_index], page + 1, channel.currentUsers);
		}
		int rows = SendMessageA(list, LB_GETCOUNT, 0, 0);
		int at = 0;
		Channel other;
		for (; at < rows; ++at) {
			int index = SendMessageA(list, LB_GETITEMDATA, at, 0);
			g_UserChannels.get(other, index);
			if (atol((char *)other.name + strlen("Lob_33_")) > number) break;
		}
		int row = SendMessageA(list, OD_INSERTSTRING, at, (LPARAM)channel.name);
		SendMessageA(list, LB_SETITEMDATA, row, i);
		OwnerDraw::CellData cell;
		cell.type = OwnerDraw::CellData::TEXT;
		cell.string.set((unsigned short *)title);
		cell.hint.set((unsigned short *)L"");
		SendMessageA(list, OD_SETCELL, MAKEWPARAM(g_Col_ChanName, row), (LPARAM)&cell);
		if (current) SendMessageA(list, LB_SETCURSEL, row, 0);
	}

	int restored = SendMessageA(list, OD_FINDSTRING, 0, (LPARAM)selected.text());
	if (restored != -1) SendMessageA(list, LB_SETTOPINDEX, restored, 0);
	SendMessageA(list, OD_DISABLEPAINT, 0, 0);
	InvalidateRect(list, NULL, false);
}
#endif


WideWstring Get_Locale_Name(int locale)
{
	Wstring label = "WOL:Locale" + Wstring_FromInt(locale, 2);
	WideWstring text(StringTable::LoadString(label.get(), NULL, "D:\\Ra2\\wonline.cpp", 2644));
	if (locale < 37) {
		return(text);
	} else {
		return(text);
	}
}

#if 0
WideWstring User_Tooltip(User const & user)
{
	Wstring name((char *)user.name);
	WideWstring text = Wstring_ToWide(name) + L" : ";
	name.toLower();
	Ladder ladder;
	memset(&ladder, 0, sizeof(ladder));
	if (g_ActiveLadder && g_ActiveLadder->getValue(name, ladder)) {
		text += WideWstring(StringTable::LoadString("TXT_RANK", NULL, "D:\\Ra2\\wonline.cpp", 2672)) + WideWstring(L" ") + WideWstring_FromInt(ladder.rung);
	} else {
		text += StringTable::LoadString("TXT_UNRANKED", NULL, "D:\\Ra2\\wonline.cpp", 2674);
	}
	Wstring squad((char *)user.squadname);
	if (squad.length() == 0) squad = (char *)user.squadabbrev;
	if (squad.length() > 0) {
		text += L": " + Wstring_ToWide(squad);
	}
	if (g_UserLocales.contains(name)) {
		int * locale = 0;
		if (g_pChat && g_UserLocales.getPointer(name, &locale) && locale && *locale) {
			text += L" : " + Get_Locale_Name(*locale);
		}
	}
	return(text);
}
#endif


WideWstring Get_Mission_Description(Wstring const & filename)
{
	if (stricmp(filename.text(), "RandMap.Sed") == 0) {
		return(WideWstring(StringTable::LoadString("TXT_RANDOM_MAP_DESCRIPTION", NULL, "D:\\Ra2\\wonline.cpp", 2705)));
	}
	for (int index = 0; index < Session.Scenarios.Count(); index++) {
		MultiMission * mission = Session.Scenarios[index];
		if (stricmp(filename.text(), mission->Get_Filename()) == 0) {
			return(WideWstring(mission->Description()));
		}
	}
	return(WideWstring(L""));
}

#if 0
WideWstring Game_List_Tooltip(HWND list, int row)
{
	if (row == -1) {
		return(WideWstring(StringTable::LoadString("STT:LobbyListGames", NULL, "D:\\Ra2\\wonline.cpp", 2722)));
	}
	Channel channel;
	memset(&channel, 0, sizeof(channel));
	int index = SendMessageA(list, LB_GETITEMDATA, row, 0);
	if (index >= 0 && index < g_GameChannelList.length()) {
		g_GameChannelList.get(channel, index);
		Wstring host((char *)channel.name);
		host.truncate('\'');
		User user;
		memset(&user, 0, sizeof(user));
		strncpy((char *)user.name, host.get(), 10);
		WideWstring tooltip = User_Tooltip(user);
		tooltip += WideWstring(L", ");
		wchar_t ping_text[64];
		if (channel.latency != -1) {
			swprintf(ping_text, StringTable::LoadString("MSG:PingInfo", NULL, "D:\\Ra2\\wonline.cpp", 2756), channel.latency);
			tooltip += ping_text;
		} else {
			tooltip += StringTable::LoadString("TXT_UNKNOWN_PING", NULL, "D:\\Ra2\\wonline.cpp", 2759);
		}
		char options[132];
		strncpy(options, (char *)channel.exInfo, 128);
		int build = 0;
		int hash = 0;
		int ai_count = 0;
		int observer = 0;
		int not_full = 0;
		Wstring scenario;
		Parse_Channel_ExInfo(options, &build, &hash, &ai_count, &observer, &not_full, &scenario);
		if (scenario.length() > 0) {
			WideWstring description = Get_Mission_Description(scenario);
			if (description.length() > 0) {
				tooltip += L", ";
				tooltip += description;
			}
		}
		return(tooltip);
	}
	return(WideWstring(StringTable::LoadString("STT:LobbyListGames", NULL, "D:\\Ra2\\wonline.cpp", 2729)));
}
#endif


#if 0
WideWstring User_List_Tooltip(HWND list, int row)
{
	if (row == -1) {
		return(WideWstring(StringTable::LoadString("STT:LobbyListUsers", NULL, "D:\\Ra2\\wonline.cpp", 2785)));
	}
	int index = SendMessageA(list, LB_GETITEMDATA, row, 0);
	if (index >= 0 && index < g_UserList.length()) {
		User user;
		memset(&user, 0, sizeof(user));
		g_UserList.get(user, index);
		return(User_Tooltip(user));
	}
	return(WideWstring(StringTable::LoadString("STT:LobbyListUsers", NULL, "D:\\Ra2\\wonline.cpp", 2790)));
}
#endif


#if 0
void Refresh_Game_List(void)
{
	HWND dialog = WS_Find_Dialog(0x113);
	if (!dialog) return;
	HWND list = GetDlgItem(dialog, 0x6EE);
	if (!list) return;

	Wstring selected;
	int selected_row = SendMessageA(list, LB_GETCURSEL, 0, 0);
	if (selected_row != -1) Get_Window_Item_Text(list, selected_row, selected);
	Wstring top;
	int top_row = SendMessageA(list, LB_GETTOPINDEX, 0, 0);
	if (top_row != -1) Get_Window_Item_Text(list, top_row, top);
	SendMessageA(list, OD_DISABLEPAINT, 0, 1);
	g_GameListRefreshing = true;
	SendMessageA(list, LB_RESETCONTENT, 0, 0);

	int count = g_GameChannelList.length();
	for (int i = 0; i < count; ++i) {
		Channel channel;
		memset(&channel, 0, sizeof(channel));
		g_GameChannelList.get(channel, i);
		if (Is_Channel_Flag_11(&channel) || Is_Channel_Flag_12(&channel)) continue;
		char options[132];
		strncpy(options, (char *)channel.exInfo, 128);
		int build = 0;
		int hash = 0;
		int ai_count = 0;
		int observer = 0;
		int not_full = 0;
		Wstring scenario;
		Parse_Channel_ExInfo(options, &build, &hash, &ai_count, &observer, &not_full, &scenario);
		int participants = channel.currentUsers - observer + ai_count;
		int capacity = channel.maxUsers - ((observer || not_full) ? 1 : 0);
		wchar_t title[80];
		swprintf(title, L"%hs %d/%d", channel.name, participants, capacity);
		int row = SendMessageA(list, OD_INSERTSTRING, -1, (LPARAM)channel.name);
		SendMessageA(list, LB_SETITEMDATA, row, i);

		OwnerDraw::CellData cell;
		cell.type = OwnerDraw::CellData::TEXT;
		cell.string.set((unsigned short *)title);
		cell.hint.set((unsigned short *)L"");
		cell.color = ColorSystem;
		if (ObserverMode) {
			if (!not_full || observer) cell.color = ColorNoJoin;
		} else if (participants == capacity) {
			cell.color = ColorNoJoin;
		}
		SendMessageA(list, OD_SETCELL, MAKEWPARAM(g_Col_GameName, row), (LPARAM)&cell);

		cell.type = OwnerDraw::CellData::INVALID;
		cell.hint.set((unsigned short *)L"");
		cell.type = OwnerDraw::CellData::SURFACE;
		cell.hint.set((unsigned short *)L"");
		if (Is_Channel_Flag_8(&channel)) {
			cell.surf = SurfaceCache.GetSurface("wolclan.pcx", false);
			cell.hint.set(StringTable::LoadString("TXT_BATTLECLAN_GAME", NULL, "D:\\Ra2\\wonline.cpp", 2903));
		} else if (Is_Channel_Tournament(&channel)) {
			cell.surf = SurfaceCache.GetSurface("woltrny.pcx", false);
			cell.hint.set(StringTable::LoadString("TXT_TOURNAMENT_GAME", NULL, "D:\\Ra2\\wonline.cpp", 2908));
		} else {
			cell.surf = SurfaceCache.GetSurface("gt18.pcx", false);
			cell.hint.set((unsigned short *)L"");
		}
		SendMessageA(list, OD_SETCELL, MAKEWPARAM(g_Col_GameIcon, row), (LPARAM)&cell);

		cell.type = OwnerDraw::CellData::SURFACE;
		cell.hint.set((unsigned short *)L"");
		if (channel.flags & 0x100) {
			cell.surf = SurfaceCache.GetSurface("wolpriv.pcx", false);
			cell.hint.set(StringTable::LoadString("TXT_GAME_PASSWORD", NULL, "D:\\Ra2\\wonline.cpp", 2922));
		} else {
			cell.type = OwnerDraw::CellData::INVALID;
			cell.hint.set((unsigned short *)L"");
		}
		SendMessageA(list, OD_SETCELL, MAKEWPARAM(g_Col_GamePrivate, row), (LPARAM)&cell);

		cell.type = OwnerDraw::CellData::INVALID;
		cell.string.set((unsigned short *)L"");
		cell.hint.set((unsigned short *)L"");
		if (channel.reserved & 0x600) {
			if (!observer && not_full) {
				cell.type = OwnerDraw::CellData::SURFACE;
				cell.surf = SurfaceCache.GetSurface("wolrslkob.pcx", false);
			} else {
				cell.type = OwnerDraw::CellData::SURFACE;
				cell.surf = SurfaceCache.GetSurface("wolreslk.pcx", false);
			}
		} else if (!observer && not_full) {
			cell.type = OwnerDraw::CellData::SURFACE;
			cell.surf = SurfaceCache.GetSurface("wolob.pcx", false);
		}
		SendMessageA(list, OD_SETCELL, MAKEWPARAM(g_Col_GameObserver, row), (LPARAM)&cell);

		if (channel.latency == -1) {
			cell.type = OwnerDraw::CellData::TEXT;
			cell.string.set((unsigned short *)L"???");
		} else {
			cell.pingtime = channel.latency;
			cell.type = OwnerDraw::CellData::PING;
		}
		swprintf(title, StringTable::LoadString("WOL:GamePing", NULL, "D:\\Ra2\\wonline.cpp", 2962), channel.latency);
		cell.hint.set((unsigned short *)title);
		SendMessageA(list, OD_SETCELL, MAKEWPARAM(g_Col_GamePingTime, row), (LPARAM)&cell);
	}

	int restored = SendMessageA(list, OD_FINDSTRING, 0, (LPARAM)selected.text());
	SendMessageA(list, LB_SETCURSEL, restored, 0);
	int restored_top = SendMessageA(list, OD_FINDSTRING, 0, (LPARAM)top.text());
	if (restored_top != -1) SendMessageA(list, LB_SETTOPINDEX, restored_top, 0);
	SendMessageA(list, OD_DISABLEPAINT, 0, 0);
	InvalidateRect(list, NULL, false);
	if (dialog == WS_Top_Window()) SetFocus(GetDlgItem(dialog, 0x540));
}
#endif


#if 0
void Draw_Player_List(int keep_selection, bool force)
{
	HWND win = GameoptWindow();
	HWND userwin;
	int i;
	if (win) {
		Refresh_Slot_Settings(win, force);
		return;
	}
	if (WS_Find_Dialog(0x6B)) return;
	win = WS_Find_Dialog(0x116);
	if (win) {
		Fill_Buddy_List(win);
	} else {
		win = WS_Find_Dialog(0x113);
	}
	userwin = GetDlgItem(win, 0x53E);
	if (!win || !userwin) return;

	Dictionary<WideWstring, bool> lbdict(Wstring_Hash);
	LBSaveSelections(userwin, lbdict);
	int topindex = SendDlgItemMessageA(win, 0x53E, LB_GETTOPINDEX, 0, 0);
	SendDlgItemMessageA(win, 0x53E, OD_DISABLEPAINT, 0, 1);
	SendDlgItemMessageA(win, 0x53E, LB_RESETCONTENT, 0, 0);

	Wstring ladder_name;
	Ladder ladder;
	memset(&ladder, 0, sizeof(ladder));
	OwnerDraw::CellData thecell;
	Wstring const & nick = g_NickName;
	Wstring local_name = nick.toLower();
	wchar_t info[80];
	wchar_t rank_string[80];
	char count_text[20];
	User user;
	static wchar_t localestring[64];

	for (i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(user, i);
		Wstring const user_name((char *)user.name);
		if (local_name == user_name.toLower()) {
			HWND hwnd = GetDlgItem(win, 0x555);
			if (hwnd) {
				LONG style = GetWindowLongA(hwnd, GWL_STYLE);
				if (user.flags & 1) {
					style &= ~WS_DISABLED;
				} else {
					style |= WS_DISABLED;
				}
				SetWindowLongA(hwnd, GWL_STYLE, style);
				InvalidateRect(hwnd, NULL, 0);
			}
			hwnd = GetDlgItem(win, 0x587);
			if (hwnd) {
				LONG style = GetWindowLongA(hwnd, GWL_STYLE);
				if (user.flags & 1) {
					style &= ~WS_DISABLED;
				} else {
					style |= WS_DISABLED;
				}
				SetWindowLongA(hwnd, GWL_STYLE, style);
				InvalidateRect(hwnd, NULL, 0);
			}
		}

		if (g_CurrentChannel.type == 0) {
			swprintf(info, L"%hs", user.name);
			int row = SendDlgItemMessageA(win, 0x53E, OD_INSERTSTRINGW, (WPARAM)-1, (LPARAM)info);
			thecell.type = OwnerDraw::CellData::TEXT;
			thecell.hint.set(StringTable::LoadString("TXT_UNRANKED", NULL, "D:\\Ra2\\wonline.cpp", 3096));
			Char_To_Wide(info, (char *)user.name, -1);
			thecell.string.set(info);
			if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
				if (!(user.flags & 1)) {
					Wstring team_name = Wstring_Narrow(thecell.string.get());
					if (g_WDTTeams.contains(team_name)) {
						int color;
						int * value = 0;
						if (g_WDTTeams.getPointer(team_name, &value) && value) color = *value;
						WideWstring converted_team(Narrow_To_Wide(team_name.get()));
						thecell.string = converted_team;
						thecell.color = PlayerColorTable[color];
					} else {
						thecell.color = ColorSystem;
					}
				} else {
					thecell.color = ColorSystem;
				}
			} else {
				thecell.color = ColorSystem;
			}

			ladder_name.set((char *)user.name);
			ladder_name.toLower();
			memset(localestring, 0, sizeof(localestring));
			int locale = 0;
			if (g_UserLocales.contains(ladder_name) && g_pChat) {
				int * value = 0;
				if (g_UserLocales.getPointer(ladder_name, &value) && value) locale = *value;
				if (locale) {
					WideWstring locale_name = Get_Locale_Name(locale);
					swprintf(localestring, L"(%hs)", locale_name.get());
				}
			}
			if (g_ActiveLadder->getValue(ladder_name, ladder)) {
				swprintf(rank_string, L"%s %d", StringTable::LoadString("TXT_RANK", NULL, "D:\\Ra2\\wonline.cpp", 3162), ladder.rung);
				thecell.hint.set(rank_string);
			}
			if (g_UserLocales.contains(ladder_name)) {
				if (locale) thecell.hint.insert(L" ", 0);
				thecell.hint.insert(localestring, 0);
			}
			SendMessageA(GetDlgItem(win, 0x53E), LB_SETITEMDATA, row, i);
			SendDlgItemMessageA(win, 0x53E, OD_SETCELL, MAKEWPARAM(g_Col_Username, i), (LPARAM)&thecell);

			thecell.type = OwnerDraw::CellData::TEXT;
			ladder_name.set((char *)user.name);
			ladder_name.toLower();
			rank_string[0] = 0;
			if (strlen((char *)user.squadabbrev)) {
				swprintf(rank_string, L"[%hs]", user.squadabbrev);
				wchar_t squad_name[200];
				Char_To_Wide(squad_name, (char *)user.squadname, -1);
				thecell.hint.set(squad_name);
			} else if (g_ActiveLadder->getValue(ladder_name, ladder)) {
				swprintf(rank_string, L"(%s %d)", StringTable::LoadString("TXT_RANK", NULL, "D:\\Ra2\\wonline.cpp", 3204), ladder.rung);
				thecell.hint.set(L"");
			}
			thecell.string.set(rank_string);
			SendDlgItemMessageA(win, 0x53E, OD_SETCELL, MAKEWPARAM(g_Col_Rank, i), (LPARAM)&thecell);

			thecell.type = OwnerDraw::CellData::TEXT;
			if (user.flags & 1) {
				thecell.type = OwnerDraw::CellData::SURFACE;
				thecell.surf = SurfaceCache.GetSurface("woloper.pcx", false);
				thecell.hint.set(StringTable::LoadString("TXT_OPER", NULL, "D:\\Ra2\\wonline.cpp", 3216));
			} else if (user.flags & 2) {
				thecell.type = OwnerDraw::CellData::SURFACE;
				thecell.surf = SurfaceCache.GetSurface("wolvoice.pcx", false);
				thecell.hint.set(StringTable::LoadString("TXT_VOICE", NULL, "D:\\Ra2\\wonline.cpp", 3222));
			} else {
				thecell.string.set(L"");
				thecell.hint.set(L"");
			}
			SendDlgItemMessageA(win, 0x53E, OD_SETCELL, MAKEWPARAM(g_Col_Chanop, i), (LPARAM)&thecell);
		} else {
			if (HouseTypes.Count() <= 1) return;
			int slot_index;
			{
				Wstring lookup_name((char *)user.name);
				slot_index = Player_Name_To_Index(lookup_name);
			}
			int house = 0;
			int star = 0;
			if (slot_index != -1) {
				house = g_UserInfo[slot_index].house;
				if (g_UserInfo[slot_index].accepted) star = 1;
				if (slot_index == 0) {
					g_UserInfo[slot_index].accepted = 1;
					star = 2;
				}
			}
			house = house > 0 ? house : 0;

			ladder_name.set((char *)user.name);
			ladder_name.toLower();
			swprintf(info, L"%hs", user.name);
			for (int j = 0; j < (int)wcslen(info); j++) {
				info[j] = (wchar_t)tolower(info[j]);
			}
			SendDlgItemMessageA(win, 0x53E, OD_INSERTSTRINGW, (WPARAM)-1, (LPARAM)info);

			thecell.type = OwnerDraw::CellData::PRIMARY;
			thecell.color = PlayerColorTable[g_UserInfo[slot_index].color];
			thecell.hint.set(StringTable::LoadString("TXT_UNRANKED", NULL, "D:\\Ra2\\wonline.cpp", 3297));
			if (g_ActiveLadder->getValue(ladder_name, ladder)) {
				swprintf(rank_string, L"%s %d", StringTable::LoadString("TXT_RANK", NULL, "D:\\Ra2\\wonline.cpp", 3300), ladder.rung);
				thecell.hint.set(rank_string);
			}
			SendDlgItemMessageA(win, 0x53E, OD_SETCELL, MAKEWPARAM(g_Col_Username, i), (LPARAM)&thecell);

			thecell.type = OwnerDraw::CellData::TEXT;
			if (strlen((char *)user.squadabbrev)) {
				Char_To_Wide(info, (char *)user.squadname, -1);
				thecell.hint.set(info);
				Char_To_Wide(info, (char *)user.squadabbrev, -1);
			} else {
				if (g_ActiveLadder->getValue(ladder_name, ladder)) {
					swprintf(info, L"(%s %d)", StringTable::LoadString("TXT_RANK", NULL, "D:\\Ra2\\wonline.cpp", 3319), ladder.rung);
					thecell.hint.set(L"");
				} else {
					swprintf(info, L"");
					thecell.hint.set(L"");
				}
			}
			thecell.string.set(info);
			SendDlgItemMessageA(win, 0x53E, OD_SETCELL, MAKEWPARAM(g_Col_Gameinfo, i), (LPARAM)&thecell);

			thecell.type = OwnerDraw::CellData::SURFACE;
			switch (house) {
			case 0:
				thecell.surf = SurfaceCache.GetSurface("usai.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\wonline.cpp", 3355));
				break;
			case 1:
				thecell.surf = SurfaceCache.GetSurface("japi.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\wonline.cpp", 3359));
				break;
			case 2:
				thecell.surf = SurfaceCache.GetSurface("frai.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\wonline.cpp", 3363));
				break;
			case 3:
				thecell.surf = SurfaceCache.GetSurface("geri.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\wonline.cpp", 3367));
				break;
			case 4:
				thecell.surf = SurfaceCache.GetSurface("gbri.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_GDI", NULL, "D:\\Ra2\\wonline.cpp", 3371));
				break;
			case 5:
				thecell.surf = SurfaceCache.GetSurface("djbi.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\wonline.cpp", 3375));
				break;
			case 6:
				thecell.surf = SurfaceCache.GetSurface("arbi.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\wonline.cpp", 3379));
				break;
			case 7:
				thecell.surf = SurfaceCache.GetSurface("lati.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\wonline.cpp", 3383));
				break;
			case 8:
				thecell.surf = SurfaceCache.GetSurface("rusi.pcx", false);
				swprintf(info, L"%s", StringTable::LoadString("TXT_NOD", NULL, "D:\\Ra2\\wonline.cpp", 3387));
				break;
			}
			thecell.hint.set(info);
			SendDlgItemMessageA(win, 0x53E, OD_SETCELL, MAKEWPARAM(g_Col_House, i), (LPARAM)&thecell);

			thecell.type = OwnerDraw::CellData::SURFACE;
			if (star) {
				if (star == 2) {
					thecell.surf = SurfaceCache.GetSurface("wolhost.pcx", false);
				} else {
					thecell.surf = SurfaceCache.GetSurface("wolacpt.pcx", false);
				}
				thecell.hint.set(StringTable::LoadString("TXT_ACCEPTED", NULL, "D:\\Ra2\\wonline.cpp", 3401));
			} else {
				thecell.type = OwnerDraw::CellData::INVALID;
				thecell.hint.set(L"");
			}
			SendDlgItemMessageA(win, 0x53E, OD_SETCELL, MAKEWPARAM(g_Col_Accept, i), (LPARAM)&thecell);

			int average_ping = 0;
			g_pNetUtil->GetAvgPing(user.ipaddr, &average_ping);
			if (average_ping < 0) average_ping = 0;
			thecell.type = OwnerDraw::CellData::PING;
			thecell.pingtime = average_ping;
			wchar_t ping_text[200];
			swprintf(ping_text, StringTable::LoadString("MSG:PingInfo", NULL, "D:\\Ra2\\wonline.cpp", 3417), average_ping);
			if (average_ping == 0) {
				swprintf(ping_text, StringTable::LoadString("TXT_UNKNOWN_PING", NULL, "D:\\Ra2\\wonline.cpp", 3419));
			}
			thecell.hint.set(ping_text);
			Wstring const ping_name((char *)user.name);
			if (local_name == ping_name.toLower()) {
				thecell.type = OwnerDraw::CellData::INVALID;
			}
			SendDlgItemMessageA(win, 0x53E, OD_SETCELL, MAKEWPARAM(g_Col_GamePing, i), (LPARAM)&thecell);
		}
	}

	if (keep_selection == 0) LBRestoreSelections(userwin, lbdict);
	int sel_count = SendDlgItemMessageA(win, 0x53E, LB_GETSELCOUNT, 0, 0);
	int user_count = g_UserList.length();
	if (sel_count > 0) {
		sprintf(count_text, "%d  (%d)", user_count, sel_count);
	} else {
		sprintf(count_text, "%d", user_count);
	}
	SendDlgItemMessageA(win, 0x60B, OD_SETTEXT, 0, (LPARAM)count_text);
	SendDlgItemMessageA(win, 0x53E, LB_SETTOPINDEX, (WPARAM)topindex, 0);
	SendDlgItemMessageA(win, 0x53E, OD_DISABLEPAINT, 0, 0);
	InvalidateRect(GetDlgItem(win, 0x53E), NULL, 0);
}
#endif











#if 0
void Redraw_Player_List(void)
{
	Draw_Player_List(0, false);
}
#endif



#if 0
int Startup_Chat(HINSTANCE hInstance)
{
	HRESULT hresult = NULL;
	_Module.Init(NULL, hInstance);

	g_pChat = NULL;
	g_pNetUtil = NULL;
	g_pDownload = NULL;
	g_dwChatAdvise = 0;
	g_dwNetUtilAdvise = 0;
	g_dwDownloadAdvise = 0;

	CoCreateInstance(CLSID_Chat, NULL, CLSCTX_INPROC_SERVER, IID_IChat, (void **)&g_pChat);
	if (g_pChat) {
		CComObject<CChatEventSink>::CreateInstance(&g_pChatSink);
		hresult = AtlAdvise(g_pChat, g_pChatSink->GetUnknown(), IID_IChatEvent, &g_dwChatAdvise);
	}
	if ((g_pChat == NULL) || (hresult != S_OK)) {
		ODMessageBox(StringTable::LoadString("TXT_APIMISSING", NULL, "D:\\Ra2\\wonline.cpp", 7881), 0, NULL, false);
		return(-2);
	}

	CoCreateInstance(CLSID_NetUtil, NULL, CLSCTX_INPROC_SERVER, IID_INetUtil, (void **)&g_pNetUtil);
	if (g_pNetUtil) {
		CComObject<CNetUtilEventSink>::CreateInstance(&g_pNetUtilSink);
		hresult = AtlAdvise(g_pNetUtil, g_pNetUtilSink->GetUnknown(), IID_INetUtilEvent, &g_dwNetUtilAdvise);
	}
	if ((g_pNetUtil == NULL) || (hresult != S_OK)) {
		ODMessageBox(StringTable::LoadString("TXT_APIMISSING", NULL, "D:\\Ra2\\wonline.cpp", 7897), 0, NULL, false);
		return(-2);
	}

	CoCreateInstance(CLSID_Download, NULL, CLSCTX_INPROC_SERVER, IID_IDownload, (void **)&g_pDownload);
	if (g_pDownload) {
		CComObject<CDownloadEventSink>::CreateInstance(&g_pDownloadSink);
		hresult = AtlAdvise(g_pDownload, g_pDownloadSink->GetUnknown(), IID_IDownloadEvent, &g_dwDownloadAdvise);
	}
	if ((g_pDownload == NULL) || (hresult != S_OK)) {
		ODMessageBox(StringTable::LoadString("TXT_APIMISSING", NULL, "D:\\Ra2\\wonline.cpp", 7913), 0, NULL, false);
		return(-2);
	}

	unsigned long version;
	g_pChat->GetVersion(&version);
	if (version < 0x1000E) {
		ODMessageBox(StringTable::LoadString("TXT_APIWRONGVERSION", NULL, "D:\\Ra2\\wonline.cpp", 7925), 0, NULL, false);
		return(-2);
	}

	g_pChat->SetAttributeValue("RegPath", APP_REG_KEY);

	HKEY rKey;
	char buf[256];
	if (APP_REG_KEY != NULL && strlen(APP_REG_KEY)) {
		strcpy(buf, APP_REG_KEY);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, buf, 0, KEY_READ, &rKey) == ERROR_SUCCESS) {
			DWORD type;
			DWORD value;
			DWORD valuesize = sizeof(value);
			if (RegQueryValueEx(rKey, "SKU", NULL, &type, (LPBYTE)&value, &valuesize) == ERROR_SUCCESS) {
				g_pChat->SetProductSKU(value);
			}
			RegCloseKey(rKey);
		}
	}

	return(1);
}
#endif


/// <summary>
/// Determines the sort order of two channel users.
/// Channel owners come first, then users with voice, then everyone else; equals are listed
/// alphabetically. This is the order the user list box is kept in.
/// </summary>
/// <returns>Returns with a negative value if the u1 user sorts first, a positive value if
/// the u2 one does, and zero if the two cannot be told apart.</returns>
int Compare_Users(User * u1, User * u2)
{
	User * users[2];
	int value[2];
	users[0] = u1;
	users[1] = u2;
	value[0] = 0;
	value[1] = 0;

	for (int i = 0; i < 2; i++) {
		if (users[i]->flags & 1) value[i] += 4;
		if (users[i]->flags & 2) value[i] += 2;
	}
	if (value[0] < value[1]) {
		return(1);
	} else if (value[0] > value[1]) {
		return(-1);
	}
	return(stricmp((char *)u1->name, (char *)u2->name));
}

#include "multiplayer.h"


/// <summary>
/// Handles a user leaving the current channel. If the departing user is ourselves, resets all
/// local channel/session state and returns to the lobby (closing any open guest or game-options
/// dialogs); otherwise removes the given user from our local user list and updates the
/// game-options acceptance state before refreshing the user list display.
/// </summary>
/// <param name="user">The user who left the channel.</param>
#if 0
void Handle_User_Leave(User & user)
{
	stricmp(g_NickName.get(), (char *)user.name);

	if (g_CurrentChannel.type != 0) VocClass::PlayGlobal(Rule->PlayerLeft, 1.0f, 0x2000, 0);

	if (user.flags & 0x8000) {
		DebugString("Local user leaving channel\n");
		if (WolOptionsLocked != 0) DebugString("Local user leaving channel after game start\n");
		wchar_t name[64];
		Char_To_Wide(name, (char *)g_CurrentChannel.name, -1);
		if (strncmp((char *)g_CurrentChannel.name, "Lob_33_", 7) == 0) {
			int num = atol(((char *)g_CurrentChannel.name) + strlen("Lob_33_"));
			int num2 = num / 8;
			int num1 = num % 8;
			if (num2 == 0) {
				swprintf(name, L"%s", g_Lobbies[num1]);
			} else {
				swprintf(name, L"%s %d", g_Lobbies[num1], num2 + 1);
			}
		}
		if (Online_Session_State != 2 && Online_Session_State != 1) {
			Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_YOULEFT", NULL, "D:\\Ra2\\wonline.cpp", 7993), name);
		}
		g_ChannelCount = 0;
		DebugString("Channel Leave \n");
		memset(&g_CurrentChannel, 0, sizeof(Channel));
		memset(g_UserInfo, 0, sizeof(ChannelUserInfo) * 16);
		g_UserList.clear();
		g_IsChannelCreator = 0;
		HWND wait_dlg = WS_Find_Dialog(0xC4);
		if (wait_dlg) {
			g_ActiveWaitFlags &= ~0x40;
			if (g_ActiveWaitFlags == 0) {
				WS_Destroy_Dialog(wait_dlg, 0);
			}
		}
		HWND top = WS_Top_Window();
		HWND control = GetDlgItem(top, 0x540);
		if (control) {
			SetFocus(control);
		}

		++Session.Suspended;
		Session.Options.ScenarioDescription[0] = 0;
		Session.ScenarioFileLength = 0;
		Session.ScenarioDigest[0] = 0;
		HWND mainwin = WS_Find_Dialog(0x113);
		if (mainwin && g_PlayingNetGame) {
			ShowWindow(mainwin, SW_HIDE);
			OwnerDraw::Set_Paint_Disabled(mainwin, 1);
		}
		HWND guest_dlg = WS_Find_Dialog(0xC9);
		if (guest_dlg) {
			WS_Destroy_Dialog(guest_dlg, 2);
			ShowWindow(guest_dlg, SW_HIDE);
			OwnerDraw::Set_Paint_Disabled(guest_dlg, 1);
			if (Online_Session_State == 3) {
				if (g_PlayingNetGame) {
					if (mainwin) {
						ShowWindow(mainwin, SW_HIDE);
						OwnerDraw::Set_Paint_Disabled(mainwin, 1);
					}
				} else {
					Join_Selected_Lobby(false);
					if (mainwin) PostMessageA(mainwin, 0x593, 0, 0);
				}
			} else if (Online_Session_State == 4) {
				DebugString("Setting g_JoinLobbyNow, Guest\n");
				g_JoinLobbyNow = 1;
			}
		}
		HWND gamewin = WS_Find_Dialog(0xC2);
		if (gamewin) {
			WS_Destroy_Dialog(gamewin, 2);
			ShowWindow(gamewin, SW_HIDE);
			OwnerDraw::Set_Paint_Disabled(gamewin, 1);
			if (Online_Session_State == 3) {
				if (g_PlayingNetGame) {
					if (mainwin) {
						ShowWindow(mainwin, SW_HIDE);
						OwnerDraw::Set_Paint_Disabled(mainwin, 1);
					}
				} else {
					Join_Selected_Lobby(false);
					if (mainwin) PostMessageA(mainwin, 0x593, 0, 0);
				}
			} else if (Online_Session_State == 4) {
				DebugString("Setting g_JoinLobbyNow, Host\n");
			}
		}
		if (mainwin) {
			if (!g_PlayingNetGame) {
				ShowWindow(mainwin, SW_SHOW);
				OwnerDraw::Set_Paint_Disabled(mainwin, 0);
			}
			Refresh_Game_List();
		}
		--Session.Suspended;
		return;
	}

	if (WolOptionsLocked == 2) {
		DebugString("other player leaving channel after Game Started\n");
		return;
	}

	if (&user) {
		for (int slot = 0; slot < 8; slot++) {
			if (Session.Slots[slot] && !strcmp(Wide_To_Narrow(Session.Slots[slot]->Name), (char *)user.name)) {
				HWND lobby = GameoptWindow();
				if (lobby) Set_Player_Ping(lobby, slot, -1);
				break;
			}
		}
	}

	User temp;
	for (int i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(temp, i);
		if (stricmp((char *)temp.name, (char *)user.name) == 0) {
			g_UserList.remove(i);
			if (g_CurrentChannel.type != 0) {
				memmove(g_UserInfo + i, g_UserInfo + i + 1, sizeof(ChannelUserInfo) * (15 - i));
				if (WolOptionsLocked != 2) {
					for (int * accepted = &g_UserInfo[1].accepted; accepted < &g_UserInfo[16].accepted; accepted += sizeof(ChannelUserInfo) / sizeof(int)) {
						*accepted = 0;
						EnableWindow(GetDlgItem(GameoptWindow(), 0x59F), 1);
					}
					if (!g_IsChannelCreator) {
						HWND guest = WS_Find_Dialog(0xC9);
						Enable_Player_Combos(guest, true);
					}
					Enable_Start_Button();
				}
			}
			if (i == 0 && g_CurrentChannel.type != 0 && g_IsChannelCreator == 0) {
				g_pChat->RequestChannelLeave();
			}
			break;
		}
	}

	if (g_CurrentChannel.type != 0) {
		Release_Slot_By_Name((char *)user.name);
		for (int player = 0; player < Session.Players.Count(); player++) {
			if (stricmp(Wide_To_Narrow(Session.Players[player]->Name), (char *)user.name) == 0) {
				NodeNameType * entry = Session.Players[player];
				Session.Players.Delete_Index(player);
				Session.Remove_Player_Port(player);
				delete entry;
				break;
			}
		}
		for (int slot = 0; slot < 8; slot++) {
			if (!Session.Slots[slot] || !Is_Real_Player_Entry(Session.Slots[slot])) g_PlayerPings[slot] = -1;
			char * slot_name = Wide_To_Narrow(Session.Slots[slot]->Name);
			Store_User_Ping(slot_name, g_PlayerPings[slot]);
		}
		if (g_IsChannelCreator) {
			Enable_Start_Button();
			Send_Player_List();
			Send_Lobby_Options(true, true);
			if (WS_Top_Window_ID() == 0x6B) {
				HWND window = WS_Top_Window();
				int index = SendDlgItemMessageA(window, 0x6EB, LB_GETCURSEL, 0, 0);
				if (index != -1) {
					MultiplayerGameMode * mode = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6EB, LB_GETITEMDATA, index, 0);
					if (mode) {
						mode->vt_entry_78();
						Draw_Player_List(0, false);
						return;
					}
				}
			} else if (Session.MPGameMode) {
				Session.MPGameMode->vt_entry_78();
			}
		}
	}
	Draw_Player_List(0, false);
}
#endif



#if 0
static inline void Reset_Wait_Event_At(int index) { ResetEvent(g_WaitEventHandles[index]); }
#endif

#if 0
static inline void Reset_All_Wait_Events(void) { for (int i = 0; i < EV_COUNT; i++) { Reset_Wait_Event_At(i); } }
#endif

/// <summary>
/// Logs the local player out of the WOL chat session. Sends a logout request to the Chat
/// interface and waits (with timeout retries) for it to complete or be aborted, then, for
/// World Domination Tour sessions, also closes the event handles and fully shuts down chat.
/// </summary>
#if 0
void Logout_WOnline(void)
{
	int i;
	DWORD wait_result;

	g_IgnoreNetStatus = TRUE;

	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
		g_WDTTeams.clear();
	}

	if (g_pChat != NULL) {
		Reset_All_Wait_Events();
		WOL_Wait_Callback();
		if (g_pChat->RequestLogout() == S_OK) {
			{
				WideWstring title(StringTable::LoadString("TXT_WOL_LOGGING_OUT", NULL, "D:\\Ra2\\wonline.cpp", 8220));
				Show_Wait_Window(0x20, title, false);
			}
			for (i = 0; i < 5; i++) {
				while ((wait_result = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100)) == WAIT_TIMEOUT) {
					WOL_Wait_Callback();
				}
				ResetEvent(g_WaitEventHandles[wait_result]);
				if (wait_result == EV_ABORT) {
					break;
				}
				if (wait_result == EV_NETIGNORE) {
					break;
				}
			}
		}

		HWND profile = WS_Find_Dialog(0x11E);
		if (profile) WS_Destroy_Dialog(profile, 0);
		Close_Wait_Window(0x20);
	}
}
#endif


/// <summary>
/// Tears down the WOL COM interfaces: unadvises each event sink, releases the Chat, NetUtil,
/// and Download interfaces, clears the global pointers, and terminates the ATL module.
/// </summary>
#if 0
void Shutdown_Chat(void)
{
	if (MultiplayerMapPreview) {
		delete MultiplayerMapPreview;
		MultiplayerMapPreview = NULL;
	}
	HWND profile = WS_Find_Dialog(0x11E);
	if (profile) WS_Destroy_Dialog(profile, 0);
	if (g_WOLInitialized) {
		Free_Lobby_Entries();
		if (g_pChat) {
			AtlUnadvise(g_pChat, IID_IChatEvent, g_dwChatAdvise);
			g_pChat->Release();
		}
		if (g_pNetUtil) {
			AtlUnadvise(g_pNetUtil, IID_INetUtilEvent, g_dwNetUtilAdvise);
			g_pNetUtil->Release();
		}
		if (g_pDownload) {
			AtlUnadvise(g_pDownload, IID_IDownloadEvent, g_dwDownloadAdvise);
			g_pDownload->Release();
		}
		g_pChat = NULL;
		g_pNetUtil = NULL;
		g_pDownload = NULL;
		for (int i = 0; i < EV_COUNT; i++) {
			CloseHandle(g_WaitEventHandles[i]);
		}
		_Module.Term();
		g_WOLInitialized = false;
	}
}
#endif



/// <summary>
/// Requests the World Domination Tour state.
/// This routine puts up no wait window of its own, which is what lets the wait callback
/// poll the tour state in the background while the player is doing something else. The
/// language code is refreshed from the installed product's SKU on the way through.
/// </summary>
/// <returns>Returns with true if a request was issued, or false if there is no NetUtil connection
/// to issue it on.</returns>
#if 0
bool Get_WDT_State_Silent(void)
{
	DWORD value = 0;

	HKEY rKey;
	char buf[256];
	if (APP_REG_KEY != NULL && strlen(APP_REG_KEY)) {
		strcpy(buf, APP_REG_KEY);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, buf, 0, KEY_READ, &rKey) == ERROR_SUCCESS) {
			DWORD type;
			DWORD valuesize = sizeof(value);
			RegQueryValueEx(rKey, "SKU", NULL, &type, (LPBYTE)&value, &valuesize);
			RegCloseKey(rKey);
		}
	}

	g_LanguageCode = value & 0xFF;
	if (g_pChat != NULL) {
		g_pChat->SetProductSKU(value);
	}

	if (g_pNetUtil != NULL) {
		DebugString("Get_WDT_State_Silent: WDTHost is %s, port is %d\n", g_WDTHost, g_WDTPort);
		g_pNetUtil->RequestWDTState(g_WDTHost, g_WDTPort, 3 /* RA2: everything, as WDT_REQ_STATE_EVERYTHING of wdtnet.h */);
		return(true);
	}
	return(false);
}
#endif



#if 0
int Init_Wonline(void)
{
	if (g_WOLInitialized) return(1);
	g_WOLInitialized = true;
	Init_Lobby_Entries();
	Clear_Slot_Lists();
	Reset_Slots();

	DWORD value = 0;
	HKEY rKey;
	char regkey[256];
	if (APP_REG_KEY != NULL && strlen(APP_REG_KEY)) {
		strcpy(regkey, APP_REG_KEY);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, regkey, 0, KEY_READ, &rKey) == ERROR_SUCCESS) {
			DWORD type;
			DWORD valuesize = sizeof(value);
			RegQueryValueEx(rKey, "SKU", NULL, &type, (LPBYTE)&value, &valuesize);
			RegCloseKey(rKey);
		}
	}

	value &= 0xFF;
	g_LanguageCode = value;
	g_GameSKU = value + 0x2100;

	Reset_WOL_Globals();
	g_IgnoreNetStatus = true;
	for (int i = 0; i < EV_COUNT; i++) {
		g_WaitEventHandles[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	return(Startup_Chat(ProgramInstance));
}
#endif


#include "wolopt.h"

namespace OwnerDraw {


}



#if 0
static inline char Get_Locale_Pointer(Wstring const & name, int ** value) { return g_UserLocales.getPointer(name, value); }
#endif


#if 0
static inline int Ask_Locale(void)
{
	SimpleWonlineDialogControl control;
	int answer = Run_Dialog_Control(0xFE, Locale_Dialog_Proc, &control);
	if (answer < 0) {
		return(0);
	}
	return(answer);
}
#endif



#if 0
bool Connect_To_Server(Server * server)
{
	strcpy((char *)server->login, g_NickName.get());
	strcpy((char *)server->password, g_LoginPassword.get());
	for (int i = 0; i < EV_COUNT; i++) {
		ResetEvent(g_WaitEventHandles[i]);
	}
	{
		WideWstring connecting(StringTable::LoadString("TXT_CONNECTING_SERVER", NULL, "D:\\Ra2\\wonline.cpp", 8397));
		Show_Wait_Window(0x10, connecting, false);
	}
	g_pChat->RequestConnection(server, 20, ((int &)g_PlaintextPassword) & 0xFF);

	int attempts = 20;
	for (;;) {
		int remaining = 30000;
		DWORD event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100);
		while (event == WAIT_TIMEOUT) {
			remaining -= 100;
			if (remaining <= 0) {
				goto connect_failed;
			}
			WOL_Wait_Callback();
			event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100);
		}
		{
			ResetEvent(g_WaitEventHandles[event]);
			if (event == EV_ABORT) {
				Close_Wait_Window(0xFFFFFFFF);
				return(false);
			}
			if (event == EV_ERROR) {
				ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 8410), 0, Wait_Callback_Thunk);
				Close_Wait_Window(0xFFFFFFFF);
				return(false);
			}
		}
		if (event == EV_COUNT) {
connect_failed:
			Close_Wait_Window(0xFFFFFFFF);
			return(false);
		}
		if (attempts-- < 0) {
			ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 8417), 0, Wait_Callback_Thunk);
			Close_Wait_Window(0xFFFFFFFF);
			return(false);
		}
		if (event == EV_CONNECTED) {
			break;
		}
	}
	Close_Wait_Window(0xFFFFFFFF);
	g_pChat->SetClientVersion(0x1000E);
	g_IgnoreNetStatus = false;
	Session.Read_MultiPlayer_Settings();

	WOLOptionsClass options;
	options.Load();
	rc_ptr<PlayerProfile> profile;
	if (options.Is_Bit1_Clear()) {
		profile = PlayerProfile::Load(g_NickName);
	} else {
		profile = g_PlayerProfile.Get();
	}
	if (Online_Session_State == 3 || Online_Session_State == 4) {
		{
			Wstring name(Server_Name_Without_Prefix(server));
			profile->SetName(&name);
		}
		if (options.Is_Bit1_Clear()) {
			profile->Save(&g_NickName);
		}
	}
	strcpy(Session.Handle, g_NickName.get());
	Session.Set_Side(profile->Country, false);
	Session.Set_Color(profile->Color, false);
	Session.Write_MultiPlayer_Settings();
	WOL_Set_Last_Valid_Persona(g_NickName);
	if (g_pNetUtil && strlen(g_LadderAddress)) {
		g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, g_NickName.get(), g_GameSKU & 0xFFFF00, -1, 0, 0);
	}
	for (int j = 0; j < EV_COUNT; j++) {
		ResetEvent(g_WaitEventHandles[j]);
	}
	User user;
	memset(&user, 0, sizeof(user));
	strcpy((char *)user.name, g_NickName.get());
	g_pChat->RequestUserLocale(&user);
	for (;;) {
		int remaining = 30000;
		DWORD event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100);
		while (event == WAIT_TIMEOUT) {
			remaining -= 100;
			if (remaining <= 0) {
				goto locale_wait_failed;
			}
			WOL_Wait_Callback();
			event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100);
		}
		{
			ResetEvent(g_WaitEventHandles[event]);
			if (event == EV_ABORT) {
				Close_Wait_Window(0xFFFFFFFF);
				break;
			}
			if (event == EV_ERROR) {
				ODMessageBox(StringTable::LoadString("TXT_WDT_NET_ERR", NULL, "D:\\Ra2\\wonline.cpp", 8501), 0, Wait_Callback_Thunk);
				Close_Wait_Window(0xFFFFFFFF);
				break;
			}
		}
		if (event == EV_COUNT) {
locale_wait_failed:
			Close_Wait_Window(0xFFFFFFFF);
			break;
		}
		if (event == EV_LOCALE) {
			break;
		}
	}

	Wstring const & nick_source = g_NickName;
	Wstring nick = nick_source.toLower();
	if (g_UserLocales.contains(nick)) {
		int locale = 0;
		int * value = NULL;
		if (Get_Locale_Pointer(nick, &value) && value != NULL) {
			locale = *value;
		}
		if (locale != 0) {
			if (profile.Get() != NULL && profile->Locale != locale) {
				if (profile->Locale == 0) {
					profile->SetLocale(locale, 2);
					profile->Save(&g_NickName);
				}
				locale = profile->Locale;
				g_pChat->RequestSetLocale((Locale)locale);
				g_pChat->RequestUserLocale(&user);
				g_UserLocales.updateValue(nick, locale);
			}
			return(true);
		}
	}
	int chosen = 0;
	if (profile.Get() == NULL || (chosen = profile->Locale) == 0) {
		chosen = Ask_Locale();
		if (profile.Get() != NULL) {
			profile->SetLocale(chosen, 3);
			profile->Save(&g_NickName);
		}
	}
	if (chosen != 0) {
		g_pChat->RequestSetLocale((Locale)chosen);
		g_pChat->RequestUserLocale(&user);
		int stored = chosen;
		g_UserLocales.updateValue(nick, stored);
	}
	return(true);
}
#endif



#if 0
bool Connect_To_Selected_Server(void)
{
	if (g_CurrentServerIndex >= 1 && g_CurrentServerIndex < g_Servers.length()) {
		Server * server = NULL;
		g_Servers.getPointer(&server, g_CurrentServerIndex);
		return(Connect_To_Server(server));
	}
	return(false);
}
#endif


#if 0
bool Recall_Stored_Login(Wstring const & name)
{
	g_LoginPassword.clear();
	for (int index = 1; index <= 32; index++) {
		char const * nickname;
		char const * password;
		if (g_pChat->GetNick(index, &nickname, &password) == 0 && nickname && strlen(nickname) && !stricmp(name.text(), nickname)) {
			g_NickName = nickname;
			g_LoginPassword = password;
			g_PlaintextPassword = false;
			return(true);
		}
	}
	g_NickName.clear();
	return(false);
}
#endif


#if 0
int Find_Server_Suffix(Wstring const & name)
{
	if (name.length() != 0) {
		for (int i = 1; i < g_Servers.length(); i++) {
			Server * server = NULL;
			g_Servers.getPointer(&server, i);
			char * separator = strchr((char *)server->name, ':');
			if (separator && *separator && name == (separator + 1)) {
				return(i);
			}
		}
	}
	return(-1);
}
#endif


static const HRESULT CHAT_E_BANNED = (HRESULT)0x80040072;
static const HRESULT CHAT_E_DISABLED = (HRESULT)0x80040074;
static const HRESULT CHAT_E_SERIALBANNED = (HRESULT)0x80040075;
static const HRESULT CHAT_E_SERIALDUP = (HRESULT)0x80040076;
static const HRESULT CHAT_E_SERIALUNKNOWN = (HRESULT)0x80040077;
static const HRESULT CHAT_E_SKUSERIALMISMATCH = (HRESULT)0x80040078;

#if 0
bool Handle_Connection_Error(void)
{
	bool handled = false;
	char url[260];
	if (g_OnConnectionResult == CHAT_E_SERIALUNKNOWN || g_OnConnectionResult == CHAT_E_SKUSERIALMISMATCH || g_OnConnectionResult == CHAT_E_BANNED || g_OnConnectionResult == CHAT_E_DISABLED || g_OnConnectionResult == CHAT_E_SERIALBANNED || g_OnConnectionResult == CHAT_E_SERIALDUP) {
		handled = true;
		if (ODMessageBox(g_LastErrorMessage, 4, Wait_Callback_Thunk, false) == 6) {
			int language = g_LanguageCode;
			switch (g_OnConnectionResult) {
			case CHAT_E_SERIALUNKNOWN: sprintf(url, "http://apiregister.westwood.com/support/serialunknown_%d.html", language); break;
			case CHAT_E_SKUSERIALMISMATCH: sprintf(url, "http://apiregister.westwood.com/support/skuserialmismatch_%d.html", language); break;
			case CHAT_E_BANNED: sprintf(url, "http://apiregister.westwood.com/support/banned_%d.html", language); break;
			case CHAT_E_DISABLED: sprintf(url, "http://apiregister.westwood.com/support/disabled_%d.html", language); break;
			case CHAT_E_SERIALBANNED: sprintf(url, "http://apiregister.westwood.com/support/serialbanned_%d.html", language); break;
			case CHAT_E_SERIALDUP: sprintf(url, "http://apiregister.westwood.com/support/serialdup_%d.html", language); break;
			}
			ViewHTML(url, 1);
		}
	} else {
		ODMessageBox(g_LastErrorMessage, 0, Wait_Callback_Thunk, false);
	}
	return(handled);
}
#endif


#if 0
void Close_Wait_Window(unsigned int event)
{
	HWND win = WS_Find_Dialog(196);
	if (win) {
		g_ActiveWaitFlags &= ~event;
		if (g_ActiveWaitFlags == 0) {
			WS_Destroy_Dialog(win, 0);
		}
	}
	HWND top = WS_Top_Window();
	HWND control = GetDlgItem(top, 1344);
	if (control) {
		SetFocus(control);
	}
}
#endif


#if 0
void Reset_Wait_Events(void)
{
	for (int i = 0; i < EV_COUNT; i++) {
		ResetEvent(g_WaitEventHandles[i]);
	}
}
#endif



#include "ccfile.h"
#include "mixfile.h"
#include "_pk.h"
#include "wdtnet.h"

#if 0
int Pick_Lobby(bool require_room)
{
	int count = g_UserChannels.length();
	Channel channel;
	memset(&channel, 0, sizeof(channel));
	char section[32];
	char entry[32];
	char fallback[32];
	char name[32];
	wchar_t territory[32];
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
		memset(section, 0, sizeof(section));
		memset(territory, 0, 32);
		memset(entry, 0, sizeof(entry));
		memset(fallback, 0, sizeof(fallback));
		MixFileClass * archive = new MixFileClass("WDT.MIX", &FastKey);
		{
			INIClass ini;
			CCFileClass file("WDTMAP.INI");
			ini.Load(file, false);
			sprintf(entry, "Territory%02d", Session.TournamentTerritory);
			strcpy(fallback, entry);
			if (Current_WDT_State()->MapID == 0) {
				ini.Get_String("NorthAmerica", entry, fallback, section, sizeof(section));
			} else {
				ini.Get_String("Europe", entry, fallback, section, sizeof(section));
			}
			ini.Get_String(section, "Name", "A territory", name, sizeof(name));
			Char_To_Wide(territory, name, -1);
			g_TerritoryName = _wcsdup(territory);
			delete archive;
		}
	}
	Wstring lobby_name = Wstring("Lob_33_") + Wstring_FromInt(g_LobbyNumber, 1);
	int i;
	for (i = 0; i < count; i++) {
		g_UserChannels.get(channel, i);
		if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
			if (wcscmp((wchar_t const *)g_Lobbies[i], territory) != 0) {
				continue;
			}
			if (!require_room || (int)channel.currentUsers <= 25) {
				return(i);
			}
		}
		Wstring channel_name((char const *)channel.name);
		if (channel_name == lobby_name) {
			if (!require_room || (int)channel.currentUsers <= 25) {
				return(i);
			}
		}
	}
	int best = -1;
	unsigned int smallest = 10000;
	for (i = 0; i < count; i++) {
		g_UserChannels.get(channel, i);
		if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
			if (wcsncmp((wchar_t const *)g_Lobbies[i], territory, wcslen(territory)) != 0) {
				continue;
			}
			if (!require_room || (int)channel.currentUsers <= 25) {
				g_LobbyNumber = atol((char const *)channel.name + strlen("Lob_33_"));
				return(i);
			}
			if (channel.currentUsers < smallest) {
				best = i;
				smallest = channel.currentUsers;
			}
		}
		if (strncmp((char const *)channel.name, "Lob_33_", strlen("Lob_33_")) == 0) {
			if (!require_room || (int)channel.currentUsers <= 25) {
				g_LobbyNumber = atol((char const *)channel.name + strlen("Lob_33_"));
				return(i);
			}
			if (channel.currentUsers < smallest) {
				best = i;
				smallest = channel.currentUsers;
			}
		}
	}
	if (best != -1) {
		g_UserChannels.get(channel, best);
		g_LobbyNumber = atol((char const *)channel.name + strlen("Lob_33_"));
	}
	return(best);
}
#endif


#if 0
bool Leave_Current_Channel(unsigned int event, bool block)
{
	if (g_ChannelCount) {
		DebugString("Leave_Channel, channel %s type %d\n", g_CurrentChannel.name, g_CurrentChannel.type);
		g_pChat->RequestChannelLeave();
		{
			WideWstring title(StringTable::LoadString("WOL:LeavingChannel", NULL, "D:\\Ra2\\wonline.cpp", 9379));
			Show_Wait_Window(event, title, block);
		}
		for (int i = 0; g_ChannelCount && i < 150; i++) {
			Sleep(100);
			WOL_Wait_Callback();
		}
		Close_Wait_Window(0x40);
	}
	return(g_ChannelCount == 0);
}
#endif


#if 0
int Request_Channel_Join(Channel * channel, Wstring const & password, WideWstring title)
{
	char const * name = (char const *)channel->name;
	if (strcmp(name, (char const *)g_CurrentChannel.name) && strcmp(name, (char const *)g_PendingChannel.name)) {
		if (g_ChannelCount) {
			DebugString("Leave_Channel, channel %s type %d\n", g_CurrentChannel.name, g_CurrentChannel.type);
			g_pChat->RequestChannelLeave();
			{
				WideWstring leaving(StringTable::LoadString("WOL:LeavingChannel", NULL, "D:\\Ra2\\wonline.cpp", 9379));
				Show_Wait_Window(0x40, leaving, true);
			}
			for (int i = 0; g_ChannelCount && i < 150; i++) {
				Sleep(100);
				WOL_Wait_Callback();
			}
			Close_Wait_Window(0x40);
			if (g_ChannelCount) {
				return(1);
			}
		}
		strcpy((char *)channel->key, password.text());
		g_PendingChannel = *channel;
		{
			WideWstring message(title.text());
			Show_Wait_Window(2, message, false);
		}
		DebugString("Requesting channel join %s\n", name);
		return g_pChat->RequestChannelJoin(channel);
	}
	return(0);
}
#endif


#if 0
int Join_Selected_Lobby(bool join)
{
	int choice = Pick_Lobby(join);
	if (choice != -1) {
		Channel channel;
		g_UserChannels.get(channel, choice);
		int result;
		{
			Wstring password("zotclot9");
			result = Request_Channel_Join(&channel, password, WideWstring(StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 9428)));
		}
		if (!result) {
			HWND dialog = WS_Find_Dialog(0x113);
			if (dialog) {
				SetFocus(GetDlgItem(dialog, 1344));
			}
			Refresh_Game_List();
			return(choice);
		}
	}
	Refresh_Game_List();
	return(-1);
}
#endif


#if 0
int Save_Nickname(char const * name, char const * password, bool mangle, int locale)
{
	int slot = 0;
	char const * nick;
	char const * stored;
	for (int i = 1; i <= 32; i++) {
		nick = NULL;
		stored = NULL;
		if (g_pChat->GetNick(i, &nick, &stored) == 0 && nick && !strcmp(nick, name)) {
			slot = i;
			break;
		}
	}
	if (slot == 0) {
		for (int i = 1; i <= 32; i++) {
			nick = NULL;
			stored = NULL;
			if (g_pChat->GetNick(i, &nick, &stored) != 0 || !nick || strlen(nick) == 0) {
				slot = i;
				break;
			}
		}
	}
	if (slot == 0) {
		int next = Session.LastNicknameSlot;
		if (next == -1) {
			next = 1;
		}
		slot = next++;
		Session.LastNicknameSlot = next;
		if (next > 32) {
			Session.LastNicknameSlot = 1;
		}
	}
	if (slot) {
		g_pChat->SetNick(slot, name, password, mangle);
		g_pChat->SetNickLocale(slot, (Locale)locale);
	}
	return(slot);
}
#endif


#if 0
Wstring Server_Name_Without_Prefix(Server const * server)
{
	Wstring name((char const *)server->name);
	Wstring token;
	int position = name.getToken(0, ":", token);
	if (position >= 0) {
		name.remove(0, position + 1);
	}
	return(name);
}
#endif


#define OD_ADDSTRING_COMBO (WM_USER + 188)
#define OD_ADDSTRING_LIST (WM_USER + 200)

#if 0
bool Populate_Server_Choices(HWND list, bool combo)
{
	if (combo) {
		Clear_ComboBox(list);
	} else {
		Clear_Listbox(list);
	}
	if (g_Servers.length() == 1) {
		if (combo) {
			SendMessage(list, OD_ADDSTRING_COMBO, 0, (LPARAM)"GUI:NoServersAvailable");
		} else {
			SendMessage(list, OD_ADDSTRING_LIST, 0, (LPARAM)"GUI:NoServersAvailable");
		}
		EnableWindow(list, FALSE);
		return(false);
	}
	Wstring preferred;
	WOLOptionsClass options;
	options.Load();
	rc_ptr<PlayerProfile> profile;
	if (options.Is_Bit1_Clear()) {
		profile = PlayerProfile::Load(g_NickName);
	} else {
		profile = g_PlayerProfile.Get();
	}
	if (profile.Get()) {
		preferred = profile->Name;
	}
	EnableWindow(list, TRUE);
	int selected = -1;
	for (int i = 1; i < g_Servers.length(); i++) {
		Server * server = NULL;
		g_Servers.getPointer(&server, i);
		DebugString("Server found %s\n", (char *)server->name);
		Wstring display = Server_Name_Without_Prefix(server);
		int row;
		if (combo) {
			row = SendMessage(list, OD_ADDSTRING_COMBO, 0, (LPARAM)display.text());
			SendMessage(list, CB_SETITEMDATA, row, i);
		} else {
			row = SendMessage(list, OD_ADDSTRING_LIST, 0, (LPARAM)display.text());
			SendMessage(list, LB_SETITEMDATA, row, i);
		}
		if (preferred.length() > 0) {
			bool match = ((Wstring const &)preferred).toLower() == ((Wstring const &)display).toLower();
			if (match) {
				selected = row;
				g_CurrentServerIndex = i;
				g_TargetServerIndex = i;
			}
		}
	}
	if (selected == -1) {
		selected = 0;
	}
	if (combo) {
		SendMessage(list, CB_SETCURSEL, selected, 0);
	} else {
		SendMessage(list, LB_SETCURSEL, selected, 0);
		SendMessage(list, LB_SETTOPINDEX, selected, 0);
	}
	return(true);
}
#endif



#include "misc.h"

#if 0
bool __fastcall Join_Game_Channel(Channel const * channel, bool observer)
{
	if (!Can_Join_Channel(channel, true, observer)) {
		return(false);
	}
	unsigned int mode = channel->reserved;
	int maximum = (mode & 0x400) ? 800 : ((mode & 0x200) ? 640 : 1024);
	if (VideoModeWidth > maximum) {
		{
			WideWstring warning((mode & 0x400) ? StringTable::LoadString("WOL:ResolutionLimited800", NULL, "D:\\Ra2\\wonline.cpp", 10143) : StringTable::LoadString("WOL:ResolutionLimited640", NULL, "D:\\Ra2\\wonline.cpp", 10144));
			if (ODMessageBox(warning.get(), 4, Wait_Callback_Thunk, true) == 6) {
				int width = (maximum == 800) ? 800 : 640;
				int height = (width == 800) ? 600 : 480;
				if (Test_Display_Mode_Dialog(width, height)) {
					Options.ScreenWidth = width;
					Options.ScreenHeight = height;
					Options.Save_Settings();
				}
			}
		}
		if (VideoModeWidth > maximum) {
			return(false);
		}
	}
	Wstring password;
	if (channel->flags & 0x100) {
		Darken_Surface(NULL, true);
		WonlineStringDialogControl control;
		if (Run_Dialog_Control(0xE1, (DLGPROC)Wonline_Password_Dialog_Proc, &control) == 2) {
			return(false);
		}
		password = control.Text;
		password.replace(" ", "_");
	}
	return(Request_Channel_Join((Channel *)channel, password, WideWstring(StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 10199))) == 0);
}
#endif



#if 0
void Update_Channel_Player_Limit(void)
{
	int limit = Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex);
	if (ObserverMode == 1) {
		++limit;
	}
	int fixed_slots = 0;
	int open_slots = 0;
	int player_slots = 0;
	int i = 1;
	for (; i < limit; ++i) {
		NodeNameType * entry = Session.Slots[i];
		if (entry != LobbyEntries[0] && entry != LobbyEntries[6]) {
			if (Is_Real_Player_Entry(entry)) {
				++fixed_slots;
			} else if (Is_AI_Entry(entry)) {
				++player_slots;
			}
		} else {
			++open_slots;
		}
	}
	for (; i < 8; ++i) {
		NodeNameType * entry = Session.Slots[i];
		if (entry == LobbyEntries[6]) {
			++open_slots;
		} else if (Is_Real_Player_Entry(entry)) {
			++fixed_slots;
		}
	}
	if (g_pChat) {
		int capacity = open_slots + fixed_slots + 1;
		Wstring command = Wstring("mode #") + Wstring((char const *)g_CurrentChannel.name) + Wstring(" +l ") + Wstring_FromInt(capacity) + Wstring("\r\n");
		g_pChat->RequestRawMessage(command.get());
		char info[80];
		memset(info, 0, sizeof(info));
		Encode_Channel_ExInfo(info);
		info[40] = 0;
		g_pChat->SetChannelExInfo(info);
		g_pChat->Unknown_Virtual_FC(1, capacity + player_slots);
	}
}
#endif



#if 0
wchar_t * __fastcall Player_Ping_Tooltip(wchar_t const * name)
{
	int ping = g_PlayerPings[Find_Slot_By_Name(name)];
	if (ping < 0) {
		swprintf(g_PingTooltip, StringTable::LoadString("TXT_LATENCY_ICON_TOOLTIP_NOPING", NULL, "D:\\Ra2\\wonline.cpp", 14063), name, StringTable::LoadString("TXT_UNKNOWN", NULL, "D:\\Ra2\\wonline.cpp", 14063), StringTable::LoadString("TXT_UNKNOWN_PING", NULL, "D:\\Ra2\\wonline.cpp", 14063));
	} else if (ping < 300) {
		swprintf(g_PingTooltip, StringTable::LoadString("TXT_LATENCY_ICON_TOOLTIP_PING", NULL, "D:\\Ra2\\wonline.cpp", 14065), name, StringTable::LoadString("TXT_GOOD", NULL, "D:\\Ra2\\wonline.cpp", 14065), ping);
	} else if (ping < 500) {
		swprintf(g_PingTooltip, StringTable::LoadString("TXT_LATENCY_ICON_TOOLTIP_PING", NULL, "D:\\Ra2\\wonline.cpp", 14067), name, StringTable::LoadString("TXT_FAIR", NULL, "D:\\Ra2\\wonline.cpp", 14067), ping);
	} else {
		swprintf(g_PingTooltip, StringTable::LoadString("TXT_LATENCY_ICON_TOOLTIP_PING", NULL, "D:\\Ra2\\wonline.cpp", 14069), name, StringTable::LoadString("TXT_POOR", NULL, "D:\\Ra2\\wonline.cpp", 14069), ping);
	}
	return(g_PingTooltip);
}
#endif


#if 0
int __fastcall Slot_From_Control_Id(int id)
{
	if (id == 0x754) {
		return(0);
	}
	if (id == 0x755) {
		return(1);
	}
	if (id == 0x756) {
		return(2);
	}
	if (id == 0x757) {
		return(3);
	}
	if (id == 0x758) {
		return(4);
	}
	if (id == 0x759) {
		return(5);
	}
	if (id == 0x75A) {
		return(6);
	}
	return(id == 0x75B ? 7 : -1);
}
#endif


#if 0
void __fastcall Enforce_Tournament_Options(HWND dialog)
{
	if (Is_Channel_Tournament(&g_CurrentChannel)) {
		EnableWindow(GetDlgItem(dialog, 0x552), FALSE);
		EnableWindow(GetDlgItem(dialog, 0x553), FALSE);
		EnableWindow(GetDlgItem(dialog, 0x597), FALSE);
		Session.Options.AIPlayers = 0;
		Session.Options.AlliesAllowed = false;
		HWND first = GetDlgItem(dialog, 0x54E);
		SendMessage(first, BM_SETCHECK, 1, 0);
		EnableWindow(first, FALSE);
		HWND second = GetDlgItem(dialog, 0x54B);
		SendMessage(second, BM_SETCHECK, 1, 0);
		EnableWindow(second, FALSE);
		Session.Options.ShortGame = true;
		Session.Options.Bases = true;
	}
}
#endif


#if 0
void __fastcall Leave_Channel_With_Message(HWND dialog)
{
	if (dialog) {
		if (ObserverMode == 0) {
			ODMessageBox(StringTable::LoadString("TXT_CHANNEL_FULL", NULL, "D:\\Ra2\\wonline.cpp", 15375), 0, Wait_Callback_Thunk, false);
		} else {
			ODMessageBox(StringTable::LoadString("TXT_OBSERVER_FULL", NULL, "D:\\Ra2\\wonline.cpp", 15377), 0, Wait_Callback_Thunk, false);
		}
		if (g_pChat->RequestChannelLeave() < 0) {
			WS_Destroy_Dialog(dialog, 2);
		}
	}
}
#endif


#define OD_SETLABEL (WM_USER + 178)

#if 0
void Show_Wait_Window(unsigned int event, WideWstring const & text, bool block)
{
	HWND dialog = WS_Find_Dialog(0xC4);
	if (dialog) {
		g_ActiveWaitFlags |= event;
		if (text.length() > 0) {
			HWND label = GetDlgItem(dialog, 0x5B2);
			SendMessage(label, OD_SETLABEL, 0, (LPARAM)text.text());
		}
	} else if (event) {
		HWND focus = WS_Top_Window();
		focus = GetDlgItem(focus, 0x540);
		dialog = WS_Create_Dialog(ProgramInstance, 0xC4, MainWindow, (DLGPROC)Wonline_Wait_Dialog_Proc, 0);
		Show_Dialog(dialog, 0xC4);
		HWND label = GetDlgItem(dialog, 0x5B2);
		SendMessage(label, OD_SETLABEL, 0, (LPARAM)text.text());
		g_ActiveWaitFlags = event;
		if (focus) {
			SetFocus(focus);
		}
	}
	if (block && g_ActiveWaitFlags) {
		WS_Wait_Dialog(dialog, Wait_Callback_Thunk, false, true, false);
	}
}
#endif



#if 0
LRESULT CALLBACK Wonline_Wait_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_DESTROY:
		KillTimer(window, 0x45);
		break;
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(1);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		break;
	case WM_ERASEBKGND:
		return(1);
	case WM_INITDIALOG:
		return(0);
	case WM_COMMAND:
		switch ((unsigned short)wparam) {
		case 0x58D: // the abort button
			SetEvent(g_WaitEventHandles[0]);
			WS_Destroy_Dialog(window, 1);
			break;
		case 0x58E: // the status button
			ViewHTML("http://networkstatus.westwood.com", 0);
			break;
		}
		break;
	case WM_TIMER:
		if (wparam == 0x45) {
			ShowWindow(window, SW_SHOW);
			KillTimer(window, 0x45);
		}
		break;
	case 0x497:
		SendMessage(window, 0x4A9, 0, 1);
		SetTimer(window, 0x45, g_WaitDialogDelay, NULL);
		OwnerDraw::Set_Dialog_Waiting(window);
		break;
	}
	return(0);
}
#endif


#if 0
void Set_Wait_Window_Text(LPARAM text)
{
	HWND dialog = WS_Find_Dialog(0xC4);
	if (dialog != NULL) {
		SendDlgItemMessage(dialog, 0x5B2, OD_SETLABEL, 0, text);
	}
}
#endif


#include "wsproto.h"

#if 0
void __fastcall User_List_To_Session_Players(User * user)
{
	Session.NumPlayers = 1;
	User entry;
	for (int i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(entry, i);
		NodeNameType * player = Find_Lobby_Name(Narrow_To_Wide((char const *)entry.name));
		if (player && Is_Real_Player_Entry(player)) {
			player->Player.SquadID = entry.squadID;
		}
		if (stricmp((char const *)entry.name, g_NickName.get())) {
			if (entry.flags & 1) {
				NodeNameType * owner = Find_Lobby_Name(Narrow_To_Wide((char const *)entry.name));
				NetNumType network;
				NetNodeType node;
				owner->Address.Get_Address(network, node);
				Session.HostAddress.Set_Address(network, node, 0);
			}
			int ping = 0;
			g_pNetUtil->GetAvgPing(entry.ipaddr, &ping);
			if (ping < 0) {
				ping = 0;
			}
			PlayerPings[Session.NumPlayers++] = ping;
		}
	}
	PacketTransport->Clear_Broadcast_Addresses();
	for (int n = 1; n < Session.Players.Count(); n++) {
		DebugString("Adding broadcast address %s\n", Session.Players[n]->Address.As_String());
		PacketTransport->Set_Broadcast_Address((char const *)&Session.Players[n]->Address, 0);
	}
	DebugString("User_List_To_Session_Players: Session.HostAddress = %s\n", Session.HostAddress.As_String());
}
#endif



#if 0
bool __fastcall Local_User_Is_Channel_Owner(void)
{
	return(User_Is_Channel_Owner(g_NickName));
}
#endif


#if 0
bool User_Is_Channel_Owner(Wstring const & name)
{
	User * owner = Find_Owner_User();
	if (owner == NULL) {
		return(false);
	}
	bool same = stricmp((char *)owner->name, name.text()) == 0;
	return(same);
}
#endif


#if 0
User * Find_Owner_User(void)
{
	for (int i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(g_OwnerUser, i);
		if (g_OwnerUser.flags & 1) {
			return(&g_OwnerUser);
		}
	}
	return(NULL);
}
#endif


#if 0
void __fastcall Mark_Player_Checked(int index)
{
	if (index >= 0 && index < Session.Players.Count()) {
		HWND dialog = WS_Find_Dialog(0x12C);
		if (dialog) {
			HWND box = GetDlgItem(dialog, g_PlayerCheckBoxIDs[index]);
			g_PlayerChecked[index] = true;
			SendMessage(box, BM_SETCHECK, 1, 0);
			PostMessage(dialog, WM_PAINT, 0, 0);
		}
	}
}
#endif



#if 0
void Open_Readiness_Dialog(void)
{
	HWND dialog = WS_Create_Dialog(ProgramInstance, 300, MainWindow, (DLGPROC)Player_Readiness_Dialog_Proc, 0);
	if (dialog != NULL) {
		Show_Dialog(dialog, 300);
	}
}
#endif



#if 0
LRESULT CALLBACK Player_Readiness_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		ValidateRect(window, NULL);
		return(0);
	case WM_ERASEBKGND:
		return(1);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(1);
	case WM_COMMAND:
		wparam &= 0xFFFF;
		switch (wparam) {
		case 0x761: case 0x762: case 0x764: case 0x765:
		case 0x766: case 0x767: case 0x768: case 0x769:
			if (g_PlayerReady[wparam - 0x761]) {
				if (SendDlgItemMessage(window, wparam, BM_GETCHECK, 0, 0) == 0) {
					SendDlgItemMessage(window, wparam, BM_SETCHECK, 1, 0);
				}
			} else {
				if (SendDlgItemMessage(window, wparam, BM_GETCHECK, 0, 0) == 1) {
					SendDlgItemMessage(window, wparam, BM_SETCHECK, 0, 0);
					return(0);
				}
			}
		}
		break;
	case WM_CTLCOLORMSGBOX: case WM_CTLCOLOREDIT: case WM_CTLCOLORLISTBOX: case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG: case WM_CTLCOLORSCROLLBAR: case WM_CTLCOLORSTATIC:
		return((long)GetStockObject(BLACK_BRUSH));
	case 0x497:
		for (int i = 0; i < 8; ++i) {
			HWND control = GetDlgItem(window, g_PlayerCheckBoxIDs[i]);
			memset(g_PlayerChecked, 0, 8);
			if (i > Session.Players.Count() - 1) {
				DestroyWindow(control);
			}
		}
		for (int player = 0; player < Session.Players.Count(); ++player) {
			HWND control = GetDlgItem(window, g_PlayerCheckBoxIDs[player]);
			SendMessage(control, 0x4B2, 0, (LPARAM)Session.Players[player]->Name);
			SendMessage(control, BM_SETCHECK, 0, 0);
		}
		SendMessage(window, 0x4A9, 0, 1);
		OwnerDraw::Set_Dialog_Waiting(window);
		ShowWindow(window, SW_SHOW);
		break;
	}
	return(0);
}
#endif



#if 0
void __fastcall Set_Connection_Status(int index, int status)
{
	if (index < Session.Players.Count() && g_ConnectionStatus[index] < g_ConnectionStatusRank[index]) {
		g_ConnectionStatus[index] = status;
	}
}
#endif


#if 0
void Report_Connection_Errors(void)
{
	wchar_t message[256];
	User * owner = Find_Owner_User();
	if (owner) {
		int owner_index = -1;
		int i;
		for (i = 0; i < Session.Players.Count(); i++) {
			if (!stricmp(Wide_To_Narrow(Session.Players[i]->Name), (char const *)owner->name)) {
				owner_index = i;
				break;
			}
		}
		bool reported = false;
		for (i = 0; i < Session.Players.Count(); i++) {
			switch (g_ConnectionStatus[i]) {
			case 0:
				reported = true;
				DebugString("Player %s was unable to establish a connection\n", Wide_To_Narrow(Session.Players[i]->Name));
				swprintf(message, StringTable::LoadString("TXT_TALK_REPORTED_PROBLEM", NULL, "D:\\Ra2\\wonline.cpp", 16894), Session.Players[i]->Name);
				Lobby_Chat_Printf(ColorSystem, message);
				break;
			case 1:
				reported = true;
				DebugString("Unable to connect to player %s\n", Wide_To_Narrow(Session.Players[i]->Name));
				swprintf(message, StringTable::LoadString("TXT_TALK_LOCAL_PROBLEM", NULL, "D:\\Ra2\\wonline.cpp", 16904), Session.Players[i]->Name);
				Lobby_Chat_Printf(ColorSystem, message);
				break;
			case 2:
				reported = true;
				if (i == owner_index) {
					DebugString("Unable to talk to the pserver port\n");
					Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_TALK_FORWARDER_PROBLEM_GUES", NULL, "D:\\Ra2\\wonline.cpp", 16915));
				} else {
					DebugString("Player %s was unable to connect to the pserver port\n", Wide_To_Narrow(Session.Players[i]->Name));
					swprintf(message, StringTable::LoadString("TXT_TALK_FORWARDER_PROBLEM_HOST", NULL, "D:\\Ra2\\wonline.cpp", 16918), Session.Players[i]->Name);
					Lobby_Chat_Printf(ColorSystem, message);
				}
				break;
			case 3:
				reported = true;
				DebugString("The game host reported a connection problem between players\n");
				Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_TALK_HOST_ABORTED", NULL, "D:\\Ra2\\wonline.cpp", 16929));
				break;
			}
		}
		if (!reported) {
			Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_FAILED_TO_TALK", NULL, "D:\\Ra2\\wonline.cpp", 16942));
		}
	}
}
#endif


#if 0
unsigned short __fastcall Get_Port_Blitz_Port_Number(int send_port, int mangled_port, int unused, int port_spread)
{
	int port = send_port + mangled_port;
	if (port_spread != 1) {
		if (port_spread == 2) {
			port = mangled_port + send_port * 2;
		} else if (port_spread > 1) {
			port = send_port % 2 + mangled_port + (send_port / 2) * port_spread;
		} else if (port_spread < 0) {
			if (port_spread == -100) {
				port = mangled_port + send_port * 100;
			} else {
				port = send_port % 2 - (send_port / 2) * port_spread + mangled_port;
			}
		}
	}
	DebugString("Get_Port_Blitz_Port_Number - send port = %d, mangled_port = %d, port_spread = %d, port = %d\n", send_port, mangled_port, port_spread, port);
	if ((unsigned short)port < 1024) {
		port = 1024;
	}
	return((unsigned short)port);
}
#endif


#include "firewall.h"

#if 0
int __fastcall Build_Connection_Pairs(int (* pairs)[2])
{
	int order[8];
	memset(order, 0, sizeof(order));
	int i = 0;
	int * out = order;
	for (; i < 8; i++) {
		if (Is_Real_Player_Entry(Session.Slots[i])) {
			*out++ = i;
		}
	}
	for (int a = 0; a < Session.Players.Count(); a++) {
		for (int b = 0; b < Session.Players.Count() - 1; b++) {
			int current = Session.WolPlayerState[order[b]];
			int next = Session.WolPlayerState[order[b + 1]];
			if (FirewallHelper.Rank_A(current) < FirewallHelper.Rank_A(next)) {
				int temp = order[b];
				order[b] = order[b + 1];
				order[b + 1] = temp;
			}
		}
	}
	DebugString("Connection order = %d, %d, %d, %d, %d, %d\n", order[0], order[1], order[2], order[3], order[4], order[5]);
	int n = 0;
	DebugString("Connection pairs....\n");
	for (int x = 0; x < Session.Players.Count(); x++) {
		for (int y = x + 1; y < Session.Players.Count(); y++) {
			pairs[n][0] = order[x];
			pairs[n][1] = order[y];
			DebugString("\t%d & %d\n", order[x], order[y]);
			n++;
		}
	}
	return(n);
}
#endif


#if 0
void __fastcall Register_Connection_State(char const * sender, char const * receiver, int state)
{
	int sender_slot = -1;
	int receiver_slot = -1;
	DebugString("Registering connection state from %s to %s. conn_state = %d\n", sender, receiver, state);
	int i = 0;
	for (NodeNameType ** slot = Session.Slots; (long)slot < (long)(Session.Slots + 8); ++slot, ++i) {
		if (stricmp(Wide_To_Narrow((*slot)->Name), sender) == 0) {
			sender_slot = i;
		}
		if (stricmp(Wide_To_Narrow((*slot)->Name), receiver) == 0) {
			receiver_slot = i;
		}
	}
	for (i = 0; i < Session.ConnectionResultCount; ++i) {
		if (Session.PeerProbeData[i][0] == sender_slot && Session.PeerProbeData[i][1] == receiver_slot) {
			Session.ConnectionResults[i][0] = state;
			return;
		}
		if (Session.PeerProbeData[i][0] == receiver_slot && Session.PeerProbeData[i][1] == sender_slot) {
			Session.ConnectionResults[i][1] = state;
			return;
		}
	}
}
#endif


#if 0
void __fastcall Store_Player_Port(char const * name, unsigned short port)
{
	DebugString("Player %s is telling us we should send to port %d\n", name, port);
	for (int i = 0; i < 8; ++i) {
		if (!stricmp(Wide_To_Narrow(Session.Slots[i]->Name), name)) {
			Session.PeerPorts[i] = port;
			return;
		}
	}
}
#endif


#if 0
void __fastcall Send_User_Port(char const * name, unsigned short port)
{
	User * user = NULL;
	char text[256];
	for (int i = 0; i < g_UserList.length(); ++i) {
		g_UserList.getPointer(&user, i);
		if (stricmp(name, (char const *)user->name) == 0) {
			text[0] = 'Z';
			text[1] = '4';
			sprintf(text + 2, "%d", port);
			DebugString("Sending string %s to user %s\n", text, name);
			g_pChat->RequestPrivateGameOptions(user, text);
		}
	}
}
#endif


#if 0
void __fastcall Send_Public_Options(char kind, char const * text)
{
	char options[256];
	options[0] = 'Z';
	options[1] = '1';
	options[2] = kind + '0';
	if (text) {
		strcpy(options + 3, text);
	} else {
		options[3] = 0;
	}
	DebugString("Sending public options string %s\n", options);
	g_pChat->RequestPublicGameOptions(options);
}
#endif


#if 0
void Resend_Pending_Requests(void)
{
	for (int index = 0; index < Session.KeepAliveCount; ++index) {
		if (Session.KeepAliveTimer[index] == 0) {
			GlobalPacketType packet;
			memset(&packet, 0, sizeof(packet));
			packet.Command = NET_PING;
			strcpy(packet.Name, g_NickName.get());
			char const * name = Wide_To_Narrow(Session.Slots[Session.KeepAliveSlot[index]]->Name);
			DebugString("Sending keepalive NET_PING packet to %s\n", name);
			Ipx.Send_Global_Message_To_Name(&packet, 0x2F, 0, Session.Slots[Session.KeepAliveSlot[index]]->Name);
			Session.KeepAliveTimer[index] = 1200;
		}
	}
}
#endif


#if 0
void __fastcall Queue_Peer_Timeout(int peer)
{
	Session.KeepAliveSlot[Session.KeepAliveCount] = peer;
	Session.KeepAliveTimer[Session.KeepAliveCount] = 1200;
	++Session.KeepAliveCount;
}
#endif




#if 0
bool Connect_Player_Pairs(void)
{
	CDTimerClass<SystemTimerClass> timer;
	Session.KeepAliveCount = 0;
	memset(&Session.PeerAttemptBlock[3], 0, 0x20);
	int pair_slot = -1;
	g_SuspendChatPump = 0;
	if (Session.ConnectionResultCount == 0) {
		return(false);
	}
	((SessionPortAllocator &)Session).Allocate_Player_Ports();
	if (Session.Players.Count() > 0) {
		HWND window = WS_Find_Dialog(300);
		if (window) {
			HWND control = GetDlgItem(window, g_PlayerCheckBoxIDs[0]);
			g_PlayerChecked[0] = true;
			SendMessage(control, BM_SETCHECK, 1, 0);
			PostMessage(window, WM_PAINT, 0, 0);
		}
	}
	for (int pair = 0; pair < Session.ConnectionResultCount; ++pair) {
		int first = Session.PeerProbeData[pair][0];
		int second = Session.PeerProbeData[pair][1];
		int peer = -1;
		NodeNameType * first_player = Session.Slots[first];
		NodeNameType * second_player = Session.Slots[second];
		char const * first_name = Wide_To_Narrow(first_player->Name);
		char const * second_name = Wide_To_Narrow(second_player->Name);
		DebugString("Connecting pair %d, %s and %s\n", pair, first_name, second_name);
		char const * name = Wide_To_Narrow(Session.Slots[first]->Name);
		if (stricmp(name, g_NickName.get()) == 0) {
			peer = second;
			pair_slot = pair * 2 + 1;
			Session.ConnectionResults[pair][0] = 11;
		}
		name = Wide_To_Narrow(Session.Slots[second]->Name);
		if (stricmp(name, g_NickName.get()) == 0) {
			peer = first;
			pair_slot = pair * 2;
			Session.ConnectionResults[pair][1] = 11;
		}
		if (peer != -1) {
			int step;
			if (FirewallHelper.Rank_B(Session.WolPlayerState[first]) > FirewallHelper.Rank_B(Session.WolPlayerState[second])) {
				step = FirewallHelper.Rank_B(Session.WolPlayerState[first]);
			} else {
				step = FirewallHelper.Rank_B(Session.WolPlayerState[second]);
			}
			if (!Start_Talking_To(peer, pair_slot, step)) {
				return(false);
			}
		} else {
			DebugString("Waiting for other guys to connect\n");
			timer = CDTimerClass<SystemTimerClass>(960);
			int progress = 0;
			do {
				Sleep(5);
				WOL_Wait_Callback();
				Resend_Pending_Requests();
				int first_status = Session.ConnectionResults[pair][0];
				int second_status = Session.ConnectionResults[pair][1];
				if ((first_status == 11 || first_status == 0) && (second_status == 11 || second_status == 0)) {
					DebugString("Got both results - woohoo!\n");
					break;
				}
				if (first_status >= 1 && first_status < 11 && first_status - 1 > progress) {
					DebugString("Connecting player 1 is trying again - try_now = %d\n", first_status - 1);
					progress = first_status - 1;
					timer = CDTimerClass<SystemTimerClass>(840);
				}
				if (second_status >= 1 && second_status < 11 && second_status - 1 > progress) {
					DebugString("Connecting player 2 is trying again - try_now = %d\n", second_status - 1);
					progress = second_status - 1;
					timer = CDTimerClass<SystemTimerClass>(840);
				}
			} while (timer.Value() != 0);
		}
		if (Session.ConnectionResults[pair][0] != 11) {
			return(false);
		}
		if (Session.ConnectionResults[pair][1] != 11) {
			return(false);
		}
		DebugString("Connection good, next pair\n");
	}
	return(true);
}
#endif



#if 0
bool Receive_Host_Map_Preview(void)
{
	IPXAddressClass address;
	if (g_CurrentChannel.type && g_UserList.length() >= 2) {
		User * owner = Find_Owner_User();
		if (owner && !stricmp((char const *)owner->name, g_NickName.text())) {
			return(false);
		}
		if (WolOptionsLocked == 0 && Ipx.Global_Num_Receive()) {
			Call_Back();
			GlobalPacketType packet;
			int length = sizeof(packet);
			unsigned short product;
			if (Ipx.Peek_Global_Message(0, &packet, &length, &address, &product, NULL) && packet.Command == NET_PREVIEW_MODE) {
				HWND dialog = WS_Find_Dialog(0xC9);
				if (dialog) {
					ShowWindow(GetDlgItem(dialog, 0x468), SW_SHOWNORMAL);
				}
				User user;
				g_UserList.get(user, 0);
				user.next = NULL;
				User_List_To_Session_Players(&user);
				if (Ipx.Get_Global_Message(&packet, &length, &address, &product) && packet.Command == NET_PREVIEW_MODE && address.Is_Same_Node(Session.HostAddress) && product == 0xAA03) {
					owner = Find_Owner_User();
					if (owner && !stricmp(packet.Name, (char const *)owner->name)) {
						DebugString("Received NET_PREVIEW_MODE packet from host\n");
						Ipx.Set_Timing(50, (unsigned long)-1, 5000, true);
						g_ReceivingHostPreview = true;
						Receive_Random_Map_Preview();
						g_ReceivingHostPreview = false;
						Ipx.Set_Timing(30, (unsigned long)-1, 600, true);
						return(true);
					}
				}
			}
		}
	}
	return(false);
}
#endif


#if 0
void Add_Test_Player_Ol_Cruddy(void)
{
	NodeNameType * player = new NodeNameType;
	Char_To_Wide(player->Name, "Ol Cruddy", -1);
	player->Player.SquadID = -1;
	player->Player.House = 0;
	player->Player.Color = 3;
	NetNumType network;
	NetNodeType node;
	memset(network, 0, sizeof(network));
	node[5] = 0;
	node[4] = 0;
	node[3] = 82;
	node[2] = 81;
	node[1] = 2;
	node[0] = 10;
	player->Address.Set_Address(network, node, 0);
	Session.Players.Add(player);
	PlayerPings[Session.NumPlayers] = 100;
	++Session.NumPlayers;
}
#endif


#if 0
void Add_Test_Player_Potato(void)
{
	NodeNameType * player = new NodeNameType;
	Char_To_Wide(player->Name, "Potato", -1);
	player->Player.SquadID = -1;
	player->Player.House = 0;
	player->Player.Color = 0;
	NetNumType network;
	NetNodeType node;
	memset(network, 0, sizeof(network));
	node[5] = 0;
	node[4] = 0;
	node[3] = 97;
	node[2] = 20;
	node[1] = 2;
	node[0] = 10;
	player->Address.Set_Address(network, node, 0);
	Session.Players.Add(player);
	PlayerPings[Session.NumPlayers] = 100;
	++Session.NumPlayers;
}
#endif


#if 0
void Add_Test_Player_Potato_Too(void)
{
	NodeNameType * player = new NodeNameType;
	Char_To_Wide(player->Name, "PotatoToo", -1);
	player->Player.SquadID = -1;
	player->Player.House = 1;
	player->Player.Color = 2;
	NetNumType network;
	NetNodeType node;
	memset(network, 0, sizeof(network));
	node[5] = 0;
	node[4] = 0;
	node[3] = 250;
	node[2] = 80;
	node[1] = 2;
	node[0] = 10;
	player->Address.Set_Address(network, node, 0);
	Session.Players.Add(player);
	PlayerPings[Session.NumPlayers] = 100;
	++Session.NumPlayers;
	Session.HostAddress = player->Address;
}
#endif

