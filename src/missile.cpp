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

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "missile.h"

#include "_map.h"
#include "cell.h"
#include "coord.h"
#include "face.h"
#include "globals.h"
#include "mouse.h"
#include "velocity.h"


/// <summary>
/// Handles one frame of guided missile flight.
/// This routine is used by the bullet logic to fly a homing projectile. It advances the
/// projectile along its current velocity, swings that velocity toward the target by no more
/// than the allowed turn rate, and flies the missile over the intervening terrain so that it
/// clears hills and bridges before it stoops onto its target.
/// </summary>
/// <param name="coord">The projectile position; advanced to its new position.</param>
/// <param name="velocity">The projectile velocity; steered toward the target.</param>
/// <param name="target_coord">The coordinate to home on, or COORD_NONE to merely level
/// out.</param>
/// <param name="rot">The greatest amount the projectile may turn during this frame.</param>
/// <param name="aircraft">Is the projectile chasing an aircraft?</param>
/// <param name="airburst">Should the projectile detonate in the air rather than stoop?</param>
/// <param name="veryhigh">Should the projectile cruise at extra altitude?</param>
/// <param name="level">Should the projectile hold its height (no climb or dive steering)?</param>
/// <returns>Returns with the distance still to be covered, with the vertical component
/// discounted. A projectile with nothing to home on levels out, advances and returns its speed.</returns>
int Projectile_Motion(Coord & coord, TVelocity3D<double> & velocity, Coord const & target_coord, DirType const & rot, bool aircraft, bool airburst, bool veryhigh, bool level)
{
	if (target_coord == COORD_NONE) {
		DirType pitch = velocity.Get_Pitch();
		pitch.Turn(DirType(DIR_NE), rot);
		velocity.Set_Pitch(pitch);
		TVelocity3D<double> frame_velocity(velocity);
		coord = coord + Point3D(frame_velocity.X, frame_velocity.Y, frame_velocity.Z);
		return(velocity.Magnitude());
	} else {
		TVelocity3D<double> frame_velocity(velocity, velocity.Z);
		coord = coord + Point3D(frame_velocity.X, frame_velocity.Y, frame_velocity.Z);
		int target_distance = coord.Distance_To(target_coord);
		int horizontal_distance = Point2D(coord).Distance_To(Point2D(target_coord));

		Coord delta = target_coord - coord;

		TVelocity3D<double> target_direction;
		TVelocity3D<double> dir = TVelocity3D<double>(delta.X, delta.Y, delta.Z);
		target_direction = dir;

		DirType desired_yaw = velocity.Get_Yaw();
		desired_yaw.Turn(target_direction.Get_Yaw(), rot);
		velocity.Set_Yaw(desired_yaw);

		DirType desired_pitch = velocity.Get_Pitch();

		if (!aircraft && (airburst || horizontal_distance > ((veryhigh ? 6 : 3) * CELL_LEPTON)) && rot.As_Dir256() > 1) {
			TVelocity3D<double> lookahead_velocity(velocity);
			Coord lookahead_coord = coord + Point3D(lookahead_velocity.X, lookahead_velocity.Y, lookahead_velocity.Z) * 6;

			int terrain_height = Map.Get_Height_GL(lookahead_coord);
			if (Map[lookahead_coord].IsUnderBridge) {
				terrain_height += BRIDGE_LEPTON_HEIGHT;
			}

			int cell_distance = target_distance / CELL_LEPTON;
			cell_distance = (airburst || veryhigh) ? 10 : min(cell_distance, 5);

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
					desired_pitch.Turn(DIR_E, rot / 2);
				}
			}
		} else if (!level) {
			desired_pitch.Turn(target_direction.Get_Pitch(), (rot + DirType(256)) / 2);
		}

		velocity.Set_Pitch(desired_pitch);

		delta.Z = airburst ? 0 : delta.Z / 4;

		return(delta.Length());
	}
}
