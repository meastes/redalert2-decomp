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
ShapeButtonClass TabClass::ThumbOpenButton;
ShapeButtonClass TabClass::ThumbClosedButton;


// Static-member initializer or guarded atexit destructor (0x006A0490, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x006A04C0, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x006A04E0, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x006A0500, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// Static-member initializer or guarded atexit destructor (0x006A0520, _$E) emitted by the definitions in TabClass_static_members_init_006a0460.cpp; no source of its own.


// void __fastcall TabClass::Free_Button_Shape(int index) (0x006A0540)
// Frees button shape index when it was loaded rather than retrieved.

void __fastcall TabClass::Free_Button_Shape(int index)
{
    if (ButtonShapeAllocated[index]) {
        operator delete((void *)ButtonShapes[index]);
        ButtonShapeAllocated[index] = false;
    }
    ButtonShapes[index] = 0;
}


// int __fastcall TabClass::Command_From_Name(char const *name) (0x006A0580)
// Index of name among the command names, else the no-command value.

int __fastcall TabClass::Command_From_Name(char const *name)
{
    for (int index = 0; index < 11; index++) {
        if (strcmp(CommandNames[index], name) == 0) {
            return index;
        }
    }
    return NoAdvancedCommand;
}


// void __fastcall TabClass::Set_Command_Position(int slot, int button) (0x006A05E0)
// Maps slot to button.

void __fastcall TabClass::Set_Command_Position(int slot, int button)
{
    SlotButton[slot] = button;
}


// int __fastcall TabClass::Get_Command_Position(int slot) (0x006A05F0)
// The button slot maps to. No direct callers (inlined elsewhere).

int __fastcall TabClass::Get_Command_Position(int slot)
{
    return SlotButton[slot];
}


// ShapeButtonClass *__fastcall TabClass::Get_Button(int slot) (0x006A0600)
// The button mapped to slot, or null when the map entry is out of range.

ShapeButtonClass *__fastcall TabClass::Get_Button(int slot)
{
    int button = SlotButton[slot];
    if (button >= 0 && button < 25) {
        return &Buttons[button];
    }
    return 0;
}


// Rect __fastcall TabClass::Get_Button_Rect(int slot) (0x006A0620)
// Screen rectangle of slot: the 0x00AC120C rectangle moved down by the
// mapped button's row.

Rect __fastcall TabClass::Get_Button_Rect(int slot)
{
    Rect rect = *ButtonSlotRect;
    return Rect(SlotButton[slot] * TabClass_SlotPitch + rect.X, TabClass_SlotTop, rect.Width, rect.Height);
}


// void __fastcall TabClass::Set_Button_Count(int rows) (0x006A0670)
// Sets the bar's row count and its edge: the 0x00AC1210 rectangle's X less
// rows widths of the 0x00AC120C rectangle, but not less than the right edge
// of the 0x00AC1204 rectangle.

void __fastcall TabClass::Set_Button_Count(int rows)
{
    TabClass_ButtonRows = rows;
    int top = TabEndRect->X - rows * ButtonSlotRect->Width;
    int limit = ThumbOpenRect->X + ThumbOpenRect->Width;
    TabClass_ButtonsLeft = top > limit ? top : limit;
}


// int __fastcall TabClass::Is_Team_Command(int value) (0x006A06B0)
// 1 when value lies in [0x00ABE370, 0x00ABE468], else 0.

int __fastcall TabClass::Is_Team_Command(int value)
{
    if (value >= TabClass_Team01Command && value <= TabClass_Team03Command) {
        return 1;
    }
    return 0;
}


// int __fastcall TabClass::Team_Number(int value) (0x006A06D0)
// value counted from 0x00ABE370, starting at 1.

int __fastcall TabClass::Team_Number(int value)
{
    return value - TabClass_Team01Command + 1;
}


// TabClass::TabClass() (0x006A06E0)
// The sidebar's constructor, the notice sink and the member timers; looks up
// the button slots of the bar's commands by name (0x006A0580, expanded here)
// and registers itself as the planning mode's notice sink.

TabClass::TabClass() : IsThumbActive(true), unknown_4f15(true), IsDrawn(true)
{
    TabClass_AttackMoveCommand = Command_From_Name("AttackMove");
    TabClass_BeaconCommand = Command_From_Name("Beacon");
    TabClass_CheerCommand = Command_From_Name("Cheer");
    TabClass_DeployCommand = Command_From_Name("Deploy");
    TabClass_GuardCommand = Command_From_Name("Guard");
    TabClass_PlanningModeCommand = Command_From_Name("PlanningMode");
    TabClass_StopCommand = Command_From_Name("Stop");
    TabClass_Team01Command = Command_From_Name("Team01");
    TabClass_Team02Command = Command_From_Name("Team02");
    TabClass_Team03Command = Command_From_Name("Team03");
    TabClass_TypeSelectCommand = Command_From_Name("TypeSelect");
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
        Free_Button_Shape(index);
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
    TabClass_ButtonWidth = header[1];
    TabClass_ButtonHeight = header[2];
    Rect slot = *ButtonSlotRect;
    TabClass_ButtonsLeft = TabEndRect->X - TabClass_ButtonRows * slot.Width;
    TabClass_SlotTop = TabBarRect->Y;
    TabClass_SlotGap = slot.Width - TabClass_ButtonWidth;
    TabClass_SlotPitch = TabClass_SlotGap + TabClass_ButtonWidth;
    Credits.Current = 0;
}


// Init_Shape_Button (0x006A1860): name from mischa85 names.csv

extern "C" unsigned int DAT_008323cc;

void __fastcall Init_Shape_Button(int object_address, unsigned int unknown_0x24, unsigned int unknown_0x20)
{
    *(unsigned int *)(object_address + 0x24) = unknown_0x24;
    *(unsigned int *)(object_address + 0x50) = DAT_008323cc;
    *(unsigned int *)(object_address + 0x20) = unknown_0x20;
    *(unsigned char *)(object_address + 0x2d) = 0;
    *(unsigned int *)(object_address + 0x30) = 0;
    return;
}



// void TabClass::Init_IO() (0x006A0C50, slot 8)
// After the sidebar's, when the game is not reloading and the shell art is
// loaded: 0x006A1880, then sets up the two side buttons (ids 0xF0 and 0xF1)
// at the 0x00AC1204 and 0x00AC1208 rectangles with the 0x00AC1098 shape.

void TabClass::Init_IO()
{
    SidebarClass::Init_IO();
    if (!Debug_Map && Is_Sidebar_Art_Loaded()) {
        Setup_Command_Buttons();
        ThumbOpenButton.ID = 0xF0;
        ThumbOpenButton.ShapeDrawer = SidebarDrawer;
        ThumbOpenButton.IsOn = false;
        ThumbOpenButton.ToggleType = 0;
        ThumbOpenButton.Flags = 5;
        void const *shape = unknown_buffer_ac1098;
        Rect area = *ThumbOpenRect;
        ThumbOpenButton.Set_Position(area.X, area.Y);
        ThumbOpenButton.Set_Shape(shape, 0, 0);
        ThumbClosedButton.ID = 0xF1;
        ThumbClosedButton.ShapeDrawer = SidebarDrawer;
        ThumbClosedButton.IsOn = false;
        ThumbClosedButton.ToggleType = 0;
        ThumbClosedButton.Flags = 5;
        Rect area2 = *ThumbClosedRect;
        ThumbClosedButton.Set_Position(area2.X, area2.Y);
        ThumbClosedButton.Set_Shape(shape, 0, 0);
    }
}


// void TabClass::Add_Command_Buttons() (0x006A0D50)
// Unlinks each of the 25 buttons and adds it to the button list.

void TabClass::Add_Command_Buttons()
{
    for (int index = 0; index < 25; index++) {
        ShapeButtonClass &button = Buttons[index];
        button.Zap();
        Add_A_Button(button);
    }
}


// void TabClass::Remove_Command_Buttons() (0x006A0D80)
// Removes the 25 buttons from the button list.

void TabClass::Remove_Command_Buttons()
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
        if (IsSidebarActive) {
            if (IsThumbActive) {
                Show_Buttons();
            } else {
                Add_A_Button(ThumbClosedButton);
                Add_Button_ToolTip(&ThumbClosedButton, "Tip:ThumbClosed");
            }
        } else if (IsThumbActive) {
            for (int index = 0; index < 25; index++) {
                Remove_A_Button(Buttons[index]);
            }
            Remove_A_Button(ThumbOpenButton);
            for (int slot = 0; slot < 25; slot++) {
                ShapeButtonClass *button = Get_Button(slot);
                if (button != 0) {
                    ToolTips->Remove(button->ID);
                }
            }
            ToolTips->Remove(0xF0);
        } else {
            Remove_A_Button(ThumbClosedButton);
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
    if (MoneyFlashTimer.Value() == 1) {
        unknown_4f15 = true;
        Flag_To_Redraw(false);
    }
    Rect rect(TabBarRect->X, TabBarRect->Y, unknown_int_839638, TabBarHeight - 2);
    if (rect.Is_Point_Within(xy)) {
        Map.Override_Mouse_Shape(0, false);
    }
    int key = input;
    if (key >= 0x80D6 && key < 0x80EF) {
        int id = (key & ~0x8000) - 0xD6;
        if (id == TabClass_AttackMoveCommand) {
            Command_Attack_Move();
        } else if (id == TabClass_BeaconCommand) {
            Unknown_006F64B0();
        } else if (id == TabClass_CheerCommand) {
            Command_Cheer();
        } else if (id == TabClass_DeployCommand) {
            Command_Deploy();
        } else if (id == TabClass_GuardCommand) {
            Command_Guard();
        } else if (id == TabClass_PlanningModeCommand) {
            if (Get_Button(TabClass_PlanningModeCommand)->IsOn) {
                Command_Planning_Mode_On(true);
            } else {
                Command_Planning_Mode_Off(true);
            }
        } else if (id == TabClass_StopCommand) {
            Command_Stop();
        } else if (Is_Team_Command(id)) {
            int group = Team_Number(id);
            if (Group_Count(group) == 0) {
                Group_Assign(group);
            } else if (Group_Is_All_Selected(group)) {
                Group_Reselect(group);
            } else {
                Group_Select(group);
            }
        } else if (id == TabClass_TypeSelectCommand) {
            Command_Type_Select();
        }
    } else if (key == 0x80F0) {
        Unknown_Play_Sound(Rules->GUICloseSound);
        Close_Thumb();
    } else if (key == 0x80F1) {
        if (MuteSWLaunches == 0) {
            Unknown_Play_Sound(Rules->GUIOpenSound);
            Open_Thumb();
        }
    } else if (key >= 0xC0D6 && key < 0xC0EF) {
        int id = (key & ~0xC000) - 0xD6;
        if (Is_Team_Command(id)) {
            Group_Clear(Team_Number(id));
        }
    }
    Credits.AI(0);
    SidebarClass::AI(input, xy);
}


// void __fastcall TabClass::Add_Button_ToolTip(ControlClass *control, char const *label) (0x006A1270)
// Registers a control's area as a help region under the control's ID with
// the given string table label, replacing any region with that ID.

void __fastcall TabClass::Add_Button_ToolTip(ControlClass *control, char const *label)
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
            ShapeButtonClass *button = Get_Button(slot);
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
        if (TabBarRect->Is_Point_Within(mouse)) {
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
                if (drawn_on[index] == LogicalSurface) {
                    goto drawn;
                }
            }
            redraw = true;
        drawn:;
        }
    }
    if (!Debug_Map) {
        Draw_Credits_Tab();
        Credits.Graphic_Logic(true);
        if (redraw) {
            Rect view = LogicalSurface->Get_Rect();
            Point2D where(0, 0);
            where.X = TabBarRect->X;
            where.Y = TabBarRect->Y;
            Rect narrow = view;
            narrow.Width = TabBarRect->Width;
            Surface *surface = LogicalSurface;
            CC_Draw_Shape(surface, SidebarDrawer, (ShapeFileStruct const *)unknown_buffer_ac1048, 0,
                          where, narrow, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            if (IsThumbActive) {
                where.X = ThumbOpenRect->X;
                where.Y = ThumbOpenRect->Y;
            } else {
                where.X = ThumbClosedRect->X;
                where.Y = ThumbClosedRect->Y;
            }
            CC_Draw_Shape(surface, SidebarDrawer, (ShapeFileStruct const *)unknown_buffer_ac1098, 0,
                          where, view, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            if (IsThumbActive) {
                where.X = ButtonSlotRect->X;
                where.Y = TabBarRect->Y;
                for (int tile = 0; tile < unknown_int_ac0ff4; tile++) {
                    CC_Draw_Shape(surface, SidebarDrawer, (ShapeFileStruct const *)unknown_buffer_ac10a4, 0,
                                  where, view, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    where.X += ButtonSlotRect->Width;
                }
                for (int slot = 0; slot < 25; slot++) {
                    ShapeButtonClass *button = Get_Button(slot);
                    if (button != 0 && button->ShapeData != 0) {
                        button->Draw_Me(1);
                        button->IsDrawn = false;
                    }
                }
            }
            where.X = TabEndRect->X;
            CC_Draw_Shape(surface, SidebarDrawer, (ShapeFileStruct const *)unknown_buffer_ac10b0, 0,
                          where, view, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            if (IsThumbActive) {
                ThumbOpenButton.Draw_Me(1);
            } else {
                ThumbClosedButton.Draw_Me(1);
            }
            IsDrawn = true;
            unknown_4f15 = false;
            if (drawn_count < sizeof(drawn_on) / sizeof(drawn_on[0])) {
                drawn_count++;
            }
            for (int index = drawn_count - 2; index >= 0; index--) {
                drawn_on[index + 1] = drawn_on[index];
            }
            drawn_on[0] = surface;
        } else {
            IsDrawn = false;
        }
    }
    SidebarClass::Draw_It(complete);
}


// atexit destructor of TabClass::Draw_It's local static last_spot (0x006A1700, _$E):
// emitted by the definition in TabClass_Draw_It_006a12d0.cpp; no source of its own.


// void TabClass::Draw_Credits_Tab() (0x006A1710)
// Draws the bar's background shape at the origin of the sidebar surface
// (brightness 1000) and notes that a sidebar button repainted.

void TabClass::Draw_Credits_Tab()
{
    ShapeFileStruct const *shape = (ShapeFileStruct const *)unknown_buffer_ac10f8;
    CC_Draw_Shape(SidebarSurface, SidebarDrawer, shape, 0,
                  Point2D(0, 0), SidebarSurface->Get_Rect(), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    SidebarClass_IsToBlitSidebar = true;
}


// void TabClass::Flash_Money() (0x006A1770)
// Redraws the bar: sets +0x4F15 and restarts the +0x4F08 timer at 7 frames.

void TabClass::Flash_Money()
{
    unknown_4f15 = true;
    Flag_To_Redraw(false);
    MoneyFlashTimer = 7;
}


void TabClass::Setup_Command_Buttons()
{
    for (int index=0; index<25; index++) Buttons[index].Set_Shape(0,0,0);
    Rect area = *ThumbClosedRect;
    int right=area.X+area.Width;
    for (int slot=0;slot<25;slot++) {
        ShapeButtonClass *button=Get_Button(slot);
        if (button) {
            Rect box=*ButtonSlotRect;
            int x = SlotButton[slot]*TabClass_SlotPitch + box.X;
            int y = TabClass_SlotTop;
            if (box.Width+x<=right) {
                button->ID=slot+0xd6;
                button->ShapeDrawer=SidebarDrawer;
                button->IsOn=false;
                button->ToggleType=0;
                button->Set_Flags(5);
                button->Set_Position(x,y);
                button->Set_Shape(ButtonShapes[slot],0,0);
            }
        }
    }
    ShapeButtonClass *planning=Get_Button(TabClass_PlanningModeCommand);
    if (planning) {
        planning->ToggleType=1;
        planning->ReflectButtonState=true;
    }
    for (int team_slot=TabClass_Team01Command;team_slot<=TabClass_Team03Command;team_slot++) {
        ShapeButtonClass *button=Get_Button(team_slot);
        if (button) button->Set_Flags(0x55);
    }
}


// static void TabClass::Setup_Thumb_Buttons() (0x006A19E0)
// Sets up the two side buttons: ids 0xF0 and 0xF1 at the 0x00AC1204 and
// 0x00AC1208 rectangles with the 0x00AC1098 shape. Init_IO inlines it; this
// is the copy the linker kept.

void TabClass::Setup_Thumb_Buttons()
{
    ThumbOpenButton.ID = 0xF0;
    ThumbOpenButton.ShapeDrawer = SidebarDrawer;
    ThumbOpenButton.IsOn = false;
    ThumbOpenButton.ToggleType = 0;
    ThumbOpenButton.Flags = 5;
    void const *shape = unknown_buffer_ac1098;
    Rect area = *ThumbOpenRect;
    ThumbOpenButton.Set_Position(area.X, area.Y);
    ThumbOpenButton.Set_Shape(shape, 0, 0);
    ThumbClosedButton.ID = 0xF1;
    ThumbClosedButton.ShapeDrawer = SidebarDrawer;
    ThumbClosedButton.IsOn = false;
    ThumbClosedButton.ToggleType = 0;
    ThumbClosedButton.Flags = 5;
    Rect area2 = *ThumbClosedRect;
    ThumbClosedButton.Set_Position(area2.X, area2.Y);
    ThumbClosedButton.Set_Shape(shape, 0, 0);
}


void TabClass::Show_Buttons()
{
    Add_Command_Buttons();
    Add_A_Button(ThumbOpenButton);
    Add_Button_ToolTip(Get_Button(TabClass_AttackMoveCommand), "Tip:AttackMove");
    Add_Button_ToolTip(Get_Button(TabClass_BeaconCommand), "Tip:Beacon");
    Add_Button_ToolTip(Get_Button(TabClass_CheerCommand), "Tip:Cheer");
    Add_Button_ToolTip(Get_Button(TabClass_DeployCommand), "Tip:Deploy");
    Add_Button_ToolTip(Get_Button(TabClass_GuardCommand), "Tip:Guard");
    Add_Button_ToolTip(Get_Button(TabClass_PlanningModeCommand), "Tip:PlanningMode");
    {
        ShapeButtonClass *button = Get_Button(TabClass_StopCommand);
        Add_Button_ToolTip(button, "Tip:Stop");
    }
    {
        ShapeButtonClass *button = Get_Button(TabClass_Team01Command);
        Add_Button_ToolTip(button, "Tip:Team01");
    }
    {
        ShapeButtonClass *button = Get_Button(TabClass_Team02Command);
        Add_Button_ToolTip(button, "Tip:Team02");
    }
    {
        ShapeButtonClass *button = Get_Button(TabClass_Team03Command);
        Add_Button_ToolTip(button, "Tip:Team03");
    }
    {
        ShapeButtonClass *button = Get_Button(TabClass_TypeSelectCommand);
        Add_Button_ToolTip(button, "Tip:TypeSelect");
    }
    Add_Button_ToolTip(&ThumbOpenButton, "Tip:ThumbOpen");
}


// void TabClass::Hide_Buttons() (0x006A1DA0)
// Removes the open bar's 25 buttons and the extra one from the button list,
// and the help regions of the buttons in use and of the bar itself (0xF0).
// Removes the 25 buttons through 0x006A0D80 (inlined here; TabClass::AI's
// inlined copy of this function calls it out of line).

void TabClass::Hide_Buttons()
{
    Remove_Command_Buttons();
    Remove_A_Button(ThumbOpenButton);
    for (int slot = 0; slot < 25; slot++) {
        ShapeButtonClass *button = Get_Button(slot);
        if (button != 0) {
            ToolTips->Remove(button->ID);
        }
    }
    ToolTips->Remove(0xF0);
}


// void TabClass::Show_Thumb_Button() (0x006A1E20)
// Shows the closed bar's thumb button with its help region.

void TabClass::Show_Thumb_Button()
{
    Add_A_Button(ThumbClosedButton);
    Add_Button_ToolTip(&ThumbClosedButton, "Tip:ThumbClosed");
}


// void TabClass::Hide_Thumb_Button() (0x006A1EA0)
// Hides the closed bar's thumb button and its help region (0xF1).

void TabClass::Hide_Thumb_Button()
{
    Remove_A_Button(ThumbClosedButton);
    ToolTips->Remove(0xF1);
}


// void TabClass::Open_Thumb() (0x006A1EC0)
// Opens the bar when it is closed: swaps the thumb for the bar's buttons.

void TabClass::Open_Thumb()
{
    if (!IsThumbActive) {
        Hide_Thumb_Button();
        IsThumbActive = true;
        Show_Buttons();
        unknown_4f15 = true;
        Flag_To_Redraw(0);
    }
}


// void TabClass::Close_Thumb() (0x006A1F10)
// Closes the bar when it is open: swaps the bar's buttons for the thumb.

void TabClass::Close_Thumb()
{
    if (IsThumbActive) {
        Hide_Buttons();
        IsThumbActive = false;
        Show_Thumb_Button();
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
        ShapeButtonClass *button = Get_Button(TabClass_PlanningModeCommand);
        if (button != 0 && !button->IsOn) {
            button->Turn_On();
        }
        return true;
    }
    if (code == 0x11C8) {
        ShapeButtonClass *button = Get_Button(TabClass_PlanningModeCommand);
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
