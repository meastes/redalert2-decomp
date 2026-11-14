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


// EditClass::EditClass(unsigned id, wchar_t *text, int max_len, unsigned flags, int x, int y, int w, int h, int editflags) (0x004B2970)
// A width or height of -1 fits the text (or max_len wide characters) in the
// style's font.

EditClass::EditClass(unsigned id, wchar_t *text, int max_len, unsigned flags, int x, int y, int w, int h,
                     int editflags) :
    ControlClass(id, x, y, w, h, LEFTPRESS, false),
    TextFlags(flags & ~KEYBOARD)
{
    EditFlags = editflags;
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


// EditClass::~EditClass() (0x004B2A60)

EditClass::~EditClass()
{
    if (Has_Focus()) {
        Clear_Focus();
    }
}


// void EditClass::Set_Text(wchar_t *text, int max_len) (0x004B2A90, slot 34)

void EditClass::Set_Text(wchar_t *text, int max_len)
{
    String = text;
    MaxLength = max_len - 1;
    Length = wcslen(String);
    Flag_To_Redraw();
}


// int EditClass::Draw_Me(int forced) (0x004B2AC0, slot 27)
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


// void EditClass::Draw_Background() (0x004B2C50, slot 36)
// An outlined box (style 4), filled.

void EditClass::Draw_Background()
{
    Draw_Box(Rect(X, Y, Width, Height), 4, true);
}


// void EditClass::Draw_Text(wchar_t const *text) (0x004B2C90, slot 37)
// Prints the text inside the box (bright, 0x2000, while it has the focus)
// in colour scheme Color, clipped to the inner width; with the focus and
// room for another character, an underscore cursor follows the text.

void EditClass::Draw_Text(wchar_t const *text)
{
    unsigned flags = Has_Focus() ? 0x2000 : 0;
    Point2D point(X + 1, Y + 1);
    Fancy_Clip_Text_Print(text, unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &point,
                          ColorSchemes[Color], 0, TextFlags | flags, Width - 2, 0);

    BitFontClass *font = Font_For_Style(TextFlags);
    if (Has_Focus() && wcslen(text) < (unsigned)MaxLength
        && font->String_Pixel_Width(text, 0) + font->String_Pixel_Width(L"_", 0) < Width - 2) {
        point.X = X + 1 + font->String_Pixel_Width(text, 0);
        point.Y = Y + 1;
        Fancy_Clip_Text_Print(L"_", unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &point,
                              ColorSchemes[Color], 0, TextFlags | flags, (unsigned)-1, 0);
    }
}


// bool EditClass::Handle_Key(int ascii) (0x004B2DD0, slot 38)
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


// void EditClass::Set_Focus() (0x004B2F20, slot 20)
// Puts the cursor at the end of the text.

void EditClass::Set_Focus()
{
    Length = 0;
    if (String != 0) {
        Length = wcslen(String);
    }
    GadgetClass::Set_Focus();
}
