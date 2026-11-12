// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// CoordStruct BuildingClass::unknown_slot41(int dock) const 0x004440E0 (slot 41; TibSun building.cpp
// Docking_Coord): a weeder's dock two cells right and one down of its position cell at its height, a
// refinery's half a cell right of the object's dock, and for a helipad or repair depot the docking
// offset of the contact's dock index (NumberOfDocks/DockingOffsets). The function-scope `coord` the dock sums
// are assigned to is from ra2-decomp BuildingClass::Docking_Coord (YR 0x00447B20, src/building.cpp); the sums use
// TPoint3D<int>::operator+ (RA2_COORD_TPOINT3D).
#include "shared/building.hpp"
// @ref ?unknown_slot41@ObjectClass@@UBE?AUCoordStruct@@H@Z=005d54f0
// @ref ?FindLinkIndex@RadioClass@@QBEHPAVTechnoClass@@@Z=00636e70
CoordStruct BuildingClass::unknown_slot41(int dock) const
{
    if (Type->Weeder) {
        Cell cell = unknown_slot104() + Cell(2, 1);
        CoordStruct coord = CoordStruct(cell);
        coord.Z = CoordStruct(Coord).Z;
        return coord;
    }
    CoordStruct coord;

    if (Type->Refinery) {
        return ObjectClass::unknown_slot41(dock) + CoordStruct(128, 0, 0);
    }
    if (!Type->Helipad && !Type->UnitRepair) {
        return ObjectClass::unknown_slot41(dock);
    }
    switch (Type->NumberOfDocks) {
        case 0:
            return Center_Coord();

        case 1:
            coord = Center_Coord() + Type->DockingOffsets[0];
            return coord;

        default:
            {
                int index = FindLinkIndex((TechnoClass *)dock);
                if (index >= 0 && index < Type->NumberOfDocks) {
                    coord = Center_Coord() + Type->DockingOffsets[index];
                    return coord;
                }
            }
            return Center_Coord();
    }
}
