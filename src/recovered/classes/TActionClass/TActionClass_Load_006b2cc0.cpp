// HRESULT __stdcall TActionClass::Load(IStream *stream) (0x006B2CC0)
#include "shared/object.hpp"

HRESULT __stdcall TActionClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TActionClass(NoInitClass());
    Swizzler.Swizzle((void **)&Next);
    Swizzler.Swizzle((void **)&unknown_30);
    Swizzler.Swizzle((void **)&unknown_4c);
    Swizzler.Swizzle((void **)&unknown_50);
    return S_OK;
}


