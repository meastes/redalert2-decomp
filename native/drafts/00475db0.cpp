// NON-EXACT DRAFT 0x475DB0 FUN_00475db0
// entry 0x475DB0; exact-match score 98.91 (misses_index best); plan "rewrite: asm-shaped"
// origin lane: Scout998 (build/class-work/Scout998/w/rt0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// int CellClass::unknown_475db0(int levels) (0x00475DB0): Westwood's
// Reduce_Tiberium: takes up to levels stages of the cell's ore and answers how
// many it took. An emptied cell loses its overlay (Unknown_00472540(-1)), is
// redrawn on the radar, leaves the spread lists and queues its ore-free
// neighbours for spreading; the overlay area is registered for redraw.
#include "shared/cell.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/overlay.hpp"
#include "shared/tiberium.hpp"
#include "shared/tactical.hpp"
#include "shared/veinhole.hpp"

int CellClass::unknown_475db0(int levels)
{
    Rect dirty = Union(unknown_474eb0(), unknown_475100());
    dirty.Y -= unknown_rect_839630.Y;

    int tibtype = OverlayClass::To_Tiberium(OverlayIndex);
    int reducer = levels;

    if (levels > 0 && tibtype != -1) {
        TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[tibtype];
        if (OverlayData == 11) {
            tiberium->Add_Growth_Cell(MapCoords);
        }
        if (OverlayData + 1 > levels) {
            OverlayData -= levels;
            reducer = levels;
        } else {
            reducer = OverlayData;
            OverlayIndex = -1;
            OverlayData = 0;
            Recalc_Attributes(-1);
            Map.Radar_Background(MapCoords);
            TiberiumClass::Clear_Spread_Cell(MapCoords);
            for (int facing = 0; facing < 8; facing++) {
                Cell adjacent = Adjacent_Cell(MapCoords, facing);
                if (Map.Unknown_0054AAB0(adjacent) && !tiberium->SpreadMap[unknown_cell_index_428a90(adjacent)]) {
                    tiberium->Add_Spread_Cell(adjacent);
                }
            }
        }
        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
        return reducer;
    }
    return 0;
}
