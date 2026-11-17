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
// Function comment blocks from mischa85/RedAlert2 392e904 src/gauge.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Local variable names from mischa85/RedAlert2 392e904 src/gauge.cpp where both functions declare the same local types (names only).

// gauge.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/gauge.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"
#include "shared/mouse.hpp"
#include "shared/text_print.hpp"
#include "shared/color_scheme.hpp"
#include "shared/convert.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"


// Header-static dynamic initializer of the unit at 0x004D00D0: the empty
// rectangle of shared/shape_file.hpp (0x00866078), the unit's first function.


/***************************************************************************
 * GaugeClass::GaugeClass -- class constructor                             *
 *                                                                         *
 * INPUT:    id                     -- button ID                           *
 *                                                                         *
 *             x,y                     -- upper-left corner, in pixels     *
 *                                                                         *
 *             w,h                     -- width, height, in pixels         *
 *                                                                         *
 * OUTPUT:   none.                                                         *
 *                                                                         *
 * WARNINGS: none.                                                         *
 *                                                                         *
 * HISTORY:  01/05/1995 MML : Created.                                     *
 *=========================================================================*/
// (0x004D00F0)
// A sticky 0..255 gauge, horizontal when wider than tall, with a thumb.

GaugeClass::GaugeClass(unsigned id, int x, int y, int w, int h) :
    ControlClass(id, x, y, w, h, LEFTPRESS | LEFTHELD | LEFTRELEASE, true)
{
    Set_Maximum(255);
    Set_Value(0);
    HasThumb = true;
    IsHorizontal = (w > h);
    IsColorized = true;
    ClickDiff = 0;
}


/***********************************************************************************************
 * GaugeClass::Set_Maximum -- Sets the maximum value for the gauge.                            *
 *                                                                                             *
 *    This routine will set the maximum value for the gauge. This is the largest value that    *
 *    the current setting may reach. The ability to change this allows the gauge to use and    *
 *    return values that are convenient for the programmer's current needs.                    *
 *                                                                                             *
 * INPUT:   value -- The value to use as the gauge maximum.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the gauge maximum changed? A false indicates that the specified value    *
 *                already matches the current maximum.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004D0170, slot 34)

int GaugeClass::Set_Maximum(int value)
{
    if (value != MaxValue) {
        MaxValue = value;
        Flag_To_Redraw();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * GaugeClass::Set_Value -- Set the value of the gauge.                                        *
 *                                                                                             *
 *    This routine will set the current value for the gauge. This value is clipped to the      *
 *    limits of the gauge maximum.                                                             *
 *                                                                                             *
 * INPUT:   value -- The value to set at the new current value.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the current setting changed? A false indicates that the setting          *
 *                specified is the same as what was already there.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004D0190, slot 35)
// Clamps value to 0..MaxValue.

int GaugeClass::Set_Value(int value)
{
    int maximum = MaxValue;
    if (value < 0) {
        value = 0;
    } else if (value > maximum) {
        value = maximum;
    }
    if (value != CurValue) {
        CurValue = value;
        Flag_To_Redraw();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * GaugeClass::Pixel_To_Value -- Convert a pixel offset into a gauge value.                    *
 *                                                                                             *
 *    Use this routine to convert the specified pixel offset into a gauge value. This is used  *
 *    in translating mouse clicks into a corresponding setting for the gauge.                  *
 *                                                                                             *
 * INPUT:   pixel -- The pixel offset form the start of the gauge.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the setting value in gauge coordinates.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004D01C0, slot 40)
// The value at a screen coordinate along the gauge (inside the 1-pixel border).

int GaugeClass::Pixel_To_Value(int pixel)
{
    int maximum;
    if (IsHorizontal) {
        pixel -= X + 1;
        maximum = Width;
    } else {
        pixel -= Y + 1;
        maximum = Height;
    }
    maximum -= 2;
    if (pixel < 0) {
        pixel = 0;
    } else if (pixel > maximum) {
        pixel = maximum;
    }
    return (int)((double)pixel / maximum * MaxValue);
}


/***********************************************************************************************
 * GaugeClass::Value_To_Pixel -- Convert gauge value to pixel offset.                          *
 *                                                                                             *
 *    Use this routine to convert the specified gauge value into a pixel offset from the       *
 *    star of the gauge. This is used for thumb positioning.                                   *
 *                                                                                             *
 * INPUT:   value -- The value to convert to a pixel offset.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the pixel offset of the specified value from the start of the         *
 *          gauge.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004D0240, slot 41)
// The screen coordinate of a value along the gauge.

int GaugeClass::Value_To_Pixel(int value)
{
    int maximum;
    int start;
    if (IsHorizontal) {
        start = Width;
        maximum = X;
    } else {
        start = Height;
        maximum = Y;
    }
    start -= 2;
    return (int)((double)value / MaxValue * start + maximum);
}


/***********************************************************************************************
 * GaugeClass::Draw_Me -- Draws the body of the gauge.                                         *
 *                                                                                             *
 *    This routine will draw the body of the gauge if necessary.                               *
 *                                                                                             *
 * INPUT:      forced   -- Should the gauge be redrawn regardless of the current redraw flag?  *
 *                                                                                             *
 * OUTPUT:     bool; Was the gauge redrawn?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x004D0280, slot 27)
// A box; a colorized gauge fills it up to the value in the gadget colour
// scheme's colour; then the thumb.

int GaugeClass::Draw_Me(int forced)
{
    if (ControlClass::Draw_Me(forced)) {
        WWMouse->Conditional_Hide_Mouse(Rect(X, Y, Width, Height));
        Draw_Box(Rect(X, Y, Width, Height), 0, true);
        if (IsColorized) {
            int middle = Value_To_Pixel(CurValue);
            int color = ColorSchemes[Gadget_Color_Scheme()]->unknown_color_0x330;
            if (IsHorizontal) {
                if (middle >= X + 1) {
                    LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, middle - X - 1, Height - 2), NormalDrawer->Convert(color));
                }
            } else {
                if (middle >= Y + 1) {
                    LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, Width - 2, middle - Y - 1), NormalDrawer->Convert(color));
                }
            }
        }
        if (HasThumb) {
            Draw_Thumb();
        }
        WWMouse->Conditional_Show_Mouse();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * GaugeClass::Action -- Handles input events for the gauge.                                   *
 *                                                                                             *
 *    This routine will handle input event processing for the gauge. It will adjust the        *
 *    current setting of the gauge according to the mouse position.                            *
 *                                                                                             *
 * INPUT:   flags -- The input event that is the reason for this function call.                *
 *          key   -- The key code that caused the event.                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the even recognized, processed, and no further gadget scanning is        *
 *                desired (for this pass).                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004D0420, slot 31)
// Dragging the thumb sets the value under the mouse (minus the grab offset
// within the thumb). An unchanged value consumes the input.

int GaugeClass::Action(unsigned flags, unsigned &key)
{
    if (!HasThumb) {
        return false;
    }
    Sticky_Process(flags);
    if ((flags & LEFTPRESS) || ((flags & LEFTHELD) && StuckOn == this)) {
        if (flags & LEFTPRESS) {
            int curpix = Value_To_Pixel(CurValue);
            int clickpix = IsHorizontal ? WWMouse->Get_Mouse_X() : WWMouse->Get_Mouse_Y();
            if (clickpix > curpix && clickpix - curpix < Thumb_Pixels()) {
                ClickDiff = clickpix - curpix;
            } else {
                ClickDiff = 0;
            }
            int testval = Pixel_To_Value(IsHorizontal ? WWMouse->Get_Mouse_X() - ClickDiff : WWMouse->Get_Mouse_Y() - ClickDiff);
            while (testval < CurValue && ClickDiff > 0) {
                ClickDiff--;
                testval = Pixel_To_Value(IsHorizontal ? WWMouse->Get_Mouse_X() - ClickDiff : WWMouse->Get_Mouse_Y() - ClickDiff);
            }
        }
        if (!Set_Value(Pixel_To_Value(IsHorizontal ? WWMouse->Get_Mouse_X() - ClickDiff : WWMouse->Get_Mouse_Y() - ClickDiff))) {
            ControlClass::Action(0, key);
            key = 0;
            return true;
        }
    } else {
        flags &= ~LEFTHELD;
    }
    return ControlClass::Action(flags, key);
}


/***********************************************************************************************
 * GaugeClass::Draw_Thumb -- Draws the body of the gauge.                                      *
 *                                                                                             *
 *    This routine will draw the body of the gauge if necessary.                               *
 *                                                                                             *
 * INPUT:      none.                                                                           *
 *                                                                                             *
 * OUTPUT:     none.                                                                           *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
// (0x004D0580, slot 39)
// A raised box four pixels long at the current value, kept inside the gauge.

void GaugeClass::Draw_Thumb()
{
    int x = Value_To_Pixel(CurValue);
    if (x + 4 > Value_To_Pixel(MaxValue)) {
        x = Value_To_Pixel(MaxValue) - 2;
    }
    if (x < X) {
        x = X;
    }
    if (IsHorizontal) {
        Draw_Box(Rect(x, Y, 4, Height), 1, true);
    } else {
        Draw_Box(Rect(X, x, Width, 4), 1, true);
    }
}


/***********************************************************************************************
 * TriColorGaugeClass::TriColorGaugeClass -- Constructor for 3 color (red\yellow\green) gauge. *
 *                                                                                             *
 *    This routine will draw the body of the gauge if necessary.                               *
 *                                                                                             *
 * INPUT:      See below.                                                                      *
 *                                                                                             *
 * OUTPUT:     none.                                                                           *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
// (0x004D0630)

TriColorGaugeClass::TriColorGaugeClass(unsigned id, int x, int y, int w, int h) :
    GaugeClass(id, x, y, w, h),
    RedLimit(0),
    YellowLimit(0)
{
}


/***********************************************************************************************
 * TriColorGaugeClass::Set_Red_Limit -- Set the value for the red area of gauge.               *
 *                                                                                             *
 * INPUT:      int value.                                                                      *
 *                                                                                             *
 * OUTPUT:     bool true of false.                                                             *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
// (0x004D06D0, slot 42)

int TriColorGaugeClass::Set_Red_Limit(int value)
{
    if (value >= 0 && value < MaxValue) {
        RedLimit = value;
        Flag_To_Redraw();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * TriColorGaugeClass::Set_Yellow_Limit -- Set the value for the yellow area of gauge.         *
 *                                                                                             *
 * INPUT:      int value.                                                                      *
 *                                                                                             *
 * OUTPUT:     bool true of false.                                                             *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
// (0x004D0700, slot 43)

int TriColorGaugeClass::Set_Yellow_Limit(int value)
{
    if (value >= 0 && value < MaxValue) {
        YellowLimit = value;
        Flag_To_Redraw();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * TriColorGaugeClass::Draw_Me -- Draw the tri color gauge.                                    *
 *                                                                                             *
 * INPUT:      int forced -- draw or not?                                                      *
 *                                                                                             *
 * OUTPUT:     bool true of false.                                                             *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
// (0x004D0730, slot 27)
// Redraws the box (raised when disabled) and fills it in up to three colours:
// up to the red limit, the yellow limit and the value. Several rectangles
// keep the original's mixed corner/size arithmetic.

int TriColorGaugeClass::Draw_Me(int forced)
{
    if (GaugeClass::Draw_Me(forced)) {
        WWMouse->Conditional_Hide_Mouse(Rect(X, Y, Width, Height));
        Draw_Box(Rect(X, Y, Width, Height), IsDisabled ? 1 : 0, true);
        int red = Value_To_Pixel(RedLimit);
        int yellow = Value_To_Pixel(YellowLimit);
        int middle = Value_To_Pixel(CurValue);
        if (CurValue <= RedLimit) {
            if (IsHorizontal) {
                LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, middle - X + 1, Height - 2), NormalDrawer->Convert(6));
            } else {
                LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, Width - 2, middle - Y + 1), NormalDrawer->Convert(6));
            }
        } else if (CurValue > RedLimit && CurValue <= YellowLimit) {
            if (IsHorizontal) {
                LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, red - X + 1, Height - 2), NormalDrawer->Convert(6));
                LogicalSurface->Fill_Rect(Rect(red - X + 1, Y + 1, middle - X - 1, Height - 2), NormalDrawer->Convert(5));
            } else {
                LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, Width - 2, red - Y + 1), NormalDrawer->Convert(6));
                LogicalSurface->Fill_Rect(Rect(X + 1, red - Y + 1, X + Width - 2, middle - Y + 1), NormalDrawer->Convert(5));
            }
        } else if (CurValue > YellowLimit && CurValue <= MaxValue) {
            if (IsHorizontal) {
                LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, red - X + 1, Height - 2), NormalDrawer->Convert(6));
                LogicalSurface->Fill_Rect(Rect(red, Y + 1, yellow - X + 1, Height - 2), NormalDrawer->Convert(5));
                LogicalSurface->Fill_Rect(Rect(yellow, Y + 1, middle - X + 1, Height - 2), NormalDrawer->Convert(3));
            } else {
                LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, Width - 2, red - Y + 1), NormalDrawer->Convert(6));
                LogicalSurface->Fill_Rect(Rect(X + 1, red, Width - 2, yellow - Y + 1), NormalDrawer->Convert(5));
                LogicalSurface->Fill_Rect(Rect(X + 1, yellow, Width - 2, middle - Y + 1), NormalDrawer->Convert(3));
            }
        }
        if (HasThumb) {
            Draw_Thumb();
        }
        WWMouse->Conditional_Show_Mouse();
        return true;
    }
    return false;
}


// GaugeClass::Get_Value (0x004D0C80)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// GaugeClass::Use_Thumb (0x004D0C90)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// GaugeClass::Thumb_Pixels (0x004D0CA0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// GaugeClass::scalar_deleting_destructor (0x004D0CB0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// TriColorGaugeClass::scalar_deleting_destructor (0x004D0CD0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.
