// wchar_t const *DeleteCommandClass::Get_Description() const -- 0x0051DA10
// The localized description (Cmnd:DeleteDesc).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@FELI@Cmnd?3DeleteDesc?$AA@=007dd6e0

wchar_t const *DeleteCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:DeleteDesc", 0, "D:\\Ra2\\Init.CPP", 6308);
}
