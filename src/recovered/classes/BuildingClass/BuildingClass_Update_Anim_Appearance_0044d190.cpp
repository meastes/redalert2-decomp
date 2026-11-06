// void BuildingClass::Update_Anim_Appearance_0044d190() (0x0044D190)
// Gives every animation in unknown_454 whose type has +0x318 set the slot
// 115 value and the frame (the height level of its cell, slot 254, adjusted
// by 0x006D86F0 when slot 203 holds; also kept in unknown_5c4), then the
// building's layer. Written as ra2-decomp's Update_Anim_Appearance (YR 0x00452000): the drawer loop is the
// Set_Anim_Drawer call 0x0044D0F0 and it is passed the stored member (Brightness); that smaller body lets Mark
// 0x0043BFC0 expand it.
#include "shared/building.hpp"
// @ref ?unknown_6d86f0@TechnoClass@@QAEHH@Z=006d86f0

void BuildingClass::Update_Anim_Appearance_0044d190()
{
    int value = unknown_slot115();
    int height = ((UnknownBuildingSlot105 *)unknown_slot105())->unknown_10a;
    int level = unknown_slot254(height);
    if (unknown_slot203()) {
        level = unknown_6d86f0(level);
    }
    unknown_5c4 = level;
    unknown_44d0f0(value, (unsigned short)unknown_5c4);
    unknown_44d2d0(unknown_5bd);
}
