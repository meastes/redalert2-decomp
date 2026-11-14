// textbtn.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/textbtn.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"


// Header-static dynamic initializer of the unit at 0x006E7220: the empty
// rectangle of shared/shape_file.hpp (0x00AC0848), the unit's first function.


// TextButtonClass::TextButtonClass(unsigned id, wchar_t const *text, int style, int x, int y, int w, int h, bool blackborder, bool bordered) (0x006E7240)
// A width or height of -1 is measured from the text and the current font.

TextButtonClass::TextButtonClass(unsigned id, wchar_t const *text, int style, int x, int y, int w, int h,
                                 bool blackborder, bool bordered) :
    ToggleClass(id, x, y, w, h),
    IsBlackBorder(blackborder),
    IsBordered(bordered),
    String(text),
    PrintFlags(style)
{
    if (w == -1 || h == -1) {
        BitFontClass *font = Font_For_Style(PrintFlags);
        if (w == -1) {
            Width = font->String_Pixel_Width(String, 0) + 8;
        }
        if (h == -1) {
            Height = font->Height + 2;
        }
    }
}


// TextButtonClass::TextButtonClass() (0x006E72D0)
// An empty button with the default text style 2.

TextButtonClass::TextButtonClass() :
    ToggleClass(0, 0, 0, 0, 0)
{
    IsBlackBorder = false;
    String = 0;
    PrintFlags = 2;
}


// int TextButtonClass::Draw_Me(int forced) (0x006E7300, slot 27)

int TextButtonClass::Draw_Me(int forced)
{
    if (ControlClass::Draw_Me(forced)) {
        Draw_Background();
        Draw_Text(String);
        return true;
    }
    return false;
}


// void TextButtonClass::Set_Text(wchar_t const *text, bool resize) (0x006E7340, slot 34)
// Changes the text; resize refits the button to it.

void TextButtonClass::Set_Text(wchar_t const *text, bool resize)
{
    String = text;
    Flag_To_Redraw();
    if (resize && String != 0) {
        BitFontClass *font = Font_For_Style(PrintFlags);
        Width = font->String_Pixel_Width(String, 0) + 8;
        Height = font->Height + 2;
    }
}


// void TextButtonClass::Draw_Background() (0x006E7390, slot 36)
// Unless the button is bordered: an outline one pixel outside it (palette
// index 12 of the box colour scheme) when it has a black border, then a
// filled box: disabled raised (3), pressed down (0) or raised (1).

void TextButtonClass::Draw_Background()
{
    if (!IsBordered) {
        if (IsBlackBorder) {
            unknown_surface_83995c->Draw_Rect(Rect(X - 1, Y - 1, X + Width + 2, Y + Height + 2),
                                              ColorSchemes[Gadget_Color_Scheme()]->Converter->Convert(12));
        }
        int style;
        if (IsDisabled) {
            style = 3;
        } else {
            style = !IsPressed;
        }
        Draw_Box(Rect(X, Y, Width, Height), style, true);
    }
}


// void TextButtonClass::Draw_Text(wchar_t const *text) (0x006E7460, slot 37)
// Prints the text centred on the button (one pixel down) in the box colour
// scheme (scheme 0 when the index is out of range): disabled plain, pressed
// or on with 0x6000, otherwise with 0x5000, added to the button's print
// flags and centring (0x100).

void TextButtonClass::Draw_Text(wchar_t const *text)
{
    int index = Gadget_Color_Scheme();
    if (String != 0) {
        unsigned flags;
        if (IsDisabled) {
            flags = 0;
        } else if (IsPressed || IsOn) {
            flags = 0x6000;
        } else {
            flags = 0x5000;
        }
        if (ColorSchemes.Count() != 0) {
            if (index <= 0 || index >= ColorSchemes.Count()) {
                index = 0;
            }
            ColorScheme *scheme = ColorSchemes[index];
            if (scheme != 0) {
                Point2D point(X + (Width >> 1) - 1, Y + 1);
                Fancy_Text_Print(text, unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &point,
                                 scheme, 0, PrintFlags | flags | 0x100, 1);
            }
        }
    }
}


// void TextButtonClass::Set_Style(int style) (0x006E7520, slot 35)

void TextButtonClass::Set_Style(int style)
{
    PrintFlags = style;
}


// void *TextButtonClass::`scalar deleting destructor'(unsigned int) (0x006E7530)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.
