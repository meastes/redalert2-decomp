/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 * Ported to RA2 1.006 by RedAlert2 contributors, 2026 (from the RA2 recovery of
 * meastes/ra2-decomp and Ghidra).
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"

#include "ownrdraw.h"
#include "geoconst1.h" // RA2: the module's own copy of the geometry constants (its first initializers)
#include "_keyboar.h"
#include "_mixfile.h"
#include "_rules.h"
#include "_surface.h"
#include "_xmouse.h"
#include "arraylist.h"
#include "bsurface.h"
#include "conquer.h"
#include "data.h"
#include "dbgprint.h"
#include "dict.h"
#include "dsaudio.h"
#include "dsurface.h"
#include "globals.h"
#include "goptions.h"
#include "hsv.h"
#include "keyboard.h"
#include "misc.h"
#include "mixfile.h"
#include "msgloop.h"
#include "rgb.h"
#include "rules.h"
#include "session.h"
#include "srfcache.h"
#include "swap.h"
#include "theme.h"
#include "voc.h"
#include "vox.h"
#include "windlg.h"

#include <commctrl.h>
#include <ctime>
#include <sys\timeb.h>
#include <mbstring.h> // RA2: the includes this file needs
#include "bittext.h" // RA2: the includes this file needs
#include "dsurface.h"
#include "init.h" // RA2: the includes this file needs
#include "bitfont.h" // RA2: BitFont
#include "scenario.h" // RA2: IsGDI
#include "shapeset.h" // RA2: the dialog art
#include "voc.h" // RA2: PlayGlobal
#include "windlg.h" // RA2: Get_Display_Rect
#include "draw.h" // RA2: Draw_Shape
#include "session.h" // RA2: Is_Currently_In_Game
#include "stringtable.h" // RA2: string-table text
#include "dict.h" // RA2: wlib's Dictionary (the control and window tables)
#include "arraylist.h" // RA2: wlib's ArrayList
#include "geoconst2.h" // RA2: the module's slope and bridge constants (after globals.h)
#include "scroll.h"
#include "txtlabel2.h"
#include "win.h"
#include "winstub.h"

// RA2: the dialog code files its windows in the audio file window list (0x00A522B4), not in msgloop's modeless list
void __fastcall Add_Audio_File_Window(HWND window); // RA2: 0x522B20
void __fastcall Remove_Audio_File_Window(HWND window); // RA2: 0x522B80

namespace OwnerDraw { class WindowList; }

/*
**	RA2: a text node of a control record's list (TextNode) and the list as seen from the record
**	(NodeList: WinData's Nodes at +0x34).
*/
struct TextNode { // RA2: as above
	TextNode * Next; // RA2: as above
	int Reserved_04; // RA2: as above
	unsigned short * Data;		// RA2: its own Text
	int Reserved_0C; // RA2: as above
	unsigned short Text[1];	// RA2: allocated with room for the text
};

struct NodeList { // RA2: as above
	char Reserved_00[0x34]; // RA2: as above
	TextNode * Head; // RA2: as above
	TextNode * Add_Node(int length); // RA2: 0x6003D0
	void Remove_Node(TextNode * node); // RA2: 0x600410
};
class ShapeSet; // RA2: as above

/*
**	RA2: the WM_INITDIALOG lparam of a dialog the module creates (0x5FF610): the template id and the caller's value.
*/
struct DialogInitParam { // RA2: as above
	unsigned short Resource; // RA2: as above
	int Param; // RA2: as above
};

namespace OwnerDraw { // RA2: as above

/*
**	RA2: the key of the control-message dictionary (a message for a window).
*/
struct MessageKey { // RA2: as above
	int Message; // RA2: as above
	int Window; // RA2: as above
	bool operator == (MessageKey const & other) const; // RA2: as above
};

/*
**	RA2: the text request owner-drawn controls answer for message 0x4E9 (an item of a list or combo, by index).
*/
struct TextRequest { // RA2: as above
	int Item; // RA2: as above
	int Index; // RA2: as above
	WideWstring Text; // RA2: as above
};

}

// RA2: the module's functions, declared before their first use
unsigned int __fastcall Hash_Key(unsigned int &key); // RA2: as above
int __fastcall Hash_CtrlMsg(int * pair); // RA2: as above
bool __fastcall Focus_Next_Tab_Item(HWND window, unsigned int previous); // RA2: as above
int __fastcall OwnerDraw_Control_Height(unsigned int key, HWND window); // RA2: as above
unsigned char __fastcall Kind_Test(int kind); // RA2: as above
int __fastcall OwnerDraw_Control_Kind(unsigned int key, HWND window); // RA2: as above
int __fastcall OwnerDraw_Control_Flags(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Tested_Control(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Listed_Kind(int kind); // RA2: as above
void __fastcall Send_Control_Value(HWND window, unsigned char value); // RA2: as above
void __fastcall Send_Control_Flag(HWND window, unsigned char flag); // RA2: as above
void __fastcall Send_Control_Pair(HWND window, int low, int high); // RA2: as above
void __fastcall Slide_Pending_Dialog(HWND window); // RA2: as above
bool __fastcall Is_Owner_Tested_Control(unsigned int key, HWND window); // RA2: as above
unsigned int __fastcall Count_Free_Buttons(HWND window); // RA2: as above
void __fastcall Assign_All_Control_Sounds(HWND window); // RA2: as above
int __fastcall Dialog_Box(int resource, int (__stdcall *proc)(struct HWND__ *, unsigned int, unsigned int, long), bool Show); // RA2: as above
int __fastcall OD_Draw_Text(unsigned int color, BitFont * font, Rect const & area, wchar_t const * text, int length, int x_alignment, int y_alignment, Surface * surface, int shown); // RA2: as above
void __fastcall Dialog_On_OD_4EC(HWND dialog); // RA2: 0x5E8710
int __fastcall Dialog_ID_Of_Window(HWND window); // RA2: 0x5EAB00
void __fastcall Dialog_On_WM_PAINT(HWND dialog); // RA2: 0x5FEE70
void __fastcall Dialog_On_WM_DRAWITEM(long item); // RA2: as above
BOOL __stdcall Redraw_Window(HWND window, RECT *rect); // RA2: as above
void __fastcall Show_Dialog(HWND dialog, int kind); // RA2: as above
void __fastcall Move_Window(HWND window, int x, int y); // RA2: as above
int __stdcall Cancel_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
int __fastcall Clear_ComboBox(HWND combo); // RA2: as above
bool __fastcall Prompt_Dialog(unsigned short const *text); // RA2: as above
int __stdcall Choice_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
int __fastcall Clear_List_Box(HWND listbox); // RA2: as above
LRESULT __fastcall Get_Control_Text(HWND window, Wstring &text); // RA2: as above
LRESULT __fastcall Get_Control_Wide_Text(HWND window, WideWstring &text); // RA2: as above
LRESULT __fastcall Get_List_Item_Text(HWND window, WPARAM index, Wstring &text); // RA2: as above
LRESULT __fastcall Get_List_Item_Wide_Text(HWND window, WPARAM index, WideWstring &text); // RA2: as above
LRESULT __fastcall Get_Combo_Item_Text(HWND window, WPARAM index, Wstring &text); // RA2: as above
LRESULT __fastcall Get_Combo_Item_Wide_Text(HWND window, WPARAM index, WideWstring &text); // RA2: as above
namespace OwnerDraw { // RA2: as above
void Init_Default_Colors(); // RA2: as above
int __fastcall Control_Typing_Sound(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Image_Control(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Image_Mode3_Control(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Animated_Control(unsigned int key, HWND window); // RA2: as above
int __fastcall Control_Frame_Delay(unsigned int key, HWND window); // RA2: as above
int __fastcall Control_Sound_Slot(HWND dialog, HWND window); // RA2: as above
int __fastcall Control_Animation_Loops(unsigned int key, HWND window); // RA2: as above
int __fastcall Control_Animation_Rate(unsigned int key, HWND window); // RA2: as above
void *__fastcall Control_Animation_Art(unsigned int key, HWND window); // RA2: as above
int __fastcall Control_Image(unsigned int key, HWND window); // RA2: as above
void const *__fastcall Control_Animation_Shape(unsigned int key, HWND window, bool &allocated); // RA2: as above
int __fastcall Control_Text_Style(HWND dialog, HWND window); // RA2: as above
void __fastcall Set_Control_Image(HWND control, Surface *image); // RA2: as above
bool __fastcall Is_Control_Checked(HWND window); // RA2: as above
void __fastcall Set_Control_Result(HWND control, int value); // RA2: as above
int __fastcall Get_Control_Result(HWND control); // RA2: as above
WideWstring __fastcall Get_Item_Tip_Text(HWND window, int a, int b); // RA2: as above
WideWstring __fastcall Get_Control_Tip_Text(HWND window, int a); // RA2: as above
WideWstring __fastcall Get_Item_Tip_Text_At(HWND window, HWND item, int x, int y); // RA2: as above
WideWstring __fastcall Control_Tooltip(unsigned int key, HWND window); // RA2: as above
char const *__fastcall Control_Tooltip_Label(unsigned int key, HWND window); // RA2: as above
void __fastcall Set_Control_Disabled(HWND control, bool value); // RA2: as above
bool __fastcall Is_Control_Disabled(HWND control); // RA2: as above
BOOL __stdcall Enable_Animated_Control(HWND window, LPARAM lparam); // RA2: as above
void __fastcall Enable_Animated_Controls(HWND window); // RA2: as above
void __fastcall Disable_Animated_Controls(HWND window); // RA2: as above
void __fastcall Slide_Dialog(HWND dialog, bool reverse); // RA2: as above
bool __fastcall WS_Save_Dialog_Values(HWND dialog); // RA2: as above
bool __fastcall Play_Dialog_Move_In(HWND dialog); // RA2: as above
void __fastcall Set_Dialog_Hidden(HWND control); // RA2: as above
void __fastcall Clear_Dialog_Hidden(HWND control); // RA2: as above
void __fastcall Set_Dialog_Waiting(HWND control); // RA2: as above
void __fastcall Clear_Dialog_Waiting(HWND control); // RA2: as above
bool __fastcall Select_Control_Rect(HWND dialog,HWND control,Rect *bounds); // RA2: as above
bool __fastcall Is_Free_Owner_Button(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Default_Button(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Owner_Default_Button(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Flag_Control(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Campaign_Button(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Network_Button(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Cancel_Button(unsigned int key, HWND window); // RA2: as above
bool __fastcall Is_Main_Menu_Movie(unsigned int key, HWND window); // RA2: as above
BOOL __stdcall Count_Visible_Free_Buttons(HWND window, LPARAM); // RA2: as above
BOOL __stdcall Count_Visible_Default_Buttons(HWND window, LPARAM); // RA2: as above
bool __fastcall Has_Visible_Default_Button(HWND dialog); // RA2: as above
BOOL __stdcall Assign_Button_Kind(HWND window, LPARAM); // RA2: as above
void __fastcall Assign_Button_Kinds(HWND window); // RA2: as above
BOOL __stdcall Init_Control_Animation(HWND window, LPARAM); // RA2: as above
BOOL __fastcall Is_Listed_Dialog_Kind(int kind); // RA2: as above
void __fastcall Init_Control_Animations(HWND window); // RA2: as above
BOOL __stdcall Reset_Tested_Control(HWND window, LPARAM); // RA2: as above
BOOL __stdcall Assign_Control_Sounds(HWND window, LPARAM); // RA2: as above
void __fastcall Move_Control_To_Rect(HWND window, Rect const &rect); // RA2: as above
void __fastcall Move_Free_Button(HWND window, Point2D const &size); // RA2: as above
void __fastcall Move_Tested_Control(HWND window, Point2D const &size); // RA2: as above
void __fastcall Move_Default_Button_Owner(HWND window); // RA2: as above
void __fastcall Move_Default_Button(HWND window); // RA2: as above
void __fastcall Move_Flag_Control(HWND window); // RA2: as above
void __fastcall Move_Lobby_Control(HWND window); // RA2: as above
void __fastcall Move_Centred_Control(HWND window, Point2D const &size); // RA2: as above
void __fastcall Adjust_Control_Position(HWND window); // RA2: as above
BOOL __stdcall Move_Child_For_Resolution(HWND window, LPARAM lparam); // RA2: as above
int __fastcall Control_Drawer(HWND control); // RA2: as above
bool __fastcall Is_Framed_Dialog(HWND dialog); // RA2: as above
bool __fastcall Centre_Dialog(HWND control); // RA2: as above
bool __fastcall Is_Lobby_Frame_Dialog(HWND control); // RA2: as above
bool __fastcall Update_Lobby_Frame_Dialog(HWND control); // RA2: as above
void __fastcall Set_Lobby_Frame_Dialog(HWND control); // RA2: as above
void __fastcall Clear_Lobby_Frame_Dialog(HWND control); // RA2: as above
bool __fastcall Is_Lobby_Art_Dialog(HWND control); // RA2: as above
bool __fastcall Update_Lobby_Art_Dialog(HWND control); // RA2: as above
void __fastcall Set_Lobby_Art_Dialog(HWND control); // RA2: as above
void __fastcall Clear_Lobby_Art_Dialog(HWND control); // RA2: as above
bool __fastcall Is_Chat_Dialog(HWND control); // RA2: as above
bool __fastcall Update_Chat_Dialog(HWND control); // RA2: as above
bool __fastcall Is_Setup_Dialog(HWND control); // RA2: as above
bool __fastcall Update_Setup_Dialog(HWND control); // RA2: as above
int __fastcall Control_Shape_640(HWND control); // RA2: as above
int __fastcall Control_Shape(HWND control); // RA2: as above
void __fastcall Set_Control_Art(unsigned int key); // RA2: as above
void __fastcall Set_Dialog_Kind(HWND control, int value); // RA2: as above
void __fastcall Register_Window_Classes(); // RA2: as above
LRESULT __stdcall Combo_Drop_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
BOOL __stdcall Replace_Edit(HWND window, LPARAM); // RA2: as above
bool __fastcall Subclass_Dialog(HWND window, LPARAM lparam); // RA2: as above
BOOL __stdcall Set_Paint_Disabled(HWND window, LPARAM lparam); // RA2: as above
BOOL __stdcall Subclass_Control(HWND window, LPARAM lparam); // RA2: as above
BOOL __fastcall Restore_Window(HWND window); // RA2: as above
BOOL __stdcall Restore_Child(HWND window, LPARAM lparam); // RA2: as above
bool __fastcall Start_Tooltip(Rect const &area, unsigned short const *text, HWND window); // RA2: as above
bool __fastcall Show_Tooltip(bool save_background); // RA2: as above
bool Hide_Tooltip(); // RA2: as above
bool End_Tooltip(); // RA2: as above
LRESULT __stdcall Control_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Default_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Owner_Button_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Tab_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Edit_Control_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam); // RA2: as above
int __fastcall Edit_Text(HWND window, wchar_t *text, int size, int *caret); // RA2: as above
LRESULT __stdcall Static_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Checkbox_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam); // RA2: as above
LRESULT __stdcall Button_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam); // RA2: as above
LRESULT __stdcall Scroll_Bar_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Progress_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Trackbar_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Group_Box_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Hotkey_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
void __fastcall Hotkey_Text(unsigned short hotkey, wchar_t *text); // RA2: as above
void Init_Color_Masks(); // RA2: as above
void __fastcall Load_Dialog_Images(); // RA2: as above
bool __fastcall Draw_Edge_Glow(Surface *surface, Point2D const &start, Point2D const &end, int color, int alpha); // RA2: as above
void __fastcall Draw_Edge_Glows(Surface *surface, Rect const &rect, int raised, int count, int left_alpha, int top_alpha, int right_alpha, int bottom_alpha); // RA2: as above
void __fastcall Draw_Arrow(Surface *surface, Rect const &rect, int upward, int pressed, bool prefixed); // RA2: as above
int __fastcall Color_To_Pixel(unsigned long color); // RA2: as above
unsigned long __fastcall Pixel_To_Color(unsigned short pixel); // RA2: as above
void __fastcall Draw_Frame(Surface *surface, Rect const &rect, int thickness, int color); // RA2: as above
void __fastcall Draw_Button_Frame(Surface *surface, Rect const &rect, int thickness); // RA2: as above
int __fastcall Draw_Text_In_Rect(Surface *surface, wchar_t const *text, RECT *area, BitFont *font, unsigned long color, int flags); // RA2: as above
int __fastcall Draw_Text_In_Rect_Ex(Surface *surface, wchar_t const *text, RECT *area, BitFont *font, unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10); // RA2: as above
float __fastcall Glow_Factor(int value); // RA2: as above
void __fastcall Draw_Dimmed_Back(Rect const &area, HWND window, int alpha); // RA2: as above
void __fastcall Draw_Progress_Fill(Rect const &area, Surface *surface, int color, int fraction); // RA2: as above
void __fastcall Shade_Edges(Rect const &area, Surface &surface, int left_width, int top_height); // RA2: as above
void __fastcall Draw_Bits(Rect const *area, Surface *surface); // RA2: as above
BOOL __stdcall Add_Window(HWND window, WindowList *list); // RA2: as above
int Release_Mouse_For_Dialog(); // RA2: as above
int Capture_Mouse_After_Dialog(); // RA2: as above
void __fastcall End_Dialog(HWND window); // RA2: as above
void __fastcall Display_Dialog(HWND window); // RA2: as above
bool Dialog_Message_Handler(); // RA2: as above
HWND __fastcall Custom_Message_Box(unsigned short const *text, unsigned short const *button, bool *cancelled); // RA2: as above
void __fastcall Draw_Edit_Text(Surface * surface, Rect & rect, wchar_t const * text, int caret, BitFont * font, unsigned int color, int * scroll, int ime, int password, int frame, int shown); // RA2: as above
int __fastcall Controlled_Dialog(int resource,DLGPROC procedure,DialogControl *control); // RA2: as above
bool __fastcall Control_Test(HWND window); // RA2: as above
}

namespace OwnerDraw { extern unsigned short Blue_Mask; } // RA2: as above
namespace OwnerDraw { extern unsigned short Red_Mask; } // RA2: as above
namespace OwnerDraw { extern unsigned short Green_Mask; } // RA2: as above
bool Is_In_Main_Loop(); // RA2: as above
bool __fastcall Main_Loop(); // RA2: as above
void __stdcall Set_Paint_Disabled_Children(struct HWND__ *window, int unknown2); // RA2: as above



namespace OwnerDraw { // RA2: as above

/*
**	RA2: the record of a subclassed window, or NULL.
*/
inline WinData * __cdecl Get_Record(HWND window) // RA2: as above
{
	WinData * record = 0; // RA2: as above
	Control_Table.getPointer((unsigned int const &)window, &record); // RA2: as above
	return(record); // RA2: as above
}

/*
**	RA2: how far a sidebar-anchored control sits in from the edge (none in a game session).
*/
inline int __cdecl Sidebar_Inset(int width, unsigned char ingame) // RA2: as above
{
	int inset = 0; // RA2: as above
	if (!ingame) { // RA2: as above
		int excess = (width - Base_Screen_Width) / 2; // RA2: as above
		inset = excess < 0 ? 0 : excess; // RA2: as above
	}
	return(inset); // RA2: as above
}

/*
**	RA2: a control's sidebar margin, its record's value or centred in the sidebar width.
*/
inline int __cdecl Sidebar_Margin(HWND window, int width) // RA2: as above
{
	WinData * record = 0; // RA2: as above
	Control_Table.getPointer((unsigned int const &)window, &record); // RA2: as above
	if (record && record->SoundSlot) return(record->SoundSlot); // RA2: as above
	return((SidebarWidth - width) / 2); // RA2: as above
}

}
void __fastcall Show_Dialog_With_Flag(HWND dialog); // RA2: 0x5115F0
namespace OwnerDraw { LRESULT __stdcall New_Edit_Window_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); } // RA2: 0x5EAD00
void IME_Update_Composition(void); // RA2: 0x737E30

/*
**	RA2: the rectangle moved by (x, y).
*/
inline Rect __cdecl Offset_Rect(Rect const & rect, int x, int y) // RA2: as above
{
	return(Rect(rect.X + x, rect.Y + y, rect.Width, rect.Height)); // RA2: as above
}
namespace OwnerDraw { void Shutdown(void); } // RA2: 0x5EAC20


SurfaceCacheClass SurfaceCache;


namespace OwnerDraw { // RA2: as above

void Init_Default_Colors() // RA2: as above
{
    Border_Width = 1; // RA2: as above
    Glow_Color = 0x7f; // RA2: as above
    Text_Color = 0xffff; // RA2: as above
    Highlight_Text_Color = 0xeeeeee; // RA2: as above
    Tab_Text_Color = 0x9f9f; // RA2: as above
    White_Color = 0xffffff; // RA2: as above
    Grey_Color = 0x808080; // RA2: as above
    Fill_Color = 0xff; // RA2: as above
    Pressed_Fill_Color = 0x626262; // RA2: as above
    Default_Frame_Color = 0xff; // RA2: as above
    Alternate_Frame_Color = 0x929292; // RA2: as above
    Disabled_Text_Color = 0x9f; // RA2: as above
    Light_Grey_Color = 0x8f8f8f; // RA2: as above
    Frame_Color = 0x9f; // RA2: as above
    Dark_Grey_Color = 0x646464; // RA2: as above
    Dim_Alpha = 0x60; // RA2: as above
    Olive_Color = 0xaaaa; // RA2: as above
    Dark_Brown_Color = 0x221b0b; // RA2: as above
    Brown_Color = 0x443716; // RA2: as above
}

}


// RA2: the ra2-decomp column copy constructor is defined in the owner-draw unit.
OwnerDraw::ColumnData::ColumnData(ColumnData & that)
	: xPos(that.xPos), width(that.width), cells(that.cells)
{
}


// RA2: TibSun's list box cell constructor (its stores in TibSun's order).
OwnerDraw::CellData::CellData(void)
{
	type = CellData::INVALID;
	color = -1;
	pingtime = -1;
	surf = NULL;
}


/// <summary>
/// Fetches the hash value for a window handle.
/// This is the hash routine handed to the dictionaries that key their entries by the
/// window handle of a subclassed control.
/// </summary>
/// <returns>Returns with the handle itself, taken as an unsigned value.</returns>
unsigned int __fastcall Hash_Key(unsigned int &key) // RA2: the key as its own hash
{
    return key; // RA2: as above
}


/// <summary>
/// Fetches the hash value for a control message key.
/// This is the hash routine handed to the dictionary CtrlProc keeps of the messages it is
/// already in the middle of handling.
/// </summary>
/// <returns>Returns with the hash value formed from the window and the message.</returns>
int __fastcall Hash_CtrlMsg(int * pair) // RA2: as above
{
    return pair[1] * *pair; // RA2: as above
}


/// <summary>
/// Determines if two control message keys refer to the same thing.
/// </summary>
/// <returns>bool; Do both keys name the same window and the same message?</returns>
namespace OwnerDraw { // RA2: as above

bool MessageKey::operator==(MessageKey const &other) const // RA2: as above
{
    if (other.Window == Window && other.Message == Message) { // RA2: as above
        return true; // RA2: as above
    }
    return false; // RA2: as above
}

}


/// <summary>
/// Sets the owner-draw metrics and colors to their defaults.
/// This routine establishes the border thickness, the blend strength and the family of
/// colors that every owner-draw control paints with. Each control asks for it as it is
/// subclassed, so the values are always current.
/// </summary>
unsigned int __fastcall Hash_Window(HWND &key); // RA2: as above

Dictionary<HWND, WNDPROC> Window_Proc_Table(Hash_Window); // RA2: as above


unsigned int __fastcall Hash_Window(HWND &key); // RA2: as above

Dictionary<HWND, WNDPROC> Window_Table(Hash_Window); // RA2: as above


inline void NodeList::Remove_Node(TextNode *node) // RA2: as above
{
    TextNode *previous = 0; // RA2: as above
    TextNode *current = Head; // RA2: as above
    while (current != 0) { // RA2: as above
        if (node == current) { // RA2: as above
            break;
        }
        previous = current; // RA2: as above
        current = current->Next; // RA2: as above
    }
    if (current != 0) { // RA2: as above
        if (previous != 0) { // RA2: as above
            TextNode *next = current->Next; // RA2: as above
            previous->Next = next; // RA2: as above
        } else {
            TextNode *next = current->Next; // RA2: as above
            Head = next; // RA2: as above
        }
        operator delete(current); // RA2: as above
    }
}

namespace OwnerDraw { // RA2: as above

inline WinData::~WinData() // RA2: as above
{
    Set_Wide_Text(0); // RA2: as above
    while (Nodes) { // RA2: as above
        ((NodeList *)this)->Remove_Node(Nodes); // RA2: as above
    }
    delete WideText; // RA2: as above
}

}

unsigned int __fastcall Hash_Key(unsigned int const &key); // RA2: as above

Dictionary<unsigned int, OwnerDraw::WinData> Control_Table(Hash_Key); // RA2: as above


using namespace OwnerDraw; // RA2: the per-function files


bool __fastcall Focus_Next_Tab_Item(HWND window, unsigned int previous) // RA2: as above
{
    HWND item; // RA2: as above

    item = (GetParent(window)); // RA2: as above
    item = (GetNextDlgTabItem(item, window, (int)(previous & 0xff))); // RA2: as above
    if (item != window) { // RA2: as above
        SetFocus(item); // RA2: as above
        return 1; // RA2: as above
    }
    return 0; // RA2: as above
}


int __fastcall OwnerDraw_Control_Height(unsigned int key, HWND window) // RA2: as above
{
    OwnerDraw::WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    bool general = ((((((kind == 0x10c) || (kind == 0x94)) || (kind == 0xa3)) || // RA2: as above
                ((((((kind == 0x73 || (kind == 0xd8)) || // RA2: as above
                        ((kind == 0xbbb || ((kind == 0xf5 || (kind == 0x105)))))) || (kind == 0xe2)) || // RA2: as above
                    ((((kind == 0xd5 || (kind == 0x2b5)) || (kind == 0xb7)) || // RA2: as above
                      ((kind == 0x2b4 || (kind == 0xbc7)))))) || // RA2: as above
                  (((kind == 0x101 || ((kind == 0x129 || (kind == 0xbc6)))) || (kind == 0xd7)))))) || // RA2: as above
              (((((kind == 0x108 || (kind == 0xbc)) || (kind == 0xbd)) || // RA2: as above
                  ((kind == 0xbb || (kind == 0xb5)))) || // RA2: as above
                ((kind == 0xbba || ((kind == 0xff || (kind == 0x6b)))))))) || // RA2: as above
            ((((kind == 0x100 || (((kind == 0x102 || (kind == 0xb8)) || (kind == 0xd6)))) || // RA2: as above
                (((kind == 0x103 || (kind == 0x125)) || // RA2: as above
                  ((kind == 0xe7 || ((kind == 0x116 || (kind == 0xc2)))))))) || // RA2: as above
              ((kind == 0xc9 || // RA2: as above
                (((((kind == 0x11d || (kind == 0x11c)) || (kind == 0x114)) || // RA2: as above
                    (((kind == 700 || (kind == 0x113)) || // RA2: as above
                      ((kind == 0x109 || ((kind == 0x10f || (kind == 0x117)))))))) || // RA2: as above
                  ((kind == 0xe6 || (((kind == 0xf3 || (kind == 0xf4)) || (kind == 0x10e)))))))))))) && // RA2: as above
          (id == 0x694); // RA2: as above
    if (general) { // RA2: as above
        return (0x1e); // RA2: as above
    }
    bool dual_rail = (((kind == 0xbc) || (kind == 0xbd)) || // RA2: as above
            ((kind == 0x102 || ((kind == 0xc2 || (kind == 0xc9)))))) && // RA2: as above
          ((id == 0x6ec || (id == 0x5a8))); // RA2: as above
    if (dual_rail) { // RA2: as above
        return (0x1e); // RA2: as above
    }
    if ((kind == 0x103) || (kind == 0xbc7)) { // RA2: as above
        if (id != 0x6f4) { // RA2: as above
            if (id == 0x6f3 || id == 0x69f || id == 0x6f6 || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2 || id == 0x73c || id == 0x73d || id == 0x73e || id == 0x73f || id == 0x740 || id == 0x735 || id == 0x732 || id == 0x3ea || id == 0x733) { // RA2: as above
                return 0x1e; // RA2: as above
            }
            bool tall = ((((((id == 0x411) || (id == 0x412)) || (id == 0x419)) || // RA2: as above
                        ((id == 0x41a || (id == 0x6a3)))) || // RA2: as above
                      ((id == 0x6a4 || ((id == 0x6a5 || (id == 0x6a7)))))) || (id == 0x6a6)) || // RA2: as above
                  ((((id == 0x6a8 || (id == 0x73a)) || (id == 0x739)) || // RA2: as above
                    ((id == 0x73b || (id == 0x738))))); // RA2: as above
            if (tall) { // RA2: as above
                return (0x3c); // RA2: as above
            }
        } else {
            return 0xf; // RA2: as above
        }
    }
    if ((kind == 0x108) || (kind == 0xbc6)) { // RA2: as above
        if (id == 0x6d2 || id == 0x3ea || id == 0x733 || id == 0x71b || id == 0x732 || id == 0x69d || id == 0x69f || id == 0x69e || id == 0x6a0 || id == 0x6a1 || id == 0x5c8 || id == 0x411 || id == 0x412 || id == 0x413 || id == 0x414 || id == 0x415 || id == 0x416 || id == 0x417 || id == 0x418) { // RA2: as above
            return 0x1e; // RA2: as above
        }
        bool tall2 = ((((id == 0x419) || (id == 0x41a)) || (id == 0x41b)) || // RA2: as above
                (((((((id == 0x41c || (id == 0x41d)) || // RA2: as above
                          ((id == 0x41e || ((id == 0x41f || (id == 0x420)))))) || (id == 0x6a3)) || // RA2: as above
                      (((((id == 0x6a4 || (id == 0x6a9)) || (id == 0x6ac)) || // RA2: as above
                          ((id == 0x6af || (id == 0x6b2)))) || (id == 0x6b5)))) || // RA2: as above
                    (((id == 0x6b8 || (id == 0x6a5)) || // RA2: as above
                      (((id == 0x6a7 || (((id == 0x6aa || (id == 0x6ad)) || (id == 0x6b0)))) || // RA2: as above
                        (((id == 0x6b3 || (id == 0x6b6)) || (id == 0x6b9)))))))) || // RA2: as above
                  ((id == 0x6a6 || (id == 0x6a8)))))) || // RA2: as above
              (((id == 0x6ab || (((id == 0x6ae || (id == 0x6b1)) || (id == 0x6b4)))) || // RA2: as above
                (((id == 0x6b7 || (id == 0x6ba)) || // RA2: as above
                  ((id == 0x70a || // RA2: as above
                    (((id == 0x70b || (id == 0x70c)) || // RA2: as above
                      ((id == 0x70d || // RA2: as above
                        ((((id == 0x70e || (id == 0x70f)) || (id == 0x710)) || (id == 0x711))))))))) // RA2: as above
                  )))); // RA2: as above
        if (tall2) { // RA2: as above
            return (0x3c); // RA2: as above
        }
    }
    if (Kind_Test(kind) != 0 && id == 0x695) { // RA2: as above
        return 0xf; // RA2: as above
    }
    return 0xf; // RA2: as above
}


/// <summary>
/// Subclasses a dialog and every one of its controls for owner drawing.
/// This is the routine a dialog procedure calls when it is first created. Painting is
/// suppressed across the whole window while the controls are being hooked, so the dialog
/// never flickers through its stock Windows appearance on the way.
/// </summary>
/// <param name="lparam">Caller supplied value to store with each control for its own use.</param>
unsigned char __fastcall Kind_Test(int kind) // RA2: as above
{
    if ((((((kind != 0x10c) && (kind != 0x94)) && (kind != 0xb6)) && // RA2: as above
              ((((kind != 0xa3 && (kind != 0x73)) && // RA2: as above
                  ((kind != 0xd8 && ((kind != 0xbbb && (kind != 0xf5)))))) && // RA2: as above
                ((kind != 0x105 && // RA2: as above
                  ((((kind != 0xe2 && (kind != 0xd5)) && (kind != 0x2b5)) && // RA2: as above
                    ((kind != 0xb7 && (kind != 0x2b4)))))))))) && // RA2: as above
            ((((kind != 0x101 && ((kind != 0x129 && (kind != 0xbc7)))) && (kind != 0xbc6)) && // RA2: as above
              (((((((kind != 0xd7 && (kind != 0x108)) && (kind != 0xbc)) && // RA2: as above
                      ((kind != 0xbd && (kind != 0xbb)))) && // RA2: as above
                    ((kind != 0xb5 && ((kind != 0xbba && (kind != 0xff)))))) && (kind != 0xea)) && // RA2: as above
                (((kind != 0x6b && (kind != 0x100)) && (kind != 0x102)))))))) && // RA2: as above
          ((((((kind != 0xb8 && (kind != 0xd6)) && // RA2: as above
                  ((kind != 0x103 && ((kind != 0x11f && (kind != 0x125)))))) && (kind != 0x115)) // RA2: as above
              && ((((((kind != 0x122 && (kind != 0x112)) && (kind != 0xe7)) && // RA2: as above
                          (((kind != 0x116 && (kind != 0xc2)) && // RA2: as above
                            ((kind != 0xc9 && ((kind != 0x11d && (kind != 0x11c)))))))) && // RA2: as above
                        (kind != 0x113)) && // RA2: as above
                      (((((kind != 0xfe && (kind != 0x109)) && (kind != 0x10f)) && // RA2: as above
                          ((kind != 0x117 && (kind != 0x114)))) && (kind != 0xc3)))))) && // RA2: as above
            ((((kind != 0x124 && (kind != 0xe6)) && // RA2: as above
                ((kind != 0xf3 && (((kind != 0xf4 && (kind != 700)) && (kind != 0x11b)))))) && // RA2: as above
              (kind != 0x10e)))))) { // RA2: as above
        return 0; // RA2: as above
    }
    return 1; // RA2: as above
}


/// <summary>
/// Sets the paint suppression flag for one window.
/// This is the enumeration callback Subclass_Dialog uses to silence, and later re-enable,
/// painting across a whole dialog. A window with no owner-draw record yet is given one.
/// </summary>
/// <param name="lparam">Should painting be suppressed for this window?</param>
int __fastcall OwnerDraw_Control_Kind(unsigned int key, HWND window) // RA2: as above
{

    OwnerDraw::WinData *record = 0; // RA2: as above

    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (((((((((((kind != 0x10c) && (kind != 0x94)) && (kind != 0xa3)) && // RA2: as above
                        (((kind != 0x73 && (kind != 0xd8)) && // RA2: as above
                          ((kind != 0xbbb && ((kind != 0xf5 && (kind != 0x105)))))))) && // RA2: as above
                      ((kind != 0xe2 && // RA2: as above
                        (((((kind != 0xd5 && (kind != 0x2b5)) && (kind != 0xb7)) && // RA2: as above
                            ((kind != 0x2b4 && (kind != 0xbc7)))) && (kind != 0x101)))))) && // RA2: as above
                    (((kind != 0x129 && (kind != 0xbc6)) && // RA2: as above
                      ((kind != 0xd7 && (((kind != 0x108 && (kind != 0xbc)) && (kind != 0xbd)))))))) && // RA2: as above
                  (((((kind != 0xbb && (kind != 0xb5)) && (kind != 0xbba)) && // RA2: as above
                      ((kind != 0xff && (kind != 0x6b)))) && // RA2: as above
                    ((((kind != 0x100 && (((kind != 0x102 && (kind != 0xb8)) && (kind != 0xd6)))) && // RA2: as above
                        ((kind != 0x103 && (kind != 0x125)))) && (kind != 0xe7)))))) && // RA2: as above
                ((((kind != 0x116 && (kind != 0xc2)) && // RA2: as above
                    ((kind != 0xc9 && // RA2: as above
                      ((((kind != 0x11d && (kind != 0x11c)) && (kind != 0x114)) && // RA2: as above
                        ((kind != 700 && (kind != 0x113)))))))) && (kind != 0x109)))) && // RA2: as above
              (((kind != 0x10f && (kind != 0x117)) && // RA2: as above
                ((kind != 0xe6 && (((kind != 0xf3 && (kind != 0xf4)) && (kind != 0x10e)))))))) || // RA2: as above
            (id != 0x694)) && // RA2: as above
          ((((kind != 0xbc && (kind != 0xbd)) && // RA2: as above
              ((kind != 0x102 && ((kind != 0xc2 && (kind != 0xc9)))))) || // RA2: as above
            ((id != 0x6ec && (id != 0x5a8)))))) { // RA2: as above
        if ((kind == 0x103) || (kind == 0xbc7)) { // RA2: as above
            if (id == 0x6f4) { // RA2: as above
                return (3); // RA2: as above
            }
            if (id == 0x6f3) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x69f) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6f6) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6ef) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6f1) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6f0) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6f2) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x733) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x735) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x732) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x3ea) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x73c) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x73d) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x73e) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x73f) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x740) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x736) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x737) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x411) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x412) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x419) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x41a) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6a3) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6a4) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6a5) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6a7) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6a6) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x6a8) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x73a) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x739) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x73b) { // RA2: as above
                return (1); // RA2: as above
            }
            if (id == 0x738) { // RA2: as above
                return (1); // RA2: as above
            }
        }
        if ((((kind != 0x108) && (kind != 0xbc6)) || // RA2: as above
                ((((((id != 0x6d2 && (((id != 0x3ea && (id != 0x733)) && (id != 0x71b)))) && // RA2: as above
                        ((((((id != 0x732 && (id != 0x69d)) && (id != 0x69f)) && // RA2: as above
                              ((id != 0x69e && (id != 0x6a0)))) && // RA2: as above
                            (((id != 0x6a1 && // RA2: as above
                                ((((id != 0x5c8 && (id != 0x411)) && (id != 0x412)) && // RA2: as above
                                  ((id != 0x413 && (id != 0x414)))))) && (id != 0x415)))) && // RA2: as above
                          ((id != 0x416 && (id != 0x417)))))) && // RA2: as above
                      ((id != 0x418 && (((id != 0x419 && (id != 0x41a)) && (id != 0x41b)))))) && // RA2: as above
                    ((((id != 0x41c && (id != 0x41d)) && (id != 0x41e)) && // RA2: as above
                      (((((id != 0x41f && (id != 0x420)) && // RA2: as above
                            ((id != 0x6a3 && (((id != 0x6a4 && (id != 0x6a9)) && (id != 0x6ac)))))) && // RA2: as above
                          ((id != 0x6af && (id != 0x6b2)))) && // RA2: as above
                        (((id != 0x6b5 && // RA2: as above
                            (((id != 0x6b8 && (id != 0x6a5)) && // RA2: as above
                              ((id != 0x6a7 && // RA2: as above
                                (((((id != 0x6aa && (id != 0x6ad)) && (id != 0x6b0)) && // RA2: as above
                                    ((id != 0x6b3 && (id != 0x6b6)))) && (id != 0x6b9)))))))) && // RA2: as above
                          (((id != 0x6a6 && (id != 0x6a8)) && (id != 0x6ab)))))))))) && // RA2: as above
                  (((((id != 0x6ae && (id != 0x6b1)) && (id != 0x6b4)) && // RA2: as above
                      ((((id != 0x6b7 && (id != 0x6ba)) && // RA2: as above
                          ((id != 0x70a && ((id != 0x70b && (id != 0x70c)))))) && (id != 0x70d)))) && // RA2: as above
                    ((((id != 0x70e && (id != 0x70f)) && (id != 0x710)) && (id != 0x711)))))))) && // RA2: as above
              ((Kind_Test(kind) != 0 && id == 0x695))) { // RA2: as above
            return (3); // RA2: as above
        }
    }
    return (1); // RA2: as above
}


/// <summary>
/// Prepares one control for owner drawing.
/// This is the enumeration callback Subclass_Dialog uses. The control's window class and style
/// pick the custom procedure that will paint it, its original procedure is displaced by
/// CtrlProc and remembered, and the control is then told that it has been subclassed. The
/// shared fonts, color masks and cached artwork are built on the first control to arrive.
/// </summary>
/// <param name="lparam">Caller supplied value to store with the control for its own use.</param>
int __fastcall OwnerDraw_Control_Flags(unsigned int key, HWND window) // RA2: as above
{

    OwnerDraw::WinData *record = 0; // RA2: as above

    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if ((((((((((kind != 0x10c) && (kind != 0x94)) && (kind != 0xa3)) && // RA2: as above
                      (((kind != 0x73 && (kind != 0xd8)) && // RA2: as above
                        ((kind != 0xbbb && ((kind != 0xf5 && (kind != 0x105)))))))) && (kind != 0xe2)) && // RA2: as above
                  (((((kind != 0xd5 && (kind != 0x2b5)) && (kind != 0xb7)) && // RA2: as above
                      ((kind != 0x2b4 && (kind != 0xbc7)))) && (kind != 0x101)))) && // RA2: as above
                ((((((kind != 0x129 && (kind != 0xbc6)) && // RA2: as above
                        (((kind != 0xd7 && (((kind != 0x108 && (kind != 0xbc)) && (kind != 0xbd)))) && // RA2: as above
                          (((kind != 0xbb && (kind != 0xb5)) && (kind != 0xbba)))))) && // RA2: as above
                      ((kind != 0xff && (kind != 0x6b)))) && // RA2: as above
                    (((kind != 0x100 && (((kind != 0x102 && (kind != 0xb8)) && (kind != 0xd6)))) && // RA2: as above
                      (((kind != 0x103 && (kind != 0x125)) && // RA2: as above
                        ((kind != 0xe7 && // RA2: as above
                          (((kind != 0x116 && (kind != 0xc2)) && // RA2: as above
                            ((kind != 0xc9 && // RA2: as above
                              ((((kind != 0x11d && (kind != 0x11c)) && (kind != 0x114)) && // RA2: as above
                                ((kind != 700 && (kind != 0x113)))))))))))))))) && // RA2: as above
                  ((kind != 0x109 && ((kind != 0x10f && (kind != 0x117)))))))) && // RA2: as above
              ((kind != 0xe6 && (((kind != 0xf3 && (kind != 0xf4)) && (kind != 0x10e)))))) || // RA2: as above
            (id != 0x694)) && // RA2: as above
          ((((kind != 0xbc && (kind != 0xbd)) && // RA2: as above
              ((kind != 0x102 && ((kind != 0xc2 && (kind != 0xc9)))))) || // RA2: as above
            ((id != 0x6ec && (id != 0x5a8)))))) { // RA2: as above
        if ((kind == 0x103) || (kind == 0xbc7)) { // RA2: as above
            if (id == 0x6f4) { // RA2: as above
                return 0x10; // RA2: as above
            }
            if (id == 0x6f3 || id == 0x69f || id == 0x6f6 || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2 || id == 0x733 || id == 0x735 || id == 0x732 || id == 0x3ea) { // RA2: as above
                return 8; // RA2: as above
            }
            if (id == 0x73c || id == 0x73d || id == 0x73e || id == 0x73f || id == 0x740) { // RA2: as above
                return 0x10; // RA2: as above
            }
            bool wide = id == 0x411 || id == 0x412 || id == 0x419 || id == 0x41a || id == 0x6a3 || id == 0x6a4 || id == 0x6a5 || id == 0x6a7 || id == 0x6a6 || id == 0x6a8 || id == 0x73a || id == 0x739 || id == 0x73b || id == 0x738; // RA2: as above
            if (wide) { // RA2: as above
                return 0x20; // RA2: as above
            }
        }
        if ((kind == 0x108) || (kind == 0xbc6)) { // RA2: as above
            if (((((id == 0x6d2) || (((id == 0x3ea || (id == 0x733)) || (id == 0x71b)))) || // RA2: as above
                      (((id == 0x732 || (id == 0x69d)) || (id == 0x69f)))) || // RA2: as above
                    ((id == 0x69e || (id == 0x6a0)))) || // RA2: as above
                  ((((id == 0x6a1 || // RA2: as above
                        ((((id == 0x5c8 || (id == 0x411)) || (id == 0x412)) || // RA2: as above
                          ((id == 0x413 || (id == 0x414)))))) || // RA2: as above
                      ((id == 0x415 || ((id == 0x416 || (id == 0x417)))))) || (id == 0x418)))) { // RA2: as above
                return (0x20); // RA2: as above
            }
            bool tall = ((((((((id == 0x419) || (id == 0x41a)) || (id == 0x41b)) || // RA2: as above
                          ((((id == 0x41c || (id == 0x41d)) || // RA2: as above
                              ((id == 0x41e || ((id == 0x41f || (id == 0x420)))))) || (id == 0x6a3)))) // RA2: as above
                        || ((((id == 0x6a4 || (id == 0x6a9)) || (id == 0x6ac)) || // RA2: as above
                                ((id == 0x6af || (id == 0x6b2)))))) || // RA2: as above
                      ((((id == 0x6b5 || ((id == 0x6b8 || (id == 0x6a5)))) || (id == 0x6a7)) || // RA2: as above
                        (((((id == 0x6aa || (id == 0x6ad)) || (id == 0x6b0)) || // RA2: as above
                            ((id == 0x6b3 || (id == 0x6b6)))) || (id == 0x6b9)))))) || // RA2: as above
                    ((((id == 0x6a6 || (id == 0x6a8)) || // RA2: as above
                        ((id == 0x6ab || (((id == 0x6ae || (id == 0x6b1)) || (id == 0x6b4)))))) || // RA2: as above
                      (((id == 0x6b7 || (id == 0x6ba)) || // RA2: as above
                        ((id == 0x70a || // RA2: as above
                          (((id == 0x70b || (id == 0x70c)) || // RA2: as above
                            ((id == 0x70d || (((id == 0x70e || (id == 0x70f)) || (id == 0x710))))))))) // RA2: as above
                        ))))) || (id == 0x711)); // RA2: as above
            if (tall) { // RA2: as above
                return (0x40); // RA2: as above
            }
        }
        char special = Kind_Test(kind); // RA2: as above
        if ((special != 0) && (id == 0x695)) { // RA2: as above
            return (0x10); // RA2: as above
        }
    }
    return (8); // RA2: as above
}


/// <summary>
/// Removes a dialog and all of its controls from the owner-draw dictionaries.
/// Use this routine as a subclassed dialog is torn down. Windows is free to hand the same
/// handles out again, and a stale entry would misdirect the next dialog to use them.
/// </summary>
namespace OwnerDraw { // RA2: as above

int __fastcall Control_Typing_Sound(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (((kind == 0x103) || (kind == 0xbc7)) && (id == 0x6f4)) { // RA2: as above
        return Rule->MessageCharTyped; // RA2: as above
    }
    return -1; // RA2: as above
}

}


/// <summary>
/// Removes one window from the owner-draw dictionaries.
/// This is the enumeration callback ODCleanupDicts uses. It drops the window's original
/// procedure, its custom procedure and its owner-draw record.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 1444 bytes differ (adopted text)
bool __fastcall Is_Tested_Control(unsigned int key, HWND window) // RA2: as above
{

    OwnerDraw::WinData *record = 0; // RA2: as above

    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    bool general = ((((((((kind == 0x10c) || (kind == 0x94)) || (kind == 0xa3)) || // RA2: as above
                    ((kind == 0x73 || (kind == 0xd8)))) || (kind == 0xbbb)) || // RA2: as above
                (((kind == 0xf5 || (kind == 0x105)) || // RA2: as above
                  ((kind == 0xe2 || (((kind == 0xd5 || (kind == 0x2b5)) || (kind == 0xb7)))))))) || // RA2: as above
              (((kind == 0x2b4 || (kind == 0xbc7)) || // RA2: as above
                ((kind == 0x101 || // RA2: as above
                  ((((kind == 0x129 || (kind == 0xbc6)) || // RA2: as above
                      ((kind == 0xd7 || (((kind == 0x108 || (kind == 0xbc)) || (kind == 0xbd)))))) || // RA2: as above
                    ((kind == 0xbb || (kind == 0xb5)))))))))) || // RA2: as above
            ((kind == 0xbba || // RA2: as above
              (((kind == 0xff || (kind == 0x6b)) || // RA2: as above
                ((kind == 0x100 || // RA2: as above
                  ((((kind == 0x102 || (kind == 0xb8)) || (kind == 0xd6)) || // RA2: as above
									   ((kind == 0x103 || Is_Listed_Kind(kind)))))))))))) && (id == 0x694) // RA2: as above
          ; // RA2: as above
    if (general) { // RA2: as above
        return true; // RA2: as above
    }
    bool dual_rail = (((kind == 0xbc) || (kind == 0xbd)) || // RA2: as above
            ((kind == 0x102 || ((kind == 0xc2 || (kind == 0xc9)))))) && // RA2: as above
          ((id == 0x6ec || (id == 0x5a8))); // RA2: as above
    if (dual_rail) { // RA2: as above
        return true; // RA2: as above
    }
    bool special_103 = ((kind == 0x103) || (kind == 0xbc7)) && // RA2: as above
          (((((id == 0x6f3 || (((id == 0x6f4 || (id == 0x69f)) || (id == 0x6f6)))) || // RA2: as above
                (((id == 0x6ef || (id == 0x6f1)) || (id == 0x6f0)))) || // RA2: as above
              (((id == 0x6f2 || (id == 0x733)) || // RA2: as above
                ((id == 0x735 || (((id == 0x732 || (id == 0x3ea)) || (id == 0x736)))))))) || // RA2: as above
            (((id == 0x737 || (id == 0x73c)) || // RA2: as above
              ((((id == 0x73d || // RA2: as above
                    (((id == 0x73e || (id == 0x73f)) || // RA2: as above
                      ((id == 0x740 || // RA2: as above
                        ((((id == 0x411 || (id == 0x412)) || (id == 0x419)) || // RA2: as above
                          ((id == 0x41a || (id == 0x6a3)))))))))) || (id == 0x6a4)) || // RA2: as above
                (((id == 0x6a5 || (id == 0x6a7)) || // RA2: as above
                  (((id == 0x6a6 || (((id == 0x6a8 || (id == 0x73a)) || (id == 0x739)))) || // RA2: as above
                    ((id == 0x73b || (id == 0x738))))))))))))); // RA2: as above
    if (special_103) { // RA2: as above
        return true; // RA2: as above
    }
    bool special_108 = ((kind == 0x108) || (kind == 0xbc6)) && // RA2: as above
          (((((id == 0x6d2 || (((id == 0x3ea || (id == 0x69d)) || (id == 0x69f)))) || // RA2: as above
                ((((id == 0x69e || (id == 0x6a0)) || (id == 0x6a1)) || // RA2: as above
                  ((id == 0x5c8 || (id == 0x411)))))) || // RA2: as above
              ((id == 0x412 || // RA2: as above
                ((((((id == 0x413 || (id == 0x414)) || (id == 0x415)) || // RA2: as above
                      ((id == 0x416 || (id == 0x417)))) || (id == 0x418)) || // RA2: as above
                  ((id == 0x419 || (id == 0x41a)))))))) || // RA2: as above
            (((((id == 0x41b || (((id == 0x41c || (id == 0x41d)) || (id == 0x41e)))) || // RA2: as above
                  ((((id == 0x41f || (id == 0x420)) || // RA2: as above
                      ((((id == 0x6a3 || ((id == 0x6a4 || (id == 0x6a9)))) || (id == 0x6ac)) || // RA2: as above
                        (((id == 0x6af || (id == 0x6b2)) || (id == 0x6b5)))))) || // RA2: as above
                    ((((id == 0x6b8 || (id == 0x6a5)) || // RA2: as above
                        ((id == 0x6a7 || ((id == 0x6aa || (id == 0x6ad)))))) || (id == 0x6b0)))))) // RA2: as above
                || (((((id == 0x6b3 || (id == 0x6b6)) || (id == 0x6b9)) || // RA2: as above
                          ((id == 0x6a6 || (id == 0x6a8)))) || // RA2: as above
                        (((id == 0x6ab || ((id == 0x6ae || (id == 0x6b1)))) || // RA2: as above
                          ((id == 0x6b4 || // RA2: as above
                            ((((((id == 0x6b7 || (id == 0x6ba)) || (id == 0x70a)) || // RA2: as above
                                  ((id == 0x70b || (id == 0x70c)))) || // RA2: as above
                                ((id == 0x70d || ((id == 0x70e || (id == 0x70f)))))) || (id == 0x710)))) // RA2: as above
                          )))))) || // RA2: as above
              ((((id == 0x711 || (id == 0x733)) || (id == 0x732)) || // RA2: as above
                ((id == 0x71b || (id == 0x735))))))))); // RA2: as above
    if (special_108) { // RA2: as above
        return true; // RA2: as above
    }
    char special = Kind_Test(kind); // RA2: as above
    if (((special != 0) && (id == 0x695)) && Session.Is_In_Game() == 0) { // RA2: as above
        return true; // RA2: as above
    }
    return false; // RA2: as above
}
#endif


bool __fastcall Is_Listed_Kind(int kind) // RA2: as above
{
	return kind == 0x125 ||
		kind == 0xe7 ||
		kind == 0x116 ||
		kind == 0xc2 ||
		kind == 0xc9 ||
		kind == 0x11d ||
		kind == 0x11c ||
		kind == 0x114 ||
		kind == 0x2bc ||
		kind == 0x113 ||
		kind == 0x109 ||
		kind == 0x10f ||
		kind == 0x117 ||
		kind == 0xe6 ||
		kind == 0xf3 ||
		kind == 0xf4 ||
		kind == 0x10e;
}


/// <summary>
/// Starts displaying a tooltip over the given area.
/// Any tooltip already on screen is taken down first, since there is only ever one. The
/// area underneath is saved so the tooltip can be hidden again without the dialog having
/// to repaint itself.
/// </summary>
/// <param name="rect">The area of the screen the tooltip is to occupy.</param>
/// <param name="text">The text to display, or NULL for the placeholder text.</param>
/// <param name="window">The control the tooltip belongs to.</param>
/// <returns>bool; Was the tooltip displayed?</returns>
namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Image_Control(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (((((((kind != 0xbc) && (kind != 0xbd)) && (kind != 0xc2)) && (kind != 0xc9)) || // RA2: as above
             ((((id != 0x6d2 && (id != 0x6d3)) && // RA2: as above
                 ((id != 0x6d4 && ((id != 0x6d5 && (id != 0x6d6)))))) && // RA2: as above
                ((id != 0x6d7 && ((id != 0x6d8 && (id != 0x6d9)))))))) && // RA2: as above
            ((((kind != 0x102 && (((kind != 0xbc && (kind != 0xbd)) && (kind != 0xc2)))) && // RA2: as above
                (kind != 0xc9)) || // RA2: as above
             (((((id != 0x6da && (id != 0x6db)) && // RA2: as above
                    ((id != 0x6dc && ((id != 0x6dd && (id != 0x6de)))))) && (id != 0x6df)) && // RA2: as above
                ((id != 0x6e0 && (id != 0x6e1)))))))) && // RA2: as above
         (((kind != 0xc2 && (kind != 0xc9)) || // RA2: as above
            (((((id != 0x754 && (id != 0x755)) && (id != 0x756)) && // RA2: as above
                (((id != 0x757 && (id != 0x758)) && (id != 0x759)))) && // RA2: as above
             ((id != 0x75a && (id != 0x75b)))))))) { // RA2: as above
        return false; // RA2: as above
    }
    return true; // RA2: as above
}

}


/// <summary>
/// Draws the tooltip onto the visible surface.
/// Use this routine to bring the tooltip back after a repaint has forced it into hiding.
/// The background is only captured when the caller asks for it, since redrawing the
/// tooltip must not save the tooltip as its own background.
/// </summary>
/// <param name="save_background">Should the area under the tooltip be captured first?</param>
/// <returns>bool; Was the tooltip drawn?</returns>
namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Image_Mode3_Control(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    return false; // RA2: as above
}

}


/// <summary>
/// Hides the tooltip by restoring the area it covered.
/// The tooltip stays active while hidden, so it can be put back with Show_Tooltip once
/// whatever prompted the hide has finished painting.
/// </summary>
/// <returns>bool; Was the tooltip hidden?</returns>
namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Animated_Control(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) { // RA2: as above
        return true; // RA2: as above
    }
    if (kind == 0x94) { // RA2: as above
        if (((id == 0x6ea) || (id == 0x6ec)) || (id == 0x6eb)) { // RA2: as above
            return true; // RA2: as above
        }
    }
    else if (kind == 0x103) { // RA2: as above
        if (id == 0x72b) { // RA2: as above
            return true; // RA2: as above
        }
    }
    else if ((kind == 0xbc7) && (id == 0x72b)) { // RA2: as above
        return true; // RA2: as above
    }
    return false; // RA2: as above
}

}


/// <summary>
/// Takes the tooltip down for good.
/// The area it covered is restored and the saved background released. Use this routine
/// when the mouse leaves the control, or when the control itself is going away.
/// </summary>
/// <returns>bool; Was there a tooltip to take down?</returns>
namespace OwnerDraw { // RA2: as above

int __fastcall Control_Frame_Delay(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    return 0xc; // RA2: as above
}

}


/// <summary>
/// Handles every message sent to a subclassed control.
/// This is the procedure that displaces the stock Windows procedure of each control, and
/// it is the heart of the owner-draw system. It enforces the modal window stack, guards
/// against a message re-entering the same control, drives the tooltip and hover timers,
/// accumulates the area that has to reach the screen, and then hands the message to the
/// control's own custom procedure -- which is usually the one that calls the original
/// Windows procedure.
/// </summary>
/// <returns>Returns with the result of the control's custom procedure, or zero when the
/// message was swallowed here.</returns>
namespace OwnerDraw { // RA2: as above

int __fastcall Control_Sound_Slot(HWND dialog, HWND window) // RA2: as above
{
    WinData *record = Get_Record(dialog); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) { // RA2: as above
        return 0x25; // RA2: as above
    }
    if (((((kind == 0xbc) || (kind == 0xbd)) || (kind == 0xc2)) || // RA2: as above
            ((kind == 0xc9 || (kind == 0x102)))) && ((id == 0x6ec || (id == 0x5a8)))) { // RA2: as above
        return 0xe; // RA2: as above
    }
    return 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

int __fastcall Control_Animation_Loops(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) { // RA2: as above
        return 1; // RA2: as above
    }
    return 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

int __fastcall Control_Animation_Rate(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) { // RA2: as above
        return 100; // RA2: as above
    }
    if (kind == 0x94) { // RA2: as above
        if (id == 0x6ea || id == 0x6ec || id == 0x6eb) { // RA2: as above
            return 100; // RA2: as above
        }
    }
    else if (kind == 0x103) { // RA2: as above
        if (id == 0x72b) { // RA2: as above
            return 100; // RA2: as above
        }
    }
    else if (kind == 0xbc7 && id == 0x72b) { // RA2: as above
        return 100; // RA2: as above
    }
    return 0; // RA2: as above
}

}


void *__fastcall Get_Drawer(); // RA2: as above
void *__fastcall Get_Listed_Drawer(); // RA2: as above
void *Get_Slot_Art(); // RA2: as above
void *Get_Chat_Art(); // RA2: as above

namespace OwnerDraw { // RA2: as above

void *__fastcall Control_Animation_Art(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    void *art = Get_Drawer(); // RA2: as above
    if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) { // RA2: as above
        art = Get_Listed_Drawer(); // RA2: as above
    }
    if (kind == 0x94) { // RA2: as above
        if (id == 0x6ea || id == 0x6eb || id == 0x6ec) { // RA2: as above
            return Get_Slot_Art(); // RA2: as above
        }
    } else if (kind == 0x103) { // RA2: as above
        if (id == 0x72b) { // RA2: as above
            return Get_Chat_Art(); // RA2: as above
        }
    } else if (kind == 0xbc7 && id == 0x72b) { // RA2: as above
        return Get_Chat_Art(); // RA2: as above
    }
    return art; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

int __fastcall Control_Image(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    return 0; // RA2: as above
}

}



namespace OwnerDraw { // RA2: as above

void const *__fastcall Control_Animation_Shape(unsigned int key, HWND window, bool &allocated) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) { // RA2: as above
        return Load_Alloc_Data(Listed_Kind_Art_Name, allocated); // RA2: as above
    }
    if (kind == 0x94) { // RA2: as above
        if (id == 0x6ea) { // RA2: as above
            return Art_Side_Animation; // RA2: as above
        }
        if (id == 0x6eb) { // RA2: as above
            return Art_Button_Corner; // RA2: as above
        }
        if (id == 0x6ec) { // RA2: as above
            return Art_Side_Strip; // RA2: as above
        }
    } else if ((kind == 0x103 || kind == 0xbc7) && id == 0x72b) { // RA2: as above
        return Art_Score_Frame; // RA2: as above
    }
    return 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

inline bool __fastcall Is_Text_Control(HWND control) // RA2: as above
{
    unsigned int key = (unsigned int)control; // RA2: as above
    char name[128]; // RA2: as above
    GetClassNameA(control, name, 128); // RA2: as above
    if (strcmp(name, "Edit") == 0) { // RA2: as above
        return true; // RA2: as above
    }
    if (strcmp(name, "NewEdit") == 0) { // RA2: as above
        return true; // RA2: as above
    }
    if (strcmp(name, "ListBox") == 0) { // RA2: as above
        WinData *record = 0; // RA2: as above
        if (Control_Table.getPointer(key, &record)) { // RA2: as above
            return record->SkipTests == 1; // RA2: as above
        }
    }
    if (strcmp(name, "ComboBox") == 0) { // RA2: as above
        return (GetWindowLongA((HWND)key, GWL_STYLE) & 3) != 3; // RA2: as above
    }
    return false; // RA2: as above
}

int __fastcall Control_Text_Style(HWND dialog, HWND window) // RA2: as above
{
    WinData *record = Get_Record(dialog); // RA2: as above
    if (record != 0) { // RA2: as above
        int kind = record->Kind; // RA2: as above
        if (kind == 0x10d || kind == 0xd9 || kind == 0xf0 || kind == 0xce || kind == 0x120 || // RA2: as above
            kind == 0x121 || kind == 0x115 || kind == 0xd3 || kind == 0xcf || kind == 0x11f || // RA2: as above
            kind == 0xc3 || kind == 0x11b || kind == 0xe1 || kind == 0x11e || kind == 0xc4 || // RA2: as above
            kind == 300 || kind == 0xd0 || kind == 0xfc || kind == 0x126) { // RA2: as above
            record->ButtonKind = 2; // RA2: as above
            if (Is_Text_Control(window)) { // RA2: as above
                return 0xc0; // RA2: as above
            }
        }
    }
    return 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Control_Image(HWND control, Surface *image) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->PictureMode = 2; // RA2: as above
        record->Image = image; // RA2: as above
        InvalidateRect(control, 0, TRUE); // RA2: as above
    }
}

}


void __fastcall Send_Control_Value(HWND window, unsigned char value) // RA2: as above
{
    SendMessageA(window, 0x4e5, 0, value); // RA2: as above
}


void __fastcall Send_Control_Flag(HWND window, unsigned char flag) // RA2: as above
{
    SendMessageA(window, 0x4e6, 0, (int)(flag & 0xff)); // RA2: as above
}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Control_Checked(HWND window) // RA2: as above
{
    return SendMessageA(window, 0x4e7, 0, 0) ? true : false; // RA2: as above
}

}


void __fastcall Send_Control_Pair(HWND window, int low, int high) // RA2: as above
{
    SendMessageA(window, 0x4e8, 0, (LPARAM)((unsigned short)low | ((unsigned long)(unsigned short)high << 16))); // RA2: as above
}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Control_Result(HWND control, int value) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->Result = value; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

int __fastcall Get_Control_Result(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        return record->Result; // RA2: as above
    }
    return -1; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

WideWstring __fastcall Get_Item_Tip_Text(HWND window, int a, int b) // RA2: as above
{
    TextRequest request; // RA2: as above
    request.Item = a; // RA2: as above
    request.Index = b; // RA2: as above
    SendMessageA(window, 0x4e9, 0, (LPARAM)&request); // RA2: as above
    return request.Text; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

WideWstring __fastcall Get_Control_Tip_Text(HWND window, int a) // RA2: as above
{
    TextRequest request; // RA2: as above
    request.Item = a; // RA2: as above
    request.Index = -1; // RA2: as above
    SendMessageA(window, 0x4e9, 0, (LPARAM)&request); // RA2: as above
    return request.Text; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

WideWstring __fastcall Get_Item_Tip_Text_At(HWND window, HWND item, int x, int y) // RA2: as above
{
    int index = SendMessageA(item, 0x4e8, 0, (unsigned short)x | ((unsigned long)(unsigned short)y << 16)); // RA2: as above
    WideWstring text = Get_Item_Tip_Text(window, (int)item, index); // RA2: as above
    return text; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

WideWstring __fastcall Control_Tooltip(unsigned int key, HWND window) // RA2: as above
{
    char const *label = Control_Tooltip_Label(key, window); // RA2: as above
    if (label) { // RA2: as above
        return WideWstring(StringTable::LoadString(label, 0, "D:\\Ra2\\ownrdraw.cpp", 1902)); // RA2: as above
    }
    return WideWstring(L""); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

inline int __cdecl Record_Kind(unsigned int key) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    return record != 0 ? record->Kind : 0; // RA2: as above
}
char const *__fastcall Control_Tooltip_Label(unsigned int key, HWND window) // RA2: as above
{
    int kind = Record_Kind(key); // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (kind == 0xe2) { // RA2: as above
        if (id == 0x683) { // RA2: as above
            return "STT:MainButtonSinglePlayer"; // RA2: as above
        }
        if (id == 0x684) { // RA2: as above
            return "STT:MainButtonWWOnline"; // RA2: as above
        }
        if (id == 0x578) { // RA2: as above
            return "STT:MainButtonNetwork"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:MainButtonMovies"; // RA2: as above
        }
        if (id == 0x55c) { // RA2: as above
            return "STT:MainButtonOptions"; // RA2: as above
        }
        if (id == 0x3ee) { // RA2: as above
            return "STT:MainButtonExitGame"; // RA2: as above
        }
    } else if (kind == 0x100) { // RA2: as above
        if (id == 0x688) { // RA2: as above
            return "STT:SingleButtonNewCampaign"; // RA2: as above
        }
        if (id == 0x689) { // RA2: as above
            return "STT:SingleButtonLoadSavedGame"; // RA2: as above
        }
        if (id == 0x579) { // RA2: as above
            return "STT:SingleButtonSkirmish"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:SingleButtonBack"; // RA2: as above
        }
    } else if (kind == 0x94) { // RA2: as above
        if (id == 0x6ea) { // RA2: as above
            return "STT:CampaignAnimAllied"; // RA2: as above
        }
        if (id == 0x6eb) { // RA2: as above
            return "STT:CampaignAnimTutorial"; // RA2: as above
        }
        if (id == 0x6ec) { // RA2: as above
            return "STT:CampaignAnimSoviet"; // RA2: as above
        }
        if (id == 0x50f) { // RA2: as above
            return "STT:CampaignSliderDifficulty"; // RA2: as above
        }
        if (id == 0x40e) { // RA2: as above
            return "STT:CampaignButtonLoad"; // RA2: as above
        }
        if (id == 0x455) { // RA2: as above
            return "STT:CampaignList"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:CampaignButtonBack"; // RA2: as above
        }
    } else if (kind == 0xb7) { // RA2: as above
        if (id == 0x525) { // RA2: as above
            return "STT:LoadList"; // RA2: as above
        }
        if (id == 0x40f) { // RA2: as above
            return "STT:LoadButtonLoad"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:LoadButtonBack"; // RA2: as above
        }
    } else if (kind == 0x102) { // RA2: as above
        if (id == 0x6a0) { // RA2: as above
            return "STT:SkirmishEditPlayer"; // RA2: as above
        }
        if ((((id == 0x50b) || (id == 0x50e)) || (id == 0x516)) || (((id == 0x51a || (id == 0x51b)) || ((id == 0x51c || (id == 0x51d)))))) { // RA2: as above
            return "STT:SkirmishComboAIPlayer"; // RA2: as above
        }
        if (((id == 0x6da) || (id == 0x6db)) || (((id == 0x6dc || (((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)))) || ((id == 0x6e0 || (id == 0x6e1)))))) { // RA2: as above
            return "STT:SkirmishPictureFlag"; // RA2: as above
        }
        if (((id == 0x6a1) || (id == 0x510)) || ((id == 0x513 || ((((id == 0x51e || (id == 0x514)) || (id == 0x51f)) || ((id == 0x520 || (id == 0x521)))))))) { // RA2: as above
            return "STT:SkirmishComboCountry"; // RA2: as above
        }
        if (((id == 0x6a2) || (id == 0x522)) || (((id == 0x523 || (((id == 0x524 || (id == 0x525)) || (id == 0x526)))) || ((id == 0x527 || (id == 0x528)))))) { // RA2: as above
            return "STT:SkirmishComboColor"; // RA2: as above
        }
        if (id == 0x529) { // RA2: as above
            return "STT:SkirmishSliderSpeed"; // RA2: as above
        }
        if (id == 0x511) { // RA2: as above
            return "STT:SkirmishSliderCredits"; // RA2: as above
        }
        if (id == 0x50c) { // RA2: as above
            return "STT:SkirmishSliderUnit"; // RA2: as above
        }
        if (id == 0x5aa) { // RA2: as above
            return "STT:SkirmishButtonChooseMap"; // RA2: as above
        }
        if (id == 0x693) { // RA2: as above
            return "STT:SkirmishCBoxRedeploys"; // RA2: as above
        }
        if (id == 0x54e) { // RA2: as above
            return "STT:SkirmishCBoxShortGame"; // RA2: as above
        }
        if (id == 0x69a) { // RA2: as above
            return "STT:SkirmishCBoxSWAllowed"; // RA2: as above
        }
        if (id == 0x69d) { // RA2: as above
            return "STT:SkirmishCBoxBuildOffAlly"; // RA2: as above
        }
        if (id == 0x696) { // RA2: as above
            return "STT:SkirmishCBoxCrates"; // RA2: as above
        }
        if (id == 0x468) { // RA2: as above
            return "STT:SkirmishMapThumbnail"; // RA2: as above
        }
        if (id == 0x6ec) { // RA2: as above
            return "STT:SkirmishLabelGameType"; // RA2: as above
        }
        if (id == 0x5a8) { // RA2: as above
            return "STT:SkirmishLabelScenario"; // RA2: as above
        }
        if (id == 0x617) { // RA2: as above
            return "STT:SkirmishButtonStartGame"; // RA2: as above
        }
        if (id == 0x5c0) { // RA2: as above
            return "STT:SkirmishButtonBack"; // RA2: as above
        }
    } else if (kind == 0xbb) { // RA2: as above
        if (id == 0x541) { // RA2: as above
            return "STT:LobbyEditYourName"; // RA2: as above
        }
        if (id == 0x53e) { // RA2: as above
            return "STT:LobbyListUsers"; // RA2: as above
        }
        if (id == 0x549) { // RA2: as above
            return "STT:LobbyListGames"; // RA2: as above
        }
        if (id == 0x53f) { // RA2: as above
            return "STT:LobbyEditOutput"; // RA2: as above
        }
        if (id == 0x540) { // RA2: as above
            return "STT:LobbyEditInput"; // RA2: as above
        }
        if (id == 0x6cc) { // RA2: as above
            return "STT:LobbyButtonNew"; // RA2: as above
        }
        if (id == 0x6cd) { // RA2: as above
            if (ObserverMode == 0) { // RA2: as above
                return "STT:LobbyButtonJoin"; // RA2: as above
            }
            return "STT:LobbyButtonObserve"; // RA2: as above
        }
        if (id == 0x760) { // RA2: as above
            return "STT:ObserverToggle"; // RA2: as above
        }
        if (id == 0x6ce) { // RA2: as above
            return "STT:LobbyButtonObserve"; // RA2: as above
        }
        if (id == 0x5c0) { // RA2: as above
            return "STT:LobbyButtonBack"; // RA2: as above
        }
    } else if (kind == 0xbc || kind == 0xc2) { // RA2: as above
        if ((((id == 0x6d2) || (((id == 0x6d3 || (id == 0x6d4)) || (id == 0x6d5)))) || (((id == 0x6d6 || (id == 0x6d7)) || (id == 0x6d8)))) || (id == 0x6d9)) { // RA2: as above
            return "STT:HostPictureAcceptance"; // RA2: as above
        }
        if (((id == 0x411) || (id == 0x412)) || ((id == 0x413 || ((((id == 0x414 || (id == 0x415)) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))))))) { // RA2: as above
            return "STT:HostComboPlayer"; // RA2: as above
        }
        if (((id == 0x6da) || (id == 0x6db)) || (((id == 0x6dc || (((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)))) || ((id == 0x6e0 || (id == 0x6e1)))))) { // RA2: as above
            return "STT:HostPictureFlag"; // RA2: as above
        }
        if ((((id == 0x6a1) || (id == 0x510)) || ((id == 0x513 || (((id == 0x51e || (id == 0x514)) || (id == 0x51f)))))) || ((id == 0x520 || (id == 0x521)))) { // RA2: as above
            return "STT:HostComboCountry"; // RA2: as above
        }
        if (((id == 0x6a2) || (id == 0x522)) || ((id == 0x523 || ((((id == 0x524 || (id == 0x525)) || (id == 0x526)) || ((id == 0x527 || (id == 0x528)))))))) { // RA2: as above
            return "STT:HostComboColor"; // RA2: as above
        }
        if (id == 0x529) { // RA2: as above
            return "STT:HostSliderSpeed"; // RA2: as above
        }
        if (id == 0x402) { // RA2: as above
            return "STT:HostSliderCredits"; // RA2: as above
        }
        if (id == 0x54f) { // RA2: as above
            return "STT:HostSliderUnit"; // RA2: as above
        }
        if (id == 1000) { // RA2: as above
            return "STT:HostCBoxRedeploys"; // RA2: as above
        }
        if (id == 0x54e) { // RA2: as above
            return "STT:HostCBoxShortGame"; // RA2: as above
        }
        if (id == 0x69a) { // RA2: as above
            return "STT:HostCBoxSWAllowed"; // RA2: as above
        }
        if (id == 0x69d) { // RA2: as above
            return "STT:HostCBoxBuildOffAlly"; // RA2: as above
        }
        if (id == 0x54d) { // RA2: as above
            return "STT:HostCBoxCrates"; // RA2: as above
        }
        if (id == 0x53f) { // RA2: as above
            return "STT:HostEditOutput"; // RA2: as above
        }
        if (id == 0x540) { // RA2: as above
            return "STT:HostEditInput"; // RA2: as above
        }
        if (id == 0x468) { // RA2: as above
            return "STT:HostMapThumbnail"; // RA2: as above
        }
        if (id == 0x5a8) { // RA2: as above
            return "STT:HostLabelScenario"; // RA2: as above
        }
        if (id == 0x588) { // RA2: as above
            return "STT:HostButtonGo"; // RA2: as above
        }
        if (id == 0x5aa) { // RA2: as above
            return "STT:HostButtonChooseMap"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            if (kind == 0xc2) { // RA2: as above
                return "STT:WOLNewGameCancel"; // RA2: as above
            }
            return "STT:HostButtonBack"; // RA2: as above
        }
    } else if (kind == 0xbd || kind == 0xc9) { // RA2: as above
        if (((id == 0x6d2) || (((id == 0x6d3 || (id == 0x6d4)) || (id == 0x6d5)))) || ((((id == 0x6d6 || (id == 0x6d7)) || (id == 0x6d8)) || (id == 0x6d9)))) { // RA2: as above
            return "STT:GuestPictureAcceptance"; // RA2: as above
        }
        if ((((id == 0x411) || (id == 0x412)) || ((id == 0x413 || (((id == 0x414 || (id == 0x415)) || (id == 0x416)))))) || ((id == 0x417 || (id == 0x418)))) { // RA2: as above
            return "STT:GuestComboPlayer"; // RA2: as above
        }
        if (((id == 0x6da) || (id == 0x6db)) || ((id == 0x6dc || ((((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)) || ((id == 0x6e0 || (id == 0x6e1)))))))) { // RA2: as above
            return "STT:GuestPictureFlag"; // RA2: as above
        }
        if (((id == 0x6a1) || (id == 0x510)) || (((id == 0x513 || (((id == 0x51e || (id == 0x514)) || (id == 0x51f)))) || ((id == 0x520 || (id == 0x521)))))) { // RA2: as above
            return "STT:GuestComboCountry"; // RA2: as above
        }
        if ((((id == 0x6a2) || (id == 0x522)) || ((id == 0x523 || (((id == 0x524 || (id == 0x525)) || (id == 0x526)))))) || ((id == 0x527 || (id == 0x528)))) { // RA2: as above
            return "STT:GuestComboColor"; // RA2: as above
        }
        if (id == 0x529) { // RA2: as above
            return "STT:GuestSliderSpeed"; // RA2: as above
        }
        if (id == 0x402) { // RA2: as above
            return "STT:GuestSliderCredits"; // RA2: as above
        }
        if (id == 0x54f) { // RA2: as above
            return "STT:GuestSliderUnit"; // RA2: as above
        }
        if (id == 1000) { // RA2: as above
            return "STT:GuestCBoxRedeploys"; // RA2: as above
        }
        if (id == 0x54e) { // RA2: as above
            return "STT:GuestCBoxShortGame"; // RA2: as above
        }
        if (id == 0x69a) { // RA2: as above
            return "STT:GuestCBoxSWAllowed"; // RA2: as above
        }
        if (id == 0x69d) { // RA2: as above
            return "STT:GuestCBoxBuildOffAlly"; // RA2: as above
        }
        if (id == 0x54d) { // RA2: as above
            return "STT:GuestCBoxCrates"; // RA2: as above
        }
        if (id == 0x53f) { // RA2: as above
            return "STT:GuestEditOutput"; // RA2: as above
        }
        if (id == 0x540) { // RA2: as above
            return "STT:GuestEditInput"; // RA2: as above
        }
        if (id == 0x468) { // RA2: as above
            return "STT:GuestMapThumbnail"; // RA2: as above
        }
        if (id == 0x5a8) { // RA2: as above
            return "STT:GuestLabelScenario"; // RA2: as above
        }
        if (id == 0x59f) { // RA2: as above
            return "STT:GuestButtonAccept"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            if (kind == 0xc9) { // RA2: as above
                return "STT:WOLNewGameCancel"; // RA2: as above
            }
            return "STT:GuestButtonBack"; // RA2: as above
        }
    } else if (kind == 0x101) { // RA2: as above
        if (id == 0x68d) { // RA2: as above
            return "STT:OptionsButtonSneak"; // RA2: as above
        }
        if (id == 0x68e) { // RA2: as above
            return "STT:OptionsButtonMovies"; // RA2: as above
        }
        if (id == 0x68f) { // RA2: as above
            return "STT:OptionsButtonCredits"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:OptionsButtonBack"; // RA2: as above
        }
    } else if (kind == 0x129) { // RA2: as above
        if (id == 0x745) { // RA2: as above
            return "GUI:PlayMovie"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:MsnDltButtonBack"; // RA2: as above
        }
        if (id == 0x744) { // RA2: as above
            return "GUI:SelectMovie"; // RA2: as above
        }
    } else if (kind == 0xd5) { // RA2: as above
        if (id == 0x5ce) { // RA2: as above
            return "STT:MainOptButtonKeyboard"; // RA2: as above
        }
        if (id == 0x5cd) { // RA2: as above
            return "STT:MainOptButtonNetwork"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:MainOptButtonBack"; // RA2: as above
        }
        if (id == 0x674) { // RA2: as above
            return "STT:MainOptCBoxStretch"; // RA2: as above
        }
        if (id == 0x52b) { // RA2: as above
            return "STT:MainOptSliderVisual"; // RA2: as above
        }
        if (id == 0x6ed) { // RA2: as above
            return "STT:MainOptComboModes"; // RA2: as above
        }
        if (id == 0x529) { // RA2: as above
            return "STT:MainOptSliderSpeed"; // RA2: as above
        }
        if (id == 0x50f) { // RA2: as above
            return "STT:MainOptSliderDifficulty"; // RA2: as above
        }
        if (id == 0x601) { // RA2: as above
            return "STT:MainOptCBoxTargetLines"; // RA2: as above
        }
        if (id == 0x604) { // RA2: as above
            return "STT:MainOptCBoxHidden"; // RA2: as above
        }
        if (id == 0x602) { // RA2: as above
            return "STT:MainOptCBoxTooltips"; // RA2: as above
        }
        if (id == 0x52a) { // RA2: as above
            return "STT:MainOptSliderScroll"; // RA2: as above
        }
        if (id == 0x51a) { // RA2: as above
            return "STT:MainOptCBoxScrollCoasting"; // RA2: as above
        }
        if (id == 0x52f) { // RA2: as above
            return "STT:MainOptSliderMusic"; // RA2: as above
        }
        if (id == 0x532) { // RA2: as above
            return "STT:MainOptSliderSound"; // RA2: as above
        }
        if (id == 0x536) { // RA2: as above
            return "STT:MainOptSliderVoice"; // RA2: as above
        }
    } else if (kind == 0xd7) { // RA2: as above
        if (id == 0x5d4) { // RA2: as above
            return "STT:MultiOptComboCard"; // RA2: as above
        }
        if (id == 0x5d1) { // RA2: as above
            return "STT:MultiOptEditSocketNum"; // RA2: as above
        }
        if (id == 0x5d3) { // RA2: as above
            return "STT:MultiOptEditDestAddr"; // RA2: as above
        }
        if (id == 0x762) { // RA2: as above
            return "STT:MultiOptSendDelay"; // RA2: as above
        }
        if (id == 0x5d7) { // RA2: as above
            return "STT:MultiOptPortNumber"; // RA2: as above
        }
        if (id == 1) { // RA2: as above
            return "STT:MultiOptButtonOk"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:MultiOptButtonCancel"; // RA2: as above
        }
    } else if (kind == 0xa3) { // RA2: as above
        if (id == 0x4c7) { // RA2: as above
            return "STT:KeyboardComboCategory"; // RA2: as above
        }
        if (id == 0x6e9) { // RA2: as above
            return "STT:KeyboardGroupDescription"; // RA2: as above
        }
        if (id == 0x4c8) { // RA2: as above
            return "STT:KeyboardListCommands"; // RA2: as above
        }
        if (id == 0x63c) { // RA2: as above
            return "STT:KeyboardLabelShortcut"; // RA2: as above
        }
        if (id == 0x63d) { // RA2: as above
            return "STT:KeyboardLabelError"; // RA2: as above
        }
        if (id == 0x4cd) { // RA2: as above
            return "STT:KeyboardEditEntry"; // RA2: as above
        }
        if (id == 0x4cf) { // RA2: as above
            return "STT:KeyboardButtonAssign"; // RA2: as above
        }
        if (id == 0x63b) { // RA2: as above
            return "STT:KeyboardLabelAssigned"; // RA2: as above
        }
        if (id == 0x4d0) { // RA2: as above
            return "STT:KeyboardButtonResetAll"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:KeyboardButtonBack"; // RA2: as above
        }
    } else if (kind == 0x10c) { // RA2: as above
        if (id == 0x6e7) { // RA2: as above
            return "STT:BriefingLabelContent"; // RA2: as above
        }
        if (id == 0x423) { // RA2: as above
            return "STT:BriefingButtonReturn"; // RA2: as above
        }
    } else if (kind == 0xb6) { // RA2: as above
        if (id == 0x6c9) { // RA2: as above
            int state = Get_Control_Result(window); // RA2: as above
            if (state == 3) { // RA2: as above
                return "STT:ConfirmExitButtonLeaveEx"; // RA2: as above
            }
            if (state == 2) { // RA2: as above
                return "STT:ConfirmExitButtonLeave"; // RA2: as above
            }
            return "STT:Empty"; // RA2: as above
        }
        if (id == 0x712) { // RA2: as above
            int state = Get_Control_Result(window); // RA2: as above
            if (state == 4) { // RA2: as above
                return "STT:ConfirmExitButtonObserve"; // RA2: as above
            }
            if (state == 5) { // RA2: as above
                return "STT:ConfirmExitButtonRestart"; // RA2: as above
            }
            return "STT:Empty"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:ConfirmExitButtonResume"; // RA2: as above
        }
    } else if (kind == 0xd9) { // RA2: as above
        if (id == 0x5c1) { // RA2: as above
            return "STT:ConfirmModeButtonOk"; // RA2: as above
        }
        if (id == 0x6ca) { // RA2: as above
            return "STT:ConfirmModeButtonCancel"; // RA2: as above
        }
    } else if (kind == 0x73) { // RA2: as above
        if ((((((id == 0x411) || (id == 0x412)) || (id == 0x413)) || ((id == 0x414 || (id == 0x415)))) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))) { // RA2: as above
            return "STT:DiplomacyLabelPlayer"; // RA2: as above
        }
        if ((((id == 0x6cf) || (id == 0x6d0)) || (id == 0x6d1)) || (((id == 0x6d2 || (id == 0x6d3)) || ((id == 0x6d4 || (id == 0x6d5)))))) { // RA2: as above
            return "STT:DiplomacyCBoxAllies"; // RA2: as above
        }
        if ((((id == 0x6d6) || (id == 0x6d7)) || ((id == 0x6d8 || (((id == 0x6d9 || (id == 0x6e2)) || (id == 0x6e3)))))) || (id == 0x6e4)) { // RA2: as above
            return "STT:DiplomacyCBoxListeners"; // RA2: as above
        }
        if ((((id == 0x419) || (id == 0x41a)) || ((id == 0x41b || ((id == 0x41c || (id == 0x41d)))))) || ((id == 0x41e || ((id == 0x41f || (id == 0x420)))))) { // RA2: as above
            return "STT:DiplomacyCBoxKills"; // RA2: as above
        }
        if (id == 0x423) { // RA2: as above
            return "STT:DiplomacyButtonReturn"; // RA2: as above
        }
        if (id == 0x730) { // RA2: as above
            return "STT:TauntsOn"; // RA2: as above
        }
    } else if (kind == 0xbbb) { // RA2: as above
        if (id == 0x529) { // RA2: as above
            return "STT:IGGameOptSliderSpeed"; // RA2: as above
        }
        if (id == 0x52a) { // RA2: as above
            return "STT:IGGameOptSliderScroll"; // RA2: as above
        }
        if (id == 0x52b) { // RA2: as above
            return "STT:IGGameOptSliderVisual"; // RA2: as above
        }
        if (id == 0x602) { // RA2: as above
            return "STT:IGGameOptCBoxTooltips"; // RA2: as above
        }
        if (id == 0x601) { // RA2: as above
            return "STT:IGGameOptCBoxTargetLines"; // RA2: as above
        }
        if (id == 0x604) { // RA2: as above
            return "STT:IGGameOptCBoxHidden"; // RA2: as above
        }
        if (id == 0x51a) { // RA2: as above
            return "STT:IGGameOptCBoxScrollCoast"; // RA2: as above
        }
        if (id == 0x52d) { // RA2: as above
            return "STT:IGGameOptButtonSound"; // RA2: as above
        }
        if (id == 0x52c) { // RA2: as above
            return "STT:IGGameOptButtonKeyboard"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:IGGameOptButtonBack"; // RA2: as above
        }
    } else if (kind == 0x105) { // RA2: as above
        if (id == 0x405) { // RA2: as above
            return "STT:GenerateCBoxEnvironment"; // RA2: as above
        }
        if (id == 0x3ea) { // RA2: as above
            return "STT:GenerateCBoxTime"; // RA2: as above
        }
        if (id == 0x407) { // RA2: as above
            return "STT:GenerateCBoxTheater"; // RA2: as above
        }
        if (id == 0x406) { // RA2: as above
            return "STT:GenerateCBoxMapSize"; // RA2: as above
        }
        if (id == 0x408) { // RA2: as above
            return "STT:GenerateCBoxResources"; // RA2: as above
        }
        if (id == 0x3eb) { // RA2: as above
            return "STT:GenerateSliderNumPlayers"; // RA2: as above
        }
        if (id == 0x621) { // RA2: as above
            return "STT:GenerateButtonSurprise"; // RA2: as above
        }
        if (id == 0x620) { // RA2: as above
            return "STT:GenerateButtonPreview"; // RA2: as above
        }
        if (id == 0x6c2) { // RA2: as above
            return "STT:GenerateButtonLoadMap"; // RA2: as above
        }
        if (id == 0x6c3) { // RA2: as above
            return "STT:GenerateButtonSaveMap"; // RA2: as above
        }
        if (id == 0x6c4) { // RA2: as above
            return "STT:GenerateButtonDeleteMap"; // RA2: as above
        }
        if (id == 0x6c5) { // RA2: as above
            return "STT:GenerateButtonUseMap"; // RA2: as above
        }
        if (id == 0x5c0) { // RA2: as above
            return "STT:GenerateButtonCancel"; // RA2: as above
        }
    } else if (kind == 0x2b5) { // RA2: as above
        if (id == 0x528) { // RA2: as above
            return "STT:MsnDltListMission"; // RA2: as above
        }
        if (id == 0x6c8) { // RA2: as above
            return "STT:MsnDltButtonDelete"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:MsnDltButtonBack"; // RA2: as above
        }
    } else if (kind == 0x2b4) { // RA2: as above
        if (id == 0x527) { // RA2: as above
            return "STT:MsnSaveListMission"; // RA2: as above
        }
        if (id == 0x526) { // RA2: as above
            return "STT:MsnSaveEditName"; // RA2: as above
        }
        if (id == 0x6c7) { // RA2: as above
            return "STT:MsnSaveButtonSave"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:MsnSaveButtonBack"; // RA2: as above
        }
    } else if (kind == 0x108) { // RA2: as above
        if (id == 0x6d2) { // RA2: as above
            return "STT:MPScoreLabelGame"; // RA2: as above
        }
        if (id == 0x3ea) { // RA2: as above
            return "STT:MPScoreLabelTime"; // RA2: as above
        }
        if ((id == 0x411) || (id == 0x412) || (id == 0x413) || (id == 0x414) || (id == 0x415) || (id == 0x416) || (id == 0x417) || (id == 0x418)) { // RA2: as above
            return "STT:MPScoreLabelPlayer"; // RA2: as above
        }
        if ((id == 0x419) || (id == 0x41a) || (id == 0x41b) || (id == 0x41c) || (id == 0x41d) || (id == 0x41e) || (id == 0x41f) || (id == 0x420)) { // RA2: as above
            return "STT:MPScoreLabelKills"; // RA2: as above
        }
        if ((id == 0x6a3) || (id == 0x6a4) || (id == 0x6a9) || (id == 0x6ac) || (id == 0x6af) || (id == 0x6b2) || (id == 0x6b5) || (id == 0x6b8)) { // RA2: as above
            return "STT:MPScoreLabelLosses"; // RA2: as above
        }
        if ((id == 0x6a5) || (id == 0x6a7) || (id == 0x6aa) || (id == 0x6ad) || (id == 0x6b0) || (id == 0x6b3) || (id == 0x6b6) || (id == 0x6b9)) { // RA2: as above
            return "STT:MPScoreLabelBuilt"; // RA2: as above
        }
        if ((id == 0x6a6) || (id == 0x6a8) || (id == 0x6ab) || (id == 0x6ae) || (id == 0x6b1) || (id == 0x6b4) || (id == 0x6b7) || (id == 0x6ba)) { // RA2: as above
            return "STT:MPScoreLabelScore"; // RA2: as above
        }
        if (id == 0x6d1) { // RA2: as above
            return "STT:MPScoreButtonContinue"; // RA2: as above
        }
    } else if (kind == 0xb5) { // RA2: as above
        if (id == 0x521) { // RA2: as above
            return "STT:IGSingleOptButtonControls"; // RA2: as above
        }
        if (id == 0x51e) { // RA2: as above
            return "STT:IGSingleOptButtonLoad"; // RA2: as above
        }
        if (id == 0x51f) { // RA2: as above
            return "STT:IGSingleOptButtonSave"; // RA2: as above
        }
        if (id == 0x520) { // RA2: as above
            return "STT:IGSingleOptButtonDelete"; // RA2: as above
        }
        if (id == 0x522) { // RA2: as above
            return "STT:IGSingleOptButtonAbort"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:IGSingleOptButtonBack"; // RA2: as above
        }
    } else if (kind == 0xbba) { // RA2: as above
        if (id == 0x521) { // RA2: as above
            return "STT:IGMultiOptButtonControls"; // RA2: as above
        }
        if (id == 0x522) { // RA2: as above
            return "STT:IGMultiOptButtonAbort"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:IGMultiOptButtonResume"; // RA2: as above
        }
    } else if (kind == 0xea) { // RA2: as above
        if ((((id == 0x640) || (id == 0x642)) || (id == 0x641)) || (((id == 0x643 || (id == 0x644)) || ((id == 0x645 || ((id == 0x646 || (id == 0x647)))))))) { // RA2: as above
            return "STT:ReconnectButtonPlayer"; // RA2: as above
        }
        if ((((id == 0x648) || (id == 0x649)) || (id == 0x64a)) || (((id == 0x64b || (id == 0x64c)) || ((id == 0x64d || ((id == 0x64e || (id == 0x64f)))))))) { // RA2: as above
            return "STT:ReconnectProgressPlayer"; // RA2: as above
        }
        if (id == 0x650) { // RA2: as above
            return "STT:ReconnectLabelTime"; // RA2: as above
        }
        if (id == 0x651) { // RA2: as above
            return "STT:ReconnectEditInfo"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:ReconnectButtonLeaveGame"; // RA2: as above
        }
    } else if (kind == 0x6b) { // RA2: as above
        if (id == 0x6eb) { // RA2: as above
            return "STT:ScenarioListGameType"; // RA2: as above
        }
        if (id == 0x553) { // RA2: as above
            return "STT:ScenarioListMaps"; // RA2: as above
        }
        if (id == 0x468) { // RA2: as above
            return "STT:ScenarioMapThumbnail"; // RA2: as above
        }
        if (id == 0x6c5) { // RA2: as above
            return "STT:ScenarioButtonUseMap"; // RA2: as above
        }
        if (id == 0x583) { // RA2: as above
            return "STT:ScenarioButtonRandom"; // RA2: as above
        }
        if (id == 0x5c0) { // RA2: as above
            return "STT:ScenarioButtonCancel"; // RA2: as above
        }
    } else if (kind == 0xb8) { // RA2: as above
        if (id == 0x52f) { // RA2: as above
            return "STT:SoundOptSliderMusic"; // RA2: as above
        }
        if (id == 0x532) { // RA2: as above
            return "STT:SoundOptSliderSound"; // RA2: as above
        }
        if (id == 0x536) { // RA2: as above
            return "STT:SoundOptSliderVoice"; // RA2: as above
        }
        if (id == 0x530) { // RA2: as above
            return "STT:SoundOptListScore"; // RA2: as above
        }
        if (id == 0x533) { // RA2: as above
            return "STT:SoundOptCBoxShuffle"; // RA2: as above
        }
        if (id == 0x534) { // RA2: as above
            return "STT:SoundOptCBoxRepeat"; // RA2: as above
        }
        if (id == 0x531) { // RA2: as above
            return "STT:SoundOptButtonPlay"; // RA2: as above
        }
        if (id == 0x535) { // RA2: as above
            return "STT:SoundOptButtonStop"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:SoundOptButtonBack"; // RA2: as above
        }
    } else if (kind == 0x103) { // RA2: as above
        if (id == 0x6f1) { // RA2: as above
            return "STT:StandScoreParTime"; // RA2: as above
        }
        if (id == 0x6f2) { // RA2: as above
            return "STT:StandScoreYourTime"; // RA2: as above
        }
        if (id == 0x6f3) { // RA2: as above
            return "STT:StandScoreTitle"; // RA2: as above
        }
        if (id == 0x6f4) { // RA2: as above
            return "STT:StandScoreMessage"; // RA2: as above
        }
        if (id == 0x6f6) { // RA2: as above
            return "STT:StandScoreKills"; // RA2: as above
        }
        if (id == 0x423) { // RA2: as above
            return "STT:StandScoreButtonReturn"; // RA2: as above
        }
    } else if (kind == 0xff) { // RA2: as above
        if (id == 0x3ef) { // RA2: as above
            return "STT:OptionInternetLatency"; // RA2: as above
        }
        if (id == 0x529) { // RA2: as above
            return "STT:OptionInternetSpeedSlider"; // RA2: as above
        }
        if (id == 0x521) { // RA2: as above
            return "STT:OptionInternetGameControls"; // RA2: as above
        }
        if (id == 0x522) { // RA2: as above
            return "STT:OptionInternetAbortMission"; // RA2: as above
        }
        if (id == 0x675) { // RA2: as above
            return "STT:IGMultiOptButtonResume"; // RA2: as above
        }
    } else if (kind == 0x122) { // RA2: as above
        if (id == 0x6f9) { // RA2: as above
            return "STT:WOLAddBuddyQueryDoNotAsk"; // RA2: as above
        }
        if (id == 1) { // RA2: as above
            return "STT:WOLAddBuddyQueryYes"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:WOLAddBuddyQueryNo"; // RA2: as above
        }
    } else if (kind == 0x112) { // RA2: as above
        if (id == 0x6f9) { // RA2: as above
            return "STT:WOLAutoLoginDoNotAsk"; // RA2: as above
        }
        if (id == 1) { // RA2: as above
            return "STT:WOLAutoLoginYes"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:WOLAutoLoginNo"; // RA2: as above
        }
    } else if (kind == 0x116) { // RA2: as above
        if (id == 0x689) { // RA2: as above
            return "STT:WOLBuddyBuddyList"; // RA2: as above
        }
        if (id == 0x6f0) { // RA2: as above
            return "STT:WOLBuddyWins"; // RA2: as above
        }
        if (id == 0x6f1) { // RA2: as above
            return "STT:WOLBuddyLosses"; // RA2: as above
        }
        if (id == 0x701) { // RA2: as above
            return "STT:WOLBuddyDisconnects"; // RA2: as above
        }
        if (id == 0x6f4) { // RA2: as above
            return "STT:WOLBuddyRank"; // RA2: as above
        }
        if (id == 0x6f5) { // RA2: as above
            return "STT:WOLBuddyPoints"; // RA2: as above
        }
        if (id == 0x62b) { // RA2: as above
            return "STT:WOLBuddyCreateGame"; // RA2: as above
        }
        if (id == 0x62d) { // RA2: as above
            return "STT:WOLBuddyJoin"; // RA2: as above
        }
        if (id == 0x760) { // RA2: as above
            return "STT:ObserverToggle"; // RA2: as above
        }
        if (id == 0x5c2) { // RA2: as above
            return "STT:WOLBuddyPage"; // RA2: as above
        }
        if (id == 0x702) { // RA2: as above
            return "STT:WOLBuddyAdd"; // RA2: as above
        }
        if (id == 0x703) { // RA2: as above
            return "STT:WOLBuddyRemove"; // RA2: as above
        }
        if (id == 0x704) { // RA2: as above
            return "STT:WOLBuddyRefresh"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:WOLBuddyBack"; // RA2: as above
        }
    } else if (kind == 0x113) { // RA2: as above
        if (id == 0x55d) { // RA2: as above
            return "STT:WOLLobbyRefreshChannels"; // RA2: as above
        }
        if (id == 0x53f) { // RA2: as above
            return "STT:LobbyEditOutput"; // RA2: as above
        }
        if (id == 0x540) { // RA2: as above
            return "STT:LobbyEditInput"; // RA2: as above
        }
        if (id == 0x3ed) { // RA2: as above
            return "STT:EmoteButton"; // RA2: as above
        }
        if (id == 0x53d) { // RA2: as above
            return "STT:WOLLobbyChannelList"; // RA2: as above
        }
        if (id == 0x62b) { // RA2: as above
            return "STT:LobbyButtonNew"; // RA2: as above
        }
        if (id == 0x62d) { // RA2: as above
            if (ObserverMode == 0) { // RA2: as above
                return "STT:LobbyButtonJoin"; // RA2: as above
            }
            return "STT:LobbyButtonObserve"; // RA2: as above
        }
        if (id == 0x760) { // RA2: as above
            return "STT:ObserverToggle"; // RA2: as above
        }
        if (id == 0x689) { // RA2: as above
            return "STT:WOLBuddy"; // RA2: as above
        }
        if (id == 0x558) { // RA2: as above
            return "STT:WOLClanLadder"; // RA2: as above
        }
        if (id == 0x61f) { // RA2: as above
            return "STT:WOLLobbyOptions"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:WOLLobbyBack"; // RA2: as above
        }
    } else if (kind == 0xfe) { // RA2: as above
        if (id == 0x3f0) { // RA2: as above
            return "STT:WOLMyInformationLocale"; // RA2: as above
        }
        if (id == 1) { // RA2: as above
            return "STT:WOLLocaleOK"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:WOLLocaleCancel"; // RA2: as above
        }
    } else if (kind == 0x10f) { // RA2: as above
        if (id == 0x628) { // RA2: as above
            return "STT:WOLLoginNickname"; // RA2: as above
        }
        if (id == 0x72c) { // RA2: as above
            return "STT:WOLLoginPassword"; // RA2: as above
        }
        if (id == 0x72d) { // RA2: as above
            return "STT:WOLLoginRemember"; // RA2: as above
        }
        if (id == 0x539) { // RA2: as above
            return "STT:WOLLoginLogin"; // RA2: as above
        }
        if (id == 0x6ea) { // RA2: as above
            return "STT:WOLNewAccount"; // RA2: as above
        }
        if (id == 0x6eb) { // RA2: as above
            return "STT:WOLLoginForget"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:WOLLoginBack"; // RA2: as above
        }
        if (id == 0x6ec) { // RA2: as above
            return "STT:WOLManageAccount"; // RA2: as above
        }
    } else if (kind == 0x11d) { // RA2: as above
        if (id == 0x628) { // RA2: as above
            return "STT:IGRLogin"; // RA2: as above
        }
        if (id == 0x72c) { // RA2: as above
            return "STT:WOLLoginPassword"; // RA2: as above
        }
        if (id == 0x539) { // RA2: as above
            return "STT:WOLLoginLogin"; // RA2: as above
        }
        if (id == 0x6ea) { // RA2: as above
            return "STT:WOLNewAccount"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:WOLLoginBack"; // RA2: as above
        }
        if (id == 0x6ec) { // RA2: as above
            return "STT:WOLManageAccount"; // RA2: as above
        }
        if (id == 0x629) { // RA2: as above
            return "STT:WOLMyInformationServer"; // RA2: as above
        }
        if (id == 0x62a) { // RA2: as above
            return "STT:WOLMyInformationLocale"; // RA2: as above
        }
        if (id == 0x6a1) { // RA2: as above
            return "STT:WOLMyInformationCountry"; // RA2: as above
        }
        if (id == 0x6a2) { // RA2: as above
            return "STT:WOLMyInformationColor"; // RA2: as above
        }
    } else if (kind == 0x117) { // RA2: as above
        if (id == 0x53f) { // RA2: as above
            return "STT:WOLMatchOutput"; // RA2: as above
        }
        if (id == 0x5c0) { // RA2: as above
            return (Session.Type == GAME_INTERNET) && (Online_Session_State == 2) ? "WOL:WDTMatchBack" : "STT:WOLMatchCancel"; // RA2: as above
        }
    } else if (kind == 0x114) { // RA2: as above
        if (id == 0x628) { // RA2: as above
            return "STT:WOLMyInformationName"; // RA2: as above
        }
        if (id == 0x629) { // RA2: as above
            return "STT:WOLMyInformationServer"; // RA2: as above
        }
        if (id == 0x62a) { // RA2: as above
            return "STT:WOLMyInformationLocale"; // RA2: as above
        }
        if (id == 0x6a1) { // RA2: as above
            return "STT:WOLMyInformationCountry"; // RA2: as above
        }
        if (id == 0x6a2) { // RA2: as above
            return "STT:WOLMyInformationColor"; // RA2: as above
        }
        if (id == 0x6ff) { // RA2: as above
            return "STT:WOLMyInformationAutoLogin"; // RA2: as above
        }
        if (id == 0x6f0) { // RA2: as above
            return "STT:WOLMyInformationWins"; // RA2: as above
        }
        if (id == 0x6f1) { // RA2: as above
            return "STT:WOLMyInformationLosses"; // RA2: as above
        }
        if (id == 0x701) { // RA2: as above
            return "STT:WOLMyInformationDisconnects"; // RA2: as above
        }
        if (id == 0x6f4) { // RA2: as above
            return "STT:WOLMyInformationRank"; // RA2: as above
        }
        if (id == 0x6f5) { // RA2: as above
            return "STT:WOLMyInformationPoints"; // RA2: as above
        }
        if (id == 0x687) { // RA2: as above
            return "STT:WOLBuddy"; // RA2: as above
        }
        if (id == 0x688) { // RA2: as above
            return "STT:WOLClanLadder"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:WOLMyInformationBack"; // RA2: as above
        }
        if (id == 0x6ea) { // RA2: as above
            return "STT:WOLNewAccount"; // RA2: as above
        }
        if (id == 0x6eb) { // RA2: as above
            return "STT:WOLLoginForget"; // RA2: as above
        }
        if (id == 0x6ec) { // RA2: as above
            return "STT:WOLManageAccount"; // RA2: as above
        }
    } else if (kind == 0xc3) { // RA2: as above
        if (id == 0x548) { // RA2: as above
            return "STT:WOLNewGameMaxPlayers"; // RA2: as above
        }
        if (id == 0x72c) { // RA2: as above
            return "STT:WOLNewGamePassword"; // RA2: as above
        }
        if (id == 0x629) { // RA2: as above
            return "STT:WOLNewGameTournament"; // RA2: as above
        }
        if (id == 0x66c) { // RA2: as above
            return "STT:WOLNewGameClanTournament"; // RA2: as above
        }
        if (id == 1) { // RA2: as above
            return "STT:WOLNewGameOK"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:WOLNewGameCancel"; // RA2: as above
        }
        if (id == 0x71e) { // RA2: as above
            return "STT:WOLLimitScreenResolution"; // RA2: as above
        }
    } else if (kind == 0x2bc) { // RA2: as above
        if (id == 0x5f5) { // RA2: as above
            return "STT:WOLOptionsPage"; // RA2: as above
        }
        if (id == 0x5f6) { // RA2: as above
            return "STT:WOLOptionsLanguageFilter"; // RA2: as above
        }
        if (id == 0x5f7) { // RA2: as above
            return "STT:WOLOptionsFind"; // RA2: as above
        }
        if (id == 0x5f8) { // RA2: as above
            return "STT:WOLOptionsMusic"; // RA2: as above
        }
        if (id == 0x5f9) { // RA2: as above
            return "STT:WOLOptionsShowAllInLobby"; // RA2: as above
        }
        if (id == 1) { // RA2: as above
            return "STT:WOLOptionsOk"; // RA2: as above
        }
        if (id == 0x63a) { // RA2: as above
            return "STT:WOLOptionsSoundOptions"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:WOLOptionsCancel"; // RA2: as above
        }
        if (id == 0x6ed) { // RA2: as above
            return "STT:MainOptComboModes"; // RA2: as above
        }
        if (id == 0x5fa) { // RA2: as above
            return "STT:WOLOptionFilterAsian"; // RA2: as above
        }
        if (id == 0x5fb) { // RA2: as above
            return "STT:WOLOptionFilterNonAsian"; // RA2: as above
        }
    } else if (kind == 0x10e) { // RA2: as above
        if (id == 0x6f0) { // RA2: as above
            return "STT:WOLMyInformationWins"; // RA2: as above
        }
        if (id == 0x6f1) { // RA2: as above
            return "STT:WOLMyInformationLosses"; // RA2: as above
        }
        if (id == 0x701) { // RA2: as above
            return "STT:WOLMyInformationDisconnects"; // RA2: as above
        }
        if (id == 0x6f4) { // RA2: as above
            return "STT:WOLMyInformationRank"; // RA2: as above
        }
        if (id == 0x6f5) { // RA2: as above
            return "STT:WOLMyInformationPoints"; // RA2: as above
        }
        if (id == 0x6e0) { // RA2: as above
            return "STT:WOLWelcomeQuickMatch"; // RA2: as above
        }
        if (id == 0x6e1) { // RA2: as above
            return "STT:WOLWelcomeCustomMatch"; // RA2: as above
        }
        if (id == 0x6e2) { // RA2: as above
            return "STT:WOLBuddy"; // RA2: as above
        }
        if (id == 0x6e3) { // RA2: as above
            return "STT:WOLWelcomeWDT"; // RA2: as above
        }
        if (id == 0x6e4) { // RA2: as above
            return "STT:WOLWelcomeMyInformation"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:WOLWelcomeBack"; // RA2: as above
        }
    } else if (kind == 0x11c) { // RA2: as above
        if (id == 0x6e0) { // RA2: as above
            return "STT:WOLWelcomeQuickMatch"; // RA2: as above
        }
        if (id == 0x6e1) { // RA2: as above
            return "STT:WOLWelcomeCustomMatch"; // RA2: as above
        }
        if (id == 0x6e2) { // RA2: as above
            return "STT:WOLBuddy"; // RA2: as above
        }
        if (id == 0x6e3) { // RA2: as above
            return "STT:WOLWelcomeWDT"; // RA2: as above
        }
        if (id == 0x686) { // RA2: as above
            return "STT:WOLWelcomeBack"; // RA2: as above
        }
    } else if (kind == 0xe7) { // RA2: as above
        if (id == 0x62e) { // RA2: as above
            return "STT:WOLBeginNickMonth"; // RA2: as above
        }
        if (id == 0x62f) { // RA2: as above
            return "STT:WOLBeginNickDay"; // RA2: as above
        }
        if (id == 0x630) { // RA2: as above
            return "STT:WOLBeginNickYear"; // RA2: as above
        }
        if (id == 0x626) { // RA2: as above
            return "STT:WOLBeginNickEmail"; // RA2: as above
        }
        if ((id == 0x631) || (id == 0x70a)) { // RA2: as above
            return "STT:WOLBeginNickNewsletter"; // RA2: as above
        }
        if (id == 1) { // RA2: as above
            return "STT:WOLBeginNickContinue"; // RA2: as above
        }
        if (id == 0x576) { // RA2: as above
            return "STT:WOLBeginNickViewPrivacy"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:WOLBeginNickCancel"; // RA2: as above
        }
    } else if (kind == 0xe6 || kind == 0xf3 || kind == 0xf4) { // RA2: as above
        if (id == 1) { // RA2: as above
            return "STT:WOLNewNickOk"; // RA2: as above
        }
        if (id == 2) { // RA2: as above
            return "STT:WOLNewNickCancel"; // RA2: as above
        }
        if (id == 0x628) { // RA2: as above
            return "STT:WOLNewNickNickname"; // RA2: as above
        }
        if (id == 0x72c) { // RA2: as above
            return "STT:WOLNewNickPassword"; // RA2: as above
        }
        if (id == 0x627) { // RA2: as above
            return "STT:WOLNewNickVerify"; // RA2: as above
        }
        if (id == 0x5e5) { // RA2: as above
            return "STT:WOLNewNickParentEMail"; // RA2: as above
        }
        if ((id == 0x66b) || (id == 0x70b)) { // RA2: as above
            return "STT:WOLNewNickSellInfo"; // RA2: as above
        }
    } else if (kind == 0x125) { // RA2: as above
        if (id == 0x686) { // RA2: as above
            return "WOL:WDTFactionBack"; // RA2: as above
        }
        if (id != 0) { // RA2: as above
            return "WDT:FactionSelectToolTip"; // RA2: as above
        }
    } else if (kind == 0xbc6) { // RA2: as above
        if (((id == 0x411) || (id == 0x412)) || ((id == 0x413 || ((((id == 0x414 || (id == 0x415)) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))))))) { // RA2: as above
            return "STT:MPScoreLabelPlayer"; // RA2: as above
        }
        if (((id == 0x419) || ((id == 0x41a || (id == 0x41b)))) || (((id == 0x41c || (((id == 0x41d || (id == 0x41e)) || (id == 0x41f))) ) || (id == 0x420)))) { // RA2: as above
            return "STT:MPScoreLabelKills"; // RA2: as above
        }
        if ((((id == 0x6a3) || (id == 0x6a4)) || ((id == 0x6a9 || ((id == 0x6ac || (id == 0x6af)))))) || ((id == 0x6b2 || ((id == 0x6b5 || (id == 0x6b8)))))) { // RA2: as above
            return "STT:MPScoreLabelLosses"; // RA2: as above
        }
        if ((((id == 0x6a5) || (((id == 0x6a7 || (id == 0x6aa)) || (id == 0x6ad)))) || (((id == 0x6b0 || (id == 0x6b3)) || (id == 0x6b6)))) || (id == 0x6b9)) { // RA2: as above
            return "STT:MPScoreLabelBuilt"; // RA2: as above
        }
        if (((id == 0x6a6) || (id == 0x6a8)) || ((id == 0x6ab || ((((id == 0x6ae || (id == 0x6b1)) || (id == 0x6b4)) || ((id == 0x6b7 || (id == 0x6ba)))))))) { // RA2: as above
            return "STT:MPScoreLabelScore"; // RA2: as above
        }
        if (((id == 0x70a) || ((id == 0x70b || (id == 0x70c)))) || (((id == 0x70d || (((id == 0x70e || (id == 0x70f)) || (id == 0x710)))) || (id == 0x711)))) { // RA2: as above
            return "STT:MPScoreLabelStatus"; // RA2: as above
        }
        if (id == 0x733) { // RA2: as above
            return "STT:MPScoreLabelCampaignName"; // RA2: as above
        }
        if (id == 0x71b) { // RA2: as above
            return "STT:MPScoreLabelMapName"; // RA2: as above
        }
        if (id == 0x732) { // RA2: as above
            return "STT:MPScoreLabelDifficulty"; // RA2: as above
        }
        if (id == 0x6d1) { // RA2: as above
            return "STT:MPCampScoreContinue"; // RA2: as above
        }
        if (id == 0x5c0) { // RA2: as above
            return "STT:MPCampScoreQuit"; // RA2: as above
        }
        if (id == 0x3ea) { // RA2: as above
            return "STT:MPScoreLabelTime"; // RA2: as above
        }
    } else if (kind == 0xbc7) { // RA2: as above
        if (id == 0x733) { // RA2: as above
            return "STT:MPScoreLabelCampaignName"; // RA2: as above
        }
        if (id == 0x735) { // RA2: as above
            return "STT:MPCampEndCampaignNumber"; // RA2: as above
        }
        if (id == 0x732) { // RA2: as above
            return "STT:MPScoreLabelDifficulty"; // RA2: as above
        }
        if (id == 0x3ea) { // RA2: as above
            return "STT:MPCampEndTime"; // RA2: as above
        }
        if (((id == 0x411) || (id == 0x412)) || ((id == 0x73a || (id == 0x739)))) { // RA2: as above
            return "STT:MPScoreLabelPlayer"; // RA2: as above
        }
        if ((id == 0x419) || (id == 0x41a)) { // RA2: as above
            return "STT:MPScoreLabelKills"; // RA2: as above
        }
        if ((id == 0x6a3) || (id == 0x6a4)) { // RA2: as above
            return "STT:MPScoreLabelLosses"; // RA2: as above
        }
        if ((id == 0x6a5) || (id == 0x6a7)) { // RA2: as above
            return "STT:MPScoreLabelBuilt"; // RA2: as above
        }
        if ((((id == 0x6a6) || (id == 0x6a8)) || (id == 0x73b)) || (id == 0x738)) { // RA2: as above
            return "STT:MPScoreLabelScore"; // RA2: as above
        }
        if (id == 0x6d1) { // RA2: as above
            return "STT:MPScoreButtonContinue"; // RA2: as above
        }
    }
    return 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Control_Disabled(HWND control, bool value) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsDisabled = value; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Control_Disabled(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        return record->IsDisabled; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above


inline bool __cdecl Is_Option_Kind(int kind) // RA2: as above
{
    return kind == 0xbc || kind == 0xbd || kind == 0x102 || kind == 0xc2 || kind == 0xc9; // RA2: as above
}
inline int __cdecl Kind_Of(unsigned int key) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    return record != 0 ? record->Kind : 0; // RA2: as above
}
inline bool __cdecl Is_Plain_Control(HWND window) // RA2: as above
{
	if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) == BS_OWNERDRAW) { // RA2: as above
		HWND control = window; // RA2: as above
		WinData * record = 0; // RA2: as above
		Control_Table.getPointer((unsigned int const &)control, &record); // RA2: as above
		if (record != 0 && record->SkipTests == 0) { // RA2: as above
			return true; // RA2: as above
		}
	}
	return false; // RA2: as above
}

#ifdef RA2_NON_MATCHING // not exact: 2216 bytes differ (adopted text; the image calls Is_Listed_Kind out of line, ours expands it)
inline bool Is_Quick_Control_B(int id)
{
	return id == 0x411 ||
		id == 0x412 ||
		id == 0x419 ||
		id == 0x41a ||
		id == 0x6a3 ||
		id == 0x6a4 ||
		id == 0x6a5 ||
		id == 0x6a7 ||
		id == 0x6a6 ||
		id == 0x6a8 ||
		id == 0x73a ||
		id == 0x739 ||
		id == 0x73b ||
		id == 0x738;
}

inline bool Is_Quick_Control_D(int id)
{
	return id == 0x419 ||
		id == 0x41a ||
		id == 0x41b ||
		id == 0x41c ||
		id == 0x41d ||
		id == 0x41e ||
		id == 0x41f ||
		id == 0x420 ||
		id == 0x6a3 ||
		id == 0x6a4 ||
		id == 0x6a9 ||
		id == 0x6ac ||
		id == 0x6af ||
		id == 0x6b2 ||
		id == 0x6b5 ||
		id == 0x6b8 ||
		id == 0x6a5 ||
		id == 0x6a7 ||
		id == 0x6aa ||
		id == 0x6ad ||
		id == 0x6b0 ||
		id == 0x6b3 ||
		id == 0x6b6 ||
		id == 0x6b9 ||
		id == 0x6a6 ||
		id == 0x6a8 ||
		id == 0x6ab ||
		id == 0x6ae ||
		id == 0x6b1 ||
		id == 0x6b4 ||
		id == 0x6b7 ||
		id == 0x6ba ||
		id == 0x70a ||
		id == 0x70b ||
		id == 0x70c ||
		id == 0x70d ||
		id == 0x70e ||
		id == 0x70f ||
		id == 0x710 ||
		id == 0x711;
}

inline WinData * Get_Data(HWND window)
{
	WinData * data = NULL;
	if (!Control_Table.getPointer((unsigned int const &)window, &data)) {
		return NULL;
	}
	return data;
}

inline bool Is_Flag_Control_B(int type, int id)
{
	return (type == 0xbc ||
			type == 0xbd ||
			type == 0x102 ||
			type == 0xc2 ||
			   type == 0xc9) && (id == 0x6ec ||
			id == 0x5a8);
}

inline int Get_Dialog_Type(HWND dialog)
{
	WinData * data = Get_Data(dialog);
	if (data != NULL) {
		return data->Kind;
	}
	return 0;
}

BOOL __stdcall Enable_Animated_Control(HWND window, LPARAM lparam) // RA2: as above
{
	HWND parent = GetParent(window);
	int type = Get_Dialog_Type(parent);
	int id = GetDlgCtrlID(window);
	bool value = lparam != 0;

	if (((type == 0x10c || type == 0x94 || type == 0xa3 || type == 0x73 || type == 0xd8 || type == 0xbbb || type == 0xf5 || type == 0x105 || type == 0xe2 || type == 0xd5 || type == 0x2b5 || type == 0xb7 || type == 0x2b4 || type == 0xbc7 || type == 0x101 || type == 0x129 || type == 0xbc6 || type == 0xd7 || type == 0x108 || type == 0xbc || type == 0xbd || type == 0xbb || type == 0xb5 || type == 0xbba || type == 0xff || type == 0x6b || type == 0x100 || type == 0x102 || type == 0xb8 || type == 0xd6 || type == 0x103 || Is_Listed_Kind(type)) && id == 0x694) ||
		(Kind_Test(type) && id == 0x695) ||
		((type == 0x94 || type == 0xd8 || type == 0xf5 || type == 0xe2 || type == 0xd5 || type == 0x101 || type == 0x129 || type == 0xd7 || type == 0xbb || type == 0x100 || type == 0xd6 || type == 0x125 || type == 0x122 || type == 0x112 || type == 0xe7 || type == 0x116 || type == 0x11d || type == 0x11c || type == 0xfe || type == 0x10f || type == 0x117 || type == 0x114 || type == 0xe6 || type == 0xf3 || type == 0xf4 || type == 0x2bc || type == 0x10e || type == 0xbc6 || type == 0xbc7) && id == 0x71c) ||
		((type == 0xbc || type == 0xbd || type == 0x102 || type == 0xc2 || type == 0xc9 || type == 0x105 || type == 0x6b || type == 0x113) && id == 0x468) ||
		Is_Flag_Control_B(type, id) ||
		((type == 0x103 || type == 0xbc7) && (id == 0x6f3 || id == 0x6f4 || id == 0x72b || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2 || id == 0x69f || id == 0x6f6 || id == 0x733 || id == 0x735 || id == 0x732 || id == 0x3ea || id == 0x736 || id == 0x737 || id == 0x73c || id == 0x73d || id == 0x73e || id == 0x73f || id == 0x740 || Is_Quick_Control_B(id))) ||
		((type == 0x108 || type == 0xbc6) && (id == 0x6d2 || id == 0x3ea || id == 0x69d || id == 0x69f || id == 0x69e || id == 0x6a0 || id == 0x6a1 || id == 0x5c8 || id == 0x411 || id == 0x412 || id == 0x413 || id == 0x414 || id == 0x415 || id == 0x416 || id == 0x417 || id == 0x418 || Is_Quick_Control_D(id) || id == 0x733 || id == 0x732 || id == 0x71b || id == 0x735)) ||
		(Is_Plain_Control(window) && Is_Owner_Tested_Control((unsigned int)parent, window)) ||
		(Is_Plain_Control(window) && Is_Default_Button((unsigned int)parent, window))) {
		Set_Control_Disabled(window, value);
	}
	return TRUE;
}
#endif

}


void *__fastcall Get_Drawer(); // RA2: as above
void *__fastcall Get_Alternate_Drawer(); // RA2: as above
void *Get_Chat_Art(); // RA2: as above
Point2D __fastcall Point_Copy(Point2D const *source); // RA2: as above
void __cdecl Audio_Update(); // RA2: as above

namespace OwnerDraw { // RA2: as above

/*
**	RA2: starts the animation of every animated control of the dialog.
*/
void __fastcall Enable_Animated_Controls(HWND window) // RA2: as above
{
	EnumChildWindows(window, Enable_Animated_Control, 1); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

/*
**	RA2: stops the animation of every animated control of the dialog.
*/
void __fastcall Disable_Animated_Controls(HWND window) // RA2: as above
{
	EnumChildWindows(window, Enable_Animated_Control, 0); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above


inline int __cdecl Visible_Count(HWND dialog) // RA2: as above
{
    Visible_Free_Buttons = 0; // RA2: as above
    EnumChildWindows(dialog, Count_Visible_Free_Buttons, 0); // RA2: as above
    return Visible_Free_Buttons; // RA2: as above
}

inline bool __cdecl Any_Visible(HWND dialog) // RA2: as above
{
    Visible_Default_Buttons = 0; // RA2: as above
    EnumChildWindows(dialog, Count_Visible_Default_Buttons, 0); // RA2: as above
    return Visible_Default_Buttons >= 1; // RA2: as above
}

inline int __cdecl Slide_Phase(int delta) // RA2: as above
{
    if (delta < 0) return -1; // RA2: as above
    if (delta >= 6) return -2; // RA2: as above
    return delta; // RA2: as above
}

inline int __cdecl Last_Step(int const *order, int pieces) // RA2: as above
{
    int steps = 0; // RA2: as above
    for (int j = 0; j < pieces; j++) { // RA2: as above
        if (steps <= order[j]) steps = order[j]; // RA2: as above
    }
    return steps; // RA2: as above
}

void __fastcall Slide_Dialog(HWND dialog, bool reverse) // RA2: as above
{
    WinData *record = Get_Record(dialog); // RA2: as above
    bool has_d7 = record ? record->IsChat : false; // RA2: as above
    record = Get_Record(dialog); // RA2: as above
    bool has_d6 = record ? record->IsLobbyFrame : false; // RA2: as above
    record = Get_Record(dialog); // RA2: as above
    bool has_d5 = record ? record->IsLobbyArt : false; // RA2: as above

    ConvertClass *drawer = (ConvertClass *)Get_Drawer(); // RA2: as above
    ConvertClass *drawer_b = (ConvertClass *)Get_Alternate_Drawer(); // RA2: as above
    ShapeSet *shape_e8 = ArtSlideTop; // RA2: as above
    ShapeSet *shape_b8 = Slide_Button_Shape; // RA2: as above
    ShapeSet *shape_ec = ArtSlideMPButton; // RA2: as above
    ShapeSet *shape_b4 = ArtSlideWarning; // RA2: as above
    ConvertClass *drawer_c = (ConvertClass *)Get_Chat_Art(); // RA2: as above
    ShapeSet *shape_f0 = (ShapeSet *)ArtScoreTop; // RA2: as above
    int count = SlideButtonCount; // RA2: as above
    Rect window = AlternateSurface->Get_Rect(); // RA2: as above
    Rect rect_c4 = *SlideTopRect; // RA2: as above
    Rect rect_b8 = *SlideMPButtonRect; // RA2: as above
    Rect rect_bc = *SlideWarningRect; // RA2: as above
    Rect rect_b4 = *SlideButtonRect; // RA2: as above
    Rect rect_c0 = *DialogBackRect; // RA2: as above

    RECT client;
    Get_Display_Rect(dialog, &client); // RA2: as above
    RECT screen; // RA2: as above
    GetWindowRect(dialog, &screen); // RA2: as above
    int dx = screen.left - client.left; // RA2: as above
    int dy = screen.top - client.top; // RA2: as above

    int x = rect_b4.X; // RA2: as above
    int y = rect_b4.Y; // RA2: as above
    int width = rect_b4.Width; // RA2: as above
    int height = rect_b4.Height * count; // RA2: as above
    if (has_d5) { // RA2: as above
        x = rect_bc.X; // RA2: as above
        width = rect_bc.Width; // RA2: as above
        y = rect_bc.Y; // RA2: as above
        height += rect_b4.Y; // RA2: as above
    }
    Rect from(x, y, width, height); // RA2: as above
    Rect to = from; // RA2: as above
    to.X += dx; // RA2: as above
    to.Y += dy; // RA2: as above
    Rect from_c0 = rect_c0; // RA2: as above
    Rect to_c0 = from_c0; // RA2: as above
    to_c0.X += dx; // RA2: as above
    to_c0.Y += dy; // RA2: as above

    int dir = reverse ? -1 : 1; // RA2: as above
    int base_64 = 5; // RA2: as above
    if (reverse) base_64 = 10; // RA2: as above
    int base_8c = 11; // RA2: as above
    if (reverse) base_8c = 16; // RA2: as above
    int base_94 = 1; // RA2: as above
    if (reverse) base_94 = 6; // RA2: as above
    int base_88 = 0; // RA2: as above
    if (reverse) base_88 = 5; // RA2: as above
    int base_90 = 0; // RA2: as above
    if (reverse) base_90 = 5; // RA2: as above

    int buttons = Visible_Count(dialog); // RA2: as above
    int last = count - 1; // RA2: as above
    int top = rect_b4.Y; // RA2: as above
    if (has_d6) top += rect_b4.Height; // RA2: as above
    bool any = Any_Visible(dialog); // RA2: as above
    if (!any) last++; // RA2: as above

    int pieces = count + 3; // RA2: as above
    int limit = pieces - 2; // RA2: as above
    int *order = new int[pieces]; // RA2: as above
    int total = count + 3; // RA2: as above
    int value = 1; // RA2: as above
    for (int i = 0; i < limit; i++) { // RA2: as above
        order[i] = value++; // RA2: as above
    }
    order[total - 1] = 0; // RA2: as above
    order[limit] = value + 1; // RA2: as above
    order[pieces - 3] = 0; // RA2: as above
    int steps = Last_Step(order, pieces) + 6; // RA2: as above

    for (int step = 0; step < steps; step++) { // RA2: as above
        if (has_d6) { // RA2: as above
            Point2D point = Point_Copy((Point2D const *)&rect_b8); // RA2: as above
            int phase = Slide_Phase(step - order[pieces - 3]); // RA2: as above
            if (phase == -1) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer, shape_ec, reverse ? 6 : 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else if (phase == -2) { // RA2: as above
                if (!reverse) { // RA2: as above
					Draw_Shape(*AlternateSurface, *drawer, shape_ec, 6, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
                }
            }
        }
        if (has_d5) { // RA2: as above
            Point2D point = Point_Copy((Point2D const *)&rect_bc); // RA2: as above
            int phase = Slide_Phase(step - order[pieces - 2]); // RA2: as above
            if (phase == -1) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer, shape_e8, !reverse, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else if (phase == -2) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer, shape_e8, reverse != 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else {
                point = Point_Copy((Point2D const *)&rect_c4); // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer, shape_e8, 1, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
                point = Point_Copy((Point2D const *)&rect_bc); // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer, shape_b4, phase * dir + base_88, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            }
        }
        if (has_d6) { // RA2: as above
            Point2D point = Point_Copy((Point2D const *)&rect_b8); // RA2: as above
            int phase = Slide_Phase(step - order[pieces - 3]); // RA2: as above
            if (phase == -2) { // RA2: as above
                if (reverse) { // RA2: as above
					Draw_Shape(*AlternateSurface, *drawer, shape_ec, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
                }
            } else {
				Draw_Shape(*AlternateSurface, *drawer, shape_ec, phase * dir + base_94, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            }
        }
        int k; // RA2: as above
        for (k = 0; k < buttons; k++) { // RA2: as above
            Point2D point(rect_b4.X, k * rect_b4.Height + top); // RA2: as above
            int phase = Slide_Phase(step - order[k]); // RA2: as above
            if (phase == -1) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 10 : 1, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else if (phase == -2) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 1 : 10, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, phase * dir + base_64, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            }
        }
        for (k = buttons; k < last; k++) { // RA2: as above
            Point2D point(rect_b4.X, k * rect_b4.Height + top); // RA2: as above
            int phase = Slide_Phase(step - order[k]); // RA2: as above
            if (phase == -1) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 10 : 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else if (phase == -2) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 0 : 10, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, phase * dir + base_8c, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            }
        }
        if (any) { // RA2: as above
            Point2D point(rect_b4.X, Button_Strip_Bottom->Y - rect_b4.Height); // RA2: as above
            int phase = Slide_Phase(step - order[count - 1]); // RA2: as above
            if (phase == -1) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 10 : 1, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else if (phase == -2) { // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 1 : 10, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, phase * dir + base_64, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            }
        }
        if (has_d7) { // RA2: as above
            Point2D point = Point_Copy((Point2D const *)&rect_c0); // RA2: as above
            int phase = Slide_Phase(step - order[total - 1]); // RA2: as above
            if (phase == -1) { // RA2: as above
                if (!reverse) { // RA2: as above
                    if (VideoModeWidth >= Base_Screen_Width) point.X += 0x50; // RA2: as above
                }
				Draw_Shape(*AlternateSurface, *drawer_c, (reverse ? Art_Chat_Back : shape_f0), 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else if (phase == -2) { // RA2: as above
                if (reverse) { // RA2: as above
                    if (VideoModeWidth >= Base_Screen_Width) point.X += 0x50; // RA2: as above
                }
				Draw_Shape(*AlternateSurface, *drawer_c, (reverse ? shape_f0 : Art_Chat_Back), 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else {
                if (VideoModeWidth >= Base_Screen_Width) point.X += 0x50; // RA2: as above
				Draw_Shape(*AlternateSurface, *drawer_c, shape_f0, phase * dir + base_90, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            }
        }
        VisibleSurface->Lock();
        AlternateSurface->Lock();
        VisibleSurface->Blit_From(to, *AlternateSurface, from, false, true); // RA2: as above
        if (has_d7) { // RA2: as above
            VisibleSurface->Blit_From(to_c0, *AlternateSurface, from_c0, false, true); // RA2: as above
        }
        AlternateSurface->Unlock();
        VisibleSurface->Unlock();
        Audio_Update(); // RA2: as above
        Sleep(30); // RA2: as above
    }
    delete [] order; // RA2: as above
    if (reverse) { // RA2: as above
        VocClass::PlayGlobal(Rule->ShellButtonSlideSound, 1.0f, 0x2000, 0); // RA2: as above
        while (MouseCursor->Get_Mouse_State()) { // RA2: as above
            MouseCursor->Show_Mouse(); // RA2: as above
        }
        SendMessageA(dialog, 0x4ec, 0, 0); // RA2: as above
    } else {
        SendMessageA(dialog, 0x4ed, 0, 0); // RA2: as above
    }
}

}


void __fastcall Slide_Pending_Dialog(HWND window) // RA2: as above
{
    OwnerDraw::WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)window, &record); // RA2: as above
    if (record && record->IsNotifyPending) { // RA2: as above
        HWND item = GetDlgItem(window, 0x71a); // RA2: as above
        if (item) { // RA2: as above
            SendMessageA(item, 0x4e2, 0, 0); // RA2: as above
        }
        OwnerDraw::Slide_Dialog(window, false); // RA2: as above
        record->IsNotifyPending = false; // RA2: as above
    }
}


namespace OwnerDraw { // RA2: as above

bool __fastcall WS_Save_Dialog_Values(HWND dialog) // RA2: as above
{
    if (Session.Is_In_Game()) { // RA2: as above
        return false; // RA2: as above
    }
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)dialog, &record); // RA2: as above
    if (!record) { // RA2: as above
        return false; // RA2: as above
    }
    bool active = record->IsHidden && record->ButtonKind == 1; // RA2: as above
    bool visible = IsWindowVisible(dialog) != 0; // RA2: as above
    if (!active || !visible) { // RA2: as above
        return false; // RA2: as above
    }
    VocClass::PlayGlobal(Rule->GUIMoveOutSound, 1.0f, 0x2000, 0); // RA2: as above
    bool enabled = IsWindowEnabled(dialog) != 0; // RA2: as above
    EnableWindow(dialog, FALSE); // RA2: as above
    EnumChildWindows(dialog, Enable_Animated_Control, 1); // RA2: as above
    record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)dialog, &record); // RA2: as above
    if (!record) { // RA2: as above
        return false; // RA2: as above
    }
    record->IsNotifyPending = true; // RA2: as above
    InvalidateRect(dialog, 0, FALSE); // RA2: as above
    DWORD start = GetTickCount(); // RA2: as above
    while (!Dialog_Message_Handler()) { // RA2: as above
        int done = !record->IsNotifyPending; // RA2: as above
        if ((int)(GetTickCount() - start) > 5000 || done) { // RA2: as above
            break;
        }
    }
    EnumChildWindows(dialog, Enable_Animated_Control, 0); // RA2: as above
    EnableWindow(dialog, enabled); // RA2: as above
    return true; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Play_Dialog_Move_In(HWND dialog) // RA2: as above
{
    if (Session.Is_In_Game()) { // RA2: as above
        return false; // RA2: as above
    }
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)dialog, &record); // RA2: as above
    if (!record) { // RA2: as above
        return false; // RA2: as above
    }
    bool active = record->IsHidden && record->ButtonKind == 1; // RA2: as above
    bool visible = IsWindowVisible(dialog) != 0; // RA2: as above
    if (!active || !visible) { // RA2: as above
        return false; // RA2: as above
    }
    VocClass::PlayGlobal(Rule->GUIMoveInSound, 1.0f, 0x2000, 0); // RA2: as above
    bool enabled = IsWindowEnabled(dialog) != 0; // RA2: as above
    EnableWindow(dialog, FALSE); // RA2: as above
    EnumChildWindows(dialog, Enable_Animated_Control, 1); // RA2: as above
    Slide_Dialog(dialog, true); // RA2: as above
    EnumChildWindows(dialog, Enable_Animated_Control, 0); // RA2: as above
    EnableWindow(dialog, enabled); // RA2: as above
    InvalidateRect(dialog, 0, FALSE); // RA2: as above
    return true; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Dialog_Hidden(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsHidden = true; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Clear_Dialog_Hidden(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsHidden = false; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Dialog_Waiting(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsWaiting = true; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Clear_Dialog_Waiting(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsWaiting = false; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above
bool __fastcall Select_Control_Rect(HWND dialog,HWND control,Rect *bounds) // RA2: as above
{
    WinData *record = Get_Record(dialog); // RA2: as above
    int kind = record ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(control); // RA2: as above
    Rect *selected = 0; // RA2: as above
    if (kind == 0x94) { // RA2: as above
        if (VideoModeWidth < ScreenWidths[1]) return false; // RA2: as above
        if (id == 0x6ea) selected = new Rect(0x1e,0x1a,0x23a,0x87); // RA2: as above
        else if (id == 0x6eb) selected = new Rect(0x52,0xbb,0x1d4,0x6c); // RA2: as above
        else if (id == 0x6ec) selected = new Rect(0x62,0x12a,0x1bc,0x95); // RA2: as above
        else if (id == 0x50f) selected = new Rect(0xb1,0x1fd,0x110,0x15); // RA2: as above
        else if (id == 0x71e) selected = new Rect(0xb1,0x1dc,0x8d,0x14); // RA2: as above
        else if (id == 0x670) selected = new Rect(0x135,0x1dc,0x8d,0x14); // RA2: as above
        else return false; // RA2: as above
    }
    else if (kind == 0x103) { // RA2: as above
        if (id != 0x72b) return false; // RA2: as above
        if (VideoModeWidth >= ScreenWidths[1]) { // RA2: as above
            if (Scen->IsGDI) selected = new Rect(0x68,0x8e,0x1a8,0xdc); // RA2: as above
            else selected = new Rect(0x67,0x82,0x1a8,0xe6); // RA2: as above
        } else {
            if (Scen->IsGDI) selected = new Rect(0x18,0x8e,0x1a8,0xdc); // RA2: as above
            else selected = new Rect(0x17,0x82,0x1a8,0xe6); // RA2: as above
        }
    }
    else if (kind == 0xbc7) { // RA2: as above
        if (id != 0x72b) return false; // RA2: as above
        if (VideoModeWidth >= ScreenWidths[1]) { // RA2: as above
            if (Scen->IsGDI) selected = new Rect(0x68,0x8e,0x1a8,0xdc); // RA2: as above
            else selected = new Rect(0x67,0x82,0x1a8,0xe6); // RA2: as above
        } else {
            if (Scen->IsGDI) selected = new Rect(0x18,0x8e,0x1a8,0xdc); // RA2: as above
            else selected = new Rect(0x17,0x82,0x1a8,0xe6); // RA2: as above
        }
    }
    else if (kind == 0xbc) { // RA2: as above
        if (id != 0x72f) return false; // RA2: as above
        if (VideoModeWidth == ScreenWidths[0]) selected = new Rect(0x34,0x4e,0x18c,0x8f); // RA2: as above
        else if (VideoModeWidth == ScreenWidths[1] || VideoModeWidth == ScreenWidths[2]) selected = new Rect(0x84,0x8a,0x18c,0x8f); // RA2: as above
        else return false; // RA2: as above
    }
    else if (kind == 0xbd) { // RA2: as above
        if (id != 0x72f) return false; // RA2: as above
        if (VideoModeWidth == ScreenWidths[0]) selected = new Rect(0x34,0x4e,0x18c,0x8f); // RA2: as above
        else if (VideoModeWidth == ScreenWidths[1] || VideoModeWidth == ScreenWidths[2]) selected = new Rect(0x84,0x8a,0x18c,0x8f); // RA2: as above
        else return false; // RA2: as above
    }
    else if (kind == 0xc2) { // RA2: as above
        if (id != 0x72f) return false; // RA2: as above
        if (VideoModeWidth == ScreenWidths[0]) selected = new Rect(0x34,0x4e,0x18c,0x8f); // RA2: as above
        else if (VideoModeWidth == ScreenWidths[1] || VideoModeWidth == ScreenWidths[2]) selected = new Rect(0x84,0x8a,0x18c,0x8f); // RA2: as above
        else return false; // RA2: as above
    }
    else if (kind == 0xc9) { // RA2: as above
        if (id != 0x72f) return false; // RA2: as above
        if (VideoModeWidth == ScreenWidths[0]) selected = new Rect(0x34,0x4e,0x18c,0x8f); // RA2: as above
        else if (VideoModeWidth == ScreenWidths[1] || VideoModeWidth == ScreenWidths[2]) selected = new Rect(0x84,0x8a,0x18c,0x8f); // RA2: as above
        else return false; // RA2: as above
    }
    else return false; // RA2: as above
    if (!selected) return false; // RA2: as above
    *bounds = *selected;
    delete selected; // RA2: as above
    return true; // RA2: as above
}
}


bool __fastcall Is_Owner_Tested_Control(unsigned int key, HWND window) // RA2: as above
{
    OwnerDraw::WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (((((((((kind == 0x10c) || (kind == 0x94)) || (kind == 0xa3)) || // RA2: as above
                    (((kind == 0x73 || (kind == 0xd8)) || // RA2: as above
                      ((kind == 0xbbb || ((kind == 0xf5 || (kind == 0x105)))))))) || // RA2: as above
                  ((kind == 0xe2 || (((kind == 0xd5 || (kind == 0x2b5)) || (kind == 0xb7)))))) || // RA2: as above
                (((kind == 0x2b4 || (kind == 0xbc7)) || // RA2: as above
                  (((kind == 0x101 || ((kind == 0x129 || (kind == 0xbc6)))) || (kind == 0xd7)))))) || // RA2: as above
              ((((((((kind == 0x108 || (kind == 0xbc)) || (kind == 0xbd)) || // RA2: as above
                        ((kind == 0xbb || (kind == 0xb5)))) || // RA2: as above
                      ((kind == 0xbba || ((kind == 0xff || (kind == 0x6b)))))) || (kind == 0x100)) || // RA2: as above
                  ((((kind == 0x102 || (kind == 0xb8)) || (kind == 0xd6)) || // RA2: as above
                    (((kind == 0x103 || (kind == 0x125)) || // RA2: as above
                      ((kind == 0xe7 || ((kind == 0x116 || (kind == 0xc2)))))))))) || (kind == 0xc9)))) || // RA2: as above
            (((((kind == 0x11d || (kind == 0x11c)) || (kind == 0x114)) || // RA2: as above
                ((kind == 700 || (kind == 0x113)))) || // RA2: as above
              (((kind == 0x109 || ((kind == 0x10f || (kind == 0x117)))) || // RA2: as above
                ((kind == 0xe6 || (((kind == 0xf3 || (kind == 0xf4)) || (kind == 0x10e)))))))))) && // RA2: as above
          (id == 0x694)) { // RA2: as above
        return true; // RA2: as above
    }
    if ((((((kind == 0x94) || (kind == 0xd8)) || // RA2: as above
                ((kind == 0xf5 || ((kind == 0xe2 || (kind == 0xd5)))))) || // RA2: as above
              ((kind == 0x101 || // RA2: as above
                (((((kind == 0x129 || (kind == 0xd7)) || (kind == 0xbb)) || // RA2: as above
                    ((kind == 0x100 || (kind == 0xd6)))) || (kind == 0x125)))))) || // RA2: as above
            (((((kind == 0x122 || (kind == 0x112)) || // RA2: as above
                  ((kind == 0xe7 || (((kind == 0x116 || (kind == 0x11d)) || (kind == 0x11c)))))) || // RA2: as above
                (((kind == 0xfe || (kind == 0x10f)) || (kind == 0x117)))) || // RA2: as above
              (((kind == 0x114 || (kind == 0xe6)) || // RA2: as above
                (((kind == 0xf3 || (((kind == 0xf4 || (kind == 700)) || (kind == 0x10e)))) || // RA2: as above
                  ((kind == 0xbc6 || (kind == 0xbc7)))))))))) && (id == 0x71c)) { // RA2: as above
        return true; // RA2: as above
    }
    if ((((kind == 0xbc) || (kind == 0xbd)) || // RA2: as above
            ((kind == 0x102 || // RA2: as above
              ((((kind == 0xc2 || (kind == 0xc9)) || (kind == 0x105)) || // RA2: as above
                ((kind == 0x6b || (kind == 0x113)))))))) && (id == 0x468)) { // RA2: as above
        return true; // RA2: as above
    }
    if (kind == 0xe2) { // RA2: as above
        return id == 0x686 || id == 0x578 || id == 0x55c || id == 0x683 || id == 0x684; // RA2: as above
    }
    if (kind == 0x101) { // RA2: as above
        return id == 0x68e || id == 0x68d || id == 0x68f; // RA2: as above
    }
    if (kind == 0x129) { // RA2: as above
        return id == 0x745; // RA2: as above
    }
    if (kind == 0x100) { // RA2: as above
        return id == 0x689 || id == 0x688 || id == 0x579; // RA2: as above
    }
    if (kind == 0x94) { // RA2: as above
        return id == 0x40e; // RA2: as above
    }
    if (kind == 0xb7) { // RA2: as above
        return id == 0x40f; // RA2: as above
    }
    if (kind == 0x2b4) { // RA2: as above
        return id == 0x6c7; // RA2: as above
    }
    if (kind == 0x2b5) { // RA2: as above
        return id == 0x6c8; // RA2: as above
    }
    if (kind == 0x105) { // RA2: as above
        return id == 0x6c4 || id == 0x6c2 || id == 0x6c3 || id == 0x6c5; // RA2: as above
    }
    if (kind == 0x102) { // RA2: as above
        return id == 0x6ec || id == 0x5aa || id == 0x5a8 || id == 0x617; // RA2: as above
    }
    if (kind == 0xb5) { // RA2: as above
        return id == 0x522 || id == 0x521 || id == 0x51e || id == 0x51f || id == 0x520; // RA2: as above
    }
    if (kind == 0xbba) { // RA2: as above
        return id == 0x522 || id == 0x521; // RA2: as above
    }
    if (kind == 0xbbb) { // RA2: as above
        return id == 0x52c || id == 0x52d; // RA2: as above
    }
    if (kind == 0xd5) { // RA2: as above
        return id == 0x5ce || id == 0x5cd; // RA2: as above
    }
    if (kind == 0xfb) { // RA2: as above
        return id == 6 || id == 1; // RA2: as above
    }
    if (kind == 0xd8) { // RA2: as above
        return id == 0x6cb; // RA2: as above
    }
    if (kind == 0xd7) { // RA2: as above
        return id == 1; // RA2: as above
    }
    if (kind == 0xbc || kind == 0xc2) { // RA2: as above
        return id == 0x6ec || id == 0x588 || id == 0x5aa || id == 0x5a8; // RA2: as above
    }
    if (kind == 0xbd || kind == 0xc9) { // RA2: as above
        return id == 0x59f || id == 0x6ec || id == 0x5a8; // RA2: as above
    }
    if (kind == 0xbb) { // RA2: as above
        return id == 0x6cd || id == 0x6cc || id == 0x6ce; // RA2: as above
    }
    if (kind == 0xb6) { // RA2: as above
        return id == 0x6c9 || id == 0x712 || id == 0x524; // RA2: as above
    }
    if (kind == 0x6b) { // RA2: as above
        return id == 0x583 || id == 0x6c5; // RA2: as above
    }
    if (kind == 0xbc6) { // RA2: as above
        return id == 0x6d1; // RA2: as above
    }
    if (kind == 0xbc7) { // RA2: as above
        return id == 0x6d1; // RA2: as above
    }
    if (kind == 0x10f || kind == 0x11d) { // RA2: as above
        return id == 0x539 || id == 0x6ea || id == 0x6ec || id == 0x6eb; // RA2: as above
    }
    if (kind == 0x109) { // RA2: as above
        return id == 1 || id == 0x625 || id == 0x53b; // RA2: as above
    }
    if (kind == 0x10e || kind == 0x11c) { // RA2: as above
        return id == 0x6e0 || id == 0x6e1 || id == 0x6e2 || id == 0x6e4 || id == 0x6e3; // RA2: as above
    }
    if (kind == 0x114) { // RA2: as above
        return id == 0x687 || id == 0x6ea || id == 0x6ec || id == 0x6eb || id == 0x688; // RA2: as above
    }
    if (kind == 0x113) { // RA2: as above
        return id == 0x61f || id == 0x558 || id == 0x689 || id == 0x62d || id == 0x62b || id == 0x53d; // RA2: as above
    }
    if (kind == 700) { // RA2: as above
        return id == 1 || id == 0x63a; // RA2: as above
    }
    if (kind == 0x116) { // RA2: as above
        return id == 0x62b || id == 0x62d || id == 0x5c2 || id == 0x702 || id == 0x703; // RA2: as above
    }
    if (kind == 0xff) { // RA2: as above
        return id == 0x521 || id == 0x522; // RA2: as above
    }
    if (kind == 0x122) { // RA2: as above
        return id == 1 || id == 2; // RA2: as above
    }
    if (kind == 0x112) { // RA2: as above
        return id == 1; // RA2: as above
    }
    if (kind == 0xe7) { // RA2: as above
        return id == 1 || id == 0x576; // RA2: as above
    }
    if (kind == 0xfe) { // RA2: as above
        return id == 1; // RA2: as above
    }
    if (kind == 0xe6 || kind == 0xf3 || kind == 0xf4) { // RA2: as above
        return id == 1; // RA2: as above
    }
    return false; // RA2: as above
}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Free_Owner_Button(unsigned int key, HWND window) // RA2: as above
{
    if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) != BS_OWNERDRAW) { // RA2: as above
        return false; // RA2: as above
    }
    HWND control = window; // RA2: as above
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int const &)control, &record); // RA2: as above
    if (record && record->SkipTests == 0) { // RA2: as above
        return Is_Owner_Tested_Control(key, window); // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Default_Button(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (kind == 0xe2) { // RA2: as above
        return id == 0x3ee; // RA2: as above
    }
    if (((kind == 0x73) || (kind == 0x10c)) || (kind == 0x103)) { // RA2: as above
        return id == 0x423; // RA2: as above
    }
    if (kind == 0x108) { // RA2: as above
        return id == 0x6d1; // RA2: as above
    }
    if ((((kind != 0x94) && (kind != 0xb6)) && // RA2: as above
            ((kind != 0xa3 && ((kind != 0xd8 && (kind != 0xbbb)))))) && // RA2: as above
         ((((kind != 0xf5 && // RA2: as above
                ((((kind != 0xd5 && (kind != 0x2b5)) && (kind != 0xb7)) && // RA2: as above
                 ((kind != 0x2b4 && (kind != 0x101)))))) && // RA2: as above
             ((((kind != 0x129 && ((kind != 0xb5 && (kind != 0xbba)))) && (kind != 0x100)) && // RA2: as above
                (((((kind != 0xb8 && (kind != 0xd6)) && (kind != 0x125)) && // RA2: as above
                    ((kind != 0x116 && (kind != 0x11d)))) && (kind != 0x11c)))))) && // RA2: as above
            (((kind != 0x113 && (kind != 0x109)) && // RA2: as above
             ((kind != 0x10f && ((kind != 0x114 && (kind != 0x10e)))))))))) { // RA2: as above
        if ((kind == 0x102) || // RA2: as above
             ((((kind == 0x105 || (kind == 0x6b)) || (kind == 0xbb)) || // RA2: as above
                ((kind == 0x117 || (kind == 0xbc6)))))) { // RA2: as above
            return id == 0x5c0; // RA2: as above
        }
        if (((((kind == 0xbc) || (kind == 0xbd)) || // RA2: as above
                 ((kind == 0xea || (((kind == 0xd7 || (kind == 0xc2)) || (kind == 0xc9)))))) || // RA2: as above
                ((kind == 700 || (kind == 0xe7)))) || // RA2: as above
             ((kind == 0xe6 || // RA2: as above
                (((kind == 0xf3 || (kind == 0xf4)) || // RA2: as above
                 ((kind == 0x122 || (((kind == 0x112 || (kind == 0xfe)) || (kind == 0xfc)))))))))) { // RA2: as above
            return id == 2; // RA2: as above
        }
        if (kind == 0xd4) { // RA2: as above
            return id == 1; // RA2: as above
        }
        if (kind == 0xfb) { // RA2: as above
            return id == 7; // RA2: as above
        }
        if (kind == 0xff) { // RA2: as above
            return id == 0x675; // RA2: as above
        }
        return false; // RA2: as above
    }
    return id == 0x686; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Owner_Default_Button(unsigned int key, HWND window) // RA2: as above
{
    if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) != BS_OWNERDRAW) { // RA2: as above
        return false; // RA2: as above
    }
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)window, &record); // RA2: as above
    if (record && record->SkipTests == 0) { // RA2: as above
        return Is_Default_Button(key, window); // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Flag_Control(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (Kind_Test(kind) && id == 0x695) { // RA2: as above
        return true; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Campaign_Button(unsigned int key, HWND window) // RA2: as above
{
    if (Session.Is_In_Game()) { // RA2: as above
        return false; // RA2: as above
    }
    if (Is_Free_Owner_Button(key, window)) { // RA2: as above
        return true; // RA2: as above
    }
    if (Is_Plain_Control(window) && Is_Default_Button(key, window)) { // RA2: as above
        return true; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Network_Button(unsigned int key, HWND window) // RA2: as above
{
    if (!Session.Is_In_Game()) { // RA2: as above
        return false; // RA2: as above
    }
    if (Is_Free_Owner_Button(key, window)) { // RA2: as above
        return true; // RA2: as above
    }
    if (Is_Plain_Control(window) && Is_Default_Button(key, window)) { // RA2: as above
        return true; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Cancel_Button(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (kind == 0x10d) { // RA2: as above
        return id == 0x5c1 || id == 0x5c0; // RA2: as above
    }
    if (kind == 0xd9) { // RA2: as above
        return id == 0x5c1 || id == 0x6ca; // RA2: as above
    }
    if (kind == 0xa3) { // RA2: as above
        return id == 0x4cf || id == 0x4d0; // RA2: as above
    }
    if (kind == 0x105) { // RA2: as above
        return id == 0x620 || id == 0x621; // RA2: as above
    }
    if (kind == 0xf0) { // RA2: as above
        return id == 0x5c0; // RA2: as above
    }
    if (kind == 0xce) { // RA2: as above
        return id == 0x5ae; // RA2: as above
    }
    if (kind == 0x120) { // RA2: as above
        return id == 0x5ae || id == 2; // RA2: as above
    }
    if (kind == 0x121) { // RA2: as above
        return id == 0x5ae || id == 0x5af || id == 2; // RA2: as above
    }
    if (kind == 0xea) { // RA2: as above
        return id == 0x640 || id == 0x642 || id == 0x641 || id == 0x643 || id == 0x644 || id == 0x645 || id == 0x646 || id == 0x647; // RA2: as above
    }
    if (kind == 0xb8) { // RA2: as above
        return id == 0x531 || id == 0x535; // RA2: as above
    }
    if (kind == 0x11f) { // RA2: as above
        return id == 0x5c0; // RA2: as above
    }
    if (kind == 0x115) { // RA2: as above
        return id == 0x702 || id == 0x686; // RA2: as above
    }
    if (kind == 0xd3) { // RA2: as above
        return id == 2; // RA2: as above
    }
    if (kind == 0xcf) { // RA2: as above
        return id == 0x5c2 || id == 2; // RA2: as above
    }
    if (kind == 0xc3) { // RA2: as above
        return id == 1 || id == 2; // RA2: as above
    }
    if (kind == 0x11b) { // RA2: as above
        return id == 0x5c2 || id == 0x686; // RA2: as above
    }
    if (kind == 0xe1) { // RA2: as above
        return id == 1 || id == 2; // RA2: as above
    }
    if (kind == 0x11e) { // RA2: as above
        return id == 0x5c2 || id == 0x686; // RA2: as above
    }
    if (kind == 0xc4) { // RA2: as above
        return id == 0x58d || id == 0x58e; // RA2: as above
    }
    if (kind == 0xd0) { // RA2: as above
        return id == 1; // RA2: as above
    }
    if (kind == 0xfc) { // RA2: as above
        return id == 1 || id == 2; // RA2: as above
    }
    if (kind == 0x126) { // RA2: as above
        return id == 6 || id == 7; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Main_Menu_Movie(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (kind == 0xe2) { // RA2: as above
        return id == 0x71d; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

BOOL __stdcall Count_Visible_Free_Buttons(HWND window, LPARAM) // RA2: as above
{
    HWND parent = GetParent(window);
    if (Is_Free_Owner_Button((unsigned int)parent, window) && IsWindowVisible(window)) { // RA2: as above
        Visible_Free_Buttons++; // RA2: as above
    }
    return TRUE; // RA2: as above
}

}


unsigned int __fastcall Count_Free_Buttons(HWND window) // RA2: as above
{
    Visible_Free_Buttons = 0; // RA2: as above
    EnumChildWindows(window, Count_Visible_Free_Buttons, 0); // RA2: as above
    return Visible_Free_Buttons; // RA2: as above
}


namespace OwnerDraw { // RA2: as above

BOOL __stdcall Count_Visible_Default_Buttons(HWND window, LPARAM) // RA2: as above
{
    HWND parent = GetParent(window);
    if (Is_Owner_Default_Button((unsigned int)parent, window) && IsWindowVisible(window)) { // RA2: as above
        Visible_Default_Buttons++; // RA2: as above
    }
    return TRUE; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Has_Visible_Default_Button(HWND dialog) // RA2: as above
{
    Visible_Default_Buttons = 0; // RA2: as above
    EnumChildWindows(dialog, Count_Visible_Default_Buttons, 0); // RA2: as above
    return Visible_Default_Buttons >= 1; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

BOOL __stdcall Assign_Button_Kind(HWND window, LPARAM) // RA2: as above
{
    WinData *record = Get_Record(window); // RA2: as above
    if (record != 0) { // RA2: as above
        HWND parent = GetParent(window);
        if (Is_Campaign_Button((unsigned int)parent, window)) { // RA2: as above
            record->ButtonKind = 1; // RA2: as above
            return TRUE; // RA2: as above
        }
        if (Is_Network_Button((unsigned int)parent, window)) { // RA2: as above
            record->ButtonKind = 2; // RA2: as above
            return TRUE; // RA2: as above
        }
        if (Is_Cancel_Button((unsigned int)parent, window)) { // RA2: as above
            record->ButtonKind = 3; // RA2: as above
        }
    }
    return TRUE; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

/*
**	RA2: sorts every control of the dialog into its button kind.
*/
void __fastcall Assign_Button_Kinds(HWND window) // RA2: as above
{
	EnumChildWindows(window, Assign_Button_Kind, 0); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above


struct ControlAnimationState { // RA2: as above
    void const *Shape; // RA2: as above
    bool IsShapeAllocated; // RA2: as above
    int IsTestedControl; // RA2: as above
    int ControlHeight; // RA2: as above
    int ControlKind; // RA2: as above
    int ControlFlags; // RA2: as above
    int Reserved_90; // RA2: as above
    int FrameCount; // RA2: as above
    int Frame; // RA2: as above
    int LastTick; // RA2: as above
    int TimerInterval; // RA2: as above
    int Reserved_A4; // RA2: as above
    bool IsPlaying; // RA2: as above
};
static inline ControlAnimationState *__cdecl Animation(WinData *record) // RA2: as above
{
    return (ControlAnimationState *)record->Animation; // RA2: as above
}

static inline void __cdecl Get_Animation_Record(unsigned int const &key, WinData *&record) // RA2: as above
{
    Control_Table.getPointer(key, &record); // RA2: as above
}
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (005e8130 has no row); main: best 49.20, tried 5 times (frame, inline, ruling)
BOOL __stdcall Init_Control_Animation(HWND window, LPARAM) // RA2: as above
{
    HWND parent = GetParent(window);
    unsigned int key = (unsigned int)window; // RA2: as above
    WinData *record = 0; // RA2: as above
    Get_Animation_Record(key, record); // RA2: as above
    if (record == 0) { // RA2: as above
        return TRUE; // RA2: as above
    }
    if (Is_Tested_Control((unsigned int)parent, window)) { // RA2: as above
        record->PictureMode = 1; // RA2: as above
        Animation(record)->IsPlaying = false; // RA2: as above
        Animation(record)->IsTestedControl = 1; // RA2: as above
        Animation(record)->ControlHeight = OwnerDraw_Control_Height((unsigned int)parent, window); // RA2: as above
        Animation(record)->ControlKind = OwnerDraw_Control_Kind((unsigned int)parent, window); // RA2: as above
        Animation(record)->ControlFlags = OwnerDraw_Control_Flags((unsigned int)parent, window); // RA2: as above
        record->TypingSound = Control_Typing_Sound((unsigned int)parent, window); // RA2: as above
        return TRUE; // RA2: as above
    }
    bool animated = false; // RA2: as above
    if (Is_Image_Control((unsigned int)parent, window)) { // RA2: as above
        record->PictureMode = 2; // RA2: as above
        if (record->Image == 0) { // RA2: as above
            record->Image = (Surface *)Control_Image((unsigned int)parent, window); // RA2: as above
        }
    } else {
        if (Is_Image_Mode3_Control((unsigned int)parent, window)) { // RA2: as above
            record->PictureMode = 3; // RA2: as above
        } else if (Is_Animated_Control((unsigned int)parent, window)) { // RA2: as above
            record->PictureMode = 4; // RA2: as above
            animated = true; // RA2: as above
            if (Animation(record)->Shape == 0) { // RA2: as above
                bool allocated = false; // RA2: as above
                void *art = Control_Animation_Art((unsigned int)parent, window); // RA2: as above
                Animation(record)->Shape = Control_Animation_Shape((unsigned int)parent, window, allocated); // RA2: as above
                record->Drawer = (int)art; // RA2: as above
                Animation(record)->IsShapeAllocated = allocated; // RA2: as above
            }
        } else {
            goto done; // RA2: as above
        }
    }
    Animation(record)->Frame = 0; // RA2: as above
    Animation(record)->LastTick = GetTickCount(); // RA2: as above
    Animation(record)->TimerInterval = Control_Animation_Loops((unsigned int)parent, window); // RA2: as above
done: // RA2: as above
    if (Animation(record)->TimerInterval < 1) { // RA2: as above
        if (animated && Animation(record)->Shape) { // RA2: as above
            Animation(record)->FrameCount = ((short const *)Animation(record)->Shape)[3]; // RA2: as above
            Animation(record)->IsPlaying = false; // RA2: as above
        }
    } else {
        SetTimer(window, 0, Animation(record)->TimerInterval, 0); // RA2: as above
        if (animated) { // RA2: as above
            Animation(record)->IsPlaying = true; // RA2: as above
        }
    }
    return TRUE; // RA2: as above
}
#endif

}


namespace OwnerDraw { // RA2: as above

BOOL __fastcall Is_Listed_Dialog_Kind(int kind) // RA2: as above
{
    if (((((((kind != 0x94) && (kind != 0xd8)) && (kind != 0xf5)) && // RA2: as above
                ((kind != 0xe2 && (kind != 0xd5)))) && (kind != 0x101)) && // RA2: as above
            (((((kind != 0x129 && (kind != 0xd7)) && // RA2: as above
                  ((kind != 0xbb && (((kind != 0x100 && (kind != 0xd6)) && (kind != 0x125)))))) // RA2: as above
                && ((kind != 0x122 && (kind != 0x112)))) && (kind != 0xe7)))) && // RA2: as above
          ((((kind != 0x116 && (kind != 0x11d)) && // RA2: as above
              ((kind != 0x11c && (((kind != 0xfe && (kind != 0x10f)) && (kind != 0x117)))))) && // RA2: as above
            (((kind != 0x114 && (kind != 0xe6)) && // RA2: as above
              ((kind != 0xf3 && // RA2: as above
                (((kind != 0xf4 && (kind != 700)) && // RA2: as above
                  ((kind != 0x10e && ((kind != 0xbc6 && (kind != 0xbc7)))))))))))))) { // RA2: as above
        return 0; // RA2: as above
    }
    return 1; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

/*
**	RA2: sets up the animation of every control of the dialog.
*/
void __fastcall Init_Control_Animations(HWND window) // RA2: as above
{
	EnumChildWindows(window, Init_Control_Animation, 0); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

BOOL __stdcall Reset_Tested_Control(HWND window, LPARAM) // RA2: as above
{
    HWND parent = GetParent(window);
    if (Is_Tested_Control((unsigned int)parent, window)) { // RA2: as above
        SendMessageA(window, 0x4ee, 0, 0); // RA2: as above
    }
    return TRUE; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

/*
**	RA2: resets every tested control of the dialog.
*/
}

void __fastcall Dialog_On_OD_4EC(HWND window) // RA2: as above
{
    using namespace OwnerDraw;
	EnumChildWindows(window, Reset_Tested_Control, 0); // RA2: as above
}

namespace OwnerDraw { // RA2: as above

}


namespace OwnerDraw { // RA2: as above

BOOL __stdcall Assign_Control_Sounds(HWND window, LPARAM) // RA2: as above
{
    HWND parent = GetParent(window);
    WinData *record = Get_Record(window); // RA2: as above
    if (record) { // RA2: as above
        record->SoundSlot = Control_Sound_Slot(parent, window); // RA2: as above
        record->TextStyle = Control_Text_Style(parent, window); // RA2: as above
    }
    return TRUE; // RA2: as above
}

}


void __fastcall Assign_All_Control_Sounds(HWND window) // RA2: as above
{
    EnumChildWindows(window, Assign_Control_Sounds, 0); // RA2: as above
}


namespace OwnerDraw { // RA2: as above

void __fastcall Move_Control_To_Rect(HWND window, Rect const &rect) // RA2: as above
{
    HWND parent = GetParent(window);
    unsigned char flag = Session.Is_In_Game(); // RA2: as above
    RECT parent_rect;
    GetWindowRect(parent, &parent_rect); // RA2: as above
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int offset = 0; // RA2: as above
    if (!flag) { // RA2: as above
        offset = (((area.Width - Base_Screen_Width) / 2) < 0 ? 0 : ((area.Width - Base_Screen_Width) / 2)); // RA2: as above
    }
    Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int shift = 0; // RA2: as above
    if (!flag) { // RA2: as above
        shift = (((area2.Height - Base_Screen_Height) / 2) < 0 ? 0 : ((area2.Height - Base_Screen_Height) / 2)); // RA2: as above
    }
    MoveWindow(window, rect.X + offset, rect.Y + shift, rect.Width, rect.Height, FALSE); // RA2: as above
}

}


#pragma intrinsic(abs) // RA2: as above
namespace OwnerDraw { // RA2: as above

void __fastcall Move_Free_Button(HWND window, Point2D const &size) // RA2: as above
{
    HWND parent = GetParent(window);
    unsigned char flag = Session.Is_In_Game(); // RA2: as above
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int offset = 0; // RA2: as above
    if (!flag) { // RA2: as above
        offset = (area.Width - Base_Screen_Width) / 2; // RA2: as above
        offset = offset < 0 ? 0 : offset; // RA2: as above
    }
    Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int shift = 0; // RA2: as above
    if (!flag) { // RA2: as above
        shift = (((area2.Height - Base_Screen_Height) / 2) < 0 ? 0 : ((area2.Height - Base_Screen_Height) / 2)); // RA2: as above
        shift -= (((size.Y - Base_Screen_Height) / 2) < 0 ? 0 : ((size.Y - Base_Screen_Height) / 2)); // RA2: as above
    }
    int x;
    int y;
    int width; // RA2: as above
    int height; // RA2: as above
    if (flag) { // RA2: as above
        width = Button_Strip_Shape->Get_Width(); // RA2: as above
        height = Button_Strip_Shape->Get_Height(); // RA2: as above
        Rect *strip = Free_Button_Strip; // RA2: as above
        x = parent_rect.right - offset - parent_rect.left - 0x93; // RA2: as above
        HWND dialog = GetParent(window); // RA2: as above
        RECT parent2; // RA2: as above
        RECT rect2;
        GetWindowRect(dialog, &parent2); // RA2: as above
        GetWindowRect(window, &rect2);
        int top = rect2.top - parent2.top; // RA2: as above
        y = ((rect2.bottom - parent2.top - top) / 2 - 0xc6 + top) / 0x2c * height + strip->Y; // RA2: as above
    } else {
        width = Slide_Button_Shape->Get_Width(); // RA2: as above
        height = Slide_Button_Shape->Get_Height(); // RA2: as above
        Rect *strip = Button_Strip_Top; // RA2: as above
        x = parent_rect.right - offset - parent_rect.left - 0x9c; // RA2: as above
        int top = strip->Y; // RA2: as above
        int pos = rect.top - top - parent_rect.top + shift; // RA2: as above
        int row = pos / strip->Height; // RA2: as above
        int prod = strip->Height * row; // RA2: as above
        int up = abs(strip->Height - pos + prod); // RA2: as above
        int down = abs(pos - prod); // RA2: as above
        y = (down < up ? row : row + 1) * strip->Height + top; // RA2: as above
    }
    MoveWindow(window, x, y, width, height, FALSE); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Move_Tested_Control(HWND window, Point2D const &size) // RA2: as above
{
    HWND parent = GetParent(window);
    unsigned char flag = Session.Is_In_Game(); // RA2: as above
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int offset = 0; // RA2: as above
    if (!flag) { // RA2: as above
        int excess = (area.Width - Base_Screen_Width) / 2; // RA2: as above
        offset = excess < 0 ? 0 : excess; // RA2: as above
    }
    Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int shift = 0; // RA2: as above
    if (!flag) { // RA2: as above
        shift = (((area2.Height - Base_Screen_Height) / 2) < 0 ? 0 : ((area2.Height - Base_Screen_Height) / 2)); // RA2: as above
        shift -= (((size.Y - Base_Screen_Height) / 2) < 0 ? 0 : ((size.Y - Base_Screen_Height) / 2)); // RA2: as above
    }
    Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top); // RA2: as above
    MoveWindow(window, parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left, rect.top - parent_rect.top + shift, box.Width, box.Height, FALSE); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Move_Default_Button_Owner(HWND window) // RA2: as above
{
    HWND parent = GetParent(window);
    unsigned char flag = Session.Is_In_Game(); // RA2: as above
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int offset = Sidebar_Inset(area.Width, flag); // RA2: as above
    int x;
    int y;
    int width; // RA2: as above
    int height; // RA2: as above
    if (flag) { // RA2: as above
        x = parent_rect.right - offset - parent_rect.left - 0x93; // RA2: as above
        width = Button_Strip_Shape->Get_Width(); // RA2: as above
        height = Button_Strip_Shape->Get_Height(); // RA2: as above
        y = Default_Button_Y->Y - height; // RA2: as above
    } else {
        width = Slide_Button_Shape->Get_Width(); // RA2: as above
        x = parent_rect.right - offset - parent_rect.left - 0x9c; // RA2: as above
        height = Slide_Button_Shape->Get_Height(); // RA2: as above
        int top = Button_Strip_Top->Y; // RA2: as above
        int cell = Button_Strip_Top->Height; // RA2: as above
        y = ((Button_Strip_Bottom->Y - top) / cell - 1) * cell + top; // RA2: as above
    }
    MoveWindow(window, x, y, width, height, FALSE); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Move_Default_Button(HWND window) // RA2: as above
{
    HWND parent = GetParent(window);
    unsigned char flag = Session.Is_In_Game(); // RA2: as above
    RECT parent_rect;
    RECT rect;
    GetClientRect(parent, &parent_rect); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int offset = 0; // RA2: as above
    if (!flag) { // RA2: as above
        int excess = (area.Width - Base_Screen_Width) / 2; // RA2: as above
        offset = excess < 0 ? 0 : excess; // RA2: as above
    }
    Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top); // RA2: as above
    box.X = parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left; // RA2: as above
    int bottom; // RA2: as above
    if (flag) bottom = Default_Button_Strip->Y; // RA2: as above
    else bottom = Button_Strip_Bottom->Y; // RA2: as above
    box.Y = bottom - box.Height; // RA2: as above
    MoveWindow(window, box.X, box.Y, box.Width, box.Height, FALSE); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Move_Flag_Control(HWND window) // RA2: as above
{
    HWND parent = GetParent(window);
    unsigned char flag = Session.Is_In_Game(); // RA2: as above
    RECT parent_rect;
    RECT rect;
    GetClientRect(parent, &parent_rect); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    int left = parent_rect.left; // RA2: as above
    Rect area(left, parent_rect.top, parent_rect.right - left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int offset = 0; // RA2: as above
    if (!flag) { // RA2: as above
        offset = (area.Width - Base_Screen_Width) / 2; // RA2: as above
        offset = offset < 0 ? 0 : offset; // RA2: as above
    }
    Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int shift = 0; // RA2: as above
    if (!flag) { // RA2: as above
        shift = (area2.Height - Base_Screen_Height) / 2; // RA2: as above
        shift = shift < 0 ? 0 : shift; // RA2: as above
    }
    int width = rect.right - rect.left; // RA2: as above
    int height = rect.bottom - rect.top; // RA2: as above
    MoveWindow(window, offset + 10 + left, parent_rect.bottom - shift - height - 1, width, height, FALSE); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Move_Lobby_Control(HWND window) // RA2: as above
{
    HWND parent = GetParent(window);
    unsigned char flag = Session.Is_In_Game(); // RA2: as above
    RECT parent_rect;
    RECT rect;
    GetClientRect(parent, &parent_rect); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top); // RA2: as above
    int offset = 0; // RA2: as above
    if (!flag) { // RA2: as above
        int excess = (area.Width - Base_Screen_Width) / 2; // RA2: as above
        offset = excess < 0 ? 0 : excess; // RA2: as above
    }
    Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top); // RA2: as above
    box.X = parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left; // RA2: as above
    Rect *strip = Default_Button_Strip; // RA2: as above
    if (!flag) strip = Button_Strip_Bottom; // RA2: as above
    int bottom = strip->Height + strip->Y; // RA2: as above
    box.Y = bottom - box.Height; // RA2: as above
    MoveWindow(window, box.X, box.Y, box.Width, box.Height, FALSE); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Move_Centred_Control(HWND window, Point2D const &size) // RA2: as above
{
    HWND parent = GetParent(window);
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    MoveWindow(window, (VideoModeWidth - size.X) / 2 - parent_rect.left + rect.left, // RA2: as above
               (VideoModeHeight - size.Y) / 2 - parent_rect.top + rect.top, rect.right - rect.left, // RA2: as above
               rect.bottom - rect.top, FALSE); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (005e9410 has no row); main: best 99.79, tried 11 times (frame, register, order)
void __fastcall Adjust_Control_Position(HWND window) // RA2: as above
{
    HWND parent = GetParent(window);
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    int x = rect.left - parent_rect.left; // RA2: as above
    int y = rect.top - parent_rect.top; // RA2: as above
    int width = rect.right - rect.left; // RA2: as above
    int height = rect.bottom - rect.top; // RA2: as above
    int kind = Kind_Of((unsigned int)parent); // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    int old_x = x; // RA2: as above
    int old_y = y; // RA2: as above
    int old_width = width; // RA2: as above
    int old_height = height; // RA2: as above
    if ((kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103 || Is_Listed_Kind(kind)) && id == 0x694) { // RA2: as above
        bool c12 = kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103; // RA2: as above
        if (c12) { // RA2: as above
            bool c4 = Is_Option_Kind(kind) || kind == 0x105 || kind == 0x6b || kind == 0x113; // RA2: as above
            if (c4) { // RA2: as above
                if (!Session.Is_In_Game()) { // RA2: as above
                    y += 1; // RA2: as above
                }
            } else {
                if (!Session.Is_In_Game()) { // RA2: as above
                    y += 7; // RA2: as above
                    height += 1; // RA2: as above
                }
            }
        } else if (kind == 0x125 || kind == 0xe7 || kind == 0x116 || kind == 0xc2 || kind == 0xc9 || kind == 0x11d || kind == 0x11c || kind == 0x114 || kind == 700 || kind == 0x113 || kind == 0x109 || kind == 0x10f || kind == 0x117 || kind == 0xe6 || kind == 0xf3 || kind == 0xf4 || kind == 0x10e) { // RA2: as above
            if (kind != 0xc2 && kind != 0xc9 && kind != 0x113) { // RA2: as above
                y += 2; // RA2: as above
            }
        }
    } else if (kind == 0x102) { // RA2: as above
        if (id == 0x50c) { // RA2: as above
            y -= 1; // RA2: as above
        } else if (id == 0x54e || id == 0x693 || id == 0x696 || id == 0x69a) { // RA2: as above
            x -= 1; // RA2: as above
        }
    } else if (kind == 0xbb) { // RA2: as above
        if (id == 0x540) { // RA2: as above
            width -= 1; // RA2: as above
        }
    } else if (kind == 0xbc || kind == 0xbd) { // RA2: as above
        if (id == 0x53f) { // RA2: as above
            width += 1; // RA2: as above
        } else if (id == 0x54f) { // RA2: as above
            y -= 1; // RA2: as above
        } else if (id == 0x54e || id == 1000 || id == 0x54d || id == 0x69a) { // RA2: as above
            x -= 1; // RA2: as above
        }
    } else if (kind == 0x103) { // RA2: as above
        if (id == 0x6f3 || id == 0x6f4 || id == 0x69f || id == 0x6f6 || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2) { // RA2: as above
            if (VideoModeWidth >= Base_Screen_Width) { // RA2: as above
                y -= (Base_Screen_Height - ScreenHeights[0]) / 2; // RA2: as above
            }
        }
    } else if (kind == 0xbc7) { // RA2: as above
        if (id == 0x733 || id == 0x735 || id == 0x732 || id == 0x3ea || id == 0x73c || id == 0x411 || id == 0x412 || id == 0x73d || id == 0x419 || id == 0x41a || id == 0x73e || id == 0x6a3 || id == 0x6a4 || id == 0x73f || id == 0x6a5 || id == 0x6a7 || id == 0x740 || id == 0x6a6 || id == 0x6a8 || id == 0x736 || id == 0x73a || id == 0x73b || id == 0x737 || id == 0x739 || id == 0x738) { // RA2: as above
            if (VideoModeWidth >= Base_Screen_Width) { // RA2: as above
                y -= (Base_Screen_Height - ScreenHeights[0]) / 2; // RA2: as above
            }
        }
    }
    if (x != old_x || y != old_y || width != old_width || old_height != height) { // RA2: as above
        MoveWindow(window, x, y, width, height, FALSE); // RA2: as above
    }
}
#endif

}


namespace OwnerDraw { // RA2: as above

inline bool __cdecl Is_Lobby_Control(unsigned int key, HWND window) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    int kind = record != 0 ? record->Kind : 0; // RA2: as above
    int id = GetDlgCtrlID(window); // RA2: as above
    if (kind == 0xe2) { // RA2: as above
        return id == 0x71d; // RA2: as above
    }
    return false; // RA2: as above
}
BOOL __stdcall Move_Child_For_Resolution(HWND window, LPARAM lparam) // RA2: as above
{
    HWND parent = GetParent(window);
    if (Current_Dialog == parent) { // RA2: as above
        Rect rect(0, 0, 0, 0); // RA2: as above
        Point2D const &size = *(Point2D const *)lparam; // RA2: as above
        if (Select_Control_Rect(parent, window, &rect)) { // RA2: as above
            Move_Control_To_Rect(window, rect); // RA2: as above
        } else if (Is_Free_Owner_Button((unsigned int)parent, window)) { // RA2: as above
            Move_Free_Button(window, size); // RA2: as above
        } else if (Is_Owner_Tested_Control((unsigned int)parent, window)) { // RA2: as above
            Move_Tested_Control(window, size); // RA2: as above
        } else if (Is_Owner_Default_Button((unsigned int)parent, window)) { // RA2: as above
            Move_Default_Button_Owner(window); // RA2: as above
        } else if (Is_Default_Button((unsigned int)parent, window)) { // RA2: as above
            Move_Default_Button(window); // RA2: as above
        } else if (Is_Flag_Control((unsigned int)parent, window)) { // RA2: as above
            Move_Flag_Control(window); // RA2: as above
        } else if (Is_Lobby_Control((unsigned int)parent, window)) { // RA2: as above
            Move_Lobby_Control(window); // RA2: as above
        } else {
            Move_Centred_Control(window, size); // RA2: as above
        }
        Adjust_Control_Position(window); // RA2: as above
    }
    return TRUE; // RA2: as above
}

}


void __fastcall Resize_Dialog_To_Screen(HWND dialog, Point2D *old_size) // RA2: as above
{
    int width = VideoModeWidth; // RA2: as above
    int height = VideoModeHeight; // RA2: as above
    MoveWindow(dialog, 0, 0, width, height, 0); // RA2: as above
    Current_Dialog = dialog; // RA2: as above
    LPARAM param = (LPARAM)old_size; // RA2: as above
    EnumChildWindows(dialog, OwnerDraw::Move_Child_For_Resolution, param); // RA2: as above
}


namespace OwnerDraw { // RA2: as above

int __fastcall Control_Drawer(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    return record ? record->Drawer : 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above
bool __fastcall Is_Framed_Dialog(HWND dialog) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)dialog, &record); // RA2: as above
    if (!record) return false; // RA2: as above
    int kind = record->Kind; // RA2: as above
    bool special = kind == 0x10c || // RA2: as above
        kind == 0x94 || // RA2: as above
        kind == 0xb6 || // RA2: as above
        kind == 0xa3 || // RA2: as above
        kind == 0x73 || // RA2: as above
        kind == 0xd8 || // RA2: as above
        kind == 0xbbb || // RA2: as above
        kind == 0xf5 || // RA2: as above
        kind == 0x105 || // RA2: as above
        kind == 0xd5 || // RA2: as above
        kind == 0xe2 || // RA2: as above
        kind == 0x2b5 || // RA2: as above
        kind == 0xb7 || // RA2: as above
        kind == 0x2b4 || // RA2: as above
        kind == 0x101 || // RA2: as above
        kind == 0x129 || // RA2: as above
        kind == 0xbc6 || // RA2: as above
        kind == 0xbc7 || // RA2: as above
        kind == 0x10b || // RA2: as above
        kind == 0xd7 || // RA2: as above
        kind == 0x108 || // RA2: as above
        kind == 0xbc || // RA2: as above
        kind == 0xbd || // RA2: as above
        kind == 0xbb || // RA2: as above
        kind == 0xb5 || // RA2: as above
        kind == 0xbba || // RA2: as above
        kind == 0xff || // RA2: as above
        kind == 0xea || // RA2: as above
        kind == 0x6b || // RA2: as above
        kind == 0x100 || // RA2: as above
        kind == 0x102 || // RA2: as above
        kind == 0xb8 || // RA2: as above
        kind == 0xd6 || // RA2: as above
        kind == 0x103 || // RA2: as above
        kind == 0xd4 || // RA2: as above
        kind == 0x125 || // RA2: as above
        kind == 0x122 || // RA2: as above
        kind == 0x112 || // RA2: as above
        kind == 0xe7 || // RA2: as above
        kind == 0x116 || // RA2: as above
        kind == 0xc2 || // RA2: as above
        kind == 0xc9 || // RA2: as above
        kind == 0x113 || // RA2: as above
        kind == 0xfe || // RA2: as above
        kind == 0x10f || // RA2: as above
        kind == 0x11d || // RA2: as above
        kind == 0x11c || // RA2: as above
        kind == 0x117 || // RA2: as above
        kind == 0x114 || // RA2: as above
        kind == 0xe6 || // RA2: as above
        kind == 0xf3 || // RA2: as above
        kind == 0xf4 || // RA2: as above
        kind == 0x2bc || // RA2: as above
        kind == 0x10e || // RA2: as above
        kind == 0xfb; // RA2: as above
    if (special) { // RA2: as above
        record->ButtonKind = 1; // RA2: as above
        record->IsHidden = true; // RA2: as above
    }
    return special; // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above

inline WinData *__cdecl Control_Record(HWND control) // RA2: as above
{
    unsigned int key = (unsigned int)control; // RA2: as above
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    return record; // RA2: as above
}

bool __fastcall Centre_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(*(unsigned int *)&control, &record); // RA2: as above
    if (!record) { // RA2: as above
        return false; // RA2: as above
    }
    int kind = record->Kind; // RA2: as above
    bool special = kind == 0x10d || // RA2: as above
        kind == 0xd9 || // RA2: as above
        kind == 0xf0 || // RA2: as above
        kind == 0xce || // RA2: as above
        kind == 0x120 || // RA2: as above
        kind == 0x121 || // RA2: as above
        kind == 0x115 || // RA2: as above
        kind == 0xd3 || // RA2: as above
        kind == 0xcf || // RA2: as above
        kind == 0x11f || // RA2: as above
        kind == 0xc3 || // RA2: as above
        kind == 0x11b || // RA2: as above
        kind == 0xe1 || // RA2: as above
        kind == 0x11e || // RA2: as above
        kind == 0xc4 || // RA2: as above
        kind == 300 || // RA2: as above
        kind == 0xd0 || // RA2: as above
        kind == 0xfc || // RA2: as above
        kind == 0x126; // RA2: as above
    if (special) { // RA2: as above
        record->ButtonKind = 2; // RA2: as above
    }
    return special; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Lobby_Frame_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        return record->IsLobbyFrame; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Update_Lobby_Frame_Dialog(HWND control) // RA2: as above
{
    WinData *record = Control_Record(control); // RA2: as above
    if (!record) { // RA2: as above
        return false; // RA2: as above
    }
    return record->IsLobbyFrame = record->Kind == 0xbc || record->Kind == 0xbd || record->Kind == 0x102 || record->Kind == 0xc2 || record->Kind == 0xc9; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Lobby_Frame_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsLobbyFrame = true; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Clear_Lobby_Frame_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsLobbyFrame = false; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Lobby_Art_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        return record->IsLobbyArt; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Update_Lobby_Art_Dialog(HWND control) // RA2: as above
{
    WinData *record = Control_Record(control); // RA2: as above
    if (!record) { // RA2: as above
        return false; // RA2: as above
    }
    return record->IsLobbyArt = record->Kind == 0xbc || record->Kind == 0xbd || record->Kind == 0x102 || record->Kind == 0xc2 || record->Kind == 0xc9 || record->Kind == 0x105 || record->Kind == 0x6b || record->Kind == 0x113; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Lobby_Art_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsLobbyArt = true; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Clear_Lobby_Art_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        record->IsLobbyArt = false; // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Chat_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    return record ? record->IsChat : false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Update_Chat_Dialog(HWND control) // RA2: as above
{
    WinData *record = Control_Record(control); // RA2: as above
    if (!record) { // RA2: as above
        return false; // RA2: as above
    }
    return record->IsChat = record->Kind == 0x103 || record->Kind == 0xbc7; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Is_Setup_Dialog(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        return record->IsSetup; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Update_Setup_Dialog(HWND control) // RA2: as above
{
    WinData *record = Control_Record(control); // RA2: as above
    if (!record) { // RA2: as above
        return false; // RA2: as above
    }
    return record->IsSetup = record->Kind == 0x108 || record->Kind == 0xbc6; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

int __fastcall Control_Shape_640(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    return record ? record->Shape640 : 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

int __fastcall Control_Shape(HWND control) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    return record ? record->Shape : 0; // RA2: as above
}

}


struct ShapeSet; // RA2: as above
void *Get_Score_Drawer(); // RA2: as above

namespace OwnerDraw { // RA2: as above

void __fastcall Set_Control_Art(unsigned int key) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer(key, &record); // RA2: as above
    if (record != 0) { // RA2: as above
        int kind = record->Kind; // RA2: as above
        if (kind == 0x94) { // RA2: as above
            record->Drawer = (int)Get_Slot_Art(); // RA2: as above
            record->Shape640 = (int)Art_Button_Back; // RA2: as above
            record->Shape = (int)Art_Button_Back; // RA2: as above
        } else if (kind == 0x103) { // RA2: as above
            record->Drawer = (int)Get_Chat_Art(); // RA2: as above
            record->Shape640 = (int)Art_Chat_Back; // RA2: as above
            record->Shape = (int)Art_Chat_Back; // RA2: as above
        } else if (kind == 0x108) { // RA2: as above
            record->Drawer = (int)Get_Score_Drawer(); // RA2: as above
            record->Shape640 = (int)Art_Score_Screen; // RA2: as above
            record->Shape = (int)Art_Score_Screen; // RA2: as above
        } else if (kind == 0xBC6) { // RA2: as above
            record->Drawer = (int)Get_Chat_Art(); // RA2: as above
            record->Shape640 = (int)Art_Chat_Back; // RA2: as above
            record->Shape = (int)Art_Chat_Back; // RA2: as above
        } else if (kind == 0xBC7) { // RA2: as above
            record->Drawer = (int)Get_Chat_Art(); // RA2: as above
            record->Shape640 = (int)Art_Chat_Back; // RA2: as above
            record->Shape = (int)Art_Chat_Back; // RA2: as above
        } else {
            record->Drawer = (int)Get_Drawer(); // RA2: as above
            record->Shape640 = (int)Art_Dialog_Back640; // RA2: as above
            record->Shape = (int)Art_Dialog_Back; // RA2: as above
        }
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Dialog_Kind(HWND control, int value) // RA2: as above
{
    WinData *record = Control_Record(control); // RA2: as above
    if (record) { // RA2: as above
        record->Kind = value; // RA2: as above
    }
}

}

int __fastcall Dialog_ID_Of_Window(HWND control) // RA2: as above
{
    using namespace OwnerDraw;
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)control, &record); // RA2: as above
    if (record) { // RA2: as above
        return record->Kind; // RA2: as above
    }
    return 0; // RA2: as above
}

int __fastcall Dialog_Box(int resource, int (__stdcall *proc)(struct HWND__ *, unsigned int, unsigned int, long), bool Show) // RA2: as above
{
    HWND dialog = OwnerDraw::Begin_Dialog(resource, proc, 0); // RA2: as above
    int result = 0; // RA2: as above
    if (dialog) { // RA2: as above
        SetWindowLongA(dialog, DWL_USER, (LONG)&result); // RA2: as above
        OwnerDraw::Display_Dialog(dialog); // RA2: as above
        if (Show) { // RA2: as above
            Show_Dialog_With_Flag(dialog); // RA2: as above
        }
        while (result == 0) { // RA2: as above
            if (OwnerDraw::Dialog_Message_Handler()) { // RA2: as above
                break;
            }
        }
        OwnerDraw::End_Dialog(dialog); // RA2: as above
    }
    return result; // RA2: as above
}


/*
**	RA2: called by Init_Game; RA2 sets the colours in a static initializer instead, so nothing is left to do.
*/
void OwnerDraw::Initialize(void)
{
}


/*
**	RA2: called on exit (WinMain, Prog_End); does nothing.
*/
void OwnerDraw::Shutdown(void) // RA2: as above
{
}


namespace OwnerDraw { // RA2: as above

void __fastcall Register_Window_Classes() // RA2: as above
{
    if (Classes_Registered == 1) { // RA2: as above
        return;
    }
    Classes_Registered = 1; // RA2: as above
    WNDCLASSA window_class; // RA2: as above
    memset(&window_class, 0, sizeof(window_class)); // RA2: as above
    window_class.hInstance = ProgramInstance; // RA2: as above
    window_class.style = 3; // RA2: as above
    window_class.lpfnWndProc = Combo_Drop_Proc; // RA2: as above
    window_class.cbClsExtra = 0; // RA2: as above
    window_class.cbWndExtra = 0; // RA2: as above
    window_class.hIcon = 0; // RA2: as above
    window_class.hCursor = 0; // RA2: as above
    window_class.hbrBackground = 0; // RA2: as above
    window_class.lpszMenuName = "ComboDropWin"; // RA2: as above
    window_class.lpszClassName = "ComboDropWin"; // RA2: as above
    RegisterClassA(&window_class); // RA2: as above
    memset(&window_class, 0, sizeof(window_class)); // RA2: as above
    window_class.style = 0x83; // RA2: as above
    window_class.lpfnWndProc = New_Edit_Window_Proc; // RA2: as above
    window_class.cbClsExtra = 0; // RA2: as above
    window_class.cbWndExtra = 0; // RA2: as above
    window_class.hInstance = ProgramInstance; // RA2: as above
    window_class.hIcon = 0; // RA2: as above
    window_class.hCursor = 0; // RA2: as above
    window_class.hbrBackground = 0; // RA2: as above
    window_class.lpszMenuName = 0; // RA2: as above
    window_class.lpszClassName = "NewEdit"; // RA2: as above
    RegisterClassA(&window_class); // RA2: as above
}

}


LRESULT __stdcall OwnerDraw::New_Edit_Window_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    return DefWindowProcA(window, message, wparam, lparam); // RA2: as above
}


namespace OwnerDraw { // RA2: as above

int __fastcall Draw_Text_In_Rect_Ex(Surface *surface, wchar_t const *text, RECT *area, BitFont *font, // RA2: as above
    unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10); // RA2: as above

HWND Combo_Owner; // RA2: as above
HWND Dropdown_Owner; // RA2: as above
HWND Dropdown_Window; // RA2: as above

struct ScrollBarInfo { // RA2: as above
    unsigned int cbSize; // RA2: as above
    unsigned int fMask; // RA2: as above
    int nMin; // RA2: as above
    int nMax; // RA2: as above
    unsigned int nPage; // RA2: as above
    int nPos; // RA2: as above
    int nTrackPos; // RA2: as above
};

struct ListItemData { // RA2: as above
    int Reserved_00; // RA2: as above
    int Reserved_04; // RA2: as above
    wchar_t *text; // RA2: as above
};

inline int __cdecl Get_Item_Text(HWND owner, int const &index, wchar_t *text, int size) // RA2: as above
{
    WNDPROC proc = 0; // RA2: as above
    Window_Proc_Table.getValue(owner, proc); // RA2: as above
    text[0] = 0; // RA2: as above
    int item = CallWindowProcA(proc, owner, 0x199, index, 0); // RA2: as above
    if (item == -1 || item == 0) { // RA2: as above
        item = CallWindowProcA(proc, owner, 0x150, index, 0); // RA2: as above
    }
    if (item != -1 && item != 0) { // RA2: as above
        wcsncpy(text, ((ListItemData *)item)->text, size); // RA2: as above
        text[size - 1] = 0; // RA2: as above
        return wcslen(text); // RA2: as above
    }
    return 0; // RA2: as above
}


LRESULT __stdcall Combo_Drop_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    static void *solid_brush = CreateSolidBrush(0x306030); // RA2: as above
	(void)solid_brush; // RA2: the reference text's own statement here: TibSun archive code/ownrdraw.cpp:263, ComboDropWinCtrlProc (adopted text)
    RECT rect;
    Get_Display_Rect(window, &rect);
    RECT client;
    GetClientRect(window, &client);
    Rect fill;
    RECT parent_rect;
    memset(&parent_rect, 0, sizeof(parent_rect));
    int scrollbar_width = 2 * Border_Width + 18; // RA2: as above
    int need_scrollbar = -1;
    int max_top = 0; // RA2: as above
    HWND parent = 0; // RA2: as above
    if (Combo_Owner) { // RA2: as above
        parent = GetParent(Combo_Owner); // RA2: as above
    }
    WinData *parent_data = 0; // RA2: as above
    if (parent) {
        Control_Table.getPointer((unsigned int const &)parent, &parent_data); // RA2: as above
        Get_Display_Rect(parent, &parent_rect);
    }
    Dropdown_Owner = parent; // RA2: as above
    Dropdown_Window = window; // RA2: as above
    WinData *data = 0; // RA2: as above
    Control_Table.getPointer((unsigned int const &)window, &data); // RA2: as above
    if (data == 0) { // RA2: as above
        DebugString("ComboBox dropdown windata = NULL\n");
    }
    WinData *master = 0; // RA2: as above
    if (Combo_Owner) { // RA2: as above
        Control_Table.getPointer((unsigned int const &)Combo_Owner, &master); // RA2: as above
    }
	if (message != CB_GETCOUNT && message != CB_GETITEMHEIGHT && message != WM_VSCROLL) { // RA2: as above
		int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0); // RA2: as above
		int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0); // RA2: as above
        if (item_height <= 1) {
            item_height = 1;
        }
        need_scrollbar = item_count * item_height > client.bottom - client.top; // RA2: as above
        max_top = item_count - (client.bottom - client.top) / item_height; // RA2: as above
        if (data) {
            if ((unsigned int)data->ScrollBar > 1) { // RA2: as above
                ScrollBarInfo info; // RA2: as above
                info.fMask = 5; // RA2: as above
                info.nMin = 0;
                info.nMax = max_top; // RA2: as above
                info.nPos = data->Position; // RA2: as above
                info.cbSize = 0x1c; // RA2: as above
				SendMessageA(data->ScrollBar, SBM_SETSCROLLINFO, 0, (LPARAM)&info); // RA2: as above
            }
            if (data->ScrollBar) { // RA2: as above
                BringWindowToTop(data->ScrollBar); // RA2: as above
            }
        }
    }
    switch (message) {
		case WM_ERASEBKGND: // RA2: as above
        return 0; // RA2: as above
		case WM_CREATE: { // RA2: as above
        SetCapture(window);
        Combo_Owner = *(HWND *)lparam; // RA2: as above
        char buffer[100]; // RA2: as above
        sprintf(buffer, "Dropd=%p  Master=%p  Dlg=%p", window, Combo_Owner, GetParent(Combo_Owner)); // RA2: as above
        DebugString(buffer); // RA2: as above
        SetTimer(window, 0, 20, 0); // RA2: as above
        return 0; // RA2: as above
    }
		case WM_NCDESTROY: // RA2: as above
        Dropdown_Window = 0; // RA2: as above
        Dropdown_Owner = 0; // RA2: as above
        ReleaseCapture();
        KillTimer(window, 0);
        break;
		case WM_VSCROLL: { // RA2: as above
			int top = SendMessageA(data->ScrollBar, SBM_GETPOS, 0, 0); // RA2: as above
			if (top != SendMessageA(window, CB_GETTOPINDEX, 0, 0)) { // RA2: as above
				SendMessageA(window, CB_SETTOPINDEX, top, 0); // RA2: as above
        }
        break;
    }
		case WM_TIMER: { // RA2: as above
        HWND capture = GetCapture(); // RA2: as above
        if (capture == window) { // RA2: as above
            ReleaseCapture();
            Sleep(0);
            SetCapture(capture); // RA2: as above
        }
        break;
    }
		case CB_GETTOPINDEX: // RA2: as above
        if (data) {
            return data->Position; // RA2: as above
        }
        break;
		case WM_MOUSEMOVE: { // RA2: as above
        int x = lparam & 0xffff; // RA2: as above
        int y = (unsigned int)lparam >> 16; // RA2: as above
        int index = SendMessageA(window, 0x4e8, 0, (y << 16) | x); // RA2: as above
        if (index != -1) {
            if (data->ItemState != index) { // RA2: as above
                InvalidateRect(window, 0, 0); // RA2: as above
            }
            data->ItemState = index; // RA2: as above
            HWND tip = GetDlgItem(parent, 0x695); // RA2: as above
            if (tip) { // RA2: as above
                WideWstring text; // RA2: as above
                text = Get_Item_Tip_Text(parent, (int)Combo_Owner, index); // RA2: as above
                if (wcscmp((wchar_t const *)text.get(), L"")) { // RA2: as above
                    SendMessageA(tip, 0x4b2, 0, (LPARAM)text.text()); // RA2: as above
                } else {
                    text = Get_Control_Tip_Text(parent, (int)Combo_Owner); // RA2: as above
                    if (wcscmp((wchar_t const *)text.get(), L"")) { // RA2: as above
                        SendMessageA(tip, 0x4b2, 0, (LPARAM)text.text()); // RA2: as above
                    }
                }
            }
        }
        return 0; // RA2: as above
    }
		case CB_SETTOPINDEX: { // RA2: as above
        int top = wparam; // RA2: as above
			int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0); // RA2: as above
			int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0); // RA2: as above
        if (!item_count || !item_height) {
            break;
        }
        int visible = (client.bottom - client.top) / item_height;
        if (top < 0) { // RA2: as above
            top = 0; // RA2: as above
        }
        int limit = item_count - visible; // RA2: as above
        if (limit > 0) { // RA2: as above
            if (top > limit) { // RA2: as above
                top = limit; // RA2: as above
            }
        } else {
            top = 0; // RA2: as above
        }
        if (top != data->Position) { // RA2: as above
            data->Position = top; // RA2: as above
            InvalidateRect(window, 0, 0); // RA2: as above
        }
        return 0; // RA2: as above
    }
		case WM_LBUTTONDOWN: // RA2: as above
		case WM_LBUTTONDBLCLK: { // RA2: as above
        int x = lparam & 0xffff; // RA2: as above
        int y = (unsigned int)lparam >> 16; // RA2: as above
        if (x > client.right && x < client.right + data->ScrollWidth && y > 0 && y < client.bottom) { // RA2: as above
            SendMessageA(data->ScrollBar, message, wparam, lparam); // RA2: as above
            break;
        }
        VocClass::PlayGlobal(Rule->GUIComboCloseSound, 1.0f, 0x2000, 0); // RA2: as above
        if (x >= 0 && y >= 0 && x <= client.right && y <= client.bottom) {
				int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0); // RA2: as above
				int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0); // RA2: as above
            int index = y / item_height;
            if (data) {
                index += data->Position; // RA2: as above
            }
            int clamped = index < 0 ? 0 : index;
            int max_index = item_count - 1; // RA2: as above
            if (max_index < clamped) {
                clamped = max_index;
            }
				SendMessageA(Combo_Owner, CB_SETCURSEL, clamped, 0); // RA2: as above
				int id = GetWindowLongA(Combo_Owner, GWL_ID); // RA2: as above
            if (need_scrollbar) {
                DestroyWindow(data->ScrollBar); // RA2: as above
            }
            ReleaseCapture();
				SendMessageA(Combo_Owner, CB_SHOWDROPDOWN, 0, 0); // RA2: as above
				PostMessageA(parent, WM_COMMAND, (id & 0xffff) | 0x10000, (LPARAM)Combo_Owner); // RA2: as above
            return 0; // RA2: as above
        }
        if (need_scrollbar) {
            DestroyWindow(data->ScrollBar); // RA2: as above
        }
        ReleaseCapture();
			SendMessageA(Combo_Owner, CB_SHOWDROPDOWN, 0, 0); // RA2: as above
        return 0; // RA2: as above
    }
    case 0x4e8: { // RA2: as above
        int x = lparam & 0xffff; // RA2: as above
        int y = (unsigned int)lparam >> 16; // RA2: as above
        int result = -1; // RA2: as above
        if (x >= 0 && y >= 0 && x < client.right && y < client.bottom) {
			int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0); // RA2: as above
			int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0); // RA2: as above
            int index = y / item_height + data->Position; // RA2: as above
            int clamped = index < 0 ? 0 : index;
            int max_index = item_count - 1; // RA2: as above
            if (max_index < clamped) {
                clamped = max_index;
            }
            result = clamped; // RA2: as above
        }
        return result; // RA2: as above
    }
    case 0x4ea: // RA2: as above
        return (LRESULT)Combo_Owner; // RA2: as above
    case 0x7e8: // RA2: as above
		data->ItemState = SendMessageA(Combo_Owner, CB_GETCURSEL, 0, 0); // RA2: as above
        return 0; // RA2: as above
	case WM_PAINT: { // RA2: as above
        BitFont *font = parent_data->Font; // RA2: as above
		int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0); // RA2: as above
		int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0); // RA2: as above
        int client_height = client.bottom - client.top;
        Rect dest_rect(rect.left, rect.top, rect.right - rect.left, client_height);
        int selected = data->ItemState; // RA2: as above
        Rect src;
        src.X = rect.left - parent_rect.left; // RA2: as above
        src.Y = rect.top - parent_rect.top; // RA2: as above
        src.Width = client.right; // RA2: as above
        src.Height = client.bottom; // RA2: as above
        if (data->Back == 0) { // RA2: as above
            Rect dst(0, 0, client.right, client.bottom);
            BSurface *surface = new BSurface(client.right, client.bottom, 2); // RA2: as above
            data->Back = surface; // RA2: as above
            Surface_Count++; // RA2: as above
            if (parent_data && parent_data->Back) { // RA2: as above
                surface->Blit_From(dst, *parent_data->Back, src, false, true); // RA2: as above
            }
            int total = client.right * client.bottom;
            unsigned short *pixels = (unsigned short *)surface->Lock(); // RA2: as above
            for (int i = 0; i < total; i++) { // RA2: as above
                unsigned short pixel = pixels[i]; // RA2: as above
                pixels[i] = Blend_Color(pixel, 0, data->TextStyle); // RA2: as above
            }
            surface->Unlock();
        }
        {
            Rect from; // RA2: as above
            from.X = 0; // RA2: as above
            from.Y = 0; // RA2: as above
            from.Width = dest_rect.Width; // RA2: as above
            from.Height = dest_rect.Height; // RA2: as above
            AlternateSurface->Blit_From(dest_rect, *data->Back, from, false, true); // RA2: as above
        }
        Rect frame(dest_rect); // RA2: as above
        int color = Color_To_Pixel(Default_Frame_Color); // RA2: as above
        if (master && master->IsAlternateColors) { // RA2: as above
            color = Color_To_Pixel(Alternate_Frame_Color); // RA2: as above
        }
        Draw_Frame(AlternateSurface, frame, 1, color); // RA2: as above
        if (need_scrollbar && data && data->ScrollBar) { // RA2: as above
            Rect outer(dest_rect); // RA2: as above
            outer.Width += scrollbar_width; // RA2: as above
            Draw_Frame(AlternateSurface, outer, 1, color); // RA2: as above
        }
        int first = data->Position; // RA2: as above
        Rect row; // RA2: as above
        for (int index = first; index < item_count; index++) { // RA2: as above
            row.X = rect.left; // RA2: as above
            int width = rect.right - rect.left; // RA2: as above
            row.Y = (index - first) * item_height + rect.top; // RA2: as above
            if ((index - first) * item_height + item_height > client_height) { // RA2: as above
                break;
            }
            wchar_t text[128]; // RA2: as above
            Get_Item_Text(Combo_Owner, index, text, 0x80); // RA2: as above
            if (index == selected) { // RA2: as above
                fill.X = row.X; // RA2: as above
                fill.Y = row.Y; // RA2: as above
                fill.Width = width; // RA2: as above
                fill.Height = item_height; // RA2: as above
                int fill_color; // RA2: as above
                if (master ? master->IsAlternateColors : false) { // RA2: as above
                    fill_color = Color_To_Pixel(Pressed_Fill_Color); // RA2: as above
                } else {
                    fill_color = Color_To_Pixel(Fill_Color); // RA2: as above
                }
                AlternateSurface->Fill_Rect(fill, fill_color);
            }
            RECT text_rect;
            text_rect.top = row.Y; // RA2: as above
            text_rect.left = row.X + 3; // RA2: as above
            text_rect.right = row.X + width; // RA2: as above
            text_rect.bottom = row.Y + item_height; // RA2: as above
            unsigned long text_color = Text_Color; // RA2: as above
            if (master && master->IsAlternateColors) { // RA2: as above
                text_color = Highlight_Text_Color; // RA2: as above
            }
            if (index < 50 && master && master->ItemColors[index] >= 0) { // RA2: as above
                text_color = master->ItemColors[index]; // RA2: as above
                if (master->IsItemFilled) { // RA2: as above
                    Rect fill2; // RA2: as above
                    fill2.X = row.X; // RA2: as above
                    fill2.Y = row.Y; // RA2: as above
                    fill2.Width = width; // RA2: as above
                    fill2.Height = item_height; // RA2: as above
                    Inflate_Rect(fill2, 2); // RA2: as above
                    AlternateSurface->Fill_Rect(fill2, Color_To_Pixel(text_color)); // RA2: as above
                }
            }
            int max_width = client.right - 20; // RA2: as above
            int length = wcslen(text); // RA2: as above
            int text_width = font->String_Pixel_Width(text, 0); // RA2: as above
            while (length && text_width > max_width) { // RA2: as above
                length--; // RA2: as above
                text[length] = 0; // RA2: as above
                wcscat(text, L"..."); // RA2: as above
                text_width = font->String_Pixel_Width(text, 0); // RA2: as above
            }
            Draw_Text_In_Rect_Ex(AlternateSurface, text, &text_rect, font, text_color, 4, 12, 0, 0, 0); // RA2: as above
        }
        AlternateSurface->Lock();
        VisibleSurface->Lock();
        Rect screen(dest_rect); // RA2: as above
        RECT window_rect;
        GetWindowRect(window, &window_rect);
        screen.X += window_rect.left - rect.left; // RA2: as above
        screen.Y += window_rect.top - rect.top; // RA2: as above
        VisibleSurface->Blit_From(screen, *AlternateSurface, dest_rect, false, true); // RA2: as above
        VisibleSurface->Unlock();
        AlternateSurface->Unlock();
        ValidateRect(window, 0);
        return 0; // RA2: as above
    }
	case WM_DESTROY: // RA2: as above
        Remove_Audio_File_Window(window); // RA2: as above
        break;
    }
    if (need_scrollbar == 1) {
        if (data && !data->ScrollBar) { // RA2: as above
            data->ScrollBar = (HWND)1; // RA2: as above
			GetWindowLongA(window, GWL_ID); // RA2: as above
            parent = GetParent(window);
            RECT parent_display;
            Get_Display_Rect(parent, &parent_display);
            RECT drop_display; // RA2: as above
            Get_Display_Rect(window, &drop_display); // RA2: as above
            int left = drop_display.left - parent_display.left; // RA2: as above
            int top = drop_display.top - parent_display.top; // RA2: as above
            bool alternate = false; // RA2: as above
            int x = left - scrollbar_width + client.right; // RA2: as above
            int y = top + client.top; // RA2: as above
            int height = drop_display.bottom - drop_display.top; // RA2: as above
            if (master) { // RA2: as above
                alternate = master->IsAlternateColors; // RA2: as above
            }
            HWND scroll = CreateWindowExA(0, "Scrollbar", 0, 0x50010001, x, y, scrollbar_width, height, parent, 0, // RA2: as above
                ProgramInstance, 0); // RA2: as above
            data->ScrollBar = scroll; // RA2: as above
            data->ScrollWidth = scrollbar_width; // RA2: as above
            Subclass_Control(scroll, 0); // RA2: as above
            WinData *scroll_data = 0; // RA2: as above
            Control_Table.getPointer((unsigned int const &)data->ScrollBar, &scroll_data); // RA2: as above
            scroll_data->ScrollOwner = window; // RA2: as above
            scroll_data->IsAlternateColors = data->IsAlternateColors; // RA2: as above
            ScrollBarInfo info; // RA2: as above
            info.nMax = max_top; // RA2: as above
            info.fMask = 5; // RA2: as above
            info.nMin = 0;
            info.nPos = data->Position; // RA2: as above
            info.cbSize = 0x1c; // RA2: as above
			SendMessageA(data->ScrollBar, SBM_SETSCROLLINFO, 0, (LPARAM)&info); // RA2: as above
            SendMessageA(data->ScrollBar, 0x4f1, 0, alternate); // RA2: as above
			SetWindowPos(window, 0, 0, 0, drop_display.right - drop_display.left - scrollbar_width, drop_display.bottom - drop_display.top, SWP_NOMOVE); // RA2: as above
			ShowWindow(data->ScrollBar, SW_SHOW); // RA2: as above
            BringWindowToTop(data->ScrollBar); // RA2: as above
            InvalidateRect(data->ScrollBar, 0, 0); // RA2: as above
            UpdateWindow(data->ScrollBar); // RA2: as above
            SendMessageA(data->ScrollBar, 0x4a9, (WPARAM)data->ScrollBar, 1); // RA2: as above
            SendMessageA(data->ScrollBar, 0x4d2, 0, 1); // RA2: as above
            RECT validate; // RA2: as above
            validate.left = left; // RA2: as above
            validate.top = top; // RA2: as above
            validate.right = left + client.right + 1; // RA2: as above
            validate.bottom = top + client.bottom + 1; // RA2: as above
            ValidateRect(parent, &validate); // RA2: as above
        }
    } else if (need_scrollbar == 0 && data && data->ScrollBar && !data->SkipCheckPaint) { // RA2: as above
        DestroyWindow(data->ScrollBar); // RA2: as above
        Restore_Child(data->ScrollBar, 0); // RA2: as above
        data->ScrollBar = 0; // RA2: as above
        data->ScrollWidth = 0; // RA2: as above
		SetWindowPos(window, 0, 0, 0, client.right - client.left + scrollbar_width, client.bottom - client.top, SWP_NOMOVE); // RA2: as above
        parent = GetParent(window);
        RECT validate_parent; // RA2: as above
        Get_Display_Rect(parent, &validate_parent); // RA2: as above
        RECT validate; // RA2: as above
        validate.left = rect.left - validate_parent.left; // RA2: as above
        validate.top = rect.top - validate_parent.top; // RA2: as above
        validate.right = validate.left + client.right + scrollbar_width; // RA2: as above
        validate.bottom = validate.top + client.bottom + 1; // RA2: as above
        ValidateRect(parent, &validate); // RA2: as above
    }
    return DefWindowProcA(window, message, wparam, lparam); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

BOOL __stdcall Replace_Edit(HWND window, LPARAM) // RA2: as above
{
    unsigned int style = GetWindowLongA(window, GWL_STYLE); // RA2: as above
	SetWindowLongA(window, GWL_USERDATA, 0); // RA2: as above
	if ((style & WS_VISIBLE) && !(style & WS_DISABLED)) { // RA2: as above
        char name[128]; // RA2: as above
        GetClassNameA(window, name, sizeof(name)); // RA2: as above
        if (strcmp(name, "Edit") == 0) { // RA2: as above
            HWND parent = GetParent(window);
            RECT rect;
            RECT parent_rect;
            Get_Display_Rect(window, &rect);
            Get_Display_Rect(parent, &parent_rect);
            int x = rect.left - parent_rect.left; // RA2: as above
            int y = rect.top - parent_rect.top; // RA2: as above
			DWORD list_style = (WS_CHILD | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT); // RA2: as above
			if (style & WS_TABSTOP) { // RA2: as above
				list_style = (WS_CHILD | WS_TABSTOP | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT); // RA2: as above
            }
            HWND list = CreateWindowExA(0, "ListBox", 0, list_style, x, y, // RA2: as above
                                        rect.right - rect.left, rect.bottom - rect.top, parent, 0, ProgramInstance, 0); // RA2: as above
            LONG *data = (LONG *)operator new(8); // RA2: as above
            data[0] = (LONG)"NewEdit"; // RA2: as above
            data[1] = GetWindowLongA(window, GWL_STYLE); // RA2: as above
			SetWindowLongA(list, GWL_USERDATA, (LONG)data); // RA2: as above
            EnableWindow(window, FALSE); // RA2: as above
            ShowWindow(window, SW_HIDE); // RA2: as above
            EnableWindow(list, TRUE); // RA2: as above
            ShowWindow(list, SW_SHOWNORMAL); // RA2: as above
			LONG id = GetWindowLongA(window, GWL_ID); // RA2: as above
			SetWindowLongA(window, GWL_ID, GetWindowLongA(list, GWL_ID)); // RA2: as above
			SetWindowLongA(list, GWL_ID, id); // RA2: as above
        }
    }
    return TRUE; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above


bool __fastcall Subclass_Dialog(HWND window, LPARAM lparam) // RA2: as above
{
    Register_Window_Classes(); // RA2: as above
    Current_Dialog = window; // RA2: as above
    EnumChildWindows(window, Set_Paint_Disabled, 1); // RA2: as above
    Set_Paint_Disabled(window, 1); // RA2: as above
    EnumChildWindows(window, Replace_Edit, 1); // RA2: as above
    EnumChildWindows(window, Subclass_Control, lparam); // RA2: as above
    Subclass_Control(window, lparam); // RA2: as above
    EnumChildWindows(window, Set_Paint_Disabled, 0); // RA2: as above
    Set_Paint_Disabled(window, 0); // RA2: as above
    return true; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

BOOL __stdcall Set_Paint_Disabled(HWND window, LPARAM lparam) // RA2: as above
{
    WinData *data = 0; // RA2: as above
    if (!Control_Table.getPointer((unsigned int const &)window, &data)) { // RA2: as above
        WinData temp;
        Control_Table.add((unsigned int const &)window, temp); // RA2: as above
        Control_Table.getPointer((unsigned int const &)window, &data); // RA2: as above
    }
    data->SkipCheckPaint = lparam; // RA2: as above
    return TRUE; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

LRESULT __stdcall List_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Combo_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall New_Edit_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Static_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
LRESULT __stdcall Owner_Button_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above

BOOL __stdcall Subclass_Control(HWND window, LPARAM lparam) // RA2: as above
{
    char class_name[128];
    GetClassNameA(window, class_name, sizeof(class_name)); // RA2: as above
    LONG style = GetWindowLongA(window, GWL_STYLE); // RA2: as above
    RECT rect1;
    Get_Display_Rect(window, &rect1);
    RECT rect2;
    GetClientRect(window, &rect2);
    Add_Audio_File_Window(window); // RA2: as above
    Init_Default_Colors(); // RA2: as above
    if (!Masks_Ready) { // RA2: as above
        Init_Color_Masks(); // RA2: as above
        Load_Dialog_Images(); // RA2: as above
        Masks_Ready = 1; // RA2: as above
    }
    WNDPROC custom_proc = 0; // RA2: as above
    int kind = 0xb; // RA2: as above
	LONG * user_data = (LONG *)GetWindowLongA(window, GWL_USERDATA); // RA2: as above
    if (user_data) { // RA2: as above
        strcpy(class_name, (char const *)user_data[0]); // RA2: as above
    }
    if (strcmp(class_name, "ScrollBar") == 0) { // RA2: as above
        custom_proc = Scroll_Bar_Proc; // RA2: as above
        kind = 8; // RA2: as above
    } else if (strcmp(class_name, "ListBox") == 0) { // RA2: as above
        custom_proc = List_Proc; // RA2: as above
        kind = 4; // RA2: as above
    } else if (strcmp(class_name, "ComboBox") == 0) { // RA2: as above
        custom_proc = Combo_Proc; // RA2: as above
        kind = 3; // RA2: as above
    } else if (strcmp(class_name, "msctls_trackbar32") == 0) { // RA2: as above
        custom_proc = Trackbar_Proc; // RA2: as above
        kind = 7; // RA2: as above
    } else if (strcmp(class_name, "msctls_progress32") == 0) { // RA2: as above
        custom_proc = Progress_Proc; // RA2: as above
        kind = 6; // RA2: as above
    } else if (strcmp(class_name, "NewEdit") == 0) { // RA2: as above
        custom_proc = New_Edit_Proc; // RA2: as above
        kind = 1; // RA2: as above
    } else if (strcmp(class_name, "Edit") == 0) { // RA2: as above
        custom_proc = Edit_Control_Proc; // RA2: as above
        kind = 1; // RA2: as above
    } else if (strcmp(class_name, "Static") == 0) { // RA2: as above
        custom_proc = Static_Proc; // RA2: as above
        kind = 2; // RA2: as above
    } else if (strcmp(class_name, "SysTabControl32") == 0) { // RA2: as above
        custom_proc = Tab_Proc; // RA2: as above
        kind = 0xa; // RA2: as above
    } else if (strcmp(class_name, "Button") == 0) { // RA2: as above
		if ((style & (unsigned char)BS_GROUPBOX) == (unsigned char)BS_GROUPBOX) { // RA2: as above
            custom_proc = Group_Box_Proc; // RA2: as above
		} else if ((style & (unsigned char)BS_OWNERDRAW) == (unsigned char)BS_OWNERDRAW) { // RA2: as above
            custom_proc = Owner_Button_Proc; // RA2: as above
		} else if ((style & (unsigned char)BS_AUTOCHECKBOX) == (unsigned char)BS_AUTOCHECKBOX) { // RA2: as above
            custom_proc = Checkbox_Proc; // RA2: as above
		} else if ((style & (unsigned char)BS_AUTORADIOBUTTON) == (unsigned char)BS_AUTORADIOBUTTON) { // RA2: as above
            custom_proc = Button_Proc; // RA2: as above
        }
        kind = 0; // RA2: as above
    } else if (strcmp(class_name, "msctls_hotkey32") == 0) { // RA2: as above
        custom_proc = Hotkey_Proc; // RA2: as above
        kind = 9; // RA2: as above
    } else {
        custom_proc = Default_Proc; // RA2: as above
    }
	WNDPROC original_proc = (WNDPROC)SetWindowLongA(window, GWL_WNDPROC, (LONG)Control_Proc); // RA2: as above
    if (!Window_Table.contains(window)) { // RA2: as above
        Window_Table.add(window, custom_proc); // RA2: as above
    }
    if (!Window_Proc_Table.contains(window)) { // RA2: as above
        Window_Proc_Table.add(window, original_proc); // RA2: as above
    }
    WinData *data = 0; // RA2: as above
    if (!Control_Table.getPointer((unsigned int const &)window, &data)) { // RA2: as above
        WinData temp;
        Control_Table.add((unsigned int const &)window, temp); // RA2: as above
        Control_Table.getPointer((unsigned int const &)window, &data); // RA2: as above
    }
    data->SubclassParam = lparam; // RA2: as above
    data->SkipTests = kind; // RA2: as above
    if (user_data) { // RA2: as above
        data->UserDataWord = user_data[1]; // RA2: as above
    }
    WNDPROC proc = 0; // RA2: as above
    Window_Proc_Table.getValue(window, proc); // RA2: as above
    char text[2048];
    text[0] = 0; // RA2: as above
    CallWindowProcA(proc, window, WM_GETTEXT, sizeof(text), (LPARAM)text); // RA2: as above
    if (text[0]) { // RA2: as above
        data->Set_Wide_Text(StringTable::LoadString(text, 0, "D:\\Ra2\\ownrdraw.cpp", 0x1a47)); // RA2: as above
    } else {
        data->Set_Wide_Text(0); // RA2: as above
    }
    data->Reserved_2C = 0; // RA2: as above
	SendMessageA(window, OD_SUBCLASSED, 0, 0); // RA2: as above
    return TRUE; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above


struct WindowData { // RA2: as above
    ~WindowData() {} // RA2: as above
};


BOOL __fastcall Restore_Window(HWND window) // RA2: as above
{
    EnumChildWindows(window, Restore_Child, 0); // RA2: as above
    Restore_Child(window, 0); // RA2: as above
    return TRUE; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

BOOL __stdcall Restore_Child(HWND window, LPARAM lparam) // RA2: as above
{
    Window_Proc_Table.remove(window); // RA2: as above
    Control_Table.remove((unsigned int const &)window); // RA2: as above
    Window_Table.remove(window); // RA2: as above
	void * data = (void *)GetWindowLongA(window, GWL_USERDATA); // RA2: as above
    if (data != 0) { // RA2: as above
        operator delete(data); // RA2: as above
    }
	SetWindowLongA(window, GWL_USERDATA, 0); // RA2: as above
    Remove_Audio_File_Window(window); // RA2: as above
    return 1; // RA2: as above
}

}


OwnerDraw::Tooltip ODTooltip; // RA2: 0x00A737F0, the dialogs' one tooltip (its initializer comes after the dictionaries')

namespace OwnerDraw { // RA2: as above

OwnerDraw::Tooltip::Tooltip() // RA2: as above
{
    bounds.X = 0; // RA2: as above
    bounds.Y = 0; // RA2: as above
    bounds.Width = 0; // RA2: as above
    bounds.Height = 0; // RA2: as above
    background = 0; // RA2: as above
    text[0] = 0; // RA2: as above
    isActive = 0; // RA2: as above
    isHidden = 0; // RA2: as above
    window = 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above
bool __fastcall Start_Tooltip(Rect const &area, unsigned short const *text, HWND window) // RA2: as above
{
    End_Tooltip(); // RA2: as above
    ODLastTooltipTime = time(0); // RA2: as above
    swprintf(ODTooltip.text, StringTable::LoadString("GUI:ToolTip", 0, "D:\\Ra2\\ownrdraw.cpp", 0x1a88)); // RA2: as above
    if (text) wcscpy(ODTooltip.text, text); // RA2: as above
    ODTooltip.bounds = area; // RA2: as above
    ODTooltip.isActive = 1; // RA2: as above
    ODTooltip.window = window;
    return Show_Tooltip(true); // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above
bool __fastcall Show_Tooltip(bool save_background) // RA2: as above
{
    if (!ODTooltip.isActive) return false; // RA2: as above
    if (save_background) {
        if (ODTooltip.background) delete ODTooltip.background; // RA2: as above
        ODTooltip.background = 0; // RA2: as above
        Surface *background = new BSurface(ODTooltip.bounds.Width, ODTooltip.bounds.Height, 2); // RA2: as above
        ODTooltip.background = background; // RA2: as above
        Rect destination(0, 0, ODTooltip.bounds.Width, ODTooltip.bounds.Height); // RA2: as above
        background->Blit_From(destination, *VisibleSurface, ODTooltip.bounds, false, true); // RA2: as above
    }
    ODTooltip.isHidden = 0; // RA2: as above
    Rect area = ODTooltip.bounds; // RA2: as above
    int color = Color_To_Pixel(Dark_Brown_Color); // RA2: as above
    VisibleSurface->Lock();
    VisibleSurface->Fill_Rect(area, color); // RA2: as above
    VisibleSurface->Unlock();
    Shade_Edges(area, *VisibleSurface, 8, 4); // RA2: as above
    OD_Draw_Text(Text_Color, 0, area, ODTooltip.text, // RA2: as above
        wcslen(ODTooltip.text), 1, 1, VisibleSurface, 0); // RA2: as above
    ++area.X; // RA2: as above
    ++area.Y; // RA2: as above
    area.Width -= 2; // RA2: as above
    area.Height -= 2; // RA2: as above
    Draw_Frame(VisibleSurface, area, 1, -1); // RA2: as above
    return true; // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above

bool Hide_Tooltip() // RA2: as above
{
    if (!ODTooltip.isActive) { // RA2: as above
        return false; // RA2: as above
    }
    if (ODTooltip.isHidden == 1) { // RA2: as above
        return false; // RA2: as above
    }
    if (!ODTooltip.background) { // RA2: as above
        return false; // RA2: as above
    }
    Rect area = ODTooltip.bounds; // RA2: as above
    VisibleSurface->Blit_From(area, *ODTooltip.background, Rect(0, 0, area.Width, area.Height), // RA2: as above
                                      false, true); // RA2: as above
    ODTooltip.isHidden = 1; // RA2: as above
    return true; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool End_Tooltip() // RA2: as above
{
    if (!ODTooltip.isActive) { // RA2: as above
        return false; // RA2: as above
    }
    if (ODTooltip.isHidden != 1 && ODTooltip.background) { // RA2: as above
            Rect area = ODTooltip.bounds; // RA2: as above
            VisibleSurface->Blit_From(area, *ODTooltip.background, Rect(0, 0, area.Width, area.Height), // RA2: as above
                                              false, true); // RA2: as above
            ODTooltip.isHidden = 1; // RA2: as above
    }
    if (ODTooltip.background) { // RA2: as above
        delete ODTooltip.background;
    }
    ODTooltip.background = 0; // RA2: as above
    ODTooltip.isActive = 0; // RA2: as above
    ODTooltip.isHidden = 0; // RA2: as above
    return true; // RA2: as above
}

}


typedef ArrayList<HWND> ModalWindowList; // RA2: as above


int __fastcall IME_Get_Result(void); // RA2: as above
void IME_Cancel_Composition(); // RA2: as above
BOOL __fastcall IME_Service_Message(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: as above
namespace OwnerDraw { // RA2: as above
void __fastcall WideToCharString(char * dest, wchar_t const * src, int count); // RA2: 0x6FA0F0
}
void __fastcall Inflate_Rect(Rect & rect, int dx, int dy); // RA2: as above
int __fastcall Hash_CtrlMsg(int *); // RA2: as above
namespace OwnerDraw { // RA2: as above
LRESULT __stdcall Owner_Button_Proc(HWND, UINT, WPARAM, LPARAM); // RA2: as above
LRESULT __stdcall List_Proc(HWND, UINT, WPARAM, LPARAM); // RA2: as above
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (005ee460 has no row); 4898 bytes differ against the frozen image bound (image_bounds.csv)
LRESULT __stdcall Control_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
	static POINT min_update_rect = {0xFFFFFF, 0xFFFFFF};
	static POINT max_update_rect;
	static int num_rect_updates;

	/*
	 * Tracks whether the game window had focus on the previous call so the
	 * controls can be refreshed when focus is regained.
	 */
	static bool was_in_focus = true;

	/*
	 * Set to 1 around the SetWindowPos() call issued by the OD_SETTOP handler,
	 * and checked by the WM_WINDOWPOSCHANGING handler so the engine does not
	 * fight its own z-order change.
	 */
	static char in_programmatic_reorder;

	if (message == WM_SETCURSOR) { // RA2: as above
		return(1);
	}
    if (IME_Service_Message(window, message, wparam, lparam)) return IME_Get_Result(); // RA2: as above

    WNDPROC specific_proc = 0; // RA2: as above
	::Window_Table.getValue(window, specific_proc); // RA2: as above

	WNDPROC original_proc = 0; // RA2: as above
	::Window_Proc_Table.getValue(window, original_proc); // RA2: as above

	if (message == WM_SYSKEYUP && wparam == 0x9) { // RA2: as above
		SendMessageA(MainWindow, 0x105, 0x9, lparam); // RA2: as above
	}

	LRESULT result = 0;
	BOOL show_tooltip = 0; // RA2: as above
    BOOL allow_proc = 1; // RA2: as above
	int anim_state = 0;

	static Dictionary<MessageKey, bool> ctrlmessages((unsigned int(__fastcall *)(MessageKey &))Hash_CtrlMsg); // RA2: as above

	MessageKey key; // RA2: as above
	key.Window = (int)window; // RA2: as above
	key.Message = message; // RA2: as above

	RECT display_rect;
	Get_Display_Rect(window, &display_rect);
	RECT window_rect;
	GetWindowRect(window, &window_rect);
	int offset_x = window_rect.left - display_rect.left;
	int offset_y = window_rect.top - display_rect.top;


	static ArrayList<HWND> hwndarray;

	if (hwndarray.length() != 0) {
		HWND topwindow = 0; // RA2: as above
		hwndarray.get(topwindow, hwndarray.length() - 1); // RA2: as above

	/*
	 * When a window has been pushed to the top (modal) via OD_SETTOP, swallow
	 * mouse messages that are not directed at it or one of its children.
	 */
		BOOL allow = 0; // RA2: as above
		BOOL forward = 0; // RA2: as above
		if (GetParent(window) == 0) { // RA2: as above
			allow = 1; // RA2: as above
		}
		if (GetWindowLongA(window, -12) <= 0) { // RA2: as above
			allow = 1; // RA2: as above
		}

		HWND parent = window;
		while (parent != 0) { // RA2: as above
			if (parent == topwindow) {
				allow = 1; // RA2: as above
				break;
			}
			parent = GetParent(parent);
		}

		if (message < 0x200 || message > 0x209) { // RA2: as above
			forward = 1; // RA2: as above
		}
		if (message >= 0xa0 && message <= 0xa9) { // RA2: as above
			forward = 0; // RA2: as above
		}
		if (message >= 0x100 && message <= 0x108) { // RA2: as above
			forward = 0; // RA2: as above
		}
		if (message == WM_SYSKEYUP || message == WM_SYSKEYDOWN || message == WM_SYSCOMMAND || message == WM_SYSCHAR) { // RA2: as above
			forward = 1; // RA2: as above
		}
		if (message == 0x49b || message == WM_TIMER || message == 0x4ad) { // RA2: as above
			forward = 0; // RA2: as above
		}
		if (!allow && !forward) {
			return(0);
		}
	}

	/*
	 * Guard against re-entrant processing of the same message for the same
	 * window. A handful of messages are allowed to re-enter.
	 */
	if (ctrlmessages.contains(key)) {
		if (message != WM_COMMAND && message != WM_SYSKEYDOWN && message != WM_SYSKEYUP // RA2: as above
			&& message != WM_SYSCOMMAND && message != WM_SYSCHAR) { // RA2: as above
			return(0);
		}
	}

	bool processing = true;
	ctrlmessages.remove(key);
	ctrlmessages.add(key, processing);

	RECT client_rect;
	GetClientRect(window, &client_rect);
	RECT disp_rect;
	Get_Display_Rect(window, &disp_rect);

	bool in_focus = GameInFocus;

	bool is_paint = false;
	if (message == WM_PAINT) { // RA2: as above
		is_paint = true;
		if (!in_focus) {
			ValidateRect(window, 0);
			ctrlmessages.remove(key);
			return(0);
		}
	}

	if (in_focus == true && !was_in_focus) {
		InvalidateRect(MainWindow, 0, 0); // RA2: as above
		in_focus = GameInFocus;
	}
	was_in_focus = in_focus;

	RECT update_rect;
	if (is_paint) {
		++num_rect_updates;
		GetUpdateRect(window, &update_rect, 0); // RA2: as above
		update_rect.left += disp_rect.left;
		update_rect.right += disp_rect.left;
		update_rect.top += disp_rect.top;
		update_rect.bottom += disp_rect.top;
	}

	WinData *data = Get_Record(window); // RA2: as above

	if (message == 0x4a0) { // RA2: as above
		result = (data && data->ScrollBar != 0); // RA2: as above
		goto cleanup;
	}

	/*
	 * Save the device context's current font / colors so they can be
	 * restored by OD_RESTOREDC.
	 */
	if (message == 0x49f && data) { // RA2: as above
		HDC hdc = (HDC)lparam;
		HGDIOBJ old = SelectObject(hdc, GetStockObject(0xd)); // RA2: as above
		(*(HFONT *)((char *)data + 0x1E8)) = (HFONT)old; // RA2: as above
		SelectObject(hdc, old);
		(*(int *)((char *)data + 0x1EC)) = GetBkMode(hdc); // RA2: as above
		(*(COLORREF *)((char *)data + 0x1F0)) = GetBkColor(hdc); // RA2: as above
		(*(COLORREF *)((char *)data + 0x1F4)) = GetTextColor(hdc); // RA2: as above
		result = 1;
		goto cleanup;
	}

	/*
	 * Restore the device context state saved by OD_SAVEDC.
	 */
	if (message == 0x49e && data) { // RA2: as above
		HDC hdc = (HDC)lparam;
		SelectObject(hdc, (*(HFONT *)((char *)data + 0x1E8))); // RA2: as above
		SetBkMode(hdc, (*(int *)((char *)data + 0x1EC))); // RA2: as above
		SetBkColor(hdc, (*(COLORREF *)((char *)data + 0x1F0))); // RA2: as above
		SetTextColor(hdc, (*(COLORREF *)((char *)data + 0x1F4))); // RA2: as above
		result = 1;
		goto cleanup;
	}

	/*
	 * Toggle paint suppression for this control (and its linked window).
	 */
	if (message == 0x49d && data) { // RA2: as above
		{
			HWND linked = (HWND)data->ScrollBar; // RA2: as above
			result = data->SkipCheckPaint; // RA2: as above
			data->SkipCheckPaint = lparam; // RA2: as above
			if (linked != 0) { // RA2: as above
				WinData * linkeddata = Get_Record(linked); // RA2: as above
				if (linkeddata != 0) { // RA2: as above
					linkeddata->SkipCheckPaint = lparam; // RA2: as above
				}
			}
		}

		goto call_custom_proc;
	}

	/*
	 * Push a window to the top of the modal stack.
	 */
	if (message == 0x4a9) { // RA2: as above
		HWND topwindow = 0; // RA2: as above
		hwndarray.get(topwindow, hwndarray.length() - 1); // RA2: as above
		result = (LRESULT)topwindow;

		HWND target = window;
		if (wparam) {
			target = (HWND)wparam;
		}
		int scan = 0;
		for (int index = 0; index < hwndarray.length(); index++) {
			hwndarray.get(topwindow, scan);
			if (topwindow == target) {
				hwndarray.remove(scan);
			} else {
				scan++;
			}
		}

		if (lparam) {
			hwndarray.addTail(target);
			in_programmatic_reorder = 1;
			SetWindowPos(target, 0, 0, 0, 0, 0, 0x1 | 0x2); // RA2: as above
			in_programmatic_reorder = 0;
		}
		goto cleanup;
	}

	if (hwndarray.length() != 0) {

		/*
		 * Keep the modal window pinned on top when Windows tries to reorder it.
		 */
		if (message == WM_WINDOWPOSCHANGING) { // RA2: as above
			if (in_programmatic_reorder != 1) {
				HWND found = 0; // RA2: as above
				for (int index = 0; index < hwndarray.length(); index++) {
					hwndarray.get(found, index);
					if (found == window) {
						int *wp = (int *)lparam; // RA2: as above
						if (index == hwndarray.length() - 1 && wp[1] != 0 && (wp[6] & 0x4) == 0) { // RA2: as above
							wp[1] = 0; // RA2: as above
							InvalidateRect(window, 0, 0); // RA2: as above
							result = 0;
						} else {
							wp[6] |= 0x200 | 0x4; // RA2: as above
							InvalidateRect(window, 0, 0); // RA2: as above
							result = 0;
						}
						goto cleanup;
					}
				}
			}
			goto call_custom_proc;
		}

		/*
		 * Drop a destroyed window from the modal stack.
		 */
		if (message == WM_DESTROY) { // RA2: as above
			HWND found = 0; // RA2: as above
			int scan = 0;
			for (int index = 0; index < hwndarray.length(); index++) {
				hwndarray.get(found, scan);
				if (found == window) {
					hwndarray.remove(scan);
				} else {
					scan++;
				}
			}
		}
	}

	/*
	 * Tear down the tooltip when its owning window is destroyed, hidden, or
	 * loses focus.
	 */
	if (window == (HWND)ODTooltip.window // RA2: as above
		&& (message == WM_NCDESTROY || message == WM_SHOWWINDOW || message == WM_KILLFOCUS) // RA2: as above
		&& ODTooltip.isActive) {
		End_Tooltip(); // RA2: as above
	}

	if (message == WM_ERASEBKGND) { // RA2: as above
		result = 1;
		goto cleanup;
	} else if (message == WM_SETFOCUS) { // RA2: as above
		if (specific_proc == Owner_Button_Proc || specific_proc == List_Proc) { // RA2: as above
			SetFocus((HWND)wparam);
		}
		if (data && !data->ImeState) { IME_Cancel_Composition(); data->ImeState = 1; } // RA2: as above
	} else if (message == WM_KILLFOCUS) { // RA2: as above
		data->ImeState = 0; // RA2: as above
	} else if (message == WM_SHOWWINDOW) { // RA2: as above
		if (wparam == 0 && data) { // RA2: as above
			(*(int *)((char *)data + 0x1FC)) = 0; // RA2: as above
		}
	}
	if (data) {
		if (message == OD_SETIMAGE) { // RA2: as above
			result = (LRESULT)data->Image; // RA2: as above
			data->Image = (Surface *)lparam; // RA2: as above
			goto cleanup;
		} else if (message == OD_SETALTIMAGE) { // RA2: as above
			result = (LRESULT)data->AlternateImage; // RA2: as above
			data->AlternateImage = (Surface *)lparam; // RA2: as above
			goto cleanup;
		} else if (message == 0x49a) { // RA2: as above
			result = (*(int *)((char *)data + 0x24)); // RA2: as above
			(*(int *)((char *)data + 0x24)) = lparam; // RA2: as above
			goto cleanup;
		}
		if ((*(int *)((char *)data + 0x24)) && (message == WM_TIMER || (message >= 0x200 && message <= 0x209))) { // RA2: as above
			RECT corner; // RA2: as above
			GetWindowRect(window, &corner); // RA2: as above
			WindowFromPoint(*(POINT *)&corner); // RA2: as above
		}
	}
    if (message == 0xC) allow_proc = false; // RA2: as above
    else if (message == 0xD) { allow_proc = false; result = 0; } // RA2: as above
    else if (message == 0x189) { allow_proc = false; result = 0; } // RA2: as above
    else if (message == 0x143 || message == 0x14C || message == 0x158 || message == 0x14D || message == 0x14A || message == 0x148 || message == 0x180 || message == 0x18F || message == 0x1A2 || message == 0x18C || message == 0x181) { allow_proc = false; result = -1; } // RA2: as above
    if (data) {
        if (message == 0x4CE) { allow_proc = false; result = data->Reserved_2C == 0; } // RA2: as above
        else if (message == 0x4B5) { // RA2: as above
            char *destination = (char *)lparam; // RA2: as above
            if (destination) { // RA2: as above
                *destination = 0;
                if (data->Text) { // RA2: as above
                    WideToCharString(destination, data->Text, wparam); // RA2: as above
                    if (wparam > 0) destination[wparam - 1] = 0; // RA2: as above
                }
            }
        } else if (message == 0x4B3) { // RA2: as above
            wchar_t *destination = (wchar_t *)lparam; // RA2: as above
            if (destination) { // RA2: as above
                *destination = 0;
                if (data->Text) { // RA2: as above
                    wcsncpy(destination, data->Text, wparam); // RA2: as above
                    if ((int)wparam > 0) destination[wparam - 1] = 0; // RA2: as above
                }
            }
        } else if (message == 0x4EB) { // RA2: as above
            *(int *)((char *)data + 0x50) = lparam;
        } else if (message == 0x4B4) { // RA2: as above
            data->Set_Text((char const *)lparam); // RA2: as above
            data->Reserved_2C = 1; // RA2: as above
        } else if (message == 0x4D1) { // RA2: as above
            result = *(int *)((char *)data + 0x30); // RA2: as above
            *(int *)((char *)data + 0x30) = wparam;
            allow_proc = false; // RA2: as above
        } else if (message == 0x4B2) { // RA2: as above
            wchar_t const *text = data->Text; // RA2: as above
            wchar_t const *new_text = (wchar_t const *)lparam; // RA2: as above
            bool either = text != 0 || new_text != 0; // RA2: as above
            bool added = text == 0 && new_text != 0; // RA2: as above
            bool removed = text != 0 && new_text == 0; // RA2: as above
            bool changed = added || removed || (either && wcscmp(text, new_text) != 0); // RA2: as above
            data->Set_Wide_Text((wchar_t const *)lparam); // RA2: as above
            data->Reserved_2C = 0; // RA2: as above
            if (changed && data->PictureMode == 1 && data->Reserved_94[0xA8 - 0x94]) { // RA2: as above
                KillTimer(window, 0);
                data->Reserved_94[0xA8 - 0x94] = 0; // RA2: as above
                SendMessageA(window, 0x4EE, 0, 0); // RA2: as above
            }
        }
    }
	if (message == WM_MOUSEMOVE) { // RA2: as above
        HWND parent = GetParent(window);
        HWND status = GetDlgItem(parent, 0x695); // RA2: as above
        if (status) { // RA2: as above
            WideWstring text; // RA2: as above
            unsigned int mx = (unsigned short)lparam; // RA2: as above
            unsigned int my = (unsigned short)((unsigned int)lparam >> 16); // RA2: as above
            text = Get_Item_Tip_Text_At(parent, window, mx, my); // RA2: as above
            if (wcscmp(text.get(), (wchar_t const *)L"") == 0) { // RA2: as above
                text = Get_Control_Tip_Text(parent, (int)window); // RA2: as above
                if (wcscmp(text.get(), (wchar_t const *)L"") == 0) { // RA2: as above
					text = Control_Tooltip((unsigned int)parent, window); // RA2: as above
                }
            }
            SendMessageA(status, 0x4B2, 0, (LPARAM)text.text()); // RA2: as above
        }
    }

	if (is_paint && data) { // RA2: as above

		/*
		 * Paint path: when painting is disabled, just tell the control to repaint
		 * its frame.
		 */
		if (data->SkipCheckPaint) { // RA2: as above
			ValidateRect(window, 0);
			result = CallWindowProcA(specific_proc, window, 0x4a5, wparam, lparam); // RA2: as above
		} else {
			Rect sibling_rect;
			RECT intersect;

			/*
			 * If this paint overlaps the tooltip, hide the tooltip first and re-show it
			 * once painting is finished.
			 */
			{
				RECT tooltip_rect;
				tooltip_rect.left = ODTooltip.bounds.X;
				tooltip_rect.right = ODTooltip.bounds.Width + ODTooltip.bounds.X + 1;
				tooltip_rect.top = ODTooltip.bounds.Y;
				tooltip_rect.bottom = ODTooltip.bounds.Height + ODTooltip.bounds.Y + 1;
				if (num_rect_updates == 1) {
					RECT intersect;
					if (IntersectRect(&intersect, &disp_rect, &tooltip_rect)) {
						if (ODTooltip.isActive && ODTooltip.isHidden != 1 && ODTooltip.background != 0) { // RA2: as above
							Rect drect = ODTooltip.bounds;
							Rect srect(0, 0, ODTooltip.bounds.Width, ODTooltip.bounds.Height);
							VisibleSurface->Blit_From(drect, *ODTooltip.background, srect, false, true); // RA2: as above
							ODTooltip.isHidden = true;
							show_tooltip = true;
						}
					}
				}
			}

			if (min_update_rect.x >= disp_rect.left) {
				min_update_rect.x = disp_rect.left;
			}
			if (min_update_rect.y >= disp_rect.top) {
				min_update_rect.y = disp_rect.top;
			}
			if (max_update_rect.x <= disp_rect.right) {
				max_update_rect.x = disp_rect.right;
			}
			if (max_update_rect.y <= disp_rect.bottom) {
				max_update_rect.y = disp_rect.bottom;
			}

			/*
			 * Walk up to the owning dialog window (the first ancestor that draws its own
			 * background) and read its animation state.
			 */
			HWND owner = window; // RA2: as above
			while (owner != 0) { // RA2: as above
				if (GetWindowLongA(owner, 0x4) != 0) { // RA2: as above
					break;
				}
				owner = GetParent(owner);
			}

			WinData *ownerdata = 0; // RA2: as above
			if (owner != 0) { // RA2: as above
				ownerdata = Get_Record(owner); // RA2: as above
			}

			int state;
			if (owner == window) {
				if ((*(int *)((char *)ownerdata + 0x1FC)) < 1) { // RA2: as above
					state = 1;
					anim_state = 1;
					goto call_proc_paint;
				}
				state = (*(int *)((char *)ownerdata + 0x1FC)); // RA2: as above
			} else {
				if (ownerdata == 0) { // RA2: as above
					goto skip_paint; // RA2: as above
				}
				state = (*(int *)((char *)ownerdata + 0x1FC)); // RA2: as above
			}

			anim_state = state;
			if (state < 1) {
			skip_paint: // RA2: as above
				state = anim_state;
				ValidateRect(window, 0);
				result = 1;
			} else {

			call_proc_paint:
				result = CallWindowProcA(specific_proc, window, message, wparam, lparam); // RA2: as above
				if (ownerdata != 0) { // RA2: as above
					(*(int *)((char *)ownerdata + 0x1FC)) = state; // RA2: as above
				}

				{
					ArrayList<HWND> children;
					EnumChildWindows(window, (WNDENUMPROC)Add_Window, (LPARAM)&children); // RA2: as above

					HWND combo_owner = 0; // RA2: as above
					HWND child = 0; // RA2: as above
					for (int index = 0; index < children.length(); index++) {
						children.get(child, index);

						WNDPROC childproc = 0; // RA2: as above
						::Window_Proc_Table.getValue(child, childproc); // RA2: as above

						if (childproc != (WNDPROC)Combo_Drop_Proc) { // RA2: as above
							InvalidateRect(child, 0, 0); // RA2: as above
							UpdateWindow(child);
						} else {
							combo_owner = child;
						}
					}

					if (combo_owner != 0) { // RA2: as above
						InvalidateRect(combo_owner, 0, 0); // RA2: as above
						UpdateWindow(combo_owner);
					} else if (Dropdown_Window != 0) { // RA2: as above
						if (Dropdown_Owner == owner) { // RA2: as above
							Rect drop_rect;
							Get_Display_Rect(Dropdown_Window, (RECT *)&drop_rect); // RA2: as above
							RECT intersect;
							if (IntersectRect(&intersect, &disp_rect, (const RECT *)&drop_rect)) {
								InvalidateRect(Dropdown_Window, 0, 0); // RA2: as above
								UpdateWindow(Dropdown_Window); // RA2: as above
							}
						}
					}
				}

				state = anim_state;
			}

		finalize_paint:
			if (state > 0) {
				HWND sibling = owner;
				while (window == owner || num_rect_updates == 1) {
					if (sibling == 0) { // RA2: as above
						break;
					}
					sibling = GetWindow(sibling, 0x3); // RA2: as above
					if (sibling == 0) { // RA2: as above
						break;
					}
					if (GetWindowLongA(sibling, 0x4)) { // RA2: as above
						Get_Display_Rect(sibling, (RECT *)&sibling_rect); // RA2: as above
						if (IntersectRect(&intersect, &disp_rect, (const RECT *)&sibling_rect)) {
							InvalidateRect(sibling, 0, 0); // RA2: as above
							UpdateWindow(sibling);
							break;
						}
					}
				}
			}
		}
		goto after_proc;
	} else {
        if (allow_proc) goto call_custom_proc; // RA2: as above
        goto after_proc;
	}

call_custom_proc:
	if (specific_proc != 0) { // RA2: as above
		result = CallWindowProcA(specific_proc, window, message, wparam, lparam); // RA2: as above
	}

after_proc:
	if (message == WM_NCDESTROY) { // RA2: as above
		HWND destroyed = window;
		WinData * destroydata;
		unsigned int destroyed_key=(unsigned int)destroyed; // RA2: as above
		if (::Control_Table.getPointer(destroyed_key, &destroydata)) { // RA2: as above
			if (destroydata != 0 && destroydata->Back != 0) { // RA2: as above
				delete destroydata->Back; // RA2: as above
				destroydata->Back = 0; // RA2: as above
				Surface_Count--; // RA2: as above
			}
		}
		Restore_Child(window, 0); // RA2: as above
	}

cleanup:
    ctrlmessages.remove(key);
    if (data && is_paint) { // RA2: as above
        if (GetWindowLongA(window, 4) && num_rect_updates > 1) *(int *)((char *)data + 0x1FC) = 2; // RA2: as above
        if (--num_rect_updates == 0) {
            if (!data->SkipCheckPaint && anim_state >= 1) { // RA2: as above
                Rect source(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y); // RA2: as above
                Rect destination(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y); // RA2: as above
                if (*(int *)((char *)data + 0x1FC) == 1) { // RA2: as above
                    *(int *)((char *)data + 0x1FC) = 2;
                    if (GetWindowLongA(window, 4) && Play_Dialog_Move_In(window)) *(int *)((char *)data + 0x1FC) = 3; // RA2: as above
                    Rect whole(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y); // RA2: as above
                    destination = Offset_Rect(whole, offset_x, offset_y); // RA2: as above
                    VisibleSurface->Lock();
                    AlternateSurface->Lock();
                    VisibleSurface->Blit_From(destination, *AlternateSurface, whole, false, true); // RA2: as above
                    AlternateSurface->Unlock();
                    VisibleSurface->Unlock();
                    ArrayList<HWND> children;
                    EnumChildWindows(window, (WNDENUMPROC)Add_Window, (LPARAM)&children); // RA2: as above
                    HWND child = 0; // RA2: as above
                    for (int index = 0; index < children.length(); index++) {
                        children.get(child, index);
                        SendMessageA(child, 0x4AF, 0, 0); // RA2: as above
                    }
                } else {
                    destination = Offset_Rect(source, offset_x, offset_y); // RA2: as above
                    char class_name[128];
                    GetClassNameA(window, class_name, sizeof(class_name)); // RA2: as above
                    if (strcmp(class_name, "ComboBox") == 0) {
						Inflate_Rect(source, -1, -1); // RA2: as above
						Inflate_Rect(destination, -1, -1); // RA2: as above
                    }
                    VisibleSurface->Lock();
                    AlternateSurface->Lock();
                    VisibleSurface->Blit_From(destination, *AlternateSurface, source, false, true); // RA2: as above
                    AlternateSurface->Unlock();
                    VisibleSurface->Unlock();
                }
            }
            min_update_rect.x = 0xFFFFFF;
            min_update_rect.y = 0xFFFFFF;
            max_update_rect.x = 0;
            max_update_rect.y = 0;
        }
    }
    if (show_tooltip) Show_Tooltip(true); // RA2: as above
	if (message == WM_INITDIALOG) { // RA2: as above
        result = 0;
    }
    return result; // RA2: as above
}
#endif

}


/// <summary>
/// Handles the messages for a control with no owner-draw procedure of its own.
/// This is the fallback custom procedure InitializeCtrl hands to any control class the
/// owner-draw system does not paint. Only the edit coloring message is claimed, so that a
/// child edit box picks up the dialog's own font and text color.
/// </summary>
/// <returns>Returns with a null background brush for the edit coloring message; otherwise
/// with the result of the original window procedure.</returns>
namespace OwnerDraw { // RA2: as above
LRESULT __stdcall Default_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    WNDPROC proc = 0; // RA2: as above
    Window_Proc_Table.getValue(window, proc); // RA2: as above
    RECT area; // RA2: as above
    Get_Display_Rect(window, &area); // RA2: as above
    RECT client;
    GetClientRect(window, &client);
    WinData *record = 0; // RA2: as above
    Control_Table.getPointer((unsigned int const &)window, &record); // RA2: as above
	if (message == WM_CTLCOLOREDIT) { // RA2: as above
        SetTextColor((HDC)wparam, Text_Color); // RA2: as above
		SetBkMode((HDC)wparam, TRANSPARENT); // RA2: as above
		return (LRESULT)GetStockObject(NULL_BRUSH); // RA2: as above
    }
    return CallWindowProcA(proc, window, message, wparam, lparam); // RA2: as above
}
}


/// <summary>
/// Handles the messages for an owner-drawn push button.
/// The button is painted either from an image supplied by the dialog or from the button
/// artwork fitted to the control, with the caption drawn over it and the whole control
/// dimmed while it is disabled. The click sound is played as the button goes down.
/// </summary>
/// <returns>Returns with the result of the original window procedure for anything this
/// routine does not handle itself.</returns>
void *__fastcall Get_Network_Drawer(); // RA2: 0x6F45F0
void *__fastcall Get_Cancel_Drawer(); // RA2: 0x6F2250

namespace OwnerDraw { // RA2: as above

/*
**	RA2: the window procedure of the owner-drawn (BS_OWNERDRAW) buttons: plain buttons from the b?e_ art,
**	or the campaign, network and cancel button shapes by the record's button kind, which can flash.
*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (005f0320 has no row); 980 bytes differ against main's function-map extent
LRESULT __stdcall Owner_Button_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
	static char last_look = 'u'; // RA2: as above

	WNDPROC previous = 0; // RA2: as above
	Window_Proc_Table.getValue(window, previous); // RA2: as above
	RECT display; // RA2: as above
	memset(&display, 0, sizeof(display)); // RA2: as above
	Get_Display_Rect(window, &display); // RA2: as above
	Rect origin; // RA2: as above
	origin.X = display.left; // RA2: as above
	origin.Y = display.top; // RA2: as above
	origin.Width = display.right - display.left; // RA2: as above
	origin.Height = display.bottom - display.top; // RA2: as above
	RECT client;
	memset(&client, 0, sizeof(client)); // RA2: as above
	GetClientRect(window, &client);
	WinData * data = Get_Record(window); // RA2: as above
	if (data == NULL) {
		return(CallWindowProcA(previous, window, message, wparam, lparam)); // RA2: as above
	}
	int state = data->ItemState; // RA2: as above
	LONG style = GetWindowLongA(window, GWL_STYLE); // RA2: as above

	switch (message) {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if (data->IsDisabled) { // RA2: as above
				return(0);
			}
			VocClass::PlayGlobal(Rule->GUIMainButtonSound, 1.0f, 0x2000, 0); // RA2: as above
			break;

		case WM_ACTIVATE:
		case WM_KILLFOCUS:
		case WM_MOUSEACTIVATE:
			return(0);

		case WM_PAINT: {
			if (data->IsDisabled) { // RA2: as above
				ValidateRect(window, 0);
				return(0);
			}
			Rect rect = origin;
			unsigned long color = Text_Color; // RA2: as above
			if (!data->Back) { // RA2: as above
				BSurface * captured = new BSurface(client.right + 1, client.bottom + 1, 2); // RA2: as above
				data->Back = captured; // RA2: as above
				++Surface_Count; // RA2: as above
				Rect destination, source; // RA2: as above
				destination.X = 0; // RA2: as above
				destination.Y = 0; // RA2: as above
				destination.Width = client.right + 1; // RA2: as above
				destination.Height = client.bottom + 1; // RA2: as above
				source.X = display.left; // RA2: as above
				source.Y = display.top; // RA2: as above
				source.Width = client.right + 1; // RA2: as above
				source.Height = client.bottom + 1; // RA2: as above
				captured->Blit_From(destination, *AlternateSurface, source, false, true); // RA2: as above
			}
			if (data->ButtonKind != 0) { // RA2: as above
				ConvertClass * drawer = 0; // RA2: as above
				ShapeSet * shape = 0; // RA2: as above
				int frame = 0;
				if (data->ButtonKind == 1) { // RA2: as above
					drawer = (ConvertClass *)Get_Alternate_Drawer(); // RA2: as above
					shape = Slide_Button_Shape; // RA2: as above
					frame = 2; // RA2: as above
					if (state & 1) {
						frame = 4; // RA2: as above
					} else if (data->IsFlashLit) { // RA2: as above
						frame = 3; // RA2: as above
					}
				} else if (data->ButtonKind == 2) { // RA2: as above
					drawer = (ConvertClass *)Get_Network_Drawer(); // RA2: as above
					shape = Button_Strip_Shape; // RA2: as above
					if (state & 1) {
						frame = 1; // RA2: as above
					} else if (data->IsFlashLit) {
						frame = 2; // RA2: as above
					}
				} else if (data->ButtonKind == 3) { // RA2: as above
					drawer = (ConvertClass *)Get_Cancel_Drawer(); // RA2: as above
					shape = Cancel_Button_Shape; // RA2: as above
					if (state & 1) {
						frame = 1; // RA2: as above
					} else if (data->IsFlashLit) {
						frame = 2; // RA2: as above
					}
				}
				if (style & WS_DISABLED) {
					int pixel;
					if (Session.Is_In_Game()) {
						if (Scen->IsGDI) {
							pixel = DSurface::Build_Hicolor_Pixel(Allied_Disabled_Color);
						} else {
							pixel = DSurface::Build_Hicolor_Pixel(Soviet_Disabled_Color);
						}
					} else {
						pixel = DSurface::Build_Hicolor_Pixel(Menu_Disabled_Color);
					}
					color = Pixel_To_Color((unsigned short)pixel); // RA2: as above
				}
				Point2D point = Point_Copy((Point2D const *)&rect); // RA2: as above
				Rect window_rect = AlternateSurface->Get_Rect(); // RA2: as above
				if (drawer && shape) { // RA2: as above
					Draw_Shape(*AlternateSurface, *drawer, shape, frame, point, window_rect, ShapeFlags_Type(0x400), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
				}
			} else {
				if (data->Image) { // RA2: as above
					Surface * image = data->Image; // RA2: as above
					if ((state & 1) && data->AlternateImage) image = data->AlternateImage; // RA2: as above
					Rect source = rect; // RA2: as above
					source.Y = source.X = 0; // RA2: as above
					AlternateSurface->Blit_From(rect, *image, source, false, true); // RA2: as above
				} else {
					char updown = 'u';
					if (state & 1) updown = 'd'; // RA2: as above
					if (style & WS_DISABLED) {
						updown = 'u';
					} else if (updown == 'd' && last_look == 'u') { // RA2: as above
						VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, 0); // RA2: as above
					}
					last_look = updown; // RA2: as above
					int widths[2] = {7, 7};
					int margins[2] = {10, 10};
					int heights[2] = {24, 30};
					int index = 0; // RA2: as above
					for (unsigned i = 0; i < 2; i++) { // RA2: as above
						if (heights[i] > rect.Height && i != 0) break; // RA2: as above
						index = i;
					}
					int height = heights[index];
					int width = widths[index]; // RA2: as above
					int margin = margins[index];
					if (data->Back) { // RA2: as above
						Rect source, destination; // RA2: as above
						source.X = 0; // RA2: as above
						source.Y = 0; // RA2: as above
						source.Width = client.right + 1; // RA2: as above
						source.Height = client.bottom + 1; // RA2: as above
						destination.X = display.left; // RA2: as above
						destination.Y = display.top; // RA2: as above
						destination.Width = client.right + 1; // RA2: as above
						destination.Height = client.bottom + 1; // RA2: as above
						AlternateSurface->Blit_From(destination, *data->Back, source, false, true); // RA2: as above
						InvalidateRect(window, 0, 0); // RA2: as above
					}
					origin.Y += (rect.Height - height) / 2;
					if (state & 1) origin.Y += 2; // RA2: as above
					char name[40]; // RA2: as above
					sprintf(name, "b%c%c_li%d.pcx", updown, 'e', height); // RA2: as above
					Surface * image = SurfaceCache.GetSurface(name, 0); // RA2: as above
					origin.Height = image->Get_Height(); // RA2: as above
					Rect destination, source; // RA2: as above
					destination = origin; // RA2: as above
					destination.Width = width; // RA2: as above
					source.Width = width; // RA2: as above
					source.Y = 0; // RA2: as above
					source.X = 0; // RA2: as above
					destination.Height = height; // RA2: as above
					source.Height = height; // RA2: as above
					AlternateSurface->Blit_From(destination, *image, source, false, true); // RA2: as above
					sprintf(name, "b%c%c_mi%d.pcx", updown, 'e', height); // RA2: as above
					image = SurfaceCache.GetSurface(name, 0); // RA2: as above
					Rect middle = origin; // RA2: as above
					middle.X += width; // RA2: as above
					middle.Width -= margin; // RA2: as above
					middle.Height = image->Get_Height(); // RA2: as above
					SurfaceCache.Draw(middle, *AlternateSurface, *image, 0, 0); // RA2: as above
					sprintf(name, "b%c%c_ri%d.pcx", updown, 'e', height); // RA2: as above
					image = SurfaceCache.GetSurface(name, 0); // RA2: as above
					destination = origin; // RA2: as above
					destination.X += origin.Width - margin; // RA2: as above
					destination.Width = margin; // RA2: as above
					destination.Height = image->Get_Height(); // RA2: as above
					source.Width = destination.Width; // RA2: as above
					source.Height = destination.Height; // RA2: as above
					source.Y = 0; // RA2: as above
					source.X = 0; // RA2: as above
					AlternateSurface->Blit_From(destination, *image, source, false, true); // RA2: as above
				}
			}
			if (!data->Image && data->Text) { // RA2: as above
				RECT unused_client; // RA2: as above
				GetClientRect(window, &unused_client); // RA2: as above
				RECT text_rect;
				text_rect.left = origin.X; // RA2: as above
				text_rect.top = origin.Y + 1; // RA2: as above
				text_rect.right = origin.Width + origin.X - 2; // RA2: as above
				text_rect.bottom = origin.Height + origin.Y; // RA2: as above
				if (state & 1) {
					text_rect.left += 2; // RA2: as above
					text_rect.top += 4; // RA2: as above
				}
				Draw_Text_In_Rect_Ex(AlternateSurface, data->Text, &text_rect, data->Font, color, 5, 0xc, 0, 0, 0); // RA2: as above
			}
			if (data->ButtonKind == 0 && (style & WS_DISABLED)) { // RA2: as above
				Blend_Rect(&rect, AlternateSurface, 0, 128); // RA2: as above
			}
			ValidateRect(window, 0);
			return(0);
		}

		case WM_TIMER: // RA2: as above
			data->IsFlashLit = !data->IsFlashLit; // RA2: as above
			InvalidateRect(window, 0, TRUE); // RA2: as above
			break;

		case WM_USER + 220: { // RA2: lparam 1 starts the flashing, anything else stops it
			Control_Table.getPointer((unsigned int const &)window, &data); // RA2: as above
			BOOL flash = (lparam == 1);
			if (flash && !data->IsFlashing) {
				data->IsFlashing = true; // RA2: as above
				SetTimer(window, 0, 1000, 0); // RA2: as above
			} else if (!flash && data->IsFlashing) {
				data->IsFlashing = false; // RA2: as above
				data->IsFlashLit = false; // RA2: as above
				KillTimer(window, 0);
				InvalidateRect(window, 0, TRUE); // RA2: as above
			}
			break;
		}
	}
	return(CallWindowProcA(previous, window, message, wparam, lparam)); // RA2: as above
}
#endif

}


/// <summary>
/// Handles the messages for an owner-drawn tab control.
/// The body of the control is painted as dimmed dialog background and each tab is built
/// from its corner and middle artwork, with the caption drawn over it in the remapped
/// font. The active tab gets the brighter text color.
/// </summary>
/// <returns>Returns with the result of the original window procedure for anything this
/// routine does not paint itself.</returns>
namespace OwnerDraw { // RA2: as above

/*
**	RA2: TibSun's TextBoxCtrlProc, the tab control procedure, with wide titles.
*/
union ODColorBits {
	struct {
		unsigned int red : 8;
		unsigned int green : 8;
		unsigned int blue : 8;
		unsigned int a : 8;
	};
	int v;
};

LRESULT __stdcall Tab_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
	static char string1[64];
	static wchar_t string2[64]; // RA2: the title as wide text

	LRESULT result = 0;
	WNDPROC proc = NULL;
	WinData * data = NULL;
	Control_Table.getPointer((unsigned int const &)window, &data); // RA2: the control table
	Window_Proc_Table.getValue(window, proc); // RA2: the window procedure table

	if (message == WM_ERASEBKGND) {
		return(1);
	}

	if (message == WM_NCPAINT) {
		return(0);
	}

	if (message == OD_SUBCLASSED) {
		RECT rect;
		Get_Display_Rect(window, &rect);

		Surface * surf = SurfaceCache.GetSurface("tab_tlu.pcx");
		SendMessage(window, TCM_SETITEMSIZE, 0, MAKELPARAM(89, surf->Get_Height() - 1));

	}

	if (message == WM_PAINT) {
		RECT winrect;
		Get_Display_Rect(window, &winrect);

		TabCtrl_GetItemCount(window);

		RECT tabrect;
		TabCtrl_GetItemRect(window, 0, &tabrect);
		TabCtrl_GetItemRect(window, 0, &tabrect);

		winrect.top += (tabrect.bottom - tabrect.top) + 3;
		Rect dimrect;
		dimrect.X = winrect.left;
		dimrect.Y = winrect.top;
		dimrect.Width = winrect.right - winrect.left;
		dimrect.Height = winrect.bottom - winrect.top;

		Draw_Dimmed_Back(dimrect, window, 0); // RA2: with an alpha
		ValidateRect(window, NULL);

		dimrect.X = winrect.left;
		dimrect.Y = winrect.top;
		dimrect.Width = winrect.right - winrect.left;
		dimrect.Height = winrect.bottom - winrect.top;

		Surface * image = SurfaceCache.GetSurface("tab_fml.pcx");
		if (image) {
			image->Get_Height();
			int width = image->Get_Width();
			SurfaceCache.Draw(Rect(Point2D(winrect.left, winrect.top), width, winrect.bottom - winrect.top), *AlternateSurface, *image, 0, 0);
		}

		image = SurfaceCache.GetSurface("tab_fmr.pcx");
		if (image) {
			image->Get_Height();
			int width = image->Get_Width();
			SurfaceCache.Draw(Rect(winrect.right - width, winrect.top, width, winrect.bottom - winrect.top), *AlternateSurface, *image, 0, 0);
		}

		image = SurfaceCache.GetSurface("tab_ftm.pcx");
		if (image) {
			int height = image->Get_Height();
			image->Get_Width();
			SurfaceCache.Draw(Rect(Point2D(winrect.left, winrect.top), winrect.right - winrect.left, height), *AlternateSurface, *image, 0, 0);
		}

		image = SurfaceCache.GetSurface("tab_fbm.pcx");
		if (image) {
			int height = image->Get_Height();
			image->Get_Width();
			SurfaceCache.Draw(Rect(winrect.left, winrect.bottom - height, winrect.right - winrect.left, height), *AlternateSurface, *image, 0, 0);
		}

		image = SurfaceCache.GetSurface("tab_ftl.pcx");
		if (image) {
			int height = image->Get_Height();
			int width = image->Get_Width();
			AlternateSurface->Blit_From(Rect(Point2D(winrect.left, winrect.top), width, height), *image, Rect(0, 0, width, height));
		}

		image = SurfaceCache.GetSurface("tab_ftr.pcx");
		if (image) {
			int height = image->Get_Height();
			int width = image->Get_Width();
			AlternateSurface->Blit_From(Rect(winrect.right - width, winrect.top, width, height), *image, Rect(0, 0, width, height));
		}

		image = SurfaceCache.GetSurface("tab_fbl.pcx");
		if (image) {
			int height = image->Get_Height();
			int width = image->Get_Width();
			AlternateSurface->Blit_From(Rect(winrect.left, winrect.bottom - height, width, height), *image, Rect(0, 0, width, height));
		}

		image = SurfaceCache.GetSurface("tab_fbr.pcx");
		if (image) {
			int height = image->Get_Height();
			int width = image->Get_Width();
			AlternateSurface->Blit_From(Rect(Point2D(winrect.right - width, winrect.bottom - height), width, height), *image, Rect(0, 0, width, height));
		}

		int current = TabCtrl_GetCurSel(window);
		Get_Display_Rect(window, &winrect);
		int itab = current + 1;

		RECT itemrect;
		while (true) {
			int tab = itab;

			if (TabCtrl_GetItemRect(window, tab, &itemrect)) {
				TC_ITEM item;
				memset(&item, 0, sizeof(item));
				item.pszText = string1;
				item.cchTextMax = sizeof(string1);
				strcpy(string1, "Title");
				item.mask = TCIF_TEXT;
				TabCtrl_GetItem(window, tab, &item);

				char state = 'd';
				if (tab == current) {
					state = 'u';
				}

				dimrect.X = winrect.left + (itemrect.left - MIN(itemrect.left, 6));
				dimrect.Y = itemrect.top + winrect.top;
				dimrect.Width = itemrect.right + (MIN(itemrect.left, 6) - itemrect.left);
				dimrect.Height = itemrect.bottom - itemrect.top;

				char fname[80];
				Surface * tab_lu = SurfaceCache.GetSurface("tab_tlu.pcx");
				sprintf(fname, "tab_tm%c.pcx", state);
				image = SurfaceCache.GetSurface(fname);
				if (image) {
					int height = image->Get_Height();
					image->Get_Width();
					int width = dimrect.Width - 2 * tab_lu->Get_Width();
					int draw_y = dimrect.Y;
					int draw_x = dimrect.X + tab_lu->Get_Width();
					SurfaceCache.Draw(Rect(draw_x, draw_y, width, height), *AlternateSurface, *image, 0, 0);
				}

				sprintf(fname, "tab_tl%c.pcx", state);
				image = SurfaceCache.GetSurface(fname);
				if (image) {
					int height = image->Get_Height();
					int width = image->Get_Width();
					ODColorBits transparent;
					transparent.v = RGB(255, 0, 255);
					SurfaceCache.DrawTrans(Rect(Point2D(dimrect.X, dimrect.Y), width, height), *AlternateSurface, *image, DSurface::Build_Hicolor_Pixel(transparent.red, transparent.green, transparent.blue));
				}

				sprintf(fname, "tab_tr%c.pcx", state);
				image = SurfaceCache.GetSurface(fname);
				if (image) {
					int height = image->Get_Height();
					int width = image->Get_Width();
					Rect trrect;
					trrect.X = dimrect.X + dimrect.Width - width;
					trrect.Y = dimrect.Y;
					trrect.Width = width;
					trrect.Height = height;
					ODColorBits transparent;
					transparent.v = RGB(255, 0, 255);
					SurfaceCache.DrawTrans(trrect, *AlternateSurface, *image, DSurface::Build_Hicolor_Pixel(transparent.red, transparent.green, transparent.blue));
				}

				if (item.pszText) {
					strcpy(string1, item.pszText);
				}

				RECT text_rect; // RA2: a RECT
				text_rect.left = dimrect.X;
				text_rect.right = dimrect.X + dimrect.Width;
				text_rect.top = dimrect.Y + 6;
				text_rect.bottom = dimrect.Y + dimrect.Height;

				COLORREF color = Olive_Color; // RA2: the dim text colour
				if (itab == current) {
					color = Text_Color;
				}
				swprintf(string2, L"%hs", string1);
				Draw_Text_In_Rect_Ex(AlternateSurface, string2, &text_rect, data->Font, color, 5, 0xc, 0, 0, 0);

				ValidateRect(window, &itemrect);
				if (itab == current) {
					break;
				}

				itab++;
			} else {
				itab = 0;
			}
		}

		ValidateRect(window, NULL);
	} else if (proc) {
		result = CallWindowProc(proc, window, message, wparam, lparam);
	}
	return(result);
}


}


/// <summary>
/// Handles the messages for an owner-drawn static text control.
/// The caption is kept in the control's owner-draw record rather than in the window, so it
/// can be drawn in the remapped font over a cached copy of the dialog background. The
/// dialog can recolor the text at any time with OD_SETCOLOR.
/// </summary>
/// <returns>Returns with the result of the original window procedure for anything this
/// routine does not handle itself.</returns>
namespace OwnerDraw { // RA2: as above
void __fastcall Draw_Edit_Text(Surface *,Rect &,wchar_t const *,int, // RA2: as above
    BitFont *,unsigned int,int *,int,int,int,int); // RA2: as above
LRESULT __stdcall Edit_Control_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam) // RA2: as above
{
    WNDPROC previous=0; // RA2: as above
    Window_Proc_Table.getValue(window, previous); // RA2: as above
    WinData *data = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)window, &data); // RA2: as above
    char class_name[32];
    char narrow[0x800]; // RA2: as above
    unsigned short wide[0x800]; // RA2: as above
    if(!data) return CallWindowProcA(previous,window,message,wparam,lparam); // RA2: as above
    if(GetFocus()==window && !data->IsEditFocus) { // RA2: as above
        data->Position=1; // RA2: as above
        SetFocus(MainWindow);
    }
	LONG style = GetWindowLongA(window, GWL_STYLE); // RA2: as above
	if ((message == WM_KEYUP || message == WM_KEYDOWN) && wparam == 9) return 0; // RA2: as above
	if (message == OD_SUBCLASSED) { // RA2: as above
        RECT winrect,client,parentrect; // RA2: as above
        GetWindowRect(window,&winrect); // RA2: as above
        GetClientRect(window,&client); // RA2: as above
        GetWindowRect(GetParent(window),&parentrect); // RA2: as above
        MoveWindow(window,winrect.left-parentrect.left+1,winrect.top-parentrect.top+1, // RA2: as above
            client.right-2,client.bottom-2,0); // RA2: as above
        if(GetFocus()==window) { // RA2: as above
            data->Position=1; // RA2: as above
            SetFocus(MainWindow);
        }
		if (style & WS_TABSTOP) { // RA2: as above
            data->HadTabStop=1; // RA2: as above
			SetWindowLongA(window, GWL_STYLE, style & ~WS_TABSTOP); // RA2: as above
        }
        return 0; // RA2: as above
    }
	if (message == WM_SETFOCUS) { // RA2: as above
		SendMessageA(window, EM_SETSEL, (WPARAM)-1, -1); // RA2: as above
		if (!data->IsEditFocus) PostMessageA(window, OD_REFOCUS, 0, 0); // RA2: as above
        goto invalidate; // RA2: as above
    }
	if (message == 0x4b5 || message == 0x4b3 || message == WM_GETTEXTLENGTH) { // RA2: as above
        wide[0]=0; // RA2: as above
        Edit_Text(window,wide,0x800,0); // RA2: as above
		if (message == WM_GETTEXTLENGTH) return wcslen(wide); // RA2: as above
        if(message==0x4b3) { // RA2: as above
            wcsncpy((unsigned short *)lparam,wide,wparam); // RA2: as above
            ((unsigned short *)lparam)[wparam-1]=0; // RA2: as above
            return wcslen((unsigned short *)lparam); // RA2: as above
        }
        WideToCharString((char *)lparam,wide,wparam); // RA2: as above
        ((char *)lparam)[wparam-1]=0; // RA2: as above
        return strlen((char *)lparam); // RA2: as above
    }
	if (message == 0x4b4 || message == OD_SETTEXTW) { // RA2: as above
        narrow[0]=0; // RA2: as above
		if (data->Text) WideCharToMultiByte(CP_ACP, 0, data->Text, -1, narrow, 0x800, 0, 0); // RA2: as above
		return CallWindowProcA(previous, window, WM_SETTEXT, 0, (LPARAM)narrow); // RA2: as above
    }
	if (message == WM_CHAR) { // RA2: as above
        if(wparam==0xd) { // RA2: as above
			if (style & ES_MULTILINE) { // RA2: as above
				unsigned int length = CallWindowProcA(previous, window, WM_GETTEXTLENGTH, 0, 0) + 3; // RA2: as above
                char *text=new char[length]; // RA2: as above
				CallWindowProcA(previous, window, WM_GETTEXT, length, (LPARAM)text); // RA2: as above
                strcat(text,"\r\n"); // RA2: as above
				CallWindowProcA(previous, window, WM_SETTEXT, 0, (LPARAM)text); // RA2: as above
                delete [] text; // RA2: as above
				unsigned int id = GetWindowLongA(window, GWL_ID) & 0xffff; // RA2: as above
				SendMessageA(GetParent(window), WM_COMMAND, id | 0x5010000, (LPARAM)window); // RA2: as above
                return 0; // RA2: as above
            }
        } else if(wparam==9) { // RA2: as above
            HWND next=window; // RA2: as above
            HWND parent=GetParent(next); // RA2: as above
            HWND tab=GetNextDlgTabItem(parent,next,0); // RA2: as above
            if(tab)next=tab; // RA2: as above
            SetFocus(next);
            return 0; // RA2: as above
        }
        goto call_previous; // RA2: as above
    }
	if (message == OD_ACTIVATE) { // RA2: as above
        int pending=data->Position; // RA2: as above
        data->IsEditFocus=1; // RA2: as above
        if(pending) { // RA2: as above
            SetFocus(window);
            data->Position=0; // RA2: as above
        }
		if (data->HadTabStop) SetWindowLongA(window, GWL_STYLE, style | WS_TABSTOP); // RA2: as above
        return 0; // RA2: as above
    }
	if (message == WM_PAINT || message == WM_ERASEBKGND) { // RA2: as above
        RECT display,update; // RA2: as above
        Get_Display_Rect(window,&display); // RA2: as above
        GetClassNameA(GetParent(window),class_name,16); // RA2: as above
        bool combo=strcmp(class_name,"ComboBox")==0; // RA2: as above
		if (message == WM_PAINT && GetUpdateRect(window, &update, 0)) { // RA2: as above
            update.left+=display.left; // RA2: as above
            update.right+=display.left; // RA2: as above
            update.top+=display.top; // RA2: as above
            update.bottom+=display.top; // RA2: as above
        }
        Rect background(display.left,display.top,display.right-display.left+1,display.bottom-display.top+1); // RA2: as above
        Draw_Dimmed_Back(background,window,data->TextStyle); // RA2: as above
        if(!combo)Draw_Frame(AlternateSurface,background,1,-1); // RA2: as above
        int caret=0; // RA2: as above
        Edit_Text(window,Edit_Buffer,0x1400,&caret); // RA2: as above
        Rect text(display.left,display.top,display.right-display.left+1,display.bottom-display.top+1); // RA2: as above
		LONG text_style = GetWindowLongA(window, GWL_STYLE); // RA2: as above
        Draw_Edit_Text(AlternateSurface,text,Edit_Buffer,caret, // RA2: as above
			data->Font, Text_Color, &data->EditScroll, data->ImeState, ((unsigned int)text_style >> 5) & 1, 0, 0); // RA2: as above
        ValidateRect(window,0); // RA2: as above
    }
	if (message == WM_CONTEXTMENU) return 1; // RA2: as above
	if (message == WM_MOUSEMOVE) return 1; // RA2: as above
	if (message == WM_KEYDOWN || message == WM_KEYUP || message == WM_SYSKEYDOWN || message == WM_SYSKEYUP || message == WM_SYSCHAR || message == WM_SYSDEADCHAR || message == WM_KILLFOCUS || message == WM_LBUTTONDOWN) { // RA2: as above
invalidate: // RA2: as above
        GetClassNameA(GetParent(window),class_name,32); // RA2: as above
        if(strcmp(class_name,"ComboBox")==0)InvalidateRect(GetParent(window),0,0); // RA2: as above
        InvalidateRect(window,0,0); // RA2: as above
    }
call_previous: // RA2: as above
    return CallWindowProcA(previous,window,message,wparam,lparam); // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above

int __fastcall Edit_Text(HWND window, wchar_t *text, int size, int *caret) // RA2: as above
{
    WNDPROC proc = 0; // RA2: as above
    Window_Proc_Table.getValue(window, proc); // RA2: as above
    char buffer[1024]; // RA2: as above
    int start = 0; // RA2: as above
    buffer[0] = 0; // RA2: as above
    CallWindowProcA(proc, window, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer); // RA2: as above
    if (caret) { // RA2: as above
        DWORD selection_start; // RA2: as above
        DWORD selection_end; // RA2: as above
        start = (unsigned long)CallWindowProcA(proc, window, EM_GETSEL, (WPARAM)&selection_start, (LPARAM)&selection_end) >> 16; // RA2: as above
    }
    *text = 0;
	MultiByteToWideChar(CP_ACP, 0, buffer, -1, text, size); // RA2: as above
    if (caret) { // RA2: as above
        *caret = _mbsnbcnt((unsigned char *)buffer, start);
    }
    if (size > 0) { // RA2: as above
        text[size - 1] = 0; // RA2: as above
    }
    int length = wcslen(text); // RA2: as above
    wchar_t *copy = new wchar_t[length + 3]; // RA2: as above
    memset(copy, 0, (length + 3) * sizeof(wchar_t)); // RA2: as above
    int count = 0; // RA2: as above
    int i = 0;
    for (; i < length; i++) { // RA2: as above
        if (text[i] != '\r' && text[i] != '\n') { // RA2: as above
            copy[count++] = text[i]; // RA2: as above
        }
    }
    if (i != count) { // RA2: as above
        wcscat(copy, L"\r\n"); // RA2: as above
    }
    wcscpy(text, copy); // RA2: as above
    delete[] copy; // RA2: as above
    return wcslen(text); // RA2: as above
}

}


unsigned short __fastcall IME_Char_To_Wide(unsigned int wparam, long lparam); // RA2: 0x738130 (winstub.cpp)

namespace OwnerDraw { // RA2: as above


/*
**	RA2: the window procedure of the "NewEdit" control, an edit control that keeps its text in the record as wide
**	characters and draws itself. The cursor keys (the jump table at 0x5F2B04) move the caret in it.
*/
LRESULT __stdcall New_Edit_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x5F22A0, the NewEdit window procedure
{
	WNDPROC proc = NULL;
	int changed = 0;
	int handled = 0;
	if (message == WM_GETDLGCODE) {
		return DLGC_WANTALLKEYS;
	}
	Window_Proc_Table.getValue(window, proc);
	WinData * data = NULL;
	Control_Table.getPointer((unsigned int const &)window, &data);
	HWND parent = GetParent(window);

	if ((message == WM_KEYUP || message == WM_KEYDOWN) && wparam == VK_TAB) {
		if (message == WM_KEYDOWN) {
			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			SendMessage(parent, WM_USER + 219, shift, (LPARAM)window);
		}
		return 0;
	}
	if (message == OD_SUBCLASSED) {
		RECT window_rect;
		GetWindowRect(window, &window_rect);
		RECT client_rect;
		GetClientRect(window, &client_rect);
		RECT parent_rect;
		GetWindowRect(parent, &parent_rect);
		SetWindowPos(window, NULL, window_rect.left - parent_rect.left + 1,
		window_rect.top - parent_rect.top + 1, client_rect.right - 2,
		client_rect.bottom - 2, SWP_SHOWWINDOW);
		return 0;
	} else if (message == EM_LIMITTEXT) {
		data->MaxLength = wparam;
		if (wparam) {
			data->WideText->truncate(wparam);
		}
	} else if (message == WM_USER + 181 || message == WM_USER + 179 || message == WM_GETTEXTLENGTH) {
		if (message == WM_GETTEXTLENGTH) {
			return data->WideText->length();
		}
		WideWstring * query_text = data->WideText;
		if (message == WM_USER + 179) {
			wcsncpy((wchar_t *)lparam, query_text->get(), wparam);
			((wchar_t *)lparam)[wparam - 1] = 0;
			return wcslen((wchar_t *)lparam);
		}
		WideToCharString((char *)lparam, query_text->get(), wparam);
		((char *)lparam)[wparam - 1] = 0;
		return strlen((char *)lparam);
	} else {
		if (message == WM_USER + 180 || message == WM_USER + 178) {
			data->WideText->clear();
			data->Caret = 0;
			data->EditScroll = 0;
			if (data->Text != NULL) {
				data->WideText->set(data->Text);
			}
			if (data->MaxLength) {
				data->WideText->truncate((unsigned int)data->MaxLength);
			}
			data->Caret = data->WideText->length();
		}
		if (message == WM_KEYDOWN) {
			if (wparam == VK_RETURN) {
				SendMessage(parent, WM_USER + 218, 0, (LPARAM)window);
				if (data->UserDataWord & ES_MULTILINE) {
					*data->WideText += L"\r\n";
					WPARAM command = (unsigned short)GetWindowLong(window, GWL_ID) | 0x5010000;
					SendMessage(parent, WM_COMMAND, command, (LPARAM)window);
				}
				return 0;
			}
		} else if (message == WM_SETFOCUS) {
			data->IsCaretHidden = 0;
			SetTimer(window, 0, 1000, NULL);
		} else if (message == WM_KILLFOCUS) {
			KillTimer(window, 0);
		} else if (message == WM_TIMER) {
			data->IsCaretHidden ^= 1;
			InvalidateRect(window, NULL, FALSE);
		} else {
			if (message == WM_PAINT || message == WM_ERASEBKGND) {
				RECT display_rect;
				Get_Display_Rect(window, &display_rect);
				char paint_class[80];
				GetClassName(parent, paint_class, sizeof(paint_class));
				bool is_combo = strcmp(paint_class, "ComboBox") == 0;
				RECT update_rect;
				if (message == WM_PAINT && GetUpdateRect(window, &update_rect, FALSE)) {
					update_rect.left += display_rect.left;
					update_rect.right += display_rect.left;
					update_rect.top += display_rect.top;
					update_rect.bottom += display_rect.top;
				}
				Rect draw_rect;
				draw_rect.X = display_rect.left;
				draw_rect.Y = display_rect.top;
				draw_rect.Width = display_rect.right - display_rect.left + 1;
				draw_rect.Height = display_rect.bottom - display_rect.top + 1;
				Draw_Dimmed_Back(draw_rect, window, data->TextStyle);
				if (!is_combo) {
					Draw_Frame(AlternateSurface, draw_rect, 1, -1); // RA2: a one-pixel frame (the twin, YR, draws two)
				}
				display_rect.left += 2;
				Rect text_rect;
				text_rect.X = display_rect.left;
				text_rect.Y = display_rect.top;
				text_rect.Width = display_rect.right - display_rect.left + 1;
				text_rect.Height = display_rect.bottom - display_rect.top + 1;
				Caret_Hidden = data->IsCaretHidden;
				Draw_Edit_Text(AlternateSurface, text_rect, data->WideText->get(),
					data->Caret, data->Font, Text_Color,
					&data->EditScroll, data->ImeState,
					(data->UserDataWord & ES_PASSWORD) != 0, 0, 0);
				ValidateRect(window, NULL);
			}
			if (message == WM_CONTEXTMENU) {
				return 1;
			}
			if (message == WM_MOUSEMOVE) {
				return 1;
			}
		}
		if (message == WM_KEYDOWN || message == WM_SETFOCUS || message == WM_KILLFOCUS ||
		message == WM_KEYUP || message == WM_SYSKEYDOWN || message == WM_SYSKEYUP ||
		message == WM_SYSCHAR || message == WM_SYSDEADCHAR || message == WM_LBUTTONDOWN) {
			char class_name[32];
			GetClassName(parent, class_name, sizeof(class_name));
			if (strcmp(class_name, "ComboBox") == 0) {
				InvalidateRect(parent, NULL, FALSE);
			}
			InvalidateRect(window, NULL, FALSE);
		}
		wchar_t ch = 0;
		if (message == WM_CHAR) {
			if (wparam > 31) {
				ch = (wchar_t)wparam;
			}
		} else if (message == WM_KEYDOWN) {
			switch (wparam) {
			case VK_DELETE:
				if (data->Caret < (int)data->WideText->length()) {
					data->WideText->remove(data->Caret, 1);
					changed = 1;
				}
				handled = 1;
				break;
			case VK_BACK:
				if (data->Caret > 0) {
					data->Caret--;
					data->WideText->remove(data->Caret, 1);
					changed = 1;
				}
				handled = 1;
				break;
			case VK_LEFT:
				if (data->Caret > 0) {
					--data->Caret;
					return 0;
				}
				handled = 1;
				break;
			case VK_RIGHT:
				if (data->Caret < (int)data->WideText->length()) {
					++data->Caret;
					return 0;
				}
				handled = 1;
				break;
			case VK_END:
				data->Caret = data->WideText->length();
				return 0;
			case VK_HOME:
				data->Caret = 0;
				return 0;
			default:
				break;
			}
		} else if (message == WM_IME_CHAR) {
			ch = IME_Char_To_Wide(wparam, lparam);
			handled = 1;
		} else if (message == WM_USER + 217) {
			ch = (wchar_t)wparam;
			handled = 1;
		}
		if (ch) {
			if ((!data->Excluded || !wcschr(data->Excluded, ch)) &&
				(!data->MaxLength || (int)data->WideText->length() < data->MaxLength) &&
				ch > 31 && (!data->IsAsciiOnly || ch < 256)) {
				data->WideText->insert(ch, data->Caret++);
				changed = 1;
			}
			handled = 1;
		}
		if (changed) {
			int id = (unsigned short)GetWindowLong(window, GWL_ID);
			SendMessage(parent, WM_COMMAND, id | (EN_CHANGE << 16), (LPARAM)window);
			SendMessage(parent, WM_COMMAND, id | (EN_UPDATE << 16), (LPARAM)window);
		}
		if (handled) {
			return 0;
		}
	}
	return CallWindowProc(proc, window, message, wparam, lparam);
}


} // RA2: as above


#include "movie.h" // RA2: the includes this file needs
#include "movies.h" // RA2: the includes this file needs
MovieHandle * __fastcall OwnerDraw_InitMovieHandle(char const * name, Surface * surface, Rect * bounds); // RA2: 0x5A1D30 (movie.cpp)

/*
**	RA2: a static control's view of its record: picture modes, typed-out text, shape animation and movies.
*/
struct StaticRecord { // RA2: as above
	char Reserved_00[0x10]; // RA2: as above
	Surface * Back;					// RA2: +0x10 what lies under the control
	Surface * Image;				// RA2: +0x14 picture mode 2
	char Reserved_18[0x28 - 0x18]; // RA2: as above
	wchar_t * Text;					// RA2: +0x28
	char Reserved_2C[0x58 - 0x2C]; // RA2: as above
	MovieHandle * Movie;			// RA2: +0x58 playing in the control
	int IsLooping;					// RA2: +0x5C the movie restarts when it ends
	MovieHandle * Movie2;			// RA2: +0x60 released with it
	BitFont * Font;			// RA2: +0x64
	char Reserved_68[0x70 - 0x68]; // RA2: as above
	int PictureMode;				// RA2: +0x70 0 text, 1 typed text, 2 image, 3 shape, 4 animation
	ConvertClass * Drawer;			// RA2: +0x74
	ShapeSet * Shape;				// RA2: +0x78
	bool IsShapeAllocated;			// RA2: +0x7C
	int Typed;						// RA2: +0x80 characters shown so far
	int TypeRate;					// RA2: +0x84 the timer period
	int TypeStep;					// RA2: +0x88 characters per tick
	int TypeExtra;					// RA2: +0x8C
	int TypingSound;				// RA2: +0x90 -1 for none
	int FrameCount;					// RA2: +0x94 (the frame delay until the shape is known)
	int Frame;						// RA2: +0x98
	int StartTime;					// RA2: +0x9C
	int Delay;						// RA2: +0xA0 before the picture shows
	HWND Notify;					// RA2: +0xA4 told of each new frame (0x4D8)
	bool IsPlaying;					// RA2: +0xA8
	int TextFlags;					// RA2: +0xAC
	char Reserved_B0[0xB4 - 0xB0]; // RA2: as above
	bool IsFilled;					// RA2: +0xB4 a solid background
	COLORREF FillColor;				// RA2: +0xB8
	bool IsDisabled;				// RA2: +0xBC
	char Reserved_BD[0xEC - 0xBD]; // RA2: as above
	COLORREF TextColor;				// RA2: +0xEC
};

namespace OwnerDraw { // RA2: as above

/*
**	RA2: stops the control's movies (and its timer).
*/
static void Stop_Static_Movie(HWND window, StaticRecord * data) // RA2: as above
{
	if (data->Movie) { // RA2: as above
		delete data->Movie; // RA2: as above
		data->Movie = 0; // RA2: as above
	}
	KillTimer(window, 0x65); // RA2: as above
	if (data->Movie2) { // RA2: as above
		delete data->Movie2; // RA2: as above
		data->Movie2 = 0; // RA2: as above
	}
}

/*
**	RA2: the window procedure of the static controls: text (typed out), an image, a shape or its
**	animation, or a movie.
*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (005f2b50 has no row)
LRESULT __stdcall Static_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
	WinData * record = 0; // RA2: as above
	Control_Table.getPointer((unsigned int const &)window, &record); // RA2: as above
	StaticRecord * data = (StaticRecord *)record; // RA2: as above
	if (data == 0) { // RA2: as above
		return(0);
	}

	switch (message) {
		case WM_DESTROY:
			if (data->Back) { // RA2: as above
				delete data->Back; // RA2: as above
				data->Back = 0; // RA2: as above
			}
			if (data->IsShapeAllocated) { // RA2: as above
				operator delete(data->Shape); // RA2: as above
				data->Shape = 0; // RA2: as above
			}
			if ((data->PictureMode == 1 && data->IsPlaying) || data->PictureMode == 4) { // RA2: as above
				KillTimer(window, 0);
			}
			Stop_Static_Movie(window, data); // RA2: as above
			break;

		case WM_MOVE:
		case WM_SIZE:
		case WM_WINDOWPOSCHANGED: // RA2: as above
			if (data->Back) { // RA2: as above
				delete data->Back; // RA2: as above
			}
			data->Back = 0; // RA2: as above
			InvalidateRect(window, 0, FALSE); // RA2: as above
			return(0);

		case WM_PAINT: {
			if (data->IsDisabled) { // RA2: as above
				ValidateRect(window, 0);
				return(0);
			}
			if (data->Movie) { // RA2: as above
				ValidateRect(window, 0);
				return(0);
			}
			if (!data->Back) { // RA2: as above
				RECT display; // RA2: as above
				Get_Display_Rect(window, &display); // RA2: as above
				RECT client;
				GetClientRect(window, &client);
				BSurface * surface = new BSurface(client.right + 1, client.bottom + 1, 2); // RA2: as above
				data->Back = surface; // RA2: as above
				++Surface_Count; // RA2: as above
				Rect source(display.left, display.top, client.right + 1, client.bottom + 1); // RA2: as above
				Rect destination(0, 0, client.right + 1, client.bottom + 1); // RA2: as above
				surface->Blit_From(destination, *AlternateSurface, source, false, true); // RA2: as above
			}
			RECT area; // RA2: as above
			Get_Display_Rect(window, &area); // RA2: as above
			if (data->IsFilled) { // RA2: as above
				Rect fill(area.left, area.top, area.right - area.left, area.bottom - area.top); // RA2: as above
				AlternateSurface->Fill_Rect(fill, DSurface::Build_Hicolor_Pixel(data->FillColor & 0xFF, (data->FillColor >> 8) & 0xFF, (data->FillColor >> 16) & 0xFF)); // RA2: as above
			}
			int mode = data->PictureMode; // RA2: as above
			bool typed = (mode == 1); // RA2: as above
			if (mode == 0 || typed) { // RA2: as above
				if (data->Text && (mode == 0 || (typed && data->IsPlaying))) { // RA2: as above
					LONG style = GetWindowLongA(window, GWL_STYLE); // RA2: as above
					int flags = 16; // RA2: as above
					if (style & SS_CENTER) {
						flags = 17; // RA2: as above
					} else if (style & SS_RIGHT) {
						flags = 18; // RA2: as above
					}
					COLORREF color = data->TextColor; // RA2: as above
					if (style & WS_DISABLED) {
						color = Disabled_Text_Color; // RA2: as above
					}
					Draw_Text_In_Rect_Ex(AlternateSurface, data->Text, &area, data->Font, color, flags, data->TextFlags, 0, data->Typed, (short)data->TypeExtra); // RA2: as above
					if (data->TypingSound != -1) { // RA2: as above
						VocClass::PlayGlobal(data->TypingSound, 1.0f, 0x2000, 0); // RA2: as above
					}
					if (typed) { // RA2: as above
						int length = wcslen(data->Text) + 1 + data->TypeExtra; // RA2: as above
						if (data->Typed < length) { // RA2: as above
							data->Typed += data->TypeStep; // RA2: as above
							if (data->Typed >= length) { // RA2: as above
								KillTimer(window, 0);
							}
						}
					}
				}
			} else {
				bool animated = (mode == 4); // RA2: as above
				if ((mode == 2 || mode == 3 || animated) && (int)(GetTickCount() - data->StartTime) > data->Delay) { // RA2: as above
					Rect bounds(area.left, area.top, area.right - area.left, area.bottom - area.top); // RA2: as above
					RECT client;
					GetClientRect(window, &client);
					Rect source(client.left, client.top, client.right - client.left, client.bottom - client.top); // RA2: as above
					AlternateSurface->Blit_From(bounds, *data->Back, source, false, true); // RA2: as above
					if (mode == 2) { // RA2: as above
						Surface * image = data->Image; // RA2: as above
						if (image) {
							int width = image->Get_Width();
							int height = image->Get_Height();
							Rect rect = bounds; // RA2: as above
							if (width < bounds.Width) { // RA2: as above
								rect.X = (bounds.Width - width) / 2 + bounds.X; // RA2: as above
								rect.Width = width;
							}
							if (height < bounds.Height) { // RA2: as above
								rect.Y = bounds.Y + (bounds.Height - height) / 2; // RA2: as above
								rect.Height = height;
							}
							SurfaceCache.DrawTrans(rect, *AlternateSurface, *image, (short)DSurface::Build_Hicolor_Pixel(255, 0, 255)); // RA2: as above
							ValidateRect(window, 0);
							return(0);
						}
					} else if ((mode == 3 || animated) && data->Shape) { // RA2: as above
						ShapeSet * shape = data->Shape; // RA2: as above
						ConvertClass * drawer = data->Drawer; // RA2: as above
						Point2D point; // RA2: as above
						point.X = bounds.X; // RA2: as above
						if (shape->Get_Width() < bounds.Width) { // RA2: as above
							point.X = (bounds.Width - shape->Get_Width()) / 2 + bounds.X; // RA2: as above
						}
						point.Y = bounds.Y; // RA2: as above
						if (shape->Get_Height() < bounds.Height) { // RA2: as above
							point.Y = bounds.Y + (bounds.Height - shape->Get_Height()) / 2; // RA2: as above
						}
						Rect window_rect = AlternateSurface->Get_Rect(); // RA2: as above
						int frame = data->Frame; // RA2: as above
						Draw_Shape(*AlternateSurface, *drawer, shape, frame, point, window_rect, ShapeFlags_Type(0x400), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
						if (animated && data->IsPlaying) { // RA2: as above
							int next = frame + 1; // RA2: as above
							if (next >= data->FrameCount) { // RA2: as above
								next = 0; // RA2: as above
							}
							data->Frame = next; // RA2: as above
							if (data->Notify) { // RA2: as above
								SendMessageA(data->Notify, WM_USER + 216, next, (LPARAM)window); // RA2: the new frame
								ValidateRect(window, 0);
								return(0);
							}
						}
					}
				}
			}
			ValidateRect(window, 0);
			return(0);
		}

		case WM_TIMER: // RA2: as above
			if (wparam == 0x65) { // RA2: as above
				if (!data->Movie) { // RA2: as above
					return(0);
				}
				if (data->Movie->AdvanceFrame()) { // RA2: as above
					InvalidateRect(window, 0, FALSE); // RA2: as above
				}
				if (!data->Movie->FramesLeft()) { // RA2: as above
					return(0);
				}
				if (data->IsLooping) { // RA2: as above
					data->Movie->SeekToFrame(1); // RA2: as above
					DebugString("Looping movie\n"); // RA2: as above
					return(0);
				}
				Stop_Static_Movie(window, data); // RA2: as above
				return(0);
			} else {
				int mode = data->PictureMode; // RA2: as above
				bool once; // RA2: as above
				if (mode == 1) { // RA2: as above
					InvalidateRect(window, 0, TRUE); // RA2: as above
					return(0);
				}
				if (mode == 2 || mode == 3) { // RA2: as above
					once = true; // RA2: as above
				} else {
					once = false; // RA2: as above
					if (mode != 4) return(0); // RA2: as above
				}
				if ((int)(GetTickCount() - data->StartTime) <= data->Delay) { // RA2: as above
					return(0);
				}
				if (once || (mode == 4 && data->IsPlaying)) { // RA2: as above
					InvalidateRect(window, 0, TRUE); // RA2: as above
					if (once) { // RA2: as above
						KillTimer(window, 0);
						return(0);
					}
				}
				if (data->FrameCount) { // RA2: as above
					return(0);
				}
				KillTimer(window, 0);
				if (!data->Shape) { // RA2: as above
					return(0);
				}
				data->FrameCount = data->Shape->Get_Count(); // RA2: as above
				SetTimer(window, 0, data->FrameCount, 0); // RA2: as above
				return(0);
			}

		case OD_SUBCLASSED: {
			WNDPROC proc = 0; // RA2: as above
			Window_Proc_Table.getValue(window, proc); // RA2: as above
			HWND parent = GetParent(window);
			data->PictureMode = 0; // RA2: as above
			Record_Kind((unsigned int)parent); // RA2: as above
			GetDlgCtrlID(window); // RA2: as above
			data->TextFlags = 0xC; // RA2: as above
			data->TextColor = Text_Color; // RA2: as above
			return(0);
		}

		case OD_SETCOLOR:
			if ((COLORREF)lparam != data->TextColor) { // RA2: as above
				InvalidateRect(window, 0, FALSE); // RA2: as above
			}
			if (lparam == -1) {
				data->TextColor = Text_Color; // RA2: as above
				return(0);
			}
			data->TextColor = lparam; // RA2: as above
			return(0);

		case WM_USER + 177: // RA2: a solid background colour
			data->IsFilled = true; // RA2: as above
			data->FillColor = lparam; // RA2: as above
			return(0);

		case OD_SETTEXTW: // RA2: as above
		case WM_USER + 180: // RA2: the text changed: the background is put back
			if (data->Back) { // RA2: as above
				RECT display; // RA2: as above
				Get_Display_Rect(window, &display); // RA2: as above
				RECT client;
				GetClientRect(window, &client);
				Rect destination(display.left, display.top, client.right + 1, client.bottom + 1); // RA2: as above
				Rect source(0, 0, client.right + 1, client.bottom + 1); // RA2: as above
				AlternateSurface->Blit_From(destination, *data->Back, source, false, true); // RA2: as above
				InvalidateRect(window, 0, FALSE); // RA2: as above
			}
			return(1);

		case WM_USER + 211: { // RA2: starts the animation
			if (!data->Shape || data->PictureMode != 4 || data->IsPlaying) { // RA2: as above
				return(0);
			}
			data->IsPlaying = true; // RA2: as above
			HWND parent = GetParent(window);
			int kind = Record_Kind((unsigned int)parent); // RA2: as above
			int id = GetDlgCtrlID(window); // RA2: as above
			if (!((kind == 0x94 || kind == 0xd8 || kind == 0xf5 || kind == 0xe2 || kind == 0xd5 || kind == 0x101 || kind == 0x129 || kind == 0xd7 || kind == 0xbb || kind == 0x100 || kind == 0xd6 || kind == 0x125 || kind == 0x122 || kind == 0x112 || kind == 0xe7 || kind == 0x116 || kind == 0x11d || kind == 0x11c || kind == 0xfe || kind == 0x10f || kind == 0x117 || kind == 0x114 || kind == 0xe6 || kind == 0xf3 || kind == 0xf4 || kind == 700 || kind == 0x10e || kind == 0xbc6 || kind == 0xbc7) && id == 0x71c)) { // RA2: as above
				bool slow; // RA2: as above
				if (kind == 0x94) { // RA2: as above
					if (id == 0x6ea || id == 0x6ec) { // RA2: as above
						SetTimer(window, 0, 100, 0); // RA2: as above
						return(0);
					}
					slow = (id == 0x6eb); // RA2: as above
				} else if (kind == 0x103 || kind == 0xbc7) { // RA2: as above
					slow = (id == 0x72b); // RA2: as above
				} else {
					slow = false; // RA2: as above
				}
				if (!slow) { // RA2: as above
					SetTimer(window, 0, 0, 0); // RA2: as above
					return(0);
				}
			}
			SetTimer(window, 0, 100, 0); // RA2: as above
			return(0);
		}

		case WM_USER + 212: // RA2: stops the animation
			if (data->PictureMode != 4 || !data->IsPlaying) { // RA2: as above
				return(0);
			}
			KillTimer(window, 0);
			data->IsPlaying = false; // RA2: as above
			return(0);

		case WM_USER + 213: // RA2: sets the animation frame
			if (data->PictureMode == 4) { // RA2: as above
				data->Frame = lparam; // RA2: as above
				InvalidateRect(window, 0, TRUE); // RA2: as above
				return(0);
			}
			break;

		case WM_USER + 214: // RA2: the animation frame
			if (data->PictureMode != 4) { // RA2: as above
				return(-1);
			}
			return(data->Frame); // RA2: as above

		case WM_USER + 215: // RA2: the window told of new frames
			if (data->PictureMode == 4) { // RA2: as above
				data->Notify = (HWND)lparam; // RA2: as above
				return(0);
			}
			break;

		case WM_USER + 223: // RA2: plays the movie of the Movies list at wparam
			if ((int)wparam >= 0 && (int)wparam < Movies.Count()) { // RA2: as above
				char const * name = Movies[wparam]; // RA2: as above
				Stop_Static_Movie(window, data); // RA2: as above
				RECT display; // RA2: as above
				Get_Display_Rect(window, &display); // RA2: as above
				data->Movie = OwnerDraw_InitMovieHandle(name, AlternateSurface, 0); // RA2: as above
				if (data->Movie) { // RA2: as above
					data->Movie->SetPosition(display.left, display.top); // RA2: as above
					MoveWindow(window, display.left, display.top, ((int *)data->Movie)[2], ((int *)data->Movie)[3], FALSE); // RA2: as above
					SetTimer(window, 0x65, 0x22, 0); // RA2: as above
					return(0);
				}
				Stop_Static_Movie(window, data); // RA2: as above
				return(0);
			}
			break;

		case WM_USER + 224: // RA2: pauses the movie
			if (data->Movie) { // RA2: as above
				data->Movie->Pause(true); // RA2: as above
				return(0);
			}
			break;

		case WM_USER + 225: // RA2: resumes the movie
			if (data->Movie) { // RA2: as above
				data->Movie->Pause(false); // RA2: as above
				return(0);
			}
			break;

		case WM_USER + 226: // RA2: stops the movie
			Stop_Static_Movie(window, data); // RA2: as above
			return(0);

		case WM_USER + 227: // RA2: the movie loops
			data->IsLooping = wparam; // RA2: as above
			break;

		case WM_USER + 228: { // RA2: plays the named movie
			Stop_Static_Movie(window, data); // RA2: as above
			RECT display; // RA2: as above
			Get_Display_Rect(window, &display); // RA2: as above
			data->Movie = OwnerDraw_InitMovieHandle((char const *)lparam, AlternateSurface, 0); // RA2: as above
			if (data->Movie) { // RA2: as above
				data->Movie->SetPosition(display.left, display.top); // RA2: as above
				MoveWindow(window, display.left, display.top, ((int *)data->Movie)[2], ((int *)data->Movie)[3], FALSE); // RA2: as above
				SetTimer(window, 0x65, 0x22, 0); // RA2: as above
				return(0);
			}
			Stop_Static_Movie(window, data); // RA2: as above
			return(0);
		}

		case WM_USER + 238: // RA2: starts typing the text out
			if (data->PictureMode == 1 && !data->IsPlaying) { // RA2: as above
				data->IsPlaying = true; // RA2: as above
				data->Typed = 1; // RA2: as above
				SetTimer(window, 0, data->TypeRate, 0); // RA2: as above
				InvalidateRect(window, 0, FALSE); // RA2: as above
				return(0);
			}
			break;

		case WM_USER + 240: // RA2: draws the movie's frame
			if (data->Movie) { // RA2: as above
				data->Movie->Blit(); // RA2: as above
				return(0);
			}
			break;
	}

	WNDPROC proc = 0; // RA2: as above
	Window_Proc_Table.getValue(window, proc); // RA2: as above
	return(CallWindowProcA(proc, window, message, wparam, lparam)); // RA2: as above
}
#endif

}


/// <summary>
/// Handles the messages for an owner-drawn list box.
/// Besides repainting the stock list box, this routine provides the multi-column list the
/// dialogs are built around: the columns and the text, color and icon of each cell live in
/// the control's owner-draw record. A scroll bar is attached to, or taken away from, the
/// list as its contents demand.
/// </summary>
/// <returns>Returns with the result of the original window procedure for anything this
/// routine does not handle itself.</returns>
namespace OwnerDraw { // RA2: as above
LRESULT __stdcall Checkbox_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam) // RA2: as above
{
    unsigned int key=(unsigned int)window; // RA2: as above
    WinData *record=0; // RA2: as above
    WinData *&data=record; // RA2: as above
    Control_Table.getPointer(key, &data); // RA2: as above
    if(data) { // RA2: as above
    switch(message) { // RA2: as above
		case WM_PAINT: // RA2: as above
        if(data->SkipCheckPaint)break; // RA2: as above
        {
            RECT client;
            struct PaintBounds { RECT display; RECT text; } bounds; // RA2: as above
            GetClientRect(window,&client); // RA2: as above
            Get_Display_Rect(window,&bounds.text); // RA2: as above
            Get_Display_Rect(window,&bounds.display); // RA2: as above
            Rect destination; // RA2: as above
            destination=Rect(bounds.display.left,bounds.display.top,bounds.display.right-bounds.display.left,bounds.display.bottom-bounds.display.top); // RA2: as above
            destination.Width=18; // RA2: as above
            destination.Height=18; // RA2: as above
				LONG style = GetWindowLongA(window, GWL_STYLE); // RA2: as above
            int state=data->ItemState; // RA2: as above
            bool special=data->IsCheckAltArt; // RA2: as above
            bool alternate=data->IsCheckAltSelected; // RA2: as above
				bool checked = state == BST_CHECKED; // RA2: as above
            char image_name[64]; // RA2: as above
            if(special) { // RA2: as above
                if(checked) { // RA2: as above
                    if(alternate)strcpy(image_name,"cce_i.pcx"); // RA2: as above
                    else strcpy(image_name,"cce_il.pcx"); // RA2: as above
                } else {
                    if(alternate)strcpy(image_name,"cce_ir.pcx"); // RA2: as above
                    else strcpy(image_name,"cue_i.pcx"); // RA2: as above
                }
            } else {
                char letter='u'; // RA2: as above
                if(checked)letter='c'; // RA2: as above
                sprintf(image_name,"c%ce_i.pcx",letter); // RA2: as above
            }
            Surface *image=SurfaceCache.GetSurface(image_name,0); // RA2: as above
            int height=image->Get_Height(); // RA2: as above
            Rect source(0,0,image->Get_Width(),height); // RA2: as above
            AlternateSurface->Blit_From(destination,*image,source,false,true); // RA2: as above
				if (style & WS_DISABLED) Blend_Rect(&destination, AlternateSurface, 0, Dim_Alpha); // RA2: as above
            wchar_t *text=data->Text; // RA2: as above
            bounds.text.left+=26; // RA2: as above
            if(text) { // RA2: as above
                unsigned int color=Text_Color; // RA2: as above
					if (style & WS_DISABLED) color = Disabled_Text_Color; // RA2: as above
                Draw_Text_In_Rect_Ex(AlternateSurface,text,&bounds.text,data->Font,color,4,0xc,0,0,0); // RA2: as above
            }
            ValidateRect(window,0); // RA2: as above
        }
zero_result: // RA2: as above
        return 0; // RA2: as above
case BM_GETCHECK: return data->ItemState; // RA2: as above
case BM_SETCHECK: // RA2: as above
        data->ItemState=wparam; // RA2: as above
        InvalidateRect(window,0,0); // RA2: as above
        return 0; // RA2: as above
case WM_SETFOCUS:case WM_KILLFOCUS: InvalidateRect(window, 0, 0); break; // RA2: as above
case WM_LBUTTONDOWN:case WM_LBUTTONDBLCLK: { // RA2: as above
        Point2D click((unsigned int)lparam & 0xffff,(unsigned int)lparam>>16); // RA2: as above
        if(click.X>=18)goto zero_result; // RA2: as above
        if(click.Y>=18)goto zero_result; // RA2: as above
	int checked = data->ItemState != BST_CHECKED; // RA2: as above
        data->ItemState=checked; // RA2: as above
        InvalidateRect(window,0,0); // RA2: as above
        VocClass::PlayGlobal(Rule->GUICheckboxSound,1.0f,0x2000,0); // RA2: as above
        HWND parent=GetParent(window); // RA2: as above
	SendMessageA(parent, WM_COMMAND, (unsigned int)(unsigned short)GetWindowLongA(window, GWL_ID) | ((unsigned int)(unsigned short)checked << 16), (LPARAM)window); // RA2: as above
        return 0; // RA2: as above
    }
case OD_SUBCLASSED: { // RA2: as above
        WNDPROC previous=0; // RA2: as above
        Window_Proc_Table.getValue(window, previous); // RA2: as above
	data->ItemState = CallWindowProcA(previous, window, BM_GETCHECK, 0, 0); // RA2: as above
        break;
    }
    case 0x4e5: { // RA2: as above
        bool previous=data->IsCheckAltSelected; // RA2: as above
        data->IsCheckAltArt=lparam!=0; // RA2: as above
        if(previous!=data->IsCheckAltArt)InvalidateRect(window,0,0); // RA2: as above
        break;
    }
    case 0x4e6: { // RA2: as above
        bool previous=data->IsCheckAltSelected; // RA2: as above
        data->IsCheckAltSelected=lparam!=0; // RA2: as above
        if(previous!=data->IsCheckAltSelected)InvalidateRect(window,0,0); // RA2: as above
        break;
    }
    case 0x4e7:return data->IsCheckAltSelected; // RA2: as above
    }
    WNDPROC previous=0; // RA2: as above
    Window_Proc_Table.getValue(window, previous); // RA2: as above
    return CallWindowProcA(previous,window,message,wparam,lparam); // RA2: as above
    }
    return 0; // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above
LRESULT __stdcall Button_Proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam) // RA2: as above
{
    WinData *data=0; // RA2: as above
    Control_Table.getPointer((unsigned int)window, &data); // RA2: as above
    RECT display; memset(&display,0,sizeof(display)); // RA2: as above
    Get_Display_Rect(window,&display); // RA2: as above
    Rect origin; // RA2: as above
    origin.X=display.left;origin.Y=display.top; // RA2: as above
    origin.Width=display.right-display.left;origin.Height=display.bottom-display.top; // RA2: as above
    RECT client; memset(&client,0,sizeof(client)); // RA2: as above
    GetClientRect(window,&client); // RA2: as above
    int state=data->ItemState; // RA2: as above
	LONG style = GetWindowLongA(window, GWL_STYLE); // RA2: as above
    switch(message) { // RA2: as above
		case WM_PAINT: { // RA2: as above
        Rect rect=origin; // RA2: as above
        unsigned long color=Text_Color; // RA2: as above
        if(!data->Back) { // RA2: as above
            BSurface *captured=new BSurface(client.right+1,client.bottom+1,2); // RA2: as above
            data->Back=captured; // RA2: as above
            ++Surface_Count; // RA2: as above
            Rect destination,source; // RA2: as above
            destination.X=0;destination.Y=0; // RA2: as above
            destination.Width=client.right+1;destination.Height=client.bottom+1; // RA2: as above
            source.X=display.left;source.Y=display.top; // RA2: as above
            source.Width=client.right+1;source.Height=client.bottom+1; // RA2: as above
            captured->Blit_From(destination,*AlternateSurface,source,false,true); // RA2: as above
        }
        if(data->Image) { // RA2: as above
            Surface *image=data->Image; // RA2: as above
            if((state & 1) && data->AlternateImage)image=data->AlternateImage; // RA2: as above
            Rect source=rect; // RA2: as above
            source.Y=source.X=0; // RA2: as above
            AlternateSurface->Blit_From(rect,*image,source,false,true); // RA2: as above
        } else {
            char updown='u'; // RA2: as above
            if(state & 1)updown='d'; // RA2: as above
				if (style & WS_DISABLED) updown = 'u'; // RA2: as above
            int widths[2]={7,7}; // RA2: as above
            int margins[2]={10,10}; // RA2: as above
            int heights[2]={24,30}; // RA2: as above
            unsigned index=0; // RA2: as above
            for(unsigned i=0;i<2;i++) { // RA2: as above
                if(heights[i]>rect.Height && i!=0)break; // RA2: as above
                index=i; // RA2: as above
            }
            int width=widths[index]; // RA2: as above
            int height=heights[index]; // RA2: as above
            int margin=margins[index]; // RA2: as above
            if(data->Back) { // RA2: as above
                Rect source,destination; // RA2: as above
                source.X=0;source.Y=0; // RA2: as above
                source.Width=client.right+1;source.Height=client.bottom+1; // RA2: as above
                destination.X=display.left;destination.Y=display.top; // RA2: as above
                destination.Width=client.right+1;destination.Height=client.bottom+1; // RA2: as above
                AlternateSurface->Blit_From(destination,*data->Back,source,false,true); // RA2: as above
                InvalidateRect(window,0,0); // RA2: as above
            }
            origin.Y+=(rect.Height-height)/2; // RA2: as above
            if(state & 1)origin.Y+=2; // RA2: as above
            char name[40]; // RA2: as above
            sprintf(name,"b%c%c_li%d.pcx",updown,'e',height); // RA2: as above
            Surface *image=SurfaceCache.GetSurface(name,0); // RA2: as above
            origin.Height=image->Get_Height(); // RA2: as above
            Rect destination,source; // RA2: as above
            destination=origin; // RA2: as above
            destination.Width=width; // RA2: as above
            source.Width=width; // RA2: as above
            source.Y=0; // RA2: as above
            source.X=0; // RA2: as above
            destination.Height=height; // RA2: as above
            source.Height=height; // RA2: as above
            AlternateSurface->Blit_From(destination,*image,source,false,true); // RA2: as above
            sprintf(name,"b%c%c_mi%d.pcx",updown,'e',height); // RA2: as above
            image=SurfaceCache.GetSurface(name,0); // RA2: as above
            Rect middle=origin; // RA2: as above
            middle.X+=width; // RA2: as above
            middle.Width-=margin; // RA2: as above
            middle.Height=image->Get_Height(); // RA2: as above
            SurfaceCache.Draw(middle, *AlternateSurface, *image, 0, 0); // RA2: as above
            sprintf(name,"b%c%c_ri%d.pcx",updown,'e',height); // RA2: as above
            image=SurfaceCache.GetSurface(name,0); // RA2: as above
            destination=origin; // RA2: as above
            destination.X+=origin.Width-margin; // RA2: as above
            destination.Width=margin; // RA2: as above
            destination.Height=image->Get_Height(); // RA2: as above
            source.Width=destination.Width; // RA2: as above
            source.Height=destination.Height; // RA2: as above
            source.Y=0; // RA2: as above
            source.X=0; // RA2: as above
            AlternateSurface->Blit_From(destination,*image,source,false,true); // RA2: as above
        }
        if(!data->Image && data->Text) { // RA2: as above
            RECT unused_client; // RA2: as above
            GetClientRect(window,&unused_client); // RA2: as above
            RECT text_rect;
            text_rect.left=origin.X; // RA2: as above
            text_rect.top=origin.Y+1; // RA2: as above
            text_rect.right=origin.Width+origin.X-2; // RA2: as above
            text_rect.bottom=origin.Height+origin.Y-2; // RA2: as above
            if(state & 1) {text_rect.left+=2;text_rect.top+=4;} // RA2: as above
            Draw_Text_In_Rect_Ex(AlternateSurface,data->Text,&text_rect,data->Font,color,5,0xc,0,0,0); // RA2: as above
        }
			if (style & WS_DISABLED) Blend_Rect(&rect, AlternateSurface, 0, 128); // RA2: as above
        ValidateRect(window,0); // RA2: as above
        return 0; // RA2: as above
    }
		case BM_GETCHECK: return data->ItemState; // RA2: as above
		case BM_SETCHECK: // RA2: as above
        data->ItemState=wparam; // RA2: as above
        InvalidateRect(window,0,1); // RA2: as above
        break;
		case OD_SUBCLASSED: { // RA2: as above
        WNDPROC previous=0; // RA2: as above
        Window_Proc_Table.getValue(window, previous); // RA2: as above
			data->ItemState = CallWindowProcA(previous, window, BM_GETCHECK, 0, 0); // RA2: as above
        break;
    }
		case WM_LBUTTONDOWN:case WM_LBUTTONDBLCLK: // RA2: as above
        if(!data->ItemState) { // RA2: as above
            data->ItemState=1; // RA2: as above
            InvalidateRect(window,0,1); // RA2: as above
            VocClass::PlayGlobal(Rule->GenericClick,1.0f,0x2000,0); // RA2: as above
            break;
        }
        return 0; // RA2: as above
		case WM_LBUTTONUP: { // RA2: as above
        LockWindowUpdate(GetParent(window)); // RA2: as above
        WNDPROC previous=0; // RA2: as above
        Window_Proc_Table.getValue(window, previous); // RA2: as above
			LRESULT result = CallWindowProcA(previous, window, WM_LBUTTONUP, wparam, lparam); // RA2: as above
        LockWindowUpdate(0); // RA2: as above
        return result; // RA2: as above
    }
    }
    WNDPROC previous=0; // RA2: as above
    Window_Proc_Table.getValue(window, previous); // RA2: as above
    return CallWindowProcA(previous,window,message,wparam,lparam); // RA2: as above
}
}


#pragma intrinsic(log) // RA2: as above

	/*
	 * Keep the attached scrollbar synchronized with the listbox state. This is skipped for
	 * the few messages that are queried while computing the scroll state (to avoid recursion).
	 */
namespace OwnerDraw { // RA2: as above

void __fastcall Draw_Edge_Glows(Surface *surface, Rect const &rect, int raised, int count, // RA2: as above
                                               int left_alpha, int top_alpha, int right_alpha, int bottom_alpha); // RA2: as above

struct ScrollBarRecord { // RA2: as above
    char SubclassParam[0x08]; // RA2: as above
    HWND ScrollOwner; // RA2: as above
    char Reserved_0C[0x10 - 0x0c]; // RA2: as above
    Surface *Back; // RA2: as above
    char Reserved_14[0x20 - 0x14]; // RA2: as above
    int SkipCheckPaint; // RA2: as above
    char Reserved_24[0xbc - 0x24]; // RA2: as above
    bool IsDisabled; // RA2: as above
    char IsHidden[0xc8 - 0xbd]; // RA2: as above
    int TextStyle; // RA2: as above
    char Reserved_CC; // RA2: as above
    bool IsAlternateColors; // RA2: as above
    char Reserved_CE[0xe8 - 0xce]; // RA2: as above
    int result; // RA2: as above
    int dragging; // RA2: as above
    int range; // RA2: as above
    int position; // RA2: as above
    int up_pressed; // RA2: as above
    int down_pressed; // RA2: as above
    int keep_capture; // RA2: as above
};

LRESULT __stdcall Scroll_Bar_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    RECT client_rect;
    RECT display_rect;
    GetClientRect(window, &client_rect);
    Get_Display_Rect(window, &display_rect);
    client_rect.bottom -= 2 * Border_Width; // RA2: as above
    client_rect.right -= 2 * Border_Width; // RA2: as above
    display_rect.left += Border_Width; // RA2: as above
    display_rect.right -= Border_Width; // RA2: as above
    display_rect.top += Border_Width; // RA2: as above
    display_rect.bottom -= Border_Width; // RA2: as above
    ScrollBarRecord *data = (ScrollBarRecord *)Get_Record(window); // RA2: as above
    if (data == 0) { // RA2: as above
        return 0; // RA2: as above
    }
    bool keep_parent_capture = false;
    if (data->keep_capture) { // RA2: as above
        keep_parent_capture = true;
    }
    int message_result = data->result; // RA2: as above
    int dragging = data->dragging; // RA2: as above
    int range = data->range; // RA2: as above
    int position = data->position; // RA2: as above
    int up_pressed = data->up_pressed; // RA2: as above
    int down_pressed = data->down_pressed; // RA2: as above
    if (!range) {
        range = 100;
    }
    int grip_top = 0;
    int grip_bottom = 0;
	int scroll_code = SB_LINEUP;
    int width = client_rect.right - client_rect.left;
    int travel = client_rect.bottom - client_rect.top - 44; // RA2: as above
    int grip_height = (int)((double)travel - log((double)(range + 1)) * (double)travel * 0.2); // RA2: as above
    if (grip_height <= 14) {
        grip_height = 14;
    }
    travel -= grip_height; // RA2: as above
    if (travel <= 1) {
        travel = 1;
    }
	if (message < WM_USER || message == 0x4a5) { // RA2: as above
        if (!dragging) {
            grip_top = position * travel / range + client_rect.top + 22;
            grip_bottom = grip_top + grip_height;
        } else {
            POINT cursor;
            GetCursorPos(&cursor);
            ScreenToClient(window, &cursor);
            grip_top = cursor.y - grip_height / 2;
            if (grip_top < 22) {
                grip_top = 22;
            }
            if (client_rect.bottom - grip_height - 22 < grip_top) {
                grip_top = client_rect.bottom - grip_height - 22;
            }
            grip_bottom = grip_top + grip_height;
			scroll_code = SB_THUMBTRACK; // RA2: as above
            position = range * (grip_top - 22) / travel;
        }
    }
    switch (message) {
		case SBM_GETPOS: // RA2: as above
        return position; // RA2: as above
		case SBM_SETPOS: // RA2: as above
        if ((int)wparam <= range && (int)wparam > 0) {
            position = (int)wparam;
        }
        break;
		case SBM_SETRANGE: // RA2: as above
        range = (int)lparam;
        if (position > range) {
            position = range;
        }
        break;
		case SBM_SETSCROLLINFO: { // RA2: as above
        SCROLLINFO *info = (SCROLLINFO *)lparam; // RA2: as above
        range = info->nMax;
        position = info->nPos;
        break;
    }
		case WM_NCHITTEST: // RA2: as above
		case WM_GETDLGCODE: { // RA2: as above
        WNDPROC proc = 0; // RA2: as above
        Window_Proc_Table.getValue(window, proc); // RA2: as above
        return CallWindowProcA(proc, window, message, wparam, lparam); // RA2: as above
    }
		case WM_ERASEBKGND: // RA2: as above
        return 0; // RA2: as above
		case WM_PAINT: { // RA2: as above
        if (data->IsDisabled) { // RA2: as above
            ValidateRect(window, 0);
            break;
        }
        if (data->SkipCheckPaint) { // RA2: as above
            return 0; // RA2: as above
        }
        Rect src_rect(0, 0, client_rect.right, client_rect.bottom); // RA2: as above
        Rect full_rect(display_rect.left, display_rect.top, client_rect.right, client_rect.bottom); // RA2: as above
        HWND parent = GetParent(window);
        WinData *parent_data = 0; // RA2: as above
        if (parent) {
            Control_Table.getPointer((unsigned int const &)parent, &parent_data); // RA2: as above
        }
        RECT parent_display;
        Get_Display_Rect(parent, &parent_display);
        Rect source_rect = src_rect;
        if (parent_data && parent_data->Back) { // RA2: as above
            source_rect.X = display_rect.left + source_rect.X - parent_display.left;
            source_rect.Y += display_rect.top - parent_display.top;
        }
        if (data->Back) { // RA2: as above
            if (data->Back->Get_Width() != client_rect.right || data->Back->Get_Height() != client_rect.bottom) { // RA2: as above
                delete data->Back; // RA2: as above
                data->Back = 0; // RA2: as above
            }
        }
        if (!data->Back) { // RA2: as above
            BSurface *background = new BSurface(client_rect.right, client_rect.bottom, 2); // RA2: as above
            data->Back = background; // RA2: as above
            Surface_Count++; // RA2: as above
            if (parent_data && parent_data->Back) { // RA2: as above
                background->Blit_From(src_rect, *parent_data->Back, source_rect, false, true); // RA2: as above
            }
            int count = client_rect.bottom * client_rect.right; // RA2: as above
            unsigned short *pixels = (unsigned short *)background->Lock(); // RA2: as above
            for (int i = 0; i < count; i++) {
                pixels[i] = Blend_Color(pixels[i], 0xffff, data->TextStyle); // RA2: as above
            }
            if (pixels) { // RA2: as above
                background->Unlock();
            }
        }
        if (parent_data && parent_data->Back) { // RA2: as above
            AlternateSurface->Blit_From(full_rect, *parent_data->Back, source_rect, false, true); // RA2: as above
        }
        int color = Color_To_Pixel(Default_Frame_Color); // RA2: as above
        if (data->IsAlternateColors) { // RA2: as above
            color = Color_To_Pixel(Alternate_Frame_Color); // RA2: as above
        }
        Draw_Frame(AlternateSurface, full_rect, Border_Width, color); // RA2: as above
        Rect grip_rect(display_rect.left, grip_top + display_rect.top, client_rect.right, grip_bottom - grip_top); // RA2: as above
        full_rect = grip_rect; // RA2: as above
        src_rect = grip_rect; // RA2: as above
        src_rect.X = 0;
        src_rect.Y = grip_top;
        Surface *grip_middle; // RA2: as above
        if (data->IsAlternateColors) { // RA2: as above
            grip_middle = SurfaceCache.GetSurface("gsbgripm.pcx", 0); // RA2: as above
        } else {
            grip_middle = SurfaceCache.GetSurface("sbgripm.pcx", 0); // RA2: as above
        }
        if (grip_middle) { // RA2: as above
            grip_rect.Width = grip_middle->Get_Width(); // RA2: as above
        }
        SurfaceCache.Draw(grip_rect, *AlternateSurface, *grip_middle, 0, 0); // RA2: as above
        Rect grip_src(0, 0, grip_middle->Get_Height(), grip_middle->Get_Width()); // RA2: as above
        Surface *grip_top_image; // RA2: as above
        if (data->IsAlternateColors) { // RA2: as above
            grip_top_image = SurfaceCache.GetSurface("gsbgript.pcx", 0); // RA2: as above
        } else {
            grip_top_image = SurfaceCache.GetSurface("sbgript.pcx", 0); // RA2: as above
        }
        if (grip_top_image) { // RA2: as above
            full_rect.Height = grip_top_image->Get_Height(); // RA2: as above
        }
        AlternateSurface->Blit_From(full_rect, *grip_top_image, grip_src, false, true); // RA2: as above
        Surface *grip_bottom_image; // RA2: as above
        if (data->IsAlternateColors) { // RA2: as above
            grip_bottom_image = SurfaceCache.GetSurface("gsbgripb.pcx", 0); // RA2: as above
        } else {
            grip_bottom_image = SurfaceCache.GetSurface("sbgripb.pcx", 0); // RA2: as above
        }
        if (grip_bottom_image) { // RA2: as above
            full_rect.Y = display_rect.top + grip_bottom - grip_bottom_image->Get_Height(); // RA2: as above
        }
        AlternateSurface->Blit_From(full_rect, *grip_bottom_image, grip_src, false, true); // RA2: as above
        Rect up_rect(display_rect.left, display_rect.top, client_rect.right, 22); // RA2: as above
        full_rect = up_rect; // RA2: as above
        src_rect = up_rect; // RA2: as above
        src_rect.X = 0;
        src_rect.Y = 0;
        AlternateSurface->Blit_From(full_rect, *data->Back, src_rect, false, true); // RA2: as above
        Draw_Edge_Glows(AlternateSurface, up_rect, up_pressed == 0, 2, Glow_Color, // RA2: as above
                                       Glow_Color, Glow_Color, Glow_Color); // RA2: as above
        Draw_Arrow(AlternateSurface, up_rect, 1, up_pressed, data->IsAlternateColors); // RA2: as above
        Rect down_rect(display_rect.left, display_rect.bottom - 22, client_rect.right, 22); // RA2: as above
        full_rect = down_rect; // RA2: as above
        src_rect = down_rect; // RA2: as above
        src_rect.X = 0;
        src_rect.Y = client_rect.bottom - 22;
        AlternateSurface->Blit_From(full_rect, *data->Back, src_rect, false, true); // RA2: as above
        Draw_Edge_Glows(AlternateSurface, down_rect, down_pressed == 0, 2, Glow_Color, // RA2: as above
                                       Glow_Color, Glow_Color, Glow_Color); // RA2: as above
        Draw_Arrow(AlternateSurface, down_rect, 0, down_pressed, data->IsAlternateColors); // RA2: as above
        ValidateRect(window, 0);
        break;
    }
		case WM_TIMER: { // RA2: as above
        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(window, &cursor);
        up_pressed = 0;
        down_pressed = 0;
        if (message_result && cursor.x > client_rect.right - width) {
            if (cursor.y < 22) {
                if (!dragging) {
                    up_pressed = 1;
                }
                if (position != 0) {
						scroll_code = SB_LINEUP; // RA2: as above
                    position--; // RA2: as above
                }
            } else if (cursor.y > client_rect.bottom - 22) {
                if (!dragging) {
                    down_pressed = 1;
                }
                if (position + 1 <= range) {
						scroll_code = SB_LINEDOWN; // RA2: as above
                    position++; // RA2: as above
                }
            }
        }
        SetTimer(window, 0, 0x19, 0); // RA2: as above
        break;
    }
		case WM_MOUSEMOVE: // RA2: as above
        if (dragging) {
            RECT area = client_rect; // RA2: as above
            area.left = client_rect.right - width; // RA2: as above
            InvalidateRect(window, &area, 0); // RA2: as above
        }
			if (wparam & MK_LBUTTON) { // RA2: as above
            break;
        }
		case WM_LBUTTONUP: // RA2: as above
        message_result = 0;
        dragging = 0;
        if (up_pressed || down_pressed) {
            InvalidateRect(window, 0, 0); // RA2: as above
        }
        up_pressed = 0;
        down_pressed = 0;
        KillTimer(window, 0);
        ReleaseCapture();
        if (keep_parent_capture) {
            SetCapture(data->ScrollOwner); // RA2: as above
        }
			scroll_code = SB_ENDSCROLL; // RA2: as above
        break;
		case WM_LBUTTONDOWN: // RA2: as above
		case WM_LBUTTONDBLCLK: { // RA2: as above
		if (message == WM_LBUTTONDOWN) { // RA2: as above
            message_result = 1;
            SetCapture(window);
            SetTimer(window, 0, 500, 0); // RA2: as above
        } else {
            message_result = 0;
            dragging = 0;
            KillTimer(window, 0);
            ReleaseCapture();
            if (keep_parent_capture) {
                SetCapture(data->ScrollOwner); // RA2: as above
            }
        }
        POINT click; // RA2: as above
        click.x = (unsigned short)lparam; // RA2: as above
        click.y = (unsigned short)(((unsigned long)lparam >> 16) & 0xffff); // RA2: as above
        int repeat = 1; // RA2: as above
		if (message == WM_LBUTTONDBLCLK) { // RA2: as above
            repeat = 2; // RA2: as above
        }
        down_pressed = 0;
        up_pressed = 0;
        while (repeat > 0) {
            if (click.x > client_rect.right - width) { // RA2: as above
                if (click.y < 22 && position) { // RA2: as above
                    up_pressed = 1;
						scroll_code = SB_LINEUP; // RA2: as above
                    position--; // RA2: as above
                } else {
                    bool skip_thumb = false; // RA2: as above
                    if (click.y > client_rect.bottom - 22) { // RA2: as above
                        if (position + 1 <= range) {
                            down_pressed = 1;
								scroll_code = SB_LINEDOWN; // RA2: as above
                            position++; // RA2: as above
                            skip_thumb = true; // RA2: as above
                        }
                    }
                    if (!skip_thumb) { // RA2: as above
                        if (click.y < grip_top || click.y >= grip_bottom) { // RA2: as above
                            grip_top = click.y - grip_height / 2; // RA2: as above
                            if (grip_top < 22) {
                                grip_top = 22;
                            }
                            if (client_rect.bottom - grip_height - 22 < grip_top) {
                                grip_top = client_rect.bottom - grip_height - 22;
                            }
                            grip_bottom = grip_top + grip_height;
								scroll_code = SB_THUMBTRACK; // RA2: as above
                            position = range * (grip_top - 22) / travel;
						} else if (message == WM_LBUTTONDOWN) { // RA2: as above
                            dragging = 1;
                        }
                    }
                }
            }
            repeat--; // RA2: as above
        }
        break;
    }
    case 0x4d2: // RA2: as above
        data->keep_capture = lparam != 0; // RA2: as above
        break;
    case 0x4f1: // RA2: as above
        data->IsAlternateColors = lparam == 1; // RA2: as above
        break;
    }
    int send_notify = 0;
    if (position != data->position || range != data->range) { // RA2: as above
        if (data->ScrollOwner) { // RA2: as above
            send_notify = 1;
        }
    }
    data->result = message_result; // RA2: as above
    data->dragging = dragging; // RA2: as above
    data->range = range; // RA2: as above
    data->position = position; // RA2: as above
    data->up_pressed = up_pressed; // RA2: as above
    data->down_pressed = down_pressed; // RA2: as above
    if (send_notify) {
		SendMessageA(data->ScrollOwner, WM_VSCROLL, (unsigned short)position << 16 | (unsigned short)scroll_code, (LPARAM)window); // RA2: as above
        InvalidateRect(window, 0, 0); // RA2: as above
    }
    return 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above
LRESULT __stdcall Progress_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    RECT rect;
    Get_Display_Rect(window, &rect);
    WinData *data = 0; // RA2: as above
    Control_Table.getPointer((unsigned int)window, &data); // RA2: as above
    if (data) {
        switch (message) {
			case OD_SUBCLASSED: // RA2: as above
            data->RangeMax = 100; // RA2: as above
            break;
			case PBM_SETRANGE: // RA2: as above
            data->ItemState = (unsigned int)lparam & 0xffff; // RA2: as above
            data->RangeMax = (unsigned int)lparam >> 16; // RA2: as above
            break;
			case PBM_SETPOS: { // RA2: as above
            int position = (int)wparam;
            if (position < data->ItemState) position = data->ItemState; // RA2: as above
            if (position > data->RangeMax) position = data->RangeMax; // RA2: as above
            data->Position = position; // RA2: as above
            InvalidateRect(window, 0, 0); // RA2: as above
            break;
        }
			case WM_PAINT: { // RA2: as above
            Rect source(0, 0, rect.right-rect.left+1, rect.bottom-rect.top+1); // RA2: as above
            Rect destination(rect.left, rect.top, rect.right-rect.left+1, rect.bottom-rect.top+1); // RA2: as above
            if (!data->Back) { // RA2: as above
                Surface *background = new BSurface(rect.right-rect.left+1, rect.bottom-rect.top+1, 2); // RA2: as above
                data->Back = background; // RA2: as above
                ++Surface_Count; // RA2: as above
                background->Blit_From(source, *AlternateSurface, destination, false, true); // RA2: as above
            }
            AlternateSurface->Blit_From(destination, *data->Back, source, false, true); // RA2: as above
            int position = (data->Position * 65536) / (data->RangeMax - data->ItemState); // RA2: as above
            int color = Color_To_Pixel(0xff); // RA2: as above
            Draw_Progress_Fill(destination, AlternateSurface, color, position); // RA2: as above
            ValidateRect(window, 0);
            break;
        }
        }
    }
    return 0; // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above

struct TrackBarState { // RA2: as above
    int result; // RA2: as above
    int dragging; // RA2: as above
    int range; // RA2: as above
    int value; // RA2: as above
    int minimum; // RA2: as above
    int thumb_pos; // RA2: as above
    int step; // RA2: as above
    int show_numbers; // RA2: as above
    int click_suppress; // RA2: as above
};

LRESULT __stdcall Trackbar_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    bool play_click = true;

    RECT client_rect;
    RECT display_rect;
    GetClientRect(window, &client_rect);
    Get_Display_Rect(window, &display_rect);
    int number_width = 50;

    WinData *data = Get_Record(window); // RA2: as above
    if (data == 0) { // RA2: as above
        return 0; // RA2: as above
    }
    TrackBarState *track = (TrackBarState *)&data->ItemState; // RA2: as above

    int message_result = track->result; // RA2: as above
    int dragging = track->dragging; // RA2: as above
    int range = track->range; // RA2: as above
    int value = track->value; // RA2: as above
    int minimum = track->minimum; // RA2: as above
    int maximum;
    int thumb_pos = track->thumb_pos; // RA2: as above
    int step = track->step; // RA2: as above
    int show_numbers = track->show_numbers; // RA2: as above

    if (!show_numbers) {
        number_width = 0;
    }

    int slider_width = client_rect.right - client_rect.left - number_width - 13;
    if (slider_width <= 1) {
        slider_width = 1;
    }

    if (!range) {
        WNDPROC proc = 0; // RA2: as above
        Window_Proc_Table.getValue(window, proc); // RA2: as above

        int range_max = CallWindowProcA(proc, window, 0x402, 0, 0); // RA2: as above
        minimum = CallWindowProcA(proc, window, 0x401, 0, 0); // RA2: as above
        range = range_max - minimum;
        value = CallWindowProcA(proc, window, 0x400, 0, 0) - minimum; // RA2: as above
        thumb_pos = value * slider_width / range;
        if (!range) {
            range = 100;
        }

        track->thumb_pos = thumb_pos; // RA2: as above
        track->range = range; // RA2: as above
        track->value = value; // RA2: as above
        track->minimum = minimum; // RA2: as above
        track->step = step; // RA2: as above
        track->show_numbers = show_numbers; // RA2: as above
    }

    if (!step) {
        number_width = 50;
        step = 1;
        show_numbers = 1;
    }

    LONG grip_left;
    LONG grip_right;
    if (!dragging) {
        grip_left = client_rect.left + thumb_pos + 1;
        grip_right = grip_left + 12;
    } else {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(window, &point);

        int xpos = point.x - 6;
        if (xpos < 1) {
            xpos = 1;
        }
        if (client_rect.right - number_width - 12 < xpos) { // RA2: as above
            xpos = client_rect.right - number_width - 12; // RA2: as above
        }

        int idx = ((range + 1) * (xpos - 1)) / slider_width;
        if (idx >= range) {
            idx = range;
        }

        value = step * ((minimum + idx) / step) - minimum;
        thumb_pos = value * slider_width / range;
        grip_left = thumb_pos + 1;
        grip_right = grip_left + 12;
    }

    switch (message) {
    case 0x84: // RA2: as above
    case 0x87: { // RA2: as above
        WNDPROC proc = 0; // RA2: as above
        Window_Proc_Table.getValue(window, proc); // RA2: as above
        return CallWindowProcA(proc, window, message, wparam, lparam); // RA2: as above
    }

    case 0xa: // RA2: as above
        InvalidateRect(window, 0, FALSE); // RA2: as above
        break;

    case 0xf: { // RA2: as above
        RECT paint_rect; // RA2: as above
        Get_Display_Rect(window, &paint_rect); // RA2: as above
        Rect disp_rect(paint_rect.left, paint_rect.top, paint_rect.right - paint_rect.left, paint_rect.bottom - paint_rect.top); // RA2: as above
        LONG style = GetWindowLongA(window, -16); // RA2: as above

        if (data->Back == 0) { // RA2: as above
            RECT disp_copy;
            RECT client_copy;
            Get_Display_Rect(window, &disp_copy);
            GetClientRect(window, &client_copy);

            Surface *surf = new BSurface(client_copy.right + 1, client_copy.bottom + 1, 2); // RA2: as above
            data->Back = surf; // RA2: as above
            Surface_Count++; // RA2: as above

            Rect src_rect(0, 0, client_copy.right + 1, client_copy.bottom + 1);
            Rect dst_rect(disp_copy.left, disp_copy.top, client_copy.right + 1, client_copy.bottom + 1);
            surf->Blit_From(src_rect, *AlternateSurface, dst_rect, false, true); // RA2: as above
            Blend_Rect(&src_rect, surf, 0, data->TextStyle); // RA2: as above
        }

        if (data->Back != 0) { // RA2: as above
            RECT disp_copy;
            RECT client_copy;
            Get_Display_Rect(window, &disp_copy);
            GetClientRect(window, &client_copy);

            Rect src_rect(0, 0, client_copy.right + 1, client_copy.bottom + 1);
            Rect dst_rect(disp_copy.left, disp_copy.top, client_copy.right + 1, client_copy.bottom + 1);
            AlternateSurface->Blit_From(dst_rect, *data->Back, src_rect, false, true); // RA2: as above
        }

        if (show_numbers) {
            {
                Surface *center = SurfaceCache.GetSurface("trofm.pcx", 0); // RA2: as above
                Rect center_rect(disp_rect.X + disp_rect.Width - number_width + 1, disp_rect.Y - 1, number_width, center->Get_Height()); // RA2: as above
                SurfaceCache.Draw(center_rect, *AlternateSurface, *center, 0, 0);
            }
            {
                Surface *left = SurfaceCache.GetSurface("trofl.pcx", 0); // RA2: as above
                Rect left_dst(disp_rect.X + disp_rect.Width - number_width + 1, disp_rect.Y - 1, left->Get_Width(), left->Get_Height()); // RA2: as above
                Rect left_src = left_dst; // RA2: as above
                left_src.Y = 0; // RA2: as above
                left_src.X = 0; // RA2: as above
                AlternateSurface->Blit_From(left_dst, *left, left_src, false, true); // RA2: as above
            }
            {
                Surface *right = SurfaceCache.GetSurface("trofr.pcx", 0); // RA2: as above
                Rect right_dst(disp_rect.X + disp_rect.Width - right->Get_Width() + 1, disp_rect.Y - 1, right->Get_Width(), right->Get_Height()); // RA2: as above
                Rect right_src = right_dst; // RA2: as above
                right_src.Y = 0; // RA2: as above
                right_src.X = 0; // RA2: as above

                AlternateSurface->Blit_From(right_dst, *right, right_src, false, true); // RA2: as above
            }
        }

        Rect grip_rect(paint_rect.left + grip_left, paint_rect.top, grip_right - grip_left, paint_rect.bottom - paint_rect.top); // RA2: as above
        Surface *grip = SurfaceCache.GetSurface("trakgrip.pcx", 0); // RA2: as above
        Rect grip_src; // RA2: as above
        grip_src.X = 0; // RA2: as above
        grip_src.Y = 0; // RA2: as above
        grip_src.Width = grip->Get_Width(); // RA2: as above
        grip_src.Height = grip->Get_Height(); // RA2: as above
        AlternateSurface->Blit_From(grip_rect, *grip, grip_src, false, true); // RA2: as above

        int disabled = style & 0x8000000; // RA2: as above
        if (disabled) { // RA2: as above
            Blend_Rect(&grip_rect, AlternateSurface, 0, Dim_Alpha); // RA2: as above
        }

        int frame_color = Color_To_Pixel(Default_Frame_Color); // RA2: as above
        if (disabled) { // RA2: as above
            frame_color = Color_To_Pixel(Frame_Color); // RA2: as above
        }

        Rect frame_rect = disp_rect; // RA2: as above
        frame_rect.Width = disp_rect.Width - number_width; // RA2: as above
        Draw_Frame(AlternateSurface, frame_rect, 1, frame_color); // RA2: as above

        Rect number_rect = disp_rect; // RA2: as above
        int inset = (show_numbers != 0) + 1; // RA2: as above
        number_rect.X = inset - number_width + disp_rect.Width + disp_rect.X; // RA2: as above
        number_rect.Width = number_width - inset; // RA2: as above
        Draw_Frame(AlternateSurface, number_rect, 1, frame_color); // RA2: as above

        if (show_numbers) {
            wchar_t buffer[32]; // RA2: as above


            swprintf(buffer, L"%d", step * ((minimum + value) / step)); // RA2: as above

            int text_color = Text_Color; // RA2: as above
            if (disabled) { // RA2: as above
                text_color = Disabled_Text_Color; // RA2: as above
            }

            RECT text_rect = paint_rect; // RA2: as above
            text_rect.left = paint_rect.right - 49; // RA2: as above
            Draw_Text_In_Rect_Ex(AlternateSurface, buffer, &text_rect, data->Font, text_color, 5, 0xc, 0, 0, 0); // RA2: as above
        }

        ValidateRect(window, 0);
        break;
    }

    case 0x14: // RA2: as above
        return 0; // RA2: as above

    case 0x200: // RA2: as above
        if (dragging) {
            RECT rect = client_rect;
            InvalidateRect(window, &rect, FALSE);
        }
        if (wparam & 1) { // RA2: as above
            break;
        }

    case 0x202: // RA2: as above
        message_result = 0;
        dragging = 0;
        ReleaseCapture();
        break;

    case 0x201: // RA2: as above
    case 0x203: { // RA2: as above
		if (message == WM_LBUTTONDOWN) { // RA2: as above
            message_result = 1;
            SetCapture(window);
        } else {
            message_result = 0;
            ReleaseCapture();
        }

        int xpos = (unsigned short)lparam; // RA2: as above
        int ypos = (unsigned short)((unsigned int)lparam >> 16); // RA2: as above
        if (ypos > client_rect.bottom - 18) {
            if (xpos >= grip_left && xpos < grip_right) {
				if (message == WM_LBUTTONDOWN) { // RA2: as above
                    dragging = 1;
                }
            } else {
                int x = xpos - 6;
                if (x < 1) {
                    x = 1;
                }
                if (client_rect.right - number_width - 12 < x) { // RA2: as above
                    x = client_rect.right - number_width - 12; // RA2: as above
                }


                int idx = ((range + 1) * (x - 1)) / slider_width;
                if (idx >= range) {
                    idx = range;
                }

                value = step * ((minimum + idx) / step) - minimum;
                thumb_pos = value * slider_width / range;
            }
        }
        break;
    }

    case 0x400: // RA2: as above
        return step * ((minimum + value) / step); // RA2: as above

    case 0x405: // RA2: as above
        if (lparam - minimum <= range && lparam - minimum >= 0) {
            value = lparam - minimum;
        }
        play_click = false;
        thumb_pos = value * slider_width / range;
        break;

    case 0x406: // RA2: as above
        minimum = (unsigned short)lparam; // RA2: as above
        maximum = (unsigned short)((unsigned int)lparam >> 16); // RA2: as above
        range = maximum - minimum;
        if (value > range) {
            value = range;
        }
        if (value < minimum) {
            value = minimum;
        }
        play_click = false;
        thumb_pos = value * slider_width / range;
        break;

    case 0x4ab: // RA2: as above
        step = lparam;
        break;

    case 0x4ac: // RA2: as above
        show_numbers = lparam;
        break;

    case 0x4ae: // RA2: as above
        track->click_suppress = (wparam == 0); // RA2: as above
        break;
    }

    int changed = 0;
    if (value != track->value || range != track->range || minimum != track->minimum) { // RA2: as above
        changed = 1;
    }

    track->result = message_result; // RA2: as above
    track->dragging = dragging; // RA2: as above
    track->thumb_pos = thumb_pos; // RA2: as above
    track->range = range; // RA2: as above
    track->value = value; // RA2: as above
    track->minimum = minimum; // RA2: as above
    track->step = step; // RA2: as above
    track->show_numbers = show_numbers; // RA2: as above

    if (changed) {
        InvalidateRect(window, 0, FALSE); // RA2: as above

        HWND parent = GetParent(window);
        SendMessageA(parent, 0x114, (WPARAM)((unsigned short)5 | (unsigned long)(unsigned short)(value + minimum) << 16), (LPARAM)window); // RA2: as above

        if (play_click == true && !track->click_suppress) { // RA2: as above
            VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, 0); // RA2: as above
        }
    }

    return 0; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

LRESULT __stdcall Group_Box_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    switch (message) {
		case WM_PAINT: { // RA2: as above
        int gap = 0; // RA2: as above
        WinData *record = Get_Record(window); // RA2: as above
        if (!record) { // RA2: as above
            return 0; // RA2: as above
        }
        wchar_t const *text = record->Text; // RA2: as above
        if (!text) { // RA2: as above
            text = L""; // RA2: as above
        }
        RECT area; // RA2: as above
        Get_Display_Rect(window, &area); // RA2: as above
        int width = area.right - area.left; // RA2: as above
        int middle = record->Font->LineHeight / 2 + area.top; // RA2: as above
        int text_width; // RA2: as above
        int text_height; // RA2: as above
        record->Font->Text_Dimensions(text, &text_width, &text_height, width); // RA2: as above
        Rect caption(area.left + 10, area.top, width, area.bottom - area.top); // RA2: as above
        OD_Draw_Text(Text_Color, 0, caption, text, wcslen(text), 0, 0, AlternateSurface, 0); // RA2: as above
        int color = Color_To_Pixel(Default_Frame_Color); // RA2: as above
        AlternateSurface->Draw_Line(Point2D(area.left, middle), Point2D(area.left + 8, middle), color); // RA2: as above
        if (text_width) { // RA2: as above
            gap = 12; // RA2: as above
        }
        AlternateSurface->Draw_Line(Point2D(area.left + text_width + gap, middle), Point2D(area.right, middle), color); // RA2: as above
        AlternateSurface->Draw_Line(Point2D(area.left, middle), Point2D(area.left, area.bottom), color); // RA2: as above
        AlternateSurface->Draw_Line(Point2D(area.left, area.bottom), Point2D(area.right, area.bottom), color); // RA2: as above
        AlternateSurface->Draw_Line(Point2D(area.right, middle), Point2D(area.right, area.bottom), color); // RA2: as above
        ValidateRect(window, 0);
        return 0; // RA2: as above
    }
		case WM_ERASEBKGND: // RA2: as above
        return 1; // RA2: as above
		case WM_NCPAINT: // RA2: as above
        return 0; // RA2: as above
    }
    WNDPROC proc = 0; // RA2: as above
    Window_Proc_Table.getValue(window, proc); // RA2: as above
    return CallWindowProcA(proc, window, message, wparam, lparam); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

LRESULT __stdcall Hotkey_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    switch (message) {
		case WM_PAINT: { // RA2: as above
        RECT area; // RA2: as above
        Get_Display_Rect(window, &area); // RA2: as above
        Rect screen(area.left, area.top, area.right - area.left + 1, area.bottom - area.top + 1); // RA2: as above
        Rect local(0, 0, screen.Width, screen.Height); // RA2: as above
        WinData *record = 0; // RA2: as above
        Control_Table.getPointer((unsigned int)window, &record); // RA2: as above
        if (!record) { // RA2: as above
            return 0; // RA2: as above
        }
        if (!record->Back) { // RA2: as above
            Surface *surface = new BSurface(screen.Width, screen.Height, 2); // RA2: as above
            record->Back = surface; // RA2: as above
            Surface_Count++; // RA2: as above
            surface->Blit_From(local, *AlternateSurface, screen, false, true); // RA2: as above
        }
        wchar_t text[64]; // RA2: as above
			unsigned short hotkey = (unsigned short)SendMessageA(window, HKM_GETHOTKEY, 0, 0); // RA2: as above
        Hotkey_Text(hotkey, text); // RA2: as above
        if (record->Back) { // RA2: as above
            AlternateSurface->Blit_From(screen, *record->Back, local, false, true); // RA2: as above
        }
        Draw_Frame(AlternateSurface, screen, 1, -1); // RA2: as above
        if (wcslen(text) > 0) { // RA2: as above
            area.left += 4; // RA2: as above
            area.top += 4; // RA2: as above
            area.right -= 4; // RA2: as above
            area.bottom -= 4; // RA2: as above
            Draw_Text_In_Rect(AlternateSurface, text, &area, record->Font, Text_Color, 0x24); // RA2: as above
        }
        ValidateRect(window, 0);
        return 0; // RA2: as above
    }
		case WM_ERASEBKGND: // RA2: as above
        return 1; // RA2: as above
		case WM_NCPAINT: // RA2: as above
        return 0; // RA2: as above
    }
    WNDPROC proc = 0; // RA2: as above
    Window_Proc_Table.getValue(window, proc); // RA2: as above
    return CallWindowProcA(proc, window, message, wparam, lparam); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Hotkey_Text(unsigned short hotkey, wchar_t * text) // RA2: as above
{
    char name[32]; // RA2: as above
    char buffer[500]; // RA2: as above
    unsigned char modifiers = (unsigned char)(hotkey >> 8); // RA2: as above
    buffer[0] = 0; // RA2: as above
    *text = 0;
    if (modifiers & 8) { // RA2: as above
        return;
    }
    if (modifiers & 4) { // RA2: as above
        LONG key = MapVirtualKeyA(0x12, 0) << 16 | 0x2000001; // RA2: as above
        GetKeyNameTextA(key, name, sizeof(name)); // RA2: as above
        strcat(buffer, name); // RA2: as above
        strcat(buffer, "+");
    }
    if (modifiers & 2) { // RA2: as above
        LONG key = MapVirtualKeyA(0x11, 0) << 16 | 0x2000001; // RA2: as above
        GetKeyNameTextA(key, name, sizeof(name)); // RA2: as above
        strcat(buffer, name); // RA2: as above
        strcat(buffer, "+");
    }
    if (modifiers & 1) { // RA2: as above
        LONG key = MapVirtualKeyA(0x10, 0) << 16 | 0x2000001; // RA2: as above
        GetKeyNameTextA(key, name, sizeof(name)); // RA2: as above
        strcat(buffer, name); // RA2: as above
        strcat(buffer, "+");
    }
    LONG key = MapVirtualKeyA(hotkey & 0xff, 0) << 16 | 0x2000001; // RA2: as above
    GetKeyNameTextA(key, name, sizeof(name)); // RA2: as above
    strcat(buffer, name); // RA2: as above
    swprintf(text, L"%hs", buffer); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

unsigned short Red_Mask = 0; // RA2: as above
unsigned short Green_Mask = 0; // RA2: as above
unsigned short Blue_Mask = 0; // RA2: as above

void Init_Color_Masks() // RA2: as above
{
	Red_Mask = 0xff; // RA2: as above
	Red_Mask >>= DSurface::Get_Red_Left(); // RA2: as above
	Red_Mask <<= DSurface::Get_Red_Right(); // RA2: as above
	Green_Mask = 0xff; // RA2: as above
	Green_Mask >>= DSurface::Get_Green_Left(); // RA2: as above
	Green_Mask <<= DSurface::Get_Green_Right(); // RA2: as above
    Blue_Mask = 0xff; // RA2: as above
    Blue_Mask >>= DSurface::Get_Blue_Left(); // RA2: as above
    Blue_Mask <<= DSurface::Get_Blue_Right(); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Load_Dialog_Images() // RA2: as above
{
    SurfaceCache.CachePCX(Dialog_Back_Name, 2, 0); // RA2: as above
    SurfaceCache.CachePCX("gdii.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("nodi.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("rani.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("usai.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("japi.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("frai.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("geri.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("gbri.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("djbi.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("arbi.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("lati.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("rusi.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("obsi.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("arrow_uu.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("arrow_ud.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("arrow_du.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("arrow_dd.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("leftbar.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("rightbar.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("trakgrip.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("sbgript.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("sbgripm.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("sbgripb.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("gsbgript.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("gsbgripm.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("gsbgripb.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bar_ll.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bar_lr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bar_ul.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bar_ur.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("dlgsysi.pcx", 1, 0); // RA2: as above
    SurfaceCache.CachePalettedPCX("dlgsysa.pcx");
    SurfaceCache.CachePCX("wouban.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodban.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wouleave.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodleave.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wousqlch.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodsqlch.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woudcon.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woddcon.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woukick.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodkick.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wouhelp.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodhelp.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woufind.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodfind.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wouopt.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodopt.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woutrny.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodtrny.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wouclan.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodclan.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woufgame.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodfgame.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wouact.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodact.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wouref.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wodref.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_tlu.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_tmu.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_tru.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_tld.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_tmd.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_trd.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_ftl.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_ftr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_ftm.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_fbr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_fbl.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_fbm.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_fmr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("tab_fml.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woloper.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolsqlch.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woltrny.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("woluser.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolvoice.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolpriv.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolacpt.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolhost.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolclan.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolob.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolnob.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolrslkob.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolrslknob.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("gt18.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("dnarrowp.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("uparrowp.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("dnarrowr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("uparrowr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("gdnarrowp.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("guparrowp.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("gdnarrowr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("guparrowr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("trofl.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("trofm.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("trofr.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("sb_psh_u.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("sb_psh_d.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("sb_rel_u.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("sb_rel_d.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bst_chkd.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bst_uchk.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bst_chkg.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bst_uckg.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("ccd_i.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("cce_i.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("cud_i.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("cue_i.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("cce_ir.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("cce_il.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bue_li30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bue_mi30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bue_ri30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bde_li30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bde_mi30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bde_ri30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bud_li30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bud_mi30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bud_ri30.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bue_li24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bue_mi24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bue_ri24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bde_li24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bde_mi24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bde_ri24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bud_li24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bud_mi24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("bud_ri24.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("wolreslk.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("pingg.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("pingy.pcx", 2, 0); // RA2: as above
    SurfaceCache.CachePCX("pingr.pcx", 2, 0); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Draw_Edge_Glow(Surface * surface, Point2D const & start, Point2D const & end, int color, // RA2: as above
                                              int alpha) // RA2: as above
{
    Point2D startpoint = start;
    Point2D endpoint = end;
    if (startpoint.X > endpoint.X) {
        swap(startpoint, endpoint);
    }
    int bpp = surface->Bytes_Per_Pixel(); // RA2: as above
    void *buffer = surface->Lock(startpoint); // RA2: as above
    if (buffer != 0) { // RA2: as above
        if (startpoint.Y == endpoint.Y) {
            if (bpp == 1) {
                memset(buffer, color, endpoint.X - startpoint.X + 1);
            } else {
                for (int i = 0; i <= endpoint.X - startpoint.X; i++) {
                    *((unsigned short *)buffer + i) = Blend_Color(*((unsigned short *)buffer + i), color, alpha);
                }
            }
        } else if (startpoint.X == endpoint.X) {
            int pitch = startpoint.Y > endpoint.Y ? -surface->Stride() : surface->Stride(); // RA2: as above
            int dy = abs(endpoint.Y - startpoint.Y);
            for (int i = 0; i <= dy; i++) {
                if (bpp == 1) {
                    *(unsigned char *)buffer = color;
                } else {
                    *(unsigned short *)buffer = Blend_Color(*(unsigned short *)buffer, color, alpha);
                }
                buffer = (unsigned char *)buffer + pitch;
            }
        } else {
            int dx = endpoint.X - startpoint.X;
            int dy = endpoint.Y - startpoint.Y;
            int pitch = surface->Stride(); // RA2: as above
            if (dy < 0) {
                pitch = -pitch;
            }
            dy = abs(dy);
            int dx2 = 2 * dx;
            int dy2 = 2 * dy;
            if (dx > dy) {
                int delta = dy2 - dx;
                for (int i = 0; i < dx; i++) {
                    if (bpp == 1) {
                        *((unsigned char *)buffer + i) = color;
                    } else {
                        *((unsigned short *)buffer + i) = Blend_Color(*((unsigned short *)buffer + i), color, alpha);
                    }
                    if (delta > 0) {
                        buffer = (unsigned char *)buffer + pitch;
                        delta -= dx2;
                    }
                    delta += dy2;
                }
            } else {
                int delta = dx2 - dy;
                int k = 0;
                for (int i = 0; i < dy; i++) {
                    if (bpp == 1) {
                        *((unsigned char *)buffer + k) = color;
                    } else {
                        *((unsigned short *)buffer + k) = Blend_Color(*((unsigned short *)buffer + k), color, alpha);
                    }
                    if (delta > 0) {
                        k++;
                        delta -= dy2;
                    }
                    delta += dx2;
                    buffer = (unsigned char *)buffer + pitch;
                }
            }
        }
        surface->Unlock();
        return true; // RA2: as above
    }
    return false; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

bool __fastcall Draw_Edge_Glow(Surface *surface, Point2D const &start, Point2D const &end, int color, // RA2: as above
                                              int alpha); // RA2: as above

void __fastcall Draw_Edge_Glows(Surface * surface, Rect const & rect, int raised, int count, // RA2: as above
                                               int left_alpha, int top_alpha, int right_alpha, int bottom_alpha) // RA2: as above
{
    int light = 0xffff; // RA2: as above
    int dark = 0; // RA2: as above
    if (!raised) {
        dark = 0xffff; // RA2: as above
        light = 0; // RA2: as above
    }
    for (int i = 0; i < count; i++) {
        Draw_Edge_Glow(surface, Point2D(rect.X + i, rect.Y + i), Point2D(rect.X + rect.Width - 2 - i, rect.Y + i), light, top_alpha); // RA2: as above
        Draw_Edge_Glow(surface, Point2D(rect.X + i, rect.Y + rect.Height - 1 - i), // RA2: as above
                                      Point2D(rect.X + rect.Width - 1 - i, rect.Y + rect.Height - 1 - i), dark, bottom_alpha); // RA2: as above
        Draw_Edge_Glow(surface, Point2D(rect.X + i, rect.Y + i + 1), Point2D(rect.X + i, rect.Y + rect.Height - 1 - i), light, // RA2: as above
                                      left_alpha); // RA2: as above
        Draw_Edge_Glow(surface, Point2D(rect.X + rect.Width - 1 - i, rect.Y + i), // RA2: as above
                                      Point2D(rect.X + rect.Width - 1 - i, rect.Y + rect.Height - 2 - i), dark, right_alpha); // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Draw_Arrow(Surface * surface, Rect const & rect, int upward, int pressed, bool prefixed) // RA2: as above
{
    char name[32]; // RA2: as above
    char state = 'r';
    if (pressed) {
        state = 'p';
    }
    if (upward) {
        sprintf(name, "guparrow%c.pcx", state); // RA2: as above
    } else {
        sprintf(name, "gdnarrow%c.pcx", state); // RA2: as above
    }
    Surface *arrow = SurfaceCache.GetSurface(name + !prefixed, 0); // RA2: as above
    Rect destination = rect; // RA2: as above
    destination.Width = arrow->Get_Width(); // RA2: as above
    destination.Height = arrow->Get_Height(); // RA2: as above
    Rect source; // RA2: as above
    source.Y = 0; // RA2: as above
    source.X = 0; // RA2: as above
    source.Width = arrow->Get_Width(); // RA2: as above
    source.Height = arrow->Get_Height(); // RA2: as above
    surface->Blit_From(destination, *arrow, source, false, true); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

int __fastcall Color_To_Pixel(unsigned long color) // RA2: as above
{
    if (color == 0xffffffff) { // RA2: as above
        return -1; // RA2: as above
    }
    union ColorChannels { // RA2: as above
        struct { unsigned int red : 8; unsigned int green : 8; unsigned int blue : 8; unsigned int alpha : 8; } channels; // RA2: as above
        int value; // RA2: as above
    } packed; // RA2: as above
    packed.value = color; // RA2: as above
    return DSurface::Build_Hicolor_Pixel(color & 0xff, packed.channels.green, (color >> 16) & 0xff); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

unsigned long __fastcall Pixel_To_Color(unsigned short pixel) // RA2: as above
{
    RGBClass color = DSurface::Deconstruct_Hicolor_Pixel(pixel); // RA2: as above
    return 0x02000000 | ((unsigned long)color.Get_Blue() << 16) | ((unsigned short)color.Get_Green() << 8) | color.Get_Red(); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Draw_Frame(Surface *surface, Rect const &rect, int thickness, int color) // RA2: as above
{
    if (color == -1) {
        color = Color_To_Pixel(Default_Frame_Color); // RA2: as above
    }
    Rect frame = rect; // RA2: as above
    frame.X -= thickness; // RA2: as above
    frame.Y -= thickness; // RA2: as above
    frame.Width += thickness * 2; // RA2: as above
    frame.Height += thickness * 2; // RA2: as above
    for (int i = 0; i < thickness; i++) { // RA2: as above
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + i), Point2D(frame.X + frame.Width - 2 - i, frame.Y + i), color); // RA2: as above
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + frame.Height - 1 - i), // RA2: as above
                           Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 1 - i), color); // RA2: as above
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + i + 1), Point2D(frame.X + i, frame.Y + frame.Height - 1 - i), color); // RA2: as above
        surface->Draw_Line(Point2D(frame.X + frame.Width - 1 - i, frame.Y + i), // RA2: as above
                           Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 2 - i), color); // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Draw_Button_Frame(Surface * surface, Rect const & rect, int thickness) // RA2: as above
{
    Rect frame = rect; // RA2: as above
    int top_color = DSurface::Build_Hicolor_Pixel(0x27, 0xf8, 0x74); // RA2: as above
    int bottom_color = DSurface::Build_Hicolor_Pixel(0x13, 0x7b, 0x39); // RA2: as above
    int left_color = DSurface::Build_Hicolor_Pixel(0x1e, 0xba, 0x57); // RA2: as above
    int right_color = DSurface::Build_Hicolor_Pixel(0x18, 0x99, 0x47); // RA2: as above
    frame.X += -1 - thickness; // RA2: as above
    frame.Y += -1 - thickness; // RA2: as above
    frame.Width += 2 * thickness + 2; // RA2: as above
    frame.Height += 2 * thickness + 2; // RA2: as above
    for (int i = 0; i < thickness; i++) { // RA2: as above
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + i), Point2D(frame.X + frame.Width - 2 - i, frame.Y + i), top_color); // RA2: as above
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + frame.Height - 1 - i), // RA2: as above
                           Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 1 - i), bottom_color); // RA2: as above
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + i + 1), Point2D(frame.X + i, frame.Y + frame.Height - 1 - i), left_color); // RA2: as above
        surface->Draw_Line(Point2D(frame.X + frame.Width - 1 - i, frame.Y + i), // RA2: as above
                           Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 2 - i), right_color); // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above
int __fastcall Draw_Text_In_Rect(Surface * surface, wchar_t const * text, RECT * area, // RA2: as above
    BitFont *font, unsigned long color, int flags) // RA2: as above
{
    unsigned int pixel = (((unsigned char)color >> (unsigned char)DSurface::RedLeft) << DSurface::RedRight) // RA2: as above
        | (((unsigned char)(color >> 16) >> (unsigned char)DSurface::BlueLeft) << DSurface::BlueRight) // RA2: as above
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)DSurface::GreenLeft) << DSurface::GreenRight); // RA2: as above
    int x = area->left; // RA2: as above
    int y = area->top; // RA2: as above
    int width = area->right - x; // RA2: as above
    int height = area->bottom - y; // RA2: as above
    if (font == 0) { // RA2: as above
        font = BitFont::Instance; // RA2: as above
    }
    font->Set_Clip(1); // RA2: as above
    font->Set_Bounds((LTRBStruct *)(tagRECT const *)area); // RA2: as above
    font->Set_Color((unsigned short)pixel); // RA2: as above
    BitText::Instance->Draw_Text(font, surface, text, x, y, width, height, 0, 0, 0); // RA2: as above
    return 0; // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above
int __fastcall Draw_Text_In_Rect_Ex(Surface *surface, wchar_t const *text, RECT *area, BitFont *font, // RA2: as above
    unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10) // RA2: as above
{
    unsigned int pixel = (((unsigned char)color >> (unsigned char)DSurface::RedLeft) << DSurface::RedRight) // RA2: as above
        | (((unsigned char)(color >> 16) >> (unsigned char)DSurface::BlueLeft) << DSurface::BlueRight) // RA2: as above
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)DSurface::GreenLeft) << DSurface::GreenRight); // RA2: as above
    int x=area->left; // RA2: as above
    int y=area->top; // RA2: as above
    int width=area->right-x; // RA2: as above
    int height=area->bottom-y; // RA2: as above
    if(flags & 4) { // RA2: as above
        int text_width,text_height; // RA2: as above
        font->Text_Dimensions(text,&text_width,&text_height,width); // RA2: as above
        y+=(height-text_height)/2; // RA2: as above
    }
    font->Set_Clip(1); // RA2: as above
    font->Set_Bounds((LTRBStruct *)(tagRECT const *)area); // RA2: as above
    font->Set_Color((unsigned short)pixel); // RA2: as above
    BitText::Instance->Draw_Text(font,surface,text,x,y,width,height,flags,arg9,arg10); // RA2: as above
    return 0; // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above

float __fastcall Glow_Factor(int value) // RA2: as above
{
    int centers[3] = { 0x2b, 0x80, 0xd5 }; // RA2: as above
    float factor = 1.0f; // RA2: as above
    for (int i = 0; i < 3; i++) {
        int center = centers[i]; // RA2: as above
        if (value > center - 16 && value <= center) { // RA2: as above
            factor = 0.6f * ((center - value) * 0.0625f) + 0.4f; // RA2: as above
        } else if (value > center && value <= center + 16) { // RA2: as above
            factor = 0.6f * ((value - center) * 0.0625f) + 0.4f; // RA2: as above
        }
    }
    return factor; // RA2: as above
}

}


int __fastcall OD_Draw_Text(unsigned int color, BitFont *font, Rect const &area, wchar_t const *text, // RA2: as above
	int length, int x_alignment, int y_alignment, Surface * surface, int shown) // RA2: as above
{
    if (!GameInFocus) return 0; // RA2: as above
    if (!surface) surface = AlternateSurface; // RA2: as above
    unsigned int pixel = (((unsigned char)color >> (unsigned char)DSurface::RedLeft) << DSurface::RedRight) // RA2: as above
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)DSurface::GreenLeft) << DSurface::GreenRight) // RA2: as above
        | (((unsigned char)(color >> 16) >> (unsigned char)DSurface::BlueLeft) << DSurface::BlueRight); // RA2: as above
    int x = area.X; // RA2: as above
    int y = area.Y; // RA2: as above
    int width = area.Width; // RA2: as above
    int height = area.Height; // RA2: as above
    if (!font) font = BitFont::Instance; // RA2: as above
    int text_width, text_height; // RA2: as above
    font->Text_Dimensions(text, &text_width, &text_height, width); // RA2: as above
    if (x_alignment == 1) x += (width - text_width + 1) / 2; // RA2: as above
    else if (x_alignment == 2) x -= (text_width + 1) / 2; // RA2: as above
    else if (x_alignment == 3) x += -1 - text_width; // RA2: as above
    else if (x_alignment == 4) x += width - text_width - 1; // RA2: as above
    if (y_alignment == 1) y += (height - text_height + 1) / 2; // RA2: as above
    else if (y_alignment == 2) y -= (text_height + 1) / 2; // RA2: as above
    else if (y_alignment == 3) y += -1 - text_height; // RA2: as above
    tagRECT clip; // RA2: as above
    clip.left = area.X; // RA2: as above
    clip.top = area.Y; // RA2: as above
    clip.right = clip.left + width; // RA2: as above
    clip.bottom = clip.top + height; // RA2: as above
    font->Set_Clip(1); // RA2: as above
    font->Set_Bounds((LTRBStruct *)&clip); // RA2: as above
    font->Set_Color((unsigned short)pixel); // RA2: as above
	int end = BitText::Instance->Print(font, surface, text, x, y, length, shown); // RA2: as above
    return end - x; // RA2: as above
}


void __fastcall Dialog_On_WM_DRAWITEM(long item) // RA2: as above
{
    DRAWITEMSTRUCT *draw = (DRAWITEMSTRUCT *)item; // RA2: as above
    if (VisibleSurface && AlternateSurface) { // RA2: as above
        RECT area; // RA2: as above
        RECT client;
        Get_Display_Rect(draw->hwndItem, &area); // RA2: as above
        GetClientRect(draw->hwndItem, &client); // RA2: as above
		if (GetWindowLongA(draw->hwndItem, GWL_STYLE) & WS_BORDER) { // RA2: as above
			int border_x = GetSystemMetrics(SM_CXBORDER); // RA2: as above
			int border_y = GetSystemMetrics(SM_CYBORDER); // RA2: as above
            area.left += border_x; // RA2: as above
            area.right -= border_x; // RA2: as above
            area.top += border_y; // RA2: as above
            area.bottom -= border_y; // RA2: as above
        }
		if (draw->CtlType == ODT_BUTTON) { // RA2: as above
            HWND window = draw->hwndItem; // RA2: as above
            OwnerDraw::WinData *record = 0; // RA2: as above
            Control_Table.getPointer((unsigned int)window, &record); // RA2: as above
            if (record) { // RA2: as above
                record->ItemState = draw->itemState; // RA2: as above
                InvalidateRect(window, &draw->rcItem, FALSE); // RA2: as above
                UpdateWindow(window);
            }
        }
    }
}


namespace OwnerDraw { // RA2: as above

void __fastcall Draw_Dimmed_Back(Rect const &area, HWND window, int alpha) // RA2: as above
{
    WinData *record = 0; // RA2: as above
    WinData *parent_record = 0; // RA2: as above
    Control_Table.getPointer(*(unsigned int *)&window, &record); // RA2: as above
    RECT client;
    GetClientRect(window, &client);
    RECT screen; // RA2: as above
    Get_Display_Rect(window, &screen); // RA2: as above
    Surface *surface = record->Back; // RA2: as above
    if (surface == 0) { // RA2: as above
        surface = new BSurface(client.right + 1, client.bottom + 1, 2); // RA2: as above
        record->Back = surface; // RA2: as above
        Surface_Count++; // RA2: as above
        Rect full(0, 0, client.right + 1, client.bottom + 1); // RA2: as above
        HWND parent = GetParent(window);
        Control_Table.getPointer(*(unsigned int *)&parent, &parent_record); // RA2: as above
        Surface *parent_surface = parent_record->Back; // RA2: as above
        if (parent_surface) { // RA2: as above
            RECT parent_screen; // RA2: as above
            Get_Display_Rect(parent, &parent_screen); // RA2: as above
            Rect from; // RA2: as above
            from.X = screen.left - parent_screen.left; // RA2: as above
            from.Y = screen.top - parent_screen.top; // RA2: as above
            from.Width = screen.right - screen.left + 1; // RA2: as above
            from.Height = screen.bottom - screen.top + 1; // RA2: as above
            surface->Blit_From(full, *parent_surface, from, false, true); // RA2: as above
        } else {
            Surface *source = VisibleSurface; // RA2: as above
            Rect from; // RA2: as above
            from.X = screen.left; // RA2: as above
            from.Y = screen.top; // RA2: as above
            from.Width = screen.right - screen.left + 1; // RA2: as above
            from.Height = screen.bottom - screen.top + 1; // RA2: as above
            if (source) { // RA2: as above
                surface->Blit_From(full, *source, from, false, true); // RA2: as above
            }
        }
        unsigned short *pixels = (unsigned short *)surface->Lock(); // RA2: as above
        if (pixels) { // RA2: as above
            for (int i = 0; i < surface->Get_Width() * surface->Get_Height(); i++) { // RA2: as above
                pixels[i] = Blend_Color(pixels[i], 0, alpha); // RA2: as above
            }
            surface->Unlock();
        }
    }
    Rect from; // RA2: as above
    from.X = area.X; // RA2: as above
    from.Y = area.Y; // RA2: as above
    from.Width = area.Width; // RA2: as above
    from.Height = area.Height; // RA2: as above
    from.X -= screen.left; // RA2: as above
    from.Y -= screen.top; // RA2: as above
    AlternateSurface->Blit_From(area, *surface, from, false, true); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Draw_Progress_Fill(Rect const & area, Surface * surface, int color, int fraction) // RA2: as above
{
    int fade = 1;
    int run = (area.Width * fraction) >> 16; // RA2: as above
    if (run < 0) return;
    if (run == 0) run = 1;
    unsigned short *pixels = (unsigned short *)surface->Lock(); // RA2: as above
    if (!pixels) return; // RA2: as above
    for (int row = 0; row < area.Height; row++) { // RA2: as above
        int index = area.X + (area.Y + row) * (surface->Stride() / 2); // RA2: as above
        int quarter = area.Height / 4; // RA2: as above
        if (row == quarter * 3) { // RA2: as above
            fade = 1;
        }
        if (row == quarter) { // RA2: as above
            fade = 0;
        }
        if (run > 0) {
            unsigned short *pixel = &pixels[index]; // RA2: as above
            for (int i = 0; i < run; i++) {
                if (fade) {
                    int alpha = (255 * (i + 1)) / area.Width; // RA2: as above
                    *pixel = Blend_Color(*pixel, color, alpha);
                } else {
                    *pixel = color;
                }
                pixel++; // RA2: as above
            }
        }
    }
    surface->Unlock();
}

}


namespace OwnerDraw { // RA2: as above
void __fastcall Shade_Edges(Rect const &area, Surface &surface, int left_width, int top_height) // RA2: as above
{
    unsigned short *pixels = (unsigned short *)surface.Lock(); // RA2: as above

    if (pixels != 0) { // RA2: as above
        int stride = surface.Stride() / 2;
        int y;

        for (y = area.Y; y < area.Y + area.Height; ++y) { // RA2: as above
            int index = y * stride + area.X; // RA2: as above

            for (int x = area.X; x < area.X + left_width; ++x) { // RA2: as above
                unsigned short pixel = pixels[index]; // RA2: as above

				pixels[index] = ((pixel >> 1) & ((Red_Mask >> 1) & Red_Mask)) | // RA2: as above
					((pixel >> 1) & ((Green_Mask >> 1) & Green_Mask)) | // RA2: as above
                                ((pixel >> 1) & (Blue_Mask >> 1)); // RA2: as above

                index++;
            }
        }

        for (y = area.Y; y < area.Y + top_height; ++y) { // RA2: as above
            int index = y * stride + area.X + left_width; // RA2: as above

            for (int x = area.X + left_width; x < area.Width + area.X; ++x) { // RA2: as above
                unsigned short pixel = pixels[index]; // RA2: as above

				pixels[index] = ((pixel >> 1) & ((Red_Mask >> 1) & Red_Mask)) | // RA2: as above
					((pixel >> 1) & ((Green_Mask >> 1) & Green_Mask)) | // RA2: as above
                                ((pixel >> 1) & (Blue_Mask >> 1)); // RA2: as above

                index++;
            }
        }

        surface.Unlock();
    }
}
}


void __fastcall Blend_Rect(Rect *area, Surface *surface, int color, int alpha) // RA2: as above
{
    unsigned short *pixels = (unsigned short *)surface->Lock(); // RA2: as above
    if (pixels) { // RA2: as above
        int stride = surface->Stride() / 2; // RA2: as above
        for (int y = area->Y; y < area->Y + area->Height; ++y) { // RA2: as above
            int index = y * stride + area->X; // RA2: as above
            for (int x = area->X; x < area->X + area->Width; ++x) { // RA2: as above
                pixels[index] = OwnerDraw::Blend_Color(pixels[index], color, alpha); // RA2: as above
                ++index;
            }
        }
        surface->Unlock();
    }
}


namespace OwnerDraw { // RA2: as above

void __fastcall Draw_Bits(Rect const * area, Surface * surface) // RA2: as above
{
    unsigned char palette[768]; // RA2: as above
    Surface *image = SurfaceCache.GetSurface("bits_i.pcx", (void *)palette); // RA2: as above
    Surface *alpha = SurfaceCache.GetSurface("bits_a.pcx", 0); // RA2: as above
    int count = 0; // RA2: as above
    SurfaceCacheConvertPalette(palette); // RA2: as above
    int x = 0; // RA2: as above
    Rect rect = *area; // RA2: as above
    if (x < area->Width) { // RA2: as above
        do {
            int frame = rand() % 8; // RA2: as above
            rect.X = area->X + x; // RA2: as above
            rect.Y = area->Y + 1; // RA2: as above
            rect.Width = 10; // RA2: as above
            rect.Height = 12; // RA2: as above
            SurfaceCache.DrawMasked(rect, *surface, *image, *alpha, palette, 0, frame * 10, 0); // RA2: as above
            frame = rand() % 8; // RA2: as above
            rect.X = area->X + x; // RA2: as above
            rect.Width = 10; // RA2: as above
            rect.Height = 12; // RA2: as above
            rect.Y = area->Y + area->Height - 16; // RA2: as above
            SurfaceCache.DrawMasked(rect, *surface, *image, *alpha, palette, 0, frame * 10, 0); // RA2: as above
            count++; // RA2: as above
            if (count % 6 == 0) { // RA2: as above
                x += 10;
            }
            x += 10;
        } while (x < area->Width); // RA2: as above
    }
}

}


bool __fastcall Is_Interface_Art_Loaded(void); // RA2: 0x6F3460 (txtlabel2.cpp)
void __fastcall Draw_Interface_Frame(Surface * surface, Rect const & window, bool arg3); // RA2: 0x6F3650 (txtlabel2.cpp)
void __fastcall Draw_Dialog_Backdrop(Surface * surface, Rect const & window, ConvertClass * drawer, ShapeSet * shape640, ShapeSet * shape); // RA2: 0x6F3930 (txtlabel2.cpp)
void __fastcall Draw_Slide_MP_Button(Surface * surface, Rect const & window); // RA2: 0x6F3A60 (txtlabel2.cpp)
void __fastcall Draw_Slide_Top(Surface * surface, Rect const & window); // RA2: 0x6F3AC0 (txtlabel2.cpp)
void __fastcall Draw_Score_Top(Surface * surface, Rect const & window); // RA2: 0x6F3B20 (txtlabel2.cpp)
void __fastcall Draw_Sidebar_Art(Surface * surface, Rect const & window, bool tabs); // RA2: 0x6F4620 (txtlabel2.cpp)
ConvertClass * __cdecl Get_Dialog_Drawer(void); // RA2: 0x6F2230 (txtlabel2.cpp)

namespace OwnerDraw { // RA2: as above

static inline void __cdecl Lookup(unsigned int const &key, WinData *&record) // RA2: as above
{
    Control_Table.getPointer(key, &record); // RA2: as above
}
}

#ifdef RA2_NON_MATCHING // not exact: 536 bytes differ: Is_Chat_Dialog's lookup calls keyHash (0x602020) where the executable expands it; the root is a little short of IL for that budget (two probe statements flip it), TibSun's Draw_Dialog_Back forms tried; Art_Popup_Back* are txtlabel2's ArtPopupBack* (globals.h declares the other spelling)
void __fastcall Dialog_On_WM_PAINT(HWND window) // RA2: as above
{
    using namespace OwnerDraw;
    ConvertClass *drawer = 0; // RA2: as above
    unsigned int key = (unsigned int)window; // RA2: as above
    WinData *record = 0; // RA2: as above
    Lookup(key, record); // RA2: as above
    WinData *const &found = record; // RA2: as above
    if (found) { // RA2: as above
        RECT client;
        GetClientRect(window, &client);
        RECT screen; // RA2: as above
        Get_Display_Rect(window, &screen); // RA2: as above
        Rect full; // RA2: as above
        full.Set(screen.left, screen.top, screen.right - screen.left, screen.bottom - screen.top); // RA2: as above
        if (full.Width <= client.right) { // RA2: as above
            full.Width = client.right; // RA2: as above
        }
        if (full.Height <= client.bottom) { // RA2: as above
            full.Height = client.bottom; // RA2: as above
        }
        if (record->SkipCheckPaint == 0) { // RA2: as above
            Surface *surface = record->Back; // RA2: as above
            if (!surface) {
                surface = new BSurface(client.right, client.bottom, 2); // RA2: as above
                record->Back = surface; // RA2: as above
                Surface_Count++; // RA2: as above
            }
            if (record->ButtonKind == 1) { // RA2: as above
                if (Session.Is_In_Game()) { // RA2: as above
					Draw_Sidebar_Art(surface, full, false); // RA2: as above
                } else {
					if (Is_Interface_Art_Loaded()) { // RA2: as above
                        Draw_Interface_Frame(surface, full, !record->IsWaiting); // RA2: as above
                        drawer = (ConvertClass *)Control_Drawer(window); // RA2: as above
						ShapeSet * shape640 = (ShapeSet *)Control_Shape_640(window); // RA2: as above
						ShapeSet * shape = (ShapeSet *)Control_Shape(window); // RA2: as above
						Draw_Dialog_Backdrop(surface, full, drawer, shape640, shape); // RA2: as above
                        if (record->IsLobbyArt) { // RA2: as above
							Draw_Slide_Top(surface, full); // RA2: as above
                        }
                        if (record->IsLobbyFrame) { // RA2: as above
							Draw_Slide_MP_Button(surface, full); // RA2: as above
                        }
                        if (Is_Chat_Dialog(window)) { // RA2: as above
							Draw_Score_Top(surface, full); // RA2: as above
                        }
                    }
                }
            } else if (record->ButtonKind == 2) { // RA2: as above
                Rect to = full; // RA2: as above
                to.X = 0; // RA2: as above
                to.Y = 0; // RA2: as above
                surface->Blit_From(to, *AlternateSurface, full, false, true); // RA2: as above
                void *shape; // RA2: as above
                if (Session.Is_In_Game()) { // RA2: as above
                    if (Scen->IsGDI) { // RA2: as above
                        shape = Art_Popup_Back_Allied; // RA2: as above
                    } else {
                        shape = Art_Popup_Back_Soviet; // RA2: as above
                    }
                } else {
                    shape = Art_Popup_Back; // RA2: as above
                }
				drawer = Get_Dialog_Drawer(); // RA2: as above
                Draw_Shape(*surface, *drawer, (ShapeSet const *)shape, 0, Point2D(0, 0), to, ShapeFlags_Type(0x400), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
            } else {
                Surface *back = SurfaceCache.GetSurface(OwnerDraw::Dialog_Back_Name, 0); // RA2: as above
                Rect to = full; // RA2: as above
                Rect from = full; // RA2: as above
                to.X -= screen.left; // RA2: as above
                to.Y -= screen.top; // RA2: as above
                int width = back->Get_Width(); // RA2: as above
                int height = back->Get_Height(); // RA2: as above
                if (VideoModeWidth > width) { // RA2: as above
                    from.X -= (VideoModeWidth - width) / 2; // RA2: as above
                }
                if (VideoModeHeight > height) { // RA2: as above
                    from.Y -= (VideoModeHeight - height) / 2; // RA2: as above
                }
                surface->Blit_From(to, *back, from, false, true); // RA2: as above
            }
            if (surface) { // RA2: as above
                Rect from = full; // RA2: as above
                from.X -= screen.left; // RA2: as above
                from.Y -= screen.top; // RA2: as above
                AlternateSurface->Blit_From(full, *surface, from, false, true); // RA2: as above
            }
        }
    }
}
#endif

namespace OwnerDraw { // RA2: as above

}


void On_WM_NCDESTROY(HWND window) // RA2: 0x5FF380 (ra2-decomp's twin 0x6223C0)
{
    WinData *record = 0; // RA2: as above
	Control_Table.getPointer((unsigned int const &)window, &record); // RA2: as above
    if (record && record->Back) { // RA2: as above
        delete record->Back; // RA2: as above
        record->Back = 0; // RA2: as above
        --Surface_Count; // RA2: as above
    }
}


BOOL __stdcall Redraw_Window(HWND window, RECT *rect) // RA2: as above
{
    InvalidateRect(window, rect, 0); // RA2: as above
    UpdateWindow(window);
    return 1; // RA2: as above
}


namespace OwnerDraw { // RA2: as above

class WindowList { // RA2: as above
public: // RA2: as above
    int Count; // RA2: as above
    int Capacity; // RA2: as above
    HWND *Items; // RA2: as above
    char Grow() // RA2: as above
    {
        if (Count < Capacity) return 0; // RA2: as above
        int capacity = Count * 2; // RA2: as above
        if (capacity < 10) capacity = 10; // RA2: as above
        HWND *items = (HWND *)(new unsigned char[capacity * sizeof(HWND)]); // RA2: as above
        memset(items, 0, capacity * sizeof(HWND)); // RA2: as above
        if (Items != 0) memcpy(items, Items, Count * sizeof(HWND)); // RA2: as above
        delete [] (unsigned char *)Items; // RA2: as above
        Items = items; // RA2: as above
        Capacity = capacity; // RA2: as above
        return 1; // RA2: as above
    }
    char Add(HWND &window, int index) // RA2: as above
    {
        if (index > Count) index = Count; // RA2: as above
        if (index >= Capacity) Grow(); // RA2: as above
        if (Count >= Capacity) Grow(); // RA2: as above
        if (index < Count) memmove(Items + index + 1, Items + index, sizeof(HWND) * (Count - index)); // RA2: as above
        new ((void *)(Items + index)) HWND((HWND &)window); // RA2: as above
        Count++; // RA2: as above
        return 1; // RA2: as above
    }
    int Length() const { return Count; } // RA2: as above
};

BOOL __stdcall Add_Window(HWND window, WindowList * list) // RA2: as above
{
    if (list != 0) { // RA2: as above
        list->Add(window, list->Length()); // RA2: as above
    }
    return TRUE; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

int Release_Mouse_For_Dialog() // RA2: as above
{
    if (MouseCursor && MouseCursor->Is_Captured() == true) { // RA2: as above
        MouseCursor->Release_Mouse();
    }
    return ++Mouse_Release_Depth; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

int Capture_Mouse_After_Dialog() // RA2: as above
{
    if (Mouse_Release_Depth > 0) { // RA2: as above
        --Mouse_Release_Depth; // RA2: as above
    }
    if (Mouse_Release_Depth == 0 && MouseCursor && !MouseCursor->Is_Captured()) { // RA2: as above
        MouseCursor->Capture_Mouse();
    }
    return Mouse_Release_Depth; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

HWND __fastcall Begin_Dialog(int resource, DLGPROC proc, LPARAM param) // RA2: as above
{
    unsigned short id = (unsigned short)resource; // RA2: as above
    DLGTEMPLATE const *dialog_template = (DLGTEMPLATE const *)Fetch_Resource((char const *)id, (char const *)5); // RA2: as above
    if (!dialog_template) { // RA2: as above
        return 0; // RA2: as above
    }
    int index = g_DialogCount; // RA2: as above
    DialogInitParam init; // RA2: as above
    g_Dialogs[index].handle = 0; // RA2: as above
    g_Dialogs[index].id = 0; // RA2: as above
    g_DialogCount++;
    init.Resource = id; // RA2: as above
    init.Param = param; // RA2: as above
    HWND window = CreateDialogIndirectParamA(ProgramInstance, dialog_template, MainWindow, (DLGPROC)proc, (LPARAM)&init); // RA2: as above
    if (!window) { // RA2: as above
        g_DialogCount--;
        return 0; // RA2: as above
    }
    g_Dialogs[index].handle = window; // RA2: as above
    g_Dialogs[index].id = id; // RA2: as above
	Add_Modeless_Dialog(window); // RA2: 0x005B6000, the modeless dialog list (its byte-identical twin 0x522B20 is the audio file window list)
    g_TopWindow = window; // RA2: as above
    g_TopWindowID = id; // RA2: as above
    return window; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall End_Dialog(HWND window) // RA2: as above
{
    Keyboard->Clear();
    OwnerDraw::WS_Save_Dialog_Values(window); // RA2: as above
    DestroyWindow(window);
    for (int index = 0; index < g_DialogCount; index++) {
        if (g_Dialogs[index].handle == window) {
            memmove(&g_Dialogs[index], &g_Dialogs[index + 1], (g_DialogCount - (index + 1)) * sizeof(WSDialogStruct)); // RA2: as above
            g_Dialogs[g_DialogCount - 1].handle = 0; // RA2: as above
            g_Dialogs[g_DialogCount - 1].id = 0; // RA2: as above
            g_DialogCount--;
            if (g_DialogCount > 0) {
                g_TopWindow = g_Dialogs[g_DialogCount - 1].handle; // RA2: as above
                g_TopWindowID = g_Dialogs[g_DialogCount - 1].id;
                SetForegroundWindow(g_TopWindow);
                SetFocus(g_TopWindow);
            } else {
                g_TopWindow = 0; // RA2: as above
                g_TopWindowID = 0;
                SetForegroundWindow(MainWindow);
                SetFocus(MainWindow);
            }
            return;
        }
    }
}

}


namespace OwnerDraw { // RA2: as above

void __fastcall Display_Dialog(HWND window) // RA2: as above
{
    ShowWindow(window, SW_SHOWNORMAL);
    SetForegroundWindow(window);
    Keyboard->Clear();
}

}


void __fastcall Show_Dialog(HWND dialog, int kind) // RA2: 0x005FF7E0, ra2-decomp's UI::RegisterWindow (Yuri's Revenge 0x00622820): the controls are subclassed, then the dialog is set up for its kind
{
	using namespace OwnerDraw;
	Subclass_Dialog(dialog, 0);
    Resize_Dialogs(dialog); // RA2: as above
    Set_Dialog_Kind(dialog, kind); // RA2: as above
    if (!Session.Is_In_Game()) { // RA2: as above
        Set_Control_Art((unsigned int)dialog); // RA2: as above
    }
    Update_Lobby_Art_Dialog(dialog); // RA2: as above
    Update_Lobby_Frame_Dialog(dialog); // RA2: as above
    Update_Chat_Dialog(dialog); // RA2: as above
    Update_Setup_Dialog(dialog); // RA2: as above
    EnumChildWindows(dialog, Assign_Control_Sounds, 0); // RA2: as above
    if (Is_Framed_Dialog(dialog)) { // RA2: as above
        Point2D size; // RA2: as above
        size.X = ScreenWidths[0]; // RA2: as above
        size.Y = ScreenHeights[0]; // RA2: as above
        Resize_Dialog_To_Screen(dialog, &size); // RA2: as above
    } else {
        Centre_Dialog(dialog); // RA2: as above
    }
    EnumChildWindows(dialog, Assign_Button_Kind, 0); // RA2: as above
    EnumChildWindows(dialog, Init_Control_Animation, 0); // RA2: as above
    Center_Window_Within_Window(dialog); // RA2: as above
}


/*
**	RA2: the default handling of an owner-drawn dialog (TibSun's, with RA2's tooltip label, dimmed
**	painting, pending slides and the per-kind dialog setup).
*/
namespace OwnerDraw { // RA2: as above

#ifdef RA2_NON_MATCHING // not exact: 1419 bytes differ by position, ours 377 instructions against 472 (1,414 before with a hand-split start case); ra2-decomp's twin text (UI::StandardWndProc, Yuri's Revenge 0x00622B50) in this tree's names. The executable expands Show_Dialog at both of its sites here (Subclass_Dialog expanded in the first and called in the second, Set_Dialog_Kind called in the first and expanded in the second) and keeps Show_Dialog's own body at 0x005FF7E0; ours calls Show_Dialog twice. The cases follow the executable's block order (the hit test ahead of the dialog start, where the twin has it after): the tip texts then match but for Get_Item_Tip_Text, which the executable calls and ours expands. Scratch: with Show_Dialog declared inline both sites expand and 457 instructions come out, but its own body is then not emitted; the tip texts and the two record lookups also divide the inline budget another way
int OwnerDraw::Default_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005FFB00, ra2-decomp's twin text (UI::StandardWndProc, Yuri's Revenge 0x00622B50) in this tree's names
{
	switch (message) {
		case WM_DESTROY:
			Remove_Modeless_Dialog(window);
			--_dialog_count;
			SetFocus(MainWindow);
			return(0);

		case WM_DRAWITEM:
			::Dialog_On_WM_DRAWITEM(lparam); // RA2: the movie-aware draw (0x5FE380)
			return(1);

		case WM_PAINT: {
			WinData * data = Get_Record(window);
			if (data != NULL) {
				if (data->IsDisabled) { // RA2: a disabled dialog only validates
					ValidateRect(window, NULL);
					return(1);
				}
				Dialog_On_WM_PAINT(window);
				Slide_Pending_Dialog(window); // RA2: a pending slide-in finishes
				ValidateRect(window, NULL);
			}
			return(0);
		}

		case WM_ERASEBKGND:
			return(1);

		case WM_NCHITTEST: {
			HWND tip = GetDlgItem(window, 0x695); // RA2: the dialog's tooltip label shows the text under the mouse
			if (tip != NULL) {
				int x = LOWORD(lparam);
				int y = HIWORD(lparam);
				RECT rect;
				GetWindowRect(window, &rect);
				POINT pt;
				pt.x = x - rect.left;
				pt.y = y - rect.top;
				WideWstring text;
				HWND child = ChildWindowFromPointEx(window, pt, CWP_SKIPINVISIBLE);
				if (child != NULL) {
					text = Get_Item_Tip_Text_At(window, child, x, y);
					if (wcscmp(text.get(), L"") == 0) {
						text = Get_Control_Tip_Text(window, (int)child);
						if (wcscmp(text.get(), L"") == 0) {
							text = Control_Tooltip((unsigned int)window, child);
						}
					}
				}
				SendMessage(tip, OD_SETTEXTW, 0, (LPARAM)text.text());
			}
			return(0);
		}

		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			return((int)GetStockObject(BLACK_BRUSH));

		case WM_INITDIALOG:
			++_dialog_count;
			if (lparam != 0) {
				Show_Dialog(window, *(unsigned short *)lparam);
			} else {
				Show_Dialog(window, 0);
			}
			SetFocus(window);
			return(0);

		case OD_SUBCLASSED:
			SendMessage(window, OD_SETTOP, (WPARAM)window, 1);
			return(0);

		case WM_USER + 236: // RA2: the slide-in finished: the tested controls reset
			Dialog_On_OD_4EC(window);
			return(0);
	}
	return(0);
}
#endif

}


/// <summary>
/// Services the game while a dialog is up.
/// A dialog's own message pump calls this routine every pass. It dispatches the pending
/// Windows messages and then either runs the game logic loop -- so that a multiplayer
/// game keeps up while the dialog is showing -- or just the maintenance callback.
/// </summary>
/// <returns>bool; Has the game ended, so that the dialog should be shut down?</returns>
namespace OwnerDraw { // RA2: as above

bool Dialog_Message_Handler() // RA2: as above
{
    Windows_Message_Handler();
    if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH || Session_Busy || // RA2: as above
		Session.Suspended != 0) { // RA2: as above
        Call_Back();
        return false; // RA2: as above
    }
    if (!Is_In_Main_Loop() && Main_Loop()) { // RA2: as above
        return true; // RA2: as above
    }
    return false; // RA2: as above
}

}


/// <summary>
/// Moves a dialog to the position specified.
/// The position is relative to the client area of the main game window rather than to the
/// desktop, and the dialog keeps its current size.
/// </summary>
/// <param name="x">The horizontal position to move to, or -1 to leave it where it is.</param>
/// <param name="y">The vertical position to move to, or -1 to leave it where it is.</param>
/// <returns>Returns with non-zero if the dialog was moved.</returns>
void __fastcall Move_Window(HWND window, int x, int y) // RA2: as above
{
    RECT screen; // RA2: as above
    RECT rect;
    screen.right = VideoModeWidth; // RA2: as above
    screen.left = 0; // RA2: as above
    screen.top = 0; // RA2: as above
    screen.bottom = VideoModeHeight; // RA2: as above
    ClientToScreen(MainWindow, (POINT *)&screen.left); // RA2: as above
    ClientToScreen(MainWindow, (POINT *)&screen.right); // RA2: as above
    GetWindowRect(window, &rect); // RA2: as above
    rect.right -= rect.left; // RA2: as above
    rect.bottom -= rect.top; // RA2: as above
    rect.left = x == -1 ? rect.left - screen.left : x; // RA2: as above
    rect.top = y == -1 ? rect.top - screen.top : y; // RA2: as above
    MoveWindow(window, rect.left, rect.top, rect.right, rect.bottom, FALSE); // RA2: as above
}


/// <summary>
/// Creates the custom message box dialog.
/// This routine brings the modeless message box up, captures the mouse and makes the box
/// the top window. The second button stays hidden unless a caption is supplied for it.
/// </summary>
/// <param name="btn1txt">The message text to show in the box.</param>
/// <param name="btn2txt">The caption for the cancel button, or NULL to leave it hidden.</param>
/// <param name="cancelled">Flag to be raised if the player cancels the box.</param>
/// <returns>Returns with the handle of the message box, or NULL if it could not be
/// created.</returns>
namespace OwnerDraw { // RA2: as above

HWND __fastcall Custom_Message_Box(unsigned short const *text, unsigned short const *button, bool *cancelled) // RA2: as above
{
    HWND dialog = OwnerDraw::Begin_Dialog(0xf0, Cancel_Dialog_Proc, 0); // RA2: as above
    if (dialog) { // RA2: as above
        SetWindowLongA(dialog, DWL_USER, (LONG)cancelled); // RA2: as above
        HWND label = GetDlgItem(dialog, 0x5b0); // RA2: as above
		SendMessageA(label, OD_SETTEXTW, 0, (LPARAM)text); // RA2: as above
        if (button) { // RA2: as above
            HWND item = GetDlgItem(dialog, 0x5c0); // RA2: as above
			SendMessageA(item, OD_SETTEXTW, 0, (LPARAM)button); // RA2: as above
            EnableWindow(item, TRUE); // RA2: as above
            ShowWindow(item, SW_SHOW); // RA2: as above
        }
    }
    return dialog; // RA2: as above
}

}


/// <summary>
/// Handles the messages for a custom message box.
/// This routine lets the default dialog handler have first refusal of the message and
/// then watches for the cancel button. Cancelling feeds an escape key to the game
/// keyboard and raises the flag the box was opened with.
/// </summary>
/// <returns>Returns with the message result, or zero when the message was consumed here.</returns>
int __stdcall Cancel_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
    if (result) { // RA2: as above
        return result; // RA2: as above
    }
	if (message == WM_COMMAND && wparam == 0x5c0) { // RA2: as above
        bool *cancelled = (bool *)GetWindowLongA(window, DWL_USER); // RA2: as above
        if (cancelled) {
			Keyboard->Put((KeyNumType)KN_ESC); // RA2: as above
            *cancelled = true;
        }
    }
    return 0; // RA2: as above
}


/// <summary>
/// Sets the text displayed by a custom message box.
/// Use this routine to change the prompt of a message box that is already on the screen.
/// The box is repainted before the routine returns.
/// </summary>
namespace OwnerDraw { // RA2: as above

WinData::WinData() // RA2: as above
{
    memset(this, 0, sizeof(WinData)); // RA2: as above
    SkipTests = 0xb; // RA2: as above
    Font = BitFont::Instance; // RA2: as above
    WideText = new WideWstring; // RA2: as above
	Caret = 0; // RA2: as above
    Reserved_5C = 0; // RA2: as above
    TypingSound = -1; // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above
inline void WinData::Copy_List(TextNode const *source) // RA2: as above
{
    while (Nodes) { // RA2: as above
        TextNode *node = Nodes; // RA2: as above
        TextNode *previous = 0; // RA2: as above
        TextNode *current; // RA2: as above
        for (current = Nodes; current; current = current->Next) { // RA2: as above
            if (node == current) { // RA2: as above
                break;
            }
            previous = current; // RA2: as above
        }
        if (current) { // RA2: as above
            if (previous) { // RA2: as above
                previous->Next = current->Next; // RA2: as above
            } else {
                Nodes = current->Next; // RA2: as above
            }
            operator delete(current); // RA2: as above
        }
    }
    for (; source; source = source->Next) { // RA2: as above
        TextNode *node = (TextNode *)operator new(wcslen(source->Data) * 2 + 0x12); // RA2: as above
        if (node) { // RA2: as above
            node->Data = node->Text; // RA2: as above
            node->Next = Nodes; // RA2: as above
            node->Reserved_04 = 0; // RA2: as above
            Nodes = node; // RA2: as above
        }
        node->Reserved_0C = source->Reserved_0C; // RA2: as above
        node->Reserved_04 = source->Reserved_04; // RA2: as above
        wcscpy(node->Data, source->Data); // RA2: as above
    }
}
}


namespace OwnerDraw { // RA2: as above

void __fastcall Set_Custom_Message_Box_Text(HWND dialog, wchar_t const * text) // RA2: as above
{
    HWND item = GetDlgItem(dialog, 0x5B0); // RA2: as above
	SendMessageA(item, OD_SETTEXTW, 0, (LPARAM)text); // RA2: as above
    UpdateWindow(dialog); // RA2: as above
}

}


namespace OwnerDraw { // RA2: as above

void WinData::Set_Text(char const * text) // RA2: as above
{
    if (Text) { // RA2: as above
        operator delete(Text); // RA2: as above
        Text = 0; // RA2: as above
    }
    if (text && *text) { // RA2: as above
        Text = (wchar_t *)operator new((strlen(text) + 1) * sizeof(wchar_t)); // RA2: as above
        swprintf(Text, L"%hs", text); // RA2: as above
    }
}

}


namespace OwnerDraw { // RA2: as above

inline WinData &WinData::operator=(WinData const &other) // RA2: as above
{
    TextNode *list = Nodes; // RA2: as above
    wchar_t *text = Text; // RA2: as above
    WideWstring *kept_3c = WideText; // RA2: as above
    if (this == &other) { // RA2: as above
        return *this; // RA2: as above
    }
    memcpy(this, &other, sizeof(WinData)); // RA2: as above
    Text = text; // RA2: as above
    WideText = kept_3c; // RA2: as above
    Nodes = list; // RA2: as above
    Set_Wide_Text(other.Text); // RA2: as above
    Copy_List(other.Nodes); // RA2: as above
    return *this; // RA2: as above
}

}


#ifndef RA2_OWNERDRAW_CHECK_FIELDS // RA2: as above
#endif // RA2: as above

namespace OwnerDraw { // RA2: as above

WinData::WinData(WinData const & other) // RA2: 0x6006E0 (ra2-decomp's twin 0x623730), never called
{
    TextNode *list = Nodes; // RA2: as above
    wchar_t *text = Text; // RA2: as above
    WideWstring *kept_3c = WideText; // RA2: as above
    memcpy(this, &other, sizeof(WinData)); // RA2: as above
    Text = text; // RA2: as above
    WideText = kept_3c; // RA2: as above
    Nodes = list; // RA2: as above
    wchar_t *source = other.Text; // RA2: as above
    if (Text) { // RA2: as above
        operator delete(Text); // RA2: as above
        Text = 0; // RA2: as above
    }
    if (source && *source) { // RA2: as above
        Text = (wchar_t *)operator new(wcslen(source) * 2 + 2); // RA2: as above
        wcscpy(Text, source); // RA2: as above
    }
    {
        TextNode const *source_list=other.Nodes; // RA2: as above
    while (Nodes) { // RA2: as above
        TextNode *node = Nodes; // RA2: as above
        TextNode *previous = 0; // RA2: as above
        TextNode *current; // RA2: as above
        for (current = Nodes; current; current = current->Next) { // RA2: as above
            if (node == current) { // RA2: as above
                break;
            }
            previous = current; // RA2: as above
        }
        if (current) { // RA2: as above
            if (previous) { // RA2: as above
                previous->Next = current->Next; // RA2: as above
            } else {
                Nodes = current->Next; // RA2: as above
            }
            operator delete(current); // RA2: as above
        }
    }
    for (; source_list; source_list = source_list->Next) { // RA2: as above
        TextNode *node = (TextNode *)operator new(wcslen(source_list->Data) * 2 + 0x12); // RA2: as above
        if (node) { // RA2: as above
            node->Data = node->Text; // RA2: as above
            node->Next = Nodes; // RA2: as above
            node->Reserved_04 = 0; // RA2: as above
            Nodes = node; // RA2: as above
        }
        node->Reserved_0C = source_list->Reserved_0C; // RA2: as above
        node->Reserved_04 = source_list->Reserved_04; // RA2: as above
        wcscpy(node->Data, source_list->Data); // RA2: as above
    }
    }
}

}


int __fastcall Clear_ComboBox(HWND combo) // RA2: as above
{
	int index = SendMessageA(combo, CB_GETCOUNT, 0, 0); // RA2: as above
    while (index--) { // RA2: as above
		SendMessageA(combo, CB_DELETESTRING, index, 0); // RA2: as above
    }
    return 0; // RA2: as above
}


namespace OwnerDraw { // RA2: as above


template<class T> inline T Max_Of(T a, T b) { return a < b ? b : a; } // RA2: as above

void __fastcall Draw_Edit_Text(Surface *surface, Rect &rect, wchar_t const *text, int caret, // RA2: as above
                                              BitFont *font, unsigned int color, int *scroll, int ime, // RA2: as above
	int password, int frame, int shown) // RA2: as above
{
    wchar_t buffer[0x800]; // RA2: as above
    int caret_x = -1; // RA2: as above
    int composition_length = 0; // RA2: as above
    int composing = 0; // RA2: as above
    if (text == 0) { // RA2: as above
        text = L""; // RA2: as above
    }
    int text_length = wcslen(text); // RA2: as above
    if (caret < 0 || caret > text_length) { // RA2: as above
        caret = 0; // RA2: as above
    }
    if (ime) { // RA2: as above
        IME_Update_Composition(); // RA2: as above
        composition_length = IMECompositionStringLength; // RA2: as above
        composing = IMEComposing; // RA2: as above
    }
    buffer[0] = 0; // RA2: as above
    wcsncpy(buffer, text, 0x800); // RA2: as above
    if (composition_length) { // RA2: as above
        wcsncpy(&buffer[caret], IMECompositionString, 0x800 - caret); // RA2: as above
        wcsncpy(&buffer[composition_length + caret], &text[caret], 0x800 - composition_length - caret); // RA2: as above
    }
    buffer[0x7FF] = 0; // RA2: as above
    int length = wcslen(buffer); // RA2: as above
    if (password && length > 0) { // RA2: as above
        wchar_t *p = buffer; // RA2: as above
        for (int i = length; i != 0; i--) { // RA2: as above
            *p++ = '*';
        }
    }
    int cursor; // RA2: as above
    if (composing || composition_length) { // RA2: as above
        cursor = IMECompositionCursorPos + caret; // RA2: as above
    } else {
        cursor = caret; // RA2: as above
    }
    if (cursor < *scroll + 5) { // RA2: as above
        if (cursor >= 5) { // RA2: as above
            *scroll = cursor - 5;
        } else {
            *scroll = 0;
        }
    }
    while (*scroll < length) { // RA2: as above
        if (font->Chars_In_Width(&buffer[*scroll], rect.Width - 5, 0, 0) >= cursor - *scroll) { // RA2: as above
            break;
        }
        (*scroll)++; // RA2: as above
    }
    int before = caret - *scroll; // RA2: as above
    int lead = Max_Of(before, 0); // RA2: as above
    int composed = composition_length; // RA2: as above
    if (before < 0) { // RA2: as above
        before += composition_length; // RA2: as above
        composed = Max_Of(before, 0); // RA2: as above
    }
    int after = length - composition_length - caret; // RA2: as above
    if (before < 0) { // RA2: as above
        after += before; // RA2: as above
    }
    if (after < 0) { // RA2: as above
        after = 0; // RA2: as above
    }
    text = &buffer[*scroll]; // RA2: as above
    if (frame) { // RA2: as above
        Rect area(rect.X - 1, rect.Y - 1, font->String_Pixel_Width(text, 0) + 5, rect.Height + 2); // RA2: as above
        surface->Fill_Rect(area, 0); // RA2: as above
    }
    if (lead) { // RA2: as above
		int width = OD_Draw_Text(color, font, rect, text, lead, 0, 0, surface, shown); // RA2: as above
        rect.X += width; // RA2: as above
        rect.Width -= width; // RA2: as above
        if (rect.Width < 0) { // RA2: as above
            rect.Width = 0; // RA2: as above
        }
        text += lead; // RA2: as above
    }
    if (composed) { // RA2: as above
        cursor -= *scroll + lead; // RA2: as above
        int rest; // RA2: as above
        if (cursor < 0 || cursor > composed) { // RA2: as above
            rest = -1; // RA2: as above
        } else {
            rest = composed - cursor; // RA2: as above
            composed -= rest; // RA2: as above
        }
        if (composed > 0) { // RA2: as above
			int width = OD_Draw_Text(Tab_Text_Color, font, rect, text, composed, 0, 0, surface, shown); // RA2: as above
            rect.X += width; // RA2: as above
            rect.Width -= width; // RA2: as above
            if (rect.Width < 0) { // RA2: as above
                rect.Width = 0; // RA2: as above
            }
            text += composed; // RA2: as above
        }
        if (rest >= 0) { // RA2: as above
            caret_x = rect.X; // RA2: as above
            if (rest) { // RA2: as above
				int width = OD_Draw_Text(Tab_Text_Color, font, rect, text, rest, 0, 0, surface, shown); // RA2: as above
                rect.X += width; // RA2: as above
                rect.Width -= width; // RA2: as above
                if (rect.Width < 0) { // RA2: as above
                    rect.Width = 0; // RA2: as above
                }
                text += rest; // RA2: as above
            }
        }
    } else {
        caret_x = rect.X; // RA2: as above
    }
    if (after) { // RA2: as above
		int width = OD_Draw_Text(color, font, rect, text, after, 0, 0, surface, shown); // RA2: as above
        rect.X += width; // RA2: as above
        rect.Width -= width; // RA2: as above
        if (rect.Width < 0) { // RA2: as above
            rect.Width = 0; // RA2: as above
        }
    }
    if (ime && caret_x > 0 && !Caret_Hidden) { // RA2: as above
        Draw_Edge_Glow(surface, Point2D(caret_x, rect.Y), Point2D(caret_x, rect.Y + rect.Height - 2), // RA2: as above
                                      White_Color, 0xFF); // RA2: as above
        Draw_Edge_Glow(surface, Point2D(caret_x + 1, rect.Y), Point2D(caret_x + 1, rect.Y + rect.Height - 2), // RA2: as above
                                      White_Color, 0xFF); // RA2: as above
    }
    Caret_Hidden = 0; // RA2: as above
}

}


bool __fastcall Prompt_Dialog(unsigned short const *text) // RA2: as above
{
    int result = -1; // RA2: as above
    HWND dialog = OwnerDraw::Begin_Dialog(0x10d, Choice_Dialog_Proc, 0); // RA2: as above
    if (dialog) { // RA2: as above
        HWND item = GetDlgItem(dialog, 0x5b0); // RA2: as above
		SendMessageA(item, OD_SETTEXTW, 0, (LPARAM)text); // RA2: as above
        SetWindowLongA(dialog, DWL_USER, (LONG)&result); // RA2: as above
        OwnerDraw::Display_Dialog(dialog); // RA2: as above
        while (result < 0) { // RA2: as above
            Windows_Message_Handler();
            if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && !Session_Busy && // RA2: as above
				Session.Suspended == 0 && !Is_In_Main_Loop() && Main_Loop()) { // RA2: as above
                break;
            }
        }
        OwnerDraw::End_Dialog(dialog); // RA2: as above
    }
    return result == 0x5c1; // RA2: as above
}


int __stdcall Choice_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: as above
{
    int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
    if (result) { // RA2: as above
        return result; // RA2: as above
    }
    int *choice = (int *)GetWindowLongA(window, DWL_USER); // RA2: as above
	if (message == WM_COMMAND) { // RA2: as above
        int id = wparam & 0xffff; // RA2: as above
        if (id >= 0x5c0 && id <= 0x5c1) { // RA2: as above
            *choice = id;
        }
    }
    return 0; // RA2: as above
}


#ifndef RA2_OWNERDRAW_CHECK_FIELDS // RA2: as above
#endif // RA2: as above

int __fastcall Clear_List_Box(HWND listbox) // RA2: as above
{
	int index = SendMessageA(listbox, LB_GETCOUNT, 0, 0); // RA2: as above
    while (index--) { // RA2: as above
		SendMessageA(listbox, LB_DELETESTRING, index, 0); // RA2: as above
    }
    return 0; // RA2: as above
}


LRESULT __fastcall Get_Control_Text(HWND window, Wstring &text) // RA2: as above
{
	LRESULT length = SendMessageA(window, WM_GETTEXTLENGTH, 0, 0); // RA2: as above
    if (length <= 0) { // RA2: as above
        text.clear(); // RA2: as above
        return 0; // RA2: as above
    }
    char *buffer = new char[length + 2]; // RA2: as above
    LRESULT result = SendMessageA(window, 0x4B5, length + 1, (LPARAM)buffer); // RA2: as above
    text = buffer; // RA2: as above
    delete [] buffer; // RA2: as above
    return result; // RA2: as above
}


LRESULT __fastcall Get_Control_Wide_Text(HWND window, WideWstring &text) // RA2: as above
{
	LRESULT length = SendMessageA(window, WM_GETTEXTLENGTH, 0, 0); // RA2: as above
    if (length <= 0) { // RA2: as above
        text.clear(); // RA2: as above
        return 0; // RA2: as above
    }
    unsigned short *buffer = new unsigned short[length + 2]; // RA2: as above
    LRESULT result = SendMessageA(window, 0x4B3, length + 1, (LPARAM)buffer); // RA2: as above
    text = buffer; // RA2: as above
    delete [] buffer; // RA2: as above
    return result; // RA2: as above
}


LRESULT __fastcall Get_List_Item_Text(HWND window, WPARAM index, Wstring &text) // RA2: as above
{
	LRESULT length = SendMessageA(window, LB_GETTEXTLEN, index, 0); // RA2: as above
    if (length <= 0) { // RA2: as above
        text.clear(); // RA2: as above
    }
    char *buffer = new char[length + 2]; // RA2: as above
    LRESULT result = SendMessageA(window, 0x4B7, index, (LPARAM)buffer); // RA2: as above
    text = buffer; // RA2: as above
    delete [] buffer; // RA2: as above
    return result; // RA2: as above
}


LRESULT __fastcall Get_List_Item_Wide_Text(HWND window, WPARAM index, WideWstring &text) // RA2: as above
{
	LRESULT length = SendMessageA(window, LB_GETTEXTLEN, index, 0); // RA2: as above
    if (length <= 0) { // RA2: as above
        text.clear(); // RA2: as above
    }
    unsigned short *buffer = new unsigned short[length + 2]; // RA2: as above
    LRESULT result = SendMessageA(window, 0x4B6, index, (LPARAM)buffer); // RA2: as above
    text = buffer; // RA2: as above
    delete [] buffer; // RA2: as above
    return result; // RA2: as above
}


LRESULT __fastcall Get_Combo_Item_Text(HWND window, WPARAM index, Wstring &text) // RA2: as above
{
	LRESULT length = SendMessageA(window, CB_GETLBTEXTLEN, index, 0); // RA2: as above
    if (length <= 0) { // RA2: as above
        text.clear(); // RA2: as above
    }
    char *buffer = new char[length + 2]; // RA2: as above
    LRESULT result = SendMessageA(window, 0x4BD, index, (LPARAM)buffer); // RA2: as above
    text = buffer; // RA2: as above
    delete [] buffer; // RA2: as above
    return result; // RA2: as above
}


LRESULT __fastcall Get_Combo_Item_Wide_Text(HWND window, WPARAM index, WideWstring &text) // RA2: as above
{
	LRESULT length = SendMessageA(window, CB_GETLBTEXTLEN, index, 0); // RA2: as above
    if (length <= 0) { // RA2: as above
        text.clear(); // RA2: as above
    }
    unsigned short *buffer = new unsigned short[length + 2]; // RA2: as above
    LRESULT result = SendMessageA(window, 0x4C3, index, (LPARAM)buffer); // RA2: as above
    text = buffer; // RA2: as above
    delete [] buffer; // RA2: as above
    return result; // RA2: as above
}


OwnerDraw::DialogControl::DialogControl() {} // RA2: as above
OwnerDraw::DialogControl::~DialogControl() {} // RA2: 0x00601040


namespace OwnerDraw {

/*
**	RA2: 0x00601050, a control the table knows stops its wait; the same answer as Set_Window, without a control.
*/
void __stdcall End_Control_Wait(unsigned int key, HWND window)
{
	WinData * record = Get_Record((HWND)key);
	if (record) {
		record->IsWaiting = true;
	}
}

SimpleDialogControl::SimpleDialogControl(void) : window(0), IsDone(false) {} // RA2: 0x006010C0

SimpleDialogControl::~SimpleDialogControl(void) {} // RA2: 0x006010E0

/*
**	RA2: 0x006010F0, the dialog's answer arrives: the window is kept, a control the table knows stops its wait.
*/
void SimpleDialogControl::Set_Window(unsigned int key, HWND window)
{
	this->window = window;
	WinData * record = Get_Record((HWND)key);
	if (record) {
		record->IsWaiting = true;
	}
	IsDone = true;
}

bool SimpleDialogControl::Is_Done(void) // RA2: 0x00601170
{
	return(IsDone);
}

HWND SimpleDialogControl::Get_Window(void) // RA2: 0x00601180
{
	return(window);
}

bool SimpleDialogControl::Is_Cancelled(void) // RA2: 0x00601190
{
	return(false);
}

}


namespace OwnerDraw { // RA2: as above
int __fastcall Controlled_Dialog(int resource,DLGPROC procedure,DialogControl *control) // RA2: as above
{
    HWND window=Begin_Dialog(resource,procedure,(int)control); // RA2: as above
    if(!window) { // RA2: as above
        control->Set_Window(0,0); // RA2: as above
        return 0; // RA2: as above
    }
	ShowWindow(window, SW_SHOWNORMAL); // RA2: as above
    SetForegroundWindow(window);
    Keyboard->Clear();
    while(!control->Is_Done()&&!control->Is_Cancelled()) { // RA2: as above
        if(Dialog_Message_Handler())break; // RA2: as above
        Title_Screen_Restore(false); // RA2: as above
    }
    End_Dialog(window); // RA2: as above
    return (int)control->Get_Window(); // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above
bool __fastcall Control_Test(HWND window) // RA2: as above
{
    char name[128]; // RA2: as above
    GetClassNameA(window,name,sizeof(name)); // RA2: as above
    if(!strcmp(name,"Edit"))return true; // RA2: as above
    if(!strcmp(name,"NewEdit"))return true; // RA2: as above
    if(!strcmp(name,"ListBox")) { // RA2: as above
        WinData *record=0; // RA2: as above
        unsigned int const &key=(unsigned int const &)window; // RA2: as above
        if(Control_Table.getPointer(key, &record))return record->SkipTests==1; // RA2: as above
    }
	if (!strcmp(name, "ComboBox")) return (GetWindowLongA(window, GWL_STYLE) & 3) != CBS_DROPDOWNLIST; // RA2: as above
    return false; // RA2: as above
}
}


namespace OwnerDraw { // RA2: as above

void WinData::Set_Wide_Text(wchar_t const *text) // RA2: as above
{
	if (Text != NULL) { // RA2: as above
		delete[] Text; // RA2: as above
		Text = NULL; // RA2: as above
	}
	if (text != NULL && *text != L'\0') { // RA2: as above
		Text = new wchar_t[wcslen(text) + 1]; // RA2: as above
		wcscpy(Text, text); // RA2: as above
	}
}

}


TextNode *NodeList::Add_Node(int length) // RA2: as above
{
    TextNode *node = (TextNode *)operator new(length * 2 + 0x12); // RA2: as above
    if (node != 0) { // RA2: as above
        node->Data = node->Text; // RA2: as above
        node->Next = Head; // RA2: as above
        node->Reserved_04 = 0; // RA2: as above
        Head = node; // RA2: as above
    }
    return node; // RA2: as above
}
