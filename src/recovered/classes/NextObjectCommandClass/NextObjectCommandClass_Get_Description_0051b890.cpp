// wchar_t const *NextObjectCommandClass::Get_Description() const -- 0x0051B890
// The localized description (TXT_NEXT_OBJECT_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BF@MPOF@TXT_NEXT_OBJECT_DESC?$AA@=007dcbb0

wchar_t const *NextObjectCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_NEXT_OBJECT_DESC", 0, "D:\\Ra2\\Init.CPP", 6275);
}
