// wchar_t const *CursorPositionCommandClass::Get_Description() const -- 0x0051D970
// The localized description (DESC:CursorCheat).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BB@PEOA@DESC?3CursorCheat?$AA@=007dd694

wchar_t const *CursorPositionCommandClass::Get_Description() const
{
    return StringTable::Load_String("DESC:CursorCheat", 0, "D:\\Ra2\\Init.CPP", 8042);
}
