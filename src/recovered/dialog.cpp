// dialog.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/dialog.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/text_print.hpp"


// Header-static dynamic initializer of the unit at 0x00497340: the empty
// rectangle of shared/shape_file.hpp (0x00852490), the unit's first function.


// Rect Text_Rect(wchar_t const *text, int x, int y, unsigned flags, int padx, int pady) (0x00497360)
// The box text would occupy at x, y in the current font (flag 0x100 centres
// it on x, 0x200 right-aligns it), grown by padx/pady on each side.

Rect __fastcall Text_Rect(wchar_t const *text, int x, int y, unsigned flags, int padx, int pady)
{
    BitFontClass *font = BitFont;
    int width = font->String_Pixel_Width(text, 0);
    int height = font->Height;
    int left = x;
    if (flags & 0x100) {
        left = x - width / 2;
    } else if (flags & 0x200) {
        left = x - width;
    }
    return Rect(left - padx, y - pady, width + padx * 2, height + pady * 2);
}


// void Draw_Box(Rect const &rect, int style, bool fill) (0x004973D0)
// Draws a GUI box in the current box colour scheme on the GUI surface:
// optionally filled, then by style a sunken or raised bevel (styles 0-3:
// shadow lines right and bottom, highlight lines top and left, corner
// pixels), an outline (4) or an outline inset by one pixel (5). Styles
// follow Westwood's BOXSTYLE_DOWN, RAISED, DIS_DOWN, DIS_RAISED, BOX,
// BORDER (hypothesis); 2 and 3 use fixed palette indices.

struct BoxStyleType {
    int Filler;
    int Shadow;
    int Highlight;
    int Corner;
};

void __fastcall Draw_Box(Rect const &rect, int style, bool fill)
{
    ColorScheme *scheme = ColorSchemes[Gadget_Color_Scheme()];
    BoxStyleType const styles[6] = {
        {scheme->unknown_color_0x320, scheme->unknown_color_0x328, scheme->unknown_color_0x31c, scheme->unknown_color_0x324},
        {scheme->unknown_color_0x320, scheme->unknown_color_0x31c, scheme->unknown_color_0x328, scheme->unknown_color_0x324},
        {13, 15, 12, 13},
        {13, 12, 14, 13},
        {scheme->unknown_color_0x320, scheme->unknown_color_0x32c, scheme->unknown_color_0x32c, scheme->unknown_color_0x320},
        {scheme->unknown_color_0x320, scheme->unknown_color_0x32c, scheme->unknown_color_0x32c, scheme->unknown_color_0x320},
    };
    BoxStyleType const &colors = styles[style];

    if (fill) {
        unknown_surface_83995c->Fill_Rect(rect, scheme->Converter->Convert(colors.Filler));
    }

    switch (style) {
    case 4:
        unknown_surface_83995c->Draw_Rect(rect, scheme->Converter->Convert(colors.Highlight));
        break;

    case 5:
        unknown_surface_83995c->Draw_Rect(Rect(rect.X + 1, rect.Y + 1, rect.Width - 1, rect.Height - 1),
                                          scheme->Converter->Convert(colors.Highlight));
        break;

    default:
        unknown_surface_83995c->Draw_Line(Point2D(rect.X, rect.Y + rect.Height),
                                          Point2D(rect.X + rect.Width, rect.Y + rect.Height),
                                          scheme->Converter->Convert(colors.Shadow));
        unknown_surface_83995c->Draw_Line(Point2D(rect.X + rect.Width, rect.Y),
                                          Point2D(rect.X + rect.Width, rect.Y + rect.Height),
                                          scheme->Converter->Convert(colors.Shadow));
        unknown_surface_83995c->Draw_Line(Point2D(rect.X, rect.Y),
                                          Point2D(rect.X + rect.Width, rect.Y),
                                          scheme->Converter->Convert(colors.Highlight));
        unknown_surface_83995c->Draw_Line(Point2D(rect.X, rect.Y),
                                          Point2D(rect.X, rect.Y + rect.Height),
                                          scheme->Converter->Convert(colors.Highlight));
        unknown_surface_83995c->Put_Pixel(Point2D(rect.X, rect.Y + rect.Height),
                                          scheme->Converter->Convert(colors.Corner));
        unknown_surface_83995c->Put_Pixel(Point2D(rect.X + rect.Width, rect.Y),
                                          scheme->Converter->Convert(colors.Corner));
        break;
    }
}


// int Text_Lines(wchar_t const *text, int style, int columns, int *width, int *height) (0x004977D0)
// Measures text wrapped to the pixel width of `columns` 'W' characters in
// the style's font: the text area goes to width/height, the line count is
// returned (0 for no text or no font).

int __fastcall Text_Lines(wchar_t const *text, int style, int columns, int *width, int *height)
{
    *width = 0;
    *height = 0;
    BitFontClass *font = Font_For_Style(style);
    if (text != 0 && font != 0) {
        font->Get_Text_Area(text, width, height, font->Char_Pixel_Width(L'W') * columns);
        return *height / font->Height;
    }
    return 0;
}


// Point2D Simple_Text_Print(wchar_t const *text, Surface *surface, Rect const *clip, Point2D *xy, unsigned fore, unsigned back, unsigned flags, int unknown8) (0x00497830)
// Prints text at xy inside clip in colour fore (flag 0x100 centres, 0x200
// right-aligns; 0x8000 puts a translucent black box behind it, 0x400 an
// opaque one); returns where it was drawn.

Point2D __fastcall Simple_Text_Print(wchar_t const *text, Surface *surface, Rect const *clip, Point2D *xy,
                                     unsigned fore, unsigned back, unsigned flags, int unknown8)
{
    BitFontClass *font = BitFont;
    int x = clip->X + xy->X;
    int y = clip->Y + xy->Y;
    int width;
    int height;
    font->Get_Text_Area(text, &width, &height, clip->Width);

    if (flags & 0x100) {
        x -= width / 2;
    } else if (flags & 0x200) {
        x -= width;
    }

    if (flags & 0x8000) {
        RGBClass shadow(0, 0, 0);
        Rect box(x - 1, y - 1, width + 2, height + 2);
        surface->Fill_Rect_Trans(box, shadow, 60);
    } else if (flags & 0x400) {
        surface->Fill_Rect(Rect(x - 1, y - 1, width + 2, height + 2), 0);
    }

    tagRECT box;
    box.left = clip->X;
    box.top = clip->Y;
    box.right = clip->X + clip->Width;
    box.bottom = clip->Y + clip->Height;
    font->Set_Clipping(1);
    font->Set_Clip_Box(&box);
    font->Set_Color(fore);
    BitText->Print(font, surface, text, x, y, 0, 0);
    return Point2D(x, y);
}


// Point2D Fancy_Text_Print(wchar_t const *text, Surface *surface, Rect const *clip, Point2D *xy, ColorScheme *scheme, unsigned back, unsigned flags, int unknown8) (0x00497990)
// Simple_Text_Print in the base colour of a colour scheme.

Point2D __fastcall Fancy_Text_Print(wchar_t const *text, Surface *surface, Rect const *clip, Point2D *xy,
                                    ColorScheme *scheme, unsigned back, unsigned flags, int unknown8)
{
    RGBClass color = scheme->BaseColor.To_RGB();
    unsigned pixel = ((color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
                   | ((color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
                   | ((color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
    return Simple_Text_Print(text, surface, clip, xy, pixel, back, flags, unknown8);
}


// BitFontClass *Font_For_Style(int style) (0x00497A50)
// The current bit font; the style is not consulted (one font).

BitFontClass *__fastcall Font_For_Style(int style)
{
    return BitFont;
}


// Point2D Plain_Text_Print(wchar_t const *format, Surface *surface, Rect const *clip, Point2D *xy, unsigned fore, unsigned back, unsigned flags, ...) (0x00497A60)
// Formats the text (vswprintf) and prints it with Simple_Text_Print.

Point2D __cdecl Plain_Text_Print(wchar_t const *format, Surface *surface, Rect const *clip, Point2D *xy,
                                 unsigned fore, unsigned back, unsigned flags, ...)
{
    wchar_t buffer[512];
    if (format != 0) {
        vswprintf(buffer, format, (char *)(&flags + 1));
        return Simple_Text_Print(buffer, surface, clip, xy, fore, back, flags, 1);
    }
    return Simple_Text_Print(0, surface, clip, xy, fore, back, flags, 1);
}


// Point2D Fancy_Text_Printf(wchar_t const *format, Surface *surface, Rect const *clip, Point2D *xy, ColorScheme *scheme, unsigned back, unsigned flags, ...) (0x00497B40)
// Formats the text (vswprintf) and prints it in the scheme's base colour.

Point2D __cdecl Fancy_Text_Printf(wchar_t const *format, Surface *surface, Rect const *clip, Point2D *xy,
                                  ColorScheme *scheme, unsigned back, unsigned flags, ...)
{
    wchar_t buffer[512];
    if (format != 0) {
        vswprintf(buffer, format, (char *)(&flags + 1));
        return Scheme_Text_Print(buffer, surface, clip, xy, scheme, back, flags, 1);
    }
    return Scheme_Text_Print(0, surface, clip, xy, scheme, back, flags, 1);
}


// Point2D Conquer_Clip_Text_Print(wchar_t const *text, Surface *surface, Rect const *clip, Point2D *xy, unsigned fore, unsigned back, unsigned flags, unsigned width, int const *tabs) (0x00497CE0)
// Prints tab-separated text, clipped to `width` pixels from xy: each field
// is printed with Simple_Text_Print; a tab advances to the next stop in
// `tabs` (or the next multiple-of-50 stop when there is no table). A field
// that does not fit is cut at the last whole character and ends printing.
// Returns where the last print ended (xy when nothing was printed).

Point2D __fastcall Conquer_Clip_Text_Print(wchar_t const *text, Surface *surface, Rect const *clip,
                                           Point2D *xy, unsigned fore, unsigned back, unsigned flags,
                                           unsigned width, int const *tabs)
{
    Point2D result = *xy;
    if (text != 0) {
        wchar_t buffer[512];
        wcscpy(buffer, text);
        BitFontClass *font = Font_For_Style(flags);
        wchar_t *source = buffer;
        unsigned offset = 0;
        int processing = 1;
        while (processing && offset < width) {
            wchar_t *tab = wcschr(source, L'\t');
            if (tab != 0) {
                *tab = 0;
            }
            if (*source != 0) {
                unsigned pixels = 0;
                wchar_t *p = source;
                do {
                    pixels += font->Char_Pixel_Width(*p++);
                } while (*p != 0 && offset + pixels < width);
                if (offset + pixels >= width) {
                    p--;
                    pixels -= font->Char_Pixel_Width(*p);
                    *p = 0;
                    processing = 0;
                }
                Point2D pos(xy->X + offset, xy->Y);
                result = Simple_Text_Print(source, surface, clip, &pos, fore, back, flags, 1);
                offset += pixels;
            }
            if (tab == 0) {
                break;
            }
            if (tabs != 0) {
                while (offset > (unsigned)*tabs) {
                    tabs++;
                }
                offset = *tabs;
            } else {
                offset = (offset + 1) * 50;
            }
            source = tab + 1;
        }
    }
    return result;
}


// Point2D Fancy_Clip_Text_Print(wchar_t const *text, Surface *surface, Rect const *clip, Point2D *xy, ColorScheme *scheme, unsigned back, unsigned flags, unsigned width, int const *tabs) (0x00497E80)
// Conquer_Clip_Text_Print in the base colour of a colour scheme.

Point2D __fastcall Fancy_Clip_Text_Print(wchar_t const *text, Surface *surface, Rect const *clip,
                                         Point2D *xy, ColorScheme *scheme, unsigned back, unsigned flags,
                                         unsigned width, int const *tabs)
{
    RGBClass color = scheme->BaseColor.To_RGB();
    unsigned pixel = ((color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
                   | ((color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
                   | ((color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
    return Conquer_Clip_Text_Print(text, surface, clip, xy, pixel, back, flags, width, tabs);
}


// Point2D unknown_scheme_text_print_497f40(wchar_t const *text, Surface *surface, Rect const *clip, Point2D *xy, unsigned fore, unsigned back, unsigned flags, int scheme, int unknown8) (0x00497f40)
// Simple_Text_Print in the base colour of colour scheme `scheme` (an index
// into ColorSchemes), or in `fore` when scheme is -1.

Point2D __fastcall unknown_scheme_text_print_497f40(wchar_t const *text, Surface *surface, Rect const *clip,
                                                    Point2D *xy, unsigned fore, unsigned back, unsigned flags,
                                                    int scheme, int unknown8)
{
    if (scheme == -1) {
        return Simple_Text_Print(text, surface, clip, xy, fore, back, flags, unknown8);
    }
    return Scheme_Text_Print(text, surface, clip, xy, ColorSchemes[scheme], back, flags, unknown8);
}


// Point2D unknown_scheme_text_print_498050(wchar_t const *text, Surface *surface, Rect const *clip, Point2D *xy, unsigned fore, unsigned back, unsigned flags, int scheme, int unknown8) (0x00498050)
// Simple_Text_Print in the base colour of colour scheme `scheme` (an index
// into ColorSchemes), or in `fore` when scheme is -1.

Point2D __fastcall unknown_scheme_text_print_498050(wchar_t const *text, Surface *surface, Rect const *clip,
                                                    Point2D *xy, unsigned fore, unsigned back, unsigned flags,
                                                    int scheme, int unknown8)
{
    if (scheme == -1) {
        return Simple_Text_Print(text, surface, clip, xy, fore, back, flags, unknown8);
    }
    return Scheme_Text_Print(text, surface, clip, xy, ColorSchemes[scheme], back, flags, unknown8);
}


// unsigned char *unknown_fill_buffer_498160(int value) (0x00498160)
// Clears the unit's 16-byte buffer, fills its last five bytes with value
// and returns it.

unsigned char *__fastcall unknown_fill_buffer_498160(int value)
{
    memset(unknown_buffer_852480, 0, 16);
    memset(unknown_buffer_852480 + 11, value, 5);
    return unknown_buffer_852480;
}
