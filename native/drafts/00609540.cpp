// NON-EXACT DRAFT 0x609540 ParticleClass::unknown_func_609540 (spark behaviour, TibSun Spark_Behavior_AI)
// entry 0x609540; exact-match score 66.45 (Scout709 w/keep/sp_6645.cpp; MISSES_INDEX best 66.45, parked:
// inline/ruling/frame). Recovered source text by our lanes/workers; never exact-match credit. Not original
// bytes. Reached in the M15 win run at frame ~8,000 (ParticleSystemClass 0x0060B670 -> 0x00609C90). Text
// unchanged (Scout1125, M15).
// void ParticleClass::unknown_func_609540() (0x00609540)
// Spark behaviour (TibSun ParticleClass::Spark_Behavior_AI): the spark falls
// under gravity until it meets the ground, a bridge deck or a wall, where it
// glances off along the cell slope and is flagged done; its colour then moves
// a step further along the type's colour list.
#include "shared/particle.hpp"
#include "shared/cd_timer.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/rules.hpp"
#include "shared/matrix3d.hpp"
#include "shared/bounce.hpp"
#include "shared/tunnel_locomotion.hpp"
#include "shared/anim.hpp"
#include "shared/building_type.hpp"
#include "shared/crt_memory.hpp"
#include "shared/unknown_vector3.hpp"
// @ref ?unknown_475840@CellClass@@QBE_NHH@Z=00475840
// @ref _unknown_geometry_j=00a76510
// @ref __real@4@40069600000000000000=0079caa8
// @ref _Particle_Color_Step_007a10f8=007a10f8
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// Original immutable double at 0x007A10F8 (0.05).
extern "C" double const Particle_Color_Step_007a10f8;

// NATIVE (Scout1125 adaptation): the file-local TibSun helpers the lane joined from 0x00608BB0's unit
// (src/recovered/classes/ParticleClass/ParticleClass_unknown_func_608bb0_00608bb0.cpp, text copied).

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
    int height = Map.Get_Height_GL(coord);
    ground = height;
    return height + unknown_geometry_j;
}

// Keeps a value within [low, high].
static inline void Limit(int &value, int low, int high)
{
    if (value < low) {
        value = low;
    } else if (value > high) {
        value = high;
    }
}


void ParticleClass::unknown_func_609540()
{
    CoordStruct coord = Coord;
    unknown_fc.Z -= Rules->unknown_1320;
    Vector3 velocity(unknown_fc.X, unknown_fc.Y, unknown_fc.Z);
    Vector3 position(coord.X, coord.Y, coord.Z);
    double gravity = Rules->unknown_1320;
    velocity.Z -= gravity;

    CoordStruct old_coord;
    old_coord.Set_00434820(position.X, position.Y, position.Z);
    position.Add_004376e0(velocity);
    CoordStruct coord2;
    coord2.Set_00434820(position.X, position.Y, position.Z);

    int height_gl;
    int height_bridge = Bridge_Height(coord2, height_gl);

    CellClass *cellptr = &Map[coord2];
    bool start_under_bridge = false;
    bool end_under_bridge = false;
    if ((cellptr->unknown_140 & 0x100) || (Map[old_coord].unknown_140 & 0x100)) {
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
            BuildingClass *building = static_cast<BuildingClass *>(cellptr->unknown_4717b0());
            if (building || cellptr->unknown_475840(-1, -1)) {
                blocked = true;
                if (building) {
                    if (building->Type->LaserFence && building->unknown_4ec >= 8) {
                        blocked = false;
                    }
                    if (building->unknown_slot31()) {
                        blocked = false;
                    }
                }
            } else {
                blocked = false;
            }
        }
    }

    float ground_z = (float)height_gl;
    bool sloped = true;
    if (position.Z >= ground_z) {
        if (!start_under_bridge && !end_under_bridge && !blocked) {
            sloped = false;
        }
    }
    if (sloped) {
        if (start_under_bridge) {
            position.Z = height_bridge;
        } else if (end_under_bridge) {
            position.Z = (float)(height_bridge - 20);
        } else if ((float)(height_gl - 100) < position.Z) {
            position.Z = ground_z;
        }
        velocity = Slope_Vector(velocity, coord2, 1);
        unknown_121 = true;
    }

    unknown_slot103((CoordStruct const &)UnknownVector3(position.X, position.Y, position.Z));
    double color_change = unknown_id_source_a3d290->Random()(0, 0x7ffffffe) * 4.656612877414201e-10;
    color_change *= Particle_Color_Step_007a10f8;
    color_change += Class->ColorSpeed;
    ColorState.Fraction = color_change + ColorState.Fraction;
    if (ColorState.Fraction > 1.0) {
        if (ColorState.ColorIndex < Class->ColorList.Count() - 2) {
            ColorState.ColorIndex++;
            ColorState.Fraction = 0;
        } else {
            ColorState.Fraction = 1.0;
        }
    }
}
