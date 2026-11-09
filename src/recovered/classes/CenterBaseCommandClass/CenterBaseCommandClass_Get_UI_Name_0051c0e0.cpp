// wchar_t const *CenterBaseCommandClass::Get_UI_Name() const -- 0x0051C0E0
// The localized name (TXT_CENTER_BASE).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@PEOO@TXT_CENTER_BASE?$AA@=007dce70

wchar_t const *CenterBaseCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_CENTER_BASE", 0, "D:\\Ra2\\Init.CPP", 6763);
}
