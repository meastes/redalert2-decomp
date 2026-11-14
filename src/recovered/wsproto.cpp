// wsproto.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wsproto.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/winsock_interface.hpp"


// Header-static dynamic initializers of the unit at 0x0076AA90: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0076AA90-0x0076AC71); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0076AAB0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AAE0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AB00, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AB20, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AB40, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AB60, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AB80, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076ABC0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076ABF0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AC20, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AC50, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// Dynamic initializer (0x0076AC60, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0076aa90.cpp; no source of its own.


// WinsockInterfaceClass::WinsockInterfaceClass() (0x0076AC80)
// Empty buffer lists, no sockets, every static buffer free.

WinsockInterfaceClass::WinsockInterfaceClass()
{
    Socket = INVALID_SOCKET;
    Socket2 = INVALID_SOCKET;
    WinsockInitialised = false;
    for (int i = 0; i < 128; i++) {
        StaticInBuffers[i].InUse = false;
        StaticInBuffers[i].IsAllocated = false;
        StaticOutBuffers[i].InUse = false;
        StaticOutBuffers[i].IsAllocated = false;
        StaticOutBuffers[i].IsAlternate = false;
        StaticOutBuffers[i].unknown_18 = false;
        StaticAltBuffers[i].InUse = false;
        StaticAltBuffers[i].IsAllocated = false;
        StaticAltBuffers[i].IsAlternate = true;
        StaticAltBuffers[i].unknown_18 = false;
    }
    StaticInBuffersInUse = 0;
    StaticOutBuffersInUse = 0;
    StaticAltBuffersInUse = 0;
    NextInBuffer = 0;
    NextOutBuffer = 0;
    unknown_04 = 0x240;
    Debug_Printf("WinsockInterface constructed\n");
}


// WinsockInterfaceClass::~WinsockInterfaceClass() (0x0076AD70)

WinsockInterfaceClass::~WinsockInterfaceClass()
{
    Close();
}


// void WinsockInterfaceClass::Close() (0x0076AE10)
// Stops listening, closes the sockets and shuts Winsock down.

void WinsockInterfaceClass::Close()
{
    if (WinsockInitialised) {
        Stop_Listening();
        Close_Socket();
        WSACleanup();
        WinsockInitialised = false;
    }
}


// void WinsockInterfaceClass::Close_Socket() (0x0076AE40, slot 1)

void WinsockInterfaceClass::Close_Socket()
{
    if (Socket != INVALID_SOCKET) {
        closesocket(Socket);
        Socket = INVALID_SOCKET;
    }
    if (Socket2 != INVALID_SOCKET) {
        closesocket(Socket2);
        Socket2 = INVALID_SOCKET;
    }
}


// bool WinsockInterfaceClass::Start_Listening() (0x0076AE80, slot 7)
// Asks for read and write notifications on both sockets.

bool WinsockInterfaceClass::Start_Listening()
{
    if (WSAAsyncSelect(Socket, unknown_window_b263d8, Protocol_Event_Message(-1), 3) == SOCKET_ERROR) {
        Debug_Printf("Async select failed on primary socket - error code %d.\n", WSAGetLastError());
        return false;
    }
    if (Socket2 != INVALID_SOCKET) {
        if (WSAAsyncSelect(Socket2, unknown_window_b263d8, Protocol_Event_Message(Socket2), 3) == SOCKET_ERROR) {
            Debug_Printf("Async select failed on alternate socket - error code %d.\n", WSAGetLastError());
            return false;
        }
    }
    return true;
}


// void WinsockInterfaceClass::Stop_Listening() (0x0076AF10, slot 8)

void WinsockInterfaceClass::Stop_Listening()
{
    if (Socket != INVALID_SOCKET && unknown_window_b263d8) {
        WSAAsyncSelect(Socket, unknown_window_b263d8, 0, 0);
    }
    if (Socket2 != INVALID_SOCKET && unknown_window_b263d8) {
        WSAAsyncSelect(Socket2, unknown_window_b263d8, 0, 0);
    }
}


// void WinsockInterfaceClass::Discard_In_Buffers() (0x0076AF60, slot 5)

void WinsockInterfaceClass::Discard_In_Buffers()
{
    while (InBuffers.Count()) {
        WinsockBufferType *packet = InBuffers[0];
        if (packet->IsAllocated) {
            delete packet;
        } else {
            packet->InUse = false;
            StaticInBuffersInUse--;
        }
        InBuffers.Delete(0);
    }
    StaticInBuffersInUse = 0;
    NextInBuffer = 0;
}


// void WinsockInterfaceClass::Discard_Out_Buffers() (0x0076AFD0, slot 6)

void WinsockInterfaceClass::Discard_Out_Buffers()
{
    WinsockBufferType *packet;
    while (OutBuffers.Count()) {
        packet = OutBuffers[0];
        if (packet->IsAllocated) {
            delete packet;
        } else {
            packet->InUse = false;
            StaticOutBuffersInUse--;
        }
        OutBuffers.Delete(0);
    }
    StaticOutBuffersInUse = 0;
    NextOutBuffer = 0;
    while (AltBuffers.Count()) {
        packet = AltBuffers[0];
        if (packet->IsAllocated) {
            delete packet;
        } else {
            packet->InUse = false;
            StaticAltBuffersInUse--;
        }
        AltBuffers.Delete(0);
    }
    StaticAltBuffersInUse = 0;
    NextAltBuffer = 0;
}


// bool WinsockInterfaceClass::Init() (0x0076B0A0)
// Starts Winsock 1.1 (once), empties the buffers and takes the smallest
// usable maximum frame size of the UDP/IPX protocols as the packet size.

bool WinsockInterfaceClass::Init()
{
    short version;
    int rc;

    Debug_Printf("WinsockInterface init.\n");
    if (WinsockInitialised) {
        Debug_Printf("WinsockInterface already initialised\n");
        return true;
    }
    if (unknown_b2aea0 > 0 && !unknown_b2ae9c) {
        unknown_b2ae9c = true;
        MessageBoxA(unknown_window_b263d8, "Warning - Packet Loss Simulation mode enabled", "Note", 0);
    }
    char *buffer = new char[sizeof(UnknownWinsockData)];
    UnknownWinsockData *winsock_info = (UnknownWinsockData *)(&buffer[0]);
    Socket = INVALID_SOCKET;
    Discard_In_Buffers();
    Discard_Out_Buffers();
    Debug_Printf("About to call WSAStartup\n");
    version = (1 << 8) | 1;
    rc = WSAStartup(version, winsock_info);
    if (rc) {
        Debug_Printf("Winsock failed to initialise - error code %d.\n", rc);
        delete [] buffer;
        return false;
    }
    Debug_Printf("Winsock initialised OK\n");
    if ((winsock_info->wVersion & 0x00ff) != (version & 0x00ff) ||
        (winsock_info->wVersion >> 8) != (version >> 8)) {
        Debug_Printf("Winsock version is less than 1.1\n");
        delete [] buffer;
        return false;
    }
    Debug_Printf("Winsock version is %d.%d\n", winsock_info->wVersion & 0x00ff, winsock_info->wVersion >> 8);

    unsigned long length;
    char protocol_buffer[4096];
    unknown_04 = 0x222;
    length = sizeof(protocol_buffer);
    int count = EnumProtocolsA(Protocols, protocol_buffer, &length);
    if (count != SOCKET_ERROR) {
        int max_frame = 0x7FFFFFFF;
        UnknownProtocolInfo *info = (UnknownProtocolInfo *)protocol_buffer;
        for (int i = 0; i < count; i++) {
            Debug_Printf("Found protocol %s, max frame size is %d\n", info[i].lpProtocol, info[i].dwMessageSize);
            int size = info[i].dwMessageSize;
            if (size != 0 && size != -1 && max_frame >= size) {
                max_frame = size;
            }
        }
        if (max_frame != 0x7FFFFFFF && max_frame > 400) {
            unknown_04 = max_frame;
        }
    } else {
        Debug_Printf("Failed to get protocol enumeration\n");
    }
    WinsockInitialised = true;
    delete [] buffer;
    return true;
}


// void WinsockInterfaceClass::Build_Packet_CRC(WinsockBufferType *packet) (0x0076B290, slot 21)
// Checksums the packet's data a word at a time (the last partial word masked).

void WinsockInterfaceClass::Build_Packet_CRC(WinsockBufferType *packet)
{
    packet->CRC = 0;
    unsigned long *ptr = (unsigned long *)packet->Buffer;
    for (int i = 0; i < packet->BufferLen / 4; i++) {
        Add_CRC(&packet->CRC, *ptr++);
    }
    int remainder = packet->BufferLen & 3;
    if (remainder) {
        Add_CRC(&packet->CRC, *ptr & (0xFFFFFFFF >> (32 - remainder * 8)));
    }
}


// bool WinsockInterfaceClass::Passes_CRC_Check(WinsockBufferType *packet) (0x0076B310, slot 22)

bool WinsockInterfaceClass::Passes_CRC_Check(WinsockBufferType *packet)
{
    if (packet->BufferLen >= (int)sizeof(packet->Buffer)) {
        return false;
    }
    unsigned long crc = 0;
    unsigned long *ptr = (unsigned long *)packet->Buffer;
    for (int i = 0; i < packet->BufferLen / 4; i++) {
        Add_CRC(&crc, *ptr++);
    }
    int remainder = packet->BufferLen & 3;
    if (remainder) {
        Add_CRC(&crc, *ptr & (0xFFFFFFFF >> (32 - remainder * 8)));
    }
    if (crc == packet->CRC) {
        return true;
    }
    Debug_Printf("Error in Winsock packet CRC\n");
    return false;
}


// WinsockInterfaceClass::WinsockBufferType *WinsockInterfaceClass::Get_New_Out_Buffer(bool alternate) (0x0076B3A0)
// A free static outgoing buffer (round-robin) from the normal or alternate
// pool, or an allocated one when the pool is exhausted.

WinsockInterfaceClass::WinsockBufferType *WinsockInterfaceClass::Get_New_Out_Buffer(bool alternate)
{
    WinsockBufferType *buffer = 0;
    int i;
    if (!alternate) {
        if (StaticOutBuffersInUse == 128) {
            buffer = new WinsockBufferType;
            buffer->IsAllocated = true;
            buffer->InUse = true;
        } else {
            for (i = 0; i < 128; i++) {
                int index = NextOutBuffer++;
                if (NextOutBuffer > 127) {
                    NextOutBuffer = 0;
                }
                if (!StaticOutBuffers[index].InUse) {
                    buffer = &StaticOutBuffers[index];
                    buffer->InUse = true;
                    StaticOutBuffersInUse++;
                    break;
                }
            }
        }
        if (buffer) {
            buffer->IsAlternate = false;
            buffer->unknown_18 = 0;
        }
    } else {
        if (StaticAltBuffersInUse == 128) {
            buffer = new WinsockBufferType;
            buffer->IsAllocated = true;
            buffer->InUse = true;
        } else {
            for (i = 0; i < 128; i++) {
                int index = NextAltBuffer++;
                if (NextAltBuffer > 127) {
                    NextAltBuffer = 0;
                }
                if (!StaticAltBuffers[index].InUse) {
                    buffer = &StaticAltBuffers[index];
                    buffer->InUse = true;
                    StaticAltBuffersInUse++;
                    break;
                }
            }
        }
        if (buffer) {
            buffer->IsAlternate = true;
            buffer->unknown_18 = 0;
        }
    }
    return buffer;
}


// WinsockInterfaceClass::WinsockBufferType *WinsockInterfaceClass::Get_New_In_Buffer() (0x0076B500)
// A free static incoming buffer (round-robin), or an allocated one when the
// pool is exhausted.

WinsockInterfaceClass::WinsockBufferType *WinsockInterfaceClass::Get_New_In_Buffer()
{
    WinsockBufferType *buffer = 0;
    if (StaticInBuffersInUse == 128) {
        buffer = new WinsockBufferType;
        buffer->IsAllocated = true;
        buffer->InUse = true;
    } else {
        for (int i = 0; i < 128; i++) {
            int index = NextInBuffer++;
            if (NextInBuffer > 127) {
                NextInBuffer = 0;
            }
            if (!StaticInBuffers[index].InUse) {
                buffer = &StaticInBuffers[index];
                buffer->InUse = true;
                StaticInBuffersInUse++;
                break;
            }
        }
    }
    if (buffer) {
        buffer->IsAlternate = false;
        buffer->unknown_18 = 0;
    }
    return buffer;
}


// int WinsockInterfaceClass::Read(void *buf, int *buflen, void *address, int *addresslen, unsigned short *port) (0x0076B5A0, slot 2)
// Takes the oldest incoming packet: its data, sender address and port;
// returns its length (0 when none).

int WinsockInterfaceClass::Read(void *buf, int *buflen, void *address, int *addresslen, unsigned short *port)
{
    WWKeyboard->Check();
    if (InBuffers.Count() == 0) {
        return 0;
    }
    WinsockBufferType *packet = InBuffers[0];
    memcpy(buf, packet->Buffer, packet->BufferLen);
    unsigned int length = *addresslen;
    if (length > sizeof(packet->Address)) {
        length = sizeof(packet->Address);
    }
    *addresslen = length;
    memcpy(address, packet->Address, length);
    *port = packet->unknown_1a;
    *buflen = packet->BufferLen;
    InBuffers.Delete(0);
    if (packet->IsAllocated) {
        delete packet;
    } else {
        packet->InUse = false;
        StaticInBuffersInUse--;
    }
    return *buflen;
}


// void WinsockInterfaceClass::WriteTo(void *buf, int buflen, void *address, int addresslen, bool alternate, unsigned char arg6, unsigned short arg7) (0x0076B670, slot 3)
// Queues a packet for one address (checksummed) and posts a write
// notification for its socket.

void WinsockInterfaceClass::WriteTo(void *buf, int buflen, void *address, int addresslen, bool alternate, unsigned char arg6, unsigned short arg7)
{
    WinsockBufferType *packet = Get_New_Out_Buffer(alternate);
    memcpy(packet->Buffer, buf, buflen);
    packet->BufferLen = buflen;
    packet->IsBroadcast = false;
    packet->unknown_18 = arg6;
    memcpy(packet->Address, address, addresslen);
    packet->unknown_1a = arg7;
    Build_Packet_CRC(packet);
    SOCKET socket;
    if (alternate) {
        AltBuffers.Add(packet);
        socket = Socket2;
    } else {
        OutBuffers.Add(packet);
        socket = Socket;
    }
    SendMessageA((HWND)unknown_window_b263d8, Protocol_Event_Message(socket), 0, 2);
    WWKeyboard->Check();
}


// void WinsockInterfaceClass::Broadcast(void *buf, int buflen, bool alternate) (0x0076B7A0, slot 4)
// Queues a broadcast packet (checksummed) and posts a write notification.

void WinsockInterfaceClass::Broadcast(void *buf, int buflen, bool alternate)
{
    WinsockBufferType *packet = Get_New_Out_Buffer(alternate);
    memcpy(packet->Buffer, buf, buflen);
    packet->BufferLen = buflen;
    Build_Packet_CRC(packet);
    packet->IsBroadcast = true;
    SOCKET socket;
    if (alternate) {
        AltBuffers.Add(packet);
        socket = Socket2;
    } else {
        OutBuffers.Add(packet);
        socket = Socket;
    }
    SendMessageA((HWND)unknown_window_b263d8, Protocol_Event_Message(socket), 0, 2);
    WWKeyboard->Check();
}


// void WinsockInterfaceClass::Clear_Socket_Error(SOCKET socket) (0x0076B8A0, slot 9)
// Reads and then clears the socket's pending error.

void WinsockInterfaceClass::Clear_Socket_Error(SOCKET socket)
{
    unsigned long error_code;
    int length = 4;
    getsockopt(socket, 0xFFFF, 0x1007, (char *)&error_code, &length);
    error_code = 0;
    setsockopt(socket, 0xFFFF, 0x1007, (char *)&error_code, length);
}


// bool WinsockInterfaceClass::Set_Socket_Options(SOCKET socket) (0x0076B8F0, slot 10)
// Sizes the receive and send buffers of a socket (the primary by default).

bool WinsockInterfaceClass::Set_Socket_Options(SOCKET socket)
{
    if (socket == INVALID_SOCKET) {
        socket = Socket;
    }
    if (setsockopt(socket, 0xFFFF, 0x1002, (char *)&SocketReceiveBuffer, 4) == SOCKET_ERROR) {
        Debug_Printf("Failed to set socket option SO_RCVBUF - error code %d.\n", WSAGetLastError());
    } else {
        Debug_Printf("Socket option SO_RCVBUF set OK\n");
    }
    if (setsockopt(socket, 0xFFFF, 0x1001, (char *)&SocketSendBuffer, 4) == SOCKET_ERROR) {
        Debug_Printf("Failed to set socket option SO_SNDBUF - error code %d.\n", WSAGetLastError());
    } else {
        Debug_Printf("Socket option SO_SNDBUF set OK\n");
    }
    return true;
}


// bool WinsockInterfaceClass::Get_Host_Name(char *name, int length) (0x0076B990, slot 17)

bool WinsockInterfaceClass::Get_Host_Name(char *name, int length)
{
    if (gethostname(name, length) == SOCKET_ERROR) {
        Debug_Printf("Error - WinsockInterface Unable to get host name. Error code %d\n", WSAGetLastError());
        return false;
    }
    Debug_Printf("WinsockInterface Host name is %s\n", name);
    return true;
}


// void __fastcall WinsockInterfaceClass::unknown_76b9e0(int value) (0x0076B9E0)

void __fastcall WinsockInterfaceClass::unknown_76b9e0(int value)
{
    unknown_b2aea0 = value;
}


// int WinsockInterfaceClass::unknown_76b9f0() (0x0076B9F0)

int WinsockInterfaceClass::unknown_76b9f0()
{
    return unknown_b2aea0;
}


// WinsockInterfaceClass::Get_Protocol (0x0076BA00, slot 13)
// Inline in shared/winsock_interface.hpp; emitted with the vtable. No source of its own.


// WinsockInterfaceClass::Protocol_Event_Message (0x0076BA10, slot 14)
// Inline in shared/winsock_interface.hpp; emitted with the vtable. No source of its own.


// WinsockInterfaceClass::Open_Socket (0x0076BA20, slot 15)
// Inline in shared/winsock_interface.hpp; emitted with the vtable. No source of its own.


// WinsockInterfaceClass::Message_Handler (0x0076BA30, slot 16)
// Inline in shared/winsock_interface.hpp; emitted with the vtable. No source of its own.


// void *WinsockInterfaceClass::`scalar deleting destructor'(unsigned int flags) (0x0076BA40)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


extern "C" void __cdecl release_76bb00(void*);
class Cleanup_76bb00 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_76bb00::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_76bb00(pointer); items=0; } owns=0; count=0; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<WinsockInterfaceClass::WinsockBufferType *>::Fetch (0x0076BB50; VectorClass<WinsockInterfaceClass::WinsockBufferType *> vtable 0x007afafc slot 6).
// VectorClass<WinsockInterfaceClass::WinsockBufferType *>::operator equal (0x0076BB60; VectorClass<WinsockInterfaceClass::WinsockBufferType *> vtable 0x007afafc slot 1).
// VectorClass<WinsockInterfaceClass::WinsockBufferType *>::Resize (0x0076BBA0; VectorClass<WinsockInterfaceClass::WinsockBufferType *> vtable 0x007afafc slot 2).
// VectorClass<WinsockInterfaceClass::WinsockBufferType *>::Clear (0x0076BC50; VectorClass<WinsockInterfaceClass::WinsockBufferType *> vtable 0x007afafc slot 3).
// VectorClass<WinsockInterfaceClass::WinsockBufferType *>::ID_object (0x0076BC80; VectorClass<WinsockInterfaceClass::WinsockBufferType *> vtable 0x007afafc slot 4).
// VectorClass<WinsockInterfaceClass::WinsockBufferType *>::ID_pointer (0x0076BCC0; VectorClass<WinsockInterfaceClass::WinsockBufferType *> vtable 0x007afafc slot 5).


// DynamicVectorClass<WinsockInterfaceClass::WinsockBufferType *>::Resize (0x0076BCE0)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (population-1744).


extern "C" unsigned char vtable_76bdc0[]; extern "C" void __cdecl release_buffer_76bdc0(void*); extern "C" void __cdecl release_object_76bdc0(void*);
class Leaf_76bdc0 { public: void* get(unsigned int flags); };
void* Leaf_76bdc0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_76bdc0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_76bdc0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_76bdc0(this); return this; }


extern "C" unsigned char vtable_76be10[]; extern "C" void __cdecl release_buffer_76be10(void*); extern "C" void __cdecl release_object_76be10(void*);
class Leaf_76be10 { public: void* get(unsigned int flags); };
void* Leaf_76be10::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_76be10; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_76be10((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_76be10(this); return this; }
