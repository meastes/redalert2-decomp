
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y12), appended to the owner-draw unit after ownrdraw_combo_native.inc:
// OwnerDraw::ListBoxHandler, gamemd.exe 0x00618D40 (14,666 bytes, unmatched in the YR tree; the
// campaign dialog's list of campaigns reached it as a link-closure trap). Behaviour code: Red Alert 2's
// rewrite of its list procedure (game.exe 0x005F6690, native/link/behaviour_m14_ownerdraw.cpp
// unknown_list_proc_5f6690) in the YR tree's names, drawn as the YR combo's drop list draws its rows
// (FUN_0060D540 above): the dimmed background and the beveled border, the rows in the control's font,
// the selected row filled with ListSelectionFillColor. Messages: the wide adds and inserts 0x4C8, 0x4C9,
// 0x4CC, 0x4CD, 0x4CE (gamemd.exe's own; the port's list keeps the narrow text, the wide text is kept
// here), the list's own selection (LB_SETSEL, LB_SETCURSEL, LB_GETSEL, LB_GETCURSEL, LB_GETSELCOUNT,
// LB_GETSELITEMS, LB_SELITEMRANGE; the original never lets the Win32 list hold one), the click, the
// double click (LBN_DBLCLK) and the right click clearing a multiple-selection list.
// NOT YET: the columns and cells (0x4A6, 0x4A8: the LAN lobby's player list) answer -1 and draw
// nothing; the scroll bar is the port's. Never part of an exact build.
// ---------------------------------------------------------------------------------------------
namespace {
struct YRListRows {
	HWND Window;
	int Current;               // the current row (-1: none)
	int Count;                 // rows with a selection flag
	int Capacity;
	int *Flags;
	int TextCount;
	int TextCapacity;
	wchar_t **Texts;           // the rows' wide text, by row
};
YRListRows YRLists[16];

YRListRows *YR_List(HWND window, bool make)
{
	for (int index = 0; index < 16; index++) {
		if (YRLists[index].Window == window) return &YRLists[index];
	}
	if (make) {
		for (int index = 0; index < 16; index++) {
			if (YRLists[index].Window == NULL) {
				memset(&YRLists[index], 0, sizeof(YRLists[index]));
				YRLists[index].Window = window;
				YRLists[index].Current = -1;
				return &YRLists[index];
			}
		}
	}
	return NULL;
}

void YR_List_Clear_Texts(YRListRows *list)
{
	for (int row = 0; row < list->TextCount; row++) delete [] list->Texts[row];
	list->TextCount = 0;
}

void YR_List_Free(HWND window)
{
	YRListRows *list = YR_List(window, false);
	if (list == NULL) return;
	YR_List_Clear_Texts(list);
	delete [] list->Texts;
	delete [] list->Flags;
	memset(list, 0, sizeof(*list));
}

void YR_List_Insert_Text(YRListRows *list, int row, wchar_t const *text)
{
	if (list->TextCount == list->TextCapacity) {
		int capacity = list->TextCapacity < 10 ? 10 : list->TextCapacity * 2;
		wchar_t **texts = new wchar_t *[capacity];
		if (list->Texts != NULL) memcpy(texts, list->Texts, sizeof(wchar_t *) * list->TextCount);
		delete [] list->Texts;
		list->Texts = texts;
		list->TextCapacity = capacity;
	}
	if (row < 0 || row > list->TextCount) row = list->TextCount;
	memmove(list->Texts + row + 1, list->Texts + row, sizeof(wchar_t *) * (list->TextCount - row));
	int length = text != NULL ? (int)wcslen(text) : 0;
	list->Texts[row] = new wchar_t[length + 1];
	if (length) memcpy(list->Texts[row], text, sizeof(wchar_t) * length);
	list->Texts[row][length] = 0;
	list->TextCount++;
}

void YR_List_Delete_Text(YRListRows *list, int row)
{
	if (row < 0 || row >= list->TextCount) return;
	delete [] list->Texts[row];
	memmove(list->Texts + row, list->Texts + row + 1, sizeof(wchar_t *) * (list->TextCount - row - 1));
	list->TextCount--;
}

// Rows up to `last` get a selection flag (new ones clear).
void YR_List_Grow(YRListRows *list, int last)
{
	if (last < list->Count || last + 1 < 0) return;
	int count = last + 1;
	if (count > list->Capacity) {
		int capacity = list->Count * 2 < 10 ? 10 : list->Count * 2;
		while (capacity < count) capacity *= 2;
		int *flags = new int[capacity];
		memset(flags, 0, sizeof(int) * capacity);
		if (list->Flags != NULL) memcpy(flags, list->Flags, sizeof(int) * list->Count);
		delete [] list->Flags;
		list->Flags = flags;
		list->Capacity = capacity;
	}
	for (int row = list->Count; row < count; row++) list->Flags[row] = 0;
	list->Count = count;
}

int &YR_List_Flag(YRListRows *list, int row)
{
	if (row < 0) row = 0;
	if (row >= list->Count) row = list->Count - 1;
	return list->Flags[row];
}

int YR_List_Get_Sel(HWND window, int row)
{
	YRListRows *list = YR_List(window, false);
	if (list == NULL || list->Flags == NULL || row < 0 || row >= list->Count) return 0;
	return list->Flags[row];
}

void YR_List_Notify(HWND window, int code)
{
	SendMessage(GetParent(window), WM_COMMAND, (WPARAM)((GetWindowLong(window, GWL_ID) & 0xFFFF) | (code << 16)), (LPARAM)window);
}

// A row's wide text: the one kept here, else the port list's narrow text widened.
bool YR_List_Text(HWND window, int row, wchar_t *out, int size)
{
	out[0] = 0;
	YRListRows *list = YR_List(window, false);
	if (list != NULL && row >= 0 && row < list->TextCount && list->Texts[row] != NULL) {
		int n = 0;
		for (; list->Texts[row][n] && n < size - 1; n++) out[n] = list->Texts[row][n];
		out[n] = 0;
		return true;
	}
	char narrow[256];
	LRESULT length = YR_Original(window, LB_GETTEXTLEN, row, 0);
	if (length < 0 || length >= (LRESULT)sizeof(narrow) || YR_Original(window, LB_GETTEXT, row, (LPARAM)narrow) < 0) return false;
	int n = 0;
	for (; narrow[n] && n < size - 1; n++) out[n] = (unsigned char)narrow[n];
	out[n] = 0;
	return true;
}
}

LRESULT CALLBACK OwnerDraw::ListBoxHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WinData *data = Find_Data(window);
	if (data == NULL) {
		return 0;
	}
	switch (message) {
	case 0x4C8:
	case 0x4C9:
	case 0x4CC:
	case 0x4CD:
	case 0x4CE: {
		// A wide row: appended (0x4C8, 0x4CD) or inserted at wparam (-1 appends).
		wchar_t const *wide = (wchar_t const *)lparam;
		char narrow[512];
		WideToCharString(narrow, wide != NULL ? wide : L"", sizeof(narrow) - 1);
		bool append = message == 0x4C8 || message == 0x4CD;
		LRESULT row = YR_Original(window, append ? LB_ADDSTRING : LB_INSERTSTRING, append ? 0 : wparam, (LPARAM)narrow);
		if (row >= 0) {
			YRListRows *list = YR_List(window, true);
			if (list != NULL) YR_List_Insert_Text(list, (int)row, wide);
		}
		InvalidateRect(window, NULL, TRUE);
		return row;
	}
	case 0x4A6:
	case 0x4A8:
		return -1;
	case LB_SETSEL: {
		int row = (int)lparam;
		if (row < -1) return -1;
		YRListRows *list = YR_List(window, true);
		if (list == NULL) return -1;
		int rows = (int)YR_Original(window, LB_GETCOUNT, 0, 0);
		if (row >= rows - 1) row = rows - 1;
		YR_List_Grow(list, row);
		if (row == -1) {
			for (int i = 0; i < list->Count; i++) list->Flags[i] = (int)wparam;
		} else if (list->Count) {
			YR_List_Flag(list, row) = (int)wparam;
		}
		YR_List_Notify(window, LBN_SELCHANGE);
		InvalidateRect(window, NULL, FALSE);
		return 0;
	}
	case LB_SETCURSEL: {
		int row = (int)wparam;
		if (row >= -1 && row < (int)YR_Original(window, LB_GETCOUNT, 0, 0)) {
			YRListRows *list = YR_List(window, true);
			if (list != NULL) {
				if (list->Current != -1) SendMessage(window, LB_SETSEL, 0, list->Current);
				list->Current = row;
				if (row != -1) SendMessage(window, LB_SETSEL, 1, row);
			}
		}
		YR_List_Notify(window, LBN_SELCHANGE);
		InvalidateRect(window, NULL, FALSE);
		return 0;
	}
	case LB_GETSEL:
		return YR_List_Get_Sel(window, (int)wparam);
	case LB_GETCURSEL: {
		YRListRows *list = YR_List(window, false);
		return list != NULL ? list->Current : -1;
	}
	case LB_GETSELCOUNT: {
		YRListRows *list = YR_List(window, false);
		int selected = 0;
		for (int i = 0; list != NULL && i < list->Count; i++) if (list->Flags[i]) selected++;
		return selected;
	}
	case LB_GETSELITEMS: {
		YRListRows *list = YR_List(window, false);
		int found = 0;
		int *out = (int *)lparam;
		for (int i = 0; list != NULL && i < list->Count && found < (int)wparam; i++) if (list->Flags[i]) out[found++] = i;
		return found;
	}
	case LB_SELITEMRANGE: {
		int first = (int)(lparam & 0xFFFF);
		int last = (int)((lparam >> 16) & 0xFFFF);
		int rows = (int)YR_Original(window, LB_GETCOUNT, 0, 0);
		if (last < first) return -1;
		if (last >= rows) last = rows - 1;
		YRListRows *list = YR_List(window, true);
		if (list == NULL) return -1;
		YR_List_Grow(list, last);
		for (int i = first; i <= last; i++) if (list->Count) YR_List_Flag(list, i) = (int)wparam;
		YR_List_Notify(window, LBN_SELCHANGE);
		return 0;
	}
	case LB_DELETESTRING:
		if (YRListRows *list = YR_List(window, false)) {
			int row = (int)wparam;
			if (list->Count > row && list->Count) {
				if (row < 0) row = 0;
				memmove(list->Flags + row, list->Flags + row + 1, sizeof(int) * (list->Count - row - 1));
				list->Count--;
			}
			YR_List_Delete_Text(list, (int)wparam);
		}
		break;
	case LB_RESETCONTENT:
	case WM_NCDESTROY:
		YR_List_Free(window);
		break;
	case WM_LBUTTONDOWN: {
		int top = (int)SendMessage(window, LB_GETTOPINDEX, 0, 0);
		int height = (int)SendMessage(window, LB_GETITEMHEIGHT, 0, 0);
		int row = (int)(((unsigned long)lparam >> 16) & 0xFFFF) / (height ? height : 1) + top;
		LONG style = GetWindowLong(window, GWL_STYLE);
		SetFocus(window);
		if (style & LBS_MULTIPLESEL) {
			int selected = (int)SendMessage(window, LB_GETSEL, row, 0);
			VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000);
			SendMessage(window, LB_SETSEL, !selected, row);
		} else if (!(style & LBS_NOSEL)) {
			VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000);
			SendMessage(window, LB_SETCURSEL, row, 0);
		}
		InvalidateRect(window, NULL, FALSE);
		YR_List_Notify(window, LBN_SELCHANGE);
		return 0;
	}
	case WM_LBUTTONDBLCLK:
		PostMessage(GetParent(window), WM_COMMAND, (WPARAM)((GetWindowLong(window, GWL_ID) & 0xFFFF) | (LBN_DBLCLK << 16)), (LPARAM)window);
		return 0;
	case WM_RBUTTONDOWN:
		if (GetWindowLong(window, GWL_STYLE) & LBS_MULTIPLESEL) {
			SendMessage(window, LB_SETSEL, 0, -1);
			SendMessage(window, LB_SETCURSEL, (WPARAM)-1, 0);
			YR_List_Notify(window, LBN_SELCHANGE);
			InvalidateRect(window, NULL, FALSE);
			return 0;
		}
		break;
	}
	if (message != WM_PAINT) {
		if (message == WM_ERASEBKGND) return 0;
		LRESULT result = YR_Original(window, message, wparam, lparam);
		if (message == LB_ADDSTRING || message == LB_RESETCONTENT) InvalidateRect(window, NULL, TRUE);
		return result;
	}
	if (!data->ScrollBar.skipDraw) {
		RECT where;
		Get_Display_Rect(window, &where);
		Rect area(where.left, where.top, where.right - where.left, where.bottom - where.top);
		Surface *surface = AlternateSurface;
		OwnerDraw::CopyDimmedBackground(area, window, YR_REC(int, data, 0xC8));
		OwnerDraw::DrawBeveledBorder(*surface, area, 1, ODColorToHiColor(OwnerDraw::DefaultBorderColor));
		BitFont *font = data->Wide.font != NULL ? data->Wide.font : BitFont::Instance;
		int count = (int)YR_Original(window, LB_GETCOUNT, 0, 0);
		int top = (int)YR_Original(window, LB_GETTOPINDEX, 0, 0);
		int height = (int)YR_Original(window, LB_GETITEMHEIGHT, 0, 0);
		if (height <= 0) height = 16;
		for (int row = top; row < count; row++) {
			Rect line(area.X + 2, area.Y + 2 + (row - top) * height, area.Width - 4, height);
			if (line.Y + line.Height > area.Y + area.Height) break;
			if (YR_List_Get_Sel(window, row)) {
				surface->Fill_Rect(line, ODColorToHiColor(OwnerDraw::ListSelectionFillColor));
			}
			wchar_t text[256];
			if (YR_List_Text(window, row, text, 256) && text[0]) {
				RECT box = {line.X + 2, line.Y, line.X + line.Width - 2, line.Y + line.Height};
				OwnerDraw::DrawWideText(surface, text, (LTRBStruct *)&box, font, OwnerDraw::PrimaryTextColor, 4, 0xC, 0, 0, 0);
			}
		}
	}
	ValidateRect(window, NULL);
	return 0;
}
