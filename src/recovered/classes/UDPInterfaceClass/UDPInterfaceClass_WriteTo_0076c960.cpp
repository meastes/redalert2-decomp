// void UDPInterfaceClass::WriteTo(void *buf, int buflen, void *address, int addresslen, bool alternate, unsigned char arg6, unsigned short arg7) (0x0076C960, slot 3)
// A write to a broadcast address becomes a broadcast.
#include "shared/udp_interface.hpp"
// @ref ?Is_Broadcast@IPXAddressClass@@QAEHXZ=00523560
// @ref ?WriteTo@WinsockInterfaceClass@@UAEXPAXH0H_NEG@Z=0076b670

void UDPInterfaceClass::WriteTo(void *buf, int buflen, void *address, int addresslen, bool alternate, unsigned char arg6, unsigned short arg7)
{
    if (address && ((IPXAddressClass *)address)->Is_Broadcast()) {
        Broadcast(buf, buflen, alternate);
        return;
    }
    WinsockInterfaceClass::WriteTo(buf, buflen, address, addresslen, alternate, arg6, arg7);
}
