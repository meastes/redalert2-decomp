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

#pragma once

#include "wstring.h"

template <class T>
class ArrayList;
class WideWstring; // RA2: the wide chat text

enum WonlineResult
{
	WONLINE_DOWNLOAD_PATCH = -1,
	WONLINE_BACK = 0,
	WONLINE_OK = 1,
};

extern int g_BuddyColumns[6]; // RA2: 0x007FD308, the column positions of the buddy list

extern Wstring g_NickName; // RA2: a Wstring, the persona of the login (0x00B26CB8)

extern char WestwoodOnline_Clan1_Players[];
extern char WestwoodOnline_Clan2_Players[];

extern int WestwoodOnline_StartTime;
extern int WestwoodOnline_Tournament;
extern int WestwoodOnline_GameID;
extern int WestwoodOnline_GameSKU_TS;
extern int WestwoodOnline_GameSKU_FS;
extern int WestwoodOnline_GameSKU_WDT;
extern char WestwoodOnline_LoginName[];
extern char WestwoodOnline_UserName[];

extern int g_SuspendChatPump;
extern int g_PingsSent;
extern int g_PingsReceived;

extern int g_MaxPlayers;

extern char g_GameServerHost[128];
extern int g_GameServerPort;
extern char g_LadderAddress[]; // RA2: 0x00B29018, the address of the ladder server (empty when there is none)
extern char LastFindName[]; // RA2: 0x00B29CD8, the nick the last /find command asked for
extern int g_LadderPort; // RA2: 0x00B2A8AC, the port of the ladder server
extern wchar_t * g_TerritoryName; // RA2: 0x00B28CC8, the name of the WDT territory of this player (a copy made by _wcsdup)


void DoFindPage(void);
int Player_Name_To_Index(Wstring const & name); // RA2: 0x0073C2E0, the name is a Wstring
void ViewHTML(const char * name, int no_ask);
bool Switch_Server(int type);
void Read_WOL_Settings(void);
void Write_WOL_Settings(void);
void Apply_WOL_Settings(void);
int Send_Chat_Message(WideWstring const & msg); // RA2: 0x0073CA50, the text is wide
int Send_Chat_Action(WideWstring const & msg); // RA2: 0x0073CB80, the text is wide
void Encode_Game_Options(char *out);
int GetINIHash(void);
void Encode_Channel_ExInfo(char *out);
void SetPlayerAccepted(Wstring const & who, int status); // RA2: 0x0073D150, the name is a Wstring
int GetPlayerAccepted(Wstring const & who); // RA2: 0x0073D3F0, the name is a Wstring
int Assign_House_And_Color(Wstring const & who, int house, int color); // RA2: 0x0073D550, the name is a Wstring
void GoBack(void);
bool WOL_Wait_Callback(void);
void Reset_WOL_Globals(void);
void Draw_Channel_List(void);
void Display_Users(void);
int Startup_Chat(HINSTANCE hInstance);
void Handle_User_Leave(struct User & user);
void Logout_WOnline(void);
void Shutdown_Chat(void);
bool Get_WDT_State_Silent(void); // RA2: 0x00742270, returns whether the state was asked for
WonlineResult Login_WOL(void);
int Join_WOL_Lobby(HWND win);
int Pick_Lobby(void);
int Join_Lobby(void);
void Set_Wait_Dialog_Text(char *status_text);
void Close_Wait_Window(unsigned int event);
void Fill_Session_Players(struct User * users);
bool Is_Channel_Owner(char *name);
User & Get_Channel_Host(void);
void Sync_Scenario_With_Guests(void);
bool Handle_Preview_Download(void);
void Poke_The_Host(void);
void Init_WDT(void);
int Select_WDT_Server(void);
int Select_WDT_Location(HWND window);
int Request_WDT_Server_List(void);
bool Request_WDT_Cycle(void);
void Init_WDT_Sounds(void);
void Deinit_WDT_Sounds(void);
void Play_WDT_Sound(ArrayList<Wstring> list);

// Declarations moved from the units that used them (one declaration per name).
extern bool Is_Current_Channel_Tournament(void); // RA2: 0x0073C370 (wonline.cpp)
