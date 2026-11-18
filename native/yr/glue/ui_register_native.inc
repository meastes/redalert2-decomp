
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW, LAN), appended to the owner-draw unit after lane Y3's drafts:
// UI::RegisterWindow, YR 0x00622820 (810 bytes, unmatched in ra2-decomp). The network dialogs
// (netdlg2.cpp's game list, host and guest screens; netshare.cpp) make their dialog with
// CreateDialog and register it here instead of through UI::StandardWndProc's WM_INITDIALOG: the
// same preparation (lane Y3's draft above) with the dialog id given, from gamemd.exe's code: the
// owner-draw classes, the controls' images and subclasses, the scaling, the id kept in the record
// (+0x6C), the out-of-game art, the record's flags by id (FUN_0060CAF0, FUN_0060C930, FUN_0060CCC0,
// FUN_0060CDB0 compute the same tests), the layout band, the bands' and draw modes' passes and the
// centring. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
namespace UI {
void RegisterWindow(HWND window, int id)
{
	UI::RegisterComboDropAndNewEditClasses();
	OwnerDraw::CurrentDialogHwnd = window;
	EnumChildWindows(window, OwnerDraw::SetNeedsControlImage, 1);
	OwnerDraw::SetNeedsControlImage(window, 1);
	EnumChildWindows(window, FUN_0060F320, 1);
	EnumChildWindows(window, OwnerDraw::RegisterChildControlProc, 0);
	OwnerDraw::RegisterChildControlProc(window, 0);
	EnumChildWindows(window, OwnerDraw::SetNeedsControlImage, 0);
	OwnerDraw::SetNeedsControlImage(window, 0);
	Resize_Dialogs(window);
	FUN_0060D2C0(window, id);
	if (!Session.FUN_0069BBE0()) {
		FUN_0060CF00(window);
	}
	FUN_0060CAF0(window);
	FUN_0060C930(window);
	FUN_0060CCC0(window);
	FUN_0060CDB0(window);
	EnumChildWindows(window, OwnerDraw::InitCompactDialogControlsProc, 0);
	if (OwnerDraw::TrySetDialogLayoutBand1(window)) {
		int offset[2] = { DAT_007F5BE0, DAT_007F5BEC };
		OwnerDraw::UpdateControlPosition(window, (LPARAM)offset);
	} else {
		OwnerDraw::TrySetDialogLayoutBand2(window);
	}
	EnumChildWindows(window, OwnerDraw::ClassifyLayoutBand, 0);
	EnumChildWindows(window, OwnerDraw::ResetControlDrawModeAndTimerProc, 0);
	Center_Window_Within_Window(window);
}
}
