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

#include "house.h" // RA2: the live human count
#include "stringtable.h" // RA2: button captions


#define RA2_SOURCE "D:\\Ra2\\GOptions.CPP" // RA2: the source name LoadString records

/*
**	RA2's game options dialog controls (resource IDs; names chosen here).
*/
#define IDC_GOPTIONS_LEAVE		1737	// RA2: Leave / Observe
#define IDC_GOPTIONS_RESTART	1810	// RA2: Restart / Observe
#define IDC_GOPTIONS_RESUME		1670	// RA2: Resume

static int LeaveResult; // RA2: what the Leave button returns (0xA40E1C)
static int RestartResult; // RA2: what the Restart button returns (0xA40E20)


BOOL CALLBACK Abort_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: defined after the dialog


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
			if (HIWORD(wparam) == 0) { // RA2: as above
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
