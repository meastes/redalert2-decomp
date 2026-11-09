// wchar_t const *CenterREventCommandClass::Get_UI_Name() const -- 0x0051C5E0
// The localized name (TXT_RADAR_EVENT).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@OALH@TXT_RADAR_EVENT?$AA@=007dcf74

wchar_t const *CenterREventCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RADAR_EVENT", 0, "D:\\Ra2\\Init.CPP", 6998);
}
