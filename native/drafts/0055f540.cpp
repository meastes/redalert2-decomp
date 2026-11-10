// NON-EXACT DRAFT 0x55F540 FUN_0055f540
// entry 0x55F540; exact-match score 98.73 (misses_index best); plan "draft as-is"
// origin lane: Scout1029 (build/class-work/Scout1029/keep/n2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// bool MapClass::Unknown_0055F540(Cell const &cell) (0x0055F540)
// north/south-set-2 twin of 0x0055DEF0 (Damage_Low_Bridge_EW): piece 0x00560C10, spring 0x0055FA60 (draft, Scout1029).
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/tactical.hpp"
extern Rect unknown_rect_839630;
// ref ?Damage_Low_Bridge2_Piece_NS_00560C10MapClassQAEXABVCellZ=0055fcc0
// ref ?Spring_Low_Bridge_Destroyed_EWMapClassQAEXVCellZ=0055ea50
// ref ?unknown_474eb0CellClassQBE?AVRectXZ=00474eb0
// ref ?unknown_475100CellClassQBE?AVRectXZ=00475100
// ref ?Recalc_AttributesCellClassQAEXHZ=00472540
// ref ?Unknown_0047CC10CellClassQAEXXZ=0047cc10
// ref ??$UnionVRectYI?BVRectABV00Z=0045efe0
// ref ??GRectPoint2DQBE?AU0ABU0Z=0047d040
// ref ??0RectQAEABURectPoint2DHHZ=00453e40
// ref ?Register_Dirty_AreaTacticalQAEXVRect_NZ=006a3020
// ref ?unknown_tactical_83996c3PAVTacticalA=0083996c
// ref ?unknown_rect_8396303VRectA=00839630
// ref ?Unknown_00631570RadarClassQAEXABVCellZ=00631570
// ref ?Unknown_0054EBD0MapClassQAEHXZ=0054ebd0
// ref ?Unknown_005687B0MapClassQAEXABVRectZ=005687b0

bool MapClass::Unknown_0055F540(Cell const &cell)
{
    CellClass *cellptr1 = &Map[cell];
    CellClass *cellptr2 = &Map[cell - Cell(1, 0)];
    CellClass *cellptr3 = &Map[cell + Cell(1, 0)];

    int otype = cellptr1->OverlayIndex;
    bool didwork = false;

    Rect draw_rect = Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect());
    Rect recalc_rect(0, 0, 0, 0);

    do {
        if (otype == 0xe3) {
            cellptr2->OverlayIndex = 0xe4;
            cellptr3->OverlayIndex = 0xe4;
            cellptr1->OverlayIndex = 0xe4;
            Damage_Low_Bridge2_Piece_NS_00560C10(cell + Cell(0, 1));
        } else if (otype == 0xe5) {
            cellptr2->OverlayIndex = 0xe6;
            cellptr3->OverlayIndex = 0xe6;
            cellptr1->OverlayIndex = 0xe6;
            Damage_Low_Bridge2_Piece_NS_00560C10(cell - Cell(0, 1));
        } else if (otype < 0xdc) {
            cellptr2->OverlayIndex = 0xdc;
            cellptr3->OverlayIndex = 0xdc;
            cellptr1->OverlayIndex = 0xdc;
            Damage_Low_Bridge2_Piece_NS_00560C10(cell - Cell(0, 1));
            Damage_Low_Bridge2_Piece_NS_00560C10(cell + Cell(0, 1));
        } else if (otype < 0xdf) {
            cellptr2->OverlayIndex = 0xe8;
            cellptr3->OverlayIndex = 0xe8;
            cellptr1->OverlayIndex = 0xe8;
            Map.Radar_Background(cellptr2->MapCoords);
            Map.Radar_Background(cellptr3->MapCoords);
            Map.Radar_Background(cellptr1->MapCoords);
            Damage_Low_Bridge2_Piece_NS_00560C10(cell - Cell(0, 1));
            Damage_Low_Bridge2_Piece_NS_00560C10(cell + Cell(0, 1));
            Unknown_0055FA60(cell);
            didwork = true;
            recalc_rect = Rect(cell.X - 1, cell.Y - 1, 3, 3);
        } else {
            break;
        }

        draw_rect = Union(draw_rect, Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);

        cellptr1->Recalc_Attributes(-1);
        cellptr2->Recalc_Attributes(-1);
        cellptr3->Recalc_Attributes(-1);
        cellptr1->Kill_Illegal_Occupiers();
        cellptr2->Kill_Illegal_Occupiers();
        cellptr3->Kill_Illegal_Occupiers();

        if (didwork) {
            Unknown_0054EBD0();
        }

        if (recalc_rect.Is_Valid()) {
            Unknown_005687B0(recalc_rect);
        }
    } while (false);

    return didwork;
}
