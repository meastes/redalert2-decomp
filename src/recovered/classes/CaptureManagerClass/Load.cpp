// ?Load@CaptureManagerClass@@UAGJPAUIStream@@@Z (0x00468280)
// AbstractClass load, vtables, pointer fixups.
#include "shared/capture_manager.hpp"

HRESULT __stdcall CaptureManagerClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) CaptureManagerClass(NoInitClass());
        Swizzler.Swizzle((void **)&Unit);
        Swizzler.Swizzle((void **)&unknown_28);
        Swizzler.Swizzle((void **)&Owner);
    }
    return result;
}
