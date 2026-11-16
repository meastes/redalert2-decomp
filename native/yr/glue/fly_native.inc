
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y7), appended to the fly unit: FlyLocomotionClass::Nearing_Target,
// YR 0x004CEFB0 (1,628 bytes, unmatched in ra2-decomp; a link-closure trap that ended a Simulator
// skirmish when an aircraft flew to a destination). From the RA2 tree's twin
// (src/recovered/fly.cpp, unknown_4be720 at 0x004BE720) in the TibSun archive's shape (fly.cpp),
// with gamemd.exe's code read for YR's differences: a fly-by aircraft type (AircraftTypeClass
// IsFlyBy, +0xE0B) never stages its approach; a docking building's coordinate is
// BuildingClass::Docking_Coord(docker) when the aircraft is linked to it, else the landing zone
// New_LZ picks (COORD_NONE when none); slowing to a stop calls Land (0x004CFA70) below 0.05
// (0x007E8AE8). The leading virtual call whose result gamemd.exe discards (slot 0x1B8, the
// object's cell) is left out. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
int FlyLocomotionClass::Nearing_Target(bool stage_approach, CoordStruct target)
{
	IFlyControlPtr flyctrl(LinkedTo);
	Coord coord(target.X, target.Y, target.Z);

	if (flyctrl != NULL && LinkedTo->TarCom != NULL && LinkedTo->Ammo && flyctrl->Is_Strafe()) {
		stage_approach = false;
	}
	if (LinkedTo->What_Am_I() == RTTI_AIRCRAFT && ((AircraftClass *)LinkedTo)->Class->IsFlyBy) {
		stage_approach = false;
	}

	/*
	 * The distance to the destination, or to a building's docking point on it.
	 */
	Coord aim = coord;
	BuildingClass * building = Map[coord.As_Cell()].Cell_Building();
	if (building != NULL && !LinkedTo->TClass->IsHunterSeeker) {
		if (LinkedTo->ContainsLink(building)) {
			aim = building->Docking_Coord(LinkedTo);
		} else if (LinkedTo->What_Am_I() == RTTI_AIRCRAFT) {
			AbstractClass * lz = ((AircraftClass *)LinkedTo)->New_LZ(building);
			aim = (lz != NULL) ? lz->Center_Coord() : COORD_NONE;
		}
	}

	LinkedTo->Get_Height();
	Coord here = LinkedTo->Center_Coord();
	int dist = (Point2D(here.X, here.Y) - Point2D(aim.X, aim.Y)).Length();

	/*
	 * Turn the body toward the destination (a strafing run close by keeps its heading
	 * unless the turn is within 45 degrees).
	 */
	if (flyctrl == NULL || !flyctrl->Is_Locked()) {
		DirType dir = Direction(LinkedTo->Center_Coord(), coord);
		bool turn = true;
		if (flyctrl != NULL && flyctrl->Is_Strafe()) {
			Coord now = LinkedTo->Center_Coord();
			if ((Point2D(now.X, now.Y) - Point2D(coord.X, coord.Y)).Length() <= 0x300) {
				short diff = (short)(unsigned short)(dir.Raw - LinkedTo->PrimaryFacing.Current().Raw);
				if ((diff < 0 ? -diff : diff) > 0x2000) {
					turn = false;
				}
			}
		}
		if (turn) {
			LinkedTo->PrimaryFacing.Set_Desired(Direction(LinkedTo->Center_Coord(), coord));
		}
	}

	/*
	 * Aim the turret: at the target (or the landing direction) when about to arrive,
	 * else at the destination.
	 */
	if (dist < 0x100 && stage_approach && !LinkedTo->TClass->IsHunterSeeker) {
		if (LinkedTo->TarCom != NULL && LinkedTo->Ammo && (flyctrl == NULL || !flyctrl->Is_Strafe())) {
			if (flyctrl == NULL || !flyctrl->Is_Locked()) {
				LinkedTo->SecondaryFacing.Set_Desired(LinkedTo->Direction(LinkedTo->TarCom));
			}
		} else {
			DirType land(DIR_N);
			if (flyctrl != NULL) {
				land = DirType(FacingType(flyctrl->Landing_Direction()));
			}
			if (flyctrl == NULL || !flyctrl->Is_Locked()) {
				LinkedTo->SecondaryFacing.Set_Desired(land);
			}
		}
	} else if (flyctrl == NULL || !flyctrl->Is_Locked()) {
		LinkedTo->SecondaryFacing.Set_Desired(Direction(LinkedTo->Center_Coord(), coord));
	}

	/*
	 * The flight level: the destination's height while elevating close to it; a dropship
	 * descends to a third of its level over its slowdown distance; else the type's level.
	 */
	if (IsElevating && dist < 0x300) {
		FlightLevel = DestinationCoord.Z - Map.Get_Height_GL(DestinationCoord);
	} else if (LinkedTo->TClass->IsDropship) {
		int level = LinkedTo->TClass->GetFlightLevel();
		int slowdown = LinkedTo->TClass->SlowdownDistance;
		if (dist < slowdown) {
			float ratio = (float)(dist / slowdown);
			FlightLevel = (int)(level * ratio + (1.0f - ratio) * (level / 3));
		} else {
			FlightLevel = level;
		}
	} else {
		FlightLevel = LinkedTo->TClass->GetFlightLevel();
	}

	/*
	 * Stage the speed down on the approach; at a stop, land.
	 */
	if (!LinkedTo->TClass->IsHunterSeeker && Needs_To_Land()) {
		if (dist < 0x80) {
			if (stage_approach) {
				TargetSpeed = 0.0;
			}
			if (!IsElevating && CurrentSpeed < 0.05) {
				Land();
			}
		} else if (dist < 0x200) {
			if (stage_approach) {
				TargetSpeed = 0.5;
			}
		} else if (dist < 0x300 && stage_approach) {
			TargetSpeed = 0.75;
		}
	}

	/*
	 * A computer player's selected aircraft that flies into the shroud (or the fog) is
	 * unselected.
	 */
	if (LinkedTo->IsSelected && !LinkedTo->House->Is_Player_Control()) {
		Coord at = LinkedTo->Position;
		if (Map.Is_Shrouded(at) || (Scen->Special.IsFogOfWar && Map.IsLocationFogged(at))) {
			LinkedTo->Unselect();
		}
	}
	return dist;
}
