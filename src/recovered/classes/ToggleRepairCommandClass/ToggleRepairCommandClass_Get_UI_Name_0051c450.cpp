// wchar_t const *ToggleRepairCommandClass::Get_UI_Name() const -- 0x0051C450
// The localized name (TXT_REPAIR_MODE).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@IOHI@TXT_REPAIR_MODE?$AA@=007dcedc

wchar_t const *ToggleRepairCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_REPAIR_MODE", 0, "D:\\Ra2\\Init.CPP", 6906);
}
