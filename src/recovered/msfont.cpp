// msfont.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/msfont.json carries the addresses and the rows that
// stay in their own files.


#include "shared/msfont.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"


// Dynamic initializer of unknown_empty_rect (0x005B3F20, _$E), the file-local
// empty Rect from shared/shape_file.hpp; this first source of the unit also
// defines the file's globals (0x00A71858-0x00A71877).
// Unresolved: original names of the globals.

int unknown_font_count_a71858;
MSFontSlot unknown_font_slots_a71860[3];


// MSFont::MSFont(bool sidebar) (0x005B3F40)
// Clears the colour (RGBClass's default constructor), loads the shared font
// data and counts the instance.

MSFont::MSFont(bool sidebar)
{
    Load(sidebar);
    unknown_font_count_a71858++;
}


// MSFont::~MSFont() (0x005B3F70)
// The last font releases the shared slots, the drawer and an owned shape.

#pragma auto_inline(off)
MSFont::~MSFont()
{
    if (--unknown_font_count_a71858 == 0) {
        for (int index = 0; index < 3; index++) {
            if (unknown_font_slots_a71860[index].unknown_00 != 0) {
                unknown_font_slots_a71860[index].unknown_00 = 0;
                unknown_font_slots_a71860[index].unknown_04 = false;
            }
        }
        delete Drawer;
        if (Shape != 0 && OwnsShape == true) {
            operator delete(Shape);
        }
        Shape = 0;
        OwnsShape = false;
    }
}
#pragma auto_inline(on)


// bool MSFont::Load(bool sidebar) (0x005B3FD0)
// First font only: FULLFNT3.SHP (mixfile cache, else loaded and owned), the
// SIDEFNT3/FULLFNT3 palette into 0x00838710, colour 0x44, the drawer.

bool MSFont::Load(bool sidebar)
{
    CCFileClass file;

    if (unknown_font_count_a71858 == 0) {
        OwnsShape = false;
        Shape = (ShapeFileStruct *)MixFileClass::Retrieve("FULLFNT3.SHP");
        if (Shape == 0) {
            file.Set_Name("FULLFNT3.SHP");
            Shape = (ShapeFileStruct *)Load_Alloc_Data(file);
            OwnsShape = true;
            Debug_Printf("MSFont: AllocLoaded FULLFNT3.SHP\n");
        }
        if (Shape == 0) {
            return false;
        }
        if (sidebar == true) {
            file.Set_Name("SIDEFNT3.PAL");
        } else {
            file.Set_Name("FULLFNT3.PAL");
        }
        unsigned char *palette = (unsigned char *)Load_Alloc_Data(file);
        if (palette == 0) {
            return false;
        }
        for (int index = 0; index < 256; index++) {
            unknown_palette_838710[index] = RGBClass(palette[index * 3] << 2, palette[index * 3 + 1] << 2, palette[index * 3 + 2] << 2);
        }
        unsigned char red = palette[0xCC] << 2;
        unsigned char blue = palette[0xCE] << 2;
        unsigned char green = palette[0xCD] << 2;
        Color.Red = red;
        Color.Green = green;
        Color.Blue = blue;
        Pixel = (((palette[0xCD] << 2) >> unknown_shift_852fcc) << unknown_shift_852fc8)
              | (((palette[0xCE] << 2) >> unknown_shift_852fc4) << unknown_shift_852fc0)
              | (((palette[0xCC] << 2) >> unknown_shift_852fbc) << unknown_shift_852fb8);
        operator delete(palette);
        Drawer = new ConvertClass(unknown_palette_838710, unknown_palette_838710, unknown_surface_839950, 1, false);
        if (Drawer == 0) {
            return false;
        }
        Width = Shape->Width;
        Height = Shape->Height;
    }
    unknown_check_406f30();
    return true;
}


// void MSFont::Text_Rect(unsigned short const *text, Rect &rect) (0x005B41F0)
// Bounding rectangle of a multi-line string: widest line plus Width, one
// Height per line; empty for a null or empty string.

void MSFont::Text_Rect(unsigned short const *text, Rect &rect)
{
    int widest = 0;
    int width = 0;
    if (text != 0 && wcslen(text) != 0) {
        int height = Height;
        do {
            width = 0;
            while (*text != 0 && *text != '\n') {
                width += Char_Width(*text++);
            }
            if (width > widest) {
                widest = width;
            }
            if (*text == '\n') {
                text++;
                height += Height;
            }
        } while (*text != 0);
        int total = Width + widest;
        rect.Height = height;
        rect.Width = total;
        rect.X = 0;
        rect.Y = 0;
        return;
    }
    rect.X = width;
    rect.Y = width;
    rect.Width = width;
    rect.Height = width;
}


// int MSFont::String_Width(unsigned short const *text) (0x005B42A0)
// Pixel width of the widest line of a multi-line string.

int MSFont::String_Width(unsigned short const *text)
{
    int widest = 0;
    if (text != 0) {
        do {
            int width = 0;
            while (*text != 0 && *text != '\n') {
                width += Char_Width(*text++);
            }
            if (*text == '\n') {
                text++;
            }
            if (width > widest) {
                widest = width;
            }
        } while (*text != 0);
    }
    return widest;
}


// int MSFont::Char_Width(unsigned short ch) (0x005B4300)
// Advance of one character: 8 for a space, 0 for control characters, else
// the glyph frame width plus one.

int MSFont::Char_Width(unsigned short ch)
{
    char c = (char)ch;
    CharToOemBuffA(&c, &c, 1);
    if (c == ' ') {
        return 8;
    }
    if ((unsigned char)c > ' ') {
        int index;
        if ((unsigned char)c - '!' <= 0) {
            index = 0;
        } else if ((unsigned char)c - '!' >= 0xD8) {
            index = 0xD8;
        } else {
            index = (unsigned char)c - '!';
        }
        Rect rect = Shape->Get_Frame_Rect(index * 3 + 2);
        return rect.Width + 1;
    }
    return 0;
}


// void MSFont::Print_Char(Surface *surface, unsigned short ch, int x, int y, int style, int unknown6) (0x005B43D0)
// Draws one glyph (frame (c - '!') * 3 + style) with its left edge at x;
// spaces and control characters draw nothing. Character 0xB0 is reported
// to the debug log first.

void MSFont::Print_Char(Surface *surface, unsigned short ch, int x, int y, int style, int unknown6)
{
    char c = (char)ch;
    if (c == 0xB0) {
        Debug_Printf("Denzil!\n");
    }
    CharToOemBuffA(&c, &c, 1);
    if ((unsigned char)c > ' ') {
        int index;
        if ((unsigned char)c - '!' <= 0) {
            index = 0;
        } else if ((unsigned char)c - '!' >= 0xD8) {
            index = 0xD8;
        } else {
            index = (unsigned char)c - '!';
        }
        int frame = index * 3;
        Point2D offset(0, 0);
        Rect rect = Shape->Get_Frame_Rect(frame + 2);
        Point2D point(x - rect.X, y);
        CC_Draw_Shape(surface, Drawer, Shape, frame + style, point, surface->Get_Rect(),
                      0x400, 0, 0, 0, 1000, 0, 0, offset);
    }
}


// void MSFont::Print(Surface *surface, unsigned short const *text, int x, int y, int style) (0x005B44F0)
// Draws a string glyph by glyph; '\n' returns to x on the next line.

void MSFont::Print(Surface *surface, unsigned short const *text, int x, int y, int style)
{
    int xpos = x;
    while (*text != 0) {
        if (*text == '\n') {
            xpos = x;
            y += Height;
        } else {
            if (*text > ' ') {
                char c = (char)*text;
                char oem;
                CharToOemBuffA(&c, &oem, 1);
                int index = (0 >= oem - 33u) ? 0 : oem - 33u;
                index = (216 <= index) ? 216 : index;
                int frame = index * 3;
                Point2D offset(0, 0);
                Rect rect = Shape->Get_Frame_Rect(frame + 2);
                Point2D point(xpos - rect.X, y);
                CC_Draw_Shape(surface, Drawer, Shape, frame + style, point, surface->Get_Rect(),
                              0x400, 0, 0, 0, 1000, 0, 0, offset);
            }
            xpos += Char_Width(*text);
        }
        text++;
    }
}


// void *MSFont::`scalar deleting destructor'(unsigned int flags) (0x005B4670)
// Compiler-generated from the virtual destructor.
