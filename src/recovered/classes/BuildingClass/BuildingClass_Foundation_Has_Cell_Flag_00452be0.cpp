// Test flag 0x400000 on every cell of the building foundation (TibSun building.cpp Should_Fog shape:
// OpenTS, SPDX-License-Identifier: GPL-3.0-or-later; Copyright 2025 Electronic Arts Inc.; Copyright 2026
// OpenTS contributors; EA's GPLv3 Section 7 additional terms apply). The member TPoint2D sum is named
// because 0x00442060's file-local operator+(Cell, Cell) would otherwise be chosen here.
#include "shared/building.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
#include "shared/cell.hpp"
// @ref ??AMapClass@@QAEAAVCellClass@@ABVCell@@@Z=00548070
bool BuildingClass::unknown_452be0()
{
    Cell cell = unknown_slot104();
    Cell const *occupy = Type->Occupy_List(0);
    while (*occupy != Cell(32767, 32767)) {
        if (!(Map[Cell(occupy->operator+(cell))].unknown_140 & 0x400000)) {
            return false;
        }
        occupy++;
    }
    return true;
}
