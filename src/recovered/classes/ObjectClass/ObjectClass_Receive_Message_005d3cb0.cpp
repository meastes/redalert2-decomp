// int ObjectClass::Receive_Message(RadioClass *from, int message, AbstractClass *&param) (0x005D3CB0)
#include "shared/object_type.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848

int ObjectClass::Receive_Message(RadioClass *from, int message, AbstractClass *&param)
{
    switch (message) {
    case 0x22:
        if (Health_Ratio() < Rules->unknown_1360) {
            return 1;
        }
        return 10;
    case 0x0D:
        Mark(2);
        return 1;
    }
    return 0;
}
