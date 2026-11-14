// fly.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/fly.json carries the addresses and the rows that
// stay in their own files.


#define RA2_SHIP_MOVE_TO_VALUE
#define RA2_COORD_AS_CELL
#define RA2_COORD_TPOINT3D
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/fly_locomotion.hpp"
#include "shared/foot.hpp"
#include "shared/techno_type.hpp"
#include "shared/cell.hpp"
#include "shared/aircraft.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/team.hpp"
#include "shared/house.hpp"
#include "shared/fast_math.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/sound_play.hpp"
#include "shared/bullet.hpp"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#include "shared/matrix3d.hpp"
#include "shared/house_type.hpp"
#include "shared/special.hpp"
#include "shared/id_source.hpp"
#include "shared/random.hpp"
#include "shared/unknown_globals.hpp"


// Header-static dynamic initializers of the unit at 0x004BBF20:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004BBF20-0x004BC106); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004BBF50, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BBF70, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BBF90, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BBFB0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BBFD0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BBFF0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BC030, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BC060, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BC090, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BC0C0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BC0D0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// Dynamic initializer (0x004BC0F0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004bbf20.cpp; no source of its own.


// FlyLocomotionClass::FlyLocomotionClass() (0x004BC110)

FlyLocomotionClass::FlyLocomotionClass() :
    unknown_18(false),
    unknown_1c(unknown_geometry_l),
    unknown_28(unknown_geometry_l),
    unknown_34(false),
    unknown_38(0),
    unknown_40(0),
    unknown_48(0),
    unknown_50(false),
    unknown_51(false),
    unknown_52(false),
    unknown_53(false),
    unknown_54(0),
    unknown_58(0),
    unknown_5c(false)
{
}


// HRESULT __stdcall FlyLocomotionClass::Link_To_Object(void *object) (0x004BC190, ILocomotion 3)
// Links, then notes whether the object is an airport-bound aircraft.

HRESULT __stdcall FlyLocomotionClass::Link_To_Object(void *object)
{
    HRESULT result = LocomotionClass::Link_To_Object(object);
    if (result == S_OK) {
        if (object && ((FootClass *)object)->Kind_Of() == 2
            && ((UnknownFlyAircraft *)object)->Type->AirportBound) {
            unknown_18 = true;
        } else {
            unknown_18 = false;
        }
    }
    return result;
}


// FlyLocomotionClass::~FlyLocomotionClass() (0x004BC1E0)

FlyLocomotionClass::~FlyLocomotionClass()
{
}


// bool __stdcall FlyLocomotionClass::Is_Moving() (0x004BC200, ILocomotion 4)
// Flying somewhere, or still above the ground (+0x240).

bool __stdcall FlyLocomotionClass::Is_Moving()
{
    return unknown_34 || LinkedTo->unknown_240 > 0.0f;
}


// bool __stdcall FlyLocomotionClass::Is_Moving_Now() (0x004BC230, ILocomotion 32)
// Any speed.

bool __stdcall FlyLocomotionClass::Is_Moving_Now()
{
    if (unknown_48 == 0.0) {
        return false;
    }
    return true;
}


// CoordStruct __stdcall FlyLocomotionClass::Destination() (0x004BC250, ILocomotion 5)

CoordStruct __stdcall FlyLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_1c.X, unknown_1c.Y, unknown_1c.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}


// bool __stdcall FlyLocomotionClass::Process() (0x004BC2B0, ILocomotion 16)
// TibSun Fly.cpp Process: flight level, commence, rotation and movement steps, the
// approach to the destination, landing/take-off and the edge-of-map check; returns Is_Moving().

bool __stdcall FlyLocomotionClass::Process()
{
    if (!unknown_51 && !unknown_50 && unknown_40 >= 1.0 && unknown_38 == 0) {
        unknown_38 = LinkedTo->Techno_Type_Class()->Flight_Level();
    }
    if (LinkedTo->Strength > 0 && !unknown_51 && !unknown_50 && LinkedTo->unknown_slot122()) {
        LinkedTo->Commence();
    }
    Rotation_AI();
    unknown_4bcd50();
    if (!Is_Powered()) {
        return false;
    }
    if (!LinkedTo->unknown_7d) {
        return false;
    }
    if (LinkedTo->Strength > 0 && unknown_1c != unknown_geometry_l && !unknown_51 && !unknown_50 && LinkedTo->Get_Height_AGL() > 0) {
        unknown_4be720(true, CoordStruct(unknown_1c.X, unknown_1c.Y, unknown_1c.Z));
    }
    if (LinkedTo->Strength > 0) {
        unknown_4bca00();
    }
    Edge_Of_World_AI();
    return Is_Moving();
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void FlyLocomotionClass::Move_To(COMCoordStruct to) (0x004BC3E0)
// From ra2-decomp FlyLocomotionClass::Move_To (YR 0x004CCC80).
// Heads for a new destination unless it is the cell already being landed on
// or the unit cannot move (slots 201/202/111/112, unpowered). The null
// coordinate lands an airborne or taking-off unit where it is (else clears
// the destination); any other one is flown to, at flight level above the
// ground while attacking with ammunition, taking off first when needed.
// Twin forms: the COM coordinate taken as a CoordStruct for the cell and null
// tests, the ground height read straight from it, the landing position through
// the object's coordinate accessor (the twin's PositionCoord), and the `locked`
// result of Is_Locked_To_Straight_Flight (inline here, 0x004BFA10).

void __stdcall FlyLocomotionClass::Move_To(COMCoordStruct to)
{
    if (CoordStruct(to).As_Cell() != unknown_1c.As_Cell() || !unknown_51) {

        if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202() && !LinkedTo->unknown_slot111() && !LinkedTo->unknown_slot112() && Is_Powered()) {

            if (CoordStruct(to) == unknown_geometry_l) {
                int landing_altitude = 0;
                UnknownComPtr<IFlyControl,&unknown_uuid_7d80a8> flyctrl(LinkedTo);
                if (flyctrl != 0) {
                    landing_altitude = flyctrl->Landing_Altitude();
                }

                if ((LinkedTo->Get_Height_AGL() > landing_altitude || unknown_50) && !unknown_51) {
                    unknown_1c = LinkedTo->Position_Coord_004f3f40();
                    unknown_4bf1f0();
                } else {
                    unknown_1c = unknown_geometry_l;
                }
                unknown_5c = false;
            } else {
                unknown_1c = to;

                if (LinkedTo->unknown_228 && LinkedTo->unknown_254) {
                    unknown_1c.Z = LinkedTo->Techno_Type_Class()->Flight_Level() + Map.Get_Height_GL(to);
                }

                UnknownComPtr<IFlyControl,&unknown_uuid_7d80a8> flyctrl(LinkedTo);
                int landing_altitude = 0;
                if (flyctrl != 0) {
                    landing_altitude = flyctrl->Landing_Altitude();
                }

                unknown_34 = true;

                if (LinkedTo->Strength > 0 && !unknown_50 && (unknown_51 || LinkedTo->Get_Height_AGL() <= landing_altitude)) {
                    Take_Off_004bf0d0();
                }

                bool locked = Is_Locked_To_Straight_Flight_004bfa10();
                unknown_5c = locked;
                if (LinkedTo->Kind_Of() == 2 && !((AircraftClass *)LinkedTo)->Type->Landable) {
                    unknown_5c = true;
                }
            }
        }
    }
}


CoordStruct Fly_Cell_Coord_004bdfb0(Cell const &cell);
void __stdcall FlyLocomotionClass::Stop_Moving()
{
    if (Is_Moving()) {
        Cell cell(Object_Coord(LinkedTo));
        if (!LinkedTo->unknown_32c && !Map.Unknown_0055A660(cell, true) && !LinkedTo->unknown_slot282()) {
            cell = Map.Unknown_005689D0(cell, 1);
        }
        FootClass *linked = LinkedTo;
        if (linked && linked->Kind_Of() == 2) {
            AircraftClass *aircraft = static_cast<AircraftClass *>(linked);
            if (cell == unknown_geometry_k) {
                LinkedTo->Take_Damage(&LinkedTo->Strength, 0, Rules->unknown_c74, 0, true, 1);
            } else {
                AbstractClass *destination;
                if (LinkedTo->Get_Mission() == 1) destination = aircraft->unknown_4198b0();
                else destination = aircraft->Select_Landing_Destination_004185b0(&Map[cell]);
                LinkedTo->Assign_Destination(destination, true);
                return;
            }
        } else {
            Cell nearby = Map.Nearby_Location(cell, 1, -1, 9, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
            if (nearby != unknown_geometry_k) {
                CoordStruct destination = Fly_Cell_Coord_004bdfb0(nearby);
                destination.Z = Map.Get_Height_GL(destination);
                if ((reinterpret_cast<UnknownLocoCell *>(&Map[nearby])->Flags & 0x100)) destination.Z += unknown_geometry_j;
                LinkedTo->Assign_Destination(&Map[destination], true);
                if (!unknown_51) LinkedTo->Assign_Destination(&Map[destination], true);
                return;
            }
            if (LinkedTo->Strength <= 0) return;
            LinkedTo->Take_Damage(&LinkedTo->Strength, 0, Rules->unknown_c74, 0, true, 1);
        }
        unknown_1c = unknown_geometry_l;
    }
}


bool FlyLocomotionClass::unknown_4bca00()
{
    FootClass *initial=LinkedTo;
    if (initial && initial->Kind_Of()==2 && !static_cast<AircraftClass *>(initial)->Type->Landable) {
        unknown_5c=true;
        unknown_51=false;
        unknown_50=false;
        unknown_38=LinkedTo->Techno_Type_Class()->Flight_Level();
        return false;
    }
    if (LinkedTo->Strength>0 && (unknown_51 || unknown_50)) {
        int layer=LinkedTo->In_Which_Layer();
        LinkedTo->Mark(0);
        Map.Remove(LinkedTo);
        if (unknown_51) Process_Landing_004bdfb0();
        if (unknown_50) Process_Take_Off_004bddf0();
        if (layer!=LinkedTo->In_Which_Layer()) {
            bool ok=true;
            if (LinkedTo->In_Which_Layer()==2 && !unknown_50) {
                if (!LinkedTo->unknown_slot309(&Map[Object_Coord(LinkedTo)])) {
                    unknown_50=true;
                    LinkedTo->unknown_79=false;
                    LinkedTo->Mark(0);
                    LinkedTo->Set_Height_AGL(LinkedTo->Get_Height_AGL()+10);
                    LinkedTo->Mark(1);
                    ok=false;
                }
            }
            if (ok) {
                if (LinkedTo->In_Which_Layer()==2) {
                    LinkedTo->Assign_Destination(0,true);
                    LinkedTo->unknown_slot149(0x18);
                    LinkedTo->unknown_slot263(0,0,0,0);
                    LinkedTo->unknown_slot262(0,0,0,0);
                    Map.Unknown_0054A550(Object_Coord(LinkedTo),LinkedTo->unknown_1f4-3,LinkedTo->unknown_1f4+3,0);
                } else {
                    LinkedTo->unknown_slot149(0x19);
                    if (LinkedTo->HasAnyLink() && LinkedTo->unknown_494 && LinkedTo->unknown_494!=LinkedTo->Contact(0))
                        LinkedTo->unknown_slot149(3);
                }
            }
        }
        Map.Submit(LinkedTo);
        LinkedTo->Mark(1);
    }
    return false;
}


// bool FlyLocomotionClass::Edge_Of_World_AI() (0x004BCC60)
// Landing check: a blocked cell flags the aircraft; a landed carrier (mission 4)
// unloads and frees its passengers, then both finishing slots are called.

bool __fastcall Is_Civilian_Passenger_004139F0(FootClass *passenger);

bool FlyLocomotionClass::Edge_Of_World_AI()
{
    if (!Map.Unknown_0055A7F0(CoordStruct(LinkedTo->Coord))) {
        if (LinkedTo->Get_Mission() == 4) {
            while (LinkedTo->Cargo.Object) {
                FootClass *passenger = (FootClass *)LinkedTo->Cargo.Detach_Object();
                if (Is_Civilian_Passenger_004139F0(passenger)) {
                    passenger->House->unknown_141 = true;
                }
                if (passenger->Team) {
                    passenger->Team->TargetNotAssigned = true;
                }
                delete passenger;
            }
            if (LinkedTo->Team) {
                LinkedTo->Team->TargetNotAssigned = true;
            }
            LinkedTo->unknown_slot210();
            LinkedTo->Delete_Me();
            return true;
        }
    } else {
        LinkedTo->unknown_32d = true;
    }
    return false;
}


extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
extern double const unknown_move_angle_scale_79b620;
// The helipad test's views (TibSun BuildingClass::Class, BuildingTypeClass::IsHelipad).
struct UnknownFlyBuildingType {
    char unknown_000[0x1365];
    bool Helipad;                       // +0x1365
};
struct UnknownFlyBuilding {
    char unknown_000[0x418];
    UnknownFlyBuildingType *Type;       // +0x418
};
inline int Fly_Land_004bcd50(CellClass const &cell)
{
    return cell.unknown_ec;
}
inline int Fly_Distance_004bcd50(CoordStruct const &from,CoordStruct const &to)
{
    CoordStruct difference=from-to;
    double x=difference.X;
    double y=difference.Y;
    return (int)Fast_Sqrt(x*x+y*y);
}
inline UnknownFacingDir Fly_Direction_004bcd50(CoordStruct const &from,CoordStruct const &to)
{
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y)-to.Y,double(to.X)-from.X)
        -unknown_facing_origin_79b630)*unknown_facing_scale_79b628));
}
inline double Fly_As_Radian_004bcd50(UnknownFacingDir const &dir)
{
    return ((short)dir.Raw-0x3fff)*unknown_move_angle_scale_79b620;
}
inline CoordStruct Fly_Move_Coord_004bcd50(CoordStruct const &start,UnknownFacingDir const &dir,int distance)
{
    double radians=Fly_As_Radian_004bcd50(dir);
    return CoordStruct((int)(double(start.X)+Fast_Cos(radians)*double(distance)),
        (int)(double(start.Y)-Fast_Sin(radians)*double(distance)),start.Z);
}
void FlyLocomotionClass::unknown_4bcd50()
{
    Cell here=LinkedTo->unknown_slot104();
    if (here!=LinkedTo->unknown_458 && Is_Moving_Now()) {
        unknown_tracker_839ea0.Update_00413600(LinkedTo,LinkedTo->unknown_458,here);
    }
    if (LinkedTo->Get_Mission()==7 && unknown_5c) {
        unknown_5c=false;
    }
    CoordStruct coord;
    if ((!Is_Powered() || LinkedTo->Strength==0) && LinkedTo->Get_Height_AGL()!=0) {
        if (LinkedTo->Strength==0) {
            unknown_58+=1;
        } else {
            unknown_58+=3;
        }
        coord=Object_Coord(LinkedTo);
        CoordStruct oldcoord=coord;
        coord.Z-=unknown_58;
        Map.Get_Height_GL(coord);
        Map[coord];
        if (Map.In_Radar(Cell(coord))) {
            Map.Remove(LinkedTo);
            LinkedTo->Mark(0);
            LinkedTo->Set_Coord(coord);
            LinkedTo->Mark(1);
            Map.Submit(LinkedTo);
        }
        if (LinkedTo->Get_Height_AGL()<=0) {
            unknown_tracker_839ea0.Unknown_00413310(LinkedTo);
            LinkedTo->Set_Height_AGL(0);
            int damage=LinkedTo->Strength;
            if (damage==0) {
                Cell cell=Cell(coord);
                LinkedTo->unknown_6d7e20(0);
                int sound;
                if (Map[cell].unknown_ec!=2) {
                    sound=LinkedTo->Techno_Type_Class()->ImpactLandSound;
                    if (sound==-1) sound=Rules->ImpactLandSound;
                } else {
                    sound=LinkedTo->Techno_Type_Class()->ImpactWaterSound;
                    if (sound==-1) sound=Rules->ImpactWaterSound;
                }
                if (sound!=-1) {
                    Voc_Play_At(sound,Object_Coord(LinkedTo),0);
                }
                LinkedTo->Delete_Me();
                return;
            }
            LinkedTo->Take_Damage(&LinkedTo->Strength,0,Rules->unknown_c74,0,true,1);
            Cell cell=Cell(coord);
            new AnimClass(Select_Damage_Anim(damage,Rules->unknown_c74,Fly_Land_004bcd50(Map[cell]),&oldcoord),coord,0,1,0x2600,Get_Explosion_Z_0047fcc0(coord));
            Unknown_0047F600(coord,damage,Rules->unknown_c74,0,0);
            Unknown_0047E2C0(coord,damage,0,Rules->unknown_c74,1);
            return;
        }
    } else {
        if (LinkedTo->Strength>0 && LinkedTo->unknown_494!=0 && LinkedTo->Get_Mission()==5 && LinkedTo->MissionQueue==-1 && !unknown_18) {
            LinkedTo->QueueMission(2,false);
        }
    }
    if (!Is_Moving()) {
        return;
    }
    TechnoTypeClass *ttype=LinkedTo->Techno_Type_Class();
    bool is_dropship=ttype->IsDropship;
    bool is_hunter_seeker=ttype->HunterSeeker;
    LinkedTo->Mark(0);
    coord=Object_Coord(LinkedTo);
    Physics_004bdc20(coord,LinkedTo->unknown_2e0.Current());
    if (!Map.In_Radar(Cell(coord)) && LinkedTo->unknown_slot282()) {
        Cell cell=Cell(coord);
        Cell edge=Map.Unknown_00547F30(cell);
        if (edge.X<Map.unknown_128/2) {
            coord.X+=128;
        } else {
            coord.X-=128;
        }
        if (Map.In_Radar(Cell(coord))) {
            LinkedTo->Set_Coord(coord);
        } else {
            coord=Coord_Scatter(coord,64,false);
            if (Map.In_Radar(Cell(coord))) {
                LinkedTo->Set_Coord(coord);
            }
        }
    } else {
        LinkedTo->Set_Coord(coord);
    }
    int bridge_height=0;
    int current_height=LinkedTo->Get_Height_AGL();
    if (!LinkedTo->unknown_79 && current_height>=unknown_geometry_j && (Loco_Cell(Object_Coord(LinkedTo))->Flags&0x100)) {
        current_height-=unknown_geometry_j;
        bridge_height=unknown_geometry_j;
    }
    CoordStruct dest=unknown_1c;
    int dist=Fly_Distance_004bcd50(dest,Object_Coord(LinkedTo));
    if (current_height<unknown_38 && LinkedTo->Strength>0) {
        bool is_loaded=false;
        UnknownComPtr<IFlyControl,&unknown_uuid_7d80a8> flyctrl(LinkedTo);
        if (flyctrl!=0) {
            is_loaded=flyctrl->Is_Loaded()!=0;
        }
        if (is_dropship) {
            int climb=unknown_38-current_height;
            if (climb>16) climb=16;
            LinkedTo->Set_Height_AGL(bridge_height+current_height+climb);
        } else if (is_hunter_seeker) {
            if (unknown_50) {
                int climb=unknown_38-current_height;
                climb=MIN(Rules->HunterSeekerEmergeSpeed,climb);
                LinkedTo->Set_Height_AGL(bridge_height+current_height+climb);
            } else {
                int climb=unknown_38-current_height;
                climb=MIN(Rules->HunterSeekerAscentSpeed,climb);
                LinkedTo->Set_Height_AGL(bridge_height+current_height+climb);
            }
        } else {
            int climb=unknown_38-current_height;
            climb=MIN(is_loaded?10:20,climb);
            LinkedTo->Set_Height_AGL(bridge_height+current_height+climb);
        }
        LinkedTo->unknown_79=false;
    }
    if (current_height>unknown_38 || LinkedTo->Strength==0) {
        int descent=current_height-unknown_38;
        if (is_dropship) {
            if (unknown_51) {
                int limit=descent/20+10;
                descent=MIN(MIN(limit,48),descent);
            } else if (unknown_38==LinkedTo->Techno_Type_Class()->Flight_Level()) {
                if (current_height>16) descent=16;
                else descent=current_height;
            } else {
                if (current_height>6) descent=6;
                else descent=current_height;
            }
        } else if (is_hunter_seeker) {
            descent=MIN(Rules->HunterSeekerDescentSpeed,current_height);
        } else {
            if (descent/20<descent) {
                descent/=20;
            }
            if (descent>=50) {
                descent=50;
            } else if (descent<=20) {
                descent=20;
            }
        }
        current_height-=descent;
        if (LinkedTo->Strength==0 && current_height<=1) {
            current_height=1;
        } else if (current_height<0) {
            current_height=0;
        }
        if (current_height==0 && bridge_height>0) {
            LinkedTo->unknown_79=true;
            bridge_height=0;
        }
        LinkedTo->Set_Height_AGL(bridge_height+current_height);
        if (LinkedTo->Strength>0 && unknown_1c!=unknown_geometry_l) {
            if (LinkedTo->Get_Mission()!=7 || LinkedTo->unknown_494==0
                || LinkedTo->unknown_494->Kind_Of()!=6
                || !((UnknownFlyBuilding *)LinkedTo->unknown_494)->Type->Helipad) {
                Cell target=Cell(unknown_1c);
                CoordStruct center(target.X*256+128,target.Y*256+128,0);
                CoordStruct current=Object_Coord(LinkedTo);
                LinkedTo->Set_Coord(Fly_Move_Coord_004bcd50(current,Fly_Direction_004bcd50(current,center),5));
            }
        }
    }
    if (LinkedTo->Strength>0 && Is_In_Flight() && unknown_1c!=unknown_geometry_l) {
        UnknownComPtr<IFlyControl,&unknown_uuid_7d80a8> flyctrl(LinkedTo);
        if (!unknown_4bf8f0()) {
            unknown_40=1.0;
        } else if (LinkedTo->Techno_Type_Class()->HunterSeeker) {
            if (!unknown_50 && LinkedTo->unknown_228!=0) {
                unknown_40=1.0;
            } else {
                unknown_40=0.0;
            }
        } else {
            double speed=(double)dist/(double)ttype->SlowdownDistance;
            if (1.0<speed) {
                speed=1.0;
            }
            unknown_40=speed;
            if (unknown_40<0.1) {
                if (dist>85) {
                    unknown_40=0.1;
                } else {
                    unknown_40=0.0;
                    unknown_48*=0.5;
                }
            }
            if ((double)dist<unknown_48) {
                unknown_48=(double)dist;
            }
            if (unknown_40==0.0 && unknown_48==0.0 && dist>0) {
                unknown_48=0.05;
            }
        }
    }
    if (is_dropship && LinkedTo->Strength>0 && unknown_48>0.0 && !unknown_50) {
        int slowdown=ttype->SlowdownDistance;
        if (dist<slowdown) {
            double factor=1.0-((double)dist-((double)slowdown*0.6))/((double)slowdown*0.4);
            LinkedTo->unknown_240=MIN(LinkedTo->Techno_Type_Class()->PitchAngle,factor*LinkedTo->Techno_Type_Class()->PitchAngle);
        }
    }
    if (!unknown_5c && LinkedTo->Strength>0 && !unknown_51 && !unknown_50) {
        if (Cell(unknown_1c)==LinkedTo->unknown_slot104() && unknown_40==0.0 && (LinkedTo->unknown_228==0 || LinkedTo->unknown_254==0)) {
            unknown_4bf1f0();
        }
    }
    if (LinkedTo->Strength>0) {
        if (unknown_48<unknown_40) {
            unknown_48=MIN(unknown_48+0.1,unknown_40);
        } else if (unknown_48>unknown_40) {
            unknown_48=MAX(unknown_48-0.1,unknown_40);
        }
    }
    LinkedTo->Mark(1);
}


extern double const unknown_move_angle_scale_79b620;
int FlyLocomotionClass::Physics_004bdc20(CoordStruct &coord,UnknownFacingDir facing)
{
    int actual=Apparent_Speed();
    if (actual>0) {
        CoordStruct next=Fly_Move_Coord_004bcd50(coord,facing,actual);
        coord=next;
        if (!Map.In_Radar(coord.As_Cell())) return 2;
        return 1;
    }
    return 0;
}


extern "C" int __cdecl abs(int);
inline UnknownFacingDir Fly_Add_Rotation_004bdd10(UnknownFacingDir const &direction, int rotation)
{
    return UnknownFacingDir((unsigned short)(direction.Raw+((unsigned int)rotation<<8)));
}
void FlyLocomotionClass::Rotation_AI()
{
    if (unknown_53 && LinkedTo->Strength>0) {
        if (!Is_Powered()) {
            UnknownFacingDir current=LinkedTo->unknown_2e0.Current();
            UnknownFacingDir direction=Fly_Add_Rotation_004bdd10(current,unknown_54);
            LinkedTo->unknown_2e0.Set(direction);
            int step=abs(unknown_54)<1?abs(unknown_54):1;
            if (unknown_54<0) unknown_54+=step;
            else unknown_54-=step;
        } else {
            unknown_53=false;
            Map[Object_Coord(LinkedTo)].Trigger_Veins();
        }
    }
}


extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
inline UnknownFacingDir Fly_Direction_004bddf0(CoordStruct const &from,CoordStruct const &to)
{
    int from_x=from.X;
    int to_x=to.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y)-to.Y,double(to_x)-from_x)
        -unknown_facing_origin_79b630)*unknown_facing_scale_79b628));
}
bool FlyLocomotionClass::Process_Take_Off_004bddf0()
{
    int height=LinkedTo->Get_Height_AGL();
    if (!LinkedTo->unknown_79 && (Loco_Cell(Object_Coord(LinkedTo))->Flags & 0x100) && height>=unknown_geometry_j)
        height-=unknown_geometry_j;
    UnknownFlyPtr flyctrl(LinkedTo);
    int landing_altitude=0;
    if (flyctrl) landing_altitude=flyctrl->unknown_slot3();
    int relative_height=height-landing_altitude;
    int relative_level=unknown_38-landing_altitude;
    unknown_50=false;
    unknown_51=false;
    if (relative_height>relative_level-relative_level/3) {
        LinkedTo->unknown_2f8.Set_Desired(LinkedTo->unknown_2e0.Desired());
    } else if (relative_height>relative_level/2) {
        UnknownFacingDir dir=Fly_Direction_004bddf0(LinkedTo->Center_Coord(),unknown_1c);
        LinkedTo->unknown_2e0.Set_Desired(dir);
        unknown_40=1;
    }
    return true;
}


extern float const unknown_fly_pitch_zero_79a738;
extern double const unknown_fly_pitch_floor_79b610;
extern double const unknown_fly_pitch_rate_7a0ff8;
inline CoordStruct Fly_Cell_Coord_004bdfb0(Cell const &cell)
{
    CoordStruct coord(cell.X*256+128,cell.Y*256+128,0);
    return coord;
}
// Cell step through the real TPoint2D<short> sum (TibSun inline.h Adjacent_Cell).
inline Cell Fly_Adjacent_Cell_004bdfb0(Cell const &cell,int facing)
{
    return cell+AdjacentCell[facing&7];
}
bool FlyLocomotionClass::Process_Landing_004bdfb0()
{
    if (!unknown_51) return true;
    bool has_landed=false;
    int height=LinkedTo->Get_Height_AGL();
    if ((Loco_Cell(Object_Coord(LinkedTo))->Flags & 0x100) && height>=unknown_geometry_j)
        height-=unknown_geometry_j;
    if (LinkedTo->Techno_Type_Class()->IsDropship) {
      if (height==0 && LinkedTo->unknown_240>unknown_fly_pitch_zero_79a738) {
        double pitch=LinkedTo->unknown_240-unknown_fly_pitch_rate_7a0ff8;
        if (unknown_fly_pitch_floor_79b610>pitch) pitch=unknown_fly_pitch_floor_79b610;
        LinkedTo->unknown_240=(float)pitch;
      }
      unknown_40=0;
    } else {
      unknown_40=0;
    }
    int landing_altitude=0;
    UnknownFlyPtr flyctrl(LinkedTo);
    if (flyctrl) landing_altitude=flyctrl->unknown_slot3();
    CellClass *destination=&Map[unknown_1c];
    if ((unknown_18 && !LinkedTo->ContainsLink((TechnoClass *)destination->Cell_Building())) ||
        (!unknown_18 && LinkedTo->Can_Enter_Cell(destination,-1,-1,0,true)!=0)) {
        Take_Off_004bf0d0();
        Cell const &pos=Cell(Object_Coord(LinkedTo));
        Cell nearby=Map.Nearby_Location(pos,1,-1,9,false,Point2D(1,1),false,false,false,true,Cell(0,0),0);
        if (nearby!=unknown_geometry_k) {
            CoordStruct next=Fly_Cell_Coord_004bdfb0(nearby);
            next.Z=Map.Get_Height_GL(next);
            if ((reinterpret_cast<UnknownLocoCell *>(&Map[nearby])->Flags&0x100)) next.Z+=unknown_geometry_j;
            Move_To(next);
        } else {
            LinkedTo->Take_Damage(&LinkedTo->Strength,0,Rules->unknown_c74,0,true,1);
            unknown_1c=unknown_geometry_l;
            return false;
        }
    }
    if (!unknown_52 && height<300) {
        CoordStruct coord=Object_Coord(LinkedTo);
        coord.Z=Map.Get_Height_GL(coord);
        unknown_52=true;
        if (LinkedTo->Techno_Type_Class()->IsDropship) {
            new AnimClass(AnimTypes[AnimTypeClass::From_Name("DROPLAND")],coord,0,1,0x600,0);
        } else if (LinkedTo->Kind_Of()==2 && static_cast<AircraftClass *>(LinkedTo)->Type->Carryall) {
            new AnimClass(AnimTypes[AnimTypeClass::From_Name("CARYLAND")],coord,0,1,0x600,0);
        }
        if (LinkedTo->Strength>0) Voc_Play_At(LinkedTo->Techno_Type_Class()->AuxSound2,coord,0);
    }
    if (height<=landing_altitude && LinkedTo->unknown_240<=unknown_fly_pitch_zero_79a738) {
        CoordStruct coord=Object_Coord(LinkedTo);
        if ((Loco_Cell(coord)->Flags&0x100) && coord.Z>=Map.Get_Height_GL(coord)+unknown_geometry_j)
            LinkedTo->unknown_79=true;
        LinkedTo->Set_Height_AGL(landing_altitude);
        unknown_51=false;
        unknown_50=false;
        has_landed=true;
        unknown_tracker_839ea0.Unknown_00413310(LinkedTo);
        LinkedTo->unknown_slot306(0);
        unknown_48=0;
        unknown_40=0;
        if (LinkedTo->unknown_454!=Cell(0,0)) {
            for (int facing=0;facing<8;facing++) {
                Cell adjacent=Fly_Adjacent_Cell_004bdfb0(LinkedTo->unknown_454,facing);
                CellClass *cell=&Map[adjacent];
                cell->unknown_122--;
            }
            LinkedTo->unknown_454=LinkedTo->unknown_slot104();
            for (facing=0;facing<8;facing++) {
                Cell adjacent=Fly_Adjacent_Cell_004bdfb0(LinkedTo->unknown_454,facing);
                CellClass *cell=&Map[adjacent];
                cell->unknown_122++;
            }
        } else {
            LinkedTo->unknown_454=LinkedTo->unknown_slot104();
            for (int facing=0;facing<8;facing++) {
                Cell adjacent=Fly_Adjacent_Cell_004bdfb0(LinkedTo->unknown_454,facing);
                CellClass *cell=&Map[adjacent];
                cell->unknown_122++;
            }
        }
        if (Cell(LinkedTo->Center_Coord())==Cell(unknown_1c) || Map[Cell(unknown_1c)].Cell_Building()==LinkedTo->Contact(0)) {
            unknown_34=false;
            Move_To(CoordStruct(unknown_geometry_l.X,unknown_geometry_l.Y,unknown_geometry_l.Z));
            LinkedTo->Assign_Destination(0,true);
        }
    }
    return has_landed;
}


extern double const unknown_fly_land_speed_7a10f8;
inline UnknownFacingDir Fly_Direction_XY_004be720(CoordStruct const &from,CoordStruct const &to)
{
    UnknownFacingDir dir;
    if (from.Equal_XY_004bfb60(to)) dir.Raw=0;
    dir=UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y)-to.Y,double(to.X)-from.X)
        -unknown_facing_origin_79b630)*unknown_facing_scale_79b628));
    return dir;
}
inline UnknownFacingDir Fly_Facing_Diff_004be720(UnknownFacingDir const &a,UnknownFacingDir const &b)
{
    return UnknownFacingDir((unsigned short)(a.Raw-b.Raw));
}
inline bool Fly_Is_Complete_Turn(UnknownFacingDir const &self,UnknownFacingDir const &towards,UnknownFacingDir const &by)
{
    UnknownFacingDir diff(Fly_Facing_Diff_004be720(self,towards));
    if (by.Magnitude_At_Least_004bfb30(diff)) {
        return true;
    }
    return false;
}
inline int Fly_Distance_XY_004be720(CoordStruct const &from,CoordStruct const &to)
{
    return (Point2D(from.X,from.Y)-Point2D(to.X,to.Y)).Length();
}
int FlyLocomotionClass::unknown_4be720(bool stage,CoordStruct coord)
{
    LinkedTo->unknown_slot104();
    UnknownComPtr<IFlyControl,&unknown_uuid_7d80a8> flyctrl(LinkedTo);
    if (flyctrl!=0 && LinkedTo->unknown_228!=0 && LinkedTo->unknown_254!=0 && flyctrl->Is_Strafe()) {
        stage=false;
    }
    CoordStruct aim=coord;
    ObjectClass *building=Map[Cell(coord)].Cell_Building();
    if (building!=0 && !LinkedTo->Techno_Type_Class()->HunterSeeker) {
        if (!LinkedTo->ContainsLink((TechnoClass *)building)) {
            FootClass *self=LinkedTo;
            if (self!=0 && self->Kind_Of()==2) {
                AbstractClass *dock=((AircraftClass *)self)->Select_Landing_Destination_004185b0(building);
                if (dock==0) {
                    aim=unknown_geometry_l;
                } else {
                    aim=dock->Center_Coord();
                }
            }
        } else {
            aim=building->unknown_slot41((int)LinkedTo);
        }
    }
    LinkedTo->Get_Height();
    int dist=Fly_Distance_XY_004be720(LinkedTo->Center_Coord(),aim);
    if (flyctrl==0 || !flyctrl->Is_Locked()) {
        UnknownFacingDir dir=UnknownFacingDir(LinkedTo->Center_Coord(),CoordStruct(coord));
        bool turn=true;
        if (flyctrl!=0 && flyctrl->Is_Strafe()) {
            if (Fly_Distance_XY_004be720(LinkedTo->Center_Coord(),coord)<=0x300) {
                if (!Fly_Is_Complete_Turn(dir,LinkedTo->unknown_2e0.Current(),UnknownFacingDir((unsigned short)0x2000))) {
                    turn=false;
                }
            }
        }
        if (turn) {
            LinkedTo->unknown_2e0.Set_Desired(Fly_Direction_XY_004be720(LinkedTo->Center_Coord(),CoordStruct(coord)));
        }
    }
    if (dist<0x100 && stage && !LinkedTo->Techno_Type_Class()->HunterSeeker) {
        if (LinkedTo->unknown_228!=0 && LinkedTo->unknown_254!=0 && (flyctrl==0 || !flyctrl->Is_Strafe())) {
            if (flyctrl==0 || !flyctrl->Is_Locked()) {
                LinkedTo->unknown_2f8.Set_Desired(LinkedTo->Direction_To(LinkedTo->unknown_228));
            }
        } else {
            UnknownFacingDir land((unsigned short)0);
            if (flyctrl!=0) {
                land=UnknownFacingDir((unsigned short)(flyctrl->Landing_Direction()<<13));
            }
            if (flyctrl==0 || !flyctrl->Is_Locked()) {
                LinkedTo->unknown_2f8.Set_Desired(land);
            }
        }
    } else {
        if (flyctrl==0 || !flyctrl->Is_Locked()) {
            LinkedTo->unknown_2f8.Set_Desired(Fly_Direction_004bddf0(LinkedTo->Center_Coord(),coord));
        }
    }
    if (unknown_5c && dist<0x300) {
        unknown_38=unknown_1c.Z-Map.Get_Height_GL(unknown_1c);
    } else if (LinkedTo->Techno_Type_Class()->IsDropship) {
        if (dist>=LinkedTo->Techno_Type_Class()->SlowdownDistance) {
            unknown_38=LinkedTo->Techno_Type_Class()->Flight_Level();
        } else {
            float ratio=(float)(dist/LinkedTo->Techno_Type_Class()->SlowdownDistance);
            unknown_38=(int)(LinkedTo->Techno_Type_Class()->Flight_Level()*ratio+(1.0f-ratio)*(LinkedTo->Techno_Type_Class()->Flight_Level()/3));
        }
    } else {
        unknown_38=LinkedTo->Techno_Type_Class()->Flight_Level();
    }
    if (!LinkedTo->Techno_Type_Class()->HunterSeeker && unknown_4bf8f0()) {
        if (dist<0x80) {
            if (stage) {
                unknown_40=0.0;
            }
            if (!unknown_5c && unknown_48<unknown_fly_land_speed_7a10f8) {
                unknown_4bf1f0();
            }
        } else if (dist<0x200) {
            if (stage) {
                unknown_40=0.5;
            }
        } else if (dist<0x300 && stage) {
            unknown_40=0.75;
        }
    }
    if (LinkedTo->unknown_77 && !LinkedTo->House->Is_Player_Control()) {
        if (Map.Is_Shrouded(Object_Coord(LinkedTo)) || ((*(unsigned int *)unknown_id_source_a3d290&0x1000) && Map.Is_Fogged(Object_Coord(LinkedTo)))) {
            LinkedTo->Unselect();
        }
    }
    return dist;
}


// Matrix3D * __stdcall FlyLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key) (0x004BED90, ILocomotion 9)
// Turned by the second facing (+0x2F8; the key gains it as bits 3..7). In
// the air, a crashing aircraft (+0x37D) rolls and pitches by its own angles
// (+0x280/+0x284; key -1); one faster than its type's PitchSpeed pitches by
// PitchAngle (key bit 2) and rolls by RollAngle while turning (bits 0 and 1).

Matrix3D * __stdcall FlyLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key)
{
    Matrix3D transform;
    transform.Make_Identity();
    if (key != 0) {
        *key |= Dir_To_32(LinkedTo->unknown_2f8.Current()) << 3;
    }
    transform.Rotate_Z((float)((Dir_To_32(LinkedTo->unknown_2f8.Current()) - 8) * -0.19634954084936207));
    if (LinkedTo->Get_Height_AGL() > 0) {
        if (LinkedTo->unknown_37d) {
            transform.Rotate_X(LinkedTo->unknown_280);
            if (unknown_48 > LinkedTo->Techno_Type_Class()->PitchSpeed) {
                transform.Rotate_Y((float)(LinkedTo->unknown_284 + LinkedTo->Techno_Type_Class()->PitchAngle));
            } else {
                transform.Rotate_Y(LinkedTo->unknown_284);
            }
            if (key != 0) {
                *key = -1;
            }
        } else if (unknown_48 > LinkedTo->Techno_Type_Class()->PitchSpeed) {
            if (LinkedTo->Techno_Type_Class()->PitchAngle != 0.0) {
                if (key != 0) {
                    *key |= 4;
                }
                transform.Rotate_Y((float)LinkedTo->Techno_Type_Class()->PitchAngle);
            }
            if (LinkedTo->Techno_Type_Class()->RollAngle != 0.0) {
                if (LinkedTo->unknown_2f8.Is_Rotating_CW()) {
                    if (key != 0) {
                        *key |= 1;
                    }
                    transform.Rotate_X((float)LinkedTo->Techno_Type_Class()->RollAngle);
                } else if (LinkedTo->unknown_2f8.Is_Rotating_CCW()) {
                    if (key != 0) {
                        *key |= 2;
                    }
                    transform.Rotate_X((float)-LinkedTo->Techno_Type_Class()->RollAngle);
                }
            }
        }
    }
    *matrix = transform;
    return matrix;
}


// Point2D __stdcall FlyLocomotionClass::Draw_Point() (0x004BEFB0, ILocomotion 11)
// The screen offset of the drawn aircraft: (0, bob), where the bob (0..2) follows the
// frame counter when the aircraft is not a dropship, neither +0x50 nor +0x51 is set
// and the height the 0x007D80A8 interface reports is below slot 108.

Point2D __stdcall FlyLocomotionClass::Draw_Point()
{
    int bob = 0;
    UnknownFlyPtr source(LinkedTo);
    int height = 0;
    if (source) {
        height = source->unknown_slot3();
    }
    if (!LinkedTo->Techno_Type_Class()->IsDropship && !unknown_51 && !unknown_50 && LinkedTo->Get_Height_AGL() > height) {
        bob = (int)(Fast_Sin((Frame % 20) * 0.3141592653589793) * 1.5 + 0.5);
    }
    return Point2D(0, bob);
}


// Point2D __stdcall FlyLocomotionClass::Shadow_Point() (0x004BF0C0, ILocomotion 12)

Point2D __stdcall FlyLocomotionClass::Shadow_Point()
{
    return Point2D(0, 0);
}


void FlyLocomotionClass::Take_Off_004bf0d0()
{
    if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202() && !LinkedTo->unknown_slot111() && !LinkedTo->unknown_slot112()) {
        unknown_51=false;
        unknown_50=true;
        if (LinkedTo->unknown_slot178()==unknown_geometry_k)
            unknown_tracker_839ea0.Unknown_004131E0(LinkedTo);
        unknown_38=LinkedTo->Techno_Type_Class()->Flight_Level();
        if (LinkedTo->Get_Height_AGL()==0) {
            LinkedTo->unknown_2e0.Set(LinkedTo->unknown_2f8.Desired());
        }
        Voc_Play_At(LinkedTo->Techno_Type_Class()->AuxSound1,Object_Coord(LinkedTo),0);
    }
}


// void FlyLocomotionClass::unknown_4bf1f0() (0x004BF1F0)
// Unless the linked techno is on mission 7 or passes its check 0x006D1470,
// nothing happens. An airport-bound aircraft that is out of radio contact
// with the building in its cell calls its slot 261 (false, true); otherwise
// the flight state resets (+0x50/+0x52 cleared, +0x51 set, +0x38 zero).

void FlyLocomotionClass::unknown_4bf1f0()
{
    if (LinkedTo->Get_Mission() == 7 || !LinkedTo->unknown_6d1470()) {
        if (LinkedTo->Kind_Of() == 2) {
            AircraftClass *aircraft = (AircraftClass *)LinkedTo;
            if (aircraft->Type->AirportBound) {
                CellClass *cell = (CellClass *)aircraft->Get_Cell_Ptr();
                if (!LinkedTo->ContainsLink((TechnoClass *)cell->Cell_Building())) {
                    aircraft->unknown_slot261(false, true);
                    return;
                }
            }
        }
        unknown_50 = false;
        unknown_51 = true;
        unknown_52 = false;
        unknown_38 = 0;
    }
}


// Matrix3D * __stdcall FlyLocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key) (0x004BF280, ILocomotion 10)
// As LocomotionClass::Shadow_Matrix, turned by the second facing (+0x2F8);
// a dropship's shadow ignores the cell's ramp.

Matrix3D * __stdcall FlyLocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key)
{
    CoordStruct coord = LinkedTo->Coord;
    int ramp = Map[coord].unknown_11c;
    if (LinkedTo->Techno_Type_Class()->IsDropship) {
        ramp = 0;
    }
    Matrix3D transform;
    Matrix3D ramp_matrix;
    transform = *Unknown_00718D60(&ramp_matrix, ramp);
    transform.Rotate_Z((float)((Dir_To_32(LinkedTo->unknown_2f8.Current()) - 8) * -0.19634954084936207));
    if (key != 0 && *key != -1) {
        *key = ((*key << 6) + ramp) << 5;
        *key |= Dir_To_32(LinkedTo->unknown_2f8.Current());
    }
    *matrix = transform;
    return matrix;
}


// void __stdcall FlyLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x004BF390, ILocomotion 19)
// Sets the linked object's turret facing at once.

void __stdcall FlyLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2f8.Set(UnknownFacingDir(dir));
}


// bool FlyLocomotionClass::Is_In_Flight() (0x004BF3C0)
// Not +0x51, and either not +0x50 or at least half the +0x38 height up
// (slot 108). Expanded where it is used; this out-of-line copy is never
// called.

bool FlyLocomotionClass::Is_In_Flight()
{
    if (!unknown_51 && (!unknown_50 || LinkedTo->Get_Height_AGL() >= unknown_38 / 2)) {
        return true;
    }
    return false;
}


// HRESULT __stdcall FlyLocomotionClass::GetClassID(GUID *clsid) (0x004BF400)

HRESULT __stdcall FlyLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_FlyLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall FlyLocomotionClass::Load(IStream *stream) (0x004BF440)
// LocomotionClass::Load, then rebuilt in place.

HRESULT __stdcall FlyLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) FlyLocomotionClass(NoInitClass());
    }
    return result;
}


// int __stdcall FlyLocomotionClass::Layer() (0x004BF470, ILocomotion 29)
// Above ground (slot 108 positive): layer 4, otherwise 2.

int __stdcall FlyLocomotionClass::Layer()
{
    return LinkedTo->Get_Height_AGL() <= 0 ? 2 : 4;
}


// bool __stdcall FlyLocomotionClass::Power_Off() (0x004BF4A0, ILocomotion 23)
// A moving aircraft starts drifting (a random 20..30, either way) and stops.

bool __stdcall FlyLocomotionClass::Power_Off()
{
    if (Is_Moving()) {
        Start_Drift();
        Stop_Moving();
        unknown_58 = 0;
    }
    return LocomotionClass::Power_Off();
}


// bool __stdcall FlyLocomotionClass::Is_Powered() (0x004BF510, ILocomotion 24)

bool __stdcall FlyLocomotionClass::Is_Powered()
{
    return LocomotionClass::Is_Powered();
}


// bool __stdcall FlyLocomotionClass::Is_Ion_Sensitive() (0x004BF520, ILocomotion 25)
// Everything but hunter-seekers.

bool __stdcall FlyLocomotionClass::Is_Ion_Sensitive()
{
    return !LinkedTo->Techno_Type_Class()->HunterSeeker;
}


// void FlyLocomotionClass::Start_Drift() (0x004BF550)
// A drift of 20..30, either way at random. Power_Off expands it; this
// out-of-line copy is never called.

void FlyLocomotionClass::Start_Drift()
{
    unknown_53 = true;
    unknown_54 = unknown_id_source_a3d290->Random()(20, 30);
    if (unknown_id_source_a3d290->Random()(0, 99) < 50) {
        unknown_54 = -unknown_54;
    }
}


// int __stdcall FlyLocomotionClass::Apparent_Speed() (0x004BF5A0, ILocomotion 33)
// The type's speed scaled by the current speed fraction.

int __stdcall FlyLocomotionClass::Apparent_Speed()
{
    return LinkedTo->Techno_Type_Class()->Speed * unknown_48;
}


// int __stdcall FlyLocomotionClass::Get_Status() (0x004BF5D0, ILocomotion 36)
// 1 or 0 for the flags at +0x51 and +0x50; otherwise 2 moving, 3 not.

int __stdcall FlyLocomotionClass::Get_Status()
{
    if (unknown_51) {
        return 1;
    }
    if (unknown_50) {
        return 0;
    }
    return Is_Moving() ? 2 : 3;
}


void __stdcall FlyLocomotionClass::Acquire_Hunter_Seeker_Target()
{
    if (LinkedTo->unknown_228 == 0) {
        DynamicVectorClass<ObjectClass *> aitargets;
        DynamicVectorClass<ObjectClass *> humantargets;

        bool atenemy = unknown_game_mode_a3d298 != 0 && LinkedTo->House->unknown_54f0 != -1 && !LinkedTo->House->Is_Human_Player();

        for (int index = Technos.Count() - 1; index >= 0; index--) {
            TechnoClass *techno = Technos[index];

            bool enemy = true;
            if (!atenemy) {
                if (LinkedTo->House->Is_Ally(techno->House)) {
                    enemy = false;
                }
            } else {
                if (techno->House != Houses[LinkedTo->House->unknown_54f0]) {
                    enemy = false;
                }
            }

            if (enemy && techno->Strength > 0 && !techno->unknown_76 && techno->unknown_7d &&
                !techno->Techno_Type_Class()->Invisible && techno->Techno_Type_Class()->unknown_201 &&
                !(unknown_id_source_a3d290->Special().HarvesterImmune && techno->Kind_Of() == 1 && ((UnitClass *)techno)->Type->Harvester)) {

                if (unknown_game_mode_a3d298 != 0 && LinkedTo->House->Is_Human_Player() && !techno->House->Type->MultiplayPassive &&
                    (techno->Kind_Of() == 1 || techno->Kind_Of() == 6 || techno->Kind_Of() == 15 || techno->Kind_Of() == 2)) {
                    humantargets.Add(techno);
                } else {
                    aitargets.Add(techno);
                }
            }
        }

        if (humantargets.Count() > 0) {
            LinkedTo->unknown_slot220(humantargets[unknown_id_source_a3d290->Random()(0, humantargets.Count() - 1)]);
        } else if (aitargets.Count() > 0) {
            LinkedTo->unknown_slot220(aitargets[unknown_id_source_a3d290->Random()(0, aitargets.Count() - 1)]);
        }
    }
}


// bool FlyLocomotionClass::unknown_4bf8f0() (0x004BF8F0; TibSun Needs_To_Land)
bool FlyLocomotionClass::unknown_4bf8f0()
{
    FootClass *object = LinkedTo;
    if (object && object->Kind_Of() == 2 && ((IFlyControl *)(AircraftClass *)object)->Is_Locked()) {
        return false;
    }
    if (unknown_51) {
        return true;
    }
    if (!unknown_5c) {
        return true;
    }
    UnknownHeldComPtr<IFlyControl, &unknown_uuid_7d80a8> flyctrl(LinkedTo);
    if (flyctrl != 0 && !flyctrl->Is_Strafe() && !flyctrl.Checked()->Is_Fighter()) {
        return true;
    }
    if (LinkedTo->unknown_254 == 0) {
        return true;
    }
    return false;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool FlyLocomotionClass::Is_Locked_To_Straight_Flight() (0x004BFA10), from ra2-decomp (YR 0x004D02B0):
// the comip pointer tested with != 0 (comip operator!=(int)).
bool FlyLocomotionClass::Is_Locked_To_Straight_Flight_004bfa10()
{
    if (unknown_1c.Z > Map.Get_Height_GL(unknown_1c) + 120)
        return true;
    if (LinkedTo->unknown_228 != 0 && LinkedTo->unknown_254 != 0)
        return true;
    UnknownComPtr<IFlyControl, &unknown_uuid_7d80a8> flyctrl(LinkedTo);
    if (flyctrl != 0) {
        if (flyctrl->Is_Locked())
            return true;
    }
    return false;
}


// int FlyLocomotionClass::Size() (0x004BFAF0): inline in shared/fly_locomotion.hpp, emitted with the vtable; no source of its own.


// void *FlyLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x004BFB00)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


// UnknownComPtr<IFlyControl>::operator==(int) 0x004BFB90: out-of-line copy of the inline member (shared/fly_control_ptr.hpp)
// emitted by this unit where VC6 refuses to expand it; no source of its own.


// UnknownComPtr<IFlyControl>::Release_Held 0x004BFBC0: out-of-line copy of the inline member (shared/fly_control_ptr.hpp)
// emitted by this unit where VC6 refuses to expand it; no source of its own.


extern "C" unsigned char table_4bfbd0[]; extern "C" void* __cdecl storage_4bfbd0(unsigned int);
class Storage_4bfbd0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4bfbd0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4bfbd0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4bfbd0(requested << 2); owns=1; } return this; }


// UnknownComPtr<IFlyControl>::Query_From 0x004BFC20: out-of-line copy of the inline member (shared/fly_control_ptr.hpp)
// emitted by this unit where VC6 refuses to expand it; no source of its own.


// LocomotionClass::QueryInterface adjustor thunk for ILocomotion (0x004BFCA0)
// Compiler generated (this adjusted by -4, then the LocomotionClass member);
// FlyLocomotionClass does not override it, and the linker kept this unit's copy
// for every locomotor vtable that uses it. No source of its own.


// LocomotionClass::AddRef adjustor thunk for ILocomotion (0x004BFCB0)
// Compiler generated (this adjusted by -4, then the LocomotionClass member);
// FlyLocomotionClass does not override it, and the linker kept this unit's copy
// for every locomotor vtable that uses it. No source of its own.


// LocomotionClass::Release adjustor thunk for ILocomotion (0x004BFCC0)
// Compiler generated (this adjusted by -4, then the LocomotionClass member);
// FlyLocomotionClass does not override it, and the linker kept this unit's copy
// for every locomotor vtable that uses it. No source of its own.
