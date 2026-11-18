/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/COMBAT.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : COMBAT.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 19, 1994                                           *
 *                                                                                             *
 *                  Last Update : July 26, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Combat_Anim -- Determines explosion animation to play.                                    *
 *   Explosion_Damage -- Inflict an explosion damage affect.                                   *
 *   Modify_Damage -- Adjusts damage to reflect the nature of the target.                      *
 *   Wide_Area_Damage -- Apply wide area damage to the map.                                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "combat.h"

#include "_map.h"
#include "_rtti.h"
#include "_rules.h"
#include "_tactica.h"
#include "aircraft.h"
#include "anim.h"
#include "bound.h"
#include "building.h"
#include "builtype.h"
#include "ccrand.h"
#include "cell.h"
#include "globals.h"
#include "goptions.h" // RA2: Options.DetailLevel gates the combat light
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "inline.h"
#include "isotype.h"
#include "tracker.h"
#include "mouse.h"
#include "overtype.h"
#include "ovrlight.h"
#include "partsys.h"
#include "psystype.h"
#include "rules.h"
#include "scenario.h"
#include "stimer.h"
#include "tactical.h"
#include "team.h"
#include "teamtype.h"
#include "tiberium.h"
#include "unit.h"
#include "unittype.h"
#include "vanim.h"
#include "vector3.h"
#include "vein.h"
#include "warhead.h"

#include "mph.hh"

#include <windef.h>
inline static int Explosion_Distance(Coord const & coord1, Coord const & coord2); // RA2: defined after its first caller

namespace FPSCounter { extern unsigned CurrentFrameRate; }
namespace Detail { unsigned GetMinFrameRate(void); }



/***********************************************************************************************
 * Modify_Damage -- Adjusts damage to reflect the nature of the target.                        *
 *                                                                                             *
 *    This routine is the core of combat tactics. It implements the                            *
 *    affect various armor types have against various weapon types. By                         *
 *    careful exploitation of this table, tactical advantage can be                            *
 *    obtained.                                                                                *
 *                                                                                             *
 * INPUT:   damage   -- The damage points to process.                                          *
 *                                                                                             *
 *          warhead  -- The source of the damage points.                                       *
 *                                                                                             *
 *          armor    -- The type of armor defending against the damage.                        *
 *                                                                                             *
 *          distance -- The distance (in leptons) from the source of the damage.               *
 *                                                                                             *
 * OUTPUT:  Returns with the adjusted damage points to inflict upon the                        *
 *          target.                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/16/1994 JLB : Created.                                                                 *
 *   04/17/1994 JLB : Always does a minimum of damage.                                         *
 *   01/01/1995 JLB : Takes into account distance from damage source.                          *
 *   04/11/1996 JLB : Changed damage fall-off formula for less damage fall-off.                *
 *=============================================================================================*/
int Modify_Damage(int damage, WarheadTypeClass const * warhead, ArmorType armor, int distance)
{
	if (!damage || Scen->Special.IsInert || warhead == NULL) return(0);

	if (damage < 0) {
		if (distance < 8) return(damage); // RA2: Modify_Damage at 0x47E1C0 matches only so
		return(0);
	}

	float fdamage = (float)damage; // RA2: as above
	float fmin = fdamage * warhead->PercentAtMax; // RA2: as above
	int spread = (int)(warhead->SpreadFactor * 256.0f); // RA2: as above
	if (fmin != fdamage && spread) { // RA2: as above
		damage = (int)((fdamage - fmin) * (spread - distance) / spread + fmin); // RA2: as above
	}

	damage = MAX(damage, 0); // RA2: as above
	damage = (int)(damage * warhead->Modifier[armor]); // RA2: as above
	return(MIN(damage, Rule->MaxDamage)); // RA2: as above
} // RA2: Modify_Damage at 0x47E1C0 has no Bound, MAX, Rule and 3 more


/// <summary>
/// Handles the chain reaction of a tiberium field.
/// This routine is called when a cell of chain reactive growth is disturbed. The tiberium
/// there goes up, damaging whatever is standing in the blast, and the neighboring growth
/// is set off in turn so that the reaction ripples across the field.
/// </summary>
/// <param name="cell">The cell whose growth should be set off.</param>
void Chain_Reaction_Damage(Cell const & cell)
{
	// RA2: empty in RA2 (a bare ret at 0x47E2B0)
}


/***********************************************************************************************
 * Explosion_Damage -- Inflict an explosion damage affect.                                     *
 *                                                                                             *
 *    Processes the collateral damage affects typically caused by an                           *
 *    explosion.                                                                               *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate of ground zero.                                         *
 *                                                                                             *
 *          strength -- Raw damage points at ground zero.                                      *
 *                                                                                             *
 *          source   -- Source of the explosion (who is responsible).                          *
 *                                                                                             *
 *          warhead  -- The kind of explosion to process.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine can consume some time and will affect the AI                       *
 *             of nearby enemy units (possibly).                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1991 JLB : Created.                                                                 *
 *   11/30/1991 JLB : Uses coordinate system.                                                  *
 *   12/27/1991 JLB : Radius of explosion damage effect.                                       *
 *   04/13/1994 JLB : Streamlined.                                                             *
 *   04/16/1994 JLB : Warhead damage type modifier.                                            *
 *   04/17/1994 JLB : Cleaned up.                                                              *
 *   06/20/1994 JLB : Uses object pointers to distribute damage.                               *
 *   06/20/1994 JLB : Source is a pointer.                                                     *
 *   06/18/1996 JLB : Strength could be negative for healing effects.                          *
 *=============================================================================================*/
/*
**	RA2: the helpers Explosion_Damage expands inline: distances to a victim, the iron curtain test and the
**	bridge middle tiles.
*/
static inline int Explosion_Group_Distance(Coord const & a, Coord b)
{
	return((a - b).Length());
}

static inline int Explosion_Length(Point3D const & d)
{
	double x = d.X;
	double y = d.Y;
	double z = d.Z;
	return((int)fastmath::sqrt(x * x + y * y + z * z));
}

static inline bool Is_Techno_Object(ObjectClass * object)
{
	bool const & result = (object->AbstractFlags & ABSTRACT_TECHNO) != 0;
	return(result);
}

static inline bool Is_Iron_Curtained(ObjectClass * object)
{
	if (object) {
		if (Is_Techno_Object(object) && ((TechnoClass *)object)->Unknown_Virtual_32C()) {
			return(true);
		}
	}
	return(false);
}

static inline bool Is_Bridge_Middle(int ittype)
{
	return(ittype == IsometricTileTypeClass::BridgeMiddle1 + 0 ||
		ittype == IsometricTileTypeClass::BridgeMiddle1 + 1 ||
		ittype == IsometricTileTypeClass::BridgeMiddle1 + 2 ||
		ittype == IsometricTileTypeClass::BridgeMiddle1 + 3 ||
		ittype == IsometricTileTypeClass::BridgeMiddle2 + 0 ||
		ittype == IsometricTileTypeClass::BridgeMiddle2 + 1 ||
		ittype == IsometricTileTypeClass::BridgeMiddle2 + 2 ||
		ittype == IsometricTileTypeClass::BridgeMiddle2 + 3);
}


/// <summary>
/// Inflicts an explosion's damage on everything around it.
/// Aircraft in the blast radius are found through the aircraft tracker, everything on the ground
/// through the cells in reach; each victim then takes damage by its distance from the blast. A
/// victim under an iron curtain close to the blast shields the others (the result is 2). Rocker
/// warheads rock the vehicles around, bridges and exploding overlays in the cell are damaged, and
/// the warhead's particle system is spawned.
/// </summary>
/// <returns>2 when an iron curtained object blocked the blast, 1 when nothing was hit, else 0.</returns>
#ifdef RA2_NON_MATCHING // not exact: 4429 bytes differ (main's body carried; the original calls TPoint3D<int>(x, y, z) and Length out of line, 0x41B630 and 0x41B780)
int Explosion_Damage(Coord const & coord, int strength, TechnoClass * source, WarheadTypeClass const * warhead, bool dochainreaction)
{
	int distance;
	ObjectClass * object;
	DynamicVectorClass<DamageGroup *> groups;
	int i;

	if (!strength || Scen->Special.IsInert || warhead == NULL) {
		return(1);
	}

	int range = (int)(warhead->SpreadFactor * 256.0f);
	Cell cell = coord.As_Cell();
	CellClass * cellptr = &Map[cell];
	bool isbridge = false;
	bool hit = false;
	bool blocked = false;
	bool close = true;
	if (warhead->SpreadFactor > 0.5f) {
		close = false;
	}

	int cell_height = Map.Get_Height_GL(Coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0));
	if (cell_height < coord.Z) {
		AircraftTracker.Append_Radius_Bins(cellptr, (int)warhead->SpreadFactor);
		TechnoClass * techno;
		while ((techno = AircraftTracker.Get()) != NULL) {
			if (techno->IsActive && techno->IsDown && techno->Strength > 0) {
				distance = Explosion_Group_Distance(coord, techno->Position);
				if (distance <= range) {
					if (close && distance < 0x55 && techno->Unknown_Virtual_32C()) {
						blocked = true;
					}
					DamageGroup * group = new DamageGroup;
					group->Target = techno;
					group->Distance = Explosion_Group_Distance(coord, techno->Position);
					groups.Add(group);
				}
			}
		}
	}

	if (cellptr->IsUnderBridge && coord.Z > Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT / 2) {
		isbridge = true;
	}

	int rings = (int)(warhead->SpreadFactor + 0.99);
	for (int m = 0; m < MapClass::RadiusCount[rings]; m++) {
		Cell tcell = cell + MapClass::RadiusOffset[m];
		cellptr = &Map[tcell];
		if (cellptr->Overlay != OVERLAY_NONE) {
			OverlayTypeClass * overlay = OverlayTypes[cellptr->Overlay];
			if (overlay->IsChainReaction && (!overlay->IsTiberium || warhead->IsTiberiumDestroyer) && dochainreaction) {
				cellptr->Reduce_Tiberium(strength / 10);
			}
			if (overlay->IsWall) {
				if (warhead->WallAbsoluteDestroyer) {
					cellptr->Reduce_Wall(-1);
				} else if (warhead->IsWallDestroyer || (warhead->IsWoodDestroyer && overlay->Armor == ARMOR_WOOD)) {
					cellptr->Reduce_Wall(strength);
				}
			}
			if (cellptr->Overlay == OVERLAY_NONE) {
				TechnoClass::Remove_Target(cellptr);
			}
		}

		object = cellptr->Cell_Occupier(isbridge);
		for (; object; object = object->Next) {
			if (object != source && object->IsActive) {
				if (object->Fetch_RTTI() == RTTI_UNIT && Scen->Special.IsHarvesterImmune) {
					if (Rule->HarvesterUnit.Is_In_List((UnitTypeClass const *)object->Class_Of())) {
						continue;
					}
				}
				DamageGroup * group = new DamageGroup;
				group->Target = object;
				if (object->Fetch_RTTI() == RTTI_BUILDING) {
					if (m == 0) {
						if (coord.Z - cellptr->Center_Coord().Z > LEVEL_LEPTON_H * 2) {
							group->Distance = Explosion_Length(coord - cellptr->Center_Coord()) - LEVEL_LEPTON_H * 2;
						} else {
							group->Distance = 0;
						}
					} else {
						group->Distance = Explosion_Length(coord - cellptr->Center_Coord());
					}
				} else {
					group->Distance = Explosion_Length(coord - object->Target_Coord());
				}
				if (close && m == 0 && object) {
					if (Is_Techno_Object(object) && ((TechnoClass *)object)->Unknown_Virtual_32C() && group->Distance < 0x55) {
						blocked = true;
					}
				}
				groups.Add(group);
			}
		}
	}

	for (int j = 0; j < groups.Count(); j++) {
		object = groups[j]->Target;
		distance = groups[j]->Distance;
		if (object->IsActive && (object->Fetch_RTTI() != RTTI_BUILDING || !((BuildingClass *)object)->Class->IsInvisibleInGame)) {
			if (!blocked || Is_Iron_Curtained(object)) {
				if (object->Fetch_RTTI() == RTTI_AIRCRAFT && object->In_Air()) {
					distance /= 2;
				}
				if (object->Strength > 0 && object->IsDown && !object->IsInLimbo && distance <= range) {
					int damage = strength;
					object->Take_Damage(damage, distance, warhead, source, false, false);
					hit = true;
				}
			}
		}
	}

	for (int k = 0; k < groups.Count(); k++) {
		delete groups[k];
	}
	groups.Clear();

	if (blocked) {
		return(2);
	}

	double force = strength * 0.01 < 4.0 ? strength * 0.01 : 4.0;
	if (warhead->IsRocker && force > 0.3) {
		for (int x = cell.X - 3; x <= cell.X + 3; x++) {
			for (int y = cell.Y - 3; y <= cell.Y + 3; y++) {
				object = isbridge ? Map[Cell(x, y)].Cell_Occupier(true) : Map[Cell(x, y)].Cell_Occupier(false);
				for (; object; object = object->Next) {
					TechnoClass * techno = Dynamic_Cast<TechnoClass *>(object);
					if (techno) {
						if ((short)x == cell.X && (short)y == cell.Y && source) {
							Coord tcoord = techno->Position;
							Coord scoord = source->Position;
							Vector3 dir((float)(scoord.X - tcoord.X), (float)(scoord.Y - tcoord.Y), (float)(scoord.Z - tcoord.Z));
							dir.Normalize();
							dir = dir * 10.0f;
							tcoord += Coord((int)dir.X, (int)dir.Y, (int)dir.Z);
							techno->Rock(tcoord, (float)force);
						} else {
							techno->Rock(coord, (float)force);
						}
					}
				}
			}
		}
	}

	bool ion = warhead == Rule->IonCannonWarhead;
	cellptr = &Map[cell];
	if (Scen->Special.IsDestroyBridges && warhead->IsWallDestroyer) {
		int ittype = cellptr->ITType - IsometricTileTypeClass::BridgeSet + 1;
		CellClass * deck = NULL;
		if (cellptr->IsUnderBridge) {
			if (cellptr->IsBridgeDeck) {
				deck = &Map[cellptr->Fetch_CellID()];
			} else {
				deck = &Map[cellptr->BridgeDeckCell->Fetch_CellID()];
			}
		}

		if ((deck != NULL && (deck->Overlay == OVERLAY_BRIDGE1 || deck->Overlay == OVERLAY_BRIDGE2)) || Is_Bridge_Middle(ittype)) {
			if (!cellptr->IsUnderBridge || (coord.Z <= BRIDGE_LEPTON_HEIGHT + LEVEL_LEPTON_H * (cellptr->Height + 1) && coord.Z > BRIDGE_LEPTON_HEIGHT + LEVEL_LEPTON_H * (cellptr->Height - 2))) {
				if (warhead->IsWallDestroyer && (warhead == Rule->IonCannonWarhead || Random_Pick(1, Rule->BridgeStrength) < strength)) {
					bool damaged = Map.Damage_Bridge(cell);
					int tries = 3;
					while (!damaged && ion && tries > 0) {
						damaged = Map.Damage_Bridge(cell);
						tries--;
					}
					if (damaged) {
						TechnoClass::Remove_Target(cellptr);
					}
					Point2D point;
					TacticalMap->Coord_To_Pixel(coord, point);
					TacticalMap->Register_Dirty_Area(Rect(point.X - 128, point.Y - 128, 256, 256), false);
				}
			}
		}

		int ittype2 = cellptr->ITType - IsometricTileTypeClass::TrainBridgeSet + 1;
		if ((deck != NULL && (deck->Overlay == OVERLAY_RAIL_BRIDGE1 || deck->Overlay == OVERLAY_RAIL_BRIDGE2)) || Is_Bridge_Middle(ittype2)) {
			if (!cellptr->IsUnderBridge || (coord.Z <= BRIDGE_LEPTON_HEIGHT + LEVEL_LEPTON_H * (cellptr->Height + 1) && coord.Z > BRIDGE_LEPTON_HEIGHT + LEVEL_LEPTON_H * (cellptr->Height - 2))) {
				if (warhead->IsWallDestroyer && (warhead == Rule->IonCannonWarhead || Random_Pick(1, Rule->BridgeStrength) < strength)) {
					bool damaged = Map.Damage_Bridge(cell);
					int tries = 3;
					while (!damaged && ion && tries > 0) {
						damaged = Map.Damage_Bridge(cell);
						tries--;
					}
					if (damaged) {
						TechnoClass::Remove_Target(cellptr);
					}
					Point2D point;
					TacticalMap->Coord_To_Pixel(coord, point);
					TacticalMap->Register_Dirty_Area(Rect(point.X - 96, point.Y - 96, 192, 192), false);
				}
			}
		}

		if (cellptr->Is_Overlay_Low_Bridge()) {
			if (warhead == Rule->IonCannonWarhead || Random_Pick(1, Rule->BridgeStrength) < strength) {
				if (Map.Damage_Low_Bridge(cell)) {
					TechnoClass::Remove_Target(cellptr);
				}
			}
		}
		if (cellptr->Overlay >= 0xCD && cellptr->Overlay <= 0xE6) {
			if (warhead == Rule->IonCannonWarhead || Random_Pick(1, Rule->BridgeStrength) < strength) {
				if (Map.Damage_Low_Bridge2(cell)) {
					TechnoClass::Remove_Target(cellptr);
				}
			}
		}
	}

	if (cellptr->Overlay != OVERLAY_NONE && OverlayTypes[cellptr->Overlay]->IsExplosive) {
		cellptr->Register_For_Redraw();
		cellptr->Overlay = OVERLAY_NONE;
		cellptr->Recalc_Attributes(-1);
		Map.Update_Cell_Zone(cellptr->CellID);
		Map.Update_Cell_Subzones(cellptr->CellID);
		TechnoClass::Remove_Target(cellptr);

		new AnimClass(Rule->BarrelExplode, coord, 0, 1, ShapeFlags_Type(0x600), 0);
		Explosion_Damage(coord, Rule->AmmoCrateDamage, NULL, Rule->C4Warhead, true);
		for (i = 0; i < Rule->BarrelDebris.Count(); i++) {
			if (Percent_Chance(15)) {
				new VoxelAnimClass(Rule->BarrelDebris[i], coord, NULL);
				break;
			}
		}
		if (Percent_Chance(25)) {
			(new ParticleSystemClass(Rule->BarrelParticle, coord, NULL, NULL, COORD_NONE))->Spawn_Held_Particle(coord, coord);
		}
	}

	if (warhead->Particle != NULL) {
		ParticleSystemClass * psys;
		if (warhead->Particle->BehavesLike == 1) {
			psys = GasSystem;
		} else {
			psys = new ParticleSystemClass(warhead->Particle, coord, NULL, NULL, COORD_NONE);
		}
		psys->Spawn_Held_Particle(coord, coord);
	}

	return(!hit);
}
#endif


/***********************************************************************************************
 * Combat_Anim -- Determines explosion animation to play.                                      *
 *                                                                                             *
 *    This routine is called when a projectile impacts. This routine will determine what       *
 *    animation should be played.                                                              *
 *                                                                                             *
 * INPUT:   damage   -- The amount of damage this warhead possess (warhead size).              *
 *                                                                                             *
 *          warhead  -- The type of warhead.                                                   *
 *                                                                                             *
 *          land     -- The land type that this explosion is over. Sometimes, this makes       *
 *                      a difference (especially over water).                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the animation to play. If no animation is to be played, then          *
 *          ANIM_NONE is returned.                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
AnimTypeClass const * Combat_Anim(int damage, WarheadTypeClass const * warhead, LandType land, Coord const & coord)
{
	if (damage == 0 || warhead == NULL) { // RA2: Combat_Anim at 0x47F4D0 has no enum {
		return(NULL);
	}

	if (land == LAND_WATER && warhead->IsConventional && !Map[coord].IsUnderBridge && coord.Z < Map.Get_Height_GL(coord) + LEVEL_LEPTON_H * 2) { // RA2: Combat_Anim at 0x47F4D0 matches only so
		if (Rule->SplashList.Count()) {
			int val = MIN(damage, 35 * Rule->SplashList.Count() - 1); // RA2: as above
			return(Rule->SplashList[val / 35]); // RA2: as above
		}
		return(NULL);
	}

	if (warhead == Rule->LightningWarhead) { // RA2: as above
		return(Rule->WeatherConBoltExplosion); // RA2: as above
	}

	if (warhead->ExplosionSet.Count()) {
		if (warhead->IsEMEffect) {
			return(warhead->ExplosionSet[Random_Pick(0, warhead->ExplosionSet.Count() - 1)]);
		}
		int val = MIN(damage, 25 * warhead->ExplosionSet.Count() - 1); // RA2: as above
		return(warhead->ExplosionSet[val / 25]); // RA2: as above
	}

	return(NULL);
}


/// <summary>
/// Creates the lighting flash of an explosion.
/// This routine is called alongside the explosion animation so that a bright warhead
/// briefly lights up the terrain around the blast.
/// </summary>
/// <param name="coord">The coordinate the blast is centered upon.</param>
/// <param name="damage">The strength of the blast, which governs the size of the flash.</param>
/// <param name="forced">Should the flash occur even if the warhead is not a bright one?</param>
void Combat_Lighting(Coord coord, int damage, WarheadTypeClass const * warhead, int forced, int flags) // RA2: 0x47F600 takes the forced flag as an int and a fifth argument, the light kind flags
{
	if ((Options.DetailLevel != 0 && FPSCounter::CurrentFrameRate >= Detail::GetMinFrameRate()) || (flags & 0xF) != 0) { // RA2: the detail level and frame rate gate, bypassed by the flags
		if ((char)forced || warhead != NULL && warhead->IsBright) { // RA2: forced is tested by its low byte
			int size; // RA2: as above
			float light = warhead->CombatLightSize; // RA2: the warhead's own light size takes precedence
			if (light > 0.0f) { // RA2: as above
				if (light > 1.0f) { // RA2: as above
					light = 1.0f; // RA2: as above
				}
				size = (int)(light * 63.0f); // RA2: as above
			} else { // RA2: as above
				damage *= 64; // RA2: as above
				size = damage >> 8; // RA2: as above
				if (size >= 63) { // RA2: as above
					size = 63; // RA2: as above
				} else if (size <= 21) { // RA2: as above
					size = 21; // RA2: as above
				}
			}

			SpotLightClass * spot = new SpotLightClass(coord, size); // RA2: the light is kept to set its flags
			spot->Disable(flags); // RA2: the flags are ORed into the light
		}
	}
}




/***********************************************************************************************
 * Wide_Area_Damage -- Apply wide area damage to the map.                                      *
 *                                                                                             *
 *    This routine will apply damage to a very wide area on the map. The damage will be        *
 *    spread out from the coordinate specified by the radius specified. The amount of damage   *
 *    will attenuate according to the distance from center.                                    *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate that the explosion damage will center about.            *
 *                                                                                             *
 *          radius   -- The radius of the explosion.                                           *
 *                                                                                             *
 *          damage   -- The amount of damage to apply at the center location.                  *
 *                                                                                             *
 *          source   -- Pointer to the purpetrator of the damage (if any).                     *
 *                                                                                             *
 *          warhead  -- The type of warhead that is causing the damage.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Wide_Area_Damage(Coord const & coord, LEPTON radius, int rawdamage, TechnoClass * source, WarheadTypeClass const * warhead)
{
	if (warhead == NULL) return;

	int cell_radius = (radius + CELL_LEPTON_W-1) / CELL_LEPTON_W;
	Cell cell = coord.As_Cell();

	for (int x = -cell_radius; x <= cell_radius; x++) { // RA2: Wide_Area_Damage at 0x47F6E0 has no DeformThreshhold, Deform, Map
		for (int y = -cell_radius; y <= cell_radius; y++) {
			int xpos = cell.X + x;
			int ypos = cell.Y + y;
			if ((unsigned)xpos > MAP_CELL_W) {
				continue;
			}
			if ((unsigned)ypos > MAP_CELL_H) {
				continue;
			}
			Cell tcell = Cell(xpos, ypos);
			if (!Map.In_Radar(tcell)) continue;

			int dist_from_center = Distance(Coord(x+cell_radius, y+cell_radius), Coord(cell_radius, cell_radius));
			double damagemod = 1;
			if (dist_from_center > 0) {
				damagemod = 1.0 / ((double)cell_radius / dist_from_center);
			}
			Coord tcoord(tcell, coord.Z); // RA2: Wide_Area_Damage at 0x47F6E0 matches only so
			Explosion_Damage(tcoord, rawdamage * damagemod, source, warhead);
		}
	}
}


/// <summary>
/// Determines the pitch to launch a ballistic projectile at.
/// This is the routine the weapon code calls when it wants to lob something at a target.
/// It takes the solution from Calculate_Projectile_Angle and turns it into a pitch the launcher
/// can be aimed with, correcting the flat solution when it would send the shot skyward.
/// </summary>
/// <param name="high_arc">Should the shot be lobbed rather than fired flat?</param>
/// <param name="speed">The speed the projectile will be launched at.</param>
/// <param name="distance">The horizontal distance to the target.</param>
/// <param name="height">The height of the target relative to the launch point.</param>
/// <param name="gravity">The gravity that will act upon the projectile.</param>
/// <param name="pitch">Reference to the pitch to be filled in.</param>
/// <returns>bool; Was a firing solution found?</returns>
bool Calculate_Projectile_Pitch(bool high_arc, int speed, int distance, int height, double gravity, DirType & pitch)
{
	double angle;
	if (Calculate_Projectile_Angle(high_arc, speed, distance, height, gravity, angle)) {
		if (high_arc) {
			pitch = DirType(angle);
			return(true);
		}

		double test;
		Calculate_Projectile_Angle(false, speed, distance + 1, height, gravity, test);
		if (test < angle) {
			pitch = DirType(-angle);
		} else {
			pitch = DirType(angle);
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Determines the elevation angle for a ballistic shot.
/// This routine solves the projectile equation for the angle that will drop the shot onto
/// the target. A reachable target usually has two answers -- a flat fast one and a lobbed
/// one -- and the caller says which of them it wants.
/// </summary>
/// <param name="high_arc">Should the lobbed solution be returned rather than the flat one?</param>
/// <param name="speed">The speed the projectile will be launched at.</param>
/// <param name="distance">The horizontal distance to the target.</param>
/// <param name="height">The height of the target relative to the launch point.</param>
/// <param name="gravity">The gravity that will act upon the projectile.</param>
/// <param name="angle">Reference to the angle, in radians, to be filled in.</param>
/// <returns>bool; Was a firing solution found?</returns>
bool Calculate_Projectile_Angle(bool high_arc, int speed, int distance, int height, double gravity, double & angle)
{
	if (height > 0) { // RA2: Calculate_Projectile_Angle at 0x47F9B0 matches only so
		angle = 0.7853822499999998; // RA2: as above
		return(true); // RA2: as above
	}

	if ((double)distance == 0.0) {
		distance = 0;
	}

	double dx = (double)distance;
	double velocity = (double)speed; // RA2: as above
	double dy = (double)height;
	double v = velocity; // RA2: as above

	double vsq = v * v; // RA2: Calculate_Projectile_Angle at 0x47F9B0 has no double dy2 = dy * dy;
	double base = vsq - ((double)height * gravity); // RA2: as above
	double g2 = gravity * gravity;
	double dx2 = (dx * dx); // RA2: as above

	double value = (v * v * vsq)
				 - (2 * vsq * dy * gravity)
				 - (g2 * dx2);

	double denominator = (((dy * dy) / dx2) + 1.0) * 2; // RA2: as above

	if (value < 0.0 || denominator == 0.0) {
		return(false); // RA2: as above
	}

	double numerator = !high_arc ? base + fastmath::sqrt(value)
								  : base - fastmath::sqrt(value);

	value = numerator / denominator;

	if (value >= 0.0) {
		angle = fastmath::acos(fastmath::sqrt(value) / v);
		return(true);
	}
	return(false);
}


/// <summary>
/// RA2
/// </summary>
bool Calculate_Projectile_Dir(int speed, int distance, int height, double gravity, DirType & dir)
{
	double angle;
	if (Calculate_Projectile_Angle(false, speed, distance, height, gravity, angle)) {
		if (angle < 0.0) {
			angle += M_PI * 2;
		}
		dir = DirType(angle);
		return(true);
	}
	return(false);
}


/// <summary>
/// Determines the launch speed needed to cover a range.
/// This routine is used by artillery style weapons to pick a muzzle velocity that will
/// carry the shot out as far as the target sits.
/// </summary>
/// <param name="range">The distance the projectile must cover.</param>
/// <param name="gravity">The gravity that will act upon the projectile.</param>
/// <returns>Returns with the speed to launch the projectile at.</returns>
MPHType Calculate_Projectile_Speed(int range, double gravity)
{
	return((MPHType)(int)fastmath::sqrt((double)range * gravity * 1.2));
}


/// <summary>
/// Can a ballistic projectile reach the target at all?
/// This routine is used before a lobbed weapon is allowed to fire so that the object does
/// not line up a shot that gravity will never let it complete.
/// </summary>
/// <param name="speed">The speed the projectile would be launched at.</param>
/// <param name="distance">The horizontal distance to the target.</param>
/// <param name="height">The height of the target relative to the launch point.</param>
/// <param name="gravity">The gravity that will act upon the projectile.</param>
/// <returns>bool; Does a firing solution exist?</returns>
bool Is_Projectile_Trajectory_Valid(int speed, int distance, int height, double gravity)
{
	double v = (double)speed;
	double dx = (double)distance;
	if (dx == 0.0) dx = 0.001; // RA2: Is_Projectile_Trajectory_Valid at 0x47FBA0 matches only so
	double dy = (double)height;

	double y2 = dy * dy;
	double vsq = v * v;
	double g2 = gravity * gravity;
	double x2 = dx * dx;

	double discriminant = (v * v * vsq)
						- (2 * vsq * dy * gravity)
						- (g2 * x2);

	if (discriminant < 0.0) {
		return(false);
	}

	double denominator = 2 * ((y2 / x2) + 1.0);
	if (denominator == 0.0) {
		return(false); // RA2: as above
	}

	double base = vsq - (dy * gravity);

	if (((base + fastmath::sqrt(discriminant)) / denominator) >= 0.0 ||
		((base - fastmath::sqrt(discriminant)) / denominator) >= 0.0) {
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}


/// <summary>
/// Fetches the Z adjustment to display an explosion with.
/// Explosion animations are nudged toward the viewer so that the blast draws over the
/// object it is consuming instead of disappearing behind it.
/// </summary>
/// <returns>Returns with the Z adjustment to hand to the animation.</returns>
int Get_Explosion_Z(Coord coord)
{
	return(-15);
}


/// <summary>
/// Fetches the gravity that acts upon a floating projectile.
/// Bullets marked as floaters drift toward their target rather than plummeting, so the
/// firing and trajectory routines use this gentler pull in place of the rule gravity.
/// </summary>
/// <returns>Returns with the gravity to use for a floating projectile.</returns>
double Get_Floater_Gravity(void)
{
	return(Rule->Gravity * 0.5);
}


/// <summary>
/// Determines the distance between two points, ignoring a slight height difference.
/// This routine is used when explosion damage is being dealt out, so that a victim
/// perched a fraction of a level above or below the blast is not judged to be further
/// away than one standing level with it.
/// </summary>
/// <returns>Returns with the distance between the two coordinates.</returns>
inline static int Explosion_Distance(Coord const & coord1, Coord const & coord2)
{
	int z1 = coord1.Z;
	int z2 = coord2.Z;
	if (abs(z2 - z1) < LEVEL_LEPTON_H) {
		z2 = coord1.Z;
	}
	return(Coord(coord1.X - coord2.X, coord1.Y - coord2.Y, z1 - z2).Length());
}

