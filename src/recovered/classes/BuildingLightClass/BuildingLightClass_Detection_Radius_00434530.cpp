// int BuildingLightClass::Detection_Radius() const (0x00434530)
// The distance step (Sweep_Stage) scaled by 5.973333333333333 and
// offset by the rules value at +0x6D0. TibSun blight.cpp Detection_Radius
// shape (OpenTS; SPDX-License-Identifier: GPL-3.0-or-later; see
// THIRD_PARTY.md): the scale is its function-local `static const double
// _scale` (0x0079C7A8, read from memory) and the result is a named radius;
// the cast-and-add initializer (TibSun: `radius += ...` as a second
// statement) gives slot 67's register order for the inlined add.
#include "shared/building_light.hpp"
#include "shared/rules.hpp"
// @ref ?Sweep_Stage@BuildingLightClass@@QBEHXZ=00434610
// @ref _?_scale@?1??Detection_Radius@BuildingLightClass@@QBEHXZ@4NB=0079c7a8

int BuildingLightClass::Detection_Radius() const
{
    static const double _scale = 5.973333333333333;
    int radius = (int)(Sweep_Stage() * _scale) + Rules->unknown_6d0;
    return(radius);
}
