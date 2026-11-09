// wchar_t const *ToggleSellCommandClass::Get_UI_Name() const -- 0x0051C4D0
// The localized name (TXT_SELL_MODE).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0O@IOEL@TXT_SELL_MODE?$AA@=007dcf10

wchar_t const *ToggleSellCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SELL_MODE", 0, "D:\\Ra2\\Init.CPP", 6932);
}
