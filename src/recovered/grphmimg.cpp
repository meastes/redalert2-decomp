// grphmimg.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/grphmimg.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ini_class.hpp"
#include "shared/graphic_menu.hpp"
#include "shared/wstring.hpp"
#include "shared/cc_file_class.hpp"


// Header-static dynamic initializer of the GraphicMenuImageItem unit
// (0x004DF7F0, _$E): shape_file.hpp's empty rect (0x00A40E48), emitted
// through shared/graphic_menu.hpp by the drafts of this plan; no source of its own.


// GraphicMenuItem *__fastcall Create_Image_Item(Wstring const &section, INIClass &ini, void *unknown_a,
// Point2D const *offset) (0x004DF810): a GraphicMenuImageItem from a menu INI section (ID, Origin,
// ActiveRect, Image, Highlighted, Disabled, HighlightSound, SelectVQ); none without an ID. Origin
// and the active rectangle are moved by offset. Names are hypotheses.

extern char unknown_string_83c55c[];

GraphicMenuItem *__fastcall Create_Image_Item(Wstring const &section, INIClass &ini, void *unknown_a, Point2D const *offset)
{
    int id = ini.Get_Int(section, "ID", -1);
    if (id == -1) {
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
    Wstring image;
    Wstring highlighted;
    Wstring disabled;
    ini.Get_String(section, "Image", unknown_string_83c55c, image);
    ini.Get_String(section, "Highlighted", unknown_string_83c55c, highlighted);
    ini.Get_String(section, "Disabled", unknown_string_83c55c, disabled);
    Wstring sound;
    ini.Get_String(section, "HighlightSound", unknown_string_83c55c, sound);
    Wstring vq;
    ini.Get_String(section, "SelectVQ", unknown_string_83c55c, vq);
    return new GraphicMenuImageItem(id, unknown_a, origin, area, image, highlighted, disabled, sound, vq);
}


// GraphicMenuImageItem::GraphicMenuImageItem(...) (0x004DFB30)
// Takes the engine and the active area, the hover sound and the select movie name, and
// adds the PCX images that are named (highlighted and disabled ones start hidden).

GraphicMenuImageItem::GraphicMenuImageItem(int id, void *unknown_list, Point2D const &origin, INIRect const &area,
                                           Wstring const &image, Wstring const &highlighted, Wstring const &disabled,
                                           Wstring const &highlight_sound, Wstring const &select_vq)
    : GraphicMenuItem(id), Engine((MSEngine *)unknown_list), Area(*(Rect const *)&area),
      unknown_image_24(0), unknown_image_28(0), unknown_image_2c(0), unknown_sound_30(0)
{
    MSEngine *engine = (MSEngine *)unknown_list;
    unknown_sound_30 = highlight_sound.Length() > 0 ? new MSSfxEntry("HighlightSound", highlight_sound) : 0;
    unknown_name_34 = select_vq;

    if (highlighted.Length() > 0) {
        unknown_image_28 = new MSPCXAnim(highlighted, &engine->Anims, origin, true);
        if (unknown_image_28 != 0) {
            unknown_image_28->Set_Unknown_0C(false);
            engine->Add_Anim(unknown_image_28);
        }
    }
    if (image.Length() > 0) {
        unknown_image_24 = new MSPCXAnim(image, &engine->Anims, origin, true);
        if (unknown_image_24 != 0) {
            engine->Add_Anim(unknown_image_24);
        }
    }
    if (disabled.Length() > 0) {
        unknown_image_2c = new MSPCXAnim(disabled, &engine->Anims, origin, true);
        if (unknown_image_2c != 0) {
            unknown_image_2c->Set_Unknown_0C(false);
            engine->Add_Anim(unknown_image_2c);
        }
    }
}


// GraphicMenuImageItem::~GraphicMenuImageItem() -- 0x004DFCD0
// Deletes the hover sound entry and destroys the name; the images belong
// to the screen engine and are not deleted here.

GraphicMenuImageItem::~GraphicMenuImageItem()
{
    delete unknown_sound_30;
}


// 0x004dfd10: function not yet identified
//
// Owner: GraphicMenuImageItem, virtual slot 2 (vtable +0x8) (from RTTI/vtables).
// Body: 66 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __thiscall Is_Mouse_Over_004dfd10(int param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_004dfd10 {

struct Owner_004dfd10 { unsigned int Is_Mouse_Over_004dfd10(int *mouse); };

unsigned int Owner_004dfd10::Is_Mouse_Over_004dfd10(int *mouse)
{
    int item_address = (int)this;
    if (*(char *)(item_address + 9) != '\0') {
        if ((*mouse >= *(int *)(item_address + 0x14)) &&
              (*mouse < *(int *)(item_address + 0x1c) + *(int *)(item_address + 0x14))) {
            if ((mouse[1] >= *(int *)(item_address + 0x18)) &&
                  (mouse[1] < *(int *)(item_address + 0x20) + *(int *)(item_address + 0x18))) {
                return 1;
            }
        }
    }
    return 0;
}

}  // namespace seed_004dfd10


// void GraphicMenuImageItem::Enabled_Changed(bool enabled) (0x004DFD60)
// Switches the three images (the first shows when visible and disabled, the second when
// visible and enabled, the third when hidden), tells the engine's animations about the
// item's area, repaints and plays the hover sound when the item became enabled.

void GraphicMenuImageItem::Enabled_Changed(bool enabled)
{
    if (unknown_image_24 != 0) {
        unknown_image_24->Set_Unknown_0C(Visible && !enabled);
    }
    if (unknown_image_28 != 0) {
        unknown_image_28->Set_Unknown_0C(Visible && enabled);
    }
    if (unknown_image_2c != 0) {
        unknown_image_2c->Set_Unknown_0C(!Visible);
    }
    Engine->Broadcast((int)&Area);
    Engine->Redraw();
    if (enabled && unknown_sound_30 != 0) {
        unknown_sound_30->Play();
    }
}


// void GraphicMenuImageItem::Visible_Changed(bool visible) (0x004DFDF0)
// Switches the three images (the first shows when visible and disabled, the second when
// visible and enabled, the third when hidden), tells the engine's animations about the
// item's area and repaints.

void GraphicMenuImageItem::Visible_Changed(bool visible)
{
    if (unknown_image_24 != 0) {
        unknown_image_24->Set_Unknown_0C(visible && !Enabled);
    }
    if (unknown_image_28 != 0) {
        unknown_image_28->Set_Unknown_0C(visible && Enabled);
    }
    if (unknown_image_2c != 0) {
        unknown_image_2c->Set_Unknown_0C(!visible);
    }
    Engine->Broadcast((int)&Area);
    Engine->Redraw();
}


// void GraphicMenuImageItem::Play_Sound(int unknown) (0x004DFE70)
// Plays the hover sound, then, when the item's movie file exists, plays that movie
// (a 640x400 VQA) on the background surface and waits for it (up to 300000 ms).
// The argument is the screen engine (MSEngine *) whose animation list the movie joins.

void GraphicMenuImageItem::Play_Sound(int unknown)
{
    MSEngine *engine = (MSEngine *)unknown;
    GraphicMenuItem::Play_Sound(unknown);
    if (CCFileClass(unknown_name_34).Is_Available()) {
        MSVQAnim *movie = new MSVQAnim(unknown_name_34, unknown_surface_839958, &engine->Anims, true);
        if (movie != 0) {
            engine->Wait_For(movie, 300000);
        }
    }
}


// GraphicMenuImageItem scalar deleting destructor (0x004DFF10, ??_G): emitted
// with the vtable by the destructor's definition; the destructor is expanded inline.
