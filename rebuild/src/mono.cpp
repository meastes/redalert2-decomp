// SPDX-License-Identifier: GPL-3.0-or-later
// Adopted from mischa85/RedAlert2 (src/mono.cpp, commit 32ed14c); that file carries no notice of its own.
// See rebuild/LICENSE.md and THIRD_PARTY.md.
/**
 *  RA2: the monochrome debug window (MonoClass): an 80 x 25 character page drawn with GDI.
 *  In the executable these functions lie directly before motlib.cpp's (0x0059E160-0x0059EB60).
 */

#include "always.h" // RA2: as above
#include "mono.h" // RA2: as above

#include "point.h" // RA2: as above

#include <cstdio> // RA2: as above
#include <cstring> // RA2: as above
#include <cstdarg> // RA2: as above
#include <cwchar> // RA2: as above


/// <summary>
/// Whether the mono window is on and the page it shows.
/// </summary>
bool MonoClass::Enabled = false; // RA2: 0x00A70F0A
MonoClass * MonoClass::Current; // RA2: 0x00A70F0C

static HWND MonoWindow; // RA2: 0x00A70F10, the dialog the pages are drawn on
static HFONT MonoFont; // RA2: 0x00A70F14, the Courier font of the dialog
static TEXTMETRICA MonoMetrics; // RA2: 0x00A70EA0, the font's metrics (height, external leading and maximum width are read)
static COLORREF MonoTextColor; // RA2: 0x00A70DE8
static COLORREF MonoBackColor; // RA2: 0x00A70E00


/// <summary>
/// The pixel position of a character cell.
/// </summary>
struct MonoPoint : public Point2D // RA2: as above
{
	MonoPoint(int column, int row); // RA2: as above
};


MonoPoint::MonoPoint(int column, int row) // RA2: 0x0059E160, the cell size from the font metrics with a margin of ten pixels
{
	Point2D p(MonoMetrics.tmMaxCharWidth * column + 10, (MonoMetrics.tmHeight + MonoMetrics.tmExternalLeading) * row + 10); // RA2: as above
	X = p.X; // RA2: as above
	Y = p.Y; // RA2: as above
}


MonoClass::MonoClass(int handle) // RA2: 0x0059E190, starts a blank 80 x 25 window at the top left
{
	Handle = handle; // RA2: as above
	X = 0; // RA2: as above
	Y = 0; // RA2: as above
	WindowX = 0; // RA2: as above
	WindowY = 0; // RA2: as above
	WindowWidth = 80; // RA2: as above
	WindowHeight = 25; // RA2: as above
	Clear(); // RA2: as above
}


MonoClass::~MonoClass(void) // RA2: 0x0059E220, an empty destructor
{
}


int MonoClass::Get_Handle(void) const // RA2: 0x0059E230
{
	return(Handle); // RA2: as above
}


void MonoClass::Set_Cursor(int x, int y) // RA2: 0x0059E240
{
	X = x; // RA2: as above
	Y = y; // RA2: as above
}


void MonoClass::Sub_Window(int x, int y, int w, int h) // RA2: 0x0059E260, stores the window without clipping it
{
	WindowX = x; // RA2: as above
	WindowY = y; // RA2: as above
	WindowWidth = w; // RA2: as above
	WindowHeight = h; // RA2: as above
}


void MonoClass::Clear(void) // RA2: 0x0059E290, blanks the whole 80 x 25 grid and redraws it
{
	for (int y = 0; y < LINES; y++) { // RA2: as above
		for (int x = 0; x < COLUMNS; x++) { // RA2: as above
			Characters[x][y] = ' '; // RA2: as above
			Attributes[x][y] = 0; // RA2: as above
		}
	}
	Changed = true; // RA2: as above
	Refresh(0, 0, COLUMNS, LINES); // RA2: as above
}


bool MonoClass::Is_Changed(void) const // RA2: 0x0059E2E0
{
	return(Changed); // RA2: as above
}


void MonoClass::View(void) // RA2: 0x0059E2F0, a page that was not on display is cleared first
{
	if (Enabled) { // RA2: as above
		if (Current != this) { // RA2: as above
			Current = this; // RA2: as above
			Clear(); // RA2: as above
		}
		Refresh(0, 0, COLUMNS, LINES); // RA2: as above
	}
}


void MonoClass::Set_Default_Attribute(MonoAttribute attrib) // RA2: 0x0059E370
{
	if (Enabled) { // RA2: as above
		Attribute = attrib; // RA2: as above
	}
}


void MonoClass::Fill_Attrib(int x, int y, int w, int h, MonoAttribute attrib) // RA2: 0x0059E390, window relative block
{
	if (Enabled) { // RA2: as above
		int left = WindowX + x; // RA2: as above
		int top = WindowY + y; // RA2: as above
		int right = left + w; // RA2: as above
		int bottom = top + h; // RA2: as above
		for (int row = top; row < bottom; row++) { // RA2: as above
			for (int column = left; column < right; column++) { // RA2: as above
				Attributes[column][row] = attrib; // RA2: as above
			}
		}
		Refresh(left, top, w, h); // RA2: as above
	}
}


void MonoClass::Scroll(int lines) // RA2: 0x0059E410, positive moves the window's text up, negative down
{
	if (Enabled) { // RA2: as above
		int x = WindowX; // RA2: as above
		int y = WindowY; // RA2: as above
		int width = WindowWidth; // RA2: as above
		int height = WindowHeight; // RA2: as above
		if (lines > 0) { // RA2: as above
			for (int column = x; column < x + width; column++) { // RA2: as above
				for (int row = y; row < y + height - 1; row++) { // RA2: as above
					Characters[column][row] = Characters[column][row + 1]; // RA2: as above
					Attributes[column][row] = Attributes[column][row + 1]; // RA2: as above
				}
				Characters[column][y + height - 1] = ' '; // RA2: as above
				Attributes[column][y + height - 1] = 0; // RA2: as above
			}
		} else if (lines < 0) { // RA2: as above
			for (int column = x; column < x + width; column++) { // RA2: as above
				for (int row = y + height - 1; row > y; row--) { // RA2: as above
					Characters[column][row] = Characters[column][row - 1]; // RA2: as above
					Attributes[column][row] = Attributes[column][row - 1]; // RA2: as above
				}
				Characters[column][y] = ' '; // RA2: as above
				Attributes[column][y] = 0; // RA2: as above
			}
		}
		Refresh(0, 0, COLUMNS, LINES); // RA2: as above
	}
}


void MonoClass::Pan(int cols) // RA2: 0x0059E590, positive moves the window's text left, negative right
{
	if (Enabled) { // RA2: as above
		int x = WindowX; // RA2: as above
		int width = WindowWidth; // RA2: as above
		int y = WindowY; // RA2: as above
		int height = WindowHeight; // RA2: as above
		if (cols > 0) { // RA2: as above
			for (int row = y; row < y + height; row++) { // RA2: as above
				for (int column = x; column < x + width - 1; column++) { // RA2: as above
					Characters[column][row] = Characters[column + 1][row]; // RA2: as above
					Attributes[column][row] = Attributes[column + 1][row]; // RA2: as above
				}
				Characters[x + width - 1][row] = ' '; // RA2: as above
				Attributes[x + width - 1][row] = 0; // RA2: as above
			}
		} else if (cols < 0) { // RA2: as above
			for (int row = y; row < y + height; row++) { // RA2: as above
				for (int column = x + width - 1; column > x; column--) { // RA2: as above
					Characters[column][row] = Characters[column - 1][row]; // RA2: as above
					Attributes[column][row] = Attributes[column - 1][row]; // RA2: as above
				}
				Characters[x][row] = ' '; // RA2: as above
				Attributes[x][row] = 0; // RA2: as above
			}
		}
		Refresh(0, 0, COLUMNS, LINES); // RA2: as above
	}
}


void MonoClass::Print(char const * text) // RA2: 0x0059E6D0, writes at the cursor clipped to the window width, a newline returns the cursor to column 0 one line down
{
	if (Enabled) { // RA2: as above
		int cx = X; // RA2: as above
		int start = WindowX + cx; // RA2: as above
		int row = WindowY + Y; // RA2: as above
		int limit = WindowWidth + WindowX; // RA2: as above
		Changed = false; // RA2: as above
		int end = (int)strlen(text) + start; // RA2: as above
		if (end > limit) { // RA2: as above
			end = limit; // RA2: as above
		}
		for (int col = start, i = 0; col < end; col++, i++) { // RA2: as above
			char c = text[i]; // RA2: as above
			Characters[col][row] = Map_Character(c); // RA2: as above
			Attributes[col][row] = Attribute; // RA2: as above
			if (c == '\n') { // RA2: as above
				X = 0; // RA2: as above
				Y++; // RA2: as above
			} else {
				X++; // RA2: as above
			}
		}
		Refresh(start, row, end - start, 1); // RA2: as above
	}
}


void __cdecl MonoClass::Printf(char const * text, ...) // RA2: 0x0059E7F0
{
	if (Enabled) { // RA2: as above
		char buffer[256]; // RA2: as above
		va_list args; // RA2: as above
		va_start(args, text); // RA2: as above
		vsprintf(buffer, text, args); // RA2: as above
		buffer[255] = '\0'; // RA2: as above
		Print(buffer); // RA2: as above
	}
}


void __cdecl MonoClass::Printf(wchar_t const * text, ...) // RA2: 0x0059E840, formats wide, then prints narrow through %S
{
	if (Enabled) { // RA2: as above
		wchar_t wide[256]; // RA2: as above
		char buffer[256]; // RA2: as above
		va_list args; // RA2: as above
		va_start(args, text); // RA2: as above
		vswprintf(wide, text, args); // RA2: as above
		sprintf(buffer, "%S", wide); // RA2: as above
		Print(buffer); // RA2: as above
	}
}


void MonoClass::Text_Print(char const * text, int x, int y, MonoAttribute attrib) // RA2: 0x0059E8A0
{
	if (Enabled) { // RA2: as above
		Set_Cursor(x, y); // RA2: as above
		Set_Default_Attribute(attrib); // RA2: as above
		Print(text); // RA2: as above
	}
}


void MonoClass::Text_Print(wchar_t const * text, int x, int y, MonoAttribute attrib) // RA2: 0x0059E8E0, prints a wide string through %S
{
	if (Enabled) { // RA2: as above
		char buffer[1024]; // RA2: as above
		Set_Cursor(x, y); // RA2: as above
		Set_Default_Attribute(attrib); // RA2: as above
		sprintf(buffer, "%S", text); // RA2: as above
		Print(buffer); // RA2: as above
	}
}


char MonoClass::Map_Character(char character) // RA2: 0x0059E960, line drawing characters of the IBM set become + | - =
{
	if (character == '\n') { // RA2: as above
		return(' '); // RA2: as above
	}
	if (character == -0x4d) { // RA2: as above
		return('|'); // RA2: as above
	}
	if (character == -0x3c) { // RA2: as above
		return('-'); // RA2: as above
	}
	if (character == -0x33) { // RA2: as above
		return('='); // RA2: as above
	}
	if (character == -0x26 || character == -0x3e || character == -0x41 || character == -0x3a || character == -0x3d || character == -0x2f || character == -0x3b || character == -0x28 || character == -0x4b || character == -0x40 || character == -0x3f || character == -0x27 || character == -0x2b || character == -0x4c) { // RA2: as above
		character = '+'; // RA2: as above
	}
	return(character); // RA2: as above
}


void MonoClass::Draw_Character(HDC dc, int x, int y) // RA2: 0x0059E9D0, attribute 1 is inverse video
{
	int attribute = Attributes[x][y]; // RA2: as above
	if (attribute != DrawnAttribute) { // RA2: as above
		if (attribute == 1) { // RA2: as above
			SetBkColor(dc, MonoTextColor); // RA2: as above
			SetTextColor(dc, MonoBackColor); // RA2: as above
		} else {
			SetBkColor(dc, MonoBackColor); // RA2: as above
			SetTextColor(dc, MonoTextColor); // RA2: as above
		}
		DrawnAttribute = attribute; // RA2: as above
	}
	char character = Characters[x][y]; // RA2: as above
	MonoPoint point(x, y); // RA2: as above
	TextOutA(dc, point.X, point.Y, &character, 1); // RA2: as above
}


void MonoClass::Refresh(int x, int y, int w, int h) // RA2: 0x0059EA90, redraws a block of cells when this is the page on display
{
	if (Enabled && this == Current) { // RA2: as above
		HDC dc = GetDC(MonoWindow); // RA2: as above
		SetBkMode(dc, OPAQUE); // RA2: as above
		SetBkColor(dc, MonoBackColor); // RA2: as above
		SetTextColor(dc, MonoTextColor); // RA2: as above
		if (dc != NULL) { // RA2: as above
			HGDIOBJ old_font = SelectObject(dc, MonoFont); // RA2: as above
			for (int row = y; row < y + h; row++) { // RA2: as above
				for (int column = x; column < x + w; column++) { // RA2: as above
					Draw_Character(dc, column, row); // RA2: as above
				}
			}
			SelectObject(dc, old_font); // RA2: as above
			ReleaseDC(MonoWindow, dc); // RA2: as above
		}
	}
}
