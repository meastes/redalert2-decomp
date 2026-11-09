// wchar_t const *CursorPositionCommandClass::Get_UI_Name() const -- 0x0051D930
// The localized name (GUI:CursorCheat).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@GOFB@GUI?3CursorCheat?$AA@=007dd678

wchar_t const *CursorPositionCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("GUI:CursorCheat", 0, "D:\\Ra2\\Init.CPP", 8040);
}
