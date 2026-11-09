// wchar_t const *BeaconPlacementCommandClass::Get_UI_Name() const -- 0x0051C550
// The localized name (TXT_PLACE_BEACON).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BB@EEGD@TXT_PLACE_BEACON?$AA@=007dcf34

wchar_t const *BeaconPlacementCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_PLACE_BEACON", 0, "D:\\Ra2\\Init.CPP", 6975);
}
