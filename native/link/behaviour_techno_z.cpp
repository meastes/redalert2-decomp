// TechnoClass::unknown_slot176 (0x006CFEA0) for the native build (Scout1091,
// native M8). NOT EXACT; never original code or bytes. The three adjustments
// are the original's initialised statics (0x007F7124, 0x007F7128, 0x007F712C),
// read from the user's game.exe at run time.
#include "shared/unit.hpp"
#include "shared/unit_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/overlay_type.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/facing.hpp"

namespace ra2port { void *original_data_pointer(unsigned int va); }

namespace {
int Stored_Int(unsigned int va, int fallback)
{
    int const *value = (int const *)ra2port::original_data_pointer(va);
    return value != 0 ? *value : fallback;
}
// AdjacentCell (0x00851A70) by an eight-way facing.
Cell Adjacent(Cell const &cell, int facing)
{
    static int const dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    static int const dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    Cell result;
    result.X = (short)(cell.X + dx[facing & 7]);
    result.Y = (short)(cell.Y + dy[facing & 7]);
    return result;
}
bool Is_Rock(CellClass const *cell)
{
    return cell->OverlayIndex != -1 && OverlayTypes[cell->OverlayIndex]->IsARock;
}
}  // namespace

// 0x006CFEA0 TechnoClass::unknown_slot176 (TibSun TechnoClass::Get_Z_Adjust;
// decompile): the screen rise of the object's height, less 3 for a unit
// tethered to an unloading building, 14 for a harvester on a refinery's cell;
// on a ramp next to a rock (south, east, south-east) the rock adjustment; a
// ramp ahead of or behind the facing gives the second ramp adjustment, a ramp
// here the first; facing north or west (behind) or south or east (ahead), a
// shadowed neighbour or a tile taller than 36 pixels lowers it by 1 or 2.
int TechnoClass::unknown_slot176()
{
    int const ramp1 = Stored_Int(0x007F7124, -1);
    int const ramp2 = Stored_Int(0x007F7128, -1);
    int const rock = Stored_Int(0x007F712C, 3);
    int zadjust = -Unknown_006A2970(unknown_slot110());
    Cell cell = unknown_slot104();
    CellClass *here = &Map[cell];
    if (Kind_Of() == 1) {
        UnitClass *unit = (UnitClass *)this;
        if (unknown_370) {
            TechnoClass *contact = Contact_With_Whom(0);
            if (contact != 0 && contact->Kind_Of() == 6 && contact->Get_Mission() == 0x10) {
                return zadjust - 3;
            }
        }
        if (unit->Type->Harvester) {
            CellClass *cell_ptr = (CellClass *)unknown_slot105();
            BuildingClass *building = cell_ptr != 0 ? (BuildingClass *)cell_ptr->unknown_4717b0() : 0;
            if (building != 0 && building->Type->Refinery) {
                return zadjust - 14;
            }
        }
    }
    if (here->unknown_11c != 0) {
        CellClass *south = &Map[Adjacent(cell, 4)];
        CellClass *east = &Map[Adjacent(cell, 2)];
        CellClass *south_east = &Map[Adjacent(cell, 3)];
        if (south != 0 && east != 0 && south_east != 0) {
            if (south->OverlayIndex != -1) {
                if (Is_Rock(south)) return rock - 1 + zadjust;
            } else if (east->OverlayIndex != -1) {
                if (Is_Rock(east)) return rock - 1 + zadjust;
            } else if (Is_Rock(south_east)) {
                return rock - 1 + zadjust;
            }
        }
    }
    int dir8 = ((unknown_2c8.Current().Raw >> 12) + 1) >> 1 & 7;
    CellClass *ahead_right = &Map[Adjacent(cell, dir8 + 1)];
    CellClass *ahead_left = &Map[Adjacent(cell, dir8 - 1)];
    CellClass *ahead = &Map[Adjacent(cell, dir8)];
    CellClass *behind = &Map[Adjacent(cell, dir8 - 4)];
    CellClass *behind_right = &Map[Adjacent(cell, dir8 + 3)];
    CellClass *behind_left = &Map[Adjacent(cell, dir8 - 3)];
    if (ahead_right == 0 || ahead_left == 0 || ahead == 0) {
        return zadjust - 1;
    }
    if (ahead_right->unknown_11c != 0 || ahead_left->unknown_11c != 0 || ahead->unknown_11c != 0) {
        return zadjust + ramp2;
    }
    if (behind == 0 || behind_right == 0 || behind_left == 0) {
        return zadjust - 1;
    }
    if (behind->unknown_11c != 0 || behind_right->unknown_11c != 0 || behind_left->unknown_11c != 0) {
        return zadjust + ramp2;
    }
    if (here->unknown_11c != 0) {
        return zadjust + ramp1;
    }
    CellClass *cells[3];
    if (dir8 == 6 || dir8 == 0) {
        cells[0] = behind;
        cells[1] = behind_right;
        cells[2] = behind_left;
    } else if (dir8 == 4 || dir8 == 2) {
        cells[0] = ahead_right;
        cells[1] = ahead_left;
        cells[2] = ahead;
    } else {
        return zadjust - 1;
    }
    if (!(here->unknown_140 & 0x10000)) {
        for (int i = 0; i < 3; ++i) {
            if (cells[i]->unknown_140 & 0x10000) {
                return zadjust - 1;
            }
        }
        for (int i = 0; i < 3; ++i) {
            int type = cells[i]->IsoTileIndex;
            int subtile = 0;
            IsometricTileTypeClass *tile;
            if (type == 0xFFFF || type == 0xFF) {
                tile = IsoTileTypes[0];
            } else {
                tile = IsoTileTypes[type];
                subtile = (unsigned char)cells[i]->unknown_11a;
            }
            int width = 0;
            int height = 0;
            tile->Get_Tile_Pixel_Dimensions(subtile, width, height);
            if (height > 36) {
                return zadjust - 2;
            }
        }
    }
    return zadjust - 1;
}
