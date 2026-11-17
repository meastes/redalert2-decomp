// missile.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/missile.json carries the addresses and the rows that
// stay in their own files.


#define RA2_DIR_MISSILE_ANGLES
// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RA2_POINT_LENGTH
#endif

#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define RA2_DIR_MISSILE_MEMBERS
#include "shared/dir_type.hpp"
#include "shared/crt_memory.hpp"
#define RA2_BULLET_MISSILE_VELOCITY
#define RA2_BULLET_MISSILE_SET_PITCH_SOURCE
#include "shared/bullet_velocity.hpp"


// Header-static dynamic initializers of the unit at 0x00593680: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00593680-0x00593861); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005936A0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x005936D0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x005936F0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x00593710, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x00593730, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x00593750, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x00593770, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x005937B0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x005937E0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x00593810, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x00593840, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


// Dynamic initializer (0x00593850, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00593680.cpp; no source of its own.


DirType DirType::operator+(DirType const &other) const
{
    return DirType(Facing + other.Facing);
}


DirType DirType::operator-(DirType const &other) const
{
    return DirType(Facing - other.Facing);
}


DirType DirType::operator/(int divisor) const
{
    return DirType(Facing / divisor);
}


#pragma intrinsic(abs)

bool DirType::Is_Complete_Turn(DirType const &towards, DirType const &by)
{
    DirType difference(*this - towards);
    if (abs(by.Facing) >= abs(difference.Facing)) {
        return true;
    }
    return false;
}


#pragma intrinsic(abs)

bool DirType::Turn(DirType const &towards, DirType const &by)
{
    if (Is_Complete_Turn(towards, by)) {
        Facing = towards.Facing;
        return true;
    }
    DirType difference(towards - *this);
    if (difference.Facing < 0) {
        *this -= by;
    } else {
        *this += by;
    }
    return false;
}


void TVelocity3D<double>::Set_Pitch(DirType const &pitch)
{
    double current_pitch = Get_Pitch().As_Radian();
    double speed = Speed();
    if (current_pitch != 0) {
        X /= Fast_Cos(current_pitch);
        Y /= Fast_Cos(current_pitch);
    }
    X *= Fast_Cos(pitch.As_Radian());
    Y *= Fast_Cos(pitch.As_Radian());
    Z = speed * Fast_Sin(pitch.As_Radian());
}

// Not exact (RA2_NON_MATCHING): Projectile_Motion at 0x00593870, best score 57.74, 2026-11-13; differs: 640 instructions in the original, 645 in the draft (shape 96.65)
#ifdef RA2_NON_MATCHING
// Projectile_Motion 0x00593870 (Missile.cpp; TibSun missile.h:17 declares it): Scout914 pmD1 / Scout1129 pm1_9922
// body on the lane's overlay headers (shared/dir_type.hpp and shared/bullet_velocity.hpp carrying the Missile unit's
// members as in-class inline bodies, TibSun face.h:219-250 and velocity.h:113-125 forms over double math declarations).
#define RA2_POINT_LENGTH
#include "shared/fast_math.hpp"
#include "shared/point.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"

template<class T>
class TPoint3D : public TPoint2D<T> {
    typedef TPoint2D<T> BASECLASS;
public:
    TPoint3D() {}
    TPoint3D(T x, T y, T z) : BASECLASS(x, y), Z(z) {}
    TPoint3D<T> const operator-(TPoint3D<T> const &rvalue) const { return TPoint3D<T>(X - rvalue.X, Y - rvalue.Y, Z - rvalue.Z); }
    TPoint3D<T> const operator+(TPoint3D<T> const &rvalue) const { return TPoint3D<T>(X + rvalue.X, Y + rvalue.Y, Z + rvalue.Z); }
    TPoint3D<T> const operator*(T rvalue) const { return TPoint3D<T>(X * rvalue, Y * rvalue, Z * rvalue); }
    T Length() const { return T(unknown_sqrt_4ba410((double)X * (double)X + (double)Y * (double)Y + (double)Z * (double)Z)); }
    T Distance_To(TPoint3D<T> const &point) const { return (*this - point).Length(); }
    bool operator==(TPoint3D<T> const &rvalue) const { return (X == rvalue.X && Y == rvalue.Y && Z == rvalue.Z); }
    T Z;
};
typedef TPoint3D<int> Point3D;

class Coord : public Point3D {
    typedef TPoint3D<int> BASECLASS;
public:
    Coord() : BASECLASS() {}
    Coord(int x, int y, int z = 0) : BASECLASS(x, y, z) {}
    Coord(BASECLASS const &pt) : BASECLASS(pt) {}
    Coord(CoordStruct const &coord) { X = coord.X; Y = coord.Y; Z = coord.Z; }
};


int __fastcall Projectile_Motion(Coord &coord, TVelocity3D<double> &velocity, Coord const &target_coord, DirType const &rot, bool aircraft, bool airburst, bool veryhigh, bool unknown_8)
{
    if (target_coord == Coord(unknown_geometry_l)) {
        DirType pitch = velocity.Get_Pitch();
        pitch.Turn(DirType(FACING_NE), rot);
        velocity.Set_Pitch(pitch);
        TVelocity3D<double> frame_velocity(velocity);
        coord = coord + Point3D(frame_velocity.X, frame_velocity.Y, frame_velocity.Z);
        return velocity.Speed();
    } else {
        TVelocity3D<double> frame_velocity(velocity);
        coord = coord + Point3D(frame_velocity.X, frame_velocity.Y, frame_velocity.Z);
        int target_distance = coord.Distance_To(target_coord);
        int horizontal_distance = TPoint2D<int>(coord).Distance_To(TPoint2D<int>(target_coord));

        Coord delta = target_coord - coord;

        TVelocity3D<double> target_direction;
        target_direction = TVelocity3D<double>(delta.X, delta.Y, delta.Z);

        DirType desired_yaw = velocity.Get_Yaw();
        desired_yaw.Turn(target_direction.Get_Yaw(), rot);
        velocity.Set_Yaw(desired_yaw);

        DirType desired_pitch = velocity.Get_Pitch();

        if (!aircraft && (airburst || horizontal_distance > ((veryhigh ? 6 : 3) * 256)) && rot.As_Dir256() > 1) {
            TVelocity3D<double> lookahead_velocity(velocity);
            Coord lookahead_coord = coord + Point3D(lookahead_velocity.X, lookahead_velocity.Y, lookahead_velocity.Z) * 6;

            int terrain_height = Map.Get_Height_GL((CoordStruct const &)lookahead_coord);
            if (Map[(CoordStruct const &)lookahead_coord].IsUnderBridge) {
                terrain_height += unknown_geometry_j;
            }

            int cell_distance = target_distance / 256;
            if (airburst || veryhigh) {
                cell_distance = 10;
            } else if (cell_distance >= 5) {
                cell_distance = 5;
            }

            if (!unknown_8) {
                int height_delta = coord.Z - unknown_geometry_g * cell_distance - terrain_height;
                if (height_delta < -20) {
                    coord.Z += 18;
                } else if (height_delta > 20) {
                    coord.Z -= 18;
                }

                if (height_delta < -(unknown_geometry_g / 2)) {
                    desired_pitch.Turn(DirType(FACING_NE), rot / 2);
                } else if (height_delta > unknown_geometry_g / 2) {
                    desired_pitch.Turn(DirType(Dir256(0x40 + 0x08)), rot / 2);
                } else {
                    DirType level_pitch(FACING_E);
                    DirType turn_rate = rot / 2;
                    desired_pitch.Turn(level_pitch, turn_rate);
                }
            }
        } else if (!unknown_8) {
            desired_pitch.Turn(target_direction.Get_Pitch(), (rot + DirType(256)) / 2);
        }

        velocity.Set_Pitch(desired_pitch);

        if (airburst) {
            delta.Z = 0;
        } else {
            delta.Z /= 4;
        }

        return delta.Length();
    }
}

#undef RA2_POINT_LENGTH
#endif

