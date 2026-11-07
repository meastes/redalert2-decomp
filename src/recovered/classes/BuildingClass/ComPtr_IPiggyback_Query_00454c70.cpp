// HRESULT UnknownComPtr<IPiggyback>::Query(UnknownComPtr<ILocomotion> const &other) (0x00454C70)
// The converting query from a held locomotor (comip's _QueryInterface; YR twin ra2-decomp 0x0045AEA0
// family in building.cpp), out of line in Building.CPP after its last vector instances. Written as the
// explicit specialization of the stand-in overload in shared/com_ptr.hpp, defined at the end of the unit
// (Mission_Unload 0x00449290 keeps calling it); the body is comip's text as in Query_From (a
// `return Query_From(other);` wrapper moves the result register).
#include "shared/com_ptr.hpp"
#include "shared/locomotion.hpp"

extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;      // 0x007CFC30

template <>
HRESULT UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::Query(UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> const &other)
{
    HRESULT result;
    if (other) {
        IPiggyback *pointer;
        result = other->QueryInterface(*iid, (void **)&pointer);
        if (FAILED(result)) {
            Attach(0);
            return result;
        }
        Attach(pointer);
    } else {
        operator=((IPiggyback *)0);
        result = E_NOINTERFACE;
    }
    return result;
}
