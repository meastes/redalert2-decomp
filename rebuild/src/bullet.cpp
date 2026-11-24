/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/BULLET.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BULLET.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 23, 1994                                               *
 *                                                                                             *
 *                  Last Update : October 10, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BulletClass::AI -- Logic processing for bullet.                                           *
 *   BulletClass::BulletClass -- Bullet constructor.                                           *
 *   BulletClass::Bullet_Explodes -- Performs bullet explosion logic.                          *
 *   BulletClass::Detach -- Removes specified target from this bullet's targeting system.      *
 *   BulletClass::Draw_It -- Displays the bullet at location specified.                        *
 *   BulletClass::In_Which_Layer -- Fetches the layer that the bullet resides in.              *
 *   BulletClass::Init -- Clears the bullets array for scenario preparation.                   *
 *   BulletClass::Is_Forced_To_Explode -- Checks if bullet should explode NOW.                 *
 *   BulletClass::Mark -- Performs related map refreshing under bullet.                        *
 *   BulletClass::Occupy_List -- Determines the bullet occupation list.                        *
 *   BulletClass::Shape_Number -- Fetches the shape number for the bullet object.              *
 *   BulletClass::Sort_Y -- Sort coordinate for bullet rendering.                              *
 *   BulletClass::Target_Coord -- Fetches coordinate to use when firing on this object.        *
 *   BulletClass::Unlimbo -- Transitions a bullet object into the game render/logic system.    *
 *   BulletClass::delete -- Bullet memory delete.                                              *
 *   BulletClass::new -- Allocates memory for bullet object.                                   *
 *   BulletClass::~BulletClass -- Destructor for bullet objects.                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table

#include "bullet.h"
#include "temporal.h"
#include "cast.h"
#include "parasite.h"
#include "bomb.h"
#include "bomblist.h"
#include "capmgr.h"
#include "voc.h"
#include "unittype.h"

#include "_convert.h"
#include "_map.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "aircraft.h"
#include "anim.h"
#include "building.h"
#include "bullettype.h"
#include "ccrand.h"
#include "cell.h"
#include "combat.h"
#include "convert.h"
#include "draw.h"
#include "empulse.h"
#include "fly.h"
#include "house.h"
#include "houstype.h"
#include "infantry.h"
#include "inline.h"
#include "lightcon.h"
#include "map.h"
#include "missile.h"
#include "overtype.h"
#include "partsys.h"
#include "rules.h"
#include "scheme.h"
#include "stimer.h"
#include "sun.h"
#include "swizzle.h"
#include "tactical.h"
#include "techno.h"
#include "tracker.h"
#include "unit.h"
#include "voxdrsys.h"
#include "warhead.h"
#include "weapon.h"
#include "radsite.h" // RA2
#include "trajhelp.h" // RA2: TrajectoryHelper::GetObstacle
#include "globals.h"


DynamicVectorClass<BulletClass *> BulletClass::ScalableBullets; // RA2: 0x008502F8, the Scalable bullets in flight




/***********************************************************************************************
 * BulletClass::BulletClass -- Bullet constructor.                                             *
 *                                                                                             *
 *    This is the constructor for the bullet class. It handles all                             *
 *    initialization of the bullet and starting it in motion toward its                        *
 *    target.                                                                                  *
 *                                                                                             *
 * INPUT:   id       -- The type of bullet this is (could be missile).                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *   06/20/1994 JLB : Firer is a base class pointer.                                           *
 *   12/10/1994 JLB : Auto calculate range optional.                                           *
 *   12/12/1994 JLB : Handles small arms as an instantaneous effect.                           *
 *   12/23/1994 JLB : Fixed scatter algorithm for non-homing projectiles.                      *
 *   12/31/1994 JLB : Removed range parameter (not needed).                                    *
 *=============================================================================================*/
BulletClass::BulletClass() :
    Class(0),
    Payback(0),
    IsInaccurate(false),
    IsBright(false),
    BounceCount(0),
    field_A4(true),
    IsLaunching(true),
    CourseLockCounter(0),
    TarCom(0),
    MaxSpeed(0),
    InheritedColor(-1),
    ClosureSamples(0),
    SmoothedClosure(0),
    Warhead(0),
    AnimFrame(0),
    AnimRate(0),
    WeaponType(0),
    LastMapCoords(-1, -1),
    NextAnim(0),
    SpawnNextAnim(false)
{
    Create_ID();
    SourceCoords.X = 0;
    SourceCoords.Y = 0;
    SourceCoords.Z = 0;
    TargetCoords.X = 0;
    TargetCoords.Y = 0;
    TargetCoords.Z = 0;
    Bullets.Add(this);
}


/// <summary>
/// Fills in the firing data for a newly created projectile.
/// This routine is used by Create_Bullet to tell the projectile everything it must know
/// about the shot -- what it is, what it was aimed at, and who is to be credited for the
/// damage it causes.
/// </summary>
/// <param name="payback">The object that fired the shot. It receives credit for any kill.</param>
/// <param name="strength">The damage the projectile will inflict when it detonates.</param>
/// <param name="bright">Should the impact throw off a lighting flash?</param>
void BulletClass::Set_Bullet_Data(BulletTypeClass const *type, AbstractClass *target, TechnoClass *payback, int strength, WarheadTypeClass const *warhead, int max_speed, bool bright) // RA2: no range (YR's Construct), at 0x45F4F0
{
	Class = (BulletTypeClass *)type;
	Payback = payback;
	TarCom = target;
	MaxSpeed = max_speed;
	Warhead = (WarheadTypeClass *)warhead;
	IsBright = bright;
	Strength = strength;
	AnimFrame = 0;
	AnimRate = Class->AnimRate;
	if (Class->FirersPalette && Payback != NULL) {
		InheritedColor = Payback->House->Scheme;
	} else {
		InheritedColor = -1;
	}
	DamageMultiplier = 256;
	NextAnim = NULL;
	SpawnNextAnim = false;
}





/***********************************************************************************************
 * BulletClass::~BulletClass -- Destructor for bullet objects.                                 *
 *                                                                                             *
 *    The bullet destructor must detect if a dog has been attached to this bullet. If so,      *
 *    then the attached dog must be unlimboed back onto the map. This operation is necessary   *
 *    because, unlike other objects, the dog flies with the bullet it fires.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass::~BulletClass(void)
{
	Detach_This_From_All(this, true);

	if (SpawnNextAnim) { // RA2: a bullet with an anim was tracked in AnimPtrTracker (inlined Delete at 0x45F5C0)
		AnimPtrTracker.Delete(this);
	}

	if (GameActive) {
		BulletClass::Limbo();
	}

	Class=0;
	Payback=0;
	NextAnim = NULL; // RA2: also forgets the anim (+0x13C)
	Bullets.Delete(this);
}



/***********************************************************************************************
 * BulletClass::Occupy_List -- Determines the bullet occupation list.                          *
 *                                                                                             *
 *    This function will determine the cell occupation list and return a pointer to it. Most   *
 *    bullets are small and the list is usually short, but on occasion, it can be a list that  *
 *    rivals the size of regular vehicles.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cell offset list that covers all the cells a bullet  *
 *          is over.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   01/05/1995 JLB : Handles projectiles with altitude.                                       *
 *=============================================================================================*/
Cell const * BulletClass::Occupy_List(bool) const
{
	static Cell const _list[] = { REFRESH_EOL };

	return(_list);
}


/***********************************************************************************************
 * BulletClass::Mark -- Performs related map refreshing under bullet.                          *
 *                                                                                             *
 *    This routine marks the objects under the bullet so that they will                        *
 *    be redrawn. This is necessary as the bullet moves -- objects under                       *
 *    its path need to be restored.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BulletClass::Mark(MarkType mark)
{
	if (BASECLASS::Mark(mark)) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BulletClass::AI -- Logic processing for bullet.                                             *
 *                                                                                             *
 *    This routine will perform all logic (flight) logic on the bullet.                        *
 *    Primarily this is motion, fuse tracking, and detonation logic. Call                      *
 *    this routine no more than once per bullet per game tick.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 4639 bytes differ against the image-derived bound 0045f710 (6438 body bytes, image_bounds.csv).
#include "ion.h"
#include "revent.h"
void BulletClass::AI(void)
{
	/*
	 * This is how many frames the closure detector spends warming up, and also
	 * the closure value beneath which a homing projectile is judged to have
	 * stopped gaining on its target.
	 */
	static const int _limit = 4 * TICKS_PER_SECOND;


	BASECLASS::AI();



	if (!IsActive) return;

	// RA2: the nuke animation holds the bullet until it finishes.
	if (SpawnNextAnim) {
		if (NextAnim) return;
		AnimPtrTracker.Delete(this);
		SpawnNextAnim = false;
		Bullet_Explodes(false);
		Delete_Me();
		return;
	}

	/*
	**	Ballistic objects are handled here.
	*/
	bool collided = false;
	bool forced = false;			// Forced explosion.

	/*
	 * A dropping projectile that has finished its fall has arrived and must
	 * detonate.
	 */
	if (Class->IsDropping && !IsFalling) {
		forced = true;
	}

	/*
	 * Projectiles that carry an animation cycle through their frames here,
	 * looping back to the start of the sequence when the end is passed.
	 */
	if (Class->AnimLow || Class->AnimHigh) {
		AnimRate--;
		if (AnimRate == 0) {
			AnimRate = Class->AnimRate;
			AnimFrame++;
			if (AnimFrame > Class->AnimHigh) {
				AnimFrame = Class->AnimLow;
			}
		}
	}

	/*
	**	Move the projectile forward according to its speed
	**	and direction.
	*/
	Coord coord = PositionCoord;

	/*
	 * Projectiles that leave a trail behind them drop one puff of it every
	 * configured number of frames.
	 */
	if (Class->Trailer != NULL) {
		if (Class->ScaledSpawnDelay) {
			if (Frame % Class->ScaledSpawnDelay == 0) {
				new AnimClass(Class->Trailer, coord, 1, 1);
			}
		} else if (Frame % Class->SpawnDelay == 0) {
			new AnimClass(Class->Trailer, coord, 1, 1);
		}
	}

	ImpactType impact = IMPACT_NONE;
	TVelocity3D<double> velocity;

	/*
	 * There are two flight models. A projectile that has a rate of turn is
	 * flown by the missile autopilot, which steers it toward its target and
	 * over any terrain in the way. Everything else merely follows the arc it
	 * was launched along and bounces off whatever it lands on.
	 */
	if (Class->ROT > 0) {
		int max_speed = MaxSpeed;
		double speed = Velocity.Speed();

		/*
		 * The projectile stays in its launch phase until it has worked up to
		 * cruising speed. A fast projectile is given no launch phase at all.
		 */
		if (!Class->CourseLockDuration) {
			if (MaxSpeed >= 40 || speed + 0.5 >= max_speed) {
				IsLaunching = false;
			}
		} else if (CourseLockCounter < Class->CourseLockDuration && ++CourseLockCounter >= Class->CourseLockDuration) {
			IsLaunching = false;
		}

		int acceleration = Class->Acceleration;

		/*
		**	Homing projectiles constantly change facing to face toward the target but
		**	they only do so every other game frame (improves game speed and makes
		**	missiles not so deadly).
		*/
		if (IsLaunching && !Class->CourseLockDuration) {
			acceleration = (Frame % 2 == 0) ? 1 : 0;
		}

		/*
		 * Work the projectile's speed toward its maximum, accelerating while it
		 * is flying too slowly and coasting down while it is flying too fast.
		 */
		if (speed < max_speed) {
			speed += acceleration;
			if (speed >= max_speed) {
				speed = max_speed;
			}
			Velocity.Set_Speed(speed);
		} else if (speed > max_speed) {
			speed -= acceleration / 2;
			if (speed <= 0) {
				speed = 0;
			}
			Velocity.Set_Speed(speed);
		}

		TVelocity3D<double> velocity = Velocity;

		/*
		 * Fetch the point to home on. An object is aimed at through its target
		 * coordinate so that the projectile strikes the body of the victim
		 * rather than the ground it happens to be standing on.
		 */
		Coord target_coord = TarCom != NULL ? TarCom->As_Coord() : COORD_NONE;
		if (TarCom != NULL && (TarCom->AbstractFlags & ABSTRACT_OBJECT)) {
			target_coord = ((ObjectClass *)TarCom)->Target_Coord();
		}

		/*
		 * Swing the rate of turn above and below its nominal value over a
		 * fifteen frame cycle. This is what gives a missile its weaving flight
		 * path. The projectile's ID is mixed into the cycle so that missiles
		 * fired together do not all weave in unison.
		 */
		double & rotvar = Rule->MissileROTVar;
		double phase = ((Frame + Fetch_ID()) % TICKS_PER_SECOND) * (1.0 / TICKS_PER_SECOND);
		int rot = (fastmath::sin(phase * DEG_TO_RAD(360)) * rotvar + (rotvar + 1)) * Class->ROT;

		/*
		 * Allow a sharper turn over the last cell of the approach so that the
		 * projectile can still catch a target that dodges at the last moment.
		 */
		if (Center_Coord().Distance_To(target_coord) < CELL_LEPTON) {
			rot *= 1.5;
		}

		Coord old_coord = coord;

		/*
		 * A projectile chasing an aircraft flies straight at its quarry rather
		 * than hugging the terrain on the way, since there is no ground between
		 * the two of them that needs clearing.
		 */
		int is_aircraft = TarCom != NULL && TarCom->RTTI == RTTI_AIRCRAFT ? true : false;

		int distance = Projectile_Motion(coord, velocity, target_coord, DirType((Dir256)(IsLaunching == false ? rot : 0)), (bool &)is_aircraft, Class->IsAirburst, Class->IsVeryHigh, Class->Level); // RA2: the eighth argument (BulletTypeClass +0x26D) is pushed at 0x0045FD0E
		CellClass * cellptr = &Map[coord];
		TVelocity3D<double> new_velocity = velocity;
		Velocity = new_velocity;

		/*
		 * The projectile has arrived once it is within half a frame's travel of
		 * its target or it has flown into the ground. One that arrives while
		 * still airborne is snapped onto the target first so that the explosion
		 * is seen on the victim. An airburst projectile is left where it is,
		 * since it is meant to go off overhead.
		 */
		double half_speed = Velocity.Speed() * 0.5;
		if (distance <= half_speed || HeightAGL <= 0) {
			forced = true;
			impact = IMPACT_NORMAL;
			if (HeightAGL > 0 && !Class->IsAirburst && target_coord != COORD_NONE) {
				coord = target_coord;
			}
		}

		/*
		 * Keep a smoothed measure of how quickly the projectile is gaining on
		 * its target. The first sixty frames are merely accumulated; after that
		 * the measure decays toward the closure of the moment. Should it fall
		 * away to nearly nothing, then the projectile is circling its target
		 * instead of closing on it and is forced to detonate rather than fly
		 * forever. Airburst and very high projectiles are exempt, since they
		 * come down on their targets from above.
		 */
		if (target_coord == COORD_NONE && HeightAGL >= Rule->MissileSafetyAltitude) {
			forced = true;
			impact = IMPACT_NORMAL;
		}

		int delta = ::Distance(old_coord, target_coord) - ::Distance(coord, target_coord);

		if (!IsLaunching) {
			if (ClosureSamples < _limit) {
				ClosureSamples++;
				SmoothedClosure += delta;
			} else {
				SmoothedClosure = SmoothedClosure * ((4 * TICKS_PER_SECOND - 1.0) / (4 * TICKS_PER_SECOND)) + delta;
				if (SmoothedClosure >= 0 && SmoothedClosure < _limit) {
					if (!Class->IsAirburst && !Class->IsVeryHigh) {
						forced = true;
						impact = IMPACT_NORMAL;
					}
				}
			}
		}

		/*
		 * A projectile that crossed the plane of a bridge deck during this step,
		 * from either side, strikes the bridge instead of passing through it.
		 */
		if (impact == IMPACT_NONE && (cellptr->IsUnderBridge || Map[old_coord].IsUnderBridge)) {
			int bridge_height = Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT;
			if (coord.Z > bridge_height && old_coord.Z < bridge_height) {
				impact = IMPACT_NORMAL;
				coord.Z = bridge_height;
				forced = true;
			} else if (coord.Z < bridge_height && old_coord.Z > bridge_height) {
				impact = IMPACT_NORMAL;
				coord.Z = bridge_height;
				forced = true;
			}
		}
	} else {
		/*
		 * The position local briefly holds the current velocity: it seeds the
		 * two velocity locals and the slow-speed check before being reused as
		 * the floating point position for the flight step below.
		 */
		TVelocity3D<double> position = Velocity;
		TVelocity3D<double> new_velocity = position;
		velocity = TVelocity3D<double>(position.X, position.Y, position.Z);

		/*
		 * A projectile that is barely moving at all has nothing left to do but
		 * land.
		 */
		if (position.Speed() < 8) {
			impact = IMPACT_NORMAL;
		}
		position.X = coord.X;
		position.Y = coord.Y;
		position.Z = coord.Z;

		/*
		 * Floating projectiles fall at half gravity, which is what gives them
		 * their lazy drifting arc.
		 */
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
			Coord delta((int)step.X, (int)step.Y, (int)step.Z);
			Coord old_coord = coord;
			coord = coord + delta;
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

			/*
			 * Pull the projectile down by gravity and then step it forward along
			 * its arc.
			 */
			velocity.Z -= gravity;

			Point3D old_coord((int)position.X, (int)position.Y, (int)position.Z);
			position += velocity;

			Coord new_coord(position.X, position.Y, position.Z);
			int height = Map.Get_Height_GL(new_coord);
			int bridge_height = height + BRIDGE_LEPTON_HEIGHT;

			CellClass * cellptr = &Map[new_coord];

			/*
			 * See if the arc carried the projectile through the deck of a bridge,
			 * either dropping onto it from above or rising into it from below.
			 */
			bool fell_through_bridge = false;
			bool rose_through_bridge = false;
			if (cellptr->IsUnderBridge || Map[old_coord].IsUnderBridge) {
				if (new_coord.Z >= bridge_height) {
					if (old_coord.Z < bridge_height) {
						rose_through_bridge = true;
					}
				} else if (old_coord.Z >= bridge_height) {
					fell_through_bridge = true;
				}
			}

			/*
			 * Otherwise see if the projectile ran into a wall or a building on the
			 * way. The firer's own building, a dormant laser fence, a vehicle that
			 * happens to be deployed, and anything belonging to an ally are all
			 * passed through harmlessly.
			 */
			bool hit_obstacle = false;
			if (!fell_through_bridge && !rose_through_bridge) {
				if (position.Z >= height && position.Z - 150 < height) {
					BuildingClass * building = cellptr->Cell_Building();
					if (building != NULL || cellptr->Has_Wall_Or_Gate()) {
						hit_obstacle = true;
						if (building != NULL) {
							if (building == payback || building->Class->IsLaserFence && building->LaserFenceFrame >= 8) {
								hit_obstacle = false;
							}
							if (building->Considered_Vehicle()) {
								hit_obstacle = false;
							}
							if (payback != NULL && payback->House->Is_Ally(building)) {
								hit_obstacle = false;
							}
						}
					}
				}
			}

			/*
			 * The projectile bounces unless it is still clear of the ground and has
			 * struck nothing along the way.
			 */
			bool do_bounce = true;
			if (position.Z >= height) {
				if (!fell_through_bridge && !rose_through_bridge && !hit_obstacle) {
					do_bounce = false;
				}
			}

			if (do_bounce) {
				if (payback == NULL) {
					if (fell_through_bridge) {
						position.Z = bridge_height;
					} else if (rose_through_bridge) {
						bridge_height -= 20;
						position.Z = bridge_height;
					} else {
						bridge_height = height - 100;
						if (bridge_height < position.Z) {
							position.Z = height;
						}
					}
					Matrix3D slope_matrix;
					Matrix3D slope_matrix_inv;
					slope_matrix_inv = Matrix3D::Orthogonal_Inverse(
						slope_matrix = Get_Slope_Matrix(TacticalMap->Get_Cell_Ramp(new_coord)));
					Vector3 vec(velocity.X, -velocity.Y, velocity.Z);
					vec = slope_matrix_inv.Rotate_Vector(vec);
					vec *= elasticity;
					vec.Z = -vec.Z;
					vec = slope_matrix.Rotate_Vector(vec);
					velocity.X = vec.X;
					velocity.Y = -vec.Y;
					velocity.Z = vec.Z;
				} else {
					if (fell_through_bridge) {
						position.Z = bridge_height;
					} else if (rose_through_bridge) {
						bridge_height -= 20;
						position.Z = bridge_height;
					} else {
						goto bounced;
					}
				}
				impact = IMPACT_NORMAL;
				forced = true;
			}
		bounced:
			coord = Coord((int)position.X, (int)position.Y, (int)position.Z);
		}

		Cell here = coord.As_Cell();
		Cell there = TargetCoords.As_Cell();
		if (here == there && !Class->Vertical && HeightAGL < LEVEL_LEPTON_H * 2) {
			impact = IMPACT_NORMAL;
			forced = true;
			collided = true;
		} else {
			CellClass & cell = Map[here];
			CellClass & target_cell = Map[there];
			BuildingClass * building = cell.Cell_Building();
			BuildingClass * target_building = target_cell.Cell_Building();
			if (building != NULL && !Class->Vertical && building == target_building && HeightAGL < LEVEL_LEPTON_H * 2) {
				impact = IMPACT_NORMAL;
				forced = true;
				collided = true;
			} else {
				TechnoClass * techno = Map[coord].Cell_Techno();
				bool techno_valid = techno != NULL;
				bool payback_valid = Payback != NULL;
				bool is_owner = payback_valid && techno == Payback;
				bool ally = techno_valid && payback_valid && Payback->House->Is_Ally(techno);
				bool close = techno_valid && coord.Distance_To(techno->PositionCoord) < CELL_LEPTON / 2;
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
					new_velocity.X = velocity.X;
					new_velocity.Y = velocity.Y;
					if (!Class->Vertical) {
						Velocity = TVelocity3D<double>(new_velocity.X, new_velocity.Y, velocity.Z);
					}
					if (Velocity.Speed() < 10 && HeightAGL < 10) {
						impact = IMPACT_NORMAL;
						forced = true;
					}
				}
			}
		}
	}

	switch (impact) {
		/*
		**	When a projectile reaches the edge of the world, it
		**	vanishes from existence -- presumed to explode off
		**	map.
		*/
		case IMPACT_EDGE:
			Mark();
			Delete_Me();
			break;

		default:
		case IMPACT_NONE:

		/*
		**	The projectile has moved. Check its fuse. If detonation
		**	is signaled, then do so. Otherwise, just move.
		*/
		case IMPACT_NORMAL: {
			Mark();

			/*
			 * A fueled projectile carries only so much range with it. Once the
			 * fuel is spent, it detonates wherever it happens to be.
			 */
			Set_Coord(coord);

			/*
			 * A projectile that flies into an active firestorm wall is consumed
			 * by it. The wall lets its owner's own fire through, as it does any
			 * projectile built to ignore firestorms.
			 */
			BuildingClass * building = Map[coord].Cell_Building();
			if (building != NULL && building->Class->IsFirestormWall && building->House->FirestormDefenseActivated) {
				if (Payback == NULL || building->House != Payback->House) {
					if (Class_Of() != NULL && !Class_Of()->IsIgnoresFirestorm) {
						building->Crossing_Firestorm(this, false);
						Delete_Me();
						LastMapCoords = coord.As_Cell();
						return;
					}
				}
			}

			/*
			**	See if the bullet should be forced to explode now in spite of what
			**	the fuse would otherwise indicate. Maybe the bullet hit a wall?
			*/
			if (!forced) {
				Coord cur_coord = PositionCoord;
				forced = Is_Forced_To_Explode(cur_coord);
				Set_Coord(cur_coord);
			}

			/*
			**	If the bullet is not to explode, then perform normal flight
			**	maintenance (usually nothing). Otherwise, explode and then
			**	delete the bullet.
			*/
			if (forced && HeightAGL < 0) {
				Set_Height_AGL(0);
			}
			FuseResultType fuse = (Is_Homing() || Class->IsFueled) ? Fuse.Fuse_Checkup(coord) : FUSE_WAIT;
			if (Payback && Payback->TClass->JumpJet && fuse == FUSE_EXPLODE_FAR) {
				fuse = FUSE_EXPLODE_CLOSE;
			}
			if (!forced && (Class->IsDropping || fuse == FUSE_WAIT)) {
				/*
				**	Certain projectiles lose strength when they travel.
				*/
				if (Class->IsDegenerate && Strength > 5) {
					Strength--;
				}

			} else {
				/*
				 * Nudge the detonation onto the target itself when the
				 * projectile went off close by, so that the explosion is seen
				 * to strike the victim rather than the air alongside it. A
				 * collision is judged three times as generously as a mere fuse
				 * expiry, and an airburst projectile is always left where it
				 * went off.
				 */
				if (TarCom != NULL) {
					if ((fuse == FUSE_EXPLODE_CLOSE || collided) && !Class->IsAirburst && !Class->IsInaccurate) {
						Coord midpoint = coord;
						Coord target_coord = TarCom->As_Coord();
						midpoint.Z = (midpoint.Z + target_coord.Z) / 2;
						int target_distance = midpoint.Distance_To(target_coord);
						if (collided) {
							target_distance /= 3;
						}

						if (fuse == FUSE_EXPLODE_CLOSE || target_distance <= MAX(CELL_LEPTON / 2, Velocity.Speed() * 2)) {
							Set_Coord(TarCom->Center_Coord());
						}
					}
				}
				// RA2: Start the flash and radar event before deferring damage to NUKEBALL.
				if (*Warhead == "NUKE") {
					if (HeightAGL < 0) Set_Height_AGL(0);
					IonStormClass::Start_Nuke_Flash();
					Submit_Radar_Event(RADAREVENT_SUPERWEAPON_ACTIVATED, Get_Cell());
					AnimType index = AnimTypeClass::From_Name("NUKEBALL");
					AnimTypeClass * type;
					if (index != ANIM_NONE && (type = AnimTypes[index]) != NULL) {
						NextAnim = new AnimClass(type, PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL | SHAPE_ZGRAD), Get_Explosion_Z(PositionCoord));
						SpawnNextAnim = true;
						AnimPtrTracker.Add(this);
						break;
					}
				}
				Bullet_Explodes(forced);
				Delete_Me();
			}
			break;
		}
	}

	LastMapCoords = coord.As_Cell();
}
#endif


/***********************************************************************************************
 * BulletClass::Shape_Number -- Fetches the shape number for the bullet object.                *
 *                                                                                             *
 *    Use this routine to fetch a shape number to use for this bullet object.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number to use when drawing this bullet.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BulletClass::Shape_Number(void) const
{
	int shapenum = 0;

	if (!Class->IsFaceless) {
		shapenum = UnitClass::BodyShape[DirType(fastmath::atan2(-Velocity.Y, Velocity.X)).As_Dir32()];
	}

	if (Class->AnimLow || Class->AnimHigh) {
		shapenum = AnimFrame;
	}

	return(shapenum);
}


/***********************************************************************************************
 * BulletClass::Draw_It -- Displays the bullet at location specified.                          *
 *                                                                                             *
 *    This routine displays the bullet visual at the location specified.                       *
 *                                                                                             *
 * INPUT:   x,y   -- The center coordinate to render the bullet at.                            *
 *                                                                                             *
 *          window   -- The window to clip to.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   06/27/1994 JLB : Takes a window clipping parameter.                                       *
 *   01/08/1995 JLB : Handles translucent colors if necessary.                                 *
 *=============================================================================================*/
void BulletClass::Draw_It(Point2D const & point, Rect const & cliprect) const
{
	if (Scen->Special.IsFogOfWar && Map.Is_Fogged(Get_Coord())) return; // RA2: Draw_It at 0x4610D0 matches only so

	if (Class->IsInvisible) return;

	if (SpawnNextAnim) return;

	if (Class->IsVoxel) {
		Matrix3D matrix(true);
		matrix.Rotate_Z(Velocity.Get_Yaw().As_Radian32());
		matrix.Rotate_Y(-Velocity.Get_Pitch().As_Radian32());
		Draw_Voxel(Class->Voxel, Get_Isometric_View_Matrix() * matrix, point, cliprect, 0, SHAPE_ALPHA, NORMAL_LIGHT);

	} else {
		ShapeSet const * shapeptr = (ShapeSet const *)Get_Image_Data();
		if (shapeptr == NULL) return;

		int shapenum = Shape_Number();

		int height_agl = HeightAGL;
		int height_gl = Map.Get_Height_GL(Get_Coord());

		if (!IsOnBridge && Map[Get_Coord()].IsUnderBridge && height_agl >= BRIDGE_LEPTON_HEIGHT) {
			height_agl -= BRIDGE_LEPTON_HEIGHT;
			height_gl += BRIDGE_LEPTON_HEIGHT;
		}

		if (height_agl > 0 && Class->IsShadow) {
			Point2D shadowpoint = point + Point2D(0, TacticalMap->Z_Lepton_To_Pixel(height_agl));
			Draw_Shape(*LogicalSurface, *NormalDrawer, shapeptr, shapenum, shadowpoint, cliprect, ShapeFlags_Type(SHAPE_DARKEN|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_ZGRAD), 0, -10 - TacticalMap->Z_Lepton_To_Pixel(height_gl));
		}

		ConvertClass * drawer = NormalDrawer;
		if (Class->IsAnimPalette) {
			drawer = AnimDrawer;
		} else if (Class->FirersPalette) {
			if (InheritedColor != -1) {
				drawer = ColorSchemes[InheritedColor]->Converter;
			} else {
				drawer = ColorSchemes[PlayerPtr->Scheme]->Converter;
			}
		}
		Draw_Shape(*LogicalSurface, *drawer, shapeptr, shapenum, point, cliprect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_ALPHA|SHAPE_ZGRAD), 0, -30 - TacticalMap->Z_Lepton_To_Pixel(Height));
	}
}


/// <summary>
/// RA2: retargets the bullet at the cell under its target (no target while the scenario is set up, for
/// a target in the air, or at the zero cell).
/// </summary>
void BulletClass::LoseTarget(void) // RA2: at 0x461470
{
	Cell cell(TarCom->Center_Coord());
	if (!ScenarioInit && !TarCom->In_Air() && cell != CELL_NONE) {
		TarCom = &Map[cell];
	} else {
		TarCom = NULL;
	}
}






/***********************************************************************************************
 * BulletClass::Detach -- Removes specified target from this bullet's targeting system.        *
 *                                                                                             *
 *    When an object is removed from the game system, it must be removed all targeting and     *
 *    tracking systems as well. This routine is used to remove the specified object from the   *
 *    bullet. If the object isn't part of this bullet's tracking system, then no action is     *
 *    performed.                                                                               *
 *                                                                                             *
 * INPUT:   target   -- The target to remove from this tracking system.                        *
 *                                                                                             *
 *          all      -- Is the target going away for good as opposed to just cloaking/hiding?  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletClass::Detach(AbstractClass const * target, bool all)
{
	BASECLASS::Detach(target, all);

	if (Payback == target) {
		Payback = NULL;
	}
	if (TarCom == target) {
		LoseTarget(); // RA2: retargets at the cell under the target instead of dropping the target (inlined LoseTarget, 0x461549)
	}
	if (Class == target) {
		Class = NULL;
	}
	if (WeaponType == target) { // RA2: also the weapon type (+0x118)
		WeaponType = NULL;
	}
	if (NextAnim == target) { // RA2: also the anim (+0x13C)
		NextAnim = NULL;
	}
	if (all && target->Fetch_RTTI() == RTTI_BULLET) { // RA2: a Scalable bullet that goes leaves the list of those in flight
		BulletClass * bullet = (BulletClass *)target;
		if (bullet->Class->Scalable) {
			for (int index = 0; index < ScalableBullets.Count(); index++) {
				if (bullet == ScalableBullets[index]) {
					ScalableBullets.Delete(bullet);
					return;
				}
			}
		}
	}
}



/***********************************************************************************************
 * BulletClass::Unlimbo -- Transitions a bullet object into the game render/logic system.      *
 *                                                                                             *
 *    This routine is used to take a bullet object that is in limbo and transition it to the   *
 *    game system. A bullet object so transitioned, will be drawn and logic processing         *
 *    performed. In effect, it comes into existence.                                           *
 *                                                                                             *
 * INPUT:   coord -- The location where the bullet object is to appear.                        *
 *                                                                                             *
 *          dir   -- The initial facing for the bullet object.                                 *
 *                                                                                             *
 * OUTPUT:  bool; Was the unlimbo successful?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
inline float Projectile_Launch_Distance(Coord const & source, Coord const & target)
{
    Vector3 delta((float)(source.X-target.X), (float)(target.Y-source.Y), (float)(source.Z-target.Z));
    float distance=fastmath::sqrt((double)delta.X*delta.X+(double)delta.Y*delta.Y+(double)delta.Z*delta.Z);
    return distance;
}

bool BulletClass::Unlimbo(Coord const &source, TVelocity3D<double> const &velocity)
{
    if (ObjectClass::Unlimbo(source, DIR_N)) {
        Velocity = velocity;
        SourceCoords = source;
        LastMapCoords = SourceCoords.As_Cell();
        Map.Remove(this);
        Coord target = TarCom->As_Coord();
        TargetCoords = target;
        if (Class->FlakScatter && Class->IsInvisible) {
            float distance=Projectile_Launch_Distance(source,target);
            int maximum=Rule->BallisticScatter*2;
            int scatter=Scen->RandomNumber(0,maximum);
            int product = (int)((unsigned int)scatter * (unsigned int)(int)distance); // original32-bit IMUL before signed IDIV
            int radius = product / WeaponType->Range;
            int random_direction = Scen->RandomNumber(0, 0x7ffffffe);
            double random_angle = random_direction * 4.656612877414201e-10;
            random_angle *= 6.283185307179586;
            short raw_direction = (short)(int)((random_angle - 1.5707963267948966) * -10430.060040584269);
            int z=target.Z;
            double angle = ((int)raw_direction - 0x3fff) * -9.587672516830327e-05;
            int y = (int)((double)target.Y - fastmath::sin(angle) * (double)radius);
            int x = (int)(fastmath::cos(angle) * (double)radius + target.X);
            target = Coord(x, y, z);
        }
        if (Class->IsInvisible) {
            HouseClass *house = Payback != 0 ? Payback->House : 0;
            Coord collision = Map.Firestorm_On_Path(source, target, house);
            if (collision != COORD_NONE) {
                collision.Z = Map.Get_Height_GL(collision);
                Set_Coord(collision);
                BuildingClass *building = static_cast<BuildingClass *>(Map[collision].Cell_Building());
                building->Crossing_Firestorm(this, false);
                Delete_Me();
            } else {
                CellClass *blocking = TrajectoryHelper::FindFirstObstacle(source, target, Class, house);
                if (blocking != 0) Set_Coord(blocking->Center_Coord());
                else Set_Coord(target);
                Set_Coord(target);
                MaxSpeed = 0;
                Velocity.Set_Speed(0.0);
            }
        }
		int arm = TarCom != 0 && TarCom->Fetch_RTTI() == RTTI_AIRCRAFT ? 0 : Class->Arming;
        Fuse.Arm_Fuse(Coord(PositionCoord), target, arm, 0x7fffffff);
        if (Class->ROT > 0) {
            Velocity.Set_Speed(1.0);
        }
        if (IsActive) Map.Submit(this);
        return true;
    }
    return false;
}



/***********************************************************************************************
 * BulletClass::In_Which_Layer -- Fetches the layer that the bullet resides in.                *
 *                                                                                             *
 *    This examines the bullet to determine what rendering layer it should be in. The          *
 *    normal logic applies unless this is a torpedo. A torpedo is always in the surface        *
 *    layer.                                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the render layer that this bullet should reside in.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
LayerType BulletClass::In_Which_Layer(void) const
{
	return(Class->Flat ? LAYER_SURFACE : LAYER_AIR); // RA2: In_Which_Layer at 0x461BF0 matches only so
}


/***********************************************************************************************
 * BulletClass::Is_Forced_To_Explode -- Checks if bullet should explode NOW.                   *
 *                                                                                             *
 *    This routine will examine the bullet and where it is travelling in order to determine    *
 *    if it should prematurely explode. Typical of this would be when a bullet hits a wall     *
 *    or a torpedo hits a ship -- regardless of where the projectile was originally aimed.     *
 *                                                                                             *
 * INPUT:   coord -- The new coordinate to place the bullet at presuming it is forced to       *
 *                   explode and a modification of the bullet's coordinate is needed.          *
 *                   Otherwise, the coordinate is not modified.                                *
 *                                                                                             *
 * OUTPUT:  bool; Should the bullet explode now?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BulletClass::Is_Forced_To_Explode(Coord & coord) const
{
	coord = PositionCoord;
	CellClass * cellptr = &Map[coord];

	/*
	**	RA2: a bullet that cannot cross cliffs or walls explodes at the obstacle on its way from the
	**	source to the target cell (TrajectoryHelper::GetObstacle at 0x4BBAF0).
	*/
	if (Class->SubjectToCliffs || Class->SubjectToWalls) {
		CellClass * source = &Map[SourceCoords];
		CellClass * destination = &Map[TargetCoords];
		CellClass * previous = &Map[LastMapCoords];
		if (TrajectoryHelper::GetObstacle(source, destination, previous, coord, Class, Payback == NULL ? NULL : Payback->House)) {
			return(true);
		}
	}

	if (HeightAGL <= -LEVEL_LEPTON_H * 4) { // RA2: far below the ground
		return(true);
	}

	if (TarCom != NULL && Class->FlakScatter) { // RA2: a flak bullet that is lower than its target explodes
		Coord current = PositionCoord;
		if (current.Z < TarCom->As_Coord().Z && HeightAGL < 0) {
			return(true);
		}
	}

	if (Class->Level && cellptr->On_Ground()) { // RA2: a level bullet that reaches the ground
		return(true);
	}

	/*
	**	Bullets are generally more effective when they are fired at aircraft.
	*/
	if (Class->IsAntiAircraft && TarCom != NULL && TarCom->In_Air() && Distance(TarCom) < CELL_LEPTON / 2) { // RA2: no jumpjet test
		return(true);
	}

	return(false);
}



/// <summary>
/// RA2
/// </summary>
static inline int Coord_Dist(Coord const & a, Coord const & b)
{
	Coord c = a - b;
	return(c.Length());
}


/***********************************************************************************************
 * BulletClass::Bullet_Explodes -- Performs bullet explosion logic.                            *
 *                                                                                             *
 *    This handles the exploding bullet action. It will generate the animation and the         *
 *    damage as necessary.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The bullet should be deleted after this routine is called.                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletClass::Bullet_Explodes(bool forced)
{
	Coord coord = PositionCoord;
	TechnoClass * target = NULL; // RA2: Bullet_Explodes at 0x461DE0 matches only so
	if (TarCom != NULL && TarCom->In_Air()) {
		target = (TechnoClass *)TarCom;
	}

	if (!Class->IsInaccurate) {
		if (TarCom != NULL) {
			if (Coord_Dist(coord, TarCom->Center_Coord()) < 32 && !Class->IsAirburst && !Class->IsInaccurate) {
				coord = TarCom->Center_Coord();
			}
		}

		if (!Warhead->IsEMEffect && !Class->IsAirburst) {
			if (!forced && !Class->IsArcing && !Is_Homing() && Fuse.Fuse_Target() != COORD_NONE) {
				coord = Fuse.Fuse_Target();
			}

			if (target != NULL && target->In_Which_Layer() != LAYER_GROUND) {
				if (Distance(TarCom) < 128) {
					coord = target->Target_Coord();
				}
			} else if (TarCom != NULL && Distance(TarCom) < 42) {
				coord = TarCom->As_Coord();
				if (TarCom->Fetch_RTTI() == RTTI_BUILDING) {
					Point3D const & offset = ((BuildingClass *)TarCom)->Class->TargetCoordOffset;
					if (offset.X != 0 || offset.Y != 0 || offset.Z != 0) {
						coord = ((ObjectClass *)TarCom)->Target_Coord();
					}
				}
			}
		}
	}

	if (!Class->IsAirburst) {
		Coord newcoord = coord;
		for (int i = 0; i < Class->Cluster; i++) {
			Detonate(coord);
			if (!IsActive) {
				break;
			}
			coord = Coord_Scatter(newcoord, Random_Pick(CELL_LEPTON, CELL_LEPTON * 2));
		}
	} else {
		Detonate(coord);
	}
}


/// <summary>
/// RA2
/// </summary>
static inline ObjectClass * Object_Of(AbstractClass * target)
{
	return(target == NULL ? NULL : ((target->AbstractFlags & 2) ? (ObjectClass *)target : NULL));
}


/// <summary>
/// RA2
/// </summary>
static inline bool Is_Foot_Object(AbstractClass const * target)
{
	return(target != NULL && (target->AbstractFlags & 4));
}


/// <summary>
/// RA2
/// </summary>
static inline Vector3 Normalize_Dir(const Vector3 & vec)
{
	float len = (float)fastmath::sqrt(vec.Z*vec.Z + vec.Y*vec.Y + vec.X*vec.X);
	if (len != 0.0) {
		return(vec / len);
	}
	return(vec);
}


/// <summary>
/// Performs the projectile's damage effect at the location specified.
/// This routine applies whatever the warhead calls for -- an electromagnetic pulse, a
/// tiberium web, or ordinary blast damage -- and then spawns the impact animation and any
/// lighting flash. Cluster projectiles use this moment to choose their sub-targets and
/// launch their bomblets.
/// </summary>
/// <param name="coord">The location that the projectile detonates at.</param>
void BulletClass::Detonate(Coord const &position)
{
    int damage_result = 1;
    bool special_impact = false;
    if (Warhead->ShakeXlo != 0 || Warhead->ShakeXhi != 0)
        Map.ScreenX = Sim_Random_Pick(Warhead->ShakeXlo, Warhead->ShakeXhi);
    if (Warhead->ShakeYlo != 0 || Warhead->ShakeYhi != 0)
        Map.ScreenY = Sim_Random_Pick(Warhead->ShakeYlo, Warhead->ShakeYhi);
    if (WeaponType != 0) {
        int level = WeaponType->RadLevel;
        if (level > 0) {
            ApplyRadiationToCell(position.As_Cell(), (int)WeaponType->WarheadPtr->SpreadFactor, level);
        }
    }
    if (Warhead->MindControl) {
        if (Payback != 0 && Payback->CaptureManager != 0) {
            ObjectClass *object = TarCom == 0 ? 0 :
                (TarCom->AbstractFlags & ABSTRACT_OBJECT ? static_cast<ObjectClass *>(TarCom) : 0);
			bool target_house = TarCom != 0 && (TarCom->AbstractFlags & ABSTRACT_TECHNO) &&
                static_cast<TechnoClass *>(TarCom)->House->Is_Player_Control();
            if (object != 0 && object->IsActive && object->Tag != 0) {
				object->Tag->Spring(TEVENT_ATTACKED, object, CELL_NONE, false, Payback);
				object->Tag->Spring(TEVENT_ATTACKED_BY, object, CELL_NONE, false, Payback);
            }
            if (static_cast<CaptureManagerClass *>(Payback->CaptureManager)->Capture(static_cast<TechnoClass *>(TarCom))
                && Rule->YuriMindControlSound != -1
                && (Payback->House->Is_Player_Control() || target_house)) {
				VocClass::PlayAt(Rule->YuriMindControlSound, object->PositionCoord, 0);
            }
        }
    } else if (Warhead->IvanBomb) {
        TechnoClass *target = TarCom == 0 ? 0 :
            (TarCom->AbstractFlags & ABSTRACT_TECHNO ? static_cast<TechnoClass *>(TarCom) : 0);
        BombList.Plant(Payback, target);
    } else if (Warhead->ElectricAssault) {
		if (TarCom != 0 && TarCom->Fetch_RTTI() == RTTI_BUILDING && Payback != 0 && Payback->Fetch_RTTI() == RTTI_INFANTRY)
            static_cast<BuildingClass *>(TarCom)->Rotate_Overpowerer(static_cast<InfantryClass *>(Payback));
    } else if (Warhead->Parasite) {
        if (Payback != 0) {
            FootClass *target = TarCom == 0 ? 0 :
                (TarCom->AbstractFlags & ABSTRACT_FOOT ? static_cast<FootClass *>(TarCom) : 0);
            static_cast<FootClass *>(Payback)->ParasiteImUsing->TryInfect(target);
        }
    } else if (Warhead->Temporal) {
        if (Payback != 0 && TarCom != 0) {
			TechnoClass * target = As_Techno(TarCom); // RA2: ra2-decomp's twin statement
            Payback->TemporalImUsing->Fire(target);
        }
    } else if (Warhead->BombDisarm) {
		if (TarCom != 0 && (TarCom->AbstractFlags & ABSTRACT_OBJECT)) {
            ObjectClass *object = static_cast<ObjectClass *>(TarCom);
            if (object->AttachedBomb != 0) object->AttachedBomb->Disarm();
        }
    } else if (Warhead->MakesDisguise) {
        if (Payback != 0) Payback->DisguiseAs(static_cast<TechnoClass *>(TarCom));
    } else if (Warhead->NukeMaker) {
        NukeMaker();
    } else {
        if (Class->ShrapnelWeapon != 0) Shrapnel();
        int damage = DamageMultiplier * Strength;
        damage_result = Explosion_Damage(position, damage >> 8, Payback, Warhead, true);
        if (!IsActive) return;
        if (damage_result == 2) special_impact = true;
    }
    Coord impact = position;
    if (Class->IsInvisible) impact = Coord_Scatter(impact, 32, false);
    int land = -1;
    if (Get_Height_AGL() < LEVEL_LEPTON_H * 2) {
		land = Map[Get_Coord()].Land_Type();
		if (land == LAND_WATER && damage_result == 0) {
			ObjectClass * occupier = Map[Get_Coord()].Cell_Occupier();
			if (occupier != 0 && occupier->Fetch_RTTI() == RTTI_UNIT) {
                UnitTypeClass const *type = static_cast<UnitClass *>(occupier)->Class;
                if (type->Naval && !type->Underwater) land = -1;
            }
        }
    }
	AnimTypeClass const * animation = Combat_Anim(Strength, Warhead, (LandType)land, PositionCoord);
    if (IsBright && !special_impact) {
        unsigned flags = 0;
        if (Warhead->CLDisableRed) flags = 2;
        if (Warhead->CLDisableGreen) flags |= 4;
        if (Warhead->CLDisableBlue) flags |= 8;
        Combat_Lighting(impact, Strength, Warhead, 1, flags);
    }
    if (special_impact) {
        AnimTypeClass const *nullify = Rule->WeaponNullifyAnim;
        if (nullify != 0)
			new AnimClass(nullify, impact, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL | SHAPE_ZGRAD), Get_Explosion_Z(impact));
    } else {
        if (animation != 0) {
			AnimClass * created = new AnimClass(animation, impact, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL | SHAPE_ZGRAD), Get_Explosion_Z(impact));
            if (created == 0 && Warhead == Rule->NukeWarhead) {
                int house_type = -1;
                if (Payback != 0) house_type = Payback->House->Class->House;
                AnimClass::Do_Atom_Damage((HousesType)house_type, impact.As_Cell());
            }
        }
        if (Class->IsAirburst) {
            WeaponTypeClass *weapon = Class->AirburstWeapon;
            BulletTypeClass const *projectile = weapon->Bullet;
            unsigned facing = 0;
            CellClass *center = static_cast<CellClass *>(Get_Cell_Ptr());
            int remaining = 8;
            do {
                CellClass &neighbor = center->Adjacent_Cell((FacingType)facing);
                facing = (facing + 1) & 7;
                BulletClass *fragment = projectile->Create_Bullet(&neighbor, Payback, weapon->Attack, weapon->WarheadPtr, 50, false);
                if (fragment != 0) {
					TVelocity3D<double> velocity(DirType((Dir256)Random_Pick(DIR_MIN, DIR_NE)), DirType(DIR_S), weapon->MaxSpeed / 10); // RA2: ra2-decomp's twin velocity
					fragment->Unlimbo(PositionCoord, velocity);
                }
            } while (--remaining != 0);
            BulletClass *fragment = projectile->Create_Bullet(center, Payback, weapon->Attack, weapon->WarheadPtr, 50, false);
            if (fragment != 0) {
				TVelocity3D<double> velocity(DirType((Dir256)Random_Pick(DIR_MIN, DIR_NE)), DirType(DIR_S), weapon->MaxSpeed / 10);
				fragment->Unlimbo(PositionCoord, velocity);
            }
        }
    }
}


/// <summary>
/// RA2: the type's shrapnel: up to ShrapnelCount fragments of its ShrapnelWeapon at the enemies around the
/// impact, ring by ring within the weapon's range, and the rest at random cells nearby (a negative count:
/// fewer the farther the impact is from the firer). Ported from meastes 0x462C50.
/// </summary>
void BulletClass::Shrapnel(void) // RA2: at 0x462C50
{
	int configured = Class->ShrapnelCount;
	int count;
	if (configured < 0) {
		if (Payback == NULL) {
			count = 3;
		} else {
			count = -configured - ::Distance(Payback->Center_Coord(), PositionCoord) / CELL_LEPTON;
			if (count <= 0) {
				return;
			}
		}
	} else {
		count = configured;
	}

	if (Get_Cell_Ptr()->Cell_Occupier() == NULL ||
		Get_Cell_Ptr()->Cell_Occupier()->Fetch_RTTI() == RTTI_BUILDING) {
		return;
	}

	WeaponTypeClass * weapon = Class->ShrapnelWeapon;
	BulletTypeClass const * type = weapon->Bullet;
	Cell center = Get_Cell();
	int range = weapon->Range / CELL_LEPTON;
	int fired = 0;
	TVelocity3D<double> velocity;
	for (int radius = 1; radius <= range; radius++) {
		int end = MapClass::RadiusCount[radius];
		for (int index = MapClass::RadiusCount[radius - 1]; index < end; index++) {
			CellClass * cellptr = &Map[center + MapClass::RadiusOffset[index]];
			ObjectClass * object = cellptr->Cell_Occupier();
			if (object != NULL && object != Payback && Payback != NULL &&
				!Payback->House->Is_Ally(object)) {
				BulletClass * bullet = type->Create_Bullet(object, Payback,
					weapon->Attack, weapon->WarheadPtr, weapon->MaxSpeed, weapon->IsBright);
				if (bullet != NULL) {
					velocity.Set(0, 0, 0);
					velocity.Set_Yaw(::Direction(PositionCoord, object->Center_Coord()));
					velocity.Set_Pitch(DirType(DIR_NE));
					velocity.Set_Speed(weapon->MaxSpeed);
					bullet->Unlimbo(PositionCoord, velocity);
					Coord source;
					source = PositionCoord;
					if (weapon->IsLaser && Payback != NULL) {
						Payback->Laser_Zap(object, 0, weapon, source);
					}
					if (weapon->IsElectricBolt && Payback != NULL) {
						Payback->Electric_Zap(object, weapon, source);
					}
				}
				fired++;
				if (fired == count) {
					return;
				}
			}
		}
	}

	for (; fired < count; fired++) {
		Cell cell;
		cell = Get_Cell();
		cell.X += Random_Pick(-2, 2);
		cell.Y += Random_Pick(-2, 2);
		BulletClass * bullet = type->Create_Bullet(&Map[cell], Payback,
			weapon->Attack, weapon->WarheadPtr, weapon->MaxSpeed, weapon->IsBright);
		if (bullet != NULL) {
			velocity.Set(0, 0, 0);
			CellClass & destination = Map[cell];
			velocity.Set_Yaw(::Direction(PositionCoord, destination.Center_Coord()));
			velocity.Set_Speed(weapon->MaxSpeed);
			velocity.Set_Pitch(DirType(DIR_NE));
			bullet->Unlimbo(PositionCoord, velocity);
			Coord source;
			source = PositionCoord;
			if (weapon->IsLaser && Payback != NULL) {
				Payback->Laser_Zap(&Map[cell], 0, weapon, source);
			}
			if (weapon->IsElectricBolt && Payback != NULL) {
				Payback->Electric_Zap(&Map[cell], weapon, source);
			}
		}
	}
}



/// <summary>
/// RA2: leaves radiation in the cell: a new radiation site there, or more for the one it has.
/// </summary>
void BulletClass::ApplyRadiationToCell(Cell cell, int radius, int amount) // RA2: at 0x463720
{
	CellClass * cptr = &Map[cell];
	RadSiteClass * site = cptr->Get_Rad_Site();
	if (site == NULL) {
		site = new RadSiteClass();
		site->SetBaseCell(&cell);
		site->SetSpread(radius);
		site->SetRadLevel(amount);
		site->Activate();
		cptr->Set_Rad_Site(site);
	} else {
		site->Add(amount);
	}
}





/// <summary>
/// Reads this projectile back in from a save game stream.
/// This routine restores the virtual table and then swizzles the saved pointers so that they
/// refer to the objects they pointed at when the game was saved.
/// </summary>
/// <returns>Returns with S_OK if the projectile was loaded successfully.</returns>
HRESULT STDMETHODCALLTYPE BulletClass::Load(IStream * stream)
{
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		new (this) BulletClass(NoInitClass());

		if (NextAnim) { // RA2: a bullet with an anim is tracked again in AnimPtrTracker (inlined Add at 0x463847)
			AnimPtrTracker.Add(this);
		}
		Swizzle_Pointer(&Class);
		Swizzle_Pointer(&Payback);
		Swizzle_Pointer(&Warhead);
		Swizzle_Pointer(&TarCom);
		Swizzle_Pointer(&WeaponType); // RA2: also the weapon type (+0x118) ...
		Swizzle_Pointer(&NextAnim); // RA2: ... and the anim (+0x13C)
	}
	return(result);
}



/// <summary>
/// Writes this projectile out to a save game stream.
/// </summary>
/// <param name="cleardirty">Should the object be marked as clean once it has been written?</param>
/// <returns>Returns with S_OK if the projectile was saved successfully.</returns>
HRESULT STDMETHODCALLTYPE BulletClass::Save(IStream * stream, BOOL cleardirty)
{
	return(BASECLASS::Save(stream, cleardirty));
}


/// <summary>
/// Takes out a reference on this projectile.
/// This is the IUnknown implementation used by the COM machinery that owns projectiles.
/// </summary>
/// <returns>Returns with the number of references now outstanding.</returns>
ULONG STDMETHODCALLTYPE BulletClass::AddRef(void)
{
	COMRefCount++;
	return(InterlockedIncrement(&RefCount));
}


/// <summary>
/// Drops a reference to this projectile.
/// This is the IUnknown implementation. The projectile deletes itself when the last
/// reference to it is released.
/// </summary>
/// <returns>Returns with the number of references still outstanding.</returns>
ULONG STDMETHODCALLTYPE BulletClass::Release(void)
{
	COMRefCount--;
	ULONG count = InterlockedDecrement(&RefCount);
	if (count == 0) {
		delete this;
	}
	return(count);
}


/// <summary>
/// Can this projectile steer toward its target?
/// The flight logic calls this routine to decide whether the projectile should be turned
/// toward its target each game frame or simply left to follow its launch trajectory.
/// </summary>
/// <returns>bool; Is this projectile a homing type?</returns>
bool BulletClass::Is_Homing(void) const
{
	if (Class->ROT > 0) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Creates a projectile and fills in the data for the shot.
/// This routine is used by the weapon firing code in place of a bare new -- projectiles are
/// COM objects, so the instance must come from the class factory. The projectile is inert
/// until it is unlimboed with a starting position and velocity.
/// </summary>
/// <param name="payback">The object that fired the shot. It receives credit for any kill.</param>
/// <param name="strength">The damage the projectile will inflict when it detonates.</param>
/// <param name="bright">Should the impact throw off a lighting flash?</param>
/// <returns>Returns with a pointer to the projectile created, or NULL if one could not be
/// made.</returns>
BulletClass * __fastcall BulletTypeClass::Create_Bullet(AbstractClass * target, TechnoClass * payback, int strength, WarheadTypeClass const * warhead, int speed, bool bright) const // RA2: a member, no range, at 0x463990
{
	LPVOID unk = NULL;
	if (FAILED(CoCreateInstance(CLSID_BulletClass, NULL, CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER|CLSCTX_LOCAL_SERVER, IID_IUnknown, &unk))) {
		return(NULL);
	}

	BulletClass * bullet = (BulletClass *)unk;
	bullet->Set_Bullet_Data(this, target, payback, strength, warhead, speed, bright);
	return(bullet);
}


/// <summary>
/// Draws the voxel model for this projectile.
/// This routine poses every layer of the model, renders it through the voxel draw system,
/// and then blits the result to the logical surface in the projectile's own color scheme.
/// </summary>
/// <param name="voxeldata">The voxel and motion libraries to draw the projectile with.</param>
/// <param name="cliprect">The clipping rectangle to draw within.</param>
/// <param name="frame">The motion library frame to pose the model with.</param>
void BulletClass::Draw_Voxel(VoxelDataStruct const & voxeldata, Matrix3D const & transform, Point2D const & drawpoint, Rect const & cliprect, int frame, ShapeFlags_Type flags, int brightness) const
{
	VoxelLibrary * voxlib = voxeldata.VoxLib;
	MotionLibrary * motlib = voxeldata.MotLib;

	VoxelDrawSystem::Precalculate_Light(voxlib, 0, 0, transform, VoxelLightSource);
	VoxelDrawSystem::Reset();

	for (unsigned int i = 0; i < voxlib->Get_Layer_Count(); i++) {
		Matrix3D matrix;
		/// Without a motion library the matrix is left uninitialized and used as it stands.
		if (motlib != NULL) {
			Matrix3D hva_transform = motlib->Get_Layer_Matrix(i, frame);
			matrix = transform * hva_transform;
		}
		VoxelDrawSystem::Prep_For_Object(voxlib, i, 0, VoxelCameraMatrix * matrix);
	}

	SurfaceRegion region = VoxelDrawSystem::Render();
	static int flat_height_offset = -65; // RA2: a Flat bullet is drawn at a fixed height offset, read from memory (0x7D11A4)
	int height_offset = 0;
	ShapeFlags_Type draw_flags = ShapeFlags_Type(flags & ~SHAPE_REMAP); // RA2: a local for the flags
	if (Class->Flat) {
		height_offset = flat_height_offset;
		draw_flags = ShapeFlags_Type(draw_flags | SHAPE_ZGRAD); // RA2: ... and with the z gradient
	}
	Blit_Block(*LogicalSurface, *ColorSchemes[Class->Color]->Converter, *VoxelDrawSystem::Get_Surface(), region.Bounds, region.Point + drawpoint, cliprect, NULL, ColorSchemes[Class->Color]->Converter->Blitter_From_Flags(draw_flags), height_offset, Class->Flat ? ZGRAD_GROUND : ZGRAD_90DEG, brightness, 0); // RA2: the full argument list
}



/// <summary>
/// RA2
/// </summary>
void BulletClass::Set_Weapon_Type(WeaponTypeClass * weapon)
{
	WeaponType = weapon;
}


/// <summary>
/// RA2
/// </summary>
WeaponTypeClass * BulletClass::Get_Weapon_Type(void) const // RA2: at 0x463BB0
{
	return(WeaponType);
}


/// <summary>
/// RA2: counts the bullet among the Scalable ones in flight; the more there are, the later the type spawns.
/// </summary>
void BulletClass::InitScalable(void) // RA2: at 0x463BC0
{
	ScalableBullets.Add(this);
	int extra = ScalableBullets.Count() - 5;
	if (extra < 0) {
		extra = 0;
	}
	Class->SetScaledSpawnDelay(Class->SpawnDelay + extra / 3);
}


/// <summary>
/// RA2: a NukeMaker warhead's payload: a NukePayload bullet dropped straight down on the target's cell.
/// </summary>
void BulletClass::NukeMaker(void) // RA2: at 0x463C50
{
	AbstractClass * target = TarCom;
	CellClass * cptr = &Map[target->Center_Coord().As_Cell()];
	WeaponTypeClass * weapon = Weapons[WeaponTypeClass::From_Name("NukePayload")];

	BulletTypeClass const * type = weapon->Bullet;
	WarheadTypeClass const * warhead = weapon->WarheadPtr;
	int speed = weapon->MaxSpeed;
	BulletClass * bullet = type->Create_Bullet(target, Payback, weapon->Attack, warhead, speed, true);
	if (bullet != NULL) {
		bullet->Limbo();
		bullet->WeaponType = weapon;
		Coord coord = cptr->Center_Coord();
		coord = coord + Coord(0, 0, WeaponType->Bullet->DetonationAltitude);
		double angle = DirType(DIR_N).As_Radian(); // RA2: straight down; Y is built first (sin before any cos), then X, where the TVelocity3D constructors compute X first
		double y = fastmath::sin(angle);
		y *= fastmath::cos(angle);
		y *= -1.0;
		TVelocity2D<double> planar(fastmath::cos(angle) * fastmath::cos(angle) * -1.0, y);
		TVelocity3D<double> velocity;
		static_cast<TVelocity2D<double> &>(velocity) = planar; // RA2: as above, X and Y copied together
		velocity.Z = fastmath::sin(angle) * -1.0;
		bullet->Unlimbo(coord, velocity);
	}
}


/// <summary>
/// Fetches the storage size of this object.
/// This routine is used by the save game machinery to know how many bytes of the object
/// must be written out.
/// </summary>
/// <returns>Returns with the number of bytes this projectile occupies.</returns>
int BulletClass::Fetch_Object_Size(void) const // RA2: no argument
{
	return(sizeof(*this));
}


/// <summary>
/// Fetches the run time type of this object.
/// </summary>
RTTIType BulletClass::Fetch_RTTI(void) const
{
	return(RTTI_BULLET);
}











/// <summary>
/// Fetches the class identifier for this projectile.
/// This is the IPersist implementation the save and load machinery uses to recognize which
/// kind of object it is about to read back from the stream.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT STDMETHODCALLTYPE BulletClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_BulletClass;
	return(S_OK);
}


/// <summary>
/// Assigns a new target for this projectile.
/// This routine is used when the launcher wants the projectile to chase something other than
/// what it was originally fired at. Only homing projectiles will act on the change.
/// </summary>
void BulletClass::Assign_Target(AbstractClass * target)
{
	TarCom = target;
}


/// <summary>
/// Fetches the type class object for this projectile.
/// </summary>
/// <returns>Returns with a pointer to the bullet type this projectile was created from.</returns>
ObjectTypeClass const * BulletClass::Class_Of(void) const
{
	return((ObjectTypeClass const *)Class);
}

