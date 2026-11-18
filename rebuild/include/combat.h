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

#pragma once

#include "coord.h"

#include "armor.hh"
#include "land.hh"
#include "mph.hh"

class WarheadTypeClass;
class AnimTypeClass;
class TechnoClass;
class Coord;
class DirType;
class ObjectClass;

/*
**	RA2: one victim of an explosion and its distance from the blast (Explosion_Damage gathers them first).
*/
struct DamageGroup
{
	ObjectClass * Target;
	int Distance;
};

int Modify_Damage(int damage, WarheadTypeClass const * warhead, ArmorType armor, int distance);
void Chain_Reaction_Damage(Cell const & cell);
int Explosion_Damage(Coord const & coord, int strength, TechnoClass * source, WarheadTypeClass const * warhead, bool dochainreaction=true); // RA2: returns 2 when an iron curtain blocked the blast, 1 when nothing was hit
AnimTypeClass const * Combat_Anim(int damage, WarheadTypeClass const * warhead, LandType land, Coord const & coord);
void Combat_Lighting(Coord coord, int damage, WarheadTypeClass const * warhead, int forced, int flags); // RA2: 0x0047F600; the RA2 form with the light kind flags, forced is tested by its low byte
void Wide_Area_Damage(Coord const & coord, LEPTON radius, int rawdamage, TechnoClass * source, WarheadTypeClass const * warhead);

bool Calculate_Projectile_Pitch(bool high_arc, int speed, int distance, int height, double gravity, DirType & pitch);
bool Calculate_Projectile_Angle(bool high_arc, int speed, int distance, int height, double gravity, double & angle);
bool Is_Projectile_Trajectory_Valid(int speed, int distance, int height, double gravity);

MPHType Calculate_Projectile_Speed(int range, double gravity);
int Get_Explosion_Z(Coord coord);
double Get_Floater_Gravity(void);
