// int ObjectClass::In_Which_Layer() const (0x005D2BC0)
#include "shared/object_type.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848

int ObjectClass::In_Which_Layer() const
{
    if (!In_Air()) {
        return 2;
    }
    return (Get_Height_AGL() >= Rules->unknown_378) + 3;
}
