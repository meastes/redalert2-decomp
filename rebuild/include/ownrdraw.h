/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "arraylist.h"
#include "keyboard.h"
#include "surface.h"
#include "win.h"
#include "winfix.h"
#include "widewstring.h"
#include "wstring.h"


struct TextNode;
class BitFont;
class WideWstring;

namespace OwnerDraw {

	struct CellData {
		CellData(void);
		CellData(CellData const & that) : type(that.type), string(that.string), hint(that.hint), color(that.color), surf(that.surf), pingtime(that.pingtime)  {}
		CellData const & operator=(CellData & that)
		{
			type = that.type;
			string = that.string;
			hint = that.hint;
			color = that.color;
			surf = that.surf;
			pingtime = that.pingtime;
			return(*this);
		}

		enum DataType {
			INVALID,
			TEXT,
			SURFACE,
			PING,
			PRIMARY,
		};

		/*
		 * This specifies what kind of content the cell holds, and hence how it is drawn.
		 * A cell constructs as INVALID, and an INVALID cell contributes nothing to the row.
		 */
		DataType type;

		/*
		 * This is the text drawn in a TEXT cell. A PRIMARY cell ignores it and takes its
		 * text from the list box item itself instead.
		 */
		WideWstring string; // RA2: wide

		/*
		 * This is the tooltip text for the cell, shown when the mouse rests over this
		 * column of the row. If empty, then the cell has no tooltip.
		 */
		WideWstring hint; // RA2: wide

		/*
		 * This is the color the cell's text is drawn in. If -1, then the default list text
		 * color is used instead.
		 */
		int color;

		/*
		 * Pointer to the image drawn in a SURFACE cell, centered vertically in the row. The
		 * cell does not own the surface.
		 */
		Surface * surf;

		/*
		 * This is the round trip time in milliseconds, drawn as a bar in a PING cell. The
		 * bar fills in proportion to one second, and turns yellow at 300 and red at 500.
		 */
		int pingtime;
	};

	struct ColumnData {
		ColumnData(void) : xPos(0), width(0), cells() {}
		ColumnData(ColumnData & that) : xPos(that.xPos), width(that.width), cells(that.cells) {}

		/*
		 * This is the offset of the column from the left edge of a row, expressed in pixels.
		 */
		int xPos;

		/*
		 * This is the width available to the column, expressed in pixels. Text wider than
		 * this is truncated with an ellipsis. If zero, then the width is unlimited.
		 */
		int width;

		/*
		 * These are the column's cells, one for each row of the list box and indexed by row
		 * number. A row with no cell here draws nothing in this column.
		 */
		ArrayList<CellData> cells;
	};

	struct Tooltip {

		/*
		 * This is the area of the screen the tooltip covers.
		 */
		Rect bounds;

		/*
		 * Pointer to a saved copy of the screen underneath the tooltip. Hiding the tooltip
		 * blits this back, so the dialog beneath never has to repaint itself.
		 */
		Surface * background;

		/*
		 * This is the text displayed in the tooltip. If the owning control supplies none,
		 * then the placeholder "Tool Tip" is used.
		 */
		wchar_t text[128]; // RA2: wide

		/*
		 * If a tooltip currently exists, then this flag will be true. It stays true while
		 * the tooltip is hidden, and is only cleared when the tooltip is taken down.
		 */
		int isActive;

		/*
		 * If the tooltip is active but has been erased from the screen, then this flag will
		 * be true. A hidden tooltip can be put back without saving the screen again.
		 */
		int isHidden;

		/*
		 * This is the control the tooltip belongs to. The tooltip is taken down when that
		 * control is destroyed, hidden, or loses the keyboard focus.
		 */
		HWND window;

		Tooltip(void);
	};

	/*
	 * Key of the dictionary CtrlProc keeps of the messages it is already in the
	 * middle of handling, so that a control cannot re-enter its own handler.
	 */
	struct CtrlMsgData {
		/*
		 * These two together identify one message in flight -- the message code and the
		 * control it was sent to.
		 */
		UINT message;
		HWND window;

		bool operator ==(CtrlMsgData const & that) const;
	};

	/*
	 * Measurements of one of the remap fonts, cached by ODGetFontMetrics.
	 */
	struct FontMetrics {
		int charWidths[256];	/// inked width of each character, indexed by character code
		int glyphWidth;			/// width of the inked part of a glyph cell
		int glyphHeight;		/// height of the inked part of a glyph cell
		int topMargin;			/// blank rows above each row of glyphs
		int leftMargin;			/// blank columns before each glyph
	};

	/*
	**	RA2's control record (0x200 bytes): one per subclassed window, kept in the control table.
	*/
	struct WinData {
		WinData(void); // RA2: 0x6002F0
		~WinData(void); // RA2: frees the text, the node list and the wide text
		void Copy_List(TextNode const * source); // RA2: 0x600460
		WinData & operator = (WinData const & other); // RA2: 0x6005C0, keeps WideText
		WinData(WinData const & other); // RA2: 0x6006E0, without the self test
		void Set_Text(char const * text); // RA2: 0x600560, a wide copy
		void Set_Wide_Text(wchar_t const * text); // RA2: 0x600510

		int SubclassParam;				// RA2: +0x00 the lparam Subclass_Control was given
		int ScrollWidth;				// RA2: +0x04 a combo drop list's scroll bar width
		HWND ScrollOwner;				// RA2: +0x08 the window the drop list's scroll bar reports to
		HWND ScrollBar;					// RA2: +0x0C the drop list's scroll bar (1 while it is made)
		Surface * Back;					// RA2: +0x10 a BSurface of the background
		Surface * Image;				// RA2: +0x14 set by Set_Control_Image
		union {
			char Reserved_18[0x20 - 0x18];
			Surface * AlternateImage;	// RA2: +0x18 a button's second image
		};
		int SkipCheckPaint;				// RA2: +0x20 a check box skips painting when set
		char Reserved_24[0x28 - 0x24];
		wchar_t * Text;					// RA2: +0x28 owned wide text
		int Reserved_2C;
		char Reserved_30[0x34 - 0x30];
		TextNode * Nodes;				// RA2: +0x34 owned node list
		int Reserved_38;
		WideWstring * WideText;			// RA2: +0x3C made by the constructor
		int Reserved_40;
		int EditScroll;					// RA2: +0x44 an edit control's scroll position
		char Reserved_48[0x54 - 0x48];
		int UserDataWord;				// RA2: +0x54 the second word of the GWL_USERDATA block
		char Reserved_58[0x5C - 0x58];
		int Reserved_5C;
		int Reserved_60;
		BitFont * Font;					// RA2: +0x64 the game font by default
		int SkipTests;					// RA2: +0x68 nonzero skips the owner-draw tests
		int Kind;						// RA2: +0x6C the dialog kind (Set_Dialog_Kind)
		int PictureMode;				// RA2: +0x70 1 animated by rows, 2 image, 3 unused, 4 animation
		int Drawer;						// RA2: +0x74 the background drawer (Control_Drawer)
		char Animation[0x90 - 0x78];	// RA2: +0x78 animation state (Init_Control_Animation)
		int TypingSound;				// RA2: +0x90 -1 by default (Control_Typing_Sound)
		char Reserved_94[0xB0 - 0x94];
		int ButtonKind;					// RA2: +0xB0 1 campaign, 2 network, 3 cancel (Assign_Button_Kind)
		char Reserved_B4[0xBC - 0xB4];
		bool IsDisabled;				// RA2: +0xBC (Set_Control_Disabled)
		bool IsHidden;					// RA2: +0xBD kept while the dialog is hidden
		bool IsNotifyPending;			// RA2: +0xBE cleared after notifying control 0x71A
		char Reserved_BF;
		int Result;						// RA2: +0xC0 what the button returns (Set_Control_Result)
		bool IsFlashing;				// RA2: +0xC4 an owner-drawn button flashes (message 0x4DC, a 1 s timer)
		bool IsFlashLit;				// RA2: +0xC5 its lit phase
		char Reserved_C6[0xC8 - 0xC6];
		int TextStyle;					// RA2: +0xC8 0xC0 for text controls of the listed dialogs
		bool IsItemFilled;				// RA2: +0xCC a drop list fills its coloured rows
		bool IsAlternateColors;			// RA2: +0xCD a drop list uses the alternate colours
		char Reserved_CE[0xD0 - 0xCE];
		int ComboStyle;					// RA2: +0xD0 combo message 0x4DE's lparam
		bool IsWaiting;					// RA2: +0xD4 a WOL wait is showing
		bool IsLobbyArt;				// RA2: +0xD5 lobby dialog art
		bool IsLobbyFrame;				// RA2: +0xD6 lobby dialog frame
		bool IsChat;					// RA2: +0xD7 chat dialogs 0x103 / 0xBC7
		bool IsSetup;					// RA2: +0xD8 setup dialogs 0x108 / 0xBC6
		bool IsCheckAltArt;				// RA2: +0xD9 a check box's alternate art
		bool IsCheckAltSelected;		// RA2: +0xDA which alternate art
		char Reserved_DB;
		int SoundSlot;					// RA2: +0xDC (Control_Sound_Slot)
		int Shape640;					// RA2: +0xE0 the 640-wide background shape
		int Shape;						// RA2: +0xE4 the background shape
		int ItemState;					// RA2: +0xE8 the last WM_DRAWITEM itemState; a progress bar's minimum
		int RangeMax;					// RA2: +0xEC a progress bar's maximum
		int Position;					// RA2: +0xF0 a progress bar's position
		int IsEditFocus;				// RA2: +0xF4 an edit control takes the focus
		int HadTabStop;					// RA2: +0xF8 an edit control had WS_TABSTOP
		char Reserved_FC[0x110 - 0xFC];
		int ItemColors[50];				// RA2: +0x110 a combo box's item colours, negative for none
		char Reserved_1D8[0x200 - 0x1D8];
	};


	void Initialize(void);
	bool Subclass_Dialog(HWND window, LPARAM lParam);

	int Capture_Mouse(void);
	int Release_Mouse(void);

	HWND Begin_Dialog(int id, DLGPROC proc, LPARAM param = 0); // RA2: param
	void Display_Dialog(HWND window);
	void End_Dialog(HWND window);

	int Default_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	bool Dialog_Message_Handler(void);
	void Set_Control_Result(HWND control, int value); // RA2: what an owner-draw button returns (0x5E2040)

	bool Start_Tooltip(Rect const & rect, wchar_t const * text, HWND window); // RA2: wide text
	bool Show_Tooltip(bool save_background);
	bool Hide_Tooltip(void);
	bool End_Tooltip(void);

	HWND Custom_Message_Box(wchar_t const * btn1txt, wchar_t const * btn2txt = NULL, bool * cancelled = NULL); // RA2: wide text
	void Set_Custom_Message_Box_Text(HWND window, wchar_t const * text); // RA2: wide text
	void __fastcall Get_Keyboard_Key_String(unsigned short hotkey, wchar_t * text); // RA2: 0x5FC600; the key name of a hotkey value
};


#define OD_TEXT_ALIGN_MIN 1
#define OD_TEXT_ALIGN_CENTER 2
#define OD_TEXT_ALIGN_MAX 3

int OD_Draw_Text(COLORREF color, HFONT font, Rect const & rect, const char * text, int len, int x_alignment, int y_alignment, Surface * surface);
class BitFont;
int OD_Draw_Text(unsigned color, BitFont * font, Rect const & rect, wchar_t const * text, int len, int x_alignment, int y_alignment, Surface * surface, int unused); // RA2: wide text with a BitFont (0x5FE1B0)

void On_WM_NCDESTROY(HWND window);
void Slide_Pending_Dialog(HWND window); // RA2: when the dialog's record has its notify flag set, tells control 0x71A (0x4E2), slides the dialog and clears the flag, at 0x005E5F20

extern COLORREF ODColorText;
extern COLORREF ODColorTextDim;
extern COLORREF ODColorDisabled;
extern COLORREF ODColorFrame;
extern COLORREF ODListBoxColor;
extern COLORREF ODColorUnused1;


/// Flags for ODDrawCharRemap.
#define OD_DRAW_CHAR_FLAG_HORIZONTAL_CENTER 1
#define OD_DRAW_CHAR_ALIGN_FLAG_RIGHT 2
#define OD_DRAW_CHAR_FLAG_VERTICAL_CENTER 4


namespace OwnerDraw { extern unsigned short Red_Green_Masks[]; } // RA2: 0xA763DC, the red and green channel masks of the 16-bit pixel format (two entries; ownrdraw.cpp defines them)
namespace OwnerDraw { extern unsigned short Blue_Mask; } // RA2: 0xA763E0, the blue channel mask

namespace OwnerDraw { // RA2: the helper belongs to OwnerDraw (ownrdraw.cpp, egos.cpp and srfcache.cpp expand it)

/*
**	RA2: blends a colour into a pixel by alpha (0-255) per channel through the colour masks.
*/
inline unsigned short __cdecl Blend_Color(unsigned short pixel, unsigned short color, unsigned char alpha) // RA2: as above
{
	static unsigned int color_alpha; // RA2: as above
	static unsigned int pixel_alpha; // RA2: as above
	color_alpha = alpha; // RA2: as above
	pixel_alpha = 255 - alpha; // RA2: as above
	unsigned short red = (((pixel & Red_Green_Masks[0]) * pixel_alpha) + ((color & Red_Green_Masks[0]) * color_alpha)) >> 8 & Red_Green_Masks[0]; // RA2: as above
	unsigned short green = (((pixel & Red_Green_Masks[1]) * pixel_alpha) + ((color & Red_Green_Masks[1]) * color_alpha)) >> 8 & Red_Green_Masks[1]; // RA2: as above
	unsigned short blue = (((pixel & Blue_Mask) * pixel_alpha) + ((color & Blue_Mask) * color_alpha)) >> 8; // RA2: as above
	return((unsigned short)(red | green | blue)); // RA2: as above
}

} // RA2: as above


/*
 * Owner-draw control messages, handled by CtrlProc and the per-control
 * *CtrlProc functions in ownrdraw.cpp. Each is WM_USER + offset. The comment
 * above each says what it does, which control(s) handle it, and its
 * wParam / lParam / return contract.
 */

/*
 * Initialize a freshly subclassed control's WinData. Per-control: listbox sets item height/font,
 * combobox sets item height and clears item colors, static captures its caption, progress bar sets
 * max=100, edit strips WS_TABSTOP and deflects focus.
 * Used by: all controls.  Input: none.  Output: none.
 */
#define OD_SUBCLASSED				(WM_USER + 151)

/*
 * Set a per-element color. listbox = per-row background (wParam = row); combobox = per-item text
 * color (wParam = item 0..50); static = text color (wParam ignored, lParam == -1 resets to default).
 * Used by: listbox, combobox, static.  Input: wParam = element index, lParam = COLORREF.  Output: none.
 */
#define OD_SETCOLOR					(WM_USER + 152)

/*
 * Enable or disable hover tooltips for the control.
 * Used by: all controls.  Input: lParam = BOOL.  Output: previous enabled state.
 */
#define OD_TOOLTIPS					(WM_USER + 154)

/*
 * Ask the parent dialog for a control's tooltip text; the dialog copies it into the lParam buffer.
 * Used by: sent by the framework to the parent dialog.  Input: wParam = control ID, lParam = char[].  Output: none.
 */
#define OD_GETTIPTEXT				(WM_USER + 155)

/*
 * Set the control's primary user image (WinData::Image).
 * Used by: all controls.  Input: lParam = Surface*.  Output: previous Image.
 */
#define OD_SETIMAGE					(WM_USER + 156)

/*
 * Suppress painting for the control and its attached child window.
 * Used by: all controls.  Input: lParam = BOOL.  Output: previous state.
 */
#define OD_DISABLEPAINT				(WM_USER + 157)

/*
**	RA2: adds a wide string to an owner-drawn list box, returning the new row.
*/
#define OD_ADDSTRINGW				(WM_USER + 205)

/*
 * Restore the font / bk-mode / bk-color / text-color into the HDC that were saved by OD_SAVEDC.
 * Used by: all controls.  Input: lParam = HDC.  Output: 1.
 */
#define OD_RESTOREDC				(WM_USER + 158)

/*
 * Save the HDC's current font / bk-mode / bk-color / text-color into WinData for OD_RESTOREDC.
 * Used by: all controls.  Input: lParam = HDC.  Output: 1.
 */
#define OD_SAVEDC					(WM_USER + 159)

/*
 * Query whether the control has an attached child window (scrollbar / dropdown).
 * Used by: all controls.  Input: none.  Output: BOOL.
 */
#define OD_HASATTACHED				(WM_USER + 160)

/*
 * Non-painting refresh tick: sent in place of WM_PAINT while painting is disabled, so the control can
 * update grip/scroll state without drawing.
 * Used by: scrollbar, listbox (sent by CtrlProc).  Input: forwarded wParam/lParam.  Output: none.
 */
#define OD_REFRESHNOPAINT			(WM_USER + 165)

/*
 * Add a column to the multi-column listbox.
 * Used by: listbox.  Input: wParam = column width, lParam = column x-position (also the id).  Output: the x-position (existing one if a column is already there).
 */
#define OD_ADDCOLUMN				(WM_USER + 166)

/*
 * Remove the column whose x-position matches lParam.
 * Used by: listbox.  Input: lParam = column x-position/id.  Output: the id, or -1 if not found.
 */
#define OD_REMOVECOLUMN				(WM_USER + 167)

/*
 * Set the contents of one listbox cell.
 * Used by: listbox.  Input: wParam = MAKEWPARAM(columnId, row), lParam = CellData*.  Output: column id, or -1 on failure.
 */
#define OD_SETCELL					(WM_USER + 168)

/*
 * Push a window onto the modal z-order stack and pin it above its siblings.
 * Used by: all controls (CtrlProc).  Input: wParam = target HWND (0 = self), lParam = BOOL (1 = add and raise, 0 = remove).  Output: previous top window.
 */
#define OD_SETTOP					(WM_USER + 169)

/*
 * Set the control's alternate image (WinData::AltImage, e.g. the pressed/hover button surface).
 * Used by: all controls.  Input: lParam = Surface*.  Output: previous AltImage.
 */
#define OD_SETALTIMAGE				(WM_USER + 170)

/*
 * Set the trackbar step (the increment applied per click).
 * Used by: trackbar.  Input: lParam = INT step.  Output: none.
 */
#define OD_SETTRACKSTEP				(WM_USER + 171)

/*
 * Show or hide the numeric value drawn beside the trackbar.
 * Used by: trackbar.  Input: lParam = BOOL.  Output: none.
 */
#define OD_TRACKNUMBERS				(WM_USER + 172)

/*
 * Hit-test a listbox cell at a client point and copy its text into the buffer (for the per-cell tooltip).
 * Used by: listbox.  Input: wParam = MAKELPARAM(x, y), lParam = char[].  Output: non-zero when the cell text is empty.
 */
#define OD_GETCELLTIP				(WM_USER + 173)

/*
 * Suppress the click sound on trackbar value changes.
 * Used by: trackbar.  Input: wParam = BOOL (0 = silent).  Output: none.
 */
#define OD_TRACKSILENT				(WM_USER + 174)

/*
 * Sent to a dialog's children once the animated reveal finishes; edit controls re-enable focus/tab-stop
 * and apply any focus that arrived during the animation.
 * Used by: edit controls.  Input: none.  Output: none.
 */
#define OD_ACTIVATE					(WM_USER + 175)

/*
 * No dedicated handler: re-enters the control's WndProc so the edit-box focus-deflection at function
 * entry runs again (used after focus changes).
 * Used by: edit controls.  Input: none.  Output: none.
 */
#define OD_REFOCUS					(WM_USER + 176)

/*
 * Set the scrollbar's "keep parent capture" flag.
 * Used by: scrollbar.  Input: lParam = BOOL.  Output: none.
 */
#define OD_SETKEEPCAPTURE			(WM_USER + 177)
#define OD_SETTEXTW					(WM_USER + 178)	// RA2: sets a control's wide caption (goptions 0x4DE01B)

/*
 * Initialize a combo-box dropdown window; seeds its highlighted selection from the owner combo.
 * Used by: combo-box dropdown (ComboDropWinCtrlProc).  Input: none.  Output: none.
 */
#define OD_DROPSUBCLASSED			(WM_USER + 1000)

/*
**	RA2: a modal control a dialog loop waits on (Is_Done / Is_Cancelled), and the text request owner-drawn
**	controls answer for message 0x4E9.
*/
namespace OwnerDraw {
class DialogControl { // RA2: the base of the controls dialogs report to (vtable 0x007A7B28)
	public:
		DialogControl(void); // RA2: 0x00601030
		virtual ~DialogControl(void); // RA2: as above
		virtual void Set_Window(unsigned int key, HWND window) = 0; // RA2: as above
		virtual bool Is_Done(void) = 0; // RA2: as above
		virtual HWND Get_Window(void) = 0; // RA2: as above
		virtual bool Is_Cancelled(void) = 0; // RA2: as above
};
class SimpleDialogControl : public DialogControl { // RA2: vtable 0x007A7B40
	public:
		SimpleDialogControl(void); // RA2: 0x006010C0, defined in another file
		virtual ~SimpleDialogControl(void); // RA2: 0x006010E0, as above
		virtual void Set_Window(unsigned int key, HWND window); // RA2: 0x006010F0
		virtual bool Is_Done(void); // RA2: 0x00601170, reads the byte at +8
		virtual HWND Get_Window(void); // RA2: 0x00601180, returns the dword at +4 (the dialogs store their answer there)
		virtual bool Is_Cancelled(void); // RA2: 0x00601190
		HWND window; // RA2: +0x04
		bool IsDone; // RA2: +0x08
};
int __fastcall Controlled_Dialog(int resource, DLGPROC procedure, DialogControl * control); // RA2: 0x006011A0, runs the dialog until its control reports
}

struct ToolTipTextRequest { // RA2: what a combo box asks for with message 0x4E9
		HWND Window; // RA2: the control
		int Item; // RA2: the list item under the cursor, -1 for none
		WideWstring Text; // RA2: the answer
};

template<class K, class V> class Dictionary;
// Declarations moved from the units that used them (one declaration per name).
extern bool __fastcall Focus_Next_Tab_Item(long lparam, bool flag); // RA2: 0x005DEF80
extern int __fastcall Dialog_Box(int resource, int(__stdcall * proc)(struct HWND__ *, unsigned int, unsigned int, long), bool show); // RA2: 0x005EAB60 (ownrdraw.cpp)
extern Dictionary<unsigned int, OwnerDraw::WinData> Control_Table; // RA2: as above
extern OwnerDraw::Tooltip ODTooltip; // RA2: as above
namespace OwnerDraw { extern int Masks_Ready; } // RA2: as above
namespace OwnerDraw { extern long ODLastTooltipTime; } // RA2: as above
namespace OwnerDraw { extern int Caret_Hidden; } // RA2: 0xA763EC, the caret blink state Draw_Edit_Text reads
namespace OwnerDraw { extern char const * Dialog_Back_Name; } // RA2: as above
extern HWND Dropdown_Window; // RA2: the open drop-down list (ownrdraw.cpp defines it)
extern HWND Dropdown_Owner; // RA2: the combo box that owns it

void __fastcall Resize_Dialog_To_Screen(HWND dialog, Point2D * old_size); // RA2: 0x5E9F00
