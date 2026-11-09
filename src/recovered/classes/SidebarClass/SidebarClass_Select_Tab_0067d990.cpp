// void SidebarClass::Select_Tab(int index) (0x0067D990)
// Switches to another tab: releases the old tab button and hides its strip,
// then presses the new tab button, shows its strip and refreshes the scroll buttons.
#include "shared/sidebar.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Turn_Off@ToggleClass@@QAEXXZ=006eb170
// @ref ?Turn_On@ToggleClass@@QAEXXZ=006eb160
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ?unknown_int_83962c@@3HA=0083962c
// @ref ?unknown_int_839624@@3HA=00839624
// @ref ?unknown_abcd64@@3HA=00abcd64

void SidebarClass::Select_Tab(int index)
{
    if (index == unknown_4ed8) {
        return;
    }
    ShapeButtonClass &old_button = unknown_buttons_ab9508[unknown_4ed8];
    old_button.Turn_Off();
    StripClass &old = Column[unknown_4ed8];
    old.Unknown_0067F730();
    unknown_4ed8 = index;
    ShapeButtonClass &button = unknown_buttons_ab9508[unknown_4ed8];
    button.Turn_On();
    StripClass &current = Column[unknown_4ed8];
    current.Activate();
    Unknown_0067C850();
    unknown_4ee3 = true;
}
