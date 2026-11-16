// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y8): the shell Options dialog's procedure (template 0xD5), gamemd.exe
// 0x0055FDB0, unmatched in the YR tree (a link-closure trap: the main menu's Options crashed).
// Written from gamemd.exe's code with the RA2 twin's behaviour row as the text (native/link/
// behaviour_m22_options.cpp, Main_Options_Proc 0x00542590): the same controls and messages; YR's
// resolution box sets the mode without RA2's test dialog. Behaviour code for the native build;
// never part of an exact build. NATIVE: the Enhancements button (id 0x7F80, appended to the
// template by the port, port/src/native_dialogs.cpp) opens the Enhancements page below.
// ---------------------------------------------------------------------------------------------
#include "winfix.h"
#include "misc.h"

extern HWND DAT_00B73550;                                // the main window (ownrdraw.cpp)
int FUN_00407000(void);                                  // wpaudio: a sound device is available
void Yr_Run_Enhancements(HWND options);                  // below (NATIVE)
void Yr_Enhancements_Button_Text(HWND options);

static int *Options_Modes = NULL;                        // 0x00ABCF58: EnumDisplayModes' width/height pairs
static bool Options_Ready = false;                       // 0x00ABCF54: the sliders are set up
static int Options_Initial_Mode = -1;                    // 0x0082A27C: the box's first selection
static char const * const Options_Detail_Names[3] = {"TXT_LOW", "TXT_MEDIUM", "TXT_HIGH"};     // 0x0082A248
static char const * const Options_Difficulty_Names[3] = {"TXT_EASY", "TXT_NORMAL", "TXT_HARD"};   // 0x0082A254
static char const * const Options_Scroll_Names[7] = {"TXT_SLOWEST", "TXT_SLOWER", "TXT_SLOW", "TXT_MEDIUM",
	"TXT_FAST", "TXT_FASTER", "TXT_FASTEST"};                                                  // 0x0082A260

BOOL CALLBACK FUN_0055FDB0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc) return rc;
	int *result = (int *)ra2port_get_dialog_user(window);
	HWND control = (HWND)lparam;

	switch (message) {
		case WM_COMMAND: {
			int id = wparam & 0xFFFF;
			if (id == 0x5CD || id == 0x5CE) {			// the network page, the keyboard dialog
				free(Options_Modes);
				Options_Modes = NULL;
				*result = id;
			} else if (id == 0x686) {					// OK
				if ((wparam >> 16) == 0) {
					free(Options_Modes);
					Options_Modes = NULL;
					*result = 0x5CB;
				}
			} else if (id == 0x7F80) {					// NATIVE: ENHANCEMENTS
				if ((wparam >> 16) == 0) Yr_Run_Enhancements(window);
			} else if (id == 0x6ED && (wparam >> 16) == 1) {	// the resolution box: CBN_SELCHANGE
				Center_Window_Within_Window(window, DAT_00B73550);
				HWND combo = GetDlgItem(window, 0x6ED);
				if (combo == NULL) return 0;
				int item = SendMessageA(combo, CB_GETCURSEL, 0, 0);
				int mode = SendMessageA(combo, CB_GETITEMDATA, item, 0);
				TempOptions.ScreenWidth = Options_Modes[mode * 2];
				TempOptions.ScreenHeight = Options_Modes[mode * 2 + 1];
				if (TempOptions.ScreenWidth != Options.ScreenWidth || TempOptions.ScreenHeight != Options.ScreenHeight) {
					Options.ScreenHeight = TempOptions.ScreenHeight;
					Options.ScreenWidth = TempOptions.ScreenWidth;
				}
			}
			return 0;
		}

		case WM_DESTROY:
			if (Options_Modes != NULL) {
				free(Options_Modes);
				Options_Modes = NULL;
			}
			return 0;

		case WM_HSCROLL: {
			if ((short)wparam != TB_THUMBTRACK) return 0;
			int position = (int)(wparam >> 16);
			char const *name;
			int label;
			if (control == GetDlgItem(window, 0x52B)) {
				name = Options_Detail_Names[position ? 2 : 0];
				label = 0x673;
			} else if (control == GetDlgItem(window, 0x50F)) {
				name = Options_Difficulty_Names[position];
				label = 0x670;
			} else if (control == GetDlgItem(window, 0x52A)) {
				name = Options_Scroll_Names[position];
				label = 0x672;
			} else {
				if (!Options_Ready) return 0;
				if (control == GetDlgItem(window, 0x52F)) {
					Options.Set_Score_Volume((float)(SendMessageA(control, 0x400, 0, 0) * 0.1), true);
				} else if (control == GetDlgItem(window, 0x532)) {
					Options.Set_Sound_Volume((float)(SendMessageA(control, 0x400, 0, 0) * 0.1), true);
				} else if (control == GetDlgItem(window, 0x536)) {
					Options.Set_Voice_Volume((float)(SendMessageA(control, 0x400, 0, 0) * 0.1), true);
				}
				return 0;
			}
			HWND text = GetDlgItem(window, label);
			if (text) {
				SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::LoadString(name, NULL, "D:\\ra2mdpost\\MainOpt.cpp", 0x229));
			}
			return 0;
		}

		case 0x497: {									// the dialog's controls are set up
			Yr_Enhancements_Button_Text(window);		// NATIVE: the ENHANCEMENTS button's label
			Options_Ready = false;
			HWND slider = GetDlgItem(window, 0x52B);
			if (slider) {
				SendMessageA(slider, 0x4AC, 0, 0);
				SendMessageA(slider, 0x406, 1, 0x10000);		// TBM_SETRANGE 0..1
				SendMessageA(slider, 0x405, 1, Options.DetailLevel != 0);	// TBM_SETPOS
			}
			HWND combo = GetDlgItem(window, 0x6ED);
			Options_Modes = Options.AllowHiResModes ? EnumDisplayModes(640, 480, 0x1000, 0x1000, 16)
			                                        : EnumDisplayModes(640, 480, 0x400, 0x300, 16);
			int selected = -1;
			int count = 0;
			for (int index = 0; Options_Modes != NULL && Options_Modes[index * 2] != 0; ++index) {
				int width = Options_Modes[index * 2];
				int height = Options_Modes[index * 2 + 1];
				if (Options.AllowHiResModes || width == 640 || width == 800 || width == 1024) {
					if (width == TempOptions.ScreenWidth && height == TempOptions.ScreenHeight) {
						selected = count;
					}
					char buffer[64];
					sprintf(buffer, "%d x %d x 16", width, height);
					int item = SendMessageA(combo, 0x4BC, 0, (LPARAM)buffer);
					SendMessageA(combo, CB_SETITEMDATA, item, index);
					++count;
				}
			}
			SendMessageA(combo, CB_SETCURSEL, selected, 0);
			Options_Initial_Mode = selected;
			slider = GetDlgItem(window, 0x50F);
			if (slider) {
				SendMessageA(slider, 0x4AC, 0, 0);
				SendMessageA(slider, 0x406, 1, 0x20000);
				SendMessageA(slider, 0x405, 1, Options.Difficulty);
			}
			HWND box = GetDlgItem(window, 0x601);
			if (box) SendMessageA(box, BM_SETCHECK, Options.ActionLines != 0, 0);
			box = GetDlgItem(window, 0x604);
			if (box) SendMessageA(box, BM_SETCHECK, Options.ShowHidden != 0, 0);
			box = GetDlgItem(window, 0x602);
			if (box) SendMessageA(box, BM_SETCHECK, Options.ToolTips != 0, 0);
			slider = GetDlgItem(window, 0x52A);
			if (slider) {
				SendMessageA(slider, 0x4AC, 0, 0);
				SendMessageA(slider, 0x406, 1, 0x60000);
				SendMessageA(slider, 0x405, 1, 6 - Options.ScrollRate);
			}
			int sound = FUN_00407000();
			static int const volume_ids[3] = {0x52F, 0x532, 0x536};
			float const *volumes[3] = {&Options.ScoreVolume, &Options.SoundVolume, &Options.VoiceVolume};
			for (int i = 0; i < 3; ++i) {
				slider = GetDlgItem(window, volume_ids[i]);
				if (slider) {
					SendMessageA(slider, 0x4AE, 0, 0);
					SendMessageA(slider, 0x406, 1, 0xA0000);
					SendMessageA(slider, 0x405, 1, (int)(*volumes[i] * 10.0 + 0.5));
					EnableWindow(slider, sound);
				}
			}
			Options_Ready = true;
			return 0;
		}
	}
	return 0;
}

// ---------------------------------------------------------------------------------------------
// NATIVE ADDITION (lane Y8; not in the original game): the Enhancements page in Yuri's Revenge,
// RA2's native/link/native_enhancements.cpp (lane C3, C7) on the YR tree's dialog code. The port
// gives the page's template under the Options dialog's id while it is created (ra2port_native_page),
// so the game's own dialog code draws it with the Options look; the rows and what they change are
// the host's (platform/sdl/enhancements.cpp, ra2_enh_*). Rows whose code is RA2's only (Game
// interface size, Map zoom, Zoom gestures, HD vehicles: native/yr/README.md, lane Y8) are left off
// the page (the presets still set them in the shared settings; Yuri's Revenge does not read them).
// ---------------------------------------------------------------------------------------------
extern "C" void ra2port_native_page(int on);          // port/src/native_dialogs.cpp
extern "C" int ra2_enh_rows(void);
extern "C" const char *ra2_enh_label(int);
extern "C" const char *ra2_enh_help(int);
extern "C" int ra2_enh_automatic(int);
extern "C" int ra2_enh_value_count(int);
extern "C" const char *ra2_enh_value_name(int, int);
extern "C" int ra2_enh_value_index(int);
extern "C" void ra2_enh_set_value(int, int);
extern "C" const char *ra2_enh_preset(void);
extern "C" void ra2_enh_begin(void);
extern "C" void ra2_enh_set_preset(int);
extern "C" void ra2_enh_accept(void);

namespace {
const int kEnhRows = 11;            // rows the template has (port/src/native_dialogs.cpp)
const int kEnhLabel = 0x7F20, kEnhDropdown = 0x7F10, kEnhPresetText = 0x7F40;
const int kEnhOriginal = 0x7F00, kEnhEnhanced = 0x7F01, kEnhOk = 0x7F02, kEnhCancel = 0x7F03;
const int kEnhHint = 0x695, kEnhTitle = 0x694, kEnhHeading = 0x7F7E, kEnhRowHelp = 0x7F70;
int enh_hint_row = -1;            // a shown row (index into enh_shown), -1: the page

// The host's rows this game shows (indices into ra2_enh_*): every row but RA2's only ones.
int enh_shown[32];
int enh_count = 0;
void Enh_Pick_Rows()
{
	static char const * const ra2_only[] = {"Game interface size", "Map zoom", "Trackpad and wheel zoom",
		"Wheel zoom", "Pinch zoom", "HD vehicles"};	// the zoom row's label names the platform's input
	enh_count = 0;
	for (int i = 0; i < ra2_enh_rows() && enh_count < kEnhRows; ++i) {
		bool skip = false;
		for (char const *label : ra2_only) skip = skip || strcmp(ra2_enh_label(i), label) == 0;
		if (!skip) enh_shown[enh_count++] = i;
	}
}

void Enh_Set_Wide(HWND dialog, int id, wchar_t const *text)
{
	HWND control = GetDlgItem(dialog, id);
	if (control) SendMessageA(control, 0x4B2, 0, (LPARAM)text);
}
void Enh_Set_Text(HWND dialog, int id, char const *text)
{
	wchar_t wide[256];
	int n = 0;
	for (; text && text[n] && n < 255; ++n) wide[n] = (unsigned char)text[n];
	wide[n] = 0;
	Enh_Set_Wide(dialog, id, wide);
}
void Enh_Fill(HWND dialog)
{
	for (int i = 0; i < enh_count; ++i) {
		HWND combo = GetDlgItem(dialog, kEnhDropdown + i);
		if (!combo) continue;
		int row = enh_shown[i];
		for (int k = 0; k < ra2_enh_value_count(row); ++k) SendMessageA(combo, 0x4BC, 0, (LPARAM)ra2_enh_value_name(row, k));
		EnableWindow(combo, !ra2_enh_automatic(row));
	}
}
void Enh_Refresh(HWND dialog)
{
	for (int i = 0; i < kEnhRows; ++i) {
		if (i >= enh_count) {
			ShowWindow(GetDlgItem(dialog, kEnhLabel + i), SW_HIDE);
			ShowWindow(GetDlgItem(dialog, kEnhDropdown + i), SW_HIDE);
			continue;
		}
		Enh_Set_Text(dialog, kEnhLabel + i, ra2_enh_label(enh_shown[i]));
		HWND combo = GetDlgItem(dialog, kEnhDropdown + i);
		if (combo) SendMessageA(combo, CB_SETCURSEL, ra2_enh_value_index(enh_shown[i]), 0);
	}
	Enh_Set_Text(dialog, kEnhPresetText, ra2_enh_preset());
	Enh_Set_Text(dialog, kEnhHint, ra2_enh_help(-1));
	Enh_Set_Text(dialog, kEnhRowHelp, enh_hint_row >= 0 ? ra2_enh_help(enh_shown[enh_hint_row]) : "");
}
BOOL CALLBACK Yr_Enhancements_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int handled = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (handled) return handled;
	int *result = (int *)ra2port_get_dialog_user(window);
	switch (message) {
		case 0x497:
			Enh_Set_Text(window, kEnhTitle, "Enhancements");
			Enh_Set_Text(window, kEnhHeading, "Native additions (not in the original game)");
			Enh_Set_Wide(window, kEnhOk, StringTable::LoadString("GUI:Ok", NULL, __FILE__, __LINE__));
			Enh_Set_Wide(window, kEnhCancel, StringTable::LoadString("GUI:Cancel", NULL, __FILE__, __LINE__));
			Enh_Set_Text(window, kEnhOriginal, "Original");
			Enh_Set_Text(window, kEnhEnhanced, "Enhanced");
			Enh_Fill(window);
			Enh_Refresh(window);
			return 0;
		case WM_COMMAND: {
			int id = wparam & 0xFFFF;
			int code = (int)(wparam >> 16);
			if (id >= kEnhDropdown && id < kEnhDropdown + enh_count) {
				if (code != CBN_SELCHANGE) return 0;
				enh_hint_row = id - kEnhDropdown;
				ra2_enh_set_value(enh_shown[enh_hint_row], (int)SendMessageA(GetDlgItem(window, id), CB_GETCURSEL, 0, 0));
				Enh_Refresh(window);
				return 0;
			}
			if (code != 0) return 0;
			if (id == kEnhOriginal || id == kEnhEnhanced) {
				ra2_enh_set_preset(id == kEnhEnhanced);
				enh_hint_row = -1;
				Enh_Refresh(window);
			} else if (id == kEnhOk) {
				ra2_enh_accept();
				if (result) *result = 1;
			} else if (id == kEnhCancel) {
				if (result) *result = 0;
			}
			return 0;
		}
	}
	return 0;
}
}  // namespace

// Runs the page over the Options dialog, as the Options dialog's runner runs its own pages:
// created, shown, the shell's idle steps until Ok or Cancel, closed. The Options dialog is hidden
// meanwhile (RA2's page also slides the Options column out first; not done here).
void Yr_Run_Enhancements(HWND options)
{
	int result = -1;
	enh_hint_row = -1;
	ra2_enh_begin();
	Enh_Pick_Rows();
	if (options) ShowWindow(options, SW_HIDE);
	ra2port_native_page(1);
	HWND dialog = UI::BeginDialog(0xD5, (DLGPROC)Yr_Enhancements_Proc, 0);
	ra2port_native_page(0);
	if (!dialog) {
		if (options) ShowWindow(options, SW_SHOW);
		return;
	}
	ra2port_set_dialog_user(dialog, &result);
	OwnerDraw::Display_Dialog(dialog);
	while (result < 0) {
		if (OwnerDraw::Dialog_Message_Handler() == true) break;
		Title_Screen_Restore();
	}
	UI::EndDialog(dialog);
	if (options) {
		ShowWindow(options, SW_SHOW);
		InvalidateRect(options, NULL, FALSE);
	}
	fprintf(stderr, "enhancements: page closed (%s)\n", result == 1 ? "accept" : "cancel");
}

void Yr_Enhancements_Button_Text(HWND options)
{
	Enh_Set_Text(options, 0x7F80, "Enhancements");
}
