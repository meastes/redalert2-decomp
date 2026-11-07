// HRESULT UnknownComPtr<IPersist>::Query(UnknownComPtr<ILocomotion> const &other) (0x00454BF0)
// The converting query from a held locomotor (comip's _QueryInterface, YR twin ra2-decomp 0x0045AEA0
// in building.cpp), out of line in Building.CPP after its last vector instances; written as the
// explicit specialization of the stand-in overload in shared/com_ptr.hpp, defined at the end of the
// unit so its callers there keep calling it (the comip text, not the Query_From wrapper: a
// wrapper call moves the result register).
#include "shared/com_ptr.hpp"
#include "shared/locomotion.hpp"

template <>
HRESULT UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208>::Query(UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> const &other)
{
    HRESULT result;
    if (other) {
        IPersist *pointer;
        result = other->QueryInterface(*iid, (void **)&pointer);
        if (FAILED(result)) {
            Attach(0);
            return result;
        }
        Attach(pointer);
    } else {
        operator=((IPersist *)0);
        result = E_NOINTERFACE;
    }
    return result;
}
