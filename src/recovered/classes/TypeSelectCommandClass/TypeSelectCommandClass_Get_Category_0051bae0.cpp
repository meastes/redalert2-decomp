// wchar_t const *TypeSelectCommandClass::Get_Category() const -- 0x0051BAE0
// The localized category (TXT_SELECTION).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0O@KGDO@TXT_SELECTION?$AA@=007dcba0

wchar_t const *TypeSelectCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6437);
}
