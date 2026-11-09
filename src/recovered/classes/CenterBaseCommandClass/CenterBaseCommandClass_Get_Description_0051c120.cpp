// wchar_t const *CenterBaseCommandClass::Get_Description() const -- 0x0051C120
// The localized description (TXT_CENTER_BASE_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BF@FDCM@TXT_CENTER_BASE_DESC?$AA@=007dce80

wchar_t const *CenterBaseCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_CENTER_BASE_DESC", 0, "D:\\Ra2\\Init.CPP", 6767);
}
