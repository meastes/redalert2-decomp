// NON-EXACT DRAFT 0x673090 FUN_00673090 (SessionClass::Allocate_Ports)
// entry 0x673090; exact-match score 94.42 (misses_index best); this text 82.90 (codex4 r506); plan "draft, verify"
// origin lane: Scout775 (build/class-work/Scout775/keep/ap_z1_8290_loose9238.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1257 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1257): the eight 0x24-byte name rows at +0x2DD0 (header: char[0x120]) indexed by
// offset, the address port read as its first word; each changed line is marked NATIVE.
#include "shared/session_state.hpp"
#include "shared/vector_class.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_memory.hpp"
#include "shared/debug_printf.hpp"
#include "shared/wol_sink.hpp"
#include "shared/wstring.hpp"
#include "shared/udp_interface.hpp"
void SessionClass::Allocate_Ports_00673090()
{
    int bump = 1;
    int index;
    int other;
    int names;
    int slot;
    DynamicVectorClass<NodeNameType *> order;
    char used[8];

    Debug_Printf("Allocating port numbers and local addresses\n");
    if (Players.Count() < 2) {
        return;
    }
    names = 0;
    memset(unknown_2dd0, 0, sizeof(unknown_2dd0));
    unknown_2fc0 = true;
    for (index = 0; index < Players.Count(); index++) {
        slot = Unknown_00672F90(index);
        if (slot >= 0 && slot < 8) {
            Players[index]->Address.Set_Port(unknown_2fa0.unknown[slot]);
            char *name = Wide_To_Narrow_006fa120(Players[index]->Name);
            Debug_Printf("Allocated port for player %s - %s\n", name, Players[index]->Address.As_String());
        }
    }

    memset(used, 0, sizeof(used));
    for (index = 0; index < Players.Count(); index++) {
        int best = 0x7FFFFFFF;
        int lowest = 0x7FFFFFFF;
        for (other = 0; other < Players.Count(); other++) {
            if (!used[other] && Players[other]->unknown_53 < lowest) {
                lowest = Players[other]->unknown_53;
                best = other;
            }
        }
        if (best == 0x7FFFFFFF) {
            break;
        }
        used[best] = true;
        order.Add(Players[best]);
        if (order.Count() == Players.Count()) {
            break;
        }
    }

    memset(used, 0, sizeof(used));
    for (index = 0; index < order.Count(); index++) {
        if (used[index]) {
            continue;
        }
        IPXAddressClass address = order[index]->Address;
        for (other = 0; other < order.Count(); other++) {
            if (other != index && !used[other] && address.Is_Same_Node(order[other]->Address)) {
                int first = Unknown_00672FF0(Wide_To_Narrow_006fa120(order[other]->Name));
                int second = Unknown_00672FF0(Wide_To_Narrow_006fa120(order[index]->Name));
                if (unknown_2fa0.unknown[first] == unknown_2fa0.unknown[second]) {
                    order[other]->Address.Set_Port(unknown_2fa0.unknown[first] + bump++);
                    used[other] = true;
                    Debug_Printf("Adjusted port number to %d for player %s\n", (unsigned short)bump - 1,
                        Wide_To_Narrow_006fa120(order[other]->Name));
                } else {
                    Debug_Printf("Two player behand the same NAT but they are using different port bases anyway\n");
                }
                char *name = Wide_To_Narrow_006fa120(order[index]->Name);
                if (_stricmp(name, unknown_wstring_b26cb8.Get()) == 0) {
                    strcpy(unknown_2dd0 + 0x24 * names++, Wide_To_Narrow_006fa120(order[other]->Name));   // NATIVE: char[8][0x24] rows (codex4 r506)
                }
                name = Wide_To_Narrow_006fa120(order[other]->Name);
                if (_stricmp(name, unknown_wstring_b26cb8.Get()) == 0) {
                    strcpy(unknown_2dd0 + 0x24 * names++, Wide_To_Narrow_006fa120(order[index]->Name));   // NATIVE: char[8][0x24] rows
                }
            }
        }
    }

    if (PortNumberOverride != 0) {
        Debug_Printf("Using %d for local port\n", PortNumberOverride);
        Players[0]->Address.Set_Port(PortNumberOverride);
        PlayerSlots[Unknown_00672F90(0)]->Address.Set_Port(PortNumberOverride);
    }
    for (index = 0; index < Players.Count(); index++) {
        unknown_3022.unknown[index] = *(unsigned short *)Players[index]->Address.NetworkNumber;   // NATIVE: the UDP port word (the header accessor is guarded off in this unit)
        if (unknown_3022.unknown[index] == 0) {
            unknown_3022.unknown[index] = 1234;
        }
    }

    IPXAddressClass local = Players[0]->Address;
    NetNodeType node;
    local.Get_Node(node);
    for (index = 1; index < Players.Count(); index++) {
        if (local.Is_Same_Node(Players[index]->Address)) {
            Debug_Printf("%s has the same IP address as me\n", Wide_To_Narrow_006fa120(Players[index]->Name));
            for (slot = 0; slot < 8; slot++) {
                NodeNameType *entry = PlayerSlots[slot];
                char *name = Wide_To_Narrow_006fa120(Players[index]->Name);
                if (_stricmp(name, Wide_To_Narrow_006fa120(entry->Name)) == 0) {
                    NetNodeType nat;
                    unknown_293c.Addresses[slot].Get_Node(nat);
                    Players[index]->Address.Set_Address(0, nat, *(unsigned short *)Players[index]->Address.NetworkNumber);   // NATIVE: the UDP port word
                    name = Wide_To_Narrow_006fa120(Players[index]->Name);
                    Debug_Printf("Talking to NAT player %s on IP address %s\n", name, Players[index]->Address.As_String());
                }
            }
        }
    }

    if (PacketTransport) {
        ((UDPInterfaceClass *)PacketTransport)->Close_All_Port_Sockets();   // slot 28 (was spelled unknown_slot28)
        // behaviour fix (codex4 r506): the original calls slot 23 OpenSocket(port), not slot 15 Open_Socket.
        ((UDPInterfaceClass *)PacketTransport)->OpenSocket(unknown_3022.unknown[0]);
        PacketTransport->Start_Listening();
    }
}
// @ref ??_C@_0CN@NAB@Allocating?5port?5numbers?5and?5loca@=007f3cdc
// @ref ??_C@_0CD@LBNG@Allocated?5port?5for?5player?5?$CFs?5?9?5?$CF@=007f3cb8
// @ref ??_C@_0CK@HMFD@Adjusted?5port?5number?5to?5?$CFd?5for?5p@=007f3c8c
// @ref ??_C@_0EP@EAAE@Two?5player?5behand?5the?5same?5NAT?5b@=007f3c3c
// @ref ??_C@_0BJ@JGMO@Using?5?$CFd?5for?5local?5port?6?$AA@=007f3c20
// @ref ??_C@_0CC@MBMH@?$CFs?5has?5the?5same?5IP?5address?5as?5me@=007f3bfc
// @ref ??_C@_0CL@HGMK@Talking?5to?5NAT?5player?5?$CFs?5on?5IP?5a@=007f3bd0
