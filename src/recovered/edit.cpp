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
// Function comment blocks from mischa85/RedAlert2 392e904 src/edit.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/edit.cpp where both define the function (names only).

// edit.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/edit.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"
#include "shared/mouse.hpp"
#include "shared/keyboard.hpp"


// Header-static dynamic initializer of the unit at 0x004B2950: the empty
// rectangle of shared/shape_file.hpp (0x00853080), the unit's first function.


/***********************************************************************************************
 * EditClass::EditClass -- Normal constructor for edit class object.                           *
 *                                                                                             *
 *    This is the normal constructor used to create an edit object.                            *
 *                                                                                             *
 * INPUT:   id    -- The ID number for this edit object. This is the ID number that will be    *
 *                   returned by the Input() function when the <RETURN> key is pressed if this *
 *                   gadget has the keyboard input focus.                                      *
 *                                                                                             *
 *          text  -- Reference to the text buffer that the edit gadget will modify as keyboard *
 *                   input is processed. The value that this buffer contains is the default    *
 *                   text displayed.                                                           *
 *                                                                                             *
 *          maxlen-- The maximum size of the text buffer specified. This length INCLUDES the   *
 *                   trailing null character so a simple sizeof() function call can be used.   *
 *                                                                                             *
 *          flags -- These are the text print control flags. It is used to control how the     *
 *                   text looks in the edit box. Use the normal TPF_??? flags.                 *
 *                                                                                             *
 *          x,y   -- The pixel coordinates of the upper left corner of the edit gadget area.   *
 *                                                                                             *
 *          w,h   -- The pixel dimensions of the edit box. If either of these are no provided, *
 *                   or set to -1, then the dimension is determined from the string itself.    *
 *                                                                                             *
 *          sytle -- This style flag parameter control what kind of characters are allowed in  *
 *                   the edit box. The initial string in the text buffer may contain illegal   *
 *                   characters, but they are NOT removed regardless of this parameter.        *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   01/05/1995 MML : Created.                                                                 *
 *   01/21/1995 JLB : Modified.                                                                *
 *=============================================================================================*/
// (0x004B2970)
// A width or height of -1 fits the text (or max_len wide characters) in the
// style's font.

EditClass::EditClass(unsigned id, wchar_t *text, int max_len, unsigned flags, int x, int y, int w, int h,
                     int style) :
    ControlClass(id, x, y, w, h, LEFTPRESS, false),
    TextFlags(flags & ~KEYBOARD)
{
    EditFlags = style;
    Set_Text(text, max_len);
    Color = Gadget_Color_Scheme();
    if (w == -1 || h == -1) {
        BitFontClass *font = Font_For_Style(flags);
        if (h == -1) {
            Height = font->Height + 1;
        }
        if (w == -1) {
            if (wcslen(String) > 0) {
                Width = font->String_Pixel_Width(String, 0) + 6;
            } else {
                Width = font->String_Pixel_Width(L"X", 0) * (MaxLength + 1) + 2;
            }
        }
    }
    IsReadOnly = 0;
}


/***********************************************************************************************
 * EditClass::~EditClass -- Default destructor for the edit gadget.                            *
 *                                                                                             *
 *    This default destructor removes the focus setting if it currently has it.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004B2A60)

EditClass::~EditClass()
{
    if (Has_Focus()) {
        Clear_Focus();
    }
}


/***********************************************************************************************
 * EditClass::Set_Text -- Sets the text to the edit gadget.                                    *
 *                                                                                             *
 *    Use this routine to change the text that this edit gadget refers to.                     *
 *                                                                                             *
 * INPUT:   text     -- Reference to the character array that this edit gadget will be         *
 *                      modifying.                                                             *
 *          max_len  -- The maximum size of the buffer that will be modified.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004B2A90, slot 34)

void EditClass::Set_Text(wchar_t *text, int max_len)
{
    String = text;
    MaxLength = max_len - 1;
    Length = wcslen(String);
    Flag_To_Redraw();
}


/***********************************************************************************************
 * EditClass::Draw_Me -- Draws the edit box and embedded text.                                 *
 *                                                                                             *
 *    This routine will render the edit box. This will show the box outline as well as any     *
 *    text it may contain.                                                                     *
 *                                                                                             *
 * INPUT:   forced   -- Should the edit box be drawn even if it thinks it doesn't have to?     *
 *                                                                                             *
 * OUTPUT:  Was the edit box drawn?                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004B2AC0, slot 27)
// Draws the field with the mouse hidden over it.

int EditClass::Draw_Me(int forced)
{
    if (ControlClass::Draw_Me(forced)) {
        WWMouse->Conditional_Hide_Mouse(Rect(X, Y, Width, Height));
        Draw_Background();
        Draw_Text(String);
        WWMouse->Conditional_Show_Mouse();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * EditClass::Action -- Handles input events.                                                  *
 *                                                                                             *
 *    This routine will handle all mouse and keyboard events directed at this edit box         *
 *    gadget. For keyboard events, this will insert the characters into the edit box.          *
 *                                                                                             *
 * INPUT:   flags -- The event flag that triggered this function call.                         *
 *                                                                                             *
 *          key   -- Reference to the keyboard/mouse event that triggered this function call.  *
 *                                                                                             *
 * OUTPUT:  Should the list be processed further?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int EditClass::Action(unsigned flags, unsigned &key)
{
    if (IsReadOnly) return 0;
    if (flags & LEFTPRESS) {
        flags &= ~LEFTPRESS;
        Set_Focus();
        Flag_To_Redraw();
    }
    if ((flags & KEYBOARD) && Has_Focus()) {
        unsigned current_key = key;
        if (current_key == 27) {
            Clear_Focus();
            flags = 0;
            goto done;
        }
        int ascii = (unsigned char)WWKeyboard->To_ASCII((KeyNumType)current_key);
        current_key = key;
        if ((current_key & 0x1000) && ascii >= '0' && ascii <= '9') {
            key &= ~0x1000;
            if (flags & 0x44) goto done;
            if (!Handle_Key(ascii)) goto done;
        } else {
            if (((current_key & 0x1000) == 0 && ascii >= 32 && ascii <= 255) || current_key == 13 || current_key == 8) {
                if (flags & 0x44) goto done;
                if (!Handle_Key(WWKeyboard->To_ASCII((KeyNumType)current_key))) goto done;
            }
        }
        flags &= ~KEYBOARD;
        key = 0;
    }
done:
    return ControlClass::Action(flags, key);
}


/***********************************************************************************************
 * EditClass::Draw_Background -- Draw the background to the edit gadget.                       *
 *                                                                                             *
 *    This routine will redraw the edit gadget background. The overlaying text is handled by   *
 *    a different routine. The mouse is guaranteed to be hidden when this routine is called.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004B2C50, slot 36)
// An outlined box (style 4), filled.

void EditClass::Draw_Background()
{
    Draw_Box(Rect(X, Y, Width, Height), 4, true);
}


/***********************************************************************************************
 * EditClass::Draw_Text -- Draws the edit gadget text.                                         *
 *                                                                                             *
 *    This routine is called when the edit gadget text needs to be drawn. The background has   *
 *    already been drawn by the time this function is called. The mouse is guaranteed to be    *
 *    hidden as well.                                                                          *
 *                                                                                             *
 * INPUT:   text  -- The text to draw in the edit gadget.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004B2C90, slot 37)
// Prints the text inside the box (bright, 0x2000, while it has the focus)
// in colour scheme Color, clipped to the inner width; with the focus and
// room for another character, an underscore cursor follows the text.

void EditClass::Draw_Text(wchar_t const *text)
{
    unsigned flags = Has_Focus() ? 0x2000 : 0;
    Point2D point(X + 1, Y + 1);
    Fancy_Clip_Text_Print(text, LogicalSurface, &LogicalSurface->Get_Rect(), &point,
                          ColorSchemes[Color], 0, TextFlags | flags, Width - 2, 0);

    BitFontClass *font = Font_For_Style(TextFlags);
    if (Has_Focus() && wcslen(text) < (unsigned)MaxLength
        && font->String_Pixel_Width(text, 0) + font->String_Pixel_Width(L"_", 0) < Width - 2) {
        point.X = X + 1 + font->String_Pixel_Width(text, 0);
        point.Y = Y + 1;
        Fancy_Clip_Text_Print(L"_", LogicalSurface, &LogicalSurface->Get_Rect(), &point,
                              ColorSchemes[Color], 0, TextFlags | flags, (unsigned)-1, 0);
    }
}


/***********************************************************************************************
 * EditClass::Handle_Key -- Handles keyboard input to edit gadget.                             *
 *                                                                                             *
 *    This is the gruntwork routine that processes keyboard input to the edit gadget. This     *
 *    routine will be called when keyboard input has been detected and this gadget has the     *
 *    current focus.                                                                           *
 *                                                                                             *
 * INPUT:   ascii -- The ASCII key code that was fetched from the keyboard buffer.             *
 *                                                                                             *
 * OUTPUT:  bool; Should this keyboard input NOT cause the gadget ID number to be returned     *
 *                from the controlling Input() routine? Typically, the return value would be   *
 *                true unless the focus is lost due to the <RETURN> key being pressed.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004B2DD0, slot 38)
// Westwood's edit-key handling: 0 does nothing, backspace (8) removes the
// last character, return (13) drops the focus (and answers false); any
// other key that fits (pixel width and MaxLength) is appended when allowed
// by EditFlags (1 letters, 2 digits, 4 other printable characters,
// 8 upper-case letters); spaces always, but not as the first character.

extern "C" int __cdecl isgraph(int character);     // 0x00782C8D
extern "C" int __cdecl isalpha(int character);     // 0x00782B0E
extern "C" int __cdecl isdigit(int character);     // 0x00782B8C
extern "C" int __cdecl isalnum(int character);     // 0x00782C31
extern "C" int __cdecl toupper(int character);     // 0x007829D3

bool EditClass::Handle_Key(int ascii)
{
    BitFontClass *font = Font_For_Style(TextFlags);

    switch (ascii) {
    case 0:
        break;

    case 8:
        if (Length != 0) {
            Length--;
            String[Length] = 0;
            Flag_To_Redraw();
        }
        break;

    case 13:
        Clear_Focus();
        return false;

    default:
        if (font->String_Pixel_Width(String, 0) + font->Char_Pixel_Width(ascii) < Width - 2
            && (unsigned)Length < (unsigned)MaxLength) {
            if (!isgraph(ascii) && ascii != ' ') {
                break;
            }
            if (ascii == ' ' && Length == 0) {
                break;
            }
            if ((EditFlags & 8) && isalpha(ascii)) {
                ascii = toupper(ascii);
            }
            if ((!(EditFlags & 2) || !isdigit(ascii))
                && (!(EditFlags & 1) || !isalpha(ascii))
                && (!(EditFlags & 4) || isalnum(ascii))
                && ascii != ' ') {
                break;
            }
            String[Length++] = (wchar_t)ascii;
            String[Length] = 0;
            Flag_To_Redraw();
        }
        break;
    }
    return true;
}


/// <summary>
/// Gives the keyboard focus to this edit gadget.
/// This routine will resynchronize the edit length with whatever text the gadget was
/// handed before letting the normal focus processing proceed, so that typing continues
/// from the end of the existing text rather than overwriting it.
/// </summary>
// (0x004B2F20, slot 20)
// Puts the cursor at the end of the text.

void EditClass::Set_Focus()
{
    Length = 0;
    if (String != 0) {
        Length = wcslen(String);
    }
    GadgetClass::Set_Focus();
}
