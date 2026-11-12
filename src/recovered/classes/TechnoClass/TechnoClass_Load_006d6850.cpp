// HRESULT __stdcall TechnoClass::Load(IStream *stream) (0x006D6850, slot 5)
// RadioClass::Load, then pointer fixups, the 0x00AC0BF8 list entry and the audio controller.
#include "shared/techno.hpp"
// @ref ?Swizzler@@3VSwizzleManagerClass@@A=00abd868
// @ref ?Init@UnknownAudioController@@QAEXXZ=00405bf0
// @ref ?Load@RadioClass@@UAGJPAUIStream@@@Z=00636c60
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20
// @ref ?unknown_abstracts_ac0bf8@@3V?$DynamicVectorClass@PAVAbstractClass@@@@A=00ac0bf8

HRESULT __stdcall TechnoClass::Load(IStream *stream)
{
    HRESULT result = RadioClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    unknown_408 = 0;
    Swizzler.Swizzle((void **)&House);
    for (int index = 0; index < 8; index++) {
        Swizzler.Swizzle((void **)&unknown_25c[index]);
    }
    Swizzler.Swizzle((void **)&unknown_1b0);
    Swizzler.Swizzle((void **)&unknown_3f8);
    Swizzler.Swizzle((void **)&unknown_228);
    Swizzler.Swizzle((void **)&unknown_22c);
    Swizzler.Swizzle((void **)&unknown_230);
    Swizzler.Swizzle((void **)&unknown_384);
    Swizzler.Swizzle((void **)&SpawnManager);
    Swizzler.Swizzle((void **)&unknown_234);
    Swizzler.Swizzle((void **)&unknown_23c);
    Swizzler.Swizzle((void **)&unknown_40c);
    Swizzler.Swizzle((void **)&unknown_410);
    Swizzler.Swizzle((void **)&unknown_208);
    Swizzler.Swizzle((void **)&unknown_20c);
    Swizzler.Swizzle((void **)&Cargo.Object);
    Swizzler.Swizzle((void **)&unknown_27c);
    if (unknown_108) {
        unknown_abstracts_ac0bf8.Delete(this);
        unknown_abstracts_ac0bf8.Add(this);
    }
    Swizzler.Swizzle((void **)&unknown_108);
    unknown_3d8.Init();
    unknown_3ec = 1;
    unknown_3f0 = 0;
    return S_OK;
}
