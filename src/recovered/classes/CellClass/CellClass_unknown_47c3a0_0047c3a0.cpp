// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool CellClass::unknown_47c3a0(int tiberium, int data) (0x0047C3A0): Place_Tiberium. Adapted from ra2-decomp src/cell.cpp
// CellClass::Place_Tiberium (gamemd 0x00487190, MATCHED there; TibSun shape). VectorClass::operator[](int) (the
// twin's and TibSun's signature, now the shared default) is required; with unsigned the grow branch's second
// Union swaps eax/ebx.
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/overlay.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tiberium.hpp"
#include "shared/id_source.hpp"

bool CellClass::unknown_47c3a0(int tib, int data)
{
    TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[tib];
    if (data < tiberium->unknown_e4) {
        if (Can_Tiberium_Germinate(tiberium)) {
            if (unknown_11c != 0) {
                new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + tiberium->NumFrames + 2 * unknown_11c + (Random_Pick(0, 1) - 2)], Fetch_CellID());
            } else {
                new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + Random_Pick(0, 11)], Fetch_CellID());
            }
            tiberium->Add_Growth_Cell(MapCoords);
            OverlayData = data;
            unknown_47c080();
            return true;
        }

        if (unknown_4788e0()) {
            if (Get_Tiberium_Type() == tib) {
                OverlayData += data;
                OverlayData = OverlayData < tiberium->unknown_e4 - 1 ? OverlayData : tiberium->unknown_e4 - 1;
                Rect rect = Union(Union(unknown_4752b0(), unknown_474eb0()), unknown_475100());
                unknown_tactical_83996c->Register_Dirty_Area(rect - unknown_rect_839630.Top_Left(), 0);
                tiberium->Add_Spread_Cell(MapCoords);
                return true;
            }
        }
    }
    return false;
}
