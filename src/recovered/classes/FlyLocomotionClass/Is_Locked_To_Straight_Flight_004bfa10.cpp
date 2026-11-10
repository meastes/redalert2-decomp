// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// @ref ?Release_Held@?$UnknownComPtr@UIFlyControl@@$1?unknown_uuid_7d80a8@@3UGUID@@B@@QAEXXZ=004bfbc0
// bool FlyLocomotionClass::Is_Locked_To_Straight_Flight() (0x004BFA10), from ra2-decomp (YR 0x004D02B0):
// the comip pointer tested with != 0 (comip operator!=(int)).
#include "shared/fly_locomotion.hpp"
#include "shared/aircraft.hpp"
#include "shared/map_view.hpp"
bool FlyLocomotionClass::Is_Locked_To_Straight_Flight_004bfa10()
{
    if (unknown_1c.Z > Map.Get_Height_GL(unknown_1c) + 120)
        return true;
    if (LinkedTo->unknown_228 != 0 && LinkedTo->unknown_254 != 0)
        return true;
    UnknownComPtr<IFlyControl, &unknown_uuid_7d80a8> flyctrl(LinkedTo);
    if (flyctrl != 0) {
        if (flyctrl->Is_Locked())
            return true;
    }
    return false;
}
