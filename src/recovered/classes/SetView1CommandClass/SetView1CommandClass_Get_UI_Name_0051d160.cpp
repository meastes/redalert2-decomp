// wchar_t const *SetView1CommandClass::Get_UI_Name() const -- 0x0051D160
// The localized name (TXT_SET_BOOKMARK1).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@ILAO@TXT_SET_BOOKMARK1?$AA@=007dd368

wchar_t const *SetView1CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK1", 0, "D:\\Ra2\\Init.CPP", 7656);
}
