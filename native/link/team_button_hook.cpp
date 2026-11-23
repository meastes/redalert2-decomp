// The touch host's question (platform/sdl/host.cpp ra2_native_team_button_at; lane Y15): which
// command-bar team button (1-3) is under a game pixel, else 0. A hold there sets the team
// (platform/sdl/touch.cpp): the bar's own buttons, TabClass::Get_Button (0x006A0600) for the
// Team01..Team03 commands, while the bar is open (IsThumbActive; Close_Thumb removes them).
// The YR game's answer is native/yr/glue/yr_app_hooks.cpp's.
#include "shared/mouse_class.hpp"
#include "shared/gscreen.hpp"
#include <stdio.h>

extern MouseClass Map;                 // 0x008324E0 (as shared/map_view.hpp declares it)

extern "C" int ra2_native_team_button_at(int x, int y)
{
    if (!Map.IsThumbActive) return 0;
    for (int slot = TabClass_Team01Command; slot >= 0 && slot <= TabClass_Team03Command; slot++) {
        ShapeButtonClass *button = TabClass::Get_Button(slot);
        if (button && !button->IsDisabled && x >= button->X && x < button->X + button->Width &&
            y >= button->Y && y < button->Y + button->Height) {
            return TabClass::Team_Number(slot);
        }
    }
    return 0;
}

// Test scaffolding (lane N10): an input script's point "@cmd=<command>" is the middle of the command
// bar's button for that command (harness_input.cpp asks it; as the YR game's glue/yr_app_hooks.cpp).
// 1 when found; 0 when the spec is not "cmd=" or the bar is closed or has no such button (logged).
static bool Same_Text(char const *a, char const *b)
{
    while (*a && *a == *b) ++a, ++b;
    return *a == *b;
}
int Native_Harness_Bar_Point(char const *spec, int *x, int *y)
{
    if (spec[0] != 'c' || spec[1] != 'm' || spec[2] != 'd' || spec[3] != '=') return 0;
    static struct { char const *name; int const *slot; } const commands[] = {
        {"AttackMove", &TabClass_AttackMoveCommand}, {"Beacon", &TabClass_BeaconCommand},
        {"Cheer", &TabClass_CheerCommand}, {"Deploy", &TabClass_DeployCommand}, {"Guard", &TabClass_GuardCommand},
        {"PlanningMode", &TabClass_PlanningModeCommand}, {"Stop", &TabClass_StopCommand},
        {"TypeSelect", &TabClass_TypeSelectCommand}};
    int slot = -1;
    for (unsigned i = 0; i < sizeof commands / sizeof commands[0]; ++i)
        if (Same_Text(spec + 4, commands[i].name)) slot = *commands[i].slot;
    ShapeButtonClass *button = Map.IsThumbActive && slot >= 0 ? TabClass::Get_Button(slot) : 0;
    if (button == 0 || button->IsDisabled) {
        fprintf(stderr, "input-script: no command bar button for %s (slot %d); the bar's buttons:", spec, slot);
        for (unsigned i = 0; i < sizeof commands / sizeof commands[0]; ++i) {
            ShapeButtonClass *other = Map.IsThumbActive ? TabClass::Get_Button(*commands[i].slot) : 0;
            if (other && !other->IsDisabled) fprintf(stderr, " %s", commands[i].name);
        }
        fprintf(stderr, "\n");
        return 0;
    }
    *x = button->X + button->Width / 2;
    *y = button->Y + button->Height / 2;
    return 1;
}

// Test scaffolding (lane N31): the command bar's thumb. ThumbOpenButton (0xF0, the close thumb at the left end of
// the bar's buttons) is in the gadget list while the bar is open, ThumbClosedButton (0xF1, the open thumb at the
// bar's right end) while it is closed (TabClass::AI 0x006A0F30, unknown_slot54 0x006A0DA0). "@thumb" (harness_input.cpp)
// is the middle of the one shown now; Native_Harness_Thumb logs both (the harness order `thumb`, harness_orders.cpp)
// as `harness-thumb open|closed close X,Y WxH[ listed] open X,Y WxH[ listed] bar X,Y WxH end X,Y` (the bar and its end
// piece as the art layout placed them: Draw_It 0x006A12D0 draws there).
static bool In_Gadget_List(GadgetClass const *gadget)
{
    for (GadgetClass *g = GScreenClass::Buttons; g != 0; g = (GadgetClass *)g->Get_Next())
        if (g == gadget) return true;
    return false;
}
int Native_Harness_Thumb_Point(int *x, int *y)
{
    ShapeButtonClass &button = Map.IsThumbActive ? TabClass::ThumbOpenButton : TabClass::ThumbClosedButton;
    if (!In_Gadget_List(&button)) {
        fprintf(stderr, "input-script: the bar's %s thumb is not a button now\n", Map.IsThumbActive ? "close" : "open");
        return 0;
    }
    *x = button.X + button.Width / 2;
    *y = button.Y + button.Height / 2;
    return 1;
}
int Native_Harness_Thumb(void)
{
    ShapeButtonClass const &a = TabClass::ThumbOpenButton;
    ShapeButtonClass const &b = TabClass::ThumbClosedButton;
    Rect bar = TabBarRect ? *TabBarRect : Rect(0, 0, 0, 0);
    Rect end = TabEndRect ? *TabEndRect : Rect(0, 0, 0, 0);
    fprintf(stderr, "harness-thumb %s close %d,%d %dx%d%s open %d,%d %dx%d%s bar %d,%d %dx%d end %d,%d\n", Map.IsThumbActive ? "open" : "closed",
            a.X, a.Y, a.Width, a.Height, In_Gadget_List(&a) ? " listed" : "",
            b.X, b.Y, b.Width, b.Height, In_Gadget_List(&b) ? " listed" : "", bar.X, bar.Y, bar.Width, bar.Height, end.X, end.Y);
    return 1;
}
