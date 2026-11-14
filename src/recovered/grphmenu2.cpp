// grphmenu2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/grphmenu2.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ini_class.hpp"
#include "shared/graphic_menu.hpp"


// Header-static dynamic initializer of the GraphicMenuAnimItem unit
// (0x004DF330, _$E): shape_file.hpp's empty rect (0x00A40E38), emitted
// through shared/graphic_menu.hpp by the drafts of this plan; no source of its own.


// GraphicMenuItem *__fastcall Create_Anim_Item(Wstring const &section, INIClass &ini, void *unknown_a,
// Point2D const *offset) (0x004DF350): a GraphicMenuAnimItem from a menu INI section (ID, Image,
// Palette, Origin, ActiveRect, Loop, Rate); none without an ID, image or palette. Origin and the
// active rectangle are moved by offset. Names are hypotheses. The literals convert implicitly to
// the Wstring overloads; Loop goes through the char const * Get_Bool (Wstring 0x0076E730).

extern char unknown_string_83c55c[];

GraphicMenuItem *__fastcall Create_Anim_Item(Wstring const &section, INIClass &ini, void *unknown_a, Point2D const *offset)
{
    int id = ini.Get_Int(section, "ID", -1);
    if (id == -1) {
        return 0;
    }
    Wstring image;
    ini.Get_String(section, "Image", unknown_string_83c55c, image);
    if (!image.Length()) {
        return 0;
    }
    Wstring palette;
    ini.Get_String(section, "Palette", unknown_string_83c55c, palette);
    if (!palette.Length()) {
        return 0;
    }
    Point2D origin(0, 0);
    origin = ini.Get_Point(section, "Origin", origin);
    origin.X += offset->X;
    origin.Y += offset->Y;
    INIRect area;
    area.X = 0;
    area.Y = 0;
    area.Width = 0;
    area.Height = 0;
    area = ini.Get_Rect(section, "ActiveRect", area);
    area.X += offset->X;
    area.Y += offset->Y;
    bool loop = ini.Get_Bool(section.Peek_Buffer(), "Loop", true);
    int rate = ini.Get_Int(section, "Rate", 5);
    return new GraphicMenuAnimItem(id, unknown_a, origin, area, image, palette, rate, loop);
}


// GraphicMenuAnimItem::~GraphicMenuAnimItem() -- 0x004DF740
// Deletes the item's animation (virtual destructor), then the base part.

GraphicMenuAnimItem::~GraphicMenuAnimItem()
{
    delete Anim;
}


// 0x004df760: function not yet identified
//
// Owner: GraphicMenuAnimItem, virtual slot 2 (vtable +0x8) (from RTTI/vtables).
// Body: 66 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __thiscall Is_Mouse_Over_004df760(int param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_004df760 {

struct Owner_004df760 { unsigned int Is_Mouse_Over_004df760(int *mouse); };

unsigned int Owner_004df760::Is_Mouse_Over_004df760(int *mouse)
{
    int item_address = (int)this;
    if (*(char *)(item_address + 9) != '\0') {
        if ((*mouse >= *(int *)(item_address + 0x10)) &&
              (*mouse < *(int *)(item_address + 0x18) + *(int *)(item_address + 0x10))) {
            if ((mouse[1] >= *(int *)(item_address + 0x14)) &&
                  (mouse[1] < *(int *)(item_address + 0x1c) + *(int *)(item_address + 0x14))) {
                return 1;
            }
        }
    }
    return 0;
}

}  // namespace seed_004df760


// GraphicMenuAnimItem scalar deleting destructor (0x004DF7B0, ??_G): emitted
// with the vtable by the destructor's definition; the destructor is expanded inline.
