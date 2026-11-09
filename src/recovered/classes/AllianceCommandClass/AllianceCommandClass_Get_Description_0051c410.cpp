// wchar_t const *AllianceCommandClass::Get_Description() const -- 0x0051C410
// The localized description (TXT_ALLIANCE_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@LENP@TXT_ALLIANCE_DESC?$AA@=007dceb8

wchar_t const *AllianceCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_ALLIANCE_DESC", 0, "D:\\Ra2\\Init.CPP", 6852);
}
