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

/*
**	RA2: the ship locomotor, the locomotion of every vessel. Tiberian Sun has no such file;
**	the executable's code (from 0x00675410) is the drive locomotor's (drive.cpp) with the few
**	changes marked below: the ship's own geometry constants, track tables and speed constants,
**	a wake behind a moving ship, and no chase of an infantry navigation target.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: as above

#include "ship.h" // RA2: the ship locomotor has its own header

#include "_map.h" // RA2: as above
#include "_rules.h" // RA2: as above
#include "anim.h" // RA2: as above
#include "assert.h" // RA2: as above
#include "cell.h" // RA2: as above
#include "foot.h" // RA2: as above
#include "globals.h" // RA2: as above
#include "house.h" // RA2: as above
#include "infantry.h" // RA2: as above
#include "inline.h" // RA2: as above
#include "noinit.h" // RA2: as above
#include "overtype.h" // RA2: as above
#include "rules.h" // RA2: as above
#include "tube.h" // RA2: as above
#include "unit.h" // RA2: as above
#include "unittype.h" // RA2: as above
#include "wave.h" // RA2: as above
#include "spawnmanager.h" // RA2: Start_Of_Move asks the spawn manager about launching spawns



EXTERN_C const CLSID CLSID_ShipLocomotion; // RA2: 0x007A1FF0, the ship locomotor's class id (next to CLSID_HoverLocomotion 0x007A1F80)

#include "layer.hh" // RA2: as above
#include "tube.hh" // RA2: as above

#include <new.h> // RA2: as above

static const double _ship_deaccel = 0.3f; // RA2: the ship's own copy of the constant, at 0x007A96B8
static const double _ship_sinking = 0.1f; // RA2: as above, at 0x007A96C0
static const double _ship_sinking_scale = 0.0015f; // RA2: as above, at 0x007A96C8


/// <summary>
/// Creates the ship locomotor in its default state: no destination, no track to follow and
/// no locomotor riding along on top of it.
/// </summary>
ShipLocomotionClass::ShipLocomotionClass(void) : // RA2: as above
	BASECLASS(), // RA2: as above
	CurrentRamp(0), // RA2: as above
	PreviousRamp(0), // RA2: as above
	RampTimer(), // RA2: as above
	DestinationCoord(COORD_NONE), // RA2: as above
	HeadToCoord(COORD_NONE), // RA2: as above
	IsOnShortTrack(false), // RA2: as above
	IsTurretLockedDown(false), // RA2: as above
	IsRotating(false), // RA2: as above
	IsDriving(false), // RA2: as above
	IsRocking(false), // RA2: as above
	IsLocomotorUnlocked(true), // RA2: as above
	SpeedAccum(0), // RA2: as above
	TargetSpeed(0), // RA2: as above
	TrackNumber(-1), // RA2: as above
	TrackIndex(-1), // RA2: as above
	Piggybacker(NULL) // RA2: as above
{
}


/// <summary>
/// Destroys the ship locomotor.
/// </summary>
ShipLocomotionClass::~ShipLocomotionClass(void) // RA2: as above
{
}


/// <summary>
/// Fetches the class identifier of whichever locomotor is driving the unit.
/// That is the identifier of the locomotor riding along on this driver when there is one,
/// and the driver's own otherwise.
/// </summary>
/// <param name="classid">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK if the identifier was supplied, E_FAIL if the locomotor
/// could not be asked, or E_POINTER if no destination was supplied.</returns>
HRESULT ShipLocomotionClass::Piggyback_CLSID(CLSID * classid) // RA2: as above
{
	if (classid == NULL) { // RA2: as above
		return(E_POINTER); // RA2: as above
	}

	if (Piggybacker != NULL) { // RA2: as above
		IPersistPtr ptr(Piggybacker); // RA2: as above
		if (ptr == NULL) { // RA2: as above
			return(E_FAIL); // RA2: as above
		}
		return(ptr->GetClassID(classid)); // RA2: as above
	}

	IPersistPtr ptr(this); // RA2: as above
	if (ptr == NULL) { // RA2: as above
		return(E_FAIL); // RA2: as above
	}
	return(ptr->GetClassID(classid)); // RA2: as above
}


/// <summary>
/// Fetches an interface supported by this locomotor.
/// The driver answers for the piggyback interface on top of whatever the base locomotor
/// already supports.
/// </summary>
/// <param name="riid">The identifier of the interface asked for.</param>
/// <param name="ppvObject">Pointer to the interface pointer to fill in.</param>
/// <returns>Returns with S_OK if the interface was supplied, otherwise
/// E_NOINTERFACE.</returns>
HRESULT STDMETHODCALLTYPE ShipLocomotionClass::QueryInterface(REFIID riid, LPVOID * ppvObject) // RA2: as above
{
	HRESULT result = BASECLASS::QueryInterface(riid, ppvObject); // RA2: as above

	if (result == E_NOINTERFACE) { // RA2: as above
		if (riid == IID_IPiggyback) { // RA2: as above
			*ppvObject = (IPiggyback*)this;
		}
		if (*ppvObject == NULL) { // RA2: as above
			result = E_NOINTERFACE; // RA2: as above
		} else {
			AddRef(); // RA2: as above
			result = S_OK; // RA2: as above
		}
	}
	return(result); // RA2: as above
}


/// <summary>
/// Loads the driver from the supplied stream.
/// The driver is rebuilt in place, and any locomotor that was riding along on it when the
/// game was saved is recreated and reattached.
/// </summary>
/// <param name="stream">The stream to read from.</param>
/// <returns>Returns with S_OK if the driver was read back successfully.</returns>
HRESULT STDMETHODCALLTYPE ShipLocomotionClass::Load(IStream * stream) // RA2: as above
{
	HRESULT result = BASECLASS::Load(stream); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		new (this) ShipLocomotionClass(NoInitClass()); // RA2: as above

		bool haspiggy; // RA2: as above
		stream->Read(&haspiggy, sizeof(haspiggy), NULL); // RA2: as above

		if (haspiggy) { // RA2: as above
			result = OleLoadFromStream(stream, IID_ILocomotion, (void**)&Piggybacker); // RA2: as above
		}
	}
	return(result); // RA2: as above
}


/// <summary>
/// Saves the driver to the supplied stream.
/// Any locomotor riding along on this driver is written out with it, so that the unit
/// comes back in the same state it was saved in.
/// </summary>
/// <param name="stream">The stream to write to.</param>
/// <param name="cleardirty">Should the dirty flag be cleared once written?</param>
/// <returns>Returns with S_OK if the driver was written out successfully.</returns>
HRESULT STDMETHODCALLTYPE ShipLocomotionClass::Save(IStream * stream, BOOL cleardirty) // RA2: as above
{
	HRESULT result = BASECLASS::Save(stream, cleardirty); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		bool haspiggy; // RA2: as above
		if (Piggybacker != NULL) { // RA2: as above
			haspiggy = true; // RA2: as above
			stream->Write(&haspiggy, sizeof(haspiggy), NULL); // RA2: as above
			IPersistStreamPtr ptr(Piggybacker); // RA2: as above
			result = OleSaveToStream(ptr, stream); // RA2: as above
		} else {
			haspiggy = false; // RA2: as above
			stream->Write(&haspiggy, sizeof(haspiggy), NULL); // RA2: as above
		}
	}
	return(result); // RA2: as above
}


/// <summary>
/// Takes on a locomotor to ride along on top of this driver.
/// A unit that must travel in some special manner -- through a tunnel, or aboard a
/// carrier -- keeps its driver but lets the special locomotor move it for the duration.
/// </summary>
/// <param name="pointer">The locomotor that is to take over the unit.</param>
/// <returns>Returns with S_OK if the locomotor was taken on, E_FAIL if one is already
/// riding, or E_POINTER if none was supplied.</returns>
HRESULT STDMETHODCALLTYPE ShipLocomotionClass::Begin_Piggyback(ILocomotion *pointer) // RA2: as above
{
	if (pointer == NULL) { // RA2: as above
		return(E_POINTER); // RA2: as above
	}
	if (Piggybacker == NULL) { // RA2: as above
		Piggybacker = pointer; // RA2: as above
		return(S_OK); // RA2: as above
	}
	return(E_FAIL); // RA2: as above
}


/// <summary>
/// Hands the piggybacking locomotor back to the caller.
/// The riding locomotor is detached and given up, leaving this driver in sole charge of
/// the unit once more.
/// </summary>
/// <param name="pointer">Pointer to the locomotor pointer to fill in.</param>
/// <returns>Returns with S_OK if a locomotor was handed back, S_FALSE if there was none
/// riding, or E_POINTER if no destination was supplied.</returns>
HRESULT ShipLocomotionClass::End_Piggyback(ILocomotion **pointer) // RA2: as above
{
	if (pointer == NULL) { // RA2: as above
		return(E_POINTER); // RA2: as above
	}
	if (Piggybacker != NULL) { // RA2: as above
		*pointer = Piggybacker;
		Piggybacker.Detach(); // RA2: as above
		return(S_OK); // RA2: as above
	}
	return(S_FALSE); // RA2: as above
}


/// <summary>
/// May the piggybacking locomotor hand control back now?
/// A temporary locomotor riding on this driver asks before it lets go. The driver refuses
/// while the vehicle is still under way or while it has been locked, so that control comes
/// back only once the unit has settled.
/// </summary>
/// <returns>bool; Is it safe to end the piggyback?</returns>
boolean ShipLocomotionClass::Is_Ok_To_End(void) // RA2: as above
{
	if (!Is_Moving() && (Piggybacker != NULL && IsLocomotorUnlocked)) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}


/// <summary>
/// Fetches the tilt matrix for the slope the unit is on.
/// While the vehicle is still rocking from one slope to the next, the matrix is blended
/// between the two so that the body swings over the crest rather than jumping.
/// </summary>
/// <returns>Returns with the matrix that tilts the unit onto its slope. This is the
/// identity matrix for a unit standing on level ground.</returns>
Matrix3D ShipLocomotionClass::Get_Slope_Matrix(void) const // RA2: as above
{
	Matrix3D mtx(true); // RA2: as above

	if (!RampTimer.Has_Completed2()) { // RA2: as above
		mtx = ::Get_Slope_Transition_Matrix(PreviousRamp, CurrentRamp, Get_Slope_Ratio()); // RA2: as above
	} else if (CurrentRamp) { // RA2: as above
		mtx = ::Get_Slope_Matrix(CurrentRamp); // RA2: as above
	}

	return(mtx); // RA2: as above
}


/// <summary>
/// Fetches how far the unit has tilted toward its new slope.
/// </summary>
/// <returns>Returns with the progress of the slope transition, from 0 to 1.</returns>
double ShipLocomotionClass::Get_Slope_Ratio(void) const // RA2: as above
{
	return(RampTimer.Progress()); // RA2: as above
}


/// <summary>
/// Sets the slope that the unit is driving onto.
/// The vehicle tilts from the slope it was on to the new one over a short span of time,
/// so that a tank cresting a ramp rocks over it instead of snapping to the new angle.
/// </summary>
/// <param name="ramp">The ramp the unit is moving onto.</param>
void ShipLocomotionClass::Set_Slope(int ramp) // RA2: as above
{
	int cur = CurrentRamp; // RA2: as above
	if (ramp != cur) { // RA2: as above
		PreviousRamp = cur; // RA2: as above
		CurrentRamp = ramp; // RA2: as above
		RampTimer = 3; // RA2: as above
	}
}


/// <summary>
/// Sets the unit's slope with no transition at all.
/// Use this routine when the vehicle should simply already be on the new slope, such as
/// when it is first placed on the map.
/// </summary>
/// <param name="ramp">The ramp the unit is to be sitting on.</param>
void STDMETHODCALLTYPE ShipLocomotionClass::Force_New_Slope(int ramp) // RA2: as above
{
	PreviousRamp = ramp; // RA2: as above
	CurrentRamp = ramp; // RA2: as above
	RampTimer = 0; // RA2: as above
}


/// <summary>
/// Does the unit still have somewhere to be?
/// A unit counts as moving while it owes a destination, and also while it is part way
/// between cells, even if it is not making any headway at this moment.
/// </summary>
/// <returns>bool; Is the unit under way or owing a move?</returns>
boolean STDMETHODCALLTYPE ShipLocomotionClass::Is_Moving(void) // RA2: as above
{
	if (DestinationCoord != COORD_NONE) { // RA2: as above
		return(true); // RA2: as above
	}
	if (HeadToCoord != COORD_NONE && (HeadToCoord.X != LinkedTo->PositionCoord.X || HeadToCoord.Y != LinkedTo->PositionCoord.Y)) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}


/// <summary>
/// Is the unit visibly in motion at this instant?
/// A vehicle turning on the spot counts, as does one under way at some speed. One that
/// has been given a destination but has not gotten rolling yet does not.
/// </summary>
/// <returns>bool; Is the unit moving right now?</returns>
boolean STDMETHODCALLTYPE ShipLocomotionClass::Is_Moving_Now(void) // RA2: as above
{
	if (LinkedTo->PrimaryFacing.Is_Rotating()) { // RA2: as above
		return(true); // RA2: as above
	}
	if (Is_Moving() && HeadToCoord != COORD_NONE && LinkedTo->Current_Speed() > 0) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}


/// <summary>
/// Fetches the final destination of the driving unit.
/// </summary>
/// <returns>Returns with the destination coordinate, or COORD_NONE if the unit has
/// nowhere it must be.</returns>
CoordStruct STDMETHODCALLTYPE ShipLocomotionClass::Destination(void) // RA2: as above
{
	return(DestinationCoord); // RA2: as above
}


/// <summary>
/// Fetches the location the unit is immediately headed for.
/// </summary>
/// <returns>Returns with the coordinate being driven toward. A unit that is not under
/// way returns its current position instead.</returns>
CoordStruct STDMETHODCALLTYPE ShipLocomotionClass::Head_To_Coord(void) // RA2: as above
{
	if (HeadToCoord != COORD_NONE) { // RA2: as above
		return(HeadToCoord); // RA2: as above
	}
	return(LinkedTo->PositionCoord); // RA2: as above
}


/// <summary>
/// Sets the location the unit is to drive to.
/// A stunned unit ignores the order entirely. A destination that lies beneath a bridge is
/// raised to the deck, since that is where the vehicle will actually end up driving.
/// </summary>
/// <param name="to">The location to drive to.</param>
void STDMETHODCALLTYPE ShipLocomotionClass::Move_To(CoordStruct to) // RA2: as above
{
	if (!LinkedTo->IsUnderEMP() && !LinkedTo->IsParalyzed() && !LinkedTo->IsBeingWarpedOut() && !LinkedTo->IsWarpingIn()) { // RA2: as YR's; TibSun's form does not match (Drive's and Ship's copies match only so)
		DestinationCoord = to; // RA2: as above
		if (Coord(to) != COORD_NONE) { // RA2: as above
			if (Map[to].IsUnderBridge) { // RA2: as above
				DestinationCoord.Z += BRIDGE_LEPTON_HEIGHT; // RA2: as above
			}
		}
	}
}


/// <summary>
/// Handles the order for the unit to stop.
/// The destination is given up and the driver begins slowing down. A train engine passes
/// the order back along the line so that every car it is pulling stops with it.
/// </summary>
void STDMETHODCALLTYPE ShipLocomotionClass::Stop_Moving(void) // RA2: as above
{
	if (HeadToCoord != COORD_NONE) { // RA2: as above
		if (LinkedTo->TClass->IsTrain) { // RA2: as above
			UnitClass *unit = (UnitClass *)LinkedTo; // RA2: as above

			if (!unit->IsFollowing) { // RA2: as above
				UnitClass *follower = unit->FollowingMe; // RA2: as above
				while (follower != NULL) { // RA2: as above
					follower->Locomotion->Stop_Moving(); // RA2: as above
					follower = follower->FollowingMe; // RA2: as above
					if (follower == NULL || follower == follower->FollowingMe) { // RA2: as above
						break; // RA2: as above
					}
				}
			}
		}
	}

	TargetSpeed = TargetSpeed < _ship_deaccel ? TargetSpeed : _ship_deaccel; // RA2: reads the ship's own copy of the constant
	DestinationCoord = COORD_NONE; // RA2: as above
}


/// <summary>
/// Is the unit sitting square upon the ground?
/// The drawing code asks this to find out whether the vehicle has settled onto its slope
/// and is neither pitched nor rolled, in which case it can take the cheaper of the two
/// ways of building the unit's draw matrix.
/// </summary>
/// <returns>bool; Is the unit level and settled?</returns>
BOOL ShipLocomotionClass::Is_Angled(void) const // RA2: as above
{
	float percent = Get_Slope_Ratio(); // RA2: as above
	if (percent == 1.0) { // RA2: as above
		if ((float)fabs(LinkedTo->AngleRotatedSideways) < 0.005 && (float)fabs(LinkedTo->AngleRotatedForwards) < 0.005) { // RA2: as above
			return(true); // RA2: as above
		}
	}
	return(false); // RA2: as above
}


/// <summary>
/// Fetches the transformation matrix to draw the unit through.
/// This routine lays the vehicle's slope tilt over the matrix the base locomotor supplies.
/// A unit that is pitched or rolled is also lifted and shoved sideways, so that the voxel
/// appears to pivot about the ground it rests on rather than about its own center.
/// </summary>
/// <param name="key">Pointer to the voxel cache key to be updated. May be NULL.</param>
/// <returns>Returns with the matrix the unit is to be rendered through.</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced); main: best 97.08, tried 11 times (frame, bound, register)
Matrix3DStruct STDMETHODCALLTYPE ShipLocomotionClass::Draw_Matrix(int *key) // RA2: as above
{
	Matrix3DStruct m; // RA2: as above

	if (!Is_Angled()) { // RA2: as above
		Matrix3D m1, m2; // RA2: as above
		m1.Make_Identity(); // RA2: as above
		m2.Make_Identity(); // RA2: as above

		float val = LinkedTo->TClass->VoxelCenterY; // RA2: as above
		float val2 = LinkedTo->TClass->VoxelCenterX; // RA2: as above

		float fcos = fastmath::cos(LinkedTo->AngleRotatedForwards); // RA2: as above
		float fsin = fastmath::sin(LinkedTo->AngleRotatedForwards); // RA2: as above

		float scos = fastmath::cos(LinkedTo->AngleRotatedSideways); // RA2: as above
		float ssin = fastmath::sin(LinkedTo->AngleRotatedSideways); // RA2: as above

		int num = fabs(ssin) * val + fabs(fsin) * val2; // RA2: as above

		int x_shift = (int)(val2 - (float)(int)(fcos * val2)); // RA2: as above
		int x_offset = x_shift; // RA2: as above
		int y_shift = (int)(val - (float)(int)(scos * val)); // RA2: as above
		int y_offset = y_shift; // RA2: as above
		if (LinkedTo->AngleRotatedForwards < 0.0f) { // RA2: as above
			x_offset = -x_shift; // RA2: as above
		}
		if (LinkedTo->AngleRotatedSideways > 0.0f) { // RA2: as above
			y_offset = -y_shift; // RA2: as above
		}

		m1.Translate_Z(num); // RA2: as above
		m2.Translate_X(x_offset); // RA2: as above
		m2.Translate_Y(y_offset); // RA2: as above
		m2.Rotate_X(LinkedTo->AngleRotatedSideways); // RA2: as above
		m2.Rotate_Y(LinkedTo->AngleRotatedForwards); // RA2: as above

		if (key) { // RA2: as above
			*key = -1;
		}

		Matrix3D basemtx(BASECLASS::Draw_Matrix(key)); // RA2: as above
		Matrix3D slopemtx = Get_Slope_Matrix(); // RA2: as above
		m = (Matrix3DStruct &)(((m1 * slopemtx) * basemtx) * m2); // RA2: as above
		return(m); // RA2: as above
	}

	if (key && *key != -1) { // RA2: as above
		int keybase = *key << 6; // RA2: as above
		*key = keybase;
		*key = keybase + CurrentRamp;
	}

	Matrix3D basemtx(BASECLASS::Draw_Matrix(key)); // RA2: as above
	Matrix3D slopemtx = Get_Slope_Matrix(); // RA2: as above
	m = (Matrix3DStruct &)(slopemtx * basemtx); // RA2: as above
	return(m); // RA2: as above
}
#endif


/// <summary>
/// RA2: the shadow transform; an angled unit's is not cached (the key becomes -1).
/// </summary>
Matrix3DStruct STDMETHODCALLTYPE ShipLocomotionClass::Shadow_Matrix(int *key) // RA2: at 0x4A0500 (YR's body)
{
	if (!Is_Angled()) { // RA2: as above
		if (key) { // RA2: as above
			*key = -1;
		}
	}
	return(BASECLASS::Shadow_Matrix(key)); // RA2: as above
}





/// <summary>
/// Handles the unit being placed onto the map.
/// The driver adopts the slope of the cell the vehicle appears on straight away, so that
/// a unit unlimboed onto a ramp is never seen tilting itself into place.
/// </summary>
void STDMETHODCALLTYPE ShipLocomotionClass::Unlimbo(void) // RA2: as above
{
	Force_New_Slope(LinkedTo->Get_Cell_Ptr()->Ramp); // RA2: as above
}


/// <summary>
/// Processes the ship's movement and rotation for one game frame.
/// A ship that is following a track keeps on following it. Otherwise it starts on a new track
/// toward the next cell of its path, or heads for its navigation target, and it rotates in place
/// when the path asks for a turn first. A ship that is moving on open water leaves a wake behind it.
/// </summary>
/// <returns>Returns with true if the ship is moving.</returns>
boolean STDMETHODCALLTYPE ShipLocomotionClass::Process(void) // RA2: as above
{
	Set_Slope(LinkedTo->Get_Cell_Ptr()->Ramp); // RA2: as above

	/*
	**	If the unit is following a track, then continue
	**	to do so -- mindlessly.
	*/
	if (TrackNumber != -1 && IsDriving) { // RA2: as above

		/*
		**	Perform the movement accumulation.
		*/
		if (While_Moving()) return(false); // RA2: as above
		if (!LinkedTo->IsActive) return(false); // RA2: as above
		if (TrackNumber == -1 && (Is_Moving() || LinkedTo->Path[0] != FACING_NONE) && (LinkedTo->Fetch_RTTI() != RTTI_UNIT || !((UnitClass*)LinkedTo)->IsDumping)) { // RA2: Fetch_RTTI() is the virtual slot, not the RTTI member
			bool res = false; // RA2: no chase of an infantry NavCom before the move starts (Drive has one)
			Start_Of_Move(res); // RA2: as above
			if (res) return(false); // RA2: as above
			if (!LinkedTo->IsActive) return(false); // RA2: as above
			While_Moving(true); // RA2: as above
			if (!LinkedTo->IsActive) return(false); // RA2: as above
		}
	} else {

		if (LinkedTo->NavCom != NULL && LinkedTo->NavCom->Fetch_RTTI() == RTTI_CELL && LinkedTo->PositionCell == ((CellClass *)LinkedTo->NavCom)->CellID) { // RA2: Fetch_RTTI() virtual call
			Abandon_Navigation(); // RA2: as above
			return(false); // RA2: as above
		} else {

			if (LinkedTo->CurrentMission == MISSION_GUARD && !IsDriving && DestinationCoord != COORD_NONE && LinkedTo->PositionCoord == DestinationCoord) { // RA2: as above
				Abandon_Navigation(); // RA2: as above
				return(false); // RA2: as above
			}

			/*
			**	For tracked units that are rotating in place, perform the rotation now.
			*/
			if (LinkedTo->PrimaryFacing.Is_Rotating()) { // RA2: as above
				IsRotating = true; // RA2: as above
			} else {

				if (IsRotating) { // RA2: as above
					IsRotating = false; // RA2: as above
					LinkedTo->Per_Cell_Process(PCP_ROTATION); // RA2: as above
					if (!LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(false); // RA2: no LinkedTo == NULL test
				}

				/*
				**	The unit has no track to follow, but if there
				**	is a navigation target or a remaining path,
				**	then start on a new track.
				*/
				if ((LinkedTo->Mission != MISSION_GUARD || Is_Moving()) && LinkedTo->Mission != MISSION_UNLOAD) { // RA2: as above
					if (Is_Moving() || LinkedTo->Path[0] != FACING_NONE) { // RA2: as above

						/*
						**	Double check to make sure that the movement destination is
						**	in a zone that this unit can travel to. If not, then abort
						**	the navigation target.
						*/
						if (LinkedTo->IsLocked && LinkedTo->Mission != MISSION_ENTER && Is_Moving() && !LinkedTo->Is_In_Same_Zone(DestinationCoord)) { // RA2: as above
							Stop_Driver(); // RA2: as above
							if (Abandon_Navigation()) { // RA2: as above
								return(false); // RA2: as above
							}
						} else {
							bool res = false; // RA2: as above
							Start_Of_Move(res); // RA2: as above
							if (res) return(false); // RA2: as above
							if (!LinkedTo->IsActive) return(false); // RA2: as above
							While_Moving(); // RA2: as above
							if (!LinkedTo->IsActive) return(false); // RA2: as above
						}
					} else if (LinkedTo->IsSinking) { // RA2: as above
						Stop_Driver(); // RA2: as above
						TargetSpeed = 0; // RA2: as above
					} else {
						if (LinkedTo->NavCom != NULL) { // RA2: as above
							Move_To(LinkedTo->NavCom->Destination_Coord(LinkedTo)); // RA2: Destination_Coord takes the unit
						}
					}
				}
			}
		}
	}

	if (Is_Moving_Now() && (Frame % 8) == 0) { // RA2: a wake every 8th frame (Drive: 10th)
		if (!LinkedTo->Techno_Type_Class()->Underwater && !LinkedTo->IsOnBridge && LinkedTo->Get_Cell_Ptr()->Land_Type() == LAND_WATER) { // RA2: a submerged ship leaves no wake
			if (Rule->Wake != NULL) { // RA2: as above
				new AnimClass(Rule->Wake, LinkedTo->PositionCoord, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: the executable passes the extra arguments 0, 1, 0x600, 0
			}
		}
	}

	if (DestinationCoord == COORD_NONE && HeadToCoord == COORD_NONE) { // RA2: as above
		if (LinkedTo->Path[0] == FACING_NONE && LinkedTo->Speed > 0) { // RA2: as above
			LinkedTo->Set_Speed(0); // RA2: as above
		}
	}

	return(Is_Moving()); // RA2: as above
}



/// <summary>
/// Marks the midpoint of the track the ship is following as occupied, or as free again.
/// </summary>
/// <param name="headto">The coordinate the ship is heading to.</param>
/// <param name="type">The kind of marking to perform.</param>
void ShipLocomotionClass::Mark_Track(Coord const & headto, MarkType type) // RA2: as above
{
	if (headto != COORD_NONE) { // RA2: as above
		if (!IsOnShortTrack && TrackNumber != -1) { // RA2: as above
			int tracknum = TrackControl[TrackNumber].Track; // RA2: as above
			if (tracknum) { // RA2: as above
				TrackType const * ptr = RawTracks[tracknum - 1].Track; // RA2: as above
				int cellidx = RawTracks[tracknum - 1].Cell; // RA2: as above
				if (cellidx > -1) { // RA2: as above
					Dir256 dir = ptr[cellidx].Facing; // RA2: as above

					if (TrackIndex < cellidx && cellidx != -1) { // RA2: as above
						Point2D offset = Smooth_Turn(ptr[cellidx].Offset, dir); // RA2: as above
						if (type == MARK_UP) { // RA2: as above
							Coord coord(offset.X, offset.Y, LinkedTo->Height); // RA2: as above
							LinkedTo->Clear_Occupy_Bit(coord); // RA2: as above
						} else {
							if (type == MARK_DOWN || type == MARK_DOWN_FORCED) { // RA2: as above
								Coord coord(offset.X, offset.Y, LinkedTo->Height); // RA2: as above
								LinkedTo->Set_Occupy_Bit(coord); // RA2: as above
							}
						}
					}
				}
			}
		}

		if (type == MARK_UP) { // RA2: as above
			LinkedTo->Clear_Occupy_Bit(headto); // RA2: as above
		} else {
			if (type == MARK_DOWN || type == MARK_DOWN_FORCED) { // RA2: as above
				LinkedTo->Set_Occupy_Bit(headto); // RA2: as above
			}
		}
	}
}


/// <summary>
/// Forces the ship onto the indicated track.
/// </summary>
/// <param name="track">The track to follow.</param>
/// <param name="coord">The coordinate the track leads to.</param>
void STDMETHODCALLTYPE ShipLocomotionClass::Force_Track(int track, CoordStruct coord) // RA2: as above
{
	TrackNumber = track; // RA2: as above
	TrackIndex = 0; // RA2: as above
	if ((Coord)coord != COORD_NONE) { // RA2: as above
		if (Start_Driver(coord)) { // RA2: as above
			DestinationCoord = coord; // RA2: as above
			TargetSpeed = 1.0; // RA2: as above
		}
	}
}


/// <summary>
/// Stops the ship from driving and gives up the cell bits it holds along its track.
/// </summary>
/// <returns>bool; Was the ship driving when it was stopped?</returns>
bool ShipLocomotionClass::Stop_Driver(void) // RA2: as above
{
	if (HeadToCoord != COORD_NONE) { // RA2: as above
		HeadToCoord = COORD_NONE; // RA2: as above
		IsDriving = false; // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}


/// <summary>
/// Starts the ship heading toward the destination desired.
/// </summary>
/// <param name="headto">The coordinate of the immediate drive destination, never farther than one cell away.</param>
/// <returns>bool; Could the ship start driving?</returns>
bool ShipLocomotionClass::Start_Driver(Coord const & headto) // RA2: as above
{
	Stop_Driver(); // RA2: as above
	if (headto != COORD_NONE) { // RA2: as above
		HeadToCoord = headto; // RA2: as above
		IsDriving = true; // RA2: as above

		if (Map[headto].Goodie_Check(LinkedTo) && !LinkedTo->IsInLimbo) { // RA2: as above
			Mark_Track(headto, MARK_DOWN); // RA2: as above
			return(true); // RA2: as above
		}
		if (!LinkedTo->IsActive) return(false); // RA2: as above

		HeadToCoord = COORD_NONE; // RA2: as above
		IsDriving = false; // RA2: as above
	}
	return(false); // RA2: as above
}


/// <summary>
/// Tries to turn the ship to the specified direction.
/// When the ship is not moving, the turn is made with a series of short drives, as a three point
/// turn, until the ship faces the direction desired.
/// </summary>
/// <param name="coord">The direction to face.</param>
void ShipLocomotionClass::Do_Turn(DirStruct coord) // RA2: as above
{
	assert(LinkedTo->IsActive); // RA2: as above

	DirType dir = (DirType &)coord; // RA2: as above

	//if (dir != PrimaryFacing) {

#ifdef TOFIX // RA2: as above
		/*
		**	Special rotation track is needed for units that
		**	cannot rotate in place.
		*/
		if (Special.IsThreePoint && TrackNumber == -1 && TClass->Speed == SPEED_WHEEL) { // RA2: as above
			int			facediff;   // Signed difference between current and desired facing. // RA2: as above
			FacingType	face;       // Current facing (ordinal value). // RA2: as above

			facediff = PrimaryFacing.Difference(dir) >> 5; // RA2: as above
			facediff = Bound(facediff, -2, 2); // RA2: as above
			if (facediff) { // RA2: as above
				face = Dir_Facing(PrimaryFacing); // RA2: as above

				IsOnShortTrack = true; // RA2: as above
				Force_Track(face*FACING_COUNT + (face + facediff), Coord); // RA2: as above

				Path[0] = FACING_NONE; // RA2: as above
				Set_Speed(MPH_LIGHT_SPEED);		// Full speed. // RA2: as above
			}
		} else {
			PrimaryFacing.Set_Desired(dir); // RA2: as above
		}
#else // RA2: as above
			LinkedTo->PrimaryFacing.Set_Desired(dir); // RA2: as above
//			IsRotating = true;
#endif // RA2: as above
	//}
}


/// <summary>
/// Processes the ship's movement along its track.
/// The speed the ship may build up is accumulated, and every full step of it moves the ship
/// along the track, entering the next cell, crushing what lies in the way and jumping to the
/// following track where the path turns. The part of a step left over is spent on the way.
/// </summary>
/// <param name="just_started">Is this the first call after the move was started?</param>
/// <returns>bool; Is the ship done moving for this frame (arrived or blocked)?</returns>
#ifdef RA2_NON_MATCHING // not exact as a landing: the text is exact, but three of its forms have no reference text at their place (the train's `UnitClass * unit` cast local, `Map[Coord(coord)]` and the `Cell here` local; variation_candidates.csv, lane F2 session 2) and the named-local count is a window (`target`)
bool ShipLocomotionClass::While_Moving(bool just_started) // RA2: as above
{
	assert(LinkedTo->IsActive); // RA2: as above

	/*
	**	Perform quick legality checks.
	*/
	if ((!IsDriving || TrackNumber == -1) && LinkedTo->Path[0] != FACING_COUNT || (IsRotating && !LinkedTo->TClass->IsTurretEquipped)) { // RA2: as above
		SpeedAccum = 0;		// Kludge?  No speed should accumulate if movement is on hold. // RA2: as above
		return(false); // RA2: as above
	}
	if (LinkedTo->TClass->IsAccelerates) { // RA2: as above
		bool not_passive = LinkedTo->RTTI != RTTI_UNIT || !((UnitClass *)LinkedTo)->Class->IsPassive; // RA2: as above
		if (TrackNumber < BACKUP_INTO_REFINERY && not_passive) { // RA2: as above
			Coord dest_coord = DestinationCoord; // RA2: as above
			dest_coord.Z = Map.Get_Height_GL(dest_coord) + (Map[dest_coord].IsUnderBridge ? BRIDGE_LEPTON_HEIGHT : 0); // RA2: as above
			int distance = (LinkedTo->PositionCoord - dest_coord).Length();//LinkedTo->PositionCoord.Distance_To(dest_coord); // RA2: as above

			bool forced_speed = false; // RA2: as above
			double speed = LinkedTo->Speed; // RA2: as above

			TechnoTypeClass const * tclass = LinkedTo->TClass; // RA2: as above

			if (distance < tclass->SlowdownDistance) { // RA2: as above
				forced_speed = true; // RA2: as above
				speed = MAX(_ship_deaccel, speed - tclass->MaxSpeed * tclass->DeaccelerationFactor); // RA2: the type's MaxSpeed, not Get_Max_Speed()
			} else if (LinkedTo->IsSinking) { // RA2: as above
				speed = MAX(_ship_sinking, speed - tclass->MaxSpeed * _ship_sinking_scale); // RA2: as above
				forced_speed = true; // RA2: as above
			}

			if (LinkedTo->IsCrushing) { // RA2: as above
				TargetSpeed = MIN(TargetSpeed, 0.2); // RA2: as above
				LinkedTo->Set_Speed(TargetSpeed); // RA2: as above
			} else if (forced_speed) { // RA2: as above
				LinkedTo->Set_Speed(speed); // RA2: as above
			} else if (LinkedTo->Speed < TargetSpeed) { // RA2: as above
				LinkedTo->Set_Speed(MIN(TargetSpeed, LinkedTo->Speed + tclass->AccelerationFactor)); // RA2: as above
			} else if (LinkedTo->Speed > TargetSpeed) { // RA2: as above
				LinkedTo->Set_Speed(MAX(TargetSpeed, LinkedTo->Speed - tclass->MaxSpeed * tclass->DeaccelerationFactor)); // RA2: as above
			}

			if (LinkedTo->RTTI == RTTI_UNIT) { // RA2: as above
				UnitClass * following = ((UnitClass *)LinkedTo)->FollowingMe; // RA2: as above
				if (following != NULL) { // RA2: as above
					do { // RA2: as above
						following->Set_Speed(LinkedTo->Speed); // RA2: as above
						following = following->FollowingMe; // RA2: as above
					} while (following && following != following->FollowingMe); // RA2: as above
				}
			}
		}
	} else {
		LinkedTo->Set_Speed(TargetSpeed); // RA2: as above
	}

	/*
	**	If enough movement has accumulated so that the unit can
	**	visibly move on the map, then process accordingly.
	**	Slow the unit down if he's carrying a flag.
	*/
	int maxspeed = LinkedTo->Current_Speed(); // RA2: as above
	//if (IsFormationMove) maxspeed = FormationMaxSpeed;

	int actual;			// Working movement addition value. // RA2: as above
	actual = SpeedAccum + (just_started ? 0 : maxspeed); // RA2: the speed accumulation comes before the next facing is read

	FacingType		nextface;		// Next facing queued in path. // RA2: declared after actual
	nextface = LinkedTo->Path[0]; // RA2: as above

	if (nextface == FACING_COUNT && TrackNumber == -1) { // RA2: as above
		LinkedTo->Mark(MARK_UP); // RA2: as above
		Stop_Driver(); // RA2: as above

		TubeType tubenum = (TubeType)LinkedTo->Get_Cell_Ptr()->Tube; // RA2: as above
		if (tubenum >= TUBE_FIRST && tubenum < Tubes.Count()) { // RA2: as above
			TubeClass * tube = Tubes[tubenum]; // RA2: as above
			Cell c = tube->Exit; // RA2: as above
			HeadToCoord = c.As_Coord(); // RA2: as above
			memcpy((char*)&LinkedTo->Path, (char*)&LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (CONQUER_PATH_MAX-1)); // RA2: as above
			LinkedTo->Path[CONQUER_PATH_MAX-1] = FACING_NONE; // RA2: as above
			LinkedTo->CurrentTube = tubenum; // RA2: as above
			TPoint2D<short> const & enter = tube->Enter; // RA2: the entry point is held in a reference before the tube direction is set
			LinkedTo->CurrentTubeDir = FACING_FIRST; // RA2: as above
			LinkedTo->LastTubeCoord = Map[Adjacent_Cell(Cell(enter), tube->Dirs[0])].Cell_Coord(); // RA2: as above
			int current_height = Map.Get_Height_GL(LinkedTo->PositionCoord); // RA2: as above
			Cell exit = tube->Exit; // RA2: the exit cell is a new local, read before the count
			int count = tube->Count; // RA2: as above
			LinkedTo->LastTubeCoord.Z = current_height + (Map.Get_Height_GL(exit) - current_height) / count; // RA2: uses the new local
			IsDriving = true; // RA2: as above
			TrackNumber = -1; // RA2: as above
		} else {
			LinkedTo->Path[0] = FACING_NONE; // RA2: as above
			TrackNumber = -1; // RA2: as above
			HeadToCoord = COORD_NONE; // RA2: as above
			Stop_Driver(); // RA2: as above
		}
		return(false); // RA2: as above
	}

	if (actual > ((PIXEL_LEPTON_W + PIXEL_LEPTON_H) / 2)) { // RA2: as above
		TurnTrackType	const * track;  // Track control pointer. // RA2: as above
		TrackType		const	* ptr;  // Pointer to coord offset values. // RA2: as above
		int				tracknum;       // The track number being processed. // RA2: as above
		bool				adj;        // Is a turn coming up? // RA2: as above

		track = &TrackControl[TrackNumber]; // RA2: as above
		if (IsOnShortTrack) { // RA2: as above
			tracknum = track->StartTrack; // RA2: as above
		} else {
			tracknum = track->Track; // RA2: as above
		}
		ptr = RawTracks[tracknum-1].Track; // RA2: as above
		//nextface = LinkedTo->Path[0];

		if (nextface < FACING_NONE || nextface > FACING_COUNT) { // RA2: as above
			LinkedTo->Path[0] = FACING_NONE; // RA2: as above
			return(false); // RA2: as above
		}

		/*
		**	Determine if there is a turn coming up. If there is
		**	a turn, then track jumping might occur.
		*/
		adj = false; // RA2: as above
		if (nextface != FACING_COUNT && nextface != FACING_NONE && Dir_Facing(track->Facing) != nextface) { // RA2: as above
			adj = true; // RA2: as above
		}

		/*
		**	Skip ahead the number of track steps required (limited only
		**	by track length). Set the unit to the new position and
		**	flag the unit accordingly.
		*/
		//LinkedTo->Mark(MARK_UP);
		while (actual > (PIXEL_LEPTON_W + PIXEL_LEPTON_H) / 2) { // RA2: as above
			Point2D	offset; // RA2: as above
			Dir256	dir; // RA2: as above

			actual -= (PIXEL_LEPTON_W + PIXEL_LEPTON_H) / 2; // RA2: as above

			offset = ptr[TrackIndex].Offset; // RA2: as above
			if (offset != Point2D(0, 0) || !TrackIndex) { // RA2: as above
				if (LinkedTo->Occupies_Cells()) { // RA2: as above
					LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord); // RA2: as above
					LinkedTo->IsOccupyingCell = false; // RA2: as above
					LinkedTo->IsToPathAroundBlockage = false; // RA2: as above
				}

				Cell oldcell; // RA2: as above
				if (!TrackIndex) { // RA2: as above
					oldcell = LinkedTo->PositionCell; // RA2: as above
				} else {
					dir = ptr[TrackIndex - 1].Facing; // RA2: as above
					Point2D prevoffset = ptr[TrackIndex - 1].Offset; // RA2: as above
					oldcell = Coord(Smooth_Turn(prevoffset, dir), 0).As_Cell(); // RA2: as above
				}

				dir = ptr[TrackIndex].Facing; // RA2: as above
				Coord newcoord = Coord(Smooth_Turn(offset, dir), 0); // RA2: as above
				Cell newcoord_cell = newcoord.As_Cell(); // RA2: both cells are read into locals before the comparison
				Cell linked_coord_cell = LinkedTo->PositionCoord.As_Cell(); // RA2: as above
				Cell newcell = newcoord.As_Cell(); // RA2: as above

				if (newcoord_cell != linked_coord_cell) { // RA2: as above
					LinkedTo->Mark(MARK_UP); // RA2: as above
					LinkedTo->PositionCoord = newcoord; // RA2: as above
					CellClass * oldcellptr = &Map[oldcell]; // RA2: as above
					CellClass * newcellptr = &Map[newcell]; // RA2: as above
					if (newcellptr->Height == oldcellptr->Height - BRIDGE_CELL_HEIGHT && newcellptr->IsUnderBridge) { // RA2: as above
						LinkedTo->IsOnBridge = true; // RA2: as above
					}
					if (!newcellptr->IsUnderBridge && oldcellptr->IsUnderBridge) { // RA2: as above
						LinkedTo->IsOnBridge = false; // RA2: as above
					}
					if (LinkedTo->TClass->IsTrain && !((UnitClass *)LinkedTo)->IsFollowing) { // RA2: as above
						UnitClass * unit = (UnitClass *)LinkedTo; // RA2: the train is held in a local across the calls of the loop (no reference text; a variation)
						bool is_bridge = unit->IsOnBridge || (LinkedTo->PositionCoord.Z >= (Map.Get_Height_GL(LinkedTo->PositionCoord) + BRIDGE_LEPTON_HEIGHT)); // RA2: as above
						ObjectClass * occupier = is_bridge ? Map[newcell].Cell_Bridge_Occupier() : Map[newcell].Cell_Occupier(); // RA2: one conditional expression over the two accessors
						while (occupier != NULL) { // RA2: as above
							ObjectClass * next = occupier->Next; // RA2: as above
							if (!occupier->Class_Of()->IsCrushable) { // RA2: as above
								int strength = 10000; // RA2: as above
								occupier->Take_Damage(strength, 0, Rule->C4Warhead, NULL, true, true); // RA2: as above
								strength = 20; // RA2: as above
								unit->Take_Damage(strength, 0, Rule->C4Warhead, NULL, true, false); // RA2: through the local
							}
							occupier = next; // RA2: as above
						}
					}
					if (!LinkedTo->IsActive) return(false); // RA2: as above
					LinkedTo->Mark(MARK_DOWN); // RA2: as above

					if (newcellptr->Overlay != OVERLAY_NONE) { // RA2: as above
						OverlayTypeClass * otype = OverlayTypes[newcellptr->Overlay]; // RA2: as above
						if (IsRocking && (LinkedTo->TClass->IsCrusher || LinkedTo->Has_Ability(ABILITY_CRUSHER)) && otype->HeapID == OVERLAY_SANDBAG_WALL) { // RA2: as above
							LinkedTo->IsCrushing = true; // RA2: as above
							if (LinkedTo->Techno_Type_Class()->IsTiltsWhenCrushes) { // RA2: as above
								LinkedTo->RockingForwardsPerFrame = -0.02f; // RA2: as above
							}
						}
					}
				} else {
					bool down = LinkedTo->IsDown; // RA2: as above
					LinkedTo->IsDown = false; // RA2: as above
					LinkedTo->PositionCoord = newcoord; // RA2: as above
					LinkedTo->IsDown = down; // RA2: as above
				}

				if (!LinkedTo->IsActive) { // RA2: as above
					return(false); // RA2: as above
				}

				bool down = LinkedTo->IsDown; // RA2: as above
				LinkedTo->IsDown = false; // RA2: as above
				LinkedTo->HeightAGL = 0; // RA2: as above
				LinkedTo->IsDown = down; // RA2: as above

				LinkedTo->PrimaryFacing.Set(dir); // RA2: as above

				/*
				**	See if "per cell" processing is necessary.
				*/
				if (TrackIndex && RawTracks[tracknum-1].Cell == TrackIndex) { // RA2: as above
					LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord); // RA2: as above
				}

				/*
				**	The unit could "jump tracks". Check to see if the unit should
				**	do so.
				*/
				if (nextface != FACING_COUNT && nextface != FACING_NONE && adj && RawTracks[tracknum-1].Jump == TrackIndex && TrackIndex) { // RA2: as above
					TurnTrackType const * newtrack;		// Proposed jump-to track. // RA2: as above
					int	tnum; // RA2: as above

					tnum = (int)(DirType(track->Facing).As_Dir8() * FACING_COUNT) + (int)nextface; // RA2: the whole direction word is read (As_Dir8), where the turn test above goes through Dir_Facing
					newtrack = &TrackControl[tnum]; // RA2: as above
					if (newtrack->Track && RawTracks[newtrack->Track-1].Entry) { // RA2: as above
						double oldspeed = LinkedTo->Speed; // RA2: as above

						Coord c = Adjacent_Cell(HeadToCoord, nextface); // RA2: as above

						switch (LinkedTo->Can_Enter_Cell(&Map[c.As_Cell()], nextface, LinkedTo->Get_Cell_Height())) { // RA2: as above
							case MOVE_OK: // RA2: as above
							case MOVE_MOVING_BLOCK: // RA2: as above
								if (LinkedTo->RTTI == RTTI_UNIT && !((UnitClass *)LinkedTo)->Class->IsPassive) break; // RA2: as above
								IsOnShortTrack = false;		// Shouldn't be necessary, but... // RA2: as above
								TrackNumber = tnum; // RA2: as above
								track = newtrack; // RA2: as above

								tracknum = track->Track; // RA2: as above
								TrackIndex = RawTracks[tracknum-1].Entry-1;	// Anticipate increment. // RA2: as above
								ptr = RawTracks[tracknum-1].Track; // RA2: as above
								adj = false; // RA2: as above

								Stop_Driver(); // RA2: as above
								IsDriving = true; // RA2: as above
								LinkedTo->Per_Cell_Process(PCP_END); // RA2: as above
								IsDriving = false; // RA2: as above
								if (!LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(false); // RA2: no LinkedTo == NULL test
								if (Start_Driver(c)) { // RA2: as above
									LinkedTo->Set_Speed(oldspeed); // RA2: as above
									memcpy((char*)&LinkedTo->Path, (char*)&LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (CONQUER_PATH_MAX-1)); // RA2: as above
									LinkedTo->Path[CONQUER_PATH_MAX-1] = FACING_NONE; // RA2: as above
								}
								break; // RA2: as above

							case MOVE_CLOAK: // RA2: as above
								Map[c].Shimmer(); // RA2: as above
								break; // RA2: as above

							case MOVE_CLOSED_GATE: // RA2: as above
								Incoming(c.As_Cell()); // RA2: through the locomotor's own Incoming, as Start_Of_Move
								break; // RA2: as above

							case MOVE_TEMP: { // RA2: as above
									bool bridge; // RA2: as above
									if (!Map[c].IsUnderBridge || abs(LinkedTo->PositionCoord.Z / LEVEL_LEPTON_H - Map[c].Height) <= 2) { // RA2: as above
										bridge = false; // RA2: as above
									} else {
										bridge = true; // RA2: as above
									}
									Map[HeadToCoord].Incoming(COORD_NONE, true, true, bridge); // RA2: as above
								}
								break; // RA2: as above
						}
					}
				}
				TrackIndex++; // RA2: as above

			} else {
				Coord dc = HeadToCoord - LinkedTo->PositionCoord; // RA2: as above
				int d = abs(dc.X) + abs(dc.Y); // RA2: as above
				actual += (int)((1.0 - (double)d / 11.0) * 7.0); // RA2: as above
				LinkedTo->IsOccupyingCell = true; // RA2: as above
				LinkedTo->IsToPathAroundBlockage = false; // RA2: as above

				Cell here = LinkedTo->PositionCoord.As_Cell(); // RA2: the unit's cell is read first and compared on the left (no reference text; a variation)
				if (here != HeadToCoord.As_Cell()) { // RA2: as above
					LinkedTo->Mark(MARK_UP); // RA2: as above
					LinkedTo->PositionCoord = HeadToCoord; // RA2: as above
					LinkedTo->HeightAGL = 0; // RA2: as above
					LinkedTo->Mark(MARK_DOWN); // RA2: as above
				} else {
					bool down = LinkedTo->IsDown; // RA2: as above
					LinkedTo->IsDown = false; // RA2: as above
					LinkedTo->PositionCoord = HeadToCoord; // RA2: as above
					LinkedTo->HeightAGL = 0; // RA2: as above
					LinkedTo->IsDown = down; // RA2: as above
				}

				//actual = 0;
				//DestinationCoord = Head_To_Coord();
				Stop_Driver(); // RA2: as above
				TrackNumber = -1; // RA2: as above
				TrackIndex = NULL; // RA2: as above

				bool arrived = false; // RA2: as above
				AbstractClass * target = LinkedTo->NavCom; // RA2: the ship's text holds the NavCom in a local (Drive's has its maxspeed local instead; the function is sensitive to the count of named locals)
				if (target != NULL && LinkedTo->PositionCell == target->Destination_Coord(LinkedTo).As_Cell()) { // RA2: Destination_Coord takes the unit
					if (abs(LinkedTo->Destination_Coord(LinkedTo).Z - DestinationCoord.Z) < 2 * LEVEL_LEPTON_H) { // RA2: as above
						arrived = true; // RA2: as above
						DestinationCoord = COORD_NONE; // RA2: as above
						Stop_Driver(); // RA2: as above
						IsDriving = false; // RA2: as above
					}
				}

				/*
				**	Perform "per cell" activities.
				*/
				//LinkedTo->Mark(MARK_DOWN);
				LinkedTo->Per_Cell_Process(PCP_END); // RA2: as above
				if (!LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(true); // RA2: no LinkedTo == NULL test
				//LinkedTo->Mark(MARK_UP);

				if (arrived) { // RA2: as above
					LinkedTo->Abort_Motion(); // RA2: not just NavCom = NULL
					LinkedTo->Path[0] = FACING_NONE; // RA2: as above
					if (LinkedTo->Mission == MISSION_MOVE && LinkedTo->Enter_Idle_Mode()) { // RA2: as above
						return(true); // RA2: as above
					}
				}

				if (LinkedTo->Is_Docked_For_Repair()) { // RA2: as above
					return(true); // RA2: as above
				}

				if (!LinkedTo->IsActive) return(false); // RA2: as above

				break; // RA2: as above
			}
		}
		// if (LinkedTo->IsActive) {
		// 	LinkedTo->Mark(MARK_DOWN);
		// }
	}

	/*
	**	Replace any remainder back into the unit's movement
	**	accumulator to be processed next pass.
	*/
	SpeedAccum = actual; // RA2: as above

	if (actual > 0 && TrackNumber > -1) { // RA2: as above
		TurnTrackType	const * track;  // Track control pointer. // RA2: as above
		TrackType		const	* ptr;  // Pointer to coord offset values. // RA2: as above
		int				tracknum;       // The track number being processed. // RA2: as above

		track = &TrackControl[TrackNumber]; // RA2: as above
		if (IsOnShortTrack) { // RA2: as above
			tracknum = track->StartTrack; // RA2: as above
		} else {
			tracknum = track->Track; // RA2: as above
		}
		ptr = RawTracks[tracknum-1].Track; // RA2: as above

		Dir256	dir; // RA2: as above
		Point2D offset = ptr[TrackIndex].Offset; // RA2: declared with its value, after dir
		if (offset != Point2D(0, 0) || !TrackIndex) { // RA2: as above

			Coord oldcoord = LinkedTo->PositionCoord; // RA2: as above

			dir = ptr[TrackIndex].Facing; // RA2: as above
			Coord movement(Smooth_Turn(offset, dir) - LinkedTo->PositionCoord, 0); // RA2: constructed in place
			Coord stepcoord = movement + LinkedTo->PositionCoord; // RA2: as above
			CellClass * stepcellptr = &Map[stepcoord]; // RA2: as above

			Coord partial(0, 0, 0); // RA2: as above
			movement = Lerp(partial, movement, SpeedAccum / 7.0); // RA2: the blended step overwrites the movement /// 7 == (CELL_LEPTON / ((CELL_PIXEL_W + CELL_PIXEL_H) / 2))
			Coord partialcoord = oldcoord + movement; // RA2: as above

			CellClass * oldcellptr = &Map[oldcoord]; // RA2: as above
			CellClass * partialcellptr = &Map[partialcoord]; // RA2: as above

			Coord coord = LinkedTo->PositionCoord; // RA2: the height is not kept apart: the chosen coordinate replaces it whole

			if (partialcellptr != stepcellptr && partialcellptr != oldcellptr) { // RA2: as above
				if (SpeedAccum > 3) { // RA2: as above
					coord = stepcoord; // RA2: as above
				}
			} else {
				coord = partialcoord; // RA2: as above
			}

			CellClass * newcellptr = &Map[Coord(coord)]; // RA2: the cell is looked up through a copy of the coordinate (no reference text; a variation)

			if (coord.As_Cell() != LinkedTo->PositionCoord.As_Cell()) { // RA2: as above
				LinkedTo->Mark(MARK_UP); // RA2: as above
				LinkedTo->PositionCoord = coord; // RA2: as above
				if (newcellptr->Height == oldcellptr->Height - BRIDGE_CELL_HEIGHT && newcellptr->IsUnderBridge) { // RA2: as above
					LinkedTo->IsOnBridge = true; // RA2: as above
				}
				if (!newcellptr->IsUnderBridge && oldcellptr->IsUnderBridge) { // RA2: as above
					LinkedTo->IsOnBridge = false; // RA2: as above
				}
				LinkedTo->Mark(MARK_DOWN); // RA2: as above
			} else {
				bool down = LinkedTo->IsDown; // RA2: as above
				LinkedTo->IsDown = false; // RA2: as above
				LinkedTo->PositionCoord = coord; // RA2: as above
				LinkedTo->IsDown = down; // RA2: as above
			}
		}
	}

	return(false); // RA2: as above
}
#endif



/// <summary>
/// Tries to get the ship to advance toward the next cell of its path.
/// This checks that the cell may be entered, reserves it, and picks the track the ship is to
/// follow into it, with a smooth turn when the path bends there.
/// </summary>
/// <param name="stop_processing">Set when the ship must not be processed any further this frame.</param>
/// <param name="retry">May the path be rebuilt and the move tried once more?</param>
/// <param name="force_straight">Is the ship to drive straight, without a smooth turn?</param>
/// <returns>bool; Should this routine be called again because the start was delayed?</returns>
bool ShipLocomotionClass::Start_Of_Move(bool & stop_processing, bool retry, bool force_straight) // RA2: ra2-decomp's twin text
{
	FacingType facing = LinkedTo->Path[0];				// Direction movement will commence. // RA2: as above

	if (!Is_Moving() && facing == FACING_NONE) { // RA2: as above
		IsTurretLockedDown = false; // RA2: as above
		Stop_Driver(); // RA2: as above
		if (LinkedTo->Get_Mission() == MISSION_MOVE) { // RA2: as above
			stop_processing = LinkedTo->Enter_Idle_Mode(false, true); // RA2: as above
		}
		return(false);		// Why is it calling this routine!?! // RA2: as above
	}

	if (DestinationCoord == COORD_NONE) return(false); // RA2: as above

	if (LinkedTo->SpawnManager != NULL && LinkedTo->SpawnManager->CountLaunchingSpawns() != 0) return(true); // RA2: wait while spawns are launching
	if (LinkedTo->IsBeingWarpedOut() || LinkedTo->IsWarpingIn()) return(false); // RA2: a unit being warped out or in does not start a move (tested after the spawn manager)
	if (LinkedTo->IsUnderEMP() || LinkedTo->IsParalyzed()) return(true); // RA2: replaces the StunDuration test

	/*
	**	Reduce the path length if the target is a unit and the
	**	range to the unit is less than the precalculated path steps.
	*/
	if (facing != FACING_NONE) { // RA2: as above
		if ((LinkedTo->NavCom != NULL && LinkedTo->NavCom->Fetch_RTTI() == RTTI_UNIT) || (LinkedTo->NavCom != NULL && LinkedTo->NavCom->Fetch_RTTI() == RTTI_INFANTRY)) { // RA2: Fetch_RTTI() tests instead of dynamic_cast
			int dist = LinkedTo->Distance(DestinationCoord) / CELL_LEPTON_W; // RA2: as above
			if (dist < CONQUER_PATH_MAX) { // RA2: as above
				LinkedTo->Path[dist] = FACING_NONE; // RA2: as above
				facing = LinkedTo->Path[0];		// Maybe needed. // RA2: as above
			}
		}
	}

	/*
	**	If the path is invalid at this point, then generate one. If
	**	generating a new path fails, then abort NavCom.
	*/
	if (facing == FACING_NONE) { // RA2: as above

		/*
		**	If after a path search, there is still no valid path, then set the
		**	NavCom to null and let the script take care of assigning a new
		**	navigation target.
		*/
		if (LinkedTo->PathDelay != 0) { // RA2: as above
			return(false); // RA2: as above
		}
		LinkedTo->PathDelay = CDTimerClass<FrameTimerClass>((int)(Rule->PathDelay * TICKS_PER_MINUTE)); // RA2: as above

		if (!LinkedTo->Basic_Path(DestinationCoord.As_Cell(), 0, 0)) { // RA2: as above
			if (LinkedTo == NULL) { // RA2: as above
				stop_processing = true; // RA2: as above
				return(false); // RA2: as above
			}
			if (!LinkedTo->Is_In_Same_Zone(DestinationCoord)) { // RA2: a failed zone test comes first and gives up the destination
				LinkedTo->Assign_Destination(NULL, true); // RA2: as above
				return(false); // RA2: as above
			}

			if (DestinationCoord == COORD_NONE) return(false); // RA2: as above

			/*
			**	If the unit is close enough to the target then just stop
			**	driving now. This prevents the fidgeting that would occur
			**	if they mindlessly kept trying to get to the exact location
			**	desired. This is quite necessary since it is typical to move
			**	several units with the same mouse click.
			*/
			if (!LinkedTo->Is_On_Priority_Mission() && LinkedTo->Distance(DestinationCoord) < Rule->CloseEnoughDistance && (LinkedTo->Get_Mission() == MISSION_MOVE || LinkedTo->Get_Mission() == MISSION_GUARD_AREA)) { // RA2: as above
				Stop_Driver(); // RA2: as above
				if (Abandon_Navigation()) return(true); // RA2: as above
				if (!LinkedTo->IsActive) return(false); // RA2: as above
			} else {
				/*
				**	If a basic path could not be found, but the immediate move destination is
				**	blocked by a friendly temporary blockage, then cause that blockage
				**	to scatter.
				*/
				Cell cell = Adjacent_Cell(LinkedTo->Center_Coord().As_Cell(), LinkedTo->PrimaryFacing.Current().As_Dir8()); // RA2: as above
				if (Map.In_Local_Radar(cell, true)) { // RA2: as above
					MoveType ok = LinkedTo->Can_Enter_Cell(&Map[cell], LinkedTo->PrimaryFacing.Current().As_Dir8(), LinkedTo->Get_Cell_Height(), 0, true); // RA2: as above
					if (ok == MOVE_CLOSED_GATE) { // RA2: as above
						Incoming(cell); // RA2: as above
					} else if (ok == MOVE_TEMP) { // RA2: as above
						CellClass * cellptr = &Map[cell]; // RA2: as above
						TechnoClass * blockage = cellptr->Cell_Techno(Point2D(0, 0), LinkedTo->Get_Coord().Z > Map.Get_Height_GL(Coord(cell.X * CELL_LEPTON_W + CELL_LEPTON_W / 2, cell.Y * CELL_LEPTON_H + CELL_LEPTON_H / 2, 0)) + 2 * LEVEL_LEPTON_H); // RA2: as above
						if (blockage != NULL && LinkedTo->House->Is_Ally(blockage) && !LinkedTo->TClass->IsTrain) { // RA2: as above

							/*
							**	If the target can be told to get out of the way, only bother
							**	to do so if we aren't very close to the target and this
							**	object can just say "good enough" and stop here.
							*/
							if (LinkedTo->Distance(DestinationCoord) < Rule->CloseEnoughDistance && !LinkedTo->HasAnyLink()) { // RA2: as above
								if (abs(DestinationCoord.Z - LinkedTo->PositionCoord.Z) < 2 * LEVEL_LEPTON_H) { // RA2: as above
									if (Map[LinkedTo->Get_Coord()].Land_Type() != LAND_TUNNEL) { // RA2: as above
										Stop_Driver(); // RA2: as above
										if (Abandon_Navigation()) return(true); // RA2: the result is tested here, not returned as it is (the exe repeats the inlined call at this site)
										return(false); // RA2: as above
									}
								}
							}

							bool is_bridge = cellptr->IsUnderBridge && abs(LinkedTo->PositionCoord.Z / LEVEL_LEPTON_H - cellptr->Height) > 2; // RA2: as above
							cellptr->Incoming(COORD_NONE, true, true, is_bridge); // RA2: as above
						}
					}
				}

				if (LinkedTo->TryTryAgain > 0) { // RA2: as above
					LinkedTo->TryTryAgain--; // RA2: as above
				} else {
					Stop_Driver(); // RA2: as above
					if (Abandon_Navigation()) return(true); // RA2: as above
					if (!LinkedTo->IsActive) return(false); // RA2: as above
					if (LinkedTo->IsNewNavCom) VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000, 0); // RA2: as above
					LinkedTo->IsNewNavCom = false; // RA2: as above
				}
			}

			/*
			**	Since the path was blocked, check to make sure that it was completely
			**	blocked. If so and it has a valid TarCom and it is out of range of the
			**	TarCom, then give this unit a range limit so that it might not pick
			**	a "can't reach" target again.
			*/
			if (!Is_Moving() && LinkedTo->TarCom != NULL && !LinkedTo->IsCloseEnoughToAttack(LinkedTo->TarCom)) { // RA2: as above
				LinkedTo->IsScanLimited = true; // RA2: as above
				if (LinkedTo->Team != NULL) LinkedTo->Team->Scan_Limit(); // RA2: as above
				LinkedTo->Assign_Target(NULL); // RA2: as above
			}

			/*
			**	Stop the movement, for now, and let the subsequent logic in later game
			**	frames resume movement as appropriate.
			*/
			Stop_Driver(); // RA2: as above
			TrackNumber = -1; // RA2: as above
			IsTurretLockedDown = false; // RA2: as above
			return(false); // RA2: as above
		}

		if (LinkedTo->Path[0] == FACING_COUNT) { // RA2: as above
			return(false); // RA2: as above
		}

		/*
		**	If a basic path could be found, but the immediate move destination is
		**	blocked by a friendly temporary blockage, then cause that blockage
		**	to scatter.
		*/
		Cell cell = Adjacent_Cell(LinkedTo->Center_Coord().As_Cell(), LinkedTo->Path[0]); // RA2: as above
		if (Map.In_Local_Radar(cell, true)) { // RA2: as above
			MoveType ok = LinkedTo->Can_Enter_Cell(&Map[cell], LinkedTo->Path[0], LinkedTo->Get_Cell_Height(), 0, true); // RA2: as above

			if (ok == MOVE_CLOSED_GATE) { // RA2: as above
				Incoming(cell); // RA2: as above
			} else if (ok == MOVE_TEMP) { // RA2: as above
				CellClass * cellptr = &Map[cell]; // RA2: as above
				TechnoClass * blockage = cellptr->Cell_Techno(Point2D(0, 0), LinkedTo->Get_Coord().Z > Map.Get_Height_GL(cell) + 2 * LEVEL_LEPTON_H); // RA2: as above
				if (blockage != NULL && LinkedTo->House->Is_Ally(blockage) && !LinkedTo->TClass->IsTrain) { // RA2: as above

					/*
					**	If the target can be told to get out of the way, only bother
					**	to do so if we aren't very close to the target and this
					**	object can just say "good enough" and stop here.
					*/
					if (LinkedTo->Distance(DestinationCoord) < Rule->CloseEnoughDistance && !LinkedTo->HasAnyLink()) { // RA2: as above
						if (abs(DestinationCoord.Z - LinkedTo->PositionCoord.Z) < 2 * LEVEL_LEPTON_H) { // RA2: as above
							if (Map[LinkedTo->Get_Coord()].Land_Type() != LAND_TUNNEL) { // RA2: as above
								Stop_Driver(); // RA2: as above
								return(Abandon_Navigation()); // RA2: as above
							}
						}
					}

					bool is_bridge = cellptr->IsUnderBridge && abs(LinkedTo->PositionCoord.Z / LEVEL_LEPTON_H - cellptr->Height) > 2; // RA2: as above
					cellptr->Incoming(COORD_NONE, true, true, is_bridge); // RA2: as above
				}
			}
		}

		LinkedTo->TryTryAgain = FootClass::PATH_RETRY; // RA2: as above
		facing = LinkedTo->Path[0]; // RA2: as above
	}

	/*
	 * Beyond this point, actual track assignment can begin. This is reached either
	 * because the path was already valid (facing != FACING_NONE on entry) or because
	 * a new path was just successfully generated.
	 */
	if (facing == FACING_COUNT) { // RA2: as above
		return(false); // RA2: as above
	}

	/*
	**	Determine the coordinate of the next cell to move into.
	*/
	Coord dest = Adjacent_Cell(LinkedTo->PositionCoord, facing); // RA2: as above

	CellClass const & here = Map[LinkedTo->Get_Coord()]; // RA2: as above
	int cell_height = here.Height + (LinkedTo->IsOnBridge ? BRIDGE_CELL_HEIGHT : 0); // RA2: as above

	/*
	 * If this move crosses a bridge transition, force a full scan next process.
	 */
	CellClass const & next = Map[Adjacent_Cell(LinkedTo->PositionCoord, facing)]; // RA2: as above
	if (LinkedTo->IsOnBridge ^ next.IsUnderBridge) { // RA2: as above
		LinkedTo->IsPlanningToLook = true; // RA2: as above
	}
	if (!LinkedTo->Is_Ready_To_Move()) { // RA2: as above
		return(true); // RA2: as above
	}
	if (!Incoming(dest.As_Cell())) { // RA2: as above
		return(true); // RA2: as above
	}

	/*
	**	Set the facing correctly if it isn't already correct. This
	**	means starting a rotation track if necessary.
	*/
	if (DirType(DIR_MIN) < (LinkedTo->PrimaryFacing.Current() - DirType(facing))) { // RA2: as above

		/*
		**	Request a change of facing.
		*/
		Do_Turn((DirStruct &)DirType(facing)); // RA2: as above
		return(true); // RA2: as above

	} else {

		/* NOTE:  Beyond this point, actual track assignment can begin.
		**
		**	If the cell to move into is impassable (probably for some unexpected
		**	reason), then abort the path list and set the speed to zero. The
		**	next time this routine is called, a new path will be generated.
		*/
		Cell destcell = dest.As_Cell(); // RA2: as above
		CellClass * destptr = &Map[destcell]; // RA2: as above
		bool crushable_dest = false; // RA2: as above
		LinkedTo->Mark(MARK_UP); // RA2: as above
		MoveType cando = LinkedTo->Can_Enter_Cell(destptr, facing, cell_height, 0, true); // RA2: as above
		LinkedTo->Mark(MARK_DOWN); // RA2: as above

		/*
		 * A train ignores all blockages, and a crusher rolls straight over sandbags.
		 */
		if (cando < MOVE_NO && LinkedTo->TClass->IsTrain) { // RA2: as above
			cando = MOVE_OK; // RA2: as above
		} else if ((cando == MOVE_DESTROYABLE || cando == MOVE_FRIENDLY_DESTROYABLE) && LinkedTo->TClass->IsCrusher && destptr->Overlay == OVERLAY_SANDBAG_WALL) { // RA2: as above
			cando = MOVE_OK; // RA2: as above
		}


		/*
		 * If the destination cell holds a crushable overlay, then remember to drive
		 * straight through it rather than curving around.
		 */
		if (destptr->Overlay != OVERLAY_NONE && cando == MOVE_OK && OverlayTypes[destptr->Overlay]->IsCrushable) { // RA2: as above
			crushable_dest = true; // RA2: as above
		}

		if (cando != MOVE_OK) { // RA2: as above
			/*
			 * The destination cell cannot be entered. Handle each blockage type.
			 */
			if (cando == MOVE_CLOSED_GATE) { // RA2: as above
				Incoming(destcell); // RA2: as above
			}

			/*
			**	If a temporary friendly object is blocking the path, then cause it to
			**	get out of the way.
			*/
			else if (cando == MOVE_TEMP) { // RA2: as above
				if (!LinkedTo->TClass->IsTrain) { // RA2: as above
					if (retry) { // RA2: as above
						LinkedTo->Path[0] = FACING_NONE; // RA2: as above
						LinkedTo->PathDelay = CDTimerClass<FrameTimerClass>(0); // RA2: as above
						return(Start_Of_Move(stop_processing, false, false)); // RA2: as above
					}
					if (LinkedTo->Distance(DestinationCoord) >= Rule->CloseEnoughDistance // RA2: as above
						|| abs(DestinationCoord.Z - LinkedTo->PositionCoord.Z) >= 2 * LEVEL_LEPTON_H // RA2: as above
						|| Map[LinkedTo->Get_Coord()].Land_Type() == LAND_TUNNEL) { // RA2: as above
						bool is_bridge = Map[destcell].IsUnderBridge && abs(LinkedTo->PositionCoord.Z / LEVEL_LEPTON_H - Map[destcell].Height) > 2; // RA2: as above
						Map[destcell].Incoming(COORD_NONE, true, true, is_bridge); // RA2: as above
					} else {
						Stop_Driver(); // RA2: as above
						if (Abandon_Navigation()) { // RA2: as above
							return(true); // RA2: as above
						}
					}
				}

			/*
			**	If a cloaked object is blocking, then shimmer the cell.
			*/
			} else if (cando == MOVE_CLOAK) { // RA2: as above
				Map[destcell].Shimmer(); // RA2: as above
				if (retry) { // RA2: as above
					LinkedTo->Path[0] = FACING_NONE; // RA2: as above
					return(Start_Of_Move(stop_processing, false, false)); // RA2: as above
				}

				Stop_Driver(); // RA2: as above
				return(Abandon_Navigation()); // RA2: as above
			}

			Stop_Driver(); // RA2: as above
			if (cando == MOVE_MOVING_BLOCK) { // RA2: as above

				/*
				 * If this is a temporary blockage, try once more to path around it.
				 */
				if (!LinkedTo->IsToPathAroundBlockage) { // RA2: as above
					LinkedTo->IsToPathAroundBlockage = true; // RA2: as above
					LinkedTo->BlockagePathDelay = CDTimerClass<FrameTimerClass>(Rule->BlockagePathDelay); // RA2: as above
				}
				if (LinkedTo->PathDelay == 0) { // RA2: as above
					bool around_blockage = LinkedTo->IsToPathAroundBlockage && LinkedTo->BlockagePathDelay == 0; // RA2: as above
					bool ok = LinkedTo->Basic_Path(DestinationCoord.As_Cell(), 0, (around_blockage != NULL) + 1); // RA2: as above
					if (LinkedTo == NULL) { // RA2: as above
						stop_processing = true; // RA2: as above
						return(false); // RA2: as above
					}
					if (ok || LinkedTo->Is_In_Same_Zone(DestinationCoord)) { // RA2: as above
						LinkedTo->PathDelay = CDTimerClass<FrameTimerClass>((int)(Rule->PathDelay * TICKS_PER_MINUTE)); // RA2: as above
						return(true); // RA2: as above
					}
					LinkedTo->Assign_Destination(NULL, true); // RA2: as above
					return(false); // RA2: as above
				}
			}
			/*
			** If blocked by a moving block then just exit start of move and
			** try again next tick.
			*/
			if (cando == MOVE_DESTROYABLE || cando == MOVE_FRIENDLY_DESTROYABLE) { // RA2: as above
				if (retry) { // RA2: as above
					LinkedTo->Path[0] = FACING_NONE;		// Path is blocked! // RA2: as above
					LinkedTo->PathDelay = CDTimerClass<FrameTimerClass>(0); // RA2: as above
					return(Start_Of_Move(stop_processing, false, false)); // RA2: as above
				}

				/*
				 * An enemy unit, building, or wall is blocking. Pick a fight with it.
				 */
				if (Map[destcell].Cell_Object(Point2D(0, 0), 0)) { // RA2: as above
					if (!LinkedTo->House->Is_Ally(Map[destcell].Cell_Object(Point2D(0, 0), 0))) { // RA2: as above
						LinkedTo->Override_Mission(MISSION_ATTACK, Map[destcell].Cell_Object(Point2D(0, 0), 0), 0); // RA2: as above
					}
				} else {
					if (Map[destcell].Overlay != OVERLAY_NONE && OverlayTypes[Map[destcell].Overlay]->IsWall) { // RA2: as above
						LinkedTo->Override_Mission(MISSION_ATTACK, &Map[destcell], 0); // RA2: as above
					}
				}
			} else {
				if (LinkedTo->IsNewNavCom) VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000, 0); // RA2: as above
			}


			if (cando == MOVE_NO && retry) { // RA2: as above
				LinkedTo->Path[0] = FACING_NONE; // RA2: as above
				LinkedTo->PathDelay = CDTimerClass<FrameTimerClass>(0); // RA2: as above
				return(Start_Of_Move(stop_processing, false, false)); // RA2: as above
			} else if (cando == MOVE_NO && !retry) { // RA2: as above
				Stop_Driver(); // RA2: as above
				return(Abandon_Navigation()); // RA2: as above
			}

			LinkedTo->IsNewNavCom = false; // RA2: as above
			TrackNumber = -1; // RA2: as above
			return(true); // RA2: as above
		}

		/*
		 * Determine the speed that the unit can travel to the desired square.
		 * The terrain cost is a fractional multiplier (1.0 == full speed). It
		 * is clamped to a maximum of full speed and scaled according to whether
		 * the unit is travelling uphill or downhill.
		 */
		int height; // RA2: as above
		LandType ground; // RA2: as above
		if (abs(cell_height - Map[destcell].Height) < 2) { // RA2: as above
			height = Map[destcell].Height; // RA2: as above
			ground = Map[destcell].Land_Type(); // RA2: as above
		} else {
			height = cell_height; // RA2: as above
			ground = LAND_ROAD; // RA2: as above
		}

		TechnoTypeClass const * ttype = LinkedTo->TClass; // RA2: as above
		double speed = Ground[ground].Cost[ttype->Speed]; // RA2: as above
		if (speed > 1.0) speed = 1.0; // RA2: as above

		int destheight = Map.Get_Height_GL(Map[destcell].Cell_Coord()); // RA2: as above
		int unitheight = Map.Get_Height_GL(LinkedTo->PositionCoord); // RA2: as above
		if (destheight > unitheight && LinkedTo->Fetch_RTTI() == RTTI_UNIT) { // RA2: one condition per arm
			if (LinkedTo->TClass->Speed == SPEED_TRACK) { // RA2: as above
				speed *= Rule->TrackedUphill; // RA2: as above
			} else {
				speed *= Rule->WheeledUphill; // RA2: as above
			}
		} else if (destheight < unitheight && LinkedTo->Fetch_RTTI() == RTTI_UNIT) { // RA2: as above
			if (LinkedTo->TClass->Speed == SPEED_TRACK) { // RA2: as above
				speed *= Rule->TrackedDownhill; // RA2: as above
			} else {
				speed *= Rule->WheeledDownhill; // RA2: as above
			}
		}
		if (speed == 0.0) speed = 0.5; // RA2: as above

		/*
		**	A damaged unit has a reduced speed.
		*/
		if (LinkedTo->HealthRatio <= Rule->ConditionYellow) { // RA2: as above
			speed *= 0.75;	// Three quarters speed. // RA2: as above
		}
		if (TrackNumber < FACING_COUNT * FACING_COUNT) { // RA2: as above
			TargetSpeed = speed; // RA2: as above
		} else {
			if (speed != LinkedTo->Speed) { // RA2: as above
				LinkedTo->Set_Speed(speed);		// Full speed. // RA2: as above
			}
		}

		/*
		**	Reserve the destination cell so that it won't become
		**	occupied AS this unit is moving into it.
		*/
		LinkedTo->Overrun_Square(dest.As_Cell(), true); // RA2: as above

		/*
		**	Determine which track to use (based on recorded path).
		*/
		FacingType nextface = LinkedTo->Path[1]; // RA2: as above
		if (nextface == FACING_NONE) { // RA2: as above
			if (LinkedTo->Distance(DestinationCoord) > 2 * CELL_LEPTON_W) { // RA2: as above

				/*
				 * The end of the path was reached but the destination is still far away.
				 * Regenerate the path so that movement can continue.
				 */
				int patharg = LinkedTo->TClass->IsTrain ? 1 : 0; // RA2: as above
				bool found = LinkedTo->Basic_Path(DestinationCoord.As_Cell(), patharg, 0); // RA2: as above
				if (!found) { // RA2: as above
					if (LinkedTo == NULL) { // RA2: as above
						stop_processing = true; // RA2: as above
						return(found); // RA2: as above
					}
					if (!LinkedTo->Is_In_Same_Zone(DestinationCoord)) { // RA2: as above
						LinkedTo->Assign_Destination(NULL, true); // RA2: as above
					}
				}
				nextface = LinkedTo->Path[1]; // RA2: as above
			}
		}
		if (nextface == FACING_COUNT) nextface = facing; // RA2: as above
		if (nextface == FACING_NONE || force_straight) { // RA2: as above
			nextface = facing; // RA2: as above
		}

		/*
		 * If the cell that this unit would curve into contains a crushable
		 * overlay (such as a fence), then don't curve. Instead, drive straight
		 * through it and flag the unit as rocking from the crush.
		 */
		CellClass const * adjcell; // RA2: as above
		if (nextface != FACING_NONE && (adjcell = &destptr->Adjacent_Cell(nextface)) != 0 && adjcell->Overlay != OVERLAY_NONE && OverlayTypes[adjcell->Overlay]->IsCrushable || crushable_dest) { // RA2: as above
			nextface = facing; // RA2: as above
			IsRocking = true; // RA2: as above
		} else {
			IsRocking = false; // RA2: as above
		}

		IsOnShortTrack = false; // RA2: as above
		TrackNumber = facing * FACING_COUNT + nextface; // RA2: as above
		if (TrackControl[TrackNumber].Track == 0) { // RA2: as above
			TrackNumber = facing * FACING_COUNT + facing; // RA2: as above
		}

		if (TrackControl[TrackNumber].Flag & F_D) { // RA2: as above
			/*
			**	If the middle cell of a two cell track contains a crate,
			**	the check for goodies before movement starts.
			*/
			MoveType nextcando; // RA2: as above
			if (!Map[destcell].Goodie_Check(LinkedTo) && !LinkedTo->IsInLimbo) { // RA2: as above
				nextcando = MOVE_NO; // RA2: as above
				if (!LinkedTo->IsActive) return(false); // RA2: as above
			} else {
				if (!LinkedTo->IsActive) return(false); // RA2: as above
				dest = Adjacent_Cell(dest, nextface); // RA2: as above
				destcell = dest.As_Cell(); // RA2: as above
				nextcando = LinkedTo->Can_Enter_Cell(&Map[destcell], nextface, height, 0, true); // RA2: as above
				if (nextcando < MOVE_NO && LinkedTo->TClass->IsTrain) { // RA2: as above
					nextcando = MOVE_OK; // RA2: as above
				} else if ((nextcando == MOVE_FRIENDLY_DESTROYABLE || nextcando == MOVE_DESTROYABLE) && LinkedTo->TClass->IsCrusher && Map[destcell].Overlay == OVERLAY_SANDBAG_WALL) { // RA2: as above
					nextcando = MOVE_OK; // RA2: as above
				}
			}
			if (!LinkedTo->IsActive) return(false); // RA2: as above

			if (nextcando != MOVE_OK) { // RA2: as above

				if (nextcando == MOVE_CLOSED_GATE) { // RA2: as above
					Incoming(destcell); // RA2: as above
				}
				else if (nextcando == MOVE_MOVING_BLOCK) { // RA2: as above
					return(Start_Of_Move(stop_processing, retry, true)); // RA2: as above
				}

				/*
				**	If a temporary friendly object is blocking the path, then cause it to
				**	get out of the way.
				*/
				else if (nextcando == MOVE_TEMP) { // RA2: as above
					if (!LinkedTo->TClass->IsTrain) { // RA2: as above
						if (retry) { // RA2: as above
							LinkedTo->Path[0] = FACING_NONE; // RA2: as above
							LinkedTo->PathDelay = CDTimerClass<FrameTimerClass>(0); // RA2: as above
							return(Start_Of_Move(stop_processing, false, false)); // RA2: as above
						}
						if (LinkedTo->Distance(DestinationCoord) >= Rule->CloseEnoughDistance // RA2: as above
							|| abs(DestinationCoord.Z - LinkedTo->PositionCoord.Z) >= 2 * LEVEL_LEPTON_H // RA2: as above
							|| Map[LinkedTo->Get_Coord()].Land_Type() == LAND_TUNNEL) { // RA2: as above
							CellClass * cellptr = &Map[destcell]; // RA2: as above
							bool is_bridge = cellptr->IsUnderBridge && abs(LinkedTo->PositionCoord.Z / LEVEL_LEPTON_H - cellptr->Height) > 2; // RA2: as above
							cellptr->Incoming(COORD_NONE, true, true, is_bridge); // RA2: as above
						} else {
							Stop_Driver(); // RA2: as above
							if (Abandon_Navigation()) { // RA2: as above
								return(true); // RA2: as above
							}
						}
					}
				}

				/*
				**	If a cloaked object is blocking, then shimmer the cell.
				*/
				else if (nextcando == MOVE_CLOAK) { // RA2: as above
					Map[destcell].Shimmer(); // RA2: as above
					if (retry) { // RA2: as above
						LinkedTo->Path[0] = FACING_NONE; // RA2: as above
						return(Start_Of_Move(stop_processing, false, false)); // RA2: as above
					}
					Stop_Driver(); // RA2: as above
					return(Abandon_Navigation()); // RA2: as above
				}

				else if (nextcando == MOVE_NO) { // RA2: as above
					if (retry) { // RA2: as above
						LinkedTo->Path[0] = FACING_NONE;		// Path is blocked! // RA2: as above
						LinkedTo->PathDelay = CDTimerClass<FrameTimerClass>(0); // RA2: as above
						return(Start_Of_Move(stop_processing, false, false)); // RA2: as above
					}
					Stop_Driver(); // RA2: as above
					return(Abandon_Navigation()); // RA2: as above
				}

				/*
				 * A closed gate, a train-ignored or scattered temporary blockage,
				 * and an unhandled destroyable obstacle all fall through to here.
				 * Blow away the path and the current track. A destroyable obstacle
				 * gets one more pass (which picks the fight); everything else falls
				 * into the track-start logic below, which stops the unit.
				 */
				LinkedTo->Path[0] = FACING_NONE;		// Path is blocked! // RA2: as above
				TrackNumber = -1; // RA2: as above
				dest = COORD_NONE; // RA2: as above
				if (nextcando == MOVE_DESTROYABLE || nextcando == MOVE_FRIENDLY_DESTROYABLE) { // RA2: as above
					return(Start_Of_Move(stop_processing, retry, true)); // RA2: as above
				}

			} else {
				memcpy(&LinkedTo->Path[0], &LinkedTo->Path[2], sizeof(LinkedTo->Path[0]) * (CONQUER_PATH_MAX - 2)); // RA2: as above
				LinkedTo->Path[CONQUER_PATH_MAX-2] = FACING_NONE; // RA2: as above
				LinkedTo->IsPlanningToLook = true; // RA2: as above
			}
		} else {
			memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (CONQUER_PATH_MAX - 1)); // RA2: as above
		}

		LinkedTo->Path[CONQUER_PATH_MAX-1] = FACING_NONE; // RA2: as above
		LinkedTo->LastPathingCell = dest.As_Cell(); // RA2: as above
		LinkedTo->IsNewNavCom = false; // RA2: as above
		TrackIndex = 0; // RA2: as above
		if (!Start_Driver(dest)) { // RA2: as above
			TrackNumber = -1; // RA2: as above
			LinkedTo->Path[0] = FACING_NONE; // RA2: as above
			LinkedTo->Set_Speed(0); // RA2: as above
		}
	}
	return(false); // RA2: as above
}



/// <summary>
/// Handles the track laying logic for the unit. A ship leaves no track, so there is nothing to do.
/// </summary>
void ShipLocomotionClass::Lay_Track(void) // RA2: as above
{
	assert(LinkedTo->IsActive); // RA2: as above

#ifdef NEVER // RA2: as above
	static IconCommandType * _trackdirs[FACING_COUNT] = { // RA2: as above
		TrackN_S, // RA2: as above
		TrackNE_SW, // RA2: as above
		TrackE_W, // RA2: as above
		TrackNW_SE, // RA2: as above
		TrackN_S, // RA2: as above
		TrackNE_SW, // RA2: as above
		TrackE_W, // RA2: as above
		TrackNW_SE // RA2: as above
	};

	if (!(ClassF & CLASSF_TRACKS)) return; // RA2: as above

	Icon_Install(Coord_Cell(Coord), _trackdirs[Facing_To_8(BodyFacing)]); // RA2: as above
#endif // RA2: as above
}


/// <summary>
/// Works out the coordinate on a smooth turn track from its offset and facing.
/// </summary>
/// <param name="adj">The offset from the center of the destination cell, in track form.</param>
/// <param name="dir">The facing of the track at that point; it is flipped to match the offset.</param>
/// <returns>Returns with the point the ship is to be placed at.</returns>
Point2D ShipLocomotionClass::Smooth_Turn(Point2D const & adj, Dir256 & dir) // RA2: as above
{
	Dir256 workdir = dir; // RA2: as above
	int x,y; // RA2: as above
	int temp; // RA2: as above
	TrackControlType flags = TrackControl[TrackNumber].Flag; // RA2: as above

	x = adj.X; // RA2: as above
	y = adj.Y; // RA2: as above

	if (flags & F_T) { // RA2: as above
		temp	= x; // RA2: as above
		x		= y; // RA2: as above
		y 		= temp; // RA2: as above
		workdir = (Dir256)((DIR_W - workdir) & DIR_MAX); // RA2: as above
	}

	if (flags & F_X) { // RA2: as above
		x 		 = -x; // RA2: as above
		workdir = (Dir256)(-workdir & DIR_MAX); // RA2: as above
	}

	if (flags & F_Y) { // RA2: as above
		y = -y; // RA2: as above
		workdir = (Dir256)((DIR_S - workdir) & DIR_MAX); // RA2: as above
	}

	dir = workdir; // RA2: as above

	return(Point2D(HeadToCoord.X + x, HeadToCoord.Y + y)); // RA2: as above
}


/// <summary>
/// Warns the map that the unit is about to enter a cell.
/// This gives a friendly gate standing in that cell the chance to open before the
/// vehicle arrives at it.
/// </summary>
/// <param name="cell">The cell the unit is about to drive into.</param>
/// <returns>bool; Is the way into that cell clear?</returns>
bool ShipLocomotionClass::Incoming(Cell cell) // RA2: as above
{
	return(Map.Try_Open_Gate(LinkedTo, cell)); // RA2: as above
}


/// <summary>
/// Fetches the display layer the driving unit belongs to.
/// </summary>
/// <returns>Returns with LAYER_GROUND, since a driving unit travels on the ground.</returns>
LayerType STDMETHODCALLTYPE ShipLocomotionClass::In_Which_Layer(void) // RA2: as above
{
	return(LAYER_GROUND); // RA2: as above
}


/// <summary>
/// Fetches the class identifier of this locomotor.
/// The persistence system uses this to know which locomotor to create when the unit is
/// loaded back in.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT STDMETHODCALLTYPE ShipLocomotionClass::GetClassID(CLSID * retval) // RA2: as above
{
	if (retval == NULL) return(E_POINTER); // RA2: as above
	*retval = CLSID_ShipLocomotion;
	return(S_OK); // RA2: as above
}


/// <summary>
/// Fetches the depth adjustment for the driving unit.
/// A driving vehicle sits at the depth of the ground it is standing on.
/// </summary>
/// <returns>Returns with the adjustment to apply to the unit's draw depth.</returns>
int STDMETHODCALLTYPE ShipLocomotionClass::Z_Adjust(void) // RA2: as above
{
	return(0); // RA2: as above
}


/// <summary>
/// Fetches the depth gradient the unit is to be drawn with.
/// </summary>
/// <returns>Returns with the gradient the base locomotor asks for.</returns>
ZGradientType STDMETHODCALLTYPE ShipLocomotionClass::Z_Gradient(void) // RA2: as above
{
	return(BASECLASS::Z_Gradient()); // RA2: as above
}


/// <summary>
/// Abandons the unit's current navigation order.
/// If the unit still has legs of a route queued up, it is dropped into its idle mode so
/// that it can take up the next one. Otherwise it is simply left with nowhere to go.
/// </summary>
/// <returns>bool; Did the unit take up a new mission?</returns>
bool ShipLocomotionClass::Abandon_Navigation(void) // RA2: as above
{
	if (LinkedTo->RouteQueue.Count() == 0) { // RA2: as above
		LinkedTo->Assign_Destination(0, true); // RA2: RA2's form
		return(false); // RA2: as above
	}
	LinkedTo->Abort_Motion(); // RA2: as above
	return LinkedTo->Enter_Idle_Mode(false, true); // RA2: as above
}



/// <summary>
/// Marks or removes the unit's occupation of the cells it is driving through.
/// A vehicle between cells lays claim to more than the one it stands in, so the map must
/// be told about every cell of the track it is committed to.
/// </summary>
/// <param name="mark">The MarkType to apply to the cells occupied.</param>
void STDMETHODCALLTYPE ShipLocomotionClass::Mark_All_Occupation_Bits(int mark) // RA2: as above
{
	if (HeadToCoord != COORD_NONE) { // RA2: as above
		Mark_Track(HeadToCoord, (MarkType)mark); // RA2: as above
	}
}


/// <summary>
/// Is the driver headed for the specified location?
/// The cell the unit is bound for counts, and so does the cell it is about to reach at
/// its next per cell process point, so that a vehicle part way around a turn is still
/// recognized as moving into the cell it is committed to.
/// </summary>
/// <param name="to">The location to test against.</param>
/// <returns>bool; Is the unit moving there?</returns>
boolean STDMETHODCALLTYPE ShipLocomotionClass::Is_Moving_Here(CoordStruct to) // RA2: as above
{
	Coord coord = Head_To_Coord(); // RA2: as above

	if (coord != COORD_NONE) { // RA2: as above
		if (!IsOnShortTrack && TrackNumber != -1) { // RA2: as above
			int tracknum = TrackControl[TrackNumber].Track; // RA2: as above
			if (tracknum) { // RA2: as above
				TrackType const * ptr = RawTracks[tracknum - 1].Track; // RA2: as above
				int cellidx = RawTracks[tracknum - 1].Cell; // RA2: as above
				if (cellidx > -1) { // RA2: as above
					Dir256 dir = ptr[cellidx].Facing; // RA2: as above

					if (TrackIndex < cellidx && cellidx != -1) { // RA2: as above
						Point2D pt = Smooth_Turn(ptr[cellidx].Offset, dir); // RA2: as above
						Coord coord = LinkedTo->PositionCoord; // RA2: as YR's; TibSun's form does not match (Drive's and Ship's copies match only so)
						coord.X = pt.X; // RA2: as YR's; TibSun's form does not match (Drive's and Ship's copies match only so)
						coord.Y = pt.Y; // RA2: as YR's; TibSun's form does not match (Drive's and Ship's copies match only so)
						if (coord.As_Cell() == ((Coord &)to).As_Cell() && abs(coord.Z - to.Z) <= LEVEL_LEPTON_H) { // RA2: as above
							return(true); // RA2: as above
						}
					}
				}
			}
		}

		if (coord.As_Cell() == ((Coord &)to).As_Cell() && abs(coord.Z - to.Z) <= LEVEL_LEPTON_H) { // RA2: as above
			return(true); // RA2: as above
		}
	}

	return(false); // RA2: as above
}


/// <summary>
/// Is the unit about to jump onto another track?
/// A turn queued up in the path can be taken smoothly by hopping from the track the unit
/// is on to one that curves toward the new facing. This routine tells the caller that
/// such a hop is due, but performs none of it.
/// </summary>
/// <returns>bool; Will the driver jump tracks?</returns>
boolean STDMETHODCALLTYPE ShipLocomotionClass::Will_Jump_Tracks(void) // RA2: as above
{
	/// This repeats the track jump test that While_Moving performs.
	assert(LinkedTo->IsActive); // RA2: as above

	TurnTrackType	const * track;	// Track control pointer. // RA2: as above
	int				tracknum;		// The track number being processed. // RA2: as above
	FacingType		nextface;		// Next facing queued in path. // RA2: as above
	bool				adj;		// Is a turn coming up? // RA2: as above

	nextface = LinkedTo->Path[0]; // RA2: as above

	if (nextface >= FACING_NONE && nextface <= FACING_COUNT) { // RA2: as above

		track = &TrackControl[TrackNumber]; // RA2: as above
		if (IsOnShortTrack) { // RA2: as above
			tracknum = track->StartTrack; // RA2: as above
		} else {
			tracknum = track->Track; // RA2: as above
		}

		/*
		**	Determine if there is a turn coming up. If there is
		**	a turn, then track jumping might occur.
		*/
		adj = false; // RA2: as above
		if (nextface != FACING_COUNT && nextface != FACING_NONE && Dir_Facing(track->Facing) != nextface) { // RA2: as above
			adj = true; // RA2: as above
		}

		/*
		**	The unit could "jump tracks". Check to see if the unit should
		**	do so.
		*/
		if (nextface != FACING_COUNT && nextface != FACING_NONE && adj && RawTracks[tracknum-1].Jump == TrackIndex && TrackIndex) { // RA2: as above
			TurnTrackType const * newtrack;		// Proposed jump-to track. // RA2: as above
			int	tnum; // RA2: as above

			tnum = (int)(Dir_Facing(track->Facing) * FACING_COUNT) + (int)nextface; // RA2: as above
			newtrack = &TrackControl[tnum]; // RA2: as above
			if (newtrack->Track && RawTracks[newtrack->Track-1].Entry) { // RA2: as above
				return(true); // RA2: as above
			}
		}
	}
	return(false); // RA2: as above
}


/// <summary>
/// Locks the driver against being handed back.
/// While locked, this driver will not report itself ready to end a piggyback, so a
/// temporary locomotor riding on top of it keeps control of the unit.
/// </summary>
void STDMETHODCALLTYPE ShipLocomotionClass::Lock(void) // RA2: as above
{
	IsLocomotorUnlocked = false; // RA2: as above
}


/// <summary>
/// Unlocks the driver so that it may be handed back.
/// This is the counterpart to Lock. The driver may once again report itself ready to
/// end a piggyback.
/// </summary>
void STDMETHODCALLTYPE ShipLocomotionClass::Unlock(void) // RA2: as above
{
	IsLocomotorUnlocked = true; // RA2: as above
}


/// <summary>
/// Fetches the turn track the unit is currently following.
/// </summary>
/// <returns>Returns with the track control number, or -1 if the unit is not on a
/// track.</returns>
int STDMETHODCALLTYPE ShipLocomotionClass::Get_Track_Number(void) // RA2: as above
{
	return(TrackNumber); // RA2: as above
}


/// <summary>
/// Fetches how far along its track the unit has traveled.
/// </summary>
/// <returns>Returns with the index into the track the unit has reached, or -1 if the
/// unit is not following one.</returns>
int STDMETHODCALLTYPE ShipLocomotionClass::Get_Track_Index(void) // RA2: as above
{
	return(TrackIndex); // RA2: as above
}


/// <summary>
/// Fetches the movement the driver has banked up along its track.
/// </summary>
/// <returns>Returns with the accumulated movement not yet spent advancing the unit.</returns>
int STDMETHODCALLTYPE ShipLocomotionClass::Get_Speed_Accum(void) // RA2: as above
{
	return(SpeedAccum); // RA2: as above
}


/// <summary>
/// Adds a reference to this locomotor.
/// </summary>
/// <returns>Returns with the reference count once the new reference is counted.</returns>
ULONG STDMETHODCALLTYPE ShipLocomotionClass::AddRef(void) // RA2: as above
{
	return(BASECLASS::AddRef()); // RA2: as above
}


/// <summary>
/// Releases a reference to this locomotor.
/// </summary>
/// <returns>Returns with the reference count remaining after the release.</returns>
ULONG STDMETHODCALLTYPE ShipLocomotionClass::Release(void) // RA2: as above
{
	return(BASECLASS::Release()); // RA2: as above
}



/***************************************************************************
**	Smooth turn track tables. These are coordinate offsets from the center
**	of the destination cell. These are the raw tracks that are modified
**	by negating the X and Y portions as necessary. Also for reverse travelling
**	direction, the track list can be processed backward.
**
**	Track 1 = N
**	Track 2 = NE
**	Track 3 = N->NE 45 deg (double path consumption)
**	Track 4 = N->E 90 deg (double path consumption)
**	Track 5 = NE->SE 90 deg (double path consumption)
**	Track 6 = NE->N 45 deg (double path consumption)
**	Track 7 = N->NE (facing change only)
**	Track 8 = NE->E (facing change only)
**	Track 9 = N->E (facing change only)
**	Track 10= NE->SE (facing change only)
**	Track 11= back up into refinery
**	Track 12= drive out of refinery
*/
ShipLocomotionClass::TrackType const ShipLocomotionClass::Track1[24] = { // RA2: as above
	{{0,245},(Dir256)0}, // RA2: as above
	{{0,234},(Dir256)0}, // RA2: as above
	{{0,223},(Dir256)0}, // RA2: as above
	{{0,212},(Dir256)0}, // RA2: as above
	{{0,201},(Dir256)0}, // RA2: as above
	{{0,190},(Dir256)0}, // RA2: as above
	{{0,179},(Dir256)0}, // RA2: as above
	{{0,168},(Dir256)0}, // RA2: as above
	{{0,157},(Dir256)0}, // RA2: as above
	{{0,146},(Dir256)0}, // RA2: as above
	{{0,135},(Dir256)0}, // RA2: as above
	{{0,124},(Dir256)0},		// Track jump check here. // RA2: as above
	{{0,113},(Dir256)0}, // RA2: as above
	{{0,102},(Dir256)0}, // RA2: as above
	{{0,91},(Dir256)0}, // RA2: as above
	{{0,80},(Dir256)0}, // RA2: as above
	{{0,69},(Dir256)0}, // RA2: as above
	{{0,58},(Dir256)0}, // RA2: as above
	{{0,47},(Dir256)0}, // RA2: as above
	{{0,36},(Dir256)0}, // RA2: as above
	{{0,25},(Dir256)0}, // RA2: as above
	{{0,14},(Dir256)0}, // RA2: as above
	{{0,3},(Dir256)0}, // RA2: as above
	{{0,0},(Dir256)0} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track2[] = { // RA2: as above
	{{-248,248},(Dir256)32}, // RA2: as above
	{{-240,240},(Dir256)32}, // RA2: as above
	{{-232,232},(Dir256)32}, // RA2: as above
	{{-224,224},(Dir256)32}, // RA2: as above
	{{-216,216},(Dir256)32}, // RA2: as above
	{{-208,208},(Dir256)32}, // RA2: as above
	{{-200,200},(Dir256)32}, // RA2: as above
	{{-192,192},(Dir256)32}, // RA2: as above
	{{-184,184},(Dir256)32}, // RA2: as above
	{{-176,176},(Dir256)32}, // RA2: as above
	{{-168,168},(Dir256)32}, // RA2: as above
	{{-160,160},(Dir256)32}, // RA2: as above
	{{-152,152},(Dir256)32}, // RA2: as above
	{{-144,144},(Dir256)32}, // RA2: as above
	{{-136,136},(Dir256)32}, // RA2: as above
	{{-129,129},(Dir256)32},		// Track jump check here. // RA2: as above
	{{-120,120},(Dir256)32}, // RA2: as above
	{{-112,112},(Dir256)32}, // RA2: as above
	{{-104,104},(Dir256)32}, // RA2: as above
	{{-96,96},(Dir256)32}, // RA2: as above
	{{-88,88},(Dir256)32}, // RA2: as above
	{{-80,80},(Dir256)32}, // RA2: as above
	{{-72,72},(Dir256)32}, // RA2: as above
	{{-64,64},(Dir256)32}, // RA2: as above
	{{-56,56},(Dir256)32}, // RA2: as above
	{{-48,48},(Dir256)32}, // RA2: as above
	{{-40,40},(Dir256)32}, // RA2: as above
	{{-32,32},(Dir256)32}, // RA2: as above
	{{-24,24},(Dir256)32}, // RA2: as above
	{{-16,16},(Dir256)32}, // RA2: as above
	{{-8,8},(Dir256)32}, // RA2: as above
	{{0,0},(Dir256)32} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track3[] = { // RA2: as above
	{{-256,501},(Dir256)0}, // RA2: as above
	{{-256,490},(Dir256)0}, // RA2: as above
	{{-256,479},(Dir256)0}, // RA2: as above
	{{-256,468},(Dir256)0}, // RA2: as above
	{{-256,457},(Dir256)0}, // RA2: as above
	{{-256,446},(Dir256)0}, // RA2: as above
	{{-256,435},(Dir256)0}, // RA2: as above
	{{-256,424},(Dir256)0}, // RA2: as above
	{{-256,413},(Dir256)0}, // RA2: as above
	{{-256,402},(Dir256)0}, // RA2: as above
	{{-256,391},(Dir256)0}, // RA2: as above
	{{-256,383},(Dir256)0}, // RA2: as above
	{{-256,373},(Dir256)0},		// Jump entry point here. // RA2: as above
	{{-256,363},(Dir256)0}, // RA2: as above
	{{-254,352},(Dir256)1}, // RA2: as above
	{{-252,341},(Dir256)3}, // RA2: as above
	{{-250,332},(Dir256)4}, // RA2: as above
	{{-248,321},(Dir256)5}, // RA2: as above
	{{-245,311},(Dir256)7}, // RA2: as above
	{{-241,302},(Dir256)8}, // RA2: as above
	{{-237,292},(Dir256)9}, // RA2: as above
	{{-233,282},(Dir256)11}, // RA2: as above
	{{-229,272},(Dir256)12}, // RA2: as above
	{{-225,263},(Dir256)13},		// Center cell processing here. // RA2: as above
	{{-220,252},(Dir256)15}, // RA2: as above
	{{-216,243},(Dir256)16}, // RA2: as above
	{{-212,236},(Dir256)17}, // RA2: as above
	{{-206,224},(Dir256)19}, // RA2: as above
	{{-202,215},(Dir256)20}, // RA2: as above
	{{-195,207},(Dir256)21}, // RA2: as above
	{{-190,198},(Dir256)23}, // RA2: as above
	{{-183,186},(Dir256)24}, // RA2: as above
	{{-179,176},(Dir256)25}, // RA2: as above
	{{-168,168},(Dir256)27}, // RA2: as above
	{{-160,160},(Dir256)28}, // RA2: as above
	{{-152,152},(Dir256)29}, // RA2: as above
	{{-144,144},(Dir256)31}, // RA2: as above
	{{-136,136},(Dir256)32}, // RA2: as above
	{{-129,129},(Dir256)32},		// Track jump check here. // RA2: as above
	{{-120,120},(Dir256)32}, // RA2: as above
	{{-112,112},(Dir256)32}, // RA2: as above
	{{-104,104},(Dir256)32}, // RA2: as above
	{{-96,96},(Dir256)32}, // RA2: as above
	{{-88,88},(Dir256)32}, // RA2: as above
	{{-80,80},(Dir256)32}, // RA2: as above
	{{-72,72},(Dir256)32}, // RA2: as above
	{{-64,64},(Dir256)32}, // RA2: as above
	{{-56,56},(Dir256)32}, // RA2: as above
	{{-48,48},(Dir256)32}, // RA2: as above
	{{-40,40},(Dir256)32}, // RA2: as above
	{{-32,32},(Dir256)32}, // RA2: as above
	{{-24,24},(Dir256)32}, // RA2: as above
	{{-16,16},(Dir256)32}, // RA2: as above
	{{-8,8},(Dir256)32}, // RA2: as above
	{{0,0},(Dir256)32} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track4[] = { // RA2: as above
	{{-256,245},(Dir256)0}, // RA2: as above
	{{-256,235},(Dir256)0}, // RA2: as above
	{{-256,224},(Dir256)0}, // RA2: as above
	{{-256,213},(Dir256)0}, // RA2: as above
	{{-255,203},(Dir256)0}, // RA2: as above
	{{-253,192},(Dir256)0}, // RA2: as above
	{{-251,181},(Dir256)1}, // RA2: as above
	{{-249,171},(Dir256)1}, // RA2: as above
	{{-246,160},(Dir256)2}, // RA2: as above
	{{-243,149},(Dir256)3}, // RA2: as above
	{{-240,139},(Dir256)4}, // RA2: as above
	{{-236,127},(Dir256)5},		// Track entry here. // RA2: as above
	{{-232,117},(Dir256)8}, // RA2: as above
	{{-228,109},(Dir256)12}, // RA2: as above
	{{-222,99},(Dir256)16}, // RA2: as above
	{{-219,90},(Dir256)20}, // RA2: as above
	{{-213,82},(Dir256)23}, // RA2: as above
	{{-206,72},(Dir256)27}, // RA2: as above
	{{-201,64},(Dir256)32}, // RA2: as above
	{{-195,56},(Dir256)36}, // RA2: as above
	{{-186,48},(Dir256)39}, // RA2: as above
	{{-177,43},(Dir256)43}, // RA2: as above
	{{-168,36},(Dir256)47}, // RA2: as above
	{{-160,32},(Dir256)51}, // RA2: as above
	{{-147,27},(Dir256)54}, // RA2: as above
	{{-135,23},(Dir256)57}, // RA2: as above
	{{-126,20},(Dir256)60},		// Track jump here. // RA2: as above
	{{-113,17},(Dir256)62}, // RA2: as above
	{{-104,13},(Dir256)63}, // RA2: as above
	{{-94,9},(Dir256)64}, // RA2: as above
	{{-84,6},(Dir256)64}, // RA2: as above
	{{-75,4},(Dir256)66}, // RA2: as above
	{{-64,3},(Dir256)64}, // RA2: as above
	{{-53,2},(Dir256)64}, // RA2: as above
	{{-43,1},(Dir256)64}, // RA2: as above
	{{-32,0},(Dir256)64}, // RA2: as above
	{{-21,0},(Dir256)64}, // RA2: as above
	{{-11,0},(Dir256)64}, // RA2: as above
	{{0,0},(Dir256)64} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track5[] = { // RA2: as above
	{{-504,-8},(Dir256)32}, // RA2: as above
	{{-496,-16},(Dir256)32}, // RA2: as above
	{{-488,-24},(Dir256)32}, // RA2: as above
	{{-480,-32},(Dir256)32}, // RA2: as above
	{{-472,-40},(Dir256)32}, // RA2: as above
	{{-464,-48},(Dir256)32}, // RA2: as above
	{{-456,-56},(Dir256)32}, // RA2: as above
	{{-448,-64},(Dir256)32}, // RA2: as above
	{{-440,-72},(Dir256)32}, // RA2: as above
	{{-432,-80},(Dir256)32}, // RA2: as above
	{{-424,-88},(Dir256)32}, // RA2: as above
	{{-416,-96},(Dir256)32}, // RA2: as above
	{{-408,-104},(Dir256)32}, // RA2: as above
	{{-400,-112},(Dir256)32}, // RA2: as above
	{{-392,-120},(Dir256)32}, // RA2: as above
	{{-385,-127},(Dir256)32},		// Track entry here. // RA2: as above
	{{-376,-136},(Dir256)32}, // RA2: as above
	{{-368,-143},(Dir256)32}, // RA2: as above
	{{-361,-150},(Dir256)32}, // RA2: as above
	{{-353,-158},(Dir256)32}, // RA2: as above
	{{-344,-166},(Dir256)32}, // RA2: as above
	{{-336,-173},(Dir256)35}, // RA2: as above
	{{-329,-181},(Dir256)38}, // RA2: as above
	{{-322,-188},(Dir256)41}, // RA2: as above
	{{-316,-194},(Dir256)44}, // RA2: as above
	{{-306,-199},(Dir256)47}, // RA2: as above
	{{-296,-204},(Dir256)50}, // RA2: as above
	{{-288,-208},(Dir256)53}, // RA2: as above
	{{-277,-211},(Dir256)56}, // RA2: as above
	{{-267,-212},(Dir256)59}, // RA2: as above
	{{-256,-213},(Dir256)62}, // RA2: as above
	{{-245,-212},(Dir256)66}, // RA2: as above
	{{-235,-211},(Dir256)69}, // RA2: as above
	{{-225,-208},(Dir256)72}, // RA2: as above
	{{-216,-204},(Dir256)75}, // RA2: as above
	{{-208,-199},(Dir256)78}, // RA2: as above
	{{-198,-194},(Dir256)81}, // RA2: as above
	{{-188,-188},(Dir256)84}, // RA2: as above
	{{-181,-181},(Dir256)87}, // RA2: as above
	{{-176,-173},(Dir256)90}, // RA2: as above
	{{-168,-166},(Dir256)93}, // RA2: as above
	{{-160,-158},(Dir256)96}, // RA2: as above
	{{-152,-150},(Dir256)96}, // RA2: as above
	{{-144,-143},(Dir256)96}, // RA2: as above
	{{-136,-136},(Dir256)96}, // RA2: as above
	{{-129,-129},(Dir256)96},		// Track jump check here. // RA2: as above
	{{-120,-120},(Dir256)96}, // RA2: as above
	{{-112,-112},(Dir256)96}, // RA2: as above
	{{-104,-104},(Dir256)96}, // RA2: as above
	{{-96,-96},(Dir256)96}, // RA2: as above
	{{-88,-88},(Dir256)96}, // RA2: as above
	{{-80,-80},(Dir256)96}, // RA2: as above
	{{-72,-72},(Dir256)96}, // RA2: as above
	{{-64,-64},(Dir256)96}, // RA2: as above
	{{-56,-56},(Dir256)96}, // RA2: as above
	{{-48,-48},(Dir256)96}, // RA2: as above
	{{-40,-40},(Dir256)96}, // RA2: as above
	{{-32,-32},(Dir256)96}, // RA2: as above
	{{-24,-24},(Dir256)96}, // RA2: as above
	{{-16,-16},(Dir256)96}, // RA2: as above
	{{-8,-8},(Dir256)96}, // RA2: as above
	{{0,0},(Dir256)96} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track6[] = { // RA2: as above
	{{-512,256},(Dir256)32}, // RA2: as above
	{{-504,248},(Dir256)32}, // RA2: as above
	{{-496,240},(Dir256)32}, // RA2: as above
	{{-488,232},(Dir256)32}, // RA2: as above
	{{-480,224},(Dir256)32}, // RA2: as above
	{{-472,216},(Dir256)32}, // RA2: as above
	{{-464,208},(Dir256)32}, // RA2: as above
	{{-456,200},(Dir256)32}, // RA2: as above
	{{-448,192},(Dir256)32}, // RA2: as above
	{{-440,184},(Dir256)32}, // RA2: as above
	{{-432,176},(Dir256)32}, // RA2: as above
	{{-424,168},(Dir256)32}, // RA2: as above
	{{-416,160},(Dir256)32}, // RA2: as above
	{{-408,152},(Dir256)32}, // RA2: as above
	{{-400,144},(Dir256)32}, // RA2: as above
	{{-392,136},(Dir256)32}, // RA2: as above
	{{-385,129},(Dir256)32},		// Jump entry point here. // RA2: as above
	{{-376,120},(Dir256)32}, // RA2: as above
	{{-368,112},(Dir256)32}, // RA2: as above
	{{-360,104},(Dir256)32}, // RA2: as above
	{{-352,96},(Dir256)32}, // RA2: as above
	{{-344,88},(Dir256)32}, // RA2: as above
	{{-338,85},(Dir256)32}, // RA2: as above
	{{-328,78},(Dir256)35}, // RA2: as above
	{{-320,72},(Dir256)37}, // RA2: as above
	{{-311,66},(Dir256)40}, // RA2: as above
	{{-302,59},(Dir256)43}, // RA2: as above
	{{-294,55},(Dir256)45}, // RA2: as above
	{{-285,50},(Dir256)48}, // RA2: as above
	{{-277,43},(Dir256)51}, // RA2: as above
	{{-267,38},(Dir256)53}, // RA2: as above
	{{-258,34},(Dir256)56}, // RA2: as above
	{{-248,28},(Dir256)59}, // RA2: as above
	{{-238,25},(Dir256)61}, // RA2: as above
	{{-229,21},(Dir256)64}, // RA2: as above
	{{-218,17},(Dir256)64}, // RA2: as above
	{{-208,14},(Dir256)64}, // RA2: as above
	{{-199,11},(Dir256)64}, // RA2: as above
	{{-189,9},(Dir256)64}, // RA2: as above
	{{-178,7},(Dir256)64}, // RA2: as above
	{{-169,5},(Dir256)64}, // RA2: as above
	{{-158,3},(Dir256)64}, // RA2: as above
	{{-147,1},(Dir256)64}, // RA2: as above
	{{-137,0},(Dir256)64}, // RA2: as above
	{{-129,0},(Dir256)64},		// Track jump check here. // RA2: as above
	{{-117,0},(Dir256)64}, // RA2: as above
	{{-107,0},(Dir256)64}, // RA2: as above
	{{-96,0},(Dir256)64}, // RA2: as above
	{{-85,0},(Dir256)64}, // RA2: as above
	{{-75,0},(Dir256)64}, // RA2: as above
	{{-64,0},(Dir256)64}, // RA2: as above
	{{-53,0},(Dir256)64}, // RA2: as above
	{{-43,0},(Dir256)64}, // RA2: as above
	{{-32,0},(Dir256)64}, // RA2: as above
	{{-21,0},(Dir256)64}, // RA2: as above
	{{-11,0},(Dir256)64}, // RA2: as above
	{{0,0},(Dir256)64} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track7[] = { // RA2: as above
	{{-1,6},(Dir256)0}, // RA2: as above
	{{-2,12},(Dir256)4}, // RA2: as above
	{{-4,17},(Dir256)8}, // RA2: as above
	{{-6,24},(Dir256)12}, // RA2: as above
	{{-10,31},(Dir256)16}, // RA2: as above
	{{-13,36},(Dir256)19}, // RA2: as above
	{{-16,43},(Dir256)22}, // RA2: as above
	{{-3,48},(Dir256)23}, // RA2: as above
	{{-21,53},(Dir256)24}, // RA2: as above
	{{-24,56},(Dir256)25}, // RA2: as above
	{{-26,60},(Dir256)26}, // RA2: as above
	{{-29,64},(Dir256)27}, // RA2: as above
	{{-32,67},(Dir256)28}, // RA2: as above
	{{-35,70},(Dir256)29}, // RA2: as above
	{{-33,67},(Dir256)30}, // RA2: as above
	{{-31,64},(Dir256)30}, // RA2: as above
	{{-29,60},(Dir256)30}, // RA2: as above
	{{-27,56},(Dir256)30}, // RA2: as above
	{{-25,53},(Dir256)31}, // RA2: as above
	{{-23,48},(Dir256)31}, // RA2: as above
	{{-21,43},(Dir256)31}, // RA2: as above
	{{-19,36},(Dir256)31}, // RA2: as above
	{{-15,31},(Dir256)31}, // RA2: as above
	{{-12,24},(Dir256)32}, // RA2: as above
	{{-9,17},(Dir256)32}, // RA2: as above
	{{-6,12},(Dir256)32}, // RA2: as above
	{{-3,6},(Dir256)32}, // RA2: as above
	{{0,0},(Dir256)32} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track8[] = { // RA2: as above
	{{-4,3},(Dir256)32}, // RA2: as above
	{{-9,6},(Dir256)36}, // RA2: as above
	{{-15,10},(Dir256)40}, // RA2: as above
	{{-21,12},(Dir256)44}, // RA2: as above
	{{-28,13},(Dir256)46}, // RA2: as above
	{{-36,14},(Dir256)48}, // RA2: as above
	{{-43,15},(Dir256)50}, // RA2: as above
	{{-48,16},(Dir256)52}, // RA2: as above
	{{-55,17},(Dir256)54}, // RA2: as above
	{{-62,18},(Dir256)56}, // RA2: as above
	{{-64,17},(Dir256)58}, // RA2: as above
	{{-62,16},(Dir256)60}, // RA2: as above
	{{-55,14},(Dir256)62}, // RA2: as above
	{{-49,12},(Dir256)64}, // RA2: as above
	{{-43,10},(Dir256)64}, // RA2: as above
	{{-38,8},(Dir256)64}, // RA2: as above
	{{-30,6},(Dir256)64}, // RA2: as above
	{{-23,4},(Dir256)64}, // RA2: as above
	{{-17,2},(Dir256)64}, // RA2: as above
	{{-11,1},(Dir256)64}, // RA2: as above
	{{-7,0},(Dir256)64}, // RA2: as above
	{{0,0},(Dir256)64} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track9[] = { // RA2: as above
	{{2,-11},(Dir256)0}, // RA2: as above
	{{4,-21},(Dir256)2}, // RA2: as above
	{{6,-32},(Dir256)4}, // RA2: as above
	{{9,-43},(Dir256)6}, // RA2: as above
	{{12,-50},(Dir256)9}, // RA2: as above
	{{15,-56},(Dir256)11}, // RA2: as above
	{{18,-64},(Dir256)13}, // RA2: as above
	{{21,-72},(Dir256)16}, // RA2: as above
	{{18,-64},(Dir256)18}, // RA2: as above
	{{14,-56},(Dir256)20}, // RA2: as above
	{{10,-50},(Dir256)22}, // RA2: as above
	{{4,-43},(Dir256)24}, // RA2: as above
	{{0,-34},(Dir256)26}, // RA2: as above
	{{-8,-23},(Dir256)28}, // RA2: as above
	{{-14,-18},(Dir256)30}, // RA2: as above
	{{-21,-11},(Dir256)32}, // RA2: as above
	{{-31,-3},(Dir256)34}, // RA2: as above
	{{-40,2},(Dir256)36}, // RA2: as above
	{{-46,7},(Dir256)39}, // RA2: as above
	{{-53,11},(Dir256)41}, // RA2: as above
	{{-59,16},(Dir256)43}, // RA2: as above
	{{-66,19},(Dir256)45}, // RA2: as above
	{{-73,21},(Dir256)48}, // RA2: as above
	{{-66,19},(Dir256)50}, // RA2: as above
	{{-59,17},(Dir256)52}, // RA2: as above
	{{-52,11},(Dir256)54}, // RA2: as above
	{{-44,8},(Dir256)56}, // RA2: as above
	{{-33,5},(Dir256)58}, // RA2: as above
	{{-21,3},(Dir256)62}, // RA2: as above
	{{-11,1},(Dir256)64}, // RA2: as above
	{{0,0},(Dir256)64} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track10[] = { // RA2: as above
	{{11,-10},(Dir256)32}, // RA2: as above
	{{21,-16},(Dir256)37}, // RA2: as above
	{{32,-21},(Dir256)42}, // RA2: as above
	{{43,-23},(Dir256)47}, // RA2: as above
	{{50,-27},(Dir256)52}, // RA2: as above
	{{56,-29},(Dir256)57}, // RA2: as above
	{{64,-32},(Dir256)60}, // RA2: as above
	{{56,-30},(Dir256)62}, // RA2: as above
	{{50,-28},(Dir256)64}, // RA2: as above
	{{42,-27},(Dir256)68}, // RA2: as above
	{{30,-26},(Dir256)70}, // RA2: as above
	{{21,-25},(Dir256)72}, // RA2: as above
	{{11,-24},(Dir256)74}, // RA2: as above
	{{0,-23},(Dir256)76}, // RA2: as above
	{{-11,-24},(Dir256)78}, // RA2: as above
	{{-21,-25},(Dir256)80}, // RA2: as above
	{{-32,-26},(Dir256)82}, // RA2: as above
	{{-43,-27},(Dir256)84}, // RA2: as above
	{{-50,-28},(Dir256)86}, // RA2: as above
	{{-59,-30},(Dir256)88}, // RA2: as above
	{{-64,-32},(Dir256)90}, // RA2: as above
	{{-59,-29},(Dir256)92}, // RA2: as above
	{{-50,-27},(Dir256)94}, // RA2: as above
	{{-43,-23},(Dir256)95}, // RA2: as above
	{{-32,-21},(Dir256)96}, // RA2: as above
	{{-21,-16},(Dir256)96}, // RA2: as above
	{{-11,-10},(Dir256)96}, // RA2: as above
	{{0,0},(Dir256)96} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track11[] = { // RA2: as above
	{{0,256},DIR_SW}, // RA2: as above
	{{8,243},DIR_SW}, // RA2: as above
	{{16,229},DIR_SW}, // RA2: as above
	{{24,214},DIR_SW}, // RA2: as above
	{{32,200},DIR_SW}, // RA2: as above
	{{40,185},DIR_SW}, // RA2: as above
	{{48,171},DIR_SW}, // RA2: as above
	{{56,156},DIR_SW}, // RA2: as above
	{{64,141},DIR_SW}, // RA2: as above
	{{72,127},DIR_SW}, // RA2: as above
	{{80,113},DIR_SW}, // RA2: as above
	{{88,100},DIR_SW}, // RA2: as above
	{{96,85},DIR_SW}, // RA2: as above

	{{0,0},DIR_SW} // RA2: as above
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track12[] = { // RA2: as above
	{{96,-171},DIR_SW}, // RA2: as above
	{{88,-156},DIR_SW}, // RA2: as above
	{{80,-143},DIR_SW}, // RA2: as above
	{{72,-129},DIR_SW}, // RA2: as above
	{{64,-115},DIR_SW}, // RA2: as above
	{{56,-100},DIR_SW}, // RA2: as above
	{{48,-85},DIR_SW}, // RA2: as above
	{{40,-71},DIR_SW}, // RA2: as above
	{{32,-56},DIR_SW}, // RA2: as above
	{{24,-42},DIR_SW}, // RA2: as above
	{{16,-27},DIR_SW}, // RA2: as above
	{{8,-13},DIR_SW}, // RA2: as above

	{{0,0},DIR_SW} // RA2: as above
};

/*
**	Drive out of weapon's factory.
*/
ShipLocomotionClass::TrackType const ShipLocomotionClass::Track13[] = { // RA2: as above
	{{-670,-68},DIR_E}, // RA2: as above
	{{-660,-67},DIR_E}, // RA2: as above
	{{-650,-66},DIR_E}, // RA2: as above
	{{-639,-65},DIR_E}, // RA2: as above
	{{-630,-64},DIR_E}, // RA2: as above
	{{-620,-63},DIR_E}, // RA2: as above
	{{-610,-62},DIR_E}, // RA2: as above
	{{-600,-61},DIR_E}, // RA2: as above
	{{-590,-60},DIR_E}, // RA2: as above
	{{-580,-59},DIR_E}, // RA2: as above
	{{-570,-58},DIR_E}, // RA2: as above
	{{-560,-57},DIR_E}, // RA2: as above
	{{-550,-56},DIR_E}, // RA2: as above
	{{-540,-55},DIR_E}, // RA2: as above
	{{-530,-54},DIR_E}, // RA2: as above
	{{-520,-53},DIR_E}, // RA2: as above
	{{-510,-52},DIR_E}, // RA2: as above
	{{-500,-51},DIR_E}, // RA2: as above
	{{-490,-50},DIR_E}, // RA2: as above
	{{-480,-49},DIR_E}, // RA2: as above
	{{-470,-48},DIR_E}, // RA2: as above
	{{-460,-47},DIR_E}, // RA2: as above
	{{-450,-46},DIR_E}, // RA2: as above
	{{-440,-45},DIR_E}, // RA2: as above
	{{-430,-44},DIR_E}, // RA2: as above
	{{-420,-43},DIR_E}, // RA2: as above
	{{-410,-42},DIR_E}, // RA2: as above
	{{-400,-41},DIR_E}, // RA2: as above
	{{-390,-40},DIR_E}, // RA2: as above
	{{-380,-39},DIR_E}, // RA2: as above
	{{-370,-38},DIR_E}, // RA2: as above
	{{-360,-37},DIR_E}, // RA2: as above
	{{-350,-36},DIR_E}, // RA2: as above
	{{-340,-35},DIR_E}, // RA2: as above
	{{-330,-34},DIR_E}, // RA2: as above
	{{-320,-33},DIR_E}, // RA2: as above
	{{-310,-32},DIR_E}, // RA2: as above
	{{-300,-31},DIR_E}, // RA2: as above
	{{-290,-30},DIR_E}, // RA2: as above
	{{-280,-29},DIR_E}, // RA2: as above
	{{-270,-28},DIR_E}, // RA2: as above
	{{-260,-27},DIR_E}, // RA2: as above
	{{-250,-26},DIR_E}, // RA2: as above
	{{-240,-25},DIR_E}, // RA2: as above
	{{-230,-24},DIR_E}, // RA2: as above
	{{-220,-23},DIR_E}, // RA2: as above
	{{-210,-22},DIR_E}, // RA2: as above
	{{-200,-21},DIR_E}, // RA2: as above
	{{-190,-20},DIR_E}, // RA2: as above
	{{-180,-19},DIR_E}, // RA2: as above
	{{-170,-18},DIR_E}, // RA2: as above
	{{-160,-17},DIR_E}, // RA2: as above
	{{-150,-16},DIR_E}, // RA2: as above
	{{-140,-15},DIR_E}, // RA2: as above
	{{-130,-14},DIR_E}, // RA2: as above
	{{-120,-13},DIR_E}, // RA2: as above
	{{-110,-12},DIR_E}, // RA2: as above
	{{-100,-11},DIR_E}, // RA2: as above
	{{-90,-10},DIR_E}, // RA2: as above
	{{-80,-9},DIR_E}, // RA2: as above
	{{-70,-8},DIR_E}, // RA2: as above
	{{-60,-7},DIR_E}, // RA2: as above
	{{-50,-6},DIR_E}, // RA2: as above
	{{-40,-5},DIR_E}, // RA2: as above
	{{-30,-4},DIR_E}, // RA2: as above
	{{-20,-3},DIR_E}, // RA2: as above
	{{-10,-2},DIR_E}, // RA2: as above
	{{0,-1},DIR_E}, // RA2: as above

	{{0,0},DIR_E} // RA2: as above
};


/*
**	There are a limited basic number of tracks that a vehicle can follow. These
**	are they. Each track can be interpreted differently but this is controlled
**	by the TrackControl structure elaborated elsewhere.
*/
ShipLocomotionClass::RawTrackType const ShipLocomotionClass::RawTracks[13] = { // RA2: as above
	{Track1, -1, 0, -1}, // RA2: as above
	{Track2, -1, 0, -1}, // RA2: as above
	{Track3, 37, 12, 22}, // RA2: as above
	{Track4, 26, 11, 19}, // RA2: as above
	{Track5, 45, 15, 31}, // RA2: as above
	{Track6, 44, 16, 27}, // RA2: as above
	{Track7, -1, 0, -1}, // RA2: as above
	{Track8, -1, 0, -1}, // RA2: as above
	{Track9, -1, 0, -1}, // RA2: as above
	{Track10, -1, 0, -1}, // RA2: as above
	{Track11, -1, 0, -1}, // RA2: as above
	{Track12, -1, 0, -1}, // RA2: as above
	{Track13, -1, 0, -1} // RA2: as above
};


/***************************************************************************
**	Smooth turning control table. Given two directions in a path list, this
**	table determines which track to use and what modifying operations need
**	be performed on the track data.
*/
ShipLocomotionClass::TurnTrackType const ShipLocomotionClass::TrackControl[67] = { // RA2: as above
	{1,	0,		DIR_N,	F_},                                                                // 0-0 // RA2: as above
	{3,	7,		DIR_NE,	F_D},                                                               //	0-1 (raw chart) // RA2: as above
	{4,	9,		DIR_E,	F_D},                                                               //	0-2 (raw chart) // RA2: as above
	{0,	0,		DIR_SE,	F_},                                                                // 0-3 ! // RA2: as above
	{0,	0,		DIR_S,	F_},                                                                // 0-4 ! // RA2: as above
	{0,	0,		DIR_SW,	F_},                                                                // 0-5 ! // RA2: as above
	{4,	9,		DIR_W,	(ShipLocomotionClass::TrackControlType)(F_X|F_D)},                 // 0-6 // RA2: as above
	{3,	7,		DIR_NW,	(ShipLocomotionClass::TrackControlType)(F_X|F_D)},                 // 0-7 // RA2: as above
	{6,	8,		DIR_N,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_Y|F_D)},         // 1-0 // RA2: as above
	{2,	0,		DIR_NE,	F_},                                                                //	1-1 (raw chart) // RA2: as above
	{6,	8,		DIR_E,	F_D},                                                               //	1-2 (raw chart) // RA2: as above
	{5,	10,	DIR_SE,	F_D},                                                                   //	1-3 (raw chart) // RA2: as above
	{0,	0,		DIR_S,	F_},                                                                // 1-4 ! // RA2: as above
	{0,	0,		DIR_SW,	F_},                                                                // 1-5 ! // RA2: as above
	{0,	0,		DIR_W,	F_},                                                                // 1-6 ! // RA2: as above
	{5,	10,	DIR_NW,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_Y|F_D)},             // 1-7 // RA2: as above
	{4,	9,		DIR_N,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_Y|F_D)},         // 2-0 // RA2: as above
	{3,	7,		DIR_NE,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_Y|F_D)},         // 2-1 // RA2: as above
	{1,	0,		DIR_E,	(ShipLocomotionClass::TrackControlType)(F_T|F_X)},                 // 2-2 // RA2: as above
	{3,	7,		DIR_SE,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_D)},             // 2-3 // RA2: as above
	{4,	9,		DIR_S,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_D)},             // 2-4 // RA2: as above
	{0,	0,		DIR_SW,	F_},                                                                // 2-5 ! // RA2: as above
	{0,	0,		DIR_W,	F_},                                                                // 2-6 ! // RA2: as above
	{0,	0,		DIR_NW,	F_},                                                                // 2-7 ! // RA2: as above
	{0,	0,		DIR_N,	F_},                                                                // 3-0 ! // RA2: as above
	{5,	10,	DIR_NE,	(ShipLocomotionClass::TrackControlType)(F_Y|F_D)},                     // 3-1 // RA2: as above
	{6,	8,		DIR_E,	(ShipLocomotionClass::TrackControlType)(F_Y|F_D)},                 // 3-2 // RA2: as above
	{2,	0,		DIR_SE,	F_Y},                                                               // 3-3 // RA2: as above
	{6,	8,		DIR_S,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_D)},             // 3-4 // RA2: as above
	{5,	10,	DIR_SW,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_D)},                 // 3-5 // RA2: as above
	{0,	0,		DIR_W,	F_},                                                                // 3-6 ! // RA2: as above
	{0,	0,		DIR_NW,	F_},                                                                // 3-7 ! // RA2: as above
	{0,	0,		DIR_N,	F_},                                                                // 4-0 ! // RA2: as above
	{0,	0,		DIR_NE,	F_},                                                                // 4-1 ! // RA2: as above
	{4,	9,		DIR_E,	(ShipLocomotionClass::TrackControlType)(F_Y|F_D)},                 // 4-2 // RA2: as above
	{3,	7,		DIR_SE,	(ShipLocomotionClass::TrackControlType)(F_Y|F_D)},                 // 4-3 // RA2: as above
	{1,	0,		DIR_S,	F_Y},                                                               // 4-4 // RA2: as above
	{3,	7,		DIR_SW,	(ShipLocomotionClass::TrackControlType)(F_X|F_Y|F_D)},             // 4-5 // RA2: as above
	{4,	9,		DIR_W,	(ShipLocomotionClass::TrackControlType)(F_X|F_Y|F_D)},             // 4-6 // RA2: as above
	{0,	0,		DIR_NW,	F_},                                                                // 4-7 ! // RA2: as above
	{0,	0,		DIR_N,	F_},                                                                // 5-0 ! // RA2: as above
	{0,	0,		DIR_NE,	F_},                                                                // 5-1 ! // RA2: as above
	{0,	0,		DIR_E,	F_},                                                                // 5-2 ! // RA2: as above
	{5,	10,	DIR_SE,	(ShipLocomotionClass::TrackControlType)(F_T|F_D)},                     // 5-3 // RA2: as above
	{6,	8,		DIR_S,	(ShipLocomotionClass::TrackControlType)(F_T|F_D)},                 // 5-4 // RA2: as above
	{2,	0,		DIR_SW,	F_T},                                                               // 5-5 // RA2: as above
	{6,	8,		DIR_W,	(ShipLocomotionClass::TrackControlType)(F_X|F_Y|F_D)},             // 5-6 // RA2: as above
	{5,	10,	DIR_NW,	(ShipLocomotionClass::TrackControlType)(F_X|F_Y|F_D)},                 // 5-7 // RA2: as above
	{4,	9,		DIR_N,	(ShipLocomotionClass::TrackControlType)(F_T|F_Y|F_D)},             // 6-0 // RA2: as above
	{0,	0,		DIR_NE,	F_},                                                                // 6-1 ! // RA2: as above
	{0,	0,		DIR_E,	F_},                                                                // 6-2 ! // RA2: as above
	{0,	0,		DIR_SE,	F_},                                                                // 6-3 ! // RA2: as above
	{4,	9,		DIR_S,	(ShipLocomotionClass::TrackControlType)(F_T|F_D)},                 // 6-4 // RA2: as above
	{3,	7,		DIR_SW,	(ShipLocomotionClass::TrackControlType)(F_T|F_D)},                 // 6-5 // RA2: as above
	{1,	0,		DIR_W,	F_T},                                                               // 6-6 // RA2: as above
	{3,	7,		DIR_NW,	(ShipLocomotionClass::TrackControlType)(F_T|F_Y|F_D)},             // 6-7 // RA2: as above
	{6,	8,		DIR_N,	(ShipLocomotionClass::TrackControlType)(F_T|F_Y|F_D)},             // 7-0 // RA2: as above
	{5,	10,	DIR_NE,	(ShipLocomotionClass::TrackControlType)(F_T|F_Y|F_D)},                 // 7-1 // RA2: as above
	{0,	0,		DIR_E,	F_},                                                                // 7-2 ! // RA2: as above
	{0,	0,		DIR_SE,	F_},                                                                // 7-3 ! // RA2: as above
	{0,	0,		DIR_S,	F_},                                                                // 7-4 ! // RA2: as above
	{5,	10,	DIR_SW,	(ShipLocomotionClass::TrackControlType)(F_X|F_D)},                     // 7-5 // RA2: as above
	{6,	8,		DIR_W,	(ShipLocomotionClass::TrackControlType)(F_X|F_D)},                 // 7-6 // RA2: as above
	{2,	0,		DIR_NW,	F_X},                                                               // 7-7 // RA2: as above

	{11,	11,	DIR_SW,	F_},                                                            // Backup harvester into refinery. // RA2: as above
	{12,	12,	DIR_SW,	F_},                                                            // Drive back into refinery. // RA2: as above
	{13,	13,	DIR_SW,	F_}                                                             // Drive out of weapons factory. // RA2: as above
};
