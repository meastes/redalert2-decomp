// wchar_t const *AllToCheerCommandClass::Get_UI_Name() const -- 0x0051BE40
// The localized name (Cmnd:AllCheer).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0O@MKAO@Cmnd?3AllCheer?$AA@=007dcd78

wchar_t const *AllToCheerCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:AllCheer", 0, "D:\\Ra2\\Init.CPP", 6621);
}
