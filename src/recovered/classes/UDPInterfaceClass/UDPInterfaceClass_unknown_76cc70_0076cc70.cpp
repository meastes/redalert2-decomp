// bool UDPInterfaceClass::Is_Reachable(unsigned long *address, unsigned short port) (0x0076CC70)
// False when the address and port are marked unreachable.
#include "shared/udp_interface.hpp"

bool UDPInterfaceClass::Is_Reachable(unsigned long *address, unsigned short port)
{
    for (int i = 0; i < NumUnreachable; i++) {
        if (port == UnreachablePorts[i] && *address == UnreachableAddresses[i]) {
            return false;
        }
    }
    return true;
}
