/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

// From mischa85/RedAlert2 dbca015 src/shapebtn.cpp (matching RA2 1.006 decomp; TibSun shapebtn.cpp); modified:
// the function bodies are this tree's accepted ones (the shape is the raw shape file, not ShapeSet; the
// drawing call is CC_Draw_Shape with its full argument list) (modules.json source_gaps).

/* $Header: /CounterStrike/SHAPEBTN.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SHAPEBTN.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : September 20, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ShapeButtonClass::Draw_Me -- Renders the shape button's imagery.                          *
 *   ShapeButtonClass::Set_Shape -- Assigns a shape to this shape button.                      *
 *   ShapeButtonClass::ShapeButtonClass -- Constructor for a shape type button.                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"
#include "shared/palette.hpp"
#include "shared/unknown_globals.hpp"

/*
**	The unit's first function (0x00675000) is the dynamic initializer of the empty
**	rectangle shared/shape_file.hpp defines for every unit that includes it.
*/


/***********************************************************************************************
 * ShapeButtonClass::ShapeButtonClass -- Default Constructor for a shape type button.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   You must call Set_Shape() before using a button constructed with this function, *
 *             and you must set X & Y, and ID.                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ShapeButtonClass::ShapeButtonClass() :
    ToggleClass(0, 0, 0, 0, 0),
    IsFlashOn(false),
    FlashDelay(0),
    FlashCounter(0),
    ReflectButtonState(false),
    DrawOffsetX(0),
    DrawOffsetY(0),
    DrawOnSidebar(false),
    ShapeDrawer(unknown_drawer_8323c4),
    DrawFaded(false),
    IsShapeLoaded(false)
{
}


/***********************************************************************************************
 * ShapeButtonClass::ShapeButtonClass -- Constructor for a shape type button.                  *
 *                                                                                             *
 *    This is the normal constructor for a shape type button. Shape buttons are ones that      *
 *    have their imagery controlled by a shape file. The various states of the button are      *
 *    given a visual form as one of these shapes. Button dimensions are controlled by the      *
 *    first shape.                                                                             *
 *                                                                                             *
 * INPUT:   id    -- The button ID.                                                            *
 *                                                                                             *
 *          shape -- Pointer to the shape file that controls the button's display.             *
 *                                                                                             *
 *          x,y   -- The pixel coordinate of the upper left corner of the button.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The width and height of the shape is controlled by the first shape in the       *
 *             shape file provided. This means that all the shapes in the shape file must be   *
 *             the same size.                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ShapeButtonClass::ShapeButtonClass(unsigned id, void const *shape, int x, int y, int w, int h, bool flag) :
    ToggleClass(id, x, y, 0, 0),
    IsFlashOn(false),
    FlashDelay(0),
    ReflectButtonState(false),
    DrawOffsetX(0),
    DrawOffsetY(0),
    DrawOnSidebar(false),
    ShapeDrawer(unknown_drawer_8323c4),
    DrawFaded(flag),
    IsShapeLoaded(false)
{
    ShapeData = shape;
    if (shape != 0) {
        Width = ((short const *)shape)[1];
        Height = ((short const *)shape)[2];
    } else {
        Width = 0;
        Height = 0;
    }
    if (w != 0) {
        Width = w;
    }
    if (h != 0) {
        Height = h;
    }
}


ShapeButtonClass::~ShapeButtonClass()
{
    if (IsShapeLoaded && ShapeData != 0) {
        delete (char *)ShapeData;
        ShapeData = 0;
        IsShapeLoaded = false;
        Width = 0;
        Height = 0;
    }
}


/***********************************************************************************************
 * ShapeButtonClass::Set_Shape -- Assigns a shape to this shape button.                        *
 *                                                                                             *
 *    This routine will assign the specified shape to this shape object.                       *
 *                                                                                             *
 * INPUT:   data  -- Pointer to the shape to assign.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ShapeButtonClass::Set_Shape(void const *shape, int w, int h)
{
    if (IsShapeLoaded && ShapeData != 0) {
        delete (char *)ShapeData;
        ShapeData = 0;
        IsShapeLoaded = false;
        Width = 0;
        Height = 0;
    }
    ShapeData = shape;
    if (shape != 0) {
        Width = ((short const *)shape)[1];
        Height = ((short const *)shape)[2];
    } else {
        Width = 0;
        Height = 0;
    }
    if (w != 0) {
        Width = w;
    }
    if (h != 0) {
        Height = h;
    }
}


void ShapeButtonClass::Clear_Shape()
{
    if (IsShapeLoaded && ShapeData != 0) {
        operator delete((void *)ShapeData);
        ShapeData = 0;
        IsShapeLoaded = false;
        Width = 0;
        Height = 0;
    }
}


void ShapeButtonClass::Set_Shape_Loaded(bool allocated)
{
    IsShapeLoaded = allocated;
}


/***********************************************************************************************
 * ShapeButtonClass::Draw_Me -- Renders the shape button's imagery.                            *
 *                                                                                             *
 *    This function is called when the button detects that it must be redrawn. The actual      *
 *    shape to use is controled by the button's state and the shape file provided when then    *
 *    button was constructed.                                                                  *
 *                                                                                             *
 * INPUT:   forced   -- Should the button be redrawn regardless of the redraw flag?            *
 *                                                                                             *
 * OUTPUT:  bool; Was the shape redrawn?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ShapeButtonClass::Draw_Me(int forced)
{
    if (ControlClass::Draw_Me(forced) && ShapeData) {
        int shapenum;
        if (IsDisabled) {
            shapenum = 2;
        } else if (ReflectButtonState) {
            if (IsFlashOn) {
                shapenum = (IsOn ? 1 : 0) + 3;
            } else {
                shapenum = IsOn ? 1 : 0;
            }
        } else {
            shapenum = IsPressed ? 1 : 0;
        }
        Surface *surface = DrawOnSidebar ? unknown_surface_839948 : unknown_surface_83995c;
        CC_Draw_Shape(surface, (ConvertClass *)ShapeDrawer, (ShapeFileStruct const *)ShapeData, shapenum,
                      Point2D(X + DrawOffsetX, Y + DrawOffsetY), unknown_window_839640, DrawFaded ? 0x800 : 0,
                      0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        IsDrawn = true;
        return true;
    }
    return false;
}


bool ShapeButtonClass::Start_Flashing(int start, int length, bool flag)
{
    if (!FlashDelay) {
        FlashDelay = start;
        FlashCounter = start + length;
        IsFlashOn = flag;
        return true;
    }
    return false;
}


bool ShapeButtonClass::Stop_Flashing()
{
    if (FlashDelay) {
        IsFlashOn = false;
        FlashCounter = 0;
        FlashDelay = 0;
        return true;
    }
    return false;
}


bool ShapeButtonClass::AI()
{
    if (IsDisabled) {
        return Stop_Flashing();
    }
    if (FlashCounter) {
        FlashCounter--;
        if (!FlashCounter) {
            IsFlashOn = !IsFlashOn;
            FlashCounter = FlashDelay;
            return true;
        }
    }
    return false;
}


bool ShapeButtonClass::Is_Flashing() const
{
    return FlashDelay != 0;
}


bool ShapeButtonClass::Is_Flash_On() const
{
    return IsFlashOn;
}
