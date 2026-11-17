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
// Function comment blocks from mischa85/RedAlert2 392e904 src/statbtn.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// statbtn.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/statbtn.json carries the addresses and the rows that
// stay in their own files.


#include "shared/gadget.hpp"
#include "shared/blit.hpp"


/***********************************************************************************************
 * StaticButtonClass::StaticButtonClass -- Normal constructor for a text button.               *
 *                                                                                             *
 *    This is the constructor for text buttons if the text is provided as a string pointer.    *
 *                                                                                             *
 * INPUT:   id    -- The ID number to assign to this button.                                   *
 *                                                                                             *
 *          text  -- Pointer to the text string to display on top of the button.               *
 *                                                                                             *
 *          x,y   -- Pixel coordinate of button's upper left corner.                           *
 *                                                                                             *
 *          w,h   -- Dimensions of the button. If these are not filled in (or with -1), then   *
 *                   the dimensions are adapted to fit the text assigned to the button.        *
 *                                                                                             *
 *          style -- The print style for the text in the button. These are the TPF_ flags      *
 *                   used by Fancy_Text_Print().                                               *
 *                                                                                             *
 * OUTPUT:   none                                                                              *
 *                                                                                             *
 * WARNINGS: Call Set_Text & Set_Style, & init X,Y,Width,Height,ID before using this button.   *
 *                                                                                             *
 * HISTORY:  01/15/1995 JLB : Created.                                                         *
 *=============================================================================================*/
// (0x00698760)
// A width or height of -1 is taken from the text in the style's font.

StaticButtonClass::StaticButtonClass(unsigned id, wchar_t const *text, int style, int x, int y, int w, int h) :
    GadgetClass(x, y, w, h, 0, false),
    Text(0),
    Style(style)
{
    Set_Text(text);
    if (w == -1 || h == -1) {
        BitFontClass *font = Font_For_Style(Style);
        if (w == -1) {
            Width = font->String_Pixel_Width(Text, 0);
        }
        if (h == -1) {
            Height = font->Height;
        }
    }
}


/***********************************************************************************************
 * StaticButtonClass::StaticButtonClass -- Default constructor for a text button.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:   none                                                                              *
 *                                                                                             *
 * WARNINGS: none                                                                              *
 *                                                                                             *
 * HISTORY:  01/15/1995 JLB : Created.                                                         *
 *=============================================================================================*/
// (0x006987F0)

StaticButtonClass::StaticButtonClass() :
    GadgetClass(0, 0, 0, 0, 0, false),
    Text(0),
    Style(2)
{
}


/// <summary>
/// Destroys the static button.
/// The button holds its own copy of the text it displays, so that copy is freed here.
/// </summary>
// (0x00698830)

StaticButtonClass::~StaticButtonClass()
{
    if (Text != 0) {
        delete Text;
    }
}


/***********************************************************************************************
 * StaticButtonClass::Draw_Me -- Draws the text buttons as indicated.                          *
 *                                                                                             *
 *    This routine will draw the text button.                                                  *
 *                                                                                             *
 * INPUT:   forced   -- If the button is to be redrawn regardless of the state of the redraw   *
 *                      flag, then this parameter will be true.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the button redrawn?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/03/1995 MML : Created.                                                                 *
 *   01/16/1995 JLB : Modified                                                                 *
 *=============================================================================================*/
// (0x00698860, slot 27)

int StaticButtonClass::Draw_Me(int forced)
{
    if (GadgetClass::Draw_Me(forced)) {
        Draw_Background();
        Draw_Text(Text);
        return true;
    }
    return false;
}


/***********************************************************************************************
 * StaticButtonClass::Set_Text -- Assigns a new text string to this button.                    *
 *                                                                                             *
 *    Use this routine to assign a new text string to this button. By using this function it   *
 *    is possible to dynamically change the button's text. An example of this would be an      *
 *    on/off button that every time it is clicked, the text toggles between "on" and "off".    *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to assign to this button.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The text is NOT copied to this button. You must make sure that the text         *
 *             remains valid throughout the lifetime of this text button.                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006988A0, slot 33)
// Replaces the owned copy of the text and flags a redraw; with resize, the
// gadget takes the text's size in the style's font (after restoring the
// screen under it and discarding the saved area).

void StaticButtonClass::Set_Text(wchar_t const *text, bool resize)
{
    if (Text != 0) {
        delete Text;
        Text = 0;
    }
    if (text != 0) {
        Text = new wchar_t[wcslen(text) + 1];
        if (Text != 0) {
            wcscpy(Text, text);
        }
    }
    Flag_To_Redraw();
    if (resize && Text != 0) {
        Draw_Background();
        BitFontClass *font = Font_For_Style(Style);
        Width = font->String_Pixel_Width(Text, 0);
        Height = font->Height;
        Background = Buffer();
    }
}


/***********************************************************************************************
 * StaticButtonClass::Draw_Background -- Draws the background to the text button.              *
 *                                                                                             *
 *    This localizes the drawing of the background for the text button. By overriding this     *
 *    function you can give a different background to the button. The text is drawn using      *
 *    a different routine. The mouse is hidden, if necessary, before this routine is called.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00698960, slot 34)
// Saves the screen area under the gadget on first use, then restores it.

void StaticButtonClass::Draw_Background()
{
    if (!Background.Is_Valid() && Width > 0 && Height > 0) {
        new (&Background) Buffer(LogicalSurface->Bytes_Per_Pixel() * Height * Width);
        if (Background.Is_Valid()) {
            To_Buffer(*LogicalSurface, Rect(X, Y, Width, Height), Background);
        }
    }
    if (Background.Is_Valid()) {
        From_Buffer(*LogicalSurface, Rect(X, Y, Width, Height), Background);
    }
}


/***********************************************************************************************
 * StaticButtonClass::Draw_Text -- This draws the text for the text button.                    *
 *                                                                                             *
 *    This routine draws the text for the text button. You can override this routine if you    *
 *    wish different text rendering styles or colors. The background has already been drawn    *
 *    by the time this function is called. The mouse is hidden, if necessary, as well.         *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to print over the button.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00698A10, slot 35)
// Prints text at the gadget, centred or right-aligned by Style, in the
// gadgets' colour scheme.

void StaticButtonClass::Draw_Text(wchar_t const *text)
{
    if (Text != 0) {
        int x = X;
        if (Style & 0x100) {
            x += Width / 2;
        }
        if (Style & 0x200) {
            x += Width - 1;
        }
        Fancy_Text_Printf(text, LogicalSurface, &LogicalSurface->Get_Rect(), &Point2D(x, Y),
                          ColorSchemes[Gadget_Color_Scheme()], 0, Style);

    }
}


// StaticButtonClass scalar deleting destructor (0x00698AA0): compiler
// generated with the unit's vtable, the destructor expanded inline; no
// source of its own.
