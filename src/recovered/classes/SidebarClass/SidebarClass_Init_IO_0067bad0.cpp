// void SidebarClass::Init_IO() (0x0067BAD0, slot 8)
// Places the sidebar, configures its buttons (the two top buttons, the four
// tab buttons and the two scroll buttons) and the four strips.
#include "shared/sidebar.hpp"
// @ref ?Init_IO@RadarClass@@UAEXXZ=0062f490
// @ref ?Unknown_0067B910@SidebarClass@@QAEXXZ=0067b910
// @ref ?Debug_Map@@3_NA=00a40d13
// @ref ?unknown_drawer_8323cc@@3PAVConvertClass@@A=008323cc
// @ref ?unknown_abcd4c@@3HA=00abcd4c
// @ref ?unknown_abcd50@@3HA=00abcd50
// @ref ?unknown_abcd58@@3HA=00abcd58
// @ref ?unknown_abcd5c@@3HA=00abcd5c
// @ref ?unknown_int_abcd70@@3HA=00abcd70
// @ref ?Stop_Flashing@ShapeButtonClass@@QAE_NXZ=00675300
// @ref ?Disable@GadgetClass@@UAEXXZ=004cf150
// @ref ?Unknown_0067E7E0@StripClass@SidebarClass@@QAEXH@Z=0067e7e0
// @ref ?unknown_flag_a40b30@@3_NA=00a40b30
// @ref ?unknown_int_839620@@3HA=00839620
// @ref ?unknown_int_839630@@3HA=00839630
// @ref ?unknown_int_839638@@3HA=00839638
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ?unknown_abcd48@@3HA=00abcd48
// @ref ?unknown_abcd54@@3HA=00abcd54
// @ref ?unknown_abcd60@@3HA=00abcd60
// @ref ?unknown_abcd64@@3HA=00abcd64
// @ref ?unknown_abcd74@@3HA=00abcd74

void SidebarClass::Init_IO()
{
    RadarClass::Init_IO();
    Unknown_0067B910();
    Unknown_0067BA60();
    if (Debug_Map) {
        return;
    }

    unknown_button_abcc58.IsSticky = true;
    unknown_button_abcc58.ID = 101;
    unknown_button_abcc58.X = unknown_abcd48;
    unknown_button_abcc58.Y = unknown_abcd4c;
    unknown_button_abcc58.DrawOffsetX = -480;
    unknown_button_abcc58.DrawOffsetY = 0;
    unknown_button_abcc58.DrawOnSidebar = true;
    unknown_button_abcc58.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_abcc58.IsOn = false;
    unknown_button_abcc58.ToggleType = 1;
    unknown_button_abcc58.ReflectButtonState = true;
    unknown_button_abcc58.Stop_Flashing();

    unknown_button_ab96b8.IsSticky = true;
    unknown_button_ab96b8.ID = 102;
    unknown_button_ab96b8.X = unknown_button_abcc58.X + unknown_abcd50;
    unknown_button_ab96b8.Y = unknown_button_abcc58.Y;
    unknown_button_ab96b8.DrawOffsetX = -480;
    unknown_button_ab96b8.DrawOffsetY = 0;
    unknown_button_ab96b8.DrawOnSidebar = true;
    unknown_button_ab96b8.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_ab96b8.IsOn = false;
    unknown_button_ab96b8.ToggleType = 1;
    unknown_button_ab96b8.ReflectButtonState = true;
    unknown_button_ab96b8.Stop_Flashing();

    for (int index = 0; index < 4; index++) {
        unknown_buttons_ab9508[index].IsSticky = true;
        unknown_buttons_ab9508[index].ID = 203 + index;
        unknown_buttons_ab9508[index].X = unknown_abcd5c * index + unknown_abcd54;
        unknown_buttons_ab9508[index].Y = unknown_abcd58;
        unknown_buttons_ab9508[index].DrawOffsetX = -480;
        unknown_buttons_ab9508[index].DrawOffsetY = 0;
        unknown_buttons_ab9508[index].DrawOnSidebar = true;
        unknown_buttons_ab9508[index].ShapeDrawer = unknown_drawer_8323cc;
        unknown_buttons_ab9508[index].IsOn = false;
        unknown_buttons_ab9508[index].ToggleType = 2;
        unknown_buttons_ab9508[index].ReflectButtonState = true;
        unknown_buttons_ab9508[index].Disable();
        unknown_buttons_ab9508[index].Stop_Flashing();
    }

    unknown_button_abcbe8.IsSticky = true;
    unknown_button_abcbe8.ID = 201;
    unknown_button_abcbe8.DrawOnSidebar = true;
    unknown_button_abcbe8.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_abcbe8.Flags = 0x55;
    unknown_button_abcbe8.Stop_Flashing();

    unknown_button_abccc0.IsSticky = true;
    unknown_button_abccc0.ID = 200;
    unknown_button_abccc0.DrawOnSidebar = true;
    unknown_button_abccc0.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_abccc0.Flags = 0x55;
    unknown_button_abccc0.Stop_Flashing();

    for (int column = 0; column < 4; column++) {
        Column[column].unknown_1c = unknown_abcd60;
        Column[column].unknown_20 = unknown_abcd64;
        Column[column].unknown_24 = Rect(0, 0, 60, unknown_int_abcd70);
        Column[column].Unknown_0067E7E0(column);
    }
    Reposition_Sidebar();
    if (unknown_4ee1) {
        unknown_slot54(0);
        unknown_slot54(1);
    }
}
