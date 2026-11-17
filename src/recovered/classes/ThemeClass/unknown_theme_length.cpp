// int ThemeClass::unknown_theme_length(unsigned theme) const -- 0x006E8150
// Purpose: the +0x284 value (a float, truncated) of theme `theme`'s
// control record, 0 for an unknown theme.
// Assumptions: the themes are a DynamicVectorClass of control records at
// +0x14 (elements +0x18, count +0x24); +0x284 is taken to be the length.





#define THEME_CONTROL_RECORD
#define THEME_SOUND_CONTROLS
#include "shared/theme.hpp"

int ThemeClass::unknown_theme_length(unsigned theme) const
{
    if (theme < Themes.Count()) {
        return (int)Themes[theme]->Duration;
    }
    return 0;
}
