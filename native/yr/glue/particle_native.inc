// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y11b): ParticleClass::Spark_Behavior_AI, gamemd.exe 0x0062C6E0 (1,072
// bytes), unmatched in the YR tree (a link-closure trap once weapons hit: the sparks of a hit).
// Written from behaviour: Red Alert 2's draft (src/recovered/particle.cpp, 0x00609540; TibSun's
// ParticleClass::Spark_Behavior_AI) in the YR tree's names, checked against gamemd.exe's code: the
// spark falls under gravity until it meets the ground, a bridge deck or a wall, where it stops and is
// flagged done (gamemd.exe's slope reflection there goes to a local it never reads); its colour then
// moves on along the type's colour list.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
#include "building.h"
#include "builtype.h"
#include "cell.h"

void ParticleClass::Spark_Behavior_AI(void)
{
	Coord coord = Position;
	MovementDirection.Z -= (float)Rule->Gravity;
	Vector3 velocity = MovementDirection;
	Vector3 position((float)coord.X, (float)coord.Y, (float)coord.Z);
	velocity.Z -= (float)Rule->Gravity;

	Coord old_coord((int)position.X, (int)position.Y, (int)position.Z);
	position += velocity;
	Coord new_coord((int)position.X, (int)position.Y, (int)position.Z);

	int height = Map.Get_Height_GL(new_coord);
	int bridge_height = height + BRIDGE_LEPTON_HEIGHT;
	CellClass * cell = &Map[new_coord];
	bool fell = false;
	bool rose = false;
	if (cell->IsUnderBridge || Map[old_coord].IsUnderBridge) {
		if (new_coord.Z >= bridge_height) {
			if (old_coord.Z < bridge_height) {
				rose = true;
			}
		} else if (old_coord.Z >= bridge_height) {
			fell = true;
		}
	}

	bool blocked = false;
	if (!fell && !rose && position.Z >= (float)height && position.Z - 150.0f < (float)height) {
		BuildingClass * building = cell->Cell_Building();
		if (building != NULL || cell->Has_Wall_Or_Gate()) {
			blocked = true;
			if (building != NULL) {
				if (building->Class->IsLaserFence && building->LaserFenceFrame >= 8) {
					blocked = false;
				}
				if (building->Considered_Vehicle()) {
					blocked = false;
				}
			}
		}
	}

	float ground = (float)height;
	if (position.Z < ground || fell || rose || blocked) {
		if (fell) {
			position.Z = (float)bridge_height;
		} else if (rose) {
			position.Z = (float)(bridge_height - 20);
		} else if ((float)(height - 100) < position.Z) {
			position.Z = ground;
		}
		// gamemd.exe reflects the velocity off the cell's slope here into a local it does not use again.
		IsToDie = true;
	}

	Set_Coord(Coord((int)position.X, (int)position.Y, (int)position.Z));
	ColorAccum += Scen->RandomNumber(0, 0x7FFFFFFE) * 4.656612877414201e-10 * 0.05 + Class->ColorSpeed;
	if (ColorAccum > 1.0) {
		if (ColorIndex < Class->ColorList.Count() - 2) {
			ColorIndex++;
			ColorAccum = 0.0;
		} else {
			ColorAccum = 1.0;
		}
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y15), appended to the particle unit: ParticleClass::Gas_Behavior_AI (gamemd.exe (particle.cpp)), unmatched in
// ra2-decomp. Red Alert 2's text of the same routine (mischa85/RedAlert2 src/particle.cpp, game.exe 0x00608BB0; SPDX
// GPL-3.0-or-later, Copyright 2026 OpenTS contributors, LICENSE.md's terms; THIRD_PARTY.md), with the
// YR tree's names where they differ (marked NATIVE). Yuri's Revenge's routine is not compared with it
// instruction by instruction. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
// Its helper (the same file: the deflection off a ramped cell).
static inline Vector3 Slope_Vector(Vector3 & vector, Coord const & coord, double length) // RA2: defined above Gas_Behavior_AI, its first user
{
	Matrix3D slope_matrix;
	Matrix3D slope_matrix_inv = Matrix3D::Orthogonal_Inverse(
		slope_matrix = Get_Slope_Matrix(TacticalMap->Get_Cell_Ramp(coord)));
	Vector3 vec(vector.X, -vector.Y, vector.Z);
	vec = slope_matrix_inv.Rotate_Vector(vec);
	vec *= length;
	vec.Z = -vec.Z;
	vec = slope_matrix.Rotate_Vector(vec);
	vector.X = vec.X;
	vector.Y = -vec.Y;
	vector.Z = vec.Z;
	return(vector);
}

void ParticleClass::Gas_Behavior_AI(void)
{
	if ((Frame & 1) == 0) {
		Coord coord = Get_Coord();
		if ((abs(Scen->RandomNumber()) & 7) == 0) {
			int dx = 0;
			int dy = 0;
			if ((abs(Scen->RandomNumber()) & 1) == 0) {
				dx = (int)abs(Scen->RandomNumber()) % 3 - 1;
			} else {
				dy = (int)abs(Scen->RandomNumber()) % 3 - 1;
			}
			GasDrift += Point2D(dx, dy);
			if (GasDrift.X < -5) {
				GasDrift.X = -5;
			} else if (GasDrift.X > 5) {
				GasDrift.X = 5;
			}
			if (GasDrift.Y < -5) {
				GasDrift.Y = -5;
			} else if (GasDrift.Y > 5) {
				GasDrift.Y = 5;
			}
		}

		Vector3 position(coord.X, coord.Y, coord.Z);
		Vector3 & velocity = GasVelocity;
		velocity = Vector3(0, 0, -2);
		double gravity = Rule->Gravity; // RA2: the gravity is read into a local first
		velocity.Z -= gravity; // RA2: as above

		Coord old_coord(position.X, position.Y, position.Z);
		position += velocity;
		Coord coord2(position.X, position.Y, position.Z);

		int height_gl = Map.Get_Height_GL(coord2);
		int height_bridge = height_gl + BRIDGE_LEPTON_HEIGHT;

		CellClass * cellptr = &Map[coord2];
		bool start_under_bridge = false;
		bool end_under_bridge = false;
		if (cellptr->IsUnderBridge || Map[old_coord].IsUnderBridge) {
			if (coord2.Z >= height_bridge) {
				if (old_coord.Z < height_bridge) {
					end_under_bridge = true;
				}
			} else {
				if (old_coord.Z >= height_bridge) {
					start_under_bridge = true;
				}
			}
		}

		bool blocked = false;
		if (!start_under_bridge && !end_under_bridge) {
			if (position.Z >= (double)height_gl && position.Z - 150.0f < (double)height_gl) {
				BuildingClass * building = cellptr->Cell_Building();
				if (building || cellptr->Has_Wall_Or_Gate()) {
					blocked = true;
					if (building) {
						if (building->Class->IsLaserFence && building->LaserFenceFrame >= 8) {
							blocked = false;
						}
						if (building->Considered_Vehicle()) {
							blocked = false;
						}
					}
				} else {
					blocked = false;
				}
			}
		}

		if (position.Z < (float)height_gl || start_under_bridge || end_under_bridge || blocked) { // RA2: the position is compared as a float and decides the whole block
			if (start_under_bridge) { // RA2: the position is moved onto the bridge, below it or onto the ground before the slope is applied
				position.Z = height_bridge; // RA2: as above
			} else if (end_under_bridge) { // RA2: as above
				position.Z = height_bridge - 20; // RA2: as above
			} else if ((float)(height_gl - 100) < position.Z) { // RA2: as above
				position.Z = (float)height_gl; // RA2: as above
			}
			Slope_Vector(velocity, coord2, 1);
		}

		if (HeightAGL > 5 && (Frame % 2) == 0) {
			GasDrift.Z = MAX(-5, GasDrift.Z - 1);
		} else {
			GasDrift.Z = MAX(GasDrift.Z, 0);
		}
	}

	RemainingDC--;

	if (RemainingDC == 0 && Class->Damage) { // RA2: no WEAKGAS test and no Visceroid spawn
		RemainingDC = Class->MaxDC; // RA2: as above
		ObjectClass * occupier = Map[Get_Coord()].Cell_Occupier(); // RA2: as above
		while (occupier) { // RA2: as above
			Coord delta = Get_Coord() - occupier->Get_Coord(); // RA2: as above
			ObjectClass * next = occupier->Next; // RA2: as above
			if (occupier->IsActive) { // RA2: as above
				if (occupier->Strength > 0) { // RA2: as above
					int damage = Class->Damage; // RA2: as above
					occupier->Take_Damage(damage, TacticalMap->Z_Lepton_To_Pixel(abs(delta.X) + abs(delta.Y)), Class->Warhead, 0, 0, 0); // RA2: the result is not kept
				}
			}
			occupier = next; // RA2: as above
		}
	}
	if ((Fetch_ID() + (Class->MaxEC - RemainingEC)) % ((Fetch_ID() & 1) + Class->StateAIAdvance) == 0) { // RA2: Fetch_ID first, then the EC difference; the divisor adds the ID bit first
		StateAI++;
	}
	if (StateAI == Class->EndStateAI) {
		if (Class->DeleteOnStateLimit) {
			IsToDie = true;
			return; // RA2: returns here
		}
		StateAI = 0; // RA2: as above
	}
}
