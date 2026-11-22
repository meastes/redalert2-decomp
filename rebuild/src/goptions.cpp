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

/* $Header: /counterstrike/GOPTIONS.CPP 6     3/15/97 7:18p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OPTIONS.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 8, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 27, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OptionsClass::Process -- Handles all the options graphic interface.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "goptions.h"

#include "_keyboar.h"
#include "_map.h"
#include "data.h"
#include "dbgprint.h"
#include "gamedlg.h"
#include "language\language.h"
#include "loaddlg.h"
#include "ownrdraw.h"
#include "queue.h"
#include "restate.h"
#include "scenario.h"
#include "wonline.h"

#include "special.hh"

#include "_timer.h" // RA2: the anti-cheat timers
#include "house.h" // RA2: the live human count
#include "stringtable.h" // RA2: button captions


#define RA2_SOURCE "D:\\Ra2\\GOptions.CPP" // RA2: the source name LoadString records

/*
**	RA2's game options dialog controls (resource IDs; names chosen here).
*/
#define IDC_GOPTIONS_LEAVE		1737	// RA2: Leave / Observe
#define IDC_GOPTIONS_RESTART	1810	// RA2: Restart / Observe
#define IDC_GOPTIONS_RESUME		1670	// RA2: Resume
#define IDD_OPT_CTRL_WOL_SLIDERS 255		// RA2: the online options dialog with the latency and speed sliders

static int LeaveResult; // RA2: what the Leave button returns (0xA40E1C)
static int RestartResult; // RA2: what the Restart button returns (0xA40E20)


void Game_Options_On_INITDIALOG(HWND window);
BOOL CALLBACK Game_Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK Abort_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: defined after the dialog


/// <summary>
/// Displays the in game options dialog.
/// This routine is used by the special dialog handler when the player calls up the options
/// screen. Which dialog appears depends on the kind of game in progress. If the player asked
/// for the mission briefing it is restated on the way out.
/// </summary>
void Game_Options_Dialog(void)
{
	int rc = 0;

	HWND dialog;
	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
		dialog = OwnerDraw::Begin_Dialog(IDD_OPT_CTRL_SP, Game_Options_Dialog_Proc, 0);
	} else if (Session.Type == GAME_INTERNET) {
		if (MuteSWLaunches) {
			dialog = OwnerDraw::Begin_Dialog(IDD_OPT_CTRL_MP, Game_Options_Dialog_Proc, 0);
		} else {
			dialog = OwnerDraw::Begin_Dialog(IDD_OPT_CTRL_WOL_SLIDERS, Game_Options_Dialog_Proc, 0);
		}
	} else {
		dialog = OwnerDraw::Begin_Dialog(IDD_OPT_CTRL_MP, Game_Options_Dialog_Proc, 0);
	}

	if (dialog) {
		SetWindowLong(dialog, DWL_USER, (LONG)&rc);
		OwnerDraw::Display_Dialog(dialog);
		while (rc == 0) {
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				rc = IDOK;
			}
		}
		OwnerDraw::End_Dialog(dialog);
	}

	if (rc == IDC_BRIEFING) {
		Restate_Mission(Scen);
	}

	Map.Flag_To_Redraw(GS_REDRAW_ALL);
}


/// <summary>
/// Services the option buttons of the in game options dialog: save, load, delete,
/// briefing, settings, abort and resume. In an internet game a change of the latency or
/// game speed slider is sent as an event when the dialog is resumed, at most once every
/// fifteen seconds each so that it cannot be used to cheat.
/// </summary>
static BOOL Game_Options_On_COMMAND(HWND window, WPARAM wparam)
{
	HWND handle;

	int code = HIWORD(wparam);
	int * retval = (int *)GetWindowLong(window, DWL_USER);

	static CDTimerClass<SystemTimerClass> SpeedTimer;
	static CDTimerClass<SystemTimerClass> LatencyTimer;

	switch (LOWORD(wparam)) {

		case IDC_DELETE_GAME:
			if (!code) {
				ShowWindow(window, SW_HIDE);
				LoadOptionsClass().Delete();
				Game_Options_On_INITDIALOG(window);
				ShowWindow(window, SW_SHOW);
				UpdateWindow(window);
			}
			break;

		case IDC_SAVE_GAME:
			if (!code) {
				if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
					ShowWindow(window, SW_HIDE);
					wchar_t description[512];
					swprintf(description, L"%s", Scen->Description);
					LoadOptionsClass().Save(description);
					Game_Options_On_INITDIALOG(window);
					ShowWindow(window, SW_SHOW);
					UpdateWindow(window);
				} else {
					OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::SAVEGAME));
					*retval = IDC_SAVE_GAME;
				}
			}
			break;

		case IDC_LOAD_GAME:
			if (!code) {
				ShowWindow(window, SW_HIDE);
				if (LoadOptionsClass().Load()) {
					*retval = IDC_LOAD_GAME;
				} else {
					ShowWindow(window, SW_SHOW);
					UpdateWindow(window);
				}
			}
			break;

		case IDC_BRIEFING:
			if (!code) {
				*retval = IDC_BRIEFING;
			}
			break;

		case IDC_GAME_CONTROLS:
			if (!code) {
				SpecialDialog = SDLG_SETTINGS;
				*retval = IDOK;
			}
			break;

		case IDC_ABORT_MISSION:
			if (!code) {
				if (Session.Type == GAME_INTERNET) {
					SpecialDialog = SDLG_SURRENDER;
					if (!WestwoodOnline_Tournament) {
						SpecialDialog = SDLG_ABORT;
					}
				} else {
					SpecialDialog = SDLG_ABORT;
				}
				*retval = IDCANCEL;
			}
			break;

		case IDC_RESUME_MISSION:
		case IDC_GOPTIONS_RESUME:
			if (!code) {
				if (Session.Type == GAME_INTERNET && !MuteSWLaunches) {
					handle = GetDlgItem(window, IDC_CTRLWOL_CONNECTION);
					if (handle) {
						int fudge = 3 - SendMessage(handle, TBM_GETPOS, 0, 0);
						if (fudge != Session.LatencyFudge) {
							if (!LatencyTimer) {
								LatencyTimer = 900;
								OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::LATENCYFUDGE, fudge));
								DebugString("LATENCYFUDGE event created - %d\n", fudge);
							} else {
								DebugString("Ignoring latency change to avoid cheating\n");
							}
						}
					}
					handle = GetDlgItem(window, IDC_GAME_SPEED_SLIDER);
					if (handle) {
						int speed = (OptionsClass::MAX_SPEED_SETTING - 1) - SendMessage(handle, TBM_GETPOS, 0, 0);
						if (Options.GameSpeed != speed) {
							if (!SpeedTimer) {
								SpeedTimer = 900;
								OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::GAMESPEED, speed));
							} else {
								DebugString("Ignoring speed change to avoid cheating\n");
							}
						}
					}
				}
				*retval = IDOK;
			}
			break;

		default:
			break;
	}
	return(FALSE);
}


/// <summary>
/// Handles messages for the in game options dialog.
/// This routine offers every message to the owner draw system first. What is left it uses
/// to service the option buttons, and dragging the game speed or connection quality slider
/// updates the label beside it.
/// </summary>
/// <returns>Returns with TRUE if the owner draw system consumed the message.</returns>
BOOL CALLBACK Game_Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	static char const * GameConnectionQualityNames[] = {
		"TXT_WORST_CONNECTION",
		"TXT_POOR_CONNECTION",
		"TXT_GOOD_CONNECTION",
		"TXT_BEST_CONNECTION"
	};

	BOOL rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);

	HWND handle;

	if (rc) {
		return(rc);
	}

	switch (message) {

		case OD_SUBCLASSED:
			Game_Options_On_INITDIALOG(window);
			break;

		case WM_COMMAND:
			return(Game_Options_On_COMMAND(window, wparam));

		case WM_HSCROLL: {
			if (LOWORD(wparam) == SB_THUMBTRACK) {
				int pos = HIWORD(wparam);
				char const * textid;

				if ((HWND)lparam == GetDlgItem(window, IDC_GAME_SPEED_SLIDER)) {
					textid = Game_Speed_Labels[pos];
					handle = GetDlgItem(window, IDC_GAME_SPEED_LABEL);
				} else if ((HWND)lparam == GetDlgItem(window, IDC_CTRLWOL_CONNECTION)) {
					textid = GameConnectionQualityNames[pos];
					handle = GetDlgItem(window, IDC_SCROLL_SPEED_LABEL);
				} else {
					break;
				}

				if (handle) {
					SendMessage(handle, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString(textid, NULL, RA2_SOURCE, 196));
				}
			}
			break;
		}

		default:
			break;
	}

	return(FALSE);
}


/// <summary>
/// Prepares the controls of the game options dialog.
/// This routine is called when the dialog is created, and again whenever a save or delete
/// has changed what is on disk. Load and delete need saved games; an internet game primes
/// the latency and game speed sliders.
/// </summary>
void Game_Options_On_INITDIALOG(HWND window)
{
	HWND handle;

	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
		bool present = LoadOptionsClass().Files_Present();

		handle = GetDlgItem(window, IDC_LOAD_GAME);
		if (handle) {
			EnableWindow(handle, present);
		}

		handle = GetDlgItem(window, IDC_DELETE_GAME);
		if (handle) {
			EnableWindow(handle, present);
		}
	} else if (Session.Type == GAME_INTERNET && !MuteSWLaunches) {
		handle = GetDlgItem(window, IDC_CTRLWOL_CONNECTION);
		if (handle) {
			int fudge = 3 - Session.LatencyFudge;
			Slider_SetRange(handle, 0, 3);
			Slider_SetPos(handle, fudge);
		}

		handle = GetDlgItem(window, IDC_GAME_SPEED_SLIDER);
		if (handle) {
			Slider_SetRange(handle, 0, OptionsClass::MAX_SPEED_SETTING - 1);
			Slider_SetPos(handle, (OptionsClass::MAX_SPEED_SETTING - 1) - Options.GameSpeed);
		}
	}
}


/// <summary>
/// Displays the in game options dialog.
/// This routine is used by the special dialog handler when the player calls up the options
/// screen. Which dialog appears depends on the kind of game in progress. Game input stays
/// locked out for as long as the dialog is up, and if the player asked for the mission
/// briefing it is restated on the way out.
/// </summary>
int Abort_Dialog(void) // RA2: at 0x4DDF20, returns the choice
{
	int result = 0; // RA2: Abort_Dialog at 0x4DDF20 matches only so
	HWND dialog = OwnerDraw::Begin_Dialog(IDD_MISSION_ABORT, Abort_Dialog_Proc, 0); // RA2: as above
	if (dialog) {
		SetWindowLong(dialog, DWL_USER, (LONG)&result); // RA2: as above
		OwnerDraw::Display_Dialog(dialog);
		while (result == 0) { // RA2: as above
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				result = 1; // RA2: as above
			}
		}
		OwnerDraw::End_Dialog(dialog);
	}
	return(result); // RA2: as above
} // RA2: Abort_Dialog at 0x4DDF20 has no IgnoreInput, Scen, IsInputLocked and 2 more


/// <summary>
/// Handles messages for the in game options dialog.
/// This routine offers every message to the owner draw system first. What is left it uses
/// to service the option buttons -- save, load, delete, briefing, resume, abort and
/// settings -- either acting on them directly or noting the player's choice for
/// Abort_Dialog to deal with once the dialog comes down. Dragging the game speed or
/// connection quality slider updates the label beside it.
/// </summary>
/// <returns>Returns with TRUE if the owner draw system consumed the message.</returns>
BOOL CALLBACK Abort_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: at 0x4DDF90
{
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
	if (result) { // RA2: as above
		return(result); // RA2: as above
	}

	switch (message) {
		case WM_COMMAND: { // RA2: as above
			int * retval = (int *)GetWindowLong(window, DWL_USER); // RA2: as above
			if (HIWORD(wparam) == BN_CLICKED) { // RA2: as above
				switch (LOWORD(wparam)) { // RA2: as above
					case IDC_GOPTIONS_LEAVE: // RA2: as above
						*retval = LeaveResult;
						return(FALSE); // RA2: as above

					case IDOK: // RA2: as above
					case IDC_GOPTIONS_RESUME: // RA2: as above
						*retval = 1;
						return(FALSE); // RA2: as above

					case IDC_GOPTIONS_RESTART: // RA2: as above
						*retval = RestartResult;
						break;
				} // RA2: no HIWORD, GetWindowLong, LOWORD and 30 more (TibSun's use does not compile against RA2's declaration)
			}
			break;
		}

		case OD_SUBCLASSED: { // RA2: as above
			HWND leave = GetDlgItem(window, IDC_GOPTIONS_LEAVE); // RA2: as above
			SendMessage(leave, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString("GUI:Leave", NULL, RA2_SOURCE, 543)); // RA2: as above
			LeaveResult = 2; // RA2: as above

			HWND restart = GetDlgItem(window, IDC_GOPTIONS_RESTART); // RA2: as above
			RestartResult = 0; // RA2: as above
			if (Session.Type == GAME_NORMAL) { // RA2: as above
				SendMessage(restart, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString("GUI:Restart", NULL, RA2_SOURCE, 554)); // RA2: as above
				RestartResult = 5; // RA2: as above
			} else if (Session.Type == GAME_SKIRMISH) { // RA2: as above
				ShowWindow(restart, SW_HIDE); // RA2: as above
			} else if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) { // RA2: as above
				int humans = 0; // RA2: as above
				for (int index = 0; index < Houses.Count(); index++) { // RA2: as above
					if (Houses[index]->IsHuman && !Houses[index]->IsDefeated) { // RA2: as above
						humans++; // RA2: as above
					}
				}
				if (humans >= 2 && !PlayerPtr->IsDefeated) { // RA2: as above
					LeaveResult = 3; // RA2: as above
					SendMessage(restart, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString("GUI:Observe", NULL, RA2_SOURCE, 582)); // RA2: as above
					RestartResult = 4; // RA2: as above
				} else {
					ShowWindow(restart, SW_HIDE); // RA2: as above
				}
			}
			OwnerDraw::Set_Control_Result(leave, LeaveResult); // RA2: as above
			OwnerDraw::Set_Control_Result(restart, RestartResult); // RA2: as above
			return(FALSE); // RA2: as above
		}
	}

	return(FALSE);
}
