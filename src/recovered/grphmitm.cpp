// grphmitm.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/grphmitm.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ini_class.hpp"
#include "shared/graphic_menu.hpp"


// Header-static dynamic initializer of the GraphicMenuItem unit (0x004DFF60,
// _$E): shape_file.hpp's empty rect (0x00A40E58), emitted through
// shared/graphic_menu.hpp by the drafts of this plan; no source of its own.


// GraphicMenuItem *__fastcall Create_Menu_Item(Wstring const &section, INIClass &ini, void *unknown_a,
// Point2D const *offset) (0x004DFF80): the menu item a section's Type= names (Image 0x004DF810, Shortcut
// 0x004E0250, Anim 0x004DF350; none otherwise), with its SelectSound= when given. Names are
// hypotheses. unknown_a and offset pass through to the item factories (the anim item constructor
// hands its animation to unknown_a, 0x005B2D60; offset is added to the item's origin and area).

extern char unknown_string_83c55c[];
GraphicMenuItem *__fastcall Create_Image_Item(Wstring const &section, INIClass &ini, void *unknown_a, Point2D const *offset);
GraphicMenuItem *__fastcall Create_Anim_Item(Wstring const &section, INIClass &ini, void *unknown_a, Point2D const *offset);
GraphicMenuItem *__fastcall Create_Shortcut_Item(Wstring const &section, INIClass &ini);

GraphicMenuItem *__fastcall Create_Menu_Item(Wstring const &section, INIClass &ini, void *unknown_a, Point2D const *offset)
{
    Wstring type;
    ini.Get_String(section, "Type", unknown_string_83c55c, type);
    GraphicMenuItem *item = 0;
    if (type == "Image") {
        item = Create_Image_Item(section, ini, unknown_a, offset);
    } else if (type == "Shortcut") {
        item = Create_Shortcut_Item(section, ini);
    } else if (type == "Anim") {
        item = Create_Anim_Item(section, ini, unknown_a, offset);
    }
    if (item) {
        Wstring sound;
        if (ini.Get_String(section, "SelectSound", unknown_string_83c55c, sound) > 0) {
            item->Sound = new MSSfxEntry("SelectSound", sound);
        }
    }
    return item;
}


// GraphicMenuItem::GraphicMenuItem(int id) -- 0x004E0120
// A disabled, visible item without a sound.

GraphicMenuItem::GraphicMenuItem(int id) : ID(id), Enabled(false), Visible(true), Sound(0)
{
}


// GraphicMenuItem::~GraphicMenuItem() -- 0x004E0140
// Deletes the item's sound entry (MSSfxEntry::~MSSfxEntry 0x005B16C0).

GraphicMenuItem::~GraphicMenuItem()
{
    delete Sound;
}


// void GraphicMenuItem::Set_Enabled(bool enabled) -- 0x004E0160
// Stores the flag and notifies the item (virtual Enabled_Changed) when it changes.

void GraphicMenuItem::Set_Enabled(bool enabled)
{
    if (enabled != Enabled) {
        Enabled = enabled;
        Enabled_Changed(enabled);
    }
}


// void GraphicMenuItem::Set_Visible(bool visible) -- 0x004E0180
// Stores the flag and notifies the item (virtual Visible_Changed) when it changes.

void GraphicMenuItem::Set_Visible(bool visible)
{
    if (visible != Visible) {
        Visible = visible;
        Visible_Changed(visible);
    }
}


// void GraphicMenuItem::Enabled_Changed(bool enabled) -- 0x004E01A0
// Virtual; empty in the base class.

void GraphicMenuItem::Enabled_Changed(bool enabled)
{
}


// void GraphicMenuItem::Visible_Changed(bool visible) -- 0x004E01B0
// Virtual; empty in the base class.

void GraphicMenuItem::Visible_Changed(bool visible)
{
}


// bool GraphicMenuItem::Contains(Point2D const &point) -- 0x004E01C0
// Virtual; never hit in the base class.

bool GraphicMenuItem::Contains(Point2D const &point)
{
    return false;
}


// bool GraphicMenuItem::Is_Input_Key(int unknown) -- 0x004E01D0
// Virtual; false in the base class.

bool GraphicMenuItem::Is_Input_Key(int unknown)
{
    return false;
}


// void GraphicMenuItem::Play_Sound(int) -- 0x004E01E0
// Plays the item's sound entry, if any (MSSfxEntry::Play 0x005B16D0).

void GraphicMenuItem::Play_Sound(int unknown)
{
    if (Sound != 0) {
        Sound->Play();
    }
}


// GraphicMenuItem scalar deleting destructor (0x004E01F0, ??_G): emitted
// with the vtable by the constructor's unit; the destructor is expanded inline.
