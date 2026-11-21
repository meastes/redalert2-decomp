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

#include "always.h"
#include "rect.h"
#include "geometry.h"
#include <string.h>
#include <wchar.h>
#include <windows.h>
#include <mmsystem.h>
#include "beacon.h"
#include "beaconpacket.h"
#include "beacontextpacket.h"
#include "bitfont.h"
#include "bittext.h"
#include "convert.h"
#include "data.h"
#include "draw.h"
#include "dsurface.h"
#include "shapeset.h"
#include "vector.h"
#include "_tactica.h"
#include "globals.h"
#include "house.h"
#include "scheme.h"
#include "session.h"
#include "tactical.h"
#include "mixfile.h"
#include "rawfile.h"
#include "lightcon.h"
#include "radar.h"
#include "mouse.h"
#include "_map.h"
#include "conquer.h"
#include "ipxmgr.h"
#include "revent.h"
#undef LoadString
#include "stringtable.h"
#include "voc.h"
#include "vox.h"
#include "_rules.h"
#include "rules.h"
#include "dbgprint.h"
#include "alphashp.h"


ShapeSet const * BeaconArt;
ShapeSet const * RadarBeaconArt;

BeaconManagerClass BeaconManager;

/// <summary>
/// The distance between two positions.
/// </summary>
static inline int Beacon_Distance(Coord a, Coord b)
{
	return((a - b).Length());
}

/*
**	RA2: the packet that tells the other players about a beacon (placed: command 0x20, removed: 0x21); sent as 0x3D bytes.
*/


/*
**	RA2: the packet that tells the other players about the text of a beacon (command 0x22); sent as 0x1C7 bytes.
*/


/// <summary>
/// Makes an empty beacon: no position, not active, not being edited and without text.
/// </summary>
BeaconClass::BeaconClass(void)
{
	BeaconCoord = COORD_NONE;
	IsActive = 0;
	IsFlashing = 0;
	memset(Text, 0, sizeof(Text));
}
/// <summary>
/// Draws the beacon marker at its position on the tactical view and, when it has text, a box with the text below it.
/// </summary>
void BeaconClass::Draw(Surface * surface, Rect bounds)
{
	ColorScheme * scheme = ColorSchemes[Houses[HouseID]->Scheme];
	Point2D pt;
	if (!TacticalMap->Coord_To_Pixel(BeaconCoord, pt)) {
		return;
	}

	int half = BeaconManager.BeaconFrameCount / 2;
	Draw_Shape(*surface, *scheme->Converter, BeaconArt, (IsFlashing ? half : 0) + Frame % half, pt, bounds, (ShapeFlags_Type)(SHAPE_CENTER | SHAPE_WIN_REL));

	if (wcslen(Text) == 0) {
		return;
	}
	wchar_t text[128];
	memcpy(text, Text, sizeof(text));

	if (IsFlashing) {
		if (Session.Messages.Is_Edit() && timeGetTime() % 500 < 250) {
			wchar_t * cursor = wcsrchr(text, L'_');
			if (cursor && wcslen(cursor) == 1) {
				*cursor = L' ';
			}
		}
	}
	if (!(IsFlashing && Session.Messages.Is_Edit())) {
		wchar_t * cursor = wcsrchr(text, L'_');
		if ((cursor && wcslen(cursor) == 1) || ((cursor = wcsrchr(text, L' ')) && wcslen(cursor) == 1)) {
			*cursor = 0;
		}
	}

	BitFont * font = BitFont::Instance;
	int width = 0;
	int height = 0;
	font->Text_Dimensions(text, &width, &height, 200);
	Rect box(pt.X - width / 2 - 4, pt.Y + 32, width + 8, height + 8);
	RGBClass rgb = scheme->HSV;
	Rect fill = Intersect(box, bounds, (int *)NULL, (int *)NULL);
	int color = DSurface::Build_Hicolor_Pixel(rgb);
	surface->Fill_Rect(fill, 0);
	surface->Draw_Rect(fill, color);
	font->Set_Clip(true);
	LTRBStruct clip;
	clip.Left = bounds.X;
	clip.Top = bounds.Y;
	clip.Right = bounds.X + bounds.Width - 1;
	clip.Bottom = bounds.Y + bounds.Height - 1;
	font->Set_Bounds(&clip);
	font->Set_Color((unsigned short)color);
	BitText::Instance->Draw_Text(font, surface, text, box.X + 4, box.Y + 4, 200, box.Height, 0, 0, 0);
}

/// <summary>
/// Does nothing (a stub the executable keeps).
/// </summary>
void __stdcall Beacon_Stub_Seven(int, int, int, int, int, int, int)
{
}

/// <summary>
/// Takes the position (unless it is COORD_NONE) and, for an owner below 8, the owner, and makes the beacon active.
/// </summary>
void BeaconClass::Set_Coord_And_House(Coord coord, int house)
{
	if (!(coord == COORD_NONE)) {
		BeaconCoord = coord;
	}
	if (house < 8) {
		HouseID = house;
		IsActive = 1;
	}
}
/// <summary>
/// Sets whether the beacon is being edited.
/// </summary>
void BeaconClass::Set_Flashing(bool flashing)
{
	IsFlashing = flashing;
}

/// <summary>
/// Does nothing (a stub the executable keeps).
/// </summary>
void Beacon_Stub(void)
{
}

/// <summary>
/// Makes the beacon inactive.
/// </summary>
void BeaconClass::Clear_Active(void)
{
	IsActive = 0;
}

/// <summary>
/// Clears the beacon's text and copies up to 127 characters of the given text into it.
/// </summary>
void BeaconClass::Set_Text(wchar_t const * text)
{
	memset(Text, 0, sizeof(Text));
	if (text) {
		wcsncpy(Text, text, 127);
	}
}

/// <summary>
/// Draws the beacon's pulsing marker on the radar, or with toclear only restores the background under it.
/// </summary>
void BeaconClass::Draw_On_Radar(Surface * surface, Rect bounds, bool toclear)
{
	int frame = Frame % BeaconManager.RadarBeaconAnimPeriod;
	if (frame < BeaconManager.RadarBeaconFrameCount + 1 || toclear) {
		Point2D pt = Map.Coord_To_Radar_Pixel(BeaconCoord, true);
		if (frame < BeaconManager.RadarBeaconFrameCount && !toclear) {
			Draw_Shape(*surface, *ColorSchemes[Houses[HouseID]->Scheme]->Converter, RadarBeaconArt, frame, pt, bounds, (ShapeFlags_Type)(SHAPE_CENTER | SHAPE_WIN_REL));
		}
		int width = BeaconManager.RadarBeaconSize.X;
		int height = BeaconManager.RadarBeaconSize.Y;
		Rect box(pt.X - width / 2, pt.Y - height / 2, width, height);
		Map.LastDrawRect = Intersect(Union(Map.LastDrawRect, box.Bias_To(Map.RadarRect)), Map.RadarRect, (int *)NULL, (int *)NULL);
		for (int y = box.Y; y < box.Y + box.Height; y++) {
			for (int x = box.X; x < box.X + box.Width; x++) {
				Map.Radar_Pixel(Point2D(x, y));
			}
		}
	}
}

/// <summary>
/// Whether the player may see the beacon: it is active and its owner is allied with the player (both ways) and not defeated.
/// </summary>
bool BeaconClass::Is_Visible_To_Player(void) const
{
	if (IsActive && PlayerPtr->Is_Ally((HousesType)HouseID) && Houses[HouseID]->Is_Ally(PlayerPtr) && !Houses[HouseID]->IsDefeated) {
		return(true);
	}
	return(false);
}

/// <summary>
/// Makes a manager without beacons.
/// </summary>
BeaconManagerClass::BeaconManagerClass(void)
{
	memset(Beacons, 0, sizeof(Beacons));
	AllocatedCount = 0;
	RadarBeaconAnimPeriod = 0;
}

/// <summary>
/// Deletes every beacon.
/// </summary>
BeaconManagerClass::~BeaconManagerClass(void)
{
	Clear_Slots();
}

/// <summary>
/// Deletes every beacon and clears the slots.
/// </summary>
void BeaconManagerClass::Clear_Slots(void)
{
	if (AllocatedCount) {
		for (int house = 0; house < 8; house++) {
			for (int slot = 0; slot < 3; slot++) {
				if (Beacons[house][slot]) {
					delete Beacons[house][slot];
					Beacons[house][slot] = NULL;
				}
			}
		}
		AllocatedCount = 0;
	}
}

/// <summary>
/// Loads the beacon art for the tactical view and the radar, and keeps the sizes and frame counts.
/// </summary>
void BeaconManagerClass::Load_Art(void)
{
	RawFileClass beaconfile("PBEACON.SHP");
	BeaconArt = (ShapeSet const *)Load_Alloc_Data(beaconfile);
	if (BeaconArt == NULL) {
		BeaconArt = (ShapeSet const *)MixFileClass::Retrieve("PBEACON.SHP");
	}
	if (BeaconArt != NULL) {
		BeaconSize.X = BeaconArt->Get_Width();
		BeaconSize.Y = BeaconArt->Get_Height();
		BeaconFrameCount = BeaconArt->Get_Count();
	} else {
		DebugString("Failed to load BeaconArt");
	}

	RawFileClass radarfile("RDRBEACN.SHP");
	RadarBeaconArt = (ShapeSet const *)Load_Alloc_Data(radarfile);
	if (RadarBeaconArt == NULL) {
		RadarBeaconArt = (ShapeSet const *)MixFileClass::Retrieve("RDRBEACN.SHP");
	}
	if (RadarBeaconArt != NULL) {
		RadarBeaconSize.X = RadarBeaconArt->Get_Width();
		RadarBeaconSize.Y = RadarBeaconArt->Get_Height();
		RadarBeaconFrameCount = RadarBeaconArt->Get_Count();
		RadarBeaconAnimPeriod = RadarBeaconFrameCount << 2;
	} else {
		DebugString("Failed to load RadarBeaconArt");
	}
}

/// <summary>
/// Draws the beacons the player may see.
/// </summary>
void BeaconManagerClass::Draw(Surface * surface, Rect bounds)
{
	if (AllocatedCount) {
		for (int house = 0; house < 8; house++) {
			for (int slot = 0; slot < 3; slot++) {
				BeaconClass * beacon = Beacons[house][slot];
				if (beacon && beacon->Is_Visible_To_Player()) {
					beacon->Draw(surface, bounds);
				}
			}
		}
	}
}

/// <summary>
/// Adds a beacon of the house, in its first free slot when id is -1 or else in the given slot (replacing the one there). In a network game it announces the beacon.
/// </summary>
void BeaconManagerClass::Place_Beacon(int house, Coord coord, int id)
{
	if (!CanPlaceBeacon(house) && id == -1) {
		return;
	}
	bool full = false;
	BeaconClass * beacon = new BeaconClass;
	if (id == -1) {
		full = true;
		int slot;
		for (slot = 0; slot < 3; slot++) {
			if (Beacons[house][slot] == NULL) {
				full = false;
				break;
			}
		}
		if (!full) {
			id = slot;
			Beacons[house][id] = beacon;
		}
	} else {
		if (Beacons[house][id]) {
			delete Beacons[house][id];
			AllocatedCount--;
		}
		Beacons[house][id] = beacon;
	}
	if (full) {
		delete beacon;
		return;
	}
	beacon->Set_Coord_And_House(coord, house);
	AllocatedCount++;
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
		if (house == PlayerPtr->HeapID) {
			Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_BEACON_HELP1", NULL, "D:\\Ra2\\Beacon.CPP", 811), PlayerPtr->Scheme, TextPrintType(0x4046), 225, false);
			Session.Messages.Add_Message(NULL, 0, StringTable::LoadString("TXT_BEACON_HELP2", NULL, "D:\\Ra2\\Beacon.CPP", 812), PlayerPtr->Scheme, TextPrintType(0x4046), 225, false);
			Queue_Speech("EVA_BeaconPlaced", -1, -1);
			VocClass::PlayGlobal(Rule->PlaceBeaconSound, 1.0f, 0x2000);
			BeaconPacket packet;
			packet.Command = 0x20;
			strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name));
			packet.Position = coord;
			packet.ID = (unsigned char)id;
			packet.House = (unsigned char)house;
			for (int player = 1; player < Session.Players.Count(); player++) {
				DebugString("Sending beacon placement to %s\n", Session.Players[player]);
				Ipx.Send_Global_Message(&packet, 0x3D, 1, &Session.Players[player]->Address, 0, 0);
			}
		} else if (beacon->Is_Visible_To_Player()) {
			if (Submit_Radar_Event(RADAREVENT_BEACON_PLACED, Cell(coord.X / 256, coord.Y / 256))) {
				Queue_Speech("EVA_BeaconDetected", -1, -1);
			}
		}
	}
}

/// <summary>
/// Whether the house has a free beacon slot.
/// </summary>
bool BeaconManagerClass::CanPlaceBeacon(int house)
{
	for (int slot = 0; slot < 3; slot++) {
		if (Beacons[house][slot] == NULL) {
			return(true);
		}
	}
	return(false);
}

/// <summary>
/// Returns a zero point (a stub the executable keeps).
/// </summary>
Point2D __stdcall Beacon_Zero_Point(void)
{
	return(Point2D(0, 0));
}

/// <summary>
/// Starts editing the beacon near the position, if there is one.
/// </summary>
bool BeaconManagerClass::Select_Beacon(Coord coord)
{
	BeaconClass * beacon = FindBeaconAt(coord);
	if (beacon) {
		Unselect_All();
		beacon->IsFlashing = 1;
		return(true);
	}
	return(false);
}

/// <summary>
/// Stops editing every beacon.
/// </summary>
void BeaconManagerClass::Clear_Selection(void)
{
	if (AllocatedCount) {
		for (int house = 0; house < 8; house++) {
			for (int slot = 0; slot < 3; slot++) {
				if (Beacons[house][slot]) {
					Beacons[house][slot]->IsFlashing = 0;
				}
			}
		}
	}
}

/// <summary>
/// Finds a beacon within 128 leptons of the position.
/// </summary>
BeaconClass * BeaconManagerClass::FindBeaconAt(Coord coord)
{
	if (AllocatedCount) {
		for (int house = 0; house < 8; house++) {
			for (int slot = 0; slot < 3; slot++) {
				if (Beacons[house][slot] && Beacon_Distance(coord, Beacons[house][slot]->BeaconCoord) < 128) {
					return(Beacons[house][slot]);
				}
			}
		}
	}
	return(NULL);
}

/// <summary>
/// Finds the beacon that is being edited, for the given house or for any house when it is -1.
/// </summary>
BeaconClass * BeaconManagerClass::FindSelected(int house)
{
	if (AllocatedCount) {
		for (int index = 0; index < 8; index++) {
			for (int slot = 0; slot < 3; slot++) {
				BeaconClass * beacon = Beacons[index][slot];
				if (beacon && beacon->IsFlashing) {
					if (house == -1) {
						return(beacon);
					}
					if (house == beacon->HouseID) {
						return(beacon);
					}
				}
			}
		}
	}
	return(NULL);
}

/// <summary>
/// Removes a beacon: the one in the slot of the house, or with -1 and -1 the beacon that is being edited. A beacon of the player is announced to the other players.
/// </summary>
void BeaconManagerClass::Remove_Beacon(int house, int id)
{
	if (AllocatedCount) {
		bool selected;
		BeaconClass * beacon;
		selected = house == -1 && id == -1;
		if (selected) {
			beacon = FindSelected(-1);
		} else {
			beacon = Beacons[house][id];
		}
		if (beacon) {
			int owner = beacon->HouseID;
			if (owner != PlayerPtr->HeapID && selected) {
				beacon->IsActive = 0;
				beacon->IsFlashing = 0;
				return;
			}
			if (beacon->Is_Visible_To_Player()) {
				beacon->Draw_On_Radar(Map.RadarSurface, Map.RadarSurface->Get_Rect(), true);
			}
			delete beacon;
			for (int slot = 0; slot < 3; slot++) {
				if (beacon == Beacons[owner][slot]) {
					Beacons[owner][slot] = NULL;
					AllocatedCount--;
					if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && owner == PlayerPtr->HeapID && selected) {
						BeaconPacket packet;
						packet.Command = 0x21;
						strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name));
						packet.House = (unsigned char)owner;
						packet.ID = (unsigned char)slot;
						for (int player = 1; player < Session.Players.Count(); player++) {
							DebugString("Sending beacon delete to %s\n", Session.Players[player]);
							Ipx.Send_Global_Message(&packet, 0x3D, 1, &Session.Players[player]->Address, 0, 0);
						}
					}
					return;
				}
			}
		}
	}
}


/// <summary>
/// Removes every beacon of the house.
/// </summary>
void BeaconManagerClass::Remove_All_Beacons(int house)
{
	if (AllocatedCount) {
		for (int slot = 0; slot < 3; slot++) {
			if (Beacons[house][slot]) {
				Remove_Beacon(house, slot);
			}
		}
	}
}

/// <summary>
/// Sets the text of a beacon: the one being edited when house and slot are both -1, else the one in the slot. A text the player typed is sent to the other players when it is done.
/// </summary>
void BeaconManagerClass::EditBeaconMessage(wchar_t const * text, int house, int slot, bool done)
{
	bool edited;
	BeaconClass * beacon;
	edited = house == -1 && slot == -1;
	if (edited) {
		beacon = FindSelected(-1);
	} else {
		beacon = Beacons[house][slot];
	}
	if (beacon) {
		beacon->Set_Text(text);
		if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
			if (edited && beacon->HouseID == PlayerPtr->HeapID && done) {
				BeaconTextPacket packet;
				packet.Command = 0x22;
				strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name));
				if (text) {
					wcsncpy(packet.Text, text, 128);
				} else {
					wcscpy(packet.Text, Narrow_To_Wide(""));
				}
				packet.House = (char)PlayerPtr->HeapID;
				packet.Slot = -1;
				for (int index = 0; index < 3; index++) {
					if (Beacons[packet.House][index] == beacon) {
						packet.Slot = (char)index;
					}
				}
				if (packet.Slot != -1) {
					for (int player = 1; player < Session.Players.Count(); player++) {
						DebugString("Sending beacon text to %s\n", Session.Players[player]);
						Ipx.Send_Global_Message(&packet, 0x1C7, 1, &Session.Players[player]->Address, 0, 0);
					}
				}
			} else if (beacon->HouseID != PlayerPtr->HeapID && beacon->Is_Visible_To_Player()) {
				Coord coord = beacon->BeaconCoord;
				Submit_Radar_Event(RADAREVENT_BEACON_PLACED, Cell(coord.X / 256, coord.Y / 256));
			}
		}
	}
}


/// <summary>
/// Marks the beacons of allies on the radar, during the lit phase of the animation.
/// </summary>
void BeaconManagerClass::Draw_On_Radar(Surface * surface, Rect bounds)
{
	if (AllocatedCount && Frame % RadarBeaconAnimPeriod < BeaconManager.RadarBeaconFrameCount + 1) {
		for (int house = 0; house < 8; house++) {
			for (int slot = 0; slot < 3; slot++) {
				BeaconClass * beacon = Beacons[house][slot];
				if (beacon && (beacon->IsActive) && PlayerPtr->Is_Ally((HousesType)beacon->HouseID) && Houses[beacon->HouseID]->Is_Ally(PlayerPtr) && !Houses[beacon->HouseID]->IsDefeated) {
					beacon->Draw_On_Radar(surface, bounds, false);
				}
			}
		}
	}
}

/// <summary>
/// Whether a visible beacon of an ally exists and the radar animation is in its lit phase.
/// </summary>
bool BeaconManagerClass::Is_Flashing(void)
{
	if (AllocatedCount) {
		bool found = false;
		for (int house = 0; house < 8; house++) {
			int slot = 0;
			do {
				BeaconClass * beacon = Beacons[house][slot];
				if (beacon && (beacon->IsActive) && PlayerPtr->Is_Ally((HousesType)beacon->HouseID) && Houses[beacon->HouseID]->Is_Ally(PlayerPtr) && !Houses[beacon->HouseID]->IsDefeated) {
					found = true;
					break;
				}
				slot++;
			} while (slot < 3);
		}
		if (found && Frame % RadarBeaconAnimPeriod < BeaconManager.RadarBeaconFrameCount + 1) {
			return(true);
		}
	}
	return(false);
}
