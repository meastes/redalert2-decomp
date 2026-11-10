#ifndef RA2_SHARED_BULLET_VELOCITY_HPP
#define RA2_SHARED_BULLET_VELOCITY_HPP
#ifndef RA2_BULLET_MISSILE_VELOCITY
#include "shared/bullet.hpp"
#else
#include "shared/dir_type.hpp"
#include "shared/fast_math.hpp"

// Same existing three-double TVelocity3D<double> representation, using the Missile unit's direction view.
// Westwood's TVelocity2D<T> and TVelocity3D<T> (TibSun velocity.h). RA2 uses the double
// instances only; they are written as explicit specializations carrying the recovered
// members (a real template is a separate round).
template <class T> struct TVelocity2D;
template <class T> struct TVelocity3D;
template <> struct TVelocity2D<double> {
    TVelocity2D() {}
    TVelocity2D(double x, double y) : X(x), Y(y) {}
    double Speed() const { return Fast_Sqrt(X * X + Y * Y); }
    void Nonzero_Check() { if (X == 0 && Y == 0) X = 100; }
    double X;
    double Y;
};
template <> struct TVelocity3D<double> : public TVelocity2D<double> {
    TVelocity3D() {}
    TVelocity3D(TVelocity3D const &velocity) : TVelocity2D<double>(velocity), Z(velocity.Z) {}
    TVelocity3D(double x, double y, double z) : TVelocity2D<double>(TVelocity2D<double>(x,y)), Z(z) {}
    TVelocity3D<double> &operator+=(TVelocity3D<double> const &other);
    TVelocity2D<double> &XY() { return *this; }
    TVelocity2D<double> const &XY() const { return *this; }
    double Horizontal_Speed() const { return Fast_Sqrt(X * X + Y * Y); }
    double Speed() const { return Fast_Sqrt(X * X + Y * Y + Z * Z); }
    void Nonzero_Check() { if (X == 0 && Y == 0 && Z == 0) X = 100; }
    DirType Get_Yaw() const { return Fast_Atan2(-Y, X); }
#ifdef RA2_BULLET_MISSILE_GET_PITCH_SOURCE
    DirType Get_Pitch() const;
#else
    DirType Get_Pitch() const { double const &horizontal = TVelocity2D<double>::Speed(); return Fast_Atan2(Z, horizontal); }
#endif
    void Set_Yaw(DirType const &yaw)
    {
        TVelocity2D<double>::Nonzero_Check();
        double horizontal_speed = Horizontal_Speed();
        X = horizontal_speed * Fast_Cos(yaw.As_Radian());
        Y = -horizontal_speed * Fast_Sin(yaw.As_Radian());
    }
#ifdef RA2_BULLET_MISSILE_SET_PITCH_SOURCE
    void Set_Pitch(DirType const &pitch);
#else
    void Set_Pitch(DirType const &pitch)
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
#endif
    void Set_Speed(double speed)
    {
        Nonzero_Check();
        double ratio = speed / Speed();
        X *= ratio;
        Y *= ratio;
        Z *= ratio;
    }
    double Z;
};

#endif
#endif
