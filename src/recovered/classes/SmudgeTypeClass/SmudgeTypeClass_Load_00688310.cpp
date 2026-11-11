// HRESULT __stdcall SmudgeTypeClass::Load(IStream *stream) (0x00688310)
// Restores the type in place and reloads its voxel and image.
#include "shared/smudge_type.hpp"
// @ref ?Load@ObjectTypeClass@@UAGJPAUIStream@@@Z=005d7c30
// @ref ?Fetch_Voxel_Image@ObjectTypeClass@@QAEXXZ=005d6730
// @ref ?Fetch_Normal_Image@ObjectTypeClass@@QAEXXZ=005d7630

HRESULT __stdcall SmudgeTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) SmudgeTypeClass(NoInitClass());
        Fetch_Voxel_Image();
        Fetch_Normal_Image();
        result = S_OK;
    }
    return result;
}
