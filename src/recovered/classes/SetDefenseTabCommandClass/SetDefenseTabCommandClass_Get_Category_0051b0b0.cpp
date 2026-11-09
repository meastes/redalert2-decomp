// wchar_t const *SetDefenseTabCommandClass::Get_Category() const -- 0x0051B0B0
// The localized category (TXT_INTERFACE).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0O@HJCC@TXT_INTERFACE?$AA@=007dc93c

wchar_t const *SetDefenseTabCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 5959);
}
