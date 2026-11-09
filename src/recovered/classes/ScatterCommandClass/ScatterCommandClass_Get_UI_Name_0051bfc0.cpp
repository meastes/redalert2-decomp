// wchar_t const *ScatterCommandClass::Get_UI_Name() const -- 0x0051BFC0
// The localized name (TXT_SCATTER).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0M@NLJG@TXT_SCATTER?$AA@=007dce10

wchar_t const *ScatterCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SCATTER", 0, "D:\\Ra2\\Init.CPP", 6705);
}
