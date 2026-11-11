// NON-EXACT DRAFT 0x500AA0 InfantryClass::unknown_slot93 (Per_Cell_Process) (4617 B)
// entry 0x500AA0; exact-match score 93.45 (lane best), this text 93.45; plan "draft, verify"
// origin lane: Scout612 (build/class-work/Scout612/w/pd3.cpp)
// NATIVE: unknown_slot260/unknown_slot71 respelled Assign_Destination/Mark (current names).
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
// Infantry arrival: garrison, capture, transport, sabotage, and cell effects.
#include "shared/infantry.hpp"
#include "shared/tag.hpp"
#include "shared/map_view.hpp"
#include "shared/techno_map.hpp"
#include "shared/building_type_class.hpp"
#include "shared/sidebar_building.hpp"
#include "shared/house_type.hpp"
#include "shared/rules.hpp"
#include "shared/session_state.hpp"
#include "shared/random2.hpp"
#include "shared/sound_play.hpp"
#include "shared/vox_class.hpp"
#include "shared/radar_event.hpp"
#include "shared/tileset_index.hpp"
#include "shared/unknown_geometry.hpp"
void Native_Building_Infiltrate_00452390(BuildingClass *building, HouseClass *house);   // NATIVE: link/behaviour_m10.cpp
bool Native_Building_Is_Going_To_Blow_00500AA0(BuildingClass *building);               // NATIVE: link/behaviour_m10_building.cpp
void Native_Building_Set_Going_To_Blow_00500AA0(BuildingClass *building);
void Native_Building_Arm_C4_00500AA0(BuildingClass *building, int countdown, AbstractClass *who);
// @ref ?Unknown_00452390@BuildingClass@@=00452390
// @ref ?Unknown_00452EB0@BuildingClass@@=00452eb0
// @ref ?Unknown_004536D0@BuildingClass@@=004536d0
// @ref ?Is_Ally@HouseClass@@QBE_NPBVTechnoClass@@@Z=004e5580
// @ref ?Spring@TagClass@@=006b3df0
// @ref ?Is_To_Inherit@TagClass@@=006b41f0
// @ref ?Unknown_006D9610@TechnoClass@@=006d9610
// @ref ?unknown_slot93@FootClass@@=004c7220

// @ref ??_C@_0BD@HGEC@EVA_BridgeRepaired?$AA@=007daef0
// @ref ?Unknown_00552570@MapClass@@QAEXABVCell@@@Z=00552570
// @ref ?Unknown_00555A60@MapClass@@QAEXABVCell@@@Z=00555a60

inline CoordStruct Adjacent_Cell(CoordStruct coord, int facing)
{
    TPoint2D<int> const &offset=unknown_facing_offsets_851ac0[facing & 7];
    return CoordStruct(coord.X+offset.X,coord.Y+offset.Y,coord.Z);
}

void InfantryClass::unknown_slot93(int why)
{
    UnknownCellClass *cell=Map_Cell(CoordStruct(Coord));
    if (why==2) {
        if (Get_Mission()==8 && (Type->Assaulter || Type->Occupier) && unknown_494 && unknown_494->Kind_Of()==6) {
            BuildingClass *building=static_cast<BuildingClass *>(unknown_494);
            if (building==cell->Unknown_004717B0()) {
                if (!building->Unknown_00452EB0(this)) {
                    Assign_Destination(0,1);
                    unknown_slot87(building->Center_Coord(),true,true);
                    return;
                }
                if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                if (Type->Occupier) {
                    unknown_slot52();
                    building->unknown_554.Add(this);
                    building->Unknown_006D9610(building->unknown_slot105());
                    return;
                } else if (Type->Assaulter) {
                    building->Unknown_004536D0(this);
                    Assign_Destination(0,1);
                    unknown_slot87(building->Center_Coord(),true,true);
                    return;
                }
                return;
            }
        }
        if (Get_Mission()==9) {
            BuildingClass *building=cell->Unknown_004717B0();
            if (building && (building==unknown_494 || building==unknown_228)) {
                if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                if (unknown_slot32()->VoiceDie.Count()>0 && House->Is_Player_Control()) {
                    CoordStruct location=Coord;
                    Voc_Play_At(unknown_slot32()->VoiceDie.Pick(unknown_random_839218()),location,0);
                }
                if (unknown_slot32()->DieSound.Count()>0) {
                    CoordStruct location=Coord;
                    Voc_Play_At(unknown_slot32()->DieSound.Pick(unknown_random_839218()),location,0);
                }
                building->House->Refund_Money(unknown_slot167());
                if (unknown_34) unknown_34->Spring(48,this,unknown_geometry_k,false,0);
                unknown_slot60();
                return;
            }
        }
        if (Get_Mission()==8 || Get_Mission()==11 || Get_Mission()==25) {
            AbstractClass *navigation=unknown_494;
            bool is_techno;
            if (navigation) is_techno=navigation->AbstractFlags.IsTechno;
            if (navigation && is_techno && static_cast<ObjectClass *>(navigation)->unknown_slot31()) {
                TechnoClass *destination=static_cast<TechnoClass *>(unknown_494);
                if (destination && destination->Kind_Of()==6) {
                    if (unknown_slot104()==destination->unknown_slot104()) {
                        if (destination->unknown_34) destination->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                        destination->unknown_slot54(false);
                        destination->unknown_slot222(House,1);
                        destination->unknown_290=Type->ArrayIndex;
                        destination->unknown_slot173();
                        if (unknown_34 && unknown_34->Is_To_Inherit()) destination->Attach_Tag(unknown_34);
                        if (unknown_34) unknown_34->Spring(48,this,unknown_geometry_k,false,0);
                        unknown_slot60();
                        return;
                    }
                    UnknownCoord434820 locomotor_destination(Locomotor.Checked()->Destination());
                    if (locomotor_destination==unknown_geometry_l || unknown_slot104()==Coord_Cell(locomotor_destination)) {
                        Assign_Destination(unknown_494,1);
                        return;
                    }
                }
            }
            BuildingClass *building=cell->Unknown_004717B0();
            if (building && (building==unknown_494 || building==unknown_228)) {
                if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                if (Type->Engineer) {
                    if (building->Kind_Of()==6 && building->Type->BridgeRepairHut) {
                        if (House->Is_Player_Control() && Unknown_0063B5B0(14,unknown_slot104())) Speech_Play_Name("EVA_BridgeRepaired",-1,-1);
                        bool train=false;
                        for (int y=-2;y<3;y++) {
                            for (int x=-2;x<3;x++) {
                                Cell spot=Cell(unknown_slot104()+Cell(x,y));
                                int tile=Map_Cell_At(spot)->unknown_38;
                                Cell overlay_spot=Cell(Cell(x,y)+unknown_slot104());
                                int overlay=Map_Cell_At(overlay_spot)->unknown_44;
                                if ((tile>=unknown_tileset_a6cb68 && tile<unknown_tileset_a6cb68+16) || (overlay>=74 && overlay<=101)) train=true;
                            }
                        }
                        if (train) Map.Unknown_00552570(unknown_slot104());
                        else Map.Unknown_00555A60(unknown_slot104());
                        for (int index=Infantry.Count()-1;index>=0;--index) Infantry[index]->Detach(building,false);
                        building->unknown_slot173();
                    } else if (House->Is_Ally(building) || (building->House->Type->MultiplayPassive && building->Type->CanBeOccupied)) {
                        if (building->Strength==building->unknown_slot32()->MaxStrength) {
                            Assign_Destination(0,1);
                            unknown_slot87(building->Center_Coord(),true,true);
                            return;
                        }
                        building->unknown_slot233();
                    } else {
                        bool capturable=false;
                        if (As_Building(building)) capturable=building->Type->Capturable;
                        if (unknown_game_mode_a3d298!=0 && unknown_session_a3d298.unknown_34 && building->Health_Ratio()>Rules->unknown_1370) {
                            int max_damage=building->Strength-int(building->unknown_slot32()->MaxStrength*Rules->unknown_1370*0.5);
                            double cap=max_damage;
                            int damage=int((building->unknown_slot32()->MaxStrength*((1.0-Rules->unknown_1370*0.5)*0.5) < cap) ? building->unknown_slot32()->MaxStrength*((1.0-Rules->unknown_1370*0.5)*0.5) : cap);
                            building->unknown_slot86(&damage,0,Rules->unknown_c74,this,true,0);
                        } else if (capturable) {
                            if (building->Mission==19) {
                                Assign_Destination(0,1);
                                unknown_slot87(building->Center_Coord(),true,true);
                                return;
                            }
                            if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                            building->House->unknown_18b=true;
                            if (unknown_34 && unknown_34->Is_To_Inherit()) building->Attach_Tag(unknown_34);
                            building->unknown_slot222(House,1);
                            building->unknown_290=Type->ArrayIndex;
                            building->unknown_slot173();
                        }
                    }
                } else if (Type->Agent) Native_Building_Infiltrate_00452390(building, House);   // NATIVE: map_techno.hpp's BuildingClass view lacks the member (link/behaviour_m10.cpp)
                if (unknown_34) unknown_34->Spring(48,this,unknown_geometry_k,false,0);
                unknown_slot60();
                return;
            }
            if (!unknown_494) {
                if (Mission!=11) unknown_slot261(false,true);
                if (Map_Cell(CoordStruct(Coord))->Unknown_004717B0()) unknown_slot87(unknown_geometry_l,true,false);
            }
        }
        TechnoClass *transport;
        if (Type->VehicleThief) {
            transport=static_cast<UnknownCellClass *>(static_cast<void *>(unknown_slot105()))->Unknown_00473E30(unknown_79);
            if (!transport) transport=static_cast<UnknownCellClass *>(static_cast<void *>(unknown_slot105()))->Unknown_00473E80(unknown_79);
            if (!transport) transport=static_cast<UnknownCellClass *>(static_cast<void *>(unknown_slot105()))->Unknown_004717B0();
        } else {
            transport=static_cast<UnknownCellClass *>(static_cast<void *>(unknown_slot105()))->Unknown_004717B0();
            if (!transport) transport=static_cast<UnknownCellClass *>(static_cast<void *>(unknown_slot105()))->Unknown_00473E80(unknown_79);
            if (!transport) transport=static_cast<UnknownCellClass *>(static_cast<void *>(unknown_slot105()))->Unknown_00473E30(unknown_79);
        }
        TechnoClass *at_destination=0;
        if (unknown_494 && unknown_494->Kind_Of()==11) {
            UnknownCellClass *destination_cell=Map_Cell(unknown_494->Center_Coord());
            if (Type->VehicleThief) {
                at_destination=destination_cell->Unknown_00473E30(true);
                if (!at_destination) at_destination=destination_cell->Unknown_00473E80(true);
                if (!at_destination) at_destination=destination_cell->Unknown_004717B0();
            } else {
                at_destination=destination_cell->Unknown_004717B0();
                if (!at_destination) at_destination=destination_cell->Unknown_00473E80(true);
                if (!at_destination) at_destination=destination_cell->Unknown_00473E30(true);
            }
        }
        if (Get_Mission()==7 && transport && (unknown_494==transport || unknown_228==transport || (at_destination && transport==at_destination))) {
            if (transport->unknown_34) transport->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
            if (transport->Kind_Of()==6) {
                if (transport==static_cast<UnknownCellClass *>(static_cast<void *>(unknown_slot105()))->Unknown_004717B0() && unknown_slot149(21)==1) {
                    unknown_slot52();
                    unknown_slot150(2,transport);
                    transport->Cargo.Attach(this);
                    unknown_4cd9b0();
                    return;
                }
            } else if (unknown_slot104()==transport->unknown_slot104() && transport==unknown_494) {
                if (unknown_slot150(15,transport)==1) {
                    unknown_6d6e50(0);
                    unknown_79=false;
                    unknown_slot52();
                    static_cast<FootClass *>(transport)->Unknown_004CCE80(this);
                    unknown_slot69();
                    return;
                }
                Assign_Destination(0,1);
                unknown_slot116(5,false);
                unknown_slot87(unknown_geometry_l,true,true);
                return;
            }
        }
        if (Get_Mission()==17 && Type->C4) {
            BuildingClass *building=cell->Unknown_004717B0();
            if (building && building==unknown_494) {
                if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                // NATIVE (Scout1231): the building's C4 members (+0x5AF going to blow, +0x420 countdown,
                // +0x438 who planted it) through link/behaviour_m10_building.cpp: this unit's BuildingClass
                // view has no names for them, and the VC6 offsets land elsewhere in the arm64 layout
                // (+0x5AF is the top byte of the animation pointer unknown_454[7]: user crash 2026-11-10).
                if (building->Get_Mission()!=19 && !building->unknown_slot203()) {
                    if (Native_Building_Is_Going_To_Blow_00500AA0(building)) {
                        Assign_Destination(0,1);
                        unknown_244=unknown_slot187(1);
                        unknown_slot87(building->Center_Coord(),true,true);
                        return;
                    }
                    Native_Building_Set_Going_To_Blow_00500AA0(building);
                    double warning_delay=Rules->unknown_13b8*900.0;
                    warning_delay*=0.5;
                    building->unknown_slot79(int(warning_delay));
                    Native_Building_Arm_C4_00500AA0(building,int(Rules->unknown_13b8*900.0),this);
                }
                unknown_4cd9b0();
                unknown_slot252(false);
                unknown_244=unknown_slot187(1);
                unknown_slot87(building->Center_Coord(),true,true);
                return;
            }
            if (reinterpret_cast<AbstractClass *>(Map_Cell(Center_Coord()))==unknown_494) {
                Unknown_0047E2C0(CoordStruct(Coord),Rules->unknown_13a8,this,Rules->unknown_c74,true);
                unknown_slot291();
                unknown_slot87(Adjacent_Cell(CoordStruct(Coord),Dir_To_8(unknown_2e0.Current())),true,true);
                unknown_slot116(2,false);
                if (!unknown_494 || Map_Cell(unknown_494->Center_Coord())->unknown_ec==2) Mark(1);
                Unknown_0047E2C0(CoordStruct(Coord),Rules->unknown_13a8,static_cast<TechnoClass *>(0),Rules->unknown_c74,true);
                Unknown_0047E2C0(CoordStruct(Coord),Rules->unknown_13a8,static_cast<TechnoClass *>(0),Rules->unknown_c74,true);
                if (!unknown_7d) return;
                Mark(1);
            }
        }
        if (unknown_370) {
            unknown_slot149(8);
            if (Type->VoiceComment.Count()>1) Voc_Play_At(Type->VoiceComment[1],CoordStruct(Coord),0);
            if (unknown_79) {
                if ((cell->unknown_128.Composite & 0x1c)==0x1c) cell->Unknown_00476920(&unknown_geometry_l,true,true,true);
            } else if ((cell->unknown_124.Composite & 0x1c)==0x1c) cell->Unknown_00476920(&unknown_geometry_l,true,true,false);
        }
        if (MissionQueue==-1 && !unknown_494 && !unknown_228 && !In_Radio_Contact()) unknown_slot261(false,true);
        Commence();
        unknown_slot263(false,false,false,0);
        unknown_slot262(false,false,false,0);
        Map.Unknown_0054A550(CoordStruct(Coord),0,unknown_1f4+3,0);
        UnknownCellClass *current=Map_Cell(CoordStruct(Coord));
        int land=current->unknown_ec;
        if (!Locomotor.Checked()->Is_Moving() && !Type->C4 && !unknown_6d7910(14) && (land==3 || land==2) && (!unknown_79 || !(current->Flags & 0x100))) {
            int damage=Strength;
            unknown_slot86(&damage,0,Rules->unknown_c74,0,true,0);
            return;
        }
    }
    if (unknown_7d) FootClass::unknown_slot93(why);
}
