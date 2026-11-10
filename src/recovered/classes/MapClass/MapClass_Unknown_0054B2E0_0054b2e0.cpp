// int MapClass::Unknown_0054B2E0() (0x0054B2E0)
// TibSun Overpass: the final pass over a freshly loaded map. Deletes the animations
// marked for it (+0x177), clears every cell's cross-line flags (+0x140 bits 20-21),
// then per cell clears +0x30, rebuilds its drawer, clears the anim-attached bit (17),
// marks the row or column of a cell whose tag crosses horizontally or vertically,
// adjusts its tiberium (summing the values), recalculates it and gives a wall its owner.
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_techno.hpp"
#include "shared/overlay_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Anims@@3V?$DynamicVectorClass@PAVAnimClass@@@@A=00a40960
// @ref ?unknown_overlay_types_a35de8@@3V?$DynamicVectorClass@PAVUnknownOverlayType@@@@A=00a35de8

int MapClass::Unknown_0054B2E0()
{
    int i;
    int value = 0;

    for (i = 0; i < Anims.Count(); i++) {
        if (Anims[i]->unknown_177) {
            delete Anims[i];
            i--;
        }
    }

    Reset_Iterator();

    CellClass *iter = Iterate();
    while (iter) {
        iter->IsHorizontalLine = false; iter->IsVerticalLine = false;
        iter = Iterate();
    }

    Reset_Iterator();

    iter = Iterate();
    while (iter) {

        iter->unknown_30 = 0;
        iter->Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
        iter->IsAnimAttached = false;

        if (iter->unknown_3c) {

            if (iter->unknown_3c->unknown_func_6b3da0()) {
                for (i = 0; i < Map.unknown_128; i++) {
                    Map[XY_Cell(i + unknown_120, iter->MapCoords.Y)].IsHorizontalLine = true;
                }
            }
            else if (iter->unknown_3c->unknown_func_6b3d90()) {
                for (i = 0; i < Map.unknown_12c; i++) {
                    Map[XY_Cell(iter->MapCoords.X, i + unknown_124)].IsVerticalLine = true;
                }
            }
        }

        value += iter->Tiberium_Adjust(false);
        iter->Recalc_Attributes(-1);
        if (iter->OverlayIndex != -1) {
            if (unknown_overlay_types_a35de8[iter->OverlayIndex]->unknown_278) {
                iter->Set_Wall_Owner();
            }
        }

        iter = Iterate();
    }
    return value;
}
