// int __fastcall IPXConnClass::Broadcast(char *buf, int buflen) (0x00523F70)
#include "shared/ipx_conn.hpp"
// @ref ?PacketTransport@@3PAVWinsockInterfaceClass@@A=00839c50

int __fastcall IPXConnClass::Broadcast(char *buf, int buflen)
{
    PacketTransport->Broadcast(buf, buflen, 0);
    return 1;
}
