/*
**	Command & Conquer Red Alert 2
**	The dialog frame drawing of txtlabel2.cpp that other units call (lane R2: the declarations
**	moved here from the units that used them).
*/
#pragma once

#include "rect.h"

class Surface;

void __fastcall Draw_Interface_Screen(Surface * surface, Rect const & rect, bool arg3); // RA2: 0x006F3A20, draws the dialog frame and its backdrop
Rect __fastcall Get_Title_Screen_Area(void); // RA2: 0x006F1F30
