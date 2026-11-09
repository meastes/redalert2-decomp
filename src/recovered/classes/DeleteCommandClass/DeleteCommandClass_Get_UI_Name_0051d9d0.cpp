// wchar_t const *DeleteCommandClass::Get_UI_Name() const -- 0x0051D9D0
// The localized name (Cmnd:Delete).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0M@BFMM@Cmnd?3Delete?$AA@=007dd6d4

wchar_t const *DeleteCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:Delete", 0, "D:\\Ra2\\Init.CPP", 6304);
}
