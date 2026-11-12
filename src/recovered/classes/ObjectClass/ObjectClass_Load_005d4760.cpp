// HRESULT __stdcall ObjectClass::Load(IStream *stream) (0x005D4760)
#include "shared/object_type.hpp"
// @ref ?Load@AbstractClass@@UAGJPAUIStream@@@Z=00410120
// @ref ?Init@UnknownAudioController@@QAEXXZ=00405bf0
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20
// @ref ?Swizzler@@3VSwizzleManagerClass@@A=00abd868

HRESULT __stdcall ObjectClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    Swizzler.Swizzle((void **)&unknown_30);
    Swizzler.Swizzle((void **)&unknown_34);
    Swizzler.Swizzle((void **)&unknown_38);
    Swizzler.Swizzle((void **)&unknown_18);
    unknown_3c.Init();
    unknown_50.Init();
    unknown_94 = 0;
    return S_OK;
}
