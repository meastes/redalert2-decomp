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
**	RA2: the parasite of the terror drone and the squid: the owner leaves the map while it sits in its
**	victim, a vehicle (drone) or a ship (squid), and returns when the victim dies or is let go. TibSun has
**	no such file; the code is the executable's from 0x006061A0.
*/

#define INCLUDE_COM
#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "parasite.h" // RA2: the includes this file needs
#include "anim.h" // RA2: the includes this file needs
#include "animtype.h" // RA2: the includes this file needs
#include "crc.h" // RA2: the includes this file needs
#include "foot.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "swizzle.h" // RA2: the includes this file needs
#include "tracker.h" // RA2: the includes this file needs
#include "_map.h" // RA2: the includes this file needs
#include "cell.h" // RA2: the includes this file needs
#include "house.h" // RA2: the includes this file needs
#include "inline.h" // RA2: the includes this file needs
#include "weapon.h" // RA2: the includes this file needs
#include "partsys.h" // RA2: the includes this file needs
#include "_rules.h" // RA2: the includes this file needs
#include "rules.h" // RA2: the includes this file needs
#include "warhead.h" // RA2: the includes this file needs
#include "scenario.h" // RA2: the includes this file needs
#include "veteran.h" // RA2: the includes this file needs
#include <new.h> // RA2: the includes this file needs

static void __fastcall Make_Wakes(FootClass * victim, Coord coord); // RA2: 0x00606E10


DynamicVectorClass<ParasiteClass *> Parasites; // RA2: 0x00A76430; every parasite in the game (its initializer is the one at 0x00606120, its destructor the one at 0x00606160)

/// <summary>
/// Makes a parasite that belongs to no one yet, and lists it.
/// </summary>
ParasiteClass::ParasiteClass(void) // RA2: 0x006061A0
{
	Parasites.Add(this);
}


/// <summary>
/// Makes the parasite of the given foot soldier or vehicle and lists it. It sits in no victim yet.
/// </summary>
ParasiteClass::ParasiteClass(FootClass * owner) // RA2: 0x00606240
{
	Owner = owner;
	Victim = NULL;
	DamageDeliveryTimer = 0;
	SuppressionTimer = 0;
	GrappleAnimGotInvalid = false;
	GrappleAnimFrame = 0;
	GrappleAnimDelay = 0;
	GrappleAnim = NULL;
	GrappleState = 0;
	Parasites.Add(this);
}


/// <summary>
/// The constructor used when the parasite is read from a save game; its fields come from the stream.
/// </summary>
ParasiteClass::ParasiteClass(NoInitClass const & x) : // RA2: 0x00606320
	AbstractClass(x)
{
}


/// <summary>
/// Drops the grapple animation and takes the parasite out of the list of parasites.
/// </summary>
ParasiteClass::~ParasiteClass(void) // RA2: 0x00606370
{
	Clear_Grapple(); // RA2: the executable has the body of Clear_Grapple in line here
	Parasites.Delete(this);
}


/// <summary>
/// Adds what makes the parasite differ between machines to the checksum of the game state.
/// </summary>
void ParasiteClass::Compute_CRC(CRCEngine & crc) const // RA2: 0x00606460
{
	BASECLASS::Compute_CRC(crc);
	if (GrappleAnim != NULL) {
		crc(GrappleAnim->Fetch_ID());
	}
	crc(GrappleState);
	crc(GrappleAnimFrame);
	crc(GrappleAnimDelay);
	crc(GrappleAnimGotInvalid);
}


/// <summary>
/// Drops the grapple animation (squid) and its tracker entry, and resets the grapple counters.
/// </summary>
void ParasiteClass::Clear_Grapple(void) // RA2: 0x006064C0; named for what it does (YR: sub_629530)
{
	GrappleAnimFrame = 0;
	GrappleAnimDelay = 0;
	GrappleState = 0;
	if (GrappleAnim != NULL) {
		GrappleAnim->Delete_Me();
		GrappleAnim = NULL;
	}
	if (GrappleAnimGotInvalid) {
		AnimPtrTracker.Delete(this);
		GrappleAnimGotInvalid = false;
	}
}


/// <summary>
/// Reads the parasite back in from the save game stream. The owner, the victim and the grapple animation
/// are pointers that the swizzler remaps afterwards.
/// </summary>
HRESULT STDMETHODCALLTYPE ParasiteClass::Load(IStream * stream) // RA2: 0x00606540
{
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		new (this) ParasiteClass(NoInitClass());
		if (GrappleAnim != NULL) {
			AnimPtrTracker.Add(this);
		}
		Swizzle_Pointer(&Owner);
		Swizzle_Pointer(&Victim);
		Swizzle_Pointer(&GrappleAnim);
	}
	return(result);
}


/// <summary>
/// Writes the parasite to the save game stream; it has nothing of its own to add to the base object.
/// </summary>
HRESULT STDMETHODCALLTYPE ParasiteClass::Save(IStream * stream, BOOL cleardirty) // RA2: 0x00606640
{
	return(BASECLASS::Save(stream, cleardirty));
}


/// <summary>
/// Fetches the persistent class identifier for the parasite.
/// </summary>
HRESULT STDMETHODCALLTYPE ParasiteClass::GetClassID(CLSID * retval) // RA2: 0x00606660
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_ParasiteClass;
	return(S_OK);
}


/// <summary>
/// Starts the grapple animation (squid) over from its first frame.
/// </summary>
void ParasiteClass::Reset_Grapple_Counters(void) // RA2: 0x006066A0; named for what it does (YR: sub_629710)
{
	GrappleAnimDelay = 0;
	GrappleAnimFrame = 0;
}


/// <summary>
/// Steps the squid's grapple animation: each state has its own frame set (by the victim's facing) and delay.
/// </summary>
/// <returns>Returns true when the last frame of the state was shown.</returns>
bool ParasiteClass::UpdateGrapple(void) // RA2: 0x006066B0
{
	DirType dir = Victim->PrimaryFacing.Current();
	int base;
	int limit;
	switch (GrappleState) {
		case 0:
		case 1:
			base = 0;
			limit = 3;
			break;
		case 2:
			base = 80; // RA2: 0x50, the second set of ten frames per facing
			limit = 4;
			break;
		default:
			base = 160; // RA2: 0xA0, the third set of ten frames per facing
			limit = 4;
			break;
	}
	if (++GrappleAnimDelay >= limit) {
		GrappleAnimDelay = 0;
		if (++GrappleAnimFrame >= 10) {
			return(true);
		}
		if (GrappleAnim != NULL) {
			GrappleAnim->Set_Rate(128);
			GrappleAnim->Set_Stage(GrappleAnimFrame + base + dir.As_Dir8() * 10);
		}
	}
	return(false);
}


/// <summary>
/// One frame of the squid holding a ship: the grapple animation (SQDG) is made, the ship is shaken from
/// side to side, splashes are made and the ship takes its damage; it lets go when it is out of the water.
/// </summary>
void ParasiteClass::UpdateSquid(void) // RA2: 0x00606780
{
	WeaponTypeClass * weapon = Owner->Get_Class_Weapon_Data(0)->Weapon;
	int damage;
	WarheadTypeClass const * warhead = weapon->WarheadPtr;
	Coord coord = Victim->Position;
	CellClass * cell = Victim->Get_Cell_Ptr();
	double factor = 1.0;
	if (cell == NULL || cell->Is_Tile_Water()) { // RA2: the squid only works while the ship is on water or off the map
		Victim->ParalysisTimer = warhead->Paralyzes; // RA2: the ship is held for the warhead's paralyze time

		switch (GrappleState) {
			case 0:
			{
				Victim->AngleRotatedSideways = 0;
				AnimType anim = AnimTypeClass::From_Name("SQDG"); // RA2: the squid grapple animation
				if (anim != ANIM_NONE) {
					AnimTypeClass * type = AnimTypes[anim];
					if (type != NULL) {
						AnimClass * grapple = new AnimClass(type, coord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
						if (grapple != NULL) {
							GrappleAnim = grapple;
							AnimPtrTracker.Add(this);
						}
					}
				}
				GrappleState = 1;
				Reset_Grapple_Counters();
				UpdateGrapple();
				break;
			}

			case 1:
				if (UpdateGrapple()) {
					GrappleState = Scen->RandomNumber(0, 1) ? 2 : 3;
					Reset_Grapple_Counters();
					UpdateGrapple();
					Make_Wakes(Victim, coord);
				}
				break;

			case 2:
				factor = -1.0;

			case 3:
			{
				if (UpdateGrapple()) {
					GrappleState = 4;
					Reset_Grapple_Counters();
					UpdateGrapple();
				}
				double angle = GrappleAnimFrame * factor * 0.31415926535897931;
				Victim->AngleRotatedSideways = (float)fastmath::sin(angle) * 0.78539819f; // RA2: the ship rolls up to pi/4, sine of the frame angle (pi/10 per frame)
				break;
			}

			case 4:
			{
				Victim->AngleRotatedSideways = 0;
				int count = Rule->SplashList.Count();
				double rad = -Victim->PrimaryFacing.Current().As_Radian();
				double cosine = fastmath::cos(rad);
				double sine = fastmath::sin(rad);
				for (int i = 0; i < 3; i++) {
					double across = 64.0;
					double along = i * 128.0 - 128.0;
					if (Scen->RandomNumber(0, 1)) {
						across = -64.0;
					}
					int x = (int)(along * cosine - across * sine);
					int y = (int)(along * sine + across * cosine);
					Coord splash = coord + Coord(x, y, 0);
					new AnimClass(Rule->SplashList[Scen->RandomNumber(0, count - 1)], splash, 2, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), -10); // RA2: a random splash from the rules, delay 2, drawn 10 below
				}
				if (warhead->Culling && (Victim->Health_Status() == 0 || (Victim->Health_Status() == 1 && Owner->Veterancy.Is_Elite()))) { // RA2: a culling warhead finishes a ship in the red, or in the yellow when the squid is elite (Health_Status 0 red, 1 yellow)
					if (Owner->Techno_Type_Class()->IsTrainable) {
						TechnoTypeClass const * victimtype = Victim->Techno_Type_Class();
						TechnoTypeClass const * ownertype = Owner->Techno_Type_Class();
						Owner->Veterancy.Made_A_Kill(ownertype->Cost_Of(Owner->House), victimtype->Cost_Of(Victim->House));
					}
					FootClass * victim = Victim;
					ExitUnit();
					victim->IsSinking = true; // RA2: the ship sinks with the squid's kill credit
					victim->Death_Announcement(Owner);
					victim->Stun();
					Clear_Grapple();
					Owner->ParalysisTimer = 0;
					return;
				}
				damage = weapon->Attack;
				Victim->Take_Damage(damage, 0, warhead, Owner, false, true);
				if (Victim != NULL) {
					GrappleState = Scen->RandomNumber(0, 1) ? 2 : 3;
					Reset_Grapple_Counters();
					UpdateGrapple();
					Make_Wakes(Victim, coord);
				} else {
					Clear_Grapple();
				}
				break;
			}

			default:
				ExitUnit();
				break;
		}

		AnimClass * grapple = GrappleAnim;
		if (grapple != NULL && Owner != NULL && Victim != NULL) {
			CellClass * vcell = Victim->Get_Cell_Ptr();
			grapple->Attach_To(Victim);
			grapple->AlternativeDrawer = (ConvertClass *)Owner->GetRemapColour();
			if (vcell != NULL) {
				grapple->AlternativeBrightness = vcell->Brightness;
			}
		}
	} else {
		ExitUnit();
		if (Owner != NULL) {
			Owner->ParalysisTimer = 0;
		}
	}
}


/// <summary>
/// Makes three wake animations at random places around the given ship position, turned with the ship.
/// </summary>
static void __fastcall Make_Wakes(FootClass * victim, Coord coord) // RA2: 0x00606E10
{
	double rad = -victim->PrimaryFacing.Current().As_Radian();
	double cosine = fastmath::cos(rad);
	double sine = fastmath::sin(rad);
	for (int i = 0; i < 3; i++) {
		double along = Scen->RandomNumber(-180, 180);
		double across = Scen->RandomNumber(-64, 64);
		Coord wake = coord + Coord((int)(along * cosine - across * sine), (int)(along * sine + across * cosine), 0);
		new AnimClass(Rule->Wake, wake, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
	}
}


/// <summary>
/// One frame of the parasite: a squid in a ship is run by UpdateSquid; the drone delivers its damage to the
/// vehicle (a soldier is killed outright) each time its weapon is ready, with sparks and the weapon's animation.
/// </summary>
void ParasiteClass::AI(void) // RA2: 0x00606F50
{
	if (Victim == NULL) {
		return;
	}
	if (Victim->Techno_Type_Class()->Naval && !Victim->Techno_Type_Class()->Organic && Owner->Techno_Type_Class()->Naval && Owner->Techno_Type_Class()->Organic) { // RA2: only a naval organic owner (the squid) in a naval machine; the executable tests all four flags
		UpdateSquid();
		return;
	}
	WeaponTypeClass * weapon = Owner->Get_Class_Weapon_Data(0)->Weapon;
	WarheadTypeClass const * warhead = weapon->WarheadPtr;
	Coord coord = Victim->Position;
	if (DamageDeliveryTimer != 0) {
		return;
	}
	DamageDeliveryTimer = weapon->ROF;
	Victim->ParalysisTimer = warhead->Paralyzes;
	int damage;
	if (Victim->Fetch_RTTI() == RTTI_INFANTRY) {
		damage = Victim->Strength;
		Victim->Take_Damage(damage, 0, warhead, Owner, true, true);
	} else {
		new ParticleSystemClass(Rule->DefaultSparkSystem, coord, NULL, NULL, COORD_NONE);
		DirType dir = Victim->PrimaryFacing.Current();
		if (weapon->Anim.Count() != 0) {
			AnimTypeClass const * type = weapon->Anim[dir.As_Dir8()];
			if (type != NULL) {
				new AnimClass(type, coord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
			}
		}
		dir = dir.Left_90();
		int side = Scen->RandomNumber(0, 1) ? -2 : 2;
		double rad = dir.As_Radian();
		Coord pushed((int)(fastmath::cos(rad) * side + coord.X), (int)(coord.Y - fastmath::sin(rad) * side), coord.Z);
		Victim->Rock(pushed, 1.5f); // RA2: the vehicle is pushed two leptons sideways, force 1.5
		damage = weapon->Attack;
		Victim->Take_Damage(damage, 0, warhead, Owner, false, true);
	}
}


/// <summary>
/// The body of CanExistOnVictimCell, written once as an inline function. The executable expands it into ExitUnit and
/// Get_Exit_Coord and also keeps an out of line copy that nothing calls; the compiler expands a function this large
/// only when it is inline, so CanExistOnVictimCell itself forwards here and keeps the copy.
/// </summary>
static inline bool Owner_Fits_Victim_Cell(ParasiteClass const * parasite)
{
	if (parasite->Victim == NULL) {
		return(false);
	}
	if (parasite->Victim->In_Air()) {
		return(false);
	}
	if (!parasite->Owner->Techno_Type_Class()->Naval) {
		if (parasite->Owner != NULL && parasite->Owner->Fetch_RTTI() == RTTI_UNIT && parasite->Victim->Get_Cell_Ptr()->Cell_Terrain(false) != NULL) {
			return(false);
		}
		LandType land = parasite->Victim->Get_Cell_Ptr()->Land_Type();
		if (land == LAND_WATER || land == LAND_BEACH || land == LAND_ROCK) {
			return(false);
		}
	}
	return(true);
}


/// <summary>
/// Called when an object is going away: the owner and the grapple animation are forgotten; when the victim goes
/// the owner comes out again where the victim was (or dies when there is no room).
/// </summary>
void ParasiteClass::Detach(AbstractClass const * target, bool all) // RA2: 0x00607200
{
	if (target == Owner) {
		Owner = NULL;
		return;
	}
	if (target == Victim) {
		if (!ScenarioActive) {
			Victim = NULL;
			return;
		}
		if (SuppressionTimer > 0 && !Owner->Unknown_Virtual_32C()) { // RA2: the owner is under suppression and not iron curtained (vtable +0x32C)
			Victim->ParasiteEatingMe = NULL;
			Victim = NULL;
			Owner->Delete_Me();
			return;
		}
		Coord coord = Get_Exit_Coord();
		ScenarioInit++;
		if (coord != COORD_NONE) {
			if (Owner->Unlimbo(coord, Victim->PrimaryFacing.Current().As_Dir256())) {
				ScenarioInit--;
				if (Owner->ShouldBeReselectOnUnlimbo && Owner->House->Is_Player_Control()) {
					Owner->ShouldBeReselectOnUnlimbo = false;
					Owner->Select();
				}
				if (Owner->OldTeam != NULL) {
					Owner->OldTeam->Add(Owner);
				}
				if (!Owner->HaveMegaMission()) {
					Owner->Assign_Archive_Target(NULL);
					Owner->Assign_Target(NULL);
					Owner->Assign_Destination(NULL, true);
				}
				Owner->Enter_Idle_Mode(false, true);
				Owner->UpdateSight(false, false, false, NULL);
				Map.RevealArea3(Coord(Owner->Position), Owner->LastSightRange - 3, Owner->LastSightRange + 3, 0); // RA2: reveals the owner's sight range again, radius +3 (TryInfect uses +2)
				Victim = NULL;
				return;
			}
		}
		ScenarioInit--;
		Owner->Strength = 0;
		Owner->Delete_Me();
		Victim = NULL;
		return;
	}
	if (target == GrappleAnim) {
		GrappleAnim = NULL;
		GrappleAnimGotInvalid = true;
	}
}


/// <summary>
/// The owner leaves the victim: it is placed on a free cell beside it (a ship) or on its cell and goes back
/// to idle; when it cannot be placed or the victim is not a ship that lets it out, it dies.
/// </summary>
void ParasiteClass::ExitUnit(void) // RA2: 0x00607440
{
	bool naval = Owner->Techno_Type_Class()->Naval;
	int face = Victim->PrimaryFacing.Current().As_Dir8();
	DirType dir;
	if (face >= 0 && face <= 2) {
		dir = Victim->PrimaryFacing.Current().Right_90();
	} else {
		dir = Victim->PrimaryFacing.Current().Left_90();
	}
	Cell cell;
	if (naval) {
		cell = Adjacent_Cell(Victim->Get_Cell(), dir.As_Dir8());
	} else if (SuppressionTimer == 0) {
		cell = Victim->Get_Cell();
	} else {
		Owner->Strength = 0;
		Victim->ParalysisTimer = 0;
		Victim->ParasiteEatingMe = NULL;
		Owner->Delete_Me();
		Victim = NULL;
		return;
	}
	CellClass * cellptr = &Map[cell];
	if (!cellptr->Is_Clear_To_Move(Owner->Techno_Type_Class()->Speed, false, false, Map.Get_Cell_Zone(cell, Owner->Techno_Type_Class()->MZone, false), Owner->Techno_Type_Class()->MZone, -1, true)) {
		cell = Victim->Nearby_Location(NULL);
	}
	Coord coord = COORD_NONE;
	if (cell != CELL_NONE) {
		coord = Map[cell].Center_Coord();
	}
	if (cell == CELL_NONE || coord == COORD_NONE || !Owner_Fits_Victim_Cell(this) || !Owner->Unlimbo(coord, dir.As_Dir256())) {
		Owner->Strength = 0;
		Owner->Delete_Me();
	} else {
		if (Owner->ShouldBeReselectOnUnlimbo && Owner->House->Is_Player_Control()) {
			Owner->ShouldBeReselectOnUnlimbo = false;
			Owner->Select();
		}
		if (Owner->OldTeam != NULL) {
			Owner->OldTeam->Add(Owner);
		}
		if (!Owner->HaveMegaMission()) {
			Owner->Assign_Archive_Target(NULL);
			Owner->Assign_Target(NULL);
			Owner->Assign_Destination(NULL, true);
		}
		Owner->Enter_Idle_Mode(false, true);
		Owner->UpdateSight(false, false, false, NULL);
		Owner->ParalysisTimer = Owner->Get_Class_Weapon_Data(0)->Weapon->ROF * 3;
		Clear_Grapple();
	}
	Victim->AngleRotatedSideways = 0;
	Victim->ParalysisTimer = 0;
	Victim->ParasiteEatingMe = NULL;
	Victim = NULL;
}


/// <summary>
/// Tells whether the parasite can infect the given vehicle: it must be on the map and alive, not infected
/// already, of a type that can be infected and, for a squid, in the water.
/// </summary>
bool ParasiteClass::CanInfect(FootClass * victim) const // RA2: 0x006078F0
{
	if (victim != NULL && !victim->IsInLimbo && victim->IsActive && victim->Strength != 0 && victim->ParasiteEatingMe == NULL && victim->Techno_Type_Class()->Parasiteable) {
		if (Owner == NULL || !Owner->Techno_Type_Class()->Naval || victim->Get_Cell_Ptr() == NULL || victim->Get_Cell_Ptr()->Is_Tile_Water()) {
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// The sight range the foot object last revealed with. A small accessor like the executable's: it takes inline budget in TryInfect.
/// </summary>
static inline int Sight_Range_Of(FootClass const * foot) // RA2: the executable expands accessors here; they decide which later calls stay out of line
{
	return(foot->LastSightRange);
}


/// <summary>
/// Records the parasite's owner in the vehicle it infects.
/// </summary>
static inline void Set_Eaten_By(FootClass * victim, FootClass * owner)
{
	victim->ParasiteEatingMe = owner;
}


/// <summary>
/// The owner jumps at the target: when it can be infected the owner is held by it (its locomotion is forced
/// into its track), otherwise the owner is put back on the map where it jumped from.
/// </summary>
void ParasiteClass::TryInfect(FootClass * target) // RA2: 0x00607980
{
	Clear_Grapple();
	if (!CanInfect(target)) {
		if (!Owner->Unlimbo(Map[Owner->LastAdjacencyCell].Center_Coord(), DIR_N)) {
			Owner->Delete_Me();
			return;
		}
		Owner->RemoveSight(false, false, false, NULL);
		Map.RevealArea3(Coord(Owner->Position), Sight_Range_Of(Owner) - 3, Sight_Range_Of(Owner) + 2, 0);
		if (!Owner->HaveMegaMission()) {
			Owner->Assign_Target(NULL);
			Owner->Assign_Destination(NULL, true);
		}
		Owner->Enter_Idle_Mode(false, true);
		return;
	}
	Owner->Locomotion->Force_Track(-1, Coord(target->Position));
	Set_Eaten_By(target, Owner);
	Victim = target;
}


/// <summary>
/// Tells whether the owner could stand where the victim is: the victim must be on the ground, and a land
/// owner needs a cell without a terrain object that is not water, beach or rock.
/// </summary>
bool ParasiteClass::CanExistOnVictimCell(void) const // RA2: 0x00607BA0; nothing calls this copy, the callers expand Owner_Fits_Victim_Cell
{
	return(Owner_Fits_Victim_Cell(this));
}


/// <summary>
/// Finds where the owner comes out when the victim goes away: the victim's position when the owner can stand
/// there, otherwise the closest free spot on the cell the victim faces.
/// </summary>
/// <returns>Returns the position, or COORD_NONE when there is none.</returns>
Coord ParasiteClass::Get_Exit_Coord(void) // RA2: 0x00607C30; named for what it does (YR: sub_62AC30)
{
	if (Victim == NULL) {
		return(COORD_NONE);
	}
	if (Owner_Fits_Victim_Cell(this)) {
		return(Victim->Position);
	}
	FacingType face = Victim->PrimaryFacing.Current().As_Dir8();
	CellClass * victimcell = Victim->Get_Cell_Ptr();
	CellClass * cell = &victimcell->Adjacent_Cell(face);
	if (!Owner->Techno_Type_Class()->Naval) {
		LandType land = cell->Land_Type();
		if (land == LAND_WATER || land == LAND_BEACH || land == LAND_ROCK) {
			return(COORD_NONE);
		}
	}
	Coord coord = cell->Closest_Free_Spot(cell->Center_Coord(), false, false);
	if (coord != COORD_NONE) {
		return(coord);
	}
	return(COORD_NONE);
}


/// <summary>
/// The size of the object, for the save game and for copying.
/// </summary>
int ParasiteClass::Fetch_Object_Size(void) const // RA2: 0x00607DD0
{
	return(sizeof(*this));
}


/// <summary>
/// The kind of object this is.
/// </summary>
RTTIType ParasiteClass::Fetch_RTTI(void) const // RA2: 0x00607DE0
{
	return(RTTI_PARASITE);
}
