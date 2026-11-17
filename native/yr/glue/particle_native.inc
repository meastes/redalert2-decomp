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
