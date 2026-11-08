// Unknown84E8D0::Unknown_0042E370 (0x0042E370): places a beacon of the house at the coordinate in the
// slot (-1: the first free one; none free: nothing). The player's own placement shows the help lines,
// plays EVA_BeaconPlaced and the placement sound and sends packet 0x20 to the other players; an
// allied one raises a radar event (kind 11) and EVA_BeaconDetected.
#include "shared/beacon.hpp"
#include "shared/house.hpp"
#include "shared/session_state.hpp"
#include "shared/string_table.hpp"
#include "shared/message_list.hpp"
#include "shared/vox_class.hpp"
#include "shared/sound_play.hpp"
#include "shared/rules.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/debug_printf.hpp"
#include "shared/radar_event.hpp"
// @ref ??_C@_0BB@KGIJ@TXT_BEACON_HELP1?$AA@=007cf474
// @ref ??_C@_0BB@FJDA@TXT_BEACON_HELP2?$AA@=007cf460
// @ref ??_C@_0BC@BPJA@D?3?2Ra2?2Beacon?4CPP?$AA@=007cf488
// @ref ??_C@_0BB@HPKH@EVA_BeaconPlaced?$AA@=007cf44c
// @ref ??_C@_0CA@LLCO@Sending?5beacon?5placement?5to?5?$CFs?6?$AA@=007cf42c
// @ref ??_C@_0BD@GOLF@EVA_BeaconDetected?$AA@=007cf418

void Unknown84E8D0::Unknown_0042E370(int house, CoordStruct coord, int slot)
{
    if (!Unknown_0042E700(house) && slot == -1) {
        return;
    }
    UnknownBeacon *beacon = new UnknownBeacon;
    bool full = false;
    if (slot == -1) {
        full = true;
        int index;
        for (index = 0; index < 3; index++) {
            if (unknown_00[house][index] == 0) {
                full = false;
                break;
            }
        }
        if (!full) {
            slot = index;
            unknown_00[house][slot] = beacon;
        }
    } else {
        if (unknown_00[house][slot]) {
            delete unknown_00[house][slot];
            unknown_60--;
            unknown_00[house][slot] = beacon;
        } else {
            unknown_00[house][slot] = beacon;
        }
    }
    if (full) {
        delete beacon;
        return;
    }
    beacon->Unknown_0042DD60(coord, house);
    unknown_60++;
    if (unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 5) {
        if (house == unknown_player_a35db4->ArrayIndex) {
            unknown_session_a3d298.Messages.Add_Message(0, 0,
                StringTable::Load_String("TXT_BEACON_HELP1", 0, "D:\\Ra2\\Beacon.CPP", 811),
                unknown_player_a35db4->ColorSchemeIndex, 0x4046, 225, false);
            unknown_session_a3d298.Messages.Add_Message(0, 0,
                StringTable::Load_String("TXT_BEACON_HELP2", 0, "D:\\Ra2\\Beacon.CPP", 812),
                unknown_player_a35db4->ColorSchemeIndex, 0x4046, 225, false);
            Speech_Play_Name("EVA_BeaconPlaced", -1, -1);
            Voc_Play(Rules->PlaceBeaconSound, 1.0f, 0x2000, 0);
            UnknownBeaconPacket packet;
            packet.Type = 0x20;
            strcpy(packet.Name, Unknown_006FA120(unknown_players_a3fa7c[0]->Name));
            packet.X = coord.X;
            packet.Y = coord.Y;
            packet.Z = coord.Z;
            packet.Slot = (unsigned char)slot;
            packet.House = (unsigned char)house;
            for (int index = 1; index < unknown_players_a3fa7c.Count(); index++) {
                Debug_Printf("Sending beacon placement to %s\n", unknown_players_a3fa7c[index]);
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x3d, 1, &unknown_players_a3fa7c[index]->Address, 0, 0);
            }
        } else if (beacon->Unknown_0042E080()) {
            if (Unknown_0063B5B0(11, Cell(coord.X / 256, coord.Y / 256))) {
                Speech_Play_Name("EVA_BeaconDetected", -1, -1);
            }
        }
    }
}
