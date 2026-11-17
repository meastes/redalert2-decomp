#define RA2_DIR_MISSILE_ANGLES
#define RA2_BULLET_MISSILE_VELOCITY
#define RA2_BULLET_MISSILE_GET_PITCH_SOURCE
#include "shared/bullet_velocity.hpp"

DirType TVelocity3D<double>::Get_Pitch() const
{
    double const &horizontal = TVelocity2D<double>::Speed();
    return Fast_Atan2(Z, horizontal);
}
