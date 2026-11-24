/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "netshare.h"
#include "bitfont.h" // RA2: the lobby messages are measured with BitFont
#include "multiplayer.h" // RA2: MultiplayerGameMode

#include "_rules.h"
#include "cd.h"
#include "conquer.h"
#include "data.h"
#include "dbgprint.h"
#include "globals.h"
#include "firewall.h" // RA2: FirewallHelperClass
#include "goptions.h"
#include "ipxmgr.h"
#include "language\language.h"
#include "lzopipe.h"
#include "lzostraw.h"
#include "mapgen.h"
#include "modemhst.h"
#include "mplayer.h" // RA2: Clear_Vector
#include "srfcache.h" // RA2: the images of the slots
#include "msgbox.h"
#include "netdlg.h"
#include "netdlg2.h"
#include "newmenu.h"
#include "nulldlg.h"
#include "nullmgr.h"
#include "ownrdraw.h"
#include "rules.h"
#include "scenario.h"
#include "sendfile.h"
#include "session.h"
#include "stimer.h"
#include "swap.h" // RA2: Change_Slot_Type exchanges the chosen and the seated entry (ra2-decomp's banked text)
#include "wdtnet.h"
#include "windlg.h"
#include "wolapi.h" // RA2: main has no include/wolapi/; the part of the WOL API this file uses is one flat header
#include "wonline.h"
#include "worlddom.h"
#include "wstring.h"
#include "xpipe.h"
#include "xstraw.h"

#include <ctime>
#include <wchar.h> // RA2: the wide string functions the lobby messages use
#undef LoadString // RA2: windows.h maps it to LoadStringA
#include "stringtable.h" // RA2: the messages are loaded from the string table

// RA2: callees that are not in a header yet
LRESULT __fastcall Get_List_Item_Wide_Text(HWND window, WPARAM index, WideWstring & text); // RA2: 0x00600F10, the text of a list box item
void __fastcall Dialog_On_WM_PAINT(HWND dialog); // RA2: 0x005FEE70, defined in ownrdraw.cpp
void __fastcall Dialog_On_WM_DRAWITEM(long item); // RA2: as above
void __fastcall Show_Dialog(HWND dialog, int kind); // RA2: as above
void __fastcall Move_Default_Button_Owner(HWND window); // RA2: as above
void __fastcall Slide_Pending_Dialog(HWND window); // RA2: 0x005E5F20, defined in ownrdraw.cpp
MultiplayerGameMode * First_Game_Mode(void); // RA2: 0x005B6EC0, defined in the game mode list
int Net2SetHouseAndColor(char const * who, int house, int color); // RA2: 0x005BCCF0, defined in netdlg2.cpp
rc_ptr<WDTTerritory> Find_WDT_Territory(int index); // RA2: 0x00769690, the tour's territory with this index (defined in worlddom.cpp)
char * __fastcall Wide_To_Narrow(wchar_t const * text); // RA2: 0x006FA120, static buffer
wchar_t * __fastcall Narrow_To_Wide(char const * narrow); // RA2: 0x006FA180, static buffer
unsigned int __fastcall Compute_Name_CRC(wchar_t const * name); // RA2: 0x005BBA50, the wide form of netdlg.h's
void Join_Lobby_Start_Reminder(void); // RA2: 0x005C28E0, defined in netdlg2.cpp
void __cdecl Join_Lobby_Reset_Ready(void); // RA2: 0x005C2940, defined in netdlg2.cpp
void Wol_Send_Start_Reminder(void); // RA2: 0x0075C960, sends the game option "G" through the chat service (the counterpart of Send_Start_Reminder)
void Wol_Handle_Start_Reminder(void); // RA2: 0x0075C980, the joiner is told to accept: makes the ready button available and prints GUI:HostGameStartJoiner
void Wol_Reset_Ready(void); // RA2: 0x0075CA00, sends message 0x4DC to the ready button (the counterpart of Join_Lobby_Reset_Ready)

bool __fastcall Local_User_Is_Channel_Owner(void); // RA2: 0x0074D960, the local user owns the channel (WOL)
void __fastcall Register_Connection_State(char const * sender, char const * receiver, int state); // RA2: 0x0074E1F0, the sender reports its connection state to the receiver (WOL)
void __fastcall Store_Player_Port(char const * name, unsigned short port); // RA2: 0x0074E2D0, the port a player reported (WOL)
void __fastcall Leave_Channel_With_Message(HWND dialog); // RA2: 0x0074C5F0, the joiner has no slot any more (WOL)
void __fastcall Store_User_Ping(char const * name, int ping); // RA2: 0x0075B110, a user's ping (WOL)
void __fastcall SetPlayerAccepted(Wstring const & name, int status); // RA2: 0x0073D150, sets a player's accept status through the chat service (the counterpart of Net2SetAccept)
int __fastcall GetPlayerAccepted(Wstring const & name); // RA2: 0x0073D3F0, a player's accept status (the counterpart of Net2GetAccept)
int __fastcall Assign_House_And_Color(Wstring const & name, int country, int color); // RA2: 0x0073D550, the counterpart of Net2SetHouseAndColor
bool Is_Tournament_Session(void); // RA2: 0x0073BDB0, Online_Session_State (Session.TournamentState) is 1 or 2
void Redraw_Player_List(void); // RA2: 0x007412F0, defined in wonline.cpp
void Draw_Player_List(int keep_selection, bool force); // RA2: 0x0073FEE0, redraws the chat service's player list (the counterpart of Net2DisplayUsers)
bool __fastcall Is_Real_Player_Entry(NodeNameType * entry); // RA2: 0x005C9320, defined below
Wstring __fastcall Lobby_Entry_Name(NodeNameType * entry); // RA2: 0x005CD0E0, the lobby's name for a fixed entry
void __fastcall Fill_Observer_Combo(HWND dialog, int id); // RA2: 0x004D1640, as netdlg2.cpp
void __fastcall Fill_Random_Color_Combo(HWND dialog, int id); // RA2: 0x004D2250
void __fastcall Select_Color_Combo(HWND dialog, int id, int color); // RA2: 0x004D2480
void __fastcall Fill_Side_Combo(HWND dialog, int id); // RA2: 0x004D1560
void __fastcall Fill_Color_Combo(HWND dialog, int id); // RA2: 0x004D2080
int __fastcall Player_Side_Combo_ID(int player); // RA2: 0x004D1350
int __fastcall Player_Color_Combo_ID(int player); // RA2: 0x004D1CB0
void __fastcall Select_Player_Side(HWND dialog, int player); // RA2: 0x004D1820
void __fastcall Select_Player_Color(HWND dialog, int player); // RA2: 0x004D23C0
void __fastcall Lock_Slots(HWND dialog, int slot); // RA2: defined below at 005C9DA0.
void __fastcall Update_Slot_Controls(HWND dialog, int slot, bool flag); // RA2: 0x005C9B90
void __fastcall Show_Observer_Slot_Controls(HWND dialog); // RA2: 0x005CAFD0, defined below
void __fastcall Change_Slot_Count(HWND dialog, int old_count, int count, MultiplayerGameMode * mode); // RA2: 0x005CB300
NodeNameType * __fastcall Find_Lobby_Name(wchar_t const * name); // RA2: 0x005C8B20, the fixed entry with this name
NodeNameType * __fastcall Find_Lobby_Entry_By_Name(WideWstring const & name); // RA2: 0x005CD1B0
void __fastcall Set_Slot_Names(wchar_t const * list); // RA2: 0x005C8DD0, as netdlg2.cpp
int __fastcall Side_Combo_Data(HWND dialog, int id, int index); // RA2: 0x004D1C70, as netdlg2.cpp
int __fastcall Side_Combo_Player(int id); // RA2: 0x004D13B0, as netdlg2.cpp
int __fastcall Color_Combo_Player(int id); // RA2: 0x004D1D10, as netdlg2.cpp
int __fastcall Color_Combo_Data(HWND dialog, int id, int index); // RA2: 0x004D2900, as netdlg2.cpp
void __fastcall Select_Side_Combo(HWND dialog, int id, int side); // RA2: 0x004D1A90, selects the side with this item data in a side combo box
void __fastcall Color_Combo_Changed(HWND dialog, int id); // RA2: 0x004D2700, a color combo box was changed
Surface * __fastcall Country_Flag(int country); // RA2: 0x004D1120, the flag picture of a country
namespace OwnerDraw { void __fastcall Set_Control_Image(HWND control, Surface * image); } // RA2: 0x005E1F30, as ownrdraw.cpp
void Store_Slot_Pings(void); // RA2: 0x0075B0C0, defined in wonline.cpp
int __fastcall Slot_Country(int slot); // RA2: 0x005CA3F0, defined below
int __fastcall Slot_Color(int slot); // RA2: 0x005CA480, defined below
int __fastcall Slot_Color_Control_Id(int slot); // RA2: 0x005CA4E0, defined below
int __fastcall Is_Slot_Accepted(int slot); // RA2: 0x005CA3B0, defined below
void __fastcall Refresh_Slot_Colors(HWND dialog); // RA2: 0x005CA540, defined below
void __fastcall Refresh_Slot_Combos(HWND dialog); // RA2: 0x005CA6C0, defined below
int __fastcall Player_Control_ID(int player); // RA2: 0x004D0EE0, the id of the flag control of a player
void __fastcall Set_Player_Map_State(char const * name, int state); // RA2: 0x005C3F00, defined above
Wstring __fastcall Wstring_FromWide(WideWstring const & text); // RA2: 0x00771450, as netdlg2.cpp
void Join_Lobby_Start_Reminder(void); // RA2: 0x005C28E0

void __fastcall Char_To_Wide(wchar_t * dest, char const * source, int limit); // RA2: 0x006FA0C0, widens a narrow string
#define OD_GETTEXTW (WM_USER + 182) // RA2: 0x4B6, a list box item's wide text (Get_List_Item_Wide_Text)
#define OD_GETTEXTA (WM_USER + 183) // RA2: 0x4B7, a list box item's narrow text (Get_List_Item_Text)
bool Is_Current_Channel_Tournament(void); // RA2: 0x0073C370, an Internet game in a restricted channel (WOL)
bool Is_Current_Channel_Flag_8(void); // RA2: 0x0073C3A0, an Internet game in a channel that takes one player less (WOL)
bool __fastcall Update_Lobby_Count(int count, bool flag1, bool flag2); // RA2: 0x005BC430, defined in netdlg2.cpp
bool __fastcall Wol_Update_Lobby_Count(int count, bool flag1, bool flag2); // RA2: 0x0075CA30, hands the count to Lobby_Update_Dialog of the options dialog (the counterpart of Update_Lobby_Count)
bool __fastcall Skirmish_Has_Computer_After(int player); // RA2: 0x00682930
User * Find_Owner_User(void); // RA2: 0x0074DA60, the channel user that owns the channel, NULL if none (WOL)
IChat * g_pChat;
INetUtil * g_pNetUtil;
IDownload * g_pDownload;

const COLORREF ColorSystem     = RGB(255, 255, 255)|(255<<24);  /// 0xFFFFFFFF
const COLORREF ColorUser       = RGB(240, 240, 0);              /// 0x0000F0F0
const COLORREF ColorPriv       = RGB(128, 128, 255);            /// 0x00FF8080
const COLORREF ColorPrivAction = RGB(255, 0, 255);              /// 0x00FF00FF
const COLORREF ColorAction     = RGB(255, 80, 48);              /// 0x003050FF
const COLORREF ColorOp         = RGB(0, 255, 255);              /// 0x00FFFF00
const COLORREF ColorPaged      = RGB(255, 255, 255);            /// 0x00FFFFFF
const COLORREF ColorMe         = RGB(255, 255, 64);             /// 0x0040FFFF
const COLORREF ColorNoJoin     = RGB(128, 128, 128);            /// 0x00808080

COLORREF PlayerColorTable[9] = { // RA2: 0x007E6D94, the nine player colors (GDlgSupp.cpp's Gold..Gray abbreviations), not TibSun's eight
	RGB(221, 226, 13),	/// 0x000DE2DD, Gold
	RGB(255, 25, 25),	/// 0x001919FF, Red
	RGB(42, 116, 226),	/// 0x00E2742A, Blue
	RGB(62, 209, 46),	/// 0x002ED13E, Green
	RGB(255, 160, 25),	/// 0x0019A0FF, Orange
	RGB(50, 215, 230),	/// 0x00E6D732, SkyBlue
	RGB(149, 40, 189),	/// 0x00BD2895, Purple
	RGB(255, 154, 235),	/// 0x00EB9AFF, Pink
	RGB(96, 96, 96)	/// 0x00606060, Gray
};


bool IsRandomMap = true;

MapPreviewClass *MultiplayerMapPreview;


/// <summary>
/// Computes a hash value for a string.
/// This routine supplies the bucket value for the dictionaries that the multiplayer dialogs
/// key by name.
/// </summary>
/// <param name="string">The string to hash.</param>
/// <returns>Returns with the hash value of the string.</returns>
unsigned int Wstring_Hash(Wstring & string)
{
	unsigned int hash = 0;

	hash = string.length();

	for (unsigned int i = 0; i < string.length(); i++) {
		hash += *(string.get() + i);
		hash += i;
		hash = (hash << 8) ^ (hash >> 24);
	}
	return(hash);
}

/// <summary>
/// Computes a hash value for a wide string.
/// This routine supplies the bucket value for the dictionaries that key by a wide name.
/// </summary>
/// <param name="string">The string to hash.</param>
/// <returns>Returns with the hash value of the string.</returns>
unsigned int Wstring_Hash(WideWstring & string) // RA2: 0x005C2BF0, the wide string class has a hash of its own; the character and the index are added in one expression
{
	unsigned int hash = 0;

	hash = string.length();

	for (unsigned int i = 0; i < string.length(); i++) {
		hash += *(string.get() + i) + i; // RA2: as above
		hash = (hash << 8) ^ (hash >> 24);
	}
	return(hash);
}



/// <summary>
/// Fetches the game options dialog that is currently up.
/// The same options are presented by four different dialogs depending on how the game was
/// started. Use this routine rather than trying to remember which one the player is looking
/// at.
/// </summary>
/// <returns>Returns with the handle of the open game options dialog. NULL is returned if
/// none of them is up.</returns>
HWND GameoptWindow(void)
{
	HWND dialog;

	dialog = WS_Find_Dialog(IDD_WOL_GAMEOPT);
	if (dialog) {
		return(dialog);
	}
	dialog = WS_Find_Dialog(IDD_WOL_GUEST);
	if (dialog) {
		return(dialog);
	}
	dialog = WS_Find_Dialog(IDD_MPLAYER_HOST);
	if (dialog) {
		return(dialog);
	}
	dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST);
	if (dialog) {
		return(dialog);
	}
	return(0);
}


/// <summary>
/// Prints a formatted chat message to the player.
/// This routine hunts down the topmost dialog that has somewhere to show public and private
/// messages and puts the text there, so the caller does not have to know which dialog the
/// player is looking at. If no dialog wants chat messages, the message is quietly dropped.
/// </summary>
/// <param name="color">Color to display the message in, or -1 for the default.</param>
/// <param name="fmt">Printf style format string for the message.</param>
void __cdecl Lobby_Chat_Printf(int color, const wchar_t * fmt, ...) // RA2: 0x005C2C80, the line is built from a wide format into a wide buffer; main names it Lobby_Chat_Printf (netdlg2.cpp)
{
	va_list va;
	wchar_t buffer[1024]; // RA2: as above
	memset(buffer, 0, sizeof(buffer));

	va_start(va, fmt);
	vswprintf(buffer, fmt, va); // RA2: as above
	va_end(va);

	if (WS_Top_Window() != 0) {
		HWND top = WS_Top_Window();
		HWND msg = GetDlgItem(top, IDC_PMESSAGES);
		while (msg == 0) {
			top = WS_Next_Lower_Dialog(top);
			if (top == 0) {
				msg = 0;
				break;
			}
			msg = GetDlgItem(top, IDC_PMESSAGES);
		}

		if (msg != 0) {
			_DrawMessage(color, buffer, msg);
		}
	}
}



/// <summary>
/// Prints a formatted system message to the player.
/// This routine hunts down the topmost dialog that has somewhere to show system messages and
/// puts the text there, so the caller does not have to know which dialog the player is
/// looking at. If no dialog wants system messages, the message is quietly dropped.
/// </summary>
/// <param name="color">Color to display the message in, or -1 for the default.</param>
/// <param name="fmt">Printf style format string for the message.</param>
void __cdecl SMessagePrintf(int color, const wchar_t * fmt, ...) // RA2: 0x005C2D10, the line is built from a wide format into a wide buffer
{
	va_list va;
	wchar_t buffer[1024]; // RA2: as above
	memset(buffer, 0, sizeof(buffer));

	va_start(va, fmt);
	vswprintf(buffer, fmt, va); // RA2: as above
	va_end(va);

	if (WS_Top_Window() != 0) {
		HWND top = WS_Top_Window();
		HWND msg = GetDlgItem(top, IDC_SMESSAGES);
		while (msg == 0) {
			top = WS_Next_Lower_Dialog(top);
			if (top == 0) {
				msg = 0;
				break;
			}
			msg = GetDlgItem(top, IDC_SMESSAGES);
		}

		if (msg != 0) {
			_DrawMessage(color, buffer, msg);
		}
	}
}



/// <summary>
/// Draws a message into a message list box.
/// This routine word wraps the message to the width of the list box and adds each resulting
/// line as its own entry, so that a long chat message stays readable. Embedded newlines
/// break the text as well.
/// </summary>
/// <param name="color">Color to display the message in, or -1 for the list box
/// default.</param>
/// <param name="message">The text to display.</param>
/// <param name="window">The message list box to display the text in.</param>
void _DrawMessage(int color, const wchar_t * message, HWND window) // RA2: 0x005C2DA0, the text is wide and measured with the BitFont, and the leading blanks of a wrapped line are skipped
{
	RECT rect;
	struct { HDC hdc; SIZE size; } l; // RA2: as above

	int length = wcslen(message); // RA2: as above

	int offset = 18;
	Get_Display_Rect(window, &rect);
	if (SendMessage(window, OD_HASATTACHED, 0, 0)) {
		offset = 1;
	}

	l.hdc = GetDC(window); // RA2: as above
	SendMessage(window, OD_RESTOREDC, 0, (LPARAM)l.hdc); // RA2: as above

	while (length) {
		if (message != NULL) {
			wchar_t const * newline = wcschr(message, L'\n'); // RA2: as above
			if (newline != NULL) {
				int linelen = newline - message + 1;
				if (length >= linelen) {
					length = linelen;
				}
			}
		}

		wchar_t * copy = _wcsdup(message); // RA2: as above
		copy[length] = 0; // RA2: as above
		BitFont::Instance->Text_Dimensions(copy, (int *)&l.size.cx, (int *)&l.size.cy, 0); // RA2: as above
		free(copy); // RA2: as above
		int maxWidth = rect.right - rect.left - offset;

		if (l.size.cx >= maxWidth - 4) { // RA2: as above
			int reduceBy;
			if (l.size.cx / 2 > maxWidth) { // RA2: as above
				reduceBy = 10;
				length -= reduceBy;
			} else {
				reduceBy = 1;
			}

			int found = -1;
			int idx = length - 1;

			while (idx > 0) {
				if (!iswgraph(message[idx])) { // RA2: as above
					found = idx;
					break;
				}
				idx--;
			}

			if (found == -1) {
				length -= reduceBy;
				found = length;
			}
			length = found;
		} else {
			_SetMessageString(window, message, length, color);
			message += length;
			while (*message == L' ') { // RA2: as above
				message++; // RA2: as above
			}
			length = wcslen(message); // RA2: as above
		}
	}

	ReleaseDC(window, l.hdc); // RA2: as above
}



/// <summary>
/// Adds a single line of text to a message list box.
/// This is the low level routine that _DrawMessage uses once it has decided where the text
/// should break. The list box is capped, so a long game does not pile up messages without
/// limit.
/// </summary>
/// <param name="window">The message list box to add the line to.</param>
/// <param name="message">The text to add; only the leading characters are taken.</param>
/// <param name="length">Number of characters of the message to add.</param>
/// <param name="color">Color to display the line in, or -1 for the list box default.</param>
void _SetMessageString(HWND window, const wchar_t * message, int length, int color) // RA2: 0x005C2F20, the text is wide; the list only scrolls to the new line when it was showing its end
{
	static wchar_t buffer[512]; // RA2: as above
	memset(buffer, 0, sizeof(buffer));
	wcsncpy(buffer, message, length); // RA2: as above
	wchar_t * line_end = wcschr(buffer, L'\r'); // RA2: as above
	if (line_end != NULL) {
		line_end[0] = L'\0'; // RA2: as above
	} else {
		line_end = wcschr(buffer, L'\n'); // RA2: as above
		if (line_end != NULL) {
			line_end[0] = L'\0'; // RA2: as above
		}
	}

	int old = SendMessage(window, OD_DISABLEPAINT, 0, 1);
	int topindex = SendMessage(window, LB_GETTOPINDEX, 0, 0); // RA2: as above
	int count = SendMessage(window, LB_GETCOUNT, 0, 0); // RA2: as above
	int itemheight = SendMessage(window, LB_GETITEMHEIGHT, topindex, 0); // RA2: as above
	RECT rect;
	GetWindowRect(window, &rect); // RA2: as above
	bool atbottom = (count - topindex) <= (rect.bottom - rect.top) / itemheight; // RA2: as above
	if (count > 500) { // RA2: as above
		SendMessage(window, LB_DELETESTRING, 0, 0); // RA2: as above
		count--; // RA2: as above
	}

	int index = SendMessage(window, OD_ADDSTRINGW, 0, (LPARAM)buffer); // RA2: as above
	if (color != -1) {
		SendMessage(window, OD_SETCOLOR, index, color);
	}

	if (atbottom) { // RA2: as above
		SendMessage(window, LB_SETTOPINDEX, count, 0); // RA2: as above
	}
	SendMessage(window, OD_DISABLEPAINT, 0, old);
}


/// <summary>
/// Adds a dot to the last chat line.
/// This routine finds the chat list of the topmost dialog and appends a dot to its last line. Once
/// that line is long enough the dot starts a new line, so a long wait shows progress.
/// </summary>
void Add_Chat_Dot(void) // RA2: 0x005C3040, TibSun has no such routine
{
	if (WS_Top_Window() != 0) {
		HWND top = WS_Top_Window();
		HWND msg = GetDlgItem(top, IDC_PMESSAGES);
		while (msg == 0) {
			top = WS_Next_Lower_Dialog(top);
			if (top == 0) {
				return;
			}
			msg = GetDlgItem(top, IDC_PMESSAGES);
		}

		int old = SendMessage(msg, OD_DISABLEPAINT, 0, 1); // RA2: as above
		int count = SendMessage(msg, LB_GETCOUNT, 0, 0); // RA2: as above
		WideWstring text; // RA2: as above
		if (count > 0) { // RA2: as above
			Get_List_Item_Wide_Text(msg, count - 1, text); // RA2: as above
		}
		if (text.length() < 0x24) { // RA2: as above
			SendMessage(msg, LB_DELETESTRING, count - 1, 0); // RA2: as above
			text += L"."; // RA2: as above
		} else {
			text = L"."; // RA2: as above
			count++;
		}
		SendMessage(msg, OD_ADDSTRINGW, 0, (LPARAM)text.text()); // RA2: as above
		while (count > 500) { // RA2: as above
			SendMessage(msg, LB_DELETESTRING, 0, 0); // RA2: as above
		}
		SendMessage(msg, OD_DISABLEPAINT, 0, old); // RA2: as above
	}
}



/// <summary>
/// Counts the human teams still in the game.
/// A group of allied players counts as one team. This routine is used to recognize the point
/// where only one side is left standing, which is why the house being examined is left out
/// of the tally.
/// </summary>
/// <param name="house">The house to leave out of the count.</param>
/// <returns>Returns with the number of opposing teams still alive.</returns>
int CountAliveTeams(HouseClass * house)
{
	int count = 0;
	for (int i = 0; i < Houses.Count(); i++) {
		HouseClass * house1 = Houses[i];
		if (house1 != NULL && !house1->IsDefeated && house1->IsHuman && house1 != house) {
			bool has_ally = false;
			for (int j = 0; j < Houses.Count(); j++) {
				if (j != i) {
					HouseClass * house2 = Houses[j];
					if (house2 != NULL && !house2->IsDefeated && house2->IsHuman) {
						if (house2 != house && house1->Is_Ally(Houses[j]) && house2->Is_Ally(house1)) {
							if (j > i && !has_ally) {
								count++;
							}
							has_ally = true;
						}
					}
				}
			}
			if (!has_ally) {
				count++;
			}
		}
	}
	return(count);
}


/// <summary>
/// Displays an owner drawn message box and waits for an answer.
/// Use this routine in place of the Windows message box so that the prompt matches the
/// game's artwork. The box holds up the caller, but the supplied callback is polled while it
/// is on screen so that network traffic keeps flowing.
/// </summary>
/// <param name="text">The message to display. Nothing at all happens for an empty
/// message.</param>
/// <param name="type">The button layout to use; MB_OK, MB_OKCANCEL or MB_YESNO.</param>
/// <param name="callback">Idle routine to poll while the box is up.</param>
/// <param name="large">Should the large version of the box be used?</param>
/// <returns>Returns with the control ID of the button the player pressed. Zero is returned
/// if there was nothing to display.</returns>
int ODMessageBox(const wchar_t * text, int type, bool (*callback)(void), bool large) // RA2: 0x005C3230, the text is wide and the box has RA2's dialogs: 0xD0 for OK, 0xFC for OK and Cancel, 0x126 for Yes and No, 0xFB for the large box
{
	if (text != NULL && wcslen(text) != 0) { // RA2: as above
		int id; // RA2: as above
		if (large) {
			id = 0xfb; // RA2: as above
		} else if (type == MB_OK) { // RA2: as above
			id = IDD_MSGBOX_3_SMALL; // RA2: as above
		} else if (type == MB_OKCANCEL) { // RA2: as above
			id = 0xfc; // RA2: as above
		} else {
			id = (type != MB_YESNO) ? 0xfb : 0x126; // RA2: as above
		}
		HWND dialog = WS_Create_Dialog(ProgramInstance, id, MainWindow, ODMessageBox_Proc, false); // RA2: as above
		Show_Dialog(dialog, id); // RA2: as above
		if (type == MB_OK) {
			HWND ok = GetDlgItem(dialog, IDOK);
			SetWindowLong(ok, GWL_STYLE, GetWindowLong(ok, GWL_STYLE) | WS_VISIBLE);
			if (large) {
				Move_Default_Button_Owner(ok); // RA2: as above
			}
		} else if (type == MB_OKCANCEL) { // RA2: as above
			HWND ok = GetDlgItem(dialog, IDOK);
			SetWindowLong(ok, GWL_STYLE, GetWindowLong(ok, GWL_STYLE) | WS_VISIBLE);
			HWND cancel = GetDlgItem(dialog, IDCANCEL); // RA2: as above
			SetWindowLong(cancel, GWL_STYLE, GetWindowLong(cancel, GWL_STYLE) | WS_VISIBLE); // RA2: as above
		} else if (type == MB_YESNO) { // RA2: as above
			HWND yes = GetDlgItem(dialog, IDYES);
			SetWindowLong(yes, GWL_STYLE, GetWindowLong(yes, GWL_STYLE) | WS_VISIBLE);
			HWND no = GetDlgItem(dialog, IDNO);
			SetWindowLong(no, GWL_STYLE, GetWindowLong(no, GWL_STYLE) | WS_VISIBLE);
		}
		HWND label = GetDlgItem(dialog, IDC_MSGBOX_TEXT); // RA2: as above
		SendMessage(label, OD_SETTEXTW, 0, (LPARAM)text); // RA2: as above
		ShowWindow(dialog, SW_NORMAL);
		return(WS_Wait_Dialog(dialog, callback, false, true, true)); // RA2: as above
	}
	return(0);
}



/// <summary>
/// Handles the messages for the owner drawn message box.
/// This routine paints the box through the owner draw system and tears it down with
/// whichever of the buttons the player pressed.
/// </summary>
/// <returns>Returns with TRUE if the message was dealt with here, FALSE otherwise.</returns>
int CALLBACK ODMessageBox_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
		case WM_ERASEBKGND: // RA2: the cases come in this order in the executable
			return(TRUE);

		case WM_PAINT:
			Dialog_On_WM_PAINT(window); // RA2: the dialog is painted by the owner draw paint (0x005FEE70), not OwnerDraw::Draw_Dialog_Back
			Slide_Pending_Dialog(window); // RA2: a dialog waiting to slide in does so now (0x005E5F20)
			ValidateRect(window, NULL);
			return(TRUE);

		case WM_DRAWITEM:
			Dialog_On_WM_DRAWITEM(lparam); // RA2: as above
			return(TRUE);

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case IDOK:
				case IDCANCEL:
				case IDYES:
				case IDNO:
					WS_Destroy_Dialog(window, LOWORD(wparam));
					return(TRUE);
			}
			break;
	}
	return(FALSE);
}


/// <summary>
/// Shows the name of a game mode in a lobby dialog.
/// The name is put into the game mode text control of the dialog.
/// </summary>
/// <param name="dialog">The lobby dialog.</param>
/// <param name="mode">The game mode whose name is shown.</param>
void __fastcall Set_Item_Text(HWND dialog, MultiplayerGameMode * mode) // RA2: 0x005C3450, TibSun has no such routine
{
	HWND item = GetDlgItem(dialog, 0x6ec); // RA2: as above
	SendMessage(item, OD_SETTEXTW, 0, (LPARAM)mode->UIName.text()); // RA2: as above
}

/// <summary>
/// Shows a scenario in a lobby dialog.
/// The scenario at the given index of the session's scenario list is handed to the scenario text
/// control of the dialog.
/// </summary>
/// <param name="dialog">The lobby dialog.</param>
/// <param name="index">Index of the scenario in the session's list.</param>
void __fastcall Set_Scenario_Item_Text(HWND dialog, int index) // RA2: 0x005C3480, TibSun has no such routine
{
	MultiMission * map = Session.Scenarios[index]; // RA2: as above
	HWND item = GetDlgItem(dialog, IDC_SCENARIONAME); // RA2: as above
	SendMessage(item, OD_SETTEXTW, 0, (LPARAM)map); // RA2: as above
}

/// <summary>
/// Shows the description of the chosen scenario in a lobby dialog.
/// </summary>
/// <param name="dialog">The lobby dialog.</param>
void __fastcall Set_Skirmish_Dialog_Text(HWND dialog) // RA2: 0x005C34B0, TibSun has no such routine
{
	HWND item = GetDlgItem(dialog, IDC_SCENARIONAME); // RA2: as above
	SendMessage(item, OD_SETTEXTW, 0, (LPARAM)Session.Options.ScenarioDescription); // RA2: as above
}

/// <summary>
/// Fetches the game mode the session options name.
/// The first game mode of the list is used when the options name none that exists.
/// </summary>
/// <returns>Returns with the current game mode.</returns>
MultiplayerGameMode * Get_Current_Game_Mode(void) // RA2: 0x005C34D0, TibSun has no such routine
{
	MultiplayerGameMode * mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex); // RA2: as above
	if (mode == NULL) { // RA2: as above
		mode = First_Game_Mode(); // RA2: as above
	}
	return(mode); // RA2: as above
}

/// <summary>
/// Passes a change of the game mode on to the game mode itself.
/// In the game mode dialog the mode picked in its list is told; elsewhere the session's own mode is.
/// </summary>
void __fastcall Handle_Mode_Selection(void) // RA2: 0x005C34F0, TibSun has no such routine; slot 31 (offset 0x7C) of the game mode is called
{
	if (WS_Top_Window_ID() == 0x6b) { // RA2: as above
		HWND dialog = WS_Top_Window(); // RA2: as above
		int selected = SendDlgItemMessage(dialog, 0x6eb, LB_GETCURSEL, 0, 0); // RA2: as above
		if (selected != -1) { // RA2: as above
			MultiplayerGameMode * mode = (MultiplayerGameMode *)SendDlgItemMessage(dialog, 0x6eb, LB_GETITEMDATA, selected, 0); // RA2: as above
			if (mode != NULL) { // RA2: as above
				mode->vt_entry_7C(); // RA2: as above
			}
		}
	} else if (Session.MPGameMode != NULL) { // RA2: as above
		Session.MPGameMode->vt_entry_7C(); // RA2: as above
	}
}







/// <summary>
/// Displays the current game options in the setup dialog.
/// This routine pushes the session options out to the sliders and check boxes. On the
/// initializing pass it also establishes the slider ranges and greys out whichever options a
/// World Domination Tour territory refuses to let the players meddle with.
/// </summary>
/// <param name="window">The game options dialog to update.</param>
/// <param name="initialize">Is this the first call for a freshly created dialog?</param>
void __fastcall Fill_Options_Dialog(HWND window, int initialize) // RA2: 0x005C3550, RA2 has the options of its own: only the credits, unit count, short game, MCV and crates controls are limited by a tour territory
{
	int unit_count = Session.Options.UnitCount; // RA2: as above
	int credits = Session.Options.Credits; // RA2: as above
	int game_speed = Session.Options.GameSpeed; // RA2: as above

	if (initialize) {
		rc_ptr<WDTTerritory> territory; // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: as above
			territory = Find_WDT_Territory(Session.TournamentTerritory); // RA2: as above
		}
		if (territory.Get() != NULL) { // RA2: as above
			EnableWindow(GetDlgItem(window, IDC_YOURSIDE), FALSE);
			EnableWindow(GetDlgItem(window, 0x6d4), FALSE); // RA2: as above
			if (!(territory->UserModBooleans & 0x20000)) { // RA2: as above
				EnableWindow(GetDlgItem(window, IDC_CREDITS), FALSE);
			}
			if (!(territory->UserModBooleans & 0x8000)) { // RA2: as above
				EnableWindow(GetDlgItem(window, IDC_UNITCOUNT), FALSE);
			}
			if (!(territory->UserModBooleans & 0x400)) { // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_SHORT_GAME), FALSE); /// Short Game
			}
			if (!(territory->UserModBooleans & 0x20)) { // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_REDEPLOY_MCV), FALSE); /// Re-Deployable MCV
			}
			if (!(territory->UserModBooleans & 0x100)) { // RA2: as above
				EnableWindow(GetDlgItem(window, IDC_CRATES), FALSE);
			}
		}
		SendDlgItemMessage(window, IDC_GAME_SPEED_SLIDER, TBM_SETRANGE, TRUE, MAKELONG(0, 6)); /// Game Speed
		SendDlgItemMessage(window, IDC_CREDITS, TBM_SETRANGE, TRUE, MAKELONG(Rule->MinMoney, Rule->MPMaxMoney)); // RA2: as above
		SendDlgItemMessage(window, IDC_CREDITS, OD_SETTRACKSTEP, 0, Rule->MoneyIncrement); // RA2: as above
		SendDlgItemMessage(window, IDC_UNITCOUNT, TBM_SETRANGE, TRUE, MAKELONG(Rule->MinUnitCount, Rule->MaxUnitCount)); // RA2: as above
	}

	SendDlgItemMessage(window, IDC_GAME_SPEED_SLIDER, TBM_SETPOS, TRUE, 6 - game_speed); // RA2: as above
	SendDlgItemMessage(window, IDC_CREDITS, TBM_SETPOS, TRUE, credits); // RA2: as above
	SendDlgItemMessage(window, IDC_UNITCOUNT, TBM_SETPOS, TRUE, unit_count); // RA2: as above

	int button_state[] = { BST_UNCHECKED, BST_CHECKED };
	SendDlgItemMessage(window, IDC_SHORT_GAME, BM_SETCHECK, button_state[Session.Options.ShortGame], 0);
	SendDlgItemMessage(window, 0x69a, BM_SETCHECK, button_state[Session.Options.SWAllowed], 0); // RA2: as above
	SendDlgItemMessage(window, 0x69d, BM_SETCHECK, button_state[Session.Options.BuildOffAlly], 0); // RA2: as above
	SendDlgItemMessage(window, IDC_REDEPLOY_MCV, BM_SETCHECK, button_state[Session.Options.MCVRedeploy], 0);
	SendDlgItemMessage(window, IDC_CRATES, BM_SETCHECK, button_state[Session.Options.Goodies], 0);
}



void Encode_Game_Options(char *out);
void Net2EncodeGameopt(char *out);


/// <summary>
/// Broadcasts the host's game options if they need broadcasting.
/// This routine is called from the game setup dialog's idle loop. It only puts a packet on
/// the wire when an option has actually changed or when the caller insists, so that dragging
/// a slider about does not flood the other players.
/// </summary>
/// <param name="force">Should the options be sent even though nothing has changed?</param>
/// <param name="now">Should the send happen immediately rather than waiting for the next
/// opportunity?</param>
void __fastcall Send_Lobby_Options(bool force, bool now) // RA2: 0x005C3810, RA2 sends fewer options than TibSun (no tech level, AI players, difficulty, allies, truce, flag, fog or bases)
{
	static int _last_pump_time; // RA2: as above
	static bool _need_to_pump; // RA2: as above
	static bool _last_short_game; // RA2: as above
	static bool _last_sw_allowed; // RA2: as above
	static bool _last_build_off_ally; // RA2: as above
	static bool _last_mcv_redeploy; // RA2: as above
	static bool _last_crates; // RA2: as above
	static int _last_game_speed = -1;
	static int _last_credits = -1;
	static int _last_unit_count = -1;
	static bool _last_scenario_is_official = true;
	static unsigned int _last_scenario_file_length = -1;
	static wchar_t _last_scenario_description[45]; // RA2: as above
	static char _last_scenario_file_name[14];
	static char _last_scenario_digest[33];

	bool do_pump = false;
	bool it_is_time = false;

	/// if we didn't pump this second, we can consider pumping
	if (time(NULL) - _last_pump_time != 0) {
		it_is_time = true;
	}

	/// if it isn't time to pump, but we were told to pump, remember to do
	/// so when time comes, regardless of if anything changed
	if (!it_is_time) {
		if (force) {
			_need_to_pump = true;
		}
	}

	/// if it is time to pump, or we were told to pump, do so
	if (it_is_time || now) {
		if (_need_to_pump) {
			force = true;
		}
		_need_to_pump = false;

		if (g_pChat != NULL) {
			if (time(NULL) - _last_pump_time >= 5) {
				do_pump = true;
			}
		} else {
			if (time(NULL) - _last_pump_time >= 2) {
				do_pump = true;
			}
		}

		if (force) {
			do_pump = true;
		}

		if (_last_game_speed != Session.Options.GameSpeed) do_pump = true;
		if (_last_credits != Session.Options.Credits) do_pump = true;
		if (_last_unit_count != Session.Options.UnitCount) do_pump = true;
		if (_last_short_game != Session.Options.ShortGame) do_pump = true;
		if (_last_sw_allowed != Session.Options.SWAllowed) do_pump = true; // RA2: as above
		if (_last_build_off_ally != Session.Options.BuildOffAlly) do_pump = true; // RA2: as above
		if (_last_mcv_redeploy != Session.Options.MCVRedeploy) do_pump = true;
		if (_last_crates != Session.Options.Goodies) do_pump = true; // RA2: as above

		_last_scenario_description[43] = 0;
		if (wcsicmp(_last_scenario_description, Session.Options.ScenarioDescription)) do_pump = true; // RA2: as above

		_last_scenario_file_name[13] = 0;
		if (stricmp(_last_scenario_file_name, Session.ScenarioFileName)) do_pump = true;

		_last_scenario_digest[32] = 0;
		if (stricmp(_last_scenario_digest, Session.ScenarioDigest)) do_pump = true;

		if (_last_scenario_file_length != Session.ScenarioFileLength) do_pump = true;
		if (_last_scenario_is_official != Session.ScenarioIsOfficial) do_pump = true;

		if (do_pump) {
			_last_pump_time = time(0);
			_last_game_speed = Session.Options.GameSpeed;
			_last_credits = Session.Options.Credits;
			_last_unit_count = Session.Options.UnitCount;
			_last_short_game = Session.Options.ShortGame;
			_last_sw_allowed = Session.Options.SWAllowed; // RA2: as above
			_last_build_off_ally = Session.Options.BuildOffAlly; // RA2: as above
			_last_mcv_redeploy = Session.Options.MCVRedeploy;
			_last_crates = Session.Options.Goodies; // RA2: as above
			wcscpy(_last_scenario_description, Session.Options.ScenarioDescription); // RA2: as above
			strcpy(_last_scenario_digest, Session.ScenarioDigest);
			strcpy(_last_scenario_file_name, Session.ScenarioFileName);
			_last_scenario_file_length = Session.ScenarioFileLength;
			_last_scenario_is_official = Session.ScenarioIsOfficial;

			char buffer[520]; // RA2: as above
			memset(buffer, '\0', sizeof(buffer));

			if (g_pChat != NULL) {
				Encode_Game_Options(buffer);
				Send_Host_Option(buffer); // RA2: as above
			} else {
				Net2EncodeGameopt(buffer);
				Send_Host_Option(buffer); // RA2: as above
			}
		}
	}
}


/// <summary>
/// Reminds the guests that the host is about to start the game.
/// A reminder packet goes to every other player of the lobby over the network layer.
/// </summary>
void Send_Start_Reminder(void) // RA2: 0x005C3AE0, TibSun has no such routine
{
	GlobalPacketType packet;
	memset(&packet, 0, sizeof(packet));
	packet.Command = NET_COMMAND_28; // RA2: as above
	strcpy(packet.Name, Session.Handle);
	int count = Session.Players.Count(); // RA2: as above
	for (int i = 1; i < count; i++) { // RA2: as above
		Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address, 0, 0); // RA2: as above
		Call_Back();
	}
}

/// <summary>
/// Sends the start reminder through whichever layer the game is played over.
/// </summary>
void __fastcall Dispatch_Game_Mode(void) // RA2: 0x005C3B70, TibSun has no such routine
{
	switch (Session.Type) {
		case GAME_IPX:
			Send_Start_Reminder(); // RA2: as above
			break;

		case GAME_INTERNET:
			Wol_Send_Start_Reminder(); // RA2: as above
			break;
	}
}

/// <summary>
/// Handles the host's start reminder in the guest's lobby.
/// The guest is marked as reminded and told, through the chat service or the network layer.
/// </summary>
void __fastcall Handle_Start_Reminder(void) // RA2: 0x005C3C20, TibSun has no such routine
{
	Session.IsStartReminded = true; // RA2: as above
	if (g_pChat != NULL) {
		Wol_Handle_Start_Reminder(); // RA2: as above
	} else {
		Join_Lobby_Start_Reminder(); // RA2: as above
	}
}

/// <summary>
/// Handles the host taking the start reminder back in the guest's lobby.
/// The reminded mark is cleared and the ready button is reset, through the chat service or the network layer.
/// </summary>
void __fastcall Handle_Reset_Ready(void) // RA2: 0x005C3C40, TibSun has no such routine
{
	Session.IsStartReminded = false; // RA2: as above
	if (g_pChat != NULL) {
		Wol_Reset_Ready(); // RA2: as above
	} else {
		Join_Lobby_Reset_Ready(); // RA2: as above
	}
}






/// <summary>
/// Sends an encoded game options string to every player.
/// This routine hides the difference between the internet chat service and the direct
/// network layer, so the game options only have to be encoded once.
/// </summary>
/// <param name="options">The encoded option string to send.</param>
void __fastcall Send_Host_Option(char * options) // RA2: 0x005C3C60, the options go out as a plain string and the player color is the preferred one; main names it Send_Host_Option (netdlg2.cpp)
{
	if (g_pChat != NULL) {
		char buffer[0x400]; // RA2: as above
		strncpy(buffer, options, 0x3ff); // RA2: as above
		g_pChat->RequestPublicGameOptions((LPCSTR)buffer); // RA2: as above
	} else {
		GlobalPacketType packet;
		memset(&packet, 0, sizeof(packet));
		packet.Command = NET_PUB_GAMEOPT;
		strcpy(packet.Name, Session.Handle);
		strcpy(packet.Options.Buf, options);
		packet.Options.Color = Session.PrefColor; // RA2: as above
		packet.Options.NameCRC = Compute_Name_CRC(Session.GameName);
		for (int i = 1; i < Session.Players.Count(); i++) {
			NodeNameType * node = Session.Players[i]; // RA2: as above
			DebugString("Sending public game options to %s\n", Wide_To_Narrow(node->Name)); // RA2: as above
			Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &node->Address, 0, 0); // RA2: as above
			Call_Back();
		}
	}
}



/// <summary>
/// Sends an encoded game options string to one player.
/// This routine hides the difference between the internet chat service and the direct
/// network layer, so the caller only has to name the player who is to receive the options.
/// </summary>
/// <param name="player">Name of the player to send the options to.</param>
/// <param name="options">The encoded option string to send.</param>
void __fastcall Send_Player_Choice(char const * player, char const * options) // RA2: 0x005C3D90, the packet carries the chosen color and the wide host name; main names it Send_Player_Choice (netdlg2.cpp)
{
	if (g_pChat != NULL) {
		User user;
		memset(&user, '\0', sizeof(user));
		strcpy((char *)user.name, player);
		g_pChat->RequestPrivateGameOptions(&user, (LPCSTR)options);
	} else {
		GlobalPacketType packet;
		memset(&packet, 0, sizeof(packet));
		packet.Command = NET_PRIV_GAMEOPT; // RA2: as above
		strcpy(packet.Name, Session.Handle);
		strcpy(packet.Options.Buf, options);
		Session.GPacket.Options.Color = Session.Chosen_Color(); // RA2: as above
		Session.GPacket.Options.NameCRC = Compute_Name_CRC(Session.GameName);
		for (int i = 1; i < Session.Players.Count(); i++) {
			if (stricmp(Wide_To_Narrow(Session.Players[i]->Name), player) == 0) { // RA2: as above
				DebugString("Sending private game options to %ls\n", Session.Players[i]->Name); // RA2: as above
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address, 0, 0); // RA2: as above
				Call_Back();
			}
		}
	}
}


/// <summary>
/// Records whether a player has the host's map.
/// The state is kept with the player; when a player lacks the map or is being sent it, the lobby
/// chat says so. The host always has the map.
/// </summary>
/// <param name="name">Name of the player, or NULL for the local player.</param>
/// <param name="state">The map state: 0 for none, 1 for present, 2 for being transferred.</param>
void __fastcall Set_Player_Map_State(char const * name, int state) // RA2: 0x005C3F00, TibSun has no such routine
{
	if (name == NULL) { // RA2: as above
		name = Session.Handle; // RA2: as above
	}
	if (wcscmp(Narrow_To_Wide(name), Session.GameName) == 0) { // RA2: as above
		state = 1; // RA2: as above
	}
	int found = -1;
	for (int i = 0; i < Session.Players.Count(); i++) { // RA2: as above
		if (stricmp(Wide_To_Narrow(Session.Players[i]->Name), name) == 0) { // RA2: as above
			found = i; // RA2: as above
			break;
		}
	}
	if (found != -1) { // RA2: as above
		Session.Players[found]->Slot.Unknown7F = state; // RA2: as above
		if (state == 0) { // RA2: as above
			Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostNoMap", NULL, "D:\\Ra2\\netshare.cpp", 1115), Session.Players[found]->Name, Session.Options.ScenarioDescription); // RA2: as above
		} else if (state == 2) { // RA2: as above
			Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostMapTransfer", NULL, "D:\\Ra2\\netshare.cpp", 1119), Session.Players[found]->Name, Session.Options.ScenarioDescription); // RA2: as above
		}
	}
}



int __fastcall Find_Slot_By_Name(wchar_t const * name); // RA2: 0x005C8C30, defined in address order below; the earlier callers expand it inline
int Find_First_Player_Slot(void); // RA2: 0x005C8C70, as above
int __fastcall Get_Slot_Control_Id(int slot); // RA2: 0x005C97A0, as above


/// <summary>
/// Decodes a public game options string sent by the host.
/// This routine applies the host's settings to the local session and refreshes the game
/// options dialog to suit. When something material has changed, everybody's accept flag is
/// cleared so that the players have to agree to the new terms. A player whose preferred
/// color has been taken is quietly moved to the one the host assigned.
/// </summary>
/// <param name="options">The encoded option string as received.</param>
/// <param name="name">Name of the player the options came from.</param>
/// <returns>bool; Did the scenario change?</returns>
bool __fastcall Net2DecodeGameopt(char const * text, char const * sender); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: 7768 bytes differ at 005c3ff0 (8191 body bytes, image_bounds.csv).
/*
 * If the host is forcing a color reassignment, then this flag will be true. It is set when
 * the host accepts the game options and cleared once the assignment has been processed, and
 * it suppresses the "color in use" message that would otherwise appear.
 */
int IsColorChangePending; // RA2: used only by Net2DecodeGameopt (and wonline's parked draft), so parked with it (DECISIONS: a global whose only user is a parked function)

bool __fastcall Net2DecodeGameopt(char const * text, char const * sender) // RA2: 0x005C3FF0, replaces TibSun's DecodePubGameopt; the executable decodes every kind of game options string (Z, O, A, C, L, G, K, P and the host's full options) and returns true when the scenario changed
{
	static int _last_game_speed = -1; // RA2: 0x007E6DD8, the previous options are kept in function statics: three ints in the data segment
	static int _last_credits = -1; // RA2: 0x007E6DDC
	static int _last_unit_count = -1; // RA2: 0x007E6DE0
	static bool _last_short_game; // RA2: 0x00A72D53, five flags in the bss
	static bool _last_super_weapons; // RA2: 0x00A72D54
	static bool _last_build_off_ally; // RA2: 0x00A72D55
	static bool _last_mcv_redeploy; // RA2: 0x00A72D56
	static bool _last_goodies; // RA2: 0x00A72D57

	bool changed = false; // RA2: as above
	char *copy = strdup(text); // RA2: as above
	if (copy[0] == 'Z') { // RA2: as above
		switch (copy[1]) { // RA2: as above
		case '1': { // RA2: as above
			for (int i = 0; i < 8; i++) { // RA2: as above
				if (_stricmp(sender, Wide_To_Narrow(Session.Slots[i]->Name)) == 0) { // RA2: as above
					char receiver[64]; // RA2: as above
					strcpy(receiver, copy + 3); // RA2: as above
					Register_Connection_State(sender, receiver, copy[2] - '0'); // RA2: as above
				}
			}
			free(copy); // RA2: as above
			return(false); // RA2: as above
		}
		case '2': // RA2: as above
			Store_Player_Port(sender, (unsigned short)atol(copy + 2)); // RA2: as above
			free(copy); // RA2: as above
			return(false); // RA2: as above
		case '3': // RA2: as above
			if (!Local_User_Is_Channel_Owner()) { // RA2: as above
				HWND dialog = WS_Find_Dialog(IDD_WOL_GUEST); // RA2: as above
				HWND preview = GetDlgItem(dialog, 0x3F2); // RA2: as above
				if (preview != NULL) { // RA2: as above
					ShowWindow(preview, SW_SHOWNORMAL); // RA2: as above
					ShowWindow(GetDlgItem(dialog, 0x468), SW_HIDE); // RA2: as above
					SendMessageA(preview, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString("TXT_NAT_NO_PREVIEW", NULL, "D:\\Ra2\\netshare.cpp", 1214)); // RA2: as above
				}
			}
			break; // RA2: as above
		}
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}
	if (Session.Type == GAME_INTERNET && (g_PlayingNetGame || WolOptionsLocked != 0)) { // RA2: as above
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}
	if (copy[0] == 'O') { // RA2: as above
		HWND dialog = WS_Find_Dialog(IDD_WOL_GUEST); // RA2: as above
		if (dialog != NULL || (dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST)) != 0) { // RA2: as above
			int old = LobbyObserverSlot; // RA2: as above
			LobbyObserverSlot = copy[1] == '8' ? -1 : copy[1] - '0'; // RA2: as above
			if (LobbyObserverSlot != old) { // RA2: as above
				Show_Observer_Slot_Controls(dialog); // RA2: as above
				wchar_t name[256]; // RA2: as above
				wcscpy(name, Narrow_To_Wide(copy + 2)); // RA2: as above
				if (name[0] != 0) { // RA2: as above
					NodeNameType *entry; // RA2: as above
					{
						WideWstring wide(name); // RA2: as above
						entry = Find_Lobby_Entry_By_Name(wide); // RA2: as above
					}
					if (entry == NULL) { // RA2: as above
						entry = Find_Lobby_Name(name); // RA2: as above
						if (entry == NULL) { // RA2: as above
							free(copy); // RA2: as above
							return(false); // RA2: as above
						}
					}
					if (entry == LobbyEntries[6] // RA2: as above
						|| (entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] // RA2: as above
							&& entry != LobbyEntries[5] && entry != LobbyEntries[2] // RA2: as above
							&& entry != LobbyEntries[3] && entry != LobbyEntries[4] // RA2: as above
							&& entry->Slot.SpectatorFlag == -1)) { // RA2: as above
						Session.Slots[LobbyObserverSlot] = entry; // RA2: as above
						Fill_Observer_Combo(dialog, Player_Side_Combo_ID(LobbyObserverSlot)); // RA2: as above
						SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(LobbyObserverSlot)), 0x4F1, 0, 1); // RA2: as above
						int id = Player_Color_Combo_ID(LobbyObserverSlot); // RA2: as above
						Fill_Random_Color_Combo(dialog, id); // RA2: as above
						Select_Color_Combo(dialog, id, 8); // RA2: as above
					} else {
						Fill_Side_Combo(dialog, Player_Side_Combo_ID(LobbyObserverSlot)); // RA2: as above
						SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(LobbyObserverSlot)), 0x4F1, 0, 0); // RA2: as above
						int id = Player_Color_Combo_ID(LobbyObserverSlot); // RA2: as above
						Fill_Color_Combo(dialog, id); // RA2: as above
						Select_Color_Combo(dialog, id, -2); // RA2: as above
					}
					Session.Slots[LobbyObserverSlot] = entry; // RA2: as above
				}
			}
			if (LobbyObserverSlot >= 0 && LobbyObserverSlot != Session.LocalSlot) { // RA2: as above
				Update_Slot_Controls(dialog, LobbyObserverSlot, false); // RA2: as above
			}
		}
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}
	if (copy[0] == 'A') { // RA2: as above
		int value = atol(copy + 1); // RA2: as above
		if (g_pChat != NULL) { // RA2: as above
			SetPlayerAccepted(Wstring(sender), value); // RA2: as above
			free(copy); // RA2: as above
			return(false); // RA2: as above
		}
		Net2SetAccept(sender, value); // RA2: as above
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}
	if (copy[0] == 'C') { // RA2: as above
		if (copy[1] == '0') { // RA2: as above
			DebugString("@@@@ Decoding cooperative campaign game options %s\n", text); // RA2: as above
			int index = -1; // RA2: as above
			DynamicVectorClass<int> list; // RA2: as above
			int time = 0; // RA2: as above
			int unknown = 0; // RA2: as above
			MPlayerScoreType scores[2]; // RA2: as above
			char *token = strtok(copy + 2, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				index = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				time = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				unknown = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				scores[1].Kills[0] = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				scores[1].Built[0] = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				scores[1].Lost[0] = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				scores[1].Score[0] = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				scores[0].Kills[0] = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				scores[0].Built[0] = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				scores[0].Lost[0] = atol(token); // RA2: as above
			}
			token = strtok(NULL, ","); // RA2: as above
			if (token != NULL) { // RA2: as above
				scores[0].Score[0] = atol(token); // RA2: as above
			}
			for (int i = 0; i < 10; i++) { // RA2: as above
				token = strtok(NULL, ","); // RA2: as above
				if (token == NULL) { // RA2: as above
					break; // RA2: as above
				}
				int value = atol(token); // RA2: as above
				if (value < 0) { // RA2: as above
					break; // RA2: as above
				}
				list.Add(value); // RA2: as above
			}
			char *first = strtok(NULL, ","); // RA2: as above
			char *second = strtok(NULL, ","); // RA2: as above
			for (int mode = 0; mode <= MultiplayerGameMode::GetModeCount(); mode++) { // RA2: as above
				if (MultiplayerGameMode::Get(mode) != NULL) { // RA2: as above
					DebugString("Setting Campaign info for Mode %d: %s\n", mode, // RA2: as above
						Wide_To_Narrow(MultiplayerGameMode::Get(mode)->UIName.text())); // RA2: as above
					if (MultiplayerGameMode::Get(mode)->vt_entry_48(index, list, first, second)) { // RA2: as above
						MultiplayerGameMode::Get(mode)->vt_entry_4C(time, unknown, scores); // RA2: as above
					}
				}
			}
		} else {
			DebugString("@@@@ Decoding cooperative campaign game options %s\n", text); // RA2: as above
			for (int mode = 0; mode <= MultiplayerGameMode::GetModeCount(); mode++) { // RA2: as above
				if (MultiplayerGameMode::Get(mode) != NULL) { // RA2: as above
					DebugString("Handling Campaign info for Mode %d: %s\n", mode, // RA2: as above
						Wide_To_Narrow(MultiplayerGameMode::Get(mode)->UIName.text())); // RA2: as above
					MultiplayerGameMode::Get(mode)->vt_entry_64(copy[1] - '0', copy + 2); // RA2: as above
				}
			}
		}
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}
	if (copy[0] == 'L') { // RA2: as above
		WideWstring names = Wstring_ToWide(Wstring(text + 1)); // RA2: as above
		Set_Slot_Names(names.get()); // RA2: as above
		HWND dialog = WS_Find_Dialog(IDD_WOL_GUEST); // RA2: as above
		if (dialog != NULL) { // RA2: as above
			int slot = Find_First_Player_Slot(); // RA2: as above
			if (slot == -1) { // RA2: as above
				Leave_Channel_With_Message(dialog); // RA2: as above
			} else if (slot != Session.LocalSlot) { // RA2: as above
				int id = Player_Side_Combo_ID(slot); // RA2: as above
				if (Session.Slots[slot]->Slot.SpectatorFlag == -1) { // RA2: as above
					Fill_Observer_Combo(dialog, id); // RA2: as above
					SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 1); // RA2: as above
					int color = Player_Color_Combo_ID(slot); // RA2: as above
					Fill_Random_Color_Combo(dialog, color); // RA2: as above
					Select_Color_Combo(dialog, color, 8); // RA2: as above
				} else {
					id = Player_Side_Combo_ID(slot); // RA2: as above
					// NATIVE (Scout1274): the original (0x005C4A13) fills the side combo only when a game mode is set and its slot 1 is false
					if (Session.MPGameMode != NULL && !Session.MPGameMode->vt_entry_04()) { // RA2: as above
						Fill_Side_Combo(dialog, id); // RA2: as above
					}
					SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 0); // RA2: as above
					int color = Player_Color_Combo_ID(slot); // RA2: as above
					Fill_Color_Combo(dialog, color); // RA2: as above
					Select_Color_Combo(dialog, color, -2); // RA2: as above
				}
				slot = Find_First_Player_Slot(); // RA2: as above
				Session.LocalSlot = slot; // RA2: as above
				Lock_Slots(dialog, slot); // RA2: enable the new local slot and disable the other controls.
				Select_Player_Side(dialog, Session.LocalSlot); // RA2: as above
				Select_Player_Color(dialog, Session.LocalSlot); // RA2: as above
			}
		}
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}
	if (copy[0] == 'G') { // RA2: as above
		if (WS_Find_Dialog(IDD_WOL_GUEST) != 0) { // RA2: as above
			Session.IsStartReminded = true; // RA2: as above
			if (g_pChat != NULL) { // RA2: as above
				Wol_Handle_Start_Reminder(); // RA2: as above
				free(copy); // RA2: as above
				return(false); // RA2: as above
			}
			Join_Lobby_Start_Reminder(); // RA2: as above
		}
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}
	if (copy[0] == 'K') { // RA2: as above
		if (WS_Find_Dialog(IDD_WOL_GAMEOPT) != 0 || WS_Find_Dialog(IDD_MPLAYER_HOST) != 0) { // RA2: as above
			int state; // RA2: as above
			switch (copy[1]) { // RA2: as above
			case '1': // RA2: as above
				state = 1; // RA2: as above
				break; // RA2: as above
			case '2': // RA2: as above
				state = 2; // RA2: as above
				break; // RA2: as above
			default:
				state = 0; // RA2: as above
				break; // RA2: as above
			}
			Set_Player_Map_State(sender, state); // RA2: as above
		}
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}
	if (copy[0] == 'P') { // RA2: as above
		DebugString("Got Ping game options string. %s\n", copy); // RA2: as above
		char const *p = copy + 3; // RA2: as above
		if (Session.Type == GAME_INTERNET) { // RA2: as above
			while (*p != 0) { // RA2: as above
				char name[64]; // RA2: as above
				char number[68]; // RA2: as above
				int i = 0; // RA2: as above
				while (*p != 0 && *p != ',') { // RA2: as above
					name[i++] = *p++; // RA2: as above
				}
				name[i] = 0; // RA2: as above
				i = 0; // RA2: as above
				if (*p != 0) { // RA2: as above
					p++; // RA2: as above
				}
				while (*p != 0 && *p != ',') { // RA2: as above
					number[i++] = *p++; // RA2: as above
				}
				number[i] = 0; // RA2: as above
				int ping = atol(number); // RA2: as above
				if (*p != 0) { // RA2: as above
					p++; // RA2: as above
				}
				Store_User_Ping(name, ping); // RA2: as above
			}
		}
		free(copy); // RA2: as above
		return(false); // RA2: as above
	}

	int dialog_id = WS_Top_Window_ID(); // RA2: as above
	if (dialog_id != IDD_MPLAYER_HOST && dialog_id != IDD_MPLAYER_GUEST) { // RA2: as above
		SendDlgItemMessageA(GameoptWindow(), IDC_USERS, OD_DISABLEPAINT, 0, 1); // RA2: as above
	}
	DebugString("Decoding game options %s\n", text); // RA2: as above
	char *token = strtok(copy, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Seed = atol(token); // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Options.GameSpeed = atol(token);
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.GameSpeed = atol(token); // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.Credits = atol(token); // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.UnitCount = atol(token); // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.ShortGame = atol(token) != false; // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.SWAllowed = atol(token) != false; // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.BuildOffAlly = atol(token) != false; // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.MCVRedeploy = atoi(token) != false; // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.Goodies = atoi(token) != false; // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.Options.MPModeIndex = atol(token); // RA2: as above
	}
	bool same_scenario = true; // RA2: as above
	token = strtok(NULL, ","); // RA2: as above
	WideWstring scenario_name(Session.Type == GAME_INTERNET && Is_Tournament_Session() // RA2: as above
			? WideWstring(L"") : Wide_From_7_Bit(Wstring(token))); // RA2: as above
	token = strtok(NULL, ","); // RA2: as above
	int old_count = Session.ScenarioMaxPlayers; // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.ScenarioMaxPlayers = atoi(token); // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	bool old_official = Session.ScenarioIsOfficial; // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.ScenarioIsOfficial = atoi(token) != false; // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	int old_length = Session.ScenarioFileLength; // RA2: as above
	if (token != NULL) { // RA2: as above
		Session.ScenarioFileLength = atoi(token); // RA2: as above
	}
	token = strtok(NULL, ","); // RA2: as above
	if (token != NULL) { // RA2: as above
		if (_stricmp(Session.ScenarioFileName, token) != 0) { // RA2: as above
			same_scenario = false; // RA2: as above
		}
		strncpy(Session.ScenarioFileName, token, 0x202); // RA2: as above
		strcpy(Scen->ScenarioName, Session.ScenarioFileName); // RA2: as above
	}
	char *digest = strtok(NULL, ":"); // RA2: as above
	if (digest != NULL) { // RA2: as above
		if (strcmp(Session.ScenarioDigest, digest) != 0) { // RA2: as above
			same_scenario = false; // RA2: as above
		}
		strncpy(Session.ScenarioDigest, digest, 0x21); // RA2: as above
	}
	MultiplayerGameMode *mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex); // RA2: as above
	if (dialog_id == IDD_MPLAYER_GUEST || dialog_id == IDD_WOL_GUEST) { // RA2: as above
		Change_Slot_Count(GameoptWindow(), old_count, Session.ScenarioMaxPlayers, // RA2: as above
			mode); // RA2: as above
	}
	bool found = true; // RA2: as above
	if (!same_scenario || wcslen(Session.Options.ScenarioDescription) == 0) { // RA2: as above
		found = false; // RA2: as above
		same_scenario = false; // RA2: as above
		for (int i = 0; i < Session.Scenarios.Count(); i++) { // RA2: as above
			MultiMission *scenario = Session.Scenarios[i]; // RA2: as above
			if (_stricmp(Session.ScenarioFileName, scenario->Get_Filename()) == 0) { // RA2: as above
				CCFileClass file(scenario->Get_Filename()); // RA2: as above
				if (file.Is_Available(false) && file.Size() == Session.ScenarioFileLength) { // RA2: as above
					INIClass ini; // RA2: as above
					ini.Load(file, false); // RA2: as above
					char buffer[32]; // RA2: as above
					ini.Get_String("Digest", "1", "No digest here mate. Nope.", buffer, sizeof(buffer)); // RA2: as above
					if (Session.ScenarioDigest[0] == 0 // RA2: as above
						|| strcmp(Session.ScenarioDigest, buffer) == 0) { // RA2: as above
						wcscpy(Session.Options.ScenarioDescription, scenario->Description()); // RA2: as above
						found = true; // RA2: as above
					}
				}
			}
		}
		if (!found && scenario_name.length() > 0) { // RA2: as above
			wcscpy(Session.Options.ScenarioDescription, scenario_name.get()); // RA2: as above
		}
		if (Session.MPGameMode != NULL) { // RA2: as above
			Session.MPGameMode->vt_entry_24(); // RA2: as above
		}
	}
	mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex); // RA2: as above
	if (mode != Session.MPGameMode) { // RA2: as above
		if (Session.MPGameMode != NULL) { // RA2: as above
			Session.MPGameMode->vt_entry_84(); // RA2: as above
		}
		if (mode != NULL) { // RA2: as above
			mode->vt_entry_20(); // RA2: as above
		}
	}
	Session.MPGameMode = mode; // RA2: as above
	if (mode != NULL && !same_scenario) { // RA2: as above
		mode->UnfixAlliances(); // RA2: as above
	}
	HWND dialog = GameoptWindow(); // RA2: as above
	Set_Item_Text(dialog, mode); // RA2: the verified game-mode label function at 005C3450.
	Set_Skirmish_Dialog_Text(dialog); // RA2: the verified scenario label function at 005C34B0.
	Scen->Scenario = -1; // RA2: as above
	Frame = 0; // RA2: as above
	Session.CommProtocol = DEFAULT_COMM_PROTOCOL; // RA2: restore the default protocol after selecting a scenario
	if (old_official != Session.ScenarioIsOfficial // RA2: as above
		|| old_length != Session.ScenarioFileLength) { // RA2: as above
		same_scenario = false; // RA2: as above
	}
	if (!same_scenario) { // RA2: as above
		DebugString("Not same scenario...\n"); // RA2: as above
		if (Session.ScenarioIsOfficial && !found // RA2: as above
			&& _stricmp(Session.ScenarioFileName, "RandMap.Sed") != 0) { // RA2: as above
			if (GameoptWindow() != 0) { // RA2: as above
				Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:JoinerNoMap", NULL, "D:\\Ra2\\netshare.cpp", 1818), // RA2: as above
					Session.Options.ScenarioDescription); // RA2: as above
			}
			LobbyDialogFlag = false; // RA2: as above
			EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), FALSE); // RA2: as above
			InvalidateRect(GetDlgItem(GameoptWindow(), IDC_ACCEPT), 0, FALSE); // RA2: as above
			Send_Host_Option("K0"); // RA2: as above
		} else {
			LobbyDialogFlag = true; // RA2: as above
			EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), TRUE); // RA2: as above
			InvalidateRect(GetDlgItem(GameoptWindow(), IDC_ACCEPT), 0, FALSE); // RA2: as above
			if (!Session.ScenarioIsOfficial && !found // RA2: as above
				&& _stricmp(Session.ScenarioFileName, "RandMap.Sed") != 0) { // RA2: as above
				if (GameoptWindow() != 0) { // RA2: as above
					Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:JoinerMapTransfer", NULL, "D:\\Ra2\\netshare.cpp", 1834), // RA2: as above
						Session.Options.ScenarioDescription); // RA2: as above
				}
				Send_Host_Option("K2"); // RA2: as above
			} else {
				Send_Host_Option("K1"); // RA2: as above
			}
		}
		Update_Network_Dialog_Preview(GameoptWindow()); // RA2: as above
	}
	if (digest == NULL) { // RA2: as above
		Lobby_Chat_Printf(-1, L"Gameopt parse failed"); // RA2: as above
	}
	{
		unsigned int address = 0; // RA2: as above
		for (;;) { // RA2: as above
			token = strtok(NULL, ",:"); // RA2: as above
			if (token == NULL) { // RA2: as above
				break; // RA2: as above
			}
			char * name = token; // RA2: TibSun's DecodePubGameopt keeps the handle so (char * handle = token;)
			if (strcmp(name, "@") == 0) { // RA2: as above
				break; // RA2: as above
			}
			token = strtok(NULL, ",:"); // RA2: as above
			if (token == NULL) { // RA2: as above
				break; // RA2: as above
			}
			int country = atol(token); // RA2: as above
			token = strtok(NULL, ",:"); // RA2: as above
			if (token == NULL) { // RA2: as above
				break; // RA2: as above
			}
			int color = atol(token); // RA2: as above
			token = strtok(NULL, ",:"); // RA2: as above
			if (token == NULL) { // RA2: as above
				break; // RA2: as above
			}
			sscanf(token, "%X", &address); // RA2: as above
			int i = 8; // RA2: as above
			if (address != 0 && Session.Type == GAME_INTERNET) { // RA2: as above
				for (i = 0; i < 8; i++) { // RA2: as above
					if (_stricmp(name, Wide_To_Narrow(Session.Slots[i]->Name)) == 0) { // RA2: as above
						NetNumType net; // RA2: as above
						NetNodeType node; // RA2: as above
						memset(node, 0, sizeof(node)); // RA2: as above
						memcpy(node, &address, sizeof(address)); // RA2: as above
						memset(net, 0, sizeof(net)); // RA2: as above
						Session.SlotAddresses[i].Set_Address(net, node, 0); // RA2: as above
						token = strtok(NULL, ",:"); // RA2: as above
						if (token != NULL) { // RA2: as above
							int value; // RA2: as above
							sscanf(token, "%X", &value); // RA2: as above
							Session.WolPlayerState[i] = value; // RA2: as above
							token = strtok(NULL, ",:"); // RA2: as above
							if (token != NULL) { // RA2: as above
								int value2; // RA2: as above
								sscanf(token, "%X", &value2); // RA2: as above
								Session.PlayerPortBase[i] = value2; // RA2: as above
							}
						}
						break; // RA2: as above
					}
				}
			}
			if (i == 8) { // RA2: as above
				strtok(NULL, ",:"); // RA2: as above
				strtok(NULL, ",:"); // RA2: as above
			}
			IsColorChangePending = 1; // RA2: as above
			if (_stricmp(name, Session.Handle) == 0 && color != Session.Color) { // RA2: as above
				if (IsColorChangePending == 0) { // RA2: as above
					Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_COLOR_IN_USE", NULL, "D:\\Ra2\\netshare.cpp", 1954)); // RA2: as above
				}
				Session.Color = color; // RA2: as above
			}
			if (_stricmp(name, Session.Handle) == 0) { // RA2: as above
				IsColorChangePending = 0; // RA2: as above
			}
			if (g_pChat != NULL && Assign_House_And_Color(Wstring(name), country, color)) { // RA2: as above
				changed = true; // RA2: as above
			} else if (g_pChat == 0 && Net2SetHouseAndColor(name, country, color) != 0) { // RA2: as above
				changed = true; // RA2: as above
			}
			address = 0; // RA2: as above
		}
	}
	{
		char before[64]; // RA2: as above
		before[0] = 0; // RA2: as above
		for (int i = 0; i < 8; i++) { // RA2: as above
			char number[12]; // RA2: as above
			sprintf(number, "%d,", Session.Options.AISlots.Countries[i]); // RA2: as above
			strcat(before, number); // RA2: as above
		}
		int count = 0; // RA2: as above
		for (int j = 0; j < 8; j++) { // RA2: as above
			token = strtok(NULL, ",:"); // RA2: as above
			if (token == NULL) { // RA2: as above
				break; // RA2: as above
			}
			Session.Options.AISlots.Difficulties[j] = atol(token); // RA2: as above
			token = strtok(NULL, ",:"); // RA2: as above
			if (token == NULL) { // RA2: as above
				break; // RA2: as above
			}
			Session.Options.AISlots.Countries[j] = atol(token); // RA2: as above
			token = strtok(NULL, ",:"); // RA2: as above
			if (token == NULL) { // RA2: as above
				break; // RA2: as above
			}
			Session.Options.AISlots.Colors[j] = atol(token); // RA2: as above
			if (Session.Options.AISlots.Countries[j] != -1) { // RA2: as above
				count++; // RA2: as above
			}
		}
		Session.Options.AIPlayers = count; // RA2: as above
		char after[64]; // RA2: as above
		after[0] = 0; // RA2: as above
		for (int k = 0; k < 8; k++) { // RA2: as above
			char number[12]; // RA2: as above
			sprintf(number, "%d,", Session.Options.AISlots.Countries[k]); // RA2: as above
			strcat(after, number); // RA2: as above
		}
		if (strcmp(before, after) != 0) { // RA2: as above
			changed = true; // RA2: as above
		}
	}
	free(copy); // RA2: as above
	if (Session.Type != GAME_INTERNET || !Is_Tournament_Session()) { // RA2: as above
		Fill_Options_Dialog(GameoptWindow(), 0); // RA2: as above
	}
	if (_last_game_speed != Session.Options.GameSpeed) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (_last_credits != Session.Options.Credits) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (_last_unit_count != Session.Options.UnitCount) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (_last_short_game != Session.Options.ShortGame) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (_last_super_weapons != Session.Options.SWAllowed) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (_last_build_off_ally != Session.Options.BuildOffAlly) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (_last_mcv_redeploy != Session.Options.MCVRedeploy) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (_last_goodies != Session.Options.Goodies) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (!same_scenario || changed) { // RA2: as above
		int result; // RA2: as above
		if (g_pChat != NULL) { // RA2: as above
			result = GetPlayerAccepted(NULL); // RA2: as above
		} else {
			result = Net2GetAccept(0); // RA2: as above
		}
		Handle_Reset_Ready(); // RA2: the settings change clears the reminder and updates the active lobby.
		if (result == 1) { // RA2: as above
			if (g_pChat != NULL) { // RA2: as above
				SetPlayerAccepted(NULL, 0); // RA2: as above
			} else {
				Net2SetAccept(0, 0); // RA2: as above
			}
			char buffer[64]; // RA2: as above
			sprintf(buffer, "A0"); // RA2: as above
			Send_Host_Option(buffer); // RA2: as above
			if (LobbyDialogFlag) { // RA2: as above
				EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), TRUE); // RA2: as above
			} else {
				EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), FALSE); // RA2: as above
			}
			InvalidateRect(GetDlgItem(GameoptWindow(), IDC_ACCEPT), NULL, FALSE); // RA2: as above
		} else if (!IsWindowEnabled(GetDlgItem(GameoptWindow(), IDC_ACCEPT)) && LobbyDialogFlag) { // RA2: as above
			EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), TRUE); // RA2: as above
			InvalidateRect(GetDlgItem(GameoptWindow(), IDC_ACCEPT), NULL, FALSE); // RA2: as above
		}
	}
	if (dialog_id != IDD_MPLAYER_HOST && dialog_id != IDD_MPLAYER_GUEST) { // RA2: as above
		SendDlgItemMessage(GameoptWindow(), IDC_USERS, OD_DISABLEPAINT, 0, 0); // RA2: as above
	}
	if (g_pChat != NULL) { // RA2: as above
		Redraw_Player_List(); // RA2: as above
	} else {
		Net2DisplayUsers(); // RA2: as above
	}
	_last_game_speed = Session.Options.GameSpeed; // RA2: as above
	_last_credits = Session.Options.Credits; // RA2: as above
	_last_unit_count = Session.Options.UnitCount; // RA2: as above
	_last_short_game = Session.Options.ShortGame; // RA2: as above
	_last_super_weapons = Session.Options.SWAllowed; // RA2: as above
	_last_build_off_ally = Session.Options.BuildOffAlly; // RA2: as above
	_last_mcv_redeploy = Session.Options.MCVRedeploy; // RA2: as above
	_last_goodies = Session.Options.Goodies; // RA2: as above
	return(!same_scenario); // RA2: as above
}
#endif


/// <summary>
/// Fetches the text of a list box item as a wide string.
/// A list box that keeps wide text answers message 0x4CF; the others hand out their narrow text,
/// which is widened here.
/// </summary>
static inline void LBGetText(HWND listbox, int index, wchar_t * wide, char * narrow) // RA2: ra2-decomp's file-local helper of the same name (src/netshare.cpp)
{
	if (SendMessage(listbox, WM_USER + 207, index, 0) != 0) {
		SendMessage(listbox, OD_GETTEXTW, index, (LPARAM)wide);
	} else {
		SendMessage(listbox, OD_GETTEXTA, index, (LPARAM)narrow);
		Char_To_Wide(wide, narrow, -1);
	}
}


inline void Select_List_Row_Text(HWND listbox, wchar_t const * text) // RA2: the executable keeps this sequence inline in LBRestoreSelections
{
	int index = SendMessage(listbox, WM_USER + 202, -1, (LPARAM)text); // RA2: message 0x4CA finds the item by its wide text
	if (index != LB_ERR) { // RA2: as above
		ListBox_SetCurSel(listbox, index); // RA2: as above
	}
}


/// <summary>
/// Saves the current selection of a list box.
/// The multiplayer dialogs rebuild their list boxes from scratch whenever the game state
/// changes. Call this routine first so that whatever the player had highlighted can be put
/// back afterwards.
/// </summary>
/// <param name="listbox">The list box to record the selection of.</param>
/// <param name="lbdict">The dictionary to record the selected entries into.</param>
void LBSaveSelections(HWND listbox, Dictionary<WideWstring,bool> & lbdict) // RA2: 0x005C5FF0, the dictionary and the key are wide
{
	int count = ListBox_GetCount(listbox);
	WideWstring key; // RA2: the key is a wide string
	wchar_t wide[1024]; // RA2: the item text is fetched wide, through the narrow text when the list box has no wide text
	char buffer[1024]; // RA2: as above

	if (count == 0) {
		return;
	}
	int style = GetWindowLong(listbox, GWL_STYLE);
	if (style & LBS_MULTIPLESEL) {
		for (int i = 0; i < count; i++) {
			if (ListBox_GetSel(listbox, i) != 0) {
				if (SendMessage(listbox, WM_USER + 207, i, 0)) { // RA2: the text of the item is fetched wide, through the narrow text when the list box has no wide text (written out here, as ra2-decomp's twin has it)
					SendMessage(listbox, OD_GETTEXTW, i, (LPARAM)wide); // RA2: message 0x4B6, as Get_List_Item_Wide_Text
				} else {
					SendMessage(listbox, OD_GETTEXTA, i, (LPARAM)buffer); // RA2: message 0x4B7, as Get_List_Item_Text
					Char_To_Wide(wide, buffer, -1); // RA2: 0x006FA0C0
				}
				key = wide; // RA2: as above
				bool value = true;
				lbdict.add(key, value);
			}
		}
	} else if (!(style & LBS_NOSEL)) {
		int index = ListBox_GetCurSel(listbox);
		if (index >= 0) {
			wide[0] = '\0'; // RA2: as above
			LBGetText(listbox, index, wide, buffer); // RA2: as above
			key = wide; // RA2: as above
			bool value = true;
			lbdict.add(key, value);
		}
	}
}


/// <summary>
/// Restores a list box selection that was saved earlier.
/// This routine is the other half of LBSaveSelections. Call it once the list box has been
/// refilled to put the player's highlight back where it was.
/// </summary>
/// <param name="listbox">The list box to restore the selection within.</param>
/// <param name="lbdict">The dictionary the selection was saved into.</param>
void LBRestoreSelections(HWND listbox, Dictionary<WideWstring,bool> & lbdict) // RA2: 0x005C6310, the dictionary and the key are wide
{
	wchar_t wide[1024]; // RA2: the item text is fetched wide
	char buffer[1024]; // RA2: as above
	int count = ListBox_GetCount(listbox);
	WideWstring key; // RA2: the key is a wide string

	if (count) {
		int style = GetWindowLong(listbox, GWL_STYLE);
		if (style & LBS_MULTIPLESEL) {
			for (int i = 0; i < count; i++) {
				if (SendMessage(listbox, WM_USER + 207, i, 0)) { // RA2: the text of the item is fetched wide, through the narrow text when the list box has no wide text
					SendMessage(listbox, OD_GETTEXTW, i, (LPARAM)wide); // RA2: message 0x4B6, as Get_List_Item_Wide_Text
				} else {
					SendMessage(listbox, OD_GETTEXTA, i, (LPARAM)buffer); // RA2: message 0x4B7, as Get_List_Item_Text
					Char_To_Wide(wide, buffer, -1); // RA2: 0x006FA0C0
				}
				key = wide; // RA2: as above
				if (lbdict.contains(key)) {
					ListBox_SetSel(listbox, TRUE, i);
				}
			}
		} else if (!(style & LBS_NOSEL)) {
			bool value;
			if (lbdict.removeAny(key, value)) {
				wcscpy(wide, key.get()); // RA2: as above
				Select_List_Row_Text(listbox, wide); // RA2: as above
			}
		}
	}
}


/// <summary>
/// Fetches how many players the lobby takes.
/// An Internet game takes fewer in a restricted channel; anything else takes the full eight.
/// </summary>
/// <returns>Returns with the number of slots the lobby offers.</returns>
int Get_Lobby_Slot_Limit(void) // RA2: 0x005C6520, not in TibSun; a restricted Internet channel takes two players or four, other games eight
{
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		if (Is_Current_Channel_Tournament() && !Is_Current_Channel_Flag_8()) { // RA2: as above
			return(2); // RA2: as above
		}
		return(4); // RA2: as above
	}
	return(8); // RA2: as above
}


int Get_Lobby_Slot_Limit_Plus_One(void) // RA2: 0x005C6550, not in TibSun; the limit with room for one observer
{
	int limit = Get_Lobby_Slot_Limit(); // RA2: as above
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		if (limit >= 4) { // RA2: as above
			return(limit); // RA2: as above
		}
	} else if (limit >= 8) { // RA2: as above
		return(limit); // RA2: as above
	}
	return(limit + 1); // RA2: as above
}


int RandomMapWaypointCount(int index); // RA2: defined below, after its caller as in the executable

/// <summary>
/// Fetches how many players a scenario takes.
/// </summary>
/// <param name="index">Index into the multiplayer scenario list, -1 for none.</param>
/// <returns>Returns with the number of slots.</returns>
int __fastcall Get_Scenario_Slot_Limit(int index) // RA2: 0x005C65A0, not in TibSun; how many players a scenario takes in this game
{
	if (index == -1) { // RA2: as above
		return(8); // RA2: as above
	}
	int limit = RandomMapWaypointCount(index); // RA2: as above
	if (Session.MPGameMode != NULL) { // RA2: as above
		int mode_limit = Session.MPGameMode->vt_entry_80(); // RA2: as above
		if (mode_limit != -1 && mode_limit < limit) { // RA2: as above
			limit = mode_limit; // RA2: as above
		}
	}
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		if (Is_Current_Channel_Flag_8()) { // RA2: as above
			limit &= ~1; // RA2: as above
		} else if (Is_Current_Channel_Tournament()) { // RA2: as above
			limit = 2; // RA2: as above
		}
	}
	return(limit); // RA2: as above
}


/// <summary>
/// Fetches the number of starting positions a scenario offers.
/// This routine is used to check that there is somewhere to put every player before a map is
/// allowed into a multiplayer game.
/// </summary>
/// <param name="index">Index into the multiplayer scenario list.</param>
/// <returns>Returns with the number of start positions on the map. Zero is returned if the
/// scenario could not be read.</returns>
int RandomMapWaypointCount(int index)
{
	char wp[32];
	char buf[132];
	if (index >= 0 && index < Session.Scenarios.Count()) {
		strcpy(buf, Session.Scenarios[index]->Get_Filename());
		CCFileClass file(buf);

		INIClass ini;
		if (!ini.Load(file)) {
			return(0);
		}

		int count = 0;

		for (int i = 0; i < MAX_PLAYERS; i++) {
			sprintf(wp, "%d", i);
			if (ini.Get_Int("Waypoints", wp, -1) != -1) {
				count++;
			}
		}

		if (count == 0) {
			count = ini.Get_Int("RandomMap", "NumPlayers", 0);
		}

		return(count);
	}
	return(0);
}


int CALLBACK Scenario_DlgProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, as msgbox.cpp

static int LastPreviewedScenario; // RA2: 0x00A728D0
static HWND ScenarioPick; // RA2: 0x00A728E0
static int OriginalScenario; // RA2: 0x00A72C80
static wchar_t PickedScenarioDescription[DESCRIP_MAX]; // RA2: 0x00A72A68, the description of the map highlighted in the list (0 if none); Scenario_DlgProc passes it on


int Get_Selected_Scenario_Row(void) // RA2: 0x005C67A0, the highlighted row of the map list (the executable also keeps this call inline in Scenario_Select_Callback)
{
	return(SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETCURSEL, 0, 0)); // RA2: as above
}


// RA2: the map dialog and its idle callback both keep the highlighted scenario name.
static inline void Save_Scenario_Name(void)
{
	HWND list = ScenarioPick;
	int sel = SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETCURSEL, 0, 0);
	if (sel != -1) {
		MultiMission * title = (MultiMission *)SendDlgItemMessage(list, IDC_AILEVEL_SLIDER, LB_GETITEMDATA, sel, 0);
		if (title != NULL) {
			wcscpy(PickedScenarioDescription, title->Description());
			return;
		}
	}
	PickedScenarioDescription[0] = 0;
}


/// <summary>
/// Handles the idle processing while the map selection dialog is up.
/// This routine keeps the preview in step with whichever map is highlighted and pumps the
/// network layer the session is using, so that a game sitting in the lobby does not stall
/// while the host browses for a scenario.
/// </summary>
/// <returns>bool; Should the dialog be shut down?</returns>
bool Scenario_Select_Callback(void)
{
	int row = SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETCURSEL, 0, 0);
	if (row != -1) { // RA2: the list items carry the MultiMission, the scenario index is searched for
		MultiMission * mission = (MultiMission *)SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETITEMDATA, row, 0); // RA2: as above
		int index = -1; // RA2: the scenario is searched for by hand
		for (int i = 0; i < Session.Scenarios.Count(); i++) { // RA2: as above
			if (mission == Session.Scenarios[i]) { // RA2: as above
				index = i; // RA2: as above
				break; // RA2: as above
			}
		}
		if (index != -1 && index != LastPreviewedScenario) {
			Set_Scenario_Info_From_Index(index);
			if (Session.Scenarios[index]->Is_Random_Map()) { // RA2: the random map is told by its flag, not by its file name
				delete MultiplayerMapPreview;
				MultiplayerMapPreview = new MapPreviewClass;
				MultiplayerMapPreview->Read_PCX_Preview("RandMap.img");
				if (MultiplayerMapPreview->Get_Preview_Surface() == NULL) {
					Update_Network_Dialog_Preview(ScenarioPick);
				}
				InvalidateRect(ScenarioPick, NULL, FALSE);
			} else {
				Update_Network_Dialog_Preview(ScenarioPick);
			}
			LastPreviewedScenario = index;
			Session.Options.ScenarioIndex = OriginalScenario;
			Set_Scenario_Info_From_Index(OriginalScenario);
		}
	}
	Save_Scenario_Name();
	if (Session.Type == GAME_IPX) { // RA2: IPX (3) takes the network callback, the Internet game its own
		return(Net2Callback());
	}
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		return(WOL_Wait_Callback());
	}
	if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
		return(ModemHostCallback());
	}
	Call_Back();
	return(false);
}


/// <summary>
/// Brings up the multiplayer map selection dialog.
/// Use this routine to let the host choose the scenario for the game. The dialog does not
/// return until the player settles on a map or backs out.
/// </summary>
/// <param name="top">The window to parent and center the dialog against.</param>
/// <returns>Returns with the control ID that dismissed the dialog, either IDOK or
/// IDCANCEL.</returns>
int Scenario_Dialog(HWND top)
{
	Darken_Surface(NULL, true); // RA2: the screen behind is dimmed instead of redrawing the menu background
	ScenarioPick = WS_Create_Dialog(ProgramInstance, 107, top, (DLGPROC)Scenario_DlgProc, FALSE); // RA2: the map dialog is resource 107 in RA2 (112 in TibSun)
	Show_Dialog(ScenarioPick, 107); // RA2: as above
	SendMessage(ScenarioPick, OD_SETTOP, 0, 1); // RA2: message 0x4A9
	ShowWindow(ScenarioPick, SW_NORMAL);
	return(WS_Wait_Dialog(ScenarioPick, Scenario_Select_Callback, false, true, true)); // RA2: as above
}


static bool MapListBusy; // RA2: 0x00A72D58, set while the dialog refills its lists
static bool ModeWarningShown; // RA2: 0x00A72D59, the game mode's start conditions were reported

bool __fastcall Accept_Map_Selection(HWND dialog); // RA2: 0x005C71F0, defined below
int __fastcall Select_List_Item_By_Data(HWND dialog, int data); // RA2: 0x005C7160, defined below
void __fastcall Fill_Game_Mode_List(HWND list, int mode_index); // RA2: 0x005B71E0, fills the game mode list and selects the mode
void __fastcall Dialog_On_OD_4EC(HWND dialog); // RA2: 0x005E8710, message 0x4EC
namespace OwnerDraw { bool __fastcall WS_Save_Dialog_Values(HWND dialog); } // RA2: 0x005E5FC0, as netdlg2.cpp
namespace OwnerDraw { bool __fastcall Play_Dialog_Move_In(HWND dialog); } // RA2: 0x005E61B0, the dialog slides in, defined in ownrdraw.cpp

// RA2: the helpers of the map dialog, ra2-decomp's file-local helpers of the same names (src/netshare.cpp)
static inline MultiplayerGameMode * Get_Selected_Game_Mode(HWND window)
{
	int selected = SendDlgItemMessage(window, 1771, LB_GETCURSEL, 0, 0);
	if (selected == -1) return(NULL);
	return((MultiplayerGameMode *)SendDlgItemMessage(window, 1771, LB_GETITEMDATA, selected, 0));
}


static inline int Select_Scenario_By_Name(HWND window, wchar_t const * name)
{
	if (name == NULL) {
		return(-1);
	}
	int found = 0;
	int count = SendDlgItemMessage(window, IDC_AILEVEL_SLIDER, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < count; i++) {
		MultiMission * candidate = (MultiMission *)SendDlgItemMessage(window, IDC_AILEVEL_SLIDER, LB_GETITEMDATA, i, 0);
		if (wcscmp(candidate->Description(), name) == 0) {
			found = i;
		}
	}
	SendDlgItemMessage(window, IDC_AILEVEL_SLIDER, LB_SETCURSEL, found, 0);
	SendDlgItemMessage(window, IDC_AILEVEL_SLIDER, LB_SETTOPINDEX, found, 0);
	return(found);
}

static inline void Select_Scenario_Index(HWND window, int index)
{
	if (index < Session.Scenarios.Count()) {
		Select_List_Item_By_Data(window, (int)Session.Scenarios[index]);
	}
}

static inline void Enable_Random_Scenario(HWND window, MultiplayerGameMode * mode)
{
	bool allowed = mode->IsAIAllowed();
	HWND button = GetDlgItem(window, IDC_CREATE_RANDOM_MAP);
	EnableWindow(button, allowed);
}

static inline void Populate_Scenario_List(HWND window, MultiplayerGameMode * mode)
{
	DynamicVectorClass<MultiMission const *> maps;
	int count = Session.Scenarios.Count();
	for (int i = 0; i < count; i++) {
		MultiMission const * map = Session.Scenarios[i];
		if (mode->IsMapAllowed(map)) {
			maps.Add(map);
		}
	}
	HWND list = GetDlgItem(window, IDC_AILEVEL_SLIDER);
	mode->DrawTeamDropdown(list, &maps, NULL);
}


/// <summary>
/// Handles the messages for the multiplayer map selection dialog.
/// This routine fills the map list, paints the preview of the highlighted map, and services
/// the random map generator button.
/// </summary>
/// <returns>Returns with TRUE if the message was dealt with here, FALSE to leave it to the
/// dialog manager.</returns>
#ifdef RA2_NON_MATCHING // not exact: 1219 bytes differ by position, ours 96 bytes shorter (1,212 before the shared ToolTipTextRequest replaced a second declaration of the 0x4E9 request; 1,624 before that, main-derived text); ra2-decomp's twin text (Yuri's Revenge 0x005E6920, matched there) with its file-local helpers, in this tree's names: the calls and their order are the executable's up to the mode list. There the executable calls the DynamicVectorClass constructor and expands the vector's Clear where ours expands the constructor and calls Clear, and at the dialog's start it expands Select_List_Item_By_Data where ours calls it: an inline budget difference. Scratch probes: five or six more empty inline call sites at the start of the dialog's start case give the executable's calls everywhere; no reference text has them
int CALLBACK Scenario_DlgProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005C69E0, ra2-decomp's twin text (Yuri's Revenge 0x005E6920) in this tree's names
{
	switch (message) {
		case WM_DRAWITEM:
			Dialog_On_WM_DRAWITEM(lparam);
		// Fall through: both messages have been handled.
		case WM_ERASEBKGND:
			return(TRUE);

		case WM_PAINT:
			Dialog_On_WM_PAINT(window); // RA2: the dialog is painted by the owner draw paint (0x005FEE70), not OwnerDraw::Draw_Dialog_Back
			if (MultiplayerMapPreview) MultiplayerMapPreview->Blit_Preview(window);
			Slide_Pending_Dialog(window); // RA2: a dialog waiting to slide in does so now (0x005E5F20)
			ValidateRect(window, NULL);
			break;

		case WM_NCDESTROY:
			On_WM_NCDESTROY(window);
			break;

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case IDC_CANCEL:
					WS_Destroy_Dialog(window, 2);
					break;
				case IDC_CREATE_RANDOM_MAP: {
					OwnerDraw::WS_Save_Dialog_Values(window); // RA2: the dialog's values are saved before it is hidden (0x005E5FC0)
					ShowWindow(window, SW_HIDE);
					int scenario = CreateRandomMap();
					if (scenario != -1) {
						MultiplayerGameMode * mode = Get_Selected_Game_Mode(window);
						if (mode) {
							Populate_Scenario_List(window, mode);
				}
						Select_Scenario_Index(window, scenario);
						Set_Scenario_Info_From_Index(scenario);
						if (!MultiplayerMapPreview->Get_Preview_Surface()) Update_Network_Dialog_Preview(window);
						Session.Options.ScenarioIndex = OriginalScenario;
						Set_Scenario_Info_From_Index(OriginalScenario);
						bool accepted = Accept_Map_Selection(window);
						Darken_Surface(NULL, true);
						if (accepted) break;
			}
					OwnerDraw::Play_Dialog_Move_In(window);
					ShowWindow(window, SW_SHOW);
					break;
		}
				case 1733: // RA2: the accept button
					Accept_Map_Selection(window);
					break;
				case 1771: // RA2: the game mode list
					if (!MapListBusy && HIWORD(wparam) == LBN_SELCHANGE) {
						MultiplayerGameMode * mode = Get_Selected_Game_Mode(window);
						if (mode) {
							WideWstring error;
							if (mode->vt_entry_0C(&error)) {
								mode->AssignTeams();
								Populate_Scenario_List(window, mode);
								Enable_Random_Scenario(window, mode);
								Select_Scenario_By_Name(window, PickedScenarioDescription);
								Save_Scenario_Name();
								ModeWarningShown = false;
							} else {
								if (!ModeWarningShown) {
									ODMessageBox(error.get(), 0, NULL, false);
									ModeWarningShown = true;
						}
								SendDlgItemMessage(window, 1771, LB_SETCURSEL, 0, 0);
					}
				}
			}
					break;
		}
			break;

		case 0x4EC:
			Dialog_On_OD_4EC(window);
			break;

		case 0x4E9: { // RA2: the tool tip of a mode in the list
			ToolTipTextRequest * request = (ToolTipTextRequest *)lparam;
			int id = GetDlgCtrlID(request->Window);
			request->Text = (wchar_t const *)NULL;
			if (id == 1771 && request->Item != -1) {
				MultiplayerGameMode * mode = (MultiplayerGameMode *)SendDlgItemMessage(window, id, LB_GETITEMDATA, request->Item, 0);
				if (mode) request->Text = mode->CSFTooltip.text();
		}
			return(TRUE);
	}

	case OD_SUBCLASSED: {
		MapListBusy = true;
		MultiplayerGameMode * mode = Get_Current_Game_Mode();
		HWND list = GetDlgItem(window, 1771);
		Fill_Game_Mode_List(list, mode->MPModeIndex);
		if (mode) {
			Populate_Scenario_List(window, mode);
		}
		Enable_Random_Scenario(window, mode);
		if (Session.Options.ScenarioIndex != -1) {
			Select_Scenario_Index(window, Session.Options.ScenarioIndex);
		}
		OriginalScenario = Session.Options.ScenarioIndex;
		LastPreviewedScenario = -1;
		MapListBusy = false;
		break;
	}
	}
	return(FALSE);
}
#endif


/// <summary>
/// Selects the item of the map list that carries the given data.
/// The list is searched for the item whose data is the pointer, highlighted and scrolled to.
/// </summary>
/// <param name="dialog">The map dialog.</param>
/// <param name="data">The item data to look for, the MultiMission of the map.</param>
/// <returns>Returns with the index of the item, 0 if no item has the data, -1 without data.</returns>
int __fastcall Select_List_Item_By_Data(HWND dialog, int data) // RA2: 0x005C7160, not in TibSun
{
	if (data == 0) { // RA2: as above
		return(-1); // RA2: as above
	}
	int found = 0; // RA2: as above
	int count = SendDlgItemMessage(dialog, IDC_AILEVEL_SLIDER, LB_GETCOUNT, 0, 0); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		if (SendDlgItemMessage(dialog, IDC_AILEVEL_SLIDER, LB_GETITEMDATA, i, 0) == data) { // RA2: as above
			found = i; // RA2: as above
		}
	}
	SendDlgItemMessage(dialog, IDC_AILEVEL_SLIDER, LB_SETCURSEL, found, 0); // RA2: as above
	SendDlgItemMessage(dialog, IDC_AILEVEL_SLIDER, LB_SETTOPINDEX, found, 0); // RA2: as above
	return(found); // RA2: as above
}


/// <summary>
/// Takes the map and the game mode the map dialog has selected.
/// When the map holds fewer players than the lobby, the game mode's test may ask first whether
/// the extra players are to be ejected; Cancel keeps the old settings. The new mode replaces the
/// old one, the dialog ends and the game options dialog shows the new mode and map.
/// </summary>
/// <param name="dialog">The map dialog.</param>
/// <returns>Returns with true if the selection was taken.</returns>
bool __fastcall Accept_Map_Selection(HWND dialog) // RA2: 0x005C71F0, not in TibSun (TibSun's IDOK only stores the index)
{
	int row = SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETCURSEL, 0, 0); // RA2: as above
	int index = -1; // RA2: as above
	if (row != -1) { // RA2: as above
		MultiMission * picked = (MultiMission *)SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETITEMDATA, row, 0); // RA2: as above
		for (int i = 0; i < Session.Scenarios.Count(); i++) { // RA2: as above
			if (picked == Session.Scenarios[i]) { // RA2: as above
				index = i; // RA2: as above
				break; // RA2: as above
			}
		}
	}
	if (index == -1) { // RA2: as above
		return(false); // RA2: as above
	}
	MultiplayerGameMode * old_mode = Session.MPGameMode; // RA2: the limit is computed as if the new mode were already set
	HWND lobby = ScenarioPick; // RA2: as above
	MultiplayerGameMode * mode = NULL; // RA2: as above
	int selected = SendDlgItemMessage(ScenarioPick, 1771, LB_GETCURSEL, 0, 0); // RA2: as above
	if (selected != -1) { // RA2: as above
		mode = (MultiplayerGameMode *)SendDlgItemMessage(lobby, 1771, LB_GETITEMDATA, selected, 0); // RA2: as above
	}
	Session.MPGameMode = mode; // RA2: as above
	bool skirmish_allowed = mode->IsSkirmishAllowed(); // RA2: as above
	bool flag = mode->vt_entry_A0(); // RA2: as above
	int players = RandomMapWaypointCount(index); // RA2: as above
	if (Session.MPGameMode != NULL) { // RA2: as above
		int limit = Session.MPGameMode->vt_entry_80(); // RA2: as above
		if (limit != -1 && limit < players) { // RA2: as above
			players = limit; // RA2: as above
		}
	}
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		if (Is_Current_Channel_Flag_8()) { // RA2: as above
			players &= ~1; // RA2: as above
		} else if (Is_Current_Channel_Tournament()) { // RA2: as above
			players = 2; // RA2: as above
		}
	}
	Session.MPGameMode = old_mode; // RA2: as above
	bool computers = false; // RA2: as above
	bool ask; // RA2: as above
	switch (Session.Type) { // RA2: as above
		case GAME_IPX:
			ask = Update_Lobby_Count(players, skirmish_allowed, flag); // RA2: as above
			break;

		case GAME_INTERNET:
			ask = Wol_Update_Lobby_Count(players, skirmish_allowed, flag); // RA2: 0x0075CA30, as above
			break;

		case GAME_SKIRMISH:
			ask = Skirmish_Has_Computer_After(players); // RA2: as above
			computers = true; // RA2: as above
			break;

		default:
			ask = false; // RA2: as above
			break;
	}
	if (ask) { // RA2: as above
		wchar_t const * text = StringTable::LoadString(!computers ? "GUI:EjectPlayers" : "GUI:EjectAIPlayers", NULL, "D:\\Ra2\\netshare.cpp", 2655); // RA2: as above
		wchar_t const * ok = StringTable::LoadString("GUI:Ok", NULL, "D:\\Ra2\\netshare.cpp", 2656); // RA2: as above
		wchar_t const * cancel = StringTable::LoadString("GUI:Cancel", NULL, "D:\\Ra2\\netshare.cpp", 2657); // RA2: as above
		if (WWMessageBox(L" ").Process(text, ok, cancel, NULL, false)) { // RA2: as above
			return(false); // RA2: as above
		}
	}
	if (mode != Session.MPGameMode) { // RA2: as above
		if (Session.MPGameMode != NULL) { // RA2: as above
			Session.MPGameMode->vt_entry_84(); // RA2: as above
		}
		if (mode != NULL) { // RA2: as above
			Session.MPGameMode = mode; // RA2: as above
			Session.Options.MPModeIndex = mode->MPModeIndex; // RA2: as above
			Session.Options.ScenarioIndex = index; // RA2: as above
			mode->vt_entry_20(); // RA2: as above
		}
	}
	Session.Options.ScenarioIndex = index; // RA2: as above
	if (mode != NULL) { // RA2: as above
		mode->vt_entry_24(); // RA2: as above
	}
	mode->AssignTeamsAtStart(); // RA2: as above
	mode->UnfixAlliances(); // RA2: as above
	WS_Destroy_Dialog(dialog, 1); // RA2: as above
	HWND setup = WS_Find_Dialog(IDD_WOL_GAMEOPT); // RA2: the options dialog that is up takes the new choice
	if (setup == NULL) { // RA2: as above
		setup = WS_Find_Dialog(IDD_WOL_GUEST); // RA2: as above
		if (setup == NULL) { // RA2: as above
			setup = WS_Find_Dialog(IDD_MPLAYER_HOST); // RA2: as above
			if (setup == NULL) { // RA2: as above
				setup = WS_Find_Dialog(IDD_MPLAYER_GUEST); // RA2: as above
			}
		}
	}
	HWND control = GetDlgItem(setup, 1772); // RA2: as above
	SendMessage(control, OD_SETTEXTW, 0, (LPARAM)mode->UIName.text()); // RA2: as above
	MultiMission * current = Session.Scenarios[Session.Options.ScenarioIndex]; // RA2: as above
	control = GetDlgItem(setup, IDC_SCENARIONAME); // RA2: as above
	SendMessage(control, OD_SETTEXTW, 0, (LPARAM)current); // RA2: the description is the first member of the MultiMission
	mode->vt_entry_50(); // RA2: as above
	return(true); // RA2: as above
}


/// <summary>
/// Performs the last setup step before a multiplayer game begins.
/// This routine copies the agreed session options into the globals the game logic actually
/// reads, so that every machine starts the scenario with the same rules in force.
/// </summary>
void PregameSetup(void)
{
	Scen->Scenario = Session.Options.ScenarioIndex;
	Frame = 0;
	Session.NumPlayers = Session.Players.Count();
	DebugString("Pregame setup for %d players.\n", Session.NumPlayers);
	Options.GameSpeed = Session.Options.GameSpeed;
	Session.CommProtocol = DEFAULT_COMM_PROTOCOL;
	Special.IsHarvesterImmune = Session.Options.HarvTruce;
	Special.IsDestroyBridges = Session.Options.BridgeDestruction;
	Special.IsTGrowth = true;
	Special.IsTSpread = true;
	Special.Apply_To_Game();
}


/// <summary>
/// Updates the map preview shown in a network game dialog.
/// This routine is called whenever the selected scenario changes. A guest that does not have
/// the scenario locally asks the host for a preview instead of building one, so the picture
/// may not appear until that download arrives.
/// </summary>
/// <param name="win">The dialog window that displays the preview.</param>
void Update_Network_Dialog_Preview(HWND win)
{
	delete MultiplayerMapPreview;
	MultiplayerMapPreview = NULL;

	switch (Session.Type) {
		case GAME_IPX:
			if (WS_Top_Window_ID() == IDD_MPLAYER_GUEST && !Find_Local_Scenario(Session.ScenarioFileName, Session.ScenarioFileLength, Session.ScenarioDigest, Session.ScenarioIsOfficial, false)) {
				InvalidateRect(win, NULL, FALSE); // RA2: the preview is cleared while the request is out
				GlobalPacketType packet;
				memset(&packet, 0, sizeof(packet));
				packet.Command = NET_REQ_PREVIEW;
				while (true) {
					int sent;
					if (Session.Type == GAME_IPX) { // RA2: the request goes to the host's address, or by the host's name in an Internet game
						sent = Ipx.Send_Global_Message(&packet, 0x2F, 1, &Session.HostAddress, 0, 0); // RA2: as above
					} else {
						User * host = Find_Owner_User(); // RA2: as above
						sent = 0; // RA2: as above
						if (host != NULL) { // RA2: as above
							sent = Ipx.Send_Global_Message_To_Name(&packet, 0x2F, 1, Narrow_To_Wide((char const *)host->name)); // RA2: as above
						}
					}
					if (sent) {
						break;
					}
					Call_Back();
				}
				while (Ipx.Global_Num_Send() != 0) {
					Call_Back();
				}
				return;
			}
			break;

		case GAME_INTERNET:
			if (WS_Find_Dialog(IDD_WOL_GUEST) != NULL) { // RA2: the preview of an Internet game is asked for only while the guest dialog is up; TibSun's owner test and the status texts are gone
				bool present = false; // RA2: the scenario file is looked for in the local directory
				if (Find_Local_Scenario(Session.ScenarioFileName, Session.ScenarioFileLength, Session.ScenarioDigest, Session.ScenarioIsOfficial, false)) {
					char name[128]; // RA2: the file name is checked for the .MPR extension, upper case
					strcpy(name, Session.ScenarioFileName); // RA2: as above
					strupr(name); // RA2: as above
					DebugString("Checking for file %s in local directory\n", name); // RA2: as above
					if (!Session.ScenarioIsOfficial && strstr(name, ".MPR") != NULL) { // RA2: as above
						break; // RA2: as above
					}
					{
						RawFileClass r(Session.ScenarioFileName);
						if (r.Is_Available()) {
							present = true;
						}
					}
					if (!present) { // RA2: as above
						break; // RA2: as above
					}
				}
				if (!Session.Is_Random_Map() && MultiplayerMapPreview != NULL) { // RA2: as above
					delete MultiplayerMapPreview;
					MultiplayerMapPreview = NULL;
					InvalidateRect(win, NULL, FALSE);
					return;
				}
				InvalidateRect(win, NULL, FALSE);
				return;
			}
			break;

		case GAME_MODEM:
		case GAME_NULL_MODEM:
			if (!Find_Local_Scenario(Session.ScenarioFileName, Session.ScenarioFileLength, Session.ScenarioDigest, Session.ScenarioIsOfficial, false)) {
				SerialPacketType packet;
				memset ((void*)&packet, 0, sizeof (packet));
				packet.Command = SERIAL_REQ_PREVIEW;

				CDTimerClass<SystemTimerClass> response_timer = PACKET_SENDING_TIMEOUT;
				while (response_timer) {
					int sent = NullModem.Send_Message(&packet, sizeof(packet), 1);
					while (NullModem.Num_Send() > 0 && response_timer) {
						Call_Back();
					}
					if (sent) break;
				}
				return;
			}
			break;
	}

	if (MultiplayerMapPreview != NULL) { // RA2: the old preview goes first, then the file is looked for
		delete MultiplayerMapPreview;
		MultiplayerMapPreview = NULL;
	}
	bool unavailable = CCFileClass(Session.ScenarioFileName).Is_Available() == false;
	if (!unavailable) { // RA2: a missing file leaves the preview empty and the window as it is
		MultiplayerMapPreview = new MapPreviewClass;
	}
	if (MultiplayerMapPreview != NULL) {
		MultiplayerMapPreview->Read_INI_Preview(Session.ScenarioFileName);
		InvalidateRect(win, NULL, FALSE);
	}
}


/// <summary>
/// Receives the random map preview image from the host.
/// This is the guest side of the preview handshake. The host is told that this machine is
/// ready, the compressed preview file is downloaded, and the decompressed image becomes the
/// preview shown in the multiplayer dialog.
/// </summary>
void Receive_Random_Map_Preview(void)
{
	Ipx.Set_Timing(50, -1, 5000, true); // RA2: the immediate flag is passed
	DebugString("Starting map preview download\n");

	GlobalPacketType packet;
	memset(&packet, 0, sizeof(packet));
	packet.Command = NET_PREVIEW_ACK;
	DebugString("Sending preview mode acks\n");
	while (true) {
		int sent; // RA2: the acknowledgement goes to the host's address, or by the host's name in an Internet game
		if (Session.Type == GAME_IPX) { // RA2: as above
			sent = Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.HostAddress, 0, 0); // RA2: as above
		} else {
			User * host = Find_Owner_User(); // RA2: as above
			if (host == NULL) { // RA2: as above
				break; // RA2: as above
			}
			sent = Ipx.Send_Global_Message_To_Name(&packet, sizeof(packet), 1, Narrow_To_Wide((char const *)host->name)); // RA2: as above
		}
		if (sent) {
			break;
		}
		Call_Back();
	}
	while (Ipx.Global_Num_Send() != 0) {
		Call_Back();
	}
	DebugString("Preview mode acks sent\n");

	Session.GAddress = Session.HostAddress;
	DebugString("Calling Get_File_From_Host to receive the file download\n");
	char preview_name[256];
	bool got_file = Get_File_From_Host(preview_name, GAME_IPX, false);
	if (!got_file) {
		DebugString("got_file is false. Download failed\n");
		Ipx.Set_Timing(TIMER_SECOND / 2, -1, 10 * TIMER_SECOND, true); // RA2: as above
		return;
	}

	DebugString("Loading the compressed preview image\n");
	RawFileClass file(preview_name);
	int size = file.Size();
	char * buffer = new char[size];
	file.Read(buffer, size);
	int preview_size = ((int *)buffer)[0];

	DebugString("Decompressing the preview image\n");

	BufferStraw bstraw(&((int *)buffer)[1], size);
	LZOStraw lzostraw(LZOStraw::DECOMPRESS, 0x2000); // RA2: the block size is passed
	lzostraw.Get_From(&bstraw);
	char * preview = new char[2 * preview_size];
	lzostraw.Get(preview, preview_size);

	DebugString("Creating the new preview surface\n");
	if (MultiplayerMapPreview) {
		delete MultiplayerMapPreview;
	}
	MultiplayerMapPreview = new MapPreviewClass;
	MultiplayerMapPreview->Create_Preview_Surface(preview);
	InvalidateRect(WS_Top_Window(), NULL, FALSE);

	DebugString("Cleaning up the temporary decompression buffers\n");
	delete [] preview;
	delete [] buffer;

	Ipx.Set_Timing(TIMER_SECOND / 2, -1, 10 * TIMER_SECOND, true); // RA2: as above
}


/// <summary>
/// Sets the session's scenario information from the scenario list.
/// This routine is used whenever the host picks a different map. If the scenario file is not
/// on the hard drive, the player is asked for the disk that holds it, and the selection
/// fails if that disk cannot be made available.
/// </summary>
/// <param name="index">Index into the multiplayer scenario list, or -1 to clear the
/// selection.</param>
/// <returns>bool; Was the scenario information set?</returns>
bool Set_Scenario_Info_From_Index(int index)
{
	if (index == -1) {
		Session.ScenarioFileName[0] = '\0';
		Session.ScenarioDigest[0] = '\0';
		Session.ScenarioFileLength = 0;
		Session.ScenarioIsOfficial = 0;
		Session.ScenarioMaxPlayers = 0; // RA2: the number of slots of the scenario is cleared too
		Session.Options.ScenarioDescription[0] = '\0';
		return(false);
	}

	bool unavailable = CCFileClass(Session.Scenarios[index]->Get_Filename()).Is_Available() == false;

	if (unavailable) {
		if (CD::Get_Required_Disk() != DISK_LOCAL) {
			if (!Session.Scenarios[index]->Is_On_CD(CD::Get_Current_Disk(60)) && !CD().Force_Available(Session.Scenarios[index]->On_Which_CD())) { // RA2: the current disk is asked for with a timeout
				return(false);
			}
		} else {
			WWMessageBox(L" ").Process(StringTable::LoadString("TXT_UNABLE_READ_SCENARIO", NULL, "D:\\Ra2\\netshare.cpp", 3312), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\netshare.cpp", 3312), NULL, NULL, false); // RA2: the texts come from the string table, the message box has a caption
			return(false);
		}
	}

	wcscpy(Session.Options.ScenarioDescription, Session.Scenarios[index]->Description()); // RA2: the scenario description is wide
	strcpy(Session.ScenarioDigest, (Session.Scenarios[index])->Get_Digest());
	strcpy(Session.ScenarioFileName, (Session.Scenarios[index])->Get_Filename());
	strcpy(Scen->ScenarioName, Session.ScenarioFileName);
	Session.ScenarioIsOfficial = Session.Scenarios[index]->Get_Official();
	int players = Get_Scenario_Slot_Limit(index); // RA2: the number of slots of the scenario is kept in the session
	Session.ScenarioMaxPlayers = players; // RA2: as above
	Session.ScenarioFileLength = CCFileClass(Session.ScenarioFileName).Size();
	Session.ScenarioFileLength = CCFileClass(Session.ScenarioFileName).Size();
	return(true);
}


/// <summary>
/// Sends the random map preview image to the guests.
/// Only a generated map needs this -- an ordinary scenario's preview is built from the map
/// file the guest already holds. The guests are put into preview receive mode and, once they
/// have all answered, the compressed image is handed to the remote file sender.
/// </summary>
/// <remarks>Only the game host should call this routine.</remarks>
void __fastcall Send_Map_Preview(MapPreviewClass * preview); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: against the image bound (1,325 B) 1,131 bytes differ by position (was 1,247; ra2-decomp's twin text, Yuri's Revenge 0x005E7EB0, in this tree's names): the constant 0 takes ebx for the whole function where the image keeps the packet length 455 in ebx through the two message loops and clears ebx only for the file block, so the row is one use of the constant 0 over, or one of 455 short, ahead of the file block; the node test written `== true` (3 bytes off, a compare the image does not make) is held on variation_candidates.csv (lanes F45, O58); exact in scratch with ra2-decomp's Narrow_Text (narrow.h) at the two `no preview download possible` lines, which the twin's own text of this function does not use: held on variation_candidates.csv, kind C (lane F49, owner ruling 2026-11-23)
void __fastcall Send_Map_Preview(MapPreviewClass * preview) // RA2: 0x005C7EF0, TibSun's Send_Preview_To_Guests takes no preview; it is called when a guest asks for the preview
{
	CDTimerClass<SystemTimerClass> response_timer;
	char options[256]; // RA2: the options string of an Internet game that cannot send the preview
	GlobalPacketType packet;
	GlobalPacketType response;
	bool responses[MAX_PLAYERS];
	int length;
	unsigned short product_id;
	int size;

	if (preview != NULL && Session.Players.Count() > 1) { // RA2: the preview is passed, not read from MultiplayerMapPreview; the random map test is gone
		if (Session.Type == GAME_INTERNET) { // RA2: a player behind a NAT or with different local and external addresses cannot download the preview
			if (FirewallHelper.Detected != 0
				&& !(FirewallHelper.Detected & 1)) {
			no_preview: // RA2: the host sends the options string Z3, which tells the guests there is no preview to expect
				options[0] = 'Z';
				options[1] = '3';
				options[2] = '\0';
				DebugString(
					"Sending public options string %s\n",
					options);
				g_pChat->RequestPublicGameOptions(options);
				return;
			}

			int peer = 1;
			if (peer < Session.Players.Count()) {
				do {
					int slot = Session.Player_To_Slot(peer);
					if (slot >= 0 && slot < MAX_PLAYERS) {
						int flags = Session.WolPlayerState[slot];
						if (flags == 0 || (flags & 1)) {
							if (Session.SlotAddresses[slot].
								Is_Same_Node(
									Session.Players[peer]->Address)) {
								goto next_peer;
							}
							DebugString(
								"Player %s has different local and external IPs - no preview download possible\n",
								Wide_To_Narrow(
									Session.Players[peer]->Name));
						} else {
							DebugString(
								"Player %s has a NAT - no preview download possible\n",
								Wide_To_Narrow(
									Session.Players[peer]->Name));
						}
						goto no_preview;
					}

				next_peer:
					peer++;
				} while (peer < Session.Players.Count());
			}
		}

		DebugString("Starting map preview upload\n");
		memset(&packet, 0, sizeof(packet));
		packet.Command = NET_PREVIEW_MODE;
		strcpy(packet.Name, Session.Handle);
		Ipx.Set_Timing(50, -1, 5000, true); // RA2: the immediate flag is passed
		response_timer = TIMER_SECOND * 20;

		for (int i = 1; i < Session.Players.Count(); i++) {
			Ipx.Send_Global_Message_To_Name(
				&packet, sizeof(packet), 1,
				Session.Players[i]->Name); // RA2: the guest is addressed by name, the packet length is the full 455 bytes
			Call_Back();
		}

		while (Ipx.Global_Num_Send() != 0 && response_timer) {
			Call_Back();
		}
		if (!response_timer) return;

		IPXAddressClass sender_address;
		memset(responses, 0, sizeof(responses));
		int num_responses = 0;
		response_timer = TIMER_SECOND * 30;
		DebugString(
			"Waiting for %d players to signal ready to receive\n",
			Session.Players.Count() - 1);

		while (num_responses < Session.Players.Count() - 1
			&& response_timer) {
			Call_Back();
			length = sizeof(response);
			if (Ipx.Get_Global_Message(
					&response, &length,
					&sender_address, &product_id)) {
				if (response.Command == NET_PREVIEW_ACK) {
					for (int j = 1;
						j < Session.Players.Count(); j++) {
						if (sender_address.Is_Same_Node(
								Session.Players[j]->Address)) { // RA2: the node addresses are compared, not the whole address
							if (!responses[j]) {
								responses[j] = true;
								num_responses++;
							}
							break;
						}
					}
				}
			}
		}

		if (num_responses == Session.Players.Count() - 1) {
			DebugString("Creating low-res preview image\n");
			size = 0;
			unsigned * image =
				preview->Create_Paletted_Preview(64, size);
			DebugString("Preview size is %d bytes\n", size);

			unsigned * buffer = new unsigned[size];
			BufferPipe bpipe(buffer, size * sizeof(unsigned));
			LZOPipe lzopipe(LZOPipe::COMPRESS, 0x2000); // RA2: the block size is passed
			lzopipe.Put_To(&bpipe);
			int comp_size = lzopipe.Put(image, size);
			comp_size += lzopipe.End();
			delete[] image; // RA2: the paletted preview is released

			DebugString(
				"Compressed preview image is %d bytes\n",
				comp_size);
			RawFileClass file("Preview.bin");
			if (file.Is_Available()) {
				file.Delete();
			}
			file.Open(RawFileClass::WRITE);
			file.Write(&size, sizeof(size));
			file.Write(buffer, comp_size);
			file.Close();
			delete[] buffer;

			DebugString(
				"Calling Send_Remote_File to send the preview\n");
			Send_Remote_File(
				"Preview.bin", GAME_IPX, true, false);
			Ipx.Set_Timing(
				TIMER_SECOND / 2, -1, 10 * TIMER_SECOND, true); // RA2: as above
		}
	}
}
#endif


/// <summary>
/// Fetches a digest string for the current random map seed.
/// A generated map has no file to checksum, so this routine builds the identity string that
/// stands in for one. It is what lets the multiplayer scenario checks tell one generated map
/// from another. The map description is deliberately left out -- renaming a map does not
/// make it a different map.
/// </summary>
/// <returns>Returns with the digest string for the map.</returns>
/// <remarks>The digest is allocated by this routine. The caller must delete it.</remarks>
char * CalcRandomMapDigest(void)
{
	unsigned int checksum = 0;
	unsigned int const * data = (unsigned int const *)&RandomMapGen.SeedData.Biome; // RA2: the digest covers 0x140 bytes from Biome on
	int size = 320; // RA2: as above
	wchar_t description[sizeof(RandomMapGen.SeedData.MapDescription) / sizeof(wchar_t)]; // RA2: the description is wide
	memcpy(description, RandomMapGen.SeedData.MapDescription, sizeof(description));
	memset(RandomMapGen.SeedData.MapDescription, '\0', sizeof(description));

	do { // RA2: the loop tests at the end
		unsigned int hibit = checksum >> 31;
		if (size >= 4) {
			checksum = (checksum << 1) + *data++ + hibit; // RA2: as above
			size -= sizeof(unsigned int);
		} else {
			unsigned int val = 0;
			unsigned char const * bytes = (unsigned char const *)data;
			while (size != 0) {
				val = (val << 8) | *bytes++;
				size--;
			}
			checksum = (checksum << 1) + val + hibit; // RA2: as above
		}
	} while (size > 0); // RA2: as above

	char * digest = new char[12];
	sprintf(digest, "%08X", checksum);
	memcpy(RandomMapGen.SeedData.MapDescription, description, sizeof(description));
	return(digest);
}


/// <summary>
/// Creates a new random map for a multiplayer game.
/// This routine prompts the host with the random map generator, saves the resulting seed out
/// under the random map file name, and makes sure the map has an entry in the scenario list
/// so that it can be picked like any other map.
/// </summary>
/// <returns>Returns with the scenario list index of the random map. Zero is returned if the
/// player backed out of the generator.</returns>
int CreateRandomMap(void)
{
	bool (*callback)(void); // RA2: the dialog is given the callback of the session type
	if (Session.Type == GAME_IPX) {
		callback = Net2Callback; // RA2: as above
	} else if (Session.Type == GAME_INTERNET) {
		callback = WOL_Wait_Callback; // RA2: as above
	} else if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
		callback = ModemHostCallback;
	} else {
		callback = MapGen_Call_Back;
	}
	int result = Do_Random_Map_Dialog(callback); // RA2: as above

	if (result != IDOK) {
		return(-1); // RA2: -1 means no map, the callers test for it
	}

	IsRandomMap = true;
	RandomMapGen.SeedData.Save(RANDOM_MAP_FILE_NAME);

	delete MultiplayerMapPreview;
	MultiplayerMapPreview = new MapPreviewClass;
	MultiplayerMapPreview->Read_PCX_Preview("RandMap.img");

	int index;
	bool found = false;
	for (index = 0; index < Session.Scenarios.Count(); index++) {
		if (Session.Scenarios[index]->Is_Random_Map()) { // RA2: the random map is told by its flag, not by its file name
			found = true;
			break;
		}
	}

	if (!found) {
		char * digest = CalcRandomMapDigest();
		MultiMission * scenario = new MultiMission(RANDOM_MAP_FILE_NAME, RandomMapGen.SeedData.MapDescription, digest, true, NULL, 2, 4); // RA2: the constructor takes the official flag, game modes and the player range
		Session.Scenarios.Add(scenario);
		delete digest;
	} else {
		Session.Scenarios[index]->Set_Description(RandomMapGen.SeedData.MapDescription);
	}

	return(index);
}


// RA2: the lobby's slot logic starts here (0x005C8660): the fixed entries, the slot table and its lists, in address order

wchar_t const * LobbyEntryTexts[7]; // RA2: 0x00A72D0C, the names of the fixed lobby entries, loaded by Init_Lobby_Entries
NodeNameType * LobbyEntries[7]; // RA2: 0x00A72D2C, as above
int LobbyObserverSlot = -1; // RA2: 0x007E6D90, the slot of the observer, -1 at first
char SlotColorsOld[36]; // RA2: 0x00A726B0, the color list Refresh_Slot_Settings saw last (cleared by Clear_Slot_Lists)
char SlotColors[36]; // RA2: 0x00A726D4, the color list of the slots now
char SlotCountries[60]; // RA2: 0x00A726F8, the country list of the slots now
wchar_t SlotNames[216]; // RA2: 0x00A72734, the slot names of the slots now
wchar_t SlotNamesOld[176]; // RA2: 0x00A728E4, the slot names Refresh_Slot_Settings saw last
char SlotCountriesOld[60]; // RA2: 0x00A72A44, the country list Refresh_Slot_Settings saw last

/// <summary>
/// Tells whether a name is the host's name.
/// </summary>
bool __fastcall Is_Host_Name(wchar_t const * name) // RA2: 0x005C8660, not in TibSun; the host is the game's name (Session.GameName)
{
	return(wcscmp(name, Session.GameName) == 0); // RA2: as above
}

namespace UI {
bool __fastcall Is_Host_Player(int player) // RA2: 0x005C8680, not in TibSun; the player of the list is the host
{
	return(wcscmp(Session.Players[player]->Name, Session.GameName) == 0); // RA2: as above
}
}

bool __fastcall Is_Host(void) // RA2: 0x005C86A0, not in TibSun; the local handle is the game's name
{
	wchar_t * handle = Narrow_To_Wide(Session.Handle); // RA2: the handle is widened before the game's name is pushed
	return(wcscmp(handle, Session.GameName) == 0); // RA2: as above
}

bool __fastcall Is_Local_Name(wchar_t const * name) // RA2: 0x005C86C0, not in TibSun; the name is the local handle
{
	return(wcscmp(name, Narrow_To_Wide(Session.Handle)) == 0); // RA2: as above
}

void __fastcall Init_Lobby_Entries(void) // RA2: 0x005C86E0, not in TibSun; the fixed entries of the slot lists with their names
{
	if (LobbyEntryTexts[0] == NULL) { // RA2: as above
		LobbyEntryTexts[0] = StringTable::LoadString("GUI:Open", NULL, "D:\\Ra2\\netshare.cpp", 3736); // RA2: as above
		LobbyEntryTexts[1] = StringTable::LoadString("GUI:Closed", NULL, "D:\\Ra2\\netshare.cpp", 3737); // RA2: as above
		LobbyEntryTexts[2] = StringTable::LoadString("GUI:AIEasy", NULL, "D:\\Ra2\\netshare.cpp", 3738); // RA2: as above
		LobbyEntryTexts[3] = StringTable::LoadString("GUI:AINormal", NULL, "D:\\Ra2\\netshare.cpp", 3739); // RA2: as above
		LobbyEntryTexts[4] = StringTable::LoadString("GUI:AIHard", NULL, "D:\\Ra2\\netshare.cpp", 3740); // RA2: as above
		LobbyEntryTexts[5] = StringTable::LoadString("GUI:Waiting", NULL, "D:\\Ra2\\netshare.cpp", 3741); // RA2: as above
		LobbyEntryTexts[6] = StringTable::LoadString("GUI:OpenObserver", NULL, "D:\\Ra2\\netshare.cpp", 3742); // RA2: as above
	}
	if (LobbyEntries[0] == NULL) { // RA2: as above
		LobbyEntries[0] = new NodeNameType; // RA2: as above
		wcscpy(LobbyEntries[0]->Name, LobbyEntryTexts[0]); // RA2: as above
	}
	if (LobbyEntries[1] == NULL) { // RA2: as above
		LobbyEntries[1] = new NodeNameType; // RA2: as above
		wcscpy(LobbyEntries[1]->Name, LobbyEntryTexts[1]); // RA2: as above
	}
	if (LobbyEntries[2] == NULL) { // RA2: as above
		LobbyEntries[2] = new NodeNameType; // RA2: as above
		wcscpy(LobbyEntries[2]->Name, LobbyEntryTexts[2]); // RA2: as above
	}
	if (LobbyEntries[3] == NULL) { // RA2: as above
		LobbyEntries[3] = new NodeNameType; // RA2: as above
		wcscpy(LobbyEntries[3]->Name, LobbyEntryTexts[3]); // RA2: as above
	}
	if (LobbyEntries[4] == NULL) { // RA2: as above
		LobbyEntries[4] = new NodeNameType; // RA2: as above
		wcscpy(LobbyEntries[4]->Name, LobbyEntryTexts[4]); // RA2: as above
	}
	if (LobbyEntries[5] == NULL) { // RA2: as above
		LobbyEntries[5] = new NodeNameType; // RA2: as above
		wcscpy(LobbyEntries[5]->Name, LobbyEntryTexts[5]); // RA2: as above
	}
	if (LobbyEntries[6] == NULL) { // RA2: as above
		LobbyEntries[6] = new NodeNameType; // RA2: as above
		wcscpy(LobbyEntries[6]->Name, LobbyEntryTexts[6]); // RA2: as above
	}
}

void __fastcall Free_Lobby_Entries(void) // RA2: 0x005C8950, not in TibSun; the fixed entries are freed and the player list emptied
{
	if (LobbyEntries[0] != NULL) { // RA2: as above
		delete LobbyEntries[0]; // RA2: as above
		LobbyEntries[0] = NULL; // RA2: as above
	}
	if (LobbyEntries[1] != NULL) { // RA2: as above
		delete LobbyEntries[1]; // RA2: as above
		LobbyEntries[1] = NULL; // RA2: as above
	}
	if (LobbyEntries[2] != NULL) { // RA2: as above
		delete LobbyEntries[2]; // RA2: as above
		LobbyEntries[2] = NULL; // RA2: as above
	}
	if (LobbyEntries[3] != NULL) { // RA2: as above
		delete LobbyEntries[3]; // RA2: as above
		LobbyEntries[3] = NULL; // RA2: as above
	}
	if (LobbyEntries[4] != NULL) { // RA2: as above
		delete LobbyEntries[4]; // RA2: as above
		LobbyEntries[4] = NULL; // RA2: as above
	}
	if (LobbyEntries[5] != NULL) { // RA2: as above
		delete LobbyEntries[5]; // RA2: as above
		LobbyEntries[5] = NULL; // RA2: as above
	}
	if (LobbyEntries[6] != NULL) { // RA2: as above
		delete LobbyEntries[6]; // RA2: as above
		LobbyEntries[6] = NULL; // RA2: as above
	}
	Clear_Vector(&Session.Players); // RA2: as above
}

void __fastcall Fill_Slots(void) // RA2: 0x005C8A10, not in TibSun; the slots up to the limit are open, the others closed
{
	int limit = Get_Lobby_Slot_Limit(); // RA2: as above
	for (int i = 0; i < limit; i++) { // RA2: as above
		Session.Slots[i] = LobbyEntries[0]; // RA2: as above
	}
	for (int j = limit; j < 8; j++) { // RA2: as above
		Session.Slots[j] = LobbyEntries[1]; // RA2: as above
	}
}

void __fastcall Clear_Slot_Lists(void) // RA2: 0x005C8A70, not in TibSun; the lists Refresh_Slot_Settings compares are emptied
{
	SlotColorsOld[0] = '\0'; // RA2: as above
	SlotColors[0] = '\0'; // RA2: as above
	SlotCountriesOld[0] = '\0'; // RA2: as above
	SlotCountries[0] = '\0'; // RA2: as above
}

void __fastcall Reset_Slots(void) // RA2: 0x005C8A90, not in TibSun; the slots are open or closed again, nobody is seated, the computer players are cleared
{
	int limit = Get_Lobby_Slot_Limit(); // RA2: as above
	for (int i = 0; i < limit; i++) { // RA2: as above
		Session.Slots[i] = LobbyEntries[0]; // RA2: as above
	}
	for (int j = limit; j < 8; j++) { // RA2: as above
		Session.Slots[j] = LobbyEntries[1]; // RA2: as above
	}
	Session.LocalSlot = -1; // RA2: as above
	Session.Options.Reset_AI(); // RA2: as above
	SlotNamesOld[0] = L'\0'; // RA2: as above
	SlotNames[0] = L'\0'; // RA2: as above
}

NodeNameType * __fastcall Find_Lobby_Name(wchar_t const * name) // RA2: 0x005C8B20, not in TibSun; the fixed entry of this name, or the player of the list
{
	if (wcscmp(LobbyEntries[0]->Name, name) == 0) { // RA2: as above
		return(LobbyEntries[0]); // RA2: as above
	}
	if (wcscmp(LobbyEntries[1]->Name, name) == 0) { // RA2: as above
		return(LobbyEntries[1]); // RA2: as above
	}
	if (wcscmp(LobbyEntries[2]->Name, name) == 0) { // RA2: as above
		return(LobbyEntries[2]); // RA2: as above
	}
	if (wcscmp(LobbyEntries[3]->Name, name) == 0) { // RA2: as above
		return(LobbyEntries[3]); // RA2: as above
	}
	if (wcscmp(LobbyEntries[4]->Name, name) == 0) { // RA2: as above
		return(LobbyEntries[4]); // RA2: as above
	}
	if (wcscmp(LobbyEntries[5]->Name, name) == 0) { // RA2: as above
		return(LobbyEntries[5]); // RA2: as above
	}
	if (wcscmp(LobbyEntries[6]->Name, name) == 0) { // RA2: as above
		return(LobbyEntries[6]); // RA2: as above
	}
	int count = Session.Players.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		NodeNameType * node = Session.Players[i]; // RA2: as above
		if (wcscmp(node->Name, name) == 0) { // RA2: as above
			return(node); // RA2: as above
		}
	}
	return(NULL); // RA2: as above
}

/// <summary>
/// Finds the lobby slot that holds a player.
/// </summary>
/// <param name="name">The player's name.</param>
/// <returns>The slot, or -1 if no slot holds the name.</returns>
int __fastcall Find_Slot_By_Name(wchar_t const * name) // RA2: 0x005C8C30, the slots are compared by name; the executable has no Session member for the search, and the callers before it expand it inline
{
	for (int i = 0; i < 8; i++) { // RA2: as above
		if (Session.Slots[i] != NULL && wcsicmp(name, Session.Slots[i]->Name) == 0) { // RA2: as above
			return(i); // RA2: as above
		}
	}
	return(-1); // RA2: as above
}

int Find_First_Player_Slot(void) // RA2: 0x005C8C70, the slot of the first player in the list, or -1 without players
{
	if (Session.Players.Count() == 0) { // RA2: as above
		return(-1); // RA2: as above
	}
	return(Find_Slot_By_Name(Session.Players[0]->Name)); // RA2: as above
}

void __fastcall Set_Player_Slot(int slot, NodeNameType * node) // RA2: 0x005C8CC0, not in TibSun; seats an entry in a slot
{
	Session.Slots[slot] = node; // RA2: as above
}

void __fastcall Seat_First_Player(int slot) // RA2: 0x005C8CD0, not in TibSun; the first player of the list takes the slot and the local slot is told
{
	Session.LocalSlot = slot; // RA2: as above
	Session.Slots[slot] = Session.Players[0]; // RA2: as above
}

void __fastcall Get_Slot_Names(wchar_t * list) // RA2: 0x005C8CF0, not in TibSun; the names of the eight slots, each followed by a comma
{
	*list = L'\0'; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		NodeNameType * entry = Session.Slots[i]; // RA2: as above
		WideWstring name; // RA2: as above
		if (entry == NULL || entry == LobbyEntries[0] || entry == LobbyEntries[1] // RA2: the test of Is_Real_Player_Entry is written out, the executable does not call it here
			|| entry == LobbyEntries[5] || entry == LobbyEntries[6] // RA2: as above
			|| entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]) { // RA2: as above
			Wstring text = Lobby_Entry_Name(entry); // RA2: as above
			name = Wstring_ToWide(text); // RA2: as above
		} else {
			name = entry->Name; // RA2: as above
		}
		wcscat(list, name.get()); // RA2: as above
		wcscat(list, L","); // RA2: as above
	}
}

inline HWND Find_Lobby_Dialog(void) // RA2: the lobby dialog of the session type: the dialog is kept in a variable between the four searches (GameoptWindow returns without it)
{
	HWND dialog = WS_Find_Dialog(IDD_WOL_GAMEOPT); // RA2: as above
	if (dialog == NULL) { // RA2: as above
		dialog = WS_Find_Dialog(IDD_WOL_GUEST); // RA2: as above
	}
	if (dialog == NULL) { // RA2: as above
		dialog = WS_Find_Dialog(IDD_MPLAYER_HOST); // RA2: as above
	}
	if (dialog == NULL) { // RA2: as above
		dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST); // RA2: as above
	}
	return(dialog); // RA2: as above
}

#ifdef RA2_NON_MATCHING // not exact: 4 bytes differ (adopted text); exact with the twin's order of the four locals (open ahead of closed) once the seven lobby entries are separate globals, the twin's form (lane F59, held: OnPrivateGameOptions then turns by one byte)
void __fastcall Set_Slot_Names(wchar_t const * list) // RA2: 0x005C8DD0, not in TibSun; takes the slot names the host sent, seats the entries and sets the observer's combo boxes
{
	if (*list == L'\0') { // RA2: as above
		return;
	}
	wchar_t buffer[170]; // RA2: as above
	wcscpy(buffer, list); // RA2: as above
	bool observer[8]; // RA2: the slots that held an observer before
	NodeNameType * observer_entry = LobbyEntries[6]; // RA2: the entries are read once into locals
	NodeNameType * waiting = LobbyEntries[5]; // RA2: as above
	NodeNameType * closed = LobbyEntries[1]; // RA2: as above
	NodeNameType * open = LobbyEntries[0]; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		NodeNameType * entry = Session.Slots[i]; // RA2: as above
		observer[i] = entry == observer_entry // RA2: as above
			|| (entry != NULL && entry != open && entry != closed && entry != waiting && entry->Slot.SpectatorFlag == -1); // RA2: as above
	}
	Fill_Slots(); // RA2: as above
	int slot = 0; // RA2: as above
	for (wchar_t * name = wcstok(buffer, L","); name != NULL; name = wcstok(NULL, L",")) { // RA2: as above
		NodeNameType * entry; // RA2: as above
		{
			WideWstring text(name); // RA2: as above
			entry = Find_Lobby_Entry_By_Name(text); // RA2: as above
		}
		if (entry == NULL) { // RA2: as above
			entry = Find_Lobby_Name(name); // RA2: as above
			if (entry == NULL) { // RA2: as above
				entry = LobbyEntries[5]; // RA2: as above
			}
		}
		if (entry == LobbyEntries[6] // RA2: as above
			|| (entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] // RA2: as above
				&& entry != LobbyEntries[5] && entry != LobbyEntries[2] // RA2: as above
				&& entry != LobbyEntries[3] && entry != LobbyEntries[4] // RA2: as above
				&& entry->Slot.SpectatorFlag == -1)) { // RA2: as above
			HWND dialog = Find_Lobby_Dialog(); // RA2: as above
			if (!observer[slot]) { // RA2: as above
				Set_Player_Slot(slot, entry); // RA2: the seat is set through Set_Player_Slot (0x005C8CC0), expanded inline
				Fill_Observer_Combo(dialog, Player_Side_Combo_ID(slot)); // RA2: as above
				SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 1); // RA2: as above
				int id = Player_Color_Combo_ID(slot); // RA2: as above
				Fill_Random_Color_Combo(dialog, id); // RA2: as above
				Select_Color_Combo(dialog, id, 8); // RA2: as above
			}
		} else {
			HWND dialog = Find_Lobby_Dialog(); // RA2: as above
			if (observer[slot]) { // RA2: as above
				int id = Player_Side_Combo_ID(slot); // RA2: as above
				if (Side_Combo_Data(dialog, id, -1) == -3) { // RA2: as above
					Fill_Side_Combo(dialog, id); // RA2: as above
					SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 0); // RA2: as above
					id = Player_Color_Combo_ID(slot); // RA2: as above
					Fill_Color_Combo(dialog, id); // RA2: as above
					Select_Color_Combo(dialog, id, -2); // RA2: as above
				}
			}
		}
		Set_Player_Slot(slot, entry); // RA2: as above
		slot++; // RA2: as above
	}
}
#endif

int __fastcall Assign_Slot(NodeNameType * entry) // RA2: 0x005C9170, not in TibSun; seats a joining entry in an open slot, or an observer in an observer slot, and gives the slot
{
	for (int i = 0; i < 8; i++) { // RA2: as above
		if ((Session.Slots[i] == LobbyEntries[0] && entry->Slot.SpectatorFlag != -1) // RA2: as above
			|| (Session.Slots[i] == LobbyEntries[6] && entry->Slot.SpectatorFlag == -1)) { // RA2: as above
			Session.Slots[i] = entry; // RA2: as above
			return(i); // RA2: as above
		}
	}
	return(-1); // RA2: as above
}

void __fastcall Release_Slot_By_Address(IPXAddressClass & address) // RA2: 0x005C91C0, not in TibSun; the address is passed on to IPXAddressClass::operator== (0x005235D0), a non-const reference; frees the slot of the player at an address (an observer's slot reopens for observers)
{
	int limit = 4; // RA2: the limit of Get_Lobby_Slot_Limit written out, the executable compares the game type with the same constant 4
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		if (Is_Current_Channel_Tournament() && !Is_Current_Channel_Flag_8()) { // RA2: as above
			limit = 2; // RA2: as above
		}
	} else {
		limit = 8; // RA2: as above
	}
	if (LobbyObserverSlot == 0 && limit < 7) { // RA2: as above
		limit++; // RA2: as above
	}
	for (int i = 0; i < 8; i++) { // RA2: as above
		NodeNameType * node = Session.Slots[i]; // RA2: as above
		if (node->Address == address) { // RA2: as above
			if (Session.Slots[i]->Slot.SpectatorFlag == -1) { // RA2: as above
				Session.Slots[i] = LobbyEntries[6]; // RA2: as above
				return;
			}
			NodeNameType * entry = LobbyEntries[0]; // RA2: as above
			if (i >= limit) { // RA2: as above
				entry = LobbyEntries[1]; // RA2: as above
			}
			Session.Slots[i] = entry; // RA2: as above
			return;
		}
	}
}

void __fastcall Release_Slot_By_Name(char const * name) // RA2: 0x005C9270, not in TibSun; frees the slot of the player of this name, as above
{
	int limit = Get_Lobby_Slot_Limit(); // RA2: as above
	if (LobbyObserverSlot == 0 && limit < 7) { // RA2: as above
		limit++; // RA2: as above
	}
	for (int i = 0; i < 8; i++) { // RA2: as above
		NodeNameType * node = Session.Slots[i]; // RA2: as above
		if (node != NULL && _stricmp(name, Wide_To_Narrow(node->Name)) == 0) { // RA2: as above
			if (Session.Slots[i]->Slot.SpectatorFlag == -1) { // RA2: as above
				Session.Slots[i] = LobbyEntries[6]; // RA2: as above
				return;
			}
			NodeNameType * entry = LobbyEntries[0]; // RA2: as above
			if (i >= limit) { // RA2: as above
				entry = LobbyEntries[1]; // RA2: as above
			}
			Session.Slots[i] = entry; // RA2: as above
			return;
		}
	}
}

bool __fastcall Is_Real_Player_Entry(NodeNameType * entry) // RA2: 0x005C9320, not in TibSun; the entry is a player: not open, closed, waiting, observer or a computer player; the value is a plain expression, the executable returns it in eax
{
	return(entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[5] && entry != LobbyEntries[6] && entry != LobbyEntries[2] && entry != LobbyEntries[3] && entry != LobbyEntries[4]); // RA2: as above
}

bool __fastcall Is_AI_Entry(NodeNameType * entry) // RA2: 0x005C9370, not in TibSun; the entry is a computer player, as above
{
	return(entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])); // RA2: as above
}

bool __fastcall Is_Seated_Entry(NodeNameType * entry) // RA2: 0x005C93A0, not in TibSun; the entry is a player or a computer player: not open, closed, waiting or observer, as above
{
	return(entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[6] && entry != LobbyEntries[5]); // RA2: as above
}

int __fastcall Get_AI_Difficulty(NodeNameType * entry) // RA2: 0x005C93D0, not in TibSun; 2 for the easy, 1 for the medium and 0 for the hard computer player, -1 for any other entry
{
	int difficulty = -1; // RA2: as above
	if (entry == LobbyEntries[2]) { // RA2: as above
		return(2); // RA2: as above
	}
	if (entry == LobbyEntries[3]) { // RA2: as above
		return(1); // RA2: as above
	}
	if (entry == LobbyEntries[4]) { // RA2: as above
		difficulty = 0; // RA2: as above
	}
	return(difficulty); // RA2: as above
}

int Count_AI_Slots(void) // RA2: 0x005C9400, not in TibSun; the slots holding an easy, medium or hard computer player
{
	NodeNameType * easy = LobbyEntries[2]; // RA2: the entries are read once into locals
	NodeNameType * medium = LobbyEntries[3]; // RA2: as above
	NodeNameType * hard = LobbyEntries[4]; // RA2: as above
	int count = 0; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		NodeNameType * entry = Session.Slots[i]; // RA2: as above
		if (entry != NULL && (entry == easy || entry == medium || entry == hard)) { // RA2: as above
			count++; // RA2: as above
		}
	}
	return(count); // RA2: as above
}

bool __fastcall Is_All_Slots_Filled(void) // RA2: 0x005C9440, not in TibSun; no slot up to the player limit is open (the observer's slot counts when it is the first)
{
	int count = Session.MaxPlayers < Session.ScenarioMaxPlayers ? Session.MaxPlayers : Session.ScenarioMaxPlayers; // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		if (Session.Slots[i] == LobbyEntries[0]) { // RA2: as above
			return(false); // RA2: as above
		}
	}
	if (LobbyObserverSlot == 0 && count < 8 && Session.Slots[count] == LobbyEntries[0]) { // RA2: as above
		return(false); // RA2: as above
	}
	return(true); // RA2: as above
}

bool __fastcall Is_Observer_Slot_Full(void) // RA2: 0x005C9490, not in TibSun; no slot is left for an observer
{
	for (int i = 0; i < 8; i++) { // RA2: as above
		if (Session.Slots[i] == LobbyEntries[6]) { // RA2: as above
			return(false); // RA2: as above
		}
	}
	return(true); // RA2: as above
}

bool __fastcall Is_Observer_Slot_Occupied(void) // RA2: 0x005C94B0, not in TibSun; the observer slot holds a seated entry whose spectator flag is set (ra2-decomp's YR twin text, 0x5E9580)
{
	int index = LobbyObserverSlot;
	NodeNameType * open = LobbyEntries[0];
	NodeNameType * closed = LobbyEntries[1];

	for (int i = 0; i < 8; i++) { // RA2: as above
		NodeNameType * slot = Session.Slots[i];
		if (slot && slot != open && slot != closed && slot != LobbyEntries[6] && slot != LobbyEntries[5] && i == index && slot->Slot.SpectatorFlag == -1) { // RA2: as above
			return(true); // RA2: as above
		}
	}
	return(false); // RA2: as above
}

bool Is_Channel_Full(void) // RA2: 0x005C9510, not in TibSun; more players than the channel takes, the observer not counted
{
	int limit = Session.MaxPlayers < Session.ScenarioMaxPlayers ? Session.MaxPlayers : Session.ScenarioMaxPlayers; // RA2: as above
	if (Session.Players.Count() - Is_Observer_Slot_Occupied() > limit) { // RA2: as above
		DebugString("Channel full - Session.ScenarioMaxPlayers = %d\n", Session.ScenarioMaxPlayers); // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

bool __fastcall Is_Color_Taken(int color) // RA2: 0x005C95D0, not in TibSun; a color is taken by a seated player or by a computer player's choice
{
	if (color != -2) { // RA2: as above
		int i; // RA2: as above
		for (i = 0; i < 8; i++) { // RA2: as above
			NodeNameType * entry = Session.Slots[i]; // RA2: as above
			if (Is_Real_Player_Entry(entry) && entry->Get_Color() == color) { // RA2: a player, not a computer player (the test of Is_Real_Player_Entry expanded)
				return(true); // RA2: as above
			}
		}
		for (i = 0; i < 8; i++) { // RA2: as above
			if (Session.Options.AISlots.Colors[i] == color) { // RA2: as above
				return(true); // RA2: as above
			}
		}
	}
	return(false); // RA2: as above
}

int __fastcall Get_Entry_Kind(NodeNameType * entry) // RA2: 0x005C9650, not in TibSun; the type of a fixed entry: 2 open, 3 closed, 4 easy, 5 medium, 6 hard computer, 7 observer (other entries count as open)
{
	if (entry == LobbyEntries[0]) { // RA2: as above
		return(2); // RA2: as above
	}
	if (entry == LobbyEntries[1]) { // RA2: as above
		return(3); // RA2: as above
	}
	if (entry == LobbyEntries[2]) { // RA2: as above
		return(4); // RA2: as above
	}
	if (entry == LobbyEntries[3]) { // RA2: as above
		return(5); // RA2: as above
	}
	if (entry == LobbyEntries[4]) { // RA2: as above
		return(6); // RA2: as above
	}
	return(entry == LobbyEntries[6] ? 7 : 2); // RA2: as above
}

NodeNameType * __fastcall Get_Kind_Entry(int kind) // RA2: 0x005C96B0, not in TibSun; the fixed entry of a type (see Get_Entry_Kind), NULL for any other
{
	if (kind == 2) { // RA2: as above
		return(LobbyEntries[0]); // RA2: as above
	}
	if (kind == 3) { // RA2: as above
		return(LobbyEntries[1]); // RA2: as above
	}
	if (kind == 4) { // RA2: as above
		return(LobbyEntries[2]); // RA2: as above
	}
	if (kind == 5) { // RA2: as above
		return(LobbyEntries[3]); // RA2: as above
	}
	if (kind == 6) { // RA2: as above
		return(LobbyEntries[4]); // RA2: as above
	}
	return(kind == 7 ? LobbyEntries[6] : NULL); // RA2: as above
}

void __fastcall Enable_Start_Button(void) // RA2: 0x005C9700, not in TibSun; the start button is enabled and every player but the host loses the accept status
{
	if (WS_Top_Window_ID() != IDD_MPLAYER_HOST) { // RA2: as above
		HWND dialog = GameoptWindow(); // RA2: as above
		EnableWindow(GetDlgItem(dialog, IDC_ACCEPT), TRUE); // RA2: as above
	}
	int count = Session.Players.Count(); // RA2: as above
	for (int i = 0; i < count; i++) { // RA2: as above
		if (wcscmp(Session.Players[i]->Name, Session.GameName) != 0 && Session.Players[i]->Player.Status != 0) { // RA2: as above
			Session.Players[i]->Player.Status = 0; // RA2: as above
		}
	}
}

int __fastcall Get_Slot_Control_Id(int slot) // RA2: 0x005C97A0, the id of the control of a slot (1041 to 1048, not in TibSun's resource list), or -1; the callers before it expand it inline
{
	if (slot == 0) { // RA2: as above
		return(1041); // RA2: as above
	}
	if (slot == 1) { // RA2: as above
		return(1042); // RA2: as above
	}
	if (slot == 2) { // RA2: as above
		return(1043); // RA2: as above
	}
	if (slot == 3) { // RA2: as above
		return(1044); // RA2: as above
	}
	if (slot == 4) { // RA2: as above
		return(1045); // RA2: as above
	}
	if (slot == 5) { // RA2: as above
		return(1046); // RA2: as above
	}
	if (slot == 6) { // RA2: as above
		return(1047); // RA2: as above
	}
	if (slot == 7) { // RA2: as above
		return(1048); // RA2: as above
	}
	return(-1); // RA2: as above
}

int __fastcall Control_Slot(int id) // RA2: 0x005C9800, not in TibSun; the slot a control belongs to, or -1
{
	if (id == 1041) { // RA2: as above
		return(0); // RA2: as above
	}
	if (id == 1042) { // RA2: as above
		return(1); // RA2: as above
	}
	if (id == 1043) { // RA2: as above
		return(2); // RA2: as above
	}
	if (id == 1044) { // RA2: as above
		return(3); // RA2: as above
	}
	if (id == 1045) { // RA2: as above
		return(4); // RA2: as above
	}
	if (id == 1046) { // RA2: as above
		return(5); // RA2: as above
	}
	if (id == 1047) { // RA2: as above
		return(6); // RA2: as above
	}
	return(id == 1048 ? 7 : -1); // RA2: as above
}

wchar_t const * __fastcall Entry_Type_Name(NodeNameType * entry) // RA2: 0x005C9870, not in TibSun; the tool tip text of an entry's type
{
	if (entry == LobbyEntries[0]) { // RA2: as above
		return(StringTable::LoadString("STT:PlayerOpen", NULL, "D:\\Ra2\\netshare.cpp", 4538)); // RA2: as above
	}
	if (entry == LobbyEntries[1]) { // RA2: as above
		return(StringTable::LoadString("STT:PlayerClosed", NULL, "D:\\Ra2\\netshare.cpp", 4540)); // RA2: as above
	}
	if (entry == LobbyEntries[2]) { // RA2: as above
		return(StringTable::LoadString("STT:PlayerDumbAI", NULL, "D:\\Ra2\\netshare.cpp", 4542)); // RA2: as above
	}
	if (entry == LobbyEntries[3]) { // RA2: as above
		return(StringTable::LoadString("STT:PlayerSmartAI", NULL, "D:\\Ra2\\netshare.cpp", 4544)); // RA2: as above
	}
	if (entry == LobbyEntries[4]) { // RA2: as above
		return(StringTable::LoadString("STT:PlayerGeniusAI", NULL, "D:\\Ra2\\netshare.cpp", 4546)); // RA2: as above
	}
	if (entry == LobbyEntries[6]) { // RA2: as above
		return(StringTable::LoadString("STT:PlayerOpenObserver", NULL, "D:\\Ra2\\netshare.cpp", 4548)); // RA2: as above
	}
	return(StringTable::LoadString("STT:PlayerHuman", NULL, "D:\\Ra2\\netshare.cpp", 4550)); // RA2: as above
}

void __fastcall Select_Combo_By_Data(HWND dialog, int id, int data) // RA2: 0x005C9950, not in TibSun; selects the item of a combo box that carries this data, with the box hidden while it is searched
{
	HWND combo = GetDlgItem(dialog, id); // RA2: as above
	ShowWindow(combo, SW_HIDE); // RA2: as above
	int count = SendDlgItemMessageA(dialog, id, CB_GETCOUNT, 0, 0); // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		if (SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0) == data) { // RA2: as above
			SendDlgItemMessageA(dialog, id, CB_SETCURSEL, index, 0); // RA2: as above
			break;
		}
	}
	ShowWindow(combo, SW_SHOW); // RA2: as above
}

int __fastcall Combo_Selected_Data(HWND dialog, int id, int index) // RA2: 0x005C99D0, not in TibSun; the data of an item of a combo box (the selected one for -1), 0 when it has none
{
	if (index == -1) { // RA2: as above
		index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0); // RA2: as above
	}
	int data = SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0); // RA2: as above
	return(data == -1 ? 0 : data); // RA2: as above
}

void __fastcall Set_Slot_Country(HWND dialog, int id) // RA2: 0x005C9A10, not in TibSun; a side combo box was changed: the local player takes the side, the choice of a computer player is kept, other slots forget theirs
{
	int country = Side_Combo_Data(dialog, id, -1); // RA2: as above
	int slot = Side_Combo_Player(id); // RA2: as above
	if (slot == Session.LocalSlot) { // RA2: as above
		Session.Set_Side(country, false); // RA2: as above
		if (Session.Players.Count() != 0) { // RA2: as above
			Session.Players[0]->Set_Country(country); // RA2: as above
		}
	} else {
		NodeNameType * entry = Session.Slots[slot]; // RA2: as above
		if (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])) { // RA2: as above
			Session.Options.AISlots.Countries[slot] = country; // RA2: as above
		} else {
			Session.Options.AISlots.Countries[slot] = -1; // RA2: as above
			Session.Options.AISlots.Difficulties[slot] = -1; // RA2: as above
			Session.Options.AISlots.Colors[slot] = -1; // RA2: as above
		}
	}
}

void __fastcall Set_Slot_Color(HWND dialog, int id) // RA2: 0x005C9AB0, not in TibSun; a color combo box was changed: the local player takes the color unless it is taken, other slots keep theirs
{
	int color = Color_Combo_Data(dialog, id, -1); // RA2: as above
	int slot = Color_Combo_Player(id); // RA2: as above
	if (slot == Session.LocalSlot) { // RA2: as above
		Color_Combo_Changed(dialog, id); // RA2: as above
		if (Is_Color_Taken(color)) { // RA2: as above
			color = -2; // RA2: as above
		}
		Session.Set_Color(color, false); // RA2: as above
		if (Session.Players.Count() != 0) { // RA2: as above
			Session.Players[0]->Set_Color(color); // RA2: as above
		}
	} else {
		Session.Options.AISlots.Colors[slot] = color; // RA2: as above
	}
}

void __fastcall Update_Slot_Controls(HWND dialog, int slot, bool redraw) // RA2: 0x005C9B90, not in TibSun; shows, enables and fills the side and color boxes of a slot, the observer's slot keeps them closed
{
	int side_id = Player_Side_Combo_ID(slot); // RA2: as above
	int color_id = Player_Color_Combo_ID(slot); // RA2: as above
	if (LobbyObserverSlot >= 0 && slot == LobbyObserverSlot // RA2: as above
		&& (Session.Slots[slot] == LobbyEntries[6] || slot == 0 || Session.Slots[slot]->Slot.SpectatorFlag == -1)) { // RA2: as above
		Select_Side_Combo(dialog, side_id, -3); // RA2: as above
		Select_Color_Combo(dialog, color_id, -2); // RA2: as above
		HWND side = GetDlgItem(dialog, side_id); // RA2: as above
		EnableWindow(side, FALSE); // RA2: as above
		HWND control = GetDlgItem(dialog, color_id); // RA2: as above
		EnableWindow(control, FALSE); // RA2: as above
		ShowWindow(control, SW_HIDE); // RA2: as above
		return;
	}
	if (!redraw) { // RA2: as above
		Select_Side_Combo(dialog, side_id, -2); // RA2: as above
		Select_Color_Combo(dialog, color_id, -2); // RA2: as above
	}
	HWND side = GetDlgItem(dialog, side_id); // RA2: as above
	EnableWindow(side, redraw); // RA2: as above
	HWND control = GetDlgItem(dialog, color_id); // RA2: as above
	EnableWindow(control, redraw); // RA2: as above
	int limit = Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex); // RA2: as above
	if (LobbyObserverSlot == 0 && limit < 7) { // RA2: as above
		limit++; // RA2: as above
	}
	if (slot < limit || slot == LobbyObserverSlot) { // RA2: as above
		ShowWindow(control, SW_SHOW); // RA2: as above
	}
}

void __fastcall Unlock_Slots(HWND dialog, int slot) // RA2: 0x005C9CF0, not in TibSun; every slot's control is enabled again, the slot's boxes are shown as for Lock_Slots
{
	for (int i = 0; i < 8; i++) { // RA2: as above
		HWND control = GetDlgItem(dialog, Get_Slot_Control_Id(i)); // RA2: as above
		EnableWindow(control, TRUE); // RA2: as above
		Update_Slot_Controls(dialog, i, false); // RA2: as above
	}
	Update_Slot_Controls(dialog, slot, true); // RA2: as above
}

void __fastcall Lock_Slots(HWND dialog, int slot) // RA2: 0x005C9DA0, not in TibSun; every slot's control is disabled but the one of this slot
{
	for (int i = 0; i < 8; i++) { // RA2: as above
		HWND control = GetDlgItem(dialog, Get_Slot_Control_Id(i)); // RA2: as above
		EnableWindow(control, FALSE); // RA2: as above
		Update_Slot_Controls(dialog, i, false); // RA2: as above
	}
	HWND control = GetDlgItem(dialog, Get_Slot_Control_Id(slot)); // RA2: as above
	EnableWindow(control, TRUE); // RA2: as above
	Update_Slot_Controls(dialog, slot, true); // RA2: as above
}

char const * LobbyHostImage = "wolhost.pcx"; // RA2: 0x007E6DB8, the host's picture of the slot list (a variable holding the file name, not a literal at the call)
char const * LobbyAcceptImage = "wolacpt.pcx"; // RA2: 0x007E6DBC, the picture of a player who accepted

void __fastcall Refresh_Slot_Images(HWND dialog) // RA2: 0x005C9EC0, not in TibSun; shows the host's or accepted picture at every slot's control
{
	for (int i = 0; i < 8; i++) { // RA2: as above
		HWND control = GetDlgItem(dialog, Slot_Color_Control_Id(i)); // RA2: as above
		Surface * image = NULL; // RA2: as above
		if (Is_Real_Player_Entry(Session.Slots[i]) && Is_Host_Name(Session.Slots[i]->Name)) { // RA2: as above
			image = SurfaceCache.GetSurface(LobbyHostImage, NULL); // RA2: as above
		} else if (Is_AI_Entry(Session.Slots[i]) || Session.Slots[i]->Player.Status == 1) { // RA2: as above
			image = SurfaceCache.GetSurface(LobbyAcceptImage, NULL); // RA2: as above
		}
		OwnerDraw::Set_Control_Image(control, image); // RA2: as above
	}
}

inline void Refresh_Country_Flags(HWND dialog) // RA2: not in TibSun; the flag picture at each player's control; the executable expands it twice in Refresh_Slot_Settings
{
	for (int i = 0; i < 8; i++) { // RA2: as above
		HWND control = GetDlgItem(dialog, Player_Control_ID(i)); // RA2: as above
		Surface * flag = Country_Flag(Slot_Country(i)); // RA2: as above
		OwnerDraw::Set_Control_Image(control, flag); // RA2: as above
	}
}

inline void Refresh_Side_Combos(HWND dialog) // RA2: not in TibSun; selects each slot's side in its side box (expanded twice in Refresh_Slot_Settings)
{
	for (int i = 0; i < 8; i++) { // RA2: as above
		int id = Player_Side_Combo_ID(i); // RA2: as above
		Select_Side_Combo(dialog, id, Slot_Country(i)); // RA2: as above
	}
}

inline void Refresh_Slot_Image(HWND dialog, int slot) // RA2: not in TibSun; the host's or accepted picture at one slot's control
{
	HWND control = GetDlgItem(dialog, Slot_Color_Control_Id(slot)); // RA2: as above
	Surface * image = NULL; // RA2: as above
	if (Is_Real_Player_Entry(Session.Slots[slot]) && Is_Host_Name(Session.Slots[slot]->Name)) { // RA2: as above
		image = SurfaceCache.GetSurface(LobbyHostImage, NULL); // RA2: as above
	} else if (Is_Slot_Accepted(slot) == 1) { // RA2: as above
		image = SurfaceCache.GetSurface(LobbyAcceptImage, NULL); // RA2: as above
	}
	OwnerDraw::Set_Control_Image(control, image); // RA2: as above
}

inline bool Country_List_Changed(void) // RA2: not in TibSun; rebuilds the country list text and tells whether it differs from the last one
{
	strcpy(SlotCountriesOld, SlotCountries); // RA2: as above
	SlotCountries[0] = '\0'; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		char text[4]; // RA2: the executable's frame holds only four bytes for the text (push ecx), enough for "-2,"
		sprintf(text, "%d,", Slot_Country(i)); // RA2: as above
		strcat(SlotCountries, text); // RA2: as above
	}
	return strcmp(SlotCountriesOld, SlotCountries) != 0; // RA2: as above
}

inline bool Color_List_Changed(void) // RA2: not in TibSun; rebuilds the color list text and tells whether it differs from the last one
{
	strcpy(SlotColorsOld, SlotColors); // RA2: as above
	SlotColors[0] = '\0'; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		char text[4]; // RA2: the executable's frame holds only four bytes for the text (push ecx), enough for "-2,"
		sprintf(text, "%d,", Slot_Color(i)); // RA2: as above
		strcat(SlotColors, text); // RA2: as above
	}
	return strcmp(SlotColorsOld, SlotColors) != 0; // RA2: as above
}

inline bool Slot_Names_Changed(void) // RA2: not in TibSun; reads the slot names again and tells whether they differ from the last ones
{
	wcscpy(SlotNamesOld, SlotNames); // RA2: as above
	Get_Slot_Names(SlotNames); // RA2: as above
	return wcscmp(SlotNamesOld, SlotNames) != 0; // RA2: as above
}

void __fastcall Refresh_Slot_Settings(HWND dialog, bool full) // RA2: 0x005CA000, not in TibSun; refreshes the flags, side and color boxes and the slot pictures; when only the slot lists changed the boxes are left alone; written with the inline helpers above (the executable expands them, which keeps the callees Slot_Country, Slot_Color, Slot_Color_Control_Id and Is_Slot_Accepted out of line)
{
	if (full) { // RA2: as above
		Refresh_Slot_Combos(dialog); // RA2: as above
		Refresh_Country_Flags(dialog); // RA2: as above
		Refresh_Side_Combos(dialog); // RA2: as above
		Refresh_Slot_Colors(dialog); // RA2: as above
		if (Session.Type == GAME_INTERNET) { // RA2: as above
			Store_Slot_Pings(); // RA2: as above
		}
		return; // RA2: as above
	}
	for (int slot = 0; slot < 8; slot++) { // RA2: as above
		Refresh_Slot_Image(dialog, slot); // RA2: as above
	}
	if (Slot_Names_Changed()) { // RA2: as above
		Refresh_Slot_Combos(dialog); // RA2: as above
		if (Session.Type == GAME_INTERNET) { // RA2: as above
			Store_Slot_Pings(); // RA2: as above
		}
	}
	Refresh_Country_Flags(dialog); // RA2: as above
	if (Country_List_Changed()) { // RA2: as above
		Refresh_Side_Combos(dialog); // RA2: as above
	}
	if (Color_List_Changed()) { // RA2: as above
		Refresh_Slot_Colors(dialog); // RA2: as above
	}
}


int __fastcall Is_Slot_Accepted(int slot) // RA2: 0x005CA3B0, not in TibSun; the slot holds a computer player or a player who accepted
{
	NodeNameType * entry = Session.Slots[slot]; // RA2: as above
	int accepted = 0; // RA2: as above
	if ((entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])) // RA2: as above
		|| entry->Player.Status == 1) { // RA2: as above
		accepted = 1; // RA2: as above
	}
	return(accepted); // RA2: as above
}

int __fastcall Slot_Country(int slot) // RA2: 0x005CA3F0, not in TibSun; the side of a slot: the game mode's default for a lobby of its own, a computer player's choice, -2 for an open, closed or waiting slot, -3 for the observer's
{
	int country = -2; // RA2: as above
	NodeNameType * entry = Session.Slots[slot]; // RA2: as above
	if (Session.MPGameMode != NULL && (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET)) { // RA2: as above
		country = Session.MPGameMode->DefaultCountry(); // RA2: as above
	}
	if (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])) { // RA2: as above
		return(Session.Options.AISlots.Countries[slot]); // RA2: as above
	}
	if (entry == LobbyEntries[0] || entry == LobbyEntries[1] || entry == LobbyEntries[5]) { // RA2: as above
		return(country); // RA2: as above
	}
	if (entry == LobbyEntries[6]) { // RA2: as above
		return(-3); // RA2: as above
	}
	return(entry->Get_Country()); // RA2: as above
}

int __fastcall Slot_Color(int slot) // RA2: 0x005CA480, not in TibSun; the color of a slot: a computer player's choice, -2 (random) for an open, closed, waiting or observer slot
{
	NodeNameType * entry = Session.Slots[slot]; // RA2: as above
	if (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])) { // RA2: as above
		return(Session.Options.AISlots.Colors[slot]); // RA2: as above
	}
	if (entry == LobbyEntries[0]) { // RA2: as above
		return(-2); // RA2: as above
	}
	if (entry == LobbyEntries[1]) { // RA2: as above
		return(-2); // RA2: as above
	}
	if (entry == LobbyEntries[5]) { // RA2: as above
		return(-2); // RA2: as above
	}
	if (entry == LobbyEntries[6]) { // RA2: as above
		return(-2); // RA2: as above
	}
	return(entry->Get_Color()); // RA2: as above
}

int __fastcall Slot_Color_Control_Id(int slot) // RA2: 0x005CA4E0, not in TibSun; the id of the control that shows a slot's color (1746 to 1753), or -1
{
	if (slot == 0) { // RA2: as above
		return(1746); // RA2: as above
	}
	if (slot == 1) { // RA2: as above
		return(1747); // RA2: as above
	}
	if (slot == 2) { // RA2: as above
		return(1748); // RA2: as above
	}
	if (slot == 3) { // RA2: as above
		return(1749); // RA2: as above
	}
	if (slot == 4) { // RA2: as above
		return(1750); // RA2: as above
	}
	if (slot == 5) { // RA2: as above
		return(1751); // RA2: as above
	}
	if (slot == 6) { // RA2: as above
		return(1752); // RA2: as above
	}
	if (slot == 7) { // RA2: as above
		return(1753); // RA2: as above
	}
	return(-1); // RA2: as above
}

void __fastcall Refresh_Slot_Colors(HWND dialog) // RA2: 0x005CA540, not in TibSun; selects each slot's color in its color box, the first player's slot first
{
	int own = Find_First_Player_Slot(); // RA2: as above
	if (own != -1) { // RA2: as above
		int id = Player_Color_Combo_ID(own); // RA2: as above
		Select_Color_Combo(dialog, id, Slot_Color(own)); // RA2: as above
	}
	for (int i = 0; i < 8; i++) { // RA2: as above
		if (i != own) { // RA2: as above
			int id = Player_Color_Combo_ID(i); // RA2: as above
			Select_Color_Combo(dialog, id, Slot_Color(i)); // RA2: as above
		}
	}
}

#define OD_COMBOADDSTRINGW (WM_USER + 194) // RA2: 0x4C2, adds a wide string to an owner-drawn combo box
#define OD_COMBOSETEDIT (WM_USER + 192) // RA2: 0x4C0, sets the text shown in the combo box itself

inline void Add_Combo_Entry(HWND combo, wchar_t const * text, NodeNameType * const & entry) // RA2: not in TibSun; adds a wide string to a combo box and attaches the lobby entry as its data; the entry is taken by reference because the executable reads it after the text was added (a by-value argument is read first)
{
	int index = SendMessageA(combo, OD_COMBOADDSTRINGW, 0, (LPARAM)text); // RA2: as above
	SendMessageA(combo, CB_SETITEMDATA, index, (LPARAM)entry); // RA2: as above
}

void __fastcall Refresh_Slot_Combos(HWND dialog) // RA2: 0x005CA6C0, not in TibSun; refills the entry list of every slot's combo box: the entries that may be chosen by the host, the current entry shown
{
	bool ai_allowed = (Session.MPGameMode == NULL || Session.MPGameMode->IsSkirmishAllowed()) && !Is_Current_Channel_Tournament(); // RA2: as above
	int limit = Get_Lobby_Slot_Limit(); // RA2: as above
	if (LobbyObserverSlot == 0) { // RA2: as above
		if (ai_allowed || limit < Get_Lobby_Slot_Limit_Plus_One()) { // RA2: as above
			limit++; // RA2: as above
		}
	}
	for (int slot = 0; slot < 8; slot++) { // RA2: as above
		bool beyond_map = false; // RA2: as above
		if (slot == LobbyObserverSlot) { // RA2: as above
			if (slot > Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex)) { // RA2: as above
				NodeNameType * seated = Session.Slots[slot]; // RA2: as above
				if (seated != LobbyEntries[0] && (seated == NULL || (seated != LobbyEntries[2] && seated != LobbyEntries[3] && seated != LobbyEntries[4]))) { // RA2: as above
					beyond_map = true; // RA2: as above
				}
			}
			if (slot == LobbyObserverSlot && Session.MPGameMode != NULL) { // RA2: as above
				Session.MPGameMode->vt_entry_A0(); // RA2: as above
			}
		}
		int id = Get_Slot_Control_Id(slot); // RA2: as above
		HWND combo = GetDlgItem(dialog, id); // RA2: as above
		bool hidden = ShowWindow(combo, SW_HIDE) == 0; // RA2: as above
		SendDlgItemMessageA(dialog, id, CB_RESETCONTENT, 0, 0); // RA2: as above
		NodeNameType * entry = Session.Slots[slot]; // RA2: as above
		wchar_t const * text = entry->Name; // RA2: as above
		if (entry == LobbyEntries[0]) { // RA2: as above
			text = StringTable::LoadString("GUI:Open", NULL, "D:\\Ra2\\netshare.cpp", 4885); // RA2: as above
		} else if (entry == LobbyEntries[1]) { // RA2: as above
			text = StringTable::LoadString("GUI:Closed", NULL, "D:\\Ra2\\netshare.cpp", 4887); // RA2: as above
		} else if (entry == LobbyEntries[2]) { // RA2: as above
			text = StringTable::LoadString("GUI:AIEasy", NULL, "D:\\Ra2\\netshare.cpp", 4889); // RA2: as above
		} else if (entry == LobbyEntries[3]) { // RA2: as above
			text = StringTable::LoadString("GUI:AINormal", NULL, "D:\\Ra2\\netshare.cpp", 4891); // RA2: as above
		} else if (entry == LobbyEntries[4]) { // RA2: as above
			text = StringTable::LoadString("GUI:AIHard", NULL, "D:\\Ra2\\netshare.cpp", 4893); // RA2: as above
		} else if (entry == LobbyEntries[5]) { // RA2: as above
			text = StringTable::LoadString("GUI:Waiting", NULL, "D:\\Ra2\\netshare.cpp", 4895); // RA2: as above
		} else if (entry == LobbyEntries[6]) { // RA2: as above
			text = StringTable::LoadString("GUI:OpenObserver", NULL, "D:\\Ra2\\netshare.cpp", 4897); // RA2: as above
		}
		if (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]) && !ai_allowed) { // RA2: as above
			text = StringTable::LoadString("GUI:Open", NULL, "D:\\Ra2\\netshare.cpp", 4904); // RA2: as above
		}
		WideWstring current(text); // RA2: as above
		if (Is_Host()) { // RA2: as above
			if (Is_Real_Player_Entry(entry)) { // RA2: as above
				Add_Combo_Entry(combo, text, entry); // RA2: as above
			}
			if (!Is_Local_Name(text)) { // RA2: as above
				if (slot < limit && (!beyond_map || slot != LobbyObserverSlot || slot < Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex))) { // RA2: as above
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:Open", NULL, "D:\\Ra2\\netshare.cpp", 4928), LobbyEntries[0]); // RA2: as above
				}
				if (slot == LobbyObserverSlot) { // RA2: as above
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:OpenObserver", NULL, "D:\\Ra2\\netshare.cpp", 4938), LobbyEntries[6]); // RA2: as above
				}
				Add_Combo_Entry(combo, StringTable::LoadString("GUI:Closed", NULL, "D:\\Ra2\\netshare.cpp", 4945), LobbyEntries[1]); // RA2: as above
				if (ai_allowed && !beyond_map) { // RA2: as above
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:AIEasy", NULL, "D:\\Ra2\\netshare.cpp", 4953), LobbyEntries[2]); // RA2: as above
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:AINormal", NULL, "D:\\Ra2\\netshare.cpp", 4959), LobbyEntries[3]); // RA2: as above
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:AIHard", NULL, "D:\\Ra2\\netshare.cpp", 4965), LobbyEntries[4]); // RA2: as above
				}
			}
		} else {
			Add_Combo_Entry(combo, text, entry); // RA2: as above
		}
		SendMessageA(combo, OD_COMBOSETEDIT, -1, (LPARAM)current.text()); // RA2: as above
		if (!hidden) { // RA2: as above
			ShowWindow(combo, SW_SHOW); // RA2: as above
		}
	}
}


bool __fastcall Reset_AI_Slots(HWND dialog) // RA2: 0x005CACB0, not in TibSun; a computer player picked in a slot's combo box is replaced by an open slot, or by a closed one beyond the lobby's limit; tells whether any was
{
	bool changed = false; // RA2: as above
	int limit = Get_Lobby_Slot_Limit(); // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		int id = Get_Slot_Control_Id(i); // RA2: as above
		int selected = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0); // RA2: as above
		NodeNameType * data = (NodeNameType *)SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, selected, 0); // RA2: as above
		if (Is_AI_Entry(data)) { // RA2: as above
			if (i < limit) { // RA2: as above
				Session.Slots[i] = LobbyEntries[0]; // RA2: as above
			} else {
				Session.Slots[i] = LobbyEntries[1]; // RA2: as above
			}
			Update_Slot_Controls(dialog, i, false); // RA2: as above
			Session.Options.AISlots.Difficulties[i] = -1; // RA2: as above
			Session.Options.AISlots.Countries[i] = -1; // RA2: as above
			Session.Options.AISlots.Colors[i] = -1; // RA2: as above
			changed = true; // RA2: as above
		}
	}
	return(changed); // RA2: as above
}

void __fastcall Show_Slot_Controls(HWND dialog, int first, int count) // RA2: 0x005CAE00, not in TibSun; shows the controls of count slots from the first one on
{
	int end = first + count; // RA2: as above
	for (int slot = first; slot < end; ++slot) { // RA2: as above
		int color_id = Slot_Color_Control_Id(slot); // RA2: as above
		int slot_id = Get_Slot_Control_Id(slot); // RA2: as above
		int player_id = Player_Control_ID(slot); // RA2: as above
		int shade_id = Player_Color_Combo_ID(slot); // RA2: as above
		int side_id = Player_Side_Combo_ID(slot); // RA2: as above
		HWND color = GetDlgItem(dialog, color_id); // RA2: as above
		HWND combo = GetDlgItem(dialog, slot_id); // RA2: as above
		HWND player = GetDlgItem(dialog, player_id); // RA2: as above
		HWND shade = GetDlgItem(dialog, shade_id); // RA2: as above
		HWND side = GetDlgItem(dialog, side_id); // RA2: as above
		NodeNameType * entry = Session.Slots[slot]; // RA2: as above
		bool observer; // RA2: as above
		if (entry == LobbyEntries[6] || (entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[5] && entry->Slot.SpectatorFlag == -1)) { // RA2: as above
			observer = true; // RA2: as above
		} else {
			observer = false; // RA2: as above
		}
		ShowWindow(color, SW_SHOW); // RA2: as above
		ShowWindow(combo, SW_SHOW); // RA2: as above
		ShowWindow(player, SW_SHOW); // RA2: as above
		if (observer) { // RA2: as above
			ShowWindow(shade, SW_HIDE); // RA2: as above
		} else {
			ShowWindow(shade, SW_SHOW); // RA2: as above
		}
		ShowWindow(side, SW_SHOW); // RA2: as above
	}
}

void __fastcall Show_Observer_Slot_Controls(HWND dialog) // RA2: 0x005CAFD0, not in TibSun; shows the controls of the observer's slot (the last one when the observer is the first), the color box stays hidden for an observer
{
	int slot = LobbyObserverSlot; // RA2: as above
	if (slot < 0) { // RA2: as above
		return; // RA2: as above
	}
	if (slot == 0) { // RA2: as above
		slot = Session.ScenarioMaxPlayers; // RA2: as above
	}
	if (slot >= 8) { // RA2: as above
		return; // RA2: as above
	}
	int color_id; // RA2: as above
	int slot_id; // RA2: as above
	if (slot == 0) { // RA2: as above
		color_id = 1746; // RA2: as above
		slot_id = 1041; // RA2: as above
	} else if (slot == 1) { // RA2: as above
		color_id = 1747; // RA2: as above
		slot_id = 1042; // RA2: as above
	} else if (slot == 2) { // RA2: as above
		color_id = 1748; // RA2: as above
		slot_id = 1043; // RA2: as above
	} else if (slot == 3) { // RA2: as above
		color_id = 1749; // RA2: as above
		slot_id = 1044; // RA2: as above
	} else if (slot == 4) { // RA2: as above
		color_id = 1750; // RA2: as above
		slot_id = 1045; // RA2: as above
	} else if (slot == 5) { // RA2: as above
		color_id = 1751; // RA2: as above
		slot_id = 1046; // RA2: as above
	} else if (slot == 6) { // RA2: as above
		color_id = 1752; // RA2: as above
		slot_id = 1047; // RA2: as above
	} else if (slot == 7) { // RA2: as above
		color_id = 1753; // RA2: as above
		slot_id = 1048; // RA2: as above
	} else {
		color_id = -1; // RA2: as above
		slot_id = -1; // RA2: as above
	}
	int player_id = Player_Control_ID(slot); // RA2: as above
	int color_combo_id = Player_Color_Combo_ID(slot); // RA2: as above
	int side_combo_id = Player_Side_Combo_ID(slot); // RA2: as above
	NodeNameType * entry = Session.Slots[slot]; // RA2: as above
	bool observer; // RA2: as above
	if (entry == LobbyEntries[6] || (entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[5] && entry->Slot.SpectatorFlag == -1)) { // RA2: as above
		observer = true; // RA2: as above
	} else {
		observer = false; // RA2: as above
	}
	HWND color = GetDlgItem(dialog, color_id); // RA2: as above
	HWND combo = GetDlgItem(dialog, slot_id); // RA2: as above
	HWND player = GetDlgItem(dialog, player_id); // RA2: as above
	HWND color_combo = GetDlgItem(dialog, color_combo_id); // RA2: as above
	HWND side_combo = GetDlgItem(dialog, side_combo_id); // RA2: as above
	ShowWindow(color, SW_SHOW); // RA2: as above
	ShowWindow(combo, SW_SHOW); // RA2: as above
	ShowWindow(player, SW_SHOW); // RA2: as above
	if (!observer) { // RA2: as above
		ShowWindow(color_combo, SW_SHOW); // RA2: as above
		ShowWindow(side_combo, SW_SHOW); // RA2: as above
	} else {
		ShowWindow(color_combo, SW_HIDE); // RA2: as above
		ShowWindow(side_combo, SW_SHOW); // RA2: as above
	}
}

void __fastcall Hide_Slot_Controls(HWND dialog, int first) // RA2: 0x005CB170, not in TibSun; hides the controls of the slots from the first one on, except the observer's
{
	for (int slot = first; slot < 8; ++slot) { // RA2: as above
		if (slot == LobbyObserverSlot) { // RA2: as above
			continue; // RA2: as above
		}
		int color_id = Slot_Color_Control_Id(slot); // RA2: as above
		int slot_id = Get_Slot_Control_Id(slot); // RA2: as above
		int player_id = Player_Control_ID(slot); // RA2: as above
		int side_id = Player_Side_Combo_ID(slot); // RA2: as above
		int shade_id = Player_Color_Combo_ID(slot); // RA2: as above
		HWND color = GetDlgItem(dialog, color_id); // RA2: as above
		HWND entry = GetDlgItem(dialog, slot_id); // RA2: as above
		HWND player = GetDlgItem(dialog, player_id); // RA2: as above
		HWND side = GetDlgItem(dialog, side_id); // RA2: as above
		HWND shade = GetDlgItem(dialog, shade_id); // RA2: as above
		ShowWindow(color, SW_HIDE); // RA2: as above
		ShowWindow(entry, SW_HIDE); // RA2: as above
		ShowWindow(player, SW_HIDE); // RA2: as above
		ShowWindow(side, SW_HIDE); // RA2: as above
		ShowWindow(shade, SW_HIDE); // RA2: as above
	}
}

void __fastcall Change_Slot_Count(HWND dialog, int old_count, int count, MultiplayerGameMode * mode) // RA2: 0x005CB300, not in TibSun; shows or hides the controls of the slots that were added or removed, both counts limited to the lobby's
{
	if (Session.MPGameMode != NULL && !Session.MPGameMode->IsSkirmishAllowed() && !Is_Current_Channel_Tournament()) { // RA2: as above
		int limit = Get_Lobby_Slot_Limit(); // RA2: as above
		if (old_count > limit) { // RA2: as above
			old_count = limit; // RA2: as above
		}
	}
	if ((mode != NULL && !mode->IsSkirmishAllowed()) || Is_Current_Channel_Tournament()) { // RA2: as above
		int limit = Get_Lobby_Slot_Limit(); // RA2: as above
		if (count > limit) { // RA2: as above
			count = limit; // RA2: as above
		}
	}
	int added = count - old_count; // RA2: as above
	if (added > 0) { // RA2: as above
		Show_Slot_Controls(dialog, old_count, added); // RA2: as above
		Show_Observer_Slot_Controls(dialog); // RA2: as above
	} else {
		if (added < 0) { // RA2: as above
			Hide_Slot_Controls(dialog, count); // RA2: as above
		}
		Show_Observer_Slot_Controls(dialog); // RA2: as above
	}
}


void __fastcall Change_Slot_Type(HWND dialog, int id, NodeNameType * node, int mode); // RA2: 0x005CBF20, defined below
Wstring Wstring_Narrow(WideWstring const & text); // RA2: 0x00771280, as wonline.cpp

void __fastcall Change_Slot_Range(HWND dialog, int first, int count) // RA2: 0x005CB3F0, not in TibSun; the entries chosen in the combo boxes of count slots from the first one on are seated, empty ones become open or closed, then the controls are shown
{
	int end = first + count; // RA2: as above
	int limit = Get_Lobby_Slot_Limit(); // RA2: as above
	for (int i = first; i < end; i++) { // RA2: as above
		int id = Get_Slot_Control_Id(i); // RA2: as above
		int index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0); // RA2: as above
		NodeNameType * entry = (NodeNameType *)SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0); // RA2: as above
		NodeNameType * open = LobbyEntries[0]; // RA2: the open and closed entries are read once, before the test: the executable loads both into registers there
		NodeNameType * closed = LobbyEntries[1]; // RA2: as above
		if (entry != (NodeNameType *)-1 && entry != NULL && entry != open && entry != closed && entry != LobbyEntries[6] && entry != LobbyEntries[5]) { // RA2: as above
			Change_Slot_Type(dialog, id, entry, 0); // RA2: as above
		} else if (i != LobbyObserverSlot) { // RA2: as above
			if (i < limit) { // RA2: as above
				Change_Slot_Type(dialog, id, open, 0); // RA2: as above
			} else {
				Change_Slot_Type(dialog, id, closed, 0); // RA2: as above
			}
		}
	}
	Show_Slot_Controls(dialog, first, count); // RA2: as above
	Net2DisplayUsers(); // RA2: as above
	Send_Lobby_Options(true, false); // RA2: as above
}

void __fastcall Clear_Observer_Slots(HWND dialog) // RA2: 0x005CB560, not in TibSun; observers are removed from the slots: the open ones become open or closed, the observer's own slot keeps its entry unless the scenario has no room for it
{
	bool changed = false; // RA2: as above
	if (LobbyObserverSlot < 1) { // RA2: as above
		int limit = Get_Lobby_Slot_Limit(); // RA2: as above
		for (int slot = 1; slot < 8; slot++) { // RA2: as above
			int id = Get_Slot_Control_Id(slot); // RA2: as above
			NodeNameType * entry = Session.Slots[slot]; // RA2: as above
			if (slot < limit) { // RA2: as above
				if (entry == LobbyEntries[6] || entry->Slot.SpectatorFlag == -1) { // RA2: as above
					Change_Slot_Type(dialog, id, LobbyEntries[0], 1); // RA2: as above
					changed = true; // RA2: as above
				}
			} else if (entry == LobbyEntries[6] || entry->Slot.SpectatorFlag == -1) { // RA2: as above
				Change_Slot_Type(dialog, id, LobbyEntries[1], 1); // RA2: as above
				changed = true; // RA2: as above
			}
		}
	} else {
		NodeNameType * entry = Session.Slots[LobbyObserverSlot]; // RA2: as above
		if (LobbyObserverSlot > Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex)) { // RA2: as above
			NodeNameType * open = LobbyEntries[0]; // RA2: read once at the head of the test, as in Change_Slot_Range
			NodeNameType * closed = LobbyEntries[1]; // RA2: as above
			if ((Is_Seated_Entry(entry) && entry->Slot.SpectatorFlag != -1) // RA2: as above
				|| (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])) // RA2: as above
				|| entry == open) { // RA2: as above
				entry = closed; // RA2: as above
				changed = true; // RA2: as above
			}
		}
		Show_Observer_Slot_Controls(dialog); // RA2: as above
		Change_Slot_Type(dialog, Get_Slot_Control_Id(LobbyObserverSlot), entry, 1); // RA2: as above
	}
	if (changed) { // RA2: as above
		Set_Slot_Country(dialog, Player_Side_Combo_ID(LobbyObserverSlot)); // RA2: as above
	}
	Net2DisplayUsers(); // RA2: as above
	Send_Lobby_Options(true, false); // RA2: as above
}

void __fastcall Close_Slots_From(HWND dialog, int first) // RA2: 0x005CB7C0, not in TibSun; the slots from the first one on become open or closed, their controls are hidden and the lobby is redrawn and sent
{
	int limit = Get_Lobby_Slot_Limit(); // RA2: as above
	for (int slot = first; slot < 8; slot++) { // RA2: as above
		int id = Get_Slot_Control_Id(slot); // RA2: as above
		if (slot < limit) { // RA2: as above
			if (slot != LobbyObserverSlot) { // RA2: as above
				Change_Slot_Type(dialog, id, LobbyEntries[0], 1); // RA2: as above
			}
		} else if (slot != LobbyObserverSlot) { // RA2: as above
			Change_Slot_Type(dialog, id, LobbyEntries[1], 1); // RA2: as above
		}
	}
	Hide_Slot_Controls(dialog, first); // RA2: as above
	if (g_pChat != NULL) { // RA2: as above
		Redraw_Player_List(); // RA2: as above
	} else {
		Net2DisplayUsers(); // RA2: as above
	}
	Send_Lobby_Options(true, false); // RA2: as above
}

inline void Format_Slot(char * buffer, Wstring const & narrow) { sprintf(buffer, "O%d%s", LobbyObserverSlot, narrow.c_str()); } // RA2: ra2-decomp's file-local helper of the same name (src/netshare.cpp): the observer slot as the host sends it

int LobbySlotCount = 8; // RA2: 0x007E6DE4, the number of slots the scenario gave the lobby last (8 before one was chosen); used only by Change_Scenario_Slots
int LobbyExtraSlot; // RA2: 0x00A72D5C, 1 when the observer takes a slot of its own beyond the scenario's; as above

void __fastcall Change_Scenario_Slots(HWND dialog, int previous, int scenario) // RA2: 0x005CB8D0, not in TibSun; the lobby takes the number of slots of the scenario: new slots are shown and filled, removed ones are closed, and the observer's slot is moved to the last slot when the game mode asks for one
{
	int players = Get_Scenario_Slot_Limit(scenario); // RA2: as above
	bool ai_allowed = (Session.MPGameMode == NULL || Session.MPGameMode->IsSkirmishAllowed()) && !Is_Current_Channel_Tournament(); // RA2: as above
	if (!ai_allowed) { // RA2: as above
		int limit = Get_Lobby_Slot_Limit(); // RA2: as above
		if (players > limit) { // RA2: as above
			players = limit; // RA2: as above
		}
	}
	if (previous == -1) { // RA2: as above
		LobbySlotCount = 8; // RA2: as above
		LobbyExtraSlot = 0; // RA2: as above
	}
	int added = players - LobbySlotCount; // RA2: as above
	int extra = 0; // RA2: as above
	if (LobbyObserverSlot == 0) { // RA2: as above
		if (ai_allowed || players < Get_Lobby_Slot_Limit_Plus_One()) { // RA2: as above
			extra = 1; // RA2: as above
			if (LobbyExtraSlot == 0 && added + players < 7) { // RA2: as above
				added++; // RA2: as above
			}
		}
	}
	if (previous == -1 && LobbyObserverSlot == 0) { // RA2: as above
		int side = Player_Side_Combo_ID(0); // RA2: as above
		Fill_Observer_Combo(dialog, side); // RA2: as above
		Select_Side_Combo(dialog, side, -3); // RA2: as above
		SendMessageA(GetDlgItem(dialog, 1041), 0x4F1, 0, 1); // RA2: as above
		int color = Player_Color_Combo_ID(0); // RA2: as above
		Fill_Random_Color_Combo(dialog, color); // RA2: as above
		Select_Color_Combo(dialog, color, 8); // RA2: as above
	}
	if (added > 0) { // RA2: as above
		Change_Slot_Range(dialog, LobbyExtraSlot + LobbySlotCount, added); // RA2: as above
	} else if (added < 0) { // RA2: as above
		int first = extra + players; // RA2: the original expands Close_Slots_From's body here (main's text writes it out)
		int limit = Get_Lobby_Slot_Limit(); // RA2: as above
		for (int slot = first; slot < 8; slot++) { // RA2: as above
			int id = Get_Slot_Control_Id(slot); // RA2: as above
			if (slot < limit) { // RA2: as above
				if (slot != LobbyObserverSlot) { // RA2: as above
					Change_Slot_Type(dialog, id, LobbyEntries[0], 1); // RA2: as above
				}
			} else if (slot != LobbyObserverSlot) { // RA2: as above
				Change_Slot_Type(dialog, id, LobbyEntries[1], 1); // RA2: as above
			}
		}
		Hide_Slot_Controls(dialog, first); // RA2: as above
		if (g_pChat != NULL) { // RA2: as above
			Redraw_Player_List(); // RA2: as above
		} else {
			Net2DisplayUsers(); // RA2: as above
		}
		Send_Lobby_Options(true, false); // RA2: as above
	} else if (extra + players >= 7) { // RA2: as above
		Change_Slot_Range(dialog, 7, 1); // RA2: as above
	}
	Clear_Observer_Slots(dialog); // RA2: as above
	if (LobbyObserverSlot == -1 && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_A0()) { // RA2: as above
		LobbyObserverSlot = 7; // RA2: as above
		Clear_Observer_Slots(dialog); // RA2: as above
		DebugString("Sending observer slot of 7\n"); // RA2: as above
		NodeNameType * entry = Session.Slots[LobbyObserverSlot]; // RA2: as above
		WideWstring name; // RA2: as above
		if (entry == NULL || entry == LobbyEntries[0] || entry == LobbyEntries[1] || entry == LobbyEntries[5] || entry == LobbyEntries[6] // RA2: as above
			|| entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]) { // RA2: as above
			Wstring narrow = Lobby_Entry_Name(entry); // RA2: as above
			name = Wstring_ToWide(narrow); // RA2: as above
		} else {
			name = entry->Name; // RA2: as above
		}
		char text[256]; // RA2: as above
		Format_Slot(text, Wstring_Narrow(name.get())); // RA2: as above
		Send_Host_Option(text); // RA2: as above
	}
	LobbySlotCount = players; // RA2: as above
	LobbyExtraSlot = extra; // RA2: as above
}


bool __fastcall Lobby_Update_Dialog(HWND dialog, int count, bool flag1, bool flag2) // RA2: 0x005CBDB0, not in TibSun; tells whether the combo boxes hold a computer player where the game mode takes none, or a player beyond the count: an observer beyond it counts only when the game mode takes observers
{
	if (LobbyObserverSlot == 0) { // RA2: as above
		count++; // RA2: as above
	}
	for (int i = 0; i < 8; i++) { // RA2: as above
		int id = Get_Slot_Control_Id(i); // RA2: as above
		int index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0); // RA2: as above
		int data = SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0); // RA2: as above
		NodeNameType * entry; // RA2: as above
		if (data == -1) { // RA2: as above
			entry = NULL; // RA2: as above
		} else {
			entry = (NodeNameType *)data; // RA2: as above
		}
		if (Is_AI_Entry(entry) && Session.MPGameMode != NULL && !flag1) { // RA2: as above
			return(true); // RA2: as above
		}
		if (i >= count) { // RA2: as above
			bool player = entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[6] && entry != LobbyEntries[5]; // RA2: as above
			if (player && entry->Get_Country() != -3) { // RA2: as above
				return(true); // RA2: as above
			}
			if (Session.MPGameMode != NULL && !flag2 && player && entry->Get_Country() == -3) { // RA2: as above
				return(true); // RA2: as above
			}
		}
	}
	return(false); // RA2: as above
}

void __fastcall Reject_Join(NodeNameType * node); // RA2: 0x005BC3E0, defined in netdlg2.cpp
void Send_Slot_Names(void); // RA2: 0x005BBD20, defined in netdlg2.cpp
void Send_Player_List(void); // RA2: 0x0073C480, defined in wonline.cpp
void __fastcall Kick_Channel_User(wchar_t const * name); // RA2: 0x0075CAE0, looks the channel user up by name and has the chat service remove it (WOL)
void Update_Channel_Player_Limit(void); // RA2: 0x00749F20, tells the chat service the channel's new player limit (WOL)

#ifdef RA2_NON_MATCHING // not exact: 11 bytes differ against the image-derived bound 005cbf20 (2,003 B, ours 2,003 B; previously 1,745): ra2-decomp's banked text for the Yuri's Revenge twin (0x5EC9F0) in this tree's names, the entry as a NodeNameType pointer. Left: the load order of three lobby entries ahead of the first player test and the registers of the three computer entries in the count loop; 6 bytes (the count loop's load order alone) with the seven lobby entries as separate globals, the twin's form (lane F59, held)
void __fastcall Change_Slot_Type(HWND dialog, int id, NodeNameType * node, int mode) // RA2: 0x005CBF20, not in TibSun; seats an entry in the slot of a combo box: a computer player is refused beside an observer, a player who is replaced is turned away, and the boxes follow the new entry; mode 1 and 2 tell the other players, mode 2 sends the lobby options as well
{
	NodeNameType * newnode = node;
	int slot = Control_Slot(id);
	NodeNameType * old = Session.Slots[slot];

	if (Is_AI_Entry(node) && slot != LobbyObserverSlot) {
		if (ObserverMode || (LobbyObserverSlot > 0 && (Session.Slots[LobbyObserverSlot]->Slot.SpectatorFlag == -1 || Session.Slots[LobbyObserverSlot] == LobbyEntries[6]))) {
			Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_NO_AI_WITH_OBSERVER", NULL, "D:\\Ra2\\netshare.cpp", 5538));
			swap(node, old);
			if (Is_AI_Entry(node)) {
				node = LobbyEntries[1];
			}
			Select_Combo_By_Data(dialog, id, (int)node);
		}
	} else if (node == LobbyEntries[6]) {
		bool booted = false;
		for (int j = 0; j < 8; j++) {
			if (j != slot && Is_AI_Entry(Session.Slots[j])) {
				int slot_id = Get_Slot_Control_Id(j);
				Select_Combo_By_Data(dialog, slot_id, (int)LobbyEntries[1]);
				Change_Slot_Type(dialog, slot_id, LobbyEntries[1], false);
				booted = true;
			}
		}
		if (booted) {
			Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_OBSERVER_BOOTED_AI", NULL, "D:\\Ra2\\netshare.cpp", 5567));
		}
	}

	if (newnode == old) {
		return;
	}
	Session.Slots[slot] = newnode;

	if (mode == 1 || mode == 2) {
		if (!Is_Real_Player_Entry(newnode) && Is_Real_Player_Entry(old)) {
			if (g_pChat != NULL) {
				Kick_Channel_User(old->Name);
			} else {
				Reject_Join(old);
			}
		}
		if (g_pChat != NULL) {
			Send_Player_List();
		} else {
			Send_Slot_Names();
		}
	}

	bool new_ai = Is_AI_Entry(newnode);
	bool old_ai = Is_AI_Entry(old);
	bool new_obs = newnode == LobbyEntries[6] || (Is_Real_Player_Entry(newnode) && newnode->Slot.SpectatorFlag == -1);
	bool old_obs = old == LobbyEntries[6] || (Is_Real_Player_Entry(old) && old->Slot.SpectatorFlag == -1);

	if (new_obs) {
		Fill_Observer_Combo(dialog, Player_Side_Combo_ID(slot));
		SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 1);
		int color = Player_Color_Combo_ID(slot);
		Fill_Random_Color_Combo(dialog, color);
		Select_Color_Combo(dialog, color, 8);
		Update_Slot_Controls(dialog, slot, false);
		Session.Options.AISlots.Difficulties[slot] = -1;
		Session.Options.AISlots.Countries[slot] = -1;
		Session.Options.AISlots.Colors[slot] = -1;
	} else if (old_obs) {
		Fill_Side_Combo(dialog, Player_Side_Combo_ID(slot));
		SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 0);
		int color = Player_Color_Combo_ID(slot);
		Fill_Random_Color_Combo(dialog, color);
		Select_Color_Combo(dialog, color, -2);
		Update_Slot_Controls(dialog, slot, false);
		Session.Options.AISlots.Difficulties[slot] = -1;
		Session.Options.AISlots.Countries[slot] = -1;
		Session.Options.AISlots.Colors[slot] = -1;
	}

	if (new_ai) {
		Update_Slot_Controls(dialog, slot, true);
		Session.Options.AISlots.Difficulties[slot] = Get_AI_Difficulty(newnode);
		Session.Options.AISlots.Countries[slot] = -2;
		Session.Options.AISlots.Colors[slot] = -2;
	} else if (old_ai) {
		Update_Slot_Controls(dialog, slot, false);
		Session.Options.AISlots.Difficulties[slot] = -1;
		Session.Options.AISlots.Countries[slot] = -1;
		Session.Options.AISlots.Colors[slot] = -1;
	}

	if (g_pChat != NULL) {
		Update_Channel_Player_Limit();
	}

	bool ai_changed = new_ai || old_ai;
	bool obs_changed = new_obs || old_obs;
	if (ai_changed || obs_changed) {
		Session.Options.AIPlayers = Count_AI_Slots();
		if (mode == 2) {
			if (g_pChat != NULL) {
				Redraw_Player_List();
			} else {
				Net2DisplayUsers();
			}
			Send_Lobby_Options(true, false);
		}
	}
}
#endif

void __fastcall Select_Entry_Kind(HWND dialog, int id, int kind) // RA2: 0x005CC700, not in TibSun; picks the item of a type (see Get_Entry_Kind) in a slot's combo box and seats it
{
	NodeNameType * entry = Get_Kind_Entry(kind); // RA2: as above
	Select_Combo_By_Data(dialog, id, (int)entry); // RA2: as above
	Change_Slot_Type(dialog, id, entry, 0); // RA2: as above
}

void __fastcall Load_Random_Map_Preview(HWND dialog) // RA2: 0x005CC800, not in TibSun; the preview of a random map is read from RandMap.img, any other map's is drawn
{
	if (Session.Scenarios[Session.Options.ScenarioIndex]->Is_Random_Map()) { // RA2: as above
		delete MultiplayerMapPreview; // RA2: as above
		MultiplayerMapPreview = new MapPreviewClass; // RA2: as above
		MultiplayerMapPreview->Read_PCX_Preview("RandMap.img"); // RA2: as above
		if (MultiplayerMapPreview->Get_Preview_Surface() == NULL) { // RA2: as above
			Update_Network_Dialog_Preview(dialog); // RA2: as above
		}
		InvalidateRect(dialog, NULL, FALSE); // RA2: as above
	} else {
		Update_Network_Dialog_Preview(dialog); // RA2: as above
	}
}

void __fastcall Save_Lobby_Options(HWND dialog, SessionOptionsClass * options) // RA2: 0x005CC890, not in TibSun; the game mode, map, the type, side and color of every slot but the first and the game settings are kept in the options
{
	options->MPGameMode = Session.Options.MPModeIndex; // RA2: as above
	options->ScenIndex = Session.Options.ScenarioIndex; // RA2: as above
	for (int slot = 1; slot < 8; slot++) { // RA2: as above
		int id = Get_Slot_Control_Id(slot); // RA2: as above
		int index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0); // RA2: as above
		int data = SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0); // RA2: as above
		options->SlotData[slot][0] = Get_Entry_Kind((NodeNameType *)(data == -1 ? 0 : data)); // RA2: as above
		int side_id = Player_Side_Combo_ID(slot); // RA2: as above
		options->SlotData[slot][1] = Side_Combo_Data(dialog, side_id, -1); // RA2: as above
		int color_id = Player_Color_Combo_ID(slot); // RA2: as above
		options->SlotData[slot][2] = Color_Combo_Data(dialog, color_id, -1); // RA2: as above
	}
	options->GameSpeed = Session.Options.GameSpeed; // RA2: as above
	options->Credits = Session.Options.Credits; // RA2: as above
	options->UnitCount = Session.Options.UnitCount; // RA2: as above
	options->ShortGame = Session.Options.ShortGame; // RA2: as above
	options->SuperWeaponsAllowed = Session.Options.SWAllowed; // RA2: as above
	options->BuildOffAlly = Session.Options.BuildOffAlly; // RA2: as above
	options->MCVRepacks = Session.Options.MCVRedeploy; // RA2: as above
	options->CratesAppear = Session.Options.Goodies; // RA2: as above
}

void __fastcall Load_Lobby_Options(HWND dialog, SessionOptionsClass * options) // RA2: 0x005CCA30, not in TibSun; the saved game mode, map and settings are made current again (a restricted channel takes the first game mode if the saved one is not allowed) and every slot but the first is set to its saved type, side and color
{
	if (options->MPGameMode < 1 || options->MPGameMode > MultiplayerGameMode::GetModeCount()) { // RA2: as above
		options->MPGameMode = First_Game_Mode()->MPModeIndex; // RA2: as above
	}
	Session.Options.MPModeIndex = options->MPGameMode; // RA2: as above
	MultiplayerGameMode * mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex); // RA2: as above
	if (mode == NULL) { // RA2: as above
		mode = First_Game_Mode(); // RA2: as above
	}
	if ((Is_Current_Channel_Tournament() || Is_Current_Channel_Flag_8()) && mode != NULL && mode->vt_entry_04()) { // RA2: as above
		options->MPGameMode = First_Game_Mode()->MPModeIndex; // RA2: as above
		Session.Options.MPModeIndex = options->MPGameMode; // RA2: as above
		MultiplayerGameMode * found = MultiplayerGameMode::Get(Session.Options.MPModeIndex); // RA2: as above
		if (found == NULL) { // RA2: as above
			found = First_Game_Mode(); // RA2: as above
		}
		mode = found; // RA2: as above
	}
	options->MPGameMode = mode->MPModeIndex; // RA2: as above
	Session.Options.MPModeIndex = mode->MPModeIndex; // RA2: as above
	if (options->ScenIndex < 0 || options->ScenIndex >= Session.Scenarios.Count()) { // RA2: as above
		options->ScenIndex = 0; // RA2: as above
	}
	Session.Options.ScenarioIndex = Set_Scenario_Info_From_Index(options->ScenIndex) ? options->ScenIndex : 0; // RA2: as above
	MultiMission * scenario = Session.Scenarios[Session.Options.ScenarioIndex]; // RA2: the scenario being checked
	if (!mode->IsMapAllowed(scenario)) { // RA2: as above
		int index = mode->FirstValidMapIndex(); // RA2: as above
		if (index == -1) { // RA2: as above
			mode = First_Game_Mode(); // RA2: as above
			options->MPGameMode = mode->MPModeIndex; // RA2: as above
			Session.Options.MPModeIndex = mode->MPModeIndex; // RA2: as above
			index = mode->FirstValidMapIndex(); // RA2: as above
		}
		options->ScenIndex = index; // RA2: as above
		Session.Options.ScenarioIndex = index; // RA2: as above
		Set_Scenario_Info_From_Index(index); // RA2: as above
	}
	Session.MPGameMode = mode; // RA2: as above
	Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex); // RA2: as above
	Session.MPGameMode->vt_entry_20(); // RA2: as above
	Session.MPGameMode->vt_entry_24(); // RA2: as above
	mode->AssignTeamsAtStart(); // RA2: as above
	mode->UnfixAlliances(); // RA2: as above
	int limit = 8; // RA2: as above
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		limit = (Is_Current_Channel_Tournament() && !Is_Current_Channel_Flag_8()) ? 2 : 4; // RA2: as above
	}
	if (LobbyObserverSlot == 0 && limit < 8) { // RA2: as above
		limit++; // RA2: as above
	}
	for (int slot = 1; slot < 8; slot++) { // RA2: as above
		int id = Get_Slot_Control_Id(slot); // RA2: as above
		int kind = options->SlotData[slot][0]; // RA2: as above
		if (Is_Current_Channel_Tournament() || LobbyObserverSlot == 0 || ObserverMode != 0) { // RA2: as above
			if (kind == 4) { // RA2: as above
				kind = 2; // RA2: as above
			} else if (kind == 5) { // RA2: as above
				kind = 2; // RA2: as above
			} else if (kind == 6) { // RA2: as above
				kind = 2; // RA2: as above
			}
		}
		if (slot >= limit) { // RA2: as above
			kind = 3; // RA2: as above
		} else if (kind == 7) { // RA2: as above
			kind = 2; // RA2: as above
		}
		Select_Entry_Kind(dialog, id, kind); // RA2: as above
		int side_id = Player_Side_Combo_ID(slot); // RA2: as above
		int color_id = Player_Color_Combo_ID(slot); // RA2: as above
		if (kind != 2 && kind != 3) { // RA2: as above
			if (kind == 7) { // RA2: as above
				Select_Side_Combo(dialog, side_id, -3); // RA2: as above
				Select_Color_Combo(dialog, color_id, -2); // RA2: as above
				HWND side = GetDlgItem(dialog, side_id); // RA2: as above
				HWND colors = GetDlgItem(dialog, color_id); // RA2: as above
				EnableWindow(side, FALSE); // RA2: as above
				EnableWindow(colors, FALSE); // RA2: as above
			} else {
				Select_Side_Combo(dialog, side_id, options->SlotData[slot][1]); // RA2: as above
				Set_Slot_Country(dialog, side_id); // RA2: as above
				Select_Color_Combo(dialog, color_id, options->SlotData[slot][2]); // RA2: as above
				Set_Slot_Color(dialog, color_id); // RA2: as above
			}
		} else {
			Select_Side_Combo(dialog, side_id, Session.MPGameMode->DefaultCountry()); // RA2: as above
			Select_Color_Combo(dialog, color_id, -2); // RA2: as above
			HWND side = GetDlgItem(dialog, side_id); // RA2: as above
			HWND colors = GetDlgItem(dialog, color_id); // RA2: as above
			EnableWindow(side, FALSE); // RA2: as above
			EnableWindow(colors, FALSE); // RA2: as above
		}
	}
	Session.Options.GameSpeed = options->GameSpeed; // RA2: as above
	Session.Options.Credits = options->Credits; // RA2: as above
	Session.Options.UnitCount = options->UnitCount; // RA2: as above
	Session.Options.ShortGame = options->ShortGame; // RA2: as above
	Session.Options.SWAllowed = options->SuperWeaponsAllowed; // RA2: as above
	Session.Options.BuildOffAlly = options->BuildOffAlly; // RA2: as above
	Session.Options.MCVRedeploy = options->MCVRepacks; // RA2: as above
	Session.Options.Goodies = options->CratesAppear; // RA2: as above
}


void __fastcall Get_Window_Wide_Text(HWND window, WideWstring & text); // RA2: 0x00600E50, the text of a window, as Get_List_Item_Wide_Text for a list box item

WideWstring __fastcall Limit_Name_Text(HWND control) // RA2: 0x005CCF50, not in TibSun; the text of a name edit with the line breaks cut off; it is cut to what the lobby allows: 120 characters, or the ones below 0x100 after the first wider character (40 at least) in a name with wider characters
{
	WideWstring text; // RA2: as above
	Get_Window_Wide_Text(control, text); // RA2: as above
	unsigned int limit = 120; // RA2: as above
	if (!text.Is_Narrow()) { // RA2: as above
		unsigned int i; // RA2: as above
		for (i = 0; i < text.length(); i++) { // RA2: as above
			if (text.getChar(i) > 256) { // RA2: as above
				break; // RA2: as above
			}
		}
		limit = i - 1; // RA2: as above
		if (limit <= 40) { // RA2: as above
			limit = 40; // RA2: as above
		}
	}
	if (text.length() > limit) { // RA2: as above
		text.truncate(limit); // RA2: as above
		SendMessageA(control, OD_SETTEXTW, 0, (LPARAM)text.text()); // RA2: as above
	}
	{
		WideWstring line_breaks(L"\n\r"); // RA2: as above
		text.trimTrailing(line_breaks); // RA2: as above
	}
	return(text); // RA2: as above
}

void __fastcall Enable_Player_Combos(HWND dialog, bool value) // RA2: 0x005CD030, not in TibSun; enables or disables the side and color boxes of the first player's slot, unless it is the observer's
{
	if (dialog != NULL) { // RA2: as above
		int slot = Find_First_Player_Slot(); // RA2: as above
		if (slot != LobbyObserverSlot) { // RA2: as above
			HWND control = GetDlgItem(dialog, Player_Side_Combo_ID(slot)); // RA2: as above
			if (control != NULL) { // RA2: as above
				EnableWindow(control, value); // RA2: as above
			}
			control = GetDlgItem(dialog, Player_Color_Combo_ID(slot)); // RA2: as above
			if (control != NULL) { // RA2: as above
				EnableWindow(control, value); // RA2: as above
			}
		}
	}
}

char const * LobbyEntryCodes[7] = { "@Open@", "@Closed@", "@Waiting@", "@EasyAI@", "@MediumAI@", "@HardAI@", "@OpenObserver@" }; // RA2: 0x007E6DE8, the names the fixed entries go by in the slot names the host sends: open, closed, waiting, easy, medium and hard computer player and open observer slot

Wstring __fastcall Lobby_Entry_Name(NodeNameType * entry) // RA2: 0x005CD0E0, not in TibSun; the name of a fixed entry in the slot names the host sends
{
	if (entry == LobbyEntries[0]) { // RA2: as above
		return(Wstring(LobbyEntryCodes[0])); // RA2: as above
	}
	if (entry == LobbyEntries[1]) { // RA2: as above
		return(Wstring(LobbyEntryCodes[1])); // RA2: as above
	}
	if (entry == LobbyEntries[5]) { // RA2: as above
		return(Wstring(LobbyEntryCodes[2])); // RA2: as above
	}
	if (entry == LobbyEntries[2]) { // RA2: as above
		return(Wstring(LobbyEntryCodes[3])); // RA2: as above
	}
	if (entry == LobbyEntries[3]) { // RA2: as above
		return(Wstring(LobbyEntryCodes[4])); // RA2: as above
	}
	if (entry == LobbyEntries[4]) { // RA2: as above
		return(Wstring(LobbyEntryCodes[5])); // RA2: as above
	}
	if (entry == LobbyEntries[6]) { // RA2: as above
		return(Wstring(LobbyEntryCodes[6])); // RA2: as above
	}
	return(Wstring(LobbyEntryCodes[2])); // RA2: as above
}

NodeNameType * __fastcall Find_Lobby_Entry_By_Name(WideWstring const & name) // RA2: 0x005CD1B0, not in TibSun; the fixed entry a name of the slot names stands for, NULL for any other
{
	if (name.Is_Narrow()) { // RA2: as above
		Wstring narrow = Wstring_Narrow(name); // RA2: as above
		if (narrow == LobbyEntryCodes[0]) { // RA2: as above
			return(LobbyEntries[0]); // RA2: as above
		}
		if (narrow == LobbyEntryCodes[1]) { // RA2: as above
			return(LobbyEntries[1]); // RA2: as above
		}
		if (narrow == LobbyEntryCodes[2]) { // RA2: as above
			return(LobbyEntries[5]); // RA2: as above
		}
		if (narrow == LobbyEntryCodes[3]) { // RA2: as above
			return(LobbyEntries[2]); // RA2: as above
		}
		if (narrow == LobbyEntryCodes[4]) { // RA2: as above
			return(LobbyEntries[3]); // RA2: as above
		}
		if (narrow == LobbyEntryCodes[5]) { // RA2: as above
			return(LobbyEntries[4]); // RA2: as above
		}
		if (narrow == LobbyEntryCodes[6]) { // RA2: as above
			return(LobbyEntries[6]); // RA2: as above
		}
	}
	return(NULL); // RA2: as above
}

bool __fastcall Is_Lobby_Name(Wstring const & name) // RA2: 0x005CD2F0, not in TibSun; the name is one of the seven names of the fixed entries
{
	return(name == LobbyEntryCodes[0] || name == LobbyEntryCodes[1] || name == LobbyEntryCodes[2] || name == LobbyEntryCodes[3] || name == LobbyEntryCodes[4] || name == LobbyEntryCodes[5] || name == LobbyEntryCodes[6]); // RA2: as above
}

void Load_Observer_Mode(void) // RA2: 0x005CD380, not in TibSun; reads the observer mode from RA2.INI
{
	ObserverMode = ConfigINI.Get_Int("MultiPlayer", "ObserverMode", 0); // RA2: as above
}

void __fastcall Save_Observer_Mode(void) // RA2: 0x005CD3A0, not in TibSun; writes the observer mode to RA2.INI
{
	RawFileClass file("RA2.INI"); // RA2: as above
	ConfigINI.Put_Int("MultiPlayer", "ObserverMode", ObserverMode); // RA2: as above
	ConfigINI.Save(file, false); // RA2: as above
}

void __fastcall Update_Observer_Button(HWND dialog) // RA2: 0x005CD3F0, not in TibSun; the observer radio button, its picture and the text of the join button follow the observer mode
{
	if (dialog != NULL) { // RA2: as above
		HWND control = GetDlgItem(dialog, 1888); // RA2: as above
		if (control != NULL) { // RA2: as above
			SendMessageA(control, BM_SETCHECK, ObserverMode != 0, 0); // RA2: as above
		}
		Surface * image; // RA2: as above
		if (ObserverMode) { // RA2: as above
			image = SurfaceCache.GetSurface("wolob.pcx", NULL); // RA2: as above
		} else {
			image = SurfaceCache.GetSurface("wolnob.pcx", NULL); // RA2: as above
		}
		HWND picture = GetDlgItem(dialog, 1887); // RA2: as above
		OwnerDraw::Set_Control_Image(picture, image); // RA2: as above
		control = GetDlgItem(dialog, 1741); // RA2: as above
		if (control != NULL) { // RA2: as above
			wchar_t const * text; // RA2: as above
			if (ObserverMode) { // RA2: as above
				text = StringTable::LoadString("GUI:Observe", NULL, "D:\\Ra2\\netshare.cpp", 6072); // RA2: as above
			} else {
				text = StringTable::LoadString("GUI:Join", NULL, "D:\\Ra2\\netshare.cpp", 6074); // RA2: as above
			}
			SendMessageA(control, OD_SETTEXTW, 0, (LPARAM)text); // RA2: as above
			InvalidateRect(control, NULL, FALSE); // RA2: as above
		}
		control = GetDlgItem(dialog, IDC_JOINLEAVE); // RA2: as above
		if (control != NULL) { // RA2: as above
			wchar_t const * text; // RA2: as above
			if (ObserverMode) { // RA2: as above
				text = StringTable::LoadString("GUI:Observe", NULL, "D:\\Ra2\\netshare.cpp", 6083); // RA2: as above
			} else if (WS_Find_Dialog(0x113) == dialog) { // RA2: as above
				text = StringTable::LoadString("GUI:JoinGame", NULL, "D:\\Ra2\\netshare.cpp", 6087); // RA2: as above
			} else {
				text = StringTable::LoadString("GUI:Join", NULL, "D:\\Ra2\\netshare.cpp", 6089); // RA2: as above
			}
			SendMessageA(control, OD_SETTEXTW, 0, (LPARAM)text); // RA2: as above
			InvalidateRect(control, NULL, FALSE); // RA2: as above
		}
	}
}

void __fastcall Update_Observer_Buttons(HWND dialog) // RA2: 0x005CD540, not in TibSun; the observer buttons of the dialog and of every lobby dialog that is open follow the observer mode
{
	if (dialog != NULL) { // RA2: as above
		Update_Observer_Button(dialog); // RA2: as above
	}
	HWND other = WS_Find_Dialog(0x116); // RA2: as above
	if (other != NULL) { // RA2: as above
		Update_Observer_Button(other); // RA2: as above
	}
	other = WS_Find_Dialog(0x113); // RA2: as above
	if (other != NULL) { // RA2: as above
		Update_Observer_Button(other); // RA2: as above
	}
	other = WS_Find_Dialog(IDD_MPLAYER_GAME_LIST); // RA2: as above
	if (other != NULL) { // RA2: as above
		Update_Observer_Button(other); // RA2: as above
	}
}

