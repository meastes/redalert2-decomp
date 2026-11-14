// aircraft.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/aircraft.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/aircraft.hpp"
#include "shared/infantry.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/matrix3d.hpp"
#include "shared/house.hpp"
#include "shared/team.hpp"
#include "shared/id_source.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/random.hpp"
#include "shared/team_type.hpp"
#include "shared/bullet.hpp"
#include "shared/bullet_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/aircraft_vector_xy.hpp"
#include "shared/fast_math.hpp"
#include "shared/facing.hpp"
#include "shared/particle_system.hpp"
#include "shared/crt_memory.hpp"
#include "shared/waypoint_path.hpp"
#include "shared/debug_printf.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/object_lists.hpp"
#include "shared/ccini_class.hpp"
#include "shared/object.hpp"
#include "shared/com_ptr.hpp"


// Header-static dynamic initializers of the unit at 0x00413800:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00413800-0x004139E6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00413830, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x00413850, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x00413870, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x00413890, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x004138B0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x004138D0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x00413910, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x00413940, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x00413970, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x004139A0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x004139B0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Dynamic initializer (0x004139D0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00413800.cpp; no source of its own.


// Predicate in Aircraft.cpp (004139F0): a civilian infantry passenger with +5C1 clear.
bool __fastcall Is_Civilian_Passenger_004139F0(FootClass *passenger)
{
    if (!passenger) return false;
    InfantryClass *infantry = As_Infantry(passenger);
    if (!infantry) return false;
    if (!infantry->Type->Civilian) return false;
    if (infantry->unknown_5c1) return false;
    return true;
}


// AircraftClass::AircraftClass(AircraftTypeClass *type, HouseClass *house) (0x00413A40)
// Takes an ID, creates the type's locomotor and links it to the aircraft,
// initializes from the type, then joins the aircraft list and the techno
// index.

AircraftClass::AircraftClass(AircraftTypeClass *type, HouseClass *house) :
    FootClass(house),
    Type(type),
    unknown_5b0(false),
    unknown_5b1(false),
    unknown_5b2(false),
    unknown_5b4(0),
    unknown_5b8(false),
    unknown_5b9(false),
    unknown_5ba(false),
    unknown_5bb(5),
    unknown_5bc(true),
    unknown_5bd(true)
{
    Create_ID();
    if (Type != 0) {
        Locomotor.CreateInstance(Type->Locomotor, 0, CLSCTX_ALL);
        Locomotor.Checked()->Link_To_Object(this);
    }
    Init();
    Aircraft.Add(this);
    unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
}


// void AircraftClass::Init() (0x00413CA0, slot 9)
// TechnoClass::Init, then from the type: flag +0x118 when the house type
// lists it, both facings' rate, the second facing set to the first, the
// flight level, ammunition and strength; finally tells the house.

void AircraftClass::Init()
{
    TechnoClass::Init();
    if (Type != 0) {
        if (House != 0) {
            DynamicVectorClass<TechnoTypeClass *> &types = (DynamicVectorClass<TechnoTypeClass *> &)House->Type->VeteranAircraft;
            for (int index = 0; index < types.Count(); index++) {
                if (types[index] == Type) {
                    unknown_118.Set_Veteran(true);
                    break;
                }
            }
        }
        unknown_2e0.Set_ROT(Type->ROT);
        unknown_2f8.Set_ROT(Type->ROT);
        unknown_2f8.Set(unknown_2e0.Current());
        Set_Height_AGL(Type->Flight_Level());
        unknown_254 = Type->InitialAmmo != -1 ? Type->InitialAmmo : Type->Ammo;
        unknown_70 = Strength = Type->MaxStrength;
    }
    if (House != 0) {
        House->unknown_4ea970(this);
    }
}


// AircraftClass::~AircraftClass() (0x00413DA0)
// While the game runs (0x00A40958) and the type is set: tells the house
// (flag +0x144 when it has none of the type left), leaves the team and the
// house, then forgets the type. Always leaves the aircraft list and the
// techno index.

AircraftClass::~AircraftClass()
{
    if (unknown_flag_a40958 && Type != 0) {
        if (House->Can_Build(Type, 0, 0) == -1) {
            House->unknown_144 = true;
        }
        unknown_6d3040(0);
        if (Team != 0) {
            Team->Unknown_006B8FB0(this, -1, 0);
            Team = 0;
        }
        House->unknown_4ea800(this);
        FootClass::Limbo();
        Type = 0;
    }
    Unknown_006ECB90(this, true);
    Aircraft.Delete(this);
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    unknown_7d = false;
}


// HRESULT __stdcall AircraftClass::QueryInterface(GUID const &iid, void **object) (0x00413FA0, slot 0)
// AbstractClass's interfaces, then IFlyControl (the +0x5A8 part).

HRESULT __stdcall AircraftClass::QueryInterface(GUID const &iid, void **object)
{
    HRESULT result = FootClass::QueryInterface(iid, object);
    if (FAILED(result)) {
        if (iid == IID_IFlyControl) {
            *object = static_cast<IFlyControl *>(this);
        }
        result = S_OK;
        AddRef();
    }
    return result;
}


// unsigned long __stdcall AircraftClass::AddRef() (0x00414000, slot 1)

unsigned long __stdcall AircraftClass::AddRef()
{
    return FootClass::AddRef();
}


// unsigned long __stdcall AircraftClass::Release() (0x00414010, slot 2)

unsigned long __stdcall AircraftClass::Release()
{
    return FootClass::Release();
}


// Aircraft placement (00414020,slot53); preserve ordered type/map/weapon reads.
bool AircraftClass::Unlimbo(CoordStruct const *coord, int dir)
{
    CoordStruct placed = *coord;
    if (!Type->MissileSpawn) {
        if (!unknown_32c && Map.Unknown_0055A7F0(*coord)) {
            placed.Z = Map.Get_Height_GL(*coord);
        } else {
            AircraftTypeClass *type = Type;
            placed.Z = Map.Get_Height_GL(*coord) + type->Flight_Level();
        }
    }
    if (FootClass::Unlimbo(&placed, dir)) {
        if (!(Type->unknown_200 && Type->Landable) ||
            (unknown_slot231(0)->WeaponType && unknown_slot231(0)->WeaponType->unknown_11f))
            unknown_32c = true;
        if (Cargo.Object) unknown_5b1 = true;
        unknown_2f8.Set(UnknownFacingDir((unsigned short)((unsigned char)dir << 8)));
        StageClass::Set_Rate(1);
        StageClass::Stage = 0;
        int altitude = Type->Flight_Level();
        if (Get_Height_AGL() == altitude) unknown_slot306(1.0);
        else unknown_slot306(0.0);
        return true;
    }
    return false;
}


// Aircraft drawing: original 0x004141C0, 1256 bytes plus 8 alignment NOPs.
// COM plain-value returns and explicit coordinate conversion follow the original ABI.

Matrix3D __fastcall Aircraft_Isometric_View_Matrix_00717f90();

void AircraftClass::unknown_slot67(Point2D const &point, Rect const &window)
{
    if (!Debug_Map && unknown_b263d8 &&
        (unknown_id_source_a3d290->unknown_000 & 0x1000)) {
        CoordStruct destination = (CoordStruct)Locomotor.Checked()->Head_To_Coord();
        destination.Z = Position_Coord_004f3f40().Z;
        if (Map.Is_Fogged(destination) &&
            Map.Is_Fogged(Position_Coord_004f3f40()) && !House->Is_Player_Control()) return;
    }

    Point2D draw_point = point;
    draw_point += Locomotor.Checked()->Draw_Point();
    if (Cargo.Object && Type->Carryall) {
        Cargo.Attached_Object()->unknown_slot67(draw_point, window);
    }
    unknown_slot104();
    if (Type->unknown_206 && Type->unknown_b0.unknown_00) {
        CoordStruct ground = Position_Coord_004f3f40();
        ground.Z = Map.Get_Height_GL(ground);
        Point2D shadow_point;
        unknown_tactical_83996c->Coord_To_Pixel(ground, shadow_point);
        shadow_point += Locomotor.Checked()->Shadow_Point();

        int key = 0;
        int saved_height = Get_Height_AGL();
        bool saved_flag = unknown_slot47();
        unknown_5a0 = false;
        UnknownCellClass *cell = reinterpret_cast<UnknownCellClass *>(&Map[Position_Coord_004f3f40()]);
        bool draw_on_ground = false;
        if ((unknown_79 || saved_height < unknown_geometry_j) &&
            (!unknown_79 || saved_height < 0)) {
            draw_on_ground = true;
        } else {
            if ((cell->Flags & 0x100) &&
                (((cell->Flags & 0x800) && (cell->Unknown_00476AC0(0)->Flags & 0x100)) ||
                 (!(cell->Flags & 0x800) && (cell->Unknown_00476AC0(6)->Flags & 0x100)))) {
                Set_Height_AGL(unknown_geometry_j);
                shadow_point.Y -= Unknown_006A2970(unknown_geometry_j);
            } else {
                draw_on_ground = true;
            }
        }
        if (draw_on_ground) Set_Height_AGL(0);

        Matrix3D matrix;
        matrix = *Locomotor.Checked()->Shadow_Matrix(&COMMatrix3DStruct(), &key);
        if (!unknown_325) {
            unknown_4c9a40(Type->unknown_b0, 0,
                key, &Type->unknown_23c,
                window, shadow_point,
                Aircraft_Isometric_View_Matrix_00717f90() * matrix, true, 0, 0, 0);
        }
        Set_Height_AGL(saved_height);
        unknown_5a0 = saved_flag;
        unknown_tactical_83996c->Add_To_Selectables(this, draw_point);

        int scenario_scale;
        if (Unknown_0051FC50()) scenario_scale = unknown_id_source_a3d290->IonLevel;
        else scenario_scale = unknown_id_source_a3d290->AmbientLevel;
        int level = Get_Height_AGL() / (unknown_geometry_g + unknown_geometry_g);
        int altitude_term = level * scenario_scale;
        UnknownCellClass *ground_cell = reinterpret_cast<UnknownCellClass *>(&Map[ground]);
        int shadow_height = ground_cell->unknown_10a + Rules->ExtraAircraftLight;
        shadow_height += altitude_term;
        key = 0;
        matrix = *Locomotor.Checked()->Draw_Matrix(&COMMatrix3DStruct(), &key);
        unknown_slot246(Type->unknown_b0, 0,
            key, &Type->unknown_214,
            window, draw_point,
            Aircraft_Isometric_View_Matrix_00717f90() * matrix, shadow_height, 0);
    }
    // Original base draw receives the ORIGINAL first argument, not draw_point.
    FootClass::unknown_slot67(point, window);
}


// 0x004146b0: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_004146b0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004146b0 {

void __stdcall FUN_004146b0(int unused_0, int unused_1)
{
    return;
}

}  // namespace seed_004146b0


// int AircraftClass::Do_MISSION_HUNT() (0x004146C0, slot 132)
// Out of ammunition: leave the team, slot 261 (0, 1) and wait the mission's
// delay (+0x10 minutes of 900 frames) plus 0-2 frames. Otherwise look for
// a target near the aircraft (with 0x40 first in some game modes); without
// one slot 261 (0, 1), with one queue mission 1. Returns the delay.

int AircraftClass::Do_MISSION_HUNT()
{
    if (unknown_254 == 0) {
        if (Team != 0) {
            Team->Unknown_006B8FB0(this, -1, 0);
        }
        unknown_slot261(0, 1);
        int delay = (int)(Control()->unknown_10 * 900.0);
        return delay + unknown_id_source_a3d290->Random()(0, 2);
    }
    if (unknown_228 == 0) {
        if (unknown_game_mode_a3d298 != 0) {
            CoordStruct coord = Coord;
            unknown_slot220(unknown_slot219(0x40, &coord, 0));
        }
        if (unknown_228 == 0) {
            CoordStruct coord = Coord;
            unknown_slot220(unknown_slot219(0, &coord, 0));
        }
        if (unknown_228 == 0) {
            unknown_slot261(0, 1);
            return 1;
        }
    }
    QueueMission(1, false);
    return 1;
}


// AircraftClass AI 0x004147F0. Unknown fields retain observed offsets; behavior names remain hypotheses.
extern CellClass unknown_dummy_cell_a6f9f8;
void AircraftClass::AI()
{
    switch (Mission) {
    case 1:
    case 27:
        break;
    default:
        unknown_5ba = false;
    }
    if (unknown_20c) unknown_20c->AI();
    if ((unknown_slot111() || unknown_slot112()) && Frame % 24 == 0 && Rules->unknown_2a8)
        new AnimClass(Rules->unknown_2a8, CoordStruct(Coord), 0, 1, 0x600, 0);
    if (unknown_slot112() || (unknown_slot111() && unknown_210)) {
        Locomotor.Checked()->Process();
        if (!unknown_7d) return;
    }
    if ((unknown_slot112() && unknown_20c) || unknown_slot111()) {
        if (unknown_228) unknown_slot220(0);
        if (unknown_494) Assign_Destination(0, true);
        return;
    }
    if (unknown_228) {
        if (((ObjectClass *)unknown_228)->In_Air()) unknown_slot220(0);
        else if (unknown_228) {
            unsigned char const &target_is_techno = unknown_228->AbstractFlags.IsTechno;
            if (target_is_techno && !House->Is_Ally((ObjectClass *)unknown_228) &&
                ((TechnoClass *)unknown_228)->unknown_1b8 == 2 &&
                !((TechnoClass *)unknown_228)->unknown_slot192(House)) unknown_slot220(0);
        }
    }
    FootClass::AI();
    if (!unknown_7d) return;
    if (unknown_325) {
        CoordStruct current = Coord;
        Set_Coord(current - CoordStruct(0, 0, 5));
        if (Get_Height_AGL() < -400) {
            Record_The_Kill(0);
            Delete_Me();
            return;
        }
        if ((Frame & 3) == 0) {
            int offset_y = unknown_id_source_a3d290->Random()(-170, 170);
            int offset_x = unknown_id_source_a3d290->Random()(-170, 170);
            CoordStruct offset(offset_x, offset_y, -Get_Height_AGL());
            current = Coord;
            current = current + offset;
            new AnimClass(Rules->unknown_09c, current, 0, 1, 0x600, 0);
        }
    }
    CoordStruct position = Coord;
    if (Type->Trailer && Frame % Type->SpawnDelay == 0)
        new AnimClass(Type->Trailer, position, 1, 1, 0x600, 0);
    if (!Map.Unknown_0055A660(unknown_slot104(), true) && unknown_slot282()) {
        Delete_Me();
        return;
    }
    if (unknown_494 == &unknown_dummy_cell_a6f9f8) {
        Assign_Destination(0, true);
        unknown_slot220(0);
        unknown_slot261(false, true);
    }
    if (unknown_228 == &unknown_dummy_cell_a6f9f8) {
        Assign_Destination(0, true);
        unknown_slot220(0);
        unknown_slot261(false, true);
    }
    if (unknown_slot122()) Commence();
    if (unknown_5b0 && Mission != 1) {
        unknown_5b0 = false;
        --unknown_254;
    }
    if (Health_Ratio() < Rules->unknown_1370 && Get_Height_AGL() > 0) {
        int chance = Strength != 0 ? 10 : 80;
        if (unknown_id_source_a3d290->Random()(0, 99) < chance)
            new AnimClass(AnimTypes[AnimTypeClass::From_Name("SGRYSMK1")], CoordStruct(Coord), 0, 1, 0x600, 0);
    }
    if (Cargo.Object && Type->Carryall) {
        ((TechnoClass *)Cargo.Attached_Object())->unknown_2e0 = unknown_2f8;
        ((TechnoClass *)Cargo.Attached_Object())->unknown_2f8 = unknown_2f8;
        CoordStruct passenger_position = Coord;
        Cargo.Attached_Object()->Set_Coord(passenger_position);
    }
}


// AircraftClass mission slot137, 0x00414D90. Complete1024 bytes includes five-way dispatch table.
// State names remain provisional; original call and read ordering retained.
int AircraftClass::Do_MISSION_UNLOAD()
{
    switch (unknown_a8) {
    case 0:
        if (Get_Height_AGL() == 0 && unknown_240 == 0.0) {
            if (unknown_494) {
                CoordStruct current = Coord;
                if (current != unknown_494->Center_Coord()) goto choose_destination;
            }
            unknown_a8 = 3;
mission_delay:
    default: {
        int delay = (int)(Control()->unknown_10 * 900.0);
        return unknown_id_source_a3d290->Random()(0, 2) + delay;
    }

        }
choose_destination:
        if (!unknown_494 && Type->IsDropship && Get_Height_AGL() > 0) {
            TechnoClass *dock = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
            if (dock) Assign_Destination(dock, true);
            else Assign_Destination(unknown_4198b0(), true);
        } else if (!unknown_494) {
            unknown_a8 = 2;
        } else if (unknown_slot309(unknown_494)) {
            FootClass *passenger = (FootClass *)Cargo.Attached_Object();
            if (!passenger || !passenger->Team || passenger->Team->Class->Waypoint_Location() == unknown_geometry_k) {
                Assign_Destination(Select_Landing_Destination_004185b0(&Map[unknown_id_source_a3d290->Get_Waypoint_Location(99)]), true);
                if (Team) Team->unknown_func_6b79e0(unknown_494);
            } else {
                Assign_Destination(Select_Landing_Destination_004185b0(&Map[passenger->Team->Class->Waypoint_Location()]), true);
            }
        } else {
            int flight_level = Type->Flight_Level();
            unknown_a8 = Get_Height_AGL() == flight_level ? 1 : 4;
        }
        goto mission_delay;
    case 1:
        if (!Locomotor.Checked()->Is_Moving()) unknown_a8 = 2;
        if (!unknown_slot309(unknown_494)) unknown_a8 = 0;
        goto mission_delay;
    case 2:
        if (!Locomotor.Checked()->Is_Moving()) unknown_a8 = 3;
        return 1;
    case 3:
        if (!unknown_370) {
            if (Cargo.Object) {
                if (Type->Carryall) {
                    Mark(0);
                    unknown_416420();
                    Mark(1);
                } else {
                FootClass *passenger = (FootClass *)Cargo.Detach_Object();
                Map.Pick_Up(unknown_slot104(), this);
                if (Exit_Object(passenger)) passenger->unknown_37c = false;
                Map.Place_Down(unknown_slot104(), this);
                if (!passenger->unknown_37c && passenger->Team) Team->unknown_func_6b8cc0(passenger);
                if (!Cargo.Object) unknown_slot261(false, true);
                }
            } else {
                unknown_slot261(false, true);
            }
        }
        goto mission_delay;
    case 4:
        unknown_a8 = 0;
        return 1;
    }
}


// int AircraftClass::Do_MISSION_PARADROP_APPROACH() (0x00415190, slot 146)
// Without a target: clear it and queue mission 4. With one but no +0x494:
// slot 260 on it. Within the rules distance (+0x468): queue mission 27 and
// use up one of +0x5BB. Always 3.

int AircraftClass::Do_MISSION_PARADROP_APPROACH()
{
    if (unknown_228 == 0) {
        Assign_Destination(0, true);
        QueueMission(4, false);
        return 3;
    }
    if (unknown_494 == 0) {
        Assign_Destination(unknown_228, true);
        return 3;
    }
    if (Planar_Distance(unknown_228) <= Rules->unknown_468) {
        QueueMission(27, false);
        unknown_5bb--;
    }
    return 3;
}


// int AircraftClass::Do_MISSION_PARADROP_OVERFLY() (0x00415210, slot 147; paradrop overfly
// mission hypothesis). Out of range of the target (Rules +0x468): queue mission
// 26 while +0x5BB is positive, else retreat (mission 4). In range: drop over a
// map cell that passes 0x0055A7F0. Without target or cargo: retreat. Always 5.
// The in-range retreat is written out again in the out-of-range branch; VC6
// merges the two identical copies (the registers only rotate as in the original
// with both statements present).

int AircraftClass::Do_MISSION_PARADROP_OVERFLY()
{
    enum { RetreatMission = 4, ParadropApproachMission = 26 };
    AbstractClass *target = unknown_228;
    unknown_5ba = true;
    if (target && Cargo.Object) {
        if (Planar_Distance(target) > Rules->unknown_468) {
            signed char queue_state = unknown_5bb;
            unknown_5ba = false;
            if (queue_state > 0) {
                QueueMission(ParadropApproachMission, false);
                return 5;
            }
            unknown_slot220(0);
            Assign_Destination(0, true);
            QueueMission(RetreatMission, false);
            return 5;
        } else {
            CoordStruct location = Coord;
            if (Map.Unknown_0055A7F0(location)) unknown_415510();
            return 5;
        }
    } else {
        unknown_5ba = false;
    }
    unknown_slot220(0);
    Assign_Destination(0, true);
    QueueMission(RetreatMission, false);
    return 5;
}


// int AircraftClass::Do_MISSION_RETREAT() (0x00415300, slot 134)
// Without +0x494: target the cell the map finds on the house's edge (+0x128,
// 0 when out of range) unless it finds none. With +0x494: clear the target
// once slot 105 answers it. Always 3.

int AircraftClass::Do_MISSION_RETREAT()
{
    if (unknown_494 == 0) {
        int edge = House->unknown_128;
        if (edge < 0 || edge > 3) {
            edge = 0;
        }
        Cell cell = Map.Calculated_Cell(edge, unknown_geometry_k, unknown_geometry_k, 4, 1, 0);
        if (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y) {
            Assign_Destination((AbstractClass *)&Map[cell], true);
            return 3;
        }
    } else if (unknown_494 == Get_Cell_Ptr()) {
        Assign_Destination(0, true);
    }
    return 3;
}


// int AircraftClass::Exit_Object(TechnoClass *techno) (0x004153C0, slot 62)
// Puts techno down next to the aircraft: tries the facings in table order
// for a cell slot 101 accepts (techno's), then has techno take the spot
// (slot 53) facing that way. On success techno gets mission 2 and the cell
// as target, and the aircraft radios it (message 2; 9 to the contact when
// that answers 1). Returns whether techno was placed.

int AircraftClass::Exit_Object(TechnoClass *techno)
{
    Cell cell(0, 0);
    int index;
    for (index = 0; index < 8; index++) {
        cell = Adjacent_Cell(unknown_slot104(), unknown_facings_7ce4a8[index]);
        if (techno->Can_Enter_Cell(&Map[cell], -1, -1, 0, 1) == 0) {
            break;
        }
    }
    UnknownFacingDir dir = Facing_Dir(unknown_facings_7ce4a8[index]);
    CoordStruct coord = Coord;
    if (techno->Unlimbo(&coord, Dir_To_256(dir))) {
        techno->QueueMission(2, false);
        techno->Assign_Destination((AbstractClass *)&Map[cell], true);
        if (unknown_slot150(2, techno) == 1) {
            unknown_slot149(9);
        }
        return 1;
    }
    return 0;
}


// Aircraft drop helper00415510,original629+11;private current-header diagnostic.
// int-zero return spelling is a hypothesis from both original EAX-zero exits;
// callers ignore the return, so no original source-level return type is claimed.
extern double const unknown_angle_scale_79b620;
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);

int AircraftClass::unknown_415510()
{
    // Cargo detach's FootClass result association remains a layout hypothesis.
    FootClass *passenger = static_cast<FootClass *>(Cargo.Detach_Object());
    if (passenger) {
        --unknown_254;
        CoordStruct landing = Center_Coord();
        UnknownFacingDir facing;
        if (unknown_254 & 1) {
            facing = UnknownFacingDir((unsigned short)(unknown_2e0.Current().Raw - 0x3fff));
        } else {
            facing = UnknownFacingDir((unsigned short)(unknown_2e0.Current().Raw + 0x3fff));
        }
        double angle = ((short)facing.Raw - 0x3fff) * unknown_angle_scale_79b620;
        int altitude = landing.Z;
        double center_y = landing.Y;
        int sample_y = (int)(center_y - unknown_sin_4ba480(angle) * 128.0);
        int sample_x = (int)(unknown_cos_4ba4d0(angle) * 128.0 + landing.X);
        landing = CoordStruct(sample_x, sample_y, altitude);
        CellClass *map_cell = &Map[landing];
        UnknownCellClass *cell = reinterpret_cast<UnknownCellClass *>(map_cell);
        if (passenger->Can_Enter_Cell(map_cell, -1, 0, 0, 1) == 0) {
            CoordStruct spot = cell->Unknown_004764B0(landing, 0, false);
            if (spot != unknown_geometry_l) {
                // The original tests spot.Z, but retains the aircraft altitude.
                landing.X = spot.X;
                landing.Y = spot.Y;
                if (passenger->unknown_slot56(landing)) {
                    {
                    CoordStruct sound_position = Coord;
                    Voc_Play_At(Rules->ChuteSound, sound_position, 0);
                    }
                    passenger->unknown_454 = Coord_Cell(Map[landing].Center_Coord());
                    if (Team) Team->Unknown_006B8FB0(passenger, -1, 0);
                    unknown_244 = CDTimerClass<FrameTimerClass>(0);
                    unknown_5bb = 5;
                    return 0;
                }
            }
        }
        Cargo.Attach(passenger);
        passenger->Hidden();
        ++unknown_254;
    }
    return 0;
}


// AircraftClass::Fire_At (slot 221, 0x00415790): fires the techno weapon, then aims a
// projectile's velocity (level flight for ROT 0, straight at the target for ROT 1) and
// reveals the area around a player aircraft that fires from or into the shroud.

double __fastcall Velocity_Length3_0041B7C0(TVelocity3D<double> const *velocity);
void __fastcall Ensure_Velocity3_0041B7F0(TVelocity3D<double> *velocity);
double __fastcall Velocity_LengthXY_0041B830(TVelocity3D<double> const *velocity);
void __fastcall Ensure_VelocityXY_0041B860(TVelocity3D<double> *velocity);
double __fastcall Aircraft_XY_Length_0041B750(AircraftVectorXY const *vector);

inline void UnknownFacingDir::From_Radian(double angle)
{
    Raw = (unsigned short)(short)((angle - 1.5707963267948966) * -10430.060040584269);
}
// Westwood's TVelocity3D::Set_Speed.
static inline void Set_Speed(TVelocity3D<double> &velocity, double speed)
{
    Ensure_Velocity3_0041B7F0(&velocity);
    double ratio = speed / Velocity_Length3_0041B7C0(&velocity);
    velocity.X *= ratio;
    velocity.Y *= ratio;
    velocity.Z *= ratio;
}

// Westwood's TVelocity3D::Set_Pitch.
static inline void Set_Pitch(TVelocity3D<double> &velocity, UnknownFacingDir const &pitch)
{
    double current_pitch = velocity.Get_Pitch().As_Radian();
    double speed = Velocity_Length3_0041B7C0(&velocity);
    if (current_pitch != 0) {
        velocity.X /= Fast_Cos(current_pitch);
        velocity.Y /= Fast_Cos(current_pitch);
    }
    velocity.X *= Fast_Cos(pitch.As_Radian());
    velocity.Y *= Fast_Cos(pitch.As_Radian());
    velocity.Z = speed * Fast_Sin(pitch.As_Radian());
}

// Westwood's TVelocity2D::Set_Yaw.
static inline void Set_Yaw(TVelocity3D<double> &velocity, UnknownFacingDir const &yaw)
{
    Ensure_VelocityXY_0041B860(&velocity);
    double horizontal_speed = Velocity_LengthXY_0041B830(&velocity);
    double x = horizontal_speed * Fast_Cos(yaw.As_Radian());
    velocity.X = x;
    velocity.Y = -horizontal_speed * Fast_Sin(yaw.As_Radian());
}

// Westwood's TVelocity2D::Get_Yaw.
static inline UnknownFacingDir Get_Yaw(TVelocity3D<double> const &velocity)
{
    UnknownFacingDir dir;
    dir.From_Radian(Fast_Atan2(-velocity.Y, velocity.X));
    return dir;
}

// Westwood's TVelocity3D::Get_Pitch, expanded here (the horizontal speed stays out of line).
static inline UnknownFacingDir Get_Pitch(TVelocity3D<double> const &velocity)
{
    UnknownFacingDir dir;
    dir.From_Radian(Fast_Atan2(velocity.Z, Aircraft_XY_Length_0041B750((AircraftVectorXY const *)&velocity)));
    return dir;
}

// The difference between two coordinates.
static inline UnknownVector3 Coord_Difference(CoordStruct const &to, CoordStruct const &from)
{
    return UnknownVector3(to.X - from.X, to.Y - from.Y, to.Z - from.Z);
}

// A coordinate moved by an offset.
static inline UnknownVector3 Offset(CoordStruct coord, UnknownVector3 const &offset)
{
    return UnknownVector3(offset.X + coord.X, offset.Y + coord.Y, offset.Z + coord.Z);
}

// A vector as a coordinate.
static inline CoordStruct As_Coord(UnknownVector3 const &vector)
{
    return *(CoordStruct const *)&vector;
}

// Westwood's Coord: a point with an inline (x, y, z = 0) constructor over the out-of-line base one.
struct OffsetCoord : public UnknownVector3 {
    OffsetCoord(int x, int y, int z = 0) : UnknownVector3(x, y, z) {}
};

BulletClass *AircraftClass::unknown_slot221(AbstractClass *target, int arg2)
{
    if (Cargo.Object) {
        unknown_415510();
        return 0;
    }

    BulletClass *bullet = TechnoClass::unknown_slot221(target, arg2);
    if (bullet) {
        if (bullet->Type->ROT == 0) {
            Set_Speed(bullet->Velocity, Locomotor.Checked()->Apparent_Speed());
            Set_Pitch(bullet->Velocity, UnknownFacingDir(0x4000));
            Set_Yaw(bullet->Velocity, unknown_2f8.Current());
        }

        if (bullet->Type->ROT == 1) {
            UnknownVector3 diff = Coord_Difference(target->Center_Coord(), Center_Coord());
            TVelocity3D<double> dir = TVelocity3D<double>(diff.X, diff.Y, diff.Z);
            Set_Yaw(bullet->Velocity, Get_Yaw(dir));
            Set_Pitch(bullet->Velocity, Get_Pitch(dir));
            Set_Speed(bullet->Velocity, unknown_slot231(0)->WeaponType->unknown_a8);
        }

        if (House->Is_Player_Control()) {
            if (Map.Is_Shrouded(Position_Coord_004f3f40()) ||
                Map.Is_Shrouded(As_Coord(Offset(Position_Coord_004f3f40(), OffsetCoord(512, 512)))) ||
                Map.Is_Shrouded(As_Coord(Offset(Position_Coord_004f3f40(), OffsetCoord(-512, -512)))) ||
                Map.Is_Shrouded(As_Coord(Offset(Position_Coord_004f3f40(), OffsetCoord(512, -512)))) ||
                Map.Is_Shrouded(As_Coord(Offset(Position_Coord_004f3f40(), OffsetCoord(512, -512)))) ||
                Map.Is_Shrouded(target->Center_Coord())) {
                Map.Sight_From(Position_Coord_004f3f40(), Rules->AttackingAircraftSightRange, House, 0, 0, 0, 1, 0);
                Map.Sight_From(Position_Coord_004f3f40(), Rules->AttackingAircraftSightRange, House, 0, 0, 0, 1, 1);
            }
        }
    }
    if (unknown_5b2) {
        Delete_Me();
    }
    return bullet;
}


// Aircraft damage response (00415E70,slot86): particle or explosion effect after destruction.
int AircraftClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int arg6)
{
    int result = FootClass::Take_Damage(damage, distance, warhead, source, forced, arg6);
    switch (result) {
    case 4: {
        unknown_slot216(0);
        if (warhead == Rules->unknown_cac) {
            int count = abs(unknown_id_source_a3d290->Random()() % 3) + 7;
            for (int remaining = count; remaining > 0; --remaining) {
                ParticleSystemClass *system = new ParticleSystemClass(Rules->unknown_cf4, Center_Coord(), 0, this, unknown_geometry_l);
                system->unknown_e5 = true;
            }
        } else if (Type->Explosion.Count() > 0) {
            unsigned choice;
            TypeList<AnimTypeClass *> *explosions;
            AnimTypeClass *selected;
            // Preserve RNG, list-count and selected-value reads before the position call.
            new AnimClass((choice = (unsigned)unknown_id_source_a3d290->Random()(),
                           explosions = &Type->Explosion,
                           selected = (*explosions)[choice % explosions->Count()]),
                          unknown_slot40(), 0, 1, 0x600, 0);
        }
        // The unknown integer parameter carries the original source pointer.
        if (!unknown_slot224((int)source)) Delete_Me();
        break;
    }
    case 5:
        return 5;
    }
    return result;
}


// Aircraft slot133 move dispatcher00415FF0, independently verified26+6. Private diagnostic only.
int AircraftClass::Do_MISSION_MOVE()
{
    if (Type->Carryall) return Move_Carryall_00416670();
    return unknown_416010();
}


// AircraftClass normal move mission 0x00416010 (slot 133 0x00415FF0 tail; Status 0-4: validate LZ, take off,
// fly to LZ, idle, land).

// Whether two coordinates lie within one cell of each other on the ground plane.
static inline bool Within_One_Cell(CoordStruct here, CoordStruct target)
{
    UnknownVector3 delta = UnknownVector3(here.X - target.X, here.Y - target.Y, 0);
    return delta.Length() < 256;
}

// Do_MISSION_MOVE for aircraft that are not carryalls (0x00415FF0 dispatches here).
int AircraftClass::unknown_416010()
{
    switch (unknown_a8) {
    case 0:
        if (!unknown_494) {
            unknown_slot261(false, true);
        } else {
            Assign_Destination(Select_Landing_Destination_004185b0(unknown_494), true);
            unknown_a8 = 1;
        }
        break;
    case 1: {
        if (!unknown_494) {
            unknown_slot261(false, true);
            return 1;
        }
        CoordStruct destination = unknown_494->unknown_slot19(this);
        Locomotor.Checked()->Move_To(destination);
        unknown_a8 = 2;
        return 1;
    }
    case 2:
        if (unknown_418 != -1) {
            bool proceed = false;
            if (unknown_494) {
                proceed = Within_One_Cell(Coord, unknown_494->Center_Coord());
            } else {
                proceed = true;
            }
            if (proceed) {
                unknown_4cb110(unknown_player_a35db4->WaypointPaths[unknown_418]->Next_Waypoint(
                    unknown_player_a35db4->WaypointPaths[unknown_418]->Get_Waypoint(unknown_576)));
            }
        }
        if (Locomotor.Checked()->Is_Moving()) {
            if (!unknown_494 || Destination_Cell(unknown_494, this) != unknown_slot104()) {
                if (unknown_494 && !Is_Landing_Cell_Available_00419250(Destination_Cell(unknown_494, this), true)) {
                    unknown_a8 = 0;
                } else {
                    unknown_a8 = 4;
                }
            } else {
                unknown_a8 = 3;
            }
        } else {
            unknown_a8 = 3;
        }
        return 1;
    case 3:
        if (!Locomotor.Checked()->Is_Moving()) {
            unknown_slot261(false, true);
        }
        return 1;
    case 4:
        if (Locomotor.Checked()->Is_Moving()) {
            if (!unknown_494 || Destination_Cell(unknown_494, this) != unknown_slot104()) {
                if (unknown_494 && !Is_Landing_Cell_Available_00419250(Destination_Cell(unknown_494, this), true)) {
                    unknown_a8 = 0;
                }
            } else {
                unknown_a8 = 3;
            }
        } else {
            unknown_a8 = 3;
        }
        return 1;
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// Aircraft carryall passenger placement, original 00416420, complete 592+0.
// FootClass passenger identity follows the original locomotor and virtual-slot use.

void AircraftClass::unknown_416420()
{
    Debug_Printf("Do_MISSION_MOVE_Carryall - LAND - Dropping off cargo\n");
    FootClass *passenger = static_cast<FootClass *>(Cargo.Detach_Object());
    CoordStruct position = Coord;
    position.Z = Map.Get_Height_GL(position);
    UnknownCellClass *cell = reinterpret_cast<UnknownCellClass *>(&Map[position]);
    if (cell->Flags & 0x100) {
        position.Z += unknown_geometry_j;
        passenger->unknown_79 = true;
    } else {
        passenger->unknown_79 = false;
    }
    {
        passenger->Locomotor.Checked()->Release();
        passenger->Locomotor.Detach();
        passenger->Locomotor = UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>(
            passenger->Techno_Type_Class()->Locomotor);
    }
    passenger->Locomotor.Checked()->Link_To_Object(passenger);
    if (!passenger->Unlimbo(&position, 0)) {
        Cargo.Attach(passenger);
        return;
    }
    {
        passenger->unknown_37c = false;
        passenger->unknown_slot263(0, 0, false, 0);
        passenger->unknown_slot262(0, 0, 0, 0);
        Map.Unknown_0054A550(CoordStruct(Coord), passenger->unknown_1f4 - 3,
                            passenger->unknown_1f4 + 3, 0);
        Cell next = unknown_6cf130(0);
        if (Contact(0) == passenger) {
            unknown_slot149(25);
            unknown_slot149(3);
        }
        if (next != unknown_geometry_k) {
            Assign_Destination(&Map[next], true);
            return;
        }
        Assign_Destination(0, true);
    }
}


// Aircraft carryall branch of movement slot133, original00416670, complete1444+12.
// Private typed diagnostic candidate, not accepted; one own nonvirtual declaration proposal.
// All446 code instructions and four original switch edges: R354 original-only proof.
// Existing UnknownCellClass view and Foot subtype interpretation remain hypotheses.
// No new Cell/Rules/Bullet fields or foreign interface is proposed or applied.

int AircraftClass::Move_Carryall_00416670()
{
    CoordStruct bridge_position;
    CoordStruct occupant_position;
    switch (unknown_a8) {
    case 0: {
        Debug_Printf("Do_MISSION_MOVE_Carryall - VALIDATE_LZ\n");
        if (!unknown_494) {
            Debug_Printf("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - NavCom == NULL\n");
            unknown_slot261(false, true);
            break;
        }
        TechnoClass *target = unknown_494->As_Techno();
        if (target && !Cargo.Object && House->Is_Ally(target)) {
            // Models the original byte temporary; source type/spelling remain nonunique.
            bool const &target_is_techno = target->AbstractFlags.IsTechno;
            if (target_is_techno && !target->Owning_House()->Is_Ally(this)) goto select_destination;
            if (target->Kind_Of() == 1) {
                Debug_Printf("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - target != NULL\n");
                if (Contact(0) != target) unknown_slot149(3);
                if (unknown_slot150(2, target) == 1) {
                    Debug_Printf("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - RADIO_HELLO got RADIO_ROGER\n");
                    if (unknown_slot149(36) != 1) {
                        Debug_Printf("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - RADIO_WANT_RIDE did not get RADIO_ROGER\n");
                        unknown_slot149(3);
                        unknown_slot261(false, true);
                        break;
                    }
                    unknown_slot149(7);
                } else {
                    Assign_Destination(0, true);
                    unknown_slot261(false, true);
                    break;
                }
            }
        }
select_destination:
        Assign_Destination(Select_Landing_Destination_004185b0(unknown_494), true);
        if (Team) Team->unknown_func_6b79e0(unknown_494);
        unknown_a8 = 1;
        Debug_Printf("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - Status = TAKE_OFF\n");
        break;
    }
    case 1: {
        if (!unknown_494) {
            unknown_slot261(false, true);
            return 1;
        }
        CoordStruct destination = unknown_494->Center_Coord();
        Locomotor.Checked()->Move_To(destination);
        unknown_a8 = 2;
        return 1;
    }
    case 2:
        if (unknown_494 && unknown_494->Kind_Of() != 11 && Contact(0) != unknown_494) {
            Debug_Printf("Do_MISSION_MOVE_Carryall - FLY_TO_LZ - Lost contact\n");
            unknown_a8 = 0;
            return 1;
        }
        if (unknown_494 && unknown_494->Kind_Of() == 11 && !unknown_slot309(unknown_494)) {
            unknown_a8 = 0;
            return 1;
        }
        if (Locomotor.Checked()->Get_Status() == 1) {
            Debug_Printf("Do_MISSION_MOVE_Carryall - FLY_TO_LZ - Begin landing\n");
            if (unknown_494 && unknown_494->Kind_Of() == 1) {
                CoordStruct current_position = Coord;
                Cell current_cell = Coord_Cell(current_position);
                CoordStruct target_position = static_cast<ObjectClass *>(unknown_494)->Coord;
                Cell target_cell = Coord_Cell(target_position);
                if (target_cell != current_cell) {
                    unknown_a8 = 0;
                    Debug_Printf("Do_MISSION_MOVE_Carryall - FLY_TO_LZ - Target moved\n");
                    return 1;
                }
            }
            unknown_5bc = false;
        }
        if (!Locomotor.Checked()->Is_Moving()) {
            Debug_Printf("Do_MISSION_MOVE_Carryall - FLY_TO_LZ - Stopped moving\n");
            unknown_a8 = 3;
        }
        return 1;
    case 3: {
        Debug_Printf("Do_MISSION_MOVE_Carryall - LAND\n");
        if (Cargo.Object && unknown_3f8) {
            unknown_5bc = true;
            return 1;
        }
        if (Cargo.Object) {
            Mark(0);
            unknown_416420();
            Mark(1);
            unknown_a8 = 0;
            return 1;
        }
        Debug_Printf("Do_MISSION_MOVE_Carryall - LAND - Picking up cargo\n");
        Mark(0);
        bridge_position = Coord;
        occupant_position = Coord;
        FootClass *passenger = reinterpret_cast<UnknownCellClass *>(&Map[occupant_position])->Unknown_00473E30(
            (reinterpret_cast<UnknownCellClass *>(&Map[bridge_position])->Flags >> 8) & 1);
        if (passenger && passenger == Contact(0)) {
            Debug_Printf("Do_MISSION_MOVE_Carryall - LAND - Got Cell_Unit\n");
            if (unknown_slot149(19) == 1) {
                Debug_Printf("Do_MISSION_MOVE_Carryall - LAND - RADIO_NEED_TO_MOVE got RADIO_ROGER\n");
                passenger->Limbo();
                passenger->unknown_79 = false;
                passenger->unknown_37c = true;
                Cargo.Attach(passenger);
                goto finalize_pickup;
            }
        }
        unknown_a8 = 0;
finalize_pickup:
        unknown_slot149(3);
        Mark(1);
        unknown_5bc = true;
        unknown_slot261(false, true);
        return 1;
    }
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// AircraftClass::Do_MISSION_PATROL 0x00416C20: Do_MISSION_PATROL (Status 0-4: validate LZ, take off,
// patrol toward the LZ while looking for a dock when out of ammo or a threat, idle, land).

// Whether two coordinates lie within one cell of each other on the ground plane (2D length).
static inline bool Near_In_Plane(CoordStruct here, CoordStruct target)
{
    CoordStruct delta(here.X - target.X, here.Y - target.Y, 0);
    double y = delta.Y;
    double x = delta.X;
    return (int)unknown_sqrt_4ba410(x * x + y * y) < 256;
}

int AircraftClass::Do_MISSION_PATROL()
{
    unknown_37f = true;
    switch (unknown_a8) {
    case 0:
        if (!unknown_494) {
            unknown_slot261(false, true);
        } else {
            Assign_Destination(Select_Landing_Destination_004185b0(unknown_494), true);
            if (Team) {
                Team->unknown_func_6b79e0(unknown_494);
            }
            unknown_a8 = 1;
        }
        break;
    case 1: {
        if (!unknown_494) {
            unknown_slot261(false, true);
            return 1;
        }
        CoordStruct destination = unknown_494->unknown_slot19(this);
        Locomotor.Checked()->Move_To(destination);
        unknown_a8 = 2;
        return 1;
    }
    case 2: {
        CoordStruct position;
        if (unknown_254 == 0) {
            TechnoClass *dock = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
            if (dock) {
                Override_Mission(7, 0, dock);
                unknown_a8 = 0;
                return 1;
            }
        }
        if (unknown_254 != 0) {
            position = Coord;
            AbstractClass *threat = unknown_slot219(2, &position, false);
            if (threat && threat->AbstractFlags.IsObject) {
                Override_Mission(1, threat, 0);
                unknown_a8 = 0;
                return 1;
            }
        }
        if (unknown_418 != -1) {
            bool proceed = false;
            if (unknown_494) {
                proceed = Near_In_Plane(Coord, unknown_494->Center_Coord());
            } else {
                proceed = true;
            }
            if (proceed) {
                unknown_4cb110(unknown_player_a35db4->WaypointPaths[unknown_418]->Next_Waypoint(
                    unknown_player_a35db4->WaypointPaths[unknown_418]->Get_Waypoint(unknown_576)));
            }
        }
        if (!Locomotor.Checked()->Is_Moving()) {
            unknown_a8 = 3;
        } else {
            if (unknown_494 && !Is_Landing_Cell_Available_00419250(Destination_Cell(unknown_494, this), true)) {
                unknown_a8 = 0;
            } else {
                unknown_a8 = 4;
            }
        }
        return 1;
    }
    case 3:
        unknown_slot261(false, true);
        return 1;
    case 4:
        if (!Locomotor.Checked()->Is_Moving()) {
            unknown_a8 = 3;
        } else {
            if (unknown_494) {
                if (!Is_Landing_Cell_Available_00419250(Destination_Cell(unknown_494, this), true)) {
                    unknown_a8 = 0;
                }
            }
        }
        return 1;
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// Aircraft mission selection 0x00417010, slot261. Behavior labels are hypotheses.
bool AircraftClass::unknown_slot261(bool first, bool second)
{
    if (Has_Suspended_Mission()) {
        Restore_Mission();
        if (Mission == 25) {
            unknown_a8 = 0;
            unknown_5ba = false;
        }
        return false;
    }
    int current_mission = Mission;
    bool mission_blocks_selection = current_mission == 4 || (current_mission > 25 && current_mission <= 27);
    if (mission_blocks_selection) return false;
    if (MissionQueue == 26) {
        Commence();
        return false;
    }
    bool result = FootClass::unknown_slot261(first, second);
    int mission = !House->Is_Human_Player() && !Team && unknown_slot163() ? 11 : 5;
    int landing_altitude = Landing_Altitude();
    if (In_Which_Layer() == 2) goto grounded;
    if (Get_Height_AGL() <= landing_altitude || Type->MissileSpawn) goto grounded;
    if (Cargo.Object) {
        if (!unknown_32c) goto select_destination;
        if (!Team) {
            mission = 16;
            Assign_Destination(unknown_4198b0(), true);
            goto choose_mission;
        }
        goto default_mission;
    } else {
        if (!unknown_32c) goto check_owner;
        if (!House->Is_Human_Player()) goto check_owner;
        goto check_team;
check_owner:
        if (!House->Is_Human_Player() && Type->Ammo == 0) goto check_team;
        goto check_weapon;
check_team:
        if (Team && Team->unknown_func_6baa00())
            Team->Unknown_006B8FB0(this, -1, 0);
check_weapon:
        if (unknown_slot231(0)->WeaponType) {
        if (unknown_32c) {
            if (!unknown_254) {
                if (Team) Team->Unknown_006B8FB0(this, -1, 0);
                unknown_slot220(0);
                Assign_Destination(0, true);
                mission = 4;
            } else if (!Team) mission = 15;
            goto choose_mission;
        }
        if ((unknown_254 && unknown_228 && Get_Mission() == 1) || MissionQueue == 1) {
            mission = 1;
            goto choose_mission;
        }
        if (!In_Air() || (unknown_494 && Mission == 7) || Type->Dock.Count() <= 0 ||
            (!unknown_32d && Team)) goto choose_mission;
        {
            TechnoClass *dock = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
            Assign_Destination(0, true);
            if (dock && unknown_slot150(2, dock) == 1) {
                Assign_Destination(dock, true);
                mission = 7;
                goto choose_mission;
            }
        }
        if (Type->AirportBound) {
            unknown_slot224(0);
            return false;
        }
        goto select_destination;
        }
        if (Team) return false;
        goto select_destination;
    }
select_destination:
    Assign_Destination(unknown_4198b0(), true);
    mission = 2;
    goto choose_mission;
grounded:
    if (unknown_32c) {
        if (Cargo.Object) mission = Team ? 5 : 16;
        else if (!Team) {
            unknown_slot220(0);
            Assign_Destination(0, true);
            mission = 4;
        }
        goto choose_mission;
    }
    Assign_Destination(0, true);
    unknown_slot220(0);
    if (!House->Is_Human_Player() && !Team && unknown_slot163()) {
        mission = 11;
        goto choose_mission;
    }
default_mission:
    mission = 5;
choose_mission:
    if (!unknown_254 && unknown_slot163() && !HasAnyLink()) {
        TechnoClass *dock = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
        if (dock) {
            Assign_Destination(dock, true);
            unknown_slot220(0);
            mission = 7;
        } else if (Type->AirportBound) {
            unknown_slot224(0);
            return false;
        }
    }
    if (HasAnyLink()) mission = 7;
    QueueMission(mission, false);
    if (unknown_slot122()) Commence();
    return result;
}


// bool AircraftClass::unknown_slot78(int arg1, ObjectClass *arg2, bool arg3) (0x004174B0, slot 78)
// Acts on slot 28's answer for (arg2, arg3): 2 does nothing, 3, 4 and 17
// go to slot 200 (7, 16 and 2), anything else to FootClass's slot 78 with
// the answer in place of arg1.

bool AircraftClass::unknown_slot78(int arg1, ObjectClass *arg2, bool arg3)
{
    int action = unknown_slot28((ObjectClass *)arg2, arg3);
    switch (action) {
    case 17:
        return unknown_slot200(2, 0, arg2, 0);

    case 2:
        return false;

    case 3:
        return unknown_slot200(7, 0, arg2, 0);

    case 4:
        return unknown_slot200(16, 0, 0, 0);
    }
    return FootClass::unknown_slot78(action, arg2, arg3);
}


// bool AircraftClass::unknown_slot77(int arg1, Cell const *arg2, Cell const *arg3, bool arg4) (0x00417560, slot 77)
// FootClass's.

bool AircraftClass::unknown_slot77(int arg1, Cell const *arg2, Cell const *arg3, bool arg4)
{
    return FootClass::unknown_slot77(arg1, arg2, arg3, arg4);
}


// bool AircraftClass::unknown_slot200(int arg1, AbstractClass *arg2, AbstractClass *arg3, AbstractClass *arg4) (0x00417580, slot 200)
// TechnoClass's.

bool AircraftClass::unknown_slot200(int arg1, AbstractClass *arg2, AbstractClass *arg3, AbstractClass *arg4)
{
    return TechnoClass::unknown_slot200(arg1, arg2, arg3, arg4);
}


// int AircraftClass::unknown_slot28(ObjectClass *object, bool arg2) (0x004175A0)
// The action against an object (hypothesis: What_Action): the foot's
// answer, adjusted. An airport-bound aircraft does not use action 0x1A. A
// player's carryall picks up (0x11) an allied empty unit when not carrying,
// and otherwise asks slot 27 about the object's cell for action 0; entering
// (4) needs a passenger and firing (5) a primary weapon; a player's
// aircraft docks (3) at an airfield (the rules' +0x778 type) or helipad it
// is in radio contact with when the pad is empty and accepted (message 0xF). A
// carryall does not pick up from a weapons factory's cell; action 2 on a
// slot-48 object that is not owned and whose slot-50 type is 0x15 becomes
// 1; fire on an object whose type has +0x203 becomes 2.

// A building as read here (a view of BuildingClass).
struct UnknownAircraftBuilding {
    char unknown_000[0x418];
    BuildingTypeClass *Type;                    // +0x418
};
// A cell as read here (a view of CellClass).
class UnknownAircraftCell {
public:
    UnknownAircraftBuilding *Unknown_004717B0();    // 0x004717B0 (the building in the cell)
};

int AircraftClass::unknown_slot28(ObjectClass *object, bool arg2)
{
    int action = FootClass::unknown_slot28(object, arg2);
    if (action == 8) {
        return action;
    }
    if (action == 0x1A && Type->AirportBound) {
        action = 0;
    }
    if (Type->Carryall && House->Is_Player_Control()) {
        if (action == 7 || action == 0) {
            if (House->Is_Ally((TechnoClass *)object)) {
                bool active;
                if (object != 0) {
                    active = object->AbstractFlags.IsTechno;
                }
                if (object == 0 || !active || object->Owning_House()->Is_Ally(this)) {
                    if (Cargo.Object == 0 && object->Kind_Of() == 1) {
                        action = 0x11;
                        goto docking;
                    }
                }
            }
        } else {
            goto checks;
        }
    }
    if (action == 0) {
        Cell cell = Coord_Cell(object->Center_Coord());
        action = unknown_slot27(&cell, action != 0, arg2);
    }
checks:
    if (action == 4) {
        if (Cargo.Count == 0) {
            action = 0;
        }
    } else if (action == 5) {
        if (unknown_slot231(0)->WeaponType == 0) {
            action = 0;
        }
    }
docking:
    if (House->Is_Player_Control() && action == 7 && object->Kind_Of() == 6) {
        BuildingTypeClass *type = ((UnknownAircraftBuilding *)object)->Type;
        if ((type == Rules->unknown_778 || type->unknown_1365)
            && ((TechnoClass *)object)->Unknown_00636ED0(this) && ((TechnoClass *)object)->Cargo.Object == 0
            && unknown_slot150(0xF, (TechnoClass *)object) == 1) {
            action = 3;
        }
    }
    if (Type->Carryall && action == 0x11) {
        Cell cell = object->unknown_slot104();
        if (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y) {
            UnknownAircraftBuilding *building = ((UnknownAircraftCell &)((UnknownPlacementCellView &)Map[cell])).Unknown_004717B0();
            if (building != 0 && building->Type->unknown_1357) {
                action = 0;
            }
        }
    } else if (action == 2) {
        if (object->unknown_slot48() && object->unknown_slot51(true) == 0 && object->unknown_slot50(true)->Kind_Of() == 0x15) {
            action = 1;
        }
    } else if (action == 5 && object->Class_Of()->unknown_203) {
        action = 2;
    }
    return action;
}


// int AircraftClass::unknown_slot27(Cell const *arg1, bool arg2, bool arg3) (0x00417840, slot 27)
// 0 unless the house allows it (0x004F52B0); FootClass's action, except
// that 5 becomes 0 without a primary weapon.

int AircraftClass::unknown_slot27(Cell const *arg1, bool arg2, bool arg3)
{
    if (!House->Is_Player_Control()) {
        return 0;
    }
    int action = FootClass::unknown_slot27(arg1, arg2, arg3);
    if (action == 5 && unknown_slot231(0)->WeaponType == 0) {
        action = 0;
    }
    return action;
}


unsigned int AircraftClass::Pose_Dir_00417890() const { return Rules->unknown_50; }


// AircraftClass::Do_MISSION_ATTACK 0x004178A0: Do_MISSION_ATTACK (Status 0-10: validate, pick an attack
// location, fly to it, fire, fire again, the strafing run's later shots, return to base).
// Body 3330 bytes including the main and six Can_Fire switch tables; 14 nop pad bytes follow.

extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;

// The direction from one coordinate to another (Westwood's DirType(Coord, Coord); the out-of-line
// copy is 0x00423EF0, this unit inlines it).
inline UnknownFacingDir::UnknownFacingDir(CoordStruct const &from, CoordStruct const &to)
{
    if (from.X == to.X && from.Y == to.Y) Raw = 0;
    *this = UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y) - to.Y,
        double(to.X) - from.X) - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
}

int AircraftClass::Do_MISSION_ATTACK()
{
    switch (unknown_a8) {
    case 0:
        unknown_5ba = false;
        if (unknown_228 == 0) {
            unknown_a8 = 10;
        } else {
            unknown_a8 = 1;
        }
        return 1;
    case 1:
        unknown_5ba = false;
        if (unknown_5b0) {
            unknown_5b0 = false;
            unknown_254--;
        }
        if (unknown_228 == 0 || unknown_254 == 0) {
            unknown_a8 = 10;
        } else {
            Assign_Destination(unknown_418f10(unknown_228), true);
            if (unknown_494 != 0) {
                unknown_a8 = 3;
            } else {
                unknown_a8 = 10;
            }
        }
        break;
    case 3:
        if (unknown_5b0) {
            unknown_5b0 = false;
            unknown_254--;
        }
        unknown_5ba = false;
        if (unknown_228 != 0 && unknown_254 != 0) {
            if (Is_Strafe()) {
                if (Planar_Distance(unknown_228) < unknown_slot231(0)->WeaponType->unknown_b4) {
                    unknown_a8 = 4;
                    return 1;
                }
                Assign_Destination(unknown_228, true);
            } else {
                if (Is_Fighter() || !Locomotor.Checked()->Is_Moving_Now()) {
                    unknown_a8 = 4;
                    return 1;
                }
            }
            if (unknown_494 == 0) {
                unknown_a8 = 1;
                return 1;
            }
            int distance = Planar_Distance(unknown_494);
            if (distance < 0x200) {
                unknown_2f8.Set_Desired(Direction_To(unknown_228));
                if (distance < 0x10) {
                    unknown_a8 = 4;
                    Assign_Destination(0, true);
                }
            } else {
                unknown_2f8.Set_Desired(UnknownFacingDir(unknown_slot43(0), unknown_494->Center_Coord()));
                return 1;
            }
        } else {
            unknown_a8 = 10;
        }
        return 1;
    case 4:
        if (unknown_228 == 0 || unknown_254 == 0) {
            unknown_a8 = 10;
            return 1;
        }
        if (!Is_Strafe()) {
            unknown_2e0.Set_Desired(Direction_To(unknown_228));
            unknown_2f8.Set_Desired(Direction_To(unknown_228));
        }
        switch (unknown_slot218(unknown_228, unknown_slot174(unknown_228), 1)) {
        case 9:
            unknown_slot252(false);
            break;
        case 3:
            break;
        case 2:
            if (unknown_254 == 0) {
                unknown_a8 = 10;
            } else {
                if (unknown_slot213(unknown_228) && !Is_Strafe()) {
                    if (Is_Fighter()) {
                        unknown_a8 = 4;
                    } else {
                        unknown_a8 = Rules->CurleyShuffle ? 1 : 4;
                    }
                } else {
                    unknown_a8 = 1;
                }
                if (Is_Strafe()) {
                    return 45;
                }
            }
            break;
        case 0: {
            unknown_5b0 = true;
            unknown_slot221(unknown_228, unknown_slot174(unknown_228));
            Map[unknown_228->Center_Coord()].Incoming(&Position_Coord_004f3f40(), true, false, false);
            if (Is_Strafe()) {
                unknown_a8 = 6;
                unknown_5ba = true;
                return unknown_slot231(0)->WeaponType->unknown_b0;
            }
            if (Is_Fighter()) {
                unknown_5ba = true;
                unknown_a8 = unknown_254 <= 0 ? 10 : 1;
                return unknown_slot231(0)->WeaponType->unknown_b0;
            }
            unknown_a8 = 5;
            break;
        }
        default:
            if (unknown_254 == 0) {
                unknown_a8 = 10;
            } else if (!Is_Strafe()) {
                unknown_a8 = 5;
            }
            break;
        }
        return 1;
    case 5:
        if (unknown_228 == 0) {
            unknown_a8 = 10;
            return 1;
        }
        unknown_2e0.Set_Desired(Direction_To(unknown_228));
        unknown_2f8.Set_Desired(Direction_To(unknown_228));
        switch (unknown_slot218(unknown_228, unknown_slot174(unknown_228), 1)) {
        case 9:
            unknown_slot252(false);
            break;
        case 3:
            break;
        case 2:
            if (unknown_254 == 0) {
                unknown_a8 = 10;
            } else {
                if (!unknown_slot213(unknown_228) || Is_Strafe()) {
                    unknown_a8 = 1;
                } else {
                    unknown_a8 = Rules->CurleyShuffle ? 1 : 4;
                }
                if (Is_Strafe()) {
                    return 45;
                }
            }
            break;
        case 0: {
            unknown_slot221(unknown_228, unknown_slot174(unknown_228));
            Map[unknown_228->Center_Coord()].Incoming(&Position_Coord_004f3f40(), true, false, false);
            if (unknown_254 != 0) {
                unknown_a8 = Rules->CurleyShuffle ? 1 : 4;
            } else {
                unknown_a8 = 10;
            }
            break;
        }
        default:
            if (unknown_254 == 0) {
                unknown_a8 = 10;
            } else if (!unknown_slot213(unknown_228)) {
                unknown_a8 = 1;
            } else {
                unknown_a8 = Rules->CurleyShuffle ? 1 : 4;
            }
            break;
        }
        break;
    case 6:
        if (unknown_228 == 0) {
            unknown_a8 = 10;
            return 1;
        }
        switch (unknown_slot218(unknown_228, unknown_slot174(unknown_228), 1)) {
        case 8:
            Assign_Destination(unknown_228, true);
        case 0:
        case 2:
        case 9: {
            unknown_slot221(unknown_228, unknown_slot174(unknown_228));
            Map[unknown_228->Center_Coord()].Incoming(&Position_Coord_004f3f40(), true, false, false);
            Assign_Destination(unknown_228, true);
            unknown_a8 = 7;
            return unknown_slot231(0)->WeaponType->unknown_b0;
        }
        default:
            if (unknown_254 == 0) {
                unknown_a8 = 10;
                unknown_5ba = false;
            }
            return 1;
        }
    case 7:
        if (unknown_228 == 0) {
            unknown_a8 = 10;
            return 1;
        }
        switch (unknown_slot218(unknown_228, unknown_slot174(unknown_228), 1)) {
        case 8:
            Assign_Destination(unknown_228, true);
        case 0:
        case 2:
        case 9: {
            unknown_slot221(unknown_228, unknown_slot174(unknown_228));
            Map[unknown_228->Center_Coord()].Incoming(&Position_Coord_004f3f40(), true, false, false);
            Assign_Destination(unknown_228, true);
            unknown_a8 = 8;
            return unknown_slot231(0)->WeaponType->unknown_b0;
        }
        default:
            if (unknown_254 == 0) {
                unknown_a8 = 10;
                unknown_5ba = false;
            }
            return 1;
        }
    case 8:
        if (unknown_228 == 0) {
            unknown_a8 = 10;
            return 1;
        }
        switch (unknown_slot218(unknown_228, unknown_slot174(unknown_228), 1)) {
        case 8:
            Assign_Destination(unknown_228, true);
        case 0:
        case 2:
        case 9: {
            unknown_slot221(unknown_228, unknown_slot174(unknown_228));
            Map[unknown_228->Center_Coord()].Incoming(&Position_Coord_004f3f40(), true, false, false);
            Assign_Destination(unknown_228, true);
            unknown_a8 = 9;
            return unknown_slot231(0)->WeaponType->unknown_b0;
        }
        default:
            if (unknown_254 == 0) {
                unknown_a8 = 10;
                unknown_5ba = false;
            }
            return 1;
        }
    case 9:
        if (unknown_228 == 0) {
            unknown_a8 = 10;
            return 1;
        }
        switch (unknown_slot218(unknown_228, unknown_slot174(unknown_228), 1)) {
        case 0:
        case 2:
        case 8:
        case 9: {
            unknown_slot221(unknown_228, unknown_slot174(unknown_228));
            Map[unknown_228->Center_Coord()].Incoming(&Position_Coord_004f3f40(), true, false, false);
            unknown_a8 = 3;
            return (unknown_slot231(0)->WeaponType->unknown_b4 + 0x400) / Type->Speed;
        }
        default:
            if (unknown_254 == 0) {
                unknown_a8 = 10;
                unknown_5ba = false;
            }
            return 1;
        }
    case 10:
        unknown_5ba = false;
        if (unknown_5b0) {
            unknown_5b0 = false;
            if (unknown_254 > 0) {
                unknown_254--;
            }
        }
        if (unknown_254 == 0) {
            if (unknown_32c || House->Is_Human_Player()) {
                unknown_slot220(0);
            }
        } else if (unknown_228 != 0) {
            unknown_a8 = 1;
            return 1;
        }
        unknown_5ba = false;
        Assign_Destination(0, true);
        unknown_5bd = false;
        unknown_slot261(false, true);
        unknown_5bd = true;
        return 1;
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// AircraftClass::Select_Landing_Destination 0x004185B0 (Westwood's New_LZ): keeps a usable landing zone, otherwise
// scans rings of cells around it (eight directions from a random start) for a free landing cell.
// RulesClass +0x10E0 (the scan radius in leptons) is a hypothesis.
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);
// The direction in radians (Westwood's DirType::As_Radian).
inline double UnknownFacingDir::As_Radian() const
{
    return ((int)(short)Raw - 16383) * -9.587672516830327e-05;
}

// The cell a distance away in a direction (Westwood's Move_Cell).
static inline Cell Move_Cell(Cell const &start, UnknownFacingDir const &dir, short distance)
{
    double radians = dir.As_Radian();
    return Cell(start.X + unknown_cos_4ba4d0(radians) * distance, start.Y - unknown_sin_4ba480(radians) * distance);
}

AbstractClass *AircraftClass::Select_Landing_Destination_004185b0(AbstractClass *desired)
{
    if (desired) {
        if (Team && Team->Unknown_006BA990()) return desired;
        if (unknown_slot309(desired)) {
            Cell cell = Coord_Cell(desired->unknown_slot19(0));
            if (Is_Landing_Cell_Available_00419250(cell, false)) return desired;
        }
        CoordStruct coord = desired->Center_Coord();
        if (!unknown_a40774) {
            for (int radius = 0; radius < Rules->unknown_10e0 / 256; radius++) {
                int phase = unknown_id_source_a3d290->Random()(0, 7);
                Cell previous(0, 0);
                for (int direction = 0; direction < 8; direction++) {
                    Cell cell = Move_Cell(Coord_Cell(coord), UnknownFacingDir((unsigned short)(Facing_Dir(direction + phase).Raw & ~0x1fff)), radius);
                    if (Map.Unknown_0055A660(cell, true)) {
                        CellClass *candidate = &Map[cell];
                        if (cell != previous && unknown_slot309(candidate) &&
                            Is_Landing_Cell_Available_00419250(cell, false)) {
                            return candidate;
                        }
                        previous = cell;
                    }
                }
            }
        }
    }
    return desired;
}


// AircraftClass Receive_Message (slot 95, 0x00418840): the radio handler. Body 1186 bytes including the two-level switch tables; 14 nop pad bytes follow.

// Aircraft radio handler slot95,0x00418840. Full1186-byte body includes two-level70-byte switch data;14 frozen padding.
// Classify from the common RadioClass prefix before accessing Infantry-owned fields.
static inline InfantryClass *Aircraft_Radio_Infantry(RadioClass *radio)
{
    if (!radio) return 0;
    return radio->Kind_Of() == 15 ? (InfantryClass *)radio : 0;
}
static inline bool Aircraft_Accepts_Radio_00418840(int mission) { return mission != 4 && (mission <= 25 || mission > 27); }
static inline bool Aircraft_Radio_Target_Is_Current_Cell_00418840(AircraftClass *aircraft)
{
    AbstractClass *cell = aircraft->Get_Cell_Ptr();
    AbstractClass *target = aircraft->unknown_494;
    return target == cell;
}
int AircraftClass::Receive_Message(RadioClass *radio, int message, AbstractClass *&param)
{
    if (!Aircraft_Accepts_Radio_00418840(Mission)) return 0;
    switch (message) {
    case 31: {
        AircraftTypeClass *type = Type;
        int current_ammo = unknown_254;
        int capacity = type->Ammo;
        if (current_ammo >= capacity / 2 && unknown_228) return 1;
        return FootClass::Receive_Message(radio, message, param);
    }
    case 29: {
        if (unknown_228) return 10;
        AircraftTypeClass *type = Type;
        int current_ammo = unknown_254;
        int capacity = type->Ammo;
        return current_ammo == capacity ? 1 : 10;
    }
    case 33: {
        AircraftTypeClass *type = Type;
        int current_ammo = unknown_254;
        int capacity = type->Ammo;
        return current_ammo == capacity ? 1 : 10;
    }
    case 23:
        QueueMission(2, false);
        Assign_Destination(unknown_4198b0(), true);
        unknown_slot149(3);
        break;
    case 18: {
        FootClass::Receive_Message(radio, message, param);
        AbstractClass *destination = param;
        if (destination && destination->Kind_Of() == 6) {
            if (unknown_slot150(15, destination->As_Techno()) != 1) return 10;
            QueueMission(7, false);
            Assign_Destination(destination, true);
        } else {
            QueueMission(2, false);
            Assign_Destination(destination, true);
        }
        Commence();
        return 1;
    }
    case 19:
        FootClass::Receive_Message(radio, message, param);
        if (!Locomotor.Checked()->Is_Moving() || !unknown_494) return 1;
        if (Type->AirportBound && unknown_494->Kind_Of() == 11 && Aircraft_Radio_Target_Is_Current_Cell_00418840(this)) return 1;
        return 10;
    case 21: {
        AircraftTypeClass *type = Type;
        int count = Cargo.Count;
        int capacity = type->Passengers;
        if (count == capacity) unknown_2a8.Count_Down(type->DeployTime);
        InfantryClass *infantry = Aircraft_Radio_Infantry(radio);
        if (infantry && infantry->Type->Civilian && !infantry->unknown_5c1) {
            unknown_slot220(0);
            Assign_Destination(0, true);
            QueueMission(4, false);
        }
        return 5;
    }
    case 14: {
        int capacity = Type->Passengers;
        if (capacity > 0 && Cargo.Count < capacity) {
            FootClass::Receive_Message(radio, message, param);
            if (!Locomotor.Checked()->Is_Moving()) {
                unknown_2a8.Count_Up(Type->DeployTime);
                if (unknown_slot150(19, (TechnoClass *)radio) == 1) {
                    param = this;
                    if (unknown_slot151(18, &param, (TechnoClass *)radio) != 1) {
                        unknown_slot150(3, (TechnoClass *)radio);
                        return 1;
                    }
                    Contact(0)->Unselect();
                }
            }
            return 1;
        }
        break;
    }
    case 15: {
        if (Type->Passengers == 0 || !radio || !House->Is_Ally((TechnoClass *)radio)) return 0;
        AircraftTypeClass *type = Type;
        int count = Cargo.Count;
        int capacity = type->Passengers;
        return count < capacity ? 1 : 10;
    }
    case 8:
        if (Type->Carryall && Get_Mission() == 2 && unknown_370) {
            if ((Cargo.Object && Cargo.Attached_Object() == radio) || unknown_494 == radio) return 10;
        }
        break;
    default:
        break;
    }
    return FootClass::Receive_Message(radio, message, param);
}


// int AircraftClass::unknown_slot182(AbstractClass *object, Cell *cell) (0x00418CF0, slot 182)
// Looks for a cell next to the aircraft (facings in table order) that is
// on the map and either object's cell or passes the cell's test
// (0x00478760); leaves it in *cell (the last one tried otherwise; the zero
// cell before the search). Always 0.

int AircraftClass::unknown_slot182(AbstractClass *object, Cell *cell)
{
    *cell = unknown_geometry_k;
    Cell center = Coord_Cell(Center_Coord());
    for (int index = 0; index < 8; index++) {
        *cell = Adjacent_Cell(Cell(center), unknown_facings_7ce4c8[index]);
        if (Map.In_Radar(*cell)) {
            Cell here = Coord_Cell(object->Center_Coord());
            if (here.X == cell->X && here.Y == cell->Y) {
                break;
            }
            if (Map[*cell].Unknown_00478760(0, 0, 0, -1, 0, -1, 1)) {
                break;
            }
        }
    }
    return 0;
}


// int AircraftClass::Can_Enter_Cell(CellClass *arg1, int arg2, int arg3, CellClass *arg4, bool arg5) (0x00418E00, slot 101)
// Whether the aircraft may enter cell arg1 (0 yes, 7 no): a team member
// stays within its area (0x0055A740); a cell held by an active non-allied
// object (other than a building with Invisible types, or in some modes)
// must pass the cell's test (0x00478760); and some aircraft (+0x372) avoid
// cells the map rejects (0x00568140) outside game mode 0.

int AircraftClass::Can_Enter_Cell(CellClass *arg1, int arg2, int arg3, CellClass *arg4, bool arg5)
{
    CellClass *cell = arg1;
    if (Team != 0 && Team->Unknown_006BA990() && !Map.Unknown_0055A740(cell, 1)) {
        return 0;
    }
    TechnoClass *occupier = cell->Occupier;
    bool active;
    if (occupier != 0) {
        active = occupier->AbstractFlags.IsTechno;
    }
    if ((occupier == 0 || !active || occupier->House->Is_Ally(House)
         || (occupier->unknown_1b8 != 2 && unknown_a40774 == 0
             && (occupier->Kind_Of() != 6 || !((BuildingClass *)occupier)->Type->Invisible)))
        && !cell->Unknown_00478760(4, 0, 0, -1, 0, -1, 1)) {
        return 7;
    }
    if (unknown_game_mode_a3d298 == 0 && unknown_372 && !unknown_32c) {
        if (Map.Is_Shrouded(cell->Center_Coord())) {
            return 7;
        }
    }
    return 0;
}


// AircraftClass Good_Fire_Location 0x00418F10 (name from TibSun aircraft.cpp; thiscall, ret 4): searches rings
// of cells around the target for a landing-capable firing spot, nearest to the target's NavCom (or to this
// aircraft), and returns the best or the second-best cell at random.
extern double const unknown_angle_scale_79b620;
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);

// Explicit modulo-2^32 coordinate subtraction, as executed by the original SUB.
static inline int Aircraft_Wrapped_Subtract_00418F10(int left, int right)
{
    return (int)((unsigned int)left - (unsigned int)right);
}

// The ground-plane point of a coordinate.
static inline Point2D As_Point(CoordStruct const &coord)
{
    return Point2D(coord.X, coord.Y);
}

// The length of a point taken as a vector (Westwood's TPoint2D::Length).
static inline int Point_Length(Point2D const &point)
{
    return (int)unknown_sqrt_4ba410((double)point.X * (double)point.X + (double)point.Y * (double)point.Y);
}

// The distance between two points (Westwood's TPoint2D::Distance_To).
static inline int Point_Distance(Point2D const &from, Point2D const &to)
{
    return Point_Length(from - to);
}

AbstractClass *AircraftClass::unknown_418f10(AbstractClass *desired)
{
    if (desired) {
        if (Is_Strafe()) return desired;

        int range = Weapon_Range(0);
        CoordStruct center = desired->Center_Coord();
        Cell best_cell = unknown_geometry_k;
        Cell previous_record_cell = unknown_geometry_k;
        int best_distance = -1;
        int previous_record_distance = -1;
        CoordStruct destination = unknown_geometry_l;
        if (desired->AbstractFlags.IsFoot) {
            AbstractClass *next = static_cast<FootClass *>(desired)->unknown_494;
            if (next) destination = next->Center_Coord();
        }

        int radius = Aircraft_Wrapped_Subtract_00418F10(range, 256);
        if (radius > 256) {
            double center_y = center.Y;
            double center_x = center.X;
            do {
                double ring_radius = radius;
                for (int direction = 0; direction < 255; direction += 16) {
                    double angle = ((short)((unsigned char)direction << 8) - 0x3fff)
                                   * unknown_angle_scale_79b620;
                    int sample_y = (int)(center_y - unknown_sin_4ba480(angle) * ring_radius);
                    int sample_x = (int)(unknown_cos_4ba4d0(angle) * ring_radius + center_x);
                    Cell sample = XY_Cell(sample_x / 256, sample_y / 256);
                    Cell cell = sample;
                    if (Map.Unknown_0055A660(cell, true) &&
                        (unknown_game_mode_a3d298 || unknown_32c ||
                         (reinterpret_cast<UnknownCellClass *>(&Map[cell])->unknown_12c & 0x10)) &&
                        Is_Landing_Cell_Available_00419250(cell, true)) {
                        int distance;
                        if (destination != unknown_geometry_l) {
                            distance = Point_Distance(Point2D(sample_x, sample_y), Point2D(destination.X, destination.Y));
                        } else {
                            distance = Point_Distance(As_Point(Center_Coord()), Point2D(sample_x, sample_y));
                        }
                        if (best_distance == -1 || distance < best_distance) {
                            previous_record_distance = best_distance;
                            previous_record_cell = best_cell;
                            best_distance = distance;
                            best_cell = cell;
                        }
                    }
                }
                if (best_distance != -1) break;
                radius -= 256;
            } while (radius > 256);
        }
        if (previous_record_distance == -1) previous_record_cell = best_cell;
        if (best_distance != -1) {
            if (unknown_id_source_a3d290->Random()(0, 99) < 50) return &Map[best_cell];
            return &Map[previous_record_cell];
        }
    }
    return 0;
}


// Aircraft landing-cell predicate00419250. Boolean return spelling is a hypothesis.


// Aircraft landing-cell predicate00419250. Boolean return spelling is a hypothesis.
bool AircraftClass::Is_Landing_Cell_Available_00419250(Cell const &cell, bool allow_self)
{
    if (!Map.Unknown_0055A660(cell, true)) return true;
    if (Type->Spawned) {
        Point2D offset(0, 0);
        TechnoClass *occupant = (TechnoClass *)Map[cell].Cell_Techno(offset, false, 0);
        if (occupant) {
            if (occupant->SpawnManager) return true;
            if (occupant->Techno_Type_Class()->Spawned) return true;
        }
    }
    if (Type->AirportBound) return true;
    bool ignore_contact = Type->Carryall && unknown_494 && unknown_494->Kind_Of() == 1;
    CellClass *where = &Map[cell];
    for (int index = 0; index < unknown_FootClass_list_00865f38.Count(); index++) {
        FootClass *foot = unknown_FootClass_list_00865f38[index];
        if (foot && (!ignore_contact || unknown_494 != foot) &&
            (allow_self || foot != this) && !foot->unknown_76 && foot->unknown_74 &&
            (foot->unknown_slot104() == cell || foot->unknown_494 == where)) return false;
    }
    return true;
}


// int AircraftClass::Do_MISSION_ENTER() (0x004193D0, slot 138): the Enter
// mission (MissionClass slot 138 is mission 7's handler). Status +0xA8 steps
// 6 (wait for radio contact with the destination) -> 7 (closing in: nudge
// the position up to 5 leptons per frame toward the docking coordinate,
// then ask the contact (slot 149, message 21) and, as a carryall, hand the
// passenger over). Body 1248 bytes: the code plus its jump table
// (0x00419890-0x004198AF).
// The destination as a building (it may also be a cell).
static inline BuildingClass *Building_Destination(AbstractClass *target)
{
    if (!target) return 0;
    return target->Kind_Of() == 6 ? (BuildingClass *)target : 0;
}

int AircraftClass::Do_MISSION_ENTER()
{
    enum { MoveMission = 2, GuardMission = 5 };
    if (Get_Height_AGL() == 0 && Type->AirportBound && unknown_494) {
        if (unknown_494 != ((CellClass *)Get_Cell_Ptr())->Unknown_004717B0()) {
            unknown_5b4 = Building_Destination(unknown_494);
            unknown_slot149(3);
            QueueMission(2, true);
            return 1;
        }
    }
    if (Get_Height_AGL() > 0) {
        AbstractClass *target = unknown_494;
        TechnoClass *previous = unknown_5b4;
        if (previous != target && target && target->Kind_Of() == 6)
            unknown_5b4 = Building_Destination(unknown_494);
    }
    if (!unknown_494 && In_Which_Layer() != 2 && unknown_slot149(14) != 1 && !unknown_6d8080()) {
        unknown_slot261(false, true);
        return 1;
    }
    switch (unknown_a8) {
    case 0:
        unknown_a8 = 6;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        unknown_5bc = true;
        return 1;
    case 6:
        if (Type->AirportBound && Get_Height_AGL() != 0) unknown_slot149(14);
        if (!HasAnyLink()) {
            if (!unknown_6d8080()) {
                if (Locomotor.Checked()->Get_Status() == 1) {
                    unknown_a8 = 7;
                    unknown_5bc = true;
                }
                if (MissionQueue == -1) {
                    Assign_Destination(0, true);
                    unknown_slot261(false, true);
                    unknown_5bc = true;
                    return 1;
                }
                unknown_5bc = true;
            }
            unknown_5bc = true;
            return 1;
        }
        if (Locomotor.Checked()->Get_Status() == 1 || (Type->AirportBound && Get_Height_AGL() == 0)) {
            unknown_a8 = 7;
            unknown_5bc = false;
        }
        return 3;
    case 7:
        if (Locomotor.Checked()->Get_Status() == 1) {
            AbstractClass *target = unknown_494;
            if (target) {
                CoordStruct destination = target->Kind_Of() == 6
                    ? ((ObjectClass *)target)->unknown_slot41((int)this)
                    : unknown_494->Center_Coord();
                CoordStruct current = Coord;
                int dx = destination.X - current.X;
                int dy = destination.Y - current.Y;
                if (dx > 0) {
                    if (dx > 5) dx = 5;
                } else if (dx < -5) dx = -5;
                if (dy > 0) {
                    if (dy > 5) dy = 5;
                } else if (dy < -5) dy = -5;
                current.X += dx;
                current.Y += dy;
                Set_Coord(current);
            }
            unknown_5bc = true;
            return 1;
        }
        unknown_5bc = true;
        switch (unknown_slot149(21)) {
        default:
            unknown_slot261(false, true);
            return 1;
        case 5:
            Limbo();
            Contact(0)->Cargo.Attach(this);
            return 1;
        case 1:
            break;
        }
        if (Type->Carryall && Cargo.Object) {
            TechnoClass *contact = Contact(0);
            FootClass *passenger = (FootClass *)Cargo.Detach_Object();
            unknown_slot149(3);
            if (passenger->unknown_slot150(2, contact) == 1) {
                Cargo.Attach(passenger);
                Mark(0);
                unknown_416420();
                Mark(1);
            } else {
                Assign_Destination(unknown_4198b0(), true);
            }
            QueueMission(MoveMission, false);
        } else {
            QueueMission(GuardMission, false);
        }
        return 1;
    }
    return 1;
}


// AircraftClass::unknown_4198b0 0x004198B0: where to land (Good_LZ): the cell next to the nearest
// dock building (distance quartered for the first dock type), else next to the nearest own techno,
// else the aircraft's own cell.

// The distance between two coordinates through UnknownVector3 (0x0041B630/0x0041B780, out of line).
static inline int Vector_Distance(CoordStruct const &here, CoordStruct const &there)
{
    UnknownVector3 delta = UnknownVector3(here.X - there.X, here.Y - there.Y, here.Z - there.Z);
    return delta.Length();
}

// The same distance computed inline from the coordinate difference.
static inline int Coord_Distance(CoordStruct const &here, CoordStruct const &there)
{
    CoordStruct delta(here.X - there.X, here.Y - there.Y, here.Z - there.Z);
    double y = delta.Y;
    double z = delta.Z;
    double x = delta.X;
    return (int)unknown_sqrt_4ba410(x * x + z * z + y * y);
}

AbstractClass *AircraftClass::unknown_4198b0()
{
    Cell best_cell = Cell(0, 0);
    int best_distance = -1;
    int index;

    if (Type->Dock.Count() > 0) {
        int count = House->Buildings.Count();
        for (index = 0; index < count; index++) {
            BuildingClass *building = House->Buildings[index];
            if (building && !building->unknown_76) {
                int distance = Vector_Distance(Center_Coord(), building->Center_Coord());
                if (building->Type == Type->Dock[0]) {
                    distance /= 4;
                }
                if (best_distance == -1 || distance < best_distance) {
                    Cell cell = building->unknown_slot104();
                    cell = Map.Nearby_Location(cell, 0, Map.Zone(cell, 0, false), 0, false, Point2D(3, 3), 0, 0, 0, 1, Cell(0, 0), 0);
                    if (cell != unknown_geometry_k) {
                        best_distance = distance;
                        best_cell = cell;
                    }
                }
            }
        }
    }

    if (best_distance != -1) {
        if (best_distance < 256) {
            Cell cell = Map.Nearby_Location(unknown_slot104(), 0, Map.Zone(unknown_slot104(), 0, false), 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
            return &Map[cell];
        }
        return &Map[best_cell];
    }

    for (index = 0; index < unknown_objects_a40368.Count(); index++) {
        TechnoClass *techno = unknown_objects_a40368[index]->As_Techno();
        if (techno && !techno->unknown_76 && techno->House == House && techno != this) {
            int distance = Coord_Distance(Center_Coord(), techno->Center_Coord());
            if (best_distance == -1 || distance < best_distance) {
                Cell cell = techno->unknown_slot104();
                cell = Map.Nearby_Location(cell, 0, Map.Zone(cell, 0, false), 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
                if (cell != unknown_geometry_k) {
                    best_distance = distance;
                    best_cell = cell;
                }
            }
        }
    }
    if (best_distance != -1) {
        return &Map[best_cell];
    }
    return &Map[(CoordStruct)Coord];
}


// UnknownFacingDir AircraftClass::unknown_slot183() (0x00419CB0, slot 183)
// The facing at +0x2F8, as slot 162.

UnknownFacingDir AircraftClass::unknown_slot183()
{
    return unknown_2f8.Current();
}


// void AircraftClass::Scatter(CoordStruct const &source, bool forced, bool nokidding) (0x00419CD0, slot 87)
// When the current mission's control has flag +0x09, slot 261 with (0, 1);
// the arguments are not used.

void AircraftClass::Scatter(CoordStruct const &source, bool forced, bool nokidding)
{
    if (Control()->unknown_09) {
        unknown_slot261(0, 1);
    }
}


// int AircraftClass::Do_MISSION_GUARD() (0x00419D00)
// The aircraft's guard mission (hypothesis: Mission_Guard); returns the
// delay until the next call. At flight level: a team member with a +0x494
// target attacks (mission 2) and waits the mission rate; without a weapon
// it lands at its cell (slot 260, mission 2); otherwise outside a team it
// returns (slot 261). Below it: a slot-163 aircraft low on ammunition (empty,
// below half or below full by the 0x0083C4D4/0x0083C4D5 settings) not in
// radio contact heads for the nearest dock (mission 7); one reloading at a
// reload building stays. A +0x228 object starts mission 1; idle aircraft
// return 0x2D; a computer house's aircraft picks a target near its
// coordinate (slot 220, mission 1); a player's aircraft not in slot 21
// waits the mission rate; otherwise the foot's guard.

// Rearming settings (0x0083C4D4: rearm only when empty; 0x0083C4D5: rearm
// below half; hypothesis).
extern bool unknown_flag_83c4d4;
extern bool unknown_flag_83c4d5;

int AircraftClass::Do_MISSION_GUARD()
{
    int level = Type->Flight_Level();
    if (Get_Height_AGL() == level) {
        if (Team != 0) {
            if (unknown_494 != 0) {
                unknown_5bc = true;
                QueueMission(2, false);
            }
            return (int)(Control()->unknown_10 * 900.0);
        }
        if (unknown_slot231(0)->WeaponType == 0) {
            CoordStruct coord = Coord;
            Assign_Destination((AbstractClass *)&Map[coord], true);
            QueueMission(2, false);
            return 1;
        }
        if (Team == 0) {
            unknown_5bc = true;
            unknown_slot261(0, true);
        }
        return 1;
    }
    if (unknown_flag_83c4d4) {
        if (unknown_254 == 0 && unknown_slot163()) {
            goto rearm;
        }
    } else if (unknown_flag_83c4d5) {
        if (unknown_slot163() && unknown_254 < Type->Ammo / 2) {
            goto rearm;
        }
    } else {
        if (unknown_slot163() && unknown_254 < Type->Ammo) {
            goto rearm;
        }
    }
reloading:
    if (unknown_254 != -1 && unknown_254 < Type->Ammo / 2 && HasAnyLink()
        && Contact(0)->Kind_Of() == 6 && ((BuildingClass *)Contact(0))->Type->unknown_134a) {
        return 1;
    }
    if (unknown_228 != 0) {
        QueueMission(1, false);
        return 1;
    }
    if (!unknown_slot163()) {
        return 0x2D;
    }
    if (Get_Height_AGL() == 0 && !HasAnyLink()) {
        return 0x2D;
    }
    if (!House->Unknown_004F52F0()) {
        CoordStruct coord = Coord;
        AbstractClass *target = House->unknown_func_4eb360(coord);
        if (target != 0) {
            unknown_slot220(target);
            QueueMission(1, false);
        }
    }
    if (House->Unknown_004F52F0() && !In_Air()) {
        int delay = (int)(Control()->unknown_10 * 900.0);
        return delay + unknown_id_source_a3d290->Random()(0, 2);
    }
    return FootClass::Do_MISSION_GUARD();
rearm:
    if (!HasAnyLink()) {
        TechnoClass *dock = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
        if (dock != 0) {
            QueueMission(7, false);
            Assign_Destination(dock, true);
            unknown_slot220(0);
            return 1;
        }
    }
    goto reloading;
}


// int AircraftClass::Do_MISSION_GUARD_AREA() (0x0041A080, slot 130)
// At the type's flight level: slot 261 (0, 1) unless in a team, 1.
// Otherwise slot 261 when out of ammunition, slot 163 holds and there is
// no radio contact; with a target, queue mission 1; else FootClass's.

int AircraftClass::Do_MISSION_GUARD_AREA()
{
    if (Get_Height_AGL() == Type->Flight_Level()) {
        if (Team == 0) {
            unknown_slot261(0, 1);
        }
        return 1;
    }
    if (unknown_254 == 0 && unknown_slot163() && !HasAnyLink()) {
        unknown_slot261(0, 1);
    }
    if (unknown_228 != 0) {
        QueueMission(1, false);
        return 1;
    }
    return FootClass::Do_MISSION_GUARD_AREA();
}


// int AircraftClass::unknown_slot218(AbstractClass *arg1, int arg2, bool check_range) (0x0041A120, slot 218)
// TechnoClass's answer; when that is 0: 1 while +0x5B1 is set and +0xFC is
// empty, else 2 when a non-fighter faces (+0x2F8) more than 0x800 away
// from arg1, else 0.

int AircraftClass::unknown_slot218(AbstractClass *arg1, int arg2, bool check_range)
{
    int result = TechnoClass::unknown_slot218(arg1, arg2, check_range);
    if (result == 0) {
        if (unknown_5b1 && Cargo.Object == 0) {
            return 1;
        }
        UnknownFacingDir facing = unknown_2f8.Current();
        if (!Is_Fighter()) {
            short difference = (short)(facing.Raw - Direction_To(arg1).Raw);
            if (abs(0x800) < abs(difference)) {
                return 2;
            }
        }
        result = 0;
    }
    return result;
}


// Aircraft destination setter 0041A1C0: original831+1; current existing shared view.
// Complete original 831+1, vtable260, independently bounded by vtable29/41A500.
// Uses existing canonical UnknownCellClass Flags+140/unknown_e4+E4; no header overlay.
// Class/operation names remain hypotheses; original ordered calls are preserved.

void AircraftClass::Assign_Destination(AbstractClass *target, bool immediate)
{
    if (target) {
        if (target->In_Air()) {
            target = 0;
            goto set_destination;
        }
        if (target->What_Am_I() == 6 && (Get_Mission() == 7 || MissionQueue == 7)) {
            BuildingClass *building = static_cast<BuildingClass *>(target);
            if (!HasAnyLink()) {
                if (!building->Unknown_00636ED0(this)) {
                    unknown_6d6e50(target);
                    if (building->Type->unknown_1365) {
                        TechnoClass *dock = unknown_slot299(
                            (DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
                        Assign_Destination(0, true);
                        int next_mission = 2;
                        if (dock && unknown_slot150(15, dock) == 1) {
                            unknown_slot150(2, dock);
                            target = dock;
                            next_mission = 7;
                        } else {
                            target = unknown_4198b0();
                        }
                        QueueMission(next_mission, false);
                        if (unknown_slot122()) Commence();
                    }
                    if (building->Type->unknown_1349) {
                        unknown_3f8 = building;
                        target = 0;
                    }
                } else {
                    if (unknown_slot150(14, building) != 1) {
                        unknown_slot149(3);
                        if (building->Type->unknown_1349 || building->Type->unknown_134a) {
                            unknown_6d6e50(target);
                            target = 0;
                        }
                    }
                    if (!building->Type->unknown_1349 && !building->Type->unknown_134a) {
                        unknown_6d6e50(target);
                    } else if (unknown_494) {
                        unknown_6d6e50(unknown_494);
                    }
                }
            } else {
                TechnoClass *techno = target->As_Techno();
                if (techno) {
                    if (!techno->Unknown_00636ED0(this)) {
                        unknown_6d6e50(target);
                    } else if (!techno->ContainsLink(this)) {
                        if (unknown_slot149(14) == 1) return;
                        unknown_slot149(3);
                        if (building->Type->unknown_1349 || building->Type->unknown_134a) {
                            unknown_6d6e50(target);
                            target = 0;
                        }
                    }
                }
            }
        }
        UnknownCellClass *cell = reinterpret_cast<UnknownCellClass *>(Get_Cell_Ptr());
        if (!(cell->Flags & 0x100)) {
            ObjectClass *occupier = cell->unknown_e4;
            while (occupier && (occupier == this || occupier->Kind_Of() != 6)) {
                occupier = occupier->unknown_30;
            }
            if (occupier) {
                BuildingClass *building = static_cast<BuildingClass *>(occupier);
                if (building->Type->unknown_1349 || building->Type->unknown_134a) {
                    if (!Locomotor.Checked()->Is_Powered() && !House->Unknown_0051FC80()) {
                        Locomotor.Checked()->Power_On();
                    }
                    if (Contact(0) == building && target != Contact(0)) unknown_slot149(3);
                }
            }
        }
    }
set_destination:
    FootClass::Assign_Destination(target, immediate);
}


// int AircraftClass::In_Which_Layer() const (0x0041A500, slot 29)
// The locomotor's layer.

int AircraftClass::In_Which_Layer() const
{
    return Locomotor.Checked()->Layer();
}


// void AircraftClass::Look(bool arg1, bool arg2) (0x0041A530, slot 70)
// Reveals the map around the aircraft for its house: with the type's Sight
// (1 on the ground, slot 108 0) passing arg1/arg2, twice (last argument 0
// and 1). Airborne without Sight: in scenarios with flag 0x1000, with the
// rules range (+0xEC), flagging whether it flies below half the rules
// flight level.

void AircraftClass::Look(bool arg1, bool arg2)
{
    CoordStruct coord;
    int sight = Type->Sight;
    if (Get_Height_AGL() == 0) {
        sight = 1;
    }
    if (sight != 0) {
        coord = Coord;
        Map.Sight_From(coord, sight, House, arg1, arg2, 0, 1, 0);
        coord = Coord;
        Map.Sight_From(coord, sight, House, arg1, arg2, 0, 1, 1);
    } else if (unknown_id_source_a3d290->unknown_000 & 0x1000) {
        sight = Rules->unknown_ec;
        coord = Coord;
        Map.Sight_From(coord, sight, House, 0, 0, 1, Get_Height_AGL() < Rules->unknown_6dc / 2, 0);
        coord = Coord;
        Map.Sight_From(coord, sight, House, 0, 0, 1, Get_Height_AGL() < Rules->unknown_6dc / 2, 1);
    }
}


// Aircraft.cpp INI output at0041A6C0; static member spelling is a source identity hypothesis.
extern char const *unknown_aircraft_section_79b0d0;
void __fastcall AircraftClass::Write_INI_0041A6C0(CCINIClass &ini)
{
    ini.Clear(unknown_aircraft_section_79b0d0, 0);
    for (int index = 0; index < Aircraft.Count(); index++) {
        AircraftClass *aircraft = Aircraft[index];
        if (aircraft && !aircraft->unknown_76 && aircraft->unknown_7d) {
            char id[12];
            sprintf(id, "%d", index);
            char const *tag;
            if (aircraft->unknown_34) tag = (char const *)aircraft->unknown_34->Class->ID;
            else tag = "None";
            unsigned raw = *(unsigned const *)&aircraft->unknown_2e0.Current();
            unsigned direction = (((raw >> 7) + 1) >> 1) & 0xff;
            char line[128];
            sprintf(line, "%s,%s,%d,%d,%d,%d,%s,%s,%d,%d,%d,%d",
                    (char const *)aircraft->House->IniName, (char const *)aircraft->Type->ID,
                    (int)(aircraft->Health_Ratio() * 256.0),
                    aircraft->unknown_slot104().X, aircraft->unknown_slot104().Y,
                    direction, MissionClass::Name_From_Mission(aircraft->Get_Mission()), tag,
                    aircraft->unknown_118.To_Integer(), aircraft->unknown_1ac,
                    aircraft->unknown_379, aircraft->unknown_37a);
            ini.Put_String(unknown_aircraft_section_79b0d0, id, line);
        }
    }
}


// Aircraft.cpp scenario input0041A840; static member identity is a hypothesis.
extern char const *unknown_aircraft_section_79b0d0;
extern int unknown_ini_format_a40d24;
void __fastcall AircraftClass::Read_INI_0041A840(CCINIClass &ini)
{
    int count = ini.Entry_Count(unknown_aircraft_section_79b0d0);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_aircraft_section_79b0d0, index);
        char buffer[128];
        ini.Get_String(unknown_aircraft_section_79b0d0, entry, 0, buffer, sizeof(buffer));
        int house_index = HouseClass::Find_Index(strtok(buffer, ","));
        if (house_index != -1) {
            int type_index = AircraftTypeClass::From_Name(strtok(0, ","));
            if (type_index != -1) {
                HouseClass *house = Houses[house_index];
                AircraftClass *aircraft = new AircraftClass(AircraftTypes[type_index], house);
                if (aircraft) {
                    int health = atoi(strtok(0, ","));
                    Cell cell;
                    if (unknown_ini_format_a40d24 >= 4) {
                        Cell parsed;
                        parsed.X = (short)atoi(strtok(0, ","));
                        parsed.Y = (short)atoi(strtok(0, ","));
                        cell = parsed;
                    } else {
                        int packed = atoi(strtok(0, ","));
                        cell = Cell((short)(packed % 128), (short)(packed / 128));
                    }
                    CoordStruct coord = Cell_Coord(cell);
                    int direction = atoi(strtok(0, ","));
                    int mission = MissionClass::Mission_From_Name(strtok(0, ","));
                    TagTypeClass *tag_type = TagTypeClass::From_Name(strtok(0, ","));
                    if (tag_type) {
                        TagClass *tag = TagClass::Find_Or_Make(tag_type);
                        if (tag) aircraft->Attach_Tag(tag);
                    }
                    char *token = strtok(0, ",");
                    if (token) aircraft->unknown_118.From_Integer(atoi(token));
                    token = strtok(0, ",");
                    if (token) aircraft->unknown_1ac = atoi(token);
                    token = strtok(0, ",");
                    if (token) aircraft->unknown_379 = atoi(token) != 0;
                    token = strtok(0, ",");
                    if (token) aircraft->unknown_37a = atoi(token) != 0;
                    if (aircraft->Unlimbo(&coord, direction)) {
                        AircraftTypeClass *type = aircraft->Type;
                        aircraft->Strength = (int)((double)health * (1.0 / 256.0) * type->MaxStrength);
                        if (aircraft->Strength > type->MaxStrength - 3) aircraft->Strength = type->MaxStrength;
                        if (aircraft->Strength == 0) aircraft->Strength = 1;
                        aircraft->unknown_70 = aircraft->Strength;
                        if (unknown_game_mode_a3d298 == 0 || aircraft->House->Unknown_004F52F0()) {
                            aircraft->QueueMission(mission, false);
                            aircraft->Commence();
                        } else aircraft->unknown_slot261(false, true);
                    } else {
                        delete aircraft;
                    }
                }
            }
        }
    }
}


// HRESULT __stdcall AircraftClass::Load(IStream *stream) (0x0041AB80, slot 5)
// Leaves the techno index, loads the foot, rebuilds itself in place while
// keeping the locomotor, rejoins the index and fixes up pointers.

HRESULT __stdcall AircraftClass::Load(IStream *stream)
{
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    HRESULT result = FootClass::Load(stream);
    if (SUCCEEDED(result)) {
        ILocomotion *locomotor = Locomotor;
        new (this) AircraftClass(NoInitClass());
        Locomotor = locomotor;
        locomotor->Release();
        unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
        Swizzle_Pointer(Type);
        Swizzle_Pointer(unknown_5b4);
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall AircraftClass::Save(IStream *stream, BOOL clear_dirty) (0x0041AD10, slot 6)

HRESULT __stdcall AircraftClass::Save(IStream *stream, BOOL clear_dirty)
{
    return FootClass::Save(stream, clear_dirty);
}


// bool AircraftClass::unknown_slot122() (0x0041AD30, slot 122)
// False on missions 6 and 21 or while locked (+0x5BA); otherwise +0x5BC.

bool AircraftClass::unknown_slot122()
{
    if (Mission == 6 || Mission == 21 || unknown_5ba) {
        return false;
    }
    return unknown_5bc ? true : false;
}


// void AircraftClass::Compute_CRC(WWCRCEngine &crc) const (0x0041AD60, slot 13)

void AircraftClass::Compute_CRC(WWCRCEngine &crc) const
{
    FootClass::Compute_CRC(crc);
    crc(unknown_5b1);
    crc(unknown_5b8);
    crc(unknown_5b9);
    crc((long)unknown_5bb);
}


// void AircraftClass::Detach(AbstractClass *target, bool all) (0x0041ADB0, slot 10)
// FootClass::Detach, then forgets target as +0x5B4 or as the type.

void AircraftClass::Detach(AbstractClass *target, bool all)
{
    FootClass::Detach(target, all);
    if (unknown_5b4 == target) {
        unknown_5b4 = 0;
    }
    if (Type == target) {
        Type = 0;
    }
}


// int __stdcall AircraftClass::Landing_Altitude() (0x0041ADF0, IFlyControl slot 3)
// 100 for a carryall in radio contact or carrying (+0xFC), else 0; but 0
// while a free carryall on mission 7 is in contact with a building whose
// type has flag +0x1349 or +0x1365.

int __stdcall AircraftClass::Landing_Altitude()
{
    if (Type->Carryall && Cargo.Object == 0 && HasAnyLink()) {
        TechnoClass *contact = Contact_With_Whom(0);
        if (Get_Mission() == 7 && contact != 0 && contact->Kind_Of() == 6) {
            BuildingTypeClass *type = ((BuildingClass *)contact)->Type;
            if (type->unknown_1349 || type->unknown_1365) {
                return 0;
            }
        }
    }
    if ((HasAnyLink() && Type->Carryall) || (Cargo.Object != 0 && Type->Carryall)) {
        return 100;
    }
    return 0;
}


// int __stdcall AircraftClass::Landing_Direction() (0x0041AEB0, IFlyControl slot 4)
// One of eight directions: the radio contact's facing (+0x2E0), else this
// aircraft's +0x2F8 facing when +0xFC is set, else the rules default.

int __stdcall AircraftClass::Landing_Direction()
{
    if (HasAnyLink()) {
        TechnoClass *contact = Contact_With_Whom(0);
        return Dir_To_8(contact->unknown_2e0.Current());
    }
    if (Cargo.Object != 0) {
        return Dir_To_8(unknown_2f8.Current());
    }
    return Rules->unknown_50;
}


// long __stdcall AircraftClass::Is_Loaded() (0x0041AF20, IFlyControl slot 5)
// Whether +0xFC is set.

long __stdcall AircraftClass::Is_Loaded()
{
    return (bool)(Cargo.Object != 0);
}


// long __stdcall AircraftClass::Is_Strafe() (0x0041AF40, IFlyControl slot 6)
// Whether the primary weapon's projectile is visible and turns at most 1.

long __stdcall AircraftClass::Is_Strafe()
{
    WeaponStruct *weapon = unknown_slot231(0);
    if (weapon == 0) {
        return 0;
    }
    WeaponTypeClass *weapon_type = weapon->WeaponType;
    if (weapon_type == 0) {
        return 0;
    }
    BulletTypeClass *projectile = weapon_type->Projectile;
    if (projectile->ROT > 1) {
        return 0;
    }
    return !projectile->Inviso;
}


// long __stdcall AircraftClass::Is_Fighter() (0x0041AF90, IFlyControl slot 7)

long __stdcall AircraftClass::Is_Fighter()
{
    return Type->Fighter != 0;
}


// long __stdcall AircraftClass::Is_Locked() (0x0041AFB0, IFlyControl slot 8)

long __stdcall AircraftClass::Is_Locked()
{
    return unknown_5ba;
}


// bool AircraftClass::Commence() (0x0041AFC0, slot 117)
// Unlocks (+0x5BA), then commences the queued mission.

bool AircraftClass::Commence()
{
    unknown_5ba = false;
    return MissionClass::Commence();
}


// bool AircraftClass::unknown_slot282() (0x0041AFD0, slot 282)

bool AircraftClass::unknown_slot282()
{
    if (unknown_228 == 0 && unknown_32d && Get_Mission() != 26 && Get_Mission() != 27) {
        if ((Team == 0 && unknown_32c) || (Team != 0 && Team->Unknown_006BA990())) {
            return true;
        }
    }
    return false;
}


// void AircraftClass::unknown_slot207() (0x0041B030, slot 207)
// Does nothing (TechnoClass's slot 207 does something for other technos).

void AircraftClass::unknown_slot207()
{
}


// bool AircraftClass::unknown_slot31() (0x0041B040, slot 31)
// Answers slot 20 (a tail call through the vtable).

bool AircraftClass::unknown_slot31()
{
    return On_Ground();
}


// bool AircraftClass::In_Air() const (0x0041B050, slot 21)
// For the two rules aircraft types (+0x434, +0x464): whether the locomotor
// is moving; otherwise FootClass's answer.

bool AircraftClass::In_Air() const
{
    if (Type == Rules->V3Rocket.Type || Type == Rules->DMisl.Type) {
        return Locomotor.Checked()->Is_Moving_Now() != 0;
    }
    return FootClass::In_Air();
}


// bool AircraftClass::On_Ground() const (0x0041B0B0, slot 20)
// For the two rules aircraft types (+0x434, +0x464): whether the locomotor
// is not moving; otherwise ObjectClass's answer.

bool AircraftClass::On_Ground() const
{
    if (Type == Rules->V3Rocket.Type || Type == Rules->DMisl.Type) {
        return !Locomotor.Checked()->Is_Moving_Now();
    }
    return FootClass::On_Ground();
}


// bool AircraftClass::unknown_slot242() (0x0041B110, slot 242)

bool AircraftClass::unknown_slot242()
{
    return unknown_5bd;
}


// void AircraftClass::Set_Mission(int mission) (0x0041B120, slot 118)
// Sets the mission, except that missions 4, 26 and 27 are only replaced by
// one another.

void AircraftClass::Set_Mission(int mission)
{
    if ((Mission == 4 || (Mission > 25 && Mission <= 27)) && mission != 4 && (mission <= 25 || mission > 27)) {
        return;
    }
    MissionClass::Set_Mission(mission);
}


// void AircraftClass::QueueMission(int mission, bool commence) (0x0041B160, slot 116)
// Queues the mission, except that missions 4, 26 and 27 are only replaced
// by one another.

void AircraftClass::QueueMission(int mission, bool commence)
{
    if ((Mission == 4 || (Mission > 25 && Mission <= 27)) && mission != 4 && (mission <= 25 || mission > 27)) {
        return;
    }
    MissionClass::QueueMission(mission, commence);
}


// void AircraftClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination) (0x0041B1A0, slot 119)
// FootClass's override of the mission, except that missions 4, 26 and 27
// are only replaced by one another.

void AircraftClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination)
{
    if ((Mission == 4 || (Mission > 25 && Mission <= 27)) && mission != 4 && (mission <= 25 || mission > 27)) {
        return;
    }
    FootClass::Override_Mission(mission, target, destination);
}


// TechnoClass *AircraftClass::unknown_slot299(DynamicVectorClass<TechnoTypeClass *> const *types, int arg2, int arg3) (0x0041B1E0, slot 299)
// For airport-bound types keeps +0x5B4 while slot 150 (+0x258) answers 1
// for it with 15; otherwise replaces it with FootClass's answer for
// (types, 0, arg3).

TechnoClass *AircraftClass::unknown_slot299(DynamicVectorClass<TechnoTypeClass *> const *types, int arg2, int arg3)
{
    if (Type->AirportBound && unknown_5b4 != 0) {
        if (unknown_slot150(15, unknown_5b4) != 1) {
            unknown_5b4 = 0;
        } else {
            return unknown_5b4;
        }
    }
    unknown_5b4 = FootClass::unknown_slot299(types, 0, arg3);
    return unknown_5b4;
}


CoordStruct ObjectClass::unknown_slot40()
{
    return Center_Coord();
}


CoordStruct ObjectClass::unknown_slot42() const
{
    return Center_Coord();
}


CoordStruct ObjectClass::unknown_slot44(int arg1)
{
    return Center_Coord();
}


Cell ObjectClass::unknown_slot104() const
{
    Cell cell;
    cell.X = Coord.X / 256;
    cell.Y = Coord.Y / 256;
    return cell;
}


class Leaf_41b2d0 { public: unsigned int get(unsigned int unused_argument_1); };
unsigned int Leaf_41b2d0::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; return zero_value; }


class Leaf_41b2e0 { public: unsigned int get(unsigned int unused_argument_1); };
unsigned int Leaf_41b2e0::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; return zero_value; }


class Leaf_41b2f0 { public: void get(unsigned int unused_argument_1); };
void Leaf_41b2f0::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


class Leaf_41b300 { public: bool get(); };
bool Leaf_41b300::get() { return false; }


class Leaf_41b310 { public: void get(unsigned int unused_argument_1); };
void Leaf_41b310::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


// 0x0041b360: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 5 sites (data).
// Ghidra prototype: undefined1 FUN_0041b360(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0041b360 {

unsigned char  FUN_0041b360(void)
{
    return 0;
}

}  // namespace seed_0041b360


// TechnoClass::unknown_slot159 (0x0041B370, TechnoClass slot 159): the inline
// member defined in shared/techno.hpp, emitted in this unit with the vtables
// that reference it; no source of its own.


class Leaf_41b380 { public: bool get(); };
bool Leaf_41b380::get() { return false; }


// TechnoClass::unknown_slot203 (0x0041B390, TechnoClass slot 203): the inline
// member defined in shared/techno.hpp, emitted in this unit with the vtables
// that reference it; no source of its own.


class Leaf_41b3d0 { public: bool get(); };
bool Leaf_41b3d0::get() { return false; }


class Leaf_41b3e0 { public: void get(); };
void Leaf_41b3e0::get() {  }


class Leaf_41b400 { public: void get(); };
void Leaf_41b400::get() {  }


class Leaf_41b410 { public: unsigned char get(); };
unsigned char Leaf_41b410::get() { return *(unsigned char*)((char*)this + 0x170); }


// TechnoClass::unknown_slot49 (0x0041B420, TechnoClass slot 49): the inline
// member defined in shared/techno.hpp, emitted in this unit with the vtables
// that reference it; no source of its own.


class Leaf_41b430 { public: void get(); };
void Leaf_41b430::get() { *(unsigned char*)((char*)this + 0x170) = 0; }


class Leaf_41b440 { public: bool get(); };
bool Leaf_41b440::get() { return false; }


// 0x0041b450: function not yet identified
//
// Body: 14 bytes (frozen inventory bound).
// Referenced from 4 sites (data).
// Ghidra prototype: bool __fastcall FUN_0041b450(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0041b450 {

bool __fastcall FUN_0041b450(int aircraft_address)
{
    return *(int *)(aircraft_address + 0x4c4) != 0;
}

}  // namespace seed_0041b450


class Leaf_41b460 { public: bool get(); };
bool Leaf_41b460::get() { return false; }


class Leaf_41b470 { public: unsigned char get(); };
unsigned char Leaf_41b470::get() { return *(unsigned char*)((char*)this + 0x5a0); }


class Leaf_41b480 { public: bool get(); };
bool Leaf_41b480::get() { return false; }


// FootClass::unknown_slot293 (0x0041B490, FootClass slot 293): the inline
// member defined in shared/foot.hpp, emitted in this unit with the vtables
// that reference it; no source of its own.


class Leaf_41b4f0 { public: void get(); };
void Leaf_41b4f0::get() {  }


class Leaf_41b500 { public: void get(); };
void Leaf_41b500::get() {  }


class Leaf_41b510 { public: void get(); };
void Leaf_41b510::get() {  }


class Leaf_41b520 { public: void get(unsigned int unused_argument_1); };
void Leaf_41b520::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


class Leaf_41b530 { public: void get(unsigned int unused_argument_1, unsigned int unused_argument_2); };
void Leaf_41b530::get(unsigned int unused_argument_1, unsigned int unused_argument_2) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


class Leaf_41b540 { public: void get(unsigned int unused_argument_1); };
void Leaf_41b540::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


// FootClass::unknown_slot178 (0x0041B550, FootClass slot 178): the inline
// member defined in shared/foot.hpp, emitted in this unit with the vtables
// that reference it; no source of its own.


class Leaf_41b560 { public: void get(unsigned int unknown_0x458_value); };
void Leaf_41b560::get(unsigned int unknown_0x458_value) { unsigned int object_address = (unsigned int)this; unsigned int unknown_0x458; unsigned int unused_value; unknown_0x458 = unknown_0x458_value; *(unsigned int*)(object_address + 0x458) = unknown_0x458; }


// int AircraftClass::Size_Of() const (0x0041B570, slot 12): inline in shared/aircraft.hpp, emitted with the aircraft's vtable; no source of its own.


// int AircraftClass::Kind_Of() const (0x0041B580, slot 11): inline in shared/aircraft.hpp, emitted with the aircraft's vtable; no source of its own.


// HRESULT __stdcall AircraftClass::GetClassID(GUID *clsid) (0x0041B590, slot 3): inline in shared/aircraft.hpp, emitted with the aircraft's vtable; no source of its own.


// wchar_t const *AircraftClass::unknown_slot35() (0x0041B5D0, slot 35)
// The type's UIName.

wchar_t const *AircraftClass::unknown_slot35()
{
    return Type->UIName;
}


// UnknownFacingDir AircraftClass::unknown_slot162() (0x0041B5E0, slot 162)
// The facing at +0x2F8 (TechnoClass's slot uses +0x2E0).

UnknownFacingDir AircraftClass::unknown_slot162()
{
    return unknown_2f8.Current();
}


// ObjectTypeClass *AircraftClass::Class_Of() const (0x0041B600, slot 33)

ObjectTypeClass *AircraftClass::Class_Of() const
{
    return Type;
}


// void *AircraftClass::`scalar deleting destructor'(unsigned int flags) (0x0041B610, slot 8)
// Compiler-generated with the aircraft's vtable; it calls the destructor
// out of line. No source of its own.


// Existing UnknownVector3 coordinate-difference constructor (0041B630).
// Never inlined anywhere in the program (about 200 call sites to 0x0041B630 and 45 to 0x0041B780
// across many units); this unit is the only one that sees the definition, so auto-inlining is off.
#pragma auto_inline(off)
UnknownVector3::UnknownVector3(int x, int y, int z) : X(x), Y(y), Z(z) {}
#pragma auto_inline(on)


// Library-derived COM support template, original 0041B650, 136+8.
// Defined in the canonical header; naturally emitted by Aircraft unknown_416420.
// No explicit instantiation, anchor, or duplicate implementation.


// Original local-double-vector XY length0041B750; free/member identity is a hypothesis.
double __fastcall Aircraft_XY_Length_0041B750(AircraftVectorXY const *vector)
{
    double y = vector->Y;
    double x = vector->X;
    return unknown_sqrt_4ba410(x * x + y * y);
}


// Existing UnknownVector3 Euclidean length (0041B780), double intermediates then signed FTOL.
// Never inlined anywhere in the program (about 200 call sites to 0x0041B630 and 45 to 0x0041B780
// across many units); this unit is the only one that sees the definition, so auto-inlining is off.
#pragma auto_inline(off)
int UnknownVector3::Length() const
{
    double z = Z;
    double y = Y;
    double x = X;
    return (int)unknown_sqrt_4ba410(x * x + y * y + z * z);
}
#pragma auto_inline(on)


// Original Aircraft.cpp velocity helper; free __fastcall versus member spelling is an ABI hypothesis.
// Caller415790 passes the current TVelocity3D<double> at bullet+0xD0, same ECX receiver.
extern double const Aircraft_Velocity_Zero_0079b610;
double __fastcall Velocity_Length3_0041B7C0(TVelocity3D<double> const *velocity)
{
    double z = velocity->Z;
    double y = velocity->Y;
    double x = velocity->X;
    return unknown_sqrt_4ba410(x * x + y * y + z * z);
}


// Original Aircraft.cpp velocity helper; free __fastcall versus member spelling is an ABI hypothesis.
// Caller415790 passes the current TVelocity3D<double> at bullet+0xD0, same ECX receiver.
extern double const Aircraft_Velocity_Zero_0079b610;
void __fastcall Ensure_Velocity3_0041B7F0(TVelocity3D<double> *velocity)
{
    if (velocity->X == Aircraft_Velocity_Zero_0079b610 && velocity->Y == Aircraft_Velocity_Zero_0079b610 && velocity->Z == Aircraft_Velocity_Zero_0079b610) velocity->X = 100.0;
}


// Original Aircraft.cpp velocity helper; free __fastcall versus member spelling is an ABI hypothesis.
// Caller415790 passes the current TVelocity3D<double> at bullet+0xD0, same ECX receiver.
extern double const Aircraft_Velocity_Zero_0079b610;
double __fastcall Velocity_LengthXY_0041B830(TVelocity3D<double> const *velocity)
{
    double y = velocity->Y;
    double x = velocity->X;
    return unknown_sqrt_4ba410(x * x + y * y);
}


// Original Aircraft.cpp velocity helper; free __fastcall versus member spelling is an ABI hypothesis.
// Caller415790 passes the current TVelocity3D<double> at bullet+0xD0, same ECX receiver.
extern double const Aircraft_Velocity_Zero_0079b610;
void __fastcall Ensure_VelocityXY_0041B860(TVelocity3D<double> *velocity)
{
    if (velocity->X == Aircraft_Velocity_Zero_0079b610 && velocity->Y == Aircraft_Velocity_Zero_0079b610) velocity->X = 100.0;
}


// IndexClass<long, AbstractClass *>::Increase_Table_Size (0x0041B900), the
// object index's (shared/object_index.hpp). Template member defined in
// shared/ini_class.hpp; this unit naturally emits the instance through its
// existing index callers. No source of its own.


// IndexClass<long, AbstractClass *>::Search_For_Node (0x0041B990), the object
// index's (shared/object_index.hpp). Template member defined in
// shared/ini_class.hpp; this unit naturally emits the instance through its
// existing index callers. No source of its own.


// IndexClass<long, AbstractClass *>::search_compfunc (0x0041BA10), the object
// index's (shared/object_index.hpp). Template member defined in
// shared/ini_class.hpp; this unit keeps the instance. No source of its own.


// AircraftClass::QueryInterface adjustor thunk for IRTTITypeInfo (this - 4) (0x0041ba40)
// Compiler generated with the aircraft's vtables; no source of its own.


// AircraftClass::AddRef adjustor thunk for IRTTITypeInfo (this - 4) (0x0041ba50)
// Compiler generated with the aircraft's vtables; no source of its own.


// AircraftClass::Release adjustor thunk for IRTTITypeInfo (this - 4) (0x0041ba60)
// Compiler generated with the aircraft's vtables; no source of its own.
