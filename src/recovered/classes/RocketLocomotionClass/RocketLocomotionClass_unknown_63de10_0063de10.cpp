// double RocketLocomotionClass::unknown_63de10() (0x0063DE10)
#include "shared/rocket_locomotion.hpp"
#include "shared/fast_math.hpp"
#include "shared/point.hpp"

static inline int Square(int x) { return x * x; }

// double RocketLocomotionClass::unknown_63de10() (0x0063DE10): the pitch from
// the rocket to its destination, the arctangent of the height over the ground
// distance (straight down with no ground distance); TibSun/YR
// RocketLocomotionClass::Pitch_To_Target (ra2-decomp rocket.cpp 0x00662240).
// An ordinary member: this copy has no callers, Process auto-inlines its two
// calls. Its square sum needs CoordStruct on TPoint3D (RA2_COORD_TPOINT3D in
// the unit's first row).
double RocketLocomotionClass::unknown_63de10()
{
    CoordStruct coord = Object_Coord(LinkedTo);
    CoordStruct delta = unknown_18 - coord;
    float distance = Fast_Sqrt((double)(Square(delta.X) + Square(delta.Y)));
    if (distance > 0.0) {
        return Fast_Atan((double)(delta.Z / distance));
    }
    return -1.5707963267948966;
}

