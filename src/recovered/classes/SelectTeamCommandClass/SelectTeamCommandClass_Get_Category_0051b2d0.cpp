// wchar_t const *SelectTeamCommandClass::Get_Category() const -- 0x0051B2D0
// The localized category (TXT_TEAM).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_08NJIF@TXT_TEAM?$AA@=007dc8f4

wchar_t const *SelectTeamCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_TEAM", 0, "D:\\Ra2\\Init.CPP", 6039);
}
