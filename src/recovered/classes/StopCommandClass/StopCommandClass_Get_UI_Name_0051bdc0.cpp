// wchar_t const *StopCommandClass::Get_UI_Name() const -- 0x0051BDC0
// The localized name (TXT_STOP_OBJECT).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@BBLC@TXT_STOP_OBJECT?$AA@=007dcd44

wchar_t const *StopCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_STOP_OBJECT", 0, "D:\\Ra2\\Init.CPP", 6603);
}
