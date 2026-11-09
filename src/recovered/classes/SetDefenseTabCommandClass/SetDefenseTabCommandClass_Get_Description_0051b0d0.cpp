// wchar_t const *SetDefenseTabCommandClass::Get_Description() const -- 0x0051B0D0
// The localized description (TXT_DEFENSE_TAB_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BF@BGMI@TXT_DEFENSE_TAB_DESC?$AA@=007dc980

wchar_t const *SetDefenseTabCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_DEFENSE_TAB_DESC", 0, "D:\\Ra2\\Init.CPP", 5961);
}
