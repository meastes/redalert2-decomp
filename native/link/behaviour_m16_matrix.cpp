// Behaviour row (Scout1133, native M16; NOT EXACT, native/behaviour_rewrites.json).
// Matrix3D::Matrix3D(COMMatrix3DStruct const &): the COM matrix view of the copy
// constructor at 0x0058FE10 (one body: twelve floats copied). AircraftClass::Draw
// 0x004141C0 converts ILocomotion's Draw_Matrix/Shadow_Matrix results through it.
#define RA2_LOCOMOTION_COM_VALUE_TYPES
#include "shared/matrix3d.hpp"
#include <string.h>
Matrix3D::Matrix3D(COMMatrix3DStruct const &matrix)
{
    memcpy(this, &matrix, sizeof(*this));
}
