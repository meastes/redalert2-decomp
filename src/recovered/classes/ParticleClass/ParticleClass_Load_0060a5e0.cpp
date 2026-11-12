// HRESULT __stdcall ParticleClass::Load(IStream *stream) (0x0060A5E0)
#include "shared/particle.hpp"
// @ref ?Load@ObjectClass@@UAGJPAUIStream@@@Z=005d4760
// @ref ??0ObjectClass@@QAE@ABVNoInitClass@@@Z=005d2500
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20
// @ref ?Swizzler@@3VSwizzleManagerClass@@A=00abd868

HRESULT __stdcall ParticleClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) ParticleClass(NoInitClass());
    Swizzler.Swizzle((void **)&Class);
    Swizzler.Swizzle((void **)&System);
    return S_OK;
}
