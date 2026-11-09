// wchar_t const *CursorPositionCommandClass::Get_Category() const -- 0x0051D950
// The localized category (GUI:Debug).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_09PEOJ@GUI?3Debug?$AA@=007dd688

wchar_t const *CursorPositionCommandClass::Get_Category() const
{
    return StringTable::Load_String("GUI:Debug", 0, "D:\\Ra2\\Init.CPP", 8041);
}
