// unit.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/unit.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/unit.hpp"
#include "shared/locomotion.hpp"
#include "shared/house.hpp"
#include "shared/anim.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/building_type.hpp"
#include "shared/team.hpp"
#include "shared/weapon_type.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/unit_type.hpp"
#include "shared/infantry.hpp"
#include "shared/infantry_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/tag.hpp"
#include "shared/random.hpp"
#include "shared/vox_class.hpp"
#include "shared/radar_event.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/id_source.hpp"
#include "shared/tiberium.hpp"
#include "shared/sound_play.hpp"
#include "shared/adjacent_cell.hpp"
#include "shared/building_type_class.hpp"
#include "shared/overlay_type.hpp"
#include "shared/text_print.hpp"
#include "shared/blit.hpp"
#include "shared/matrix3d.hpp"
#include "shared/surface.hpp"
#include "shared/bullet_type.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect // Unit.cpp keeps no empty rectangle of its own
#include "shared/palette.hpp"
#include "shared/tactical.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/color_scheme.hpp"
#include "shared/fast_math.hpp"
#include "shared/building_light.hpp"


// Header-static dynamic initializers of the unit at 0x006FA1E0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006FA1E0-0x006FA3C6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006FA210, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA230, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA250, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA270, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA290, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA2B0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA2F0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA320, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA350, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA380, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA390, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Dynamic initializer (0x006FA3B0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006fa1e0.cpp; no source of its own.


// Unit.cpp's drawing rectangle (0x00AD0490): a copy of the header-static
// unknown_static_rect made by its dynamic initializer (0x006FA3D0, _$E), the
// unit's first own initializer after the header statics. UnitClass slot 312
// (0x006FF3E0) offsets it by the draw position. Name is a hypothesis.

Rect unknown_unit_rect_ad0490 = unknown_static_rect;


// UnitClass::UnitClass(UnitTypeClass *type, HouseClass *house) (0x006FA400)
// Default state for a type and owner: joins Units, takes the body facing for
// the turret, creates and links the type's locomotor, takes the type's turn
// rate, ammunition and strength, registers with the owner, becomes a veteran
// when the owner's type or training says so, and joins the techno index.

UnitClass::UnitClass(UnitTypeClass *type, HouseClass *house) :
    FootClass(house),
    unknown_5a8(-1),
    Type(type),
    unknown_5b0(0),
    unknown_5b4(-1),
    unknown_5b8(false),
    unknown_5b9(false),
    unknown_5ba(false),
    unknown_5bb(false),
    unknown_5bc(-1),
    unknown_5c0(-1),
    unknown_5c4(0),
    unknown_5c8(false),
    unknown_5cc(0)
{
    Create_ID();
    Units.Add(this);
    unknown_2f8.Set(unknown_2e0.Current());
    if (Type) {
        // Interface* assignment: this unit's Assign_Destination 0x007053F0 specializes the ILocomotion copy assignment.
        Locomotor = (ILocomotion *)UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>(Type->Locomotor);
        Locomotor.Checked()->Link_To_Object(this);
    }
    if (Type) {
        unknown_2e0.Set_ROT(Type->ROT);
        unknown_2f8.Set_ROT(Type->ROT);
        int ammo = Type->InitialAmmo;
        if (ammo != -1) {
            unknown_254 = ammo;
        } else {
            unknown_254 = Type->Ammo;
        }
        unknown_32a = Type->Cloakable;
        unknown_70 = Strength = Type->MaxStrength;
    }
    unknown_194 = CDTimerClass<FrameTimerClass>(0);
    if (House) {
        Unit_House_View(House)->unknown_4ea970(this);
    }
    TechnoClass::Init();
    if (Type) {
        if (House) {
            DynamicVectorClass<UnitTypeClass *> &veterans = Unit_House_View(House)->unknown_34->unknown_168;
            for (int index = 0; index < veterans.Count(); index++) {
                if (veterans[index] == Type) {
                    unknown_118.Set_Veteran(true);
                    break;
                }
            }
        }
        if (House && Unit_House_View(House)->unknown_207 && !Type->Naval && Type->Trainable) {
            unknown_118.Set_Veteran(true);
        }
        if (Type->Gunner) {
            unknown_6d83f0(0);
        }
    }
    unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
}


// UnitClass::~UnitClass() (0x006FA7C0)
// Releases the object at +0x5C4. While the game runs (0x00A40958) and the type
// is set: tells the house (flag +0x144 when it has none of the type left),
// leaves the team and the house, destroys the passengers (unless 0x00A40774),
// and takes off the map (FootClass slot 52; a recorded house hears of the
// cell). Always leaves the unit list and the techno index.

UnitClass::~UnitClass()
{
    if (unknown_5c4) {
        unknown_5c4->unknown_4b25c0();
        unknown_5c4 = 0;
    }
    if (unknown_flag_a40958 && Type != 0) {
        if (House->Can_Build(Type, 0, 0) == -1) {
            House->unknown_144 = true;
        }
        if (Team != 0) {
            Team->Remove(this, -1, 0);
            Team = 0;
        }
        House->unknown_4ea800(this);
        if (unknown_a40774 == 0) {
            while (Cargo.Object != 0) {
                ObjectClass *passenger = Cargo.Detach_Object();
                if (passenger != 0) {
                    delete passenger;
                }
            }
        }
        if (FootClass::Limbo() && unknown_5b4 != -1) {
            ObjectClass *object = this;   // (the call below stays virtual, as in the original)
            Houses[unknown_5b4]->Flag_Attach(object->unknown_slot104(), 0);
            unknown_5b4 = -1;
        }
    }
    Unknown_006ECB90(this, true);
    Units.Delete(this);
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    unknown_7d = false;
}


// Unit AI: damage/death, deployment, falling, repair and refinery navigation.
// The original caller passes House in ECX and this unit in EDX; source spelling is provisional.
BuildingClass *__fastcall Unknown_00450F90(HouseClass *house, UnitClass *unit);
inline CoordStruct Unit_AI_Smoke_Position(CoordStruct const &position)
{
    CoordStruct smoke = position;
    smoke.X += 120;
    smoke.Y += 120;
    return smoke;
}
inline Cell Unit_AI_Refinery_Exit_Cell(Cell const &position)
{
    return Cell((short)(position.X + 3), (short)(position.Y + 1));
}
void UnitClass::AI()
{
    if (unknown_20c) unknown_20c->AI();
    if ((unknown_slot111() || unknown_slot112()) && Frame % 24 == 0 && Rules->unknown_2a8) {
        new AnimClass(Rules->unknown_2a8, Unit_AI_Smoke_Position(Coord), 0, 1, 0x600, 0);
    }
    if (unknown_slot112() || (unknown_slot111() && unknown_210)) {
        Locomotor.Checked()->Process();
        if (!unknown_7d) return;
    }
    if ((unknown_slot112() && unknown_20c) || unknown_slot111()) {
        if (unknown_228) unknown_slot220(0);
        if (unknown_494) {
            Assign_Destination(0, true);
            return;
        }
        return;
    }
    if (unknown_5c0 != -1) {
        if (unknown_5c0++ >= Type->MaxDeathCounter) {
            Explode();
            Mark(0);
            Delete_Me();
            return;
        }
    }
    if (unknown_574 >= 0) {
        Unknown_006FAA30();
        unknown_slot267(false);
        return;
    }
    int countdown = unknown_5a8 - 1;
    if (countdown < -1) countdown = -1;
    unknown_5a8 = countdown;
    if (Type->DeploysInto) {
        for (int index = 0; index < Rules->unknown_7c8.Count(); index++) {
            if (Rules->unknown_7c8[index] == Type->DeploysInto) {
                if (House->unknown_13b && !House->Is_Human_Player() && unknown_game_mode_a3d298 != 0 && House->unknown_50.Count() == 0 && Mission != 15 && Mission != 16) {
                    QueueMission(15, false);
                }
                break;
            }
        }
    }
    if (unknown_slot122()) Commence();
    FootClass::AI();
    if (Type->CanDisguise && !Type->PermaDisguise) UpdateDisguise();
    if (unknown_325) {
        CoordStruct position = Coord;
        CoordStruct lowered(position.X, position.Y, position.Z - 5);
        Set_Coord(lowered);
        if (Get_Height_AGL() < -400) {
            Record_The_Kill(0);
            Delete_Me();
            return;
        }
        if ((Frame & 3) == 0) {
            int y_offset = unknown_id_source_a3d290->Random()(-170, 170);
            int x_offset = unknown_id_source_a3d290->Random()(-170, 170);
            int height = Get_Height_AGL();
            CoordStruct offset(x_offset, y_offset, -height);
            CoordStruct origin = Coord;
            offset.X = origin.X + offset.X;
            offset.Y += origin.Y;
            offset.Z += origin.Z;
            position = offset;
            new AnimClass(Rules->unknown_09c, position, 0, 1, 0x600, 0);
        }
    }
    if (unknown_7d) {
        if (Get_Mission() != 10) unknown_5ba = false;
        unknown_6fbc90();
        unknown_6fb890();
        if (Get_Mission() == 5) {
            CoordStruct position = Coord;
            if (!Map.Unknown_0054AB00(position) && unknown_32d) {
                if (Team) Team->TargetNotAssigned = true;
                unknown_slot210();
                Delete_Me();
                return;
            }
        }
        if (Type->SmallVisceroid || Type->LargeVisceroid) unknown_6fbf40();
        unknown_slot239();
        if (Type->Passengers > 0 && !unknown_2a8.Is_Count_Down_Complete() && Get_Mission() != 16 && unknown_slot149(17) != 1) {
            unknown_2a8.Count_Down(Type->DeployTime);
        }
        if (unknown_slot122()) Commence();
        if (unknown_1b8 == 2 && unknown_5b4 != -1) Do_Shimmer();
        if ((Frame & 15) == 0 && (Mission == 11 || Mission == 5) && !House->Is_Human_Player() && MissionQueue == -1 && Strength < Type->MaxStrength && (!unknown_1b0 || Mission == 11) && !Type->Harvester && !Type->Weeder) {
            BuildingClass *repair_building = Unknown_00450F90(House, this);
            if (repair_building) {
                if (!unknown_1b0) {
                    CoordStruct position = Coord;
                    unknown_6d6e50(&Map[position]);
                }
                QueueMission(7, false);
                unknown_slot220(0);
                Assign_Destination(repair_building, true);
            }
        }
        if (Type->Harvester && !House->Is_Human_Player() && Frame - unknown_ac > 300 && !Locomotor.Checked()->Is_Moving()) {
            BuildingClass *refinery = (BuildingClass *)Map[CoordStruct(Coord)].Cell_Building();
            if (refinery && refinery->Type->Refinery) {
                if (unknown_slot104() == Unit_AI_Refinery_Exit_Cell(refinery->unknown_slot104())) {
                    ObjectClass *contact = Map[CoordStruct(Coord)].Cell_Building();
                    if (HasAnyLink()) unknown_slot149(3);
                    Assign_Destination(&Map[unknown_6cf130(contact)], true);
                    QueueMission(2, true);
                }
            }
        }
    }
}


// Unit body/turret turning using current shared Facing and weapon layouts.
void UnitClass::unknown_6fb890()
{
    if (unknown_228 && !unknown_599) {
        UnknownFacingDir toward = Direction_To(unknown_228);
        if (Type->Turret) {
            WeaponStruct *weapon = unknown_slot230();
            if (Unknown_006D8640(weapon) && !weapon->WeaponType->unknown_107) {
                if (weapon->TurretLocked) {
                    unknown_2f8.Set_Desired(unknown_2e0.Current());
                } else {
                    unknown_2f8.Set_Desired(toward);
                }
            }
        } else if (Type->SpeedType == 1 && !unknown_494
            && !Locomotor.Checked()->Is_Moving()
            && unknown_2e0.Current().Raw == toward.Raw) {
            unknown_2e0.Set_Desired(toward);
        }
    }
    if (Type->TurretSpins) {
        UnknownFacingDir direction((unsigned short)((Dir_To_256(unknown_2f8.Current()) + 8) << 8));
        unknown_2f8.Set(direction);
    } else {
        unknown_599 = false;
        if (Type->Turret) {
            if (unknown_2f8.Is_Rotating()) {
                if (!Type->TurretSpins) {
                    unknown_599 = unknown_2f8.Is_Rotating();
                }
            } else if (!unknown_228) {
                WeaponStruct *weapon = unknown_slot230();
                bool locked = Unknown_006D8640(weapon) && weapon->TurretLocked;
                if (unknown_494 && !locked) {
                    unknown_2f8.Set_Desired(Direction_To(unknown_494));
                } else {
                    unknown_2f8.Set_Desired(unknown_2e0.Current());
                }
            }
        }
    }
    if (Type->Turret) {
        unknown_3f0 = unknown_2f8.Is_Rotating();
    }
}


// Checks mission 5 and the coordinate test before notifying the team and slots210/60.
bool UnitClass::Edge_Of_World_AI()
{
    if (Get_Mission() == 5) {
        CoordStruct position = Coord;
        if (!Map.Unknown_0054AB00(position) && unknown_32d) {
            if (Team) {
                Team->TargetNotAssigned = true;
            }
            unknown_slot210();
            Delete_Me();
            return true;
        }
    }
    return false;
}

// Not exact (RA2_NON_MATCHING): UnitClass::Unknown_006FAA30 at 0x006FAA30, best score 50.39, 2026-11-13; differs: 522 instructions in the original, 502 in the draft (shape 88.09)
#ifdef RA2_NON_MATCHING
// void UnitClass::Unknown_006FAA30() (0x006FAA30)
// TibSun UnitClass::Tunnel_AI: walks the unit along the tube it entered, then hands it back to the
// map at the far mouth, or waits underground and shoos idle occupiers off the exit cell.
#include "shared/unknown_vector3.hpp"
TubeClass *__fastcall Cell_Tube_0047a130(CellClass const *cell);
extern double const unknown_tube_speed_79d568;   // 0x0079D568
extern double const unknown_diagonal_lepton_7a3a20;   // 0x007A3A20
extern double const unknown_lepton_79a700;   // 0x0079A700
extern double const unknown_facing_radians_79b620;
#include "shared/rect.hpp"
extern Point2D const unknown_adjacent_offsets_00851ac0[8];   // 0x00851AC0
inline CoordStruct Adjacent_Coord_006faa30(CoordStruct const &coord, int dir)
{
    return coord + unknown_adjacent_offsets_00851ac0[dir & 7];
}
// Hypotheses: the out-of-line length (0x0041B780) reads the coordinate difference as UnknownVector3.
inline int Distance_006faa30(CoordStruct const &a, CoordStruct const &b)
{
    return (a - b).Length();
}
inline CoordStruct Cell_Center_006faa30(Cell cell, int z)
{
    return CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, z);
}
inline double Radian_006faa30(UnknownFacingDir const &dir)
{
    return ((short)dir.Raw - 0x3FFF) * unknown_facing_radians_79b620;
}
inline CoordStruct Move_Coord_006faa30(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
    double radians = Radian_006faa30(dir);
    return CoordStruct((int)(start.X + Fast_Cos(radians) * distance), (int)(start.Y - Fast_Sin(radians) * distance), start.Z);
}

void UnitClass::Unknown_006FAA30()
{
    CoordStruct coord;
    bool finished = unknown_TubeClass_list_008661c8[unknown_574]->Directions[unknown_575] == -1;
    if (!finished) {
        int distance = Distance_006faa30(Center_Coord(), unknown_45c);
        TubeClass *tube = unknown_TubeClass_list_008661c8[unknown_574];
        UnknownFacingDir direction_temp;
        UnknownFacingDir direction = direction_temp.Direction(Center_Coord(), unknown_45c);
        int speed = (int)(Type->Speed * unknown_tube_speed_79d568);
        CoordStruct exit = Cell_Center_006faa30(tube->ExitCell, 0);
        CoordStruct enter = Cell_Center_006faa30(tube->EnterCell, 0);
        int height_step = tube->unknown_1c0;
        int enter_height = Map.Get_Height_GL(enter);
        height_step = (Map.Get_Height_GL(exit) - enter_height) / height_step;
        if (distance <= speed) {
            unknown_575++;
            int tube_dir = tube->Directions[unknown_575];
            finished = tube_dir == -1;
            Set_Coord(unknown_45c);
            if (!finished) {
                coord = Adjacent_Coord_006faa30(unknown_45c, tube_dir);
                CoordStruct delta = Map[coord].unknown_475d60() - Map[unknown_45c].unknown_475d60();
                unknown_45c += CoordStruct(delta.X, delta.Y, 0);
                unknown_45c.Z += height_step;
                UnknownFacingDir dir = UnknownFacingDir().Direction(Center_Coord(), unknown_45c);
                CoordStruct new_coord = Move_Coord_006faa30(Coord, dir, speed - distance);
                double divisor;
                if (tube_dir % 2) {
                    divisor = unknown_diagonal_lepton_7a3a20;
                } else {
                    divisor = unknown_lepton_79a700;
                }
                new_coord.Z += (speed - distance) / divisor * height_step;
                Set_Coord(new_coord);
                return;
            }
        } else {
            int tube_dir = tube->Directions[unknown_575];
            CoordStruct new_coord = Move_Coord_006faa30(Coord, direction, speed);
            double divisor;
            if (tube_dir % 2) {
                divisor = unknown_diagonal_lepton_7a3a20;
            } else {
                divisor = unknown_lepton_79a700;
            }
            new_coord.Z += speed / divisor * height_step;
            Set_Coord(new_coord);
            return;
        }
    }
    CellClass *cell = &Map[CoordStruct(Coord)];
    ObjectClass *occupier = cell->OccupierPtr;
    if (occupier != 0) {
        DynamicVectorClass<ObjectClass *> list;
        do {
            list.Add(occupier);
            occupier = occupier->unknown_30;
        } while (occupier != 0);
        for (int index = 0; index < list.Count(); index++) {
            ObjectClass *object = list[index];
            if (object->Kind_Of() == 1 || object->Kind_Of() == 15) {
                if (!static_cast<FootClass *>(object)->Locomotor.Checked()->Is_Moving()) {
                    object->Scatter(CoordStruct(0, 0, 0), true, true);
                }
            }
        }
        unknown_slot306(0.0);
    } else {
        TubeClass *tube = unknown_TubeClass_list_008661c8[unknown_574];
        CoordStruct exit_coord = Cell_Center_006faa30(tube->ExitCell, unknown_45c.Z);
        Set_Coord(exit_coord);
        unknown_574 = -1;
        Mark(1);
        if (unknown_494 == cell) {
            Scatter(CoordStruct(0, 0, 0), true, true);
        }
        unknown_57b = true;
        unknown_slot93(2);
        unknown_slot306(1.0);
        TubeClass *here = Cell_Tube_0047a130(&Map[CoordStruct(Coord)]);
        if (here != 0) {
            unknown_2e0.Set(UnknownFacingDir((unsigned short)((here->Facing << 13) - 0x8000)));
        }
    }
}
#endif

// void UnitClass::unknown_slot239() (0x006FBB40, slot 239)
// While a unit that cannot fire on the move is moving, keeps the timer at
// +0x194 from running out (one frame more than it has left); then the
// techno behaviour.

void UnitClass::unknown_slot239()
{
    if (!Type->MobileFire && Locomotor.Checked()->Is_Moving() && unknown_194.Value() > 0) {
        unknown_194 = CDTimerClass<FrameTimerClass>(unknown_194.Value() + 1);
    }
    TechnoClass::unknown_slot239();
}


// bool UnitClass::unknown_slot284() (0x006FBBF0, slot 284)
// True for DeployToFire types; for a type that deploys into a tick-tank
// building, owned by a non-human house (0x004F52F0), with a unit at
// +0x228: whether the cell here allows building (0x0047CDD0).

bool UnitClass::unknown_slot284()
{
    if (Type->DeployToFire) {
        return true;
    }
    if (Type->DeploysInto && Type->DeploysInto->TickTank && !House->Unknown_004F52F0() && unknown_228) {
        switch (unknown_228->What_Am_I()) {
        case 1:
        {
            CoordStruct coord = Coord;
            return Map[coord].Can_Build_Here() ? true : false;
        }
        case 6:
        case 15:
            return false;
        }
    }
    return false;
}


// Responds to the action for the current target; field/action names provisional.
extern "C" int const Unit_Attack_Frames_007F8F2C[8];
void UnitClass::unknown_6fbc90()
{
    if (!unknown_228) return;
    if (!unknown_slot231(0)->WeaponType) return;
    int weapon = unknown_slot174(unknown_228);
    int action = unknown_slot218(unknown_228, weapon, 1);
    if ((action == 0 || action == 2) && unknown_slot284()) {
        QueueMission(16, false);
        return;
    }
    switch (action) {
    case 5:
        if (unknown_6c16f0(weapon) < 0) {
            AbstractClass *target = unknown_228;
            if (target && target->AbstractFlags.IsObject && target->Kind_Of() == 1) {
                if (((ObjectClass *)target)->Health_Ratio() >= Rules->unknown_1360) {
                    unknown_slot220(0);
                    return;
                }
            } else {
                unknown_slot220(0);
                return;
            }
        }
        break;
    case 0:
        if (!Type->unknown_ba8) unknown_57d = false;
        if (Type->LargeVisceroid || Type->SmallVisceroid) {
            StageClass::Stage = Unit_Attack_Frames_007F8F2C[Dir_To_8(Direction_To(unknown_228))];
            StageClass::Set_Rate(5);
        }
        unknown_slot221(unknown_228, weapon);
        return;
    case 2:
        if (!Type->unknown_ba9 && Type->Turret) {
            unknown_2f8.Set_Desired(Direction_To(unknown_228));
            return;
        }
        if (!unknown_494 && !Locomotor.Checked()->Is_Moving()) {
            unknown_2e0.Set_Desired(Direction_To(unknown_228));
            unknown_2f8.Set_Desired(unknown_2e0.Desired());
        }
        break;
    case 9:
        unknown_57d = false;
        unknown_slot252(false);
        return;
    case 8:
    case 11:
        unknown_57d = false;
        return;
    case 6:
        if (SpawnManager) SpawnManager->Abandon_Target();
        break;
    }
}


// void UnitClass::unknown_6fbf40() (0x006FBF40; TibSun UnitClass::Visceroid_AI):
// steps the idle animation, sends a small visceroid to merge with a neighbouring
// one, otherwise wanders to a random (or the remembered) adjacent cell.
// The cell next to `cell` toward an 8-way facing (TibSun Adjacent_Cell; here
// the cell is taken by value, which orders TPoint2D<short>::operator+'s loads as
// the original).
inline Cell Unit_Adjacent_Cell(Cell cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}

void UnitClass::unknown_6fbf40()
{
    Cell position = unknown_slot104();
    if (StageClass::Stage >= 90) {
        if (StageClass::Stage % 5 != 4) return;
        StageClass::Stage = unknown_id_source_a3d290->Random()(0, 2) * 30;
        StageClass::Set_Rate(1);
        return;
    }
    if (StageClass::Stage % 30 == 29) {
        StageClass::Stage = unknown_id_source_a3d290->Random()(0, 2) * 30;
    }
    StageClass::Set_Rate(1);
    if (Locomotor.Checked()->Is_Moving()) return;
    if (Type->SmallVisceroid) {
        int direction = 7;
        for (int index = 0; index < 8; index++) {
            direction = (direction + 1) & 7;
            ObjectClass *object = Map[Unit_Adjacent_Cell(position, direction)].OccupierPtr;
            if (object && object->Kind_Of() == 1 && ((UnitClass *)object)->Type->SmallVisceroid) {
                UnitClass *other = (UnitClass *)object;
                if (other->unknown_494 || other->unknown_228) return;
                other->Assign_Destination(this, true);
                unknown_5bc = -1;
                return;
            }
        }
    }
    if (unknown_494 || unknown_228) return;
    int direction;
    if (unknown_5bc == -1 || unknown_id_source_a3d290->Random()(0, 2) == 0) {
        direction = unknown_id_source_a3d290->Random()(0, 7);
        unknown_5bc = direction;
    } else {
        direction = unknown_5bc;
    }
    CellClass *destination = &Map[Unit_Adjacent_Cell(position, direction)];
    if (Can_Enter_Cell(destination, direction, unknown_5d47d0(), &Map[position], 1) == 0) {
        Assign_Destination(destination, true);
        return;
    }
    unknown_5bc = -1;
}


// Complete Unit radio handler, 0x006FC1F0: 1858 body + 14 padding.
// Includes the original switch tables in the independent bound; not accepted.
// Radio sender downcasts express observed Techno/Foot offsets, not proved types.
// Private Scope12 Cargo query; EAX int/const/source-owner spellings remain hypotheses.
// Uses the bounded Scope12 declaration-only headers; no callee implementation or wrapper.

static inline double Unit_Radio_Used_Size(CargoClass const &cargo) { return (double)cargo.GetTotalSize(); }
int UnitClass::Receive_Message(RadioClass *radio, int message, AbstractClass *&param)
{
    TechnoClass *sender = (TechnoClass *)radio;
    switch (message) {
    case 36:
        if (unknown_slot111()) return 0;
        {
            CellClass &tile = Map[unknown_slot19(0)];
            if ((tile.unknown_140 & 0x100) && !Is_Moving_Onto_Bridge(0)) return 10;
        }
        if (Get_Mission() == 16) return 10;
        if (unknown_370) return 10;
        return unknown_574 == -1 ? 1 : 10;
    case 7:
        FootClass::Receive_Message(radio, message, param);
        Assign_Destination(0, true);
        unknown_slot220(0);
        QueueMission(0, false);
        unknown_4c8c90();
        if (!unknown_370 || !Contact(0)) {
            unknown_slot150(2, sender);
            unknown_slot149(0x18);
        }
        return 1;
    case 15:
        if (Type->Passengers != 0 && radio && House->Is_Ally(sender)) {
            if (unknown_slot111()) return 0;
            if (!sender->unknown_234 &&
                (!radio->AbstractFlags.IsFoot || !((FootClass *)radio)->unknown_580)) {
                if (sender->unknown_230 &&
                    *(int const *)((char const *)sender->unknown_230 + 0x24) != 0) return 0;
                int capacity = Type->Passengers;

                if (!(Unit_Radio_Used_Size(Cargo) + sender->Techno_Type_Class()->Size <= capacity))
                    goto no_transport_room;
                if (sender->Techno_Type_Class()->Size > Type->SizeLimit)
                    goto no_transport_room;
                return 1;
            }
        }
        return 0;
    case 22:
        FootClass::Receive_Message(radio, message, param);
        if (!unknown_599 && unknown_2e0.Current().Raw != 0x4000) {
            Locomotor.Checked()->Do_Turn(UnknownFacingDir(0x4000));
            return 1;
        }
        if (!Locomotor.Checked()->Is_Moving()) {
            TechnoClass *contact = Contact(0);
            if (unknown_370 && contact && contact->Kind_Of() == 6 && Get_Mission() == 7)
                unknown_slot150(21, contact);
        }
        return 1;
    case 21:
        {
            int capacity = Type->Passengers;
            if (Cargo.GetTotalSize() == capacity)
                unknown_2a8.Count_Down(Type->DeployTime);
        }
        return 5;
    case 14:
        if ((Locomotor.Checked()->Is_Moving() || unknown_494) && Mission == 2) return 10;
        if (Contact(0) != radio) {
            int capacity = Type->Passengers;

            if (Unit_Radio_Used_Size(Cargo) + sender->Techno_Type_Class()->Size <= capacity &&
                !(sender->Techno_Type_Class()->Size > Type->SizeLimit)) {
                if (!HasAnyLink()) unknown_slot150(2, sender);
            } else goto no_transport_room;
        }
        if (!unknown_494) {
            Cell cell = unknown_slot104();
            CellClass &tile = Map[cell];
            int own_zone = Map.Zone(unknown_454, sender->Techno_Type_Class()->MovementZone, false);
            int sender_zone = -1;
            if (radio && radio->AbstractFlags.IsFoot)
                sender_zone = Map.Zone(((FootClass *)radio)->unknown_454,
                                       sender->Techno_Type_Class()->MovementZone, false);
            if (own_zone != sender_zone) {
                Assign_Destination(radio, true);
                return 14;
            }
            if (!unknown_494 && tile.unknown_ec == 2) {
                Assign_Destination(radio, true);
                return 14;
            }
        }
        {
            if (Type->Passengers > 0) {
                int capacity = Type->Passengers;

                if (Unit_Radio_Used_Size(Cargo) + sender->Techno_Type_Class()->Size <= capacity &&
                    !(sender->Techno_Type_Class()->Size > Type->SizeLimit)) {
                    FootClass::Receive_Message(radio, message, param);
                    if (!Locomotor.Checked()->Is_Moving() && !unknown_599 && !unknown_370 &&
                        unknown_slot150(0x13, sender) == 1) {
                        param = this;
                        // Existing long&/int* ABI spellings both occupy one Win32 DWORD.
                        if (unknown_slot151(0x12, &param, sender) != 1)
                            unknown_slot150(3, sender);
                    }
                    return 1;
                }
            }
        }
        break;
    case 23:
        if ((Type->Harvester || Type->Weeder) && unknown_5b9) {
            unknown_5b9 = false;
            Scatter(unknown_geometry_l, true, false);
            QueueMission(10, false);
            if (unknown_slot122()) Commence();
        }
        break;
    case 3:
        if (Get_Mission() == 12) QueueMission(5, false);
        FootClass::Receive_Message(radio, message, param);
        return 1;
    }
    return FootClass::Receive_Message(radio, message, param);
no_transport_room:
    return 10;
}


// bool UnitClass::Unlimbo(CoordStruct const *coord, int dir) (0x006FC940, slot 53: Unlimbo)
// The foot placement, then the turret faces dir (a 0..255 direction); a
// cloakable unit not yet cloaked starts in state 2 (+0x1B8); visceroids
// start the +0xE0 counter at a random 0..29 with a one-frame timer,
// everything else at zero.

bool UnitClass::Unlimbo(CoordStruct const *coord, int dir)
{
    if (FootClass::Unlimbo(coord, dir)) {
        unknown_2f8.Set(UnknownFacingDir((unsigned short)(dir << 8)));
        if (unknown_32a && !unknown_32d) {
            unknown_1b8 = 2;
        }
        if (!Type->SmallVisceroid && !Type->LargeVisceroid) {
            StageClass::Stage = 0;
            static_cast<CDTimerClass<FrameTimerClass> &>(StageClass::StageTimer) = CDTimerClass<FrameTimerClass>(0);
            StageClass::Rate = 0;
            return true;
        }
        StageClass::Stage = unknown_id_source_a3d290->Random()(0, 29);
        static_cast<CDTimerClass<FrameTimerClass> &>(StageClass::StageTimer) = CDTimerClass<FrameTimerClass>(1);
        StageClass::Rate = 1;
        return true;
    }
    return false;
}


// int UnitClass::Take_Damage (0x006FCA30, slot 86: the damage path; Take_Damage hypothesis).
// After FootClass's damage, a destroyed unit plays its death frames, sinks (heavy naval
// units over water), sprays particles or explodes, unloads or kills its passengers, may
// release a crew member and may drop a crate near itself (0x005502E0 with a 1x1 size);
// a damaged harvester reports the attack, flees from the attacker or returns to a dock.
extern char const Unit_Ore_Miner_Under_Attack_007F819C[];
static inline AnimTypeClass *Unit_Damage_Last_Water_Animation() {
    DynamicVectorClass<AnimTypeClass *> const &animations =
        static_cast<DynamicVectorClass<AnimTypeClass *> const &>(Rules->SplashList);
    return animations[animations.Count() - 1];
}
static inline CoordStruct Unit_Damage_Raised_Position(CoordStruct const &position) { CoordStruct result = position; result.Z += 5; return result; }
int UnitClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead,
                            TechnoClass *source, bool forced, int arg6)
{
    CoordStruct animation_position;
    bool restore_selection = unknown_77 && House->Is_Player_Control();
    int result = FootClass::Take_Damage(damage, distance, warhead, source, forced, arg6);
    if (result == 5) return result;
    if (result == 4) {
        if (Type->DeathFrames > 0) {
            if (unknown_5c0 == -1) {
                unknown_5c0 = 0;
                unknown_slot216((int)source);
            }
            Strength = 1;
            unknown_7d = true;
        } else {
            if (Type->Naval && !Type->Underwater && !Type->Organic &&
                Type->Weight >= Rules->ShipSinkingWeight) {
                CoordStruct position = Coord;
                Cell cell = Coord_Cell(position);
                if (Map[cell].unknown_47b7e0() && !unknown_205) {
                    unknown_slot216((int)source);
                    Strength = 1;
                    unknown_7d = true;
                    unknown_325 = true;
                    unknown_slot210();
                    goto remove_passengers;
                }
            }
            unknown_slot216((int)source);
            if (warhead == Rules->unknown_cac) {
                int count = abs(unknown_id_source_a3d290->Random()() % 3) + 7;
                while (count > 0) {
                    ParticleSystemClass *system = new ParticleSystemClass(Rules->unknown_cf4,
                        Center_Coord(), 0, this, unknown_geometry_l);
                    system->unknown_e5 = true;
                    --count;
                }
            } else {
                if (Get_Height_AGL() <= 10 && unknown_7c) {
                    animation_position = Coord;
                    if (Map[animation_position].unknown_ec == 2) {
                        new AnimClass(Rules->unknown_09c, (animation_position = Coord), 0, 1, 0x600, 0);
                        new AnimClass(Unit_Damage_Last_Water_Animation(),
                            (animation_position = Unit_Damage_Raised_Position(Coord)),
                            0, 1, 0x600, 0);
                        goto remove_passengers;
                    }
                }
                Explode();
            }
        }
remove_passengers:
        Mark(0);
        if (!Type->Crashable) {
            while (Cargo.Object) {
                FootClass *passenger = unknown_4ccfc0();
                if (!passenger) break;
                int move = passenger->Can_Enter_Cell(&Map[CoordStruct(Coord)], -1, -1, 0, 1);
                bool can_place = move == 0 || move == 2;
                ++unknown_a40774;
                passenger->unknown_79 = unknown_79;
                animation_position = Coord;
                animation_position.Z = Get_Cell_Ptr()->Center_Coord().Z;
                if (!forced && !unknown_7c && can_place) {
                    int facing = Dir_To_256(unknown_2e0.Current());
                    if (passenger->Unlimbo(&animation_position, facing)) {
                        passenger->Scatter(unknown_geometry_l, true, false);
                        if (!House->Unknown_004F52F0()) {
                            if (Team) Team->Add(passenger);
                            else passenger->QueueMission(15, false);
                        }
                        if (restore_selection) passenger->unknown_slot80();
                        goto passenger_removed;
                    }
                }
                passenger->Record_The_Kill(source);
                delete passenger;
passenger_removed:
                --unknown_a40774;
            }
        }
        {
            InfantryClass *crew;
            if (unknown_290 == -1) {
                if ((unsigned char)arg6 || !Type->Crewed || Type->Passengers != 0 ||
                    (double)unknown_id_source_a3d290->Random()(0, 0x7ffffffe) *
                        (1.0 / 2147483646.0) >= Rules->CrewEscape)
                    goto drop_crate;
            }
            if (unknown_290 != -1) {
                crew = new InfantryClass(InfantryTypes[unknown_290], House);
            } else {
                InfantryTypeClass *crew_type = (InfantryTypeClass *)unknown_slot184();
                if (!crew_type) goto drop_crate;
                crew = new InfantryClass(crew_type, House);
            }
            if (crew) {
                crew->unknown_79 = unknown_79;
                CoordStruct position = Coord;
                if (crew->Unlimbo(&position, 0)) {
                    crew->Strength = unknown_id_source_a3d290->Random()(5, crew->Type->MaxStrength / 2);
                    crew->Scatter(unknown_geometry_l, true, false);
                    if (!House->Unknown_004F52F0()) crew->QueueMission(15, false);
                    else crew->QueueMission(5, false);
                    if (restore_selection) crew->unknown_slot80();
                    if (unknown_34 && unknown_34->Is_To_Inherit()) crew->Attach_Tag(unknown_34);
                } else {
                    delete crew;
                }
            }
        }
drop_crate:
        if (Type->CarriesCrate &&
            ((unknown_id_source_a3d290->TruckCrate && !Type->IsTrain) ||
             (unknown_id_source_a3d290->unknown_2229 && Type->IsTrain))) {
            Cell destination;
            destination = Map.Nearby_Location(unknown_slot104(), 1, -1, 0, false,
                Point2D(1, 1), 1, 0, 0, 1, Cell(0, 0), 0);
            if (destination != unknown_geometry_k && unknown_flag_a3d2c1)
                Map.Unknown_0054E580(destination, 20);
        }
        if (Type->Crashable) {
            if (!unknown_slot224(0)) Delete_Me();
        } else {
            if (!unknown_325) Delete_Me();
        }
    } else {
        if (result && Type->Harvester && House == unknown_player_a35db4) {
            Cell cell = Coord_Cell(unknown_slot19(0));
            if (Unknown_0063B5B0(4, cell))
                Speech_Play_Name(Unit_Ore_Miner_Under_Attack_007F819C, -1, -1);
        }
        if (Team || !source || unknown_370 || House->Is_Ally(source) || House->Unknown_004F52F0())
            return result;
        if (unknown_707170(source)) {
            Assign_Destination(source, true);
            QueueMission(2, false);
            return result;
        }
        if (!(Type->Harvester || Type->Weeder) || unknown_slot166() <= 0 ||
            Health_Ratio() > Rules->unknown_1368) return result;
        TechnoClass *dock = 0;
        for (int index = 0; index < Type->Dock.Count(); ++index) {
            dock = unknown_slot301(Type->Dock[index], 0, 0);
            if (dock) break;
        }
        if (dock && !ContainsLink(dock) && unknown_slot150(2, dock) == 1)
            QueueMission(7, false);
    }
    return result;
}


// UnitClass death animations and stored-resource explosion strength.

// The original passes a shake magnitude in ECX to this disabled engine hook.
void __fastcall Unit_Unknown_Shake_00482e00(int magnitude);

void UnitClass::Explode()
{
    if (Type->Explosion.Count()>0) {
        AnimTypeClass *animation=Type->Explosion.Pick(unknown_id_source_a3d290->Random());
        if ((Type->Explodes || unknown_6d7910(10)) && (Type->Ammo==-1 || unknown_254>0)) {
            animation=Type->Explosion[Type->Explosion.Count()-1];
        }
        new AnimClass(animation,Position_Coord_004f3f40(),0,1,0x600,0);
        if (unknown_294.Get_Total_Amount()>0 && Rules->unknown_1429 && !(unknown_id_source_a3d290->unknown_000&0x800)) {
            int power=0;
            for (int i=0;i<unknown_TiberiumClass_list_00ac0b30.Count();i++) {
                power+=unknown_294.Get_Amount(i)*unknown_TiberiumClass_list_00ac0b30[i]->Power;
            }
        }
        if (Type->MaxStrength>Rules->unknown_4e8) {
            int shakes=Type->MaxStrength/(Rules->unknown_4e8/2)+3;
            Unit_Unknown_Shake_00482e00(shakes<6?shakes:6);
        }
    }
    if (Type->DestroyAnim.Count()>0) {
        AnimTypeClass *animation=Type->DestroyAnim.Pick(unknown_id_source_a3d290->Random());
        new AnimClass(animation,Position_Coord_004f3f40(),0,1,0x600,0);
    }
}


// bool UnitClass::unknown_slot78(int arg1, ObjectClass *object, bool arg3) (0x006FD5A0, slot 78)
// arg1 is an action, object the object it applies to (hypothesis: Westwood's
// Active_Click_With). Re-derives the action (slot 28); a changed answer of
// 3 becomes 1 and of 9 or 16 becomes 5, as do 27 and 29. Moving onto
// itself (2 or 5) is refused; otherwise the foot behaviour.

bool UnitClass::unknown_slot78(int arg1, ObjectClass *object, bool arg3)
{
    int action = unknown_slot28(object, arg3);
    if (arg1 != action) {
        arg1 = action;
        switch (action) {
        case 3:
            arg1 = 1;
            break;
        case 9:
        case 16:
            arg1 = 5;
            break;
        }
    }
    if (arg1 == 27 || arg1 == 29) {
        arg1 = 5;
    }
    if (object == this && (arg1 == 2 || arg1 == 5)) {
        return false;
    }
    return FootClass::unknown_slot78(arg1, object, arg3);
}


// bool UnitClass::unknown_slot77(int arg1, Cell const *arg2, Cell const *arg3, bool arg4) (0x006FD610, slot 77)
// The FootClass behaviour.

bool UnitClass::unknown_slot77(int arg1, Cell const *arg2, Cell const *arg3, bool arg4)
{
    return FootClass::unknown_slot77(arg1, arg2, arg3, arg4);
}


// bool UnitClass::unknown_slot261(bool arg1, bool arg2) (0x006FD630, slot 261)
// Choosing the idle mission (hypothesis: Westwood's Enter_Idle_Mode; arg1
// initial). After the foot step: a unit with a destination moves (2);
// harvesters and weeders harvest (10) unless in radio contact, already
// harvesting, or, for a computer house, off their land type (5); armed
// units guard (5), or hunt/area-guard (11) when their house is below the
// rules' +0x10A8 and they lack ability 16 and DefaultToGuardArea; units
// with passengers to drop unload (16). The mission is queued unless the
// unit is in mission 25, 11 or 16.

// The center of a cell (an inline helper taking a reference; hypothesis).
static inline CoordStruct Cell_Center_Of(Cell const &cell)
{
    return CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
}

bool UnitClass::unknown_slot261(bool arg1, bool arg2)
{
    bool result = FootClass::unknown_slot261(arg1, arg2);
    if (unknown_slot270() || Get_Mission() == 28) {
        return result;
    }
    if (Get_Mission() == 2 && !unknown_494) {
        CoordStruct center = Cell_Center_Of(unknown_slot104());
        CoordStruct coord = Coord;
        if (coord.X == center.X && coord.Y == center.Y && coord.Z == center.Z) {
            unknown_slot149(3);
        }
    }
    unknown_4c8b00();
    int mission;
    if (unknown_494) {
        mission = 2;
        goto queue;
    }
    if (!Type->Harvester && !Type->Weeder) {
        if (!unknown_slot163()) {
            if (unknown_32c && Type->Passengers > 0 && Cargo.Object && !Team) {
                mission = 16;
                goto queue;
            }
            if (unknown_57c) {
                goto done;
            }
            if (Mission == 16 && Type->DeploysInto) {
                goto done;
            }
            if (Mission == 11) {
                goto done;
            }
            goto guard;
        }
        if (Get_Mission() == 5 || Get_Mission() == 11) {
            return result;
        }
        if (Get_Mission() != -1 && (Control()->unknown_07 || Control()->unknown_05)) {
            return result;
        }
        if (!(House->unknown_194 >= Rules->unknown_10a8 || unknown_6d7910(16) || Type->DefaultToGuardArea)
            || Team) {
            mission = 5;
        } else {
            mission = 11;
        }
        goto queue;
    }
    if (HasAnyLink()) {
        return result;
    }
    if (Get_Mission() == 10 || MissionQueue == 10) {
        return result;
    }
    mission = 10;
    if (arg1 || !House->Unknown_004F52F0()) {
        goto stop;
    }
    {
        CoordStruct coord = Coord;
        int land = Map[coord].unknown_ec;
        if (land == (Type->Harvester ? 5 : 11)) {
            goto stop;
        }
    }
guard:
    mission = 5;
stop:
    unknown_slot220(0);
    Assign_Destination(0, true);
queue:
    if (Mission != 25 && Mission != 11 && Mission != 16) {
        QueueMission(mission, false);
    }
done:
    return result;
}


// bool UnitClass::unknown_6fd930() (0x006FD930; slot 132 calls it: may start
// deploying). Without a DeploysInto type it returns true. A unit whose
// DeploysInto is one of the Rules +0x7C8 (BuildConst) types and whose house
// has +0x139 set looks for a free area (a Shipyard-sized one near the unit,
// then a BuildConst-sized one near that) and moves there, clearing +0x139.
// Otherwise it tries to place the building with its origin one cell
// north-west (AdjacentCell[7]) of the unit's centre, then of each of 36
// cells around the unit; when one fits it moves to that cell. The 36 offsets
// are a local static array (its atexit callback is 0x006FDFB0). The first
// placement test calls the cell sum out of line (0x0042ADD0, thiscall: a
// nested site in Deploy_Origin_6fd930 after the direct sites have spent the
// inline budget). The two zero cells are separate block locals (their shared
// slot puts the found cell below them). Names are hypotheses.

// The building origin for a unit cell: one cell north-west.
static inline Cell Deploy_Origin_6fd930(Cell const &cell)
{
    return cell + AdjacentCell[7];
}

// A search offset added to the unit's cell.
static inline Cell Offset_Cell_6fd930(Cell const &offset, Cell const &position)
{
    Cell result;
    result.X = offset.X;
    result.Y = offset.Y;
    result.X += position.X;
    result.Y += position.Y;
    return result;
}

bool UnitClass::unknown_6fd930()
{
    if (Type->DeploysInto == 0) return true;
    Mark(0);
    for (int index = 0; index < Rules->unknown_7c8.Count(); index++) {
        if (Rules->unknown_7c8[index] == Type->DeploysInto) {
            if (Owning_House()->unknown_139) {
                if (!unknown_494) {
                    Cell first;
                    {
                        Cell zero(0, 0);
                        first = Map.Nearby_Location(unknown_slot104(), 5, -1, 0, false, Point2D(House->unknown_func_4ef600(Rules->Shipyard)->Width() + 1, House->unknown_func_4ef600(Rules->Shipyard)->Foundation_Height(false) + 1), 0, 0, 0, 1, zero, 0);
                    }
                    {
                        Cell zero(0, 0);
                        first = Map.Nearby_Location(first, 1, -1, 0, false, Point2D(House->unknown_func_4ef600(Rules->unknown_7c8)->Width() + 1, House->unknown_func_4ef600(Rules->unknown_7c8)->Foundation_Height(false) + 1), 0, 0, 0, 1, zero, 0);
                    }
                    if (first != unknown_geometry_k) Assign_Destination(&Map[first], true);
                    Owning_House()->unknown_139 = false;
                }
                goto finish;
            }
            break;
        }
    }
    if (!unknown_494) {
        {
        if (Type->DeploysInto->Legal_Placement(Deploy_Origin_6fd930(Coord_Cell(Center_Coord())), 0)) {
            Mark(1);
            return true;
        }
        }
        if (!unknown_494) {
            static Cell offsets[36] = {
                Cell(0, -1),
                Cell(0, -2),
                Cell(1, -2),
                Cell(-1, -2),
                Cell(0, -3),
                Cell(1, -3),
                Cell(-1, -3),
                Cell(2, -3),
                Cell(-2, -3),
                Cell(0, -4),
                Cell(1, -4),
                Cell(-1, -4),
                Cell(2, -4),
                Cell(-2, -4),
                Cell(0, 1),
                Cell(0, 2),
                Cell(1, 2),
                Cell(-1, 2),
                Cell(0, 3),
                Cell(1, 3),
                Cell(-1, 3),
                Cell(2, 3),
                Cell(-2, 3),
                Cell(0, 4),
                Cell(1, 4),
                Cell(-1, 4),
                Cell(2, 4),
                Cell(-2, 4),
                Cell(-1, 0),
                Cell(-2, 0),
                Cell(-3, 0),
                Cell(-4, 0),
                Cell(1, 0),
                Cell(2, 0),
                Cell(3, 0),
                Cell(4, 0)
            };
            for (int offset_index = 0; offset_index < 36; offset_index++) {
                Cell adjacent = Offset_Cell_6fd930(offsets[offset_index], unknown_slot104());
                Cell placement = adjacent + AdjacentCell[7];
                if (Type->DeploysInto->Legal_Placement(placement, 0)) {
                    Assign_Destination(&Map[adjacent], true);
                    break;
                }
            }
        }
    }
finish:
    Mark(1);
    if (!unknown_494 && !House->Unknown_004F52F0()) Scatter(unknown_geometry_l, false, false);
    return false;
}


// The atexit callback (0x006FDFB0, _$E) of UnitClass::unknown_6fd930's local
// static array of 36 search offsets (0x00AD04E8); emitted with the
// UnitClass unit, no source of its own. Replaces the unidentified seed
// function at the same legacy source path.


// bool UnitClass::unknown_6fdfc0() (0x006FDFC0; hypothesis: Try_To_Deploy).
// A unit with a DeploysInto type (slot 186 permitting, not moving) tests
// the building's footprint at its cell (one cell north-west unless
// 0x0045EC50 says the type deploys in place). A blocked site plays
// EVA_CannotDeployHere for the player. A clear site first turns the unit to
// the type's deploy facing (0x0045EC90), then replaces it with a new
// building: attackers of the unit retarget the building (a construction
// yard is not retargeted by vehicle thieves), veterancy, health ratio,
// selection and tag carry over, and for a construction yard in a
// multiplayer game a house without 0x004F52F0 moves its base center and
// plan there. Returns true while deploying.
// Names are hypotheses.

bool UnitClass::unknown_6fdfc0()
{
    if (unknown_slot186()) {
        if (unknown_494) return false;
        if (Locomotor.Checked()->Is_Moving()) return false;
        if (Type->DeploysInto == 0) return false;
        Mark(0);
        Locomotor.Checked()->Mark_All_Occupation_Bits(0);
        Cell cell = unknown_slot104();
        Cell origin;
        if (Type->DeploysInto->unknown_func_45ec50()) {
            origin = cell;
        } else {
            origin = cell + AdjacentCell[7];
        }
        if (!Type->DeploysInto->Legal_Placement(origin, 0)) {
            if (House->Is_Player_Control()) Speech_Play_Name("EVA_CannotDeployHere", -1, -1);
            if (!House->Unknown_004F52F0()) Type->DeploysInto->unknown_458b10(origin, House);
            Locomotor.Checked()->Mark_All_Occupation_Bits(1);
            Mark(1);
        } else {
            Locomotor.Checked()->Mark_All_Occupation_Bits(1);
            Mark(1);
            int facing = Type->DeploysInto->unknown_func_45ec90();
            if (Dir_To_256(unknown_2e0.Current()) != facing) {
                if (!Locomotor.Checked()->Is_Moving_Now()) {
                    Locomotor.Checked()->Do_Turn(UnknownFacingDir((unsigned short)(facing << 8)));
                }
                unknown_slot149(3);
                unknown_57c = true;
                return true;
            }
            bool selected = unknown_77;
            Mark(0);
            Locomotor.Checked()->Mark_All_Occupation_Bits(0);
            BuildingClass *building = new BuildingClass(Type->DeploysInto, House);
            if (building) {
                building->QueueMission(18, false);
                if (building->Unlimbo(&CoordStruct(origin.X * 256 + 128, origin.Y * 256 + 128, 0), 0)) {
                    unknown_slot149(3);
                    for (int index = 0; index < Technos.Count(); index++) {
                        TechnoClass *techno = Technos[index];
                        if (techno->unknown_228 && techno->unknown_228->Kind_Of() == 1 &&
                            techno->unknown_228 == this && techno->unknown_7d &&
                            techno != this && techno != building) {
                            if (building->Type->ConstructionYard && techno->Kind_Of() == 15 &&
                                ((InfantryTypeClass *)techno->Techno_Type_Class())->VehicleThief) {
                                techno->unknown_slot220(0);
                            } else {
                                techno->unknown_slot220(building);
                            }
                        }
                    }
                    building->unknown_114 = unknown_114;
                    building->unknown_1ac = unknown_1ac;
                    building->unknown_118 = unknown_118;
                    if (building->House->Is_Player_Control()) {
                        Voc_Play_At(Rules->BuildingDrop, Center_Coord(), 0);
                    } else {
                        building->unknown_59a = true;
                        building->unknown_59b = true;
                    }
                    if (building->Type->Artillary || building->Type->TickTank) {
                        building->unknown_2c8.Set(UnknownFacingDir(0x4000));
                    }
                    building->unknown_slot220(unknown_228);
                    building->Revealed(House);
                    building->unknown_5ad = true;
                    if (!House->Unknown_004F52F0() && building->Type->ConstructionYard && unknown_game_mode_a3d298) {
                        CoordStruct position = building->Coord;
                        Cell base = Coord_Cell(position);
                        House->BaseCenter = Cell_Center(base);
                        House->Unknown_004EF5A0();
                        House->unknown_55f0.Nodes[0].Cell = base;
                        House->unknown_55f0.unknown_50 = base;
                        House->unknown_136 = true;
                        House->unknown_13a = true;
                        House->unknown_13b = true;
                        House->Unknown_004F6310();
                    }
                    building->Strength = (int)(Health_Ratio() * building->Type->MaxStrength);
                    building->Strength = building->Strength > 1 ? building->Strength : 1;
                    building->unknown_70 = building->Strength;
                    if (selected) building->unknown_slot80();
                    unknown_slot210();
                    if (unknown_34) {
                        building->Attach_Tag(unknown_34);
                        unknown_34->AttachCount--;
                        unknown_34 = 0;
                    }
                    Delete_Me();
                    return true;
                }
                delete building;
            }
            Locomotor.Checked()->Mark_All_Occupation_Bits(1);
            Mark(1);
        }
    }
    unknown_57c = false;
    return false;
}


// Per-cell arrival, docking, movement completion and terrain consequences.
extern "C" GUID const CLSID_HoverLocomotionClass;
TechnoClass *__fastcall Unknown_0040DC20(AbstractClass *object);
ILocomotion *__fastcall Unit_Checked_Locomotor_004cde90(UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600> const &owner);

void UnitClass::unknown_slot93(int reason)
{
    Cell cell=unknown_slot104();
    if ((reason==2 || reason==0) && unknown_57c) {
        unknown_6fdfc0();
        if (!unknown_7d) return;
    }
    if (reason==2) {
        TechnoClass *contact=Contact(0);
        if ((Get_Mission()==7 || Get_Mission()==25) && contact && contact->Kind_Of()==6) {
            CoordStruct center=Cell_Coord(Coord_Cell(Center_Coord()));
            CoordStruct contact_center=Cell_Coord(Coord_Cell(contact->unknown_slot41((int)this)));
            if (Coord_Cell(center)==Coord_Cell(contact_center)) {
                UnknownComPtr<IPersist,&unknown_uuid_IPersist_7cf208> persist;
                if (Locomotor) {
                    HRESULT status=persist.Query_Held(Unit_Checked_Locomotor_004cde90(Locomotor));
                    if (FAILED(status) && status!=E_NOINTERFACE) _com_issue_error(status);
                }
                GUID clsid;
                persist.Checked()->GetClassID(&clsid);
                if (clsid==CLSID_HoverLocomotionClass && ((BuildingClass *)contact)->Type->UnitRepair && !unknown_494) unknown_494=contact;
                if (contact==unknown_494) {
                    FootClass::unknown_slot93(2);
                    unknown_slot149(21);
                    Locomotor.Checked()->Power_Off();
                    return;
                }
            }
        }
        if (unknown_370 && contact && contact->Kind_Of()==6 && Get_Mission()==7) {
            if (contact==Map[cell-Cell(0,1)].Cell_Building()) {
                switch (unknown_slot150(21,contact)) {
                case 1: case 5: break;
                default: Scatter(unknown_geometry_l,true,false);break;
                }
            }
        }
        UnknownCellClass *current=(UnknownCellClass *)Get_Cell_Ptr();
        TechnoClass *transport=current->Unknown_004717B0();
        if (!transport) {
            transport=((UnknownCellClass *)Get_Cell_Ptr())->Unknown_00473E80(unknown_79);
            if (!transport) {
                transport=((UnknownCellClass *)Get_Cell_Ptr())->Unknown_00473E30(unknown_79);
            }
        }
        if (transport==this) transport=Unknown_0040DC20(transport->unknown_30);
        if (Get_Mission()==7 && transport && transport!=this) {
            if (unknown_slot104()==transport->unknown_slot104() && ContainsLink(transport) && transport->Techno_Type_Class()->Passengers>0) {
                if (unknown_slot150(15,transport)==1) {
                    unknown_6d6e50(0);
                    unknown_79=false;
                    Limbo();
                    transport->Cargo.Attach(this);
                    Hidden();
                } else {
                    Assign_Destination(0,true);
                    QueueMission(5,false);
                    Scatter(unknown_geometry_l,true,true);
                }
                return;
            }
        }
        if (unknown_370 && (Get_Mission()!=7 || (Unknown_0040DC20(unknown_494) && Contact(0)!=unknown_494)) && Get_Mission()!=16) {
            bool arrived=unknown_494==0;
            if (unknown_494 && unknown_494->Kind_Of()==11 && ((CellClass *)unknown_494)->MapCoords==Coord_Cell(Position_Coord_004f3f40())) arrived=true;
            if (!Map[Position_Coord_004f3f40()].Cell_Building() || arrived) {
                contact=Contact(0);
                if (arrived || !contact || contact->Kind_Of()!=6 || !((BuildingClass *)contact)->Type->WeaponsFactory || Map[Position_Coord_004f3f40()].Cell_Building()!=contact) {
                    int response=unknown_slot149(8);
                    if (response==23) {
                        if (unknown_494 && unknown_494!=Get_Cell_Ptr()) {
                            AbstractClass *destination=unknown_494;
                            if (destination && destination->AbstractFlags.IsTechno) unknown_slot150(14,(TechnoClass *)destination);
                        } else {
                            BuildingClass *building;
                            if (!contact) building=0;
                            else building=contact->Kind_Of()==6 ? (BuildingClass *)contact : 0;
                            if (!Type->Harvester && !Type->Weeder) {
                                if (!House->Unknown_004F52F0() && building && building->Type->WeaponsFactory) {
                                    Cell where=House->Unknown_004EB270(this);
                                    if (where!=unknown_geometry_k) {
                                        QueueMission(2,false);
                                        Assign_Destination(&Map[where],true);
                                        Commence();
                                        unknown_6d6e50(&Map[where]);
                                        QueueMission(11,false);
                                    } else unknown_6d6e50(0);
                                } else {
                                    AbstractClass *destination=unknown_1b0;
                                    if (destination && destination!=unknown_494) Assign_Destination(destination,true);
                                    else { unknown_4cd9b0();Scatter(unknown_geometry_l,true,false); }
                                }
                            } else QueueMission(10,true);
                        }
                    } else if (response!=10 && (Type->Harvester || Type->Weeder)) {
                        if (unknown_1b0) {
                            QueueMission(10,false);
                            Assign_Destination(unknown_1b0,true);
                            unknown_6d6e50(0);
                        } else if (!unknown_494) Scatter(unknown_geometry_l,true,false);
                    }
                }
            }
            if (Map[Position_Coord_004f3f40()].Cell_Building() && !unknown_494 && unknown_49c.Count()==0 && unknown_478.Count()==0) Scatter(unknown_geometry_l,true,true);
        }
        if (Get_Mission()==5 && !Map.Unknown_0054AB00(Position_Coord_004f3f40()) && unknown_32d) {
            if (Team) Team->TargetNotAssigned=true;
            unknown_slot210();
            Delete_Me();
            return;
        }
        unknown_slot263(false,false,false,0);
        unknown_slot262(false,false,false,0);
        Map.Unknown_0054A550(Position_Coord_004f3f40(),0,unknown_1f4+3,0);
        if (!unknown_5b9 && unknown_slot122()) Commence();
        if (Type->Harvester && Get_Mission()!=16 && Mission!=7 && MissionQueue!=7 && !unknown_5b9) {
            RadioClass *radio=Contact(0);
            if (radio && radio->What_Am_I()==6 && ((BuildingClass *)radio)->Type->Refinery) unknown_slot149(3);
        }
        if (Type->Weeder && Get_Mission()!=16 && Mission!=7 && MissionQueue!=7 && !unknown_5b9) {
            RadioClass *radio=Contact(0);
            if (radio && radio->What_Am_I()==6 && ((BuildingClass *)radio)->Type->Weeder) unknown_slot149(3);
        }
        if (!unknown_494 && Path[0]==-1 && !Type->MobileFire) unknown_244=unknown_slot187(1)/4;
        CellClass *current_cell=&Map[Position_Coord_004f3f40()];
        int land=current_cell->unknown_ec;
        if (!Locomotor.Checked()->Is_Moving() && Can_Enter_Cell(current_cell,-1,-1,0,1)==7 && (!unknown_79 || !(current_cell->unknown_140&0x100)) && !unknown_325) {
            if (Mission!=7) {
                new AnimClass(Select_Damage_Anim(Strength,Rules->unknown_c74,land,&Position_Coord_004f3f40()),Position_Coord_004f3f40(),0,1,0x2600,Get_Explosion_Z_0047fcc0(Position_Coord_004f3f40()));
                int damage=Strength;
                Unknown_0047F600(Center_Coord(),damage,Rules->unknown_c74,0,0);
                Take_Damage(&damage,0,Rules->unknown_c74,0,true,0);
                return;
            }
            Scatter(unknown_geometry_l,true,true);
        }
    }
    CellClass *current_cell=&Map[cell];
    if ((Type->Crusher || unknown_6d7910(17)) && current_cell->OverlayIndex!=-1) {
        OverlayTypeClass *overlay=OverlayTypes[current_cell->OverlayIndex];
        if (overlay->unknown_1fd) {
            Voc_Play_At(overlay->unknown_1c0,Center_Coord(),0);
            current_cell->Reduce_Wall(-1);
            unknown_59f=false;
            unknown_28c+=0.019999999552965164f;
        }
    }
    Overrun_Square(unknown_slot104(),false);
    if (unknown_7d) FootClass::unknown_slot93(reason);
}


// 0x006fe5f0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_006fe5f0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006fe5f0 {

void  FUN_006fe5f0(void)
{
    return;
}

}  // namespace seed_006fe5f0


// 0x006fe600: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_006fe600(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006fe600 {

void  FUN_006fe600(void)
{
    return;
}

}  // namespace seed_006fe600


// bool UnitClass::Render(Rect *bounds, bool arg2, bool arg3) (0x006FF350, slot 63)
// Not drawn while +0x370 is set and the first contact is a building in or
// queued for mission 16 and the state at +0x2A8 is busy; otherwise the
// object behaviour (with arg3 cleared).

bool UnitClass::Render(Rect *bounds, bool arg2, bool arg3)
{
    if (unknown_370) {
        TechnoClass *contact = Contact_With_Whom(0);
        if (contact->Kind_Of() == 6 && (contact->Get_Mission() == 16 || contact->MissionQueue == 16)) {
            DelayTimerClass *state = &contact->unknown_2a8;
            if (state->Is_Count_Up_Active() || state->Is_Count_Down_Active() || state->Is_Count_Up_Complete() || state->Is_Count_Down_Complete()) {
                return false;
            }
        }
    }
    return ObjectClass::Render(bounds, arg2, false);
}


// void UnitClass::unknown_slot312(Surface &surface, Point2D point, Rect window,
// int depth) (slot 312, 0x006FF3E0; slots 310/311 call it). Blits the
// unit's image surface (unknown_surface_83995c, through the unit rectangle
// 0x00AD0490 moved to point - (128, 128)) with the house color scheme's
// blitter for the cloak/flag state. A TooBigToFitUnderBridge unit (when
// 0x006CF670 holds and 0x006CF9D0 is 0, or it is in radio contact with a
// weapons factory) is drawn in two parts: all but the bottom 16 rows at
// depth slot 176 - 5, then the bottom 16 rows. Names are hypotheses.

void UnitClass::unknown_slot312(Surface &surface, Point2D point, Rect window, int depth)
{
    bool split = false;
    if (Type->TooBigToFitUnderBridge) {
        if (unknown_6cf670() && unknown_6cf9d0() == 0) {
            split = true;
        } else {
            TechnoClass *contact = Contact_With_Whom(0);
            if (unknown_494 && contact && contact->Kind_Of() == 6 && ((BuildingClass *)contact)->Type->WeaponsFactory)
                split = true;
        }
    }
    int draw_adjust = 0;
    unsigned flags = 0x2800;
    switch (unknown_slot25(false, 0)) {
    case 1: flags = 0x2802; break;
    case 2:
    case 3: flags = 0x2804; break;
    case 4:
        flags = 0x280A;
        if (unknown_1bc.Value) flags = 0x280C;
        draw_adjust = unknown_6d6750();
        break;
    }
    if (unknown_slot111() || unknown_slot112()) flags |= 4;
    if (House->Is_Player_Control() && unknown_slot48()) flags = unknown_slot244(flags);
    Blitter *blitter = ColorSchemes[House->ColorSchemeIndex]->Converter->Select_Blitter_00485AA0(flags);
    Rect destination(unknown_unit_rect_ad0490.X + point.X - 128,
                     unknown_unit_rect_ad0490.Y + point.Y - 128,
                     unknown_unit_rect_ad0490.Width, unknown_unit_rect_ad0490.Height);
    if (split && unknown_unit_rect_ad0490.Height > 16) {
        Rect source = unknown_unit_rect_ad0490;
        destination.Height = unknown_unit_rect_ad0490.Height - 16;
        source.Height = destination.Height;
        Bit_Blit(surface, window, destination, *unknown_surface_83995c, unknown_surface_83995c->Get_Rect(), source,
                 *blitter, unknown_slot176() - 5, 0, depth, draw_adjust);
        destination.Y += destination.Height;
        source.Y += destination.Height;
        destination.Height = 16;
        source.Height = 16;
        Bit_Blit(surface, window, destination, *unknown_surface_83995c, unknown_surface_83995c->Get_Rect(), source,
                 *blitter, unknown_slot176(), 2, depth, draw_adjust);
        return;
    }
    Bit_Blit(surface, window, destination, *unknown_surface_83995c, unknown_surface_83995c->Get_Rect(), unknown_unit_rect_ad0490,
             *blitter, unknown_slot176(), unknown_slot177(), depth, draw_adjust);
}


// Vehicle shape frames, turret compositing and optional voxel barrels.

// Original717F90 copies48 bytes fromAF77A8 to the hidden ECX result and returns it inEAX.
Matrix3D __fastcall Unit_Isometric_View_00717f90();
extern ConvertClass *unknown_drawer_8323c4;

inline Vector3 Unit_Negated_Vector_007005b0(float const &x,float y,float z)
{
    return Vector3(x,y,z);
}
inline Vector3 operator-(Vector3 const &vector)
{
    return Unit_Negated_Vector_007005b0(-vector.X,-vector.Y,-vector.Z);
}

inline Vector3 Unit_Matrix_Translation_007005b0(Matrix3D const &matrix)
{
    return Vector3(matrix.Row[0][3],matrix.Row[1][3],matrix.Row[2][3]);
}

inline int Unit_Dir_To_4_007005b0(UnknownFacingDir const &direction)
{
    return (((*(unsigned const *)&direction>>13)+1)>>1)%4u;
}

inline double Unit_Direction_Radians32_007005b0(UnknownFacingDir const &direction)
{
    return (Dir_To_32(direction)-8)*-0.19634954084936207;
}

inline bool Unit_Voxel_Loaded_007005b0(UnknownObjectPair const &voxel)
{
    return voxel.unknown_00!=0 && voxel.unknown_04!=0;
}

void UnitClass::unknown_slot311(Point2D point,Rect window,int brightness)
{
    UnknownObjectPair *voxel;
    ShapeFileStruct const *shape=static_cast<ShapeFileStruct const *>(Get_Image_Data());
    bool barrel_above=true;
    int frame;
    if (Type->Facings==8 && !unknown_slot48()) frame=(Dir_To_8(unknown_2e0.Current())+1)&7;
    else frame=0;
    if (unknown_5a8>=0) {
        frame=Type->StartFiringFrame+Type->FiringFrames*frame+unknown_5a8/2;
    } else if (!Locomotor.Checked()->Is_Moving() && Type->IdleRate==0) {
        if (unknown_5c0>=0) {
            int death_frame=unknown_5c0/Type->DeathFrameRate;
            int start_death_frame=Type->StartDeathFrame;
            int last_frame=Type->DeathFrames-1;
            if (death_frame>=last_frame) death_frame=last_frame;
            frame=start_death_frame+death_frame;
        } else if (unknown_5a0) {
            if (Type->StandingFrames==0) frame=Type->StartWalkFrame+frame*Type->WalkFrames;
            else frame=Type->StartStandFrame+frame*Type->StandingFrames;
        }
    } else {
        frame=Type->StartWalkFrame+frame*Type->WalkFrames+unknown_430%Type->WalkFrames;
    }
    if (Type->Turret) {
        Point2D shadow_point=point;
        if (unknown_37c) shadow_point.Y-=14;
        CC_Draw_Shape(unknown_surface_83995c,unknown_drawer_8323c4,shape,frame+shape->Get_Count()/2,
                      shadow_point,window,0x2e01,0,unknown_slot176()-2,0,1000,0,0,Point2D(0,0));
        Surface *old_surface=unknown_surface_83995c;
        unknown_surface_83995c=unknown_surface_ad0604;
        Point2D composite_point=Point2D(128,128);
        Matrix3D barrel_matrix;
        Rect composite_window=unknown_surface_83995c->Get_Rect();
        unknown_5bb=true;
        unknown_unit_rect_ad0490=unknown_static_rect;
        voxel=&Type->unknown_c0;
        if (Unit_Voxel_Loaded_007005b0(*voxel)) {
            int key=-1;
            Matrix3D draw_result;
            Matrix3D turret_matrix(*Locomotor.Checked()->Draw_Matrix(&draw_result,&key));
            turret_matrix.Translate_X((float)(Type->TurretOffset/8));
            turret_matrix.Rotate_Z((float)(Unit_Direction_Radians32_007005b0(unknown_2f8.Current())-Unit_Direction_Radians32_007005b0(unknown_2e0.Current())));
            barrel_matrix=turret_matrix;
            Vector3 translation=Unit_Matrix_Translation_007005b0(turret_matrix);
            barrel_matrix.Translate(-translation);
            if (unknown_330.unknown_18) turret_matrix.Translate_X(-unknown_330.unknown_14);
            Vector3 fire_offset=Vector3((float)(unknown_slot231(0)->FLH.X/-8),0.0f,(float)(unknown_slot231(0)->FLH.Z/-8));
            barrel_matrix.Translate(-fire_offset);
            FacingClass pitch=unknown_2c8;
            BulletTypeClass const *projectile=unknown_slot231(0)->WeaponType->Projectile;
            if (projectile->Inviso) {
                int direction=Dir_To_256(pitch.Current());
                if (direction>64 && direction<=128) direction=(direction-64)/3+64;
                else direction=(64-direction)/3+64;
                pitch.Set(UnknownFacingDir((unsigned short)(direction<<8)));
            }
            if (unknown_350.unknown_18) barrel_matrix.Translate_X(-unknown_350.unknown_14);
            barrel_matrix.Rotate_Y((float)-Unit_Direction_Radians32_007005b0(pitch.Current()));
            barrel_matrix.Translate(fire_offset);
            barrel_matrix.Translate(translation);
            barrel_above=Unit_Dir_To_4_007005b0(unknown_2f8.Current())>0 && Unit_Dir_To_4_007005b0(unknown_2f8.Current())<3;
        }
        unknown_slot293(shape,frame,&composite_point,&composite_window,0,256,0,0,0,brightness,0,0,Point2D(0,0),0x2800);
        voxel=&Type->unknown_c0;
        if (Unit_Voxel_Loaded_007005b0(*voxel) && !barrel_above) {
            // Slot246 retains its existing nine-word ABI; the observed references are explicit.
            unknown_slot246(Type->unknown_c0,0,-1,0,composite_window,composite_point,
                            (Unit_Isometric_View_00717f90()*barrel_matrix),brightness,0x2800);
        }
        unsigned turret_frame=((Dir_To_32(unknown_2f8.Current())+4)&31)+8*Type->WalkFrames;
        unknown_slot293(shape,turret_frame,&composite_point,&composite_window,0,256,0,0,0,brightness,0,0,Point2D(0,0),0x2820);
        voxel=&Type->unknown_c0;
        if (Unit_Voxel_Loaded_007005b0(*voxel) && barrel_above) {
            unknown_slot246(Type->unknown_c0,0,-1,0,composite_window,composite_point,
                            (Unit_Isometric_View_00717f90()*barrel_matrix),brightness,0x2820);
        }
        int apparent_brightness=unknown_slot254(brightness);
        unknown_slot312(*old_surface,point,window,apparent_brightness);
        unknown_surface_83995c->Fill_Rect(unknown_unit_rect_ad0490,0);
        unknown_surface_83995c=old_surface;
        unknown_5bb=false;
    } else {
        if (Type->TooBigToFitUnderBridge && unknown_6cf670() && unknown_6cf9d0()==0) {
            unknown_slot293(shape,frame,&point,&window,0,256,-16,0,0,brightness,0,0,Point2D(0,0),0);
            return;
        }
        unknown_slot293(shape,frame,&point,&window,0,256,0,unknown_slot177(),0,brightness,0,0,Point2D(0,0),0);
    }
}


// Not exact (RA2_NON_MATCHING): UnitClass::unknown_slot310 at 0x006FF690, best score 86.76, 2026-11-13; differs: 1076 instructions in the original, 1084 in the draft (shape 98.33)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x6FF690 FUN_006ff690
// entry 0x6FF690; exact-match score 86.76 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout974 (build/class-work/Scout974/best_8676.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// The shadow mask surface (cache_key) is a Surface * since Scout1102 r2 (shadow draw 0x004C9A40 takes Surface *mask).
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// From TibSun code/unit.cpp (codex4/codex2 drafts); Is_Cached, Is_Barrel_Ahead and the shadow-key
// goto form from ra2-decomp 0x0073B470 (UnitClass::Unit_Draw_Voxel). Adapted to the exact RA2 006FF690 body.
// Contains material derived from Electronic Arts source code; EA GPLv3 Section 7 terms apply.
Matrix3D __fastcall Unit_Isometric_View_00717f90();
inline Vector3 operator-(Vector3 const &vector);
inline Vector3 Unit_Matrix_Translation_007005b0(Matrix3D const &matrix);
inline int Unit_Dir_To_4_007005b0(UnknownFacingDir const &direction);
inline double Unit_Direction_Radians32_007005b0(UnknownFacingDir const &direction);
inline bool Unit_Voxel_Loaded_007005b0(UnknownObjectPair const &voxel);
extern bool unknown_use_voxel_cache_839a58;
extern HouseClass *unknown_player_house_a35db4;
extern int unknown_bridge_height_ad057c;
extern void *unknown_cache_empty_b2b624;
// TibSun face.h direction accessors (twin: Current().As_Radian32())
inline unsigned UnknownFacingDir::Round_To_32() const
{
    return ((((*(unsigned const *)this)>>10)+1)>>1);
}
inline Dir32 UnknownFacingDir::As_Dir32() const
{
    return (Dir32)(Round_To_32()%32U);
}
inline double UnknownFacingDir::As_Radian32() const
{
    return (double)((As_Dir32()-8)*-0.19634954084936207);
}
inline void * const &UnknownBuffer::operator[](int const &id) const
{
    if (Is_Present_0070a350(id)) return ((NodeElement const *)unknown_10)->Data;
    return unknown_cache_empty_b2b624;
}
// ra2-decomp unit.cpp Is_Cached shape (twin-shaped helper)
static inline bool Unit_Is_Cached_006ff690(UnknownBuffer *cache,int key)
{
    if (key!=-1 && cache!=0) {
        return((*cache)[key]!=0);
    }
    return(false);
}
static inline bool Unit_Is_Barrel_Ahead_006ff690(FacingClass const &facing)
{
    return(Unit_Dir_To_4_007005b0(facing.Current())>0 && Unit_Dir_To_4_007005b0(facing.Current())<3);
}
void UnitClass::unknown_slot310(Point2D point,Rect window,int brightness)
{
    Point2D drawpoint=point;
    TechnoTypeClass *type=Type;
    if (!unknown_slot245(unknown_player_house_a35db4)) type=static_cast<TechnoTypeClass *>(unknown_slot50(true));
    int frame=unknown_430 % (int)type->unknown_b0.unknown_04->unknown_frame_count;
    Rect cliprect=window;
    Surface *old_surface=unknown_surface_83995c;
    int key=0;
    Matrix3D main_matrix;
    unknown_surface_83995c=unknown_surface_ad0604;
    drawpoint=Point2D(128,128);
    cliprect=unknown_surface_83995c->Get_Rect();
    unknown_5bb=true;
    key=unknown_use_voxel_cache_839a58 ? 0 : -1;
    {
        Matrix3D draw_result;
        Matrix3D converted(*Locomotor.Checked()->Draw_Matrix(&draw_result,&key));
        main_matrix=converted;
    }
    if (key!=-1) key=(key<<5)|(frame&31);
    int hull_facing=(key>>5)&0x3ff;
    Rect rect=cliprect;
    unknown_unit_rect_ad0490=unknown_static_rect;
    if (Type->unknown_1b8 && SpawnManager->Docked_Count()==0) {
        key|=0x10000;
        unknown_slot246(type->unknown_b8,frame,key,&type->unknown_214,rect,drawpoint,Unit_Isometric_View_00717f90()*main_matrix,brightness,0x2800);
    } else {
        unknown_slot246(type->unknown_b0,frame,key,&type->unknown_214,rect,drawpoint,Unit_Isometric_View_00717f90()*main_matrix,brightness,0x2800);
    }
    int turret_key=key;
    if (turret_key!=-1) {
        turret_key=(turret_key&~31)|unknown_2f8.Current().As_Dir32();
        if (Techno_Type_Class()->TurretOffset==0) turret_key&=0xfffffc1f;
    }
    if (!type->Turret && (!unknown_6d83e0() || !unknown_6d8460())) {
        if (Unit_Voxel_Loaded_007005b0(type->unknown_c0)) {
            main_matrix.Rotate_Y((float)-(unknown_2c8.Current().As_Radian32()-main_matrix.Get_Y_Rotation()));
            main_matrix.Translate((float)(unknown_slot231(0)->FLH.X*unknown_geometry_c),0.0f,(float)(unknown_slot231(0)->FLH.Z*unknown_geometry_c));
            unknown_slot246(type->unknown_c0,frame,turret_key,&type->unknown_250,cliprect,drawpoint,Unit_Isometric_View_00717f90()*main_matrix,brightness,0);
        }
    } else {
        int turret_index=0;
        bool has_barrel;
        if (unknown_6d83e0()) {
            turret_index=unknown_6d8470();
            turret_key|=turret_index<<16;
            has_barrel=Unit_Voxel_Loaded_007005b0(type->unknown_140[turret_index]);
        } else has_barrel=Unit_Voxel_Loaded_007005b0(type->unknown_c0);
        bool cached_turret=Unit_Is_Cached_006ff690(&type->unknown_228,turret_key);
        bool cached_barrel=has_barrel ? Unit_Is_Cached_006ff690(&type->unknown_250,turret_key) : false;
        Matrix3D turret_matrix;
        Matrix3D barrel_matrix;
        if (!cached_turret || (has_barrel && !cached_barrel)) {
            main_matrix.Translate_X((float)(type->TurretOffset*unknown_geometry_c));
            main_matrix.Rotate_Z((float)(unknown_2f8.Current().As_Radian32()-unknown_2e0.Current().As_Radian32()));
            Matrix3D barrel;
            barrel=main_matrix;
            Vector3 translation=Unit_Matrix_Translation_007005b0(main_matrix);
            barrel.Translate(-translation);
            barrel.Rotate_Y((float)-unknown_2c8.Current().As_Radian32());
            barrel.Translate(translation);
            turret_matrix=Unit_Isometric_View_00717f90()*main_matrix;
            barrel_matrix=Unit_Isometric_View_00717f90()*barrel;
        } else {
            turret_matrix=main_matrix;
            barrel_matrix=main_matrix;
        }
        bool barrel_above;
        if (unknown_6d83e0()) {
            if (Unit_Is_Barrel_Ahead_006ff690(unknown_2f8)) barrel_above=true;
            else {
                barrel_above=false;
                if (has_barrel) unknown_slot246(type->unknown_140[turret_index],0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
            }
            unknown_slot246(type->unknown_c8[turret_index],frame,turret_key,&type->unknown_228,cliprect,drawpoint,turret_matrix,brightness,0x2800);
            if (barrel_above && has_barrel) unknown_slot246(type->unknown_140[turret_index],0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
        } else {
            if (Unit_Is_Barrel_Ahead_006ff690(unknown_2f8)) barrel_above=true;
            else {
                barrel_above=false;
                if (Unit_Voxel_Loaded_007005b0(type->unknown_c0)) unknown_slot246(type->unknown_c0,0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
            }
            unknown_slot246(type->unknown_b8,frame,turret_key,&type->unknown_228,cliprect,drawpoint,turret_matrix,brightness,0x2800);
            if (barrel_above && Unit_Voxel_Loaded_007005b0(type->unknown_c0)) unknown_slot246(type->unknown_c0,0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
        }
    }
    rect=window;
    if (unknown_322>0) rect=Intersect(rect,Rect(0,0,unknown_int_839638,unknown_322-unknown_tactical_83996c->TacPixelY));
    else if (unknown_325) unknown_322=(short)(unknown_tactical_83996c->TacPixelY+point.Y+unknown_unit_rect_ad0490.Y+unknown_unit_rect_ad0490.Height-128);
    int apparent_brightness=unknown_slot254(brightness);
    if (unknown_slot203()) apparent_brightness=unknown_6d86f0(apparent_brightness);
    unknown_slot312(*old_surface,point,rect,apparent_brightness);
    Rect dirtyrect=unknown_unit_rect_ad0490;
    Surface *composite_surface=unknown_surface_83995c;
    unknown_surface_83995c=old_surface;
    unknown_5bb=false;
    if (!unknown_325) {
        Point2D shadow_drawpoint=point;
        if (Type->BalloonHover) goto plain;
        if (Type->SpeedType==1) goto plain;
        if (Type->SpeedType==2) {
plain:
            key=hull_facing;
        } else {
            key=hull_facing<<5;
            key|=(Locomotor.Checked()->Shadow_Point().Y-16)&31;
        }
        {
            Matrix3D shadow_result;
            Matrix3D converted(*Locomotor.Checked()->Shadow_Matrix(&shadow_result,&key));
            main_matrix=converted;
        }
        if (type->HunterSeeker) {
            CoordStruct coord=Position_Coord_004f3f40();
            coord.Z=Map.Get_Height_GL(coord);
            unknown_tactical_83996c->Coord_To_Pixel(coord,shadow_drawpoint);
            shadow_drawpoint+=Locomotor.Checked()->Shadow_Point();
        }
        if (unknown_37c) shadow_drawpoint.Y-=14;
        int height=Get_Height_AGL();
        CellClass *cell= &Map[Position_Coord_004f3f40()];
        if (unknown_37c && !unknown_79 && height>=unknown_bridge_height_ad057c && (cell->unknown_140&0x100) &&
            (((cell->unknown_140&0x800) && (cell->Adjacent_Cell(0).unknown_140&0x100)) ||
             (!(cell->unknown_140&0x800) && (cell->Adjacent_Cell(6).unknown_140&0x100)))) shadow_drawpoint.Y-=60;
        Surface *cache_key;   // the shadow mask surface
        if (In_Air()) cache_key=0;
        else {
            cache_key=composite_surface;
            if (Type->BalloonHover || Type->SpeedType==1 || Type->SpeedType==2) {
                if (Get_Height_AGL()<=0) key|=0x80000000;
                else key=-1;
            }
        }
        if (Get_Height_AGL()>0) {
            Draw_Voxel_Shadow(type->unknown_b0,type->ShadowIndex,key,&type->unknown_23c,window,shadow_drawpoint,Unit_Isometric_View_00717f90()*main_matrix,true,cache_key,Locomotor.Checked()->Shadow_Point().X,Locomotor.Checked()->Shadow_Point().Y+1);
        } else {
            Draw_Voxel_Shadow(type->unknown_b0,type->ShadowIndex,key,&type->unknown_23c,window,shadow_drawpoint,Unit_Isometric_View_00717f90()*main_matrix,true,cache_key,Locomotor.Checked()->Shadow_Point().X,Locomotor.Checked()->Shadow_Point().Y);
        }
    }
    composite_surface->Fill_Rect(dirtyrect,0);
}
#endif

// void UnitClass::unknown_slot67(Point2D const &point, Rect const &window) (0x00700E70, slot 67)
// TibSun UnitClass::Draw_It.
bool Unknown_0051FC50();
bool Unknown_0051FC60();
// By value: the Move_Coord result is built after radians dies and shares its slot. Move_Coord is TibSun inline.h's
// text (the radian computed inside it, as AnimClass::Flaming_Guy_AI spells it): the separate Radian helper was a
// declaration-count tie (Scout1293: exact under the Can_Deploy_Now headers only with this form).
inline bool Coord_Pixel_00700e70(CoordStruct coord, Point2D &pixel)
{
    return unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
}

inline CoordStruct Move_Coord_00700e70(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
    double radians = ((short)dir.Raw - 0x3FFF) * unknown_facing_radians_79b620;
    return CoordStruct((int)(start.X + unknown_cos_4ba4d0(radians) * distance), (int)(start.Y - unknown_sin_4ba480(radians) * distance), start.Z);
}

void UnitClass::unknown_slot67(Point2D const &input, Rect const &window)
{
    static ShapeFileStruct const *harvesting_shape = (ShapeFileStruct const *)MixFileClass::Retrieve("OREGATH.SHP");

    Point2D adjusted_point = input;
    adjusted_point.Y -= *reinterpret_cast<signed char const *>(&unknown_59e);
    Point2D raw_point = adjusted_point;
    if (unknown_37c) {
        adjusted_point.Y += 14;
    }
    Cell cell = unknown_slot104();
    if (unknown_slot25(false, 0) != 5) {
        unknown_tactical_83996c->Add_To_Selectables(this, adjusted_point);
        int height = Get_Height_AGL();
        CellClass *cptr = &Map[CoordStruct(Coord)];
        int brightness;
        if (((!unknown_79 && height >= unknown_geometry_j) || (unknown_79 && height >= 0))
            && (cptr->unknown_140 & 0x100)
            && (((cptr->unknown_140 & 0x800) && (cptr->Adjacent_Cell(0).unknown_140 & 0x100))
                || (!(cptr->unknown_140 & 0x800) && (cptr->Adjacent_Cell(6).unknown_140 & 0x100)))) {
            int level;
            if (Unknown_0051FC50()) {
                level = unknown_id_source_a3d290->IonLevel;
            } else if (Unknown_0051FC60()) {
                level = unknown_id_source_a3d290->unknown_22f4;
            } else {
                level = unknown_id_source_a3d290->AmbientLevel;
            }
            brightness = Map[cell].unknown_10a + 4 * level;
        } else {
            int shade = 0;
            if ((Map[cell].unknown_140 & 0x10000) && !In_Air()) {
                shade = -500;
            }
            brightness = Map[cell].unknown_10a + shade;
        }
        brightness += Rules->ExtraUnitLight;

        if (Type->Harvester && unknown_5ba && !Locomotor.Checked()->Is_Moving_Now()) {
            Point2D pixel;
            int face = Dir_To_8(unknown_2e0.Current());
            Coord_Pixel_00700e70(Move_Coord_00700e70(unknown_slot42(), Facing_Dir(face), 30), pixel);
            int offset = Dir_To_8(unknown_2e0.Current());
            offset = 7 - offset;
            if (unknown_20c == 0 && !unknown_slot111() && !unknown_slot112()) {
                CC_Draw_Shape(unknown_surface_83995c, Unit_Draw_Convert_008323C0, harvesting_shape,
                    (unknown_430 + Frame) % 15 + offset * 15, pixel, window, 0x2a00, 0,
                    unknown_slot176() - 2, 0, brightness, 0, 0, Point2D(0, 0));
            }
        }

        UnitTypeClass *oldclass = Type;
        if (Type->Harvester && unknown_5b9 && oldclass->UnloadingClass != 0) {
            Type = oldclass->UnloadingClass;
        }
        ObjectTypeClass *type = Type;
        if (!unknown_slot245(unknown_player_a35db4)) {
            type = unknown_slot50(true);
        }
        if (unknown_574 == -1) {
            if (type->unknown_206) {
                if (type->unknown_b0.unknown_00 != 0) {
                    unknown_slot310(adjusted_point, window, brightness);
                }
            } else {
                unknown_slot311(adjusted_point, window, brightness);
            }
        }
        Type = oldclass;
        if (unknown_5b4 != -1) {
            CC_Draw_Shape(unknown_surface_83995c, ColorSchemes[Houses[unknown_5b4]->ColorSchemeIndex]->Converter,
                (ShapeFileStruct const *)MixFileClass::Retrieve("FLAGFLY.SHP"), Frame % 14, raw_point, window, 0xa00,
                0, 0, 0, brightness, 0, 0, Point2D(0, 0));
        }
        FootClass::unknown_slot67(raw_point, window);
        if (unknown_314.Value() != 0) {
            unknown_6cc020();
        }
    }
}


#include "shared/house_adjacent.hpp"
#include "shared/mission.hpp"
#include "shared/teleport_locomotion.hpp"
#define DIST_A(o) Dist_A(this, o)
#define DIST_B(o) Dist_B(this, o)
#include "shared/tube.hpp"
#include "shared/parasite.hpp"
#include "shared/warhead_type.hpp"
#include "shared/ground_type.hpp"
#include "shared/fogged_object.hpp"
#include "shared/terrain.hpp"
#include "shared/dir_type.hpp"
#include "shared/incdec.hpp"
#include "shared/bullet.hpp"
#include "shared/aircraft.hpp"
#include "shared/map_elements.hpp"
#include "shared/ccini_class.hpp"
#include "shared/techno_map.hpp"
#include "shared/crt_memory.hpp"
#include "shared/terrain_type.hpp"
#include "shared/string_table.hpp"


// One harvesting step. Operation names beyond shared types remain hypotheses.
bool UnitClass::unknown_7013a0()
{
    CoordStruct position = Coord;
    CellClass *cell = &Map[position];
    if (unknown_494) {
        return true;
    }
    if (Type->Harvester && unknown_slot165() < 1.0 && cell->unknown_ec == 5) {
        if (Type->Weeder) {
            cell->unknown_47c040();
            unknown_294.Add_Amount(1.0f, 0);
            StageClass::Stage = 0;
            StageClass::Set_Rate(Rules->unknown_1180 * 3);
            goto harvested;
        }
        int resource = cell->Tiberium_Type_Here();
        float capacity = (float)Type->Storage;
        int available = (int)(capacity - unknown_294.Get_Total_Amount() > 1.0f
            ? 1.0f : (float)Type->Storage - unknown_294.Get_Total_Amount());
        int taken = cell->unknown_475db0(available);
        if (taken > 0) {
            unknown_294.Add_Amount((float)taken, resource);
            StageClass::Stage = 0;
            StageClass::Set_Rate(Rules->unknown_1180);
            goto harvested;
        }
        return false;
    }
    StageClass::Stage = 0;
    StageClass::Set_Rate(0);
    return false;
harvested:
    return true;
}


// Original RTTI vtable79D0CC raw descriptor .?AV?$DynamicVectorClass@PBVParticleSystemTypeClass@@@@ proves const pointee.
// Do_MISSION_UNLOAD (TibSun unit.cpp): passengers leave a transport, harvesters dump at the refinery, MCVs deploy.

int UnitClass::Do_MISSION_UNLOAD()
{
    Cell cell;
    if (Type->Passengers>0) {
        switch (unknown_a8) {
            case 0: {
                if (Locomotor.Checked()->Is_Moving()) return 10;
                Cell current=unknown_slot104();
                CellClass &current_cell=Map[current];
                if (!unknown_494 && current_cell.unknown_ec==2) {
                    Cell destination=Map.Nearby_Location(unknown_slot104(),2,-1,0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                    Assign_Destination(&Map[destination],true);
                    return 10;
                }
                int passengers=Cargo.Count;
                int facing=unknown_slot182(0,&cell);
                if (passengers && cell!=unknown_geometry_k) {
                    if (unknown_6d83e0()) {
                        if (passengers==1) unknown_5cc=0;
                        else unknown_5cc=1;
                    }
                    UnknownFacingDir direction((unsigned short)(facing<<13));
                    Locomotor.Checked()->Do_Turn(direction);
                    unknown_a8=1;
                    return 1;
                }
                QueueMission(5,false);
                break;
            }
            case 1:
            if (!unknown_599) { unknown_a8=3;return 1; }
            break;
            case 3:
            if (Cargo.Count>unknown_5cc) {
                FootClass *passenger=unknown_4ccfc0();
                if (passenger) {
                    UnknownFacingDir opposite((unsigned short)(unknown_2e0.Current().Raw+0x7fff));
                    int attempt=0;
                    int start=((*(unsigned const *)&opposite>>12)+1)>>1;
                    bool prefer_far=true;
                    bool placed=false;
                    for (;attempt<8;++attempt) {
                        int facing=(start+attempt)&7;
                        Cell near_cell=Adjacent_Cell(unknown_slot104(),(FacingType)facing);
                        Cell far_cell=Adjacent_Cell(near_cell,(FacingType)facing);
                        bool far_allowed=passenger->Can_Enter_Cell(&Map[far_cell],facing,unknown_5d47d0(),0,1)==0;
                        bool near_allowed=passenger->Can_Enter_Cell(&Map[near_cell],facing,unknown_5d47d0(),0,1)==0;
                        if (!near_allowed || (!far_allowed && prefer_far)) {
                            if (prefer_far && attempt+1==8) { attempt=0;prefer_far=false; }
                            continue;
                        }
                        if (Map[near_cell].unknown_140&0x100) continue;
                        ++unknown_a40774;
                        CoordStruct position=Cell_Coord(near_cell);
                        position=Map.Unknown_0049CB70(position,false);
                        UnknownFacingDir direction((unsigned short)(facing<<13));
                        placed=passenger->Unlimbo(&position,Dir_To_256(direction));
                        --unknown_a40774;
                        if (placed) {
                            passenger->QueueMission(2,false);
                            if (prefer_far) passenger->Assign_Destination(&Map[far_cell],true);
                            else passenger->Assign_Destination(&Map[near_cell],true);
                            if (Team) Team->Add(passenger);
                        }
                        break;
                    }
                    if (!placed) { Cargo.Attach(passenger);passenger->Hidden(); }
                }
            } else unknown_a8=4;
            break;
            case 4:
            QueueMission(5,false);
            unknown_a4=true;
            break;
        }
        int delay=Control()->Normal_Delay();
        return delay+Random_Pick(0,2);
    }
    if (!Type->Harvester && !Type->Weeder) {
        if (Type->DeploysInto) {
            switch (unknown_a8) {
                case 2:
                if (!unknown_57c) {
                    unknown_slot261(false,true);
                    Commence();
                } else if (!unknown_6fdfc0() && unknown_494) unknown_57c=false;
                break;
                case 0:
                unknown_slot149(3);
                Path[0]=-1;
                unknown_a8=1;
                // Initial setup falls through into the movement check.
                case 1:
                if (!Locomotor.Checked()->Is_Moving()) {
                    unknown_6fdfc0();
                    if (unknown_7d) {
                        if (unknown_57c) unknown_a8=2;
                        else if (!House->Unknown_004F52F0() && unknown_game_mode_a3d298) QueueMission(15,false);
                        else QueueMission(5,false);
                    }
                } else if (MissionQueue!=-1 && MissionQueue!=16) Commence();
                break;
                default: break;
            }
        } else {
            if (Type->IsSimpleDeployer) {
                QueueMission(5,false);
                Commence();
            }
            return FootClass::Do_MISSION_UNLOAD();
        }
        int delay=Control()->Normal_Delay();
        return delay+Random_Pick(0,2);
    }
    if (!HasAnyLink()) {
        unknown_slot261(false,true);
        unknown_5b9=false;
        if (Locomotor.Checked()->Is_Moving()) Stop_Driver();
        if (unknown_slot122()) Commence();
        return 1;
    }
    int current_dir=Dir_To_256(unknown_2e0.Current());
    if (current_dir!=64) {
        if (!unknown_599) Locomotor.Checked()->Do_Turn(UnknownFacingDir(0x4000));
        return 5;
    }
    if (!unknown_5b9) {
        unknown_5b9=true;
        StageClass::Stage=0;
        StageClass::Set_Rate(1);
        if (Type->Harvester) {
            BuildingClass *building=(BuildingClass *)Map[unknown_slot104()+AdjacentCell[6]].Cell_Building();
            if (building) building->unknown_44c920(7,building->Health_Ratio()<=Rules->unknown_1368,false,0);
        }
        unknown_a8=3;
    } else {
        switch (unknown_a8) {
            case 4: {
                if (Type->Weeder) {
                    unknown_5b9=false;
                    if (unknown_494 && MissionQueue!=-1 && MissionQueue!=10) {
                        if (Locomotor.Checked()->Is_Moving()) Stop_Driver();
                        if (unknown_slot122()) Commence();
                    } else {
                        QueueMission(10,false);
                        if (unknown_slot122()) { if (HasAnyLink()) unknown_slot149(3);Commence(); }
                    }
                } else {
                    BuildingClass *building=(BuildingClass *)Map[unknown_slot104()+AdjacentCell[6]].Cell_Building();
                    bool active=false;
                    if (building && building->Type->Refinery) active=building->unknown_454[8]!=0;
                    if (active) return 1;
                    unknown_5b9=false;
                    if (unknown_494 && MissionQueue!=-1 && MissionQueue!=10) {
                        if (Locomotor.Checked()->Is_Moving()) Stop_Driver();
                        if (unknown_slot122()) Commence();
                    } else {
                        QueueMission(10,false);
                        if (unknown_slot122()) { if (HasAnyLink()) unknown_slot149(3);Commence(); }
                    }
                }
                break;
            }
            case 3: {
                BuildingClass *building=(BuildingClass *)Map[unknown_slot104()+AdjacentCell[6]].Cell_Building();
                if (!building) {
                    if (HasAnyLink()) unknown_slot149(3);
                    QueueMission(10,true);
                    int delay=Control()->Normal_Delay();
                    return delay+Random_Pick(0,2);
                } else {
                    if (StageClass::Stage>=Rules->HarvesterDumpRate*900.0) {
                        DynamicVectorClass<ParticleSystemTypeClass const *> smoke;
                        for (int n=building->Techno_Type_Class()->DamageParticleSystems.Count()-1;n>=0;--n) {
                            if (building->Techno_Type_Class()->DamageParticleSystems[n]->BehavesLike==0) smoke.Add(building->Techno_Type_Class()->DamageParticleSystems[n]);
                        }
                        if (!building->unknown_25c[5] && smoke.Count()>0 && Get_Height_AGL()>-10) {
                            TechnoTypeClass *type=building->Techno_Type_Class();
                            CoordStruct &o=type->RefnSmokeOffsetOne; CoordStruct offset=o;
                            building->unknown_25c[5]=new ParticleSystemClass(const_cast<ParticleSystemTypeClass *>(smoke[unknown_id_source_a3d290->Random()(0,smoke.Count()-1)]),building->Position_Coord_004f3f40()+offset,0,building,unknown_geometry_l);
                        }
                        if (!building->unknown_25c[6] && smoke.Count()>0 && Get_Height_AGL()>-10) {
                            TechnoTypeClass *type=building->Techno_Type_Class();
                            CoordStruct &o=type->RefnSmokeOffsetTwo; CoordStruct offset=o;
                            building->unknown_25c[6]=new ParticleSystemClass(const_cast<ParticleSystemTypeClass *>(smoke[unknown_id_source_a3d290->Random()(0,smoke.Count()-1)]),building->Position_Coord_004f3f40()+offset,0,building,unknown_geometry_l);
                        }
                        if (!building->unknown_454[10]) building->unknown_44c920(10,building->Health_Ratio()<=Rules->unknown_1368,false,0);
                        int slot=unknown_294.First_Used_Slot();
                        HouseClass *owner=building->Owning_House();
                        int purifiers=owner->unknown_52cc;
                        if (owner->Power_Fraction()<1.0) purifiers=0;
                        if (!owner->unknown_134 && unknown_game_mode_a3d298!=0) purifiers+=Rules->AIVirtualPurifiers[owner->unknown_c8];
                        float amount=slot!=-1 ? unknown_294.Get_Amount(slot) : 0.0f;
                        float bonus=(float)purifiers*Rules->PurifierBonus*amount;
                        float removed;
                        if (slot!=-1 && (removed=unknown_294.Remove_Amount(amount,slot))>0.0f) {
                            if (Type->Weeder) {
                                House->Place_Tiberium((int)removed,slot);
                                StageClass::Stage=0;
                            } else {
                                owner->Harvested(removed,slot);
                                if (bonus>0.0f) owner->Harvested(bonus,slot);
                                StageClass::Stage=0;
                            }
                        } else {
                            if (building->Type->Refinery) building->unknown_44c920(8,building->Health_Ratio()<=Rules->unknown_1368,false,0);
                            unknown_a8=4;
                            if (building->unknown_25c[5]) building->unknown_25c[5]->Delete_Me();
                            if (building->unknown_25c[6]) building->unknown_25c[6]->Delete_Me();
                            if (building->unknown_454[10]) building->unknown_44cfd0(10);
                        }
                    }
                    if (unknown_494 && MissionQueue!=-1 && MissionQueue!=10) {
                        if (building->Type->Refinery) building->unknown_44c920(8,building->Health_Ratio()<=Rules->unknown_1368,false,0);
                        unknown_a8=4;
                        if (building->unknown_25c[5]) building->unknown_25c[5]->Delete_Me();
                        if (building->unknown_25c[6]) building->unknown_25c[6]->Delete_Me();
                        if (building->unknown_454[10]) building->unknown_44cfd0(10);
                    }
                    return 1;
                }

                break;
            }
            default: break;
        }
    }
    int delay=Control()->Normal_Delay();
    return delay+Random_Pick(0,2);
}


// int UnitClass::Do_MISSION_HARVEST() (0x007025A0, slot 131)
// TibSun UnitClass::Mission_Harvest: the harvester state machine (looking,
// harvesting, find home, heading home, going idle). Goes to guard when no dock
// building type is owned.

ILocomotion *__fastcall Unit_Checked_Locomotor_004cde90(UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600> const &owner);

class UnknownBuildingView_007025a0 : public TechnoClass {
public:
    BuildingTypeClass *Type;
};

inline CoordStruct Coord_Delta_007025a0(CoordStruct const &a, CoordStruct const &b)
{
    return CoordStruct(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
inline int Length_007025a0(CoordStruct const &coord)
{
    double x = coord.X;
    double y = coord.Y;
    double z = coord.Z;
    return (int)unknown_sqrt_4ba410(z * z + y * y + x * x);
}
inline int Dist_A(ObjectClass const *self, ObjectClass const *other)
{
    return Length_007025a0(Coord_Delta_007025a0(self->Center_Coord(), other->Center_Coord()));
}
inline int Dist_B(ObjectClass const *self, ObjectClass const *other)
{
    return Offset_Length(Coord_Delta_007025a0(self->Center_Coord(), other->Center_Coord()));
}

int UnitClass::Do_MISSION_HARVEST()
{
    enum { LOOKING, HARVESTING, FINDHOME, HEADINGHOME, GOINGTOIDLE };
    int index;

    if (!Type->Harvester && !Type->Weeder) {
        return 450;
    }
    if (Type->Dock.Count() == 0 && !House->Is_Human_Player()) {
        QueueMission(5, false);
        return 1;
    }
    bool found = false;
    for (index = 0; index < Type->Dock.Count(); index++) {
        if (House->unknown_5420.unknown_func_491d10(Type->Dock[index]->ArrayIndex) > 0) {
            found = true;
            break;
        }
    }
    if (!found) {
        QueueMission(5, false);
        return 1;
    }
    {
        bool teleporter = Type->Teleporter;
        switch (unknown_a8) {
        case LOOKING:
            if (!Type->Weeder && unknown_slot165() >= 1.0) {
                unknown_a8 = FINDHOME;
                return 1;
            }
            {
                bool hastarget = true;
                if (unknown_1b0 != 0) {
                    Assign_Destination(unknown_1b0, true);
                    unknown_6d6e50(0);
                    hastarget = false;
                }
                unknown_5ba = false;
                bool ok;
                if (Type->Weeder) {
                    ok = unknown_4cc400(Rules->TiberiumFarScan / 256);
                } else {
                    UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist;
                    if (Locomotor) {
                        HRESULT status = persist.Query_Held(Unit_Checked_Locomotor_004cde90(Locomotor));
                        if (FAILED(status) && status != E_NOINTERFACE) {
                            _com_issue_error(status);
                        }
                    }
                    GUID clsid;
                    persist.Checked()->GetClassID(&clsid);
                    if (memcmp(&clsid, &CLSID_TeleportLocomotionClass, sizeof(GUID)) == 0 && unknown_494 != 0) {
                        Assign_Destination(0, true);
                    }
                    ok = unknown_4cb830(Rules->TiberiumFarScan / 256, hastarget);
                }
                if (ok) {
                    unknown_5ba = true;
                    StageClass::Set_Rate(2);
                    StageClass::Stage = 0;
                    unknown_a8 = HARVESTING;
                    return 1;
                }
                if (unknown_494 == 0) {
                    if (unknown_1b0 != 0) {
                        Assign_Destination(unknown_1b0, true);
                    } else {
                        unknown_a8 = GOINGTOIDLE;
                        unknown_328 = true;
                        if (Type->Harvester) {
                            House->unknown_189 = true;
                        }
                        return 105;
                    }
                } else {
                    unknown_328 = false;
                }
            }
            break;
        case HARVESTING:
            if (StageClass::Rate == 0) {
                StageClass::Stage = 0;
                StageClass::Set_Rate(Rules->unknown_1180);
            }
            if (StageClass::Stage < 9) {
                return 1;
            }
            if (!unknown_7013a0()) {
                unknown_5ba = false;
                if (Type->Harvester && unknown_slot165() == 1.0) {
                    unknown_a8 = FINDHOME;
                    Cell cell;
                    if (Type->Harvester) {
                        cell = unknown_4cb8f0(Rules->TiberiumNearScan / 256, 0);
                    } else {
                        cell = unknown_4cc100(Rules->TiberiumNearScan / 256);
                    }
                    if (cell != unknown_geometry_k) {
                        unknown_6d6e50(&Map[cell]);
                    } else {
                        unknown_6d6e50(0);
                    }
                    return 1;
                }
                bool ok;
                if (Type->Harvester) {
                    ok = unknown_4cb830(Rules->TiberiumNearScan / 256, false);
                } else {
                    ok = unknown_4cc400(Rules->TiberiumNearScan / 256);
                }
                if (!ok && unknown_494 == 0) {
                    unknown_6d6e50(0);
                    unknown_a8 = FINDHOME;
                } else {
                    unknown_a8 = HARVESTING;
                    unknown_5ba = true;
                }
                return 1;
            }
            return 1;
        case FINDHOME:
            if (unknown_494 != 0 && teleporter && unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0)) {
                unknown_4cd9b0();
            }
            if (unknown_494 == 0) {
                TechnoClass *nearest = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
                if (((!teleporter && nearest != 0 && DIST_A(nearest) <= Rules->HarvesterTooFarDistance * 256)
                    || (teleporter && nearest != 0 && DIST_A(nearest) <= Rules->ChronoHarvTooFarDistance * 256))
                    && unknown_slot150(2, nearest) == 1) {
                    unknown_a8 = HEADINGHOME;
                    break;
                }
                unknown_a40774++;
                nearest = unknown_slot299((DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 1);
                unknown_a40774--;
                if (nearest != 0) {
                    if (DIST_B(nearest) > 0x300 || teleporter) {
                        Cell cell = Coord_Cell(CoordStruct(nearest->Coord));
                        BuildingTypeClass *dock = ((UnknownBuildingView_007025a0 *)nearest)->Type;
                        Cell here = cell;
                        here.X = dock->QueueingCell.X + cell.X;
                        here.Y += dock->QueueingCell.Y;
                        here = Map.Nearby_Location(here, 2, -1, 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
                        if (!(here == unknown_geometry_k)) {
                            Assign_Destination(&Map[here], true);
                        } else {
                            Assign_Destination(0, true);
                        }
                    }
                }
            }
            break;
        case HEADINGHOME:
            QueueMission(7, false);
            return 1;
        case GOINGTOIDLE:
            if (unknown_328) {
                if (House->unknown_5420.unknown_func_491d10(Rules->unknown_778->ArrayIndex) > 0) {
                    QueueMission(0x14, false);
                } else {
                    QueueMission(0xf, false);
                }
            }
            {
                ObjectClass *object = Map[CoordStruct(Coord)].Cell_Building();
                if (object != 0) {
                    BuildingTypeClass *type = ((UnknownBuildingView_007025a0 *)object)->Type;
                    if (type->Refinery || type->Weeder) {
                        Assign_Destination(&Map[unknown_6cf130(object)], true);
                    }
                }
            }
            QueueMission(5, false);
            break;
        }
        int delay = (int)(Control()->unknown_10 * 900.0);
        return delay + Random_Pick(0, 2);
    }
}


// int UnitClass::Do_MISSION_HUNT() (0x00702F60, slot 132)
// The mission step of a unit that deploys into a building (when that
// building type is listed at rules +0x7C8, or the unit has a target, or the
// owner is human): status 0 becomes 1 when 0x006FD930 and 0x006FDFC0 allow
// it, status 1 falls back to 0 without +0x57C. Answers the mission's delay
// plus 0..2 frames; other units take the foot behaviour.

int UnitClass::Do_MISSION_HUNT()
{
    BuildingTypeClass *building = Type->DeploysInto;
    if (building && (Is_Listed(Rules->unknown_7c8, building) || unknown_228 || House->Unknown_004F52F0())) {
        switch (unknown_a8) {
        case 0:
            if (unknown_6fd930() && unknown_6fdfc0()) {
                unknown_a8 = 1;
            }
            break;
        case 1:
            if (!unknown_57c) {
                unknown_a8 = 0;
            }
            break;
        }
        int delay = (int)(Control()->unknown_10 * 900.0);
        return delay + unknown_id_source_a3d290->Random()(0, 2);
    }
    return FootClass::Do_MISSION_HUNT();
}


// int UnitClass::Can_Enter_Cell(CellClass *cell, int direction, int height, CellClass *adjacent, bool unused) (0x00703040)
// TibSun UnitClass::Can_Enter_Cell.
extern "C" int __cdecl abs(int);
TubeClass *__fastcall Cell_Tube_0047a130(CellClass const *cell);
TechnoClass *__fastcall As_Techno_0040dbd0(AbstractClass *object);
struct UnknownIsoTile {
    char unknown_000[0x2B4];
    int Width;                  // +0x2B4
    int Height;                 // +0x2B8
};
extern DynamicVectorClass<UnknownIsoTile *> unknown_iso_tiles_a40cd0;
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
inline UnknownFacingDir Direction_00703040(CoordStruct const &coord1,CoordStruct const &coord2)
{
    int x1=coord1.X;
    int x2=coord2.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)coord1.Y-(double)coord2.Y,(double)x2-(double)x1)-unknown_facing_origin_79b630)*unknown_facing_scale_79b628));
}
inline int Distance_00703040(CoordStruct const &coord1,CoordStruct const &coord2)
{
    CoordStruct diff=coord1-coord2;
    double y=diff.Y;
    double z=diff.Z;
    double x=diff.X;
    return (int)Fast_Sqrt(y*y+z*z+x*x);
}
struct CellOccupancyFlags_00703040 { unsigned int unknown_0:5; unsigned int vehicle:1; unsigned int unknown_6:26; };

int UnitClass::Can_Enter_Cell(CellClass *cell,int direction,int height,CellClass *adjacent,bool unused)
{
    bool bridge=(cell->unknown_140&0x100) && (height==-1 || abs(height-cell->unknown_11b)>1);
    unsigned char flags=cell->unknown_124;
    bool vehicle=reinterpret_cast<CellOccupancyFlags_00703040 const &>(cell->unknown_124).vehicle!=0;
    int infantry_owner=cell->unknown_54;
    TubeClass *tube=Cell_Tube_0047a130(cell);
    if (Type->MovementRestrictedTo!=-1) {
        if (cell->unknown_ec==10) {
            UnknownIsoTile *tile=unknown_iso_tiles_a40cd0[cell->IsoTileIndex];
            if ((tile->Width==5 && tile->Height==3) || (tile->Width==4 && tile->Height==3)) {
                if (cell->unknown_11a!=2) return 7;
            } else if ((tile->Width==3 && tile->Height==4 || tile->Width==3 && tile->Height==5) && cell->unknown_11a!=6) {
                return 7;
            }
        }
        if (cell->unknown_ec!=Type->MovementRestrictedTo && cell->unknown_ec!=10) {
            if (cell->OverlayIndex<0xED || cell->OverlayIndex>0xEE || height==cell->unknown_11b) return 7;
        }
    }
    if (direction==8) {
        if (tube) {
            Cell exit=tube->ExitCell;
            if (exit!=Cell(0,0)) return 0;
        }
        return 7;
    }
    if (tube) {
        int angle=abs(direction-tube->Facing);
        if (angle>2 && angle<6 && direction!=-1) return 7;
    }
    unsigned opposite=(direction-4)&7;
    TubeClass *other_tube=Cell_Tube_0047a130(&cell->Adjacent_Cell(opposite));
    if (other_tube) {
        int angle=abs(opposite-other_tube->Facing);
        if (angle>2 && angle<6 && direction!=-1) return 7;
    }
    int result=unknown_slot102(reinterpret_cast<UnknownCellClass*>(cell),direction,&height,&bridge,reinterpret_cast<UnknownCellClass*>(adjacent));
    if (result==7) return 7;
    if (height!=-1 && (cell->unknown_140&0x100) && height==cell->unknown_11b+4) {
        flags=cell->unknown_128;
        vehicle=reinterpret_cast<CellOccupancyFlags_00703040 const &>(cell->unknown_128).vehicle!=0;
        infantry_owner=cell->unknown_58;
    }
    if (!unknown_a40774 && !Map.Unknown_0055A740(cell,true) && !unknown_slot189() && unknown_32d) return 7;
    result=FootClass::Can_Enter_Cell(cell,direction,height,adjacent,unused);
    if (result==7) return 7;
    if (cell->OverlayIndex!=-1) {
        OverlayTypeClass *overlay=OverlayTypes[cell->OverlayIndex];
        if (overlay->Crate && !House->Is_Human_Player() && unknown_game_mode_a3d298==0) return 7;
        if (overlay->Wall) {
            if (overlay->unknown_1fd && (Type->Crusher || unknown_6d7910(0x11))) {
                if (House->Is_Ally(cell->unknown_50)) {
                    if (result<4) result=4;
                }
            } else {
                if (unknown_slot163()) {
                    WarheadTypeClass *warhead=unknown_slot231(0)->WeaponType->Warhead;
                    if (warhead->Wall || (warhead->Wood && overlay->unknown_9c==6)) {
                        if (House->Is_Ally(cell->unknown_50)) {
                            if (result<4) result=4;
                        } else {
                            if (result<5) result=5;
                        }
                    } else {
                        return 7;
                    }
                } else {
                    return 7;
                }
            }
        }
    }
    bool crushable=false;
    ObjectClass *occupant=bridge ? cell->AltOccupierPtr : cell->OccupierPtr;
    while (occupant) {
        if (this!=occupant) {
            if (unknown_588 && unknown_588->unknown_28) {
                if (occupant->unknown_slot104()==unknown_588->unknown_28->unknown_slot104()) return 0;
            }
            TechnoClass *contact=occupant->AbstractFlags.IsTechno ? static_cast<TechnoClass*>(occupant) : 0;
            if (ContainsLink(contact) && occupant->Kind_Of()==6 && !static_cast<BuildingClass*>(occupant)->Cell_Is_Impassable_Row_00453af0(cell)) {
                occupant=occupant->unknown_30;
                continue;
            }
            if (Type->SmallVisceroid && occupant->Kind_Of()==1 && static_cast<UnitClass*>(occupant)->Type->SmallVisceroid) return 0;
            if (occupant->Kind_Of()==6) {
                BuildingClass *building=static_cast<BuildingClass*>(occupant);
                if (building->Type->Gate) {
                    if (!building->unknown_44d880()) {
                        if (building->House->Is_Ally(House)) {
                            if (result<3) result=3;
                        } else {
                            if (!unknown_slot163()) return 7;
                            if (result<5) result=5;
                        }
                    }
                    occupant=occupant->unknown_30;
                    continue;
                }
                if (direction!=-1 && building->Type->UnitRepair && cell->Cell_Building()==building && !building->Cell_Is_Impassable_Row_00453af0(cell)) {
                    occupant=occupant->unknown_30;
                    continue;
                }
                if (building->Type->InvisibleInGame) {
                    occupant=occupant->unknown_30;
                    continue;
                }
                if (building->Type->LaserFence && (building->unknown_4ec==12 || building->unknown_4ec==8)) {
                    occupant=occupant->unknown_30;
                    continue;
                }
                if (building->Type->FirestormWall) {
                    if (building->House->unknown_142) return 7;
                    occupant=occupant->unknown_30;
                    continue;
                }
                if (building->Type->Bib) {
                    if (Map[Adjacent_Cell(cell->MapCoords,2)].Cell_Building()!=building) {
                        occupant=occupant->unknown_30;
                        continue;
                    }
                }
            }
            if (Get_Mission()==7 && occupant==unknown_494 && occupant->Kind_Of()==1) return 0;
            if (Get_Mission()==11 && unknown_1b0==occupant) return 7;
            FootClass *foot;
            if (occupant->AbstractFlags.IsFoot) {
                foot=static_cast<FootClass*>(occupant);
            } else {
                foot=0;
            }
            bool is_moving=foot!=0 && (foot->unknown_494 || foot->unknown_2e0.Is_Rotating() || foot->Locomotor.Checked()->Is_Moving());
            if (House->Is_Ally(static_cast<TechnoClass*>(occupant))) {
                if (is_moving) {
                    int face=Dir_To_8(unknown_2e0.Current());
                    UnknownFacingDir reverse=UnknownFacingDir((unsigned short)(static_cast<FootClass*>(occupant)->unknown_2e0.Current().Raw+0x7FFF));
                    int techface=Dir_To_8(reverse);
                    int &tech=techface;
                    UnknownFacingDir direct=Direction_00703040(Position_Coord_004f3f40(),occupant->Position_Coord_004f3f40());
                    if (face==tech && Distance_00703040(Center_Coord(),occupant->Center_Coord())<=0x1FF && Dir_To_8(direct)==face) return 7;
                    if ((foot->unknown_5a0 && foot->Kind_Of()!=15) || foot->Locomotor.Checked()->Will_Jump_Tracks()) {
                        if (result<2) result=2;
                    }
                } else {
                    if (occupant->Kind_Of()==6) return 7;
                    if (result<6) result=6;
                }
            } else {
                TechnoClass *techno=As_Techno_0040dbd0(occupant);
                if (techno && techno->unknown_1b8==2) {
                    if (result<1) result=1;
                } else {
                    ObjectTypeClass *otype;
                    if ((Type->Crusher || unknown_6d7910(0x11)) && (otype=occupant->Class_Of())->unknown_1fd) {
                        if (!House->Is_Ally(static_cast<TechnoClass*>(occupant))) crushable=true;
                    } else {
                        if (!unknown_slot231(0)->WeaponType && !Type->IsTrain) return 7;
                        switch (occupant->Kind_Of()) {
                        case 36: {
                            int which=unknown_slot174(occupant);
                            WeaponTypeClass *weapon=unknown_slot231(which)->WeaponType;
                            if (weapon==0 || weapon->Warhead==0 || !weapon->Warhead->Wood) return 7;
                        }
                        default:
                            if (result<5) result=5;
                            break;
                        case 6:
                            if (static_cast<BuildingClass*>(occupant)->Type->BridgeRepairHut) return 7;
                            if (result<5) result=5;
                            break;
                        }
                    }
                }
            }
        } else {
            vehicle=false;
            flags&=~0x20;
        }
        occupant=occupant->unknown_30;
    }
    if (!bridge && Ground[cell->unknown_ec].Cost[Type->SpeedType]==0) return 7;
    if (result==0 && !crushable && (flags&0x3F)) {
        if (vehicle) {
            result=2;
        } else {
            if (infantry_owner!=-1 && House->Is_Ally(infantry_owner)) {
                result=2;
            } else {
                if (!Type->Crusher && !unknown_6d7910(0x11)) {
                    if (Type->IsTrain || (unknown_slot231(0)->WeaponType!=0 && unknown_slot231(0)->WeaponType->Projectile->AG)) {
                        result=5;
                    } else {
                        return 7;
                    }
                }
            }
        }
    }
    if (result==0 && crushable && vehicle) {
        if (!cell->Cell_Unit() || !static_cast<UnitClass*>(cell->Cell_Unit())->Type->unknown_1fd) return 2;
    }
    return result;
}


// int UnitClass::unknown_slot28(ObjectClass *object, bool arg2) (0x00703BD0, slot 28)
// TibSun UnitClass::What_Action(ObjectClass const *, bool): the action for a clicked object.
// Action numbers are ActionType values as in the Foot answer: 4 self, 5 attack, 7 select,
// 0x1E no deploy, 0x1D repair, 0x1F enter. A deploying unit checks its building footprint
// with its own occupation lifted; a loaded unit checks the passenger can stand in the cell.
TechnoClass *__fastcall As_Techno_0040dbd0(AbstractClass *object);
unsigned char __fastcall Infantry_Action_Allowed_00614b50(int action);

// The cell's land type as an inline, so its read is not shared across the passenger's virtual
// call (shared/cell_class.hpp has this member; shared/cell.hpp does not).
inline int Land_Type_00703bd0(CellClass const *cell) { return cell->unknown_ec; }
// A building's type (a view: BuildingClass is not declared in this unit).
class UnknownBuildingView_00703bd0 : public TechnoClass {
public:
    BuildingTypeClass *Type;
};

int UnitClass::unknown_slot28(ObjectClass *object, bool arg2)
{
    int action = FootClass::unknown_slot28(object, arg2);
    if (action == 8) {
        return 8;
    }
    if (unknown_6c16f0(-1) < 0 && House->Is_Player_Control() && action != 4) {
        if (House->Is_Ally((TechnoClass *)object)) {
            if (object->unknown_slot31() && object != this && object->Not_Underground()) {
                if (object->Kind_Of() != 2 || Map[object->Center_Coord()].Cell_Building() == 0) {
                    if (object->Health_Ratio() < Rules->unknown_1360) {
                        action = 0x1d;
                    }
                }
            } else if (object->Kind_Of() != 6) {
                action = 7;
            }
        } else {
            action = 0;
        }
    }
    if (action == 5) {
        if (!Can_Player_Fire()) {
            if ((Type->Crusher || unknown_6d7910(0x11)) && object->Class_Of()->unknown_1fd) {
                action = 1;
            } else {
                action = 7;
            }
        }
    } else if (action == 4) {
        if (Infantry_Action_Allowed_00614b50(action)) {
            if (Type->DeploysInto != 0) {
                Cell cell = Cell(Center_Coord());
                for (int index = 0; index < Rules->unknown_7c8.Count(); index++) {
                    if (Rules->unknown_7c8[index] == Type->DeploysInto) {
                        cell = cell + AdjacentCell[7];
                        break;
                    }
                }
                Mark(0);
                Locomotor.Checked()->Mark_All_Occupation_Bits(0);
                if (!Type->DeploysInto->Legal_Placement(cell, 0)) {
                    action = 0x1e;
                }
                Locomotor.Checked()->Mark_All_Occupation_Bits(1);
                Mark(1);
            } else if (!Type->IsSimpleDeployer) {
                if (Type->Passengers > 0) {
                    if (Cargo.Count == 0) {
                        action = 0x1e;
                    } else if (!unknown_79) {
                        CellClass *cell = &Map[CoordStruct(Coord)];
                        ObjectClass *passenger = Cargo.Attached_Object();
                        if (passenger != 0 && Ground[Land_Type_00703bd0(cell)].Cost[passenger->Techno_Type_Class()->SpeedType] < 0.01 && Land_Type_00703bd0(cell) != 2) {
                            action = 0x1e;
                        }
                        if (Type->SpeedType != 3 && Get_Height_AGL() != 0) {
                            action = 0x1e;
                        }
                    }
                } else {
                    action = 0;
                }
            }
            if (action == 4 && !unknown_slot186()) {
                action = 0x1e;
            }
        } else {
            action = 0x1e;
        }
    }
    TechnoClass *techno = As_Techno_0040dbd0(object);
    if (House->Is_Player_Control() && techno != 0 && techno->House->Is_Ally(this)) {
        if (object->Kind_Of() == 6 && unknown_slot150(0xF, (TechnoClass *)object) == 1) {
            action = 3;
        }
    }
    if (House->Is_Player_Control() && action == 7) {
        if (object->Kind_Of() == 6) {
            UnknownBuildingView_00703bd0 *building = (UnknownBuildingView_00703bd0 *)object;
            if (building->Type->UnitRepair && unknown_slot150(0xF, building) == 1 && !building->HasAnyLink() && building->Cargo.Object == 0) {
                action = 1;
            }
        }
    } else if (action == 5) {
        if (!unknown_slot163()) {
            action = 0x3B;
        }
    }
    if (action != 0x1F) {
        if (House->Is_Ally((TechnoClass *)object) && House->Is_Player_Control() && As_Techno_0040dbd0(object) != 0 && action != 5 && action != 0x1D && action != 0x1A) {
            TechnoTypeClass *type = object->Techno_Type_Class();
            if (object != this && type != 0 && type->Passengers > 0) {
                if (object != 0 && object->AbstractFlags.IsFoot && static_cast<FootClass *>(object)->Locomotor.Checked()->Is_Moving()) {
                    action = 0x1F;
                } else {
                    switch (unknown_slot150(0xF, (TechnoClass *)object)) {
                    case 1:
                        action = 3;
                        break;
                    case 10:
                        action = 0x1F;
                        break;
                    }
                }
            }
        }
    }
    if (action == 0 && House->Is_Player_Control()) {
        action = 2;
    }
    if (action == 5 && !unknown_slot185()) {
        action = 0;
    }
    if (action == 2) {
        if (object->unknown_slot48() && object->unknown_slot51(true) == 0) {
            if (object->unknown_slot50(true)->Kind_Of() == 0x15) {
                action = 1;
            }
        }
    } else if (action == 5 && object->Class_Of()->unknown_203) {
        action = 2;
    }
    return action;
}


// int UnitClass::unknown_slot27(Cell const *where, bool arg2, bool arg3) (0x007041F0, slot 27)
// The action for a cell (hypothesis: What_Action): arg1 points at the
// cell, arg2 asks to look through the fog. Only for player-controlled
// units. Starts from the foot's answer (an unarmed unit's 5 becomes 59);
// on a visible cell a harvester answers 6 for tiberium land or fogged
// tiberium overlay, a weeder 6 for vein land or fogged veins (overlay
// 0x7E, data >= 0x30); actions 1 then give 0x23/0x24 for cells with
// 0x00479CC0 and 0 for units with +0x5C8; an action 5 the unit cannot
// take (slot 185) becomes 0.

class OverlayTypeClass;
extern DynamicVectorClass<OverlayTypeClass *> OverlayTypes;
// An overlay type's tiberium flag (a view until shared/overlay_type.hpp is
// installed).
struct UnknownUnitOverlayTypeView {
    char unknown_000[0x279];
    bool Tiberium;                          // +0x279
};
// The height a bridge adds (0x00AD057C).

int UnitClass::unknown_slot27(Cell const *where, bool arg2, bool arg3)
{
    if (!House->Is_Player_Control()) {
        return 0;
    }
    CellClass *cell = &Map[*where];
    int action = FootClass::unknown_slot27(where, arg2, arg3);
    if (unknown_6c16f0(-1) < 0 && House->Is_Player_Control() && action == 5) {
        action = 59;
    }
    int overlay = -1;
    if (arg2 && (unknown_id_source_a3d290->unknown_000 & 0x1000) && cell->FoggedObjects) {
        for (int index = 0; index < cell->FoggedObjects->Count(); index++) {
            FoggedObjectClass *fogged = (*cell->FoggedObjects)[index];
            if (fogged->unknown_74 && fogged->unknown_30 == 20) {
                overlay = fogged->unknown_24;
            }
        }
    }
    CoordStruct coord(where->X * 256 + 128, where->Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    if (Map[coord].unknown_140 & 0x100) {
        coord.Z += unknown_geometry_j;
    }
    if (!Map.Is_Shrouded(coord)) {
        if (action == 1) {
            if ((cell->unknown_ec == 5 || overlay != -1
                    && ((UnknownUnitOverlayTypeView *)OverlayTypes[overlay])->Tiberium && arg2)
                && Type->Harvester) {
                return 6;
            }
            if ((cell->unknown_ec == 11 || overlay == 0x7E && cell->OverlayData >= 0x30 && arg2)
                && Type->Weeder) {
                return 6;
            }
            goto move;
        }
    } else if (action == 1) {
        goto move;
    }
    if (action == 5 && !unknown_slot185()) {
        action = 0;
    }
    return action;
move:
    if (Map[*where].Has_Tunnel()) {
        return Map[*where].unknown_47a120(this) ? 0x23 : 0x24;
    }
    if (unknown_5c8) {
        action = 0;
    }
    return action;
}


// int UnitClass::Do_MISSION_GUARD() (0x00704440, slot 129)
// The harvest mission step (hypothesis). A computer harvester or weeder
// returns to mission 10 when its house owns one of its dock types (a
// harvester only while the house lacks +0x189); a human-owned teleporting
// miner does when a friendly refinery is adjacent, or when it is full and
// not moving. A unit deploying into a listed building for a computer house
// with +0x13B queues mission 16 and answers the mission's delay plus 0..2;
// a weeder flagged +0x5A2 returns to mission 10. Then the foot behaviour.

// A building's type and house (a view until shared/building.hpp is
// installed: the type pointer follows TechnoClass at +0x418).
struct UnknownUnitBuildingView2 : TechnoClass {
    BuildingTypeClass *Type;                // +0x418
};

int UnitClass::Do_MISSION_GUARD()
{
    if (Type->Harvester || Type->Weeder) {
        if (!House->Unknown_004F52F0()) {
            for (int index = 0; index < Type->Dock.Count(); index++) {
                if (House->unknown_5420.unknown_func_491d10(Type->Dock[index]->ArrayIndex) > 0) {
                    if (!Type->Harvester || !House->unknown_189) {
                        QueueMission(10, false);
                        return 1;
                    }
                    break;
                }
            }
        } else if (Type->Teleporter) {
            CellClass *cell = (CellClass *)Get_Cell_Ptr();
            for (int facing = 0; facing < 8; facing++) {
                UnknownUnitBuildingView2 *building = (UnknownUnitBuildingView2 *)cell->Adjacent_Cell(facing).Cell_Building();
                if (building && building->Type->Refinery && building->House == House) {
                    QueueMission(10, false);
                    return 1;
                }
            }
            if (unknown_slot165() == 1.0 && Locomotor.Checked()->Is_Moving()) {
                QueueMission(10, false);
                return 1;
            }
        }
    }
    BuildingTypeClass *building = Type->DeploysInto;
    if (building && Is_Listed(Rules->unknown_7c8, building)
        && House->unknown_13b && !House->Unknown_004F52F0()) {
        QueueMission(16, false);
        int delay = (int)(Control()->unknown_10 * 900.0);
        return delay + unknown_id_source_a3d290->Random()(0, 2);
    }
    if (Type->Weeder && unknown_5a2) {
        unknown_5a2 = false;
        QueueMission(10, false);
    }
    return FootClass::Do_MISSION_GUARD();
}


// int UnitClass::Do_MISSION_MOVE() (0x00704680, slot 133)
// Clears +0x5BA; with +0x5C8 set queues mission 5 and answers 1; otherwise
// starts the +0x2A8 change over the type's DeployTime when it is idle and
// continues with the foot behaviour.

int UnitClass::Do_MISSION_MOVE()
{
    unknown_5ba = false;
    if (unknown_5c8) {
        QueueMission(5, false);
        return 1;
    }
    if (!unknown_2a8.Is_Count_Down_Complete()) {
        unknown_2a8.Count_Down(Type->DeployTime);
    }
    return FootClass::Do_MISSION_MOVE();
}


// int UnitClass::Do_MISSION_PATROL() (0x007046E0, slot 145)
// Clears +0x5BA, starts the +0x2A8 change over the type's DeployTime when
// it is idle, then the foot behaviour.

int UnitClass::Do_MISSION_PATROL()
{
    unknown_5ba = false;
    if (!unknown_2a8.Is_Count_Down_Complete()) {
        unknown_2a8.Count_Down(Type->DeployTime);
    }
    return FootClass::Do_MISSION_PATROL();
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// UnitClass::Desired_Load_Dir (0x00704730, slot 182), from ra2-decomp UnitClass::Desired_Load_Dir
// 0x00740B60 (YR twin). Scores the eight neighbouring cells against the direction to the
// passenger (or the reverse of the body facing): +128 for a cell the passenger may enter (or
// already occupies), or, without a passenger, for passable land with no building, vehicle or
// monolith and no unallied occupant; -128 otherwise; less the angular distance, less 100 for
// FACING_S. Stores the best cell in *cell (the null cell when none scores above 0) and answers
// its facing from Harvester_Load_List (0x007F8F4C; FACING_S by default); trains keep their body
// facing.

// CELL_NONE (0x00AD0488; as UnitClass slot 55).
// Harvester_Load_List: the facing answered for each exit (0x007F8F4C).
extern int const unknown_facings_7f8f4c[8];

// The twin's file-local Direction wrapper (DirType::Direction 0x00423EF0, called out of line).
static inline UnknownFacingDir &_Direction(UnknownFacingDir &dir, ObjectClass *from, AbstractClass *to)
{
    return dir.Direction(from->Center_Coord(), to->Center_Coord());
}

int UnitClass::unknown_slot182(AbstractClass *object, Cell *cell)
{
    FacingType face = FACING_N;
    int faceto;
    if (object) {
        UnknownFacingDir direction = _Direction(direction, this, object);
        faceto = Dir_To_256(direction);
    } else {
        UnknownFacingDir dir((unsigned short)(unknown_2e0.Current().Raw + 0x7FFF));
        faceto = Dir_To_256(dir);
    }
    int desired = (signed char)faceto;
    FacingType bestdir = FACING_N;
    int bestval = -1;
    for (; face < FACING_COUNT; face++) {
        int value = 0;
        Cell cellnum = Adjacent_Cell(unknown_slot104(), face);
        if (object) {
            value = (!((ObjectClass *)object)->Can_Enter_Cell(&Map[cellnum], face, unknown_5d47d0(), 0, 1)
                || ((ObjectClass *)object)->unknown_slot104() == cellnum) ? 128 : -128;
        } else {
            CellClass *land = &Map[Cell(cellnum)];
            if (Ground[land->unknown_ec].Cost[0] == 0.0f || land->Occupy_124.Building || land->Occupy_124.Vehicle || land->Occupy_124.Monolith
                || (land->unknown_124 & 0x1F) == 0x1F) {
                value = -128;
            } else {
                if (land->Cell_Techno(Point2D(0, 0), false, 0)
                    && !House->Is_Ally((TechnoClass *)land->Cell_Techno(Point2D(0, 0), false, 0))) {
                    value = -128;
                } else {
                    value = 128;
                }
            }
        }
        int here = DirType(face).As_Dir256();
        int diff = (signed char)here - desired;
        value -= abs(diff);
        if (face == FACING_S) {
            value -= 100;
        }
        if (bestval == -1 || value > bestval) {
            bestval = value;
            bestdir = face;
        }
    }
    int facing = FACING_S;
    *cell = unknown_geometry_k;
    if (bestval > 0) {
        *cell = Adjacent_Cell(unknown_slot104(), bestdir);
        facing = unknown_facings_7f8f4c[bestdir];
    }
    if (Type->IsTrain) {
        facing = Dir_To_8(unknown_2e0.Current());
    }
    return facing;
}


// bool UnitClass::Flag_Attach(int arg1) (0x007049C0)
// Records arg1 at +0x5B4 (-1 means none; a house index, see slot 52) when
// nothing is recorded yet, then calls slot 71 with 2. Answers whether it did.

bool UnitClass::Flag_Attach(int arg1)
{
    if (arg1 != -1 && unknown_5b4 == -1) {
        unknown_5b4 = arg1;
        Mark(2);
        return true;
    }
    return false;
}


// Clears the recorded +0x5B4 value and notifies virtual slot71 when set.
bool UnitClass::Flag_Remove()
{
    if (unknown_5b4 != -1) {
        unknown_5b4 = -1;
        Mark(2);
        return true;
    }
    return false;
}


// int UnitClass::unknown_slot166() (0x00704A20, slot 166)
// With pip scale 2 (tiberium) the pips to show: the stored total over the
// type's Storage times the type's pip count (slot 29), rounded; otherwise
// the techno's answer.

int UnitClass::unknown_slot166()
{
    switch (Type->PipScale) {
    case 2:
    {
        double ratio = unknown_294.Get_Total_Amount() / Type->Storage;
        return (int)(Type->Max_Pips() * ratio + 0.5);
    }
    }
    return TechnoClass::unknown_slot166();
}


// void UnitClass::APC_Close_Door() (0x00704A80): restarts the timed change at
// +0x2A8 over the type's DeployTime. No call sites: AI 0x006FB100 and slot 95
// 0x006FC1F0 expand the same statement inline.

void UnitClass::APC_Close_Door()
{
    unknown_2a8.Count_Down(Type->DeployTime);
}


// void UnitClass::APC_Open_Door() (0x00704AA0)
// A standing unit (locomotor not moving, +0x599 clear) restarts its timer at
// +0x2A8 with the type's DeployTime (0x00496B70).

void UnitClass::APC_Open_Door()
{
    if (!Locomotor.Checked()->Is_Moving() && !unknown_599) {
        unknown_2a8.Count_Up(Type->DeployTime);
    }
}


// InfantryTypeClass *UnitClass::unknown_slot184() (0x00704B00, slot 184)
// The TechnoClass behaviour.

InfantryTypeClass *UnitClass::unknown_slot184()
{
    return TechnoClass::unknown_slot184();
}


// int UnitClass::Do_MISSION_REPAIR() (0x00704B10, slot 141)
// The harvest step (hypothesis): looks for the rules' refinery type (slot
// 301); with one in reach asks it to dock (radio message 2) and on a roger
// queues mission 7; otherwise, unless the house owns such a building,
// gives up (slot 261). Answers the mission's delay in frames.

int UnitClass::Do_MISSION_REPAIR()
{
    TechnoClass *refinery = unknown_slot301(Rules->unknown_778, 0, 0);
    unknown_5ba = false;
    if (!refinery) {
        if (!House->unknown_5420.unknown_func_491d10(Rules->unknown_778->ArrayIndex)) {
            unknown_slot261(0, 1);
        }
    } else if (unknown_slot150(2, refinery) == 1) {
        QueueMission(7, false);
        return 1;
    }
    return (int)(Control()->unknown_10 * 900.0);
}


// UnknownFacingDir UnitClass::unknown_slot183() (0x00704BB0, slot 183)
// The turret's current facing for a turreted type, otherwise the techno's.

UnknownFacingDir UnitClass::unknown_slot183()
{
    if (Type->Turret) {
        return unknown_2f8.Current();
    }
    return TechnoClass::unknown_slot183();
}


// int UnitClass::unknown_slot218(AbstractClass *target, int weapon, bool check_range)
// (0x00704C00, slot 218; TibSun UnitClass::Can_Fire): the techno answer refined
// for deploy-to-fire, tethering, healing weapons, firing while moving, turret
// rotation and the turret's angle to the target, then the locomotor's answer.
// The target as a techno (0x0040DBD0; null for other kinds).
TechnoClass *__fastcall As_Techno_0040dbd0(AbstractClass *target);
// TibSun's DirType comparison: the larger angular distance either way
// (both sides by value).
inline bool operator<(UnknownFacingDir left, UnknownFacingDir right)
{
    return abs((short)left.Raw) < abs((short)right.Raw);
}

int UnitClass::unknown_slot218(AbstractClass *target, int weapon, bool check_range)
{
    if (SpawnManager && SpawnManager->Launching_Count()) return 10;
    int action = TechnoClass::unknown_slot218(target, weapon, check_range);
    if (action == 0 || action == 2) {
        CoordStruct position = Coord;
        bool usable_land = Map[position].Can_Build_Here();
        if (Type->DeployToFire && unknown_slot284() && !usable_land) return 11;
    }
    if (action == 0) {
        if (unknown_370 && Contact_With_Whom(0)->Kind_Of() == 6) return 6;
        WeaponTypeClass *weapon_type = unknown_slot231(weapon)->WeaponType;
        if (unknown_6c16f0(-1) < 0) {
            TechnoClass *object = As_Techno_0040dbd0(target);
            if (!object || !object->unknown_slot31() || object->Health_Ratio() >= Rules->unknown_1360) return 5;
        }
        if (!Type->MobileFire && unknown_494) return 7;
        if ((weapon_type->unknown_106 || weapon_type->unknown_105) && unknown_494) return 7;
        if (unknown_208 && unknown_494) return 7;
        if (!unknown_57d && unknown_599 && weapon_type->Projectile->ROT == 0) return 4;
        if (!weapon_type->unknown_107 && !Type->LargeVisceroid && !Type->SmallVisceroid) {
            UnknownFacingDir current;
            if (Type->Turret) current = unknown_2f8.Current();
            else current = unknown_2e0.Current();
            int steps = weapon_type->Projectile->ROT ? 16 : 8;
            if (UnknownFacingDir((unsigned short)((unsigned char)steps << 8)) < UnknownFacingDir((unsigned short)(current.Raw - Direction_To(target).Raw))) {
                return 2;
            }
        }
        int state = Locomotor.Checked()->State();
        if (state) return state;
    }
    return action;
}


// BulletClass *UnitClass::unknown_slot221(AbstractClass *target, int arg2) (0x00704ED0, slot 221: Fire)
// target is the target, arg2 the weapon index; answers the bullet. Without a
// weapon nothing. For the primary weapon, notes whether the burst's firing
// sync frame is pending. Unless slot 218 refuses, fires through the techno;
// then reloads when ammunition ran short (not for ManualReload), starts
// the firing animation (FiringFrames * 2 - 1 frames at +0x5A8) when not
// synchronized, updates scalable bullets, and for DisguiseWhenStill types
// restarts the +0x184 timer with the weapon's +0x118 delay.

BulletClass *UnitClass::unknown_slot221(AbstractClass *target, int arg2)
{
    BulletClass *bullet = 0;
    WeaponTypeClass *weapon = unknown_slot231(arg2)->WeaponType;
    if (!weapon) {
        return 0;
    }
    bool synchronized = false;
    if (arg2 == 0) {
        int burst = unknown_310 % weapon->Burst;
        int frame;
        if (burst < 2) {
            frame = Type->FiringSyncFrame[burst];
        } else {
            frame = -1;
        }
        if (unknown_5a8 != -1 && frame != -1) {
            synchronized = true;
        } else {
            synchronized = false;
        }
    }
    if (!unknown_slot218(target, arg2, 1)) {
        bullet = TechnoClass::unknown_slot221(target, arg2);
        if (bullet) {
            if (Type->Ammo > 0 && unknown_254 < Type->Ammo && !Type->ManualReload) {
                unknown_6c8300();
            }
            if (!synchronized && Type->FiringFrames > 0) {
                unknown_5a8 = Type->FiringFrames * 2 - 1;
            }
            if (bullet->Type->Scalable) {
                bullet->unknown_func_463bc0();
            }
            if (Type->DisguiseWhenStill) {
                unknown_184 = CDTimerClass<FrameTimerClass>(weapon->unknown_118);
            }
        }
    }
    return bullet;
}


// ObjectTypeClass *UnitClass::Class_Of() const (0x00705020, slot 33)
// The unit type.

ObjectTypeClass *UnitClass::Class_Of() const
{
    return Type;
}


// double UnitClass::unknown_slot165() (0x00705030, slot 165)
// For harvesters and weeders the fill ratio (the stored total over the
// type's Storage); 0 for everything else.

double UnitClass::unknown_slot165()
{
    if (!Type->Harvester && !Type->Weeder) {
        return 0.0;
    }
    return unknown_294.Get_Total_Amount() / Type->Storage;
}


// void UnitClass::unknown_slot304() (0x00705070, slot 304)
// A computer-owned crusher (or one with ability 17) next to a crushable
// target, with no destination, or a BalloonHover unit, goes for the target
// at +0x228 (slot 260); otherwise the foot behaviour.

// The target as an object (an inline cast helper; hypothesis).
static inline ObjectClass *As_Object(AbstractClass *target)
{
    return !target ? 0 : (target->AbstractFlags.IsTechno ? (ObjectClass *)target : 0);
}

void UnitClass::unknown_slot304()
{
    if (!House->Unknown_004F52F0() && unknown_228 && !unknown_494) {
        ObjectClass *object = As_Object(unknown_228);
        if ((Type->Crusher || unknown_6d7910(17)) && unknown_5d4c00(unknown_228) < Rules->unknown_1390
            && object && object->Class_Of()->unknown_1fd && (Type->AutoCrush || !House->Unknown_004F52F0())) {
            Assign_Destination(unknown_228, true);
            return;
        }
    }
    if (Type->BalloonHover) {
        Assign_Destination(unknown_228, true);
        return;
    }
    FootClass::unknown_slot304();
}


// void UnitClass::Overrun_Square(Cell const &where, bool threaten) (0x00705160, slot 302)
// Crushing (hypothesis): where is the cell entered, threaten asks only to
// scatter. Picks the bridge level (+0x79, or this cell's level 4 below the
// unit's). Only for crushers (or ability 17). With arg2, scatters the
// level's occupants (0x00476920). Otherwise every crushable occupant within
// 0x4000 (squared) that is an enemy (or any, for trains) and not flagged
// +0x7A is crushed: a vehicle thief heading for this unit takes it over
// instead; afterwards slot 252.

// A value the scatter takes (0x00AD04C0; meaning unknown).
// An infantry's type and destination (a view until shared/infantry.hpp is
// installed).
struct UnknownUnitInfantryView2 : TechnoClass {
    char unknown_418[0x494 - 0x418];
    AbstractClass *unknown_494;             // +0x494
    char unknown_498[0x5A8 - 0x498];
    InfantryTypeClass *Type;                // +0x5A8
};

void UnitClass::Overrun_Square(Cell const &where, bool threaten)
{
    CellClass *cell = &Map[where];
    bool upper = false;
    if (cell->unknown_140 & 0x100) {
        if (unknown_79) {
            upper = true;
        } else {
            Cell position = Coord_Cell(Center_Coord());
            if (Map[position].unknown_11b == cell->unknown_11b + 4) {
                upper = true;
            }
        }
    }
    if (!Type->Crusher && !unknown_6d7910(17)) {
        return;
    }
    if (threaten) {
        if (upper) {
            if (cell->unknown_128 & 0x1F) {
                cell->Incoming(&unknown_geometry_l, true, false, true);
            }
        } else if (cell->unknown_124 & 0x1F) {
            cell->Incoming(&unknown_geometry_l, true, false, false);
        }
        return;
    }
    ObjectClass *object;
    if (upper) {
        object = cell->AltOccupierPtr;
    } else {
        object = cell->OccupierPtr;
    }
    int crushed = 0;
    while (object) {
        if (object->Class_Of()->unknown_1fd
            && (!House->Is_Ally((TechnoClass *)object) || Type->IsTrain)
            && Distance_Squared(object->Center_Coord()) < 0x4000 && !object->unknown_7a) {
            UnknownUnitInfantryView2 *infantry = (UnknownUnitInfantryView2 *)object;
            if (object->Kind_Of() == 15 && infantry->Type->VehicleThief && infantry->unknown_494 == this && !Type->IsTrain) {
                ObjectClass *next = object->unknown_30;
                unknown_372 = infantry->unknown_372;
                unknown_slot54(false);
                unknown_slot222(infantry->House, 1);
                object->Delete_Me();
                object = next;
            } else {
                ObjectClass *next = object->unknown_30;
                crushed = 1;
                Voc_Play_At(object->Class_Of()->unknown_1c0, CoordStruct(Coord), 0);
                object->Record_The_Kill(this);
                object->Mark(0);
                object->Limbo();
                object->Delete_Me();
                object = next;
            }
        } else {
            object = object->unknown_30;
        }
    }
    if (crushed) {
        unknown_slot252((int)object);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// UnitClass destination assignment, original RA2 0x007053F0.
// Assign_Destination(AbstractClass *, bool): vtable slot 260.
// Statement shape follows the TibSun reconstruction unit.cpp 5117-5449 (shape only; see THIRD_PARTY.md).
extern "C" GUID const CLSID_DriveLocomotionClass, CLSID_TunnelLocomotionClass;
extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;
extern Cell const unknown_unload_offset_851a88;
// The native view is the known MapClass::ZoneConnections member at Map+0x4C.
extern DynamicVectorClass<ZoneConnectionClass> unknown_zone_connections_83252c;
// Westwood runtime-kind cast: null input or a mismatched class yields null.
template <class T,int kind> inline T *Unit_Dynamic_Cast(AbstractClass *target)
{
    if(!target) return 0;
    return target->Kind_Of()==kind ? (T *)target : 0;
}
// comip's _Release: Unit.cpp releases the held locomotor inline.
template <> inline void UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::Release_Held()
{
    if (Pointer != 0) {
        Pointer->Release();
    }
}
// comip's operator=: the new reference through _AddRef, which stays out of line
// here (0x0069ED20 for IPiggyback, 0x00453EA0 for ILocomotion; as SuperClass::Place 0x0069D850).
template <> void UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::_AddRef() const;
template <> inline UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> &
UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::operator=(UnknownComPtr const &other)
{
    if (Pointer != other.Pointer) {
        IPiggyback *old = Pointer;
        Pointer = other.Pointer;
        _AddRef();
        if (old != 0) {
            old->Release();
        }
    }
    return *this;
}
template <> void UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::_AddRef() const;
template <> inline UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> &
UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::operator=(UnknownComPtr const &other)
{
    if (Pointer != other.Pointer) {
        ILocomotion *old = Pointer;
        Pointer = other.Pointer;
        _AddRef();
        if (old != 0) {
            old->Release();
        }
    }
    return *this;
}
// TibSun target.h Is_Target_Cell: a cell target (the RTTI read is the virtual Kind_Of in RA2).
inline bool Is_Target_Cell(AbstractClass *target) {return(target != 0 && target->Kind_Of() == 11);}
// True when the unit type lists the building type among its docks.
inline bool Unit_Is_Dock(UnitTypeClass const *type, BuildingTypeClass const *building)
{
    for (int i = 0; i < type->Dock.Count(); i++) {
        if (type->Dock[i] == building) return true;
    }
    return false;
}
void UnitClass::Assign_Destination(AbstractClass *target, bool immediate)
{
	/*
	**	Abort early if there is anything wrong with the parameters
	**	or the unit already is assigned the specified destination.
	*/
	if (Type->BalloonHover && !target && unknown_494 && unknown_228) {
        if (unknown_494==unknown_228) return;
        if (Is_Target_Cell(unknown_494)) {
            CellClass *destination=Unit_Dynamic_Cast<CellClass,11>(unknown_494);
            if(destination->OccupierPtr==unknown_228) return;
        }
    }
    if(target==unknown_494 && !unknown_190) return;
    unknown_190=false;
    if(unknown_5c8) {FootClass::unknown_4cd9b0();return;}

	/*
	**	Transport vehicles must tell all passengers that are about to load, that they
	**	cannot proceed. This is accomplished with a radio message to this effect.
	*/
	if (HasAnyLink() && Type->Passengers > 0 && Contact(0)->Kind_Of() == 15) {
		if(target && target->AbstractFlags.IsTechno && !ContainsLink((TechnoClass *)target)) unknown_slot149(3);
	}

	/*
	 * If tethered to a carryall while sleeping, untether before moving.
	 */
	if (target != 0 && HasAnyLink() && Get_Mission() == 0 && unknown_370 && Contact(0)->Kind_Of() == 2) {
		AircraftClass * a = (AircraftClass *)Contact(0);
		if (a->Type->Carryall) {
			a->Assign_Destination(0, true);
			unknown_slot149(25);
			unknown_slot149(3);
		}
	}

	BuildingClass * b=Unit_Dynamic_Cast<BuildingClass,6>(target);
    if(Get_Mission()==16 || MissionQueue==16) {
        Cell unload_cell=unknown_slot104()+unknown_unload_offset_851a88;
        BuildingClass *refinery=(BuildingClass *)Map[unload_cell].Cell_Building();
        if(refinery && Type->Harvester) {
            if(refinery->unknown_25c[5]) refinery->unknown_25c[5]->Delete_Me();
            if(refinery->unknown_25c[6]) refinery->unknown_25c[6]->Delete_Me();
            if(refinery->unknown_454[10]) refinery->unknown_44cfd0(10);
        }
    }

	/*
	**	Handle entry logic here.
	*/
	if (Get_Mission() == 7 || MissionQueue == 7) {

		/*
		**	If not already in radio contact (presumed with the transport), then
		**	either try to establish contact if allowed, or just move close and
		**	wait until radio contact can be established.
		*/
		if (!HasAnyLink()) {
			TechnoClass *destination_techno=target->As_Techno();
            if(destination_techno && destination_techno->Kind_Of()==1) {
                unknown_3f8=Unit_Dynamic_Cast<UnitClass,1>(target);
                unknown_6d6e50(0);target=0;
            } else if (b != 0 && b->Kind_Of()==6) {

				/*
				**	Determine if the transport is already in radio contact. If so, then just move
				**	toward the transport and try to establish contact at a later time.
				*/
				if (b->HasAnyLink()) {
					unknown_6d6e50(target);

					/*
					**	HACK ALERT: The repair bay is counting on the assignment of the unknown_494 by this routine.
					**	The refinery must NOT have the navcom assigned by this routine.
					*/
					if (b->Type->UnitRepair) {
						if(Health_Ratio()<Rules->unknown_1360) {target=0;unknown_3f8=b;}
                        else {unknown_slot149(3);target=0;}
					}
				} else {
					AbstractClass * oldnav = unknown_494;
					if (unknown_slot150(14, b) != 1) {
						unknown_slot149(3);
						if (b->Type->UnitRepair) {
							unknown_6d6e50(target);
						}
					} else {
						if (b->Type->DockUnload && unknown_494 != oldnav) {
							target = unknown_494;
						}
					}
				}
			} else {
				TechnoClass * techno = target && target->AbstractFlags.IsTechno ? (TechnoClass *)target : 0;
				if (techno != 0) {

					/*
					**	Determine if the transport is already in radio contact. If so, then just move
					**	toward the transport and try to establish contact at a later time.
					*/
					if (techno->HasAnyLink()) {
						unknown_6d6e50(target);
					} else {
						if (unknown_slot150(2, techno) == 1) {
							if (unknown_slot149(14) == 1) {
                            // RA2 retains the destination and continues after the handshake.
                        } else
							unknown_slot149(3);
						}
					}
				}
			}
		} else {
			unknown_4d0 = -1;
		}
	} else {
		unknown_4d0 = -1;
	}

	bool useplan = false;

	/*
	 * Subterranean units prepend their navigation target onto the queue and
	 * re-target the nearest reachable cell when driving rather than burrowing.
	 */
	if (target != 0 && Type->unknown_af5 && Locomotor.Checked()->Is_Moving()) {
		UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(Locomotor);
		GUID clsid;
		persist.Checked()->GetClassID(&clsid);
		if (memcmp(&clsid,&CLSID_DriveLocomotionClass,sizeof(GUID))==0) {
			unknown_49c.Add_Head(target);
			unknown_478.Clear();
			CellClass * tcell = Get_Target_Cell_Ptr();
			target = tcell;
			immediate = false;
			if (tcell != 0 && ((tcell->unknown_140&0x100)!=0) && !Is_Moving_Onto_Bridge(0)) {
				Cell nearby = Map.Nearby_Location(tcell->MapCoords, 1, Map.Zone(tcell->MapCoords, Type->MovementZone, false), Type->MovementZone, false, Point2D(1, 1), false, true, false, false, Cell(0,0), 0);
				if (nearby != unknown_geometry_k) {
					target = &Map[nearby];
				}
			}
		}
	}

	bool needsswap = false;

	if (target != 0) {
		bool surface = false;

		/*
		 * A subterranean unit that is told to move immediately must decide whether
		 * it can burrow straight to the destination or has to plan a surface route.
		 */
		if (Type->unknown_af5 && immediate) {
			bool isbridge = Is_Moving_Onto_Bridge(0);

			Cell from = Cell(unknown_slot19(0));
			bool canburrowfrom = Map[from].Can_Burrow_Here();

			Cell to = Cell(target->Center_Coord());
			CellClass * tocell = &Map[to];
			bool isunderbridgeto = ((tocell->unknown_140&0x100)!=0);
			bool canburrowto = tocell->Can_Burrow_Here();
			int height = Get_Height_AGL();

			if (isbridge && isunderbridgeto) {
				int z1 = Map.Zone_Connection_Index(from, 3, 0);
				if (z1 == Map.Zone_Connection_Index(to, 3, 0) && z1 != -1) {
					if (!unknown_zone_connections_83252c[z1].unknown_08) {
						Cell d1 = Map.Unknown_005651E0(from, from);
						Cell d2 = Map.Unknown_005651E0(to, to);
						if (Cell_Length(d1-d2) <= 2) {
							needsswap = true;
						}
					} else {
						needsswap = true;
					}
				}
			}
			if ((isbridge || isunderbridgeto) && !needsswap) {
				useplan = true;
			}

			if (((!canburrowfrom && height >= 0) || !canburrowto) && !needsswap) {
				useplan = true;
			}
		}

		if (useplan) {
			target = Plan_Route_00707f60(target);
			surface = true;
		} else if (immediate) {
			unknown_478.Clear();
			surface = true;
		}

		/*
		 * Surfacing tunneler keeps its current cell at the route front.
		 */
		if (surface) {
			if (Locomotor.Checked()->Is_Surfacing()) {
				unknown_478.Add_Head(target);
				target = &Map[Position_Coord_004f3f40()];
			}
		}

	} else { unknown_478.Clear(); }

    if(Type->Teleporter && !unknown_210) {
        TechnoClass *contact=Contact(0);
        UnknownComPtr<IPersist,&unknown_uuid_IPersist_7cf208> persist(Locomotor);
        GUID tclsid;
        persist.Checked()->GetClassID(&tclsid);
        bool drive=true;
        if(contact) {
            if(contact->Kind_Of()==6) {
                BuildingClass *building=Unit_Dynamic_Cast<BuildingClass,6>(contact);
                CellClass *cell=Unit_Dynamic_Cast<CellClass,11>(target);
                if(building->Type->DockUnload && cell && !cell->Cell_Unit(false)) {
                    drive=false;
                    if(memcmp(&tclsid,&unknown_clsid_7a1fd0,sizeof(GUID))!=0) {
                        UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> piggy(Locomotor);
                        if(piggy && piggy.Checked()->Is_Piggybacking() && piggy.Checked()->Is_Ok_To_End()) {
                            piggy.Checked()->End_Piggyback(&Locomotor);
                        } else {
                            Locomotor.Checked()->Stop_Moving();
                            Set_Mission(-1);
                            QueueMission(7,false);
                            unknown_598=true;
                            unknown_190=true;
                        }
                    }
                }
            }
        }
        if((contact == 0 || drive == true) && memcmp(&tclsid,&CLSID_DriveLocomotionClass,sizeof(GUID))!=0) {
            UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> piggy(Locomotor);
            if(piggy && piggy.Checked()->Is_Piggybacking() && piggy.Checked()->Is_Ok_To_End()) piggy.Checked()->End_Piggyback(&Locomotor);
            UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600> walk(CLSID_DriveLocomotionClass);
            walk.Checked()->Link_To_Object(this);
            piggy=UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30>(walk);
            if(piggy) {piggy.Checked()->Begin_Piggyback(Locomotor);Locomotor=walk;}
        }
    }
	/*
	 * If the locomotor is a tunnel locomotor and the unit is on the ground,
	 * surface the tunneler by swapping the tunnel locomotor for a drive one.
	 * (Mirrors BuildingClass weapons-factory exit, building.cpp:6236-6251.)
	 */
	if (target != 0 && !Locomotor.Checked()->Is_Moving()) {
		UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist;
        if(Locomotor) {
            HRESULT result=persist.Query_Held(Unit_Checked_Locomotor_004cde90(Locomotor));
            if(FAILED(result)&&result!=E_NOINTERFACE) _com_issue_error(result);
        }
		GUID clsid;
		persist.Checked()->GetClassID(&clsid);
		if (memcmp(&clsid,&CLSID_TunnelLocomotionClass,sizeof(GUID))==0 && Get_Height_AGL() == 0) {
            CoordStruct tc = target->Center_Coord();
			int gl = Map.Get_Height_GL(tc);
			if (tc.Z < gl) tc.Z = gl;

			bool doswap = needsswap;
			if (!doswap) {
				bool canburrow = !Is_Moving_Onto_Bridge(0)
					&& !(Map[target->Center_Coord()].unknown_140&0x100)
					&& Map[unknown_slot19(0)].Can_Burrow_Here()
					&& Map[target->Center_Coord()].Can_Burrow_Here();
				if (canburrow) {
					Cell to = Cell(target->Center_Coord());
					if (!unknown_709790(&Get_Target_Cell(), &to)) {
						doswap = true;
					}
				} else {
					doswap = true;
				}
			}

			if (doswap) {
				UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggy;
                if(Locomotor) {
                    HRESULT result=piggy.Query_Held(Unit_Checked_Locomotor_004cde90(Locomotor));
                    if(FAILED(result)&&result!=E_NOINTERFACE) _com_issue_error(result);
                }
				if (piggy != 0 && piggy.Checked()->Is_Piggybacking()) {
					piggy.Checked()->End_Piggyback(&Locomotor);
				}
				UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> walk(CLSID_DriveLocomotionClass);
				walk.Checked()->Link_To_Object(this);
				{
                UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> walk_piggy;
                if(walk) {
                    HRESULT result=walk_piggy.Query_Held(Unit_Checked_Locomotor_004cde90(walk));
                    if(FAILED(result)&&result!=E_NOINTERFACE) _com_issue_error(result);
                }
                piggy=(IPiggyback *)walk_piggy;
                }
				if (piggy != 0) {
					piggy.Checked()->Begin_Piggyback(Locomotor);
					Locomotor = (ILocomotion *)walk;
					Locomotor.Checked()->Force_New_Slope(Map[Position_Coord_004f3f40()].unknown_11c);
				}
			}
		}
	}

	/*
	**	If the player clicked on a friendly repair facility and the repair
	**	facility is currently not involved with some other unit (radio or unloading).
	*/
	if (b != 0 && b->Type->UnitRepair) {
		if (b->HasAnyLink() && b->Contact(0) != this) {
			unknown_6d6e50(target);
		} else {

			/*
			**	Establish radio contact protocol. If the facility responds correctly,
			**	then remain in radio contact and proceed toward the desired destination.
			*/
			if (unknown_slot150(2, b) == 1) {
				if (unknown_slot149(14) == 1) {
					FootClass::Assign_Destination(target, immediate);
					unknown_4d0 = -1;
					return;
				}

				/*
				**	Failure to establish a docking relationship with the refinery.
				**	Bail & await further instructions.
				*/
				unknown_slot149(3);
				target = 0;
			}
		}
	}

	/*
	 * A weapons factory that this unit is exiting wants the unit driven to its
	 * exit cell before anything else; queue the real destination behind it.
	 */
	BuildingClass * wf = (BuildingClass *)Contact(0);
	if (wf != 0 && wf->Kind_Of() == 6 && wf->Type->WeaponsFactory && Mission != 7) {
		Cell exitcell = Cell(wf->Position_Coord_004f3f40()) + Cell(5, 1);
		if (target != &Map[exitcell]) {
			unknown_49c.Clear();
			unknown_478.Clear();
			if (target != 0) {
				unknown_4c8bb0(target);
			}
			return;
		}
	}

	/*
	 * Harvesters and weeders automatically enter their dock building when the
	 * clicked target is that building (and the houses are allied).
	 */
	if (b != 0 && Type->Dock.Count() > 0 && Unit_Is_Dock(Type, b->Type) &&
		Kind_Of() == 1 && (Type->Harvester || Type->Weeder) &&
		Get_Mission() != 16) {

		if (Contact(0) != b && !b->HasAnyLink() && unknown_slot150(2, b) == 1 &&
			Get_Mission() != 7 && Get_Mission() != 10) {
			QueueMission(7, false);
			target = 0;
		} else {
			if (target != 0 && target->Kind_Of() == 6 && Contact(0) != target) {
				Cell nearby = Map.Nearby_Location(Cell(b->Position_Coord_004f3f40()), 2, Map.Zone(Cell(b->Position_Coord_004f3f40()), Type->MovementZone, false), Type->MovementZone, false, Point2D(1, 1), false, true, false, false, Cell(0,0), 0);
				if (nearby != unknown_geometry_k) {
					target = &Map[nearby];
				} else {
					target = 0;
				}
			}
		}
	}

	/*
	 * If the locomotor is unpowered and we are not in an ion storm, power it
	 * back on when an adjacent friendly repair facility occupies our cell.
	 */
	if (!Locomotor.Checked()->Is_Powered() && !House->Unknown_0051FC80()) {
		CellClass * cellptr = (CellClass *)Get_Cell_Ptr();
		if (!(cellptr->unknown_140&0x100)) {
			ObjectClass * occupier = cellptr->OccupierPtr;
			while (occupier != 0) {
				if (occupier != this && occupier->Kind_Of() == 6 && ((BuildingClass *)occupier)->Type->UnitRepair) {
					Locomotor.Checked()->Power_On();
					break;
				}
				occupier = occupier->unknown_30;
			}
		}
	}

	FootClass::Assign_Destination(target, immediate);
}


// AbstractClass *UnitClass::unknown_slot219(int arg1, CoordStruct const *coord, bool arg3) (0x00706A10, slot 219)
// Nothing for a computer-owned DeployToFire unit. Unless arg1 already has
// one of the flags 0x1B978, adds the projectile flags (0x00733A60) of the
// current weapon (0x006D83E0/0x006D84A0) or of both weapons; then the foot
// behaviour.

AbstractClass *UnitClass::unknown_slot219(int arg1, CoordStruct const *coord, bool arg3)
{
    if (House->Unknown_004F52F0() && Type->DeployToFire) {
        return 0;
    }
    if (!(arg1 & 0x1B978)) {
        if (unknown_6d83e0()) {
            arg1 |= unknown_slot231(unknown_6d84a0())->WeaponType->Allowed_Threats();
        } else {
            if (unknown_slot231(0)->WeaponType) {
                arg1 |= unknown_slot231(0)->WeaponType->Allowed_Threats();
            }
            if (unknown_slot231(1)->WeaponType) {
                arg1 |= unknown_slot231(1)->WeaponType->Allowed_Threats();
            }
        }
    }
    return FootClass::unknown_slot219(arg1, coord, arg3);
}


// Original 0x00706AE0: 1147 body bytes and 5 independently bounded NOP bytes.
// Creates scenario units from [Units], then resolves their linked-unit indices.
extern char const *unknown_units_section_7addc0;
extern int unknown_ini_format_a40d24;
// VC6 health block emits exact original native instructions, without a float32 store. This does not establish a portable source-width contract.
static inline UnitTypeClass *Unit_Read_Float_Health_00706AE0(UnitClass &unit, float health)
{
    UnitTypeClass *type = unit.Type;
    unit.Strength = (int)((double)health * type->MaxStrength * (1.0 / 256.0));
    return type;
}
static inline void Resolve_Unit_Link(int link_index,unsigned linked_link_index)
{
    UnitClass *unit = Units[link_index];
    if (linked_link_index == (unsigned)-1 || linked_link_index >= (unsigned)Units.Count()) {
        unit->unknown_5b0 = 0;
    } else {
        UnitClass *linked = Units[linked_link_index];
        unit->unknown_5b0 = linked;
        linked->unknown_5b8 = true;
    }
}
void __fastcall UnitClass::Read_INI(CCINIClass &ini)
{
    DynamicVectorClass<int> links;
    links.Clear();
    int count = ini.Entry_Count(unknown_units_section_7addc0);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_units_section_7addc0, index);
        char buffer[128];
        ini.Get_String(unknown_units_section_7addc0, entry, 0, buffer, sizeof(buffer));
        int house_index = HouseClass::Find_Index(strtok(buffer, ","));
        if (house_index != -1) {
            int type_index = UnitTypeClass::From_Name(strtok(0, ","));
            HouseClass *house = Houses[house_index];
            if (type_index != -1) {
                UnitClass *unit = new UnitClass(UnitTypes[type_index], house);
                if (unit != 0) {
                    int health = atoi(strtok(0, ","));
                    Cell cell;
                    if (unknown_ini_format_a40d24 >= 4) {
                        int x = atoi(strtok(0, ","));
                        short y = (short)atoi(strtok(0, ","));
                        cell = Cell((short)x, y);
                    } else {
                        int packed = atoi(strtok(0, ","));
                        cell = Cell((short)(packed % 128), (short)(packed / 128));
                    }
                    CoordStruct coord = Cell_Center(cell);
                    int direction = atoi(strtok(0, ","));
                    int mission = MissionClass::Mission_From_Name(strtok(0, ","));
                    TagTypeClass *tag_type = TagTypeClass::From_Name(strtok(0, ","));
                    if (tag_type) {
                        TagClass *tag = TagClass::Find_Or_Make(tag_type);
                        if (tag) unit->Attach_Tag(tag);
                    }
                    char *token = strtok(0, ",");
                    if (token) unit->unknown_118.From_Integer(atoi(token));
                    token = strtok(0, ",");
                    if (token) unit->unknown_1ac = atoi(token);
                    token = strtok(0, ",");
                    if (token) {
                        unit->unknown_79 = atoi(token) != 0;
                        if (unit->unknown_79) coord.Z = Map.Get_Height_GL(coord) + unknown_geometry_j;
                    }
                    token = strtok(0, ",");
                    if (token) {
                        int linked_index = atoi(token);
                        links.Add(linked_index);
                    }
                    token = strtok(0, ",");
                    if (token) unit->unknown_379 = atoi(token) != 0;
                    token = strtok(0, ",");
                    if (token) unit->unknown_37a = atoi(token) != 0;
                    if (unit->Unlimbo(&coord, direction)) {
                        UnitTypeClass *type = Unit_Read_Float_Health_00706AE0(*unit, (float)health);
                        if (unit->Strength > type->MaxStrength - 3) unit->Strength = type->MaxStrength;
                        if (unit->Strength == 0) unit->Strength = 1;
                        unit->unknown_70 = unit->Strength;
                        if (unknown_game_mode_a3d298 == 0 || unit->House->Unknown_004F52F0()) {
                            unit->QueueMission(mission, false);
                            if (unit->unknown_slot122()) unit->Commence();
                        } else {
                            unit->unknown_slot261(false, true);
                        }
                    } else {
                        delete unit;
                    }
                }
            }
        }
    }
    for (int link_index = 0; link_index < Units.Count(); link_index++) {
        unsigned linked_link_index = (unsigned)links[link_index];
        Resolve_Unit_Link(link_index,linked_link_index);
    }
}


// static void __fastcall UnitClass::Write_INI(CCINIClass &ini) (0x00706F60)
// Writes the [Units] section: it clears the section, then adds one line per active unit that is on
// the map (hypothesis; the inverse of Read_INI): house, type, strength (0-256), cell x and y,
// facing (0-255), mission, attached tag name ("None" without), the +0x118 value, +0x1AC, +0x79,
// the index of the linked unit (-1 without), +0x379 and +0x37A.

extern char const *unknown_units_section_7addc0;

void __fastcall UnitClass::Write_INI(CCINIClass &ini)
{
    ini.Clear(unknown_units_section_7addc0, 0);
    for (int index = 0; index < Units.Count(); index++) {
        UnitClass *unit = Units[index];
        if (unit != 0 && !unit->unknown_76 && unit->unknown_7d) {
            char id[12];
            sprintf(id, "%d", index);
            int linked = unit->unknown_5b0 != 0 ? Units.ID((UnitClass *)unit->unknown_5b0) : -1;
            char const *tag;
            if (unit->unknown_34 != 0 && unit->unknown_34->Class != 0) {
                tag = (char const *)unit->unknown_34->Class->ID;
            } else {
                tag = "None";
            }
            unsigned raw = *(unsigned const *)&unit->unknown_2e0.Current();
            unsigned direction = (((raw >> 7) + 1) >> 1) & 0xff;
            char line[128];
            sprintf(line, "%s,%s,%d,%d,%d,%d,%s,%s,%d,%d,%d,%d,%d,%d",
                    (char const *)unit->House->IniName, (char const *)unit->Type->ID,
                    (int)(unit->Health_Ratio() * 256.0),
                    unit->unknown_slot104().X, unit->unknown_slot104().Y,
                    direction,
                    MissionClass::Name_From_Mission(unit->Get_Mission()), tag,
                    unit->unknown_118.To_Integer(), unit->unknown_1ac, unit->unknown_79, linked,
                    unit->unknown_379, unit->unknown_37a);
            ini.Put_String(unknown_units_section_7addc0, id, line);
        }
    }
}


// int UnitClass::unknown_707130() (0x00707130): the value of the stored load
// (StorageClass::Get_Total_Value) scaled by the owner's float at +0x148 of its
// type. No call sites: the original inlines it everywhere.

int UnitClass::unknown_707130()
{
    float value = (float)unknown_294.Get_Total_Value();
    return (int)(House->Unknown_004F5B30() * value);
}


// bool UnitClass::unknown_707170(TechnoClass *target) (0x00707170)
// Whether this unit may crush the target (hypothesis): a crusher (or ability
// 17), a target whose type is crushable, within the rules' +0x1390 distance;
// computer houses need AutoCrush or the rules' +0x142F; the owner is not in
// mode 2 and (human houses) is rich enough; the target must not refuse
// slot 49 for this owner.

// The distance between two coordinates (an inline helper; hypothesis).
static inline int Coord_Distance(CoordStruct const &a, CoordStruct const &b)
{
    CoordStruct delta(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    return (int)Fast_Sqrt((double)delta.X * delta.X + (double)delta.Z * delta.Z + (double)delta.Y * delta.Y);
}

bool UnitClass::unknown_707170(TechnoClass *target)
{
    if (!Type->Crusher && !unknown_6d7910(17)) {
        return false;
    }
    if (target && target->Techno_Type_Class()->unknown_1fd) {
        if (Coord_Distance(Center_Coord(), target->Center_Coord()) <= Rules->unknown_1390
            && (!House->Unknown_004F52F0() || Rules->unknown_142f || Type->AutoCrush)
            && House->unknown_c8 != 2
            && (House->Unknown_004F52F0() || House->unknown_194 >= Rules->unknown_10b0)) {
            if (target->unknown_slot49(House)) {
                return false;
            }
            return true;
        }
    }
    return false;
}


// void UnitClass::Scatter(CoordStruct const &source, bool forced, bool nokidding) (0x007072D0, slot 87)
// Scatter (hypothesis): moves the unit off its cell. Units that cannot move
// now (slot 155), teleporting units, units turning, units with a navigation
// target (unless nokidding) and units flagged at +0x5C8 stay. Without a
// source the unit takes the nearest free cell (0x005502E0) as its
// destination; otherwise, unless the mission is paralyzed, it is a unit
// flagged at +0x5B9, it is headed somewhere (and not told nokidding, or
// +0x599) or it has a target (and is not forced, three times in four), it
// tries the eight neighbouring cells starting from the direction away from
// the source (or its body facing) with a random -1..+1 turn, preferring one
// that the tactical view shows at the same cell and is not flagged 0x100,
// and moves there (mission 2).

// The null cell and coordinate (0x00AD0488, 0x00AD04C0) and the height of
// one level (0x00AD0588): the unit's copies of shared/unknown_geometry.hpp's
// constants, which this file does not include (as UnitClass slot 55).
// The direction scale (as UnknownFacingDir's coordinate constructor 0x00423EF0).
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;

void UnitClass::Scatter(CoordStruct const &source, bool forced, bool nokidding)
{
    if (!unknown_slot155()) {
        return;
    }
    UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(Locomotor);
    GUID clsid;
    persist.Checked()->GetClassID(&clsid);
    if (memcmp(&clsid, &CLSID_TeleportLocomotionClass, sizeof(GUID)) == 0) {
        return;
    }
    if (!Control()->unknown_09 && !forced) {
        return;
    }
    if (unknown_2e0.Is_Rotating()) {
        return;
    }
    if (unknown_494 != 0 && !nokidding) {
        return;
    }
    if (unknown_5c8) {
        return;
    }
    if (source == unknown_geometry_l) {
        Cell found = Map.Nearby_Location(Coord_Cell(unknown_slot19(0)), Type->SpeedType, -1, 0, unknown_79,
            Point2D(1, 1), 0, 1, 0, 1, Cell(0, 0), 0);
        if (found != unknown_geometry_k) {
            Assign_Destination(&Map[found], true);
        }
    } else {
        if (Control()->unknown_07) {
            return;
        }
        if ((Kind_Of() != 1 || !unknown_5b9)
            && (unknown_494 == 0 || (nokidding && !unknown_599))
            && (unknown_228 == 0 || forced || Random_Pick(1, 4) == 1)) {
            int facing;
            if (source != unknown_geometry_l) {
                CoordStruct here = Coord;
                UnknownFacingDir dir((unsigned short)((Fast_Atan2((double)source.Y - here.Y, (double)here.X - source.X)
                    - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
                facing = (Dir_To_8(dir) + Random_Pick(0, 2) - 1) & 7;
            } else {
                facing = (Dir_To_8(unknown_2e0.Current()) + Random_Pick(0, 2) - 1) & 7;
            }
            Cell alt(0, 0);
            Cell found(0, 0);
            Cell center = Coord_Cell(unknown_slot19(0));
            CellClass *cell = &Map[center];
            int level = (Is_Moving_Onto_Bridge(0) + cell->unknown_11b) ? 4 : 0;
            CoordStruct here = unknown_slot19(0);
            here.Z = unknown_geometry_g * level;
            for (int index = 0; index < 8; index++) {
                int dir = (facing + index) & 7;
                Cell adjacent = center + AdjacentCell[dir & 7];
                CellClass *next = &Map[adjacent];
                if (Map.Unknown_0055A660(adjacent, true) && Can_Enter_Cell(next, dir, Unknown_005D47D0(), 0, 1) == 0) {
                    if (alt.X == unknown_geometry_k.X) {
                        if (alt.Y == unknown_geometry_k.Y) {
                            alt = adjacent;
                        }
                    }
                    if (found == unknown_geometry_k) {
                        CoordStruct coord = Cell_Center(adjacent);
                        coord.Z = here.Z;
                        if (adjacent == ((UnknownTacticalView *)unknown_tactical_83996c)->Unknown_006A6A50(coord)
                            && !(Map[adjacent].unknown_140 & 0x100)) {
                            found = adjacent;
                            break;
                        }
                    }
                }
            }
            Cell dest = found;
            if (found == unknown_geometry_k) {
                dest = alt;
            }
            if (dest != unknown_geometry_k) {
                QueueMission(2, false);
                Assign_Destination(&Map[dest], true);
            }
        }
    }
}


// bool UnitClass::Limbo() (0x007078F0, slot 52)
// Taking the unit off the map: FootClass's slot 52, and when it succeeds the
// house recorded at +0x5B4 (an index into Houses) hears of the unit's cell
// (0x004E76A0) and the index resets to -1.

bool UnitClass::Limbo()
{
    if (FootClass::Limbo()) {
        if (unknown_5b4 != -1) {
            Houses[unknown_5b4]->Flag_Attach(unknown_slot104(), 0);
            unknown_5b4 = -1;
        }
        return true;
    }
    return false;
}


// int UnitClass::Do_MISSION_GUARD_AREA() (0x00707940, slot 130)
// The FootClass behaviour.

int UnitClass::Do_MISSION_GUARD_AREA()
{
    return FootClass::Do_MISSION_GUARD_AREA();
}


// bool UnitClass::unknown_slot159() (0x00707950, slot 159)
// True when the state at +0x2A8 is in none of its three active phases.

bool UnitClass::unknown_slot159()
{
    if (unknown_2a8.Is_Count_Up_Active() || unknown_2a8.Is_Count_Down_Active() || unknown_2a8.Is_Count_Up_Complete()) {
        return false;
    }
    return true;
}


// void UnitClass::Set_Occupy_Bit(CoordStruct const &coord) (0x00707980, slot 58)
// Sets the vehicle occupation bit (0x20) of the cell at coord: the bridge level's (+0x128)
// when coord is at or above the bridge height (0x00AD057C) over a bridge cell, else the
// ground's (+0x124). ObjectClass slot 58 (0x005D4960) does the same with the 0x40 bit.

// The height a bridge adds (0x00AD057C).

void UnitClass::Set_Occupy_Bit(CoordStruct const &coord)
{
    UnknownCellClass *cell = Map_Cell(coord);
    if (Map.Get_Height_GL(coord) + unknown_geometry_j <= coord.Z && (cell->Flags & 0x100)) {
        cell->unknown_128.Occupy.Vehicle = true;
    } else {
        cell->unknown_124.Occupy.Vehicle = true;
    }
}


// void UnitClass::Clear_Occupy_Bit(CoordStruct const &coord) (0x007079E0, slot 59)
// Clears the vehicle occupation bit (0x20) of the cell at coord: the bridge level's (+0x128)
// when coord is at or above the bridge height (0x00AD057C) else the
// ground's (+0x124). ObjectClass slot 59 (0x005D49E0) does the same with the 0x40 bit.

// The height a bridge adds (0x00AD057C).

void UnitClass::Clear_Occupy_Bit(CoordStruct const &coord)
{
    UnknownCellClass *cell = Map_Cell(coord);
    if (Map.Get_Height_GL(coord) + unknown_geometry_j <= coord.Z) {
        cell->unknown_128.Occupy.Vehicle = false;
    } else {
        cell->unknown_124.Occupy.Vehicle = false;
    }
}


// bool UnitClass::unknown_slot122() (0x00707A40, slot 122)
// Whether the unit may start its deploy state (hypothesis): never in
// missions 6/21 or with +0x5B9; while busy (moving, outside missions 5/1,
// or flagged +0xA4) only when queued for mission 7; the +0x2A8 state must
// be idle. A unit docked with a weapons factory may only while that
// factory queues mission 2 or 7; one standing on a weapons factory's cell
// may unless it is the cell just south of the factory's own.

// A building's type (a view until shared/building.hpp is installed: the
// type pointer follows TechnoClass at +0x418).
struct UnknownUnitBuildingView : TechnoClass {
    BuildingTypeClass *Type;                // +0x418
};

bool UnitClass::unknown_slot122()
{
    if (Mission == 6 || Mission == 21 || unknown_5b9) {
        return false;
    }
    if (MissionQueue != 7 && Locomotor.Checked()->Is_Moving_Now() && Get_Height_AGL() >= 0
        && Get_Mission() != 5 && (Get_Mission() != 1 || unknown_228) && !unknown_a4) {
        return false;
    }
    if (!unknown_2a8.Is_Count_Down_Complete()) {
        return false;
    }
    TechnoClass *contact = Contact_With_Whom(0);
    if (contact) {
        if (contact->Kind_Of() == 6 && ((UnknownUnitBuildingView *)contact)->Type->WeaponsFactory
            && MissionQueue != 2 && MissionQueue != 7) {
            return false;
        }
    } else {
        CoordStruct coord = Coord;
        ObjectClass *building = ((CellClass *)&Map[coord])->Cell_Building();
        if (building && ((UnknownUnitBuildingView *)building)->Type->WeaponsFactory) {
            CoordStruct where = building->Coord;
            Cell factory = Coord_Cell(where);
            CoordStruct here = Coord;
            Cell cell = Coord_Cell(here);
            Cell delta(cell.X - factory.X, cell.Y - factory.Y);
            if (delta.X == 0 && delta.Y == 1) {
                return false;
            }
        }
    }
    return true;
}


// HRESULT __stdcall UnitClass::Load(IStream *stream) (0x00707C20, slot 5)
// Leaves the techno index, loads the foot, rebuilds itself in place while
// keeping the locomotor, rejoins the index and fixes up pointers.

HRESULT __stdcall UnitClass::Load(IStream *stream)
{
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    HRESULT result = FootClass::Load(stream);
    if (SUCCEEDED(result)) {
        ILocomotion *locomotor = Locomotor;
        new (this) UnitClass(NoInitClass());
        Locomotor = locomotor;
        locomotor->Release();
        unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
        Swizzle_Pointer(Type);
        Swizzle_Pointer(unknown_5b0);
        unknown_5c4 = 0;
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall UnitClass::Save(IStream *stream, BOOL clear_dirty) (0x00707DB0, slot 6)
// Releases the object at +0x5C4 first, then FootClass::Save.

HRESULT __stdcall UnitClass::Save(IStream *stream, BOOL clear_dirty)
{
    if (unknown_5c4) {
        unknown_5c4->unknown_4b25c0();
        unknown_5c4 = 0;
    }
    return FootClass::Save(stream, clear_dirty);
}


// void UnitClass::Compute_CRC(WWCRCEngine &crc) const (0x00707DF0, slot 13)
// FootClass's CRC, then the type (kind and ID), the object at +0x5B0, and
// a few unit fields.

void UnitClass::Compute_CRC(WWCRCEngine &crc) const
{
    FootClass::Compute_CRC(crc);
    crc((long)Type->Kind_Of());
    crc(Type->Fetch_ID());
    if (unknown_5b0) {
        crc(unknown_5b0->Fetch_ID());
    }
    crc((long)unknown_5b4);
    crc(unknown_5b8);
    crc(unknown_5b9);
    crc(unknown_5ba);
    crc(unknown_577);
}


// void UnitClass::Detach(AbstractClass *target, bool all) (0x00707E90, slot 10)
// FootClass::Detach, then forgets target as +0x5B0 or as the type.

void UnitClass::Detach(AbstractClass *target, bool all)
{
    FootClass::Detach(target, all);
    if (unknown_5b0 == target) {
        unknown_5b0 = 0;
    }
    if (Type == target) {
        Type = 0;
    }
}


// void UnitClass::Record_The_Kill(TechnoClass *killer) (0x00707ED0, slot 55)
// Springs the tag's events 7 (only with a killer), 48 and 29 for this unit,
// unless +0x290 is set and the tag declines (0x006B41F0); then the techno
// behaviour.

// A cell passed with the unit's events (0x00AD0488; meaning unknown).

void UnitClass::Record_The_Kill(TechnoClass *killer)
{
    if (unknown_34 && (unknown_290 == -1 || !unknown_34->Is_To_Inherit())) {
        if (killer != 0) {
            unknown_34->Spring(7, this, unknown_geometry_k, 0, 0);
        }
        if (unknown_34) {
            unknown_34->Spring(48, this, unknown_geometry_k, 0, 0);
        }
        if (unknown_34) {
            unknown_34->Spring(29, this, unknown_geometry_k, 0, 0);
        }
    }
    TechnoClass::Record_The_Kill(killer);
}


// int UnitClass::Do_MISSION_ATTACK() (0x00707F50, slot 126)
// The FootClass behaviour.

int UnitClass::Do_MISSION_ATTACK()
{
    return FootClass::Do_MISSION_ATTACK();
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// UnitClass::Plan_Route (0x00707F60), from ra2-decomp UnitClass::Plan_Route 0x007447B0 (YR twin).
// Split a subterranean vehicle trip at bridge ends and reachable surface cells.

AbstractClass *UnitClass::Plan_Route_00707f60(AbstractClass *target)
{

    unknown_478.Clear();
    bool leaving_bridge=Is_Moving_Onto_Bridge(0);
    Cell origin=Cell(unknown_slot19(0));
    bool origin_burrow=Map[origin].Can_Burrow_Here() || Get_Height_AGL()<0;
    Cell goal=Cell(target->Center_Coord());
    CellClass *goal_cell=&Map[goal];
    bool goal_bridge=(goal_cell->unknown_140&0x100)!=0;
    bool goal_burrow=goal_cell->Can_Burrow_Here();
    if (leaving_bridge) {
        if (goal_bridge) {
            Cell departure=Map.Unknown_005651E0(origin,goal);
            Cell arrival=Map.Unknown_005651E0(goal,goal);
            if (unknown_709790(&departure,&arrival)) {
                Cell first=Map.Nearby_Location(departure,1,Map.Zone(departure,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                Cell last=Map.Nearby_Location(arrival,1,Map.Zone(departure,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                if (first!=unknown_geometry_k && last!=unknown_geometry_k) {
                    unknown_478.Add(&Map[first]);
                    unknown_478.Add(&Map[last]);
                } else if (Map.Unknown_0054F7C0(origin,goal,0,true,true,false)) {
                    if (departure!=origin) unknown_478.Add(&Map[departure]);
                    unknown_478.Add(&Map[arrival]);
                } else if (first!=unknown_geometry_k) {
                    last=Map.Nearby_Location(goal,1,-1,0,true,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                    if (last!=unknown_geometry_k && first!=last) {
                        unknown_478.Add(&Map[first]);
                        target=&Map[last];
                    } else {
                        Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                        if (alternative==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                        target=&Map[alternative];
                    }
                } else {
                    Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                    if (alternative==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                    target=&Map[alternative];
                }
            } else {
                if (departure!=origin) unknown_478.Add(&Map[departure]);
                unknown_478.Add(&Map[arrival]);
            }
        } else if (goal_burrow) {
            Cell departure=Map.Unknown_005651E0(origin,goal);
            if (unknown_709790(&departure,&goal)) {
                Cell first=Map.Nearby_Location(departure,1,Map.Zone(departure,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                if (first!=unknown_geometry_k) {
                    unknown_478.Add(&Map[first]);
                } else if (Map.Unknown_0054F7C0(origin,goal,0,true,false,false)) {
                    if (departure!=origin) unknown_478.Add(&Map[departure]);
                } else {
                    Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                    if (alternative==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                    target=&Map[alternative];
                }
            } else if (departure!=origin) unknown_478.Add(&Map[departure]);
        } else {
            Cell departure=Map.Unknown_005651E0(origin,goal);
            if (unknown_709790(&departure,&goal)) {
                Cell first=Map.Nearby_Location(departure,1,Map.Zone(departure,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                Cell last=Map.Nearby_Location(goal,1,Map.Zone(goal,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                if (first!=unknown_geometry_k && last!=unknown_geometry_k) {
                    unknown_478.Add(&Map[first]);
                    unknown_478.Add(&Map[last]);
                } else if (Map.Unknown_0054F7C0(origin,goal,0,true,false,false)) {
                    if (departure!=origin) unknown_478.Add(&Map[departure]);
                } else if (first!=unknown_geometry_k) {
                    last=Map.Nearby_Location(goal,1,-1,0,true,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                    if (last!=unknown_geometry_k && last!=first) {
                        unknown_478.Add(&Map[first]);
                        target=&Map[last];
                    } else {
                        Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                        if (replacement==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                        target=&Map[replacement];
                    }
                } else {
                    Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                    if (alternative==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                    target=&Map[alternative];
                }
            } else if (departure!=origin) unknown_478.Add(&Map[departure]);
        }
    } else if (origin_burrow) {
        if (goal_bridge) {
            Cell arrival=Map.Unknown_005651E0(goal,goal);
            if (unknown_709790(&origin,&arrival)) {
                Cell last=Map.Nearby_Location(arrival,1,Map.Zone(arrival,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                if (last!=unknown_geometry_k) {
                    unknown_478.Add(&Map[last]);
                } else if (Map.Unknown_0054F7C0(origin,goal,0,false,true,false)) {
                    if (arrival!=origin) unknown_478.Add(&Map[arrival]);
                } else {
                    last=Map.Nearby_Location(goal,1,-1,0,false,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                    if (last!=unknown_geometry_k) {
                        target=&Map[last];
                    } else {
                        Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                        if (replacement==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                        target=&Map[replacement];
                    }
                }
            } else if (arrival!=origin) unknown_478.Add(&Map[arrival]);
        } else if (!goal_burrow && unknown_709790(&origin,&goal)) {
            Cell last=Map.Nearby_Location(goal,1,Map.Zone(goal,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
            if (last!=unknown_geometry_k) {
                unknown_478.Add(&Map[last]);
            } else if (!Map.Unknown_0054F7C0(origin,goal,0,true,true,false)) {
                last=Map.Nearby_Location(goal,1,-1,0,false,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                if (last!=unknown_geometry_k) {
                    target=&Map[last];
                } else {
                    Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                    if (replacement==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                    target=&Map[replacement];
                }
            }
        }
    } else if (goal_bridge) {
        Cell arrival=Map.Unknown_005651E0(goal,goal);
        if (unknown_709790(&origin,&arrival)) {
            Cell first=Map.Nearby_Location(origin,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
            Cell last=Map.Nearby_Location(arrival,1,Map.Zone(arrival,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
            if (first!=unknown_geometry_k && last!=unknown_geometry_k) {
                unknown_478.Add(&Map[first]);
                unknown_478.Add(&Map[last]);
            } else if (Map.Unknown_0054F7C0(origin,goal,0,false,true,false)) {
                if (arrival!=origin) unknown_478.Add(&Map[arrival]);
            } else if (first!=unknown_geometry_k) {
                last=Map.Nearby_Location(goal,1,-1,0,false,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                if (last!=unknown_geometry_k && first!=last) {
                    unknown_478.Add(&Map[first]);
                    target=&Map[last];
                } else {
                    Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                    if (replacement==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                    target=&Map[replacement];
                }
            } else {
                Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                if (alternative==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                target=&Map[alternative];
            }
        } else if (arrival!=origin) unknown_478.Add(&Map[arrival]);
    } else if (goal_burrow) {
        if (unknown_709790(&origin,&goal)) {
            Cell first=Map.Nearby_Location(origin,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
            if (first!=unknown_geometry_k) {
                unknown_478.Add(&Map[first]);
            } else if (!Map.Unknown_0054F7C0(origin,goal,0,false,false,false)) {
                Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                if (alternative==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                target=&Map[alternative];
            }
        }
    } else if (unknown_709790(&origin,&goal)) {
        Cell first=Map.Nearby_Location(origin,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
        Cell last=Map.Nearby_Location(goal,1,Map.Zone(goal,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
        if (first!=unknown_geometry_k && last!=unknown_geometry_k) {
            unknown_478.Add(&Map[first]);
            unknown_478.Add(&Map[last]);
        } else if (!Map.Unknown_0054F7C0(origin,goal,0,true,true,false)) {
            if (first!=unknown_geometry_k) {
                last=Map.Nearby_Location(goal,1,-1,0,false,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                if (last!=unknown_geometry_k && first!=last) {
                    unknown_478.Add(&Map[first]);
                    target=&Map[last];
                } else {
                    Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                    if (replacement==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                    target=&Map[replacement];
                }
            } else {
                Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                if (alternative==unknown_geometry_k || Get_Height_AGL()<0) return 0;
                target=&Map[alternative];
            }
        }
    }
    unknown_478.Add(target);
    AbstractClass *next=unknown_478[0];
    unknown_478.Delete(0);
    return next;
}


// bool UnitClass::unknown_709790(Cell const *from, Cell const *to) (0x00709790)
// Whether the unit should be routed by the path estimator (hypothesis): true
// unless slot 108 is non-negative, the map accepts the trip, the cells differ,
// are further apart than 1 and closer than 12 (Chebyshev), both pass
// the map's test 0x0055A660 and the estimator (0x0042AA30) finds the path no
// longer than 15.

extern "C" int __cdecl abs(int value);
#pragma intrinsic(abs)

bool UnitClass::unknown_709790(Cell const *from, Cell const *to)
{
    if (Get_Height_AGL() >= 0 && Map.Unknown_0054F7C0(*from, *to, 0, false, 0, false)) {
        if (from->X == to->X && from->Y == to->Y) {
            return false;
        }
        int dy = abs(to->Y - from->Y);
        int dx = abs(to->X - from->X);
        int distance = dx > dy ? dx : dy;
        if (distance == 1) {
            return false;
        }
        if (distance < 12 && Map.Unknown_0055A660(*from, true) && Map.Unknown_0055A660(*to, true)) {
            return Search.Test_Cell_Walk(*from, *to, this, false, (Map_Cell_At(*to)->Flags >> 8) & 1, 0) > 15;
        }
    }
    return true;
}


// bool UnitClass::INoticeSink_Unknown(int code, int arg) (0x00709890;
// UnitClass's INoticeSink vtable 0x007ADDD4, `this` adjusted from the +8
// subobject)
// Notice 0x117B for a BalloonHover or JumpJet type in radio contact: tell
// the contact message 8, then attack the target at +0x1B0 unless it is the
// navigation target (+0x494), otherwise clear the navigation and move to the
// zero coordinate. Notice 0x117C for a Crashable type: slot 109 (0), the
// death explosion (or, for a BalloonHover type, the death weapon) and slot
// 60. True when handled. (InfantryClass 0x00509590 is the same notice pair.)

bool UnitClass::INoticeSink_Unknown(int code, int arg)
{
    if ((Type->BalloonHover || Type->JumpJet) && code == 0x117B && HasAnyLink()) {
        unknown_slot149(8);
        if (unknown_1b0 != 0 && unknown_1b0 != unknown_494) {
            Assign_Destination(unknown_1b0, true);
            return true;
        }
        Unknown_004CD9B0();
        Scatter(unknown_geometry_l, true, false);
        return true;
    }
    if (Type->Crashable && code == 0x117C) {
        Set_Height_AGL(0);
        if (!Type->BalloonHover) {
            Explode();
        } else {
            unknown_6d7e20(0);
        }
        Delete_Me();
        return true;
    }
    return false;
}


// bool UnitClass::unknown_slot222(HouseClass *house, int arg2) (0x007099A0, slot 222)
// house is the new owner (hypothesis: a change of owner). Nothing for the current
// owner; otherwise clears +0x5B8, passes the change on to the unit at
// +0x5B0 (which keeps it, flagged at its +0x5B8), then the foot behaviour
// with arg2 forced to 1.

bool UnitClass::unknown_slot222(HouseClass *house, int arg2)
{
    if (house != House) {
        unknown_5b8 = false;
        UnitClass *unit = (UnitClass *)unknown_5b0;
        if (unit) {
            unit->unknown_slot222(house, 1);
            unknown_5b0 = unit;
            unit->unknown_5b8 = true;
        }
        return FootClass::unknown_slot222(house, 1);
    }
    return false;
}


// bool UnitClass::unknown_slot31() (0x00709A00, slot 31)
// True unless the type is NonVehicle.

bool UnitClass::unknown_slot31()
{
    return !Type->NonVehicle;
}


// void UnitClass::ReceiveGunner(int arg1) (0x00709A20, slot 280)
// arg1 is a techno whose owned object at +0x208 moves to this unit (the
// IFV gunner hand-over; hypothesis): selects weapon 7, and when the
// donor's +0x244 timer was still running restarts this unit's (slot 187
// with 7, counted at +0x310). Either way the donor type's IFVMode weapon is
// then selected (the call is outside the guards).

void UnitClass::ReceiveGunner(int arg1)
{
    TechnoClass *from = (TechnoClass *)arg1;
    UnknownTechno208 *object = from->unknown_208;
    if (object) {
        int running = 0;
        if (from->unknown_244.Value()) {
            running = 1;
        }
        unknown_208 = object;
        object->unknown_24 = (int)this;
        from->unknown_208 = 0;
        unknown_6d83f0(7);
        if (running) {
            unknown_310++;
            unknown_244 = CDTimerClass<FrameTimerClass>(unknown_slot187(7));
        }
    }
    unknown_6d83f0(from->Techno_Type_Class()->IFVMode);
}


// void UnitClass::RemoveGunner(int arg1) (0x00709AE0, slot 281)
// The reverse of slot 280: this unit's owned object at +0x208 moves to the
// techno arg1 (refreshed through 0x006E2500 when its +0x28 is set); when
// this unit's +0x244 timer was still running the receiver's restarts
// (slot 187 with 0, counted at +0x310). Either way this unit then selects
// weapon 0 (the call is outside the guards).

void UnitClass::RemoveGunner(int arg1)
{
    TechnoClass *to = (TechnoClass *)arg1;
    if (to && unknown_208) {
        int running = 0;
        if (unknown_244.Value()) {
            running = 1;
        }
        to->unknown_208 = unknown_208;
        unknown_208->unknown_24 = (int)to;
        unknown_208 = 0;
        if (to->unknown_208 && to->unknown_208->unknown_28) {
            to->unknown_208->unknown_6e2500();
        }
        if (running) {
            to->unknown_310++;
            to->unknown_244 = CDTimerClass<FrameTimerClass>(to->unknown_slot187(0));
        }
    }
    unknown_6d83f0(0);
}


// ObjectTypeClass *UnitClass::unknown_slot50(bool arg1) (0x00709BB0, slot 50)
// The type as the player sees it: the real type for the player's allies
// (unless arg1), otherwise the disguise at +0x40C.

ObjectTypeClass *UnitClass::unknown_slot50(bool arg1)
{
    if (House->Is_Ally(unknown_player_a35db4) && !arg1) {
        return Type;
    }
    return (ObjectTypeClass *)unknown_40c;
}


// HouseClass *UnitClass::unknown_slot51(bool arg1) (0x00709BF0, slot 51)
// The owner as the player sees it: the real owner for the player's allies
// (unless arg1), otherwise the disguise's house at +0x410.

HouseClass *UnitClass::unknown_slot51(bool arg1)
{
    if (House->Is_Ally(unknown_player_a35db4) && !arg1) {
        return House;
    }
    return (HouseClass *)unknown_410;
}


// void UnitClass::Disguise_As_Type(ObjectTypeClass *type) (0x00709C30)
// Starts a disguise (hypothesis): a given type becomes the shown type (+0x40C)
// with no house (+0x410), the flag at +0x170 is set, the frame is stamped at
// +0x174 and slot 266 reacts.

void UnitClass::Disguise_As_Type(ObjectTypeClass *type)
{
    if (type) {
        unknown_40c = type;
        unknown_410 = 0;
        unknown_170 = true;
        unknown_174 = Frame;
        unknown_slot266();
    }
}


// Current slot255 signature is TechnoClass*, but original Kind_Of cases also
// accept CellClass(11) and TerrainClass(36). Preserve the declared ABI;
// the observed branch identities justify the local casts, not a global rewrite.
void UnitClass::unknown_slot255(TechnoClass *target)
{
    if (!target) return;
    switch (target->Kind_Of()) {
    case 1:
        if (!target->unknown_slot48()) return;
        unknown_40c = target->unknown_slot50(true);
        unknown_410 = target->unknown_slot51(true);
        break;
    case 11:
        if (!unknown_slot208(target)) return;
        unknown_40c = OverlayTypes[((CellClass *)target)->OverlayIndex];
        unknown_410 = 0;
        break;
    case 36:
        unknown_40c = target->Class_Of();
        unknown_410 = 0;
        TechnoClass::unknown_slot255(target);
        return;
    default:
        return;
    }
    TechnoClass::unknown_slot255(target);
}


// void UnitClass::unknown_slot256() (0x00709D20, slot 256)
// Clears the flag at +0x170, runs slot 266 and forgets +0x40C/+0x410.

void UnitClass::unknown_slot256()
{
    unknown_170 = false;
    unknown_slot266();
    unknown_40c = 0;
    unknown_410 = 0;
}


// bool UnitClass::unknown_slot49(HouseClass *house) (0x00709D50, slot 49)
// arg1 is a house. True while slot 48 holds, the house is not allied with
// the owner, its bit 0x0047C300 of this cell is clear, and the disguise's
// house (+0x410) is that house, none, or one of its allies.

bool UnitClass::unknown_slot49(HouseClass *house)
{
    if (unknown_slot48()) {
        Cell position = Coord_Cell(Center_Coord());
        CellClass &cell = Map[position];
        if (!House->Is_Ally(house) && !cell.unknown_47c300(house->ArrayIndex)) {
            HouseClass *disguise = (HouseClass *)unknown_410;
            if (disguise == house || !disguise || house->Is_Ally(disguise)) {
                return true;
            }
        }
    }
    return false;
}


// void UnitClass::Init() (0x00709E10, slot 9)
// The techno initialisation, then veterancy: units of a type the owner's
// house type lists (+0x168), and trainable land units of a house with
// +0x207, start as veterans; Gunner types select weapon 0.

void UnitClass::Init()
{
    TechnoClass::Init();
    if (Type) {
        if (House) {
            DynamicVectorClass<UnitTypeClass *> &veterans = Unit_House_View(House)->unknown_34->unknown_168;
            for (int index = 0; index < veterans.Count(); index++) {
                if (veterans[index] == Type) {
                    unknown_118.Set_Veteran(true);
                    break;
                }
            }
        }
        if (House && Unit_House_View(House)->unknown_207 && !Type->Naval && Type->Trainable) {
            unknown_118.Set_Veteran(true);
        }
        if (Type->Gunner) {
            unknown_6d83f0(0);
        }
    }
}


// Disguise-when-still AI and nearby hostile-infantry suppression.
static inline TypeList<TerrainTypeClass *> &Unit_Default_Mirage_Disguises()
{
    return Rules->DefaultMirageDisguises;
}
static inline int Unit_Infantry_Blink_Disguise_Time()
{
    return Rules->InfantryBlinkDisguiseTime;
}
void UnitClass::UpdateDisguise()
{
    {
        bool can_disguise = false;
        if (!unknown_slot48() && !Locomotor.Checked()->Is_Moving() && Type->DisguiseWhenStill && !Contact(0))
            can_disguise = true;
        if (Locomotor.Checked()->Is_Moving()) goto clear_disguise;
        if (Frame % 8 != 0) {
            Cell position = unknown_slot104();
            bool bridge = ((CellClass *)Get_Cell_Ptr())->unknown_47b960();
            for (int facing = 0; facing < 8; facing++) {
                Cell adjacent = AdjacentCell[facing & 7] + position;
                CellClass &tile = Map[adjacent];
                TechnoClass *nearby = (TechnoClass *)tile.Cell_Infantry(bridge);
                if (nearby && !House->Is_Ally(nearby)) {
                    unknown_178 = CDTimerClass<FrameTimerClass>(Unit_Infantry_Blink_Disguise_Time());
                    goto clear_disguise;
                }
            }
        }
        if (!can_disguise || unknown_178.Value() != 0) return;
        int choice = unknown_id_source_a3d290->Random()(0, Unit_Default_Mirage_Disguises().Count() - 1);
        int last = Unit_Default_Mirage_Disguises().Count() - 1;
        if (choice >= last) choice = last;
        ObjectTypeClass *type = Unit_Default_Mirage_Disguises()[choice];
        if (type) {
            unknown_40c = type;
            unknown_410 = 0;
            unknown_170 = true;
            unknown_174 = Frame;
            unknown_slot266();
        }
        return;
    }
clear_disguise:
    unknown_slot256();
}


// wchar_t const *UnitClass::unknown_slot35() (0x0070A0E0, slot 35)
// The name to show: a disguised unit (slot 49 for the player, with a
// disguise type and house) shows the disguise's name; a unit with an
// occupied passenger slot (0x006D83E0/0x006D83D0) shows its type's name
// prefixed by the first passenger's role (Tip:Repair for a passenger whose
// slot 193 holds, Tip:MachineGun for a non-civilian infantry whose weapon's
// warhead has Bullets, otherwise its own name; Tip:Rocket by default).

// An infantry passenger's type (a view until shared/infantry.hpp is installed).
struct UnknownUnitInfantryView : TechnoClass {
    char unknown_418[0x5A8 - 0x418];
    InfantryTypeClass *Type;                // +0x5A8
};

wchar_t const *UnitClass::unknown_slot35()
{
    if (unknown_slot49(unknown_player_a35db4) && unknown_slot50(true)) {
        if (!unknown_slot51(true)) {
            return 0;
        }
        return unknown_slot50(true)->UIName;
    }
    if (unknown_6d83e0() && unknown_6d83d0()) {
        wchar_t const *role = 0;
        TechnoClass *passenger = (TechnoClass *)Cargo.Attached_Object();
        if (passenger && passenger->Kind_Of() == 15) {
            if (passenger->unknown_slot193()) {
                role = StringTable::Load_String("Tip:Repair", 0, "D:\\Ra2\\Unit.CPP", 10328);
            } else if (!((UnknownUnitInfantryView *)passenger)->Type->Civilian) {
                role = passenger->Class_Of()->UIName;
                WeaponStruct *weapon = passenger->unknown_slot230();
                if (weapon && weapon->WeaponType && weapon->WeaponType->Warhead && weapon->WeaponType->Warhead->Bullets) {
                    role = StringTable::Load_String("Tip:MachineGun", 0, "D:\\Ra2\\Unit.CPP", 10341);
                }
            }
        }
        if (!role) {
            role = StringTable::Load_String("Tip:Rocket", 0, "D:\\Ra2\\Unit.CPP", 10349);
        }
        swprintf(unknown_5d0, L"%s %s", role, Type->UIName);
        return unknown_5d0;
    }
    return Type->UIName;
}


// 0x0070a240: function not yet identified
//
// Body: 27 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __fastcall FUN_0070a240(int param_1)
// Callees with accepted source:
//   Is_Immobilized_006d8e80 -> src/recovered/006d8e80.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0070a240 {

extern unsigned int __fastcall Is_Immobilized_006d8e80(void);

unsigned int __fastcall FUN_0070a240(int object_address)
{
    char result_byte;

    if (*(int *)(object_address + 0x5c0) == -1) {
        result_byte = (char)(Is_Immobilized_006d8e80());
        if (result_byte == '\0') {
            return 0;
        }
    }
    return 1;
}

}  // namespace seed_0070a240


// bool UnitClass::unknown_slot274() (0x0070A260, slot 274)
// Tail jump to the existing out-of-line TechnoClass implementation.

bool UnitClass::unknown_slot274()
{
    return TechnoClass::unknown_slot274();
}


// bool UnitClass::unknown_slot275() (0x0070A270, slot 275)
// Tail jump to the existing out-of-line TechnoClass implementation.

bool UnitClass::unknown_slot275()
{
    return TechnoClass::unknown_slot275();
}


// int UnitClass::Size_Of() const (0x0070A280, slot 12): inline in shared/unit.hpp, emitted with the unit's vtable; no source of its own.


// HRESULT __stdcall UnitClass::GetClassID(GUID *clsid) (0x0070A290, slot 3): inline in shared/unit.hpp, emitted with the unit's vtable; no source of its own.


// int UnitClass::Kind_Of() const (0x0070A2D0, slot 11): inline in shared/unit.hpp, emitted with the unit's vtable; no source of its own.


// UnknownFacingDir UnitClass::unknown_slot162() (0x0070A2E0, slot 162)
// Inline in shared/unit.hpp (the turret facing for a turreted type,
// otherwise the body's); emitted with the unit's vtable. No source of its own.


// void *UnitClass::`scalar deleting destructor'(unsigned int flags) (0x0070A330, slot 8)
// Compiler-generated with the unit's vtable; it calls the destructor out of
// line. No source of its own.


// Voxel cache index: signed keys, lazy sorting and archive lookup.
bool UnknownBuffer::Is_Present_0070a350(int const &id) const
    {
        if (unknown_04==0) return false;
        if (Is_Archive_Same_0070a3f0(id)) return true;
        NodeElement const *node=Search_For_Node_006d9850(id);
        if (node) {
            const_cast<UnknownBuffer *>(this)->unknown_10=node;
            return true;
        }
        return false;
    }


// Voxel cache index: signed keys, lazy sorting and archive lookup.
bool UnknownBuffer::Is_Archive_Same_0070a3f0(int const &id) const
    {
        NodeElement const *node=unknown_10;
        if (node && node->ID==id) return true;
        return false;
    }
