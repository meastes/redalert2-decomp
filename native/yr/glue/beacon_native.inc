
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-INS (lane N3, 2026-11-19), appended to the beacon unit: functions unmatched in
// ra2-decomp that were link-closure traps on the command bar's Beacon button (TabClass::AI -> FUN_00731A30:
// beacon mode; the map click -> DisplayClass::Mouse_Left_Release -> PlaceBeacon; every frame after,
// BeaconClass::Draw -> Drawing::Intersect). Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------

#include "stringtable.h"
#include "vox.h"
#include "rules.h"
#include "audevent.h"

// Drawing::Intersect (gamemd.exe's clip of a draw rectangle to bounds; the callers pass null offsets): the
// tree's own template Intersect (rect.h, TibSun's text) with the same arguments.
Rect __fastcall Drawing::Intersect(Rect const & a, Rect const & b, int * x, int * y)
{
	return(::Intersect(a, b, x, y));
}

// BeaconManagerClass::PlaceBeacon (gamemd.exe 0x00430BA0, 909 bytes): written from gamemd.exe's
// instructions (native/yr/tools/yr_dis.py) on the shape of Red Alert 2's accepted twin (game.exe 0x0042E370,
// src/recovered/beacon.cpp), with YR's names and the inlined BeaconClass::SetCoordAndHouse and
// VisibleToPlayer called: a beacon of the house in the slot (-1: the first free one; none free: nothing).
// In a network game the player's own placement shows the help lines, plays EVA_BeaconPlaced and the
// placement sound and sends packet 0x20 to the other players; an allied one raises radar event 11 and
// EVA_BeaconDetected.
#pragma pack(push, 1)
struct BeaconPlacePacket {
	int Command;
	char Name[43];
	int X;
	int Y;
	int Z;
	unsigned char ID;
	unsigned char House;
	char Rest[456 - 0x3d];
};
#pragma pack(pop)

void BeaconManagerClass::PlaceBeacon(int house, Coord coord, int id)
{
	if (!CanPlaceBeacon(house) && id == -1) {
		return;
	}
	BeaconClass * beacon = new BeaconClass;
	if (id == -1) {
		int slot;
		for (slot = 0; slot < 3; slot++) {
			if (Beacons[house][slot] == NULL) {
				break;
			}
		}
		if (slot == 3) {
			delete beacon;
			return;
		}
		id = slot;
		Beacons[house][id] = beacon;
	} else {
		if (Beacons[house][id] != NULL) {
			delete Beacons[house][id];
			AllocatedCount--;
		}
		Beacons[house][id] = beacon;
	}
	beacon->SetCoordAndHouse(coord, house);
	AllocatedCount++;
	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
		return;
	}
	if (house == PlayerPtr->HeapID) {
		Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_BEACON_HELP1", NULL, "D:\\ra2mdpost\\Beacon.CPP", 0x32b),
			PlayerPtr->Scheme, TextPrintType(0x4046), 225, false);
		Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_BEACON_HELP2", NULL, "D:\\ra2mdpost\\Beacon.CPP", 0x32c),
			PlayerPtr->Scheme, TextPrintType(0x4046), 225, false);
		VoxClass::Play("EVA_BeaconPlaced", -1, -1);
		VocClass::PlayGlobal(Rule->PlaceBeaconSound, 1.0f, 0x2000);
		BeaconPlacePacket packet;
		packet.Command = 0x20;
		strcpy(packet.Name, FUN_007350C0(Session.Players[0]->Name));
		packet.X = coord.X;
		packet.Y = coord.Y;
		packet.Z = coord.Z;
		packet.ID = (unsigned char)id;
		packet.House = (unsigned char)house;
		for (int p = 1; p < Session.Players.Count(); p++) {
			FUN_004068E0("Sending beacon placement to %s\n", Session.Players[p]);
			Ipx.Send_Global_Message(&packet, 0x3d, 1, &Session.Players[p]->Address, 0, 0);
		}
	} else if (beacon->VisibleToPlayer()) {
		if (RadarEventClass::Create((RadarEventType)0xb, coord.As_Cell())) {
			VoxClass::Play("EVA_BeaconDetected", -1, -1);
		}
	}
}
