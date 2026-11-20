/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table

#include "ion.h"

#include "_convert.h"
#include "_map.h"
#include "_mixfile.h"
#include "_rect.h"
#include "_rules.h"
#include "_surface.h"
#include "aircraft.h"
#include "anim.h"
#include "building.h"
#include "ccrand.h"
#include "cell.h"
#include "combat.h"
#include "convert.h"
#include "data.h"
#include "draw.h"
#include "foot.h"
#include "globals.h"
#include "gscreen.h"
#include "house.h"
#include "language\language.h"
#include "laser.h"
#include "lightcon.h"
#include "mixfile.h"
#include "mouse.h"
#include "palette.h"
#include "rules.h"
#include "scenario.h"
#include "scheme.h"
#include "session.h"
#include "sun.h"
#include "theme.h"
#include "shapeset.h" // RA2: the image data
#include "tactical.h" // RA2: Pixel_To_Z_Lepton
#include "_tactica.h" // RA2: TacticalMap
#include "vector.h" // RA2: the lists
#include "revent.h" // RA2: the radar event
#include "voc.h" // RA2: the sounds
#include "stringtable.h" // RA2: the text comes from the string table
#include "swizzle.h" // RA2: Swizzle_Pointer
#include "vox.h"

#include "color.hh"
#include "ftimer.h"



void Combat_Lighting(Coord coord, int damage, WarheadTypeClass const * warhead, int forced, int flags); // RA2: 0x0047F600; combat.h declares the call with one argument less (bool forced)


/*
**	RA2: the color scheme lists give the storm tints (palette.h walks them; 0x00603C10 counts the schemes of a list).
*/
int Scheme_List_Count(void); // RA2: 0x00603C10


bool IonStormClass::IsActive = false; // RA2: 0x00A51934
int IonStormClass::Deferment = 0; // RA2: 0x00A51938
int IonStormClass::NukeFlashState = 0; // RA2: 0x00A5193C
HouseClass * IonStormClass::Owner = NULL; // RA2: 0x00A51940
bool IonStormClass::IsEnding = false; // RA2: 0x00A51944
int IonStormClass::StartFrame = -1; // RA2: 0x007DDB10
int IonStormClass::Duration = -1; // RA2: 0x007DDB14
int IonStormClass::NukeFlashStart = -1; // RA2: 0x007DDB18
int IonStormClass::NukeFlashDelay = -1; // RA2: 0x007DDB1C
Cell IonStormClass::Coords = CELL_NONE; // RA2: 0x00A51878; its initializer 0x51F100 copies the unit's CELL_NONE (ra2-decomp ion.cpp: LightningStorm::Coords = DAT_00A9F9F8)
DynamicVectorClass<AnimClass const *> IonStormClass::CloudsManifesting; // RA2: 0x00A51908
DynamicVectorClass<AnimClass const *> IonStormClass::CloudsPresent; // RA2: 0x00A51880
DynamicVectorClass<AnimClass const *> IonStormClass::BoltsPresent; // RA2: 0x00A518C8


/// <summary>
/// Clears the ion storm state back to its dormant condition.
/// This routine is called as a scenario is being set up, so that no storm or pending
/// storm is carried over from the previous mission.
/// </summary>
void IonStormClass::Init(void)
{
	Coords = CELL_NONE; // RA2: Init at 0x51F2F0 matches only so
	IsActive = false;
	NukeFlashState = 0; // RA2: as above
	StartFrame = -1;
	Duration = -1;
	NukeFlashStart = -1; // RA2: as above
	NukeFlashDelay = -1; // RA2: as above
	Deferment = 0;
	Owner = NULL; // RA2: as above
	CloudsManifesting.Clear(); // RA2: as above
	CloudsPresent.Clear(); // RA2: as above
	BoltsPresent.Clear(); // RA2: as above
	Scen->DesiredAmbientLight = Scen->AmbientLight; // RA2: as above
	Set_Lighting(-1, -1, -1, false); // RA2: as above
}


/// <summary>
/// Saves the ion storm state to the save game stream.
/// </summary>
/// <returns>Returns with the result of the last stream operation attempted.</returns>
HRESULT IonStormClass::Save(IStream * stream)
{
	HRESULT hr = stream->Write(&IsActive, sizeof(IsActive), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Write(&StartFrame, sizeof(StartFrame), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Write(&Duration, sizeof(Duration), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Write(&Deferment, sizeof(Deferment), NULL);
	if (FAILED(hr)) return(hr); // RA2: Save at 0x51F3F0 matches only so

	hr = stream->Write(&IsEnding, sizeof(IsEnding), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Write(&NukeFlashStart, sizeof(NukeFlashStart), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Write(&NukeFlashDelay, sizeof(NukeFlashDelay), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Write(&NukeFlashState, sizeof(NukeFlashState), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Write(&Coords, sizeof(Coords), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Write(&Owner, sizeof(Owner), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	int count = CloudsManifesting.Count(); // RA2: as above
	hr = stream->Write(&count, sizeof(count), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	for (int i = 0; i < count; i++) { // RA2: as above
		AnimClass const * anim = CloudsManifesting[i]; // RA2: as above
		hr = stream->Write(&anim, sizeof(anim), NULL); // RA2: as above
		if (FAILED(hr)) return(hr); // RA2: as above
	}

	count = CloudsPresent.Count(); // RA2: as above
	hr = stream->Write(&count, sizeof(count), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	for (int j = 0; j < count; j++) { // RA2: as above
		AnimClass const * anim = CloudsPresent[j]; // RA2: as above
		hr = stream->Write(&anim, sizeof(anim), NULL); // RA2: as above
		if (FAILED(hr)) return(hr); // RA2: as above
	}

	count = BoltsPresent.Count(); // RA2: as above
	hr = stream->Write(&count, sizeof(count), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	for (int k = 0; k < count; k++) { // RA2: as above
		AnimClass const * anim = BoltsPresent[k]; // RA2: as above
		hr = stream->Write(&anim, sizeof(anim), NULL); // RA2: as above
		if (FAILED(hr)) return(hr); // RA2: as above
	}

	return(hr);
}


/// <summary>
/// Loads the ion storm state from the save game stream.
/// </summary>
/// <returns>Returns with the result of the last stream operation attempted.</returns>
/// <remarks>Only the bookkeeping is restored here. Post_Load_Game must still call
/// Apply_Secondary_Effect to put the world back into its storm bound state.</remarks>
HRESULT IonStormClass::Load(IStream * stream)
{
	HRESULT hr = stream->Read(&IsActive, sizeof(IsActive), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Read(&StartFrame, sizeof(StartFrame), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Read(&Duration, sizeof(Duration), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Read(&Deferment, sizeof(Deferment), NULL);
	if (FAILED(hr)) return(hr); // RA2: Load at 0x51F5E0 matches only so

	hr = stream->Read(&IsEnding, sizeof(IsEnding), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Read(&NukeFlashStart, sizeof(NukeFlashStart), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Read(&NukeFlashDelay, sizeof(NukeFlashDelay), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Read(&NukeFlashState, sizeof(NukeFlashState), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Read(&Coords, sizeof(Coords), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	hr = stream->Read(&Owner, sizeof(Owner), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	Swizzle_Pointer(&Owner); // RA2: as above

	int count; // RA2: as above
	hr = stream->Read(&count, sizeof(count), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	for (int i = 0; i < count; i++) { // RA2: as above
		AnimClass * anim; // RA2: as above
		hr = stream->Read(&anim, sizeof(anim), NULL); // RA2: as above
		if (FAILED(hr)) return(hr); // RA2: as above
		CloudsManifesting.Add(anim); // RA2: as above
	}

	for (i = 0; i < count; i++) { // RA2: as above
		Swizzle_Pointer(&CloudsManifesting[i]); // RA2: as above
	}

	hr = stream->Read(&count, sizeof(count), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	for (int j = 0; j < count; j++) { // RA2: as above
		AnimClass * anim; // RA2: as above
		hr = stream->Read(&anim, sizeof(anim), NULL); // RA2: as above
		if (FAILED(hr)) return(hr); // RA2: as above
		CloudsPresent.Add(anim); // RA2: as above
	}

	for (j = 0; j < count; j++) { // RA2: as above
		Swizzle_Pointer(&CloudsPresent[j]); // RA2: as above
	}

	hr = stream->Read(&count, sizeof(count), NULL); // RA2: as above
	if (FAILED(hr)) return(hr); // RA2: as above

	for (int k = 0; k < count; k++) { // RA2: as above
		AnimClass * anim; // RA2: as above
		hr = stream->Read(&anim, sizeof(anim), NULL); // RA2: as above
		if (FAILED(hr)) return(hr); // RA2: as above
		BoltsPresent.Add(anim); // RA2: as above
	}

	for (k = 0; k < count; k++) { // RA2: as above
		Swizzle_Pointer(&BoltsPresent[k]); // RA2: as above
	}

	return(hr);
}


/// <summary>
/// Records whether an ion storm is in progress.
/// This routine is used by the storm start and end handlers. The rest of the game asks
/// Is_Ion_Storm_Active rather than reaching for the flag itself.
/// </summary>
/// <remarks>This merely records the state. Use Ion_Storm_Begin and Ion_Storm_End to
/// actually raise or lift a storm.</remarks>
void IonStormClass::Set_Ion_Storm_Active(bool active)
{
	IsActive = active;
}


/// <summary>
/// Starts an ion storm, or schedules one to arrive later.
/// When a warning period is asked for, this routine merely arms the countdown and
/// returns; the storm itself begins when the AI process works the countdown down to
/// zero. Otherwise the storm breaks immediately -- ion sensitive locomotors lose power
/// (aircraft rather dramatically), the world takes on the ion tint, and the storm theme
/// replaces whatever was playing. A storm already in progress is left undisturbed.
/// </summary>
/// <param name="duration">The number of game frames the storm should last.</param>
/// <param name="warning">The number of game frames to wait before the storm breaks. Use
/// zero to start it right now.</param>
void IonStormClass::Ion_Storm_Begin(int duration, int deferment, Cell cell, HouseClass * owner) // RA2: a storm aimed at a cell for a house, with a deferment; no tint loops (0x51F930)
{
	if (cell == CELL_NONE) { // RA2: as above
		while (!Map.In_Radar(cell)) { // RA2: as above
			cell = Cell(Random_Pick(0, Map.MapRect.Width), Random_Pick(0, Map.MapRect.Height)); // RA2: as above
		}
	}
	Coords = cell; // RA2: as above
	Owner = owner; // RA2: as above
	if (IsActive) { // RA2: as above
		return; // RA2: as above
	}
	if (deferment) { // RA2: as above
		if (Deferment == 0 || Deferment >= deferment) { // RA2: as above
			Deferment = deferment; // RA2: as above
		}
		Duration = duration;
		return; // RA2: as above
	}
	Submit_Radar_Event(RADAREVENT_SUPERWEAPON_ACTIVATED, cell); // RA2: as above
	Duration = duration;
	StartFrame = Frame;
	IsActive = true; // RA2: as above
	for (int index = 0; index < Houses.Count(); index++) { // RA2: as above
		if (Owner == NULL || !Owner->Is_Ally(Houses[index])) { // RA2: as above
			if (!Houses[index]->IsDefeated && !MuteSWLaunches) { // RA2: as above
				Houses[index]->Begin_Radar_Blackout(duration); // RA2: as above
			}
		}
	}
	if (PlayerPtr != NULL) { // RA2: as above
		PlayerPtr->RecalcRadar = true;
	}
	if (NukeFlashState != 1) { // RA2: as above
		Scen->DesiredAmbientLight = Scen->IonAmbientLight;
		Set_Lighting(Scen->IonRedTint * 1000 / 100, // RA2: as above
			Scen->IonGreenTint * 1000 / 100, // RA2: as above
			Scen->IonBlueTint * 1000 / 100, true); // RA2: as above
	}
	if (Rule->LightningPrintText) { // RA2: as above
		int color = 3; // RA2: as above
		if (PlayerPtr != NULL) { // RA2: as above
			color = PlayerPtr->Scheme; // RA2: as above
		}
		VocClass::PlayGlobal(Rule->StormSound, 1.0f, 0x2000, NULL); // RA2: as above
		Session.Messages.Add_Message(NULL, 0, // RA2: as above
			StringTable::LoadString("TXT_LIGHTNING_STORM", NULL, "D:\\Ra2\\Ion.cpp", 465), color, TextPrintType(0x4046), 150, false); // RA2: as above
	}
	Map.Flag_To_Redraw(GS_REDRAW_TACTICAL);
}


/// <summary>
/// Brings the ion storm to an end.
/// This routine restores everything the storm took away -- power comes back to the ion
/// sensitive locomotors, the ambient light and the color tinting return to normal, and
/// the music that was playing when the storm rolled in is resumed. The screen static is
/// shown while the palettes are rebuilt, since that is a lengthy business.
/// </summary>
void IonStormClass::Ion_Storm_End(void)
{
	if (IsActive) { // RA2: Ion_Storm_End at 0x51FBB0 matches only so
		IsEnding = true; // RA2: as above
	}
} // RA2: Ion_Storm_End at 0x51FBB0 has no Scen, DesiredAmbientLight, AmbientLight and 12 more


void IonStormClass::Ion_Storm_Stop(void) // RA2: the storm is over (0x51FBD0)
{
	if (IsActive) { // RA2: as above
		IsActive = false; // RA2: as above
		Coords = CELL_NONE; // RA2: as above
		Owner = NULL; // RA2: as above
		if (NukeFlashState != 1) { // RA2: as above
			Scen->DesiredAmbientLight = Scen->AmbientLight; // RA2: as above
			Set_Lighting(-1, -1, -1, false); // RA2: as above
			Map.Flag_To_Redraw(GS_REDRAW_TACTICAL); // RA2: as above
		}
	}
}


int IonStormClass::Is_Ion_Storm_Pending(void) // RA2: active or deferred, returns int (0x51FC30)
{
	if (IsActive || Deferment > 0) { // RA2: as above
		return(1); // RA2: as above
	}
	return(0); // RA2: as above
}


/// <summary>
/// Determines if an ion storm is currently raging.
/// </summary>
/// <returns>bool; Is an ion storm in progress?</returns>
bool IonStormClass::Is_Ion_Storm_Active(void)
{
	return(IsActive);
}


bool NukeFlash::IsFadingIn(void) // RA2: nuke flash state 1 (0x51FC60)
{
	return(IonStormClass::NukeFlashState == 1); // RA2: as above
}


bool NukeFlash::IsFadingOut(void) // RA2: nuke flash state 2 (0x51FC70)
{
	return(IonStormClass::NukeFlashState == 2); // RA2: as above
}


bool HouseClass::Ion_Sensitives_Should_Be_Offline(void) // RA2: always false (0x51FC80)
{
	return(false); // RA2: as above
}


/// <summary>
/// Strikes the specified cell with a bolt of lightning.
/// This routine handles the whole spectacle -- the thunderclap, the explosion animation,
/// the combat lighting flash, and the damage inflicted on whatever was unlucky enough to
/// be standing there. Debris is thrown up if something was destroyed or the terrain was
/// altered, and a jagged laser trail is drawn from the strike point up into the sky.
/// </summary>
void IonStormClass::Lightning_Bolt(Cell cell)
{
	CellClass & cellptr = Map[cell]; // RA2: Lightning_Bolt at 0x51FC90 matches only so
	int half = ((ShapeSet const *)Rule->WeatherConBolts[0]->Get_Image_Data())->Get_Height() / 2; // RA2: as above
	int height = TacticalMap->Pixel_To_Z_Lepton(half); // RA2: as above
	Coord coord(cell, cellptr.Height * LEVEL_LEPTON_H + height + (cellptr.IsUnderBridge ? BRIDGE_LEPTON_HEIGHT : 0)); // RA2: as above
	if (coord == COORD_NONE) { // RA2: as above
		return; // RA2: as above
	}
	int pick = Scen->RandomNumber(); // RA2: as above
	AnimTypeClass const * type = Rule->WeatherConClouds[(unsigned)pick % Rule->WeatherConClouds.Count()]; // RA2: as above
	AnimClass * anim = new AnimClass(type, coord, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
	CloudsManifesting.Add(anim); // RA2: as above
	CloudsPresent.Add(anim); // RA2: as above
} // RA2: Lightning_Bolt at 0x51FC90 has no Rule, IonStormWarhead, Height and 6 more


void IonStormClass::Lightning_Strike(Coord coord) // RA2: the bolt of a cloud lands (0x51FE50)
{
	CellClass & cellptr = Map[coord]; // RA2: as above
	Coord center = Coord(cellptr.Cell_Coord()); // RA2: as above
	int pick = Scen->RandomNumber(); // RA2: as above
	AnimTypeClass const * type = Rule->WeatherConBolts[(unsigned)pick % Rule->WeatherConBolts.Count()]; // RA2: as above
	AnimClass * bolt = new AnimClass(type, center, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
	BoltsPresent.Add(bolt); // RA2: as above

	Cell cell = cellptr.Fetch_CellID(); // RA2: as above
	int level = cellptr.Height; // RA2: as above
	Coord where(cell, (cellptr.IsUnderBridge ? BRIDGE_LEPTON_HEIGHT : 0) + level * LEVEL_LEPTON_H); // RA2: as above
	if (where == COORD_NONE) { // RA2: as above
		return; // RA2: as above
	}
	if (Rule->LightningSounds.Count() > 0) { // RA2: as above
		int sound = Scen->RandomNumber(); // RA2: as above
		VocClass::PlayAt(Rule->LightningSounds[(unsigned)sound % Rule->LightningSounds.Count()], where, NULL); // RA2: as above
	}
	LandType land = cellptr.Land_Type(); // RA2: as above
	int damage = Rule->LightningDamage; // RA2: as above
	AnimTypeClass const * anim = Combat_Anim(damage, Rule->LightningWarhead, land, where); // RA2: as above
	new AnimClass(anim, where, 0, 1, ShapeFlags_Type(0x2600), Get_Explosion_Z(where)); // RA2: as above

	bool changed = false; // RA2: as above
	ObjectClass * building = cellptr.Cell_Building(); // RA2: as above
	ObjectClass * object = cellptr.Cell_Techno(); // RA2: as above
	bool sheltered = object != NULL && object->Fetch_RTTI() == RTTI_INFANTRY; // RA2: as above
	if (building == NULL && object == NULL) { // RA2: as above
		switch (cellptr.Land_Type()) { // RA2: as above
			case LAND_ROAD: // RA2: as above
			case LAND_ROCK: // RA2: as above
			case LAND_WALL: // RA2: as above
			case LAND_WEEDS: // RA2: as above
				changed = true; // RA2: as above
				break; // RA2: as above
		}
	}
	Combat_Lighting(where, damage, Rule->LightningWarhead, 0, 0); // RA2: 0x0047F600 takes the forced flag as an int and a fifth argument, the flags
	Explosion_Damage(where, damage, NULL, Rule->LightningWarhead, true); // RA2: as above
	if (building != cellptr.Cell_Building() || object != cellptr.Cell_Techno() || cellptr.Height != level) { // RA2: as above
		changed = true; // RA2: as above
	}
	if (!sheltered && changed) { // RA2: as above
		int count = Random_Pick(2, 4); // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: as above
			new AnimClass(Rule->MetallicDebris[Random_Pick(0, Rule->MetallicDebris.Count() - 1)], where, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
		}
	}
}


/// <summary>
/// Handles the per frame logic of the ion storm.
/// While a storm rages this routine expires it when its duration runs out and
/// occasionally calls down a lightning bolt, favoring objects that carry a lightning
/// rod. While no storm rages it counts down any pending deferment, warning the player
/// as the storm approaches and starting it when the wait is over.
/// </summary>
void IonStormClass::AI(void)
{
	if (NukeFlashState == 1) { // RA2: AI at 0x520200 matches only so
		if (NukeFlashDelay != -1 && NukeFlashStart + NukeFlashDelay < Frame) { // RA2: as above
			End_Nuke_Flash(); // RA2: as above
		}
	} else if (NukeFlashState == 2) { // RA2: as above
		if (NukeFlashDelay != -1 && NukeFlashStart + NukeFlashDelay < Frame) { // RA2: as above
			NukeFlashState = 0; // RA2: as above
		}
	}

	if (BoltsPresent.Count() > 0) { // RA2: as above
		for (int index = BoltsPresent.Count() - 1; index >= 0; index--) { // RA2: as above
			AnimClass const * bolt = BoltsPresent[index]; // RA2: as above
			int stage = bolt->Fetch_Stage(); // RA2: as above
			if (stage >= ((ShapeSet const *)bolt->Class_Of()->Get_Image_Data())->Get_Count() / 2) { // RA2: as above
				BoltsPresent.Delete_Index(index); // RA2: as above
			}
		}
	}
	for (int index = CloudsManifesting.Count() - 1; index >= 0; index--) { // RA2: as above
		AnimClass const * cloud = CloudsManifesting[index]; // RA2: as above
		int stage = cloud->Fetch_Stage(); // RA2: as above
		if (stage > ((ShapeSet const *)cloud->Class_Of()->Get_Image_Data())->Get_Count() / 2) { // RA2: as above
			Lightning_Strike(cloud->Center_Coord()); // RA2: as above
			CloudsManifesting.Delete_Index(index); // RA2: as above
		}
	}
	if (CloudsPresent.Count() > 0) { // RA2: as above
		for (int index = CloudsPresent.Count() - 1; index >= 0; index--) { // RA2: as above
			AnimClass const * cloud = CloudsPresent[index]; // RA2: as above
			int stage = cloud->Fetch_Stage(); // RA2: as above
			if (stage >= ((ShapeSet const *)cloud->Class_Of()->Get_Image_Data())->Get_Count() - 1) { // RA2: as above
				CloudsPresent.Delete_Index(index); // RA2: as above
			}
		}
	} else if (IsEnding) { // RA2: as above
		Ion_Storm_Stop(); // RA2: as above
		IsEnding = false; // RA2: as above
	}

	if (IsActive && !IsEnding) { // RA2: as above
		if (Duration != -1 && StartFrame + Duration < Frame) {
			IsEnding = true; // RA2: as above
			return;
		}
		if (Frame % Rule->LightningHitDelay == 0) { // RA2: as above
			Lightning_Bolt(Coords); // RA2: as above
		}
		if (Frame % Rule->LightningScatterDelay == 0) { // RA2: as above
			int range = Rule->LightningCellSpread >> 1; // RA2: as above
			int tries; // RA2: as above
			Cell cell; // RA2: as above
			for (tries = 3; tries > 0; tries--) { // RA2: as above
				cell = Coords; // RA2: as above
				cell.X += Random_Pick(-range, range); // RA2: as above
				cell.Y += Random_Pick(-range, range); // RA2: as above
				bool crowded = false; // RA2: as above
				for (int index = 0; index < CloudsPresent.Count(); index++) { // RA2: as above
					Cell other = Cell(CloudsPresent[index]->Center_Coord()); // RA2: as above
					int dx = cell.X - other.X; // RA2: as above
					if (dx < 0) { // RA2: as above
						dx = -dx; // RA2: as above
					}
					int dy = cell.Y - other.Y; // RA2: as above
					if (dy < 0) { // RA2: as above
						dy = -dy; // RA2: as above
					}
					if (dx + dy < Rule->LightningSeparation) { // RA2: as above
						crowded = true; // RA2: as above
						break;
					}
				}
				if (Map.In_Radar(cell) && !crowded) { // RA2: as above
					break;
				}
			}
			if (tries > 0) { // RA2: as above
				Lightning_Bolt(cell);
			}
		}
	} else if (Deferment > 0) { // RA2: as above
		Deferment--; // RA2: as above
		if (Deferment == 0) { // RA2: as above
			Ion_Storm_Begin(Duration, 0, Coords, Owner); // RA2: as above
		} else if (Deferment % (TICKS_PER_SECOND * 15) == 0 && Rule->LightningPrintText) { // RA2: as above
			int color = 3; // RA2: as above
			if (PlayerPtr != NULL) { // RA2: as above
				color = PlayerPtr->Scheme; // RA2: as above
			}
			Queue_Speech("EVA_LightningStormCreated", -1, -1); // RA2: as above
			Session.Messages.Add_Message(NULL, 0, // RA2: as above
				StringTable::LoadString("TXT_LIGHTNING_STORM_APPROACHING", NULL, "D:\\Ra2\\Ion.cpp", 904), color, TextPrintType(0x46), 150, // RA2: as above
				false); // RA2: as above
		}
	}
}


/// <summary>
/// Re-applies the visible effects of an ion storm.
/// This routine is used to put the world back into the state a raging storm leaves it
/// in -- ion sensitive locomotors shut down, the ambient light dimmed, and every tile
/// drawer and color scheme tinted. The save game loader calls this after restoring a
/// scenario that was saved while a storm was in progress.
/// </summary>
/// <param name="do_static">Should the screen static be shown while the tinting proceeds?</param>
void IonStormClass::Apply_Secondary_Effect(void) // RA2: puts the lighting of a nuke flash or a running storm back (0x520770)
{
	if (NukeFlashState == 1) { // RA2: as above
		Scen->DesiredAmbientLight = Scen->NukeAmbient; // RA2: as above
		Set_Lighting(Scen->NukeRedTint * 1000 / 100, // RA2: as above
			Scen->NukeGreenTint * 1000 / 100, // RA2: as above
			Scen->NukeBlueTint * 1000 / 100, true); // RA2: as above
	} else if (IsActive) { // RA2: as above
		PlayerPtr->RecalcRadar = true;
		Scen->DesiredAmbientLight = Scen->IonAmbientLight;
		Set_Lighting(Scen->IonRedTint * 1000 / 100, // RA2: as above
			Scen->IonGreenTint * 1000 / 100, // RA2: as above
			Scen->IonBlueTint * 1000 / 100, true); // RA2: as above
	}
}


void IonStormClass::Start_Nuke_Flash(void) // RA2: nuke lighting for 30 frames (0x5208A0)
{
	NukeFlashState = 1; // RA2: as above
	NukeFlashDelay = 30; // RA2: as above
	NukeFlashStart = Frame; // RA2: as above
	Scen->AmbientChangeTimer = 1; // RA2: as above
	Scen->DesiredAmbientLight = Scen->NukeAmbient; // RA2: as above
	Set_Lighting(Scen->NukeRedTint * 1000 / 100, // RA2: as above
		Scen->NukeGreenTint * 1000 / 100, // RA2: as above
		Scen->NukeBlueTint * 1000 / 100, true); // RA2: as above
	Map.Flag_To_Redraw(GS_REDRAW_TACTICAL); // RA2: as above
}


void IonStormClass::End_Nuke_Flash(void) // RA2: back to the storm light or the scenario light for 15 frames (0x520980)
{
	NukeFlashState = 2; // RA2: as above
	NukeFlashStart = Frame; // RA2: as above
	NukeFlashDelay = 15; // RA2: as above
	if (IsActive) { // RA2: as above
		Scen->DesiredAmbientLight = Scen->IonAmbientLight; // RA2: as above
		Set_Lighting(Scen->IonRedTint * 1000 / 100, // RA2: as above
			Scen->IonGreenTint * 1000 / 100, // RA2: as above
			Scen->IonBlueTint * 1000 / 100, true); // RA2: as above
	} else {
		Scen->DesiredAmbientLight = Scen->AmbientLight; // RA2: as above
		Set_Lighting(-1, -1, -1, false); // RA2: as above
	}
	Map.Flag_To_Redraw(GS_REDRAW_TACTICAL); // RA2: as above
}


void IonStormClass::Set_Scheme_Lighting(int red, int green, int blue, bool unknown4) // RA2: tints the converters of the color scheme lists (0x520A80)
{
	SchemeListIterator iterator; // RA2: as above
	for (DynamicVectorClass<ColorScheme *> * list = iterator.Next(); list != NULL; list = iterator.Next()) { // RA2: as above
		int count = Scheme_List_Count(); // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: as above
			((LightConvertClass *)(*list)[index]->Converter)->Apply_Tint(red, green, blue, unknown4); // RA2: as above
		}
	}
}


void IonStormClass::Set_Lighting(int red, int green, int blue, bool unknown4) // RA2: tints every tile drawer and color scheme (0x520B00)
{
	int index; // RA2: as above
	for (index = 0; index < TileDrawers.Count(); index++) { // RA2: as above
		TileDrawers[index]->Apply_Tint(red, green, blue, unknown4); // RA2: as above
	}
	for (index = 0; index < ColorSchemes.Count(); index++) { // RA2: as above
		if (ColorSchemes[index]->IntensityLevels > 1) { // RA2: as above
			((LightConvertClass *)ColorSchemes[index]->Converter)->Apply_Tint(red, green, blue, unknown4); // RA2: as above
		}
	}
	Set_Scheme_Lighting(red, green, blue, unknown4); // RA2: as above
	Map.Update_Cell_Colors(); // RA2: as above
}


void IonStormClass::Ion_Storm_Active_Message(void) // RA2: tells the local player that a storm is already running (0x520BF0)
{
	wchar_t const * text = StringTable::LoadString("Msg:LightningStormActive", NULL, "D:\\Ra2\\Ion.cpp", 1066); // RA2: as above
	int color = 3; // RA2: as above
	if (PlayerPtr != NULL) { // RA2: as above
		color = PlayerPtr->Scheme; // RA2: as above
	}
	Session.Messages.Add_Message(NULL, 0, text, color, TextPrintType(0x4046), 150, false); // RA2: as above
}
