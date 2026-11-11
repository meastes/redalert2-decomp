"""Typed scratch rendering of the frozen Building Exit_Object export."""
import hashlib
from types import SimpleNamespace
from seed_guarded_cfg import guarded_graph, require

ENTRY=0x440520
RAW_SHA256='924315e3fd3d88fde9fd32796aa8c554e64da4f978b96334bf8dfcd14c82a8cc'
BODY_SHA256='51242dcd5c6bf4e83609187b51bad670a811f93d32546063ac55b040b226b931'
PROVIDERS={4290704: ('?Pose_Dir_00417890@AircraftClass@@QBEIXZ', 0), 4375456: ('?Next_Buildable@BaseClass@@QAEPAVBaseNodeClass@@H@Z', 4), 4375504: ('?Next_Buildable_Index@BaseClass@@QAEHH@Z', 4), 4377456: ('?unknown_42cb70@BaseClass@@QAEHPAVBaseNodeClass@@@Z', 4), 4557104: ('?Width@BuildingTypeClass@@QBEHXZ', 0), 4557120: ('?Foundation_Height@BuildingTypeClass@@QBEH_N@Z', 4), 4557584: ('?unknown_458b10@BuildingTypeClass@@QBEHABVCell@@PAVHouseClass@@@Z', 8), 4658784: ('?Cell_Techno@CellClass@@QBEPAVObjectClass@@ABUPoint2D@@_NPAV2@@Z', 12), 4659120: ('?Cell_Building@CellClass@@QBEPAVObjectClass@@XZ', 0), 4676960: ('?unknown_475d60@CellClass@@QBE?AUCoordStruct@@XZ', 4), 4950800: ('?Set_Current@FacingClass@@QAE_NABUUnknownFacingDir@@@Z', 4), 4957696: ('?Fast_Atan2@@YAMNN@Z', 0), 5016496: ('?unknown_4c8bb0@FootClass@@QAEXPAVAbstractClass@@@Z', 4), 5180976: ('?unknown_func_4f0e30@HouseClass@@QAE?AVCell@@PAVBuildingTypeClass@@@Z', 8), 5191456: ('?unknown_func_4f3720@HouseClass@@QAEXH@Z', 4), 5198576: ('?unknown_func_4f52f0@HouseClass@@QAE_NXZ', 0), 5198624: ('?unknown_func_4f5320@HouseClass@@QAE_NPAVBuildingTypeClass@@ABVCell@@@Z', 8), 5373056: ('?get@Leaf_51fc80@@QAE_NXZ', 0), 5537904: ('??AMapClass@@QAEAAVCellClass@@ABVCell@@@Z', 4), 5571296: ('?Nearby_Location@MapClass@@QAE?AVCell@@ABV2@HHH_NUPoint2D@@11110H@Z', 56), 5613152: ('?Unknown_0055A660@MapClass@@QAE_NABVCell@@_N@Z', 8), 5678208: ('?Unknown_0056A480@MapClass@@QAEXABVCell@@PAVHouseClass@@PAVBuildingTypeClass@@@Z', 12), 5678688: ('?Unknown_0056A660@MapClass@@QAEXABVCell@@PAVHouseClass@@PAVBuildingTypeClass@@@Z', 12), 6113568: ('?unknown_func_5d4920@ObjectClass@@QAEXH@Z', 4), 6516384: ('?Has_Free_Slot@UnknownList_636ea0@@QBE_NXZ', 0), 6516432: ('?Unknown_00636ED0@RadioClass@@QBE_NPAVTechnoClass@@@Z', 4), 6516496: ('?In_Radio_Contact@RadioClass@@QBE_NXZ', 0), 6523040: ('??RRandom2Class@@QAEHHH@Z', 8), 7171664: ('?unknown_6d6e50@TechnoClass@@QAEXPAVAbstractClass@@@Z', 4), 5157488: ('?Unknown_004EB270@HouseClass@@QAE?AVCell@@PAVFootClass@@@Z', 8), 5178304: ('?Where_To_Place_Building@HouseClass@@QAE?AVCell@@PAVBuildingTypeClass@@P6IHPAV1@ABV2@HH@ZH@Z', 16), 7139632: ('?unknown_6cf130@TechnoClass@@QAE?AVCell@@PAVAbstractClass@@@Z', 8), 5178000: ('?House_Base_Distance_Score@@YIHPAVHouseClass@@ABVCell@@HH@Z', 8)}
CANONICAL_EXTENTS={5157488: (240, 'c63b2b748d25dd792da2f008ca6d6bb031d4109b814d1e938684ffe15a44c6c4', 8), 5178304: (2629, '294c38af5ea58e1a7a5b550a2ae97e406cc0f41e7196bb6a42ec9fea3ac31717', 16), 7139632: (284, 'fe7f6f14de060a3c7a7fe01968176925b2594d544e5d27f0bcb6dec1922315ab', 8)}

def canonical_cleanup(receivers,entry):
    # These three pass7 inventory rows omit reachable epilogues. Their complete
    # extents are already independently accepted in the canonical catalog.
    # This seed-local path neither infers bounds from a candidate nor changes
    # inventory, generic receiver checks, or acceptance tooling.
    size,digest,expected=CANONICAL_EXTENTS[entry]
    row=receivers.accepted(entry)
    require(row['body_bytes']==size and row['body_sha256']==digest,
            'Accepted Building provider extent differs')
    raw=receivers.audit.original(entry,size)
    require(hashlib.sha256(raw).hexdigest()==digest,'Accepted Building provider bytes differ')
    bound=dict(contiguous=True,ranges=[dict(start=f'{entry:08x}',
        end_inclusive=f'{entry+size-1:08x}',bytes_hex=raw.hex())])
    evidence=SimpleNamespace(function=lambda address:(None,bound) if address==entry else receivers.inventory.function(address))
    graph=guarded_graph(receivers.audit,evidence,entry)
    widths={int(i.operands[0].imm) if i.operands else 0 for i in graph['nodes'].values() if i.mnemonic=='ret'}
    require(widths=={expected},'Accepted Building provider cleanup differs')
    return expected

def verify_graph(graph,receivers):
    require(graph['body_sha256']==BODY_SHA256,'Frozen Building exit body differs')
    for target,(symbol,cleanup) in PROVIDERS.items():
        require(receivers.accepted(target)['symbol']==symbol,'Building exit provider symbol differs')
        actual=canonical_cleanup(receivers,target) if target in CANONICAL_EXTENTS else receivers.cleanup(target)
        require(actual==cleanup,'Building exit provider cleanup differs')

def verify_native(inventory,repository):
    import catalog_io
    from seed_capture_receivers import Receivers
    from verify_manifest import Audit
    audit=Audit(repository,'private/original-target-20261020/inputs/game.exe')
    graph=guarded_graph(audit,inventory,ENTRY)
    catalog=catalog_io.load(catalog_io.locate(repository),references=False)
    receivers=Receivers(repository,catalog,audit,inventory,graph,None)
    verify_graph(graph,receivers)

def optional_building_exit_seed(raw,inventory,repository):
    if hashlib.sha256(raw.encode()).hexdigest()!=RAW_SHA256:
        return None
    try:
        verify_native(inventory,repository)
    except (ValueError,KeyError,TypeError,OSError):
        return None
    return CANDIDATE

CANDIDATE = """#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_ALL
#define RA2_COORD_AS_CELL      // TibSun Coord::As_Cell and the member Cell comparisons for Unlimbo 0x0043D3A0
#define RA2_POINT3D_TIBSUN     // TibSun TPoint3D operator-/Length/Distance_To and ObjectClass::Distance/Distance_To
#define RA2_FACING_LEFT90
#define RA2_FACING_DIFFERENCE  // DirType(int) and DirType::operator- for Barrel_Pitch 0x0043B7E0 (TibSun face.h)
#define RA2_BUILDING_RAW_DELAY_COORD
#define RA2_COORD_TPOINT3D
#define RA2_BUILDING_TRANSLATION_VALUES
#define RA2_BUILDING_BARREL_MATRIX_VALUE
#define RA2_MATRIX3D_INLINE_COPY
#define RA2_BUILDING_VECTOR_NEGATE
#define RA2_COORD_POINT_Z
#define RA2_MAP_BOMB_BRIDGE
#define RA2_BUILDING_LOCOMOTOR_ADDREF_DECL
#define RA2_BUILDING_MISSION_NORMAL_DELAY
#define RA2_FACING_DIR256
#define RA2_BUILDING_DELAY_POINT
#define RA2_STAGE_SETTERS
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_POINT_ADD_IN_PLACE
#define RA2_RADIO_CONTACT_WITH_WHOM  // RadioClass::Contact_With_Whom() for Repair_Mission 0x00447260
// Header-static dynamic initializers of the unit at 0x00438500:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00438500-0x004386E6); the other
// drafts of this plan are emitted by these includes.
// Provisional typed Building Exit_Object seed, original00440520.
// Recovered source by Scout761; native adaptation by Scout1069; current canonical APIs by codex4.
// Archive shape checked in TibSun code/building.cpp. No exact-body claim.
#include "shared/cell.hpp"
#include "shared/building.hpp"
#include "shared/gscreen.hpp"
#include "shared/map.hpp"
#include "shared/building_map.hpp"
#include "shared/house.hpp"
#include "shared/foot.hpp"
#include "shared/aircraft.hpp"
#include "shared/fast_math.hpp"
#include "shared/rules.hpp"
#include "shared/id_source.hpp"
#include "shared/building_type_list.hpp"
// @ref ?Pose_Dir_00417890@AircraftClass@@QBEIXZ=00417890
// @ref ?Fast_Atan2@@=004ba600
// @ref ?unknown_a40774@@3HA=00a40774
// @ref _unknown_a40774=00a40774
// @ref ?House_Base_Distance_Score@@YIHPAVHouseClass@@ABVCell@@HH@Z=004f0290
inline UnknownFacingDir Direction_Between(CoordStruct const &from, CoordStruct const &to)
{
    int x2 = to.X;
    int x1 = from.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)from.Y - (double)to.Y, (double)x2 - (double)x1) - 1.5707963267948966) * -10430.060040584269));
}
inline UnknownFacingDir Direction_Between(Cell const &from, Cell const &to)
{
    short x1 = from.X;
    short x2 = to.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)(int)from.Y - (double)(int)to.Y, (double)(int)x2 - (double)(int)x1) - 1.5707963267948966) * -10430.060040584269));
}
int __fastcall House_Base_Distance_Score(HouseClass *house, Cell const &cell, int a, int b);
inline Cell Center_Cell(AbstractClass *object)
{
    return Cell(object->Center_Coord());
}
int BuildingClass::unknown_slot62(TechnoClass *base)
{
    if (!base) return 0;
    base->unknown_32d = true;
    AircraftClass *aircraft = 0;
    if (base->Kind_Of() == 2) aircraft = (AircraftClass *)base;
    Cell cellnum;
    Cell exitcell;
    switch (base->Kind_Of()) {
    case 2:
        House->unknown_func_4f3720(2);
        House->unknown_5548 = -1;
        if (!Unknown_00636ED0(base) && (!House->Unknown_0051FC80() || aircraft->Type->AirportBound)) {
            if (!aircraft->Type->AirportBound) {
Cell edge = Cell(Map.MapLocalSize.X, -Map.MapLocalSize.X) + Cell(1, Map.MapSize.Width) + Cell(Map.MapLocalSize.Y, Map.MapLocalSize.Y);
Cell const &diff = Cell(Center_Coord()) - edge;
if (diff.X - diff.Y > Map.MapLocalSize.Width) {
edge = Cell((short)(edge.X - 1 + Map.MapLocalSize.Width), (short)(edge.Y - Map.MapLocalSize.Width));   // NATIVE: the lane's Cell +/- operators spelled out (ambiguous here)
exitcell = edge;
int random = Random_Pick(0, Map.MapLocalSize.Height);
exitcell.X += random; exitcell.Y += random;
} else {
edge = edge - Cell(1, 0);
exitcell = edge;
int random = Random_Pick(0, Map.MapLocalSize.Height);
exitcell.X += random; exitcell.Y += random;
}
                ++unknown_a40774;
                if (aircraft->unknown_slot53(&CoordStruct(exitcell, 0), 0)) {
                    AbstractClass *target = unknown_1b0;
                    if (target) {
                        aircraft->Assign_Destination(target, 1);
                    } else {
                        Cell cell = aircraft->unknown_6cf130(this);
                        if (cell != unknown_geometry_k) {
                            aircraft->Assign_Destination(&Map[cell], 1);
                        } else {
                            aircraft->Assign_Destination(0, 1);
                        }
                    }
                    aircraft->Set_Mission(2);
                    --unknown_a40774;
                    return 2;
                }
                --unknown_a40774;
                return 0;
            }
        } else {
            aircraft->unknown_func_5d4920(0);
            ++unknown_a40774;
            if (House->Unknown_0051FC80()) {
                Cell nearby = aircraft->unknown_6cf130(this);
                if (aircraft->unknown_slot53(&CoordStruct(nearby, 0), aircraft->Pose_Dir_00417890())) {
                    --unknown_a40774;
                    return 2;
                }
            } else {
                if (aircraft->unknown_slot53(&unknown_slot41((int)aircraft), aircraft->Pose_Dir_00417890())) {
                    unknown_slot150(2, aircraft);
                    unknown_slot150(0x18, aircraft);
                    aircraft->unknown_slot103(unknown_slot41((int)aircraft));
                    aircraft->unknown_5b4 = this;
                    if (unknown_1b0 && !aircraft->Type->AirportBound) {
                        aircraft->Assign_Destination(unknown_1b0, 1);
                        aircraft->unknown_slot116(2, 0);
                    }
                    --unknown_a40774;
                    return 2;
                }
            }
            --unknown_a40774;
            return 0;
        }
        break;
    case 1:
    case 15:
        if (!Type->Hospital && !Type->Armory && !Type->WeaponsFactory && !Unknown_00636EA0()) return 1;
        if (!Type->Hospital && !Type->Armory) {
            House->unknown_func_4f3720(base->Kind_Of());
            if (base->Kind_Of() == 1) House->unknown_5540 = -1;
            if (base->Kind_Of() == 15) House->unknown_5544 = -1;
        }
        if (!Type->Refinery && !Type->Weeder) {
            if (Type->WeaponsFactory) {
                if (Type->Naval) {
                    if (!In_Radio_Contact()) unknown_slot116(0x10, 0);
                    Cell navalcell = Center_Cell(this);
                    if (unknown_1b0) {
                        Cell target = Center_Cell(unknown_1b0);
                        UnknownFacingDir dir = Direction_Between(unknown_slot104(), target);
                        int facing = Dir_To_8(dir);
                        while (Map[navalcell].Cell_Building() == this) {
                            navalcell = Adjacent_Cell(navalcell, facing);
                        }
                    }
                    if (unknown_1b0 && Map[navalcell].unknown_ec == 2 && !Map[navalcell].Cell_Techno(Point2D(0, 0), 0, 0) && Map.Unknown_0055A660(navalcell, true)) {
 } else {
                        navalcell = Map.Nearby_Location(Center_Cell(this), base->unknown_slot32()->SpeedType, -1, 0, 0, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
 }
                    if (!base->unknown_slot53(&Map[navalcell].Center_Coord(), 0x40)) return 0;
                    if (unknown_1b0) {
                        base->Assign_Destination(unknown_1b0, 1);
                        base->unknown_slot116(2, 0);
                    }
                    base->Mark(0);
                    base->unknown_slot103(Map[navalcell].unknown_475d60());
                    base->Mark(1);
                    return 2;
                } else {
                    base->unknown_6d6e50(unknown_1b0);
                    if (Get_Mission() == 0x10) {
                        int count = House->Buildings.Count();
                        for (int index = 0; index < count; index++) {
                            BuildingClass *bldg = House->Buildings[index];
                            if (bldg->Type == Type && bldg != this && bldg->Get_Mission() == 5 && !bldg->unknown_41c) {
                                AbstractClass *temp = unknown_41c;
                                bldg->unknown_41c = temp;
                                unknown_41c = 0;
                                int retval = bldg->unknown_slot62(base);
                                bldg->unknown_41c = 0;
                                unknown_41c = temp;
                                return retval;
                            }
                        }
                        return 1;
                    }
                    ++unknown_a40774;
                    if (base->unknown_slot53(&unknown_slot44(0), 0x40)) {
                        base->Mark(0);
                        base->unknown_slot103(unknown_slot44(0));
                        base->Mark(1);
                        unknown_slot150(2, base);
                        unknown_slot150(0x18, base);
                        unknown_slot116(0x10, 0);
                        --unknown_a40774;
                        return 2;
                    }
                }
            } else if (Type->Factory == 0x10 || Type->Hospital || Type->Armory || Type->Cloning) {
                base->unknown_6d6e50(unknown_1b0);
                exitcell = unknown_slot280(base);
                if (exitcell == unknown_geometry_k) return 0;
                if (Type->Factory == 0x10 && !Type->Cloning) {
                    TechnoTypeClass *type = base->unknown_slot32();
                    for (int index = 0; index < House->unknown_98.Count(); index++) {
                        BuildingClass *vat = House->unknown_98[index];
                        TechnoClass *object = (TechnoClass *)type->unknown_slot35(vat->House);
                        vat->unknown_slot62(object);
                    }
                }
                CoordStruct exitcoord;
                int dir = Dir_To_256(Direction_Between(Center_Coord(), CoordStruct(exitcell)));
                Cell to = exitcell;
                Cell cell = unknown_slot104();
                int x = cell.X;
                if (to.X >= x + Type->Width()) {
                    to.X -= 1;
                } else if (to.X < cell.X) {
                    to.X += 1;
                }
                int y = cell.Y;
                if (to.Y >= y + Type->Foundation_Height(false)) {
                    to.Y -= 1;
                } else if (to.Y < cell.Y) {
                    to.Y += 1;
                }
                exitcoord = CoordStruct(to, 0);
                if (Type->GDIBarracks) {
                    if (exitcell.X == x + 1 && exitcell.Y == y + 2) {
                        exitcoord = exitcoord + Type->ExitCoord;
                    }
                }
                if (Type->NODBarracks && exitcell.X == x + 2 && exitcell.Y == y + 2) {
                    exitcoord = exitcoord + Type->ExitCoord;
                }
                ++unknown_a40774;
                if (base->unknown_slot53(&exitcoord, dir)) {
                    FootClass *foot = (FootClass *)base;
                    if (foot->unknown_494) {
                        if (!base->unknown_slot32()->JumpJet && !base->unknown_slot32()->Teleporter) {
                            if (foot->unknown_494) base->unknown_6d6e50(foot->unknown_494);
                            base->unknown_slot116(2, 0);
                            base->Assign_Destination(&Map[exitcell], 1);
                        }
                    }
                    if (!House->unknown_func_4f52f0() || Type->Hospital) {
                        base->unknown_slot116(0xB, 0);
                        Cell dest = House->Unknown_004EB270(foot);
                        if (dest != unknown_geometry_k && Type->Factory != 0) {
                            base->unknown_6d6e50(&Map[dest]);
                            foot->unknown_4c8bb0(&Map[dest]);
                        } else {
                            base->unknown_6d6e50(0);
                        }
                    }
                    if (unknown_slot150(2, base) == 1) {
                        unknown_slot150(9, base);
                    }
                    --unknown_a40774;
                    return 2;
                }
            } else {
                exitcell = unknown_slot280(base);
 if (exitcell == unknown_geometry_k) return 0;
 int dir = Dir_To_256(Direction_Between(Center_Coord(), CoordStruct(exitcell)));
                Cell to = exitcell;
                Cell cell = unknown_slot104();
                int x = cell.X;
                if (to.X >= x + Type->Width()) {
                    to.X -= 1;
                } else if (to.X < cell.X) {
                    to.X += 1;
                }
                int y = cell.Y;
                if (to.Y >= y + Type->Foundation_Height(false)) {
                    to.Y -= 1;
                } else if (to.Y < cell.Y) {
                    to.Y += 1;
                }
                CoordStruct exitcoord = CoordStruct(to, 0);
                if (Type->GDIBarracks) {
                    if (exitcell.X == x + 1 && exitcell.Y == y + 2) {
                        exitcoord = exitcoord + Type->ExitCoord;
                    }
                }
                if (Type->NODBarracks && exitcell.X == x + 2 && exitcell.Y == y + 2) {
                    exitcoord = exitcoord + Type->ExitCoord;
                }
                ++unknown_a40774;
                if (base->unknown_slot53(&exitcoord, dir)) {
                    base->unknown_slot116(2, 0);
                    base->Assign_Destination(&Map[exitcell], 1);
                    if (!House->unknown_func_4f52f0()) {
                        base->unknown_slot116(0xB, 0);
                        Cell dest = House->Unknown_004EB270((FootClass *)base);
                        if (dest != unknown_geometry_k && Type->Factory != 0) {
                            base->unknown_6d6e50(&Map[dest]);
                        } else {
                            base->unknown_6d6e50(0);
                        }
                    }
                    --unknown_a40774;
                    return 2;
                }
            }
        } else {
            if (base->Kind_Of() == 1) {
                exitcell = Adjacent_Cell(Cell(Center_Coord()), 5);
                ++unknown_a40774;
                if (base->unknown_slot53(&CoordStruct(Adjacent_Cell(exitcell, 4), 0), 0xA0)) {
                    base->unknown_2e0.Set_Current(UnknownFacingDir((unsigned short)0x8000));
                    base->unknown_slot116(10, 0);
                }
            } else {
                base->unknown_slot87(unknown_geometry_l, true, false);
                return 0;
            }
        }
        --unknown_a40774;
        return 0;
    case 6:
        if (!House->unknown_func_4f52f0()) {
            House->unknown_func_4f3720(6);
            House->unknown_553c = -1;
            BuildingClass *building = (BuildingClass *)base;
            BaseNodeClass *node = House->unknown_55f0.Next_Buildable(building->Type->ArrayIndex);
            CoordStruct placementcoord(0, 0, 0);
            BuildingTypeClass *buildingtype = building->Type;
            if (node && node->Cell != Cell(0, 0)) {
                if (buildingtype->PowersUpBuilding[0] != 0 || House->unknown_func_4f5320(buildingtype, node->Cell)) {
                    placementcoord = CoordStruct(node->Cell);
                } else {
                    placementcoord = CoordStruct(House->Where_To_Place_Building(buildingtype, House_Base_Distance_Score, -1));
                    if (placementcoord == unknown_geometry_l) return 0;
                    node->Cell = Cell(placementcoord);
                }
            } else {
                if (buildingtype->PowersUpBuilding[0] == 0) {
                    placementcoord = CoordStruct(House->Where_To_Place_Building(buildingtype, House_Base_Distance_Score, -1));
                } else {
                    Cell upgrade = House->unknown_func_4f0e30(buildingtype);
                    if (upgrade != unknown_geometry_k) {
                        placementcoord = CoordStruct(upgrade);
                    }
                }
                if (node && placementcoord != unknown_geometry_l) {
                    node->Cell = Cell(placementcoord);
                }
            }
            if (placementcoord != unknown_geometry_l) {
                cellnum = placementcoord;
                switch (((BuildingTypeClass *)base->Class_Of())->unknown_458b10(cellnum, House)) {
                default:
                    return 0;
                case 1: {
                    int failures = House->unknown_55f0.unknown_42cb70(node);
                    if (unknown_game_mode_a3d298 && failures > Rules->MaximumBuildingPlacementFailures && node) {
                        House->unknown_55f0.Nodes.Delete(House->unknown_55f0.Nodes.ID(node));
                    }
                    return 1;
                }
                case 0:
                    if (base->unknown_slot53(&placementcoord, 0)) {
                        if (node && building->Type->ArrayIndex == House->unknown_553c) {
                            House->unknown_553c = -1;
                        }
                        if (base->Mission == -1 && base->MissionQueue == 0x12) {
                            base->Commence();
                        }
                        if (base->Kind_Of() == 6) {
                            if (building->Type->FirestormWall) {
                                Map.Unknown_0056A480(Cell(placementcoord), House, building->Type);
                            } else if (building->Type->ToOverlay && building->Type->ToOverlay->Wall) {
                                Map.Unknown_0056A660(Cell(placementcoord), House, building->Type);
                            }
                        }
                        if (building->Type == Rules->unknown_78c) {
                            int index = House->unknown_55f0.Nodes.ID(node) + 1;
                            int count = House->unknown_55f0.Nodes.Count();
                            for (; index < count; index++) {
                                if (House->unknown_55f0.Nodes[index].Type >= 0 && BuildingTypes[House->unknown_55f0.Nodes[index].Type]->IsBaseDefense) {
                                    CoordStruct coord = base->Coord;
                                    House->unknown_55f0.Nodes[index].Cell = Cell(coord);
                                    break;
                                }
                            }
                        }
                        return 2;
                    }
                case 2:
                    if (node) {
                        int index = House->unknown_55f0.Nodes.ID(node);
                        BuildingTypeClass *nodetype = BuildingTypes[node->Type];
                        if (!nodetype->Wall && !nodetype->Gate) {
                            Cell placecell = placementcoord;
                            for (int i = 0; i < House->unknown_55f0.Nodes.Count(); i++) {
                                if (House->unknown_55f0.Nodes[i].Cell == placecell) {
                                    House->unknown_55f0.Nodes[i].Cell = Cell(0, 0);
                                }
                            }
                            return 0;
                        }
                        House->unknown_55f0.Nodes.Delete(index);
                    }
                    break;
                }
            } else {
                if (buildingtype->PowersUpBuilding[0] != 0) {
                    if (House->unknown_55f0.Next_Buildable(-1) == node) {
                        House->unknown_55f0.Nodes.Delete(House->unknown_55f0.Next_Buildable_Index(-1));
                    }
                }
            }
        }
        break;
    }
    return 0;
}
"""
