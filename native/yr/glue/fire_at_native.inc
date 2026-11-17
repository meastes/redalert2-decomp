// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y11b): TechnoClass::Fire_At, gamemd.exe 0x006FDD50 (7,167 bytes),
// unmatched in the YR tree (a link-closure trap: the first shot of a long game ended it). Written
// from behaviour: Red Alert 2's text (src/recovered/techno.cpp, TechnoClass::Fire_At 0x006F1E50 in
// game.exe) in the YR tree's names, with Yuri's Revenge's changes read from gamemd.exe's code:
// Armageddon mode instead of the debug map flag; the drain weapon (it latches on and does not fire);
// the spray attack (the target point steps round eight offsets, Static_Spray below); area fire at the
// firer's own cell; the veterancy firepower test; the occupied-building, bunker and open-topped
// damage multipliers; the disk laser (a DiskLaserClass fires instead of a bullet); the building's
// occupant cycling; recoil on turret-equipped types; the berserk half rearm; the occupant and
// open-topped firing animations; the sound skipped for gattling types; the magnetron beam; the last
// fire frame; the railgun's beam damage (gamemd.exe 0x0070C690, not in the tree; lane Y12, below).
// Behaviour code for the native build; never part
// of an exact build.
// ---------------------------------------------------------------------------------------------
#include "disklaser.h"
#include "wave.h"
#include "radbeam.h"
#include "spawnmgr.h"
#include "veteran.h"

// gamemd.exe 0x00B0EAA8: the spray attack's eight target offsets (built once; destroyed at exit by
// TechnoClass::FUN_006FF950).
static Coord const Static_Spray[8] = {
	Coord(256, 0, 0), Coord(180, 180, 0), Coord(0, 256, 0), Coord(-180, 180, 0),
	Coord(-256, 0, 0), Coord(-180, -180, 0), Coord(0, -256, 0), Coord(180, -180, 0)
};

// NON-EXACT NATIVE DRAFT (lane Y12): the railgun's beam damage, gamemd.exe 0x0070C690 (1,523 bytes, a
// TechnoClass member the tree declares but does not define), written from gamemd.exe's code. The beam is walked from
// the firing point to the target in 50 steps (i / 49 of the way); in each new cell it passes, every
// techno in the cell's occupier list (the bridge list when the cell is under a bridge and the beam is
// at least four levels above the cell) other than the firer is hit when it lies within
// Rule->RailgunDamageRadius of the beam's line (a building always), or when it is the target. If the
// beam meets the ground first it stops there: a destroyable cliff there may collapse
// (Rule->CollapseChance) and nothing is damaged. Otherwise the target is hit too (it is always hit
// when the walk missed it), each hit object takes the weapon's AmbientDamage with its warhead from
// the firer, a cliff cell aimed at may collapse, and the beam ends at the target. Returns the end.
Coord TechnoClass::Railgun_Beam_Damage(Coord & source, AbstractClass * target, WeaponTypeClass * weapon)
{
	TechnoClass * firer = this;
	Coord target_coord = target->Center_Coord();
	float sx = (float)source.X, sy = (float)source.Y, sz = (float)source.Z;
	float dx = (float)target_coord.X - sx, dy = (float)target_coord.Y - sy, dz = (float)target_coord.Z - sz;
	Cell last_cell = source.As_Cell();
	bool hit_target = false;
	DynamicVectorClass<ObjectClass *> hits;

	for (int step = 0; step < 50; step++) {
		Coord point = Lerp(source, target_coord, (float)step * (1.0f / 49.0f));
		Cell cell = point.As_Cell();
		if (cell == last_cell) {
			continue;
		}
		last_cell = cell;
		CellClass & cellref = Map[cell];
		bool bridge = cellref.IsUnderBridge && point.Z >= (cellref.Height + 4) * LEVEL_LEPTON_H;
		for (ObjectClass * object = cellref.Cell_Occupier(bridge); object != NULL; object = object->Next) {
			if (object == firer) {
				continue;
			}
			if (object == target) {
				hit_target = true;
			}
			if (!object->Is_Techno()) {
				continue;
			}
			float distance = 0.0f;
			if (object->Fetch_RTTI() != RTTI_BUILDING) {
				// the distance of the object from the beam's line: w = P - S, e = (w x D) x D, |e . w| / |e|
				float wx = (float)object->Position.X - sx, wy = (float)object->Position.Y - sy, wz = (float)object->Position.Z - sz;
				float cx = wy * dz - wz * dy, cy = wz * dx - wx * dz, cz = wx * dy - wy * dx;
				float ex = cy * dz - cz * dy, ey = cz * dx - cx * dz, ez = cx * dy - cy * dx;
				double length = sqrt((double)ex * ex + (double)ey * ey + (double)ez * ez);
				distance = length > 0.0 ? (float)fabs((ex * wx + ey * wy + ez * wz) / length) : 0.0f;
			}
			if (distance < (float)Rule->RailgunDamageRadius || object == target) {
				if (hits.ID(object) == -1) {
					hits.Add(object);
				}
			}
		}
		if (Map.Get_Height_GL(point) > point.Z) {
			CellClass * ground = &Map[point];
			if (ground->Is_Tile_Destroyable_Cliff() && Scen->RandomNumber(0, 99) < Rule->CollapseChance) {
				Map.Collapse_Cliff(&Map[point]);
			}
			return(point);
		}
	}
	if (!hit_target && target != NULL && (target->AbstractFlags & 2)) {
		if (hits.ID((ObjectClass *)target) == -1) {
			hits.Add((ObjectClass *)target);
		}
	}
	for (int index = 0; index < hits.Count(); index++) {
		int damage = weapon->AmbientDamage;
		hits[index]->Take_Damage(damage, 0, weapon->WarheadPtr, firer, false, false, NULL);
	}
	if (target->Fetch_RTTI() == RTTI_CELL) {
		CellClass * cell = (CellClass *)target;
		if (cell->Is_Tile_Destroyable_Cliff() && Scen->RandomNumber(0, 99) < Rule->CollapseChance) {
			Map.Collapse_Cliff(cell);
		}
	}
	return(target_coord);
}

BulletClass * TechnoClass::Fire_At(AbstractClass * target, int which)
{
	WeaponTypeClass * weapon = Get_Class_Weapon_Data(which)->Weapon;
	if (weapon == NULL) {
		return(NULL);
	}
	ObjectClass * object = (target != NULL && (target->AbstractFlags & 2)) ? (ObjectClass *)target : NULL;
	BulletTypeClass const * btype = weapon->Bullet;
	if (Unsorted::ArmageddonMode || target == NULL || (object != NULL && object->IsInLimbo)) {
		return(NULL);
	}
	if (weapon->Suicide) {
		int damage = Techno_Type_Class()->MaxStrength;
		Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false, NULL);
		return(NULL);
	}
	if ((weapon->UseFireParticles && ParticleSystems[0] != NULL) || (weapon->IsRailgun && ParticleSystems[4] != NULL)
		|| (weapon->UseSparkParticles && ParticleSystems[1] != NULL) || (weapon->IsSonic && Wave != NULL)) {
		return(NULL);
	}

	if (weapon->Spawner) {
		SpawnManager->SetTarget(target);
		if (IsOwnedByPlayer || IsDiscoveredByPlayer) {
			if (!Map.Is_Shrouded(Center_Coord()) && !Map.IsLocationFogged(Center_Coord())) {
				return(NULL);
			}
			if (Fetch_RTTI() == RTTI_INFANTRY && IsOwnedByPlayer) {
				return(NULL);
			}
		}
		if (object != NULL) {
			HouseClass * owner = object->Owner_HouseClass();
			if (owner != NULL && owner->Is_Player_Control() && weapon->RevealOnFire) {
				Coord center = Center_Coord();
				Map.RevealArea1(&center, 3, owner, false, false, false, true, false);
			}
			Map.RevealArea3(Center_Coord(), 0, 4, false);
		}
		return(NULL);
	}

	// Yuri's Revenge: a drain weapon (the Yuri slave miner's, the drainer's) latches on to a
	// building whose type can be drained and stops attacking.
	if (weapon->DrainWeapon) {
		if ((target->AbstractFlags & 1) && ((TechnoClass *)target)->Techno_Type_Class()->Drainable) {
			FUN_0070FD70((BuildingClass *)target);
			Assign_Target(NULL);
		}
		return(NULL);
	}

	TVelocity3D<double> velocity;
	Coord target_coord;
	if (Techno_Type_Class()->SprayAttack) {
		if (BurstIndex == 0) {
			SprayOffsetIndex = Scen->RandomNumber(0, 7);
		} else {
			SprayOffsetIndex = (SprayOffsetIndex + 8 / weapon->Burst) % 8;
		}
		target_coord = Position + Static_Spray[SprayOffsetIndex];
	} else if (weapon->AreaFire) {
		Coord center = Center_Coord();
		CellClass * cell = &Map[center.As_Cell()];
		target_coord = cell->Center_Coord();
		target = &Map[target_coord];
	} else if (object != NULL) {
		target_coord = object->Target_Coord();
	} else {
		target_coord = target->As_Coord();
	}
	if (Techno_Type_Class()->SprayAttack) {
		target = &Map[target_coord];
	}

	Coord fire_coord = Fire_Coord(which, Point3D(0, 0, 0));
	DirType dir;
	if (btype->ROT == 0 && !btype->IsDropping) {
		dir = DirType(fastmath::atan2((double)fire_coord.Y - (double)target_coord.Y, (double)target_coord.X - (double)fire_coord.X));
	} else {
		dir = Fire_Direction();
		if (btype->IsDropping) {
			fire_coord = Center_Coord();
		}
	}

	int firepower = weapon->Attack;
	if (weapon->IsSonic || weapon->UseFireParticles) {
		firepower = 0;
	} else if (firepower > 0) {
		firepower = (int)(House->FirepowerBias * FirepowerBias * firepower);
		if (Has_Ability(ABILITY_FIREPOWER)) {
			firepower = (int)(firepower * Rule->VeteranCombat);
		}
	}
	if (CanOccupyFire()) {
		firepower = (int)(firepower * Rule->OccupyDamageMultiplier);
	}
	if (BunkerLinkedItem != NULL && Fetch_RTTI() != RTTI_BUILDING) {
		firepower = (int)(firepower * Rule->BunkerDamageMultiplier);
	}
	if (InOpenToppedTransport) {
		firepower = (int)(firepower * Rule->OpenToppedDamageMultiplier);
	}

	// Yuri's Revenge: the disk laser (the floating disc) fires its own beams instead of a bullet.
	if (weapon->DiskLaser) {
		DiskLaserClass * laser = new DiskLaserClass;
		if (laser != NULL) {
			BurstIndex++;
			int delay = Rearm_Delay(which);
			ChargeTurretDelay = delay;
			Arm = delay;
			BurstIndex %= weapon->Burst;
			laser->Fire(this, (TechnoClass *)target, weapon, firepower);
			return(NULL);
		}
	}

	int dx = target_coord.X - fire_coord.X;
	int dy = target_coord.Y - fire_coord.Y;
	int distance = (int)fastmath::sqrt((double)(dx * dx + dy * dy));
	int speed = weapon->Get_Speed(distance);
	BulletClass * bullet = Create_Bullet(weapon->Bullet, target, this, firepower, weapon->WarheadPtr, speed, weapon->IsBright);
	if (bullet != NULL) {
		bullet->Set_Weapon_Type(weapon);
		bullet->Limbo();
		if (AbstractFlags & 4) {
			FootClass * foot = (FootClass *)this;
			if (foot->Locomotion->Is_Moving() && !Techno_Type_Class()->JumpJet) {
				bullet->IsInaccurate = true;
			}
		}
		if (!bullet->IsInaccurate && !btype->IsInaccurate) {
			TechnoClass * techno = (TarCom != NULL && (TarCom->AbstractFlags & 1)) ? (TechnoClass *)TarCom : NULL;
			WeaponTypeClass * bullet_weapon = bullet->Get_Weapon_Type();
			if (techno != NULL && bullet_weapon != NULL) {
				techno->EstimatedHealth -= Estimate_Damage(techno, bullet_weapon);
			}
		}

		Coord displacement = Predict_Target_Coord() - fire_coord;
		if (btype->IsInaccurate && btype->IsArcing) {
			int spread;
			if (!btype->FlakScatter || btype->IsInvisible) {
				spread = Scen->RandomNumber(Rule->BallisticScatter / 2, Rule->BallisticScatter);
			} else {
				float length = (float)fastmath::sqrt((double)displacement.X * displacement.X + (double)displacement.Y * displacement.Y + (double)displacement.Z * displacement.Z);
				spread = Scen->RandomNumber(0, Rule->BallisticScatter) * (int)length / Weapon_Range(which);
			}
			DirType facing(Scen->RandomNumber(0, 0x7FFFFFFE) * 4.656612877414201e-10 * 6.283185307179586);
			double angle = ((int)(short)facing.Raw - 0x3FFF) * -9.587672516830327e-05;
			displacement.Y = (int)((double)displacement.Y - fastmath::sin(angle) * (double)spread);
			displacement.X = (int)(fastmath::cos(angle) * (double)spread + (double)displacement.X);
		}
		if (btype->ROT == 0 && !btype->IsDropping) {
			dir = DirType(fastmath::atan2((double)-displacement.Y, (double)displacement.X));
		} else {
			dir = Fire_Direction();
			if (btype->IsDropping) {
				fire_coord = Center_Coord();
			}
		}
		int length = (int)fastmath::sqrt((double)displacement.X * displacement.X + (double)displacement.Y * displacement.Y + (double)displacement.Z * displacement.Z);
		if (speed > length / 2) {
			speed = length / 2;
		}

		int segments = Techno_Type_Class()->RadialFireSegments;
		BulletTypeClass const * projectile = weapon->Bullet;
		if (projectile->ROT > 0 || projectile->Vertical) {
			if (segments == 0) {
				speed = 1;
			}
			bullet->MaxSpeed = weapon->MaxSpeed;
		}
		velocity.X = 100.0;
		velocity.Y = 0.0;
		velocity.Z = 0.0;
		double ratio = (double)speed / fastmath::sqrt(velocity.X * velocity.X + velocity.Y * velocity.Y + velocity.Z * velocity.Z);
		velocity.X *= ratio;
		velocity.Y *= ratio;
		velocity.Z *= ratio;
		if (segments > 0) {
			double step = 6.283185307179586 / (double)segments * unknown_43C;
			DirType current = PrimaryFacing.Current();
			double base = ((int)(short)current.Raw - 0x3FFF) * -9.587672516830327e-05;
			velocity.Set_Yaw(DirType(base + step));
			unknown_43C++;
			if (unknown_43C >= segments) {
				unknown_43C = 0;
			}
		} else {
			velocity.Set_Yaw(dir);
		}

		int height = displacement.Z;
		DirType pitch((int)0x3FFF);
		bool valid = true;
		if (bullet->Class->IsArcing) {
			double gravity = (double)Rule->Gravity;
			if (bullet->Class->IsFloater) {
				gravity = Get_Floater_Gravity();
			}
			int planar = (int)fastmath::sqrt((double)displacement.X * displacement.X + (double)displacement.Y * displacement.Y);
			valid = Calculate_Projectile_Pitch(Should_Use_High_Arc(which), speed, planar, height, gravity, pitch);
		} else if (bullet->Class->IsVoxel) {
			if (target->Center_Coord().Z < Center_Coord().Z) {
				pitch = DirType((int)0x8000);
			} else {
				pitch = DirType((int)0x4000);
			}
		} else {
			int abs_z = height < 0 ? -height : height;
			if (abs_z > 200) {
				double y = 20.0;
				if (TarCom != NULL && TarCom->Fetch_RTTI() == RTTI_BUILDING) {
					Coord turret = Turret_Coord(which);
					height = ((BuildingClass *)TarCom)->Class->Height() * 200 - turret.Z;
					abs_z = height < 0 ? -height : height;
					if ((double)abs_z < 20.0) {
						y = 0.0;
					}
				}
				double planar = fastmath::sqrt((double)displacement.X * displacement.X + (double)displacement.Y * displacement.Y);
				if (planar < 1.0) {
					planar = 1.0;
				}
				double angle = fastmath::atan2f((float)((double)abs_z - y), (float)planar);
				if (height < 0) {
					angle = -angle;
				}
				pitch = DirType(angle);
			}
		}
		velocity.Set_Pitch(pitch);

		if (!valid) {
			bullet->Release();
			bullet = NULL;
		} else if (!bullet->Unlimbo(fire_coord, velocity)) {
			bullet->Release();
			bullet = NULL;
		} else {
			// Yuri's Revenge: an occupied building fires its occupants in turn.
			if (CanOccupyFire() && Fetch_RTTI() == RTTI_BUILDING) {
				BuildingClass * building = (BuildingClass *)this;
				building->FiringOccupantIndex++;
				building->FiringOccupantIndex %= building->GetOccupantCount();
			}
			if (bullet->Class->IsInvisible && object != NULL && object->IsOnBridge) {
				bullet->IsOnBridge = true;
			}
			if (Is_Turret_Equipped() && Techno_Type_Class()->TurretRecoil) {
				TurretRecoil.Fire();
				BarrelRecoil.Fire();
			}
			if (weapon->UseFireParticles && ParticleSystems[0] == NULL) {
				ParticleSystems[0] = new ParticleSystemClass(weapon->AttachedParticleSystem, fire_coord, target, this, Coord(0, 0, 0), NULL);
			}
			if (weapon->UseSparkParticles && ParticleSystems[1] == NULL) {
				ParticleSystems[1] = new ParticleSystemClass(weapon->AttachedParticleSystem, fire_coord, target, this, Coord(0, 0, 0), NULL);
			}
			if (weapon->IsRailgun && ParticleSystems[4] == NULL) {
				// gamemd.exe 0x006FF20F: the beam's damage first; the beam is drawn to where it ended.
				Coord beam_end = Railgun_Beam_Damage(fire_coord, target, weapon);
				ParticleSystems[4] = new ParticleSystemClass(weapon->AttachedParticleSystem, fire_coord, NULL, this, beam_end, NULL);
			}

			BurstIndex++;
			int delay = Rearm_Delay(which);
			if (Berzerk) {
				delay /= 2;
			}
			ChargeTurretDelay = delay;
			Arm = delay;
			BurstIndex %= weapon->Burst;

			AnimTypeClass const * anim_type = NULL;
			if (weapon->Anim.Count() == 8) {
				anim_type = weapon->Anim[(((Fire_Direction().Raw >> 12) + 1) >> 1 & 7) + 1 & 7];
			} else if (weapon->Anim.Count() > 0) {
				anim_type = weapon->Anim[0];
			}
			if (CanOccupyFire()) {
				anim_type = weapon->OccupantAnim;
			}
			if (anim_type == NULL && InOpenToppedTransport && weapon->OpenToppedAnim != NULL) {
				anim_type = weapon->OpenToppedAnim;
			}
			if (weapon->Sound.Count() > 0 && !Techno_Type_Class()->IsGattling) {
				VocClass::PlayAt(weapon->Sound[(unsigned short)SoundRandomSeed % weapon->Sound.Count()], fire_coord, NULL);
			}
			if (anim_type != NULL) {
				AnimClass * anim = new AnimClass(anim_type, fire_coord, 0, 1, (ShapeFlags_Type)0x600, 0, false);
				if (anim != NULL && Fetch_RTTI() == RTTI_BUILDING) {
					int zadjust = -((fire_coord.Y - Render_Coord().Y) / 4);
					anim->ZAdjust = zadjust < 0 ? zadjust : 0;
					if (GetOccupantCount() > 0) {
						anim->ZAdjust = -200;
					}
				}
				if (anim != NULL && Fetch_RTTI() != RTTI_BUILDING) {
					anim->SetOwnerObject(this);
				}
			}
			if (weapon->IsSonic) {
				Wave = new WaveClass(fire_coord, target_coord, this, WAVE_SONIC, (TechnoClass *)target);
			}
			if (Techno_Type_Class()->IsTargetLaser && House->Is_Player_Control()) {
				TargetingLaserTimer = 15;
			}
			if (weapon->IsLaser) {
				if (Fetch_RTTI() == RTTI_BUILDING) {
					LaserDrawClass * laser = CreateLaser(target, which, Get_Primary_Weapon(), Coord(0, 0, 0));
					BuildingClass * building = (BuildingClass *)this;
					if (laser != NULL && building->Class == Rule->PrismType) {
						laser->Thickness = 3;
						if (building->SupportingPrisms > 0) {
							laser->IsSupported = true;
							laser->Thickness = 5;
						}
					}
				} else {
					LaserDrawClass * laser = CreateLaser(target, which, weapon, Coord(0, 0, 0));
					if (laser != NULL && weapon->IsHouseColor) {
						laser->Thickness = 2;
					}
				}
			} else if (weapon->IsElectricBolt) {
				FireEBolt(object);
			} else if (weapon->IsRadBeam) {
				CreateRadBeam(object, (weapon->WarheadPtr != NULL && weapon->WarheadPtr->Temporal) ? 1 : 0);
			} else if (weapon->IsRadEruption) {
				CreateRadBeams((int)weapon->WarheadPtr->SpreadFactor);
			} else if (weapon->IsMagBeam) {
				if (Wave == NULL || (target != NULL && target->Fetch_RTTI() == RTTI_BUILDING)) {
					if (Wave == NULL) {
						Wave = new WaveClass(fire_coord, target_coord, this, (WaveType)3, (TechnoClass *)target);
					}
				}
			}
			Reduce_Ammunition();
			Mark(MARK_CHANGE);
			if (!(IsOwnedByPlayer || IsDiscoveredByPlayer)
				|| ((Map.Is_Shrouded(Center_Coord()) || Map.IsLocationFogged(Center_Coord())) && !(Fetch_RTTI() == RTTI_INFANTRY && IsOwnedByPlayer))) {
				if (object != NULL) {
					HouseClass * owner = object->Owner_HouseClass();
					if (owner != NULL && owner->Is_Player_Control() && weapon->RevealOnFire) {
						Coord center = Center_Coord();
						Map.RevealArea1(&center, 3, owner, false, false, false, true, false);
						Map.RevealArea3(Center_Coord(), 0, 4, false);
					}
				}
			}
			LastFireBulletFrame = Frame;
		}
	}

	if (weapon->LimboLaunch) {
		if (Techno_Type_Class()->ReselectIfLimboed && IsSelected && Owner_HouseClass()->Is_Player_Control()
			&& target->Fetch_RTTI() == RTTI_UNIT) {
			ShouldBeReselectOnUnlimbo = true;
		}
		if (Techno_Type_Class()->RejoinTeamIfLimboed && (AbstractFlags & 4) && ((FootClass *)this)->Team != NULL
			&& target->Fetch_RTTI() == RTTI_UNIT) {
			OldTeam = ((FootClass *)this)->Team;
		}
		Limbo();
		if (weapon->WarheadPtr->Parasite && bullet != NULL) {
			if (target->AbstractFlags & 4) {
				((FootClass *)target)->LastBeParasitedStartFrame = Frame + 20;
			}
			bullet->Limbo();
			bullet->Construct(weapon->Bullet, target, this, firepower, weapon->WarheadPtr, weapon->MaxSpeed, weapon->IsBright);
			bullet->Unlimbo(fire_coord, velocity);
		}
	}
	if (Techno_Type_Class()->DistributedFire && (CurrentMission != MISSION_GUARD || Fetch_RTTI() == RTTI_BUILDING)) {
		AttackedTargets.Add(target);
		Assign_Target(NULL);
	}
	if (weapon->FireOnce) {
		if ((AbstractFlags & 4) && ((FootClass *)this)->Team != NULL) {
			((FootClass *)this)->Team->Assign_Mission_Target(NULL);
			((FootClass *)this)->Team->IsNextMission = true;
		}
		Assign_Target(NULL);
	}
	return(bullet);
}
