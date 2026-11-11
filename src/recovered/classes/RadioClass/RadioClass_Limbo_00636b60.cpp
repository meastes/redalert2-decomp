// bool RadioClass::Limbo() (0x00636B60, slot 52)
// Sends message 3 to every contact first unless unknown_76 is set.
#include "shared/radio.hpp"
// @ref ?Limbo@ObjectClass@@UAE_NXZ=005d3700

bool RadioClass::Limbo()
{
    if (!unknown_76) {
        SendToEachLink(3);
    }
    return ObjectClass::Limbo();
}
