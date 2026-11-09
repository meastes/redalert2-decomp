// wchar_t const *SetUnitTabCommandClass::Get_UI_Name() const -- 0x0051B1E0
// The localized name (TXT_UNIT_TAB).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0N@OJPB@TXT_UNIT_TAB?$AA@=007dc9d8

wchar_t const *SetUnitTabCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_UNIT_TAB", 0, "D:\\Ra2\\Init.CPP", 6004);
}
