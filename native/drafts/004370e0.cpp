// NON-EXACT DRAFT 0x4370E0 UnknownBounce::Unknown_004370E0 (Bounce AI: one step) (1448 B)
// entry 0x4370E0; exact-match score 99.77 (lane best), this text 99.77; plan "draft, verify"
// origin lane: Scout811 (build/class-work/Scout811/w/ba0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
// @ref __real@4@40069600000000000000=0079caa8
// @ref __real@8@bff2d1b71758e2196800=0079caa0
// @ref __real@8@bff39d495182a9930800=0079ca98
// @ref __real@8@4000a000000000000000=0079ca78
#include "shared/building_class.hpp"
#include "shared/tactical.hpp"
#include "shared/unknown_vector3.hpp"

// TibSun Slope_Vector: reflects a vector off the slope of the cell at coord.
static inline Vector3 Slope_Vector(Vector3 &vector, CoordStruct const &coord, float length)
{
    Matrix3D slope_matrix;
    slope_matrix = *Unknown_00718D60(&Matrix3D(), unknown_tactical_83996c->Get_Cell_Ramp_006a7110(coord));
    Matrix3D slope_matrix_inv = Matrix3D_Rigid_Inverse_00591420(slope_matrix);
    Vector3 vec;
    vec.Set_00437690(vector.X, -vector.Y, vector.Z);
    vec = slope_matrix_inv.Rotate_Vector(vec);
    vec.Scale_004376b0(length);
    vec.Z = -vec.Z;
    vec = slope_matrix.Rotate_Vector(vec);
    vector.X = vec.X;
    vector.Y = -vec.Y;
    vector.Z = vec.Z;
    return vector;
}

// The height of a bridge deck over coord; ground receives the ground height.
static inline int Bridge_Height(CoordStruct const &coord, int &ground)
{
    int height = Map.Unknown_0055A280(coord);
    ground = height;
    return height + unknown_geometry_j;
}

BounceResultType BounceClass::AI()
{
    BounceResultType bounce_result = BOUNCE_MOVING;

    Vector3 &position = unknown_00.Position;
    Vector3 &velocity = *(Vector3 *)unknown_00.unknown_24;
    Vector3 old_position = position;
    Vector3 old_velocity = velocity;
    double elasticity = unknown_00.unknown_00;
    double max_velocity = unknown_00.unknown_10;

    velocity.Z = ((Vector3 *)unknown_00.unknown_24)->Z - unknown_00.unknown_08;

    if (max_velocity > 0.0) {
        double speed = velocity.Length();
        if (speed > max_velocity) {
            velocity.Scale_004376b0((float)(speed / speed));
        }
    }

    bool has_impact = true;
    {
    CoordStruct previous_coord;
    previous_coord.Set_00434820((int)position.X, (int)position.Y, (int)position.Z);
    position.Add_004376e0(velocity);
    CoordStruct current_coord;
    current_coord.Set_00434820((int)position.X, (int)position.Y, (int)position.Z);

    int ground_height;
    int bridge_height = Bridge_Height(current_coord, ground_height);
    CellClass *cellptr = &Map[current_coord];

    bool hit_bridge_top = false;
    bool hit_bridge_bottom = false;
    if ((cellptr->unknown_140 & 0x100) || (Map[previous_coord].unknown_140 & 0x100)) {
        if (current_coord.Z >= bridge_height) {
            if (previous_coord.Z < bridge_height) {
                hit_bridge_bottom = true;
            }
        } else if (previous_coord.Z >= bridge_height) {
            hit_bridge_top = true;
        }
    }

    bool hit_obstacle = false;
    if (!hit_bridge_top && !hit_bridge_bottom) {
        if (ground_height <= position.Z && position.Z - 150.0f < ground_height) {
            BuildingClass *building = (BuildingClass *)cellptr->unknown_4717b0();
            if (building == 0 && !cellptr->unknown_475840(-1, -1)) {
                hit_obstacle = false;
            } else {
                hit_obstacle = true;
                if (building != 0) {
                    if (building->Type->LaserFence && building->unknown_4ec >= 8) {
                        hit_obstacle = false;
                    }
                    if (building->unknown_slot31()) {
                        hit_obstacle = false;
                    }
                }
            }
        }
    }

    float ground_z = (float)ground_height;
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

        velocity = Slope_Vector(velocity, current_coord, (float)elasticity);
    }
    }
    if (has_impact) {
        Quaternion &angular = (Quaternion &)unknown_40;
        angular[0] = -angular[0];
        angular[1] = -angular[1];
        angular[2] = -angular[2];

        {
            UnknownVector3 impact_coord((int)unknown_00.Position.X, (int)unknown_00.Position.Y, (int)unknown_00.Position.Z);
            UnknownVector3 previous_impact_coord((int)old_position.X, (int)old_position.Y, (int)old_position.Z);
            int previous_cell_height = Map[(CoordStruct const &)previous_impact_coord].unknown_11b;
            if (Map[(CoordStruct const &)impact_coord].unknown_11b - previous_cell_height >= 2) {
                if (old_velocity.Z < -0.0002 && unknown_00.Position.Z > old_position.Z
                    || old_velocity.Z >= -0.0003 && (old_velocity.Z + old_position.Z + 1.0f < unknown_00.Position.Z)) {

                    position = old_position;
                    velocity = old_velocity;

                    int bounce_facing = Map[(CoordStruct const &)impact_coord].unknown_479ac0((CoordStruct const &)previous_impact_coord);
                    if (bounce_facing != -1) {
                        Matrix3D temp;
                        Matrix3D bounce_matrix;
                        bounce_matrix = *Unknown_00719010(&temp, bounce_facing);
                        velocity = bounce_matrix * velocity;
                        velocity *= unknown_00.unknown_00;
                    }
                }
            }
        }

        bounce_result = BOUNCE_IMPACT;
    }

    (Quaternion &)unknown_30 = (Quaternion &)unknown_30 * (Quaternion &)unknown_40;

    double remaining_motion = Get_Remaining_Motion();
    if (remaining_motion < 2.5) {
        bounce_result = BOUNCE_SETTLED;
    }
    return bounce_result;
}
