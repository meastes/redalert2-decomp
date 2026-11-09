// wchar_t const *SetStructureTabCommandClass::Get_UI_Name() const -- 0x0051AFD0
// The localized name (TXT_STRUCTURE_TAB).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@CJML@TXT_STRUCTURE_TAB?$AA@=007dc928

wchar_t const *SetStructureTabCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_STRUCTURE_TAB", 0, "D:\\Ra2\\Init.CPP", 5931);
}
