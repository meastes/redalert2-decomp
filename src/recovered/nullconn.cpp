// nullconn.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/nullconn.json carries the addresses and the rows that
// stay in their own files.


#include "shared/null_modem.hpp"
#include "shared/crt_memory.hpp"


// NullModemConnClass::NullModemConnClass(int numsend, int numreceive, int maxlen, unsigned short magicnum) -- 0x005CDB40
// A serial (null-modem) connection: ConnectionClass with a 60-tick retry
// delta, unlimited retries and a 1200-tick timeout; no port yet (-1) and a
// send buffer of MaxPacketLen + 11 bytes (header and checksum). Class name
// from RTTI (vtable 0x007A73C8); member names follow Westwood's
// NullModemConnClass (hypotheses). Bound by linear sweep to the ret 0x10.

NullModemConnClass::NullModemConnClass(int numsend, int numreceive, int maxlen, unsigned short magicnum) :
    ConnectionClass(numsend, numreceive, maxlen, magicnum, 60, (unsigned long)-1, 1200)
{
    PortHandle = -1;
    SendBuf = new char[MaxPacketLen + 11];
}


// NullModemConnClass::~NullModemConnClass() (0x005CDB90)
// Frees the send buffer (the base destructor follows).

NullModemConnClass::~NullModemConnClass()
{
    delete SendBuf;
}


// void NullModemConnClass::Init(int port) -- 0x005CDBB0
// The base initialization, then the serial port handle (+0x4C).

void NullModemConnClass::Init(int port)
{
    ConnectionClass::Init();
    PortHandle = port;
}


// int NullModemConnClass::Send(char *buf, int buflen, void *extrabuf, int extralen, bool flagged, unsigned short id) -- 0x005CDBD0
// Slot 9: frames the packet for the serial line (magic 0xDABD, length,
// magic 0xDEAF, the data, a rotating checksum of the data and a carriage
// return) in SendBuf and writes it through the serial port object (SerialPort,
// 0x00B25BB0; Write_To_Serial_Port 0x00734C70). Nothing is sent without an
// open port. The extra buffer and flags are unused here.

int NullModemConnClass::Send(char *buf, int buflen, void *extrabuf, int extralen, bool flagged, unsigned short id)
{
    if (PortHandle == -1) {
        return 0;
    }
    SerialHeaderType *header = (SerialHeaderType *)SendBuf;
    header->MagicNumber = 0xdabd;
    header->Length = (unsigned short)buflen;
    header->MagicNumber2 = 0xdeaf;
    unsigned long sendlen = sizeof(SerialHeaderType);
    memcpy(SendBuf + sendlen, buf, buflen);
    sendlen += buflen;
    int *ibuf = (int *)(SendBuf + sendlen);
    *ibuf = Compute_CRC(buf, buflen);
    sendlen += sizeof(int);
    *(SendBuf + sendlen) = '\r';
    sendlen += 1;
    SerialPort->Write_To_Serial_Port((unsigned char *)SendBuf, sendlen);
    return 1;
}


// int NullModemConnClass::Compute_CRC(char *buf, int buflen) (0x005CDC80)
// The rotating checksum of the data: each step doubles the sum, adds back
// its top bit and adds the next byte. Send expands it inline.

int __fastcall NullModemConnClass::Compute_CRC(char *buf, int buflen)
{
    unsigned int sum = 0;
    unsigned int hibit;
    for (int index = 0; index < buflen; index++) {
        if (sum & 0x80000000) {
            hibit = 1;
        } else {
            hibit = 0;
        }
        sum <<= 1;
        sum += hibit;
        sum += (unsigned char)buf[index];
    }
    return (int)sum;
}


// int NullModemConnClass::Packet_Overhead_Size() (0x005CDCB0)
// The bytes a packet carries for the serial link only: the frame header and CRC
// (PACKET_SERIAL_OVERHEAD_SIZE) and the queue header (CommHeaderType).

int NullModemConnClass::Packet_Overhead_Size()
{
    return PACKET_SERIAL_OVERHEAD_SIZE + sizeof(CommHeaderType);
}


// void NullModemConnClass::Init() -- 0x005CDCC0
// Slot 1: the base initialization only.

void NullModemConnClass::Init()
{
    ConnectionClass::Init();
}


// NullModemConnClass scalar deleting destructor (0x005CDCD0, ??_G): emitted
// with the vtable by the constructor; the destructor (inline in the class)
// frees the send buffer, then calls ConnectionClass::~ConnectionClass.
