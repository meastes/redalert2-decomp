// void TauntCommandClass::Execute(int key) const -- 0x0051B5F0
// Sends this taunt to the other players of a network game (modes 3 and 4) at most every five
// seconds: plays it locally, sends the packet to every connection named like a house that is a
// multiplayer house, and, when the router flags are set, to the router's flagged connections.
#define VECTOR_IMPLEMENT_ALL
#include "shared/command.hpp"
#include "shared/house_screen.hpp"
#include "shared/lobby.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/owner_talk.hpp"
#include "shared/connection.hpp"
#include "shared/taunt_packet.hpp"

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);
extern "C" char *__cdecl strcpy(char *destination, char const *source);
#pragma intrinsic(strcpy)
extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);

void TauntCommandClass::Execute(int key) const
{
    if (unknown_game_mode_a3d298 != 3 && unknown_game_mode_a3d298 != 4) {
        return;
    }
    if (MuteSWLaunches != 0) {
        return;
    }
    unsigned long now = timeGetTime();
    if (unknown_taunt_time_7db560 != 0xFFFFFFFF && (int)(now - unknown_taunt_time_7db560) <= 5000) {
        return;
    }
    if (!((unknown_flag_a3f120 && unknown_game_mode_a3d298 == 3) ||
          (unknown_flag_a3f121 && unknown_game_mode_a3d298 == 4))) {
        return;
    }
    unknown_taunt_time_7db560 = now;
    UnknownTauntPacket packet;
    packet.Type = 0x1D;
    strcpy(packet.Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name));
    packet.Session = Game_PlayerColor;
    packet.Checksum = Compute_Name_CRC_W(unknown_host_name_a3f618);
    unsigned char high = unknown_players_a3fa7c[0]->unknown_4b << 4;
    packet.Packed = ((TauntNumber & 0xf) ^ high) & 0xf ^ high;
    Play_Taunt(packet.Packed);
    packet.Muted = 0;
    int houses = Houses.Count();
    for (int h = 0; h < houses; h++) {
        HouseClass *house = Houses[h];
        if (house != 0 && house->Type->Multiplay && unknown_flags_a3f118[h]) {
            int connections = Ipx.Num_Connections();
            for (int c = 0; c < connections; c++) {
                int id = Ipx.Connection_ID(c);
                if (wcscmp(house->UIName, Ipx.Connection_Name(id)) == 0) {
                    IPXAddressClass *address = Ipx.Connection_Address(id);
                    if (unknown_flag_a3fba8) {
                        packet.Muted |= OwnerTalk.Mask_From_ID(id);
                    } else {
                        Ipx.Send_Global_Message(&packet, 0x1C7, 1, address, 0, 0);
                        Ipx.Service();
                    }
                }
            }
        }
    }
    if (unknown_flag_a3fba8 && packet.Muted) {
        Ipx.Send_Global_Message(&packet, 0x1C7, 1, &unknown_players_a3fa7c[0]->Address, 0, 0);
        Ipx.Service();
    }
}
