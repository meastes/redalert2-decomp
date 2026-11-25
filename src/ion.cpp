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
	NukeFlashState = 0;
	StartFrame = -1;
	Duration = -1;
	NukeFlashStart = -1;
	NukeFlashDelay = -1;
	Deferment = 0;
	Owner = NULL;
	CloudsManifesting.Clear();
	CloudsPresent.Clear();
	BoltsPresent.Clear();
	Scen->DesiredAmbientLight = Scen->AmbientLight;
	Set_Lighting(-1, -1, -1, false);
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

	hr = stream->Write(&IsEnding, sizeof(IsEnding), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Write(&NukeFlashStart, sizeof(NukeFlashStart), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Write(&NukeFlashDelay, sizeof(NukeFlashDelay), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Write(&NukeFlashState, sizeof(NukeFlashState), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Write(&Coords, sizeof(Coords), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Write(&Owner, sizeof(Owner), NULL);
	if (FAILED(hr)) return(hr);

	int count = CloudsManifesting.Count();
	hr = stream->Write(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int i = 0; i < count; i++) {
		AnimClass const * anim = CloudsManifesting[i];
		hr = stream->Write(&anim, sizeof(anim), NULL);
		if (FAILED(hr)) return(hr);
	}

	count = CloudsPresent.Count();
	hr = stream->Write(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int j = 0; j < count; j++) {
		AnimClass const * anim = CloudsPresent[j];
		hr = stream->Write(&anim, sizeof(anim), NULL);
		if (FAILED(hr)) return(hr);
	}

	count = BoltsPresent.Count();
	hr = stream->Write(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int k = 0; k < count; k++) {
		AnimClass const * anim = BoltsPresent[k];
		hr = stream->Write(&anim, sizeof(anim), NULL);
		if (FAILED(hr)) return(hr);
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

	hr = stream->Read(&IsEnding, sizeof(IsEnding), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Read(&NukeFlashStart, sizeof(NukeFlashStart), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Read(&NukeFlashDelay, sizeof(NukeFlashDelay), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Read(&NukeFlashState, sizeof(NukeFlashState), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Read(&Coords, sizeof(Coords), NULL);
	if (FAILED(hr)) return(hr);

	hr = stream->Read(&Owner, sizeof(Owner), NULL);
	if (FAILED(hr)) return(hr);

	Swizzle_Pointer(&Owner);

	int count;
	hr = stream->Read(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int i = 0; i < count; i++) {
		AnimClass * anim;
		hr = stream->Read(&anim, sizeof(anim), NULL);
		if (FAILED(hr)) return(hr);
		CloudsManifesting.Add(anim);
	}

	for (i = 0; i < count; i++) {
		Swizzle_Pointer(&CloudsManifesting[i]);
	}

	hr = stream->Read(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int j = 0; j < count; j++) {
		AnimClass * anim;
		hr = stream->Read(&anim, sizeof(anim), NULL);
		if (FAILED(hr)) return(hr);
		CloudsPresent.Add(anim);
	}

	for (j = 0; j < count; j++) {
		Swizzle_Pointer(&CloudsPresent[j]);
	}

	hr = stream->Read(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int k = 0; k < count; k++) {
		AnimClass * anim;
		hr = stream->Read(&anim, sizeof(anim), NULL);
		if (FAILED(hr)) return(hr);
		BoltsPresent.Add(anim);
	}

	for (k = 0; k < count; k++) {
		Swizzle_Pointer(&BoltsPresent[k]);
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
	if (cell == CELL_NONE) {
		while (!Map.In_Radar(cell)) {
			cell = Cell(Random_Pick(0, Map.MapRect.Width), Random_Pick(0, Map.MapRect.Height));
		}
	}
	Coords = cell;
	Owner = owner;
	if (IsActive) {
		return;
	}
	if (deferment) {
		if (Deferment == 0 || Deferment >= deferment) {
			Deferment = deferment;
		}
		Duration = duration;
		return;
	}
	Submit_Radar_Event(RADAREVENT_SUPERWEAPON_ACTIVATED, cell);
	Duration = duration;
	StartFrame = Frame;
	IsActive = true;
	for (int index = 0; index < Houses.Count(); index++) {
		if (Owner == NULL || !Owner->Is_Ally(Houses[index])) {
			if (!Houses[index]->IsDefeated && !MuteSWLaunches) {
				Houses[index]->Begin_Radar_Blackout(duration);
			}
		}
	}
	if (PlayerPtr != NULL) {
		PlayerPtr->RecalcRadar = true;
	}
	if (NukeFlashState != 1) {
		Scen->DesiredAmbientLight = Scen->IonAmbientLight;
		Set_Lighting(Scen->IonRedTint * 1000 / 100,
			Scen->IonGreenTint * 1000 / 100,
			Scen->IonBlueTint * 1000 / 100, true);
	}
	if (Rule->LightningPrintText) {
		int color = 3;
		if (PlayerPtr != NULL) {
			color = PlayerPtr->Scheme;
		}
		VocClass::PlayGlobal(Rule->StormSound, 1.0f, 0x2000, NULL);
		Session.Messages.Add_Message(NULL, 0,
			StringTable::LoadString("TXT_LIGHTNING_STORM", NULL, "D:\\Ra2\\Ion.cpp", 465), color, TextPrintType(TPF_6PT_GRAD | TPF_FULLSHADOW | TPF_USE_GRAD_PAL), 150, false);
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
		IsEnding = true;
	}
} // RA2: Ion_Storm_End at 0x51FBB0 has no Scen, DesiredAmbientLight, AmbientLight and 12 more


void IonStormClass::Ion_Storm_Stop(void) // RA2: the storm is over (0x51FBD0)
{
	if (IsActive) {
		IsActive = false;
		Coords = CELL_NONE;
		Owner = NULL;
		if (NukeFlashState != 1) {
			Scen->DesiredAmbientLight = Scen->AmbientLight;
			Set_Lighting(-1, -1, -1, false);
			Map.Flag_To_Redraw(GS_REDRAW_TACTICAL);
		}
	}
}


bool IonStormClass::Is_Ion_Storm_Pending(void) // RA2: active or deferred (0x51FC30)
{
	return(IsActive || Deferment > 0);
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
	return(IonStormClass::NukeFlashState == 1);
}


bool NukeFlash::IsFadingOut(void) // RA2: nuke flash state 2 (0x51FC70)
{
	return(IonStormClass::NukeFlashState == 2);
}


bool HouseClass::Ion_Sensitives_Should_Be_Offline(void) // RA2: always false (0x51FC80)
{
	return(false);
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
	int half = ((ShapeSet const *)Rule->WeatherConBolts[0]->Get_Image_Data())->Get_Height() / 2;
	int height = TacticalMap->Pixel_To_Z_Lepton(half);
	Coord coord(cell, cellptr.Height * LEVEL_LEPTON_H + height + (cellptr.IsUnderBridge ? BRIDGE_LEPTON_HEIGHT : 0));
	if (coord == COORD_NONE) {
		return;
	}
	int pick = Scen->RandomNumber();
	AnimTypeClass const * type = Rule->WeatherConClouds[(unsigned)pick % Rule->WeatherConClouds.Count()];
	AnimClass * anim = new AnimClass(type, coord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
	CloudsManifesting.Add(anim);
	CloudsPresent.Add(anim);
} // RA2: Lightning_Bolt at 0x51FC90 has no Rule, IonStormWarhead, Height and 6 more


void IonStormClass::Lightning_Strike(Coord coord) // RA2: the bolt of a cloud lands (0x51FE50)
{
	CellClass & cellptr = Map[coord];
	Coord center = Coord(cellptr.Cell_Coord());
	int pick = Scen->RandomNumber();
	AnimTypeClass const * type = Rule->WeatherConBolts[(unsigned)pick % Rule->WeatherConBolts.Count()];
	AnimClass * bolt = new AnimClass(type, center, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
	BoltsPresent.Add(bolt);

	Cell cell = cellptr.Fetch_CellID();
	int level = cellptr.Height;
	Coord where(cell, (cellptr.IsUnderBridge ? BRIDGE_LEPTON_HEIGHT : 0) + level * LEVEL_LEPTON_H);
	if (where == COORD_NONE) {
		return;
	}
	if (Rule->LightningSounds.Count() > 0) {
		int sound = Scen->RandomNumber();
		VocClass::PlayAt(Rule->LightningSounds[(unsigned)sound % Rule->LightningSounds.Count()], where, NULL);
	}
	LandType land = cellptr.Land_Type();
	int damage = Rule->LightningDamage;
	AnimTypeClass const * anim = Combat_Anim(damage, Rule->LightningWarhead, land, where);
	new AnimClass(anim, where, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL | SHAPE_ZGRAD), Get_Explosion_Z(where));

	bool changed = false;
	ObjectClass * building = cellptr.Cell_Building();
	ObjectClass * object = cellptr.Cell_Techno();
	bool sheltered = object != NULL && object->Fetch_RTTI() == RTTI_INFANTRY;
	if (building == NULL && object == NULL) {
		switch (cellptr.Land_Type()) {
			case LAND_ROAD:
			case LAND_ROCK:
			case LAND_WALL:
			case LAND_WEEDS:
				changed = true;
				break;
		}
	}
	Combat_Lighting(where, damage, Rule->LightningWarhead, 0, 0); // RA2: 0x0047F600 takes the forced flag as an int and a fifth argument, the flags
	Explosion_Damage(where, damage, NULL, Rule->LightningWarhead, true);
	if (building != cellptr.Cell_Building() || object != cellptr.Cell_Techno() || cellptr.Height != level) {
		changed = true;
	}
	if (!sheltered && changed) {
		int count = Random_Pick(2, 4);
		for (int index = 0; index < count; index++) {
			new AnimClass(Rule->MetallicDebris[Random_Pick(0, Rule->MetallicDebris.Count() - 1)], where, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
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
		if (NukeFlashDelay != -1 && NukeFlashStart + NukeFlashDelay < Frame) {
			End_Nuke_Flash();
		}
	} else if (NukeFlashState == 2) {
		if (NukeFlashDelay != -1 && NukeFlashStart + NukeFlashDelay < Frame) {
			NukeFlashState = 0;
		}
	}

	if (BoltsPresent.Count() > 0) {
		for (int index = BoltsPresent.Count() - 1; index >= 0; index--) {
			AnimClass const * bolt = BoltsPresent[index];
			int stage = bolt->Fetch_Stage();
			if (stage >= ((ShapeSet const *)bolt->Class_Of()->Get_Image_Data())->Get_Count() / 2) {
				BoltsPresent.Delete_Index(index);
			}
		}
	}
	for (int index = CloudsManifesting.Count() - 1; index >= 0; index--) {
		AnimClass const * cloud = CloudsManifesting[index];
		int stage = cloud->Fetch_Stage();
		if (stage > ((ShapeSet const *)cloud->Class_Of()->Get_Image_Data())->Get_Count() / 2) {
			Lightning_Strike(cloud->Center_Coord());
			CloudsManifesting.Delete_Index(index);
		}
	}
	if (CloudsPresent.Count() > 0) {
		for (int index = CloudsPresent.Count() - 1; index >= 0; index--) {
			AnimClass const * cloud = CloudsPresent[index];
			int stage = cloud->Fetch_Stage();
			if (stage >= ((ShapeSet const *)cloud->Class_Of()->Get_Image_Data())->Get_Count() - 1) {
				CloudsPresent.Delete_Index(index);
			}
		}
	} else if (IsEnding) {
		Ion_Storm_Stop();
		IsEnding = false;
	}

	if (IsActive && !IsEnding) {
		if (Duration != -1 && StartFrame + Duration < Frame) {
			IsEnding = true;
			return;
		}
		if (Frame % Rule->LightningHitDelay == 0) {
			Lightning_Bolt(Coords);
		}
		if (Frame % Rule->LightningScatterDelay == 0) {
			int range = Rule->LightningCellSpread >> 1;
			int tries;
			Cell cell;
			for (tries = 3; tries > 0; tries--) {
				cell = Coords;
				cell.X += Random_Pick(-range, range);
				cell.Y += Random_Pick(-range, range);
				bool crowded = false;
				for (int index = 0; index < CloudsPresent.Count(); index++) {
					Cell other = Cell(CloudsPresent[index]->Center_Coord());
					int dx = cell.X - other.X;
					if (dx < 0) {
						dx = -dx;
					}
					int dy = cell.Y - other.Y;
					if (dy < 0) {
						dy = -dy;
					}
					if (dx + dy < Rule->LightningSeparation) {
						crowded = true;
						break;
					}
				}
				if (Map.In_Radar(cell) && !crowded) {
					break;
				}
			}
			if (tries > 0) {
				Lightning_Bolt(cell);
			}
		}
	} else if (Deferment > 0) {
		Deferment--;
		if (Deferment == 0) {
			Ion_Storm_Begin(Duration, 0, Coords, Owner);
		} else if (Deferment % (TICKS_PER_SECOND * 15) == 0 && Rule->LightningPrintText) {
			int color = 3;
			if (PlayerPtr != NULL) {
				color = PlayerPtr->Scheme;
			}
			Queue_Speech("EVA_LightningStormCreated", -1, -1);
			Session.Messages.Add_Message(NULL, 0,
				StringTable::LoadString("TXT_LIGHTNING_STORM_APPROACHING", NULL, "D:\\Ra2\\Ion.cpp", 904), color, TextPrintType(TPF_6PT_GRAD | TPF_FULLSHADOW), 150,
				false);
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
	if (NukeFlashState == 1) {
		Scen->DesiredAmbientLight = Scen->NukeAmbient;
		Set_Lighting(Scen->NukeRedTint * 1000 / 100,
			Scen->NukeGreenTint * 1000 / 100,
			Scen->NukeBlueTint * 1000 / 100, true);
	} else if (IsActive) {
		PlayerPtr->RecalcRadar = true;
		Scen->DesiredAmbientLight = Scen->IonAmbientLight;
		Set_Lighting(Scen->IonRedTint * 1000 / 100,
			Scen->IonGreenTint * 1000 / 100,
			Scen->IonBlueTint * 1000 / 100, true);
	}
}


void IonStormClass::Start_Nuke_Flash(void) // RA2: nuke lighting for 30 frames (0x5208A0)
{
	NukeFlashState = 1;
	NukeFlashDelay = 30;
	NukeFlashStart = Frame;
	Scen->AmbientChangeTimer = 1;
	Scen->DesiredAmbientLight = Scen->NukeAmbient;
	Set_Lighting(Scen->NukeRedTint * 1000 / 100,
		Scen->NukeGreenTint * 1000 / 100,
		Scen->NukeBlueTint * 1000 / 100, true);
	Map.Flag_To_Redraw(GS_REDRAW_TACTICAL);
}


void IonStormClass::End_Nuke_Flash(void) // RA2: back to the storm light or the scenario light for 15 frames (0x520980)
{
	NukeFlashState = 2;
	NukeFlashStart = Frame;
	NukeFlashDelay = 15;
	if (IsActive) {
		Scen->DesiredAmbientLight = Scen->IonAmbientLight;
		Set_Lighting(Scen->IonRedTint * 1000 / 100,
			Scen->IonGreenTint * 1000 / 100,
			Scen->IonBlueTint * 1000 / 100, true);
	} else {
		Scen->DesiredAmbientLight = Scen->AmbientLight;
		Set_Lighting(-1, -1, -1, false);
	}
	Map.Flag_To_Redraw(GS_REDRAW_TACTICAL);
}


void IonStormClass::Set_Scheme_Lighting(int red, int green, int blue, bool ion_light) // RA2: tints the converters of the color scheme lists (0x520A80)
{
	SchemeListIterator iterator;
	for (DynamicVectorClass<ColorScheme *> * list = iterator.Next(); list != NULL; list = iterator.Next()) {
		int count = Scheme_List_Count();
		for (int index = 0; index < count; index++) {
			((LightConvertClass *)(*list)[index]->Converter)->Apply_Tint(red, green, blue, ion_light);
		}
	}
}


void IonStormClass::Set_Lighting(int red, int green, int blue, bool ion_light) // RA2: tints every tile drawer and color scheme (0x520B00)
{
	int index;
	for (index = 0; index < TileDrawers.Count(); index++) {
		TileDrawers[index]->Apply_Tint(red, green, blue, ion_light);
	}
	for (index = 0; index < ColorSchemes.Count(); index++) {
		if (ColorSchemes[index]->IntensityLevels > 1) {
			((LightConvertClass *)ColorSchemes[index]->Converter)->Apply_Tint(red, green, blue, ion_light);
		}
	}
	Set_Scheme_Lighting(red, green, blue, ion_light);
	Map.Update_Cell_Colors();
}


void IonStormClass::Ion_Storm_Active_Message(void) // RA2: tells the local player that a storm is already running (0x520BF0)
{
	wchar_t const * text = StringTable::LoadString("Msg:LightningStormActive", NULL, "D:\\Ra2\\Ion.cpp", 1066);
	int color = 3;
	if (PlayerPtr != NULL) {
		color = PlayerPtr->Scheme;
	}
	Session.Messages.Add_Message(NULL, 0, text, color, TextPrintType(TPF_6PT_GRAD | TPF_FULLSHADOW | TPF_USE_GRAD_PAL), 150, false);
}
