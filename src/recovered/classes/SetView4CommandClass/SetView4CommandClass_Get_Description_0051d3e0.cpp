// wchar_t const *SetView4CommandClass::Get_Description() const -- 0x0051D3E0
// The localized description (TXT_SET_BOOKMARK4_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BH@EHIN@TXT_SET_BOOKMARK4_DESC?$AA@=007dd424

wchar_t const *SetView4CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK4_DESC", 0, "D:\\Ra2\\Init.CPP", 7741);
}
