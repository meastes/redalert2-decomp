/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 * Control_Proc uses rectangle translations from gibbo101/ra2-decomp.
 * List_Proc follows the list-box message order in gibbo101/ra2-decomp.
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
struct TextNode {
		TextNode * Next;
		int Reserved_04;
	unsigned short * Data;		// RA2: its own Text
		int Reserved_0C;
	unsigned short Text[1];	// RA2: allocated with room for the text
};

struct NodeList {
		char Reserved_00[0x34];
		TextNode * Head;
	TextNode * Add_Node(int length); // RA2: 0x6003D0
	void Remove_Node(TextNode * node); // RA2: 0x600410
};
class ShapeSet;

/*
**	RA2: the WM_INITDIALOG lparam of a dialog the module creates (0x5FF610): the template id and the caller's value.
*/
struct DialogInitParam {
		unsigned short Resource;
		int Param;
};

namespace OwnerDraw {

/*
**	RA2: the key of the control-message dictionary (a message for a window).
*/
struct MessageKey {
		int Message;
		int Window;
		bool operator == (MessageKey const & other) const;
};

/*
**	RA2: the text request owner-drawn controls answer for message 0x4E9 (an item of a list or combo, by index).
*/
struct TextRequest {
		int Item;
		int Index;
		WideWstring Text;
};

}

// RA2: the module's functions, declared before their first use
unsigned int __fastcall Hash_Key(unsigned int & key);
int __fastcall Hash_CtrlMsg(int * pair);
bool __fastcall Focus_Next_Tab_Item(HWND window, unsigned int previous);
int __fastcall OwnerDraw_Control_Height(unsigned int key, HWND window);
unsigned char __fastcall Kind_Test(int kind);
int __fastcall OwnerDraw_Control_Kind(unsigned int key, HWND window);
int __fastcall OwnerDraw_Control_Flags(unsigned int key, HWND window);
bool __fastcall Is_Tested_Control(unsigned int key, HWND window);
bool __fastcall Is_Listed_Kind(int kind);
void __fastcall Send_Control_Value(HWND window, unsigned char value);
void __fastcall Send_Control_Flag(HWND window, unsigned char flag);
void __fastcall Send_Control_Pair(HWND window, int low, int high);
void __fastcall Slide_Pending_Dialog(HWND window);
bool __fastcall Is_Owner_Tested_Control(unsigned int key, HWND window);
unsigned int __fastcall Count_Free_Buttons(HWND window);
void __fastcall Assign_All_Control_Sounds(HWND window);
int __fastcall Dialog_Box(int resource, int(__stdcall * proc)(struct HWND__ *, unsigned int, unsigned int, long), bool Show);
int __fastcall OD_Draw_Text(unsigned int color, BitFont * font, Rect const & area, wchar_t const * text, int length, int x_alignment, int y_alignment, Surface * surface, int shown);
void __fastcall Dialog_On_OD_4EC(HWND dialog); // RA2: 0x5E8710
int __fastcall Dialog_ID_Of_Window(HWND window); // RA2: 0x5EAB00
void __fastcall Dialog_On_WM_PAINT(HWND dialog); // RA2: 0x5FEE70
void __fastcall Dialog_On_WM_DRAWITEM(long item);
BOOL __stdcall Redraw_Window(HWND window, RECT * rect);
void __fastcall Show_Dialog(HWND dialog, int kind);
void __fastcall Move_Window(HWND window, int x, int y);
int __stdcall Cancel_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int __fastcall Clear_ComboBox(HWND combo);
bool __fastcall Prompt_Dialog(unsigned short const * text);
int __stdcall Choice_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int __fastcall Clear_List_Box(HWND listbox);
LRESULT __fastcall Get_Control_Text(HWND window, Wstring & text);
LRESULT __fastcall Get_Control_Wide_Text(HWND window, WideWstring & text);
LRESULT __fastcall Get_List_Item_Text(HWND window, WPARAM index, Wstring & text);
LRESULT __fastcall Get_List_Item_Wide_Text(HWND window, WPARAM index, WideWstring & text);
LRESULT __fastcall Get_Combo_Item_Text(HWND window, WPARAM index, Wstring & text);
LRESULT __fastcall Get_Combo_Item_Wide_Text(HWND window, WPARAM index, WideWstring & text);
namespace OwnerDraw {
void Init_Default_Colors();
int __fastcall Control_Typing_Sound(unsigned int key, HWND window);
bool __fastcall Is_Image_Control(unsigned int key, HWND window);
bool __fastcall Is_Image_Mode3_Control(unsigned int key, HWND window);
bool __fastcall Is_Animated_Control(unsigned int key, HWND window);
int __fastcall Control_Frame_Delay(unsigned int key, HWND window);
int __fastcall Control_Sound_Slot(HWND dialog, HWND window);
int __fastcall Control_Animation_Loops(unsigned int key, HWND window);
int __fastcall Control_Animation_Rate(unsigned int key, HWND window);
void * __fastcall Control_Animation_Art(unsigned int key, HWND window);
int __fastcall Control_Image(unsigned int key, HWND window);
void const * __fastcall Control_Animation_Shape(unsigned int key, HWND window, bool & allocated);
int __fastcall Control_Text_Style(HWND dialog, HWND window);
void __fastcall Set_Control_Image(HWND control, Surface * image);
bool __fastcall Is_Control_Checked(HWND window);
void __fastcall Set_Control_Result(HWND control, int value);
int __fastcall Get_Control_Result(HWND control);
WideWstring __fastcall Get_Item_Tip_Text(HWND window, int a, int b);
WideWstring __fastcall Get_Control_Tip_Text(HWND window, int a);
WideWstring __fastcall Get_Item_Tip_Text_At(HWND window, HWND item, int x, int y);
WideWstring __fastcall Control_Tooltip(unsigned int key, HWND window);
char const * __fastcall Control_Tooltip_Label(unsigned int key, HWND window);
void __fastcall Set_Control_Disabled(HWND control, bool value);
bool __fastcall Is_Control_Disabled(HWND control);
BOOL __stdcall Enable_Animated_Control(HWND window, LPARAM lparam);
void __fastcall Enable_Animated_Controls(HWND window);
void __fastcall Disable_Animated_Controls(HWND window);
void __fastcall Slide_Dialog(HWND dialog, bool reverse);
bool __fastcall WS_Save_Dialog_Values(HWND dialog);
bool __fastcall Play_Dialog_Move_In(HWND dialog);
void __fastcall Set_Dialog_Hidden(HWND control);
void __fastcall Clear_Dialog_Hidden(HWND control);
void __fastcall Set_Dialog_Waiting(HWND control);
void __fastcall Clear_Dialog_Waiting(HWND control);
bool __fastcall Select_Control_Rect(HWND dialog, HWND control, Rect * bounds);
bool __fastcall Is_Free_Owner_Button(unsigned int key, HWND window);
bool __fastcall Is_Default_Button(unsigned int key, HWND window);
bool __fastcall Is_Owner_Default_Button(unsigned int key, HWND window);
bool __fastcall Is_Flag_Control(unsigned int key, HWND window);
bool __fastcall Is_Campaign_Button(unsigned int key, HWND window);
bool __fastcall Is_Network_Button(unsigned int key, HWND window);
bool __fastcall Is_Cancel_Button(unsigned int key, HWND window);
bool __fastcall Is_Main_Menu_Movie(unsigned int key, HWND window);
BOOL __stdcall Count_Visible_Free_Buttons(HWND window, LPARAM);
BOOL __stdcall Count_Visible_Default_Buttons(HWND window, LPARAM);
bool __fastcall Has_Visible_Default_Button(HWND dialog);
BOOL __stdcall Assign_Button_Kind(HWND window, LPARAM);
void __fastcall Assign_Button_Kinds(HWND window);
BOOL __stdcall Init_Control_Animation(HWND window, LPARAM);
bool __fastcall Is_Listed_Dialog_Kind(int kind);
void __fastcall Init_Control_Animations(HWND window);
BOOL __stdcall Reset_Tested_Control(HWND window, LPARAM);
BOOL __stdcall Assign_Control_Sounds(HWND window, LPARAM);
void __fastcall Move_Control_To_Rect(HWND window, Rect const & rect);
void __fastcall Move_Free_Button(HWND window, Point2D const & size);
void __fastcall Move_Tested_Control(HWND window, Point2D const & size);
void __fastcall Move_Default_Button_Owner(HWND window);
void __fastcall Move_Default_Button(HWND window);
void __fastcall Move_Flag_Control(HWND window);
void __fastcall Move_Lobby_Control(HWND window);
void __fastcall Move_Centred_Control(HWND window, Point2D const & size);
void __fastcall Adjust_Control_Position(HWND window);
BOOL __stdcall Move_Child_For_Resolution(HWND window, LPARAM lparam);
int __fastcall Control_Drawer(HWND control);
bool __fastcall Is_Framed_Dialog(HWND dialog);
bool __fastcall Centre_Dialog(HWND control);
bool __fastcall Is_Lobby_Frame_Dialog(HWND control);
bool __fastcall Update_Lobby_Frame_Dialog(HWND control);
void __fastcall Set_Lobby_Frame_Dialog(HWND control);
void __fastcall Clear_Lobby_Frame_Dialog(HWND control);
bool __fastcall Is_Lobby_Art_Dialog(HWND control);
bool __fastcall Update_Lobby_Art_Dialog(HWND control);
void __fastcall Set_Lobby_Art_Dialog(HWND control);
void __fastcall Clear_Lobby_Art_Dialog(HWND control);
bool __fastcall Is_Chat_Dialog(HWND control);
bool __fastcall Update_Chat_Dialog(HWND control);
bool __fastcall Is_Setup_Dialog(HWND control);
bool __fastcall Update_Setup_Dialog(HWND control);
int __fastcall Control_Shape_640(HWND control);
int __fastcall Control_Shape(HWND control);
void __fastcall Set_Control_Art(unsigned int key);
void __fastcall Set_Dialog_Kind(HWND control, int value);
void __fastcall Register_Window_Classes();
LRESULT __stdcall Combo_Drop_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
BOOL __stdcall Replace_Edit(HWND window, LPARAM);
bool __fastcall Subclass_Dialog(HWND window, LPARAM lparam);
BOOL __stdcall Set_Paint_Disabled(HWND window, LPARAM lparam);
BOOL __stdcall Subclass_Control(HWND window, LPARAM lparam);
BOOL __fastcall Restore_Window(HWND window);
BOOL __stdcall Restore_Child(HWND window, LPARAM lparam);
bool __fastcall Start_Tooltip(Rect const & area, unsigned short const * text, HWND window);
bool __fastcall Show_Tooltip(bool save_background);
bool Hide_Tooltip();
bool End_Tooltip();
LRESULT __stdcall Control_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Default_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Owner_Button_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Tab_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Edit_Control_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int __fastcall Edit_Text(HWND window, wchar_t * text, int size, int * caret);
LRESULT __stdcall Static_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Checkbox_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Button_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Scroll_Bar_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Progress_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Trackbar_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Group_Box_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Hotkey_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Hotkey_Text(unsigned short hotkey, wchar_t * text);
void Init_Color_Masks();
void __fastcall Load_Dialog_Images();
bool __fastcall Draw_Edge_Glow(Surface * surface, Point2D const & start, Point2D const & end, int color, int alpha);
void __fastcall Draw_Edge_Glows(Surface * surface, Rect const & rect, int raised, int count, int left_alpha, int top_alpha, int right_alpha, int bottom_alpha);
void __fastcall Draw_Arrow(Surface * surface, Rect const & rect, int upward, int pressed, bool prefixed);
int __fastcall Color_To_Pixel(unsigned long color);
unsigned long __fastcall Pixel_To_Color(unsigned short pixel);
void __fastcall Draw_Frame(Surface * surface, Rect const & rect, int thickness, int color);
void __fastcall Draw_Button_Frame(Surface * surface, Rect const & rect, int thickness);
int __fastcall Draw_Text_In_Rect(Surface * surface, wchar_t const * text, RECT * area, BitFont * font, unsigned long color, int flags);
int __fastcall Draw_Text_In_Rect_Ex(Surface * surface, wchar_t const * text, RECT * area, BitFont * font, unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10);
float __fastcall Glow_Factor(int value);
void __fastcall Draw_Dimmed_Back(Rect const & area, HWND window, int alpha);
void __fastcall Draw_Progress_Fill(Rect const & area, Surface * surface, int color, int fraction);
void __fastcall Shade_Edges(Rect const & area, Surface & surface, int left_width, int top_height);
void __fastcall Draw_Bits(Rect const * area, Surface * surface);
BOOL __stdcall Add_Window(HWND window, WindowList * list);
int Release_Mouse_For_Dialog();
int Capture_Mouse_After_Dialog();
void __fastcall End_Dialog(HWND window);
void __fastcall Display_Dialog(HWND window);
bool Dialog_Message_Handler();
HWND __fastcall Custom_Message_Box(unsigned short const * text, unsigned short const * button, bool * cancelled);
void __fastcall Draw_Edit_Text(Surface * surface, Rect & rect, wchar_t const * text, int caret, BitFont * font, unsigned int color, int * scroll, int ime, int password, int frame, int shown);
int __fastcall Controlled_Dialog(int resource, DLGPROC procedure, DialogControl * control);
bool __fastcall Control_Test(HWND window);
}

namespace OwnerDraw { extern unsigned short Blue_Mask; }
namespace OwnerDraw { extern unsigned short Red_Mask; }
namespace OwnerDraw { extern unsigned short Green_Mask; }
bool Is_In_Main_Loop();
bool __fastcall Main_Loop();
void __stdcall Set_Paint_Disabled_Children(struct HWND__ * window, int unknown2);



namespace OwnerDraw {

/*
**	RA2: the record of a subclassed window, or NULL.
*/
inline WinData * __cdecl Get_Record(HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int const &)window, &record);
	return(record);
}

/*
**	RA2: the record of a subclassed window with the lookup's result tested (ra2-decomp's Get_Data).
*/
inline WinData * Get_Data(HWND window)
{
	WinData * data = NULL;
	if (!Control_Table.getPointer((unsigned int const &)window, &data)) {
		return NULL;
	}
	return data;
}

/*
**	RA2: how far a sidebar-anchored control sits in from the edge (none in a game session).
*/
inline int __cdecl Sidebar_Inset(int width, unsigned char ingame)
{
	int inset = 0;
	if (!ingame) {
		int excess = (width - Base_Screen_Width) / 2;
		inset = excess < 0 ? 0 : excess;
	}
	return(inset);
}

/*
**	RA2: a control's sidebar margin, its record's value or centred in the sidebar width.
*/
inline int __cdecl Sidebar_Margin(HWND window, int width)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int const &)window, &record);
	if (record && record->SoundSlot) return(record->SoundSlot);
	return((SidebarWidth - width) / 2);
}

}
void __fastcall Show_Dialog_With_Flag(HWND dialog); // RA2: 0x5115F0
namespace OwnerDraw { LRESULT __stdcall New_Edit_Window_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); } // RA2: 0x5EAD00
void IME_Update_Composition(void); // RA2: 0x737E30

namespace OwnerDraw { void Shutdown(void); } // RA2: 0x5EAC20


SurfaceCacheClass SurfaceCache;


namespace OwnerDraw {

void Init_Default_Colors()
{
	Border_Width = 1;
	Glow_Color = 0x7f;
	Text_Color = 0xffff;
	Highlight_Text_Color = 0xeeeeee;
	Tab_Text_Color = 0x9f9f;
	White_Color = 0xffffff;
	Grey_Color = 0x808080;
	Fill_Color = 0xff;
	Pressed_Fill_Color = 0x626262;
	Default_Frame_Color = 0xff;
	Alternate_Frame_Color = 0x929292;
	Disabled_Text_Color = 0x9f;
	Light_Grey_Color = 0x8f8f8f;
	Frame_Color = 0x9f;
	Dark_Grey_Color = 0x646464;
	Dim_Alpha = 0x60;
	Olive_Color = 0xaaaa;
	Dark_Brown_Color = 0x221b0b;
	Brown_Color = 0x443716;
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
	return key;
}


/// <summary>
/// Fetches the hash value for a control message key.
/// This is the hash routine handed to the dictionary CtrlProc keeps of the messages it is
/// already in the middle of handling.
/// </summary>
/// <returns>Returns with the hash value formed from the window and the message.</returns>
int __fastcall Hash_CtrlMsg(int * pair)
{
	return pair[1] * *pair;
}


/// <summary>
/// Determines if two control message keys refer to the same thing.
/// </summary>
/// <returns>bool; Do both keys name the same window and the same message?</returns>
namespace OwnerDraw {

bool MessageKey::operator==(MessageKey const & other) const
{
	if (other.Window == Window && other.Message == Message) {
		return true;
    }
	return false;
}

}


/// <summary>
/// Sets the owner-draw metrics and colors to their defaults.
/// This routine establishes the border thickness, the blend strength and the family of
/// colors that every owner-draw control paints with. Each control asks for it as it is
/// subclassed, so the values are always current.
/// </summary>
unsigned int __fastcall Hash_Window(HWND & key);

Dictionary<HWND, WNDPROC> Window_Proc_Table(Hash_Window);


unsigned int __fastcall Hash_Window(HWND & key);

Dictionary<HWND, WNDPROC> Window_Table(Hash_Window);


inline void NodeList::Remove_Node(TextNode * node)
{
	TextNode * previous = 0;
	TextNode * current = Head;
	while (current != 0) {
		if (node == current) {
            break;
        }
		previous = current;
		current = current->Next;
    }
	if (current != 0) {
		if (previous != 0) {
			TextNode * next = current->Next;
			previous->Next = next;
        } else {
			TextNode * next = current->Next;
			Head = next;
        }
		operator delete(current);
    }
}

namespace OwnerDraw {

inline WinData::~WinData()
{
	Set_Wide_Text(0);
	while (Nodes) {
		((NodeList *)this)->Remove_Node(Nodes);
    }
	delete WideText;
}

}

unsigned int __fastcall Hash_Key(unsigned int const & key);

Dictionary<unsigned int, OwnerDraw::WinData> Control_Table(Hash_Key);


using namespace OwnerDraw; // RA2: the per-function files


bool __fastcall Focus_Next_Tab_Item(HWND window, unsigned int previous)
{
	HWND item;

	item = (GetParent(window));
	item = (GetNextDlgTabItem(item, window, (int)(previous & 0xff)));
	if (item != window) {
		SetFocus(item);
		return 1;
    }
	return 0;
}


int __fastcall OwnerDraw_Control_Height(unsigned int key, HWND window)
{
	OwnerDraw::WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	bool general = ((((((kind == 0x10c) || (kind == 0x94)) || (kind == 0xa3)) ||
						 ((((((kind == 0x73 || (kind == 0xd8)) ||
								 ((kind == 0xbbb || ((kind == 0xf5 || (kind == 0x105)))))) || (kind == 0xe2)) ||
							   ((((kind == 0xd5 || (kind == 0x2b5)) || (kind == 0xb7)) ||
								   ((kind == 0x2b4 || (kind == 0xbc7)))))) ||
							 (((kind == 0x101 || ((kind == 0x129 || (kind == 0xbc6)))) || (kind == 0xd7)))))) ||
						(((((kind == 0x108 || (kind == 0xbc)) || (kind == 0xbd)) ||
							  ((kind == 0xbb || (kind == 0xb5)))) ||
							((kind == 0xbba || ((kind == 0xff || (kind == 0x6b)))))))) ||
					   ((((kind == 0x100 || (((kind == 0x102 || (kind == 0xb8)) || (kind == 0xd6)))) ||
							 (((kind == 0x103 || (kind == 0x125)) ||
								 ((kind == 0xe7 || ((kind == 0x116 || (kind == 0xc2)))))))) ||
						   ((kind == 0xc9 ||
							   (((((kind == 0x11d || (kind == 0x11c)) || (kind == 0x114)) ||
									 (((kind == 700 || (kind == 0x113)) ||
										 ((kind == 0x109 || ((kind == 0x10f || (kind == 0x117)))))))) ||
								   ((kind == 0xe6 || (((kind == 0xf3 || (kind == 0xf4)) || (kind == 0x10e)))))))))))) &&
		(id == 0x694);
	if (general) {
		return (0x1e);
    }
	bool dual_rail = (((kind == 0xbc) || (kind == 0xbd)) ||
						 ((kind == 0x102 || ((kind == 0xc2 || (kind == 0xc9)))))) &&
		((id == 0x6ec || (id == 0x5a8)));
	if (dual_rail) {
		return (0x1e);
    }
	if ((kind == 0x103) || (kind == 0xbc7)) {
		if (id != 0x6f4) {
			if (id == 0x6f3 || id == 0x69f || id == 0x6f6 || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2 || id == 0x73c || id == 0x73d || id == 0x73e || id == 0x73f || id == 0x740 || id == 0x735 || id == 0x732 || id == 0x3ea || id == 0x733) {
				return 0x1e;
            }
			bool tall = ((((((id == 0x411) || (id == 0x412)) || (id == 0x419)) ||
							  ((id == 0x41a || (id == 0x6a3)))) ||
							 ((id == 0x6a4 || ((id == 0x6a5 || (id == 0x6a7)))))) || (id == 0x6a6)) ||
				((((id == 0x6a8 || (id == 0x73a)) || (id == 0x739)) ||
					((id == 0x73b || (id == 0x738)))));
			if (tall) {
				return (0x3c);
            }
        } else {
			return 0xf;
        }
    }
	if ((kind == 0x108) || (kind == 0xbc6)) {
		if (id == 0x6d2 || id == 0x3ea || id == 0x733 || id == 0x71b || id == 0x732 || id == 0x69d || id == 0x69f || id == 0x69e || id == 0x6a0 || id == 0x6a1 || id == 0x5c8 || id == 0x411 || id == 0x412 || id == 0x413 || id == 0x414 || id == 0x415 || id == 0x416 || id == 0x417 || id == 0x418) {
			return 0x1e;
        }
		bool tall2 = ((((id == 0x419) || (id == 0x41a)) || (id == 0x41b)) ||
						 (((((((id == 0x41c || (id == 0x41d)) ||
								  ((id == 0x41e || ((id == 0x41f || (id == 0x420)))))) || (id == 0x6a3)) ||
								(((((id == 0x6a4 || (id == 0x6a9)) || (id == 0x6ac)) ||
									  ((id == 0x6af || (id == 0x6b2)))) || (id == 0x6b5)))) ||
							   (((id == 0x6b8 || (id == 0x6a5)) ||
								   (((id == 0x6a7 || (((id == 0x6aa || (id == 0x6ad)) || (id == 0x6b0)))) ||
									   (((id == 0x6b3 || (id == 0x6b6)) || (id == 0x6b9)))))))) ||
							 ((id == 0x6a6 || (id == 0x6a8)))))) ||
			(((id == 0x6ab || (((id == 0x6ae || (id == 0x6b1)) || (id == 0x6b4)))) ||
				(((id == 0x6b7 || (id == 0x6ba)) ||
					((id == 0x70a ||
						(((id == 0x70b || (id == 0x70c)) ||
							((id == 0x70d ||
								((((id == 0x70e || (id == 0x70f)) || (id == 0x710)) || (id == 0x711)))))))))
                  ))));
		if (tall2) {
			return (0x3c);
        }
    }
	if (Kind_Test(kind) != 0 && id == 0x695) {
		return 0xf;
    }
	return 0xf;
}


/// <summary>
/// Subclasses a dialog and every one of its controls for owner drawing.
/// This is the routine a dialog procedure calls when it is first created. Painting is
/// suppressed across the whole window while the controls are being hooked, so the dialog
/// never flickers through its stock Windows appearance on the way.
/// </summary>
/// <param name="lparam">Caller supplied value to store with each control for its own use.</param>
unsigned char __fastcall Kind_Test(int kind)
{
	if ((((((kind != 0x10c) && (kind != 0x94)) && (kind != 0xb6)) &&
			 ((((kind != 0xa3 && (kind != 0x73)) &&
				   ((kind != 0xd8 && ((kind != 0xbbb && (kind != 0xf5)))))) &&
				 ((kind != 0x105 &&
					 ((((kind != 0xe2 && (kind != 0xd5)) && (kind != 0x2b5)) &&
						 ((kind != 0xb7 && (kind != 0x2b4)))))))))) &&
			((((kind != 0x101 && ((kind != 0x129 && (kind != 0xbc7)))) && (kind != 0xbc6)) &&
				(((((((kind != 0xd7 && (kind != 0x108)) && (kind != 0xbc)) &&
						((kind != 0xbd && (kind != 0xbb)))) &&
					   ((kind != 0xb5 && ((kind != 0xbba && (kind != 0xff)))))) && (kind != 0xea)) &&
					(((kind != 0x6b && (kind != 0x100)) && (kind != 0x102)))))))) &&
		((((((kind != 0xb8 && (kind != 0xd6)) &&
				((kind != 0x103 && ((kind != 0x11f && (kind != 0x125)))))) && (kind != 0x115))
              && ((((((kind != 0x122 && (kind != 0x112)) && (kind != 0xe7)) &&
					 (((kind != 0x116 && (kind != 0xc2)) &&
						 ((kind != 0xc9 && ((kind != 0x11d && (kind != 0x11c)))))))) &&
					(kind != 0x113)) &&
				  (((((kind != 0xfe && (kind != 0x109)) && (kind != 0x10f)) &&
						((kind != 0x117 && (kind != 0x114)))) && (kind != 0xc3)))))) &&
			((((kind != 0x124 && (kind != 0xe6)) &&
				  ((kind != 0xf3 && (((kind != 0xf4 && (kind != 700)) && (kind != 0x11b)))))) &&
				(kind != 0x10e)))))) {
		return 0;
    }
	return 1;
}


/// <summary>
/// Sets the paint suppression flag for one window.
/// This is the enumeration callback Subclass_Dialog uses to silence, and later re-enable,
/// painting across a whole dialog. A window with no owner-draw record yet is given one.
/// </summary>
/// <param name="lparam">Should painting be suppressed for this window?</param>
int __fastcall OwnerDraw_Control_Kind(unsigned int key, HWND window)
{

	OwnerDraw::WinData * record = 0;

	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (((((((((((kind != 0x10c) && (kind != 0x94)) && (kind != 0xa3)) &&
				  (((kind != 0x73 && (kind != 0xd8)) &&
					  ((kind != 0xbbb && ((kind != 0xf5 && (kind != 0x105)))))))) &&
				 ((kind != 0xe2 &&
					 (((((kind != 0xd5 && (kind != 0x2b5)) && (kind != 0xb7)) &&
						   ((kind != 0x2b4 && (kind != 0xbc7)))) && (kind != 0x101)))))) &&
				(((kind != 0x129 && (kind != 0xbc6)) &&
					((kind != 0xd7 && (((kind != 0x108 && (kind != 0xbc)) && (kind != 0xbd)))))))) &&
			   (((((kind != 0xbb && (kind != 0xb5)) && (kind != 0xbba)) &&
					 ((kind != 0xff && (kind != 0x6b)))) &&
				   ((((kind != 0x100 && (((kind != 0x102 && (kind != 0xb8)) && (kind != 0xd6)))) &&
						 ((kind != 0x103 && (kind != 0x125)))) && (kind != 0xe7)))))) &&
			  ((((kind != 0x116 && (kind != 0xc2)) &&
					((kind != 0xc9 &&
						((((kind != 0x11d && (kind != 0x11c)) && (kind != 0x114)) &&
							((kind != 700 && (kind != 0x113)))))))) && (kind != 0x109)))) &&
			 (((kind != 0x10f && (kind != 0x117)) &&
				 ((kind != 0xe6 && (((kind != 0xf3 && (kind != 0xf4)) && (kind != 0x10e)))))))) ||
			(id != 0x694)) &&
		((((kind != 0xbc && (kind != 0xbd)) &&
			  ((kind != 0x102 && ((kind != 0xc2 && (kind != 0xc9)))))) ||
			((id != 0x6ec && (id != 0x5a8)))))) {
		if ((kind == 0x103) || (kind == 0xbc7)) {
			if (id == 0x6f4) {
				return (3);
            }
			if (id == 0x6f3) {
				return (1);
            }
			if (id == 0x69f) {
				return (1);
            }
			if (id == 0x6f6) {
				return (1);
            }
			if (id == 0x6ef) {
				return (1);
            }
			if (id == 0x6f1) {
				return (1);
            }
			if (id == 0x6f0) {
				return (1);
            }
			if (id == 0x6f2) {
				return (1);
            }
			if (id == 0x733) {
				return (1);
            }
			if (id == 0x735) {
				return (1);
            }
			if (id == 0x732) {
				return (1);
            }
			if (id == 0x3ea) {
				return (1);
            }
			if (id == 0x73c) {
				return (1);
            }
			if (id == 0x73d) {
				return (1);
            }
			if (id == 0x73e) {
				return (1);
            }
			if (id == 0x73f) {
				return (1);
            }
			if (id == 0x740) {
				return (1);
            }
			if (id == 0x736) {
				return (1);
            }
			if (id == 0x737) {
				return (1);
            }
			if (id == 0x411) {
				return (1);
            }
			if (id == 0x412) {
				return (1);
            }
			if (id == 0x419) {
				return (1);
            }
			if (id == 0x41a) {
				return (1);
            }
			if (id == 0x6a3) {
				return (1);
            }
			if (id == 0x6a4) {
				return (1);
            }
			if (id == 0x6a5) {
				return (1);
            }
			if (id == 0x6a7) {
				return (1);
            }
			if (id == 0x6a6) {
				return (1);
            }
			if (id == 0x6a8) {
				return (1);
            }
			if (id == 0x73a) {
				return (1);
            }
			if (id == 0x739) {
				return (1);
            }
			if (id == 0x73b) {
				return (1);
            }
			if (id == 0x738) {
				return (1);
            }
        }
		if ((((kind != 0x108) && (kind != 0xbc6)) ||
				((((((id != 0x6d2 && (((id != 0x3ea && (id != 0x733)) && (id != 0x71b)))) &&
						((((((id != 0x732 && (id != 0x69d)) && (id != 0x69f)) &&
							   ((id != 0x69e && (id != 0x6a0)))) &&
							  (((id != 0x6a1 &&
									((((id != 0x5c8 && (id != 0x411)) && (id != 0x412)) &&
										((id != 0x413 && (id != 0x414)))))) && (id != 0x415)))) &&
							((id != 0x416 && (id != 0x417)))))) &&
					   ((id != 0x418 && (((id != 0x419 && (id != 0x41a)) && (id != 0x41b)))))) &&
					  ((((id != 0x41c && (id != 0x41d)) && (id != 0x41e)) &&
						  (((((id != 0x41f && (id != 0x420)) &&
								 ((id != 0x6a3 && (((id != 0x6a4 && (id != 0x6a9)) && (id != 0x6ac)))))) &&
								((id != 0x6af && (id != 0x6b2)))) &&
							  (((id != 0x6b5 &&
									(((id != 0x6b8 && (id != 0x6a5)) &&
										((id != 0x6a7 &&
											(((((id != 0x6aa && (id != 0x6ad)) && (id != 0x6b0)) &&
												  ((id != 0x6b3 && (id != 0x6b6)))) && (id != 0x6b9)))))))) &&
								  (((id != 0x6a6 && (id != 0x6a8)) && (id != 0x6ab)))))))))) &&
					(((((id != 0x6ae && (id != 0x6b1)) && (id != 0x6b4)) &&
						  ((((id != 0x6b7 && (id != 0x6ba)) &&
								((id != 0x70a && ((id != 0x70b && (id != 0x70c)))))) && (id != 0x70d)))) &&
						((((id != 0x70e && (id != 0x70f)) && (id != 0x710)) && (id != 0x711)))))))) &&
			((Kind_Test(kind) != 0 && id == 0x695))) {
			return (3);
        }
    }
	return (1);
}


/// <summary>
/// Prepares one control for owner drawing.
/// This is the enumeration callback Subclass_Dialog uses. The control's window class and style
/// pick the custom procedure that will paint it, its original procedure is displaced by
/// CtrlProc and remembered, and the control is then told that it has been subclassed. The
/// shared fonts, color masks and cached artwork are built on the first control to arrive.
/// </summary>
/// <param name="lparam">Caller supplied value to store with the control for its own use.</param>
int __fastcall OwnerDraw_Control_Flags(unsigned int key, HWND window)
{

	OwnerDraw::WinData * record = 0;

	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if ((((((((((kind != 0x10c) && (kind != 0x94)) && (kind != 0xa3)) &&
				 (((kind != 0x73 && (kind != 0xd8)) &&
					 ((kind != 0xbbb && ((kind != 0xf5 && (kind != 0x105)))))))) && (kind != 0xe2)) &&
			   (((((kind != 0xd5 && (kind != 0x2b5)) && (kind != 0xb7)) &&
					 ((kind != 0x2b4 && (kind != 0xbc7)))) && (kind != 0x101)))) &&
			  ((((((kind != 0x129 && (kind != 0xbc6)) &&
					  (((kind != 0xd7 && (((kind != 0x108 && (kind != 0xbc)) && (kind != 0xbd)))) &&
						  (((kind != 0xbb && (kind != 0xb5)) && (kind != 0xbba)))))) &&
					 ((kind != 0xff && (kind != 0x6b)))) &&
					(((kind != 0x100 && (((kind != 0x102 && (kind != 0xb8)) && (kind != 0xd6)))) &&
						(((kind != 0x103 && (kind != 0x125)) &&
							((kind != 0xe7 &&
								(((kind != 0x116 && (kind != 0xc2)) &&
									((kind != 0xc9 &&
										((((kind != 0x11d && (kind != 0x11c)) && (kind != 0x114)) &&
											((kind != 700 && (kind != 0x113)))))))))))))))) &&
				  ((kind != 0x109 && ((kind != 0x10f && (kind != 0x117)))))))) &&
			 ((kind != 0xe6 && (((kind != 0xf3 && (kind != 0xf4)) && (kind != 0x10e)))))) ||
			(id != 0x694)) &&
		((((kind != 0xbc && (kind != 0xbd)) &&
			  ((kind != 0x102 && ((kind != 0xc2 && (kind != 0xc9)))))) ||
			((id != 0x6ec && (id != 0x5a8)))))) {
		if ((kind == 0x103) || (kind == 0xbc7)) {
			if (id == 0x6f4) {
				return 0x10;
            }
			if (id == 0x6f3 || id == 0x69f || id == 0x6f6 || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2 || id == 0x733 || id == 0x735 || id == 0x732 || id == 0x3ea) {
				return 8;
            }
			if (id == 0x73c || id == 0x73d || id == 0x73e || id == 0x73f || id == 0x740) {
				return 0x10;
            }
			bool wide = id == 0x411 || id == 0x412 || id == 0x419 || id == 0x41a || id == 0x6a3 || id == 0x6a4 || id == 0x6a5 || id == 0x6a7 || id == 0x6a6 || id == 0x6a8 || id == 0x73a || id == 0x739 || id == 0x73b || id == 0x738;
			if (wide) {
				return 0x20;
            }
        }
		if ((kind == 0x108) || (kind == 0xbc6)) {
			if (((((id == 0x6d2) || (((id == 0x3ea || (id == 0x733)) || (id == 0x71b)))) ||
					 (((id == 0x732 || (id == 0x69d)) || (id == 0x69f)))) ||
					((id == 0x69e || (id == 0x6a0)))) ||
				((((id == 0x6a1 ||
					   ((((id == 0x5c8 || (id == 0x411)) || (id == 0x412)) ||
						   ((id == 0x413 || (id == 0x414)))))) ||
					  ((id == 0x415 || ((id == 0x416 || (id == 0x417)))))) || (id == 0x418)))) {
				return (0x20);
            }
			bool tall = ((((((((id == 0x419) || (id == 0x41a)) || (id == 0x41b)) ||
								((((id == 0x41c || (id == 0x41d)) ||
									  ((id == 0x41e || ((id == 0x41f || (id == 0x420)))))) || (id == 0x6a3))))
                        || ((((id == 0x6a4 || (id == 0x6a9)) || (id == 0x6ac)) ||
								   ((id == 0x6af || (id == 0x6b2)))))) ||
							  ((((id == 0x6b5 || ((id == 0x6b8 || (id == 0x6a5)))) || (id == 0x6a7)) ||
								  (((((id == 0x6aa || (id == 0x6ad)) || (id == 0x6b0)) ||
										((id == 0x6b3 || (id == 0x6b6)))) || (id == 0x6b9)))))) ||
							 ((((id == 0x6a6 || (id == 0x6a8)) ||
								   ((id == 0x6ab || (((id == 0x6ae || (id == 0x6b1)) || (id == 0x6b4)))))) ||
								 (((id == 0x6b7 || (id == 0x6ba)) ||
									 ((id == 0x70a ||
										 (((id == 0x70b || (id == 0x70c)) ||
											 ((id == 0x70d || (((id == 0x70e || (id == 0x70f)) || (id == 0x710)))))))))
                        ))))) || (id == 0x711));
			if (tall) {
				return (0x40);
            }
        }
		char special = Kind_Test(kind);
		if ((special != 0) && (id == 0x695)) {
			return (0x10);
        }
    }
	return (8);
}


/// <summary>
/// Removes a dialog and all of its controls from the owner-draw dictionaries.
/// Use this routine as a subclassed dialog is torn down. Windows is free to hand the same
/// handles out again, and a stale entry would misdirect the next dialog to use them.
/// </summary>
namespace OwnerDraw {

int __fastcall Control_Typing_Sound(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (((kind == 0x103) || (kind == 0xbc7)) && (id == 0x6f4)) {
		return Rule->MessageCharTyped;
    }
	return -1;
}

}


/// <summary>
/// Removes one window from the owner-draw dictionaries.
/// This is the enumeration callback ODCleanupDicts uses. It drops the window's original
/// procedure, its custom procedure and its owner-draw record.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 1444 bytes differ (adopted text)
bool __fastcall Is_Tested_Control(unsigned int key, HWND window)
{

	OwnerDraw::WinData * record = 0;

	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if ((kind == 0x10c ||
			kind == 0x94 ||
			kind == 0xa3 ||
			kind == 0x73 ||
			kind == 0xd8 ||
			kind == 0xbbb ||
			kind == 0xf5 ||
			kind == 0x105 ||
			kind == 0xe2 ||
			kind == 0xd5 ||
			kind == 0x2b5 ||
			kind == 0xb7 ||
			kind == 0x2b4 ||
			kind == 0xbc7 ||
			kind == 0x101 ||
			kind == 0x129 ||
			kind == 0xbc6 ||
			kind == 0xd7 ||
			kind == 0x108 ||
			kind == 0xbc ||
			kind == 0xbd ||
			kind == 0xbb ||
			kind == 0xb5 ||
			kind == 0xbba ||
			kind == 0xff ||
			kind == 0x6b ||
			kind == 0x100 ||
			kind == 0x102 ||
			kind == 0xb8 ||
			kind == 0xd6 ||
			kind == 0x103 ||
			Is_Listed_Kind(kind)) && id == 0x694) {
		return true;
	}
	bool dual_rail = (((kind == 0xbc) || (kind == 0xbd)) ||
						 ((kind == 0x102 || ((kind == 0xc2 || (kind == 0xc9)))))) &&
		((id == 0x6ec || (id == 0x5a8)));
	if (dual_rail) {
		return true;
    }
	bool special_103 = ((kind == 0x103) || (kind == 0xbc7)) &&
		(((((id == 0x6f3 || (((id == 0x6f4 || (id == 0x69f)) || (id == 0x6f6)))) ||
			   (((id == 0x6ef || (id == 0x6f1)) || (id == 0x6f0)))) ||
			  (((id == 0x6f2 || (id == 0x733)) ||
				  ((id == 0x735 || (((id == 0x732 || (id == 0x3ea)) || (id == 0x736)))))))) ||
			(((id == 0x737 || (id == 0x73c)) ||
				((((id == 0x73d ||
					   (((id == 0x73e || (id == 0x73f)) ||
						   ((id == 0x740 ||
							   ((((id == 0x411 || (id == 0x412)) || (id == 0x419)) ||
								   ((id == 0x41a || (id == 0x6a3)))))))))) || (id == 0x6a4)) ||
					(((id == 0x6a5 || (id == 0x6a7)) ||
						(((id == 0x6a6 || (((id == 0x6a8 || (id == 0x73a)) || (id == 0x739)))) ||
							((id == 0x73b || (id == 0x738)))))))))))));
	if (special_103) {
		return true;
    }
	bool special_108 = ((kind == 0x108) || (kind == 0xbc6)) &&
		(((((id == 0x6d2 || (((id == 0x3ea || (id == 0x69d)) || (id == 0x69f)))) ||
			   ((((id == 0x69e || (id == 0x6a0)) || (id == 0x6a1)) ||
				   ((id == 0x5c8 || (id == 0x411)))))) ||
			  ((id == 0x412 ||
				  ((((((id == 0x413 || (id == 0x414)) || (id == 0x415)) ||
						 ((id == 0x416 || (id == 0x417)))) || (id == 0x418)) ||
					  ((id == 0x419 || (id == 0x41a)))))))) ||
			(((((id == 0x41b || (((id == 0x41c || (id == 0x41d)) || (id == 0x41e)))) ||
				   ((((id == 0x41f || (id == 0x420)) ||
						 ((((id == 0x6a3 || ((id == 0x6a4 || (id == 0x6a9)))) || (id == 0x6ac)) ||
							 (((id == 0x6af || (id == 0x6b2)) || (id == 0x6b5)))))) ||
					   ((((id == 0x6b8 || (id == 0x6a5)) ||
							 ((id == 0x6a7 || ((id == 0x6aa || (id == 0x6ad)))))) || (id == 0x6b0))))))
                || (((((id == 0x6b3 || (id == 0x6b6)) || (id == 0x6b9)) ||
						((id == 0x6a6 || (id == 0x6a8)))) ||
					  (((id == 0x6ab || ((id == 0x6ae || (id == 0x6b1)))) ||
						  ((id == 0x6b4 ||
							  ((((((id == 0x6b7 || (id == 0x6ba)) || (id == 0x70a)) ||
									 ((id == 0x70b || (id == 0x70c)))) ||
									((id == 0x70d || ((id == 0x70e || (id == 0x70f)))))) || (id == 0x710))))
                          )))))) ||
				((((id == 0x711 || (id == 0x733)) || (id == 0x732)) ||
					((id == 0x71b || (id == 0x735)))))))));
	if (special_108) {
		return true;
    }
	char special = Kind_Test(kind);
	if (((special != 0) && (id == 0x695)) && Session.Is_In_Game() == 0) {
		return true;
    }
	return false;
}
#endif


bool __fastcall Is_Listed_Kind(int kind)
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
namespace OwnerDraw {

bool __fastcall Is_Image_Control(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (((((((kind != 0xbc) && (kind != 0xbd)) && (kind != 0xc2)) && (kind != 0xc9)) ||
			 ((((id != 0x6d2 && (id != 0x6d3)) &&
				   ((id != 0x6d4 && ((id != 0x6d5 && (id != 0x6d6)))))) &&
				 ((id != 0x6d7 && ((id != 0x6d8 && (id != 0x6d9)))))))) &&
			((((kind != 0x102 && (((kind != 0xbc && (kind != 0xbd)) && (kind != 0xc2)))) &&
				  (kind != 0xc9)) ||
				(((((id != 0x6da && (id != 0x6db)) &&
					   ((id != 0x6dc && ((id != 0x6dd && (id != 0x6de)))))) && (id != 0x6df)) &&
					((id != 0x6e0 && (id != 0x6e1)))))))) &&
		(((kind != 0xc2 && (kind != 0xc9)) ||
			(((((id != 0x754 && (id != 0x755)) && (id != 0x756)) &&
				  (((id != 0x757 && (id != 0x758)) && (id != 0x759)))) &&
				((id != 0x75a && (id != 0x75b)))))))) {
		return false;
    }
	return true;
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
namespace OwnerDraw {

bool __fastcall Is_Image_Mode3_Control(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	return false;
}

}


/// <summary>
/// Hides the tooltip by restoring the area it covered.
/// The tooltip stays active while hidden, so it can be put back with Show_Tooltip once
/// whatever prompted the hide has finished painting.
/// </summary>
/// <returns>bool; Was the tooltip hidden?</returns>
namespace OwnerDraw {

bool __fastcall Is_Animated_Control(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) {
		return true;
    }
	if (kind == 0x94) {
		if (((id == 0x6ea) || (id == 0x6ec)) || (id == 0x6eb)) {
			return true;
        }
    }
    else if (kind == 0x103) {
		if (id == 0x72b) {
			return true;
        }
    }
    else if ((kind == 0xbc7) && (id == 0x72b)) {
		return true;
    }
	return false;
}

}


/// <summary>
/// Takes the tooltip down for good.
/// The area it covered is restored and the saved background released. Use this routine
/// when the mouse leaves the control, or when the control itself is going away.
/// </summary>
/// <returns>bool; Was there a tooltip to take down?</returns>
namespace OwnerDraw {

int __fastcall Control_Frame_Delay(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	return 0xc;
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
namespace OwnerDraw {

int __fastcall Control_Sound_Slot(HWND dialog, HWND window)
{
	WinData * record = Get_Record(dialog);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) {
		return 0x25;
    }
	if (((((kind == 0xbc) || (kind == 0xbd)) || (kind == 0xc2)) ||
			((kind == 0xc9 || (kind == 0x102)))) && ((id == 0x6ec || (id == 0x5a8)))) {
		return 0xe;
    }
	return 0;
}

}


namespace OwnerDraw {

int __fastcall Control_Animation_Loops(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) {
		return 1;
    }
	return 0;
}

}


namespace OwnerDraw {

int __fastcall Control_Animation_Rate(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) {
		return 100;
    }
	if (kind == 0x94) {
		if (id == 0x6ea || id == 0x6ec || id == 0x6eb) {
			return 100;
        }
    }
    else if (kind == 0x103) {
		if (id == 0x72b) {
			return 100;
        }
    }
    else if (kind == 0xbc7 && id == 0x72b) {
		return 100;
    }
	return 0;
}

}


void * __fastcall Get_Drawer();
void * __fastcall Get_Listed_Drawer();
void * Get_Slot_Art();
void * Get_Chat_Art();

namespace OwnerDraw {

void * __fastcall Control_Animation_Art(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	void * art = Get_Drawer();
	if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) {
		art = Get_Listed_Drawer();
    }
	if (kind == 0x94) {
		if (id == 0x6ea || id == 0x6eb || id == 0x6ec) {
			return Get_Slot_Art();
        }
	} else if (kind == 0x103) {
		if (id == 0x72b) {
			return Get_Chat_Art();
        }
	} else if (kind == 0xbc7 && id == 0x72b) {
		return Get_Chat_Art();
    }
	return art;
}

}


namespace OwnerDraw {

int __fastcall Control_Image(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	return 0;
}

}



namespace OwnerDraw {

void const * __fastcall Control_Animation_Shape(unsigned int key, HWND window, bool & allocated)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (Is_Listed_Dialog_Kind(kind) && id == 0x71c) {
		return Load_Alloc_Data(Listed_Kind_Art_Name, allocated);
    }
	if (kind == 0x94) {
		if (id == 0x6ea) {
			return Art_Side_Animation;
        }
		if (id == 0x6eb) {
			return Art_Button_Corner;
        }
		if (id == 0x6ec) {
			return Art_Side_Strip;
        }
	} else if ((kind == 0x103 || kind == 0xbc7) && id == 0x72b) {
		return Art_Score_Frame;
    }
	return 0;
}

}


namespace OwnerDraw {

inline bool __fastcall Is_Text_Control(HWND control)
{
	unsigned int key = (unsigned int)control;
	char name[128];
	GetClassNameA(control, name, 128);
	if (strcmp(name, "Edit") == 0) {
		return true;
    }
	if (strcmp(name, "NewEdit") == 0) {
		return true;
    }
	if (strcmp(name, "ListBox") == 0) {
		WinData * record = 0;
		if (Control_Table.getPointer(key, &record)) {
			return record->SkipTests == 1;
        }
    }
	if (strcmp(name, "ComboBox") == 0) {
		return (GetWindowLongA((HWND)key, GWL_STYLE) & 3) != 3;
    }
	return false;
}

int __fastcall Control_Text_Style(HWND dialog, HWND window)
{
	WinData * record = Get_Record(dialog);
	if (record != 0) {
		int kind = record->Kind;
		if (kind == 0x10d || kind == 0xd9 || kind == 0xf0 || kind == 0xce || kind == 0x120 ||
			kind == 0x121 || kind == 0x115 || kind == 0xd3 || kind == 0xcf || kind == 0x11f ||
			kind == 0xc3 || kind == 0x11b || kind == 0xe1 || kind == 0x11e || kind == 0xc4 ||
			kind == 300 || kind == 0xd0 || kind == 0xfc || kind == 0x126) {
			record->ButtonKind = 2;
			if (Is_Text_Control(window)) {
				return 0xc0;
            }
        }
    }
	return 0;
}

}


namespace OwnerDraw {

void __fastcall Set_Control_Image(HWND control, Surface * image)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->PictureMode = 2;
		record->Image = image;
		InvalidateRect(control, 0, TRUE);
    }
}

}


void __fastcall Send_Control_Value(HWND window, unsigned char value)
{
	SendMessageA(window, 0x4e5, 0, value);
}


void __fastcall Send_Control_Flag(HWND window, unsigned char flag)
{
	SendMessageA(window, 0x4e6, 0, (int)(flag & 0xff));
}


namespace OwnerDraw {

bool __fastcall Is_Control_Checked(HWND window)
{
	return SendMessageA(window, 0x4e7, 0, 0) ? true : false;
}

}


void __fastcall Send_Control_Pair(HWND window, int low, int high)
{
	SendMessageA(window, 0x4e8, 0, (LPARAM)((unsigned short)low | ((unsigned long)(unsigned short)high << 16)));
}


namespace OwnerDraw {

void __fastcall Set_Control_Result(HWND control, int value)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->Result = value;
    }
}

}


namespace OwnerDraw {

int __fastcall Get_Control_Result(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		return record->Result;
    }
	return -1;
}

}


namespace OwnerDraw {

WideWstring __fastcall Get_Item_Tip_Text(HWND window, int a, int b)
{
	TextRequest request;
	request.Item = a;
	request.Index = b;
	SendMessageA(window, 0x4e9, 0, (LPARAM)&request);
	return request.Text;
}

}


namespace OwnerDraw {

WideWstring __fastcall Get_Control_Tip_Text(HWND window, int a)
{
	TextRequest request;
	request.Item = a;
	request.Index = -1;
	SendMessageA(window, 0x4e9, 0, (LPARAM)&request);
	return request.Text;
}

}


namespace OwnerDraw {

WideWstring __fastcall Get_Item_Tip_Text_At(HWND window, HWND item, int x, int y)
{
	int index = SendMessageA(item, 0x4e8, 0, (unsigned short)x | ((unsigned long)(unsigned short)y << 16));
	WideWstring text = Get_Item_Tip_Text(window, (int)item, index);
	return text;
}

}


namespace OwnerDraw {

WideWstring __fastcall Control_Tooltip(unsigned int key, HWND window)
{
	char const * label = Control_Tooltip_Label(key, window);
	if (label) {
		return WideWstring(StringTable::LoadString(label, 0, "D:\\Ra2\\ownrdraw.cpp", 1902));
    }
	return WideWstring(L"");
}

}


namespace OwnerDraw {

inline int __cdecl Record_Kind(unsigned int key)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	return record != 0 ? record->Kind : 0;
}
char const * __fastcall Control_Tooltip_Label(unsigned int key, HWND window)
{
	int kind = Record_Kind(key);
	int id = GetDlgCtrlID(window);
	if (kind == 0xe2) {
		if (id == 0x683) {
			return "STT:MainButtonSinglePlayer";
        }
		if (id == 0x684) {
			return "STT:MainButtonWWOnline";
        }
		if (id == 0x578) {
			return "STT:MainButtonNetwork";
        }
		if (id == 0x686) {
			return "STT:MainButtonMovies";
        }
		if (id == 0x55c) {
			return "STT:MainButtonOptions";
        }
		if (id == 0x3ee) {
			return "STT:MainButtonExitGame";
        }
	} else if (kind == 0x100) {
		if (id == 0x688) {
			return "STT:SingleButtonNewCampaign";
        }
		if (id == 0x689) {
			return "STT:SingleButtonLoadSavedGame";
        }
		if (id == 0x579) {
			return "STT:SingleButtonSkirmish";
        }
		if (id == 0x686) {
			return "STT:SingleButtonBack";
        }
	} else if (kind == 0x94) {
		if (id == 0x6ea) {
			return "STT:CampaignAnimAllied";
        }
		if (id == 0x6eb) {
			return "STT:CampaignAnimTutorial";
        }
		if (id == 0x6ec) {
			return "STT:CampaignAnimSoviet";
        }
		if (id == 0x50f) {
			return "STT:CampaignSliderDifficulty";
        }
		if (id == 0x40e) {
			return "STT:CampaignButtonLoad";
        }
		if (id == 0x455) {
			return "STT:CampaignList";
        }
		if (id == 0x686) {
			return "STT:CampaignButtonBack";
        }
	} else if (kind == 0xb7) {
		if (id == 0x525) {
			return "STT:LoadList";
        }
		if (id == 0x40f) {
			return "STT:LoadButtonLoad";
        }
		if (id == 0x686) {
			return "STT:LoadButtonBack";
        }
	} else if (kind == 0x102) {
		if (id == 0x6a0) {
			return "STT:SkirmishEditPlayer";
        }
		if ((((id == 0x50b) || (id == 0x50e)) || (id == 0x516)) || (((id == 0x51a || (id == 0x51b)) || ((id == 0x51c || (id == 0x51d)))))) {
			return "STT:SkirmishComboAIPlayer";
        }
		if (((id == 0x6da) || (id == 0x6db)) || (((id == 0x6dc || (((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)))) || ((id == 0x6e0 || (id == 0x6e1)))))) {
			return "STT:SkirmishPictureFlag";
        }
		if (((id == 0x6a1) || (id == 0x510)) || ((id == 0x513 || ((((id == 0x51e || (id == 0x514)) || (id == 0x51f)) || ((id == 0x520 || (id == 0x521)))))))) {
			return "STT:SkirmishComboCountry";
        }
		if (((id == 0x6a2) || (id == 0x522)) || (((id == 0x523 || (((id == 0x524 || (id == 0x525)) || (id == 0x526)))) || ((id == 0x527 || (id == 0x528)))))) {
			return "STT:SkirmishComboColor";
        }
		if (id == 0x529) {
			return "STT:SkirmishSliderSpeed";
        }
		if (id == 0x511) {
			return "STT:SkirmishSliderCredits";
        }
		if (id == 0x50c) {
			return "STT:SkirmishSliderUnit";
        }
		if (id == 0x5aa) {
			return "STT:SkirmishButtonChooseMap";
        }
		if (id == 0x693) {
			return "STT:SkirmishCBoxRedeploys";
        }
		if (id == 0x54e) {
			return "STT:SkirmishCBoxShortGame";
        }
		if (id == 0x69a) {
			return "STT:SkirmishCBoxSWAllowed";
        }
		if (id == 0x69d) {
			return "STT:SkirmishCBoxBuildOffAlly";
        }
		if (id == 0x696) {
			return "STT:SkirmishCBoxCrates";
        }
		if (id == 0x468) {
			return "STT:SkirmishMapThumbnail";
        }
		if (id == 0x6ec) {
			return "STT:SkirmishLabelGameType";
        }
		if (id == 0x5a8) {
			return "STT:SkirmishLabelScenario";
        }
		if (id == 0x617) {
			return "STT:SkirmishButtonStartGame";
        }
		if (id == 0x5c0) {
			return "STT:SkirmishButtonBack";
        }
	} else if (kind == 0xbb) {
		if (id == 0x541) {
			return "STT:LobbyEditYourName";
        }
		if (id == 0x53e) {
			return "STT:LobbyListUsers";
        }
		if (id == 0x549) {
			return "STT:LobbyListGames";
        }
		if (id == 0x53f) {
			return "STT:LobbyEditOutput";
        }
		if (id == 0x540) {
			return "STT:LobbyEditInput";
        }
		if (id == 0x6cc) {
			return "STT:LobbyButtonNew";
        }
		if (id == 0x6cd) {
			if (ObserverMode == 0) {
				return "STT:LobbyButtonJoin";
            }
			return "STT:LobbyButtonObserve";
        }
		if (id == 0x760) {
			return "STT:ObserverToggle";
        }
		if (id == 0x6ce) {
			return "STT:LobbyButtonObserve";
        }
		if (id == 0x5c0) {
			return "STT:LobbyButtonBack";
        }
	} else if (kind == 0xbc || kind == 0xc2) {
		if ((((id == 0x6d2) || (((id == 0x6d3 || (id == 0x6d4)) || (id == 0x6d5)))) || (((id == 0x6d6 || (id == 0x6d7)) || (id == 0x6d8)))) || (id == 0x6d9)) {
			return "STT:HostPictureAcceptance";
        }
		if (((id == 0x411) || (id == 0x412)) || ((id == 0x413 || ((((id == 0x414 || (id == 0x415)) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))))))) {
			return "STT:HostComboPlayer";
        }
		if (((id == 0x6da) || (id == 0x6db)) || (((id == 0x6dc || (((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)))) || ((id == 0x6e0 || (id == 0x6e1)))))) {
			return "STT:HostPictureFlag";
        }
		if ((((id == 0x6a1) || (id == 0x510)) || ((id == 0x513 || (((id == 0x51e || (id == 0x514)) || (id == 0x51f)))))) || ((id == 0x520 || (id == 0x521)))) {
			return "STT:HostComboCountry";
        }
		if (((id == 0x6a2) || (id == 0x522)) || ((id == 0x523 || ((((id == 0x524 || (id == 0x525)) || (id == 0x526)) || ((id == 0x527 || (id == 0x528)))))))) {
			return "STT:HostComboColor";
        }
		if (id == 0x529) {
			return "STT:HostSliderSpeed";
        }
		if (id == 0x402) {
			return "STT:HostSliderCredits";
        }
		if (id == 0x54f) {
			return "STT:HostSliderUnit";
        }
		if (id == 1000) {
			return "STT:HostCBoxRedeploys";
        }
		if (id == 0x54e) {
			return "STT:HostCBoxShortGame";
        }
		if (id == 0x69a) {
			return "STT:HostCBoxSWAllowed";
        }
		if (id == 0x69d) {
			return "STT:HostCBoxBuildOffAlly";
        }
		if (id == 0x54d) {
			return "STT:HostCBoxCrates";
        }
		if (id == 0x53f) {
			return "STT:HostEditOutput";
        }
		if (id == 0x540) {
			return "STT:HostEditInput";
        }
		if (id == 0x468) {
			return "STT:HostMapThumbnail";
        }
		if (id == 0x5a8) {
			return "STT:HostLabelScenario";
        }
		if (id == 0x588) {
			return "STT:HostButtonGo";
        }
		if (id == 0x5aa) {
			return "STT:HostButtonChooseMap";
        }
		if (id == 2) {
			if (kind == 0xc2) {
				return "STT:WOLNewGameCancel";
            }
			return "STT:HostButtonBack";
        }
	} else if (kind == 0xbd || kind == 0xc9) {
		if (((id == 0x6d2) || (((id == 0x6d3 || (id == 0x6d4)) || (id == 0x6d5)))) || ((((id == 0x6d6 || (id == 0x6d7)) || (id == 0x6d8)) || (id == 0x6d9)))) {
			return "STT:GuestPictureAcceptance";
        }
		if ((((id == 0x411) || (id == 0x412)) || ((id == 0x413 || (((id == 0x414 || (id == 0x415)) || (id == 0x416)))))) || ((id == 0x417 || (id == 0x418)))) {
			return "STT:GuestComboPlayer";
        }
		if (((id == 0x6da) || (id == 0x6db)) || ((id == 0x6dc || ((((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)) || ((id == 0x6e0 || (id == 0x6e1)))))))) {
			return "STT:GuestPictureFlag";
        }
		if (((id == 0x6a1) || (id == 0x510)) || (((id == 0x513 || (((id == 0x51e || (id == 0x514)) || (id == 0x51f)))) || ((id == 0x520 || (id == 0x521)))))) {
			return "STT:GuestComboCountry";
        }
		if ((((id == 0x6a2) || (id == 0x522)) || ((id == 0x523 || (((id == 0x524 || (id == 0x525)) || (id == 0x526)))))) || ((id == 0x527 || (id == 0x528)))) {
			return "STT:GuestComboColor";
        }
		if (id == 0x529) {
			return "STT:GuestSliderSpeed";
        }
		if (id == 0x402) {
			return "STT:GuestSliderCredits";
        }
		if (id == 0x54f) {
			return "STT:GuestSliderUnit";
        }
		if (id == 1000) {
			return "STT:GuestCBoxRedeploys";
        }
		if (id == 0x54e) {
			return "STT:GuestCBoxShortGame";
        }
		if (id == 0x69a) {
			return "STT:GuestCBoxSWAllowed";
        }
		if (id == 0x69d) {
			return "STT:GuestCBoxBuildOffAlly";
        }
		if (id == 0x54d) {
			return "STT:GuestCBoxCrates";
        }
		if (id == 0x53f) {
			return "STT:GuestEditOutput";
        }
		if (id == 0x540) {
			return "STT:GuestEditInput";
        }
		if (id == 0x468) {
			return "STT:GuestMapThumbnail";
        }
		if (id == 0x5a8) {
			return "STT:GuestLabelScenario";
        }
		if (id == 0x59f) {
			return "STT:GuestButtonAccept";
        }
		if (id == 2) {
			if (kind == 0xc9) {
				return "STT:WOLNewGameCancel";
            }
			return "STT:GuestButtonBack";
        }
	} else if (kind == 0x101) {
		if (id == 0x68d) {
			return "STT:OptionsButtonSneak";
        }
		if (id == 0x68e) {
			return "STT:OptionsButtonMovies";
        }
		if (id == 0x68f) {
			return "STT:OptionsButtonCredits";
        }
		if (id == 0x686) {
			return "STT:OptionsButtonBack";
        }
	} else if (kind == 0x129) {
		if (id == 0x745) {
			return "GUI:PlayMovie";
        }
		if (id == 0x686) {
			return "STT:MsnDltButtonBack";
        }
		if (id == 0x744) {
			return "GUI:SelectMovie";
        }
	} else if (kind == 0xd5) {
		if (id == 0x5ce) {
			return "STT:MainOptButtonKeyboard";
        }
		if (id == 0x5cd) {
			return "STT:MainOptButtonNetwork";
        }
		if (id == 0x686) {
			return "STT:MainOptButtonBack";
        }
		if (id == 0x674) {
			return "STT:MainOptCBoxStretch";
        }
		if (id == 0x52b) {
			return "STT:MainOptSliderVisual";
        }
		if (id == 0x6ed) {
			return "STT:MainOptComboModes";
        }
		if (id == 0x529) {
			return "STT:MainOptSliderSpeed";
        }
		if (id == 0x50f) {
			return "STT:MainOptSliderDifficulty";
        }
		if (id == 0x601) {
			return "STT:MainOptCBoxTargetLines";
        }
		if (id == 0x604) {
			return "STT:MainOptCBoxHidden";
        }
		if (id == 0x602) {
			return "STT:MainOptCBoxTooltips";
        }
		if (id == 0x52a) {
			return "STT:MainOptSliderScroll";
        }
		if (id == 0x51a) {
			return "STT:MainOptCBoxScrollCoasting";
        }
		if (id == 0x52f) {
			return "STT:MainOptSliderMusic";
        }
		if (id == 0x532) {
			return "STT:MainOptSliderSound";
        }
		if (id == 0x536) {
			return "STT:MainOptSliderVoice";
        }
	} else if (kind == 0xd7) {
		if (id == 0x5d4) {
			return "STT:MultiOptComboCard";
        }
		if (id == 0x5d1) {
			return "STT:MultiOptEditSocketNum";
        }
		if (id == 0x5d3) {
			return "STT:MultiOptEditDestAddr";
        }
		if (id == 0x762) {
			return "STT:MultiOptSendDelay";
        }
		if (id == 0x5d7) {
			return "STT:MultiOptPortNumber";
        }
		if (id == 1) {
			return "STT:MultiOptButtonOk";
        }
		if (id == 2) {
			return "STT:MultiOptButtonCancel";
        }
	} else if (kind == 0xa3) {
		if (id == 0x4c7) {
			return "STT:KeyboardComboCategory";
        }
		if (id == 0x6e9) {
			return "STT:KeyboardGroupDescription";
        }
		if (id == 0x4c8) {
			return "STT:KeyboardListCommands";
        }
		if (id == 0x63c) {
			return "STT:KeyboardLabelShortcut";
        }
		if (id == 0x63d) {
			return "STT:KeyboardLabelError";
        }
		if (id == 0x4cd) {
			return "STT:KeyboardEditEntry";
        }
		if (id == 0x4cf) {
			return "STT:KeyboardButtonAssign";
        }
		if (id == 0x63b) {
			return "STT:KeyboardLabelAssigned";
        }
		if (id == 0x4d0) {
			return "STT:KeyboardButtonResetAll";
        }
		if (id == 0x686) {
			return "STT:KeyboardButtonBack";
        }
	} else if (kind == 0x10c) {
		if (id == 0x6e7) {
			return "STT:BriefingLabelContent";
        }
		if (id == 0x423) {
			return "STT:BriefingButtonReturn";
        }
	} else if (kind == 0xb6) {
		if (id == 0x6c9) {
			int state = Get_Control_Result(window);
			if (state == 3) {
				return "STT:ConfirmExitButtonLeaveEx";
            }
			if (state == 2) {
				return "STT:ConfirmExitButtonLeave";
            }
			return "STT:Empty";
        }
		if (id == 0x712) {
			int state = Get_Control_Result(window);
			if (state == 4) {
				return "STT:ConfirmExitButtonObserve";
            }
			if (state == 5) {
				return "STT:ConfirmExitButtonRestart";
            }
			return "STT:Empty";
        }
		if (id == 0x686) {
			return "STT:ConfirmExitButtonResume";
        }
	} else if (kind == 0xd9) {
		if (id == 0x5c1) {
			return "STT:ConfirmModeButtonOk";
        }
		if (id == 0x6ca) {
			return "STT:ConfirmModeButtonCancel";
        }
	} else if (kind == 0x73) {
		if ((((((id == 0x411) || (id == 0x412)) || (id == 0x413)) || ((id == 0x414 || (id == 0x415)))) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))) {
			return "STT:DiplomacyLabelPlayer";
        }
		if ((((id == 0x6cf) || (id == 0x6d0)) || (id == 0x6d1)) || (((id == 0x6d2 || (id == 0x6d3)) || ((id == 0x6d4 || (id == 0x6d5)))))) {
			return "STT:DiplomacyCBoxAllies";
        }
		if ((((id == 0x6d6) || (id == 0x6d7)) || ((id == 0x6d8 || (((id == 0x6d9 || (id == 0x6e2)) || (id == 0x6e3)))))) || (id == 0x6e4)) {
			return "STT:DiplomacyCBoxListeners";
        }
		if ((((id == 0x419) || (id == 0x41a)) || ((id == 0x41b || ((id == 0x41c || (id == 0x41d)))))) || ((id == 0x41e || ((id == 0x41f || (id == 0x420)))))) {
			return "STT:DiplomacyCBoxKills";
        }
		if (id == 0x423) {
			return "STT:DiplomacyButtonReturn";
        }
		if (id == 0x730) {
			return "STT:TauntsOn";
        }
	} else if (kind == 0xbbb) {
		if (id == 0x529) {
			return "STT:IGGameOptSliderSpeed";
        }
		if (id == 0x52a) {
			return "STT:IGGameOptSliderScroll";
        }
		if (id == 0x52b) {
			return "STT:IGGameOptSliderVisual";
        }
		if (id == 0x602) {
			return "STT:IGGameOptCBoxTooltips";
        }
		if (id == 0x601) {
			return "STT:IGGameOptCBoxTargetLines";
        }
		if (id == 0x604) {
			return "STT:IGGameOptCBoxHidden";
        }
		if (id == 0x51a) {
			return "STT:IGGameOptCBoxScrollCoast";
        }
		if (id == 0x52d) {
			return "STT:IGGameOptButtonSound";
        }
		if (id == 0x52c) {
			return "STT:IGGameOptButtonKeyboard";
        }
		if (id == 0x686) {
			return "STT:IGGameOptButtonBack";
        }
	} else if (kind == 0x105) {
		if (id == 0x405) {
			return "STT:GenerateCBoxEnvironment";
        }
		if (id == 0x3ea) {
			return "STT:GenerateCBoxTime";
        }
		if (id == 0x407) {
			return "STT:GenerateCBoxTheater";
        }
		if (id == 0x406) {
			return "STT:GenerateCBoxMapSize";
        }
		if (id == 0x408) {
			return "STT:GenerateCBoxResources";
        }
		if (id == 0x3eb) {
			return "STT:GenerateSliderNumPlayers";
        }
		if (id == 0x621) {
			return "STT:GenerateButtonSurprise";
        }
		if (id == 0x620) {
			return "STT:GenerateButtonPreview";
        }
		if (id == 0x6c2) {
			return "STT:GenerateButtonLoadMap";
        }
		if (id == 0x6c3) {
			return "STT:GenerateButtonSaveMap";
        }
		if (id == 0x6c4) {
			return "STT:GenerateButtonDeleteMap";
        }
		if (id == 0x6c5) {
			return "STT:GenerateButtonUseMap";
        }
		if (id == 0x5c0) {
			return "STT:GenerateButtonCancel";
        }
	} else if (kind == 0x2b5) {
		if (id == 0x528) {
			return "STT:MsnDltListMission";
        }
		if (id == 0x6c8) {
			return "STT:MsnDltButtonDelete";
        }
		if (id == 0x686) {
			return "STT:MsnDltButtonBack";
        }
	} else if (kind == 0x2b4) {
		if (id == 0x527) {
			return "STT:MsnSaveListMission";
        }
		if (id == 0x526) {
			return "STT:MsnSaveEditName";
        }
		if (id == 0x6c7) {
			return "STT:MsnSaveButtonSave";
        }
		if (id == 0x686) {
			return "STT:MsnSaveButtonBack";
        }
	} else if (kind == 0x108) {
		if (id == 0x6d2) {
			return "STT:MPScoreLabelGame";
        }
		if (id == 0x3ea) {
			return "STT:MPScoreLabelTime";
        }
		if ((id == 0x411) || (id == 0x412) || (id == 0x413) || (id == 0x414) || (id == 0x415) || (id == 0x416) || (id == 0x417) || (id == 0x418)) {
			return "STT:MPScoreLabelPlayer";
        }
		if ((id == 0x419) || (id == 0x41a) || (id == 0x41b) || (id == 0x41c) || (id == 0x41d) || (id == 0x41e) || (id == 0x41f) || (id == 0x420)) {
			return "STT:MPScoreLabelKills";
        }
		if ((id == 0x6a3) || (id == 0x6a4) || (id == 0x6a9) || (id == 0x6ac) || (id == 0x6af) || (id == 0x6b2) || (id == 0x6b5) || (id == 0x6b8)) {
			return "STT:MPScoreLabelLosses";
        }
		if ((id == 0x6a5) || (id == 0x6a7) || (id == 0x6aa) || (id == 0x6ad) || (id == 0x6b0) || (id == 0x6b3) || (id == 0x6b6) || (id == 0x6b9)) {
			return "STT:MPScoreLabelBuilt";
        }
		if ((id == 0x6a6) || (id == 0x6a8) || (id == 0x6ab) || (id == 0x6ae) || (id == 0x6b1) || (id == 0x6b4) || (id == 0x6b7) || (id == 0x6ba)) {
			return "STT:MPScoreLabelScore";
        }
		if (id == 0x6d1) {
			return "STT:MPScoreButtonContinue";
        }
	} else if (kind == 0xb5) {
		if (id == 0x521) {
			return "STT:IGSingleOptButtonControls";
        }
		if (id == 0x51e) {
			return "STT:IGSingleOptButtonLoad";
        }
		if (id == 0x51f) {
			return "STT:IGSingleOptButtonSave";
        }
		if (id == 0x520) {
			return "STT:IGSingleOptButtonDelete";
        }
		if (id == 0x522) {
			return "STT:IGSingleOptButtonAbort";
        }
		if (id == 0x686) {
			return "STT:IGSingleOptButtonBack";
        }
	} else if (kind == 0xbba) {
		if (id == 0x521) {
			return "STT:IGMultiOptButtonControls";
        }
		if (id == 0x522) {
			return "STT:IGMultiOptButtonAbort";
        }
		if (id == 0x686) {
			return "STT:IGMultiOptButtonResume";
        }
	} else if (kind == 0xea) {
		if ((((id == 0x640) || (id == 0x642)) || (id == 0x641)) || (((id == 0x643 || (id == 0x644)) || ((id == 0x645 || ((id == 0x646 || (id == 0x647)))))))) {
			return "STT:ReconnectButtonPlayer";
        }
		if ((((id == 0x648) || (id == 0x649)) || (id == 0x64a)) || (((id == 0x64b || (id == 0x64c)) || ((id == 0x64d || ((id == 0x64e || (id == 0x64f)))))))) {
			return "STT:ReconnectProgressPlayer";
        }
		if (id == 0x650) {
			return "STT:ReconnectLabelTime";
        }
		if (id == 0x651) {
			return "STT:ReconnectEditInfo";
        }
		if (id == 2) {
			return "STT:ReconnectButtonLeaveGame";
        }
	} else if (kind == 0x6b) {
		if (id == 0x6eb) {
			return "STT:ScenarioListGameType";
        }
		if (id == 0x553) {
			return "STT:ScenarioListMaps";
        }
		if (id == 0x468) {
			return "STT:ScenarioMapThumbnail";
        }
		if (id == 0x6c5) {
			return "STT:ScenarioButtonUseMap";
        }
		if (id == 0x583) {
			return "STT:ScenarioButtonRandom";
        }
		if (id == 0x5c0) {
			return "STT:ScenarioButtonCancel";
        }
	} else if (kind == 0xb8) {
		if (id == 0x52f) {
			return "STT:SoundOptSliderMusic";
        }
		if (id == 0x532) {
			return "STT:SoundOptSliderSound";
        }
		if (id == 0x536) {
			return "STT:SoundOptSliderVoice";
        }
		if (id == 0x530) {
			return "STT:SoundOptListScore";
        }
		if (id == 0x533) {
			return "STT:SoundOptCBoxShuffle";
        }
		if (id == 0x534) {
			return "STT:SoundOptCBoxRepeat";
        }
		if (id == 0x531) {
			return "STT:SoundOptButtonPlay";
        }
		if (id == 0x535) {
			return "STT:SoundOptButtonStop";
        }
		if (id == 0x686) {
			return "STT:SoundOptButtonBack";
        }
	} else if (kind == 0x103) {
		if (id == 0x6f1) {
			return "STT:StandScoreParTime";
        }
		if (id == 0x6f2) {
			return "STT:StandScoreYourTime";
        }
		if (id == 0x6f3) {
			return "STT:StandScoreTitle";
        }
		if (id == 0x6f4) {
			return "STT:StandScoreMessage";
        }
		if (id == 0x6f6) {
			return "STT:StandScoreKills";
        }
		if (id == 0x423) {
			return "STT:StandScoreButtonReturn";
        }
	} else if (kind == 0xff) {
		if (id == 0x3ef) {
			return "STT:OptionInternetLatency";
        }
		if (id == 0x529) {
			return "STT:OptionInternetSpeedSlider";
        }
		if (id == 0x521) {
			return "STT:OptionInternetGameControls";
        }
		if (id == 0x522) {
			return "STT:OptionInternetAbortMission";
        }
		if (id == 0x675) {
			return "STT:IGMultiOptButtonResume";
        }
	} else if (kind == 0x122) {
		if (id == 0x6f9) {
			return "STT:WOLAddBuddyQueryDoNotAsk";
        }
		if (id == 1) {
			return "STT:WOLAddBuddyQueryYes";
        }
		if (id == 2) {
			return "STT:WOLAddBuddyQueryNo";
        }
	} else if (kind == 0x112) {
		if (id == 0x6f9) {
			return "STT:WOLAutoLoginDoNotAsk";
        }
		if (id == 1) {
			return "STT:WOLAutoLoginYes";
        }
		if (id == 2) {
			return "STT:WOLAutoLoginNo";
        }
	} else if (kind == 0x116) {
		if (id == 0x689) {
			return "STT:WOLBuddyBuddyList";
        }
		if (id == 0x6f0) {
			return "STT:WOLBuddyWins";
        }
		if (id == 0x6f1) {
			return "STT:WOLBuddyLosses";
        }
		if (id == 0x701) {
			return "STT:WOLBuddyDisconnects";
        }
		if (id == 0x6f4) {
			return "STT:WOLBuddyRank";
        }
		if (id == 0x6f5) {
			return "STT:WOLBuddyPoints";
        }
		if (id == 0x62b) {
			return "STT:WOLBuddyCreateGame";
        }
		if (id == 0x62d) {
			return "STT:WOLBuddyJoin";
        }
		if (id == 0x760) {
			return "STT:ObserverToggle";
        }
		if (id == 0x5c2) {
			return "STT:WOLBuddyPage";
        }
		if (id == 0x702) {
			return "STT:WOLBuddyAdd";
        }
		if (id == 0x703) {
			return "STT:WOLBuddyRemove";
        }
		if (id == 0x704) {
			return "STT:WOLBuddyRefresh";
        }
		if (id == 0x686) {
			return "STT:WOLBuddyBack";
        }
	} else if (kind == 0x113) {
		if (id == 0x55d) {
			return "STT:WOLLobbyRefreshChannels";
        }
		if (id == 0x53f) {
			return "STT:LobbyEditOutput";
        }
		if (id == 0x540) {
			return "STT:LobbyEditInput";
        }
		if (id == 0x3ed) {
			return "STT:EmoteButton";
        }
		if (id == 0x53d) {
			return "STT:WOLLobbyChannelList";
        }
		if (id == 0x62b) {
			return "STT:LobbyButtonNew";
        }
		if (id == 0x62d) {
			if (ObserverMode == 0) {
				return "STT:LobbyButtonJoin";
            }
			return "STT:LobbyButtonObserve";
        }
		if (id == 0x760) {
			return "STT:ObserverToggle";
        }
		if (id == 0x689) {
			return "STT:WOLBuddy";
        }
		if (id == 0x558) {
			return "STT:WOLClanLadder";
        }
		if (id == 0x61f) {
			return "STT:WOLLobbyOptions";
        }
		if (id == 0x686) {
			return "STT:WOLLobbyBack";
        }
	} else if (kind == 0xfe) {
		if (id == 0x3f0) {
			return "STT:WOLMyInformationLocale";
        }
		if (id == 1) {
			return "STT:WOLLocaleOK";
        }
		if (id == 2) {
			return "STT:WOLLocaleCancel";
        }
	} else if (kind == 0x10f) {
		if (id == 0x628) {
			return "STT:WOLLoginNickname";
        }
		if (id == 0x72c) {
			return "STT:WOLLoginPassword";
        }
		if (id == 0x72d) {
			return "STT:WOLLoginRemember";
        }
		if (id == 0x539) {
			return "STT:WOLLoginLogin";
        }
		if (id == 0x6ea) {
			return "STT:WOLNewAccount";
        }
		if (id == 0x6eb) {
			return "STT:WOLLoginForget";
        }
		if (id == 0x686) {
			return "STT:WOLLoginBack";
        }
		if (id == 0x6ec) {
			return "STT:WOLManageAccount";
        }
	} else if (kind == 0x11d) {
		if (id == 0x628) {
			return "STT:IGRLogin";
        }
		if (id == 0x72c) {
			return "STT:WOLLoginPassword";
        }
		if (id == 0x539) {
			return "STT:WOLLoginLogin";
        }
		if (id == 0x6ea) {
			return "STT:WOLNewAccount";
        }
		if (id == 0x686) {
			return "STT:WOLLoginBack";
        }
		if (id == 0x6ec) {
			return "STT:WOLManageAccount";
        }
		if (id == 0x629) {
			return "STT:WOLMyInformationServer";
        }
		if (id == 0x62a) {
			return "STT:WOLMyInformationLocale";
        }
		if (id == 0x6a1) {
			return "STT:WOLMyInformationCountry";
        }
		if (id == 0x6a2) {
			return "STT:WOLMyInformationColor";
        }
	} else if (kind == 0x117) {
		if (id == 0x53f) {
			return "STT:WOLMatchOutput";
        }
		if (id == 0x5c0) {
			return (Session.Type == GAME_INTERNET) && (Online_Session_State == 2) ? "WOL:WDTMatchBack" : "STT:WOLMatchCancel";
        }
	} else if (kind == 0x114) {
		if (id == 0x628) {
			return "STT:WOLMyInformationName";
        }
		if (id == 0x629) {
			return "STT:WOLMyInformationServer";
        }
		if (id == 0x62a) {
			return "STT:WOLMyInformationLocale";
        }
		if (id == 0x6a1) {
			return "STT:WOLMyInformationCountry";
        }
		if (id == 0x6a2) {
			return "STT:WOLMyInformationColor";
        }
		if (id == 0x6ff) {
			return "STT:WOLMyInformationAutoLogin";
        }
		if (id == 0x6f0) {
			return "STT:WOLMyInformationWins";
        }
		if (id == 0x6f1) {
			return "STT:WOLMyInformationLosses";
        }
		if (id == 0x701) {
			return "STT:WOLMyInformationDisconnects";
        }
		if (id == 0x6f4) {
			return "STT:WOLMyInformationRank";
        }
		if (id == 0x6f5) {
			return "STT:WOLMyInformationPoints";
        }
		if (id == 0x687) {
			return "STT:WOLBuddy";
        }
		if (id == 0x688) {
			return "STT:WOLClanLadder";
        }
		if (id == 0x686) {
			return "STT:WOLMyInformationBack";
        }
		if (id == 0x6ea) {
			return "STT:WOLNewAccount";
        }
		if (id == 0x6eb) {
			return "STT:WOLLoginForget";
        }
		if (id == 0x6ec) {
			return "STT:WOLManageAccount";
        }
	} else if (kind == 0xc3) {
		if (id == 0x548) {
			return "STT:WOLNewGameMaxPlayers";
        }
		if (id == 0x72c) {
			return "STT:WOLNewGamePassword";
        }
		if (id == 0x629) {
			return "STT:WOLNewGameTournament";
        }
		if (id == 0x66c) {
			return "STT:WOLNewGameClanTournament";
        }
		if (id == 1) {
			return "STT:WOLNewGameOK";
        }
		if (id == 2) {
			return "STT:WOLNewGameCancel";
        }
		if (id == 0x71e) {
			return "STT:WOLLimitScreenResolution";
        }
	} else if (kind == 0x2bc) {
		if (id == 0x5f5) {
			return "STT:WOLOptionsPage";
        }
		if (id == 0x5f6) {
			return "STT:WOLOptionsLanguageFilter";
        }
		if (id == 0x5f7) {
			return "STT:WOLOptionsFind";
        }
		if (id == 0x5f8) {
			return "STT:WOLOptionsMusic";
        }
		if (id == 0x5f9) {
			return "STT:WOLOptionsShowAllInLobby";
        }
		if (id == 1) {
			return "STT:WOLOptionsOk";
        }
		if (id == 0x63a) {
			return "STT:WOLOptionsSoundOptions";
        }
		if (id == 2) {
			return "STT:WOLOptionsCancel";
        }
		if (id == 0x6ed) {
			return "STT:MainOptComboModes";
        }
		if (id == 0x5fa) {
			return "STT:WOLOptionFilterAsian";
        }
		if (id == 0x5fb) {
			return "STT:WOLOptionFilterNonAsian";
        }
	} else if (kind == 0x10e) {
		if (id == 0x6f0) {
			return "STT:WOLMyInformationWins";
        }
		if (id == 0x6f1) {
			return "STT:WOLMyInformationLosses";
        }
		if (id == 0x701) {
			return "STT:WOLMyInformationDisconnects";
        }
		if (id == 0x6f4) {
			return "STT:WOLMyInformationRank";
        }
		if (id == 0x6f5) {
			return "STT:WOLMyInformationPoints";
        }
		if (id == 0x6e0) {
			return "STT:WOLWelcomeQuickMatch";
        }
		if (id == 0x6e1) {
			return "STT:WOLWelcomeCustomMatch";
        }
		if (id == 0x6e2) {
			return "STT:WOLBuddy";
        }
		if (id == 0x6e3) {
			return "STT:WOLWelcomeWDT";
        }
		if (id == 0x6e4) {
			return "STT:WOLWelcomeMyInformation";
        }
		if (id == 0x686) {
			return "STT:WOLWelcomeBack";
        }
	} else if (kind == 0x11c) {
		if (id == 0x6e0) {
			return "STT:WOLWelcomeQuickMatch";
        }
		if (id == 0x6e1) {
			return "STT:WOLWelcomeCustomMatch";
        }
		if (id == 0x6e2) {
			return "STT:WOLBuddy";
        }
		if (id == 0x6e3) {
			return "STT:WOLWelcomeWDT";
        }
		if (id == 0x686) {
			return "STT:WOLWelcomeBack";
        }
	} else if (kind == 0xe7) {
		if (id == 0x62e) {
			return "STT:WOLBeginNickMonth";
        }
		if (id == 0x62f) {
			return "STT:WOLBeginNickDay";
        }
		if (id == 0x630) {
			return "STT:WOLBeginNickYear";
        }
		if (id == 0x626) {
			return "STT:WOLBeginNickEmail";
        }
		if ((id == 0x631) || (id == 0x70a)) {
			return "STT:WOLBeginNickNewsletter";
        }
		if (id == 1) {
			return "STT:WOLBeginNickContinue";
        }
		if (id == 0x576) {
			return "STT:WOLBeginNickViewPrivacy";
        }
		if (id == 2) {
			return "STT:WOLBeginNickCancel";
        }
	} else if (kind == 0xe6 || kind == 0xf3 || kind == 0xf4) {
		if (id == 1) {
			return "STT:WOLNewNickOk";
        }
		if (id == 2) {
			return "STT:WOLNewNickCancel";
        }
		if (id == 0x628) {
			return "STT:WOLNewNickNickname";
        }
		if (id == 0x72c) {
			return "STT:WOLNewNickPassword";
        }
		if (id == 0x627) {
			return "STT:WOLNewNickVerify";
        }
		if (id == 0x5e5) {
			return "STT:WOLNewNickParentEMail";
        }
		if ((id == 0x66b) || (id == 0x70b)) {
			return "STT:WOLNewNickSellInfo";
        }
	} else if (kind == 0x125) {
		if (id == 0x686) {
			return "WOL:WDTFactionBack";
        }
		if (id != 0) {
			return "WDT:FactionSelectToolTip";
        }
	} else if (kind == 0xbc6) {
		if (((id == 0x411) || (id == 0x412)) || ((id == 0x413 || ((((id == 0x414 || (id == 0x415)) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))))))) {
			return "STT:MPScoreLabelPlayer";
        }
		if (((id == 0x419) || ((id == 0x41a || (id == 0x41b)))) || (((id == 0x41c || (((id == 0x41d || (id == 0x41e)) || (id == 0x41f)))) || (id == 0x420)))) {
			return "STT:MPScoreLabelKills";
        }
		if ((((id == 0x6a3) || (id == 0x6a4)) || ((id == 0x6a9 || ((id == 0x6ac || (id == 0x6af)))))) || ((id == 0x6b2 || ((id == 0x6b5 || (id == 0x6b8)))))) {
			return "STT:MPScoreLabelLosses";
        }
		if ((((id == 0x6a5) || (((id == 0x6a7 || (id == 0x6aa)) || (id == 0x6ad)))) || (((id == 0x6b0 || (id == 0x6b3)) || (id == 0x6b6)))) || (id == 0x6b9)) {
			return "STT:MPScoreLabelBuilt";
        }
		if (((id == 0x6a6) || (id == 0x6a8)) || ((id == 0x6ab || ((((id == 0x6ae || (id == 0x6b1)) || (id == 0x6b4)) || ((id == 0x6b7 || (id == 0x6ba)))))))) {
			return "STT:MPScoreLabelScore";
        }
		if (((id == 0x70a) || ((id == 0x70b || (id == 0x70c)))) || (((id == 0x70d || (((id == 0x70e || (id == 0x70f)) || (id == 0x710)))) || (id == 0x711)))) {
			return "STT:MPScoreLabelStatus";
        }
		if (id == 0x733) {
			return "STT:MPScoreLabelCampaignName";
        }
		if (id == 0x71b) {
			return "STT:MPScoreLabelMapName";
        }
		if (id == 0x732) {
			return "STT:MPScoreLabelDifficulty";
        }
		if (id == 0x6d1) {
			return "STT:MPCampScoreContinue";
        }
		if (id == 0x5c0) {
			return "STT:MPCampScoreQuit";
        }
		if (id == 0x3ea) {
			return "STT:MPScoreLabelTime";
        }
	} else if (kind == 0xbc7) {
		if (id == 0x733) {
			return "STT:MPScoreLabelCampaignName";
        }
		if (id == 0x735) {
			return "STT:MPCampEndCampaignNumber";
        }
		if (id == 0x732) {
			return "STT:MPScoreLabelDifficulty";
        }
		if (id == 0x3ea) {
			return "STT:MPCampEndTime";
        }
		if (((id == 0x411) || (id == 0x412)) || ((id == 0x73a || (id == 0x739)))) {
			return "STT:MPScoreLabelPlayer";
        }
		if ((id == 0x419) || (id == 0x41a)) {
			return "STT:MPScoreLabelKills";
        }
		if ((id == 0x6a3) || (id == 0x6a4)) {
			return "STT:MPScoreLabelLosses";
        }
		if ((id == 0x6a5) || (id == 0x6a7)) {
			return "STT:MPScoreLabelBuilt";
        }
		if ((((id == 0x6a6) || (id == 0x6a8)) || (id == 0x73b)) || (id == 0x738)) {
			return "STT:MPScoreLabelScore";
        }
		if (id == 0x6d1) {
			return "STT:MPScoreButtonContinue";
        }
    }
	return 0;
}

}


namespace OwnerDraw {

void __fastcall Set_Control_Disabled(HWND control, bool value)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsDisabled = value;
    }
}

}


namespace OwnerDraw {

bool __fastcall Is_Control_Disabled(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		return record->IsDisabled;
    }
	return false;
}

}


namespace OwnerDraw {


inline bool __cdecl Is_Option_Kind(int kind)
{
	return kind == 0xbc || kind == 0xbd || kind == 0x102 || kind == 0xc2 || kind == 0xc9;
}
inline int __cdecl Kind_Of(unsigned int key)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	return record != 0 ? record->Kind : 0;
}
inline bool __cdecl Is_Plain_Control(HWND window)
{
	if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) == BS_OWNERDRAW) {
		HWND control = window;
		WinData * record = 0;
		Control_Table.getPointer((unsigned int const &)control, &record);
		if (record != 0 && record->SkipTests == 0) {
			return true;
		}
	}
	return false;
}

#ifdef RA2_NON_MATCHING // not exact: 2216 bytes differ (adopted text; the image calls Is_Listed_Kind out of line, ours expands it: Is_Listed_Kind's || chain is under VC6's auto-inline size cap here; written as nine if groups returning unsigned int it stays exact and this row is 1 byte off, the image testing al where ours tests eax)
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

BOOL __stdcall Enable_Animated_Control(HWND window, LPARAM lparam)
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


void * __fastcall Get_Drawer();
void * __fastcall Get_Alternate_Drawer();
void * Get_Chat_Art();
Point2D __fastcall Point_Copy(Point2D const * source);
void __cdecl Audio_Update();

namespace OwnerDraw {

/*
**	RA2: starts the animation of every animated control of the dialog.
*/
void __fastcall Enable_Animated_Controls(HWND window)
{
	EnumChildWindows(window, Enable_Animated_Control, 1);
}

}


namespace OwnerDraw {

/*
**	RA2: stops the animation of every animated control of the dialog.
*/
void __fastcall Disable_Animated_Controls(HWND window)
{
	EnumChildWindows(window, Enable_Animated_Control, 0);
}

}


namespace OwnerDraw {


inline int __cdecl Visible_Count(HWND dialog)
{
	Visible_Free_Buttons = 0;
	EnumChildWindows(dialog, Count_Visible_Free_Buttons, 0);
	return Visible_Free_Buttons;
}

inline bool __cdecl Any_Visible(HWND dialog)
{
	Visible_Default_Buttons = 0;
	EnumChildWindows(dialog, Count_Visible_Default_Buttons, 0);
	return Visible_Default_Buttons >= 1;
}

inline int __cdecl Slide_Phase(int delta)
{
	if (delta < 0) return -1;
	if (delta >= 6) return -2;
	return delta;
}

inline int __cdecl Last_Step(int const * order, int pieces)
{
	int steps = 0;
	for (int j = 0; j < pieces; j++) {
		if (steps <= order[j]) steps = order[j];
    }
	return steps;
}

void __fastcall Slide_Dialog(HWND dialog, bool reverse)
{
	WinData * record = Get_Record(dialog);
	bool has_d7 = record ? record->IsChat : false;
	record = Get_Record(dialog);
	bool has_d6 = record ? record->IsLobbyFrame : false;
	record = Get_Record(dialog);
	bool has_d5 = record ? record->IsLobbyArt : false;

	ConvertClass * drawer = (ConvertClass *)Get_Drawer();
	ConvertClass * drawer_b = (ConvertClass *)Get_Alternate_Drawer();
	ShapeSet * shape_e8 = ArtSlideTop;
	ShapeSet * shape_b8 = Slide_Button_Shape;
	ShapeSet * shape_ec = ArtSlideMPButton;
	ShapeSet * shape_b4 = ArtSlideWarning;
	ConvertClass * drawer_c = (ConvertClass *)Get_Chat_Art();
	ShapeSet * shape_f0 = (ShapeSet *)ArtScoreTop;
	int count = SlideButtonCount;
	Rect window = AlternateSurface->Get_Rect();
	Rect rect_c4 = *SlideTopRect;
	Rect rect_b8 = *SlideMPButtonRect;
	Rect rect_bc = *SlideWarningRect;
	Rect rect_b4 = *SlideButtonRect;
	Rect rect_c0 = *DialogBackRect;

    RECT client;
	Get_Display_Rect(dialog, &client);
	RECT screen;
	GetWindowRect(dialog, &screen);
	int dx = screen.left - client.left;
	int dy = screen.top - client.top;

	int x = rect_b4.X;
	int y = rect_b4.Y;
	int width = rect_b4.Width;
	int height = rect_b4.Height * count;
	if (has_d5) {
		x = rect_bc.X;
		width = rect_bc.Width;
		y = rect_bc.Y;
		height += rect_b4.Y;
    }
	Rect from(x, y, width, height);
	Rect to = from;
	to.X += dx;
	to.Y += dy;
	Rect from_c0 = rect_c0;
	Rect to_c0 = from_c0;
	to_c0.X += dx;
	to_c0.Y += dy;

	int dir = reverse ? -1 : 1;
	int base_64 = 5;
	if (reverse) base_64 = 10;
	int base_8c = 11;
	if (reverse) base_8c = 16;
	int base_94 = 1;
	if (reverse) base_94 = 6;
	int base_88 = 0;
	if (reverse) base_88 = 5;
	int base_90 = 0;
	if (reverse) base_90 = 5;

	int buttons = Visible_Count(dialog);
	int last = count - 1;
	int top = rect_b4.Y;
	if (has_d6) top += rect_b4.Height;
	bool any = Any_Visible(dialog);
	if (!any) last++;

	int pieces = count + 3;
	int limit = pieces - 2;
	int * order = new int[pieces];
	int total = count + 3;
	int value = 1;
	for (int i = 0; i < limit; i++) {
		order[i] = value++;
    }
	order[total - 1] = 0;
	order[limit] = value + 1;
	order[pieces - 3] = 0;
	int steps = Last_Step(order, pieces) + 6;

	for (int step = 0; step < steps; step++) {
		if (has_d6) {
			Point2D point = Point_Copy((Point2D const *)&rect_b8);
			int phase = Slide_Phase(step - order[pieces - 3]);
			if (phase == -1) {
				Draw_Shape(*AlternateSurface, *drawer, shape_ec, reverse ? 6 : 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
			} else if (phase == -2) {
				if (!reverse) {
					Draw_Shape(*AlternateSurface, *drawer, shape_ec, 6, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
                }
            }
        }
		if (has_d5) {
			Point2D point = Point_Copy((Point2D const *)&rect_bc);
			int phase = Slide_Phase(step - order[pieces - 2]);
			if (phase == -1) {
				Draw_Shape(*AlternateSurface, *drawer, shape_e8, !reverse, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
			} else if (phase == -2) {
				Draw_Shape(*AlternateSurface, *drawer, shape_e8, reverse != 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            } else {
				point = Point_Copy((Point2D const *)&rect_c4);
				Draw_Shape(*AlternateSurface, *drawer, shape_e8, 1, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
				point = Point_Copy((Point2D const *)&rect_bc);
				Draw_Shape(*AlternateSurface, *drawer, shape_b4, phase * dir + base_88, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            }
        }
		if (has_d6) {
			Point2D point = Point_Copy((Point2D const *)&rect_b8);
			int phase = Slide_Phase(step - order[pieces - 3]);
			if (phase == -2) {
				if (reverse) {
					Draw_Shape(*AlternateSurface, *drawer, shape_ec, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
                }
            } else {
				Draw_Shape(*AlternateSurface, *drawer, shape_ec, phase * dir + base_94, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            }
        }
		int k;
		for (k = 0; k < buttons; k++) {
			Point2D point(rect_b4.X, k * rect_b4.Height + top);
			int phase = Slide_Phase(step - order[k]);
			if (phase == -1) {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 10 : 1, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
			} else if (phase == -2) {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 1 : 10, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            } else {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, phase * dir + base_64, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            }
        }
		for (k = buttons; k < last; k++) {
			Point2D point(rect_b4.X, k * rect_b4.Height + top);
			int phase = Slide_Phase(step - order[k]);
			if (phase == -1) {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 10 : 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
			} else if (phase == -2) {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 0 : 10, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            } else {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, phase * dir + base_8c, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            }
        }
		if (any) {
			Point2D point(rect_b4.X, Button_Strip_Bottom->Y - rect_b4.Height);
			int phase = Slide_Phase(step - order[count - 1]);
			if (phase == -1) {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 10 : 1, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
			} else if (phase == -2) {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, reverse ? 1 : 10, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            } else {
				Draw_Shape(*AlternateSurface, *drawer_b, shape_b8, phase * dir + base_64, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            }
        }
		if (has_d7) {
			Point2D point = Point_Copy((Point2D const *)&rect_c0);
			int phase = Slide_Phase(step - order[total - 1]);
			if (phase == -1) {
				if (!reverse) {
					if (VideoModeWidth >= Base_Screen_Width) point.X += 0x50;
                }
				Draw_Shape(*AlternateSurface, *drawer_c, (reverse ? Art_Chat_Back : shape_f0), 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
			} else if (phase == -2) {
				if (reverse) {
					if (VideoModeWidth >= Base_Screen_Width) point.X += 0x50;
                }
				Draw_Shape(*AlternateSurface, *drawer_c, (reverse ? shape_f0 : Art_Chat_Back), 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            } else {
				if (VideoModeWidth >= Base_Screen_Width) point.X += 0x50;
				Draw_Shape(*AlternateSurface, *drawer_c, shape_f0, phase * dir + base_90, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
            }
        }
        VisibleSurface->Lock();
        AlternateSurface->Lock();
		VisibleSurface->Blit_From(to, *AlternateSurface, from, false, true);
		if (has_d7) {
			VisibleSurface->Blit_From(to_c0, *AlternateSurface, from_c0, false, true);
        }
        AlternateSurface->Unlock();
        VisibleSurface->Unlock();
		Audio_Update();
		Sleep(30);
    }
	delete[] order;
	if (reverse) {
		VocClass::PlayGlobal(Rule->ShellButtonSlideSound, 1.0f, 0x2000, 0);
		while (MouseCursor->Get_Mouse_State()) {
			MouseCursor->Show_Mouse();
        }
		SendMessageA(dialog, 0x4ec, 0, 0);
    } else {
		SendMessageA(dialog, 0x4ed, 0, 0);
    }
}

}


void __fastcall Slide_Pending_Dialog(HWND window)
{
	OwnerDraw::WinData * record = OwnerDraw::Get_Record(window); // RA2: ra2-decomp's Find_Data (Yuri's Revenge 0x00607FD0)
	if (record && record->IsNotifyPending) {
		HWND item = GetDlgItem(window, 0x71a);
		if (item) {
			SendMessageA(item, 0x4e2, 0, 0);
        }
		OwnerDraw::Slide_Dialog(window, false);
		record->IsNotifyPending = false;
    }
}


namespace OwnerDraw {

bool __fastcall WS_Save_Dialog_Values(HWND dialog)
{
	if (Session.Is_In_Game()) {
		return false;
    }
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)dialog, &record);
	if (!record) {
		return false;
    }
	bool active = record->IsHidden && record->ButtonKind == 1;
	bool visible = IsWindowVisible(dialog) != 0;
	if (!active || !visible) {
		return false;
    }
	VocClass::PlayGlobal(Rule->GUIMoveOutSound, 1.0f, 0x2000, 0);
	bool enabled = IsWindowEnabled(dialog) != 0;
	EnableWindow(dialog, FALSE);
	EnumChildWindows(dialog, Enable_Animated_Control, 1);
	record = 0;
	Control_Table.getPointer((unsigned int)dialog, &record);
	if (!record) {
		return false;
    }
	record->IsNotifyPending = true;
	InvalidateRect(dialog, 0, FALSE);
	DWORD start = GetTickCount();
	while (!Dialog_Message_Handler()) {
		int done = !record->IsNotifyPending;
		if ((int)(GetTickCount() - start) > 5000 || done) {
            break;
        }
    }
	EnumChildWindows(dialog, Enable_Animated_Control, 0);
	EnableWindow(dialog, enabled);
	return true;
}

}


namespace OwnerDraw {

bool __fastcall Play_Dialog_Move_In(HWND dialog)
{
	if (Session.Is_In_Game()) {
		return false;
    }
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)dialog, &record);
	if (!record) {
		return false;
    }
	bool active = record->IsHidden && record->ButtonKind == 1;
	bool visible = IsWindowVisible(dialog) != 0;
	if (!active || !visible) {
		return false;
    }
	VocClass::PlayGlobal(Rule->GUIMoveInSound, 1.0f, 0x2000, 0);
	bool enabled = IsWindowEnabled(dialog) != 0;
	EnableWindow(dialog, FALSE);
	EnumChildWindows(dialog, Enable_Animated_Control, 1);
	Slide_Dialog(dialog, true);
	EnumChildWindows(dialog, Enable_Animated_Control, 0);
	EnableWindow(dialog, enabled);
	InvalidateRect(dialog, 0, FALSE);
	return true;
}

}


namespace OwnerDraw {

void __fastcall Set_Dialog_Hidden(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsHidden = true;
    }
}

}


namespace OwnerDraw {

void __fastcall Clear_Dialog_Hidden(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsHidden = false;
    }
}

}


namespace OwnerDraw {

void __fastcall Set_Dialog_Waiting(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsWaiting = true;
    }
}

}


namespace OwnerDraw {

void __fastcall Clear_Dialog_Waiting(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsWaiting = false;
    }
}

}


namespace OwnerDraw {
bool __fastcall Select_Control_Rect(HWND dialog, HWND control, Rect * bounds)
{
	WinData * record = Get_Record(dialog);
	int kind = record ? record->Kind : 0;
	int id = GetDlgCtrlID(control);
	Rect * selected = 0;
	if (kind == 0x94) {
		if (VideoModeWidth < MedResWidth) return false;
		if (id == 0x6ea) selected = new Rect(0x1e, 0x1a, 0x23a, 0x87);
		else if (id == 0x6eb) selected = new Rect(0x52, 0xbb, 0x1d4, 0x6c);
		else if (id == 0x6ec) selected = new Rect(0x62, 0x12a, 0x1bc, 0x95);
		else if (id == 0x50f) selected = new Rect(0xb1, 0x1fd, 0x110, 0x15);
		else if (id == 0x71e) selected = new Rect(0xb1, 0x1dc, 0x8d, 0x14);
		else if (id == 0x670) selected = new Rect(0x135, 0x1dc, 0x8d, 0x14);
		else return false;
    }
    else if (kind == 0x103) {
		if (id != 0x72b) return false;
		if (VideoModeWidth >= MedResWidth) {
			if (Scen->IsGDI) selected = new Rect(0x68, 0x8e, 0x1a8, 0xdc);
			else selected = new Rect(0x67, 0x82, 0x1a8, 0xe6);
        } else {
			if (Scen->IsGDI) selected = new Rect(0x18, 0x8e, 0x1a8, 0xdc);
			else selected = new Rect(0x17, 0x82, 0x1a8, 0xe6);
        }
    }
    else if (kind == 0xbc7) {
		if (id != 0x72b) return false;
		if (VideoModeWidth >= MedResWidth) {
			if (Scen->IsGDI) selected = new Rect(0x68, 0x8e, 0x1a8, 0xdc);
			else selected = new Rect(0x67, 0x82, 0x1a8, 0xe6);
        } else {
			if (Scen->IsGDI) selected = new Rect(0x18, 0x8e, 0x1a8, 0xdc);
			else selected = new Rect(0x17, 0x82, 0x1a8, 0xe6);
        }
    }
    else if (kind == 0xbc) {
		if (id != 0x72f) return false;
		if (VideoModeWidth == LowResWidth) selected = new Rect(0x34, 0x4e, 0x18c, 0x8f);
		else if (VideoModeWidth == MedResWidth || VideoModeWidth == HighResWidth) selected = new Rect(0x84, 0x8a, 0x18c, 0x8f);
		else return false;
    }
    else if (kind == 0xbd) {
		if (id != 0x72f) return false;
		if (VideoModeWidth == LowResWidth) selected = new Rect(0x34, 0x4e, 0x18c, 0x8f);
		else if (VideoModeWidth == MedResWidth || VideoModeWidth == HighResWidth) selected = new Rect(0x84, 0x8a, 0x18c, 0x8f);
		else return false;
    }
    else if (kind == 0xc2) {
		if (id != 0x72f) return false;
		if (VideoModeWidth == LowResWidth) selected = new Rect(0x34, 0x4e, 0x18c, 0x8f);
		else if (VideoModeWidth == MedResWidth || VideoModeWidth == HighResWidth) selected = new Rect(0x84, 0x8a, 0x18c, 0x8f);
		else return false;
    }
    else if (kind == 0xc9) {
		if (id != 0x72f) return false;
		if (VideoModeWidth == LowResWidth) selected = new Rect(0x34, 0x4e, 0x18c, 0x8f);
		else if (VideoModeWidth == MedResWidth || VideoModeWidth == HighResWidth) selected = new Rect(0x84, 0x8a, 0x18c, 0x8f);
		else return false;
    }
    else return false;
	if (!selected) return false;
    *bounds = *selected;
	delete selected;
	return true;
}
}


bool __fastcall Is_Owner_Tested_Control(unsigned int key, HWND window)
{
	OwnerDraw::WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (((((((((kind == 0x10c) || (kind == 0x94)) || (kind == 0xa3)) ||
				(((kind == 0x73 || (kind == 0xd8)) ||
					((kind == 0xbbb || ((kind == 0xf5 || (kind == 0x105)))))))) ||
			   ((kind == 0xe2 || (((kind == 0xd5 || (kind == 0x2b5)) || (kind == 0xb7)))))) ||
			  (((kind == 0x2b4 || (kind == 0xbc7)) ||
				  (((kind == 0x101 || ((kind == 0x129 || (kind == 0xbc6)))) || (kind == 0xd7)))))) ||
			 ((((((((kind == 0x108 || (kind == 0xbc)) || (kind == 0xbd)) ||
					  ((kind == 0xbb || (kind == 0xb5)))) ||
					 ((kind == 0xbba || ((kind == 0xff || (kind == 0x6b)))))) || (kind == 0x100)) ||
				   ((((kind == 0x102 || (kind == 0xb8)) || (kind == 0xd6)) ||
					   (((kind == 0x103 || (kind == 0x125)) ||
						   ((kind == 0xe7 || ((kind == 0x116 || (kind == 0xc2)))))))))) || (kind == 0xc9)))) ||
			(((((kind == 0x11d || (kind == 0x11c)) || (kind == 0x114)) ||
				  ((kind == 700 || (kind == 0x113)))) ||
				(((kind == 0x109 || ((kind == 0x10f || (kind == 0x117)))) ||
					((kind == 0xe6 || (((kind == 0xf3 || (kind == 0xf4)) || (kind == 0x10e)))))))))) &&
		(id == 0x694)) {
		return true;
    }
	if ((((((kind == 0x94) || (kind == 0xd8)) ||
			  ((kind == 0xf5 || ((kind == 0xe2 || (kind == 0xd5)))))) ||
			 ((kind == 0x101 ||
				 (((((kind == 0x129 || (kind == 0xd7)) || (kind == 0xbb)) ||
					   ((kind == 0x100 || (kind == 0xd6)))) || (kind == 0x125)))))) ||
			(((((kind == 0x122 || (kind == 0x112)) ||
				   ((kind == 0xe7 || (((kind == 0x116 || (kind == 0x11d)) || (kind == 0x11c)))))) ||
				  (((kind == 0xfe || (kind == 0x10f)) || (kind == 0x117)))) ||
				(((kind == 0x114 || (kind == 0xe6)) ||
					(((kind == 0xf3 || (((kind == 0xf4 || (kind == 700)) || (kind == 0x10e)))) ||
						((kind == 0xbc6 || (kind == 0xbc7)))))))))) && (id == 0x71c)) {
		return true;
    }
	if ((((kind == 0xbc) || (kind == 0xbd)) ||
			((kind == 0x102 ||
				((((kind == 0xc2 || (kind == 0xc9)) || (kind == 0x105)) ||
					((kind == 0x6b || (kind == 0x113)))))))) && (id == 0x468)) {
		return true;
    }
	if (kind == 0xe2) {
		return id == 0x686 || id == 0x578 || id == 0x55c || id == 0x683 || id == 0x684;
    }
	if (kind == 0x101) {
		return id == 0x68e || id == 0x68d || id == 0x68f;
    }
	if (kind == 0x129) {
		return id == 0x745;
    }
	if (kind == 0x100) {
		return id == 0x689 || id == 0x688 || id == 0x579;
    }
	if (kind == 0x94) {
		return id == 0x40e;
    }
	if (kind == 0xb7) {
		return id == 0x40f;
    }
	if (kind == 0x2b4) {
		return id == 0x6c7;
    }
	if (kind == 0x2b5) {
		return id == 0x6c8;
    }
	if (kind == 0x105) {
		return id == 0x6c4 || id == 0x6c2 || id == 0x6c3 || id == 0x6c5;
    }
	if (kind == 0x102) {
		return id == 0x6ec || id == 0x5aa || id == 0x5a8 || id == 0x617;
    }
	if (kind == 0xb5) {
		return id == 0x522 || id == 0x521 || id == 0x51e || id == 0x51f || id == 0x520;
    }
	if (kind == 0xbba) {
		return id == 0x522 || id == 0x521;
    }
	if (kind == 0xbbb) {
		return id == 0x52c || id == 0x52d;
    }
	if (kind == 0xd5) {
		return id == 0x5ce || id == 0x5cd;
    }
	if (kind == 0xfb) {
		return id == 6 || id == 1;
    }
	if (kind == 0xd8) {
		return id == 0x6cb;
    }
	if (kind == 0xd7) {
		return id == 1;
    }
	if (kind == 0xbc || kind == 0xc2) {
		return id == 0x6ec || id == 0x588 || id == 0x5aa || id == 0x5a8;
    }
	if (kind == 0xbd || kind == 0xc9) {
		return id == 0x59f || id == 0x6ec || id == 0x5a8;
    }
	if (kind == 0xbb) {
		return id == 0x6cd || id == 0x6cc || id == 0x6ce;
    }
	if (kind == 0xb6) {
		return id == 0x6c9 || id == 0x712 || id == 0x524;
    }
	if (kind == 0x6b) {
		return id == 0x583 || id == 0x6c5;
    }
	if (kind == 0xbc6) {
		return id == 0x6d1;
    }
	if (kind == 0xbc7) {
		return id == 0x6d1;
    }
	if (kind == 0x10f || kind == 0x11d) {
		return id == 0x539 || id == 0x6ea || id == 0x6ec || id == 0x6eb;
    }
	if (kind == 0x109) {
		return id == 1 || id == 0x625 || id == 0x53b;
    }
	if (kind == 0x10e || kind == 0x11c) {
		return id == 0x6e0 || id == 0x6e1 || id == 0x6e2 || id == 0x6e4 || id == 0x6e3;
    }
	if (kind == 0x114) {
		return id == 0x687 || id == 0x6ea || id == 0x6ec || id == 0x6eb || id == 0x688;
    }
	if (kind == 0x113) {
		return id == 0x61f || id == 0x558 || id == 0x689 || id == 0x62d || id == 0x62b || id == 0x53d;
    }
	if (kind == 700) {
		return id == 1 || id == 0x63a;
    }
	if (kind == 0x116) {
		return id == 0x62b || id == 0x62d || id == 0x5c2 || id == 0x702 || id == 0x703;
    }
	if (kind == 0xff) {
		return id == 0x521 || id == 0x522;
    }
	if (kind == 0x122) {
		return id == 1 || id == 2;
    }
	if (kind == 0x112) {
		return id == 1;
    }
	if (kind == 0xe7) {
		return id == 1 || id == 0x576;
    }
	if (kind == 0xfe) {
		return id == 1;
    }
	if (kind == 0xe6 || kind == 0xf3 || kind == 0xf4) {
		return id == 1;
    }
	return false;
}


namespace OwnerDraw {

bool __fastcall Is_Free_Owner_Button(unsigned int key, HWND window)
{
	if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) != BS_OWNERDRAW) {
		return false;
    }
	HWND control = window;
	WinData * record = 0;
	Control_Table.getPointer((unsigned int const &)control, &record);
	if (record && record->SkipTests == 0) {
		return Is_Owner_Tested_Control(key, window);
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Is_Default_Button(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (kind == 0xe2) {
		return id == 0x3ee;
    }
	if (((kind == 0x73) || (kind == 0x10c)) || (kind == 0x103)) {
		return id == 0x423;
    }
	if (kind == 0x108) {
		return id == 0x6d1;
    }
	if ((((kind != 0x94) && (kind != 0xb6)) &&
			((kind != 0xa3 && ((kind != 0xd8 && (kind != 0xbbb)))))) &&
		((((kind != 0xf5 &&
			   ((((kind != 0xd5 && (kind != 0x2b5)) && (kind != 0xb7)) &&
				   ((kind != 0x2b4 && (kind != 0x101)))))) &&
			  ((((kind != 0x129 && ((kind != 0xb5 && (kind != 0xbba)))) && (kind != 0x100)) &&
				  (((((kind != 0xb8 && (kind != 0xd6)) && (kind != 0x125)) &&
						((kind != 0x116 && (kind != 0x11d)))) && (kind != 0x11c)))))) &&
			(((kind != 0x113 && (kind != 0x109)) &&
				((kind != 0x10f && ((kind != 0x114 && (kind != 0x10e)))))))))) {
		if ((kind == 0x102) ||
			((((kind == 0x105 || (kind == 0x6b)) || (kind == 0xbb)) ||
				((kind == 0x117 || (kind == 0xbc6)))))) {
			return id == 0x5c0;
        }
		if (((((kind == 0xbc) || (kind == 0xbd)) ||
				 ((kind == 0xea || (((kind == 0xd7 || (kind == 0xc2)) || (kind == 0xc9)))))) ||
				((kind == 700 || (kind == 0xe7)))) ||
			((kind == 0xe6 ||
				(((kind == 0xf3 || (kind == 0xf4)) ||
					((kind == 0x122 || (((kind == 0x112 || (kind == 0xfe)) || (kind == 0xfc)))))))))) {
			return id == 2;
        }
		if (kind == 0xd4) {
			return id == 1;
        }
		if (kind == 0xfb) {
			return id == 7;
        }
		if (kind == 0xff) {
			return id == 0x675;
        }
		return false;
    }
	return id == 0x686;
}

}


namespace OwnerDraw {

bool __fastcall Is_Owner_Default_Button(unsigned int key, HWND window)
{
	if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) != BS_OWNERDRAW) {
		return false;
    }
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)window, &record);
	if (record && record->SkipTests == 0) {
		return Is_Default_Button(key, window);
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Is_Flag_Control(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (Kind_Test(kind) && id == 0x695) {
		return true;
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Is_Campaign_Button(unsigned int key, HWND window)
{
	if (Session.Is_In_Game()) {
		return false;
    }
	if (Is_Free_Owner_Button(key, window)) {
		return true;
    }
	if (Is_Plain_Control(window) && Is_Default_Button(key, window)) {
		return true;
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Is_Network_Button(unsigned int key, HWND window)
{
	if (!Session.Is_In_Game()) {
		return false;
    }
	if (Is_Free_Owner_Button(key, window)) {
		return true;
    }
	if (Is_Plain_Control(window) && Is_Default_Button(key, window)) {
		return true;
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Is_Cancel_Button(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (kind == 0x10d) {
		return id == 0x5c1 || id == 0x5c0;
    }
	if (kind == 0xd9) {
		return id == 0x5c1 || id == 0x6ca;
    }
	if (kind == 0xa3) {
		return id == 0x4cf || id == 0x4d0;
    }
	if (kind == 0x105) {
		return id == 0x620 || id == 0x621;
    }
	if (kind == 0xf0) {
		return id == 0x5c0;
    }
	if (kind == 0xce) {
		return id == 0x5ae;
    }
	if (kind == 0x120) {
		return id == 0x5ae || id == 2;
    }
	if (kind == 0x121) {
		return id == 0x5ae || id == 0x5af || id == 2;
    }
	if (kind == 0xea) {
		return id == 0x640 || id == 0x642 || id == 0x641 || id == 0x643 || id == 0x644 || id == 0x645 || id == 0x646 || id == 0x647;
    }
	if (kind == 0xb8) {
		return id == 0x531 || id == 0x535;
    }
	if (kind == 0x11f) {
		return id == 0x5c0;
    }
	if (kind == 0x115) {
		return id == 0x702 || id == 0x686;
    }
	if (kind == 0xd3) {
		return id == 2;
    }
	if (kind == 0xcf) {
		return id == 0x5c2 || id == 2;
    }
	if (kind == 0xc3) {
		return id == 1 || id == 2;
    }
	if (kind == 0x11b) {
		return id == 0x5c2 || id == 0x686;
    }
	if (kind == 0xe1) {
		return id == 1 || id == 2;
    }
	if (kind == 0x11e) {
		return id == 0x5c2 || id == 0x686;
    }
	if (kind == 0xc4) {
		return id == 0x58d || id == 0x58e;
    }
	if (kind == 0xd0) {
		return id == 1;
    }
	if (kind == 0xfc) {
		return id == 1 || id == 2;
    }
	if (kind == 0x126) {
		return id == 6 || id == 7;
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Is_Main_Menu_Movie(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (kind == 0xe2) {
		return id == 0x71d;
    }
	return false;
}

}


namespace OwnerDraw {

BOOL __stdcall Count_Visible_Free_Buttons(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
	if (Is_Free_Owner_Button((unsigned int)parent, window) && IsWindowVisible(window)) {
		Visible_Free_Buttons++;
    }
	return TRUE;
}

}


unsigned int __fastcall Count_Free_Buttons(HWND window)
{
	Visible_Free_Buttons = 0;
	EnumChildWindows(window, Count_Visible_Free_Buttons, 0);
	return Visible_Free_Buttons;
}


namespace OwnerDraw {

BOOL __stdcall Count_Visible_Default_Buttons(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
	if (Is_Owner_Default_Button((unsigned int)parent, window) && IsWindowVisible(window)) {
		Visible_Default_Buttons++;
    }
	return TRUE;
}

}


namespace OwnerDraw {

bool __fastcall Has_Visible_Default_Button(HWND dialog)
{
	Visible_Default_Buttons = 0;
	EnumChildWindows(dialog, Count_Visible_Default_Buttons, 0);
	return Visible_Default_Buttons >= 1;
}

}


namespace OwnerDraw {

BOOL __stdcall Assign_Button_Kind(HWND window, LPARAM)
{
	WinData * record = Get_Record(window);
	if (record != 0) {
        HWND parent = GetParent(window);
		if (Is_Campaign_Button((unsigned int)parent, window)) {
			record->ButtonKind = 1;
			return TRUE;
        }
		if (Is_Network_Button((unsigned int)parent, window)) {
			record->ButtonKind = 2;
			return TRUE;
        }
		if (Is_Cancel_Button((unsigned int)parent, window)) {
			record->ButtonKind = 3;
        }
    }
	return TRUE;
}

}


namespace OwnerDraw {

/*
**	RA2: sorts every control of the dialog into its button kind.
*/
void __fastcall Assign_Button_Kinds(HWND window)
{
	EnumChildWindows(window, Assign_Button_Kind, 0);
}

}


namespace OwnerDraw {


struct ControlAnimationState {
		void const * Shape;
		bool IsShapeAllocated;
		int IsTestedControl;
		int ControlHeight;
		int ControlKind;
		int ControlFlags;
		int Reserved_90;
		int FrameCount;
		int Frame;
		int LastTick;
		int TimerInterval;
		int Reserved_A4;
		bool IsPlaying;
};
static inline ControlAnimationState * __cdecl Animation(WinData * record)
{
	return (ControlAnimationState *)record->Animation;
}

static inline void __cdecl Get_Animation_Record(unsigned int const & key, WinData *& record)
{
	Control_Table.getPointer(key, &record);
}
#ifdef RA2_NON_MATCHING // not exact: 1049 bytes differ by position against the image-derived bound 005e8130 (1,110 B; ours 896 B): the call list differs. The image's list is reached (995 by position, 1,072 B; registers then differ: zero in ebp, the parent in edi and GetDlgCtrlID through memory in the image) with the record through Get_Record, the animation fields as WinData members, the six helpers' lookups through Get_Record and Control_Animation_Art at 174 units; held, because the member declarations and the helper text turn count-sensitive rows (lane F57, private/dossiers/fable/patches/f57_ownrdraw_init_control_animation_call_list.patch); main: best 49.20
BOOL __stdcall Init_Control_Animation(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
	unsigned int key = (unsigned int)window;
	WinData * record = 0;
	Get_Animation_Record(key, record);
	if (record == 0) {
		return TRUE;
    }
	if (Is_Tested_Control((unsigned int)parent, window)) {
		record->PictureMode = 1;
		Animation(record)->IsPlaying = false;
		Animation(record)->IsTestedControl = 1;
		Animation(record)->ControlHeight = OwnerDraw_Control_Height((unsigned int)parent, window);
		Animation(record)->ControlKind = OwnerDraw_Control_Kind((unsigned int)parent, window);
		Animation(record)->ControlFlags = OwnerDraw_Control_Flags((unsigned int)parent, window);
		record->TypingSound = Control_Typing_Sound((unsigned int)parent, window);
		return TRUE;
    }
	bool animated = false;
	if (Is_Image_Control((unsigned int)parent, window)) {
		record->PictureMode = 2;
		if (record->Image == 0) {
			record->Image = (Surface *)Control_Image((unsigned int)parent, window);
        }
    } else {
		if (Is_Image_Mode3_Control((unsigned int)parent, window)) {
			record->PictureMode = 3;
		} else if (Is_Animated_Control((unsigned int)parent, window)) {
			record->PictureMode = 4;
			animated = true;
			if (Animation(record)->Shape == 0) {
				bool allocated = false;
				void * art = Control_Animation_Art((unsigned int)parent, window);
				Animation(record)->Shape = Control_Animation_Shape((unsigned int)parent, window, allocated);
				record->Drawer = (int)art;
				Animation(record)->IsShapeAllocated = allocated;
            }
        } else {
			goto done;
        }
    }
	Animation(record)->Frame = 0;
	Animation(record)->LastTick = GetTickCount();
	Animation(record)->TimerInterval = Control_Animation_Loops((unsigned int)parent, window);
done:
	if (Animation(record)->TimerInterval < 1) {
		if (animated && Animation(record)->Shape) {
			Animation(record)->FrameCount = ((short const *)Animation(record)->Shape)[3];
			Animation(record)->IsPlaying = false;
        }
    } else {
		SetTimer(window, 0, Animation(record)->TimerInterval, 0);
		if (animated) {
			Animation(record)->IsPlaying = true;
        }
    }
	return TRUE;
}
#endif

}


namespace OwnerDraw {

bool __fastcall Is_Listed_Dialog_Kind(int kind)
{
	return kind == 0x94 ||
		kind == 0xd8 ||
		kind == 0xf5 ||
		kind == 0xe2 ||
		kind == 0xd5 ||
		kind == 0x101 ||
		kind == 0x129 ||
		kind == 0xd7 ||
		kind == 0xbb ||
		kind == 0x100 ||
		kind == 0xd6 ||
		kind == 0x125 ||
		kind == 0x122 ||
		kind == 0x112 ||
		kind == 0xe7 ||
		kind == 0x116 ||
		kind == 0x11d ||
		kind == 0x11c ||
		kind == 0xfe ||
		kind == 0x10f ||
		kind == 0x117 ||
		kind == 0x114 ||
		kind == 0xe6 ||
		kind == 0xf3 ||
		kind == 0xf4 ||
		kind == 700 ||
		kind == 0x10e ||
		kind == 0xbc6 ||
		kind == 0xbc7;
}

}


namespace OwnerDraw {

/*
**	RA2: sets up the animation of every control of the dialog.
*/
void __fastcall Init_Control_Animations(HWND window)
{
	EnumChildWindows(window, Init_Control_Animation, 0);
}

}


namespace OwnerDraw {

BOOL __stdcall Reset_Tested_Control(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
	if (Is_Tested_Control((unsigned int)parent, window)) {
		SendMessageA(window, 0x4ee, 0, 0);
    }
	return TRUE;
}

}


namespace OwnerDraw {

/*
**	RA2: resets every tested control of the dialog.
*/
}

void __fastcall Dialog_On_OD_4EC(HWND window)
{
    using namespace OwnerDraw;
	EnumChildWindows(window, Reset_Tested_Control, 0);
}

namespace OwnerDraw {

}


namespace OwnerDraw {

BOOL __stdcall Assign_Control_Sounds(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
	WinData * record = Get_Record(window);
	if (record) {
		record->SoundSlot = Control_Sound_Slot(parent, window);
		record->TextStyle = Control_Text_Style(parent, window);
    }
	return TRUE;
}

}


void __fastcall Assign_All_Control_Sounds(HWND window)
{
	EnumChildWindows(window, Assign_Control_Sounds, 0);
}


namespace OwnerDraw {

void __fastcall Move_Control_To_Rect(HWND window, Rect const & rect)
{
    HWND parent = GetParent(window);
	unsigned char flag = Session.Is_In_Game();
    RECT parent_rect;
	GetWindowRect(parent, &parent_rect);
	Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int offset = 0;
	if (!flag) {
		offset = (((area.Width - Base_Screen_Width) / 2) < 0 ? 0 : ((area.Width - Base_Screen_Width) / 2));
    }
	Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int shift = 0;
	if (!flag) {
		shift = (((area2.Height - Base_Screen_Height) / 2) < 0 ? 0 : ((area2.Height - Base_Screen_Height) / 2));
    }
	MoveWindow(window, rect.X + offset, rect.Y + shift, rect.Width, rect.Height, FALSE);
}

}


#pragma intrinsic(abs)
namespace OwnerDraw {

void __fastcall Move_Free_Button(HWND window, Point2D const & size)
{
    HWND parent = GetParent(window);
	unsigned char flag = Session.Is_In_Game();
    RECT parent_rect;
    RECT rect;
	GetWindowRect(parent, &parent_rect);
	GetWindowRect(window, &rect);
	Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int offset = 0;
	if (!flag) {
		offset = (area.Width - Base_Screen_Width) / 2;
		offset = offset < 0 ? 0 : offset;
    }
	Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int shift = 0;
	if (!flag) {
		shift = (((area2.Height - Base_Screen_Height) / 2) < 0 ? 0 : ((area2.Height - Base_Screen_Height) / 2));
		shift -= (((size.Y - Base_Screen_Height) / 2) < 0 ? 0 : ((size.Y - Base_Screen_Height) / 2));
    }
    int x;
    int y;
	int width;
	int height;
	if (flag) {
		width = Button_Strip_Shape->Get_Width();
		height = Button_Strip_Shape->Get_Height();
		Rect * strip = Free_Button_Strip;
		x = parent_rect.right - offset - parent_rect.left - 0x93;
		HWND dialog = GetParent(window);
		RECT parent2;
        RECT rect2;
		GetWindowRect(dialog, &parent2);
        GetWindowRect(window, &rect2);
		int top = rect2.top - parent2.top;
		y = ((rect2.bottom - parent2.top - top) / 2 - 0xc6 + top) / 0x2c * height + strip->Y;
    } else {
		width = Slide_Button_Shape->Get_Width();
		height = Slide_Button_Shape->Get_Height();
		Rect * strip = Button_Strip_Top;
		x = parent_rect.right - offset - parent_rect.left - 0x9c;
		int top = strip->Y;
		int pos = rect.top - top - parent_rect.top + shift;
		int row = pos / strip->Height;
		int prod = strip->Height * row;
		int up = abs(strip->Height - pos + prod);
		int down = abs(pos - prod);
		y = (down < up ? row : row + 1) * strip->Height + top;
    }
	MoveWindow(window, x, y, width, height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall Move_Tested_Control(HWND window, Point2D const & size)
{
    HWND parent = GetParent(window);
	unsigned char flag = Session.Is_In_Game();
    RECT parent_rect;
    RECT rect;
	GetWindowRect(parent, &parent_rect);
	GetWindowRect(window, &rect);
	Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int offset = 0;
	if (!flag) {
		int excess = (area.Width - Base_Screen_Width) / 2;
		offset = excess < 0 ? 0 : excess;
    }
	Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int shift = 0;
	if (!flag) {
		shift = (((area2.Height - Base_Screen_Height) / 2) < 0 ? 0 : ((area2.Height - Base_Screen_Height) / 2));
		shift -= (((size.Y - Base_Screen_Height) / 2) < 0 ? 0 : ((size.Y - Base_Screen_Height) / 2));
    }
	Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	MoveWindow(window, parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left, rect.top - parent_rect.top + shift, box.Width, box.Height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall Move_Default_Button_Owner(HWND window)
{
    HWND parent = GetParent(window);
	unsigned char flag = Session.Is_In_Game();
    RECT parent_rect;
    RECT rect;
	GetWindowRect(parent, &parent_rect);
	GetWindowRect(window, &rect);
	Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int offset = Sidebar_Inset(area.Width, flag);
    int x;
    int y;
	int width;
	int height;
	if (flag) {
		x = parent_rect.right - offset - parent_rect.left - 0x93;
		width = Button_Strip_Shape->Get_Width();
		height = Button_Strip_Shape->Get_Height();
		y = Default_Button_Y->Y - height;
    } else {
		width = Slide_Button_Shape->Get_Width();
		x = parent_rect.right - offset - parent_rect.left - 0x9c;
		height = Slide_Button_Shape->Get_Height();
		int top = Button_Strip_Top->Y;
		int cell = Button_Strip_Top->Height;
		y = ((Button_Strip_Bottom->Y - top) / cell - 1) * cell + top;
    }
	MoveWindow(window, x, y, width, height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall Move_Default_Button(HWND window)
{
    HWND parent = GetParent(window);
	unsigned char flag = Session.Is_In_Game();
    RECT parent_rect;
    RECT rect;
	GetClientRect(parent, &parent_rect);
	GetWindowRect(window, &rect);
	Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int offset = 0;
	if (!flag) {
		int excess = (area.Width - Base_Screen_Width) / 2;
		offset = excess < 0 ? 0 : excess;
    }
	Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	box.X = parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left;
	int bottom;
	if (flag) bottom = Default_Button_Strip->Y;
	else bottom = Button_Strip_Bottom->Y;
	box.Y = bottom - box.Height;
	MoveWindow(window, box.X, box.Y, box.Width, box.Height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall Move_Flag_Control(HWND window)
{
    HWND parent = GetParent(window);
	unsigned char flag = Session.Is_In_Game();
    RECT parent_rect;
    RECT rect;
	GetClientRect(parent, &parent_rect);
	GetWindowRect(window, &rect);
	int left = parent_rect.left;
	Rect area(left, parent_rect.top, parent_rect.right - left, parent_rect.bottom - parent_rect.top);
	int offset = 0;
	if (!flag) {
		offset = (area.Width - Base_Screen_Width) / 2;
		offset = offset < 0 ? 0 : offset;
    }
	Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int shift = 0;
	if (!flag) {
		shift = (area2.Height - Base_Screen_Height) / 2;
		shift = shift < 0 ? 0 : shift;
    }
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	MoveWindow(window, offset + 10 + left, parent_rect.bottom - shift - height - 1, width, height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall Move_Lobby_Control(HWND window)
{
    HWND parent = GetParent(window);
	unsigned char flag = Session.Is_In_Game();
    RECT parent_rect;
    RECT rect;
	GetClientRect(parent, &parent_rect);
	GetWindowRect(window, &rect);
	Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
	int offset = 0;
	if (!flag) {
		int excess = (area.Width - Base_Screen_Width) / 2;
		offset = excess < 0 ? 0 : excess;
    }
	Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	box.X = parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left;
	Rect * strip = Default_Button_Strip;
	if (!flag) strip = Button_Strip_Bottom;
	int bottom = strip->Height + strip->Y;
	box.Y = bottom - box.Height;
	MoveWindow(window, box.X, box.Y, box.Width, box.Height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall Move_Centred_Control(HWND window, Point2D const & size)
{
    HWND parent = GetParent(window);
    RECT parent_rect;
    RECT rect;
	GetWindowRect(parent, &parent_rect);
	GetWindowRect(window, &rect);
	MoveWindow(window, (VideoModeWidth - size.X) / 2 - parent_rect.left + rect.left,
		(VideoModeHeight - size.Y) / 2 - parent_rect.top + rect.top, rect.right - rect.left,
		rect.bottom - rect.top, FALSE);
}

}


namespace OwnerDraw {

#ifdef RA2_NON_MATCHING // not exact: 1749 bytes differ against the image-derived bound 005e9410 (1,938 B; ours 1,984 B): the image calls Is_Listed_Kind 0x5E0DA0 in the first test and ours expands it there (the family of Enable_Animated_Control 0x5E4780 and Is_Tested_Control 0x5E0760). With the predicate kept out of line (probe only) the frame, calls and size are the image's and one byte is left: the operand order of the last compare (height against its old value at +0x765), which six orders of the closing test do not give
void __fastcall Adjust_Control_Position(HWND window)
{
    HWND parent = GetParent(window);
    RECT parent_rect;
    RECT rect;
	GetWindowRect(parent, &parent_rect);
	GetWindowRect(window, &rect);
	int x = rect.left - parent_rect.left;
	int y = rect.top - parent_rect.top;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int kind = Kind_Of((unsigned int)parent);
	int id = GetDlgCtrlID(window);
	int old_x = x;
	int old_y = y;
	int old_width = width;
	int old_height = height;
	if ((kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103 || Is_Listed_Kind(kind)) && id == 0x694) {
		bool c12 = kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103;
		if (c12) {
			bool c4 = Is_Option_Kind(kind) || kind == 0x105 || kind == 0x6b || kind == 0x113;
			if (c4) {
				if (!Session.Is_In_Game()) {
					y += 1;
                }
            } else {
				if (!Session.Is_In_Game()) {
					y += 7;
					height += 1;
                }
            }
		} else if (kind == 0x125 || kind == 0xe7 || kind == 0x116 || kind == 0xc2 || kind == 0xc9 || kind == 0x11d || kind == 0x11c || kind == 0x114 || kind == 700 || kind == 0x113 || kind == 0x109 || kind == 0x10f || kind == 0x117 || kind == 0xe6 || kind == 0xf3 || kind == 0xf4 || kind == 0x10e) {
			if (kind != 0xc2 && kind != 0xc9 && kind != 0x113) {
				y += 2;
            }
        }
	} else if (kind == 0x102) {
		if (id == 0x50c) {
			y -= 1;
		} else if (id == 0x54e || id == 0x693 || id == 0x696 || id == 0x69a) {
			x -= 1;
        }
	} else if (kind == 0xbb) {
		if (id == 0x540) {
			width -= 1;
        }
	} else if (kind == 0xbc || kind == 0xbd) {
		if (id == 0x53f) {
			width += 1;
		} else if (id == 0x54f) {
			y -= 1;
		} else if (id == 0x54e || id == 1000 || id == 0x54d || id == 0x69a) {
			x -= 1;
        }
	} else if (kind == 0x103) {
		if (id == 0x6f3 || id == 0x6f4 || id == 0x69f || id == 0x6f6 || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2) {
			if (VideoModeWidth >= Base_Screen_Width) {
				y -= (Base_Screen_Height - LowResHeight) / 2;
            }
        }
	} else if (kind == 0xbc7) {
		if (id == 0x733 || id == 0x735 || id == 0x732 || id == 0x3ea || id == 0x73c || id == 0x411 || id == 0x412 || id == 0x73d || id == 0x419 || id == 0x41a || id == 0x73e || id == 0x6a3 || id == 0x6a4 || id == 0x73f || id == 0x6a5 || id == 0x6a7 || id == 0x740 || id == 0x6a6 || id == 0x6a8 || id == 0x736 || id == 0x73a || id == 0x73b || id == 0x737 || id == 0x739 || id == 0x738) {
			if (VideoModeWidth >= Base_Screen_Width) {
				y -= (Base_Screen_Height - LowResHeight) / 2;
            }
        }
    }
	if (x != old_x || y != old_y || width != old_width || old_height != height) {
		MoveWindow(window, x, y, width, height, FALSE);
    }
}
#endif

}


namespace OwnerDraw {

inline bool __cdecl Is_Lobby_Control(unsigned int key, HWND window)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	int kind = record != 0 ? record->Kind : 0;
	int id = GetDlgCtrlID(window);
	if (kind == 0xe2) {
		return id == 0x71d;
    }
	return false;
}
BOOL __stdcall Move_Child_For_Resolution(HWND window, LPARAM lparam)
{
    HWND parent = GetParent(window);
	if (Current_Dialog == parent) {
		Rect rect(0, 0, 0, 0);
		Point2D const & size = *(Point2D const *)lparam;
		if (Select_Control_Rect(parent, window, &rect)) {
			Move_Control_To_Rect(window, rect);
		} else if (Is_Free_Owner_Button((unsigned int)parent, window)) {
			Move_Free_Button(window, size);
		} else if (Is_Owner_Tested_Control((unsigned int)parent, window)) {
			Move_Tested_Control(window, size);
		} else if (Is_Owner_Default_Button((unsigned int)parent, window)) {
			Move_Default_Button_Owner(window);
		} else if (Is_Default_Button((unsigned int)parent, window)) {
			Move_Default_Button(window);
		} else if (Is_Flag_Control((unsigned int)parent, window)) {
			Move_Flag_Control(window);
		} else if (Is_Lobby_Control((unsigned int)parent, window)) {
			Move_Lobby_Control(window);
        } else {
			Move_Centred_Control(window, size);
        }
		Adjust_Control_Position(window);
    }
	return TRUE;
}

}


void __fastcall Resize_Dialog_To_Screen(HWND dialog, Point2D * old_size)
{
	int width = VideoModeWidth;
	int height = VideoModeHeight;
	MoveWindow(dialog, 0, 0, width, height, 0);
	Current_Dialog = dialog;
	LPARAM param = (LPARAM)old_size;
	EnumChildWindows(dialog, OwnerDraw::Move_Child_For_Resolution, param);
}


namespace OwnerDraw {

int __fastcall Control_Drawer(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	return record ? record->Drawer : 0;
}

}


namespace OwnerDraw {
bool __fastcall Is_Framed_Dialog(HWND dialog)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)dialog, &record);
	if (!record) return false;
	int kind = record->Kind;
	bool special = kind == 0x10c ||
		kind == 0x94 ||
		kind == 0xb6 ||
		kind == 0xa3 ||
		kind == 0x73 ||
		kind == 0xd8 ||
		kind == 0xbbb ||
		kind == 0xf5 ||
		kind == 0x105 ||
		kind == 0xd5 ||
		kind == 0xe2 ||
		kind == 0x2b5 ||
		kind == 0xb7 ||
		kind == 0x2b4 ||
		kind == 0x101 ||
		kind == 0x129 ||
		kind == 0xbc6 ||
		kind == 0xbc7 ||
		kind == 0x10b ||
		kind == 0xd7 ||
		kind == 0x108 ||
		kind == 0xbc ||
		kind == 0xbd ||
		kind == 0xbb ||
		kind == 0xb5 ||
		kind == 0xbba ||
		kind == 0xff ||
		kind == 0xea ||
		kind == 0x6b ||
		kind == 0x100 ||
		kind == 0x102 ||
		kind == 0xb8 ||
		kind == 0xd6 ||
		kind == 0x103 ||
		kind == 0xd4 ||
		kind == 0x125 ||
		kind == 0x122 ||
		kind == 0x112 ||
		kind == 0xe7 ||
		kind == 0x116 ||
		kind == 0xc2 ||
		kind == 0xc9 ||
		kind == 0x113 ||
		kind == 0xfe ||
		kind == 0x10f ||
		kind == 0x11d ||
		kind == 0x11c ||
		kind == 0x117 ||
		kind == 0x114 ||
		kind == 0xe6 ||
		kind == 0xf3 ||
		kind == 0xf4 ||
		kind == 0x2bc ||
		kind == 0x10e ||
		kind == 0xfb;
	if (special) {
		record->ButtonKind = 1;
		record->IsHidden = true;
    }
	return special;
}
}


namespace OwnerDraw {

inline WinData * __cdecl Control_Record(HWND control)
{
	unsigned int key = (unsigned int)control;
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	return record;
}

bool __fastcall Centre_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer(*(unsigned int *)&control, &record);
	if (!record) {
		return false;
    }
	int kind = record->Kind;
	bool special = kind == 0x10d ||
		kind == 0xd9 ||
		kind == 0xf0 ||
		kind == 0xce ||
		kind == 0x120 ||
		kind == 0x121 ||
		kind == 0x115 ||
		kind == 0xd3 ||
		kind == 0xcf ||
		kind == 0x11f ||
		kind == 0xc3 ||
		kind == 0x11b ||
		kind == 0xe1 ||
		kind == 0x11e ||
		kind == 0xc4 ||
		kind == 300 ||
		kind == 0xd0 ||
		kind == 0xfc ||
		kind == 0x126;
	if (special) {
		record->ButtonKind = 2;
    }
	return special;
}

}


namespace OwnerDraw {

bool __fastcall Is_Lobby_Frame_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		return record->IsLobbyFrame;
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Update_Lobby_Frame_Dialog(HWND control)
{
	WinData * record = Control_Record(control);
	if (!record) {
		return false;
    }
	return record->IsLobbyFrame = record->Kind == 0xbc || record->Kind == 0xbd || record->Kind == 0x102 || record->Kind == 0xc2 || record->Kind == 0xc9;
}

}


namespace OwnerDraw {

void __fastcall Set_Lobby_Frame_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsLobbyFrame = true;
    }
}

}


namespace OwnerDraw {

void __fastcall Clear_Lobby_Frame_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsLobbyFrame = false;
    }
}

}


namespace OwnerDraw {

bool __fastcall Is_Lobby_Art_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		return record->IsLobbyArt;
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Update_Lobby_Art_Dialog(HWND control)
{
	WinData * record = Control_Record(control);
	if (!record) {
		return false;
    }
	return record->IsLobbyArt = record->Kind == 0xbc || record->Kind == 0xbd || record->Kind == 0x102 || record->Kind == 0xc2 || record->Kind == 0xc9 || record->Kind == 0x105 || record->Kind == 0x6b || record->Kind == 0x113;
}

}


namespace OwnerDraw {

void __fastcall Set_Lobby_Art_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsLobbyArt = true;
    }
}

}


namespace OwnerDraw {

void __fastcall Clear_Lobby_Art_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		record->IsLobbyArt = false;
    }
}

}


namespace OwnerDraw {

bool __fastcall Is_Chat_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	return record ? record->IsChat : false;
}

}


namespace OwnerDraw {

bool __fastcall Update_Chat_Dialog(HWND control)
{
	WinData * record = Control_Record(control);
	if (!record) {
		return false;
    }
	return record->IsChat = record->Kind == 0x103 || record->Kind == 0xbc7;
}

}


namespace OwnerDraw {

bool __fastcall Is_Setup_Dialog(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		return record->IsSetup;
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Update_Setup_Dialog(HWND control)
{
	WinData * record = Control_Record(control);
	if (!record) {
		return false;
    }
	return record->IsSetup = record->Kind == 0x108 || record->Kind == 0xbc6;
}

}


namespace OwnerDraw {

int __fastcall Control_Shape_640(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	return record ? record->Shape640 : 0;
}

}


namespace OwnerDraw {

int __fastcall Control_Shape(HWND control)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	return record ? record->Shape : 0;
}

}


struct ShapeSet;
void * Get_Score_Drawer();

namespace OwnerDraw {

void __fastcall Set_Control_Art(unsigned int key)
{
	WinData * record = 0;
	Control_Table.getPointer(key, &record);
	if (record != 0) {
		int kind = record->Kind;
		if (kind == 0x94) {
			record->Drawer = (int)Get_Slot_Art();
			record->Shape640 = (int)Art_Button_Back;
			record->Shape = (int)Art_Button_Back;
		} else if (kind == 0x103) {
			record->Drawer = (int)Get_Chat_Art();
			record->Shape640 = (int)Art_Chat_Back;
			record->Shape = (int)Art_Chat_Back;
		} else if (kind == 0x108) {
			record->Drawer = (int)Get_Score_Drawer();
			record->Shape640 = (int)Art_Score_Screen;
			record->Shape = (int)Art_Score_Screen;
		} else if (kind == 0xBC6) {
			record->Drawer = (int)Get_Chat_Art();
			record->Shape640 = (int)Art_Chat_Back;
			record->Shape = (int)Art_Chat_Back;
		} else if (kind == 0xBC7) {
			record->Drawer = (int)Get_Chat_Art();
			record->Shape640 = (int)Art_Chat_Back;
			record->Shape = (int)Art_Chat_Back;
        } else {
			record->Drawer = (int)Get_Drawer();
			record->Shape640 = (int)Art_Dialog_Back640;
			record->Shape = (int)Art_Dialog_Back;
        }
    }
}

}


namespace OwnerDraw {

void __fastcall Set_Dialog_Kind(HWND control, int value)
{
	WinData * record = Get_Record(control); // RA2: ra2-decomp's Find_Data (Yuri's Revenge 0x0060D2C0)
	if (record) {
		record->Kind = value;
    }
}

}

int __fastcall Dialog_ID_Of_Window(HWND control)
{
    using namespace OwnerDraw;
	WinData * record = 0;
	Control_Table.getPointer((unsigned int)control, &record);
	if (record) {
		return record->Kind;
    }
	return 0;
}

int __fastcall Dialog_Box(int resource, int(__stdcall * proc)(struct HWND__ *, unsigned int, unsigned int, long), bool Show)
{
	HWND dialog = OwnerDraw::Begin_Dialog(resource, proc, 0);
	int result = 0;
	if (dialog) {
		SetWindowLongA(dialog, DWL_USER, (LONG)&result);
		OwnerDraw::Display_Dialog(dialog);
		if (Show) {
			Show_Dialog_With_Flag(dialog);
        }
		while (result == 0) {
			if (OwnerDraw::Dialog_Message_Handler()) {
                break;
            }
        }
		OwnerDraw::End_Dialog(dialog);
    }
	return result;
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
void OwnerDraw::Shutdown(void)
{
}


namespace OwnerDraw {

void __fastcall Register_Window_Classes()
{
	if (Classes_Registered == 1) {
        return;
    }
	Classes_Registered = 1;
	WNDCLASSA window_class;
	memset(&window_class, 0, sizeof(window_class));
	window_class.hInstance = ProgramInstance;
	window_class.style = 3;
	window_class.lpfnWndProc = Combo_Drop_Proc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hIcon = 0;
	window_class.hCursor = 0;
	window_class.hbrBackground = 0;
	window_class.lpszMenuName = "ComboDropWin";
	window_class.lpszClassName = "ComboDropWin";
	RegisterClassA(&window_class);
	memset(&window_class, 0, sizeof(window_class));
	window_class.style = 0x83;
	window_class.lpfnWndProc = New_Edit_Window_Proc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = ProgramInstance;
	window_class.hIcon = 0;
	window_class.hCursor = 0;
	window_class.hbrBackground = 0;
	window_class.lpszMenuName = 0;
	window_class.lpszClassName = "NewEdit";
	RegisterClassA(&window_class);
}

}


LRESULT __stdcall OwnerDraw::New_Edit_Window_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProcA(window, message, wparam, lparam);
}


namespace OwnerDraw {

int __fastcall Draw_Text_In_Rect_Ex(Surface * surface, wchar_t const * text, RECT * area, BitFont * font,
	unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10);

HWND Combo_Owner;
HWND Dropdown_Owner;
HWND Dropdown_Window;

struct ScrollBarInfo {
		unsigned int cbSize;
		unsigned int fMask;
		int nMin;
		int nMax;
		unsigned int nPage;
		int nPos;
		int nTrackPos;
};

struct ListItemData {
		int Reserved_00;
		int Reserved_04;
		wchar_t * text;
};

inline int __cdecl Get_Item_Text(HWND owner, int const & index, wchar_t * text, int size)
{
	WNDPROC proc = 0;
	Window_Proc_Table.getValue(owner, proc);
	text[0] = 0;
	int item = CallWindowProcA(proc, owner, 0x199, index, 0);
	if (item == -1 || item == 0) {
		item = CallWindowProcA(proc, owner, 0x150, index, 0);
    }
	if (item != -1 && item != 0) {
		wcsncpy(text, ((ListItemData *)item)->text, size);
		text[size - 1] = 0;
		return wcslen(text);
    }
	return 0;
}


LRESULT __stdcall Combo_Drop_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	static void * solid_brush = CreateSolidBrush(0x306030);
	(void)solid_brush; // RA2: the reference text's own statement here: TibSun archive code/ownrdraw.cpp:263, ComboDropWinCtrlProc (adopted text)
    RECT rect;
    Get_Display_Rect(window, &rect);
    RECT client;
    GetClientRect(window, &client);
    Rect fill;
    RECT parent_rect;
    memset(&parent_rect, 0, sizeof(parent_rect));
	int scrollbar_width = 2 * Border_Width + 18;
    int need_scrollbar = -1;
	int max_top = 0;
	HWND parent = 0;
	if (Combo_Owner) {
		parent = GetParent(Combo_Owner);
    }
	WinData * parent_data = 0;
    if (parent) {
		Control_Table.getPointer((unsigned int const &)parent, &parent_data);
        Get_Display_Rect(parent, &parent_rect);
    }
	Dropdown_Owner = parent;
	Dropdown_Window = window;
	WinData * data = 0;
	Control_Table.getPointer((unsigned int const &)window, &data);
	if (data == 0) {
        DebugString("ComboBox dropdown windata = NULL\n");
    }
	WinData * master = 0;
	if (Combo_Owner) {
		Control_Table.getPointer((unsigned int const &)Combo_Owner, &master);
    }
	if (message != CB_GETCOUNT && message != CB_GETITEMHEIGHT && message != WM_VSCROLL) {
		int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0);
		int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0);
        if (item_height <= 1) {
            item_height = 1;
        }
		need_scrollbar = item_count * item_height > client.bottom - client.top;
		max_top = item_count - (client.bottom - client.top) / item_height;
        if (data) {
			if ((unsigned int)data->ScrollBar > 1) {
				ScrollBarInfo info;
				info.fMask = 5;
                info.nMin = 0;
				info.nMax = max_top;
				info.nPos = data->Position;
				info.cbSize = 0x1c;
				SendMessageA(data->ScrollBar, SBM_SETSCROLLINFO, 0, (LPARAM)&info);
            }
			if (data->ScrollBar) {
				BringWindowToTop(data->ScrollBar);
            }
        }
    }
    switch (message) {
		case WM_ERASEBKGND:
			return 0;
		case WM_CREATE: {
        SetCapture(window);
			Combo_Owner = *(HWND *)lparam;
			char buffer[100];
			sprintf(buffer, "Dropd=%p  Master=%p  Dlg=%p", window, Combo_Owner, GetParent(Combo_Owner));
			DebugString(buffer);
			SetTimer(window, 0, 20, 0);
			return 0;
    }
		case WM_NCDESTROY:
			Dropdown_Window = 0;
			Dropdown_Owner = 0;
        ReleaseCapture();
        KillTimer(window, 0);
        break;
		case WM_VSCROLL: {
			int top = SendMessageA(data->ScrollBar, SBM_GETPOS, 0, 0);
			if (top != SendMessageA(window, CB_GETTOPINDEX, 0, 0)) {
				SendMessageA(window, CB_SETTOPINDEX, top, 0);
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
        if (data) {
				return data->Position;
        }
        break;
		case WM_MOUSEMOVE: {
			int x = lparam & 0xffff;
			int y = (unsigned int)lparam >> 16;
			int index = SendMessageA(window, 0x4e8, 0, (y << 16) | x);
        if (index != -1) {
				if (data->ItemState != index) {
					InvalidateRect(window, 0, 0);
            }
				data->ItemState = index;
				HWND tip = GetDlgItem(parent, 0x695);
				if (tip) {
					WideWstring text;
					text = Get_Item_Tip_Text(parent, (int)Combo_Owner, index);
					if (wcscmp((wchar_t const *)text.get(), L"")) {
						SendMessageA(tip, 0x4b2, 0, (LPARAM)text.text());
                } else {
						text = Get_Control_Tip_Text(parent, (int)Combo_Owner);
						if (wcscmp((wchar_t const *)text.get(), L"")) {
							SendMessageA(tip, 0x4b2, 0, (LPARAM)text.text());
                    }
                }
            }
        }
			return 0;
    }
		case CB_SETTOPINDEX: {
			int top = wparam;
			int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0);
			int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0);
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
			if (top != data->Position) {
				data->Position = top;
				InvalidateRect(window, 0, 0);
        }
			return 0;
    }
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK: {
			int x = lparam & 0xffff;
			int y = (unsigned int)lparam >> 16;
			if (x > client.right && x < client.right + data->ScrollWidth && y > 0 && y < client.bottom) {
				SendMessageA(data->ScrollBar, message, wparam, lparam);
            break;
        }
			VocClass::PlayGlobal(Rule->GUIComboCloseSound, 1.0f, 0x2000, 0);
        if (x >= 0 && y >= 0 && x <= client.right && y <= client.bottom) {
				int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0);
				int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0);
            int index = y / item_height;
            if (data) {
					index += data->Position;
            }
            int clamped = index < 0 ? 0 : index;
				int max_index = item_count - 1;
            if (max_index < clamped) {
                clamped = max_index;
            }
				SendMessageA(Combo_Owner, CB_SETCURSEL, clamped, 0);
				int id = GetWindowLongA(Combo_Owner, GWL_ID);
            if (need_scrollbar) {
					DestroyWindow(data->ScrollBar);
            }
            ReleaseCapture();
				SendMessageA(Combo_Owner, CB_SHOWDROPDOWN, 0, 0);
				PostMessageA(parent, WM_COMMAND, (id & 0xffff) | 0x10000, (LPARAM)Combo_Owner);
				return 0;
        }
        if (need_scrollbar) {
				DestroyWindow(data->ScrollBar);
        }
        ReleaseCapture();
			SendMessageA(Combo_Owner, CB_SHOWDROPDOWN, 0, 0);
			return 0;
    }
		case 0x4e8: {
			int x = lparam & 0xffff;
			int y = (unsigned int)lparam >> 16;
			int result = -1;
        if (x >= 0 && y >= 0 && x < client.right && y < client.bottom) {
				int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0);
				int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0);
				int index = y / item_height + data->Position;
            int clamped = index < 0 ? 0 : index;
				int max_index = item_count - 1;
            if (max_index < clamped) {
                clamped = max_index;
            }
				result = clamped;
        }
			return result;
    }
		case 0x4ea:
			return (LRESULT)Combo_Owner;
		case 0x7e8:
			data->ItemState = SendMessageA(Combo_Owner, CB_GETCURSEL, 0, 0);
			return 0;
		case WM_PAINT: {
			BitFont * font = parent_data->Font;
			int item_count = SendMessageA(Combo_Owner, CB_GETCOUNT, 0, 0);
			int item_height = SendMessageA(Combo_Owner, CB_GETITEMHEIGHT, 0, 0);
        int client_height = client.bottom - client.top;
        Rect dest_rect(rect.left, rect.top, rect.right - rect.left, client_height);
			int selected = data->ItemState;
        Rect src;
			src.X = rect.left - parent_rect.left;
			src.Y = rect.top - parent_rect.top;
			src.Width = client.right;
			src.Height = client.bottom;
			if (data->Back == 0) {
            Rect dst(0, 0, client.right, client.bottom);
				BSurface * surface = new BSurface(client.right, client.bottom, 2);
				data->Back = surface;
				Surface_Count++;
				if (parent_data && parent_data->Back) {
					surface->Blit_From(dst, *parent_data->Back, src, false, true);
            }
            int total = client.right * client.bottom;
				unsigned short * pixels = (unsigned short *)surface->Lock();
				for (int i = 0; i < total; i++) {
					unsigned short pixel = pixels[i];
					pixels[i] = Blend_Color(pixel, 0, data->TextStyle);
            }
            surface->Unlock();
        }
        {
				Rect from;
				from.X = 0;
				from.Y = 0;
				from.Width = dest_rect.Width;
				from.Height = dest_rect.Height;
				AlternateSurface->Blit_From(dest_rect, *data->Back, from, false, true);
        }
			Rect frame(dest_rect);
			int color = Color_To_Pixel(Default_Frame_Color);
			if (master && master->IsAlternateColors) {
				color = Color_To_Pixel(Alternate_Frame_Color);
        }
			Draw_Frame(AlternateSurface, frame, 1, color);
			if (need_scrollbar && data && data->ScrollBar) {
				Rect outer(dest_rect);
				outer.Width += scrollbar_width;
				Draw_Frame(AlternateSurface, outer, 1, color);
        }
			int first = data->Position;
			Rect row;
			for (int index = first; index < item_count; index++) {
				row.X = rect.left;
				int width = rect.right - rect.left;
				row.Y = (index - first) * item_height + rect.top;
				if ((index - first) * item_height + item_height > client_height) {
                break;
            }
				wchar_t text[128];
				Get_Item_Text(Combo_Owner, index, text, 0x80);
				if (index == selected) {
					fill.X = row.X;
					fill.Y = row.Y;
					fill.Width = width;
					fill.Height = item_height;
					int fill_color;
					if (master ? master->IsAlternateColors : false) {
						fill_color = Color_To_Pixel(Pressed_Fill_Color);
                } else {
						fill_color = Color_To_Pixel(Fill_Color);
                }
                AlternateSurface->Fill_Rect(fill, fill_color);
            }
            RECT text_rect;
				text_rect.top = row.Y;
				text_rect.left = row.X + 3;
				text_rect.right = row.X + width;
				text_rect.bottom = row.Y + item_height;
				unsigned long text_color = Text_Color;
				if (master && master->IsAlternateColors) {
					text_color = Highlight_Text_Color;
            }
				if (index < 50 && master && master->ItemColors[index] >= 0) {
					text_color = master->ItemColors[index];
					if (master->IsItemFilled) {
						Rect fill2;
						fill2.X = row.X;
						fill2.Y = row.Y;
						fill2.Width = width;
						fill2.Height = item_height;
						Inflate_Rect(fill2, 2);
						AlternateSurface->Fill_Rect(fill2, Color_To_Pixel(text_color));
                }
            }
				int max_width = client.right - 20;
				int length = wcslen(text);
				int text_width = font->String_Pixel_Width(text, 0);
				while (length && text_width > max_width) {
					length--;
					text[length] = 0;
					wcscat(text, L"...");
					text_width = font->String_Pixel_Width(text, 0);
            }
				Draw_Text_In_Rect_Ex(AlternateSurface, text, &text_rect, font, text_color, 4, 12, 0, 0, 0);
        }
        AlternateSurface->Lock();
        VisibleSurface->Lock();
			Rect screen(dest_rect);
        RECT window_rect;
        GetWindowRect(window, &window_rect);
			screen.X += window_rect.left - rect.left;
			screen.Y += window_rect.top - rect.top;
			VisibleSurface->Blit_From(screen, *AlternateSurface, dest_rect, false, true);
        VisibleSurface->Unlock();
        AlternateSurface->Unlock();
        ValidateRect(window, 0);
			return 0;
    }
		case WM_DESTROY:
			Remove_Audio_File_Window(window);
        break;
    }
    if (need_scrollbar == 1) {
		if (data && !data->ScrollBar) {
			data->ScrollBar = (HWND)1;
			GetWindowLongA(window, GWL_ID);
            parent = GetParent(window);
            RECT parent_display;
            Get_Display_Rect(parent, &parent_display);
			RECT drop_display;
			Get_Display_Rect(window, &drop_display);
			int left = drop_display.left - parent_display.left;
			int top = drop_display.top - parent_display.top;
			bool alternate = false;
			int x = left - scrollbar_width + client.right;
			int y = top + client.top;
			int height = drop_display.bottom - drop_display.top;
			if (master) {
				alternate = master->IsAlternateColors;
            }
			HWND scroll = CreateWindowExA(0, "Scrollbar", 0, 0x50010001, x, y, scrollbar_width, height, parent, 0,
				ProgramInstance, 0);
			data->ScrollBar = scroll;
			data->ScrollWidth = scrollbar_width;
			Subclass_Control(scroll, 0);
			WinData * scroll_data = 0;
			Control_Table.getPointer((unsigned int const &)data->ScrollBar, &scroll_data);
			scroll_data->ScrollOwner = window;
			scroll_data->IsAlternateColors = data->IsAlternateColors;
			ScrollBarInfo info;
			info.nMax = max_top;
			info.fMask = 5;
            info.nMin = 0;
			info.nPos = data->Position;
			info.cbSize = 0x1c;
			SendMessageA(data->ScrollBar, SBM_SETSCROLLINFO, 0, (LPARAM)&info);
			SendMessageA(data->ScrollBar, 0x4f1, 0, alternate);
			SetWindowPos(window, 0, 0, 0, drop_display.right - drop_display.left - scrollbar_width, drop_display.bottom - drop_display.top, SWP_NOMOVE);
			ShowWindow(data->ScrollBar, SW_SHOW);
			BringWindowToTop(data->ScrollBar);
			InvalidateRect(data->ScrollBar, 0, 0);
			UpdateWindow(data->ScrollBar);
			SendMessageA(data->ScrollBar, 0x4a9, (WPARAM)data->ScrollBar, 1);
			SendMessageA(data->ScrollBar, 0x4d2, 0, 1);
			RECT validate;
			validate.left = left;
			validate.top = top;
			validate.right = left + client.right + 1;
			validate.bottom = top + client.bottom + 1;
			ValidateRect(parent, &validate);
        }
	} else if (need_scrollbar == 0 && data && data->ScrollBar && !data->SkipCheckPaint) {
		DestroyWindow(data->ScrollBar);
		Restore_Child(data->ScrollBar, 0);
		data->ScrollBar = 0;
		data->ScrollWidth = 0;
		SetWindowPos(window, 0, 0, 0, client.right - client.left + scrollbar_width, client.bottom - client.top, SWP_NOMOVE);
        parent = GetParent(window);
		RECT validate_parent;
		Get_Display_Rect(parent, &validate_parent);
		RECT validate;
		validate.left = rect.left - validate_parent.left;
		validate.top = rect.top - validate_parent.top;
		validate.right = validate.left + client.right + scrollbar_width;
		validate.bottom = validate.top + client.bottom + 1;
		ValidateRect(parent, &validate);
    }
	return DefWindowProcA(window, message, wparam, lparam);
}

}


namespace OwnerDraw {

BOOL __stdcall Replace_Edit(HWND window, LPARAM)
{
	unsigned int style = GetWindowLongA(window, GWL_STYLE);
	SetWindowLongA(window, GWL_USERDATA, 0);
	if ((style & WS_VISIBLE) && !(style & WS_DISABLED)) {
		char name[128];
		GetClassNameA(window, name, sizeof(name));
		if (strcmp(name, "Edit") == 0) {
            HWND parent = GetParent(window);
            RECT rect;
            RECT parent_rect;
            Get_Display_Rect(window, &rect);
            Get_Display_Rect(parent, &parent_rect);
			int x = rect.left - parent_rect.left;
			int y = rect.top - parent_rect.top;
			DWORD list_style = (WS_CHILD | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT);
			if (style & WS_TABSTOP) {
				list_style = (WS_CHILD | WS_TABSTOP | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT);
            }
			HWND list = CreateWindowExA(0, "ListBox", 0, list_style, x, y,
				rect.right - rect.left, rect.bottom - rect.top, parent, 0, ProgramInstance, 0);
			LONG * data = (LONG *)operator new(8);
			data[0] = (LONG) "NewEdit";
			data[1] = GetWindowLongA(window, GWL_STYLE);
			SetWindowLongA(list, GWL_USERDATA, (LONG)data);
			EnableWindow(window, FALSE);
			ShowWindow(window, SW_HIDE);
			EnableWindow(list, TRUE);
			ShowWindow(list, SW_SHOWNORMAL);
			LONG id = GetWindowLongA(window, GWL_ID);
			SetWindowLongA(window, GWL_ID, GetWindowLongA(list, GWL_ID));
			SetWindowLongA(list, GWL_ID, id);
        }
    }
	return TRUE;
}

}


namespace OwnerDraw {


bool __fastcall Subclass_Dialog(HWND window, LPARAM lparam)
{
	Register_Window_Classes();
	Current_Dialog = window;
	EnumChildWindows(window, Set_Paint_Disabled, 1);
	Set_Paint_Disabled(window, 1);
	EnumChildWindows(window, Replace_Edit, 1);
	EnumChildWindows(window, Subclass_Control, lparam);
	Subclass_Control(window, lparam);
	EnumChildWindows(window, Set_Paint_Disabled, 0);
	Set_Paint_Disabled(window, 0);
	return true;
}

}


namespace OwnerDraw {

BOOL __stdcall Set_Paint_Disabled(HWND window, LPARAM lparam)
{
	WinData * data = 0;
	if (!Control_Table.getPointer((unsigned int const &)window, &data)) {
        WinData temp;
		Control_Table.add((unsigned int const &)window, temp);
		Control_Table.getPointer((unsigned int const &)window, &data);
    }
	data->SkipCheckPaint = lparam;
	return TRUE;
}

}


namespace OwnerDraw {

LRESULT __stdcall List_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Combo_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall New_Edit_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Static_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall Owner_Button_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

BOOL __stdcall Subclass_Control(HWND window, LPARAM lparam)
{
    char class_name[128];
	GetClassNameA(window, class_name, sizeof(class_name));
	LONG style = GetWindowLongA(window, GWL_STYLE);
    RECT rect1;
    Get_Display_Rect(window, &rect1);
    RECT rect2;
    GetClientRect(window, &rect2);
	Add_Audio_File_Window(window);
	Init_Default_Colors();
	if (!Masks_Ready) {
		Init_Color_Masks();
		Load_Dialog_Images();
		Masks_Ready = 1;
    }
	WNDPROC custom_proc = 0;
	int kind = 0xb;
	LONG * user_data = (LONG *)GetWindowLongA(window, GWL_USERDATA);
	if (user_data) {
		strcpy(class_name, (char const *)user_data[0]);
    }
	if (strcmp(class_name, "ScrollBar") == 0) {
		custom_proc = Scroll_Bar_Proc;
		kind = 8;
	} else if (strcmp(class_name, "ListBox") == 0) {
		custom_proc = List_Proc;
		kind = 4;
	} else if (strcmp(class_name, "ComboBox") == 0) {
		custom_proc = Combo_Proc;
		kind = 3;
	} else if (strcmp(class_name, "msctls_trackbar32") == 0) {
		custom_proc = Trackbar_Proc;
		kind = 7;
	} else if (strcmp(class_name, "msctls_progress32") == 0) {
		custom_proc = Progress_Proc;
		kind = 6;
	} else if (strcmp(class_name, "NewEdit") == 0) {
		custom_proc = New_Edit_Proc;
		kind = 1;
	} else if (strcmp(class_name, "Edit") == 0) {
		custom_proc = Edit_Control_Proc;
		kind = 1;
	} else if (strcmp(class_name, "Static") == 0) {
		custom_proc = Static_Proc;
		kind = 2;
	} else if (strcmp(class_name, "SysTabControl32") == 0) {
		custom_proc = Tab_Proc;
		kind = 0xa;
	} else if (strcmp(class_name, "Button") == 0) {
		if ((style & (unsigned char)BS_GROUPBOX) == (unsigned char)BS_GROUPBOX) {
			custom_proc = Group_Box_Proc;
		} else if ((style & (unsigned char)BS_OWNERDRAW) == (unsigned char)BS_OWNERDRAW) {
			custom_proc = Owner_Button_Proc;
		} else if ((style & (unsigned char)BS_AUTOCHECKBOX) == (unsigned char)BS_AUTOCHECKBOX) {
			custom_proc = Checkbox_Proc;
		} else if ((style & (unsigned char)BS_AUTORADIOBUTTON) == (unsigned char)BS_AUTORADIOBUTTON) {
			custom_proc = Button_Proc;
        }
		kind = 0;
	} else if (strcmp(class_name, "msctls_hotkey32") == 0) {
		custom_proc = Hotkey_Proc;
		kind = 9;
    } else {
		custom_proc = Default_Proc;
    }
	WNDPROC original_proc = (WNDPROC)SetWindowLongA(window, GWL_WNDPROC, (LONG)Control_Proc);
	if (!Window_Table.contains(window)) {
		Window_Table.add(window, custom_proc);
    }
	if (!Window_Proc_Table.contains(window)) {
		Window_Proc_Table.add(window, original_proc);
    }
	WinData * data = 0;
	if (!Control_Table.getPointer((unsigned int const &)window, &data)) {
        WinData temp;
		Control_Table.add((unsigned int const &)window, temp);
		Control_Table.getPointer((unsigned int const &)window, &data);
    }
	data->SubclassParam = lparam;
	data->SkipTests = kind;
	if (user_data) {
		data->UserDataWord = user_data[1];
    }
	WNDPROC proc = 0;
	Window_Proc_Table.getValue(window, proc);
    char text[2048];
	text[0] = 0;
	CallWindowProcA(proc, window, WM_GETTEXT, sizeof(text), (LPARAM)text);
	if (text[0]) {
		data->Set_Wide_Text(StringTable::LoadString(text, 0, "D:\\Ra2\\ownrdraw.cpp", 0x1a47));
    } else {
		data->Set_Wide_Text(0);
    }
	data->Reserved_2C = 0;
	SendMessageA(window, OD_SUBCLASSED, 0, 0);
	return TRUE;
}

}


namespace OwnerDraw {


struct WindowData {
		~WindowData() {}
};


BOOL __fastcall Restore_Window(HWND window)
{
	EnumChildWindows(window, Restore_Child, 0);
	Restore_Child(window, 0);
	return TRUE;
}

}


namespace OwnerDraw {

BOOL __stdcall Restore_Child(HWND window, LPARAM lparam)
{
	Window_Proc_Table.remove(window);
	Control_Table.remove((unsigned int const &)window);
	Window_Table.remove(window);
	void * data = (void *)GetWindowLongA(window, GWL_USERDATA);
	if (data != 0) {
		operator delete(data);
    }
	SetWindowLongA(window, GWL_USERDATA, 0);
	Remove_Audio_File_Window(window);
	return 1;
}

}


OwnerDraw::Tooltip ODTooltip; // RA2: 0x00A737F0, the dialogs' one tooltip (its initializer comes after the dictionaries')

namespace OwnerDraw {

OwnerDraw::Tooltip::Tooltip()
{
	bounds.X = 0;
	bounds.Y = 0;
	bounds.Width = 0;
	bounds.Height = 0;
	background = 0;
	text[0] = 0;
	isActive = 0;
	isHidden = 0;
	window = 0;
}

}


namespace OwnerDraw {
bool __fastcall Start_Tooltip(Rect const & area, unsigned short const * text, HWND window)
{
	End_Tooltip();
	ODLastTooltipTime = time(0);
	swprintf(ODTooltip.text, StringTable::LoadString("GUI:ToolTip", 0, "D:\\Ra2\\ownrdraw.cpp", 0x1a88));
	if (text) wcscpy(ODTooltip.text, text);
	ODTooltip.bounds = area;
	ODTooltip.isActive = 1;
    ODTooltip.window = window;
	return Show_Tooltip(true);
}
}


namespace OwnerDraw {
bool __fastcall Show_Tooltip(bool save_background)
{
	if (!ODTooltip.isActive) return false;
    if (save_background) {
		if (ODTooltip.background) delete ODTooltip.background;
		ODTooltip.background = 0;
		Surface * background = new BSurface(ODTooltip.bounds.Width, ODTooltip.bounds.Height, 2);
		ODTooltip.background = background;
		Rect destination(0, 0, ODTooltip.bounds.Width, ODTooltip.bounds.Height);
		background->Blit_From(destination, *VisibleSurface, ODTooltip.bounds, false, true);
    }
	ODTooltip.isHidden = 0;
	Rect area = ODTooltip.bounds;
	int color = Color_To_Pixel(Dark_Brown_Color);
    VisibleSurface->Lock();
	VisibleSurface->Fill_Rect(area, color);
    VisibleSurface->Unlock();
	Shade_Edges(area, *VisibleSurface, 8, 4);
	OD_Draw_Text(Text_Color, 0, area, ODTooltip.text,
		wcslen(ODTooltip.text), 1, 1, VisibleSurface, 0);
	++area.X;
	++area.Y;
	area.Width -= 2;
	area.Height -= 2;
	Draw_Frame(VisibleSurface, area, 1, -1);
	return true;
}
}


namespace OwnerDraw {

bool Hide_Tooltip()
{
	if (!ODTooltip.isActive) {
		return false;
    }
	if (ODTooltip.isHidden == 1) {
		return false;
    }
	if (!ODTooltip.background) {
		return false;
    }
	Rect area = ODTooltip.bounds;
	VisibleSurface->Blit_From(area, *ODTooltip.background, Rect(0, 0, area.Width, area.Height),
		false, true);
	ODTooltip.isHidden = 1;
	return true;
}

}


namespace OwnerDraw {

bool End_Tooltip()
{
	if (!ODTooltip.isActive) {
		return false;
    }
	if (ODTooltip.isHidden != 1 && ODTooltip.background) {
		Rect area = ODTooltip.bounds;
		VisibleSurface->Blit_From(area, *ODTooltip.background, Rect(0, 0, area.Width, area.Height),
			false, true);
		ODTooltip.isHidden = 1;
    }
	if (ODTooltip.background) {
        delete ODTooltip.background;
    }
	ODTooltip.background = 0;
	ODTooltip.isActive = 0;
	ODTooltip.isHidden = 0;
	return true;
}

}


typedef ArrayList<HWND> ModalWindowList;


int __fastcall IME_Get_Result(void);
void IME_Cancel_Composition();
BOOL __fastcall IME_Service_Message(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
namespace OwnerDraw {
void __fastcall WideToCharString(char * dest, wchar_t const * src, int count); // RA2: 0x6FA0F0
}
void __fastcall Inflate_Rect(Rect & rect, int dx, int dy);
int __fastcall Hash_CtrlMsg(int *);
namespace OwnerDraw {
LRESULT __stdcall Owner_Button_Proc(HWND, UINT, WPARAM, LPARAM);
LRESULT __stdcall List_Proc(HWND, UINT, WPARAM, LPARAM);
#ifdef RA2_NON_MATCHING // not exact: 4863 bytes differ against the image-derived bound 005ee460 (7,252 body bytes, 12 padding) (was 4884, the 1.001 dialog copies the update rectangle for its destination (RobinDigitalPro/ra2-1001-decomp 1510aed); previously 4892; window offset and rectangle translations as ra2-decomp's banked form banks/0x00610CA0_odh.cpp (11df85d) has it); TibSun CtrlProc's getTail and named WM_SYSKEYUP/VK_TAB give the same score
LRESULT __stdcall Control_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
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

	if (message == WM_SETCURSOR) {
		return(1);
	}
	if (IME_Service_Message(window, message, wparam, lparam)) return IME_Get_Result();

	WNDPROC specific_proc = 0;
	::Window_Table.getValue(window, specific_proc);

	WNDPROC original_proc = 0;
	::Window_Proc_Table.getValue(window, original_proc);

	if (message == WM_SYSKEYUP && wparam == VK_TAB) {
		SendMessageA(MainWindow, WM_SYSKEYUP, VK_TAB, lparam);
	}

	LRESULT result = 0;
	BOOL show_tooltip = 0;
	BOOL allow_proc = 1;
	int anim_state = 0;

	static Dictionary<MessageKey, bool> ctrlmessages((unsigned int(__fastcall *)(MessageKey &))Hash_CtrlMsg);

	MessageKey key;
	key.Window = (int)window;
	key.Message = message;

	RECT display_rect;
	Get_Display_Rect(window, &display_rect);
	RECT window_rect;
	GetWindowRect(window, &window_rect);
	Point2D offset(window_rect.left - display_rect.left, window_rect.top - display_rect.top);


	static ArrayList<HWND> hwndarray;

	if (hwndarray.length() != 0) {
		HWND topwindow = 0;
		hwndarray.getTail(topwindow);

	/*
	 * When a window has been pushed to the top (modal) via OD_SETTOP, swallow
	 * mouse messages that are not directed at it or one of its children.
	 */
		BOOL allow = 0;
		BOOL forward = 0;
		if (GetParent(window) == 0) {
			allow = 1;
		}
		if (GetWindowLongA(window, -12) <= 0) {
			allow = 1;
		}

		HWND parent = window;
		while (parent != 0) {
			if (parent == topwindow) {
				allow = 1;
				break;
			}
			parent = GetParent(parent);
		}

		if (message < 0x200 || message > 0x209) {
			forward = 1;
		}
		if (message >= 0xa0 && message <= 0xa9) {
			forward = 0;
		}
		if (message >= 0x100 && message <= 0x108) {
			forward = 0;
		}
		if (message == WM_SYSKEYUP || message == WM_SYSKEYDOWN || message == WM_SYSCOMMAND || message == WM_SYSCHAR) {
			forward = 1;
		}
		if (message == OD_GETTIPTEXT || message == WM_TIMER || message == OD_GETCELLTIP) {
			forward = 0;
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
		if (message != WM_COMMAND && message != WM_SYSKEYDOWN && message != WM_SYSKEYUP
			&& message != WM_SYSCOMMAND && message != WM_SYSCHAR) {
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
	if (message == WM_PAINT) {
		is_paint = true;
		if (!in_focus) {
			ValidateRect(window, 0);
			ctrlmessages.remove(key);
			return(0);
		}
	}

	if (in_focus == true && !was_in_focus) {
		InvalidateRect(MainWindow, 0, 0);
		in_focus = GameInFocus;
	}
	was_in_focus = in_focus;

	RECT update_rect;
	if (is_paint) {
		++num_rect_updates;
		GetUpdateRect(window, &update_rect, 0);
		update_rect.left += disp_rect.left;
		update_rect.right += disp_rect.left;
		update_rect.top += disp_rect.top;
		update_rect.bottom += disp_rect.top;
	}

	WinData * data = Get_Record(window);

	if (message == OD_HASATTACHED) {
		result = (data != NULL && data->ScrollBar != NULL);
		goto cleanup;
	}

	/*
	 * Save the device context's current font / colors so they can be
	 * restored by OD_RESTOREDC.
	 */
	if (message == OD_SAVEDC && data) {
		HDC hdc = (HDC)lparam;
		HGDIOBJ old = SelectObject(hdc, GetStockObject(0xd));
		(*(HFONT *)((char *)data + 0x1E8)) = (HFONT)old;
		SelectObject(hdc, old);
		(*(int *)((char *)data + 0x1EC)) = GetBkMode(hdc);
		(*(COLORREF *)((char *)data + 0x1F0)) = GetBkColor(hdc);
		(*(COLORREF *)((char *)data + 0x1F4)) = GetTextColor(hdc);
		result = 1;
		goto cleanup;
	}

	/*
	 * Restore the device context state saved by OD_SAVEDC.
	 */
	if (message == OD_RESTOREDC && data) {
		HDC hdc = (HDC)lparam;
		SelectObject(hdc, (*(HFONT *)((char *)data + 0x1E8)));
		SetBkMode(hdc, (*(int *)((char *)data + 0x1EC)));
		SetBkColor(hdc, (*(COLORREF *)((char *)data + 0x1F0)));
		SetTextColor(hdc, (*(COLORREF *)((char *)data + 0x1F4)));
		result = 1;
		goto cleanup;
	}

	/*
	 * Toggle paint suppression for this control (and its linked window).
	 */
	if (message == OD_DISABLEPAINT && data) {
		{
			HWND linked = data->ScrollBar;
			result = data->SkipCheckPaint;
			data->SkipCheckPaint = lparam;
			if (linked != 0) {
				WinData * linkeddata = Get_Record(linked);
				if (linkeddata != 0) {
					linkeddata->SkipCheckPaint = lparam;
				}
			}
		}

		goto call_custom_proc;
	}

	/*
	 * Push a window to the top of the modal stack.
	 */
	if (message == OD_SETTOP) {
		HWND topwindow = 0;
		hwndarray.getTail(topwindow);
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
			SetWindowPos(target, 0, 0, 0, 0, 0, 0x1 | 0x2);
			in_programmatic_reorder = 0;
		}
		goto cleanup;
	}

	if (hwndarray.length() != 0) {

		/*
		 * Keep the modal window pinned on top when Windows tries to reorder it.
		 */
		if (message == WM_WINDOWPOSCHANGING) {
			if (in_programmatic_reorder != 1) {
				HWND found = 0;
				for (int index = 0; index < hwndarray.length(); index++) {
					hwndarray.get(found, index);
					if (found == window) {
						int * wp = (int *)lparam;
						if (index == hwndarray.length() - 1 && wp[1] != 0 && (wp[6] & 0x4) == 0) {
							wp[1] = 0;
							InvalidateRect(window, 0, 0);
							result = 0;
						} else {
							wp[6] |= 0x200 | 0x4;
							InvalidateRect(window, 0, 0);
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
		if (message == WM_DESTROY) {
			HWND found = 0;
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
	if (window == (HWND)ODTooltip.window
		&& (message == WM_NCDESTROY || message == WM_SHOWWINDOW || message == WM_KILLFOCUS)
		&& ODTooltip.isActive) {
		End_Tooltip();
	}

	if (message == WM_ERASEBKGND) {
		result = 1;
		goto cleanup;
	} else if (message == WM_SETFOCUS) {
		if (specific_proc == Owner_Button_Proc || specific_proc == List_Proc) {
			SetFocus((HWND)wparam);
		}
		if (data && !data->ImeState) { IME_Cancel_Composition(); data->ImeState = 1; }
	} else if (message == WM_KILLFOCUS) {
		data->ImeState = 0;
	} else if (message == WM_SHOWWINDOW) {
		if (wparam == 0 && data) {
			(*(int *)((char *)data + 0x1FC)) = 0;
		}
	}
	if (data) {
		if (message == OD_SETIMAGE) {
			result = (LRESULT)data->Image;
			data->Image = (Surface *)lparam;
			goto cleanup;
		} else if (message == OD_SETALTIMAGE) {
			result = (LRESULT)data->AlternateImage;
			data->AlternateImage = (Surface *)lparam;
			goto cleanup;
		} else if (message == OD_TOOLTIPS) {
			result = (*(int *)((char *)data + 0x24));
			(*(int *)((char *)data + 0x24)) = lparam;
			goto cleanup;
		}
		if ((*(int *)((char *)data + 0x24)) && (message == WM_TIMER || (message >= 0x200 && message <= 0x209))) {
			RECT corner;
			GetWindowRect(window, &corner);
			WindowFromPoint(*(POINT *)&corner);
		}
	}
	if (message == 0xC) allow_proc = false;
	else if (message == 0xD) { allow_proc = false; result = 0; }
    else if (message == 0x189) { allow_proc = false; result = 0; }
    else if (message == 0x143 || message == 0x14C || message == 0x158 || message == 0x14D || message == 0x14A || message == 0x148 || message == 0x180 || message == 0x18F || message == 0x1A2 || message == 0x18C || message == 0x181) { allow_proc = false; result = -1; }
    if (data) {
		if (message == 0x4CE) { allow_proc = false; result = data->Reserved_2C == 0; }
        else if (message == 0x4B5) {
			char * destination = (char *)lparam;
			if (destination) {
                *destination = 0;
				if (data->Text) {
					WideToCharString(destination, data->Text, wparam);
					if (wparam > 0) destination[wparam - 1] = 0;
                }
            }
		} else if (message == 0x4B3) {
			wchar_t * destination = (wchar_t *)lparam;
			if (destination) {
                *destination = 0;
				if (data->Text) {
					wcsncpy(destination, data->Text, wparam);
					if ((int)wparam > 0) destination[wparam - 1] = 0;
                }
            }
		} else if (message == 0x4EB) {
            *(int *)((char *)data + 0x50) = lparam;
		} else if (message == 0x4B4) {
			data->Set_Text((char const *)lparam);
			data->Reserved_2C = 1;
		} else if (message == 0x4D1) {
			result = *(int *)((char *)data + 0x30);
            *(int *)((char *)data + 0x30) = wparam;
			allow_proc = false;
		} else if (message == OD_SETTEXTW) {
			wchar_t const * text = data->Text;
			wchar_t const * new_text = (wchar_t const *)lparam;
			bool either = text != 0 || new_text != 0;
			bool added = text == 0 && new_text != 0;
			bool removed = text != 0 && new_text == 0;
			bool changed = added || removed || (either && wcscmp(text, new_text) != 0);
			data->Set_Wide_Text((wchar_t const *)lparam);
			data->Reserved_2C = 0;
			if (changed && data->PictureMode == 1 && data->Reserved_94[0xA8 - 0x94]) {
                KillTimer(window, 0);
				data->Reserved_94[0xA8 - 0x94] = 0;
				SendMessageA(window, 0x4EE, 0, 0);
            }
        }
    }
	if (message == WM_MOUSEMOVE) {
        HWND parent = GetParent(window);
		HWND status = GetDlgItem(parent, 0x695);
		if (status) {
			WideWstring text;
			unsigned int mx = (unsigned short)lparam;
			unsigned int my = (unsigned short)((unsigned int)lparam >> 16);
			text = Get_Item_Tip_Text_At(parent, window, mx, my);
			if (wcscmp(text.get(), (wchar_t const *)L"") == 0) {
				text = Get_Control_Tip_Text(parent, (int)window);
				if (wcscmp(text.get(), (wchar_t const *)L"") == 0) {
					text = Control_Tooltip((unsigned int)parent, window);
                }
            }
			SendMessageA(status, 0x4B2, 0, (LPARAM)text.text());
        }
    }

	if (is_paint && data) {

		/*
		 * Paint path: when painting is disabled, just tell the control to repaint
		 * its frame.
		 */
		if (data->SkipCheckPaint) {
			ValidateRect(window, 0);
			result = CallWindowProcA(specific_proc, window, 0x4a5, wparam, lparam);
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
						if (ODTooltip.isActive && ODTooltip.isHidden != 1 && ODTooltip.background != 0) {
							Rect drect = ODTooltip.bounds;
							Rect srect(0, 0, ODTooltip.bounds.Width, ODTooltip.bounds.Height);
							VisibleSurface->Blit_From(drect, *ODTooltip.background, srect, false, true);
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
			HWND owner = window;
			while (owner != 0) {
				if (GetWindowLongA(owner, 0x4) != 0) {
					break;
				}
				owner = GetParent(owner);
			}

			WinData * ownerdata = 0;
			if (owner != 0) {
				ownerdata = Get_Record(owner);
			}

			int state;
			if (owner == window) {
				if ((*(int *)((char *)ownerdata + 0x1FC)) < 1) {
					state = 1;
					anim_state = 1;
					goto call_proc_paint;
				}
				state = (*(int *)((char *)ownerdata + 0x1FC));
			} else {
				if (ownerdata == 0) {
					goto skip_paint;
				}
				state = (*(int *)((char *)ownerdata + 0x1FC));
			}

			anim_state = state;
			if (state < 1) {
			skip_paint:
				state = anim_state;
				ValidateRect(window, 0);
				result = 1;
			} else {

			call_proc_paint:
				result = CallWindowProcA(specific_proc, window, message, wparam, lparam);
				if (ownerdata != 0) {
					(*(int *)((char *)ownerdata + 0x1FC)) = state;
				}

				{
					ArrayList<HWND> children;
					EnumChildWindows(window, (WNDENUMPROC)Add_Window, (LPARAM)&children);

					HWND combo_owner = 0;
					HWND child = 0;
					for (int index = 0; index < children.length(); index++) {
						children.get(child, index);

						WNDPROC childproc = 0;
						::Window_Proc_Table.getValue(child, childproc);

						if (childproc != (WNDPROC)Combo_Drop_Proc) {
							InvalidateRect(child, 0, 0);
							UpdateWindow(child);
						} else {
							combo_owner = child;
						}
					}

					if (combo_owner != 0) {
						InvalidateRect(combo_owner, 0, 0);
						UpdateWindow(combo_owner);
					} else if (Dropdown_Window != 0) {
						if (Dropdown_Owner == owner) {
							Rect drop_rect;
							Get_Display_Rect(Dropdown_Window, (RECT *)&drop_rect);
							RECT intersect;
							if (IntersectRect(&intersect, &disp_rect, (const RECT *)&drop_rect)) {
								InvalidateRect(Dropdown_Window, 0, 0);
								UpdateWindow(Dropdown_Window);
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
					if (sibling == 0) {
						break;
					}
					sibling = GetWindow(sibling, 0x3);
					if (sibling == 0) {
						break;
					}
					if (GetWindowLongA(sibling, 0x4)) {
						Get_Display_Rect(sibling, (RECT *)&sibling_rect);
						if (IntersectRect(&intersect, &disp_rect, (const RECT *)&sibling_rect)) {
							InvalidateRect(sibling, 0, 0);
							UpdateWindow(sibling);
							break;
						}
					}
				}
			}
		}
		goto after_proc;
	} else {
		if (allow_proc) goto call_custom_proc;
        goto after_proc;
	}

call_custom_proc:
	if (specific_proc != 0) {
		result = CallWindowProcA(specific_proc, window, message, wparam, lparam);
	}

after_proc:
	if (message == WM_NCDESTROY) {
		HWND destroyed = window;
		WinData * destroydata;
		unsigned int destroyed_key = (unsigned int)destroyed;
		if (::Control_Table.getPointer(destroyed_key, &destroydata)) {
			if (destroydata != 0 && destroydata->Back != 0) {
				delete destroydata->Back;
				destroydata->Back = 0;
				Surface_Count--;
			}
		}
		Restore_Child(window, 0);
	}

cleanup:
    ctrlmessages.remove(key);
	if (data && is_paint) {
		if (GetWindowLongA(window, 4) && num_rect_updates > 1) *(int *)((char *)data + 0x1FC) = 2;
        if (--num_rect_updates == 0) {
			if (!data->SkipCheckPaint && anim_state >= 1) {
				Rect source(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y);
				Rect destination(source);
				if (*(int *)((char *)data + 0x1FC) == 1) {
                    *(int *)((char *)data + 0x1FC) = 2;
					if (GetWindowLongA(window, 4) && Play_Dialog_Move_In(window)) *(int *)((char *)data + 0x1FC) = 3;
					Rect whole(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y);
					destination = whole;
					destination += offset;
                    VisibleSurface->Lock();
                    AlternateSurface->Lock();
					VisibleSurface->Blit_From(destination, *AlternateSurface, whole, false, true);
                    AlternateSurface->Unlock();
                    VisibleSurface->Unlock();
                    ArrayList<HWND> children;
					EnumChildWindows(window, (WNDENUMPROC)Add_Window, (LPARAM)&children);
					HWND child = 0;
                    for (int index = 0; index < children.length(); index++) {
                        children.get(child, index);
						SendMessageA(child, 0x4AF, 0, 0);
                    }
                } else {
					destination = source;
					destination += offset;
                    char class_name[128];
					GetClassNameA(window, class_name, sizeof(class_name));
                    if (strcmp(class_name, "ComboBox") == 0) {
						Inflate_Rect(source, -1, -1);
						Inflate_Rect(destination, -1, -1);
                    }
                    VisibleSurface->Lock();
                    AlternateSurface->Lock();
					VisibleSurface->Blit_From(destination, *AlternateSurface, source, false, true);
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
	if (show_tooltip) Show_Tooltip(true);
	if (message == WM_INITDIALOG) {
        result = 0;
    }
	return result;
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
namespace OwnerDraw {
LRESULT __stdcall Default_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WNDPROC proc = 0;
	Window_Proc_Table.getValue(window, proc);
	RECT area;
	Get_Display_Rect(window, &area);
    RECT client;
    GetClientRect(window, &client);
	WinData * record = 0;
	Control_Table.getPointer((unsigned int const &)window, &record);
	if (message == WM_CTLCOLOREDIT) {
		SetTextColor((HDC)wparam, Text_Color);
		SetBkMode((HDC)wparam, TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);
    }
	return CallWindowProcA(proc, window, message, wparam, lparam);
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
ConvertClass * __cdecl Get_Sidebar_Art_Drawer(void); // RA2: 0x6F45F0, the side's button palette (ra2-decomp's GetSideButtonConvert)
ConvertClass * __cdecl Get_Main_Button_Drawer(void); // RA2: 0x6F2250
ConvertClass * __cdecl Get_Button_Anim_Drawer(void); // RA2: 0x6F34C0

namespace OwnerDraw {

/*
**	RA2: the window procedure of the owner-drawn (BS_OWNERDRAW) buttons: plain buttons from the b?e_ art,
**	or the campaign, network and cancel button shapes by the record's button kind, which can flash.
**	The text is ra2-decomp's matched twin (OwnerDrawButtonHandler, Yuri's Revenge 0x00612B70) in this
**	tree's names; RA2 has two sides where the twin has three and converts the pixel through Pixel_To_Color.
*/
LRESULT __stdcall Owner_Button_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WNDPROC previous = NULL;
	Window_Proc_Table.getValue(window, previous);

	RECT display;
	memset(&display, 0, sizeof(display));
	Get_Display_Rect(window, &display);

	Rect origin;
	origin.X = display.left;
	origin.Y = display.top;
	origin.Width = display.right - display.left;
	origin.Height = display.bottom - display.top;

	RECT client;
	memset(&client, 0, sizeof(client));
	GetClientRect(window, &client);

	WinData * data = Get_Record(window);
	if (data == NULL) {
		return(CallWindowProcA(previous, window, message, wparam, lparam));
	}

	int state = data->ItemState;
	LONG style = GetWindowLongA(window, GWL_STYLE);

	switch (message) {
		case WM_TIMER:
			data->IsFlashLit = !data->IsFlashLit;
			InvalidateRect(window, NULL, TRUE);
			break;

		case WM_USER + 220: { // RA2: lparam 1 starts the flashing, anything else stops it
			Control_Table.getPointer((unsigned int const &)window, &data);
			bool on = lparam == 1;
			if (on && !data->IsFlashing) {
				data->IsFlashing = true;
				SetTimer(window, 0, 1000, NULL);
			} else if (!on && data->IsFlashing) {
				data->IsFlashing = false;
				data->IsFlashLit = false;
				KillTimer(window, 0);
				InvalidateRect(window, NULL, TRUE);
			}
			break;
		}

		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if (data->IsDisabled) {
				return(0);
			}
			VocClass::PlayGlobal(Rule->GUIMainButtonSound, 1.0f, 0x2000, 0);
			break;

		case WM_ACTIVATE:
		case WM_KILLFOCUS:
		case WM_MOUSEACTIVATE:
			return(0);

		case WM_PAINT: {
			if (data->IsDisabled) {
				ValidateRect(window, NULL);
				return(0);
			}

			Rect rect = origin;
			unsigned color = Text_Color;
			Surface * image;

			if (data->Back == NULL) {
				Surface * surface = new BSurface(client.right + 1, client.bottom + 1, 2);
				data->Back = surface;
				Surface_Count++;

				Rect dest(0, 0, client.right + 1, client.bottom + 1);
				Rect src(display.left, display.top, client.right + 1, client.bottom + 1);

				surface->Blit_From(dest, *AlternateSurface, src);
			}

			if (data->ButtonKind != 0) {
				int kind = data->ButtonKind; // RA2: the kind read once (flagged: the twin tests the member each time)
				ConvertClass * convert = NULL;
				ShapeSet const * shape = NULL;
				int frame = 0;

				if (kind == 1) {
					convert = Get_Button_Anim_Drawer();
					shape = Slide_Button_Shape;
					int f = 2;
					if (state & 1) {
						f = 4;
					} else if (data->IsFlashLit) {
						f = 3;
					}
					frame = f;
				} else if (kind == 2) {
					convert = Get_Sidebar_Art_Drawer();
					shape = Button_Strip_Shape;
					int f = 0;
					if (state & 1) {
						f = 1;
					} else if (data->IsFlashLit) {
						f = 2;
					}
					frame = f;
				} else if (kind == 3) {
					convert = Get_Main_Button_Drawer();
					shape = (ShapeSet const *)ArtMainButton;
					int f = 0;
					if (state & 1) {
						f = 1;
					} else if (data->IsFlashLit) {
						f = 2;
					}
					frame = f;
				}

				if (style & WS_DISABLED) {
					int pixel;
					if (Session.Is_In_Game()) {
						if (Scen->IsGDI) { // RA2: two sides
							pixel = DSurface::Build_Hicolor_Pixel(TealColor);
						} else {
							pixel = DSurface::Build_Hicolor_Pixel(DarkRedColor2);
						}
					} else {
						pixel = DSurface::Build_Hicolor_Pixel(DarkRedColor1);
					}
					color = Pixel_To_Color((unsigned short)pixel);
				}

				Point2D point = Point_Copy((Point2D const *)&rect);
				Rect window_rect = AlternateSurface->Get_Rect();
				if (convert && shape) {
					Draw_Shape(*AlternateSurface, *convert, shape, frame, point, window_rect, SHAPE_WIN_REL);
				}

			} else if (data->Image != NULL) {
				image = data->Image;
				if ((state & 1) && data->AlternateImage != NULL) {
					image = data->AlternateImage;
				}

				Rect src = rect;
				src.X = 0;
				src.Y = 0;
				AlternateSurface->Blit_From(rect, *image, src);

			} else {
				static char last_look = 'u';

				char updown = 'u';
				if (state & 1) {
					updown = 'd';
				}
				if (style & WS_DISABLED) {
					updown = 'u';
				} else if (updown == 'd' && last_look == 'u') {
					VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, 0);
				}
				last_look = updown;

				int widths[2] = {7, 7};
				int margins[2] = {10, 10};
				int heights[2] = {24, 30};

				int index = 0;
				for (unsigned int i = 0; i < 2; i++) {
					if (heights[i] > rect.Height && i != 0) {
						break;
					}
					index = i;
				}

				int height = heights[index];
				int w = widths[index];
				int margin = margins[index];

				if (data->Back != NULL) {
					Rect cachesrc(0, 0, client.right + 1, client.bottom + 1);
					Rect cachedest(display.left, display.top, client.right + 1, client.bottom + 1);
					AlternateSurface->Blit_From(cachedest, *data->Back, cachesrc);
					InvalidateRect(window, NULL, FALSE);
				}

				origin.Y += (rect.Height - height) / 2;
				if (state & 1) {
					origin.Y += 2;
				}

				Rect destrect;
				Rect sourcerect;
				char buffer[40];

				sprintf(buffer, "b%c%c_li%d.pcx", updown, 'e', height);
				image = SurfaceCache.GetSurface(buffer);
				origin.Height = image->Get_Height();
				destrect = origin;
				destrect.Width = w;
				sourcerect.Y = 0;
				sourcerect.X = 0;
				destrect.Height = height;
				sourcerect.Width = w;
				sourcerect.Height = height;
				AlternateSurface->Blit_From(destrect, *image, sourcerect);

				sprintf(buffer, "b%c%c_mi%d.pcx", updown, 'e', height);
				image = SurfaceCache.GetSurface(buffer);
				Rect midrect = origin;
				midrect.X += w;
				midrect.Width -= margin;
				midrect.Height = image->Get_Height();
				SurfaceCache.Draw(midrect, *AlternateSurface, *image, 0, 0);

				sprintf(buffer, "b%c%c_ri%d.pcx", updown, 'e', height);
				image = SurfaceCache.GetSurface(buffer);
				destrect = origin;
				destrect.X += origin.Width - margin;
				destrect.Width = margin;
				destrect.Height = image->Get_Height();
				sourcerect.Height = destrect.Height;
				sourcerect.Y = 0;
				sourcerect.X = 0;
				sourcerect.Width = destrect.Width;
				AlternateSurface->Blit_From(destrect, *image, sourcerect);
			}

			if (data->Image == NULL && data->Text) {
				RECT unused_client;
				GetClientRect(window, &unused_client);

				Rect text_rect(
					origin.X,
					origin.Y + 1,
					origin.Width + origin.X - 2,
					origin.Height + origin.Y);
				if (state & 1) {
					text_rect.X += 2;
					text_rect.Y += 4;
				}
				Draw_Text_In_Rect_Ex(AlternateSurface, data->Text, (RECT *)&text_rect, data->Font, color, 5, 0xc, 0, 0, 0);
			}

			if (data->ButtonKind == 0 && (style & WS_DISABLED)) {
				Blend_Rect(&rect, AlternateSurface, 0, 128);
			}
			ValidateRect(window, NULL);
			return(0);
		}
	}

	return(CallWindowProcA(previous, window, message, wparam, lparam));
}

}


/// <summary>
/// Handles the messages for an owner-drawn tab control.
/// The body of the control is painted as dimmed dialog background and each tab is built
/// from its corner and middle artwork, with the caption drawn over it in the remapped
/// font. The active tab gets the brighter text color.
/// </summary>
/// <returns>Returns with the result of the original window procedure for anything this
/// routine does not paint itself.</returns>
namespace OwnerDraw {

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

LRESULT __stdcall Tab_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
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
namespace OwnerDraw {
void __fastcall Draw_Edit_Text(Surface *, Rect &, wchar_t const *, int,
	BitFont *, unsigned int, int *, int, int, int, int);
LRESULT __stdcall Edit_Control_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WNDPROC previous = 0;
	Window_Proc_Table.getValue(window, previous);
	WinData * data = 0;
	Control_Table.getPointer((unsigned int)window, &data);
    char class_name[32];
	char narrow[0x800];
	unsigned short wide[0x800];
	if (!data) return CallWindowProcA(previous, window, message, wparam, lparam);
	if (GetFocus() == window && !data->IsEditFocus) {
		data->Position = 1;
        SetFocus(MainWindow);
    }
	LONG style = GetWindowLongA(window, GWL_STYLE);
	if ((message == WM_KEYUP || message == WM_KEYDOWN) && wparam == 9) return 0;
	if (message == OD_SUBCLASSED) {
		RECT winrect, client, parentrect;
		GetWindowRect(window, &winrect);
		GetClientRect(window, &client);
		GetWindowRect(GetParent(window), &parentrect);
		MoveWindow(window, winrect.left - parentrect.left + 1, winrect.top - parentrect.top + 1,
			client.right - 2, client.bottom - 2, 0);
		if (GetFocus() == window) {
			data->Position = 1;
            SetFocus(MainWindow);
        }
		if (style & WS_TABSTOP) {
			data->HadTabStop = 1;
			SetWindowLongA(window, GWL_STYLE, style & ~WS_TABSTOP);
        }
		return 0;
    }
	if (message == WM_SETFOCUS) {
		SendMessageA(window, EM_SETSEL, (WPARAM)-1, -1);
		if (!data->IsEditFocus) PostMessageA(window, OD_REFOCUS, 0, 0);
		goto invalidate;
    }
	if (message == 0x4b5 || message == 0x4b3 || message == WM_GETTEXTLENGTH) {
		wide[0] = 0;
		Edit_Text(window, wide, 0x800, 0);
		if (message == WM_GETTEXTLENGTH) return wcslen(wide);
		if (message == 0x4b3) {
			wcsncpy((unsigned short *)lparam, wide, wparam);
			((unsigned short *)lparam)[wparam - 1] = 0;
			return wcslen((unsigned short *)lparam);
        }
		WideToCharString((char *)lparam, wide, wparam);
		((char *)lparam)[wparam - 1] = 0;
		return strlen((char *)lparam);
    }
	if (message == 0x4b4 || message == OD_SETTEXTW) {
		narrow[0] = 0;
		if (data->Text) WideCharToMultiByte(CP_ACP, 0, data->Text, -1, narrow, 0x800, 0, 0);
		return CallWindowProcA(previous, window, WM_SETTEXT, 0, (LPARAM)narrow);
    }
	if (message == WM_CHAR) {
		if (wparam == 0xd) {
			if (style & ES_MULTILINE) {
				unsigned int length = CallWindowProcA(previous, window, WM_GETTEXTLENGTH, 0, 0) + 3;
				char * text = new char[length];
				CallWindowProcA(previous, window, WM_GETTEXT, length, (LPARAM)text);
				strcat(text, "\r\n");
				CallWindowProcA(previous, window, WM_SETTEXT, 0, (LPARAM)text);
				delete[] text;
				unsigned int id = GetWindowLongA(window, GWL_ID) & 0xffff;
				SendMessageA(GetParent(window), WM_COMMAND, id | 0x5010000, (LPARAM)window);
				return 0;
            }
		} else if (wparam == 9) {
			HWND next = window;
			HWND parent = GetParent(next);
			HWND tab = GetNextDlgTabItem(parent, next, 0);
			if (tab) next = tab;
            SetFocus(next);
			return 0;
        }
		goto call_previous;
    }
	if (message == OD_ACTIVATE) {
		int pending = data->Position;
		data->IsEditFocus = 1;
		if (pending) {
            SetFocus(window);
			data->Position = 0;
        }
		if (data->HadTabStop) SetWindowLongA(window, GWL_STYLE, style | WS_TABSTOP);
		return 0;
    }
	if (message == WM_PAINT || message == WM_ERASEBKGND) {
		RECT display, update;
		Get_Display_Rect(window, &display);
		GetClassNameA(GetParent(window), class_name, 16);
		bool combo = strcmp(class_name, "ComboBox") == 0;
		if (message == WM_PAINT && GetUpdateRect(window, &update, 0)) {
			update.left += display.left;
			update.right += display.left;
			update.top += display.top;
			update.bottom += display.top;
        }
		Rect background(display.left, display.top, display.right - display.left + 1, display.bottom - display.top + 1);
		Draw_Dimmed_Back(background, window, data->TextStyle);
		if (!combo) Draw_Frame(AlternateSurface, background, 1, -1);
		int caret = 0;
		Edit_Text(window, Edit_Buffer, 0x1400, &caret);
		Rect text(display.left, display.top, display.right - display.left + 1, display.bottom - display.top + 1);
		LONG text_style = GetWindowLongA(window, GWL_STYLE);
		Draw_Edit_Text(AlternateSurface, text, Edit_Buffer, caret,
			data->Font, Text_Color, &data->EditScroll, data->ImeState, ((unsigned int)text_style >> 5) & 1, 0, 0);
		ValidateRect(window, 0);
    }
	if (message == WM_CONTEXTMENU) return 1;
	if (message == WM_MOUSEMOVE) return 1;
	if (message == WM_KEYDOWN || message == WM_KEYUP || message == WM_SYSKEYDOWN || message == WM_SYSKEYUP || message == WM_SYSCHAR || message == WM_SYSDEADCHAR || message == WM_KILLFOCUS || message == WM_LBUTTONDOWN) {
	invalidate:
		GetClassNameA(GetParent(window), class_name, 32);
		if (strcmp(class_name, "ComboBox") == 0) InvalidateRect(GetParent(window), 0, 0);
		InvalidateRect(window, 0, 0);
    }
call_previous:
	return CallWindowProcA(previous, window, message, wparam, lparam);
}
}


namespace OwnerDraw {

int __fastcall Edit_Text(HWND window, wchar_t * text, int size, int * caret)
{
	WNDPROC proc = 0;
	Window_Proc_Table.getValue(window, proc);
	char buffer[1024];
	int start = 0;
	buffer[0] = 0;
	CallWindowProcA(proc, window, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer);
	if (caret) {
		DWORD selection_start;
		DWORD selection_end;
		start = (unsigned long)CallWindowProcA(proc, window, EM_GETSEL, (WPARAM)&selection_start, (LPARAM)&selection_end) >> 16;
    }
    *text = 0;
	MultiByteToWideChar(CP_ACP, 0, buffer, -1, text, size);
	if (caret) {
        *caret = _mbsnbcnt((unsigned char *)buffer, start);
    }
	if (size > 0) {
		text[size - 1] = 0;
    }
	int length = wcslen(text);
	wchar_t * copy = new wchar_t[length + 3];
	memset(copy, 0, (length + 3) * sizeof(wchar_t));
	int count = 0;
    int i = 0;
	for (; i < length; i++) {
		if (text[i] != '\r' && text[i] != '\n') {
			copy[count++] = text[i];
        }
    }
	if (i != count) {
		wcscat(copy, L"\r\n");
    }
	wcscpy(text, copy);
	delete[] copy;
	return wcslen(text);
}

}


unsigned short __fastcall IME_Char_To_Wide(unsigned int wparam, long lparam); // RA2: 0x738130 (winstub.cpp)

namespace OwnerDraw {


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


}


#include "movie.h" // RA2: the includes this file needs
#include "movies.h" // RA2: the includes this file needs
MovieHandle * __fastcall OwnerDraw_InitMovieHandle(char const * name, Surface * surface, Rect * bounds); // RA2: 0x5A1D30 (movie.cpp)

/*
**	RA2: a static control's view of its record: picture modes, typed-out text, shape animation and movies.
*/
struct StaticRecord {
		char Reserved_00[0x10];
	Surface * Back;					// RA2: +0x10 what lies under the control
	Surface * Image;				// RA2: +0x14 picture mode 2
		char Reserved_18[0x28 - 0x18];
	wchar_t * Text;					// RA2: +0x28
		char Reserved_2C[0x58 - 0x2C];
	MovieHandle * Movie;			// RA2: +0x58 playing in the control
	int IsLooping;					// RA2: +0x5C the movie restarts when it ends
	MovieHandle * Movie2;			// RA2: +0x60 released with it
	BitFont * Font;			// RA2: +0x64
		char Reserved_68[0x70 - 0x68];
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
		char Reserved_B0[0xB4 - 0xB0];
	bool IsFilled;					// RA2: +0xB4 a solid background
	COLORREF FillColor;				// RA2: +0xB8
	bool IsDisabled;				// RA2: +0xBC
		char Reserved_BD[0xEC - 0xBD];
	COLORREF TextColor;				// RA2: +0xEC
};

namespace OwnerDraw {

/*
**	RA2: stops the control's movies (and its timer).
*/
static void Stop_Static_Movie(HWND window, StaticRecord * data)
{
	if (data->Movie) {
		delete data->Movie;
		data->Movie = 0;
	}
	KillTimer(window, 0x65);
	if (data->Movie2) {
		delete data->Movie2;
		data->Movie2 = 0;
	}
}

/*
**	RA2: the window procedure of the static controls: text (typed out), an image, a shape or its
**	animation, or a movie.
*/
#ifdef RA2_NON_MATCHING // not exact: 3056 bytes differ against the image-derived bound 005f2b50 (4512 body bytes, image_bounds.csv; 4326 before, 3067 before the published Yuri's Revenge StaticHandler's frame step, ++frame in the paint case): the record through ra2-decomp's Get_Data (its sibling dialog procedures, flagged: another site), the cases in the image's dispatch order with the real Control_Frame_Delay and Control_Animation_Rate calls, both movie starts written out
LRESULT __stdcall Static_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	StaticRecord * data = (StaticRecord *)Get_Data(window);
	if (data == 0) {
		return(0);
	}

	switch (message) {
		case WM_DESTROY:
			if (data->Back) {
				delete data->Back;
				data->Back = 0;
			}
			if (data->IsShapeAllocated) {
				operator delete(data->Shape);
				data->Shape = 0;
			}
			if ((data->PictureMode == 1 && data->IsPlaying) || data->PictureMode == 4) KillTimer(window, 0);
			Stop_Static_Movie(window, data);
			// RA2: destruction continues through the previous window procedure.
		default: {
			WNDPROC proc = 0;
			Window_Proc_Table.getValue(window, proc);
			return(CallWindowProcA(proc, window, message, wparam, lparam));
		}


		case WM_MOVE:
		case WM_SIZE:
		case WM_WINDOWPOSCHANGED:
			if (data->Back) {
				delete data->Back;
			}
			data->Back = 0;
			InvalidateRect(window, 0, FALSE);
			break;

		case WM_PAINT: {
			if (data->IsDisabled) {
				ValidateRect(window, 0);
				break;
			}
			if (data->Movie) {
				ValidateRect(window, 0);
				break;
			}
			if (!data->Back) {
				RECT display;
				Get_Display_Rect(window, &display);
				RECT client;
				GetClientRect(window, &client);
				Surface * surface = new BSurface(client.right + 1, client.bottom + 1, 2);
				data->Back = surface;
				++Surface_Count;
				Rect destination(0, 0, client.right + 1, client.bottom + 1);
				Rect source(display.left, display.top, client.right + 1, client.bottom + 1);
				surface->Blit_From(destination, *AlternateSurface, source, false, true);
			}
			RECT area;
			Get_Display_Rect(window, &area);
			if (data->IsFilled) {
				Rect fill(area.left, area.top, area.right - area.left, area.bottom - area.top);
				AlternateSurface->Fill_Rect(fill, DSurface::Build_Hicolor_Pixel(GetRValue(data->FillColor), GetGValue(data->FillColor), (data->FillColor >> 16) & 0xff));
			}
			int mode = data->PictureMode;
			bool plain = mode == 0;
			bool typed = (mode == 1);
			if (plain || typed) {
				if (data->Text && (plain || (typed && data->IsPlaying))) {
					LONG style = GetWindowLongA(window, GWL_STYLE);
					int flags = 16;
					if (style & SS_CENTER) {
						flags = 17;
					} else if (style & SS_RIGHT) {
						flags = 18;
					}
					COLORREF color = data->TextColor;
					if (style & WS_DISABLED) {
						color = Disabled_Text_Color;
					}
					Draw_Text_In_Rect_Ex(AlternateSurface, data->Text, &area, data->Font, color, flags, data->TextFlags, 0, data->Typed, data->TypeExtra);
					if (data->TypingSound != -1) {
						VocClass::PlayGlobal(data->TypingSound, 1.0f, 0x2000, 0);
					}
					if (typed) {
						int length = wcslen(data->Text) + 1 + data->TypeExtra;
						if (data->Typed < length) {
							data->Typed += data->TypeStep;
							if (data->Typed >= length) {
								KillTimer(window, 0);
							}
						}
					}
				}
			} else {
				bool image_mode = mode == 2;
				bool shape_mode = mode == 3;
				bool animated = (mode == 4);
				if ((image_mode || shape_mode || animated) && (int)(GetTickCount() - data->StartTime) > data->Delay) {
					Rect bounds(area.left, area.top, area.right - area.left, area.bottom - area.top);
					RECT client;
					GetClientRect(window, &client);
					Rect source(client.left, client.top, client.right - client.left, client.bottom - client.top);
					AlternateSurface->Blit_From(bounds, *data->Back, source, false, true);
					if (image_mode) {
						Surface * image = data->Image;
						if (image) {
							int width = image->Get_Width();
							int height = image->Get_Height();
							Rect rect = bounds;
							if (width < bounds.Width) {
								rect.X = (bounds.Width - width) / 2 + bounds.X;
								rect.Width = width;
							}
							if (height < bounds.Height) {
								rect.Y = bounds.Y + (bounds.Height - height) / 2;
								rect.Height = height;
							}
							SurfaceCache.DrawTrans(rect, *AlternateSurface, *image, (short)DSurface::Build_Hicolor_Pixel(255, 0, 255));
							ValidateRect(window, 0);
							break;
						}
					} else if ((shape_mode || animated) && data->Shape) {
						ShapeSet * shape = data->Shape;
						ConvertClass * drawer = data->Drawer;
						int height = shape->Get_Height();
						int width = shape->Get_Width();
						Rect rect = bounds;
						if (rect.Width > width) {
							rect.X += (rect.Width - width) / 2;
							rect.Width = width;
						}
						if (rect.Height > height) {
							rect.Y += (rect.Height - height) / 2;
							rect.Height = height;
						}
						Point2D point(rect.X, rect.Y);
						Rect window_rect = AlternateSurface->Get_Rect();
						int frame = data->Frame;
						Draw_Shape(*AlternateSurface, *drawer, shape, frame, point, window_rect, SHAPE_WIN_REL, 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0));
						if (animated && data->IsPlaying) {
							if (++frame >= data->FrameCount) {
								frame = 0;
							}
							data->Frame = frame;
							if (data->Notify) {
								SendMessageA(data->Notify, WM_USER + 216, frame, (LPARAM)window); // RA2: the new frame
								ValidateRect(window, 0);
								break;
							}
						}
					}
				}
			}
			ValidateRect(window, 0);
			break;
		}

		case WM_TIMER:
			if (wparam == 0x65) {
				if (!data->Movie) {
					break;
				}
				if (data->Movie->AdvanceFrame()) {
					InvalidateRect(window, 0, FALSE);
				}
				if (!data->Movie->FramesLeft()) {
					break;
				}
				if (data->IsLooping) {
					data->Movie->SeekToFrame(1);
					DebugString("Looping movie\n");
					break;
				}
				Stop_Static_Movie(window, data);
				break;
			} else {
				int mode = data->PictureMode;
				bool once;
				if (mode == 1) {
					InvalidateRect(window, 0, TRUE);
					break;
				}
				bool animated = mode == 4;
				if (mode == 2 || mode == 3) {
					once = true;
				} else {
					once = false;
					if (!animated) break;
				}
				if ((int)(GetTickCount() - data->StartTime) <= data->Delay) {
					break;
				}
				if (once || (animated && data->IsPlaying)) {
					InvalidateRect(window, 0, TRUE);
					if (once) {
						KillTimer(window, 0);
						break;
					}
				}
				if (data->FrameCount) {
					break;
				}
				KillTimer(window, 0);
				if (!data->Shape) {
					break;
				}
				data->FrameCount = data->Shape->Get_Count();
				SetTimer(window, 0, data->FrameCount, 0);
				break;
			}

		case OD_SUBCLASSED: {
			WNDPROC proc = 0;
			Window_Proc_Table.getValue(window, proc);
			HWND parent = GetParent(window);
			data->PictureMode = 0;
			data->TextFlags = Control_Frame_Delay((unsigned int)parent, window);
			data->TextColor = Text_Color;
			break;
		}

		case WM_USER + 211: { // RA2: starts the animation
			if (!data->Shape || data->PictureMode != 4 || data->IsPlaying) {
				break;
			}
			data->IsPlaying = true;
			SetTimer(window, 0, Control_Animation_Rate((unsigned int)GetParent(window), window), 0);
			break;
		}

		case OD_SETCOLOR:
			if ((COLORREF)lparam != data->TextColor) {
				InvalidateRect(window, 0, FALSE);
			}
			if (lparam == -1) {
				data->TextColor = Text_Color;
				break;
			}
			data->TextColor = lparam;
			break;

		case WM_USER + 177: // RA2: a solid background colour
			data->IsFilled = true;
			data->FillColor = lparam;
			break;

		case OD_SETTEXTW:
		case WM_USER + 180: { // RA2: the text changed: the background is put back
			Surface * back = data->Back;
			if (back) {
				RECT display;
				Get_Display_Rect(window, &display);
				RECT client;
				GetClientRect(window, &client);
				Rect destination, source;
				source.X = 0;
				source.Y = 0;
				source.Width = client.right + 1;
				source.Height = client.bottom + 1;
				destination.X = display.left;
				destination.Y = display.top;
				destination.Width = client.right + 1;
				destination.Height = client.bottom + 1;
				AlternateSurface->Blit_From(destination, *back, source, false, true);
				InvalidateRect(window, 0, FALSE);
			}
			return(1);

				}

		case WM_USER + 212: // RA2: stops the animation
			if (data->PictureMode != 4 || !data->IsPlaying) {
				break;
			}
			KillTimer(window, 0);
			data->IsPlaying = false;
			break;

		case WM_USER + 238: // RA2: starts typing the text out
			if (data->PictureMode == 1 && !data->IsPlaying) {
				data->IsPlaying = true;
				data->Typed = 1;
				SetTimer(window, 0, data->TypeRate, 0);
				InvalidateRect(window, 0, FALSE);
			}
			break;

		case WM_USER + 213: // RA2: sets the animation frame
			if (data->PictureMode == 4) {
				data->Frame = lparam;
				InvalidateRect(window, 0, TRUE);
			}
			break;

		case WM_USER + 214: // RA2: the animation frame
			if (data->PictureMode != 4) return(-1);
			return(data->Frame);

		case WM_USER + 215: // RA2: the window told of new frames
			if (data->PictureMode == 4) {
				data->Notify = (HWND)lparam;
			}
			break;

		case WM_USER + 223: // RA2: plays the movie of the Movies list at wparam
			if ((int)wparam >= 0 && (int)wparam < Movies.Count()) {
				char const * name = Movies[wparam];
				Stop_Static_Movie(window, data);
				RECT display;
				Get_Display_Rect(window, &display);
				data->Movie = OwnerDraw_InitMovieHandle(name, AlternateSurface, 0);
				if (data->Movie) {
					data->Movie->SetPosition(display.left, display.top);
					MoveWindow(window, display.left, display.top, data->Movie->Width, data->Movie->Height, FALSE);
					SetTimer(window, 0x65, 0x22, 0);
				} else {
					KillTimer(window, 0x65);
					if (data->Movie2) {
						delete data->Movie2;
						data->Movie2 = NULL;
					}
				}
			}
			break;

		case WM_USER + 228: { // RA2: plays the named movie
			Stop_Static_Movie(window, data);
			RECT display;
			Get_Display_Rect(window, &display);
			data->Movie = OwnerDraw_InitMovieHandle((char const *)lparam, AlternateSurface, 0);
			if (data->Movie) {
				data->Movie->SetPosition(display.left, display.top);
				MoveWindow(window, display.left, display.top, data->Movie->Width, data->Movie->Height, FALSE);
				SetTimer(window, 0x65, 0x22, 0);
			} else {
				KillTimer(window, 0x65);
				if (data->Movie2) {
					delete data->Movie2;
					data->Movie2 = NULL;
				}
			}
			break;
		}

		case WM_USER + 226: // RA2: stops the movie
			Stop_Static_Movie(window, data);
			break;

		case WM_USER + 240: // RA2: draws the movie's frame
			if (data->Movie) {
				data->Movie->Blit();
			}
			break;

		case WM_USER + 224: // RA2: pauses the movie
			if (data->Movie) {
				data->Movie->Pause(true);
			}
			break;

		case WM_USER + 225: // RA2: resumes the movie
			if (data->Movie) {
				data->Movie->Pause(false);
			}
			break;

		case WM_USER + 227: // RA2: the movie loops
			data->IsLooping = wparam;
			break;
	}

	return(0);
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
namespace OwnerDraw {
LRESULT __stdcall Checkbox_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	unsigned int key = (unsigned int)window;
	WinData * record = 0;
	WinData *& data = record;
	Control_Table.getPointer(key, &data);
	if (data) {
		switch (message) {
			case WM_PAINT:
				if (data->SkipCheckPaint) break;
        {
            RECT client;
					struct PaintBounds { RECT display; RECT text; } bounds;
					GetClientRect(window, &client);
					Get_Display_Rect(window, &bounds.text);
					Get_Display_Rect(window, &bounds.display);
					Rect destination;
					destination = Rect(bounds.display.left, bounds.display.top, bounds.display.right - bounds.display.left, bounds.display.bottom - bounds.display.top);
					destination.Width = 18;
					destination.Height = 18;
					LONG style = GetWindowLongA(window, GWL_STYLE);
					int state = data->ItemState;
					bool special = data->IsCheckAltArt;
					bool alternate = data->IsCheckAltSelected;
					bool checked = state == BST_CHECKED;
					char image_name[64];
					if (special) {
						if (checked) {
							if (alternate)strcpy(image_name, "cce_i.pcx");
							else strcpy(image_name, "cce_il.pcx");
                } else {
							if (alternate)strcpy(image_name, "cce_ir.pcx");
							else strcpy(image_name, "cue_i.pcx");
                }
            } else {
						char letter = 'u';
						if (checked) letter = 'c';
						sprintf(image_name, "c%ce_i.pcx", letter);
            }
					Surface * image = SurfaceCache.GetSurface(image_name, 0);
					int height = image->Get_Height();
					Rect source(0, 0, image->Get_Width(), height);
					AlternateSurface->Blit_From(destination, *image, source, false, true);
					if (style & WS_DISABLED) Blend_Rect(&destination, AlternateSurface, 0, Dim_Alpha);
					wchar_t * text = data->Text;
					bounds.text.left += 26;
					if (text) {
						unsigned int color = Text_Color;
						if (style & WS_DISABLED) color = Disabled_Text_Color;
						Draw_Text_In_Rect_Ex(AlternateSurface, text, &bounds.text, data->Font, color, 4, 0xc, 0, 0, 0);
            }
					ValidateRect(window, 0);
        }
			zero_result:
				return 0;
			case BM_GETCHECK: return data->ItemState;
			case BM_SETCHECK:
				data->ItemState = wparam;
				InvalidateRect(window, 0, 0);
				return 0;
			case WM_SETFOCUS:case WM_KILLFOCUS: InvalidateRect(window, 0, 0); break;
			case WM_LBUTTONDOWN:case WM_LBUTTONDBLCLK: {
				Point2D click((unsigned int)lparam & 0xffff, (unsigned int)lparam >> 16);
				if (click.X >= 18) goto zero_result;
				if (click.Y >= 18) goto zero_result;
				int checked = data->ItemState != BST_CHECKED;
				data->ItemState = checked;
				InvalidateRect(window, 0, 0);
				VocClass::PlayGlobal(Rule->GUICheckboxSound, 1.0f, 0x2000, 0);
				HWND parent = GetParent(window);
				SendMessageA(parent, WM_COMMAND, (unsigned int)(unsigned short)GetWindowLongA(window, GWL_ID) | ((unsigned int)(unsigned short)checked << 16), (LPARAM)window);
				return 0;
    }
			case OD_SUBCLASSED: {
				WNDPROC previous = 0;
				Window_Proc_Table.getValue(window, previous);
				data->ItemState = CallWindowProcA(previous, window, BM_GETCHECK, 0, 0);
        break;
    }
			case 0x4e5: {
				bool previous = data->IsCheckAltSelected;
				data->IsCheckAltArt = lparam != 0;
				if (previous != data->IsCheckAltArt) InvalidateRect(window, 0, 0);
        break;
    }
			case 0x4e6: {
				bool previous = data->IsCheckAltSelected;
				data->IsCheckAltSelected = lparam != 0;
				if (previous != data->IsCheckAltSelected) InvalidateRect(window, 0, 0);
        break;
    }
			case 0x4e7: return data->IsCheckAltSelected;
    }
		WNDPROC previous = 0;
		Window_Proc_Table.getValue(window, previous);
		return CallWindowProcA(previous, window, message, wparam, lparam);
    }
	return 0;
}
}


namespace OwnerDraw {
LRESULT __stdcall Button_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WinData * data = 0;
	Control_Table.getPointer((unsigned int)window, &data);
	RECT display; memset(&display, 0, sizeof(display));
	Get_Display_Rect(window, &display);
	Rect origin;
	origin.X = display.left;origin.Y = display.top;
	origin.Width = display.right - display.left;origin.Height = display.bottom - display.top;
	RECT client; memset(&client, 0, sizeof(client));
	GetClientRect(window, &client);
	int state = data->ItemState;
	LONG style = GetWindowLongA(window, GWL_STYLE);
	switch (message) {
		case WM_PAINT: {
			Rect rect = origin;
			unsigned long color = Text_Color;
			if (!data->Back) {
				BSurface * captured = new BSurface(client.right + 1, client.bottom + 1, 2);
				data->Back = captured;
				++Surface_Count;
				Rect destination, source;
				destination.X = 0;destination.Y = 0;
				destination.Width = client.right + 1;destination.Height = client.bottom + 1;
				source.X = display.left;source.Y = display.top;
				source.Width = client.right + 1;source.Height = client.bottom + 1;
				captured->Blit_From(destination, *AlternateSurface, source, false, true);
        }
			if (data->Image) {
				Surface * image = data->Image;
				if ((state & 1) && data->AlternateImage) image = data->AlternateImage;
				Rect source = rect;
				source.Y = source.X = 0;
				AlternateSurface->Blit_From(rect, *image, source, false, true);
        } else {
				char updown = 'u';
				if (state & 1) updown = 'd';
				if (style & WS_DISABLED) updown = 'u';
				int widths[2] = { 7, 7 };
				int margins[2] = { 10, 10 };
				int heights[2] = { 24, 30 };
				unsigned index = 0;
				for (unsigned i = 0; i < 2; i++) {
					if (heights[i] > rect.Height && i != 0) break;
					index = i;
            }
				int width = widths[index];
				int height = heights[index];
				int margin = margins[index];
				if (data->Back) {
					Rect source, destination;
					source.X = 0;source.Y = 0;
					source.Width = client.right + 1;source.Height = client.bottom + 1;
					destination.X = display.left;destination.Y = display.top;
					destination.Width = client.right + 1;destination.Height = client.bottom + 1;
					AlternateSurface->Blit_From(destination, *data->Back, source, false, true);
					InvalidateRect(window, 0, 0);
            }
				origin.Y += (rect.Height - height) / 2;
				if (state & 1) origin.Y += 2;
				char name[40];
				sprintf(name, "b%c%c_li%d.pcx", updown, 'e', height);
				Surface * image = SurfaceCache.GetSurface(name, 0);
				origin.Height = image->Get_Height();
				Rect destination, source;
				destination = origin;
				destination.Width = width;
				source.Width = width;
				source.Y = 0;
				source.X = 0;
				destination.Height = height;
				source.Height = height;
				AlternateSurface->Blit_From(destination, *image, source, false, true);
				sprintf(name, "b%c%c_mi%d.pcx", updown, 'e', height);
				image = SurfaceCache.GetSurface(name, 0);
				Rect middle = origin;
				middle.X += width;
				middle.Width -= margin;
				middle.Height = image->Get_Height();
				SurfaceCache.Draw(middle, *AlternateSurface, *image, 0, 0);
				sprintf(name, "b%c%c_ri%d.pcx", updown, 'e', height);
				image = SurfaceCache.GetSurface(name, 0);
				destination = origin;
				destination.X += origin.Width - margin;
				destination.Width = margin;
				destination.Height = image->Get_Height();
				source.Width = destination.Width;
				source.Height = destination.Height;
				source.Y = 0;
				source.X = 0;
				AlternateSurface->Blit_From(destination, *image, source, false, true);
        }
			if (!data->Image && data->Text) {
				RECT unused_client;
				GetClientRect(window, &unused_client);
            RECT text_rect;
				text_rect.left = origin.X;
				text_rect.top = origin.Y + 1;
				text_rect.right = origin.Width + origin.X - 2;
				text_rect.bottom = origin.Height + origin.Y - 2;
				if (state & 1) {text_rect.left += 2;text_rect.top += 4;}
				Draw_Text_In_Rect_Ex(AlternateSurface, data->Text, &text_rect, data->Font, color, 5, 0xc, 0, 0, 0);
        }
			if (style & WS_DISABLED) Blend_Rect(&rect, AlternateSurface, 0, 128);
			ValidateRect(window, 0);
			return 0;
    }
		case BM_GETCHECK: return data->ItemState;
		case BM_SETCHECK:
			data->ItemState = wparam;
			InvalidateRect(window, 0, 1);
        break;
		case OD_SUBCLASSED: {
			WNDPROC previous = 0;
			Window_Proc_Table.getValue(window, previous);
			data->ItemState = CallWindowProcA(previous, window, BM_GETCHECK, 0, 0);
        break;
    }
		case WM_LBUTTONDOWN:case WM_LBUTTONDBLCLK:
			if (!data->ItemState) {
				data->ItemState = 1;
				InvalidateRect(window, 0, 1);
				VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, 0);
            break;
        }
			return 0;
		case WM_LBUTTONUP: {
			LockWindowUpdate(GetParent(window));
			WNDPROC previous = 0;
			Window_Proc_Table.getValue(window, previous);
			LRESULT result = CallWindowProcA(previous, window, WM_LBUTTONUP, wparam, lparam);
			LockWindowUpdate(0);
			return result;
    }
    }
	WNDPROC previous = 0;
	Window_Proc_Table.getValue(window, previous);
	return CallWindowProcA(previous, window, message, wparam, lparam);
}
}


#pragma intrinsic(log)

	/*
	 * Keep the attached scrollbar synchronized with the listbox state. This is skipped for
	 * the few messages that are queried while computing the scroll state (to avoid recursion).
	 */
namespace OwnerDraw {

void __fastcall Draw_Edge_Glows(Surface * surface, Rect const & rect, int raised, int count,
	int left_alpha, int top_alpha, int right_alpha, int bottom_alpha);

struct ScrollBarRecord {
		char SubclassParam[0x08];
		HWND ScrollOwner;
		char Reserved_0C[0x10 - 0x0c];
		Surface * Back;
		char Reserved_14[0x20 - 0x14];
		int SkipCheckPaint;
		char Reserved_24[0xbc - 0x24];
		bool IsDisabled;
		char IsHidden[0xc8 - 0xbd];
		int TextStyle;
		char Reserved_CC;
		bool IsAlternateColors;
		char Reserved_CE[0xe8 - 0xce];
		int result;
		int dragging;
		int range;
		int position;
		int up_pressed;
		int down_pressed;
		int keep_capture;
};

LRESULT __stdcall Scroll_Bar_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    RECT client_rect;
    RECT display_rect;
    GetClientRect(window, &client_rect);
    Get_Display_Rect(window, &display_rect);
	client_rect.bottom -= 2 * Border_Width;
	client_rect.right -= 2 * Border_Width;
	display_rect.left += Border_Width;
	display_rect.right -= Border_Width;
	display_rect.top += Border_Width;
	display_rect.bottom -= Border_Width;
	ScrollBarRecord * data = (ScrollBarRecord *)Get_Record(window);
	if (data == 0) {
		return 0;
    }
    bool keep_parent_capture = false;
	if (data->keep_capture) {
        keep_parent_capture = true;
    }
	int message_result = data->result;
	int dragging = data->dragging;
	int range = data->range;
	int position = data->position;
	int up_pressed = data->up_pressed;
	int down_pressed = data->down_pressed;
    if (!range) {
        range = 100;
    }
    int grip_top = 0;
    int grip_bottom = 0;
	int scroll_code = SB_LINEUP;
    int width = client_rect.right - client_rect.left;
	int travel = client_rect.bottom - client_rect.top - 44;
	int grip_height = (int)((double)travel - log((double)(range + 1)) * (double)travel * 0.2);
    if (grip_height <= 14) {
        grip_height = 14;
    }
	travel -= grip_height;
    if (travel <= 1) {
        travel = 1;
    }
	if (message < WM_USER || message == 0x4a5) {
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
			scroll_code = SB_THUMBTRACK;
            position = range * (grip_top - 22) / travel;
        }
    }
    switch (message) {
		case SBM_GETPOS:
			return position;
		case SBM_SETPOS:
        if ((int)wparam <= range && (int)wparam > 0) {
            position = (int)wparam;
        }
        break;
		case SBM_SETRANGE:
        range = (int)lparam;
        if (position > range) {
            position = range;
        }
        break;
		case SBM_SETSCROLLINFO: {
			SCROLLINFO * info = (SCROLLINFO *)lparam;
        range = info->nMax;
        position = info->nPos;
        break;
    }
		case WM_NCHITTEST:
		case WM_GETDLGCODE: {
			WNDPROC proc = 0;
			Window_Proc_Table.getValue(window, proc);
			return CallWindowProcA(proc, window, message, wparam, lparam);
    }
		case WM_ERASEBKGND:
			return 0;
		case WM_PAINT: {
			if (data->IsDisabled) {
            ValidateRect(window, 0);
            break;
        }
			if (data->SkipCheckPaint) {
				return 0;
        }
			Rect src_rect(0, 0, client_rect.right, client_rect.bottom);
			Rect full_rect(display_rect.left, display_rect.top, client_rect.right, client_rect.bottom);
        HWND parent = GetParent(window);
		WinData * parent_data = 0;
        if (parent) {
			Control_Table.getPointer((unsigned int const &)parent, &parent_data);
        }
        RECT parent_display;
        Get_Display_Rect(parent, &parent_display);
        Rect source_rect = src_rect;
		if (parent_data && parent_data->Back) {
            source_rect.X = display_rect.left + source_rect.X - parent_display.left;
            source_rect.Y += display_rect.top - parent_display.top;
        }
		if (data->Back) {
			if (data->Back->Get_Width() != client_rect.right || data->Back->Get_Height() != client_rect.bottom) {
				delete data->Back;
				data->Back = 0;
            }
        }
		if (!data->Back) {
			BSurface * background = new BSurface(client_rect.right, client_rect.bottom, 2);
			data->Back = background;
			Surface_Count++;
			if (parent_data && parent_data->Back) {
				background->Blit_From(src_rect, *parent_data->Back, source_rect, false, true);
            }
			int count = client_rect.bottom * client_rect.right;
				unsigned short * pixels = (unsigned short *)background->Lock();
            for (int i = 0; i < count; i++) {
				pixels[i] = Blend_Color(pixels[i], 0xffff, data->TextStyle);
            }
			if (pixels) {
                background->Unlock();
            }
        }
		if (parent_data && parent_data->Back) {
			AlternateSurface->Blit_From(full_rect, *parent_data->Back, source_rect, false, true);
        }
		int color = Color_To_Pixel(Default_Frame_Color);
		if (data->IsAlternateColors) {
			color = Color_To_Pixel(Alternate_Frame_Color);
        }
		Draw_Frame(AlternateSurface, full_rect, Border_Width, color);
			Rect grip_rect(display_rect.left, grip_top + display_rect.top, client_rect.right, grip_bottom - grip_top);
			full_rect = grip_rect;
			src_rect = grip_rect;
        src_rect.X = 0;
        src_rect.Y = grip_top;
		Surface * grip_middle;
		if (data->IsAlternateColors) {
			grip_middle = SurfaceCache.GetSurface("gsbgripm.pcx", 0);
        } else {
			grip_middle = SurfaceCache.GetSurface("sbgripm.pcx", 0);
        }
		if (grip_middle) {
			grip_rect.Width = grip_middle->Get_Width();
        }
		SurfaceCache.Draw(grip_rect, *AlternateSurface, *grip_middle, 0, 0);
		Rect grip_src(0, 0, grip_middle->Get_Height(), grip_middle->Get_Width());
		Surface * grip_top_image;
		if (data->IsAlternateColors) {
			grip_top_image = SurfaceCache.GetSurface("gsbgript.pcx", 0);
        } else {
			grip_top_image = SurfaceCache.GetSurface("sbgript.pcx", 0);
        }
		if (grip_top_image) {
			full_rect.Height = grip_top_image->Get_Height();
        }
		AlternateSurface->Blit_From(full_rect, *grip_top_image, grip_src, false, true);
		Surface * grip_bottom_image;
		if (data->IsAlternateColors) {
			grip_bottom_image = SurfaceCache.GetSurface("gsbgripb.pcx", 0);
        } else {
			grip_bottom_image = SurfaceCache.GetSurface("sbgripb.pcx", 0);
        }
		if (grip_bottom_image) {
			full_rect.Y = display_rect.top + grip_bottom - grip_bottom_image->Get_Height();
        }
		AlternateSurface->Blit_From(full_rect, *grip_bottom_image, grip_src, false, true);
			Rect up_rect(display_rect.left, display_rect.top, client_rect.right, 22);
			full_rect = up_rect;
			src_rect = up_rect;
        src_rect.X = 0;
        src_rect.Y = 0;
		AlternateSurface->Blit_From(full_rect, *data->Back, src_rect, false, true);
		Draw_Edge_Glows(AlternateSurface, up_rect, up_pressed == 0, 2, Glow_Color,
			Glow_Color, Glow_Color, Glow_Color);
		Draw_Arrow(AlternateSurface, up_rect, 1, up_pressed, data->IsAlternateColors);
			Rect down_rect(display_rect.left, display_rect.bottom - 22, client_rect.right, 22);
			full_rect = down_rect;
			src_rect = down_rect;
        src_rect.X = 0;
        src_rect.Y = client_rect.bottom - 22;
		AlternateSurface->Blit_From(full_rect, *data->Back, src_rect, false, true);
		Draw_Edge_Glows(AlternateSurface, down_rect, down_pressed == 0, 2, Glow_Color,
			Glow_Color, Glow_Color, Glow_Color);
		Draw_Arrow(AlternateSurface, down_rect, 0, down_pressed, data->IsAlternateColors);
        ValidateRect(window, 0);
        break;
    }
		case WM_TIMER: {
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
						scroll_code = SB_LINEUP;
						position--;
                }
            } else if (cursor.y > client_rect.bottom - 22) {
                if (!dragging) {
                    down_pressed = 1;
                }
                if (position + 1 <= range) {
						scroll_code = SB_LINEDOWN;
						position++;
                }
            }
        }
		SetTimer(window, 0, 0x19, 0);
        break;
    }
		case WM_MOUSEMOVE:
        if (dragging) {
			RECT area = client_rect;
			area.left = client_rect.right - width;
			InvalidateRect(window, &area, 0);
        }
			if (wparam & MK_LBUTTON) {
            break;
        }
		case WM_LBUTTONUP:
        message_result = 0;
        dragging = 0;
        if (up_pressed || down_pressed) {
			InvalidateRect(window, 0, 0);
        }
        up_pressed = 0;
        down_pressed = 0;
        KillTimer(window, 0);
        ReleaseCapture();
        if (keep_parent_capture) {
			SetCapture(data->ScrollOwner);
        }
			scroll_code = SB_ENDSCROLL;
        break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK: {
			if (message == WM_LBUTTONDOWN) {
            message_result = 1;
            SetCapture(window);
			SetTimer(window, 0, 500, 0);
        } else {
            message_result = 0;
            dragging = 0;
            KillTimer(window, 0);
            ReleaseCapture();
            if (keep_parent_capture) {
				SetCapture(data->ScrollOwner);
            }
        }
			POINT click;
			click.x = (unsigned short)lparam;
			click.y = (unsigned short)(((unsigned long)lparam >> 16) & 0xffff);
			int repeat = 1;
			if (message == WM_LBUTTONDBLCLK) {
				repeat = 2;
        }
        down_pressed = 0;
        up_pressed = 0;
        while (repeat > 0) {
			if (click.x > client_rect.right - width) {
					if (click.y < 22 && position) {
                    up_pressed = 1;
						scroll_code = SB_LINEUP;
						position--;
                } else {
					bool skip_thumb = false;
						if (click.y > client_rect.bottom - 22) {
                        if (position + 1 <= range) {
                            down_pressed = 1;
								scroll_code = SB_LINEDOWN;
								position++;
								skip_thumb = true;
                        }
                    }
					if (!skip_thumb) {
							if (click.y < grip_top || click.y >= grip_bottom) {
								grip_top = click.y - grip_height / 2;
                            if (grip_top < 22) {
                                grip_top = 22;
                            }
                            if (client_rect.bottom - grip_height - 22 < grip_top) {
                                grip_top = client_rect.bottom - grip_height - 22;
                            }
                            grip_bottom = grip_top + grip_height;
								scroll_code = SB_THUMBTRACK;
                            position = range * (grip_top - 22) / travel;
							} else if (message == WM_LBUTTONDOWN) {
                            dragging = 1;
                        }
                    }
                }
            }
			repeat--;
        }
        break;
    }
	case 0x4d2:
		data->keep_capture = lparam != 0;
        break;
	case 0x4f1:
		data->IsAlternateColors = lparam == 1;
        break;
    }
    int send_notify = 0;
	if (position != data->position || range != data->range) {
		if (data->ScrollOwner) {
            send_notify = 1;
        }
    }
	data->result = message_result;
	data->dragging = dragging;
	data->range = range;
	data->position = position;
	data->up_pressed = up_pressed;
	data->down_pressed = down_pressed;
    if (send_notify) {
		SendMessageA(data->ScrollOwner, WM_VSCROLL, (unsigned short)position << 16 | (unsigned short)scroll_code, (LPARAM)window);
		InvalidateRect(window, 0, 0);
    }
	return 0;
}

}


namespace OwnerDraw {
LRESULT __stdcall Progress_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    RECT rect;
    Get_Display_Rect(window, &rect);
	WinData * data = 0;
	Control_Table.getPointer((unsigned int)window, &data);
    if (data) {
        switch (message) {
			case OD_SUBCLASSED:
				data->RangeMax = 100;
            break;
			case PBM_SETRANGE:
				data->ItemState = (unsigned int)lparam & 0xffff;
				data->RangeMax = (unsigned int)lparam >> 16;
            break;
			case PBM_SETPOS: {
            int position = (int)wparam;
				if (position < data->ItemState) position = data->ItemState;
				if (position > data->RangeMax) position = data->RangeMax;
				data->Position = position;
				InvalidateRect(window, 0, 0);
            break;
        }
			case WM_PAINT: {
				Rect source(0, 0, rect.right - rect.left + 1, rect.bottom - rect.top + 1);
				Rect destination(rect.left, rect.top, rect.right - rect.left + 1, rect.bottom - rect.top + 1);
				if (!data->Back) {
					Surface * background = new BSurface(rect.right - rect.left + 1, rect.bottom - rect.top + 1, 2);
					data->Back = background;
					++Surface_Count;
					background->Blit_From(source, *AlternateSurface, destination, false, true);
            }
				AlternateSurface->Blit_From(destination, *data->Back, source, false, true);
				int position = (data->Position * 65536) / (data->RangeMax - data->ItemState);
				int color = Color_To_Pixel(0xff);
				Draw_Progress_Fill(destination, AlternateSurface, color, position);
            ValidateRect(window, 0);
            break;
        }
        }
    }
	return 0;
}
}


namespace OwnerDraw {

struct TrackBarState {
		int result;
		int dragging;
		int range;
		int value;
		int minimum;
		int thumb_pos;
		int step;
		int show_numbers;
		int click_suppress;
};

LRESULT __stdcall Trackbar_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    bool play_click = true;

    RECT client_rect;
    RECT display_rect;
    GetClientRect(window, &client_rect);
    Get_Display_Rect(window, &display_rect);
    int number_width = 50;

	WinData * data = Get_Record(window);
	if (data == 0) {
		return 0;
    }
	TrackBarState * track = (TrackBarState *)&data->ItemState;

	int message_result = track->result;
	int dragging = track->dragging;
	int range = track->range;
	int value = track->value;
	int minimum = track->minimum;
    int maximum;
	int thumb_pos = track->thumb_pos;
	int step = track->step;
	int show_numbers = track->show_numbers;

    if (!show_numbers) {
        number_width = 0;
    }

    int slider_width = client_rect.right - client_rect.left - number_width - 13;
    if (slider_width <= 1) {
        slider_width = 1;
    }

    if (!range) {
		WNDPROC proc = 0;
		Window_Proc_Table.getValue(window, proc);

		int range_max = CallWindowProcA(proc, window, 0x402, 0, 0);
		minimum = CallWindowProcA(proc, window, 0x401, 0, 0);
        range = range_max - minimum;
		value = CallWindowProcA(proc, window, 0x400, 0, 0) - minimum;
        thumb_pos = value * slider_width / range;
        if (!range) {
            range = 100;
        }

		track->thumb_pos = thumb_pos;
		track->range = range;
		track->value = value;
		track->minimum = minimum;
		track->step = step;
		track->show_numbers = show_numbers;
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
		if (client_rect.right - number_width - 12 < xpos) {
			xpos = client_rect.right - number_width - 12;
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
		case 0x84:
		case 0x87: {
			WNDPROC proc = 0;
			Window_Proc_Table.getValue(window, proc);
			return CallWindowProcA(proc, window, message, wparam, lparam);
    }

		case 0xa:
			InvalidateRect(window, 0, FALSE);
        break;

		case 0xf: {
			RECT paint_rect;
			Get_Display_Rect(window, &paint_rect);
			Rect disp_rect(paint_rect.left, paint_rect.top, paint_rect.right - paint_rect.left, paint_rect.bottom - paint_rect.top);
			LONG style = GetWindowLongA(window, -16);

			if (data->Back == 0) {
            RECT disp_copy;
            RECT client_copy;
            Get_Display_Rect(window, &disp_copy);
            GetClientRect(window, &client_copy);

				Surface * surf = new BSurface(client_copy.right + 1, client_copy.bottom + 1, 2);
				data->Back = surf;
				Surface_Count++;

            Rect src_rect(0, 0, client_copy.right + 1, client_copy.bottom + 1);
            Rect dst_rect(disp_copy.left, disp_copy.top, client_copy.right + 1, client_copy.bottom + 1);
				surf->Blit_From(src_rect, *AlternateSurface, dst_rect, false, true);
				Blend_Rect(&src_rect, surf, 0, data->TextStyle);
        }

			if (data->Back != 0) {
            RECT disp_copy;
            RECT client_copy;
            Get_Display_Rect(window, &disp_copy);
            GetClientRect(window, &client_copy);

            Rect src_rect(0, 0, client_copy.right + 1, client_copy.bottom + 1);
            Rect dst_rect(disp_copy.left, disp_copy.top, client_copy.right + 1, client_copy.bottom + 1);
				AlternateSurface->Blit_From(dst_rect, *data->Back, src_rect, false, true);
        }

        if (show_numbers) {
            {
				Surface * center = SurfaceCache.GetSurface("trofm.pcx", 0);
					Rect center_rect(disp_rect.X + disp_rect.Width - number_width + 1, disp_rect.Y - 1, number_width, center->Get_Height());
                SurfaceCache.Draw(center_rect, *AlternateSurface, *center, 0, 0);
            }
            {
				Surface * left = SurfaceCache.GetSurface("trofl.pcx", 0);
					Rect left_dst(disp_rect.X + disp_rect.Width - number_width + 1, disp_rect.Y - 1, left->Get_Width(), left->Get_Height());
					Rect left_src = left_dst;
					left_src.Y = 0;
					left_src.X = 0;
				AlternateSurface->Blit_From(left_dst, *left, left_src, false, true);
            }
            {
				Surface * right = SurfaceCache.GetSurface("trofr.pcx", 0);
					Rect right_dst(disp_rect.X + disp_rect.Width - right->Get_Width() + 1, disp_rect.Y - 1, right->Get_Width(), right->Get_Height());
					Rect right_src = right_dst;
					right_src.Y = 0;
					right_src.X = 0;

				AlternateSurface->Blit_From(right_dst, *right, right_src, false, true);
            }
        }

			Rect grip_rect(paint_rect.left + grip_left, paint_rect.top, grip_right - grip_left, paint_rect.bottom - paint_rect.top);
			Surface * grip = SurfaceCache.GetSurface("trakgrip.pcx", 0);
			Rect grip_src;
			grip_src.X = 0;
			grip_src.Y = 0;
			grip_src.Width = grip->Get_Width();
			grip_src.Height = grip->Get_Height();
			AlternateSurface->Blit_From(grip_rect, *grip, grip_src, false, true);

			int disabled = style & 0x8000000;
			if (disabled) {
				Blend_Rect(&grip_rect, AlternateSurface, 0, Dim_Alpha);
        }

		int frame_color = Color_To_Pixel(Default_Frame_Color);
			if (disabled) {
			frame_color = Color_To_Pixel(Frame_Color);
        }

			Rect frame_rect = disp_rect;
			frame_rect.Width = disp_rect.Width - number_width;
			Draw_Frame(AlternateSurface, frame_rect, 1, frame_color);

			Rect number_rect = disp_rect;
			int inset = (show_numbers != 0) + 1;
			number_rect.X = inset - number_width + disp_rect.Width + disp_rect.X;
			number_rect.Width = number_width - inset;
			Draw_Frame(AlternateSurface, number_rect, 1, frame_color);

        if (show_numbers) {
				wchar_t buffer[32];


				swprintf(buffer, L"%d", step * ((minimum + value) / step));

				int text_color = Text_Color;
				if (disabled) {
					text_color = Disabled_Text_Color;
            }

				RECT text_rect = paint_rect;
				text_rect.left = paint_rect.right - 49;
				Draw_Text_In_Rect_Ex(AlternateSurface, buffer, &text_rect, data->Font, text_color, 5, 0xc, 0, 0, 0);
        }

        ValidateRect(window, 0);
        break;
    }

		case 0x14:
			return 0;

		case 0x200:
        if (dragging) {
            RECT rect = client_rect;
            InvalidateRect(window, &rect, FALSE);
        }
			if (wparam & 1) {
            break;
        }

		case 0x202:
        message_result = 0;
        dragging = 0;
        ReleaseCapture();
        break;

		case 0x201:
		case 0x203: {
			if (message == WM_LBUTTONDOWN) {
            message_result = 1;
            SetCapture(window);
        } else {
            message_result = 0;
            ReleaseCapture();
        }

		int xpos = (unsigned short)lparam;
		int ypos = (unsigned short)((unsigned int)lparam >> 16);
        if (ypos > client_rect.bottom - 18) {
            if (xpos >= grip_left && xpos < grip_right) {
					if (message == WM_LBUTTONDOWN) {
                    dragging = 1;
                }
            } else {
                int x = xpos - 6;
                if (x < 1) {
                    x = 1;
                }
					if (client_rect.right - number_width - 12 < x) {
						x = client_rect.right - number_width - 12;
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

		case 0x400:
			return step * ((minimum + value) / step);

		case 0x405:
        if (lparam - minimum <= range && lparam - minimum >= 0) {
            value = lparam - minimum;
        }
        play_click = false;
        thumb_pos = value * slider_width / range;
        break;

		case 0x406:
			minimum = (unsigned short)lparam;
			maximum = (unsigned short)((unsigned int)lparam >> 16);
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

		case 0x4ab:
        step = lparam;
        break;

		case 0x4ac:
        show_numbers = lparam;
        break;

		case 0x4ae:
			track->click_suppress = (wparam == 0);
        break;
    }

    int changed = 0;
	if (value != track->value || range != track->range || minimum != track->minimum) {
        changed = 1;
    }

	track->result = message_result;
	track->dragging = dragging;
	track->thumb_pos = thumb_pos;
	track->range = range;
	track->value = value;
	track->minimum = minimum;
	track->step = step;
	track->show_numbers = show_numbers;

    if (changed) {
		InvalidateRect(window, 0, FALSE);

        HWND parent = GetParent(window);
		SendMessageA(parent, 0x114, (WPARAM)((unsigned short)5 | (unsigned long)(unsigned short)(value + minimum) << 16), (LPARAM)window);

		if (play_click == true && !track->click_suppress) {
			VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, 0);
        }
    }

	return 0;
}

}


namespace OwnerDraw {

LRESULT __stdcall Group_Box_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
		case WM_PAINT: {
			int gap = 0;
			WinData * record = Get_Record(window);
			if (!record) {
				return 0;
        }
			wchar_t const * text = record->Text;
			if (!text) {
				text = L"";
        }
			RECT area;
			Get_Display_Rect(window, &area);
			int width = area.right - area.left;
			int middle = record->Font->LineHeight / 2 + area.top;
			int text_width;
			int text_height;
			record->Font->Text_Dimensions(text, &text_width, &text_height, width);
			Rect caption(area.left + 10, area.top, width, area.bottom - area.top);
			OD_Draw_Text(Text_Color, 0, caption, text, wcslen(text), 0, 0, AlternateSurface, 0);
			int color = Color_To_Pixel(Default_Frame_Color);
			AlternateSurface->Draw_Line(Point2D(area.left, middle), Point2D(area.left + 8, middle), color);
			if (text_width) {
				gap = 12;
        }
			AlternateSurface->Draw_Line(Point2D(area.left + text_width + gap, middle), Point2D(area.right, middle), color);
			AlternateSurface->Draw_Line(Point2D(area.left, middle), Point2D(area.left, area.bottom), color);
			AlternateSurface->Draw_Line(Point2D(area.left, area.bottom), Point2D(area.right, area.bottom), color);
			AlternateSurface->Draw_Line(Point2D(area.right, middle), Point2D(area.right, area.bottom), color);
        ValidateRect(window, 0);
			return 0;
    }
		case WM_ERASEBKGND:
			return 1;
		case WM_NCPAINT:
			return 0;
    }
	WNDPROC proc = 0;
	Window_Proc_Table.getValue(window, proc);
	return CallWindowProcA(proc, window, message, wparam, lparam);
}

}


namespace OwnerDraw {

LRESULT __stdcall Hotkey_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
		case WM_PAINT: {
			RECT area;
			Get_Display_Rect(window, &area);
			Rect screen(area.left, area.top, area.right - area.left + 1, area.bottom - area.top + 1);
			Rect local(0, 0, screen.Width, screen.Height);
			WinData * record = 0;
			Control_Table.getPointer((unsigned int)window, &record);
			if (!record) {
				return 0;
        }
			if (!record->Back) {
				Surface * surface = new BSurface(screen.Width, screen.Height, 2);
				record->Back = surface;
				Surface_Count++;
				surface->Blit_From(local, *AlternateSurface, screen, false, true);
        }
			wchar_t text[64];
			unsigned short hotkey = (unsigned short)SendMessageA(window, HKM_GETHOTKEY, 0, 0);
			Hotkey_Text(hotkey, text);
			if (record->Back) {
				AlternateSurface->Blit_From(screen, *record->Back, local, false, true);
        }
			Draw_Frame(AlternateSurface, screen, 1, -1);
			if (wcslen(text) > 0) {
				area.left += 4;
				area.top += 4;
				area.right -= 4;
				area.bottom -= 4;
				Draw_Text_In_Rect(AlternateSurface, text, &area, record->Font, Text_Color, 0x24);
        }
        ValidateRect(window, 0);
		return 0;
    }
		case WM_ERASEBKGND:
			return 1;
		case WM_NCPAINT:
			return 0;
    }
	WNDPROC proc = 0;
	Window_Proc_Table.getValue(window, proc);
	return CallWindowProcA(proc, window, message, wparam, lparam);
}

}


namespace OwnerDraw {

void __fastcall Hotkey_Text(unsigned short hotkey, wchar_t * text)
{
	char name[32];
	char buffer[500];
	unsigned char modifiers = (unsigned char)(hotkey >> 8);
	buffer[0] = 0;
    *text = 0;
	if (modifiers & 8) {
        return;
    }
	if (modifiers & 4) {
		LONG key = MapVirtualKeyA(0x12, 0) << 16 | 0x2000001;
		GetKeyNameTextA(key, name, sizeof(name));
		strcat(buffer, name);
        strcat(buffer, "+");
    }
	if (modifiers & 2) {
		LONG key = MapVirtualKeyA(0x11, 0) << 16 | 0x2000001;
		GetKeyNameTextA(key, name, sizeof(name));
		strcat(buffer, name);
        strcat(buffer, "+");
    }
	if (modifiers & 1) {
		LONG key = MapVirtualKeyA(0x10, 0) << 16 | 0x2000001;
		GetKeyNameTextA(key, name, sizeof(name));
		strcat(buffer, name);
        strcat(buffer, "+");
    }
	LONG key = MapVirtualKeyA(hotkey & 0xff, 0) << 16 | 0x2000001;
	GetKeyNameTextA(key, name, sizeof(name));
	strcat(buffer, name);
	swprintf(text, L"%hs", buffer);
}

}


namespace OwnerDraw {

unsigned short Red_Mask = 0;
unsigned short Green_Mask = 0;
unsigned short Blue_Mask = 0;

void Init_Color_Masks()
{
	Red_Mask = 0xff;
	Red_Mask >>= DSurface::Get_Red_Left();
	Red_Mask <<= DSurface::Get_Red_Right();
	Green_Mask = 0xff;
	Green_Mask >>= DSurface::Get_Green_Left();
	Green_Mask <<= DSurface::Get_Green_Right();
	Blue_Mask = 0xff;
	Blue_Mask >>= DSurface::Get_Blue_Left();
	Blue_Mask <<= DSurface::Get_Blue_Right();
}

}


namespace OwnerDraw {

void __fastcall Load_Dialog_Images()
{
	SurfaceCache.CachePCX(Dialog_Back_Name, 2, 0);
	SurfaceCache.CachePCX("gdii.pcx", 2, 0);
	SurfaceCache.CachePCX("nodi.pcx", 2, 0);
	SurfaceCache.CachePCX("rani.pcx", 2, 0);
	SurfaceCache.CachePCX("usai.pcx", 2, 0);
	SurfaceCache.CachePCX("japi.pcx", 2, 0);
	SurfaceCache.CachePCX("frai.pcx", 2, 0);
	SurfaceCache.CachePCX("geri.pcx", 2, 0);
	SurfaceCache.CachePCX("gbri.pcx", 2, 0);
	SurfaceCache.CachePCX("djbi.pcx", 2, 0);
	SurfaceCache.CachePCX("arbi.pcx", 2, 0);
	SurfaceCache.CachePCX("lati.pcx", 2, 0);
	SurfaceCache.CachePCX("rusi.pcx", 2, 0);
	SurfaceCache.CachePCX("obsi.pcx", 2, 0);
	SurfaceCache.CachePCX("arrow_uu.pcx", 2, 0);
	SurfaceCache.CachePCX("arrow_ud.pcx", 2, 0);
	SurfaceCache.CachePCX("arrow_du.pcx", 2, 0);
	SurfaceCache.CachePCX("arrow_dd.pcx", 2, 0);
	SurfaceCache.CachePCX("leftbar.pcx", 2, 0);
	SurfaceCache.CachePCX("rightbar.pcx", 2, 0);
	SurfaceCache.CachePCX("trakgrip.pcx", 2, 0);
	SurfaceCache.CachePCX("sbgript.pcx", 2, 0);
	SurfaceCache.CachePCX("sbgripm.pcx", 2, 0);
	SurfaceCache.CachePCX("sbgripb.pcx", 2, 0);
	SurfaceCache.CachePCX("gsbgript.pcx", 2, 0);
	SurfaceCache.CachePCX("gsbgripm.pcx", 2, 0);
	SurfaceCache.CachePCX("gsbgripb.pcx", 2, 0);
	SurfaceCache.CachePCX("bar_ll.pcx", 2, 0);
	SurfaceCache.CachePCX("bar_lr.pcx", 2, 0);
	SurfaceCache.CachePCX("bar_ul.pcx", 2, 0);
	SurfaceCache.CachePCX("bar_ur.pcx", 2, 0);
	SurfaceCache.CachePCX("dlgsysi.pcx", 1, 0);
    SurfaceCache.CachePalettedPCX("dlgsysa.pcx");
	SurfaceCache.CachePCX("wouban.pcx", 2, 0);
	SurfaceCache.CachePCX("wodban.pcx", 2, 0);
	SurfaceCache.CachePCX("wouleave.pcx", 2, 0);
	SurfaceCache.CachePCX("wodleave.pcx", 2, 0);
	SurfaceCache.CachePCX("wousqlch.pcx", 2, 0);
	SurfaceCache.CachePCX("wodsqlch.pcx", 2, 0);
	SurfaceCache.CachePCX("woudcon.pcx", 2, 0);
	SurfaceCache.CachePCX("woddcon.pcx", 2, 0);
	SurfaceCache.CachePCX("woukick.pcx", 2, 0);
	SurfaceCache.CachePCX("wodkick.pcx", 2, 0);
	SurfaceCache.CachePCX("wouhelp.pcx", 2, 0);
	SurfaceCache.CachePCX("wodhelp.pcx", 2, 0);
	SurfaceCache.CachePCX("woufind.pcx", 2, 0);
	SurfaceCache.CachePCX("wodfind.pcx", 2, 0);
	SurfaceCache.CachePCX("wouopt.pcx", 2, 0);
	SurfaceCache.CachePCX("wodopt.pcx", 2, 0);
	SurfaceCache.CachePCX("woutrny.pcx", 2, 0);
	SurfaceCache.CachePCX("wodtrny.pcx", 2, 0);
	SurfaceCache.CachePCX("wouclan.pcx", 2, 0);
	SurfaceCache.CachePCX("wodclan.pcx", 2, 0);
	SurfaceCache.CachePCX("woufgame.pcx", 2, 0);
	SurfaceCache.CachePCX("wodfgame.pcx", 2, 0);
	SurfaceCache.CachePCX("wouact.pcx", 2, 0);
	SurfaceCache.CachePCX("wodact.pcx", 2, 0);
	SurfaceCache.CachePCX("wouref.pcx", 2, 0);
	SurfaceCache.CachePCX("wodref.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_tlu.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_tmu.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_tru.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_tld.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_tmd.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_trd.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_ftl.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_ftr.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_ftm.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_fbr.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_fbl.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_fbm.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_fmr.pcx", 2, 0);
	SurfaceCache.CachePCX("tab_fml.pcx", 2, 0);
	SurfaceCache.CachePCX("woloper.pcx", 2, 0);
	SurfaceCache.CachePCX("wolsqlch.pcx", 2, 0);
	SurfaceCache.CachePCX("woltrny.pcx", 2, 0);
	SurfaceCache.CachePCX("woluser.pcx", 2, 0);
	SurfaceCache.CachePCX("wolvoice.pcx", 2, 0);
	SurfaceCache.CachePCX("wolpriv.pcx", 2, 0);
	SurfaceCache.CachePCX("wolacpt.pcx", 2, 0);
	SurfaceCache.CachePCX("wolhost.pcx", 2, 0);
	SurfaceCache.CachePCX("wolclan.pcx", 2, 0);
	SurfaceCache.CachePCX("wolob.pcx", 2, 0);
	SurfaceCache.CachePCX("wolnob.pcx", 2, 0);
	SurfaceCache.CachePCX("wolrslkob.pcx", 2, 0);
	SurfaceCache.CachePCX("wolrslknob.pcx", 2, 0);
	SurfaceCache.CachePCX("gt18.pcx", 2, 0);
	SurfaceCache.CachePCX("dnarrowp.pcx", 2, 0);
	SurfaceCache.CachePCX("uparrowp.pcx", 2, 0);
	SurfaceCache.CachePCX("dnarrowr.pcx", 2, 0);
	SurfaceCache.CachePCX("uparrowr.pcx", 2, 0);
	SurfaceCache.CachePCX("gdnarrowp.pcx", 2, 0);
	SurfaceCache.CachePCX("guparrowp.pcx", 2, 0);
	SurfaceCache.CachePCX("gdnarrowr.pcx", 2, 0);
	SurfaceCache.CachePCX("guparrowr.pcx", 2, 0);
	SurfaceCache.CachePCX("trofl.pcx", 2, 0);
	SurfaceCache.CachePCX("trofm.pcx", 2, 0);
	SurfaceCache.CachePCX("trofr.pcx", 2, 0);
	SurfaceCache.CachePCX("sb_psh_u.pcx", 2, 0);
	SurfaceCache.CachePCX("sb_psh_d.pcx", 2, 0);
	SurfaceCache.CachePCX("sb_rel_u.pcx", 2, 0);
	SurfaceCache.CachePCX("sb_rel_d.pcx", 2, 0);
	SurfaceCache.CachePCX("bst_chkd.pcx", 2, 0);
	SurfaceCache.CachePCX("bst_uchk.pcx", 2, 0);
	SurfaceCache.CachePCX("bst_chkg.pcx", 2, 0);
	SurfaceCache.CachePCX("bst_uckg.pcx", 2, 0);
	SurfaceCache.CachePCX("ccd_i.pcx", 2, 0);
	SurfaceCache.CachePCX("cce_i.pcx", 2, 0);
	SurfaceCache.CachePCX("cud_i.pcx", 2, 0);
	SurfaceCache.CachePCX("cue_i.pcx", 2, 0);
	SurfaceCache.CachePCX("cce_ir.pcx", 2, 0);
	SurfaceCache.CachePCX("cce_il.pcx", 2, 0);
	SurfaceCache.CachePCX("bue_li30.pcx", 2, 0);
	SurfaceCache.CachePCX("bue_mi30.pcx", 2, 0);
	SurfaceCache.CachePCX("bue_ri30.pcx", 2, 0);
	SurfaceCache.CachePCX("bde_li30.pcx", 2, 0);
	SurfaceCache.CachePCX("bde_mi30.pcx", 2, 0);
	SurfaceCache.CachePCX("bde_ri30.pcx", 2, 0);
	SurfaceCache.CachePCX("bud_li30.pcx", 2, 0);
	SurfaceCache.CachePCX("bud_mi30.pcx", 2, 0);
	SurfaceCache.CachePCX("bud_ri30.pcx", 2, 0);
	SurfaceCache.CachePCX("bue_li24.pcx", 2, 0);
	SurfaceCache.CachePCX("bue_mi24.pcx", 2, 0);
	SurfaceCache.CachePCX("bue_ri24.pcx", 2, 0);
	SurfaceCache.CachePCX("bde_li24.pcx", 2, 0);
	SurfaceCache.CachePCX("bde_mi24.pcx", 2, 0);
	SurfaceCache.CachePCX("bde_ri24.pcx", 2, 0);
	SurfaceCache.CachePCX("bud_li24.pcx", 2, 0);
	SurfaceCache.CachePCX("bud_mi24.pcx", 2, 0);
	SurfaceCache.CachePCX("bud_ri24.pcx", 2, 0);
	SurfaceCache.CachePCX("wolreslk.pcx", 2, 0);
	SurfaceCache.CachePCX("pingg.pcx", 2, 0);
	SurfaceCache.CachePCX("pingy.pcx", 2, 0);
	SurfaceCache.CachePCX("pingr.pcx", 2, 0);
}

}


namespace OwnerDraw {

bool __fastcall Draw_Edge_Glow(Surface * surface, Point2D const & start, Point2D const & end, int color,
	int alpha)
{
    Point2D startpoint = start;
    Point2D endpoint = end;
    if (startpoint.X > endpoint.X) {
        swap(startpoint, endpoint);
    }
	int bpp = surface->Bytes_Per_Pixel();
	void * buffer = surface->Lock(startpoint);
	if (buffer != 0) {
        if (startpoint.Y == endpoint.Y) {
            if (bpp == 1) {
                memset(buffer, color, endpoint.X - startpoint.X + 1);
            } else {
                for (int i = 0; i <= endpoint.X - startpoint.X; i++) {
                    *((unsigned short *)buffer + i) = Blend_Color(*((unsigned short *)buffer + i), color, alpha);
                }
            }
        } else if (startpoint.X == endpoint.X) {
			int pitch = startpoint.Y > endpoint.Y ? -surface->Stride() : surface->Stride();
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
			int pitch = surface->Stride();
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
		return true;
    }
	return false;
}

}


namespace OwnerDraw {

bool __fastcall Draw_Edge_Glow(Surface * surface, Point2D const & start, Point2D const & end, int color,
	int alpha);

void __fastcall Draw_Edge_Glows(Surface * surface, Rect const & rect, int raised, int count,
	int left_alpha, int top_alpha, int right_alpha, int bottom_alpha)
{
	int light = 0xffff;
	int dark = 0;
    if (!raised) {
		dark = 0xffff;
		light = 0;
    }
    for (int i = 0; i < count; i++) {
		Draw_Edge_Glow(surface, Point2D(rect.X + i, rect.Y + i), Point2D(rect.X + rect.Width - 2 - i, rect.Y + i), light, top_alpha);
		Draw_Edge_Glow(surface, Point2D(rect.X + i, rect.Y + rect.Height - 1 - i),
			Point2D(rect.X + rect.Width - 1 - i, rect.Y + rect.Height - 1 - i), dark, bottom_alpha);
		Draw_Edge_Glow(surface, Point2D(rect.X + i, rect.Y + i + 1), Point2D(rect.X + i, rect.Y + rect.Height - 1 - i), light,
			left_alpha);
		Draw_Edge_Glow(surface, Point2D(rect.X + rect.Width - 1 - i, rect.Y + i),
			Point2D(rect.X + rect.Width - 1 - i, rect.Y + rect.Height - 2 - i), dark, right_alpha);
    }
}

}


namespace OwnerDraw {

void __fastcall Draw_Arrow(Surface * surface, Rect const & rect, int upward, int pressed, bool prefixed)
{
	char name[32];
    char state = 'r';
    if (pressed) {
        state = 'p';
    }
    if (upward) {
		sprintf(name, "guparrow%c.pcx", state);
    } else {
		sprintf(name, "gdnarrow%c.pcx", state);
    }
	Surface * arrow = SurfaceCache.GetSurface(name + !prefixed, 0);
	Rect destination = rect;
	destination.Width = arrow->Get_Width();
	destination.Height = arrow->Get_Height();
	Rect source;
	source.Y = 0;
	source.X = 0;
	source.Width = arrow->Get_Width();
	source.Height = arrow->Get_Height();
	surface->Blit_From(destination, *arrow, source, false, true);
}

}


namespace OwnerDraw {

int __fastcall Color_To_Pixel(unsigned long color)
{
	if (color == 0xffffffff) {
		return -1;
    }
	union ColorChannels {
			struct { unsigned int red : 8; unsigned int green : 8; unsigned int blue : 8; unsigned int alpha : 8; } channels;
			int value;
	} packed;
	packed.value = color;
	return DSurface::Build_Hicolor_Pixel(color & 0xff, packed.channels.green, (color >> 16) & 0xff);
}

}


namespace OwnerDraw {

unsigned long __fastcall Pixel_To_Color(unsigned short pixel)
{
	RGBClass color = DSurface::Deconstruct_Hicolor_Pixel(pixel);
	return 0x02000000 | ((unsigned long)color.Get_Blue() << 16) | ((unsigned short)color.Get_Green() << 8) | color.Get_Red();
}

}


namespace OwnerDraw {

void __fastcall Draw_Frame(Surface * surface, Rect const & rect, int thickness, int color)
{
    if (color == -1) {
		color = Color_To_Pixel(Default_Frame_Color);
    }
	Rect frame = rect;
	frame.X -= thickness;
	frame.Y -= thickness;
	frame.Width += thickness * 2;
	frame.Height += thickness * 2;
	for (int i = 0; i < thickness; i++) {
		surface->Draw_Line(Point2D(frame.X + i, frame.Y + i), Point2D(frame.X + frame.Width - 2 - i, frame.Y + i), color);
		surface->Draw_Line(Point2D(frame.X + i, frame.Y + frame.Height - 1 - i),
			Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 1 - i), color);
		surface->Draw_Line(Point2D(frame.X + i, frame.Y + i + 1), Point2D(frame.X + i, frame.Y + frame.Height - 1 - i), color);
		surface->Draw_Line(Point2D(frame.X + frame.Width - 1 - i, frame.Y + i),
			Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 2 - i), color);
    }
}

}


namespace OwnerDraw {

void __fastcall Draw_Button_Frame(Surface * surface, Rect const & rect, int thickness)
{
	Rect frame = rect;
	int top_color = DSurface::Build_Hicolor_Pixel(0x27, 0xf8, 0x74);
	int bottom_color = DSurface::Build_Hicolor_Pixel(0x13, 0x7b, 0x39);
	int left_color = DSurface::Build_Hicolor_Pixel(0x1e, 0xba, 0x57);
	int right_color = DSurface::Build_Hicolor_Pixel(0x18, 0x99, 0x47);
	frame.X += -1 - thickness;
	frame.Y += -1 - thickness;
	frame.Width += 2 * thickness + 2;
	frame.Height += 2 * thickness + 2;
	for (int i = 0; i < thickness; i++) {
		surface->Draw_Line(Point2D(frame.X + i, frame.Y + i), Point2D(frame.X + frame.Width - 2 - i, frame.Y + i), top_color);
		surface->Draw_Line(Point2D(frame.X + i, frame.Y + frame.Height - 1 - i),
			Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 1 - i), bottom_color);
		surface->Draw_Line(Point2D(frame.X + i, frame.Y + i + 1), Point2D(frame.X + i, frame.Y + frame.Height - 1 - i), left_color);
		surface->Draw_Line(Point2D(frame.X + frame.Width - 1 - i, frame.Y + i),
			Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 2 - i), right_color);
    }
}

}


namespace OwnerDraw {
int __fastcall Draw_Text_In_Rect(Surface * surface, wchar_t const * text, RECT * area,
	BitFont * font, unsigned long color, int flags)
{
	unsigned int pixel = (((unsigned char)color >> (unsigned char)DSurface::RedLeft) << DSurface::RedRight)
        | (((unsigned char)(color >> 16) >> (unsigned char)DSurface::BlueLeft) << DSurface::BlueRight)
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)DSurface::GreenLeft) << DSurface::GreenRight);
	int x = area->left;
	int y = area->top;
	int width = area->right - x;
	int height = area->bottom - y;
	if (font == 0) {
		font = BitFont::Instance;
    }
	font->Set_Clip(1);
	font->Set_Bounds((LTRBStruct *)(tagRECT const *)area);
	font->Set_Color((unsigned short)pixel);
	BitText::Instance->Draw_Text(font, surface, text, x, y, width, height, 0, 0, 0);
	return 0;
}
}


namespace OwnerDraw {
int __fastcall Draw_Text_In_Rect_Ex(Surface * surface, wchar_t const * text, RECT * area, BitFont * font,
	unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10)
{
	unsigned int pixel = (((unsigned char)color >> (unsigned char)DSurface::RedLeft) << DSurface::RedRight)
        | (((unsigned char)(color >> 16) >> (unsigned char)DSurface::BlueLeft) << DSurface::BlueRight)
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)DSurface::GreenLeft) << DSurface::GreenRight);
	int x = area->left;
	int y = area->top;
	int width = area->right - x;
	int height = area->bottom - y;
	if (flags & 4) {
		int text_width, text_height;
		font->Text_Dimensions(text, &text_width, &text_height, width);
		y += (height - text_height) / 2;
    }
	font->Set_Clip(1);
	font->Set_Bounds((LTRBStruct *)(tagRECT const *)area);
	font->Set_Color((unsigned short)pixel);
	BitText::Instance->Draw_Text(font, surface, text, x, y, width, height, flags, arg9, arg10);
	return 0;
}
}


namespace OwnerDraw {

float __fastcall Glow_Factor(int value)
{
	int centers[3] = { 0x2b, 0x80, 0xd5 };
	float factor = 1.0f;
    for (int i = 0; i < 3; i++) {
		int center = centers[i];
		if (value > center - 16 && value <= center) {
			factor = 0.6f * ((center - value) * 0.0625f) + 0.4f;
		} else if (value > center && value <= center + 16) {
			factor = 0.6f * ((value - center) * 0.0625f) + 0.4f;
        }
    }
	return factor;
}

}


int __fastcall OD_Draw_Text(unsigned int color, BitFont * font, Rect const & area, wchar_t const * text,
	int length, int x_alignment, int y_alignment, Surface * surface, int shown)
{
	if (!GameInFocus) return 0;
	if (!surface) surface = AlternateSurface;
	unsigned int pixel = (((unsigned char)color >> (unsigned char)DSurface::RedLeft) << DSurface::RedRight)
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)DSurface::GreenLeft) << DSurface::GreenRight)
        | (((unsigned char)(color >> 16) >> (unsigned char)DSurface::BlueLeft) << DSurface::BlueRight);
	int x = area.X;
	int y = area.Y;
	int width = area.Width;
	int height = area.Height;
	if (!font) font = BitFont::Instance;
	int text_width, text_height;
	font->Text_Dimensions(text, &text_width, &text_height, width);
	if (x_alignment == 1) x += (width - text_width + 1) / 2;
	else if (x_alignment == 2) x -= (text_width + 1) / 2;
	else if (x_alignment == 3) x += -1 - text_width;
	else if (x_alignment == 4) x += width - text_width - 1;
	if (y_alignment == 1) y += (height - text_height + 1) / 2;
	else if (y_alignment == 2) y -= (text_height + 1) / 2;
	else if (y_alignment == 3) y += -1 - text_height;
	tagRECT clip;
	clip.left = area.X;
	clip.top = area.Y;
	clip.right = clip.left + width;
	clip.bottom = clip.top + height;
	font->Set_Clip(1);
	font->Set_Bounds((LTRBStruct *)&clip);
	font->Set_Color((unsigned short)pixel);
	int end = BitText::Instance->Print(font, surface, text, x, y, length, shown);
	return end - x;
}


void __fastcall Dialog_On_WM_DRAWITEM(long item)
{
	DRAWITEMSTRUCT * draw = (DRAWITEMSTRUCT *)item;
	if (VisibleSurface && AlternateSurface) {
		RECT area;
        RECT client;
		Get_Display_Rect(draw->hwndItem, &area);
		GetClientRect(draw->hwndItem, &client);
		if (GetWindowLongA(draw->hwndItem, GWL_STYLE) & WS_BORDER) {
			int border_x = GetSystemMetrics(SM_CXBORDER);
			int border_y = GetSystemMetrics(SM_CYBORDER);
			area.left += border_x;
			area.right -= border_x;
			area.top += border_y;
			area.bottom -= border_y;
        }
		if (draw->CtlType == ODT_BUTTON) {
			HWND window = draw->hwndItem;
			OwnerDraw::WinData * record = 0;
			Control_Table.getPointer((unsigned int)window, &record);
			if (record) {
				record->ItemState = draw->itemState;
				InvalidateRect(window, &draw->rcItem, FALSE);
                UpdateWindow(window);
            }
        }
    }
}


namespace OwnerDraw {

void __fastcall Draw_Dimmed_Back(Rect const & area, HWND window, int alpha)
{
	WinData * record = 0;
	WinData * parent_record = 0;
	Control_Table.getPointer(*(unsigned int *)&window, &record);
    RECT client;
    GetClientRect(window, &client);
	RECT screen;
	Get_Display_Rect(window, &screen);
	Surface * surface = record->Back;
	if (surface == 0) {
		surface = new BSurface(client.right + 1, client.bottom + 1, 2);
		record->Back = surface;
		Surface_Count++;
		Rect full(0, 0, client.right + 1, client.bottom + 1);
        HWND parent = GetParent(window);
		Control_Table.getPointer(*(unsigned int *)&parent, &parent_record);
		Surface * parent_surface = parent_record->Back;
		if (parent_surface) {
			RECT parent_screen;
			Get_Display_Rect(parent, &parent_screen);
			Rect from;
			from.X = screen.left - parent_screen.left;
			from.Y = screen.top - parent_screen.top;
			from.Width = screen.right - screen.left + 1;
			from.Height = screen.bottom - screen.top + 1;
			surface->Blit_From(full, *parent_surface, from, false, true);
        } else {
			Surface * source = VisibleSurface;
			Rect from;
			from.X = screen.left;
			from.Y = screen.top;
			from.Width = screen.right - screen.left + 1;
			from.Height = screen.bottom - screen.top + 1;
			if (source) {
				surface->Blit_From(full, *source, from, false, true);
            }
        }
		unsigned short * pixels = (unsigned short *)surface->Lock();
		if (pixels) {
			for (int i = 0; i < surface->Get_Width() * surface->Get_Height(); i++) {
				pixels[i] = Blend_Color(pixels[i], 0, alpha);
            }
            surface->Unlock();
        }
    }
	Rect from;
	from.X = area.X;
	from.Y = area.Y;
	from.Width = area.Width;
	from.Height = area.Height;
	from.X -= screen.left;
	from.Y -= screen.top;
	AlternateSurface->Blit_From(area, *surface, from, false, true);
}

}


namespace OwnerDraw {

void __fastcall Draw_Progress_Fill(Rect const & area, Surface * surface, int color, int fraction)
{
    int fade = 1;
	int run = (area.Width * fraction) >> 16;
    if (run < 0) return;
    if (run == 0) run = 1;
	unsigned short * pixels = (unsigned short *)surface->Lock();
	if (!pixels) return;
	for (int row = 0; row < area.Height; row++) {
		int index = area.X + (area.Y + row) * (surface->Stride() / 2);
		int quarter = area.Height / 4;
		if (row == quarter * 3) {
            fade = 1;
        }
		if (row == quarter) {
            fade = 0;
        }
        if (run > 0) {
			unsigned short * pixel = &pixels[index];
            for (int i = 0; i < run; i++) {
                if (fade) {
					int alpha = (255 * (i + 1)) / area.Width;
                    *pixel = Blend_Color(*pixel, color, alpha);
                } else {
                    *pixel = color;
                }
				pixel++;
            }
        }
    }
    surface->Unlock();
}

}


namespace OwnerDraw {
void __fastcall Shade_Edges(Rect const & area, Surface & surface, int left_width, int top_height)
{
	unsigned short * pixels = (unsigned short *)surface.Lock();

	if (pixels != 0) {
        int stride = surface.Stride() / 2;
        int y;

		for (y = area.Y; y < area.Y + area.Height; ++y) {
			int index = y * stride + area.X;

			for (int x = area.X; x < area.X + left_width; ++x) {
				unsigned short pixel = pixels[index];

				pixels[index] = ((pixel >> 1) & ((Red_Mask >> 1) & Red_Mask)) |
					((pixel >> 1) & ((Green_Mask >> 1) & Green_Mask)) |
					((pixel >> 1) & (Blue_Mask >> 1));

                index++;
            }
        }

		for (y = area.Y; y < area.Y + top_height; ++y) {
			int index = y * stride + area.X + left_width;

			for (int x = area.X + left_width; x < area.Width + area.X; ++x) {
				unsigned short pixel = pixels[index];

				pixels[index] = ((pixel >> 1) & ((Red_Mask >> 1) & Red_Mask)) |
					((pixel >> 1) & ((Green_Mask >> 1) & Green_Mask)) |
					((pixel >> 1) & (Blue_Mask >> 1));

                index++;
            }
        }

        surface.Unlock();
    }
}
}


void __fastcall Blend_Rect(Rect * area, Surface * surface, int color, int alpha)
{
	unsigned short * pixels = (unsigned short *)surface->Lock();
	if (pixels) {
		int stride = surface->Stride() / 2;
		for (int y = area->Y; y < area->Y + area->Height; ++y) {
			int index = y * stride + area->X;
			for (int x = area->X; x < area->X + area->Width; ++x) {
				pixels[index] = OwnerDraw::Blend_Color(pixels[index], color, alpha);
                ++index;
            }
        }
        surface->Unlock();
    }
}


namespace OwnerDraw {

void __fastcall Draw_Bits(Rect const * area, Surface * surface)
{
	unsigned char palette[768];
	Surface * image = SurfaceCache.GetSurface("bits_i.pcx", (void *)palette);
	Surface * alpha = SurfaceCache.GetSurface("bits_a.pcx", 0);
	int count = 0;
	SurfaceCacheConvertPalette(palette);
	int x = 0;
	Rect rect = *area;
	if (x < area->Width) {
        do {
			int frame = rand() % 8;
			rect.X = area->X + x;
			rect.Y = area->Y + 1;
			rect.Width = 10;
			rect.Height = 12;
			SurfaceCache.DrawMasked(rect, *surface, *image, *alpha, palette, 0, frame * 10, 0);
			frame = rand() % 8;
			rect.X = area->X + x;
			rect.Width = 10;
			rect.Height = 12;
			rect.Y = area->Y + area->Height - 16;
			SurfaceCache.DrawMasked(rect, *surface, *image, *alpha, palette, 0, frame * 10, 0);
			count++;
			if (count % 6 == 0) {
                x += 10;
            }
            x += 10;
		} while (x < area->Width);
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


void __fastcall Dialog_On_WM_PAINT(HWND window)
{
	using namespace OwnerDraw;
	WinData * entry = Get_Record(window);
	if (entry == NULL) {
		return;
	}
	RECT client;
	GetClientRect(window, &client);

	RECT screen;
	Get_Display_Rect(window, &screen);

	Rect full;
	full.Set(screen.left, screen.top, screen.right - screen.left, screen.bottom - screen.top);
	int client_width = client.right;
	if (full.Width <= client_width) {
		full.Width = client_width;
	}
	int client_height = client.bottom;
	if (full.Height <= client_height) {
		full.Height = client_height;
	}

	if (entry->SkipCheckPaint == 0) {
		Surface * surface = entry->Back;
		if (!surface) {
			surface = new BSurface(client.right, client.bottom, 2);
			entry->Back = surface;
			Surface_Count++;
		}

		int mode = entry->ButtonKind;
		if (mode == 1) {
			if (Session.Is_In_Game()) {
				Draw_Sidebar_Art(surface, full, false);
			} else if (Is_Interface_Art_Loaded()) {
				bool active = !entry->IsWaiting;
				Draw_Interface_Frame(surface, full, active);

				ConvertClass * drawer = (ConvertClass *)Control_Drawer(window);
				ShapeSet * shape640 = (ShapeSet *)Control_Shape_640(window);
				ShapeSet * shape = (ShapeSet *)Control_Shape(window);
				Draw_Dialog_Backdrop(surface, full, drawer, shape640, shape);

				if (entry->IsLobbyArt) {
					Draw_Slide_Top(surface, full);
				}
				if (entry->IsLobbyFrame) {
					Draw_Slide_MP_Button(surface, full);
				}
				if (Is_Chat_Dialog(window)) {
					Draw_Score_Top(surface, full);
				}
			}
		} else if (mode == 2) {
			Rect to = full;
			to.X = 0;
			to.Y = 0;
			surface->Blit_From(to, *AlternateSurface, full, false, true);

			void * shape;
			if (Session.Is_In_Game()) {
				bool allied = Scen->IsGDI;
				if (allied) {
					shape = ArtPopupBackAllied;
				} else {
					shape = ArtPopupBackSoviet;
				}
			} else {
				shape = ArtPopupBack;
			}
			ConvertClass * drawer = Get_Dialog_Drawer();
			Draw_Shape(*surface, *drawer, (ShapeSet const *)shape, 0, Point2D(0, 0), to, SHAPE_WIN_REL);
		} else {
			Surface * back = SurfaceCache.GetSurface(OwnerDraw::Dialog_Back_Name, 0);

			Rect to = full;
			Rect from = full;
			to.X -= screen.left;
			to.Y -= screen.top;

			int width = back->Get_Width();
			int height = back->Get_Height();
			int screen_width = VideoModeWidth;
			if (screen_width > width) {
				from.X += (screen_width - width) / -2;
			}
			int screen_height = VideoModeHeight;
			if (screen_height > height) {
				from.Y += (screen_height - height) / -2;
			}
			surface->Blit_From(to, *back, from, false, true);
		}

		if (surface) {
			Rect from = full;
			from.X -= screen.left;
			from.Y -= screen.top;
			AlternateSurface->Blit_From(full, *surface, from, false, true);
		}
	}
}

namespace OwnerDraw {

}


void On_WM_NCDESTROY(HWND window) // RA2: 0x5FF380 (ra2-decomp's twin 0x6223C0)
{
	WinData * record = 0;
	Control_Table.getPointer((unsigned int const &)window, &record);
	if (record && record->Back) {
		delete record->Back;
		record->Back = 0;
		--Surface_Count;
    }
}


BOOL __stdcall Redraw_Window(HWND window, RECT * rect)
{
	InvalidateRect(window, rect, 0);
    UpdateWindow(window);
	return 1;
}


namespace OwnerDraw {

class WindowList {
	public:
		int Count;
		int Capacity;
		HWND * Items;
		char Grow()
    {
			if (Count < Capacity) return 0;
			int capacity = Count * 2;
			if (capacity < 10) capacity = 10;
			HWND * items = (HWND *)(new unsigned char[capacity * sizeof(HWND)]);
			memset(items, 0, capacity * sizeof(HWND));
			if (Items != 0) memcpy(items, Items, Count * sizeof(HWND));
			delete[] (unsigned char *)Items;
			Items = items;
			Capacity = capacity;
			return 1;
    }
		char Add(HWND & window, int index)
    {
			if (index > Count) index = Count;
			if (index >= Capacity) Grow();
			if (Count >= Capacity) Grow();
			if (index < Count) memmove(Items + index + 1, Items + index, sizeof(HWND) * (Count - index));
			new ((void *)(Items + index)) HWND((HWND &)window);
			Count++;
			return 1;
    }
		int Length() const { return Count; }
};

BOOL __stdcall Add_Window(HWND window, WindowList * list)
{
	if (list != 0) {
		list->Add(window, list->Length());
    }
	return TRUE;
}

}


namespace OwnerDraw {

int Release_Mouse_For_Dialog()
{
	if (MouseCursor && MouseCursor->Is_Captured() == true) {
        MouseCursor->Release_Mouse();
    }
	return ++Mouse_Release_Depth;
}

}


namespace OwnerDraw {

int Capture_Mouse_After_Dialog()
{
	if (Mouse_Release_Depth > 0) {
		--Mouse_Release_Depth;
    }
	if (Mouse_Release_Depth == 0 && MouseCursor && !MouseCursor->Is_Captured()) {
        MouseCursor->Capture_Mouse();
    }
	return Mouse_Release_Depth;
}

}


namespace OwnerDraw {

HWND __fastcall Begin_Dialog(int resource, DLGPROC proc, LPARAM param)
{
	unsigned short id = (unsigned short)resource;
	DLGTEMPLATE const * dialog_template = (DLGTEMPLATE const *)Fetch_Resource((char const *)id, (char const *)5);
	if (!dialog_template) {
		return 0;
    }
	int index = g_DialogCount;
	DialogInitParam init;
	g_Dialogs[index].handle = 0;
	g_Dialogs[index].id = 0;
    g_DialogCount++;
	init.Resource = id;
	init.Param = param;
	HWND window = CreateDialogIndirectParamA(ProgramInstance, dialog_template, MainWindow, (DLGPROC)proc, (LPARAM)&init);
	if (!window) {
        g_DialogCount--;
		return 0;
    }
	g_Dialogs[index].handle = window;
	g_Dialogs[index].id = id;
	Add_Modeless_Dialog(window); // RA2: 0x005B6000, the modeless dialog list (its byte-identical twin 0x522B20 is the audio file window list)
	g_TopWindow = window;
	g_TopWindowID = id;
	return window;
}

}


namespace OwnerDraw {

void __fastcall End_Dialog(HWND window)
{
    Keyboard->Clear();
	OwnerDraw::WS_Save_Dialog_Values(window);
    DestroyWindow(window);
    for (int index = 0; index < g_DialogCount; index++) {
        if (g_Dialogs[index].handle == window) {
			memmove(&g_Dialogs[index], &g_Dialogs[index + 1], (g_DialogCount - (index + 1)) * sizeof(WSDialogStruct));
			g_Dialogs[g_DialogCount - 1].handle = 0;
			g_Dialogs[g_DialogCount - 1].id = 0;
            g_DialogCount--;
            if (g_DialogCount > 0) {
				g_TopWindow = g_Dialogs[g_DialogCount - 1].handle;
                g_TopWindowID = g_Dialogs[g_DialogCount - 1].id;
                SetForegroundWindow(g_TopWindow);
                SetFocus(g_TopWindow);
            } else {
				g_TopWindow = 0;
                g_TopWindowID = 0;
                SetForegroundWindow(MainWindow);
                SetFocus(MainWindow);
            }
            return;
        }
    }
}

}


namespace OwnerDraw {

void __fastcall Display_Dialog(HWND window)
{
    ShowWindow(window, SW_SHOWNORMAL);
    SetForegroundWindow(window);
    Keyboard->Clear();
}

}


void __fastcall Show_Dialog(HWND dialog, int kind) // RA2: 0x005FF7E0, ra2-decomp's UI::RegisterWindow (Yuri's Revenge 0x00622820) statement for statement: the controls are subclassed, then the dialog is set up for its kind. With the twin's three one-call wrappers and its two scalar sizes the function is under the size limit of a function not declared inline, and Default_Dialog_Proc expands it twice
{
	using namespace OwnerDraw;
	Subclass_Dialog(dialog, 0);
	Resize_Dialogs(dialog);
	Set_Dialog_Kind(dialog, kind);
	if (!Session.Is_In_Game()) {
		Set_Control_Art((unsigned int)dialog);
	}
	Update_Lobby_Art_Dialog(dialog);
	Update_Lobby_Frame_Dialog(dialog);
	Update_Chat_Dialog(dialog);
	Update_Setup_Dialog(dialog);
	Assign_All_Control_Sounds(dialog);
	if (Is_Framed_Dialog(dialog)) {
		int layout[2] = { LowResWidth, LowResHeight };
		Resize_Dialog_To_Screen(dialog, (Point2D *)layout);
	} else {
		Centre_Dialog(dialog);
	}
	Assign_Button_Kinds(dialog);
	Init_Control_Animations(dialog);
	Center_Window_Within_Window(dialog);
}


/*
**	RA2: the default handling of an owner-drawn dialog (TibSun's, with RA2's tooltip label, dimmed
**	painting, pending slides and the per-kind dialog setup).
*/
namespace OwnerDraw {

int OwnerDraw::Default_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005FFB00, ra2-decomp's twin text (UI::StandardWndProc, Yuri's Revenge 0x00622B50) in this tree's names and the twin's case order
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
			WinData * data = Get_Data(window);
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

		case WM_INITDIALOG:
			++_dialog_count;
			if (lparam != 0) {
				Show_Dialog(window, *(unsigned short *)lparam);
			} else {
				Show_Dialog(window, 0);
			}
			SetFocus(window);
			return(0);

		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			return((int)GetStockObject(BLACK_BRUSH));

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

		case OD_SUBCLASSED:
			SendMessage(window, OD_SETTOP, (WPARAM)window, 1);
			return(0);

		case WM_USER + 236: // RA2: the slide-in finished: the tested controls reset
			Dialog_On_OD_4EC(window);
			return(0);
	}
	return(0);
}

}


/// <summary>
/// Services the game while a dialog is up.
/// A dialog's own message pump calls this routine every pass. It dispatches the pending
/// Windows messages and then either runs the game logic loop -- so that a multiplayer
/// game keeps up while the dialog is showing -- or just the maintenance callback.
/// </summary>
/// <returns>bool; Has the game ended, so that the dialog should be shut down?</returns>
namespace OwnerDraw {

bool Dialog_Message_Handler()
{
    Windows_Message_Handler();
	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH || Session_Busy ||
		Session.Suspended != 0) {
        Call_Back();
		return false;
    }
	if (!Is_In_Main_Loop() && Main_Loop()) {
		return true;
    }
	return false;
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
void __fastcall Move_Window(HWND window, int x, int y)
{
	RECT screen;
    RECT rect;
	screen.right = VideoModeWidth;
	screen.left = 0;
	screen.top = 0;
	screen.bottom = VideoModeHeight;
	ClientToScreen(MainWindow, (POINT *)&screen.left);
	ClientToScreen(MainWindow, (POINT *)&screen.right);
	GetWindowRect(window, &rect);
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = x == -1 ? rect.left - screen.left : x;
	rect.top = y == -1 ? rect.top - screen.top : y;
	MoveWindow(window, rect.left, rect.top, rect.right, rect.bottom, FALSE);
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
namespace OwnerDraw {

HWND __fastcall Custom_Message_Box(unsigned short const * text, unsigned short const * button, bool * cancelled)
{
	HWND dialog = OwnerDraw::Begin_Dialog(0xf0, Cancel_Dialog_Proc, 0);
	if (dialog) {
		SetWindowLongA(dialog, DWL_USER, (LONG)cancelled);
		HWND label = GetDlgItem(dialog, 0x5b0);
		SendMessageA(label, OD_SETTEXTW, 0, (LPARAM)text);
		if (button) {
			HWND item = GetDlgItem(dialog, 0x5c0);
			SendMessageA(item, OD_SETTEXTW, 0, (LPARAM)button);
			EnableWindow(item, TRUE);
			ShowWindow(item, SW_SHOW);
        }
    }
	return dialog;
}

}


/// <summary>
/// Handles the messages for a custom message box.
/// This routine lets the default dialog handler have first refusal of the message and
/// then watches for the cancel button. Cancelling feeds an escape key to the game
/// keyboard and raises the flag the box was opened with.
/// </summary>
/// <returns>Returns with the message result, or zero when the message was consumed here.</returns>
int __stdcall Cancel_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) {
		return result;
    }
	if (message == WM_COMMAND && wparam == 0x5c0) {
		bool * cancelled = (bool *)GetWindowLongA(window, DWL_USER);
        if (cancelled) {
			Keyboard->Put((KeyNumType)KN_ESC);
            *cancelled = true;
        }
    }
	return 0;
}


/// <summary>
/// Sets the text displayed by a custom message box.
/// Use this routine to change the prompt of a message box that is already on the screen.
/// The box is repainted before the routine returns.
/// </summary>
namespace OwnerDraw {

WinData::WinData()
{
	memset(this, 0, sizeof(WinData));
	SkipTests = 0xb;
	Font = BitFont::Instance;
	WideText = new WideWstring;
	Caret = 0;
	Reserved_5C = 0;
	TypingSound = -1;
}

}


namespace OwnerDraw {
inline void WinData::Copy_List(TextNode const * source)
{
	while (Nodes) {
		TextNode * node = Nodes;
		TextNode * previous = 0;
		TextNode * current;
		for (current = Nodes; current; current = current->Next) {
			if (node == current) {
                break;
            }
			previous = current;
        }
		if (current) {
			if (previous) {
				previous->Next = current->Next;
            } else {
				Nodes = current->Next;
            }
			operator delete(current);
        }
    }
	for (; source; source = source->Next) {
		TextNode * node = (TextNode *)operator new(wcslen(source->Data) * 2 + 0x12);
		if (node) {
			node->Data = node->Text;
			node->Next = Nodes;
			node->Reserved_04 = 0;
			Nodes = node;
        }
		node->Reserved_0C = source->Reserved_0C;
		node->Reserved_04 = source->Reserved_04;
		wcscpy(node->Data, source->Data);
    }
}
}


namespace OwnerDraw {

void __fastcall Set_Custom_Message_Box_Text(HWND dialog, wchar_t const * text)
{
	HWND item = GetDlgItem(dialog, 0x5B0);
	SendMessageA(item, OD_SETTEXTW, 0, (LPARAM)text);
	UpdateWindow(dialog);
}

}


namespace OwnerDraw {

void WinData::Set_Text(char const * text)
{
	if (Text) {
		operator delete(Text);
		Text = 0;
    }
	if (text && *text) {
		Text = (wchar_t *)operator new((strlen(text) + 1) * sizeof(wchar_t));
		swprintf(Text, L"%hs", text);
    }
}

}


namespace OwnerDraw {

inline WinData & WinData::operator=(WinData const & other)
{
	TextNode * list = Nodes;
	wchar_t * text = Text;
	WideWstring * kept_3c = WideText;
	if (this == &other) {
		return *this;
    }
	memcpy(this, &other, sizeof(WinData));
	Text = text;
	WideText = kept_3c;
	Nodes = list;
	Set_Wide_Text(other.Text);
	Copy_List(other.Nodes);
	return *this;
}

}


#ifndef RA2_OWNERDRAW_CHECK_FIELDS
#endif

namespace OwnerDraw {

WinData::WinData(WinData const & other) // RA2: 0x6006E0 (ra2-decomp's twin 0x623730), never called
{
	TextNode * list = Nodes;
	wchar_t * text = Text;
	WideWstring * kept_3c = WideText;
	memcpy(this, &other, sizeof(WinData));
	Text = text;
	WideText = kept_3c;
	Nodes = list;
	wchar_t * source = other.Text;
	if (Text) {
		operator delete(Text);
		Text = 0;
    }
	if (source && *source) {
		Text = (wchar_t *)operator new(wcslen(source) * 2 + 2);
		wcscpy(Text, source);
    }
    {
		TextNode const * source_list = other.Nodes;
		while (Nodes) {
			TextNode * node = Nodes;
			TextNode * previous = 0;
			TextNode * current;
			for (current = Nodes; current; current = current->Next) {
				if (node == current) {
                break;
            }
				previous = current;
        }
			if (current) {
				if (previous) {
					previous->Next = current->Next;
            } else {
					Nodes = current->Next;
            }
				operator delete(current);
        }
    }
		for (; source_list; source_list = source_list->Next) {
			TextNode * node = (TextNode *)operator new(wcslen(source_list->Data) * 2 + 0x12);
			if (node) {
				node->Data = node->Text;
				node->Next = Nodes;
				node->Reserved_04 = 0;
				Nodes = node;
        }
			node->Reserved_0C = source_list->Reserved_0C;
			node->Reserved_04 = source_list->Reserved_04;
			wcscpy(node->Data, source_list->Data);
    }
    }
}

}


int __fastcall Clear_ComboBox(HWND combo)
{
	int index = SendMessageA(combo, CB_GETCOUNT, 0, 0);
	while (index--) {
		SendMessageA(combo, CB_DELETESTRING, index, 0);
    }
	return 0;
}


namespace OwnerDraw {


template<class T> inline T Max_Of(T a, T b) { return a < b ? b : a; }

void __fastcall Draw_Edit_Text(Surface * surface, Rect & rect, wchar_t const * text, int caret,
	BitFont * font, unsigned int color, int * scroll, int ime,
	int password, int frame, int shown)
{
	wchar_t buffer[0x800];
	int caret_x = -1;
	int composition_length = 0;
	int composing = 0;
	if (text == 0) {
		text = L"";
    }
	int text_length = wcslen(text);
	if (caret < 0 || caret > text_length) {
		caret = 0;
    }
	if (ime) {
		IME_Update_Composition();
		composition_length = IMECompositionStringLength;
		composing = IMEComposing;
    }
	buffer[0] = 0;
	wcsncpy(buffer, text, 0x800);
	if (composition_length) {
		wcsncpy(&buffer[caret], IMECompositionString, 0x800 - caret);
		wcsncpy(&buffer[composition_length + caret], &text[caret], 0x800 - composition_length - caret);
    }
	buffer[0x7FF] = 0;
	int length = wcslen(buffer);
	if (password && length > 0) {
		wchar_t * p = buffer;
		for (int i = length; i != 0; i--) {
            *p++ = '*';
        }
    }
	int cursor;
	if (composing || composition_length) {
		cursor = IMECompositionCursorPos + caret;
    } else {
		cursor = caret;
    }
	if (cursor < *scroll + 5) {
		if (cursor >= 5) {
            *scroll = cursor - 5;
        } else {
            *scroll = 0;
        }
    }
	while (*scroll < length) {
		if (font->Chars_In_Width(&buffer[*scroll], rect.Width - 5, 0, 0) >= cursor - *scroll) {
            break;
        }
		(*scroll)++;
    }
	int before = caret - *scroll;
	int lead = Max_Of(before, 0);
	int composed = composition_length;
	if (before < 0) {
		before += composition_length;
		composed = Max_Of(before, 0);
    }
	int after = length - composition_length - caret;
	if (before < 0) {
		after += before;
    }
	if (after < 0) {
		after = 0;
    }
	text = &buffer[*scroll];
	if (frame) {
		Rect area(rect.X - 1, rect.Y - 1, font->String_Pixel_Width(text, 0) + 5, rect.Height + 2);
		surface->Fill_Rect(area, 0);
    }
	if (lead) {
		int width = OD_Draw_Text(color, font, rect, text, lead, 0, 0, surface, shown);
		rect.X += width;
		rect.Width -= width;
		if (rect.Width < 0) {
			rect.Width = 0;
        }
		text += lead;
    }
	if (composed) {
		cursor -= *scroll + lead;
		int rest;
		if (cursor < 0 || cursor > composed) {
			rest = -1;
        } else {
			rest = composed - cursor;
			composed -= rest;
        }
		if (composed > 0) {
			int width = OD_Draw_Text(Tab_Text_Color, font, rect, text, composed, 0, 0, surface, shown);
			rect.X += width;
			rect.Width -= width;
			if (rect.Width < 0) {
				rect.Width = 0;
            }
			text += composed;
        }
		if (rest >= 0) {
			caret_x = rect.X;
			if (rest) {
				int width = OD_Draw_Text(Tab_Text_Color, font, rect, text, rest, 0, 0, surface, shown);
				rect.X += width;
				rect.Width -= width;
				if (rect.Width < 0) {
					rect.Width = 0;
                }
				text += rest;
            }
        }
    } else {
		caret_x = rect.X;
    }
	if (after) {
		int width = OD_Draw_Text(color, font, rect, text, after, 0, 0, surface, shown);
		rect.X += width;
		rect.Width -= width;
		if (rect.Width < 0) {
			rect.Width = 0;
        }
    }
	if (ime && caret_x > 0 && !Caret_Hidden) {
		Draw_Edge_Glow(surface, Point2D(caret_x, rect.Y), Point2D(caret_x, rect.Y + rect.Height - 2),
			White_Color, 0xFF);
		Draw_Edge_Glow(surface, Point2D(caret_x + 1, rect.Y), Point2D(caret_x + 1, rect.Y + rect.Height - 2),
			White_Color, 0xFF);
    }
	Caret_Hidden = 0;
}

}


bool __fastcall Prompt_Dialog(unsigned short const * text)
{
	int result = -1;
	HWND dialog = OwnerDraw::Begin_Dialog(0x10d, Choice_Dialog_Proc, 0);
	if (dialog) {
		HWND item = GetDlgItem(dialog, 0x5b0);
		SendMessageA(item, OD_SETTEXTW, 0, (LPARAM)text);
		SetWindowLongA(dialog, DWL_USER, (LONG)&result);
		OwnerDraw::Display_Dialog(dialog);
		while (result < 0) {
            Windows_Message_Handler();
			if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && !Session_Busy &&
				Session.Suspended == 0 && !Is_In_Main_Loop() && Main_Loop()) {
                break;
            }
        }
		OwnerDraw::End_Dialog(dialog);
    }
	return result == 0x5c1;
}


int __stdcall Choice_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) {
		return result;
    }
	int * choice = (int *)GetWindowLongA(window, DWL_USER);
	if (message == WM_COMMAND) {
		int id = wparam & 0xffff;
		if (id >= 0x5c0 && id <= 0x5c1) {
            *choice = id;
        }
    }
	return 0;
}


#ifndef RA2_OWNERDRAW_CHECK_FIELDS
#endif

int __fastcall Clear_List_Box(HWND listbox)
{
	int index = SendMessageA(listbox, LB_GETCOUNT, 0, 0);
	while (index--) {
		SendMessageA(listbox, LB_DELETESTRING, index, 0);
    }
	return 0;
}


LRESULT __fastcall Get_Control_Text(HWND window, Wstring & text)
{
	LRESULT length = SendMessageA(window, WM_GETTEXTLENGTH, 0, 0);
	if (length <= 0) {
		text.clear();
		return 0;
    }
	char * buffer = new char[length + 2];
	LRESULT result = SendMessageA(window, 0x4B5, length + 1, (LPARAM)buffer);
	text = buffer;
	delete[] buffer;
	return result;
}


LRESULT __fastcall Get_Control_Wide_Text(HWND window, WideWstring & text)
{
	LRESULT length = SendMessageA(window, WM_GETTEXTLENGTH, 0, 0);
	if (length <= 0) {
		text.clear();
		return 0;
    }
	unsigned short * buffer = new unsigned short[length + 2];
	LRESULT result = SendMessageA(window, 0x4B3, length + 1, (LPARAM)buffer);
	text = buffer;
	delete[] buffer;
	return result;
}


LRESULT __fastcall Get_List_Item_Text(HWND window, WPARAM index, Wstring & text)
{
	LRESULT length = SendMessageA(window, LB_GETTEXTLEN, index, 0);
	if (length <= 0) {
		text.clear();
    }
	char * buffer = new char[length + 2];
	LRESULT result = SendMessageA(window, 0x4B7, index, (LPARAM)buffer);
	text = buffer;
	delete[] buffer;
	return result;
}


LRESULT __fastcall Get_List_Item_Wide_Text(HWND window, WPARAM index, WideWstring & text)
{
	LRESULT length = SendMessageA(window, LB_GETTEXTLEN, index, 0);
	if (length <= 0) {
		text.clear();
    }
	unsigned short * buffer = new unsigned short[length + 2];
	LRESULT result = SendMessageA(window, 0x4B6, index, (LPARAM)buffer);
	text = buffer;
	delete[] buffer;
	return result;
}


LRESULT __fastcall Get_Combo_Item_Text(HWND window, WPARAM index, Wstring & text)
{
	LRESULT length = SendMessageA(window, CB_GETLBTEXTLEN, index, 0);
	if (length <= 0) {
		text.clear();
    }
	char * buffer = new char[length + 2];
	LRESULT result = SendMessageA(window, 0x4BD, index, (LPARAM)buffer);
	text = buffer;
	delete[] buffer;
	return result;
}


LRESULT __fastcall Get_Combo_Item_Wide_Text(HWND window, WPARAM index, WideWstring & text)
{
	LRESULT length = SendMessageA(window, CB_GETLBTEXTLEN, index, 0);
	if (length <= 0) {
		text.clear();
    }
	unsigned short * buffer = new unsigned short[length + 2];
	LRESULT result = SendMessageA(window, 0x4C3, index, (LPARAM)buffer);
	text = buffer;
	delete[] buffer;
	return result;
}


OwnerDraw::DialogControl::DialogControl() {}
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


namespace OwnerDraw {
int __fastcall Controlled_Dialog(int resource, DLGPROC procedure, DialogControl * control)
{
	HWND window = Begin_Dialog(resource, procedure, (int)control);
	if (!window) {
		control->Set_Window(0, 0);
		return 0;
    }
	ShowWindow(window, SW_SHOWNORMAL);
    SetForegroundWindow(window);
    Keyboard->Clear();
	while (!control->Is_Done() && !control->Is_Cancelled()) {
		if (Dialog_Message_Handler()) break;
		Title_Screen_Restore(false);
    }
	End_Dialog(window);
	return (int)control->Get_Window();
}
}


namespace OwnerDraw {
bool __fastcall Control_Test(HWND window)
{
	char name[128];
	GetClassNameA(window, name, sizeof(name));
	if (!strcmp(name, "Edit")) return true;
	if (!strcmp(name, "NewEdit")) return true;
	if (!strcmp(name, "ListBox")) {
		WinData * record = 0;
		unsigned int const & key = (unsigned int const &)window;
		if (Control_Table.getPointer(key, &record)) return record->SkipTests == 1;
    }
	if (!strcmp(name, "ComboBox")) return (GetWindowLongA(window, GWL_STYLE) & 3) != CBS_DROPDOWNLIST;
	return false;
}
}


namespace OwnerDraw {

void WinData::Set_Wide_Text(wchar_t const * text)
{
	if (Text != NULL) {
		delete[] Text;
		Text = NULL;
	}
	if (text != NULL && *text != L'\0') {
		Text = new wchar_t[wcslen(text) + 1];
		wcscpy(Text, text);
	}
}

}


TextNode * NodeList::Add_Node(int length)
{
	TextNode * node = (TextNode *)operator new(length * 2 + 0x12);
	if (node != 0) {
		node->Data = node->Text;
		node->Next = Head;
		node->Reserved_04 = 0;
		Head = node;
    }
	return node;
}

#ifdef RA2_NON_MATCHING // not exact: 6538 bytes differ against the image-derived bound 005f4ba0 (6,895 body bytes) (the background pixel count computed before the surface lock, as RA2 1.001 and the image do, was 6541; image-faithful reads; position score 6541, was 6501: the dropdown handle is read from WinData again after ReleaseCapture and after notifying the parent, as the image does at 005f587b and 005f589e (RA2 1.001 cac41ca); owner ruling 2026-11-25; before that 6502; the forwarded dropdown result returned on its own path, as ra2-decomp's attempt f9e49e2 separates it; the dropdown's visible rows computed before the count is read, as ra2-decomp's banked form banks/0x00617250_odh.cpp (11df85d) has it; the item-data and selection cases first in the bank's order, RA2 case bodies and fall-through groups kept; the text query's shared result with the signed clamp and the item sentinel tested before null as the bank has them)
namespace OwnerDraw {

LRESULT __stdcall Combo_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WNDPROC previous = NULL;
	Window_Proc_Table.getValue(window, previous);
	RECT display, client, screen;
	Get_Display_Rect(window, &display);
	GetClientRect(window, &client);
	GetWindowRect(window, &screen);
	WinData * data = NULL;
	Control_Table.getPointer((unsigned int)window, &data);
	if (!data) return CallWindowProcA(previous, window, message, wparam, lparam);

	switch (message) {
		case WM_DESTROY:
			SendMessageA(window, CB_SHOWDROPDOWN, FALSE, 0);
			break;
		case CB_GETITEMDATA:
		case CB_SETITEMDATA:
		case LB_GETITEMDATA:
		case LB_SETITEMDATA: {
			TextNode * node = (TextNode *)CallWindowProcA(previous, window, CB_GETITEMDATA, wparam, 0);
			if (!node || node == (TextNode *)CB_ERR) return CB_ERR;
			if (message == CB_GETITEMDATA || message == LB_GETITEMDATA) return node->Reserved_04;
			node->Reserved_04 = lparam;
			return (LRESULT)node;
		}
		case CB_GETCURSEL:
			return data->HadTabStop;
		case CB_SETCURSEL: {
			data->HadTabStop = wparam;
			if (wparam == (WPARAM)-1) {
				SendMessageA(window, 0x4b4, 0, (LPARAM) "");
			} else {
				TextNode * node = (TextNode *)CallWindowProcA(previous, window, CB_GETITEMDATA, wparam, 0);
				if (node != (TextNode *)CB_ERR && node != NULL) {
					if (node->Reserved_0C) SendMessageA(window, OD_SETTEXTW, 0, (LPARAM)node->Data);
					else {
						char text[128];
						WideToCharString(text, node->Data, sizeof(text));
						SendMessageA(window, 0x4b4, 0, (LPARAM)text);
					}
				}
			}
			InvalidateRect(window, NULL, FALSE);
			if ((GetWindowLongA(window, GWL_STYLE) & 3) == CBS_DROPDOWN) return 0;
			break;
		}
		case WM_PAINT: {
			static wchar_t text[256];
			LRESULT dropped = SendMessageA(window, CB_GETDROPPEDSTATE, 0, 0);
			RECT dropped_rect, window_rect;
			GetWindowRect(window, &window_rect);
			SendMessageA(window, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)&dropped_rect);
			Rect area(display.left, display.top, display.right - display.left, display.bottom - display.top);
			HWND parent = GetParent(window);
			WinData * parent_data = NULL;
			if (parent) Control_Table.getPointer((unsigned int)parent, &parent_data);
			RECT parent_rect;
			Get_Display_Rect(parent, &parent_rect);
			if (!data->Back) {
				Rect dest(0, 0, area.Width, area.Height);
				Rect src = dest;
				Surface * surface = new BSurface(area.Width, area.Height, 2);
				data->Back = surface;
				++Surface_Count;
				if (parent_data && parent_data->Back) {
					src.X += display.left - parent_rect.left;
					src.Y += display.top - parent_rect.top;
					surface->Blit_From(dest, *parent_data->Back, src, false, true);
			}
			int total = area.Width * area.Height;
				unsigned short * pixels = (unsigned short *)surface->Lock();
				if (pixels) {
				for (int i = 0; i < total; ++i) pixels[i] = Blend_Color(pixels[i], 0, data->TextStyle);
					surface->Unlock();
				}
			}
			Draw_Dimmed_Back(area, window, data->TextStyle);
			Blend_Rect(&area, AlternateSurface, 0, data->TextStyle);
			LONG style = GetWindowLongA(window, GWL_STYLE);
			bool disabled = (style & WS_DISABLED) != 0;
			int frame = data->IsAlternateColors ? Alternate_Frame_Color : Default_Frame_Color;
			if (disabled) frame = data->IsAlternateColors ? Dark_Grey_Color : Frame_Color;
			Draw_Frame(AlternateSurface, area, 1, Color_To_Pixel(frame));
			Rect arrow(display.right - 19, area.Y + 1, area.Width, area.Height);
			Draw_Arrow(AlternateSurface, arrow, dropped, dropped, data->IsAlternateColors);
			if (disabled) Blend_Rect(&area, AlternateSurface, 0, data->TextStyle);
			if ((style & 3) == CBS_DROPDOWNLIST) {
				int index = CallWindowProcA(previous, window, CB_GETCURSEL, 0, 0);
				WNDPROC original = NULL;
				Window_Proc_Table.getValue(window, original);
				text[0] = 0;
				TextNode * selected = (TextNode *)CallWindowProcA(original, window, LB_GETITEMDATA, index, 0);
				if (!selected || selected == (TextNode *)CB_ERR) selected = (TextNode *)CallWindowProcA(original, window, CB_GETITEMDATA, index, 0);
				if (selected && selected != (TextNode *)CB_ERR) {
					wcsncpy(text, selected->Data, 256);
					text[255] = 0;
				}
				int color = data->IsAlternateColors ? Highlight_Text_Color : Text_Color;
				if (disabled) color = data->IsAlternateColors ? Light_Grey_Color : Disabled_Text_Color;
				if (data->IsItemFilled && index >= 0 && index < 50 && data->ItemColors[index] >= 0) {
					Rect fill = area;
					fill.Width -= 20;
					Inflate_Rect(fill, -1);
					color = data->ItemColors[index];
					AlternateSurface->Fill_Rect(fill, Color_To_Pixel(color));
				}
				int length = wcslen(text);
				int width = data->Font->String_Pixel_Width(text, 0);
				while (length && width > client.right - 20) {
					text[--length] = 0;
					if (length + 3 < 256) wcscat(text, L"...");
					width = data->Font->String_Pixel_Width(text, 0);
				}
				RECT text_rect = display;
				text_rect.left += 2;
				Draw_Text_In_Rect_Ex(AlternateSurface, text, &text_rect, data->Font, color, 4, 12, 0, 0, 0);
			}
			ValidateRect(window, NULL);
			return 0;
		}
		case CB_SHOWDROPDOWN: {
			if (!wparam) {
				if (data->IsEditFocus) {
					ReleaseCapture();
					SendMessageA(GetParent(window), OD_SETTOP, data->IsEditFocus, 0);
					HWND dropdown = (HWND)data->IsEditFocus;
					DestroyWindow(dropdown);
					WinData * removed = NULL;
					Control_Table.getPointer((unsigned int)dropdown, &removed);
					if (removed && removed->Back) {
						delete removed->Back;
						removed->Back = NULL;
						--Surface_Count;
					}
					Control_Table.remove((unsigned int)dropdown);
					data->IsEditFocus = 0;
				}
				return 1;
			}
			if (data->IsEditFocus) return 1;
			RECT parent_rect, drop_rect;
			Get_Display_Rect(GetParent(window), &parent_rect);
			SendMessageA(window, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)&drop_rect);
			if (drop_rect.bottom > parent_rect.bottom) drop_rect.bottom = parent_rect.bottom;
			int item_height = SendMessageA(window, CB_GETITEMHEIGHT, 0, 0);
			if (item_height <= 0) return 0;
			int height = drop_rect.bottom - drop_rect.top;
			int visible = (height - 1) / item_height;
			int count = SendMessageA(window, CB_GETCOUNT, 0, 0);
			if (count < 1) count = 1;
			if (data->ComboStyle > 0) {
				if (count >= data->ComboStyle) count = data->ComboStyle;
				height = count * item_height + 1;
			} else if (visible > count) {
				height = count * item_height + 1;
			}
			if (display.bottom + height > parent_rect.bottom) height = parent_rect.bottom - display.bottom;
			RECT parent_display, combo_display;
			Get_Display_Rect(GetParent(window), &parent_display);
			Get_Display_Rect(window, &combo_display);
			HWND dropdown = CreateWindowExA(0, "ComboDropWin", NULL, WS_CHILD, combo_display.left - parent_display.left,
				client.bottom + combo_display.top - parent_display.top + 1, client.right, height - height % item_height,
				GetParent(window), NULL, ProgramInstance, window);
			WinData * contents = NULL;
			if (!Control_Table.getPointer((unsigned int)dropdown, &contents)) {
				WinData fresh;
				Control_Table.add((unsigned int)dropdown, fresh);
			}
			Add_Audio_File_Window(dropdown);
			Add_Audio_File_Window(dropdown);
			SendMessageA(dropdown, OD_DROPSUBCLASSED, 0, 0);
			SendMessageA(GetParent(window), OD_SETTOP, (WPARAM)dropdown, 1);
			SetCapture(dropdown);
			ShowWindow(dropdown, SW_SHOWNORMAL);
			data->IsEditFocus = (int)dropdown;
			return 1;
		}


		case WM_ERASEBKGND:
			return 0;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, NULL);
			if (LOWORD(lparam) > client.right - 20) {
				LRESULT dropped = SendMessageA(window, CB_GETDROPPEDSTATE, 0, 0);
				PostMessageA(window, CB_SHOWDROPDOWN, dropped != TRUE, 0);
			}
			return 0;
		case WM_COMMAND:
			if (HIWORD(wparam) == EN_CHANGE && (GetWindowLongA(window, GWL_STYLE) & 3) != CBS_DROPDOWNLIST) {
				SendMessageA(GetParent(window), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(window), CBN_EDITCHANGE), (LPARAM)window);
				return 0;
			}
			break;
		case WM_DELETEITEM: {
			DELETEITEMSTRUCT * item = (DELETEITEMSTRUCT *)lparam;
			TextNode ** link = &data->Nodes;
			while (*link) {
				TextNode * node = *link;
				if (node == (TextNode *)item->itemData) {
					*link = node->Next;
					operator delete(node);
					break;
				}
				link = &node->Next;
			}
			if (data->HadTabStop == item->itemID) data->HadTabStop = -1;
			break;
		}
		case WM_SETFOCUS:
		case WM_SETTEXT:
		case WM_GETTEXT:
		case WM_GETTEXTLENGTH:
		case CB_LIMITTEXT:
		case OD_SETTEXTW:
		case 0x4b3:
		case 0x4b4:
		case 0x4b5:
			if ((GetWindowLongA(window, GWL_STYLE) & 3) != CBS_DROPDOWNLIST) {
				LRESULT result = 0;
				for (HWND child = GetWindow(window, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
					char name[16];
					GetClassNameA(child, name, sizeof(name));
					if (stricmp(name, "listbox") == 0) {
						if (message == WM_SETFOCUS) SetFocus(child);
						else result = SendMessageA(child, message == CB_LIMITTEXT ? EM_LIMITTEXT : message, wparam, lparam);
					}
				}
				return result;
			}
			break;
		case 0x4b8:
		case 0x4b9:
		case 0x4ba:
		case 0x4be:
		case 0x4bf:
		case 0x4c0: {
			wchar_t buffer[0x1400];
			wchar_t const * find;
			if (message <= 0x4ba) {
				Char_To_Wide(buffer, (char const *)lparam, sizeof(buffer) / sizeof(buffer[0]));
				find = buffer;
		} else {
				find = (wchar_t const *)lparam;
		}
			int count = CallWindowProcA(previous, window, CB_GETCOUNT, 0, 0);
			if (count == CB_ERR) return 0;
			for (int index = wparam; index < count; ++index) {
				TextNode * node = (TextNode *)CallWindowProcA(previous, window, CB_GETITEMDATA, index, 0);
				if (!node || node == (TextNode *)CB_ERR) continue;
				int match = (message == 0x4b9 || message == 0x4bf) ? wcsicmp(find, node->Data) : wcsnicmp(find, node->Data, wcslen(find));
				if (match == 0) {
					if (message == 0x4ba || message == 0x4c0) return CallWindowProcA(Combo_Proc, window, CB_SETCURSEL, index, 0);
					return index;
				}
		}
			return CB_ERR;
	}
		case 0x4bb:
		case 0x4bc:
		case 0x4c1:
		case 0x4c2: {
			bool wide = message >= 0x4c1;
			bool append = message == 0x4bc || message == 0x4c2;
			char const * caption = wide ? Wide_To_Narrow((wchar_t const *)lparam) : (char const *)lparam;
			LRESULT index = CallWindowProcA(previous, window, append ? CB_ADDSTRING : CB_INSERTSTRING, append ? 0 : wparam, (LPARAM)caption);
			if (index == CB_ERR || index == CB_ERRSPACE) return index;
			int length = wide ? wcslen((wchar_t const *)lparam) : strlen(caption);
			TextNode * node = (TextNode *)operator new(offsetof(TextNode, Text) + (length + 1) * sizeof(wchar_t));
			LRESULT result = CB_ERRSPACE;
			if (node) {
				node->Data = node->Text;
				node->Next = data->Nodes;
				node->Reserved_04 = 0;
				data->Nodes = node;
				node->Reserved_0C = wide;
				if (wide) wcscpy(node->Data, (wchar_t const *)lparam);
				else Char_To_Wide(node->Data, caption, length + 1);
				result = CallWindowProcA(previous, window, CB_SETITEMDATA, index, (LPARAM)node);
				if (result != CB_ERR && result != CB_ERRSPACE) return index;
		}
			CallWindowProcA(previous, window, CB_DELETESTRING, index, 0);
			TextNode ** link = &data->Nodes;
			while (*link && *link != node) link = &(*link)->Next;
			if (*link) {
				*link = node->Next;
				operator delete(node);
		}
		return result;
	}
		case CB_GETLBTEXTLEN:
		case 0x4bd:
		case 0x4c3:
		case 0x4d0: {
			if (lparam && message == 0x4bd) *(char *)lparam = 0;
			if (lparam && message == 0x4c3) *(wchar_t *)lparam = 0;
			LRESULT result = wparam;
			if (result != CB_ERR) {
				result = CallWindowProcA(previous, window, CB_GETITEMDATA, wparam, 0);
				if (result != CB_ERR && result != 0) {
					TextNode * node = (TextNode *)result;
					if (message == 0x4d0) {
						result = node->Reserved_0C;
					} else {
						result = wcslen(node->Data);
						if (lparam && message == 0x4c3) wcscpy((wchar_t *)lparam, node->Data);
						if (lparam && message == 0x4bd) WideToCharString((char *)lparam, node->Data, -1);
					}
				}
			}
			if (result < 0) result = 0;
			return result;
	}
		case 0x4da:
		case 0x4db:
			SendMessageA(GetParent(window), message, wparam, lparam);
		break;
		case 0x4dd:
			data->IsItemFilled = lparam == 1;
			break;
		case 0x4f1:
			data->IsAlternateColors = lparam == 1;
			break;
		case 0x4de:
			data->ComboStyle = lparam;
			break;
		case OD_SETCOLOR:
			if (wparam < 50) data->ItemColors[wparam] = lparam;
			else if (wparam == 50) memcpy(data->Reserved_1D8, &lparam, sizeof(lparam));
			break;
		case 0x4e8: {
			HWND drop = (HWND)data->IsEditFocus;
			LRESULT drop_result = CB_ERR;
			if (drop) {
				RECT combo, child;
				GetWindowRect(window, &combo);
				GetWindowRect(drop, &child);
				drop_result = SendMessageA(drop, message, 0, MAKELPARAM(LOWORD(lparam) + combo.left - child.left, HIWORD(lparam) + combo.top - child.top));
			}
			return drop_result;
		}
		case OD_SUBCLASSED: {
			int initialized;
			memcpy(&initialized, data->Reserved_18 + 4, sizeof(initialized));
			if (!initialized || SendMessageA(window, CB_GETITEMHEIGHT, 0, 0) != data->Font->LineHeight + 6) {
				SendMessageA(window, CB_SETITEMHEIGHT, -1, data->Font->LineHeight + 2);
				SendMessageA(window, CB_SETITEMHEIGHT, 0, data->Font->LineHeight + 6);
				initialized = 1;
				memcpy(data->Reserved_18 + 4, &initialized, sizeof(initialized));
			}
			data->HadTabStop = -1;
			memset(data->ItemColors, -1, sizeof(data->ItemColors));
			return 0;
		}
	}
	previous = NULL;
	Window_Proc_Table.getValue(window, previous);
	return CallWindowProcA(previous, window, message, wparam, lparam);
}

}
#endif

#ifdef RA2_NON_MATCHING // not exact: 13802 bytes differ against the image-derived bound 005f6690 (14,618 body bytes, 6 padding) (image-faithful widths; position score 13802, was 13791: the subclassed item height read and added as a dword before the low-word mask, as the image does at 0x005F861F; before that 13841; the three selection queries read the state through ArrayList::get as RA2 1.001 does, one indexed load each as in the image; before that 13847; the selection-change notifications sent with MAKEWPARAM and the SDK LBN_ codes; before that 14527; the message groups in the order of ra2-decomp's banked form banks/0x00618D40_lbh.cpp (11df85d), each RA2 handler and fall-through chain kept)
namespace OwnerDraw {

LRESULT __stdcall List_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	WNDPROC previous = NULL;
	Window_Proc_Table.getValue(window, previous);
	HWND parent = GetParent(window);
	int needs_scrollbar = -1;
	int max_position = 0;
	bool call_default = true;
	LRESULT result = 0;
	wchar_t string[512];

	WinData * data = NULL;
	Control_Table.getPointer((unsigned int)window, &data);
	if (!data) return 0;

	int scrollbar_width = 2 * Border_Width + 18;

	RECT client_rect;
	GetClientRect(window, &client_rect);
	RECT display_rect;
	Get_Display_Rect(window, &display_rect);
	display_rect.right -= Border_Width;
	display_rect.left += Border_Width;

	Rect content_rect;
	content_rect.X = display_rect.left;
	client_rect.right -= 2 * Border_Width;
	content_rect.Width = client_rect.right - client_rect.left;
	display_rect.top += Border_Width;
	content_rect.Y = display_rect.top;
	client_rect.bottom -= 2 * Border_Width;
	display_rect.bottom -= Border_Width;
	content_rect.Height = client_rect.bottom - client_rect.top;

	/*
	 * Keep the attached scrollbar synchronized with the listbox state. This is skipped for
	 * the few messages that are queried while computing the scroll state (to avoid recursion).
	 */
	if (message != LB_GETCOUNT && message != LB_GETITEMHEIGHT && message != WM_VSCROLL) {
		int count = SendMessage(window, LB_GETCOUNT, 0, 0);
		int item_height = SendMessage(window, LB_GETITEMHEIGHT, 0, 0);
		if (item_height <= 1) {
			item_height = 1;
		}
		needs_scrollbar = (count * item_height > client_rect.bottom - client_rect.top);
		max_position = count - (client_rect.bottom - client_rect.top) / item_height;
		if ((unsigned int)data->ScrollBar > 1) {
			SCROLLINFO info;
			info.fMask = SIF_RANGE | SIF_POS;
			info.nMin = 0;
			info.nMax = max_position;
			info.nPos = data->Position;
			info.cbSize = sizeof(SCROLLINFO);
			SendMessage(data->ScrollBar, SBM_SETSCROLLINFO, 0, (LPARAM)&info);
		}
	}

	switch (message) {
		case CB_GETITEMDATA:
		case CB_SETITEMDATA:
		case LB_GETITEMDATA:
		case LB_SETITEMDATA: {
			TextNode * node = (TextNode *)CallWindowProcA(previous, window, LB_GETITEMDATA, wparam, 0);
			call_default = false;
			result = (LRESULT)node;
			if (!node || node == (TextNode *)LB_ERR) result = LB_ERR;
			else if (message == CB_GETITEMDATA || message == LB_GETITEMDATA) result = node->Reserved_04;
			else node->Reserved_04 = lparam;
			break;
		}
		case LB_SETTOPINDEX: {
			int index = (int)wparam;
			int count = SendMessage(window, LB_GETCOUNT, 0, 0);
			int item_height = SendMessage(window, LB_GETITEMHEIGHT, 0, 0);
			if (!count || !item_height) {
				break;
			}
			int visible = (client_rect.bottom - client_rect.top) / item_height;
			if (index < 0) {
				index = 0;
			}
			if (count - visible <= 0) {
				index = 0;
			} else if (index > count - visible) {
				index = count - visible;
			}
			if (index != data->Position) {
				data->Position = index;
				InvalidateRect(window, NULL, FALSE);
			}
			return(0);
		}
		case LB_SETSEL: {
			int index = (int)lparam;
			if (index < -1) {
				return(-1);
			}
			ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
			if (sel_states == NULL) {
				sel_states = new ArrayList<int>;
				data->RangeMax = (int)sel_states;
			}
			if (index >= SendMessage(window, LB_GETCOUNT, 0, 0) - 1) {
				index = SendMessage(window, LB_GETCOUNT, 0, 0) - 1;
			}
			if (index >= sel_states->length()) {
				int unselected = 0;
				sel_states->setSize(index + 1, unselected);
			}
			if (index == -1) {
				for (int i = 0; i < sel_states->length(); i++) {
					int selected = (int)wparam;
					sel_states->replace(selected, i);
				}
			} else {
				int selected = (int)wparam;
				sel_states->replace(selected, index);
			}
			parent = GetParent(window);
			SendMessage(parent, WM_COMMAND, MAKEWPARAM(GetWindowLong(window, GWL_ID), LBN_SELCHANGE), (LPARAM)window);
			InvalidateRect(window, NULL, FALSE);
			return(0);
		}
		case LB_GETTOPINDEX:
			return(data->Position);
		case LB_GETSELCOUNT: {
			ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
			if (sel_states == NULL) {
				return(0);
			}
			int selected = 0;
			int count = 0;
			for (int row = 0; row < sel_states->length(); row++) {
				sel_states->get(selected, row);
				if (selected) {
					count++;
				}
			}
			return(count);
		}
		case LB_GETSELITEMS: {
			ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
			int total = 0;
			if (sel_states != NULL) {
				int selected = 0;
				if (sel_states->length() > 0) {
					int * out = (int *)lparam;
					int max = (int)wparam;
					for (int i = 0; i < sel_states->length(); i++) {
						sel_states->get(selected, i);
						if (selected) {
							*out = i;
							total++;
							out++;
						}
						if (total >= max) {
							break;
						}
					}
				}
			}
			return(total);
		}
		case LB_GETSEL: {
			int selected = 0;
			ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
			if (sel_states == NULL) {
				return(selected);
			}
			if ((int)wparam >= sel_states->length()) {
				return(0);
			}
			sel_states->get(selected, wparam);
			return(selected);
		}
		case LB_SETCURSEL: {
			int index = (int)wparam;
			if ((int)wparam >= -1 && index < SendMessage(window, LB_GETCOUNT, 0, 0)) {
				if (data->IsEditFocus != -1) {
					SendMessage(window, LB_SETSEL, 0, data->IsEditFocus);
				}
				data->IsEditFocus = index;
				if (index != -1) {
					SendMessage(window, LB_SETSEL, TRUE, index);
				}
			}
			parent = GetParent(window);
			SendMessage(parent, WM_COMMAND, MAKEWPARAM(GetWindowLong(window, GWL_ID), LBN_SELCHANGE), (LPARAM)window);
			InvalidateRect(window, NULL, FALSE);
			return(0);
		}
		case LB_GETCURSEL:
			return(data->IsEditFocus);
		case LB_GETITEMRECT: {
			int index = (int)wparam;
			if (index < data->Position) {
				return(-1);
			}
			if (index >= SendMessage(window, LB_GETCOUNT, 0, 0)) {
				return(-1);
			}
			int item_height = SendMessage(window, LB_GETITEMHEIGHT, 0, 0);
			int relative = index - data->Position;
			if (relative > (client_rect.bottom - client_rect.top) / item_height) {
				return(-1);
			}
			RECT * out = (RECT *)lparam;
			out->top = relative * item_height;
			out->bottom = relative * item_height + item_height;
			out->left = client_rect.left;
			out->right = client_rect.right - client_rect.left;
			return(0);
		}
		case LB_SELITEMRANGE: {
			int last = HIWORD(lparam);
			int first = LOWORD(lparam);
			int count = SendMessage(window, LB_GETCOUNT, 0, 0);
			if (last < 0 || last < first) {
				return(-1);
			}
			if (last >= count) {
				last = count - 1;
			}
			ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
			if (sel_states == NULL) {
				sel_states = new ArrayList<int>;
				data->RangeMax = (int)sel_states;
			}
			if (last >= sel_states->length()) {
				int unselected = 0;
				sel_states->setSize(last + 1, unselected);
			}
			for (int i = first; i <= last; i++) {
				int selected = (int)wparam;
				sel_states->replace(selected, i);
			}
			parent = GetParent(window);
			SendMessage(parent, WM_COMMAND, MAKEWPARAM(GetWindowLong(window, GWL_ID), LBN_SELCHANGE), (LPARAM)window);
			return(0);
		}
		case WM_LBUTTONDBLCLK:
			PostMessage(parent, WM_COMMAND, MAKEWPARAM(GetWindowLong(window, GWL_ID), LBN_DBLCLK), (LPARAM)window);
			return(0);
		case WM_LBUTTONDOWN: {
			int top = SendMessage(window, LB_GETTOPINDEX, 0, 0);
			int item_height = SendMessage(window, LB_GETITEMHEIGHT, 0, 0);
			int index = top + (int)HIWORD(lparam) / item_height;
			LONG style = GetWindowLong(window, GWL_STYLE);
			SetFocus(window);
			int paint_disabled = SendMessage(window, OD_DISABLEPAINT, 0, 1);
			if ((style & LBS_MULTIPLESEL) != 0) {
				int select = (SendMessage(window, LB_GETSEL, index, 0) == 0);
				VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, NULL);
				SendMessage(window, LB_SETSEL, select, index);
				InvalidateRect(window, NULL, FALSE);
			} else if ((style & LBS_NOSEL) == 0) {
				VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, NULL);
				SendMessage(window, LB_SETCURSEL, index, 0);
				InvalidateRect(window, NULL, FALSE);
			}
			SendMessage(window, OD_DISABLEPAINT, 0, paint_disabled);
			parent = GetParent(window);
			SendMessage(parent, WM_COMMAND, MAKEWPARAM(GetWindowLong(window, GWL_ID), LBN_SELCHANGE), (LPARAM)window);
			return(0);
		}
		case WM_RBUTTONDOWN:
			SendMessage(window, LB_SETSEL, 0, -1);
			SendMessage(window, LB_SETCURSEL, (WPARAM)-1, 0);
			parent = GetParent(window);
			SendMessage(parent, WM_COMMAND, MAKEWPARAM(GetWindowLong(window, GWL_ID), LBN_SELCHANGE), (LPARAM)window);
			InvalidateRect(window, NULL, FALSE);
			return(0);
		case OD_SETCOLOR: {
			ArrayList<int> * row_colors = (ArrayList<int> *)data->ItemState;
			if (row_colors == NULL) {
				row_colors = new ArrayList<int>;
				data->ItemState = (int)row_colors;
			}
			int index = (int)wparam;
			if (index >= row_colors->length()) {
				int blank = -1;
				row_colors->setSize(index + 1, blank);
			}
			int color = (int)lparam;
			row_colors->replace(color, index);
			InvalidateRect(window, NULL, FALSE);
			break;
		}
		case OD_SUBCLASSED: {
			data->IsEditFocus = -1;
			int height = data->Font->LineHeight + 2;
			SendMessage(window, LB_SETITEMHEIGHT, (WPARAM)-1, height & 0xffff);
			break;
		}
		case OD_ADDCOLUMN: {
			ArrayList<ColumnData> * columns = (ArrayList<ColumnData> *)data->HadTabStop;
			if (columns == NULL) {
				columns = new ArrayList<ColumnData>;
				data->HadTabStop = (int)columns;
			}
			ColumnData * column = NULL;
			int col = 0;
			while (col < columns->length()) {
				columns->getPointer(&column, col);
				if (column->xPos == lparam) {
					return(lparam);
				}
				col++;
			}
			ColumnData new_column;
			new_column.xPos = lparam;
			new_column.width = wparam;
			int length = columns->length();
			columns->add(new_column, length);
			return(lparam);
		}
		case OD_GETCELLTIP: {
			int count = SendMessage(window, LB_GETCOUNT, 0, 0);
			int item_height = SendMessage(window, LB_GETITEMHEIGHT, 0, 0);
			ArrayList<ColumnData> * columns = (ArrayList<ColumnData> *)data->HadTabStop;
			if (columns != NULL) {
				int row = data->Position + (int)HIWORD(wparam) / item_height;
				int best_column = -1;
				int best_x = 0;
				ColumnData * column = NULL;
				for (int col = 0; col < columns->length(); col++) {
					columns->getPointer(&column, col);
					if (column->xPos <= (int)LOWORD(wparam)) {
						if (column->xPos > best_x) {
							best_column = col;
							best_x = column->xPos;
						}
					}
				}
				if (best_column != -1 && row >= 0 && row < count) {
					column = NULL;
					columns->getPointer(&column, best_column);
					if (column != NULL && row < column->cells.length()) {
						CellData * cell = NULL;
						column->cells.getPointer(&cell, row);
						if (cell != NULL && lparam != 0) {
							wcscpy((wchar_t *)lparam, cell->hint.get());
							return(wcslen(cell->hint.get()) == 0);
						}
					}
				}
			}
			return(0);
		}
		case OD_REMOVECOLUMN: {
			ArrayList<ColumnData> * columns = (ArrayList<ColumnData> *)data->HadTabStop;
			if (columns != NULL) {
				ColumnData * column = NULL;
				for (int col = 0; col < columns->length(); col++) {
					columns->getPointer(&column, col);
					if (column->xPos == lparam) {
						columns->remove(col);
						return(lparam);
					}
				}
			}
			return(-1);
		}
		case OD_SETCELL: {
			ArrayList<ColumnData> * columns = (ArrayList<ColumnData> *)data->HadTabStop;
			if (columns == NULL) {
				return(-1);
			}
			int column_id = LOWORD(wparam);
			int row = HIWORD(wparam);
			ColumnData * column = NULL;
			int found = -1;
			for (int col = 0; col < columns->length(); col++) {
				columns->getPointer(&column, col);
				if (column->xPos == column_id) {
					found = col;
					break;
				}
			}
			if (found == -1) {
				return(-1);
			}
			if (row < 0 || row >= SendMessage(window, LB_GETCOUNT, 0, 0)) {
				return(-1);
			}
			CellData filler;
			if (found == 0) {
				filler.type = CellData::PRIMARY;
			}
			if (row >= column->cells.length()) {
				column->cells.setSize(row + 1, filler);
			}
			column->cells.replace(*(CellData *)lparam, row);
			return(column_id);
		}
		case 0x4c4:
		case 0x4c5:
		case 0x4c6:
		case 0x4c9:
		case 0x4ca:
		case 0x4cb: {
			wchar_t buffer[0x1400];
			wchar_t const * find;
			if (message <= 0x4c6) {
				Char_To_Wide(buffer, (char const *)lparam, sizeof(buffer) / sizeof(buffer[0]));
				find = buffer;
			} else {
				find = (wchar_t const *)lparam;
			}
			int count = SendMessageA(window, LB_GETCOUNT, 0, 0);
			call_default = false;
			if (count == LB_ERR) break;
			result = LB_ERR;
			for (int index = wparam; index < count; ++index) {
				TextNode * node = (TextNode *)CallWindowProcA(previous, window, LB_GETITEMDATA, index, 0);
				if (!node || node == (TextNode *)LB_ERR) continue;
				int match = (message == 0x4c5 || message == 0x4ca) ? wcsicmp(find, node->Data) : wcsnicmp(find, node->Data, wcslen(find));
				if (match == 0) {
					result = index;
					if (message == 0x4c6) result = SendMessage(window, LB_SETCURSEL, index, 0);
					break;
				}
			}
			break;
		}
		case 0x4c7:
		case 0x4c8:
		case 0x4cc:
		case OD_ADDSTRINGW: {
			bool wide = message >= 0x4cc;
			bool append = message == 0x4c8 || message == 0x4cd;
			char const * caption = wide ? Wide_To_Narrow((wchar_t const *)lparam) : (char const *)lparam;
			LRESULT index = CallWindowProcA(previous, window, append ? LB_ADDSTRING : LB_INSERTSTRING, append ? 0 : wparam, (LPARAM)caption);
			call_default = false;
			if (index == LB_ERR || index == LB_ERRSPACE) { result = index; break; }
			int length = wide ? wcslen((wchar_t const *)lparam) : strlen(caption);
			TextNode * node = (TextNode *)operator new(offsetof(TextNode, Text) + (length + 1) * sizeof(wchar_t));
			result = LB_ERRSPACE;
			if (node) {
				node->Data = node->Text;
				node->Next = data->Nodes;
				node->Reserved_04 = 0;
				data->Nodes = node;
				node->Reserved_0C = wide;
				if (wide) wcscpy(node->Data, (wchar_t const *)lparam);
				else Char_To_Wide(node->Data, caption, -1);
				result = CallWindowProcA(previous, window, LB_SETITEMDATA, index, (LPARAM)node);
				if (result != LB_ERR && result != LB_ERRSPACE) {
					result = index;
					int position = index;
					if (message != 0x4c8 && position != -1) {
						ArrayList<int> * row_colors = (ArrayList<int> *)data->ItemState;
						if (row_colors && position < row_colors->length()) {
							int color = -1;
							row_colors->add(color, position);
						}
						ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
						if (sel_states && position < sel_states->length()) {
							int selected = 0;
							sel_states->add(selected, position);
						}
					}
					if (message == 0x4c8 || position < 0) {
						position = SendMessage(window, LB_GETCOUNT, 0, 0);
					}

					ArrayList<ColumnData> * columns = (ArrayList<ColumnData> *)data->HadTabStop;
					CellData cell;
					if (columns != NULL) {
						for (int col = 0; col < columns->length(); col++) {

							/*
							 * The first column added implicitly holds the row's listbox string,
							 * so the new row gets a PRIMARY cell there and INVALID cells elsewhere.
							 */
							cell.type = (col == 0) ? CellData::PRIMARY : CellData::INVALID;
							ColumnData * column = NULL;
							columns->getPointer(&column, col);
							column->cells.add(cell, position);
						}
					}
					break;
				}
			}
			CallWindowProcA(previous, window, LB_DELETESTRING, index, 0);
			TextNode ** link = &data->Nodes;
			while (*link && *link != node) link = &(*link)->Next;
			if (*link) {
				*link = node->Next;
				operator delete(node);
			}
			break;
		}
		case LB_GETTEXTLEN:
		case 0x4b3:
		case 0x4b5:
		case 0x4b6:
		case 0x4b7:
		case 0x4cf: {
			bool wide = message != 0x4b5 && message != 0x4b7;
			bool copy = message != LB_GETTEXTLEN && message != 0x4cf && lparam;
			if (copy) {
				if (wide) *(wchar_t *)lparam = 0;
				else *(char *)lparam = 0;
			}
			call_default = false;
			TextNode * node = (TextNode *)CallWindowProcA(previous, window, LB_GETITEMDATA, wparam, 0);
			if (node && node != (TextNode *)LB_ERR) {
				result = message == 0x4cf ? node->Reserved_0C : wcslen(node->Data);
				if (copy && wide) wcscpy((wchar_t *)lparam, node->Data);
				if (copy && !wide) WideToCharString((char *)lparam, node->Data, -1);
			}
			break;
		}
		case 0x4e8: {
			if (LOWORD(lparam) >= client_rect.right || HIWORD(lparam) >= client_rect.bottom) return -1;
			int height = SendMessage(window, LB_GETITEMHEIGHT, 0, 0);
			if (height <= 0) return -1;
			int row = data->Position + HIWORD(lparam) / height;
			return row >= 0 && row < SendMessage(window, LB_GETCOUNT, 0, 0) ? row : -1;
		}
		case 0x4ef:
			return (LRESULT)data->ScrollBar;
		case WM_VSCROLL: {
			call_default = false;
			LRESULT position = SendMessage(data->ScrollBar, SBM_GETPOS, 0, 0);
			if (position != SendMessage(window, LB_GETTOPINDEX, 0, 0)) {
				SendMessage(window, LB_SETTOPINDEX, position, 0);
			}
			break;
		}
		case LB_DELETESTRING: {
			ArrayList<int> * row_colors = (ArrayList<int> *)data->ItemState;
			if (row_colors != NULL && row_colors->length() > (int)wparam) {
				row_colors->remove((int)wparam);
			}
			ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
			if (sel_states != NULL && sel_states->length() > (int)wparam) {
				sel_states->remove((int)wparam);
			}
			ArrayList<ColumnData> * columns = (ArrayList<ColumnData> *)data->HadTabStop;
			if (columns != NULL) {
				for (int col = 0; col < columns->length(); col++) {
					ColumnData * column = NULL;
					columns->getPointer(&column, col);
					if (column->cells.length() != 0) {
						column->cells.remove((int)wparam);
					}
				}
			}
			break;
		}
		case WM_NCDESTROY:
		case LB_RESETCONTENT: {
			ArrayList<int> * row_colors = (ArrayList<int> *)data->ItemState;
			if (row_colors != NULL) {
				delete row_colors;
			}
			ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
			if (sel_states != NULL) {
				delete sel_states;
			}
			data->ItemState = 0;
			data->RangeMax = 0;
			data->Position = 0;
			data->IsEditFocus = -1;

			ArrayList<ColumnData> * columns = (ArrayList<ColumnData> *)data->HadTabStop;
			if (columns != NULL) {
				for (int col = 0; col < columns->length(); col++) {
					ColumnData * column = NULL;
					columns->getPointer(&column, col);
					column->cells.clear();
				}
			}

			if (message == WM_NCDESTROY) {
				if (columns != NULL) {
					delete columns;
				}
				data->HadTabStop = 0;
			} else {
				parent = GetParent(window);
				SendMessage(parent, WM_COMMAND, MAKEWPARAM(GetWindowLong(window, GWL_ID), LBN_SELCHANGE), (LPARAM)window);
			}
			break;
		}
		case WM_DELETEITEM: {
			result = 1;
			DELETEITEMSTRUCT * item = (DELETEITEMSTRUCT *)lparam;
			TextNode ** link = &data->Nodes;
			while (*link) {
				TextNode * node = *link;
				if (node == (TextNode *)item->itemData) {
					*link = node->Next;
					operator delete(node);
					break;
				}
				link = &node->Next;
			}
			break;
		}
		case WM_SIZE: {
			if ((int)data->ScrollBar > 1) {
				RECT parent_display;
				Get_Display_Rect(GetParent(window), &parent_display);
				RECT win_display;
				Get_Display_Rect(window, &win_display);
				MoveWindow(data->ScrollBar, win_display.right - parent_display.left, win_display.top - parent_display.top, scrollbar_width, win_display.bottom - win_display.top, TRUE);
			}
			Surface * surface = data->Back;
			if (surface != NULL) {
				if ((unsigned short)lparam != surface->Get_Width() || HIWORD(lparam) != surface->Get_Height()) {
					WinData * cache_data = NULL;
					Control_Table.getPointer((unsigned int)window, &cache_data);
					if (cache_data != NULL && cache_data->Back != NULL) {
						delete cache_data->Back;
						cache_data->Back = NULL;
						Surface_Count--;
					}
				}
			}
			break;
		}
		case WM_SETFONT: {
			HDC dc = GetDC(window);
			TEXTMETRIC tm;
			GetTextMetrics(dc, &tm);
			ReleaseDC(window, dc);
			SendMessage(window, LB_SETITEMHEIGHT, (WPARAM)-1, (unsigned short)(LOWORD(tm.tmHeight) + 2));
			data->Font = (BitFont *)wparam;
			return(0);
		}
		case WM_ERASEBKGND:
			return(0);
		case WM_PAINT: {
			BitFont * font = data->Font;
			call_default = false;
			Draw_Dimmed_Back(content_rect, window, data->TextStyle);
			Draw_Frame(AlternateSurface, content_rect, 1, -1);

			int fill_color = Color_To_Pixel(Fill_Color);

			RECT update_rect;
			if (!GetUpdateRect(window, &update_rect, FALSE)) {
				break;
			}

			int count = SendMessage(window, LB_GETCOUNT, 0, 0);
			int index = SendMessage(window, LB_GETTOPINDEX, 0, 0);
			while (index < count) {
				RECT item_rect;
				if (SendMessage(window, LB_GETITEMRECT, index, (LPARAM)&item_rect) != -1) {
					if (client_rect.top + item_rect.bottom > client_rect.bottom) {
						break;
					}
					{
						WNDPROC proc = NULL;
						Window_Proc_Table.getValue(window, proc);
						string[0] = 0;
						TextNode * item = (TextNode *)CallWindowProcA(proc, window, LB_GETITEMDATA, index, 0);
						if (!item || item == (TextNode *)LB_ERR) item = (TextNode *)CallWindowProcA(proc, window, CB_GETITEMDATA, index, 0);
						if (item && item != (TextNode *)LB_ERR) {
							wcsncpy(string, item->Data, 512);
							string[511] = 0;
						}
					}

					ArrayList<ColumnData> * columns = (ArrayList<ColumnData> *)data->HadTabStop;
					if (columns != NULL) {
						if (SendMessage(window, LB_GETSEL, index, 0) > 0) {
							Rect fill;
							fill.X = item_rect.left + display_rect.left;
							fill.Y = item_rect.top + display_rect.top;
							fill.Width = item_rect.right - item_rect.left;
							fill.Height = item_rect.bottom - item_rect.top;
							AlternateSurface->Fill_Rect(fill, fill_color);
						}

						HDC dc = GetDC(window);
						for (int col = 0; col < columns->length(); col++) {
							ColumnData * column = NULL;
							columns->getPointer(&column, col);
							CellData * cell = NULL;
							column->cells.getPointer(&cell, index);
							if (cell == NULL || cell->type == CellData::INVALID) {
								continue;
							}

							if (cell->type == CellData::TEXT || cell->type == CellData::PRIMARY) {
								Rect text_rect;
								text_rect.X = display_rect.left + item_rect.left + column->xPos;
								text_rect.Y = display_rect.top + item_rect.top;
								text_rect.Width = 0;
								text_rect.Height = 0;
								if (cell->type == CellData::TEXT) {
									wcsncpy(string, cell->string.get(), 512);
									string[511] = 0;
								} else if (cell->type == CellData::PRIMARY) {
									{
										WNDPROC proc = NULL;
										Window_Proc_Table.getValue(window, proc);
										string[0] = 0;
										TextNode * item = (TextNode *)CallWindowProcA(proc, window, LB_GETITEMDATA, index, 0);
										if (!item || item == (TextNode *)LB_ERR) item = (TextNode *)CallWindowProcA(proc, window, CB_GETITEMDATA, index, 0);
										if (item && item != (TextNode *)LB_ERR) {
											wcsncpy(string, item->Data, 512);
											string[511] = 0;
										}
									}
								}
								COLORREF text_color = cell->color;
								if (text_color == -1) {
									text_color = Text_Color;
								}
								int max_width = column->width;
								if (max_width == 0) {
									max_width = 0xFFFF;
								}
								if (display_rect.right - max_width - column->xPos - item_rect.left - display_rect.left < 0) {
									max_width = display_rect.right - column->xPos - item_rect.left - display_rect.left;
								}
								SendMessage(window, OD_RESTOREDC, 0, (LPARAM)dc);
								int width, height;
								int length = wcslen(string);
								font->Text_Dimensions(string, &width, &height, 0);
								while (length && width > max_width) {
									string[--length] = 0;
									if (length <= 508) wcscat(string, L"...");
									font->Text_Dimensions(string, &width, &height, 0);
								}
								OD_Draw_Text(text_color, font, text_rect, string, wcslen(string), 0, 0, NULL, 0);
							} else if (cell->type == CellData::SURFACE) {
								Surface * surface = cell->surf;
								if (surface != NULL) {
									int height = surface->Get_Height();
									int width = surface->Get_Width();
									Rect surface_rect;
									surface_rect.Width = width;
									surface_rect.X = display_rect.left + item_rect.left + column->xPos;
									surface_rect.Height = height;
									surface_rect.Y = display_rect.top + item_rect.top + (item_rect.bottom - item_rect.top - height) / 2;
									SurfaceCache.DrawTrans(surface_rect, *AlternateSurface, *surface, (255 >> DSurface::RedLeft << DSurface::RedRight) | (255u >> DSurface::BlueLeft << DSurface::BlueRight));
								}
							} else {
								int ping = cell->pingtime;
								Rect ping_rect;
								ping_rect.X = display_rect.left + item_rect.left + column->xPos;
								ping_rect.Y = display_rect.top + item_rect.top;
								ping_rect.Width = 28;
								ping_rect.Height = 12;
								unsigned color;
								if (ping < 300) {
									color = DSurface::Build_Hicolor_Pixel(0, 192, 0);
								} else if (ping < 500) {
									color = DSurface::Build_Hicolor_Pixel(192, 192, 0);
								} else {
									color = DSurface::Build_Hicolor_Pixel(192, 0, 0);
								}
								Draw_Progress_Fill(ping_rect, AlternateSurface, color, (ping << 16) / 1000);
							}
						}
						ReleaseDC(window, dc);
					} else {
						ArrayList<int> * row_colors = (ArrayList<int> *)data->ItemState;
						COLORREF text_color;
						int * color_ptr = NULL;
						if (row_colors == NULL || !row_colors->getPointer(&color_ptr, index) || *color_ptr == -1) {
							text_color = Text_Color;
						} else {
							text_color = *color_ptr;
						}

						if (SendMessage(window, LB_GETSEL, index, 0) > 0) {
							Rect fill;
							fill.X = item_rect.left + display_rect.left;
							fill.Y = item_rect.top + display_rect.top;
							fill.Height = item_rect.bottom - item_rect.top;
							fill.Width = item_rect.right - item_rect.left;
							AlternateSurface->Fill_Rect(fill, fill_color);
						}

						Rect text_rect;
						text_rect.X = item_rect.left + display_rect.left + 2;
						text_rect.Y = display_rect.top + item_rect.top;
						text_rect.Width = item_rect.right - item_rect.left;
						text_rect.Height = item_rect.bottom - item_rect.top;
						OD_Draw_Text(text_color, font, text_rect, string, wcslen(string), 0, 0, NULL, 0);
					}
				}
				index++;
			}
			ValidateRect(window, &update_rect);
			break;
		}
	}

	/*
	 * Create or destroy the attached scrollbar based on whether the listbox needs one,
	 * then forward the message to the original Win32 listbox procedure.
	 */
	if (needs_scrollbar == 1) {
		if (data->ScrollBar == NULL) {
			data->ScrollBar = (HWND)1;
			GetWindowLong(window, GWL_ID);
			parent = GetParent(window);
			RECT parent_display;
			Get_Display_Rect(parent, &parent_display);
			RECT win_display;
			Get_Display_Rect(window, &win_display);
			int x = win_display.left - parent_display.left;
			int y = win_display.top - parent_display.top;
			data->ScrollBar = CreateWindowEx(0, "Scrollbar", NULL, 0x50010001,
				win_display.left - parent_display.left - scrollbar_width + client_rect.right + 1,
				client_rect.top + win_display.top - parent_display.top,
				scrollbar_width, win_display.bottom - win_display.top,
				parent, NULL, ProgramInstance, NULL);
			data->ScrollWidth = scrollbar_width;
			Subclass_Control(data->ScrollBar, 0);

			WinData * sb_data = NULL;
			Control_Table.getPointer((unsigned int)data->ScrollBar, &sb_data);
			sb_data->ScrollOwner = window;

			SCROLLINFO info;
			info.nMax = max_position;
			info.fMask = SIF_RANGE | SIF_POS;
			info.nMin = 0;
			info.nPos = data->Position;
			info.cbSize = sizeof(SCROLLINFO);
			SendMessage(data->ScrollBar, SBM_SETSCROLLINFO, 0, (LPARAM)&info);

			SetWindowPos(window, NULL, 0, 0, win_display.right - win_display.left - scrollbar_width, win_display.bottom - win_display.top, SWP_NOMOVE);
			ShowWindow(data->ScrollBar, SW_SHOW);
			BringWindowToTop(data->ScrollBar);
			InvalidateRect(data->ScrollBar, NULL, FALSE);
			UpdateWindow(data->ScrollBar);

			RECT validate_rect;
			validate_rect.left = x;
			validate_rect.top = y;
			validate_rect.right = x + client_rect.right + 1;
			validate_rect.bottom = client_rect.bottom + y + 1;
			ValidateRect(parent, &validate_rect);
		}
	} else if (needs_scrollbar == 0 && data->ScrollBar != NULL && !data->SkipCheckPaint) {
		DestroyWindow(data->ScrollBar);
		Restore_Child(data->ScrollBar, 0);
		data->ScrollBar = NULL;
		data->ScrollWidth = 0;
		SetWindowPos(window, NULL, 0, 0, client_rect.right + Border_Width - client_rect.left + scrollbar_width + 1, client_rect.bottom + 2 * Border_Width - client_rect.top, SWP_NOMOVE);
		parent = GetParent(window);
		RECT parent_display;
		Get_Display_Rect(parent, &parent_display);
		RECT validate_rect;
		validate_rect.left = display_rect.left - parent_display.left - 1;
		validate_rect.top = display_rect.top - parent_display.top - 1;
		validate_rect.right = scrollbar_width + client_rect.right + display_rect.left - parent_display.left;
		validate_rect.bottom = display_rect.top - parent_display.top + client_rect.bottom + 1;
		ValidateRect(parent, &validate_rect);
	}

	if (call_default) {
		result = CallWindowProc(previous, window, message, wparam, lparam);
	}

	if (message == WM_NCDESTROY) {
		ArrayList<int> * row_colors = (ArrayList<int> *)data->ItemState;
		if (row_colors != NULL) {
			delete row_colors;
		}
		ArrayList<int> * sel_states = (ArrayList<int> *)data->RangeMax;
		if (sel_states != NULL) {
			delete sel_states;
		}
	}

	return(result);
}

}
#endif
