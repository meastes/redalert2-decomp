// NON-EXACT DRAFT 0x6CD340 TechnoClass::unknown_slot186 (747 B)
// entry 0x6CD340; exact-match score 81.96 (lane best), this text 81.96; plan "draft, verify"
// origin lane: Scout753 (build/class-work/Scout753/w/keep/s0_8196.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
// bool TechnoClass::unknown_slot186() (0x006CD340, slot 186)
// Whether the techno may deploy (or unload) where it stands.
#include "shared/techno.hpp"
#include "shared/foot.hpp"
#include "shared/infantry.hpp"
#include "shared/building_type_class.hpp"
#include "shared/house.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
// @ref ?unknown_45ec40@BuildingTypeClass@@QBE_NXZ=0045ec40

bool TechnoClass::unknown_slot186()
{
    Cell where = unknown_slot104();
    CellClass *cell = &Map[where];
    if (cell == 0 || !cell->Is_Near_Tunnel_NW()) {
        switch (Kind_Of()) {
        case 1:
        {
            if (unknown_slot201()) {
                break;
            }
            if (((FootClass *)this)->unknown_574 >= 0) {
                break;
            }
            UnitTypeClass *type = ((UnitClass *)this)->Type;
            if (type->DeploysInto == 0 && type->Passengers == 0 && !type->unknown_bab[0] /* NATIVE: IsSimpleDeployer +0xBAB (foot.hpp's view pads it) */) {
                break;
            }
            if (type->DeploysInto != 0 && type->DeploysInto->ConstructionYard
                && ((FootClass *)this)->unknown_580 != 0) {
                break;
            }
            if (type->Passengers > 0) {
                if (unknown_79) {
                    break;
                }
                Cell center = unknown_slot104();
                CellClass *here = &Map[center];
                if (here != 0) {
                    CellClass *south = &Map[center + AdjacentCell[4]];
                    CellClass *north = &Map[center + AdjacentCell[0]];
                    CellClass *east = &Map[center + AdjacentCell[2]];
                    CellClass *west = &Map[center + AdjacentCell[6]];
                    if (here->unknown_140 & 0x100) {
                        break;
                    }
                    if (south != 0 && (south->unknown_140 & 0x100)) {
                        break;
                    }
                    if (west != 0 && (west->unknown_140 & 0x100)) {
                        break;
                    }
                    if (east != 0 && (east->unknown_140 & 0x100)) {
                        break;
                    }
                    if (north != 0 && (north->unknown_140 & 0x100)) {
                        break;
                    }
                }
                if (House->unknown_134 && here->unknown_ec == 2) {
                    break;
                }
                if (((CellClass *)unknown_slot105())->unknown_4717b0()) {
                    break;
                }
                return true;
            }
            return true;
        }
        case 6:
            return false /* NATIVE: 0x0045EC40 is Leaf_45ec40::get, which returns false (decompile) */;
        case 15:
            if (!((InfantryClass *)this)->unknown_5090b0()
                || (!((InfantryClass *)this)->unknown_5090d0() && ((InfantryClass *)this)->Sequence != 0x1f)) {
                return ((InfantryClass *)this)->unknown_5090a0();
            }
            break;
        default:
            if (!unknown_slot201() && unknown_slot32()->Passengers != 0) {
                return true;
            }
            return false;
        }
    }
    return false;
}
