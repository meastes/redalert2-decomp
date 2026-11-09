// wchar_t const *StopCommandClass::Get_Description() const -- 0x0051BE00
// The localized description (TXT_STOP_OBJECT_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BF@LCKA@TXT_STOP_OBJECT_DESC?$AA@=007dcd54

wchar_t const *StopCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_STOP_OBJECT_DESC", 0, "D:\\Ra2\\Init.CPP", 6607);
}
