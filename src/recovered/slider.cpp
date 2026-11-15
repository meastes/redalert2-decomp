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
// Function comment blocks from mischa85/RedAlert2 392e904 src/slider.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// slider.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/slider.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializer of the unit at 0x006846C0: the empty
// rectangle of shared/shape_file.hpp (0x00ABCE00), the unit's first function.
#include "shared/shape_file.hpp"




#include "shared/gadget.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/rect.hpp"
#include "shared/mouse.hpp"


/***********************************************************************************************
 * SliderClass::SliderClass -- Normal constructor for a slider (with thumb) gadget.            *
 *                                                                                             *
 *    This is the normal constructor for the slider gadget.                                    *
 *                                                                                             *
 * INPUT:   id    -- The ID number to assign to this gadget.                                   *
 *          x,y   -- The pixel coordinate of the upper left corner for this gadget.            *
 *          w,h   -- The width and height of the slider gadget. The slider automatically       *
 *                   adapts for horizontal or vertical operation depending on which of these   *
 *                   dimensions is greater.                                                    *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x006846E0)
// A gauge with a thumb; unless no_buttons, a plus button right of it and a minus button left of it
// become its peers. The thumb starts one value wide.

SliderClass::SliderClass(unsigned id, int x, int y, int w, int h, int no_buttons)
    : GaugeClass(id, x, y, w, h)
{
    NoButtons = no_buttons ? true : false;

    PlusGadget = 0;
    MinusGadget = 0;
    if (!NoButtons) {
        PlusGadget = new ShapeButtonClass(id, MixFileClass::Retrieve("BTN-PLUS.SHP"), X + Width + 2, Y);
        MinusGadget = new ShapeButtonClass(id, MixFileClass::Retrieve("BTN-MINS.SHP"), X - 6, Y);

        if (PlusGadget) {
            PlusGadget->Make_Peer(*this);
            PlusGadget->Add(*this);
            PlusGadget->Flag_To_Redraw();
        }
        if (MinusGadget) {
            MinusGadget->Make_Peer(*this);
            MinusGadget->Add(*this);
            MinusGadget->Flag_To_Redraw();
        }
    }
    Set_Thumb_Size(1);
    Recalc_Thumb();

    IsColorized = 0;
}


/***********************************************************************************************
 * SliderClass::~SliderClass -- Destructor for slider object.                                  *
 *                                                                                             *
 *    This cleans up the slider object in preparation for deletion.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006848C0)
// Deletes the plus and minus buttons.

SliderClass::~SliderClass()
{
    if (PlusGadget != 0) {
        delete PlusGadget;
        PlusGadget = 0;
    }
    if (MinusGadget != 0) {
        delete MinusGadget;
        MinusGadget = 0;
    }
}


/***********************************************************************************************
 * SliderClass::Set_Maximum -- Sets the maximum value for this slider.                         *
 *                                                                                             *
 *    This sets the maximum value that the slider can be set at. The maximum value controls    *
 *    the size of the thumb and the resolution of the thumb's movement.                        *
 *                                                                                             *
 * INPUT:   value -- The value to set for the slider's maximum.                                *
 * OUTPUT:  bool; Was the maximum value changed? A false indicates a set to the value it       *
 *                is currently set to already.                                                 *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684900, slot 34)

int SliderClass::Set_Maximum(int value)
{
    if (GaugeClass::Set_Maximum(value)) {
        Recalc_Thumb();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * SliderClass::Set_Thumb_Size -- Sets the size of the thumb in "slider units".                *
 *                                                                                             *
 *    This routine will set the size of the thumb as it relates to the maximum value the       *
 *    slider can achieve. This serves to display a proportionally sized thumb as well as       *
 *    control how the slider "bumps" up or down.                                               *
 *                                                                                             *
 * INPUT:   value -- The new value of the thumb. It should never be larger than the slider     *
 *                   maximum.                                                                  *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684980, slot 42)
// Thumb becomes value, at most MaxValue and at least 1.

void SliderClass::Set_Thumb_Size(int value)
{
    Thumb = (value < MaxValue) ? value : MaxValue;
    Thumb = (Thumb > 1) ? Thumb : 1;
    Flag_To_Redraw();
    Recalc_Thumb();
}


/***********************************************************************************************
 * SliderClass::Set_Value -- Sets the current thumb position for the slider.                   *
 *                                                                                             *
 *    This routine will set the thumb position for the slider.                                 *
 *                                                                                             *
 * INPUT:   value -- The position to set the slider. This position is relative to the maximum  *
 *                   value for the slider.                                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the slider thumb position changed at all?                                *
 * WARNINGS:  none                                                                             *
 * HISTORY:   01/15/1995 JLB : Created.                                                        *
 *=============================================================================================*/
// (0x00684A10, slot 35)
// The value is kept within MaxValue - Thumb.

int SliderClass::Set_Value(int value)
{
    int maximum = MaxValue - Thumb;
    if (value >= maximum) {
        value = maximum;
    }
    if (GaugeClass::Set_Value(value)) {
        Recalc_Thumb();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * SliderClass::Recalc_Thumb -- Recalculates the thumb pixel size and starting offset.         *
 *                                                                                             *
 *    This takes the current thumb logical size and starting value and calculates the pixel    *
 *    size and starting offset accordingly. This function should be called whenever one of     *
 *    these elements has changed.                                                              *
 *                                                                                             *
 * INPUT:      none                                                                            *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684AA0)
// The thumb's pixel size (at least 4) and start for the current value.

void SliderClass::Recalc_Thumb()
{
    int length = IsHorizontal ? Width : Height;
    int size = (int)((double)Thumb / MaxValue * length);
    ThumbSize = (size > 4) ? size : 4;
    int start = (int)((double)CurValue / MaxValue * length);
    ThumbStart = (start < length - ThumbSize) ? start : length - ThumbSize;
}


/***********************************************************************************************
 * SliderClass::Action -- Handles input processing for the slider.                             *
 *                                                                                             *
 *    This routine is called when a qualifying input event has occurred. This routine will     *
 *    process that event and make any adjustments to the slider as necessary.                  *
 *                                                                                             *
 * INPUT:   flags -- Flag bits that tell the input event that caused this function to          *
 *                   be called.                                                                *
 *          key   -- Reference to the key that caused the input event.                         *
 * OUTPUT:  bool; Was the event consumed and further processing of the gadget list should be   *
 *                aborted?                                                                     *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684B10, slot 31)
// A left press before the thumb bumps the value down a thumb, one after it bumps it up;
// a press on the thumb itself goes to the gauge. The key is consumed either way.

int SliderClass::Action(unsigned flags, unsigned &key)
{
    if (flags & LEFTPRESS) {
        int mouse;
        int edge;

        if (IsHorizontal) {
            mouse = WWMouse->Get_Mouse_X();
            edge = X;
        } else {
            mouse = WWMouse->Get_Mouse_Y();
            edge = Y;
        }
        edge += 1;

        if (mouse < edge + ThumbStart) {
            Bump(true);
            GaugeClass::Action(0, key);
            key = 0;
            return true;
        } else {
            if (mouse > edge + ThumbStart + ThumbSize) {
                Bump(false);
                GaugeClass::Action(0, key);
                key = 0;
                return true;
            } else {
                GaugeClass::Action(flags, key);
                key = 0;
                return true;
            }
        }
    }

    return GaugeClass::Action(flags, key);
}


/***********************************************************************************************
 * SliderClass::Bump -- Bumps the slider one "thumb size" up or down.                          *
 *                                                                                             *
 *    This support function will bump the slider one "step" or the size of the thumb up or     *
 *    down as specified. It is typically called when the slider is clicked outside of the      *
 *    thumb region but still inside of the slider.                                             *
 *                                                                                             *
 * INPUT:   up -- Should the bump be to increase the current position?                         *
 * OUTPUT:  bool; Was the slider changed at all? A false indicates that the slider is already  *
 *                at one end or the other.                                                     *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684BB0, slot 43)
// Moves the value by a whole thumb.

int SliderClass::Bump(int up)
{
    if (up) {
        return Set_Value(CurValue - Thumb);
    } else {
        return Set_Value(Thumb + CurValue);
    }
}


/***********************************************************************************************
 * SliderClass::Step -- Steps the slider one value up or down.                                 *
 *                                                                                             *
 *    This routine will move the slider thumb one step in the direction specified.             *
 *                                                                                             *
 * INPUT:   up -- Should the step be up (i.e., forward)?                                       *
 * OUTPUT:  bool; Was the slider changed at all? A false indicates that the slider is already  *
 *                at one end or the other.                                                     *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684BF0, slot 44)
// Moves the value by one.

int SliderClass::Step(int up)
{
    if (up) {
        return Set_Value(CurValue - 1);
    } else {
        return Set_Value(CurValue + 1);
    }
}


/***********************************************************************************************
 * SliderClass::Draw_Thumb -- Draws the "thumb" for this slider.                               *
 *                                                                                             *
 *    This will draw the thumb graphic for this slider. Sometimes the thumb requires special   *
 *    drawing, thus the need for this function separate from the normal Draw_Me function.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   The mouse is guaranteed to be hidden when this routine is called.               *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684C20, slot 39)

void SliderClass::Draw_Thumb()
{
    if (IsHorizontal) {
        Draw_Box(Rect(X + ThumbStart, Y, ThumbSize, Height), 1, true);
    } else {
        Draw_Box(Rect(X, Y + ThumbStart, Width, ThumbSize), 1, true);
    }
}


/***********************************************************************************************
 * SliderClass::Draw_Me -- Draws the body of the gauge.                                        *
 *                                                                                             *
 *    This routine will draw the body of the gauge if necessary.                               *
 *                                                                                             *
 * INPUT:   forced   -- Should the gauge be redrawn regardless of the current redraw flag?     *
 * OUTPUT:  bool; Was the gauge redrawn?                                                       *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684CA0, slot 27)

int SliderClass::Draw_Me(int forced)
{
    if (NoButtons && ControlClass::Draw_Me(forced)) {
        Draw_Box(Rect(X, Y, Width, Height), 0, true);
        Draw_Thumb();
        return true;
    }
    return GaugeClass::Draw_Me(forced);
}


/***********************************************************************************************
 * SliderClass::Peer_To_Peer -- A peer gadget was touched -- make adjustments.                 *
 *                                                                                             *
 *    This routine is called when one of the peer gadgets (the scroll arrows or the slider)    *
 *    was touched in some fashion. This routine will sort out whom and why and then make       *
 *    any necessary adjustments to the list box.                                               *
 *                                                                                             *
 * INPUT:   flags    -- The event flags that affected the peer gadget.                         *
 *          key      -- The key value at the time of the event.                                *
 *          whom     -- Which gadget is being touched.                                         *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00684D10, slot 19)
// Releasing the plus button steps up the value, the minus button down.

void SliderClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom)
{
    if (flags & LEFTRELEASE) {
        if (&whom == PlusGadget) {
            Step(false);
        }
        if (&whom == MinusGadget) {
            Step(true);
        }
    }
}
