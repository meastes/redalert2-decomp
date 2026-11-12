// int TechnoClass::unknown_slot176() (0x006CFEA0)
// TibSun TechnoClass::Get_Z_Adjust: the drawing depth adjustment from height, ramps and rocks.
// Shape from the TibSun reconstruction (techno.cpp Get_Z_Adjust, inline.h Adjacent_Cell, facing.h
// Facing_Add/Facing_Sub; 2026 OpenTS contributors, GPL-3.0-or-later, see THIRD_PARTY.md). The YR twin
// (gamemd 0x00704350) compiles to the same instructions.
#include "shared/techno.hpp"
#include "shared/foot.hpp"
#include "shared/techno_map.hpp"
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
#include "shared/tactical.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tileset_index.hpp"
#include "shared/facing.hpp"
#include "shared/house_adjacent.hpp"
// @ref ?AdjacentCell@@3QBVCell@@B=00851a70
// @ref ?unknown_techno_zadj_ramp1_7f7124@@3HA=007f7124
// @ref ?unknown_techno_zadj_ramp2_7f7128@@3HA=007f7128
// @ref ?unknown_techno_zadj_rock_7f712c@@3HA=007f712c
extern int unknown_techno_zadj_ramp1_7f7124;
extern int unknown_techno_zadj_ramp2_7f7128;
extern int unknown_techno_zadj_rock_7f712c;
extern Cell const AdjacentCell[8];
// The sum or difference of two eight-way facings (TibSun facing.h).
inline FacingType Facing_Add(int first, int second) { return (FacingType)((first + second) & 7); }
inline FacingType Facing_Sub(int first, int second) { return (FacingType)((first - second) & 7); }
// Whether an overlay is a rock (the three rock tests by the ramp).
inline bool Overlay_Is_Rock(int overlay)
{
    OverlayTypeClass *otype = OverlayTypes[overlay];
    return otype->IsARock;
}
int TechnoClass::unknown_slot176()
{
    CellClass *cptrs[3];
    int width;
    int height;
    int z = -Unknown_006A2970(Get_Height());
    int zadjust2 = z;
    int zadjust = z;
    Cell cell = unknown_slot104();
    CellClass *cptr = (CellClass *)&Map[cell];
    UnitClass *unit = (UnitClass *)this;
    if (unit->Kind_Of() == 1) {
        if (unit->unknown_370) {
            BuildingClass *building = (BuildingClass *)unit->Contact_With_Whom(0);
            if (building->Kind_Of() == 6 && building->Get_Mission() == 16) {
                return zadjust2 - 3;
            }
        }
        if (unit->Type->Harvester) {
            BuildingClass *building = (BuildingClass *)((CellClass *)Get_Cell_Ptr())->Cell_Building();
            if (building != 0 && building->Type->Refinery) {
                return zadjust2 - 14;
            }
        }
    }
    if (cptr->unknown_11c) {
        CellClass *cell_s = (CellClass *)&Map[Adjacent_Cell(cell, FACING_S)];
        CellClass *cell_e = (CellClass *)&Map[Adjacent_Cell(cell, FACING_E)];
        CellClass *cell_se = (CellClass *)&Map[Adjacent_Cell(cell, FACING_SE)];
        if (cell_s != 0 && cell_e != 0 && cell_se != 0) {
            bool rock = false;
            if (cell_s->OverlayIndex != -1) {
                if (Overlay_Is_Rock(cell_s->OverlayIndex)) {
                    rock = true;
                }
            } else if (cell_e->OverlayIndex != -1) {
                if (Overlay_Is_Rock(cell_e->OverlayIndex)) {
                    rock = true;
                }
            } else if (cell_se->OverlayIndex != -1) {
                if (Overlay_Is_Rock(cell_se->OverlayIndex)) {
                    rock = true;
                }
            }
            if (rock) {
                return unknown_techno_zadj_rock_7f712c + zadjust2 - 1;
            }
        }
    }
    CellClass *ahead_right = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Add(Dir_To_8(unknown_2e0.Current()), 1))];
    CellClass *ahead_left = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Sub(Dir_To_8(unknown_2e0.Current()), 1))];
    CellClass *ahead = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Add(Dir_To_8(unknown_2e0.Current()), 0))];
    CellClass *behind = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Sub(Dir_To_8(unknown_2e0.Current()), 4))];
    CellClass *behind_right = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Add(Dir_To_8(unknown_2e0.Current()), 3))];
    CellClass *behind_left = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Sub(Dir_To_8(unknown_2e0.Current()), 3))];
    bool ramp = false;
    if (ahead_right != 0 && ahead_left != 0 && ahead != 0) {
        if (ahead_right->unknown_11c == 0 && ahead_left->unknown_11c == 0 && ahead->unknown_11c == 0) {
            if (behind != 0 && behind_right != 0 && behind_left != 0) {
                if (behind->unknown_11c == 0 && behind_right->unknown_11c == 0 && behind_left->unknown_11c == 0) {
                    if (cptr->unknown_11c) {
                        return zadjust + unknown_techno_zadj_ramp1_7f7124;
                    }
                    if (Dir_To_8(unknown_2e0.Current()) == 6 || Dir_To_8(unknown_2e0.Current()) == 0) {
                        cptrs[0] = behind;
                        cptrs[1] = behind_right;
                        cptrs[2] = behind_left;
                    } else if (Dir_To_8(unknown_2e0.Current()) == 4 || Dir_To_8(unknown_2e0.Current()) == 2) {
                        cptrs[0] = ahead_right;
                        cptrs[1] = ahead_left;
                        cptrs[2] = ahead;
                    } else {
                        return zadjust - 1;
                    }
                    if ((cptr->unknown_140 & 0x10000) != 0) {
                        return zadjust - 1;
                    }
                    for (int i = 0; i < 3; i++) {
                        if ((cptrs[i]->unknown_140 & 0x10000) != 0) {
                            return zadjust - 1;
                        }
                    }
                    for (int j = 0; j < 3; j++) {
                        int ittype = cptrs[j]->IsoTileIndex;
                        int subtile;
                        IsometricTileTypeClass *iptr;
                        if (ittype == 0xFFFF || ittype == 0xFF) {
                            iptr = IsoTileTypes[unknown_tileset_a52f00];
                            subtile = 0;
                        } else {
                            iptr = IsoTileTypes[ittype];
                            subtile = (unsigned char)cptrs[j]->unknown_11a;
                        }
                        iptr->Get_Tile_Pixel_Dimensions(subtile, width, height);
                        if (height > 36) {
                            return zadjust - 2;
                        }
                    }
                    return zadjust - 1;
                } else {
                    ramp = true;
                }
            }
        } else {
            ramp = true;
        }
    }
    if (ramp) {
        return unknown_techno_zadj_ramp2_7f7128 + zadjust;
    }
    return zadjust - 1;
}
