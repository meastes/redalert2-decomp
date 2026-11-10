// ThemeClass::Name (0x006E8570): the theme's INI name, "none" out of range.
#define THEME_CONTROL_RECORD
#include "shared/theme.hpp"
// @ref ??_C@_04KIKH@none?$AA@=007ce0ec

char const *ThemeClass::Name(int theme) const
{
    if (theme >= 0 && theme < Themes.Count()) {
        return Themes[theme]->Name;
    }
    return "none";
}
