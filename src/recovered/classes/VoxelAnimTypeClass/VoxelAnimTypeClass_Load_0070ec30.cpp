// HRESULT __stdcall VoxelAnimTypeClass::Load(IStream *stream) (0x0070EC30)
// Restores the type in place, swizzles its type pointers and reloads its
// voxel.
#include "shared/voxel_anim_type.hpp"
// @ref ?Load@ObjectTypeClass@@UAGJPAUIStream@@@Z=005d7c30
// @ref ??0ObjectTypeClass@@QAE@ABVNoInitClass@@@Z=005d5a80
// @ref ?Fetch_Voxel_Image@ObjectTypeClass@@QAEXXZ=005d6730
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20
// @ref ?Swizzler@@3VSwizzleManagerClass@@A=00abd868

HRESULT __stdcall VoxelAnimTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) VoxelAnimTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&Warhead);
    Swizzler.Swizzle((void **)&BounceAnim);
    Swizzler.Swizzle((void **)&ExpireAnim);
    Swizzler.Swizzle((void **)&TrailerAnim);
    Swizzler.Swizzle((void **)&Spawns);
    Swizzler.Swizzle((void **)&AttachedSystem);
    Fetch_Voxel_Image();
    return S_OK;
}
