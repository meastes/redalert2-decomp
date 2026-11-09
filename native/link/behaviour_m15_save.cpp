// Behaviour rows (Scout1125, native M15; NOT EXACT, native/behaviour_rewrites.json):
// the save game's IPersistStream members that Save_Game 0x006554A0 reaches and
// that have no recovered body. From the pass-6a decompiles: GetClassID stores
// the class's CLSID (the original's .rdata GUIDs, data/com_guids.cpp), Save
// is AbstractClass::Save 0x004100C0 with a success folded to S_OK.
#include "shared/cell_class.hpp"
#include "shared/tiberium.hpp"
#include "shared/light_source.hpp"

namespace {
struct Guid16 { unsigned char bytes[16]; };
void Copy_Guid(GUID *to, Guid16 const &from)
{
    unsigned char *out = (unsigned char *)to;
    for (int i = 0; i < 16; ++i) out[i] = from.bytes[i];
}
}
extern "C" Guid16 const ra2_m15_clsid_cell __asm__("_Startup_CLSID_CellClass");               // 0x007A1E70
extern "C" Guid16 const ra2_m15_clsid_tiberium __asm__("_Startup_CLSID_TiberiumClass");       // 0x007A1E60

// 0x0047A410 (pass-6a decompile 0047a410).
HRESULT __stdcall CellClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) return E_POINTER;
    Copy_Guid(clsid, ra2_m15_clsid_cell);
    return S_OK;
}

// 0x006E9120 (pass-6a decompile 006e9120).
HRESULT __stdcall TiberiumClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) return E_POINTER;
    Copy_Guid(clsid, ra2_m15_clsid_tiberium);
    return S_OK;
}

// 0x00537BF0 (pass-6a decompile 00537bf0): the base save, success as S_OK.
HRESULT __stdcall LightSourceClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (result >= 0) result = S_OK;
    return result;
}
