
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3), appended to the owner-draw unit after ownrdraw_native.inc:
// the combo box and its drop list. Behaviour code for the native build; never part of an exact
// build.
//
// OwnerDraw::ComboBoxHandler, YR 0x00617250 (6,895 bytes, unmatched in ra2-decomp) and
// FUN_0060D540, YR 0x0060D540 (7,636 bytes, the "ComboDropWin" class procedure; unmatched).
// The combo is the RA2 tree's behaviour rewrite of RA2 0x005F4BA0 (native/link/
// behaviour_m14_ownerdraw.cpp, which runs RA2's skirmish and LAN set-up), its messages checked
// against gamemd.exe's (the same narrow/wide add, find and select cases, the arrow click, the
// drop list, 0x497/0x498 item colours at +0x110, the colour-box flag +0xCC, the alternate art
// +0xCD, +0xD0); the drop list is the RA2 tree's matched counterpart unknown_combo_drop_proc_5ead20
// (src/recovered/ownrdraw.cpp) with gamemd.exe's globals: the combo owner DAT_00AC48D0, the open
// drop list DAT_00AC48C0 / its dialog DAT_00AC48C4, DefaultBorderColor, AltBorderColor,
// ListSelectionFillColor, AltSelectionFillColor, AltComboTextColor, ControlInsetPx.
// NATIVE: the items keep their wide text in a record set as the item data (gamemd.exe keeps them in
// the record's nodes), and the open drop list is kept by combo in a side table, as the RA2 tree's
// rewrite does; the port's USER32 combo keeps the narrow text, the selection and the count.
namespace OwnerDraw {
extern int DefaultBorderColor;
extern int AltBorderColor;
extern int ListSelectionFillColor;
extern int AltSelectionFillColor;
extern int AltComboTextColor;
extern int ControlInsetPx;
extern int AltDisabledTextColor;
void __fastcall CopyDimmedBackground(Rect const & rect, HWND hWnd, int alpha);
void DrawBeveledBorder(Surface & surface, Rect const & rect, int offset, int color);
}
void ODDrawArrowBitmap(Surface * surface, Rect const & rect, BOOL upward, BOOL pressed, bool dark);
int ODColorToHiColor(COLORREF color);
void __fastcall FUN_0072A9E0(Rect *rect, int amount);
extern HWND DAT_00AC48D0;                         // 0x00AC48D0 the combo whose list is down
LRESULT CALLBACK FUN_0060D540(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

namespace {
LRESULT YR_Original(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WNDPROC proc = NULL;
	OwnerDraw::DialogProcs.getValue(window, proc);
	return CallWindowProc(proc, window, message, wparam, lparam);
}

struct YRComboItem {
	int Reserved;
	int Data;
	wchar_t *Text;
	YRComboItem *Next;
};
YRComboItem *YRComboItems = NULL;
struct { HWND Combo; HWND Drop; } YRComboDrops[16];

YRComboItem *YR_Combo_Item(HWND window, int index)
{
	YRComboItem *item = (YRComboItem *)YR_Original(window, CB_GETITEMDATA, index, 0);
	for (YRComboItem *known = YRComboItems; known != NULL; known = known->Next) {
		if (known == item) {
			return item;
		}
	}
	return NULL;
}

void YR_Combo_Free(HWND window, int index)
{
	YRComboItem *item = YR_Combo_Item(window, index);
	if (item == NULL) {
		return;
	}
	for (YRComboItem **link = &YRComboItems; *link != NULL; link = &(*link)->Next) {
		if (*link == item) {
			*link = item->Next;
			break;
		}
	}
	delete [] item->Text;
	delete item;
}

HWND &YR_Combo_Drop(HWND combo, bool make)
{
	static HWND none;
	for (int index = 0; index < 16; index++) {
		if (YRComboDrops[index].Combo == combo && YRComboDrops[index].Drop != NULL) {
			return YRComboDrops[index].Drop;
		}
	}
	if (make) {
		for (int index = 0; index < 16; index++) {
			if (YRComboDrops[index].Drop == NULL) {
				YRComboDrops[index].Combo = combo;
				return YRComboDrops[index].Drop;
			}
		}
	}
	none = NULL;
	return none;
}

// An item's wide text (the record's, else the narrow text of the port's list widened).
bool YR_Combo_Text(HWND window, int index, wchar_t *out, int size)
{
	out[0] = L'\0';
	YRComboItem *item = YR_Combo_Item(window, index);
	if (item != NULL && item->Text != NULL) {
		int n = 0;
		for (; item->Text[n] && n < size - 1; n++) {
			out[n] = item->Text[n];
		}
		out[n] = L'\0';
		return true;
	}
	char narrow[256];
	LRESULT length = YR_Original(window, CB_GETLBTEXTLEN, index, 0);
	if (length < 0 || length >= (LRESULT)sizeof(narrow) || YR_Original(window, CB_GETLBTEXT, index, (LPARAM)narrow) < 0) {
		return false;
	}
	int n = 0;
	for (; narrow[n] && n < size - 1; n++) {
		out[n] = (unsigned char)narrow[n];
	}
	out[n] = L'\0';
	return true;
}
}

LRESULT CALLBACK OwnerDraw::ComboBoxHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WinData *data = Find_Data(window);
	if (data != NULL) {
		switch (message) {
		case 0x4BB:
		case 0x4BC:
		case 0x4C1:
		case 0x4C2: {
			bool wide = message >= 0x4C1;
			bool add = message == 0x4BC || message == 0x4C2;
			int length = 0;
			char narrow[512];
			if (lparam) {
				if (wide) {
					while (((wchar_t const *)lparam)[length]) {
						length++;
					}
				} else {
					while (((char const *)lparam)[length]) {
						length++;
					}
				}
			}
			if (wide) {
				WideToCharString(narrow, (wchar_t const *)lparam, sizeof(narrow) - 1);
			} else {
				int n = length < (int)sizeof(narrow) - 1 ? length : (int)sizeof(narrow) - 1;
				memcpy(narrow, (char const *)lparam, n);
				narrow[n] = '\0';
			}
			LRESULT index = YR_Original(window, add ? CB_ADDSTRING : CB_INSERTSTRING, add ? 0 : wparam, (LPARAM)narrow);
			if (index < 0) {
				return index;
			}
			YRComboItem *item = new YRComboItem;
			item->Reserved = 0;
			item->Data = 0;
			item->Text = new wchar_t[length + 1];
			for (int i = 0; i < length; i++) {
				item->Text[i] = wide ? ((wchar_t const *)lparam)[i] : (wchar_t)(unsigned char)((char const *)lparam)[i];
			}
			item->Text[length] = L'\0';
			item->Next = YRComboItems;
			YRComboItems = item;
			YR_Original(window, CB_SETITEMDATA, index, (LPARAM)item);
			return index;
		}
		case 0x4B8:
		case 0x4B9:
		case 0x4BA:
		case 0x4BE:
		case 0x4BF:
		case 0x4C0: {
			bool wide = message >= 0x4BE;
			bool exact = message == 0x4B9 || message == 0x4BF;
			bool select = message == 0x4BA || message == 0x4C0;
			wchar_t want[256];
			int wlen = 0;
			if (lparam) {
				for (; wlen < 255; wlen++) {
					wchar_t c = wide ? ((wchar_t const *)lparam)[wlen] : (wchar_t)(unsigned char)((char const *)lparam)[wlen];
					if (c == 0) {
						break;
					}
					want[wlen] = c;
				}
			}
			want[wlen] = L'\0';
			int count = (int)YR_Original(window, CB_GETCOUNT, 0, 0);
			if (count <= 0) {
				return -1;
			}
			int start = ((int)wparam + 1 < 0 || (int)wparam + 1 >= count) ? 0 : (int)wparam + 1;
			for (int n = 0; n < count; n++) {
				int i = (start + n) % count;
				wchar_t text[256];
				if (!YR_Combo_Text(window, i, text, 256)) {
					continue;
				}
				bool match = exact ? _wcsicmp(text, want) == 0 : _wcsnicmp(text, want, wlen) == 0;
				if (!match) {
					continue;
				}
				if (select) {
					YR_Original(window, CB_SETCURSEL, i, 0);
					InvalidateRect(window, NULL, TRUE);
				}
				return i;
			}
			return -1;
		}
		case 0x4B3:
		case 0x4B5: {
			if (!lparam || (int)wparam <= 0) {
				return 0;
			}
			int selected = (int)YR_Original(window, CB_GETCURSEL, 0, 0);
			wchar_t text[256];
			text[0] = L'\0';
			if (selected >= 0) {
				YR_Combo_Text(window, selected, text, 256);
			}
			int n = 0;
			if (message == 0x4B3) {
				wchar_t *out = (wchar_t *)lparam;
				for (; text[n] && n < (int)wparam - 1; n++) {
					out[n] = text[n];
				}
				out[n] = L'\0';
			} else {
				char *out = (char *)lparam;
				for (; text[n] && n < (int)wparam - 1; n++) {
					out[n] = (char)text[n];
				}
				out[n] = '\0';
			}
			return n;
		}
		case CB_GETITEMDATA:
		case LB_GETITEMDATA: {
			YRComboItem *item = YR_Combo_Item(window, (int)wparam);
			if (item != NULL) {
				return item->Data;
			}
			return YR_Original(window, CB_GETITEMDATA, wparam, lparam);
		}
		case CB_SETITEMDATA:
		case LB_SETITEMDATA: {
			YRComboItem *item = YR_Combo_Item(window, (int)wparam);
			if (item != NULL) {
				item->Data = (int)lparam;
				return 0;
			}
			return YR_Original(window, CB_SETITEMDATA, wparam, lparam);
		}
		case CB_DELETESTRING:
			YR_Combo_Free(window, (int)wparam);
			break;
		case CB_RESETCONTENT: {
			int count = (int)YR_Original(window, CB_GETCOUNT, 0, 0);
			for (int i = 0; i < count; i++) {
				YR_Combo_Free(window, i);
			}
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK: {
			VocClass::PlayGlobal(Rule->GUIComboOpenSound, 1.0f, 0x2000);
			RECT client;
			GetClientRect(window, &client);
			if ((int)(lparam & 0xFFFF) > client.right - 20) {
				LRESULT dropped = SendMessage(window, CB_GETDROPPEDSTATE, 0, 0);
				PostMessage(window, CB_SHOWDROPDOWN, dropped != 1, 0);
			}
			return 0;
		}
		case CB_GETDROPPEDSTATE:
			return YR_Combo_Drop(window, false) != NULL ? 1 : 0;
		case CB_SHOWDROPDOWN: {
			HWND parent = GetParent(window);
			HWND &slot = YR_Combo_Drop(window, false);
			if (!wparam) {
				if (slot == NULL) {
					return 1;
				}
				HWND drop = slot;
				slot = NULL;
				ReleaseCapture();
				SendMessage(parent, 0x4A9, (WPARAM)drop, 0);
				DestroyWindow(drop);
				FUN_00610570(drop, 0);
				YR_Original(window, CB_SHOWDROPDOWN, 0, 0);
				InvalidateRect(parent, NULL, FALSE);
				return 1;
			}
			if (slot != NULL) {
				return 1;
			}
			SetFocus(window);
			RECT client;
			RECT display;
			RECT parent_display;
			GetClientRect(window, &client);
			Get_Display_Rect(window, &display);
			Get_Display_Rect(parent, &parent_display);
			int count = (int)YR_Original(window, CB_GETCOUNT, 0, 0);
			int item_height = (int)YR_Original(window, CB_GETITEMHEIGHT, 0, 0);
			if (item_height < 1) {
				item_height = 1;
			}
			int height = count * item_height + 4;
			if (display.bottom + height > parent_display.bottom - 2 * item_height) {
				height = parent_display.bottom - (2 * item_height + 4) - display.bottom;
				if (height < item_height) {
					height = parent_display.bottom - display.bottom;
					height -= height % item_height;
				}
			}
			HWND drop = CreateWindowEx(0, "ComboDropWin", NULL, WS_CHILD, display.left - parent_display.left,
				client.bottom + display.top - parent_display.top + 2, client.right, height, parent, NULL, Game::hInstance, window);
			if (drop == NULL) {
				return 0;
			}
			RegisterChildControlProc(drop, 0);
			YR_Combo_Drop(window, true) = drop;
			YR_Original(window, CB_SHOWDROPDOWN, 1, 0);
			SendMessage(drop, 0x7E8, 0, 0);
			SendMessage(parent, 0x4A9, (WPARAM)drop, 1);
			SetCapture(drop);
			ShowWindow(drop, SW_SHOWNORMAL);
			InvalidateRect(drop, NULL, FALSE);
			InvalidateRect(parent, NULL, FALSE);
			return 1;
		}
		case 0x497:
			memset(&YR_REC(int, data, 0x110), 0xFF, 50 * sizeof(int));
			break;
		case 0x498:
			if (wparam < 50) {
				(&YR_REC(int, data, 0x110))[wparam] = (int)lparam;
			}
			break;
		case WM_DESTROY:
			SendMessage(window, CB_SHOWDROPDOWN, 0, 0);
			break;
		case 0x4DD:
			YR_REC(bool, data, 0xCC) = lparam == 1;
			break;
		case 0x4DE:
			YR_REC(int, data, 0xD0) = (int)lparam;
			break;
		case 0x4F1:
			YR_REC(bool, data, 0xCD) = lparam == 1;
			break;
		case 0x4DA:
		case 0x4DB:
			return SendMessage(GetParent(window), message, wparam, lparam);
		}
	}
	if (data == NULL || message != WM_PAINT) {
		if (message == WM_ERASEBKGND) {
			return 1;
		}
		LRESULT result = YR_Original(window, message, wparam, lparam);
		if (data != NULL && (message == CB_SETCURSEL || message == CB_RESETCONTENT)) {
			InvalidateRect(window, NULL, TRUE);
		}
		return result;
	}
	if (!data->ScrollBar.skipDraw) {
		RECT where;
		Get_Display_Rect(window, &where);
		Rect area(where.left, where.top, where.right - where.left, where.bottom - where.top);
		Surface *surface = AlternateSurface;
		LONG style = GetWindowLong(window, GWL_STYLE);
		bool disabled = (style & WS_DISABLED) != 0;
		bool dropped = YR_Combo_Drop(window, false) != NULL;
		CopyDimmedBackground(area, window, YR_REC(int, data, 0xC8));
		FUN_00621B80(&area, surface, 0, YR_REC(int, data, 0xC8));
		DrawBeveledBorder(*surface, area, 1, -1);
		Rect arrow(area.X + area.Width - 20, area.Y + 1, 19, area.Height - 2);
		ODDrawArrowBitmap(surface, arrow, dropped, dropped, YR_REC(bool, data, 0xCD));
		if (disabled) {
			FUN_00621B80(&area, surface, 0, YR_REC(int, data, 0xC8));
		}
		if ((style & 3) == CBS_DROPDOWNLIST) {
			int selected = (int)YR_Original(window, CB_GETCURSEL, 0, 0);
			wchar_t text[256];
			text[0] = L'\0';
			if (selected >= 0) {
				YR_Combo_Text(window, selected, text, 256);
			}
			unsigned color = disabled ? AltDisabledTextColor : PrimaryTextColor;
			int *colors = &YR_REC(int, data, 0x110);
			if (YR_REC(bool, data, 0xCC) && selected >= 0 && selected < 50 && colors[selected] >= 0) {
				unsigned fill = colors[selected];
				Rect swatch(area.X, area.Y, area.Width - 20, area.Height);
				FUN_0072A9E0(&swatch, 2);
				surface->Fill_Rect(swatch, DSurface::Build_Hicolor_Pixel(fill & 0xFF, (fill >> 8) & 0xFF, (fill >> 16) & 0xFF));
				color = fill;
			}
			RECT box = {area.X + 2, area.Y, area.X + area.Width - 20, area.Y + area.Height};
			int room = box.right - box.left;
			BitFont *font = data->Wide.font;
			int length = wcslen(text);
			if (font != NULL) {
				while (length > 3 && font->FUN_00433ED0(text, 0) > room) {
					length--;
					text[length - 3] = L'.';
					text[length - 2] = L'.';
					text[length - 1] = L'.';
					text[length] = L'\0';
				}
			}
			if (text[0]) {
				DrawWideText(surface, text, (LTRBStruct *)&box, font, color, 4, 0xC, 0, 0, 0);
			}
		}
	}
	ValidateRect(window, NULL);
	return 0;
}

// FUN_0060D540: the drop list (see above).
LRESULT CALLBACK FUN_0060D540(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	Get_Display_Rect(window, &rect);
	RECT client;
	GetClientRect(window, &client);
	Rect fill;
	RECT parent_rect;
	memset(&parent_rect, 0, sizeof(parent_rect));
	int scrollbar_width = 2 * OwnerDraw::ControlInsetPx + 18;
	int need_scrollbar = -1;
	int max_top = 0;
	HWND parent = NULL;
	if (DAT_00AC48D0 != NULL) {
		parent = GetParent(DAT_00AC48D0);
	}
	OwnerDraw::WinData *parent_data = NULL;
	if (parent != NULL) {
		parent_data = Find_Data(parent);
		Get_Display_Rect(parent, &parent_rect);
	}
	DAT_00AC48C4 = parent;
	DAT_00AC48C0 = window;
	OwnerDraw::WinData *data = Find_Data(window);
	if (data == NULL) {
		FUN_004068E0("ComboBox dropdown windata = NULL\n");
	}
	OwnerDraw::WinData *master = (DAT_00AC48D0 != NULL) ? Find_Data(DAT_00AC48D0) : NULL;
	if (message != CB_GETCOUNT && message != CB_GETITEMHEIGHT && message != WM_VSCROLL) {
		int item_count = (int)SendMessage(DAT_00AC48D0, CB_GETCOUNT, 0, 0);
		int item_height = (int)SendMessage(DAT_00AC48D0, CB_GETITEMHEIGHT, 0, 0);
		if (item_height <= 1) {
			item_height = 1;
		}
		need_scrollbar = item_count * item_height > client.bottom - client.top;
		max_top = item_count - (client.bottom - client.top) / item_height;
		if (data != NULL) {
			if ((uintptr_t)data->attachedWindow > 1) {
				SCROLLINFO info;
				info.cbSize = sizeof(info);
				info.fMask = SIF_RANGE | SIF_POS;
				info.nMin = 0;
				info.nMax = max_top;
				info.nPos = YR_REC(int, data, 0xF0);
				SendMessage(data->attachedWindow, SBM_SETSCROLLINFO, 0, (LPARAM)&info);
			}
			if (data->attachedWindow != NULL) {
				BringWindowToTop(data->attachedWindow);
			}
		}
	}
	switch (message) {
	case WM_ERASEBKGND:
		return 0;

	case WM_CREATE:
		SetCapture(window);
		DAT_00AC48D0 = (HWND)((CREATESTRUCT *)lparam)->lpCreateParams;
		SetTimer(window, 0, 20, NULL);
		return 0;

	case WM_NCDESTROY:
		DAT_00AC48C0 = NULL;
		DAT_00AC48C4 = NULL;
		ReleaseCapture();
		KillTimer(window, 0);
		break;

	case WM_VSCROLL: {
		int top = (int)SendMessage(data->attachedWindow, SBM_GETPOS, 0, 0);
		if (top != SendMessage(window, CB_GETTOPINDEX, 0, 0)) {
			SendMessage(window, CB_SETTOPINDEX, top, 0);
		}
		break;
	}

	case WM_TIMER: {
		HWND capture = GetCapture();
		if (capture == window) {
			ReleaseCapture();
			Sleep(0);
			SetCapture(capture);
		}
		break;
	}

	case CB_GETTOPINDEX:
		if (data != NULL) {
			return YR_REC(int, data, 0xF0);
		}
		break;

	case WM_MOUSEMOVE: {
		int x = lparam & 0xFFFF;
		int y = (unsigned)lparam >> 16;
		int index = (int)SendMessage(window, 0x4E8, 0, (y << 16) | x);
		if (index != -1) {
			if (YR_REC(int, data, 0xE8) != index) {
				InvalidateRect(window, NULL, FALSE);
			}
			YR_REC(int, data, 0xE8) = index;
			HWND tip = GetDlgItem(parent, 0x695);
			if (tip != NULL) {
				WideWstring text = FUN_00603F00(parent, DAT_00AC48D0, index);
				if (wcscmp(text.get(), DAT_00887734) != 0) {
					SendMessage(tip, 0x4B2, 0, (LPARAM)text.text());
				} else {
					text = FUN_00603F60(parent, DAT_00AC48D0);
					if (wcscmp(text.get(), DAT_00887734) != 0) {
						SendMessage(tip, 0x4B2, 0, (LPARAM)text.text());
					}
				}
			}
		}
		return 0;
	}

	case CB_SETTOPINDEX: {
		int top = (int)wparam;
		int item_count = (int)SendMessage(DAT_00AC48D0, CB_GETCOUNT, 0, 0);
		int item_height = (int)SendMessage(DAT_00AC48D0, CB_GETITEMHEIGHT, 0, 0);
		if (!item_count || !item_height) {
			break;
		}
		int visible = (client.bottom - client.top) / item_height;
		if (top < 0) {
			top = 0;
		}
		int limit = item_count - visible;
		if (limit > 0) {
			if (top > limit) {
				top = limit;
			}
		} else {
			top = 0;
		}
		if (top != YR_REC(int, data, 0xF0)) {
			YR_REC(int, data, 0xF0) = top;
			InvalidateRect(window, NULL, FALSE);
		}
		return 0;
	}

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK: {
		int x = lparam & 0xFFFF;
		int y = (unsigned)lparam >> 16;
		if (x > client.right && x < client.right + data->scrollBarWidth && y > 0 && y < client.bottom) {
			SendMessage(data->attachedWindow, message, wparam, lparam);
			break;
		}
		VocClass::PlayGlobal(Rule->GUIComboCloseSound, 1.0f, 0x2000);
		HWND owner = DAT_00AC48D0;
		if (x >= 0 && y >= 0 && x <= client.right && y <= client.bottom) {
			int item_height = (int)SendMessage(owner, CB_GETITEMHEIGHT, 0, 0);
			int item_count = (int)SendMessage(owner, CB_GETCOUNT, 0, 0);
			int index = y / (item_height > 0 ? item_height : 1);
			if (data != NULL) {
				index += YR_REC(int, data, 0xF0);
			}
			int clamped = index < 0 ? 0 : index;
			if (item_count - 1 < clamped) {
				clamped = item_count - 1;
			}
			SendMessage(owner, CB_SETCURSEL, clamped, 0);
			int id = GetWindowLong(owner, GWL_ID);
			if (need_scrollbar) {
				DestroyWindow(data->attachedWindow);
			}
			ReleaseCapture();
			SendMessage(owner, CB_SHOWDROPDOWN, 0, 0);
			PostMessage(parent, WM_COMMAND, (id & 0xFFFF) | (CBN_SELCHANGE << 16), (LPARAM)owner);
			return 0;
		}
		if (need_scrollbar) {
			DestroyWindow(data->attachedWindow);
		}
		ReleaseCapture();
		SendMessage(owner, CB_SHOWDROPDOWN, 0, 0);
		return 0;
	}

	case 0x4E8: {
		int x = lparam & 0xFFFF;
		int y = (unsigned)lparam >> 16;
		int result = -1;
		if (x >= 0 && y >= 0 && x < client.right && y < client.bottom) {
			int item_height = (int)SendMessage(DAT_00AC48D0, CB_GETITEMHEIGHT, 0, 0);
			int item_count = (int)SendMessage(DAT_00AC48D0, CB_GETCOUNT, 0, 0);
			int index = y / (item_height > 0 ? item_height : 1) + YR_REC(int, data, 0xF0);
			int clamped = index < 0 ? 0 : index;
			if (item_count - 1 < clamped) {
				clamped = item_count - 1;
			}
			result = clamped;
		}
		return result;
	}

	case 0x4EA:
		return (LRESULT)DAT_00AC48D0;

	case 0x7E8:
		YR_REC(int, data, 0xE8) = (int)SendMessage(DAT_00AC48D0, CB_GETCURSEL, 0, 0);
		return 0;

	case WM_PAINT: {
		BitFont *font = parent_data != NULL ? parent_data->Wide.font : NULL;
		if (font == NULL) {
			font = BitFont::Instance;
		}
		int item_count = (int)SendMessage(DAT_00AC48D0, CB_GETCOUNT, 0, 0);
		int item_height = (int)SendMessage(DAT_00AC48D0, CB_GETITEMHEIGHT, 0, 0);
		if (item_height < 1) {
			item_height = 1;
		}
		int client_height = client.bottom - client.top;
		Rect dest_rect(rect.left, rect.top, rect.right - rect.left, client_height);
		int selected = YR_REC(int, data, 0xE8);
		Rect src(rect.left - parent_rect.left, rect.top - parent_rect.top, client.right, client.bottom);
		if (data->cachedSurface == NULL) {
			Rect dst(0, 0, client.right, client.bottom);
			BSurface *surface = new BSurface(client.right, client.bottom, 2);
			data->cachedSurface = surface;
			OwnerDraw::CachedSurfaceCount++;
			if (parent_data != NULL && parent_data->cachedSurface != NULL) {
				surface->Blit_From(dst, *parent_data->cachedSurface, src, false, true);
			}
			int total = client.right * client.bottom;
			unsigned short *pixels = (unsigned short *)surface->Lock();
			for (int i = 0; i < total; i++) {
				pixels[i] = OD_Blend_Color(pixels[i], 0, (unsigned char)YR_REC(int, data, 0xC8));
			}
			surface->Unlock();
		}
		AlternateSurface->Blit_From(dest_rect, *data->cachedSurface, Rect(0, 0, dest_rect.Width, dest_rect.Height), false, true);
		bool alternate = master != NULL && YR_REC(bool, master, 0xCD);
		int color = ODColorToHiColor(alternate ? OwnerDraw::AltBorderColor : OwnerDraw::DefaultBorderColor);
		OwnerDraw::DrawBeveledBorder(*AlternateSurface, dest_rect, 1, color);
		if (need_scrollbar && data->attachedWindow != NULL) {
			Rect outer(dest_rect);
			outer.Width += scrollbar_width;
			OwnerDraw::DrawBeveledBorder(*AlternateSurface, outer, 1, color);
		}
		int first = YR_REC(int, data, 0xF0);
		for (int index = first; index < item_count; index++) {
			int width = rect.right - rect.left;
			Rect row(rect.left, (index - first) * item_height + rect.top, width, item_height);
			if ((index - first) * item_height + item_height > client_height) {
				break;
			}
			wchar_t text[128];
			YR_Combo_Text(DAT_00AC48D0, index, text, 128);
			if (index == selected) {
				fill = row;
				AlternateSurface->Fill_Rect(fill, ODColorToHiColor(alternate ? OwnerDraw::AltSelectionFillColor : OwnerDraw::ListSelectionFillColor));
			}
			RECT text_rect = {row.X + 3, row.Y, row.X + width, row.Y + item_height};
			unsigned text_color = alternate ? OwnerDraw::AltComboTextColor : OwnerDraw::PrimaryTextColor;
			int *colors = master != NULL ? &YR_REC(int, master, 0x110) : NULL;
			if (index < 50 && colors != NULL && colors[index] >= 0) {
				text_color = colors[index];
				if (YR_REC(bool, master, 0xCC)) {
					Rect swatch = row;
					FUN_0072A9E0(&swatch, 2);
					AlternateSurface->Fill_Rect(swatch, ODColorToHiColor(text_color));
				}
			}
			int max_width = client.right - 20;
			int length = wcslen(text);
			while (length > 3 && font->FUN_00433ED0(text, 0) > max_width) {
				length--;
				text[length - 3] = L'.';
				text[length - 2] = L'.';
				text[length - 1] = L'.';
				text[length] = L'\0';
			}
			OwnerDraw::DrawWideText(AlternateSurface, text, (LTRBStruct *)&text_rect, font, text_color, 4, 12, 0, 0, 0);
		}
		AlternateSurface->Lock();
		DAT_00887308->Lock();
		RECT window_rect;
		GetWindowRect(window, &window_rect);
		Rect screen(dest_rect);
		screen.X += window_rect.left - rect.left;
		screen.Y += window_rect.top - rect.top;
		DAT_00887308->Blit_From(screen, *AlternateSurface, dest_rect, false, true);
		DAT_00887308->Unlock();
		AlternateSurface->Unlock();
		ValidateRect(window, NULL);
		return 0;
	}

	case WM_DESTROY:
		Remove_Modeless_Dialog(window);
		break;
	}

	if (need_scrollbar == 1) {
		if (data != NULL && data->attachedWindow == NULL) {
			parent = GetParent(window);
			RECT parent_display;
			RECT drop_display;
			Get_Display_Rect(parent, &parent_display);
			Get_Display_Rect(window, &drop_display);
			int left = drop_display.left - parent_display.left;
			int top = drop_display.top - parent_display.top;
			bool alternate = master != NULL && YR_REC(bool, master, 0xCD);
			HWND scroll = CreateWindowEx(0, "Scrollbar", NULL, 0x50010001, left - scrollbar_width + client.right, top + client.top,
				scrollbar_width, drop_display.bottom - drop_display.top, parent, NULL, Game::hInstance, NULL);
			data->attachedWindow = scroll;
			data->scrollBarWidth = scrollbar_width;
			OwnerDraw::RegisterChildControlProc(scroll, 0);
			OwnerDraw::WinData *scroll_data = Find_Data(scroll);
			if (scroll_data != NULL) {
				scroll_data->ownerWindow = window;
				YR_REC(bool, scroll_data, 0xCD) = YR_REC(bool, data, 0xCD);
			}
			SCROLLINFO info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_RANGE | SIF_POS;
			info.nMin = 0;
			info.nMax = max_top;
			info.nPos = YR_REC(int, data, 0xF0);
			SendMessage(scroll, SBM_SETSCROLLINFO, 0, (LPARAM)&info);
			SendMessage(scroll, 0x4F1, 0, alternate);
			SetWindowPos(window, NULL, 0, 0, drop_display.right - drop_display.left - scrollbar_width, drop_display.bottom - drop_display.top, SWP_NOMOVE);
			ShowWindow(scroll, SW_SHOW);
			BringWindowToTop(scroll);
			InvalidateRect(scroll, NULL, FALSE);
			UpdateWindow(scroll);
			SendMessage(scroll, 0x4A9, (WPARAM)scroll, 1);
			SendMessage(scroll, 0x4D2, 0, 1);
			RECT validate = {left, top, left + client.right + 1, top + client.bottom + 1};
			ValidateRect(parent, &validate);
		}
	} else if (need_scrollbar == 0 && data != NULL && data->attachedWindow != NULL && !data->paintDisabled) {
		DestroyWindow(data->attachedWindow);
		FUN_00610570(data->attachedWindow, 0);
		data->attachedWindow = NULL;
		data->scrollBarWidth = 0;
		SetWindowPos(window, NULL, 0, 0, client.right - client.left + scrollbar_width, client.bottom - client.top, SWP_NOMOVE);
		parent = GetParent(window);
		RECT validate_parent;
		Get_Display_Rect(parent, &validate_parent);
		RECT validate = {rect.left - validate_parent.left, rect.top - validate_parent.top, 0, 0};
		validate.right = validate.left + client.right + scrollbar_width;
		validate.bottom = validate.top + client.bottom + 1;
		ValidateRect(parent, &validate);
	}
	return DefWindowProc(window, message, wparam, lparam);
}

// OwnerDraw::NewEditHandler, YR 0x00614B30 (2,215 bytes, unmatched in ra2-decomp; RA2 counterpart
// 0x005F22A0): the "NewEdit" control (a list box standing in for an edit). The RA2 tree's
// behaviour rewrite (native/link/behaviour_m14_ownerdraw.cpp) with the YR record names: the text
// is Wide.editText, its limit Wide.textLimit, the caret Wide.caretIndex, the scroll
// Wide.scrollStart, the blink state Wide.caretBlink (published in DAT_00AC48C8 for the drawer
// FUN_00623880, as gamemd.exe does); Return and Tab notify the dialog (0x4DA, 0x4DB).
void __fastcall FUN_00623880(Surface *surface, Rect *rect, wchar_t *text, int caret, BitFont *font, unsigned color,
	int *scroll, int focus, int password, int p10, int p11);
extern int DAT_00AC48C8;

LRESULT CALLBACK OwnerDraw::NewEditHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WinData *data = Find_Data(window);
	if (data == NULL || data->Wide.editText == NULL) {
		return YR_Original(window, message, wparam, lparam);
	}
	WideWstring &text = *data->Wide.editText;
	HWND parent = GetParent(window);
	switch (message) {
	case WM_KEYDOWN:
	case WM_KEYUP:
		if (wparam == VK_TAB) {
			if (message == WM_KEYDOWN) {
				SendMessage(parent, 0x4DB, (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? 1 : 0, (LPARAM)window);
			}
			return 0;
		}
		if (message == WM_KEYDOWN && wparam == VK_RETURN) {
			SendMessage(parent, 0x4DA, 0, (LPARAM)window);
			return 0;
		}
		break;

	case 0x497: {
		RECT rect;
		RECT client;
		RECT parent_rect;
		GetWindowRect(window, &rect);
		GetClientRect(window, &client);
		GetWindowRect(parent, &parent_rect);
		SetWindowPos(window, NULL, rect.left - parent_rect.left + 1, rect.top - parent_rect.top + 1, client.right - 2, client.bottom - 2, SWP_SHOWWINDOW);
		return 0;
	}

	case EM_LIMITTEXT:
		data->Wide.textLimit = (int)wparam;
		if (wparam) {
			text.truncate((unsigned)wparam);
		}
		break;

	case 0x4B3: {
		wchar_t *out = (wchar_t *)lparam;
		wchar_t const *from = text.get();
		int i = 0;
		for (; from != NULL && from[i] && i < (int)wparam - 1; i++) {
			out[i] = from[i];
		}
		out[i] = L'\0';
		return i;
	}

	case 0x4B5: {
		char *out = (char *)lparam;
		WideToCharString(out, text.get(), (int)wparam - 1);
		out[wparam - 1] = '\0';
		return strlen(out);
	}

	case WM_GETTEXTLENGTH:
		return text.length();

	case 0x4B2:
	case 0x4B4:
		text.set(L"");
		data->Wide.caretIndex = 0;
		data->Wide.scrollStart = 0;
		if (data->Wide.text != NULL) {
			text.set(data->Wide.text);
		}
		if (data->Wide.textLimit) {
			text.truncate((unsigned)data->Wide.textLimit);
		}
		data->Wide.caretIndex = text.length();
		InvalidateRect(window, NULL, TRUE);
		break;

	case WM_CHAR:
		if (wparam == VK_BACK) {
			if (text.length()) {
				text.truncate(text.length() - 1);
			}
		} else if (wparam >= 0x20 && (!data->Wide.textLimit || (int)text.length() < data->Wide.textLimit)) {
			text.insert((wchar_t)wparam, text.length());
		} else {
			return 0;
		}
		data->Wide.caretIndex = text.length();
		InvalidateRect(window, NULL, TRUE);
		SendMessage(parent, WM_COMMAND, (WPARAM)((GetDlgCtrlID(window) & 0xFFFF) | (EN_CHANGE << 16)), (LPARAM)window);
		return 0;

	case WM_SETFOCUS:
		data->Wide.caretBlink = 0;
		SetTimer(window, 0, 1000, NULL);
		InvalidateRect(window, NULL, FALSE);
		break;

	case WM_KILLFOCUS:
		KillTimer(window, 0);
		InvalidateRect(window, NULL, FALSE);
		break;

	case WM_TIMER:
		data->Wide.caretBlink ^= 1;
		InvalidateRect(window, NULL, FALSE);
		return 0;

	case WM_ERASEBKGND:
	case WM_PAINT: {
		RECT display;
		char class_name[32] = "";
		Get_Display_Rect(window, &display);
		GetClassName(parent, class_name, sizeof(class_name));
		bool combo = strcmp(class_name, "ComboBox") == 0;
		Rect background(display.left, display.top, display.right - display.left + 1, display.bottom - display.top + 1);
		CopyDimmedBackground(background, window, YR_REC(int, data, 0xC8));
		if (!combo) {
			DrawBeveledBorder(*AlternateSurface, background, 1, -1);
		}
		Rect area(display.left + 2, display.top, display.right - (display.left + 2) + 1, display.bottom - display.top + 1);
		DAT_00AC48C8 = data->Wide.caretBlink;
		FUN_00623880(AlternateSurface, &area, (wchar_t *)text.get(), data->Wide.caretIndex, data->Wide.font, PrimaryTextColor,
			&data->Wide.scrollStart, data->Wide.hasFocus, ((unsigned)data->Wide.styleFlags >> 5) & 1, 0, 0);
		ValidateRect(window, NULL);
		return message == WM_ERASEBKGND ? 1 : 0;
	}

	case WM_LBUTTONDOWN:
		SetFocus(window);
		return 0;
	}
	return YR_Original(window, message, wparam, lparam);
}
