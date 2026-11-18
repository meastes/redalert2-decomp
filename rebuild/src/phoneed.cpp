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

#define GEOMETRY_H // RA2: this module has no initializer for the geometry constants of globals.h (nothing runs before its first function at 0x0060D970), so they stay undefined
#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "phoneed.h"

#include "data.h"
#include "dbgprint.h"
#include "init.h"
#include "language\language.h"
#include "ownrdraw.h"
#include "serialed.h"
#include "trim.h"
#undef LoadString // RA2: windows.h maps LoadString to LoadStringA
#include "stringtable.h" // RA2: the captions come from the string table

#undef Edit_SetText // RA2: the dialog's edit boxes are owner drawn and take the owner draw layer's own messages
#undef Edit_GetText // RA2: as above
#define Edit_SetText(h, s) ((int)SendMessage((h), 0x4b4, 0, (LPARAM)(s))) // RA2: 0x4B4 sets the text of an edit box as ANSI
#define Edit_GetText(h, s, n) ((int)SendMessage((h), 0x4b5, (n), (LPARAM)(s))) // RA2: 0x4B5 gets the text as ANSI
#define Edit_SetTextW(h, s) ((int)SendMessage((h), 0x4b2, 0, (LPARAM)(s))) // RA2: 0x4B2 sets the text as wide characters
#define Edit_GetTextW(h, s, n) ((int)SendMessage((h), 0x4b3, (n), (LPARAM)(s))) // RA2: 0x4B3 gets the text as wide characters
#define LS(name, line) StringTable::LoadString(name, NULL, "D:\\Ra2\\PhoneEd.cpp", line) // RA2: the string table lookup keeps its label and source line

wchar_t * __fastcall wcstrim(wchar_t * buffer); // RA2: 0x6EEF80, as in modemgst.cpp

static int _cdecl Phone_Compare (const void *p1, const void *p2);

class PhoneEntryEdit
{
		friend BOOL CALLBACK Edit_Phone_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	public:
		PhoneEntryEdit(void) {};
		bool Dialog(PhoneEntryClass *phone);

	private:
		void Save(void);
		void Init(void);

	private:
		HWND _Dialog;
		int RC;
		PhoneEntryClass * Phone;
		SerialSettingsType TempSettings;
};



BOOL CALLBACK Phone_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK Edit_Phone_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);


/// <summary>
/// Lets the user edit the phone directory and dial.
/// Serial options must have been read from the configuration beforehand. Any change to the
/// phone book is written back with the multiplayer settings when the dialog closes.
/// </summary>
/// <returns>bool; Was a phone book entry chosen to be dialed?</returns>
bool PhoneEdit::Dialog(void)
{
	_Dialog = OwnerDraw::Begin_Dialog(IDD_SERIAL_PHONE_LIST, (DLGPROC)Phone_Dialog_Proc);
	if (_Dialog) {

		Changed = false;
		RC = -1;
		SetWindowLong(_Dialog, DWL_USER, (LONG)this);

		/*........................................................................
		Fill in the phone directory list box
		........................................................................*/
		Build_Phone_Listbox();

		OwnerDraw::Display_Dialog(_Dialog);

		while (RC < 0) {
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				break;
			}
			Title_Screen_Restore();
		}

		OwnerDraw::End_Dialog(_Dialog);

		/*------------------------------------------------------------------------
		Save any changes we've made to the phone list or settings
		------------------------------------------------------------------------*/
		if (Changed == true) {
			Session.Write_MultiPlayer_Settings();
		}
	}
	if (RC != 1) {
		return(false);
	}
	return(true);
}


/// <summary>
/// Handles the messages for the phone directory dialog.
/// Anything the owner draw layer does not claim is dealt with here -- the list box columns
/// are set up, the add, edit and delete buttons are routed to the phone book, and picking a
/// listing copies its number into the edit box.
/// </summary>
/// <returns>Returns with the owner draw handler's result, or zero for a message this
/// routine dealt with itself.</returns>
BOOL CALLBACK Phone_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	HWND handle;

	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);

	if (rc == 0) {
		PhoneEdit * _this = (PhoneEdit *)GetWindowLong(window, DWL_USER);

		switch (message) {
		case OD_SUBCLASSED: // RA2: the executable has one case for the subclassed message and none for WM_INITDIALOG; the name limit is set here with the columns
			SendDlgItemMessage(window, IDC_PHONE_NAME, EM_SETLIMITTEXT, 20, 0);
			SendDlgItemMessage(window, IDC_PHONE_LIST, OD_ADDCOLUMN, 144, 2);
			SendDlgItemMessage(window, IDC_PHONE_LIST, OD_ADDCOLUMN, 121, 150);
			SendDlgItemMessage(window, IDC_PHONE_LIST, OD_ADDCOLUMN, 0, 275);
			break;

		case WM_COMMAND: {
			switch (LOWORD(wparam)) {
			case IDC_PHONE_EDIT:
				if (HIWORD(wparam) == 0) {
					_this->Edit();
				}
				break;

			case IDC_SERIAL_ADD:
				if (HIWORD(wparam) == 0) {
					_this->Add();
				}
				break;

			case IDC_PHONE_LIST:
				if (HIWORD(wparam) == LBN_SELCHANGE) {
					handle = GetDlgItem(_this->_Dialog, IDC_PHONE_LIST);
					if (handle != 0) {
						if (ListBox_GetCount(handle) > 0) {
							Session.CurPhoneIdx = ListBox_GetCurSel(handle);
							if (Session.CurPhoneIdx > -1) {
								handle = GetDlgItem(_this->_Dialog, IDC_PHONE_NAME);
								Edit_SetText(handle, Session.PhoneBook[Session.CurPhoneIdx]->Number);
								SetFocus(handle);
							}
						}
					}
				}
				break;

			case IDOK:
				if (HIWORD(wparam) == 0) {
					if (_this->Dial() == true) {
						_this->RC = IDOK;
					}
				}
				break;

			case IDCANCEL:
				if (HIWORD(wparam) == 0) {
					_this->RC = IDCANCEL;
				}
				break;

			case IDC_PHONE_DELETE:
				if (HIWORD(wparam) == 0) {
					_this->Delete(); // RA2: the executable calls the member here, where TibSun's procedure carried the code
				}
				break;
			}
		} break;
		}
		rc = 0;
	}

	return(rc);
}

/*------------------------------------------------------------------
Dial the current number
------------------------------------------------------------------*/

/// <summary>
/// Settles which number the dialer is about to call.
/// A number typed into the edit box that does not belong to any listing earns a phone book
/// entry of its own, so that whatever gets dialed can be dialed again later.
/// </summary>
/// <returns>bool; Is there a number worth dialing?</returns>
bool PhoneEdit::Dial(void)
{
	HWND handle = GetDlgItem(_Dialog, IDC_PHONE_NAME);
	Edit_GetText(handle, Number, sizeof(Number)); // RA2: the owner drawn edit box is read with its own message
	HWND handle2 = GetDlgItem(_Dialog, IDC_PHONE_LIST);
	Session.CurPhoneIdx = ListBox_GetCurSel(handle2);

	/*...............................................................
	If no item is selected, just dial the number in the phone #
	edit box:
	- Create a new phone entry
	- Copy the phone number into it
	- Set settings to defaults
	...............................................................*/
	if (Session.CurPhoneIdx == -1 ||
		strcmp( Session.PhoneBook[Session.CurPhoneIdx]->Number, Number) ) {

		if ( strlen(Number) == 0) {	// do not dial
			return(false);
		}

		PhoneEntryClass *p_entry = new PhoneEntryClass();
		if (p_entry == NULL) {
			return(false);
		}

		wcscpy( p_entry->Name, LS("TXT_NONAME", 231) ); // RA2: the name is a wide string from the string table
		strcpy( p_entry->Number, Number);
		p_entry->Settings.Port = 0;
		p_entry->Settings.IRQ = -1;
		p_entry->Settings.Baud = -1;
		p_entry->Settings.DialMethod = DIAL_TOUCH_TONE;
		p_entry->Settings.InitStringIndex = -1;
		p_entry->Settings.CallWaitStringIndex = CALL_WAIT_CUSTOM;
		p_entry->Settings.CallWaitString[0] = 0;

		Session.PhoneBook.Add (p_entry);
		Build_Phone_Listbox();
		Changed = true;

		/*............................................................
		Set the current listbox index to the newly-added item.
		............................................................*/
		Session.CurPhoneIdx = Session.PhoneBook.ID(p_entry);
	}
	return(true);
}


/// <summary>
/// Rebuilds the phone entry list box from scratch.
/// The phone book is sorted by name and then number first. Each entry becomes a row with its
/// name, number and baud rate, and the phone number edit box is refilled from the current
/// selection.
/// </summary>
void PhoneEdit::Build_Phone_Listbox(void)
{
	HWND handle;
	int i;
	wchar_t item[80]; // RA2: the cell texts are wide strings
	char number[80]; // RA2: the number stays narrow until it is formatted into the cell text

	OwnerDraw::CellData thecell;

	handle = GetDlgItem(_Dialog, IDC_PHONE_LIST);
	if (handle) {

		/*........................................................................
		Clear the list
		........................................................................*/
		ListBox_ResetContent(handle);

		/*
		**	Now sort the phone list by name then number
		*/
		if (Session.PhoneBook.Count() > 1) {
			qsort ((void *)(&Session.PhoneBook[0]), Session.PhoneBook.Count(), sizeof(class PhoneEntryClass *), Phone_Compare);
		}

		/*........................................................................
		Build the list
		........................................................................*/
		for (i = 0; i < Session.PhoneBook.Count(); i++) {
			if ( !(wcslen( Session.PhoneBook[i]->Name )) ) { // RA2: the name is wide
				wcscpy( item, L" " ); // RA2: the blank name is wide
			} else {
				wcsncpy( item, Session.PhoneBook[i]->Name, PhoneEntryClass::PHONE_MAX_NAME-1 ); // RA2: as above
				item[PhoneEntryClass::PHONE_MAX_NAME-1] = '\0';
			}

			int row = SendMessage(handle, OD_ADDSTRINGW, 0, (LPARAM)item); // RA2: the wide form of adding a row

			if ( !strlen( Session.PhoneBook[i]->Number ) ) {
				wcscpy( item, L" " ); // RA2: as above
			} else {
				strncpy( number, Session.PhoneBook[i]->Number, PhoneEntryClass::PHONE_MAX_NAME-1 ); // RA2: the number is narrow and widened by the format below
				number[PhoneEntryClass::PHONE_MAX_NAME-1] = '\0'; // RA2: as above
				swprintf(item, L"%hs", number); // RA2: as above
			}

			thecell.type = OwnerDraw::CellData::DataType::TEXT;
			thecell.string.set(item);

			SendMessage(handle, OD_SETCELL, MAKEWPARAM(150, row), (LPARAM)&thecell);

			if (Session.PhoneBook[i]->Settings.Baud != -1) {
				swprintf(item, L"%d", Session.PhoneBook[i]->Settings.Baud); // RA2: wide format
			} else {
				swprintf(item, L"[%s]", LS("TXT_DEFAULT", 321)); // RA2: the label is looked up in the string table
			}

			thecell.type = OwnerDraw::CellData::DataType::TEXT;
			thecell.string.set(item);

			SendMessage(handle, OD_SETCELL, MAKEWPARAM(275, row), (LPARAM)&thecell);
		}

		/*........................................................................
		Init the current phone book index
		........................................................................*/
		if (ListBox_GetCount(handle) == 0 || Session.CurPhoneIdx < -1) {
			Session.CurPhoneIdx = -1;
		} else {
			if (Session.CurPhoneIdx >= ListBox_GetCount(handle) ) {
				Session.CurPhoneIdx = 0;
			}
		}

		/*........................................................................
		Fill in phone number edit buffer
		........................................................................*/
		if (Session.CurPhoneIdx > -1) {
			ListBox_SetCurSel(handle, Session.CurPhoneIdx);
			handle = GetDlgItem(_Dialog, IDC_PHONE_NAME);
			if (handle != 0) {
				Edit_SetText(handle, Session.PhoneBook[Session.CurPhoneIdx]->Number);
			}
		}
	}
}


/*------------------------------------------------------------------
New phone listing selected.
------------------------------------------------------------------*/

/// <summary>
/// Handles a new listing being picked in the phone directory.
/// The chosen entry becomes the current one and its number is placed in the edit box, ready
/// for the user to dial or amend.
/// </summary>
void PhoneEdit::Select(void)
{
	HWND handle = GetDlgItem(_Dialog, IDC_PHONE_LIST);

	/*...............................................................
	Detect a change in the selected item; update CurPhoneIdx, and
	the edit box buffer.
	...............................................................*/

	if (handle != 0) {
		if (ListBox_GetCount(handle) > 0) {
			Session.CurPhoneIdx = ListBox_GetCurSel(handle);
			if (Session.CurPhoneIdx > -1) {
				handle = GetDlgItem(_Dialog, IDC_PHONE_NAME);
				Edit_SetText(handle, Session.PhoneBook[Session.CurPhoneIdx]->Number);
				SetFocus(handle);
			}
		}
	}
}


/*------------------------------------------------------------------
Add a new entry
------------------------------------------------------------------*/

/// <summary>
/// Adds a fresh entry to the phone book.
/// A blank entry set to the default modem settings is handed to the entry editor. It only
/// joins the phone book if the user accepts it, and it becomes the current selection so
/// that it can be dialed straight away.
/// </summary>
void PhoneEdit::Add(void)
{
	/*...............................................................
	Allocate a new phone book entry
	...............................................................*/
	PhoneEntryClass *p_entry = new PhoneEntryClass();
	if (p_entry == NULL) {
		return;
	}

	p_entry->Name[0] = '\0';
	p_entry->Number[0] = '\0';
	p_entry->Settings.Port = 0;
	p_entry->Settings.IRQ = -1;
	p_entry->Settings.Baud = -1;
	p_entry->Settings.DialMethod = DIAL_TOUCH_TONE;
	p_entry->Settings.InitStringIndex = -1;
	p_entry->Settings.CallWaitStringIndex = CALL_WAIT_CUSTOM;
	p_entry->Settings.CallWaitString[0] = '\0';

	ShowWindow(_Dialog, SW_HIDE);
	UpdateWindow(MainWindow);

	/*...............................................................
	Invoke the entry editor; if user clicks Save, add the new entry
	to the list, and rebuild the list box.
	...............................................................*/
	if ( PhoneEntryEdit().Dialog( p_entry ) == true ) {
		Session.PhoneBook.Add (p_entry);
		Changed = true;
		Build_Phone_Listbox();
		/*............................................................
		Set the current listbox index to the newly-added item.
		............................................................*/
		Session.CurPhoneIdx = Session.PhoneBook.ID(p_entry);

		/*........................................................................
		Fill in phone number edit buffer
		........................................................................*/
		if (Session.CurPhoneIdx > -1) {
			HWND handle = GetDlgItem(_Dialog, IDC_PHONE_LIST);
			if (handle != 0) {
				ListBox_SetCurSel(handle, Session.CurPhoneIdx);
			}
			handle = GetDlgItem(_Dialog, IDC_PHONE_NAME);
			if (handle != 0) {
				Edit_SetText(handle, Session.PhoneBook[Session.CurPhoneIdx]->Number);
			}
		}

	} else {

		/*...............................................................
		If the user clicked Cancel, delete the entry & keep looping.
		...............................................................*/
		delete p_entry;
	}
	ShowWindow(_Dialog, SW_SHOW);
}


/*------------------------------------------------------------------
Edit the current entry
------------------------------------------------------------------*/

/// <summary>
/// Hands the selected phone book entry to the entry editor.
/// The editor works on a copy, so the phone book only sees the changes if the user accepts
/// them. The list box is rebuilt afterward so that the new name and number show up.
/// </summary>
void PhoneEdit::Edit(void)
{
	HWND handle = GetDlgItem(_Dialog, IDC_PHONE_LIST);
	if (handle != 0) {
		Session.CurPhoneIdx = ListBox_GetCurSel(handle);
	}

	/*...............................................................
	Do nothing if no entry is selected.
	...............................................................*/
	if (Session.CurPhoneIdx <= -1) {
		return;
	}

	/*...............................................................
	Allocate a new entry & copy the currently-selected entry into it
	...............................................................*/
	PhoneEntryClass *p_entry = new PhoneEntryClass();
	if (p_entry == NULL) {
		return;
	}

	PhoneEntryClass *p_oldentry = Session.PhoneBook[Session.CurPhoneIdx];
	(*p_entry) = (*p_oldentry);

	ShowWindow(_Dialog, SW_HIDE);
	UpdateWindow(MainWindow);

	/*...............................................................
	Pass the new entry to the entry editor; if the user selects OK,
	copy the data back into our phone book.  Rebuild the list so
	the changes show up in the list box.
	...............................................................*/
	if ( PhoneEntryEdit().Dialog( p_entry ) == true ) {
		Changed = true;
		(*p_oldentry) = (*p_entry);
		Build_Phone_Listbox();
		/*............................................................
		Set the current listbox index to the newly-added item.
		............................................................*/
		Session.CurPhoneIdx = Session.PhoneBook.ID(p_oldentry);
		if (Session.CurPhoneIdx > -1) {
			ListBox_SetCurSel(handle, Session.CurPhoneIdx);
			handle = GetDlgItem(_Dialog, IDC_PHONE_NAME);
			if (handle != 0) {
				Edit_SetText(handle, p_oldentry->Number);
			}
		}
	}

	ShowWindow(_Dialog, SW_SHOW);
	delete p_entry;
}


/*------------------------------------------------------------------
Delete the current entry
------------------------------------------------------------------*/

/// <summary>
/// Removes the selected entry from the phone book.
/// The list box is rebuilt afterward so that the deletion shows up, and the number edit box
/// is cleared if nothing is left to select.
/// </summary>
void PhoneEdit::Delete(void)
{
	HWND handle = GetDlgItem(_Dialog, IDC_PHONE_LIST);
	if (handle == 0) {
		return;
	}

	Session.CurPhoneIdx = ListBox_GetCurSel(handle);

	/*...............................................................
	Do nothing if no entry is selected.
	...............................................................*/
	if (Session.CurPhoneIdx <= -1) {
		return;
	}

	/*...............................................................
	Delete the current item & rebuild the phone listbox
	...............................................................*/
	Session.PhoneBook.Delete_Index(Session.CurPhoneIdx);
	Changed = true;
	Build_Phone_Listbox();

	if (Session.CurPhoneIdx == -1) {
		handle = GetDlgItem(_Dialog, IDC_PHONE_NAME);
		if (handle != 0) {
			Edit_SetText(handle, "");
		}
	}
}


/// <summary>
/// Lets the user edit a phone book entry.
/// </summary>
/// <returns>bool; true if the user accepted the changes, false if canceled.</returns>
bool PhoneEntryEdit::Dialog(PhoneEntryClass *phone)
{
	if (phone == NULL) {
		return(false);
	}
	_Dialog = OwnerDraw::Begin_Dialog(IDD_SERIAL_PHONE_ENTRY, (DLGPROC)Edit_Phone_Dialog_Proc);
	if (_Dialog) {

		Phone = phone;
		RC = -1;
		SetWindowLong(_Dialog, DWL_USER, (LONG)this);

		Init();

		OwnerDraw::Display_Dialog(_Dialog);

		while (RC < 0) {
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				break;
			}
			Title_Screen_Restore();
		}

		OwnerDraw::End_Dialog(_Dialog);
	}

	if (RC != 1) { // RA2: the executable does not expand this function into Add and Edit (it calls it), which the single conditional return would allow
		return(false); // RA2: as above
	}
	return(true); // RA2: as above
}


/// <summary>
/// Handles the messages for the phone book entry editor dialog.
/// Anything the owner draw layer does not claim is dealt with here -- the name and number
/// boxes are trimmed as the user leaves them, the custom setting button hands off to the
/// serial settings editor, and the buttons end the dialog.
/// </summary>
/// <returns>Returns with the owner draw handler's result, or zero for a message this
/// routine dealt with itself.</returns>
BOOL CALLBACK Edit_Phone_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	wchar_t buffer[64]; // RA2: wide, so that the name can be trimmed as wide characters

	int rc;

	rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);

	if (rc == 0) {
		PhoneEntryEdit * _this = (PhoneEntryEdit *)GetWindowLong(window, DWL_USER);

		switch (message) {
		case OD_SUBCLASSED: // RA2: the limits are set on the subclassed message, there is no WM_INITDIALOG case
			SendDlgItemMessage(window, IDC_PHONE_NAME, EM_SETLIMITTEXT, 20, 0);
			SendDlgItemMessage(window, IDC_PHONE_NUMBER, EM_SETLIMITTEXT, 20, 0);
			break;

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
			case IDC_PHONE_DEFAULT:
				if (HIWORD(wparam) != 0) {
					Button_SetCheck((HWND)lparam, TRUE);
					SendDlgItemMessage(window, IDC_PHONE_CUSTOM, BM_SETCHECK, 0, 0);
				}
				break;

			case IDC_PHONE_CUSTOM:
				if (HIWORD(wparam) != 0) {
					Button_SetCheck((HWND)lparam, TRUE);
					SendDlgItemMessage(window, IDC_PHONE_DEFAULT, BM_SETCHECK, 0, 0);
					UpdateWindow(window);
					ShowWindow(window, SW_HIDE);
					UpdateWindow(MainWindow);
					SerialEdit().Dialog(&_this->TempSettings);
					ShowWindow(window, SW_SHOW);
				}
				break;

			case IDC_PHONE_NAME:
				if (HIWORD(wparam) == EN_MAXTEXT || HIWORD(wparam) == EN_KILLFOCUS) {
					Edit_GetTextW((HWND)lparam, buffer, 64); // RA2: the name box is read and written as wide characters
					wcstrim(buffer); // RA2: as above
					Edit_SetTextW((HWND)lparam, buffer); // RA2: as above
				}
				break;

			case IDOK:
				if (HIWORD(wparam) == 0) {
					_this->Save();
					_this->RC = IDOK;
				}
				break;

			case IDCANCEL:
				if (HIWORD(wparam) == 0) {
					_this->RC = IDCANCEL;
				}
				break;

			case IDC_PHONE_NUMBER:
				if (HIWORD(wparam) == EN_MAXTEXT || HIWORD(wparam) == EN_KILLFOCUS) {
					Edit_GetText((HWND)lparam, buffer, 64); // RA2: the number box is narrow; the wide buffer is reused
					strtrim((char *)buffer); // RA2: as above
					Edit_SetText((HWND)lparam, buffer); // RA2: as above
				}
				break;
			}
			break;
		}
		rc = 0;
	}

	return(rc);
}


/// <summary>
/// Stores the entry editor dialog back into the phone book entry.
/// The name and number are taken from their edit boxes and the entry either keeps the modem
/// settings the user built up in the settings editor or is reset to the defaults. An entry
/// left without a name is given the placeholder name.
/// </summary>
void PhoneEntryEdit::Save(void)
{
	bool custom;

	/*------------------------------------------------------------------------
	If 'Save', save all current settings
	------------------------------------------------------------------------*/
	HWND handle = GetDlgItem(_Dialog, IDC_PHONE_NAME);
	if (handle != 0) {
		Edit_GetTextW(handle, Phone->Name, sizeof(Phone->Name) / sizeof(Phone->Name[0])); // RA2: the name is read as wide characters
		if (wcslen( Phone->Name ) > 0) { // RA2: the length is tested as unsigned greater than zero
			_wcsupr( Phone->Name ); // RA2: as above
		} else {
			// if nothing was entered then make if NONAME
			wcscpy( Phone->Name, LS("TXT_NONAME", 757) ); // RA2: as above
		}
	}
	DebugString("Name: %ls\n", Phone->Name); // RA2: the name is wide

	handle = GetDlgItem(_Dialog, IDC_PHONE_NUMBER);
	if (handle != 0) {
		Edit_GetText(handle, Phone->Number, sizeof(Phone->Number));
	}
	DebugString("Number: %s\n", Phone->Number);

	handle = GetDlgItem(_Dialog, IDC_PHONE_CUSTOM);
	if (handle) {
		custom = Button_GetCheck(handle) == 1;
	} else {
		custom = false;
	}

	if (custom == true) {
		Phone->Settings = TempSettings;
	} else {
		Phone->Settings.Port = 0;
		Phone->Settings.IRQ = -1;
		Phone->Settings.Baud = -1;
		Phone->Settings.DialMethod = DIAL_TOUCH_TONE;
		Phone->Settings.InitStringIndex = -1;
		Phone->Settings.CallWaitStringIndex = CALL_WAIT_CUSTOM;
		Phone->Settings.CallWaitString[0] = 0;
	}

	DebugString("Settings: Port %d, Baud %d\n", Phone->Settings.Port, Phone->Settings.Baud);
}


/*
----------------------------- Various Inits ------------------------------
*/

/// <summary>
/// Fills the entry editor dialog in from the phone book entry.
/// The name and number are placed in their edit boxes and the modem settings are staged in
/// a working copy, so that the settings editor has something sensible to start from even
/// when the entry is set to use the defaults.
/// </summary>
void PhoneEntryEdit::Init(void)
{
	bool custom;

	HWND handle = GetDlgItem(_Dialog, IDC_PHONE_NAME);
	Edit_SetTextW(handle, Phone->Name); // RA2: the name is set as wide characters
	handle = GetDlgItem(_Dialog, IDC_PHONE_NUMBER);
	Edit_SetText(handle, Phone->Number);

	/*........................................................................
	Init the settings; if the phone entry is set to use defaults, init our
	settings to sensible values (in case we invoke the setting editor);
	otherwise, copy the entry's settings.
	........................................................................*/
	if (Phone->Settings.Port == 0 || Phone->Settings.IRQ == -1 ||
		Phone->Settings.Baud == -1) {
		TempSettings = Session.SerialDefaults;
		custom = false;
	} else {
		TempSettings = Phone->Settings;
		custom = true;
	}

	handle = GetDlgItem(_Dialog, IDC_PHONE_CUSTOM);
	if (handle != 0) {
		Button_SetCheck(handle, custom == true);
	}

	handle = GetDlgItem(_Dialog, IDC_PHONE_DEFAULT);
	if (handle != 0) {
		Button_SetCheck(handle, custom == false);
	}
}


/// <summary>
/// Compares two phone book entries for qsort.
/// Entries are ordered by name, and by number when the names are the same.
/// </summary>
/// <returns>int; 0 = same, negative = (*p1) goes before (*p2), positive = (*p1) goes after (*p2).</returns>
static int _cdecl Phone_Compare (const void *p1, const void *p2)
{
	class PhoneEntryClass *pe1,*pe2;
	int result;

	pe1 = *((class PhoneEntryClass **)p1);
	pe2 = *((class PhoneEntryClass **)p2);

	result = wcscmp( pe1->Name, pe2->Name ); // RA2: the names are wide

	// if strings are equal then check the phone number

	if ( !result ) {
		result = strcmp( pe1->Number, pe2->Number );
	}

	return(result);
}
