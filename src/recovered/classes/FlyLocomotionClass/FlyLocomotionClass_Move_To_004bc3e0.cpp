// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void FlyLocomotionClass::Move_To(COMCoordStruct to) (0x004BC3E0)
// From ra2-decomp FlyLocomotionClass::Move_To (YR 0x004CCC80).
// Heads for a new destination unless it is the cell already being landed on
// or the unit cannot move (slots 201/202/111/112, unpowered). The null
// coordinate lands an airborne or taking-off unit where it is (else clears
// the destination); any other one is flown to, at flight level above the
// ground while attacking with ammunition, taking off first when needed.
// Twin forms: the COM coordinate taken as a CoordStruct for the cell and null
// tests, the ground height read straight from it, the landing position through
// the object's coordinate accessor (the twin's PositionCoord), and the `locked`
// result of Is_Locked_To_Straight_Flight (inline here, 0x004BFA10).
#include "shared/fly_locomotion.hpp"
#include "shared/cell.hpp"
#include "shared/aircraft.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/techno_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?unknown_coord_865df8@@3UCoordStruct@@A=00865df8

void __stdcall FlyLocomotionClass::Move_To(COMCoordStruct to)
{
    if (CoordStruct(to).As_Cell() != unknown_1c.As_Cell() || !unknown_51) {

        if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202() && !LinkedTo->unknown_slot111() && !LinkedTo->unknown_slot112() && Is_Powered()) {

            if (CoordStruct(to) == unknown_coord_865df8) {
                int landing_altitude = 0;
                UnknownComPtr<IFlyControl,&unknown_uuid_7d80a8> flyctrl(LinkedTo);
                if (flyctrl != 0) {
                    landing_altitude = flyctrl->Landing_Altitude();
                }

                if ((LinkedTo->unknown_slot108() > landing_altitude || unknown_50) && !unknown_51) {
                    unknown_1c = LinkedTo->Position_Coord_004f3f40();
                    unknown_4bf1f0();
                } else {
                    unknown_1c = unknown_coord_865df8;
                }
                unknown_5c = false;
            } else {
                unknown_1c = to;

                if (LinkedTo->unknown_228 && LinkedTo->unknown_254) {
                    unknown_1c.Z = LinkedTo->unknown_slot32()->Flight_Level() + Map.Unknown_0055A280(to);
                }

                UnknownComPtr<IFlyControl,&unknown_uuid_7d80a8> flyctrl(LinkedTo);
                int landing_altitude = 0;
                if (flyctrl != 0) {
                    landing_altitude = flyctrl->Landing_Altitude();
                }

                unknown_34 = true;

                if (LinkedTo->Strength > 0 && !unknown_50 && (unknown_51 || LinkedTo->unknown_slot108() <= landing_altitude)) {
                    Take_Off_004bf0d0();
                }

                bool locked = Is_Locked_To_Straight_Flight_004bfa10();
                unknown_5c = locked;
                if (LinkedTo->Kind_Of() == 2 && !((AircraftClass *)LinkedTo)->Type->Landable) {
                    unknown_5c = true;
                }
            }
        }
    }
}
// @ref ?Query_From@?$UnknownComPtr@UIFlyControl@@$1?unknown_uuid_7d80a8@@3UGUID@@B@@QAEJABQAVFootClass@@@Z=004bfc20
// @ref ?Release_Held@?$UnknownComPtr@UIFlyControl@@$1?unknown_uuid_7d80a8@@3UGUID@@B@@QAEXXZ=004bfbc0
// @ref ??8?$UnknownComPtr@UIFlyControl@@$1?unknown_uuid_7d80a8@@3UGUID@@B@@QBE_NH@Z=004bfb90
