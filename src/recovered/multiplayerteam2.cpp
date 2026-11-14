// multiplayerteam2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/multiplayerteam2.json carries the addresses and the rows that
// stay in their own files.


#define RA2_FIREWALL_SETTINGS_UNIT
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/firewall_settings.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/ccini_class.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/udp_interface.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/ipx_address.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_memory.hpp"


// Header-static dynamic initializers of the unit at 0x005B9C50:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x005B9C50-0x005B9E36); the other
// drafts of this plan are emitted by these includes.
// FirewallSettings PortPool (+0x10) and the port members are declared only for this unit.


// Dynamic initializer (0x005B9C80, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9CA0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9CC0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9CE0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9D00, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9D20, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9D60, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9D90, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9DC0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9DF0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9E00, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// Dynamic initializer (0x005B9E20, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_005b9c50.cpp; no source of its own.


// The firewall settings 0x00A71B18 (dynamic initializer 0x005B9E40).
FirewallSettings unknown_firewall_a71b18;


// FirewallSettings::FirewallSettings() (0x005B9E60): all four fields zero.
FirewallSettings::FirewallSettings() :
    unknown_00(0),
    unknown_04(0),
    unknown_08(0),
    unknown_0c(0)
{
}


// Detects the firewall type once (0x005B9E80) and, when the detection found one,
// writes type and delta to [MultiPlayer] of the INI object 0x00839740 and saves RA2.INI.


void FirewallSettings::Detect_And_Save_005b9e80()
{
    bool found = false;
    if (unknown_00 == 0) {
        unknown_00 = Detect_005ba2c0(&found);
        if (found) {
            RawFileClass file("RA2.INI");
            unknown_ini_839740.Put_Int("MultiPlayer", "FirewallSettings", unknown_00, 0);
            unknown_ini_839740.Put_Int("MultiPlayer", "FirewallDelta", unknown_08, 0);
            unknown_ini_839740.Save(file, false);
        }
    }
}


// unsigned short FirewallSettings::Get_Next_Port(int step) (0x005B9F10): advances the port pool by step
// (1 when 0), wrapping past 0xFFFF to 0x800, and returns the new port once the UDP transport can open a
// spare socket on it (the socket is closed again); outside internet games, or after 256 tries, the port
// is returned untested. The shared return is the loop body's last statement and the failed open
// continues to the counter test, which leaves the exhausted path its own epilogue (jne top; ret).

unsigned short FirewallSettings::Get_Next_Port(int step)
{
    if (step == 0) {
        step = 1;
    }
    int tries = 0xFF;
    unsigned short port;
    do {
        PortPool += step;
        port = (unsigned short)PortPool;
        if (PortPool > 0xFFFF) {
            PortPool = 0x800;
        }
        if (PacketTransport && unknown_game_mode_a3d298 == 4) {
            if (!((UDPInterfaceClass *)PacketTransport)->Open_Spare_Socket(port)) {
                continue;
            }
            ((UDPInterfaceClass *)PacketTransport)->Close_Spare_Socket(port);
        }
        return port;
    } while (tries--);
    return port;
}


// bool FirewallSettings::Send_Mangler_Query(IPXAddressClass *address, unsigned short port, int packet_id, bool flag)
// (0x005B9F90): selects port on the UDP transport, sends a 0x2B mangler query ("WW", the port, flag) to address as a
// global message and services the manager until it has gone out (180 ticks at most), then resets the port.
void unknown_481fb0();

bool FirewallSettings::Send_Mangler_Query(IPXAddressClass *address, unsigned short port, int packet_id, bool flag)
{
    char buffer[456];
    ManglerPacket &packet = *(ManglerPacket *)buffer;
    packet.Type = 0x2B;
    strcpy(packet.Name, "WW");
    ((UDPInterfaceClass *)PacketTransport)->Select_Port_Socket(port);
    packet.Port = port;
    packet.unknown_37 = (flag == true);
    int count = unknown_ipx_manager_a40978.Global_Num_Send();
    Debug_Printf("Sending from port %d to %s\n", port, address->As_String());
    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, address, *(unsigned short *)address, packet_id);
    CDTimerClass<SystemTimerClass> timer(180);
    while (unknown_ipx_manager_a40978.Global_Num_Send() > count || PacketTransport->OutBuffers.Count()) {
        if (!timer) {
            break;
        }
        unknown_ipx_manager_a40978.Service();
        unknown_481fb0();
    }
    ((UDPInterfaceClass *)PacketTransport)->Select_Port_Socket(0);
    return true;
}


// unsigned short FirewallSettings::Wait_For_Mangler_Reply(int packet_id, int time) (0x005BA0D0): services the manager
// for 60 ticks (or time) looking for the mangler's 0x2C reply to packet_id (product 0xAA03); returns the port the
// mangler saw ("Mangler is seeing packets from port %d as coming from port %d") or 0.
void unknown_481fb0();

unsigned short FirewallSettings::Wait_For_Mangler_Reply(int packet_id, int time)
{
    int index = 0;
    char buffer[456];
    IPXAddressClass address;
    int length;
    unsigned long id;
    unsigned short product_id;
    CDTimerClass<SystemTimerClass> timer(60);
    if (time) {
        timer = CDTimerClass<SystemTimerClass>(time);
    }
    while (timer) {
        unknown_ipx_manager_a40978.Service();
        unknown_481fb0();
        if (index >= unknown_ipx_manager_a40978.Global_Num_Receive()) {
            index = 0;
        }
        ManglerPacket *packet = (ManglerPacket *)buffer;
        if (unknown_ipx_manager_a40978.unknown_525af0(index, buffer, &length, &address, &product_id, &id)
            && product_id == 0xAA03 && packet->Type == 0x2C) {
            int from = packet->Port;
            if (packet_id == (int)id) {
                unsigned short port = packet->SeenPort;
                Debug_Printf("Mangler is seeing packets from port %d as coming from port %d\n", from, port);
                unknown_ipx_manager_a40978.unknown_525b30(index);
                return port;
            }
        }
        index++;
    }
    return 0;
}


// Writes the firewall type (+0x00) and delta (+0x08) to [MultiPlayer] of the INI
// object 0x00839740 and saves it to RA2.INI (0x005BA210).


void FirewallSettings::Save_005ba210()
{
    RawFileClass file("RA2.INI");
    unknown_ini_839740.Put_Int("MultiPlayer", "FirewallSettings", unknown_00, 0);
    unknown_ini_839740.Put_Int("MultiPlayer", "FirewallDelta", unknown_08, 0);
    unknown_ini_839740.Save(file, false);
}


// Reads the firewall settings and delta from [MultiPlayer] of the INI object 0x00839740 (0x005BA280;
// Read_MultiPlayer_Settings calls it once). Draft: Scout1076.

void FirewallSettings::Load_005ba280()
{
    unknown_04 = unknown_ini_839740.Get_Int("MultiPlayer", "FirewallSettings", 0);
    unknown_0c = unknown_ini_839740.Get_Int("MultiPlayer", "FirewallDelta", 1);
}


// int FirewallSettings::Detect_005ba2c0(bool *found) (0x005BA2C0): the NAT/firewall detection. A source port
// given by the user skips it (type 1, or 9 with the Netgear bug flag). Otherwise resolves up to four distinct
// mangler addresses (servserv's list, else the defaults; at least three needed), checks whether two queries from
// one port come back with the same or different ports, then queries four fresh ports up to three times and
// derives the allocation delta (relative or absolute, sequential sets *found), and for sequential NATs tests
// whether a second destination port at the same address is mangled differently.
extern int unknown_source_port_a4025c;          // 0x00A4025C the source port given by the user
extern bool unknown_netgear_bug_a40188;         // 0x00A40188 Netgear bug flag (command line or SendDelay)
extern int unknown_router_b2ac30;               // 0x00B2AC30
extern int unknown_mangler_count_b2ac34;
extern char unknown_mangler_addresses_b26cbc[64][128];
extern int unknown_mangler_ports_b291a4[64];
extern char unknown_default_manglers_7e6334[][32];

int FirewallSettings::Detect_005ba2c0(bool *found)
{
    unsigned short mangler_port = 0x10E1;
    int packet_id = 0x7F000000;
    *found = false;
    int count = 0;
    IPXAddressClass addresses[4];
    CDTimerClass<SystemTimerClass> timer;
    unsigned short sources[4];
    unsigned short mangled[4];
    bool relative;
    bool sequential;

    if (unknown_source_port_a4025c != 0) {
        int type = 1;
        Debug_Printf("Source port %d specified by user\n", unknown_source_port_a4025c);
        if (unknown_netgear_bug_a40188) {
            type = 9;
            Debug_Printf("Netgear bug specified by command line or SendDelay flag\n");
        }
        return type;
    }

    timer = CDTimerClass<SystemTimerClass>(300);
    Debug_Printf("About to call gethostbyname for the mangler address\n");
    unsigned long ips[4];
    int i = 0;
    do {
        char const *name = unknown_default_manglers_7e6334[i];
        if (unknown_mangler_count_b2ac34 > i) {
            name = unknown_mangler_addresses_b26cbc[i];
            mangler_port = unknown_mangler_ports_b291a4[i];
            Debug_Printf("Using mangler from servserv\n");
        }
        i++;
        if (strlen(name) == 0) {
            break;
        }
        char hostname[256];
        strcpy(hostname, name);
        hostent *host = gethostbyname(hostname);
        if (host == 0) {
            Debug_Printf("gethostbyname failed! Error code %d\n", WSAGetLastError());
            break;
        }
        bool dup = false;
        for (int j = 0; j < count; j++) {
            if (memcmp(&ips[j], host->h_addr_list[0], 4) == 0) {
                dup = true;
                break;
            }
        }
        if (!dup) {
            int n = count++;
            ips[n] = *(unsigned long *)host->h_addr_list[0];
            Debug_Printf("Found mangler address at %d.%d.%d.%d\n",
                ((unsigned char *)&ips[n])[0], ((unsigned char *)&ips[n])[1], ((unsigned char *)&ips[n])[2], ((unsigned char *)&ips[n])[3]);
        }
    } while (count < 4 && timer);

    if (count < 3) {
        return 1;
    }
    for (int k = 0; k < count; k++) {
        NetNodeType node;
        *(unsigned long *)node = ips[k];
        addresses[k].Set_Node(node);
        addresses[k].Set_Port(mangler_port);
    }

    unsigned short port = Get_Next_Port(1);
    if (!((UDPInterfaceClass *)PacketTransport)->Open_Spare_Socket(port)) {
        return 1;
    }
    PacketTransport->Start_Listening();

    timer = CDTimerClass<SystemTimerClass>(360);
    unsigned short reply1 = 0;
    while (timer && !reply1) {
        Send_Mangler_Query(&addresses[0], port, 0x7F000000, false);
        reply1 = Wait_For_Mangler_Reply(0x7F000000, 0);
    }
    if (reply1 == 0 || reply1 == port) {
        ((UDPInterfaceClass *)PacketTransport)->Close_Spare_Socket(port);
        if (reply1 == port) {
            unknown_08 = 0;
            *found = true;
        }
        return 1;
    }

    timer = CDTimerClass<SystemTimerClass>(360);
    unsigned short reply2 = 0;
    while (timer && !reply2) {
        Send_Mangler_Query(&addresses[1], port, 0x7F000001, false);
        reply2 = Wait_For_Mangler_Reply(0x7F000001, 0);
    }
    ((UDPInterfaceClass *)PacketTransport)->Close_Spare_Socket(port);
    if (reply2 == 0 || reply2 == port) {
        return 1;
    }

    int result = (reply1 != reply2) ? 4 : 2;
    int j;
    for (int attempt = 0; attempt < 3; attempt++) {
        memset(sources, 0, sizeof(sources));
        memset(mangled, 0, sizeof(mangled));
        for (j = 0; j < 4; j++) {
            sources[j] = Get_Next_Port(j);
            if (!((UDPInterfaceClass *)PacketTransport)->Open_Spare_Socket(sources[j])) {
                for (int c = 0; c < j; c++) {
                    if (sources[c]) {
                        ((UDPInterfaceClass *)PacketTransport)->Close_Spare_Socket(sources[c]);
                    }
                }
                return result;
            }
        }
        PacketTransport->Start_Listening();
        packet_id += 10;
        int replies = 0;
        timer = CDTimerClass<SystemTimerClass>(720);
        while (timer && replies < 4) {
            for (j = 0; j < 4; j++) {
                if (mangled[j] == 0) {
                    Send_Mangler_Query(&addresses[0], sources[j], packet_id + j, false);
                    mangled[j] = Wait_For_Mangler_Reply(packet_id + j, 0);
                    if (mangled[j]) {
                        replies++;
                    }
                }
            }
        }
        for (int c = 0; c < j; c++) {
            if (sources[c]) {
                ((UDPInterfaceClass *)PacketTransport)->Close_Spare_Socket(sources[c]);
            }
        }
        if (replies < 4) {
            if (unknown_0c && unknown_04 > 1) {
                unknown_08 = unknown_0c;
            }
            return result;
        }
        relative = false;
        sequential = false;
        int delta = Get_Allocation_Delta(replies, sources, mangled, &relative, &sequential);
        if (delta) {
            int kind;
            if (relative) {
                kind = 0x20;
            } else {
                kind = 0x10;
            }
            result |= kind;
            unknown_08 = delta;
            if (sequential) {
                *found = true;
            }
            break;
        }
        if (unknown_0c && unknown_04 > 1) {
            unknown_08 = unknown_0c;
        }
    }

    if (result & 4) {
        if (result & 0x10) {
            Debug_Printf("Testing to see if the NAT mangles differently per destination port at the same IP\n");
            unsigned short port_a = Get_Next_Port(1);
            if (!((UDPInterfaceClass *)PacketTransport)->Open_Spare_Socket(port_a)) {
                return result;
            }
            unsigned short port_b = Get_Next_Port(1);
            if (!((UDPInterfaceClass *)PacketTransport)->Open_Spare_Socket(port_b)) {
                return result;
            }
            PacketTransport->Start_Listening();
            timer = CDTimerClass<SystemTimerClass>(240);
            unsigned short reply_a = 0;
            packet_id++;
            while (timer && !reply_a) {
                Send_Mangler_Query(&addresses[0], port_a, packet_id, false);
                reply_a = Wait_For_Mangler_Reply(packet_id, 0);
            }
            if (!reply_a) {
                ((UDPInterfaceClass *)PacketTransport)->Close_Spare_Socket(port_a);
                ((UDPInterfaceClass *)PacketTransport)->Close_Spare_Socket(port_b);
                return result;
            }
            IPXAddressClass other = addresses[0];
            other.Set_Port(*(unsigned short *)other.NetworkNumber + 1);
            Send_Mangler_Query(&other, port_a, packet_id, false);
            Send_Mangler_Query(&other, port_a, packet_id, false);
            Send_Mangler_Query(&other, port_a, packet_id, false);
            packet_id++;
            unsigned short reply_b = 0;
            timer = CDTimerClass<SystemTimerClass>(240);
            while (timer && !reply_b) {
                Send_Mangler_Query(&addresses[0], port_b, packet_id, false);
                reply_b = Wait_For_Mangler_Reply(packet_id, 0);
            }
            if (reply_b) {
                if (reply_b != reply_a + unknown_08) {
                    Debug_Printf("NAT uses different source ports for different destination ports\n");
                    result |= 0x40;
                } else {
                    Debug_Printf("NAT uses the same source port for different destination ports\n");
                }
            }
        } else {
            result |= 0x40;
        }
    }
    Debug_Printf("Testing for Netgear bug\n");
    if (unknown_router_b2ac30 == 2 || unknown_netgear_bug_a40188) {
        result |= 8;
        Debug_Printf("Netgear bug specified by command line or SendDelay flag\n");
    }
    return result;
}


// int FirewallSettings::Get_Allocation_Delta(int count, unsigned short *original_ports, unsigned short *ports,
// bool *relative, bool *sequential) (0x005BAC50; Detect 0x005BA2C0): sorts the mangled ports (the original ports
// alongside), then classifies the allocation: incremental (delta 1) or semi-incremental (2), absolute sequences,
// else relative to the original ports; returns the delta (0 when none fits). delta is recomputed before the
// delta == 2 test: with one assignment VC6 threads the failed delta == 1 sub-tests past it.

int FirewallSettings::Get_Allocation_Delta(int count, unsigned short *original_ports, unsigned short *ports, bool *relative, bool *sequential)
{
    Debug_Printf("Looking for port allocation pattern in original_ports %d, %d, %d, %d\n",
        original_ports[0], original_ports[1], original_ports[2], original_ports[3]);

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count - 1; j++) {
            if (ports[j] > ports[j + 1]) {
                int temp = ports[j];
                ports[j] = ports[j + 1];
                ports[j + 1] = temp;
                temp = original_ports[j];
                original_ports[j] = original_ports[j + 1];
                original_ports[j + 1] = temp;
            }
        }
    }

    int delta;
    delta = ports[1] - ports[0];
    if (delta == 1 && ports[2] - ports[1] == delta && ports[3] - ports[2] == delta) {
        Debug_Printf("Incremental port allocation detected\n");
        *relative = false;
        *sequential = true;
        return delta;
    }
    delta = ports[1] - ports[0];
    if (delta == 2 && ports[2] - ports[1] == delta && ports[3] - ports[2] == delta) {
        Debug_Printf("Semi-incremental port allocation detected\n");
        *relative = false;
        *sequential = true;
        return delta;
    }
    int delta2 = ports[2] - ports[1];
    int delta3 = ports[3] - ports[2];
    if (delta == delta2) {
        if (delta2 == delta3) {
            Debug_Printf("Looks good for absolute allocation sequence delta of %d\n", delta);
            *relative = false;
            *sequential = true;
            return delta;
        }
        Debug_Printf("Probable absolute allocation sequence delta of %d\n", delta);
        *relative = false;
        *sequential = false;
        return delta;
    }
    if (delta2 == delta3) {
        Debug_Printf("Probable absolute allocation sequence delta of %d\n", delta2);
        *relative = false;
        *sequential = false;
        return delta2;
    }

    for (int k = 0; k < count; k++) {
        ports[k] -= original_ports[k];
    }
    delta = ports[1] - ports[0];
    delta2 = ports[2] - ports[1];
    delta3 = ports[3] - ports[2];
    if (delta == delta2) {
        if (delta2 == delta3) {
            Debug_Printf("Looks good for a relative port range delta of %d\n", delta);
            *relative = true;
            *sequential = true;
            return delta;
        }
    } else if (delta != delta3) {
        if (delta2 == delta3) {
            Debug_Printf("Detected probable broken relative port range delta of %d\n", delta2);
            *relative = true;
            *sequential = false;
            return delta2;
        }
        *sequential = false;
        *relative = false;
        return 0;
    }
    Debug_Printf("Detected probable broken relative port range delta of %d\n", delta);
    *relative = true;
    *sequential = false;
    return delta;
}


// int FirewallSettings::Connection_Weight_005baee0(unsigned char flags) (0x005BAEE0): the weighted sum of
// the connection flag bits (1, 2, 3, 10, 1, 2 for bits 0-5; name hypothesis; does not read this).
int FirewallSettings::Connection_Weight_005baee0(unsigned char flags)
{
    int weight = 0;
    if (flags & 1) weight = 1;
    if (flags & 2) weight += 2;
    if (flags & 4) weight += 3;
    if (flags & 8) weight += 10;
    if (flags & 0x10) ++weight;
    if (flags & 0x20) weight += 2;
    return weight;
}


int FirewallSettings::Connection_Step_005baf20(int flags)
{
    int step=2;
    if(flags & 1) step=3;
    if(flags & 2) ++step;
    if(flags & 4) ++step;
    if(flags & 0x20) step+=5;
    return step;
}


// unsigned short FirewallSettings::Get_Mangled_Port(unsigned short port) (0x005BAF50; called from 0x0074E87B):
// the port a NAT will map port to. Types 0 and odd types return port. Otherwise resolves the mangler (servserv's
// current entry, else mangler1.westwood.com:4321), and for sequential NATs (type & 4) opens the next pool port
// (Get_Next_Port inlined); queries the mangler from it for up to 180 ticks and predicts the mapping from the
// reply and the allocation delta (+0x08): delta 100 relative, else the next delta boundary plus port % delta,
// or reply + delta for type & 0x10; wrapped into 0x400..0xFFFF.
// The servserv mangler list (declared in wol_sink.hpp, whose WOL templates this unit does not emit).
extern int unknown_mangler_count_b2ac34;
extern int unknown_mangler_index_b2ac38;
extern char unknown_mangler_addresses_b26cbc[64][128];
extern int unknown_mangler_ports_b291a4[64];
extern int unknown_mangler_packet_id_7e63d4;    // 0x007E63D4 the next mangler query id

unsigned short FirewallSettings::Get_Mangled_Port(unsigned short port)
{
    CDTimerClass<SystemTimerClass> timer;
    int type = unknown_00;
    if (type == 0) {
        return port;
    }
    if (type & 1) {
        return port;
    }
    char hostname[256];
    unsigned short mangler_port = 0x10E1;
    strcpy(hostname, "mangler1.westwood.com");
    if (unknown_mangler_count_b2ac34 > 0) {
        strcpy(hostname, unknown_mangler_addresses_b26cbc[unknown_mangler_index_b2ac38]);
        mangler_port = unknown_mangler_ports_b291a4[unknown_mangler_index_b2ac38];
        Debug_Printf("Using mangler from servserv\n");
    }
    Debug_Printf("Firewall helper - About to call gethostbyname for %s\n", hostname);
    hostent *host = gethostbyname(hostname);
    if (host == 0) {
        Debug_Printf("gethostbyname failed! Error code %d\n", WSAGetLastError());
        return port;
    }
    unsigned long ip = *(unsigned long *)host->h_addr_list[0];
    Debug_Printf("Mangler address is %d.%d.%d.%d ; %d\n",
        ((unsigned char *)&ip)[0], ((unsigned char *)&ip)[1], ((unsigned char *)&ip)[2], ((unsigned char *)&ip)[3], mangler_port);
    IPXAddressClass address;
    address.Set_Node((unsigned char *)&ip);
    address.Set_Port(mangler_port);
    Debug_Printf("Mangler address is %s\n", address.As_String());
    unsigned short source = 0;
    unsigned short opened = 0;
    Debug_Printf("fw = %08x\n", type);
    int sequential = type & 4;
    if (sequential) {
        source = Get_Next_Port(1);
        opened = source;
        if (!((UDPInterfaceClass *)PacketTransport)->Open_Spare_Socket(opened)) {
            return port;
        }
        PacketTransport->Start_Listening();
    }
    Debug_Printf("FirewallHelper - Getting mangling for port %d\n", source);
    timer = CDTimerClass<SystemTimerClass>(180);
    unsigned short mangled = 0;
    while (timer && !mangled) {
        Send_Mangler_Query(&address, source, unknown_mangler_packet_id_7e63d4, false);
        mangled = Wait_For_Mangler_Reply(unknown_mangler_packet_id_7e63d4, 30);
        Debug_Printf("FirewallHelper - Mangled port = %d\n", mangled);
    }
    unknown_mangler_packet_id_7e63d4++;
    if (opened) {
        ((UDPInterfaceClass *)PacketTransport)->Close_Spare_Socket(opened);
    }
    if (!mangled) {
        Debug_Printf("Couldn't find mangled port, returning source port\n");
        return port;
    }
    if (!sequential) {
        Debug_Printf("FirewallHelper - Returning next mangled port as %d\n", mangled);
        return mangled;
    }
    int result;
    if (type & 0x10) {
        result = unknown_08 + mangled;
    } else {
        int delta = unknown_08;
        if (delta == 100) {
            result = mangled - source + port + 100;
        } else {
            result = (mangled / delta + 1) * delta + port % delta;
        }
    }
    if (result > 0xFFFF) {
        result -= 0xFFFF;
    }
    if (result < 0x400) {
        result += 0x400;
    }
    Debug_Printf("FirewallHelper - Returning next mangled port as %d\n", result);
    return result;
}
