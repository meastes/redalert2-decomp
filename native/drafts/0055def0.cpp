// NON-EXACT DRAFT 0x55DEF0 FUN_0055def0
// entry 0x55DEF0; exact-match score 98.73 (misses_index best); plan "draft as-is"
// origin lane: Scout1029 (build/class-work/Scout1029/keep/e0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// bool MapClass::Unknown_0055DEF0(Cell const &cell) (0x0055DEF0)
// Tiberian Sun's Damage_Low_Bridge_EW: damages the three cells of an east/west
// low bridge piece one step (0x5C -> 0x5D, 0x5E -> 0x5F, below 0x50 -> 0x50,
// below 0x53 -> 0x64 destroyed), damages the pieces beside it (0x0055FCC0),
// redraws the overlays, recalculates the cells and, once destroyed, springs the
// span's triggers (0x0055EA50) and resets the zones. True when it gave way.
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/tactical.hpp"
extern Rect unknown_rect_839630;
// @ref ?Damage_Low_Bridge_Piece_EW_0055FCC0@MapClass@@QAEXABVCell@@@Z=0055fcc0
// @ref ?Spring_Low_Bridge_Destroyed_EW@MapClass@@QAEXVCell@@@Z=0055ea50
// @ref ?unknown_474eb0@CellClass@@QBE?AVRect@@XZ=00474eb0
// @ref ?unknown_475100@CellClass@@QBE?AVRect@@XZ=00475100
// @ref ?Recalc_Attributes@CellClass@@QAEXH@Z=00472540
// @ref ?Unknown_0047CC10@CellClass@@QAEXXZ=0047cc10
// @ref ??$Union@VRect@@@@YI?BVRect@@ABV0@0@Z=0045efe0
// @ref ??GRectPoint2D@@QBE?AU0@ABU0@@Z=0047d040
// @ref ??0Rect@@QAE@ABURectPoint2D@@HH@Z=00453e40
// @ref ?Register_Dirty_Area@Tactical@@QAEXVRect@@_N@Z=006a3020
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?Unknown_00631570@RadarClass@@QAEXABVCell@@@Z=00631570
// @ref ?Unknown_0054EBD0@MapClass@@QAEHXZ=0054ebd0
// @ref ?Unknown_005687B0@MapClass@@QAEXABVRect@@@Z=005687b0

bool MapClass::Unknown_0055DEF0(Cell const &cell)
{
    CellClass *cellptr1 = &Map[cell];
    CellClass *cellptr2 = &Map[cell - Cell(0, 1)];
    CellClass *cellptr3 = &Map[cell + Cell(0, 1)];

    int otype = cellptr1->OverlayIndex;
    bool didwork = false;

    Rect draw_rect = Union(cellptr1->unknown_474eb0(), cellptr1->unknown_475100());
    Rect recalc_rect(0, 0, 0, 0);

    do {
        if (otype == 0x5c) {
            cellptr2->OverlayIndex = 0x5d;
            cellptr3->OverlayIndex = 0x5d;
            cellptr1->OverlayIndex = 0x5d;
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell - Cell(1, 0));
        } else if (otype == 0x5e) {
            cellptr2->OverlayIndex = 0x5f;
            cellptr3->OverlayIndex = 0x5f;
            cellptr1->OverlayIndex = 0x5f;
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell + Cell(1, 0));
        } else if (otype < 0x50) {
            cellptr2->OverlayIndex = 0x50;
            cellptr3->OverlayIndex = 0x50;
            cellptr1->OverlayIndex = 0x50;
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell - Cell(1, 0));
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell + Cell(1, 0));
        } else if (otype < 0x53) {
            cellptr2->OverlayIndex = 0x64;
            cellptr3->OverlayIndex = 0x64;
            cellptr1->OverlayIndex = 0x64;
            Map.Unknown_00631570(cellptr2->MapCoords);
            Map.Unknown_00631570(cellptr3->MapCoords);
            Map.Unknown_00631570(cellptr1->MapCoords);
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell - Cell(1, 0));
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell + Cell(1, 0));
            Spring_Low_Bridge_Destroyed_EW(cell);
            didwork = true;
            recalc_rect = Rect(cell.X - 1, cell.Y - 1, 3, 3);
        } else {
            break;
        }

        draw_rect = Union(draw_rect, Union(cellptr1->unknown_474eb0(), cellptr1->unknown_475100())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);

        cellptr1->Recalc_Attributes(-1);
        cellptr2->Recalc_Attributes(-1);
        cellptr3->Recalc_Attributes(-1);
        cellptr1->Unknown_0047CC10();
        cellptr2->Unknown_0047CC10();
        cellptr3->Unknown_0047CC10();

        if (didwork) {
            Unknown_0054EBD0();
        }

        if (recalc_rect.Is_Valid()) {
            Unknown_005687B0(recalc_rect);
        }
    } while (false);

    return didwork;
}
