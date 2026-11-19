// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y11): BulletClass functions unmatched in the YR tree, reached once
// units fire (TechnoClass::Fire_At creates and launches the projectile). Each follows the RA2 twin's
// text (src/recovered/bullet.cpp: Unlimbo is RA2 0x004616B0, exact; AI RA2 0x0045F710, a parked draft;
// Shrapnel RA2 0x00462C50) in the YR tree's names, checked against gamemd.exe's code; YR's
// differences are listed per function. Projectile_Motion (gamemd.exe 0x005B20F0, declared in
// missile.h, undefined in the tree) follows TibSun's text (TibSun archive code/missile.cpp;
// SPDX GPL-3.0-or-later, Copyright 2026 OpenTS contributors; THIRD_PARTY.md) with YR's Level
// parameter. Behaviour code for the native build only; never part of an exact build.
// ---------------------------------------------------------------------------------------------

#include "animtype.h"
#include "matrix3d.h"
#include "revent.h"
#include "techtype.h"
#include "voxel.h"

void FUN_0053AB70(void);	// ion.cpp: the nuke's flash (gamemd.exe 0x0053AB70)

// MapClass's ring tables (protected there; as combat_native.inc): the cells within each radius.
struct Shrapnel_Radius : MapClass {
	using MapClass::RadiusCount;
	using MapClass::RadiusOffset;
};


/*
**	One frame of guided flight (gamemd.exe 0x005B20F0, 2,101 bytes). TibSun's text; YR adds `level`:
**	a level projectile keeps its pitch instead of following the terrain below it.
*/
int Projectile_Motion(Coord & coord, TVelocity3D<double> & velocity, Coord const & target_coord, DirType const & rot, bool aircraft, bool airburst, bool veryhigh, bool level)
{
	if (target_coord == COORD_NONE) {
		DirType pitch = velocity.Get_Pitch();
		pitch.Turn(DirType(Dir256(DIR_E + DIR_STEP_128)), rot);
		velocity.Set_Pitch(pitch);
		return(1);
	}

	TVelocity3D<double> frame_velocity(velocity);
	coord = Coord(coord.X + (int)frame_velocity.X, coord.Y + (int)frame_velocity.Y, coord.Z + (int)frame_velocity.Z);
	int target_distance = coord.Distance_To(target_coord);
	int horizontal_distance = Point2D(coord.X, coord.Y).Distance_To(Point2D(target_coord.X, target_coord.Y));

	Coord delta(target_coord.X - coord.X, target_coord.Y - coord.Y, target_coord.Z - coord.Z);

	TVelocity3D<double> target_direction;
	target_direction = TVelocity3D<double>(delta.X, delta.Y, delta.Z);

	DirType desired_yaw = velocity.Get_Yaw();
	desired_yaw.Turn(target_direction.Get_Yaw(), rot);
	velocity.Set_Yaw(desired_yaw);

	DirType desired_pitch = velocity.Get_Pitch();

	if (!aircraft && (airburst || horizontal_distance > ((veryhigh ? 6 : 3) * CELL_LEPTON)) && rot.As_Dir256() > 1) {
		TVelocity3D<double> lookahead_velocity(velocity);
		Coord lookahead_coord(coord.X + (int)lookahead_velocity.X * 6, coord.Y + (int)lookahead_velocity.Y * 6, coord.Z + (int)lookahead_velocity.Z * 6);

		int terrain_height = Map.Get_Height_GL(lookahead_coord);
		if (Map[lookahead_coord].IsUnderBridge) {
			terrain_height += BRIDGE_LEPTON_HEIGHT;
		}

		int cell_distance = target_distance / CELL_LEPTON;
		if (airburst || veryhigh) {
			cell_distance = 10;
		} else if (cell_distance >= 5) {
			cell_distance = 5;
		}

		if (!level) {
			int height_delta = coord.Z - LEVEL_LEPTON_H * cell_distance - terrain_height;
			if (height_delta < -20) {
				coord.Z += 18;
			} else if (height_delta > 20) {
				coord.Z -= 18;
			}

			if (height_delta < -(LEVEL_LEPTON_H / 2)) {
				desired_pitch.Turn(DirType(DIR_NE), rot / 2);
			} else if (height_delta > LEVEL_LEPTON_H / 2) {
				desired_pitch.Turn(DirType(Dir256(DIR_E + DIR_STEP_32)), rot / 2);
			} else {
				DirType level_pitch(DIR_E);
				DirType turn_rate = rot / 2;
				desired_pitch.Turn(level_pitch, turn_rate);
			}
		}
	} else {
		DirType turn_rate = (rot + DirType(256));
		desired_pitch.Turn(target_direction.Get_Pitch(), turn_rate / 2);
	}

	velocity.Set_Pitch(desired_pitch);

	if (airburst) {
		delta.Z = 0;
	} else {
		delta.Z /= 4;
	}

	return(delta.Length());
}


static inline void Bullet_Set_Speed(TVelocity3D<double> & velocity, double speed)
{
	if (velocity.X == 0.0 && velocity.Y == 0.0 && velocity.Z == 0.0) {
		velocity.X = 100.0;
	}
	double factor = speed / fastmath::sqrt(velocity.X * velocity.X + velocity.Y * velocity.Y + velocity.Z * velocity.Z);
	velocity.X *= factor;
	velocity.Y *= factor;
	velocity.Z *= factor;
}


/*
**	BulletClass::Unlimbo (gamemd.exe 0x00468670, 1,304 bytes; vtable slot 0x1F0). The RA2 twin's text.
**	YR's difference: an invisible projectile stopped by a firestorm wall on its path is only moved
**	there (RA2 also had the wall's building take it and deleted the projectile).
*/
bool BulletClass::Unlimbo(Coord const & coord, TVelocity3D<double> const & velocity)
{
	if (!ObjectClass::Unlimbo(coord, DIR_N)) {
		return(false);
	}

	Velocity = velocity;
	SourceCoords = coord;
	LastMapCoords = SourceCoords.As_Cell();
	Map.Remove(this);

	Coord target = TarCom->As_Coord();
	TargetCoords = target;

	if (Class->FlakScatter && Class->IsInvisible) {
		float dx = (float)(coord.X - target.X);
		float dy = (float)(target.Y - coord.Y);
		float dz = (float)(coord.Z - target.Z);
		float distance = (float)fastmath::sqrt((double)dx * dx + (double)dy * dy + (double)dz * dz);
		int scatter = Scen->RandomNumber(0, Rule->BallisticScatter * 2);
		int radius = (int)((unsigned)scatter * (unsigned)(int)distance) / WeaponType->Range;
		int random_direction = Scen->RandomNumber(0, 0x7FFFFFFE);
		double random_angle = random_direction * 4.656612877414201e-10 * 6.283185307179586;
		short raw_direction = (short)(int)((random_angle - 1.5707963267948966) * -10430.060040584269);
		double angle = ((int)raw_direction - 0x3FFF) * -9.587672516830327e-05;
		int y = (int)((double)target.Y - fastmath::sin(angle) * (double)radius);
		int x = (int)(fastmath::cos(angle) * (double)radius + target.X);
		target = Coord(x, y, target.Z);
	}

	if (Class->IsInvisible) {
		HouseClass * house = Payback != NULL ? Payback->House : NULL;
		Coord collision = Map.Firestorm_On_Path(coord, target, house);
		if (collision != COORD_NONE) {
			collision.Z = Map.Get_Height_GL(collision);
			Set_Coord(collision);
		} else {
			CellClass * blocking = TrajectoryHelper::FindFirstObstacle(coord, target, Class, house);
			if (blocking != NULL) {
				Set_Coord(blocking->Center_Coord());
			} else {
				Set_Coord(target);
			}
			Set_Coord(target);	// gamemd.exe sets the target again after either branch
			MaxSpeed = 0;
			Bullet_Set_Speed(Velocity, 0.0);
		}
	}

	int arm = (TarCom != NULL && TarCom->Fetch_RTTI() == RTTI_AIRCRAFT) ? 0 : Class->Arming;
	Coord here = PositionCoord;
	Fuse.Arm_Fuse(here, target, arm, 0x7FFFFFFF);

	if (Class->ROT > 0) {
		Bullet_Set_Speed(Velocity, 1.0);
	}
	if (IsActive) {
		Map.Submit(this);
	}
	return(true);
}


/*
**	BulletClass::AI (gamemd.exe 0x004666E0, 6,422 bytes; vtable slot 0x5C). The RA2 twin's text (RA2's
**	own draft for RA2 0x0045F710) in the YR tree's names, its order checked against gamemd.exe. YR's
**	differences read from gamemd.exe: no firestorm wall test after the move, Projectile_Motion takes
**	the type's Level flag, a jumpjet firer's far fuse result counts as close.
*/
void BulletClass::AI(void)
{
	ObjectClass::AI();
	if (!IsActive) {
		return;
	}

	/*
	**	A nuke's projectile waits for its NUKEBALL animation before it explodes.
	*/
	if (SpawnNextAnim) {
		if (NextAnim != NULL) {
			return;
		}
		AnimPtrTracker.Delete(this);
		SpawnNextAnim = false;
		Bullet_Explodes(false);
		Delete_Me();
		return;
	}

	bool collided = false;
	bool forced = false;
	if (Class->IsDropping && !IsFalling) {
		forced = true;
	}

	if (Class->AnimLow || Class->AnimHigh) {
		if (--AnimRate == 0) {
			AnimRate = Class->AnimRate;
			AnimFrame++;
			if (AnimFrame > Class->AnimHigh) {
				AnimFrame = Class->AnimLow;
			}
		}
	}

	Coord coord = PositionCoord;
	if (Class->Trailer != NULL) {
		if (Class->ScaledSpawnDelay != 0) {
			if (Frame % Class->ScaledSpawnDelay == 0) {
				new AnimClass(Class->Trailer, coord, 1, 1, ShapeFlags_Type(0x600), 0);
			}
		} else if (Class->SpawnDelay != 0 && Frame % Class->SpawnDelay == 0) {	// NATIVE: a zero SpawnDelay divided by zero
			new AnimClass(Class->Trailer, coord, 1, 1, ShapeFlags_Type(0x600), 0);
		}
	}

	int impact = IMPACT_NONE;

	if (Class->ROT > 0) {
		int max_speed = MaxSpeed;
		double speed = Velocity.Speed();
		if (Class->CourseLockDuration == 0) {
			if (MaxSpeed >= 40 || speed + 0.5 >= max_speed) {
				IsLaunching = false;
			}
		} else if (CourseLockCounter < Class->CourseLockDuration) {
			if (++CourseLockCounter >= Class->CourseLockDuration) {
				IsLaunching = false;
			}
		}
		int acceleration = Class->Acceleration;
		if (IsLaunching && Class->CourseLockDuration == 0) {
			acceleration = (Frame % 2 == 0) ? 1 : 0;
		}
		if (speed < max_speed) {
			speed += acceleration;
			if (speed >= max_speed) {
				speed = max_speed;
			}
			Velocity.Set_Speed(speed);
		} else if (speed > max_speed) {
			speed -= acceleration / 2;
			if (speed <= 0.0) {
				speed = 0;
			}
			Velocity.Set_Speed(speed);
		}

		TVelocity3D<double> velocity = Velocity;
		Coord target_coord = TarCom != NULL ? TarCom->As_Coord() : COORD_NONE;
		if (TarCom != NULL && (TarCom->AbstractFlags & 2)) {
			target_coord = ((ObjectClass *)TarCom)->Target_Coord();
		}

		double & rotvar = Rule->MissileROTVar;
		double phase = ((Fetch_ID() + Frame) % 15) * (1.0 / 15);
		int rot = (int)((fastmath::sin(phase * 6.283185307179586) * rotvar + (rotvar + 1.0)) * Class->ROT);
		Coord center = Center_Coord();
		if (Coord(center.X - target_coord.X, center.Y - target_coord.Y, center.Z - target_coord.Z).Length() < 256) {
			rot = (int)(rot * 1.5);
		}

		Coord old_coord = coord;
		bool aircraft = (TarCom != NULL && TarCom->Fetch_RTTI() == RTTI_AIRCRAFT);
		DirType dir((Dir256)((IsLaunching ? 0 : rot) & 0xFF));
		int distance = Projectile_Motion(coord, velocity, target_coord, dir, aircraft, Class->IsAirburst, Class->IsVeryHigh, Class->Level);
		CellClass * cellptr = &Map[coord];
		Velocity = velocity;

		if (distance <= Velocity.Speed() * 0.5 || HeightAGL <= 0) {
			forced = true;
			impact = IMPACT_NORMAL;
			if (HeightAGL > 0 && !Class->IsAirburst) {
				if (target_coord != COORD_NONE) {
					coord = target_coord;
				}
			}
		}
		if (target_coord == COORD_NONE) {
			if (HeightAGL >= Rule->MissileSafetyAltitude) {
				forced = true;
				impact = IMPACT_NORMAL;
			}
		}

		int delta = old_coord.Distance_To(target_coord) - coord.Distance_To(target_coord);
		if (!IsLaunching) {
			if (ClosureSamples < 60) {
				ClosureSamples++;
				SmoothedClosure += delta;
			} else {
				SmoothedClosure = SmoothedClosure * (59.0 / 60) + delta;
				if (SmoothedClosure >= 0 && SmoothedClosure < 60.0 && !Class->IsAirburst && !Class->IsVeryHigh) {
					forced = true;
					impact = IMPACT_NORMAL;
					goto flown;
				}
			}
		}

		if (impact == IMPACT_NONE && (cellptr->IsUnderBridge || Map[old_coord].IsUnderBridge)) {
			int bridge_height = Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT;
			if ((coord.Z > bridge_height && old_coord.Z < bridge_height) || (coord.Z < bridge_height && old_coord.Z > bridge_height)) {
				impact = IMPACT_NORMAL;
				coord.Z = bridge_height;
				forced = true;
			}
		}
	} else {
		TVelocity3D<double> position = Velocity;
		TVelocity3D<double> velocity(position.X, position.Y, position.Z);
		if (position.Speed() < 8.0) {
			impact = IMPACT_NORMAL;
		}
		position.X = coord.X;
		position.Y = coord.Y;
		position.Z = coord.Z;
		double gravity = Rule->Gravity;
		if (Class->IsFloater) {
			gravity = Get_Floater_Gravity();
		}

		if (Class->Vertical) {
			int current = (int)Velocity.Speed();
			if (current < MaxSpeed) {
				Velocity.Set_Speed(Class->Acceleration + current);
			}
			TVelocity3D<double> step = Velocity;
			Coord old_coord = coord;
			coord = Coord(coord.X + (int)step.X, coord.Y + (int)step.Y, coord.Z + (int)step.Z);
			if (coord.Z > Class->DetonationAltitude) {
				impact = IMPACT_NORMAL;
				forced = true;
			} else if (HeightAGL < 0) {
				impact = IMPACT_NORMAL;
				forced = true;
			} else {
				int bridge_height = Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT;
				if (Map[coord].IsUnderBridge || Map[old_coord].IsUnderBridge) {
					if (coord.Z >= bridge_height) {
						if (old_coord.Z < bridge_height) {
							impact = IMPACT_NORMAL;
							forced = true;
						}
					} else if (old_coord.Z >= bridge_height) {
						impact = IMPACT_NORMAL;
						forced = true;
					}
				}
			}
		} else {
			TechnoClass * payback = Payback;
			double elasticity = Class->Elasticity;
			velocity.Z -= gravity;
			Coord old_position((int)position.X, (int)position.Y, (int)position.Z);
			position += velocity;
			Coord new_coord((int)position.X, (int)position.Y, (int)position.Z);
			int height = Map.Get_Height_GL(new_coord);
			int bridge_height = height + BRIDGE_LEPTON_HEIGHT;
			CellClass * cell = &Map[new_coord];
			bool fell = false;
			bool rose = false;
			if (cell->IsUnderBridge || Map[old_position].IsUnderBridge) {
				if (new_coord.Z >= bridge_height) {
					if (old_position.Z < bridge_height) {
						rose = true;
					}
				} else if (old_position.Z >= bridge_height) {
					fell = true;
				}
			}
			bool hit = false;
			if (!fell && !rose && position.Z >= height && position.Z - 150.0 < height) {
				BuildingClass * building = cell->Cell_Building();
				if (building != NULL || cell->Has_Wall_Or_Gate()) {
					hit = true;
					if (building != NULL) {
						if (building == (BuildingClass *)payback || (building->Class->IsLaserFence && building->LaserFenceFrame >= 8)) {
							hit = false;
						}
						if (building->IsStrange()) {	// YR (lane N4): TibSun's Considered_Vehicle is the YR tree's IsStrange (BuildingClass: Class->IsVehicle(), vtable 0x80); the tree's Considered_Vehicle is another slot (Height > -20), true for every building
							hit = false;
						}
						if (payback != NULL && payback->House->Is_Ally(building)) {
							hit = false;
						}
					}
				}
			}
			if (position.Z < height || fell || rose || hit) {
				bool bounce = true;
				if (fell) {
					position.Z = bridge_height;
				} else if (rose) {
					bridge_height -= 20;
					position.Z = bridge_height;
				} else if (payback == NULL) {
					bridge_height = height - 100;
					if (bridge_height < position.Z) {
						position.Z = height;
					}
				} else {
					bounce = false;
				}
				if (bounce) {
					if (payback == NULL) {
						Matrix3D slope;
						Matrix3D inverse = Matrix3D::Orthogonal_Inverse(slope = Get_Slope_Matrix(TacticalMap->Get_Cell_Ramp(new_coord)));
						Vector3 vec((float)velocity.X, -(float)velocity.Y, (float)velocity.Z);
						vec = inverse.Rotate_Vector(vec);
						vec *= (float)elasticity;
						vec.Z = -vec.Z;
						vec = slope.Rotate_Vector(vec);
						velocity.X = vec.X;
						velocity.Y = -vec.Y;
						velocity.Z = vec.Z;
					}
					impact = IMPACT_NORMAL;
					forced = true;
				}
			}
			coord.X = (int)position.X;
			coord.Y = (int)position.Y;
			coord.Z = (int)position.Z;
		}

		Cell here = coord.As_Cell();
		Cell there = TargetCoords.As_Cell();
		if (here == there && !Class->Vertical && HeightAGL < DAT_0089DE70 * 2) {
			impact = IMPACT_NORMAL;
			forced = true;
			collided = true;
		} else {
			BuildingClass * building = Map[here].Cell_Building();
			BuildingClass * target_building = Map[there].Cell_Building();
			if (building != NULL && !Class->Vertical && building == target_building && HeightAGL < DAT_0089DE70 * 2) {
				impact = IMPACT_NORMAL;
				forced = true;
				collided = true;
			} else {
				TechnoClass * techno = Map[coord].Cell_Techno(Point2D(0, 0), false, NULL);
				bool is_owner = (Payback != NULL && techno == Payback);
				bool ally = (techno != NULL && Payback != NULL && Payback->House->Is_Ally(techno));
				bool close = (techno != NULL && coord.Distance_To(techno->PositionCoord) < 0x80);
				if (!is_owner && !ally && close) {
					impact = IMPACT_NORMAL;
					forced = true;
					if (!Class->IsInaccurate) {
						coord = techno->PositionCoord;
					}
				} else if (!Map.In_Radar(coord)) {
					impact = IMPACT_EDGE;
					forced = true;
					coord = PositionCoord;
				} else {
					if (!Class->Vertical) {
						Velocity = TVelocity3D<double>(velocity.X, velocity.Y, velocity.Z);
					}
					if (Velocity.Speed() < 10.0 && HeightAGL < 10) {
						impact = IMPACT_NORMAL;
						forced = true;
					}
				}
			}
		}
	}

flown:
	if (impact == IMPACT_EDGE) {
		Mark(MARK_CHANGE);
		Delete_Me();
	} else {
		Mark(MARK_CHANGE);
		Set_Coord(coord);

		if (!forced) {
			Coord current = PositionCoord;
			forced = Is_Forced_To_Explode(current);
			Set_Coord(current);
		}
		if (forced && HeightAGL < 0) {
			HeightAGL = 0;
		}

		FuseResultType fuse = (Class->ROT > 0 || Class->IsFueled) ? Fuse.Fuse_Checkup(coord) : FUSE_WAIT;
		if (Payback != NULL && Payback->Techno_Type_Class()->JumpJet && fuse == FUSE_EXPLODE_FAR) {
			fuse = FUSE_EXPLODE_CLOSE;
		}

		if (!forced && (Class->IsDropping || fuse == FUSE_WAIT)) {
			if (Class->IsDegenerate && Strength > 5) {
				Strength--;
			}
		} else {
			if (TarCom != NULL && (fuse == FUSE_EXPLODE_CLOSE || collided) && !Class->IsAirburst && !Class->IsInaccurate) {
				Coord midpoint = coord;
				Coord target_coord = TarCom->As_Coord();
				midpoint.Z = (midpoint.Z + target_coord.Z) / 2;
				int distance = midpoint.Distance_To(target_coord);
				if (collided) {
					distance /= 3;
				}
				double limit = Velocity.Speed() * 2;
				if (limit < 128.0) {
					limit = 128.0;
				}
				if (fuse == FUSE_EXPLODE_CLOSE || distance <= limit) {
					Set_Coord(TarCom->Center_Coord());
				}
			}

			if (Warhead != NULL && *(AbstractTypeClass const *)Warhead == "NUKE") {
				if (HeightAGL < 0) {
					HeightAGL = 0;
				}
				FUN_0053AB70();
				RadarEventClass::Create(RadarEventType(13), Get_Cell());
				int index = AnimTypeClass::From_Name("NUKEBALL");
				AnimTypeClass * type;
				if (index != -1 && (type = AnimTypes[index]) != NULL) {
					Coord here = PositionCoord;
					NextAnim = new AnimClass(type, here, 0, 1, ShapeFlags_Type(0x2600), Get_Explosion_Z(here));
					SpawnNextAnim = true;
					AnimPtrTracker.Add(this);
					LastMapCoords = coord.As_Cell();
					return;
				}
			}
			Bullet_Explodes(forced);
			Delete_Me();
		}
	}
	LastMapCoords = coord.As_Cell();
}


/*
**	BulletClass::Shrapnel (gamemd.exe 0x0046A310, 2,757 bytes): fragments of the type's ShrapnelWeapon
**	at the enemies around the cell (rings of growing radius), the rest at random cells nearby. The RA2
**	twin's text (RA2 0x00462C50); gamemd.exe's code makes the same calls (the fragments' lasers and
**	electric bolts included).
*/
void BulletClass::Shrapnel(void)
{
	int shrapnel = Class->ShrapnelCount;
	int count;
	if (shrapnel < 0) {
		if (Payback == NULL) {
			count = 3;
		} else {
			Coord here = PositionCoord;
			Coord center = Payback->Center_Coord();
			count = -shrapnel - Coord(center.X - here.X, center.Y - here.Y, center.Z - here.Z).Length() / CELL_LEPTON;
			if (count <= 0) {
				return;
			}
		}
	} else {
		count = shrapnel;
	}

	CellClass * centercell = Get_Cell_Ptr();
	if (centercell->Cell_Occupier() == NULL) {
		return;
	}
	if (centercell->Cell_Occupier()->Fetch_RTTI() == RTTI_BUILDING) {
		return;
	}

	WeaponTypeClass const * weapon = Class->ShrapnelWeapon;
	BulletTypeClass const * projectile = weapon->Bullet;
	Cell origin = Get_Cell();
	int range = weapon->Range / CELL_LEPTON;
	int fired = 0;
	TVelocity3D<double> velocity;

	for (int radius = 1; radius <= range && radius < (int)(sizeof(Shrapnel_Radius::RadiusCount) / sizeof(Shrapnel_Radius::RadiusCount[0])); radius++) {
		int end = Shrapnel_Radius::RadiusCount[radius];
		for (int index = Shrapnel_Radius::RadiusCount[radius - 1]; index < end; index++) {
			Cell cell(origin.X + Shrapnel_Radius::RadiusOffset[index].X, origin.Y + Shrapnel_Radius::RadiusOffset[index].Y);
			CellClass * cellptr = &Map[cell];
			ObjectClass * target = cellptr->Cell_Occupier();
			if (target != NULL && target != Payback && Payback != NULL && !Payback->House->Is_Ally(target)) {
				BulletClass * fragment = Create_Bullet(projectile, target, Payback, weapon->Attack, weapon->WarheadPtr, weapon->MaxSpeed, weapon->IsBright);
				if (fragment != NULL) {
					Coord here = PositionCoord;
					velocity.Set(0, 0, 0);
					velocity.Set_Yaw(DirType().Direction(here, target->Center_Coord()));
					velocity.Set_Pitch(DirType(0x2000));
					velocity.Set_Speed(weapon->MaxSpeed);
					fragment->Unlimbo(here, velocity);
					if (weapon->IsLaser) {
						Payback->CreateLaser(target, 0, (WeaponTypeClass *)weapon, here);
					}
					if (weapon->IsElectricBolt) {
						Payback->CreateEBolt(target, (WeaponTypeClass *)weapon, here);
					}
				}
				if (++fired == count) {
					return;
				}
			}
		}
	}

	for (int remaining = count - fired; remaining > 0; remaining--) {
		Cell cell = Get_Cell();
		cell.X += Random_Pick(-2, 2);
		cell.Y += Random_Pick(-2, 2);
		CellClass * cellptr = &Map[cell];
		BulletClass * fragment = Create_Bullet(projectile, cellptr, Payback, weapon->Attack, weapon->WarheadPtr, weapon->MaxSpeed, weapon->IsBright);
		if (fragment != NULL) {
			Coord here = PositionCoord;
			velocity.Set(0, 0, 0);
			velocity.Set_Yaw(DirType().Direction(here, cellptr->Center_Coord()));
			velocity.Set_Speed(weapon->MaxSpeed);
			velocity.Set_Pitch(DirType(0x2000));
			fragment->Unlimbo(here, velocity);
			if (weapon->IsLaser && Payback != NULL) {
				Payback->CreateLaser(cellptr, 0, (WeaponTypeClass *)weapon, here);
			}
			if (weapon->IsElectricBolt && Payback != NULL) {
				Payback->CreateEBolt(cellptr, (WeaponTypeClass *)weapon, here);
			}
		}
	}
}
