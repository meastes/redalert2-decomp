// tab.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tab.json carries the addresses and the rows that
// stay in their own files.


#define RA2_GADGET_SET_FLAGS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/tab.hpp"
#include "shared/crt_string.hpp"
#include "shared/tool_tip.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"


// Header-static dynamic initializers of the unit at 0x006A0270: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x006A0270-0x006A0451); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006A0290, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A02C0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A02E0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A0300, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A0320, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A0340, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A0360, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A03A0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A03D0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A0400, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A0430, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializer (0x006A0440, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_006a0270.cpp; no source of its own.


// Dynamic initializers of Tab.cpp's static buttons (0x006A0460 Buttons[25],
// 0x006A04C0, 0x006A0500, _$E) and their guarded atexit destructors
// (0x006A0490, 0x006A04E0, 0x006A0520; guard byte 0x00ABD904), emitted by
// these definitions.

ShapeButtonClass TabClass::Buttons[25];
ShapeButtonClass TabClass::unknown_button_abe400;
ShapeButtonClass TabClass::unknown_button_abe390;


// Static-member initializer or guarded atexit destructor (0x006A0490, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x006A04C0, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x006A04E0, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x006A0500, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x006A0520, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// void __fastcall TabClass::Unknown_006A0540(int index) (0x006A0540)
// Frees button shape index when it was loaded rather than retrieved.

void __fastcall TabClass::Unknown_006A0540(int index)
{
    if (ButtonShapeAllocated[index]) {
        operator delete((void *)ButtonShapes[index]);
        ButtonShapeAllocated[index] = false;
    }
    ButtonShapes[index] = 0;
}


// int __fastcall TabClass::Unknown_006A0580(char const *name) (0x006A0580)
// Index of name among the command names, else the no-command value.

int __fastcall TabClass::Unknown_006A0580(char const *name)
{
    for (int index = 0; index < 11; index++) {
        if (strcmp(unknown_command_names_7f67f8[index], name) == 0) {
            return index;
        }
    }
    return unknown_no_command_7f67f4;
}


// void __fastcall TabClass::Unknown_006A05E0(int slot, int button) (0x006A05E0)
// Maps slot to button.

void __fastcall TabClass::Unknown_006A05E0(int slot, int button)
{
    SlotButton[slot] = button;
}


// int __fastcall TabClass::Unknown_006A05F0(int slot) (0x006A05F0)
// The button slot maps to. No direct callers (inlined elsewhere).

int __fastcall TabClass::Unknown_006A05F0(int slot)
{
    return SlotButton[slot];
}


// ShapeButtonClass *__fastcall TabClass::Unknown_006A0600(int slot) (0x006A0600)
// The button mapped to slot, or null when the map entry is out of range.

ShapeButtonClass *__fastcall TabClass::Unknown_006A0600(int slot)
{
    int button = SlotButton[slot];
    if (button >= 0 && button < 25) {
        return &Buttons[button];
    }
    return 0;
}


// Rect __fastcall TabClass::Unknown_006A0620(int slot) (0x006A0620)
// Screen rectangle of slot: the 0x00AC120C rectangle moved down by the
// mapped button's row.

Rect __fastcall TabClass::Unknown_006A0620(int slot)
{
    Rect rect = *unknown_rect_ac120c;
    return Rect(SlotButton[slot] * unknown_int_abe388 + rect.X, unknown_int_abe374, rect.Width, rect.Height);
}


// void __fastcall TabClass::Unknown_006A0670(int rows) (0x006A0670)
// Sets the bar's row count and its edge: the 0x00AC1210 rectangle's X less
// rows widths of the 0x00AC120C rectangle, but not less than the right edge
// of the 0x00AC1204 rectangle.

void __fastcall TabClass::Unknown_006A0670(int rows)
{
    unknown_int_abe2ac = rows;
    int top = unknown_rect_ac1210->X - rows * unknown_rect_ac120c->Width;
    int limit = unknown_rect_ac1204->X + unknown_rect_ac1204->Width;
    unknown_int_abe37c = top > limit ? top : limit;
}


// int __fastcall TabClass::Unknown_006A06B0(int value) (0x006A06B0)
// 1 when value lies in [0x00ABE370, 0x00ABE468], else 0.

int __fastcall TabClass::Unknown_006A06B0(int value)
{
    if (value >= unknown_int_abe370 && value <= unknown_int_abe468) {
        return 1;
    }
    return 0;
}


// int __fastcall TabClass::Unknown_006A06D0(int value) (0x006A06D0)
// value counted from 0x00ABE370, starting at 1.

int __fastcall TabClass::Unknown_006A06D0(int value)
{
    return value - unknown_int_abe370 + 1;
}


// TabClass::TabClass() (0x006A06E0)
// The sidebar's constructor, the notice sink and the member timers; looks up
// the button slots of the bar's commands by name (0x006A0580, expanded here)
// and registers itself as the planning mode's notice sink.

TabClass::TabClass() : unknown_4f14(true), unknown_4f15(true), unknown_4f16(true)
{
    unknown_int_abe464 = Unknown_006A0580("AttackMove");
    unknown_int_abe294 = Unknown_006A0580("Beacon");
    unknown_int_abd910 = Unknown_006A0580("Cheer");
    unknown_int_abe278 = Unknown_006A0580("Deploy");
    unknown_int_abe2b8 = Unknown_006A0580("Guard");
    unknown_int_abe36c = Unknown_006A0580("PlanningMode");
    unknown_int_abe2bc = Unknown_006A0580("Stop");
    unknown_int_abe370 = Unknown_006A0580("Team01");
    unknown_int_abe378 = Unknown_006A0580("Team02");
    unknown_int_abe468 = Unknown_006A0580("Team03");
    unknown_int_abe290 = Unknown_006A0580("TypeSelect");
    Unknown_Set_Sink_00613e00(this);
}


// void TabClass::One_Time() (0x006A0B20, slot 5)
// Only the sidebar's one-time setup (a tail jump).

void TabClass::One_Time()
{
    SidebarClass::One_Time();
}


// void TabClass::DisposeOfArt() (0x006A0B30, slot 50)
// After the sidebar's, frees every loaded button shape.

void TabClass::DisposeOfArt()
{
    SidebarClass::DisposeOfArt();
    for (int index = 0; index < 25; index++) {
        Unknown_006A0540(index);
    }
}


// void TabClass::Init_For_House() (0x006A0B70, slot 53)
// After the sidebar's, loads the 25 button shapes ("Button%02d.SHP") and
// derives the bar metrics from the first one and the layout rectangles.

void TabClass::Init_For_House()
{
    SidebarClass::Init_For_House();
    bool allocated = false;
    for (int index = 0; index < 25; index++) {
        char name[256];
        sprintf(name, "Button%02d.SHP", index);
        ButtonShapes[index] = Unknown_Load_Shape_00495270(name, allocated);
        ButtonShapeAllocated[index] = allocated;
    }
    short const *header = (short const *)ButtonShapes[0];
    unknown_int_abe478 = header[1];
    unknown_int_abe2a8 = header[2];
    Rect slot = *unknown_rect_ac120c;
    unknown_int_abe37c = unknown_rect_ac1210->X - unknown_int_abe2ac * slot.Width;
    unknown_int_abe374 = unknown_rect_ac11fc->Y;
    unknown_int_abe460 = slot.Width - unknown_int_abe478;
    unknown_int_abe388 = unknown_int_abe460 + unknown_int_abe478;
    unknown_4eec.unknown_04 = 0;
}


// 0x006a1860: function not yet identified
//
// Body: 29 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_006a1860(int param_1,undefined4 param_2,undefined4 param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006a1860 {

extern "C" unsigned int DAT_008323cc;

void __fastcall FUN_006a1860(int object_address, unsigned int unknown_0x24, unsigned int unknown_0x20)
{
    *(unsigned int *)(object_address + 0x24) = unknown_0x24;
    *(unsigned int *)(object_address + 0x50) = DAT_008323cc;
    *(unsigned int *)(object_address + 0x20) = unknown_0x20;
    *(unsigned char *)(object_address + 0x2d) = 0;
    *(unsigned int *)(object_address + 0x30) = 0;
    return;
}

}  // namespace seed_006a1860


// void TabClass::Init_IO() (0x006A0C50, slot 8)
// After the sidebar's, when the game is not reloading and the shell art is
// loaded: 0x006A1880, then sets up the two side buttons (ids 0xF0 and 0xF1)
// at the 0x00AC1204 and 0x00AC1208 rectangles with the 0x00AC1098 shape.

void TabClass::Init_IO()
{
    SidebarClass::Init_IO();
    if (!Debug_Map && Unknown_Get_Art_006f4590()) {
        Unknown_006A1880();
        unknown_button_abe400.ID = 0xF0;
        unknown_button_abe400.ShapeDrawer = unknown_drawer_8323cc;
        unknown_button_abe400.IsOn = false;
        unknown_button_abe400.ToggleType = 0;
        unknown_button_abe400.Flags = 5;
        void const *shape = unknown_buffer_ac1098;
        Rect area = *unknown_rect_ac1204;
        unknown_button_abe400.Set_Position(area.X, area.Y);
        unknown_button_abe400.Set_Shape(shape, 0, 0);
        unknown_button_abe390.ID = 0xF1;
        unknown_button_abe390.ShapeDrawer = unknown_drawer_8323cc;
        unknown_button_abe390.IsOn = false;
        unknown_button_abe390.ToggleType = 0;
        unknown_button_abe390.Flags = 5;
        Rect area2 = *unknown_rect_ac1208;
        unknown_button_abe390.Set_Position(area2.X, area2.Y);
        unknown_button_abe390.Set_Shape(shape, 0, 0);
    }
}


// void TabClass::Unknown_006A0D50() (0x006A0D50)
// Unlinks each of the 25 buttons and adds it to the button list.

void TabClass::Unknown_006A0D50()
{
    for (int index = 0; index < 25; index++) {
        ShapeButtonClass &button = Buttons[index];
        button.Zap();
        Add_A_Button(button);
    }
}


// void TabClass::Unknown_006A0D80() (0x006A0D80)
// Removes the 25 buttons from the button list.

void TabClass::Unknown_006A0D80()
{
    for (int index = 0; index < 25; index++) {
        Remove_A_Button(Buttons[index]);
    }
}


// bool TabClass::unknown_slot54(int control) (0x006A0DA0, slot 54)
// Shows or hides the sidebar (the sidebar's slot 54); when that changed,
// updates the side buttons and their help regions, then redraws.

bool TabClass::unknown_slot54(int control)
{
    bool changed = SidebarClass::unknown_slot54(control);
    if (changed) {
        if (unknown_4ee1) {
            if (unknown_4f14) {
                Unknown_006A1AB0();
            } else {
                Add_A_Button(unknown_button_abe390);
                Unknown_006A1270(&unknown_button_abe390, "Tip:ThumbClosed");
            }
        } else if (unknown_4f14) {
            for (int index = 0; index < 25; index++) {
                Remove_A_Button(Buttons[index]);
            }
            Remove_A_Button(unknown_button_abe400);
            for (int slot = 0; slot < 25; slot++) {
                ShapeButtonClass *button = Unknown_006A0600(slot);
                if (button != 0) {
                    ToolTips->Remove(button->ID);
                }
            }
            ToolTips->Remove(0xF0);
        } else {
            Remove_A_Button(unknown_button_abe390);
            ToolTips->Remove(0xF1);
        }
        Flag_To_Redraw(2);
    }
    return changed;
}


// void TabClass::AI(int &input, Point2D const &xy) (0x006A0F30, slot 10)
// The command bar's input: redraws when its timer runs to 1, shows the
// normal mouse over the bar, runs the command of a pressed button (the team
// buttons select, assign or recall their group; with the shift form they
// clear it), opens (0x80F1, 0x006A1EC0) or closes (0x80F0, 0x006A1F10) the
// bar with its sound, then the sidebar's AI.

void TabClass::AI(int &input, Point2D const &xy)
{
    if (unknown_4f08.Value() == 1) {
        unknown_4f15 = true;
        Flag_To_Redraw(false);
    }
    Rect rect(unknown_rect_ac11fc->X, unknown_rect_ac11fc->Y, unknown_int_839638, unknown_int_7add84 - 2);
    if (rect.Is_Point_Within(xy)) {
        Map.Override_Mouse_Shape(0, false);
    }
    int key = input;
    if (key >= 0x80D6 && key < 0x80EF) {
        int id = (key & ~0x8000) - 0xD6;
        if (id == unknown_int_abe464) {
            Unknown_006F6570();
        } else if (id == unknown_int_abe294) {
            Unknown_006F64B0();
        } else if (id == unknown_int_abd910) {
            Unknown_006F5C50();
        } else if (id == unknown_int_abe278) {
            Unknown_006F5940();
        } else if (id == unknown_int_abe2b8) {
            Unknown_006F5B10();
        } else if (id == unknown_int_abe36c) {
            if (Unknown_006A0600(unknown_int_abe36c)->IsOn) {
                Unknown_006F64D0(true);
            } else {
                Unknown_006F64F0(true);
            }
        } else if (id == unknown_int_abe2bc) {
            Unknown_006F5BC0();
        } else if (Unknown_006A06B0(id)) {
            int group = Unknown_006A06D0(id);
            if (Unknown_006F5870(group) == 0) {
                Unknown_006F5D90(group);
            } else if (Unknown_006F5800(group)) {
                Unknown_006F60D0(group);
            } else {
                Unknown_006F5F00(group);
            }
        } else if (id == unknown_int_abe290) {
            Unknown_006F7360();
        }
    } else if (key == 0x80F0) {
        Unknown_Play_Sound(Rules->unknown_190);
        Unknown_006A1F10();
    } else if (key == 0x80F1) {
        if (unknown_int_a3d548 == 0) {
            Unknown_Play_Sound(Rules->unknown_18c);
            Unknown_006A1EC0();
        }
    } else if (key >= 0xC0D6 && key < 0xC0EF) {
        int id = (key & ~0xC000) - 0xD6;
        if (Unknown_006A06B0(id)) {
            Unknown_006F5E10(Unknown_006A06D0(id));
        }
    }
    unknown_4eec.Unknown_00494650(0);
    SidebarClass::AI(input, xy);
}


// void __fastcall TabClass::Unknown_006A1270(ControlClass *control, char const *label) (0x006A1270)
// Registers a control's area as a help region under the control's ID with
// the given string table label, replacing any region with that ID.

void __fastcall TabClass::Unknown_006A1270(ControlClass *control, char const *label)
{
    if (control != 0) {
        ToolTip tip;
        tip.ID = control->ID;
        tip.Label = label;
        tip.Region.X = control->X;
        tip.Region.Y = control->Y;
        tip.Region.Width = control->Width;
        tip.Region.Height = control->Height;
        ToolTips->Remove(control->ID);
        ToolTips->Add(tip);
    }
}


// void TabClass::Draw_It(bool complete) (0x006A12D0, slot 16)
// Redraws the command bar when it needs it: when asked to, when one of its
// buttons wants a repaint, when the mouse moved over it or left it, or when
// the surface it last drew on is not the current one. The bar's pieces are
// drawn as shapes (background, caps, tiles, the open bar's tail), the
// buttons that asked for a repaint are drawn again, and the surface is
// remembered (the four most recent). Then the sidebar's drawing.

void TabClass::Draw_It(bool complete)
{
    static UnknownTabSpot last_spot;
    static bool mouse_inside;
    static Surface *drawn_on[4];
    static int drawn_count;

    bool redraw = complete || unknown_4f15;
    if (!redraw) {
        for (int slot = 0; slot < 25; slot++) {
            ShapeButtonClass *button = Unknown_006A0600(slot);
            if (button != 0 && button->IsDrawn) {
                redraw = true;
                break;
            }
        }
    }
    if (redraw) {
        drawn_count = 0;
    } else {
        Point2D mouse = WWMouse->Get_Mouse_XY();
        if (unknown_rect_ac11fc->Is_Point_Within(mouse)) {
            mouse_inside = true;
            if (mouse.X != last_spot.X || mouse.Y != last_spot.Y) {
                redraw = true;
            }
        } else {
            if (mouse_inside) {
                redraw = true;
            }
            mouse_inside = false;
        }
        last_spot.X = mouse.X;
        last_spot.Y = mouse.Y;
        if (redraw) {
            drawn_count = 0;
        } else {
            int index;
            for (index = 0; index < drawn_count; index++) {
                if (drawn_on[index] == unknown_surface_83995c) {
                    goto drawn;
                }
            }
            redraw = true;
        drawn:;
        }
    }
    if (!Debug_Map) {
        Unknown_006A1710();
        unknown_4eec.Unknown_00494530(true);
        if (redraw) {
            Rect view = unknown_surface_83995c->Get_Rect();
            Point2D where(0, 0);
            where.X = unknown_rect_ac11fc->X;
            where.Y = unknown_rect_ac11fc->Y;
            Rect narrow = view;
            narrow.Width = unknown_rect_ac11fc->Width;
            Surface *surface = unknown_surface_83995c;
            CC_Draw_Shape(surface, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_buffer_ac1048, 0,
                          where, narrow, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            if (unknown_4f14) {
                where.X = unknown_rect_ac1204->X;
                where.Y = unknown_rect_ac1204->Y;
            } else {
                where.X = unknown_rect_ac1208->X;
                where.Y = unknown_rect_ac1208->Y;
            }
            CC_Draw_Shape(surface, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_buffer_ac1098, 0,
                          where, view, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            if (unknown_4f14) {
                where.X = unknown_rect_ac120c->X;
                where.Y = unknown_rect_ac11fc->Y;
                for (int tile = 0; tile < unknown_int_ac0ff4; tile++) {
                    CC_Draw_Shape(surface, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_buffer_ac10a4, 0,
                                  where, view, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    where.X += unknown_rect_ac120c->Width;
                }
                for (int slot = 0; slot < 25; slot++) {
                    ShapeButtonClass *button = Unknown_006A0600(slot);
                    if (button != 0 && button->ShapeData != 0) {
                        button->Draw_Me(1);
                        button->IsDrawn = false;
                    }
                }
            }
            where.X = unknown_rect_ac1210->X;
            CC_Draw_Shape(surface, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_buffer_ac10b0, 0,
                          where, view, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            if (unknown_4f14) {
                unknown_button_abe400.Draw_Me(1);
            } else {
                unknown_button_abe390.Draw_Me(1);
            }
            unknown_4f16 = true;
            unknown_4f15 = false;
            if (drawn_count < sizeof(drawn_on) / sizeof(drawn_on[0])) {
                drawn_count++;
            }
            for (int index = drawn_count - 2; index >= 0; index--) {
                drawn_on[index + 1] = drawn_on[index];
            }
            drawn_on[0] = surface;
        } else {
            unknown_4f16 = false;
        }
    }
    SidebarClass::Draw_It(complete);
}


// atexit destructor of TabClass::Draw_It's local static last_spot (0x006A1700, _$E):
// emitted by the definition in TabClass_Draw_It_006a12d0.cpp; no source of its own.


// void TabClass::Unknown_006A1710() (0x006A1710)
// Draws the bar's background shape at the origin of the sidebar surface
// (brightness 1000) and notes that a sidebar button repainted.

void TabClass::Unknown_006A1710()
{
    ShapeFileStruct const *shape = (ShapeFileStruct const *)unknown_buffer_ac10f8;
    CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, shape, 0,
                  Point2D(0, 0), unknown_surface_839948->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    unknown_flag_abcd84 = true;
}


// void TabClass::Unknown_006A1770() (0x006A1770)
// Redraws the bar: sets +0x4F15 and restarts the +0x4F08 timer at 7 frames.

void TabClass::Unknown_006A1770()
{
    unknown_4f15 = true;
    Flag_To_Redraw(false);
    unknown_4f08 = 7;
}


void TabClass::Unknown_006A1880()
{
    for (int index=0; index<25; index++) Buttons[index].Set_Shape(0,0,0);
    Rect area = *unknown_rect_ac1208;
    int right=area.X+area.Width;
    for (int slot=0;slot<25;slot++) {
        ShapeButtonClass *button=Unknown_006A0600(slot);
        if (button) {
            Rect box=*unknown_rect_ac120c;
            int x = SlotButton[slot]*unknown_int_abe388 + box.X;
            int y = unknown_int_abe374;
            if (box.Width+x<=right) {
                button->ID=slot+0xd6;
                button->ShapeDrawer=unknown_drawer_8323cc;
                button->IsOn=false;
                button->ToggleType=0;
                button->Set_Flags(5);
                button->Set_Position(x,y);
                button->Set_Shape(ButtonShapes[slot],0,0);
            }
        }
    }
    ShapeButtonClass *planning=Unknown_006A0600(unknown_int_abe36c);
    if (planning) {
        planning->ToggleType=1;
        planning->ReflectButtonState=true;
    }
    for (int team_slot=unknown_int_abe370;team_slot<=unknown_int_abe468;team_slot++) {
        ShapeButtonClass *button=Unknown_006A0600(team_slot);
        if (button) button->Set_Flags(0x55);
    }
}


// static void TabClass::Unknown_006A19E0() (0x006A19E0)
// Sets up the two side buttons: ids 0xF0 and 0xF1 at the 0x00AC1204 and
// 0x00AC1208 rectangles with the 0x00AC1098 shape. Init_IO inlines it; this
// is the copy the linker kept.

void TabClass::Unknown_006A19E0()
{
    unknown_button_abe400.ID = 0xF0;
    unknown_button_abe400.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_abe400.IsOn = false;
    unknown_button_abe400.ToggleType = 0;
    unknown_button_abe400.Flags = 5;
    void const *shape = unknown_buffer_ac1098;
    Rect area = *unknown_rect_ac1204;
    unknown_button_abe400.Set_Position(area.X, area.Y);
    unknown_button_abe400.Set_Shape(shape, 0, 0);
    unknown_button_abe390.ID = 0xF1;
    unknown_button_abe390.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_abe390.IsOn = false;
    unknown_button_abe390.ToggleType = 0;
    unknown_button_abe390.Flags = 5;
    Rect area2 = *unknown_rect_ac1208;
    unknown_button_abe390.Set_Position(area2.X, area2.Y);
    unknown_button_abe390.Set_Shape(shape, 0, 0);
}


void TabClass::Unknown_006A1AB0()
{
    Unknown_006A0D50();
    Add_A_Button(unknown_button_abe400);
    Unknown_006A1270(Unknown_006A0600(unknown_int_abe464), "Tip:AttackMove");
    Unknown_006A1270(Unknown_006A0600(unknown_int_abe294), "Tip:Beacon");
    Unknown_006A1270(Unknown_006A0600(unknown_int_abd910), "Tip:Cheer");
    Unknown_006A1270(Unknown_006A0600(unknown_int_abe278), "Tip:Deploy");
    Unknown_006A1270(Unknown_006A0600(unknown_int_abe2b8), "Tip:Guard");
    Unknown_006A1270(Unknown_006A0600(unknown_int_abe36c), "Tip:PlanningMode");
    {
        ShapeButtonClass *button = Unknown_006A0600(unknown_int_abe2bc);
        Unknown_006A1270(button, "Tip:Stop");
    }
    {
        ShapeButtonClass *button = Unknown_006A0600(unknown_int_abe370);
        Unknown_006A1270(button, "Tip:Team01");
    }
    {
        ShapeButtonClass *button = Unknown_006A0600(unknown_int_abe378);
        Unknown_006A1270(button, "Tip:Team02");
    }
    {
        ShapeButtonClass *button = Unknown_006A0600(unknown_int_abe468);
        Unknown_006A1270(button, "Tip:Team03");
    }
    {
        ShapeButtonClass *button = Unknown_006A0600(unknown_int_abe290);
        Unknown_006A1270(button, "Tip:TypeSelect");
    }
    Unknown_006A1270(&unknown_button_abe400, "Tip:ThumbOpen");
}


// void TabClass::Unknown_006A1DA0() (0x006A1DA0)
// Removes the open bar's 25 buttons and the extra one from the button list,
// and the help regions of the buttons in use and of the bar itself (0xF0).
// Removes the 25 buttons through 0x006A0D80 (inlined here; TabClass::AI's
// inlined copy of this function calls it out of line).

void TabClass::Unknown_006A1DA0()
{
    Unknown_006A0D80();
    Remove_A_Button(unknown_button_abe400);
    for (int slot = 0; slot < 25; slot++) {
        ShapeButtonClass *button = Unknown_006A0600(slot);
        if (button != 0) {
            ToolTips->Remove(button->ID);
        }
    }
    ToolTips->Remove(0xF0);
}


// void TabClass::Unknown_006A1E20() (0x006A1E20)
// Shows the closed bar's thumb button with its help region.

void TabClass::Unknown_006A1E20()
{
    Add_A_Button(unknown_button_abe390);
    Unknown_006A1270(&unknown_button_abe390, "Tip:ThumbClosed");
}


// void TabClass::Unknown_006A1EA0() (0x006A1EA0)
// Hides the closed bar's thumb button and its help region (0xF1).

void TabClass::Unknown_006A1EA0()
{
    Remove_A_Button(unknown_button_abe390);
    ToolTips->Remove(0xF1);
}


// void TabClass::Unknown_006A1EC0() (0x006A1EC0)
// Opens the bar when it is closed: swaps the thumb for the bar's buttons.

void TabClass::Unknown_006A1EC0()
{
    if (!unknown_4f14) {
        Unknown_006A1EA0();
        unknown_4f14 = true;
        Unknown_006A1AB0();
        unknown_4f15 = true;
        Flag_To_Redraw(0);
    }
}


// void TabClass::Unknown_006A1F10() (0x006A1F10)
// Closes the bar when it is open: swaps the bar's buttons for the thumb.

void TabClass::Unknown_006A1F10()
{
    if (unknown_4f14) {
        Unknown_006A1DA0();
        unknown_4f14 = false;
        Unknown_006A1E20();
        unknown_4f15 = true;
        Flag_To_Redraw(0);
    }
}


// bool TabClass::INoticeSink_Unknown(int code, int arg) (0x006A2020; the
// INoticeSink vtable 0x007A64DC)
// Notice 0x11C7 turns the planning-mode button on and 0x11C8 turns it off
// (the button of slot 0x00ABE36C, when it is not already in that state).
// True when handled.

bool TabClass::INoticeSink_Unknown(int code, int arg)
{
    if (code == 0x11C7) {
        ShapeButtonClass *button = Unknown_006A0600(unknown_int_abe36c);
        if (button != 0 && !button->IsOn) {
            button->Turn_On();
        }
        return true;
    }
    if (code == 0x11C8) {
        ShapeButtonClass *button = Unknown_006A0600(unknown_int_abe36c);
        if (button != 0 && button->IsOn) {
            button->Turn_Off();
        }
        return true;
    }
    return false;
}


// wchar_t const *TabClass::Help_Text(int id) (0x006A20B0, slot 33): the tooltip text of
// a control: SidebarClass's (0x00681ED0, called directly).

wchar_t const *TabClass::Help_Text(int id)
{
    return SidebarClass::Help_Text(id);
}


// ToolTip constructor, original kept copy006A20C0. Body is the real shared inline constructor.
