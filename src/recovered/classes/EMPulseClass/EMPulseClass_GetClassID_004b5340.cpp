// HRESULT EMPulseClass::GetClassID(GUID *clsid) (0x004B5340, slot 3)
#include "shared/empulse_class.hpp"
// @ref _CLSID_EMPulseClass=007a1e80

HRESULT __stdcall EMPulseClass::GetClassID(GUID *clsid)
{
    if (!clsid) {
        return E_POINTER;
    }
    *clsid = CLSID_EMPulseClass;
    return S_OK;
}
