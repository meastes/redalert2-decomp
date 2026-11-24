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
**	RA2: the locomotor of the V3 rocket and the Dreadnought missile: the rocket waits, tilts up from its launcher,
**	climbs to its cruise altitude, flies toward its target (on a lazy curve or level) and dives onto it. TibSun has
**	no such file; the code is the executable's from 0x0063D8D0.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "rocket.h" // RA2: the includes this file needs
#include "fastmath.h" // RA2: the includes this file needs
#include "_map.h" // RA2: the includes this file needs
#include "_rules.h" // RA2: the includes this file needs
#include "aircraft.h" // RA2: the includes this file needs
#include "airctype.h" // RA2: the includes this file needs
#include "anim.h" // RA2: the includes this file needs
#include "animtype.h" // RA2: the includes this file needs
#include "cell.h" // RA2: the includes this file needs
#include "combat.h" // RA2: the includes this file needs
#include "foot.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "ilocos.h" // RA2: the includes this file needs
#include "inline.h" // RA2: the includes this file needs
#include "rules.h" // RA2: the includes this file needs
#include "tracker.h" // RA2: the includes this file needs
#include "veteran.h" // RA2: the includes this file needs
#include "voc.h" // RA2: the includes this file needs


char const * RocketTakeoffAnimName = "V3TAKOFF"; // RA2: 0x007EE804; the animation played where the rocket leaves its launcher
char const * RocketTrailAnimName = "V3TRAIL"; // RA2: 0x007EE808; the animation left behind the flying rocket


/// <summary>
/// Makes a rocket locomotor that has no destination yet and rests level.
/// </summary>
RocketLocomotionClass::RocketLocomotionClass(void) : // RA2: 0x0063DAC0
	BASECLASS(),
	MovingDestination(COORD_NONE),
	MissionState(0),
	CurrentSpeed(0.0),
	CurrentPitch(0.0f),
	CruiseDistance(0)
{
}


/// <summary>
/// Ends the rocket locomotor.
/// </summary>
RocketLocomotionClass::~RocketLocomotionClass(void) // RA2: 0x0063DB30
{
}


/// <returns>bool; Has the rocket been given a destination?</returns>
boolean STDMETHODCALLTYPE RocketLocomotionClass::Is_Moving(void) // RA2: 0x0063DB50
{
	return(MovingDestination != COORD_NONE);
}


/// <returns>bool; Is the rocket in the states of flight (climbing, cruising or diving)?</returns>
boolean STDMETHODCALLTYPE RocketLocomotionClass::Is_Moving_Now(void) // RA2: 0x0063DB90
{
	if (MissionState >= 3 && MissionState <= 5) {
		return(true);
	}
	return(false);
}


/// <returns>CoordStruct; The destination the rocket flies to.</returns>
CoordStruct STDMETHODCALLTYPE RocketLocomotionClass::Destination(void) // RA2: 0x0063DBB0
{
	return(MovingDestination);
}


/// <summary>
/// Finds the point a distance ahead of the rocket, along its facing and its pitch.
/// </summary>
Coord RocketLocomotionClass::Position_At(int distance) // RA2: 0x0063DBE0
{
	Coord pos = LinkedTo->PositionCoord;
	Coord result = Move_Coord(pos, LinkedTo->PrimaryFacing.Current(), fastmath::cos(CurrentPitch) * (double)distance);
	result.Z += fastmath::sin(CurrentPitch) * (double)distance;
	return(result);
}


/// <summary>
/// Explodes the rocket when its nose is down to the destination's height, or to the deck of the bridge above it.
/// </summary>
/// <returns>bool; Did the rocket explode?</returns>
bool RocketLocomotionClass::Reached_Target(RocketStruct const * data) // RA2: 0x0063DCF0
{
	int z = Position_At(data->BodyLength).Z;
	if (z <= MovingDestination.Z) {
		Detonate();
		return(true);
	}
	CellClass * cell = LinkedTo->Get_Cell_Ptr();
	if (cell != NULL) {
		if (cell->IsUnderBridge) {
			if (cell->Center_Coord().Z == MovingDestination.Z) {
				if (z <= MovingDestination.Z + BRIDGE_LEPTON_HEIGHT) {
					Detonate();
					return(true);
				}
			}
		}
	}
	return(false);
}


/// <summary>
/// Finds the pitch from the rocket to its destination: the arctangent of the height over the ground distance,
/// straight down when there is no ground distance.
/// </summary>
double RocketLocomotionClass::Pitch_To_Target(void) // RA2: 0x0063DE10
{
	Coord pos = LinkedTo->PositionCoord;
	Coord diff = MovingDestination - pos;
	Point2D ground(diff);
	float flat = fastmath::sqrt((double)ground.Dot_Product(ground));
	if (flat > 0.0) {
		return(fastmath::atan((double)(diff.Z / flat)));
	}
	return(-1.5707963267948966);
}


/// <summary>
/// Ground (x/y) distance between the point and the position. The take-off reading sums y*y + x*x, the lazy-curve
/// reading x*x + y*y: the executable's two copies load the components in opposite orders.
/// </summary>
static inline int Ground_Distance(Point2D const & to, Coord const & from) // RA2: inline in 0x0063DE90
{
	Point2D delta = Point2D(from.X, from.Y) - to; // RA2: the difference is kept in memory
	return((int)fastmath::sqrt((double)delta.Y * (double)delta.Y + (double)delta.X * (double)delta.X));
}

static inline int Ground_Distance(Coord const & to, ObjectClass * object) // RA2: inline in 0x0063DE90
{
	return(Ground_Distance(Point2D(to.X, to.Y), object->Center_Coord())); // RA2: the point is read before the virtual call
}

static inline int Ground_Distance2(Point2D const & to, Coord const & from) // RA2: inline in 0x0063DE90; x*x + y*y
{
	Point2D delta = Point2D(from.X, from.Y) - to;
	return((int)fastmath::sqrt((double)delta.X * (double)delta.X + (double)delta.Y * (double)delta.Y));
}

static inline int Ground_Distance2(Coord const & to, ObjectClass * object) // RA2: inline in 0x0063DE90
{
	return(Ground_Distance2(Point2D(to.X, to.Y), object->Center_Coord()));
}

static inline DirType Direction_To(ObjectClass * object, Coord const & to) // RA2: inline in 0x0063DE90
{
	return(Direction(object->Center_Coord(), to));
}



/// <returns>bool; Is the rocket still flying?</returns>
boolean STDMETHODCALLTYPE RocketLocomotionClass::Process(void) // RA2: 0x0063DE90
{
	RocketStruct const * data; // RA2: the V3 rocket's data for the V3, the Dreadnought missile's for every other aircraft type
	if (((AircraftClass *)LinkedTo)->Class == Rule->V3Rocket.Type) {
		data = &Rule->V3Rocket;
	} else {
		data = &Rule->DMisl;
	}
	bool exploded = false;
	switch (MissionState) {
		case 1: // RA2: waiting before the launch
			CurrentSpeed = 0;
			if (MissionTimer.Progress() == 1.0) {
				MissionState = 2;
				MissionTimer = ProgressTimerClass<FrameTimerClass>(data->TiltFrames);
			}
			break;

		case 2: // RA2: tilting up from the initial to the final pitch, then the launch
			CurrentSpeed = 0;
			if (MissionTimer.Progress() == 1.0) {
				CurrentPitch = data->PitchFinal * 1.5707963267948966;
				MissionState = 3;
				if (LinkedTo->GetLastFlightMapCoords() == CELL_NONE) {
					AircraftTracker.Add(LinkedTo);
				}
				new AnimClass(AnimTypes[AnimTypeClass::From_Name(RocketTakeoffAnimName)], LinkedTo->PositionCoord, 2, 1, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER), -10); // RA2: starts two frames late, one loop, drawn 10 below the rocket
				VocClass::PlayAt(LinkedTo->Techno_Type_Class()->AuxSound1, LinkedTo->PositionCoord, NULL);
			} else {
				float initial = data->PitchInitial * 1.5707963267948966;
				float final = data->PitchFinal * 1.5707963267948966;
				CurrentPitch = initial + (final - initial) * MissionTimer.Progress();
			}
			break;

		case 3: // RA2: accelerating and climbing to the cruise altitude
			CurrentSpeed += data->Acceleration;
			if (CurrentSpeed > LinkedTo->Techno_Type_Class()->MaxSpeed) {
				CurrentSpeed = LinkedTo->Techno_Type_Class()->MaxSpeed;
			}
			if (LinkedTo->Get_Height_AGL() >= data->Altitude) {
				MissionState = 4;
				CruiseDistance = Ground_Distance(MovingDestination, LinkedTo);
			}
			break;

		case 4: // RA2: cruising: the lazy curve blends the pitch toward the target, otherwise it levels off until the dive point
			CurrentSpeed += data->Acceleration;
			if (CurrentSpeed > LinkedTo->Techno_Type_Class()->MaxSpeed) {
				CurrentSpeed = LinkedTo->Techno_Type_Class()->MaxSpeed;
			}
			if (data->LazyCurve && CruiseDistance != 0) {
				if (Reached_Target(data)) {
					exploded = true;
					break;
				}
				double ratio = (double)Ground_Distance2(MovingDestination, LinkedTo) / CruiseDistance;
				double rest = 1.0 - ratio;
				CurrentPitch = Pitch_To_Target() * rest + data->PitchFinal * ratio * 1.5707963267948966;
			} else {
				if (CurrentPitch > 0.0f) {
					CurrentPitch -= data->TurnRate;
					if (CurrentPitch < 0.0f) {
						CurrentPitch = 0;
					}
				}
				Coord dest = MovingDestination;
				Coord coord = LinkedTo->PositionCoord;
				int height = coord.Z - dest.Z;
				dest.Z = LinkedTo->PositionCoord.Z;
				if (LinkedTo->Distance(dest) <= height) {
					MissionState = 5;
				}
			}
			LinkedTo->PrimaryFacing.Set_Desired(Direction_To(LinkedTo, MovingDestination));
			break;

		case 5: // RA2: diving toward the target
			exploded = Reached_Target(data);
			if (!exploded) {
				double diff = Pitch_To_Target() - CurrentPitch;
				float step = diff;
				if (step < 0.0) {
					step = -step;
				}
				if (step > data->TurnRate) {
					if (diff < 0.0) {
						CurrentPitch -= data->TurnRate;
					} else {
						CurrentPitch += data->TurnRate;
					}
				} else {
					CurrentPitch += diff;
				}
			}
			break;
	}
	if (exploded) { // RA2: the rocket blew up at its target
		return(false);
	}
	if (Is_Moving_Now() && TrailerTimer == 0) {
		new AnimClass(AnimTypes[AnimTypeClass::From_Name(RocketTrailAnimName)], LinkedTo->PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER), 0); // RA2: no delay, one loop
		TrailerTimer = 3; // RA2: a trail animation every three frames
	}
	if (CurrentSpeed > 0.0) {
		LinkedTo->Set_Coord(Position_At((int)CurrentSpeed)); // RA2: moves the rocket along its facing and pitch
		Cell cell = LinkedTo->Get_Cell();
		if (cell != LinkedTo->LastFlightMapCoords && LinkedTo->IsActive) {
			AircraftTracker.Update(LinkedTo, LinkedTo->LastFlightMapCoords, cell);
		}
		if (LinkedTo->Strength <= 0) {
			Detonate(); // RA2: shot down in flight
		}
	}
	return(Is_Moving());
}


/// <summary>
/// Explodes the rocket at its nose: takes it off the aircraft tracker, picks the warhead and damage of its kind
/// (the elite ones when the object that launched it is elite), shows the explosion, damages the area and removes it.
/// </summary>
void RocketLocomotionClass::Detonate(void) // RA2: 0x0063E7D0
{
	AircraftTracker.Remove(LinkedTo);
	TechnoClass * spawner = LinkedTo->SpawnOwner; // RA2: +0x23C, the object that launched it
	bool elite = spawner != NULL && spawner->Veterancy.Is_Elite();
	RocketStruct const * data;
	WarheadTypeClass const * warhead;
	if (((AircraftClass *)LinkedTo)->Class == Rule->V3Rocket.Type) {
		data = &Rule->V3Rocket;
		warhead = elite ? Rule->V3EliteWarhead : Rule->V3Warhead;
	} else {
		data = &Rule->DMisl;
		warhead = elite ? Rule->DMislEliteWarhead : Rule->DMislWarhead;
	}
	Coord coord = Position_At(data->BodyLength);
	Cell cell = coord;
	int damage = elite ? data->EliteDamage : data->Damage;
	new AnimClass(Combat_Anim(damage, warhead, Map[cell].Land_Type(), coord), coord, 0, 1, ShapeFlags_Type(SHAPE_ZGRAD|SHAPE_WIN_REL|SHAPE_CENTER), Get_Explosion_Z(coord)); // RA2: the damage animation at the nose
	Combat_Lighting(coord, damage, warhead, 0, 0);
	Explosion_Damage(coord, damage, LinkedTo, warhead, true);
	LinkedTo->Delete_Me();
}


/// <summary>
/// Gives the rocket its destination, once: it starts with the pause before launch (or the tilt without one) and
/// the initial pitch.
/// </summary>
void STDMETHODCALLTYPE RocketLocomotionClass::Move_To(CoordStruct to) // RA2: 0x0063EA80
{
	AircraftTypeClass const * type = ((AircraftClass *)LinkedTo)->Class;
	RocketStruct const * data;
	if (type == Rule->V3Rocket.Type) {
		data = &Rule->V3Rocket;
	} else {
		data = &Rule->DMisl;
	}
	if (MovingDestination == COORD_NONE) {
		if (data->PauseFrames != 0) {
			MissionState = 1;
			MissionTimer = data->PauseFrames;
		} else {
			MissionState = 2;
			MissionTimer = data->TiltFrames;
		}
		CurrentPitch = data->PitchInitial * 1.5707963267948966;
		MovingDestination = to;
	}
}


/// <summary>
/// A rocket cannot stop.
/// </summary>
void STDMETHODCALLTYPE RocketLocomotionClass::Stop_Moving(void) // RA2: 0x0063EB50
{
}


/// <summary>
/// Gives the class id of the rocket locomotor.
/// </summary>
HRESULT STDMETHODCALLTYPE RocketLocomotionClass::GetClassID(CLSID * retval) // RA2: 0x0063EB60
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_RocketLocomotion;
	return(S_OK);
}


/// <summary>
/// Loads the locomotor, then rebuilds it in place so its second timer starts afresh.
/// </summary>
HRESULT STDMETHODCALLTYPE RocketLocomotionClass::Load(IStream * stream) // RA2: 0x0063EBA0
{
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		new (this) RocketLocomotionClass(NoInitClass());
	}
	return(result);
}


/// <returns>LayerType; The rocket flies in the air layer.</returns>
LayerType STDMETHODCALLTYPE RocketLocomotionClass::In_Which_Layer(void) // RA2: 0x0063EBF0
{
	return(LAYER_AIR);
}


/// <summary>
/// Makes the matrix the rocket is drawn with: turned to its facing and pitched; the cache key tells the initial
/// (0x20) and the final (0x40) pitch of its kind from the ones in between.
/// </summary>
Matrix3DStruct STDMETHODCALLTYPE RocketLocomotionClass::Draw_Matrix(int * key) // RA2: 0x0063EC00
{
	Matrix3D mtx(true);
	mtx.Rotate_Z(LinkedTo->PrimaryFacing.Current().As_Radian32());
	if (CurrentPitch != 0.0f) {
		mtx.Rotate_Y(-CurrentPitch);
		RocketStruct const * data;
		if (((AircraftClass *)LinkedTo)->Class == Rule->V3Rocket.Type) {
			data = &Rule->V3Rocket;
		} else {
			data = &Rule->DMisl;
		}
		float final = data->PitchFinal * 1.5707963267948966;
		if (key != NULL) {
			if ((float)(data->PitchInitial * 1.5707963267948966) == CurrentPitch) {
				*key |= 0x20;
			} else if (CurrentPitch == final) {
				*key |= 0x40;
			} else {
				*key = -1;
			}
		}
	}
	if (key != NULL) {
		*key |= LinkedTo->PrimaryFacing.Current().As_Dir32();
	}
	return((Matrix3DStruct &)mtx);
}
