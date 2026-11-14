// cctooltip.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cctooltip.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/tool_tip.hpp"
#include "shared/mouse.hpp"
#include "shared/text_print.hpp"
#include "shared/map_view.hpp"


// Header-static dynamic initializers of the unit at 0x0046DDD0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0046DDD0-0x0046DFB1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0046DDF0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DE20, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DE40, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DE60, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DE80, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DEA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DEC0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DF00, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DF30, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DF60, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DF90, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// Dynamic initializer (0x0046DFA0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0046ddd0.cpp; no source of its own.


// bool CCToolTip::Show(ToolTipRecord *record) (0x0046DFC0, slot 1)

bool CCToolTip::Show(ToolTipRecord *record)
{
    bool mouse_visible = WWMouse->Get_Mouse_State() >= 0;
    if (mouse_visible) {
    BitFontClass *font = BitFont;
    Rect *area = 0;
    if (unknown_flag_a40b30 == true) {
        if (record->unknown_00 > unknown_rect_839630.X + unknown_rect_839630.Width) {
            area = &unknown_rect_839620;
            unknown_flag_8373c2 = true;
        } else {
            area = &unknown_rect_839630;
        }
    } else if (record->unknown_00 <= unknown_rect_839620.X + unknown_rect_839620.Width) {
        area = &unknown_rect_839620;
        unknown_flag_8373c2 = true;
    } else {
        area = &unknown_rect_839630;
    }
    if (area != 0) {
        int width;
        int height;
        font->Get_Text_Area(record->Text, &width, &height, area->Width);
        width += 4;
        height += 3;
        record->unknown_08 = width > record->unknown_08 ? width : record->unknown_08;
        record->unknown_0c = height > record->unknown_0c ? height : record->unknown_0c;
        if (record->unknown_08 >= area->Width) {
            font->Get_Text_Area(record->Text, &width, &height, area->Width - 4);
            width += 4;
            height += 3;
        record->unknown_08 = width > record->unknown_08 ? width : record->unknown_08;
        record->unknown_0c = height > record->unknown_0c ? height : record->unknown_0c;
        }
        ToolTip *tip = Current;
        if (tip == 0 || tip->unknown_18 == false) {
            int over = record->unknown_00 - area->X - area->Width + record->unknown_08;
            if (over > 0) {
                record->unknown_00 -= over;
            }
            record->unknown_04 += 16;
            if (record->unknown_04 - area->Height - area->Y + record->unknown_0c > 0) {
                record->unknown_04 = record->unknown_04 - record->unknown_0c - 16;
            }
        } else {
            Rect region = tip->Region;
            if (region.X < area->Width / 2 + area->X) {
                record->unknown_00 = region.Width - 5 + region.X;
            } else {
                record->unknown_00 = region.X - width + 5;
            }
            record->unknown_00 = area->X > record->unknown_00 ? area->X : record->unknown_00;
            if (region.Y < area->Height / 2 + area->Y) {
                record->unknown_04 = region.Height - 5 + region.Y;
            } else {
                record->unknown_04 = region.Y - height + 5;
            }
            record->unknown_04 = area->Y > record->unknown_04 ? area->Y : record->unknown_04;
        }
    }
    return true;
    }
    return false;
}


// void CCToolTip::Hide(ToolTipRecord *record) (0x0046E1D0, slot 2)
// When the tip sits over the tactical area, flags the screen for a redraw,
// then clears the base record.

void CCToolTip::Hide(ToolTipRecord *record)
{
    if (unknown_flag_a40b30 == true) {
        if (record->unknown_00 >= unknown_rect_839630.X + unknown_rect_839630.Width) {
            unknown_flag_8373c2 = true;
            unknown_flag_8373c3 = true;
        }
    } else if (record->unknown_00 <= unknown_rect_839620.X + unknown_rect_839620.Width) {
        unknown_flag_8373c2 = true;
        unknown_flag_8373c3 = true;
    }
    Map.GScreenClass::Flag_To_Redraw(0);
    ToolTipManager::Hide(record);
}


// void CCToolTip::Refresh(bool update) (0x0046E230, slot 3)
// Remembers the flag and refreshes the base with its opposite.

void CCToolTip::Refresh(bool update)
{
    unknown_260 = update;
    ToolTipManager::Refresh(update == false);
}


// CCToolTip text-box drawing, original0046E250; existing slot4 contract retained.
extern RGBClass unknown_color_ac1028;
void CCToolTip::Debug_Print(ToolTipRecord *record)
{
    Rect box;
    int x = record->unknown_00;
    int y = record->unknown_04;
    Surface *surface = 0;
    if (unknown_flag_a40b30 == true) {
        int boundary = unknown_rect_839630.X + unknown_rect_839630.Width;
        if (x + record->unknown_08 <= boundary) surface = unknown_surface_839964;
        else if (unknown_260 == true && x >= boundary) {
            surface = unknown_surface_839948;
            x -= boundary;
            unknown_flag_abcd84 = true;
        }
    } else {
        int boundary = unknown_rect_839620.X + unknown_rect_839620.Width;
        if (x >= boundary) {
            surface = unknown_surface_839964;
            x -= boundary;
        } else if (unknown_260 == true && x + record->unknown_08 < boundary) {
            surface = unknown_surface_839948;
            unknown_flag_abcd84 = true;
        }
    }
    if (surface) {
        BitFontClass *font = BitFont;
        tagRECT area;
        area.left = x;
        area.top = y;
        area.right = x;
        area.bottom = y;
        wchar_t const *text = record->Text;
        int width, height;
        font->Get_Text_Area(text, &width, &height, surface->Get_Width());
        area.right = width + area.left;
        area.bottom = height + area.top;
        int box_width = area.right - area.left + 8;
        int box_height = area.bottom - area.top + 4;
        if (area.left + box_width > surface->Get_Width()) {
            area.left = surface->Get_Width() - box_width;
        }
        area.right = box_width + area.left;
        area.bottom = box_height + area.top;
        int color = Build_Hicolor_Pixel(unknown_color_ac1028.Red, unknown_color_ac1028.Green, unknown_color_ac1028.Blue);
        box.Set(area.left, area.top, box_width, box_height);
        surface->Fill_Rect(box, 0);
        surface->Draw_Rect(box, color);
        font->Set_Clipping(1);
        font->Set_Clip_Box(&area);
        font->Set_Color((unsigned short)color);
        BitText->Print_Box(font, surface, text, area.left + 4, area.top + 2,
            area.right - area.left, area.bottom - area.top, 0, 0, 0);
    }
}


// wchar_t const *CCToolTip::Get_Text(int id) (0x0046E470, slot 5)
// The text the game screen has for the tip id; none while the mouse reports a negative state.

wchar_t const *CCToolTip::Get_Text(int id)
{
    if (WWMouse->Get_Mouse_State() < 0) {
        return 0;
    }
    return Map.TabClass::Help_Text(id);
}
