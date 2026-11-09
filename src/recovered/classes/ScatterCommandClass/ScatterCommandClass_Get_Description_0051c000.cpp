// wchar_t const *ScatterCommandClass::Get_Description() const -- 0x0051C000
// The localized description (TXT_SCATTER_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BB@MGJC@TXT_SCATTER_DESC?$AA@=007dce1c

wchar_t const *ScatterCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SCATTER_DESC", 0, "D:\\Ra2\\Init.CPP", 6709);
}
