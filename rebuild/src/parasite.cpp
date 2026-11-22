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
	Parasites.Add(this); // RA2: as above
}


/// <summary>
/// Makes the parasite of the given foot soldier or vehicle and lists it. It sits in no victim yet.
/// </summary>
ParasiteClass::ParasiteClass(FootClass * owner) // RA2: 0x00606240
{
	Owner = owner; // RA2: as above
	Victim = NULL; // RA2: as above
	DamageDeliveryTimer = 0; // RA2: as above
	SuppressionTimer = 0; // RA2: as above
	GrappleAnimGotInvalid = false; // RA2: as above
	GrappleAnimFrame = 0; // RA2: as above
	GrappleAnimDelay = 0; // RA2: as above
	GrappleAnim = NULL; // RA2: as above
	GrappleState = 0; // RA2: as above
	Parasites.Add(this); // RA2: as above
}


/// <summary>
/// The constructor used when the parasite is read from a save game; its fields come from the stream.
/// </summary>
ParasiteClass::ParasiteClass(NoInitClass const & x) : // RA2: 0x00606320
	AbstractClass(x) // RA2: as above
{
}


/// <summary>
/// Drops the grapple animation and takes the parasite out of the list of parasites.
/// </summary>
ParasiteClass::~ParasiteClass(void) // RA2: 0x00606370
{
	Clear_Grapple(); // RA2: the executable has the body of Clear_Grapple in line here
	Parasites.Delete(this); // RA2: as above
}


/// <summary>
/// Adds what makes the parasite differ between machines to the checksum of the game state.
/// </summary>
void ParasiteClass::Compute_CRC(CRCEngine & crc) const // RA2: 0x00606460
{
	BASECLASS::Compute_CRC(crc); // RA2: as above
	if (GrappleAnim != NULL) { // RA2: as above
		crc(GrappleAnim->Fetch_ID()); // RA2: as above
	}
	crc(GrappleState); // RA2: as above
	crc(GrappleAnimFrame); // RA2: as above
	crc(GrappleAnimDelay); // RA2: as above
	crc(GrappleAnimGotInvalid); // RA2: as above
}


/// <summary>
/// Drops the grapple animation (squid) and its tracker entry, and resets the grapple counters.
/// </summary>
void ParasiteClass::Clear_Grapple(void) // RA2: 0x006064C0; named for what it does (YR: sub_629530)
{
	GrappleAnimFrame = 0; // RA2: as above
	GrappleAnimDelay = 0; // RA2: as above
	GrappleState = 0; // RA2: as above
	if (GrappleAnim != NULL) { // RA2: as above
		GrappleAnim->Delete_Me(); // RA2: as above
		GrappleAnim = NULL; // RA2: as above
	}
	if (GrappleAnimGotInvalid) { // RA2: as above
		AnimPtrTracker.Delete(this); // RA2: as above
		GrappleAnimGotInvalid = false; // RA2: as above
	}
}


/// <summary>
/// Reads the parasite back in from the save game stream. The owner, the victim and the grapple animation
/// are pointers that the swizzler remaps afterwards.
/// </summary>
HRESULT STDMETHODCALLTYPE ParasiteClass::Load(IStream * stream) // RA2: 0x00606540
{
	HRESULT result = BASECLASS::Load(stream); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		new (this) ParasiteClass(NoInitClass()); // RA2: as above
		if (GrappleAnim != NULL) { // RA2: as above
			AnimPtrTracker.Add(this); // RA2: as above
		}
		Swizzle_Pointer(&Owner); // RA2: as above
		Swizzle_Pointer(&Victim); // RA2: as above
		Swizzle_Pointer(&GrappleAnim); // RA2: as above
	}
	return(result); // RA2: as above
}


/// <summary>
/// Writes the parasite to the save game stream; it has nothing of its own to add to the base object.
/// </summary>
HRESULT STDMETHODCALLTYPE ParasiteClass::Save(IStream * stream, BOOL cleardirty) // RA2: 0x00606640
{
	return(BASECLASS::Save(stream, cleardirty)); // RA2: as above
}


/// <summary>
/// Fetches the persistent class identifier for the parasite.
/// </summary>
HRESULT STDMETHODCALLTYPE ParasiteClass::GetClassID(CLSID * retval) // RA2: 0x00606660
{
	if (retval == NULL) return(E_POINTER); // RA2: as above
	*retval = CLSID_ParasiteClass; // RA2: as above
	return(S_OK); // RA2: as above
}


/// <summary>
/// Starts the grapple animation (squid) over from its first frame.
/// </summary>
void ParasiteClass::Reset_Grapple_Counters(void) // RA2: 0x006066A0; named for what it does (YR: sub_629710)
{
	GrappleAnimDelay = 0; // RA2: as above
	GrappleAnimFrame = 0; // RA2: as above
}


/// <summary>
/// Steps the squid's grapple animation: each state has its own frame set (by the victim's facing) and delay.
/// </summary>
/// <returns>Returns true when the last frame of the state was shown.</returns>
bool ParasiteClass::UpdateGrapple(void) // RA2: 0x006066B0
{
	DirType dir = Victim->PrimaryFacing.Current(); // RA2: as above
	int base; // RA2: as above
	int limit; // RA2: as above
	switch (GrappleState) { // RA2: as above
		case 0: // RA2: as above
		case 1: // RA2: as above
			base = 0; // RA2: as above
			limit = 3; // RA2: as above
			break; // RA2: as above
		case 2: // RA2: as above
			base = 80; // RA2: 0x50, the second set of ten frames per facing
			limit = 4; // RA2: as above
			break; // RA2: as above
		default: // RA2: as above
			base = 160; // RA2: 0xA0, the third set of ten frames per facing
			limit = 4; // RA2: as above
			break; // RA2: as above
	}
	if (++GrappleAnimDelay >= limit) { // RA2: as above
		GrappleAnimDelay = 0; // RA2: as above
		if (++GrappleAnimFrame >= 10) { // RA2: as above
			return(true); // RA2: as above
		}
		if (GrappleAnim != NULL) { // RA2: as above
			GrappleAnim->Set_Rate(128); // RA2: as above
			GrappleAnim->Set_Stage(GrappleAnimFrame + base + dir.As_Dir8() * 10); // RA2: as above
		}
	}
	return(false); // RA2: as above
}


/// <summary>
/// One frame of the squid holding a ship: the grapple animation (SQDG) is made, the ship is shaken from
/// side to side, splashes are made and the ship takes its damage; it lets go when it is out of the water.
/// </summary>
void ParasiteClass::UpdateSquid(void) // RA2: 0x00606780
{
	WeaponTypeClass * weapon = Owner->Get_Class_Weapon_Data(0)->Weapon; // RA2: as above
	int damage; // RA2: as above
	WarheadTypeClass const * warhead = weapon->WarheadPtr; // RA2: as above
	Coord coord = Victim->Position; // RA2: as above
	CellClass * cell = Victim->Get_Cell_Ptr(); // RA2: as above
	double factor = 1.0; // RA2: as above
	if (cell == NULL || cell->Is_Tile_Water()) { // RA2: the squid only works while the ship is on water or off the map
		Victim->ParalysisTimer = warhead->Paralyzes; // RA2: the ship is held for the warhead's paralyze time

		switch (GrappleState) { // RA2: as above
			case 0: // RA2: as above
			{
				Victim->AngleRotatedSideways = 0; // RA2: as above
				AnimType anim = AnimTypeClass::From_Name("SQDG"); // RA2: the squid grapple animation
				if (anim != -1) { // RA2: as above
					AnimTypeClass * type = AnimTypes[anim]; // RA2: as above
					if (type != NULL) { // RA2: as above
						AnimClass * grapple = new AnimClass(type, coord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
						if (grapple != NULL) { // RA2: as above
							GrappleAnim = grapple; // RA2: as above
							AnimPtrTracker.Add(this); // RA2: as above
						}
					}
				}
				GrappleState = 1; // RA2: as above
				Reset_Grapple_Counters(); // RA2: as above
				UpdateGrapple(); // RA2: as above
				break; // RA2: as above
			}

			case 1: // RA2: as above
				if (UpdateGrapple()) { // RA2: as above
					GrappleState = Scen->RandomNumber(0, 1) ? 2 : 3; // RA2: as above
					Reset_Grapple_Counters(); // RA2: as above
					UpdateGrapple(); // RA2: as above
					Make_Wakes(Victim, coord); // RA2: as above
				}
				break; // RA2: as above

			case 2: // RA2: as above
				factor = -1.0; // RA2: as above

			case 3: // RA2: as above
			{
				if (UpdateGrapple()) { // RA2: as above
					GrappleState = 4; // RA2: as above
					Reset_Grapple_Counters(); // RA2: as above
					UpdateGrapple(); // RA2: as above
				}
				double angle = GrappleAnimFrame * factor * 0.31415926535897931; // RA2: as above
				Victim->AngleRotatedSideways = (float)fastmath::sin(angle) * 0.78539819f; // RA2: the ship rolls up to pi/4, sine of the frame angle (pi/10 per frame)
				break; // RA2: as above
			}

			case 4: // RA2: as above
			{
				Victim->AngleRotatedSideways = 0; // RA2: as above
				int count = Rule->SplashList.Count(); // RA2: as above
				double rad = -Victim->PrimaryFacing.Current().As_Radian(); // RA2: as above
				double cosine = fastmath::cos(rad); // RA2: as above
				double sine = fastmath::sin(rad); // RA2: as above
				for (int i = 0; i < 3; i++) { // RA2: as above
					double across = 64.0; // RA2: as above
					double along = i * 128.0 - 128.0; // RA2: as above
					if (Scen->RandomNumber(0, 1)) { // RA2: as above
						across = -64.0; // RA2: as above
					}
					int x = (int)(along * cosine - across * sine); // RA2: as above
					int y = (int)(along * sine + across * cosine); // RA2: as above
					Coord splash = coord + Coord(x, y, 0); // RA2: as above
					new AnimClass(Rule->SplashList[Scen->RandomNumber(0, count - 1)], splash, 2, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), -10); // RA2: a random splash from the rules, delay 2, drawn 10 below
				}
				if (warhead->Culling && (Victim->Health_Status() == 0 || (Victim->Health_Status() == 1 && Owner->Veterancy.Is_Elite()))) { // RA2: a culling warhead finishes a ship in the red, or in the yellow when the squid is elite (Health_Status 0 red, 1 yellow)
					if (Owner->Techno_Type_Class()->IsTrainable) { // RA2: as above
						TechnoTypeClass const * victimtype = Victim->Techno_Type_Class(); // RA2: as above
						TechnoTypeClass const * ownertype = Owner->Techno_Type_Class(); // RA2: as above
						Owner->Veterancy.Made_A_Kill(ownertype->Cost_Of(Owner->House), victimtype->Cost_Of(Victim->House)); // RA2: as above
					}
					FootClass * victim = Victim; // RA2: as above
					ExitUnit(); // RA2: as above
					victim->IsSinking = true; // RA2: the ship sinks with the squid's kill credit
					victim->Death_Announcement(Owner); // RA2: as above
					victim->Stun(); // RA2: as above
					Clear_Grapple(); // RA2: as above
					Owner->ParalysisTimer = 0; // RA2: as above
					return; // RA2: as above
				}
				damage = weapon->Attack; // RA2: as above
				Victim->Take_Damage(damage, 0, warhead, Owner, false, true); // RA2: as above
				if (Victim != NULL) { // RA2: as above
					GrappleState = Scen->RandomNumber(0, 1) ? 2 : 3; // RA2: as above
					Reset_Grapple_Counters(); // RA2: as above
					UpdateGrapple(); // RA2: as above
					Make_Wakes(Victim, coord); // RA2: as above
				} else { // RA2: as above
					Clear_Grapple(); // RA2: as above
				}
				break; // RA2: as above
			}

			default: // RA2: as above
				ExitUnit(); // RA2: as above
				break; // RA2: as above
		}

		AnimClass * grapple = GrappleAnim; // RA2: as above
		if (grapple != NULL && Owner != NULL && Victim != NULL) { // RA2: as above
			CellClass * vcell = Victim->Get_Cell_Ptr(); // RA2: as above
			grapple->Attach_To(Victim); // RA2: as above
			grapple->AlternativeDrawer = (ConvertClass *)Owner->GetRemapColour(); // RA2: as above
			if (vcell != NULL) { // RA2: as above
				grapple->AlternativeBrightness = vcell->Brightness; // RA2: as above
			}
		}
	} else { // RA2: as above
		ExitUnit(); // RA2: as above
		if (Owner != NULL) { // RA2: as above
			Owner->ParalysisTimer = 0; // RA2: as above
		}
	}
}


/// <summary>
/// Makes three wake animations at random places around the given ship position, turned with the ship.
/// </summary>
static void __fastcall Make_Wakes(FootClass * victim, Coord coord) // RA2: 0x00606E10
{
	double rad = -victim->PrimaryFacing.Current().As_Radian(); // RA2: as above
	double cosine = fastmath::cos(rad); // RA2: as above
	double sine = fastmath::sin(rad); // RA2: as above
	for (int i = 0; i < 3; i++) { // RA2: as above
		double along = Scen->RandomNumber(-180, 180); // RA2: as above
		double across = Scen->RandomNumber(-64, 64); // RA2: as above
		Coord wake = coord + Coord((int)(along * cosine - across * sine), (int)(along * sine + across * cosine), 0); // RA2: as above
		new AnimClass(Rule->Wake, wake, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
	}
}


/// <summary>
/// One frame of the parasite: a squid in a ship is run by UpdateSquid; the drone delivers its damage to the
/// vehicle (a soldier is killed outright) each time its weapon is ready, with sparks and the weapon's animation.
/// </summary>
void ParasiteClass::AI(void) // RA2: 0x00606F50
{
	if (Victim == NULL) { // RA2: as above
		return; // RA2: as above
	}
	if (Victim->Techno_Type_Class()->Naval && !Victim->Techno_Type_Class()->Organic && Owner->Techno_Type_Class()->Naval && Owner->Techno_Type_Class()->Organic) { // RA2: only a naval organic owner (the squid) in a naval machine; the executable tests all four flags
		UpdateSquid(); // RA2: as above
		return; // RA2: as above
	}
	WeaponTypeClass * weapon = Owner->Get_Class_Weapon_Data(0)->Weapon; // RA2: as above
	WarheadTypeClass const * warhead = weapon->WarheadPtr; // RA2: as above
	Coord coord = Victim->Position; // RA2: as above
	if (DamageDeliveryTimer != 0) { // RA2: as above
		return; // RA2: as above
	}
	DamageDeliveryTimer = weapon->ROF; // RA2: as above
	Victim->ParalysisTimer = warhead->Paralyzes; // RA2: as above
	int damage; // RA2: as above
	if (Victim->Fetch_RTTI() == RTTI_INFANTRY) { // RA2: as above
		damage = Victim->Strength; // RA2: as above
		Victim->Take_Damage(damage, 0, warhead, Owner, true, true); // RA2: as above
	} else { // RA2: as above
		new ParticleSystemClass(Rule->DefaultSparkSystem, coord, NULL, NULL, COORD_NONE); // RA2: as above
		DirType dir = Victim->PrimaryFacing.Current(); // RA2: as above
		if (weapon->Anim.Count() != 0) { // RA2: as above
			AnimTypeClass const * type = weapon->Anim[dir.As_Dir8()]; // RA2: as above
			if (type != NULL) { // RA2: as above
				new AnimClass(type, coord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
			}
		}
		dir = dir.Left_90(); // RA2: as above
		int side = Scen->RandomNumber(0, 1) ? -2 : 2; // RA2: as above
		double rad = dir.As_Radian(); // RA2: as above
		Coord pushed((int)(fastmath::cos(rad) * side + coord.X), (int)(coord.Y - fastmath::sin(rad) * side), coord.Z); // RA2: as above
		Victim->Rock(pushed, 1.5f); // RA2: the vehicle is pushed two leptons sideways, force 1.5
		damage = weapon->Attack; // RA2: as above
		Victim->Take_Damage(damage, 0, warhead, Owner, false, true); // RA2: as above
	}
}


/// <summary>
/// The body of CanExistOnVictimCell, written once as an inline function. The executable expands it into ExitUnit and
/// Get_Exit_Coord and also keeps an out of line copy that nothing calls; the compiler expands a function this large
/// only when it is inline, so CanExistOnVictimCell itself forwards here and keeps the copy.
/// </summary>
static inline bool Owner_Fits_Victim_Cell(ParasiteClass const * parasite) // RA2: as above
{
	if (parasite->Victim == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	if (parasite->Victim->In_Air()) { // RA2: as above
		return(false); // RA2: as above
	}
	if (!parasite->Owner->Techno_Type_Class()->Naval) { // RA2: as above
		if (parasite->Owner != NULL && parasite->Owner->Fetch_RTTI() == RTTI_UNIT && parasite->Victim->Get_Cell_Ptr()->Cell_Terrain(false) != NULL) { // RA2: as above
			return(false); // RA2: as above
		}
		LandType land = parasite->Victim->Get_Cell_Ptr()->Land_Type(); // RA2: as above
		if (land == LAND_WATER || land == LAND_BEACH || land == LAND_ROCK) { // RA2: as above
			return(false); // RA2: as above
		}
	}
	return(true); // RA2: as above
}


/// <summary>
/// Called when an object is going away: the owner and the grapple animation are forgotten; when the victim goes
/// the owner comes out again where the victim was (or dies when there is no room).
/// </summary>
void ParasiteClass::Detach(AbstractClass const * target, bool all) // RA2: 0x00607200
{
	if (target == Owner) { // RA2: as above
		Owner = NULL; // RA2: as above
		return; // RA2: as above
	}
	if (target == Victim) { // RA2: as above
		if (!ScenarioActive) { // RA2: as above
			Victim = NULL; // RA2: as above
			return; // RA2: as above
		}
		if (SuppressionTimer > 0 && !Owner->Unknown_Virtual_32C()) { // RA2: the owner is under suppression and not iron curtained (vtable +0x32C)
			Victim->ParasiteEatingMe = NULL; // RA2: as above
			Victim = NULL; // RA2: as above
			Owner->Delete_Me(); // RA2: as above
			return; // RA2: as above
		}
		Coord coord = Get_Exit_Coord(); // RA2: as above
		ScenarioInit++; // RA2: as above
		if (coord != COORD_NONE) { // RA2: as above
			if (Owner->Unlimbo(coord, Victim->PrimaryFacing.Current().As_Dir256())) { // RA2: as above
				ScenarioInit--; // RA2: as above
				if (Owner->ShouldBeReselectOnUnlimbo && Owner->House->Is_Player_Control()) { // RA2: as above
					Owner->ShouldBeReselectOnUnlimbo = false; // RA2: as above
					Owner->Select(); // RA2: as above
				}
				if (Owner->OldTeam != NULL) { // RA2: as above
					Owner->OldTeam->Add(Owner); // RA2: as above
				}
				if (!Owner->HaveMegaMission()) { // RA2: as above
					Owner->Assign_Archive_Target(NULL); // RA2: as above
					Owner->Assign_Target(NULL); // RA2: as above
					Owner->Assign_Destination(NULL, true); // RA2: as above
				}
				Owner->Enter_Idle_Mode(false, true); // RA2: as above
				Owner->UpdateSight(false, false, false, NULL); // RA2: as above
				Map.RevealArea3(Coord(Owner->Position), Owner->LastSightRange - 3, Owner->LastSightRange + 3, 0); // RA2: reveals the owner's sight range again, radius +3 (TryInfect uses +2)
				Victim = NULL; // RA2: as above
				return; // RA2: as above
			}
		}
		ScenarioInit--; // RA2: as above
		Owner->Strength = 0; // RA2: as above
		Owner->Delete_Me(); // RA2: as above
		Victim = NULL; // RA2: as above
		return; // RA2: as above
	}
	if (target == GrappleAnim) { // RA2: as above
		GrappleAnim = NULL; // RA2: as above
		GrappleAnimGotInvalid = true; // RA2: as above
	}
}


/// <summary>
/// The owner leaves the victim: it is placed on a free cell beside it (a ship) or on its cell and goes back
/// to idle; when it cannot be placed or the victim is not a ship that lets it out, it dies.
/// </summary>
void ParasiteClass::ExitUnit(void) // RA2: 0x00607440
{
	bool naval = Owner->Techno_Type_Class()->Naval; // RA2: as above
	int face = Victim->PrimaryFacing.Current().As_Dir8(); // RA2: as above
	DirType dir; // RA2: as above
	if (face >= 0 && face <= 2) { // RA2: as above
		dir = Victim->PrimaryFacing.Current().Right_90(); // RA2: as above
	} else { // RA2: as above
		dir = Victim->PrimaryFacing.Current().Left_90(); // RA2: as above
	}
	Cell cell; // RA2: as above
	if (naval) { // RA2: as above
		cell = Adjacent_Cell(Victim->Get_Cell(), dir.As_Dir8()); // RA2: as above
	} else if (SuppressionTimer == 0) { // RA2: as above
		cell = Victim->Get_Cell(); // RA2: as above
	} else { // RA2: as above
		Owner->Strength = 0; // RA2: as above
		Victim->ParalysisTimer = 0; // RA2: as above
		Victim->ParasiteEatingMe = NULL; // RA2: as above
		Owner->Delete_Me(); // RA2: as above
		Victim = NULL; // RA2: as above
		return; // RA2: as above
	}
	CellClass * cellptr = &Map[cell]; // RA2: as above
	if (!cellptr->Is_Clear_To_Move(Owner->Techno_Type_Class()->Speed, false, false, Map.Get_Cell_Zone(cell, Owner->Techno_Type_Class()->MZone, false), Owner->Techno_Type_Class()->MZone, -1, true)) { // RA2: as above
		cell = Victim->Nearby_Location(NULL); // RA2: as above
	}
	Coord coord = COORD_NONE; // RA2: as above
	if (cell != CELL_NONE) { // RA2: as above
		coord = Map[cell].Center_Coord(); // RA2: as above
	}
	if (cell == CELL_NONE || coord == COORD_NONE || !Owner_Fits_Victim_Cell(this) || !Owner->Unlimbo(coord, dir.As_Dir256())) { // RA2: as above
		Owner->Strength = 0; // RA2: as above
		Owner->Delete_Me(); // RA2: as above
	} else { // RA2: as above
		if (Owner->ShouldBeReselectOnUnlimbo && Owner->House->Is_Player_Control()) { // RA2: as above
			Owner->ShouldBeReselectOnUnlimbo = false; // RA2: as above
			Owner->Select(); // RA2: as above
		}
		if (Owner->OldTeam != NULL) { // RA2: as above
			Owner->OldTeam->Add(Owner); // RA2: as above
		}
		if (!Owner->HaveMegaMission()) { // RA2: as above
			Owner->Assign_Archive_Target(NULL); // RA2: as above
			Owner->Assign_Target(NULL); // RA2: as above
			Owner->Assign_Destination(NULL, true); // RA2: as above
		}
		Owner->Enter_Idle_Mode(false, true); // RA2: as above
		Owner->UpdateSight(false, false, false, NULL); // RA2: as above
		Owner->ParalysisTimer = Owner->Get_Class_Weapon_Data(0)->Weapon->ROF * 3; // RA2: as above
		Clear_Grapple(); // RA2: as above
	}
	Victim->AngleRotatedSideways = 0; // RA2: as above
	Victim->ParalysisTimer = 0; // RA2: as above
	Victim->ParasiteEatingMe = NULL; // RA2: as above
	Victim = NULL; // RA2: as above
}


/// <summary>
/// Tells whether the parasite can infect the given vehicle: it must be on the map and alive, not infected
/// already, of a type that can be infected and, for a squid, in the water.
/// </summary>
bool ParasiteClass::CanInfect(FootClass * victim) const // RA2: 0x006078F0
{
	if (victim != NULL && !victim->IsInLimbo && victim->IsActive && victim->Strength != 0 && victim->ParasiteEatingMe == NULL && victim->Techno_Type_Class()->Parasiteable) { // RA2: as above
		if (Owner == NULL || !Owner->Techno_Type_Class()->Naval || victim->Get_Cell_Ptr() == NULL || victim->Get_Cell_Ptr()->Is_Tile_Water()) { // RA2: as above
			return(true); // RA2: as above
		}
	}
	return(false); // RA2: as above
}


/// <summary>
/// The sight range the foot object last revealed with. A small accessor like the executable's: it takes inline budget in TryInfect.
/// </summary>
static inline int Sight_Range_Of(FootClass const * foot) // RA2: the executable expands accessors here; they decide which later calls stay out of line
{
	return(foot->LastSightRange); // RA2: as above
}


/// <summary>
/// Records the parasite's owner in the vehicle it infects.
/// </summary>
static inline void Set_Eaten_By(FootClass * victim, FootClass * owner) // RA2: as above
{
	victim->ParasiteEatingMe = owner; // RA2: as above
}


/// <summary>
/// The owner jumps at the target: when it can be infected the owner is held by it (its locomotion is forced
/// into its track), otherwise the owner is put back on the map where it jumped from.
/// </summary>
void ParasiteClass::TryInfect(FootClass * target) // RA2: 0x00607980
{
	Clear_Grapple(); // RA2: as above
	if (!CanInfect(target)) { // RA2: as above
		if (!Owner->Unlimbo(Map[Owner->LastAdjacencyCell].Center_Coord(), DIR_N)) { // RA2: as above
			Owner->Delete_Me(); // RA2: as above
			return; // RA2: as above
		}
		Owner->RemoveSight(false, false, false, NULL); // RA2: as above
		Map.RevealArea3(Coord(Owner->Position), Sight_Range_Of(Owner) - 3, Sight_Range_Of(Owner) + 2, 0); // RA2: as above
		if (!Owner->HaveMegaMission()) { // RA2: as above
			Owner->Assign_Target(NULL); // RA2: as above
			Owner->Assign_Destination(NULL, true); // RA2: as above
		}
		Owner->Enter_Idle_Mode(false, true); // RA2: as above
		return; // RA2: as above
	}
	Owner->Locomotion->Force_Track(-1, Coord(target->Position)); // RA2: as above
	Set_Eaten_By(target, Owner); // RA2: as above
	Victim = target; // RA2: as above
}


/// <summary>
/// Tells whether the owner could stand where the victim is: the victim must be on the ground, and a land
/// owner needs a cell without a terrain object that is not water, beach or rock.
/// </summary>
bool ParasiteClass::CanExistOnVictimCell(void) const // RA2: 0x00607BA0; nothing calls this copy, the callers expand Owner_Fits_Victim_Cell
{
	return(Owner_Fits_Victim_Cell(this)); // RA2: as above
}


/// <summary>
/// Finds where the owner comes out when the victim goes away: the victim's position when the owner can stand
/// there, otherwise the closest free spot on the cell the victim faces.
/// </summary>
/// <returns>Returns the position, or COORD_NONE when there is none.</returns>
Coord ParasiteClass::Get_Exit_Coord(void) // RA2: 0x00607C30; named for what it does (YR: sub_62AC30)
{
	if (Victim == NULL) { // RA2: as above
		return(COORD_NONE); // RA2: as above
	}
	if (Owner_Fits_Victim_Cell(this)) { // RA2: as above
		return(Victim->Position); // RA2: as above
	}
	FacingType face = Victim->PrimaryFacing.Current().As_Dir8(); // RA2: as above
	CellClass * victimcell = Victim->Get_Cell_Ptr(); // RA2: as above
	CellClass * cell = &victimcell->Adjacent_Cell(face); // RA2: as above
	if (!Owner->Techno_Type_Class()->Naval) { // RA2: as above
		LandType land = cell->Land_Type(); // RA2: as above
		if (land == LAND_WATER || land == LAND_BEACH || land == LAND_ROCK) { // RA2: as above
			return(COORD_NONE); // RA2: as above
		}
	}
	Coord coord = cell->Closest_Free_Spot(cell->Center_Coord(), false, false); // RA2: as above
	if (coord != COORD_NONE) { // RA2: as above
		return(coord); // RA2: as above
	}
	return(COORD_NONE); // RA2: as above
}


/// <summary>
/// The size of the object, for the save game and for copying.
/// </summary>
int ParasiteClass::Fetch_Object_Size(void) const // RA2: 0x00607DD0
{
	return(sizeof(*this)); // RA2: as above
}


/// <summary>
/// The kind of object this is.
/// </summary>
RTTIType ParasiteClass::Fetch_RTTI(void) const // RA2: 0x00607DE0
{
	return(RTTI_PARASITE); // RA2: as above
}
