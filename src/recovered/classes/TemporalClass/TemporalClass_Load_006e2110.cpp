// ?Load@TemporalClass@@UAGJPAUIStream@@@Z (0x006E2110)
// AbstractClass load, vtables, pointer fixups.
#include "shared/temporal.hpp"
// @ref ?Load@AbstractClass@@UAGJPAUIStream@@@Z=00410120
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20
// @ref ?Swizzler@@3VSwizzleManagerClass@@A=00abd868

HRESULT __stdcall TemporalClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TemporalClass(NoInitClass());
        Swizzler.Swizzle((void **)&Owner);
        Swizzler.Swizzle((void **)&Target);
        Swizzler.Swizzle((void **)&unknown_40);
        Swizzler.Swizzle((void **)&unknown_44);
        Swizzler.Swizzle((void **)&unknown_38);
        Swizzler.Swizzle((void **)&unknown_3c);
    }
    return result;
}
