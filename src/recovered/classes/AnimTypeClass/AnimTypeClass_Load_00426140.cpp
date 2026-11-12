// HRESULT __stdcall AnimTypeClass::Load(IStream *stream) (0x00426140)
// Restores the type in place, reloads its voxel and image, swizzles the
// type pointers and reloads a non-demand-loaded image for the scenario's
// theater.
#include "shared/anim_type.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
// @ref ?Load@ObjectTypeClass@@UAGJPAUIStream@@@Z=005d7c30
// @ref ?Fetch_Voxel_Image@ObjectTypeClass@@QAEXXZ=005d6730
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20
// @ref ?Swizzler@@3VSwizzleManagerClass@@A=00abd868
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290

HRESULT __stdcall AnimTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) AnimTypeClass(NoInitClass());
    Fetch_Voxel_Image();
    Fetch_Normal_Image();
    Swizzler.Swizzle((void **)&Next);
    Swizzler.Swizzle((void **)&ExpireAnim);
    Swizzler.Swizzle((void **)&TrailerAnim);
    Swizzler.Swizzle((void **)&BounceAnim);
    Swizzler.Swizzle((void **)&Spawns);
    Swizzler.Swizzle((void **)&Warhead);
    Swizzler.Swizzle((void **)&TiberiumSpawnType);
    if (!DemandLoad) {
        if (unknown_1fc) {
            char fullname[512];
            _makepath(fullname, 0, 0, ID, Theaters[unknown_id_source_a3d290->Theater].Suffix);
            unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
            return S_OK;
        }
        if (unknown_207) {
            unknown_slot40(unknown_id_source_a3d290->Theater);
        }
    }
    return S_OK;
}
