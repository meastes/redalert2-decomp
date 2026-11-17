
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y13; gamemd.exe 0x006622C0, RocketLocomotionClass::Process, 3,440
// bytes; a link-closure trap on the play paths: every V3 rocket, Dreadnought missile and Boomer
// cruise missile in flight). Unmatched in the YR tree. The text is Red Alert 2's (our accepted
// src/recovered/rocket.cpp, game.exe 0x0063DE90) with gamemd.exe's YR changes read from its code:
// the cruise missile's rocket data (CMisl), the elite flag taken from the launcher at the pause and
// tilt, the cruise missile's own launch state 6 (raised by RaiseRate out of its launcher with the
// take-off smoke every 24 frames, then straight to the climb), the layer re-submit around the
// launch (Unknown50), a detonation when the rocket touches the ground while cruising, and a move
// only onto legal cells. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

#include "voc.h"

extern char const * DAT_008399AC;   // the rocket's take-off smoke anim name (RA2: 0x007EE804)
extern char const * DAT_008399B0;   // the rocket's trail anim name (RA2: 0x007EE808)

boolean STDMETHODCALLTYPE RocketLocomotionClass::Process(void)
{
	AircraftTypeClass const * type = ((AircraftClass *)LinkedTo)->Class;
	RocketStruct const * data;
	if (type == Rule->V3Rocket.Type) {
		data = &Rule->V3Rocket;
	} else if (type == Rule->CMisl.Type) {
		data = &Rule->CMisl;
	} else {
		data = &Rule->DMisl;
	}

	switch (MissionState) {

		/*
		**	Waiting in the launcher. The cruise missile puffs smoke and is taken out of
		**	the launcher's layer once.
		*/
		case 1:
			CurrentSpeed = 0.0;
			IsElite = LinkedTo->SpawnOwner != NULL && LinkedTo->SpawnOwner->Veterancy.Is_Elite();
			if (data->Type == Rule->CMisl.Type) {
				if (TrailerTimer == 0) {
					new AnimClass(AnimTypes[AnimTypeClass::From_Name(DAT_008399AC)], LinkedTo->PositionCoord, 2, 1, (ShapeFlags_Type)0x600, -10, false);
					TrailerTimer = 24;
				}
				if (Unknown50) {
					LinkedTo->Mark(MARK_UP);
					Unknown50 = false;
					Map.Submit(LinkedTo);
					LinkedTo->Mark(MARK_DOWN);
				}
			} else {
				Unknown50 = true;
			}
			if (MissionTimer.Progress() == 1.0) {
				MissionState = (data->Type == Rule->CMisl.Type) ? 6 : 2;
				MissionTimer = data->TiltFrames;
			}
			break;

		/*
		**	Tilting from the initial to the final pitch, then launched.
		*/
		case 2:
			CurrentSpeed = 0.0;
			IsElite = LinkedTo->SpawnOwner != NULL && LinkedTo->SpawnOwner->Veterancy.Is_Elite();
			if (MissionTimer.Progress() == 1.0) {
				CurrentPitch = data->PitchFinal * 1.5707963267948966;
				MissionState = 3;
				if (LinkedTo->GetLastFlightMapCoords() == CELL_NONE) {
					AircraftTracker.Add(LinkedTo);
				}
				new AnimClass(AnimTypes[AnimTypeClass::From_Name(DAT_008399AC)], LinkedTo->PositionCoord, 2, 1, (ShapeFlags_Type)0x600, -10, false);
				VocClass::PlayAt(LinkedTo->Techno_Type_Class()->AuxSound1, LinkedTo->PositionCoord, NULL);
			} else {
				float initial = data->PitchInitial * 1.5707963267948966;
				float final = data->PitchFinal * 1.5707963267948966;
				CurrentPitch = initial + (final - initial) * MissionTimer.Progress();
			}
			break;

		/*
		**	The cruise missile rises out of its launcher at the final pitch, smoking.
		*/
		case 6:
			IsElite = LinkedTo->SpawnOwner != NULL && LinkedTo->SpawnOwner->Veterancy.Is_Elite();
			if (TrailerTimer == 0) {
				new AnimClass(AnimTypes[AnimTypeClass::From_Name(DAT_008399AC)], LinkedTo->PositionCoord, 2, 1, (ShapeFlags_Type)0x600, -10, false);
				TrailerTimer = 24;
			}
			{
				Coord coord = LinkedTo->PositionCoord;
				if (MissionTimer.Progress() == 1.0) {
					CurrentPitch = data->PitchFinal * 1.5707963267948966;
					if (LinkedTo->GetLastFlightMapCoords() == CELL_NONE) {
						VocClass::PlayAt(LinkedTo->Techno_Type_Class()->AuxSound1, coord, NULL);
						AircraftTracker.Add(LinkedTo);
					}
					TrailerTimer = 0;
					MissionState = 3;
				} else {
					coord.Z += data->RaiseRate;
					Cell cell = coord;
					if (Map.In_Radar(cell)) {
						LinkedTo->Set_Coord(coord);
					}
				}
			}
			break;

		/*
		**	Accelerating and climbing to the cruise altitude.
		*/
		case 3:
			if (!Unknown50) {
				LinkedTo->Mark(MARK_UP);
				Unknown50 = true;
				Map.Submit(LinkedTo);
				LinkedTo->Mark(MARK_DOWN);
			}
			CurrentSpeed += data->Acceleration;
			if (CurrentSpeed > LinkedTo->Techno_Type_Class()->Speed) {
				CurrentSpeed = LinkedTo->Techno_Type_Class()->Speed;
			}
			if (LinkedTo->Get_Height_AGL() >= data->Altitude) {
				MissionState = 4;
				Coord center = LinkedTo->Center_Coord();
				int dx = center.X - MovingDestination.X;
				int dy = center.Y - MovingDestination.Y;
				Unknown58 = (int)fastmath::sqrt((double)dx * (double)dx + (double)dy * (double)dy);
			}
			break;

		/*
		**	Cruising: a lazy curve blends the pitch toward the target, else level flight
		**	until the dive point.
		*/
		case 4:
			if (LinkedTo->Get_Height_AGL() <= 0) {
				Detonate();
				return(false);
			}
			CurrentSpeed += data->Acceleration;
			if (CurrentSpeed > LinkedTo->Techno_Type_Class()->Speed) {
				CurrentSpeed = LinkedTo->Techno_Type_Class()->Speed;
			}
			if (data->LazyCurve && Unknown58 != 0) {
				if (Reached_Target(data)) {
					return(false);
				}
				Coord center = LinkedTo->Center_Coord();
				int dx = center.X - MovingDestination.X;
				int dy = center.Y - MovingDestination.Y;
				int distance = (int)fastmath::sqrt((double)dx * (double)dx + (double)dy * (double)dy);
				double ratio = (double)distance / Unknown58;
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
				Coord position = LinkedTo->PositionCoord;
				int height = position.Z - dest.Z;
				dest.Z = position.Z;
				Coord center = LinkedTo->Center_Coord();
				int dx = center.X - dest.X;
				int dy = center.Y - dest.Y;
				int dz = center.Z - dest.Z;
				int distance = (int)fastmath::sqrt((double)dx * (double)dx + (double)dy * (double)dy + (double)dz * (double)dz);
				if (distance <= height) {
					MissionState = 5;
				}
			}
			LinkedTo->PrimaryFacing.Set_Desired(Direction(LinkedTo->Center_Coord(), MovingDestination));
			break;

		/*
		**	Diving onto the target.
		*/
		case 5:
			if (Reached_Target(data)) {
				return(false);
			} else {
				double diff = Pitch_To_Target() - CurrentPitch;
				double step = diff;
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

		default:
			break;
	}

	if (Is_Moving_Now() && TrailerTimer == 0) {
		new AnimClass(AnimTypes[AnimTypeClass::From_Name(DAT_008399B0)], LinkedTo->PositionCoord, 0, 1, (ShapeFlags_Type)0x600, 0, false);
		TrailerTimer = 3;
	}

	if (CurrentSpeed > 0.0) {
		Coord coord = Position_At((int)CurrentSpeed);
		Cell cell = coord;
		if (Map.In_Radar(cell)) {
			LinkedTo->Set_Coord(coord);
		}
		Cell now = LinkedTo->Get_Cell();
		if (now != LinkedTo->LastFlightMapCoords && LinkedTo->IsActive) {
			AircraftTracker.Update(LinkedTo, LinkedTo->LastFlightMapCoords, now);
		}
		if (LinkedTo->Strength <= 0) {
			Detonate();
		}
	}
	return(Is_Moving());
}
