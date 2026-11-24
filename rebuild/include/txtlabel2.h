/*
**	Command & Conquer Red Alert 2
**	The dialog frame drawing of txtlabel2.cpp that other units call.
*/
#pragma once

#include "rect.h"

class Surface;

void __fastcall Draw_Interface_Screen(Surface * surface, Rect const & rect, bool arg3); // RA2: 0x006F3A20, draws the dialog frame and its backdrop
Rect __fastcall Get_Title_Screen_Area(void); // RA2: 0x006F1F30

class ShapeSet;
// Declarations moved from the units that used them (one declaration per name).
extern void __fastcall Load_All_Art(void); // RA2: 0x006F1B60 (txtlabel2.cpp)
extern void __fastcall Load_Side_Animation_Art(void); // RA2: 0x006F2C40, loads the animation the side buttons of the campaign dialog show
extern void __fastcall Free_Side_Animation_Art(void); // RA2: 0x006F2D00, frees it again
extern void __fastcall Reload_Sidebar_Art(void); // RA2: 0x006F44D0 (txtlabel2.cpp)
extern void __fastcall Select_Tooltip_Color(int which); // RA2: 0x006F45A0 (txtlabel2.cpp)
extern ShapeSet * Button_Strip_Shape;
extern Rect * Button_Strip_Top;
extern Rect * Default_Button_Y;
extern Rect * Free_Button_Strip;
extern Rect * Default_Button_Strip;
extern void * Art_Side_Animation;
extern void * Art_Button_Corner;
extern void * Art_Side_Strip;
extern void * Art_Score_Frame;
extern ShapeSet * ArtSlideTop;
extern ShapeSet * Slide_Button_Shape;
extern ShapeSet * ArtSlideMPButton;
extern ShapeSet * ArtSlideWarning;
extern void * ArtScoreTop;
extern ShapeSet * Art_Chat_Back;
extern int SlideButtonCount;
extern Rect * SlideButtonRect;
extern Rect * SlideMPButtonRect;
extern Rect * SlideWarningRect;
extern Rect * DialogBackRect;
extern Rect * SlideTopRect;
extern Rect * Button_Strip_Bottom;
extern void * Art_Button_Back;
extern ShapeSet * Art_Score_Screen;
extern ShapeSet * Art_Dialog_Back640;
extern ShapeSet * Art_Dialog_Back;

void __fastcall Relayout_Interface_Art(int width, int height); // RA2: 0x6F33B0
void __fastcall Free_All_Art(void); // RA2: 0x006F1D20
