// SPDX-License-Identifier: GPL-3.0-or-later
// Adopted from mischa85/RedAlert2 (src/mono.cpp, commit 32ed14c); that file carries no notice of its own.
// Contains material derived from ra2-decomp, copyright 2026 its contributors, under GPL-3.0-or-later.
// See rebuild/LICENSE.md and THIRD_PARTY.md.
/**
 *  RA2: the monochrome debug window (MonoClass): an 80 x 25 character page drawn with GDI.
 *  In the executable the unit runs from its head initializers to motlib.cpp (0x0059D590-0x0059EB60): the window functions, then MonoPoint and MonoClass.
 */

#include "always.h"
#include "rect.h" // RA2: the head initializers: RECT_NONE, then geometry.h's twelve header statics (0x0059D590-0x0059D780)
#include "geometry.h"
#include "mono.h"
#include "audfile.h"

#include "point.h"

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cwchar>


static COLORREF MonoTextColor; // RA2: 0x00A70DE8
static COLORREF MonoBackColor; // RA2: 0x00A70E00
static MonoClass * MonoPages[MonoClass::MAX_MONO_PAGES]; // RA2: 0x00A70E40, the pages Mono_Window_Init makes
static TEXTMETRICA MonoMetrics; // RA2: 0x00A70EA0, the font's metrics (height, external leading and maximum width are read)
static HINSTANCE MonoInstance; // RA2: 0x00A70F00
static HWND MonoParent; // RA2: 0x00A70F04, the game window
static bool MonoWindowAllowed; // RA2: 0x00A70F08, windowed mode
static bool MonoWindowRequested; // RA2: 0x00A70F09, shown as soon as the pages exist

/// <summary>
/// Whether the mono window is on and the page it shows.
/// </summary>
bool MonoClass::Enabled = false; // RA2: 0x00A70F0A
MonoClass * MonoClass::Current; // RA2: 0x00A70F0C

static HWND MonoWindow; // RA2: 0x00A70F10, the dialog the pages are drawn on
static HFONT MonoFont; // RA2: 0x00A70F14, the Courier font of the dialog

void Mono_Window_Create(void); // RA2: 0x0059DB30, creates the modeless mono window when the first page is shown
BOOL CALLBACK Mono_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void Mono_Clear_All_Pages(void); // RA2: 0x0059DAD0


void Mono_Window_Init(HINSTANCE instance, HWND window, bool windowed) // RA2: 0x0059D780
{
	MonoInstance = instance;
	MonoParent = window;
	MonoWindowAllowed = windowed;
	for (int page = 0; page < MonoClass::MAX_MONO_PAGES; page++) {
		MonoPages[page] = new MonoClass(page);
	}
	MonoClass::Current = MonoPages[0];
	Mono_Clear_All_Pages();
	if (MonoWindowRequested) {
		MonoClass::Enable();
	}
}


void Mono_Window_Shutdown(void) // RA2: 0x0059D900
{
	for (int page = 0; page < MonoClass::MAX_MONO_PAGES; page++) {
		if (MonoPages[page] != NULL) {
			delete MonoPages[page];
			MonoPages[page] = NULL;
		}
	}
	if (MonoWindow != NULL) {
		DeleteObject(MonoFont);
		Remove_Audio_File_Window(MonoWindow);
		DestroyWindow(MonoWindow);
		MonoFont = NULL;
		MonoWindow = NULL;
	}
}


void Mono_Window_Show_Requested(void) // RA2: 0x0059D970
{
	if (MonoParent == NULL) {
		MonoWindowRequested = true;
		return;
	}
	MonoClass::Enable();
}


void MonoClass::Enable(void) // RA2: 0x0059D9C0
{
	if (MonoWindowAllowed) {
		if (MonoWindow == NULL) {
			Mono_Window_Create();
		}
		Enabled = true;
		ShowWindow(MonoWindow, SW_SHOWNORMAL);
		SetFocus(MonoParent);
	}
}


void MonoClass::Disable(void) // RA2: 0x0059DA00
{
	MonoWindowRequested = false;
	if (MonoWindow != NULL) {
		Enabled = false;
		ShowWindow(MonoWindow, SW_HIDE);
	}
}


int Mono_Window_Is_Active(void) // RA2: 0x0059DA20
{
	if (MonoClass::Is_Enabled() || MonoWindowRequested) {
		return(1);
	}
	return(0);
}


MonoClass * Mono_Viewed_Page(void) // RA2: 0x0059DA40
{
	return(MonoClass::Current);
}


MonoClass * MonoClass::Get_Page(int index) // RA2: 0x0059DA50
{
	return(MonoPages[index]);
}


void Mono_View_Page(int index) // RA2: 0x0059DA60
{
	MonoClass * page = MonoClass::Get_Page(index);
	MonoClass::Current = page;
	page->Clear();
	MonoClass::Current->Refresh(0, 0, MonoClass::COLUMNS, MonoClass::LINES);
}


void Mono_Clear_All_Pages(void) // RA2: 0x0059DAD0
{
	for (int page = 0; page < MonoClass::MAX_MONO_PAGES; page++) {
		MonoPages[page]->Clear();
	}
}


void Mono_Window_Create(void)
{
	MonoWindow = CreateDialogParamA(MonoInstance, MAKEINTRESOURCE(248), MonoParent, Mono_Dialog_Proc, 0);
	Add_Audio_File_Window(MonoWindow);
	HDC dc = GetDC(MonoWindow);

	LOGFONT lf;
	lf.lfHeight = -MulDiv(8, GetDeviceCaps(dc, LOGPIXELSY), 72);
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 0;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = 0;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	strcpy(lf.lfFaceName, "Courier");
	MonoFont = CreateFontIndirect(&lf);
	HGDIOBJ old = SelectObject(dc, MonoFont);
	GetTextMetrics(dc, &MonoMetrics);
	SelectObject(dc, old);
	MonoBackColor = GetSysColor(COLOR_BTNFACE);
	MonoTextColor = GetSysColor(COLOR_WINDOWTEXT);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = MonoMetrics.tmMaxCharWidth * (MonoClass::COLUMNS - 1) + 20;
	rect.bottom = (MonoMetrics.tmHeight + MonoMetrics.tmExternalLeading) * MonoClass::LINES + 20;
	AdjustWindowRect(&rect, WS_CAPTION, FALSE);
	RECT parent;
	GetWindowRect(MonoParent, &parent);
	MoveWindow(MonoWindow, parent.right + 5, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);
	ShowWindow(MonoWindow, SW_HIDE);
	SetFocus(MonoParent);
	ReleaseDC(MonoWindow, dc);
}


/// <summary>
/// RA2: the dialog procedure of the mono window (0x0059DCE0). It repaints the page on display,
/// hides the window on a command, and takes the keys of the sound test: 1 to 4 pick a speech
/// priority, Q W E O speak the next voice, L and T play test sounds, [ and ] start and stop
/// streams, N S stop sounds, A I M and the angle brackets change the page.
/// </summary>
#include "audplay.h"
#include "auddsnd.h"
#include "audioctl.h"
#include "audio.h"
#include "voc.h"
#include "vox.h"
#include "vector.h"

void Audio_Stop_All(void);

static AudioStreamer * MonoStreams[5]; // RA2: 0x00A70DEC, the test streams the [ key starts
static AudioController MonoTestController; // RA2: 0x00A70E70
static int MonoAudioToggle; // RA2: 0x00A70EEC, flipped by the N key
static int MonoVoiceIndex; // RA2: 0x00A70EF0, the next voice the speech keys play
static AudioController * MonoTestHandle; // RA2: 0x00A70EF4
static int MonoStreamCount; // RA2: 0x00A70EF8
static int MonoVoicePriority; // RA2: 0x00A70EFC, set by the keys 1 to 4

// Queues the next test voice, wrapping back to the first after the seventeenth or the last loaded one.
static inline void Mono_Test_Speech(int type, int priority)
{
	if (MonoVoiceIndex > 16 || MonoVoiceIndex >= Voxs.Count()) {
		MonoVoiceIndex = 0;
	}
	Queue_Speech(MonoVoiceIndex, type, priority);
	MonoVoiceIndex++;
}

BOOL CALLBACK Mono_Dialog_Proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_PAINT) {
		InvalidateRect(hwnd, NULL, TRUE);
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		MonoClass * page = Mono_Viewed_Page();
		if (page != NULL) {
			page->View();
		}
		EndPaint(hwnd, &ps);
		return TRUE;
	}
	if (msg == WM_CHAR) {
		if (MonoTestHandle == NULL) {
			AudioController * controller = &MonoTestController;
			MonoTestHandle = controller;
			controller->Init();
		}
		switch (wparam) {
			case '1':
			case '2':
			case '3':
			case '4':
				MonoVoicePriority = wparam - '1';
				return FALSE;
			case 'Q':
			case 'q':
				Mono_Test_Speech(VOXSPEECH_QUEUE, MonoVoicePriority);
				return FALSE;
			case 'W':
			case 'w':
				Mono_Test_Speech(VOXSPEECH_STANDARD, MonoVoicePriority);
				return FALSE;
			case 'E':
			case 'e':
				Mono_Test_Speech(VOXSPEECH_INTERRUPT, MonoVoicePriority);
				return FALSE;
			case 'O':
			case 'o':
				Mono_Test_Speech(-1, -1);
				return FALSE;
			case 'L':
			case 'l':
				if (AudioControllerGetEvent(MonoTestHandle) != NULL) {
					MonoTestHandle->End();
					return FALSE;
				}
				VocClass::PlayGlobal(VocClass::From_Name("TestEnvelopeFShift"), 1.0f, 0x2000, MonoTestHandle);
				return FALSE;
			case 'T':
			case 't':
				VocClass::PlayGlobal(VocClass::From_Name("TestEffect"), 1.0f, 0x2000, NULL);
				return FALSE;
			case '[':
				if (MonoStreamCount < 5) {
					AudioStreamer * stream = AudioStreamerCreate(AudioGetDevice(), 5000);
					MonoStreams[MonoStreamCount] = stream;
					if (stream != NULL) {
						if (AudioStreamerPlayWAV(stream, "intro.wav", 1) != 0) {
							MonoStreamCount++;
							return FALSE;
						}
						AudioStreamerDestroy(MonoStreams[MonoStreamCount]);
					}
				}
				return FALSE;
			case ']':
				if (MonoStreamCount > 0) {
					AudioStreamer * last = MonoStreams[MonoStreamCount - 1];
					MonoStreamCount--;
					AudioStreamerStop(last);
					AudioStreamerDestroy(MonoStreams[MonoStreamCount]);
				}
				return FALSE;
			case 'N':
			case 'n':
				MonoAudioToggle = (MonoAudioToggle == 0);
				return FALSE;
			case 'S':
			case 's':
				Audio_Stop_All();
				return FALSE;
			case 'A':
			case 'a':
				Mono_View_Page(5);
				return FALSE;
			case 'I':
			case 'i':
				Mono_View_Page(7);
				return FALSE;
			case 'M':
			case 'm':
				Mono_View_Page(6);
				return FALSE;
			case ',':
			case '<': {
				int next = Mono_Viewed_Page()->Get_Handle() + 1;
				if (next == MonoClass::MAX_MONO_PAGES) {
					next = 0;
				}
				MonoClass::Current = MonoPages[next];
				MonoClass::Current->Clear();
				MonoClass::Current->Refresh(0, 0, 80, 25);
				return FALSE;
			}
			case '.':
			case '>': {
				int prev = Mono_Viewed_Page()->Get_Handle() - 1;
				if (prev < 0) {
					prev = MonoClass::MAX_MONO_PAGES - 1;
				}
				MonoClass::Current = MonoPages[prev];
				MonoClass::Current->Clear();
				MonoClass::Current->Refresh(0, 0, 80, 25);
				return FALSE;
			}
			default:
				return FALSE;
		}
	}
	if (msg == WM_COMMAND) {
		MonoClass::Disable();
		return TRUE;
	}
	return FALSE;
}


/// <summary>
/// The pixel position of a character cell.
/// </summary>
struct MonoPoint : public Point2D
{
		MonoPoint(int column, int row);
};


MonoPoint::MonoPoint(int column, int row) // RA2: 0x0059E160, the cell size from the font metrics with a margin of ten pixels
{
	Point2D p(MonoMetrics.tmMaxCharWidth * column + 10, (MonoMetrics.tmHeight + MonoMetrics.tmExternalLeading) * row + 10);
	X = p.X;
	Y = p.Y;
}


MonoClass::MonoClass(int handle) // RA2: 0x0059E190, starts a blank 80 x 25 window at the top left
{
	Handle = handle;
	X = 0;
	Y = 0;
	WindowX = 0;
	WindowY = 0;
	WindowWidth = 80;
	WindowHeight = 25;
	Clear();
}


MonoClass::~MonoClass(void) // RA2: 0x0059E220, an empty destructor
{
}


int MonoClass::Get_Handle(void) const // RA2: 0x0059E230
{
	return(Handle);
}


void MonoClass::Set_Cursor(int x, int y) // RA2: 0x0059E240
{
	X = x;
	Y = y;
}


void MonoClass::Sub_Window(int x, int y, int w, int h) // RA2: 0x0059E260, stores the window without clipping it
{
	WindowX = x;
	WindowY = y;
	WindowWidth = w;
	WindowHeight = h;
}


void MonoClass::Clear(void) // RA2: 0x0059E290, blanks the whole 80 x 25 grid and redraws it
{
	for (int y = 0; y < LINES; y++) {
		for (int x = 0; x < COLUMNS; x++) {
			Characters[x][y] = ' ';
			Attributes[x][y] = 0;
		}
	}
	Changed = true;
	Refresh(0, 0, COLUMNS, LINES);
}


bool MonoClass::Is_Changed(void) const // RA2: 0x0059E2E0
{
	return(Changed);
}


void MonoClass::View(void) // RA2: 0x0059E2F0, a page that was not on display is cleared first
{
	if (Enabled) {
		if (Current != this) {
			Current = this;
			Clear();
		}
		Refresh(0, 0, COLUMNS, LINES);
	}
}


void MonoClass::Set_Default_Attribute(MonoAttribute attrib) // RA2: 0x0059E370
{
	if (Enabled) {
		Attribute = attrib;
	}
}


void MonoClass::Fill_Attrib(int x, int y, int w, int h, MonoAttribute attrib) // RA2: 0x0059E390, window relative block
{
	if (Enabled) {
		int left = WindowX + x;
		int top = WindowY + y;
		int right = left + w;
		int bottom = top + h;
		for (int row = top; row < bottom; row++) {
			for (int column = left; column < right; column++) {
				Attributes[column][row] = attrib;
			}
		}
		Refresh(left, top, w, h);
	}
}


void MonoClass::Scroll(int lines) // RA2: 0x0059E410, positive moves the window's text up, negative down
{
	if (Enabled) {
		int x = WindowX;
		int y = WindowY;
		int width = WindowWidth;
		int height = WindowHeight;
		if (lines > 0) {
			for (int column = x; column < x + width; column++) {
				for (int row = y; row < y + height - 1; row++) {
					Characters[column][row] = Characters[column][row + 1];
					Attributes[column][row] = Attributes[column][row + 1];
				}
				Characters[column][y + height - 1] = ' ';
				Attributes[column][y + height - 1] = 0;
			}
		} else if (lines < 0) {
			for (int column = x; column < x + width; column++) {
				for (int row = y + height - 1; row > y; row--) {
					Characters[column][row] = Characters[column][row - 1];
					Attributes[column][row] = Attributes[column][row - 1];
				}
				Characters[column][y] = ' ';
				Attributes[column][y] = 0;
			}
		}
		Refresh(0, 0, COLUMNS, LINES);
	}
}


void MonoClass::Pan(int cols) // RA2: 0x0059E590, positive moves the window's text left, negative right
{
	if (Enabled) {
		int x = WindowX;
		int width = WindowWidth;
		int y = WindowY;
		int height = WindowHeight;
		if (cols > 0) {
			for (int row = y; row < y + height; row++) {
				for (int column = x; column < x + width - 1; column++) {
					Characters[column][row] = Characters[column + 1][row];
					Attributes[column][row] = Attributes[column + 1][row];
				}
				Characters[x + width - 1][row] = ' ';
				Attributes[x + width - 1][row] = 0;
			}
		} else if (cols < 0) {
			for (int row = y; row < y + height; row++) {
				for (int column = x + width - 1; column > x; column--) {
					Characters[column][row] = Characters[column - 1][row];
					Attributes[column][row] = Attributes[column - 1][row];
				}
				Characters[x][row] = ' ';
				Attributes[x][row] = 0;
			}
		}
		Refresh(0, 0, COLUMNS, LINES);
	}
}


void MonoClass::Print(char const * text) // RA2: 0x0059E6D0, writes at the cursor clipped to the window width, a newline returns the cursor to column 0 one line down
{
	if (Enabled) {
		int cx = X;
		int start = WindowX + cx;
		int row = WindowY + Y;
		int limit = WindowWidth + WindowX;
		Changed = false;
		int end = (int)strlen(text) + start;
		if (end > limit) {
			end = limit;
		}
		for (int col = start, i = 0; col < end; col++, i++) {
			char c = text[i];
			Characters[col][row] = Map_Character(c);
			Attributes[col][row] = Attribute;
			if (c == '\n') {
				X = 0;
				Y++;
			} else {
				X++;
			}
		}
		Refresh(start, row, end - start, 1);
	}
}


void __cdecl MonoClass::Printf(char const * text, ...) // RA2: 0x0059E7F0
{
	if (Enabled) {
		char buffer[256];
		va_list args;
		va_start(args, text);
		vsprintf(buffer, text, args);
		buffer[255] = '\0';
		Print(buffer);
	}
}


void __cdecl MonoClass::Printf(wchar_t const * text, ...) // RA2: 0x0059E840, formats wide, then prints narrow through %S
{
	if (Enabled) {
		wchar_t wide[256];
		char buffer[256];
		va_list args;
		va_start(args, text);
		vswprintf(wide, text, args);
		sprintf(buffer, "%S", wide);
		Print(buffer);
	}
}


void MonoClass::Text_Print(char const * text, int x, int y, MonoAttribute attrib) // RA2: 0x0059E8A0
{
	if (Enabled) {
		Set_Cursor(x, y);
		Set_Default_Attribute(attrib);
		Print(text);
	}
}


void MonoClass::Text_Print(wchar_t const * text, int x, int y, MonoAttribute attrib) // RA2: 0x0059E8E0, prints a wide string through %S
{
	if (Enabled) {
		char buffer[1024];
		Set_Cursor(x, y);
		Set_Default_Attribute(attrib);
		sprintf(buffer, "%S", text);
		Print(buffer);
	}
}


char MonoClass::Map_Character(char character) // RA2: 0x0059E960, line drawing characters of the IBM set become + | - =
{
	if (character == '\n') {
		return(' ');
	}
	if (character == -0x4d) {
		return('|');
	}
	if (character == -0x3c) {
		return('-');
	}
	if (character == -0x33) {
		return('=');
	}
	if (character == -0x26 || character == -0x3e || character == -0x41 || character == -0x3a || character == -0x3d || character == -0x2f || character == -0x3b || character == -0x28 || character == -0x4b || character == -0x40 || character == -0x3f || character == -0x27 || character == -0x2b || character == -0x4c) {
		character = '+';
	}
	return(character);
}


void MonoClass::Draw_Character(HDC dc, int x, int y) // RA2: 0x0059E9D0, attribute 1 is inverse video
{
	int attribute = Attributes[x][y];
	if (attribute != DrawnAttribute) {
		if (attribute == 1) {
			SetBkColor(dc, MonoTextColor);
			SetTextColor(dc, MonoBackColor);
		} else {
			SetBkColor(dc, MonoBackColor);
			SetTextColor(dc, MonoTextColor);
		}
		DrawnAttribute = attribute;
	}
	char character = Characters[x][y];
	MonoPoint point(x, y);
	TextOutA(dc, point.X, point.Y, &character, 1);
}


void MonoClass::Refresh(int x, int y, int w, int h) // RA2: 0x0059EA90, redraws a block of cells when this is the page on display
{
	if (Enabled && this == Current) {
		HDC dc = GetDC(MonoWindow);
		SetBkMode(dc, OPAQUE);
		SetBkColor(dc, MonoBackColor);
		SetTextColor(dc, MonoTextColor);
		if (dc != NULL) {
			HGDIOBJ old_font = SelectObject(dc, MonoFont);
			for (int row = y; row < y + h; row++) {
				for (int column = x; column < x + w; column++) {
					Draw_Character(dc, column, row);
				}
			}
			SelectObject(dc, old_font);
			ReleaseDC(MonoWindow, dc);
		}
	}
}
