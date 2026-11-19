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
#include "geometry.h"
#include "rect.h"

#include "_keyboar.h"
#include "_map.h"
#include "_pk.h"
#include "_rules.h"
#include "arraylist.h"
#include "dict.h"
#include "ccfile.h"
#include "conquer.h"
#include "data.h"
#include "dbgprint.h"
#include "globals.h"
#include "goptions.h"
#include "ini.h"
#include "rcptr.h"
#include "rules.h"
#include "init.h"
#include "misc.h"
#include "mixfile.h"
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
#include "wonlinecontrol.h"
#include "wonlinestringcontrol.h"
#include "creategamecontrol.h"
#include "wdtnet.h"
#include "wolopt.h"
#include "wstring.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <shellapi.h>
#include <windows.h>
#undef LoadString
#include "stringtable.h"

struct ChannelUserInfo
{
	char name[20];
	int color;
	int house;
	int Unknown_1C;
	int accepted;
	char pad[0x70];
};

ChannelUserInfo g_UserInfo[16];
int g_RuleCRC;
int g_ArtCRC;
int g_AICRC;
HANDLE g_WaitEventHandles[18];
unsigned int g_ActiveWaitFlags;
wchar_t g_LastErrorMessage[256];
int g_IsChannelCreator;
bool g_GameStartRequested;
int g_MatchState;
int g_MatchTimeout;
int g_MatchResult;
long g_MatchStartTime;
Channel g_CurrentChannel;
int g_AllowPage;
int g_AllowFind;
int g_LangFilter;
int g_LobbyMusic;
int g_ShowAllChannels;
int g_DisplayAsian;
int g_DisplayLatin;
int g_LanguageCode;
unsigned g_LastWDTUserTeamRequestTime;
unsigned g_LastChannelLocaleRequestTime;
int g_NextWDTStatePollTime;
bool g_StartGameReceived;
bool g_StartGameRunning;
void Start_Game_Now(void);
Dictionary<Wstring, Ladder> * g_ActiveLadder;
int g_InstalledLadderSKU;
int g_SelectedLadderSKU;
Channel g_PendingChannel;
Server g_GameServer;
Server g_WDTServer;
int g_CurrentServerIndex;
int g_TargetServerIndex;
bool g_IgnoreNetStatus;
bool g_ShowMOTD = true;
int g_ServerType = -1;
HRESULT g_OnConnectionResult;
bool g_RememberLogin;
int g_LobbyNumber;
bool g_HasJoinedLobby = true;
bool g_WDTSoundsInitialized;
int g_NewNickResult;
int g_AgeCheckResult;
int g_BirthdateMonth;
int g_BirthdateDay;
int g_BirthdateYear;
int g_BirthdateNewsletter;
int g_AccountShareInfo;
bool g_PlaintextPassword = true;
int g_LastChatListTime;
int g_LastGameListTime;
unsigned short const * g_Lobbies[8];
int WDT_TERRITORY_LOBBY_INDEXES[8];
DWORD g_dwChatAdvise;
DWORD g_dwNetUtilAdvise;
DWORD g_dwDownloadAdvise;
int g_SquadInfoRequests;
int g_RequestingOwnSquadInfo;
int g_OwnSquadID;
char g_OwnSquadName[41];
int g_ChannelCount;
int g_LadderPos;
int g_JoinLobbyNow;
int g_UserAge;
int g_UserConsent;
int g_Col_ChanName;
int g_Col_GameName;
int g_Col_GameIcon;
int g_Col_GamePrivate;
int g_Col_GameObserver;
int g_Col_GamePingTime;
bool g_GameListRefreshing;
void __fastcall Clear_Listbox(HWND list);
void __fastcall Get_Window_Item_Text(HWND list, int row, Wstring & text);
/*
**	Initializes OLE and COM for the life of the program.
*/
class OLEInitializer
{
	public:
		OLEInitializer(void) { OleInitialize(NULL); }
		~OLEInitializer(void) { OleUninitialize(); }
};
OLEInitializer g_OLEInitializer;

Wstring g_MessageOfTheDay;

ArrayList<Server> g_Servers;
ArrayList<Channel> g_UserChannels;
ArrayList<Channel> g_GameChannelList;
ArrayList<User> g_UserList;
ArrayList<unsigned int> g_ChanListQueue;

Dictionary<Wstring, Ladder> g_BattleClansLadderDict(Wstring_Hash);
Dictionary<Wstring, Ladder> g_WDTLadderDict(Wstring_Hash);
Dictionary<Wstring, Ladder> g_GeneralLadderDict(Wstring_Hash);
Dictionary<Wstring, int> g_WDTTeams(Wstring_Hash);
ArrayList<Wstring> g_WDTTeamLookupQueue;
Dictionary<Wstring, int> g_UserLocales(Wstring_Hash);
ArrayList<Wstring> g_UserLocaleLookupQueue;

ArrayList<Wstring> g_WDT_NodWinEmphasisSounds;
ArrayList<Wstring> g_WDT_GDIWinEmphasisSounds;
ArrayList<Wstring> g_WDT_NodLoseEmphasisSounds;
ArrayList<Wstring> g_WDT_GDILoseEmphasisSounds;
ArrayList<Wstring> g_WDT_NodTerritoryWinSounds;
ArrayList<Wstring> g_WDT_GDITerritoryWinSounds;
ArrayList<Wstring> g_WDT_NodTerritoryLoseSounds;
ArrayList<Wstring> g_WDT_GDITerritoryLoseSounds;

ArrayList<Wstring> g_BuddyList;
ArrayList<Wstring> g_BuddyRequests;
ArrayList<Wstring> g_BuddyAddQueue;
Wstring g_BuddyFindName;
Wstring g_SelectedBuddy;
Dictionary<Wstring, unsigned int> g_BuddyStatus(Wstring_Hash);
Dictionary<Wstring, Channel> g_BuddyChannels(Wstring_Hash);

int g_BuddyNameWidth = g_BuddyColumns[1] - g_BuddyColumns[0] - 4;
int g_BuddyStatusWidth = g_BuddyColumns[5] - g_BuddyColumns[4] - 4;

ArrayList<Wstring> g_PingQueue;
ArrayList<Wstring> g_PingsPending;
Wstring g_MatchOpponent;

bool Is_Tournament_Session(void)
{
	return(Online_Session_State == 2 || Online_Session_State == 1);
}

Wstring g_AccountEmail;
Wstring g_AccountBirthMonth;
Wstring g_AccountBirthDay;
Wstring g_AccountBirthYear;
Wstring g_AccountName;
Wstring g_AccountPassword;
Wstring g_AccountPasswordCheck;
Wstring g_AccountParentEmail;
Wstring g_AccountExtra; // the account form's ninth string: no function of the program reads it

rc_ptr<PlayerProfile> g_PlayerProfile;
CDTimerClass<SystemTimerClass> g_PageTimer;

SimpleWonlineDialogControl::SimpleWonlineDialogControl(void)
{
}

SimpleWonlineDialogControl::~SimpleWonlineDialogControl(void)
{
}

bool SimpleWonlineDialogControl::Is_Cancelled(void)
{
	return(WOL_Wait_Callback());
}

#define OD_INSERTSTRING (WM_USER + 199)
#define OD_FINDSTRING (WM_USER + 197)
WideWstring WideWstring_FromInt(int value);
WideWstring operator+(wchar_t const * first, WideWstring const & second);

int Count_AI_Slots(void);
bool Is_Observer_Slot_Occupied(void);
bool Is_Observer_Slot_Full(void);
bool Is_Real_Player_Entry(NodeNameType * entry);
Wstring Lobby_Entry_Name(NodeNameType * entry);
void Load_Observer_Mode(void);
void Save_Observer_Mode(void);
void Update_Observer_Buttons(HWND dialog);
void Enable_Player_Combos(HWND dialog, bool value);
int Player_Side_Combo_ID(int player);
int Player_Color_Combo_ID(int player);
int Side_Combo_Data(HWND dialog, int id, int index);
void Fill_Observer_Combo(HWND dialog, int id);
void Fill_Random_Color_Combo(HWND dialog, int id);
void Select_Color_Combo(HWND dialog, int id, int color);
void Fill_Side_Combo(HWND dialog, int id);
void Fill_Color_Combo(HWND dialog, int id);
int Get_Slot_Control_Id(int slot);
char * Wide_To_Narrow(wchar_t const * text);
Wstring Wstring_FromWide(WideWstring const & text);
Wstring Wstring_Narrow(WideWstring const & text);
void Draw_Player_List(int keep_selection, bool force);
#define OD_INSERTSTRINGW (WM_USER + 204)
#define OD_SETTEXT (WM_USER + 180)
#define APP_REG_KEY "SOFTWARE\\Westwood\\Red Alert 2"
void __fastcall Refresh_Slot_Settings(HWND dialog, bool force);
void __fastcall Fill_Buddy_List(HWND dialog);
void Char_To_Wide(wchar_t * wide, char const * narrow, int length);
wchar_t * Narrow_To_Wide(char const * narrow);
int g_Col_Username;
int g_Col_Rank;
int g_Col_Chanop;
int g_Col_Gameinfo;
int g_Col_House;
int g_Col_Accept;
int g_Col_GamePing;
void Show_Wait_Window(unsigned int event, WideWstring const & text, bool block);
BOOL CALLBACK WOL_Find_Page_Dialog_Proc(HWND win, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool Wait_Callback_Thunk(void);
bool Is_Channel_Flag_8(Channel const * channel);

void Spaces_To_Underscores(char * text)
{
	for (char * space = strchr(text, ' '); space != NULL; space = strchr(space, ' ')) {
		*space = '_';
	}
}

bool Is_User_Message_Shown(User const * user)
{
	if (!user) return(false);
	if (user->flags & 1) return(true);
	if (WS_Find_Dialog(0xC2)) return(true);
	if (WS_Find_Dialog(0xC9)) return(true);
	return g_DisplayLatin != 0;
}

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

inline void Set_Match_State(int state, int timeout, int result)
{
	g_MatchResult = result;
	g_MatchState = state;
	g_MatchTimeout = timeout;
	g_MatchStartTime = time(0);
}

/// <summary>
/// Opens the Find Page dialog, waits for it to close, then restores the input-ignore flag
/// and forces the tactical map to fully redraw.
/// </summary>
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

void Open_Registration_Page(void)
{
	char url[256];
	sprintf(url, "http://games2.westwood.com/cgi-bin/cgiclient?register&request=reg_menu&LANGCODE=%d", g_LanguageCode);
	ViewHTML(url, 0);
}

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

bool Is_Channel_Tournament(Channel const * channel)
{
	if (Session.Type != GAME_INTERNET) return(false);
	return(channel->tournament || Is_Channel_Flag_8(channel));
}

bool Is_Current_Channel_Tournament(void)
{
	return(Is_Channel_Tournament(&g_CurrentChannel));
}

bool Is_Current_Channel_Flag_8(void)
{
	return(Is_Channel_Flag_8(&g_CurrentChannel));
}

bool Is_Channel_Flag_8(Channel const * channel)
{
	if (Session.Type != GAME_INTERNET) return(false);
	return((channel->reserved >> 8) & 1);
}

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

Wstring g_NickName;
Wstring g_LoginPassword;

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

/// <summary>
/// Writes the current WOL lobby settings (paging, finding, language filter, lobby music,
/// show-all) to the config INI and saves it to disk.
/// </summary>
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

/// <summary>
/// Applies the current WOL settings to the active chat session: pushes the language filter
/// and find/page flags to the chat client, and starts, continues, or fades out the lobby
/// music depending on the LobMusic setting.
/// </summary>
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

/// <summary>
/// Sends a chat message on the player's behalf.
/// The message goes privately to whoever is selected in the user list box, or out to the
/// whole channel when nothing is selected.
/// </summary>
/// <param name="msg">Text of the message to send.</param>
/// <returns>Returns with 0 if the message went to the whole channel, or 1 if it went
/// privately to the selected users.</returns>
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

/// <summary>
/// Sends a chat action on the player's behalf.
/// The action goes privately to whoever is selected in the user list box, or out to the
/// whole channel when nothing is selected.
/// </summary>
/// <param name="msg">Text of the action to send.</param>
/// <returns>Returns with 0 if the action went to the whole channel, or 1 if it went
/// privately to the selected users.</returns>
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

static inline bool Same_Player_Name(wchar_t const * first, wchar_t const * second)
{
	char * name = Wide_To_Narrow(first);
	return(stricmp(name, Wide_To_Narrow(second)) == 0);
}

/// <summary>
/// Encodes the current game options and scenario info, followed by each user's name,
/// house, and color, into a single comma-separated string for transmission over WOL.
/// </summary>
/// <param name="out">Buffer that receives the encoded options string.</param>
void Encode_Game_Options(char * out); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: 6 bytes differ (adopted text)
void Encode_Game_Options(char * out)
{
	static char useroptions[512];

	Wstring name = Session.Options.ScenarioDescription ? Wstring_FromWide(Session.Options.ScenarioDescription) : "";

	int max_players = Session.ScenarioMaxPlayers;

	/// Encode the game options and scenario info.
	sprintf(out, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%d,%s,%s:",
			Seed, Options.GameSpeed, Session.Options.GameSpeed, Session.Options.Credits, Session.Options.UnitCount,
			Session.Options.ShortGame, Session.Options.SWAllowed, Session.Options.BuildOffAlly, Session.Options.MCVRedeploy,
			Session.Options.Goodies, Session.Options.MPModeIndex, name.get(), max_players, Session.ScenarioIsOfficial,
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
int GetINIHash(void)
{
	return(g_RuleCRC ^ g_AICRC ^ g_ArtCRC);
}

/// <summary>
/// Encodes extended game/session info -- build number, data hash, options, and a flag
/// describing which house(s) still have room -- into a comma-separated string for
/// transmission over WOL.
/// </summary>
/// <param name="out">Buffer that receives the encoded info string.</param>
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

/// <summary>
/// Looks up whether the given user (or the local player if who is NULL) has accepted an
/// invitation/request.
/// </summary>
/// <param name="who">Name of the user to query, or NULL for the local player.</param>
/// <returns>The user's accepted status, or -1 if the user could not be found.</returns>
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

inline void Reset_Wait_Events(void)
{
	for (int i = 0; i < EV_COUNT; i++) {
		ResetEvent(g_WaitEventHandles[i]);
	}
}

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

inline void Close_Wait_Window(unsigned int event)
{
	HWND dialog = WS_Find_Dialog(0xC4);
	if (dialog) {
		g_ActiveWaitFlags &= ~event;
		if (g_ActiveWaitFlags == 0) {
			WS_Destroy_Dialog(dialog, 0);
		}
	}
	HWND top = WS_Top_Window();
	HWND input = GetDlgItem(top, 0x540);
	if (input) {
		SetFocus(input);
	}
}

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

/// <summary>
/// Nudges the host with a NET_PING packet while waiting in a live game channel as a guest.
/// Sets up the session's player/host address from the channel's first user, then sends a
/// single ping packet to the host address.
/// </summary>
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

/// <summary>
/// Shows the World Domination Tour "available servers" dialog and waits for it to close.
/// </summary>
/// <returns>Result code returned by the dialog (see WS_Wait_Dialog/WS_Destroy_Dialog).</returns>
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

inline bool Is_Tournament_Session_Inline(void)
{
	return(Online_Session_State == 2 || Online_Session_State == 1);
}

#ifdef RA2_NON_MATCHING // not exact: differs from the original at 0x0073E100 (main has no row for it, so no independent bound)
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
	g_MessageOfTheDay.clear();

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
	memset(g_OwnSquadName, 0, 41);
	g_OwnSquadID = 0;

	g_ChannelCount = 0;

	g_LadderPos = 1;

	g_JoinLobbyNow = 0;

	g_ChanListQueue.clear();

	WolOptionsLocked = 0;

	g_UserAge = 0;
	g_UserConsent = 0;

	g_RuleCRC = RulesClass::Get_Rule_Unique_ID();
	RulesClass::Load_Art_INI();
	g_ArtCRC = RulesClass::Get_Art_Unique_ID();
	g_AICRC = RulesClass::Get_AI_Unique_ID();

	ManglerIndex = 0;
}

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

#ifdef RA2_NON_MATCHING // exact form, count-sensitive
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


#ifdef RA2_NON_MATCHING // not exact: the chat event sinks are ATL classes (no ATL headers here)
class CChatEventSink : public CComObjectRoot, public IChatEvent
{
	public:
		CChatEventSink(void) {}
		BEGIN_COM_MAP(CChatEventSink)
		COM_INTERFACE_ENTRY(IChatEvent)
		END_COM_MAP()
		STDMETHOD(OnServerList)(HRESULT res, Server *servers);
		STDMETHOD(OnUpdateList)(HRESULT res, Update *updates);
		STDMETHOD(OnServerError)(HRESULT res, LPCSTR ircmsg);
		STDMETHOD(OnConnection)(HRESULT res, LPCSTR motd);
		STDMETHOD(OnMessageOfTheDay)(HRESULT res, LPCSTR motd);
		STDMETHOD(OnChannelList)(HRESULT res, Channel *channels);
		STDMETHOD(OnChannelCreate)(HRESULT res, Channel *channel);
		STDMETHOD(OnChannelJoin)(HRESULT res, Channel *channel, User *user);
		STDMETHOD(OnChannelLeave)(HRESULT res, Channel *channel, User *user);
		STDMETHOD(OnChannelTopic)(HRESULT res, Channel *channel, LPCSTR topic);
		STDMETHOD(OnPrivateAction)(HRESULT res, User *user, LPCSTR action);
		STDMETHOD(OnPublicAction)(HRESULT res, Channel *channel, User *user, LPCSTR action);
		STDMETHOD(OnUserList)(HRESULT res, Channel *channel, User *users);
		STDMETHOD(OnPublicMessage)(HRESULT res, Channel *channel, User *user, LPCSTR message);
		STDMETHOD(OnPrivateMessage)(HRESULT res, User *user, LPCSTR message);
		STDMETHOD(OnSystemMessage)(HRESULT res, LPCSTR message);
		STDMETHOD(OnNetStatus)(HRESULT res);
		STDMETHOD(OnLogout)(HRESULT status, User *user);
		STDMETHOD(OnPrivateGameOptions)(HRESULT res, User *user, LPCSTR options);
		STDMETHOD(OnPublicGameOptions)(HRESULT res, Channel *channel, User *user, LPCSTR options);
		STDMETHOD(OnGameStart)(HRESULT res, Channel *channel, User *users, int gameid);
		STDMETHOD(OnUserKick)(HRESULT res, Channel *channel, User *kicked, User *kicker);
		STDMETHOD(OnUserIP)(HRESULT res, User *user);
		STDMETHOD(OnFind)(HRESULT res, Channel *chan);
		STDMETHOD(OnPageSend)(HRESULT res);
		STDMETHOD(OnPaged)(HRESULT res, User *user, LPCSTR message);
		STDMETHOD(OnServerBannedYou)(HRESULT res, time_t bannedTill);
		STDMETHOD(OnUserFlags)(HRESULT res, LPCSTR name, unsigned int flags, unsigned int mask);
		STDMETHOD(OnChannelBan)(HRESULT res, LPCSTR name, int banned);
		STDMETHOD(OnSquadInfo)(HRESULT res, unsigned long id, Squad *squad);
		STDMETHOD(OnUserLocale)(HRESULT res, User *users);
		STDMETHOD(OnUserTeam)(HRESULT res, User *users);
		STDMETHOD(OnSetLocale)(HRESULT res, Locale newlocale);
		STDMETHOD(OnSetTeam)(HRESULT res, int newteam);
};

class CNetUtilEventSink : public CComObjectRoot, public INetUtilEvent
{
	public:
		CNetUtilEventSink(void) {}
		BEGIN_COM_MAP(CNetUtilEventSink)
		COM_INTERFACE_ENTRY(INetUtilEvent)
		END_COM_MAP()
		STDMETHOD(OnPing)(HRESULT res, int time, unsigned long ip, int handle);
		STDMETHOD(OnLadderList)(HRESULT res, Ladder *list, int totalCount, long timeStamp, int keyRung);
		STDMETHOD(OnGameresSent)(HRESULT res);
		STDMETHOD(OnNewNick)(HRESULT res, LPCSTR message, LPCSTR nick, LPCSTR pass);
		STDMETHOD(OnAgeCheck)(HRESULT res, int years, int consent);
		STDMETHOD(OnWDTState)(HRESULT res, unsigned char *state, int length);
};

class CDownloadEventSink : public CComObjectRoot, public IDownloadEvent
{
	public:
		CDownloadEventSink(void) {}
		BEGIN_COM_MAP(CDownloadEventSink)
		COM_INTERFACE_ENTRY(IDownloadEvent)
		END_COM_MAP()
		STDMETHOD(OnEnd)(void);
		STDMETHOD(OnError)(int error);
		STDMETHOD(OnProgressUpdate)(int bytesread, int totalsize, int timetaken, int timeleft);
		STDMETHOD(OnQueryResume)(void);
		STDMETHOD(OnStatusUpdate)(int status);
};

CComObject<CChatEventSink> * g_pChatSink;
CComObject<CNetUtilEventSink> * g_pNetUtilSink;
CComObject<CDownloadEventSink> * g_pDownloadSink;
#endif

void Redraw_Player_List(void)
{
	Draw_Player_List(0, false);
}


#ifdef RA2_NON_MATCHING // not exact: needs the ATL module and CComObject
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

namespace OwnerDraw { BOOL __stdcall Set_Paint_Disabled(HWND window, LPARAM lparam); }
void __fastcall Set_Player_Ping(HWND dialog, int player, int ping);
void __fastcall Enable_Start_Button(void);
void __fastcall Release_Slot_By_Name(char const * name);
int Join_Selected_Lobby(bool join);
int Pick_Lobby(bool require_room);
void __fastcall Store_User_Ping(char const * name, int ping);
int g_PlayerPings[8];

/// <summary>
/// Handles a user leaving the current channel. If the departing user is ourselves, resets all
/// local channel/session state and returns to the lobby (closing any open guest or game-options
/// dialogs); otherwise removes the given user from our local user list and updates the
/// game-options acceptance state before refreshing the user list display.
/// </summary>
/// <param name="user">The user who left the channel.</param>
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
		Close_Wait_Window(0x40);

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

bool g_WOLInitialized;
void __fastcall Free_Lobby_Entries(void);

static inline void Reset_Wait_Event_At(int index) { ResetEvent(g_WaitEventHandles[index]); }
static inline void Reset_All_Wait_Events(void) { for (int i = 0; i < EV_COUNT; i++) { Reset_Wait_Event_At(i); } }
/// <summary>
/// Logs the local player out of the WOL chat session. Sends a logout request to the Chat
/// interface and waits (with timeout retries) for it to complete or be aborted, then, for
/// World Domination Tour sessions, also closes the event handles and fully shuts down chat.
/// </summary>
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

#ifdef RA2_NON_MATCHING // not exact: needs the ATL module
/// <summary>
/// Tears down the WOL COM interfaces: unadvises each event sink, releases the Chat, NetUtil,
/// and Download interfaces, clears the global pointers, and terminates the ATL module.
/// </summary>
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

char g_WDTHost[128];
int g_WDTPort;

/// <summary>
/// Requests the World Domination Tour state.
/// This routine puts up no wait window of its own, which is what lets the wait callback
/// poll the tour state in the background while the player is doing something else. The
/// language code is refreshed from the installed product's SKU on the way through.
/// </summary>
/// <returns>Returns with true if a request was issued, or false if there is no NetUtil connection
/// to issue it on.</returns>
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
		g_pNetUtil->RequestWDTState(g_WDTHost, g_WDTPort, 3);
		return(true);
	}
	return(false);
}

void __fastcall Init_Lobby_Entries(void);
void __fastcall Clear_Slot_Lists(void);
void __fastcall Reset_Slots(void);

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

Wstring Server_Name_Without_Prefix(Server const * server); // RA2: 0x00745450, defined below

bool Connect_To_Server(Server * server); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: 604 bytes differ (adopted text)
BOOL CALLBACK Locale_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00751190, the country dialog (0xFE)

static inline int Ask_Locale(void) // RA2: the dialog (0xFE, Locale_Dialog_Proc 0x00751190) that asks for the country of the persona; the control is destroyed on each way out
{
	SimpleWonlineDialogControl control; // RA2: vtable 0x007AF6F4, constructed by 0x006010C0, destroyed by 0x0073C020
	int answer = OwnerDraw::Controlled_Dialog(0xFE, Locale_Dialog_Proc, &control); // RA2: as above
	if (answer < 0) { // RA2: cancelled
		return(0);
	}
	return(answer);
}

static inline char Get_Locale_Pointer(Wstring const & name, int ** value) { return g_UserLocales.getPointer(name, value); } // RA2: a wrapper layer (debt: its real name is not known); the executable calls Dictionary::getPointer 0x00767570 out of line here; an explicit specialization of getPointer (auto_inline off) does that too but also changes User_Tooltip 0x0073F250 and Draw_Player_List 0x0073FEE0, which expand it

bool Connect_To_Server(Server * server) // RA2: 0x007424C0, not in TibSun (its Login_WOL did this): logs in to the chat server with the persona and password, waits for the answer (30 seconds, 20 rounds), reads the profile of the persona, tells the ladder server about it and settles the locale of the persona; matches (2192 bytes) with the out-of-line derived destructor and the out-of-line Dictionary calls above
{
	strcpy((char *)server->login, g_NickName.get()); // RA2: the persona and the password go into the server entry
	strcpy((char *)server->password, g_LoginPassword.get()); // RA2: as above
	for (int i = 0; i < EV_COUNT; i++) { // RA2: all wait events are cleared first
		ResetEvent(g_WaitEventHandles[i]);
	}
	{
		WideWstring connecting(StringTable::LoadString("TXT_CONNECTING_SERVER", NULL, "D:\\Ra2\\wonline.cpp", 8397)); // RA2: the wait dialog takes a wide text
		Show_Wait_Window(0x10, connecting, false); // RA2: as above
	}
	g_pChat->RequestConnection(server, 20, ((int &)g_PlaintextPassword) & 0xFF); // RA2: virtual 0x14

	int attempts = 20; // RA2: rounds of the wait
	for (;;) { // RA2: as above
		int remaining = 30000; // RA2: 30 seconds in steps of 100 ms
		DWORD event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100); // RA2: as above
		while (event == WAIT_TIMEOUT) { // RA2: as above
			remaining -= 100; // RA2: as above
			if (remaining <= 0) { // RA2: as above
				goto connect_failed; // RA2: the executable shares one Close_Wait_Window block between this timeout and the event 18 test below (8 copies of it in the function); no goto-free form gave that (they give 10 copies and a different block order)
			}
			WOL_Wait_Callback(); // RA2: as above
			event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100); // RA2: as above
		}
		{ // RA2: the event that ended the wait is cleared again
			ResetEvent(g_WaitEventHandles[event]); // RA2: as above
			if (event == EV_ABORT) { // RA2: as above
				Close_Wait_Window(0xFFFFFFFF); // RA2: as above
				return(false);
			}
			if (event == EV_ERROR) { // RA2: as above
				ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 8410), 0, Wait_Callback_Thunk); // RA2: as above
				Close_Wait_Window(0xFFFFFFFF); // RA2: as above
				return(false);
			}
		}
		if (event == EV_COUNT) { // RA2: as above
connect_failed: // RA2: label of the shared block described at the goto above
			Close_Wait_Window(0xFFFFFFFF); // RA2: as above
			return(false);
		}
		if (attempts-- < 0) { // RA2: as above
			ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 8417), 0, Wait_Callback_Thunk); // RA2: as above
			Close_Wait_Window(0xFFFFFFFF); // RA2: as above
			return(false);
		}
		if (event == EV_CONNECTED) { // RA2: as above
			break;
		}
	}
	Close_Wait_Window(0xFFFFFFFF); // RA2: as above
	g_pChat->SetClientVersion(0x1000E); // RA2: virtual 0xD4
	g_IgnoreNetStatus = false; // RA2: as above
	Session.Read_MultiPlayer_Settings(); // RA2: as above

	WOLOptionsClass options; // RA2: the constructor of wolopt.h clears both members
	options.Load(); // RA2: then Load is called, as 0x004FF200 shows
	rc_ptr<PlayerProfile> profile; // RA2: as above
	if (options.Is_Bit1_Clear()) { // RA2: as above
		profile = PlayerProfile::Load(g_NickName); // RA2: the persona's own profile
	} else {
		profile = g_PlayerProfile.Get(); // RA2: the profile of Reset_WOL_Globals
	}
	if (Online_Session_State == 3 || Online_Session_State == 4) { // RA2: as above
		{
			Wstring name(Server_Name_Without_Prefix(server)); // RA2: as above
			profile->SetName(&name); // RA2: the server's name is kept in the profile
		}
		if (options.Is_Bit1_Clear()) { // RA2: as above
			profile->Save(&g_NickName); // RA2: as above
		}
	}
	strcpy(Session.Handle, g_NickName.get()); // RA2: as above
	Session.Set_Side(profile->Country, false); // RA2: as above
	Session.Set_Color(profile->Color, false); // RA2: as above
	Session.Write_MultiPlayer_Settings(); // RA2: as above
	WOL_Set_Last_Valid_Persona(g_NickName); // RA2: as above
	if (g_pNetUtil && strlen(g_LadderAddress)) { // RA2: as above
		g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, g_NickName.get(), g_GameSKU & 0xFFFF00, -1, 0, 0); // RA2: virtual 0x14
	}
	for (int j = 0; j < EV_COUNT; j++) { // RA2: as above
		ResetEvent(g_WaitEventHandles[j]);
	}
	User user;
	memset(&user, 0, sizeof(user)); // RA2: as above
	strcpy((char *)user.name, g_NickName.get()); // RA2: as above
	g_pChat->RequestUserLocale(&user); // RA2: virtual 0xBC
	for (;;) { // RA2: the second wait, for the locale
		int remaining = 30000; // RA2: as above
		DWORD event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100); // RA2: as above
		while (event == WAIT_TIMEOUT) { // RA2: as above
			remaining -= 100; // RA2: as above
			if (remaining <= 0) { // RA2: as above
				goto locale_wait_failed; // RA2: as the goto above, for the three blocks of this wait
			}
			WOL_Wait_Callback(); // RA2: as above
			event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100); // RA2: as above
		}
		{ // RA2: as above
			ResetEvent(g_WaitEventHandles[event]); // RA2: as above
			if (event == EV_ABORT) { // RA2: as above
				Close_Wait_Window(0xFFFFFFFF); // RA2: as above
				break;
			}
			if (event == EV_ERROR) { // RA2: as above
				ODMessageBox(StringTable::LoadString("TXT_WDT_NET_ERR", NULL, "D:\\Ra2\\wonline.cpp", 8501), 0, Wait_Callback_Thunk); // RA2: as above
				Close_Wait_Window(0xFFFFFFFF); // RA2: as above
				break;
			}
		}
		if (event == EV_COUNT) { // RA2: as above
locale_wait_failed: // RA2: label of the shared block of the second wait
			Close_Wait_Window(0xFFFFFFFF); // RA2: as above
			break;
		}
		if (event == EV_LOCALE) { // RA2: as above
			break;
		}
	}

	Wstring const & nick_source = g_NickName; // RA2: the const overload of toLower
	Wstring nick = nick_source.toLower(); // RA2: the locales are kept by lower case name
	if (g_UserLocales.contains(nick)) { // RA2: the server answered for the persona
		int locale = 0; // RA2: as above
		{
			int * value = NULL; // RA2: as above
			if (Get_Locale_Pointer(nick, &value) && value != NULL) { // RA2: as above
				locale = *value; // RA2: as above
			}
		}
		if (locale != 0) { // RA2: the server already knows a locale: make the profile agree with it and tell the server the profile's
			if (profile.Get() != NULL && profile->Locale != locale) { // RA2: as above
				if (profile->Locale == 0) { // RA2: the profile had none yet
					profile->SetLocale(locale, 2); // RA2: as above
					profile->Save(&g_NickName); // RA2: as above
				}
				int current = profile->Locale; // RA2: read once into a register (the executable uses esi)
				g_pChat->RequestSetLocale((Locale)current); // RA2: virtual 0xB8
				g_pChat->RequestUserLocale(&user); // RA2: virtual 0xBC
				locale = current; // RA2: the executable keeps the new locale in esi and stores it only here, for the update
				g_UserLocales.updateValue(nick, locale); // RA2: Dictionary::remove then ::add (calls 0x007675E0 and 0x00766070)
			}
			return(true);
		}
	}
	int chosen = 0; // RA2: the locale of the second path
	if (profile.Get() == NULL || (chosen = profile->Locale) == 0) { // RA2: no locale known anywhere: ask the player
		chosen = Ask_Locale(); // RA2: as above
		if (profile.Get() != NULL) { // RA2: as above
			profile->SetLocale(chosen, 3); // RA2: as above
			profile->Save(&g_NickName); // RA2: as above
		}
	}
	if (chosen != 0) { // RA2: as above
		g_pChat->RequestSetLocale((Locale)chosen); // RA2: as above
		g_pChat->RequestUserLocale(&user); // RA2: as above
		int stored = chosen; // RA2: Dictionary::updateValue takes a reference: the value goes through a variable of its own
		g_UserLocales.updateValue(nick, stored); // RA2: as above
	}
	return(true);
}
#endif


bool Connect_To_Selected_Server(void) // RA2: 0x00742D50, not in TibSun: connects to the server of the list the player picked, false when the index is not in the list
{
	if (g_CurrentServerIndex >= 1 && g_CurrentServerIndex < g_Servers.length()) { // RA2: as above
		Server * server = NULL; // RA2: as above
		g_Servers.getPointer(&server, g_CurrentServerIndex); // RA2: as above
		return(Connect_To_Server(server)); // RA2: a jump to Connect_To_Server
	}
	return(false); // RA2: as above
}

bool Recall_Stored_Login(Wstring const & name) // RA2: 0x00742D80, not in TibSun: looks the persona up among the 32 logins the chat API remembers; keeps its nickname and password when it is there, else clears the nickname
{
	g_LoginPassword.clear(); // RA2: as above
	for (int index = 1; index <= 32; index++) { // RA2: as above
		char const * nickname; // RA2: not cleared, the chat API sets both when it succeeds
		char const * password; // RA2: as above
		if (g_pChat->GetNick(index, &nickname, &password) == 0 && nickname && strlen(nickname) && !stricmp(name.text(), nickname)) { // RA2: as above
			g_NickName = nickname; // RA2: as above
			g_LoginPassword = password; // RA2: as above
			g_PlaintextPassword = false; // RA2: as above
			return(true); // RA2: as above
		}
	}
	g_NickName.clear(); // RA2: as above
	return(false); // RA2: as above
}

int Find_Server_Suffix(Wstring const & name) // RA2: 0x00742E30, not in TibSun: the index of the server whose name is "<prefix>:" followed by this name, -1 when there is none
{
	if (name.length() != 0) { // RA2: as above
		for (int i = 1; i < g_Servers.length(); i++) { // RA2: as above
			Server * server = NULL; // RA2: as above
			g_Servers.getPointer(&server, i); // RA2: as above
			char * separator = strchr((char *)server->name, ':'); // RA2: as above
			if (separator && *separator && name == (separator + 1)) { // RA2: as above
				return(i);
			}
		}
	}
	return(-1); // RA2: as above
}

static const HRESULT CHAT_E_BANNED = (HRESULT)0x80040072; // RA2: the answers of the chat server that have a support page; the page names say which is which (banned_, disabled_, serialbanned_, serialdup_, serialunknown_, skuserialmismatch_)
static const HRESULT CHAT_E_DISABLED = (HRESULT)0x80040074; // RA2: as above
static const HRESULT CHAT_E_SERIALBANNED = (HRESULT)0x80040075; // RA2: as above
static const HRESULT CHAT_E_SERIALDUP = (HRESULT)0x80040076; // RA2: as above
static const HRESULT CHAT_E_SERIALUNKNOWN = (HRESULT)0x80040077; // RA2: as above
static const HRESULT CHAT_E_SKUSERIALMISMATCH = (HRESULT)0x80040078; // RA2: as above

bool Handle_Connection_Error(void) // RA2: 0x00742EB0, not in TibSun: shows the error text of the failed connection; for the answers that have a support page it asks whether to open it and does so (4 is the yes/no kind). True when the answer was one of those
{
	bool handled = false; // RA2: as above
	char url[260]; // RA2: as above
	if (g_OnConnectionResult == CHAT_E_SERIALUNKNOWN || g_OnConnectionResult == CHAT_E_SKUSERIALMISMATCH || g_OnConnectionResult == CHAT_E_BANNED || g_OnConnectionResult == CHAT_E_DISABLED || g_OnConnectionResult == CHAT_E_SERIALBANNED || g_OnConnectionResult == CHAT_E_SERIALDUP) { // RA2: as above
		handled = true; // RA2: as above
		if (ODMessageBox(g_LastErrorMessage, 4, Wait_Callback_Thunk, false) == 6) { // RA2: IDYES
			int language = g_LanguageCode; // RA2: the support pages exist per language
			switch (g_OnConnectionResult) { // RA2: as above
			case CHAT_E_SERIALUNKNOWN: sprintf(url, "http://apiregister.westwood.com/support/serialunknown_%d.html", language); break; // RA2: as above
			case CHAT_E_SKUSERIALMISMATCH: sprintf(url, "http://apiregister.westwood.com/support/skuserialmismatch_%d.html", language); break; // RA2: as above
			case CHAT_E_BANNED: sprintf(url, "http://apiregister.westwood.com/support/banned_%d.html", language); break; // RA2: as above
			case CHAT_E_DISABLED: sprintf(url, "http://apiregister.westwood.com/support/disabled_%d.html", language); break; // RA2: as above
			case CHAT_E_SERIALBANNED: sprintf(url, "http://apiregister.westwood.com/support/serialbanned_%d.html", language); break; // RA2: as above
			case CHAT_E_SERIALDUP: sprintf(url, "http://apiregister.westwood.com/support/serialdup_%d.html", language); break; // RA2: as above
			}
			ViewHTML(url, 1); // RA2: as above
		}
	} else {
		ODMessageBox(g_LastErrorMessage, 0, Wait_Callback_Thunk, false); // RA2: as above
	}
	return(handled); // RA2: as above
}

int __fastcall Request_And_Wait_Server_List(unsigned int flags); // RA2: 0x00751C30, asks for the list of chat servers and waits for it (0 done, 1 failed, 2 quit), not yet defined
LRESULT CALLBACK Wonline_Account_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007540A0, the dialog proc of the login dialog (0x10F, 0x11D)
LRESULT CALLBACK Wonline_Autologin_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00755070, the dialog proc of the dialog (0x112) after a login
int Save_Nickname(char const * name, char const * password, bool mangle, int locale); // RA2: 0x00745320, defined below
void Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, defined in init.cpp

static inline void Set_Login_Focus(HWND window) // RA2: the login dialog puts the focus on the password box (0x72C) when a nickname is known, else on the name box (0x628); the executable has both SetFocus calls with a merged tail
{
	if (g_NickName.length() > 0 && g_LoginPassword.length() == 0) { // RA2: as above
		SetFocus(GetDlgItem(window, 0x72C)); // RA2: as above
	} else {
		SetFocus(GetDlgItem(window, 0x628)); // RA2: as above
	}
}

static inline int Fetch_Server_List(void) // RA2: shows "fetching the server list" and requests the list for the product (0x2300 in a tournament session, else 0x2100)
{
	{
		WideWstring fetching(StringTable::LoadString("TXT_FETCHING_SERVLIST", NULL, "D:\\Ra2\\wonline.cpp", 22220)); // RA2: as above
		Show_Wait_Window(16, fetching, false); // RA2: as above
	}
	int sku = 0x2300; // RA2: as above
	if (Online_Session_State != 2) { // RA2: as above
		sku = 0x2100; // RA2: as above
	}
	return(Request_And_Wait_Server_List(sku)); // RA2: as above
}

int __fastcall Login_Wonline(void) // RA2: 0x00742FD0, not in TibSun: logs in to Westwood Online: starts the chat services, fetches the server list, tries the stored login of the auto login persona, else opens the login dialog (the executable's debug text says "Creating login dlg") and connects to the server the persona's profile names; 1 when connected, 0 when not, negative when the server list could not be fetched; NOT matching (unplaced, 1904 bytes against the executable's 1857): the executable calls ArrayList::getPointer 0x00765E60 out of line in the three early expansions of Find_Server_Suffix / Connect_To_Selected_Server (the last two expansions are inline) which our build expands everywhere (about 16 bytes each; wrapper layers around the early calls make the expansions themselves vanish, so the inline budget is not what is wrong); it keeps the zero in ebp and the rc_ptr vtable in ebx where we use ebx; and it expands the derived control destructor (see SimpleWonlineDialogControl)
{
	bool failed = false; // RA2: as above
	g_OnConnectionResult = 0; // RA2: as above
	int result = Init_Wonline(); // RA2: as above
	if (result != 1) { // RA2: as above
		return(result); // RA2: as above
	}
	{ // RA2: the lists of the last session are cleared before the login
	memset(&g_CurrentChannel, 0, sizeof(g_CurrentChannel)); // RA2: as above
	memset(&g_PendingChannel, 0, sizeof(g_PendingChannel)); // RA2: as above
	g_UserChannels.clear(); // RA2: as above
	g_GameChannelList.clear(); // RA2: as above
	g_UserList.clear(); // RA2: as above
	g_ChannelCount = 0; // RA2: as above
	result = 0; // RA2: as above
	int error = Fetch_Server_List(); // RA2: as above
	switch (error) { // RA2: as above
	case 0: // RA2: as above
		break; // RA2: as above
	case 2: // RA2: as above
		failed = true; // RA2: as above
		result = -1; // RA2: as above
		break; // RA2: as above
	default: // RA2: as above
		failed = true; // RA2: as above
		result = -2; // RA2: as above
		break; // RA2: as above
	}
	WOLOptionsClass options; // RA2: as above
	options.Load(); // RA2: as above
	if (!failed) { // RA2: as above
		if (options.Is_Bit0_Clear()) { // RA2: the auto login is on
			Wstring persona = WOL_Auto_Login_Persona(); // RA2: as above
			if (persona.length() > 0) { // RA2: as above
				if (Recall_Stored_Login(persona)) { // RA2: expanded here
					int index = -1; // RA2: as above
					if (Is_Tournament_Session_Inline()) { // RA2: an Internet game
						rc_ptr<PlayerProfile> profile = PlayerProfile::Load(persona); // RA2: as above
						if (profile.Get()) { // RA2: the server of the persona's profile (Find_Server_Suffix tests the empty name itself)
							index = Find_Server_Suffix(profile->Name); // RA2: expanded here
						}
						if (index == -1) { // RA2: as above
							index = g_CurrentServerIndex; // RA2: as above
						}
					} else {
						rc_ptr<PlayerProfile> profile = PlayerProfile::Load(persona); // RA2: as above
						if (profile.Get()) { // RA2: as above
							if (profile->Name.length() == 0) { // RA2: as above
								index = g_CurrentServerIndex; // RA2: as above
							} else {
								index = Find_Server_Suffix(profile->Name); // RA2: expanded here
							}
						}
					}
					if (index != -1) { // RA2: as above
						g_CurrentServerIndex = index; // RA2: as above
						if (Connect_To_Selected_Server()) { // RA2: expanded here
							return(1); // RA2: as above
						}
						failed = Handle_Connection_Error(); // RA2: as above
					}
				}
			}
		}
		if (g_NickName.length() == 0 && options.Is_Bit1_Clear()) { // RA2: as above
			g_NickName = WOL_Last_Valid_Persona(); // RA2: as above
		}
		{
			Wstring empty(""); // RA2: as above
			WOL_Set_Auto_Login_Persona(empty); // RA2: the auto login is used once
		}
		if (failed) { // RA2: as above
			return(0); // RA2: as above
		}
		DebugString("Creating login dlg\n"); // RA2: as above
		SimpleWonlineDialogControl control; // RA2: as above
		int dialog_id = options.Is_Bit1_Clear() ? 0x10F : 0x11D; // RA2: as above
		Darken_Surface(NULL, true); // RA2: as above
		HWND window = OwnerDraw::Begin_Dialog(dialog_id, (DLGPROC)Wonline_Account_Dialog_Proc, (LPARAM)&control); // RA2: as above
		OwnerDraw::Display_Dialog(window); // RA2: as above
		Set_Login_Focus(window); // RA2: as above
		while (!control.Is_Done() && !WOL_Wait_Callback() && !OwnerDraw::Dialog_Message_Handler()) { // RA2: as above
			Title_Screen_Restore(false); // RA2: as above
		}
		OwnerDraw::End_Dialog(window); // RA2: as above
		if ((int)control.Get_Window() == 2) { // RA2: the dialog was cancelled
			failed = true; // RA2: as above
		} else {
			WOL_Wait_Callback(); // RA2: as above
			g_IgnoreNetStatus = false; // RA2: as above
			WOLOptionsClass saved; // RA2: as above
			saved.Load(); // RA2: as above
			rc_ptr<PlayerProfile> profile; // RA2: as above
			if (saved.Is_Bit1_Clear()) { // RA2: as above
				profile = PlayerProfile::Load(g_NickName); // RA2: as above
			} else {
				profile = g_PlayerProfile.Get(); // RA2: as above
			}
			int found = Find_Server_Suffix(profile->Name); // RA2: expanded here
			if (found != -1) { // RA2: as above
				g_CurrentServerIndex = found; // RA2: as above
			}
			if (g_CurrentServerIndex >= g_Servers.length()) { // RA2: as above
				g_CurrentServerIndex = -1; // RA2: as above
			}
			if (g_CurrentServerIndex == -1 && g_Servers.length() > 1) { // RA2: as above
				g_CurrentServerIndex = 1; // RA2: as above
			}
			if (Connect_To_Selected_Server()) { // RA2: expanded here
				if (g_RememberLogin && saved.Is_Bit1_Clear()) { // RA2: the login is kept for the next time
					int locale = profile->Locale; // RA2: read before the nickname is saved (0x00742FD0)
					Save_Nickname(g_NickName.get(), g_LoginPassword.get(), g_PlaintextPassword, locale); // RA2: as above
					Session.Write_MultiPlayer_Settings(); // RA2: as above
				}
				if (!saved.Is_Bit0_Clear() || !WOL_Get_Auto_Login()) { // RA2: no auto-login question unless the profile allows it and auto-login is on
					return(1); // RA2: as above
				}
				Darken_Surface(NULL, true); // RA2: as above
				SimpleWonlineDialogControl next; // RA2: as above
				OwnerDraw::Controlled_Dialog(0x112, (DLGPROC)Wonline_Autologin_Dialog_Proc, &next); // RA2: as above
				return(1); // RA2: as above
			}
		}
	}
	Close_Wait_Window(0xFFFFFFFF); // RA2: expanded here
	if (!failed) { // RA2: as above
		Handle_Connection_Error(); // RA2: as above
	}
	g_IgnoreNetStatus = true; // RA2: as above
	} // RA2: as above
	return(result); // RA2: as above
}

// RA2: declarations for the functions below (placed after Draw_Player_List: declarations above it change its code)
rc_ptr<WDTState> const & __fastcall Current_WDT_State(void); // RA2: 0x00769510, defined in worlddom.cpp

// RA2: declarations for Run_Lobby_Session (before Pick_Lobby: the number of declarations in front of it decides how Pick_Lobby is compiled, see allmatch)
void Initialize_WDT_Sounds(void); // RA2: 0x00752210, fills the lists of the tour's sounds, once
void Discard_WDT_Sound_List(ArrayList<Wstring> sounds); // RA2: 0x00752890, takes a list of sounds by value and does nothing with it
int Join_Selected_Lobby(bool join); // RA2: 0x00744930, defined below
int CALLBACK Wonline_Main_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007469C0, the dialog proc of the lobby (0x113)
void Set_Wait_Window_Text(LPARAM text); // RA2: 0x0074D750, defined below
bool Start_Network_Session(void); // RA2: 0x00514EE0, defined in init.cpp
inline Wstring Narrow_Text(wchar_t const * text) // RA2: the executable narrows the text through a named wide string that dies before the result is used (0x00743720, 0x00743A40 below)
{
	WideWstring wide(text); // RA2: as above
	return Wstring_Narrow(wide); // RA2: as above
}
inline WideWstring Wide_Text(char const * text) // RA2: the counterpart: a named narrow string that dies after the widening
{
	Wstring narrow(text); // RA2: as above
	return Wstring_ToWide(narrow); // RA2: as above
}

inline void Print_Message_Of_The_Day(void) // RA2: the message of the day
{
	if (g_MessageOfTheDay.length() > 0) { // RA2: the message of the day, printed line by line
		char * text = new char[g_MessageOfTheDay.length() + 1]; // RA2: as above
		if (text) { // RA2: as above
			strcpy(text, g_MessageOfTheDay.get()); // RA2: as above
			char * token = strtok(text, "\n"); // RA2: as above
			while (token != NULL) { // RA2: as above
				WideWstring wide_line = Wide_Text(token); // RA2: as above
				Lobby_Chat_Printf(ColorSystem, wide_line.get()); // RA2: as above
				token = strtok(NULL, "\n"); // RA2: the next token is taken before the wide line is destroyed
			}
			Lobby_Chat_Printf(ColorSystem, L"\n"); // RA2: as above
			g_MessageOfTheDay.clear(); // RA2: as above
			delete text; // RA2: as above
		}
	}
}

#ifdef RA2_NON_MATCHING // not exact: 2210 bytes differ (adopted text)
int __fastcall Run_Lobby_Session(HWND window, bool first_time) // RA2: 0x00743720, not in TibSun: the lobby session of Westwood Online: ends the finished game (the tour's victory and defeat sounds), logs in again when the connection was lost, opens the lobby dialog (0x113) with the message of the day, requests the channel list, joins a lobby and waits for the events of the chat: 1 when a game starts, 2 when the skirmish event ended it, 0 otherwise (the dialog, the chat and the wait dialogs are closed again)
{
	int result = 0; // RA2: as above
	if (g_PlayingNetGame && Session.Type == GAME_INTERNET && Online_Session_State == 2 && Session.Winner != -1 && Session.Winner < 8) { // RA2: a tour game just ended
		Wstring winner = Narrow_Text(Session.Score[Session.Winner].Name); // RA2: the name of the winner
		if (_strnicmp(winner.get(), Session.Handle, 20) == 0) { // RA2: the local player won
			if (Session.House == 0) { // RA2: as above
				Discard_WDT_Sound_List(g_WDT_GDIWinEmphasisSounds); // RA2: as above
			} else if (Session.House == 1) { // RA2: as above
				Discard_WDT_Sound_List(g_WDT_NodWinEmphasisSounds); // RA2: as above
			}
		} else if (Session.House == 0) { // RA2: the local player lost
			Discard_WDT_Sound_List(g_WDT_GDILoseEmphasisSounds); // RA2: as above
		}
	}
	g_PlayingNetGame = false; // RA2: as above
	WolOptionsLocked = 0; // RA2: as above
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: a tour game
		g_pChat->RequestSetTeam(Session.House); // RA2: virtual 0xB4
		if (!g_WDTSoundsInitialized) { // RA2: as above
			Initialize_WDT_Sounds(); // RA2: as above
		}
	}
	Start_Network_Session(); // RA2: as above
	if (g_ConnectionLost) { // RA2: as above
		int login = Login_Wonline(); // RA2: as above
		if (login != 1) { // RA2: as above
			return(login); // RA2: as above
		}
	}
	for (int i = 0; i < EV_COUNT; i++) { // RA2: as above
		ResetEvent(g_WaitEventHandles[i]); // RA2: as above
	}
	if (window == NULL) { // RA2: as above
		Darken_Surface(NULL, true); // RA2: as above
		window = OwnerDraw::Begin_Dialog(0x113, (DLGPROC)Wonline_Main_Dialog_Proc, 0); // RA2: the lobby dialog
		Print_Message_Of_The_Day(); // RA2: as above
		{
			WideWstring title(StringTable::LoadString("GUI:LoginComplete", NULL, "D:\\Ra2\\wonline.cpp", 9075)); // RA2: the wait text is wide
			Show_Wait_Window(0x10, title, false); // RA2: as above
		}
	}
	g_SquadInfoRequests++; // RA2: as above
	g_RequestingOwnSquadInfo = 1; // RA2: as above
	g_pChat->RequestSquadInfo(0); // RA2: virtual 0xB0
	g_ServerType = 0x21; // RA2: as above
	Set_Wait_Window_Text((LPARAM)StringTable::LoadString("TXT_REQ_CHANLIST", NULL, "D:\\Ra2\\wonline.cpp", 9092)); // RA2: expanded here
	if (!Request_Chat_Channel_List(true)) { // RA2: expanded here
		goto cleanup; // RA2: as above
	}
	Set_Wait_Window_Text((LPARAM)StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 9099)); // RA2: as above
	{
		int joined = Join_Selected_Lobby(g_HasJoinedLobby && !first_time); // RA2: as above
		g_HasJoinedLobby = true; // RA2: as above
		if (joined == -1) { // RA2: as above
			goto cleanup; // RA2: as above
		}
	}
	{
		DWORD event; // RA2: as above
		while ((event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100)) == WAIT_TIMEOUT) { // RA2: as above
			WOL_Wait_Callback(); // RA2: as above
		}
		ResetEvent(g_WaitEventHandles[event]); // RA2: as above
		if (event == EV_ABORT || event == EV_ERROR) { // RA2: as above
			goto logout; // RA2: as above
		}
	}
	g_JoinLobbyNow = 0; // RA2: as above
	PostMessage(window, 0x593, 0, 0); // RA2: as above
	Close_Wait_Window(0xFFFFFFFF); // RA2: expanded here
	SetFocus(MainWindow); // RA2: as above
	ShowWindow(window, SW_SHOWNORMAL); // RA2: as above
	SetFocus(window); // RA2: as above
	SetFocus(GetDlgItem(window, 0x540)); // RA2: as above
	g_LastGameListTime = 0; // RA2: as above
	g_LastChatListTime = time(0); // RA2: as above
	for (;;) { // RA2: as above
		DWORD event; // RA2: as above
		while ((event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 20)) == WAIT_TIMEOUT) { // RA2: as above
			WOL_Wait_Callback(); // RA2: as above
			if (g_JoinLobbyNow) { // RA2: the lobby is to be left and another one joined
				EnableWindow(GetDlgItem(window, 0x53D), FALSE); // RA2: as above
				EnableWindow(GetDlgItem(window, 0x6EE), FALSE); // RA2: as above
				EnableWindow(GetDlgItem(window, 0x62D), FALSE); // RA2: as above
				EnableWindow(GetDlgItem(window, 0x62B), FALSE); // RA2: as above
				EnableWindow(GetDlgItem(window, 0x689), FALSE); // RA2: as above
				EnableWindow(GetDlgItem(window, 0x558), FALSE); // RA2: as above
				EnableWindow(GetDlgItem(window, 0x61F), FALSE); // RA2: as above
				EnableWindow(GetDlgItem(window, 0x686), FALSE); // RA2: as above
				if (Join_Selected_Lobby(false) == -1) { // RA2: as above
					EnableWindow(GetDlgItem(window, 0x686), TRUE); // RA2: as above
					SetEvent(g_WaitEventHandles[EV_ERROR]); // RA2: as above
				} else {
					PostMessage(window, 0x593, 0, 0); // RA2: as above
				}
				g_JoinLobbyNow = 0; // RA2: as above
			}
		}
		ResetEvent(g_WaitEventHandles[event]); // RA2: as above
		if (event == EV_ABORT) { // RA2: as above
			goto cleanup; // RA2: as above
		}
		if (event != EV_EXIT) { // RA2: as above
			if (event == EV_SKIRMISH) { // RA2: as above
				result = 2; // RA2: as above
				break; // RA2: as above
			}
			if (event == EV_ERROR) { // RA2: as above
				goto cleanup; // RA2: as above
			}
			if (event == EV_STARTGAME) { // RA2: the game starts
				goto started; // RA2: as above
			}
		} else {
			break; // RA2: as above
		}
	}
	g_LastErrorMessage[0] = 0; // RA2: as above
logout: // RA2: as above
	WS_Destroy_Dialog(window, 0); // RA2: as above
	WOL_Wait_Callback(); // RA2: as above
	window = NULL; // RA2: as above
	if (g_pChat) { // RA2: as above
		g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
	}
cleanup: // RA2: as above
	Close_Wait_Window(0xFFFFFFFF); // RA2: expanded here
	ODMessageBox(g_LastErrorMessage, 0, Wait_Callback_Thunk, false); // RA2: as above
	if (window) { // RA2: as above
		WS_Destroy_Dialog(window, 0); // RA2: as above
	}
	while (WS_Destroy_Dialog(NULL, 0)) { // RA2: as above
	}
	if (result == 1) { // RA2: as above
		g_PlayingNetGame = true; // RA2: as above
	}
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: as above
		g_WDTTeams.clear(); // RA2: as above
	}
	return(result); // RA2: as above
started: // RA2: as above
	result = 1; // RA2: as above
	g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
	g_GameStartRequested = false; // RA2: as above
	g_LastErrorMessage[0] = 0; // RA2: as above
	goto cleanup; // RA2: as above
}
#endif


#ifdef RA2_NON_MATCHING // not exact: 19 bytes differ (adopted text)
int Pick_Lobby(bool require_room) // RA2: 0x00744090, not in TibSun: chooses the lobby channel to join. In a World Domination Tour game it is the one that carries the player's territory; else the lobby that was joined last, else the lobby with the fewest players that still has room; the index in the channel list, -1 when there is none
{
	int count = g_UserChannels.length(); // RA2: as above
	Channel channel; // RA2: as above
	memset(&channel, 0, sizeof(channel)); // RA2: as above
	char section[32]; // RA2: the INI sections of the territory
	char entry[32]; // RA2: as above
	char fallback[32]; // RA2: as above
	char name[32]; // RA2: as above
	wchar_t territory[32]; // RA2: the name of the territory, wide
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: a World Domination Tour game
		memset(section, 0, sizeof(section)); // RA2: as above
		memset(territory, 0, 32); // RA2: only the first 32 bytes
		memset(entry, 0, sizeof(entry)); // RA2: as above
		memset(fallback, 0, sizeof(fallback)); // RA2: as above
		MixFileClass * archive = new MixFileClass("WDT.MIX", &FastKey); // RA2: the tour's archive is open while the INI is read
		{
			INIClass ini; // RA2: as above
			CCFileClass file("WDTMAP.INI"); // RA2: as above
			ini.Load(file, false); // RA2: as above
			sprintf(entry, "Territory%02d", Session.TournamentTerritory); // RA2: as above
			strcpy(fallback, entry); // RA2: as above
			if (Current_WDT_State()->MapID == 0) { // RA2: the map decides which of the two sections names the territory
				ini.Get_String("NorthAmerica", entry, fallback, section, sizeof(section)); // RA2: as above
			} else {
				ini.Get_String("Europe", entry, fallback, section, sizeof(section)); // RA2: as above
			}
			ini.Get_String(section, "Name", "A territory", name, sizeof(name)); // RA2: as above
			Char_To_Wide(territory, name, -1); // RA2: as above
			g_TerritoryName = _wcsdup(territory); // RA2: as above
			delete archive; // RA2: as above
		}
	}
	Wstring lobby_name = Wstring("Lob_33_") + Wstring_FromInt(g_LobbyNumber, 1); // RA2: the lobby that was joined last
	int i; // RA2: as above
	for (i = 0; i < count; i++) { // RA2: the territory's lobby or the one joined last
		g_UserChannels.get(channel, i); // RA2: as above
		if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: as above
			if (wcscmp((wchar_t const *)g_Lobbies[i], territory) != 0) { // RA2: as above
				continue; // RA2: as above
			}
			if (!require_room || (int)channel.currentUsers <= 25) { // RA2: 25 players per lobby
				return(i); // RA2: as above
			}
		}
		Wstring channel_name((char const *)channel.name); // RA2: as above
		if (channel_name == lobby_name) { // RA2: as above
			if (!require_room || (int)channel.currentUsers <= 25) { // RA2: as above
				return(i); // RA2: as above
			}
		}
	}
	int best = -1; // RA2: the lobby with the fewest players
	unsigned int smallest = 10000; // RA2: as above
	for (i = 0; i < count; i++) { // RA2: as above
		g_UserChannels.get(channel, i); // RA2: as above
		if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: as above
			if (wcsncmp((wchar_t const *)g_Lobbies[i], territory, wcslen(territory)) != 0) { // RA2: as above
				continue; // RA2: as above
			}
			if (!require_room || (int)channel.currentUsers <= 25) { // RA2: as above
				g_LobbyNumber = atol((char const *)channel.name + strlen("Lob_33_")); // RA2: the number is remembered
				return(i); // RA2: as above
			}
			if (channel.currentUsers < smallest) { // RA2: as above
				best = i; // RA2: as above
				smallest = channel.currentUsers; // RA2: as above
			}
		}
		if (strncmp((char const *)channel.name, "Lob_33_", strlen("Lob_33_")) == 0) { // RA2: as above
			if (!require_room || (int)channel.currentUsers <= 25) { // RA2: as above
				g_LobbyNumber = atol((char const *)channel.name + strlen("Lob_33_")); // RA2: as above
				return(i); // RA2: as above
			}
			if (channel.currentUsers < smallest) { // RA2: as above
				best = i; // RA2: as above
				smallest = channel.currentUsers; // RA2: as above
			}
		}
	}
	if (best != -1) { // RA2: as above
		g_UserChannels.get(channel, best); // RA2: as above
		g_LobbyNumber = atol((char const *)channel.name + strlen("Lob_33_")); // RA2: as above
	}
	return(best); // RA2: as above
}
#endif

bool Leave_Current_Channel(unsigned int event, bool block) // RA2: 0x00744610, not in TibSun: leaves the channel the chat is in, waits up to 15 seconds for the server to confirm; true when no channel is left
{
	if (g_ChannelCount) { // RA2: as above
		DebugString("Leave_Channel, channel %s type %d\n", g_CurrentChannel.name, g_CurrentChannel.type); // RA2: as above
		g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
		{
			WideWstring title(StringTable::LoadString("WOL:LeavingChannel", NULL, "D:\\Ra2\\wonline.cpp", 9379)); // RA2: the wait dialog takes a wide text
			Show_Wait_Window(event, title, block); // RA2: as above
		}
		for (int i = 0; g_ChannelCount && i < 150; i++) { // RA2: 150 steps of 100 ms
			Sleep(100); // RA2: as above
			WOL_Wait_Callback(); // RA2: as above
		}
		Close_Wait_Window(0x40); // RA2: expanded here
	}
	return(g_ChannelCount == 0); // RA2: as above
}

int Request_Channel_Join(Channel * channel, Wstring const & password, WideWstring title) // RA2: 0x00744700, not in TibSun: joins a channel (leaves the current one first); title is the text of the wait dialog; the chat's answer, 1 when the old channel could not be left
{
	char const * name = (char const *)channel->name; // RA2: as above
	if (strcmp(name, (char const *)g_CurrentChannel.name) && strcmp(name, (char const *)g_PendingChannel.name)) { // RA2: not the channel the chat is in nor the one it was asked to join
		if (g_ChannelCount) { // RA2: as Leave_Current_Channel
			DebugString("Leave_Channel, channel %s type %d\n", g_CurrentChannel.name, g_CurrentChannel.type); // RA2: as above
			g_pChat->RequestChannelLeave(); // RA2: as above
			{
				WideWstring leaving(StringTable::LoadString("WOL:LeavingChannel", NULL, "D:\\Ra2\\wonline.cpp", 9379)); // RA2: as above
				Show_Wait_Window(0x40, leaving, true); // RA2: as above
			}
			for (int i = 0; g_ChannelCount && i < 150; i++) { // RA2: as above
				Sleep(100); // RA2: as above
				WOL_Wait_Callback(); // RA2: as above
			}
			Close_Wait_Window(0x40); // RA2: expanded here
			if (g_ChannelCount) { // RA2: the channel was not left
				return(1); // RA2: as above
			}
		}
		strcpy((char *)channel->key, password.text()); // RA2: as above
		g_PendingChannel = *channel; // RA2: as above
		{
			WideWstring message(title.text()); // RA2: as above
			Show_Wait_Window(2, message, false); // RA2: as above
		}
		DebugString("Requesting channel join %s\n", name); // RA2: as above
		return g_pChat->RequestChannelJoin(channel); // RA2: virtual 0x20
	}
	return(0); // RA2: as above
}

int Join_Selected_Lobby(bool join) // RA2: 0x00744930, not in TibSun: joins the lobby channel Pick_Lobby chose (the argument is its "needs room" flag); the lobby's index, -1 when none was picked or the join was refused
{
	int choice = Pick_Lobby(join); // RA2: as above
	if (choice != -1) { // RA2: as above
		Channel channel; // RA2: as above
		g_UserChannels.get(channel, choice); // RA2: as above
		int result; // RA2: as above
		{
			Wstring password("zotclot9"); // RA2: the password of the lobby channels
			result = Request_Channel_Join(&channel, password, WideWstring(StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 9428))); // RA2: the wait text is passed by value
		}
		if (!result) { // RA2: as above
			HWND dialog = WS_Find_Dialog(0x113); // RA2: the game list dialog gets the focus
			if (dialog) { // RA2: as above
				SetFocus(GetDlgItem(dialog, 1344)); // RA2: as above
			}
			Refresh_Game_List(); // RA2: as above
			return(choice); // RA2: as above
		}
	}
	Refresh_Game_List(); // RA2: as above
	return(-1); // RA2: as above
}

namespace OwnerDraw { void __fastcall Slide_Dialog(HWND dialog, bool reverse); } // RA2: 0x005E5150, defined in ownrdraw.cpp
LRESULT CALLBACK Birthdate_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007494D0, the dialog proc of the birthdate dialog (0xE7)
LRESULT CALLBACK Account_Details_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007498E0, the dialog proc of the account dialogs (0xE6, 0xF3, 0xF4)
void __fastcall Show_Dialog(HWND dialog, int id); // RA2: 0x005FF7E0, defined in ownrdraw.cpp

void __fastcall Create_Wonline_Account(HWND parent) // RA2: 0x00744A00, not in TibSun: asks the birthdate, checks the age with the server, asks the nickname, password and emails and requests the new nickname
{
	OwnerDraw::Slide_Dialog(parent, false); // RA2: as above
	Darken_Surface(NULL, true); // RA2: as above
	char nickname[32]; // RA2: as above
	char password[32]; // RA2: as above
	char confirmation[32]; // RA2: as above
	char email[128]; // RA2: as above
	char parent_email[128]; // RA2: as above
	int newsletter = 1; // RA2: as above
	int month = 1; // RA2: as above
	int day = 0; // RA2: as above
	int year = 0; // RA2: as above
	int share_info; // RA2: as above
	HWND dialog; // RA2: as above
	for (int index = 0; index < 18; ++index) ResetEvent(g_WaitEventHandles[index]); // RA2: as above
	memset(nickname, 0, sizeof(nickname)); // RA2: as above
	memset(password, 0, sizeof(password)); // RA2: as above
	memset(confirmation, 0, sizeof(confirmation)); // RA2: as above
	memset(email, 0, sizeof(email)); // RA2: as above
	memset(parent_email, 0, sizeof(parent_email)); // RA2: as above
	{
		WideWstring message(StringTable::LoadString("WOL:NewAccountFetch", NULL, "D:\\Ra2\\wonline.cpp", 9501)); // RA2: the wait text of the server list request
		Show_Wait_Window(0x80, message, false); // RA2: as above
	}
	DebugString("Newnick update check\n"); // RA2: as above
	g_pChat->RequestServerList(g_GameSKU, 0x10006, "TibSun", "TibPass99", 35); // RA2: the login of the account server list request
	DWORD event; // RA2: as above
	do { // RA2: as above
		event = WaitForMultipleObjects(18, g_WaitEventHandles, false, 100); // RA2: as above
		while (event == WAIT_TIMEOUT) { // RA2: as above
			WOL_Wait_Callback(); // RA2: as above
			event = WaitForMultipleObjects(18, g_WaitEventHandles, false, 100); // RA2: as above
		}
		ResetEvent(g_WaitEventHandles[event]); // RA2: as above
	} while (event != 0 && event != 1); // RA2: as above
	Close_Wait_Window(0xFFFFFFFF); // RA2: as above
	if (event == 0) { // RA2: the server list request failed
		ShowWindow(parent, SW_SHOW); // RA2: as above
		OwnerDraw::Set_Paint_Disabled(parent, 0); // RA2: as above
		return; // RA2: as above
	}
	if (event == 8) { // RA2: the game was left meanwhile, the event stays set
		SetEvent(g_WaitEventHandles[8]); // RA2: as above
		ShowWindow(parent, SW_SHOW); // RA2: as above
		OwnerDraw::Set_Paint_Disabled(parent, 0); // RA2: as above
		return; // RA2: as above
	}
birthdate_retry: // RA2: the retry is written as a jump, as in the reference body; a loop form has not been tried yet
	Darken_Surface(NULL, true); // RA2: as above
	dialog = WS_Create_Dialog(ProgramInstance, 0xE7, MainWindow, (DLGPROC)Birthdate_Dialog_Proc, 0); // RA2: the birthdate dialog
	Show_Dialog(dialog, 0xE7); // RA2: as above
	SendDlgItemMessage(dialog, 0x62E, CB_SETCURSEL, month - 1, 0); // RA2: the month list
	if (day > 0) { // RA2: the day edit
		Wstring text = Wstring_FromInt(day); // RA2: as above
		char const * number = text.get(); // RA2: as above
		HWND edit = GetDlgItem(dialog, 0x62F); // RA2: as above
		SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)number); // RA2: as above
	} else {
		HWND edit = GetDlgItem(dialog, 0x62F); // RA2: as above
		SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)""); // RA2: as above
	}
	if (year > 0) { // RA2: the year edit
		Wstring text = Wstring_FromInt(year); // RA2: as above
		char const * number = text.get(); // RA2: as above
		HWND edit = GetDlgItem(dialog, 0x630); // RA2: as above
		SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)number); // RA2: as above
	} else {
		HWND edit = GetDlgItem(dialog, 0x630); // RA2: as above
		SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)""); // RA2: as above
	}
	SendDlgItemMessage(dialog, 0x626, WM_USER + 0xB4, 0, (LPARAM)email); // RA2: the email edit
	SendDlgItemMessage(dialog, 0x631, BM_SETCHECK, newsletter, 0); // RA2: the newsletter box
	ShowWindow(parent, SW_HIDE); // RA2: as above
	ShowWindow(dialog, SW_SHOWNORMAL); // RA2: as above
	if (WS_Wait_Dialog(dialog, Wait_Callback_Thunk, false, true, true) == 1) { // RA2: the birthdate was entered
		email[0] = 0; // RA2: the answers of the birthdate dialog
		strcpy(email, g_AccountEmail.get()); // RA2: as above
		month = g_BirthdateMonth + 1; // RA2: as above
		day = g_BirthdateDay; // RA2: as above
		year = g_BirthdateYear; // RA2: as above
		newsletter = g_BirthdateNewsletter; // RA2: as above
		if (month <= 0 || day <= 0 || year <= 0 || strlen(email) < 6) { // RA2: a field is missing
			ODMessageBox(StringTable::LoadString("TXT_REQUIRED_FIELD", NULL, "D:\\Ra2\\wonline.cpp", 9595), 0, Wait_Callback_Thunk, false); // RA2: as above
			goto birthdate_retry; // RA2: as above
		}
		g_UserConsent = 0; // RA2: the age check fills both
		g_UserAge = -1; // RA2: as above
		if (g_pNetUtil->RequestAgeCheck(month, day, year, email) != 0) { // RA2: as above
			ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 9603), 0, Wait_Callback_Thunk, false); // RA2: as above
			goto account_retry; // RA2: as above
		}
		{
			WideWstring message(StringTable::LoadString("WOL:RequestingNickname", NULL, "D:\\Ra2\\wonline.cpp", 9606)); // RA2: the wait text of the age check
			Show_Wait_Window(0x100, message, true); // RA2: as above
		}
		if (g_UserAge < 0) { // RA2: the age check failed
			if (g_AgeCheckResult == (int)0x80040100) { // RA2: a field was missing
				ODMessageBox(StringTable::LoadString("TXT_REQUIRED_FIELD", NULL, "D:\\Ra2\\wonline.cpp", 9610), 0, Wait_Callback_Thunk, false); // RA2: as above
			} else {
				ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 9612), 0, Wait_Callback_Thunk, false); // RA2: as above
			}
			goto birthdate_retry; // RA2: as above
		}
		if (g_UserAge < 13 && !g_UserConsent) { // RA2: a child needs the consent of the parents
			if (ODMessageBox(StringTable::LoadString("TXT_CONSENT_REQUIRED", NULL, "D:\\Ra2\\wonline.cpp", 9621), MB_YESNO, Wait_Callback_Thunk, false) == IDYES) { // RA2: as above
				char url[256]; // RA2: as above
				sprintf(url, "http://apiregister.westwood.com/consent_form/index_%d.html", g_LanguageCode); // RA2: as above
				ViewHTML(url, 1); // RA2: as above
			}
			goto birthdate_retry; // RA2: as above
		}
account_retry: // RA2: the retry is written as a jump, as in the reference body; a loop form has not been tried yet
		int resource; // RA2: the account dialog of the age group
		if (g_UserAge < 13) resource = 0xE6; // RA2: as above
		else resource = g_UserAge >= 18 ? 0xF4 : 0xF3; // RA2: as above
		dialog = WS_Create_Dialog(ProgramInstance, resource, MainWindow, (DLGPROC)Account_Details_Dialog_Proc, 0); // RA2: as above
		Show_Dialog(dialog, resource); // RA2: as above
		if (nickname && strlen(nickname) > 0) { // RA2: the nickname edit
			HWND edit = GetDlgItem(dialog, 0x628); // RA2: as above
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)nickname); // RA2: as above
		} else {
			HWND edit = GetDlgItem(dialog, 0x628); // RA2: as above
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)""); // RA2: as above
		}
		if (parent_email && strlen(parent_email) > 0) { // RA2: the parent's email edit
			HWND edit = GetDlgItem(dialog, 0x5E5); // RA2: as above
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)parent_email); // RA2: as above
		} else {
			HWND edit = GetDlgItem(dialog, 0x5E5); // RA2: as above
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)""); // RA2: as above
		}
		if (password && strlen(password) > 0) { // RA2: the password edit
			HWND edit = GetDlgItem(dialog, 0x72C); // RA2: as above
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)password); // RA2: as above
		} else {
			HWND edit = GetDlgItem(dialog, 0x72C); // RA2: as above
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)""); // RA2: as above
		}
		if (confirmation && strlen(confirmation) > 0) { // RA2: the repeated password edit
			HWND edit = GetDlgItem(dialog, 0x627); // RA2: as above
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)confirmation); // RA2: as above
		} else {
			HWND edit = GetDlgItem(dialog, 0x627); // RA2: as above
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)""); // RA2: as above
		}
		ShowWindow(dialog, SW_SHOWNORMAL); // RA2: as above
		SendMessage(dialog, WM_USER + 0xA9, 0, 1); // RA2: as above
		if (WS_Wait_Dialog(dialog, Wait_Callback_Thunk, false, true, true) != 1) goto birthdate_retry; // RA2: cancelled, back to the birthdate
		nickname[0] = 0; // RA2: the answers of the account dialog
		strcpy(nickname, g_AccountName.get()); // RA2: as above
		password[0] = 0; // RA2: as above
		strcpy(password, g_AccountPassword.get()); // RA2: as above
		confirmation[0] = 0; // RA2: as above
		strcpy(confirmation, g_AccountPasswordCheck.get()); // RA2: as above
		parent_email[0] = 0; // RA2: as above
		if (resource == 0xF3) strcpy(parent_email, g_AccountParentEmail.get()); // RA2: only the teenagers' dialog asks for it
		share_info = 0; // RA2: as above
		if (resource == 0xF4) share_info = g_AccountShareInfo; // RA2: only the adults' dialog asks for it
		if (strcmp(password, confirmation)) { // RA2: as above
			ODMessageBox(StringTable::LoadString("TXT_PASSWORD_VERIFY", NULL, "D:\\Ra2\\wonline.cpp", 9727), 0, Wait_Callback_Thunk, false); // RA2: as above
			goto account_retry; // RA2: as above
		}
		if (strlen(password) != 8) { // RA2: as above
			ODMessageBox(StringTable::LoadString("TXT_PASSWORD_TOO_SHORT", NULL, "D:\\Ra2\\wonline.cpp", 9732), 0, Wait_Callback_Thunk, false); // RA2: as above
			goto account_retry; // RA2: as above
		}
		if (g_pNetUtil->RequestNewNick(nickname, password, email, parent_email, newsletter, share_info) != 0) { // RA2: as above
			ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 9737), 0, Wait_Callback_Thunk, false); // RA2: as above
		} else {
			{
				WideWstring message(StringTable::LoadString("TXT_REQ_NICK", NULL, "D:\\Ra2\\wonline.cpp", 9739)); // RA2: the wait text of the nickname request
				Show_Wait_Window(0x100, message, true); // RA2: as above
			}
			if (g_NewNickResult < 0) goto account_retry; // RA2: the nickname was refused
		}
		ShowWindow(parent, SW_SHOW); // RA2: as above
		OwnerDraw::Set_Paint_Disabled(parent, 0); // RA2: as above
		return; // RA2: as above
	} else {
		ShowWindow(parent, SW_SHOWNORMAL); // RA2: the dialog was cancelled
	}
	ShowWindow(parent, SW_SHOW); // RA2: as above
	OwnerDraw::Set_Paint_Disabled(parent, 0); // RA2: as above
}

int Save_Nickname(char const * name, char const * password, bool mangle, int locale) // RA2: 0x00745320, not in TibSun: stores the persona in the chat API's list of 32 logins (the slot it already has, else an empty one, else the next of a round robin) and its locale; the slot
{
	int slot = 0; // RA2: as above
	char const * nick; // RA2: as above
	char const * stored; // RA2: as above
	for (int i = 1; i <= 32; i++) { // RA2: the slot that already holds the persona
		nick = NULL; // RA2: as above
		stored = NULL; // RA2: as above
		if (g_pChat->GetNick(i, &nick, &stored) == 0 && nick && !strcmp(nick, name)) { // RA2: as above
			slot = i; // RA2: as above
			break; // RA2: as above
		}
	}
	if (slot == 0) { // RA2: else the first empty one
		for (int i = 1; i <= 32; i++) { // RA2: as above
			nick = NULL; // RA2: as above
			stored = NULL; // RA2: as above
			if (g_pChat->GetNick(i, &nick, &stored) != 0 || !nick || strlen(nick) == 0) { // RA2: as above
				slot = i; // RA2: as above
				break; // RA2: as above
			}
		}
	}
	if (slot == 0) { // RA2: else the next slot in turn
		int next = Session.LastNicknameSlot; // RA2: as above
		if (next == -1) { // RA2: as above
			next = 1; // RA2: as above
		}
		slot = next++; // RA2: as above
		Session.LastNicknameSlot = next; // RA2: as above
		if (next > 32) { // RA2: as above
			Session.LastNicknameSlot = 1; // RA2: as above
		}
	}
	if (slot) { // RA2: as above
		g_pChat->SetNick(slot, name, password, mangle); // RA2: virtual 0x94
		g_pChat->SetNickLocale(slot, (Locale)locale); // RA2: virtual 0xC8
	}
	return(slot); // RA2: as above
}

Wstring Server_Name_Without_Prefix(Server const * server) // RA2: 0x00745450, not in TibSun: the name of the server after the first colon (the part before it names the kind of server)
{
	Wstring name((char const *)server->name); // RA2: the server's name is copied into a Wstring
	Wstring token; // RA2: as above
	int position = name.getToken(0, ":", token); // RA2: the first token ends at the first colon
	if (position >= 0) { // RA2: as above
		name.remove(0, position + 1); // RA2: as above
	}
	return(name); // RA2: as above
}

#define OD_ADDSTRING_COMBO (WM_USER + 188) // RA2: 0x4BC, appends the string of lParam to an owner drawn combo box and returns its row (name guessed from its use)
#define OD_ADDSTRING_LIST (WM_USER + 200) // RA2: 0x4C8, the same for an owner drawn list box (name guessed from its use)
int __fastcall Clear_ComboBox(HWND list); // RA2: 0x006007F0, removes every row of an owner drawn combo box, defined in ownrdraw.cpp

bool Populate_Server_Choices(HWND list, bool combo) // RA2: 0x007454C0, not in TibSun: fills a list box (or combo box) with the names of the chat servers and selects the one of the persona's profile; false when the server list is empty
{
	if (combo) { // RA2: as above
		Clear_ComboBox(list); // RA2: as above
	} else {
		Clear_Listbox(list); // RA2: as above
	}
	if (g_Servers.length() == 1) { // RA2: only the entry 0 of the list: no server to choose from
		if (combo) { // RA2: as above
			SendMessage(list, OD_ADDSTRING_COMBO, 0, (LPARAM)"GUI:NoServersAvailable"); // RA2: as above
		} else {
			SendMessage(list, OD_ADDSTRING_LIST, 0, (LPARAM)"GUI:NoServersAvailable"); // RA2: as above
		}
		EnableWindow(list, FALSE); // RA2: as above
		return(false); // RA2: as above
	}
	Wstring preferred; // RA2: the server of the persona's profile
	WOLOptionsClass options; // RA2: as in Connect_To_Server
	options.Load(); // RA2: as above
	rc_ptr<PlayerProfile> profile; // RA2: as above
	if (options.Is_Bit1_Clear()) { // RA2: as above
		profile = PlayerProfile::Load(g_NickName); // RA2: as above
	} else {
		profile = g_PlayerProfile.Get(); // RA2: as above
	}
	if (profile.Get()) { // RA2: as above
		preferred = profile->Name; // RA2: as above
	}
	EnableWindow(list, TRUE); // RA2: as above
	int selected = -1; // RA2: as above
	for (int i = 1; i < g_Servers.length(); i++) { // RA2: as above
		Server * server = NULL; // RA2: as above
		g_Servers.getPointer(&server, i); // RA2: as above
		DebugString("Server found %s\n", (char *)server->name); // RA2: as above
		Wstring display = Server_Name_Without_Prefix(server); // RA2: expanded here
		int row; // RA2: as above
		if (combo) { // RA2: as above
			row = SendMessage(list, OD_ADDSTRING_COMBO, 0, (LPARAM)display.text()); // RA2: as above
			SendMessage(list, CB_SETITEMDATA, row, i); // RA2: as above
		} else {
			row = SendMessage(list, OD_ADDSTRING_LIST, 0, (LPARAM)display.text()); // RA2: as above
			SendMessage(list, LB_SETITEMDATA, row, i); // RA2: as above
		}
		if (preferred.length() > 0) { // RA2: as above
			bool match = ((Wstring const &)preferred).toLower() == ((Wstring const &)display).toLower(); // RA2: the const toLower returns a copy
			if (match) { // RA2: as above
				selected = row; // RA2: as above
				g_CurrentServerIndex = i; // RA2: the server of the profile becomes the one to connect to
				g_TargetServerIndex = i; // RA2: as above
			}
		}
	}
	if (selected == -1) { // RA2: as above
		selected = 0; // RA2: as above
	}
	if (combo) { // RA2: as above
		SendMessage(list, CB_SETCURSEL, selected, 0); // RA2: as above
	} else {
		SendMessage(list, LB_SETCURSEL, selected, 0); // RA2: as above
		SendMessage(list, LB_SETTOPINDEX, selected, 0); // RA2: as above
	}
	return(true); // RA2: as above
}

bool __fastcall Can_Join_Channel(Channel const * channel, bool check, bool observer); // RA2: 0x00755CD0, whether the player may join the game channel (the version, the full game, the observer slot ...); not yet defined
bool Test_Display_Mode_Dialog(int width, int height); // RA2: 0x00543A70, defined in mainopt.cpp
LRESULT CALLBACK Wonline_Password_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007492D0, the dialog proc of the password dialog (0xE1)
void Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, defined in init.cpp

bool __fastcall Join_Game_Channel(Channel const * channel, bool observer) // RA2: 0x00745780, not in TibSun: joins a game channel: tests that the player may (Can_Join_Channel), that the screen is not wider than the game's resolution limit (offering to switch to 800x600 or 640x480), asks for the password of a private game; true when the join was requested
{
	if (!Can_Join_Channel(channel, true, observer)) { // RA2: as above
		return(false); // RA2: as above
	}
	unsigned int mode = channel->reserved; // RA2: the flags of the game: the resolution limit
	int maximum = (mode & 0x400) ? 800 : ((mode & 0x200) ? 640 : 1024); // RA2: as above
	if (VideoModeWidth > maximum) { // RA2: the screen is wider than the game allows
		{
			WideWstring warning((mode & 0x400) ? StringTable::LoadString("WOL:ResolutionLimited800", NULL, "D:\\Ra2\\wonline.cpp", 10143) : StringTable::LoadString("WOL:ResolutionLimited640", NULL, "D:\\Ra2\\wonline.cpp", 10144)); // RA2: as above
			if (ODMessageBox(warning.get(), 4, Wait_Callback_Thunk, true) == 6) { // RA2: IDYES: the screen is switched
				int width = (maximum == 800) ? 800 : 640; // RA2: as above
				int height = (width == 800) ? 600 : 480; // RA2: as above
				if (Test_Display_Mode_Dialog(width, height)) { // RA2: as above
					Options.ScreenWidth = width; // RA2: as above
					Options.ScreenHeight = height; // RA2: as above
					Options.Save_Settings(); // RA2: as above
				}
			}
		}
		if (VideoModeWidth > maximum) { // RA2: as above
			return(false); // RA2: as above
		}
	}
	Wstring password; // RA2: as above
	if (channel->flags & 0x100) { // RA2: a private game: the player types the password
		Darken_Surface(NULL, true); // RA2: as above
		WonlineStringDialogControl control; // RA2: as above
		if (OwnerDraw::Controlled_Dialog(0xE1, (DLGPROC)Wonline_Password_Dialog_Proc, &control) == 2) { // RA2: cancelled
			return(false); // RA2: as above
		}
		password = control.Text; // RA2: as above
		password.replace(" ", "_"); // RA2: spaces are not allowed in the key
	}
	return(Request_Channel_Join((Channel *)channel, password, WideWstring(StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 10199))) == 0); // RA2: as above
}

LRESULT CALLBACK Create_Game_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00748E10, the dialog proc of the create game dialog (0xC3); not yet defined
BOOL CALLBACK Wonline_Game_Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x0074A710, the dialog proc of the game options dialog (0xC2); not yet defined
void __fastcall Seat_First_Player(int slot); // RA2: 0x005C8CD0, defined in netshare.cpp
void Clear_Vector(DynamicVectorClass<NodeNameType *> * vector); // RA2: 0x005A7480, defined in mplayer.cpp
MultiplayerGameMode * First_Game_Mode(void); // RA2: 0x005B6EC0, defined in multiplayer.cpp
namespace OwnerDraw { // RA2: as above
void __fastcall Slide_Dialog(HWND dialog, bool reverse); // RA2: 0x005E5150, defined in ownrdraw.cpp
}

static inline void Reset_Session_Players(void) // RA2: the players of the session are cleared (expanded in Host_Game)
{
	MultiplayerGameMode * mode = First_Game_Mode(); // RA2: as above
	Session.MPGameMode = mode; // RA2: as above
	Session.Options.MPModeIndex = mode->MPModeIndex; // RA2: as above
	mode->vt_entry_20(); // RA2: as above
	Set_Scenario_Info_From_Index(0); // RA2: as above
	Clear_Vector(&Session.Players); // RA2: as above
	Clear_Slot_Lists(); // RA2: as above
	Reset_Slots(); // RA2: as above
}

static inline void Show_Game_Options(void) // RA2: opens the game options dialog (0xC2) of a game the player hosts (expanded in Host_Game)
{
	if (!Is_Tournament_Session_Inline()) { // RA2: as above
		HWND top = WS_Top_Window(); // RA2: as above
		if (top) { // RA2: as above
			OwnerDraw::Slide_Dialog(top, false); // RA2: as above
			Darken_Surface(NULL, true); // RA2: as above
		}
		Darken_Surface(NULL, true); // RA2: as above
		HWND dialog = OwnerDraw::Begin_Dialog(0xC2, (DLGPROC)Wonline_Game_Options_Dialog_Proc, 0); // RA2: as above
		Session.MPGameMode->vt_entry_84(); // RA2: as above
		Session.MPGameMode->vt_entry_20(); // RA2: as above
		Session.MPGameMode->AssignTeamsAtStart(); // RA2: as above
		Session.MPGameMode->UnfixAlliances(); // RA2: as above
		ShowWindow(dialog, SW_SHOWNORMAL); // RA2: as above
		if (top) { // RA2: as above
			ShowWindow(top, SW_HIDE); // RA2: as above
		}
	}
}

/*
**	RA2: this unit's Add of the player list holds the new length in a variable (the executable forms it in the register
**	that held Length(): 0x007459A0 + 1204), as link2.cpp and objtype.cpp have it for their vectors.
*/
template<> inline bool DynamicVectorClass<NodeNameType *>::Add(NodeNameType * const & object) // RA2: as above
{
	if (ActiveCount >= Length()) { // RA2: as above
		if ((IsAllocated || !VectorMax) && GrowthStep > 0) { // RA2: as above
			int new_length = Length(); // RA2: as above
			new_length += GrowthStep; // RA2: as above
			if (!Resize(new_length)) { // RA2: as above
				return(false); // RA2: as above
			}
		} else {
			return(false); // RA2: as above
		}
	}
	(*this)[ActiveCount++] = object; // RA2: as above
	return(true); // RA2: as above
}

bool __fastcall Host_Game(void) // RA2: 0x007459A0, not in TibSun: hosts a game: asks for the game's settings (the create game dialog 0xC3, unless it is an Internet tournament game), makes the host the first player of the lobby, asks the chat service for the game channel and opens the game options dialog (0xC2); true when the channel was created
{
	Wstring password; // RA2: the key of the game channel
	if (VideoModeWidth >= 1024) { // RA2: as above
		Session.WOLLimitResolution = false; // RA2: as above
	}
	CreateGameDialogControl control(Session.WOLLimitResolution, ObserverMode == 1); // RA2: as above
	g_GameStartRequested = false; // RA2: as above
	if (Is_Tournament_Session_Inline()) { // RA2: a tournament game has two players and no dialog
		g_MaxPlayers = 2; // RA2: as above
	} else {
		Darken_Surface(NULL, true); // RA2: as above
		if (OwnerDraw::Controlled_Dialog(0xC3, (DLGPROC)Create_Game_Dialog_Proc, &control) == 2) { // RA2: cancelled
			return(false); // RA2: as above
		}
		password = control.Text; // RA2: as above
		password.replace(" ", "_"); // RA2: spaces are not allowed in the key
	}
	g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
	Channel channel; // RA2: the game channel
	memset(&channel, 0, sizeof(channel)); // RA2: as above
	sprintf((char *)channel.name, "%s's_game", *(char **)&g_NickName); // RA2: the executable reads the string pointer of the Wstring itself (no accessor call)
	channel.type = 0x21; // RA2: a game channel
	channel.minUsers = 1; // RA2: as above
	if (g_MaxPlayers < 2) { // RA2: as above
		g_MaxPlayers = 4; // RA2: as above
	}
	strcpy((char *)channel.key, password.get()); // RA2: as above
	channel.maxUsers = g_MaxPlayers; // RA2: as above
	char info[80]; // RA2: the info text of the channel
	memset(info, 0, sizeof(info)); // RA2: as above
	Encode_Channel_ExInfo(info); // RA2: as above
	info[40] = 0; // RA2: the info member holds 40 characters
	strcpy((char *)channel.exInfo, info); // RA2: as above
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: an Internet tournament game
		channel.tournament = 1; // RA2: as above
		channel.reserved |= 0x1000; // RA2: as above
	} else if (Is_Tournament_Session_Inline()) { // RA2: as above
		channel.tournament = 1; // RA2: as above
		channel.reserved |= 0x800; // RA2: as above
	} else {
		channel.tournament = control.IsTournament; // RA2: as above
		if (control.IsPrivate) { // RA2: as above
			channel.reserved |= 0x100; // RA2: as above
		}
		Session.WOLLimitResolution = control.LimitResolution; // RA2: as above
		if (control.LimitResolution) { // RA2: as above
			channel.reserved |= (VideoModeWidth == 800) ? 0x400 : 0x200; // RA2: as above
		}
	}
	channel.reserved = channel.reserved + g_LobbyNumber; // RA2: the lobby the host came from
	Leave_Current_Channel(0xC0, false); // RA2: expanded here
	Reset_Session_Players(); // RA2: expanded here
	NodeNameType * host = new NodeNameType; // RA2: the entry of the host
	memset(host, 0, sizeof(NodeNameType)); // RA2: as above
	WideWstring name = Wstring_ToWide(g_NickName); // RA2: as above
	wcscpy(host->Name, name.get()); // RA2: as above
	WOLOptionsClass options; // RA2: as above
	options.Load(); // RA2: as above
	rc_ptr<PlayerProfile> profile; // RA2: as above
	if (options.Is_Bit1_Clear()) { // RA2: as above
		profile = PlayerProfile::Load(g_NickName); // RA2: as above
	} else {
		profile = g_PlayerProfile.Get(); // RA2: as above
	}
	if (profile.Get()) { // RA2: the side and color of the persona
		Session.Set_Side(profile->Country, false); // RA2: as above
		Session.Set_Color(profile->Color, false); // RA2: as above
	}
	host->Set_Country(Session.Chosen_House()); // RA2: as above
	host->Set_Color(Session.Chosen_Color()); // RA2: as above
	host->Slot.Unknown77 = 1; // RA2: as above
	host->Slot.SpectatorFlag = 0; // RA2: as above
	LobbyObserverSlot = 7; // RA2: as above
	ObserverMode = control.Observer; // RA2: as above
	Update_Observer_Buttons(0); // RA2: as above
	if (ObserverMode) { // RA2: the host observes: the first slot, team -1, house -3
		LobbyObserverSlot = 0; // RA2: as above
		host->Slot.SpectatorFlag = -1; // RA2: as above
		host->Set_Country(-3); // RA2: as above
	}
	Session.Players.Add(host); // RA2: as above
	Seat_First_Player(0); // RA2: as above
	wcscpy(Session.GameName, name.get()); // RA2: as above
	HRESULT result = g_pChat->RequestChannelCreate(&channel); // RA2: virtual 0x1C
	if (result == 0x800401F8) { // RA2: the key was refused
		ODMessageBox(StringTable::LoadString("TXT_BAD_CHANKEY", NULL, "D:\\Ra2\\wonline.cpp", 10434), 0, Wait_Callback_Thunk, false); // RA2: as above
		Close_Wait_Window(0x80); // RA2: expanded here
		return(false); // RA2: as above
	}
	if (result != 0) { // RA2: as above
		ODMessageBox(StringTable::LoadString("TXT_CANTCREATE_CHAN", NULL, "D:\\Ra2\\wonline.cpp", 10440), 0, Wait_Callback_Thunk, false); // RA2: as above
		Close_Wait_Window(0x80); // RA2: expanded here
		return(false); // RA2: as above
	}
	WideWstring title(StringTable::LoadString("GUI:CreatingChannel", NULL, "D:\\Ra2\\wonline.cpp", 10448)); // RA2: the wait dialog takes a wide text
	Show_Wait_Window(4, title, false); // RA2: as above
	Close_Wait_Window(0x80); // RA2: expanded here
	Show_Wait_Window(4, title, true); // RA2: as above
	int index = Player_Name_To_Index(g_NickName); // RA2: expanded here
	if (index < 0) { // RA2: the host is not in the list of users
		return(false); // RA2: as above
	}
	User * user = NULL; // RA2: as above
	g_UserList.getPointer(&user, index); // RA2: as above
	host->Address = User_To_IPX_Address(user); // RA2: expanded here
	Show_Game_Options(); // RA2: expanded here
	return(true); // RA2: as above
}


int __fastcall Parse_Online_Command(wchar_t const * text) // RA2: 0x00746240, not in TibSun: the lobby chat command that looks a player up (TXT_ONLINE_COMMAND followed by a nick): 1 when the command was handled
{
	int handled = 0; // RA2: as above
	wchar_t command[64]; // RA2: as above
	wcscpy(command, StringTable::LoadString("TXT_ONLINE_COMMAND", 0, "D:\\Ra2\\wonline.cpp", 10505)); // RA2: as above
	if (wcsncmp(text, command, wcslen(command)) == 0) { // RA2: as above
		int count = 0; // RA2: as above
		wchar_t const * cursor = text + wcslen(command); // RA2: as above
		while (wcsncmp(cursor, L" ", 1) == 0) { // RA2: as above
			++cursor; // RA2: as above
		}
		wchar_t name[16]; // RA2: as above
		while (*cursor) { // RA2: as above
			if (wcsncmp(cursor, L" ", 1) == 0 || count >= 10) { // RA2: as above
				break; // RA2: as above
			}
			name[count++] = *cursor++; // RA2: as above
		}
		name[count] = 0; // RA2: as above
		if (wcslen(name) > 0) { // RA2: as above
			User user; // RA2: as above
			strcpy((char *)user.name, Wide_To_Narrow(name)); // RA2: as above
			strcpy(LastFindName, Wide_To_Narrow(name)); // RA2: as above
			g_pChat->RequestFind(&user); // RA2: virtual 0x60
			handled = 1; // RA2: as above
		} else {
			wchar_t message[256]; // RA2: as above
			swprintf(message, StringTable::LoadString("TXT_NO_NAME", 0, "D:\\Ra2\\wonline.cpp", 10525)); // RA2: as above
			HWND window = WS_Top_Window(); // RA2: as above
			if (window) { // RA2: as above
				if (GetDlgItem(window, 0x53F)) { // RA2: as above
					Lobby_Chat_Printf(ColorSystem, message); // RA2: as above
				}
				if (GetDlgItem(window, 0x5B5)) { // RA2: as above
					SMessagePrintf(ColorSystem, message); // RA2: as above
				}
			}
			if (Session.Is_In_Game()) { // RA2: as above
				int color = GadgetClass::Get_Color_Scheme(); // RA2: as above
				Session.Messages.Add_Message(0, 0, message, color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false); // RA2: as above
			}
			handled = 1; // RA2: as above
		}
	}
	return(handled); // RA2: as above
}


int __fastcall Parse_Page_Command(wchar_t const * text) // RA2: 0x00746480, not in TibSun: the lobby chat command that pages a player (TXT_PAGE_COMMAND, a nick and the text): 1 when handled, 2 when a message was shown instead
{
	int result = 0; // RA2: as above
	wchar_t nickname[16]; // RA2: as above
	User recipient; // RA2: as above
	wchar_t command[128]; // RA2: as above
	wcscpy(command, StringTable::LoadString("TXT_PAGE_COMMAND", 0, "D:\\Ra2\\wonline.cpp", 10559)); // RA2: as above
	if (_wcsnicmp(text, command, wcslen(command)) == 0) { // RA2: as above
		int name_length = 0; // RA2: as above
		unsigned length = wcslen(text) + 1; // RA2: as above
		wchar_t * copy = (wchar_t *)operator new(length * 2); // RA2: as above
		if (!copy) { // RA2: as above
			return(0); // RA2: as above
		}
		copy = wcsncpy(copy, text, length); // RA2: as above
		if (!copy) { // RA2: as above
			return(0); // RA2: as above
		}
		wchar_t * message = copy + wcslen(command); // RA2: as above
		while (*message && wcsncmp(message, L" ", 1) == 0) { // RA2: as above
			++message; // RA2: as above
		}
		while (*message) { // RA2: as above
			if (wcsncmp(message, L" ", 1) == 0 || name_length > 10) { // RA2: as above
				break; // RA2: as above
			}
			nickname[name_length++] = *message++; // RA2: as above
		}
		nickname[name_length] = 0; // RA2: as above
		while (*message && wcsncmp(message, L" ", 1) == 0) { // RA2: as above
			++message; // RA2: as above
		}
		if (name_length > 10) { // RA2: as above
			result = 1; // RA2: as above
		} else if (name_length == 0) { // RA2: as above
			result = 1; // RA2: as above
		} else {
			bool recipient_is_player = false; // RA2: as above
			if (MuteSWLaunches) { // RA2: as above
				for (int i = 0; i < Session.Players.Count(); ++i) { // RA2: as above
					if (_wcsicmp(Session.Players[i]->Name, nickname) == 0) { // RA2: as above
						recipient_is_player = true; // RA2: as above
						break; // RA2: as above
					}
				}
			}
			if (!recipient_is_player) { // RA2: as above
				if (wcslen(message) > 0) { // RA2: as above
					strcpy((char *)recipient.name, Wide_To_Narrow(nickname)); // RA2: as above
					g_pChat->RequestPageWide(&recipient, message); // RA2: virtual 0xF8
				}
				result = 1; // RA2: as above
			} else {
				int color = GadgetClass::Get_Color_Scheme(); // RA2: as above
				Session.Messages.Add_Message(0, 0, StringTable::LoadString("TXT_OBSERVER_CANT_PAGE", 0, "D:\\Ra2\\wonline.cpp", 10619), color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false); // RA2: as above
				result = 2; // RA2: as above
			}
		}
		operator delete(copy); // RA2: as above
	}
	return(result); // RA2: as above
}


int __fastcall Reply_To_Last_Pager(wchar_t const * text) // RA2: 0x007466C0, not in TibSun: the lobby chat command that answers the last pager (TXT_REPLY_COMMAND or its short form, then the text): 1 when handled, 2 when a message was shown instead
{
	int result = 0; // RA2: as above
	wchar_t reply_command[128]; // RA2: as above
	User recipient; // RA2: as above
	wchar_t short_command[128]; // RA2: as above
	wchar_t notice[128]; // RA2: as above
	wcscpy(reply_command, StringTable::LoadString("TXT_REPLY_COMMAND", 0, "D:\\Ra2\\wonline.cpp", 10639)); // RA2: as above
	wcscpy(short_command, StringTable::LoadString("TXT_REPLY_COMMAND_SHORT", 0, "D:\\Ra2\\wonline.cpp", 10640)); // RA2: as above
	if (_wcsnicmp(text, reply_command, wcslen(reply_command)) == 0 || wcsncmp(text, short_command, wcslen(short_command)) == 0) { // RA2: as above
		unsigned length = wcslen(text) + 1; // RA2: as above
		wchar_t * copy = (wchar_t *)operator new(length * 2); // RA2: as above
		if (!copy) { // RA2: as above
			return(0); // RA2: as above
		}
		copy = wcsncpy(copy, text, length); // RA2: as above
		if (!copy) { // RA2: as above
			return(0); // RA2: as above
		}
		wchar_t * message; // RA2: as above
		if (wcsncmp(text, reply_command, wcslen(reply_command)) == 0) { // RA2: as above
			message = copy + wcslen(reply_command); // RA2: as above
		} else {
			message = copy + wcslen(short_command); // RA2: as above
		}
		while (*message && wcsncmp(message, L" ", 1) == 0) { // RA2: as above
			++message; // RA2: as above
		}
		bool recipient_is_player = false; // RA2: as above
		if (MuteSWLaunches) { // RA2: as above
			for (int i = 0; i < Session.Players.Count(); ++i) { // RA2: as above
				if (_wcsicmp(Session.Players[i]->Name, Narrow_To_Wide(LastPagerName)) == 0) { // RA2: as above
					recipient_is_player = true; // RA2: as above
					break; // RA2: as above
				}
			}
		}
		if (!recipient_is_player) { // RA2: as above
			if (strlen(LastPagerName) > 0) { // RA2: as above
				if (wcslen(message) > 0) { // RA2: as above
					strcpy((char *)recipient.name, LastPagerName); // RA2: as above
					g_pChat->RequestPageWide(&recipient, message); // RA2: virtual 0xF8
				}
				result = 1; // RA2: as above
			} else {
				wcscpy(notice, StringTable::LoadString("TXT_NO_ONE_WHOM_TO_REPLY", 0, "D:\\Ra2\\wonline.cpp", 10691)); // RA2: as above
				HWND window = WS_Top_Window(); // RA2: as above
				if (window) { // RA2: as above
					if (GetDlgItem(window, 0x53F)) { // RA2: as above
						Lobby_Chat_Printf(ColorSystem, notice); // RA2: as above
					}
					if (GetDlgItem(window, 0x5B5)) { // RA2: as above
						SMessagePrintf(ColorSystem, notice); // RA2: as above
					}
				}
				if (Session.Is_In_Game()) { // RA2: as above
					int color = GadgetClass::Get_Color_Scheme(); // RA2: as above
					Session.Messages.Add_Message(0, 0, notice, color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false); // RA2: as above
				}
				result = 2; // RA2: as above
			}
		} else {
			int color = GadgetClass::Get_Color_Scheme(); // RA2: as above
			Session.Messages.Add_Message(0, 0, StringTable::LoadString("TXT_OBSERVER_CANT_PAGE", 0, "D:\\Ra2\\wonline.cpp", 10715), color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false); // RA2: as above
			result = 2; // RA2: as above
		}
		operator delete(copy); // RA2: as above
	}
	return(result); // RA2: as above
}
