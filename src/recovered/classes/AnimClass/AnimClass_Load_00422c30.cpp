// HRESULT __stdcall AnimClass::Load(IStream *stream) (0x00422C30, slot 5)
// Loads the object, rebuilds itself in place, restarts both audio
// controllers, fixes up its pointers and turns the saved index unknown_c0
// back into unknown_bc (the 0x00AB6EF8 entry's +0x30C value, else the
// palette's entry, else 0).
#include "shared/anim.hpp"
#include "shared/color_scheme.hpp"
// @ref ?Load@ObjectClass@@UAGJPAUIStream@@@Z=005d4760
// @ref ??0ObjectClass@@QAE@ABVNoInitClass@@@Z=005d2500
// @ref ??0StageTimerClass@@QAE@ABVNoInitClass@@@Z=00423f60
// @ref ?Load@PaletteName@@QAIPAXXZ=00603380
// @ref ?SwizzleManager@@3VSwizzleManagerClass@@A=00abd868
// @ref ?Swizzle@SwizzleManagerClass@@UAGJPAPAX@Z=0069fb20

HRESULT __stdcall AnimClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) AnimClass(NoInitClass());
        unknown_180.Init();
        unknown_194.Init();
        Swizzle_Pointer(Class);
        Swizzle_Pointer(unknown_b4);
        Swizzle_Pointer(unknown_15c);
        if (unknown_c0 > -1) {
            if (unknown_c0 < ColorSchemes.Count()) {
                unknown_bc = ColorSchemes[unknown_c0]->Converter;
                return result;
            }
        } else {
            if (Palette.Buffer[0] != '\0') {
                UnknownPaletteSource *source = (UnknownPaletteSource *)Palette.Load();
                if (source != 0) {
                    ColorScheme *entry = source->unknown_slot6(0);
                    if (entry != 0) {
                        unknown_bc = entry->Converter;
                        return result;
                    }
                }
            }
            unknown_bc = 0;
        }
    }
    return result;
}
