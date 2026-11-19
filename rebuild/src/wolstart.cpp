/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/



#include "always.h"
#include "rect.h"
#include "geometry.h"
#include "_xmouse.h"
#include "globals.h"
#include "rect.h"
#include "session.h"
#include "theme.h"
#include "wolopt.h"
#include "wonline.h"
#include "worlddom.h"
#include "playerprofile.h"


#include "geometry.h"

int Online_Login_Kind = 0;


/// <summary>
/// The kind of the current online session: 0 outside a session and in the lobby, 1 for a normal one and 2 for a tournament.
/// </summary>
int Online_Session_Kind(void)
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
#if 0
int __fastcall Wonline_Main(bool returning)
{
	while (MouseCursor->Get_Mouse_State() < 0) {
		MouseCursor->Show_Mouse();
	}
	Reset_Channel_State();

	if (Theme.What_Is_Playing() == Theme.From_Name("INTRO")) {
		Theme.Queue_Song(THEME_QUIET);
	}

	Read_WOL_Settings();
	bool shuffle = Theme.Is_Shuffle();
	Theme.Set_Shuffle(true);
	Apply_WOL_Settings();

	if (g_NickName.length() == 0) {
		g_NickName = WOL_Auto_Login_Persona();
		if (g_NickName.length() == 0) {
			WOLOptionsClass options;
			options.Load();
			if (options.Is_Bit1_Clear()) {
				g_NickName = WOL_Last_Valid_Persona();
			}
		}
	}

	int result = 0;
	bool done = false;
	for (;;) {
		int kind = Online_Session_Kind();
		if (Online_Login_Kind == 0 && kind != 0) {
			result = Login_Wonline();
			switch (result) {
			case 0:
				if (Online_Session_State == 0) {
					Session.Type = GAME_NORMAL;
					Shutdown_Chat();
					Theme.Set_Shuffle(shuffle);
					Close_All_Dialogs();
					return(result);
				}
				Online_Session_State = 0;
				Online_Session_Return = 0;
				break;

			case 1:
				Online_Login_Kind = kind;
				break;

			default:
				Session.Type = GAME_NORMAL;
				Online_Session_State = 0;
				Online_Session_Return = 0;
				Shutdown_Chat();
				Theme.Set_Shuffle(shuffle);
				Close_All_Dialogs();
				return(result);
			}
		}

		Apply_WOL_Settings();
		switch (Online_Session_State) {
		case 0:
			returning = false;
			result = Run_Wonline_Selection_Dialog();
			done = result != 1;
			break;

		case 1:
			if (returning) {
				Open_Buddy_Dialog();
				Online_Session_State = 0;
				break;
			}
			result = Start_Quick_Match();
			returning = false;
			if (result == 0) {
				Online_Session_State = 0;
			} else {
				done = true;
			}
			break;

		case 3:
			result = Run_Lobby_Session(0, returning);
			returning = false;
			if (result == 0) {
				if (Online_Session_State == 3) {
					Online_Session_State = 0;
				}
			} else {
				done = true;
			}
			break;

		case 2:
			if (returning) {
				Open_Buddy_Dialog();
			}
			result = WDT_Setup_Game(returning);
			returning = false;
			if (result == 0) {
				Online_Session_State = 0;
			} else {
				done = true;
			}
			break;

		case 4:
			result = Run_Online_Menu(returning);
			returning = false;
			if (result == 0) {
				if (Online_Session_State == 4) {
					Online_Session_State = Online_Session_Return;
					Online_Session_Return = 0;
				}
			} else {
				done = true;
			}
			break;

		case 5:
			result = Run_Wonline_Lobby();
			returning = false;
			if (result == -2 || result == -1) {
				Session.Type = GAME_NORMAL;
				Online_Session_State = 0;
				Online_Session_Return = 0;
				Shutdown_Chat();
				Theme.Set_Shuffle(shuffle);
				Close_All_Dialogs();
				return(result);
			}
			if (result == 0) {
				Online_Session_State = 0;
			}
			break;
		}

		if (result != 1 && Online_Login_Kind != 0) {
			bool force = false;
			if (done) {
				force = true;
			}
			if (Online_Login_Kind != Online_Session_Kind() || force) {
				Logout_WOnline();
				Online_Login_Kind = 0;
			}
		}

		if (done) {
			if (result == 1) {
				Close_All_Dialogs();
				return(result);
			}
			Shutdown_Chat();
			Session.Type = GAME_NORMAL;
			Theme.Set_Shuffle(shuffle);
			Online_Session_State = 0;
			Online_Session_Return = 0;
			Close_All_Dialogs();
			return(0);
		}
	}
}
#endif

