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
#include "narrow.h" // RA2: ra2-decomp's Narrow_Text at the two preview debug lines of Send_Map_Preview
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
void __fastcall Dialog_On_WM_DRAWITEM(long item);
void __fastcall Show_Dialog(HWND dialog, int kind);
void __fastcall Move_Default_Button_Owner(HWND window);
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
		hash += *(string.get() + i) + i;
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
	if (dialog == NULL) {
		dialog = WS_Find_Dialog(IDD_WOL_GUEST);
	}
	if (dialog == NULL) {
		dialog = WS_Find_Dialog(IDD_MPLAYER_HOST);
	}
	if (dialog == NULL) {
		dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST);
	}
	return(dialog);
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
	wchar_t buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	va_start(va, fmt);
	vswprintf(buffer, fmt, va);
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
	wchar_t buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	va_start(va, fmt);
	vswprintf(buffer, fmt, va);
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
	struct { HDC hdc; SIZE size; } l;

	int length = wcslen(message);

	int offset = 18;
	Get_Display_Rect(window, &rect);
	if (SendMessage(window, OD_HASATTACHED, 0, 0)) {
		offset = 1;
	}

	l.hdc = GetDC(window);
	SendMessage(window, OD_RESTOREDC, 0, (LPARAM)l.hdc);

	while (length) {
		if (message != NULL) {
			wchar_t const * newline = wcschr(message, L'\n');
			if (newline != NULL) {
				int linelen = newline - message + 1;
				if (length >= linelen) {
					length = linelen;
				}
			}
		}

		wchar_t * copy = _wcsdup(message);
		copy[length] = 0;
		BitFont::Instance->Text_Dimensions(copy, (int *)&l.size.cx, (int *)&l.size.cy, 0);
		free(copy);
		int maxWidth = rect.right - rect.left - offset;

		if (l.size.cx >= maxWidth - 4) {
			int reduceBy;
			if (l.size.cx / 2 > maxWidth) {
				reduceBy = 10;
				length -= reduceBy;
			} else {
				reduceBy = 1;
			}

			int found = -1;
			int idx = length - 1;

			while (idx > 0) {
				if (!iswgraph(message[idx])) {
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
			while (*message == L' ') {
				message++;
			}
			length = wcslen(message);
		}
	}

	ReleaseDC(window, l.hdc);
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
	static wchar_t buffer[512];
	memset(buffer, 0, sizeof(buffer));
	wcsncpy(buffer, message, length);
	wchar_t * line_end = wcschr(buffer, L'\r');
	if (line_end != NULL) {
		line_end[0] = L'\0';
	} else {
		line_end = wcschr(buffer, L'\n');
		if (line_end != NULL) {
			line_end[0] = L'\0';
		}
	}

	int old = SendMessage(window, OD_DISABLEPAINT, 0, 1);
	int topindex = SendMessage(window, LB_GETTOPINDEX, 0, 0);
	int count = SendMessage(window, LB_GETCOUNT, 0, 0);
	int itemheight = SendMessage(window, LB_GETITEMHEIGHT, topindex, 0);
	RECT rect;
	GetWindowRect(window, &rect);
	bool atbottom = (count - topindex) <= (rect.bottom - rect.top) / itemheight;
	if (count > 500) {
		SendMessage(window, LB_DELETESTRING, 0, 0);
		count--;
	}

	int index = SendMessage(window, OD_ADDSTRINGW, 0, (LPARAM)buffer);
	if (color != -1) {
		SendMessage(window, OD_SETCOLOR, index, color);
	}

	if (atbottom) {
		SendMessage(window, LB_SETTOPINDEX, count, 0);
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

		int old = SendMessage(msg, OD_DISABLEPAINT, 0, 1);
		int count = SendMessage(msg, LB_GETCOUNT, 0, 0);
		WideWstring text;
		if (count > 0) {
			Get_List_Item_Wide_Text(msg, count - 1, text);
		}
		if (text.length() < 0x24) {
			SendMessage(msg, LB_DELETESTRING, count - 1, 0);
			text += L".";
		} else {
			text = L".";
			count++;
		}
		SendMessage(msg, OD_ADDSTRINGW, 0, (LPARAM)text.text());
		while (count > 500) {
			SendMessage(msg, LB_DELETESTRING, 0, 0);
		}
		SendMessage(msg, OD_DISABLEPAINT, 0, old);
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
	if (text != NULL && wcslen(text) != 0) {
		int id;
		if (large) {
			id = 0xfb;
		} else if (type == MB_OK) {
			id = IDD_MSGBOX_3_SMALL;
		} else if (type == MB_OKCANCEL) {
			id = 0xfc;
		} else {
			id = (type != MB_YESNO) ? 0xfb : 0x126;
		}
		HWND dialog = WS_Create_Dialog(ProgramInstance, id, MainWindow, ODMessageBox_Proc, false);
		Show_Dialog(dialog, id);
		if (type == MB_OK) {
			HWND ok = GetDlgItem(dialog, IDOK);
			SetWindowLong(ok, GWL_STYLE, GetWindowLong(ok, GWL_STYLE) | WS_VISIBLE);
			if (large) {
				Move_Default_Button_Owner(ok);
			}
		} else if (type == MB_OKCANCEL) {
			HWND ok = GetDlgItem(dialog, IDOK);
			SetWindowLong(ok, GWL_STYLE, GetWindowLong(ok, GWL_STYLE) | WS_VISIBLE);
			HWND cancel = GetDlgItem(dialog, IDCANCEL);
			SetWindowLong(cancel, GWL_STYLE, GetWindowLong(cancel, GWL_STYLE) | WS_VISIBLE);
		} else if (type == MB_YESNO) {
			HWND yes = GetDlgItem(dialog, IDYES);
			SetWindowLong(yes, GWL_STYLE, GetWindowLong(yes, GWL_STYLE) | WS_VISIBLE);
			HWND no = GetDlgItem(dialog, IDNO);
			SetWindowLong(no, GWL_STYLE, GetWindowLong(no, GWL_STYLE) | WS_VISIBLE);
		}
		HWND label = GetDlgItem(dialog, IDC_MSGBOX_TEXT);
		SendMessage(label, OD_SETTEXTW, 0, (LPARAM)text);
		ShowWindow(dialog, SW_NORMAL);
		return(WS_Wait_Dialog(dialog, callback, false, true, true));
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
			Dialog_On_WM_DRAWITEM(lparam);
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
	HWND item = GetDlgItem(dialog, 0x6ec);
	SendMessage(item, OD_SETTEXTW, 0, (LPARAM)mode->UIName.text());
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
	MultiMission * map = Session.Scenarios[index];
	HWND item = GetDlgItem(dialog, IDC_SCENARIONAME);
	SendMessage(item, OD_SETTEXTW, 0, (LPARAM)map);
}

/// <summary>
/// Shows the description of the chosen scenario in a lobby dialog.
/// </summary>
/// <param name="dialog">The lobby dialog.</param>
void __fastcall Set_Skirmish_Dialog_Text(HWND dialog) // RA2: 0x005C34B0, TibSun has no such routine
{
	HWND item = GetDlgItem(dialog, IDC_SCENARIONAME);
	SendMessage(item, OD_SETTEXTW, 0, (LPARAM)Session.Options.ScenarioDescription);
}

/// <summary>
/// Fetches the game mode the session options name.
/// The first game mode of the list is used when the options name none that exists.
/// </summary>
/// <returns>Returns with the current game mode.</returns>
MultiplayerGameMode * Get_Current_Game_Mode(void) // RA2: 0x005C34D0, TibSun has no such routine
{
	MultiplayerGameMode * mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex);
	if (mode == NULL) {
		mode = First_Game_Mode();
	}
	return(mode);
}

/// <summary>
/// Passes a change of the game mode on to the game mode itself.
/// In the game mode dialog the mode picked in its list is told; elsewhere the session's own mode is.
/// </summary>
void __fastcall Handle_Mode_Selection(void) // RA2: 0x005C34F0, TibSun has no such routine; slot 31 (offset 0x7C) of the game mode is called
{
	if (WS_Top_Window_ID() == 0x6b) {
		HWND dialog = WS_Top_Window();
		int selected = SendDlgItemMessage(dialog, 0x6eb, LB_GETCURSEL, 0, 0);
		if (selected != -1) {
			MultiplayerGameMode * mode = (MultiplayerGameMode *)SendDlgItemMessage(dialog, 0x6eb, LB_GETITEMDATA, selected, 0);
			if (mode != NULL) {
				mode->vt_entry_7C();
			}
		}
	} else if (Session.MPGameMode != NULL) {
		Session.MPGameMode->vt_entry_7C();
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
	int unit_count = Session.Options.UnitCount;
	int credits = Session.Options.Credits;
	int game_speed = Session.Options.GameSpeed;

	if (initialize) {
		rc_ptr<WDTTerritory> territory;
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) {
			territory = Find_WDT_Territory(Session.TournamentTerritory);
		}
		if (territory.Get() != NULL) {
			EnableWindow(GetDlgItem(window, IDC_YOURSIDE), FALSE);
			EnableWindow(GetDlgItem(window, 0x6d4), FALSE);
			if (!(territory->UserModBooleans & 0x20000)) {
				EnableWindow(GetDlgItem(window, IDC_CREDITS), FALSE);
			}
			if (!(territory->UserModBooleans & 0x8000)) {
				EnableWindow(GetDlgItem(window, IDC_UNITCOUNT), FALSE);
			}
			if (!(territory->UserModBooleans & 0x400)) {
					EnableWindow(GetDlgItem(window, IDC_SHORT_GAME), FALSE); /// Short Game
			}
			if (!(territory->UserModBooleans & 0x20)) {
					EnableWindow(GetDlgItem(window, IDC_REDEPLOY_MCV), FALSE); /// Re-Deployable MCV
			}
			if (!(territory->UserModBooleans & 0x100)) {
				EnableWindow(GetDlgItem(window, IDC_CRATES), FALSE);
			}
		}
		SendDlgItemMessage(window, IDC_GAME_SPEED_SLIDER, TBM_SETRANGE, TRUE, MAKELONG(0, 6)); /// Game Speed
		SendDlgItemMessage(window, IDC_CREDITS, TBM_SETRANGE, TRUE, MAKELONG(Rule->MinMoney, Rule->MPMaxMoney));
		SendDlgItemMessage(window, IDC_CREDITS, OD_SETTRACKSTEP, 0, Rule->MoneyIncrement);
		SendDlgItemMessage(window, IDC_UNITCOUNT, TBM_SETRANGE, TRUE, MAKELONG(Rule->MinUnitCount, Rule->MaxUnitCount));
	}

	SendDlgItemMessage(window, IDC_GAME_SPEED_SLIDER, TBM_SETPOS, TRUE, 6 - game_speed);
	SendDlgItemMessage(window, IDC_CREDITS, TBM_SETPOS, TRUE, credits);
	SendDlgItemMessage(window, IDC_UNITCOUNT, TBM_SETPOS, TRUE, unit_count);

	int button_state[] = { BST_UNCHECKED, BST_CHECKED };
	SendDlgItemMessage(window, IDC_SHORT_GAME, BM_SETCHECK, button_state[Session.Options.ShortGame], 0);
	SendDlgItemMessage(window, 0x69a, BM_SETCHECK, button_state[Session.Options.SWAllowed], 0);
	SendDlgItemMessage(window, 0x69d, BM_SETCHECK, button_state[Session.Options.BuildOffAlly], 0);
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
	static int _last_pump_time;
	static bool _need_to_pump;
	static bool _last_short_game;
	static bool _last_sw_allowed;
	static bool _last_build_off_ally;
	static bool _last_mcv_redeploy;
	static bool _last_crates;
	static int _last_game_speed = -1;
	static int _last_credits = -1;
	static int _last_unit_count = -1;
	static bool _last_scenario_is_official = true;
	static unsigned int _last_scenario_file_length = -1;
	static wchar_t _last_scenario_description[45];
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
		if (_last_sw_allowed != Session.Options.SWAllowed) do_pump = true;
		if (_last_build_off_ally != Session.Options.BuildOffAlly) do_pump = true;
		if (_last_mcv_redeploy != Session.Options.MCVRedeploy) do_pump = true;
		if (_last_crates != Session.Options.Goodies) do_pump = true;

		_last_scenario_description[43] = 0;
		if (wcsicmp(_last_scenario_description, Session.Options.ScenarioDescription)) do_pump = true;

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
			_last_sw_allowed = Session.Options.SWAllowed;
			_last_build_off_ally = Session.Options.BuildOffAlly;
			_last_mcv_redeploy = Session.Options.MCVRedeploy;
			_last_crates = Session.Options.Goodies;
			wcscpy(_last_scenario_description, Session.Options.ScenarioDescription);
			strcpy(_last_scenario_digest, Session.ScenarioDigest);
			strcpy(_last_scenario_file_name, Session.ScenarioFileName);
			_last_scenario_file_length = Session.ScenarioFileLength;
			_last_scenario_is_official = Session.ScenarioIsOfficial;

			char buffer[520];
			memset(buffer, '\0', sizeof(buffer));

			if (g_pChat != NULL) {
				Encode_Game_Options(buffer);
				Send_Host_Option(buffer);
			} else {
				Net2EncodeGameopt(buffer);
				Send_Host_Option(buffer);
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
	packet.Command = NET_COMMAND_28;
	strcpy(packet.Name, Session.Handle);
	int count = Session.Players.Count();
	for (int i = 1; i < count; i++) {
		Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address, 0, 0);
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
			Send_Start_Reminder();
			break;

		case GAME_INTERNET:
			Wol_Send_Start_Reminder();
			break;
	}
}

/// <summary>
/// Handles the host's start reminder in the guest's lobby.
/// The guest is marked as reminded and told, through the chat service or the network layer.
/// </summary>
void __fastcall Handle_Start_Reminder(void) // RA2: 0x005C3C20, TibSun has no such routine
{
	Session.IsStartReminded = true;
	if (g_pChat != NULL) {
		Wol_Handle_Start_Reminder();
	} else {
		Join_Lobby_Start_Reminder();
	}
}

/// <summary>
/// Handles the host taking the start reminder back in the guest's lobby.
/// The reminded mark is cleared and the ready button is reset, through the chat service or the network layer.
/// </summary>
void __fastcall Handle_Reset_Ready(void) // RA2: 0x005C3C40, TibSun has no such routine
{
	Session.IsStartReminded = false;
	if (g_pChat != NULL) {
		Wol_Reset_Ready();
	} else {
		Join_Lobby_Reset_Ready();
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
		char buffer[0x400];
		strncpy(buffer, options, 0x3ff);
		g_pChat->RequestPublicGameOptions((LPCSTR)buffer);
	} else {
		GlobalPacketType packet;
		memset(&packet, 0, sizeof(packet));
		packet.Command = NET_PUB_GAMEOPT;
		strcpy(packet.Name, Session.Handle);
		strcpy(packet.Options.Buf, options);
		packet.Options.Color = Session.PrefColor;
		packet.Options.NameCRC = Compute_Name_CRC(Session.GameName);
		for (int i = 1; i < Session.Players.Count(); i++) {
			NodeNameType * node = Session.Players[i];
			DebugString("Sending public game options to %s\n", Wide_To_Narrow(node->Name));
			Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &node->Address, 0, 0);
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
		packet.Command = NET_PRIV_GAMEOPT;
		strcpy(packet.Name, Session.Handle);
		strcpy(packet.Options.Buf, options);
		Session.GPacket.Options.Color = Session.Chosen_Color();
		Session.GPacket.Options.NameCRC = Compute_Name_CRC(Session.GameName);
		for (int i = 1; i < Session.Players.Count(); i++) {
			if (stricmp(Wide_To_Narrow(Session.Players[i]->Name), player) == 0) {
				DebugString("Sending private game options to %ls\n", Session.Players[i]->Name);
				Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.Players[i]->Address, 0, 0);
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
	if (name == NULL) {
		name = Session.Handle;
	}
	if (wcscmp(Narrow_To_Wide(name), Session.GameName) == 0) {
		state = 1;
	}
	int found = -1;
	for (int i = 0; i < Session.Players.Count(); i++) {
		if (stricmp(Wide_To_Narrow(Session.Players[i]->Name), name) == 0) {
			found = i;
			break;
		}
	}
	if (found != -1) {
		Session.Players[found]->Slot.Unknown7F = state;
		if (state == 0) {
			Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostNoMap", NULL, "D:\\Ra2\\netshare.cpp", 1115), Session.Players[found]->Name, Session.Options.ScenarioDescription);
		} else if (state == 2) {
			Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:HostMapTransfer", NULL, "D:\\Ra2\\netshare.cpp", 1119), Session.Players[found]->Name, Session.Options.ScenarioDescription);
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
#ifdef RA2_NON_MATCHING // not exact: 7723 bytes differ by position against the image-derived bound 005c3ff0 (8,191 B, ours 8,176 B; 7,730 before): ra2-decomp's text for the Yuri's Revenge twin (DecodePubGameopt 0x005E3D10, matched there) in this tree's names, every reference bound from the image's operands (lane O91). Lane F65: the frame is the executable's (0x390, every slot) with the twin's own forms (a break in each NAT case, the implicit Wstring conversions, the address as an unsigned long and a cast store, GameoptWindow kept in one variable) and this executable's buffer sizes (one number a slot: text[4], and lists that fit under the INI's 56 bytes). The executable's shapes where it differs from the twin stay: no lock wrapper (an early return), the observer dialog test as one condition and one map-state call. Left: one register order at the top (the executable keeps zero in edi, the copy in ebx and the sender in ebp; ours the copy in edi and the sender in ebx, with 4 cached in ebp), and one inline charge (the executable calls GenericNode's constructor, 0x0040E160, for the INI's second list node). Both turn together: with one reference to the sender fewer, or one more inline site ahead of the Ping case, 71 instructions are left (the NAT case 3 pointer registers, two zero pushes in the observer case, the map name's register in the campaign case and the INI's node)
static inline void AI_Slot_String(char * buffer) // RA2: ra2-decomp's file-local helper of the same name (src/netshare.cpp); this executable lists the computer players' countries only, so one number and its comma fit in four characters (the executable keeps the text in a dword slot it shares with the port)
{
	buffer[0] = 0;
	for (int s = 0; s < 8; s++) {
		char text[4];
		sprintf(text, "%d,", Session.Options.AISlots.Countries[s]);
		strcat(buffer, text);
	}
}

/*
 * If the host is forcing a color reassignment, then this flag will be true. It is set when
 * the host accepts the game options and cleared once the assignment has been processed, and
 * it suppresses the "color in use" message that would otherwise appear.
 */
int IsColorChangePending; // RA2: used only by Net2DecodeGameopt (and wonline's parked draft), so parked with it (DECISIONS: a global whose only user is a parked function)

bool __fastcall Net2DecodeGameopt(char const * options, char const * name) // RA2: 0x005C3FF0, replaces TibSun's DecodePubGameopt; ra2-decomp's text for the Yuri's Revenge twin (DecodePubGameopt 0x005E3D10, src/netshare.cpp) in this tree's names, without the Yuri's Revenge parts (NAT debug line, waypoints, coop start spots, difficulty, teams and starts); the executable decodes every kind of game options string (Z, O, A, C, L, G, K, P and the host's full options) and returns true when the scenario changed
{
	static int _last_game_speed = -1; // RA2: 0x007E6DD8, the previous options are kept in function statics: three ints in the data segment
	static int _last_credits = -1; // RA2: 0x007E6DDC
	static int _last_unit_count = -1; // RA2: 0x007E6DE0
	static bool _last_short_game; // RA2: 0x00A72D53, five flags in the bss
	static bool _last_super_weapons; // RA2: 0x00A72D54
	static bool _last_build_off_ally; // RA2: 0x00A72D55
	static bool _last_mcv_redeploy; // RA2: 0x00A72D56
	static bool _last_goodies; // RA2: 0x00A72D57

	bool do_decode = false;
	char old_ai[32];
	char new_ai[32];

	char * string = strdup(options);
	char * token = string;

	if (*token == 'Z') {
		switch (token[1]) {
			case '1': {
				for (int i = 0; i < 8; i++) {
					if (_stricmp(name, Wide_To_Narrow(Session.Slots[i]->Name)) == 0) {
						char buffer[128];
						strcpy(buffer, &token[3]);
						int state = token[2] - '0';
						Register_Connection_State(name, buffer, state);
					}
				}
				break;
			}

			case '2': {
				int port = atol(&token[2]);
				Store_Player_Port(name, port);
				break;
			}

			case '3':
				if (!Local_User_Is_Channel_Owner()) {
					HWND dialog = WS_Find_Dialog(IDD_WOL_GUEST);
					HWND item = GetDlgItem(dialog, 0x3F2);
					if (item) {
						ShowWindow(item, SW_SHOWNORMAL);
						ShowWindow(GetDlgItem(dialog, 0x468), SW_HIDE);
						SendMessageA(item, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString("TXT_NAT_NO_PREVIEW", NULL, "D:\\Ra2\\netshare.cpp", 1214));
					}
				}
				break;
		}
		free(string);
		return(false);
	}

	if (Session.Type == GAME_INTERNET && (g_PlayingNetGame || WolOptionsLocked != 0)) {
		free(string);
		return(false);
	}
	{
		if (*token == 'O') {
			HWND dialog = WS_Find_Dialog(IDD_WOL_GUEST);
			if (dialog != NULL || (dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST)) != NULL) {
			int old = LobbyObserverSlot;
				LobbyObserverSlot = (token[1] == '8') ? -1 : token[1] - '0';
			if (LobbyObserverSlot != old) {
				Show_Observer_Slot_Controls(dialog);
					wchar_t wide[256];
					wcscpy(wide, Narrow_To_Wide(&token[2]));
					if (wide[0] != 0) {
						NodeNameType * node;
					{
							WideWstring text(wide);
							node = Find_Lobby_Entry_By_Name(text);
					}
						if (node == NULL) {
							node = Find_Lobby_Name(wide);
							if (node == NULL) {
								free(string);
							return(false);
						}
					}
						if (node == LobbyEntries[6]
						|| (node != NULL && node != LobbyEntries[0] && node != LobbyEntries[1]
							&& node != LobbyEntries[5] && node != LobbyEntries[2]
							&& node != LobbyEntries[3] && node != LobbyEntries[4]
							&& node->Slot.SpectatorFlag == -1)) {
							Session.Slots[LobbyObserverSlot] = node;
							int id = Player_Side_Combo_ID(LobbyObserverSlot);
							Fill_Observer_Combo(dialog, id);
						SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(LobbyObserverSlot)), 0x4F1, 0, 1);
							int combo = Player_Color_Combo_ID(LobbyObserverSlot);
							Fill_Random_Color_Combo(dialog, combo);
							Select_Color_Combo(dialog, combo, 8);
					} else {
							int id = Player_Side_Combo_ID(LobbyObserverSlot);
							Fill_Side_Combo(dialog, id);
						SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(LobbyObserverSlot)), 0x4F1, 0, 0);
							int combo = Player_Color_Combo_ID(LobbyObserverSlot);
							Fill_Color_Combo(dialog, combo);
							Select_Color_Combo(dialog, combo, -2);
					}
						Session.Slots[LobbyObserverSlot] = node;
				}
			}
			if (LobbyObserverSlot >= 0 && LobbyObserverSlot != Session.LocalSlot) {
				Update_Slot_Controls(dialog, LobbyObserverSlot, false);
			}
			}
			free(string);
			return(false);
		} else if (*token == 'A') {
			int status = atol(&token[1]);
			if (g_pChat != NULL) {
				SetPlayerAccepted(name, status);
			} else {
				Net2SetAccept(name, status);
			}
			free(string);
			return(false);
		} else if (*token == 'C') {
			if (string[1] == '0') {
				DebugString("@@@@ Decoding cooperative campaign game options %s\n", options);
				int index = -1;
				DynamicVectorClass<int> picks;
				int b = 0;
				int a = 0;
				MPlayerScoreType stats[2];

				token = strtok(string + 2, ",");
				if (token) index = atol(token);
				token = strtok(NULL, ",");
				if (token) a = atol(token);
				token = strtok(NULL, ",");
				if (token) b = atol(token);
				token = strtok(NULL, ",");
				if (token) stats[1].Kills[0] = atol(token);
				token = strtok(NULL, ",");
				if (token) stats[1].Built[0] = atol(token);
				token = strtok(NULL, ",");
				if (token) stats[1].Lost[0] = atol(token);
				token = strtok(NULL, ",");
				if (token) stats[1].Score[0] = atol(token);
				token = strtok(NULL, ",");
				if (token) stats[0].Kills[0] = atol(token);
				token = strtok(NULL, ",");
				if (token) stats[0].Built[0] = atol(token);
				token = strtok(NULL, ",");
				if (token) stats[0].Lost[0] = atol(token);
				token = strtok(NULL, ",");
				if (token) stats[0].Score[0] = atol(token);

				for (int i = 0; i < 10; i++) {
					token = strtok(NULL, ",");
					if (token == NULL) break;
					int pick = atol(token);
					if (pick < 0) break;
					picks.Add(pick);
				}

				char * map = strtok(NULL, ",");
				char * mapname = strtok(NULL, ",");
				for (int m = 0; m <= MultiplayerGameMode::GetModeCount(); m++) {
					if (MultiplayerGameMode::Get(m) != NULL) {
						DebugString("Setting Campaign info for Mode %d: %s\n", m, Wide_To_Narrow(MultiplayerGameMode::Get(m)->UIName.text()));
						if (MultiplayerGameMode::Get(m)->vt_entry_48(index, picks, map, mapname)) {
							MultiplayerGameMode::Get(m)->vt_entry_4C(a, b, stats);
						}
					}
				}
			} else {
				DebugString("@@@@ Decoding cooperative campaign game options %s\n", options);
				for (int m = 0; m <= MultiplayerGameMode::GetModeCount(); m++) {
					if (MultiplayerGameMode::Get(m) != NULL) {
						DebugString("Handling Campaign info for Mode %d: %s\n", m, Wide_To_Narrow(MultiplayerGameMode::Get(m)->UIName.text()));
						MultiplayerGameMode::Get(m)->vt_entry_64(string[1] - '0', &string[2]);
					}
				}
			}
			free(string);
			return(false);
		} else if (*token == 'L') {
			WideWstring list = Wstring_ToWide(options + 1);
			Set_Slot_Names(list.get());
			HWND dialog = WS_Find_Dialog(IDD_WOL_GUEST);
			if (dialog) {
				int slot = Find_First_Player_Slot();
				if (slot == -1) {
					Leave_Channel_With_Message(dialog);
				} else if (slot != Session.LocalSlot) {
					int id = Player_Side_Combo_ID(slot);
					if (Session.Slots[slot]->Slot.SpectatorFlag == -1) {
						Fill_Observer_Combo(dialog, id);
						SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 1);
						int combo = Player_Color_Combo_ID(slot);
						Fill_Random_Color_Combo(dialog, combo);
						Select_Color_Combo(dialog, combo, 8);
					} else {
						id = Player_Side_Combo_ID(slot);
						if (Session.MPGameMode != NULL && !Session.MPGameMode->vt_entry_04()) {
							Fill_Side_Combo(dialog, id);
						}
						SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 0);
						int combo = Player_Color_Combo_ID(slot);
						Fill_Color_Combo(dialog, combo);
						Select_Color_Combo(dialog, combo, -2);
					}
					int me = Find_First_Player_Slot();
					Session.LocalSlot = me;
					Lock_Slots(dialog, me);
					Select_Player_Side(dialog, Session.LocalSlot);
					Select_Player_Color(dialog, Session.LocalSlot);
				}
			}
			free(string);
			return(false);
		} else if (*token == 'G') {
			HWND dialog = WS_Find_Dialog(IDD_WOL_GUEST);
			if (dialog) {
				Handle_Start_Reminder();
			}
			free(string);
			return(false);
		} else if (*token == 'K') {
			if (WS_Find_Dialog(IDD_WOL_GAMEOPT) || WS_Find_Dialog(IDD_MPLAYER_HOST)) {
				int state;
				switch (token[1]) {
					case '1':
						state = 1;
						break;
					case '2':
						state = 2;
						break;
					default:
						state = 0;
						break;
				}
				Set_Player_Map_State(name, state);
			}
			free(string);
			return(false);
		} else if (*token == 'P') {
			DebugString("Got Ping game options string. %s\n", token);
			char * p = &token[3];
			if (Session.Type == GAME_INTERNET) {
				while (*p) {
					char player[32];
					char number[32];
					int i = 0;
					while (*p && *p != ',') {
						char c = *p++;
						player[i++] = c;
					}
					player[i] = 0;
					i = 0;
					if (*p) p++;
					while (*p && *p != ',') {
						char c = *p++;
						number[i++] = c;
					}
					number[i] = 0;
					int ping = atol(number);
					if (*p) p++;
					Store_User_Ping(player, ping);
				}
			}
			free(string);
			return(false);
		}

		int topid = WS_Top_Window_ID();
		if (topid != IDD_MPLAYER_HOST && topid != IDD_MPLAYER_GUEST) {
			SendDlgItemMessageA(GameoptWindow(), IDC_USERS, OD_DISABLEPAINT, 0, 1);
		}
		DebugString("Decoding game options %s\n", options);

		token = strtok(token, ",");
		if (token) Seed = atol(token);
		token = strtok(NULL, ",");
		if (token) Options.GameSpeed = atol(token);
		token = strtok(NULL, ",");
		if (token) Session.Options.GameSpeed = atol(token);
		token = strtok(NULL, ",");
		if (token) Session.Options.Credits = atol(token);
		token = strtok(NULL, ",");
		if (token) Session.Options.UnitCount = atol(token);
		token = strtok(NULL, ",");
		if (token) Session.Options.ShortGame = atol(token) != 0;
		token = strtok(NULL, ",");
		if (token) Session.Options.SWAllowed = atol(token) != 0;
		token = strtok(NULL, ",");
		if (token) Session.Options.BuildOffAlly = atol(token) != 0;
		token = strtok(NULL, ",");
		if (token) Session.Options.MCVRedeploy = atoi(token) != 0;
		token = strtok(NULL, ",");
		if (token) Session.Options.Goodies = atoi(token) != 0;
		token = strtok(NULL, ",");
		if (token) Session.Options.MPModeIndex = atol(token);

		bool same_scenario = true;
		char * scenario_description = strtok(NULL, ",");
		WideWstring description = (Session.Type == GAME_INTERNET && Is_Tournament_Session()) ? WideWstring(L"") : Wide_From_7_Bit(scenario_description);

		token = strtok(NULL, ",");
		int old_max = Session.ScenarioMaxPlayers;
		if (token) {
			Session.ScenarioMaxPlayers = atoi(token);
		}

		token = strtok(NULL, ",");
		bool old_official = Session.ScenarioIsOfficial;
		if (token) {
			Session.ScenarioIsOfficial = atoi(token) != 0;
		}

		token = strtok(NULL, ",");
		int old_length = Session.ScenarioFileLength;
		if (token) {
			Session.ScenarioFileLength = atoi(token);
		}

		token = strtok(NULL, ",");
		if (token) {
			if (_stricmp(Session.ScenarioFileName, token) != 0) {
				same_scenario = false;
			}
			strncpy(Session.ScenarioFileName, token, sizeof(Session.ScenarioFileName));
			strcpy(Scen->ScenarioName, Session.ScenarioFileName);
		}

		char * digest = strtok(NULL, ":");
		if (digest) {
			if (strcmp(Session.ScenarioDigest, digest) != 0) {
				same_scenario = false;
			}
			strncpy(Session.ScenarioDigest, digest, sizeof(Session.ScenarioDigest) - 1);
		}

		MultiplayerGameMode * mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex);
		if (topid == IDD_MPLAYER_GUEST || topid == IDD_WOL_GUEST) {
			Change_Slot_Count(GameoptWindow(), old_max, Session.ScenarioMaxPlayers, mode);
		}

		bool found = true;
		if (!same_scenario || wcslen(Session.Options.ScenarioDescription) == 0) {
			found = false;
			same_scenario = false;
			for (int i = 0; i < Session.Scenarios.Count(); i++) {
				MultiMission * scenario = Session.Scenarios[i];
				if (_stricmp(Session.ScenarioFileName, scenario->Get_Filename()) == 0) {
					CCFileClass file(scenario->Get_Filename());
					if (file.Is_Available(false) && file.Size() == Session.ScenarioFileLength) {
						INIClass ini;
						ini.Load(file, false);
						char scenario_digest[32];
						ini.Get_String("Digest", "1", "No digest here mate. Nope.", scenario_digest, sizeof(scenario_digest));
						if (Session.ScenarioDigest[0] == 0 || strcmp(Session.ScenarioDigest, scenario_digest) == 0) {
							wcscpy(Session.Options.ScenarioDescription, scenario->Description());
							found = true;
						}
					}
				}
			}
			if (!found && description.length() > 0) {
				wcscpy(Session.Options.ScenarioDescription, description.get());
			}
			if (Session.MPGameMode != NULL) {
				Session.MPGameMode->vt_entry_24();
			}
		}

		mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex);
		if (mode != Session.MPGameMode) {
			if (Session.MPGameMode != NULL) {
				Session.MPGameMode->vt_entry_84();
			}
			if (mode != NULL) {
				mode->vt_entry_20();
			}
		}
		Session.MPGameMode = mode;
		if (mode != NULL && !same_scenario) {
			mode->UnfixAlliances();
		}

		HWND dialog = GameoptWindow();
		Set_Item_Text(dialog, mode);
		Set_Skirmish_Dialog_Text(dialog);

		Scen->Scenario = -1;
		Frame = 0;
		Session.CommProtocol = DEFAULT_COMM_PROTOCOL;

		if (old_official != Session.ScenarioIsOfficial || old_length != Session.ScenarioFileLength) {
			same_scenario = false;
		}

		if (!same_scenario) {
			DebugString("Not same scenario...\n");
			if (Session.ScenarioIsOfficial && !found && _stricmp(Session.ScenarioFileName, "RandMap.Sed") != 0) {
				if (GameoptWindow()) {
					Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:JoinerNoMap", NULL, "D:\\Ra2\\netshare.cpp", 1818), Session.Options.ScenarioDescription);
				}
				LobbyDialogFlag = false;
				EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), FALSE);
				InvalidateRect(GetDlgItem(GameoptWindow(), IDC_ACCEPT), NULL, FALSE);
				Send_Host_Option("K0");
			} else {
				LobbyDialogFlag = true;
				EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), TRUE);
				InvalidateRect(GetDlgItem(GameoptWindow(), IDC_ACCEPT), NULL, FALSE);
				if (!Session.ScenarioIsOfficial && !found && _stricmp(Session.ScenarioFileName, "RandMap.Sed") != 0) {
					if (GameoptWindow()) {
						Lobby_Chat_Printf(-1, StringTable::LoadString("GUI:JoinerMapTransfer", NULL, "D:\\Ra2\\netshare.cpp", 1834), Session.Options.ScenarioDescription);
					}
					Send_Host_Option("K2");
				} else {
					Send_Host_Option("K1");
				}
			}
			Update_Network_Dialog_Preview(GameoptWindow());
		}

		if (digest == NULL) {
			Lobby_Chat_Printf(-1, L"Gameopt parse failed");
		}

		unsigned int address = 0;
		token = strtok(NULL, ",:");
		while (token != NULL) {
			char * handle = token;
			if (strcmp(handle, "@") == 0) break;

			token = strtok(NULL, ",:");
			if (token == NULL) break;
			int house = atol(token);

			token = strtok(NULL, ",:");
			if (token == NULL) break;
			int color = atol(token);

			token = strtok(NULL, ",:");
			if (token == NULL) break;
			sscanf(token, "%X", &address);

			if (address != 0 && Session.Type == GAME_INTERNET) {
				int i;
				for (i = 0; i < 8; i++) {
					if (_stricmp(handle, Wide_To_Narrow(Session.Slots[i]->Name)) == 0) {
						unsigned long net = 0;
						NetNodeType node;
						memset(node, 0, sizeof(node));
						*(unsigned long *)node = address;
						Session.SlotAddresses[i].Set_Address((unsigned char *)&net, node, 0);
						token = strtok(NULL, ",:");
						if (token) {
							unsigned int value;
							sscanf(token, "%X", &value);
							Session.WolPlayerState[i] = value;
							token = strtok(NULL, ",:");
							if (token) {
								unsigned int port;
								sscanf(token, "%X", &port);
								Session.PlayerPortBase[i] = port;
							}
						}
						break;
					}
				}
				if (i == 8) {
					strtok(NULL, ",:");
					strtok(NULL, ",:");
				}
			} else {
				strtok(NULL, ",:");
				strtok(NULL, ",:");
			}

			IsColorChangePending = true;
			if (_stricmp(handle, Session.Handle) == 0 && color != Session.Color) {
				if (!IsColorChangePending) {
					Lobby_Chat_Printf(-1, StringTable::LoadString("TXT_COLOR_IN_USE", NULL, "D:\\Ra2\\netshare.cpp", 1954));
				}
				Session.Color = color;
			}
			if (_stricmp(handle, Session.Handle) == 0) {
				IsColorChangePending = false;
			}

			if (g_pChat != NULL && Assign_House_And_Color(handle, house, color) ||
				g_pChat == NULL && Net2SetHouseAndColor(handle, house, color)) {
				do_decode = true;
			}
			address = 0;
			token = strtok(NULL, ",:");
		}

		AI_Slot_String(old_ai);
		int ai_players = 0;
		for (int k = 0; k < 8; k++) {
			token = strtok(NULL, ",:");
			if (token == NULL) break;
			Session.Options.AISlots.Difficulties[k] = atol(token);
			token = strtok(NULL, ",:");
			if (token == NULL) break;
			Session.Options.AISlots.Countries[k] = atol(token);
			token = strtok(NULL, ",:");
			if (token == NULL) break;
			Session.Options.AISlots.Colors[k] = atol(token);
			if (Session.Options.AISlots.Countries[k] != -1) ai_players++;
		}
		Session.Options.AIPlayers = ai_players;
		AI_Slot_String(new_ai);
		if (strcmp(old_ai, new_ai) != 0) {
			do_decode = true;
		}

		free(string);

		if (!(Session.Type == GAME_INTERNET && Is_Tournament_Session())) {
			Fill_Options_Dialog(GameoptWindow(), 0);
		}

		if (_last_game_speed != Session.Options.GameSpeed) do_decode = true;
		if (_last_credits != Session.Options.Credits) do_decode = true;
		if (_last_unit_count != Session.Options.UnitCount) do_decode = true;
		if (_last_short_game != Session.Options.ShortGame) do_decode = true;
		if (_last_super_weapons != Session.Options.SWAllowed) do_decode = true;
		if (_last_build_off_ally != Session.Options.BuildOffAlly) do_decode = true;
		if (_last_mcv_redeploy != Session.Options.MCVRedeploy) do_decode = true;
		if (_last_goodies != Session.Options.Goodies) do_decode = true;

		if (!same_scenario || do_decode) {
			int accept;
			if (g_pChat != NULL) {
				accept = GetPlayerAccepted(NULL);
			} else {
				accept = Net2GetAccept(0);
			}
			Handle_Reset_Ready();
			if (accept == 1) {
				char buffer[64];
				if (g_pChat != NULL) {
					SetPlayerAccepted(NULL, 0);
				} else {
					Net2SetAccept(0, 0);
				}
				sprintf(buffer, "A0");
				Send_Host_Option(buffer);
				if (LobbyDialogFlag) {
					EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), TRUE);
				} else {
					EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), FALSE);
				}
				InvalidateRect(GetDlgItem(GameoptWindow(), IDC_ACCEPT), NULL, FALSE);
			} else {
				if (!IsWindowEnabled(GetDlgItem(GameoptWindow(), IDC_ACCEPT)) && LobbyDialogFlag) {
					EnableWindow(GetDlgItem(GameoptWindow(), IDC_ACCEPT), TRUE);
					InvalidateRect(GetDlgItem(GameoptWindow(), IDC_ACCEPT), NULL, FALSE);
				}
			}
		}

		if (topid != IDD_MPLAYER_HOST && topid != IDD_MPLAYER_GUEST) {
			SendDlgItemMessage(GameoptWindow(), IDC_USERS, OD_DISABLEPAINT, 0, 0);
		}

		if (g_pChat != NULL) {
			Redraw_Player_List();
		} else {
			Net2DisplayUsers();
		}

		_last_game_speed = Session.Options.GameSpeed;
		_last_credits = Session.Options.Credits;
		_last_unit_count = Session.Options.UnitCount;
		_last_short_game = Session.Options.ShortGame;
		_last_super_weapons = Session.Options.SWAllowed;
		_last_build_off_ally = Session.Options.BuildOffAlly;
		_last_mcv_redeploy = Session.Options.MCVRedeploy;
		_last_goodies = Session.Options.Goodies;

		return(!same_scenario);
	}
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
	if (index != LB_ERR) {
		ListBox_SetCurSel(listbox, index);
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
	char buffer[1024];

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
				key = wide;
				bool value = true;
				lbdict.add(key, value);
			}
		}
	} else if (!(style & LBS_NOSEL)) {
		int index = ListBox_GetCurSel(listbox);
		if (index >= 0) {
			wide[0] = '\0';
			LBGetText(listbox, index, wide, buffer);
			key = wide;
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
	char buffer[1024];
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
				key = wide;
				if (lbdict.contains(key)) {
					ListBox_SetSel(listbox, TRUE, i);
				}
			}
		} else if (!(style & LBS_NOSEL)) {
			bool value;
			if (lbdict.removeAny(key, value)) {
				wcscpy(wide, key.get());
				Select_List_Row_Text(listbox, wide);
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
	if (Session.Type == GAME_INTERNET) {
		if (Is_Current_Channel_Tournament() && !Is_Current_Channel_Flag_8()) {
			return(2);
		}
		return(4);
	}
	return(8);
}


int Get_Lobby_Slot_Limit_Plus_One(void) // RA2: 0x005C6550, not in TibSun; the limit with room for one observer
{
	int limit = Get_Lobby_Slot_Limit();
	if (Session.Type == GAME_INTERNET) {
		if (limit >= 4) {
			return(limit);
		}
	} else if (limit >= 8) {
		return(limit);
	}
	return(limit + 1);
}


int RandomMapWaypointCount(int index); // RA2: defined below, after its caller as in the executable

/// <summary>
/// Fetches how many players a scenario takes.
/// </summary>
/// <param name="index">Index into the multiplayer scenario list, -1 for none.</param>
/// <returns>Returns with the number of slots.</returns>
int __fastcall Get_Scenario_Slot_Limit(int index) // RA2: 0x005C65A0, not in TibSun; how many players a scenario takes in this game
{
	if (index == -1) {
		return(8);
	}
	int limit = RandomMapWaypointCount(index);
	if (Session.MPGameMode != NULL) {
		int mode_limit = Session.MPGameMode->vt_entry_80();
		if (mode_limit != -1 && mode_limit < limit) {
			limit = mode_limit;
		}
	}
	if (Session.Type == GAME_INTERNET) {
		if (Is_Current_Channel_Flag_8()) {
			limit &= ~1;
		} else if (Is_Current_Channel_Tournament()) {
			limit = 2;
		}
	}
	return(limit);
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
	return(SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETCURSEL, 0, 0));
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
		MultiMission * mission = (MultiMission *)SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETITEMDATA, row, 0);
		int index = -1; // RA2: the scenario is searched for by hand
		for (int i = 0; i < Session.Scenarios.Count(); i++) {
			if (mission == Session.Scenarios[i]) {
				index = i;
				break;
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
	if (Session.Type == GAME_INTERNET) {
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
	Show_Dialog(ScenarioPick, 107);
	SendMessage(ScenarioPick, OD_SETTOP, 0, 1); // RA2: message 0x4A9
	ShowWindow(ScenarioPick, SW_NORMAL);
	return(WS_Wait_Dialog(ScenarioPick, Scenario_Select_Callback, false, true, true));
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
	if (data == 0) {
		return(-1);
	}
	int found = 0;
	int count = SendDlgItemMessage(dialog, IDC_AILEVEL_SLIDER, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < count; i++) {
		if (SendDlgItemMessage(dialog, IDC_AILEVEL_SLIDER, LB_GETITEMDATA, i, 0) == data) {
			found = i;
		}
	}
	SendDlgItemMessage(dialog, IDC_AILEVEL_SLIDER, LB_SETCURSEL, found, 0);
	SendDlgItemMessage(dialog, IDC_AILEVEL_SLIDER, LB_SETTOPINDEX, found, 0);
	return(found);
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
	int row = SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETCURSEL, 0, 0);
	int index = -1;
	if (row != -1) {
		MultiMission * picked = (MultiMission *)SendDlgItemMessage(ScenarioPick, IDC_AILEVEL_SLIDER, LB_GETITEMDATA, row, 0);
		for (int i = 0; i < Session.Scenarios.Count(); i++) {
			if (picked == Session.Scenarios[i]) {
				index = i;
				break;
			}
		}
	}
	if (index == -1) {
		return(false);
	}
	MultiplayerGameMode * old_mode = Session.MPGameMode; // RA2: the limit is computed as if the new mode were already set
	HWND lobby = ScenarioPick;
	MultiplayerGameMode * mode = NULL;
	int selected = SendDlgItemMessage(ScenarioPick, 1771, LB_GETCURSEL, 0, 0);
	if (selected != -1) {
		mode = (MultiplayerGameMode *)SendDlgItemMessage(lobby, 1771, LB_GETITEMDATA, selected, 0);
	}
	Session.MPGameMode = mode;
	bool skirmish_allowed = mode->IsSkirmishAllowed();
	bool flag = mode->vt_entry_A0();
	int players = RandomMapWaypointCount(index);
	if (Session.MPGameMode != NULL) {
		int limit = Session.MPGameMode->vt_entry_80();
		if (limit != -1 && limit < players) {
			players = limit;
		}
	}
	if (Session.Type == GAME_INTERNET) {
		if (Is_Current_Channel_Flag_8()) {
			players &= ~1;
		} else if (Is_Current_Channel_Tournament()) {
			players = 2;
		}
	}
	Session.MPGameMode = old_mode;
	bool computers = false;
	bool ask;
	switch (Session.Type) {
		case GAME_IPX:
			ask = Update_Lobby_Count(players, skirmish_allowed, flag);
			break;

		case GAME_INTERNET:
			ask = Wol_Update_Lobby_Count(players, skirmish_allowed, flag); // RA2: 0x0075CA30, as above
			break;

		case GAME_SKIRMISH:
			ask = Skirmish_Has_Computer_After(players);
			computers = true;
			break;

		default:
			ask = false;
			break;
	}
	if (ask) {
		wchar_t const * text = StringTable::LoadString(!computers ? "GUI:EjectPlayers" : "GUI:EjectAIPlayers", NULL, "D:\\Ra2\\netshare.cpp", 2655);
		wchar_t const * ok = StringTable::LoadString("GUI:Ok", NULL, "D:\\Ra2\\netshare.cpp", 2656);
		wchar_t const * cancel = StringTable::LoadString("GUI:Cancel", NULL, "D:\\Ra2\\netshare.cpp", 2657);
		if (WWMessageBox(L" ").Process(text, ok, cancel, NULL, false)) {
			return(false);
		}
	}
	if (mode != Session.MPGameMode) {
		if (Session.MPGameMode != NULL) {
			Session.MPGameMode->vt_entry_84();
		}
		if (mode != NULL) {
			Session.MPGameMode = mode;
			Session.Options.MPModeIndex = mode->MPModeIndex;
			Session.Options.ScenarioIndex = index;
			mode->vt_entry_20();
		}
	}
	Session.Options.ScenarioIndex = index;
	if (mode != NULL) {
		mode->vt_entry_24();
	}
	mode->AssignTeamsAtStart();
	mode->UnfixAlliances();
	WS_Destroy_Dialog(dialog, 1);
	HWND setup = WS_Find_Dialog(IDD_WOL_GAMEOPT); // RA2: the options dialog that is up takes the new choice
	if (setup == NULL) {
		setup = WS_Find_Dialog(IDD_WOL_GUEST);
		if (setup == NULL) {
			setup = WS_Find_Dialog(IDD_MPLAYER_HOST);
			if (setup == NULL) {
				setup = WS_Find_Dialog(IDD_MPLAYER_GUEST);
			}
		}
	}
	HWND control = GetDlgItem(setup, 1772);
	SendMessage(control, OD_SETTEXTW, 0, (LPARAM)mode->UIName.text());
	MultiMission * current = Session.Scenarios[Session.Options.ScenarioIndex];
	control = GetDlgItem(setup, IDC_SCENARIONAME);
	SendMessage(control, OD_SETTEXTW, 0, (LPARAM)current); // RA2: the description is the first member of the MultiMission
	mode->vt_entry_50();
	return(true);
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
						sent = Ipx.Send_Global_Message(&packet, 0x2F, 1, &Session.HostAddress, 0, 0);
					} else {
						User * host = Find_Owner_User();
						sent = 0;
						if (host != NULL) {
							sent = Ipx.Send_Global_Message_To_Name(&packet, 0x2F, 1, Narrow_To_Wide((char const *)host->name));
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
					strcpy(name, Session.ScenarioFileName);
					strupr(name);
					DebugString("Checking for file %s in local directory\n", name);
					if (!Session.ScenarioIsOfficial && strstr(name, ".MPR") != NULL) {
						break;
					}
					{
						RawFileClass r(Session.ScenarioFileName);
						if (r.Is_Available()) {
							present = true;
						}
					}
					if (!present) {
						break;
					}
				}
				if (!Session.Is_Random_Map() && MultiplayerMapPreview != NULL) {
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
		if (Session.Type == GAME_IPX) {
			sent = Ipx.Send_Global_Message(&packet, sizeof(packet), 1, &Session.HostAddress, 0, 0);
		} else {
			User * host = Find_Owner_User();
			if (host == NULL) {
				break;
			}
			sent = Ipx.Send_Global_Message_To_Name(&packet, sizeof(packet), 1, Narrow_To_Wide((char const *)host->name));
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
		Ipx.Set_Timing(TIMER_SECOND / 2, -1, 10 * TIMER_SECOND, true);
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

	Ipx.Set_Timing(TIMER_SECOND / 2, -1, 10 * TIMER_SECOND, true);
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
	Session.ScenarioMaxPlayers = players;
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
								Narrow_Text(
									Session.Players[peer]->Name));
						} else {
							DebugString(
								"Player %s has a NAT - no preview download possible\n",
								Narrow_Text(
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
				TIMER_SECOND / 2, -1, 10 * TIMER_SECOND, true);
		}
	}
}


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
	int size = 320;
	wchar_t description[sizeof(RandomMapGen.SeedData.MapDescription) / sizeof(wchar_t)]; // RA2: the description is wide
	memcpy(description, RandomMapGen.SeedData.MapDescription, sizeof(description));
	memset(RandomMapGen.SeedData.MapDescription, '\0', sizeof(description));

	do { // RA2: the loop tests at the end
		unsigned int hibit = checksum >> 31;
		if (size >= 4) {
			checksum = (checksum << 1) + *data++ + hibit;
			size -= sizeof(unsigned int);
		} else {
			unsigned int val = 0;
			unsigned char const * bytes = (unsigned char const *)data;
			while (size != 0) {
				val = (val << 8) | *bytes++;
				size--;
			}
			checksum = (checksum << 1) + val + hibit;
		}
	} while (size > 0);

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
		callback = Net2Callback;
	} else if (Session.Type == GAME_INTERNET) {
		callback = WOL_Wait_Callback;
	} else if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
		callback = ModemHostCallback;
	} else {
		callback = MapGen_Call_Back;
	}
	int result = Do_Random_Map_Dialog(callback);

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
	return(wcscmp(name, Session.GameName) == 0);
}

namespace UI {
bool __fastcall Is_Host_Player(int player) // RA2: 0x005C8680, not in TibSun; the player of the list is the host
{
	return(wcscmp(Session.Players[player]->Name, Session.GameName) == 0);
}
}

bool __fastcall Is_Host(void) // RA2: 0x005C86A0, not in TibSun; the local handle is the game's name
{
	wchar_t * handle = Narrow_To_Wide(Session.Handle); // RA2: the handle is widened before the game's name is pushed
	return(wcscmp(handle, Session.GameName) == 0);
}

bool __fastcall Is_Local_Name(wchar_t const * name) // RA2: 0x005C86C0, not in TibSun; the name is the local handle
{
	return(wcscmp(name, Narrow_To_Wide(Session.Handle)) == 0);
}

void __fastcall Init_Lobby_Entries(void) // RA2: 0x005C86E0, not in TibSun; the fixed entries of the slot lists with their names
{
	if (LobbyEntryTexts[0] == NULL) {
		LobbyEntryTexts[0] = StringTable::LoadString("GUI:Open", NULL, "D:\\Ra2\\netshare.cpp", 3736);
		LobbyEntryTexts[1] = StringTable::LoadString("GUI:Closed", NULL, "D:\\Ra2\\netshare.cpp", 3737);
		LobbyEntryTexts[2] = StringTable::LoadString("GUI:AIEasy", NULL, "D:\\Ra2\\netshare.cpp", 3738);
		LobbyEntryTexts[3] = StringTable::LoadString("GUI:AINormal", NULL, "D:\\Ra2\\netshare.cpp", 3739);
		LobbyEntryTexts[4] = StringTable::LoadString("GUI:AIHard", NULL, "D:\\Ra2\\netshare.cpp", 3740);
		LobbyEntryTexts[5] = StringTable::LoadString("GUI:Waiting", NULL, "D:\\Ra2\\netshare.cpp", 3741);
		LobbyEntryTexts[6] = StringTable::LoadString("GUI:OpenObserver", NULL, "D:\\Ra2\\netshare.cpp", 3742);
	}
	if (LobbyEntries[0] == NULL) {
		LobbyEntries[0] = new NodeNameType;
		wcscpy(LobbyEntries[0]->Name, LobbyEntryTexts[0]);
	}
	if (LobbyEntries[1] == NULL) {
		LobbyEntries[1] = new NodeNameType;
		wcscpy(LobbyEntries[1]->Name, LobbyEntryTexts[1]);
	}
	if (LobbyEntries[2] == NULL) {
		LobbyEntries[2] = new NodeNameType;
		wcscpy(LobbyEntries[2]->Name, LobbyEntryTexts[2]);
	}
	if (LobbyEntries[3] == NULL) {
		LobbyEntries[3] = new NodeNameType;
		wcscpy(LobbyEntries[3]->Name, LobbyEntryTexts[3]);
	}
	if (LobbyEntries[4] == NULL) {
		LobbyEntries[4] = new NodeNameType;
		wcscpy(LobbyEntries[4]->Name, LobbyEntryTexts[4]);
	}
	if (LobbyEntries[5] == NULL) {
		LobbyEntries[5] = new NodeNameType;
		wcscpy(LobbyEntries[5]->Name, LobbyEntryTexts[5]);
	}
	if (LobbyEntries[6] == NULL) {
		LobbyEntries[6] = new NodeNameType;
		wcscpy(LobbyEntries[6]->Name, LobbyEntryTexts[6]);
	}
}

void __fastcall Free_Lobby_Entries(void) // RA2: 0x005C8950, not in TibSun; the fixed entries are freed and the player list emptied
{
	if (LobbyEntries[0] != NULL) {
		delete LobbyEntries[0];
		LobbyEntries[0] = NULL;
	}
	if (LobbyEntries[1] != NULL) {
		delete LobbyEntries[1];
		LobbyEntries[1] = NULL;
	}
	if (LobbyEntries[2] != NULL) {
		delete LobbyEntries[2];
		LobbyEntries[2] = NULL;
	}
	if (LobbyEntries[3] != NULL) {
		delete LobbyEntries[3];
		LobbyEntries[3] = NULL;
	}
	if (LobbyEntries[4] != NULL) {
		delete LobbyEntries[4];
		LobbyEntries[4] = NULL;
	}
	if (LobbyEntries[5] != NULL) {
		delete LobbyEntries[5];
		LobbyEntries[5] = NULL;
	}
	if (LobbyEntries[6] != NULL) {
		delete LobbyEntries[6];
		LobbyEntries[6] = NULL;
	}
	Clear_Vector(&Session.Players);
}

void __fastcall Fill_Slots(void) // RA2: 0x005C8A10, not in TibSun; the slots up to the limit are open, the others closed
{
	int limit = Get_Lobby_Slot_Limit();
	for (int i = 0; i < limit; i++) {
		Session.Slots[i] = LobbyEntries[0];
	}
	for (int j = limit; j < 8; j++) {
		Session.Slots[j] = LobbyEntries[1];
	}
}

void __fastcall Clear_Slot_Lists(void) // RA2: 0x005C8A70, not in TibSun; the lists Refresh_Slot_Settings compares are emptied
{
	SlotColorsOld[0] = '\0';
	SlotColors[0] = '\0';
	SlotCountriesOld[0] = '\0';
	SlotCountries[0] = '\0';
}

void __fastcall Reset_Slots(void) // RA2: 0x005C8A90, not in TibSun; the slots are open or closed again, nobody is seated, the computer players are cleared
{
	int limit = Get_Lobby_Slot_Limit();
	for (int i = 0; i < limit; i++) {
		Session.Slots[i] = LobbyEntries[0];
	}
	for (int j = limit; j < 8; j++) {
		Session.Slots[j] = LobbyEntries[1];
	}
	Session.LocalSlot = -1;
	Session.Options.Reset_AI();
	SlotNamesOld[0] = L'\0';
	SlotNames[0] = L'\0';
}

NodeNameType * __fastcall Find_Lobby_Name(wchar_t const * name) // RA2: 0x005C8B20, not in TibSun; the fixed entry of this name, or the player of the list
{
	if (wcscmp(LobbyEntries[0]->Name, name) == 0) {
		return(LobbyEntries[0]);
	}
	if (wcscmp(LobbyEntries[1]->Name, name) == 0) {
		return(LobbyEntries[1]);
	}
	if (wcscmp(LobbyEntries[2]->Name, name) == 0) {
		return(LobbyEntries[2]);
	}
	if (wcscmp(LobbyEntries[3]->Name, name) == 0) {
		return(LobbyEntries[3]);
	}
	if (wcscmp(LobbyEntries[4]->Name, name) == 0) {
		return(LobbyEntries[4]);
	}
	if (wcscmp(LobbyEntries[5]->Name, name) == 0) {
		return(LobbyEntries[5]);
	}
	if (wcscmp(LobbyEntries[6]->Name, name) == 0) {
		return(LobbyEntries[6]);
	}
	int count = Session.Players.Count();
	for (int i = 0; i < count; i++) {
		NodeNameType * node = Session.Players[i];
		if (wcscmp(node->Name, name) == 0) {
			return(node);
		}
	}
	return(NULL);
}

/// <summary>
/// Finds the lobby slot that holds a player.
/// </summary>
/// <param name="name">The player's name.</param>
/// <returns>The slot, or -1 if no slot holds the name.</returns>
int __fastcall Find_Slot_By_Name(wchar_t const * name) // RA2: 0x005C8C30, the slots are compared by name; the executable has no Session member for the search, and the callers before it expand it inline
{
	for (int i = 0; i < 8; i++) {
		if (Session.Slots[i] != NULL && wcsicmp(name, Session.Slots[i]->Name) == 0) {
			return(i);
		}
	}
	return(-1);
}

int Find_First_Player_Slot(void) // RA2: 0x005C8C70, the slot of the first player in the list, or -1 without players
{
	if (Session.Players.Count() == 0) {
		return(-1);
	}
	return(Find_Slot_By_Name(Session.Players[0]->Name));
}

void __fastcall Set_Player_Slot(int slot, NodeNameType * node) // RA2: 0x005C8CC0, not in TibSun; seats an entry in a slot
{
	Session.Slots[slot] = node;
}

void __fastcall Seat_First_Player(int slot) // RA2: 0x005C8CD0, not in TibSun; the first player of the list takes the slot and the local slot is told
{
	Session.LocalSlot = slot;
	Session.Slots[slot] = Session.Players[0];
}

void __fastcall Get_Slot_Names(wchar_t * list) // RA2: 0x005C8CF0, not in TibSun; the names of the eight slots, each followed by a comma
{
	*list = L'\0';
	for (int i = 0; i < 8; i++) {
		NodeNameType * entry = Session.Slots[i];
		WideWstring name;
		if (entry == NULL || entry == LobbyEntries[0] || entry == LobbyEntries[1] // RA2: the test of Is_Real_Player_Entry is written out, the executable does not call it here
			|| entry == LobbyEntries[5] || entry == LobbyEntries[6]
			|| entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]) {
			Wstring text = Lobby_Entry_Name(entry);
			name = Wstring_ToWide(text);
		} else {
			name = entry->Name;
		}
		wcscat(list, name.get());
		wcscat(list, L",");
	}
}

#ifdef RA2_NON_MATCHING // not exact: 4 bytes differ (adopted text); exact with the twin's order of the four locals (open ahead of closed) once the seven lobby entries are separate globals, the twin's form (lane F59, held: OnPrivateGameOptions then turns by one byte)
void __fastcall Set_Slot_Names(wchar_t const * list) // RA2: 0x005C8DD0, not in TibSun; takes the slot names the host sent, seats the entries and sets the observer's combo boxes
{
	if (*list == L'\0') {
		return;
	}
	wchar_t buffer[170];
	wcscpy(buffer, list);
	bool observer[8]; // RA2: the slots that held an observer before
	NodeNameType * observer_entry = LobbyEntries[6]; // RA2: the entries are read once into locals
	NodeNameType * waiting = LobbyEntries[5];
	NodeNameType * closed = LobbyEntries[1];
	NodeNameType * open = LobbyEntries[0];
	for (int i = 0; i < 8; i++) {
		NodeNameType * entry = Session.Slots[i];
		observer[i] = entry == observer_entry
			|| (entry != NULL && entry != open && entry != closed && entry != waiting && entry->Slot.SpectatorFlag == -1);
	}
	Fill_Slots();
	int slot = 0;
	for (wchar_t * name = wcstok(buffer, L","); name != NULL; name = wcstok(NULL, L",")) {
		NodeNameType * entry;
		{
			WideWstring text(name);
			entry = Find_Lobby_Entry_By_Name(text);
		}
		if (entry == NULL) {
			entry = Find_Lobby_Name(name);
			if (entry == NULL) {
				entry = LobbyEntries[5];
			}
		}
		if (entry == LobbyEntries[6]
			|| (entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1]
				&& entry != LobbyEntries[5] && entry != LobbyEntries[2]
				&& entry != LobbyEntries[3] && entry != LobbyEntries[4]
				&& entry->Slot.SpectatorFlag == -1)) {
			HWND dialog = GameoptWindow();
			if (!observer[slot]) {
				Set_Player_Slot(slot, entry); // RA2: the seat is set through Set_Player_Slot (0x005C8CC0), expanded inline
				Fill_Observer_Combo(dialog, Player_Side_Combo_ID(slot));
				SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 1);
				int id = Player_Color_Combo_ID(slot);
				Fill_Random_Color_Combo(dialog, id);
				Select_Color_Combo(dialog, id, 8);
			}
		} else {
			HWND dialog = GameoptWindow();
			if (observer[slot]) {
				int id = Player_Side_Combo_ID(slot);
				if (Side_Combo_Data(dialog, id, -1) == -3) {
					Fill_Side_Combo(dialog, id);
					SendMessageA(GetDlgItem(dialog, Get_Slot_Control_Id(slot)), 0x4F1, 0, 0);
					id = Player_Color_Combo_ID(slot);
					Fill_Color_Combo(dialog, id);
					Select_Color_Combo(dialog, id, -2);
				}
			}
		}
		Set_Player_Slot(slot, entry);
		slot++;
	}
}
#endif

int __fastcall Assign_Slot(NodeNameType * entry) // RA2: 0x005C9170, not in TibSun; seats a joining entry in an open slot, or an observer in an observer slot, and gives the slot
{
	for (int i = 0; i < 8; i++) {
		if ((Session.Slots[i] == LobbyEntries[0] && entry->Slot.SpectatorFlag != -1)
			|| (Session.Slots[i] == LobbyEntries[6] && entry->Slot.SpectatorFlag == -1)) {
			Session.Slots[i] = entry;
			return(i);
		}
	}
	return(-1);
}

void __fastcall Release_Slot_By_Address(IPXAddressClass & address) // RA2: 0x005C91C0, not in TibSun; the address is passed on to IPXAddressClass::operator== (0x005235D0), a non-const reference; frees the slot of the player at an address (an observer's slot reopens for observers)
{
	int limit = 4; // RA2: the limit of Get_Lobby_Slot_Limit written out, the executable compares the game type with the same constant 4
	if (Session.Type == GAME_INTERNET) {
		if (Is_Current_Channel_Tournament() && !Is_Current_Channel_Flag_8()) {
			limit = 2;
		}
	} else {
		limit = 8;
	}
	if (LobbyObserverSlot == 0 && limit < 7) {
		limit++;
	}
	for (int i = 0; i < 8; i++) {
		NodeNameType * node = Session.Slots[i];
		if (node->Address == address) {
			if (Session.Slots[i]->Slot.SpectatorFlag == -1) {
				Session.Slots[i] = LobbyEntries[6];
				return;
			}
			NodeNameType * entry = LobbyEntries[0];
			if (i >= limit) {
				entry = LobbyEntries[1];
			}
			Session.Slots[i] = entry;
			return;
		}
	}
}

void __fastcall Release_Slot_By_Name(char const * name) // RA2: 0x005C9270, not in TibSun; frees the slot of the player of this name, as above
{
	int limit = Get_Lobby_Slot_Limit();
	if (LobbyObserverSlot == 0 && limit < 7) {
		limit++;
	}
	for (int i = 0; i < 8; i++) {
		NodeNameType * node = Session.Slots[i];
		if (node != NULL && _stricmp(name, Wide_To_Narrow(node->Name)) == 0) {
			if (Session.Slots[i]->Slot.SpectatorFlag == -1) {
				Session.Slots[i] = LobbyEntries[6];
				return;
			}
			NodeNameType * entry = LobbyEntries[0];
			if (i >= limit) {
				entry = LobbyEntries[1];
			}
			Session.Slots[i] = entry;
			return;
		}
	}
}

bool __fastcall Is_Real_Player_Entry(NodeNameType * entry) // RA2: 0x005C9320, not in TibSun; the entry is a player: not open, closed, waiting, observer or a computer player; the value is a plain expression, the executable returns it in eax
{
	return(entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[5] && entry != LobbyEntries[6] && entry != LobbyEntries[2] && entry != LobbyEntries[3] && entry != LobbyEntries[4]);
}

bool __fastcall Is_AI_Entry(NodeNameType * entry) // RA2: 0x005C9370, not in TibSun; the entry is a computer player, as above
{
	return(entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]));
}

bool __fastcall Is_Seated_Entry(NodeNameType * entry) // RA2: 0x005C93A0, not in TibSun; the entry is a player or a computer player: not open, closed, waiting or observer, as above
{
	return(entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[6] && entry != LobbyEntries[5]);
}

int __fastcall Get_AI_Difficulty(NodeNameType * entry) // RA2: 0x005C93D0, not in TibSun; 2 for the easy, 1 for the medium and 0 for the hard computer player, -1 for any other entry
{
	int difficulty = -1;
	if (entry == LobbyEntries[2]) {
		return(2);
	}
	if (entry == LobbyEntries[3]) {
		return(1);
	}
	if (entry == LobbyEntries[4]) {
		difficulty = 0;
	}
	return(difficulty);
}

int Count_AI_Slots(void) // RA2: 0x005C9400, not in TibSun; the slots holding an easy, medium or hard computer player
{
	NodeNameType * easy = LobbyEntries[2]; // RA2: the entries are read once into locals
	NodeNameType * medium = LobbyEntries[3];
	NodeNameType * hard = LobbyEntries[4];
	int count = 0;
	for (int i = 0; i < 8; i++) {
		NodeNameType * entry = Session.Slots[i];
		if (entry != NULL && (entry == easy || entry == medium || entry == hard)) {
			count++;
		}
	}
	return(count);
}

bool __fastcall Is_All_Slots_Filled(void) // RA2: 0x005C9440, not in TibSun; no slot up to the player limit is open (the observer's slot counts when it is the first)
{
	int count = Session.MaxPlayers < Session.ScenarioMaxPlayers ? Session.MaxPlayers : Session.ScenarioMaxPlayers;
	for (int i = 0; i < count; i++) {
		if (Session.Slots[i] == LobbyEntries[0]) {
			return(false);
		}
	}
	if (LobbyObserverSlot == 0 && count < 8 && Session.Slots[count] == LobbyEntries[0]) {
		return(false);
	}
	return(true);
}

bool __fastcall Is_Observer_Slot_Full(void) // RA2: 0x005C9490, not in TibSun; no slot is left for an observer
{
	for (int i = 0; i < 8; i++) {
		if (Session.Slots[i] == LobbyEntries[6]) {
			return(false);
		}
	}
	return(true);
}

bool __fastcall Is_Observer_Slot_Occupied(void) // RA2: 0x005C94B0, not in TibSun; the observer slot holds a seated entry whose spectator flag is set (ra2-decomp's YR twin text, 0x5E9580)
{
	int index = LobbyObserverSlot;
	NodeNameType * open = LobbyEntries[0];
	NodeNameType * closed = LobbyEntries[1];

	for (int i = 0; i < 8; i++) {
		NodeNameType * slot = Session.Slots[i];
		if (slot && slot != open && slot != closed && slot != LobbyEntries[6] && slot != LobbyEntries[5] && i == index && slot->Slot.SpectatorFlag == -1) {
			return(true);
		}
	}
	return(false);
}

bool Is_Channel_Full(void) // RA2: 0x005C9510, not in TibSun; more players than the channel takes, the observer not counted
{
	int limit = Session.MaxPlayers < Session.ScenarioMaxPlayers ? Session.MaxPlayers : Session.ScenarioMaxPlayers;
	if (Session.Players.Count() - Is_Observer_Slot_Occupied() > limit) {
		DebugString("Channel full - Session.ScenarioMaxPlayers = %d\n", Session.ScenarioMaxPlayers);
		return(true);
	}
	return(false);
}

bool __fastcall Is_Color_Taken(int color) // RA2: 0x005C95D0, not in TibSun; a color is taken by a seated player or by a computer player's choice
{
	if (color != -2) {
		int i;
		for (i = 0; i < 8; i++) {
			NodeNameType * entry = Session.Slots[i];
			if (Is_Real_Player_Entry(entry) && entry->Get_Color() == color) { // RA2: a player, not a computer player (the test of Is_Real_Player_Entry expanded)
				return(true);
			}
		}
		for (i = 0; i < 8; i++) {
			if (Session.Options.AISlots.Colors[i] == color) {
				return(true);
			}
		}
	}
	return(false);
}

int __fastcall Get_Entry_Kind(NodeNameType * entry) // RA2: 0x005C9650, not in TibSun; the type of a fixed entry: 2 open, 3 closed, 4 easy, 5 medium, 6 hard computer, 7 observer (other entries count as open)
{
	if (entry == LobbyEntries[0]) {
		return(2);
	}
	if (entry == LobbyEntries[1]) {
		return(3);
	}
	if (entry == LobbyEntries[2]) {
		return(4);
	}
	if (entry == LobbyEntries[3]) {
		return(5);
	}
	if (entry == LobbyEntries[4]) {
		return(6);
	}
	return(entry == LobbyEntries[6] ? 7 : 2);
}

NodeNameType * __fastcall Get_Kind_Entry(int kind) // RA2: 0x005C96B0, not in TibSun; the fixed entry of a type (see Get_Entry_Kind), NULL for any other
{
	if (kind == 2) {
		return(LobbyEntries[0]);
	}
	if (kind == 3) {
		return(LobbyEntries[1]);
	}
	if (kind == 4) {
		return(LobbyEntries[2]);
	}
	if (kind == 5) {
		return(LobbyEntries[3]);
	}
	if (kind == 6) {
		return(LobbyEntries[4]);
	}
	return(kind == 7 ? LobbyEntries[6] : NULL);
}

void __fastcall Enable_Start_Button(void) // RA2: 0x005C9700, not in TibSun; the start button is enabled and every player but the host loses the accept status
{
	if (WS_Top_Window_ID() != IDD_MPLAYER_HOST) {
		HWND dialog = GameoptWindow();
		EnableWindow(GetDlgItem(dialog, IDC_ACCEPT), TRUE);
	}
	int count = Session.Players.Count();
	for (int i = 0; i < count; i++) {
		if (wcscmp(Session.Players[i]->Name, Session.GameName) != 0 && Session.Players[i]->Player.Status != 0) {
			Session.Players[i]->Player.Status = 0;
		}
	}
}

int __fastcall Get_Slot_Control_Id(int slot) // RA2: 0x005C97A0, the id of the control of a slot (1041 to 1048, not in TibSun's resource list), or -1; the callers before it expand it inline
{
	if (slot == 0) {
		return(1041);
	}
	if (slot == 1) {
		return(1042);
	}
	if (slot == 2) {
		return(1043);
	}
	if (slot == 3) {
		return(1044);
	}
	if (slot == 4) {
		return(1045);
	}
	if (slot == 5) {
		return(1046);
	}
	if (slot == 6) {
		return(1047);
	}
	if (slot == 7) {
		return(1048);
	}
	return(-1);
}

int __fastcall Control_Slot(int id) // RA2: 0x005C9800, not in TibSun; the slot a control belongs to, or -1
{
	if (id == 1041) {
		return(0);
	}
	if (id == 1042) {
		return(1);
	}
	if (id == 1043) {
		return(2);
	}
	if (id == 1044) {
		return(3);
	}
	if (id == 1045) {
		return(4);
	}
	if (id == 1046) {
		return(5);
	}
	if (id == 1047) {
		return(6);
	}
	return(id == 1048 ? 7 : -1);
}

wchar_t const * __fastcall Entry_Type_Name(NodeNameType * entry) // RA2: 0x005C9870, not in TibSun; the tool tip text of an entry's type
{
	if (entry == LobbyEntries[0]) {
		return(StringTable::LoadString("STT:PlayerOpen", NULL, "D:\\Ra2\\netshare.cpp", 4538));
	}
	if (entry == LobbyEntries[1]) {
		return(StringTable::LoadString("STT:PlayerClosed", NULL, "D:\\Ra2\\netshare.cpp", 4540));
	}
	if (entry == LobbyEntries[2]) {
		return(StringTable::LoadString("STT:PlayerDumbAI", NULL, "D:\\Ra2\\netshare.cpp", 4542));
	}
	if (entry == LobbyEntries[3]) {
		return(StringTable::LoadString("STT:PlayerSmartAI", NULL, "D:\\Ra2\\netshare.cpp", 4544));
	}
	if (entry == LobbyEntries[4]) {
		return(StringTable::LoadString("STT:PlayerGeniusAI", NULL, "D:\\Ra2\\netshare.cpp", 4546));
	}
	if (entry == LobbyEntries[6]) {
		return(StringTable::LoadString("STT:PlayerOpenObserver", NULL, "D:\\Ra2\\netshare.cpp", 4548));
	}
	return(StringTable::LoadString("STT:PlayerHuman", NULL, "D:\\Ra2\\netshare.cpp", 4550));
}

void __fastcall Select_Combo_By_Data(HWND dialog, int id, int data) // RA2: 0x005C9950, not in TibSun; selects the item of a combo box that carries this data, with the box hidden while it is searched
{
	HWND combo = GetDlgItem(dialog, id);
	ShowWindow(combo, SW_HIDE);
	int count = SendDlgItemMessageA(dialog, id, CB_GETCOUNT, 0, 0);
	for (int index = 0; index < count; index++) {
		if (SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0) == data) {
			SendDlgItemMessageA(dialog, id, CB_SETCURSEL, index, 0);
			break;
		}
	}
	ShowWindow(combo, SW_SHOW);
}

int __fastcall Combo_Selected_Data(HWND dialog, int id, int index) // RA2: 0x005C99D0, not in TibSun; the data of an item of a combo box (the selected one for -1), 0 when it has none
{
	if (index == -1) {
		index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
	}
	int data = SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0);
	return(data == -1 ? 0 : data);
}

void __fastcall Set_Slot_Country(HWND dialog, int id) // RA2: 0x005C9A10, not in TibSun; a side combo box was changed: the local player takes the side, the choice of a computer player is kept, other slots forget theirs
{
	int country = Side_Combo_Data(dialog, id, -1);
	int slot = Side_Combo_Player(id);
	if (slot == Session.LocalSlot) {
		Session.Set_Side(country, false);
		if (Session.Players.Count() != 0) {
			Session.Players[0]->Set_Country(country);
		}
	} else {
		NodeNameType * entry = Session.Slots[slot];
		if (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])) {
			Session.Options.AISlots.Countries[slot] = country;
		} else {
			Session.Options.AISlots.Countries[slot] = -1;
			Session.Options.AISlots.Difficulties[slot] = -1;
			Session.Options.AISlots.Colors[slot] = -1;
		}
	}
}

void __fastcall Set_Slot_Color(HWND dialog, int id) // RA2: 0x005C9AB0, not in TibSun; a color combo box was changed: the local player takes the color unless it is taken, other slots keep theirs
{
	int color = Color_Combo_Data(dialog, id, -1);
	int slot = Color_Combo_Player(id);
	if (slot == Session.LocalSlot) {
		Color_Combo_Changed(dialog, id);
		if (Is_Color_Taken(color)) {
			color = -2;
		}
		Session.Set_Color(color, false);
		if (Session.Players.Count() != 0) {
			Session.Players[0]->Set_Color(color);
		}
	} else {
		Session.Options.AISlots.Colors[slot] = color;
	}
}

void __fastcall Update_Slot_Controls(HWND dialog, int slot, bool redraw) // RA2: 0x005C9B90, not in TibSun; shows, enables and fills the side and color boxes of a slot, the observer's slot keeps them closed
{
	int side_id = Player_Side_Combo_ID(slot);
	int color_id = Player_Color_Combo_ID(slot);
	if (LobbyObserverSlot >= 0 && slot == LobbyObserverSlot
		&& (Session.Slots[slot] == LobbyEntries[6] || slot == 0 || Session.Slots[slot]->Slot.SpectatorFlag == -1)) {
		Select_Side_Combo(dialog, side_id, -3);
		Select_Color_Combo(dialog, color_id, -2);
		HWND side = GetDlgItem(dialog, side_id);
		EnableWindow(side, FALSE);
		HWND control = GetDlgItem(dialog, color_id);
		EnableWindow(control, FALSE);
		ShowWindow(control, SW_HIDE);
		return;
	}
	if (!redraw) {
		Select_Side_Combo(dialog, side_id, -2);
		Select_Color_Combo(dialog, color_id, -2);
	}
	HWND side = GetDlgItem(dialog, side_id);
	EnableWindow(side, redraw);
	HWND control = GetDlgItem(dialog, color_id);
	EnableWindow(control, redraw);
	int limit = Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex);
	if (LobbyObserverSlot == 0 && limit < 7) {
		limit++;
	}
	if (slot < limit || slot == LobbyObserverSlot) {
		ShowWindow(control, SW_SHOW);
	}
}

void __fastcall Unlock_Slots(HWND dialog, int slot) // RA2: 0x005C9CF0, not in TibSun; every slot's control is enabled again, the slot's boxes are shown as for Lock_Slots
{
	for (int i = 0; i < 8; i++) {
		HWND control = GetDlgItem(dialog, Get_Slot_Control_Id(i));
		EnableWindow(control, TRUE);
		Update_Slot_Controls(dialog, i, false);
	}
	Update_Slot_Controls(dialog, slot, true);
}

void __fastcall Lock_Slots(HWND dialog, int slot) // RA2: 0x005C9DA0, not in TibSun; every slot's control is disabled but the one of this slot
{
	for (int i = 0; i < 8; i++) {
		HWND control = GetDlgItem(dialog, Get_Slot_Control_Id(i));
		EnableWindow(control, FALSE);
		Update_Slot_Controls(dialog, i, false);
	}
	HWND control = GetDlgItem(dialog, Get_Slot_Control_Id(slot));
	EnableWindow(control, TRUE);
	Update_Slot_Controls(dialog, slot, true);
}

char const * LobbyHostImage = "wolhost.pcx"; // RA2: 0x007E6DB8, the host's picture of the slot list (a variable holding the file name, not a literal at the call)
char const * LobbyAcceptImage = "wolacpt.pcx"; // RA2: 0x007E6DBC, the picture of a player who accepted

void __fastcall Refresh_Slot_Images(HWND dialog) // RA2: 0x005C9EC0, not in TibSun; shows the host's or accepted picture at every slot's control
{
	for (int i = 0; i < 8; i++) {
		HWND control = GetDlgItem(dialog, Slot_Color_Control_Id(i));
		Surface * image = NULL;
		if (Is_Real_Player_Entry(Session.Slots[i]) && Is_Host_Name(Session.Slots[i]->Name)) {
			image = SurfaceCache.GetSurface(LobbyHostImage, NULL);
		} else if (Is_AI_Entry(Session.Slots[i]) || Session.Slots[i]->Player.Status == 1) {
			image = SurfaceCache.GetSurface(LobbyAcceptImage, NULL);
		}
		OwnerDraw::Set_Control_Image(control, image);
	}
}

inline void Refresh_Country_Flags(HWND dialog) // RA2: not in TibSun; the flag picture at each player's control; the executable expands it twice in Refresh_Slot_Settings
{
	for (int i = 0; i < 8; i++) {
		HWND control = GetDlgItem(dialog, Player_Control_ID(i));
		Surface * flag = Country_Flag(Slot_Country(i));
		OwnerDraw::Set_Control_Image(control, flag);
	}
}

inline void Refresh_Side_Combos(HWND dialog) // RA2: not in TibSun; selects each slot's side in its side box (expanded twice in Refresh_Slot_Settings)
{
	for (int i = 0; i < 8; i++) {
		int id = Player_Side_Combo_ID(i);
		Select_Side_Combo(dialog, id, Slot_Country(i));
	}
}

inline void Refresh_Slot_Image(HWND dialog, int slot) // RA2: not in TibSun; the host's or accepted picture at one slot's control
{
	HWND control = GetDlgItem(dialog, Slot_Color_Control_Id(slot));
	Surface * image = NULL;
	if (Is_Real_Player_Entry(Session.Slots[slot]) && Is_Host_Name(Session.Slots[slot]->Name)) {
		image = SurfaceCache.GetSurface(LobbyHostImage, NULL);
	} else if (Is_Slot_Accepted(slot) == 1) {
		image = SurfaceCache.GetSurface(LobbyAcceptImage, NULL);
	}
	OwnerDraw::Set_Control_Image(control, image);
}

inline bool Country_List_Changed(void) // RA2: not in TibSun; rebuilds the country list text and tells whether it differs from the last one
{
	strcpy(SlotCountriesOld, SlotCountries);
	SlotCountries[0] = '\0';
	for (int i = 0; i < 8; i++) {
		char text[4]; // RA2: the executable's frame holds only four bytes for the text (push ecx), enough for "-2,"
		sprintf(text, "%d,", Slot_Country(i));
		strcat(SlotCountries, text);
	}
	return strcmp(SlotCountriesOld, SlotCountries) != 0;
}

inline bool Color_List_Changed(void) // RA2: not in TibSun; rebuilds the color list text and tells whether it differs from the last one
{
	strcpy(SlotColorsOld, SlotColors);
	SlotColors[0] = '\0';
	for (int i = 0; i < 8; i++) {
		char text[4]; // RA2: the executable's frame holds only four bytes for the text (push ecx), enough for "-2,"
		sprintf(text, "%d,", Slot_Color(i));
		strcat(SlotColors, text);
	}
	return strcmp(SlotColorsOld, SlotColors) != 0;
}

inline bool Slot_Names_Changed(void) // RA2: not in TibSun; reads the slot names again and tells whether they differ from the last ones
{
	wcscpy(SlotNamesOld, SlotNames);
	Get_Slot_Names(SlotNames);
	return wcscmp(SlotNamesOld, SlotNames) != 0;
}

void __fastcall Refresh_Slot_Settings(HWND dialog, bool full) // RA2: 0x005CA000, not in TibSun; refreshes the flags, side and color boxes and the slot pictures; when only the slot lists changed the boxes are left alone; written with the inline helpers above (the executable expands them, which keeps the callees Slot_Country, Slot_Color, Slot_Color_Control_Id and Is_Slot_Accepted out of line)
{
	if (full) {
		Refresh_Slot_Combos(dialog);
		Refresh_Country_Flags(dialog);
		Refresh_Side_Combos(dialog);
		Refresh_Slot_Colors(dialog);
		if (Session.Type == GAME_INTERNET) {
			Store_Slot_Pings();
		}
		return;
	}
	for (int slot = 0; slot < 8; slot++) {
		Refresh_Slot_Image(dialog, slot);
	}
	if (Slot_Names_Changed()) {
		Refresh_Slot_Combos(dialog);
		if (Session.Type == GAME_INTERNET) {
			Store_Slot_Pings();
		}
	}
	Refresh_Country_Flags(dialog);
	if (Country_List_Changed()) {
		Refresh_Side_Combos(dialog);
	}
	if (Color_List_Changed()) {
		Refresh_Slot_Colors(dialog);
	}
}


int __fastcall Is_Slot_Accepted(int slot) // RA2: 0x005CA3B0, not in TibSun; the slot holds a computer player or a player who accepted
{
	NodeNameType * entry = Session.Slots[slot];
	int accepted = 0;
	if ((entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]))
		|| entry->Player.Status == 1) {
		accepted = 1;
	}
	return(accepted);
}

int __fastcall Slot_Country(int slot) // RA2: 0x005CA3F0, not in TibSun; the side of a slot: the game mode's default for a lobby of its own, a computer player's choice, -2 for an open, closed or waiting slot, -3 for the observer's
{
	int country = -2;
	NodeNameType * entry = Session.Slots[slot];
	if (Session.MPGameMode != NULL && (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET)) {
		country = Session.MPGameMode->DefaultCountry();
	}
	if (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])) {
		return(Session.Options.AISlots.Countries[slot]);
	}
	if (entry == LobbyEntries[0] || entry == LobbyEntries[1] || entry == LobbyEntries[5]) {
		return(country);
	}
	if (entry == LobbyEntries[6]) {
		return(-3);
	}
	return(entry->Get_Country());
}

int __fastcall Slot_Color(int slot) // RA2: 0x005CA480, not in TibSun; the color of a slot: a computer player's choice, -2 (random) for an open, closed, waiting or observer slot
{
	NodeNameType * entry = Session.Slots[slot];
	if (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4])) {
		return(Session.Options.AISlots.Colors[slot]);
	}
	if (entry == LobbyEntries[0]) {
		return(-2);
	}
	if (entry == LobbyEntries[1]) {
		return(-2);
	}
	if (entry == LobbyEntries[5]) {
		return(-2);
	}
	if (entry == LobbyEntries[6]) {
		return(-2);
	}
	return(entry->Get_Color());
}

int __fastcall Slot_Color_Control_Id(int slot) // RA2: 0x005CA4E0, not in TibSun; the id of the control that shows a slot's color (1746 to 1753), or -1
{
	if (slot == 0) {
		return(1746);
	}
	if (slot == 1) {
		return(1747);
	}
	if (slot == 2) {
		return(1748);
	}
	if (slot == 3) {
		return(1749);
	}
	if (slot == 4) {
		return(1750);
	}
	if (slot == 5) {
		return(1751);
	}
	if (slot == 6) {
		return(1752);
	}
	if (slot == 7) {
		return(1753);
	}
	return(-1);
}

void __fastcall Refresh_Slot_Colors(HWND dialog) // RA2: 0x005CA540, not in TibSun; selects each slot's color in its color box, the first player's slot first
{
	int own = Find_First_Player_Slot();
	if (own != -1) {
		int id = Player_Color_Combo_ID(own);
		Select_Color_Combo(dialog, id, Slot_Color(own));
	}
	for (int i = 0; i < 8; i++) {
		if (i != own) {
			int id = Player_Color_Combo_ID(i);
			Select_Color_Combo(dialog, id, Slot_Color(i));
		}
	}
}

#define OD_COMBOADDSTRINGW (WM_USER + 194) // RA2: 0x4C2, adds a wide string to an owner-drawn combo box
#define OD_COMBOSETEDIT (WM_USER + 192) // RA2: 0x4C0, sets the text shown in the combo box itself

inline void Add_Combo_Entry(HWND combo, wchar_t const * text, NodeNameType * const & entry) // RA2: not in TibSun; adds a wide string to a combo box and attaches the lobby entry as its data; the entry is taken by reference because the executable reads it after the text was added (a by-value argument is read first)
{
	int index = SendMessageA(combo, OD_COMBOADDSTRINGW, 0, (LPARAM)text);
	SendMessageA(combo, CB_SETITEMDATA, index, (LPARAM)entry);
}

void __fastcall Refresh_Slot_Combos(HWND dialog) // RA2: 0x005CA6C0, not in TibSun; refills the entry list of every slot's combo box: the entries that may be chosen by the host, the current entry shown
{
	bool ai_allowed = (Session.MPGameMode == NULL || Session.MPGameMode->IsSkirmishAllowed()) && !Is_Current_Channel_Tournament();
	int limit = Get_Lobby_Slot_Limit();
	if (LobbyObserverSlot == 0) {
		if (ai_allowed || limit < Get_Lobby_Slot_Limit_Plus_One()) {
			limit++;
		}
	}
	for (int slot = 0; slot < 8; slot++) {
		bool beyond_map = false;
		if (slot == LobbyObserverSlot) {
			if (slot > Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex)) {
				NodeNameType * seated = Session.Slots[slot];
				if (seated != LobbyEntries[0] && (seated == NULL || (seated != LobbyEntries[2] && seated != LobbyEntries[3] && seated != LobbyEntries[4]))) {
					beyond_map = true;
				}
			}
			if (slot == LobbyObserverSlot && Session.MPGameMode != NULL) {
				Session.MPGameMode->vt_entry_A0();
			}
		}
		int id = Get_Slot_Control_Id(slot);
		HWND combo = GetDlgItem(dialog, id);
		bool hidden = ShowWindow(combo, SW_HIDE) == 0;
		SendDlgItemMessageA(dialog, id, CB_RESETCONTENT, 0, 0);
		NodeNameType * entry = Session.Slots[slot];
		wchar_t const * text = entry->Name;
		if (entry == LobbyEntries[0]) {
			text = StringTable::LoadString("GUI:Open", NULL, "D:\\Ra2\\netshare.cpp", 4885);
		} else if (entry == LobbyEntries[1]) {
			text = StringTable::LoadString("GUI:Closed", NULL, "D:\\Ra2\\netshare.cpp", 4887);
		} else if (entry == LobbyEntries[2]) {
			text = StringTable::LoadString("GUI:AIEasy", NULL, "D:\\Ra2\\netshare.cpp", 4889);
		} else if (entry == LobbyEntries[3]) {
			text = StringTable::LoadString("GUI:AINormal", NULL, "D:\\Ra2\\netshare.cpp", 4891);
		} else if (entry == LobbyEntries[4]) {
			text = StringTable::LoadString("GUI:AIHard", NULL, "D:\\Ra2\\netshare.cpp", 4893);
		} else if (entry == LobbyEntries[5]) {
			text = StringTable::LoadString("GUI:Waiting", NULL, "D:\\Ra2\\netshare.cpp", 4895);
		} else if (entry == LobbyEntries[6]) {
			text = StringTable::LoadString("GUI:OpenObserver", NULL, "D:\\Ra2\\netshare.cpp", 4897);
		}
		if (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]) && !ai_allowed) {
			text = StringTable::LoadString("GUI:Open", NULL, "D:\\Ra2\\netshare.cpp", 4904);
		}
		WideWstring current(text);
		if (Is_Host()) {
			if (Is_Real_Player_Entry(entry)) {
				Add_Combo_Entry(combo, text, entry);
			}
			if (!Is_Local_Name(text)) {
				if (slot < limit && (!beyond_map || slot != LobbyObserverSlot || slot < Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex))) {
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:Open", NULL, "D:\\Ra2\\netshare.cpp", 4928), LobbyEntries[0]);
				}
				if (slot == LobbyObserverSlot) {
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:OpenObserver", NULL, "D:\\Ra2\\netshare.cpp", 4938), LobbyEntries[6]);
				}
				Add_Combo_Entry(combo, StringTable::LoadString("GUI:Closed", NULL, "D:\\Ra2\\netshare.cpp", 4945), LobbyEntries[1]);
				if (ai_allowed && !beyond_map) {
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:AIEasy", NULL, "D:\\Ra2\\netshare.cpp", 4953), LobbyEntries[2]);
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:AINormal", NULL, "D:\\Ra2\\netshare.cpp", 4959), LobbyEntries[3]);
					Add_Combo_Entry(combo, StringTable::LoadString("GUI:AIHard", NULL, "D:\\Ra2\\netshare.cpp", 4965), LobbyEntries[4]);
				}
			}
		} else {
			Add_Combo_Entry(combo, text, entry);
		}
		SendMessageA(combo, OD_COMBOSETEDIT, -1, (LPARAM)current.text());
		if (!hidden) {
			ShowWindow(combo, SW_SHOW);
		}
	}
}


bool __fastcall Reset_AI_Slots(HWND dialog) // RA2: 0x005CACB0, not in TibSun; a computer player picked in a slot's combo box is replaced by an open slot, or by a closed one beyond the lobby's limit; tells whether any was
{
	bool changed = false;
	int limit = Get_Lobby_Slot_Limit();
	for (int i = 0; i < 8; i++) {
		int id = Get_Slot_Control_Id(i);
		int selected = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
		NodeNameType * data = (NodeNameType *)SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, selected, 0);
		if (Is_AI_Entry(data)) {
			if (i < limit) {
				Session.Slots[i] = LobbyEntries[0];
			} else {
				Session.Slots[i] = LobbyEntries[1];
			}
			Update_Slot_Controls(dialog, i, false);
			Session.Options.AISlots.Difficulties[i] = -1;
			Session.Options.AISlots.Countries[i] = -1;
			Session.Options.AISlots.Colors[i] = -1;
			changed = true;
		}
	}
	return(changed);
}

void __fastcall Show_Slot_Controls(HWND dialog, int first, int count) // RA2: 0x005CAE00, not in TibSun; shows the controls of count slots from the first one on
{
	int end = first + count;
	for (int slot = first; slot < end; ++slot) {
		int color_id = Slot_Color_Control_Id(slot);
		int slot_id = Get_Slot_Control_Id(slot);
		int player_id = Player_Control_ID(slot);
		int shade_id = Player_Color_Combo_ID(slot);
		int side_id = Player_Side_Combo_ID(slot);
		HWND color = GetDlgItem(dialog, color_id);
		HWND combo = GetDlgItem(dialog, slot_id);
		HWND player = GetDlgItem(dialog, player_id);
		HWND shade = GetDlgItem(dialog, shade_id);
		HWND side = GetDlgItem(dialog, side_id);
		NodeNameType * entry = Session.Slots[slot];
		bool observer;
		if (entry == LobbyEntries[6] || (entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[5] && entry->Slot.SpectatorFlag == -1)) {
			observer = true;
		} else {
			observer = false;
		}
		ShowWindow(color, SW_SHOW);
		ShowWindow(combo, SW_SHOW);
		ShowWindow(player, SW_SHOW);
		if (observer) {
			ShowWindow(shade, SW_HIDE);
		} else {
			ShowWindow(shade, SW_SHOW);
		}
		ShowWindow(side, SW_SHOW);
	}
}

void __fastcall Show_Observer_Slot_Controls(HWND dialog) // RA2: 0x005CAFD0, not in TibSun; shows the controls of the observer's slot (the last one when the observer is the first), the color box stays hidden for an observer
{
	int slot = LobbyObserverSlot;
	if (slot < 0) {
		return;
	}
	if (slot == 0) {
		slot = Session.ScenarioMaxPlayers;
	}
	if (slot >= 8) {
		return;
	}
	int color_id;
	int slot_id;
	if (slot == 0) {
		color_id = 1746;
		slot_id = 1041;
	} else if (slot == 1) {
		color_id = 1747;
		slot_id = 1042;
	} else if (slot == 2) {
		color_id = 1748;
		slot_id = 1043;
	} else if (slot == 3) {
		color_id = 1749;
		slot_id = 1044;
	} else if (slot == 4) {
		color_id = 1750;
		slot_id = 1045;
	} else if (slot == 5) {
		color_id = 1751;
		slot_id = 1046;
	} else if (slot == 6) {
		color_id = 1752;
		slot_id = 1047;
	} else if (slot == 7) {
		color_id = 1753;
		slot_id = 1048;
	} else {
		color_id = -1;
		slot_id = -1;
	}
	int player_id = Player_Control_ID(slot);
	int color_combo_id = Player_Color_Combo_ID(slot);
	int side_combo_id = Player_Side_Combo_ID(slot);
	NodeNameType * entry = Session.Slots[slot];
	bool observer;
	if (entry == LobbyEntries[6] || (entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[5] && entry->Slot.SpectatorFlag == -1)) {
		observer = true;
	} else {
		observer = false;
	}
	HWND color = GetDlgItem(dialog, color_id);
	HWND combo = GetDlgItem(dialog, slot_id);
	HWND player = GetDlgItem(dialog, player_id);
	HWND color_combo = GetDlgItem(dialog, color_combo_id);
	HWND side_combo = GetDlgItem(dialog, side_combo_id);
	ShowWindow(color, SW_SHOW);
	ShowWindow(combo, SW_SHOW);
	ShowWindow(player, SW_SHOW);
	if (!observer) {
		ShowWindow(color_combo, SW_SHOW);
		ShowWindow(side_combo, SW_SHOW);
	} else {
		ShowWindow(color_combo, SW_HIDE);
		ShowWindow(side_combo, SW_SHOW);
	}
}

void __fastcall Hide_Slot_Controls(HWND dialog, int first) // RA2: 0x005CB170, not in TibSun; hides the controls of the slots from the first one on, except the observer's
{
	for (int slot = first; slot < 8; ++slot) {
		if (slot == LobbyObserverSlot) {
			continue;
		}
		int color_id = Slot_Color_Control_Id(slot);
		int slot_id = Get_Slot_Control_Id(slot);
		int player_id = Player_Control_ID(slot);
		int side_id = Player_Side_Combo_ID(slot);
		int shade_id = Player_Color_Combo_ID(slot);
		HWND color = GetDlgItem(dialog, color_id);
		HWND entry = GetDlgItem(dialog, slot_id);
		HWND player = GetDlgItem(dialog, player_id);
		HWND side = GetDlgItem(dialog, side_id);
		HWND shade = GetDlgItem(dialog, shade_id);
		ShowWindow(color, SW_HIDE);
		ShowWindow(entry, SW_HIDE);
		ShowWindow(player, SW_HIDE);
		ShowWindow(side, SW_HIDE);
		ShowWindow(shade, SW_HIDE);
	}
}

void __fastcall Change_Slot_Count(HWND dialog, int old_count, int count, MultiplayerGameMode * mode) // RA2: 0x005CB300, not in TibSun; shows or hides the controls of the slots that were added or removed, both counts limited to the lobby's
{
	if (Session.MPGameMode != NULL && !Session.MPGameMode->IsSkirmishAllowed() && !Is_Current_Channel_Tournament()) {
		int limit = Get_Lobby_Slot_Limit();
		if (old_count > limit) {
			old_count = limit;
		}
	}
	if ((mode != NULL && !mode->IsSkirmishAllowed()) || Is_Current_Channel_Tournament()) {
		int limit = Get_Lobby_Slot_Limit();
		if (count > limit) {
			count = limit;
		}
	}
	int added = count - old_count;
	if (added > 0) {
		Show_Slot_Controls(dialog, old_count, added);
		Show_Observer_Slot_Controls(dialog);
	} else {
		if (added < 0) {
			Hide_Slot_Controls(dialog, count);
		}
		Show_Observer_Slot_Controls(dialog);
	}
}


void __fastcall Change_Slot_Type(HWND dialog, int id, NodeNameType * node, int mode); // RA2: 0x005CBF20, defined below
Wstring Wstring_Narrow(WideWstring const & text); // RA2: 0x00771280, as wonline.cpp

void __fastcall Change_Slot_Range(HWND dialog, int first, int count) // RA2: 0x005CB3F0, not in TibSun; the entries chosen in the combo boxes of count slots from the first one on are seated, empty ones become open or closed, then the controls are shown
{
	int end = first + count;
	int limit = Get_Lobby_Slot_Limit();
	for (int i = first; i < end; i++) {
		int id = Get_Slot_Control_Id(i);
		int index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
		NodeNameType * entry = (NodeNameType *)SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0);
		NodeNameType * open = LobbyEntries[0]; // RA2: the open and closed entries are read once, before the test: the executable loads both into registers there
		NodeNameType * closed = LobbyEntries[1];
		if (entry != (NodeNameType *)-1 && entry != NULL && entry != open && entry != closed && entry != LobbyEntries[6] && entry != LobbyEntries[5]) {
			Change_Slot_Type(dialog, id, entry, 0);
		} else if (i != LobbyObserverSlot) {
			if (i < limit) {
				Change_Slot_Type(dialog, id, open, 0);
			} else {
				Change_Slot_Type(dialog, id, closed, 0);
			}
		}
	}
	Show_Slot_Controls(dialog, first, count);
	Net2DisplayUsers();
	Send_Lobby_Options(true, false);
}

void __fastcall Clear_Observer_Slots(HWND dialog) // RA2: 0x005CB560, not in TibSun; observers are removed from the slots: the open ones become open or closed, the observer's own slot keeps its entry unless the scenario has no room for it
{
	bool changed = false;
	if (LobbyObserverSlot < 1) {
		int limit = Get_Lobby_Slot_Limit();
		for (int slot = 1; slot < 8; slot++) {
			int id = Get_Slot_Control_Id(slot);
			NodeNameType * entry = Session.Slots[slot];
			if (slot < limit) {
				if (entry == LobbyEntries[6] || entry->Slot.SpectatorFlag == -1) {
					Change_Slot_Type(dialog, id, LobbyEntries[0], 1);
					changed = true;
				}
			} else if (entry == LobbyEntries[6] || entry->Slot.SpectatorFlag == -1) {
				Change_Slot_Type(dialog, id, LobbyEntries[1], 1);
				changed = true;
			}
		}
	} else {
		NodeNameType * entry = Session.Slots[LobbyObserverSlot];
		if (LobbyObserverSlot > Get_Scenario_Slot_Limit(Session.Options.ScenarioIndex)) {
			NodeNameType * open = LobbyEntries[0]; // RA2: read once at the head of the test, as in Change_Slot_Range
			NodeNameType * closed = LobbyEntries[1];
			if ((Is_Seated_Entry(entry) && entry->Slot.SpectatorFlag != -1)
				|| (entry != NULL && (entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]))
				|| entry == open) {
				entry = closed;
				changed = true;
			}
		}
		Show_Observer_Slot_Controls(dialog);
		Change_Slot_Type(dialog, Get_Slot_Control_Id(LobbyObserverSlot), entry, 1);
	}
	if (changed) {
		Set_Slot_Country(dialog, Player_Side_Combo_ID(LobbyObserverSlot));
	}
	Net2DisplayUsers();
	Send_Lobby_Options(true, false);
}

void __fastcall Close_Slots_From(HWND dialog, int first) // RA2: 0x005CB7C0, not in TibSun; the slots from the first one on become open or closed, their controls are hidden and the lobby is redrawn and sent
{
	int limit = Get_Lobby_Slot_Limit();
	for (int slot = first; slot < 8; slot++) {
		int id = Get_Slot_Control_Id(slot);
		if (slot < limit) {
			if (slot != LobbyObserverSlot) {
				Change_Slot_Type(dialog, id, LobbyEntries[0], 1);
			}
		} else if (slot != LobbyObserverSlot) {
			Change_Slot_Type(dialog, id, LobbyEntries[1], 1);
		}
	}
	Hide_Slot_Controls(dialog, first);
	if (g_pChat != NULL) {
		Redraw_Player_List();
	} else {
		Net2DisplayUsers();
	}
	Send_Lobby_Options(true, false);
}

inline void Format_Slot(char * buffer, Wstring const & narrow) { sprintf(buffer, "O%d%s", LobbyObserverSlot, narrow.c_str()); } // RA2: ra2-decomp's file-local helper of the same name (src/netshare.cpp): the observer slot as the host sends it

int LobbySlotCount = 8; // RA2: 0x007E6DE4, the number of slots the scenario gave the lobby last (8 before one was chosen); used only by Change_Scenario_Slots
int LobbyExtraSlot; // RA2: 0x00A72D5C, 1 when the observer takes a slot of its own beyond the scenario's; as above

void __fastcall Change_Scenario_Slots(HWND dialog, int previous, int scenario) // RA2: 0x005CB8D0, not in TibSun; the lobby takes the number of slots of the scenario: new slots are shown and filled, removed ones are closed, and the observer's slot is moved to the last slot when the game mode asks for one
{
	int players = Get_Scenario_Slot_Limit(scenario);
	bool ai_allowed = (Session.MPGameMode == NULL || Session.MPGameMode->IsSkirmishAllowed()) && !Is_Current_Channel_Tournament();
	if (!ai_allowed) {
		int limit = Get_Lobby_Slot_Limit();
		if (players > limit) {
			players = limit;
		}
	}
	if (previous == -1) {
		LobbySlotCount = 8;
		LobbyExtraSlot = 0;
	}
	int added = players - LobbySlotCount;
	int extra = 0;
	if (LobbyObserverSlot == 0) {
		if (ai_allowed || players < Get_Lobby_Slot_Limit_Plus_One()) {
			extra = 1;
			if (LobbyExtraSlot == 0 && added + players < 7) {
				added++;
			}
		}
	}
	if (previous == -1 && LobbyObserverSlot == 0) {
		int side = Player_Side_Combo_ID(0);
		Fill_Observer_Combo(dialog, side);
		Select_Side_Combo(dialog, side, -3);
		SendMessageA(GetDlgItem(dialog, 1041), 0x4F1, 0, 1);
		int color = Player_Color_Combo_ID(0);
		Fill_Random_Color_Combo(dialog, color);
		Select_Color_Combo(dialog, color, 8);
	}
	if (added > 0) {
		Change_Slot_Range(dialog, LobbyExtraSlot + LobbySlotCount, added);
	} else if (added < 0) {
		int first = extra + players; // RA2: the original expands Close_Slots_From's body here (main's text writes it out)
		int limit = Get_Lobby_Slot_Limit();
		for (int slot = first; slot < 8; slot++) {
			int id = Get_Slot_Control_Id(slot);
			if (slot < limit) {
				if (slot != LobbyObserverSlot) {
					Change_Slot_Type(dialog, id, LobbyEntries[0], 1);
				}
			} else if (slot != LobbyObserverSlot) {
				Change_Slot_Type(dialog, id, LobbyEntries[1], 1);
			}
		}
		Hide_Slot_Controls(dialog, first);
		if (g_pChat != NULL) {
			Redraw_Player_List();
		} else {
			Net2DisplayUsers();
		}
		Send_Lobby_Options(true, false);
	} else if (extra + players >= 7) {
		Change_Slot_Range(dialog, 7, 1);
	}
	Clear_Observer_Slots(dialog);
	if (LobbyObserverSlot == -1 && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_A0()) {
		LobbyObserverSlot = 7;
		Clear_Observer_Slots(dialog);
		DebugString("Sending observer slot of 7\n");
		NodeNameType * entry = Session.Slots[LobbyObserverSlot];
		WideWstring name;
		if (entry == NULL || entry == LobbyEntries[0] || entry == LobbyEntries[1] || entry == LobbyEntries[5] || entry == LobbyEntries[6]
			|| entry == LobbyEntries[2] || entry == LobbyEntries[3] || entry == LobbyEntries[4]) {
			Wstring narrow = Lobby_Entry_Name(entry);
			name = Wstring_ToWide(narrow);
		} else {
			name = entry->Name;
		}
		char text[256];
		Format_Slot(text, Wstring_Narrow(name.get()));
		Send_Host_Option(text);
	}
	LobbySlotCount = players;
	LobbyExtraSlot = extra;
}


bool __fastcall Lobby_Update_Dialog(HWND dialog, int count, bool flag1, bool flag2) // RA2: 0x005CBDB0, not in TibSun; tells whether the combo boxes hold a computer player where the game mode takes none, or a player beyond the count: an observer beyond it counts only when the game mode takes observers
{
	if (LobbyObserverSlot == 0) {
		count++;
	}
	for (int i = 0; i < 8; i++) {
		int id = Get_Slot_Control_Id(i);
		int index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
		int data = SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0);
		NodeNameType * entry;
		if (data == -1) {
			entry = NULL;
		} else {
			entry = (NodeNameType *)data;
		}
		if (Is_AI_Entry(entry) && Session.MPGameMode != NULL && !flag1) {
			return(true);
		}
		if (i >= count) {
			bool player = entry != NULL && entry != LobbyEntries[0] && entry != LobbyEntries[1] && entry != LobbyEntries[6] && entry != LobbyEntries[5];
			if (player && entry->Get_Country() != -3) {
				return(true);
			}
			if (Session.MPGameMode != NULL && !flag2 && player && entry->Get_Country() == -3) {
				return(true);
			}
		}
	}
	return(false);
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
	NodeNameType * entry = Get_Kind_Entry(kind);
	Select_Combo_By_Data(dialog, id, (int)entry);
	Change_Slot_Type(dialog, id, entry, 0);
}

void __fastcall Load_Random_Map_Preview(HWND dialog) // RA2: 0x005CC800, not in TibSun; the preview of a random map is read from RandMap.img, any other map's is drawn
{
	if (Session.Scenarios[Session.Options.ScenarioIndex]->Is_Random_Map()) {
		delete MultiplayerMapPreview;
		MultiplayerMapPreview = new MapPreviewClass;
		MultiplayerMapPreview->Read_PCX_Preview("RandMap.img");
		if (MultiplayerMapPreview->Get_Preview_Surface() == NULL) {
			Update_Network_Dialog_Preview(dialog);
		}
		InvalidateRect(dialog, NULL, FALSE);
	} else {
		Update_Network_Dialog_Preview(dialog);
	}
}

void __fastcall Save_Lobby_Options(HWND dialog, SessionOptionsClass * options) // RA2: 0x005CC890, not in TibSun; the game mode, map, the type, side and color of every slot but the first and the game settings are kept in the options
{
	options->MPGameMode = Session.Options.MPModeIndex;
	options->ScenIndex = Session.Options.ScenarioIndex;
	for (int slot = 1; slot < 8; slot++) {
		int id = Get_Slot_Control_Id(slot);
		int index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
		int data = SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0);
		options->SlotData[slot][0] = Get_Entry_Kind((NodeNameType *)(data == -1 ? 0 : data));
		int side_id = Player_Side_Combo_ID(slot);
		options->SlotData[slot][1] = Side_Combo_Data(dialog, side_id, -1);
		int color_id = Player_Color_Combo_ID(slot);
		options->SlotData[slot][2] = Color_Combo_Data(dialog, color_id, -1);
	}
	options->GameSpeed = Session.Options.GameSpeed;
	options->Credits = Session.Options.Credits;
	options->UnitCount = Session.Options.UnitCount;
	options->ShortGame = Session.Options.ShortGame;
	options->SuperWeaponsAllowed = Session.Options.SWAllowed;
	options->BuildOffAlly = Session.Options.BuildOffAlly;
	options->MCVRepacks = Session.Options.MCVRedeploy;
	options->CratesAppear = Session.Options.Goodies;
}

void __fastcall Load_Lobby_Options(HWND dialog, SessionOptionsClass * options) // RA2: 0x005CCA30, not in TibSun; the saved game mode, map and settings are made current again (a restricted channel takes the first game mode if the saved one is not allowed) and every slot but the first is set to its saved type, side and color
{
	if (options->MPGameMode < 1 || options->MPGameMode > MultiplayerGameMode::GetModeCount()) {
		options->MPGameMode = First_Game_Mode()->MPModeIndex;
	}
	Session.Options.MPModeIndex = options->MPGameMode;
	MultiplayerGameMode * mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex);
	if (mode == NULL) {
		mode = First_Game_Mode();
	}
	if ((Is_Current_Channel_Tournament() || Is_Current_Channel_Flag_8()) && mode != NULL && mode->vt_entry_04()) {
		options->MPGameMode = First_Game_Mode()->MPModeIndex;
		Session.Options.MPModeIndex = options->MPGameMode;
		MultiplayerGameMode * found = MultiplayerGameMode::Get(Session.Options.MPModeIndex);
		if (found == NULL) {
			found = First_Game_Mode();
		}
		mode = found;
	}
	options->MPGameMode = mode->MPModeIndex;
	Session.Options.MPModeIndex = mode->MPModeIndex;
	if (options->ScenIndex < 0 || options->ScenIndex >= Session.Scenarios.Count()) {
		options->ScenIndex = 0;
	}
	Session.Options.ScenarioIndex = Set_Scenario_Info_From_Index(options->ScenIndex) ? options->ScenIndex : 0;
	MultiMission * scenario = Session.Scenarios[Session.Options.ScenarioIndex]; // RA2: the scenario being checked
	if (!mode->IsMapAllowed(scenario)) {
		int index = mode->FirstValidMapIndex();
		if (index == -1) {
			mode = First_Game_Mode();
			options->MPGameMode = mode->MPModeIndex;
			Session.Options.MPModeIndex = mode->MPModeIndex;
			index = mode->FirstValidMapIndex();
		}
		options->ScenIndex = index;
		Session.Options.ScenarioIndex = index;
		Set_Scenario_Info_From_Index(index);
	}
	Session.MPGameMode = mode;
	Set_Scenario_Info_From_Index(Session.Options.ScenarioIndex);
	Session.MPGameMode->vt_entry_20();
	Session.MPGameMode->vt_entry_24();
	mode->AssignTeamsAtStart();
	mode->UnfixAlliances();
	int limit = 8;
	if (Session.Type == GAME_INTERNET) {
		limit = (Is_Current_Channel_Tournament() && !Is_Current_Channel_Flag_8()) ? 2 : 4;
	}
	if (LobbyObserverSlot == 0 && limit < 8) {
		limit++;
	}
	for (int slot = 1; slot < 8; slot++) {
		int id = Get_Slot_Control_Id(slot);
		int kind = options->SlotData[slot][0];
		if (Is_Current_Channel_Tournament() || LobbyObserverSlot == 0 || ObserverMode != 0) {
			if (kind == 4) {
				kind = 2;
			} else if (kind == 5) {
				kind = 2;
			} else if (kind == 6) {
				kind = 2;
			}
		}
		if (slot >= limit) {
			kind = 3;
		} else if (kind == 7) {
			kind = 2;
		}
		Select_Entry_Kind(dialog, id, kind);
		int side_id = Player_Side_Combo_ID(slot);
		int color_id = Player_Color_Combo_ID(slot);
		if (kind != 2 && kind != 3) {
			if (kind == 7) {
				Select_Side_Combo(dialog, side_id, -3);
				Select_Color_Combo(dialog, color_id, -2);
				HWND side = GetDlgItem(dialog, side_id);
				HWND colors = GetDlgItem(dialog, color_id);
				EnableWindow(side, FALSE);
				EnableWindow(colors, FALSE);
			} else {
				Select_Side_Combo(dialog, side_id, options->SlotData[slot][1]);
				Set_Slot_Country(dialog, side_id);
				Select_Color_Combo(dialog, color_id, options->SlotData[slot][2]);
				Set_Slot_Color(dialog, color_id);
			}
		} else {
			Select_Side_Combo(dialog, side_id, Session.MPGameMode->DefaultCountry());
			Select_Color_Combo(dialog, color_id, -2);
			HWND side = GetDlgItem(dialog, side_id);
			HWND colors = GetDlgItem(dialog, color_id);
			EnableWindow(side, FALSE);
			EnableWindow(colors, FALSE);
		}
	}
	Session.Options.GameSpeed = options->GameSpeed;
	Session.Options.Credits = options->Credits;
	Session.Options.UnitCount = options->UnitCount;
	Session.Options.ShortGame = options->ShortGame;
	Session.Options.SWAllowed = options->SuperWeaponsAllowed;
	Session.Options.BuildOffAlly = options->BuildOffAlly;
	Session.Options.MCVRedeploy = options->MCVRepacks;
	Session.Options.Goodies = options->CratesAppear;
}


void __fastcall Get_Window_Wide_Text(HWND window, WideWstring & text); // RA2: 0x00600E50, the text of a window, as Get_List_Item_Wide_Text for a list box item

WideWstring __fastcall Limit_Name_Text(HWND control) // RA2: 0x005CCF50, not in TibSun; the text of a name edit with the line breaks cut off; it is cut to what the lobby allows: 120 characters, or the ones below 0x100 after the first wider character (40 at least) in a name with wider characters
{
	WideWstring text;
	Get_Window_Wide_Text(control, text);
	unsigned int limit = 120;
	if (!text.Is_Narrow()) {
		unsigned int i;
		for (i = 0; i < text.length(); i++) {
			if (text.getChar(i) > 256) {
				break;
			}
		}
		limit = i - 1;
		if (limit <= 40) {
			limit = 40;
		}
	}
	if (text.length() > limit) {
		text.truncate(limit);
		SendMessageA(control, OD_SETTEXTW, 0, (LPARAM)text.text());
	}
	{
		WideWstring line_breaks(L"\n\r");
		text.trimTrailing(line_breaks);
	}
	return(text);
}

void __fastcall Enable_Player_Combos(HWND dialog, bool value) // RA2: 0x005CD030, not in TibSun; enables or disables the side and color boxes of the first player's slot, unless it is the observer's
{
	if (dialog != NULL) {
		int slot = Find_First_Player_Slot();
		if (slot != LobbyObserverSlot) {
			HWND control = GetDlgItem(dialog, Player_Side_Combo_ID(slot));
			if (control != NULL) {
				EnableWindow(control, value);
			}
			control = GetDlgItem(dialog, Player_Color_Combo_ID(slot));
			if (control != NULL) {
				EnableWindow(control, value);
			}
		}
	}
}

char const * LobbyEntryCodes[7] = { "@Open@", "@Closed@", "@Waiting@", "@EasyAI@", "@MediumAI@", "@HardAI@", "@OpenObserver@" }; // RA2: 0x007E6DE8, the names the fixed entries go by in the slot names the host sends: open, closed, waiting, easy, medium and hard computer player and open observer slot

Wstring __fastcall Lobby_Entry_Name(NodeNameType * entry) // RA2: 0x005CD0E0, not in TibSun; the name of a fixed entry in the slot names the host sends
{
	if (entry == LobbyEntries[0]) {
		return(Wstring(LobbyEntryCodes[0]));
	}
	if (entry == LobbyEntries[1]) {
		return(Wstring(LobbyEntryCodes[1]));
	}
	if (entry == LobbyEntries[5]) {
		return(Wstring(LobbyEntryCodes[2]));
	}
	if (entry == LobbyEntries[2]) {
		return(Wstring(LobbyEntryCodes[3]));
	}
	if (entry == LobbyEntries[3]) {
		return(Wstring(LobbyEntryCodes[4]));
	}
	if (entry == LobbyEntries[4]) {
		return(Wstring(LobbyEntryCodes[5]));
	}
	if (entry == LobbyEntries[6]) {
		return(Wstring(LobbyEntryCodes[6]));
	}
	return(Wstring(LobbyEntryCodes[2]));
}

NodeNameType * __fastcall Find_Lobby_Entry_By_Name(WideWstring const & name) // RA2: 0x005CD1B0, not in TibSun; the fixed entry a name of the slot names stands for, NULL for any other
{
	if (name.Is_Narrow()) {
		Wstring narrow = Wstring_Narrow(name);
		if (narrow == LobbyEntryCodes[0]) {
			return(LobbyEntries[0]);
		}
		if (narrow == LobbyEntryCodes[1]) {
			return(LobbyEntries[1]);
		}
		if (narrow == LobbyEntryCodes[2]) {
			return(LobbyEntries[5]);
		}
		if (narrow == LobbyEntryCodes[3]) {
			return(LobbyEntries[2]);
		}
		if (narrow == LobbyEntryCodes[4]) {
			return(LobbyEntries[3]);
		}
		if (narrow == LobbyEntryCodes[5]) {
			return(LobbyEntries[4]);
		}
		if (narrow == LobbyEntryCodes[6]) {
			return(LobbyEntries[6]);
		}
	}
	return(NULL);
}

bool __fastcall Is_Lobby_Name(Wstring const & name) // RA2: 0x005CD2F0, not in TibSun; the name is one of the seven names of the fixed entries
{
	return(name == LobbyEntryCodes[0] || name == LobbyEntryCodes[1] || name == LobbyEntryCodes[2] || name == LobbyEntryCodes[3] || name == LobbyEntryCodes[4] || name == LobbyEntryCodes[5] || name == LobbyEntryCodes[6]);
}

void Load_Observer_Mode(void) // RA2: 0x005CD380, not in TibSun; reads the observer mode from RA2.INI
{
	ObserverMode = ConfigINI.Get_Int("MultiPlayer", "ObserverMode", 0);
}

void __fastcall Save_Observer_Mode(void) // RA2: 0x005CD3A0, not in TibSun; writes the observer mode to RA2.INI
{
	RawFileClass file("RA2.INI");
	ConfigINI.Put_Int("MultiPlayer", "ObserverMode", ObserverMode);
	ConfigINI.Save(file, false);
}

void __fastcall Update_Observer_Button(HWND dialog) // RA2: 0x005CD3F0, not in TibSun; the observer radio button, its picture and the text of the join button follow the observer mode
{
	if (dialog != NULL) {
		HWND control = GetDlgItem(dialog, 1888);
		if (control != NULL) {
			SendMessageA(control, BM_SETCHECK, ObserverMode != 0, 0);
		}
		Surface * image;
		if (ObserverMode) {
			image = SurfaceCache.GetSurface("wolob.pcx", NULL);
		} else {
			image = SurfaceCache.GetSurface("wolnob.pcx", NULL);
		}
		HWND picture = GetDlgItem(dialog, 1887);
		OwnerDraw::Set_Control_Image(picture, image);
		control = GetDlgItem(dialog, 1741);
		if (control != NULL) {
			wchar_t const * text;
			if (ObserverMode) {
				text = StringTable::LoadString("GUI:Observe", NULL, "D:\\Ra2\\netshare.cpp", 6072);
			} else {
				text = StringTable::LoadString("GUI:Join", NULL, "D:\\Ra2\\netshare.cpp", 6074);
			}
			SendMessageA(control, OD_SETTEXTW, 0, (LPARAM)text);
			InvalidateRect(control, NULL, FALSE);
		}
		control = GetDlgItem(dialog, IDC_JOINLEAVE);
		if (control != NULL) {
			wchar_t const * text;
			if (ObserverMode) {
				text = StringTable::LoadString("GUI:Observe", NULL, "D:\\Ra2\\netshare.cpp", 6083);
			} else if (WS_Find_Dialog(0x113) == dialog) {
				text = StringTable::LoadString("GUI:JoinGame", NULL, "D:\\Ra2\\netshare.cpp", 6087);
			} else {
				text = StringTable::LoadString("GUI:Join", NULL, "D:\\Ra2\\netshare.cpp", 6089);
			}
			SendMessageA(control, OD_SETTEXTW, 0, (LPARAM)text);
			InvalidateRect(control, NULL, FALSE);
		}
	}
}

void __fastcall Update_Observer_Buttons(HWND dialog) // RA2: 0x005CD540, not in TibSun; the observer buttons of the dialog and of every lobby dialog that is open follow the observer mode
{
	if (dialog != NULL) {
		Update_Observer_Button(dialog);
	}
	HWND other = WS_Find_Dialog(0x116);
	if (other != NULL) {
		Update_Observer_Button(other);
	}
	other = WS_Find_Dialog(0x113);
	if (other != NULL) {
		Update_Observer_Button(other);
	}
	other = WS_Find_Dialog(IDD_MPLAYER_GAME_LIST);
	if (other != NULL) {
		Update_Observer_Button(other);
	}
}

