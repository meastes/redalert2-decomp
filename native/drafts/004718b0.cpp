// NON-EXACT DRAFT 0x4718B0 CellClass::unknown_4718b0 (Is_Clear_To_Build)
// entry 0x4718B0; exact-match score 71.99 (misses_index best); plan "draft, verify"
// origin lane: Scout998 (build/class-work/Scout998/w/ctb0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): HEAD's signature (non-const, BuildingTypeClass const *), the overlay h2 inline Tiberium_Type_Here as a file-static helper; marked NATIVE.
// bool CellClass::unknown_4718b0(int speed, BuildingTypeClass *what, HouseClass *who) const (0x004718B0): TibSun Is_Clear_To_Build.
#include "shared/cell.hpp"
#include "shared/building_type_class.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/ground_type.hpp"
#include "shared/rules.hpp"
#include "shared/smudge.hpp"
#include "shared/overlay.hpp"
#include "shared/overlay_type.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/tileset_index.hpp"
#include "shared/house.hpp"

static inline int Tiberium_Type_Here_4718b0(CellClass const *cell)   // NATIVE: overlay h2's inline CellClass::Tiberium_Type_Here
{
    return OverlayClass::To_Tiberium(cell->OverlayIndex);
}

bool CellClass::unknown_4718b0(int speed, BuildingTypeClass const *what, HouseClass *who)   // NATIVE: HEAD's cell_class.hpp signature
{
    if (unknown_a40774) {
        return true;
    }

    if (what != 0) {
        if (what->LaserFence) {
            if (unknown_4717b0() != 0) {
                return false;
            }
            if (Cell_Terrain() != 0) {
                return false;
            }
        } else if (!what->LaserFencePost && !what->Gate) {
            if (what->unknown_bf0 == 0) {
                if (((CellClass *)this)->unknown_471830(Point2D(0, 0), false) != 0) {
                    return false;
                }
                if ((unknown_124 & 0x3F) != 0) {
                    return false;
                }
            } else {
                if (IsoTileIndex >= 0 && IsoTileIndex < IsoTileTypes.Count() && !IsoTileTypes[IsoTileIndex]->unknown_2b0) {
                    return false;
                }
                if (unknown_4717b0() != 0) {
                    return false;
                }
            }
        } else {
            ObjectClass *obj = ((CellClass *)this)->unknown_471830(Point2D(0, 0), false);
            if (obj != 0) {
                if (obj->Kind_Of() != 6) {
                    return false;
                }
                if (!((BuildingClass *)obj)->Type->LaserFence || obj->Owning_House() != who) {
                    return false;
                }
            }
            if ((unknown_124 & 0x3F) != 0) {
                return false;
            }
        }
    }

    if (Debug_Map) {
        if (!Map.Unknown_0054AAB0(MapCoords)) {
            return false;
        }
    } else {
        if (!Map.Unknown_0055A740((CellClass *)this, true)) {
            return false;
        }
    }

    if (OverlayIndex != -1) {
        HouseClass *owner = 0;
        if (unknown_50 >= 0) {
            owner = Houses[unknown_50];
        }

        if (OverlayIndex == 2 || OverlayIndex == 0) {
            if (what != 0) {
                if (what->ToOverlay != 0 && what->ToOverlay->ArrayIndex == OverlayIndex && OverlayData >= 0x10 || what == Rules->unknown_78c || what == Rules->unknown_77c || what == Rules->unknown_780) {
                    if (owner == who) {
                        return true;
                    }
                }
            }
        }

        if (OverlayIndex == 0x1A) {
            if (what != 0) {
                if ((what->ToOverlay != 0 && what->ToOverlay->ArrayIndex == 0x1A && OverlayData >= 0x10) || what == Rules->unknown_784 || what == Rules->unknown_788) {
                    if (owner == who) {
                        return true;
                    }
                }
            }
        }

        if (what != 0 && what->LaserFence) {
            if (OverlayIndex == 0x7E || Tiberium_Type_Here_4718b0(this) != -1) {
                if (unknown_4788d0() || WasUnderBridge || unknown_11c != 0) {
                    return false;
                }
                return true;
            }
        }

        if (!Debug_Map || OverlayTypes[OverlayIndex]->Wall) {
            return false;
        }
    }

    if (speed == -1) {
        if (unknown_4788d0() || WasUnderBridge || unknown_11c != 0) {
            return false;
        }
        if (what->Naval) {
            if (IsoTileIndex < unknown_tileset_a52580 || IsoTileIndex >= unknown_tileset_a52580 + 14) {
                return false;
            }
            return true;
        }
        return Ground[unknown_ec /* NATIVE: Land_Type() */].Build;
    }

    if (Ground[unknown_ec /* NATIVE: Land_Type() */].Cost[speed] == 0) {
        return false;
    }
    return true;
}
