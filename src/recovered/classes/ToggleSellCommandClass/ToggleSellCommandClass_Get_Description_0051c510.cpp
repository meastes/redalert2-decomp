// wchar_t const *ToggleSellCommandClass::Get_Description() const -- 0x0051C510
// The localized description (TXT_SELL_MODE_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BD@OLEG@TXT_SELL_MODE_DESC?$AA@=007dcf20

wchar_t const *ToggleSellCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SELL_MODE_DESC", 0, "D:\\Ra2\\Init.CPP", 6936);
}
