// bool CellClass::Is_Clear_To_Build(int speed, BuildingTypeClass const *what, HouseClass *who) (0x004718B0):
// TibSun cell.cpp Is_Clear_To_Build with RA2's naval tile check (Is_Tile_Water) and float movement costs. The
// gate/post arm is written before the foundation arm, the laser-fence arm as one condition, and the function ends in
// the if/else: VC6 shares one return-false epilogue from these forms (Scout1228 NOTES, return-false layout).
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

bool CellClass::Is_Clear_To_Build(int speed, BuildingTypeClass const *what, HouseClass *who)
{
    if (unknown_a40774) {
        return true;
    }

    if (what != 0) {
        if (what->LaserFence) {
            if (Cell_Building() != 0 || Cell_Terrain() != 0) {
                return false;
            }
        } else if (what->LaserFencePost || what->Gate) {
            ObjectClass *obj = Cell_Object(Point2D(0, 0), false);
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
        } else {
            if (what->unknown_bf0 == 0) {
                if (Cell_Object(Point2D(0, 0), false) != 0) {
                    return false;
                }
                if ((unknown_124 & 0x3F) != 0) {
                    return false;
                }
            } else {
                if (IsoTileIndex >= 0 && IsoTileIndex < IsoTileTypes.Count() && !IsoTileTypes[IsoTileIndex]->unknown_2b0) {
                    return false;
                }
                if (Cell_Building() != 0) {
                    return false;
                }
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

        if (what != 0 && what->LaserFence && (OverlayIndex == 0x7E || OverlayClass::To_Tiberium(OverlayIndex) != -1)) {
            if (Is_Bridge_Here() || WasUnderBridge || unknown_11c != 0) {
                return false;
            }
            return true;
        }

        if (!Debug_Map || OverlayTypes[OverlayIndex]->Wall) {
            return false;
        }
    }

    if (speed == -1) {
        if (Is_Bridge_Here() || WasUnderBridge || unknown_11c != 0) {
            return false;
        }
        if (what->Naval) {
            if (!Is_Tile_Water()) {
                return false;
            }
            return true;
        }
        return Ground[Land_Type()].Build;
    } else {
        if (Ground[Land_Type()].Cost[speed] == 0) {
            return false;
        }
        return true;
    }
}
