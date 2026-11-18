// The touch host's question (platform/sdl/host.cpp ra2_native_team_button_at; lane Y15): which
// command-bar team button (1-3) is under a game pixel, else 0. A hold there sets the team
// (platform/sdl/touch.cpp): the bar's own buttons, TabClass::Get_Button (0x006A0600) for the
// Team01..Team03 commands, while the bar is open (IsThumbActive; Close_Thumb removes them).
// The YR game's answer is native/yr/glue/yr_app_hooks.cpp's.
#include "shared/mouse_class.hpp"

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
