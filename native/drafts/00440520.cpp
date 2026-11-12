// NON-EXACT DRAFT 0x440520 FUN_00440520
// entry 0x440520; exact-match score 92.27 (misses_index best), this text 91.93; plan "draft, verify"
// origin lane: Scout761 (build/class-work/Scout761/keep/eo_F_9193.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): slots 71/260 under their tree names (Mark, Assign_Destination); one Cell sum spelled out; each changed line is marked NATIVE.
// BuildingClass::Exit_Object (TibSun name): puts a produced object into the world, complete original body 00440520.
#include "shared/building.hpp"
#include "shared/gscreen.hpp"
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
// @ref ?Base_Cell_Weight_By_Distance_004f0290@@YIHPAVHouseClass@@ABVCell@@HH@Z=004f0290
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
int __fastcall Base_Cell_Weight_By_Distance_004f0290(HouseClass *house, Cell const &cell, int a, int b);
inline Cell Center_Cell(AbstractClass *object)
{
    return Cell(object->Center_Coord());
}
int BuildingClass::Exit_Object(TechnoClass *base)
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
                if (aircraft->Unlimbo(&CoordStruct(exitcell, 0), 0)) {
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
                if (aircraft->Unlimbo(&CoordStruct(nearby, 0), aircraft->Pose_Dir_00417890())) {
                    --unknown_a40774;
                    return 2;
                }
            } else {
                if (aircraft->Unlimbo(&unknown_slot41((intptr_t)aircraft), aircraft->Pose_Dir_00417890())) {
                    unknown_slot150(2, aircraft);
                    unknown_slot150(0x18, aircraft);
                    aircraft->Set_Coord(unknown_slot41((intptr_t)aircraft));
                    aircraft->unknown_5b4 = this;
                    if (unknown_1b0 && !aircraft->Type->AirportBound) {
                        aircraft->Assign_Destination(unknown_1b0, 1);
                        aircraft->QueueMission(2, 0);
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
                    if (!HasAnyLink()) QueueMission(0x10, 0);
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
                        navalcell = Map.Nearby_Location(Center_Cell(this), base->Techno_Type_Class()->SpeedType, -1, 0, 0, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
 }
                    if (!base->Unlimbo(&Map[navalcell].Center_Coord(), 0x40)) return 0;
                    if (unknown_1b0) {
                        base->Assign_Destination(unknown_1b0, 1);
                        base->QueueMission(2, 0);
                    }
                    base->Mark(0);
                    base->Set_Coord(Map[navalcell].unknown_475d60());
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
                                int retval = bldg->Exit_Object(base);
                                bldg->unknown_41c = 0;
                                unknown_41c = temp;
                                return retval;
                            }
                        }
                        return 1;
                    }
                    ++unknown_a40774;
                    if (base->Unlimbo(&unknown_slot44(0), 0x40)) {
                        base->Mark(0);
                        base->Set_Coord(unknown_slot44(0));
                        base->Mark(1);
                        unknown_slot150(2, base);
                        unknown_slot150(0x18, base);
                        QueueMission(0x10, 0);
                        --unknown_a40774;
                        return 2;
                    }
                }
            } else if (Type->Factory == 0x10 || Type->Hospital || Type->Armory || Type->Cloning) {
                base->unknown_6d6e50(unknown_1b0);
                exitcell = unknown_slot280(base);
                if (exitcell == unknown_geometry_k) return 0;
                if (Type->Factory == 0x10 && !Type->Cloning) {
                    TechnoTypeClass *type = base->Techno_Type_Class();
                    for (int index = 0; index < House->unknown_98.Count(); index++) {
                        BuildingClass *vat = House->unknown_98[index];
                        TechnoClass *object = (TechnoClass *)type->Create_One_Of(vat->House);
                        vat->Exit_Object(object);
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
                if (base->Unlimbo(&exitcoord, dir)) {
                    FootClass *foot = (FootClass *)base;
                    if (foot->unknown_494) {
                        if (!base->Techno_Type_Class()->JumpJet && !base->Techno_Type_Class()->Teleporter) {
                            if (foot->unknown_494) base->unknown_6d6e50(foot->unknown_494);
                            base->QueueMission(2, 0);
                            base->Assign_Destination(&Map[exitcell], 1);
                        }
                    }
                    if (!House->Is_Human_Player() || Type->Hospital) {
                        base->QueueMission(0xB, 0);
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
                if (base->Unlimbo(&exitcoord, dir)) {
                    base->QueueMission(2, 0);
                    base->Assign_Destination(&Map[exitcell], 1);
                    if (!House->Is_Human_Player()) {
                        base->QueueMission(0xB, 0);
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
                if (base->Unlimbo(&CoordStruct(Adjacent_Cell(exitcell, 4), 0), 0xA0)) {
                    base->unknown_2e0.Set(UnknownFacingDir((unsigned short)0x8000));
                    base->QueueMission(10, 0);
                }
            } else {
                base->Scatter(unknown_geometry_l, true, false);
                return 0;
            }
        }
        --unknown_a40774;
        return 0;
    case 6:
        if (!House->Is_Human_Player()) {
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
                    placementcoord = CoordStruct(House->Where_To_Place_Building(buildingtype, Base_Cell_Weight_By_Distance_004f0290, -1));
                    if (placementcoord == unknown_geometry_l) return 0;
                    node->Cell = Cell(placementcoord);
                }
            } else {
                if (buildingtype->PowersUpBuilding[0] == 0) {
                    placementcoord = CoordStruct(House->Where_To_Place_Building(buildingtype, Base_Cell_Weight_By_Distance_004f0290, -1));
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
                    if (base->Unlimbo(&placementcoord, 0)) {
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
