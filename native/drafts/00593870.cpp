// NON-EXACT DRAFT 0x593870 Projectile_Motion
// entry 0x593870; exact-match score 99.22 (misses_index best)
// origin lane: Scout914 (build/class-work/Scout914/w/pmD1.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1121 (native/drafts.json). Not original bytes.
// @ref ?unknown_sqrt_4ba410@@YANN@Z=004ba410
// @ref ?unknown_math_4ba600@@YANNN@Z=004ba600
// @ref ?unknown_sin_4ba480@@YANN@Z=004ba480
// @ref ?unknown_cos_4ba4d0@@YANN@Z=004ba4d0
// @ref _unknown_geometry_l=00a70c08
// @ref _unknown_geometry_g=00a70c40
// @ref _unknown_geometry_j=00a70c30
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
// @ref ??G?$TPoint3D@H@@QBE?BV0@ABV0@@Z=004348f0
// @ref ?Length@?$TPoint3D@H@@QBEHXZ=0041b780
// @ref ??G?$TPoint2D@H@@QBE?BV0@ABV0@@Z=0047d040
// @ref ?Length@?$TPoint2D@H@@QBEHXZ=0047d060
// @ref ?Get_Pitch@?$TVelocity3D@N@@QBE?AVDirType@@XZ=0041b6e0
// @ref ?Horizontal_Speed@?$TVelocity3D@N@@QBENXZ=0041b750
// @ref ?Speed@?$TVelocity3D@N@@QBENXZ=0041b7c0
// @ref ?Speed@?$TVelocity2D@N@@QBENXZ=0041b830
// @ref ?Nonzero_Check@?$TVelocity2D@N@@QAEXXZ=0041b860
// @ref ??HDirType@@QBE?AV0@ABV0@@Z=005940b0
// @ref ??GDirType@@QBE?AV0@ABV0@@Z=005940d0
// @ref ??KDirType@@QBE?AV0@H@Z=005940f0
// @ref ?Is_Complete_Turn@DirType@@QAE_NABV1@0@Z=00594110
// @ref ?Turn@DirType@@QAE_NABV1@0@Z=00594140
// @ref ?Set_Pitch@?$TVelocity3D@N@@QAEXABVDirType@@@Z=005941b0
#define RA2_POINT_LENGTH
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/fast_math.hpp"
#include "shared/point.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
double __cdecl unknown_math_4ba600(double y, double x);
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);
double __cdecl unknown_sqrt_4ba410(double value);
extern "C" int __cdecl abs(int value);
#pragma intrinsic(abs)

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

enum FacingType {
    FACING_NONE = -1,
    FACING_N,
    FACING_NE,
    FACING_E,
    FACING_SE,
    FACING_S,
    FACING_SW,
    FACING_W,
    FACING_NW,
    FACING_COUNT
};
enum Dir256 {
    DIR_N = 0x00,
    DIR_NE = 0x20,
    DIR_E = 0x40,
    DIR_STEP_8 = 0x08
};

class DirType {
public:
    DirType() {}
    DirType(int dir) { Facing = dir; }
    DirType(FacingType dir) { From_Facing(dir); }
    DirType(Dir256 dir) { From_Dir256(dir); }
    void From_Facing(FacingType dir)
    {
        Facing = 0;
        Facing |= (dir << 13);
    }
    void From_Dir256(Dir256 dir)
    {
        Facing = 0;
        Facing |= (dir << 8);
    }
    DirType(double rad) { From_Radian(rad); }
    void From_Radian(double rad) { Facing = short((rad - 1.5707963267948966) * -10430.060040584269); }
    unsigned int Round_To_256() const { return ((((unsigned int)Raw >> 7) + 1) >> 1); }
    int As_Dir256() const { return (int)(Round_To_256() % 256); }
    double As_Radian() const { return (double)((Facing - 16383) * -9.5876725168303271e-05); }
    DirType operator+(DirType const &rvalue) const { return DirType(Facing + rvalue.Facing); }
    DirType operator-(DirType const &rvalue) const { return DirType(Facing - rvalue.Facing); }
    DirType operator/(int rvalue) const { return DirType(Facing / rvalue); }
    DirType const &operator+=(DirType const &rvalue) { Facing += rvalue.Facing; return *this; }
    DirType const &operator-=(DirType const &rvalue) { Facing -= rvalue.Facing; return *this; }
    bool Is_Complete_Turn(DirType const &towards, DirType const &by)
    {
        DirType diff(*this - towards);
        if (abs(by.Facing) >= abs(diff.Facing)) {
            return true;
        }
        return false;
    }
    bool Turn(DirType const &towards, DirType const &by)
    {
        if (Is_Complete_Turn(towards, by)) {
            Facing = towards.Facing;
            return true;
        }
        DirType diff(towards - *this);
        if (diff.Facing < 0) {
            *this -= by;
        } else {
            *this += by;
        }
        return false;
    }
    union {
        short Facing;
        int Raw;
    };
};

template<class T>
class TVelocity2D {
public:
    TVelocity2D() {}
    TVelocity2D(T x, T y) { X = x; Y = y; }
    T Speed() const { return(unknown_sqrt_4ba410(X * X + Y * Y)); }
    void Set_Yaw(DirType const &yaw)
    {
        Nonzero_Check();
        T horizontal_speed = Speed();
        X = horizontal_speed * unknown_cos_4ba4d0(yaw.As_Radian());
        Y = -horizontal_speed * unknown_sin_4ba480(yaw.As_Radian());
    }
    void Yaw(TVelocity2D const &towards, DirType const &rate)
    {
        DirType current_yaw = Get_Yaw();
        DirType desired_yaw = towards.Get_Yaw();
        current_yaw.Turn(desired_yaw, rate);
        Set_Yaw(current_yaw);
    }
    DirType Get_Yaw() const { return(unknown_math_4ba600((double)-Y, (double)X)); }
    void Nonzero_Check() { if (X == 0 && Y == 0) { X = 100; } }
    T X;
    T Y;
};

template<class T>
class TVelocity3D : public TVelocity2D<T> {
    typedef TVelocity2D<T> BASECLASS;
public:
    TVelocity3D() {}
    TVelocity3D(TVelocity3D const &velocity) : BASECLASS(velocity), Z(velocity.Z) {}
    TVelocity3D(T x, T y, T z) : BASECLASS(TVelocity2D<T>(x, y)), Z(z) {}
    T Horizontal_Speed() const { return(unknown_sqrt_4ba410(X * X + Y * Y)); }
    T Speed() const { return(unknown_sqrt_4ba410(X * X + Y * Y + Z * Z)); }
    void Set_Pitch(DirType const &pitch)
    {
        double current_pitch = Get_Pitch().As_Radian();
        T speed = Speed();
        if (current_pitch != 0) {
            X /= unknown_cos_4ba4d0(current_pitch);
            Y /= unknown_cos_4ba4d0(current_pitch);
        }
        X *= unknown_cos_4ba4d0(pitch.As_Radian());
        Y *= unknown_cos_4ba4d0(pitch.As_Radian());
        Z = speed * unknown_sin_4ba480(pitch.As_Radian());
    }
    void Pitch(DirType const &towards, DirType const &rate)
    {
        DirType current_pitch = Get_Pitch();
        current_pitch.Turn(towards, rate);
        Set_Pitch(current_pitch);
    }
    DirType Get_Pitch() const { return(unknown_math_4ba600((double)Z, Horizontal_Speed())); }
    T Z;
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

            int terrain_height = Map.Unknown_0055A280((CoordStruct const &)lookahead_coord);
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
                    desired_pitch.Turn(DirType(Dir256(DIR_E + DIR_STEP_8)), rot / 2);
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

// NATIVE (Scout1121): the C adapter BulletClass::AI's draft (native/drafts/0045f710.cpp) calls;
// CoordStruct/BulletVelocity/UnknownDirStruct there are layout-identical to Coord/TVelocity3D/DirType here.
extern "C" int ra2_native_projectile_motion_593870(void *coord, void *velocity, void const *target, void const *rot,
                                                   bool aircraft, bool airburst, bool very_high, bool level)
{
    return Projectile_Motion(*(Coord *)coord, *(TVelocity3D<double> *)velocity, *(Coord const *)target,
                             *(DirType const *)rot, aircraft, airburst, very_high, level);
}
