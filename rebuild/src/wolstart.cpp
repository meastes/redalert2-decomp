/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

/*
**	RA2: the entry into Westwood Online (WOLStart.cpp in the executable's debug text): Wonline_Main logs the
**	player in, runs the Internet dialogs one after the other until a game is ready or the player leaves, and
**	logs out again. TibSun has no such file; the code is the executable's from 0x0073A300.
*/

#define GEOMETRY_H // RA2: geometry.h follows the rectangle below, as the executable initializes the rectangle first (0x0073A0F0) and the geometry constants after it (0x0073A110 to 0x0073A2D0)
#include "always.h" // RA2: the includes this file needs
#include "_xmouse.h" // RA2: MouseCursor
#include "globals.h" // RA2: as above
#include "rect.h" // RA2: as above
#include "session.h" // RA2: as above
#include "theme.h" // RA2: as above
#include "wolopt.h" // RA2: as above
#include "wonline.h" // RA2: as above
#include "worlddom.h" // RA2: as above
#include "playerprofile.h" // RA2: as above


#include "geometry.h" // RA2: this module holds its own copy of the geometry constants (initialized after the rectangle, from 0x0073A110)

int Online_Login_Kind = 0; // RA2: 0x00B264F0, the session kind (1 normal, 2 tournament) the player is logged in for; 0 when not logged in

void __fastcall Reset_Channel_State(void); // RA2: 0x00753F10, clears the channel lists and the current channel
int __fastcall Login_Wonline(void); // RA2: 0x00742FD0, the login dialogs; 1 once logged in, 0 to go back
int __fastcall Run_Wonline_Selection_Dialog(void); // RA2: 0x00753110, the game selection dialog of the Internet menu; 1 once a game is ready
void __fastcall Open_Buddy_Dialog(void); // RA2: 0x0075CBA0, offers to add the buddies of the last match
int __fastcall Run_Lobby_Session(int unused, bool first_time); // RA2: 0x00743720, the lobby session
int __fastcall Run_Online_Menu(bool first_time); // RA2: 0x00759370, the online menu
int __fastcall Run_Wonline_Lobby(void); // RA2: 0x00753F70, the lobby; -2 or -1 to leave
void __fastcall Close_All_Dialogs(void); // RA2: 0x00753F50, destroys the dialogs that are still open
int Start_Quick_Match(void); // RA2: 0x00759E20, starts the match (not in this module); 1 once the game is over

/// <summary>
/// The kind of the current online session: 0 outside a session and in the lobby, 1 for a normal one and 2 for a tournament.
/// </summary>
int Online_Session_Kind(void) // RA2: 0x0073A2E0, a helper that Wonline_Main expands where it asks
{
	if (Online_Session_State != 0 && Online_Session_State != 5) {
		return((Online_Session_State == 2) + 1);
	}
	return(0);
}

/// <summary>
/// Runs Westwood Online from the Internet button of the main menu: logs in when needed, then loops over the
/// dialog of the current state until a game is ready (1), the player goes back (0) or quits (-1 or -2).
/// </summary>
int __fastcall Wonline_Main(bool returning) // RA2: 0x0073A300
{
	while (MouseCursor->Get_Mouse_State() < 0) { // RA2: shows the mouse until it is visible
		MouseCursor->Show_Mouse();
	}
	Reset_Channel_State(); // RA2: as above

	if (Theme.What_Is_Playing() == Theme.From_Name("INTRO")) { // RA2: the intro song stops at once
		Theme.Queue_Song(THEME_QUIET); // RA2: -3
	}

	Read_WOL_Settings(); // RA2: 0x0073C7C0
	bool shuffle = Theme.Is_Shuffle(); // RA2: kept to be restored on the way out
	Theme.Set_Shuffle(true); // RA2: the Internet dialogs play the songs in random order
	Apply_WOL_Settings(); // RA2: 0x0073C990

	if (g_NickName.length() == 0) { // RA2: no persona yet: the automatic login or the last valid one
		g_NickName = WOL_Auto_Login_Persona(); // RA2: as above
		if (g_NickName.length() == 0) { // RA2: as above
			WOLOptionsClass options; // RA2: as above
			options.Load(); // RA2: as above
			if (options.Is_Bit1_Clear()) { // RA2: as above
				g_NickName = WOL_Last_Valid_Persona(); // RA2: as above
			}
		}
	}

	int result = 0; // RA2: the result of the last dialog
	bool done = false; // RA2: set when a dialog ended the run
	for (;;) {
		int kind = Online_Session_Kind(); // RA2: the helper at 0x0073A2E0
		if (Online_Login_Kind == 0 && kind != 0) { // RA2: a session wants a login
			result = Login_Wonline(); // RA2: as above
			switch (result) {
			case 0: // RA2: the player backed out of the login
				if (Online_Session_State == 0) { // RA2: nothing to go back to: leave
					Session.Type = GAME_NORMAL; // RA2: as above
					Shutdown_Chat(); // RA2: 0x00742080
					Theme.Set_Shuffle(shuffle); // RA2: as above
					Close_All_Dialogs(); // RA2: as above
					return(result); // RA2: as above
				}
				Online_Session_State = 0; // RA2: back to the Internet menu
				Online_Session_Return = 0; // RA2: as above
				break;

			case 1: // RA2: logged in
				Online_Login_Kind = kind; // RA2: as above
				break;

			default: // RA2: a quit or a patch download
				Session.Type = GAME_NORMAL; // RA2: as above
				Online_Session_State = 0; // RA2: as above
				Online_Session_Return = 0; // RA2: as above
				Shutdown_Chat(); // RA2: as above
				Theme.Set_Shuffle(shuffle); // RA2: as above
				Close_All_Dialogs(); // RA2: as above
				return(result); // RA2: as above
			}
		}

		Apply_WOL_Settings(); // RA2: as above
		switch (Online_Session_State) {
		case 0: // RA2: the Internet menu
			returning = false; // RA2: as above
			result = Run_Wonline_Selection_Dialog(); // RA2: as above
			done = result != 1; // RA2: as above
			break;

		case 1: // RA2: a quick match
			if (returning) { // RA2: coming back from a game
				Open_Buddy_Dialog(); // RA2: as above
				Online_Session_State = 0; // RA2: as above
				break;
			}
			result = Start_Quick_Match(); // RA2: 0x00759E20
			returning = false; // RA2: as above
			if (result == 0) { // RA2: as above
				Online_Session_State = 0; // RA2: as above
			} else {
				done = true; // RA2: as above
			}
			break;

		case 3: // RA2: the lobby session
			result = Run_Lobby_Session(0, returning); // RA2: as above
			returning = false; // RA2: as above
			if (result == 0) { // RA2: as above
				if (Online_Session_State == 3) { // RA2: as above
					Online_Session_State = 0; // RA2: as above
				}
			} else {
				done = true; // RA2: as above
			}
			break;

		case 2: // RA2: a tournament game
			if (returning) { // RA2: coming back from a game
				Open_Buddy_Dialog(); // RA2: as above
			}
			result = WDT_Setup_Game(returning); // RA2: 0x00768E90
			returning = false; // RA2: as above
			if (result == 0) { // RA2: as above
				Online_Session_State = 0; // RA2: as above
			} else {
				done = true; // RA2: as above
			}
			break;

		case 4: // RA2: the online menu
			result = Run_Online_Menu(returning); // RA2: as above
			returning = false; // RA2: as above
			if (result == 0) { // RA2: as above
				if (Online_Session_State == 4) { // RA2: back to where the menu was entered from
					Online_Session_State = Online_Session_Return; // RA2: as above
					Online_Session_Return = 0; // RA2: as above
				}
			} else {
				done = true; // RA2: as above
			}
			break;

		case 5: // RA2: the lobby
			result = Run_Wonline_Lobby(); // RA2: as above
			returning = false; // RA2: as above
			if (result == -2 || result == -1) { // RA2: leave Westwood Online
				Session.Type = GAME_NORMAL; // RA2: as above
				Online_Session_State = 0; // RA2: as above
				Online_Session_Return = 0; // RA2: as above
				Shutdown_Chat(); // RA2: as above
				Theme.Set_Shuffle(shuffle); // RA2: as above
				Close_All_Dialogs(); // RA2: as above
				return(result); // RA2: as above
			}
			if (result == 0) { // RA2: as above
				Online_Session_State = 0; // RA2: as above
			}
			break;
		}

		if (result != 1 && Online_Login_Kind != 0) { // RA2: log out when the session kind changed or the run ends
			bool force = false; // RA2: as above
			if (done) { // RA2: as above
				force = true; // RA2: as above
			}
			if (Online_Login_Kind != Online_Session_Kind() || force) { // RA2: as above
				Logout_WOnline(); // RA2: 0x00741EA0
				Online_Login_Kind = 0; // RA2: as above
			}
		}

		if (done) { // RA2: the run ends
			if (result == 1) { // RA2: a game is ready: stay logged in
				Close_All_Dialogs(); // RA2: as above
				return(result); // RA2: as above
			}
			Shutdown_Chat(); // RA2: as above
			Session.Type = GAME_NORMAL; // RA2: as above
			Theme.Set_Shuffle(shuffle); // RA2: as above
			Online_Session_State = 0; // RA2: as above
			Online_Session_Return = 0; // RA2: as above
			Close_All_Dialogs(); // RA2: as above
			return(0); // RA2: as above
		}
	}
}
