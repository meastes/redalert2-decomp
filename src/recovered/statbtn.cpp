// statbtn.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/statbtn.json carries the addresses and the rows that
// stay in their own files.


#include "shared/gadget.hpp"
#include "shared/blit.hpp"


// StaticButtonClass::StaticButtonClass(unsigned id, wchar_t const *text, int style, int x, int y, int w, int h) (0x00698760)
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


// StaticButtonClass::StaticButtonClass() (0x006987F0)

StaticButtonClass::StaticButtonClass() :
    GadgetClass(0, 0, 0, 0, 0, false),
    Text(0),
    Style(2)
{
}


// StaticButtonClass::~StaticButtonClass() (0x00698830)

StaticButtonClass::~StaticButtonClass()
{
    if (Text != 0) {
        delete Text;
    }
}


// int StaticButtonClass::Draw_Me(int forced) (0x00698860, slot 27)

int StaticButtonClass::Draw_Me(int forced)
{
    if (GadgetClass::Draw_Me(forced)) {
        Draw_Background();
        Draw_Text(Text);
        return true;
    }
    return false;
}


// void StaticButtonClass::Set_Text(wchar_t const *text, bool resize) (0x006988A0, slot 33)
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


// void StaticButtonClass::Draw_Background() (0x00698960, slot 34)
// Saves the screen area under the gadget on first use, then restores it.

void StaticButtonClass::Draw_Background()
{
    if (!Background.Is_Valid() && Width > 0 && Height > 0) {
        new (&Background) Buffer(unknown_surface_83995c->Bytes_Per_Pixel() * Height * Width);
        if (Background.Is_Valid()) {
            To_Buffer(*unknown_surface_83995c, Rect(X, Y, Width, Height), Background);
        }
    }
    if (Background.Is_Valid()) {
        From_Buffer(*unknown_surface_83995c, Rect(X, Y, Width, Height), Background);
    }
}


// void StaticButtonClass::Draw_Text(wchar_t const *text) (0x00698A10, slot 35)
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
        Fancy_Text_Printf(text, unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &Point2D(x, Y),
                          ColorSchemes[Gadget_Color_Scheme()], 0, Style);

    }
}


// StaticButtonClass scalar deleting destructor (0x00698AA0): compiler
// generated with the unit's vtable, the destructor expanded inline; no
// source of its own.
