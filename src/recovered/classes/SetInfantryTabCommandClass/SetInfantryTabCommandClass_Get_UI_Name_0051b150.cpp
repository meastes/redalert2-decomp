// wchar_t const *SetInfantryTabCommandClass::Get_UI_Name() const -- 0x0051B150
// The localized name (TXT_INFANTRY_TAB).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BB@CCFA@TXT_INFANTRY_TAB?$AA@=007dc9a4

wchar_t const *SetInfantryTabCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_INFANTRY_TAB", 0, "D:\\Ra2\\Init.CPP", 5983);
}
