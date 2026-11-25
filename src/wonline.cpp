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
#include "geometry.h"
#include "rect.h"

#include "_keyboar.h"
#include "_map.h"
#include "_pk.h"
#include "_rules.h"
#include "arraylist.h"
#include "dict.h"
#include "ccfile.h"
#include "ccrand.h"
#include "conquer.h"
#include "data.h"
#include "dbgprint.h"
#include "firewall.h"
#include "globals.h"
#include "goptions.h"
#include "ini.h"
#include "rcptr.h"
#include "rules.h"
#include "init.h"
#include "internet.h"
#include "misc.h"
#include "mixfile.h"
#include "msgloop.h"
#include "ipxmgr.h"
#include "language\language.h"
#include "mapgen.h"
#include "netshare.h"
#include "ownertalk.h"
#include "ownrdraw.h"
#include "playerprofile.h"
#include "progress.h"
#include "scenario.h"
#include "sendfile.h"
#include "sounddlg.h"
#include "srfcache.h"
#include "session.h"
#include "theme.h"
#include "widewstring.h"
#include "windlg.h"
#include "winstub.h"
#include "chatdefs.h"
#include "downloaddefs.h"
#include "wolapi.h"
#include "wolapi_i.c"
#include "atl.h"
#include "wonline.h"
#include "worlddom.h"
#include "wonlinecontrol.h"
#include "wonlinestringcontrol.h"
#include "creategamecontrol.h"
#include "wdtnet.h"
#include "wolopt.h"
#include "wsproto.h"
#include "wspudp.h"
#include "wstring.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <shellapi.h>
#include <windows.h>
#include <snmp.h>
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
HRESULT g_LastIgnoredStatus; // RA2: 0x00B2AC00, TibSun's name: the chat's last net status while g_IgnoreNetStatus is set
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
int g_ListIconHeight = 14;
int g_Col_Rung;
int g_Col_Name;
int g_Col_Points;
int g_Col_Wins;
int g_Col_Losses;
int g_Col_Disconnects;
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
CComModule _Module;

Wstring g_MessageOfTheDay;

ArrayList<Server> g_Servers;
ArrayList<Channel> g_UserChannels;
ArrayList<Channel> g_GameChannelList;
ArrayList<User> g_UserList;
ArrayList<int> g_ChanListQueue;

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
int g_MatchPingsReceived; // RA2: 0x00B26568, the users whose ping came back since the match request
char g_MatchStartText[256]; // RA2: 0x00B2658C, what the match bot's Start command carries

bool Is_Tournament_Session(void)
{
	return(Online_Session_State == 2 || Online_Session_State == 1);
}

Wstring g_BirthdateEmail;
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
void __fastcall Update_Buddy_Profile(HWND dialog);
void Request_Buddy_Ladders(void);
int Save_Nickname(char const * name, char const * password, bool mangle, int locale);
void __fastcall Service_Pending_Player_Pings(void);
void __fastcall Send_Pending_Ping_Report(void);
BOOL CALLBACK WOL_Download_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Handle_Matchmaking_Command(char const * message, bool strict);
void __fastcall Darken_Surface(Surface * surface, bool update);
BOOL CALLBACK Page_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
Wstring __fastcall Get_Page_Recipient(HWND list);
int Compare_Users(User * u1, User * u2);
bool Is_Channel_Full(void);
bool __fastcall Is_All_Slots_Filled(void);
void __fastcall Add_Buddy(Wstring const & name);
bool __fastcall Find_Local_Address(IPXAddressClass * address);
void __fastcall User_List_To_Session_Players(User * user);
User * Find_Owner_User(void);
int __fastcall Build_Connection_Pairs(int (*pairs)[2]);
void __fastcall Enable_Start_Button(void);
bool __fastcall Request_Match(void);
bool __fastcall Find_Channel_By_Name(Wstring const & name, ArrayList<Channel> const & channels, Channel & channel);
void Service_Buddy_Find_Queue(void);
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
			Theme.Queue_Song(THEME_PICK_ANOTHER);
			return;
		}
		if (Theme.What_Is_Playing() == Theme.From_Name("INTRO")) {
			Theme.Stop();
			Theme.Queue_Song(THEME_PICK_ANOTHER);
		}
	} else {
		Theme.Queue_Song(THEME_QUIET);
	}
}

void __fastcall Set_Match_State(int state, int timeout, int result); // RA2: 0x00759DF0, below

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
/// Restarts the lobby music from the top of the lobby theme list.
static inline void Restart_Lobby_Music(void)
{
	Theme.Stop();
	Theme.Queue_Song((ThemeType)THEME_PICK_ANOTHER);
}

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
		Restart_Lobby_Music();
	} else if (music) {
		if (Theme.What_Is_Playing() == Theme.From_Name("INTRO")) {
			Restart_Lobby_Music();
		}
	} else if (!music) {
		Theme.Queue_Song(THEME_QUIET);
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
		g_pChat->RequestPublicUnicodeMessage((unsigned short const *)msg.text());
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

		if (head) g_pChat->RequestPrivateUnicodeMessage(head, (unsigned short const *)msg.text());

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
		g_pChat->RequestPublicUnicodeAction((unsigned short const *)msg.text());
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

		if (head) g_pChat->RequestPrivateUnicodeAction(head, (unsigned short const *)msg.text());

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
void Encode_Game_Options(char * out)
{
	static char useroptions[512];

	wchar_t const * description = Session.Options.ScenarioDescription;
	Wstring name = description ? Wstring_FromWide(description) : "";

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

void Reset_Wait_Events(void); // RA2: 0x00752930, below

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
	g_ChanListQueue.addTail(channel_type);
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
		if (!Wait_Channel_List()) return false;
	}
	Reset_Wait_Events();
	if (!Request_Channel_List(channel_type, autoping)) {
		SetEvent(g_WaitEventHandles[EV_ABORT]);
		return(false);
	}
	if (wait) {
		return Wait_Channel_List();
	}
	return(true);
}

/// <summary>
/// Shows the World Domination Tour "available servers" dialog and waits for it to close.
/// </summary>
/// <returns>Result code returned by the dialog (see WS_Wait_Dialog/WS_Destroy_Dialog).</returns>
bool Request_Chat_Channel_List(bool wait)
{
	return(Queue_Channel_List_Request(0, 0, wait));
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
	return(Queue_Channel_List_Request(0x21, autoping, wait));
}

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

	if (Is_Tournament_Session() && g_MatchTimeout != -1 && g_MatchState != 1 && g_MatchState != 0 && g_MatchState != 11 && (unsigned)(time(NULL) - g_MatchStartTime) > (unsigned)g_MatchTimeout) {
		if (g_MatchState >= 2 && g_MatchState <= 5) {
			Set_Match_State(0, -1, 2);
		} else {
			Lobby_Chat_Printf(0x20C0FF, StringTable::LoadString("WOL:MatchTimeout", NULL, "D:\\Ra2\\wonline.cpp", 2155));
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
					last_user = new_user;
				} else {
					last_user->next = new_user;
					last_user = new_user;
				}
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

			if (!Is_Tournament_Session()) {
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



bool __fastcall Local_User_Is_Channel_Owner(void);
void Set_Wait_Window_Text(LPARAM text); // RA2: 0x0074D750, defined below
void __fastcall Change_Slot_Type(HWND dialog, int id, NodeNameType * node, int mode); // RA2: 0x005CBF20, defined in netshare.cpp
int __fastcall Assign_Slot(NodeNameType * entry); // RA2: 0x005C9170, defined in netshare.cpp
NodeNameType * __fastcall Find_Lobby_Name(wchar_t const * name); // RA2: 0x005C8B20, defined in netshare.cpp
namespace OwnerDraw { BOOL __stdcall Set_Paint_Disabled(HWND window, LPARAM lparam); } // RA2: 0x005ECF40, defined in ownrdraw.cpp
bool __fastcall Can_Join_Channel(Channel const * channel, bool check, bool observer); // RA2: 0x00755CD0, defined below
void Clear_Vector(DynamicVectorClass<NodeNameType *> * vector); // RA2: 0x005A7480, defined in mplayer.cpp
void __fastcall Clear_Slot_Lists(void); // RA2: 0x005C8A70, defined in netshare.cpp
void __fastcall Reset_Slots(void); // RA2: 0x005C8A90, defined in netshare.cpp
BOOL CALLBACK WOL_Guest_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x0074C660, defined below
void __fastcall Show_Dialog(HWND dialog, int id); // RA2: 0x005FF7E0, defined in ownrdraw.cpp
extern int g_PlayerPings[8]; // RA2: 0x00B292AC, defined below
void Update_Channel_Player_Limit(void); // RA2: 0x00749F20, defined below
void __fastcall Kick_Channel_User(Wstring const & name); // RA2: 0x0075CA60, defined below
void __fastcall Apply_Endpoint_Map(char const * text); // RA2: 0x0075C570, defined below
void __fastcall Handle_Mode_Selection(void); // RA2: 0x005C34F0, defined in netshare.cpp
bool __fastcall Is_Host(void); // RA2: 0x005C86A0, defined in netshare.cpp
void __fastcall Send_User_Pings(void); // RA2: 0x0075B1B0, defined below
void Request_All_User_Pings(void); // RA2: 0x0075AF80, defined below

#include "multiplayer.h" // RA2: the game mode decides who may play and observe (OnPrivateGameOptions)

char g_WDTHost[128];
int g_WDTPort;
bool g_ReceivingHostPreview; // RA2: 0x00B2AC46, set while Handle_Preview_Download takes the map preview from the host


static inline void Show_Name(HWND window, Wstring const & name) // ra2-decomp's helper
{
	HWND label = GetDlgItem(window, 0x718);
	SendMessageA(label, 0x4B4, 0, (LPARAM)name.text());
}

class CChatEventSink : public CComObjectRoot, public IChatEvent
{
	public:
		CChatEventSink(void) {}
		BEGIN_COM_MAP(CChatEventSink)
		COM_INTERFACE_ENTRY(IChatEvent)
		END_COM_MAP()
		STDMETHOD(OnServerList)(HRESULT res, Server * servers)
		{
			Server * tmp = servers;
			Server * server = NULL;
			int chat_server_index = 0;
			int skumatchcount = 0;
			int game_srv_found = 0;
			char namebuf[128];
			char conndata[128];
			char conndata2[128];
			char conndata3[128]; // RA2: the second game server
			char token[128];
			char manglerdata[128]; // RA2: the mangler servers

			srand(time(0));

			/// Failed to get a server list at all -- abort the login attempt.
			if (FAILED(res)) {
				SetEvent(g_WaitEventHandles[EV_ABORT]);
				swprintf(g_LastErrorMessage, StringTable::LoadString("TXT_NO_SERV_LIST", NULL, "D:\\Ra2\\wonline.cpp", 4089));
				return(S_OK);
			}

			bool mangler_found = false; // RA2: the first mangler entry clears the old list

			/*
			 * Walk the server list, sorting each entry into the appropriate global list
			 * by its connection label.
			 */
			while (tmp != NULL) {
				if (strcmp((char *)tmp->connlabel, "IRC") == 0) {
					if (chat_server_index == 0) {
						g_Servers.addTail(*tmp);
						g_CurrentServerIndex = 0;
						chat_server_index = 1;
					} else {
						g_Servers.addTail(*tmp);

						/*
						 * Look for a SKU tag matching our game in the comma-separated name
						 * field; if found, randomly favor this server (reservoir sampling)
						 * so all matching servers get an equal chance of being picked.
						 */
						char * nameToken;
						strcpy(namebuf, (char *)tmp->name);
						int sku = -1;
						nameToken = strtok(namebuf, ",");
						while (nameToken != NULL) {
							sku = atol(nameToken);
							if (sku == (unsigned char)g_GameSKU) {
								break;
							}
							if (strchr(nameToken, ':') != NULL) {
								break;
							}
							nameToken = strtok(NULL, ",");
						}

						if (sku == (unsigned char)g_GameSKU) {
							double pick_roll = rand();
							pick_roll /= 32767.0;
							double pick_chance = 1.0 / (double)++skumatchcount;
							if (pick_roll <= pick_chance) g_CurrentServerIndex = chat_server_index;
						}
						g_TargetServerIndex = g_CurrentServerIndex;
						chat_server_index++;
					}
				} else if (strcmp((char *)tmp->connlabel, "LAD") == 0) {

					/*
					 * Ladder server -- parse its host and port out of the connection data.
					 */
					char * host;
					char * port;
					strcpy(token, (char *)tmp->conndata);
					strtok(token, ";");
					host = strtok(NULL, ";");
					if (host != NULL) {
						strcpy(g_LadderAddress, host);
					}
					port = strtok(NULL, ";");
					if (port != NULL) {
						g_LadderPort = atol(port);
					}
				} else if (strcmp((char *)tmp->connlabel, "GAM") == 0 && game_srv_found == 0) {

					/*
					 * Primary game server -- parse its host and port out of the connection data.
					 */
					char * host;
					char * port;
					memcpy(&g_GameServer, tmp, sizeof(g_GameServer));
					strcpy(conndata, (char *)g_GameServer.conndata);
					strtok(conndata, ";");
					host = strtok(NULL, ";");
					if (host != NULL) {
						strcpy(g_GameServerHost, host);
					}
					port = strtok(NULL, ";");
					if (port != NULL) {
						g_GameServerPort = atol(port);
					}
					g_WDTServerHost[0] = '\0'; // RA2: no second results server until the list names one
					g_WDTServerPort = 0;
					game_srv_found = 1;
				} else if (strcmp((char *)tmp->connlabel, "GAM") == 0 && game_srv_found == 1) { // RA2: the second game server keeps the results for the tour

					char * host;
					char * port;
					memcpy(&g_GameServer, tmp, sizeof(g_GameServer));
					strcpy(conndata3, (char *)g_GameServer.conndata);
					strtok(conndata3, ";");
					host = strtok(NULL, ";");
					if (host != NULL) {
						strcpy(g_WDTServerHost, host);
					}
					port = strtok(NULL, ";");
					if (port != NULL) {
						g_WDTServerPort = atol(port);
					}
					game_srv_found = 2;
				} else if (strcmp((char *)tmp->connlabel, "WDT") == 0) {

					/*
					 * WDT (World Domination Tour) server -- parse host and port.
					 */
					char * host;
					char * port;
					memcpy(&g_WDTServer, tmp, sizeof(g_WDTServer));
					strcpy(conndata2, (char *)g_WDTServer.conndata);
					strtok(conndata2, ";");
					host = strtok(NULL, ";");
					if (host != NULL) {
						strcpy(g_WDTHost, host);
					}
					port = strtok(NULL, ";");
					if (port != NULL) {
						g_WDTPort = atol(port);
					}
				} else if (stricmp((char *)tmp->connlabel, "MGL") == 0) { // RA2: a mangler server, for the firewall detection
					DebugString("MGL entry in server list\n");
					if (!mangler_found) {
						memset(ManglerAddresses, 0, sizeof(ManglerAddresses));
						memset(ManglerPorts, 0, sizeof(ManglerPorts));
						ManglerCount = 0;
						mangler_found = true;
					}
					Server * mangler = new Server;
					memcpy(mangler, tmp, sizeof(Server));
					strcpy(manglerdata, (char *)mangler->conndata);
					strtok(manglerdata, ";");
					char * host = strtok(NULL, ";");
					if (host != NULL) {
						strcpy(ManglerAddresses[ManglerCount], host);
						char * port = strtok(NULL, ";");
						if (port != NULL) {
							ManglerPorts[ManglerCount] = atol(port);
						}
						DebugString("Got mangler server from servserv - address is %s ; %d\n", ManglerAddresses[ManglerCount], ManglerPorts[ManglerCount]);
						ManglerCount++;
					}
					delete mangler;
				}
				tmp = tmp->next;
			}

			/*
			 * Stamp the current login name and password onto every stored chat server entry.
			 */
			server = NULL;
			for (int i = 0; i < g_Servers.length(); i++) {
				g_Servers.getPointer(&server, i);
				strcpy((char *)server->login, g_NickName.get());
				strcpy((char *)server->password, g_LoginPassword.get());
			}

			DebugString("Got %d mangler servers\n", ManglerCount); // RA2: one mangler server is picked at random
			ManglerIndex = (ManglerCount > 1) ? Sim_Random_Pick(0, ManglerCount - 1) : 0;
			DebugString("CurrentManglerServer = %d\n", ManglerIndex);

			Set_Wait_Window_Text((LPARAM)StringTable::LoadString("TXT_DETECTING_FIREWALL_SETTINGS", NULL, "D:\\Ra2\\wonline.cpp", 4271)); // RA2: the firewall is checked next
			FirewallHelper.Save_Detected();
			SetEvent(g_WaitEventHandles[EV_GOT_SERVERS]);
			return(S_OK);
		}

		STDMETHOD(OnUpdateList)(HRESULT r, Update * updates)
		{
			int patch_total = 0;

			if (SUCCEEDED(r) && updates != NULL) {
				g_SuspendChatPump = TRUE;

				Update * tmp;
				for (tmp = updates; tmp != NULL; tmp = tmp->next) {
					patch_total++;
				}

				Close_Wait_Window(0x10);

				if (ODMessageBox(StringTable::LoadString("TXT_UPGRADEREQUIRED", NULL, "D:\\Ra2\\wonline.cpp", 4318), MB_YESNO, Wait_Callback_Thunk) == IDNO) {
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					return(S_OK);
				}

				char working_dir[260];
				GetCurrentDirectoryA(256, working_dir);

				int i = 0;
				tmp = updates;
				while (tmp != NULL) {
					char fullpath[260];
					char localfile[260];
					wchar_t updatestring[200];

					/*
					 * Work out the full file name and create the directory.
					 */
					sprintf(fullpath, "%s/%s", tmp->patchpath, tmp->patchfile);
					sprintf(localfile, "%s\\%s", tmp->localpath, tmp->patchfile);
					CreateDirectoryA((char *)tmp->localpath, NULL);
					g_pDownload->DownloadFile((char *)tmp->server, (char *)tmp->login, (char *)tmp->password, fullpath, localfile, APP_REG_KEY);
					i++;
					swprintf(updatestring, StringTable::LoadString("TXT_DOWNLOADING_X_OF_Y", NULL, "D:\\Ra2\\wonline.cpp", 4340), i, patch_total);
					WideWstring title(updatestring);
					SimpleWonlineDialogControl control;
					HWND dlg = OwnerDraw::Begin_Dialog(0xD3, (DLGPROC)WOL_Download_Dialog_Proc, 0);
					HWND item = GetDlgItem(dlg, 0x5C7);
					SendMessageA(item, 0x4B2, 0, (LPARAM)title.text());
					if (WS_Wait_Dialog(dlg, Wait_Callback_Thunk, false, true, true) == IDCANCEL) {
						// Download failed
						SetEvent(g_WaitEventHandles[EV_ABORT]);
						SetCurrentDirectoryA(working_dir);
						return(S_OK);
					}
					tmp = tmp->next;
				}

				ODMessageBox(StringTable::LoadString("TXT_GAME_RESTART", NULL, "D:\\Ra2\\wonline.cpp", 4372), MB_OK, Wait_Callback_Thunk);
				SetEvent(g_WaitEventHandles[EV_QUITGAME]);
				SetCurrentDirectoryA(working_dir);
				return(S_OK);
			}

			// An error occurred, bail out
			SetEvent(g_WaitEventHandles[EV_ABORT]);
			return(S_OK);
		}

		STDMETHOD(OnSquadInfo)(HRESULT res, unsigned long id, Squad * squad)
		{
			g_SquadInfoRequests--;
			if (g_RequestingOwnSquadInfo) {
				g_RequestingOwnSquadInfo = 0;
				g_OwnSquadID = id;
				if (res != S_OK) return(S_OK);
				strcpy((char *)g_OwnSquadName, (char *)squad->abbreviation);
			} else if (res != S_OK) {
				return(S_OK);
			}

			for (int i = 0; i < g_UserList.length(); i++) {
				User * user = NULL;
				g_UserList.getPointer(&user, i);
				if (user->squadID == id) {
					strcpy((char *)user->squadabbrev, (char *)squad->abbreviation);
					strcpy((char *)user->squadname, (char *)squad->name);
				}
			}

			Wstring ladder_name((char *)squad->abbreviation);
			ladder_name.toLower();
			if (!g_BattleClansLadderDict.contains(ladder_name)) {
				g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, (char *)squad->abbreviation, 0x802100, -1, 0, 0);
			}

			if (g_SquadInfoRequests == 0) Draw_Player_List(0, false);
			return(S_OK);
		}

		STDMETHOD(OnLogout)(HRESULT r, User * user)
		{
			if (SUCCEEDED(r)) {
				Handle_User_Leave(*user);
			}
			return(S_OK);
		}

		STDMETHOD(OnServerError)(HRESULT, LPCSTR msg)
		{
			if (!g_PlayingNetGame) {
				wchar_t buffer[512];
				swprintf(buffer, L"%s : %hs", StringTable::LoadString("TXT_SERVER_ERROR", NULL, "D:\\Ra2\\wonline.cpp", 4456), msg);
				Lobby_Chat_Printf(-1, buffer);
			}
			return(S_OK);
		}

		STDMETHOD(OnConnection)(HRESULT r, const char * motd)
		{
			static BOOL icons_loaded = FALSE;

			g_OnConnectionResult = r;

			/// A failed connection stops the periodic network-status polling.
			if (FAILED(r)) {
				g_IgnoreNetStatus = true;
			}

			switch (r) {
				/// Various failure reasons -- store a localized error message and abort.
				case CHAT_E_NICKINUSE:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_LOGIN_USED", NULL, "D:\\Ra2\\wonline.cpp", 4538));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				case CHAT_E_BADPASS:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_BADPASS", NULL, "D:\\Ra2\\wonline.cpp", 4543));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				case CHAT_E_BANNED:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_BANNED", NULL, "D:\\Ra2\\wonline.cpp", 4548));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				case CHAT_E_DISABLED:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_DISABLED", NULL, "D:\\Ra2\\wonline.cpp", 4554));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				case CHAT_E_SERIALBANNED:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_BANNED", NULL, "D:\\Ra2\\wonline.cpp", 4559));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				case CHAT_E_SERIALDUP:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_SERIALDUP", NULL, "D:\\Ra2\\wonline.cpp", 4564));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				case CHAT_E_SERIALUNKNOWN:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_SERIALUNKNOWN", NULL, "D:\\Ra2\\wonline.cpp", 4569));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				case CHAT_E_SKUSERIALMISMATCH:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_SKUSERIALMISMATCH", NULL, "D:\\Ra2\\wonline.cpp", 4574));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				default:
					wcscpy(g_LastErrorMessage, StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 4580));
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					break;
				case S_OK: {
					/*
					 * First successful connection this session -- fetch and cache the
					 * icon bitmap for every game type.
					 */
					if (icons_loaded == 0) {
						LPCSTR gametype_list;
						g_pChat->GetGametypeList(&gametype_list);
						char * buffer = new char[strlen(gametype_list) + 32];
						strcpy(buffer, gametype_list);
						strcat(buffer, ",-1,0,18");
						char * cptr = strtok(buffer, ",");
						while (cptr) {
							int gtype = atol(cptr);
							LPCSTR name;
							LPCSTR url;
							unsigned char * bitmap;
							int bmp_bytes;
							g_pChat->GetGametypeInfo(gtype, g_ListIconHeight, &bitmap, &bmp_bytes, &name, &url);
							char bitmap_name[64];
							sprintf(bitmap_name, "gt%d.bmp", gtype);
							SurfaceCache.CacheBMP(bitmap_name, bitmap, bmp_bytes, 2);
							cptr = strtok(NULL, ",");
						}
						delete[] buffer;
						icons_loaded = TRUE;
					}
					/// Stash the message-of-the-day for later display, if requested.
					if (g_ShowMOTD) {
						g_MessageOfTheDay = motd;
					} else {
						g_MessageOfTheDay.clear();
					}
					/*
					 * Reset the current channel state and mark login as complete.
					 */
					g_ConnectionLost = false;
					g_ChannelCount = 0;
					memset(&g_CurrentChannel, 0, sizeof(Channel));
					memset(&g_PendingChannel, 0, sizeof(Channel));
					SetEvent(g_WaitEventHandles[EV_CONNECTED]);
					Find_Local_Address(&Session.LocalAddress);
				} break;
			}
			return(S_OK);
		}

		STDMETHOD(OnMessageOfTheDay)(HRESULT r, const char * motd)
		{
			wchar_t line[1024];

			if (FAILED(r)) {
				return(S_OK);
			}

			char * buffer = strdup(motd);
			char * cptr = strtok(buffer, "\n");
			while (cptr) {
				Char_To_Wide(line, cptr, -1);
				Lobby_Chat_Printf(ColorSystem, line);
				cptr = strtok(NULL, "\n");
			}
			free(buffer);
			Lobby_Chat_Printf(ColorSystem, L"\n");
			return(S_OK);
		}

		STDMETHOD(OnChannelCreate)(HRESULT r, struct Channel * channel)
		{
			char exinfo_out[80];

			if (FAILED(r)) {
				Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_CHANCREATE_FAILED", NULL, "D:\\Ra2\\wonline.cpp", 4626));
				g_IsChannelCreator = 0;
				g_CurrentChannel.name[0] = '\0';
				Close_Wait_Window(4);
				return(S_OK);
			}

			if (channel->type == 0) {
				Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_CREATED_CHAN", NULL, "D:\\Ra2\\wonline.cpp", 4636), Narrow_To_Wide((char *)channel->name));
			} else {
				memset(g_UserInfo, 0, sizeof(g_UserInfo));
				strcpy(g_UserInfo[0].name, g_NickName.get());
				g_UserInfo[0].house = Session.House;
				g_UserInfo[0].Unknown_1C = 0;
				g_UserInfo[0].color = Session.PrefColor;
				g_UserInfo[0].accepted = 1;
			}
			memcpy(&g_CurrentChannel, channel, sizeof(Channel));
			g_IsChannelCreator = 1;
			g_ChannelCount++;
			DebugString("Created channel\n");
			if (g_pChat && channel->type != 0) {
				memset(exinfo_out, 0, sizeof(exinfo_out));
				Encode_Channel_ExInfo(exinfo_out);
				exinfo_out[(sizeof(exinfo_out) / 2)] = 0;
				g_pChat->SetChannelExInfo(exinfo_out);
			}
			Close_Wait_Window(4);
			return(S_OK);
		}

		STDMETHOD(OnChannelList)(HRESULT r, Channel * channels)
		{
			Wstring name;
			char ladder_request_keys[512 + 1];
			memset(ladder_request_keys, 0, sizeof(ladder_request_keys));
			int counter = 0;
			ArrayList<Channel> pinged; // RA2: the game channels whose ping was known keep it in the new list

			if (g_PlayingNetGame) {
				return(S_OK);
			}

			int requested_type = 0; // RA2: the chat channels

			/*
			 * A ping-list reply is always the game channel list; otherwise the list type
			 * comes off the pending request queue.
			 */
			if (r == CHAT_S_PINGLIST) {
				requested_type = 0x21; // RA2: the game channels
			} else {
				g_ChanListQueue.removeHead(requested_type);

				if (FAILED(r)) {
					return(S_OK);
				}

				if (requested_type == 0) {
					g_UserChannels.clear();
				} else {
					DebugString("clearing game list\n");
					Channel chan;
					memset(&chan, 0, sizeof(chan));
					for (int i = 0; i < g_GameChannelList.length(); i++) {
						g_GameChannelList.get(chan, i);
						if (chan.latency != -1) {
							pinged.addTail(chan);
						}
					}
					g_GameChannelList.clear();
				}
			}

			/*
			 * Split the server's channel list into chat channels and game channels,
			 * queuing lookups for each game channel's name along the way.
			 */
			while (channels != NULL) {
				if (channels->type == 0) {
					g_UserChannels.addTail(*channels);
				} else if (r == CHAT_S_PINGLIST) { // RA2: the pings of the game channels already listed
					if (channels->latency != -1) {
						Channel * game = NULL;
						for (int i = 0; i < g_GameChannelList.length(); i++) {
							g_GameChannelList.getPointer(&game, i);
							if (game && strcmp((char *)game->name, (char *)channels->name) == 0) {
								DebugString("Setting ping %d for %s\n", channels->latency, channels->name);
								game->latency = channels->latency;
								break;
							}
						}
					}
				} else {
					g_GameChannelList.addTail(*channels);
					for (int i = 0; i < pinged.length(); i++) {
						Channel old;
						pinged.get(old, i);
						if (strcmp((char *)old.name, (char *)channels->name) == 0) {
							DebugString("restoring ping %d for channel %s\n", old.latency, channels->name);
							channels->latency = old.latency;
							break;
						}
					}

					name.set((char *)channels->name);
					name.truncate('\'');
					name.toLower();
					if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
						Wstring temp;
						bool found = false;
						for (int i = 0; i < g_WDTTeamLookupQueue.length(); i++) {
							g_WDTTeamLookupQueue.get(temp, i);
							if (temp == name) {
								found = true;
								break;
							}
						}
						if (!found) {
							g_WDTTeamLookupQueue.addHead(name);
						}
					}

					if (!g_UserLocales.contains(name)) {
						Wstring temp;
						bool found = false;
						for (int i = 0; i < g_UserLocaleLookupQueue.length(); i++) {
							g_UserLocaleLookupQueue.get(temp, i);
							if (temp == name) {
								found = true;
								break;
							}
						}
						if (!found) {
							g_UserLocaleLookupQueue.addHead(name);
						}
					}

					/// Batch up ladder-list requests for names not yet cached.
					if (!g_ActiveLadder->contains(name)) {
						strcat(ladder_request_keys, name.get());
						strcat(ladder_request_keys, ":");
						counter++;
					}
				}

				/*
				 * Flush the batch once it gets large, to keep the request packet a
				 * reasonable size.
				 */
				if (counter > 20) {
					g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, ladder_request_keys, g_GameSKU & 0xFFFF00, -1, 0, 0);
					counter = 0;
					memset(ladder_request_keys, 0, sizeof(ladder_request_keys) - 1);
				}
				channels = channels->next;
			}

			/// Flush any remaining names once the list is exhausted.
			if (counter != 0) {
				g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, ladder_request_keys, g_GameSKU & 0xFFFF00, -1, 0, 0);
				memset(ladder_request_keys, 0, sizeof(ladder_request_keys) - 1);
			}

			SetEvent(g_WaitEventHandles[EV_CHAN_LIST]);

			if (Is_Tournament_Session()) { // RA2: no lists are shown in a tournament
				DebugString("Received channel list, user list is %d long\n", g_UserList.length());
			} else {
				HWND buddy_dialog = WS_Find_Dialog(0x116); // RA2: the buddy list shows where the buddies are
				if (buddy_dialog) {
					Fill_Buddy_List(buddy_dialog);
				}
				if (requested_type == 0) {
					Refresh_Lobby_List();
					HWND lobby = WS_Find_Dialog(0x113); // RA2: the lobby's list button works again
					if (lobby) {
						KillTimer(lobby, 0x55D);
						EnableWindow(GetDlgItem(lobby, 0x55D), TRUE);
					}
				} else {
					Refresh_Game_List();
				}
			}

			return(S_OK);
		}
#ifdef RA2_NON_MATCHING // not exact: 17 bytes differ in 16 instructions, one byte shorter (1871 by position); ra2-decomp's twin text (Yuri's Revenge 0x007A5890, matched there) in this tree's names with RA2's statements; calls, frame and every other instruction equal. In the expanded ArrayList<User>::add the memmove source is formed in place (add ecx, edx) where the executable takes a new register (lea eax, [edx + ecx]), and three scratch registers after it follow. The order moves with the number of memory references in the function (scratch probes: 29 to 33 more load-and-store statements after the insertion give every instruction of the executable); no reference form found for them
		STDMETHOD(OnChannelJoin)(HRESULT result, Channel * channel, User * user) // RA2: somebody (the player included) joined a channel, or a join failed (ra2-decomp's twin text, Yuri's Revenge 0x007A5890, in this tree's names)
		{
			if (result == 0x8004006C) {
				return(S_OK);
			}
			int i;
			int players;
			wchar_t joinname[64];
			joinname[0] = 0;
			if (channel) {
				Char_To_Wide(joinname, (char *)channel->name, -1);
				if (strncmp((char *)channel->name, "Lob_33_", 7) == 0) {
					int lobby = atol((char *)channel->name + strlen("Lob_33_"));
					int d = lobby / 8;
					int m = lobby % 8;
					if (d == 0) {
						swprintf(joinname, L"%s", g_Lobbies[m]);
					} else {
						swprintf(joinname, L"%s %d", g_Lobbies[m], d + 1);
					}
				}
			}
			if (result < 0) {
				if (result == 0x80040070) {
					Channel * chan = NULL;
					for (i = 0; i < g_UserChannels.length(); i++) {
						g_UserChannels.getPointer(&chan, i);
						if (strcmp((char *)g_PendingChannel.name, (char *)chan->name) == 0) {
							g_UserChannels.remove(i);
							break;
						}
					}
					for (i = 0; i < g_GameChannelList.length(); i++) {
						g_GameChannelList.getPointer(&chan, i);
						if (strcmp((char *)g_PendingChannel.name, (char *)chan->name) == 0) {
							g_GameChannelList.remove(i);
							break;
						}
					}
					if (Is_Tournament_Session()) {
						Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_GAME_CLOSED", NULL, "D:\\Ra2\\wonline.cpp", 0x128A));
						SetEvent(g_WaitEventHandles[5]);
					} else {
						ODMessageBox(StringTable::LoadString("TXT_GAME_CLOSED", NULL, "D:\\Ra2\\wonline.cpp", 0x128D), 0, Wait_Callback_Thunk, false);
					}
				} else if (result == 0x80040071) {
					if (Is_Tournament_Session()) {
						Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_BADPASS", NULL, "D:\\Ra2\\wonline.cpp", 0x1294));
						SetEvent(g_WaitEventHandles[5]);
					} else {
						ODMessageBox(StringTable::LoadString("TXT_BADPASS", NULL, "D:\\Ra2\\wonline.cpp", 0x1297), 0, Wait_Callback_Thunk, false);
					}
				} else if (result == 0x8004006E) {
					if (Is_Tournament_Session()) {
						Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_CHANNEL_FULL", NULL, "D:\\Ra2\\wonline.cpp", 0x129E));
						SetEvent(g_WaitEventHandles[5]);
					} else {
						ODMessageBox(StringTable::LoadString("TXT_CHANNEL_FULL", NULL, "D:\\Ra2\\wonline.cpp", 0x12A1), 0, Wait_Callback_Thunk, false);
						if (g_PendingChannel.type == 0) {
							SetEvent(g_WaitEventHandles[5]);
						}
					}
				} else if (result == 0x80040072) {
					ODMessageBox(StringTable::LoadString("TXT_JOINBAN", NULL, "D:\\Ra2\\wonline.cpp", 0x12AA), 0, Wait_Callback_Thunk, false);
					SetEvent(g_WaitEventHandles[5]);
					g_LobbyNumber++;
				} else {
					Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_CANT_JOINCHAN", NULL, "D:\\Ra2\\wonline.cpp", 0x12B1), result);
					SetEvent(g_WaitEventHandles[5]);
				}
				g_JoinLobbyNow = (g_PendingChannel.type != 0);
				g_IsChannelCreator = 0;
				g_CurrentChannel.name[0] = 0;
				memset(&g_PendingChannel, 0, sizeof(Channel));
				Close_Wait_Window(2);
				g_PendingChannel.name[0] = 0;
				Refresh_Game_List();
				Draw_Player_List(0, false);
				SetEvent(g_WaitEventHandles[10]);
				HWND main = WS_Find_Dialog(0x113);
				if (main) {
					ShowWindow(main, SW_SHOW);
					OwnerDraw::Set_Paint_Disabled(main, 0);
				}
				return(S_OK);
			}

			g_PendingChannel.name[0] = 0;
			if (user->flags & 0x8000) {
				memcpy(&g_CurrentChannel, channel, sizeof(Channel));
				memset(&g_PendingChannel, 0, sizeof(Channel));
				Close_Wait_Window(2);
				g_ChannelCount++;
				DebugString("Joined Channel\n");
				if (!Is_Tournament_Session()) {
					Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_JOINED_S", NULL, "D:\\Ra2\\wonline.cpp", 0x12E1), joinname);
				}
				if (channel->type == 0) {
					g_LobbyNumber = atol((char *)channel->name + strlen("Lob_33_"));
					HWND main = WS_Find_Dialog(0x113);
					if (main) {
						EnableWindow(GetDlgItem(main, 0x53D), TRUE);
						bool join = false;
						bool observe = false;
						HWND list = GetDlgItem(main, 0x6EE);
						int index = SendMessage(list, LB_GETCURSEL, 0, 0);
						if (index != LB_ERR) {
							index = SendMessage(list, LB_GETITEMDATA, index, 0);
							if (index >= 0 && index < g_GameChannelList.length()) {
								Channel choice;
								memset(&choice, 0, sizeof(choice));
								g_GameChannelList.get(choice, index);
								join = Can_Join_Channel(&choice, false, false);
								observe = Can_Join_Channel(&choice, false, true);
							}
						}
						EnableWindow(GetDlgItem(main, 0x6EE), TRUE);
						EnableWindow(GetDlgItem(main, 0x62D), join || observe);
						EnableWindow(GetDlgItem(main, 0x62B), TRUE);
						EnableWindow(GetDlgItem(main, 0x689), TRUE);
						EnableWindow(GetDlgItem(main, 0x558), TRUE);
						EnableWindow(GetDlgItem(main, 0x61F), TRUE);
						EnableWindow(GetDlgItem(main, 0x686), TRUE);
						SetFocus(GetDlgItem(main, 0x540));
					}
				}
				if (channel->type == 33) {
					Session.MPGameMode = First_Game_Mode();
					Session.ScenarioMaxPlayers = 8;
					LobbyObserverSlot = 7;
					if (Session.MPGameMode) {
						Session.MPGameMode->vt_entry_24();
					}
					Clear_Vector(&Session.Players);
					Clear_Slot_Lists();
					Reset_Slots();
					NodeNameType * node = new NodeNameType;
					memset(node, 0, sizeof(NodeNameType));
					wcscpy(node->Name, Narrow_To_Wide((char *)user->name));
					node->Address = User_To_IPX_Address(user);
					node->Set_Country(-2);
					node->Set_Color(-2);
					if (!Is_Tournament_Session() && ObserverMode) {
						node->Set_Country(-3);
						node->Slot.SpectatorFlag = -1;
					}
					Session.Players.Add(node);
					Session.GameName[0] = 0;
					if (!Is_Tournament_Session()) {
						Darken_Surface(NULL, true);
						HWND dlg = WS_Create_Dialog(ProgramInstance, 0xC9, MainWindow, WOL_Guest_Dialog_Proc, FALSE);
						Show_Dialog(dlg, 0xC9);
						HWND main = WS_Find_Dialog(0x113);
						if (main) {
							ShowWindow(main, SW_HIDE);
							OwnerDraw::Set_Paint_Disabled(main, 0);
						}
						SendMessage(dlg, 0x4A9, 0, 1);
						ShowWindow(dlg, SW_NORMAL);
						memset(g_PlayerPings, -1, 8 * sizeof(int));
						if (user->squadID != 0 && strlen((char *)user->squadabbrev) == 0) {
							g_SquadInfoRequests++;
							g_pChat->RequestSquadInfo(user->squadID);
						}
					}
				}
			} else {
				if (g_CurrentChannel.type == 33) {
					VocClass::PlayGlobal(Rule->PlayerJoined, 1.0f, 0x2000, 0);
				}
				if (channel->type != 0 && (g_GameStartRequested || WolOptionsLocked != 0 || g_StartGameReceived)) {
					g_pChat->RequestUserKick(user);
					return(S_OK);
				}
				if (user->squadID != 0 && strlen((char *)user->squadabbrev) == 0) {
					g_SquadInfoRequests++;
					g_pChat->RequestSquadInfo(user->squadID);
				}
				{
					Wstring ladder_name;
					ladder_name.set((char *)user->name);
					ladder_name.toLower();
					if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
						Wstring name;
						bool found = false;
						for (i = 0; i < g_WDTTeamLookupQueue.length(); i++) {
							g_WDTTeamLookupQueue.get(name, i);
							if (name == ladder_name) {
								found = true;
								break;
							}
						}
						if (!found) {
							g_WDTTeamLookupQueue.addHead(ladder_name);
						}
					}
					if (!g_UserLocales.contains(ladder_name)) {
						Wstring name;
						bool found = false;
						for (i = 0; i < g_UserLocaleLookupQueue.length(); i++) {
							g_UserLocaleLookupQueue.get(name, i);
							if (name == ladder_name) {
								found = true;
								break;
							}
						}
						if (!found) {
							g_UserLocaleLookupQueue.addHead(ladder_name);
						}
					}
					if (!g_ActiveLadder->contains(ladder_name)) {
						g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, ladder_name.get(), g_GameSKU & 0xFFFF00, -1, 0, 0);
					}
				}
				if (channel->type != 0) {
					for (i = 1; i < sizeof(g_UserInfo) / sizeof(ChannelUserInfo); i++) {
						g_UserInfo[i].accepted = 0;
						EnableWindow(GetDlgItem(GameoptWindow(), 0x59F), TRUE);
					}
					if (!g_IsChannelCreator) {
						HWND dialog = WS_Find_Dialog(0xC9);
						Enable_Player_Combos(dialog, true);
					}
					Enable_Start_Button();
				}
			}

			int index = g_UserList.length();
			User temp_user;
			for (i = 0; i < g_UserList.length(); i++) {
				g_UserList.get(temp_user, i);
				if (Compare_Users(user, &temp_user) < 0) {
					index = i;
					break;
				}
			}
			User newuser = *user;
			newuser.next = NULL;
			g_UserList.add(newuser, index);
			if (!(user->flags & 0x8000) && channel->type == 0 && Is_Tournament_Session()) {
				Wstring name((char *)user->name);
				g_PingQueue.addTail(name);
				if (g_PingsPending.length() == 0) {
					Service_Pending_Player_Pings();
				}
			}
			players = g_UserList.length();
			int userinfoindex = 0;
			if (channel->type != 0) {
				{
					Wstring name((char *)user->name);
					userinfoindex = Player_Name_To_Index(name);
				}
				memmove(&g_UserInfo[userinfoindex + 1], g_UserInfo + userinfoindex, sizeof(ChannelUserInfo) * (15 - userinfoindex));
				memset(&g_UserInfo[userinfoindex], 0, sizeof(ChannelUserInfo));
				strcpy(g_UserInfo[userinfoindex].name, (char *)user->name);
				if (g_IsChannelCreator == 1) {
					if (!Is_Tournament_Session()) {
						Update_Channel_Player_Limit();
					}
					if (Is_All_Slots_Filled() && Is_Observer_Slot_Full()) {
						Wstring name((char *)user->name);
						Kick_Channel_User(name);
					} else {
						int color = 0;
						int taken = 0;
						while (true) {
							g_UserInfo[userinfoindex].color = color;
							taken = 0;
							for (i = 0; i < players; i++) {
								if (i != userinfoindex && g_UserInfo[i].color == color) {
									color++;
									taken = 1;
								}
							}
							if (taken == 0) {
								break;
							}
						}
						NodeNameType * node = new NodeNameType;
						memset(node, 0, sizeof(NodeNameType));
						wcscpy(node->Name, Narrow_To_Wide((char *)user->name));
						node->Address = User_To_IPX_Address(user);
						if (Session.MPGameMode) {
							node->Set_Country(Session.MPGameMode->DefaultCountry());
						} else {
							node->Set_Country(-2);
						}
						node->Set_Color(-2);
						if (!Is_Tournament_Session() && Is_All_Slots_Filled()) {
							node->Set_Country(-3);
							node->Set_Color(-2);
							node->Slot.SpectatorFlag = -1;
						}
						if ((user->flags & 0x8000) && !Is_Tournament_Session() && ObserverMode) {
							node->Set_Country(-3);
							node->Set_Color(-2);
							node->Slot.SpectatorFlag = -1;
						}
						Session.Players.Add(node);
						Assign_Slot(node);
						if (Is_Tournament_Session() && channel->type != 0) {
							if (Online_Session_State != 2) {
								Net2DecodeGameopt(g_MatchStartText, NULL);
							} else {
								Apply_Endpoint_Map(strchr(g_MatchStartText, ':'));
							}
							for (i = 0; i < Session.Scenarios.Count(); i++) {
								char const * filename = Session.Scenarios[i]->Get_Filename();
								if (stricmp(Session.ScenarioFileName, filename) == 0) {
									Set_Scenario_Info_From_Index(i);
									break;
								}
							}
							if (i >= Session.Scenarios.Count()) {
								Set_Scenario_Info_From_Index(0);
							}
						}
						Enable_Start_Button();
						Handle_Mode_Selection();
						char options[3];
						options[0] = 'O';
						options[1] = (LobbyObserverSlot < 0 ? 8 : LobbyObserverSlot) + '0';
						options[2] = 0;
						DebugString("Sending observer slot of %c\n", options[1]);
						g_pChat->RequestPublicGameOptions(options);
						Send_Player_List();
						Send_Lobby_Options(true, false);
						if (Is_Host()) {
							Send_User_Pings();
							Request_All_User_Pings();
						}
					}
				} else if (!(user->flags & 0x8000)) {
					NodeNameType * node = new NodeNameType;
					memset(node, 0, sizeof(NodeNameType));
					wcscpy(node->Name, Narrow_To_Wide((char *)user->name));
					node->Address = User_To_IPX_Address(user);
					node->Set_Country(-2);
					node->Set_Color(-2);
					Session.Players.Add(node);
					HWND dialog = WS_Find_Dialog(0xC9);
					Enable_Player_Combos(dialog, true);
					Enable_Start_Button();
				}
			}
			SetEvent(g_WaitEventHandles[4]);
			Draw_Player_List(0, false);
			return(S_OK);
		}
#else
		STDMETHOD(OnChannelJoin)(HRESULT result, Channel * channel, User * user);
#endif
		STDMETHOD(OnChannelLeave)(HRESULT r, struct Channel *, struct User * user)
		{
			if (FAILED(r)) {
				DebugString("OnChannelLeave with error!\n");
				return(S_OK);
			}
			Handle_User_Leave(*user);
			return(S_OK);
		}
		STDMETHOD(OnChannelTopic)(HRESULT, struct Channel *, const char *) { return(S_OK); }
		STDMETHOD(OnPrivateAction)(HRESULT r, struct User * user, const char * action)
		{
			if (!g_PlayingNetGame && SUCCEEDED(r)) {
				bool allowed;
				{
					Wstring narrow(action); // RA2: the narrow text is copied around the visibility test and freed after it, as the original does (ra2-decomp's statement at the same place)
					allowed = Is_User_Message_Shown(user);
				}
				if (allowed) {
					WideWstring wide = Wstring_ToWide(action);
					if (_stricmp(Session.Handle, (char *)user->name)) {
						Lobby_Chat_Printf(ColorPrivAction, L"%hs %s", user->name, wide.get());
					}
				}
			}
			return(S_OK);
		}

		STDMETHOD(OnPrivateUnicodeAction)(HRESULT r, struct User * user, const unsigned short * action)
		{
			if (!g_PlayingNetGame && SUCCEEDED(r)) {
				bool shown;
				{
					WideWstring wide(action);
					shown = Is_Text_Shown((unsigned char const *)user, wide);
				}
				if (shown) {
					if (_stricmp(Session.Handle, (char *)user->name)) {
						Lobby_Chat_Printf(ColorPrivAction, L"%hs %s", user->name, action);
					}
				}
			}
			return(S_OK);
		}
		STDMETHOD(OnPublicAction)(HRESULT r, struct Channel *, struct User * user, const char * action)
		{
			if (!g_PlayingNetGame && SUCCEEDED(r)) {
				bool allowed;
				{
					Wstring narrow(action); // RA2: the narrow text is copied around the visibility test and freed after it, as the original does (ra2-decomp's statement at the same place)
					allowed = Is_User_Message_Shown(user);
				}
				if (allowed) {
					WideWstring wide = Wstring_ToWide(action);
					if (_stricmp(Session.Handle, (char *)user->name)) {
						Lobby_Chat_Printf(ColorAction, L"%hs %s", user->name, wide.get());
					}
				}
			}
			return(S_OK);
		}

		STDMETHOD(OnPublicUnicodeAction)(HRESULT r, struct Channel *, struct User * user, const unsigned short * action)
		{
			if (!g_PlayingNetGame && SUCCEEDED(r)) {
				bool shown;
				{
					WideWstring wide(action);
					shown = Is_Text_Shown((unsigned char const *)user, wide);
				}
				if (shown) {
					if (_stricmp(Session.Handle, (char *)user->name)) {
						Lobby_Chat_Printf(ColorAction, L"%hs %s", user->name, action);
					}
				}
			}
			return(S_OK);
		}
		STDMETHOD(OnUserList)(HRESULT r, struct Channel *, struct User * user)
		{
			if (g_PlayingNetGame) return(S_OK);
			static char ladder_request_keys[512];
			memset(ladder_request_keys, 0, sizeof(ladder_request_keys));
			Wstring ladder_name;
			if (FAILED(r)) return(S_OK);
			SendDlgItemMessageA(WS_Top_Window(), 0x53E, LB_RESETCONTENT, 0, 0);
			g_UserList.setEmpty();
			int batch_count = 0;
			int user_count = 0;
			bool saw_owner = false;
			while (user != NULL) {
				char buffer[128];
				user_count++;
				if ((user->squadID != 0) && (strlen((char *)user->squadabbrev) == 0)) {
					g_SquadInfoRequests++;
					g_pChat->RequestSquadInfo(user->squadID);
				} else if (g_pNetUtil && (user->squadID != 0) && (strlen((char *)user->squadabbrev) != 0)) {
					Wstring squad_name((char *)user->squadabbrev);
					squad_name.toLower();
					if (!g_BattleClansLadderDict.contains(squad_name)) {
						g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, (char *)user->squadabbrev, 0x802100, -1, 0, 0);
					}
				}
				unsigned int flags = user->flags;
				strcpy(buffer, (char *)user->name);
				User entry = *user;
				entry.next = NULL;
				g_UserList.addTail(entry);
				if (flags & 1) saw_owner = true;
				ladder_name.set((char *)user->name);
				ladder_name.toLower();
				if (Session.Type == GAME_INTERNET && Online_Session_State == 2 && !g_WDTTeams.contains(ladder_name)) {
					g_WDTTeamLookupQueue.addHead(ladder_name);
				}
				if (!g_UserLocales.contains(ladder_name)) {
					g_UserLocaleLookupQueue.addHead(ladder_name);
				}
				if ((g_ActiveLadder->contains(ladder_name) == FALSE) && (user_count < 100)) {
					ladder_name.set((char *)user->name);
					ladder_name.toLower();
					strcat(ladder_request_keys, ladder_name.get());
					strcat(ladder_request_keys, ":");
					batch_count++;
				}
				if (batch_count > 20) {
					g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, ladder_request_keys, g_GameSKU & 0xFFFF00, -1, 0, 0);
					batch_count = 0;
					memset(ladder_request_keys, 0, sizeof(ladder_request_keys));
				}
				if ((g_CurrentChannel.type != 0) && !g_IsChannelCreator) {
					Wstring name((char *)user->name);
					int index = Player_Name_To_Index(name);
					memset(&g_UserInfo[index], 0, sizeof(ChannelUserInfo));
					strcpy((char *)g_UserInfo[index].name, (char *)user->name);
					if (_stricmp((char *)user->name, g_NickName.get()) != 0) {
						if (Session.Players.Count() == 1) wcscpy(Session.GameName, Narrow_To_Wide((char *)user->name));
						NodeNameType * player = new NodeNameType;
						memset(player, 0, sizeof(NodeNameType));
						WideWstring player_name = Wstring_ToWide((char *)user->name);
						wcscpy(player->Name, player_name.get());
						player->Address = User_To_IPX_Address(user);
						player->Set_Country(-2);
						player->Set_Color(-2);
						Session.Players.Add(player);
					}
				}
				user = user->next;
			}
			if (strlen(ladder_request_keys)) {
				g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, ladder_request_keys, g_GameSKU & 0xFFFF00, -1, 0, 0);
			}
			if (g_CurrentChannel.type != 0) {
				if (saw_owner == false) {
					User entry;
					for (int i = 0; i < g_UserList.length(); i++) {
						if ((g_UserList.get(entry, i)) && (_stricmp((char *)entry.name, g_NickName.get()) == 0)) {
							Handle_User_Leave(entry);
							break;
						}
					}
				}
				if (g_CurrentChannel.type != 0) {
					if (!g_IsChannelCreator && g_UserList.length() == 1 && (_stricmp((char *)user->name, g_NickName.get()) == 0)) {
						Handle_User_Leave(*user);
					}
					if (g_CurrentChannel.type != 0) {
						IsColorChangePending = 1;
						if (!g_IsChannelCreator) {
							HWND guest = WS_Find_Dialog(0xC9);
							Enable_Player_Combos(guest, true);
						}
						Enable_Start_Button();
						WOLOptionsClass options;
						options.Load();
						rc_ptr<PlayerProfile> profile;
						if (options.Is_Bit1_Clear()) {
							profile = PlayerProfile::Load(g_NickName);
						} else {
							profile = g_PlayerProfile.Get();
						}
						Session.Set_Side(profile->Country, false);
						Session.Set_Color(profile->Color, false);
						if (ObserverMode) {
							if (!Is_Tournament_Session()) {
								Session.Set_Side(-3, false);
								Session.Set_Color(-2, false);
							}
						}
						if (!ObserverMode && !Is_Tournament_Session() && Session.Chosen_House() == -3) {
							if (Session.Players.Count() != 0) {
								Session.Set_Side(Session.Players[0]->Get_Country(), false);
								Session.Set_Color(Session.Players[0]->Get_Color(), false);
							} else {
								Session.Set_Side(-2, false);
								Session.Set_Color(-2, false);
							}
						}
						char options_text[128];
						{
							NetNumType network;
							NetNodeType node;
							unsigned short socket;
							Session.LocalAddress.Get_Address(network, node, &socket);
							sprintf(options_text, "R%d,%d,%x,%x,%x", Session.Chosen_House(), Session.Chosen_Color(), *(unsigned int *)node, FirewallHelper.Detected & 0xFFFF, Session.PortBase);
							DebugString("Sending private game options (%s)\n", options_text);
						}
						User host;
						if (g_UserList.get(host, 0)) g_pChat->RequestPrivateGameOptions(&host, options_text);
					}
				}
			}
			if (Is_Tournament_Session() && g_CurrentChannel.type == 0 && !Request_Match()) {
				SetEvent(g_WaitEventHandles[EV_ERROR]);
			}
			Draw_Player_List(0, false);
			return(S_OK);
		}
		STDMETHOD(OnPublicMessage)(HRESULT r, struct Channel *, struct User * user, const char * text)
		{
			if (!g_PlayingNetGame && SUCCEEDED(r)) {
				if (Is_Tournament_Session()) {
					if (user->flags & 1) {
						Handle_Matchmaking_Command(text, false);
					}
					return(S_OK);
				}
				bool allowed;
				{
					Wstring narrow(text); // RA2: the narrow text is copied around the visibility test and freed after it, as the original does (ra2-decomp's statement at the same place)
					allowed = Is_User_Message_Shown(user);
				}
				if (!allowed) return(S_OK);
				WideWstring wide = Wstring_ToWide(text);

				/*
				 * Channel owners get a distinct highlight color.
				 */
				int color = ColorUser;
				if (user->flags & 1) color = ColorOp;
				if (_stricmp(Session.Handle, (char *)user->name)) {
					Lobby_Chat_Printf(color, L"[%hs] : %s", user->name, wide.get());
				}
			}
			return(S_OK);
		}

		STDMETHOD(OnPublicUnicodeMessage)(HRESULT r, struct Channel *, struct User * user, const unsigned short * text)
		{
			if (g_PlayingNetGame) return(S_OK);
			if (FAILED(r)) {
				return(S_OK);
			}
			WideWstring wide(text);
			if (!Is_Text_Shown((unsigned char const *)user, wide)) return(S_OK);
			int color = ColorUser;
			if (user->flags & 1) color = ColorOp;
			if (_stricmp(Session.Handle, (char *)user->name)) {
				Lobby_Chat_Printf(color, L"[%hs] : %s", user->name, wide.get());
			}
			return(S_OK);
		}
		STDMETHOD(OnPrivateMessage)(HRESULT r, struct User * user, const char * text)
		{
			if (!g_PlayingNetGame && SUCCEEDED(r)) {
				if (Is_Tournament_Session()) {
					if (user->flags & 1) {
						Handle_Matchmaking_Command(text, true);
						return(S_OK);
					}
					DebugString("Received message from non-channel owner %s: %s\n", user->name, text);
					return(S_OK);
				}
				bool allowed;
				{
					Wstring narrow(text); // RA2: the narrow text is copied around the visibility test and freed after it, as the original does (ra2-decomp's statement at the same place)
					allowed = Is_User_Message_Shown(user);
				}
				if (!allowed) return(S_OK);
				WideWstring wide = Wstring_ToWide(text);

				/*
				 * Channel owners get a distinct highlight color.
				 */
				int color = ColorPriv;
				if (user->flags & 1) color = ColorOp;
				if (_stricmp(Session.Handle, (char *)user->name)) {
					Lobby_Chat_Printf(color, L"%hs : %s", user->name, wide.get());
				}
			}
			return(S_OK);
		}

		STDMETHOD(OnPrivateUnicodeMessage)(HRESULT r, struct User * user, const unsigned short * text)
		{
			if (g_PlayingNetGame) return(S_OK);
			if (FAILED(r)) {
				return(S_OK);
			}
			WideWstring wide(text);
			if (!Is_Text_Shown((unsigned char const *)user, wide)) return(S_OK);
			int color = ColorPriv;
			if (user->flags & 1) color = ColorOp;
			if (_stricmp(Session.Handle, (char *)user->name)) {
				Lobby_Chat_Printf(color, L"%hs : %s", user->name, wide.get());
			}
			return(S_OK);
		}
		STDMETHOD(OnSystemMessage)(HRESULT r, const char * text)
		{
			if (!g_PlayingNetGame && SUCCEEDED(r)) {
				Lobby_Chat_Printf(ColorSystem, L">>> %hs", text);
			}
			return(S_OK);
		}
		STDMETHOD(OnNetStatus)(HRESULT status)
		{
			if (g_IgnoreNetStatus) {
				SetEvent(g_WaitEventHandles[EV_NETIGNORE]);
				g_LastIgnoredStatus = status;
				return(S_OK);
			}

			if (status == CHAT_S_CON_DISCONNECTED) {
				if (g_PlayingNetGame) {
					if (!g_ConnectionLost) {
						wchar_t txt[256];
						txt[0] = '\0';
						swprintf(txt, StringTable::LoadString("TXT_WOL_CONNLOST", NULL, "D:\\Ra2\\wonline.cpp", 7177));
						int scheme = GadgetClass::Get_Color_Scheme();
						Session.Messages.Add_Message(0, 0, txt, scheme, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
						Map.Flag_To_Redraw(GS_REDRAW_DIRTY);
						g_ConnectionLost = true;
						g_ChannelCount = 0;
						memset(&g_CurrentChannel, 0, sizeof(Channel));
						memset(&g_PendingChannel, 0, sizeof(Channel));
						DebugString("NetStatus disconnect ingame\n");
					}
				} else {
					if (WS_Find_Dialog(0x113)) {
						ODMessageBox(StringTable::LoadString("TXT_YOURE_DISCON", NULL, "D:\\Ra2\\wonline.cpp", 7195), MB_OK, Wait_Callback_Thunk);
					}
					g_IgnoreNetStatus = true;
					g_ConnectionLost = true;
					SetEvent(g_WaitEventHandles[EV_ABORT]);
					g_ChannelCount = 0;
					memset(&g_CurrentChannel, 0, sizeof(Channel));
					memset(&g_PendingChannel, 0, sizeof(Channel));
					DebugString("NetStatus disconnect not in game \n");
				}
			}
			return(S_OK);
		}
		STDMETHOD(OnPublicGameOptions)(HRESULT r, struct Channel *, struct User * user, const char * options)
		{
			static BasicTimerClass<SystemTimerClass> _timer;

			if (FAILED(r)) {
				return(S_OK);
			}

			char * buffer = strdup(options); // RA2: the options are decoded from a copy
			bool scenario_changed = Net2DecodeGameopt(buffer, (char *)user->name);
			free(buffer);

			if (g_PlayingNetGame || WolOptionsLocked || options[0] == 'Z') {
				return(S_OK);
			}

			bool request_preview = false;

			if (Local_User_Is_Channel_Owner()) { // RA2: the host ignores its own options
				return(S_OK);
			}

			if (Is_Tournament_Session()) { // RA2: in a tournament game the guest tells the host it is ready once it has a side and a color
				WideWstring nickname = Wstring_ToWide(g_NickName);
				NodeNameType * local = Find_Lobby_Name(nickname.get());
				if (local == NULL) {
					return(S_OK);
				}
				if (local->Get_Country() < 0 || local->Get_Color() < 0) {
					return(S_OK);
				}
				if (options[0] < '0' || options[0] > '9') {
					return(S_OK);
				}
				if (g_UserList.length() == 2) {
					g_pChat->RequestPublicGameOptions("A1");
				}
				return(S_OK);
			}

			HWND dialog = WS_Top_Window();
			if (MultiplayerMapPreview == NULL) {
				if (scenario_changed) {
					request_preview = true;
					_timer = 0;
					DebugString("New scenario - requesting a map preview\n");
				}
				if (_timer > 900 && (MultiplayerMapPreview == NULL || MultiplayerMapPreview->Get_Preview_Surface() == NULL) && !g_ReceivingHostPreview) {
					if (Session.ScenarioFileName[0] && !Find_Local_Scenario(Session.ScenarioFileName, Session.ScenarioFileLength, Session.ScenarioDigest, Session.ScenarioIsOfficial, false)) {
						request_preview = true;
						DebugString("Never recieved preview; re-requesting\n");
					} else {
						_timer = 0;
					}
				}
				if (!Session.Is_Random_Map()) {
					bool exists = false;
					if (Find_Local_Scenario(Session.ScenarioFileName, Session.ScenarioFileLength, Session.ScenarioDigest, Session.ScenarioIsOfficial, false)) {
						{
							RawFileClass file(Session.ScenarioFileName);
							if (file.Is_Available()) {
								exists = true;
							}
						}
						if (!exists) {
							request_preview = false;
							goto update_dialog;
						}
					}
					if (Is_Current_Channel_Tournament() && Session.Players[0]->Slot.Unknown77) {
						Session.Players[0]->Slot.Unknown77 = 0;
						Send_Host_Option("A0");
						EnableWindow(GetDlgItem(dialog, 0x59F), FALSE);
					}
				update_dialog:
					if (WS_Top_Window_ID() == 0xC9) {
						InvalidateRect(WS_Top_Window(), NULL, FALSE);
					}
				}
			} else {
				HWND item = GetDlgItem(dialog, 0x3F2);
				if (item) {
					ShowWindow(item, SW_HIDE);
				}
				ShowWindow(GetDlgItem(dialog, 0x468), SW_SHOWNORMAL);
			}

			if (request_preview) { // RA2: ask the host for the map
				char options_text[64];
				sprintf(options_text, "P%s", Session.ScenarioFileName);
				User host;
				g_UserList.getHead(host);
				g_pChat->RequestPrivateGameOptions(&host, options_text);
				DebugString("Sent private game option packet requesting a preview download\n");
				Poke_The_Host();
				return(S_OK); // RA2: returns here as well, as ra2-decomp's twin and main do
			}
			return(S_OK);
		}
		STDMETHOD(OnPrivateGameOptions)(HRESULT, struct User * user, const char * options)
		{
			if (g_PlayingNetGame) return(S_OK);

			if (WolOptionsLocked && options[0] != 'Z') return(S_OK); // RA2: the port messages still come once the options are locked

			if (Is_Tournament_Session() && options[0] != 'Z') return(S_OK); // RA2: a tournament game has no options to ask for

			int index = Player_Name_To_Index((char *)user->name);
			if (index == -1) return(S_OK);

			/// Dispatch on the request type encoded in the first character.
			switch (options[0]) {

				case 'Z': // RA2: a guest tells the host the port to send to
					if (options[1] == '4') {
						unsigned int port = atol(options + 2);
						DebugString("Player %s is telling us we should send to port %d\n", user->name, port & 0xFFFF);
						for (int i = 0; i < 8; i++) {
							if (stricmp(Wide_To_Narrow(Session.Slots[i]->Name), (char *)user->name) == 0) {
								Session.PeerPorts[i] = (unsigned short)port;
								break;
							}
						}
					}
					break;

				case 'R': {
					if (g_IsChannelCreator != 1) break;

					char * buffer = strdup(options + 1);

					/*
					 * RA2: find the guest's slot by its name.
					 */
					int index = -1;
					int row = 0;
					for (; row < 8; row++) {
						Wstring name = Wstring_Narrow(Session.Slots[row]->Name);
						if (stricmp(name.get(), (char *)user->name) == 0) {
							index = row;
							break;
						}
					}

					if (index != -1) {
						char * token = strtok(buffer, ",");
						int country = 0;
						if (token) country = atol(token);

						token = strtok(NULL, ",");
						int color = 0;
						if (token) color = atol(token);

						/*
						 * RA2: the guest's local address, its firewall and its port base.
						 */
						token = strtok(NULL, ",");
						unsigned int ip = 0;
						sscanf(token, "%X", &ip);
						IPXAddressClass address;
						if (ip) {
							NetNumType net;
							NetNodeType node;
							memset(net, 0, sizeof(net));
							memset(node, 0, sizeof(node));
							memcpy(node, &ip, sizeof(ip));
							address.Set_Address(net, node, 0);
							Session.SlotAddresses[row] = address;
						}

						token = strtok(NULL, ",");
						unsigned int firewall = 0;
						if (token) sscanf(token, "%X", &firewall);
						Session.WolPlayerState[row] = firewall;

						token = strtok(NULL, ",");
						unsigned int port_base = 0;
						if (token) sscanf(token, "%X", &port_base);
						Session.PlayerPortBase[row] = port_base;

						DebugString("Got private options from %s: House=%d, Color=%d, LocalAddress=%s, firewall=%08X, port_base=%d\n", user->name, country, color, address.As_String(NULL), Session.WolPlayerState[row], port_base);

						HWND window = WS_Find_Dialog(0xC2);
						if (!window) window = WS_Find_Dialog(0xC9);

						int old_country = Session.Slots[row]->Get_Country();
						NodeNameType * player = Session.Slots[row];
						bool become_observer = false;
						bool become_player = false;
						bool changed = false;
						if (old_country == -3) {
							if (country != old_country) become_player = true;
						} else if (country == -3) {
							become_observer = true;
						}

						/*
						 * RA2: the host's own slot stays a player when the game mode needs it.
						 */
						User * host = Find_Owner_User();
						if (host && stricmp((char *)host->name, g_NickName.text()) == 0 && row == 0 && become_observer && Session.MPGameMode && !Session.MPGameMode->vt_entry_A0()) {
							become_observer = false;
							changed = true;
						}

						if (become_player) { // RA2: an observer takes a player slot
							host = Find_Owner_User();
							if (host && stricmp((char *)host->name, g_NickName.text()) == 0) {
								if (Is_All_Slots_Filled()) {
									Change_Slot_Type(window, Get_Slot_Control_Id(row), LobbyEntries[6], 0);
									changed = true;
									Send_Player_List();
									Send_Lobby_Options(true, false);
									Draw_Player_List(0, false);
								} else {
									player->Set_Country(-2);
									player->Slot.SpectatorFlag = 0;
									Change_Slot_Type(window, Get_Slot_Control_Id(LobbyObserverSlot), LobbyEntries[6], 0);
									index = Assign_Slot(player);
									Change_Slot_Type(window, Get_Slot_Control_Id(index), player, 0);
									Send_Player_List();
								}
							}
						}

						if (become_observer) { // RA2: a player takes the observer slot
							host = Find_Owner_User();
							if (host && stricmp((char *)host->name, g_NickName.text()) == 0) {
								if (Is_Observer_Slot_Full()) {
									Change_Slot_Type(window, Get_Slot_Control_Id(index), LobbyEntries[0], 0);
									Send_Player_List();
								} else {
									player->Set_Country(-3);
									player->Slot.SpectatorFlag = -1;
									Change_Slot_Type(window, Get_Slot_Control_Id(index), LobbyEntries[0], 0);
									Change_Slot_Type(window, Get_Slot_Control_Id(LobbyObserverSlot), player, 0);
									Send_Player_List();
									Fill_Observer_Combo(window, Player_Side_Combo_ID(LobbyObserverSlot));
									int observer_control = Get_Slot_Control_Id(LobbyObserverSlot);
									SendMessage(GetDlgItem(window, observer_control), 0x4F1, 0, 1);
									int control = Player_Color_Combo_ID(LobbyObserverSlot);
									Fill_Random_Color_Combo(window, control);
									Select_Color_Combo(window, control, 8);
								}
								goto refresh_options;
							}
						}

						if (changed) goto request_done;

						bool country_changed = old_country != country;
						if (Session.MPGameMode && !Session.MPGameMode->IsCountryAllowed(country)) {
							country = Session.MPGameMode->IsCountryAllowed(old_country) ? old_country : -2;
							country_changed = true;
						}
						if (country_changed) Session.Slots[index]->Set_Country(country);

						bool color_changed = Session.Slots[index]->Get_Color() != color;
						if (color_changed) {
							if (Session.Is_Color_Used(color)) color = -2;
							Session.Slots[index]->Set_Color(color);
						}

						if (!country_changed && !color_changed) goto request_done;
					} else {
						goto request_done;
					}
				refresh_options:
					Send_Lobby_Options(true, false);
					Draw_Player_List(0, false);
				request_done:
					free(buffer);
					break;
				}

				case 'P': {
					if (g_IsChannelCreator != 1) break;

					DebugString("Received private preview request from channel guest\n");
					if (g_UserList.length() >= 2) {
						User temp;

						/*
						 * Only honor the request if we are genuinely hosting a
						 * random-map game with a preview ready to send.
						 */
						User * host = Find_Owner_User();
						if (host && stricmp((char *)host->name, g_NickName.text()) == 0 && !WolOptionsLocked && g_CurrentChannel.type != 0 && MultiplayerMapPreview != NULL &&
							MultiplayerMapPreview->Get_Preview_Surface() != NULL && IsRandomMap) {
							int i = Player_Name_To_Index((char *)user->name);
							if (i != -1) {
								g_UserList.get(temp, i);
								temp.next = NULL;
								User_List_To_Session_Players(&temp);
								int old = g_SuspendChatPump;
								g_SuspendChatPump = true;
								Send_Map_Preview(MultiplayerMapPreview);
								g_SuspendChatPump = old;
							}
						}
					}
					break;
				}
			}
			return(S_OK);
		}
		STDMETHOD(OnGameStart)(HRESULT, struct Channel * channel, struct User * users, int gameid)
		{
			bool found = true;
			int count = g_UserList.length();
			int listed = 0;

			g_PageTimer = CDTimerClass<SystemTimerClass>(0);
			if (g_StartGameReceived) return(S_OK);
			g_SuspendChatPump = TRUE;

			/*
			 * Every user the server lists must be one of ours.
			 */
			User * user = users;
			while (user) {
				listed++;
				Wstring name((char *)user->name);
				name.toLower();
				int i;
				for (i = 0; i < count; i++) {
					User entry;
					g_UserList.get(entry, i);
					Wstring other((char *)entry.name);
					other.toLower();
					if (name == other) break;
				}
				if (i >= count) {
					found = false;
					break;
				}
				user = user->next;
			}

			bool ok = listed == count && found;
			if (!ok) {
				g_GameStartRequested = false;
				WolOptionsLocked = 0;
			} else {
				if (Online_Session_State == 2 || Online_Session_State == 1) {
					HWND dlg = WS_Find_Dialog(0x117);
					WS_Destroy_Dialog(dlg, 0);
				}
				GameID = gameid;
				GameProduct = g_GameSKU;
				g_WDTGameSKU = 0x2300;
				WestwoodOnline_Tournament = g_CurrentChannel.tournament;
				if (Is_Current_Channel_Flag_8()) WestwoodOnline_Tournament = 2;
				strcpy(PlayerNickname, g_NickName.get());
				User * first = NULL;
				g_UserList.getPointer(&first, 0);
				if (first) strcpy(GameHostName, (char *)first->name);
				GameStartTime = time(0);
				DebugString("OnGameStart\n");
				if (channel) g_LobbyNumber = channel->reserved & 0xFF;
				g_StartGameRunning = false;
				g_StartGameReceived = true;
				User_List_To_Session_Players(users);
				WolOptionsLocked = 2;
				DebugString("Starting a %d player game.\n", Session.NumPlayers);
				User * owner = Find_Owner_User();
				if (owner) Char_To_Wide(Session.MasterPlayerName, (char *)owner->name, -1);
				Session.MasterPlayerID = -1;
				Session.ConnectionResultCount = Build_Connection_Pairs(Session.PeerProbeData);
			}
			return(S_OK);
		}
		STDMETHOD(OnUserKick)(HRESULT r, struct Channel * channel, User * kicked, User * kicker)
		{
			if (g_PlayingNetGame) {
				return(S_OK);
			}
			if (FAILED(r)) {
				return(S_OK);
			}

			static char _kickmessage[64];
			memset(_kickmessage, 0, sizeof(_kickmessage));
			char * name = (char *)channel->name;
			if (strnicmp(name, "Lob_33_", strlen("Lob_33_")) == 0) {
				int v = atol(((char *)channel->name) + strlen("Lob_33_"));
				int d = v / 8;
				int m = v % 8;
				if (d == 0) {
					sprintf(_kickmessage, "%s", g_Lobbies[m]);
				} else {
					sprintf(_kickmessage, "%s %d", g_Lobbies[m], d + 1);
				}
			} else {
				sprintf(_kickmessage, "%s", name);
			}

			if (kicked->flags & CHAT_USER_MYSELF) {
				WideWstring text;
				if (g_CurrentChannel.type && Is_Channel_Full()) {
					text = StringTable::LoadString("TXT_CHANNEL_FULL", NULL, "D:\\Ra2\\wonline.cpp", 7607);
				} else {
					wchar_t buffer[1024];
					memset(buffer, 0, sizeof(buffer));
					swprintf(buffer, StringTable::LoadString("TXT_YOURE_KICKED", NULL, "D:\\Ra2\\wonline.cpp", 7611), Narrow_To_Wide(name), Narrow_To_Wide((char *)kicker->name));
					text = buffer;
				}
				if (!g_CurrentChannel.type) {
					ODMessageBox(text.get(), MB_OK, Wait_Callback_Thunk);
					g_LobbyNumber++;
				} else {
					HWND dlg = WS_Find_Dialog(0x113);
					if (dlg) _DrawMessage(ColorSystem, text.get(), GetDlgItem(dlg, 0x53F));
				}
			} else if (!Is_All_Slots_Filled()) {
				Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_USER_KICKED", NULL, "D:\\Ra2\\wonline.cpp", 7635), Narrow_To_Wide((char *)kicked->name), Narrow_To_Wide(name), Narrow_To_Wide((char *)kicker->name));
			}
			Handle_User_Leave(*kicked);
			Draw_Player_List(0, false);
			Refresh_Game_List();
			return(S_OK);
		}
		STDMETHOD(OnUserIP)(HRESULT, User *) { return(S_OK); }
#ifdef RA2_NON_MATCHING // not exact: 2765 bytes differ by position, one call and 61 instruction lines (main-derived tree, the find results as the image's if / else if chain; the image calls keyHash 0x00767CD0 under the first lookup's expanded getPointer and ours expands it; ra2-decomp's matched twin, Yuri's Revenge 0x007A9970, expands it too, so its text does not settle the level; in scratch the row is the image's code when the lookup sits in an inline function with one more inline site after it, lane F58)
		STDMETHOD(OnFind)(HRESULT r, Channel * chan)
		{
			unsigned int status = 0;

			/*
			 * An answer for the buddy list: record where the buddy is, then ask for the next one.
			 */
			if (g_BuddyFindName.length() != 0) {
				g_BuddyStatus.getValue(g_BuddyFindName, status);
				char name[64];
				name[0] = '\0';
				if (chan) {
					if (chan->type == 0) {
						if (_strnicmp((char *)chan->name, "Lob_38_", strlen("Lob_38_")) == 0) {
							status = 7;
						} else if (_strnicmp((char *)chan->name, "Lob_35_", strlen("Lob_35_")) == 0) {
							status = 8;
						} else {
							status = 4;
						}
					} else {
						status = 5;
						Wstring channel_name((char *)chan->name);
						Channel game;
						if (Find_Channel_By_Name(channel_name, g_GameChannelList, game)) {
							if (Is_Channel_Flag_12(&game)) {
								status = 8;
							} else if (Is_Channel_Flag_11(&game)) {
								status = 7;
							}
						}
					}
					if (g_BuddyFindName.length() > 0) g_BuddyChannels.add(g_BuddyFindName, *chan);
					strcpy(name, (char *)chan->name);
					if (strncmp(name, "Lob_33_", 7) == 0) {
						int v = atol(name + strlen("Lob_33_"));
						int d = v / 8;
						int m = v % 8;
						if (d == 0) {
							sprintf(name, "%s", g_Lobbies[m]);
						} else {
							sprintf(name, "%s %d", g_Lobbies[m], d + 1);
						}
					}
					SMessagePrintf(ColorSystem, StringTable::LoadString("TXT_FINDOK", NULL, "D:\\Ra2\\wonline.cpp", 7086), name);
				} else {
					if (g_BuddyFindName.length() > 0) g_BuddyChannels.remove(g_BuddyFindName);
					if (r == CHAT_S_FIND_NOCHAN) {
						status = 3;
						SMessagePrintf(ColorSystem, StringTable::LoadString("TXT_FINDNONE", NULL, "D:\\Ra2\\wonline.cpp", 7095));
					} else if (r == CHAT_S_FIND_NOTHERE) {
						status = 2;
						SMessagePrintf(ColorSystem, StringTable::LoadString("TXT_FINDGONE", NULL, "D:\\Ra2\\wonline.cpp", 7099));
					} else if (r == CHAT_S_FIND_OFF) {
						status = 1;
						SMessagePrintf(ColorSystem, StringTable::LoadString("TXT_FINDDISABLED", NULL, "D:\\Ra2\\wonline.cpp", 7103));
					} else {
						SMessagePrintf(ColorSystem, StringTable::LoadString("TXT_FINDFAILED", NULL, "D:\\Ra2\\wonline.cpp", 7106));
					}
				}
				if (g_BuddyFindName.length() > 0) {
					g_BuddyStatus.add(g_BuddyFindName, status);
					g_BuddyFindName.clear();
					HWND dlg = WS_Find_Dialog(0x116);
					if (dlg) Fill_Buddy_List(dlg);
				}
				Service_Buddy_Find_Queue();
				return(S_OK);
			} else {

				/*
				 * An answer to the player's own /find.
				 */
				wchar_t text[128];
				if (chan) {
					if (chan->type == 0 && strncmp((char *)chan->name, "Lob_33_", strlen("Lob_33_")) == 0) {
						int v = atol((char *)chan->name + strlen("Lob_33_"));
						int d = v / 8;
						int m = v % 8;
						WideWstring lobby(g_Lobbies[m]);
						if (d != 0) {
							lobby += L" ";
							lobby += WideWstring_FromInt(d + 1);
						}
						wchar_t const * user_name = Narrow_To_Wide(LastFindName);
						wchar_t const * lobby_name = lobby.get();
						swprintf(text, StringTable::LoadString("TXT_USER_FOUND", NULL, "D:\\Ra2\\wonline.cpp", 7003), user_name, lobby_name);
					} else {
						wchar_t const * user_name = Narrow_To_Wide(LastFindName);
						wchar_t const * channel_name = Narrow_To_Wide((char *)chan->name);
						swprintf(text, StringTable::LoadString("TXT_USER_FOUND", NULL, "D:\\Ra2\\wonline.cpp", 7005), user_name, channel_name);
					}
				} else if (r == CHAT_S_FIND_NOTHERE) {
					swprintf(text, StringTable::LoadString("TXT_USER_NOT_ONLINE", NULL, "D:\\Ra2\\wonline.cpp", 7012), Narrow_To_Wide(LastFindName));
				} else if (r == CHAT_S_FIND_NOCHAN) {
					swprintf(text, StringTable::LoadString("TXT_USER_NOT_IN_CHANNEL", NULL, "D:\\Ra2\\wonline.cpp", 7014), Narrow_To_Wide(LastFindName));
				} else if (r == CHAT_S_FIND_OFF) {
					swprintf(text, StringTable::LoadString("TXT_USER_FIND_DISABLED", NULL, "D:\\Ra2\\wonline.cpp", 7016), Narrow_To_Wide(LastFindName));
				} else {
					swprintf(text, StringTable::LoadString("TXT_USER_NOT_ONLINE", NULL, "D:\\Ra2\\wonline.cpp", 7018), Narrow_To_Wide(LastFindName));
				}

				HWND dlg = WS_Top_Window();
				if (dlg) {
					if (GetDlgItem(dlg, 0x53F)) {
						Lobby_Chat_Printf(ColorSystem, text);
						return(S_OK);
					}
					if (GetDlgItem(dlg, 0x5B5)) {
						SMessagePrintf(ColorSystem, text);
						return(S_OK);
					}
				}
				if (Session.Is_In_Game()) {
					int color = GadgetClass::Get_Color_Scheme();
					Session.Messages.Add_Message(0, 0, text, color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
				}
				LastFindName[0] = '\0';
				return(S_OK);
			}
		}
#else
		STDMETHOD(OnFind)(HRESULT r, Channel * chan);
#endif
		STDMETHOD(OnPageSend)(HRESULT r)
		{
			wchar_t text[128];

			if (r == S_OK) {
				SMessagePrintf(ColorSystem, StringTable::LoadString("TXT_PAGESENT", NULL, "D:\\Ra2\\wonline.cpp", 6944));
				return(S_OK);
			}

			if (r == CHAT_S_PAGE_OFF) {
				swprintf(text, StringTable::LoadString("TXT_PAGEDISABLED", NULL, "D:\\Ra2\\wonline.cpp", 6948));
			} else if (r == CHAT_S_PAGE_NOTHERE) {
				swprintf(text, StringTable::LoadString("TXT_PAGEUSERGONE", NULL, "D:\\Ra2\\wonline.cpp", 6950));
			} else {
				swprintf(text, StringTable::LoadString("TXT_PAGEFAILED", NULL, "D:\\Ra2\\wonline.cpp", 6952));
			}

			HWND dlg = WS_Top_Window();
			if (dlg) {
				if (GetDlgItem(dlg, 0x53F)) {
					Lobby_Chat_Printf(ColorSystem, text);
					return(S_OK);
				}
				if (GetDlgItem(dlg, 0x5B5)) {
					SMessagePrintf(ColorSystem, text);
					return(S_OK);
				}
			}
			if (Session.Is_In_Game()) {
				int color = GadgetClass::Get_Color_Scheme();
				Session.Messages.Add_Message(0, 0, text, color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
			}
			return(S_OK);
		}
		STDMETHOD(OnPaged)(HRESULT r, struct User * user, LPCSTR text)
		{
			wchar_t msg[256];

			if (FAILED(r)) {
				return(S_OK);
			}

			WideWstring wide = Wstring_ToWide(text);
			if (user) strcpy(LastPagerName, (char *)user->name);

			if (g_PlayingNetGame) {
				swprintf(msg, L"(%hs) %s", user->name, wide.get());
				int color = GadgetClass::Get_Color_Scheme();
				Session.Messages.Add_Message(0, 0, msg, color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
				Map.Flag_To_Redraw(GS_REDRAW_DIRTY);
				SMessagePrintf(ColorPaged, L"(%hs) %s", user->name, wide.get());
				g_PageTimer = CDTimerClass<SystemTimerClass>(300);
			} else {
				HWND dlg = WS_Top_Window();
				while (dlg) {
					if (GetDlgItem(dlg, 0x53F)) {
						Lobby_Chat_Printf(ColorPaged, L"(%hs) %s", user->name, wide.get());
						break;
					}
					if (GetDlgItem(dlg, 0x5B5)) SMessagePrintf(ColorPaged, L"(%hs) %s", user->name, wide.get());
					dlg = WS_Next_Lower_Dialog(dlg);
				}
				if (!dlg) {
					ReleaseCapture();
					Darken_Surface(NULL, true);
					dlg = OwnerDraw::Begin_Dialog(0x11E, (DLGPROC)Page_Dialog_Proc, 0);
					if (dlg) {
						SetFocus(dlg);
						Lobby_Chat_Printf(ColorPaged, L"(%hs) %s", user->name, wide.get());
						ShowWindow(dlg, SW_SHOWNORMAL);
					}
				}
				Show_Name(dlg, Get_Page_Recipient(GetDlgItem(dlg, 0x53F)));
			}
			VocClass::PlayGlobal(Rule->IncomingMessage, 1.0f, 0x2000, 0);
			return(S_OK);
		}

		STDMETHOD(OnPagedUnicode)(HRESULT r, struct User * user, const unsigned short * text)
		{
			wchar_t msg[256];

			if (FAILED(r)) {
				return(S_OK);
			}

			if (g_PlayingNetGame) {
				swprintf(msg, L"(%hs) %s", user->name, text);
				int color = GadgetClass::Get_Color_Scheme();
				Session.Messages.Add_Message(0, 0, msg, color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
				Map.Flag_To_Redraw(GS_REDRAW_DIRTY);
				SMessagePrintf(ColorPaged, L"(%hs) %s", user->name, text);
			} else {
				HWND dlg = WS_Top_Window();
				while (dlg) {
					if (GetDlgItem(dlg, 0x53F)) {
						Lobby_Chat_Printf(ColorPaged, L"(%hs) %s", user->name, text);
						break;
					}
					if (GetDlgItem(dlg, 0x5B5)) SMessagePrintf(ColorPaged, L"(%hs) %s", user->name, text);
					dlg = WS_Next_Lower_Dialog(dlg);
				}
				if (!dlg) {
					ReleaseCapture();
					Darken_Surface(NULL, true);
					dlg = OwnerDraw::Begin_Dialog(0x11E, (DLGPROC)Page_Dialog_Proc, 0);
					SetFocus(dlg);
					Lobby_Chat_Printf(ColorPaged, L"(%hs) %s", user->name, text);
					ShowWindow(dlg, SW_SHOWNORMAL);
				}
				Show_Name(dlg, Get_Page_Recipient(GetDlgItem(dlg, 0x53F)));
			}
			VocClass::PlayGlobal(Rule->IncomingMessage, 1.0f, 0x2000, 0);
			return(S_OK);
		}
		STDMETHOD(OnServerBannedYou)(HRESULT r, time_t bannedTill)
		{
			if (!g_PlayingNetGame && r == S_OK) {
				wchar_t ban_text[200];
				swprintf(ban_text, StringTable::LoadString("TXT_BANNEDTILL", NULL, "D:\\Ra2\\wonline.cpp", 7508), ctime(&bannedTill));
				ODMessageBox(ban_text, MB_OK, Wait_Callback_Thunk);
			}
			return(S_OK);
		}
		STDMETHOD(OnUserFlags)(HRESULT r, LPCSTR name, unsigned flags, unsigned mask)
		{
			if (r == S_OK) {
				for (int i = 0; i < g_UserList.length(); i++) {
					User user;
					if ((g_UserList.get(user, i)) && (stricmp((char *)user.name, name) == 0)) {
						user.flags = ((user.flags & mask) | flags);
						g_UserList.remove(i);

						/// Insert the user again into the sorted channel user list.
						int index = g_UserList.length();
						User temp_user;
						for (int j = 0; j < g_UserList.length(); j++) {
							g_UserList.get(temp_user, j);
							if (Compare_Users(&user, &temp_user) < 0) {
								index = j;
								break;
							}
						}
						User entry = user;
						entry.next = NULL;
						g_UserList.add(entry, index);
					}
				}
				Draw_Player_List(0, false);
			}
			return(S_OK);
		}
		STDMETHOD(OnChannelBan)(HRESULT, LPCSTR name, int banned)
		{
			if (!g_PlayingNetGame) {
				if (banned) {
					Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_CHAN_BAN", NULL, "D:\\Ra2\\wonline.cpp", 7521), Narrow_To_Wide(name));
				} else {
					Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("TXT_CHAN_UNBAN", NULL, "D:\\Ra2\\wonline.cpp", 7524), Narrow_To_Wide(name));
				}
			}
			return(S_OK);
		}
		STDMETHOD(OnUserLocale)(HRESULT r, User * users)
		{
			bool set = false;
			if (r < 0) {
				DebugString("OnUserLocale callback failed!\n");
				return(S_OK);
			}

			User * user = users;
			while (user != NULL) {
				Wstring name;
				name.set((char *)user->name);
				name.toLower();
				g_UserLocales.remove(name);
				int locale = user->locale;
				g_UserLocales.add(name, locale);
				if (stricmp((const char *)users->name, g_NickName.get()) == 0) {
					WOLOptionsClass options;
					options.Load();
					rc_ptr<PlayerProfile> profile;
					if (options.Is_Bit1_Clear()) {
						profile = PlayerProfile::Load(g_NickName);
					} else {
						profile = g_PlayerProfile.Get();
					}
					profile->SetLocale(users->locale, 2);
					if (options.Is_Bit1_Clear()) profile->Save(&g_NickName);
					DebugString("OnUserLocale callback - Locale is %d\n", users->locale);
					set = true;
				}
				user = user->next;
			}

			if (set) {
				SetEvent(g_WaitEventHandles[EV_LOCALE]);
			}
			return(S_OK);
		}
		STDMETHOD(OnUserTeam)(HRESULT r, User * users)
		{
			if (SUCCEEDED(r)) {
				User * user = users;
				Wstring name;
				while (user != NULL) {
					name.set((char *)user->name);
					if (g_WDTTeams.contains(name)) {
						g_WDTTeams.remove(name);
					}
					int team = user->team;
					g_WDTTeams.add(name, team);
					user = user->next;
				}
				Draw_Player_List(0, false);
				return(S_OK);
			}
			DebugString("OnUserTeam FAILED!\n");
			return(S_OK);
		}
		STDMETHOD(OnSetLocale)(HRESULT, Locale newlocale)
		{
			Wstring name;
			char handle[64];
			strcpy(handle, Session.Handle);
			name.set(handle);
			name.toLower();
			if (g_UserLocales.contains(name)) {
				g_UserLocales.remove(name);
			}
			g_UserLocales.add(name, (int &)newlocale);
			return(S_OK);
		}
		STDMETHOD(OnSetTeam)(HRESULT r, int newteam)
		{
			if (r == S_OK) {
				Session.House = newteam;
				Wstring name;
				char handle[64];
				strcpy(handle, Session.Handle);
				name.set(handle);
				name.toLower();
				if (g_WDTTeams.contains(name)) {
					g_WDTTeams.remove(name);
				}
				g_WDTTeams.add(name, newteam);
				return(S_OK);
			}
			DebugString("OnSetTeam FAILED: %d\n", newteam);
			return(S_OK);
		}
		STDMETHOD(OnBuddyList)(HRESULT r, User * buddy_list)
		{
			if (SUCCEEDED(r)) {
				for (User * buddy = buddy_list; buddy != NULL; buddy = buddy->next) {
					Wstring name((char *)buddy->name);
					Add_Buddy(name);
				}
				Request_Buddy_Ladders();
				Close_Wait_Window(0x200);
				HWND dlg = WS_Find_Dialog(0x116);
				if (dlg) {
					Fill_Buddy_List(dlg);
					KillTimer(dlg, 0x704);
					EnableWindow(GetDlgItem(dlg, 0x704), TRUE);
				}
			}
			return(S_OK);
		}
		STDMETHOD(OnBuddyAdd)(HRESULT, User *) { return(S_OK); }
		STDMETHOD(OnBuddyDelete)(HRESULT, User *) { return(S_OK); }
		STDMETHOD(OnServerTime)(HRESULT, time_t) { return(S_OK); }
		STDMETHOD(OnInsiderStatus)(HRESULT, User *) { return(S_OK); }
		STDMETHOD(OnSetLocalIP)(HRESULT, LPCSTR) { return(S_OK); }
};

class CNetUtilEventSink : public CComObjectRoot, public INetUtilEvent
{
	public:
		CNetUtilEventSink(void) {}
		BEGIN_COM_MAP(CNetUtilEventSink)
		COM_INTERFACE_ENTRY(INetUtilEvent)
		END_COM_MAP()

		STDMETHOD(OnWDTState)(HRESULT res, unsigned char * state, int length)
		{
			if (res >= 0 && length > 0) {
				rc_ptr<WDTState> newstate = WDTState::Create(state);

				if (newstate.Get() != NULL && newstate->Has_Owner_History()) {
					if (newstate->Is_Previous_Cycle()) {
						DebugString("OnWDTState: Previous cycle data received\n");
						WDT_Store_Previous(newstate);
						SetEvent(g_WaitEventHandles[EV_11]);
					} else {
						DebugString("OnWDTState: Current cycle data received\n");
						g_NextWDTStatePollTime = time(0) + newstate->TickTime;
						WDT_Store_Current(newstate);
						SetEvent(g_WaitEventHandles[EV_11]);
					}
				} else {
					DebugString("OnWDTState: NACK received from server\n");
					SetEvent(g_WaitEventHandles[EV_11]);
				}
				return(S_OK);
			}

			/*
			 * Bad result code or zero-length packet -- signal the error event instead.
			 */
			SetEvent(g_WaitEventHandles[EV_ERROR]);
			return(S_OK);
		}

		STDMETHOD(OnGameresSent)(HRESULT)
		{
			if (Session.Type == GAME_INTERNET && Online_Session_State == 2 && WestwoodOnline_Tournament) {
				WestwoodOnline_Tournament = 1;
				Send_Statistics_Packet(true);
			}
			return(S_OK);
		}

		STDMETHOD(OnLadderList)(HRESULT res, Ladder * list, int, long, int keyRung)
		{
			if (res != S_OK) {
				list = NULL;
			}

			if (g_PlayingNetGame) return(S_OK);

			HWND rung_list = GetDlgItem(WS_Find_Dialog(0xC1), 0x57F);

			if ((rung_list) && ((keyRung == -1) && (res == S_OK))) rung_list = NULL;

			/*
			 * If there's no data at all for the ladder window, show the "not in ladder"
			 * placeholder row instead of an empty list.
			 */
			if (rung_list) {
				SendMessageA(rung_list, LB_RESETCONTENT, 0, 0);
				if (list == NULL) {
					SendMessageA(rung_list, OD_INSERTSTRINGW, (WPARAM)-1, (LPARAM)StringTable::LoadString("TXT_NOT_IN_LADDER", NULL, "D:\\Ra2\\wonline.cpp", 3638));
					InvalidateRect(rung_list, NULL, false);
					return(S_OK);
				}
			} else if (list == NULL) {
				return(S_OK);
			}

			Ladder * temp = list;
			wchar_t list_string[128];
			int top_rung = list->rung;

			OwnerDraw::CellData thecell;

			/*
			 * Cap the appropriate per-SKU ladder dictionary's size before caching new
			 * entries, so it doesn't grow without bound across repeated ladder requests.
			 */
			if (list->sku == 0x802100) {
				if (g_BattleClansLadderDict.getEntries() > 2000) {
					g_BattleClansLadderDict.clear();
				}
			} else if (list->sku == 0x2300) {
				if (g_WDTLadderDict.getEntries() > 2000) {
					g_WDTLadderDict.clear();
				}
			} else {
				if (g_GeneralLadderDict.getEntries() > 2000) {
					g_GeneralLadderDict.clear();
				}
			}

			if (list && rung_list) {
				g_LadderPos = list->rung;
			}

			Wstring const & nick = g_NickName;
			Wstring local_name = nick.toLower();

			/*
			 * Walk the rung list, inserting a row per entry (skipping entries with no
			 * recorded points) and caching each entry into its per-SKU dictionary.
			 */
			int row = 0;
			while (temp) {
				if (temp->points == (unsigned int)-1) {
					temp = temp->next;
					continue;
				}

				if (rung_list) {
					thecell.type = OwnerDraw::CellData::PRIMARY;
					swprintf(list_string, L"%d", temp->rung);

					SendMessageA(rung_list, OD_INSERTSTRINGW, (WPARAM)-1, (LPARAM)list_string);
					SendMessageA(rung_list, OD_SETCELL, MAKEWPARAM(g_Col_Rung, row), (LPARAM)&thecell);

					thecell.type = OwnerDraw::CellData::TEXT;
					wchar_t login_name[5120];
					Char_To_Wide(login_name, (char *)temp->login_name, -1);
					thecell.string.set(login_name);
					SendMessageA(rung_list, OD_SETCELL, MAKEWPARAM(g_Col_Name, row), (LPARAM)&thecell);

					swprintf(list_string, L"%d", temp->points);
					thecell.string.set(list_string);
					SendMessageA(rung_list, OD_SETCELL, MAKEWPARAM(g_Col_Points, row), (LPARAM)&thecell);

					swprintf(list_string, L"%d", temp->wins);
					thecell.string.set(list_string);
					SendMessageA(rung_list, OD_SETCELL, MAKEWPARAM(g_Col_Wins, row), (LPARAM)&thecell);

					swprintf(list_string, L"%d", temp->losses);
					thecell.string.set(list_string);
					SendMessageA(rung_list, OD_SETCELL, MAKEWPARAM(g_Col_Losses, row), (LPARAM)&thecell);

					swprintf(list_string, L"%d", temp->disconnects);
					thecell.string.set(list_string);
					SendMessageA(rung_list, OD_SETCELL, MAKEWPARAM(g_Col_Disconnects, row), (LPARAM)&thecell);

					row++;
				}

				Wstring ladder_name((char *)temp->login_name);
				ladder_name.toLower();

				if (list->sku == 0x802100) {
					g_BattleClansLadderDict.remove(ladder_name);
					g_BattleClansLadderDict.add(ladder_name, *temp);
				} else if (list->sku == 0x2300) {
					g_WDTLadderDict.remove(ladder_name);
					g_WDTLadderDict.add(ladder_name, *temp);
				} else {
					g_GeneralLadderDict.remove(ladder_name);
					g_GeneralLadderDict.add(ladder_name, *temp);
				}

				if (local_name == ladder_name) {
					rc_ptr<PlayerProfile> profile = PlayerProfile::Load(g_NickName);
					if (profile.Get() != NULL) {
						profile->UpdateStats(temp);
						profile->Save(&ladder_name);
						HWND dialog = WS_Find_Dialog(0x10E);
						if (dialog == NULL) dialog = WS_Find_Dialog(0x114);
						if (dialog != NULL) profile->PopulateDialog(dialog, g_NickName, false);
					}
				}

				temp = temp->next;
			}

			if (rung_list) {
				SendMessageA(rung_list, LB_SETCURSEL, (WPARAM)keyRung - top_rung, 0);
				InvalidateRect(rung_list, NULL, false);
			} else {
				Draw_Player_List(0, false);
			}

			Request_Buddy_Ladders();
			HWND buddy_dialog = WS_Find_Dialog(0x116);
			if (buddy_dialog != NULL) Update_Buddy_Profile(buddy_dialog);
			return(S_OK);
		}

		STDMETHOD(OnNewNick)(HRESULT res, LPCSTR message, LPCSTR nick, LPCSTR pass)
		{
			wchar_t text[5120];

			DebugString("OnNewNick\n");
			g_NewNickResult = res;

			if (res == S_OK) {
				rc_ptr<PlayerProfile> profile = PlayerProfile::Load(nick);
				int nicknum = 0;
				WOLOptionsClass options;
				options.Load();
				if (options.Is_Bit1_Clear()) {
					int locale = profile->Locale;
					nicknum = Save_Nickname(nick, pass, true, locale);
					Session.Write_MultiPlayer_Settings();
				}

				bool login_mode = true;
				bool combo_mode = false;
				HWND dlg = WS_Find_Dialog(0x10F);
				if (dlg == NULL) {
					dlg = WS_Find_Dialog(0x11D);
					combo_mode = dlg != NULL;
					if (dlg == NULL) {
						login_mode = false;
						dlg = WS_Find_Dialog(0x114);
					}
				}

				if (dlg) {
					HWND nick_list = GetDlgItem(dlg, 0x628);
					if (combo_mode) {
						SendMessageA(nick_list, 0x4B4, 0, (LPARAM)nick);
					} else {
						int index = SendMessageA(nick_list, 0x4BC, 0, (LPARAM)nick);
						SendMessageA(nick_list, CB_SETITEMDATA, index, nicknum);
						SendMessageA(nick_list, CB_SETCURSEL, index, 0);
					}
					if (login_mode) {
						g_NickName = nick;
						g_LoginPassword = pass;
						HWND password = GetDlgItem(dlg, 0x72C);
						SendMessageA(password, 0x4B4, 0, (LPARAM)g_LoginPassword.text());
						g_PlaintextPassword = true;
						EnableWindow(GetDlgItem(dlg, 0x539), true);
						if (!combo_mode) {
							EnableWindow(GetDlgItem(dlg, 0x72D), false);
							SendMessageA(GetDlgItem(dlg, 0x72D), BM_SETCHECK, 1, 0);
						}
					} else {
						SendMessageA(GetDlgItem(dlg, 0x6FF), BM_SETCHECK, 0, 0);
						EnableWindow(GetDlgItem(dlg, 0x6FF), true);
					}
					if (!combo_mode) {
						EnableWindow(GetDlgItem(dlg, 0x6EB), true);
						Wstring name(nick);
						profile->PopulateDialog(dlg, name, true);
					}
				}
			}

			if (message) {
				Char_To_Wide(text, message, -1);
				ODMessageBox(text, MB_OK, Wait_Callback_Thunk);
			} else {
				ODMessageBox(L"Unknown Status", MB_OK, Wait_Callback_Thunk);
			}

			Close_Wait_Window(0x100);
			return(S_OK);
		}

		STDMETHOD(OnAgeCheck)(HRESULT res, int age, int consent)
		{
			DebugString("OnAgeCheck\n");
			g_AgeCheckResult = res;
			if (res == S_OK) {
				g_UserAge = age;
				g_UserConsent = consent;
			}
			Close_Wait_Window(0x100);
			return(S_OK);
		}
		STDMETHOD(OnPing)(HRESULT res, int time, unsigned long, int)
		{
			if (g_PlayingNetGame && g_PingsSent) {
				if (res == S_OK && time >= 0 && time < 1000) {
					g_PingsReceived++;
				}
				return(S_OK);
			}

			if (Is_Tournament_Session()) {
				DebugString("Received Ping\n");
				g_MatchPingsReceived++;
				if (g_MatchPingsReceived >= g_PingsPending.length()) {
					if (g_MatchState == 4 || g_MatchState == 5) {
						Send_Pending_Ping_Report();
					}
					Service_Pending_Player_Pings();
				}
				return(S_OK);
			}

			if (g_CurrentChannel.type != 0) {
				Draw_Player_List(0, false);
			}

			return(S_OK);
		}
};

class CDownloadEventSink : public CComObjectRoot, public IDownloadEvent
{
	public:
		CDownloadEventSink(void) {}
		BEGIN_COM_MAP(CDownloadEventSink)
		COM_INTERFACE_ENTRY(IDownloadEvent)
		END_COM_MAP()
		STDMETHOD(OnEnd)(void)
		{
			SendMessageA(WS_Top_Window(), 0x5F4, 0, 0);
			return(S_OK);
		}

		STDMETHOD(OnError)(int error)
		{
			SendMessageA(WS_Top_Window(), 0x5F5, 0, error);
			return(S_OK);
		}

		STDMETHOD(OnProgressUpdate)(int bytesread, int totalsize, int, int timeleft)
		{
			wchar_t stat[200];
			static time_t prev_stat = 0;

			stat[0] = 0;
			SendDlgItemMessageA(WS_Top_Window(), 0x5C6, PBM_SETPOS, (unsigned)(bytesread * 100) / (unsigned)totalsize, 0);
			if (timeleft > 0) {
				swprintf(stat, StringTable::LoadString("TXT_BYTES_W_TIME", NULL, "D:\\Ra2\\wonline.cpp", 3981), bytesread, totalsize, timeleft);
			} else {
				swprintf(stat, StringTable::LoadString("TXT_BYTES_WO_TIME", NULL, "D:\\Ra2\\wonline.cpp", 3986), bytesread, totalsize);
			}
			if ((time(NULL) - prev_stat)) {
				HWND status = GetDlgItem(WS_Top_Window(), 0x5C8);
				SendMessageA(status, 0x4B2, 0, (LPARAM)stat);
				prev_stat = time(NULL);
			}
			return(S_OK);
		}

		STDMETHOD(OnStatusUpdate)(int status)
		{
			wchar_t stat[200];

			stat[0] = 0;
			switch (status) {
				case DOWNLOADSTATUS_CONNECTING:
					swprintf(stat, StringTable::LoadString("TXT_CONNECTING", NULL, "D:\\Ra2\\wonline.cpp", 4005));
					break;
				case DOWNLOADSTATUS_FINDINGFILE:
					swprintf(stat, StringTable::LoadString("TXT_FINDING_PATCH", NULL, "D:\\Ra2\\wonline.cpp", 4009));
					break;
				default:
					break;
			}
			HWND label = GetDlgItem(WS_Top_Window(), 0x5C8);
			SendMessageA(label, 0x4B2, 0, (LPARAM)stat);
			return(S_OK);
		}

		STDMETHOD(OnQueryResume)(void) { return(DOWNLOADEVENT_RESUME); }
};

CComObject<CChatEventSink> * g_pChatSink;
CComObject<CNetUtilEventSink> * g_pNetUtilSink;
CComObject<CDownloadEventSink> * g_pDownloadSink;

void Redraw_Player_List(void)
{
	Draw_Player_List(0, false);
}


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
	if (g_pChat != NULL) {
		CComObject<CChatEventSink>::CreateInstance(&g_pChatSink);
		hresult = AtlAdvise(g_pChat, g_pChatSink->GetUnknown(), IID_IChatEvent, &g_dwChatAdvise);
	}
	if ((g_pChat == NULL) || (hresult != S_OK)) {
		ODMessageBox(StringTable::LoadString("TXT_APIMISSING", NULL, "D:\\Ra2\\wonline.cpp", 7881), 0, NULL, false);
		return(-2);
	}

	CoCreateInstance(CLSID_NetUtil, NULL, CLSCTX_INPROC_SERVER, IID_INetUtil, (void **)&g_pNetUtil);
	if (g_pNetUtil != NULL) {
		CComObject<CNetUtilEventSink>::CreateInstance(&g_pNetUtilSink);
		hresult = AtlAdvise(g_pNetUtil, g_pNetUtilSink->GetUnknown(), IID_INetUtilEvent, &g_dwNetUtilAdvise);
	}
	if ((g_pNetUtil == NULL) || (hresult != S_OK)) {
		ODMessageBox(StringTable::LoadString("TXT_APIMISSING", NULL, "D:\\Ra2\\wonline.cpp", 7897), 0, NULL, false);
		return(-2);
	}

	CoCreateInstance(CLSID_Download, NULL, CLSCTX_INPROC_SERVER, IID_IDownload, (void **)&g_pDownload);
	if (g_pDownload != NULL) {
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
	if (APP_REG_KEY != NULL && strlen(APP_REG_KEY) != 0) {
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

namespace OwnerDraw { BOOL __stdcall Set_Paint_Disabled(HWND window, LPARAM lparam); }
void __fastcall Set_Player_Ping(HWND dialog, int player, int ping);
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
		Reset_Wait_Events();
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
BOOL CALLBACK Locale_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00751190, the country dialog (0xFE)

int Select_WDT_Location(void); // RA2: 0x00751130, below: the country dialog for the persona's locale

static inline void Set_Locale_Entry(Wstring & key, int locale) // RA2: as in ra2-decomp's twin
{
	int old;
	if (g_UserLocales.remove(key, old)) {
		g_UserLocales.add(key, locale);
	}
}

static inline void Apply_Locale(Wstring & key, int locale, User & user) // RA2: as in ra2-decomp's twin
{
	g_pChat->RequestSetLocale((Locale)locale);
	g_pChat->RequestUserLocale(&user);
	Set_Locale_Entry(key, locale);
}

static inline void Check_Locale(Wstring & key, int & locale, rc_ptr<PlayerProfile> & profile, User & user) // RA2: as in ra2-decomp's twin
{
	if (profile.Get() != NULL && profile.Get()->Locale != locale) {
		if (profile.Get()->Locale == 0) {
			profile.Get()->SetLocale(locale, 2);
			profile.Get()->Save(&g_NickName);
		}
		int value = profile.Get()->Locale;
		g_pChat->RequestSetLocale((Locale)value);
		g_pChat->RequestUserLocale(&user);
		locale = value;
		int old;
		if (g_UserLocales.remove(key, old)) {
			g_UserLocales.add(key, locale);
		}
	}
}

static inline int Wait_For_Event(void) // RA2: as in ra2-decomp's twin
{
	int timeout = 30000;
	int wait;
	while ((wait = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100)) == WAIT_TIMEOUT) {
		timeout -= 100;
		if (timeout <= 0) {
			return(EV_COUNT);
		}
		WOL_Wait_Callback();
	}
	ResetEvent(g_WaitEventHandles[wait]);
	return(wait);
}

bool Connect_To_Server(Server * server) // RA2: 0x007424C0, not in TibSun (its Login_WOL did this): logs in to the chat server with the persona and password, waits for the answer (30 seconds, 20 rounds), reads the profile of the persona, tells the ladder server about it and settles the locale of the persona; matches (2192 bytes) with the out-of-line derived destructor and the out-of-line Dictionary calls above
{
	strcpy((char *)server->login, g_NickName.get()); // RA2: the persona and the password go into the server entry
	strcpy((char *)server->password, g_LoginPassword.get());
	Reset_Wait_Events(); // RA2: all wait events are cleared first
	{
		WideWstring connecting(StringTable::LoadString("TXT_CONNECTING_SERVER", NULL, "D:\\Ra2\\wonline.cpp", 8397)); // RA2: the wait dialog takes a wide text
		Show_Wait_Window(0x10, connecting, false);
	}
	g_pChat->RequestConnection(server, 20, ((int &)g_PlaintextPassword) & 0xFF); // RA2: virtual 0x14

	int attempts = 20; // RA2: rounds of the wait
	for (;;) {
		int event = Wait_For_Event();
		if (event == EV_ABORT) {
			Close_Wait_Window(0xFFFFFFFF);
			return(false);
		}
		if (event == EV_ERROR) {
			ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 8410), 0, Wait_Callback_Thunk);
			Close_Wait_Window(0xFFFFFFFF);
			return(false);
		}
		if (event == EV_COUNT) {
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
	g_pChat->SetClientVersion(0x1000E); // RA2: virtual 0xD4
	g_IgnoreNetStatus = false;
	Session.Read_MultiPlayer_Settings();

	WOLOptionsClass options; // RA2: the constructor of wolopt.h clears both members
	options.Load(); // RA2: then Load is called, as 0x004FF200 shows
	rc_ptr<PlayerProfile> profile;
	if (options.Is_Bit1_Clear()) {
		profile = PlayerProfile::Load(g_NickName); // RA2: the persona's own profile
	} else {
		profile = g_PlayerProfile; // RA2: the profile of Reset_WOL_Globals
	}
	if (Online_Session_State == 3 || Online_Session_State == 4) {
		{
			Wstring name(Server_Name_Without_Prefix(server));
			profile.Get()->SetName(&name); // RA2: the server's name is kept in the profile
		}
		if (options.Is_Bit1_Clear()) {
			profile.Get()->Save(&g_NickName);
		}
	}
	strcpy(Session.Handle, g_NickName.get());
	Session.Set_Side(profile.Get()->Country, false);
	Session.Set_Color(profile.Get()->Color, false);
	Session.Write_MultiPlayer_Settings();
	WOL_Set_Last_Valid_Persona(g_NickName);
	if (g_pNetUtil && strlen(g_LadderAddress)) {
		g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, g_NickName.get(), g_GameSKU & 0xFFFF00, -1, 0, 0); // RA2: virtual 0x14
	}
	Reset_Wait_Events();
	User user;
	memset(&user, 0, sizeof(user));
	strcpy((char *)user.name, g_NickName.get());
	g_pChat->RequestUserLocale(&user); // RA2: virtual 0xBC
	for (;;) { // RA2: the second wait, for the locale
		int event = Wait_For_Event();
		if (event == EV_ABORT) {
			Close_Wait_Window(0xFFFFFFFF);
			break;
		}
		if (event == EV_ERROR) {
			ODMessageBox(StringTable::LoadString("TXT_WDT_NET_ERR", NULL, "D:\\Ra2\\wonline.cpp", 8501), 0, Wait_Callback_Thunk);
			Close_Wait_Window(0xFFFFFFFF);
			break;
		}
		if (event == EV_COUNT) {
			Close_Wait_Window(0xFFFFFFFF);
			break;
		}
		if (event == EV_LOCALE) {
			break;
		}
	}

	Wstring key = ((Wstring const &)g_NickName).toLower(); // RA2: the locales are kept by lower case name
	if (g_UserLocales.contains(key)) { // RA2: the server answered for the persona
		int locale = 0;
		g_UserLocales.getValue(key, locale);
		if (locale != 0) {
			Check_Locale(key, locale, profile, user);
			return(true);
		}
	}
	int pick = 0;
	if (profile.Get() != NULL) {
		pick = profile.Get()->Locale;
	}
	if (pick == 0) { // RA2: no locale known anywhere: ask the player
		pick = Select_WDT_Location();
		if (profile.Get() != NULL) {
			profile.Get()->SetLocale(pick, 3);
			profile.Get()->Save(&g_NickName);
		}
	}
	if (pick != 0) {
		Apply_Locale(key, pick, user);
	}
	return(true);
}


bool Connect_To_Selected_Server(void) // RA2: 0x00742D50, not in TibSun: connects to the server of the list the player picked, false when the index is not in the list
{
	if (g_CurrentServerIndex >= 1 && g_CurrentServerIndex < g_Servers.length()) {
		Server * server = NULL;
		g_Servers.getPointer(&server, g_CurrentServerIndex);
		return(Connect_To_Server(server)); // RA2: a jump to Connect_To_Server
	}
	return(false);
}

bool Recall_Stored_Login(Wstring const & name) // RA2: 0x00742D80, not in TibSun: looks the persona up among the 32 logins the chat API remembers; keeps its nickname and password when it is there, else clears the nickname
{
	g_LoginPassword.clear();
	for (int index = 1; index <= 32; index++) {
		char const * nickname; // RA2: not cleared, the chat API sets both when it succeeds
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

int Find_Server_Suffix(Wstring const & name) // RA2: 0x00742E30, not in TibSun: the index of the server whose name is "<prefix>:" followed by this name, -1 when there is none
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

bool Handle_Connection_Error(void) // RA2: 0x00742EB0, not in TibSun: shows the error text of the failed connection; for the answers that have a support page it asks whether to open it and does so (4 is the yes/no kind). True when the answer was one of those
{
	bool handled = false;
	char url[260];
	if (g_OnConnectionResult == CHAT_E_SERIALUNKNOWN || g_OnConnectionResult == CHAT_E_SKUSERIALMISMATCH || g_OnConnectionResult == CHAT_E_BANNED || g_OnConnectionResult == CHAT_E_DISABLED || g_OnConnectionResult == CHAT_E_SERIALBANNED || g_OnConnectionResult == CHAT_E_SERIALDUP) {
		handled = true;
		if (ODMessageBox(g_LastErrorMessage, 4, Wait_Callback_Thunk, false) == 6) { // RA2: IDYES
			int language = g_LanguageCode; // RA2: the support pages exist per language
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

int __fastcall Request_And_Wait_Server_List(unsigned int flags); // RA2: 0x00751C30, asks for the list of chat servers and waits for it (0 done, 1 failed, 2 quit), defined below
LRESULT CALLBACK Wonline_Account_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007540A0, the dialog proc of the login dialog (0x10F, 0x11D)
LRESULT CALLBACK Wonline_Autologin_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00755070, the dialog proc of the dialog (0x112) after a login
void Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, defined in init.cpp

static inline void Set_Login_Focus(HWND window) // RA2: the login dialog puts the focus on the password box (0x72C) when a nickname is known, else on the name box (0x628); the executable has both SetFocus calls with a merged tail
{
	if (g_NickName.length() > 0 && g_LoginPassword.length() == 0) {
		SetFocus(GetDlgItem(window, 0x72C));
	} else {
		SetFocus(GetDlgItem(window, 0x628));
	}
}

static inline int Fetch_Server_List(void) // RA2: shows "fetching the server list" and requests the list for the product (0x2300 in a tournament session, else 0x2100)
{
	{
		WideWstring fetching(StringTable::LoadString("TXT_FETCHING_SERVLIST", NULL, "D:\\Ra2\\wonline.cpp", 22220));
		Show_Wait_Window(16, fetching, false);
	}
	int sku = 0x2300;
	if (Online_Session_State != 2) {
		sku = 0x2100;
	}
	return(Request_And_Wait_Server_List(sku));
}

int __fastcall Login_Wonline(void) // RA2: 0x00742FD0, not in TibSun: logs in to Westwood Online: starts the chat services, fetches the server list, tries the stored login of the auto login persona, else opens the login dialog (the executable's debug text says "Creating login dlg") and connects to the server the persona's profile names; 1 when connected, 0 when not, negative when the server list could not be fetched; NOT matching (unplaced, 1904 bytes against the executable's 1857): the executable calls ArrayList::getPointer 0x00765E60 out of line in the three early expansions of Find_Server_Suffix / Connect_To_Selected_Server (the last two expansions are inline) which our build expands everywhere (about 16 bytes each; wrapper layers around the early calls make the expansions themselves vanish, so the inline budget is not what is wrong); it keeps the zero in ebp and the rc_ptr vtable in ebx where we use ebx; and it expands the derived control destructor (see SimpleWonlineDialogControl)
{
	bool failed = false;
	g_OnConnectionResult = 0;
	int result = Init_Wonline();
	if (result != 1) {
		return(result);
	}
	{ // RA2: the lists of the last session are cleared before the login
		memset(&g_CurrentChannel, 0, sizeof(g_CurrentChannel));
		memset(&g_PendingChannel, 0, sizeof(g_PendingChannel));
		g_UserChannels.clear();
		g_GameChannelList.clear();
		g_UserList.clear();
		g_ChannelCount = 0;
		result = 0;
		int error = Fetch_Server_List();
		switch (error) {
			case 0:
				break;
			case 2:
				failed = true;
				result = -1;
				break;
			default:
				failed = true;
				result = -2;
				break;
	}
		WOLOptionsClass options;
		options.Load();
		if (!failed) {
		if (options.Is_Bit0_Clear()) { // RA2: the auto login is on
				Wstring persona = WOL_Auto_Login_Persona();
				if (persona.length() > 0) {
				if (Recall_Stored_Login(persona)) { // RA2: expanded here
						int index = -1;
					if (Is_Tournament_Session()) { // RA2: an Internet game
							rc_ptr<PlayerProfile> profile = PlayerProfile::Load(persona);
						if (profile.Get()) { // RA2: the server of the persona's profile (Find_Server_Suffix tests the empty name itself)
							index = Find_Server_Suffix(profile->Name); // RA2: expanded here
						}
							if (index == -1) {
								index = g_CurrentServerIndex;
						}
					} else {
							rc_ptr<PlayerProfile> profile = PlayerProfile::Load(persona);
							if (profile.Get()) {
								if (profile->Name.length() == 0) {
									index = g_CurrentServerIndex;
							} else {
								index = Find_Server_Suffix(profile->Name); // RA2: expanded here
							}
						}
					}
						if (index != -1) {
							g_CurrentServerIndex = index;
						if (Connect_To_Selected_Server()) { // RA2: expanded here
								return(1);
						}
							failed = Handle_Connection_Error();
					}
				}
			}
		}
			if (g_NickName.length() == 0 && options.Is_Bit1_Clear()) {
				g_NickName = WOL_Last_Valid_Persona();
		}
		{
				Wstring empty("");
			WOL_Set_Auto_Login_Persona(empty); // RA2: the auto login is used once
		}
			if (failed) {
				return(0);
		}
			DebugString("Creating login dlg\n");
			SimpleWonlineDialogControl control;
			int dialog_id = options.Is_Bit1_Clear() ? 0x10F : 0x11D;
			Darken_Surface(NULL, true);
			HWND window = OwnerDraw::Begin_Dialog(dialog_id, (DLGPROC)Wonline_Account_Dialog_Proc, (LPARAM)&control);
			OwnerDraw::Display_Dialog(window);
			Set_Login_Focus(window);
			while (!control.Is_Done() && !WOL_Wait_Callback() && !OwnerDraw::Dialog_Message_Handler()) {
				Title_Screen_Restore(false);
		}
			OwnerDraw::End_Dialog(window);
		if ((int)control.Get_Window() == 2) { // RA2: the dialog was cancelled
				failed = true;
		} else {
				WOL_Wait_Callback();
				g_IgnoreNetStatus = false;
				WOLOptionsClass saved;
				saved.Load();
				rc_ptr<PlayerProfile> profile;
				if (saved.Is_Bit1_Clear()) {
					profile = PlayerProfile::Load(g_NickName);
			} else {
					profile = g_PlayerProfile.Get();
			}
			int found = Find_Server_Suffix(profile->Name); // RA2: expanded here
				if (found != -1) {
					g_CurrentServerIndex = found;
			}
				if (g_CurrentServerIndex >= g_Servers.length()) {
					g_CurrentServerIndex = -1;
			}
				if (g_CurrentServerIndex == -1 && g_Servers.length() > 1) {
					g_CurrentServerIndex = 1;
			}
			if (Connect_To_Selected_Server()) { // RA2: expanded here
				if (g_RememberLogin && saved.Is_Bit1_Clear()) { // RA2: the login is kept for the next time
					int locale = profile->Locale; // RA2: read before the nickname is saved (0x00742FD0)
						Save_Nickname(g_NickName.get(), g_LoginPassword.get(), g_PlaintextPassword, locale);
						Session.Write_MultiPlayer_Settings();
				}
				if (!saved.Is_Bit0_Clear() || !WOL_Get_Auto_Login()) { // RA2: no auto-login question unless the profile allows it and auto-login is on
						return(1);
				}
					Darken_Surface(NULL, true);
					SimpleWonlineDialogControl next;
					OwnerDraw::Controlled_Dialog(0x112, (DLGPROC)Wonline_Autologin_Dialog_Proc, &next);
					return(1);
			}
		}
	}
	Close_Wait_Window(0xFFFFFFFF); // RA2: expanded here
	if (!failed) {
		Handle_Connection_Error();
	}
	g_IgnoreNetStatus = true;
	}
	return(result);
}

// RA2: declarations for the functions below (placed after Draw_Player_List: declarations above it change its code)
rc_ptr<WDTState> const & __fastcall Current_WDT_State(void); // RA2: 0x00769510, defined in worlddom.cpp

// RA2: declarations for Run_Lobby_Session (before Pick_Lobby: the number of declarations in front of it decides how Pick_Lobby is compiled, see allmatch)
void Initialize_WDT_Sounds(void); // RA2: 0x00752210, fills the lists of the tour's sounds, once
void Discard_WDT_Sound_List(ArrayList<Wstring> sounds); // RA2: 0x00752890, takes a list of sounds by value and does nothing with it
int Join_Selected_Lobby(bool join); // RA2: 0x00744930, defined below
int CALLBACK Wonline_Main_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007469C0, the dialog proc of the lobby (0x113)
bool Start_Network_Session(void); // RA2: 0x00514EE0, defined in init.cpp
inline Wstring Narrow_Text(wchar_t const * text) // RA2: the executable narrows the text through a named wide string that dies before the result is used (0x00743720, 0x00743A40 below)
{
	WideWstring wide(text);
	return Wstring_Narrow(wide);
}
inline WideWstring Wide_Text(char const * text) // RA2: the counterpart: a named narrow string that dies after the widening
{
	Wstring narrow(text);
	return Wstring_ToWide(narrow);
}

inline void Print_Message_Of_The_Day(void) // RA2: the message of the day
{
	if (g_MessageOfTheDay.length() > 0) { // RA2: the message of the day, printed line by line
		char * text = new char[g_MessageOfTheDay.length() + 1];
		if (text) {
			strcpy(text, g_MessageOfTheDay.get());
			char * token = strtok(text, "\n");
			while (token != NULL) {
				WideWstring wide_line = Wide_Text(token);
				Lobby_Chat_Printf(ColorSystem, wide_line.get());
				token = strtok(NULL, "\n"); // RA2: the next token is taken before the wide line is destroyed
			}
			Lobby_Chat_Printf(ColorSystem, L"\n");
			g_MessageOfTheDay.clear();
			delete text;
		}
	}
}

int __fastcall Run_Lobby_Session(HWND window, bool first_time) // RA2: 0x00743720, not in TibSun: the lobby session of Westwood Online: ends the finished game (the tour's victory and defeat sounds), logs in again when the connection was lost, opens the lobby dialog (0x113) with the message of the day, requests the channel list, joins a lobby and waits for the events of the chat: 1 when a game starts, 2 when the skirmish event ended it, 0 otherwise (the dialog, the chat and the wait dialogs are closed again)
{
	int result = 0;
	if (g_PlayingNetGame && Session.Type == GAME_INTERNET && Online_Session_State == 2 && Session.Winner != -1 && Session.Winner < 8) { // RA2: a tour game just ended
		Wstring winner = Narrow_Text(Session.Score[Session.Winner].Name); // RA2: the name of the winner
		if (_strnicmp(winner.get(), Session.Handle, 20) == 0) { // RA2: the local player won
			if (Session.House == 0) {
				Discard_WDT_Sound_List(g_WDT_GDIWinEmphasisSounds);
			} else if (Session.House == 1) {
				Discard_WDT_Sound_List(g_WDT_NodWinEmphasisSounds);
			}
		} else if (Session.House == 0) { // RA2: the local player lost
			Discard_WDT_Sound_List(g_WDT_GDILoseEmphasisSounds);
		}
	}
	g_PlayingNetGame = false;
	WolOptionsLocked = 0;
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: a tour game
		g_pChat->RequestSetTeam(Session.House); // RA2: virtual 0xB4
		if (!g_WDTSoundsInitialized) {
			Initialize_WDT_Sounds();
		}
	}
	Start_Network_Session();
	if (g_ConnectionLost) {
		int login = Login_Wonline();
		if (login != 1) {
			return(login);
		}
	}
	Reset_Wait_Events();
	if (window == NULL) {
		Darken_Surface(NULL, true);
		window = OwnerDraw::Begin_Dialog(0x113, (DLGPROC)Wonline_Main_Dialog_Proc, 0); // RA2: the lobby dialog
		Print_Message_Of_The_Day();
		{
			WideWstring title(StringTable::LoadString("GUI:LoginComplete", NULL, "D:\\Ra2\\wonline.cpp", 9075)); // RA2: the wait text is wide
			Show_Wait_Window(0x10, title, false);
		}
	}
	g_SquadInfoRequests++;
	g_RequestingOwnSquadInfo = 1;
	g_pChat->RequestSquadInfo(0); // RA2: virtual 0xB0
	g_ServerType = 0x21;
	Set_Wait_Window_Text((LPARAM)StringTable::LoadString("TXT_REQ_CHANLIST", NULL, "D:\\Ra2\\wonline.cpp", 9092)); // RA2: expanded here
	if (!Request_Chat_Channel_List(true)) { // RA2: expanded here
		goto cleanup;
	}
	Set_Wait_Window_Text((LPARAM)StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 9099));
	{
		int joined = Join_Selected_Lobby(g_HasJoinedLobby && !first_time);
		g_HasJoinedLobby = true;
		if (joined == -1) {
			goto cleanup;
		}
	}
	{
		DWORD event;
		while ((event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100)) == WAIT_TIMEOUT) {
			WOL_Wait_Callback();
		}
		ResetEvent(g_WaitEventHandles[event]);
		if (event == EV_ABORT || event == EV_ERROR) {
			goto logout;
		}
	}
	g_JoinLobbyNow = 0;
	PostMessage(window, 0x593, 0, 0);
	Close_Wait_Window(0xFFFFFFFF); // RA2: expanded here
	SetFocus(MainWindow);
	ShowWindow(window, SW_SHOWNORMAL);
	SetFocus(window);
	SetFocus(GetDlgItem(window, 0x540));
	g_LastGameListTime = 0;
	g_LastChatListTime = time(0);
	for (;;) {
		DWORD event;
		while ((event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 20)) == WAIT_TIMEOUT) {
			WOL_Wait_Callback();
			if (g_JoinLobbyNow) { // RA2: the lobby is to be left and another one joined
				EnableWindow(GetDlgItem(window, 0x53D), FALSE);
				EnableWindow(GetDlgItem(window, 0x6EE), FALSE);
				EnableWindow(GetDlgItem(window, 0x62D), FALSE);
				EnableWindow(GetDlgItem(window, 0x62B), FALSE);
				EnableWindow(GetDlgItem(window, 0x689), FALSE);
				EnableWindow(GetDlgItem(window, 0x558), FALSE);
				EnableWindow(GetDlgItem(window, 0x61F), FALSE);
				EnableWindow(GetDlgItem(window, 0x686), FALSE);
				if (Join_Selected_Lobby(false) == -1) {
					EnableWindow(GetDlgItem(window, 0x686), TRUE);
					SetEvent(g_WaitEventHandles[EV_ERROR]);
				} else {
					PostMessage(window, 0x593, 0, 0);
				}
				g_JoinLobbyNow = 0;
			}
		}
		ResetEvent(g_WaitEventHandles[event]);
		if (event == EV_ABORT) {
			goto cleanup;
		}
		if (event != EV_EXIT) {
			if (event == EV_SKIRMISH) {
				result = 2;
				break;
			}
			if (event == EV_ERROR) {
				goto cleanup;
			}
			if (event == EV_STARTGAME) { // RA2: the game starts
				goto started;
			}
		} else {
			break;
		}
	}
	g_LastErrorMessage[0] = 0;
logout:
	WS_Destroy_Dialog(window, 0);
	WOL_Wait_Callback();
	window = NULL;
	if (g_pChat) {
		g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
	}
cleanup:
	Close_Wait_Window(0xFFFFFFFF); // RA2: expanded here
	ODMessageBox(g_LastErrorMessage, 0, Wait_Callback_Thunk, false);
	if (window) {
		WS_Destroy_Dialog(window, 0);
	}
	while (WS_Destroy_Dialog(NULL, 0)) {
	}
	if (result == 1) {
		g_PlayingNetGame = true;
	}
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
		g_WDTTeams.clear();
	}
	return(result);
started:
	result = 1;
	g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
	g_GameStartRequested = false;
	g_LastErrorMessage[0] = 0;
	goto cleanup;
}


int Pick_Lobby(bool require_room) // RA2: 0x00744090, not in TibSun: chooses the lobby channel to join. In a World Domination Tour game it is the one that carries the player's territory; else the lobby that was joined last, else the first matching lobby with room, else the least populated candidate (even when full); the index in the channel list, -1 when there is none
{
	int count = g_UserChannels.length();
	Channel channel;
	memset(&channel, 0, sizeof(channel));
	char section[32]; // RA2: the INI sections of the territory
	char entry[32];
	char fallback[32];
	char name[32];
	wchar_t territory[32]; // RA2: the name of the territory, wide
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: a World Domination Tour game
		memset(section, 0, sizeof(section));
		memset(territory, 0, 32); // RA2: only the first 32 bytes
		memset(entry, 0, sizeof(entry));
		memset(fallback, 0, sizeof(fallback));
		MixFileClass * archive = new MixFileClass("WDT.MIX", &FastKey); // RA2: the tour's archive is open while the INI is read
		{
			INIClass ini;
			CCFileClass file("WDTMAP.INI");
			ini.Load(file, false);
			sprintf(entry, "Territory%02d", Session.TournamentTerritory);
			strcpy(fallback, entry);
			if (Current_WDT_State()->MapID == 0) { // RA2: the map decides which of the two sections names the territory
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
	Wstring lobby_name = Wstring("Lob_33_") + Wstring_FromInt(g_LobbyNumber, 1); // RA2: the lobby that was joined last
	int i;
	for (i = 0; i < count; i++) { // RA2: the territory's lobby or the one joined last
		g_UserChannels.get(channel, i);
		if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
			if (wcscmp((wchar_t const *)g_Lobbies[i], territory) != 0) {
				continue;
			}
			if (!require_room || (int)channel.currentUsers <= 25) { // RA2: 25 players per lobby
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
	int best = -1; // RA2: the lobby with the fewest players
	unsigned int smallest = 10000;
	for (i = 0; i < count; i++) {
		g_UserChannels.get(channel, i);
		if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
			if (wcsncmp((wchar_t const *)g_Lobbies[i], territory, wcslen(territory)) != 0) {
				continue;
			}
			if (!require_room || (int)channel.currentUsers <= 25) {
				g_LobbyNumber = atol((char const *)channel.name + strlen("Lob_33_")); // RA2: the number is remembered
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

bool Leave_Current_Channel(unsigned int event, bool block) // RA2: 0x00744610, not in TibSun: leaves the channel the chat is in, waits up to 15 seconds for the server to confirm; true when no channel is left
{
	if (g_ChannelCount) {
		DebugString("Leave_Channel, channel %s type %d\n", g_CurrentChannel.name, g_CurrentChannel.type);
		g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
		{
			WideWstring title(StringTable::LoadString("WOL:LeavingChannel", NULL, "D:\\Ra2\\wonline.cpp", 9379)); // RA2: the wait dialog takes a wide text
			Show_Wait_Window(event, title, block);
		}
		for (int i = 0; g_ChannelCount && i < 150; i++) { // RA2: 150 steps of 100 ms
			Sleep(100);
			WOL_Wait_Callback();
		}
		Close_Wait_Window(0x40); // RA2: expanded here
	}
	return(g_ChannelCount == 0);
}

int Request_Channel_Join(Channel * channel, Wstring const & password, WideWstring title) // RA2: 0x00744700, not in TibSun: joins a channel (leaves the current one first); title is the text of the wait dialog; the chat's answer, 1 when the old channel could not be left
{
	char const * name = (char const *)channel->name;
	if (strcmp(name, (char const *)g_CurrentChannel.name) && strcmp(name, (char const *)g_PendingChannel.name)) { // RA2: not the channel the chat is in nor the one it was asked to join
		if (g_ChannelCount) { // RA2: as Leave_Current_Channel
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
			Close_Wait_Window(0x40); // RA2: expanded here
			if (g_ChannelCount) { // RA2: the channel was not left
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
		return g_pChat->RequestChannelJoin(channel); // RA2: virtual 0x20
	}
	return(0);
}

int Join_Selected_Lobby(bool join) // RA2: 0x00744930, not in TibSun: joins the lobby channel Pick_Lobby chose (the argument is its "needs room" flag); the lobby's index, -1 when none was picked or the join was refused
{
	int choice = Pick_Lobby(join);
	if (choice != -1) {
		Channel channel;
		g_UserChannels.get(channel, choice);
		int result;
		{
			Wstring password("zotclot9"); // RA2: the password of the lobby channels
			result = Request_Channel_Join(&channel, password, WideWstring(StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 9428))); // RA2: the wait text is passed by value
		}
		if (!result) {
			HWND dialog = WS_Find_Dialog(0x113); // RA2: the game list dialog gets the focus
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

namespace OwnerDraw { void __fastcall Slide_Dialog(HWND dialog, bool reverse); } // RA2: 0x005E5150, defined in ownrdraw.cpp
LRESULT CALLBACK Birthdate_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007494D0, the dialog proc of the birthdate dialog (0xE7)
LRESULT CALLBACK Account_Details_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007498E0, the dialog proc of the account dialogs (0xE6, 0xF3, 0xF4)
void __fastcall Show_Dialog(HWND dialog, int id); // RA2: 0x005FF7E0, defined in ownrdraw.cpp

void __fastcall Create_Wonline_Account(HWND parent) // RA2: 0x00744A00, not in TibSun: asks the birthdate, checks the age with the server, asks the nickname, password and emails and requests the new nickname
{
	OwnerDraw::Slide_Dialog(parent, false);
	Darken_Surface(NULL, true);
	char nickname[32];
	char password[32];
	char confirmation[32];
	char email[128];
	char parent_email[128];
	int newsletter = 1;
	int month = 1;
	int day = 0;
	int year = 0;
	int share_info;
	HWND dialog;
	for (int index = 0; index < 18; ++index) ResetEvent(g_WaitEventHandles[index]);
	memset(nickname, 0, sizeof(nickname));
	memset(password, 0, sizeof(password));
	memset(confirmation, 0, sizeof(confirmation));
	memset(email, 0, sizeof(email));
	memset(parent_email, 0, sizeof(parent_email));
	{
		WideWstring message(StringTable::LoadString("WOL:NewAccountFetch", NULL, "D:\\Ra2\\wonline.cpp", 9501)); // RA2: the wait text of the server list request
		Show_Wait_Window(0x80, message, false);
	}
	DebugString("Newnick update check\n");
	g_pChat->RequestServerList(g_GameSKU, 0x10006, "TibSun", "TibPass99", 35); // RA2: the login of the account server list request
	DWORD event;
	do {
		event = WaitForMultipleObjects(18, g_WaitEventHandles, false, 100);
		while (event == WAIT_TIMEOUT) {
			WOL_Wait_Callback();
			event = WaitForMultipleObjects(18, g_WaitEventHandles, false, 100);
		}
		ResetEvent(g_WaitEventHandles[event]);
	} while (event != 0 && event != 1);
	Close_Wait_Window(0xFFFFFFFF);
	if (event == 0) { // RA2: the server list request failed
		ShowWindow(parent, SW_SHOW);
		OwnerDraw::Set_Paint_Disabled(parent, 0);
		return;
	}
	if (event == 8) { // RA2: the game was left meanwhile, the event stays set
		SetEvent(g_WaitEventHandles[8]);
		ShowWindow(parent, SW_SHOW);
		OwnerDraw::Set_Paint_Disabled(parent, 0);
		return;
	}
birthdate_retry: // RA2: the retry is written as a jump, as in the reference body; a loop form has not been tried yet
	Darken_Surface(NULL, true);
	dialog = WS_Create_Dialog(ProgramInstance, 0xE7, MainWindow, (DLGPROC)Birthdate_Dialog_Proc, 0); // RA2: the birthdate dialog
	Show_Dialog(dialog, 0xE7);
	SendDlgItemMessage(dialog, 0x62E, CB_SETCURSEL, month - 1, 0); // RA2: the month list
	if (day > 0) { // RA2: the day edit
		Wstring text = Wstring_FromInt(day);
		char const * number = text.get();
		HWND edit = GetDlgItem(dialog, 0x62F);
		SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)number);
	} else {
		HWND edit = GetDlgItem(dialog, 0x62F);
		SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM) "");
	}
	if (year > 0) { // RA2: the year edit
		Wstring text = Wstring_FromInt(year);
		char const * number = text.get();
		HWND edit = GetDlgItem(dialog, 0x630);
		SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)number);
	} else {
		HWND edit = GetDlgItem(dialog, 0x630);
		SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM) "");
	}
	SendDlgItemMessage(dialog, 0x626, WM_USER + 0xB4, 0, (LPARAM)email); // RA2: the email edit
	SendDlgItemMessage(dialog, 0x631, BM_SETCHECK, newsletter, 0); // RA2: the newsletter box
	ShowWindow(parent, SW_HIDE);
	ShowWindow(dialog, SW_SHOWNORMAL);
	if (WS_Wait_Dialog(dialog, Wait_Callback_Thunk, false, true, true) == 1) { // RA2: the birthdate was entered
		email[0] = 0; // RA2: the answers of the birthdate dialog
		strcpy(email, g_BirthdateEmail.get());
		month = g_BirthdateMonth + 1;
		day = g_BirthdateDay;
		year = g_BirthdateYear;
		newsletter = g_BirthdateNewsletter;
		if (month <= 0 || day <= 0 || year <= 0 || strlen(email) < 6) { // RA2: a field is missing
			ODMessageBox(StringTable::LoadString("TXT_REQUIRED_FIELD", NULL, "D:\\Ra2\\wonline.cpp", 9595), 0, Wait_Callback_Thunk, false);
			goto birthdate_retry;
		}
		g_UserConsent = 0; // RA2: the age check fills both
		g_UserAge = -1;
		if (g_pNetUtil->RequestAgeCheck(month, day, year, email) != 0) {
			ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 9603), 0, Wait_Callback_Thunk, false);
			goto account_retry;
		}
		{
			WideWstring message(StringTable::LoadString("WOL:RequestingNickname", NULL, "D:\\Ra2\\wonline.cpp", 9606)); // RA2: the wait text of the age check
			Show_Wait_Window(0x100, message, true);
		}
		if (g_UserAge < 0) { // RA2: the age check failed
			if (g_AgeCheckResult == (int)0x80040100) { // RA2: a field was missing
				ODMessageBox(StringTable::LoadString("TXT_REQUIRED_FIELD", NULL, "D:\\Ra2\\wonline.cpp", 9610), 0, Wait_Callback_Thunk, false);
			} else {
				ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 9612), 0, Wait_Callback_Thunk, false);
			}
			goto birthdate_retry;
		}
		if (g_UserAge < 13 && !g_UserConsent) { // RA2: a child needs the consent of the parents
			if (ODMessageBox(StringTable::LoadString("TXT_CONSENT_REQUIRED", NULL, "D:\\Ra2\\wonline.cpp", 9621), MB_YESNO, Wait_Callback_Thunk, false) == IDYES) {
				char url[256];
				sprintf(url, "http://apiregister.westwood.com/consent_form/index_%d.html", g_LanguageCode);
				ViewHTML(url, 1);
			}
			goto birthdate_retry;
		}
account_retry: // RA2: the retry is written as a jump, as in the reference body; a loop form has not been tried yet
		int resource; // RA2: the account dialog of the age group
		if (g_UserAge < 13) resource = 0xE6;
		else resource = g_UserAge >= 18 ? 0xF4 : 0xF3;
		dialog = WS_Create_Dialog(ProgramInstance, resource, MainWindow, (DLGPROC)Account_Details_Dialog_Proc, 0);
		Show_Dialog(dialog, resource);
		if (nickname && strlen(nickname) > 0) { // RA2: the nickname edit
			HWND edit = GetDlgItem(dialog, 0x628);
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)nickname);
		} else {
			HWND edit = GetDlgItem(dialog, 0x628);
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM) "");
		}
		if (parent_email && strlen(parent_email) > 0) { // RA2: the parent's email edit
			HWND edit = GetDlgItem(dialog, 0x5E5);
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)parent_email);
		} else {
			HWND edit = GetDlgItem(dialog, 0x5E5);
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM) "");
		}
		if (password && strlen(password) > 0) { // RA2: the password edit
			HWND edit = GetDlgItem(dialog, 0x72C);
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)password);
		} else {
			HWND edit = GetDlgItem(dialog, 0x72C);
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM) "");
		}
		if (confirmation && strlen(confirmation) > 0) { // RA2: the repeated password edit
			HWND edit = GetDlgItem(dialog, 0x627);
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM)confirmation);
		} else {
			HWND edit = GetDlgItem(dialog, 0x627);
			SendMessage(edit, WM_USER + 0xB4, 0, (LPARAM) "");
		}
		ShowWindow(dialog, SW_SHOWNORMAL);
		SendMessage(dialog, WM_USER + 0xA9, 0, 1);
		if (WS_Wait_Dialog(dialog, Wait_Callback_Thunk, false, true, true) != 1) goto birthdate_retry; // RA2: cancelled, back to the birthdate
		nickname[0] = 0; // RA2: the answers of the account dialog
		strcpy(nickname, g_AccountName.get());
		password[0] = 0;
		strcpy(password, g_AccountPassword.get());
		confirmation[0] = 0;
		strcpy(confirmation, g_AccountPasswordCheck.get());
		parent_email[0] = 0;
		if (resource == 0xF3) strcpy(parent_email, g_AccountParentEmail.get()); // RA2: only the teenagers' dialog asks for it
		share_info = 0;
		if (resource == 0xF4) share_info = g_AccountShareInfo; // RA2: only the adults' dialog asks for it
		if (strcmp(password, confirmation)) {
			ODMessageBox(StringTable::LoadString("TXT_PASSWORD_VERIFY", NULL, "D:\\Ra2\\wonline.cpp", 9727), 0, Wait_Callback_Thunk, false);
			goto account_retry;
		}
		if (strlen(password) != 8) {
			ODMessageBox(StringTable::LoadString("TXT_PASSWORD_TOO_SHORT", NULL, "D:\\Ra2\\wonline.cpp", 9732), 0, Wait_Callback_Thunk, false);
			goto account_retry;
		}
		if (g_pNetUtil->RequestNewNick(nickname, password, email, parent_email, newsletter, share_info) != 0) {
			ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 9737), 0, Wait_Callback_Thunk, false);
		} else {
			{
				WideWstring message(StringTable::LoadString("TXT_REQ_NICK", NULL, "D:\\Ra2\\wonline.cpp", 9739)); // RA2: the wait text of the nickname request
				Show_Wait_Window(0x100, message, true);
			}
			if (g_NewNickResult < 0) goto account_retry; // RA2: the nickname was refused
		}
		ShowWindow(parent, SW_SHOW);
		OwnerDraw::Set_Paint_Disabled(parent, 0);
		return;
	} else {
		ShowWindow(parent, SW_SHOWNORMAL); // RA2: the dialog was cancelled
	}
	ShowWindow(parent, SW_SHOW);
	OwnerDraw::Set_Paint_Disabled(parent, 0);
}

int Save_Nickname(char const * name, char const * password, bool mangle, int locale) // RA2: 0x00745320, not in TibSun: stores the persona in the chat API's list of 32 logins (the slot it already has, else an empty one, else the next of a round robin) and its locale; the slot
{
	int slot = 0;
	char const * nick;
	char const * stored;
	for (int i = 1; i <= 32; i++) { // RA2: the slot that already holds the persona
		nick = NULL;
		stored = NULL;
		if (g_pChat->GetNick(i, &nick, &stored) == 0 && nick && !strcmp(nick, name)) {
			slot = i;
			break;
		}
	}
	if (slot == 0) { // RA2: else the first empty one
		for (int i = 1; i <= 32; i++) {
			nick = NULL;
			stored = NULL;
			if (g_pChat->GetNick(i, &nick, &stored) != 0 || !nick || strlen(nick) == 0) {
				slot = i;
				break;
			}
		}
	}
	if (slot == 0) { // RA2: else the next slot in turn
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
		g_pChat->SetNick(slot, name, password, mangle); // RA2: virtual 0x94
		g_pChat->SetNickLocale(slot, (Locale)locale); // RA2: virtual 0xC8
	}
	return(slot);
}

Wstring Server_Name_Without_Prefix(Server const * server) // RA2: 0x00745450, not in TibSun: the name of the server after the first colon (the part before it names the kind of server)
{
	Wstring name((char const *)server->name); // RA2: the server's name is copied into a Wstring
	Wstring token;
	int position = name.getToken(0, ":", token); // RA2: the first token ends at the first colon
	if (position >= 0) {
		name.remove(0, position + 1);
	}
	return(name);
}

#define OD_ADDSTRING_COMBO (WM_USER + 188) // RA2: 0x4BC, appends the string of lParam to an owner drawn combo box and returns its row (name guessed from its use)
#define OD_ADDSTRING_LIST (WM_USER + 200) // RA2: 0x4C8, the same for an owner drawn list box (name guessed from its use)
int __fastcall Clear_ComboBox(HWND list); // RA2: 0x006007F0, removes every row of an owner drawn combo box, defined in ownrdraw.cpp

bool Populate_Server_Choices(HWND list, bool combo) // RA2: 0x007454C0, not in TibSun: fills a list box (or combo box) with the names of the chat servers and selects the one of the persona's profile; false when the server list is empty
{
	if (combo) {
		Clear_ComboBox(list);
	} else {
		Clear_Listbox(list);
	}
	if (g_Servers.length() == 1) { // RA2: only the entry 0 of the list: no server to choose from
		if (combo) {
			SendMessage(list, OD_ADDSTRING_COMBO, 0, (LPARAM) "GUI:NoServersAvailable");
		} else {
			SendMessage(list, OD_ADDSTRING_LIST, 0, (LPARAM) "GUI:NoServersAvailable");
		}
		EnableWindow(list, FALSE);
		return(false);
	}
	Wstring preferred; // RA2: the server of the persona's profile
	WOLOptionsClass options; // RA2: as in Connect_To_Server
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
		Wstring display = Server_Name_Without_Prefix(server); // RA2: expanded here
		int row;
		if (combo) {
			row = SendMessage(list, OD_ADDSTRING_COMBO, 0, (LPARAM)display.text());
			SendMessage(list, CB_SETITEMDATA, row, i);
		} else {
			row = SendMessage(list, OD_ADDSTRING_LIST, 0, (LPARAM)display.text());
			SendMessage(list, LB_SETITEMDATA, row, i);
		}
		if (preferred.length() > 0) {
			bool match = ((Wstring const &)preferred).toLower() == ((Wstring const &)display).toLower(); // RA2: the const toLower returns a copy
			if (match) {
				selected = row;
				g_CurrentServerIndex = i; // RA2: the server of the profile becomes the one to connect to
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

bool __fastcall Can_Join_Channel(Channel const * channel, bool check, bool observer); // RA2: 0x00755CD0, whether the player may join the game channel (the version, the full game, the observer slot ...); defined below
bool Test_Display_Mode_Dialog(int width, int height); // RA2: 0x00543A70, defined in mainopt.cpp
LRESULT CALLBACK Wonline_Password_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x007492D0, the dialog proc of the password dialog (0xE1)
void Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, defined in init.cpp

bool __fastcall Join_Game_Channel(Channel const * channel, bool observer) // RA2: 0x00745780, not in TibSun: joins a game channel: tests that the player may (Can_Join_Channel), that the screen is not wider than the game's resolution limit (offering to switch to 800x600 or 640x480), asks for the password of a private game; true when the join was requested
{
	if (!Can_Join_Channel(channel, true, observer)) {
		return(false);
	}
	unsigned int mode = channel->reserved; // RA2: the flags of the game: the resolution limit
	int maximum = (mode & 0x400) ? 800 : ((mode & 0x200) ? 640 : 1024);
	if (VideoModeWidth > maximum) { // RA2: the screen is wider than the game allows
		{
			WideWstring warning((mode & 0x400) ? StringTable::LoadString("WOL:ResolutionLimited800", NULL, "D:\\Ra2\\wonline.cpp", 10143) : StringTable::LoadString("WOL:ResolutionLimited640", NULL, "D:\\Ra2\\wonline.cpp", 10144));
			if (ODMessageBox(warning.get(), 4, Wait_Callback_Thunk, true) == 6) { // RA2: IDYES: the screen is switched
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
	if (channel->flags & 0x100) { // RA2: a private game: the player types the password
		Darken_Surface(NULL, true);
		WonlineStringDialogControl control;
		if (OwnerDraw::Controlled_Dialog(0xE1, (DLGPROC)Wonline_Password_Dialog_Proc, &control) == 2) { // RA2: cancelled
			return(false);
		}
		password = control.Text;
		password.replace(" ", "_"); // RA2: spaces are not allowed in the key
	}
	return(Request_Channel_Join((Channel *)channel, password, WideWstring(StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 10199))) == 0);
}

LRESULT CALLBACK Create_Game_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00748E10, the dialog proc of the create game dialog (0xC3); not yet defined
BOOL CALLBACK Wonline_Game_Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x0074A710, the dialog proc of the game options dialog (0xC2); not yet defined
void __fastcall Seat_First_Player(int slot); // RA2: 0x005C8CD0, defined in netshare.cpp
void Clear_Vector(DynamicVectorClass<NodeNameType *> * vector); // RA2: 0x005A7480, defined in mplayer.cpp
MultiplayerGameMode * First_Game_Mode(void); // RA2: 0x005B6EC0, defined in multiplayer.cpp
namespace OwnerDraw {
void __fastcall Slide_Dialog(HWND dialog, bool reverse); // RA2: 0x005E5150, defined in ownrdraw.cpp
}

static inline void Reset_Session_Players(void) // RA2: the players of the session are cleared (expanded in Host_Game)
{
	MultiplayerGameMode * mode = First_Game_Mode();
	Session.MPGameMode = mode;
	Session.Options.MPModeIndex = mode->MPModeIndex;
	mode->vt_entry_20();
	Set_Scenario_Info_From_Index(0);
	Clear_Vector(&Session.Players);
	Clear_Slot_Lists();
	Reset_Slots();
}

static inline void Show_Game_Options(void) // RA2: opens the game options dialog (0xC2) of a game the player hosts (expanded in Host_Game)
{
	if (!Is_Tournament_Session()) {
		HWND top = WS_Top_Window();
		if (top) {
			OwnerDraw::Slide_Dialog(top, false);
			Darken_Surface(NULL, true);
		}
		Darken_Surface(NULL, true);
		HWND dialog = OwnerDraw::Begin_Dialog(0xC2, (DLGPROC)Wonline_Game_Options_Dialog_Proc, 0);
		Session.MPGameMode->vt_entry_84();
		Session.MPGameMode->vt_entry_20();
		Session.MPGameMode->AssignTeamsAtStart();
		Session.MPGameMode->UnfixAlliances();
		ShowWindow(dialog, SW_SHOWNORMAL);
		if (top) {
			ShowWindow(top, SW_HIDE);
		}
	}
}

bool __fastcall Host_Game(void) // RA2: 0x007459A0, not in TibSun: hosts a game: asks for the game's settings (the create game dialog 0xC3, unless it is an Internet tournament game), makes the host the first player of the lobby, asks the chat service for the game channel and opens the game options dialog (0xC2); true when the channel was created
{
	Wstring password; // RA2: the key of the game channel
	if (VideoModeWidth >= 1024) {
		Session.WOLLimitResolution = false;
	}
	CreateGameDialogControl control(Session.WOLLimitResolution, ObserverMode == 1);
	g_GameStartRequested = false;
	if (Is_Tournament_Session()) { // RA2: a tournament game has two players and no dialog
		g_MaxPlayers = 2;
	} else {
		Darken_Surface(NULL, true);
		if (OwnerDraw::Controlled_Dialog(0xC3, (DLGPROC)Create_Game_Dialog_Proc, &control) == 2) { // RA2: cancelled
			return(false);
		}
		password = control.Text;
		password.replace(" ", "_"); // RA2: spaces are not allowed in the key
	}
	g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
	Channel channel; // RA2: the game channel
	memset(&channel, 0, sizeof(channel));
	sprintf((char *)channel.name, "%s's_game", *(char **)&g_NickName); // RA2: the executable reads the string pointer of the Wstring itself (no accessor call)
	channel.type = 0x21; // RA2: a game channel
	channel.minUsers = 1;
	if (g_MaxPlayers < 2) {
		g_MaxPlayers = 4;
	}
	strcpy((char *)channel.key, password.get());
	channel.maxUsers = g_MaxPlayers;
	char info[80]; // RA2: the info text of the channel
	memset(info, 0, sizeof(info));
	Encode_Channel_ExInfo(info);
	info[40] = 0; // RA2: the info member holds 40 characters
	strcpy((char *)channel.exInfo, info);
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) { // RA2: an Internet tournament game
		channel.tournament = 1;
		channel.reserved |= 0x1000;
	} else if (Is_Tournament_Session()) {
		channel.tournament = 1;
		channel.reserved |= 0x800;
	} else {
		channel.tournament = control.IsTournament;
		if (control.IsPrivate) {
			channel.reserved |= 0x100;
		}
		Session.WOLLimitResolution = control.LimitResolution;
		if (control.LimitResolution) {
			channel.reserved |= (VideoModeWidth == 800) ? 0x400 : 0x200;
		}
	}
	channel.reserved = channel.reserved + g_LobbyNumber; // RA2: the lobby the host came from
	Leave_Current_Channel(0xC0, false); // RA2: expanded here
	Reset_Session_Players(); // RA2: expanded here
	NodeNameType * host = new NodeNameType; // RA2: the entry of the host
	memset(host, 0, sizeof(NodeNameType));
	WideWstring name = Wstring_ToWide(g_NickName);
	wcscpy(host->Name, name.get());
	WOLOptionsClass options;
	options.Load();
	rc_ptr<PlayerProfile> profile;
	if (options.Is_Bit1_Clear()) {
		profile = PlayerProfile::Load(g_NickName);
	} else {
		profile = g_PlayerProfile.Get();
	}
	if (profile.Get()) { // RA2: the side and color of the persona
		Session.Set_Side(profile->Country, false);
		Session.Set_Color(profile->Color, false);
	}
	host->Set_Country(Session.Chosen_House());
	host->Set_Color(Session.Chosen_Color());
	host->Slot.Unknown77 = 1;
	host->Slot.SpectatorFlag = 0;
	LobbyObserverSlot = 7;
	ObserverMode = control.Observer;
	Update_Observer_Buttons(0);
	if (ObserverMode) { // RA2: the host observes: the first slot, team -1, house -3
		LobbyObserverSlot = 0;
		host->Slot.SpectatorFlag = -1;
		host->Set_Country(-3);
	}
	Session.Players.Add(host);
	Seat_First_Player(0);
	wcscpy(Session.GameName, name.get());
	HRESULT result = g_pChat->RequestChannelCreate(&channel); // RA2: virtual 0x1C
	if (result == 0x800401F8) { // RA2: the key was refused
		ODMessageBox(StringTable::LoadString("TXT_BAD_CHANKEY", NULL, "D:\\Ra2\\wonline.cpp", 10434), 0, Wait_Callback_Thunk, false);
		Close_Wait_Window(0x80); // RA2: expanded here
		return(false);
	}
	if (result != 0) {
		ODMessageBox(StringTable::LoadString("TXT_CANTCREATE_CHAN", NULL, "D:\\Ra2\\wonline.cpp", 10440), 0, Wait_Callback_Thunk, false);
		Close_Wait_Window(0x80); // RA2: expanded here
		return(false);
	}
	WideWstring title(StringTable::LoadString("GUI:CreatingChannel", NULL, "D:\\Ra2\\wonline.cpp", 10448)); // RA2: the wait dialog takes a wide text
	Show_Wait_Window(4, title, false);
	Close_Wait_Window(0x80); // RA2: expanded here
	Show_Wait_Window(4, title, true);
	int index = Player_Name_To_Index(g_NickName); // RA2: expanded here
	if (index < 0) { // RA2: the host is not in the list of users
		return(false);
	}
	User * user = NULL;
	g_UserList.getPointer(&user, index);
	{ // RA2: the address of the host from its IP, built in place (main's exact text; TibSun writes the same statements out where it fills a player's address)
		NetNumType net;
		NetNodeType node;
		memset(net, 0, sizeof(net));
		memset(node, 0, sizeof(node));
		unsigned int ip_addr = ntohl(user->ipaddr);
		node[3] = ip_addr & 0xff;
		node[2] = (ip_addr >> 8) & 0xff;
		node[1] = (ip_addr >> 16) & 0xff;
		node[0] = (ip_addr >> 24) & 0xff;
		IPXAddressClass address(net, node, 0);
		host->Address = address;
	}
	Show_Game_Options(); // RA2: expanded here
	return(true);
}


int __fastcall Parse_Online_Command(wchar_t const * text) // RA2: 0x00746240, not in TibSun: the lobby chat command that looks a player up (TXT_ONLINE_COMMAND followed by a nick): 1 when the command was handled
{
	int handled = 0;
	wchar_t command[64];
	wcscpy(command, StringTable::LoadString("TXT_ONLINE_COMMAND", 0, "D:\\Ra2\\wonline.cpp", 10505));
	if (wcsncmp(text, command, wcslen(command)) == 0) {
		int count = 0;
		wchar_t const * cursor = text + wcslen(command);
		while (wcsncmp(cursor, L" ", 1) == 0) {
			++cursor;
		}
		wchar_t name[16];
		while (*cursor) {
			if (wcsncmp(cursor, L" ", 1) == 0 || count >= 10) {
				break;
			}
			name[count++] = *cursor++;
		}
		name[count] = 0;
		if (wcslen(name) > 0) {
			User user;
			strcpy((char *)user.name, Wide_To_Narrow(name));
			strcpy(LastFindName, Wide_To_Narrow(name));
			g_pChat->RequestFind(&user); // RA2: virtual 0x60
			handled = 1;
		} else {
			wchar_t message[256];
			swprintf(message, StringTable::LoadString("TXT_NO_NAME", 0, "D:\\Ra2\\wonline.cpp", 10525));
			HWND window = WS_Top_Window();
			if (window) {
				if (GetDlgItem(window, 0x53F)) {
					Lobby_Chat_Printf(ColorSystem, message);
				}
				if (GetDlgItem(window, 0x5B5)) {
					SMessagePrintf(ColorSystem, message);
				}
			}
			if (Session.Is_In_Game()) {
				int color = GadgetClass::Get_Color_Scheme();
				Session.Messages.Add_Message(0, 0, message, color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
			}
			handled = 1;
		}
	}
	return(handled);
}


int __fastcall Parse_Page_Command(wchar_t const * text) // RA2: 0x00746480, not in TibSun: the lobby chat command that pages a player (TXT_PAGE_COMMAND, a nick and the text): 1 when handled, 2 when a message was shown instead
{
	int result = 0;
	wchar_t nickname[16];
	User recipient;
	wchar_t command[128];
	wcscpy(command, StringTable::LoadString("TXT_PAGE_COMMAND", 0, "D:\\Ra2\\wonline.cpp", 10559));
	if (_wcsnicmp(text, command, wcslen(command)) == 0) {
		int name_length = 0;
		unsigned length = wcslen(text) + 1;
		wchar_t * copy = (wchar_t *)operator new(length * 2);
		if (!copy) {
			return(0);
		}
		copy = wcsncpy(copy, text, length);
		if (!copy) {
			return(0);
		}
		wchar_t * message = copy + wcslen(command);
		while (*message && wcsncmp(message, L" ", 1) == 0) {
			++message;
		}
		while (*message) {
			if (wcsncmp(message, L" ", 1) == 0 || name_length > 10) {
				break;
			}
			nickname[name_length++] = *message++;
		}
		nickname[name_length] = 0;
		while (*message && wcsncmp(message, L" ", 1) == 0) {
			++message;
		}
		if (name_length > 10) {
			result = 1;
		} else if (name_length == 0) {
			result = 1;
		} else {
			bool recipient_is_player = false;
			if (MuteSWLaunches) {
				for (int i = 0; i < Session.Players.Count(); ++i) {
					if (_wcsicmp(Session.Players[i]->Name, nickname) == 0) {
						recipient_is_player = true;
						break;
					}
				}
			}
			if (!recipient_is_player) {
				if (wcslen(message) > 0) {
					strcpy((char *)recipient.name, Wide_To_Narrow(nickname));
					g_pChat->RequestUnicodePage(&recipient, message); // RA2: virtual 0xF8
				}
				result = 1;
			} else {
				int color = GadgetClass::Get_Color_Scheme();
				Session.Messages.Add_Message(0, 0, StringTable::LoadString("TXT_OBSERVER_CANT_PAGE", 0, "D:\\Ra2\\wonline.cpp", 10619), color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
				result = 2;
			}
		}
		operator delete(copy);
	}
	return(result);
}


int __fastcall Reply_To_Last_Pager(wchar_t const * text) // RA2: 0x007466C0, not in TibSun: the lobby chat command that answers the last pager (TXT_REPLY_COMMAND or its short form, then the text): 1 when handled, 2 when a message was shown instead
{
	int result = 0;
	wchar_t reply_command[128];
	User recipient;
	wchar_t short_command[128];
	wchar_t notice[128];
	wcscpy(reply_command, StringTable::LoadString("TXT_REPLY_COMMAND", 0, "D:\\Ra2\\wonline.cpp", 10639));
	wcscpy(short_command, StringTable::LoadString("TXT_REPLY_COMMAND_SHORT", 0, "D:\\Ra2\\wonline.cpp", 10640));
	if (_wcsnicmp(text, reply_command, wcslen(reply_command)) == 0 || wcsncmp(text, short_command, wcslen(short_command)) == 0) {
		unsigned length = wcslen(text) + 1;
		wchar_t * copy = (wchar_t *)operator new(length * 2);
		if (!copy) {
			return(0);
		}
		copy = wcsncpy(copy, text, length);
		if (!copy) {
			return(0);
		}
		wchar_t * message;
		if (wcsncmp(text, reply_command, wcslen(reply_command)) == 0) {
			message = copy + wcslen(reply_command);
		} else {
			message = copy + wcslen(short_command);
		}
		while (*message && wcsncmp(message, L" ", 1) == 0) {
			++message;
		}
		bool recipient_is_player = false;
		if (MuteSWLaunches) {
			for (int i = 0; i < Session.Players.Count(); ++i) {
				if (_wcsicmp(Session.Players[i]->Name, Narrow_To_Wide(LastPagerName)) == 0) {
					recipient_is_player = true;
					break;
				}
			}
		}
		if (!recipient_is_player) {
			if (strlen(LastPagerName) > 0) {
				if (wcslen(message) > 0) {
					strcpy((char *)recipient.name, LastPagerName);
					g_pChat->RequestUnicodePage(&recipient, message); // RA2: virtual 0xF8
				}
				result = 1;
			} else {
				wcscpy(notice, StringTable::LoadString("TXT_NO_ONE_WHOM_TO_REPLY", 0, "D:\\Ra2\\wonline.cpp", 10691));
				HWND window = WS_Top_Window();
				if (window) {
					if (GetDlgItem(window, 0x53F)) {
						Lobby_Chat_Printf(ColorSystem, notice);
					}
					if (GetDlgItem(window, 0x5B5)) {
						SMessagePrintf(ColorSystem, notice);
					}
				}
				if (Session.Is_In_Game()) {
					int color = GadgetClass::Get_Color_Scheme();
					Session.Messages.Add_Message(0, 0, notice, color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
				}
				result = 2;
			}
		} else {
			int color = GadgetClass::Get_Color_Scheme();
			Session.Messages.Add_Message(0, 0, StringTable::LoadString("TXT_OBSERVER_CANT_PAGE", 0, "D:\\Ra2\\wonline.cpp", 10715), color, TextPrintType(0x16), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
			result = 2;
		}
		operator delete(copy);
	}
	return(result);
}

void __fastcall Dialog_On_WM_PAINT(HWND dialog); // RA2: 0x005FEE70, before a dialog paints (netdlg2.cpp declares it the same way)
void __fastcall Dialog_On_WM_DRAWITEM(long item); // RA2: 0x005FE380
WideWstring __fastcall Limit_Name_Text(HWND control); // RA2: 0x005CCF50, defined in netshare.cpp
void __fastcall Get_Window_Wide_Text(HWND window, WideWstring & text); // RA2: 0x00600E50
void __fastcall Get_Window_Text(HWND window, Wstring & text); // RA2: 0x00600DF0, the narrow form of Get_Window_Wide_Text
void __fastcall Save_Observer_Mode(void); // RA2: 0x005CD3A0, defined in netshare.cpp
void __fastcall Update_Observer_Buttons(HWND dialog); // RA2: 0x005CD540, defined in netshare.cpp
void __fastcall Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, defined in init.cpp
int __fastcall Get_Scenario_Slot_Limit(int index); // RA2: 0x005C65A0, defined in netshare.cpp
bool __fastcall Is_AI_Entry(NodeNameType * entry); // RA2: 0x005C9370, defined in netshare.cpp
int __fastcall Find_Slot_By_Name(wchar_t const * name); // RA2: 0x005C8C30, defined in netshare.cpp
namespace OwnerDraw { void __fastcall Set_Dialog_Waiting(HWND control); } // RA2: 0x005E6390, defined in ownrdraw.cpp
int CALLBACK WOL_Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00749B50, the options dialog (700)

int g_NewAllowPage; // RA2: the options the options dialog (700) was closed with; the lobby takes them unless it was cancelled
int g_NewAllowFind;
int g_NewLangFilter;
int g_NewLobbyMusic;
int g_NewShowAllChannels;
int g_NewDisplayAsian;
int g_NewDisplayLatin;
int g_AccountInputEvents; // RA2: counts the tab key presses of the account dialogs
void * g_OptionsDialogMemory; // RA2: freed when the options dialog (700) closes; nothing here allocates it
wchar_t g_PingTooltip[256]; // RA2: the text Player_Ping_Tooltip returns


int g_LastJoinedGameIndex = -1; // RA2: the game list entry the join button last tried
int g_LastSelectedGameIndex = -1; // RA2: the game list entry last selected
char g_LastSelectedGameName[17]; // RA2: the name of the game channel last selected

// RA2: 0x007469C0, the lobby dialog (0x113). ra2-decomp's twin text (Yuri's Revenge 0x0078AC10, matched there, 71abb6a)
// in this tree's names with RA2's cases; where a case leaves through the shared return or a break follows the
// earlier tree's accepted text (src-final), which the image's shared SetFocus tail needs.
BOOL CALLBACK Wonline_Main_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc) {
		return rc;
	}
	switch (message) {
		case WM_TIMER:
			switch (wparam) {
				case 0x55D:
					KillTimer(window, 0x55D);
					EnableWindow(GetDlgItem(window, 0x55D), TRUE);
					return FALSE;

				case 1:
					if (WS_Top_Window() == window && !Request_Chat_Channel_List(false)) {
						SetEvent(g_WaitEventHandles[5]);
					}
					break;

				case 0:
					if (WS_Top_Window() == window && !Request_Game_Channel_List(1, false)) {
						SetEvent(g_WaitEventHandles[5]);
					}
					break;
			}
			break;

		case 0x593:
			EnableWindow(GetDlgItem(window, 0x53D), FALSE);
			if (!Request_Chat_Channel_List(true) || !Request_Game_Channel_List(1, false)) {
				SetEvent(g_WaitEventHandles[5]);
			}
			EnableWindow(GetDlgItem(window, 0x53D), TRUE);
			break;

		case WM_DESTROY:
			KillTimer(window, 0);
			KillTimer(window, 0x55D);
			Save_Observer_Mode();
			return FALSE;

		case WM_DRAWITEM:
			Dialog_On_WM_DRAWITEM(lparam);
			return TRUE;

		case WM_ERASEBKGND:
			return TRUE;

		case WM_PAINT:
			Dialog_On_WM_PAINT(window);
			Slide_Pending_Dialog(window);
			ValidateRect(window, NULL);
			return FALSE;

		case WM_SHOWWINDOW:
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case 0x53D: {
					int index = SendMessage((HWND)lparam, LB_GETCURSEL, 0, 0);
					if (index == -1) {
						return FALSE;
					}
					int extra = SendMessage((HWND)lparam, LB_GETITEMDATA, index, 0);
					if (HIWORD(wparam) == LBN_SELCHANGE && extra != -1) {
						Channel chan;
						if (g_UserChannels.get(chan, extra)) {
							if (strcmp((char *)chan.name, (char *)g_CurrentChannel.name) != 0 && strcmp((char *)chan.name, (char *)g_PendingChannel.name) != 0) {
								g_JoinLobbyNow = 1;
								g_LobbyNumber = atol((char *)chan.name + strlen("Lob_33_"));
								SetEvent(g_WaitEventHandles[9]);
								return FALSE;
							}
						}
					}
					return FALSE;
				}

				case 0x53E:
					return FALSE;

				case 0x3ED: {
					HWND input = GetDlgItem(window, 0x540);
					WideWstring text = Limit_Name_Text(input);
					SendMessage(input, 0x4B2, 0, (LPARAM)L"");
					SetFocus(input);
					InvalidateRect(input, NULL, FALSE);
					int color = ColorAction;
					if (strlen((char *)g_CurrentChannel.name) == 0) {
						Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_NOT_IN_CHAN", NULL, "D:\\Ra2\\wonline.cpp", 0x2C52));
					} else if (text.length() > 0) {
						if (Send_Chat_Action(text)) {
							color = ColorPrivAction;
						}
						wchar_t * message_text = text.get();
						Lobby_Chat_Printf(color, L"%hs %s", g_NickName.c_str(), message_text);
					} else {
						Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_ENTER_MESSAGE", NULL, "D:\\Ra2\\wonline.cpp", 0x2C5A));
					}
					return FALSE;
				}

				case 0x540: {
					wchar_t nickname[16];
					wchar_t primary_command[64];
					wchar_t alternate_command[64];
					HWND input = GetDlgItem(window, 0x540);
					WideWstring text = Limit_Name_Text(input);
					if (HIWORD(wparam) == 0x501) {
						SendMessage(input, 0x4B2, 0, (LPARAM)L"");
						if (text.length() > 0) {
							int parsed = Parse_Page_Command(text.get());
							if (parsed != 0) {
								if (parsed == 1) {
									int count = 0;
									unsigned length = text.length() + 1;
									wchar_t * copy = (wchar_t *)operator new(length * 2);
									wcscpy(primary_command, StringTable::LoadString("TXT_PAGE_COMMAND", NULL, "D:\\Ra2\\wonline.cpp", 0x2BD9));
									if (!copy) {
										return FALSE;
									}
									copy = wcsncpy(copy, text.get(), length);
									if (!copy) {
										return FALSE;
									}
									wchar_t * body = copy + wcslen(primary_command);
									while (*body && wcsncmp(body, L" ", 1) == 0) {
										++body;
									}
									while (*body) {
										if (wcsncmp(body, L" ", 1) == 0 || count > 10) {
											break;
										}
										nickname[count++] = *body++;
									}
									nickname[count] = 0;
									while (*body && wcsncmp(body, L" ", 1) == 0) {
										++body;
									}
									if (count > 10) {
										Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_NAME_TOO_LONG", NULL, "D:\\Ra2\\wonline.cpp", 0x2BF7));
									} else if (count == 0) {
										Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_NO_NAME_SPECIFIED", NULL, "D:\\Ra2\\wonline.cpp", 0x2BF9));
									} else if (wcslen(body) > 0) {
										Lobby_Chat_Printf(ColorPriv, L"[%hs > %s] %s", g_NickName.c_str(), nickname, body);
									} else {
										Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_CANT_SEND_BLANK_PAGE", NULL, "D:\\Ra2\\wonline.cpp", 0x2BFF));
									}
									operator delete(copy);
								}
							} else if (Reply_To_Last_Pager(text.get())) {
								unsigned length = text.length() + 1;
								wchar_t * copy = (wchar_t *)operator new(length * 2);
								wcscpy(primary_command, StringTable::LoadString("TXT_REPLY_COMMAND", NULL, "D:\\Ra2\\wonline.cpp", 0x2C11));
								wcscpy(alternate_command, StringTable::LoadString("TXT_REPLY_COMMAND_SHORT", NULL, "D:\\Ra2\\wonline.cpp", 0x2C12));
								if (!copy) {
									return FALSE;
								}
								copy = wcsncpy(copy, text.get(), length);
								if (!copy) {
									return FALSE;
								}
								wchar_t * body = copy + (wcsncmp(copy, primary_command, wcslen(primary_command)) == 0
														  ? wcslen(primary_command)
														  : wcslen(alternate_command));
								while (*body && wcsncmp(body, L" ", 1) == 0) {
									++body;
								}
								wcscpy(nickname, Narrow_To_Wide(LastPagerName));
								if (wcslen(nickname) > 0 && wcslen(body) > 0) {
									Lobby_Chat_Printf(ColorPriv, L"[%hs > %s] %s", g_NickName.c_str(), nickname, body);
								} else if (wcslen(nickname) > 0) {
									Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_CANT_SEND_BLANK_PAGE", NULL, "D:\\Ra2\\wonline.cpp", 0x2C2F));
								}
								operator delete(copy);
							} else if (!Parse_Online_Command(text.get())) {
								if (strlen((char *)g_CurrentChannel.name) == 0) {
									Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_NOT_IN_CHAN", NULL, "D:\\Ra2\\wonline.cpp", 0x2C38));
								} else {
									int color = ColorMe;
									if (Send_Chat_Message(text) == 1) {
										color = ColorPriv;
									}
									wchar_t * message_text = text.get();
									Lobby_Chat_Printf(color, L"[%hs] %s", g_NickName.c_str(), message_text);
								}
							}
						}
					}
					return FALSE;
				}

				case 0x55D:
					EnableWindow((HWND)lparam, FALSE);
					SetTimer(window, 0x55D, 3000, NULL);
					if (!Queue_Channel_List_Request(0x21, 1, false)) {
						SetEvent(g_WaitEventHandles[5]);
					}
					break;

				case 0x558: // RA2: the tournament page button, which Yuri's Revenge dropped
					ViewHTML("http://www.westwood.com/westwoodonline/tournaments/redalert2/", 0);
					SetFocus(GetDlgItem(window, 0x540));
					break;

				case 0x547:
				case 0x5C0:
				case 0x686:
					OwnerDraw::Set_Dialog_Waiting(window);
					SetEvent(g_WaitEventHandles[6]);
					return FALSE;

				case 0x61F: {
					OwnerDraw::Slide_Dialog(window, false);
					Darken_Surface(NULL, true);
					HWND dialog = OwnerDraw::Begin_Dialog(0x2BC, (DLGPROC)WOL_Options_Dialog_Proc, 0);
					ShowWindow(dialog, SW_SHOWNORMAL);
					int result = WS_Wait_Dialog(dialog, Wait_Callback_Thunk, 0, 1, 1);
					InvalidateRect(window, NULL, FALSE);
					ShowWindow(window, SW_SHOW);
					OwnerDraw::Set_Paint_Disabled(window, 0);
					if (result == 2) {
						Apply_WOL_Settings();
					} else {
						g_AllowPage = g_NewAllowPage;
						g_AllowFind = g_NewAllowFind;
						g_LangFilter = g_NewLangFilter;
						g_LobbyMusic = g_NewLobbyMusic;
						g_ShowAllChannels = g_NewShowAllChannels;
						g_DisplayAsian = g_NewDisplayAsian;
						g_DisplayLatin = g_NewDisplayLatin;
						Write_WOL_Settings();
						Apply_WOL_Settings();
					}
					SetFocus(GetDlgItem(window, 0x540));
					return FALSE;
				}

				case 0x62D: {
					HWND list = GetDlgItem(window, 0x6EE);
					int index = SendMessage(list, LB_GETCURSEL, 0, 0);
					bool joined = false;
					int extra = -1;
					if (index != -1) {
						extra = SendMessage(list, LB_GETITEMDATA, index, 0);
						if (extra >= 0 && extra < g_GameChannelList.length()) {
							Channel chan;
							memset(&chan, 0, sizeof(chan));
							g_GameChannelList.get(chan, extra);
							joined = Can_Join_Channel(&chan, extra != g_LastJoinedGameIndex, ObserverMode != 0);
							if (joined) {
								EnableWindow(GetDlgItem(window, 0x62B), FALSE);
								EnableWindow(GetDlgItem(window, 0x62D), FALSE);
								SendMessage(GetDlgItem(window, 0x6EE), LB_SETCURSEL, -1, 0);
								EnableWindow(GetDlgItem(window, 0x689), FALSE);
								EnableWindow(GetDlgItem(window, 0x558), FALSE);
								EnableWindow(GetDlgItem(window, 0x61F), FALSE);
								EnableWindow(GetDlgItem(window, 0x686), FALSE);
								bool started = Join_Game_Channel(&chan, ObserverMode == 1);
								EnableWindow(GetDlgItem(window, 0x62B), TRUE);
								EnableWindow(GetDlgItem(window, 0x689), TRUE);
								EnableWindow(GetDlgItem(window, 0x558), TRUE);
								EnableWindow(GetDlgItem(window, 0x61F), TRUE);
								EnableWindow(GetDlgItem(window, 0x686), TRUE);
								if (started) {
									OwnerDraw::Slide_Dialog(window, false);
									Darken_Surface(NULL, true);
									ShowWindow(window, SW_HIDE);
									OwnerDraw::Set_Paint_Disabled(window, 1);
								}
							}
						}
					}
					g_LastJoinedGameIndex = extra;
					EnableWindow((HWND)lparam, joined);
					SetFocus(GetDlgItem(window, 0x540));
					break;
				}

				case 0x62B: {
					EnableWindow((HWND)lparam, FALSE);
					EnableWindow(GetDlgItem(window, 0x62D), FALSE);
					SendMessage(GetDlgItem(window, 0x6EE), LB_SETCURSEL, -1, 0);
					EnableWindow(GetDlgItem(window, 0x689), FALSE);
					EnableWindow(GetDlgItem(window, 0x558), FALSE);
					EnableWindow(GetDlgItem(window, 0x61F), FALSE);
					EnableWindow(GetDlgItem(window, 0x686), FALSE);
					bool created = Host_Game();
					EnableWindow((HWND)lparam, TRUE);
					EnableWindow(GetDlgItem(window, 0x689), TRUE);
					EnableWindow(GetDlgItem(window, 0x558), TRUE);
					EnableWindow(GetDlgItem(window, 0x61F), TRUE);
					EnableWindow(GetDlgItem(window, 0x686), TRUE);
					if (!created) {
						SetFocus(GetDlgItem(window, 0x540));
					}
					break;
				}

				case 0x608: {
					WideWstring text;
					HWND input = GetDlgItem(window, 0x540);
					Get_Window_Wide_Text(input, text);
					SendMessage(input, 0x4B2, 0, (LPARAM)L"");
					SetFocus(input);
					if (g_OwnSquadID) {
						User user;
						memset(&user, 0, sizeof(user));
						strcpy((char *)user.name, "0");
						g_pChat->RequestPrivateUnicodeMessage(&user, text.get());
					} else {
						Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_NOSQUAD", NULL, "D:\\Ra2\\wonline.cpp", 0x2C74));
					}
					return FALSE;
				}

				case 0x689:
					Online_Session_State = 4;
					Online_Session_Return = 3;
					OwnerDraw::Set_Dialog_Waiting(window);
					SetEvent(g_WaitEventHandles[6]);
					return FALSE;

				case 0x6EE: {
					bool dblclk = false;
					switch (HIWORD(wparam)) {
						case LBN_SELCHANGE:
							break;

						case LBN_DBLCLK:
							dblclk = true;
							break;

						default:
							return FALSE;
					}
					int index = SendMessage((HWND)lparam, LB_GETCURSEL, 0, 0);
					int extra = -1;
					bool joinable = false;
					if (index != -1) {
						extra = SendMessage((HWND)lparam, LB_GETITEMDATA, index, 0);
						if (extra >= 0 && extra < g_GameChannelList.length()) {
							Channel chan;
							memset(&chan, 0, sizeof(chan));
							g_GameChannelList.get(chan, extra);
							if (g_LastSelectedGameIndex == -1) {
								g_LastSelectedGameName[0] = 0;
							}
							int changed = strcmp(g_LastSelectedGameName, (char *)chan.name);
							joinable = Can_Join_Channel(&chan, dblclk || changed != 0, ObserverMode != 0);
							strcpy(g_LastSelectedGameName, (char *)chan.name);
							if (joinable && dblclk) {
								EnableWindow(GetDlgItem(window, 0x62B), FALSE);
								EnableWindow(GetDlgItem(window, 0x62D), FALSE);
								SendMessage(GetDlgItem(window, 0x6EE), LB_SETCURSEL, -1, 0);
								EnableWindow(GetDlgItem(window, 0x689), FALSE);
								EnableWindow(GetDlgItem(window, 0x558), FALSE);
								EnableWindow(GetDlgItem(window, 0x61F), FALSE);
								EnableWindow(GetDlgItem(window, 0x686), FALSE);
								if (Join_Game_Channel(&chan, ObserverMode == 1)) {
									OwnerDraw::Slide_Dialog(window, false);
									Darken_Surface(NULL, true);
									ShowWindow(window, SW_HIDE);
									OwnerDraw::Set_Paint_Disabled(window, 1);
								}
								EnableWindow(GetDlgItem(window, 0x62B), TRUE);
								EnableWindow(GetDlgItem(window, 0x689), TRUE);
								EnableWindow(GetDlgItem(window, 0x558), TRUE);
								EnableWindow(GetDlgItem(window, 0x61F), TRUE);
								EnableWindow(GetDlgItem(window, 0x686), TRUE);
							}
						} else {
							g_LastSelectedGameName[0] = 0;
						}
					}
					int selected = SendMessage((HWND)lparam, LB_GETCURSEL, 0, 0);
					if (!g_GameListRefreshing || selected != -1) {
						g_LastSelectedGameIndex = extra;
					}
					if (g_GameListRefreshing && selected == -1) {
						g_GameListRefreshing = 0;
					}
					HWND join_button = GetDlgItem(window, 0x62D);
					EnableWindow(join_button, joinable);
					return FALSE;
				}

				case 0x75F:
				case 0x760: {
					int checked = 0;
					HWND item = GetDlgItem(window, 0x760);
					if (item) {
						checked = SendMessage(item, BM_GETCHECK, 0, 0);
					}
					if (LOWORD(wparam) == 0x75F) {
						checked = !checked;
					}
					ObserverMode = checked;
					Update_Observer_Buttons(NULL);
					return FALSE;
				}

			}
			return FALSE;

		case WM_SETFOCUS:
			SetFocus(GetDlgItem(window, 0x540));
			return FALSE;

		case 0x497:
			Update_Observer_Buttons(window);
			SendDlgItemMessage(window, 0x3ED, 0x49C, 0, (LPARAM)SurfaceCache.GetSurface("wouact.pcx", NULL));
			SendDlgItemMessage(window, 0x3ED, 0x4AA, 0, (LPARAM)SurfaceCache.GetSurface("wodact.pcx", NULL));
			SendDlgItemMessage(window, 0x55D, 0x49C, 0, (LPARAM)SurfaceCache.GetSurface("wouref.pcx", NULL));
			SendDlgItemMessage(window, 0x55D, 0x4AA, 0, (LPARAM)SurfaceCache.GetSurface("wodref.pcx", NULL));
			g_Col_GameIcon = 2;
			g_Col_GamePrivate = 19;
			g_Col_GameObserver = 36;
			g_Col_GameName = 53;
			g_Col_GamePingTime = 200;
			SendDlgItemMessage(window, 0x6EE, 0x4A6, g_Col_GamePingTime - g_Col_GameName, g_Col_GameName);
			SendDlgItemMessage(window, 0x6EE, 0x4A6, 0, g_Col_GameIcon);
			SendDlgItemMessage(window, 0x6EE, 0x4A6, 0, g_Col_GamePrivate);
			SendDlgItemMessage(window, 0x6EE, 0x4A6, 0, g_Col_GamePingTime);
			SendDlgItemMessage(window, 0x6EE, 0x4A6, 0, g_Col_GameObserver);
			g_Col_ChanName = 2;
			SendDlgItemMessage(window, 0x53D, 0x4A6, 0, g_Col_ChanName);
			g_Col_Chanop = 2;
			g_Col_Username = 18;
			g_Col_Rank = 115;
			SendDlgItemMessage(window, 0x53E, 0x4A6, g_Col_Rank - g_Col_Username, g_Col_Username);
			SendDlgItemMessage(window, 0x53E, 0x4A6, 0, g_Col_Chanop);
			SendDlgItemMessage(window, 0x53E, 0x4A6, 0, g_Col_Rank);
			Refresh_Lobby_List();
			Refresh_Game_List();
			Draw_Player_List(0, false);
			SetFocus(GetDlgItem(window, 0x540));
			SetTimer(window, 0, 30000, NULL);
			SetTimer(window, 1, 53000, NULL);
			return FALSE;

		case 0x4E9: {
			ToolTipTextRequest * request = (ToolTipTextRequest *)lparam;
			if (request) {
				switch (GetDlgCtrlID(request->Window)) {
					case 0x6EE:
						request->Text = Game_List_Tooltip(request->Window, request->Item);
						break;

					case 0x53E:
						request->Text = User_List_Tooltip(request->Window, request->Item);
						break;
				}
			}
			return FALSE;
		}

		case 0x4ED:
			ShowWindow(window, SW_HIDE);
			OwnerDraw::Set_Paint_Disabled(window, 1);
			return FALSE;

	}
	return FALSE;
}


BOOL CALLBACK WOL_Find_Page_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
	long result=OwnerDraw::Default_Dialog_Proc(window,message,wparam,lparam);
	if(result) return result;
	switch(message) {
	case 0x111:
		switch((int)(wparam&0xffff)) {
		case 2:
			WS_Destroy_Dialog(window,2);
			return 0;
		case 0x5c1: {
			Wstring name;
			Get_Window_Text((HWND)lparam,name);
			WideWstring text;
			Get_Window_Wide_Text(GetDlgItem(window,0x540),text);
			BOOL enabled=(name.length()>0 && text.length()>0)?1:0;
			EnableWindow(GetDlgItem(window,0x5c2),enabled);
			return 0;
		}
		case 0x5c2: {
			HWND edit=GetDlgItem(window,0x540);
			WideWstring text=Limit_Name_Text(edit);
			SendMessageA(edit,0x4b2,0,(LPARAM)L"");
			EnableWindow(GetDlgItem(window,0x5c2),false);
			SetFocus(edit);
			if(!g_pChat) return 0;
			Wstring name;
			Get_Window_Text(GetDlgItem(window,0x5c1),name);
			name.truncate(9u);
			User user;
			strcpy((char *)user.name,name.get());
			if(text.length()>0 && strlen((char *)user.name)>0) {
				bool recipient_is_player=false;
				if(MuteSWLaunches) {
					for(int i=0;i<Session.Players.Count();++i) {
						NodeNameType *player=Session.Players[i];
						if(_stricmp(Wide_To_Narrow(player->Name),(char *)user.name)==0) {
							recipient_is_player=true;
							break;
						}
					}
				}
				if(!recipient_is_player) {
					g_pChat->RequestUnicodePage(&user, text.get());
					SMessagePrintf(ColorMe,L"[%hs] %s",g_NickName,text.get());
				} else {
					SMessagePrintf(-1,StringTable::LoadString("TXT_OBSERVER_CANT_PAGE",0,"D:\\Ra2\\wonline.cpp",0x30ed));
				}
			} else if(strlen((char *)user.name)==0) {
				SMessagePrintf(-1,StringTable::LoadString("TXT_NAME_ERROR",0,"D:\\Ra2\\wonline.cpp",0x30f9));
			} else {
				SMessagePrintf(-1,StringTable::LoadString("TXT_ENTER_MESSAGE",0,"D:\\Ra2\\wonline.cpp",0x30fb));
			}
			return 0;
		}
		case 0x5c3:
			if(g_pChat) {
				char name[64];
				memset(name,0,sizeof(name));
				SendDlgItemMessageA(window,0x5c1,0x4b5,60,(LPARAM)name);
				name[9]=0;
				User user;
				strcpy((char *)user.name,name);
				if(strlen((char *)user.name)>0) g_pChat->RequestFind(&user);
				else SMessagePrintf(-1,StringTable::LoadString("TXT_NAME_ERROR",0,"D:\\Ra2\\wonline.cpp",0x30bb));
			}
			break;
		case 0x607:
			if(g_pChat) {
				if(g_OwnSquadID==0) {
					SMessagePrintf(-1,StringTable::LoadString("TXT_NOSQUAD",0,"D:\\Ra2\\wonline.cpp",0x3107));
					break;
				} else {
					WideWstring text;
					HWND edit=GetDlgItem(window,0x540);
					Get_Window_Wide_Text(edit,text);
					SendMessageA(edit,0x4b2,0,(LPARAM)L"");
					SetFocus(edit);
					User user;
					strcpy((char *)user.name,"0");
					if (text.length() > 0) g_pChat->RequestUnicodePage(&user, text.get());
					else SMessagePrintf(-1,StringTable::LoadString("TXT_ENTER_MESSAGE",0,"D:\\Ra2\\wonline.cpp",0x311a));
				}
			}
			break;
		}
		break;
	case 0xf:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window,0);
		return 0;
	case 0x2b:
		Dialog_On_WM_DRAWITEM(lparam);
		return 1;
	case 0x497: {
		SendDlgItemMessageA(window,0x5c1,0xc5,9,0);
		HWND name_edit=GetDlgItem(window,0x5c1);
		SendMessageA(name_edit,0x4d1,1,0);
		name_edit=GetDlgItem(window,0x5c1);
		SendMessageA(name_edit,0x4b4,0,(LPARAM)"");
		if(g_PageTimer.Value()>0) {
			name_edit=GetDlgItem(window,0x5c1);
			SendMessageA(name_edit,0x4b4,0,(LPARAM)LastPagerName);
			SetFocus(GetDlgItem(window,0x540));
			g_PageTimer=CDTimerClass<SystemTimerClass>(0);
		}
		break;
	}
	case 0x4da:
		PostMessageA(GetDlgItem(window,0x5c2),0xf5,0,0);
		break;
	case 0x4db:
		Focus_Next_Tab_Item(lparam,wparam!=0);
		return 0;
	case 0x540: {
		WideWstring text=Limit_Name_Text((HWND)lparam);
		Wstring name;
		Get_Window_Text(GetDlgItem(window,0x5c1),name);
		BOOL enabled=(text.length()>0 && name.length()>0)?1:0;
		EnableWindow(GetDlgItem(window,0x5c2),enabled);
		return 0;
	}
	}
	return 0;
}


BOOL CALLBACK WOL_Download_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
	if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
	int handled=OwnerDraw::Default_Dialog_Proc(window,message,wparam,lparam);
	if(handled) return handled;
	switch(message) {
	case 0x111:
		switch(wparam & 0xffff) {
		case 2:
			g_pDownload->Abort();
			WS_Destroy_Dialog(window,2);
			break;
		default: return 0;
		}
		break;
	case 0x110: break;
	case 0x5f4:
		WS_Destroy_Dialog(window,1);
		break;
	case 0x5f5:
		ODMessageBox(StringTable::LoadString("TXT_DOWNLOAD_FAILED",0,"D:\\Ra2\\wonline.cpp",0x32f3),0,Wait_Callback_Thunk,false);
		g_pDownload->Abort();
		WS_Destroy_Dialog(window,2);
		break;
	case 0x2b:
		Dialog_On_WM_DRAWITEM(lparam);
		return 1;
	case 0x14: return 1;
	case 0xf:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window,0);
		return 0;
	default: return 0;
	}
	return 1;
}


LRESULT CALLBACK Create_Game_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
	if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
	long result=OwnerDraw::Default_Dialog_Proc(window,message,wparam,lparam);
	if(result) return result;
	CreateGameDialogControl *control=(CreateGameDialogControl *)GetWindowLongA(window,8);
	switch(message) {
	case 0xf:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window,0);
		return 0;
	case 0x14: return 1;
	case 0x2b: Dialog_On_WM_DRAWITEM(lparam); return 1;
	case 0x111:
		switch((int)(wparam&0xffff)) {
		case 1: {
			g_MaxPlayers=5;
			Wstring password;
			Get_Window_Text(GetDlgItem(window,0x72c),password);
			control->Text=password;
			control->Set_Window((unsigned)window,(HWND)1);
			return 0;
		}
		case 2:
			control->Set_Window((unsigned)window,(HWND)2);
			return 0;
		case 0x629:
			if(SendDlgItemMessageA(window,0x629,0xf0,0,0)==1) {
				SendDlgItemMessageA(window,0x66c,0xf1,0,0);
				control->IsPrivate=false;
				control->IsTournament=true;
			} else control->IsTournament=false;
			return 0;
		case 0x66c:
			if(SendDlgItemMessageA(window,0x66c,0xf0,0,0)==1) {
				SendDlgItemMessageA(window,0x629,0xf1,0,0);
				control->IsPrivate=true;
				control->IsTournament=false;
			} else control->IsPrivate=false;
			return 0;
		case 0x6df:
			if(IsDlgButtonChecked(window,0x6e0)) CheckDlgButton(window,0x6e0,0);
			else CheckDlgButton(window,0x6df,1);
			return 0;
		case 0x72c:
			if(lparam) {
				Wstring password;
				Get_Window_Text((HWND)lparam,password);
				EnableWindow(GetDlgItem(window,1),!password.containsAny(" ,;"));
			}
			return 0;
		case 0x6e0:
			if(IsDlgButtonChecked(window,0x6df)) CheckDlgButton(window,0x6df,0);
			else CheckDlgButton(window,0x6e0,1);
			return 0;
		case 0x71e:
			control->LimitResolution=SendMessageA((HWND)lparam,0xf0,0,0)==1;
			return 0;
		case 0x71f:
			control->Observer=SendMessageA((HWND)lparam,0xf0,0,0)==1;
			return 0;
		}
		break;
	case OD_SUBCLASSED: {
		HWND password=GetDlgItem(window,0x72c);
		SendMessageA(password,0x4d1,1,0);
		password=GetDlgItem(window,0x72c);
		SendMessageA(password,0x4b4,0,(LPARAM)"");
		SendDlgItemMessageA(window,0x72c,0xc5,8,0);
		SetFocus(GetDlgItem(window,0x72c));
		CheckDlgButton(window,0x629,control->IsTournament!=false);
		CheckDlgButton(window,0x66c,control->IsPrivate!=false);
		if((Session.Type!=GAME_INTERNET || Online_Session_State!=2) && (int)g_OwnSquadID<1)
			EnableWindow(GetDlgItem(window,0x66c),0);
		HWND resolution=GetDlgItem(window,0x71e);
		bool high_resolution=VideoModeWidth>=1024;
		EnableWindow(resolution,!high_resolution);
		SendMessageA(resolution,0xf1,control->LimitResolution && !high_resolution ? 1:0,0);
		HWND unknown_option=GetDlgItem(window,0x71f);
		SendMessageA(unknown_option,0xf1,control->Observer!=false,0);
		break;
	}
	case 0x4da:
		PostMessageA(GetDlgItem(window,1),0xf5,0,0);
		return 0;
	}
	return 0;
}


LRESULT CALLBACK Wonline_Password_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
	if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
	int handled=OwnerDraw::Default_Dialog_Proc(window,message,wparam,lparam);
	if(handled) return handled;
	WonlineStringDialogControl *control=(WonlineStringDialogControl *)GetWindowLongA(window,8);
	switch(message) {
	case 0xf:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window,0);
		return 0;
	case 0x2b:
		Dialog_On_WM_DRAWITEM(lparam);
		return 1;
	case 0x111:
		switch(wparam&0xffff) {
		case 1: {
			Wstring password;
			Get_Window_Text(GetDlgItem(window,0x72c),password);
			control->Text=password;
			control->Set_Window((unsigned)window,(HWND)1);
			return 0;
		}
		case 2:
			control->Set_Window((unsigned)window,(HWND)2);
			return 0;
		case 0x72c:
			if(lparam) {
				Wstring password;
				Get_Window_Text((HWND)lparam,password);
				EnableWindow(GetDlgItem(window,1),!password.containsAny(" ,;"));
			}
			return 0;
		}
		break;
	case OD_SUBCLASSED: {
		HWND edit=GetDlgItem(window,0x72c);
		SendMessageA(edit,0x4d1,1,0);
		edit=GetDlgItem(window,0x72c);
		SendMessageA(edit,0x4b4,0,(LPARAM)"");
		SendDlgItemMessageA(window,0x72c,0xc5,8,0);
		SetFocus(GetDlgItem(window,0x72c));
		return 0;
	}
	case 0x4da:
		PostMessageA(GetDlgItem(window,1),0xf5,0,0);
		return 0;
	}
	return 0;
}



LRESULT CALLBACK Birthdate_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
	switch(message) {
	case 0xf:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window,0);
		return 0;
	case 0x2b:
		Dialog_On_WM_DRAWITEM(lparam);
		return 1;
	case 0x111:
		switch(wparam&0xffff) {
		case 1: {
			g_BirthdateEmail.clear();
			HWND edit=GetDlgItem(window,0x626);
			if (edit) Get_Window_Text(edit, g_BirthdateEmail);
			g_AccountBirthMonth.clear();
			edit=GetDlgItem(window,0x62e);
			if(edit) {
				Get_Window_Text(edit,g_AccountBirthMonth);
				g_BirthdateMonth=atoi(g_AccountBirthMonth.get());
			} else g_BirthdateMonth=0;
			g_AccountBirthDay.clear();
			edit=GetDlgItem(window,0x62f);
			if(edit) {
				Get_Window_Text(edit,g_AccountBirthDay);
				g_BirthdateDay=atoi(g_AccountBirthDay.get());
			}
			g_AccountBirthYear.clear();
			edit=GetDlgItem(window,0x630);
			if(edit) {
				Get_Window_Text(edit,g_AccountBirthYear);
				g_BirthdateYear=atoi(g_AccountBirthYear.get());
			}
			g_BirthdateNewsletter=0;
			edit=GetDlgItem(window,0x631);
			if(edit) g_BirthdateNewsletter=SendMessageA(edit,0xf0,0,0);
			WS_Destroy_Dialog(window,1);
			return 0;
		}
		case 2:
			WS_Destroy_Dialog(window,2);
			return 0;
		case 0x576:
			ViewHTML("http://privacypolicy.westwood.com",0);
			return 0;
		default:
			return 0;
		}
	case 0x497: {
		HWND edit=GetDlgItem(window,0x626);
		SendMessageA(edit,0x4d1,1,0);
		SendMessageA(GetDlgItem(window,0x62f),0xc5,2,0);
		SendMessageA(GetDlgItem(window,0x630),0xc5,4,0);
		SetFocus(GetDlgItem(window,0x62f));
		HWND month_list=GetDlgItem(window,0x62e);
		Clear_ComboBox(month_list);
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_JANUARY",0,"D:\\Ra2\\wonline.cpp",0x34bc));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_FEBRUARY",0,"D:\\Ra2\\wonline.cpp",0x34bd));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_MARCH",0,"D:\\Ra2\\wonline.cpp",0x34be));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_APRIL",0,"D:\\Ra2\\wonline.cpp",0x34bf));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_MAY",0,"D:\\Ra2\\wonline.cpp",0x34c0));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_JUNE",0,"D:\\Ra2\\wonline.cpp",0x34c1));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_JULY",0,"D:\\Ra2\\wonline.cpp",0x34c2));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_AUGUST",0,"D:\\Ra2\\wonline.cpp",0x34c3));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_SEPTEMBER",0,"D:\\Ra2\\wonline.cpp",0x34c4));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_OCTOBER",0,"D:\\Ra2\\wonline.cpp",0x34c5));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_NOVEMBER",0,"D:\\Ra2\\wonline.cpp",0x34c6));
		SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::LoadString("TXT_DECEMBER",0,"D:\\Ra2\\wonline.cpp",0x34c7));
		SendDlgItemMessageA(window,0x62e,0x14e,0,0);
		SendMessageA(window,0x4a9,0,1);
		return 0;
	}
	case 0x4db:
		Focus_Next_Tab_Item(lparam,wparam!=0);
		return 0;
	}
	return 0;
}


namespace OwnerDraw { int __fastcall Dialog_ID_Of_Window(HWND control); }
LRESULT CALLBACK Account_Details_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
	switch(message) {
	case 0xf:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window,0);
		return 0;
	case 0x2b: Dialog_On_WM_DRAWITEM(lparam); return 1;
	case 0x111:
		switch(wparam&0xffff) {
		case 1: {
			int kind=OwnerDraw::Dialog_ID_Of_Window(window);
			g_AccountName.clear();
			HWND edit=GetDlgItem(window,0x628);
			if(edit) Get_Window_Text(edit,g_AccountName);
			g_AccountPassword.clear();
			edit=GetDlgItem(window,0x72c);
			if(edit) Get_Window_Text(edit,g_AccountPassword);
			g_AccountPasswordCheck.clear();
			edit=GetDlgItem(window,0x627);
			if(edit) Get_Window_Text(edit,g_AccountPasswordCheck);
			if(kind==0xf3) {
				g_AccountParentEmail.clear();
				edit=GetDlgItem(window,0x5e5);
				if(edit) Get_Window_Text(edit,g_AccountParentEmail);
			} else if(kind==0xf4) {
				g_AccountShareInfo=0;
				edit=GetDlgItem(window,0x66b);
				if(edit) g_AccountShareInfo=SendMessageA(edit,0xf0,0,0);
			}
			WS_Destroy_Dialog(window,1);
			return 0;
		}
		case 2:
			WS_Destroy_Dialog(window,2);
			return 0;
		}
		break;
	case 0x497: {
		HWND edit=GetDlgItem(window,0x628);
		SendMessageA(edit,0x4d1,1,0);
		edit=GetDlgItem(window,0x72c);
		SendMessageA(edit,0x4d1,1,0);
		edit=GetDlgItem(window,0x627);
		SendMessageA(edit,0x4d1,1,0);
		edit=GetDlgItem(window,0x5e5);
		SendMessageA(edit,0x4d1,1,0);
		SendMessageA(GetDlgItem(window,0x628),0xc5,9,0);
		SendMessageA(GetDlgItem(window,0x72c),0xc5,8,0);
		SendMessageA(GetDlgItem(window,0x627),0xc5,8,0);
		SetFocus(GetDlgItem(window,0x628));
		break;
	}
	case 0x4db:
		++g_AccountInputEvents;
		Focus_Next_Tab_Item(lparam,wparam!=0);
		return 0;
	}
	return 0;
}


// int __stdcall WOL_Options_Dialog_Proc(HWND,UINT,WPARAM,LPARAM) (0x00749B50): the WOL options dialog. Twin: TibSun WOL_Options_Dialog_Proc; original 970 B + 6 NOP.
int CALLBACK WOL_Options_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
	long result=OwnerDraw::Default_Dialog_Proc(window,message,wparam,lparam);
	if(result) return result;
	switch(message) {
		case WM_DESTROY:
		GameActive=true;
		return 0;
		case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window,0);
		return 0;
		case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return 1;
		case WM_COMMAND:
		switch((int)(wparam&0xffff)) {
		case 1: {
			g_NewAllowPage=0;
			HWND item=GetDlgItem(window,0x5f5);
					if (item) g_NewAllowPage = SendMessageA(item, BM_GETCHECK, 0, 0);
			g_NewAllowFind=0;
			item=GetDlgItem(window,0x5f7);
					if (item) g_NewAllowFind = SendMessageA(item, BM_GETCHECK, 0, 0);
			g_NewLangFilter=0;
			item=GetDlgItem(window,0x5f6);
					if (item) g_NewLangFilter = SendMessageA(item, BM_GETCHECK, 0, 0);
			g_NewLobbyMusic=0;
			item=GetDlgItem(window,0x5f8);
					if (item) g_NewLobbyMusic = SendMessageA(item, BM_GETCHECK, 0, 0);
			item=GetDlgItem(window,0x5f9);
					if (item) g_NewShowAllChannels = SendMessageA(item, BM_GETCHECK, 0, 0);
			g_NewDisplayAsian=0;
			item=GetDlgItem(window,0x5fa);
					if (item) g_NewDisplayAsian = SendMessageA(item, BM_GETCHECK, 0, 0);
			g_NewDisplayLatin=0;
			item=GetDlgItem(window,0x5fb);
					if (item) g_NewDisplayLatin = SendMessageA(item, BM_GETCHECK, 0, 0);
			operator delete(g_OptionsDialogMemory);
			g_OptionsDialogMemory=0;
			WS_Destroy_Dialog(window,1);
			return 0;
		}
		case 2:
			operator delete(g_OptionsDialogMemory);
			g_OptionsDialogMemory=0;
			WS_Destroy_Dialog(window,2);
			return 0;
		case 0x5f8:
			if((short)(wparam>>16)==1||(short)(wparam>>16)==0) {
						bool checked = SendMessageA((HWND)lparam, BM_GETCHECK, 0, 0) == BST_CHECKED;
				if(checked && !Theme.Still_Playing()) Restart_Lobby_Music();
				else if(checked) { if(Theme.What_Is_Playing()==Theme.From_Name("INTRO")) Restart_Lobby_Music(); }
				else if (!checked) Theme.Queue_Song(THEME_QUIET);
				break;
			}
			break;
		case 0x63a: {
			OwnerDraw::Slide_Dialog(window,false);
			Darken_Surface(0,true);
			GameActive=false;
			SoundControlsClass().Dialog();
					ShowWindow(window, SW_SHOW);
			OwnerDraw::Set_Paint_Disabled(window,0);
			return 0;
		}
		}
		break;
		case OD_SUBCLASSED:
			if (g_AllowPage) SendDlgItemMessageA(window, 0x5f5, BM_SETCHECK, BST_CHECKED, 0);
			if (g_AllowFind) SendDlgItemMessageA(window, 0x5f7, BM_SETCHECK, BST_CHECKED, 0);
			if (g_LangFilter) SendDlgItemMessageA(window, 0x5f6, BM_SETCHECK, BST_CHECKED, 0);
			if (g_LobbyMusic) SendDlgItemMessageA(window, 0x5f8, BM_SETCHECK, BST_CHECKED, 0);
			if (g_ShowAllChannels) SendDlgItemMessageA(window, 0x5f9, BM_SETCHECK, BST_CHECKED, 0);
			if (g_DisplayAsian) SendDlgItemMessageA(window, 0x5fa, BM_SETCHECK, BST_CHECKED, 0);
			if (g_DisplayLatin) SendDlgItemMessageA(window, 0x5fb, BM_SETCHECK, BST_CHECKED, 0);
		break;
	case 0x4ed:
			ShowWindow(window, SW_HIDE);
		OwnerDraw::Set_Paint_Disabled(window,1);
		return 0;
	}
	return 0;
}


void Update_Channel_Player_Limit(void) // RA2: 0x00749F20, not in TibSun: tells the chat service how many players the game channel takes (the slots that are open or hold players, plus the host) by a "mode #<channel> +l <number>" command, sends the channel info and the number of players with the computer players
{
	int limit = Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex);
	if (ObserverMode == 1) { // RA2: the observer takes a slot
		++limit;
	}
	int fixed_slots = 0; // RA2: the slots held by players
	int open_slots = 0;
	int player_slots = 0; // RA2: the slots held by computer players
	int i = 1;
	for (; i < limit; ++i) { // RA2: the slots the scenario has
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
	for (; i < 8; ++i) { // RA2: the slots beyond
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
		g_pChat->RequestRawMessage(command.get()); // RA2: virtual 0x9C
		char info[80];
		memset(info, 0, sizeof(info));
		Encode_Channel_ExInfo(info);
		info[40] = 0;
		g_pChat->SetChannelExInfo(info); // RA2: virtual 0xA8
		g_pChat->RequestSetPlayerCount(1, capacity + player_slots); // RA2: virtual 0xFC
	}
}

/// <summary>
/// RA2: the clan name of a lobby user, widened (ra2-decomp src/wonline.cpp's file-local Wide_Squad_Name).
/// </summary>
static inline wchar_t * Wide_Squad_Name(User const & user)
{
	return Narrow_To_Wide((char const *)user.squadname);
}
wchar_t g_PlayerTooltip[256]; // RA2: the text Player_Info_Tooltip returns
wchar_t * __fastcall Player_Info_Tooltip(wchar_t const * name) // RA2: 0x0074A120, not in TibSun: the tooltip of a player of the game lobby: the rank of the player and of the player's clan, when they have one
{
	Wstring player_name;
	Ladder player_ladder;
	memset(&player_ladder,0,sizeof(player_ladder));
	player_name.set(Wide_To_Narrow(name));
	player_name.toLower();
	bool player_found=g_ActiveLadder && g_ActiveLadder->getValue(player_name,player_ladder);
	Ladder squad_ladder;
	memset(&squad_ladder,0,sizeof(squad_ladder));
	Wstring squad_abbreviation;
	User user;
	for(int index=0;index<g_UserList.length();++index) {
		g_UserList.get(user,index);
		if(strcmp((char *)user.name,Wide_To_Narrow(name))==0) break;
	}
	bool has_squad=false;
	bool squad_found=false;
	if(strlen((char *)user.squadname)!=0) {
		has_squad=true;
		squad_abbreviation.set((char *)user.squadabbrev);
		squad_abbreviation.toLower();
		if (g_BattleClansLadderDict.getValue(squad_abbreviation, squad_ladder)) squad_found = true;
	}
	if(player_found) {
		if(has_squad) {
			if(squad_found) {
				wchar_t * squad_name = Wide_Squad_Name(user);
				swprintf(g_PlayerTooltip,StringTable::LoadString("TXT_PLAYER_TOOLTIP_R_C_CR",0,"D:\\Ra2\\wonline.cpp",0x36ce),StringTable::LoadString("TXT_RANK",0,"D:\\Ra2\\wonline.cpp",0x36ce),player_ladder.rung,squad_name,squad_ladder.rung);
			} else {
				wchar_t * squad_name = Wide_Squad_Name(user);
				swprintf(g_PlayerTooltip,StringTable::LoadString("TXT_PLAYER_TOOLTIP_R_C_CUNR",0,"D:\\Ra2\\wonline.cpp",0x36d2),StringTable::LoadString("TXT_RANK",0,"D:\\Ra2\\wonline.cpp",0x36d2),player_ladder.rung,squad_name);
			}
		} else {
			swprintf(g_PlayerTooltip,StringTable::LoadString("TXT_PLAYER_TOOLTIP_R_NOC",0,"D:\\Ra2\\wonline.cpp",0x36d6),StringTable::LoadString("TXT_RANK",0,"D:\\Ra2\\wonline.cpp",0x36d6),player_ladder.rung);
		}
	} else {
		if(has_squad) {
			if(squad_found) {
				wchar_t * squad_name = Wide_Squad_Name(user);
				swprintf(g_PlayerTooltip,StringTable::LoadString("TXT_PLAYER_TOOLTIP_UNR_C_CR",0,"D:\\Ra2\\wonline.cpp",0x36dc),StringTable::LoadString("TXT_UNRANKED",0,"D:\\Ra2\\wonline.cpp",0x36dc),squad_name,squad_ladder.rung);
			} else {
				wchar_t * squad_name = Wide_Squad_Name(user);
				swprintf(g_PlayerTooltip,StringTable::LoadString("TXT_PLAYER_TOOLTIP_UNR_C_CUNR",0,"D:\\Ra2\\wonline.cpp",0x36e0),StringTable::LoadString("TXT_UNRANKED",0,"D:\\Ra2\\wonline.cpp",0x36e0),squad_name);
			}
		} else {
			swprintf(g_PlayerTooltip,StringTable::LoadString("TXT_PLAYER_TOOLTIP_UNR_NOC",0,"D:\\Ra2\\wonline.cpp",0x36e4),StringTable::LoadString("TXT_UNRANKED",0,"D:\\Ra2\\wonline.cpp",0x36e4));
		}
	}
	return g_PlayerTooltip;
}


wchar_t * __fastcall Player_Ping_Tooltip(wchar_t const * name) // RA2: 0x0074A4E0, not in TibSun: the tooltip of the ping icon of the player of this slot name: "unknown" when no ping is known, else good (below 300), fair (below 500) or poor with the number
{
	int ping = g_PlayerPings[Find_Slot_By_Name(name)];
	if (ping < 0) { // RA2: no ping known
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

int __fastcall Slot_From_Control_Id(int id) // RA2: 0x0074A5D0, not in TibSun: the player slot (0 to 7) of the control ids 0x754 to 0x75B of the game lobby, -1 for any other; no caller (expanded where it is used)
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

void __fastcall Enforce_Tournament_Options(HWND dialog) // RA2: 0x0074A640, not in TibSun: in a tournament channel the game options dialog fixes the settings: no computer players or alliances, bases and a short game on, and the controls of them disabled
{
	if (Is_Channel_Tournament(&g_CurrentChannel)) { // RA2: expanded here
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

static void Apply_Channel_Restrictions(HWND window)
{
    if(Is_Current_Channel_Tournament()) {

                EnableWindow(GetDlgItem(window,0x552),0); EnableWindow(GetDlgItem(window,0x553),0); EnableWindow(GetDlgItem(window,0x597),0);
                Session.Options.AIPlayers=0; Session.Options.AlliesAllowed=false;
                HWND control=GetDlgItem(window,0x54e); SendMessageA(control,0xf1,1,0); EnableWindow(control,0);
                control=GetDlgItem(window,0x54b); SendMessageA(control,0xf1,1,0); EnableWindow(control,0);
                Session.Options.ShortGame=1; Session.Options.Bases=true;
            
    }
}
namespace OwnerDraw { // RA2: defined in ownrdraw.cpp
WideWstring __fastcall Get_Control_Tip_Text(HWND window, int a); // RA2: 0x005E2160
WideWstring __fastcall Control_Tooltip(unsigned int key, HWND window); // RA2: 0x005E2260
}
bool __fastcall Is_Host(void); // RA2: 0x005C86A0, defined in netshare.cpp
bool __fastcall Is_Seated_Entry(NodeNameType * entry); // RA2: 0x005C93A0, defined in netshare.cpp
int __fastcall Combo_Selected_Data(HWND dialog, int id, int index); // RA2: 0x005C99D0, defined in netshare.cpp
void __fastcall Set_Slot_Color(HWND dialog, int id); // RA2: 0x005C9AB0, defined in netshare.cpp
void __fastcall Set_Slot_Country(HWND dialog, int id); // RA2: 0x005C9A10, defined in netshare.cpp
void __fastcall Save_Lobby_Options(HWND dialog, SessionOptionsClass * options); // RA2: 0x005CC890, defined in netshare.cpp
void __fastcall Load_Lobby_Options(HWND dialog, SessionOptionsClass * options); // RA2: 0x005CCA30, defined in netshare.cpp
void __fastcall Dispatch_Game_Mode(void); // RA2: 0x005C3B70, defined in netshare.cpp
void __fastcall Load_Random_Map_Preview(HWND dialog); // RA2: 0x005CC800, defined in netshare.cpp
void __fastcall Clear_Observer_Slots(HWND dialog); // RA2: 0x005CB560, defined in netshare.cpp
bool __fastcall Reset_AI_Slots(HWND dialog); // RA2: 0x005CACB0, defined in netshare.cpp
void __fastcall Change_Scenario_Slots(HWND dialog, int old_scenario, int scenario); // RA2: 0x005CB8D0, defined in netshare.cpp
void __fastcall Set_Skirmish_Dialog_Text(HWND dialog); // RA2: 0x005C34B0, defined in netshare.cpp
void __fastcall Set_Item_Text(HWND dialog, MultiplayerGameMode * mode); // RA2: 0x005C3450, defined in netshare.cpp
void __fastcall Update_Slot_Controls(HWND dialog, int slot, bool flag); // RA2: 0x005C9B90, defined in netshare.cpp
void __fastcall Refresh_Slot_Images(HWND dialog); // RA2: 0x005C9EC0, defined in netshare.cpp
void __fastcall Enable_Start_Button(void); // RA2: 0x005C9700, defined in netshare.cpp
int __fastcall Control_Slot(int id); // RA2: 0x005C9800, defined in netshare.cpp
wchar_t const * __fastcall Entry_Type_Name(NodeNameType * entry); // RA2: 0x005C9870, defined in netshare.cpp
int __fastcall Side_Combo_Player(int id); // RA2: 0x004D13B0, defined in gdlgsupp.cpp
int __fastcall Color_Combo_Player(int id); // RA2: 0x004D1D10, defined in gdlgsupp.cpp
int __fastcall Side_Combo_Data(HWND dialog, int id, int index); // RA2: 0x004D1C70, defined in gdlgsupp.cpp
int __fastcall Color_Combo_Data(HWND dialog, int id, int index); // RA2: 0x004D2900, defined in gdlgsupp.cpp
wchar_t const * __fastcall Color_Name(int color); // RA2: 0x004D1D80, defined in gdlgsupp.cpp
wchar_t const * __fastcall Side_Name(int side); // RA2: 0x004D1420, defined in gdlgsupp.cpp
void __fastcall Fill_Side_Combos(HWND dialog); // RA2: 0x004D16D0, defined in gdlgsupp.cpp
void __fastcall Init_Color_Entries(void); // RA2: 0x004D1EA0, defined in gdlgsupp.cpp
void __fastcall Fill_Color_Combos(HWND dialog); // RA2: 0x004D2300, defined in gdlgsupp.cpp
void __fastcall Select_Player_Side(HWND dialog, int player); // RA2: 0x004D1820, defined in gdlgsupp.cpp
void __fastcall Select_Player_Color(HWND dialog, int player); // RA2: 0x004D23C0, defined in gdlgsupp.cpp
void __fastcall Send_User_Pings(void); // RA2: 0x0075B1B0, below: sends the host's pings of the channel's users to them

#ifdef RA2_NON_MATCHING // not exact: 386 bytes differ (main's text; in the start button's checks the slot sum keeps the AI count, so the later temporaries take other registers)
BOOL CALLBACK Wonline_Game_Options_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    int i;
    int handled=OwnerDraw::Default_Dialog_Proc(window,message,wparam,lparam);
    if(handled) return handled;
    if(!WolOptionsLocked) {
        switch(message) {
        case 0x497:
            Apply_Channel_Restrictions(window);
            Fill_Options_Dialog(window,1); break;
        case 0x114:
            if((short)wparam==8) {
                if(GetDlgItem(window,0x54f)) Session.Options.UnitCount=SendDlgItemMessageA(window,0x54f,0x400,0,0);
                if(GetDlgItem(window,0x402)) Session.Options.Credits=SendDlgItemMessageA(window,0x402,0x400,0,0);
                if(GetDlgItem(window,0x529)) Session.Options.GameSpeed=6-SendDlgItemMessageA(window,0x529,0x400,0,0);
            for (int i = 1; i < 8; i++) g_UserInfo[i].accepted = 0;
            Enable_Start_Button(); Draw_Player_List(0,false);
            }
            break;
        case 0x111: {
            bool changed=false;
            switch((int)(wparam&0xffff)) {
            case 0x3e8: Session.Options.MCVRedeploy=false; if(SendDlgItemMessageA(window,0x3e8,0xf0,0,0)==1) Session.Options.MCVRedeploy=true; changed=true; break;
            case 0x54d: Session.Options.Goodies=false; if(SendDlgItemMessageA(window,0x54d,0xf0,0,0)==1) Session.Options.Goodies=true; changed=true; break;
            case 0x551: Session.Options.CrapEngineers=false; if(SendDlgItemMessageA(window,0x551,0xf0,0,0)==1) Session.Options.CrapEngineers=true; changed=true; break;
            case 0x596: Session.Options.HarvTruce=false; if(SendDlgItemMessageA(window,0x596,0xf0,0,0)==1) Session.Options.HarvTruce=true; changed=true; break;
            case 0x597: Session.Options.AlliesAllowed=false; if(SendDlgItemMessageA(window,0x597,0xf0,0,0)==1) Session.Options.AlliesAllowed=true; changed=true; break;
            case 0x599: Session.Options.FogOfWar=false; if(SendDlgItemMessageA(window,0x599,0xf0,0,0)==1) Session.Options.FogOfWar=true; changed=true; break;
            case 0x624: Session.Options.BridgeDestruction=false; if(SendDlgItemMessageA(window,0x624,0xf0,0,0)==1) Session.Options.BridgeDestruction=true; changed=true; break;
            case 0x69a: AllowDisableableSupers=false; if(SendDlgItemMessageA(window,0x69a,0xf0,0,0)==1) AllowDisableableSupers=true;  break;
            case 0x54b:
                Session.Options.Bases=false;
                if(SendDlgItemMessageA(window,0x54b,0xf0,0,0)==1) Session.Options.Bases=true;
                else if(!Session.Options.Bases) { Session.Options.ShortGame=0; SendDlgItemMessageA(window,0x54e,0xf1,0,0); }
                changed=true; break;
            case 0x54e:
                Session.Options.ShortGame=0;
                if(SendDlgItemMessageA(window,0x54e,0xf0,0,0)==1) { Session.Options.ShortGame=1; if(!Session.Options.Bases) { Session.Options.Bases=true; SendDlgItemMessageA(window,0x54b,0xf1,1,0); } }
                changed=true; break;
            case 0x69d:
                Session.Options.BuildOffAlly=false;
                if(SendDlgItemMessageA(window,0x69d,0xf0,0,0)==1) Session.Options.BuildOffAlly=true;
                break;
            }
            if(changed) {
            for (int i = 1; i < 8; i++) g_UserInfo[i].accepted = 0;
            Enable_Start_Button(); Draw_Player_List(0,false);
            }
            break;
        }
        }
    }
    switch(message) {
    case 0x84: {
        HWND tooltip=GetDlgItem(window,0x695);
        if(!tooltip) goto host_unhandled;
        POINT point;
        point.x=lparam&0xffff;
        point.y=(unsigned)lparam>>16;
        RECT bounds;
        GetWindowRect(window,&bounds);
        point.x-=bounds.left;
        point.y-=bounds.top;
        WideWstring text;
        HWND child=ChildWindowFromPointEx(window,point,1);
        if(child) {
            text=OwnerDraw::Get_Control_Tip_Text(window,(int)child);
            if(wcscmp(text.get(),L"")==0)
                text=OwnerDraw::Control_Tooltip((unsigned)window,child);
        }
        SendMessageA(tooltip,0x4b2,0,(LPARAM)text.text());
        return 0;
    }
    case 0x2b:
        Dialog_On_WM_DRAWITEM(lparam);
        return 1;
    case 2:
        if(MultiplayerMapPreview) {
            delete MultiplayerMapPreview;
            MultiplayerMapPreview=0;
        }
        if(!Progress.Has_Dialog()) goto host_unhandled;
        Progress.Clear_Dialog();
        return 0;
    case 0xf:
        Dialog_On_WM_PAINT(window);
        Slide_Pending_Dialog(window);
        if(MultiplayerMapPreview) MultiplayerMapPreview->Blit_Preview(window);
        else if(Session.Options.ScenarioIndex==-1 && RandomMapGen.MapPreview) RandomMapGen.MapPreview->Blit_Preview(window);
        if(Progress.Has_Dialog()) Progress.Display_Progress(Point2D(-1,-1));
        ValidateRect(window,0);
        return 0;
    case 0x111: {
        int command=wparam&0xffff;
        switch(command) {
        case 2:
            if(WolOptionsLocked) goto host_unhandled;
            Save_Lobby_Options(window,&Session.WOL);
            Session.Write_MultiPlayer_Settings();
            if(MultiplayerMapPreview) { delete MultiplayerMapPreview; MultiplayerMapPreview=0; }
            WS_Destroy_Dialog(window,2); ShowWindow(window,0); OwnerDraw::Set_Paint_Disabled(window,1);
            g_pChat->RequestChannelLeave(); g_JoinLobbyNow=1;
            return 0;
        case 0x3ed: {
            HWND input=GetDlgItem(window,0x540);
            WideWstring text=Limit_Name_Text(input);
            SendMessageA(input,0x4b2,0,(LPARAM)L"");
            SetFocus(input);
            InvalidateRect(input,0,0);
            int color=ColorAction;
            if(strlen((char *)g_CurrentChannel.name)==0)
                Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NOT_IN_CHAN",0,"D:\\Ra2\\wonline.cpp",0x3ae5));
            else if(text.length()>0) {
                if(Send_Chat_Action(text)) color=ColorPrivAction;
                Lobby_Chat_Printf(color,L"%hs %s",g_NickName,text.get());
            } else Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_ENTER_MESSAGE",0,"D:\\Ra2\\wonline.cpp",0x3aec));
            return 0;
        }
        case 0x540: {
            wchar_t nickname[16],primary_command[64],alternate_command[64];
            WideWstring text=Limit_Name_Text((HWND)lparam);
            if((unsigned short)(wparam>>16)==0x501) {
                SendMessageA((HWND)lparam,0x4b2,0,(LPARAM)L"");
                if(text.length()>0) {
                    int parsed=Parse_Page_Command(text.get());
                    if(parsed!=0) {
                        if(parsed==1) {
                            int count=0;
                            unsigned length=text.length()+1;
                            wchar_t *copy=(wchar_t *)operator new(length*2);
                            wcscpy(primary_command,StringTable::LoadString("TXT_PAGE_COMMAND",0,"D:\\Ra2\\wonline.cpp",0x3a71));
                            if(!copy) return 0;
                            copy=wcsncpy(copy,text.get(),length);
                            if(!copy) return 0;
                            wchar_t *body=copy+wcslen(primary_command);
                            while(*body && wcsncmp(body,L" ",1)==0) ++body;
                            while(*body) {
                                if(wcsncmp(body,L" ",1)==0 || count>10) break;
                                nickname[count++]=*body++;
                            }
                            nickname[count]=0;
                            while(*body && wcsncmp(body,L" ",1)==0) ++body;
                            if(count>10) Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NAME_TOO_LONG",0,"D:\\Ra2\\wonline.cpp",0x3a8f));
                            else if(count==0) Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NO_NAME_SPECIFIED",0,"D:\\Ra2\\wonline.cpp",0x3a91));
                            else if(wcslen(body)>0) Lobby_Chat_Printf(ColorPriv,L"[%hs > %s] %s",g_NickName,nickname,body);
                            else Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_CANT_SEND_BLANK_PAGE",0,"D:\\Ra2\\wonline.cpp",0x3a97));
                            operator delete(copy);
                        }
                    } else if(Reply_To_Last_Pager(text.get())) {
                        unsigned length=text.length()+1;
                        wchar_t *copy=(wchar_t *)operator new(length*2);
                        wcscpy(primary_command,StringTable::LoadString("TXT_REPLY_COMMAND",0,"D:\\Ra2\\wonline.cpp",0x3aa9));
                        wcscpy(alternate_command,StringTable::LoadString("TXT_REPLY_COMMAND_SHORT",0,"D:\\Ra2\\wonline.cpp",0x3aaa));
                        if(!copy) return 0;
                        copy=wcsncpy(copy,text.get(),length);
                        if(!copy) return 0;
                        wchar_t *body=copy+(wcsncmp(copy,primary_command,wcslen(primary_command))==0 ? wcslen(primary_command):wcslen(alternate_command));
                        while(*body && wcsncmp(body,L" ",1)==0) ++body;
                        wcscpy(nickname,Narrow_To_Wide(LastPagerName));
                        if(wcslen(body)>0 && wcslen(nickname)>0)
                            Lobby_Chat_Printf(ColorPriv,L"[%hs > %s] %s",g_NickName,nickname,body);
                        else if(wcslen(nickname)>0)
                            Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_CANT_SEND_BLANK_PAGE",0,"D:\\Ra2\\wonline.cpp",0x3ac7));
                        operator delete(copy);
                    } else if(!Parse_Online_Command(text.get())) {
                        int color=ColorMe;
                        if(Send_Chat_Message(text)==1) color=ColorPriv;
                        Lobby_Chat_Printf(color,L"[%hs] %s",g_NickName,text.get());
                    }
                }
            }
            return 0;
        }
        case 0x411: case 0x412: case 0x413: case 0x414:
        case 0x415: case 0x416: case 0x417: case 0x418:
            if((unsigned short)(wparam>>16)!=1 || WolOptionsLocked) goto host_unhandled;
            if(!Is_Host()) goto host_unhandled;
			{ int entry = Combo_Selected_Data(window, command, -1); Change_Slot_Type(window, command, (NodeNameType *)entry, 2); }
            return 0;
        case 0x522: case 0x523: case 0x524: case 0x525: case 0x526: case 0x527: case 0x528: case 0x6a2:
            if((unsigned short)(wparam>>16)!=1 || WolOptionsLocked) goto host_unhandled;
            Set_Slot_Color(window,command);
            Send_Lobby_Options(true,false); Refresh_Slot_Settings(window,true);
            return 0;
        case 0x510: case 0x513: case 0x514: case 0x51e: case 0x51f: case 0x520: case 0x521: case 0x6a1:
            if((unsigned short)(wparam>>16)!=1 || WolOptionsLocked) goto host_unhandled;
            Set_Slot_Country(window,command);
            Send_Lobby_Options(true,false); Refresh_Slot_Settings(window,true);
            return 0;
        case 0x588: {
            SetFocus(GetDlgItem(window,0x540));
            if(g_GameStartRequested==1) goto host_unhandled;
            Save_Lobby_Options(window,&Session.WOL);
            Session.Write_MultiPlayer_Settings();
            Refresh_Slot_Settings(window,true);
            int observers=0;
            for(i=0;i<Session.Players.Count();++i) if(Session.Players[i]->Slot.SpectatorFlag==-1) ++observers;
            if(g_UserList.length()-observers<2) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_ONLY_ONE",0,"D:\\Ra2\\wonline.cpp",0x397c)); return 0; }
            if(observers && Session.Options.AIPlayers) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NO_AI_OBSERVATION",0,"D:\\Ra2\\wonline.cpp",0x3987)); return 0; }
            bool missing=false;
            for(i=1;i<Session.Players.Count();++i) if(!Session.Players[i]->Slot.Unknown7F) {
                Lobby_Chat_Printf(-1,StringTable::LoadString("GUI:HostNoMap",0,"D:\\Ra2\\wonline.cpp",0x3992),Session.Players[i],Session.Options.ScenarioDescription); missing=true;
            }
            if(missing) return 0;
            for(i=0;i<g_UserList.length();++i) if(!g_UserInfo[i].accepted) {
                Dispatch_Game_Mode(); Lobby_Chat_Printf(-1,StringTable::LoadString("GUI:HostGameStartHost",0,"D:\\Ra2\\wonline.cpp",0x39a5)); return 0;
            }
            if(Session.Type==4 && Online_Session_State==2) {
                if(g_UserList.length()<(int)g_CurrentChannel.maxUsers) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NEED_MORE_PLAYERS",0,"D:\\Ra2\\wonline.cpp",0x39af),g_CurrentChannel.maxUsers); return 0; }
                int team1=0,team2=0;
                for(i=0;i<g_UserList.length();++i) { if(g_UserInfo[i].house==0) ++team2; else ++team1; }
                if(team1!=team2) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NEED_EQUAL_TEAMS",0,"D:\\Ra2\\wonline.cpp",0x39be)); return 0; }
            }
            int limit=Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex);
            unsigned requested=SendDlgItemMessageA(window,0x552,0x400,0,0);
            if(limit<(int)(requested-observers+g_UserList.length())) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_SCENARIO_TOO_SMALL",0,"D:\\Ra2\\wonline.cpp",0x39c9),limit); return 0; }
            if(g_UserList.length()>4) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_TOO_MANY_PLAYERS",0,"D:\\Ra2\\wonline.cpp",0x39d1)); return 0; }
            MultiplayerGameMode *mode=Session.MPGameMode;
            WideWstring error;
            if(!mode->CheckStartConditions(&error)) {
                Lobby_Chat_Printf(-1,error.get()); EnableWindow(GetDlgItem(WS_Top_Window(),0x588),1); return 0;
            }
            if(Is_Channel_Flag_8(&g_CurrentChannel)) {
                User *user=0;
                unsigned squad1=0,squad2=0;
                int count1=0,count2=0;
                char observer[10]="";
                if(LobbyObserverSlot>=0) {
                    NodeNameType *node=Session.Slots[LobbyObserverSlot];
                    if(Is_Seated_Entry(node) && node->Slot.SpectatorFlag==-1) { char *name=Wide_To_Narrow(node->Name); strncpy(observer,name,10); observer[9]=0; }
                }
                for(i=0;i<g_UserList.length();++i) {
                    g_UserList.getPointer(&user,i);
                    if(_stricmp(observer,(char *)user->name)) {
                        unsigned squad=user->squadID;
                        if(squad<1) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_CLAN_NOSTART",0,"D:\\Ra2\\wonline.cpp",0x39fb)); return 0; }
                        if(!squad1 || squad1==squad) { squad1=squad; ++count1; }
                        else { if(squad2 && squad2!=squad) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_CLAN_NOSTART",0,"D:\\Ra2\\wonline.cpp",0x3a0a)); return 0; } squad2=squad; ++count2; }
                    }
                }
                if(count1!=count2) { Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_CLAN_NOSTART",0,"D:\\Ra2\\wonline.cpp",0x3a10)); return 0; }
            }
            mode->vt_entry_58(); Session.Assign_Random_Choices(); Send_Player_List();
            Send_Lobby_Options(true,true); mode->vt_entry_54(); mode->vt_entry_24();
            User *temp, *head = NULL;
            User slot_user;
            memset(&slot_user, 0, sizeof(slot_user));
            for (i = 0; i < g_UserList.length(); ++i) {
                g_UserList.get(slot_user, i);
                temp = new User;
                *temp = slot_user;
                temp->next = head;
                head = temp;
            }
            g_pChat->RequestGameStart(head);
            WolOptionsLocked = 1;
            g_GameStartRequested = true;
            temp = head;
            while (temp) {
                head = temp->next;
                delete temp;
                temp = head;
            }
            return 0;
        }
        case 0x5aa: {
            if(WolOptionsLocked) goto host_unhandled;
            int old=Session.Options.ScenarioIndex;
            bool ai=!Session.MPGameMode || Session.MPGameMode->IsSkirmishAllowed();
            bool observers=!Session.MPGameMode || Session.MPGameMode->vt_entry_A0();
            char filename[132]; wchar_t description[130];
            strcpy(filename,Session.ScenarioFileName); wcscpy(description,Session.Options.ScenarioDescription);
            IsRandomMap=false; OwnerDraw::Slide_Dialog(window,false); Darken_Surface(0,true);
            bool cancelled=Scenario_Dialog((HWND)MainWindow)==2;
            ShowWindow(window,5); OwnerDraw::Set_Paint_Disabled(window,0); SetFocus(GetDlgItem(window,0x540));
            if(cancelled) {
                Session.Options.ScenarioIndex=old; Set_Scenario_Info_From_Index(old); Update_Network_Dialog_Preview(window);
                IsRandomMap=true; ShowWindow(window,5); OwnerDraw::Set_Paint_Disabled(window,0);
                Load_Random_Map_Preview(window); Send_Lobby_Options(true,false); InvalidateRect(window,0,0); return 0;
            }
            if(!Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex)) Session.Options.ScenarioIndex=old;
            else {
                bool new_ai=!Session.MPGameMode || Session.MPGameMode->IsSkirmishAllowed();
                bool new_observers=!Session.MPGameMode || Session.MPGameMode->vt_entry_A0();
                if(observers && !new_observers) Clear_Observer_Slots(window);
                if(ai && !new_ai) {
                    if(Reset_AI_Slots(window)) {
                        Session.Options.AIPlayers=0; Send_Player_List(); Draw_Player_List(0,false); Send_Lobby_Options(true,false);
                    } else Draw_Player_List(0,true);
                }
                Change_Scenario_Slots(window,old,Session.Options.ScenarioIndex);
            }
            Update_Channel_Player_Limit(); IsRandomMap=true;
            ShowWindow(window,5); OwnerDraw::Set_Paint_Disabled(window,0); Set_Skirmish_Dialog_Text(window);
            if(strcmp(filename,Session.ScenarioFileName) || wcscmp(description,Session.Options.ScenarioDescription)) {
                for (int i = 1; i < 8; i++) g_UserInfo[i].accepted = 0;
            }
            Enable_Start_Button(); Load_Random_Map_Preview(window); Refresh_Slot_Settings(window,true); Send_Lobby_Options(true,false); InvalidateRect(window,0,0);
            return 0;
        }
        }
        return 0;
    }
    case 0x497: {
        SendDlgItemMessageA(window,0x3ed,0x49c,0,(LPARAM)SurfaceCache.SurfaceCacheClass::GetSurface("wouact.pcx",0));
        SendDlgItemMessageA(window,0x3ed,0x4aa,0,(LPARAM)SurfaceCache.SurfaceCacheClass::GetSurface("wodact.pcx",0));
        MultiplayerGameMode *mode=First_Game_Mode();
        Session.Options.MPModeIndex=mode->MPModeIndex; Session.MPGameMode=mode;
        mode->vt_entry_20(); Set_Item_Text(window,mode);
        Session.Options.ScenarioIndex=mode->FirstValidMapIndex(); Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex);
        Fill_Side_Combos(window); Init_Color_Entries(); Fill_Color_Combos(window);
        Select_Player_Side(window,Session.LocalSlot); Select_Player_Color(window,Session.LocalSlot);
        Load_Lobby_Options(window,&Session.WOL);
            Apply_Channel_Restrictions(window);
        if(Session.MPGameMode && !Session.MPGameMode->vt_entry_A0()) {
            LobbyObserverSlot=-1; ObserverMode=0; Update_Observer_Buttons(0);
            Session.Players[0]->Slot.SpectatorFlag=0;
            Session.Slots[0]->Set_Country(Session.MPGameMode->DefaultCountry());
            Session.Slots[0]->NodeNameType::Set_Color(-2); Fill_Color_Combos(window);
        }
        Update_Slot_Controls(window,0,true); Change_Scenario_Slots(window,-1,Session.Options.ScenarioIndex);
        Update_Channel_Player_Limit(); Refresh_Slot_Images(window);
        Set_Item_Text(window,Session.MPGameMode); Set_Skirmish_Dialog_Text(window); Load_Random_Map_Preview(window);
        Session.PlayingAgainstVersion=VerNum.Version_Number(); SetTimer(window,0x14af8,5000,0);
        Fill_Options_Dialog(window,1); Refresh_Slot_Settings(window,true); SetFocus(GetDlgItem(window,0x540));
        if(WolOptionsLocked) goto host_unhandled;
        User user;
        memset(&user,0,sizeof(user));
        int count=g_UserList.length();
        int handle=0;
        int i=0;
        if(count>0) {
        do {
            g_UserList.get(user,i);
            in_addr address; address.s_addr=user.ipaddr;
            DebugString("Requesting ping for %s at %s\n",user.name,inet_ntoa(address));
            g_pNetUtil->RequestPing(inet_ntoa(address),1000,&handle);
            ++i;
        } while(i<count);
        return 0;
        }
        goto host_unhandled;
    }
    case 0x114: case 0x115:
        if(!WolOptionsLocked) {
            Session.Options.UnitCount=SendDlgItemMessageA(window,0x54f,0x400,0,0);
            Session.Options.Credits=SendDlgItemMessageA(window,0x402,0x400,0,0);
            Session.Options.GameSpeed=6-SendDlgItemMessageA(window,0x529,0x400,0,0);
            return 0;
        }
        goto host_unhandled;
    case 0x113: {
        if(wparam!=0x14af8 || !Is_Host()) goto host_unhandled;
        Send_User_Pings();
        if(WolOptionsLocked) goto host_unhandled;
        User user;
        memset(&user,0,sizeof(user));
        int count=g_UserList.length();
        int handle=0;
        int i=0;
        if(count<=0) goto host_unhandled;
        do {
            g_UserList.get(user,i);
            in_addr address; address.s_addr=user.ipaddr;
            DebugString("Requesting ping for %s at %s\n",user.name,inet_ntoa(address));
            g_pNetUtil->RequestPing(inet_ntoa(address),1000,&handle);
            ++i;
        } while(i<count);
        return 0;
    }
    case 0x49b:
        { HWND control=GetDlgItem(window,wparam); SendMessageA(control,0x4b3,0x7f,lparam); return 0; }
    case 0x4e9: {
        ToolTipTextRequest *request=(ToolTipTextRequest *)lparam;
        int id;
        {
            WideWstring &text=request->Text;
            text=(wchar_t const *)0;
            if(!request->Window) goto tooltip_done;
            id=GetDlgCtrlID((HWND)request->Window);
            if(request->Item!=-1) {
                if(Control_Slot(id)==-1) {
                    if(Side_Combo_Player(id)==-1) {
                        if(Color_Combo_Player(id)!=-1) {
                            text=Color_Name(Color_Combo_Data(window,id,request->Item));
                            return 1;
                        }
                    } else {
                        int country=Side_Combo_Data(window,id,request->Item);
                        if(country!=-1) { text=Side_Name(country); return 1; }
                    }
                } else {
                    NodeNameType *entry=(NodeNameType *)Combo_Selected_Data(window,id,request->Item);
                    text=Entry_Type_Name(entry);
                    return 1;
                }
                goto tooltip_done;
            }
            if(Control_Slot(id)!=-1) {
                NodeNameType *entry=(NodeNameType *)Combo_Selected_Data(window,id,-1);
                if(entry) {
                    WideWstring label=OwnerDraw::Control_Tooltip((unsigned)window,(HWND)request->Window);
                    if((wcscmp(label.get(),StringTable::LoadString("STT:GuestComboPlayer",0,"D:\\Ra2\\wonline.cpp",0x37dd))==0 ||
                        wcscmp(label.get(),StringTable::LoadString("STT:HostComboPlayer",0,"D:\\Ra2\\wonline.cpp",0x37dd))==0) && Is_Real_Player_Entry(entry))
                        label=Player_Info_Tooltip(entry->Name);
                    text=label.get();
                }
                goto tooltip_done;
            }
        }
        {
            int player;
            if(id==0x754) player=0;
            else if(id==0x755) player=1;
            else if(id==0x756) player=2;
            else if(id==0x757) player=3;
            else if(id==0x758) player=4;
            else if(id==0x759) player=5;
            else if(id==0x75a) player=6;
            else if(id==0x75b) player=7;
            else goto tooltip_done;
            WideWstring ping;
            int slot=Get_Slot_Control_Id(player);
            if(slot!=-1) {
                NodeNameType *entry=(NodeNameType *)Combo_Selected_Data(window,slot,-1);
                if(entry && Is_Real_Player_Entry(entry)) {
                    ping=Player_Ping_Tooltip(entry->Name);
                    request->Text=ping;
                }
            }
        }
tooltip_done:
        return 1;
    }
    case 0x4ed:
        ShowWindow(window,0);
        OwnerDraw::Set_Paint_Disabled(window,1);
host_unhandled:
        return 0;
    case 3: case 0x18: goto host_unhandled;
    case 0x14: return 1;
    default: return 0;
    }
    return 0;
}
#endif



void __fastcall Leave_Channel_With_Message(HWND dialog) // RA2: 0x0074C5F0, not in TibSun: the joiner has no slot in the game any more: tells the player (the channel is full, or the observer slot is) and leaves the channel; the dialog is closed when leaving fails
{
	if (dialog) {
		if (ObserverMode == 0) {
			ODMessageBox(StringTable::LoadString("TXT_CHANNEL_FULL", NULL, "D:\\Ra2\\wonline.cpp", 15375), 0, Wait_Callback_Thunk, false);
		} else {
			ODMessageBox(StringTable::LoadString("TXT_OBSERVER_FULL", NULL, "D:\\Ra2\\wonline.cpp", 15377), 0, Wait_Callback_Thunk, false);
		}
		if (g_pChat->RequestChannelLeave() < 0) { // RA2: virtual 0x24
			WS_Destroy_Dialog(dialog, 2);
		}
	}
}

BOOL CALLBACK WOL_Guest_Dialog_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    int handled=OwnerDraw::Default_Dialog_Proc(window,message,wparam,lparam);
    if(handled) return handled;
    switch(message) {
    case 0x84: {
        HWND tooltip=GetDlgItem(window,0x695);
        if(!tooltip) return 0;
        POINT point;
        point.x=lparam&0xffff;
        point.y=(unsigned)lparam>>16;
        RECT bounds;
        GetWindowRect(window,&bounds);
        point.x-=bounds.left;
        point.y-=bounds.top;
        WideWstring text;
        HWND child=ChildWindowFromPointEx(window,point,1);
        if(child) {
            text=OwnerDraw::Get_Control_Tip_Text(window,(int)child);
            if(wcscmp(text.get(),L"")==0)
                text=OwnerDraw::Control_Tooltip((unsigned)window,child);
        }
        SendMessageA(tooltip,0x4b2,0,(LPARAM)text.text());
        return 0;
    }
    case 2:
        if(Progress.Has_Dialog()) Progress.Clear_Dialog();
        if(MultiplayerMapPreview) {
            delete MultiplayerMapPreview;
            MultiplayerMapPreview=0;
        }
        return 0;
    case 0xf:
        Dialog_On_WM_PAINT(window);
        Slide_Pending_Dialog(window);
        if(MultiplayerMapPreview) MultiplayerMapPreview->Blit_Preview(window);
        if(Progress.Has_Dialog()) Progress.Display_Progress(Point2D(-1,-1));
        ValidateRect(window,0);
        return 0;
    case 0x2b:
        Dialog_On_WM_DRAWITEM(lparam);
        return 1;
    case 0x111: {
        int command=wparam&0xffff;
        switch(command) {
        case 2:
            if(!WolOptionsLocked && g_pChat->RequestChannelLeave()<0) WS_Destroy_Dialog(window,2);
            return 0;
        case 0x3ed: {
            HWND input=GetDlgItem(window,0x540);
            WideWstring text=Limit_Name_Text(input);
            SendMessageA(input,0x4b2,0,(LPARAM)L"");
            SetFocus(input);
            InvalidateRect(input,0,0);
            int color=ColorAction;
            if(strlen((char *)g_CurrentChannel.name)==0)
                Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NOT_IN_CHAN",0,"D:\\Ra2\\wonline.cpp",0x3dae));
            else if(text.length()>0) {
                if(Send_Chat_Action(text)) color=ColorPrivAction;
                Lobby_Chat_Printf(color,L"%hs %s",g_NickName,text.get());
            } else Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_ENTER_MESSAGE",0,"D:\\Ra2\\wonline.cpp",0x3db6));
            return 0;
        }
        case 0x59f: {
            Session.Players[0]->Slot.Unknown77=1;
            char options[64];
            sprintf(options,"A1");
            Send_Host_Option(options);
            Enable_Player_Combos(window,false);
            HWND accept=GetDlgItem(window,0x59f);
            EnableWindow(accept,0);
            InvalidateRect(accept,0,0);
            SendMessageA(accept,0x4dc,0,0);
            Draw_Player_List(0,false);
            SetFocus(GetDlgItem(window,0x540));
            return 0;
        }
        case 0x540: {
            wchar_t nickname[16],primary_command[64],alternate_command[64];
            WideWstring text=Limit_Name_Text((HWND)lparam);
            if((unsigned short)(wparam>>16)==0x501) {
                SendMessageA((HWND)lparam,0x4b2,0,(LPARAM)L"");
                if(text.length()>0) {
                    int parsed=Parse_Page_Command(text.get());
                    if(parsed!=0) {
                        if(parsed==1) {
                            int count=0;
                            unsigned length=text.length()+1;
                            wchar_t *copy=(wchar_t *)operator new(length*2);
                            wcscpy(primary_command,StringTable::LoadString("TXT_PAGE_COMMAND",0,"D:\\Ra2\\wonline.cpp",0x3d3b));
                            if(!copy) return 0;
                            copy=wcsncpy(copy,text.get(),length);
                            if(!copy) return 0;
                            wchar_t *body=copy+wcslen(primary_command);
                            while(*body && wcsncmp(body,L" ",1)==0) ++body;
                            while(*body) {
                                if(wcsncmp(body,L" ",1)==0 || count>10) break;
                                nickname[count++]=*body++;
                            }
                            nickname[count]=0;
                            while(*body && wcsncmp(body,L" ",1)==0) ++body;
                            if(count>10) Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NAME_TOO_LONG",0,"D:\\Ra2\\wonline.cpp",0x3d59));
                            else if(count==0) Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_NO_NAME_SPECIFIED",0,"D:\\Ra2\\wonline.cpp",0x3d5b));
                            else if(wcslen(body)>0) Lobby_Chat_Printf(ColorPriv,L"[%hs > %s] %s",g_NickName,nickname,body);
                            else Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_CANT_SEND_BLANK_PAGE",0,"D:\\Ra2\\wonline.cpp",0x3d61));
                            operator delete(copy);
                        }
                    } else if(Reply_To_Last_Pager(text.get())) {
                        unsigned length=text.length()+1;
                        wchar_t *copy=(wchar_t *)operator new(length*2);
                        wcscpy(primary_command,StringTable::LoadString("TXT_REPLY_COMMAND",0,"D:\\Ra2\\wonline.cpp",0x3d73));
                        wcscpy(alternate_command,StringTable::LoadString("TXT_REPLY_COMMAND_SHORT",0,"D:\\Ra2\\wonline.cpp",0x3d74));
                        if(!copy) return 0;
                        copy=wcsncpy(copy,text.get(),length);
                        if(!copy) return 0;
                        wchar_t *body=copy+(wcsncmp(copy,primary_command,wcslen(primary_command))==0 ? wcslen(primary_command):wcslen(alternate_command));
                        while(*body && wcsncmp(body,L" ",1)==0) ++body;
                        wcscpy(nickname,Narrow_To_Wide(LastPagerName));
                        if(wcslen(nickname)>0 && wcslen(body)>0)
                            Lobby_Chat_Printf(ColorPriv,L"[%hs > %s] %s",g_NickName,nickname,body);
                        else if(wcslen(nickname)>0)
                            Lobby_Chat_Printf(-1,StringTable::LoadString("TXT_CANT_SEND_BLANK_PAGE",0,"D:\\Ra2\\wonline.cpp",0x3d91));
                        operator delete(copy);
                    } else if(!Parse_Online_Command(text.get())) {
                        int color=ColorMe;
                        if(Send_Chat_Message(text)==1) color=ColorPriv;
                        Lobby_Chat_Printf(color,L"[%hs] %s",g_NickName,text.get());
                    }
                }
            }
            return 0;
        }
        case 0x510: case 0x513: case 0x514:
        case 0x51e: case 0x51f: case 0x520: case 0x521: case 0x522: case 0x523:
        case 0x524: case 0x525: case 0x526: case 0x527: case 0x528:
        case 0x6a1: case 0x6a2:
            if((unsigned short)(wparam>>16)!=1) return 0;
        case 0x7fff: {
            DebugString("Sending private game options\n");
            int side_player=Side_Combo_Player(command);
            int color_player=Color_Combo_Player(command);
            int player=side_player>color_player?side_player:color_player;
            int side=Player_Side_Combo_ID(player);
            int color=Player_Color_Combo_ID(player);
            if(player==side_player) Set_Slot_Country(window,side);
            else Set_Slot_Color(window,color);
            int requested_side=Side_Combo_Data(window,side,-1);
            int requested_color=Color_Combo_Data(window,color,-1);
            unsigned char network[4],node[6];
            unsigned short socket;
			Session.LocalAddress.Get_Address(network, node, &socket);
            char options[128];
            sprintf(options,"R%d,%d,%X,%x,%x",requested_side,requested_color,*(unsigned int *)node,FirewallHelper.Detected&0xffff,Session.PortBase);
            DebugString("Sending private game options (%s)\n",options);
            Wstring host=Wstring_Narrow(Session.GameName);
            Send_Player_Choice(host.get(),options);
            return 0;
        }
        }
        return 0;
    }
	case OD_SUBCLASSED: {
        SetFocus(GetDlgItem(window,0x540));
        SendDlgItemMessageA(window,0x3ed,0x49c,0,(LPARAM)SurfaceCache.SurfaceCacheClass::GetSurface("wouact.pcx",0));
        SendDlgItemMessageA(window,0x3ed,0x4aa,0,(LPARAM)SurfaceCache.SurfaceCacheClass::GetSurface("wodact.pcx",0));
        MultiplayerGameMode *mode=First_Game_Mode();
        Session.Options.MPModeIndex=mode->MPModeIndex;
        Set_Item_Text(window,mode);
        Session.Options.ScenarioIndex=mode->FirstValidMapIndex();
        Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex);
        Fill_Side_Combos(window);
        Init_Color_Entries();
        Fill_Color_Combos(window);
        Set_Scenario_Info_From_Index(-1);
        LobbyDialogFlag=false;
        Session.ScenarioMaxPlayers=8;
        Set_Skirmish_Dialog_Text(window);
        Update_Network_Dialog_Preview(window);
        Fill_Options_Dialog(window,1);
        Draw_Player_List(0,false);
        return 0;
    }
    case 0x4e9: {
        ToolTipTextRequest *request=(ToolTipTextRequest *)lparam;
        int id;
        {
            WideWstring &text=request->Text;
            text=(wchar_t const *)0;
            if(!request->Window) goto tooltip_done;
            id=GetDlgCtrlID((HWND)request->Window);
            if(request->Item!=-1) {
                if(Control_Slot(id)==-1) {
                    if(Side_Combo_Player(id)==-1) {
                        if(Color_Combo_Player(id)!=-1) {
                            text=Color_Name(Color_Combo_Data(window,id,request->Item));
                            return 1;
                        }
                    } else {
                        int country=Side_Combo_Data(window,id,request->Item);
                        if(country!=-1) { text=Side_Name(country); return 1; }
                    }
                } else {
                    NodeNameType *entry=(NodeNameType *)Combo_Selected_Data(window,id,request->Item);
                    text=Entry_Type_Name(entry);
                    return 1;
                }
                goto tooltip_done;
            }
            if(Control_Slot(id)!=-1) {
                NodeNameType *entry=(NodeNameType *)Combo_Selected_Data(window,id,-1);
                if(entry) {
                    WideWstring label=OwnerDraw::Control_Tooltip((unsigned)window,(HWND)request->Window);
                    if((wcscmp(label.get(),StringTable::LoadString("STT:GuestComboPlayer",0,"D:\\Ra2\\wonline.cpp",0x3c92))==0 ||
                        wcscmp(label.get(),StringTable::LoadString("STT:HostComboPlayer",0,"D:\\Ra2\\wonline.cpp",0x3c92))==0) && Is_Real_Player_Entry(entry))
                        label=Player_Info_Tooltip(entry->Name);
                    text=label.get();
                }
            }
        }
        {
            int player;
            if(id==0x754) player=0;
            else if(id==0x755) player=1;
            else if(id==0x756) player=2;
            else if(id==0x757) player=3;
            else if(id==0x758) player=4;
            else if(id==0x759) player=5;
            else if(id==0x75a) player=6;
            else if(id==0x75b) player=7;
            else goto tooltip_done;
            WideWstring ping;
            int slot=Get_Slot_Control_Id(player);
            if(slot!=-1) {
                NodeNameType *entry=(NodeNameType *)Combo_Selected_Data(window,slot,-1);
                if(entry && Is_Real_Player_Entry(entry)) {
                    ping=Player_Ping_Tooltip(entry->Name);
                    request->Text=ping;
                }
            }
        }
tooltip_done:
        return 1;
    }
    case 0x4ed:
        ShowWindow(window,0);
        OwnerDraw::Set_Paint_Disabled(window,1);
        return 0;
    }
    return 0;
}

LRESULT CALLBACK Wonline_Wait_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x0074D5E0, the dialog proc of the wait dialog (0xC4)
#define OD_SETLABEL (WM_USER + 178) // RA2: 0x4B2, sets the wide text of the control (name guessed from its use)

void Show_Wait_Window(unsigned int event, WideWstring const & text, bool block) // RA2: 0x0074D4D0, not in TibSun: opens the wait dialog (0xC4) with the text, or when it is open adds the events it waits for and changes its text; block waits until it closes
{
	HWND dialog = WS_Find_Dialog(0xC4);
	if (dialog) { // RA2: the dialog is open
		g_ActiveWaitFlags |= event;
		if (text.length() > 0) {
			HWND label = GetDlgItem(dialog, 0x5B2);
			SendMessage(label, OD_SETLABEL, 0, (LPARAM)text.text());
		}
	} else if (event) {
		HWND focus = WS_Top_Window(); // RA2: the control that has the focus goes back to it
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

int g_WaitDialogDelay = 7000; // RA2: 0x007FD384, 7000 milliseconds: how long the wait dialog stays hidden

LRESULT CALLBACK Wonline_Wait_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x0074D5E0, not in TibSun: the dialog proc of the wait dialog (0xC4): the abort button sets the first wait event, the other opens the network status page; the dialog shows itself after a delay
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
		if (wparam == 0x45) { // RA2: the delay is over: the dialog shows
				ShowWindow(window, SW_SHOW);
				KillTimer(window, 0x45);
		}
			break;
	case OD_SUBCLASSED: // RA2: the dialog was made
			SendMessage(window, OD_SETTOP, 0, 1);
			SetTimer(window, 0x45, g_WaitDialogDelay, NULL);
			OwnerDraw::Set_Dialog_Waiting(window);
			break;
	}
	return(0);
}

void Set_Wait_Window_Text(LPARAM text) // RA2: 0x0074D750, not in TibSun: changes the wide text of the wait dialog when it is open; no caller (expanded where it is used)
{
	HWND dialog = WS_Find_Dialog(0xC4);
	if (dialog != NULL) {
		SendDlgItemMessage(dialog, 0x5B2, OD_SETLABEL, 0, text);
	}
}



int PlayerPings[8]; // RA2: 0x00B2A734, the ping of each player of the game (globals.h)

void __fastcall User_List_To_Session_Players(User * user) // RA2: 0x0074D7D0, not in TibSun (the executable's debug text names it): copies what the chat knows about the users into the session: the squad of each player, the address of the channel owner (the host), the pings, and the broadcast addresses of the players; the argument is not used
{
	Session.NumPlayers = 1; // RA2: the local player is slot 0
	User entry; // RA2: a copy of each user of the channel
	for (int i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(entry, i);
		NodeNameType * player = Find_Lobby_Name(Narrow_To_Wide((char const *)entry.name));
		if (player && Is_Real_Player_Entry(player)) {
			player->Player.SquadID = entry.squadID;
		}
		if (stricmp((char const *)entry.name, g_NickName.get())) { // RA2: the others
			if (entry.flags & 1) { // RA2: the owner of the channel hosts the game
				NodeNameType * owner = Find_Lobby_Name(Narrow_To_Wide((char const *)entry.name));
				NetNumType network;
				NetNodeType node;
				owner->Address.Get_Address(network, node);
				Session.HostAddress.Set_Address(network, node, 0);
			}
			int ping = 0;
			g_pNetUtil->GetAvgPing(entry.ipaddr, &ping); // RA2: virtual 0x20
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

User g_OwnerUser; // RA2: 0x00B2A658, the copy of the channel owner's user record Find_Owner_User returns
int g_PlayerCheckBoxIDs[8] = { 0x769, 0x761, 0x764, 0x762, 0x765, 0x768, 0x766, 0x767 }; // RA2: 0x007FD334, the ids of the check boxes of the players in the readiness dialog (0x12C)
bool g_PlayerChecked[8]; // RA2: 0x00B28DCC, which of them were ticked
bool User_Is_Channel_Owner(Wstring const & name); // RA2: 0x0074D9E0, defined below

bool __fastcall Local_User_Is_Channel_Owner(void) // RA2: 0x0074D960, not in TibSun: the local user is the owner of the channel (WOL)
{
	return(User_Is_Channel_Owner(g_NickName)); // RA2: expanded here
}

bool User_Is_Channel_Owner(Wstring const & name) // RA2: 0x0074D9E0, not in TibSun: the channel owner (the user with flag 1) has this name; no caller (expanded where it is used)
{
	User * owner = Find_Owner_User(); // RA2: expanded here
	if (owner == NULL) {
		return(false);
	}
	if (stricmp((char *)owner->name, name.text()) == 0) {
		return(true);
	}
	return(false);
}

User * Find_Owner_User(void) // RA2: 0x0074DA60, not in TibSun: the user of the channel with the owner flag (1), a copy kept in a global; NULL when there is none
{
	for (int i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(g_OwnerUser, i);
		if (g_OwnerUser.flags & 1) {
			return(&g_OwnerUser);
		}
	}
	return(NULL);
}

void __fastcall Mark_Player_Checked(int index) // RA2: 0x0074DAB0, not in TibSun: ticks the check box of a player in the readiness dialog (0x12C) and has the dialog redrawn
{
	if (index >= 0 && index < Session.Players.Count()) {
		HWND dialog = WS_Find_Dialog(0x12C);
		if (dialog) {
			HWND box = GetDlgItem(dialog, g_PlayerCheckBoxIDs[index]);
			g_PlayerChecked[index] = true;
			SendMessage(box, BM_SETCHECK, 1, 0); // RA2: 0xF1
			PostMessage(dialog, WM_PAINT, 0, 0); // RA2: 0xF
		}
	}
}

LRESULT CALLBACK Player_Readiness_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x0074DB40, the dialog proc of the readiness dialog (0x12C)

void Open_Readiness_Dialog(void) // RA2: 0x0074DB10, not in TibSun: opens the dialog (0x12C) that shows which players are ready; no caller (expanded where it is used)
{
	HWND dialog = WS_Create_Dialog(ProgramInstance, 300, MainWindow, (DLGPROC)Player_Readiness_Dialog_Proc, 0);
	if (dialog != NULL) {
		Show_Dialog(dialog, 300);
	}
}

bool g_PlayerReady[8]; // RA2: 0x00B28DC4, the check boxes the program has ticked (read by the readiness dialog proc)

LRESULT CALLBACK Player_Readiness_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x0074DB40, not in TibSun: the dialog proc of the readiness dialog (0x12C): a check box for each player of the game, set by Mark_Player_Checked; the player cannot change them
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
		case 0x766: case 0x767: case 0x768: case 0x769: // RA2: the check boxes of the players
			if (g_PlayerReady[wparam - 0x761]) { // RA2: ticked by the program: it stays ticked
						if (SendDlgItemMessage(window, wparam, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
							SendDlgItemMessage(window, wparam, BM_SETCHECK, BST_CHECKED, 0);
				}
			} else {
						if (SendDlgItemMessage(window, wparam, BM_GETCHECK, 0, 0) == BST_CHECKED) {
							SendDlgItemMessage(window, wparam, BM_SETCHECK, BST_UNCHECKED, 0);
							return(0);
				}
			}
		}
			break;
		case WM_CTLCOLORMSGBOX: case WM_CTLCOLOREDIT: case WM_CTLCOLORLISTBOX: case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG: case WM_CTLCOLORSCROLLBAR: case WM_CTLCOLORSTATIC:
		return((long)GetStockObject(BLACK_BRUSH)); // RA2: the controls are painted on black
	case OD_SUBCLASSED: // RA2: the dialog was made
			for (int i = 0; i < 8; ++i) {
				HWND control = GetDlgItem(window, g_PlayerCheckBoxIDs[i]);
				memset(g_PlayerChecked, 0, 8);
			if (i > Session.Players.Count() - 1) { // RA2: no player: the check box goes
					DestroyWindow(control);
			}
		}
			for (int player = 0; player < Session.Players.Count(); ++player) {
				HWND control = GetDlgItem(window, g_PlayerCheckBoxIDs[player]);
			SendMessage(control, OD_SETTEXTW, 0, (LPARAM)Session.Players[player]->Name); // RA2: the name of the player
				SendMessage(control, BM_SETCHECK, BST_UNCHECKED, 0);
		}
			SendMessage(window, OD_SETTOP, 0, 1);
			OwnerDraw::Set_Dialog_Waiting(window);
			ShowWindow(window, SW_SHOW);
			break;
	}
	return(0);
}

void Close_Readiness_Dialog(void) // RA2: 0x0074DD00, not in TibSun: closes the readiness dialog (0x12C) when it is open
{
	HWND dialog = WS_Find_Dialog(0x12C);
	if (dialog) {
		WS_Destroy_Dialog(dialog, 0);
	}
}

int g_ConnectionStatus[8]; // RA2: 0x00B26A2C, how the connection of each player of the game ended (0 reported, 1 local, 2 pserver port, 3 host aborted)
int g_ConnectionStatusRank[4] = { 1, 2, 0, 1 }; // RA2: 0x007FD324, 1, 2, 0, 1: the statuses a connection state may replace

void __fastcall Set_Connection_Status(int index, int status) // RA2: 0x0074DD20, not in TibSun: records how the connection of a player ended, unless the status it has is already worse; no caller (expanded where it is used)
{
	if (index < Session.Players.Count() && g_ConnectionStatus[index] < g_ConnectionStatusRank[index]) {
		g_ConnectionStatus[index] = status;
	}
}

void Report_Connection_Errors(void) // RA2: 0x0074DD50, not in TibSun: prints into the lobby chat what went wrong with the connections of the players: the status each player's connection ended with, the channel owner being the host; a general text when none is known
{
	wchar_t message[256];
	User * owner = Find_Owner_User(); // RA2: expanded here
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
				if (i == owner_index) { // RA2: the host could not be reached
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

unsigned short __fastcall Get_Port_Blitz_Port_Number(int send_port, int mangled_port, int unused, int port_spread) // RA2: 0x0074DFD0, not in TibSun (the executable's debug text names it so): the port to send to for a peer whose router allocates ports by this spread (1 consecutive, 2 every second, a larger one in pairs, a negative one downwards), never below 1024; no caller (expanded where it is used)
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


int __fastcall Build_Connection_Pairs(int (* pairs)[2]) // RA2: 0x0074E070, not in TibSun: orders the players of the game by how awkward their connections are (FirewallHelper.Rank_A of their state) and writes every pair of them into the table; the number of pairs
{
	int order[8]; // RA2: the slots of the players
	memset(order, 0, sizeof(order));
	int i = 0;
	int * out = order;
	for (; i < 8; i++) { // RA2: the slots that hold real players
		if (Is_Real_Player_Entry(Session.Slots[i])) {
			*out++ = i;
		}
	}
	for (int a = 0; a < Session.Players.Count(); a++) { // RA2: sorted, the most awkward first
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

void __fastcall Register_Connection_State(char const * sender, char const * receiver, int state) // RA2: 0x0074E1F0, not in TibSun: a player reported the state of its connection to another (both by name): stored for the pair of slots in either direction
{
	int sender_slot = -1;
	int receiver_slot = -1;
	DebugString("Registering connection state from %s to %s. conn_state = %d\n", sender, receiver, state);
	int i = 0;
	for (NodeNameType ** slot = Session.Slots; (long)slot < (long)(Session.Slots + 8); ++slot, ++i) { // RA2: the slots of the game
		if (stricmp(Wide_To_Narrow((*slot)->Name), sender) == 0) {
			sender_slot = i;
		}
		if (stricmp(Wide_To_Narrow((*slot)->Name), receiver) == 0) {
			receiver_slot = i;
		}
	}
	for (i = 0; i < Session.ConnectionResultCount; ++i) { // RA2: the pair is looked up
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

void __fastcall Store_Player_Port(char const * name, unsigned short port) // RA2: 0x0074E2D0, not in TibSun: a player says which port to send to: kept for its slot
{
	DebugString("Player %s is telling us we should send to port %d\n", name, port);
	for (int i = 0; i < 8; ++i) {
		if (!stricmp(Wide_To_Narrow(Session.Slots[i]->Name), name)) {
			Session.PeerPorts[i] = port;
			return;
		}
	}
}

void __fastcall Send_User_Port(char const * name, unsigned short port) // RA2: 0x0074E330, not in TibSun: tells the user of this name by private game options string ("Z4" and the port) which port to send to
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
			g_pChat->RequestPrivateGameOptions(user, text); // RA2: virtual 0x38
		}
	}
}

void __fastcall Send_Public_Options(char kind, char const * text) // RA2: 0x0074E3F0, not in TibSun: sends a public game options string "Z1", the kind and the text to the channel
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
	g_pChat->RequestPublicGameOptions(options); // RA2: virtual 0x3C
}

void Resend_Pending_Requests(void) // RA2: 0x0074E470, not in TibSun: sends a keepalive (NET_PING) to each peer whose countdown ran out and restarts it
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
			Session.KeepAliveTimer[index] = 1200; // RA2: 20 seconds
		}
	}
}

void __fastcall Queue_Peer_Timeout(int peer) // RA2: 0x0074E590, not in TibSun: queues a keepalive for this slot
{
	Session.KeepAliveSlot[Session.KeepAliveCount] = peer;
	Session.KeepAliveTimer[Session.KeepAliveCount] = 1200;
	++Session.KeepAliveCount;
}

bool __fastcall Start_Talking_To(int player, int pair_slot, int retries); // RA2: 0x0074E5E0, the executable's debug text "Start_Talking_To player %d, retries = %d" names it: makes the connection to one peer by port probes, tries again up to the number of retries; true when it is made

#ifdef RA2_NON_MATCHING // not exact: 2112 bytes differ against the image-derived bound 0074e5e0 (2,456 body bytes, 8 padding) (was 2261; the peer-name and firewall tests as ra2-decomp's banked form banks/0x00793D20_nat.cpp (11df85d) writes them, RA2's packet filtering, port storage and retries kept; all 66 calls in order)
bool __fastcall Start_Talking_To(int player, int pair_slot, int retries)
{
	CDTimerClass<SystemTimerClass> timer;
	CDTimerClass<SystemTimerClass> resend;
	bool negotiate_port = true;
	int length = 0;
	IPXAddressClass sender;
	int packet_index = 0;
	bool received = false;

	DebugString("Start_Talking_To player %d, retries = %d\n", player, retries);
	int player_index = -1;
	int i;
	for (i = 0; i < Session.Players.Count(); ++i) {
		char const * name = Wide_To_Narrow(Session.Players[i]->Name);
		if (!stricmp(name, Wide_To_Narrow(Session.Slots[player]->Name))) {
			player_index = i;
		}
	}
	if (player_index < 0) {
		Send_Public_Options(0, NULL);
		return false;
	}

	for (i = 0; i < MAX_PLAYERS; i++) {
		if (Session.NatPeerNames[i][0] == 0) {
			break;
		}
		if (stricmp(Session.NatPeerNames[i], Wide_To_Narrow(Session.Slots[player]->Name)) == 0) {
			negotiate_port = false;
			break;
		}
	}

	int theirs = Session.WolPlayerState[player];
	if (negotiate_port) {
		if (theirs == 0 || (theirs & 1)) {
			int mine = (unsigned short)FirewallHelper.Detected;
			if (mine == 0 || (mine & 1)) {
				negotiate_port = false;
			}
		}
	}

	int attempt = 0;
	unsigned short previous_port = 0;
	unsigned short source_port = 0;
	do {
		Resend_Pending_Requests();
		if (negotiate_port) {
			source_port = 0;
			if (!(FirewallHelper.Detected & 0x40)) {
				DebugString("No destination port delta - looking for earlier connections at the same address\n");
				for (i = 0; i < Session.KeepAliveCount; ++i) {
					int slot = Session.KeepAliveSlot[i];
					IPXAddressClass address = Session.Slots[slot]->Address;
					if (address.Is_Same_Node(Session.Slots[player]->Address)) {
						int port;
						memcpy(&port, &Session.PeerAttemptBlock[3 + slot * sizeof(port)], sizeof(port));
						source_port = port;
						DebugString("Found old connection port number to use - port number %d\n", source_port);
					}
				}
			}
			if (!source_port) {
				for (i = 0; i < Session.Players.Count(); ++i) {
					char const * name = Wide_To_Narrow(Session.Players[i]->Name);
					if (!stricmp(name, g_NickName.get())) {
						source_port = FirewallHelper.Get_Next_Mangled_Source_Port(Session.PlayerPorts[i]);
						break;
					}
				}
			}
			Send_User_Port(Wide_To_Narrow(Session.Slots[player]->Name), source_port);
		} else {
			Session.PeerPorts[player] = Session.PlayerPorts[player_index];
		}
		timer = CDTimerClass<SystemTimerClass>(360);
		if (attempt > 0) {
			timer = CDTimerClass<SystemTimerClass>(240);
		}
		while (timer && ((negotiate_port && Session.PeerPorts[player] == previous_port)
			|| (!negotiate_port && Session.PeerPorts[player] == 0))) {
			WOL_Wait_Callback();
		}
		previous_port = Session.PeerPorts[player];
		if (previous_port) {
			Session.PlayerPorts[player_index] = previous_port;
			Session.Players[player_index]->Address.Set_Port(Session.PeerPorts[player]);
			Session.Slots[player]->Address.Set_Port(Session.PeerPorts[player]);

			GlobalPacketType probe;
			memset(&probe, 0, 0x2f);
			probe.Command = NET_PORT_PROBE;
			strcpy(probe.Name, g_NickName.get());
			if (FirewallHelper.Is_Netgear() && !FirewallHelper.Is_Netgear(Session.WolPlayerState[player])) {
				DebugString("Doing the netgear sleep thing\n");
				CDTimerClass<SystemTimerClass> delay(240);
				while (delay) {
					WOL_Wait_Callback();
				}
			}
			DebugString("Sending NET_PORT_PROBE packet to %s at port %d\n", Wide_To_Narrow(Session.Slots[player]->Name), Session.PlayerPorts[player_index]);
			timer = CDTimerClass<SystemTimerClass>(360);
			resend = CDTimerClass<SystemTimerClass>(0);
			do {
				if (!resend) {
					Ipx.Send_Global_Message_To_Name(&probe, 0x2f, 0, Session.Slots[player]->Name);
					resend = CDTimerClass<SystemTimerClass>(30);
				}
				WOL_Wait_Callback();
				if (!received) {
					if (packet_index >= Ipx.Global_Num_Receive()) {
						packet_index = 0;
					}
					GlobalPacketType packet;
					unsigned short product;
					if (Ipx.Peek_Global_Message(packet_index, &packet, &length, &sender, &product, NULL) && product == 0xaa03) {
						switch (packet.Command) {
							case NET_PORT_PROBE:
								DebugString("Got NET_PORT_PROBE packet\n");
								if (Session.Players[player_index]->Address.Is_Same_Node(sender) && !stricmp(Wide_To_Narrow(Session.Slots[player]->Name), packet.Name)) {
									DebugString("Got NET_PORT_PROBE packet from %s\n", packet.Name);
									Send_Public_Options(11, packet.Name);
									Mark_Player_Checked(player_index);
									Queue_Peer_Timeout(player);
									received = true;
									int port = source_port;
									memcpy(&Session.PeerAttemptBlock[3 + player * sizeof(port)], &port, sizeof(port));
								}
								// The probe has been handled and can leave the receive queue.
							case NET_PING:
							case NET_MANGLER_RESPONSE:
								Ipx.Mark_Global_Message_Read(packet_index);
								break;
						}
					}
				}
				if (received && Session.ConnectionResults[pair_slot >> 1][pair_slot & 1] == 11) {
					DebugString("Got connection result - my packet must have got through\n");
					break;
				}
				int result = Session.ConnectionResults[pair_slot >> 1][pair_slot & 1];
				if (result >= 1 && result < 11 && result - 1 > attempt) {
					DebugString("Other player is trying again - their_try = %d\n", result - 1);
					attempt = result - 2;
					break;
				}
				++packet_index;
			} while (timer);
		}
		if (received && Session.ConnectionResults[pair_slot >> 1][pair_slot & 1] == 11) {
			break;
		}
		if (attempt >= retries) {
			break;
		}
		++attempt;
		Send_Public_Options(attempt + 1, Wide_To_Narrow(Session.Slots[player]->Name));
		DebugString("Trying again - trying = %d\n", attempt);
	} while (true);
	return received;
}
#endif

bool Connect_Player_Pairs(void) // RA2: 0x0074EF80, not in TibSun: makes the connections of the pairs of players Build_Connection_Pairs arranged: talks to the peer of each pair the local player is in (Start_Talking_To), waits for the others to connect among themselves; false when one of them fails
{
	CDTimerClass<SystemTimerClass> timer;
	Session.KeepAliveCount = 0;
	memset(&Session.PeerAttemptBlock[3], 0, 0x20); // RA2: the eight values at +0x2EF4 are cleared
	int pair_slot = -1;
	g_SuspendChatPump = 0; // RA2: the chat is pumped again
	if (Session.ConnectionResultCount == 0) { // RA2: no pair to make
		return(false);
	}
	((SessionPortAllocator &)Session).Allocate_Player_Ports();
	if (Session.Players.Count() > 0) { // RA2: the first check box of the readiness dialog (the local player) is ticked
		HWND window = WS_Find_Dialog(300);
		if (window) {
			HWND control = GetDlgItem(window, g_PlayerCheckBoxIDs[0]);
			g_PlayerChecked[0] = true;
			SendMessage(control, BM_SETCHECK, 1, 0);
			PostMessage(window, WM_PAINT, 0, 0);
		}
	}
	for (int pair = 0; pair < Session.ConnectionResultCount; ++pair) {
		int first = Session.PeerProbeData[pair][0]; // RA2: the slots of the pair
		int second = Session.PeerProbeData[pair][1];
		int peer = -1;
		NodeNameType * first_player = Session.Slots[first];
		NodeNameType * second_player = Session.Slots[second];
		char const * first_name = Wide_To_Narrow(first_player->Name);
		char const * second_name = Wide_To_Narrow(second_player->Name);
		DebugString("Connecting pair %d, %s and %s\n", pair, first_name, second_name);
		char const * name = Wide_To_Narrow(Session.Slots[first]->Name);
		if (stricmp(name, g_NickName.get()) == 0) { // RA2: the local player is the first of the pair
			peer = second;
			pair_slot = pair * 2 + 1;
			Session.ConnectionResults[pair][0] = 11;
		}
		name = Wide_To_Narrow(Session.Slots[second]->Name);
		if (stricmp(name, g_NickName.get()) == 0) { // RA2: or the second
			peer = first;
			pair_slot = pair * 2;
			Session.ConnectionResults[pair][1] = 11;
		}
		if (peer != -1) { // RA2: the local player is in the pair
			int step; // RA2: the harder of the two connections decides how many retries
			if (FirewallHelper.Rank_B(Session.WolPlayerState[first]) > FirewallHelper.Rank_B(Session.WolPlayerState[second])) {
				step = FirewallHelper.Rank_B(Session.WolPlayerState[first]);
			} else {
				step = FirewallHelper.Rank_B(Session.WolPlayerState[second]);
			}
			if (!Start_Talking_To(peer, pair_slot, step)) {
				return(false);
			}
		} else { // RA2: the others connect among themselves: wait for both results
			DebugString("Waiting for other guys to connect\n");
			timer = CDTimerClass<SystemTimerClass>(960); // RA2: 16 seconds
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
				if (first_status >= 1 && first_status < 11 && first_status - 1 > progress) { // RA2: the first one tries again
					DebugString("Connecting player 1 is trying again - try_now = %d\n", first_status - 1);
					progress = first_status - 1;
					timer = CDTimerClass<SystemTimerClass>(840); // RA2: 14 seconds more
				}
				if (second_status >= 1 && second_status < 11 && second_status - 1 > progress) { // RA2: the second one
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


int __fastcall Negotiate_Scenario_Transfer(bool connected); // RA2: 0x00750330, defined below

void Start_Game_Now(void) // RA2: 0x0074F300, its own debug text names it: once every player is ready, makes the connections of the pairs, has the guests take the host's scenario, sets the timing from the pings and the game speed, and starts the game
{
	DebugString("Start_Game_Now\n");
	g_StartGameRunning = true;
	PacketTransport->Discard_In_Buffers();
	PacketTransport->Discard_Out_Buffers();
	memset(g_ConnectionStatus, 0xff, sizeof(g_ConnectionStatus));
	HWND dialog = WS_Create_Dialog(ProgramInstance, 300, MainWindow, (DLGPROC)Player_Readiness_Dialog_Proc, 0); // RA2: the readiness dialog (0x12C)
	if (dialog) {
		Show_Dialog(dialog, 300);
	}
	for (int n = 0; n < 100; ++n) {
		Call_Back();
	}
	Ipx.Discard_Old_Global_Packets(300);
	bool connected = Connect_Player_Pairs();
	if (!connected) {
		g_SuspendChatPump = 0;
		if (!Local_User_Is_Channel_Owner()) {
			DebugString("Failed to verify connection paths\n");
		}
	}
	int transfer = Negotiate_Scenario_Transfer(connected);
	if (transfer) {
		DebugString("Negotiate_Scenario_Transfer failed\n");
		if (transfer == 2) {
			ODMessageBox(StringTable::LoadString("TXT_SCENARIOS_DO_NOT_MATCH", NULL, "D:\\Ra2\\wonline.cpp", 0x474B), MB_OK, Wait_Callback_Thunk, false);
		}
		HWND wait = WS_Find_Dialog(300);
		if (wait) {
			WS_Destroy_Dialog(wait, 0);
		}
		WolOptionsLocked = 0;
		if (Is_Tournament_Session()) {
			Set_Match_State(11, -1, 0);
		}
		g_GameStartRequested = false;
		Report_Connection_Errors();
		Start_Network_Session();
		g_StartGameReceived = false;
		g_StartGameRunning = false;
		if (transfer != 2) {
			return;
		}
		g_PlayingNetGame = true;
		DebugString("setting EV_EXIT\n");
		SetEvent(g_WaitEventHandles[EV_EXIT]);
		DebugString("Request channel leave in game start after scenario mismatch\n");
		g_pChat->RequestChannelLeave();
		return;
	}
	for (int i = 0; i < Session.Players.Count(); ++i) { // RA2: a player with no port takes the listen port
		if (!Session.PlayerPorts[i]) {
			Session.PlayerPorts[i] = (unsigned short)UDPInterfaceClass::UDPListenPort;
		}
	}
	User * owner = Find_Owner_User(); // RA2: the packet router listens on the owner's port
	if (owner) {
		for (int j = 0; j < Session.Players.Count(); ++j) {
			if (!stricmp(Wide_To_Narrow(Session.Players[j]->Name), (char *)owner->name)) {
				Session.PacketRouterPort = Session.PlayerPorts[j];
				break;
			}
		}
	}
	HWND wait = WS_Find_Dialog(300);
	if (wait) {
		WS_Destroy_Dialog(wait, 0);
	}
	if (Session.Type != GAME_INTERNET || Online_Session_State != 2) {
		Session.House = 0;
	}
	Session.FrameSendRate = 5;
	switch (Session.Options.GameSpeed) { // RA2: the game speed sets how far ahead and how fast
		case 0: Session.MaxAhead = 40; Session.PrecalcDesiredFrameRate = 60; Session.FrameSendRate = 10; break;
		case 1: Session.MaxAhead = 40; Session.PrecalcDesiredFrameRate = 45; Session.FrameSendRate = 10; break;
		case 2: Session.MaxAhead = 30; Session.PrecalcDesiredFrameRate = 30; break;
		case 3: Session.MaxAhead = 20; Session.PrecalcDesiredFrameRate = 20; break;
		case 4: Session.MaxAhead = 20; Session.PrecalcDesiredFrameRate = 15; break;
		case 5: Session.MaxAhead = 20; Session.PrecalcDesiredFrameRate = 12; break;
		default: Session.MaxAhead = 10; Session.PrecalcDesiredFrameRate = 10; break;
	}
	if (Session.UseRouter) {
		Session.MaxAhead += 20;
	}
	Session.PrecalcMaxAhead = 0;
	owner = Find_Owner_User();
	if (owner == NULL || stricmp((char *)owner->name, g_NickName.text())) { // RA2: only the owner sets the frame rate
		Session.PrecalcDesiredFrameRate = 0;
	}
	int worst_ping = 0;
	for (int p = 0; p < 8; ++p) {
		if (PlayerPings[p] > worst_ping) {
			worst_ping = PlayerPings[p];
		}
	}
	if (Session.UseRouter) {
		worst_ping *= 2;
	}
	Session.LatencyFudge = 1;
	if (worst_ping != 0 && worst_ping != 1000) {
		DebugString("Max ping is %d ms\n", worst_ping);
		if (worst_ping > 1000) {
			Session.LatencyFudge = 2;
		} else if (worst_ping < 600) {
			Session.LatencyFudge = 1;
		}
		int timing = 60 * worst_ping / 1000;
		timing += 120 * timing / 100;
		Ipx.Set_Timing(timing, (unsigned long)-1, timing * 8, true);
		owner = Find_Owner_User();
		if (owner && !stricmp((char *)owner->name, g_NickName.text())) {
			switch (Session.LatencyFudge) {
				case 0: case 1: Session.PrecalcMaxAhead = Session.MaxAhead; break;
				case 2: Session.PrecalcMaxAhead = Session.MaxAhead + (Session.MaxAhead >> 1); break;
				case 3: Session.PrecalcMaxAhead = Session.MaxAhead * 2; break;
			}
			Session.PrecalcMaxAhead = ((unsigned int)Session.PrecalcMaxAhead / 10) * 10;
		}
	} else {
		DebugString("Ping times not available - Using defaults\n");
		Session.PrecalcMaxAhead = (Session.MaxAhead / 10) * 10;
		if ((unsigned int)Session.PrecalcMaxAhead <= 10) {
			Session.PrecalcMaxAhead = 10;
		}
	}
	DebugString("PrecalcMaxAhead is %d\n", Session.PrecalcMaxAhead);
	DebugString("PrecalcDesiredFrameRate is %d\n", Session.PrecalcDesiredFrameRate);
	DebugString("LatencyFudge is %d\n", Session.LatencyFudge);
	DebugString("FrameSendRate is %d\n", Session.FrameSendRate);
	Session.MaxMaxAhead = Session.MaxAhead;
	memset(Session.ConnectionStats, 0, sizeof(Session.ConnectionStats));
	Options.GameSpeed = Session.Options.GameSpeed;
	UDPInterfaceClass::UDPListenPort = 1234; // RA2: the ports go back to their defaults
	AlternateSendPort = 1235;
	AlternateListenPort = 1236;
	LoopbackPort = 1237;
	Frame = 0;
	if (Session.UseRouter) {
		OwnerTalk.Wait_For_Router_Shutdown(2000, true);
	}
	Squad1Names[0] = 0;
	Squad2Names[0] = 0;
	if (Session.Type == GAME_INTERNET) {
		bool squads = Is_Current_Channel_Flag_8(); // RA2: a squad game: the names of the two squads
		if (squads) {
			User * user = NULL;
			int first_squad = 0;
			int second_squad = 0;
			int i = 0;
			if (g_UserList.length() <= 0) goto squads_done;
squad_next:
			{
			g_UserList.getPointer(&user, i);
			if (first_squad == 0 || first_squad == (int)user->squadID) {
				strcat(Squad1Names, (char *)user->name);
				strcat(Squad1Names, ",");
				first_squad = user->squadID;
			} else if (second_squad == 0 || second_squad == (int)user->squadID) {
				strcat(Squad2Names, (char *)user->name);
				strcat(Squad2Names, ",");
				second_squad = user->squadID;
				}
			}
			++i;
			if (i < g_UserList.length()) goto squad_next;
squads_done:;
		}
		if (Online_Session_State == 2) { // RA2: a tournament game: the two sides
			User * user = NULL;
			for (int i = 0; i < g_UserList.length(); ++i) {
				g_UserList.getPointer(&user, i);
				if (g_UserInfo[i].house == 0) {
					strcat(Squad1Names, (char *)user->name);
					strcat(Squad1Names, ",");
				} else {
					strcat(Squad2Names, (char *)user->name);
					strcat(Squad2Names, ",");
				}
			}
		}
	}
	DebugString("setting EV_STARTGAME\n");
	SetEvent(g_WaitEventHandles[EV_STARTGAME]);
	if (Is_Tournament_Session()) {
		Set_Match_State(11, -1, 0);
	}
	g_PlayingNetGame = true;
	DebugString("Request channel leave in game start\n");
	g_pChat->RequestChannelLeave();
	PregameSetup();
	g_StartGameReceived = false;
	g_StartGameRunning = false;
}

/*
**	RA2: the record of one TCP connection read from the MIB-II connection table
**	(text after ra2-decomp's twin, Yuri's Revenge 0x00795650).
*/
struct tConnInfoStruct {
	unsigned int State;
	unsigned long LocalIP;
	unsigned short LocalPort;
	unsigned long RemoteIP;
	unsigned short RemotePort;
};

BOOL (__stdcall * SnmpExtensionInitPtr)(DWORD uptime, HANDLE * trap_event, AsnObjectIdentifier * first_region); // RA2: 0x00B2A754
BOOL (__stdcall * SnmpExtensionQueryPtr)(BYTE pdu_type, SnmpVarBindList * bind_list, AsnInteger32 * error_status, AsnInteger32 * error_index); // RA2: 0x00B294B0
LPVOID (__stdcall * SnmpUtilMemAllocPtr)(DWORD bytes); // RA2: 0x00B2A718
VOID (__stdcall * SnmpUtilMemFreePtr)(LPVOID mem); // RA2: 0x00B2A8DC

bool __fastcall Find_Local_Address(IPXAddressClass * address) // RA2: 0x0074FCB0, its debug text says "Finding local address used to talk to the chat server": reads the TCP connection table through INETMIB1.DLL and takes the local end of the established connection to the chat server; text after ra2-decomp's twin (Yuri's Revenge 0x00795650)
{
	enum {
		ESTABLISHED = 5
	};

	enum {
		tcpConnState = 1,
		tcpConnLocalAddress,
		tcpConnLocalPort,
		tcpConnRemAddress,
		tcpConnRemPort
	};

	char server_name[128];
	unsigned char server_address[4];
	unsigned char remote_address[4];
	HANDLE trap_handle;
	AsnObjectIdentifier first_supported_region;
	DynamicVectorClass<tConnInfoStruct *> connection_list;
	int last_field;
	int index;
	AsnInteger error_status;
	AsnInteger error_index;
	int conn_entry_type_index;
	int conn_entry_type;
	bool found;
	unsigned int server_port = 0;
	NetNumType net;
	NetNodeType node;

	if (g_CurrentServerIndex < 1 || g_CurrentServerIndex >= g_Servers.length()) {
		return(false);
	}

	DebugString("Finding local address used to talk to the chat server\n");

	Server * server = NULL;
	g_Servers.getPointer(&server, g_CurrentServerIndex);
	memset(server_name, 0, sizeof(server_name));
	strncpy(server_name, (char *)server->conndata + 4, sizeof(server_name));

	char * semi_colon_ptr = strchr(server_name, ';');
	if (semi_colon_ptr) {
		*semi_colon_ptr = 0;
		sscanf(semi_colon_ptr + 1, "%d", &server_port);
	} else {
		DebugString("Failed to parse server name\n");
		return(false);
	}
	DebugString("Current chat server name is %s\n", server_name);
	DebugString("Chat server port is %d\n", server_port);

	DebugString("About to call gethostbyname\n");
	hostent * host_info = gethostbyname(server_name);

	if (!host_info) {
		DebugString("gethostbyname failed! Error code %d\n", WSAGetLastError());
		return(false);
	}

	memcpy(server_address, &host_info->h_addr_list[0][0], 4);
	unsigned long temp = *((unsigned long *)(&server_address[0]));
	temp = ntohl(temp);
	*((unsigned long *)(&server_address[0])) = temp;

	DebugString("Host address is %d.%d.%d.%d\n", server_address[3], server_address[2], server_address[1], server_address[0]);

	DebugString("About to load INETMIB1.DLL\n");

	HINSTANCE mib_ii_dll = LoadLibraryA("inetmib1.dll");
	if (mib_ii_dll == NULL) {
		DebugString("Failed to load INETMIB1.DLL\n");
		return(false);
	}

	DebugString("About to load SNMPAPI.DLL\n");

	HINSTANCE snmpapi_dll = LoadLibraryA("snmpapi.dll");
	if (snmpapi_dll == NULL) {
		DebugString("Failed to load SNMPAPI.DLL\n");
		FreeLibrary(mib_ii_dll);
		return(false);
	}

	SnmpExtensionInitPtr = (BOOL (__stdcall *)(DWORD, HANDLE *, AsnObjectIdentifier *))GetProcAddress(mib_ii_dll, "SnmpExtensionInit");
	SnmpExtensionQueryPtr = (BOOL (__stdcall *)(BYTE, SnmpVarBindList *, AsnInteger32 *, AsnInteger32 *))GetProcAddress(mib_ii_dll, "SnmpExtensionQuery");
	SnmpUtilMemAllocPtr = (LPVOID (__stdcall *)(DWORD))GetProcAddress(snmpapi_dll, "SnmpUtilMemAlloc");
	SnmpUtilMemFreePtr = (VOID (__stdcall *)(LPVOID))GetProcAddress(snmpapi_dll, "SnmpUtilMemFree");
	if (SnmpExtensionInitPtr == NULL || SnmpExtensionQueryPtr == NULL || SnmpUtilMemAllocPtr == NULL || SnmpUtilMemFreePtr == NULL) {
		DebugString("Failed to get proc addresses for linked functions\n");
		FreeLibrary(snmpapi_dll);
		FreeLibrary(mib_ii_dll);
		return(false);
	}

	SnmpVarBindList * bind_list_ptr = (SnmpVarBindList *)SnmpUtilMemAllocPtr(sizeof(SnmpVarBindList));
	SnmpVarBind * bind_ptr = (SnmpVarBind *)SnmpUtilMemAllocPtr(sizeof(SnmpVarBind));

	DebugString("About to init INETMIB1.DLL\n");
	int ok = SnmpExtensionInitPtr(GetTickCount(), &trap_handle, &first_supported_region);

	if (!ok) {
		DebugString("Failed to init the .dll\n");
		SnmpUtilMemFreePtr(bind_list_ptr);
		SnmpUtilMemFreePtr(bind_ptr);
		FreeLibrary(snmpapi_dll);
		FreeLibrary(mib_ii_dll);
		return(false);
	}

	unsigned int mib_ii_name[] = {1,3,6,1,2,1,6,13,1,1};
	unsigned int * mib_ii_name_ptr = (unsigned int *)SnmpUtilMemAllocPtr(sizeof(mib_ii_name));
	memcpy(mib_ii_name_ptr, mib_ii_name, sizeof(mib_ii_name));

	conn_entry_type_index = ARRAY_SIZE(mib_ii_name) - 1;

	bind_ptr->name.idLength = ARRAY_SIZE(mib_ii_name);
	bind_ptr->name.ids = mib_ii_name;
	bind_list_ptr->list = bind_ptr;
	bind_list_ptr->len = 1;

	last_field = 1;
	index = 0;

	while (true) {

		if (!SnmpExtensionQueryPtr(SNMP_PDU_GETNEXT, bind_list_ptr, &error_status, &error_index)) {
			DebugString("SnmpExtensionQuery returned false\n");
			SnmpUtilMemFreePtr(bind_list_ptr);
			SnmpUtilMemFreePtr(bind_ptr);
			FreeLibrary(snmpapi_dll);
			FreeLibrary(mib_ii_dll);
			return(false);
		}

		if (bind_ptr->name.idLength < ARRAY_SIZE(mib_ii_name)) {
			break;
		}

		conn_entry_type = bind_ptr->name.ids[conn_entry_type_index];

		if (last_field != conn_entry_type) {
			index = 0;
			last_field = conn_entry_type;
		}

		switch (conn_entry_type) {

			case tcpConnState:
			{
				tConnInfoStruct * new_conn = new tConnInfoStruct;
				new_conn->State = bind_ptr->value.asnValue.number;
				connection_list.Add(new_conn);
				break;
			}

			case tcpConnLocalAddress:
				connection_list[index]->LocalIP = *((unsigned long *)bind_ptr->value.asnValue.address.stream);
				index++;
				break;

			case tcpConnLocalPort:
				connection_list[index]->LocalPort = (unsigned short)bind_ptr->value.asnValue.number;
				index++;
				break;

			case tcpConnRemAddress:
				connection_list[index]->RemoteIP = *((unsigned long *)bind_ptr->value.asnValue.address.stream);
				index++;
				break;

			case tcpConnRemPort:
				connection_list[index]->RemotePort = (unsigned short)bind_ptr->value.asnValue.number;
				index++;
				break;
		}
	}

	SnmpUtilMemFreePtr(bind_list_ptr);
	SnmpUtilMemFreePtr(bind_ptr);
	SnmpUtilMemFreePtr(mib_ii_name_ptr);

	DebugString("Got %d connections in list, parsing...\n", connection_list.Count());

	found = false;
	while (connection_list.Count()) {
		tConnInfoStruct * connection = connection_list[0];

		temp = ntohl(connection->RemoteIP);
		memcpy(remote_address, (unsigned char *)&temp, 4);

		if (!found && memcmp(remote_address, server_address, 4) == 0) {
			DebugString("Found connection with same remote address as server\n");

			if (server_port == 0 || server_port == (unsigned int)connection->RemotePort) {

				DebugString("Connection has same port\n");
				if (connection->State == ESTABLISHED) {
					DebugString("Connection is ESTABLISHED\n");
					memcpy(node, &connection->LocalIP, 4);
					address->Set_Address(net, node, connection->LocalPort);
					found = true;
				} else {
					DebugString("Connection is not ESTABLISHED - skipping\n");
				}
			} else {
				DebugString("Connection has different port. Port is %d, looking for %d\n", connection->RemotePort, server_port);
			}
		}

		delete connection_list[0];
		connection_list.Delete_Index(0);
	}

	if (found) {
		DebugString("Using address %s to talk to chat server\n", address->As_String(NULL));
	}

	FreeLibrary(snmpapi_dll);
	FreeLibrary(mib_ii_dll);

	return(found);
}

int __fastcall Negotiate_Scenario_Transfer(bool connected) // RA2: 0x00750330, its caller's debug text "Negotiate_Scenario_Transfer failed" names it (wonline.h's Sync_Scenario_With_Guests): the host waits for each guest's answer (ready, wants the scenario, don't go, has none), sends the scenario to those that asked and then GO or DONT_GO; a guest checks its copy of the scenario, answers the host, takes the file and waits for GO; 0 to go on, 1 when the start is called off, 2 when the scenarios do not match
{
	GlobalPacketType packet;
	bool dont_go = !connected;
	bool no_scenario = false;
	int result = 0;
	Ipx.Clear_Global_Routed();
	Ipx.Set_Timing(50, (unsigned long)-1, 5000, true);
	User * owner;
	if (Local_User_Is_Channel_Owner()) { // RA2: the host
		memset(&packet, 0, sizeof(packet));
		int responses[MAX_PLAYERS];
		memset(responses, 0, sizeof(responses));
		int num_responses = 0;
		bool send_scenario = false;
		DebugString("About to wait for 'GO' response.\n");
		CDTimerClass<SystemTimerClass> response_timer;
		response_timer = 900;
		if (dont_go) {
			response_timer = 300;
		}
		do {
			Call_Back();
			if (Ipx.Get_Global_Message(&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID) && Session.GProductID == IPXGlobalConnClass::COMMAND_AND_CONQUER3) {
				DebugString("Packet is RA2 packet\n");
				for (int i = 1; i < Session.Players.Count(); ++i) {
					if (Session.Players[i]->Address.Is_Same_Node(Session.GAddress) && !stricmp(Wide_To_Narrow(Session.Players[i]->Name), Session.GPacket.Name) && !responses[i]) {
						if (Session.GPacket.Command == NET_REQ_SCENARIO) {
							DebugString("Packet type is NET_REQ_SCENARIO\n");
							responses[i] = Session.GPacket.Command;
							send_scenario = true;
							++num_responses;
						}
						if (Session.GPacket.Command == NET_READY_TO_GO) {
							DebugString("Packet type is NET_READY_TO_GO\n");
							responses[i] = Session.GPacket.Command;
							++num_responses;
						}
						if (Session.GPacket.Command == 42) { // RA2: NET_DONT_GO
							DebugString("Packet type is NET_DONT_GO\n");
							responses[i] = Session.GPacket.Command;
							++num_responses;
							dont_go = true;
							if (i < Session.Players.Count() && g_ConnectionStatus[i] < g_ConnectionStatusRank[i]) { // RA2: Set_Connection_Status(i, 0) written out
								g_ConnectionStatus[i] = 0;
							}
						}
						if (Session.GPacket.Command == NET_NO_SCENARIO) {
							DebugString("Packet type is NET_NO_SCENARIO\n");
							responses[i] = Session.GPacket.Command;
							result = 2;
							dont_go = true;
							no_scenario = true;
							++num_responses;
						}
					}
				}
			}
		} while (num_responses < Session.Players.Count() - 1 && response_timer);
		if (no_scenario) {
			result = 2;
		}
		if (!dont_go && !no_scenario && send_scenario && num_responses == Session.Players.Count() - 1) { // RA2: the scenario goes to the guests that asked
			while (WS_Destroy_Dialog(0, 0) == 1) {
			}
			memset(Session.ScenarioRequests, 0, sizeof(Session.ScenarioRequests));
			Session.RequestCount = 0;
			for (int j = 1; j < Session.Players.Count(); ++j) {
				if (responses[j] == NET_REQ_SCENARIO) {
					Session.ScenarioRequests[Session.RequestCount++] = (char)j;
				}
			}
			if (!Send_Remote_File(Scen->ScenarioName, GAME_INTERNET, false, true)) {
				result = 2;
				dont_go = false;
			}
		}
		memset(&packet, 0, sizeof(packet));
		if (!dont_go && num_responses == Session.Players.Count() - 1 && !no_scenario) {
			packet.Command = NET_GO;
		} else {
			packet.Command = (NetCommandType)42; // RA2: NET_DONT_GO
			dont_go = true;
			if (!result) {
				result = 1;
			}
		}
		OwnerDraw::WideToCharString(packet.Name, Session.Players[0]->Name, -1);
		for (int k = 1; k < Session.Players.Count(); ++k) {
			CDTimerClass<SystemTimerClass> timer(600);
			if (dont_go) {
				timer = CDTimerClass<SystemTimerClass>(300);
			}
			do {
				Call_Back();
			} while (!Ipx.Send_Global_Message_To_Name(&packet, 0x2F, 1, Session.Players[k]->Name) && timer);
		}
	} else { // RA2: a guest
		if (Find_Local_Scenario(Session.ScenarioFileName, Session.ScenarioFileLength, Session.ScenarioDigest, Session.ScenarioIsOfficial, true)) {
			memset(&packet, 0, sizeof(packet));
			packet.Command = NET_READY_TO_GO;
			if (dont_go) {
				packet.Command = (NetCommandType)42;
			}
			OwnerDraw::WideToCharString(packet.Name, Session.Players[0]->Name, -1);
			owner = Find_Owner_User(); // RA2: expanded here
			if (owner) {
				Ipx.Send_Global_Message_To_Name(&packet, 0x2F, 1, Narrow_To_Wide((char *)owner->name));
			}
			CDTimerClass<SystemTimerClass> timer;
			timer = 1200;
			if (dont_go) {
				timer = 300;
			}
			while (Ipx.Global_Num_Send() > 0 && timer) {
				Call_Back();
			}
		} else {
			if ((Online_Session_State != 2 && Online_Session_State == 1) || (Session.ScenarioIsOfficial && !Session.Is_Random_Map())) {
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_NO_SCENARIO;
				owner = Find_Owner_User(); // RA2: expanded here
				if (owner) {
					Ipx.Send_Global_Message_To_Name(&packet, 0x2F, 1, Narrow_To_Wide((char *)owner->name));
				}
				DebugString("Sent NET_NO_SCENARIO\n");
				CDTimerClass<SystemTimerClass> timer;
				timer = 1200;
				while (Ipx.Global_Num_Send() > 0 && timer) {
					Call_Back();
				}
				result = 2;
				dont_go = true;
			} else {
				while (WS_Destroy_Dialog(0, 0) == 1) {
				}
				if (!Get_File_From_Host(Session.ScenarioFileName, GAME_INTERNET, true)) {
					result = 2;
					dont_go = true;
				}
			}
		}
		strcpy(Scen->ScenarioName, Session.ScenarioFileName);
		DebugString("About to wait for 'GO' from host.\n");
		CDTimerClass<SystemTimerClass> response_timer;
		response_timer = 1200;
		if (dont_go) {
			response_timer = 300;
		}
		do {
			Call_Back();
			if (Ipx.Get_Global_Message(&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID) && Session.GProductID == IPXGlobalConnClass::COMMAND_AND_CONQUER3) {
				DebugString("Packet is RA2 packet\n");
				owner = Find_Owner_User(); // RA2: expanded here
				if (owner && !stricmp(Session.GPacket.Name, (char *)owner->name)) {
					if (Session.GPacket.Command == NET_GO) {
						DebugString("Got NET_GO from host\n");
						goto done;
					}
					if (Session.GPacket.Command == 42) {
						DebugString("Got NET_DONT_GO from host\n");
						dont_go = true;
						for (int j = 0; j < Session.Players.Count(); ++j) {
							if (!stricmp(Wide_To_Narrow(Session.Players[j]->Name), (char *)g_OwnerUser.name)) {
								Set_Connection_Status(j, 3); // RA2: expanded here
								break;
							}
						}
						break;
					}
				}
			}
		} while (response_timer);
		if (!response_timer) {
			dont_go = true;
		}
	}
done:
	Ipx.Set_Timing(Ipx.Global_Response_Time() + 2, (unsigned long)-1, MAX(120U, Ipx.Global_Response_Time() * 8), true);
	Ipx.Set_Global_Routed();
	if (result != 2 && dont_go) {
		result = 1;
	}
	return(result);
}

bool Handle_Preview_Download(void) // RA2: 0x00750D10 (the archive's name, TibSun wonline.cpp:9830; ra2-decomp's YR twin 0x00796710): a guest of a game channel takes the map preview from the host when the host's NET_PREVIEW_MODE packet is waiting in the global channel; true when the preview was received
{
	IPXAddressClass address;
	if (g_CurrentChannel.type && g_UserList.length() >= 2) { // RA2: in a channel with at least one other user
		User * owner = Find_Owner_User(); // RA2: expanded here
		if (owner && !stricmp((char const *)owner->name, g_NickName.text())) { // RA2: the host does not receive its own preview
			return(false);
		}
		if (WolOptionsLocked == 0 && Ipx.Global_Num_Receive()) { // RA2: a packet waits
			Call_Back();
			GlobalPacketType packet;
			int length = sizeof(packet);
			unsigned short product;
			if (Ipx.Peek_Global_Message(0, &packet, &length, &address, &product, NULL) && packet.Command == NET_PREVIEW_MODE) {
				HWND dialog = WS_Find_Dialog(0xC9); // RA2: the game lobby dialog shows its preview control
				if (dialog) {
					ShowWindow(GetDlgItem(dialog, 0x468), SW_SHOWNORMAL);
				}
				User user;
				g_UserList.get(user, 0);
				user.next = NULL;
				User_List_To_Session_Players(&user); // RA2: the session addresses first
				if (Ipx.Get_Global_Message(&packet, &length, &address, &product) && packet.Command == NET_PREVIEW_MODE && address.Is_Same_Node(Session.HostAddress) && product == 0xAA03) { // RA2: from the host
					owner = Find_Owner_User(); // RA2: expanded here
					if (owner && !stricmp(packet.Name, (char const *)owner->name)) {
						DebugString("Received NET_PREVIEW_MODE packet from host\n");
						Ipx.Set_Timing(50, (unsigned long)-1, 5000, true); // RA2: slow resends while the preview comes
						g_ReceivingHostPreview = true;
						Receive_Random_Map_Preview();
						g_ReceivingHostPreview = false;
						Ipx.Set_Timing(30, (unsigned long)-1, 600, true); // RA2: the usual timing again
						return(true);
					}
				}
			}
		}
	}
	return(false);
}

static inline int Current_Channel_Type(void) // RA2: as in ra2-decomp's twin
{
	return(g_CurrentChannel.type);
}

void Poke_The_Host(void) // RA2: 0x00750F60, its own debug text names it: a guest of an internet game channel sends the host a NET_PING through the global channel so the host learns its address
{
	if (Session.Type != GAME_INTERNET) {
		IPXAddressClass address;
		if (g_UserList.length() >= 2) {
			if (Local_User_Is_Channel_Owner()) { // RA2: expanded here; the host does not poke itself
				return;
			}
			if (WolOptionsLocked == 0 && Current_Channel_Type() != 0) {
				User user;
				g_UserList.get(user, 0);
				user.next = NULL;
				User_List_To_Session_Players(&user);
				Call_Back();
				Ipx.Set_Timing(50, (unsigned long)-1, 5000, true);
				DebugString("Poke_The_Host - Host address is %s\n", Session.HostAddress.As_String());
				GlobalPacketType packet;
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_PING;
				OwnerDraw::WideToCharString(packet.Name, Session.Players[0]->Name, -1);
				User * owner = Find_Owner_User(); // RA2: expanded here
				if (owner) {
					Ipx.Send_Global_Message_To_Name(&packet, sizeof(packet), 1, Narrow_To_Wide((char *)owner->name));
				}
				Call_Back();
			}
		}
	}
}

void Init_WDT(void) // RA2: 0x00751120, no caller; an empty function where TibSun's wonline.cpp defines Init_WDT, right after Poke_The_Host and before the World Domination Tour dialogs (TibSun's Select_WDT_Location follows here too); RA2 builds no territory lobbies
{
}

BOOL CALLBACK Locale_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00751190, defined below

int Select_WDT_Location(void) // RA2: 0x00751130, no caller; TibSun's Select_WDT_Location (its country dialog) here: runs the country dialog (0xFE) and gives the chosen locale, 0 when cancelled (Connect_To_Server expands it)
{
	SimpleWonlineDialogControl control;
	int answer = OwnerDraw::Controlled_Dialog(0xFE, Locale_Dialog_Proc, &control);
	if (answer < 0) { // RA2: cancelled
		return(0);
	}
	return(answer);
}

BOOL CALLBACK Locale_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00751190, the country dialog (0xFE): fills the list with the chat server's locales, the chosen one goes to the control
{
	if (message == WM_INITDIALOG) { // RA2: the control comes with the dialog's creation data
		if (lparam) {
			SetWindowLongA(window, 8, ((long *)lparam)[1]);
		}
	}
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (!result) {
		SimpleWonlineDialogControl * control = (SimpleWonlineDialogControl *)GetWindowLongA(window, 8);
		switch (message) {
		case WM_NCDESTROY:
			On_WM_NCDESTROY(window);
			return(FALSE);
		case WM_PAINT:
			Dialog_On_WM_PAINT(window);
			Slide_Pending_Dialog(window);
			ValidateRect(window, NULL);
			return(FALSE);
		case WM_ERASEBKGND:
			return(TRUE);
		case WM_DRAWITEM:
			Dialog_On_WM_DRAWITEM(lparam);
			return(TRUE);
		case OD_SUBCLASSED: { // RA2: the dialog is subclassed: the list takes the locales, last first
			SendDlgItemMessageA(window, 0x3F0, LB_RESETCONTENT, 0, 0);
			int count = 0;
			g_pChat->GetLocaleCount(&count);
			if (count > 0) {
				HWND list = GetDlgItem(window, 0x3F0);
				for (int locale = count - 1; locale >= 0; --locale) {
					WideWstring name = Get_Locale_Name(locale);
					DebugString("Adding country: %hs\n", name.get());
					int row; // RA2: the first two go in at the top
					if (locale < 2) {
						row = SendMessageA(list, 0x4CC, 0, (LPARAM)name.get());
					} else {
						row = SendMessageA(list, 0x4CD, 0, (LPARAM)name.get());
					}
					SendMessageA(list, LB_SETITEMDATA, row, locale);
				}
				SendMessageA(list, LB_SETCURSEL, -1, 0);
			}
			break;
		}
		case WM_COMMAND:
			switch (LOWORD(wparam)) {
			case IDOK: { // RA2: the chosen locale, 0 when none is chosen
				HWND list = GetDlgItem(window, 0x3F0);
				int selected = 0;
				int row = SendMessageA(list, LB_GETCURSEL, 0, 0);
				if (row != -1) {
					selected = SendMessageA(list, LB_GETITEMDATA, row, 0);
				}
				control->Set_Window((unsigned)window, (HWND)selected); // RA2: the control takes the locale as its result
				break;
			}
			case IDCANCEL:
				control->Set_Window((unsigned)window, 0);
				return(FALSE);
			case 0x3F0:
				if (HIWORD(wparam) == LBN_SELCHANGE) { // RA2: OK is enabled while a row is chosen
					int row = SendMessageA((HWND)lparam, LB_GETCURSEL, 0, 0);
					EnableWindow(GetDlgItem(window, IDOK), row != -1);
					return(FALSE);
				}
				break;
			}
			break;
		}
		result = FALSE;
	}
	return(result);
}

void Add_Test_Player_Ol_Cruddy(void); // RA2: 0x007513E0, below (not exact yet)
void Add_Test_Player_Potato(void); // RA2: 0x007514D0, below (not exact yet)
void Add_Test_Player_Potato_Too(void); // RA2: 0x007515B0, below (not exact yet)

// Add_Test_Player_Ol_Cruddy, Add_Test_Player_Potato and Add_Test_Player_Potato_Too: text adopted from mischa85/RedAlert2 src/wonline.cpp
void Add_Test_Player_Ol_Cruddy(void) // RA2: 0x007513E0, not in TibSun: a test player "Ol Cruddy" at 10.2.81.82 is added to the session (house 0, color 3), as a stand-in for a real opponent; called by Start_Test_Network_Game
{
	NodeNameType * player = new NodeNameType;
	Char_To_Wide(player->Name, "Ol Cruddy", -1);
	player->Player.SquadID = -1;
	player->Player.House = 0;
	player->Player.Color = 3;
	NetNumType network;
	NetNodeType node;
	memset(network, 0, sizeof(network));
	node[5] = 0; // RA2: the address 10.2.81.82
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

void Add_Test_Player_Potato(void) // RA2: 0x007514D0, not in TibSun: a test player "Potato" at 10.2.20.97 is added to the session (house 0, color 0); called by Start_Test_Network_Game
{
	NodeNameType * player = new NodeNameType;
	Char_To_Wide(player->Name, "Potato", -1);
	player->Player.SquadID = -1;
	player->Player.House = 0;
	player->Player.Color = 0;
	NetNumType network;
	NetNodeType node;
	memset(network, 0, sizeof(network));
	node[5] = 0; // RA2: the address 10.2.20.97
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

void Add_Test_Player_Potato_Too(void) // RA2: 0x007515B0, not in TibSun: a test player "PotatoToo" at 10.2.80.250 is added to the session as the host (house 1, color 2); called by Start_Test_Network_Game
{
	NodeNameType * player = new NodeNameType;
	Char_To_Wide(player->Name, "PotatoToo", -1);
	player->Player.SquadID = -1;
	player->Player.House = 1;
	player->Player.Color = 2;
	NetNumType network;
	NetNodeType node;
	memset(network, 0, sizeof(network));
	node[5] = 0; // RA2: the address 10.2.80.250
	node[4] = 0;
	node[3] = 250;
	node[2] = 80;
	node[1] = 2;
	node[0] = 10;
	player->Address.Set_Address(network, node, 0);
	Session.Players.Add(player);
	PlayerPings[Session.NumPlayers] = 100;
	++Session.NumPlayers;
	Session.HostAddress = player->Address; // RA2: the host
}

void Start_Test_Network_Game(void) // RA2: 0x007516C0, not in TibSun, no caller: starts a three player test game against the fixed test players without a server (the test machines STEVE-T3 and STEVE-T2 add them in their own order; STEVE-T3 sets the frame rate)
{
	GameID = 0;
	GameProduct = g_GameSKU;
	WestwoodOnline_Tournament = 0;
	strcpy(PlayerNickname, g_NickName.get());
	Clear_Vector(&Session.Players);
	char computer_name[1015]; // RA2: GetComputerNameA is given 0x3F7
	DWORD computer_name_size = sizeof(computer_name);
	GetComputerNameA(computer_name, &computer_name_size);
	bool precalculate = false;
	if (!_stricmp(computer_name, "STEVE-T3")) {
		Add_Test_Player_Potato_Too();
		Add_Test_Player_Ol_Cruddy();
		Add_Test_Player_Potato();
		precalculate = true;
	} else {
		if (!_stricmp(computer_name, "STEVE-T2")) {
			Add_Test_Player_Potato();
			Add_Test_Player_Ol_Cruddy();
		} else {
			Add_Test_Player_Ol_Cruddy();
			Add_Test_Player_Potato();
		}
		Add_Test_Player_Potato_Too();
	}
	PacketTransport->Clear_Broadcast_Addresses();
	char address[32] = "xxx.xxx.xxx.xxx";
	NetNumType network;
	NetNodeType node;
	int i;
	for (i = 1; i < Session.Players.Count(); ++i) {
		Session.Players[i]->Address.Get_Address(network, node);
		PacketTransport->Set_Broadcast_Address(address, 0);
	}
	WolOptionsLocked = 2;
	DebugString("Starting a %d player game.\n", Session.NumPlayers);
	Char_To_Wide(Session.MasterPlayerName, "PotatoToo", -1);
	Session.MasterPlayerID = -1;
	Session.House = 0;
	Seed = 1;
	BuildLevel = 10;
	Session.Options.UnitCount = 10;
	Session.Options.Credits = 10000;
	Session.Options.FogOfWar = false;
	Session.Options.BridgeDestruction = true;
	Session.Options.Goodies = true;
	Session.Options.MCVRedeploy = true;
	Session.Options.AlliesAllowed = true;
	Session.Options.HarvTruce = false;
	Session.Options.Bases = true;
	Session.Options.CTF = false;
	Session.Options.AIPlayers = 0;
	Session.Options.AIDifficulty = DIFF_EASY;
	Session.Options.ShortGame = true;
	Session.Options.SWAllowed = true;
	Session.Options.BuildOffAlly = false;
	Session.Options.CrapEngineers = true;
	Session.Options.ScenarioIndex = 1;
	Session.Options.GameSpeed = 0;
	Set_Scenario_Info_From_Index(1);
	Session.Options.GameSpeed = 2;
	Options.GameSpeed = 2;
	Scen->Scenario = Session.Options.ScenarioIndex;
	Session.FrameSendRate = 5;
	switch (Session.Options.GameSpeed) { // RA2: the game speed sets how far ahead and how fast
		case 0: Session.MaxAhead = 40; Session.PrecalcDesiredFrameRate = 60; Session.FrameSendRate = 10; break;
		case 1: Session.MaxAhead = 40; Session.PrecalcDesiredFrameRate = 45; Session.FrameSendRate = 10; break;
		case 2: Session.MaxAhead = 30; Session.PrecalcDesiredFrameRate = 30; break;
		case 3: Session.MaxAhead = 20; Session.PrecalcDesiredFrameRate = 20; break;
		case 4: Session.MaxAhead = 20; Session.PrecalcDesiredFrameRate = 15; break;
		case 5: Session.MaxAhead = 20; Session.PrecalcDesiredFrameRate = 12; break;
		default: Session.MaxAhead = 10; Session.PrecalcDesiredFrameRate = 10; break;
	}
	Session.PrecalcMaxAhead = 0;
	if (!precalculate) { // RA2: only STEVE-T3 sets the frame rate
		Session.PrecalcDesiredFrameRate = 0;
	}
	int worst_ping = 0;
	for (i = 0; i < Session.Players.Count(); ++i) {
		if (PlayerPings[i] > worst_ping) {
			worst_ping = PlayerPings[i];
		}
	}
	Session.LatencyFudge = 1;
	if (worst_ping == 0 || worst_ping == 1000) {
		Session.PrecalcDesiredFrameRate = 0;
		DebugString("Ping times not available\n");
	} else {
		DebugString("Max ping is %d ms\n", worst_ping);
		if (worst_ping > 1000) {
			Session.LatencyFudge = 2;
		} else if (worst_ping < 600) {
			Session.LatencyFudge = 1;
		}
		int timing = 60 * worst_ping / 1000;
		timing += 120 * timing / 100;
		Ipx.Set_Timing(timing, (unsigned long)-1, timing * 8, true);
		if (precalculate) {
			Ipx.Set_Lobby_Timing(5, (unsigned long)-1, 120);
			switch (Session.LatencyFudge) {
				case 0: case 1: Session.PrecalcMaxAhead = Session.MaxAhead; break;
				case 2: Session.PrecalcMaxAhead = (Session.MaxAhead >> 1) + Session.MaxAhead; break;
				case 3: Session.PrecalcMaxAhead = Session.MaxAhead * 2; break;
			}
			Session.PrecalcMaxAhead = ((unsigned int)Session.PrecalcMaxAhead / 10) * 10;
		}
	}
	DebugString("PrecalcMaxAhead is %d\n", Session.PrecalcMaxAhead);
	DebugString("PrecalcDesiredFrameRate is %d\n", Session.PrecalcDesiredFrameRate);
	DebugString("LatencyFudge is %d\n", Session.LatencyFudge);
	DebugString("FrameSendRate is %d\n", Session.FrameSendRate);
	Session.MaxMaxAhead = Session.MaxAhead;
	memset(Session.ConnectionStats, 0, sizeof(Session.ConnectionStats));
	UDPInterfaceClass::UDPListenPort = 1234; // RA2: the ports go back to their defaults
	Options.GameSpeed = Session.Options.GameSpeed;
	AlternateSendPort = 1235;
	AlternateListenPort = 1236;
	LoopbackPort = 1237;
	Frame = 0;
	Squad1Names[0] = 0;
	Squad2Names[0] = 0;
	PregameSetup();
}

void Request_Server_List_With_Wait(void) // RA2: 0x00751BD0, no caller: shows "fetching the server list" and requests the list for the product (0x2300 in a tournament session, else 0x2100); Login_Wonline expands its own copy (Fetch_Server_List above)
{
	{
		WideWstring fetching(StringTable::LoadString("TXT_FETCHING_SERVLIST", NULL, "D:\\Ra2\\wonline.cpp", 22220));
		Show_Wait_Window(16, fetching, false);
	}
	int sku = 0x2300;
	if (Online_Session_State != 2) {
		sku = 0x2100;
	}
	Request_And_Wait_Server_List(sku);
}


// Request_And_Wait_Server_List and Request_WDT_Cycle: the shape of ra2-decomp's twins (its src/wonline.cpp, GAMEMD 0x00797640 and 0x00797800), with RA2's values
int __fastcall Request_And_Wait_Server_List(unsigned int flags) // RA2: 0x00751C30, asks for the list of chat servers and waits for it and for the product's answer: 0 done, 1 failed, 2 quit
{
	bool ignoring = g_IgnoreNetStatus; // RA2: the net status is only recorded while the list comes
	g_IgnoreNetStatus = true;
	g_Servers.setEmpty();
	g_CurrentServerIndex = 0;
	g_TargetServerIndex = 0;
	g_pChat->RequestServerList(g_LanguageCode | flags, 0x10006, "TibSun", "TibPass99", 30);
	g_pChat->SetProductSKU(g_GameSKU);
	bool listed = false; // RA2: the server list came
	bool accepted = false; // RA2: the product was accepted (status 0x4012F)
	DWORD event;
	int result;
	for (;;) {
		while ((event = WaitForMultipleObjects(18, g_WaitEventHandles, FALSE, 50)) == WAIT_TIMEOUT) {
			WOL_Wait_Callback();
		}
		ResetEvent(g_WaitEventHandles[event]);
		if (event == EV_ABORT) {
			result = 1;
			break;
		}
		if (event == EV_QUITGAME) {
			result = 2;
			break;
		}
		if (event == EV_GOT_SERVERS) {
			listed = true;
			if (accepted) {
				result = 0;
				break;
			}
		} else if (event == EV_NETIGNORE) {
			bool failed = false;
			bool done = false;
			switch (g_LastIgnoredStatus) {
			case 0x80040067:
			case 0x80040068:
			case 0x80040069:
			case 0x8004006A:
			case 0x8004006C:
			case 0x8004006D:
					ODMessageBox(StringTable::LoadString("TXT_CANT_CONNECT", NULL, "D:\\Ra2\\wonline.cpp", 22329), 0, Wait_Callback_Thunk, false);
					failed = true;
				break;
			case 0x4012F:
					accepted = true;
					if (listed) {
						done = true;
				}
				break;
			}
			if (failed) {
				result = 1;
				break;
			}
			if (done) {
				result = 0;
				break;
			}
		}
	}
	Close_Wait_Window(~0U);
	g_IgnoreNetStatus = ignoring;
	return(result);
}

bool Request_WDT_Cycle(void) // RA2: 0x00751DF0, TibSun's Request_WDT_Cycle: asks the tour server for the state of the current cycle (giving up after 30 seconds), then for the previous cycle when the server says so; true when the state came
{
	if (g_WDTServer.conndata == NULL) {
		ODMessageBox(StringTable::LoadString("TXT_WDT_NET_ERR", NULL, "D:\\Ra2\\wonline.cpp", 22384), 0, Wait_Callback_Thunk, false);
		return(false);
	}
	Reset_Wait_Events();
	HRESULT res = g_pNetUtil->RequestWDTState(g_WDTHost, g_WDTPort, 3);
	DebugString("WDTHost is %s, port is %d\n", g_WDTHost, g_WDTPort);
	DebugString("Requesting current WDT cycle\n");
	if (res != S_OK) {
		ODMessageBox(StringTable::LoadString("TXT_WDT_NET_ERR", NULL, "D:\\Ra2\\wonline.cpp", 22400), 0, Wait_Callback_Thunk, false);
		return(false);
	}
	{
		WideWstring fetching(StringTable::LoadString("TXT_FETCHING_WDT", NULL, "D:\\Ra2\\wonline.cpp", 22404));
		Show_Wait_Window(16, fetching, false);
	}
	DWORD event;
	for (;;) {
		int timeout = 30000;
		event = WaitForMultipleObjects(18, g_WaitEventHandles, FALSE, 100);
		while (event == WAIT_TIMEOUT) {
			timeout -= 100;
			if (timeout <= 0) {
				goto failed;
			}
			WOL_Wait_Callback();
			event = WaitForMultipleObjects(18, g_WaitEventHandles, FALSE, 100);
		}
		ResetEvent(g_WaitEventHandles[event]);
		if (event == EV_ABORT) {
			Close_Wait_Window(~0U);
			return(false);
		}
		if (event == EV_ERROR) {
			ODMessageBox(StringTable::LoadString("TXT_WDT_NET_ERR", NULL, "D:\\Ra2\\wonline.cpp", 22423), 0, Wait_Callback_Thunk, false);
			Close_Wait_Window(~0U);
			return(false);
		}
		if (event == EV_COUNT) { // RA2: the timeout
failed:
	ODMessageBox(StringTable::LoadString("TXT_WDT_NET_ERR", NULL, "D:\\Ra2\\wonline.cpp", 22429), 0, Wait_Callback_Thunk, false);
	Close_Wait_Window(~0U);
			return(false);
		}
		if (event == EV_11) { // RA2: the state is stale: the previous cycle is asked for
			break;
		}
	}
	res = g_pNetUtil->RequestWDTState(g_WDTHost, g_WDTPort, 5);
	DebugString("Requesting Previous Cycle\n");
	if (res != S_OK) {
		Close_Wait_Window(~0U);
		return(false);
	}
	for (;;) {
		event = WaitForMultipleObjects(18, g_WaitEventHandles, FALSE, 100);
		while (event == WAIT_TIMEOUT) {
			WOL_Wait_Callback();
			event = WaitForMultipleObjects(18, g_WaitEventHandles, FALSE, 100);
		}
		ResetEvent(g_WaitEventHandles[event]);
		if (event == EV_ABORT) {
			Close_Wait_Window(~0U);
			return(false);
		}
		if (event == EV_ERROR) {
			ODMessageBox(StringTable::LoadString("TXT_WDT_NET_ERR", NULL, "D:\\Ra2\\wonline.cpp", 22456), 0, Wait_Callback_Thunk, false);
			Close_Wait_Window(~0U);
			return(false);
		}
		if (event == EV_11) {
			break;
		}
	}
	Close_Wait_Window(~0U);
	return(true);
}

void Initialize_WDT_Sounds(void) // RA2: 0x00752210, TibSun's Init_WDT_Sounds: fills the lists of the tour's sounds, once
{
	if (!g_WDTSoundsInitialized) {
		static Wstring _sound_names[16];

		_sound_names[0].set("01-W024.V01");
		_sound_names[1].set("01-W026.V01");
		_sound_names[2].set("01-W028.V01");
		_sound_names[3].set("01-W018.V01");
		_sound_names[4].set("01-W020.V01");
		_sound_names[5].set("01-W022.V01");
		_sound_names[6].set("00-W022.V00");
		_sound_names[7].set("00-W024.V00");
		_sound_names[8].set("00-W026.V00");
		_sound_names[9].set("00-W017.V00");
		_sound_names[10].set("00-W018.V00");
		_sound_names[11].set("00-W020.V00");
		_sound_names[12].set("01-W032.V01");
		_sound_names[13].set("01-W030.V01");
		_sound_names[14].set("00-W031.V00");
		_sound_names[15].set("00-W029.V00");

		g_WDT_NodWinEmphasisSounds.clear();
		g_WDT_GDIWinEmphasisSounds.clear();
		g_WDT_NodLoseEmphasisSounds.clear();
		g_WDT_GDILoseEmphasisSounds.clear();
		g_WDT_NodTerritoryWinSounds.clear();
		g_WDT_GDITerritoryWinSounds.clear();
		g_WDT_NodTerritoryLoseSounds.clear();
		g_WDT_GDITerritoryLoseSounds.clear();

		g_WDT_NodWinEmphasisSounds.addHead(_sound_names[0]);
		g_WDT_NodWinEmphasisSounds.addHead(_sound_names[1]);
		g_WDT_NodWinEmphasisSounds.addHead(_sound_names[2]);
		g_WDT_NodLoseEmphasisSounds.addHead(_sound_names[3]);
		g_WDT_NodLoseEmphasisSounds.addHead(_sound_names[4]);
		g_WDT_NodLoseEmphasisSounds.addHead(_sound_names[5]);
		g_WDT_GDIWinEmphasisSounds.addHead(_sound_names[6]);
		g_WDT_GDIWinEmphasisSounds.addHead(_sound_names[7]);
		g_WDT_GDIWinEmphasisSounds.addHead(_sound_names[8]);
		g_WDT_GDILoseEmphasisSounds.addHead(_sound_names[9]);
		g_WDT_GDILoseEmphasisSounds.addHead(_sound_names[10]);
		g_WDT_GDILoseEmphasisSounds.addHead(_sound_names[11]);
		g_WDT_NodTerritoryWinSounds.addHead(_sound_names[12]);
		g_WDT_NodTerritoryLoseSounds.addHead(_sound_names[13]);
		g_WDT_GDITerritoryWinSounds.addHead(_sound_names[14]);
		g_WDT_GDITerritoryLoseSounds.addHead(_sound_names[15]);

		g_WDTSoundsInitialized = true;
	}
}

void Deinitialize_WDT_Sounds(void) // RA2: 0x00752730, TibSun's Deinit_WDT_Sounds, no caller: empties the lists of the tour's sounds
{
	g_WDT_NodWinEmphasisSounds.clear();
	g_WDT_GDIWinEmphasisSounds.clear();
	g_WDT_NodLoseEmphasisSounds.clear();
	g_WDT_GDILoseEmphasisSounds.clear();
	g_WDT_NodTerritoryWinSounds.clear();
	g_WDT_GDITerritoryWinSounds.clear();
	g_WDT_NodTerritoryLoseSounds.clear();
	g_WDT_GDITerritoryLoseSounds.clear();
	g_WDTSoundsInitialized = false;
}

void Discard_WDT_Sound_List(ArrayList<Wstring> sounds) // RA2: 0x00752890, TibSun's Play_WDT_Sound with its body gone: takes a list of sounds by value and does nothing with it
{
}

DWORD __fastcall Wait_Wonline_Events(int seconds, DWORD count) // RA2: 0x007528C0, no caller: waits for one of the first count wait events, giving up after the seconds (EV_COUNT); the event is reset
{
	int remaining = seconds * 1000;
	DWORD event = WaitForMultipleObjects(count, g_WaitEventHandles, FALSE, 100);
	while (event == WAIT_TIMEOUT) {
		remaining -= 100;
		if (remaining <= 0) {
			return(EV_COUNT);
		}
		WOL_Wait_Callback();
		event = WaitForMultipleObjects(count, g_WaitEventHandles, FALSE, 100);
	}
	ResetEvent(g_WaitEventHandles[event]);
	return(event);
}

void Reset_Wait_Events(void) // RA2: 0x00752930, clears every wait event (ra2-decomp's twin, Yuri's Revenge 0x00798340); Queue_Channel_List_Request and the earlier rows expand it
{
	for (int i = 0; i < EV_COUNT; i++) {
		ResetEvent(g_WaitEventHandles[i]);
	}
}

namespace OwnerDraw { void __fastcall Set_Control_Image(HWND control, Surface * image); } // RA2: 0x005E1F30, defined in ownrdraw.cpp
int __fastcall Clear_ComboBox(HWND combo); // RA2: 0x006007F0, defined in ownrdraw.cpp

LRESULT CALLBACK Wonline_Mode_Menu_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00752950, the dialog proc of the menu of online session kinds (0x11C) when the profiles are not kept per persona
{
	if (message == WM_INITDIALOG && lparam) { // RA2: the control comes with the dialog's creation data
		SetWindowLongA(window, 8, *(long *)(lparam + 4));
	}
	long result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) {
		return(result);
	}
	OwnerDraw::DialogControl * control = (OwnerDraw::DialogControl *)GetWindowLongA(window, 8);
	switch (message) {
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(1);
	case WM_ERASEBKGND:
		return(1);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(1);
	case WM_COMMAND:
		switch (wparam & 0xFFFF) {
		case 0x6E0: // RA2: the session kinds the buttons pick
			Online_Session_State = 1;
			control->Set_Window((unsigned)window, (HWND)0x6E0);
			return(0);
		case 0x6E1:
			Online_Session_State = 3;
			control->Set_Window((unsigned)window, (HWND)0x6E1);
			return(0);
		case 0x6E2:
			Online_Session_State = 4;
			Online_Session_Return = 0;
			control->Set_Window((unsigned)window, (HWND)0x6E2);
			return(0);
		case 0x6E3:
			Online_Session_State = 2;
			control->Set_Window((unsigned)window, (HWND)0x6E3);
			return(0);
		case 0x686: // RA2: back
			Online_Session_State = 0;
			control->Set_Window((unsigned)window, (HWND)0x686);
			break;
		}
		break;
	case OD_SUBCLASSED: { // RA2: the dialog is subclassed: the legend's pictures
		HWND image_control = GetDlgItem(window, 0x746);
		Surface * image = SurfaceCache.GetSurface("gt18.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x747);
		image = SurfaceCache.GetSurface("woltrny.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x748);
		image = SurfaceCache.GetSurface("wolclan.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x749);
		image = SurfaceCache.GetSurface("wolreslk.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x74A);
		image = SurfaceCache.GetSurface("wolob.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x752);
		image = SurfaceCache.GetSurface("wolpriv.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x75C);
		image = SurfaceCache.GetSurface("pingg.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x75D);
		image = SurfaceCache.GetSurface("pingy.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x75E);
		image = SurfaceCache.GetSurface("pingr.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		break;
	}
	}
	return(0);
}

LRESULT CALLBACK Wonline_Profile_Mode_Menu_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00752CF0, the dialog proc of the menu of online session kinds with the persona's profile (0x10E)
{
	if (message == WM_INITDIALOG && lparam) { // RA2: the control comes with the dialog's creation data
		SetWindowLongA(window, 8, *(long *)(lparam + 4));
	}
	long result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) {
		return(result);
	}
	OwnerDraw::DialogControl * control = (OwnerDraw::DialogControl *)GetWindowLongA(window, 8);
	switch (message) {
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(1);
	case WM_ERASEBKGND:
		return(1);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(1);
	case WM_COMMAND:
		switch (wparam & 0xFFFF) {
		case 0x6E0: // RA2: the session kinds the buttons pick
			Online_Session_State = 1;
			control->Set_Window((unsigned)window, (HWND)0x6E0);
			return(0);
		case 0x6E1:
			Online_Session_State = 3;
			control->Set_Window((unsigned)window, (HWND)0x6E1);
			return(0);
		case 0x6E2:
			Online_Session_State = 4;
			Online_Session_Return = 0;
			control->Set_Window((unsigned)window, (HWND)0x6E2);
			return(0);
		case 0x6E3:
			Online_Session_State = 2;
			control->Set_Window((unsigned)window, (HWND)0x6E3);
			return(0);
		case 0x6E4:
			Online_Session_State = 5;
			control->Set_Window((unsigned)window, (HWND)0x6E4);
			return(0);
		case 0x686: // RA2: back
			Online_Session_State = 0;
			control->Set_Window((unsigned)window, (HWND)0x686);
			break;
		}
		break;
	case OD_SUBCLASSED: { // RA2: the dialog is subclassed: the legend's pictures
		if (g_NickName.length() > 0) { // RA2: the persona's profile fills the dialog
			rc_ptr<PlayerProfile> profile = PlayerProfile::Load(g_NickName);
			if (profile.Get()) {
				profile->PopulateDialog(window, g_NickName, true);
			}
		}
		HWND image_control = GetDlgItem(window, 0x746);
		Surface * image = SurfaceCache.GetSurface("gt18.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x747);
		image = SurfaceCache.GetSurface("woltrny.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x748);
		image = SurfaceCache.GetSurface("wolclan.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x749);
		image = SurfaceCache.GetSurface("wolreslk.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x74A);
		image = SurfaceCache.GetSurface("wolob.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x752);
		image = SurfaceCache.GetSurface("wolpriv.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x75C);
		image = SurfaceCache.GetSurface("pingg.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x75D);
		image = SurfaceCache.GetSurface("pingy.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		image_control = GetDlgItem(window, 0x75E);
		image = SurfaceCache.GetSurface("pingr.pcx", NULL);
		if (image_control && image) {
			OwnerDraw::Set_Control_Image(image_control, image);
		}
		break;
	}
	}
	return(0);
}

int Run_Wonline_Selection_Dialog(void) // RA2: 0x00753110, runs the menu of online session kinds (0x10E with the persona's profile, else 0x11C); true when a kind was chosen
{
	WOLOptionsClass options;
	options.Load();
	Darken_Surface(NULL, true);
	if (g_pNetUtil && strlen(g_LadderAddress) && g_NickName.length() > 0) {
		g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, g_NickName.get(), g_GameSKU & 0xFFFF00, -1, 0, 0); // RA2: virtual 0x14
	}
	if (options.Is_Bit1_Clear()) {
		SimpleWonlineDialogControl control;
		OwnerDraw::Controlled_Dialog(0x10E, (DLGPROC)Wonline_Profile_Mode_Menu_Dialog_Proc, &control);
		return(Online_Session_State != 0);
	}
	SimpleWonlineDialogControl control;
	OwnerDraw::Controlled_Dialog(0x11C, (DLGPROC)Wonline_Mode_Menu_Dialog_Proc, &control);
	return(Online_Session_State != 0);
}

void Fill_Locale_Combo(HWND combo) // RA2: 0x00753240, fills the country box of the login dialog with the chat server's locales (the first two at the top) and selects the persona's
{
	Clear_ComboBox(combo);
	int count = 0;
	g_pChat->GetLocaleCount(&count);
	WOLOptionsClass options;
	options.Load();
	rc_ptr<PlayerProfile> profile;
	if (options.Is_Bit1_Clear()) {
		profile = PlayerProfile::Load(g_NickName); // RA2: the persona's own profile
	} else {
		profile = g_PlayerProfile.Get(); // RA2: the profile of Reset_WOL_Globals
	}
	if (count > 0) {
		for (int locale = count - 1; locale >= 0; --locale) {
			WideWstring name = Get_Locale_Name(locale);
			int item;
			if (locale < 2) {
				item = SendMessageA(combo, 0x4C1, 0, (LPARAM)name.get());
			} else {
				item = SendMessageA(combo, 0x4C2, 0, (LPARAM)name.get());
			}
			SendMessageA(combo, CB_SETITEMDATA, item, locale);
			if (profile.Get() && locale == profile->Locale) {
				SendMessageA(combo, CB_SETCURSEL, item, 0);
			}
		}
	}
}

LRESULT __fastcall Get_Combo_Item_Wide_Text(HWND combo, WPARAM index, WideWstring & text); // RA2: 0x00600FD0, the wide text of a combo box item, defined in ownrdraw.cpp

LRESULT CALLBACK Wonline_Profile_Login_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00753370, the dialog proc of the persona dialog (0x114): picks one of the stored personas and keeps its profile (server, country, side, color) and the auto login
{
	if (message == WM_INITDIALOG && lparam) { // RA2: the control comes with the dialog's creation data
		SetWindowLongA(window, 8, *(long *)(lparam + 4));
	}
	long result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) {
		return(result);
	}
	OwnerDraw::DialogControl * control = (OwnerDraw::DialogControl *)GetWindowLongA(window, 8);
	switch (message) {
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		// RA2: falls through to the erase-background result
	case WM_ERASEBKGND:
		return(1);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(1);
	case WM_COMMAND:
		switch (wparam & 0xFFFF) {
		case 0x687: // RA2: log in with the persona
			Online_Session_State = 4;
			Online_Session_Return = 5;
			control->Set_Window((unsigned)window, (HWND)0x687);
			return(0);
		case 0x688: // RA2: the tournament pages
			ViewHTML("http://www.westwood.com/westwoodonline/tournaments/redalert2/", 0);
			return(0);
		case 0x686: // RA2: back
			Online_Session_State = 0;
			control->Set_Window((unsigned)window, (HWND)0x686);
			return(0);
		case 0x628: // RA2: the persona box
			switch (HIWORD(wparam)) {
			case CBN_SELCHANGE: {
				HWND automatic = GetDlgItem(window, 0x6FF); // RA2: the auto login check box
				SendMessageA(automatic, 0x49D, 0, 1);
				ShowWindow(automatic, SW_HIDE);
				int selected = SendMessageA((HWND)lparam, CB_GETCURSEL, 0, 0);
				int nick_index = -1;
				if (selected != -1) {
					nick_index = SendMessageA((HWND)lparam, CB_GETITEMDATA, selected, 0);
				}
				g_NickName.clear();
				if (nick_index != -1) {
					char const * nickname = NULL;
					char const * password = NULL;
					if (g_pChat->GetNick(nick_index, &nickname, &password) == 0) {
						g_NickName = nickname;
					}
				}
				rc_ptr<PlayerProfile> profile = PlayerProfile::Load(g_NickName);
				Wstring automatic_persona = WOL_Auto_Login_Persona();
				EnableWindow(automatic, nick_index != -1);
				SendMessageA(automatic, BM_SETCHECK, (automatic_persona == g_NickName) ? 1 : 0, 0);
				SendMessageA(automatic, 0x49D, 0, 0);
				ShowWindow(automatic, SW_SHOW);
				InvalidateRect(automatic, NULL, FALSE);
				if (profile.Get()) {
					profile->PopulateDialog(window, g_NickName, false);
				}
				EnableWindow(GetDlgItem(window, 0x6EB), nick_index != -1 && g_NickName.length() > 0);
				EnableWindow(GetDlgItem(window, 0x687), nick_index != -1 && g_NickName.length() > 0);
				break;
			}
			}
			break;
		case 0x629: // RA2: the server box
			switch (HIWORD(wparam)) {
			case CBN_SELCHANGE: {
				Wstring server;
				Get_Window_Text((HWND)lparam, server);
				rc_ptr<PlayerProfile> profile = PlayerProfile::Load(g_NickName);
				if (profile.Get()) {
					profile->SetName(&server);
					profile->Save(&g_NickName);
				}
				break;
			}
			}
			break;
		case 0x62A: // RA2: the country box
			switch (HIWORD(wparam)) {
			case CBN_SELCHANGE: {
				int locale = 0;
				int selected = SendMessageA((HWND)lparam, CB_GETCURSEL, 0, 0);
				if (selected != -1) {
					locale = SendMessageA((HWND)lparam, CB_GETITEMDATA, selected, 0);
				}
				rc_ptr<PlayerProfile> profile = PlayerProfile::Load(g_NickName);
				if (profile.Get()) {
					profile->SetLocale(locale, 3);
					profile->Save(&g_NickName);
				}
				break;
			}
			}
			break;
		case 0x6A1: // RA2: the side box
			switch (HIWORD(wparam)) {
			case CBN_SELCHANGE: {
				int side = -2;
				int selected = SendMessageA((HWND)lparam, CB_GETCURSEL, 0, 0);
				if (selected != -1) {
					side = SendMessageA((HWND)lparam, CB_GETITEMDATA, selected, 0);
				}
				rc_ptr<PlayerProfile> profile = PlayerProfile::Load(g_NickName);
				if (profile.Get()) {
					profile->SetCountry(side);
					profile->Save(&g_NickName);
				}
				break;
			}
			}
			break;
		case 0x6A2: // RA2: the color box
			switch (HIWORD(wparam)) {
			case CBN_SELCHANGE: {
				int color = -2;
				int selected = SendMessageA((HWND)lparam, CB_GETCURSEL, 0, 0);
				if (selected != -1) {
					color = SendMessageA((HWND)lparam, CB_GETITEMDATA, selected, 0);
				}
				rc_ptr<PlayerProfile> profile = PlayerProfile::Load(g_NickName);
				if (profile.Get()) {
					profile->SetColor(color);
					profile->Save(&g_NickName);
				}
				break;
			}
			}
			break;
		case 0x6FF: // RA2: the auto login check box
			if (SendMessageA((HWND)lparam, BM_GETCHECK, 0, 0) == 1) {
				WOL_Set_Auto_Login_Persona(g_NickName);
			} else {
				Wstring const automatic = WOL_Auto_Login_Persona();
				if (automatic.toLower() == static_cast<Wstring const &>(g_NickName).toLower()) {
					WOL_Set_Auto_Login_Persona("");
				}
			}
			break;
		case 0x6EA: // RA2: a new account
			Create_Wonline_Account(window);
			return(0);
		case 0x6EB: { // RA2: the persona is forgotten
			HWND nicknames = GetDlgItem(window, 0x628);
			int selected = SendMessageA(nicknames, CB_GETCURSEL, 0, 0);
			if (selected != -1) {
				WideWstring wide;
				Get_Combo_Item_Wide_Text(nicknames, selected, wide);
				Wstring nickname = Wstring_Narrow(wide);
				nickname.toLower();
				Wstring const automatic = WOL_Auto_Login_Persona();
				if (nickname == automatic.toLower()) {
					WOL_Set_Auto_Login_Persona("");
				}
				Wstring const last = WOL_Last_Valid_Persona();
				if (nickname == last.toLower()) {
					WOL_Set_Last_Valid_Persona("");
				}
				int nick_index = SendMessageA(nicknames, CB_GETITEMDATA, selected, 0);
				if (nick_index != -1) {
					g_pChat->SetNick(nick_index, "", "", 1);
					SendMessageA(nicknames, CB_DELETESTRING, selected, 0);
				}
				SendMessageA(nicknames, CB_SETCURSEL, -1, 0);
			}
			WOL_Unknown_Stats_Text(window, true);
			InvalidateRect(nicknames, NULL, FALSE);
			EnableWindow(GetDlgItem(window, 0x6EB), FALSE);
			SendMessageA(nicknames, CB_SETCURSEL, 0, 0);
			rc_ptr<PlayerProfile> profile = PlayerProfile::Load_Default();
			if (profile.Get()) {
				profile->PopulateDialog(window, "", false); // RA2: no persona
				HWND automatic = GetDlgItem(window, 0x6FF);
				SendMessageA(automatic, 0x49D, 0, 1);
				EnableWindow(automatic, FALSE);
				SendMessageA(automatic, BM_SETCHECK, 0, 0);
				SendMessageA(automatic, 0x49D, 0, 0);
				InvalidateRect(automatic, NULL, FALSE);
			}
			return(0);
		}
		case 0x6EC: { // RA2: the registration pages
			char url[256];
			sprintf(url, "http://games2.westwood.com/cgi-bin/cgiclient?register&request=reg_menu&LANGCODE=%d", g_LanguageCode);
			ViewHTML(url, 0);
			return(0);
		}
		}
		break;
	case 0x4ED: // RA2: the dialog hides
		ShowWindow(window, SW_HIDE);
		OwnerDraw::Set_Paint_Disabled(window, 1);
		return(0);
	case OD_SUBCLASSED: { // RA2: the dialog is subclassed: the personas the chat API remembers, the countries, sides, colors and servers
		HWND nicknames = GetDlgItem(window, 0x628);
		Clear_ComboBox(nicknames);
		SendMessageA(nicknames, CB_LIMITTEXT, 9, 0);
		WideWstring prompt(StringTable::LoadString("GUI:Default", NULL, "D:\\Ra2\\wonline.cpp", 23086));
		int item = SendMessageA(nicknames, 0x4C2, 0, (LPARAM)prompt.text());
		SendMessageA(nicknames, CB_SETITEMDATA, item, -1);
		SendMessageA(nicknames, CB_SETCURSEL, item, 0);
		char const * nickname;
		char const * password;
		for (int nick_index = 1; nick_index <= 32; ++nick_index) {
			nickname = NULL;
			password = NULL;
			if (g_pChat->GetNick(nick_index, &nickname, &password) == 0 && nickname) {
				if (strlen(nickname) > 0) {
					WideWstring wide = Wstring_ToWide(nickname);
					int entry = SendMessageA(nicknames, 0x4C2, 0, (LPARAM)wide.text());
					SendMessageA(nicknames, CB_SETITEMDATA, entry, nick_index);
				}
			}
		}
		Fill_Locale_Combo(GetDlgItem(window, 0x62A));
		Fill_Side_Combo(window, 0x6A1);
		Init_Color_Entries();
		Fill_Color_Combo(window, 0x6A2);
		Populate_Server_Choices(GetDlgItem(window, 0x629), true);
		rc_ptr<PlayerProfile> profile = PlayerProfile::Load(g_NickName);
		if (profile.Get()) {
			profile->PopulateDialog(window, g_NickName, true);
		}
		int selected_nick = -1;
		int selected = SendMessageA(nicknames, CB_GETCURSEL, 0, 0);
		if (selected != -1) {
			selected_nick = SendMessageA(nicknames, CB_GETITEMDATA, selected, 0);
		}
		BOOL valid = selected_nick != -1;
		EnableWindow(GetDlgItem(window, 0x6EB), valid);
		EnableWindow(GetDlgItem(window, 0x687), valid);
		HWND automatic_button = GetDlgItem(window, 0x6FF); // RA2: the auto login check box
		EnableWindow(automatic_button, valid);
		Wstring automatic = WOL_Auto_Login_Persona();
		SendMessageA(GetDlgItem(window, 0x6FF), BM_SETCHECK, (automatic == g_NickName) ? 1 : 0, 0);
		break;
	}
	}
	return(0);
}

void Reset_Channel_State(void) // RA2: 0x00753F10, empties the lobby, game channel and user lists and the current and pending channel (ra2-decomp's twin, Yuri's Revenge 0x00799CD0)
{
	g_UserChannels.setEmpty();
	memset(&g_CurrentChannel, 0, sizeof(g_CurrentChannel));
	memset(&g_PendingChannel, 0, sizeof(g_PendingChannel));
	g_HasJoinedLobby = true;
	g_GameChannelList.setEmpty();
	g_UserList.setEmpty();
	g_ChannelCount = 0;
}

void Close_All_Dialogs(void) // RA2: 0x00753F50, destroys every dialog, the top one first
{
	HWND dialog;
	while ((dialog = WS_Top_Window()) != NULL) {
		WS_Destroy_Dialog(dialog, 0);
	}
}

int Run_Wonline_Lobby(void) // RA2: 0x00753F70, starts the chat services, fetches the server list and runs the persona dialog (0x114): 1 when a session kind was chosen, 0 when not, -1 quit, -2 the list could not be fetched; Init_Wonline's own result when it fails
{
	int initialized = Init_Wonline();
	if (initialized == 1) {
		{
			WideWstring fetching(StringTable::LoadString("TXT_FETCHING_SERVLIST", NULL, "D:\\Ra2\\wonline.cpp", 22220));
			Show_Wait_Window(16, fetching, false);
		}
		int sku = 0x2300;
		if (Online_Session_State != 2) {
			sku = 0x2100;
		}
		switch (Request_And_Wait_Server_List(sku)) {
		case 0:
			break;
		case 2:
			return(-1);
		default:
			return(-2);
		}
		if (g_pNetUtil && strlen(g_LadderAddress) && g_NickName.length() > 0) {
			g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, g_NickName.get(), g_GameSKU & 0xFFFF00, -1, 0, 0); // RA2: virtual 0x14
		}
		SimpleWonlineDialogControl control;
		OwnerDraw::Controlled_Dialog(0x114, (DLGPROC)Wonline_Profile_Login_Dialog_Proc, &control);
		return(Online_Session_State != 0);
	}
	return(initialized);
}

LRESULT __fastcall Get_Combo_Item_Text(HWND combo, WPARAM index, Wstring & text); // RA2: 0x00600F70, the narrow text of a combo box item, defined in ownrdraw.cpp

bool g_AccountDialogUpdating; // RA2: set while the account dialog fills its own controls, so their change notices are ignored

LRESULT CALLBACK Wonline_Account_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x007540A0, the dialog proc of the login dialog (0x11D when the profiles are kept per persona, else 0x10F): the persona and password, remember-login, and the persona's profile (server, country, side, color)
{
	if (message == WM_INITDIALOG && lparam) { // RA2: the control comes with the dialog's creation data
		SetWindowLongA(window, 8, *(long *)(lparam + 4));
	}
	long result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) {
		return(result);
	}
	OwnerDraw::DialogControl * control = (OwnerDraw::DialogControl *)GetWindowLongA(window, 8);
	switch (message) {
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(1);
	case WM_ERASEBKGND:
		return(1);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(1);
	case WM_COMMAND:
		switch ((unsigned short)wparam) {
		case 0x539: // RA2: log in
			control->Set_Window((unsigned)window, (HWND)0x539);
			return(0);
		case 0x686: // RA2: back
			control->Set_Window((unsigned)window, (HWND)2);
			return(0);
		case 0x628: { // RA2: the persona box
			bool keeps_profiles = WS_Find_Dialog(0x11D) != NULL; // RA2: the dialog of the per-persona profiles is the one open
			switch ((unsigned short)(wparam >> 16)) {
			case CBN_SELCHANGE: {
				if (keeps_profiles) break;
				g_AccountDialogUpdating = true;
				g_NickName.clear();
				g_LoginPassword.clear();
				g_PlaintextPassword = true;
				HWND edit = GetDlgItem(window, 0x72C); // RA2: the password box
				SendMessageA(edit, 0x4B4, 0, (LPARAM)g_LoginPassword.text());
				bool remember = g_RememberLogin;
				int selected = SendMessageA((HWND)lparam, CB_GETCURSEL, 0, 0);
				if (selected != -1) {
					int nick_index = SendMessageA((HWND)lparam, CB_GETITEMDATA, selected, 0);
					if (nick_index > 0 && nick_index <= 32) {
						char const * nickname;
						char const * password;
						Get_Combo_Item_Text((HWND)lparam, selected, g_NickName);
						g_pChat->GetNick(nick_index, &nickname, &password);
						g_LoginPassword = password;
						edit = GetDlgItem(window, 0x72C);
						SendMessageA(edit, 0x4B4, 0, (LPARAM)g_LoginPassword.text());
						g_PlaintextPassword = false;
						remember = true;
					}
				}
				bool valid = g_NickName.length() > 0 && g_LoginPassword.length() == 8;
				HWND check = GetDlgItem(window, 0x72D); // RA2: the remember-login check box
				SendMessageA(check, BM_SETCHECK, remember, 0);
				EnableWindow(check, FALSE);
				InvalidateRect(check, NULL, FALSE);
				EnableWindow(GetDlgItem(window, 0x6EB), g_NickName.length() > 0);
				EnableWindow(GetDlgItem(window, 0x539), valid);
				g_AccountDialogUpdating = false;
				return(0);
			}
			case CBN_EDITCHANGE:
			case EN_CHANGE: {
				if ((unsigned short)(wparam >> 16) == EN_CHANGE && !keeps_profiles) break;
				if ((unsigned short)(wparam >> 16) == CBN_EDITCHANGE && keeps_profiles) break;
				if (g_AccountDialogUpdating) break;
				Wstring text;
				Get_Window_Text((HWND)lparam, text);
				bool match = Recall_Stored_Login(text);
				bool remember = match || g_RememberLogin;
				if (!match) {
					g_NickName = text;
				}
				g_AccountDialogUpdating = true;
				HWND edit = GetDlgItem(window, 0x72C);
				SendMessageA(edit, 0x4B4, 0, (LPARAM)g_LoginPassword.text());
				InvalidateRect(edit, NULL, FALSE);
				g_AccountDialogUpdating = false;
				bool valid = g_NickName.length() > 0 && g_LoginPassword.length() == 8;
				HWND check = GetDlgItem(window, 0x72D);
				ShowWindow(check, SW_HIDE);
				SendMessageA(check, 0x49D, 0, 1);
				SendMessageA(check, BM_SETCHECK, remember, 0);
				EnableWindow(check, FALSE);
				SendMessageA(check, 0x49D, 0, 0);
				ShowWindow(check, SW_SHOW);
				InvalidateRect(check, NULL, FALSE);
				EnableWindow(GetDlgItem(window, 0x6EB), g_NickName.length() > 0);
				EnableWindow(GetDlgItem(window, 0x539), valid);
				return(0);
			}
			}
			break;
		}
		case 0x629: // RA2: the server box
			if (WS_Find_Dialog(0x11D) && g_PlayerProfile.Get()) {
				Wstring server;
				Get_Window_Text(GetDlgItem(window, 0x629), server);
				g_PlayerProfile->SetName(&server);
				return(0);
			}
			break;
		case 0x62A: // RA2: the locale box
			if (WS_Find_Dialog(0x11D) && g_PlayerProfile.Get()) {
				HWND combo = GetDlgItem(window, 0x62A);
				int selected = SendMessageA(combo, CB_GETCURSEL, 0, 0);
				int locale = 0;
				if (selected != -1) {
					locale = SendMessageA(combo, CB_GETITEMDATA, selected, locale);
				}
				g_PlayerProfile->SetLocale(locale, 3);
				return(0);
			}
			break;
		case 0x6A1: // RA2: the side box
			if (WS_Find_Dialog(0x11D) && g_PlayerProfile.Get()) {
				int country = -2;
				HWND combo = GetDlgItem(window, 0x6A1);
				int selected = SendMessageA(combo, CB_GETCURSEL, 0, 0);
				if (selected != -1) country = SendMessageA(combo, CB_GETITEMDATA, selected, 0);
				g_PlayerProfile->SetCountry(country);
				return(0);
			}
			break;
		case 0x6A2: // RA2: the color box
			if (WS_Find_Dialog(0x11D) && g_PlayerProfile.Get()) {
				int color = -2;
				HWND combo = GetDlgItem(window, 0x6A2);
				int selected = SendMessageA(combo, CB_GETCURSEL, 0, 0);
				if (selected != -1) color = SendMessageA(combo, CB_GETITEMDATA, selected, 0);
				g_PlayerProfile->SetColor(color);
				return(0);
			}
			break;
		case 0x72C: // RA2: the password box
			if ((unsigned short)(wparam >> 16) == EN_CHANGE && !g_AccountDialogUpdating) {
				g_PlaintextPassword = true;
				Get_Window_Text((HWND)lparam, g_LoginPassword);
				Wstring text;
				Get_Window_Text(GetDlgItem(window, 0x628), text);
				bool valid = text.length() > 0 && g_LoginPassword.length() == 8;
				HWND check = GetDlgItem(window, 0x72D);
				SendMessageA(check, 0x49D, 0, 1);
				EnableWindow(check, valid);
				InvalidateRect(check, NULL, FALSE);
				SendMessageA(check, 0x49D, 0, 0);
				EnableWindow(GetDlgItem(window, 0x6EB), FALSE);
				EnableWindow(GetDlgItem(window, 0x539), valid);
				return(0);
			}
			break;
		case 0x72D: // RA2: the remember-login check box
			g_RememberLogin = SendMessageA((HWND)lparam, BM_GETCHECK, 0, 0) == BST_CHECKED;
			return(0);
		case 0x6EB: { // RA2: the persona is forgotten
			g_AccountDialogUpdating = true;
			HWND nicknames = GetDlgItem(window, 0x628);
			int selected = SendMessageA(nicknames, 0x4B9, -1, (LPARAM)g_NickName.text());
			if (selected != -1) {
				Wstring nickname;
				Get_Combo_Item_Text(nicknames, selected, nickname);
				nickname.toLower();
				Wstring const automatic = WOL_Auto_Login_Persona();
				if (nickname == automatic.toLower()) {
					WOL_Set_Auto_Login_Persona("");
				}
				Wstring const last = WOL_Last_Valid_Persona();
				if (nickname == last.toLower()) {
					WOL_Set_Last_Valid_Persona("");
				}
				int nick_index = SendMessageA(nicknames, CB_GETITEMDATA, selected, 0);
				SendMessageA(nicknames, CB_DELETESTRING, selected, 0);
				g_pChat->SetNick(nick_index, "", "", 1);
			}
			SendMessageA(nicknames, 0x4B4, 0, (LPARAM) "");
			InvalidateRect(nicknames, NULL, FALSE);
			HWND edit = GetDlgItem(window, 0x72C);
			SendMessageA(edit, 0x4B4, 0, (LPARAM) "");
			g_NickName.clear();
			g_LoginPassword.clear();
			g_PlaintextPassword = true;
			HWND check = GetDlgItem(window, 0x72D);
			EnableWindow(check, FALSE);
			InvalidateRect(check, NULL, FALSE);
			EnableWindow(GetDlgItem(window, 0x6EB), FALSE);
			EnableWindow(GetDlgItem(window, 0x539), FALSE);
			g_AccountDialogUpdating = false;
			return(0);
		}
		case 0x6EA: // RA2: a new account
			Create_Wonline_Account(window);
			return(0);
		case 0x6EC: { // RA2: the account pages
			char url[256];
			sprintf(url, "http://games2.westwood.com/cgi-bin/cgiclient?register&request=reg_menu&LANGCODE=%d", g_LanguageCode);
			ViewHTML(url, 0);
			return(0);
		}
		}
		break;
	case 0x497: { // RA2: the dialog is filled
		g_AccountDialogUpdating = true;
		HWND nicknames = GetDlgItem(window, 0x628);
		HWND edit = GetDlgItem(window, 0x72C);
		SendMessageA(nicknames, 0x4D1, 1, 0);
		SendMessageA(edit, 0x4D1, 1, 0);
		SendMessageA(nicknames, EM_LIMITTEXT, 9, 0);
		SendMessageA(edit, EM_LIMITTEXT, 8, 0);
		Clear_ComboBox(nicknames);
		SendMessageA(nicknames, CB_LIMITTEXT, 9, 0);
		for (int nick_index = 1; nick_index < 32; nick_index++) {
			char const * nickname;
			char const * password;
			if (g_pChat->GetNick(nick_index, &nickname, &password) == 0 && nickname != NULL && strlen(nickname) > 0) {
				int entry = SendMessageA(nicknames, 0x4BC, 0, (LPARAM)nickname);
				SendMessageA(nicknames, CB_SETITEMDATA, entry, nick_index);
			}
		}
		Wstring saved(g_NickName);
		bool remember = Recall_Stored_Login(g_NickName);
		if (!remember) {
			g_NickName = saved;
		}
		SendMessageA(nicknames, 0x4B4, 0, (LPARAM)g_NickName.text());
		if (g_NickName.length() > 0) {
			SendMessageA(edit, 0x4B4, 0, (LPARAM)g_LoginPassword.text());
			g_PlaintextPassword = false;
		}
		bool valid = g_NickName.length() > 0 && g_LoginPassword.length() == 8;
		HWND check = GetDlgItem(window, 0x72D);
		SendMessageA(check, 0x49D, 0, 1);
		SendMessageA(check, BM_SETCHECK, remember, 0);
		EnableWindow(check, FALSE);
		InvalidateRect(check, NULL, FALSE);
		SendMessageA(check, 0x49D, 0, 0);
		EnableWindow(GetDlgItem(window, 0x6EB), g_NickName.length() > 0);
		EnableWindow(GetDlgItem(window, 0x539), valid);
		WOLOptionsClass options;
		options.Load();
		if (!options.Is_Bit1_Clear()) {
			Fill_Locale_Combo(GetDlgItem(window, 0x62A));
			Fill_Side_Combo(window, 0x6A1);
			HWND sides = GetDlgItem(window, 0x6A1);
			int count = SendMessageA(sides, CB_GETCOUNT, 0, 0);
			int index;
			for (index = 0; index < count; index++) {
				int data = SendMessageA(sides, CB_GETITEMDATA, index, 0);
				if (data == g_PlayerProfile->Country) {
					SendMessageA(sides, CB_SETCURSEL, index, 0);
					break;
				}
			}
			Init_Color_Entries();
			Fill_Color_Combo(window, 0x6A2);
			HWND colors = GetDlgItem(window, 0x6A2);
			int colors_count = SendMessageA(colors, CB_GETCOUNT, 0, 0);
			for (index = 0; index < colors_count; index++) {
				int data = SendMessageA(colors, CB_GETITEMDATA, index, 0);
				if (data == g_PlayerProfile->Color) {
					SendMessageA(colors, CB_SETCURSEL, index, 0);
					break;
				}
			}
			Populate_Server_Choices(GetDlgItem(window, 0x629), true);
		}
		g_AccountDialogUpdating = false;
		return(0);
	}
	case 0x4DA: { // RA2: Enter in the password box
		Get_Window_Text(GetDlgItem(window, 0x72C), g_LoginPassword);
		Wstring text;
		Get_Window_Text(GetDlgItem(window, 0x628), text);
		if (text.length() > 0 && g_LoginPassword.length() == 8) {
			PostMessageA(GetDlgItem(window, 0x539), BM_CLICK, 0, 0);
		}
		break;
	}
	case 0x4DB: { // RA2: Tab moves the focus
		bool previous = wparam != 0;
		HWND child = (HWND)lparam;
		HWND parent = GetParent(child);
		while (parent != NULL) {
			if (child == NULL) return(0);
			if (parent == window) break;
			child = parent;
			parent = GetParent(child);
		}
		if (child != NULL) Focus_Next_Tab_Item((long)child, previous);
		break;
	}
	case 0x4ED: // RA2: the dialog is hidden
		ShowWindow(window, SW_HIDE);
		OwnerDraw::Set_Paint_Disabled(window, 1);
		break;
	}
	return(0);
}

LRESULT CALLBACK Wonline_Autologin_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00755070, the dialog proc of the dialog (0x112) after a login: OK keeps the persona for the automatic login, Cancel turns the automatic login off when its "don't ask again" box is not checked
{
	if (message == WM_INITDIALOG && lparam) { // RA2: the control comes with the dialog's creation data
		SetWindowLongA(window, 8, *(long *)(lparam + 4));
	}
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) {
		return(result);
	}
	OwnerDraw::DialogControl * control = (OwnerDraw::DialogControl *)GetWindowLongA(window, 8);
	switch (message) {
	case WM_COMMAND:
		switch (wparam & 0xFFFF) {
		case IDOK:
			WOL_Set_Auto_Login_Persona(g_NickName);
			control->Set_Window((unsigned)window, (HWND)IDOK);
			break;
		case IDCANCEL: {
			bool checked = IsDlgButtonChecked(window, 0x6F9) == BST_CHECKED;
			WOL_Set_Auto_Login(!checked);
			control->Set_Window((unsigned)window, (HWND)IDCANCEL);
			return(0);
		}
		}
		break;
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(1);
	case WM_ERASEBKGND:
		return(1);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(1);
	}
	return(0);
}

void __fastcall Update_Buddy_Buttons(HWND dialog); // RA2: 0x007561C0, below

void __fastcall Fill_Buddy_List(HWND dialog) // RA2: 0x00755180, fills the buddy list (0x689) with each buddy's name, status (offline, online, the lobby or the game with its icons and ping) and selects the chosen buddy
{
	HWND list = GetDlgItem(dialog, 0x689);
	SendMessageA(list, OD_DISABLEPAINT, 0, 1);
	for (int i = 0; i < g_BuddyList.length(); ++i) {
		Wstring buddy;
		g_BuddyList.get(buddy, i);
		WideWstring display = Wstring_ToWide(buddy);
		unsigned int state = 0;
		g_BuddyStatus.getValue(buddy, state);
		int row = SendMessageA(list, OD_FINDSTRING, 0, (LPARAM)buddy.text());
		if (row == -1) row = SendMessageA(list, 0x4C8, 0, (LPARAM)buddy.text());

		OwnerDraw::CellData name;
		name.type = OwnerDraw::CellData::TEXT;
		name.string.set(display.get());
		name.hint.set((unsigned short *)L"");
		OwnerDraw::CellData status;
		status.type = OwnerDraw::CellData::TEXT;
		status.hint.set((unsigned short *)L"");
		OwnerDraw::CellData mode;
		mode.type = OwnerDraw::CellData::INVALID;
		mode.string.set((unsigned short *)L"");
		mode.hint.set((unsigned short *)L"");
		OwnerDraw::CellData password;
		password.type = OwnerDraw::CellData::INVALID;
		password.string.set((unsigned short *)L"");
		password.hint.set((unsigned short *)L"");
		OwnerDraw::CellData game_type;
		game_type.type = OwnerDraw::CellData::INVALID;
		game_type.string.set((unsigned short *)L"");
		game_type.hint.set((unsigned short *)L"");
		OwnerDraw::CellData ping;
		ping.type = OwnerDraw::CellData::INVALID;
		ping.string.set((unsigned short *)L"");
		ping.hint.set((unsigned short *)L"");

		switch (state) {
		case 1:
			status.string.set(StringTable::LoadString("WOL:BuddyDisabledFind", NULL, "D:\\Ra2\\wonline.cpp", 24375));
			name.color = status.color = 0x808080;
			break;
		case 2:
			status.string.set(StringTable::LoadString("WOL:BuddyOffline", NULL, "D:\\Ra2\\wonline.cpp", 24381));
			name.color = status.color = 0x808080;
			break;
		case 3:
			status.string.set(StringTable::LoadString("WOL:BuddyOnline", NULL, "D:\\Ra2\\wonline.cpp", 24387));
			name.color = status.color = 0xF0F0;
			break;
		case 7:
		case 8:
			status.string.set(StringTable::LoadString("WOL:BuddyQuickMatching", NULL, "D:\\Ra2\\wonline.cpp", 24394));
			name.color = status.color = 0x80FF00;
			break;
		case 4: {
			Channel channel;
			memset(&channel, 0, sizeof(channel));
			if (!g_BuddyChannels.getValue(buddy, channel)) {
				status.string.set(StringTable::LoadString("WOL:BuddyOnline", NULL, "D:\\Ra2\\wonline.cpp", 24419));
				name.color = status.color = 0xF0F0;
			} else {
				if (channel.type == 0 && strncmp((char *)channel.name, "Lob_33_", strlen("Lob_33_")) == 0) {
					int lobby = atol((char *)channel.name + strlen("Lob_33_"));
					int page = lobby / 8;
					WideWstring location(g_Lobbies[lobby % 8]);
					if (page) {
						location += L" ";
						location += WideWstring_FromInt(page + 1);
					}
					status.string.set(location.get());
				} else {
					status.string.set((unsigned short *)Narrow_To_Wide((char *)channel.name));
				}
				name.color = status.color = 0xFF4000;
			}
			break;
		}
		case 5: {
			Channel game;
			memset(&game, 0, sizeof(game));
			char found = g_BuddyChannels.getValue(buddy, game);
			if (found) {
				Wstring game_name((char *)game.name);
				found = Find_Channel_By_Name(game_name, g_GameChannelList, game);
			}
			if (found) {
				char options[132];
				strncpy(options, (char *)game.exInfo, 128);
				int build = 0;
				int hash = 0;
				int ai_count = 0;
				int observer = 0;
				int not_full = 0;
				Wstring scenario;
				Parse_Channel_ExInfo(options, &build, &hash, &ai_count, &observer, &not_full, &scenario);
				unsigned int participants = game.currentUsers - observer + ai_count;
				int reserved = (observer != 0 || not_full != 0);
				unsigned int capacity = game.maxUsers - reserved;
				wchar_t details[80];
				swprintf(details, L"%hs %d/%d", game.name, participants, capacity);
				status.string.set((unsigned short *)details);
				name.color = status.color = 0xFFFF00;
				mode.type = OwnerDraw::CellData::SURFACE;
				if (game.tournament != 0) {
					mode.surf = SurfaceCache.GetSurface("woltrny.pcx", false);
					mode.hint.set(StringTable::LoadString("TXT_TOURNAMENT_GAME", NULL, "D:\\Ra2\\wonline.cpp", 24455));
				} else if (game.reserved & 0x100) {
					mode.surf = SurfaceCache.GetSurface("wolclan.pcx", false);
					mode.hint.set(StringTable::LoadString("TXT_BATTLECLAN_GAME", NULL, "D:\\Ra2\\wonline.cpp", 24458));
				} else {
					mode.surf = SurfaceCache.GetSurface("gt18.bmp", false);
					mode.hint.set((unsigned short *)L"");
				}
				if (game.reserved & 0x600) {
					if (observer == 0 && not_full != 0) {
						game_type.type = OwnerDraw::CellData::SURFACE;
						game_type.surf = SurfaceCache.GetSurface("wolrslkob.pcx", false);
					} else {
						game_type.type = OwnerDraw::CellData::SURFACE;
						game_type.surf = SurfaceCache.GetSurface("wolreslk.pcx", false);
					}
				} else if (observer == 0 && not_full != 0) {
					game_type.type = OwnerDraw::CellData::SURFACE;
					game_type.surf = SurfaceCache.GetSurface("wolob.pcx", false);
				}
				if (game.flags & 0x100) {
					password.type = OwnerDraw::CellData::SURFACE;
					password.surf = SurfaceCache.GetSurface("wolpriv.pcx", false);
					password.hint.set(StringTable::LoadString("TXT_GAME_PASSWORD", NULL, "D:\\Ra2\\wonline.cpp", 24490));
				}
				ping.type = OwnerDraw::CellData::PING;
				ping.pingtime = game.latency;
				swprintf(details, StringTable::LoadString("WOL:GamePing", NULL, "D:\\Ra2\\wonline.cpp", 24495), game.latency);
				ping.hint.set((unsigned short *)details);
			} else {
				status.string.set(StringTable::LoadString("WOL:BuddyOnline", NULL, "D:\\Ra2\\wonline.cpp", 24498));
				name.color = status.color = 0xF0F0;
			}
			break;
		}
		case 6:
			status.string.set(StringTable::LoadString("WOL:BuddyPlayingGame", NULL, "D:\\Ra2\\wonline.cpp", 24505));
			name.color = status.color = 0x3050FF;
			break;
		default:
			status.string.set(StringTable::LoadString("WOL:BuddyUnknownStatus", NULL, "D:\\Ra2\\wonline.cpp", 24511));
			name.color = status.color = 0x808080;
			break;
		}
		SendMessageA(list, OD_SETCELL, ((unsigned short)row << 16) | (unsigned short)g_BuddyColumns[0], (LPARAM)&name);
		SendMessageA(list, OD_SETCELL, ((unsigned short)row << 16) | (unsigned short)g_BuddyColumns[1], (LPARAM)&mode);
		SendMessageA(list, OD_SETCELL, ((unsigned short)row << 16) | (unsigned short)g_BuddyColumns[2], (LPARAM)&password);
		SendMessageA(list, OD_SETCELL, ((unsigned short)row << 16) | (unsigned short)g_BuddyColumns[3], (LPARAM)&game_type);
		SendMessageA(list, OD_SETCELL, ((unsigned short)row << 16) | (unsigned short)g_BuddyColumns[4], (LPARAM)&status);
		SendMessageA(list, OD_SETCELL, ((unsigned short)row << 16) | (unsigned short)g_BuddyColumns[5], (LPARAM)&ping);
	}
	int wanted = -1;
	if (g_SelectedBuddy.length() > 0) wanted = SendMessageA(list, OD_FINDSTRING, 0, (LPARAM)g_SelectedBuddy.text());
	if (wanted != SendMessageA(list, LB_GETCURSEL, 0, 0)) SendMessageA(list, LB_SETCURSEL, wanted, 0);
	SendMessageA(list, OD_DISABLEPAINT, 0, 0);
	InvalidateRect(list, NULL, FALSE);
	Update_Buddy_Buttons(dialog);
	Update_Buddy_Profile(dialog);
}

bool __fastcall Can_Join_Channel(Channel const * channel, bool check, bool observer) // RA2: 0x00755CD0, whether the player may join the game channel: the ladder kind, the clan, the version and rules, a free player or observer slot; tells why not in the lobby chat when check is set
{
	bool ladder = Is_Channel_Flag_11(channel) || Is_Channel_Flag_12(channel);
	int tournament = Is_Tournament_Session();
	if (ladder != tournament) return(false);
	if (Is_Channel_Flag_8(channel) && g_OwnSquadID < 1) {
		if (check) Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_NO_CLAN", NULL, "D:\\Ra2\\wonline.cpp", 24551));
		return(false);
	}
	char options[132];
	strcpy(options, (char *)channel->exInfo);
	int build = 0;
	int hash = 0;
	int ai_count = 0;
	int observers = 0;
	int not_full = 0;
	Wstring scenario;
	Parse_Channel_ExInfo(options, &build, &hash, &ai_count, &observers, &not_full, &scenario);
	if (Build_Number() == build && GetINIHash() == hash) {
		if (!observer) {
			int participants = ((int)channel->currentUsers - observers) + ai_count;
			int observer_slot = (observers == 0 && not_full == 0) ? 0 : 1;
			if (participants >= (int)channel->maxUsers - observer_slot) {
				if (check) Lobby_Chat_Printf(-1, StringTable::LoadString("WOL:GameFull", NULL, "D:\\Ra2\\wonline.cpp", 24598));
			} else {
				return(true);
			}
		} else {
			if (not_full == 0 || observers != 0) {
				if (check) Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_OBSERVER_FULL", NULL, "D:\\Ra2\\wonline.cpp", 24606));
			} else {
				return(true);
			}
		}
	} else if (check) {
		Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_MISMATCH", NULL, "D:\\Ra2\\wonline.cpp", 24587));
	}
	return(false);
}

bool __fastcall Can_Join_Selected_Buddy(void) // RA2: 0x00755EF0, whether the chosen buddy is in a lobby, or in a game channel the player may join
{
	if (g_SelectedBuddy.length() == 0) return(false);
	unsigned int state = 0;
	g_BuddyStatus.getValue(g_SelectedBuddy, state);
	if (state == 4) return(true);
	if (state != 5) return(false);
	Channel channel;
	if (!g_BuddyChannels.getValue(g_SelectedBuddy, channel)) return(false);
	Wstring game_name((char *)channel.name);
	Channel game;
	if (!Find_Channel_By_Name(game_name, g_GameChannelList, game)) return(false);
	return(Can_Join_Channel(&game, false, ObserverMode != 0));
}


void __fastcall Open_Page_Dialog(void) // RA2: 0x007560E0, opens the page dialog (0x11E) and waits for it
{
	HWND dialog = WS_Create_Dialog(ProgramInstance, 0x11E, MainWindow, (DLGPROC)Page_Dialog_Proc, 0);
	Show_Dialog(dialog, 0x11E);
	SendMessageA(dialog, 0x4A9, 0, 1);
	ShowWindow(dialog, SW_SHOWNORMAL);
	WS_Wait_Dialog(dialog, WOL_Wait_Callback, false, true, true);
}

bool Selected_Buddy_Unavailable(void) // RA2: 0x00756140, whether the chosen buddy is online (not unknown, disabled or offline)
{
	if (g_SelectedBuddy.length() == 0) return(false);
	unsigned int state = 0;
	g_BuddyStatus.getValue(g_SelectedBuddy, state);
	if ((int)state >= 0 && (int)state <= 2) return(false);
	return(true);
}

void __fastcall Update_Buddy_Buttons(HWND dialog) // RA2: 0x007561C0, enables the buddy dialog's remove (0x703), join (0x62D) and page (0x5C2) buttons for the chosen buddy
{
	EnableWindow(GetDlgItem(dialog, 0x703), g_SelectedBuddy.length() > 0);
	EnableWindow(GetDlgItem(dialog, 0x62D), Can_Join_Selected_Buddy());
	EnableWindow(GetDlgItem(dialog, 0x5C2), Selected_Buddy_Unavailable());
}

static inline void Set_Text(HWND window, wchar_t const * text) // RA2: as in ra2-decomp's twin
{
	SendMessage(window, 0x4B2, 0, (LPARAM)text);
}

void __fastcall Update_Buddy_Profile(HWND dialog) // RA2: 0x007562B0, shows the chosen buddy's name and ladder record (rung, points, wins, losses, disconnects)
{
	HWND label = GetDlgItem(dialog, 0x6EF);
	HWND rung = GetDlgItem(dialog, 0x6F4);
	HWND points = GetDlgItem(dialog, 0x6F5);
	HWND wins = GetDlgItem(dialog, 0x6F0);
	HWND losses = GetDlgItem(dialog, 0x6F1);
	HWND disconnects = GetDlgItem(dialog, 0x701);
	if (g_SelectedBuddy.length() == 0) {
		Set_Text(label, L"");
		Set_Text(rung, L"");
		Set_Text(points, L"");
		Set_Text(wins, L"");
		Set_Text(losses, L"");
		Set_Text(disconnects, L"");
		return;
	}
	WideWstring format(StringTable::LoadString("GUI:UserProfileLabel", NULL, "D:\\Ra2\\wonline.cpp", 24700));
	wchar_t * text = new wchar_t[format.length() + g_SelectedBuddy.length() + 1];
	swprintf(text, (wchar_t const *)format.get(), g_SelectedBuddy.get());
	HWND title = GetDlgItem(dialog, 0x6EF);
	Set_Text(title, text);
	delete [] text;
	Ladder ladder;
	memset(&ladder, 0, sizeof(ladder));
	Wstring key = static_cast<Wstring const &>(g_SelectedBuddy).toLower();
	if (g_GeneralLadderDict.getValue(key, ladder)) {
		Set_Text(rung, WideWstring_FromInt(ladder.rung).text());
		Set_Text(points, WideWstring_FromInt(ladder.points).text());
		Set_Text(wins, WideWstring_FromInt(ladder.wins).text());
		Set_Text(losses, WideWstring_FromInt(ladder.losses).text());
		Set_Text(disconnects, WideWstring_FromInt(ladder.disconnects).text());
	} else {
		WideWstring unknown(StringTable::LoadString("GUI:UnknownStats", NULL, "D:\\Ra2\\wonline.cpp", 24717));
		Set_Text(rung, unknown.text());
		Set_Text(points, unknown.text());
		Set_Text(wins, unknown.text());
		Set_Text(losses, unknown.text());
		Set_Text(disconnects, unknown.text());
	}
}

long g_BuddyFindTime; // RA2: when the last buddy find request was sent
long g_BuddyRefreshTime; // RA2: when the buddies' channels were last asked for

void Service_Buddy_Find_Queue(void) // RA2: 0x00756610, sends the next queued buddy find request, at most one each five seconds
{
	if (g_BuddyFindName.length() > 0) {
		if ((unsigned long)(time(NULL) - g_BuddyFindTime) < 5) return;
		g_BuddyFindName.clear();
	}
	if (g_BuddyRequests.length() != 0) {
		g_BuddyRequests.removeHead(g_BuddyFindName);
		g_BuddyFindTime = time(NULL);
		User user;
		memset(&user, 0, sizeof(user));
		strcpy((char *)user.name, g_BuddyFindName.get());
		g_pChat->RequestFind(&user);
	}
}

bool __fastcall Can_Add_Buddy(Wstring const & name) // RA2: 0x00756790, whether the name may become a buddy: not empty, not the player, no separators, not a buddy yet
{
	Wstring lower(name);
	lower.toLower();
	if (name.length() == 0) return(false);
	if (static_cast<Wstring const &>(lower).toLower() == static_cast<Wstring const &>(g_NickName).toLower()) return(false);
	if (name.containsAny(" ,;")) return(false);
	for (int i = 0; i < g_BuddyList.length(); ++i) {
		Wstring other;
		g_BuddyList.get(other, i);
		other.toLower();
		if (other == lower) return(false);
	}
	return(true);
}

void __fastcall Add_Buddy(Wstring const & name) // RA2: 0x007568C0, adds the name to the buddies and queues its find and ladder requests
{
	if (Can_Add_Buddy(name)) {
		Wstring entry(name);
		g_BuddyList.addTail(entry);
		g_BuddyRequests.addTail(entry);
		g_BuddyAddQueue.addTail(entry);
		Service_Buddy_Find_Queue();
	}
}


void __fastcall Request_Add_Buddy(Wstring const & name) // RA2: 0x00756B40, asks the chat server to add the buddy, adds it here and asks for its ladder record
{
	if (Can_Add_Buddy(name)) {
		User user;
		memset(&user, 0, sizeof(user));
		strncpy((char *)user.name, name.text(), 9);
		g_pChat->RequestBuddyAdd(&user);
		Add_Buddy(name);
		Request_Buddy_Ladders();
	}
}

static inline char Remove_Channel(Wstring & key, Channel & value) // RA2: ra2-decomp's file-local helper at this place (its LadderC is WOL::Channel here)
{
	return(g_BuddyChannels.remove(key, value));
}

void __fastcall Remove_Buddy(Wstring const & name) // RA2: 0x00756E10, removes the buddy here and asks the chat server to delete it
{
	for (int i = 0; i < g_BuddyList.length(); ++i) {
		Wstring other;
		g_BuddyList.get(other, i);
		if (other == name) {
			g_BuddyList.remove(i);
			Wstring key(name);
			Channel channel;
			Remove_Channel(key, channel);
			g_BuddyStatus.remove(key);
			User user;
			memset(&user, 0, sizeof(user));
			strncpy((char *)user.name, name.text(), 9);
			g_pChat->RequestBuddyDelete(&user);
			return;
		}
	}
}

void Refresh_Buddy_Status(void) // RA2: 0x007570E0, asks for the game channels and queues a find request for every buddy
{
	g_BuddyRefreshTime = time(NULL);
	if (!Request_Game_Channel_List(1, false)) {
		SetEvent(g_WaitEventHandles[5]);
		return;
	}
	g_BuddyRequests.clear();
	for (int i = 0; i < g_BuddyList.length(); i++) {
		Wstring entry;
		g_BuddyList.get(entry, i);
		g_BuddyRequests.addTail(entry);
	}
	Service_Buddy_Find_Queue();
}

void Request_Buddy_Ladders(void) // RA2: 0x00757380, asks the ladder server for the records of up to ten queued buddies
{
	int count = g_BuddyAddQueue.length();
	if (count > 10) {
		count = 10;
	} else if (count == 0) {
		return;
	}
	Wstring names;
	for (int i = 0; i < count; i++) {
		Wstring name;
		g_BuddyAddQueue.removeHead(name);
		if (names.length() > 0) names += ":";
		names += name;
	}
	g_pNetUtil->RequestLadderList(g_LadderAddress, g_LadderPort, names.get(), g_GameSKU & 0xFFFF00, -1, 0, 0); // RA2: virtual 0x14
}

void Reset_Buddy_Ladders(void) // RA2: 0x00757530, queues every buddy for a ladder record request
{
	g_BuddyAddQueue.clear();
	for (int i = 0; i < g_BuddyList.length(); ++i) {
		Wstring name;
		g_BuddyList.get(name, i);
		g_BuddyAddQueue.addTail(name);
	}
	Request_Buddy_Ladders();
}

void Reset_Buddy_State(void) // RA2: 0x00757720, clears the buddies, asks the chat server for the buddy list and queues the find and ladder requests again (ra2-decomp's twin, Yuri's Revenge 0x0079D4E0)
{
	g_BuddyList.clear();
	g_BuddyRequests.clear();
	g_BuddyStatus.clear();
	g_BuddyChannels.clear();
	g_pChat->RequestBuddyList();
	if (!Is_Tournament_Session()) {
		Refresh_Buddy_Status();
	}
	Reset_Buddy_Ladders();
}

BOOL CALLBACK Buddy_Message_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00757950, the dialog that pages the selected buddy (main's text; ra2-decomp's twin, Yuri's Revenge 0x0079D720)
{
	BOOL result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) return(result);
	switch (message) {
	case WM_DESTROY:
		return(FALSE);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(TRUE);
	case WM_ERASEBKGND:
		return(TRUE);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(TRUE);
	case WM_COMMAND:
		switch (wparam & 0xFFFF) {
		case 0x540:
			switch (wparam >> 16) {
			case EN_CHANGE: {
				WideWstring text;
				Get_Window_Wide_Text((HWND)lparam, text);
				EnableWindow(GetDlgItem(window, 0x5C2), text.length() > 0);
				break;
			}
			case EN_UPDATE: {
				WideWstring text;
				Get_Window_Wide_Text((HWND)lparam, text);
				text.trimTrailing(L"\n\r");
				return(FALSE);
			}
			case EN_MAXTEXT:
				SendMessageA(GetDlgItem(window, 0x5C2), BM_CLICK, 0, 0);
				break;
			}
			break;
		case 0x5C2: {
			WideWstring text;
			HWND edit = GetDlgItem(window, 0x540);
			Get_Window_Wide_Text(edit, text);
			SendMessageA(edit, 0x4B2, 0, (LPARAM)L"");
			if (text.length() > 0) {
				text.trimTrailing(L"\n\r");
				User user;
				memset(&user, 0, sizeof(user));
				strcpy((char *)user.name, g_SelectedBuddy.get());
				g_pChat->RequestUnicodePage(&user, text.get()); // RA2: virtual 0xF8
			}
			InvalidateRect(edit, NULL, FALSE);
			WS_Destroy_Dialog(window, 0x5C2);
			break;
		}
		case 0x686:
			WS_Destroy_Dialog(window, 0x686);
			break;
		}
		break;
	case OD_SUBCLASSED: {
		HWND edit = GetDlgItem(window, 0x540);
		SendMessageA(edit, EM_LIMITTEXT, 0x78, 0);
		SendMessageA(edit, 0x4B2, 0, (LPARAM)L"");
		EnableWindow(GetDlgItem(window, 0x5C2), FALSE);
		HWND label = GetDlgItem(window, 0x712);
		SendMessageA(label, 0x4B4, 0, (LPARAM)g_SelectedBuddy.text());
		break;
	}
	}
	return(FALSE);
}

LRESULT __fastcall Get_List_Item_Wide_Text(HWND window, WPARAM index, WideWstring & text); // RA2: 0x00600F10, defined in netshare.cpp

Wstring __fastcall Get_Page_Recipient(HWND list) // RA2: 0x00757CC0, the name in parentheses of the first list entry from the selection on that holds one; the selected buddy when none does (ra2-decomp's twin, Yuri's Revenge 0x0079DA90)
{
	int index = SendMessageA(list, LB_GETCURSEL, 0, 0);
	if (index == -1) {
		index = 0;
	}
	while (index < SendMessageA(list, LB_GETCOUNT, 0, 0)) {
		WideWstring text;
		Get_List_Item_Wide_Text(list, index, text);
		if (text.getChar(0) == L'(' && text.truncate(L')')) {
			text.removeChar(L'(');
			return(Wstring_Narrow(text));
		}
		index++;
	}
	return(g_SelectedBuddy);
}

BOOL CALLBACK Page_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00757D80, the page dialog (0x11E) (main's text; ra2-decomp's twin, Yuri's Revenge 0x0079DB50)
{
	User user;
	BOOL result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) return(result);
	switch (message) {
	case WM_DESTROY:
		return(FALSE);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(TRUE);
	case WM_ERASEBKGND:
		return(TRUE);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(TRUE);
	case WM_COMMAND:
		switch ((int)(wparam & 0xFFFF)) {
		case 0x540:
			switch ((unsigned short)(wparam >> 16)) {
			case EN_CHANGE: {
				WideWstring text;
				Get_Window_Wide_Text((HWND)lparam, text);
				EnableWindow(GetDlgItem(window, 0x5C2), text.length() > 0);
				return(FALSE);
			}
			case EN_UPDATE: {
				WideWstring text;
				Get_Window_Wide_Text((HWND)lparam, text);
				text.trimTrailing(L"\n\r");
				return(FALSE);
			}
			case EN_MAXTEXT: {
				WideWstring text;
				Get_Window_Wide_Text((HWND)lparam, text);
				text.trimTrailing(L"\n\r");
				if (text.length() > 0) SendMessageA(GetDlgItem(window, 0x5C2), BM_CLICK, 0, 0);
				return(FALSE);
			}
			}
			break;
		case 0x53F:
			switch ((unsigned short)(wparam >> 16)) {
			case LBN_SELCHANGE: {
				Wstring name = Get_Page_Recipient((HWND)lparam);
				HWND label = GetDlgItem(window, 0x718);
				SendMessageA(label, 0x4B4, 0, (LPARAM)name.text());
				break;
			}
			}
			break;
		case 0x5C2: {
			WideWstring text;
			HWND edit = GetDlgItem(window, 0x540);
			Get_Window_Wide_Text(edit, text);
			SendMessageA(edit, 0x4B2, 0, (LPARAM)L"");
			Wstring name = Get_Page_Recipient(GetDlgItem(window, 0x53F));
			if (name.length() > 0 && text.length() > 0) {
				text.trimTrailing(L"\n\r");
				memset(&user, 0, sizeof(user));
				strcpy((char *)user.name, name.get());
				g_pChat->RequestUnicodePage(&user, text.get()); // RA2: virtual 0xF8
				Lobby_Chat_Printf(ColorUser, L"[%hs] %s", g_NickName, text.get()); // RA2: the name is passed as the object, as the other chat lines do
			}
			EnableWindow((HWND)lparam, FALSE);
			InvalidateRect(edit, NULL, FALSE);
			break;
		}
		case 0x686:
			WS_Destroy_Dialog(window, 0x5C2);
			break;
		}
		break;
	case OD_SUBCLASSED: {
		HWND edit = GetDlgItem(window, 0x540);
		SendMessageA(edit, EM_LIMITTEXT, 0x78, 0);
		SendMessageA(edit, 0x4B2, 0, (LPARAM)L"");
		EnableWindow(GetDlgItem(window, 0x5C2), FALSE);
		Show_Name(window, Get_Page_Recipient(GetDlgItem(window, 0x53F)));
		if (MouseCursor) {
			MouseCursor->Release_Mouse();
		}
		break;
	}
	}
	return(FALSE);
}

LRESULT CALLBACK Add_Buddy_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00758210, the add buddy dialog (0x115) (main's text; ra2-decomp's twin, Yuri's Revenge 0x0079DFE0)
{
	if (message == WM_INITDIALOG && lparam) SetWindowLongA(window, 8, *(long *)(lparam + 4));
	long result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) return(result);
	SimpleWonlineDialogControl * control = (SimpleWonlineDialogControl *)GetWindowLongA(window, 8);
	switch (message) {
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
	case WM_ERASEBKGND:
		return(TRUE);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(TRUE);
	case WM_COMMAND:
		switch (wparam & 0xFFFF) {
		case 0x540:
			if ((unsigned short)(wparam >> 16) != 0) {
				Wstring name;
				Get_Window_Text((HWND)lparam, name);
				EnableWindow(GetDlgItem(window, 0x702), Can_Add_Buddy(name));
			}
			break;
		case 0x686:
			control->Set_Window((unsigned)window, (HWND)0x686);
			break;
		case 0x702: {
			Wstring name;
			HWND edit = GetDlgItem(window, 0x540);
			Get_Window_Text(edit, name);
			SendMessageA(edit, 0x4B4, 0, (LPARAM) "");
			if (Can_Add_Buddy(name)) {
				Request_Add_Buddy(name);
			}
			InvalidateRect(edit, NULL, FALSE);
			control->Set_Window((unsigned)window, (HWND)0x702);
			break;
		}
		}
		break;
	case OD_SUBCLASSED: {
		HWND edit = GetDlgItem(window, 0x540);
		SendMessageA(edit, EM_LIMITTEXT, 9, 0);
		SendMessageA(edit, 0x4D1, 1, 0);
		SendMessageA(edit, 0x4B4, 0, (LPARAM) "");
		EnableWindow(GetDlgItem(window, 0x702), FALSE);
		break;
	}
	case 0x4DA:
		PostMessageA(GetDlgItem(window, 0x702), BM_CLICK, 0, 0);
		break;
	}
	return(FALSE);
}

inline void Start_Buddy_Refresh(HWND window, HWND refresh) // RA2: main's helper
{
	SendMessageA(refresh, 0x4AA, 0, (LPARAM)SurfaceCache.GetSurface("wodref.pcx", 0));
	Reset_Buddy_State();
	SetTimer(window, 0, 1000, NULL);
}

BOOL CALLBACK Buddy_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00758750, the buddy list dialog (0x116) (main's text)
{
	if (message == WM_INITDIALOG && lparam) SetWindowLongA(window, DWL_USER, *(long *)(lparam + 4));
	BOOL result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) return(result);
	OwnerDraw::DialogControl * control = (OwnerDraw::DialogControl *)GetWindowLongA(window, DWL_USER);
	switch (message) {
	case WM_ERASEBKGND:
		return(TRUE);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(TRUE);
	case WM_DESTROY:
		KillTimer(window, 0);
		KillTimer(window, 0x704);
		Save_Observer_Mode();
		return(FALSE);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(TRUE);
	case WM_COMMAND:
		if (control->Is_Done()) return(FALSE);
		switch (LOWORD(wparam)) {
		case 0x5C2: {
			HWND page = WS_Create_Dialog(ProgramInstance, 0x11E, MainWindow, (DLGPROC)Page_Dialog_Proc, 0);
			Show_Dialog(page, 0x11E);
			SendMessageA(page, 0x4A9, 0, 1);
			ShowWindow(page, SW_SHOWNORMAL);
			WS_Wait_Dialog(page, WOL_Wait_Callback, false, true, true);
			return(FALSE);
		}
		case 0x540:
			if (HIWORD(wparam) == EN_CHANGE) {
				Wstring text;
				Get_Window_Text((HWND)lparam, text);
				EnableWindow(GetDlgItem(window, 0x702), text.length() != 0);
			}
			return(FALSE);
		case 0x62B:
			control->Set_Window((unsigned)window, (HWND)0x62B);
			return(FALSE);
		case 0x686:
			control->Set_Window((unsigned)window, (HWND)0x686);
			return(FALSE);
		case 0x62D:
			control->Set_Window((unsigned)window, (HWND)0x62D);
			return(FALSE);
		case 0x689:
			switch (wparam >> 16) {
			case LBN_DBLCLK:
				if (Can_Join_Selected_Buddy()) {
					SendMessageA(GetDlgItem(window, 0x62D), BM_CLICK, 0, 0);
					return(FALSE);
				}
				if (Selected_Buddy_Unavailable()) {
					SendMessageA(GetDlgItem(window, 0x5C2), BM_CLICK, 0, 0);
				}
				return(FALSE);
			case LBN_SELCHANGE: {
				int selected = SendMessageA((HWND)lparam, LB_GETCURSEL, 0, 0);
				if (selected == -1) g_SelectedBuddy.clear();
				else Get_Window_Item_Text((HWND)lparam, selected, g_SelectedBuddy);
				Update_Buddy_Buttons(window);
				Update_Buddy_Profile(window);
				return(FALSE);
			}
			}
			return(FALSE);
		case 0x760:
		case 0x75F: {
			int checked = 0;
			HWND button = GetDlgItem(window, 0x760);
			if (button) checked = SendMessageA(button, BM_GETCHECK, 0, 0);
			if ((short)wparam == 0x75F) checked = !checked;
			ObserverMode = checked;
			Update_Observer_Buttons(NULL);
			return(FALSE);
		}
		case 0x702: {
			SimpleWonlineDialogControl add;
			OwnerDraw::Controlled_Dialog(0x115, (DLGPROC)Add_Buddy_Dialog_Proc, &add);
			Fill_Buddy_List(window);
			return(FALSE);
		}
		case 0x703: {
			HWND list = GetDlgItem(window, 0x689);
			int selected = SendMessageA(list, LB_GETCURSEL, 0, 0);
			if (selected != -1) {
				Wstring name;
				Get_Window_Item_Text(list, selected, name);
				Remove_Buddy(name);
				SendMessageA(list, LB_DELETESTRING, selected, 0);
				g_SelectedBuddy.clear();
				Fill_Buddy_List(window);
			}
			return(FALSE);
		}
		case 0x704:
			EnableWindow((HWND)lparam, FALSE);
			Refresh_Buddy_Status();
			SetTimer(window, 0x704, 3000, NULL);
			return(FALSE);
		}
		return(FALSE);
	case WM_TIMER:
		switch (wparam) {
			case 0x704:
				KillTimer(window, 0x704);
				EnableWindow(GetDlgItem(window, 0x704), TRUE);
				return(FALSE);
			case 0:
				if ((unsigned long)(time(NULL) - g_BuddyRefreshTime) > 30) {
					Refresh_Buddy_Status();
				}
				return(FALSE);
		}
		return(FALSE);
	case 0x497: {
		Update_Observer_Buttons(window);
		HWND list = GetDlgItem(window, 0x689);
		SendMessageA(list, 0x4A6, g_BuddyNameWidth, g_BuddyColumns[0]);
		SendMessageA(list, 0x4A6, 0, g_BuddyColumns[1]);
		SendMessageA(list, 0x4A6, 0, g_BuddyColumns[2]);
		SendMessageA(list, 0x4A6, 0, g_BuddyColumns[3]);
		SendMessageA(list, 0x4A6, g_BuddyStatusWidth, g_BuddyColumns[4]);
		SendMessageA(list, 0x4A6, 0, g_BuddyColumns[5]);
		HWND refresh = GetDlgItem(window, 0x704);
		SendMessageA(refresh, 0x49C, 0, (LPARAM)SurfaceCache.GetSurface("wouref.pcx", 0));
		Start_Buddy_Refresh(window, refresh);
		return(FALSE);
	}
	}
	return(FALSE);
}

bool __fastcall Find_Channel_By_Name(Wstring const & name, ArrayList<Channel> const & channels, Channel & channel) // RA2: 0x007592D0, copies the channel of this name out of the list; cleared and false when there is none (ra2-decomp's twin, Yuri's Revenge 0x0079F0A0)
{
	memset(&channel, 0, sizeof(Channel));
	for (int i = 0; i < channels.length(); i++) {
		channels.get(channel, i);
		if (stricmp((char *)channel.name, name.text()) == 0) {
			return(true);
		}
	}
	memset(&channel, 0, sizeof(Channel));
	return(false);
}

int __fastcall Run_Online_Menu(bool first_time); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: 2579 bytes differ (main's text with main's two single-caller helpers Request_Buddy_Game_List and Report_Disconnected written out at their sites and the event numbers named; 2391 with the helpers, held on variation_candidates.csv); the second lookup as the plain found test the original keeps
int __fastcall Run_Online_Menu(bool first_time) // RA2: 0x00759370, the buddy list screen run from the online menu: hosts a game, joins a buddy's lobby or game, or leaves; 1 when a game starts, 2 for a skirmish, 0 otherwise (main's text)
{
	g_PlayingNetGame = false;
	WolOptionsLocked = 0;
	Start_Network_Session();
	if (!Request_Chat_Channel_List(true)) {
		Online_Session_Return = 0;
		return(0);
	}
	Leave_Current_Channel(0x40, true);
	g_BuddyChannels.clear();
	for (;;) {
		SimpleWonlineDialogControl control;
		Reset_Wait_Events();
		HWND dialog = OwnerDraw::Begin_Dialog(0x116, (DLGPROC)Buddy_Dialog_Proc, (LPARAM)&control);
		OwnerDraw::Display_Dialog(dialog);
		while (!control.Is_Done()) {
			DWORD event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100);
			while (event == WAIT_TIMEOUT && !control.Is_Done()) {
				WOL_Wait_Callback();
				event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 100);
			}
			ResetEvent(g_WaitEventHandles[event]);
			if (event == EV_ABORT) {
				Close_Wait_Window(~0U);
				OwnerDraw::End_Dialog(dialog);
				Online_Session_Return = 0;
				ODMessageBox(StringTable::LoadString("TXT_YOURE_DISCON", NULL, "D:\\Ra2\\wonline.cpp", 25585), MB_OK, Wait_Callback_Thunk, false);
				return(0);
			}
			if (event == EV_ERROR) {
				Close_Wait_Window(~0U);
				OwnerDraw::End_Dialog(dialog);
				Online_Session_Return = 0;
				ODMessageBox(StringTable::LoadString("TXT_YOURE_DISCON", NULL, "D:\\Ra2\\wonline.cpp", 25593), MB_OK, Wait_Callback_Thunk, false);
				return(0);
			}
			Title_Screen_Restore(false);
		}
		OwnerDraw::End_Dialog(dialog);
		switch ((int)control.Get_Window()) {
			case IDC_NEWCHAN:
				Reset_Wait_Events();
			if (!Host_Game()) {
					DebugString("Buddy List Host Create Game failed\n");
			} else {
					g_JoinLobbyNow = 0;
				do {
					DWORD event;
					do {
							event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 20);
						if (event == WAIT_TIMEOUT) {
								WOL_Wait_Callback();
						}
					} while (event == WAIT_TIMEOUT && !g_JoinLobbyNow);
						ResetEvent(g_WaitEventHandles[event]);
						if (event == EV_ABORT) {
							DebugString("EV_ABORT from Buddy List Host\n");
							g_JoinLobbyNow = 1;
					}
						if (event == EV_EXIT) {
							DebugString("EV_EXIT from Buddy List Host\n");
							g_JoinLobbyNow = 1;
					}
						if (event == EV_SKIRMISH) {
							DebugString("Skirmish from Buddy List\n");
							g_GameStartRequested = false;
							g_LastErrorMessage[0] = 0;
							g_PlayingNetGame = false;
						return(2);
						} else if (event == EV_STARTGAME) {
							DebugString("Start Game from Buddy List Host\n");
							g_GameStartRequested = false;
						g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
							g_LastErrorMessage[0] = 0;
							g_PlayingNetGame = true;
							g_JoinLobbyNow = 0;
						return(1);
					}
				} while (!g_JoinLobbyNow);
				DebugString("Stop Buddy Host\n");
				g_JoinLobbyNow = 0;
			}
			Close_Wait_Window(~0U);
			Reset_Wait_Events();
			if (g_LastErrorMessage[0]) {
				ODMessageBox(g_LastErrorMessage, MB_OK, Wait_Callback_Thunk, false);
			}
			while (WS_Destroy_Dialog(NULL, 0)) {
			}
			g_PlayingNetGame = false;
			break;
			case IDC_JOINLEAVE: {
				Channel channel;
				memset(&channel, 0, sizeof(channel));
			if (g_SelectedBuddy.length() > 0 && g_BuddyChannels.getValue(g_SelectedBuddy, channel)) {
					unsigned int kind = 0;
				if (g_BuddyStatus.getValue(g_SelectedBuddy, kind)) {
					switch (kind) {
					case 4: {
								Channel lobby;
								memset(&lobby, 0, sizeof(lobby));
								Online_Session_Return = 3;
						for (int i = 0; i < g_UserChannels.length(); i++) {
									g_UserChannels.get(lobby, i);
							if (!stricmp((char *)lobby.name, (char *)channel.name)) {
										g_LobbyNumber = atol((char *)channel.name + strlen("Lob_33_"));
										g_HasJoinedLobby = false;
								return(0);
							}
						}
						return(0);
					}
					case 5: {
								Wstring name((char *)channel.name);
								Channel game;
								char found = Find_Channel_By_Name(name, g_GameChannelList, game);
								if (!found) {
									found = Request_Game_Channel_List(0, true);
									if (!found) {
										DebugString("Buddy List Guest Get Channel List failed\n");
										SetEvent(g_WaitEventHandles[EV_ERROR]);
									}
									if (found) {
										found = Find_Channel_By_Name(name, g_GameChannelList, game);
									}
							if (!found) {
										DebugString("Buddy List Guest Find Channel by name failed\n");
								break;
							}
						}
						if (!Join_Game_Channel(&game, ObserverMode)) {
							DebugString("Buddy List Guest Join_Game failed\n");
						} else {
							g_JoinLobbyNow = 0;
							int timeout = 500;
							do {
								DWORD event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 20);
								while (event == WAIT_TIMEOUT && !g_JoinLobbyNow) {
									WOL_Wait_Callback();
									event = WaitForMultipleObjects(EV_COUNT, g_WaitEventHandles, FALSE, 20);
								}
								ResetEvent(g_WaitEventHandles[event]);
										if (event == EV_ABORT) {
									DebugString("EV_ABORT from Buddy List Guest\n");
									g_JoinLobbyNow = 1;
								}
										if (event == EV_EXIT) {
											DebugString("EV_EXIT from Buddy List Guest\n");
											g_JoinLobbyNow = 1;
								}
										if (event == EV_ERROR) {
											DebugString("EV_ERROR from Buddy List Guest\n");
											g_JoinLobbyNow = 1;
								}
										if (event == EV_CHAN_JOIN_FAIL) {
											DebugString("EV_CHAN_JOIN_FAIL from Buddy List Guest\n");
											g_JoinLobbyNow = 1;
								}
										if (event == EV_STARTGAME) {
											DebugString("EV_STARTGAME from Buddy List Guest\n");
											g_GameStartRequested = false;
									g_pChat->RequestChannelLeave(); // RA2: virtual 0x24
											g_LastErrorMessage[0] = 0;
											g_PlayingNetGame = true;
											g_JoinLobbyNow = 0;
									return(1);
								}
								if (timeout-- < 0) {
									DebugString("Timeout from Buddy List Guest\n");
									break;
								}
							} while (!g_JoinLobbyNow);
							g_JoinLobbyNow = 0;
							DebugString("Exit from BuddyList Guest\n");
						}
						break;
					}
					}
				}
			}
			break;
		}
			case IDC_CANCEL:
		case 0x686:
			if (Online_Session_Return == 3) {
				g_HasJoinedLobby = false;
			}
			return(0);
		default:
			break;
		}
	}
}
#endif

void __fastcall Set_Match_State(int state, int timeout, int result) // RA2: 0x00759DF0, sets the four values of the match state together (mischa85's name; ra2-decomp's twin, Yuri's Revenge 0x0079FC10, is an ordinary function too). No caller in this game: every caller, earlier and later in the file, expands it, and its out-of-line body stays here
{
	g_MatchResult = result;
	g_MatchState = state;
	g_MatchTimeout = timeout;
	g_MatchStartTime = time(NULL);
}

int __fastcall Start_Quick_Match(void); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: 1350 bytes differ (main and ra2-decomp channel-list request; the original calls Reset_Wait_Events here)
void __fastcall Collect_Remote_Player_Names(void); // RA2: 0x0075A6C0, below
bool __fastcall Start_Match(void); // RA2: 0x0075BE10, below
BOOL CALLBACK Wonline_Waiting_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x0075C350, below
BOOL CALLBACK Wonline_Map_Download_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x0075C830, below

inline void Show_Match_Message(char const * label, int line) // RA2: main's helper: tells the player why the match stopped
{
	ODMessageBox(StringTable::LoadString(label, NULL, "D:\\Ra2\\wonline.cpp", line), MB_OK, Wait_Callback_Thunk, false);
}

int __fastcall Start_Quick_Match(void) // RA2: 0x00759E20, the quick or tour match: finds the matching lobby, joins it and waits in the waiting dialog (0x117) for the match server; 1 when the game starts (main's text)
{
	if (ObserverMode) {
		ObserverMode = 0;
		Update_Observer_Buttons(NULL);
	}
	g_PlayingNetGame = false;
	Set_Match_State(1, -1, 0);
	if (VideoModeWidth > 640) {
		WideWstring query(Session.Type == GAME_INTERNET && Online_Session_State == 2
			? StringTable::LoadString("WOL:WDTResolutionQuery", NULL, "D:\\Ra2\\wonline.cpp", 25816)
				: StringTable::LoadString("WOL:QuickMatchResolutionQuery", NULL, "D:\\Ra2\\wonline.cpp", 25817));
		if (ODMessageBox(query.get(), MB_YESNO, Wait_Callback_Thunk, true) != IDYES) return(0);
		if (!Test_Display_Mode_Dialog(640, 480)) return(0);
		Options.ScreenWidth = 640;
		Options.ScreenHeight = 480;
		Options.Save_Settings();
	}
	WolOptionsLocked = 0;
	Start_Network_Session();
	Set_Match_State(2, 30, 0);
	g_UserChannels.setEmpty();
	g_MatchOpponent.clear();
	if (!Request_Chat_Channel_List(true)) return(0);
	if (g_UserChannels.length() == 0) {
		Show_Match_Message("WOL:NoMatchingAgentFound", 25851);
		return(0);
	}
	Channel channel;
	memset(&channel, 0, sizeof(channel));
	char const * prefix = (Session.Type == GAME_INTERNET && Online_Session_State == 2) ? "Lob_35_" : "Lob_38_";
	int i;
	for (i = 0; i < g_UserChannels.length(); i++) {
		g_UserChannels.get(channel, i);
		if (_strnicmp((char *)channel.name, prefix, strlen(prefix)) == 0) break;
	}
	if (i >= g_UserChannels.length()) {
		Show_Match_Message("WOL:MatchingServerUnavailable", 25870);
		return(0);
	}
	HWND dialog = WS_Create_Dialog(ProgramInstance, 0x117, MainWindow, (DLGPROC)Wonline_Waiting_Dialog_Proc, 0);
	Show_Dialog(dialog, 0x117);
	SendMessageA(dialog, 0x4A9, 0, 1);
	ShowWindow(dialog, SW_SHOWNORMAL);
	Reset_Wait_Events();
	int result = 0;
	Reset_Buddy_State();
	for (;;) {
		if (g_MatchState == 2) {
			int error;
			{
				Wstring password("zotclot9");
				error = Request_Channel_Join(&channel, password, StringTable::LoadString("TXT_JOINLOB", NULL, "D:\\Ra2\\wonline.cpp", 25894));
			}
			if (error) {
				Set_Match_State(0, -1, 1);
				Show_Match_Message("WOL:ChannelJoinFailure", 25897);
				break;
			}
			Set_Match_State(3, 20, 0);
		}
		if (g_MatchState == 3 && g_UserList.length() > 1 && strcmp((char *)channel.name, (char *)g_CurrentChannel.name) == 0) Request_Match();
		DWORD event;
		while ((event = WaitForMultipleObjects(18, g_WaitEventHandles, FALSE, 20)) == WAIT_TIMEOUT) WOL_Wait_Callback();
		ResetEvent(g_WaitEventHandles[event]);
		if (event == 0 || event == 6) break;
		if (event == 17) {
			Collect_Remote_Player_Names();
			Service_Pending_Player_Pings();
		} else if (event == 5) {
			if (g_MatchState == 0) {
				Leave_Current_Channel(0x40, false);
				switch (g_MatchResult) {
				case 1:
					Show_Match_Message("WOL:NoMatchingAgentFound", 25964);
					break;
				case 2:
					Show_Match_Message("WOL:MatchTimeout", 25968);
					break;
				default:
					Show_Match_Message("WOL:MatchFailExit", 25972);
					break;
				}
				break;
			}
		} else if (event == 16) {
			if (Start_Match() && Online_Session_State == 2) {
				HWND download = OwnerDraw::Begin_Dialog(0x11F, (DLGPROC)Wonline_Map_Download_Dialog_Proc, 0);
				Set_Match_State(10, 180, 0);
				SendMessageA(download, 0x4A9, 0, 1);
				ShowWindow(download, SW_SHOWNORMAL);
			}
		} else if (event == 7) {
			Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("WOL:MatchGameStarting", NULL, "D:\\Ra2\\wonline.cpp", 25997));
			g_GameStartRequested = false;
			result = 1;
			break;
		}
	}
	WS_Destroy_Dialog(dialog, 0);
	HWND download = WS_Find_Dialog(0x11F);
	if (download) WS_Destroy_Dialog(download, 0);
	if (g_MatchState != 11) Set_Match_State(1, -1, 0);
	Leave_Current_Channel(0x40, true);
	if (MultiplayerMapPreview) {
		delete MultiplayerMapPreview;
		MultiplayerMapPreview = NULL;
	}
	return(result);
}
#endif

void __fastcall Collect_Remote_Player_Names(void) // RA2: 0x0075A6C0, queues a ping request for every user of the channel but its operator and the player (main's text)
{
	User user;
	memset(&user, 0, sizeof(user));
	DebugString("User list is %d long, adding to ping request queue\n", g_UserList.length());
	for (int i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(user, i);
		if (user.flags & 1) {
			DebugString("%s is channel operator, no ping request\n", user.name);
		} else if (!_stricmp((char *)user.name, g_NickName.get())) {
			DebugString("%s is local user, no ping request\n", user.name);
		} else {
			Wstring name((char *)user.name);
			DebugString("Adding %s to ping request queue\n", name.get());
			g_PingQueue.addTail(name);
		}
	}
}

rc_ptr<WorldDominationTour::Campaign> const & __fastcall Current_WDT_Campaign(void); // RA2: 0x00769670, defined in worlddom.cpp
int Get_Performance_Profile(void); // RA2: 0x005355D0, defined in init.cpp

bool __fastcall Request_Match(void) // RA2: 0x0075A920, asks the match bot (the channel operator) for a quick or tour match with the player's country, color, rating, address and ports (ra2-decomp's twin, Yuri's Revenge 0x007A0690)
{
	g_MatchOpponent.clear();
	HWND dialog = WS_Find_Dialog(0x11F);
	if (dialog) {
		WS_Destroy_Dialog(dialog, 0);
	}
	WolOptionsLocked = 0;
	Start_Network_Session();
	if (g_UserList.length() < 2) {
		Set_Match_State(0, -1, 1);
		return(false);
	}
	g_PingQueue.clear();
	g_PingsPending.clear();
	g_MatchPingsReceived = 0;
	Collect_Remote_Player_Names();

	User user;
	memset(&user, 0, sizeof(user));
	int host_index = -1;
	DebugString("User list is %d long, adding to ping request queue\n", g_UserList.length());
	for (int i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(user, i);
		if (user.flags & 1) {
			host_index = i; // RA2: the channel operator, the match bot
		}
	}
	if (host_index == -1) {
		Set_Match_State(0, -1, 1);
		return(false);
	}

	User host;
	memset(&host, 0, sizeof(host));
	g_UserList.get(host, host_index);
	host.next = NULL;

	WOLOptionsClass options;
	options.Load();
	rc_ptr<PlayerProfile> profile;
	if (options.Is_Bit1_Clear()) {
		profile = PlayerProfile::Load(g_NickName);
	} else {
		profile = g_PlayerProfile.Get();
	}
	int country = profile->Country;
	Wstring game("Match ");
	Wstring wdt;

	if (Online_Session_State == 2) { // RA2: a tour match
		game = "MatchWDT ";
		rc_ptr<WorldDominationTour::Campaign> campaign(Current_WDT_Campaign());
		if (campaign.Get() == NULL) {
			wdt = ", WDS=0";
		} else {
			switch (campaign->PlayerFaction) {
				case 2:
					wdt = ", WDS=0";
					if (country > 4) {
						country = -1;
					}
					break;

				case 3:
					wdt = ", WDS=1";
					if (country >= 0 && country <= 4) {
						country = -1;
					}
					break;

				default:
					Set_Match_State(0, -1, 0);
					return(false);
			}
		}
		wdt += ", WDT=";
		wdt += Wstring_FromInt(Session.TournamentTerritory);
	}

	Lobby_Chat_Printf(ColorPriv, StringTable::LoadString("WOL:RequestingMatch", NULL, "D:\\Ra2\\wonline.cpp", 26179));

	Wstring request(game);
	request += "COU=";
	request += Wstring_FromInt(country);
	request += ", COL=";
	request += Wstring_FromInt(profile->Color);

	Wstring nick(g_NickName);
	nick.toLower();
	if (g_UserLocales.contains(nick)) {
		int locale = 0;
		g_UserLocales.getValue(nick, locale);
		request += ", LOC=";
		request += Wstring_FromInt(locale);
	}
	request += ", RAT=";
	request += Wstring_FromInt(Get_Performance_Profile());
	request += ", RES=";
	request += Wstring_FromInt(VideoModeWidth); // RA2: the width of the screen mode

	NetNumType net;
	NetNodeType node;
	unsigned short port;
	Session.LocalAddress.Get_Address(net, node, &port);
	int address = *(int *)node;
	request += ", ADR=";
	request += Wstring_FromInt(address);
	request += ", NAT=";
	request += Wstring_FromInt((unsigned short)FirewallHelper.Detected);
	request += ", PRT=";
	request += Wstring_FromInt(Session.PortBase);
	request += wdt;

	g_pChat->RequestPrivateMessage(&host, request.get());
	Set_Match_State(4, 180, 0);
	Service_Pending_Player_Pings();
	return(true);
}

void Request_All_User_Pings(void) // RA2: 0x0075AF80, asks for a ping of every user of the channel (ra2-decomp's twin, Yuri's Revenge 0x007A0D10)
{
	if (WolOptionsLocked == 0) {
		User user;
		memset(&user, 0, sizeof(user));
		int count = g_UserList.length();
		int handle = 0;
		for (int i = 0; i < count; i++) {
			g_UserList.get(user, i);
			in_addr addr;
			addr.s_addr = user.ipaddr;
			DebugString("Requesting ping for %s at %s\n", user.name, inet_ntoa(addr));
			g_pNetUtil->RequestPing(inet_ntoa(addr), 1000, &handle);
		}
	}
}

void __fastcall Update_User_Slot(User * user) // RA2: 0x0075B040, clears the ping light of the user's slot in the lobby (ra2-decomp's twin, Yuri's Revenge 0x007A0DD0)
{
	if (user) {
		for (int i = 0; i < 8; i++) {
			if (Session.Slots[i] && strcmp(Wide_To_Narrow(Session.Slots[i]->Name), (char *)user->name) == 0) {
				HWND window = GameoptWindow();
				if (window) {
					Set_Player_Ping(window, i, -1);
				}
				return;
			}
		}
	}
}

void __fastcall Store_User_Ping(char const * name, int ping); // RA2: 0x0075B110, below

void Store_Slot_Pings(void) // RA2: 0x0075B0C0, shows the stored ping of every slot again (ra2-decomp's twin, Yuri's Revenge 0x007A0E50)
{
	for (int i = 0; i < 8; i++) {
		if (!Session.Slots[i] || !Is_Real_Player_Entry(Session.Slots[i])) {
			g_PlayerPings[i] = -1;
		}
		char const * name = Wide_To_Narrow(Session.Slots[i]->Name);
		Store_User_Ping(name, g_PlayerPings[i]);
	}
}

void __fastcall Store_User_Ping(char const * name, int ping) // RA2: 0x0075B110, keeps a user's ping for its slot and sets the slot's ping light (ra2-decomp's twin, Yuri's Revenge 0x007A0EA0)
{
	DebugString("Got ping from gameoptions.  name = %s, ping = %d\n", name, ping);
	for (int i = 0; i < 8; i++) {
		if (Session.Slots[i] && strcmp(Wide_To_Narrow(Session.Slots[i]->Name), name) == 0) {
			g_PlayerPings[i] = ping;
			HWND window = GameoptWindow();
			if (window) {
				if (ping < 0) {
					Set_Player_Ping(window, i, -1);
				} else if (ping < 300) {
					Set_Player_Ping(window, i, 0);
				} else if (ping < 500) {
					Set_Player_Ping(window, i, 1);
				} else {
					Set_Player_Ping(window, i, 2);
				}
			}
		}
	}
}

void __fastcall Send_User_Pings(void) // RA2: 0x0075B1B0, the host sends the average ping of every user to the lobby (ra2-decomp's twin, Yuri's Revenge 0x007A0F40)
{
	int count = g_UserList.length();
	User user;
	memset(&user, 0, sizeof(user));
	char list[0x100];
	list[0] = 'P';
	list[1] = (char)(count + '0');
	list[2] = 0;
	if (WolOptionsLocked == 0) {
		for (int i = 0; i < count; i++) {
			g_UserList.get(user, i);
			int ping;
			g_pNetUtil->GetAvgPing(user.ipaddr, &ping);
			char entry[0x80];
			sprintf(entry, ",%s,%d", user.name, ping);
			strcat(list, entry);
			Store_User_Ping((char *)user.name, ping);
		}
		DebugString("Sending pings for users: %s\n", list);
		Send_Host_Option(list);
	}
}

void __fastcall Service_Pending_Player_Pings(void) // RA2: 0x0075B2D0, pings the users queued for the match, ten at a time (ra2-decomp's twin, Yuri's Revenge 0x007A1060)
{
	bool failed = false;
	User user;
	memset(&user, 0, sizeof(user));
	if (WolOptionsLocked == 0) {
		DebugString("Requesting match pings, request list is %d long, pending list is %d long\n", g_PingQueue.length(), g_PingsPending.length());
		if (g_PingQueue.length() != 0) {
			unsigned long ip = user.ipaddr;
			while (g_PingQueue.length() != 0) {
				if (g_PingsPending.length() >= 10 || failed) {
					break;
				}
				{
					Wstring name;
					g_PingQueue.getHead(name);
					int handle;
					int index;
					{
						Wstring key(name.get());
						index = Player_Name_To_Index(key);
					}
					if (index != -1) {
						g_UserList.get(user, index);
						ip = user.ipaddr;
						if (ip != 0) {
							DebugString("Pinging %s\n", name.get());
							in_addr addr;
							addr.s_addr = ip;
							g_pNetUtil->RequestPing(inet_ntoa(addr), 1000, &handle); // RA2: the result is not tested
							if (handle >= 0) {
								g_PingsPending.addTail(name);
								g_PingQueue.removeHead(name);
								DebugString("Removing %s from request list\n", name.get());
							} else {
								failed = true;
							}
						} else {
							g_PingQueue.removeHead(name);
							DebugString("%s ipaddr is zero\n", name.get());
						}
					} else {
						if (g_PingQueue.getHead(name)) {
							g_PingQueue.remove(0);
						}
						DebugString("%s not found in info\n", name.get());
					}
				}
				if (g_PingQueue.length() == 0) {
					break;
				}
			}
		}
	}
}

void __fastcall Send_Pending_Ping_Report(void) // RA2: 0x0075B590, sends the match bot the average pings of the pinged users (ra2-decomp's twin, Yuri's Revenge 0x007A1330)
{
	int count = 0;
	User user;
	memset(&user, 0, sizeof(user));
	if (WolOptionsLocked == 0) {
		Wstring pings("Pings ");
		DebugString("Sending ping list, pending list is %d long\n", g_PingsPending.length());
		if (g_PingsPending.length() != 0) {
			unsigned long ip = user.ipaddr;
			do {
				Wstring name;
				g_PingsPending.remove(name, g_PingsPending.length() - 1);
				DebugString("Removed %s from ping pending list\n", name.get());
				int index;
				{
					Wstring key(name.get());
					index = Player_Name_To_Index(key);
				}
				if (index == -1) {
					DebugString("%s not found in info\n", name.get());
				} else {
					g_UserList.get(user, index);
					ip = user.ipaddr;
					if (ip == 0) {
						DebugString("%s ipaddr is zero\n", name.get());
					} else {
						int ping = -1;
						g_pNetUtil->GetAvgPing(ip, &ping);
						if (ping != -1) {
							DebugString("%s avg ping is %d\n", name.get(), ping);
							pings += name + Wstring(",") + Wstring_FromInt(ping) + Wstring(";");
							count++;
						} else {
							DebugString("Error measuring %s/'s ping\n", name.get());
						}
					}
				}
			} while (g_PingsPending.length() != 0);
		}
		g_MatchPingsReceived = 0;
		if (count > 0) {
			User * host = Find_Owner_User();
			if (host) {
				g_pChat->RequestPrivateMessage(host, pings.get());
				DebugString("Sending ping list : %s\n", pings.get());
			}
		}
	}
}

static inline bool Match_Failed(char const * message) // RA2: the failures the match bot reports (ra2-decomp's helper)
{
	if (strnicmp(message, "Fail", 4) == 0) {
		Lobby_Chat_Printf(0x40FFFF, StringTable::LoadString("WOL:MatchFail", NULL, "D:\\Ra2\\wonline.cpp", 0x6753));
		SetEvent(g_WaitEventHandles[17]);
		return(true);
	}
	if (strnicmp(message, "PingFail", 8) == 0) {
		Lobby_Chat_Printf(0x40FFFF, StringTable::LoadString("WOL:MatchFailPing", NULL, "D:\\Ra2\\wonline.cpp", 0x6759));
		SetEvent(g_WaitEventHandles[17]);
		return(true);
	}
	if (strnicmp(message, "RankFail", 8) == 0) {
		Lobby_Chat_Printf(0x40FFFF, StringTable::LoadString("WOL:MatchFailRank", NULL, "D:\\Ra2\\wonline.cpp", 0x675F));
		SetEvent(g_WaitEventHandles[17]);
		return(true);
	}
	if (strnicmp(message, "WDTSideFail", 11) == 0) {
		Lobby_Chat_Printf(0x40FFFF, StringTable::LoadString("WOL:MatchFailWDTSide", NULL, "D:\\Ra2\\wonline.cpp", 0x6765));
		SetEvent(g_WaitEventHandles[17]);
		return(true);
	}
	return(false);
}

void __fastcall Handle_Matchmaking_Command(char const * message, bool strict) // RA2: 0x0075B910, acts on a message of the match bot (ra2-decomp's twin, Yuri's Revenge 0x007A16B0)
{
	if (strict && Match_Failed(message)) {
		return;
	}
	if (strnicmp(message, "Exit", 4) == 0) {
		Lobby_Chat_Printf(0x40FFFF, StringTable::LoadString("WOL:MatchFailExit", NULL, "D:\\Ra2\\wonline.cpp", 0x676B));
		g_pChat->RequestChannelLeave();
		ODMessageBox(StringTable::LoadString("WOL:MatchFailExit", NULL, "D:\\Ra2\\wonline.cpp", 0x676E), 0, Wait_Callback_Thunk, false);
		SetEvent(g_WaitEventHandles[0]);
		return;
	}
	if (strnicmp(message, "Working", 7) == 0) {
		if (g_MatchState == 4) {
			Set_Match_State(5, 180, 0);
		}
		Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("GUI:MatchingProgress2", NULL, "D:\\Ra2\\wonline.cpp", 0x677A));
		return;
	}
	if (strnicmp(message, "Info ", 5) == 0) {
		Lobby_Chat_Printf(0xFF8080, Narrow_To_Wide(message + 5));
		return;
	}
	if (strnicmp(message, "Pool ", 5) == 0) {
		message += 5;
		Lobby_Chat_Printf(0xFF8080, StringTable::LoadString("WOL:MatchingPlayerPool", NULL, "D:\\Ra2\\wonline.cpp", 0x6784), message);
		return;
	}
	if (strict && strnicmp(message, "Start ", 6) == 0) {
		message += 6;
		strncpy(g_MatchStartText, message, 0xFF);
		DebugString("%s\n", g_MatchStartText);
		SetEvent(g_WaitEventHandles[16]);
		return;
	}
	if (stricmp(message, "ack") == 0 || stricmp(message, "nack") == 0) {
		return;
	}
	if (strnicmp(message, "need ", 5) == 0) {
		DebugString("%s\n", message);
		return;
	}
	if (strict) {
		Lobby_Chat_Printf(0x40FFFF, StringTable::LoadString("WOL:MatchUnknownServerCommand", NULL, "D:\\Ra2\\wonline.cpp", 0x67A5));
	}
}

void __fastcall Report_Match_Player_Ping(void) // RA2: 0x0075BC40, tells the player whom the match was found with and that player's ping (ra2-decomp's twin, Yuri's Revenge 0x007A19E0)
{
	Lobby_Chat_Printf(0xC0FFFF, StringTable::LoadString("WOL:MatchFoundWith", NULL, "D:\\Ra2\\wonline.cpp", 0x67BA), g_MatchOpponent); // RA2: the name goes to the format as the string object itself (its one member, the text pointer)
	Wstring lower = ((Wstring const &)g_MatchOpponent).toLower();
	Ladder * found = NULL; // RA2: the lookup's result is not used
	g_GeneralLadderDict.getPointer(lower, &found);
	int index;
	{
		Wstring key(g_MatchOpponent.get());
		index = Player_Name_To_Index(key);
	}
	if (index == -1) {
		Lobby_Chat_Printf(0xC0FFFF, StringTable::LoadString("WOL:MatchOpponentPingUnknown", NULL, "D:\\Ra2\\wonline.cpp", 0x67C7));
	} else {
		User user;
		memset(&user, 0, sizeof(user));
		g_UserList.get(user, index);
		if (user.ipaddr == 0) {
			Lobby_Chat_Printf(0xC0FFFF, StringTable::LoadString("WOL:MatchOpponentPingUnknown", NULL, "D:\\Ra2\\wonline.cpp", 0x67CE));
		} else {
			int ping = -1;
			g_pNetUtil->GetAvgPing(user.ipaddr, &ping);
			if (ping != -1) {
				Lobby_Chat_Printf(0xC0FFFF, StringTable::LoadString("WOL:MatchOpponentPing", NULL, "D:\\Ra2\\wonline.cpp", 0x67D4), ping);
			} else {
				Lobby_Chat_Printf(0xC0FFFF, StringTable::LoadString("WOL:MatchOpponentPingUnknown", NULL, "D:\\Ra2\\wonline.cpp", 0x67D6));
			}
		}
	}
}

bool __fastcall Start_Match(void) // RA2: 0x0075BE10, the match server named the two players: the first hosts the game, the second looks for it and joins (ra2-decomp's twin, Yuri's Revenge 0x007A1BB0)
{
	WolOptionsLocked = 0;
	Start_Network_Session();
	char buffer[256];
	strncpy(buffer, g_MatchStartText, sizeof(buffer));
	strtok(buffer, ":");
	char * first = strtok(NULL, ",");
	strtok(NULL, ",");
	strtok(NULL, ",");
	strtok(NULL, ",");
	strtok(NULL, ",");
	strtok(NULL, ",");
	char * second = strtok(NULL, ","); // RA2: the second player is the seventh field (the ninth in Yuri's Revenge)
	if (first && second) {
		if (stricmp(first, g_NickName.get()) == 0 || stricmp(second, g_NickName.get()) == 0) {
			if (stricmp(first, g_NickName.get()) == 0) {
				g_MatchOpponent = second;
				Report_Match_Player_Ping();
				Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("WOL:MatchCreatingGame", NULL, "D:\\Ra2\\wonline.cpp", 0x6813));
				g_GameStartRequested = false;
				Set_Match_State(6, 30, 0);
				if (Host_Game()) {
					Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("WOL:MatchWaitingForJoin", NULL, "D:\\Ra2\\wonline.cpp", 0x6817), second);
					return(true);
				}
				Lobby_Chat_Printf(0x20C0FF, StringTable::LoadString("WOL:MatchErrorCreatingGame", NULL, "D:\\Ra2\\wonline.cpp", 0x681E));
				Set_Match_State(2, 30, 0);
			} else {
				g_MatchOpponent = first;
				Report_Match_Player_Ping();
				Wstring game_name = Wstring(first) + Wstring("'s_game");
				Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("WOL:MatchAttemptingToFindGame", NULL, "D:\\Ra2\\wonline.cpp", 0x6825), first);
				Set_Match_State(7, 30, 0);
				int attempts = 1;
				for (;;) {
					if (!Request_Game_Channel_List(0, false) || !Wait_Channel_List()) {
						Lobby_Chat_Printf(0x20C0FF, StringTable::LoadString("WOL:MatchGameChannelNotFound", NULL, "D:\\Ra2\\wonline.cpp", 0x6829), first);
						Set_Match_State(3, 20, 0);
						return(false);
					}
					Channel channel;
					memset(&channel, 0, sizeof(channel));
					for (int i = 0; i < g_GameChannelList.length(); i++) {
						g_GameChannelList.get(channel, i);
						if (stricmp((char *)channel.name, game_name.get()) == 0) {
							if (Join_Game_Channel(&channel, false)) {
								Lobby_Chat_Printf(ColorSystem, StringTable::LoadString("WOL:MatchJoinedGame", NULL, "D:\\Ra2\\wonline.cpp", 0x6833), first);
								Set_Match_State(9, 90, 0);
								return(true);
							}
							Lobby_Chat_Printf(0x20C0FF, StringTable::LoadString("WOL:MatchErrorJoiningGame", NULL, "D:\\Ra2\\wonline.cpp", 0x683B));
							return(false);
						}
					}
					WOL_Wait_Callback();
					Sleep(100);
					if (attempts++ >= 30) {
						Lobby_Chat_Printf(0x20C0FF, StringTable::LoadString("WOL:MatchGameChannelNotFound", NULL, "D:\\Ra2\\wonline.cpp", 0x6843), first);
						Set_Match_State(2, 30, 0);
						break;
					}
				}
			}
		} else {
			Lobby_Chat_Printf(0x20C0FF, StringTable::LoadString("WOL:MatchBadParameters", NULL, "D:\\Ra2\\wonline.cpp", 0x6809));
			Set_Match_State(3, 20, 0);
		}
	} else {
		Lobby_Chat_Printf(0x20C0FF, StringTable::LoadString("WOL:MatchBadParameters", NULL, "D:\\Ra2\\wonline.cpp", 0x67FF));
		Set_Match_State(3, 20, 0);
	}
	return(false);
}

void Add_Chat_Dot(void); // RA2: 0x005C3040, defined in netshare.cpp
bool __fastcall Lobby_Update_Dialog(HWND dialog, int count, bool flag1, bool flag2); // RA2: 0x005CBDB0, defined in netshare.cpp

BOOL CALLBACK Wonline_Waiting_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x0075C350, the waiting dialog (0x117) of the quick match (ra2-decomp's twin, Yuri's Revenge 0x007A2110)
{
	switch (message) {
	case WM_DESTROY:
		KillTimer(window, 0);
		return(FALSE);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(TRUE);
	case WM_ERASEBKGND:
		return(TRUE);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(TRUE);
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case 0x5C0:
			if (g_MatchState == 11) {
				WS_Destroy_Dialog(window, 0);
				return(FALSE);
			}
			SetEvent(g_WaitEventHandles[0]);
			break;
		case 0x708:
			g_pChat->RequestPublicMessage("start 4439,0,0,10000,10,1,1,1,3,Match,1,0,MP18S3.MAP,Match:mshag,4,1,xyzkerble,7,4"); // RA2: a test command
			return(FALSE);
		case 0x709:
			g_pChat->RequestPublicMessage("start 4439,0,0,10000,10,1,1,1,3,Match,1,0,MP18S3.MAP,Match:xyzkerble,7,4,mshag,4,1,");
			return(FALSE);
		}
		break;
	case WM_TIMER:
		if (wparam == 0) {
			Add_Chat_Dot();
			return(FALSE);
		}
		break;
	case OD_SUBCLASSED:
		SetTimer(window, 0, 2000, NULL);
		return(FALSE);
	}
	return(FALSE);
}

void __fastcall Set_Player_Endpoint(char const * name, char const * address_text, char const * firewall_text, char const * port_text) // RA2: 0x0075C470, stores the address, firewall state and port the match server sends for the named player (ra2-decomp's twin, Yuri's Revenge 0x007A2230; main's locals)
{
	if (name && address_text && firewall_text && port_text) {
		unsigned long address = 0;
		unsigned long firewall = 0;
		unsigned long port = 0;
		sscanf(address_text, "%X", &address);
		sscanf(firewall_text, "%X", &firewall);
		sscanf(port_text, "%X", &port);
		for (int i = 0; i < 8; i++) {
			if (stricmp(name, Wide_To_Narrow(Session.Slots[i]->Name)) == 0) {
				NetNodeType node;
				NetNumType net;
				memset(node, 0, sizeof(node));
				memset(net, 0, sizeof(net));
				memcpy(node, &address, 4);
				Session.SlotAddresses[i].Set_Address(net, node, 0);
				Session.WolPlayerState[i] = firewall;
				Session.PlayerPortBase[i] = port;
				break;
			}
		}
	}
}

void __fastcall Apply_Endpoint_Map(char const * text) // RA2: 0x0075C570, takes the two players of a matched game and their endpoints from the match server's command and sets up the random map for them (ra2-decomp's twin, Yuri's Revenge 0x007A2330)
{
	char data[256];
	strcpy(data, text);
	char * name = strtok(data, ":,");
	char * country_text = strtok(NULL, ",");
	int country = atol(country_text);
	char * color_text = strtok(NULL, ",");
	int color = atol(color_text);
	char * address = strtok(NULL, ",");
	char * firewall = strtok(NULL, ",");
	char * port = strtok(NULL, ",");
	Set_Player_Endpoint(name, address, firewall, port);
	Assign_House_And_Color(name, country, color);
	char * name2 = strtok(NULL, ",");
	char * country2_text = strtok(NULL, ",");
	int country2 = atol(country2_text);
	char * color2_text = strtok(NULL, ",");
	int color2 = atol(color2_text);
	char * address2 = strtok(NULL, ",");
	char * firewall2 = strtok(NULL, ",");
	char * port2 = strtok(NULL, ",");
	Set_Player_Endpoint(name2, address2, firewall2, port2);
	Assign_House_And_Color(name2, country2, color2);
	RandomMapGen.SeedData.NumPlayers = 4;
	strcpy(Session.ScenarioFileName, "RandMap.Sed");
	Session.Options.ScenarioIndex = -1;
	HWND dialog = WS_Find_Dialog(0x11F);
	Do_Random_Map(dialog, Wait_Callback_Thunk);
	RandomMapGen.SeedData.Save("RandMap.Sed");
	bool found = false;
	int index;
	for (index = 0; index < Session.Scenarios.Count(); index++) {
		if (Session.Scenarios[index]->Is_Random_Map()) {
			found = true;
			break;
		}
	}
	char * digest = CalcRandomMapDigest();
	if (!found) {
		MultiMission * map = new MultiMission("RandMap.Sed", RandomMapGen.SeedData.MapDescription, digest, true, NULL, 2, 4);
		Session.Scenarios.Add(map);
	} else {
		Session.Scenarios[index]->Set_Digest(digest);
		Session.Scenarios[index]->Set_Description(RandomMapGen.SeedData.MapDescription);
	}
	delete digest;
	if (Set_Scenario_Info_From_Index(index) == true) {
		Session.Options.ScenarioIndex = index;
	}
}

BOOL CALLBACK Wonline_Map_Download_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x0075C830, the map dialog (0x11F) of the quick match: shows the preview and the progress while the map is made (ra2-decomp's twin, Yuri's Revenge 0x007A2600)
{
	BOOL result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) return(result);
	switch (message) {
	case WM_COMMAND:
		if (LOWORD(wparam) == 0x5C0) {
			SetEvent(g_WaitEventHandles[0]);
			WS_Destroy_Dialog(window, 0x5C0);
		}
		break;
	case WM_DESTROY:
		if (MultiplayerMapPreview) {
			delete MultiplayerMapPreview;
			MultiplayerMapPreview = NULL;
		}
		return(FALSE);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		if (MultiplayerMapPreview) {
			MultiplayerMapPreview->Blit_Preview(window);
		} else if (Session.Options.ScenarioIndex == -1 && RandomMapGen.MapPreview) {
			RandomMapGen.MapPreview->Blit_Preview(window);
		}
		if (Progress.Has_Dialog()) Progress.Display_Progress(Point2D(-1, -1));
		ValidateRect(window, NULL);
		return(TRUE);
	case WM_ERASEBKGND:
		return(TRUE);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(TRUE);
	case 0x5C0:
		return(FALSE);
	}
	return(FALSE);
}

void Wol_Send_Start_Reminder(void) // RA2: 0x0075C960, sends the game option "G" through the chat service (ra2-decomp's twin, Yuri's Revenge 0x007A2730)
{
	if (g_pChat) {
		g_pChat->RequestPublicGameOptions("G");
	}
}

void Wol_Handle_Start_Reminder(void) // RA2: 0x0075C980, the joiner is told to accept: makes the ready button available and prints GUI:HostGameStartJoiner (ra2-decomp's twin, Yuri's Revenge 0x007A2750)
{
	HWND dialog = WS_Top_Window();
	HWND list = GetDlgItem(dialog, 0x59F);
	bool not_accepted;
	{
		Wstring empty((char *)NULL);
		not_accepted = GetPlayerAccepted(empty) != 1;
	}
	if (not_accepted) {
		SendMessageA(list, 0x4DC, 0, 1);
		Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostGameStartJoiner", NULL, "D:\\Ra2\\wonline.cpp", 0x695E));
	}
}

void Wol_Reset_Ready(void) // RA2: 0x0075CA00, sends message 0x4DC to the ready button (ra2-decomp's twin, Yuri's Revenge 0x007A27D0)
{
	HWND dialog = WS_Top_Window();
	HWND list = GetDlgItem(dialog, 0x59F);
	SendMessageA(list, 0x4DC, 0, 0);
}

bool __fastcall Wol_Update_Lobby_Count(int count, bool flag1, bool flag2) // RA2: 0x0075CA30, hands the count to Lobby_Update_Dialog of the options dialog (ra2-decomp's twin, Yuri's Revenge 0x007A2800)
{
	HWND dialog = WS_Find_Dialog(0xC2);
	return(Lobby_Update_Dialog(dialog, count, flag1, flag2));
}

void __fastcall Kick_Channel_User(Wstring const & name) // RA2: 0x0075CA60, looks the channel user up by name and has the chat service remove it (ra2-decomp's twin, Yuri's Revenge 0x007A2830)
{
	User user;
	user.flags = 0;
	for (int i = 0; i < g_UserList.length(); i++) {
		g_UserList.get(user, i);
		if (stricmp(name.text(), (char *)user.name) == 0) {
			g_pChat->RequestUserKick(&user);
			return;
		}
	}
}

void __fastcall Kick_Channel_User(wchar_t const * name) // RA2: 0x0075CAE0, the same for a wide name (ra2-decomp's twin, Yuri's Revenge 0x007A28B0)
{
	if (name) {
		User user;
		Wstring narrow = Wstring_Narrow(name);
		user.flags = 0;
		for (int i = 0; i < g_UserList.length(); i++) {
			g_UserList.get(user, i);
			if (stricmp(narrow.text(), (char *)user.name) == 0) {
				g_pChat->RequestUserKick(&user);
				break;
			}
		}
	}
}

BOOL CALLBACK Add_Match_Buddy_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x0075CC10, below

void Open_Buddy_Dialog(void) // RA2: 0x0075CBA0, after a match offers to make the opponent a buddy (ra2-decomp's twin, Yuri's Revenge 0x007A2970)
{
	bool lost = g_ConnectionLost;
	g_PlayingNetGame = false;
	if (!lost && WOL_Get_Add_Match_Buddies() && Can_Add_Buddy(g_MatchOpponent)) {
		SimpleWonlineDialogControl control;
		OwnerDraw::Controlled_Dialog(0x122, (DLGPROC)Add_Match_Buddy_Dialog_Proc, &control);
	}
}

BOOL CALLBACK Add_Match_Buddy_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x0075CC10, asks whether the match opponent becomes a buddy (0x122) (ra2-decomp's twin, Yuri's Revenge 0x007A29E0)
{
	if (message == WM_INITDIALOG && lparam) {
		SetWindowLongA(window, 8, ((int *)lparam)[1]);
	}
	BOOL result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) return(result);
	SimpleWonlineDialogControl * control = (SimpleWonlineDialogControl *)GetWindowLongA(window, 8);
	wchar_t buffer[128];
	switch (message) {
	case WM_ERASEBKGND:
		return(TRUE);
	case WM_PAINT:
		Dialog_On_WM_PAINT(window);
		Slide_Pending_Dialog(window);
		ValidateRect(window, NULL);
		return(TRUE);
	case WM_DRAWITEM:
		Dialog_On_WM_DRAWITEM(lparam);
		return(TRUE);
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			{
				bool checked = IsDlgButtonChecked(window, 0x6F9) == 1;
				WOL_Set_Add_Match_Buddies(!checked);
			}
				Request_Add_Buddy(g_MatchOpponent);
				control->Set_Window((unsigned)window, (HWND)IDOK);
			return(FALSE);
		case IDCANCEL:
			{
				bool checked = IsDlgButtonChecked(window, 0x6F9) == 1;
				WOL_Set_Add_Match_Buddies(!checked);
			}
				control->Set_Window((unsigned)window, (HWND)IDCANCEL);
			return(FALSE);
		}
		break;
	case OD_SUBCLASSED:
		{
		swprintf(buffer, StringTable::LoadString("WOL:AddBuddyQuery", NULL, "D:\\Ra2\\wonline.cpp", 0x69B7), g_MatchOpponent.get());
		HWND item = GetDlgItem(window, 0x71B);
		SendMessageA(item, 0x4B2, 0, (LPARAM)buffer);
		}
		return(FALSE);
	}
	return(FALSE);
}
