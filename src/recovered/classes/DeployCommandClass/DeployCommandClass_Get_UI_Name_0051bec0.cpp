// wchar_t const *DeployCommandClass::Get_UI_Name() const -- 0x0051BEC0
// The localized name (TXT_DEPLOY_OBJECT).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@EAHD@TXT_DEPLOY_OBJECT?$AA@=007dcdac

wchar_t const *DeployCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_DEPLOY_OBJECT", 0, "D:\\Ra2\\Init.CPP", 6648);
}
