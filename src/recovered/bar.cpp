/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/bar.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// bar.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bar.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/progress_bar.hpp"
#include "shared/gadget.hpp"
#include "shared/mouse.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"


// Dynamic initializer (0x0042B950, _$E2) of the file-local empty Rect that
// shared/shape_file.hpp defines in BAR.CPP; the unit's first source.


/***********************************************************************************************
 * ProgressBarClass::ProgressBarClass -- Constructor for the bargraph object.                  *
 *                                                                                             *
 *    This is the constructor for the bargraph object. It establishes the dimensions and       *
 *    coordinate of the bargraph as well as the colors it will use when drawn.                 *
 *                                                                                             *
 * INPUT:   w,y      -- Pixel coordinate of the upper left corner of the bargraph.             *
 *                                                                                             *
 *          width,height   -- Dimensions of the bargraph.                                      *
 *                                                                                             *
 *          forecolor   -- The color to use for the filled portion of the bargraph.            *
 *                                                                                             *
 *          backcolor   -- The color to use for the non-filled portion of the bargraph.        *
 *                                                                                             *
 *          bordercolor -- Optional border color. If not zero, then the bargraph will be       *
 *                         outlined with this color.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042B970)

ProgressBarClass::ProgressBarClass(int x, int y, int width, int height, int forecolor, int backcolor,
                                   int bordercolor) :
    X(x),
    Y(y),
    Width(width),
    Height(height),
    BarColor(forecolor),
    BackColor(backcolor),
    BorderColor(bordercolor),
    CurrentValue(0),
    LastDisplayCurrent(0),
    IsDrawn(false)
{
}


/***********************************************************************************************
 * ProgressBarClass::Is_Horizontal -- Determines if the bargraph is horizontal or not.         *
 *                                                                                             *
 *    If the bargraph is oriented horizontally, then this function will return TRUE.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this bargraph horizontal?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042B9C0)

bool ProgressBarClass::Is_Horizontal() const
{
    if (Width > Height) return true;
    return false;
}


/***********************************************************************************************
 * ProgressBarClass::Update -- Update the value and redraw as necessary.                       *
 *                                                                                             *
 *    This will update the value of the bargraph to the fill ratio specified and then          *
 *    redraw it if required. Very small changes to the bargraph value might not result in a    *
 *    visual change.                                                                           *
 *                                                                                             *
 * INPUT:   value -- The new value to assign to this bargraph.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   bool; Did this update result in a redraw?                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042B9D0)

bool ProgressBarClass::Update(double value)
{
    CurrentValue = value;
    if (!IsDrawn || value - LastDisplayCurrent >= 0.1) {
        Redraw();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * ProgressBarClass::Outline -- Draw an outline around the bargraph if supposed to.            *
 *                                                                                             *
 *    This routine will draw a border around the bargraph if this bargraph has a color         *
 *    specified for the border.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042BA10)

void ProgressBarClass::Outline() const
{
    if (Is_Outlined()) {
        unknown_surface_83995c->Draw_Rect(Rect(X, Y, Width, Height), unknown_drawer_8323c4->Convert(BorderColor));
    }
}


/***********************************************************************************************
 * ProgressBarClass::Redraw -- Redraw the bargraph.                                            *
 *                                                                                             *
 *    This will redraw the entire bargraph.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042BA80)

void ProgressBarClass::Redraw() const
{
    WWMouse->Hide_Mouse();
    Outline();

    int x = X;
    int y = Y;
    int w = Width;
    int h = Height;
    if (Is_Outlined()) {
        x += 1;
        y += 1;
        w -= 2;
        h -= 2;
    }

    int size = Is_Horizontal() ? w : h;
    int fill = CurrentValue * size;

    if (fill > 0) {
        if (Is_Horizontal()) {
            unknown_surface_83995c->Fill_Rect(Rect(x, y, fill, h), unknown_drawer_8323c4->Convert(BarColor));
        } else {
            unknown_surface_83995c->Fill_Rect(Rect(x, y + fill, w, h), unknown_drawer_8323c4->Convert(BarColor));
        }
    }

    if (w - fill > 0) {
        if (Is_Horizontal()) {
            unknown_surface_83995c->Fill_Rect(Rect(x + fill, y, w, h), unknown_drawer_8323c4->Convert(BackColor));
        } else {
            unknown_surface_83995c->Fill_Rect(Rect(x, y, w, fill - 1), unknown_drawer_8323c4->Convert(BackColor));
        }
    }
    WWMouse->Show_Mouse();

    ProgressBarClass *me = (ProgressBarClass *)this;
    me->LastDisplayCurrent = CurrentValue;
    me->IsDrawn = true;
}
