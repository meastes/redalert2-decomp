// colrlist.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/colrlist.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/color_list.hpp"


// Header-static dynamic initializer of the unit at 0x0047D7A0: the empty
// rectangle of shared/shape_file.hpp (0x00850C68), the unit's first function.


// ColorListClass::ColorListClass(int id, int x, int y, int w, int h, unsigned flags, void const *up, void const *down) (0x0047D7C0)
// An empty colour list with the default selection style.

ColorListClass::ColorListClass(int id, int x, int y, int w, int h, unsigned flags, void const *up, void const *down) :
    ListClass(id, x, y, w, h, flags, up, down),
    Style(1),
    SelectColor(0)
{
}


// ColorListClass::~ColorListClass() (0x0047D850)

ColorListClass::~ColorListClass()
{
    Colors.Clear();
    SelectColor = 0;
}


// int ColorListClass::Add_Item(wchar_t *text, int color) (0x0047D8B0, slot 51)
// Adds an entry drawn in a colour scheme.

int ColorListClass::Add_Item(wchar_t *text, int color)
{
    Colors.Add(color);
    return ListClass::Add_Item(text);
}


// void ColorListClass::Remove_Item(wchar_t *text) (0x0047D910, slot 43)
// Removes the entry with this text and its colour.

void ColorListClass::Remove_Item(wchar_t *text)
{
    int index = List.ID(text);
    if (index != -1) {
        Colors.Delete(index);
        ListClass::Remove_Item(text);
    }
}


// void ColorListClass::Remove_Item(int index) (0x0047D970, slot 42)
// Removes an entry and its colour.

void ColorListClass::Remove_Item(int index)
{
    if (index != -1) {
        Colors.Delete(index);
        ListClass::Remove_Item(index);
    }
}


// void ColorListClass::Set_Selected_Style(int style, int color) (0x0047D9C0, slot 52)

void ColorListClass::Set_Selected_Style(int style, int color)
{
    Style = style;
    SelectColor = color;
}


// void ColorListClass::Draw_Entry(int index, int x, int y, int width, int selected) (0x0047D9E0, slot 50)
// An unselected entry prints in its own colour scheme; a selected one in the selection style:
// 0 its own scheme, 1 the selection scheme (bright for the 6-point fonts), 2 a box in the
// selection colour, 3 a bar under the text, 4 inverted (a bar in the entry's colour).
// The selection scheme is SelectColor, or the entry's index when none is set (Westwood's
// COLRLIST.CPP Draw_Entry).

void ColorListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
    int color;

    if (!selected) {
        Fancy_Clip_Text_Print(List[index], unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &Point2D(x, y),
                              ColorSchemes[Colors[index]], 0, TextFlags, width, Tabs);
        return;
    }

    if (SelectColor == 0) {
        color = index;
    } else {
        color = SelectColor;
    }

    switch (Style) {
        case 0:
            Fancy_Clip_Text_Print(List[index], unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &Point2D(x, y),
                                  ColorSchemes[Colors[index]], 0, TextFlags, width, Tabs);
            break;

        case 1:
            if (TextFlags & 6) {
                Fancy_Clip_Text_Print(List[index], unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &Point2D(x, y),
                                      ColorSchemes[color], 0, TextFlags | 0x2000, width, Tabs);
            } else {
                Fancy_Clip_Text_Print(List[index], unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &Point2D(x, y),
                                      ColorSchemes[color], 0, TextFlags, width, Tabs);
            }
            break;

        case 2:
            unknown_surface_83995c->Draw_Rect(Rect(x, y, x + width - 2, y + LineHeight - 2),
                                              unknown_drawer_8323c4->Convert(ColorSchemes[color]->unknown_0x314));
            Fancy_Clip_Text_Print(List[index], unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &Point2D(x, y),
                                  ColorSchemes[Colors[index]], 0, TextFlags, width, Tabs);
            break;

        case 3:
            if (TextFlags & 6) {
                unknown_surface_83995c->Fill_Rect(Rect(x, y, width, LineHeight),
                                                  unknown_drawer_8323c4->Convert(ColorSchemes[color]->unknown_0x314));
                Fancy_Clip_Text_Print(List[index], unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &Point2D(x, y),
                                      ColorSchemes[Colors[index]], 0, TextFlags | 0x2000, width, Tabs);
            } else {
                unknown_surface_83995c->Fill_Rect(Rect(x, y, width - 1, LineHeight - 1),
                                                  unknown_drawer_8323c4->Convert(ColorSchemes[color]->unknown_0x314));
                Fancy_Clip_Text_Print(List[index], unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &Point2D(x, y),
                                      ColorSchemes[Colors[index]], 0, TextFlags, width, Tabs);
            }
            break;

        case 4:
            unknown_surface_83995c->Fill_Rect(Rect(x, y, width, LineHeight),
                                              unknown_drawer_8323c4->Convert(ColorSchemes[Colors[index]]->unknown_0x314));
            break;
    }
}


// int ColorListClass::Add_Item(wchar_t *text) (0x0047DF40)
// Compiler-emitted from shared/color_list.hpp in this unit; no source of its own.


// void *ColorListClass::`scalar deleting destructor'(unsigned int) (0x0047DF60)
// Compiler-emitted from shared/color_list.hpp in this unit; no source of its own.
