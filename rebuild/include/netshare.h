// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

#include "dict.h"
#include "globals.h"
#include "preview.h"
#include "wstring.h"
#include "widewstring.h"

class HouseClass;

int ODMessageBox(const wchar_t *text, int type, bool (*callback)(void), bool large = false);
int ODMessageBox(const char *text, int type, bool (*callback)(void), bool large = false);
int CALLBACK ODMessageBox_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

bool Set_Scenario_Info_From_Index(int index);
void PregameSetup(void);
void Update_Network_Dialog_Preview(HWND win);
void Receive_Random_Map_Preview(void);
void __fastcall Send_Map_Preview(MapPreviewClass * preview); // RA2: 0x005C7EF0, replaces TibSun's Send_Preview_To_Guests(void); named so by netdlg2.cpp
int CountAliveTeams(HouseClass * house);

int RandomMapWaypointCount(int index);
int Scenario_Dialog(HWND hWndParent);

extern struct IChat *g_pChat;
extern struct INetUtil * g_pNetUtil;
extern struct IDownload * g_pDownload;

unsigned int Wstring_Hash(Wstring & string);
unsigned int Wstring_Hash(WideWstring & string);

void __cdecl Lobby_Chat_Printf(int color, const wchar_t * fmt, ...); // RA2: 0x005C2C80, named so by netdlg2.cpp
void __cdecl SMessagePrintf(int color, const wchar_t * fmt, ...);

void _DrawMessage(int color, const wchar_t * msg, HWND window);
void _SetMessageString(HWND window, const wchar_t * msg, int len, int color);

HWND GameoptWindow(void);

void __fastcall Send_Lobby_Options(bool force, bool now = false); // RA2: 0x005C3810, named so by netdlg2.cpp
bool __fastcall Net2DecodeGameopt(char const * options, char const * name); // RA2: 0x005C3FF0, replaces TibSun's DecodePubGameopt; named so by netdlg2.cpp; returns true when the scenario changed
void __fastcall Send_Host_Option(char * options); // RA2: 0x005C3C60, named so by netdlg2.cpp
void __fastcall Send_Player_Choice(char const * player, char const * options); // RA2: 0x005C3D90, named so by netdlg2.cpp
void __fastcall Fill_Options_Dialog(HWND window, int initialize); // RA2: 0x005C3550, named so by netdlg2.cpp

void LBSaveSelections(HWND win, Dictionary<WideWstring,bool> & lbdict);
void LBRestoreSelections(HWND win, Dictionary<WideWstring,bool> & lbdict);

char * CalcRandomMapDigest(void);
int CreateRandomMap(void);

extern COLORREF PlayerColorTable[MAX_PLAYERS];

extern const COLORREF ColorSystem;
extern const COLORREF ColorUser;
extern const COLORREF ColorPriv;
extern const COLORREF ColorPrivAction;
extern const COLORREF ColorAction;
extern const COLORREF ColorOp;
extern const COLORREF ColorPaged;
extern const COLORREF ColorMe;
extern const COLORREF ColorNoJoin;

extern MapPreviewClass *MultiplayerMapPreview;

extern bool IsRandomMap;

extern int IsColorChangePending;

// Declarations moved from the units that used them (one declaration per name).
extern wchar_t const * LobbyEntryTexts[7]; // RA2: 0x00A72D0C, the names of the fixed lobby entries
struct NodeNameType;
extern NodeNameType * LobbyEntries[7]; // RA2: 0x00A72D2C, the fixed lobby entries; entry 6 is the player entry (netshare.cpp)
