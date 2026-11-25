/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 RedAlert2 contributors
 ******************************************************************************/

#pragma once

#include "coord.h"
#include "rect.h"
#include "point.h"

class Surface;

/*
**	RA2: one beacon: where a player marked the map, with the short text that goes with it. A beacon is
**	0x114 bytes: the position, a byte of flags, the text and the owning house.
*/
class BeaconClass
{
	public:
		BeaconClass(void); // RA2: 0x0042D9E0
		void Draw(Surface * surface, Rect bounds); // RA2: 0x0042DA20; the marker and its text on the tactical view
		void Set_Coord_And_House(Coord coord, int house); // RA2: 0x0042DD60; takes a position (unless it is COORD_NONE) and an owner below 8, and becomes active
		void Set_Flashing(bool flashing); // RA2: 0x0042DDB0; the beacon is being edited
		void Clear_Active(void); // RA2: 0x0042DDE0
		void Set_Text(wchar_t const * text); // RA2: 0x0042DDF0; clears the text, then copies up to 127 characters
		void Draw_On_Radar(Surface * surface, Rect bounds, bool toclear = false); // RA2: 0x0042DE20; the pulsing marker on the radar, or its background restored when toclear
		bool Is_Visible_To_Player(void) const; // RA2: 0x0042E080; active, and owned by a house that is allied with the player both ways and not defeated

		Coord BeaconCoord; // RA2: +0x00
		unsigned char IsActive : 1; // RA2: +0x0C bit 0
		unsigned char IsFlashing : 1; // RA2: +0x0C bit 1; set while the owner edits the beacon (SelectBeacon sets it, ClearSelection clears it)
		wchar_t Text[128]; // RA2: +0x0E
		int HouseID; // RA2: +0x110
};

/*
**	RA2: the beacons that players place on the map in a multiplayer game.
*/
class BeaconManagerClass
{
	public:
		BeaconManagerClass(void); // RA2: 0x0042E0E0; no beacons
		~BeaconManagerClass(void); // RA2: 0x0042E100; deletes every beacon (Clear_Slots inlined)
		void Clear_Slots(void); // RA2: 0x0042E150; deletes every beacon
		void Load_Art(void); // RA2: 0x0042E1A0; loads PBEACON.SHP and RDRBEACN.SHP and takes their sizes and frame counts
		void Draw(Surface * surface, Rect bounds); // RA2: 0x0042E290; the beacons the player may see
		void Place_Beacon(int house, Coord coord, int id); // RA2: 0x0042E370; adds a beacon in a free slot of the house (id -1) or replaces the one in slot id, and tells the other players
		bool Select_Beacon(Coord coord); // RA2: 0x0042E740; starts editing the beacon near the position
		void Clear_Selection(void); // RA2: 0x0042E830; stops editing every beacon
		void Remove_All_Beacons(int house); // RA2: 0x0042EBE0; removes every beacon of the house
		BeaconClass * FindBeaconAt(Coord coord);
		bool CanPlaceBeacon(int owner); // RA2: 0x0042E700; the house may place another beacon (YR: CanPlaceBeacon)
		BeaconClass * FindSelected(int owner); // RA2: 0x0042E940; the house's beacon that is being edited, if any
		void EditBeaconMessage(wchar_t const * text, int house, int slot, bool done); // RA2: 0x0042EC20; text being typed for the beacon, or none to stop editing
		void Draw_On_Radar(Surface * surface, Rect bounds); // RA2: 0x0042EED0; marks the beacons of allies on the radar
		void Remove_Beacon(int x, int y); // RA2: 0x0042E990; removes a beacon; Command_Delete_Plan_Or_Beacon calls it with -1 and -1 when no planned node is selected
		bool Is_Flashing(void); // RA2: 0x0042EFD0; a visible beacon of an ally is in its lit phase

		BeaconClass * Beacons[8][3]; // RA2: +0x00; three slots per house
		int AllocatedCount; // RA2: +0x60; how many beacons exist
		Point2D BeaconSize; // RA2: +0x64; width and height of the PBEACON.SHP frames
		int BeaconFrameCount; // RA2: +0x6C; frames of PBEACON.SHP
		Point2D RadarBeaconSize; // RA2: +0x70; width and height of the RDRBEACN.SHP frames
		int RadarBeaconFrameCount; // RA2: +0x78; frames of RDRBEACN.SHP
		int RadarBeaconAnimPeriod; // RA2: +0x7C; four ticks per radar frame
};

extern BeaconManagerClass BeaconManager;

extern Rect const __fastcall Intersect(Rect const & bounding_rect, Rect const & draw_rect, int * x, int * y); // RA2: 0x0041FDF0, the compiled copy of rect.h's Intersect that the beacon draws call (moved from alphashp.h: object.cpp expands it)
