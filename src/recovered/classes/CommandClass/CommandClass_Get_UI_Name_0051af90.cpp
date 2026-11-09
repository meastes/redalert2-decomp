// wchar_t const *CommandClass::Get_UI_Name() const -- 0x0051AF90
// The command's display name: by default its description (slot 4).
#include "shared/command.hpp"

wchar_t const *CommandClass::Get_UI_Name() const
{
    return Get_Description();
}
