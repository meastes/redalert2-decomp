// HRESULT __stdcall AbstractClass::Load(IStream *stream) (0x00410120)
// IPersistStream: reads the saved pointer, registers it with the swizzle manager, then reads the object over itself keeping RefCount.
#include "shared/abstract.hpp"
// @ref ?Here_I_Am@SwizzleManagerClass@@UAGJJPAX@Z=0069fba0
// @ref ?Swizzler@@3VSwizzleManagerClass@@A=00abd868

HRESULT __stdcall AbstractClass::Load(IStream *stream)
{
    if (stream == 0) {
        return E_POINTER;
    }
    LONG_PTR id;
    HRESULT result = stream->Read(&id, sizeof(id), 0);
    if (SUCCEEDED(result)) {
        Swizzler.Here_I_Am(id, this);
        long refcount = RefCount;
        result = stream->Read(this, Size_Of(), 0);
        RefCount = refcount;
    }
    return result;
}
