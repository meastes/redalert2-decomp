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

#include "mainopt.h"

#include "_map.h"
#include "_mixfile.h"
#include "_rect.h"
#include "_surface.h"
#include "convert.h"
#include "data.h"
#include "dbgprint.h"
#include "dsaudio.h"
#include "dsurface.h"
#include "gamedlg.h"
#include "globals.h"
#include "init.h"
#include "language\language.h"
#include "misc.h"
#include "mixfile.h"
#include "msgbox.h"
#include "newmenu.h"
#include "ownrdraw.h"
#include "sidebar.h"
#include "sounddlg.h"
#include "stimer.h"
#include "surface.h"
#include "wspipx.h"
#include "wwmouse.h"
#include "_rules.h"
#include "firewall.h"
#include "options.h"
#include "session.h"
#include "stringtable.h"
#include "techno.h"
#include "theme.h"
#include "windlg.h"

#include "color.hh"
#include "audio.h"
#include "txtlabel2.h"



/*
**	RA2: the original path of this file, passed with each string table lookup (see stringtable.h).
*/


enum {
	IDC_TRY_MODE_OK = 1473,
	IDC_TRY_MODE_CANCEL = 1738,
	IDC_SHOW_HIDDEN = 1540,
	IDC_OPTMAIN_OK = 1670,
	IDC_OPTMAIN_RESOLUTION = 1773,
	IDC_IPX_PORT = 1495,
	IDC_IPX_FIREWALL = 1890
};

/*
**	RA2: owner draw dialog messages the executable sends (names guessed from their use).
*/
#define OD_SETTEXT (WM_USER + 180)
#define OD_GETTEXT (WM_USER + 181)
#define OD_FINDSTRING (WM_USER + 184)
#define OD_ADDSTRING (WM_USER + 188)
#define OD_ADDITEM (WM_USER + 194)
#define OD_REFRESH_IPX (WM_USER + 100)
#define OD_TABFOCUS (WM_USER + 219)




BOOL CALLBACK Mode_Prompt_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
bool Mode_Prompt_Dialog(void);
void Apply_Game_Settings(HWND window);
BOOL CALLBACK Main_Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK IPX_Options_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK Test_Display_Mode_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
bool Change_Display_Mode(int width, int height);
bool Test_Display_Mode_Dialog(int width, int height);

OptionsClass TempOptions;


/*
**	RA2: the labels the options sliders show (0x007DFD80, 0x007DFD8C, 0x007DFD98) and the list positions the dialogs remember.
*/
static char const * DetailLabels[3] = { "TXT_LOW", "TXT_MEDIUM", "TXT_HIGH" };
static char const * DifficultyLabels[3] = { "TXT_EASY", "TXT_NORMAL", "TXT_HARD" };
static char const * ScrollLabels[7] = { "TXT_SLOWEST", "TXT_SLOWER", "TXT_SLOW", "TXT_MEDIUM", "TXT_FAST", "TXT_FASTER", "TXT_FASTEST" };
static int InitialResolution = -1;
static bool OptionsReady;
static int CurrentNetCard = -1;


/// <summary>
/// Handles the dialog that announces a display mode trial.
/// This routine shows the warning text and reports the button the player pressed.
/// </summary>
BOOL CALLBACK Mode_Prompt_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc == 0) {
		int * result = (int *)GetWindowLong(window, DWL_USER);
		switch (message) {
			case OD_SUBCLASSED: {
				wchar_t const * text = StringTable::LoadString("TXT_ABOUT_TO_TRY_MODE", NULL, "D:\\Ra2\\MainOpt.cpp", 162);
				HWND item = GetDlgItem(window, IDC_CONFIRM_MODE_TEXT);
				SendMessage(item, OD_SETTEXTW, 0, (LPARAM)text);
				break;
			}

			case WM_COMMAND:
				switch (LOWORD(wparam)) {
					case IDC_TRY_MODE_OK:
						*result = 1;
						break;

					case IDC_TRY_MODE_CANCEL:
						*result = 2;
						break;
				}
				break;
		}
		return(0);
	}
	return(rc);
}


/// <summary>
/// Warns the player that a display mode is about to be tried.
/// </summary>
/// <returns>bool; Did the player accept?</returns>
bool Mode_Prompt_Dialog(void)
{
	int rc = -1;

	HWND dialog = OwnerDraw::Begin_Dialog(IDD_OPT_CONFIRM_MODE, Mode_Prompt_Dialog_Proc);
	if (dialog) {
		SetWindowLong(dialog, DWL_USER, (LONG)&rc);
		OwnerDraw::Display_Dialog(dialog);
		while (rc < 0 && !OwnerDraw::Dialog_Message_Handler()) Title_Screen_Restore();
		OwnerDraw::End_Dialog(dialog);
	}
	return(rc == 1);
}


/// <summary>
/// Takes the settings of the main options dialog over into the game options.
/// </summary>
void Apply_Game_Settings(HWND window)
{
	HWND control = GetDlgItem(window, IDC_DETAIL_LEVEL_SLIDER);
	if (control) {
		int detail = SendMessage(control, TBM_GETPOS, 0, 0) ? 2 : 0;
		if (Options.DetailLevel != detail) {
			Options.DetailLevel = detail;
			Map.Reinit_Cell_Drawers();
		}
	}

	control = GetDlgItem(window, IDC_DIFFICULTY_SLIDER);
	if (control) {
		Options.Difficulty = SendMessage(control, TBM_GETPOS, 0, 0);
	}

	control = GetDlgItem(window, IDC_TARGET_LINES);
	if (control) {
		Options.ActionLines = SendMessage(control, BM_GETCHECK, 0, 0) == 1;
		TechnoClass::Set_Action_Lines(Options.ActionLines);
	}

	control = GetDlgItem(window, IDC_SHOW_HIDDEN);
	if (control) {
		Options.IsShowHidden = SendMessage(control, BM_GETCHECK, 0, 0) == 1;
	}

	control = GetDlgItem(window, IDC_TOOLTIPS);
	if (control) {
		Options.ToolTips = SendMessage(control, BM_GETCHECK, 0, 0) == 1;
	}

	control = GetDlgItem(window, IDC_SCROLL_SPEED_SLIDER);
	if (control) {
		Options.ScrollRate = 6 - SendMessage(control, TBM_GETPOS, 0, 0);
	}

	control = GetDlgItem(window, IDC_MUSIC_VOLUME);
	if (control) {
		Options.Set_Score_Volume((float)(SendMessage(control, TBM_GETPOS, 0, 0) * 0.1), false);
		if (Options.ScoreVolume == 0.0f) {
			Theme.Queue_Song(Theme.What_Is_Playing());
			Theme.Stop();
		}
	}

	control = GetDlgItem(window, IDC_SOUND_VOLUME);
	if (control) {
		Options.Set_Sound_Volume((float)(SendMessage(control, TBM_GETPOS, 0, 0) * 0.1), false);
	}

	control = GetDlgItem(window, IDC_VOICE_VOLUME);
	if (control) {
		Options.Set_Voice_Volume((float)(SendMessage(control, TBM_GETPOS, 0, 0) * 0.1), false);
	}
}


/// <summary>
/// Brings up the main options dialog.
/// This routine drives the options menu, dispatching to the sound, display, network,
/// keyboard and game settings dialogs until the player backs out. A resolution change is
/// offered as a trial first, and the settings are written out when the player leaves.
/// </summary>
/// <remarks>Game logic is suspended for the duration of this routine.</remarks>
void Main_Options_Dialog(void)
{
	bool old_game_active = GameActive;
	GameActive = false;

	HWND main_handle;
	LONG main_rc;

	HWND in_handle;
	LONG in_rc;

	while (true) {
		do {
			TempOptions = Options;
			main_rc = -1;
			main_handle = OwnerDraw::Begin_Dialog(IDD_OPT_MAIN, Main_Options_Dialog_Proc);
		} while (main_handle == 0);
		SetWindowLong(main_handle, DWL_USER, (LONG)&main_rc);
		OwnerDraw::Display_Dialog(main_handle);

		while (main_rc < 0) {
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				break;
			}
			Title_Screen_Restore();
		}

		Apply_Game_Settings(main_handle);
		OwnerDraw::End_Dialog(main_handle);

		switch (main_rc) {
			case IDC_OPTMAIN_NETWORK: {
				in_rc = -1;
				Session.Read_MultiPlayer_Settings();
				in_handle = OwnerDraw::Begin_Dialog(IDD_OPT_IPX, IPX_Options_Proc);
				if (in_handle) {
					SetWindowLong(in_handle, DWL_USER, (LONG)&in_rc);
					OwnerDraw::Display_Dialog(in_handle);

					while (in_rc < 0) {
						if (OwnerDraw::Dialog_Message_Handler() == true) {
							break;
						}
						Title_Screen_Restore();
					}

					OwnerDraw::End_Dialog(in_handle);
				}
			}
			break;

			case IDC_OPTMAIN_KEYBOARD:
				Options.Hotkey_Dialog();
				break;

			default:
				Options.Save_Settings();
				GameActive = old_game_active;
				return;
		}
	}
}


/// <summary>
/// Handles the main options dialog.
/// This routine reports the button the player pressed back to the options dialog driver so
/// that it can bring up the appropriate sub dialog. The sound button is disabled when there
/// is no audio hardware to talk to.
/// </summary>
#if 0
BOOL CALLBACK Main_Options_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc != 0) {
		return(rc);
	}

	int * result = (int *)GetWindowLong(window, DWL_USER);
	HWND control = (HWND)lparam;
	switch (message) {
		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case IDC_OPTMAIN_NETWORK:
				case IDC_OPTMAIN_KEYBOARD:
					delete [] ResolutionModes;
					ResolutionModes = NULL;
					*result = LOWORD(wparam);
					break;

				case IDC_OPTMAIN_OK:
					if (HIWORD(wparam) == 0) {
						delete [] ResolutionModes;
						ResolutionModes = NULL;
						*(int *)GetWindowLong(window, DWL_USER) = IDC_OPTMAIN_MAINMENU;
					}
					break;

				case IDC_OPTMAIN_RESOLUTION:
					if (HIWORD(wparam) == CBN_SELCHANGE) {
						Center_Window_Within_Window(window, MainWindow);
						HWND combo = GetDlgItem(window, IDC_OPTMAIN_RESOLUTION);
						if (combo == NULL) {
							return(0);
						}
						int item = ComboBox_GetCurSel(combo);
						int mode = ComboBox_GetItemData(combo, item);
						TempOptions.ScreenWidth = ResolutionModes[mode].Width;
						TempOptions.ScreenHeight = ResolutionModes[mode].Height;
						if (TempOptions.ScreenWidth == Options.ScreenWidth && TempOptions.ScreenHeight == Options.ScreenHeight) {
							return(0);
						}
						if (Mode_Prompt_Dialog()) {
							if (Debug_IngameModeChange) {
								Options.ScreenWidth = TempOptions.ScreenWidth;
								Options.ScreenHeight = TempOptions.ScreenHeight;
								return(0);
							}
							if (Test_Display_Mode_Dialog(TempOptions.ScreenWidth, TempOptions.ScreenHeight)) {
								Options.ScreenWidth = TempOptions.ScreenWidth;
								Options.ScreenHeight = TempOptions.ScreenHeight;
								Relayout_Interface_Art(TempOptions.ScreenWidth, TempOptions.ScreenHeight);
								Darken_Surface(NULL, true);
								return(0);
							}
							ComboBox_SetCurSel(combo, InitialResolution);
							return(0);
						}
						ComboBox_SetCurSel(combo, InitialResolution);
						return(0);
					}
					break;
			}
			return(0);

		case WM_DESTROY:
			if (ResolutionModes) {
				delete [] ResolutionModes;
				ResolutionModes = NULL;
			}
			return(0);

		case WM_HSCROLL: {
			if (LOWORD(wparam) != TB_THUMBTRACK) {
				return(0);
			}
			int position = HIWORD(wparam);
			char const * name;
			HWND text;
			if (control == GetDlgItem(window, IDC_DETAIL_LEVEL_SLIDER)) {
				int index = position;
				if (position) {
					index = 2;
				}
				name = DetailLabels[index];
				text = GetDlgItem(window, IDC_DETAIL_LEVEL_LABEL);
			} else if (control == GetDlgItem(window, IDC_DIFFICULTY_SLIDER)) {
				name = DifficultyLabels[position];
				text = GetDlgItem(window, IDC_DIFFICULTY_LABEL);
			} else if (control == GetDlgItem(window, IDC_SCROLL_SPEED_SLIDER)) {
				name = ScrollLabels[position];
				text = GetDlgItem(window, IDC_SCROLL_SPEED_LABEL);
			} else {
				if (!OptionsReady) {
					return(0);
				}
				if (control == GetDlgItem(window, IDC_MUSIC_VOLUME)) {
					Options.Set_Score_Volume((float)(SendMessage(control, TBM_GETPOS, 0, 0) * 0.1), true);
				} else if (control == GetDlgItem(window, IDC_SOUND_VOLUME)) {
					Options.Set_Sound_Volume((float)(SendMessage(control, TBM_GETPOS, 0, 0) * 0.1), true);
				} else if (control == GetDlgItem(window, IDC_VOICE_VOLUME)) {
					Options.Set_Voice_Volume((float)(SendMessage(control, TBM_GETPOS, 0, 0) * 0.1), true);
				}
				return(0);
			}
			if (text) {
				SendMessage(text, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString(name, NULL, "D:\\Ra2\\MainOpt.cpp", 550));
			}
			return(0);
		}

		case OD_SUBCLASSED: {
			OptionsReady = false;
			HWND slider = GetDlgItem(window, IDC_DETAIL_LEVEL_SLIDER);
			if (slider) {
				SendMessage(slider, OD_TRACKNUMBERS, 0, 0);
				SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG(0, 1));
				SendMessage(slider, TBM_SETPOS, TRUE, Options.DetailLevel != 0);
			}

			HWND combo = GetDlgItem(window, IDC_OPTMAIN_RESOLUTION);
			if (Options.AllowHiResModes) {
				ResolutionModes = (ModeSize *)EnumDisplayModes(640, 480, 4096, 4096, 16);
			} else {
				ResolutionModes = (ModeSize *)EnumDisplayModes(640, 480, 1024, 768, 16);
			}
			int * modes = (int *)ResolutionModes;
			int item_index = 0;
			int selected = -1;
			int count = 0;
			if (modes != NULL) {
				while (*modes != 0) {
					int width = *modes++;
					int height = *modes++;
					if (Options.AllowHiResModes || width == 640 || width == 800 || width == 1024) {
						if (width == TempOptions.ScreenWidth && height == TempOptions.ScreenHeight) {
							selected = count;
						}
						char buffer[64];
						sprintf(buffer, "%d x %d x 16", width, height);
						int item = ComboBox_AddString(combo, buffer);
						ComboBox_SetItemData(combo, item, item_index);
						count++;
					}
					item_index++;
				}
			}
			ComboBox_SetCurSel(combo, selected);
			InitialResolution = selected;

			slider = GetDlgItem(window, IDC_DIFFICULTY_SLIDER);
			if (slider) {
				SendMessage(slider, OD_TRACKNUMBERS, 0, 0);
				SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG(0, 2));
				SendMessage(slider, TBM_SETPOS, TRUE, Options.Difficulty);
			}

			HWND box = GetDlgItem(window, IDC_TARGET_LINES);
			if (box) {
				SendMessage(box, BM_SETCHECK, Options.ActionLines != 0, 0);
			}
			box = GetDlgItem(window, IDC_SHOW_HIDDEN);
			if (box) {
				SendMessage(box, BM_SETCHECK, Options.IsShowHidden != 0, 0);
			}
			box = GetDlgItem(window, IDC_TOOLTIPS);
			if (box) {
				SendMessage(box, BM_SETCHECK, Options.ToolTips != 0, 0);
			}

			slider = GetDlgItem(window, IDC_SCROLL_SPEED_SLIDER);
			if (slider) {
				SendMessage(slider, OD_TRACKNUMBERS, 0, 0);
				SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG(0, 6));
				SendMessage(slider, TBM_SETPOS, TRUE, 6 - Options.ScrollRate);
			}

			int sound = AudioDeviceAvailable();
			slider = GetDlgItem(window, IDC_MUSIC_VOLUME);
			if (slider) {
				SendMessage(slider, OD_TRACKSILENT, 0, 0);
				SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG(0, 10));
				SendMessage(slider, TBM_SETPOS, TRUE, (int)(Options.ScoreVolume * 10.0 + 0.5));
				EnableWindow(slider, sound);
			}
			slider = GetDlgItem(window, IDC_SOUND_VOLUME);
			if (slider) {
				SendMessage(slider, OD_TRACKSILENT, 0, 0);
				SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG(0, 10));
				SendMessage(slider, TBM_SETPOS, TRUE, (int)(Options.SoundVolume * 10.0 + 0.5));
				EnableWindow(slider, sound);
			}
			slider = GetDlgItem(window, IDC_VOICE_VOLUME);
			if (slider) {
				SendMessage(slider, OD_TRACKSILENT, 0, 0);
				SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG(0, 10));
				SendMessage(slider, TBM_SETPOS, TRUE, (int)(Options.VoiceVolume * 10.0 + 0.5));
				EnableWindow(slider, sound);
			}
			OptionsReady = true;
			return(0);
		}
	}
	return(0);
}
#endif


/// <summary>
/// Handles the network options dialog.
/// This routine lists the IPX addresses of the network cards bound to the machine and lets
/// the player choose one, along with the socket number and destination network to use. The
/// socket and destination network are validated before the options are accepted.
/// </summary>
BOOL CALLBACK IPX_Options_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc) {
		return(rc);
	}

	int * result = (int *)GetWindowLong(window, DWL_USER);
	char text[16];
	char address[64];
	switch (message) {
		case OD_SUBCLASSED: {
			HWND cards = GetDlgItem(window, IDC_IPX_NETCARD);
			bool temporary = false;
			if (PacketTransport == NULL) {
				PacketTransport = new IPXInterfaceClass;
				PacketTransport->Init();
				temporary = true;
			}
			if (PacketTransport != NULL) {
				DebugString("Creating IPX Interface\n");
				IPXInterfaceClass * transport = (IPXInterfaceClass *)PacketTransport;
				for (int card = 0; card < 20; card++) {
					if (transport->Get_Network_Card_Address_As_String(card, address, sizeof(address))) {
						DebugString("Got address for network card %d\n", card);
						DebugString("Address is %s\n", address);
						if (SendMessage(cards, OD_FINDSTRING, 0, (LPARAM)address) == -1) {
							SendMessage(cards, OD_ADDSTRING, 0, (LPARAM)address);
						}
					}
				}
			}
			if (temporary) {
				delete PacketTransport;
				PacketTransport = NULL;
			}
			if (SendMessage(cards, CB_GETCOUNT, 0, 0) == 0) {
				SendMessage(cards, OD_ADDITEM, 0, (LPARAM)StringTable::LoadString("TXT_NO_BOUND_NET_CARD", NULL, "D:\\Ra2\\MainOpt.cpp", 886));
			}
			int selected = Options.NetCard;
			if (selected < 0 || selected >= SendMessage(cards, CB_GETCOUNT, 0, 0)) {
				selected = 0;
			}
			SendMessage(cards, CB_SETCURSEL, selected, 0);
			SendMessage(window, OD_REFRESH_IPX, 0, 0);
			CurrentNetCard = -1;
			HWND socket = GetDlgItem(window, IDC_IPX_SOCKET);
			if (Options.Socket != 0xFFFF) {
				sprintf(text, "%d", Options.Socket);
				SendMessage(socket, OD_SETTEXT, 0, (LPARAM)text);
			}
			SendMessage(socket, EM_LIMITTEXT, 5, 0);
			HWND destination = GetDlgItem(window, IDC_IPX_DESTNET);
			if (strlen(Options.DestNet)) {
				SendMessage(destination, OD_SETTEXT, 0, (LPARAM)Options.DestNet);
			}
			SendMessage(destination, EM_LIMITTEXT, 11, 0);
			if (Session.SendDelay) {
				SendDlgItemMessage(window, IDC_IPX_FIREWALL, BM_SETCHECK, 1, 0);
			} else {
				SendDlgItemMessage(window, IDC_IPX_FIREWALL, BM_SETCHECK, 0, 0);
			}
			HWND port = GetDlgItem(window, IDC_IPX_PORT);
			if (port != (HWND)-1 && Session.PortNumberOverride != 0) {
				sprintf(text, "%d", Session.PortNumberOverride);
				SendMessage(port, OD_SETTEXT, 0, (LPARAM)text);
			}
		}
		break;

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case IDOK: {
					HWND socket_control = GetDlgItem(window, IDC_IPX_SOCKET);
					SendMessage(socket_control, OD_GETTEXT, 16, (LPARAM)text);
					unsigned short socket;
					if (strlen(text)) {
						socket = (unsigned short)atoi(text);
						if (strlen(text) > 5 || socket > 16383) {
							WWMessageBox(L" ").Process(StringTable::LoadString("TXT_INVALID_SOCKET", NULL, "D:\\Ra2\\MainOpt.cpp", 975), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\MainOpt.cpp", 975), NULL, NULL, false);
							return(0);
						}
					} else {
						socket = 0xFFFF;
					}
					char destination[32];
					char saved_destination[32];
					HWND destination_control = GetDlgItem(window, IDC_IPX_DESTNET);
					SendMessage(destination_control, OD_GETTEXT, 32, (LPARAM)destination);
					strncpy(saved_destination, destination, 32);
					if (strlen(destination)) {
						char * token = strtok(destination, ".");
						int network[4] = { -1, -1, -1, -1 };
						if (token) {
							int * part = network;
							do {
								sscanf(token, "%x", part);
								part++;
								token = strtok(NULL, ".");
							} while (token);
						}
						if ((network[0] & 255) != network[0] || (network[1] & 255) != network[1] ||
							(network[2] & 255) != network[2] || (network[3] & 255) != network[3]) {
							WWMessageBox(L" ").Process(StringTable::LoadString("TXT_DESTNET_ADDR_ERROR", NULL, "D:\\Ra2\\MainOpt.cpp", 1009), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\MainOpt.cpp", 1009), NULL, NULL, false);
							return(0);
						}
					}
					HWND card_control = GetDlgItem(window, IDC_IPX_NETCARD);
					Options.NetCard = SendMessage(card_control, CB_GETCURSEL, 0, 0);
					Options.Socket = socket;
					strcpy(Options.DestNet, saved_destination);
					HWND port_control = GetDlgItem(window, IDC_IPX_PORT);
					char port_text[32];
					memset(port_text, 0, sizeof(port_text));
					SendMessage(port_control, OD_GETTEXT, 8, (LPARAM)port_text);
					strncpy(address, port_text, 32);
					int port = atoi(address);
					if (port > 65535) {
						WWMessageBox(L" ").Process(StringTable::LoadString("TXT_PORT_NUMBER_ERROR", NULL, "D:\\Ra2\\MainOpt.cpp", 1050), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\MainOpt.cpp", 1050), NULL, NULL, false);
						return(0);
					}
					if (SendDlgItemMessage(window, IDC_IPX_FIREWALL, BM_GETCHECK, 0, 0) == 1) {
						if (!Session.SendDelay) {
							WWMessageBox(L" ").Process(StringTable::LoadString("TXT_FIREWALL_SETTINGS_WARNING1", NULL, "D:\\Ra2\\MainOpt.cpp", 1059), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\MainOpt.cpp", 1059), NULL, NULL, false);
						}
						Session.SendDelay = true;
					} else {
						Session.SendDelay = false;
					}
					if (port > 0 && Session.PortNumberOverride == 0) {
						WWMessageBox(L" ").Process(StringTable::LoadString("TXT_FIREWALL_SETTINGS_WARNING2", NULL, "D:\\Ra2\\MainOpt.cpp", 1070), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\MainOpt.cpp", 1070), NULL, NULL, false);
					}
					Session.PortNumberOverride = port;
					FirewallHelper.Clear();
					RawFileClass settings("RA2.INI");
					ConfigINI.Put_Bool("MultiPlayer", "SendDelay", Session.SendDelay);
					ConfigINI.Put_Int("MultiPlayer", "PortNumberOverride", Session.PortNumberOverride, 0);
					ConfigINI.Save(settings, false);
				}


				case IDCANCEL:
					*result = 2;
					break;

				case IDC_IPX_NETCARD: {
					HWND card_control = GetDlgItem(window, IDC_IPX_NETCARD);
					CurrentNetCard = SendMessage(card_control, CB_GETCURSEL, 0, 0);
				}
				break;
			}
			break;

		case OD_TABFOCUS:
			Focus_Next_Tab_Item(lparam, wparam != 0);
			return(0);
	}
	return(0);
}


/// <summary>
/// Switches the game over to a new screen resolution.
/// This routine tears down every drawing surface and the mouse handler, sets the new video
/// mode, then builds all of them back up and hands the tactical map its new dimensions. The
/// hicolor remap tables are rebuilt as well, since the new mode may pack its pixels
/// differently.
/// </summary>
/// <param name="width">The width of the display mode to change to.</param>
/// <param name="height">The height of the display mode to change to.</param>
/// <returns>bool; Was the mode changed? If not, the previous mode has been restored.</returns>
#if 0
bool Change_Display_Mode(int width, int height)
{
	DebugString("About to set video mode\n");

	Point2D old_size(VideoModeWidth, VideoModeHeight);

	int redleft = DSurface::Get_Red_Left();
	int redright = DSurface::Get_Red_Right();
	int greenleft = DSurface::Get_Green_Left();
	int greenright = DSurface::Get_Green_Right();
	int blueleft = DSurface::Get_Blue_Left();
	int blueright = DSurface::Get_Blue_Right();

	Hide_Mouse();

	Wait_Blit();

	if (WindowedMode == true) {
		VisibleRect = Rect(0, 0, width, height);
		VideoModeWidth = width;
		VideoModeHeight = height;
		DebugString("VisibleRect: %dx%d\n", width, height);
	} else {
		if (MouseCursor != NULL) {
			DebugString("Removing mouse handler\n");
			delete MouseCursor;
			MouseCursor = NULL;
		}

		if (VisibleSurface != NULL) {
			DebugString("Deleting primary surface\n");
			delete VisibleSurface;
			VisibleSurface = NULL;
		}

		if (!Set_Video_Mode(MainWindow, width, height, 16)) {
			DebugString("Set_Video_Mode failed.\n");

			DebugString("Restoring primary surface\n");
			VisibleSurface = DSurface::Create_Primary();

			DebugString("Restoring mouse handler\n");
			MouseCursor = new WWMouseClass(VisibleSurface, MainWindow);

			void const * mouseshp = MFCD::Retrieve("MOUSE.SHP");
			if (mouseshp != NULL) {
				Point2D hotspot = Point2D(0, 0);
				Set_Mouse_Cursor(hotspot, (ShapeSet const *)mouseshp, 0);
			}

			Map.Set_Default_Mouse(MOUSE_NORMAL);
			Map.Override_Mouse_Shape(MOUSE_NORMAL);

			Show_Mouse();

			return(false);
		}

		DebugString("Mode apparently set OK\n");
		VisibleRect = Rect(0, 0, width, height);
		DebugString("VisibleRect: %dx%d\n", width, height);
	}

	if (AlternateSurface != NULL) {
		DebugString("Deleting AlternateSurface\n");
		delete AlternateSurface;
		AlternateSurface = NULL;
	}

	if (HiddenSurface != NULL) {
		DebugString("Deleting HiddenSurface\n");
		delete HiddenSurface;
		HiddenSurface = NULL;
	}

	if (TileSurface != NULL) {
		DebugString("Deleting TileSurface\n");
		delete TileSurface;
		TileSurface = NULL;
	}

	if (SidebarSurface != NULL) {
		DebugString("Deleting SidebarSurface\n");
		delete SidebarSurface;
		SidebarSurface = NULL;
	}

	if (CompositeSurface != NULL) {
		DebugString("Deleting CompositeSurface\n");
		delete CompositeSurface;
		CompositeSurface = NULL;
	}

	if (WindowedMode == false) {
		DebugString("Setting new window size to %dx%d\n", width, height);
		SetWindowPos(MainWindow, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
		DebugString("Creating primary surface\n");
		VisibleSurface = DSurface::Create_Primary();
	} else {
		DebugString("Setting new window size to %dx%d\n", width, height);
		RECT windowrect;
		SetRect(&windowrect, 0, 0, width, height);
		AdjustWindowRectEx(&windowrect, GetWindowLong(MainWindow, GWL_STYLE), GetMenu(MainWindow) != 0, GetWindowLong(MainWindow, GWL_EXSTYLE));
		MoveWindow(MainWindow, 0, 0, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top, TRUE);
	}

	Rect temp = Get_Title_Screen_Area();

	{
		Rect sidebar(0, 0, SidebarWidth, VisibleRect.Height);
		Rect tile(0, 0, temp.Width, VisibleRect.Height);
		Rect composite(0, 0, temp.Width, VisibleRect.Height);
		Allocate_Surfaces(VisibleRect, composite, tile, sidebar, false);
	}
	LogicalSurface = HiddenSurface;

	DebugString("Recalc color remap tables\n");
	ConvertClass::Reinitialize_Hicolor_Tables(redleft, redright, greenleft, greenright, blueleft, blueright);

	if (MouseCursor == NULL) {
		MouseCursor = new WWMouseClass(VisibleSurface, MainWindow);

		void const * mouseshp = MFCD::Retrieve("MOUSE.SHP");
		if (mouseshp != NULL) {
			Point2D hotspot = Point2D(0, 0);
			Set_Mouse_Cursor(hotspot, (ShapeSet const *)mouseshp, 0);
		}

		Map.Set_Default_Mouse(MOUSE_NORMAL);
		Map.Override_Mouse_Shape(MOUSE_NORMAL);

		Show_Mouse();

	} else {
		((WWMouseClass*)MouseCursor)->Calc_Confining_Rect();
	}

	Map.Set_View_Dimensions(temp);
	Relayout_Interface_Art(VideoModeWidth, VideoModeHeight);

	Map.Init_IO();
	Map.Activate(1);
	Map.Reposition_Sidebar();
	Map.Flag_To_Redraw(GS_REDRAW_ALL);

	for (HWND dialog = WS_Top_Window(); dialog != NULL; dialog = WS_Next_Lower_Dialog(dialog)) {
		On_WM_NCDESTROY(dialog);
		Resize_Dialog_To_Screen(dialog, &old_size);
	}

	Show_Mouse();

	DebugString("Mode change complete.\n");

	return(true);
}
#endif



/// <summary>
/// Tries a display mode out and asks the player to confirm it.
/// This routine switches to the requested mode and puts up a confirmation dialog. If the
/// player does not accept the mode -- or says nothing at all, because a bad mode may well
/// leave the screen unreadable -- the previous resolution is restored.
/// </summary>
/// <param name="width">The width of the display mode to try.</param>
/// <param name="height">The height of the display mode to try.</param>
/// <returns>bool; Was the new display mode accepted and left in place?</returns>
#if 0
bool Test_Display_Mode_Dialog(int width, int height)
{
	int rc = -1;

	DebugString("Testing display mode @ %dx%d\n", width, height);
	Hide_Mouse();
	HiddenSurface->Fill(TBLACK);
	Update_Visible_Surface();

	if (!Change_Display_Mode(width, height)) {
		return(false);
	}

	HiddenSurface->Fill(TBLACK);
	Update_Visible_Surface();
	Show_Mouse();
	Darken_Surface(NULL, true);

	HWND dialog = OwnerDraw::Begin_Dialog(IDD_OPT_CONFIRM_MODE, Test_Display_Mode_Dialog_Proc);
	if (dialog) {
		SetWindowLong(dialog, DWL_USER, (LONG)&rc);
		OwnerDraw::Display_Dialog(dialog);

		CDTimerClass<SystemTimerClass> timer = 10 * TIMER_SECOND;
		while (rc < 0) {
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				break;
			}
			Title_Screen_Restore();
			if (timer <= 0) {
				PostMessage(dialog, WM_COMMAND, IDC_TRY_MODE_CANCEL, 0);
				timer = 5 * TIMER_SECOND;
			}
		}

		OwnerDraw::End_Dialog(dialog);
		if (rc != 1) {
			DebugString("Resetting display mode @ %dx%d\n", Options.ScreenWidth, Options.ScreenHeight);
			Change_Display_Mode(Options.ScreenWidth, Options.ScreenHeight);
			LogicalSurface = HiddenSurface;
			return(false);
		}
	}

	DebugString("Keeping display mode @ %dx%d\n", width, height);
	LogicalSurface = HiddenSurface;
	return(true);
}
#endif



/// <summary>
/// Handles the mode confirmation dialog.
/// This routine records the button the player pressed so that the mode test can tell
/// whether the new resolution was accepted or rejected.
/// </summary>
BOOL CALLBACK Test_Display_Mode_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc == 0) {
		int * result = (int *)GetWindowLong(window, DWL_USER);
		switch (message) {
			case OD_SUBCLASSED: {
				wchar_t const * text = StringTable::LoadString("GUI:ConfirmModeChange", NULL, "D:\\Ra2\\MainOpt.cpp", 1493);
				HWND item = GetDlgItem(window, IDC_CONFIRM_MODE_TEXT);
				SendMessage(item, OD_SETTEXTW, 0, (LPARAM)text);
				break;
			}

			case WM_COMMAND:
				switch (LOWORD(wparam)) {
					case IDC_TRY_MODE_OK:
						*result = 1;
						break;

					case IDC_TRY_MODE_CANCEL:
						*result = 2;
						break;
				}
				break;
		}
		return(0);
	}
	return(rc);
}
