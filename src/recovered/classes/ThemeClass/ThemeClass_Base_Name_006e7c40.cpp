// ThemeClass::Base_Name (0x006E7C40): the theme's INI name, "No theme" out of range (TibSun ThemeClass::Base_Name).
#define THEME_CONTROL_RECORD
#include "shared/theme.hpp"
// @ref ??_C@_08HDLJ@No?5theme?$AA@=007f8340

char const *ThemeClass::Base_Name(int theme) const
{
    if ((unsigned)theme < (unsigned)Themes.Count()) {
        return Themes[theme]->Name;
    }
    return "No theme";
}
