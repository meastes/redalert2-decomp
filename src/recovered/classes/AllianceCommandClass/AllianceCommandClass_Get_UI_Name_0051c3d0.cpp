// wchar_t const *AllianceCommandClass::Get_UI_Name() const -- 0x0051C3D0
// The localized name (TXT_ALLIANCE).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0N@DLHJ@TXT_ALLIANCE?$AA@=007dcea8

wchar_t const *AllianceCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_ALLIANCE", 0, "D:\\Ra2\\Init.CPP", 6848);
}
