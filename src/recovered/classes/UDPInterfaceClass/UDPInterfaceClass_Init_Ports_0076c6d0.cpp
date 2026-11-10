// bool UDPInterfaceClass::Init_Ports() (0x0076C6D0, slot 25)
#include "shared/udp_interface.hpp"
// @ref ?Init@WinsockInterfaceClass@@QAE_NXZ=0076b0a0

bool UDPInterfaceClass::Init_Ports()
{
    if (Init()) {
        NumSockets = 0;
        return true;
    }
    return false;
}
