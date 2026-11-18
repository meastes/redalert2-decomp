/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the beacons that players place on the map in a multiplayer game: BeaconClass holds one beacon
**	(its position, owner and text) and BeaconManagerClass keeps three slots per house. TibSun has no such
**	file; the code is the executable's from 0x0042D9E0.
*/

#define GEOMETRY_H // RA2: geometry.h follows the includes, as its constants take their initializers after those of the other headers
#include "always.h" // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs
#include <wchar.h> // RA2: the includes this file needs
#include <windows.h>
#include <mmsystem.h> // RA2: the includes this file needs
#include "beacon.h" // RA2: the includes this file needs
#include "bitfont.h" // RA2: the includes this file needs
#include "bittext.h" // RA2: the includes this file needs
#include "convert.h" // RA2: the includes this file needs
#include "data.h" // RA2: the includes this file needs
#include "draw.h" // RA2: the includes this file needs
#include "dsurface.h" // RA2: the includes this file needs
#include "rect.h" // RA2: the includes this file needs
#include "shapeset.h" // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs
#include "_tactica.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "house.h" // RA2: the includes this file needs
#include "scheme.h" // RA2: the includes this file needs
#include "session.h" // RA2: the includes this file needs
#include "tactical.h" // RA2: the includes this file needs
#include "mixfile.h" // RA2: the includes this file needs
#include "rawfile.h" // RA2: the includes this file needs
#include "lightcon.h" // RA2: the includes this file needs
#include "radar.h" // RA2: the includes this file needs
#include "mouse.h" // RA2: the includes this file needs
#include "_map.h" // RA2: the includes this file needs
#include "conquer.h" // RA2: the includes this file needs
#include "ipxmgr.h" // RA2: the includes this file needs
#include "revent.h" // RA2: the includes this file needs
#undef LoadString // RA2: as above
#include "stringtable.h" // RA2: the includes this file needs
#include "voc.h" // RA2: the includes this file needs
#include "vox.h" // RA2: the includes this file needs
#include "_rules.h" // RA2: the includes this file needs
#include "rules.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include "geometry.h" // RA2: this module holds its own copy of the geometry constants (initialized first, at 0x0042D770)
#include "alphashp.h"


ShapeSet const * BeaconArt; // RA2: 0x0084E98C; PBEACON.SHP, the marker on the tactical view
ShapeSet const * RadarBeaconArt; // RA2: 0x0084E990; RDRBEACN.SHP, the marker on the radar

BeaconManagerClass BeaconManager; // RA2: 0x0084E8D0

/// <summary>
/// The distance between two positions.
/// </summary>
static inline int Beacon_Distance(Coord a, Coord b) // RA2: the executable computes it inline at each caller
{
	return((a - b).Length());
}

#pragma pack(push, 1)
/*
**	RA2: the packet that tells the other players about a beacon (placed: command 0x20, removed: 0x21); sent as 0x3D bytes.
*/
struct BeaconPacket {
	int Command; // RA2: +0x00
	char Name[43]; // RA2: +0x04; the sender's name
	Coord Position; // RA2: +0x2F; where the beacon was placed
	unsigned char ID; // RA2: +0x3B; the slot
	unsigned char House; // RA2: +0x3C
	char Rest[456 - 0x3D]; // RA2: the packet is 456 bytes on the stack
};
#pragma pack(pop)

#pragma pack(push, 1) // RA2: as above
/*
**	RA2: the packet that tells the other players about the text of a beacon (command 0x22); sent as 0x1C7 bytes.
*/
struct BeaconTextPacket { // RA2: as above
	int Command; // RA2: +0x00
	char Name[43]; // RA2: +0x04; the sender's name
	char Slot; // RA2: +0x2F; the slot of the beacon, found in the house's list
	char House; // RA2: +0x30
	wchar_t Text[128]; // RA2: +0x31; the text, or an empty string
	char Rest[456 - 0x131]; // RA2: the packet is 456 bytes on the stack
};
#pragma pack(pop) // RA2: as above

/// <summary>
/// Makes an empty beacon: no position, not active, not being edited and without text.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 9 bytes differ (adopted text)
BeaconClass::BeaconClass(void) // RA2: 0x0042D9E0
{
	BeaconCoord = COORD_NONE; // RA2: as above
	IsActive = 0; // RA2: as above
	IsFlashing = 0; // RA2: as above
	memset(Text, 0, sizeof(Text)); // RA2: as above
}
#endif

/// <summary>
/// Draws the beacon marker at its position on the tactical view and, when it has text, a box with the text below it.
/// </summary>
void BeaconClass::Draw(Surface * surface, Rect bounds) // RA2: 0x0042DA20
{
	ColorScheme * scheme = ColorSchemes[Houses[HouseID]->Scheme]; // RA2: the owner's colors
	Point2D pt; // RA2: as above
	if (!TacticalMap->Coord_To_Pixel(BeaconCoord, pt)) { // RA2: as above
		return; // RA2: as above
	}

	int half = BeaconManager.BeaconFrameCount / 2; // RA2: the first half of the frames is the idle marker, the second the one being edited
	Draw_Shape(*surface, *scheme->Converter, BeaconArt, (IsFlashing ? half : 0) + Frame % half, pt, bounds, (ShapeFlags_Type)0x600); // RA2: as above

	if (wcslen(Text) == 0) { // RA2: as above
		return; // RA2: as above
	}
	wchar_t text[128]; // RA2: a copy, so that the cursor can be blanked
	memcpy(text, Text, sizeof(text)); // RA2: as above

	if (IsFlashing) { // RA2: as above
		if (Session.Messages.Is_Edit() && timeGetTime() % 500 < 250) { // RA2: the text cursor blinks
			wchar_t * cursor = wcsrchr(text, L'_'); // RA2: as above
			if (cursor && wcslen(cursor) == 1) { // RA2: as above
				*cursor = L' '; // RA2: as above
			}
		}
	}
	if (!(IsFlashing && Session.Messages.Is_Edit())) { // RA2: when the text is not being edited, a trailing cursor or blank is cut off
		wchar_t * cursor = wcsrchr(text, L'_'); // RA2: as above
		if ((cursor && wcslen(cursor) == 1) || ((cursor = wcsrchr(text, L' ')) && wcslen(cursor) == 1)) { // RA2: as above
			*cursor = 0; // RA2: as above
		}
	}

	BitFont * font = BitFont::Instance; // RA2: as above
	int width = 0; // RA2: as above
	int height = 0; // RA2: as above
	font->Text_Dimensions(text, &width, &height, 200); // RA2: as above
	Rect box(pt.X - width / 2 - 4, pt.Y + 32, width + 8, height + 8); // RA2: the text box, centered below the marker
	RGBClass rgb = scheme->HSV; // RA2: as above
	Rect fill = Intersect(box, bounds, (int *)NULL, (int *)NULL); // RA2: as above
	int color = DSurface::Build_Hicolor_Pixel(rgb); // RA2: as above
	surface->Fill_Rect(fill, 0); // RA2: black background
	surface->Draw_Rect(fill, color); // RA2: and a frame in the owner's color
	font->Set_Clip(true); // RA2: as above
	LTRBStruct clip; // RA2: as above
	clip.Left = bounds.X; // RA2: as above
	clip.Top = bounds.Y; // RA2: as above
	clip.Right = bounds.X + bounds.Width - 1; // RA2: as above
	clip.Bottom = bounds.Y + bounds.Height - 1; // RA2: as above
	font->Set_Bounds(&clip); // RA2: as above
	font->Set_Color((unsigned short)color); // RA2: as above
	BitText::Instance->Draw_Text(font, surface, text, box.X + 4, box.Y + 4, 200, box.Height, 0, 0, 0); // RA2: as above
}

/// <summary>
/// Does nothing (a stub the executable keeps).
/// </summary>
void __stdcall Beacon_Stub_Seven(int, int, int, int, int, int, int) // RA2: 0x0042DD50; ret 28
{
}

/// <summary>
/// Takes the position (unless it is COORD_NONE) and, for an owner below 8, the owner, and makes the beacon active.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 9 bytes differ (adopted text)
void BeaconClass::Set_Coord_And_House(Coord coord, int house) // RA2: 0x0042DD60
{
	if (!(coord == COORD_NONE)) { // RA2: as above
		BeaconCoord = coord; // RA2: as above
	}
	if (house < 8) { // RA2: as above
		HouseID = house; // RA2: as above
		IsActive = 1; // RA2: as above
	}
}
#endif

/// <summary>
/// Sets whether the beacon is being edited.
/// </summary>
void BeaconClass::Set_Flashing(bool flashing) // RA2: 0x0042DDB0
{
	IsFlashing = flashing; // RA2: as above
}

/// <summary>
/// Does nothing (a stub the executable keeps).
/// </summary>
void Beacon_Stub(void) // RA2: 0x0042DDD0
{
}

/// <summary>
/// Makes the beacon inactive.
/// </summary>
void BeaconClass::Clear_Active(void) // RA2: 0x0042DDE0
{
	IsActive = 0; // RA2: as above
}

/// <summary>
/// Clears the beacon's text and copies up to 127 characters of the given text into it.
/// </summary>
void BeaconClass::Set_Text(wchar_t const * text) // RA2: 0x0042DDF0
{
	memset(Text, 0, sizeof(Text)); // RA2: as above
	if (text) { // RA2: as above
		wcsncpy(Text, text, 127); // RA2: as above
	}
}

/// <summary>
/// Draws the beacon's pulsing marker on the radar, or with toclear only restores the background under it.
/// </summary>
void BeaconClass::Draw_On_Radar(Surface * surface, Rect bounds, bool toclear) // RA2: 0x0042DE20
{
	int frame = Frame % BeaconManager.RadarBeaconAnimPeriod; // RA2: as above
	if (frame < BeaconManager.RadarBeaconFrameCount + 1 || toclear) { // RA2: as above
		Point2D pt = Map.Coord_To_Radar_Pixel(BeaconCoord, true); // RA2: as above
		if (frame < BeaconManager.RadarBeaconFrameCount && !toclear) { // RA2: as above
			Draw_Shape(*surface, *ColorSchemes[Houses[HouseID]->Scheme]->Converter, RadarBeaconArt, frame, pt, bounds, (ShapeFlags_Type)0x600); // RA2: as above
		}
		int width = BeaconManager.RadarBeaconSize.X; // RA2: as above
		int height = BeaconManager.RadarBeaconSize.Y; // RA2: as above
		Rect box(pt.X - width / 2, pt.Y - height / 2, width, height); // RA2: the marker's rectangle
		Map.LastDrawRect = Intersect(Union(Map.LastDrawRect, box.Bias_To(Map.RadarRect)), Map.RadarRect, (int *)NULL, (int *)NULL); // RA2: the part of the sidebar the radar has to copy
		for (int y = box.Y; y < box.Y + box.Height; y++) { // RA2: the radar background is redrawn under the marker
			for (int x = box.X; x < box.X + box.Width; x++) { // RA2: as above
				Map.Radar_Pixel(Point2D(x, y)); // RA2: as above
			}
		}
	}
}

/// <summary>
/// Whether the player may see the beacon: it is active and its owner is allied with the player (both ways) and not defeated.
/// </summary>
bool BeaconClass::Is_Visible_To_Player(void) const // RA2: 0x0042E080
{
	if (IsActive && PlayerPtr->Is_Ally((HousesType)HouseID) && Houses[HouseID]->Is_Ally(PlayerPtr) && !Houses[HouseID]->IsDefeated) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

/// <summary>
/// Makes a manager without beacons.
/// </summary>
BeaconManagerClass::BeaconManagerClass(void) // RA2: 0x0042E0E0
{
	memset(Beacons, 0, sizeof(Beacons)); // RA2: as above
	AllocatedCount = 0; // RA2: as above
	RadarBeaconAnimPeriod = 0; // RA2: as above
}

/// <summary>
/// Deletes every beacon.
/// </summary>
BeaconManagerClass::~BeaconManagerClass(void) // RA2: 0x0042E100
{
	Clear_Slots(); // RA2: as above
}

/// <summary>
/// Deletes every beacon and clears the slots.
/// </summary>
void BeaconManagerClass::Clear_Slots(void) // RA2: 0x0042E150
{
	if (AllocatedCount) { // RA2: as above
		for (int house = 0; house < 8; house++) { // RA2: as above
			for (int slot = 0; slot < 3; slot++) { // RA2: as above
				if (Beacons[house][slot]) { // RA2: as above
					delete Beacons[house][slot]; // RA2: as above
					Beacons[house][slot] = NULL; // RA2: as above
				}
			}
		}
		AllocatedCount = 0; // RA2: as above
	}
}

/// <summary>
/// Loads the beacon art for the tactical view and the radar, and keeps the sizes and frame counts.
/// </summary>
void BeaconManagerClass::Load_Art(void) // RA2: 0x0042E1A0
{
	RawFileClass beaconfile("PBEACON.SHP"); // RA2: as above
	BeaconArt = (ShapeSet const *)Load_Alloc_Data(beaconfile); // RA2: as above
	if (BeaconArt == NULL) { // RA2: as above
		BeaconArt = (ShapeSet const *)MixFileClass::Retrieve("PBEACON.SHP"); // RA2: as above
	}
	if (BeaconArt != NULL) { // RA2: as above
		BeaconSize.X = BeaconArt->Get_Width(); // RA2: as above
		BeaconSize.Y = BeaconArt->Get_Height(); // RA2: as above
		BeaconFrameCount = BeaconArt->Get_Count(); // RA2: as above
	} else {
		DebugString("Failed to load BeaconArt"); // RA2: as above
	}

	RawFileClass radarfile("RDRBEACN.SHP"); // RA2: as above
	RadarBeaconArt = (ShapeSet const *)Load_Alloc_Data(radarfile); // RA2: as above
	if (RadarBeaconArt == NULL) { // RA2: as above
		RadarBeaconArt = (ShapeSet const *)MixFileClass::Retrieve("RDRBEACN.SHP"); // RA2: as above
	}
	if (RadarBeaconArt != NULL) { // RA2: as above
		RadarBeaconSize.X = RadarBeaconArt->Get_Width(); // RA2: as above
		RadarBeaconSize.Y = RadarBeaconArt->Get_Height(); // RA2: as above
		RadarBeaconFrameCount = RadarBeaconArt->Get_Count(); // RA2: as above
		RadarBeaconAnimPeriod = RadarBeaconFrameCount << 2; // RA2: four ticks per radar frame
	} else {
		DebugString("Failed to load RadarBeaconArt"); // RA2: as above
	}
}

/// <summary>
/// Draws the beacons the player may see.
/// </summary>
void BeaconManagerClass::Draw(Surface * surface, Rect bounds) // RA2: 0x0042E290
{
	if (AllocatedCount) { // RA2: as above
		for (int house = 0; house < 8; house++) { // RA2: as above
			for (int slot = 0; slot < 3; slot++) { // RA2: as above
				BeaconClass * beacon = Beacons[house][slot]; // RA2: as above
				if (beacon && beacon->Is_Visible_To_Player()) { // RA2: as above
					beacon->Draw(surface, bounds); // RA2: as above
				}
			}
		}
	}
}

/// <summary>
/// Adds a beacon of the house, in its first free slot when id is -1 or else in the given slot (replacing the one there). In a network game it announces the beacon.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 18 bytes differ (adopted text)
void BeaconManagerClass::Place_Beacon(int house, Coord coord, int id) // RA2: 0x0042E370
{
	if (!CanPlaceBeacon(house) && id == -1) { // RA2: as above
		return; // RA2: as above
	}
	bool full = false; // RA2: as above
	BeaconClass * beacon = new BeaconClass; // RA2: as above
	if (id == -1) { // RA2: as above
		full = true; // RA2: as above
		int slot; // RA2: as above
		for (slot = 0; slot < 3; slot++) { // RA2: as above
			if (Beacons[house][slot] == NULL) { // RA2: as above
				full = false; // RA2: as above
				break; // RA2: as above
			}
		}
		if (!full) { // RA2: as above
			id = slot; // RA2: as above
			Beacons[house][id] = beacon; // RA2: as above
		}
	} else {
		if (Beacons[house][id]) { // RA2: as above
			delete Beacons[house][id]; // RA2: as above
			AllocatedCount--; // RA2: as above
		}
		Beacons[house][id] = beacon; // RA2: as above
	}
	if (full) { // RA2: as above
		delete beacon; // RA2: as above
		return; // RA2: as above
	}
	beacon->Set_Coord_And_House(coord, house); // RA2: as above
	AllocatedCount++; // RA2: as above
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) { // RA2: the network games
		if (house == PlayerPtr->HeapID) { // RA2: the player's own beacon: help text, the EVA line and the packet
			Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_BEACON_HELP1", NULL, "D:\\Ra2\\Beacon.CPP", 811), PlayerPtr->Scheme, TextPrintType(0x4046), 225, false); // RA2: as above
			Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_BEACON_HELP2", NULL, "D:\\Ra2\\Beacon.CPP", 812), PlayerPtr->Scheme, TextPrintType(0x4046), 225, false); // RA2: as above
			Queue_Speech("EVA_BeaconPlaced", -1, -1); // RA2: as above
			VocClass::PlayGlobal(Rule->PlaceBeaconSound, 1.0f, 0x2000); // RA2: as above
			BeaconPacket packet; // RA2: as above
			packet.Command = 0x20; // RA2: as above
			strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: as above
			packet.Position = coord; // RA2: as above
			packet.ID = (unsigned char)id; // RA2: as above
			packet.House = (unsigned char)house; // RA2: as above
			for (int player = 1; player < Session.Players.Count(); player++) { // RA2: as above
				DebugString("Sending beacon placement to %s\n", Session.Players[player]); // RA2: as above
				Ipx.Send_Global_Message(&packet, 0x3D, 1, &Session.Players[player]->Address, 0, 0); // RA2: as above
			}
		} else if (beacon->Is_Visible_To_Player()) { // RA2: an ally's beacon: radar event and EVA line
			if (Submit_Radar_Event((RadarEventType)11, Cell(coord.X / 256, coord.Y / 256))) { // RA2: as above
				Queue_Speech("EVA_BeaconDetected", -1, -1); // RA2: as above
			}
		}
	}
}
#endif

/// <summary>
/// Whether the house has a free beacon slot.
/// </summary>
bool BeaconManagerClass::CanPlaceBeacon(int house) // RA2: 0x0042E700
{
	for (int slot = 0; slot < 3; slot++) { // RA2: as above
		if (Beacons[house][slot] == NULL) { // RA2: as above
			return(true); // RA2: as above
		}
	}
	return(false); // RA2: as above
}

/// <summary>
/// Returns a zero point (a stub the executable keeps).
/// </summary>
Point2D __stdcall Beacon_Zero_Point(void) // RA2: 0x0042E730
{
	return(Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// Starts editing the beacon near the position, if there is one.
/// </summary>
bool BeaconManagerClass::Select_Beacon(Coord coord) // RA2: 0x0042E740
{
	BeaconClass * beacon = FindBeaconAt(coord); // RA2: as above
	if (beacon) { // RA2: as above
		Unselect_All(); // RA2: as above
		beacon->IsFlashing = 1; // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

/// <summary>
/// Stops editing every beacon.
/// </summary>
void BeaconManagerClass::Clear_Selection(void) // RA2: 0x0042E830
{
	if (AllocatedCount) { // RA2: as above
		for (int house = 0; house < 8; house++) { // RA2: as above
			for (int slot = 0; slot < 3; slot++) { // RA2: as above
				if (Beacons[house][slot]) { // RA2: as above
					Beacons[house][slot]->IsFlashing = 0; // RA2: as above
				}
			}
		}
	}
}

/// <summary>
/// Finds a beacon within 128 leptons of the position.
/// </summary>
BeaconClass * BeaconManagerClass::FindBeaconAt(Coord coord) // RA2: 0x0042E860
{
	if (AllocatedCount) { // RA2: as above
		for (int house = 0; house < 8; house++) { // RA2: as above
			for (int slot = 0; slot < 3; slot++) { // RA2: as above
				if (Beacons[house][slot] && Beacon_Distance(coord, Beacons[house][slot]->BeaconCoord) < 128) { // RA2: as above
					return(Beacons[house][slot]); // RA2: as above
				}
			}
		}
	}
	return(NULL); // RA2: as above
}

/// <summary>
/// Finds the beacon that is being edited, for the given house or for any house when it is -1.
/// </summary>
BeaconClass * BeaconManagerClass::FindSelected(int house) // RA2: 0x0042E940
{
	if (AllocatedCount) { // RA2: as above
		for (int index = 0; index < 8; index++) { // RA2: as above
			for (int slot = 0; slot < 3; slot++) { // RA2: as above
				BeaconClass * beacon = Beacons[index][slot]; // RA2: as above
				if (beacon && beacon->IsFlashing) { // RA2: as above
					if (house == -1) { // RA2: as above
						return(beacon); // RA2: as above
					}
					if (house == beacon->HouseID) { // RA2: as above
						return(beacon); // RA2: as above
					}
				}
			}
		}
	}
	return(NULL); // RA2: as above
}

/// <summary>
/// Removes a beacon: the one in the slot of the house, or with -1 and -1 the beacon that is being edited. A beacon of the player is announced to the other players.
/// </summary>
void BeaconManagerClass::Remove_Beacon(int house, int id) // RA2: 0x0042E990
{
	if (AllocatedCount) { // RA2: as above
		bool selected; // RA2: as above
		BeaconClass * beacon; // RA2: as above
		selected = house == -1 && id == -1; // RA2: as above
		if (selected) { // RA2: as above
			beacon = FindSelected(-1); // RA2: as above
		} else {
			beacon = Beacons[house][id]; // RA2: as above
		}
		if (beacon) { // RA2: as above
			int owner = beacon->HouseID; // RA2: as above
			if (owner != PlayerPtr->HeapID && selected) { // RA2: an ally's beacon is only hidden, not deleted
				beacon->IsActive = 0; // RA2: as above
				beacon->IsFlashing = 0; // RA2: as above
				return; // RA2: as above
			}
			if (beacon->Is_Visible_To_Player()) { // RA2: as above
				beacon->Draw_On_Radar(Map.RadarSurface, Map.RadarSurface->Get_Rect(), true); // RA2: restores the radar under the marker
			}
			delete beacon; // RA2: as above
			for (int slot = 0; slot < 3; slot++) { // RA2: as above
				if (beacon == Beacons[owner][slot]) { // RA2: as above
					Beacons[owner][slot] = NULL; // RA2: as above
					AllocatedCount--; // RA2: as above
					if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && owner == PlayerPtr->HeapID && selected) { // RA2: the player's own beacon in a network game is announced
						BeaconPacket packet; // RA2: as above
						packet.Command = 0x21; // RA2: as above
						strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: as above
						packet.House = (unsigned char)owner; // RA2: as above
						packet.ID = (unsigned char)slot; // RA2: as above
						for (int player = 1; player < Session.Players.Count(); player++) { // RA2: as above
							DebugString("Sending beacon delete to %s\n", Session.Players[player]); // RA2: as above
							Ipx.Send_Global_Message(&packet, 0x3D, 1, &Session.Players[player]->Address, 0, 0); // RA2: as above
						}
					}
					return; // RA2: as above
				}
			}
		}
	}
}

/// <summary>
/// Removes every beacon of the house.
/// </summary>
void BeaconManagerClass::Remove_All_Beacons(int house) // RA2: 0x0042EBE0
{
	if (AllocatedCount) { // RA2: as above
		for (int slot = 0; slot < 3; slot++) { // RA2: as above
			if (Beacons[house][slot]) { // RA2: as above
				Remove_Beacon(house, slot); // RA2: as above
			}
		}
	}
}

/// <summary>
/// Sets the text of a beacon: the one being edited when house and slot are both -1, else the one in the slot. A text the player typed is sent to the other players when it is done.
/// </summary>
void BeaconManagerClass::EditBeaconMessage(wchar_t const * text, int house, int slot, bool done) // RA2: 0x0042EC20
{
	bool edited; // RA2: as above
	BeaconClass * beacon; // RA2: as above
	edited = house == -1 && slot == -1; // RA2: as above
	if (edited) { // RA2: as above
		beacon = FindSelected(-1); // RA2: as above
	} else {
		beacon = Beacons[house][slot]; // RA2: as above
	}
	if (beacon) { // RA2: as above
		beacon->Set_Text(text); // RA2: as above
		if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) { // RA2: the network games
			if (edited && beacon->HouseID == PlayerPtr->HeapID && done) { // RA2: the player's own beacon is announced
				BeaconTextPacket packet; // RA2: as above
				packet.Command = 0x22; // RA2: as above
				strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: as above
				if (text) { // RA2: as above
					wcsncpy(packet.Text, text, 128); // RA2: as above
				} else {
					wcscpy(packet.Text, Narrow_To_Wide("")); // RA2: as above
				}
				packet.House = (char)PlayerPtr->HeapID; // RA2: as above
				packet.Slot = -1; // RA2: as above
				for (int index = 0; index < 3; index++) { // RA2: as above
					if (Beacons[packet.House][index] == beacon) { // RA2: as above
						packet.Slot = (char)index; // RA2: as above
					}
				}
				if (packet.Slot != -1) { // RA2: as above
					for (int player = 1; player < Session.Players.Count(); player++) { // RA2: as above
						DebugString("Sending beacon text to %s\n", Session.Players[player]); // RA2: as above
						Ipx.Send_Global_Message(&packet, 0x1C7, 1, &Session.Players[player]->Address, 0, 0); // RA2: as above
					}
				}
			} else if (beacon->HouseID != PlayerPtr->HeapID && beacon->Is_Visible_To_Player()) { // RA2: an ally's beacon: radar event
				Coord coord = beacon->BeaconCoord; // RA2: as above
				Submit_Radar_Event((RadarEventType)11, Cell(coord.X / 256, coord.Y / 256)); // RA2: as above
			}
		}
	}
}

/// <summary>
/// Marks the beacons of allies on the radar, during the lit phase of the animation.
/// </summary>
void BeaconManagerClass::Draw_On_Radar(Surface * surface, Rect bounds) // RA2: 0x0042EED0
{
	if (AllocatedCount && Frame % RadarBeaconAnimPeriod < BeaconManager.RadarBeaconFrameCount + 1) { // RA2: the frame count is read from the global manager directly, as BeaconClass::Draw_On_Radar does
		for (int house = 0; house < 8; house++) { // RA2: as above
			for (int slot = 0; slot < 3; slot++) { // RA2: as above
				BeaconClass * beacon = Beacons[house][slot]; // RA2: as above
				if (beacon && (beacon->IsActive) && PlayerPtr->Is_Ally((HousesType)beacon->HouseID) && Houses[beacon->HouseID]->Is_Ally(PlayerPtr) && !Houses[beacon->HouseID]->IsDefeated) { // RA2: the visibility test of Is_Visible_To_Player, open-coded
					beacon->Draw_On_Radar(surface, bounds, false); // RA2: as above
				}
			}
		}
	}
}

/// <summary>
/// Whether a visible beacon of an ally exists and the radar animation is in its lit phase.
/// </summary>
bool BeaconManagerClass::Is_Flashing(void) // RA2: 0x0042EFD0
{
	if (AllocatedCount) { // RA2: as above
		bool found = false; // RA2: as above
		for (int house = 0; house < 8; house++) { // RA2: as above
			int slot = 0; // RA2: as above
			do { // RA2: as above
				BeaconClass * beacon = Beacons[house][slot]; // RA2: as above
				if (beacon && (beacon->IsActive) && PlayerPtr->Is_Ally((HousesType)beacon->HouseID) && Houses[beacon->HouseID]->Is_Ally(PlayerPtr) && !Houses[beacon->HouseID]->IsDefeated) { // RA2: the visibility test of Is_Visible_To_Player, open-coded
					found = true; // RA2: as above
					break; // RA2: as above
				}
				slot++; // RA2: as above
			} while (slot < 3); // RA2: as above
		}
		if (found && Frame % RadarBeaconAnimPeriod < BeaconManager.RadarBeaconFrameCount + 1) { // RA2: the frame count is read from the global manager directly, as BeaconClass::Draw_On_Radar does
			return(true); // RA2: as above
		}
	}
	return(false); // RA2: as above
}
