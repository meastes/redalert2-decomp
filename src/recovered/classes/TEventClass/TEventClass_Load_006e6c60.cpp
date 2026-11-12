// HRESULT __stdcall TEventClass::Load(IStream *stream) (0x006E6C60)
#include "shared/object.hpp"

HRESULT __stdcall TEventClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TEventClass(NoInitClass());
    Swizzler.Swizzle((void **)&Next);
    Swizzler.Swizzle((void **)&unknown_30);
    return S_OK;
}
