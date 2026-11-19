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
extern ShapeSet * Button_Strip_Shape; // RA2: as above
extern Rect * Button_Strip_Top; // RA2: as above
extern Rect * Default_Button_Y; // RA2: as above
extern Rect * Free_Button_Strip; // RA2: as above
extern Rect * Default_Button_Strip; // RA2: as above
extern void * Art_Side_Animation; // RA2: as above
extern void * Art_Button_Corner; // RA2: as above
extern void * Art_Side_Strip; // RA2: as above
extern void * Art_Score_Frame; // RA2: as above
extern ShapeSet * ArtSlideTop; // RA2: as above
extern ShapeSet * Slide_Button_Shape; // RA2: as above
extern ShapeSet * ArtSlideMPButton; // RA2: as above
extern ShapeSet * ArtSlideWarning; // RA2: as above
extern void * ArtScoreTop; // RA2: as above
extern ShapeSet * Art_Chat_Back; // RA2: as above
extern int SlideButtonCount; // RA2: as above
extern Rect * SlideButtonRect; // RA2: as above
extern Rect * SlideMPButtonRect; // RA2: as above
extern Rect * SlideWarningRect; // RA2: as above
extern Rect * DialogBackRect; // RA2: as above
extern Rect * SlideTopRect; // RA2: as above
extern Rect * Button_Strip_Bottom; // RA2: as above
extern void * Art_Button_Back; // RA2: as above
extern ShapeSet * Art_Score_Screen; // RA2: as above
extern ShapeSet * Art_Dialog_Back640; // RA2: as above
extern ShapeSet * Art_Dialog_Back; // RA2: as above
