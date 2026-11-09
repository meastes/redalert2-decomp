// wchar_t const *TypeSelectCommandClass::Get_Description() const -- 0x0051BB00
// The localized description (Cmnd:TypeSelectDesc).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BE@MLKH@Cmnd?3TypeSelectDesc?$AA@=007dcc70

wchar_t const *TypeSelectCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:TypeSelectDesc", 0, "D:\\Ra2\\Init.CPP", 6439);
}
