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

/* $Header: /CounterStrike/LOADDLG.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LOADDLG.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Maria Legg, Joe Bostic, Bill Randolph                        *
 *                                                                                             *
 *                   Start Date : March 19, 1995                                               *
 *                                                                                             *
 *                  Last Update : June 25, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   LoadOptionsClass::LoadOptionsClass -- class constructor                                   *
 *   LoadOptionsClass::~LoadOptionsClass -- class destructor                                   *
 *   LoadOptionsClass::Process -- main processing routine                                      *
 *   LoadOptionsClass::Clear_List -- clears the list box & Files arrays                        *
 *   LoadOptionsClass::Fill_List -- fills the list box & GameNum arrays                        *
 *   LoadOptionsClass::Num_From_Ext -- clears the list box & GameNum arrays                    *
 *   LoadOptionsClass::Compare -- for qsort                                                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "loaddlg.h"

#include "campaign.h"
#include "cd.h"
#include "conquer.h"
#include "data.h"
#include "globals.h"
#include "houstype.h"
#include "init.h"
#include "language\language.h"
#include "msgbox.h"
#include "ownrdraw.h"
#include "saveload.h"
#include "savever.h"
#include "scenario.h"
#include "session.h"
#include "stringtable.h" // RA2: StringTable::LoadString, used by Load_File
#include "win.h"

namespace OwnerDraw { // RA2: the helpers Dialog calls
void __fastcall Set_Control_Disabled(HWND control, bool value); // RA2: 0x5E46C0, used by Dialog
void __fastcall Set_Dialog_Hidden(HWND control); // RA2: 0x5E62D0, as above
void __fastcall Clear_Dialog_Hidden(HWND control); // RA2: 0x5E6330, as above
} // RA2: as above
wchar_t * __fastcall wcstrim(wchar_t * buffer); // RA2: 0x6EEF80, as above


/***********************************************************************************************
 * LoadOptionsClass::LoadOptionsClass -- class constructor                                     *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      style      style for this load/save dialog (LOAD/SAVE/DELETE)                          *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
LoadOptionsClass::LoadOptionsClass(void) :
	Files(0,0),
	Style(NONE),
	Description(NULL),
	Callback(NULL)
{
	Style = NONE;
	Description = NULL;
	Callback = NULL;
	Extension = "SAV";
	MinSpaceRequired = 2048;
	Files.Clear();
}


/***********************************************************************************************
 * LoadOptionsClass::~LoadOptionsClass -- class destructor                                     *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
LoadOptionsClass::~LoadOptionsClass(void)
{
	for (int i = 0; i < Files.Count(); i++) {
		delete Files[i];
	}
	Files.Clear();
}


/// <summary>
/// Brings up the load game dialog.
/// This routine is used by the options menu to let the player pick a saved game and
/// resume it.
/// </summary>
/// <returns>bool; Was a game loaded?</returns>
bool LoadOptionsClass::Load(void)
{
	Style = LOAD;
	Description = NULL;
	return(Dialog());
}


/// <summary>
/// Brings up the save game dialog.
/// This routine is used by the options menu when the player wants to record the current
/// game. The description offered is used to prime the edit field.
/// </summary>
/// <param name="description">The description to suggest for the saved game.</param>
/// <returns>bool; Was the game saved?</returns>
bool LoadOptionsClass::Save(wchar_t const * description) // RA2: a wide description
{
	Style = SAVE;
	if (description == NULL) {
		description = Scen->Description;
	}
	Description = (wchar_t *)description;
	return(Dialog());
}


/// <summary>
/// Brings up the delete game dialog.
/// This routine is used by the options menu to let the player clear out save games that
/// are no longer wanted.
/// </summary>
/// <returns>bool; Did the player go through with the deletion?</returns>
bool LoadOptionsClass::Delete(void)
{
	Style = WWDELETE;
	Description = NULL;
	return(Dialog());
}


/// <summary>
/// Handles a control notification from the load game dialog.
/// This routine records how the player left the dialog, so that the processing loop
/// knows whether a game was chosen or the player backed out.
/// </summary>
/// <param name="wparam">The identifier of the control that was activated.</param>
/// <param name="lparam">Window handle of the control that was activated.</param>
/// <param name="id">The notification code that accompanied the control.</param>
void LoadOptionsClass::Load_Dialog_On_WM_COMMAND(HWND window, WPARAM wparam, LPARAM lparam, int id)
{
	LoadOptionsClass * _this = (LoadOptionsClass *)GetWindowLong(window, DWL_USER);
	switch ((int)wparam) {
		case 0x525: // RA2: Load_Dialog_On_WM_COMMAND at 0x53B200 matches only so
			if (id == 2 && ListBox_GetCount((HWND)lparam) > 0) {
				_this->State = (LoadDialogState)0x40F; // RA2: as above
			}
			break;

		case 0x40F: // RA2: as above
			if (id == 0) {
				_this->State = (LoadDialogState)wparam;
			}
			break;

		case 0x686: // RA2: as above
			_this->State = (LoadDialogState)STATE_CLOSE; // RA2: as above
			break;
	}
}


/// <summary>
/// Handles a control notification from the save game dialog.
/// Picking a game in the list copies its description into the edit field, so that the
/// player can save over an existing game without typing the name out again. The buttons
/// record how the player left the dialog.
/// </summary>
/// <param name="wparam">The identifier of the control that was activated.</param>
/// <param name="lparam">Window handle of the control that was activated.</param>
/// <param name="id">The notification code that accompanied the control.</param>
void LoadOptionsClass::Save_Dialog_On_WM_COMMAND(HWND window, WPARAM wparam, LPARAM lparam, int id)
{
	LoadOptionsClass * _this = (LoadOptionsClass *)GetWindowLong(window, DWL_USER);
	switch ((int)wparam) {
		case 0x527: // RA2: Save_Dialog_On_WM_COMMAND at 0x53B280 matches only so
			if (id == 1 && ListBox_GetCount((HWND)lparam) > 0) {
				HWND list = (HWND)lparam; // RA2: as above
				int row = ListBox_GetCurSel(list); // RA2: as above
				if (row != LB_ERR) {
					FileEntryClass * fdata = (FileEntryClass *)ListBox_GetItemData(list, row); // RA2: as above
					if (!fdata->Valid || fdata->Old) { // RA2: as above
						wchar_t * text = _this->Description; // RA2: as above
						if (text != NULL) { // RA2: as above
							HWND edit = GetDlgItem(window, 0x526); // RA2: as above
							SendMessage(edit, 0x4B2, 0, (LPARAM)text); // RA2: as above
						}
					} else {
						HWND edit = GetDlgItem(window, 0x526); // RA2: as above
						SendMessage(edit, 0x4B2, 0, (LPARAM)fdata->Descr); // RA2: as above
					}
					SetFocus(GetDlgItem(window, 0x526)); // RA2: as above
					Edit_SetSel(GetDlgItem(window, 0x526), 0, -1); // RA2: as above
				}
			}
			break;

		case 0x686: // RA2: as above
			if (id == 0) {
				_this->State = (LoadDialogState)STATE_CLOSE; // RA2: as above
			}
			break;

		case 0x6C7: // RA2: as above
			if (id == 0) { // RA2: as above
				_this->State = (LoadDialogState)STATE_OK; // RA2: as above
			}
			break;
	}
}


/// <summary>
/// Handles a control notification from the delete game dialog.
/// This routine records how the player left the dialog, so that the processing loop
/// knows whether to go ahead with the deletion.
/// </summary>
/// <param name="wparam">The identifier of the control that was activated.</param>
/// <param name="id">The notification code that accompanied the control.</param>
void LoadOptionsClass::Delete_Dialog_On_WM_COMMAND(HWND window, WPARAM wparam, LPARAM lparam, int id)
{
	LoadOptionsClass * _this = (LoadOptionsClass *)GetWindowLong(window, DWL_USER);
	switch ((int)wparam) {
		case 0x686: // RA2: Delete_Dialog_On_WM_COMMAND at 0x53B390 matches only so
			if (id == 0) {
				_this->State = (LoadDialogState)STATE_CLOSE; // RA2: as above
			}
			break;

		case 0x6C8: // RA2: as above
			if (id == 0) { // RA2: as above
				_this->State = (LoadDialogState)STATE_OK; // RA2: as above
			}
			break;
	}
}


/// <summary>
/// Handles messages for the load game dialog.
/// The owner draw system is given first refusal on every message. What is left over is
/// used to set up the file list columns and to pass control activity along to the
/// command handler.
/// </summary>
/// <returns>Returns with the message result, or FALSE if nothing here dealt with it.</returns>
LRESULT CALLBACK LoadOptionsClass::Load_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);

	if (rc != 0) { // RA2: RA2's form
		return rc; // RA2: as above
	}
	switch (message) { // RA2: as above
	case 0x0f: // RA2: as above
		Slide_Pending_Dialog(window); // RA2: as above
		return(1); // RA2: as above
	case 0x111: // RA2: as above
		Load_Dialog_On_WM_COMMAND(window, (unsigned short)wparam, lparam, (unsigned short)(wparam >> 16)); // RA2: as above
		break; // RA2: as above
	case 0x216: // RA2: as above
		return On_WM_MOVING(window, wparam, lparam); // RA2: as above
	case 0x497: // RA2: as above
		SendDlgItemMessageA(window, 0x525, 0x4a6, 0xf9, 2); // RA2: as above
		SendDlgItemMessageA(window, 0x525, 0x4a6, 0x38, 0xff); // RA2: as above
		SendDlgItemMessageA(window, 0x525, 0x4a6, 0, 0x13b); // RA2: as above
		break; // RA2: as above
	}
	return(0); // RA2: as above
}



/// <summary>
/// Handles messages for the save game dialog.
/// The owner draw system is given first refusal on every message. What is left over is
/// used to set up the file list columns, cap the length of the description the player
/// may type, and pass control activity along to the command handler.
/// </summary>
/// <returns>Returns with the message result, or FALSE if nothing here dealt with it.</returns>
LRESULT __stdcall LoadOptionsClass::Save_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: RA2's form
{
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
	if (result != 0) { // RA2: as above
		return result; // RA2: as above
	}
	switch (message) { // RA2: as above
	case WM_PAINT: // RA2: as above
		Slide_Pending_Dialog(window); // RA2: as above
		return(1); // RA2: as above
	case WM_COMMAND: // RA2: as above
		Save_Dialog_On_WM_COMMAND(window, LOWORD(wparam), lparam, HIWORD(wparam)); // RA2: as above
		break; // RA2: as above
	case WM_MOVING: // RA2: as above
		return On_WM_MOVING(window, wparam, lparam); // RA2: as above
	case OD_SUBCLASSED: // RA2: as above
		SendMessageA(GetDlgItem(window, IDC_MISSION_SAVE_DESC), EM_SETLIMITTEXT, 79, 0); // RA2: as above
		SendDlgItemMessageA(window, IDC_MISSION_SAVE_LIST, OD_ADDCOLUMN, 0xf9, 2); // RA2: as above
		SendDlgItemMessageA(window, IDC_MISSION_SAVE_LIST, OD_ADDCOLUMN, 0x38, 255); // RA2: as above
		SendDlgItemMessageA(window, IDC_MISSION_SAVE_LIST, OD_ADDCOLUMN, 0, 315); // RA2: as above
		break; // RA2: as above
	case 0x4da: // RA2: as above
		((LoadOptionsClass *)GetWindowLongA(window, 8))->State = STATE_OK; // RA2: as above
		break; // RA2: as above
	}
	return(0); // RA2: as above
}



/// <summary>
/// Handles messages for the delete game dialog.
/// The owner draw system is given first refusal on every message. What is left over is
/// used to set up the file list columns and to pass control activity along to the
/// command handler.
/// </summary>
/// <returns>Returns with the message result, or FALSE if nothing here dealt with it.</returns>
LRESULT CALLBACK LoadOptionsClass::Delete_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: RA2's form
	if (result != 0) { // RA2: as above
		return result; // RA2: as above
	}
	switch (message) { // RA2: as above
	case WM_PAINT: // RA2: as above
		Slide_Pending_Dialog(window); // RA2: as above
		return(1); // RA2: as above
	case WM_COMMAND: // RA2: as above
		Delete_Dialog_On_WM_COMMAND(window, (unsigned short)wparam, lparam, (unsigned short)(wparam >> 16)); // RA2: as above
		break; // RA2: as above
	case WM_MOVING: // RA2: as above
		return On_WM_MOVING(window, wparam, lparam); // RA2: as above
	case OD_SUBCLASSED: // RA2: as above
		SendDlgItemMessage(window, IDC_MISSION_DELETE_LIST, OD_ADDCOLUMN, 0xf9, 2); // RA2: as above
		SendDlgItemMessage(window, IDC_MISSION_DELETE_LIST, OD_ADDCOLUMN, 0x38, 0xff); // RA2: as above
		SendDlgItemMessage(window, IDC_MISSION_DELETE_LIST, OD_ADDCOLUMN, 0, 0x13b); // RA2: as above
		break; // RA2: as above
	}
	return(0); // RA2: as above
}



/***********************************************************************************************
 * LoadOptionsClass::Process -- main processing routine                                        *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      false = User cancelled, true = operation completed                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
bool LoadOptionsClass::Dialog(void) // RA2: as above
{ // RA2: as above
	HWND dialog = 0; // RA2: as above
	HWND list = 0; // RA2: as above

	wchar_t buffer[256]; // RA2: the description and message texts are wide (meastes 0x53B770)
	char filename_buffer[256]; // RA2: the generated file name is kept in its own buffer

	switch (Style) { // RA2: as above
		case LOAD: { // RA2: as above
			dialog = OwnerDraw::Begin_Dialog(0xB7, (DLGPROC)Load_Dialog_Proc, 0); // RA2: dialog 0xB7 (IDD_MISSION_LOAD in TibSun)
			list = GetDlgItem(dialog, 0x525); // RA2: as above
			HWND title = GetDlgItem(dialog, 0x694); // RA2: the title control's caption comes from the virtual Load_Caption (slot 5)
			SendMessageA(title, OD_SETTEXTW, 0, (LPARAM)Load_Caption()); // RA2: as above
			if (!Session.Is_In_Game()) { // RA2: the button 0x40C is hidden outside a running game
				HWND button = GetDlgItem(dialog, 0x40C); // RA2: as above
				ShowWindow(button, SW_HIDE); // RA2: as above
			} // RA2: as above
			break; // RA2: as above
		} // RA2: as above

		case SAVE: { // RA2: as above
			if (Disk_Space_Available() < MinSpaceRequired) { // RA2: as above
				WWMessageBox().Process(StringTable::LoadString("TXT_DISKFULL", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x1AD), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x1AD), NULL, NULL, false); // RA2: wide message and button texts, line 0x1AD of the source
				return(false); // RA2: as above
			} // RA2: as above
			dialog = OwnerDraw::Begin_Dialog(0x2B4, (DLGPROC)Save_Dialog_Proc, 0); // RA2: dialog 0x2B4 (IDD_MISSION_SAVE in TibSun)
			list = GetDlgItem(dialog, 0x527); // RA2: as above
			wchar_t * description = Description; // RA2: the suggested description is put into the edit control first
			HWND edit = GetDlgItem(dialog, 0x526); // RA2: as above
			SendMessageA(edit, OD_SETTEXTW, 0, (LPARAM)description); // RA2: as above
			HWND title = GetDlgItem(dialog, 0x694); // RA2: the title is the virtual Save_Caption (slot 6)
			SendMessageA(title, OD_SETTEXTW, 0, (LPARAM)Save_Caption()); // RA2: as above
			if (!Session.Is_In_Game()) { // RA2: as in the load case
				HWND button = GetDlgItem(dialog, 0x40C); // RA2: as above
				ShowWindow(button, SW_HIDE); // RA2: as above
			} // RA2: as above
			break; // RA2: as above
		} // RA2: as above

		case WWDELETE: { // RA2: as above
			dialog = OwnerDraw::Begin_Dialog(0x2B5, (DLGPROC)Delete_Dialog_Proc, 0); // RA2: dialog 0x2B5 (IDD_MISSION_DELETE in TibSun)
			list = GetDlgItem(dialog, 0x528); // RA2: as above
			HWND title = GetDlgItem(dialog, 0x694); // RA2: the title is the virtual Delete_Caption (slot 7)
			SendMessageA(title, OD_SETTEXTW, 0, (LPARAM)Delete_Caption()); // RA2: as above
			if (!Session.Is_In_Game()) { // RA2: as in the load case
				HWND button = GetDlgItem(dialog, 0x40C); // RA2: as above
				ShowWindow(button, SW_HIDE); // RA2: as above
			} // RA2: as above
			break; // RA2: as above
		} // RA2: as above
	} // RA2: as above

	State = STATE_PENDING; // RA2: as above

	if (dialog) { // RA2: as above
		SetWindowLong(dialog, DWL_USER, (LONG)this); // RA2: as above

		if (list != 0) { // RA2: as above
			Fill_List(list); // RA2: as above
			EnableWindow(GetDlgItem(dialog, 1), bool(ListBox_GetCount(list) > 0)); // RA2: as above
		} // RA2: as above

		OwnerDraw::Display_Dialog(dialog); // RA2: as above

		do { // RA2: as above
			while (State == STATE_PENDING) { // RA2: as above
				if (OwnerDraw::Dialog_Message_Handler() == true) { // RA2: as above
					State = STATE_CLOSE; // RA2: as above
				} // RA2: as above

				if (Callback) { // RA2: as above
					Callback(); // RA2: as above
				} // RA2: as above

				if (!GameActive) { // RA2: as above
					Title_Screen_Restore(false); // RA2: as above
				} // RA2: as above
			} // RA2: as above

			if (State == STATE_OK || State == 0x40F) { // RA2: 0x40F, the list's double click, also confirms
				LRESULT row = ListBox_GetCurSel(list); // RA2: as above

				if (row == LB_ERR) { // RA2: as above
					State = STATE_PENDING; // RA2: no selection keeps the dialog up
				} else { // RA2: as above
					FileEntryClass * entry = (FileEntryClass *)ListBox_GetItemData(list, row); // RA2: as above

					switch (Style) { // RA2: as above
						case LOAD: // RA2: as above
							if (!entry->Old) { // RA2: an old entry (meastes NeedsDescription, +0x1B5) is not loaded
								DiskID old_disk = CD::Get_Current_Disk(60); // RA2: the current disk is remembered
								CD::Set_Required_Disk(entry->Num == -1 ? DISK_ANY : (DiskID)Campaigns[entry->Num]->CDNumber); // RA2: no Init_Campaigns
								if (!CD::Force_Available()) { // RA2: as above
									CD::Set_Required_Disk(old_disk); // RA2: the remembered disk is required again
									State = STATE_PENDING; // RA2: as above
								} else { // RA2: as above
									ShowWindow(dialog, SW_HIDE); // RA2: as above
									MouseCursor->Hide_Mouse(); // RA2: the mouse is hidden, no UpdateWindow / IsOldSaveGame
									if (!Load_File(entry->Filename)) { // RA2: as above
										MouseCursor->Show_Mouse(); // RA2: as above
										ShowWindow(dialog, SW_SHOW); // RA2: as above
										WWMessageBox().Process(StringTable::LoadString("TXT_ERROR_LOADING_GAME", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x218), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x218), NULL, NULL, false); // RA2: wide texts, line 0x218
										ShowWindow(dialog, SW_HIDE); // RA2: the dialog is hidden again after the message
										State = STATE_PENDING; // RA2: as above
									} // RA2: as above
								} // RA2: as above
							} // RA2: as above
							break; // RA2: as above

						case SAVE: { // RA2: as above
							HWND item = GetDlgItem(dialog, 0x527); // RA2: the list's attached control is disabled while the message boxes show
							HWND disabled = 0; // RA2: as above
							if (item) { // RA2: as above
								disabled = (HWND)SendMessageA(item, 0x4EF, 0, 0); // RA2: 0x4EF returns the attached control
								if (disabled) { // RA2: as above
									OwnerDraw::Set_Control_Disabled(disabled, true); // RA2: as above
								} // RA2: as above
							} // RA2: as above
							HWND edit = GetDlgItem(dialog, 0x526); // RA2: the description is read as wide text
							SendMessageA(edit, 0x4B3, 0x50, (LPARAM)buffer); // RA2: 0x4B3 reads the wide text of the control (at most 0x50 characters)
							wcstrim(buffer); // RA2: 0x6EEF80 trims it

							if (wcslen(buffer) == 0) { // RA2: as above
								WWMessageBox().Process(StringTable::LoadString("TXT_MUSTENTER_DESCRIPTION", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x230), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x230), NULL, NULL, false); // RA2: wide texts, line 0x230
								SetFocus(GetDlgItem(dialog, 0x526)); // RA2: as above
								Edit_SetSel(GetDlgItem(dialog, 0x526), -1, -1); // RA2: as above
								State = STATE_PENDING; // RA2: as above
								break; // RA2: as above
							} // RA2: as above

							const char * filename = NULL; // RA2: as above

							if (entry && entry->Valid) { // RA2: as above
								filename = entry->Filename; // RA2: as above
							} else { // RA2: as above
								{ // RA2: as above
									CCFileClass temp_file; // RA2: as above
									do { // RA2: as above
										sprintf(filename_buffer, "SAVE%04lX.%3s", rand(), Extension); // RA2: as above
										temp_file.Set_Name(filename_buffer); // RA2: as above
									} while (temp_file.Is_Available() == true); // RA2: as above
								} // RA2: as above

								filename = filename_buffer; // RA2: as above
							} // RA2: as above

							if (filename != NULL) { // RA2: as above
								bool exists = RawFileClass(filename).Is_Available() == true; // RA2: as above
								if (exists && WWMessageBox().Process(StringTable::LoadString("TXT_CONFIRM_SAVE", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x244), StringTable::LoadString("TXT_YES", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x244), StringTable::LoadString("TXT_NO", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x244), NULL, false)) { // RA2: wide texts, line 0x244
									State = STATE_PENDING; // RA2: as above
								} else { // RA2: as above
									ShowWindow(dialog, SW_HIDE); // RA2: the dialog is hidden while the game is saved
									if (!Save_File(filename, buffer)) { // RA2: the wide description is passed
										ShowWindow(dialog, SW_SHOW); // RA2: as above
										WWMessageBox().Process(StringTable::LoadString("TXT_ERROR_SAVING_GAME", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x253), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x253), NULL, NULL, false); // RA2: wide texts, line 0x253
										State = STATE_PENDING; // RA2: as above
									} else { // RA2: as above
										ShowWindow(dialog, SW_SHOW); // RA2: the success message is shown over the dialog
										OwnerDraw::Clear_Dialog_Hidden(dialog); // RA2: as above
										wchar_t const * message = Saved_Message(); // RA2: the virtual Saved_Message (slot 8)
										WWMessageBox().Process(message, StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x25B), NULL, NULL, false); // RA2: line 0x25B
										OwnerDraw::Set_Dialog_Hidden(dialog); // RA2: as above
										ShowWindow(dialog, SW_HIDE); // RA2: as above
										if (Description) { // RA2: the caller's description buffer is copied from the buffer
											wcscpy(buffer, Description); // RA2: as above
										} // RA2: as above
									} // RA2: as above
								} // RA2: as above
							} // RA2: as above

							if (disabled) { // RA2: the attached control is enabled again
								OwnerDraw::Set_Control_Disabled(disabled, false); // RA2: as above
							} // RA2: as above
							break; // RA2: as above
						} // RA2: as above

						case WWDELETE: { // RA2: as above
							swprintf(buffer, L"%s\n\n%s", StringTable::LoadString("TXT_DELETE_FILE_QUERY", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x26D), entry->Descr); // RA2: wide text with the entry's wide description, line 0x26D
							HWND item = GetDlgItem(dialog, 0x528); // RA2: the list's attached control is disabled while the message box shows
							HWND disabled = 0; // RA2: as above
							if (item) { // RA2: as above
								disabled = (HWND)SendMessageA(item, 0x4EF, 0, 0); // RA2: as above
								if (disabled) { // RA2: as above
									OwnerDraw::Set_Control_Disabled(disabled, true); // RA2: as above
								} // RA2: as above
							} // RA2: as above

							if (!WWMessageBox().Process(buffer, StringTable::LoadString("TXT_YES", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x278), StringTable::LoadString("TXT_NO", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x278), NULL, false)) { // RA2: wide texts, line 0x278
								Delete_File(entry->Filename); // RA2: as above
								ListBox_DeleteString(list, row); // RA2: as above
								ListBox_SetCurSel(list, 0); // RA2: as above
								if (ListBox_GetCount(list) > 0) { // RA2: as above
									State = STATE_PENDING; // RA2: as above
								} // RA2: as above
							} else { // RA2: as above
								State = STATE_PENDING; // RA2: as above
							} // RA2: as above

							if (disabled) { // RA2: the attached control is enabled again
								OwnerDraw::Set_Control_Disabled(disabled, false); // RA2: as above
							} // RA2: as above
							break; // RA2: as above
						} // RA2: as above
					} // RA2: as above
				} // RA2: as above
			} // RA2: as above
		} while (State == STATE_PENDING); // RA2: as above

		Clear_List(); // RA2: as above

		OwnerDraw::End_Dialog(dialog); // RA2: as above
	} // RA2: as above

	return(State == 0x40F ? true : false); // RA2: only the double click returns true
}



/// <summary>
/// Fetches a save game filename that is not already in use.
/// This routine is used when the player saves into an empty slot and there is no
/// existing file to write over.
/// </summary>
/// <param name="name">Buffer to fill in with the filename chosen.</param>
/// <remarks>Be sure the buffer is big enough to hold a complete filename.</remarks>
void LoadOptionsClass::Pick_Filename(char * file_name) // RA2: Pick_Filename at 0x53BF90 matches only so
{
	CCFileClass file;
	do {
		sprintf(file_name, "SAVE%04lX.%3s", rand(), Extension); // RA2: as above
		file.Set_Name(file_name); // RA2: as above
	} while (file.Is_Available() == true);
}


/***********************************************************************************************
 * LoadOptionsClass::Clear_List -- clears the list box & Files arrays                          *
 *                                                                                             *
 * This step is essential, because it frees all the strings allocated for list items.          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void LoadOptionsClass::Clear_List(void)
{
	for (int i = 0; i < Files.Count(); i++) {
		delete Files[i];
	}
	Files.Clear();
}


/***********************************************************************************************
 * LoadOptionsClass::Fill_List -- fills the list box & GameNum arrays                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *   06/25/1995 JLB : Shows which saved games are "(old)".                                     *
 *=============================================================================================*/
void LoadOptionsClass::Fill_List(HWND window) // RA2: as above
{ // RA2: as above
	OwnerDraw::CellData thecell; // RA2: a cell with wide text, set per column below (meastes 0x53C040)
	SYSTEMTIME time; // RA2: declared at the top
	FILETIME ft; // RA2: as above
	char buffer[128]; // RA2: as above
	wchar_t wide[128]; // RA2: the date and time texts are converted to wide text
	WIN32_FIND_DATAA ff; // RA2: as above

	Clear_List(); // RA2: as above

	if (Style == SAVE) { // RA2: as above
		FileEntryClass * fdata = new FileEntryClass; // RA2: as above
		swprintf(fdata->Descr, StringTable::LoadString("TXT_EMPTY_SLOT", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x2F8)); // RA2: the empty slot's description is a string table entry, line 0x2F8 of the source
		if (PlayerPtr != NULL) { // RA2: as above
			fdata->Scenario = Scen->Scenario; // RA2: as above
			fdata->House = Scen->PlayerHouse; // RA2: as above
			fdata->Num = Scen->Campaign; // RA2: as above
			wcscpy(fdata->PlayerName, PlayerPtr->Class->UIName); // RA2: the player's name is the house's UI name
		} else { // RA2: as above
			fdata->Scenario = 0; // RA2: as above
			fdata->House = (HousesType)Session.House; // RA2: as above
			fdata->Num = -1; // RA2: as above
			swprintf(fdata->PlayerName, L"%s", Narrow_To_Wide(Session.Handle)); // RA2: the player's name is the session's handle in wide text
		} // RA2: as above
		GetSystemTime(&time); // RA2: as above
		SystemTimeToFileTime(&time, &fdata->DateTime); // RA2: as above
		int game_type = Session.Type; // RA2: read into a local before the flag is set
		fdata->Valid = false; // RA2: as above
		fdata->Type = (GameType)game_type; // RA2: as above
		Files.Add(fdata); // RA2: as above
	} // RA2: as above

	sprintf(buffer, "*.%3s", Extension); // RA2: as above
	HANDLE hFind = FindFirstFileA(buffer, &ff); // RA2: as above
	FileEntryClass * fdata = NULL; // RA2: as above

	if (hFind != INVALID_HANDLE_VALUE) { // RA2: as above
		do { // RA2: as above
			if ((ff.dwFileAttributes & 0x116) == 0) { // RA2: as above
				if (fdata == NULL) { // RA2: as above
					fdata = new FileEntryClass; // RA2: as above
				} // RA2: as above
				if (Read_File(fdata, &ff) == true) { // RA2: as above
					Files.Add(fdata); // RA2: as above
					fdata = NULL; // RA2: as above
				} // RA2: as above
			} // RA2: as above
		} while (FindNextFileA(hFind, &ff)); // RA2: as above

		if (fdata != NULL) { // RA2: as above
			delete fdata; // RA2: as above
		} // RA2: as above
		FindClose(hFind); // RA2: as above
	} // RA2: as above

	if (Files.Count() > 0) { // RA2: as above
		qsort(&Files[0], Files.Count(), sizeof(FileEntryClass *), LoadOptionsClass::Compare); // RA2: as above

		SendMessageA(window, LB_RESETCONTENT, 0, 0); // RA2: as above

		for (int i = 0; i < Files.Count(); i++) { // RA2: as above
			FileEntryClass * file = Files[i]; // RA2: as above
			if (!file->Old) { // RA2: only entries not marked old are added (meastes NeedsDescription, +0x1B5)
				int row = SendMessageA(window, OD_ADDSTRINGW, 0, (LPARAM)file); // RA2: as above
				if (file->Type != GAME_NORMAL) { // RA2: as above
					thecell.type = OwnerDraw::CellData::TEXT; // RA2: as above
					thecell.string.set(L"*"); // RA2: an asterisk marks a non-normal game
					SendMessageA(window, OD_SETCELL, MAKEWPARAM(200, row), (LPARAM)&thecell); // RA2: as above
				} // RA2: as above

				if (file->DateTime.dwHighDateTime != -1 && file->DateTime.dwLowDateTime != -1) { // RA2: as above
					FileTimeToLocalFileTime(&file->DateTime, &ft); // RA2: as above
					FileTimeToSystemTime(&ft, &time); // RA2: as above
					GetDateFormatA(LOCALE_USER_DEFAULT, TIME_NOMINUTESORSECONDS, &time, NULL, buffer, sizeof(buffer)); // RA2: as above
					thecell.type = OwnerDraw::CellData::TEXT; // RA2: as above
					int length = MultiByteToWideChar(CP_ACP, 0, buffer, -1, wide, 128); // RA2: the date text is converted to wide text
					wide[127] = 0; // RA2: as above
					wide[length] = 0; // RA2: as above
					thecell.string.set(wide); // RA2: as above
					SendMessageA(window, OD_SETCELL, MAKEWPARAM(255, row), (LPARAM)&thecell); // RA2: as above
					GetTimeFormatA(LOCALE_USER_DEFAULT, 0, &time, NULL, buffer, sizeof(buffer)); // RA2: flags 0 (seconds shown)
					thecell.type = OwnerDraw::CellData::TEXT; // RA2: as above
					length = MultiByteToWideChar(CP_ACP, 0, buffer, -1, wide, 128); // RA2: the time text is converted to wide text
					wide[127] = 0; // RA2: as above
					wide[length] = 0; // RA2: as above
					thecell.string.set(wide); // RA2: as above
					SendMessageA(window, OD_SETCELL, MAKEWPARAM(315, row), (LPARAM)&thecell); // RA2: as above
				} // RA2: as above

				SendMessageA(window, LB_SETITEMDATA, row, (LPARAM)file); // RA2: as above
			} // RA2: as above
		} // RA2: as above

		switch (Style) { // RA2: as above
			case LOAD: { // RA2: as above
				for (int i = 0; i < Files.Count(); i++) { // RA2: as above
					if (Files[i]->Valid && !Files[i]->Old) { // RA2: an old entry is skipped too
						SendMessageA(window, LB_SETCURSEL, i, 0); // RA2: as above
						SendMessageA(window, LB_SETTOPINDEX, i, 0); // RA2: as above
						break; // RA2: as above
					} // RA2: as above
				} // RA2: as above
				} // RA2: as above
				break; // RA2: as above

			case SAVE: // RA2: as above
			case WWDELETE: // RA2: as above
				SendMessageA(window, LB_SETCURSEL, 0, 0); // RA2: as above
				SendMessageA(window, LB_SETTOPINDEX, 0, 0); // RA2: as above
				break; // RA2: as above
		} // RA2: as above
	} // RA2: as above
}



/// <summary>
/// Are there any save games available to load?
/// This routine is used to decide whether the load option should be offered to the
/// player at all. It settles the question as cheaply as it can, so it stops at the
/// first save game it can actually read.
/// </summary>
/// <returns>bool; Was at least one loadable save game found?</returns>
bool LoadOptionsClass::Files_Present(void)
{
	char pattern[64];
	WIN32_FIND_DATA find; // RA2: RA2's form
	bool found = false; // RA2: as above
	sprintf(pattern, "*.%3s", Extension);
	HANDLE handle = FindFirstFileA(pattern, &find); // RA2: as above
	if (handle != INVALID_HANDLE_VALUE) { // RA2: as above
		do { // RA2: as above
			if (!(find.dwFileAttributes & 0x116) && _stricmp(find.cFileName, "SAVEGAME.NET") != 0) { // RA2: as above
				found = true; // RA2: as above
				break;
			}
		} while (FindNextFileA(handle, &find)); // RA2: as above
	}
	FindClose(handle); // RA2: as above
	return found; // RA2: as above
}



/***********************************************************************************************
 * LoadOptionsClass::Compare -- for qsort                                                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      p1,p2      ptrs to elements to compare                                                 *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      0 = same, -1 = (*p1) goes BEFORE (*p2), 1 = (*p1) goes AFTER (*p2)                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *=============================================================================================*/
int __cdecl LoadOptionsClass::Compare(const void * p1, const void * p2)
{
	FileEntryClass * fe1, * fe2;

	fe1 = *((FileEntryClass **)p1);
	fe2 = *((FileEntryClass **)p2);

	int res = CompareFileTime(&fe1->DateTime, &fe2->DateTime);
	return(-res);
}


/// <summary>
/// Restores the game held in the file specified.
/// A message box is displayed while the load runs, and the scenario is taken out of
/// play first so that nothing tries to tick while the game state is being replaced.
/// </summary>
/// <returns>bool; Was the game loaded?</returns>
bool LoadOptionsClass::Load_File(const char * file_name) // RA2: as above
{ // RA2: as above
	int mouse_state = MouseCursor->Get_Mouse_State(); // RA2: the mouse state is saved first (0x53C680)
	HWND dialog = OwnerDraw::Custom_Message_Box(StringTable::LoadString("TXT_LOADING", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x41C), NULL, NULL); // RA2: a message box with the TXT_LOADING text, line 0x41C of the source
	MouseCursor->Hide_Mouse(); // RA2: as above
	if (dialog) { // RA2: as above
		OwnerDraw::Display_Dialog(dialog); // RA2: as above
	} // RA2: as above
	ScenarioActive = false; // RA2: as above
	TacticalActive = false; // RA2: as above
	Session.Leave_Game(); // RA2: 0x672E70 is called before the load
	bool loaded = Load_Game(file_name); // RA2: as above
	while (MouseCursor->Get_Mouse_State() < mouse_state) { // RA2: the mouse state is restored by showing or hiding
		MouseCursor->Show_Mouse(); // RA2: as above
	} // RA2: as above
	while (MouseCursor->Get_Mouse_State() > mouse_state) { // RA2: as above
		MouseCursor->Hide_Mouse(); // RA2: as above
	} // RA2: as above
	if (dialog) { // RA2: as above
		OwnerDraw::End_Dialog(dialog); // RA2: as above
	} // RA2: as above
	return(loaded); // RA2: as above
}



/// <summary>
/// Saves the current game to the file specified.
/// A message box is displayed while the save runs, since writing a save game takes long
/// enough that the player would otherwise think the game had locked up.
/// </summary>
/// <param name="descr">The description to record alongside the saved game.</param>
/// <returns>bool; Was the game saved?</returns>
bool LoadOptionsClass::Save_File(const char * file_name, wchar_t const * descr) // RA2: a wide description
{ // RA2: as above
	HWND dialog = OwnerDraw::Custom_Message_Box(StringTable::LoadString("TXT_SAVING_GAME", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x453), NULL, NULL); // RA2: a message box with the TXT_SAVING_GAME text, line 0x453 of the source (0x53C760)
	MouseCursor->Hide_Mouse(); // RA2: as above
	if (dialog) { // RA2: as above
		OwnerDraw::Display_Dialog(dialog); // RA2: as above
	} // RA2: as above
	bool saved = Save_Game(file_name, descr, false); // RA2: as above
	MouseCursor->Show_Mouse(); // RA2: the mouse is shown again
	if (dialog) { // RA2: the message box is closed again
		OwnerDraw::End_Dialog(dialog); // RA2: as above
	} // RA2: as above
	return(saved); // RA2: as above
}



/// <summary>
/// Removes the save game file specified.
/// </summary>
/// <returns>bool; Was the file deleted?</returns>
bool LoadOptionsClass::Delete_File(const char * file_name)
{
	if (DeleteFile(file_name) == TRUE) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Fills in a save game list entry from a file found on disk.
/// This routine peeks at the save game's header to recover the description, scenario
/// and player it belongs to. A save written by an older game version is still accepted,
/// but its description is marked so the player can tell, and the network save file is
/// never offered.
/// </summary>
/// <param name="fdata">The list entry to fill in.</param>
/// <param name="ff">The find record naming the file to examine.</param>
/// <returns>bool; Was a usable save game found in the file?</returns>
bool LoadOptionsClass::Read_File(FileEntryClass * fdata, WIN32_FIND_DATAA * ff)
{
	if (fdata == NULL && ff == NULL) {
		return(false);
	}

	if (stricmp(ff->cFileName, NET_SAVE_FILE_NAME) != 0) {

		SaveVersionInfo savever;

		bool ok = Get_Savefile_Info(ff->cFileName, &savever);
		if (!ok) {
			return(false);
		}

		if (savever.Get_Internal_Version() != ExpectedGameVersion) { // RA2: Read_File at 0x53C7F0 matches only so
			wcscpy(fdata->Descr, savever.Get_Scenario_Description()); // RA2: as above
			fdata->Old = true;
		} else {
			wcscpy(fdata->Descr, savever.Get_Scenario_Description()); // RA2: as above
			fdata->Old = false;
		}

		fdata->Valid = ok;
		fdata->Scenario = savever.Get_Scenario_Number();
		fdata->Num = savever.Get_Campaign_Number();
		fdata->Type = (GameType)savever.Get_Game_Type();
		strcpy(fdata->Filename, ff->cFileName);
		wcscpy(fdata->PlayerName, savever.Get_Player_House()); // RA2: as above
		if (strlen(fdata->Filename) == 0) {
			strcpy(fdata->Filename, ff->cAlternateFileName);
		}
		fdata->DateTime.dwHighDateTime = ff->ftLastWriteTime.dwHighDateTime;
		fdata->DateTime.dwLowDateTime = ff->ftLastWriteTime.dwLowDateTime;
		return(true);
	}
	return(false);
}


// RA2: localized captions and the confirmation used by the load/save dialog.
wchar_t const * LoadOptionsClass::Load_Caption(void) const // RA2: 0x0053C970
{
	return StringTable::LoadString("GUI:LoadMissionMenu", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x4C0);
}

wchar_t const * LoadOptionsClass::Save_Caption(void) const // RA2: 0x0053C990
{
	return StringTable::LoadString("GUI:SaveMissionMenu", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x4C6);
}

wchar_t const * LoadOptionsClass::Delete_Caption(void) const // RA2: 0x0053C9B0
{
	return StringTable::LoadString("GUI:DeleteMissionMenu", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x4CC);
}

wchar_t const * LoadOptionsClass::Saved_Message(void) const // RA2: 0x0053C9D0
{
	return StringTable::LoadString("TXT_GAME_WAS_SAVED", NULL, "D:\\Ra2\\LoadDlg.CPP", 0x4D2);
}
