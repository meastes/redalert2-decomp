// HRESULT __stdcall TriggerTypeClass::Load(IStream *stream) (0x006EEE10)
#include "shared/object.hpp"

HRESULT __stdcall TriggerTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TriggerTypeClass(NoInitClass());
    SwizzleManager.Swizzle((void **)&LinkedTo);
    SwizzleManager.Swizzle((void **)&FirstEvent);
    SwizzleManager.Swizzle((void **)&FirstAction);
    SwizzleManager.Swizzle((void **)&House);
    return S_OK;
}
