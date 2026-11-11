#include "shared/techno.hpp"
ObjectClass *CargoClass::Detach_Object() {
    ObjectClass *passenger = Object;
    if (!passenger) return 0;
    Object = passenger->unknown_30;
    passenger->unknown_30 = 0;
    --Count;
    return passenger;
}
