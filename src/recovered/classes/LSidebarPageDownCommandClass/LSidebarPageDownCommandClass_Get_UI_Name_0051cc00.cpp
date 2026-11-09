// wchar_t const *LSidebarPageDownCommandClass::Get_UI_Name() const -- 0x0051CC00
// The localized name (TXT_LSIDEBAR_PGDN).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@KJAG@TXT_LSIDEBAR_PGDN?$AA@=007dd1fc

wchar_t const *LSidebarPageDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_PGDN", 0, "D:\\Ra2\\Init.CPP", 7290);
}
