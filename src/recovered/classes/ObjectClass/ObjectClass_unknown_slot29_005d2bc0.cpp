// int ObjectClass::unknown_slot29() const (0x005D2BC0)
#include "shared/object_type.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848

int ObjectClass::unknown_slot29() const
{
    if (!unknown_slot21()) {
        return 2;
    }
    return (Get_Height_AGL() >= Rules->unknown_378) + 3;
}
