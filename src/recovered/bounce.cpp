// bounce.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bounce.json carries the addresses and the rows that
// stay in their own files.


#define RA2_BOUNCE_DECLARE_MEMBERS
#define RA2_VECTOR3_DECLARE_LENGTH
#define RA2_TPOINT3D_LENGTH
#define RA2_MATRIX3D_INLINE_COPY
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/bounce.hpp"
#include "shared/id_source.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/building_type_class.hpp"


// Header-static dynamic initializers of the unit at 0x00436A80: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00436A80-0x00436C61); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00436AA0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436AD0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436AF0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436B10, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436B30, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436B50, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436B70, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436BB0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436BE0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436C10, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436C40, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Dynamic initializer (0x00436C50, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00436a80.cpp; no source of its own.


// Random launch velocity from the original 00436C70..00436DB4.
extern "C" double const bounce_random_scale;
extern "C" double const bounce_launch_scale;
extern "C" double const bounce_full_circle;
extern "C" double const bounce_elevation_base;
extern "C" double const bounce_elevation_scale;
inline double Bounce_Random_Double(double minimum, double maximum)
{
 return minimum+(maximum-minimum)*((double)Random_Pick(0,0x7ffffffe)*bounce_random_scale);
}
void BounceClass::Init(CoordStruct const &coord, double elasticity, double unused, double min_speed, double max_speed)
{
 Vector3 velocity;
 double speed=Bounce_Random_Double(min_speed,max_speed)*bounce_launch_scale;
 double yaw=Bounce_Random_Double(0.0,1.0)*bounce_full_circle;
 double pitch=Bounce_Random_Double(1.0,2.0)*bounce_elevation_scale;
 double cosine=unknown_cos_4ba4d0(pitch);
 velocity.X=cosine*unknown_cos_4ba4d0(yaw)*speed;
 velocity.Y=(unknown_cos_4ba4d0(pitch)*unknown_sin_4ba480(yaw)*speed);
 velocity.Z=unknown_sin_4ba480(pitch)*speed;
 Init(coord,elasticity,3.0,0.0,velocity,0.0);
}


// BounceClass (TibSun bounce.cpp, unit 0x00436A80): the bouncing motion of debris
// (AnimClass +0x108, VoxelAnimClass +0x98). void BounceClass::Init(CoordStruct const &,
// double elasticity, double gravity, double max_velocity, Vector3 const &velocity,
// double rotation) (0x00436DC0): starts the bounce with a random tumble axis.

// TibSun vector3.h's scalar division and Normalize; Length2 reads the components into
// locals (X, Z, Y: the order the stored components are reloaded), and Normalize writes
// Length out (the unit's Vector3::Length 0x00437710 is out of line).
inline Vector3 operator/(Vector3 const &a, double k)
{
    return Vector3((float)(a.X / k), (float)(a.Y / k), (float)(a.Z / k));
}

inline float Length2(Vector3 const &vec)
{
    float x = vec.X;
    float z = vec.Z;
    float y = vec.Y;
    return x * x + y * y + z * z;
}

inline Vector3 Normalize(Vector3 const &vec)
{
    float len = (float)unknown_sqrt_4ba410(Length2(vec));
    if (len != 0.0) {
        return vec / len;
    }
    return vec;
}

void BounceClass::Init(CoordStruct const &coord, double elasticity, double gravity, double max_velocity,
                       Vector3 const &velocity, double rotation)
{
    unknown_00.unknown_00 = elasticity;
    unknown_00.unknown_08 = gravity;
    unknown_00.unknown_10 = max_velocity;
    unknown_00.Position.X = coord.X;
    unknown_00.Position.Y = coord.Y;
    unknown_00.Position.Z = coord.Z;
    *(Vector3 *)unknown_00.unknown_24 = velocity;

    Vector3 rotation_axis(
        Random_Pick(-65535, 65535) * (1.0f / 65535.0f),
        Random_Pick(-65535, 65535) * (1.0f / 65535.0f),
        Random_Pick(-65535, 65535) * (1.0f / 65535.0f));

    rotation_axis = Normalize(rotation_axis);
    (Quaternion &)unknown_40 = Axis_To_Quat(rotation_axis, rotation);
    Matrix3D identity_matrix;
    identity_matrix.Make_Identity();
    (Quaternion &)unknown_30 = Build_Quaternion(identity_matrix);
}


// CoordStruct BounceClass::Coord() const -- 0x00436F80
// The bouncing object's float position (+0x18) truncated to a coordinate.

CoordStruct BounceClass::Coord() const
{
    return CoordStruct((int)unknown_00.Position.X, (int)unknown_00.Position.Y, (int)unknown_00.Position.Z);
}


// Matrix3D BounceClass::unknown_matrix_436fc0() const (0x00436FC0; TibSun Get_Matrix): the
// tumble rotation as a matrix (Matrix3D's copy constructor is inline here, RA2_MATRIX3D_INLINE_COPY).
Matrix3D BounceClass::unknown_matrix_436fc0() const
{
    return Build_Matrix3D((Quaternion const &)unknown_30);
}


// double BounceClass::Get_Remaining_Motion() const (0x00436FF0; TibSun bounce.cpp): the
// speed plus the height above the ground (or bridge deck) scaled by gravity; small means at rest.

double BounceClass::Get_Remaining_Motion() const
{
    CoordStruct coord((int)unknown_00.Position.X, (int)unknown_00.Position.Y, (int)unknown_00.Position.Z);

    int height = (int)(unknown_00.Position.Z - Map.Get_Height_GL(coord));
    if ((Map[coord].unknown_140 & 0x100) && height >= unknown_geometry_j) {
        height -= unknown_geometry_j;
    }
    Vector3 const &velocity = *(Vector3 const *)unknown_00.unknown_24;
    TPoint3D<int> point((int)velocity.X, (int)velocity.Y, (int)(height * unknown_00.unknown_08 + velocity.Z));
    return point.Length();
}

// Not exact (RA2_NON_MATCHING): BounceClass::AI at 0x004370E0, best score 84.57, 2026-11-13; differs: the out-of-line Vector3::Length (0x00437710, joined into this unit by lane F on 2026-11-13) is inlined here; the original calls it (99.77 before the join)
#ifdef RA2_NON_MATCHING
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
    int height = Map.Get_Height_GL(coord);
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
            BuildingClass *building = (BuildingClass *)cellptr->Cell_Building();
            if (building == 0 && !cellptr->Has_Wall_Or_Gate(-1, -1)) {
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

                    int bounce_facing = Map[(CoordStruct const &)impact_coord].Bounce_Direction((CoordStruct const &)previous_impact_coord);
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
#endif


// float Vector3::Length() const (0x00437710): Westwood's (float)sqrt(Length2()); the out-of-line
// copy BounceClass::AI 0x004370E0 calls.
double __cdecl unknown_sqrt_4ba410(double value);     // 0x004BA410

float Vector3::Length() const
{
    return (float)unknown_sqrt_4ba410(X * X + Y * Y + Z * Z);
}
