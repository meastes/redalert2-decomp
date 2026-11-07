// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void RocketLocomotionClass::Move_To(COMCoordStruct to) (0x0063EA80)
// From ra2-decomp RocketLocomotionClass::Move_To (YR 0x006632E0; RA2 has no
// cruise-missile branch). Picks the V3 or the dreadnought missile's rocket
// data by the aircraft type; when no destination is set yet, starts the
// pause (or, without one, the tilt) state with its frame timer, sets the
// initial pitch (PitchInitial quarter turns) and takes the destination.
// ILocomotion's COM coordinate is stored through CoordStruct's conversion
// (the unit defines RA2_SHIP_MOVE_TO_VALUE in its first row, 0x0063D8D0).
#include "shared/rocket_locomotion.hpp"
#include "shared/aircraft.hpp"
#include "shared/rules.hpp"
// @ref ?unknown_coord_ab6888@@3UCoordStruct@@A=00ab6888

void __stdcall RocketLocomotionClass::Move_To(COMCoordStruct to)
{
    AircraftTypeClass const *type = ((AircraftClass *)LinkedTo)->Type;
    RocketStruct const *rocket;
    if (type == Rules->V3Rocket.Type) {
        rocket = &Rules->V3Rocket;
    } else {
        rocket = &Rules->DMisl;
    }
    if (unknown_18 == unknown_coord_ab6888) {
        if (rocket->PauseFrames != 0) {
            unknown_40 = 1;
            unknown_24 = rocket->PauseFrames;
        } else {
            unknown_40 = 2;
            unknown_24 = rocket->TiltFrames;
        }
        unknown_50 = rocket->PitchInitial * 1.5707963267948966;
        unknown_18 = to;
    }
}
