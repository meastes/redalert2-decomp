// HRESULT __stdcall TriggerTypeClass::Load(IStream *stream) (0x006EEE10)
#include "shared/object.hpp"

HRESULT __stdcall TriggerTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TriggerTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&LinkedTo);
    Swizzler.Swizzle((void **)&FirstEvent);
    Swizzler.Swizzle((void **)&FirstAction);
    Swizzler.Swizzle((void **)&House);
    return S_OK;
}
