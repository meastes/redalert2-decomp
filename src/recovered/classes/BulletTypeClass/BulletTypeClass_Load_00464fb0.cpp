// HRESULT __stdcall BulletTypeClass::Load(IStream *stream) (0x00464FB0)
// Restores the type in place, reloads its voxel and image and swizzles its
// trailer and weapon pointers.
#include "shared/bullet_type.hpp"
// @ref ?Load@ObjectTypeClass@@UAGJPAUIStream@@@Z=005d7c30
// @ref ??0ObjectTypeClass@@QAE@ABVNoInitClass@@@Z=005d5a80
// @ref ?Fetch_Voxel_Image@ObjectTypeClass@@QAEXXZ=005d6730
// @ref ?Fetch_Normal_Image@ObjectTypeClass@@QAEXXZ=005d7630
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20
// @ref ?SwizzleManager@@3VSwizzleManagerClass@@A=00abd868

HRESULT __stdcall BulletTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) BulletTypeClass(NoInitClass());
    Fetch_Voxel_Image();
    Fetch_Normal_Image();
    SwizzleManager.Swizzle((void **)&Trailer);
    SwizzleManager.Swizzle((void **)&AirburstWeapon);
    SwizzleManager.Swizzle((void **)&ShrapnelWeapon);
    return S_OK;
}
