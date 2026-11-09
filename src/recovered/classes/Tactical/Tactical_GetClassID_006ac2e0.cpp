// HRESULT __stdcall Tactical::GetClassID(GUID *clsid) (0x006AC2E0, slot 3)
// Reports the tactical view's class ID (TibSun Tactical::GetClassID).
#include "shared/tactical.hpp"
// @ref _CLSID_Tactical=007a1e90

extern "C" GUID const CLSID_Tactical;   // 0x007A1E90

HRESULT __stdcall Tactical::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_Tactical;
    return S_OK;
}
