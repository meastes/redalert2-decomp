// txtlabel.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/txtlabel.json carries the addresses and the rows that
// stay in their own files.


#include "shared/text_label.hpp"
#include "shared/color_scheme.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/surface.hpp"
#include "shared/win32_api.hpp"


// TextLabelClass::TextLabelClass(wchar_t const *text, int x, int y, int color, unsigned style) (0x006F15C0)
// A one-pixel gadget at x, y printing text in colour scheme color with print style style; the
// width is the surface's (PixWidth -1) and the text is shown at once (no reveal).

TextLabelClass::TextLabelClass(wchar_t const *text, int x, int y, int color, unsigned style) :
    GadgetClass(x, y, 1, 1, 0, 0),
    UserData1(0),
    UserData2(0),
    Style(style),
    Text(text),
    Color(color),
    PixWidth(-1),
    Scroll(0),
    IsHidden(false),
    IsRevealed(false),
    RevealCount(0),
    RevealTime(0)
{
}


// int TextLabelClass::Draw_Me(int forced) (0x006F1620, slot 27)
// Prints the label (Tiberian Sun txtlabel.cpp's Draw_Me with RA2's edit text printer): nothing while
// hidden; clipped to the drawing surface's right edge; with IsRevealed one more character every
// 16 ms, with the message typing sound while characters remain. The caret shows while focused.

int TextLabelClass::Draw_Me(int forced)
{
    if (!IsHidden && GadgetClass::Draw_Me(forced)) {
        int index = Color;
        if (ColorSchemes.Count() != 0) {
            if (index <= 0 || index >= ColorSchemes.Count()) {
                index = 0;
            }
            ColorScheme *scheme = ColorSchemes[index];
            if (scheme != 0) {
                BitFontClass *font = Font_For_Style(Style);
                Rect rect;
                rect.X = X;
                rect.Y = Y;
                rect.Width = PixWidth < 0 ? unknown_surface_83995c->Get_Width() : PixWidth;
                rect.Height = font->Height;
                int over = rect.X + rect.Width - unknown_surface_83995c->Get_Width();
                if (over > 0) {
                    rect.Width -= over;
                    if (!rect.Is_Valid()) {
                        return false;
                    }
                }
                RGBClass rgb = scheme->BaseColor.To_RGB();
                unsigned color = rgb.Red | (rgb.Green << 8) | (rgb.Blue << 16);
                int length = wcslen(Text);
                if (IsRevealed) {
                    DWORD now = timeGetTime();
                    int count = RevealCount;
                    if (RevealCount == 0) {
                        RevealCount = 1;
                    } else {
                        RevealCount += (now - RevealTime) >> 4;
                    }
                    if (count != RevealCount) {
                        RevealTime = now;
                    }
                    if (RevealCount <= length) {
                        Voc_Play(Rules->MessageCharTyped, 1.0f, 0x2000, 0);
                    }
                }
                OwnerDraw::unknown_draw_edit_text_600830(unknown_surface_83995c, rect, Text, length, font, color,
                                                        &Scroll, Has_Focus(), 0, 1, RevealCount);
                return true;
            }
        }
    }
    return false;
}


// void TextLabelClass::Set_Text(wchar_t const *text) (0x006F17E0, slot 33)
// Compiler-emitted from shared/text_label.hpp (the inline Set_Text) in this unit; no source of its own.


// void *TextLabelClass::`scalar deleting destructor'(unsigned int) (0x006F17F0)
// Compiler-emitted from shared/text_label.hpp in this unit; no source of its own.
