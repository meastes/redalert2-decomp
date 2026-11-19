
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the bounce unit: BounceClass::AI, unmatched in
// ra2-decomp. TibSun's text (TibSun archive code/bounce.cpp; SPDX GPL-3.0-or-later, Copyright 2025
// Electronic Arts Inc. and OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md).
// Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
BounceResultType BounceClass::AI(void)
{
	BounceResultType bounce_result = BOUNCE_MOVING;

	/*
	 * Remember the pre-move position and velocity so the off-cliff handler can
	 * restore them. The saved velocity is the value before gravity is applied below.
	 */
	Vector3 & position = MyCoord;
	Vector3 & velocity = Velocity;
	Vector3 old_position = position;
	Vector3 old_velocity = velocity;
	double elasticity = Elasticity;
	double max_velocity = MaxVelocity;

	/*
	 * Apply gravity to the vertical velocity component.
	 */
	velocity.Z = Velocity.Z - Gravity;

	/*
	 * Clamp the velocity to the maximum allowed.
	 */
	if (max_velocity > 0.0) {
		double speed = velocity.Length();
		if (speed > max_velocity) {
			velocity *= (float)(speed / speed);
		}
	}

	/*
	 * Advance the position by the velocity. Remember the integer cell positions
	 * before and after the move.
	 */
	Point3D previous_coord(position.X, position.Y, position.Z);
	position += velocity;
	Coord current_coord(position.X, position.Y, position.Z);

	int ground_height = Map.Get_Height_GL(current_coord);
	int bridge_height = BRIDGE_LEPTON_HEIGHT + ground_height;
	CellClass * cellptr = &Map[current_coord];

	/*
	 * Determine if the object passed through a bridge surface this frame.
	 */
	bool hit_bridge_top = false;
	bool hit_bridge_bottom = false;
	if (cellptr->IsUnderBridge || Map[(Coord const &)previous_coord].IsUnderBridge) {
		if (current_coord.Z >= bridge_height) {
			if (previous_coord.Z < bridge_height) {
				hit_bridge_bottom = true;
			}
		} else if (previous_coord.Z >= bridge_height) {
			hit_bridge_top = true;
		}
	}

	/*
	 * Determine if the object struck a building, wall or gate that should stop it.
	 */
	bool hit_obstacle = false;
	if (!hit_bridge_top && !hit_bridge_bottom) {
		if (ground_height <= position.Z && position.Z - 150.0f < ground_height) {
			BuildingClass * building = cellptr->Cell_Building();
			if (building == NULL && !cellptr->Has_Wall_Or_Gate(OVERLAY_NONE, FACING_NONE)) {
				hit_obstacle = false;
			} else {
				hit_obstacle = true;
				if (building != NULL) {
					if (building->Class->IsLaserFence && building->LaserFenceFrame >= 8) {
						hit_obstacle = false;
					}
					if (building->IsStrange()) {	// YR (lane N4): TibSun's Considered_Vehicle is the YR tree's IsStrange (BuildingClass: Class->IsVehicle(), vtable 0x80); the tree's Considered_Vehicle is another slot (Height > -20), true for every building
						hit_obstacle = false;
					}
				}
			}

		}
	}

	/*
	 * Decide whether this frame reached a surface, then snap the vertical position
	 * to the ground, bridge or obstacle. A bridge crossing or obstacle counts even
	 * when the final position remains above the ground.
	 */
	float ground_z = (float)ground_height;
	bool has_impact = true;
	if (position.Z >= ground_z) {
		if (!hit_bridge_top && !hit_bridge_bottom && !hit_obstacle) {
			has_impact = false;
		}
	}
	if (has_impact) {
		if (hit_bridge_top) {
			position.Z = (float)bridge_height;
		} else if (hit_bridge_bottom) {
			position.Z = (float)(bridge_height - 20);
		} else {
			if ((ground_height - 100) < position.Z) {
				position.Z = ground_z;
			}
		}

		/*
		 * Reflect the velocity off the cell slope. The velocity is rotated into the
		 * slope's local space, scaled by elasticity, flipped vertically, then rotated
		 * back out.
		 */
		{
			Matrix3D slope_matrix;
			Matrix3D inverse_slope_matrix = Matrix3D::Orthogonal_Inverse(
				slope_matrix = Get_Slope_Matrix(TacticalMap->Get_Cell_Ramp(current_coord)));
			Vector3 bounce_velocity(velocity.X, -velocity.Y, velocity.Z);
			bounce_velocity = inverse_slope_matrix.Rotate_Vector(bounce_velocity);
			bounce_velocity *= (float)elasticity;
			bounce_velocity.Z = -bounce_velocity.Z;
			bounce_velocity = slope_matrix.Rotate_Vector(bounce_velocity);
			velocity.X = bounce_velocity.X;
			velocity.Y = -bounce_velocity.Y;
			velocity.Z = bounce_velocity.Z;
		}

		/*
		 * Invert the spin axis of the angular velocity (but not the W component).
		 */
		AngularVelocity[0] = -AngularVelocity[0];
		AngularVelocity[1] = -AngularVelocity[1];
		AngularVelocity[2] = -AngularVelocity[2];

		/*
		 * If the object fell off a cliff edge, restore its pre-move state and bounce
		 * off the slope of the cell it would have fallen into.
		 */
		{
			Coord impact_coord(MyCoord.X, MyCoord.Y, MyCoord.Z);
			Coord previous_impact_coord(old_position.X, old_position.Y, old_position.Z);
			int previous_cell_height = Map[(Coord const &)previous_impact_coord].Height;
			if (Map[(Coord const &)impact_coord].Height - previous_cell_height >= 2) {
				if (old_velocity.Z < -0.0002 && MyCoord.Z > old_position.Z
					|| old_velocity.Z >= -0.0003 && (old_velocity.Z + old_position.Z + 1.0f < MyCoord.Z)) {

					position = old_position;
					velocity = old_velocity;

					FacingType bounce_facing = Map[(Coord const &)impact_coord].Bounce_Direction((Coord const &)previous_impact_coord);
					if (bounce_facing != FACING_NONE) {
						Matrix3D bounce_matrix = Get_Bounce_Matrix(bounce_facing);
						velocity = bounce_matrix * velocity;
						velocity *= Elasticity;
					}
				}
			}
		}

		bounce_result = BOUNCE_IMPACT;
	}

	Rotation = Rotation * AngularVelocity;

	double remaining_motion = Get_Remaining_Motion();
	if (remaining_motion < 2.5) {
		bounce_result = BOUNCE_SETTLED;
	}
	return(bounce_result);
}
