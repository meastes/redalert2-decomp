// wchar_t const *TabClass::Help_Text(int id) (0x006A20B0, slot 33): the tooltip text of
// a control: SidebarClass's (0x00681ED0, called directly).
#include "shared/tab.hpp"

wchar_t const *TabClass::Help_Text(int id)
{
    return SidebarClass::Help_Text(id);
}
