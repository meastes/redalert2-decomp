// HRESULT __stdcall TriggerClass::Load(IStream *stream) (0x006EDAD0)
#include "shared/object.hpp"

HRESULT __stdcall TriggerClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TriggerClass(NoInitClass());
    Swizzler.Swizzle((void **)&Class);
    Swizzler.Swizzle((void **)&LinkedTo);
    return S_OK;
}
