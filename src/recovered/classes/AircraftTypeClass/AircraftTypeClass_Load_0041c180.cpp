// HRESULT __stdcall AircraftTypeClass::Load(IStream *stream) (0x0041C180)
// Rebuilds the object in place, swizzles Trailer and reloads the image.
#include "shared/aircraft_type.hpp"
// @ref ?Load@TechnoTypeClass@@UAGJPAUIStream@@@Z=006de260
// @ref ??0TechnoTypeClass@@QAE@ABVNoInitClass@@@Z=006da6e0
// @ref ??_7AircraftTypeClass@@6BIPersistStream@@@=0079b678
// @ref ??_7AircraftTypeClass@@6BIRTTITypeInfo@@@=0079b65c
// @ref ??_7AircraftTypeClass@@6BINoticeSink@@@=0079b654
// @ref ??_7AircraftTypeClass@@6BINoticeSource@@@=0079b64c
// @ref ?SwizzleManager@@3VSwizzleManagerClass@@A=00abd868
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20
// @ref ?Fetch_Voxel_Image@ObjectTypeClass@@QAEXXZ=005d6730
// @ref ?Fetch_Normal_Image@ObjectTypeClass@@QAEXXZ=005d7630

HRESULT __stdcall AircraftTypeClass::Load(IStream *stream)
{
    HRESULT result = TechnoTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) AircraftTypeClass(NoInitClass());
    SwizzleManager.Swizzle((void **)&Trailer);
    Fetch_Voxel_Image();
    Fetch_Normal_Image();
    return S_OK;
}
