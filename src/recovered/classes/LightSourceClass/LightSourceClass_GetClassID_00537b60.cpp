// HRESULT LightSourceClass::GetClassID(GUID *clsid) (0x00537B60, slot 3)
#include "shared/light_source.hpp"
// @ref _CLSID_LightSource=007a1e30

HRESULT __stdcall LightSourceClass::GetClassID(GUID *clsid)
{
    if (!clsid) {
        return E_POINTER;
    }
    *clsid = CLSID_LightSource;
    return S_OK;
}
