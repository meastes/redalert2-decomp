// NON-EXACT DRAFT 0x47C3A0 FUN_0047c3a0
// entry 0x47C3A0; exact-match score 98.92 (misses_index best); plan "draft as-is"
// origin lane: Scout674 (build/class-work/Scout674/w/keep/place_tiberium_9891.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// bool CellClass::unknown_47c3a0(int tiberium, int data) (0x0047C3A0): TibSun Place_Tiberium.
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
                new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + 2 * unknown_11c + tiberium->NumFrames + Random_Pick(0, 1) - 2], Fetch_CellID());
            } else {
                new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + Random_Pick(0, 11)], Fetch_CellID());
            }
            tiberium->Add_Growth_Cell(MapCoords);
            OverlayData = data;
            unknown_47c080();
            return true;
        }

        if (unknown_4788e0()) {
            if (OverlayClass::To_Tiberium(OverlayIndex) == tib) {
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
