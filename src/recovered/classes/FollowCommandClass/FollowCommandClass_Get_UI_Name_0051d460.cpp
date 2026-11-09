// wchar_t const *FollowCommandClass::Get_UI_Name() const -- 0x0051D460
// The localized name (TXT_FOLLOW).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0L@CDFC@TXT_FOLLOW?$AA@=007dd43c

wchar_t const *FollowCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_FOLLOW", 0, "D:\\Ra2\\Init.CPP", 7764);
}
