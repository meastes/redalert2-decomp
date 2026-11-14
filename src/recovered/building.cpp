// building.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/building.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_ALL
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
#define RA2_BUILDING_CLOAK_GENERATOR  // BuildingClass Enable_/Disable_Cloak_Generator members for Captured 0x00444660
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/house.hpp"
#include "shared/building.hpp"
#include "shared/cell.hpp"
#include "shared/unit.hpp"
#include "shared/aircraft.hpp"
#include "shared/sidebar.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/tactical.hpp"
#include "shared/id_source.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
#include "shared/fast_math.hpp"
#include "shared/rules.hpp"
#include "shared/coord_distance.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/shape_file.hpp"
#include "shared/matrix3d.hpp"
#include "shared/house_type.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/weapon_type.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/smudge_type.hpp"
#include "shared/overlay.hpp"
#include "shared/infantry.hpp"
#include "shared/options.hpp"
#include "shared/tileset_index.hpp"
#include "shared/sound_play.hpp"
#include "shared/overlay_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/light_source.hpp"
#include "shared/building_light.hpp"
#include "shared/base.hpp"
#include "shared/abstract.hpp"
#include "shared/vector_class_decl.hpp"
#include "shared/bullet_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/debug_printf.hpp"
#include "shared/event.hpp"
#include "shared/target.hpp"
#include "shared/vox_class.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/unit_type.hpp"
#include "shared/infantry_type.hpp"
#define MAP_CELL(cell) ((UnknownBuildingCell52 &)Cell_At(cell))
#include "shared/map_view.hpp"
#include "shared/radar_event.hpp"
#include "shared/keyboard.hpp"
#include "shared/crt_memory.hpp"
#include "shared/foot.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/object_type.hpp"
#include "shared/tag.hpp"
#include "shared/storage_class.hpp"
#include "shared/rgb_class.hpp"
#include "shared/laser_draw.hpp" // canonical constructor declaration
#include "shared/locomotion.hpp"
#include "shared/object.hpp"
#include "shared/progress_timer.hpp"
#include "shared/ccini_class.hpp"
#include "shared/building_type_list.hpp"
#include "shared/ini_class.hpp"
#include "shared/bullet.hpp"
#define VECTOR_IMPLEMENT_ALL
#include "shared/cell_class.hpp"
#include "shared/techno_map.hpp"
#include "shared/win32_window.hpp"
#include "shared/surface.hpp"
#include "shared/display.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/hicolor.hpp"
#include "shared/cd_timer.hpp"
#include "shared/crt_math.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/fogged_object.hpp"
#include "shared/team.hpp"
#include "shared/side.hpp"
#include "shared/com_ptr.hpp"


// Header-static dynamic initializers of the unit at 0x00438500:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00438500-0x004386E6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00438530, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x00438550, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x00438570, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x00438590, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x004385B0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x004385D0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x00438610, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x00438640, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x00438670, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x004386A0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x004386B0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Dynamic initializer (0x004386D0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00438500.cpp; no source of its own.


// Test the player's active psychic detectors against an unallied house's cell.
// Free/static source ownership and input constness remain hypotheses.
bool __fastcall Building_Player_Detector_Covers_Cell_00438a60(HouseClass const *house, CellClass const *cell)
{
    if (unknown_player_a35db4->Is_Ally(house))
        return false;
    int count = unknown_player_a35db4->unknown_b0.Count();
    for (int index = 0; index < count; ++index) {
        BuildingClass *detector = unknown_player_a35db4->unknown_b0[index];
        if (detector->unknown_slot195()) {
            CoordStruct position = detector->Center_Coord();
            CoordStruct other = cell->unknown_475d60();
            CoordStruct d((int)((unsigned)position.X-(unsigned)other.X),(int)((unsigned)position.Y-(unsigned)other.Y),(int)((unsigned)position.Z-(unsigned)other.Z));
            int distance = (int)unknown_sqrt_4ba410((double)d.X*d.X+(double)d.Y*d.Y+(double)d.Z*d.Z);
            int radius = (int)((unsigned int)detector->Type->PsychicDetectionRadius << 8);
            if (distance <= radius)
                return true;
        }
    }
    return false;
}


// PRIVATE complete145-body+15-padding animation factory; now compiled under approved declarations.
// Original first stack argument is attachment; second is name (proved from original prologue and loads).
// The first argument is the owner house (SuperClass::Place 0x0069D850 passes House; the +0x160 setter
// 0x00422690 keeps it); the receiver is any AbstractClass (Place passes a cell).
AnimClass *__fastcall Create_Animation_At_Object_00438b80(HouseClass *house,AbstractClass *object,
    AbstractClass *attachment,char const *name)
{
    int index=AnimTypeClass::From_Name(name);
    AnimTypeClass *type=AnimTypes[index];
    CoordStruct position=object->Center_Coord();
    AnimClass *animation=new AnimClass(type,position,0,1,0x600,0);
    animation->unknown_func_422680(attachment);
    animation->unknown_func_422690(house);
    animation->unknown_17d=true;
    return animation;
}


// Not exact (RA2_NON_MATCHING): Building_Player_Detector_Covers_Target_004386f0 at 0x004386F0, best score 82.58, 2026-11-13; differs: 265 instructions in the original, 263 in the draft (shape 97.35)
#ifdef RA2_NON_MATCHING
// bool __fastcall Building_Player_Detector_Covers_Target_004386f0(FootClass *object) (0x004386F0)
// Tests the player's active psychic detectors against an unallied object's
// target and the last entry of its navigation queue.
static inline UnknownVector3 Detector_Offset(CoordStruct const &a, CoordStruct const &b)
{
    return UnknownVector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}

bool __fastcall Building_Player_Detector_Covers_Target_004386f0(FootClass *object)
{
    if (!unknown_player_a35db4->Is_Ally(object->House)) {
    int count = unknown_player_a35db4->unknown_b0.Count();
    for (int index = 0; index < count; ++index) {
        BuildingClass *detector = unknown_player_a35db4->unknown_b0[index];
        if (detector->unknown_slot195()) {
            AbstractClass *target = object->unknown_228;
            if (target != 0) {
                if (target->AbstractFlags.IsTechno) {
                    CoordStruct position = detector->Center_Coord();
                    CoordStruct other = target->Center_Coord();
                    UnknownVector3 offset = Detector_Offset(position, other);
                    bool hit = offset.Length() <= detector->Type->PsychicDetectionRadius * 256;
                    if (hit) {
                        return hit;
                    }
                } else {
                    bool hit = target->Kind_Of() == 11;
                    if (hit) {
                        CoordStruct position = detector->Center_Coord();
                        CoordStruct other = ((CellClass *)target)->unknown_475d60();
                        UnknownVector3 offset = Detector_Offset(position, other);
                        hit = offset.Length() <= detector->Type->PsychicDetectionRadius * 256;
                    }
                    if (hit) {
                        return hit;
                    }
                }
            }
            if (object != 0) {
            bool const &is_foot = object->AbstractFlags.IsFoot;
            if (is_foot) {
                AbstractClass *nav = object->unknown_478.Count() == 0 ? object->unknown_494 : object->unknown_478[object->unknown_478.Count() - 1];
                if (nav != 0) {
                    bool hit = nav->Kind_Of() == 11;
                    if (hit) {
                        CoordStruct position = detector->Center_Coord();
                        CoordStruct other = ((CellClass *)nav)->unknown_475d60();
                        UnknownVector3 offset = Detector_Offset(position, other);
                        double y = offset.Y;
                        double z = offset.Z;
                        double x = offset.X;
                        hit = (int)unknown_sqrt_4ba410(x * x + z * z + y * y) <= detector->Type->PsychicDetectionRadius * 256;
                        if (hit) {
                            return hit;
                        }
                    } else if (nav->AbstractFlags.IsTechno) {
                        CoordStruct position = detector->Center_Coord();
                        CoordStruct other = nav->Center_Coord();
                        UnknownVector3 offset = Detector_Offset(position, other);
                        hit = offset.Length() <= detector->Type->PsychicDetectionRadius * 256;
                        if (hit) {
                            return hit;
                        }
                    }
                }
            }
            }
        }
    }
    }
    return false;
}
#endif

// BuildingClass::BuildingClass(NoInitClass const &noinit) (0x00438C20)
// The loading constructor: rebuilds the stage and the two infantry lists,
// which have no NoInitClass constructors.

BuildingClass::BuildingClass(NoInitClass const &noinit) :
    TechnoClass(noinit),
    unknown_420(noinit),
    unknown_448(noinit),
    GateTimer(noinit),
    unknown_5a0(noinit)
{
}


// BuildingClass::BuildingClass(BuildingTypeClass *type, HouseClass *house) (0x00438CE0)
// Default state for a type and owner; faces the type's initial direction,
// joins Buildings, the lists at 0x00AC0C80/0x00AC0BF8 and the techno index,
// and sizes the radio links from the type (at least one).

BuildingClass::BuildingClass(BuildingTypeClass *type, HouseClass *house) :
    TechnoClass(house),
    Type(type),
    unknown_41c(0),
    unknown_42c(-1),
    unknown_430(-1),
    unknown_434(0),
    unknown_438(0),
    unknown_43c(0),
    unknown_440(0),
    unknown_444(0),
    unknown_4bc(false),
    unknown_4cc(-1),
    unknown_4d0(-1),
    unknown_4d4(0),
    GateTimer(0),
    unknown_4e8(0),
    unknown_4ec(0),
    unknown_4f0(0),
    unknown_50c(unknown_static_rect),
    unknown_51c(unknown_geometry_l),
    unknown_528(Point2D(0, 0)),
    unknown_530(true),
    unknown_531(false),
    unknown_534(0),
    unknown_538(false),
    unknown_539(false),
    unknown_56c(0),
    unknown_598(false),
    unknown_599(false),
    unknown_59a(false),
    unknown_59b(false),
    unknown_59c(false),
    unknown_5ac(true),
    unknown_5ad(false),
    unknown_5ae(false),
    unknown_5af(false),
    unknown_5b0(false),
    unknown_5b1(false),
    unknown_5b2(false),
    unknown_5b3(false),
    unknown_5b4(false),
    unknown_5b5(false),
    unknown_5b6(false),
    unknown_5b7(false),
    unknown_5b8(false),
    unknown_5b9(false),
    unknown_5ba(true),
    unknown_5bb(0),
    unknown_5bc(0),
    unknown_5bd(false),
    unknown_5c0(0),
    unknown_5c4(1000),
    unknown_5c6(false),
    unknown_5c7(-1),
    unknown_5c8(0),
    unknown_5d8(0)
{
    Create_ID();
    for (int i = 0; i < 3; i++) {
        unknown_4c0[i] = 0;
    }
    unknown_5cc=CoordStruct(0,0,0);
    if (Type) {
        unknown_2c8.Set_Desired(UnknownFacingDir(Type->BarrelStartPitch << 8));
    }
    memset(unknown_454, 0, sizeof(unknown_454));
    for (int index = 0; index < 8; index++) {
        unknown_49c[index] = 0;
    }
    unknown_570.Init();
    unknown_584.Init();
    unknown_2c8.Set_Desired(UnknownFacingDir(0x4000));
    unknown_4f4.Value = 0;
    unknown_BuildingClass_list_00a40af8.Add(this);
    Init();
    unknown_abstracts_ac0c80.Add(this);
    unknown_abstracts_ac0bf8.Add(this);
    unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
    if (Type) {
        int count = Type->NumberOfDocks;
        if (count < 1) {
            count = 1;
        }
        SetLinkCount(count);
    } else {
        SetLinkCount(1);
    }
}


// BuildingClass::~BuildingClass() (0x00439240)
// Stops the audio controllers, releases the owned objects, leaves Buildings,
// the lists at 0x00AC0BF8/0x00AC0C80 and the techno index, updates the
// owner, and empties the two infantry lists.

// Removes an object from one of the object lists (an inline wrapper).
static inline bool Remove_From(DynamicVectorClass<AbstractClass *> &list, AbstractClass *object)
{
    return list.Delete(object);
}

// Empties an infantry list but keeps its capacity (an inline helper).
static inline void Reset_List(DynamicVectorClass<InfantryClass *> &list)
{
    int size = list.Length();
    list.Clear();
    list.Resize(size);
}

BuildingClass::~BuildingClass()
{
    unknown_584.Unknown_00406070();
    unknown_570.Unknown_00406070();
    unknown_584.Unknown_00405C10();
    unknown_570.Unknown_00405C10();
    if (unknown_4e8) {
        unknown_4e8->Deactivate(0);
        delete unknown_4e8;
        unknown_4e8 = 0;
    }
    Unknown_006ECB90(this, true);
    Limbo();
    unknown_BuildingClass_list_00a40af8.Delete(this);
    unknown_44cfd0(-2);
    Type->Free_Buildup();
    unknown_4397c0();
    if (unknown_flag_a40958 && Type && Type->CloakGenerator) {
        unknown_5bb = -1;
        if (!unknown_5bc) {
            unknown_5bc = Type->CloakRadiusInCells;
        }
        unknown_75 = true;
        unknown_5bc = 1;
        Cloaking_AI(1);
    }
    if (unknown_452800() != -1 || unknown_452860() != -1) {
        House->Update_Present_Super_Weapons();
    }
    if (unknown_flag_a40958) {
        if (Strength != unknown_43c) {
            House->unknown_5668 = true;
        }
        if (unknown_flag_a40958 && Type) {
            if (House) {
                House->unknown_4ea800(this);
            }
            Limbo();
        }
    }
    delete unknown_41c;
    unknown_41c = 0;
    Remove_From(unknown_abstracts_ac0bf8, this);
    Remove_From(unknown_abstracts_ac0c80, this);
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    Type = 0;
    unknown_7d = false;
    Reset_List(unknown_53c);
    Reset_List(unknown_554);
}


// Create damage-fire animations at the structure's art offsets.
extern Point2D const Damage_Fire_Offset_End_0084ed30;

inline CoordStruct Projected_Coord_0043bbd0(Point2D const &point);
inline void Accumulate_Coord_004395f0(CoordStruct &destination, CoordStruct const &offset)
{
    destination.X += offset.X;
    destination.Y += offset.Y;
    destination.Z += offset.Z;
}
void BuildingClass::Create_Damage_Fires_004395f0()
{
    int type_count = Rules->DamageFireTypes.Count();
    if (type_count != 0) {
        int type_index = unknown_id_source_a3d290->Random()(0, type_count - 1);
        for (int index = 0; index < 8; ++index) {
            if (Type->DamageFireOffset[index] == Damage_Fire_Offset_End_0084ed30) return;
            if (unknown_49c[index] != 0) return;
            CoordStruct coordinate = Projected_Coord_0043bbd0(unknown_tactical_83996c->Unknown_006A2900(Type->DamageFireOffset[index]));
            Accumulate_Coord_004395f0(coordinate, unknown_slot42());
            AnimClass *fire = new AnimClass(Rules->DamageFireTypes[type_index], coordinate, 0, 1, 0x600, 0);
            if (fire) {
                unknown_49c[index] = fire;
                fire->ZAdjust = ((Type->DamageFireOffset[index].Y - (Type->Width() + Type->Foundation_Height(false)) * 15) * 3 >> 1) - 10;
                if (fire->ZAdjust > 0) fire->ZAdjust = 0;
                int frames = fire->Class->End;
                if (frames > 0) fire->Stage = unknown_id_source_a3d290->Random()(0, frames - 1);
                ++type_index;
                if (type_index >= type_count) type_index = 0;
            }
        }
    }
}


// void BuildingClass::unknown_4397c0() (0x004397C0)
// Calls slot 60 on each of the eight objects at +0x49C and clears them.
// No direct callers: inlined into the destructor, which is the evidence for
// the call there.

void BuildingClass::unknown_4397c0()
{
    for (int index = 0; index < 8; index++) {
        if (unknown_49c[index]) {
            unknown_49c[index]->Delete_Me();
            unknown_49c[index] = 0;
        }
    }
}


// Building radio dispatch: load permission, docking maintenance and construction.
// Message values and sender downcasts follow the observed original ABI.


// Hypothesis: the reference three-axis object-distance inline operation.
// This is distinct from the accepted planar ObjectClass::Distance_To.
inline int Building_Radio_Object_Distance_004397f0(ObjectClass const &origin,AbstractClass const &target)
{
    CoordStruct delta=origin.Center_Coord()-target.Center_Coord();
    return (int)unknown_sqrt_4ba410((double)delta.X*delta.X+(double)delta.Z*delta.Z+(double)delta.Y*delta.Y);
}
inline FootClass *Building_Radio_Foot_004397f0(RadioClass *sender)
{
    if (!sender) return 0;
    return sender->AbstractFlags.IsFoot?static_cast<FootClass *>(sender):0;
}
inline TechnoClass *Building_Radio_Infantry_004397f0(RadioClass *sender)
{
    if (!sender) return 0;
    return sender->Kind_Of()==15?static_cast<TechnoClass *>(sender):0;
}

inline CoordStruct Position_Coord_00450950(ObjectClass const &object);

int BuildingClass::Receive_Message(RadioClass *radio,int message,AbstractClass *&param)
{
    TechnoClass *sender=static_cast<TechnoClass *>(radio);
    switch (message) {
    case 15:
        TechnoClass::Receive_Message(radio,message,param);
        if (!House->Is_Ally(sender)) return 0;
        if (Get_Mission()==18 || Get_Mission()==19 || unknown_42c==0 ||
            (!unknown_a40774 && !Unknown_00636ED0(sender))) return 10;
        if (sender->Techno_Type_Class()->MovementZone!=5 &&
            ((Techno_Type_Class()->Naval && !sender->Techno_Type_Class()->Naval) ||
             (!Techno_Type_Class()->Naval && sender->Techno_Type_Class()->Naval))) return 10;
        if (sender->Techno_Type_Class()->BalloonHover) return 10;
        if (!unknown_530) return 10;
        if (Type->UnitRepair) {
            if (radio->Kind_Of()==1 || radio->Kind_Of()==2) {
                if (unknown_slot150(35,sender)!=1) return 1;
            }
            return 10;
        }
        if ((Type->Armory || Type->Hospital) && radio->Kind_Of()==15) {
            TechnoClass *infantry=Building_Radio_Infantry_004397f0(radio);
            if (infantry->unknown_230 && static_cast<UnknownTechno230 *>(infantry->unknown_230)->unknown_24) return 10;
            if (infantry->unknown_234) return 10;
            return unknown_254!=0?1:10;
        }
        if (Type->Helipad) return radio->What_Am_I()==2?1:10;
        if (Type->DockUnload && radio->Kind_Of()==1 && static_cast<UnitClass *>(radio)->Type->Harvester) {
            if (unknown_a40774 || !Cargo.Object) return 1;
        }
        if (Type->Weeder && radio->Kind_Of()==1 && static_cast<UnitClass *>(radio)->Type->Weeder) {
            if (unknown_a40774 || !Cargo.Object) return 1;
        }
        return 0;
    case 21:
        if (Get_Mission()==19) return 10;
        if (Type->UnitRepair || Type->UnitReload || Type->Hospital || Type->Armory) {
            unknown_5ad=true;
            QueueMission(20,false);
            radio->QueueMission(0,false);
            return 1;
        }
        if (Type->DockUnload) { radio->QueueMission(16,false); return 1; }
        break;
    case 14: {
        TechnoClass::Receive_Message(radio,message,param);
        if (!unknown_530) return 10;
        if (Type->UnitRepair && ContainsLink(sender) && unknown_slot150(34,sender)==10) return 10;
        if (!Type->Hospital && !Type->Armory) {
            if (!ContainsLink(sender) && Unknown_00636ED0(sender)) {
                unknown_slot150(2,sender);
                ContainsLink(sender);
            }
            bool needs_to_move=false;
            if (ContainsLink(sender) && (Type->DockUnload || Type->Weeder)) {
                FootClass *foot=Building_Radio_Foot_004397f0(radio);
                CellClass *cell=&Map[unknown_slot41((int)foot)];
                AbstractClass *destination=foot->unknown_494;
                if (destination && reinterpret_cast<AbstractClass *>(cell)!=destination) needs_to_move=true;
            }
            if (Contact(0) && Type->UnitRepair &&
                Building_Radio_Object_Distance_004397f0(*this,*Contact(0))>128) needs_to_move=true;
            if (unknown_slot150(19,sender)==1 || needs_to_move) {
                param=this;
                if (Type->DockUnload || Type->Weeder) {
                    param=&Map[unknown_slot104()+Cell(3,1)];
                    if (unknown_slot151(18,&param,sender)==20) {
                        unknown_slot150(24,sender);
                        if (unknown_slot150(22,sender)!=1) radio->Scatter(unknown_geometry_l,true,true);
                    }
                } else if (Type->Helipad) {
                    param=this;
                    if (unknown_slot151(18,&param,sender)==20) unknown_slot149(24);
                }
            }
            return 1;
        }
        if (!Unknown_00636ED0(sender)) {
            for (int index=0;index<RadioLinks.Length();++index) {
                TechnoClass *contact=Contact(index);
                if (unknown_slot150(34,contact)==10) unknown_slot150(23,contact);
            }
            if (!Unknown_00636ED0(sender)) return 10;
        } else {
            param=&Map[Position_Coord_00450950(*this)];
            unknown_slot151(18,&param,sender);
        }
        return 1;
    }
    case 16:
        if (!Cargo.Object && Unknown_00636ED0(sender) && !unknown_76 && House==radio->Owning_House()) {
            if (Type->Refinery || Type->UnitRepair || Type->Weeder) return 1;
        }
        return 10;
    case 11:
        QueueMission(20,false);
        TechnoClass::Receive_Message(radio,message,param);
        return 1;
    case 12:
        if (Get_Mission()!=19) {
            QueueMission(5,false);
            if (Type->ConstructionYard) {
                unknown_44cfd0(7);
                unknown_44cfd0(17);
                unknown_44c920(8,Health_Ratio()<=Rules->unknown_1368,false,0);
            }
        }
        TechnoClass::Receive_Message(radio,message,param);
        return 1;
    case 3:
        unknown_443d60(1);
        TechnoClass::Receive_Message(radio,message,param);
        return 1;
    case 8:
        if (Type->UnitRepair && Building_Radio_Object_Distance_004397f0(*this,*radio)<384) return 1;
        TechnoClass::Receive_Message(radio,message,param);
        if (!Type->WeaponsFactory && !Type->UnitRepair) return 1;
        return 23;
    case 13:
        if (Type->WeaponsFactory) return 1;
        break;
    }
    return TechnoClass::Receive_Message(radio,message,param);
}


// Clip the update bounds, test the translated drawing area, then dispatch a draw.
extern bool Debug_Map;
extern HWND MainWindow;
extern Rect unknown_rect_839630;
inline bool Overlap_Draw_Bounds(Rect const &clip,Rect const &draw) {
    return clip.X<(int)((unsigned int)draw.Width+(unsigned int)draw.X)
        && clip.Y<(int)((unsigned int)draw.Height+(unsigned int)draw.Y)
        && (int)((unsigned int)clip.Width+(unsigned int)clip.X)>draw.X
        && (int)((unsigned int)clip.Height+(unsigned int)clip.Y)>draw.Y;
}
inline Rect Offset_Draw_Bounds(Rect const &bounds,Point2D const &pixel);
bool BuildingClass::Render(Rect *bounds,bool force,bool alternate)
{
    if(Debug_Map || !MainWindow || ((force || unknown_75) && unknown_74 && !unknown_76)) {
        unknown_75=false;
        *bounds=bounds->Intersect(unknown_rect_839630);
        Point2D origin=Point2D(unknown_rect_839630.X,unknown_rect_839630.Y);

        if(Overlap_Draw_Bounds(*bounds,Offset_Draw_Bounds(Get_Render_Rect(),origin))) {
            Point2D pixel;
            unknown_tactical_83996c->Coord_To_Pixel(unknown_slot42(),pixel);
            if(bounds->X>unknown_rect_839630.X) {
                pixel.X=(int)((unsigned int)pixel.X+(unsigned int)unknown_rect_839630.X-(unsigned int)bounds->X);
            }
            if(bounds->Y>unknown_rect_839630.Y) {
                pixel.Y=(int)((unsigned int)pixel.Y+(unsigned int)unknown_rect_839630.Y-(unsigned int)bounds->Y);
            }
            if(alternate) {
                if(!unknown_5b7) {
                    unknown_slot284(pixel,*bounds);
                    return true;
                }
            } else {
                // Existing int/int declaration is a32-bit ABI view; original passes two addresses.
                unknown_slot67(pixel,*bounds);
            }
            return true;
        }
    }
    return false;
}


// Slot68 keeps its established address-word ABI until the shared virtual interface is retyped.
void BuildingClass::Editor_Draw_It(Point2D const &point_ref, Rect const &bounds)
{
    Cell cell = unknown_slot104();
    ShapeFileStruct const *shape = (ShapeFileStruct const *)Get_Image_Data();
    if (shape == 0) return;
    if (Type->InvisibleInGame) return;
    if (Get_Mission() != 24 || unknown_2a8.Is_Count_Down_Complete()) {
        if (Get_Mission() == 16 && Type->unknown_11a0 != 0) {
            TechnoClass *product = ((UnknownBuildingFactory *)unknown_41c)->Get_Object();
            if (product != 0 && product->Techno_Type_Class()->JumpJet) shape = (ShapeFileStruct const *)Type->unknown_11b0;
            else shape = (ShapeFileStruct const *)Type->unknown_11a0;
        }
        Point2D point = point_ref;
        short shape_height = shape->Get_Height();
        int height = point.Y - shape_height / 2;
        Rect clip = bounds;
        if (clip.Height > height) clip.Height = height;
        if (clip.Height > 0) {
            unknown_6d1510(shape, unknown_43be20(), &point, &clip, 0, 256,
                -2 - Unknown_006A2970(Get_Height()), 2, 0, Map[cell].unknown_10a, 0, 0, Point2D(0, 0), 0);
        }
        clip = bounds;
        clip.Y += height;
        clip.Height = shape_height;
        Point2D lower_point = point;
        lower_point.Y -= height;
        if (clip.Y + clip.Height > bounds.Y + bounds.Height) clip.Height = bounds.Y + bounds.Height - clip.Y;
        if (clip.Y < bounds.Y) {
            clip.Height += clip.Y - bounds.Y;
            lower_point.Y += clip.Y - bounds.Y;
            clip.Y = bounds.Y;
        }
        if (clip.Height > 0) {
            unknown_6d1510(shape, unknown_43be20(), &lower_point, &clip, 0, 256,
                -Unknown_006A2970(Get_Height()), 0, 0, Map[cell].unknown_10a, 0, 0, Point2D(0, 0), 0);
        }
    }
}


// BuildingClass::Draw_It 0043A5E0: structure, gate and factory-door shape layers.
// The legacy virtual interface carries Point2D const& and Rect const& as x86 address words (RET8).
void BuildingClass::unknown_slot67(Point2D const &draw_point, Rect const &bounds)
{
    Cell cell=unknown_slot104();
    ShapeFileStruct const *shape=(ShapeFileStruct const *)Get_Image_Data();
    if(shape==0) return;
    if(this!=0 && Kind_Of()==6 && unknown_42c==0 && Get_Mission()==19) {
        for(int index=0;index<18;index++) {
            if(unknown_454[index]) {
                static_cast<AnimClass *>(unknown_454[index])->unknown_17d=true;
            }
        }
    }
    if(Type->InvisibleInGame) return;
    Point2D depth_point(198,446);
    int z_adjust=Type->NormalZAdjust;
    bool air_product=false;
    if(Get_Mission()==16) {
        TechnoClass *product=Contact_With_Whom(0);
        if(product && (product->Techno_Type_Class()->JumpJet || product->Techno_Type_Class()->BalloonHover)) {
            air_product=true;
        }
    }
    if(Get_Mission()==24 && (unknown_2a8.Is_Count_Up_Active() || unknown_2a8.Is_Count_Down_Active() || unknown_2a8.Is_Count_Up_Complete())) {
        int stage=(int)(unknown_2a8.Percent_Complete()*Type->GateStages);
        if(unknown_2a8.Is_Count_Down_Active()) stage=Type->GateStages-stage;
        if(unknown_2a8.Is_Count_Down_Complete()) stage=0;
        if(unknown_2a8.Is_Count_Up_Complete()) stage=Type->GateStages-1;
        if(stage>=Type->GateStages) stage=Type->GateStages-1;
        if(stage<0) stage=0;
        ShapeFileStruct const *gate_shape=(ShapeFileStruct const *)Get_Image_Data();
        stage+=(Health_Ratio()<=Rules->unknown_1368 ? Type->GateStages+1 : 0);
        unknown_6d1510(gate_shape,stage,&draw_point,&bounds,0,256,
            z_adjust-Unknown_006A2970(Get_Height()),2,1,Map[cell].unknown_10a,0,0,Point2D(0,0),0);
        return;
    }
    if(Get_Mission()==16) {
        if((air_product && Type->unknown_11b0) || (!air_product && Type->unknown_11a0)) {
            shape=(ShapeFileStruct const *)(air_product ? Type->unknown_11b0 : Type->unknown_11a0);
            z_adjust=-20;
        }
    }
    int mission=Get_Mission();
    if(mission!=18 && mission!=19) {
        depth_point+=Type->ZShapePointMove;
    }
    Point2D footprint(Type->Width()*256-256,Type->Foundation_Height(false)*256-256);
    depth_point-=unknown_tactical_83996c->Coord_To_Pixel_Absolute(footprint);
    ShapeFileStruct const *depth_shape=unknown_building_depth_shape_85029c;
    if(Type->Width()>=8) depth_shape=0;
    if(bounds.Height>0) {
        int frame=unknown_43be20()<shape->Get_Count()/2 ? unknown_43be20() : shape->Get_Count()/2;
        unknown_6d1510(shape,frame,&draw_point,&bounds,0,256,
            z_adjust-Unknown_006A2970(Get_Height()),2,1,Map[cell].unknown_10a+Type->ExtraLight,depth_shape,0,depth_point,0);
    }
    if(Type->unknown_11cc && unknown_42c!=0) {
        unknown_6d1510(Type->unknown_11cc,unknown_43be20(),&draw_point,&bounds,0,256,
            -1-Unknown_006A2970(Get_Height()),0,1,Map[cell].unknown_10a+Type->ExtraLight,0,0,Point2D(0,0),0);
    }
    if(Get_Mission()==16 && ((air_product && Type->unknown_11b8!=0) || (!air_product && Type->unknown_11a8!=0))) {
        if(air_product) {
            unknown_6d1510(Type->unknown_11b8,Health_Ratio()<=Rules->unknown_1368,&draw_point,&bounds,0,256,
                -Unknown_006A2970(Get_Height()),0,1,Map[cell].unknown_10a+Type->ExtraLight,0,0,Point2D(0,0),0);
        } else {
            unknown_6d1510(Type->unknown_11a8,Health_Ratio()<=Rules->unknown_1368,&draw_point,&bounds,0,256,
                -Unknown_006A2970(Get_Height()),0,1,Map[cell].unknown_10a+Type->ExtraLight,0,0,Point2D(0,0),0);
        }
    }
}


// BuildingClass::Draw_Extras 0043ABD0: departing products, door shapes, turret and barrel voxels.
extern bool unknown_voxel_cache_839a58;
extern int unknown_barrel_dir_adjust_7cf658;
extern bool unknown_barrel_make_visible_7cf65c;
extern Rect unknown_rect_839630;
Matrix3D __fastcall Aircraft_Isometric_View_Matrix_00717f90();
inline double Barrel_Direction_Angle_00453900(UnknownFacingDir const &direction);
inline int Draw_Dir4_0043abd0(UnknownFacingDir const &direction)
{
    return (((*(unsigned const *)&direction >> 13)+1)>>1)&3;
}
inline bool Barrel_Side_0043abd0(FacingClass &facing)
{
    return Draw_Dir4_0043abd0(facing.Current())>0 && Draw_Dir4_0043abd0(facing.Current())<3;
}
void BuildingClass::unknown_slot284(Point2D const &xy,Rect const &bounds)
{
    Cell cell=unknown_slot104();
    if(Get_Mission()==16 && (unknown_2a8.Is_Count_Up_Active() || unknown_2a8.Is_Count_Up_Complete() || unknown_2a8.Is_Count_Down_Complete() || unknown_2a8.Is_Count_Down_Active()) && unknown_370) {
        for(int index=0;index<RadioLinks.Length();index++) {
            TechnoClass *product=Contact_With_Whom(index);
            if(product && !product->unknown_76 && product->Kind_Of()!=6) {
                CoordStruct destination=product->unknown_slot19(0);
                destination.Z=product->Position_Coord_004f3f40().Z;
                if(!unknown_b263d8 || Debug_Map || !(unknown_id_source_a3d290->unknown_000&0x1000) ||
                   (!Map.Is_Fogged(product->Position_Coord_004f3f40()) && !Map.Is_Fogged(destination))) {
                    Point2D point;
                    unknown_tactical_83996c->Coord_To_Pixel(product->unknown_slot42(),point);
                    product->unknown_slot67(point,bounds);
                }
            }
        }
    }
    if(Get_Mission()==16 && Type->DoorAnimData_11c0) {
        if(unknown_2a8.Is_Count_Down_Complete() || unknown_2a8.Is_Count_Up_Active() || unknown_2a8.Is_Count_Down_Active() || unknown_2a8.Is_Count_Up_Complete()) {
            int stage=(int)(unknown_2a8.Percent_Complete()*Type->DoorStages);
            if(unknown_2a8.Is_Count_Down_Active()) stage=Type->DoorStages-stage;
            if(unknown_2a8.Is_Count_Down_Complete()) stage=0;
            if(stage>=Type->DoorStages) stage=Type->DoorStages-1;
            if(stage<0) stage=0;
            if(Health_Ratio()<=Rules->unknown_1368 && Type->DamagedDoor) stage+=Type->DoorStages;
            int z_adjust=-5-Unknown_006A2970(Get_Height());
            unknown_6d1510(Type->DoorAnimData_11c0,stage,&xy,&bounds,0,256,z_adjust,0,0,Map[cell].unknown_10a,0,0,Point2D(0,0),0);
            if(unknown_2a8.Is_Count_Down_Active() && stage==0) unknown_75=true;
        }
    }
    if(Type->TurretAnimIsVoxel) {
        bool construction=false;
        if((Mission==18 || MissionQueue==18) && Stage<Type->AnimFrames[0].unknown_04+Type->AnimFrames[0].unknown_00-1) construction=true;
        if(((Mission!=19 || Stage<=0) && !construction) || Type->Artillary) {
            int key;
            if(unknown_voxel_cache_839a58) key=Dir_To_32(unknown_2e0.Current());
            else key=-1;
            Matrix3D matrix;
            matrix.Make_Identity();
            if(Type->unknown_b8.unknown_00) {
                matrix.Rotate_Z((float)Barrel_Direction_Angle_00453900(unknown_2e0.Current()));
                matrix.Translate_X((float)(Type->TurretOffset/8));
                Matrix3D barrel_matrix;
                barrel_matrix=matrix;
                Vector3 origin=matrix.Get_Translation();
                barrel_matrix.Translate(-origin);
                if(unknown_330.unknown_18!=0) {
                    matrix.Translate_X(-unknown_330.unknown_14);
                    key=-1;
                }
                if(unknown_350.unknown_18!=0) {
                    barrel_matrix.Translate_X(-unknown_350.unknown_14);
                    key=-1;
                }
                barrel_matrix.Rotate_Y((float)-Barrel_Direction_Angle_00453900(unknown_2c8.Current()));
                barrel_matrix.Translate(origin);
                Point2D draw_point=xy+Point2D(Type->Anims[9].Offset.X,Type->Anims[9].Offset.Y);
                bool draw_barrel=Barrel_Side_0043abd0(unknown_2e0);
                if(!draw_barrel) {
                    UnknownObjectPair *voxel=&Type->unknown_c0;
                    if(voxel->unknown_00 && voxel->unknown_04) {
                        unknown_slot246(Type->unknown_c0,0,key,&Type->unknown_250,bounds,draw_point,
                            Aircraft_Isometric_View_Matrix_00717f90()*barrel_matrix,Map[cell].unknown_10a,0);
                    }
                }
                unknown_slot246(Type->unknown_b8,0,key,&Type->unknown_228,bounds,draw_point,
                    Aircraft_Isometric_View_Matrix_00717f90()*matrix,Map[cell].unknown_10a,0);
                if(draw_barrel) {
                  UnknownObjectPair *voxel=&Type->unknown_c0;
                  if(voxel->unknown_00 && voxel->unknown_04) {
                    unknown_slot246(Type->unknown_c0,0,key,&Type->unknown_250,bounds,draw_point,
                        Aircraft_Isometric_View_Matrix_00717f90()*barrel_matrix,Map[cell].unknown_10a,0);
                  }
                }
            } else if(Type->unknown_c0.unknown_00 && Type->unknown_c0.unknown_04) {
                Vector3 origin=matrix.Get_Translation();
                matrix.Translate(-origin);
                matrix.Rotate_Z((float)Barrel_Direction_Angle_00453900(unknown_2e0.Current()));
                matrix.Rotate_Y((float)-Barrel_Direction_Angle_00453900(unknown_2c8.Current()));
                matrix.Translate(origin);
                unknown_slot246(Type->unknown_c0,0,key,&Type->unknown_250,bounds,xy+Point2D(Type->Anims[9].Offset.X,Type->Anims[9].Offset.Y),
                    Aircraft_Isometric_View_Matrix_00717f90()*matrix,Map[cell].unknown_10a,0);
            }
        }
    } else if(Type->BarrelAnimIsVoxel) {
        bool construction=false;
        if((Mission==18 || MissionQueue==18) && Stage<Type->AnimFrames[0].unknown_04+Type->AnimFrames[0].unknown_00-1) construction=true;
        if((Mission!=19 || Stage<=0) && !construction) {
            Matrix3D matrix=unknown_453900();
            int direction=(unknown_barrel_dir_adjust_7cf658+Dir_To_32(unknown_2e0.Current()))%32;
            bool in_front=direction<=16;
            AnimClass *animation=static_cast<AnimClass *>(unknown_454[9]);
            if(in_front && animation) {
                if(unknown_barrel_make_visible_7cf65c) animation->unknown_17d=false;
                animation->Render(&unknown_rect_839630,true,false);
                animation->unknown_17d=true;
            }
            unknown_slot246(Type->unknown_c0,0,-1,&Type->unknown_214,bounds,xy+Point2D(Type->Anims[9].Offset.X,Type->Anims[9].Offset.Y),
                Aircraft_Isometric_View_Matrix_00717f90()*matrix,Map[cell].unknown_10a,0);
            if(!in_front && animation) {
                if(unknown_barrel_make_visible_7cf65c) animation->unknown_17d=false;
                animation->Render(&unknown_rect_839630,true,false);
                animation->unknown_17d=true;
            }
        }
    }
}


// 0043B660: selected-building text and the cameo of a spied-on factory's product.
// Declaration-only use of the canonical palette.hpp shape API.
// The caller needs pointers to these real classes, not their full file/palette internals.
struct ShapeFileStruct;
class ConvertClass;
class Surface;
void __fastcall CC_Draw_Shape(Surface *surface, ConvertClass *drawer, ShapeFileStruct const *shape, int frame,
    Point2D const &point, Rect const &window, int flags, unsigned char const *unknown8, int unknown9, int unknown10,
    int brightness, ShapeFileStruct const *unknown12, int unknown13, Point2D offset);
extern ConvertClass *unknown_drawer_8323c4;
void BuildingClass::Draw_Selection_Overlay_0043b660(Point2D const &point, Rect const &bounds)
{
    unknown_slot104();
    if (unknown_77 && (House->Is_Ally(unknown_player_a35db4) ||
        (unknown_1a8 & (1 << unknown_player_a35db4->Type->House)))) {
        Point2D text = point + Point2D(-10, 10);
        // Slot251's existing declaration carries the three physical pointer words.
        unknown_slot251(text, point, bounds);
    }
    if ((unknown_1a8 & (1 << unknown_player_a35db4->Type->House)) && unknown_77) {
        FactoryClass *factory;
        if (House->Is_Human_Player()) factory = House->unknown_func_4eb580(Type->Factory, Type->Naval, 0);
        else factory = static_cast<FactoryClass *>(unknown_41c);
        if (factory) {
            TechnoClass *product = factory->Get_Object();
            if (product) {
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4,
                    static_cast<ShapeFileStruct const *>(product->Techno_Type_Class()->unknown_slot38()),
                    0, point, bounds, 0xe00, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
    }
}


// int BuildingClass::unknown_slot176() (0x0043B7A0, slot 176)
// The negated height of the slot 110 value; a voxel turret's animation
// (Anims[9]) adds its ZAdjust.

int BuildingClass::unknown_slot176()
{
    int height = -Unknown_006A2970(Get_Height());
    if (Type->TurretAnimIsVoxel) {
        return Type->Anims[9].ZAdjust + height;
    }
    return height;
}


// UnknownFacingDir BuildingClass::unknown_slot170(AbstractClass *target) 0x0043B7E0 (slot 170; TibSun
// building.cpp Barrel_Pitch): a laser weapon elevates toward the predicted target point from its turret
// fire point, the EM pulse cannon solves the ballistic pitch, anything else uses TechnoClass's.
// The laser arm is ra2-decomp's text (single return; DirType(int) and DirType::operator- from the unit's
// RA2_FACING_LEFT90/RA2_FACING_DIFFERENCE profile, defined in the first row 0x00438500).
// From ra2-decomp 0x0043E940 (YR twin, src/building.cpp BuildingClass::Barrel_Pitch):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

// TibSun ::Direction(Coord, Coord) (the twin discards its result).
inline UnknownFacingDir Direction_0043b7e0(CoordStruct const &from, CoordStruct const &to)
{
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y) - to.Y, double(to.X) - from.X)
        - 1.5707963267948966) * -10430.060040584269));
}

// The twin's Point3D(TPoint2D, z): a coordinate's X and Y at height z.
inline CoordStruct XY_0043b7e0(CoordStruct const &from, int z)
{
    CoordStruct result;
    result.X=from.X;
    result.Y=from.Y;
    result.Z=z;
    return result;
}

UnknownFacingDir BuildingClass::unknown_slot170(AbstractClass *target)
{
    if (unknown_slot231(0)->WeaponType->unknown_121) {
        Techno_Type_Class();
        if (target != 0) {
            CoordStruct predicted = unknown_6d65b0();
            WeaponStruct *weap = unknown_slot231(0);
            int z = weap->FLH.Z;
            CoordStruct coord = unknown_slot42() + CoordStruct(0, 0, z);
            CoordStruct pt(unknown_tactical_83996c->Unknown_006A2900(Point2D(Type->Anims[9].Offset.X, Type->Anims[9].Offset.Y)), 0);
            coord += pt;
            CoordStruct fire = predicted - coord;

            if (fire.Z == 0) {
                return static_cast<Dir256>(64);
            }

            double dr;
            int d;

            d = fire.X * fire.X + fire.Y * fire.Y;
            dr = Fast_Sqrt(d);
            d = fire.Length();
            dr = dr / d;
            if (dr > 1) {
                dr = 1;
            }

            UnknownFacingDir dir = (int)((Fast_Asin_004ba550(dr) * 16384.0) * 0.6366197723675814);
            if (fire.Z < 0) {
                dir = UnknownFacingDir(static_cast<Dir256>(128)) - dir;
            }

            return dir;

        } else {
            return unknown_2c8.Current();
        }

    } else if (Type->EMPulseCannon) {
        CoordStruct tcoord = XY_0043b7e0(target->Center_Coord(), Map.Get_Height_GL(target->Center_Coord()));
        CoordStruct fcoord = unknown_slot43(0);
        Direction_0043b7e0(fcoord, tcoord);
        int max_speed = Unknown_0047FB70(unknown_5d4c00(target), Rules->unknown_1320);

        CoordStruct ucoord = unknown_slot181(0);
        CoordStruct disp = target->Center_Coord() - ucoord;
        int distance2 = disp.X * disp.X + disp.Y * disp.Y;

        UnknownFacingDir pitch;
        bool valid = Unknown_0047F8B0(unknown_6d7d20(0), max_speed, (int)Fast_Sqrt(distance2), disp.Z, Rules->unknown_1320, &pitch);
        if (!valid) {
            valid = Unknown_0047F8B0(unknown_6d7d20(0), max_speed * 10 / 8, (int)Fast_Sqrt(distance2), disp.Z, Rules->unknown_1320, &pitch);
        }
        return pitch;
    } else {
        return TechnoClass::unknown_slot170(target);
    }
}


// Direction to a target after the building's fire-pixel or turret-animation offset.
inline CoordStruct Projected_Coord_0043bbd0(Point2D const &point) { return CoordStruct(point.X,point.Y,0); }
inline UnknownFacingDir Direction_Between_0043bbd0(CoordStruct const &from,CoordStruct const &to)
{
    int target_x=to.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)from.Y-(double)to.Y,(double)target_x-(double)from.X)-1.5707963267948966)*-10430.060040584269));
}
static inline bool Fire_Pixels_Equal_0043bbd0(Point2D const &a,Point2D const &b)
{ return a.X==b.X && a.Y==b.Y; }
static inline bool Fire_Pixels_Differ_0043bbd0(Point2D const &a,Point2D const &b)
{ return !Fire_Pixels_Equal_0043bbd0(a,b); }
UnknownFacingDir BuildingClass::unknown_slot285(AbstractClass *target)
{
    CoordStruct offset=unknown_geometry_l;
    Point2D const &pixel=Type->PrimaryFirePixelOffset;
    if(Fire_Pixels_Differ_0043bbd0(pixel,Point2D(65535,65535))) {
        offset=Projected_Coord_0043bbd0(unknown_tactical_83996c->Unknown_006A2900(pixel));
    } else {
        Point2D turret(Type->Anims[9].Offset.X,Type->Anims[9].Offset.Y);
        if(turret.X!=0 || turret.Y!=0) {
            offset=Projected_Coord_0043bbd0(unknown_tactical_83996c->Unknown_006A2900(turret));
        }
    }
    CoordStruct from=Center_Coord()+offset;
    return Direction_Between_0043bbd0(from,target->Center_Coord());
}


// BuildingClass Set_Turret_Frame 0x0043BCE0 (TibSun building.cpp Set_Turret_Frame without the charge animation).
// Free fastcall versus nonvirtual-member spelling remains a source hypothesis.
extern "C" int const unknown_turret_frame_table_007acab0[32];
int __fastcall Building_Update_Turret_Animation_Frame_0043bce0(BuildingClass *building)
{
    int stage = 0;
    int reset_step = false;

    if (building->unknown_slot232()) {
        if (building->unknown_454[9] != 0) {
            stage = unknown_turret_frame_table_007acab0[Dir_To_32(building->unknown_2e0.Current())];
            reset_step = true;

            AnimClass *anim = static_cast<AnimClass *>(building->unknown_454[9]);
            if (anim != 0) {
                anim->Set_Stage(stage);
                if (reset_step) {
                    anim->Set_Step(0);
                }
            }
        }
    }
    return(stage);
}


// Name the SHP turret animation from its image, separator and frame letter.
extern "C" char const unknown_turret_separator_007cf730[];
void BuildingClass::Set_Turret_Animation_Frame_0043bd40(int frame)
{
    if(frame!=unknown_4d0 && !Type->TurretAnimIsVoxel) {
        unknown_44cfd0(9);
        if(frame!=-1) {
            char name[64];
            strcpy(name,(char const *)Type->unknown_1c8);
            strcat(name,unknown_turret_separator_007cf730);
            unsigned int length=strlen(name);
            name[length]=(char)((unsigned int)frame+(unsigned int)'B');
            name[length+1]=0;
            unknown_44ca60(name,9,false,false,0);
            unknown_4d0=frame;
        }
    }
}


// Select the building's SHP frame from its stage, state, health and occupants.
// This original body establishes that the old palette/converter label was provisional.
int BuildingClass::unknown_43be20()
{
    BuildingTypeClass *type=Type;
    unsigned int frame=(unsigned int)StageClass::Stage;
    if(type->LaserFence) return unknown_4ec;
    if(type->FirestormWall) return unknown_4f0;
    if(unknown_42c==0) {
        if(type->Gate) frame=(unsigned int)type->AnimFrames[0].unknown_04+(unsigned int)type->AnimFrames[0].unknown_00-frame-1;
        if(Get_Mission()==19) {
            frame=(unsigned int)Type->AnimFrames[unknown_42c].unknown_04+(unsigned int)Type->AnimFrames[unknown_42c].unknown_00-frame-1;
        }
    } else {
        if(type->CanBeOccupied) {
            if(Health_Ratio()<=Rules->unknown_1370) return 1;
            return unknown_554.Count()>0?2:0;
        }
        if(type->Gate) {
            if(Health_Ratio()<=Rules->unknown_1368) return (unsigned int)Type->GateStages+1;
            return 0;
        }
        if(Health_Ratio()<=Rules->unknown_1368) {
            if(unknown_42c==1) ++frame;
            else {
            type=Type;
            int end=(int)((unsigned int)type->AnimFrames[1].unknown_04+(unsigned int)type->AnimFrames[1].unknown_00);
            int other=(int)((unsigned int)type->AnimFrames[2].unknown_04+(unsigned int)type->AnimFrames[2].unknown_00);
            if(end<=other) end=other;
            other=(int)((unsigned int)type->AnimFrames[4].unknown_04+(unsigned int)type->AnimFrames[4].unknown_00);
            if(end<=other) end=other;
            other=(int)((unsigned int)type->AnimFrames[5].unknown_04+(unsigned int)type->AnimFrames[5].unknown_00);
            if(end<=other) end=other;
            frame+=(unsigned int)end;
            }
        }
    }
    return frame;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool BuildingClass::Mark(int mark) 0x0043BFC0 (slot 71, Mark), from ra2-decomp BuildingClass::Mark
// (YR 0x0043F180, src/building.cpp): lifts the building's cell content, or puts it down: a building that turns
// into isometric tiles lays them over its clear foundation cells (clearing smudges, revealing for the player)
// and deletes itself; a wall becomes its overlay; anything else occupies its cell, snaps its coordinate,
// places its animations and clears the smudges under its foundation. MARK_CHANGE refreshes the animations.
bool BuildingClass::Mark(int mark)
{
    if (TechnoClass::Mark(mark)) {
        Cell cell = unknown_slot104();

        switch (mark) {
            case 0:
                Map.Pick_Up(cell, this);
                break;

            case 1:
            case 3:

                if (Type->unknown_bf0 != 0) {
                    if (Can_Enter_Cell(&Map[cell], -1, -1, 0, true) == 0) {
                        int w = Type->Width();
                        int h = Type->Foundation_Height(false);
                        bool any_created = false;
                        int x;
                        int y;

                        for (y = 0; y < h; y++) {
                            for (x = 0; x < w; x++) {
                                Cell newcell = cell + Cell(x, y);
                                if (Map[newcell].Is_Clear_To_Build(Type->SpeedType, Type, House)) {
                                    if (Map[newcell].IsoTileIndex != Type->unknown_bf0->ArrayIndex && Map[newcell].unknown_ec != 1) {
                                        CellClass *cptr = &Map[newcell];
                                        if (cptr->SmudgeIndex != -1) {
                                            SmudgeTypeClass *sptr = SmudgeTypes[cptr->SmudgeIndex];
                                            Cell position = cptr->MapCoords;
                                            int width = sptr->Width;
                                            int xx = cptr->SmudgeData % width;
                                            int yy = cptr->SmudgeData / width;
                                            position -= Cell(xx, yy);
                                            for (int smy = 0; smy < sptr->Height; smy++) {
                                                for (int smx = 0; smx < sptr->Width; smx++) {
                                                    Map[position + Cell(smx, smy)].SmudgeIndex = -1;
                                                    Map[position + Cell(smx, smy)].Register_As_Dirty();
                                                }
                                            }
                                        }
                                        ObjectClass *iptr = Type->unknown_bf0->Create_One_Of(House);
                                        iptr->Unlimbo(&CoordStruct(newcell), 0);
                                        if (House == unknown_player_a35db4) {
                                            Map.Sight_From(CoordStruct(newcell), 1, unknown_player_a35db4, false, false, false, true, false);
                                            Map.Sight_From(CoordStruct(newcell), 1, unknown_player_a35db4, false, false, false, true, true);
                                        }
                                        any_created = true;
                                    }
                                }
                            }
                        }

                        for (y = 0; y < h + 2; y++) {
                            for (x = 0; x < w + 2; x++) {
                                Cell snewcell = cell + Cell(x - 1, y - 1);
                                Map[snewcell].Register_For_Redraw();
                            }
                        }

                        if (any_created) {
                            SendToEachLink(3);
                            Delete_Me();
                            return true;
                        } else {
                            TechnoClass::Mark(0);
                            return false;
                        }
                    } else {
                        TechnoClass::Mark(0);
                        return false;
                    }
                } else if (Type->Wall) {
                    new OverlayClass(Type->ToOverlay, cell, House->Type->House);
                    SendToEachLink(3);
                    Delete_Me();

                } else {
                    if (mark != 3) {
                        if (Can_Enter_Cell(&Map[cell], -1, -1, 0, true) != 0) {
                            return false;
                        }
                    }
                    Map.Place_Down(cell, this);
                    Set_Coord(Type->Coord_Fixup(CoordStruct(cell)));
                    unknown_44c9a0();

                    int w = Type->Width();
                    int h = Type->Foundation_Height(false);
                    int x;
                    int y;
                    for (y = 0; y < h; y++) {
                        for (x = 0; x < w; x++) {
                            Cell newcell = cell + Cell(x, y);
                            CellClass *cptr = &Map[newcell];
                            if (cptr->SmudgeIndex != -1) {
                                SmudgeTypeClass *sptr = SmudgeTypes[cptr->SmudgeIndex];
                                Cell position = cptr->MapCoords;
                                int width = sptr->Width;
                                int xx = cptr->SmudgeData % width;
                                int yy = cptr->SmudgeData / width;
                                position -= Cell(xx, yy);
                                for (int smy = 0; smy < sptr->Height; smy++) {
                                    for (int smx = 0; smx < sptr->Width; smx++) {
                                        Map[position + Cell(smx, smy)].SmudgeIndex = -1;
                                        Map[position + Cell(smx, smy)].Register_As_Dirty();
                                    }
                                }
                            }
                        }
                    }

                    for (y = 0; y < h + 2; y++) {
                        for (x = 0; x < w + 2; x++) {
                            Cell snewcell = cell + Cell(x - 1, y - 1);
                            Map[snewcell].Register_For_Redraw();
                        }
                    }
                }
                break;

            case 2:
                Update_Anim_Appearance_0044d190();
                break;

            default:
                break;
        }
        return true;
    }
    return false;
}


// BuildingClass::AI 0043C930: production state, damage effects and mission processing.
inline CoordStruct Projected_Coord_0043bbd0(Point2D const &point);
inline void Accumulate_Coord_004395f0(CoordStruct &destination, CoordStruct const &offset);
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house);
inline CoordStruct::CoordStruct(Point2D const &point,int z)
{
    int point_y=point.Y; int point_x=point.X;
    X=point_x; Y=point_y; Z=z;
}

void BuildingClass::AI()
{
    bool working = unknown_slot195() && Get_Mission() != 18 && Get_Mission() != 19;
    if (working != unknown_598) {
        if (Type->WorkingSound != -1 || Type->NotWorkingSound != -1) {
            unknown_584.Unknown_00405FE0();
            if (working) {
                Voc_Play_At(Type->WorkingSound, Position_Coord_004f3f40(), &unknown_584);
            } else {
                Voc_Play_At(Type->NotWorkingSound, Position_Coord_004f3f40(), &unknown_584);
            }
        }
        unknown_44f8e0();
        unknown_598 = working;
    } else {
        if (Type->WorkingSound != -1 || Type->NotWorkingSound != -1) {
            Voc_Update_At(Position_Coord_004f3f40(), &unknown_584);
        }
    }
    bool damaged;
    if (Type->CanBeOccupied) {
        damaged = Health_Ratio() <= Rules->unknown_1370;
    } else {
        damaged = Health_Ratio() <= Rules->unknown_1368;
    }
    if (damaged != unknown_4bc) {
        if (damaged) {
            Create_Damage_Fires_004395f0();
        } else {
            unknown_4397c0();
        }
        unknown_4bc = damaged;
    }
    if (Type->Turret) {
        unknown_3f0 = unknown_2e0.Is_Rotating();
    } else {
        unknown_3f0 = 0;
    }
    if (unknown_20c) {
        unknown_20c->AI();
    }
    if (unknown_slot111() || unknown_slot112()) {
        if (Type->MaxNumberOccupants == 0 || Rules->unknown_2a8 == 0) {
            if (Frame % 24 == 0 && Rules->unknown_2a8) {
                new AnimClass(Rules->unknown_2a8, Position_Coord_004f3f40(), 0, 1, 0x600, 0);
            }
        } else {
            for (int index = 0; index < Type->MaxNumberOccupants; ++index) {
                if ((Frame + index) % 24 == 0 && Rules->unknown_2a8) {
                    CoordStruct location(unknown_tactical_83996c->Unknown_006A2900(Type->MuzzleFlash[index]), 0);
                    location = unknown_slot42() + location;
                    AnimClass *effect = new AnimClass(Rules->unknown_2a8, location, 0, 1, 0x600, 0);
                    effect->ZAdjust = -200;
                }
            }
        }
        if (unknown_228) {
            unknown_slot220(0);
        }
        return;
    }
    if (unknown_5a0 == 1) {
        unknown_5a0 = 0;
        unknown_5a0 = Type->ProduceCashDelay;
        if (!House->Type->MultiplayPassive && unknown_slot195()) {
            int amount = Type->ProduceCashAmount;
            if (amount > 0) {
                House->Refund_Money(amount);
            } else {
                House->Spend_Money_004e5280(-amount);
            }
        }
    }
    if (Type->SAM && unknown_228 && !unknown_228->In_Air()) {
        unknown_slot220(0);
    }
    Update_Animations_0044BD40();
    if (unknown_slot122() && unknown_42c != 0 && Commence()) {
        unknown_5ad = false;
    }
    TechnoClass::AI();
    if (!unknown_7d) return;
    if (unknown_254 == 0 && !Type->Hospital && !Type->Armory) {
        unknown_254 = Type->Ammo;
    }
    if (unknown_slot122() && Commence()) {
        unknown_5ad = false;
    }
    if (unknown_430 != -1) {
        if (unknown_42c != unknown_430) {
            unknown_42c = unknown_430;
            BuildingTriple const *control = &Type->AnimFrames[unknown_430];
            if (unknown_42c == 0 || unknown_42c == 1) {
                Set_Rate(Options.Normalize_Delay(control->unknown_08));
            } else {
                Set_Rate(control->unknown_08);
            }
            Set_Stage(control->unknown_00);
        }
        unknown_430 = -1;
    }
    if (Strength != unknown_43c) {
        House->unknown_5668 = true;
        House->unknown_5669 = true;
        unknown_43c = Strength;
        Building_Refresh_House_Buildings_0044fa40(House);
    }
    if (Strength == 0) {
        unknown_4397c0();
        if (unknown_420 == 0) {
            Limbo();
            Spawn_Survivors_0043f8e0((ObjectClass *)unknown_438);
            Delete_Me();
            Leave_Rubble_0043eba0();
        }
        return;
    }
    unknown_44b770();
    if (Type->Overpowerable) {
        for (int index = unknown_53c.Count() - 1; index >= 0; --index) {
            if (unknown_53c[index]->unknown_228 != this) {
                unknown_53c.Delete(index);
            }
        }
        int count = unknown_53c.Count();
        if (count >= 3 || (House->Power_Fraction() == 1.0 && unknown_530 && count >= 1)) {
            unknown_531 = true;
        } else {
            unknown_531 = false;
        }
    }
    if (Type->CanBeOccupied) {
        unknown_453300();
    }
    Repair_AI_0044b9b0();
    if (Type->Factory != 0) {
        Factory_AI_0044b480();
    }
    if (unknown_5af && unknown_420 == 0) {
        int damage = Strength;
        if (Type->BridgeRepairHut) {
            bool bridge_overlay = false;
            for (int y = -2; y < 3; ++y) {
                for (int x = -2; x < 3; ++x) {
                    int tile = Map[Cell(unknown_slot104() + Cell(x,y))].IsoTileIndex;
                    int overlay = Map[Cell(unknown_slot104() + Cell(x,y))].OverlayIndex;
                    if ((tile >= unknown_tileset_a6cb68 && tile < unknown_tileset_a6cb68 + 16) ||
                        (overlay >= 0x4a && overlay <= 0x65)) {
                        bridge_overlay = true;
                    }
                }
            }
            if (bridge_overlay) {
                Map.Unknown_00557090(unknown_slot104());
            } else {
                Map.Unknown_00556520(unknown_slot104());
            }
            unknown_5af = false;
            unknown_438 = 0;
        } else {
            Take_Damage(&damage, 0, Rules->unknown_c74, (TechnoClass *)unknown_438, true, 0);
        }
        if (!unknown_7d) return;
        Mark(2);
    }
    if (Type->FirestormWall && (Frame & 7) == 0) {
        int frame = unknown_4f0 & 15;
        if (House->unknown_142) {
            if (frame != 10 && frame != 5 && unknown_454[11] == 0 && (unknown_id_source_a3d290->Random()() & 15) == 0) {
                unknown_454[11] = new AnimClass(Rules->FirestormIdleAnim, Position_Coord_004f3f40() + CoordStruct(-740,-740,0), 0, 1, 0x604, -10);
            }
        }
    }
    if (unknown_228 && !unknown_slot213(unknown_228)) {
        if (unknown_228->Kind_Of() != 2) {
            unknown_slot220(0);
        } else if (unknown_228->On_Ground()) {
            unknown_slot220(0);
        }
    }
}


// BuildingClass slot 53 Unlimbo 0x0043D3A0 (TibSun building.cpp Unlimbo with the RA2 additions).
// Shape from the TibSun reconstruction (OpenTS building.cpp BuildingClass::Unlimbo; SPDX-License-Identifier:
// GPL-3.0-or-later; Copyright 2025 Electronic Arts Inc.; Copyright 2026 OpenTS contributors; contains material
// derived from Electronic Arts source code, EA's GPLv3 Section 7 additional terms apply) and the YR twin in
// gibbo101/ra2-decomp (src/building.cpp, GAMEMD 0x00440580; REFRESH_EOL is Cell(32767, 32767) in its sun.h):
// coord.As_Cell(), the REFRESH_EOL loop test and `sysloc + PositionCoord, &Map[PositionCoord]`.
// Draft by Scout753 (ul0), finished by Scout880.
extern "C" int __cdecl stricmp(char const *, char const *);
extern "C" char *__cdecl strncpy(char *, char const *, unsigned);
extern bool unknown_flag_8339e8;
void __fastcall Building_Refresh_Fence_Cell_0044e440(Cell const &origin);
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house);
// The map's object lookup at a cell (0x0049A5B0; TibSun MapClass::Cell_Object).
class UnknownCellObjectMap {
public:
    ObjectClass *Cell_Object(Cell const &cell, Point2D const &offset);
};
bool BuildingClass::Unlimbo(CoordStruct const *coord, int dir)
{
    bool ok = Can_Enter_Cell(&Map[*coord], -1, -1, 0, true) == 0;

    if (ok) {
        if (Type == Rules->unknown_78c) {
            CellClass *cellptr = &Map[*coord];
            HouseClass *owner = 0;
            if (cellptr->unknown_50 >= 0) {
                owner = Houses[cellptr->unknown_50];
            }
            if (owner == House) {
                if (cellptr->OverlayIndex == 2 || cellptr->OverlayIndex == 0) {
                    House->Sell_Wall(cellptr->MapCoords, true);
                }
            }
        } else if (Type->ToOverlay != 0) {
            CellClass *cellptr = &Map[*coord];
            HouseClass *owner = 0;
            if (cellptr->unknown_50 >= 0) {
                owner = Houses[cellptr->unknown_50];
            }
            if (owner == House) {
                if (cellptr->OverlayIndex == Type->ToOverlay->ArrayIndex) {
                    House->Sell_Wall(cellptr->MapCoords, true);
                }
            }
        } else if (Type->Gate) {
            Cell cell = coord->As_Cell();
            Cell const *occupy = Type->Occupy_List(true);
            while (*occupy != Cell(32767, 32767)) {
                CellClass *cellptr = &Map[cell + *occupy];
                HouseClass *owner = 0;
                if (cellptr->unknown_50 >= 0) {
                    owner = Houses[cellptr->unknown_50];
                }
                if (owner == House) {
                    if (cellptr->OverlayIndex == 2 || cellptr->OverlayIndex == 0 || cellptr->OverlayIndex == 0x1A) {
                        House->Sell_Wall(cellptr->MapCoords, true);
                    }
                }
                BuildingClass *building = static_cast<BuildingClass *>(cellptr->Cell_Building());
                if (building != 0 && building->Type->LaserFence && building->Owner() == Owner()) {
                    Building_Refresh_Fence_Cell_0044e440(cellptr->Fetch_CellID());
                }
                occupy++;
            }
        }
    }

    if (Type->Wall) {
        if (Can_Enter_Cell(&Map[*coord], -1, -1, 0, true) == 0) {
            ObjectClass *o = Type->ToOverlay->Create_One_Of(House);
            if (o && o->Unlimbo(coord, 0)) {
                Map[*coord].unknown_50 = House->ArrayIndex;
                unknown_slot149(3);
                Map.Sight_From(*coord, Type->Sight, House, false, false, false, true, false);
                Map.Sight_From(*coord, Type->Sight, House, false, false, false, true, true);
                Delete_Me();
                return true;
            }
        }
        return false;
    }

    if (Type->PowersUpBuilding[0] != '\0') {
        ObjectClass *object = ((UnknownCellObjectMap &)Map).Cell_Object(coord->As_Cell(), Point2D(0, 0));
        if (object != 0 && object->Kind_Of() == 6) {
            BuildingClass *building = (BuildingClass *)object;
            BuildingTypeClass *btype = Type;
            if (House == building->House && stricmp(btype->PowersUpBuilding, building->Type->ID) == 0) {
                bool ok = false;
                if (btype->PowersUpToLevel != -1) {
                    if (btype->PowersUpToLevel <= 0 || btype->PowersUpToLevel > 3) {
                        return false;
                    }
                } else {
                    if (building->unknown_5c6 < building->Type->Upgrades) {
                        ok = true;
                    }
                }
                if (!ok && building->unknown_5c6) {
                    return false;
                }

                House->unknown_5669 = true;
                House->unknown_5668 = true;
                int levels = Type->PowersUpToLevel;
                if (levels == -1) {
                    levels = 1;
                }
                char *anim = building->Type->Anims[building->unknown_5c6].Anim.Buffer;
                if (stricmp(anim, Type->unknown_1c8) != 0) {
                    strncpy(anim, Type->unknown_1c8, sizeof(building->Type->Anims[0].Anim.Buffer));
                }
                while (levels) {
                    building->Advance_Upgrade_Animation_0044c5d0();
                    levels--;
                }
                building->unknown_4c0[building->unknown_5c6 - 1] = Type;
                Building_Refresh_House_Buildings_0044fa40(House);
                House->Enable_Available_Super_Weapons_004f4db0();
                House->unknown_144 = true;
                if (Type->IsThreatRatingNode) {
                    House->unknown_func_4f3160();
                }
                Delete_Me();
                return true;
            }
        }

        return false;
    }

    if (Type->LaserFence) {
        if ((Dir_To_8(UnknownFacingDir((unsigned short)(dir << 8))) & 3) == 2) {
            unknown_4ec = 8;
        } else {
            unknown_4ec = 12;
        }
    }

    if (ok && Type->LaserFencePost) {
        unknown_4ec = 0;
        BuildingClass *building = static_cast<BuildingClass *>(Map[coord->As_Cell()].Cell_Building());
        if (building != 0 && building->Type->LaserFence && Owner() == building->Owner()) {
            building->Delete_Me();
        }
    }

    if (TechnoClass::Unlimbo(coord, dir)) {

        if (Type->CloakGenerator) {
            House->unknown_55e8 = true;
        }

        if (!unknown_7d) {
            return true;
        }

        if (unknown_game_mode_a3d298 != 0 && !House->Unknown_004F52F0() && !House->Type->MultiplayPassive) {
            unknown_59b = true;
        }

        if (unknown_id_source_a3d290->unknown_000 & 0x1000) {
            if (unknown_452be0()) {
                Create_Fogged_Copy_00452c70(0, 0, false);
            }
        }

        if (!unknown_slot31()) {
            Reserve_Base_Area_004510d0(false);
        }

        int width = Type->Width();
        int height = Type->Foundation_Height(false);
        Cell cell = coord->As_Cell();

        for (int y = 0; y < height + 2; y++) {
            for (int x = 0; x < width + 2; x++) {
                CellClass *cptr = &Map[Cell(x, y) + (cell - Cell(1, 1))];
                cptr->unknown_122++;
            }
        }

        if (Type->FirestormWall) {
            Update_Firestorm_Wall_00450950();
            CellClass *cellptr = &Map[*coord];
            for (int facing = 0; facing < 8; facing += 2) {
                BuildingClass *building = static_cast<BuildingClass *>(cellptr->Adjacent_Cell(facing).Cell_Building());
                if (building != 0 && building->Type->FirestormWall) {
                    building->Update_Firestorm_Wall_00450950();
                }
            }
        }

        House->unknown_func_4e8640();
        House->unknown_func_4eac70(this);
        House->unknown_144 = true;
        unknown_43c = 0;

        if ((!unknown_373 && (Map[*coord].unknown_12c & 0x10)) || unknown_game_mode_a3d298 != 0) {
            Revealed(unknown_player_a35db4);
        } else if (Type->LightIntensity != 0) {
            if (unknown_4e8 == 0) {
                unknown_4e8 = new LightSourceClass(Center_Coord(), Type->LightVisibility, Type->LightIntensity, Type->LightRedTint, Type->LightGreenTint, Type->LightBlueTint);
            }
            unknown_4e8->Activate(false);
        }

        if (Type->LaserFencePost && !unknown_a40774) {
            if (unknown_7d && !unknown_76) {
                unknown_44dfc0(0);
                unknown_44dfc0(2);
                unknown_44dfc0(4);
                unknown_44dfc0(6);
                unknown_44e5a0(1);
            }
        }

        if (!House->Unknown_004F52F0()) {
            Revealed(House);
        }

        if (unknown_372) {
            unknown_flag_8339e8 = true;
            Map.Flag_To_Redraw(0);
        }

        if (Type->NaturalParticleSystem != 0 && unknown_25c[2] == 0) {
            CoordStruct sysloc = Type->NaturalParticleLocation;
            unknown_25c[2] = new ParticleSystemClass(Type->NaturalParticleSystem, sysloc + Position_Coord_004f3f40(), &Map[Position_Coord_004f3f40()], 0, unknown_geometry_l);
        }

        if (Type == Rules->unknown_78c) {
            Cell cell = Map[*coord].Fetch_CellID();
            Map[cell + Cell(1, 0)].Wall_Update(false);
            Map[cell + Cell(-1, 0)].Wall_Update(false);
            Map[cell + Cell(0, 1)].Wall_Update(false);
            Map[cell + Cell(0, -1)].Wall_Update(false);
        }

        if (Type == Rules->unknown_77c || Type == Rules->unknown_784) {
            Cell cell = Map[*coord].Fetch_CellID();
            Map[cell + Cell(-1, 0)].Wall_Update(false);
            Map[cell + Cell(3, 0)].Wall_Update(false);
        }

        if (Type == Rules->unknown_780 || Type == Rules->unknown_788) {
            Cell cell = Map[*coord].Fetch_CellID();
            Map[cell + Cell(0, -1)].Wall_Update(false);
            Map[cell + Cell(0, 3)].Wall_Update(false);
        }

        if (Type->HasSpotlight) {
            unknown_4d4 = new BuildingLightClass(this);
        }

        unknown_32b = false;
        if (Type->Factory != 0) {
            unknown_4444b0(false);
        }

        for (int i = 0; i < Rules->unknown_7c8.Count(); i++) {
            if (Rules->unknown_7c8[i] == Type) {
                House->unknown_50.Add(this);
                break;
            }
        }
        if (Type->UnitRepair) {
            House->unknown_80.Add(this);
        }
        if (Type->Cloning) {
            House->unknown_98.Add(this);
        }
        if (Type->PsychicDetectionRadius > 0) {
            House->unknown_b0.Add(this);
        }
        House->Buildings.Add(this);

        if (!House->Unknown_004F52F0()) {
            House->unknown_55f0.unknown_42cad0(this);
        }

        if (Type->Factory != 0) {
            House->unknown_func_4f3170(Type->Factory, Type->Naval, 0);
        }
        return true;
    }
    return false;
}


// Complete Building destruction 0043E270; private evidenced slot286 signature hypothesis.
inline CoordStruct Position_Coord_00450950(ObjectClass const &object);
// Existing coordinate-scatter ABI, declared here without the unrelated full Bullet class.
CoordStruct __fastcall Coord_Scatter(CoordStruct const &coord,int distance,bool lock);
// Original491880: ECX hidden result, EDX input coordinate, one stack-facing argument, RET4.
CoordStruct __fastcall Destruction_Adjacent_Coord_00491880(CoordStruct const &coord,int facing);
extern "C" char *__cdecl strncpy(char *,char const *,unsigned);
class Leaf_482e00 { public: void get(); }; // Existing accepted empty original member; receiver unused.
extern int const Destruction_Facings_007cf660[4];
extern char const Destruction_Fire_Name_007cf7b8[];
// Private source hypothesis: a world-coordinate constructor over the shared three-int layout.
// The original unit has one const-particle vector construction, calling454AD0.
// This declaration is paired with the complete original-bounded specialization later in the plan.
// VectorClass<const ParticleSystemTypeClass*> constructor00454AD0: real complete body.
// Original Building interval438500..454CF0 has one dynamic-vector installation79D0CC,
// immediately after its only base-ctor call43E9BA->454AD0. Other79D0AC uses are destructors.
// WORKER original-never-inline exception, restricted to this actual constructor specialization.
#pragma auto_inline(off)
template <> VectorClass<ParticleSystemTypeClass const *>::VectorClass(unsigned size,ParticleSystemTypeClass const * const *array)
    : Vector(0),VectorMax(size),IsValid(true),IsAllocated(false)
{
    typedef ParticleSystemTypeClass const *Element;
    if (size) {
        if (array) Vector=new ((void *)array) Element[size];
        else { Vector=new Element[size]; IsAllocated=true; }
    }
}
#pragma auto_inline(on)

// Above is a complete auxiliary duplicate of canonical454AD0, zero new-byte credit.
void BuildingClass::unknown_slot286(TechnoClass *last_contact,TechnoClass *source,bool forced,Cell const *offset)
{
    unknown_6d3040(source);
    for (int index=0;index<8;++index) {
        if (unknown_49c[index]) {
            unknown_49c[index]->Delete_Me();
            unknown_49c[index]=0;
        }
    }
    if (unknown_1a8) {
        unknown_1a8=0;
        if (Type->Radar) unknown_44ab00();
    }
    if (Type->CloakGenerator) {
        unknown_5bb=-1;
        if (unknown_5bc==0) unknown_5bc=Type->CloakRadiusInCells;
        unknown_75=true;
        unknown_5bc=1;
        Cloaking_AI(1);
    }
    if (Type->LaserFencePost) unknown_44dc40(true);
    if (Techno_Type_Class()->DieSound.Count()==0) Voc_Static_Add(Rules->BuildingDieSound,Position_Coord_00450950(*this),0);
    if (Type->Width()>=2 && Type->Foundation_Height(false)>=2) {
        if (Type->Width()>2) Random_Pick(0,Type->Width()-2);
        if (Type->Foundation_Height(false)>2) Random_Pick(0,Type->Foundation_Height(false)-2);
        if (Random_Pick(0,99)<50) SmudgeTypeClass::Create_Scorch(CoordStruct(Cell(Position_Coord_00450950(*this))),100,100,true);
        else SmudgeTypeClass::Create_Crater(CoordStruct(Cell(Position_Coord_00450950(*this))),100,100,true);
    }
    while (*offset!=Cell(32767,32767)) {
        CoordStruct position;
        CoordStruct height_offset(0,0,0);
        Cell const *current=offset++;
        Cell cell=Cell(unknown_slot42())+*current;
        if (Type->Explosion.Count()>0) {
            height_offset.Z=Position_Coord_00450950(*this).Z;
            position=height_offset+Coord_Scatter(CoordStruct(cell),64,false);
            new AnimClass(Type->Explosion.Pick(unknown_id_source_a3d290->Random()()),position,Random_Pick(0,3),1,0x600,0);
        }
    }
    if (Type->Explodes) {
        for (int index=0;index<4;++index) {
            CoordStruct coord=Destruction_Adjacent_Coord_00491880(Position_Coord_00450950(*this),Destruction_Facings_007cf660[index]);
            if (Map[coord].OverlayIndex!=-1 && OverlayTypes[Map[coord].OverlayIndex]->Explodes) {
                new AnimClass(AnimTypes[AnimTypeClass::From_Name(Destruction_Fire_Name_007cf7b8)],coord,Random_Pick(1,3)+3,1,0x600,0);
            }
        }
    }
    while (unknown_294.Get_Total_Amount()>=1.0f) {
        int type=unknown_294.First_Used_Slot();
        unknown_294.Remove_Amount(1.0f,type);
        House->Tiberium.Remove_Amount(1.0f,type);
        CoordStruct coord=Coord_Scatter(Position_Coord_00450950(*this),Random_Pick(256,768),true);
        Map[coord].Place_Tiberium(type, 1);
    }
    int shakes=Type->Cost_Of(House)/Rules->unknown_4e8;
    if (shakes>0) ((Leaf_482e00 *)shakes)->get();
    if (Get_Mission()==19 || Type->Explodes) {
        unknown_420=CDTimerClass<FrameTimerClass>(0);
        StageClass::Set_Rate(0);
    } else unknown_420=CDTimerClass<FrameTimerClass>(8);
    if (Type->DestroyAnim.Count()>0) {
        AnimTypeClass *type=Type->DestroyAnim.Pick(unknown_id_source_a3d290->Random()());
        if (type) {
            AnimClass *animation=new AnimClass(type,unknown_slot42(),0,1,0x600,0);
            if (Techno_Type_Class()->Palette) {
                animation->unknown_bc=unknown_slot115();
                if (Type->PaletteFile.Buffer[0]) strncpy(animation->Palette.Buffer,Type->PaletteFile.Buffer,32);
            }
        }
    }
    // Original RTTI79D0CC: DynamicVectorClass<const ParticleSystemTypeClass *>.
    DynamicVectorClass<ParticleSystemTypeClass const *> systems;
    for (int system_index=Techno_Type_Class()->DestroyParticleSystems.Count()-1;system_index>=0;--system_index) {
        if (Techno_Type_Class()->DestroyParticleSystems[system_index]->BehavesLike==0) {
            systems.Add(Techno_Type_Class()->DestroyParticleSystems[system_index]);
        }
    }
    // Existing eight-pointer view: the last slot is the destruction particle system at+278.
    if (!unknown_25c[7] && systems.Count()>0 && Get_Height_AGL()>-10) {
        TechnoTypeClass *type=Techno_Type_Class();
        UnknownCoord434820 smoke_offset(type->DestroySmokeOffset);
        unknown_25c[7]=new ParticleSystemClass(const_cast<ParticleSystemTypeClass *>(systems[Random_Pick(0,systems.Count()-1)]),Position_Coord_00450950(*this)+smoke_offset,0,this,unknown_geometry_l);
    }
    Strength=0;
    if (forced) unknown_5b0=true;
    Spawn_Survivors_0043f8e0(static_cast<ObjectClass *>(unknown_438));
}

// Actual original dynamic-vector79D0CC slot bindings; auxiliary duplicates, zero new credit.


// 0043EBA0: replace the footprint with rubble and optionally leave a crate.
extern Rect unknown_rect_839630; // existing declaration in shared/display.hpp
static inline bool Is_List_End(Cell const *offset);
void BuildingClass::Leave_Rubble_0043eba0()
{
    void *image;
    int frame;
    if (Type->unknown_func_458e00(&image, &frame)) {
        Cell const *offset = Occupy_List(0);
        Cell origin = Coord_Cell(unknown_slot42());
        while (!Is_List_End(offset)) {
            Cell position = *offset++ + origin;
            CellClass &cell = Map[position];
            cell.Register_For_Redraw();
            cell.OverlayIndex = 0xef;
            cell.unknown_40 = 0;
            cell.Recalc_Attributes(-1);
            Map.Update_Cell_Zone(cell.MapCoords);
            Map.Unknown_00566340(cell.MapCoords);
            Unknown_006D7C80(&cell);
        }
        CellClass &base = Map[origin];
        base.unknown_40 = Type;
        Rect area = Union(base.Overlay_Render_Rect(), base.Overlay_Shadow_Render_Rect());
        Point2D view_origin(unknown_rect_839630.X, unknown_rect_839630.Y);
        Point2D translated(area.X - view_origin.X, area.Y - view_origin.Y);
        unknown_tactical_83996c->Register_Dirty_Area(Rect(RectPoint2D(translated.X, translated.Y), area.Width, area.Height), 0);
        unknown_tactical_83996c->Flag_Cell(&Map[origin]);
    }
    if (Type->CrateBeneath) {
        Cell position = Coord_Cell(unknown_slot42());
        if (Type->CrateBeneathIsMoney) {
            Map.Unknown_0054E580(position, 0);
            return;
        }
        Map.Unknown_0054E580(position, 0x14);
    }
}


// Apply building damage, notify docked contacts, and update damage animations.
class TechnoClass;
// Real original out-of-line base constructor copy; no new body credit.
// Generic complete constructor definition from the real shared vector header remains available.
CoordStruct __fastcall Coord_Scatter(CoordStruct const &coord, int distance, bool lock);
inline CoordStruct Position_Coord_00450950(ObjectClass const &object);
extern bool Damage_Radio_Runaway_007cf670;
extern float const Damage_Spawn_Multiplier_0079d0e8;
inline UnknownVector3 Damage_Coord_Difference_0043ee70(CoordStruct const &left, CoordStruct const &right)
{
    return UnknownVector3(left.X-right.X,left.Y-right.Y,left.Z-right.Z);
}
// Hypothesis: object-distance operations composed from the coordinate difference.
// Both virtual coordinates finish before any component is subtracted.
inline int Damage_Object_Distance_0043ee70(ObjectClass const &origin,AbstractClass const &target)
{
    UnknownVector3 difference=Damage_Coord_Difference_0043ee70(origin.Center_Coord(),target.Center_Coord());
    return difference.Length();
}
inline double Damage_Object_Distance_Squared_0043ee70(ObjectClass const &origin,AbstractClass const &target)
{
    UnknownVector3 difference=Damage_Coord_Difference_0043ee70(origin.Center_Coord(),target.Center_Coord());
    return (double)difference.Z*difference.Z+(double)difference.Y*difference.Y+(double)difference.X*difference.X;
}
// The original random direction stores the RNG low byte in direction bits8..15.
// The upper half of the four-byte direction representation remains unspecified.
inline UnknownFacingDir Damage_Random_Direction_0043ee70()
{
    union DirectionBits {
        unsigned packed;
        unsigned short halves[2];
        struct { unsigned low_byte:8; unsigned direction_byte:8; };
    } bits;
    bits.halves[0]=0;
    bits.direction_byte=unknown_id_source_a3d290->Random()();
    UnknownFacingDir result;
    *reinterpret_cast<unsigned *>(&result)=bits.packed;
    return result;
}

int BuildingClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int no_crew)
{
    int result=0;
    if (this!=source) {
    Health_Ratio();
    int old_frame=unknown_43be20();
    if (source && !unknown_slot31()) {
        House->unknown_5408=Frame;
        House->unknown_540c=source->Owning_House()->ArrayIndex;
        unknown_6d3400(source);
    }
    Cell const *offset=Occupy_List(0);
    DynamicVectorClass<TechnoClass *> contacts;
    for (int index=0;index<RadioLinks.Length();++index) {
        TechnoClass *contact=Contact(index);
        if (contact) contacts.Add(contact);
    }
    if (Type->LaserFence && !forced) return 0;
    if (Type->BridgeRepairHut && Type->unknown_203) return 0;
    if (Strength!=0) {
        result=TechnoClass::Take_Damage(damage,distance,warhead,source,forced,no_crew);
        if (!unknown_7d) return result;
        switch (result) {
        case 5:
            return result;
        case 4: {
            for (int index=0;index<contacts.Count();++index) {
                TechnoClass *contact=contacts[index];
                if (Damage_Object_Distance_0043ee70(*this,*contact)<256 || Type->Helipad) {
                    int strength=10*contact->Techno_Type_Class()->MaxStrength;
                    contact->Take_Damage(&strength,0,Rules->unknown_c74,0,true,1);
                } else {
                    unknown_slot150(23,contact);
                    contact->unknown_3f8=0;
                }
            }
            contacts.Delete_All();
            if (Type->CanBeOccupied) unknown_452f50(0);
            if (unknown_4e8) unknown_4e8->Deactivate(false);
            unknown_slot286(0,source,forced,offset);
            if (unknown_420.Value()>0) {
                Delete_Me();
                Leave_Rubble_0043eba0();
            }
            break;
        }
        case 2:
            if (unknown_25c[2]) static_cast<ParticleSystemClass *>(unknown_25c[2])->unknown_d4 *= 1.5f;
        case 3:
            if (Damage_Radio_Runaway_007cf670) {
                for (int index=0;index<RadioLinks.Length();++index) {
                    TechnoClass *contact=Contact(index);
                    if (contact) {
                        int separation=(int)unknown_sqrt_4ba410(Damage_Object_Distance_Squared_0043ee70(*this,*contact));
                        if (separation<512 && !Type->Helipad) {
                            unknown_slot150(23,contact);
                            contact->unknown_3f8=0;
                        }
                    }
                }
            }
            if (Techno_Type_Class()->DamageSound==-1) Voc_Static_Add(Rules->BuildingDamageSound,Position_Coord_00450950(*this),0);
            while (*offset!=Cell(32767,32767)) {
                Cell relative(*offset++);
                Cell cell=unknown_slot104()+relative;
                CoordStruct location(cell);
                location.Z=Map.Get_Height_GL(location);
                if (warhead->Sparky) {
                    AnimClass *fire=0;
                    switch (Random_Pick(0,5+Type->Foundation_Height(false)+Type->Width())) {
                    case 1: case 2: case 3: case 4: case 5:
                        fire=new AnimClass(Rules->OnFire[0],Coord_Scatter(location,96,false),0,Random_Pick(1,3),0x600,0);
                        break;
                    case 6: case 7: case 8:
                        fire=new AnimClass(Rules->OnFire[1],Coord_Scatter(location,96,false),0,Random_Pick(1,3),0x600,0);
                        break;
                    case 9:
                        fire=new AnimClass(Rules->OnFire[2],Coord_Scatter(location,96,false),0,1,0x600,0);
                        break;
                    }
                    if (fire) fire->Attach_To(this);
                }
            }
            break;
        }
    }
    if (!unknown_7d) return 4;
    if (source) {
        if (result==0) goto refresh_frame;
        if (!Type->unknown_202 && !unknown_slot31()) House->Attacked(this);
        unknown_434=source->Owning_House()->ArrayIndex;
        if (Mission!=19 && !House->Is_Ally(source) && unknown_slot231(0)->WeaponType && !unknown_slot231(0)->WeaponType->Projectile->AA && (!unknown_228 || !unknown_slot213(unknown_228))) {
            if (source->Kind_Of()==2 || (House->Unknown_004F52F0() && !Rules->PlayerReturnFire)) {
                if (!unknown_2e0.Is_Rotating()) unknown_2e0.Set_Desired(Damage_Random_Direction_0043ee70());
            } else unknown_slot220(source);
        }
    }
    if (result!=0) unknown_44d070(Health_Ratio()<=Rules->unknown_1368);
refresh_frame:
    if (old_frame!=unknown_43be20()) unknown_75=true;
    }
    return result;
}


// void BuildingClass::Init() (0x0043F840, slot 9)
// TechnoClass::Init, then registration with the owner and the type's
// strength, ammunition and turn rate; loads the buildup image (type slot 48)
// or clears unknown_5ac.

void BuildingClass::Init()
{
    TechnoClass::Init();
    if (House) {
        unknown_434 = House->Type->House;
        House->unknown_4ea970(this);
    }
    if (Type) {
        unknown_70 = Strength = Type->MaxStrength;
        if (Type->InitialAmmo == -1) {
            unknown_254 = Type->Ammo;
        } else {
            unknown_254 = Type->InitialAmmo;
        }
        unknown_2e0.Set_ROT(Type->ROT);
        if (!Type->Get_Buildup_Data()) {
            unknown_5ac = false;
            return;
        }
        Type->Free_Buildup();
        unknown_5b9 = true;
    }
}


// Complete original 0043F8E0: 1099 body bytes and 5 padding bytes.
// Name and unused void return are hypotheses. Explicit input is the attacker.
// Reuse the Building unit's existing genuine adjacent-cell operation before
// the Infantry header sees its shared guarded copy. Operand order is retained.
#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
#include "shared/adjacent_cell.hpp"
inline Cell Adjacent_Cell(Cell const &cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}
#endif
// Identical existing declaration from bullet.hpp; no Bullet layout needed.
CoordStruct __fastcall Coord_Scatter(CoordStruct const &coord, int distance, bool lock);
void BuildingClass::Spawn_Survivors_0043f8e0(ObjectClass *attacker)
{
    Cell origin=unknown_slot104();
    Cell const *offset=Occupy_List(0);
    int odds=2;
    if(unknown_438)odds=1;
    if(unknown_5b3)odds+=6;
    CoordStruct coordinate;
    int remaining=unknown_slot172();
    if(remaining==0)return;
    while(offset->X!=32767 || offset->Y!=32767) {
        Cell location=*offset++ + origin;
        CellClass *cell=&Map[location];
        if(!House->unknown_13e && remaining>0 && unknown_id_source_a3d290->Random()(0,odds)==1) {
            InfantryClass *survivor=0;
            InfantryTypeClass *crew=unknown_slot184();
            if(crew)survivor=new InfantryClass(crew,House);
            Debug_Printf("Creating survivor type '%s' from building type '%s'\n",survivor->Type->Name.Buffer,Type->Name.Buffer);
            if(survivor) {
                if(unknown_5b9 && survivor->Type->Nominal)survivor->unknown_5c1=true;
                ++unknown_a40774;
                coordinate=CoordStruct(location.X*256+128,location.Y*256+164,0);
                coordinate=((UnknownPlacementCellView *)&Map[coordinate])->Closest_Free_Spot(coordinate,false,false);
                if(coordinate==unknown_geometry_l || !survivor->Unlimbo(&coordinate,0)) {
                    delete survivor;
                } else {
                    Debug_Printf("Survivor unlimbo OK\n");
                    --remaining;
                    int maximumStrength=survivor->Type->MaxStrength;
                    survivor->Strength=unknown_id_source_a3d290->Random()(5,maximumStrength);
                    survivor->Scatter(unknown_geometry_l,true,false);
                    if(attacker && !House->Is_Ally(attacker)) {
                        survivor->QueueMission(1,false);
                        survivor->unknown_slot220(attacker);
                    } else if(House->Is_Human_Player()) {
                        survivor->QueueMission(2,false);
                    } else {
                        survivor->QueueMission(15,false);
                    }
                }
                --unknown_a40774;
            }
        }
        if(cell->Is_Clear_To_Move(1,1,1,-1,0,-1,1)) {
            if(unknown_id_source_a3d290->Random()(0,99)<50) {
                Cell hit=Coord_Cell(Coord_Scatter(CoordStruct(location.X*256+128,location.Y*256+128,0),128,false));
                CoordStruct center(hit.X*256+128,hit.Y*256+128,0);
                SmudgeTypeClass::Create_Scorch(center,100,100,false);
            } else {
                Cell hit=Coord_Cell(Coord_Scatter(CoordStruct(location.X*256+128,location.Y*256+128,0),128,false));
                CoordStruct center(hit.X*256+128,hit.Y*256+128,0);
                SmudgeTypeClass::Create_Crater(center,100,100,false);
            }
        }
    }
}


// Handle a building object action and queue the corresponding packed network event.
bool BuildingClass::unknown_slot78(int action,ObjectClass *target,bool arg3)
{
    if(action==5) {
        if(target) unknown_slot200(1,target,0,0);
    } else if(action==4) {
        if(Type->Factory!=0) OutList.Add(EventClass(Owner(),0x11,TargetClass(this)));
        if(Type->GapGenerator) unknown_slot200(0x10,target,0,0);
        if(unknown_554.Count()>0) {
            unknown_slot200(0x10,target,0,0);
            return true;
        }
    } else {
        if(action==0x37) {
            return (OutList.Add(EventClass(unknown_38->unknown_28->ArrayIndex,10,TargetClass(this))),true);
        } else {
            if(action!=1) return true;
            int factory=Type->Factory;
            if(factory==16 || factory==40 || factory==3) {
                Cell cell=Coord_Cell(target->Center_Coord());
                unknown_440140(&cell,true);
                return true;
            }
            return (OutList.Add(EventClass(Owner(),0x1e,TargetClass(this),TargetClass(target))),true);
        }
    }
    return true;
}


// bool BuildingClass::unknown_slot77(int action, Cell const *arg2, Cell const *arg3, bool arg4) (0x0043FFE0)
// Action 5 hands the cell (arg2: a Cell pointer) to slot 200 as
// action 1. Action 1: a construction yard of a house failing 0x004F52F0,
// or outside game mode/flag 0x00A3D340, is held back; otherwise a building
// that undeploys or passes slot 153 runs 0x00440140 for the cell. A
// deployable building not held back then queues event 0x16 for itself.
// Always true.


bool BuildingClass::unknown_slot77(int action, Cell const *arg2, Cell const *arg3, bool arg4)
{
    if (action == 5) {
        unknown_slot200(1, &Map[*arg2], 0, 0);
        return true;
    }
    if (action == 1) {
        bool held = false;
        if (Type->ConstructionYard && (!House->Unknown_004F52F0() || unknown_game_mode_a3d298 == 0 || !unknown_flag_a3d340)) {
            held = true;
        } else if (Type->UndeploysInto != 0 || unknown_slot153()) {
            unknown_440140(arg2, true);
        }
        if (Type->UndeploysInto != 0 && !held) {
            OutList.Add(EventClass(Owner(), 0x16, TargetClass(this)));
        }
    }
    return true;
}


// 00440140: select a reachable rally cell and queue the building's target event.
// Full original body is 777 bytes, including both queue-add paths, plus 7 NOPs.
void BuildingClass::unknown_440140(Cell const *cell, bool announce)
{
    int speed = 0;
    int zone_type = 0;
    bool bridge = ((Map[*cell].unknown_140 >> 8) & 1) != 0;
    if (Type->Factory == 3) { speed = 4; zone_type = 9; }
    if (Type->Naval) { speed = 6; zone_type = 4; }
    CoordStruct location=Coord;
    int zone = Map.Zone(Coord_Cell(location), zone_type, bridge);
    Cell found = Map.Nearby_Location(*cell, speed, zone, zone_type, bridge, Point2D(1,1), 0,0,0,1,Cell(0,0),0);
    if (found != *(Cell const *)&unknown_84ed10) {
        OutList.Add(EventClass(Owner(),0x1e,TargetClass(this),TargetClass((AbstractClass *)&Map[found])));
    } else if (Type->ConstructionYard && House->Unknown_004F52F0() && unknown_game_mode_a3d298 && unknown_flag_a3d340) {
        OutList.Add(EventClass(Owner(),0x1e,TargetClass(this),TargetClass((AbstractClass *)&Map[Center_Coord()])));
        return;
    }
    if (announce && House->Is_Player_Control() && !Type->ConstructionYard) {
        Speech_Play_Name("EVA_NewRallyPointEstablished",-1,-1);
    }
}


// Update the attack target; unload deployed vehicles denied a ground target.
void BuildingClass::unknown_slot220(AbstractClass *target)
{
    if (Mission == 19 || !unknown_slot195())
        target = 0;
    if (!target || !unknown_slot231(0)->WeaponType
        || unknown_slot231(0)->WeaponType->Projectile->AA
        || unknown_slot213(target)) {
        TechnoClass::unknown_slot220(target);
    } else {
        TechnoClass::unknown_slot220(0);
        if ((Type->TickTank || Type->Artillary)
            && !House->Is_Human_Player() && !unknown_slot201()) {
            QueueMission(19, false);
            Commence();
        }
    }
}


// Building production options available to the local player's sidebar.
void BuildingClass::unknown_slot283()
{
    if (House == unknown_player_a35db4 && !unknown_76 && unknown_373 && unknown_530) {
        switch (Type->Factory) {
        case 7: {
            for (int index = 0; index < BuildingTypes.Count(); ++index) {
                if (unknown_player_a35db4->Can_Build(BuildingTypes[index], 0, 1))
                    Map.Unknown_0067C530(7, index);
            }
            break;
        }
        case 40: {
            for (int index = 0; index < UnitTypes.Count(); ++index) {
                if (unknown_player_a35db4->Can_Build(UnitTypes[index], 0, 1))
                    Map.Unknown_0067C530(40, index);
            }
            break;
        }
        case 16: {
            for (int index = 0; index < InfantryTypes.Count(); ++index) {
                if (unknown_player_a35db4->Can_Build(InfantryTypes[index], 0, 1))
                    Map.Unknown_0067C530(16, index);
            }
            break;
        }
        case 3: {
            for (int index = 0; index < AircraftTypes.Count(); ++index) {
                if (unknown_player_a35db4->Can_Build(AircraftTypes[index], 0, 1))
                    Map.Unknown_0067C530(3, index);
            }
            break;
        }
        }
    }
}


// Not exact (RA2_NON_MATCHING): BuildingClass::Exit_Object at 0x00440520, best score 86.73, 2026-11-13; differs: 1873 instructions in the original, 1886 in the draft (shape 97.74)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x440520 FUN_00440520
// entry 0x440520; exact-match score 92.27 (misses_index best), this text 91.93; plan "draft, verify"
// origin lane: Scout761 (build/class-work/Scout761/keep/eo_F_9193.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): slots 71/260 under their tree names (Mark, Assign_Destination); one Cell sum spelled out; each changed line is marked NATIVE.
// BuildingClass::Exit_Object (TibSun name): puts a produced object into the world, complete original body 00440520.
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
                if (aircraft->Unlimbo(&CoordStruct(nearby, 0), aircraft->Pose_Dir())) {
                    --unknown_a40774;
                    return 2;
                }
            } else {
                if (aircraft->Unlimbo(&unknown_slot41((int)aircraft), aircraft->Pose_Dir())) {
                    unknown_slot150(2, aircraft);
                    unknown_slot150(0x18, aircraft);
                    aircraft->Set_Coord(unknown_slot41((int)aircraft));
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
#endif

// bool BuildingClass::Limbo() (0x00442060)
// Takes the building off the map (hypothesis: Limbo). Unless already hidden
// (unknown_76) or typeless: its eight attachments get slot 60 and are
// cleared; an active ore purifier (unknown_5b4) lowers its house's purifier
// count and an active helipad its dock count (neither below 0); a laser
// fence post runs 0x0044DC40(0); outside the special mode a sensor array
// and a disguise detector run slots 289 and 291 with 0x0084ED10, and slot 31
// failing runs 0x004513B0. An upgrade that is a threat-rating node marks the
// house for 0x004F3160. With map updates on (0x00A40958): the rules' wall
// types refresh the surrounding cells (re-raising their wall overlays) or
// their X/Y neighbours, a building without +0xBF0 releases its foundation
// and border cells (+0x122), the screen rectangle is redrawn, +0x4D4 gets
// slot 60 and the house forgets the building; then the techno's slot 52.
// A factory tells its house it left. A laser fence post outside the special
// mode clears +0x4EC.

// Set when the player loses a building (0x008339E8; hypothesis).
extern bool unknown_flag_8339e8;

// The cell offsets of the eight facings (0x00851A70; Westwood's AdjacentCell).
#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
extern Cell const AdjacentCell[8];
// The cell next to `cell` toward an 8-way facing.
inline Cell Adjacent_Cell(Cell const &cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}
#endif

// The cell members used here (a view of CellClass).
class UnknownBuildingCell52 {
public:
    void Unknown_00475960(int arg1);            // 0x00475960 (refreshes the cell; hypothesis)
    void Unknown_00475FE0(int arg1);            // 0x00475FE0 (raises the wall overlay; hypothesis)

    char unknown_000[0x44];
    int Overlay;                                // +0x044 (-1: none)
    char unknown_048[0x11E - 0x48];
    unsigned char unknown_11e;                  // +0x11E (overlay data)
    char unknown_11f[0x122 - 0x11F];
    char unknown_122;                           // +0x122 (buildings counting on the cell)
};

// Member-wise cell sum (as shared/cell.hpp's proposed Cell::operator+).
#ifndef RA2_CELLSTRUCT_PLUS
#define RA2_CELLSTRUCT_PLUS
inline Cell operator+(Cell const &left, Cell const &right)
{
    return Cell(left.X + right.X, left.Y + right.Y);
}
#endif


bool BuildingClass::Limbo()
{
    bool threat_node = false;
    CoordStruct center = Coord;
    Cell cell = Coord_Cell(center);
    bool result;
    if (!unknown_76 && Type != 0) {
        int index;
        for (index = 0; index < 8; index++) {
            if (unknown_49c[index] != 0) {
                unknown_49c[index]->Delete_Me();
                unknown_49c[index] = 0;
            }
        }
        if (Type->OrePurifier && unknown_5b4) {
            House->unknown_52cc--;
            if (House->unknown_52cc < 0) {
                House->unknown_52cc = 0;
            }
        }
        if (Type->Helipad && unknown_5b4) {
            House->unknown_218 -= Type->NumberOfDocks;
            if (House->unknown_218 < 0) {
                House->unknown_218 = 0;
            }
        }
        if (Type->LaserFencePost) {
            unknown_44dc40(0);
        }
        if (unknown_a40774 == 0) {
            if (Type->SensorArray) {
                unknown_slot289(unknown_geometry_k);
            }
            if (Type->DetectDisguise) {
                unknown_slot291(unknown_geometry_k);
            }
            if (!unknown_slot31()) {
                unknown_4513b0();
            }
        }
        for (index = 0; index < 3; index++) {
            if (unknown_4c0[index] != 0 && ((BuildingTypeClass *)unknown_4c0[index])->IsThreatRatingNode) {
                threat_node = true;
                break;
            }
        }
        if (!unknown_flag_a40958) {
            result = true;
            goto fence;
        }
        if (Type == Rules->unknown_78c) {
            for (int facing = 0; facing < 8; facing += 2) {
                UnknownBuildingCell52 &near_cell = MAP_CELL(Adjacent_Cell(cell, facing));
                near_cell.Unknown_00475960(0);
                if (near_cell.Overlay != -1 && unknown_overlay_types_a35de8[near_cell.Overlay]->unknown_278
                    && near_cell.unknown_11e < 0x10) {
                    near_cell.Unknown_00475FE0(200);
                }
            }
        }
        if (Type == Rules->unknown_77c || Type == Rules->unknown_784) {
            MAP_CELL(Cell(cell.X - 1, cell.Y)).Unknown_00475960(0);
            MAP_CELL(Cell(cell.X + 3, cell.Y)).Unknown_00475960(0);
        }
        if (Type == Rules->unknown_780 || Type == Rules->unknown_788) {
            MAP_CELL(Cell(cell.X, cell.Y - 1)).Unknown_00475960(0);
            MAP_CELL(Cell(cell.X, cell.Y + 3)).Unknown_00475960(0);
        }
        if (Type->unknown_bf0 == 0) {
            int width = Type->Width();
            int height = Type->Foundation_Height(false);
            Cell base = unknown_slot104();
            for (int y = 0; y < height + 2; y++) {
                for (int x = 0; x < width + 2; x++) {
                    Cell corner(base.X - 1, base.Y - 1);
                    UnknownBuildingCell52 &spot = MAP_CELL(corner + Cell(x, y));
                    spot.unknown_122--;
                }
            }
        }
        if (unknown_tactical_83996c != 0) {
            unknown_tactical_83996c->Register_Dirty_Area(Get_Render_Rect(), 0);
        }
        if (unknown_4d4 != 0) {
            ((ObjectClass *)unknown_4d4)->Delete_Me();
        }
        House->unknown_func_4eaba0(this);
        House->unknown_144 = true;
        House->unknown_func_4e8640();
        if (House == unknown_player_a35db4) {
            unknown_flag_8339e8 = true;
            Map.Flag_To_Redraw(0);
        }
        House->unknown_func_4f3f40(this);
    }
    result = TechnoClass::Limbo();
    if (threat_node) {
        House->unknown_func_4f3160();
    }
    if (Type->Factory != 0) {
        House->unknown_func_4f3170(Type->Factory, Type->Naval, 0);
    }
fence:
    if (Type->LaserFencePost && unknown_a40774 == 0) {
        unknown_4ec = 0;
    }
    return result;
}


// UnknownFacingDir BuildingClass::unknown_slot162() (0x004425A0, slot 162)
// The direction from this center towards the object at +0x228 unless
// slot 232 holds or there is none; otherwise the current facing at +0x2E0.

// The direction from one coordinate to another (an inline helper;
// hypothesis: Westwood's Direction()).
static inline UnknownFacingDir Coord_Direction(CoordStruct const &from, CoordStruct const &to)
{
    int fx = from.X;
    int tx = to.X;
    double angle = Fast_Atan2((double)from.Y - to.Y, (double)tx - fx);
    return UnknownFacingDir((unsigned short)(int)((angle - 1.5707963267948966) * -10430.060040584269));
}

UnknownFacingDir BuildingClass::unknown_slot162()
{
    if (!unknown_slot232() && unknown_228) {
        return Coord_Direction(Center_Coord(), unknown_228->Center_Coord());
    }
    return unknown_2e0.Current();
}


// AbstractClass *BuildingClass::unknown_slot219(int arg1, CoordStruct const *coord, bool arg3) (0x00442650, slot 219)
// Adds the projectile flags of both weapons (0x00733A60) and flag 1 to
// arg1, then the techno behaviour.

AbstractClass *BuildingClass::unknown_slot219(int arg1, CoordStruct const *coord, bool arg3)
{
    if (unknown_slot231(0)->WeaponType) {
        arg1 |= unknown_slot231(0)->WeaponType->Allowed_Threats();
    }
    if (unknown_slot231(1)->WeaponType) {
        arg1 |= unknown_slot231(1)->WeaponType->Allowed_Threats();
    }
    return TechnoClass::unknown_slot219(arg1 | 1, coord, arg3);
}


// Building completion/capture initialization, complete original body 004426D0.
extern "C" char const unknown_turret_separator_007cf730[];
extern "C" unsigned int grand_turret_frames_007acab0[32];
extern "C" Cell grand_none_0084ed10;
extern "C" Cell grand_south_step_00851a80;
// Aircraft heading member's original 9-byte body was independently reproduced.
// No canonical rename or header migration has been applied.
void BuildingClass::unknown_slot282(bool captured)
{
    if (unknown_5b4 && !captured) return;
    if (Type->ProduceCashDelay) {
        unknown_5a0 = CDTimerClass<FrameTimerClass>(Type->ProduceCashDelay);
    }
    if (!unknown_5b4) {
        if (Type->Refinery) {
            int storage_stage = (int)unknown_294.Get_Total_Amount();
            if (storage_stage != 0) storage_stage = ((int)unknown_294.Get_Total_Amount() << 2) / Type->Storage;
            unknown_5c0 = storage_stage;
            if (storage_stage >= 3) {
                unknown_44c920(6, Health_Ratio() <= Rules->unknown_1368, false, 0);
            } else if (storage_stage >= 2) {
                unknown_44c920(5, Health_Ratio() <= Rules->unknown_1368, false, 0);
            } else if (storage_stage >= 1) {
                unknown_44c920(4, Health_Ratio() <= Rules->unknown_1368, false, 0);
            } else if (storage_stage >= 0) {
                unknown_44c920(3, Health_Ratio() <= Rules->unknown_1368, false, 0);
            }
        } else {
            unknown_44c920(17, Health_Ratio() <= Rules->unknown_1368, unknown_554.Count() > 0, 0);
            unknown_44c920(3, Health_Ratio() <= Rules->unknown_1368, unknown_554.Count() > 0, 0);
            unknown_44c920(4, Health_Ratio() <= Rules->unknown_1368, false, 0);
            unknown_44c920(5, Health_Ratio() <= Rules->unknown_1368, false, 0);
            unknown_44c920(6, Health_Ratio() <= Rules->unknown_1368, false, 0);
        }
        if (Type->OrePurifier) ++House->unknown_52cc;
        if (Type->Helipad) {
            House->unknown_218 += Type->NumberOfDocks;
            // The canonical sidebar member is reached through the actual Map base.
            Map.Unknown_0067C310(3);
        }
        if (Type->SuperWeapon != -1) {
            for (int i = 0; i < House->Supers.Count(); ++i) {
                SuperClass *super = House->Supers[i];
                if (super->Type->Type == Type->SuperWeapon) {
                    if ((super->RechargeTimer.Value() / 15) * 60 <= 4) {
                        unknown_44c920(15, Health_Ratio() <= Rules->unknown_1368, unknown_554.Count() > 0, 0);
                    } else {
                        unknown_44c920(13, Health_Ratio() <= Rules->unknown_1368, unknown_554.Count() > 0, 0);
                    }
                }
            }
        }
        if (unknown_slot232() && !Type->TurretAnimIsVoxel) {
            if (unknown_4d0 != -1) {
                int frame = unknown_4d0;
                unknown_4d0 = -1;
                Set_Turret_Animation_Frame_0043bd40(frame);
            } else {
                unknown_44c920(9, false, false, 0);
            }
            if (unknown_slot232() && unknown_454[9]) {
                unsigned int frame = grand_turret_frames_007acab0[Dir_To_32(unknown_2e0.Current())];
                AnimClass *animation = static_cast<AnimClass *>(unknown_454[9]);
                if (animation) {
                    animation->Set_Stage(frame);
                    animation->Set_Step(0);
                }
            }
        }
        if (Type->LightIntensity) {
            if (!unknown_4e8) {
                unknown_4e8 = new LightSourceClass(Center_Coord(), Type->LightVisibility,
                    Type->LightIntensity, Type->LightRedTint, Type->LightGreenTint, Type->LightBlueTint);
            }
            unknown_4e8->Activate(0);
        }
        if (Type->LaserFencePost) unknown_44e5a0(false);
        if (Type->SensorArray) unknown_slot288(grand_none_0084ed10);
        if (Type->DetectDisguise) unknown_slot290(grand_none_0084ed10);
        unknown_5ad = true;
    }
    if (Type->RevealToAll && !House->Is_Player_Control()) {
        if (!captured) {
            unknown_slot262(false, false, true, unknown_player_a35db4);
            CoordStruct coordinate = Coord;
            Map.Unknown_0054A550(coordinate, 0, unknown_1f4 + 3, 0);
        }
        Unknown_0063B5B0(12, Cell(Position_Coord_004f3f40()));
    }
    if (!House->Is_Player_Control() && unknown_int_a3d548 == 0 && unknown_game_mode_a3d298 != 0 &&
        (Type->SuperWeapon == -1 || !Supers[Type->SuperWeapon]->Type->AuxBuilding ||
         House->unknown_5460.unknown_func_491d10(Supers[Type->SuperWeapon]->Type->AuxBuilding->ArrayIndex) != 0)) {
        switch (Type->SuperWeapon) {
        case 0: Speech_Play_Name("EVA_NuclearSiloDetected", -1, -1); break;
        case 1: Speech_Play_Name("EVA_IronCurtainDetected", -1, -1); break;
        case 2: Speech_Play_Name("EVA_WeatherDeviceReady", -1, -1); break;
        case 3: Speech_Play_Name("EVA_ChronosphereDetected", -1, -1); break;
        }
    }
    if (Type->WeaponsFactory && Type->Naval) {
        Cell result = Map.Nearby_Location(unknown_slot104(), 5, -1, 10, false, Point2D(5, 5),
            false, false, false, true, Cell(0, 0), 0);
        result = Cell(result.X + 2, result.Y + 2);
        if (result != grand_none_0084ed10) unknown_440140(&result, false);
    }
    House->unknown_144 = true;
    unknown_5b4 = true;
    House->unknown_5668 = true;
    House->unknown_5669 = true;
    if (Techno_Type_Class()->GapGenerator) unknown_slot235();
    if (Type->FreeUnit && unknown_a40774 == 0 && !captured && !Debug_Map &&
        (!House->Is_Human_Player() || unknown_258 == 0 || unknown_258 > Type->Raw_Cost())) {
        Cell origin(Center_Coord());
        short south_y = (short)(origin.Y + grand_south_step_00851a80.Y);
        short south_x = (short)(origin.X + grand_south_step_00851a80.X);
        Cell cell(south_x, south_y);
        bool placed = false;
        UnitClass *unit = new UnitClass(Type->FreeUnit, House);
        if (unit) {
            if (!unit->Unlimbo(&CoordStruct(cell), 0xC0)) {
                Cell nearby_cell = Map.Nearby_Location(Cell(Position_Coord_004f3f40()), 2,
                    Map.Zone(Cell(Position_Coord_004f3f40()), unit->Type->MovementZone, false),
                    unit->Type->MovementZone, false, Point2D(1, 1), true, true, false, false, Cell(0, 0), 0);
                if (nearby_cell == grand_none_0084ed10 || !unit->Unlimbo(&CoordStruct(nearby_cell), 0xA0)) {
                    Cell new_cell = Map.Nearby_Location(Cell(Position_Coord_004f3f40()), 2,
                        Map.Zone(Cell(Position_Coord_004f3f40()), unit->Type->MovementZone, false),
                        unit->Type->MovementZone, false, Point2D(1, 1), false, true, false, false, Cell(0, 0), 0);
                    if (new_cell == grand_none_0084ed10 || !unit->Unlimbo(&CoordStruct(new_cell), 0xA0)) {
                        House->Refund_Money(unit->Type->unknown_slot46(House, true));
                        delete unit;
                    } else {
                        placed = true;
                    }
                } else {
                    placed = true;
                }
            } else {
                placed = true;
            }
            if (placed) {
                unit->QueueMission(10, false);
                unit->Commence();
            }
        } else {
            House->Refund_Money(Type->FreeUnit->unknown_slot46(House, true));
        }
    }
    if (!Rules->unknown_142c && Type->HoverPad && !captured) {
        ++unknown_a40774;
        AircraftClass *aircraft = new AircraftClass(Rules->PadAircraft[0], House);
        if (aircraft) {
            aircraft->Set_Height_AGL(0);
            if (aircraft->Unlimbo(&Center_Coord(), aircraft->Pose_Dir())) {
                aircraft->QueueMission(5, false);
                aircraft->unknown_slot150(2, this);
                unknown_slot150(0x18, aircraft);
            }
        }
        --unknown_a40774;
    }
}


// void BuildingClass::Repair(int mode) (0x00443680)
// Sets the repair flag (unknown_5b8): -1 toggles it, 0 clears and 1 sets it
// (no change: nothing happens). When repair starts on a damaged building of
// a player-controlled owner, slot 79(7) runs and EVA says "EVA_Repairing";
// the toggle sound (the rules' +0x620 for a building at full strength, +0x62C
// otherwise) plays at the building for such an owner.


void BuildingClass::Repair(int mode)
{
    switch (mode) {
    case -1:
        unknown_5b8 = !unknown_5b8;
        break;
    case 0:
        if (!unknown_5b8) {
            return;
        }
        unknown_5b8 = false;
        break;
    case 1:
        if (unknown_5b8) {
            return;
        }
        unknown_5b8 = true;
        break;
    }
    int sound;
    if (unknown_5b8) {
        if (Strength == Type->MaxStrength) {
            sound = Rules->unknown_620;
        } else {
            sound = Rules->unknown_62c;
            if (House->Is_Player_Control()) {
                Clicked_As_Target(7);
            }
            unknown_5ae = true;
            if (House->Is_Player_Control()) {
                Speech_Play_Name("EVA_Repairing", -1, -1);
            }
        }
    } else {
        sound = Rules->unknown_62c;
    }
    if (House->Is_Player_Control()) {
        Voc_Play_At(sound, CoordStruct(Coord), 0);
    }
}


// void BuildingClass::Sell_Back(int mode) (0x004437A0)
// Sells the building (mission 0x13; hypothesis: Sell_Back): -1 toggles,
// 0 cancels (nothing unless selling) and 1 starts it (not while selling or
// while unknown_5af is set); the sell sound (the rules' +0x62C) plays for a
// player-controlled owner. A building that cannot be sold (unknown_5b9
// clear) only reacts when it is a firestorm wall: its type's slot 33 for the
// owner, the owner's 0x004F52F0, then slots 52 and 60.

void BuildingClass::Sell_Back(int mode)
{
    if (unknown_5b9) {
        bool sell = false;
        switch (mode) {
        case -1:
            sell = Get_Mission() != 0x13;
            break;
        case 0:
            if (Get_Mission() != 0x13) {
                return;
            }
            break;
        case 1:
            if (Get_Mission() == 0x13) {
                return;
            }
            if (unknown_5af) {
                return;
            }
            sell = true;
            break;
        }
        if (sell) {
            QueueMission(0x13, false);
            Commence();
        }
        if (House->Is_Player_Control()) {
            Voc_Play(Rules->unknown_62c, 1.0f, 0x2000, 0);
        }
    } else if (Type->FirestormWall) {
        Type->Cost_Of(House);
        House->Unknown_004F52F0();
        Limbo();
        Delete_Me();
    }
}


// Building object-selection action, 0x004438A0.
inline bool Building_Key_Down(int key);
int BuildingClass::unknown_slot28(ObjectClass *object, bool alternate)
{
    if (Type->InvisibleInGame) return 0;
    if (object->Kind_Of() == 6 && ((BuildingClass *)object)->Type->InvisibleInGame) return 0;
    int action = TechnoClass::unknown_slot28(object, alternate);
    if (action == 8) return 8;
    if (action == 4) {
        BuildingTypeClass *type = Type;
        int factory;
        if (type->GapGenerator && unknown_player_a35db4 == House) {
            if (!unknown_1fc && House->Power_Output() - House->Power_Drain() <= type->ExtraPowerDrain_c74) goto no_action;
        } else if (unknown_554.Count() <= 0) {
            if (type->Factory == 0) { action = 0; goto result_action; }
            goto primary_action;
        }
        factory = type->Factory;
        action = 4;
        if (factory == 0) goto result_action;
primary_action:
        if (unknown_32b) action = 0;
        goto result_action;
    }
    if (action == 5) {
        if (!unknown_slot231(0)->WeaponType) goto object_action;
        if (!unknown_slot231(0)->WeaponType->Projectile->AG || Type->EMPulseCannon) action = 0;
        if (Mission == 19) {
no_action:
            action = 0; goto result_action; }
    }
    if (action == 1 || action == 2) {
        if (!Can_Player_Move()) { action = 7; goto result_action; }
        int factory = Type->Factory;
        if (factory == 16 || factory == 40 || factory == 3) {
            if (!Building_Key_Down(unknown_key_a40ba4) && !Building_Key_Down(unknown_key_a40ba8)) { action = 7; goto result_action; }
            Cell cell = object->Center_Coord();
            if (Type->Factory != 3) {
                CoordStruct position;
                position.X = cell.X * 256 + 128;
                position.Y = cell.Y * 256 + 128;
                position.Z = 0;
                if (!unknown_slot171(position)) action = 2;
                if (!(Map[cell].unknown_140 & 0x100) && Map[cell].unknown_4c) { action = 2; goto result_action; }
            }
        }
    }
    if (action != 5) goto result_action;
object_action:
    if (object->Class_Of()->unknown_203) action = 2;
result_action:
    return action;
}


// Select an action for a cell using the base action and building-specific limits.
inline bool Building_Key_Down(int key) { return WWKeyboard->Down(key); }
int BuildingClass::unknown_slot27(Cell const *cell,bool alternate,bool mode)
{
    if(Type->InvisibleInGame) return 0;
    int action;
    if(Type->UndeploysInto && alternate) action=TechnoClass::unknown_slot27(cell,false,mode);
    else action=TechnoClass::unknown_slot27(cell,alternate,mode);
    if(action==1 || action==2) {
        if(!Can_Player_Move() && action==1) {
            action=0;
            goto owner_action;
        }
        if(unknown_slot153()) {
            if(!Building_Key_Down(unknown_key_a40ba4) && !Building_Key_Down(unknown_key_a40ba8)) {
                action=0;
                goto owner_action;
            }
            if(Type->Factory!=3) {
                CoordStruct position;
                position.X=cell->X*256+128;
                position.Y=cell->Y*256+128;
                position.Z=0;
                if(!unknown_slot171(position)) action=2;
                if(!(Map[*cell].unknown_140 & 0x100) && Map[*cell].unknown_4c) {
                    action=2;
                    goto owner_action;
                }
            }
        }
    }
    if(action==5) {
        if(!unknown_slot231(0)->WeaponType) goto result_action;
        if(!unknown_slot231(0)->WeaponType->Projectile->AG || Type->EMPulseCannon) action=0;
        if(Mission==19) action=0;
        else if(action==5) goto result_action;
    }
owner_action:
    if(unknown_slot153() && House->Is_Player_Control()) action=1;
result_action:
    return action;
}


// Change the building animation state and restart its frame countdown.
void BuildingClass::unknown_443d60(int state)
{
    unknown_430=state;
    if(unknown_42c==-1 || state==0 || unknown_a40774) {
        unknown_430=-1;
        unknown_42c=state;
        BuildingTriple const *control=&Type->AnimFrames[state];
        if(unknown_a40774) {
            unknown_44c920(17,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
            if(unknown_a40774) {
                unknown_44c920(3,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
                if(unknown_a40774) {
                    unknown_44c920(4,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
                    if(unknown_a40774) {
                        unknown_44c920(5,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
                        if(unknown_a40774) {
                            unknown_44c920(6,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
                        }
                    }
                }
            }
        }
        int rate=control->unknown_08;
        if(Type->Normalized && state!=0)
            rate=Options.Normalize_Delay(rate);
        StageClass::Set_Rate(rate);
        StageClass::Stage=control->unknown_00;
    }
}


// CoordStruct BuildingClass::Center_Coord() const (0x00444080, slot 18)
// The coordinate offset to the middle of the foundation: (width - 1) and
// (height - 1) half cells (128 leptons each) in X and Y.

CoordStruct BuildingClass::Center_Coord() const
{
    int y = Type->Foundation_Height(false) * 128 - 128;
    int x = Type->Width() * 128 - 128;
    CoordStruct coord = Coord;
    coord.X += x;
    coord.Y += y;
    return coord;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// CoordStruct BuildingClass::unknown_slot41(int dock) const 0x004440E0 (slot 41; TibSun building.cpp
// Docking_Coord): a weeder's dock two cells right and one down of its position cell at its height, a
// refinery's half a cell right of the object's dock, and for a helipad or repair depot the docking
// offset of the contact's dock index (NumberOfDocks/DockingOffsets). The function-scope `coord` the dock sums
// are assigned to is from ra2-decomp BuildingClass::Docking_Coord (YR 0x00447B20, src/building.cpp); the sums use
// TPoint3D<int>::operator+ (RA2_COORD_TPOINT3D).
CoordStruct BuildingClass::unknown_slot41(int dock) const
{
    if (Type->Weeder) {
        Cell cell = unknown_slot104() + Cell(2, 1);
        CoordStruct coord = CoordStruct(cell);
        coord.Z = CoordStruct(Coord).Z;
        return coord;
    }
    CoordStruct coord;

    if (Type->Refinery) {
        return ObjectClass::unknown_slot41(dock) + CoordStruct(128, 0, 0);
    }
    if (!Type->Helipad && !Type->UnitRepair) {
        return ObjectClass::unknown_slot41(dock);
    }
    switch (Type->NumberOfDocks) {
        case 0:
            return Center_Coord();

        case 1:
            coord = Center_Coord() + Type->DockingOffsets[0];
            return coord;

        default:
            {
                int index = FindLinkIndex((TechnoClass *)dock);
                if (index >= 0 && index < Type->NumberOfDocks) {
                    coord = Center_Coord() + Type->DockingOffsets[index];
                    return coord;
                }
            }
            return Center_Coord();
    }
}


// int BuildingClass::unknown_slot281(ObjectClass *object) (0x004442B0, slot 281)
// The distance in leptons from the object's center to its dock coordinate
// here (slot 41), through TibSun TPoint3D::Distance_To (shared/coord.hpp).

int BuildingClass::unknown_slot281(ObjectClass *object)
{
    return unknown_slot41((int)object).Distance_To(object->Center_Coord());
}


// CoordStruct BuildingClass::unknown_slot19(TechnoClass *techno) const (0x00444340, slot 19)
// Where a techno docks: slot 41 for helipads and repair bays, otherwise
// the center.

CoordStruct BuildingClass::unknown_slot19(TechnoClass *techno) const
{
    if (Type->Helipad) {
        return unknown_slot41((int)techno);
    }
    if (Type->UnitRepair) {
        return unknown_slot41((int)techno);
    }
    return Center_Coord();
}


// Can_Fire: occupant, power, recharge and turret-facing checks.

inline UnknownFacingDir Direction_Difference_004443b0(UnknownFacingDir const &current, UnknownFacingDir const &toward)
{
    return UnknownFacingDir((unsigned short)(current.Raw - toward.Raw));
}
inline bool Direction_Less_004443b0(UnknownFacingDir const &left, UnknownFacingDir const &right)
{
    return abs((short)left.Raw) < abs((short)right.Raw);
}

int BuildingClass::unknown_slot218(AbstractClass *target, int weapon, bool check_range)
{
    if (Type->CanBeOccupied && unknown_554.Count() == 0) return 5;
    if (Type->EMPulseCannon) return 6;
    if (!unknown_slot195()) return 6;
    if (unknown_5d8 != 0) return 3;
    int can_fire = TechnoClass::unknown_slot218(target, weapon, check_range);
    if (can_fire == 0 && unknown_slot232()) {
        UnknownFacingDir toward = unknown_slot285(target);
        int tolerance = Type->TurretAnimIsVoxel ? 0 : 8;
        if (Direction_Less_004443b0(UnknownFacingDir((unsigned short)(tolerance << 8)), Direction_Difference_004443b0(unknown_2e0.Current(), toward))) return 2;
    }
    return can_fire;
}


// bool BuildingClass::unknown_4444b0(bool take) 0x004444B0: makes this the primary factory of its
// factory type and naval group unless another building already is (that one gives way when take).
// Returns whether this building's primary state changed. Shape: YR twin FUN_00448070 (ra2-decomp).
bool BuildingClass::unknown_4444b0(bool take)
{
    int factory = Type->Factory;
    if (factory == 0) {
        return false;
    }
    bool was = unknown_32b;
    int count = House->Buildings.Count();
    BuildingClass *building;
    for (int index = 0; index < count; index++) {
        building = House->Buildings[index];
        if (!building->unknown_76 && building->unknown_32b) {
            if (building->Type->Factory == factory) {
                if ((Type->Naval && building->Type->Naval) || (!Type->Naval && !building->Type->Naval)) {
                    if (take) {
                        building->unknown_32b = false;
                        unknown_32b = true;
                    }
                    goto done;
                }
            }
        }
    }
    unknown_32b = true;
done:
    return unknown_32b != was;
}


// Toggle primary factory selection within the house's factory and naval group.
bool BuildingClass::unknown_slot287()
{
    if(Type->Factory) {
        if(unknown_32b) unknown_32b=false;
        else {
            int count=House->Buildings.Count();
            for(int index=0;index<count;++index) {
                BuildingClass *building=House->Buildings[index];
                if(!building->unknown_76 && building->Type->Factory==Type->Factory) {
                    if((Type->Naval && building->Type->Naval) || (!Type->Naval && !building->Type->Naval))
                        building->unknown_32b=false;
                }
            }
            unknown_32b=true;
            if(House->Is_Player_Control()) Speech_Play_Name("EVA_PrimaryBuildingSelected",-1,-1);
        }
        Mark(2);
    }
    return unknown_32b;
}


// bool BuildingClass::unknown_slot222(HouseClass *newowner, int announce) (0x00444660): TibSun
// BuildingClass::Captured (building.cpp:4212), independently bounded 00444660..0044537e. YR twin gamemd
// 0x00448260 (same contact-loop layout and out-of-line Set_Anim_Drawer/Set_Anim_Translucency calls).
// Original Reestablish vector slots, read after exact-target SHA256 verification.
inline UnknownVector3 Coord_Difference(CoordStruct const &,CoordStruct const &);
void __fastcall Building_Refund_Upgrades_00452710(BuildingClass *building);
void __fastcall Building_Reset_Fence_Progress_0044e570(BuildingClass *building);
extern bool unknown_flag_8339e8;
extern "C" char const capture_eva_building_007cf924[];
extern "C" char const capture_eva_tech_lost_007cf958[];
extern "C" char const capture_eva_tech_007cf93c[];
class Reestablish {
public:
    Reestablish(TechnoClass *contact, bool tethered) : Contact(contact), WasTethered(tethered) {}
    TechnoClass *Contact;
    bool WasTethered;
};
// The independently observed Reestablish base constructor is called at 0044499f.
// Keep this real member out of line; no inlined copy occurs in Building.CPP.
template<> VectorClass<Reestablish*>::VectorClass(unsigned size,Reestablish *const *array)
    : Vector(0),VectorMax(size),IsValid(true),IsAllocated(false)
{
    if(size) {
        if(array) Vector=new ((void*)array) Reestablish*[size];
        else {Vector=new Reestablish*[size];IsAllocated=true;}
    }
}
bool BuildingClass::unknown_slot222(HouseClass *newowner, int announce)
{
    if(newowner!=House) {
    if(unknown_38 && !Type->CanBeOccupied) { unknown_38->Unknown_00435FB0(); }
    if(Type->CloakGenerator) { newowner->unknown_55e8=true; }
    if(House->Type->MultiplayPassive && Type->ProduceCashStartup) {
        newowner->Refund_Money(Type->ProduceCashStartup);
        unknown_5a0=CDTimerClass<FrameTimerClass>(Type->ProduceCashDelay);
    }
    if(unknown_1a8 & (1<<newowner->Type->House)) {
        unknown_1a8-=1<<newowner->Type->House;
        if(Type->Radar) { unknown_44ab00(); }
    }
    unknown_32b=false;
    if(Type->LaserFencePost) { unknown_44dc40(0); }
    if(House->Is_Player_Control() || newowner->Is_Player_Control()) {
        if(!newowner->Type->MultiplayPassive && (char)announce) {
            if(Type->NeedsEngineer) {
                if(House!=newowner) {
                    if(House->Is_Player_Control()) { Speech_Play_Name(capture_eva_tech_lost_007cf958,-1,-1); }
                    if(newowner->Is_Player_Control()) { Speech_Play_Name(capture_eva_tech_007cf93c,-1,-1); }
                }
            } else {
                if(Unknown_0063B5B0(10,unknown_slot104())) { Speech_Play_Name(capture_eva_building_007cf924,-1,-1); }
            }
        }
        unknown_flag_8339e8=true;
        Map.Flag_To_Redraw(0);
    }
    unknown_75=true;
    unknown_59c=true;
    if(Type->NeedsEngineer) { unknown_44d600(); }
    if(unknown_game_mode_a3d298==4) { newowner->unknown_42a8.Unknown_0070C490(Type->ArrayIndex); }
    TechnoClass *passenger=(TechnoClass*)Cargo.Attached_Object();
    if(passenger) { passenger->unknown_slot222(newowner,1); }
    DynamicVectorClass<Reestablish*> reestablish;
    int i;
    // TibSun building.cpp:4212 Captured handles one Contact_With_Whom(); RA2 walks every radio link and
    // nulls a non-foot contact before the run-away message (the original pushes the nulled register).
    // TibSun's order: the tether flag is read and the contact captured inside the condition's arm, and
    // the radio-contact flag is tested after it (the run-away block precedes the capture arm, as in the
    // original and the YR twin 0x00448260).
    for(i=0;i<RadioLinks.Length();++i) {
        TechnoClass *contact=Contact(i);
        if(contact) {
            bool captured=false;
            bool tethered=false;
            if(!contact->AbstractFlags.IsFoot) contact=0;
            else if(unknown_slot150(19,contact)==1 && (Type->WeaponsFactory || Coord_Difference(contact->Center_Coord(),unknown_slot41((int)contact)).Length()<64)) {
                tethered=contact->unknown_370;
                contact->unknown_slot222(newowner,1);
                captured=true;
            }
            if(!captured) {
                unknown_slot150(23,contact);
                unknown_slot150(3,contact);
            } else {
                Reestablish *record=new Reestablish(contact,tethered);
                reestablish.Add(record);
            }
        }
    }
    if(unknown_41c) {
        ((FactoryClass*)unknown_41c)->Abandon_Production_004B97C0();
        delete unknown_41c;
        unknown_41c=0;
    }
    House->unknown_func_4eaba0(this);
    HouseClass *oldowner=House;
    TargetClass to_capture(this);
    unknown_5b3=true;
    if(Rules->unknown_7c8.Is_Present(Type)) { oldowner->unknown_50.Delete(this); }
    if(Type->UnitRepair) { oldowner->unknown_80.Delete(this); }
    if(Type->Cloning) { oldowner->unknown_98.Delete(this); }
    if(Type->PsychicDetectionRadius>0) { oldowner->unknown_b0.Delete(this); }
    oldowner->Buildings.Delete(this);
    if(Type->OrePurifier) { --oldowner->unknown_52cc; }
    if(Type->Helipad) { oldowner->unknown_218-=Type->NumberOfDocks; }
    if(Type->CloakGenerator) {
        Disable_Cloak_Generator_0044fad0();
        unknown_5bc=1;
        Cloaking_AI(1);
    }
    if(Type->GapGenerator) { unknown_slot236(); }
    unknown_slot263(false,false,false,0);
    TechnoClass::unknown_slot222(newowner,1);
    if(Type->CloakGenerator && unknown_slot195()) { Enable_Cloak_Generator_0044fa90(); }
    if(Type->GapGenerator && unknown_slot195()) { unknown_slot235(); }
    unknown_4444b0(false);
    oldowner->unknown_5410=this;
    oldowner->unknown_func_4e8640();
    House->unknown_func_4e8640();
    if(House->unknown_5410==this) { House->unknown_5410=0; }
    if(oldowner->Is_Player_Control() && Type->ConstructionYard) {
        Map.unknown_11a0=0;Map.unknown_11a4=0;Map.unknown_11a8=-1;
        Map.Unknown_00498F10(0);
    }
    House->unknown_func_4eac70(this);
    unknown_5b8=false;
    unknown_slot282(true);
    if(Type->LaserFencePost) {
        unknown_44dbe0();
        Building_Reset_Fence_Progress_0044e570(this);
    }
    if(House->Is_Player_Control()) {unknown_slot262(false,false,false,0);Map.Flag_To_Redraw(1);}
    if(unknown_1a8 & (1<<newowner->Type->House)) { unknown_1a8 &= ~(1<<newowner->Type->House); }
    Update_Anim_Appearance_0044d190();
    if(Rules->unknown_7c8.Is_Present(Type)) { newowner->unknown_50.Add(this); }
    if(Type->UnitRepair) { newowner->unknown_80.Add(this); }
    if(Type->Cloning) { newowner->unknown_98.Add(this); }
    if(Type->PsychicDetectionRadius>0) { newowner->unknown_b0.Add(this); }
    newowner->Buildings.Add(this);
    if(Type->OrePurifier) { ++House->unknown_52cc; }
    if(Type->Helipad) { House->unknown_218+=Type->NumberOfDocks; }
    if(Type->Factory) {oldowner->unknown_func_4f3170(Type->Factory,Type->Naval,0);newowner->unknown_func_4f3170(Type->Factory,Type->Naval,0);}
    for(i=0;i<reestablish.Count();++i) {
        Reestablish *record=reestablish[i];
        unknown_slot150(2,record->Contact);
        record=reestablish[i];
        if(record->WasTethered) {unknown_370=true;record=reestablish[i];record->Contact->unknown_370=true;}
    }
    if(unknown_game_mode_a3d298!=0 && !newowner->Is_Human_Player()) {
        Building_Refund_Upgrades_00452710(this);
    }
    oldowner->unknown_144=true;newowner->unknown_144=true;
    return true;
    }
    return false;
}


// int BuildingClass::Sort_Y() const (0x00445380, slot 45)
// The object value, raised by 32 for type flag +0x135F and lowered by 16
// for type flag +0x1351.

int BuildingClass::Sort_Y() const
{
    int value = ObjectClass::Sort_Y();
    return value + ((Type->TurretAnimIsVoxel ? 32 : 0) - (Type->Gate ? 16 : 0));
}


// int BuildingClass::Can_Enter_Cell(...) (0x004453B0; ret 0x14)
// Whether the building may enter (stand in) a cell: 0 when it may, 7 when it
// may not (hypothesis: Can_Enter_Cell's MOVE_OK / MOVE_NO). A deployed
// building (its type undeploys into a unit) on the map asks the cell
// whether the type's movement may enter; otherwise the type answers for its
// owner (BuildingTypeClass slot 42). arg1 is the cell (its coordinates at
// +0x24); the other arguments are unused.

// The cell argument's coordinates (a view of CellClass).
struct UnknownBuildingCellArg {
    char unknown_00[0x24];
    Cell MapCoords;                   // +0x24
};

int BuildingClass::Can_Enter_Cell(CellClass *arg1, int arg2, int arg3, CellClass *arg4, bool arg5)
{
    Cell cell = ((UnknownBuildingCellArg *)arg1)->MapCoords;
    if (Type->UndeploysInto != 0 && unknown_74) {
        return ((UnknownPlacementCellView &)Map[cell]).Unknown_004718B0(Type->SpeedType, Type, House) ? 0 : 7;
    }
    return Type->Legal_Placement(cell, House) ? 0 : 7;
}


// bool BuildingClass::Can_Demolish() (0x00445430, slot 37)
// False for an unsellable type; true while a buildup is shown and +0x42C is
// set outside missions 18/19, or for a firestorm wall whose owner lacks
// +0x142 (meaning hypotheses).

bool BuildingClass::Can_Demolish()
{
    if (!Type->Unsellable) {
        if (unknown_5b9 && unknown_42c && Get_Mission() != 19 && Get_Mission() != 18) {
            return true;
        }
        if (Type->FirestormWall && !House->unknown_142) {
            return true;
        }
    }
    return false;
}


// Clear occupants from the weapons factory exit and surrounding bib cells.
extern char const Factory_Exit_Message[];
extern char const Factory_Bib_Message[];
bool BuildingClass::unknown_4454b0()
{
    if(Type->WeaponsFactory) {
        Cell offset=Type->FoundationOutside[10];
        Cell cell=unknown_slot104()+offset;
        cell=cell+Cell(-1,0);
        Cell lookup=cell;
        CoordStruct coordinate=Cell_Center(lookup);
        CellClass &exit=Map[lookup];
        ObjectClass *occupant=exit.Cell_Techno(Point2D(0,0),false,this);
        if(occupant) {
            Debug_Printf(Factory_Exit_Message,(char const *)occupant->Techno_Type_Class()->ID);
            exit.Incoming(&unknown_geometry_l,true,true,false);
            for(int facing=0;facing<8;++facing) {
                CellClass &neighbor=exit.Adjacent_Cell(facing);
                occupant=neighbor.Cell_Techno(Point2D(0,0),false,this);
                if(occupant) {
                    Debug_Printf(Factory_Bib_Message,(char const *)occupant->Techno_Type_Class()->ID);
                    neighbor.Incoming(&coordinate,true,true,false);
                }
            }
            return true;
        }
    }
    return false;
}


// int BuildingClass::Do_MISSION_GUARD() (0x00445620)
// The building's guard mission (hypothesis: Mission_Guard); returns the
// delay until the next call. While slot 163 holds (marking unknown_5ad), a
// building with a usable super weapon (its auxiliary building owned) and
// an occupant or no occupant slots, and a +0x228 object, switches to
// mission 1 at once; otherwise it waits the mission's +0x18 rate. A stupid
// guard waits 100. Otherwise the first call (status 0) runs 0x00443D60 and
// moves to status 1; in status 1 a repair or reload bay starts mission
// 0x14 with the first contact in mission 7 within 64 leptons that accepts
// message 0x13, and a weapons factory runs 0x004454B0. A reload bay in
// radio contact then starts mission 0x14 when message 0x1D is refused and
// 0x13 accepted. The delay is the mission's +0x10 rate (three times that
// for buildings that are not repair bays) plus 0..2.

// The difference of two coordinates (an inline helper).
inline UnknownVector3 Coord_Difference(CoordStruct const &a, CoordStruct const &b)
{
    return UnknownVector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}

int BuildingClass::Do_MISSION_GUARD()
{
    if (unknown_slot163()) {
        unknown_5ad = true;
        if (!Type->EMPulseCannon) {
            int super = Type->SuperWeapon;
            if (super != -1) {
                BuildingTypeClass *aux = Supers[super]->Type->AuxBuilding;
                if (aux != 0 && House->unknown_5460.unknown_func_491d10(aux->ArrayIndex) == 0) {
                    goto check_occupants;
                }
            }
            if (Type->SuperWeapon == -1) {
check_occupants:
                if ((!Type->CanBeOccupied || unknown_554.Count() > 0) && unknown_228 != 0) {
                    QueueMission(1, false);
                    Commence();
                    return 1;
                }
            }
        }
        int delay = (int)(Control()->unknown_18 * 900.0);
        return delay + unknown_id_source_a3d290->Random()(0, 2);
    }
    if (Type->HasStupidGuardMode) {
        return 100;
    }
    switch (unknown_a8) {
    case 0:
        unknown_443d60(true);
        unknown_a8 = 1;
        break;
    case 1:
        if (Type->UnitRepair || Type->UnitReload) {
            for (int index = 0; index < RadioLinks.Length(); index++) {
                TechnoClass *techno = Contact(index);
                bool active;
                if (techno != 0) {
                    active = techno->AbstractFlags.IsTechno;
                }
                if (techno != 0 && active) {
                    if (techno->Get_Mission() == 7) {
                        UnknownVector3 distance = Coord_Difference(Center_Coord(), techno->Center_Coord());
                        if (distance.Length() < 0x40
                            && unknown_slot150(0x13, techno) == 1) {
                            QueueMission(0x14, false);
                            return 1;
                        }
                    }
                }
            }
        }
        if (Type->WeaponsFactory) {
            unknown_4454b0();
        }
        break;
    }
    if (Type->UnitReload && HasAnyLink() && unknown_slot149(0x1D) != 1 && unknown_slot149(0x13) == 1) {
        QueueMission(0x14, false);
    }
    if (Type->UnitRepair) {
        int delay = (int)(Control()->unknown_10 * 900.0);
        return delay + unknown_id_source_a3d290->Random()(0, 2);
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay * 3 + unknown_id_source_a3d290->Random()(0, 2);
}


// int BuildingClass::Do_MISSION_GUARD_AREA() (0x00445940, slot 130)
// Same as slot 129.

int BuildingClass::Do_MISSION_GUARD_AREA()
{
    return Do_MISSION_GUARD();
}


// Private Building mission139, full349+3; Rules+5E8 draft/compiler scope approved.
int BuildingClass::Do_MISSION_CONSTRUCTION()
{
    switch(unknown_a8) {
    case 0:
        unknown_443d60(0);
        unknown_slot149(11);
        {
            int sound=Type->BuildupSound;
            if(sound==-1)sound=Rules->Construction;
            if(sound!=-1)Voc_Play_At(sound,CoordStruct(Coord),&unknown_570);
        }
        unknown_75=true;
        unknown_a8=1;
        break;
    case 1:
        unknown_75=true;
        Voc_Update_At(CoordStruct(Coord),&unknown_570);
        if(unknown_5ad) {
            unknown_slot149(12);
            unknown_slot149(3);
            unknown_443d60(1);
            unknown_slot282(false);
            QueueMission(5,false);
            if(!Type->LaserFence) {
                UnknownFacingDir direction((unsigned short)((unsigned char)Type->unknown_c64<<8));
                unknown_2e0.Set(direction);
            }
            Type->Free_Buildup();
            unknown_570.Unknown_00406070();
            return 1;
        }
        break;
    }
    return 1;
}


// bool BuildingClass::unknown_445ab0() (0x00445AB0)
// False unless the type undeploys into a unit. True for a SensorArray,
// TickTank, ICBMLauncher or Artillary type; otherwise (only in a game mode)
// when unknown_1b0 is set and the owner's 0x004F52F0 holds, in single-player
// or with the flag at 0x00A3D340.

bool BuildingClass::unknown_445ab0()
{
    BuildingTypeClass *type = Type;
    if (!type->UndeploysInto) {
        return false;
    }
    if (type->SensorArray || type->TickTank || type->ICBMLauncher || type->Artillary) {
        return true;
    }
    if (!unknown_game_mode_a3d298) {
        return false;
    }
    if (unknown_1b0 && House->Unknown_004F52F0() && (!unknown_game_mode_a3d298 || unknown_flag_a3d340)) {
        return true;
    }
    return false;
}


// Building selling and undeployment state machine, original00445B30 body3308+4. The crew exit cell sums the
// random foundation entry in place (from TibSun building.cpp Mission_Deconstruction).
// The repair-pad test Distance_To(Contact(0)) is TibSun object.h's ObjectClass::Distance_To chain over the TibSun
// TPoint3D operations (shared/object.hpp, shared/coord.hpp; RA2_POINT3D_TIBSUN): the TPoint3D ctor and Length stay
// out of line (0x0041B630, 0x0041B780) as in the original.
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house);
extern "C" char const deconstruction_eva_unit_sold_007cf9c4[];
extern "C" char const deconstruction_eva_structure_sold_007cf9d4[];
// Snap to the cell center with the original signed 32-bit division.
inline CoordStruct Snap_To_Cell_Center(CoordStruct const &coord)
{
    return CoordStruct(coord.X/256*256+128,coord.Y/256*256+128,coord.Z);
}
int BuildingClass::Do_MISSION_DECONSTRUCTION()
{
    Repair(0);
    bool selected=unknown_77;
    switch(unknown_a8) {
    case 0:
        if(unknown_445ab0() && (Type->Artillary || Type->TickTank)) {
            unknown_slot220(0);
        }
        if(Type->UnitRepair && unknown_slot149(19)==1 &&
           Distance_To(Contact(0))<128) {
            TechnoClass *contact=Contact(0);
            unknown_slot149(3);
            if(unknown_372) Speech_Play_Name(deconstruction_eva_unit_sold_007cf9c4,-1,-1);
            contact->Sell_Back(1);
            QueueMission(5,false);
            return 1;
        }
        if(unknown_5c6) {
            TechnoClass *upgrade=(TechnoClass*)unknown_4c0[unknown_5c6-1]->Create_One_Of(House);
            House->Refund_Money(upgrade->unknown_slot167());
            delete upgrade;
            unknown_44c860();
            House->unknown_5668=true;
            House->unknown_5669=true;
            Building_Refresh_House_Buildings_0044fa40(House);
            if(unknown_372) Speech_Play_Name(deconstruction_eva_structure_sold_007cf9d4,-1,-1);
            QueueMission(5,false);
            return 1;
        }
        unknown_5ad=false;
        SendToEachLink(23);
        if(Type->LaserFencePost) unknown_44e5a0(0);
        unknown_4397c0();
        unknown_a8=1;
        break;
    case 1:
        SendToEachLink(3);
        if(!unknown_370) {
            if(!unknown_1b0 || !Type->UndeploysInto) {
                int count=unknown_slot172();
                bool engineer=false;
                Cell const *foundation=Occupy_List(0);
                Cell const *end=foundation;
                int cells=0;
                while(end->X!=32767 || end->Y!=32767) {++cells;++end;}
                while(count) {
                    InfantryTypeClass *crew=unknown_slot184();
                    while(crew->Engineer && engineer) crew=unknown_slot184();
                    if(crew->Engineer) engineer=true;
                    InfantryClass *infantry=0;
                    if(crew) infantry=new InfantryClass(crew,House);
                    if(infantry) {
                        ++unknown_a40774;
                        Cell cell=unknown_slot104()+foundation[unknown_id_source_a3d290->RandomNumber(0,cells-1)];
                        CoordStruct position=CoordStruct(cell)+CoordStruct(0,36,0);
                        position=Map[position].Closest_Free_Spot(position,false,false);
                        if(position==unknown_geometry_l || !infantry->Unlimbo(&position,0)) {
                            delete infantry;
                        } else {
                            if(infantry->Type->Nominal) infantry->unknown_5c1=true;
                            --unknown_a40774;
                            infantry->Scatter(unknown_geometry_l,true,false);
                            ++unknown_a40774;
                            infantry->QueueMission(2,false);
                        }
                        --unknown_a40774;
                    }
                    --count;
                }
            }
            if(House->Is_Player_Control() && !unknown_slot31()) {
                Voc_Play_At(Rules->unknown_5c4,Position_Coord_004f3f40(),0);
                if(Type->PackupSound!=-1) Voc_Play_At(Type->PackupSound,Position_Coord_004f3f40(),&unknown_570);
            }
            unknown_a8=2;
            unknown_443d60(0);
            unknown_5ad=false;
            if(selected) unknown_slot80();
        }
        break;
    case 2:
        Voc_Update_At(Position_Coord_004f3f40(),&unknown_570);
        if(unknown_5ad) {
            House->unknown_144=true;
            unknown_slot220(0);
            if(unknown_372 && !Type->UndeploysInto) Speech_Play_Name(deconstruction_eva_structure_sold_007cf9d4,-1,-1);
            if(unknown_445ab0()) {
                if(Type->Artillary &&
                   (unknown_2c8.Current()!=UnknownFacingDir((Dir256)Type->BarrelStartPitch) ||
                    unknown_2e0.Current()!=UnknownFacingDir((Dir256)Type->unknown_c64))) {
                    unknown_2c8.Set_Desired(UnknownFacingDir((Dir256)Type->BarrelStartPitch));
                    unknown_2e0.Set_Desired(UnknownFacingDir((Dir256)Type->unknown_c64));
                    return 1;
                }
                ++unknown_a40774;
                UnitClass *unit=new UnitClass(Type->UndeploysInto,House);
                --unknown_a40774;
                if(unit) {
                    double ratio=Health_Ratio();
                    int refund=unknown_slot167();
                    AbstractClass *archive=unknown_1b0;
                    CoordStruct place;
                    if(Type->SensorArray || Type->TickTank || Type->ICBMLauncher || Type->Artillary) {
                        place=Position_Coord_004f3f40();
                    } else {
                        CoordStruct position=Position_Coord_004f3f40();
                        CoordStruct adjacent(position.X+unknown_facing_offsets_851ac0[3].X,
                                             position.Y+unknown_facing_offsets_851ac0[3].Y,position.Z);
                        place=Snap_To_Cell_Center(adjacent);
                    }
                    if(unknown_4e8) unknown_4e8->Deactivate(0);
                    DynamicVectorClass<TechnoClass*> targetters;
                    for(int index=0;index<Technos.Count();++index) {
                        TechnoClass *techno=Technos[index];
                        if(techno->unknown_228 && techno->unknown_228->Kind_Of()==6 &&
                           techno->unknown_228==this && techno->unknown_7d && techno!=this && techno!=unit) {
                            targetters.Add(techno);
                        }
                    }
                    Limbo();
                    int deploy_facing=Type->unknown_func_45ec90();
                    if(unit->Unlimbo(&place,deploy_facing)) {
                        unit->Strength=(int)(unit->Class_Of()->MaxStrength*ratio);
                        unit->Strength=unit->Strength>1 ? unit->Strength : 1;
                        unit->unknown_70=unit->Strength;
                        unit->unknown_114=unknown_114;
                        unit->unknown_1ac=unknown_1ac;
                        unit->unknown_118=unknown_118;
                        if(Type->Artillary) unit->unknown_2c8.Set(UnknownFacingDir((Dir256)Type->BarrelStartPitch));
                        if(archive) {
                            unit->Assign_Destination(archive,true);
                            unit->QueueMission(2,false);
                        }
                        if(unknown_34) {
                            unit->Attach_Tag(unknown_34);
                            --unknown_34->AttachCount;
                            unknown_34=0;
                        }
                        if(selected) unit->unknown_slot80();
                        for(int index=0;index<targetters.Count();++index) targetters[index]->unknown_slot220(unit);
                    } else {
                        House->Refund_Money(refund);
                    }
                } else {
                    House->Refund_Money(unknown_slot167());
                }
                unknown_slot54(true);
                unknown_570.Unknown_00406070();
                Delete_Me();
                Building_Refresh_House_Buildings_0044fa40(House);
            } else {
                unknown_434=-1;
                Record_The_Kill(0);
                if(unknown_4e8) unknown_4e8->Deactivate(0);
                House->Refund_Money(unknown_slot167());
                Limbo();
                StorageClass *storage=&unknown_294;
                int slot=storage->First_Used_Slot();
                while(slot!=-1) {
                    int amount=(int)storage->Remove_Amount(storage->Get_Amount(slot),slot);
                    House->Harvested((float)amount,slot);
                    slot=storage->First_Used_Slot();
                }
                Delete_Me();
                Building_Refresh_House_Buildings_0044fa40(House);
                if(Type->CloakGenerator) {
                    unknown_5bb=-1;
                    if(!unknown_5bc) unknown_5bc=Type->CloakRadiusInCells;
                    unknown_75=true;
                    unknown_5bc=1;
                    Cloaking_AI(1);
                }
            }
        }
        break;
    }
    return 1;
}


// Full Building 0x00446820: 246-byte body, 10-byte original padding.
// Complete private compiler candidate; current canonical constructor and timer.
// Lead-approved private field proposal: original DWORD store at laser+0x18.

typedef char LaserDraw_size_must_be_0x58[(sizeof(LaserDrawClass)==0x58)?1:-1];
typedef char LaserDraw_field18_offset[((unsigned int)&((LaserDrawClass*)0)->unknown_18==0x18)?1:-1];
typedef char LaserDraw_housecolor_offset[((unsigned int)&((LaserDrawClass*)0)->IsHouseColor==0x1C)?1:-1];
void BuildingClass::unknown_446820(CoordStruct destination)
{
    RGBClass black;
    LaserDrawClass *laser = new LaserDrawClass(
        unknown_slot43(0), destination, 0, true,
        House->unknown_55ec, black, black,
        Rules->PrismSupportDuration, false, true, 1.0f, 0.0f);
    if (laser) {
        laser->IsHouseColor = true;
        laser->unknown_18 = 3;
    }
    int delay = Rules->PrismSupportDelay;
    unknown_244 = CDTimerClass<FrameTimerClass>(delay);
    unknown_534 = 0;
}


// Forward this mission handler to the evidenced MissionClass base implementation.
int BuildingClass::Do_MISSION_CAPTURE() { return MissionClass::Do_MISSION_CAPTURE(); }


// Forward this mission handler to the evidenced MissionClass base implementation.
int BuildingClass::Do_MISSION_HARVEST() { return MissionClass::Do_MISSION_HARVEST(); }


// Building repair mission: construction-yard, hospital, armory and unit service states.
// Original 00447260: 4503-byte complete body and 9 bytes of frozen padding.
// Repair states: 0 initializes service, 1 waits for the client, 2 performs service.
// The first contact is read through Contact_With_Whom() throughout (TibSun Mission_Repair shape).
extern "C" GUID const CLSID_HoverLocomotionClass;
extern "C" GUID const CLSID_ShipLocomotionClass;
// Existing 0051FC80 owner-condition ABI; its nominal receiver type remains unknown.
class Leaf_51fc80 { public: bool get(); };
// The repeated archive-position query and signed conversion to cell units.
inline Cell Building_Repair_Archive_Cell_00447260(AbstractClass const &target)
{
    return Cell(target.Center_Coord());
}

int BuildingClass::Do_MISSION_REPAIR()
{
    if (Type->ConstructionYard) {
        switch (unknown_a8) {
        case 0: {
                unknown_443d60(2);
                unknown_44c920(7, Health_Ratio() <= Rules->unknown_1368, false, 0);
                unknown_a8 = 2;

                break;
            }
        case 2: {
                if (!HasAnyLink()) {
                    QueueMission(5, false);
                    unknown_44cfd0(7);

                }

                break;
            }
        }
        return 1;
    }
    if (Type->Hospital) {
        switch (unknown_a8) {
        case 0: {
                unknown_a8 = 2;
                unknown_5ad = false;
                unknown_4f4.Set_Stage(0);
                unknown_4f4.Set_Rate(1);
                if (Type->Ammo != -1) {
                    --unknown_254;
                    unknown_254 = unknown_254 > 0 ? unknown_254 : 0;
                }

                break;
            }
        case 2: {
                unknown_4f4.Graphic_Logic();
                if (unknown_4f4.Fetch_Stage() >= Rules->IRepairRate * 900.0) {
                    unknown_5ad = false;
                    unknown_4f4.Set_Stage(0);
                    switch (unknown_slot149(28)) {
                    case 33:
                        if (House->Is_Player_Control()) {
                            if (Unknown_0063B5B0(8, unknown_slot104())) Speech_Play_Name("EVA_UnitRepaired", -1, -1);
                            Voc_Play_At(Rules->HealCrateSound, CoordStruct(Coord), 0);
                        }
                        // Both completed treatment and an empty reply release the patient.
                    case 10:
                        Exit_Object((TechnoClass *)Cargo.Detach_Object());
                        QueueMission(5, false);
                        break;
                    case 32:
                        Exit_Object((TechnoClass *)Cargo.Detach_Object());
                        break;
                    }
                }
                return 1;

            }
        }
        return (int)(Control()->unknown_10 * 900.0);
    }
    if (Type->Armory) {
        switch (unknown_a8) {
        case 0: {
                unknown_a8 = 2;
                unknown_5ad = false;
                unknown_4f4.Set_Stage(0);
                unknown_4f4.Set_Rate(1);
                --unknown_254;
                unknown_254 = unknown_254 > 0 ? unknown_254 : 0;

                break;
            }
        case 2: {
                unknown_4f4.Graphic_Logic();
                if (unknown_4f4.Fetch_Stage() >= Rules->IRepairRate * 900.0) {
                    TechnoClass *contact = Contact_With_Whom();
                    if (contact->unknown_118.Is_Dumbass()) contact->unknown_118.Set_Veteran(true);
                    else contact->unknown_118.Set_Elite(true);
                    Exit_Object((TechnoClass *)Cargo.Detach_Object());
                    QueueMission(5, false);
                }

                break;
            }
        }
        return (int)(Control()->unknown_10 * 900.0);
    }
    if (Type->UnitRepair) {

        switch (unknown_a8) {
        case 0: {
                if (!HasAnyLink()) {
                    unknown_44cfd0(8);
                    unknown_44cfd0(11);
                    unknown_44c920(12, Health_Ratio() <= Rules->unknown_1368, false, 0);
                    unknown_44c920(3, Health_Ratio() <= Rules->unknown_1368, false, 0);
                    QueueMission(5, true);
                    return 1;
                }
                unknown_5ad = false;
                int distance = 100;
                FootClass *contact = (FootClass *)Contact_With_Whom();
                UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(contact->Locomotor);
                GUID clsid;
                persist.Checked()->GetClassID(&clsid);
                if (clsid == CLSID_HoverLocomotionClass) distance = 200;
                if (clsid == CLSID_ShipLocomotionClass) distance = 200;
                if (unknown_slot149(19) == 1 && unknown_slot281(Contact_With_Whom()) < distance) {
                    unknown_a8 = 1;
                    return 3;
                }
                if (!reinterpret_cast<Leaf_51fc80 *>(House)->get()) contact->Locomotor.Checked()->Power_On();

                break;
            }
        case 2: {
                if (!HasAnyLink()) {
                    unknown_44cfd0(8);
                    unknown_44cfd0(11);
                    unknown_44c920(12, Health_Ratio() <= Rules->unknown_1368, false, 0);
                    unknown_44c920(3, Health_Ratio() <= Rules->unknown_1368, false, 0);
                    unknown_a8 = 1;
                    return 1;
                }
                if (unknown_4f4.unknown_10 == 0) unknown_4f4.unknown_10 = 1;
                unknown_4f4.Graphic_Logic();
                if (unknown_4f4.Fetch_Stage() >= Rules->URepairRate * 900.0 && unknown_slot149(19) == 1) {
                    unknown_5ad = false;
                    unknown_4f4.Set_Stage(0);
                    switch (unknown_slot149(28)) {
                    case 1:
                        break;
                    case 32: {
                            if (unknown_372 && House->Available_Money() == 0) Speech_Play_Name("EVA_InsufficientFunds", -1, -1);
                            unknown_44cfd0(8);
                            unknown_44cfd0(11);
                            unknown_44c920(12, Health_Ratio() <= Rules->unknown_1368, false, 0);
                            unknown_44c920(3, Health_Ratio() <= Rules->unknown_1368, false, 0);
                            unknown_a8 = 1;

                            break;
                        }
                    default: {
                            if (unknown_372 && Unknown_0063B5B0(8, unknown_slot104())) Speech_Play_Name("EVA_UnitRepaired", -1, -1);
                            unknown_44cfd0(8);
                            unknown_44cfd0(11);
                            unknown_44c920(12, Health_Ratio() <= Rules->unknown_1368, false, 0);
                            unknown_44c920(3, Health_Ratio() <= Rules->unknown_1368, false, 0);
                            unknown_a8 = 1;
                            FootClass *mover = Building_Radio_Foot_004397f0(Contact_With_Whom());
                            if (mover->unknown_1b0 && !mover->House->Is_Human_Player()) {
                                mover->QueueMission(2, false);
                                mover->Assign_Destination(mover->unknown_1b0, true);
                                mover->unknown_6d6e50(0);
                                unknown_slot149(3);
                                mover->unknown_3f8 = 0;
                                return 1;
                            }
                            Cell exit = unknown_slot280(Contact_With_Whom());
                            if (unknown_1b0) exit = Building_Repair_Archive_Cell_00447260(*unknown_1b0);
                            if (exit != *(Cell const *)&unknown_84ed10) {
                                mover->QueueMission(2, false);
                                mover->Assign_Destination(&Map[exit], true);
                                unknown_slot149(3);
                                mover->unknown_3f8 = 0;
                                return 1;
                            }

                            break;
                        }
                    }
                }
                return 1;

            }
        case 1: {
                if (!HasAnyLink()) {
                    if (unknown_454[8] || unknown_454[11]) {
                        unknown_44c920(12, Health_Ratio() <= Rules->unknown_1368, false, 0);
                        unknown_44c920(3, Health_Ratio() <= Rules->unknown_1368, false, 0);
                        unknown_44cfd0(8);
                        unknown_44cfd0(11);
                    }
                    if (!unknown_454[12]) {
                        unknown_5ad = true;
                        QueueMission(5, true);
                    }
                    return 1;
                }
                if (unknown_454[8]) return 1;
                FootClass *contact = (FootClass *)Contact_With_Whom();
                if (unknown_slot281(Contact_With_Whom()) < 200) {
                    if (contact->Locomotor.Checked()->Is_Powered()) {
                        if (!contact->Locomotor.Checked()->Is_Moving()) contact->Locomotor.Checked()->Power_Off();
                        return 1;
                    }
                    if (!contact->Locomotor.Checked()->Is_Powered() && ((FootClass *)Contact_With_Whom())->unknown_494) {
                        ((FootClass *)Contact_With_Whom())->unknown_4cd9b0();
                    }
                }
                if (unknown_slot149(19) == 1) {
                    TechnoClass *client = Contact_With_Whom();
                    bool damaged = client->Health_Ratio() < Rules->unknown_1360;
                    bool manual_reload = client->Techno_Type_Class()->ManualReload;
                    int reply = unknown_slot149(28);
                    bool roger = reply == 1;
                    bool all_done = reply == 33;
                    if ((!damaged && !manual_reload) || (!roger && !all_done)) {
                        if (client->Health_Ratio() == Rules->unknown_1360 && !((FootClass *)client)->Locomotor.Checked()->Is_Powered()) {
                            FootClass *mover = Building_Radio_Foot_004397f0(Contact_With_Whom());
                            mover->Locomotor.Checked()->Power_On();
                            if (mover->unknown_1b0 && !mover->House->Is_Human_Player()) {
                                mover->QueueMission(2, false);
                                mover->Assign_Destination(mover->unknown_1b0, true);
                                mover->unknown_6d6e50(0);
                                mover->unknown_3f8 = 0;
                                unknown_slot149(3);
                            } else {
                                Cell exit = unknown_slot280(Contact_With_Whom());
                                if (unknown_1b0) exit = Building_Repair_Archive_Cell_00447260(*unknown_1b0);
                                if (exit != *(Cell const *)&unknown_84ed10) {
                                    mover->QueueMission(2, false);
                                    mover->Assign_Destination(&Map[exit], true);
                                    mover->unknown_6d6e50(0);
                                    unknown_slot149(3);
                                    mover->unknown_3f8 = 0;
                                }
                            }
                        }
                    } else if (!client->unknown_328 || client->House->Is_Human_Player()) {
                        if (unknown_372) Speech_Play_Name("EVA_Repairing", -1, -1);
                        unknown_a8 = 2;
                        unknown_44c920(10, Health_Ratio() <= Rules->unknown_1368, false, 0);
                        unknown_44cfd0(3);
                        unknown_44cfd0(17);
                        unknown_5ad = false;
                        unknown_4f4.Set_Stage(0);
                        unknown_4f4.Set_Rate(1);
                    } else {
                        client->Sell_Back(1);
                        unknown_a8 = 0;
                        unknown_5ad = true;
                    }
                } else if (!reinterpret_cast<Leaf_51fc80 *>(House)->get()) {
                    if (!((FootClass *)Contact_With_Whom())->Locomotor.Checked()->Is_Powered()) ((FootClass *)Contact_With_Whom())->Locomotor.Checked()->Power_On();
                }

                break;
            }
        }
    } else if (Type->UnitReload) {
        bool servicing = false;
        for (int index=0; index<RadioLinks.Length(); ++index) {
            TechnoClass *contact = Contact(index);
            if (contact) {
                if (unknown_slot150(29, contact) == 1 && contact->Strength == contact->Techno_Type_Class()->MaxStrength) goto release_contact;
                if (contact->Get_Mission() != 7 && unknown_slot150(19, contact) == 1) {
                    servicing = true;
                    if (contact->Get_Mission() != 0) {
                        contact->QueueMission(0, false);
                    } else {
                        if (unknown_slot150(31, contact) != 1 && unknown_slot150(28, contact) != 1) {
                        release_contact:
                            contact->unknown_slot261(false, true);
                            contact->Set_Mission(5);
                            contact->unknown_slot194();
                        }
                    }
                }
            }
        }
        if (servicing) return (int)(Rules->ReloadRate * 900.0);
        QueueMission(5, false);
        return 3;
    } else return 15;
    return (int)(Control()->unknown_10 * 900.0);
}


// bool BuildingClass::Revealed(HouseClass *house) (0x00448FE0)
// The techno's slot 96 first (false stops here). Outside the special mode
// (unknown_a40774 clear) the owner records this type's index and its flag
// at +0x18D; the owner's +0x144 is set. A building not in radio contact then
// runs slot 282(false) when its owner passes 0x004F52F0 and neither its
// slot 91 nor its queued mission is 0x12, or when the owner fails it, the
// house argument is the owner and slot 91 is not 0x12.

bool BuildingClass::Revealed(HouseClass *house)
{
    if (TechnoClass::Revealed(house)) {
        if (unknown_a40774 == 0) {
            House->unknown_1b4 = Type->ArrayIndex;
            House->unknown_18d = true;
        }
        House->unknown_144 = true;
        if (!HasAnyLink() && House->Unknown_004F52F0() && Get_Mission() != 0x12 && MissionQueue != 0x12) {
            unknown_slot282(false);
        } else if (!HasAnyLink() && !House->Unknown_004F52F0() && house == House && Get_Mission() != 0x12) {
            unknown_slot282(false);
        }
        return true;
    }
    return false;
}


// Set the gate animation state and mission; the second stack argument is unused.
bool BuildingClass::unknown_slot261(bool opening, bool arg2)
{
    int mission;
    if(opening && !unknown_a40774 && !Debug_Map) {
        unknown_443d60(false);
        mission=18;
    } else {
        unknown_443d60(true);
        mission=5;
    }
    QueueMission(mission,false);
    return false;
}


// int BuildingClass::unknown_slot166() (0x00449110, slot 166)
// For pip scale 2: 0 unless the type is a weeder, else the type's pips
// (slot 29) scaled by the owner's ratio (0x004E5240); otherwise the techno
// value.

int BuildingClass::unknown_slot166()
{
    BuildingTypeClass *type = Type;
    switch (type->PipScale) {
    case 2:
        if (type->Weeder) {
            double ratio = House->unknown_func_4e5240();
            return type->Max_Pips() * ratio;
        }
        return 0;
    default:
        return TechnoClass::unknown_slot166();
    }
}


// void BuildingClass::unknown_slot216(int arg1) (0x00449170, slot 216)
// For a non-zero argument and a house that is the player's
// (HouseClass 0x004F52B0), remembers the building's cell in the radar
// event ring (0x0063C6C0).

void __fastcall Unknown_0063C6C0(Cell const &cell);       // 0x0063C6C0 (RadarEventClass: remembers a cell)

void BuildingClass::unknown_slot216(int arg1)
{
    if (arg1 && House->Is_Player_Control()) {
        Unknown_0063C6C0(Coord_Cell(unknown_slot19(0)));
    }
}


// Round the current facing to an image frame direction, or face the target.
UnknownFacingDir BuildingClass::unknown_slot183()
{
    if(!unknown_228)
        return (unsigned short)(Dir_To_256(unknown_2e0.Current())<<8);
    if(unknown_slot232() && !Type->TurretAnimIsVoxel)
        return (unsigned short)(Dir_To_32(unknown_2e0.Current())*2048);
    return unknown_slot285(unknown_228);
}


// Original-only bounds: 00449290, 1692 body bytes, 4 NOP padding bytes.
// Mission state and interface names are evidence-based source hypotheses.
extern "C" GUID const CLSID_TunnelLocomotionClass;
extern "C" GUID const CLSID_TeleportLocomotionClass;
extern "C" GUID const CLSID_DriveLocomotionClass;
extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;
template <> void UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600>::_AddRef() const;
template <> UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600> &
UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600>::operator=(UnknownComPtr const &other)
{
    if(Pointer!=other.Pointer) {
        ILocomotion *old=Pointer;
        Pointer=other.Pointer;
        _AddRef();
        if(old) old->Release();
    }
    return *this;
}

int BuildingClass::Do_MISSION_UNLOAD()
{
    if(unknown_554.Count()>0) unknown_452f50(0);
    if(Type->WeaponsFactory) {
        CoordStruct destination(0,0,0);
        if(!Type->Naval) {
            Cell exit=Type->FoundationOutside[10];
            Cell position=unknown_slot104()+exit;
            destination=CoordStruct(Cell(position.X-1,position.Y));
        }
        switch(unknown_a8) {
        case 0: {
            TechnoClass *unit=Contact(0);
            if(unit) { unit->QueueMission(5,false); unit->Commence(); }
            unknown_2a8.Count_Up(Type->DeployTime);
            unknown_a8=Type->Naval ? 4 : 1;
            unknown_75=true;
            unknown_44cfd0(17);
            unknown_44c920(8,Health_Ratio()<=Rules->unknown_1368,false,0);
            break;
        }
        case 1:
            if(!unknown_4454b0()) {
                Debug_Printf("Weapons factory bib clear - kicking out unit\n");
                unknown_a8=2;
            }
            break;
        case 2:
            if(unknown_2a8.Is_Count_Up_Complete()) {
                FootClass *unit=static_cast<FootClass *>(Contact(0));
                if(unit) {
                    unit->QueueMission(2,false);
                    UnknownComPtr<IPersist,&unknown_uuid_IPersist_7cf208> persist(unit->Locomotor);
                    GUID classid;
                    persist.Checked()->GetClassID(&classid);
                    if(classid==CLSID_TunnelLocomotionClass || classid==CLSID_TeleportLocomotionClass) {
                        UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> piggy(unit->Locomotor);
                        if(piggy && piggy.Checked()->Is_Piggybacking()) {
                            unit->Locomotor.Attach(0);
                            piggy.Checked()->End_Piggyback(&unit->Locomotor.Pointer);
                        }
                        UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600> drive(CLSID_DriveLocomotionClass);
                        drive.Checked()->Link_To_Object(unit);
                        piggy=UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30>(drive);
                        if(piggy) {
                            piggy.Checked()->Begin_Piggyback(unit->Locomotor);
                            unit->Locomotor=drive;
                            unit->Locomotor.Checked()->Force_Track(66,destination);
                        } else {
                            int damage=unit->Strength;
                            unit->Take_Damage(&damage,0,Rules->unknown_c74,0,true,0);
                        }
                    } else if(!(classid==CLSID_DriveLocomotionClass)) {
                        unit->Assign_Destination(&Map[unknown_slot104()+Cell(5,1)],true);
                    } else {
                        unit->Locomotor.Checked()->Force_Track(66,destination);
                    }
                    unit->unknown_slot306(0.5);
                    unknown_a8=3;
                } else {
                    unknown_2a8.Count_Down(Type->DeployTime);
                    unknown_a8=4;
                }
            }
            break;
        case 3:
            if(!unknown_370) { unknown_2a8.Count_Down(Type->DeployTime); unknown_a8=4; }
            break;
        case 4:
            if(unknown_2a8.Is_Count_Down_Complete() || Type->Naval) { unknown_slot261(false,true); unknown_75=true; }
            break;
        }
        int delay=Control()->Normal_Delay();
        return delay+unknown_id_source_a3d290->Random()(0,2);
    }
    if(Type->GapGenerator && Type->SuperGapRadiusInCells) {
        if(!unknown_1fc) {
            unknown_slot236();
            unknown_200=Type->SuperGapRadiusInCells;
            if(unknown_slot195()) {
                unknown_slot235();
                unknown_1fc=true;
                unknown_539=true;
                House->unknown_5668=true;
            } else {
                unknown_200=Type->GapRadiusInCells;
                unknown_slot235();
            }
        } else {
            unknown_slot236();
            unknown_1fc=false;
            unknown_539=false;
            unknown_200=Type->GapRadiusInCells;
            unknown_slot235();
            House->unknown_5668=true;
        }
    }
    QueueMission(5,false);
    return 1;
}


// bool BuildingClass::unknown_449930() (0x00449930)
// Whether an object other than the building stands in any cell of its
// foundation (slot 64's offsets from the slot 104 cell).

static inline bool Is_List_End(Cell const *offset)
{
    return offset->X == 0x7FFF && offset->Y == 0x7FFF;
}

// Sole original direct caller in this unit: 00449BC7.
#pragma auto_inline(off)
bool BuildingClass::unknown_449930()
{
    Cell cell = unknown_slot104();
    Cell const *offset = Occupy_List(0);
    bool found = false;
    while (!Is_List_End(offset)) {
        Cell position = *offset + cell;
        offset++;
        ObjectClass *object = Map[position].OccupierPtr;
        while (object) {
            if (this != object) {
                found = true;
                break;
            }
            object = object->unknown_30;
        }
    }
    return found;
}

#pragma auto_inline(on)


// 004499D0: gate mission state machine.
int BuildingClass::Do_MISSION_OPEN()
{
    CoordStruct closing_position;
    if (Type->Gate) {
        switch (unknown_a8) {
        case 0:
            if (unknown_2a8.Is_Count_Up_Complete()) {
                GateTimer = (int)(Type->GateCloseDelay * 900.0);
                unknown_a8 = 2;
            } else {
                if (!unknown_2a8.Is_Count_Up_Active()) {
                    if (unknown_2a8.Is_Count_Down_Active()) {
                        unknown_2a8.Reverse();
                    } else {
                        unknown_2a8.Count_Up(Type->DeployTime);
                        CoordStruct position = Coord;
                        Voc_Play_At(Rules->GateDown, position, 0);
                        unknown_tactical_83996c->Register_Dirty_Area(Get_Render_Rect(), 0);
                    }
                }
                unknown_a8 = 1;
                GateTimer = ProgressTimerClass<FrameTimerClass>((int)(Type->GateCloseDelay * 900.0));
                return 0;
            }
            break;
        case 3:
            unknown_2a8.Count_Down(Type->DeployTime);
            unknown_a8 = 4;
            closing_position = Coord;
            Voc_Play_At(Rules->GateUp, closing_position, 0);
            return 0;
        case 2:
            if (!unknown_449930()) {
                if (GateTimer.Progress() == 1.0) unknown_a8 = 3;
            } else {
                GateTimer = ProgressTimerClass<FrameTimerClass>((int)(Type->GateCloseDelay * 900.0));
            }
            break;
        case 1:
            if (unknown_2a8.Is_Count_Up_Complete()) unknown_a8 = 2;
            // Fall through to the animation refresh.
        case 4:
            if (unknown_2a8.Is_Count_Down_Complete()) {
                unknown_slot261(false, true);
                unknown_a8 = 5;
                unknown_75 = true;
            }
            if (unknown_2a8.Is_Count_Up_Active() || unknown_2a8.Is_Count_Down_Active() || unknown_2a8.Is_Count_Up_Complete()) {
                int stage = (int)(unknown_2a8.Percent_Complete() * Type->GateStages);
                if (stage != unknown_5c7) {
                    unknown_5c7 = (char)stage;
                    unknown_75 = true;
                }
            }
            return 0;
        }
        int delay = (int)(Control()->unknown_10 * 900.0);
        return delay + unknown_id_source_a3d290->Random()(0, 2);
    }
    QueueMission(5, false);
    return 1;
}


// int BuildingClass::unknown_449d40() (0x00449D40)
// The power the building supplies: the type's Power, plus its ExtraPower
// when unknown_538 is set, plus the Power of each of the three upgrades in
// unknown_4c0 when unknown_5c6 is not zero; scaled by the health ratio when
// positive and unknown_530 is set. 0 when slot 111 holds.

int BuildingClass::unknown_449d40()
{
    int power = Type->Power;
    if (unknown_slot111()) {
        return 0;
    }
    if (unknown_538) {
        power += Type->ExtraPower;
    }
    if (unknown_5c6) {
        for (int index = 0; index < 3; index++) {
            if (unknown_4c0[index]) {
                power += ((BuildingTypeClass *)unknown_4c0[index])->Power;
            }
        }
    }
    if (power > 0 && unknown_530) {
        return (int)(Health_Ratio() * power);
    }
    return 0;
}


// int BuildingClass::unknown_449dd0() (0x00449DD0)
// 0 when slot 111 holds or unknown_530 is clear. Otherwise the type's
// +0xC6C, plus its +0xC74 when unknown_539 is set, plus the +0xC6C of each
// of the three upgrades in unknown_4c0 when unknown_5c6 is not zero.

int BuildingClass::unknown_449dd0()
{
    if (unknown_slot111() || !unknown_530) {
        return 0;
    }
    int total = Type->PowerDrain_c6c;
    if (unknown_539) {
        total += Type->ExtraPowerDrain_c74;
    }
    if (unknown_5c6) {
        for (int index = 0; index < 3; index++) {
            if (unknown_4c0[index]) {
                total += ((BuildingTypeClass *)unknown_4c0[index])->PowerDrain_c6c;
            }
        }
    }
    return total;
}


// void BuildingClass::Detach(AbstractClass *target, bool all) (0x00449E40, slot 10)
// The techno detach, then clears every member that refers to the target;
// restarts animation 17 (damaged variant below the yellow health ratio) when
// the target was the animation at +0x474; an animation target leaves the
// eight at +0x49C (or goes through 0x0044CCE0).

void BuildingClass::Detach(AbstractClass *target, bool all)
{
    TechnoClass::Detach(target, all);
    if (unknown_438 == target) {
        unknown_438 = 0;
    }
    if (unknown_440 == target) {
        unknown_440 = 0;
    }
    if (unknown_41c == target) {
        unknown_41c = 0;
    }
    if (unknown_4d4 == target) {
        unknown_4d4 = 0;
    }
    if (unknown_4e8 == target) {
        unknown_4e8 = 0;
    }
    if (unknown_454[8] == target) {
        unknown_44c920(17, Health_Ratio() <= Rules->unknown_1368, false, 0);
    }
    if (target && target->What_Am_I() == 4) {
        for (int index = 0; index < 8; index++) {
            if (unknown_49c[index] == target) {
                unknown_49c[index] = 0;
                return;
            }
        }
        if (((AnimClass *)target)->unknown_100) {
            unknown_44cce0(target);
            return;
        }
    }
    if (Type == target) {
        Type = 0;
    }
    for (int index = 0; index < 3; index++) {
        if (unknown_4c0[index] == target) {
            unknown_4c0[index] = 0;
        }
    }
    unknown_53c.Delete((InfantryClass *)target);
}


// InfantryTypeClass *BuildingClass::unknown_slot184() (0x00449FB0, slot 184)
// For factory type 7 without the flag at +0x5B3, a 25% chance of the rules
// Engineer; otherwise the techno crew.

InfantryTypeClass *BuildingClass::unknown_slot184()
{
    if (!unknown_5b3 && unknown_id_source_a3d290->Random()(0, 99) < 25 && Type->Factory == 7) {
        return Rules->Engineer;
    }
    return TechnoClass::unknown_slot184();
}


// void BuildingClass::unknown_44a000(UnknownBuildingFactory *factory) (0x0044A000)
// With +0x76 set meanwhile, abandons the production of the factory's object
// when no building of the house can build its type any more.

void BuildingClass::unknown_44a000(UnknownBuildingFactory *factory)
{
    if (factory) {
        TechnoClass *object = factory->Get_Object();
        bool saved = unknown_76;
        unknown_76 = true;
        if (object) {
            if (!object->Techno_Type_Class()->Who_Can_Build_Me(true, false, false, House)) {
                House->unknown_func_4e64f0(Type->Factory, object->Techno_Type_Class()->Fetch_Heap_ID(), 0);
            }
        }
        unknown_76 = saved;
    }
}


// 0044A080: notify radio contacts and abandon production when the building is removed.
void BuildingClass::unknown_slot54(bool removing)
{
    if (removing) {
        if (unknown_41c) {
            static_cast<FactoryClass *>(unknown_41c)->Abandon_Production_004B97C0();
            delete static_cast<FactoryClass *>(unknown_41c);
            unknown_41c=0;
        }
        if (House) {
            if (Type->Factory==7) {
                unknown_44a000((UnknownBuildingFactory *)House->unknown_52e8);
                unknown_44a000((UnknownBuildingFactory *)House->unknown_52ec);
                unknown_44a000((UnknownBuildingFactory *)House->unknown_52f0);
                unknown_44a000((UnknownBuildingFactory *)House->unknown_52f4);
                unknown_44a000((UnknownBuildingFactory *)House->unknown_52f8);
            } else {
                unknown_44a000((UnknownBuildingFactory *)House->unknown_func_4eb580(Type->Factory,Type->Naval,0));
            }
        }
    }
    if (!removing) {
        for (int index=0; index<RadioLinks.Length(); ++index) {
            TechnoClass *contact=Contact(index);
            if (contact && !House->Is_Ally(contact)) unknown_slot150(3,contact);
        }
    }
    else {
        SendToEachLink(3);
    }
    ObjectClass::unknown_slot54(removing);
}


// Ask the produced building type to flush its placement cells.
int BuildingClass::Flush_For_Placement_0044a3d0(TechnoClass const *object, Cell const &cell)
{
    if (object) return static_cast<BuildingTypeClass const *>(object->Class_Of())->unknown_458b10(cell, House);
    return -1;
}


// Find a cell where a factory client can leave; original0044A400.
extern "C" Cell const exit_cell_none_0084ed10;
Cell BuildingClass::unknown_slot280(TechnoClass *object)
{
    Cell origin = unknown_slot104();
    Cell cell;
    if (Type->GDIBarracks) {
        cell = origin + Cell(1, 2);
        if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 1) == 0)
            return cell;
    }
    if (Type->NODBarracks) {
        cell = origin + Cell(2, 2);
        if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 1) == 0)
            return cell;
    }
    if (Type->Naval && Type->WeaponsFactory) {
        Cell dock = Cell(unknown_slot41((int)object));
        cell = dock;
        cell += Cell(1, 1);
        if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 0) == 0)
            return cell;
        --cell.Y;
        if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 0) == 0)
            return cell;
        ++cell.Y;
        --cell.X;
        if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 0) == 0)
            return cell;
    }
    Cell const *offset = Type->FoundationOutside;
    if (!offset || Type->Hospital) {
        int bottom = Type->Foundation_Height(false);
        for (int x = -1; x <= Type->Width(); ++x) {
            cell = origin + Cell(x, bottom);
            if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 1) == 0)
                return cell;
            cell = origin + Cell(x, -1);
            if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 1) == 0)
                return cell;
        }
        int right = Type->Width();
        for (int y = -1; y <= Type->Foundation_Height(false); ++y) {
            cell = origin + Cell(right, y);
            if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 1) == 0)
                return cell;
            cell = origin + Cell(-1, y);
            if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 1) == 0)
                return cell;
        }
    } else {
        while (offset->X != 0x7fff || offset->Y != 0x7fff) {
            cell = origin + *offset++;
            if (Map.In_Radar(cell) && object->Can_Enter_Cell(&Map[cell], -1, -1, 0, 0) == 0)
                return cell;
        }
    }
    return exit_cell_none_0084ed10;
}


// bool BuildingClass::Can_Player_Move() (0x0044A930, slot 39)
// False while +0x3FC is positive; true when slot 153 holds; otherwise
// whether the type undeploys into a unit, which for type flag +0x1353 also
// needs game mode, the flag at 0x00A3D340 and the owner's 0x004F52F0.

bool BuildingClass::Can_Player_Move()
{
    if (unknown_3fc > 0) {
        return false;
    }
    if (unknown_slot153()) {
        return true;
    }
    if (Type->ConstructionYard) {
        if (!unknown_game_mode_a3d298 || !unknown_flag_a3d340 || !House->Unknown_004F52F0()) {
            return false;
        }
    }
    return Type->UndeploysInto != 0;
}


// CoordStruct BuildingClass::unknown_slot44(int arg1) (0x0044A9A0, slot 44)
// The coordinate offset by the type's +0xC54 offset when it is not zero,
// otherwise the center.

static inline bool Coord_Differs(CoordStruct const &a, CoordStruct const &b)
{
    return a.X != b.X || a.Y != b.Y || a.Z != b.Z;
}

CoordStruct BuildingClass::unknown_slot44(int arg1)
{
    if (Coord_Differs(Type->ExitCoord, unknown_geometry_l)) {
        CoordStruct coord = Coord;
        return coord + Type->ExitCoord;
    }
    return Center_Coord();
}


// Private full184-byte helper plus8 NOP. Class/method spelling is a hypothesis.
// No actual incoming references found; canonical neighboring bodies independently bound it.
Cell BuildingClass::Offset_Toward_Map_Center_0044aa40(int mode)
{
    int dx=6;
    int dy=5;
    Cell cell=Coord_Cell(Center_Coord());
    switch(mode) {
    case 0:dx=0;break;
    case 2:dy=0;break;
    }
    if((int)((unsigned int)cell.X-(unsigned int)Map.unknown_120)>Map.unknown_128/2)dx=-dx;
    if((int)((unsigned int)cell.Y-(unsigned int)Map.unknown_124)>Map.unknown_12c/2)dy=-dy;
    int vertical=dy;
    return Cell((short)(cell.X+dx),(short)(vertical+cell.Y));
}


// void BuildingClass::unknown_44ab00() (0x0044AB00)
// Recomputes the house's flags (+0x5414): the OR of the +0x1A8 of each of
// its buildings that has a Radar type and is not selected out (+0x76
// clear); then flags the screen for a redraw.

void BuildingClass::unknown_44ab00()
{
    House->unknown_5414 = 0;
    int count = House->Buildings.Count();
    for (int index = 0; index < count; index++) {
        BuildingClass *building = House->Buildings[index];
        if (building && !building->unknown_76 && building->Type->Radar) {
            House->unknown_5414 |= building->unknown_1a8;
        }
    }
    Map.GScreenClass::Flag_To_Redraw(2);
}


// BuildingClass::Read_INI 0x0044AB80: the [Structures] entries (TibSun building.cpp Read_INI).
// Turn_On 0x44D3E0, Power_On 0x44D600 and Adjust_House_Power 0x44FA40 are inlined; the online call goes through a
// pointer copy, which gives the original's base/index order in the inlined Power_On anim loop.
extern "C" char const *unknown_structure_section_0079cb88;
extern int unknown_ini_format_a40d24;
void __fastcall BuildingClass::Read_INI(CCINIClass &ini)
{
    BuildingClass *b;
    HouseClass *bhptr;
    int classid;
    Cell cell;
    char buf[128];
    char *trigname;

    int i;
    int upgrades[3];

    for (i = 0; i < 3; i++) {
        upgrades[i] = -1;
    }

    int len = ini.Entry_Count(unknown_structure_section_0079cb88);
    for (int index = 0; index < len; index++) {
        char const *entry = ini.Get_Entry(unknown_structure_section_0079cb88, index);

        ini.Get_String(unknown_structure_section_0079cb88, entry, 0, buf, sizeof(buf));

        int bhouse = HouseClass::Find_Index(strtok(buf, ","));
        if (bhouse == -1) {
            continue;
        }
        bhptr = Houses[bhouse];

        if (bhptr == 0) {
            continue;
        }

        if (unknown_game_mode_a3d298 != 0 && bhptr == unknown_player_a35db4) {
            continue;
        }

        classid = BuildingTypeClass::From_Name(strtok(0, ","));

        if (classid != -1) {
            int strength;
            int facing;

            strength = atoi(strtok(0, ","));

            if (unknown_ini_format_a40d24 >= 4) {
                Cell parsed;
                parsed.X = (short)atoi(strtok(0, ","));
                parsed.Y = (short)atoi(strtok(0, ","));
                cell = parsed;
            } else {
                int packed = atoi(strtok(0, ","));
                cell = Cell((short)(packed % 128), (short)(packed / 128));
            }

            facing = atoi(strtok(0, ","));

            trigname = strtok(0, ",");

            bool sellable = false;
            char *token_pointer = strtok(0, ",");
            if (token_pointer) {
                sellable = atoi(token_pointer) != 0;
            }

            bool rebuild = false;
            token_pointer = strtok(0, ",");
            if (token_pointer) {
                rebuild = atoi(token_pointer) != 0;
            }

            bool online = true;
            token_pointer = strtok(0, ",");
            if (token_pointer) {
                online = atoi(token_pointer) != 0;
            }

            int upgradecount = 0;
            token_pointer = strtok(0, ",");
            if (token_pointer) {
                upgradecount = atoi(token_pointer);
            }

            int spotlight = 1;
            token_pointer = strtok(0, ",");
            if (token_pointer) {
                spotlight = atoi(token_pointer);
            }

            for (i = 0; i < 3; i++) {
                token_pointer = strtok(0, ",");
                if (token_pointer) {
                    upgrades[i] = BuildingTypeClass::From_Name(token_pointer);
                }
            }

            bool repair = false;
            token_pointer = strtok(0, ",");
            if (token_pointer) {
                repair = atoi(token_pointer) != 0;
            }

            bool nominal = false;
            token_pointer = strtok(0, ",");
            if (token_pointer) {
                nominal = atoi(token_pointer) != 0;
            }

            b = new BuildingClass(BuildingTypes[classid], bhptr);
            if (b) {

                TagTypeClass *tp = TagTypeClass::From_Name(trigname);
                if (tp) {
                    TagClass *tt = TagClass::Find_Or_Make(tp);
                    if (tt) {
                        b->Attach_Tag(tt);
                    }
                }
                b->unknown_5ac = sellable;
                b->unknown_59a = rebuild;
                b->unknown_59b = repair;
                b->unknown_599 = nominal;

                strength = (strength < 256) ? strength : 256;
                strength = b->Type->MaxStrength * (strength / 256.0);
                b->Strength = strength;
                if (b->Strength > b->Type->MaxStrength - 3) b->Strength = b->Type->MaxStrength;
                b->unknown_70 = b->Strength;

                if (b->Unlimbo(&CoordStruct(cell), facing)) {

                    if (b->unknown_4d4) {
                        static_cast<BuildingLightClass *>(b->unknown_4d4)->Set_Behavior(spotlight);
                    }

                    if (online) {
                        if (!b->unknown_530 && b->unknown_3fc == 0) {
                            BuildingClass *building = b;
                            building->unknown_44d3e0();
                        }
                    } else {
                        b->unknown_44d530();
                    }

                    b->unknown_375 = false;

                    if (b->Type->NeedsEngineer && !b->unknown_59c) {
                        b->unknown_44d6c0();
                    }

                    if (upgradecount) {
                        for (i = 0; i < upgradecount; i++) {
                            if (upgrades[i] != -1) {
                                BuildingTypeClass *utype = BuildingTypes[upgrades[i]];
                                BuildingClass *u = new BuildingClass(utype, bhptr);
                                u->Unlimbo(&CoordStruct(cell), facing);
                            }
                        }
                    }
                    if (b->unknown_slot232()) {
                        Building_Update_Turret_Animation_Frame_0043bce0(b);
                    }
                } else {
                    delete b;
                }
            }
        }
    }
}


// PRIVATE prototype hypotheses: free fastcall wrapper, Building member writer with INI reference.
extern "C" char const *unknown_structure_section_0079cb88;
void __fastcall Building_Write_All_INI_0044b200(INIClass *ini)
{
    ini->Clear(unknown_structure_section_0079cb88, 0);
    for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); ++index) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
        if (!building->unknown_76 && !building->Type->LaserFence)
            building->Write_Structure_INI_0044b250(*ini);
    }
}


// Building4D4 light: actual432FC0constructor result stored43E030; existingAbstract*field kept, realderivedcast explicit.
extern "C" char const *unknown_structure_section_0079cb88;
extern "C" char const unknown_none_007ce9b4[];
extern "C" char const unknown_integer_format_007ce9bc[];
extern "C" char const unknown_append_integer_007cfa6c[];
extern "C" char const unknown_append_string_007cfa70[];
extern "C" char const unknown_structure_format_007cfa74[];
void BuildingClass::Write_Structure_INI_0044b250(INIClass &ini)
{
    char key[12];
    char value[128];
    sprintf(key,unknown_integer_format_007ce9bc,Fetch_ID());
    int behavior=unknown_4d4?static_cast<BuildingLightClass *>(unknown_4d4)->Behavior:0;
    char const *tag=(unknown_34 && unknown_34->Class)?(char const *)unknown_34->Class->ID:unknown_none_007ce9b4;
    sprintf(value,unknown_structure_format_007cfa74,(char const *)House->IniName,(char const *)Type->ID,
            (int)(Health_Ratio()*256.0+0.5),unknown_slot104().X,unknown_slot104().Y,
            Dir_To_256(unknown_2e0.Current()),tag,(int)unknown_5ac,(int)unknown_59a,(int)unknown_530,
            (int)(signed char)unknown_5c6,behavior);
    TechnoTypeClass **upgrade=unknown_4c0;
    int remaining=3;
    do {
        char const *name=*upgrade?(char const *)(*upgrade)->ID:unknown_none_007ce9b4;
        sprintf(value+strlen(value),unknown_append_string_007cfa70,name);
        ++upgrade;
    }while(--remaining);
    sprintf(value+strlen(value),unknown_append_integer_007cfa6c,unknown_59b!=0);
    sprintf(value+strlen(value),unknown_append_integer_007cfa6c,unknown_599!=0);
    ini.Put_String(unknown_structure_section_0079cb88,key,value);
}


// CoordStruct BuildingClass::unknown_slot40() (0x0044B430, slot 40)
// The center coordinate offset by the type's coordinate at +0xC48.

CoordStruct BuildingClass::unknown_slot40()
{
    CoordStruct center = Center_Coord();
    return Type->TargetCoordOffset + center;
}


// AI factory production, completed-object placement and stalled production refunds.
void BuildingClass::Factory_AI_0044b480()
{
    if (unknown_41c && static_cast<FactoryClass *>(unknown_41c)->Is_Complete_004B9900() && unknown_448.Value() == 0) {
        TechnoClass *product = static_cast<FactoryClass *>(unknown_41c)->Get_Object();
        switch (Exit_Object(product)) {
        case 0:
            if (product->Kind_Of() == 6 && static_cast<BuildingClass *>(product)->Type->Naval) {
                product->Owning_House()->unknown_138 = false;
            }
            static_cast<FactoryClass *>(unknown_41c)->Abandon_Production_004B97C0();
            delete static_cast<FactoryClass *>(unknown_41c);
            unknown_41c = 0;
            break;
        case 1:
            unknown_448 = (int)(Rules->PlacementDelay * 900);
            break;
        case 2: {
            CoordStruct location(Coord);
            Voc_Static_Add(Type->CreateUnitSound, location, 0);
            House->Just_Built(product);
            static_cast<FactoryClass *>(unknown_41c)->Release_Completed_004B9970();
            delete static_cast<FactoryClass *>(unknown_41c);
            unknown_41c = 0;
            break;
        }
        }
    }
    if (House->unknown_136 && Get_Mission() != 18 && Get_Mission() != 19 && Type->Factory != 0) {
        if (unknown_41c) {
            if (unknown_448.Value() == 0) {
                FactoryClass *factory = static_cast<FactoryClass *>(unknown_41c);
                if (factory->Rate != 0 && !factory->unknown_6c) return;
                factory->Abandon_Production_004B97C0();
                delete static_cast<FactoryClass *>(unknown_41c);
                unknown_41c = 0;
            }
        } else {
            if (House->unknown_136 && House->Available_Money() > 10) {
                TechnoTypeClass *type = House->unknown_func_4e74c0(Type->Factory, false);
                if (type) {
                    if (type->Kind_Of() == 40) {
                        if (Type->Naval && !type->Naval) return;
                        if (type->Naval && !Type->Naval) return;
                    }
                    unknown_41c = new FactoryClass;
                    if (unknown_41c) {
                        if (!static_cast<FactoryClass *>(unknown_41c)->unknown_4b9440(type, House, 0)) {
                            delete static_cast<FactoryClass *>(unknown_41c);
                            unknown_41c = 0;
                        } else {
                            House->unknown_4ea7f0(static_cast<FactoryClass *>(unknown_41c)->Get_Object());
                            static_cast<FactoryClass *>(unknown_41c)->Start_Production_004B9670(false);
                        }
                    }
                }
            }
        }
    }
}


// 0x0044b760: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_0044b760(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0044b760 {

void  unknown_empty_0044b760(void)
{
    return;
}

}  // namespace seed_0044b760


// BuildingClass 0x0044B770: the delayed-fire countdown. At zero, action 1 fires the weapon recorded in the
// coordinate's X at the target (a prism tower's support count raises the bullet's damage), action 2 creates
// the support laser toward the recorded coordinate.
void BuildingClass::unknown_44b770()
{
    if (unknown_5c8 != 0) {
        if (--unknown_5d8 <= 0) {
            unknown_5d8 = 0;
            switch (unknown_5c8) {
                case 1:
                    if (unknown_228 != 0 && unknown_slot218(unknown_228, unknown_5cc.X, true) == 0) {
                        BulletClass *bullet = unknown_slot221(unknown_228, unknown_5cc.X);
                        if (bullet != 0 && unknown_534 != 0) {
                            bullet->DamageMultiplier = (unsigned)((Rules->PrismSupportModifier * unknown_534 + 100) * 256) / 100;
                            unknown_534 = 0;
                        }
                    }
                    break;

                case 2:
                    unknown_446820(unknown_5cc);
                    break;
            }
            unknown_5c8 = 0;
        }
    }
}


// void BuildingClass::unknown_44b870(int arg1, CoordStruct const &coord) (0x0044B870)
// Records arg1, the type's DelayedFireDelay and the coordinate (unknown_5c8,
// unknown_5d8, unknown_5cc), ends animation 3 and starts animation 10 in the
// state the health gives.

void BuildingClass::unknown_44b870(int arg1, CoordStruct const &coord)
{
    unknown_5d8 = Type->DelayedFireDelay;
    unknown_5c8 = arg1;
    memcpy(&unknown_5cc, &coord, sizeof(unknown_5cc));
    unknown_44cfd0(3);
    unknown_44c920(10, Health_Ratio() <= Rules->unknown_1368, false, 0);
}


// void BuildingClass::unknown_44b910() (0x0044B910)
// Drops the infantry in unknown_53c whose +0x228 target is not this
// building; unknown_531 is then set when three are left, or when at least
// one is, unknown_530 holds and the house has full power.

void BuildingClass::unknown_44b910()
{
    for (int index = unknown_53c.Count() - 1; index >= 0; index--) {
        if (((TechnoClass *)unknown_53c[index])->unknown_228 != this) {
            unknown_53c.Delete(index);
        }
    }
    int count = unknown_53c.Count();
    if (count >= 3 || (House->Power_Fraction() == 1.0 && unknown_530 && count >= 1)) {
        unknown_531 = true;
    } else {
        unknown_531 = false;
    }
}


// Decide AI repair/sale and apply each paid repair tick.
void BuildingClass::Repair_AI_0044b9b0()
{
    if (House->unknown_194 >= Rules->IQRepairSell && Get_Mission() != 18 && Get_Mission() != 19 && Can_Repair()) {
        if (House->Available_Money() >= Rules->CreditReserve) {
            if (!House->unknown_18c && !unknown_5b8 && (unknown_5b3 || unknown_59b || House->Unknown_004F52F0())) {
                House->unknown_18c = true;
                Repair(1);
                if (!House->Unknown_004F52F0()) {
                    House->unknown_1c8 = Random_Pick((int)(House->unknown_108 * 225.0), (int)(House->unknown_108 * 1800.0));
                }
            }
        } else {
            if ((unknown_game_mode_a3d298 != 0 || unknown_5ac) && unknown_329 && (unsigned)House->unknown_118 >= (unsigned)Rules->IQSellBack && (unsigned)Random_Pick(0, 50) < (unsigned)House->unknown_11c && unknown_34 == 0 && Type->Factory != 7 && Health_Ratio() < Rules->unknown_1370) {
                Sell_Back(1);
            }
        }
    }
    if (unknown_5b8 && Frame % (int)(Rules->unknown_1348 * 900.0) == 0) {
        unknown_5ae = !unknown_5ae;
        int cost = Type->Repair_Cost();
        int step = Type->Repair_Step();
        int old_frame = unknown_43be20();
        if (House->Available_Money() >= cost) {
            House->Unknown_004E5280(cost);
            Strength += step;
            unknown_70 += step;
            if (Strength >= Type->MaxStrength) {
                unknown_70 = Strength = Type->MaxStrength;
                unknown_5b8 = false;
            }
            unknown_44d070(Health_Ratio() <= Rules->unknown_1368);
            if (Health_Ratio() > Rules->unknown_1368 && unknown_25c[3]) unknown_25c[3]->Delete_Me();
            if (unknown_43be20() != old_frame) unknown_75 = true;
        } else {
            unknown_5b8 = false;
        }
    }
}


// Complete Building animation update; source names remain hypotheses.
extern "C" int const unknown_turret_frame_table_007acab0[32];
extern "C" float const unknown_charge_scale_0079d148;
extern "C" float const unknown_charge_limit_0079d14c;
inline void Start_Condition_Animation_0044BD40(BuildingClass *building,int index,bool damaged)
{
 char const *name=damaged?building->Type->Anims[index].Damaged.Buffer:building->Type->Anims[index].Anim.Buffer;
 if(name && *name)building->unknown_44ca60(name,index,damaged,false,0);
}
inline int Refinery_Storage_Stage_0044BD40(BuildingClass *building)
{
    int stored=(int)building->unknown_294.Get_Total_Amount();
    if(stored!=0) {
        stored=(int)building->unknown_294.Get_Total_Amount();
        stored=(int)((unsigned int)stored<<2)/building->Type->Storage;
    }
    return stored;
}
inline void Apply_Frame_And_Layer_0044BD40(BuildingClass *building,ConvertClass *value,int level)
{
    building->unknown_44d0f0(value,(unsigned short)level);
    building->unknown_44d2d0(building->unknown_5bd);
}
inline void Refresh_Building_Frame_0044BD40(BuildingClass *building)
{
    ConvertClass *value=building->unknown_slot115();
    CellClass *cell=static_cast<CellClass *>(building->Get_Cell_Ptr());
    int level=building->unknown_4521e0(cell->unknown_10a);
    building->unknown_5c4=(short)level;
    Apply_Frame_And_Layer_0044BD40(building,value,level);
}
inline int Silo_Storage_Frame_0044BD40(BuildingClass *building)
{
 int frame=0;
 if(building->Type->Storage>0) {
  int stored=(int)building->unknown_294.Get_Total_Amount();
  stored=(int)((unsigned int)stored<<2);
  frame=(int)((double)stored/building->Type->Storage+0.5);
  if(frame<0)return 0;
  if(frame>3)return 3;
 }
 return frame;
}
void BuildingClass::Update_Animations_0044BD40()
{
    bool advanced=StageClass::Graphic_Logic();
    bool finished=false;
    Refresh_Building_Frame_0044BD40(this);
    if(Type->UnitRepair && Get_Mission()!=20 && (unknown_454[8] || unknown_454[11]) && !Type->Naval) {
        Start_Condition_Animation_0044BD40(this,12,!(Health_Ratio()>Rules->unknown_1368));
        unknown_44cfd0(8);
        unknown_44cfd0(11);
    }
    if(Type->SiloDamage) {
        int stage=Silo_Storage_Frame_0044BD40(this);
        AnimClass *animation=static_cast<AnimClass *>(unknown_454[10]);
        if(stage!=0) {
            if(!animation) {
                Start_Condition_Animation_0044BD40(this,10,!(Health_Ratio()>Rules->unknown_1368));
            }
            static_cast<AnimClass *>(unknown_454[10])->Stage=stage;
        }else if(animation)unknown_44cfd0(10);
    }
    if(Type->Refinery) {
        int stage=Refinery_Storage_Stage_0044BD40(this);
        if(unknown_5c0!=stage) {
            int previous=unknown_5c0;
            if(previous>=3)unknown_44cfd0(6);
            else if(previous>=2)unknown_44cfd0(5);
            else if(previous>=1)unknown_44cfd0(4);
            else if(previous>=0)unknown_44cfd0(3);
            unknown_5c0=Refinery_Storage_Stage_0044BD40(this);
            if(unknown_5c0>=3)Start_Condition_Animation_0044BD40(this,6,!(Health_Ratio()>Rules->unknown_1368));
            else if(unknown_5c0>=2)Start_Condition_Animation_0044BD40(this,5,!(Health_Ratio()>Rules->unknown_1368));
            else if(unknown_5c0>=1)Start_Condition_Animation_0044BD40(this,4,!(Health_Ratio()>Rules->unknown_1368));
            else if(unknown_5c0>=0)Start_Condition_Animation_0044BD40(this,3,!(Health_Ratio()>Rules->unknown_1368));
        }
    }
    if(Type->SuperWeapon!=-1 && Get_Mission()!=18 && Get_Mission()!=19 && !(Type->ChargedAnimTime>unknown_charge_limit_0079d14c)) {
        for(int index=0;index<House->Supers.Count();++index) {
            SuperClass *super=House->Supers[index];
            int super_type=super->Type->Type;
            if(super_type==Type->SuperWeapon) {
                if(!(Type->ChargedAnimTime<super->RechargeTimer.Value()*unknown_charge_scale_0079d148)) {
                    if(unknown_454[13]) {
                        unknown_44cfd0(13);
                        Start_Condition_Animation_0044BD40(this,14,!(Health_Ratio()>Rules->unknown_1368));
                    }
                }else if(unknown_454[15]) {
                    unknown_44cfd0(15);
                    Start_Condition_Animation_0044BD40(this,16,!(Health_Ratio()>Rules->unknown_1368));
                }
            }
        }
    }
    if(!unknown_slot232() || Get_Mission()==18 || Get_Mission()==19) {
        if(advanced) {
            BuildingTriple *state=&Type->AnimFrames[unknown_42c];
            if(StageClass::Stage==state->unknown_04-1+state->unknown_00 ||
               (!unknown_1b0 && Type->UndeploysInto && Get_Mission()==19 && StageClass::Stage==23))unknown_5ad=true;
            if(StageClass::Stage>=state->unknown_04+state->unknown_00)finished=true;
            unknown_75=true;
        }else if(unknown_42c==-1 || StageClass::Rate==0)unknown_5ad=true;
    }
    if(unknown_slot203())unknown_75=true;
    if(unknown_slot232() && unknown_slot232() && unknown_454[9]) {
        int index=Dir_To_32(unknown_2e0.Current());
        AnimClass *animation=static_cast<AnimClass *>(unknown_454[9]);
        if(animation) {
            int &frame=animation->Stage;
            int &step=animation->Step;
            frame=unknown_turret_frame_table_007acab0[index];
            step=0;
        }
    }
    if(finished) {
        BuildingTriple *state=&Type->AnimFrames[unknown_42c];
        if(unknown_42c==0 || unknown_42c==1) {
            StageClass::Set_Rate(Options.Normalize_Delay(state->unknown_08));
        }else {
            StageClass::Set_Rate(state->unknown_08);
        }
        StageClass::Stage=state->unknown_00;
        Mark(2);
    }
}


// int BuildingClass::unknown_slot172() (0x0044C520, slot 172)
// For a crewed type without the flag at +0x5B0: the type's refund (slot 46)
// divided by a rules step (+0x1164 for owner index 1, else +0x1160; doubled
// with +0x5B3), clamped to 1..5; otherwise 0.

int BuildingClass::unknown_slot172()
{
    if (!unknown_5b0 && Type->Crewed) {
        int step = (unknown_114 == 1) ? Rules->unknown_1164 : Rules->unknown_1160;
        if (step) {
            if (unknown_5b3) {
                step += step;
            }
            int count = Type->unknown_slot46(House, false) / step;
            if (count < 1) {
                return 1;
            }
            if (count > 5) {
                return 5;
            }
            return count;
        }
    }
    return 0;
}


// void *BuildingClass::Get_Image_Data() const (0x0044C5A0, slot 26)
// The type's image: slot 39 while +0x42C is set and +0x5B4 holds, otherwise
// the buildup image (type slot 48).

void *BuildingClass::Get_Image_Data()
{
    if (unknown_42c && unknown_5b4) {
        return Type->Get_Image_Data();
    }
    return Type->Get_Buildup_Data();
}


// Advance the upgrade animation sequence and refresh animation names when strength changes.
// PRIVATE member/name and bool source spelling hypotheses; actual AL false/true and RET0.
extern "C" char const unknown_turret_separator_007cf730[];
inline void Start_Upgrade_Animation_0044c5d0(BuildingClass *building,int index,bool damaged)
{
    char const *name=damaged?building->Type->Anims[index].Damaged.Buffer:building->Type->Anims[index].Anim.Buffer;
    if(name && *name)building->unknown_44ca60(name,index,damaged,false,0);
}
bool BuildingClass::Advance_Upgrade_Animation_0044c5d0()
{
    int strength=Type->MaxStrength;
    if(Strength!=strength) {
        Strength=strength;
        if(unknown_5b6) {
            unknown_5b6=false;
            for(int index=0;index<18;++index) {
                if(unknown_454[index]) {
                    char const *name=Type->Anims[index].Anim.Buffer;
                    if(name && *name) unknown_44ca60(name,index,false,false,0);
                }
            }
        }
        if(Health_Ratio()>Rules->unknown_1368 && unknown_25c[3]) unknown_25c[3]->Delete_Me();
        for(int index=3;index<=6;++index) {
            if(!unknown_454[index]) {
                char const *name=Type->Anims[index].Anim.Buffer;
                if(name && *name) unknown_44ca60(name,index,false,false,0);
            }
        }
        unknown_75=true;
    }
    if(Type==Rules->unknown_78c) {
    int frame=(int)((unsigned int)unknown_4d0+1u);
    if(frame>2)frame=0;
    if(frame!=unknown_4d0 && !Type->TurretAnimIsVoxel) {
        unknown_44cfd0(9);
        if(frame!=-1) {
            char name[64];
            strcpy(name,(char const *)Type->unknown_1c8);
            strcat(name,unknown_turret_separator_007cf730);
            unsigned int length=strlen(name);
            name[length]=(char)((unsigned int)frame+(unsigned int)'B');
            name[length+1]=0;
            unknown_44ca60(name,9,false,false,0);
            unknown_4d0=frame;
        }
    }
    unknown_5c6=(char)((unsigned char)unknown_5c6+1u);
    return true;
    }
    if((int)unknown_5c6==Type->Upgrades)return false;
    unknown_5c6=(char)((unsigned char)unknown_5c6+1u);
    Start_Upgrade_Animation_0044c5d0(this,(int)unknown_5c6-1,Health_Ratio()<=Rules->unknown_1368);
    return true;
}


// bool BuildingClass::unknown_44c860() (0x0044C860)
// Removes the last upgrade (unknown_4c0 up to unknown_5c6). An upgrade with
// a turret clears them all and resets unknown_4d0; one with a super weapon
// tells the house (0x004F4B20). False when there was no upgrade.

bool BuildingClass::unknown_44c860()
{
    if (!unknown_5c6) {
        return false;
    }
    bool super_weapon = false;
    if (unknown_4c0[unknown_5c6 - 1] && ((BuildingTypeClass *)unknown_4c0[unknown_5c6 - 1])->SuperWeapon != -1) {
        super_weapon = true;
    }
    if (unknown_4c0[unknown_5c6 - 1] && unknown_4c0[unknown_5c6 - 1]->Turret) {
        unknown_44cfd0(9);
        unknown_4c0[unknown_5c6 - 1] = 0;
        unknown_5c6 = 0;
        unknown_4d0 = -1;
        if (super_weapon) {
            House->Update_Present_Super_Weapons();
        }
        return true;
    }
    unknown_44cfd0(unknown_5c6 - 1);
    unknown_5c6--;
    unknown_4c0[unknown_5c6] = 0;
    if (super_weapon) {
        House->Update_Present_Super_Weapons();
    }
    return true;
}


// void BuildingClass::unknown_44c920(int index, bool damaged, bool garrisoned, int arg4) (0x0044C920)
// Starts the animation of an Anims index with the type's name for the
// state (damaged first, else garrisoned, else normal), when it has one.

void BuildingClass::unknown_44c920(int index, bool damaged, bool garrisoned, int arg4)
{
    char const *name;
    if (damaged) {
        name = Type->Anims[index].Damaged.Buffer;
    } else if (garrisoned) {
        name = Type->Anims[index].Garrisoned.Buffer;
    } else {
        name = Type->Anims[index].Anim.Buffer;
    }
    if (name && *name) {
        unknown_44ca60(name, index, damaged, garrisoned, arg4);
    }
}


// PRIVATE complete177-body+15-padding Building animation refresh.
// Uses approved private Tactical::Unknown_006A2BF0(Point2D const&).
void BuildingClass::unknown_44c9a0()
{
    CoordStruct center=unknown_slot42();
    for(int index=0;index<18;++index) {
        // Original dispatch requires the ObjectClass slot103 interface; concrete entry class is not assumed.
        ObjectClass *animation=static_cast<ObjectClass *>(unknown_454[index]);
        if(animation) {
            CoordStruct position=unknown_tactical_83996c->Unknown_006A2BF0(Type->Anims[index].Offset)+center;
            animation->Set_Coord(position);
        }
    }
}


// Create and replace one attached building animation, preserving its stage.
inline int Fetch_Building_Animation_Stage_0044ca60(AnimClass const &animation)
{
    return animation.Stage;
}
inline void Set_Building_Animation_Stage_0044ca60(AnimClass &animation, int stage)
{
    animation.Stage=stage;
}
inline void Hide_Building_Animation_0044ca60(AnimClass &animation)
{
    animation.unknown_17d=true;
}
void BuildingClass::unknown_44ca60(char const *name, int index, bool damaged, bool garrisoned, int delay)
{
    unknown_44d070(damaged);
    int type = AnimTypeClass::From_Name(name);
    if (type != -1) {
        CoordStruct coord(unknown_slot42() + unknown_tactical_83996c->Unknown_006A2BF0(Type->Anims[index].Offset));
        AnimClass *animation = new AnimClass(AnimTypes[type], coord, delay, 1, 0x1600, 0);
        animation->ZAdjust = Type->Anims[index].ZAdjust;
        animation->YSortAdjust = Type->Anims[index].YSort;
        animation->unknown_100 = true;
        if (unknown_5b7) animation->unknown_179 = true;
        unknown_44d2d0(unknown_5bd);
        if (unknown_454[index]) {
            Set_Building_Animation_Stage_0044ca60(*animation,Fetch_Building_Animation_Stage_0044ca60(*static_cast<AnimClass *>(unknown_454[index])));
            AbstractClass *previous = unknown_454[index];
            unknown_454[index] = 0;
            delete previous;
        }
        unknown_454[index] = animation;
        if (!unknown_5ba && Type->Anims[index].Powered) animation->unknown_func_422c10();
        if (animation->Class->ShouldUseCellDrawer) {
            animation->unknown_bc = unknown_slot115();
            int height = reinterpret_cast<UnknownBuildingSlot105 *>(Get_Cell_Ptr())->unknown_10a;
            int brightness = unknown_slot254(height);
            if (unknown_slot203()) brightness = unknown_6d86f0(brightness);
            animation->unknown_e4 = brightness;
        }
        if (index == 9 && Type->BarrelAnimIsVoxel) Hide_Building_Animation_0044ca60(*animation);
        if (unknown_slot111()) animation->unknown_102 = true;
    }
}


// Original: 748 body bytes including 28 switch-table bytes; four frozen NOPs.
// Clear a detached building animation and restart the appropriate follow-up state.
void BuildingClass::unknown_44cce0(AbstractClass *object)
{
    if(!unknown_7d) return;
    for(int index=0;index<18;++index) {
        if(unknown_454[index]!=object) continue;
        unknown_454[index]=0;
        AnimClass *animation=static_cast<AnimClass *>(object);
        switch(index) {
        case 10:
            if(Type->UnitRepair) {
                if(HasAnyLink() && Get_Mission()==20) {
                    unknown_44c920(11,Health_Ratio()<=Rules->unknown_1368,false,0);
                    return;
                }
                unknown_44c920(17,Health_Ratio()<=Rules->unknown_1368,false,0);
            } else if(Type->IsAnimDelayedFire && animation->unknown_159) {
                unknown_44c920(3,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
            }
            break;
        case 12:
            if(Type->UnitRepair && animation->unknown_159)
                unknown_44c920(17,Health_Ratio()<=Rules->unknown_1368,false,0);
            break;
        case 14:
            if(animation->unknown_159)
                unknown_44c920(15,Health_Ratio()<=Rules->unknown_1368,false,0);
            break;
        case 16:
            if(animation->unknown_159)
                unknown_44c920(13,Health_Ratio()<=Rules->unknown_1368,false,0);
            break;
        }
        return;
    }
}


// void BuildingClass::unknown_44cfd0(int index) (0x0044CFD0)
// Deletes the owned object at unknown_454[index], or all eighteen for -2.

void BuildingClass::unknown_44cfd0(int index)
{
    if (index == -2) {
        for (int i = 0; i < 18; i++) {
            AbstractClass *object = unknown_454[i];
            if (object) {
                unknown_454[i] = 0;
                delete object;
            }
        }
    } else {
        AbstractClass *object = unknown_454[index];
        if (object) {
            unknown_454[index] = 0;
            delete object;
        }
    }
}


// Change attached animation damage state and restart each occupied slot.
void BuildingClass::unknown_44d070(bool damaged)
{
    if (unknown_5b6 == damaged) return;
    unknown_5b6 = damaged;
    for (int index = 0; index < 18; ++index) {
        if (unknown_454[index]) unknown_44c920(index, damaged, false, 0);
    }
}


// 0x0044d030: function not yet identified
//
// Body: 63 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall unknown_mark_anims_0044d030(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0044d030 {

struct Owner_0044d030 { void unknown_mark_anims_0044d030(int slot); };

void Owner_0044d030::unknown_mark_anims_0044d030(int slot)
{
    int self = (int)this;
    int *anim;
    int item;

    if (slot == -2) {
        anim = (int *)(self + 0x454);
        item = (int)(0x12);
        do {
            if (*anim != 0) {
                *(unsigned char *)(*anim + 0x17d) = 1;
            }
            anim = (int *)(anim + 1);
            item = (int)(item + -1);
        } while (item != 0);
        return;
    }
    item = (int)(*(int *)(self + 0x454 + slot * 4));
    if (item != 0) {
        *(unsigned char *)(item + 0x17d) = 1;
    }
    return;
}

}  // namespace seed_0044d030


// void BuildingClass::unknown_44d0f0(ConvertClass *value, int frame) (0x0044D0F0)
// Gives each animation in unknown_454 the value and frame (when its type has
// +0x318 set) and then the building's layer (slot 79 expands it).

void BuildingClass::unknown_44d0f0(ConvertClass *value, int frame)
{
    for (int index = 0; index < 18; index++) {
        AnimClass *anim = static_cast<AnimClass *>(unknown_454[index]);
        if (anim != 0) {
            if (anim->Class != 0 && anim->Class->ShouldUseCellDrawer) {
                anim->unknown_bc = value;
                anim->unknown_e4 = frame;
            }
            unknown_44d2d0(unknown_5bd);
        }
    }
}


// void BuildingClass::Update_Anim_Appearance_0044d190() (0x0044D190)
// Gives every animation in unknown_454 whose type has +0x318 set the slot
// 115 value and the frame (Tinted_Brightness 0x004521E0 of the height level
// of its cell: slot 254, adjusted by 0x006D86F0 when slot 203 holds; also kept
// in unknown_5c4), then the building's layer. Written as ra2-decomp's
// Update_Anim_Appearance (YR 0x00452000, which calls Tinted_Brightness
// 0x00456FB0): the drawer loop is the Set_Anim_Drawer call 0x0044D0F0 and it is
// passed the stored member (Brightness); that smaller body lets Mark 0x0043BFC0
// expand it. The Tinted_Brightness call is one charged inline level inside it:
// where Captured 0x00444660 expands this function, it leaves 0x0044D0F0 and
// 0x0044D2D0 out of line, as the original (and the YR twin 0x00448260) does.

void BuildingClass::Update_Anim_Appearance_0044d190()
{
    ConvertClass *value = unknown_slot115();
    unknown_5c4 = unknown_4521e0(((UnknownBuildingSlot105 *)Get_Cell_Ptr())->unknown_10a);
    unknown_44d0f0(value, (unsigned short)unknown_5c4);
    unknown_44d2d0(unknown_5bd);
}


// void BuildingClass::unknown_44d2d0(int layer) (0x0044D2D0)
// Gives every animation in unknown_454 the layer; 0xF becomes 0x10 when
// slot 25 answers 5 (slot 79 expands it after each animation).

void BuildingClass::unknown_44d2d0(int layer)
{
    int value = layer;
    if (layer == 0xF && unknown_slot25(false, 0) == 5) {
        value = 0x10;
    }
    for (int anim = 0; anim < 18; anim++) {
        if (unknown_454[anim] != 0) {
            ((UnknownBuildingAnim *)unknown_454[anim])->unknown_158 = value;
        }
    }
}


// 0x0044d320: function not yet identified
//
// Body: 89 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall unknown_temporal_freeze_0044d320(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0044d320 {

void __fastcall unknown_temporal_freeze_0044d320(int self)
{
    int anim;
    int offset;
    int *slot;

    *(unsigned char *)(self + 0x530) = 0;
    offset = (int)(0);
    slot = (int *)(self + 0x454);
    do {
        if ((*(char *)(*(int *)(self + 0x418) + 0xd14 + offset) != '\0') &&
              (anim = *slot, anim != 0)) {
            *(unsigned char *)(anim + 0x102) = 1;
            *(unsigned char *)(anim + 0x103) = 0;
            *(unsigned int *)(anim + 0x104) = *(unsigned int *)(anim + 0x98);
            *(unsigned char *)(anim + 0x101) = 1;
        }
        offset = (int)(offset + 0x44);
        slot = (int *)(slot + 1);
    } while (offset < 0x4c8);
    return;
}

}  // namespace seed_0044d320


// 0x0044d380: function not yet identified
//
// Body: 76 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void __fastcall unknown_temporal_thaw_0044d380(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0044d380 {

void __fastcall unknown_temporal_thaw_0044d380(int self)
{
    int anim;
    int offset;
    int *slot;

    *(unsigned char *)(self + 0x530) = 1;
    offset = (int)(0);
    slot = (int *)(self + 0x454);
    do {
        if ((*(char *)(*(int *)(self + 0x418) + 0xd14 + offset) != '\0') &&
              (anim = *slot, anim != 0)) {
            *(unsigned char *)(anim + 0x102) = 0;
            *(unsigned char *)(anim + 0x103) = 1;
            *(unsigned char *)(anim + 0x101) = 0;
        }
        offset = (int)(offset + 0x44);
        slot = (int *)(slot + 1);
    } while (offset < 0x4c8);
    return;
}

}  // namespace seed_0044d380


// int BuildingClass::unknown_slot292() (0x0044D3D0, slot 292)
// Always 0 here.

int BuildingClass::unknown_slot292()
{
    return 0;
}


// BuildingClass::Turn_On 0x0044D3E0 (TibSun building.cpp Turn_On): powers the building on.
void BuildingClass::unknown_44d3e0()
{
    if(!unknown_530 && unknown_3fc==0) {
        unknown_530=true;
        unknown_75=true;
        House->unknown_5668=true;
        House->unknown_5669=true;
        House->unknown_144=true;
        unknown_44d600();

        if(Type->Factory) {
            House->unknown_func_4f3170(Type->Factory,Type->Naval,0);
        }

        if(House->Is_Player_Control()) {
            Speech_Play_Name("EVA_BuildingOnLine",-1,-1);
        }
    }
}


// void BuildingClass::unknown_44d530() (0x0044D530)
// Powers the building off: when it is on (unknown_530) and its type has a
// power drain (+0xC6C above 0) or is Powered, clears unknown_530, sets
// unknown_75, flags the house (+0x5668, +0x5669, +0x144), runs
// unknown_44d6c0, announces EVA_BuildingOffLine to the player's house, tells
// the house its factory type is gone (0x004F3170) and, for the rules' type
// at +0x7BC, 0x004EEB90.

void BuildingClass::unknown_44d530()
{
    if (unknown_530 && (Type->PowerDrain_c6c > 0 || Type->Powered)) {
        unknown_530 = false;
        unknown_75 = true;
        House->unknown_5668 = true;
        House->unknown_5669 = true;
        House->unknown_144 = true;
        unknown_44d6c0();
        if (House->Is_Player_Control()) {
            Speech_Play_Name("EVA_BuildingOffLine", -1, -1);
        }
        if (Type->Factory) {
            House->unknown_func_4f3170(Type->Factory, Type->Naval, 0);
        }
        if (Type == Rules->unknown_7bc) {
            House->unknown_func_4eeb90();
        }
    }
}


// BuildingClass::Power_On 0x0044D600 (TibSun building.cpp Power_On): back onto the power grid.
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house);
void BuildingClass::unknown_44d600()
{
    unknown_5ba=true;
    Building_Refresh_House_Buildings_0044fa40(House);

    if(unknown_4e8) {
        unknown_4e8->Activate(0);
    }

    if(Type->LaserFencePost) {
        unknown_44e5a0(0);
    }

    int anim=0;
    do {
        if(Type->Anims[anim].Powered && unknown_454[anim])
            ((AnimClass *)unknown_454[anim])->unknown_func_422c20();
    } while(++anim<18);
}


// void BuildingClass::unknown_44d6c0() (0x0044D6C0)
// Clears unknown_5ba and refreshes the house's active buildings (0x0044F8E0,
// after 0x0044E5A0 for a laser fence post); hands 0 to the owned object at
// +0x4E8; a cloak generator type sets unknown_5bb to -1, takes the type's
// CloakRadiusInCells into an unset unknown_5bc and sets unknown_75; a gap
// generator type with +0x1FD set runs slot 236; then the laser field and
// 0x00422C10 on each animation whose Anims entry is Powered.

void BuildingClass::unknown_44d6c0()
{
    HouseClass *house = House;
    unknown_5ba = false;
    int count = house->Buildings.Count();
    for (int index = 0; index < count; index++) {
        BuildingClass *building = house->Buildings[index];
        if (building->unknown_7d && !building->unknown_76) {
            if (building->Type->LaserFencePost) {
                building->unknown_44e5a0(0);
            }
            building->unknown_44f8e0();
        }
    }
    if (unknown_4e8) {
        unknown_4e8->Deactivate(0);
    }
    if (Type->CloakGenerator) {
        unknown_5bb = -1;
        if (!unknown_5bc) {
            unknown_5bc = Type->CloakRadiusInCells;
        }
        unknown_75 = true;
    }
    if (Type->GapGenerator && unknown_1fd) {
        unknown_slot236();
    }
    if (Type->LaserFencePost) {
        unknown_44e5a0(0);
    }
    for (int anim = 0; anim < 18; anim++) {
        if (Type->Anims[anim].Powered && unknown_454[anim]) {
            ((UnknownBuildingAnim *)unknown_454[anim])->unknown_422c10();
        }
    }
}


// bool BuildingClass::unknown_44d7d0() (0x0044D7D0)
// True for a type that is not a gate. A gate outside mission 24, or whose
// +0x2A8 state is not settled (0x00496AB0 or 0x00496B50), is given mission
// 24 again (slots 118, 116, 117) and the answer is false; otherwise the
// answer of unknown_44d880.

bool BuildingClass::unknown_44d7d0()
{
    if (!Type->Gate) {
        return true;
    }
    if (Get_Mission() != 24 || unknown_2a8.Is_Count_Down_Active() || unknown_2a8.Is_Count_Down_Complete()) {
        Set_Mission(-1);
        QueueMission(24, false);
        Commence();
        return false;
    }
    return unknown_44d880();
}


// bool BuildingClass::unknown_44d880() (0x0044D880)
// True for a type that is not a gate, or for a gate in mission 24 whose
// +0x2A8 state reports done (0x00496B30).

bool BuildingClass::unknown_44d880()
{
    if (!Type->Gate) {
        return true;
    }
    if (Get_Mission() == 24 && unknown_2a8.Is_Count_Up_Complete()) {
        return true;
    }
    return false;
}


// bool BuildingClass::Can_Repair() (0x0044D8C0, slot 36)
// False without strength, without the type flag at +0x121D or when slot 31
// holds; otherwise the techno answer.

bool BuildingClass::Can_Repair()
{
    if (!Strength) {
        return false;
    }
    if (!Type->ClickRepairable) {
        return false;
    }
    if (unknown_slot31()) {
        return false;
    }
    return TechnoClass::Can_Repair();
}


// bool BuildingClass::unknown_44d900(BuildingTypeClass const *upgrade, HouseClass *house) (0x0044D900)
// Whether the upgrade type (named in its PowersUpBuilding at +0xC20) may
// power up this building for the house: the house must own it, the name
// must be the building's, and PowersUpToLevel allows another upgrade
// (-1: fewer than the type's Upgrades; 1 to 3: none yet).

bool BuildingClass::unknown_44d900(BuildingTypeClass const *upgrade, HouseClass *house)
{
    if (house == House && _stricmp(upgrade->PowersUpBuilding, Type->ID) == 0) {
        int level = upgrade->PowersUpToLevel;
        if (level != -1) {
            if (level <= 0 || level > 3) {
                return false;
            }
        } else if (unknown_5c6 < Type->Upgrades) {
            return true;
        }
        if (unknown_5c6 == 0) {
            return true;
        }
    }
    return false;
}


// WeaponStruct *BuildingClass::unknown_slot231(int index) (0x0044D980, slot 231)
// The first weapon at index with a weapon type among the unknown_5c6 types
// in unknown_4c0; otherwise the techno weapon.

WeaponStruct *BuildingClass::unknown_slot231(int index)
{
    if (unknown_5c6) {
        for (int i = 0; i < unknown_5c6; i++) {
            if (unknown_4c0[i]) {
                WeaponStruct *weapon = unknown_4c0[i]->Get_Weapon(index);
                if (weapon->WeaponType) {
                    return weapon;
                }
            }
        }
    }
    return TechnoClass::unknown_slot231(index);
}


// bool BuildingClass::unknown_slot232() (0x0044D9D0, slot 232)
// True when the type, or any of the unknown_5c6 types in unknown_4c0, has
// a turret.

bool BuildingClass::unknown_slot232()
{
    if (Type->Turret) {
        return true;
    }
    if (unknown_5c6) {
        for (int i = 0; i < unknown_5c6; i++) {
            if (unknown_4c0[i] && unknown_4c0[i]->Turret) {
                return true;
            }
        }
    }
    return false;
}


// void BuildingClass::unknown_44da20(InfantryClass *infantry) (0x0044DA20)
// Removes the infantry from unknown_53c and adds it at the end again.

void BuildingClass::unknown_44da20(InfantryClass *infantry)
{
    unknown_53c.Delete(infantry);
    unknown_53c.Add(infantry);
}


// bool BuildingClass::unknown_44daa0(ObjectClass *object) (0x0044DAA0)
// Whether the distance from the building's centre to the object's is at
// most the type's PsychicDetectionRadius in cells (256 leptons each).

bool BuildingClass::unknown_44daa0(ObjectClass *object)
{
    CoordStruct here = Center_Coord();
    CoordStruct there = object->Center_Coord();
    return here.Distance_To(there) <= Type->PsychicDetectionRadius << 8;
}


// bool BuildingClass::unknown_44db40(CellClass *cell) (0x0044DB40)
// Whether the distance from the building's centre to the cell's centre is
// at most the type's PsychicDetectionRadius in cells (256 leptons each).

bool BuildingClass::unknown_44db40(CellClass *cell)
{
    CoordStruct here = Center_Coord();
    CoordStruct there = cell->unknown_475d60();
    return here.Distance_To(there) <= Type->PsychicDetectionRadius << 8;
}


// void BuildingClass::unknown_44dbe0() (0x0044DBE0)
// Outside a movie (unknown_a40774 clear), a laser fence post that is active
// (+0x7D) and not selected out (+0x76 clear) refreshes its four directions
// (0044DFC0 with 0, 2, 4, 6) and then the field (0044E5A0).

void BuildingClass::unknown_44dbe0()
{
    if (!unknown_a40774 && Type->LaserFencePost && unknown_7d && !unknown_76) {
        unknown_44dfc0(0);
        unknown_44dfc0(2);
        unknown_44dfc0(4);
        unknown_44dfc0(6);
        unknown_44e5a0(1);
    }
}


// void BuildingClass::unknown_44dc40(int explode) 0x0044DC40 (TibSun building.cpp
// Update_Laser_Fence_Connections): records which sides of a laser fence post carry fence of
// its owner and removes those runs (0x0044E260 Disconnect_Laser_Fence).
#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
#include "shared/adjacent_cell.hpp"
inline Cell Adjacent_Cell(Cell const &cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}
#endif
void BuildingClass::unknown_44dc40(int explode)
{
    if (!unknown_a40774 && Type->LaserFencePost) {
        bool do_explode = false;
        if (explode == (int)true) {
            do_explode = true;
        }
        int dir = 0;
        Cell origin = Cell(Center_Coord());

        static int _facing_bits[] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3 };

        for (int i = 0; i < 4; i++) {
            Cell adjacent = Adjacent_Cell(origin, dir);
            BuildingClass *building = (BuildingClass *)Map[adjacent].Cell_Building();
            if (building != 0 && building->Type->LaserFence) {
                if (Owner() == building->Owner()) {
                    if (((((*(unsigned const *)&building->unknown_2e0.Current()) >> 12) + 1) >> 1 & 3) == (dir & 3)) {
                        unknown_4ec |= _facing_bits[i];
                        unknown_44e260(dir, do_explode);
                    }
                }
            }
            dir = (dir + 2) & 7;
        }
    }
}


// Find a same-owner fence post along the requested cell direction.
BuildingClass *BuildingClass::unknown_44ddb0(int facing, bool through_fences, int max_range)
{
    int range=1;
    if(max_range<0) {
        if(Type->LaserFencePost) {
            range=Type->GuardRange.Value>>8;
            if(range<=1) range=1;
        }
    } else range=max_range;
    int direction=facing&3;
    Cell cell=Coord_Cell(Center_Coord());
    BuildingClass *result=0;
    for(int index=0;index<range;++index) {
        Cell const &offset=AdjacentCell[facing&7];
        Cell next=Cell((short)(offset.X+cell.X),(short)(offset.Y+cell.Y));
        cell=next;
        BuildingClass *building=static_cast<BuildingClass *>(Map[cell].Cell_Building());
        if(building) {
            if(building->Type->LaserFencePost && Owner()==building->Owner()) result=building;
            if(!through_fences || !building->Type->LaserFence || Owner()!=building->Owner()
               || (Dir_To_8(building->unknown_2e0.Current())&3)!=direction) break;
        }
    }
    return result;
}


// Refresh all live laser-fence posts in the global building list.
void __fastcall BuildingClass::unknown_44df40()
{
    for(int index=0;index<unknown_BuildingClass_list_00a40af8.Count();++index) {
        BuildingClass *building=unknown_BuildingClass_list_00a40af8[index];
        if(building && building->Type->LaserFencePost && unknown_a40774==0 && building->unknown_7d && !building->unknown_76) {
            building->unknown_44dfc0(0);
            building->unknown_44dfc0(2);
            building->unknown_44dfc0(4);
            building->unknown_44dfc0(6);
            building->unknown_44e5a0(1);
        }
    }
}


// void BuildingClass::unknown_44dfc0(int dir) 0x0044DFC0 (TibSun building.cpp Connect_Laser_Fence):
// fills the gap toward the friendly post in dir (0x0044DDB0) with laser fence sections cell by cell,
// tearing the run down (0x0044E260) when one cannot be placed, then burns tiberium and veins under it.
#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
#include "shared/adjacent_cell.hpp"
inline Cell Adjacent_Cell(Cell const &cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}
#endif
void BuildingClass::unknown_44dfc0(int dir)
{
    BuildingClass *building;

    if (!unknown_a40774 && Type->LaserFencePost) {
        int range;
        if (Type->LaserFencePost) {
            range = Type->GuardRange.Value >> 8;
            range = range > 1 ? range : 1;
        } else {
            range = 1;
        }

        BuildingClass *post = unknown_44ddb0(dir, false, -1);
        if (post != 0 && post->Get_Mission() != 19) {
            int fenceid;
            for (fenceid = 0; fenceid < BuildingTypes.Count(); fenceid++) {
                if (BuildingTypes[fenceid]->LaserFence) {
                    break;
                }
            }

            if (fenceid < BuildingTypes.Count()) {
                bool connected = false;
                Cell working_cell = Cell(Center_Coord());
                while (range > 0) {
                    working_cell = Adjacent_Cell(Cell(working_cell.X, working_cell.Y), dir);
                    building = (BuildingClass *)Map[working_cell].Cell_Building();
                    if (building == post) {
                        connected = true;
                        break;
                    }

                    building = (BuildingClass *)BuildingTypes[fenceid]->Create_One_Of(Owning_House());
                    if (!building->Unlimbo(&CoordStruct(working_cell), Dir_To_256(Facing_Dir(dir & 3)))) {
                        delete building;
                        break;
                    }
                }
                if (!connected) {
                    unknown_44e260(dir, false);
                    return;
                }

                working_cell = Cell(Center_Coord());
                    do {
                        CellClass *cptr = &Map[working_cell];
                        if (cptr->Tiberium_Type_Here() != -1) {
                            cptr->unknown_475db0(12);
                        }
                        if (cptr->OverlayIndex == 0x7E) {
                            cptr->unknown_47c040();
                        }
                        working_cell = Cell(static_cast<TPoint2D<short> const &>(AdjacentCell[dir & 7]).operator+(working_cell));
                        building = (BuildingClass *)Map[working_cell].Cell_Building();
                    } while (building != post);
            }
        }
    }
}

// External names checked against complete original operands.


// Remove or destroy the matching laser-fence segments toward a neighboring post.
inline Cell Adjacent_Cell(Cell const &cell, int facing);
void BuildingClass::unknown_44e260(int facing, bool explode)
{
    if (Type->LaserFencePost) {
        int range = Type->GuardRange.Value >> 8;
        if (range <= 1) range = 1;
        unsigned axis = (unsigned)facing % 4;
        Cell cell = Coord_Cell(Center_Coord());
        for (int index = 0; index < range; ++index) {
            cell = Adjacent_Cell(cell, facing);
            BuildingClass *segment = static_cast<BuildingClass *>(Map[cell].Cell_Building());
            if (segment) {
                if (!segment->Type->LaserFence) return;
                if (Owner() != segment->Owner()) return;
                if ((unsigned)(Dir_To_8(segment->unknown_2e0.Current()) & 3) != axis) return;
                if (explode) {
                    Unknown_0047F600(segment->Center_Coord(), segment->Strength, Rules->unknown_c74, 0, 0);
                    segment->Take_Damage(&segment->Strength, 0, Rules->unknown_c74, 0, true, 1);
                } else {
                    segment->Delete_Me();
                }
            }
        }
    }
}


// PRIVATE Building family free cell traversal; callee signature is a hypothesis.
extern Cell const AdjacentCell[8];
inline int Building_Direction_To_4_0044e440(UnknownFacingDir const &direction)
{
    return (((*(unsigned int const *)&direction >> 12)+1)>>1)&3;
}
inline Cell Building_Adjacent_Fence_Cell_0044e440(Cell const &cell,int facing)
{
    Cell const &offset=AdjacentCell[facing&7];
    return Cell(offset.X+cell.X,offset.Y+cell.Y);
}
void __fastcall Building_Refresh_Fence_Cell_0044e440(Cell const &origin)
{
    CellClass *cell=&Map[origin];
    if(!cell)return;
    BuildingClass *fence=static_cast<BuildingClass *>(cell->Cell_Building());
    if(!fence || !fence->Type->LaserFence)return;
    int facing=Building_Direction_To_4_0044e440(fence->unknown_2e0.Current());
    BuildingClass *found=0;
    Cell previous=origin;
    Cell next=Building_Adjacent_Fence_Cell_0044e440(previous,facing);
    Cell const &probe=next;
    while(probe!=previous) {
        found=static_cast<BuildingClass *>(Map[probe].Cell_Building());
        if(!found)goto destroy;
        if(found->Type->LaserFencePost) {
            found->unknown_44e260(facing|4,false);
            break;
        }
        if(!found->Type->LaserFence)goto destroy;
        previous=probe;
        next=Building_Adjacent_Fence_Cell_0044e440(probe,facing);
    }
    if(found)return;
destroy:
    fence->Delete_Me();
}


// 0x0044E570: clear the laser-fence post DWORD at +4EC when the global gate is zero.
// Free/static ownership and the original developer name remain unknown.

void __fastcall Building_Reset_Fence_Progress_0044e570(BuildingClass *building)
{
    if(!unknown_a40774 && building->Type->LaserFencePost)
        building->unknown_4ec=0;
}


// Refresh laser-fence segment frames and apply damage along connected active fields.
void BuildingClass::unknown_44e5a0(int force)
{
    if(Type->LaserFencePost && unknown_7d && !unknown_76) {
        int range=Type->GuardRange.Value>>8;
        if(range<=1) range=1;
        bool powered=false;
        if(unknown_slot195() && unknown_5ba && Get_Mission()!=19 && Get_Mission()!=18)
            powered=true;
        unsigned facing=0;
        int remaining=4;
        do {
            Cell position=AdjacentCell[facing&7]+Coord_Cell(Center_Coord());
            Cell const &offset=AdjacentCell[facing&7];
            Cell current=position;
            BuildingClass *segment=static_cast<BuildingClass *>(Map[current].Cell_Building());
            unsigned axis=facing&3;
            if(segment && segment->Type->LaserFence && Owner()==segment->Owner()
               && (Dir_To_8(segment->unknown_2e0.Current())&3)==axis) {
                BuildingClass *first=segment;
                BuildingClass *last=segment;
                {
                    if(!(unsigned char)force) {
                        if(powered) {
                            if(segment->unknown_4ec<8) goto next_direction;
                        } else if(segment->unknown_4ec>=8) goto next_direction;
                    }
                    BuildingClass *post=0;
                    if(powered) post=unknown_44ddb0(facing,true,-1);
                    int first_frame=12;
                    int middle_frame=12;
                    int last_frame=12;
                    if(axis==2) { first_frame=8; middle_frame=8; last_frame=8; }
                    bool connected=false;
                    if(powered && post && post->unknown_slot195() && post->unknown_5ba
                       && post->Get_Mission()!=19 && post->Get_Mission()!=18) {
                        connected=true;
                        switch(facing) {
                        case 0: first_frame=5; middle_frame=4; last_frame=6; break;
                        case 4: first_frame=6; middle_frame=4; last_frame=5; break;
                        case 2: first_frame=1; middle_frame=0; last_frame=2; break;
                        case 6: first_frame=2; middle_frame=0; last_frame=1; break;
                        }
                    }
                    int distance=0;
                    while(segment && distance<range) {
                        if(!segment->Type->LaserFence) break;
                        if(Owner()!=segment->Owner()
                           || (Dir_To_8(segment->unknown_2e0.Current())&3)!=axis) break;
                        last=segment;
                        if(segment==first) segment->unknown_4ec=first_frame;
                        else segment->unknown_4ec=middle_frame;
                        segment->unknown_75=true;
                        if(connected) {
                            ObjectClass *object=Map[current].OccupierPtr;
                            while(object) {
                                int kind=object->Kind_Of();
                                if(kind>0 && (kind<=2 || kind==15)) {
                                    ObjectClass *next=object->unknown_30;
                                    if(object->Strength>0)
                                        object->Take_Damage(&object->Strength,0,Rules->unknown_c74,segment,true,1);
                                    object=next;
                                } else object=object->unknown_30;
                            }
                        }
                        ++distance;
                        Cell next=current+offset;
                        current=next;
                        segment=static_cast<BuildingClass *>(Map[current].Cell_Building());
                    }
                    if(connected && last) {
                        if(first==last) {
                            if(axis==2) last->unknown_4ec=3;
                            else last->unknown_4ec=7;
                        } else last->unknown_4ec=last_frame;
                        last->unknown_75=true;
                    }
                }
            }
next_direction:
            facing=(facing+2)&7;
        } while(--remaining);
    }
}


// Previous fire coordinate, with garrison, dual-pixel and voxel animation offsets.
static inline CoordStruct Add_Pixel_Offset(CoordStruct &coord,Point2D const &point);
CoordStruct BuildingClass::unknown_slot43(int weapon)
{
    if(Type->CanBeOccupied && unknown_554.Count()>0) {
        CoordStruct offset(unknown_tactical_83996c->Unknown_006A2900(Type->MuzzleFlash[unknown_56c]),0);
        return unknown_slot42()+offset;
    } else {
        Point2D const &pixel=Type->PrimaryFirePixelOffset;
        if(Fire_Pixels_Differ_0043bbd0(pixel,Point2D(65535,65535))) {
            if(Type->PrimaryFireDualOffset) {
                CoordStruct offset(unknown_tactical_83996c->Unknown_006A2900(pixel),0);
                return TechnoClass::unknown_slot43(weapon)+offset;
            } else {
                CoordStruct offset(unknown_tactical_83996c->Unknown_006A2900(pixel),0);
                return unknown_slot42()+offset;
            }
        } else {
            if(Type->BarrelAnimIsVoxel) return unknown_44ed60(weapon,true);
            CoordStruct coord=TechnoClass::unknown_slot43(weapon);
            if(Type->TurretAnimIsVoxel) {
                Point2D turret(Type->Anims[9].Offset.X,Type->Anims[9].Offset.Y);
                coord=Add_Pixel_Offset(coord,unknown_tactical_83996c->Unknown_006A2900(turret));
            }
            return coord;
        }
    }
}


// Resolve a building fire coordinate from garrison, pixel, or voxel-barrel offsets.
static inline CoordStruct Add_Pixel_Offset(CoordStruct &coord,Point2D const &point);
CoordStruct BuildingClass::unknown_slot181(int weapon)
{
    if(Type->CanBeOccupied && unknown_554.Count()>0) {
        CoordStruct offset=Projected_Coord_0043bbd0(unknown_tactical_83996c->Unknown_006A2900(Type->MuzzleFlash[unknown_56c]));
        return unknown_slot42()+offset;
    } else {
        Point2D const &pixel=Type->PrimaryFirePixelOffset;
        if(Fire_Pixels_Differ_0043bbd0(pixel,Point2D(65535,65535))) {
            CoordStruct offset=Projected_Coord_0043bbd0(unknown_tactical_83996c->Unknown_006A2900(pixel));
            return unknown_slot42()+offset;
        }
        if(Type->BarrelAnimIsVoxel) return unknown_44ed60(weapon,false);
        CoordStruct coord=TechnoClass::unknown_slot181(weapon);
        if(Type->TurretAnimIsVoxel) {
            Point2D turret(Type->Anims[9].Offset.X,Type->Anims[9].Offset.Y);
            coord=Add_Pixel_Offset(coord,unknown_tactical_83996c->Unknown_006A2900(turret));
        }
        return coord;
    }
}


// Compute the fire coordinate from the voxel barrel transform and turret pixel offset.
static inline CoordStruct Add_Pixel_Offset(CoordStruct &coord,Point2D const &point)
{ coord.X+=point.X; coord.Y+=point.Y; return coord; }
CoordStruct BuildingClass::unknown_44ed60(int weapon, bool previous)
{
    WeaponTypeClass *weapon_type=unknown_slot231(weapon)->WeaponType;
    int burst;
    if(previous) {
        if(unknown_310==0) burst=weapon_type->Burst-1;
        else burst=unknown_310-1;
    } else burst=unknown_310;
    int lateral;
    if(burst==0) lateral=Type->VoxelBarrelOffsetToBarrelEnd.Y;
    else if(burst==1) lateral=-Type->VoxelBarrelOffsetToBarrelEnd.Y;
    else lateral=0;
    Vector3 offset((float)Type->VoxelBarrelOffsetToBarrelEnd.X,(float)lateral,(float)Type->VoxelBarrelOffsetToBarrelEnd.Z);
    Vector3 point=unknown_453900()*offset;
    int x=(int)point.X;
    int y=(int)-point.Y;
    int z=(int)point.Z;
    CoordStruct delta(x,y,z);
    CoordStruct result=unknown_slot42()+delta;
    Point2D pixel(Type->Anims[9].Offset.X,Type->Anims[9].Offset.Y);
    return Add_Pixel_Offset(result,unknown_tactical_83996c->Unknown_006A2900(pixel));
}


// void BuildingClass::Set_Occupy_Bit(CoordStruct const &coord) (0x0044EED0, slot 58)
// Sets the building occupation bit of the cell at coord (the ground level's
// +0x124).

void BuildingClass::Set_Occupy_Bit(CoordStruct const &coord)
{
    Map_Cell_At(Coord_Cell(coord))->unknown_124.Occupy.Building = true;
}


// void BuildingClass::Clear_Occupy_Bit(CoordStruct const &coord) (0x0044EF30, slot 59)
// Clears the building occupation bit of the cell at coord (the ground level's
// +0x124).

void BuildingClass::Clear_Occupy_Bit(CoordStruct const &coord)
{
    Map_Cell_At(Coord_Cell(coord))->unknown_124.Occupy.Building = false;
}


// HRESULT __stdcall BuildingClass::Load(IStream *stream) (0x0044EF90, slot 5)
// Leaves the techno index, loads the techno, rebuilds itself in place,
// rejoins the index, fixes up pointers and reads the two infantry lists
// (a count, then the pointers).

HRESULT __stdcall BuildingClass::Load(IStream *stream)
{
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    HRESULT result = TechnoClass::Load(stream);
    if (SUCCEEDED(result)) {
        int count;
        new (this) BuildingClass(NoInitClass());
        unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
        Swizzle_Pointer(Type);
        Swizzle_Pointer(unknown_41c);
        Swizzle_Pointer(unknown_438);
        Swizzle_Pointer(unknown_440);
        Swizzle_Pointer(unknown_444);
        Swizzle_Pointer(unknown_4d4);
        unknown_570.Init();
        unknown_584.Init();

        HRESULT read = stream->Read(&count, sizeof(count), 0);
        if (FAILED(read)) {
            return read;
        }
        for (int index = 0; index < count; index++) {
            InfantryClass *infantry;
            read = stream->Read(&infantry, sizeof(infantry), 0);
            if (FAILED(read)) {
                return read;
            }
            unknown_53c.Add(infantry);
        }
        for (index = 0; index < count; index++) {
            Swizzle_Pointer(unknown_53c[index]);
        }
        read = stream->Read(&count, sizeof(count), 0);
        if (FAILED(read)) {
            return read;
        }
        for (index = 0; index < count; index++) {
            InfantryClass *infantry;
            read = stream->Read(&infantry, sizeof(infantry), 0);
            if (FAILED(read)) {
                return read;
            }
            unknown_554.Add(infantry);
        }
        for (index = 0; index < count; index++) {
            Swizzle_Pointer(unknown_554[index]);
        }
        for (index = 0; index < 18; index++) {
            Swizzle_Pointer(unknown_454[index]);
        }
        for (index = 0; index < 3; index++) {
            Swizzle_Pointer(unknown_4c0[index]);
        }
        for (index = 0; index < 8; index++) {
            Swizzle_Pointer(unknown_49c[index]);
        }
        unknown_4e8 = 0;
    }
    return result;
}


// HRESULT __stdcall BuildingClass::Save(IStream *stream, BOOL clear_dirty) (0x0044F2F0, slot 6)
// TechnoClass::Save, then each infantry list as a count and its pointers.

HRESULT __stdcall BuildingClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = TechnoClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        int count = unknown_53c.Count();
        HRESULT write = stream->Write(&count, sizeof(count), 0);
        if (FAILED(write)) {
            return write;
        }
        for (int index = 0; index < count; index++) {
            write = stream->Write(&unknown_53c[index], sizeof(InfantryClass *), 0);
            if (FAILED(write)) {
                return write;
            }
        }
        count = unknown_554.Count();
        write = stream->Write(&count, sizeof(count), 0);
        if (FAILED(write)) {
            return write;
        }
        for (index = 0; index < count; index++) {
            write = stream->Write(&unknown_554[index], sizeof(InfantryClass *), 0);
            if (FAILED(write)) {
                return write;
            }
        }
    }
    return result;
}


// bool BuildingClass::unknown_slot122() (0x0044F3B0, slot 122)
// The flag at +0x5AD.

bool BuildingClass::unknown_slot122()
{
    return unknown_5ad ? true : false;
}


// void BuildingClass::Compute_CRC(WWCRCEngine &crc) const (0x0044F3C0, slot 13)
// The techno CRC, then the timers' remaining time, the plain members, the
// owned and target objects' IDs and the flags.

void BuildingClass::Compute_CRC(WWCRCEngine &crc) const
{
    TechnoClass::Compute_CRC(crc);
    crc(unknown_420.Value());
    crc(unknown_5a0.Value());
    crc((long)unknown_42c);
    crc((long)unknown_430);
    crc((long)unknown_434);
    if (unknown_438 != 0) {
        crc(unknown_438->Fetch_ID());
    }
    crc((long)unknown_43c);
    if (unknown_440 != 0) {
        crc(unknown_440->Fetch_ID());
    }
    crc(unknown_448.Value());
    crc((long)unknown_4d0);
    crc((long)unknown_4ec);
    crc((long)unknown_4f0);
    crc(unknown_530);
    crc(unknown_5ba);
    crc(unknown_59a);
    crc(unknown_59b);
    crc(unknown_5ac);
    crc(unknown_5ad);
    crc(unknown_5b8);
    crc(unknown_5ae);
    crc(unknown_5af);
    crc(unknown_5b0);
    crc(unknown_5b1);
    crc(unknown_5b2);
    crc(unknown_5b3);
    crc(unknown_5b4);
    crc(unknown_59c);
    crc(unknown_5b5);
    crc(unknown_5b6);
    crc((long)(unsigned short)unknown_5c4);
    crc(unknown_5c6);
}


// int BuildingClass::unknown_slot25(bool arg1, int arg2) (0x0044F600, slot 25)
// While +0x5BD is set: 1 or 2 up to 10 (2 above 5); above 10, 3 when the
// house arg2 (a HouseClass *) holds the bit 0x0047C2E0 of this cell (with
// arg1), or, without arg1, when +0x372 or slot 191 holds, there is no main
// window, or the player and the owner are mutual allies in a game mode;
// otherwise 5. Without +0x5BD the techno behaviour.

extern HWND MainWindow;                 // 0x00B263D8

int BuildingClass::unknown_slot25(bool arg1, int arg2)
{
    if (unknown_5bd) {
        if (unknown_5bd > 10) {
            if (arg1) {
                HouseClass *house = (HouseClass *)arg2;
                if (house) {
                    CoordStruct coord = Coord;
                    if (((UnknownCellClass *)&Map[Coord_Cell(coord)])->Is_Sensed(house->ArrayIndex)) {
                        return 3;
                    }
                }
            } else {
                if (unknown_372 || unknown_slot191() || !MainWindow
                    || unknown_game_mode_a3d298 && House && unknown_player_a35db4
                    && unknown_player_a35db4->Is_Ally(House) && House->Is_Ally(unknown_player_a35db4)) {
                    return 3;
                }
            }
            return 5;
        }
        return (unknown_5bd > 5) + 1;
    }
    return TechnoClass::unknown_slot25(arg1, arg2);
}


// void BuildingClass::unknown_44f730() (0x0044F730)
// For each animation slot of the type: a Powered entry restarts its
// animation (0x00422C20); a PoweredLight entry with no animation yet, while
// unknown_5b4 is set (and not the delayed-fire slot 10), creates it in the
// state the health gives.

void BuildingClass::unknown_44f730()
{
    for (int index = 0; index < 18; index++) {
        if (Type->Anims[index].Powered) {
            if (unknown_454[index]) {
                ((UnknownBuildingAnim *)unknown_454[index])->unknown_422c20();
            }
        } else if (Type->Anims[index].PoweredLight && !unknown_454[index] && unknown_5b4) {
            if (!(Type->IsAnimDelayedFire && index == 10)) {
                unknown_44c920(index, Health_Ratio() <= Rules->unknown_1368, false, 0);
            }
        }
    }
}


// void BuildingClass::unknown_44f800() (0x0044F800)
// For each animation slot of the type: a Powered entry stops its animation
// (0x00422C10); a PoweredLight entry removes its animation (unknown_44cfd0)
// and, for the delayed-fire slot 10, restarts animation 3 when that is
// Powered, in the state the health gives.

void BuildingClass::unknown_44f800()
{
    for (int index = 0; index < 18; index++) {
        if (Type->Anims[index].Powered) {
            if (unknown_454[index]) {
                ((UnknownBuildingAnim *)unknown_454[index])->unknown_422c10();
            }
        } else if (Type->Anims[index].PoweredLight) {
            if (unknown_454[index]) {
                unknown_44cfd0(index);
                if (index == 10 && Type->IsAnimDelayedFire && Type->Anims[3].Powered) {
                    unknown_44c920(3, Health_Ratio() <= Rules->unknown_1368, false, 0);
                }
            }
        }
    }
}


// void BuildingClass::unknown_44f8e0() (0x0044F8E0)
// Brings the building's power-dependent state in line with slot 195 (powered
// or not): a cloak generator sets or clears unknown_5bb, a gap generator
// runs slots 235/236 (and resets +0x1FC/+0x539/+0x200 on power loss), and
// a Powered type with a drain that can toggle runs 0x0044F730 on power
// restored or 0x0044F800 on power lost.

void BuildingClass::unknown_44f8e0()
{
    if (unknown_slot195()) {
        if (Type->CloakGenerator) {
            if (unknown_5bb <= 0) {
                char radius = unknown_5bc;
                if (radius != Type->CloakRadiusInCells) {
                    unknown_5bb = 1;
                    if (radius == Type->CloakRadiusInCells) {
                        unknown_5bc = 0;
                    }
                    unknown_75 = true;
                }
            }
        }
        if (Type->GapGenerator && !unknown_1fd) {
            unknown_slot235();
        }
        if (Type->Powered && Type->PowerDrain_c6c > 0 && Type->TogglePower) {
            unknown_44f730();
        }
    } else {
        if (Type->CloakGenerator && unknown_5bb >= 0 && unknown_5bc != 0) {
            unknown_5bb = -1;
            unknown_75 = true;
        }
        if (Type->GapGenerator && unknown_1fd) {
            unknown_slot236();
            if (unknown_1fc) {
                unknown_1fc = false;
                unknown_539 = false;
                House->unknown_5668 = true;
                unknown_200 = Type->GapRadiusInCells;
                unknown_slot235();
            }
        }
        if (Type->Powered && Type->PowerDrain_c6c > 0 && Type->TogglePower) {
            unknown_44f800();
        }
    }
}


// Refresh the live buildings in a house's captured building list.
// Free fastcall versus original class-owned static/member spelling remains unproved.
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house)
{
    int count = house->Buildings.Count();
    for (int index = 0; index < count; ++index) {
        BuildingClass *building = house->Buildings[index];
        if (building->unknown_7d && !building->unknown_76) {
            if (building->Type->LaserFencePost)
                building->unknown_44e5a0(0);
            building->unknown_44f8e0();
        }
    }
}


// void BuildingClass::Enable_Cloak_Generator_0044fa90() (0x0044FA90): TibSun Enable_Cloak_Generator. A cloak
// generator's field starts growing again: state +0x5BB = 1, the radius +0x5BC restarts from 0 when it stood at the
// type's full CloakRadiusInCells, and the building is flagged for redraw. No call site references it: Captured
// 0x00444660 expands it (declared under RA2_BUILDING_CLOAK_GENERATOR, defined by this unit's first row).

void BuildingClass::Enable_Cloak_Generator_0044fa90()
{
    if (Type->CloakGenerator) {
        unknown_5bb = 1;
        if (unknown_5bc == Type->CloakRadiusInCells) {
            unknown_5bc = 0;
        }
        unknown_75 = true;
    }
}


// void BuildingClass::Disable_Cloak_Generator_0044fad0() (0x0044FAD0): TibSun Disable_Cloak_Generator. A cloak
// generator's field starts collapsing: state +0x5BB = -1, the radius +0x5BC starts from the type's full
// CloakRadiusInCells when it stood at 0, and the building is flagged for redraw. No call site references it:
// Captured 0x00444660 expands it (declared under RA2_BUILDING_CLOAK_GENERATOR, defined by this unit's first row).

void BuildingClass::Disable_Cloak_Generator_0044fad0()
{
    if (Type->CloakGenerator) {
        unknown_5bb = -1;
        if (unknown_5bc == 0) {
            unknown_5bc = Type->CloakRadiusInCells;
        }
        unknown_75 = true;
    }
}


// Call techno slot238 on units, infantry and aircraft occupying a cell.
// Free fastcall versus CellClass member source ownership remains a hypothesis.
void __fastcall Cell_Occupants_Unknown_Slot238_0044fb10(CellClass *cell)
{
    for (ObjectClass *object = cell->OccupierPtr; object; object = object->unknown_30) {
        int kind = object->Kind_Of();
        if (kind == 1 || kind == 15 || kind == 2)
            ((TechnoClass *)object)->unknown_slot238();
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void BuildingClass::Cloaking_AI(int fast) 0x0044FB50 (slot 234, Cloaking_AI), from ra2-decomp
// BuildingClass::Cloaking_AI (YR 0x00454DB0, src/building.cpp): steps the translucency of a cloaking or
// uncloaking building, restarts its natural particle system when it is fully visible, and grows or
// shrinks a cloak generator's covered circle one cell of radius per call.
// Name map: Cloak = unknown_1b8, TranslucencyLevel = unknown_5bd, IsToDisplay = unknown_75,
// Set_Anim_Translucency = unknown_44d2d0, ParticleSystems = unknown_25c, CloakGeneratorState = unknown_5bb,
// CurrentCloakRadius = unknown_5bc, CloakingSurface = unknown_cloak_surface_8502a0,
// Is_Cloaked/Cloaked_By/Uncloaked_By = Is/Set/Clear_House_Bit, Cell_Building = Cell_Building,
// IsPowerOnline = unknown_slot195, Should_Uncloak/Is_Ready_To_Cloak = unknown_slot161/160,
// Do_Uncloak/Do_Cloak = unknown_slot252/253, Enable_Cloak_Generator written out inline.
void BuildingClass::Cloaking_AI(int fast)
{
    if (!Type) return;
    Cell center=Cell(Center_Coord());
    if (unknown_1b8==1) {
        if (unknown_5bd<15) {
            ++unknown_5bd;
            if (unknown_5bd==1 || unknown_5bd==6 || unknown_5bd==11) unknown_75=true;
            unknown_44d2d0(unknown_5bd);
            if (unknown_5bd==15) {
                unknown_1b8=2;
                if (unknown_25c[2]) {delete unknown_25c[2];unknown_25c[2]=0;}
            }
        }
    } else if (unknown_1b8==3) {
        if (unknown_5bd>=0) {
            if (unknown_5bd>0) --unknown_5bd;
            if (unknown_5bd==0 || unknown_5bd==5 || unknown_5bd==10) unknown_75=true;
            unknown_44d2d0(unknown_5bd);
            if (unknown_5bd==0) {
                unknown_1b8=0;
                if (!unknown_25c[2] && (Type->NaturalParticleLocation!=unknown_geometry_l)) {
                    CoordStruct offset=Type->NaturalParticleLocation;
                    unknown_25c[2]=new ParticleSystemClass(Type->NaturalParticleSystem,
                        Position_Coord_00450950(*this)+offset,&Map[Position_Coord_00450950(*this)],0,unknown_geometry_l);
                }
            }
        }
    }
    if (unknown_1b8==2 && unknown_slot161()) unknown_slot252(false);
    if (unknown_1b8==0 && unknown_slot160()) unknown_slot253(false);
    if (unknown_5bb!=0 && Type->CloakGenerator) {
        int house=House->ArrayIndex;
        BSurface *surface=unknown_cloak_surface_8502a0;
        int width=surface->Get_Width();
        int half=width/2;
        if (unknown_5bb>0) {
            if (unknown_5bc==Type->CloakRadiusInCells) unknown_5bb=0;
            else {
                ++unknown_5bc;
                surface->Fill(0);
                surface->Fill_Circle(Point2D(half,half),unknown_5bc,surface->Get_Rect(),1);
                char *data=(char *)surface->Lock();
                Cell origin=center-Cell(half,half);
                for (int y=0;y<width;++y) {
                    for (int x=0;x<width;++x) {
                        if (*data++!=0) {
                            Cell current=origin+Cell(x,y);
                            CellClass *cell=&Map[current];
                            if (!cell->Is_House_Bit(house)) {
                                cell->Set_House_Bit(house);
                                Cell_Occupants_Unknown_Slot238_0044fb10(cell);
                                BuildingClass *building=static_cast<BuildingClass *>(cell->Cell_Building());
                                if (building && building->House->ArrayIndex==house && Cell(building->Center_Coord())==current) {
                                    --unknown_5bc;
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        } else if (unknown_5bc==0) unknown_5bb=0;
        else {
            --unknown_5bc;
            surface->Fill(2);
            surface->Fill_Circle(Point2D(half,half),Type->CloakRadiusInCells,surface->Get_Rect(),0);
            if (unknown_5bc!=0) surface->Fill_Circle(Point2D(half,half),unknown_5bc,surface->Get_Rect(),1);
            char *data=(char *)surface->Lock();
            Cell origin=center-Cell(half,half);
            for (int y=0;y<width;++y) {
                for (int x=0;x<width;++x) {
                    if (*data++==0) {
                        Cell current=origin+Cell(x,y);
                        CellClass *cell=&Map[current];
                        if (cell->Is_House_Bit(house)) {
                            cell->Clear_House_Bit(house);
                            Cell_Occupants_Unknown_Slot238_0044fb10(cell);
                            BuildingClass *building=static_cast<BuildingClass *>(cell->Cell_Building());
                            if (building && building->House->ArrayIndex==house && Cell(building->Center_Coord())==current && (char)fast==0) {
                                ++unknown_5bc;
                                return;
                            }
                        }
                    }
                }
            }
            if (unknown_5bc==0) {
                for (int i=0;i<unknown_BuildingClass_list_00a40af8.Count();++i) {
                    BuildingClass *building=unknown_BuildingClass_list_00a40af8[i];
                    if (building->unknown_7d && building!=this && building->Type->CloakGenerator &&
                        building->unknown_slot195() && building->unknown_5bb==0) {
                        Cell diff=center-Cell(building->Center_Coord());
                        int dy=diff.Y;
                        int dx=diff.X;
                        if (dy*dy+dx*dx<4*((Type->CloakRadiusInCells+2)*(Type->CloakRadiusInCells+2))) {
                            if (building->Type->CloakGenerator) {
                                building->unknown_5bb=1;
                                if (building->unknown_5bc==building->Type->CloakRadiusInCells) building->unknown_5bc=0;
                                building->unknown_75=true;
                            }
                            if (building->unknown_5bc!=0) --building->unknown_5bc;
                        }
                    }
                }
            }
        }
    }
}


// bool BuildingClass::unknown_slot195() (0x00450360, slot 195)
// Whether the building is operating: it is on (unknown_530) or has two
// occupants, is not disabled (unknown_3fc), has strength, is not a Powered
// toggling type on a low-power house without two occupants, and, for an
// engineer-captured type, is held (unknown_59c).

bool BuildingClass::unknown_slot195()
{
    if (!unknown_530 && unknown_53c.Count() < 2) {
        return false;
    }
    if (unknown_3fc > 0) {
        return false;
    }
    if (!Strength) {
        return false;
    }
    if (Type->Powered && Type->PowerDrain_c6c > 0 && Type->TogglePower) {
        if (House->Power_Fraction() < 1.0 && unknown_53c.Count() < 2) {
            return false;
        }
    }
    if (Type->NeedsEngineer && !unknown_59c) {
        return false;
    }
    return true;
}


// void BuildingClass::unknown_slot289(Cell cell) (0x00450400, slot 289)
// The cells within the type's CloakRadiusInCells of a cell (this building's
// own cell for the zero cell) for the owning house (0x0047C370): units there
// are told (slot 238).

void BuildingClass::unknown_slot289(Cell cell)
{
    int range = Type->CloakRadiusInCells;
    int range_squared = range * range;
    int house = House->ArrayIndex;
    if (cell == unknown_geometry_k) {
        cell = Coord_Cell(Center_Coord());
    }
    for (int y = -range; y < range; y++) {
        for (int x = -range; x < range; x++) {
            Cell spot = Cell(cell.X + x, cell.Y + y);
            if (x * x + y * y < range_squared) {
                UnknownCellClass *object = Map_Cell_At(spot);
                object->Unknown_0047C370(house);
                for (ObjectClass *occupier = object->unknown_e4; occupier != 0; occupier = occupier->unknown_30) {
                    int kind = occupier->Kind_Of();
                    if (kind == 1 || kind == 15 || kind == 2) {
                        ((TechnoClass *)occupier)->unknown_slot238();
                    }
                }
            }
        }
    }
}


// void BuildingClass::unknown_slot288(Cell cell) (0x00450550, slot 288)
// The cells within the type's SensorsSight of a cell (this building's
// own cell for the zero cell) for the owning house (0x0047C360): units there
// are told (slot 238).

void BuildingClass::unknown_slot288(Cell cell)
{
    if (!unknown_slot195()) {
        return;
    }
    int range = Type->SensorsSight;
    int range_squared = range * range;
    int house = House->ArrayIndex;
    if (cell == unknown_geometry_k) {
        cell = Coord_Cell(Center_Coord());
    }
    for (int y = -range; y < range; y++) {
        for (int x = -range; x < range; x++) {
            Cell spot = Cell(cell.X + x, cell.Y + y);
            if (x * x + y * y < range_squared) {
                UnknownCellClass *object = Map_Cell_At(spot);
                object->Unknown_0047C360(house);
                for (ObjectClass *occupier = object->unknown_e4; occupier != 0; occupier = occupier->unknown_30) {
                    int kind = occupier->Kind_Of();
                    if (kind == 1 || kind == 15 || kind == 2) {
                        ((TechnoClass *)occupier)->unknown_slot238();
                    }
                }
            }
        }
    }
}


// void BuildingClass::unknown_slot291(Cell cell) (0x004506B0, slot 291)
// Marks the cells within the type's DetectDisguiseRange of a cell (this
// building's own cell for the zero cell) for the owning house (0x0047C390).

void BuildingClass::unknown_slot291(Cell cell)
{
    int range = Type->DetectDisguiseRange;
    int range_squared = range * range;
    int house = House->ArrayIndex;
    if (cell == unknown_geometry_k) {
        cell = Coord_Cell(Center_Coord());
    }
    for (int y = -range; y < range; y++) {
        for (int x = -range; x < range; x++) {
            Cell spot = Cell(cell.X + x, cell.Y + y);
            if (x * x + y * y < range_squared) {
                UnknownCellClass *object = Map_Cell_At(spot);
                object->Unknown_0047C390(house);
            }
        }
    }
}


// void BuildingClass::unknown_slot290(Cell cell) (0x004507B0, slot 290)
// While slot 195 holds, marks the cells within the type's DetectDisguiseRange
// of a cell (this building's own cell for the zero cell) for the owning
// house (0x0047C380).

void BuildingClass::unknown_slot290(Cell cell)
{
    if (unknown_slot195()) {
        int range = Type->DetectDisguiseRange;
        int range_squared = range * range;
            int house = House->ArrayIndex;
        if (cell == unknown_geometry_k) {
            cell = Coord_Cell(Center_Coord());
        }
        for (int y = -range; y < range; y++) {
            for (int x = -range; x < range; x++) {
                Cell spot = Cell(cell.X + x, cell.Y + y);
                if (x * x + y * y < range_squared) {
                    UnknownCellClass *object = Map_Cell_At(spot);
                object->Unknown_0047C380(house);
                }
            }
        }
    }
}


// int BuildingClass::unknown_4508c0() (0x004508C0)
// A mask with bit (facing / 2) set for each of the four orthogonal
// neighbours (facings 0, 2, 4, 6) whose cell holds an active FirestormWall
// building that is not selected out.

inline CoordStruct Position_Coord_00450950(ObjectClass const &object) { return object.Coord; }

int BuildingClass::unknown_4508c0()
{
    int mask = 0;
    CoordStruct coord = Position_Coord_00450950(*this);
    UnknownCellClass *cell = Map_Cell(coord);
    for (int facing = 0; facing < 8; facing += 2) {
        BuildingClass *building = cell->Unknown_00476AC0(facing)->Unknown_004717B0();
        if (building && building->Type->FirestormWall && !building->unknown_76 && building->unknown_7d) {
            mask |= 1 << (facing >> 1);
        }
    }
    return mask;
}


// Refresh a Firestorm wall's neighbours, active animation, and crossing damage.
void BuildingClass::Update_Firestorm_Wall_00450950()
{
    bool active = false;
    if (!Type->FirestormWall) return;
    unsigned frame = unknown_4508c0();
    if (House->unknown_142) { frame += 32; active = true; }
    CellClass *cell = &Map[Position_Coord_00450950(*this)];
    if (frame != (unsigned)unknown_4f0) {
        unknown_4f0 = frame;
        unknown_75 = true;
        cell->Recalc_Attributes(-1);
    }
    unsigned connections = frame & 15;
    if (active && connections != 10 && connections != 5 && !unknown_454[10]) {
        AnimClass *animation = new AnimClass(Rules->FirestormActiveAnim, Position_Coord_00450950(*this) - CoordStruct(128,128,0), 1, 0, 0x600, -10);
        unknown_454[10] = animation;
        animation->unknown_179 = unknown_5b7;
    } else if (unknown_454[10]) {
        delete unknown_454[10];
        unknown_454[10] = 0;
    }
    if (active) {
        ObjectClass *object = cell->OccupierPtr;
        while (object) {
            if (object != this && object->Class_Of() && !object->Class_Of()->unknown_209) {
                switch (object->Kind_Of()) {
                case 1: case 2: case 15: {
                    ObjectClass *next = object->unknown_30;
                    unknown_450cb0(object, true);
                    if (next) { object = next; continue; }
                    break;
                }
                }
            }
            object = object->unknown_30;
        }
        CoordStruct center = cell->unknown_475d60();
        for (int x=-2; x<3; ++x) {
            for (int y=-2; y<3; ++y) {
                Cell offset((short)x, (short)y);
                CellClass *neighbor = &Map[offset + cell->MapCoords];
                if (neighbor != cell) {
                    ObjectClass *occupier = neighbor->OccupierPtr;
                    while (occupier) {
                        ObjectClass *next = occupier->unknown_30;
                        bool is_foot;
                        if (occupier) is_foot = occupier->AbstractFlags.IsFoot;
                        if (occupier && is_foot && static_cast<FootClass *>(occupier)->Locomotor.Checked()->Is_Moving_Here(center) && !occupier->Techno_Type_Class()->unknown_209) {
                            int damage = occupier->Strength;
                            occupier->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 1);
                        }
                        occupier = next;
                    }
                }
            }
        }
    }
}


// A Firestorm collision: optional warhead damage and an effect at ground/object height.
bool BuildingClass::unknown_450cb0(ObjectClass *object, bool damage)
{
    if(object && object->Strength > 0) {
        if(damage) {
            object->Take_Damage(&object->Strength, 0, Rules->unknown_cac, 0, true, 1);
        }
        if(object->Get_Height_AGL() > 100) {
            new AnimClass(Rules->FirestormAirAnim, CoordStruct(object->Coord), 0, 1, 0x600, -10);
        } else {
            new AnimClass(Rules->FirestormGroundAnim, CoordStruct(Coord), 0, 1, 0x600, -10);
        }
        return true;
    }
    return false;
}


// Cache drawing bounds for the current coordinate and tactical view position.
inline Rect Offset_Draw_Bounds(Rect const &bounds,Point2D const &pixel) { Point2D origin(bounds.X,bounds.Y); Point2D shifted=Point_Sum(origin,pixel); return Rect(shifted.X,shifted.Y,bounds.Width,bounds.Height); }
Rect BuildingClass::Get_Render_Rect()
{
    CoordStruct position=unknown_slot42();
    Point2D view(unknown_tactical_83996c->TacPixelX, unknown_tactical_83996c->TacPixelY);
    Rect result;
    if(position==unknown_51c && view==unknown_528) {
        result=unknown_50c;
    } else {
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(position,pixel);
        result=Offset_Draw_Bounds(Type->unknown_func_45e290(),pixel);
        unknown_51c=position;
        unknown_528=view;
        unknown_50c=result;
    }
    return result;
}


// void BuildingClass::Assign_Destination(AbstractClass *target, bool immediate) (0x00450F00, slot 260)
// Ignored in mission 19; otherwise records the target (0x006D6E50) when slot 153
// or the type flag +0x1353 holds, then the techno behaviour.

void BuildingClass::Assign_Destination(AbstractClass *target, bool immediate)
{
    if (Mission != 19) {
        if (unknown_slot153() || Type->ConstructionYard) {
            unknown_6d6e50(target);
        }
        TechnoClass::Assign_Destination(target, immediate);
    }
}


// bool BuildingClass::unknown_slot153() (0x00450F50, slot 153)
// True for type values 40, 16 and 3 at +0xC44 or either type flag +0x134B,
// +0x1349.

bool BuildingClass::unknown_slot153()
{
    int value = Type->Factory;
    if (value == 40 || value == 16 || value == 3 || Type->Cloning || Type->UnitRepair) {
        return true;
    }
    return false;
}


// Building-family nearby radio-acceptor search; free/static spelling remains a hypothesis.
// Genuine local-static maximum/guard retain original storage; actual generated-name bindings are audited separately.
// PRIVATE free-function ownership and selection name hypotheses.
BuildingClass *__fastcall Building_Find_Nearby_Radio_Acceptor_00450f90(HouseClass *house,TechnoClass *target)
{
    BuildingClass *selected=0;
    int best=0x7fffffff;
    static int maximum=(int)(unknown_geometry_diagonal*20.0);
    int count=house->unknown_80.Count();
    for(int index=0;index<count;++index) {
        BuildingClass *building=house->unknown_80[index];
        if(target->unknown_slot150(15,building)) {
            CoordStruct from=target->Coord;
            CoordStruct to=building->Coord;
            CoordStruct delta((int)((unsigned int)to.X-(unsigned int)from.X),
                              (int)((unsigned int)to.Y-(unsigned int)from.Y),
                              (int)((unsigned int)to.Z-(unsigned int)from.Z));
            double y=delta.Y,z=delta.Z,x=delta.X;
            int distance=(int)unknown_sqrt_4ba410(x*x+y*y+z*z);
            if(distance<best && distance<maximum) {selected=building;best=distance;}
        }
    }
    return selected;
}


// Reserve the owning house's footprint and update the base perimeter cells (004510D0).
void BuildingClass::Reserve_Base_Area_004510d0(bool skip_inner_cells)
{
    int spacing = Rules->AIBaseSpacing;
    int width = Type->Width() + spacing * 2;
    int height = Type->Foundation_Height(false) + spacing * 2;
    unsigned owner = 1U << House->ArrayIndex;
    CoordStruct position(Coord);
    Cell top_left = Cell(position) - Cell(spacing, spacing);
    for (int x = top_left.X; x < top_left.X + width; ++x) {
        for (int y = top_left.Y; y < top_left.Y + height; ++y) {
            Map[Cell(x, y)].unknown_dc |= owner;
        }
    }
    Rect &area = House->unknown_55f0.unknown_54;
    if (area.X == 0) area.X = top_left.X;
    if (area.Y == 0) area.Y = top_left.Y;
    if (top_left.X < area.X) {
        int old_x = area.X;
        area.X = top_left.X;
        area.Width += old_x - top_left.X;
    }
    if (top_left.X + width > area.Width + area.X) area.Width = top_left.X - area.X + width;
    if (top_left.Y < area.Y) {
        int old_y = area.Y;
        area.Y = top_left.Y;
        area.Height += old_y - top_left.Y;
    }
    if (top_left.Y + height > area.Height + area.Y) area.Height = top_left.Y - area.Y + height;
    if (!skip_inner_cells) {
        for (int cell_x = top_left.X - 1; cell_x < top_left.X + width + 2; ++cell_x) {
            for (int cell_y = top_left.Y - 1; cell_y < top_left.Y + height + 2; ++cell_y) {
                Cell cell = Cell(cell_x, cell_y);
                int mask = Map[cell].Occupation_Mask(House->ArrayIndex);
                if (mask == 255) House->unknown_55f0.unknown_20.Delete(cell);
                else if (mask > 0) {
                    if (House->unknown_55f0.unknown_20.ID(cell) == -1) House->unknown_55f0.unknown_20.Add(cell);
                }
            }
        }
    }
}


// Release the footprint, reapply neighbouring reservations, then rebuild perimeter cells (004513B0).
void BuildingClass::unknown_4513b0()
{
    int spacing = Rules->AIBaseSpacing;
    int width = Type->Width() + spacing * 2;
    int height = Type->Foundation_Height(false) + spacing * 2;
    unsigned owner = 1U << House->ArrayIndex;
    CoordStruct position(Coord);
    Cell top_left = Cell(position) - Cell(spacing, spacing);
    int x, y;
    for (x = top_left.X; x < top_left.X + width; ++x) {
        for (y = top_left.Y; y < top_left.Y + height; ++y) Map[Cell(x, y)].unknown_dc &= ~owner;
    }
    for (x = top_left.X - spacing; x < top_left.X + width + spacing * 2; ++x) {
        for (y = top_left.Y - spacing; y < top_left.Y + height + spacing * 2; ++y) {
            BuildingClass *building = (BuildingClass *)Map[Cell(x, y)].Cell_Building();
            if (building && building != this) building->Reserve_Base_Area_004510d0(true);
        }
    }
    for (x = top_left.X - 1; x < top_left.X + width + 2; ++x) {
        for (y = top_left.Y - 1; y < top_left.Y + height + 2; ++y) {
            Cell cell = Cell(x, y);
            int mask = Map[cell].Occupation_Mask(House->ArrayIndex);
            if (mask == 255) House->unknown_55f0.unknown_20.Delete(cell);
            else if (mask > 0) {
                if (House->unknown_55f0.unknown_20.ID(cell) == -1) House->unknown_55f0.unknown_20.Add(cell);
            } else {
                House->unknown_55f0.unknown_20.Delete(cell);
                Map[cell].unknown_dc &= ~owner;
            }
        }
    }
}


// int BuildingClass::unknown_451710() (0x00451710)
// The type's +0x11D8 when positive, else the first of the three upgrades
// (unknown_4c0) whose own value is positive; 0 when none.

int BuildingClass::unknown_451710()
{
    int value = Type->unknown_11d8;
    if (value > 0) {
        return value;
    }
    for (int index = 0; index < 3; index++) {
        if (unknown_4c0[index] && ((BuildingTypeClass *)unknown_4c0[index])->unknown_11d8 > 0) {
            return ((BuildingTypeClass *)unknown_4c0[index])->unknown_11d8;
        }
    }
    return 0;
}


// int BuildingClass::unknown_451760() (0x00451760)
// The type's +0x11DC when positive, else the first of the three upgrades
// (unknown_4c0) whose own value is positive; 0 when none.

int BuildingClass::unknown_451760()
{
    int value = Type->unknown_11dc;
    if (value > 0) {
        return value;
    }
    for (int index = 0; index < 3; index++) {
        if (unknown_4c0[index] && ((BuildingTypeClass *)unknown_4c0[index])->unknown_11dc > 0) {
            return ((BuildingTypeClass *)unknown_4c0[index])->unknown_11dc;
        }
    }
    return 0;
}


// int BuildingClass::unknown_4517b0() (0x004517B0)
// The type's +0x11E0 when positive, else the first of the three upgrades
// (unknown_4c0) whose own value is positive; 0 when none.

int BuildingClass::unknown_4517b0()
{
    int value = Type->unknown_11e0;
    if (value > 0) {
        return value;
    }
    for (int index = 0; index < 3; index++) {
        if (unknown_4c0[index] && ((BuildingTypeClass *)unknown_4c0[index])->unknown_11e0 > 0) {
            return ((BuildingTypeClass *)unknown_4c0[index])->unknown_11e0;
        }
    }
    return 0;
}


// void BuildingClass::unknown_slot264() (0x00451800, slot 264)
// Calls the map's Radar_Track for each foundation offset of the type, then sets
// unknown_37b to true.

void BuildingClass::unknown_slot264()
{
    DynamicVectorClass<Point2D> *offsets = Map.Unknown_006328D0(Type);
    for (int index = 0; index < offsets->Count(); index++) {
        Map.Radar_Track(this, Point_Sum((*offsets)[index], unknown_cell_1a0()));
    }
    unknown_37b = true;
}


// void BuildingClass::unknown_slot265() (0x00451860, slot 265)
// Calls the map's Radar_Untrack for each foundation offset of the type, then sets
// unknown_37b to false.

void BuildingClass::unknown_slot265()
{
    DynamicVectorClass<Point2D> *offsets = Map.Unknown_006328D0(Type);
    for (int index = 0; index < offsets->Count(); index++) {
        Map.Radar_Untrack(this, Point_Sum((*offsets)[index], unknown_cell_1a0()));
    }
    unknown_37b = false;
}


// void BuildingClass::unknown_slot266() (0x004518C0, slot 266)
// Passes each foundation cell of the type (offset by unknown_1a0/1a4) to
// the map's Radar_Pixel.

void BuildingClass::unknown_slot266()
{
    DynamicVectorClass<Point2D> *offsets = Map.Unknown_006328D0(Type);
    for (int index = 0; index < offsets->Count(); index++) {
        Point2D cell;
        cell = Point_Sum((*offsets)[index], unknown_cell_1a0());
        Map.Radar_Pixel(cell);
    }
}


// Radius selection with a shared result and the original signed byte/weapon units.
int BuildingClass::unknown_451930()
{
    int radius=0;
    BuildingTypeClass *type=Type;
    int configured=type->PsychicDetectionRadius;
    if(configured>0) radius=configured;
    else if(type->GapGenerator) {
        char radius_byte=unknown_1fc ? type->SuperGapRadiusInCells : type->GapRadiusInCells;
        radius=radius_byte;
    } else if(type->SensorArray || type->CloakGenerator) radius=type->CloakRadiusInCells;
    else {
        WeaponStruct *weapon=TechnoClass::unknown_slot230();
        if(Unknown_006D8640(weapon) && weapon->WeaponType->unknown_b4>0) radius=weapon->WeaponType->unknown_b4/256;
    }
    return radius;
}


// Draw the building radius, with a timed concentric radial indicator when configured.
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();
void __fastcall Draw_Radial_Indicator_00451BF0(unsigned int radius,unsigned char mode,
    CoordStruct center,RGBClass color,bool filled,bool radius_is_pixels);
void BuildingClass::unknown_slot74(int arg1)
{
    unsigned int radius=(unsigned int)unknown_451930();
    if((int)radius>0 && unknown_530 && House->Is_Player_Control()) {
        if(Type->ConcentricRadialIndicator) {
            static RGBClass white(255,255,255);
            RGBClass color=House->unknown_55e9;
            unsigned int phase=(timeGetTime()>>1)&0x3ff;
            if(!(phase&0x200)) {
                if(!(phase&0x100)) color.Adjust((~phase)&0xff,white);
                Draw_Radial_Indicator_00451BF0(radius,0,Center_Coord(),color,false,false);
            }
            double scaled=(double)radius+0.5;
            unsigned int pixels=(unsigned int)(int)(scaled/unknown_sqrt_4ba410(2.0)*60.0);
            pixels=(pixels*phase)>>10;
            if(pixels>32) Draw_Radial_Indicator_00451BF0(pixels,0,Center_Coord(),House->unknown_55e9,false,true);
        } else {
            Draw_Radial_Indicator_00451BF0(radius,(unsigned char)arg1,Center_Coord(),House->unknown_55e9,true,false);
        }
    }
}


// Actual destructor callback for unknown_slot74's local static white RGBClass.
// Its code is emitted naturally by the complete BuildingClass translation unit.
// Replaces the unidentified seed function at the same legacy source path.


// Draws a building's radius ellipse on the tactical view (0x00451BF0), optionally with four sweeping
// antialiased spokes whose angle follows the game frame. Names and parameter intent are hypotheses.
double __cdecl unknown_sqrt_4ba410(double value);
double __cdecl unknown_math_4ba520(double value);
extern double unknown_radial_intensity_7cf678[4];
void __fastcall Draw_Radial_Indicator_00451BF0(unsigned int radius,unsigned char mode,
    CoordStruct center,RGBClass color,bool filled,bool radius_is_pixels)
{
    int pixels;
    if (radius_is_pixels) {
        pixels = radius;
    } else {
        pixels = (int)(((double)radius + 0.5) / unknown_sqrt_4ba410(2.0) * 60.0);
    }
    Point2D pixel;
    unknown_tactical_83996c->Coord_To_Pixel(center, pixel);
    pixel += Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);
    Rect area = Rect(pixel.X - pixels, pixel.Y - pixels / 2, pixels * 2, pixels);
    Rect visible = Intersect(area, unknown_rect_839630);
    if (visible.Width > 0 && visible.Height > 0) {
        RGBClass black(0, 0, 0);
        RGBClass shade = color;
        if (filled) {
            shade.Adjust(50, black);
        }
        unknown_surface_83995c->Draw_Ellipse(pixel, pixels, (pixels / 2), unknown_rect_839630,
            Build_Hicolor_Pixel(shade.Red, shade.Green, shade.Blue));
        unknown_surface_83995c->Draw_Ellipse(pixel, pixels - 1, (pixels / 2) - 1, unknown_rect_839630,
            Build_Hicolor_Pixel(shade.Red, shade.Green, shade.Blue));
        if (mode) {
            double rx = pixels;
            double ry = (pixels / 2);
            for (int i = 0; i < 4; i++) {
                double angle = (double)(Frame + i) * 0.005;
                angle = angle - (int)(angle * 0.15915494309189535) * 6.283185307179586;
                Point2D end;
                if (fabs(angle - 1.5707963267948966) < 0.001) {
                    end = pixel + Point2D(0, (int)-ry);
                } else if (fabs(angle - 4.71238898038469) < 0.001) {
                    end = pixel + Point2D(0, (int)ry);
                } else {
                    double slope = unknown_math_4ba520(angle);
                    double ry2 = ry * ry;
                    double rx2 = rx * rx;
                    double x = unknown_sqrt_4ba410(1.0 / (slope * slope / ry2 + 1.0 / rx2));
                    double y = unknown_sqrt_4ba410((1.0 - x * x / rx2) * ry2);
                    if (angle > 1.5707963267948966 && angle < 4.71238898038469) {
                        x = -x;
                    }
                    if (angle < 3.141592653589793) {
                        y = -y;
                    }
                    end = pixel + Point2D((int)x, (int)y);
                }

                int top = unknown_rect_839630.Y;
unknown_surface_83995c->Draw_Depth_Antialiased_Line(unknown_rect_839630, pixel - Point2D(unknown_rect_839630.X, top), end - Point2D(unknown_rect_839630.X, top), color,
                    -500, -500, false, true, true, true, (float)unknown_radial_intensity_7cf678[i]);
            }
        }
    }
}


// void BuildingClass::Clicked_As_Target(int arg1) (0x00452060)
// A non-zero argument sets unknown_75; then the techno's slot 79. The
// building's animations (unknown_454) take the slot 115 value and the
// slot 254 value for its cell's +0x10A (adjusted by 0x006D86F0 when slot
// 203 holds; also kept in unknown_5c4) when their type has +0x318 set, and
// all of them take the building's layer (unknown_5bd, 0x10 instead of 0xF
// when slot 25 answers 5) after each animation and once more at the end.

void BuildingClass::Clicked_As_Target(int arg1)
{
    if (arg1 != 0) {
        unknown_75 = true;
    }
    TechnoClass::Clicked_As_Target(arg1);
    ConvertClass *value = unknown_slot115();
    int height = ((UnknownBuildingSlot105 *)Get_Cell_Ptr())->unknown_10a;
    int level = unknown_slot254(height);
    if (unknown_slot203()) {
        level = unknown_6d86f0(level);
    }
    unknown_5c4 = level;
    int frame = (unsigned short)level;
    for (int index = 0; index < 18; index++) {
        UnknownBuildingAnim *anim = (UnknownBuildingAnim *)unknown_454[index];
        if (anim != 0) {
            if (anim->Type != 0 && anim->Type->unknown_318) {
                anim->unknown_bc = value;
                anim->unknown_e4 = frame;
            }
            unknown_44d2d0(unknown_5bd);
        }
    }
    unknown_44d2d0(unknown_5bd);
}


// int BuildingClass::unknown_slot254(int arg1) (0x004521B0, slot 254)
// With bit 1 of +0xD8 set, values above 1500 drop by 500 and the others
// rise by 500.

int BuildingClass::unknown_slot254(int arg1)
{
    if ((FlashCount & 2) == 2) {
        if (arg1 > 1500) {
            return arg1 - 500;
        }
        return arg1 + 500;
    }
    return arg1;
}


// int BuildingClass::unknown_4521e0(int arg1) (0x004521E0)
// The slot 254 value, adjusted by 0x006D86F0 when slot 203 holds (slot 79
// expands the same pair).

int BuildingClass::unknown_4521e0(int arg1)
{
    int value = unknown_slot254(arg1);
    if (unknown_slot203()) {
        value = unknown_6d86f0(value);
    }
    return value;
}


// Radar visibility: discovery, cloak/fog and sensing checks.

bool BuildingClass::unknown_slot190(int *out_kind)
{
    if (!Type->Invisible) {
        if (Type->RadarVisible) return true;
        if (House->Is_Player_Control()) return unknown_373 ? true : false;
        int height = Type->Foundation_Height(false) * 256 - 256;
        int width = Type->Width() * 256 - 256;
        bool shrouded = Map.Is_Shrouded(CoordStruct(Coord))
            && Map.Is_Shrouded(CoordStruct(Coord) + CoordStruct(width, height, 0))
            && unknown_b263d8;
        if (unknown_1b8 != 2 && unknown_5bd != 15 && !unknown_5b7 && !shrouded) return true;
        if (!unknown_slot191()) return false;
        if (!unknown_player_a35db4->Is_Ally(House) && !unknown_5b7 && !shrouded) *out_kind = 1;
        return true;
    }
    return false;
}


// Agent infiltration: disrupt radar/power/super, steal credits or grant technology.
extern bool unknown_flag_8373c2;
// EVA_NewTechnologyAcquired
extern char const unknown_eva_007cfaac[];
// EVA_TechnologyStolen
extern char const unknown_eva_007cfac8[];
// EVA_BuildingInfCashStolen
extern char const unknown_eva_007cfae0[];
// EVA_CashStolen
extern char const unknown_eva_007cfafc[];
// EVA_EnemyBasePoweredDown
extern char const unknown_eva_007cfb0c[];
// EVA_BuildingInfiltrated
extern char const unknown_eva_007cfb28[];
// EVA_PowerSabotaged
extern char const unknown_eva_007cfb40[];
// EVA_BuildingInfRadarSabotaged
extern char const unknown_eva_007cfb54[];
// EVA_RadarSabotaged
extern char const unknown_eva_007cfb74[];

void BuildingClass::Unknown_00452390(HouseClass *house)
{
    bool visible = false;
    if(House->Is_Player_Control() || house->Is_Player_Control()) {
        if(Unknown_0063B5B0(9, unknown_slot104())) visible = true;
    }
    BuildingTypeClass *type = Type;
    if(type->Radar) {
        House->unknown_func_4f5880();
        if(House->Is_Player_Control() && !House->unknown_566a && visible)
            Speech_Play_Name(unknown_eva_007cfb74,-1,-1);
        if(house->Is_Player_Control() && !House->unknown_566a && visible)
            Speech_Play_Name(unknown_eva_007cfb54,-1,-1);
    } else if(type->Power > 0) {
        House->unknown_func_4f5800(Rules->SpyPowerBlackout);
        if(House->Is_Player_Control() && visible) {
            Speech_Play_Name(unknown_eva_007cfb40,-1,-1);
        } else if(house->Is_Player_Control() && visible) {
            Speech_Play_Name(unknown_eva_007cfb28,-1,-1);
            Speech_Play_Name(unknown_eva_007cfb0c,-1,-1);
        }
    } else if(type->SuperWeapon != -1) {
        House->Supers[type->SuperWeapon]->unknown_69ebb0();
        if(House->Is_Player_Control() || (house->Is_Player_Control() && visible))
            Speech_Play_Name(unknown_eva_007cfb28,-1,-1);
    } else {
        for(int index=0;index<Rules->BuildTech.Count();++index) {
            if(Rules->BuildTech[index] == type) {
                if(House->unknown_130 == 0) house->unknown_205 = true;
                else house->unknown_204 = true;
                house->unknown_144 = true;
                if(House->Is_Player_Control() && visible)
                    Speech_Play_Name(unknown_eva_007cfac8,-1,-1);
                if(house->Is_Player_Control() && visible) {
                    Speech_Play_Name(unknown_eva_007cfb28,-1,-1);
                    Speech_Play_Name(unknown_eva_007cfaac,-1,-1);
                }
                goto finished;
            }
        }
        if(type->Storage > 0) {
            int stolen = (int)(House->Available_Money() * Rules->SpyMoneyStealPercent);
            House->Unknown_004E5280(stolen);
            house->Refund_Money(stolen);
            if(House->Is_Player_Control() && visible)
                Speech_Play_Name(unknown_eva_007cfafc,-1,-1);
            if(house->Is_Player_Control() && visible)
                Speech_Play_Name(unknown_eva_007cfae0,-1,-1);
        } else if(type->Factory == 40) {
            house->unknown_207 = true;
            house->unknown_144 = true;
            if(house->Is_Player_Control()) unknown_flag_8373c2 = true;
            if(House->Is_Player_Control() && visible)
                Speech_Play_Name(unknown_eva_007cfac8,-1,-1);
            if(house->Is_Player_Control() && visible) {
                Speech_Play_Name(unknown_eva_007cfb28,-1,-1);
                Speech_Play_Name(unknown_eva_007cfaac,-1,-1);
            }

        } else if(type->Factory == 16) {
            house->unknown_206 = true;
            house->unknown_144 = true;
            if(house->Is_Player_Control()) unknown_flag_8373c2 = true;
            if(House->Is_Player_Control() && visible)
                Speech_Play_Name(unknown_eva_007cfac8,-1,-1);
            if(house->Is_Player_Control() && visible) {
                Speech_Play_Name(unknown_eva_007cfb28,-1,-1);
                Speech_Play_Name(unknown_eva_007cfaac,-1,-1);
            }
        }
    }
finished:
    Mark(2);
}


// Refund/remove each installed upgrade, refresh the house's buildings, then call slot98.
// Original static/member ownership of this ECX-input helper remains a hypothesis.
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house);
void __fastcall Building_Refund_Upgrades_00452710(BuildingClass *building)
{
    while (building->unknown_5c6 > 0) {
        building->House->Refund_Money(building->unknown_4c0[building->unknown_5c6 - 1]
                                      ->unknown_slot46(building->House, true));
        building->unknown_44c860();
        building->House->unknown_5668 = true;
        building->House->unknown_5669 = true;
        Building_Refresh_House_Buildings_0044fa40(building->House);
    }
    building->Sell_Back(1);
}


// bool BuildingClass::unknown_slot31() (0x004527E0, slot 31)
// True when the type undeploys into a unit and its flag at +0x1353 is clear.

bool BuildingClass::unknown_slot31()
{
    if (!Type->UndeploysInto) {
        return false;
    }
    return Type->ConstructionYard ? false : true;
}


// int BuildingClass::unknown_452800() (0x00452800)
// The type's first super weapon index, or -1 while that super weapon's
// auxiliary building is not owned by the house (0x00491D10).

int BuildingClass::unknown_452800()
{
    if (Type->SuperWeapon != -1) {
        BuildingTypeClass *aux = Supers[Type->SuperWeapon]->Type->AuxBuilding;
        if (aux != 0 && !House->unknown_5460.unknown_func_491d10(aux->ArrayIndex)) {
            return -1;
        }
    }
    return Type->SuperWeapon;
}


// int BuildingClass::unknown_452860() (0x00452860)
// The type's second super weapon index, or -1 while that super weapon's
// auxiliary building is not owned by the house (0x00491D10).

int BuildingClass::unknown_452860()
{
    if (Type->SuperWeapon2 != -1) {
        BuildingTypeClass *aux = Supers[Type->SuperWeapon2]->Type->AuxBuilding;
        if (aux != 0 && !House->unknown_5460.unknown_func_491d10(aux->ArrayIndex)) {
            return -1;
        }
    }
    return Type->SuperWeapon2;
}


// void BuildingClass::unknown_slot173() (0x004528C0, slot 173)
// Scatters (slot 87, from the zero coordinate) every active infantry whose
// cell holds this building, unless it is headed for another object.

void BuildingClass::unknown_slot173()
{
    for (int index = 0; index < Infantry.Count(); index++) {
        UnknownBuildingInfantryView *infantry = (UnknownBuildingInfantryView *)Infantry[index];
        ObjectClass *building = Map[infantry->unknown_slot19(0)].Cell_Building();
        if (infantry->unknown_7d && building && building == this
            && (!infantry->unknown_494 || infantry->unknown_494 == building)) {
            infantry->Scatter(unknown_geometry_l, 1 != 0, 1 != 0);
        }
    }
}


// Test the foundation border for sensors belonging to another house.
bool BuildingClass::unknown_slot160()
{
    if(!TechnoClass::unknown_slot160()) goto denied;
    {
        Cell origin=unknown_slot104();
        int width=Type->Width();
        int height=Type->Foundation_Height(false);
        for(int x=-1;x<=width;++x) {
            for(int y=-1;y<=height;++y) {
                TechnoClass *object=static_cast<TechnoClass *>(Cell_At(Cell((short)(origin.X+x),(short)(origin.Y+y))).Cell_Techno(Point2D(0,0),false,0));
                if(object==this) object=static_cast<TechnoClass *>(Cell_At(Cell((short)(origin.X+x),(short)(origin.Y+y))).Cell_Infantry(false));
                if(object && object->House!=House && object->Techno_Type_Class()->Sensors) goto denied;
            }
        }
        return true;

    }
denied:
    return false;
}


// Test the foundation border for sensors belonging to another house.
bool BuildingClass::unknown_slot161()
{
    if(TechnoClass::unknown_slot161()) goto detected;
    {
        Cell origin=unknown_slot104();
        int width=Type->Width();
        int height=Type->Foundation_Height(false);
        for(int x=-1;x<=width;++x) {
            for(int y=-1;y<=height;++y) {
                TechnoClass *object=static_cast<TechnoClass *>(Cell_At(Cell((short)(origin.X+x),(short)(origin.Y+y))).Cell_Techno(Point2D(0,0),false,0));
                if(object==this) object=static_cast<TechnoClass *>(Cell_At(Cell((short)(origin.X+x),(short)(origin.Y+y))).Cell_Infantry(false));
                if(object && object->House!=House && object->Techno_Type_Class()->Sensors) goto detected;
            }
        }
        return false;

    }
detected:
    return true;
}


// Test flag 0x400000 on every cell of the building foundation (TibSun building.cpp Should_Fog shape:
// OpenTS, SPDX-License-Identifier: GPL-3.0-or-later; Copyright 2025 Electronic Arts Inc.; Copyright 2026
// OpenTS contributors; EA's GPLv3 Section 7 additional terms apply). The member TPoint2D sum is named
// because 0x00442060's file-local operator+(Cell, Cell) would otherwise be chosen here.
bool BuildingClass::unknown_452be0()
{
    Cell cell = unknown_slot104();
    Cell const *occupy = Type->Occupy_List(0);
    while (*occupy != Cell(32767, 32767)) {
        if (!(Map[Cell(occupy->operator+(cell))].unknown_140 & 0x400000)) {
            return false;
        }
        occupy++;
    }
    return true;
}


// Complete483-byte owned helper; original boundary proof in ORIGINAL.json.
void BuildingClass::Create_Fogged_Copy_00452c70(
    DynamicVectorClass<FoggedObjectClass *> *replacement_list,
    CellClass *replacement_cell, bool arg)
{
    Cell const *offset=Type->Occupy_List(0);
    Unselect();
    FoggedObjectClass *copy=new FoggedObjectClass(this,arg);
    Cell base=unknown_slot104();
    for(;offset->X!=0x7fff || offset->Y!=0x7fff;++offset) {
        CellClass &cell=Cell_At(*offset+base);
        if(replacement_cell && &cell==replacement_cell) {
            replacement_list->Add(copy);
        }else if(cell.FoggedObjects) {
            cell.FoggedObjects->Add(copy);
        }else {
            cell.FoggedObjects=new DynamicVectorClass<FoggedObjectClass *>;
            cell.FoggedObjects->Resize(1);
            cell.FoggedObjects->Set_Growth_Step(1);
            cell.FoggedObjects->Add(copy);
        }
    }
}


// void BuildingClass::unknown_slot82(int arg1) (0x00452E60, slot 82)
// Clears the flag at +0x5AF (resetting +0x438 and restarting the timer at
// +0x420), then the techno behaviour.

void BuildingClass::unknown_slot82(int arg1)
{
    if (unknown_5af) {
        unknown_5af = false;
        unknown_420 = CDTimerClass<FrameTimerClass>();
        unknown_438 = 0;
    }
    TechnoClass::unknown_slot82(arg1);
}


// Whether the supplied infantry can occupy or assault this building.
bool BuildingClass::Unknown_00452EB0(TechnoClass *techno)
{
    if(!Type->CanBeOccupied) return false;
    InfantryClass *infantry=static_cast<InfantryClass *>(techno);
    if(infantry->Type->Occupier) {
        if((House==infantry->House || House->Type->MultiplayPassive)
           && unknown_554.Count()!=Type->MaxNumberOccupants
           && !(char)unknown_func_5d45b0() && !infantry->unknown_234) goto allowed;
    } else if(infantry->Type->Assaulter && !House->Is_Ally(infantry) && unknown_554.Count()) goto allowed;
    return false;
allowed:
    return true;
}


// BuildingClass 0x00452F50: releases the building's occupants (codex2 R2485).
// Names and pointer constness are hypotheses.


void BuildingClass::unknown_452f50(int control)
{
    if (unknown_554.Count() == 0) return;

    CoordStruct destination = unknown_geometry_l;
    Cell selected;
    memcpy(&selected, &unknown_84ed10, sizeof(selected));
    int width = Type->Width();
    int height = Type->Foundation_Height(false);
    bool found = false;
    Cell probe = unknown_slot104();
    probe.X = (short)((unsigned int)(int)probe.X + (unsigned int)width);
    probe.Y = (short)((unsigned int)(int)probe.Y + (unsigned int)height);
    InfantryClass *occupant = unknown_554[0];

    int entry_result = occupant->Can_Enter_Cell(&Map[probe], -1, -1, 0, 1);
    while (entry_result != 0) {
        if ((int)probe.Y <= (int)unknown_slot104().Y - 1) goto after_first_edge;
        probe.Y = (short)(probe.Y - 1);
        entry_result = occupant->Can_Enter_Cell(&Map[probe], -1, -1, 0, 1);
    }
    found = true;
    selected = probe;
after_first_edge:
    probe = unknown_slot104();
    probe.X = (short)((unsigned int)(int)probe.X + (unsigned int)width);
    probe.Y = (short)((unsigned int)(int)probe.Y + (unsigned int)height);
    while (!found && (int)probe.X > (int)unknown_slot104().X - 1) {
        probe.X = (short)(probe.X - 1);
        if (occupant->Can_Enter_Cell(&Map[probe], -1, -1, 0, 1) == 0) {
            found = true;
            selected = probe;
        }
    }

    probe = unknown_slot104();
    probe.X = (short)(probe.X - 1);
    probe.Y = (short)(probe.Y - 1);
    while (!found && (int)probe.X < (int)((unsigned int)(int)unknown_slot104().X + (unsigned int)width)) {
        probe.X = (short)(probe.X + 1);
        if (occupant->Can_Enter_Cell(&Map[probe], -1, -1, 0, 1) == 0) {
            found = true;
            selected = probe;
        }
    }

    probe = unknown_slot104();
    probe.X = (short)(probe.X - 1);
    probe.Y = (short)(probe.Y - 1);
    while (!found && (int)probe.Y < (int)((unsigned int)(int)unknown_slot104().Y + (unsigned int)height)) {
        probe.Y = (short)(probe.Y + 1);
        if (occupant->Can_Enter_Cell(&Map[probe], -1, -1, 0, 1) == 0) {
            selected = probe;
            found = true;
        }
    }
    if (!found) {
        Unknown_004536D0(0);
        return;
    }

    destination = Map[selected].Center_Coord();
    unknown_a40774 = (int)((unsigned int)unknown_a40774 + 1u);
    for (int index = (int)((unsigned int)unknown_554.Count() - 1u); index >= 0; --index) {
        InfantryClass *member = unknown_554[index];
        if (member->Unlimbo(&destination, 0)) {
            member->unknown_slot220(0);
            member->Scatter(Center_Coord(), true, true);
            if ((unsigned char)control != 0) {
                TeamClass *team = member->Team;
                if (team) team->Remove(member, -1, 0);
                member->QueueMission(15, false);
            }
        } else {
            member->Delete_Me();
        }
    }
    unknown_a40774 = (int)((unsigned int)unknown_a40774 - 1u);
    DynamicVectorClass<InfantryClass *> &occupants = unknown_554;
    int capacity = occupants.Length();
    occupants.Clear();
    occupants.Resize(capacity, 0);
    Unknown_006D9610(Get_Cell_Ptr());
}


// int BuildingClass::unknown_4532f0() (0x004532F0)
// The number of entries in the second infantry list.

int BuildingClass::unknown_4532f0()
{
    return unknown_554.Count();
}


// Private Building ownership/garrison update, complete original331+5.
// Passes the new owner house to TechnoClass slot 222, which is still declared (int, int); its HouseClass * signature is a pending rename batch.
extern "C" char const unknown_side_civilian_007ceb30[];
extern "C" char const unknown_eva_structure_abandoned_007cfc14[];
extern "C" char const unknown_eva_structure_garrisoned_007cfbfc[];
void BuildingClass::unknown_453300()
{
    if((char)unknown_func_5d45b0())unknown_452f50(0);
    int count=Houses.Count();
    HouseClass *civilian=0;
    int side=SideClass::Find_Index(unknown_side_civilian_007ceb30);
    if(count>0) {
        HouseClass **cursor=&Houses[0];
        do {
            HouseClass *house=*cursor;
            if(house->Type->Side==side)civilian=house;
            ++cursor;
        }while(--count);
    }
    if(unknown_554.Count()==0) {
        if(House!=civilian) {
            if(House->Is_Player_Control()) {
                Voc_Play(Rules->BuildingAbandonedSound,1.0f,0x2000,0);
                if(Unknown_0063B5B0(15,unknown_slot104()))
                    Speech_Play_Name(unknown_eva_structure_abandoned_007cfc14,-1,-1);
            }
            Refresh_State_Animations_00453450();
            unknown_slot222(civilian,0);
        }
    }
    if(unknown_554.Count()>0 && House==civilian) {
        Refresh_State_Animations_00453450();
        unknown_slot222(unknown_554[0]->House,0);
        if(unknown_554[0]->House->Is_Player_Control()) {
            Speech_Play_Name(unknown_eva_structure_garrisoned_007cfbfc,-1,-1);
            Voc_Play_At(Rules->BuildingGarrisonedSound,Center_Coord(),0);
        }
    }
}


// Refresh existing idle and active animation names for damage and garrison state.
void BuildingClass::Refresh_State_Animations_00453450()
{
    if(unknown_454[17]) {
        unknown_44c920(17,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
    }
    if(unknown_454[3]) {
        unknown_44c920(3,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
    }
    if(unknown_454[4]) {
        unknown_44c920(4,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
    }
    if(unknown_454[5]) {
        unknown_44c920(5,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
    }
    if(unknown_454[6]) {
        unknown_44c920(6,Health_Ratio()<=Rules->unknown_1368,unknown_554.Count()>0,0);
    }
}


// Remove occupants, with an assault animation and attacker action for each when supplied.
static inline void Occupant_Offset_To_Coord(CoordStruct &coordinate,Point2D const &point)
{
    int y=point.Y;
    int x=point.X;
    coordinate.X=x;
    coordinate.Y=y;
    coordinate.Z=0;
}
void BuildingClass::Unknown_004536D0(InfantryClass *infantry)
{
    if(!infantry) {
        for(int index=unknown_554.Count()-1;index>=0;--index)
            unknown_554[index]->Delete_Me();
    } else {
        for(int index=unknown_554.Count()-1;index>=0;--index) {
            CoordStruct coordinate;
            Occupant_Offset_To_Coord(coordinate,unknown_tactical_83996c->Unknown_006A2900(Type->MuzzleFlash[index]));
            coordinate=coordinate+unknown_slot42();
            AnimClass *animation=new AnimClass(reinterpret_cast<AnimTypeClass *>(infantry->unknown_slot231(0)->WeaponType->unknown_f4),coordinate,0,1,0x600,0);
            animation->ZAdjust=-200;
            infantry->Record_The_Kill(unknown_554[index]);
            unknown_554[index]->Delete_Me();
        }
    }
    int capacity=unknown_554.Length();
    unknown_554.Clear();
    unknown_554.Resize(capacity,0);
    Unknown_006D9610(Get_Cell_Ptr());
}


// void BuildingClass::unknown_453850() (0x00453850)
// Restarts the timer at +0x5A0 with the type's ProduceCashDelay; then, for
// a house that is not MultiplayPassive and when slot 195 holds, pays the
// type's ProduceCashAmount into the house (a negative amount is spent).

void BuildingClass::unknown_453850()
{
    unknown_5a0 = CDTimerClass<FrameTimerClass>(Type->ProduceCashDelay);
    if (!House->Type->MultiplayPassive && unknown_slot195()) {
        int amount = Type->ProduceCashAmount;
        if (amount > 0) {
            House->Refund_Money(amount);
        } else {
            House->Unknown_004E5280(-amount);
        }
    }
}


// bool BuildingClass::unknown_4538d0(ObjectTypeClass *type) (0x004538D0)
// Whether slot 62 answers for the object the type creates for the building's
// house (hypothesis).

bool BuildingClass::unknown_4538d0(ObjectTypeClass *type)
{
    TechnoClass *object = (TechnoClass *)type->Create_One_Of(House);
    return Exit_Object(object) != 0 ? true : false;
}


// Construct the voxel-barrel transform with the original ordered float stores.
inline double Barrel_Direction_Angle_00453900(UnknownFacingDir const &direction)
{
    int index=Dir_To_32(direction);
    index-=8;
    return (double)(index*-0.19634954084936207);
}
Matrix3D BuildingClass::unknown_453900()
{
    Matrix3D transform;
    transform.Make_Identity();
    transform.Translate_X((float)Type->VoxelBarrelOffsetToBuildingPivotPoint.X);
    transform.Translate_Y((float)Type->VoxelBarrelOffsetToBuildingPivotPoint.Y);
    transform.Translate_Z((float)Type->VoxelBarrelOffsetToBuildingPivotPoint.Z);
    transform.Rotate_Z((float)Barrel_Direction_Angle_00453900(unknown_2e0.Current()));
    transform.Translate_X((float)Type->VoxelBarrelOffsetToRotatePivotPoint.X);
    transform.Translate_Y((float)Type->VoxelBarrelOffsetToRotatePivotPoint.Y);
    transform.Translate_Z((float)Type->VoxelBarrelOffsetToRotatePivotPoint.Z);
    transform.Rotate_Y((float)(-Barrel_Direction_Angle_00453900(unknown_2c8.Current())));
    transform.Translate_X((float)Type->VoxelBarrelOffsetToPitchPivotPoint.X);
    transform.Translate_Y((float)Type->VoxelBarrelOffsetToPitchPivotPoint.Y);
    transform.Translate_Z((float)Type->VoxelBarrelOffsetToPitchPivotPoint.Z);
    float scale=(float)Type->unknown_13a8;
    transform.Scale_X(scale);
    transform.Scale_Y(scale);
    transform.Scale_Z(scale);
    return transform;
}


// Scale stored amount by four and divide by capacity, preserving both ordered reads.
// Free fastcall versus original Building member source spelling is unproved.
int __fastcall Building_Storage_Level_00453ab0(BuildingClass *building)
{
    if ((int)building->unknown_294.Get_Total_Amount() == 0)
        return 0;
    return (int)((unsigned int)(int)building->unknown_294.Get_Total_Amount() * 4u)
           / building->Type->Storage;
}


// Whether this building makes the given occupied cell row impassable.
bool BuildingClass::Cell_Is_Impassable_Row_00453af0(CellClass const *cell)
{
    if (cell->Cell_Building() != this)
        return false;
    if (Type->NumberImpassableRows == -1)
        return true;
    int cell_x = cell->MapCoords.X;
    int origin_x = unknown_slot104().X;
    return (int)((unsigned int)Type->NumberImpassableRows + (unsigned int)origin_x) > cell_x ? true : false;
}


class Leaf_453b50 { public: bool get(); };
bool Leaf_453b50::get() { return false; }


// 0x00453b60: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 2 sites (data).
// Ghidra prototype: undefined1 unknown_return_true_00453b60(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00453b60 {

unsigned char __stdcall unknown_return_true_00453b60(int unused_0)
{
    return 1;
}

}  // namespace seed_00453b60


class Leaf_453b70 { public: unsigned int get(); };
unsigned int Leaf_453b70::get() { return 2; }


// 0x00453b80: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 2 sites (data).
// Ghidra prototype: void unknown_none_cell_00453b80(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00453b80 {

extern "C" unsigned int _cell_none_0084ed10;

void __stdcall unknown_none_cell_00453b80(unsigned int *result)
{
    *result = _cell_none_0084ed10;
    return;
}

}  // namespace seed_00453b80


class Leaf_453b90 { public: void get(unsigned int a1); };
void Leaf_453b90::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int spare_a; unsigned int spare_b; }


class Leaf_453ba0 { public: bool get(); };
bool Leaf_453ba0::get() { return false; }


class Leaf_453bb0 { public: void get(); };
void Leaf_453bb0::get() {  }


class Leaf_453bc0 { public: bool get(); };
bool Leaf_453bc0::get() { return false; }


// 0x00453bd0: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 2 sites (data).
// Ghidra prototype: undefined1 unknown_return_false_00453bd0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00453bd0 {

unsigned char __stdcall unknown_return_false_00453bd0(int unused_0)
{
    return 0;
}

}  // namespace seed_00453bd0


class Leaf_453be0 { public: void get(unsigned int a1, unsigned int a2); };
void Leaf_453be0::get(unsigned int a1, unsigned int a2) { unsigned int self = (unsigned int)this; unsigned int spare_a; unsigned int spare_b; }


// int BuildingClass::Size_Of() const (0x00453BF0, slot 12): inline in shared/building.hpp, emitted with the unit's vtable; no source of its own.


// HRESULT __stdcall BuildingClass::GetClassID(GUID *clsid) (0x00453C00, slot 3): inline in shared/building.hpp, emitted with the unit's vtable; no source of its own.


// int BuildingClass::Kind_Of() const (0x00453C40, slot 11): inline in shared/building.hpp, emitted with the unit's vtable; no source of its own.


// int BuildingClass::unknown_slot35() (the type's UIName) (0x00453C50, slot 35): inline in shared/building.hpp, emitted with the unit's vtable; no source of its own.


// ObjectTypeClass *BuildingClass::Class_Of() const (0x00453C60, slot 33): inline in shared/building.hpp, emitted with the unit's vtable; no source of its own.


// Return the stored building coordinate offset by half a cell in X and Y.
// Slot42 uses the existing CoordStruct value result and hidden output pointer.
CoordStruct BuildingClass::unknown_slot42() const
{
    CoordStruct position=Coord;
    return position-CoordStruct(128,128,0);
}


// void *BuildingClass::`scalar deleting destructor'(unsigned int flags) (0x00453CA0, slot 8)
// Compiler-generated with the unit's vtable; it calls the destructor out of
// line. No source of its own.


extern "C" void __cdecl release_453de0(void*);
class Cleanup_453de0 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_453de0::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_453de0(pointer); items=0; } owns=0; count=0; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<InfantryClass *>::Clear (0x00453D20; DynamicVectorClass<InfantryClass *> vtable 0x0079d054 slot 3).
// DynamicVectorClass<InfantryClass *>::ID_pointer (0x00453D50; DynamicVectorClass<InfantryClass *> vtable 0x0079d054 slot 5).
// VectorClass<InfantryClass *>::Fetch (0x00453D70; VectorClass<InfantryClass *> vtable 0x0079d074 slot 6).
// DynamicVectorClass<FoggedObjectClass *>::Clear (0x00453D80; DynamicVectorClass<FoggedObjectClass *> vtable 0x0079d17c slot 3).
// DynamicVectorClass<FoggedObjectClass *>::ID_pointer (0x00453DB0; DynamicVectorClass<FoggedObjectClass *> vtable 0x0079d17c slot 5).
// VectorClass<FoggedObjectClass *>::Fetch (0x00453DD0; VectorClass<FoggedObjectClass *> vtable 0x0079d19c slot 6).
// VectorClass<Reestablish *>::Fetch (0x00453E30; VectorClass<Reestablish *> vtable 0x0079d0f0 slot 6).


class Leaf_453e40 { public: unsigned int get(unsigned int a1, unsigned int a2, unsigned int a3); };
unsigned int Leaf_453e40::get(unsigned int a1, unsigned int a2, unsigned int a3) { unsigned int scratch = (unsigned int)this; unsigned int result; unsigned int value; result = scratch; scratch = a1; value = *(unsigned int*)(scratch); *(unsigned int*)(result) = value; scratch = *(unsigned int*)(scratch + 4); value = a2; *(unsigned int*)(result + 4) = scratch; scratch = a3; *(unsigned int*)(result + 8) = value; *(unsigned int*)(result + 0xc) = scratch; return result; }


// IPiggyback pointer assignment, original 0x00453E70.
// The complete implementation is UnknownComPtr::operator=(Interface *) in
// shared/com_ptr.hpp. Building.cpp's existing callers instantiate this member.


// Held locomotor reference increment; complete original00453EA0 extent16 bytes.
// Original free-fastcall/const-member nominal distinction is not visible in its ECX/RET0 ABI.
// Original Building.cpp never expands this instance: sole call004496B1.
// Exhaustive original-unit call/jump scan is retained in the prerequisite evidence.
#pragma auto_inline(off)
template <> void UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600>::_AddRef() const
{
    if(Pointer!=0) Pointer->AddRef();
}
#pragma auto_inline(on)


// 0x00453eb0: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 17 sites (unconditional call).
// Ghidra prototype: int __fastcall CDTimer_Value_00453eb0(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00453eb0 {

extern "C" unsigned int Frame_00a40d2c;

int __fastcall CDTimer_Value_00453eb0(int *timer)
{
    int remaining;
    int elapsed;

    remaining = (int)(timer[2]);
    if (*timer != -1) {
        elapsed = (int)(Frame_00a40d2c - *timer);
        if (elapsed < remaining) {
            return remaining - elapsed;
        }
        remaining = (int)(0);
    }
    return remaining;
}

}  // namespace seed_00453eb0


extern "C" unsigned char vtable_454710[]; extern "C" void __cdecl release_buffer_454710(void*); extern "C" void __cdecl release_object_454710(void*);
class Leaf_454710 { public: void* get(unsigned int flags); };
void* Leaf_454710::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_454710; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_454710((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_454710(this); return this; }


extern "C" unsigned char vtable_454760[]; extern "C" void __cdecl release_buffer_454760(void*); extern "C" void __cdecl release_object_454760(void*);
class Leaf_454760 { public: void* get(unsigned int flags); };
void* Leaf_454760::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_454760; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_454760((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_454760(this); return this; }


extern "C" unsigned char vtable_4547b0[]; extern "C" void __cdecl release_buffer_4547b0(void*); extern "C" void __cdecl release_object_4547b0(void*);
class Leaf_4547b0 { public: void* get(unsigned int flags); };
void* Leaf_4547b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4547b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4547b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4547b0(this); return this; }


extern "C" unsigned char vtable_454800[]; extern "C" void __cdecl release_buffer_454800(void*); extern "C" void __cdecl release_object_454800(void*);
class Leaf_454800 { public: void* get(unsigned int flags); };
void* Leaf_454800::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_454800; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_454800((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_454800(this); return this; }


extern "C" unsigned char vtable_454850[]; extern "C" void __cdecl release_buffer_454850(void*); extern "C" void __cdecl release_object_454850(void*);
class Leaf_454850 { public: void* get(unsigned int flags); };
void* Leaf_454850::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_454850; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_454850((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_454850(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<InfantryClass *>::operator equal (0x00454130; VectorClass<InfantryClass *> vtable 0x0079d074 slot 1).
// VectorClass<InfantryClass *>::Resize (0x00454170; VectorClass<InfantryClass *> vtable 0x0079d074 slot 2).
// VectorClass<InfantryClass *>::Clear (0x00454220; VectorClass<InfantryClass *> vtable 0x0079d074 slot 3).
// VectorClass<InfantryClass *>::ID_object (0x00454250; VectorClass<InfantryClass *> vtable 0x0079d074 slot 4).
// VectorClass<InfantryClass *>::ID_pointer (0x00454290; VectorClass<InfantryClass *> vtable 0x0079d074 slot 5).
// DynamicVectorClass<InfantryClass *>::Resize (0x004542B0; DynamicVectorClass<InfantryClass *> vtable 0x0079d054 slot 2).
// DynamicVectorClass<InfantryClass *>::ID_object (0x00454360; DynamicVectorClass<InfantryClass *> vtable 0x0079d054 slot 4).
// VectorClass<FoggedObjectClass *>::operator equal (0x00454390; VectorClass<FoggedObjectClass *> vtable 0x0079d19c slot 1).
// DynamicVectorClass<FoggedObjectClass *>::Resize (0x004543D0; DynamicVectorClass<FoggedObjectClass *> vtable 0x0079d17c slot 2).
// DynamicVectorClass<FoggedObjectClass *>::ID_object (0x00454480; DynamicVectorClass<FoggedObjectClass *> vtable 0x0079d17c slot 4).
// VectorClass<Reestablish *>::operator equal (0x004544B0; VectorClass<Reestablish *> vtable 0x0079d0f0 slot 1).
// VectorClass<Reestablish *>::Resize (0x004544F0; VectorClass<Reestablish *> vtable 0x0079d0f0 slot 2).
// VectorClass<Reestablish *>::Clear (0x004545A0; VectorClass<Reestablish *> vtable 0x0079d0f0 slot 3).
// VectorClass<Reestablish *>::ID_object (0x004545D0; VectorClass<Reestablish *> vtable 0x0079d0f0 slot 4).
// VectorClass<Reestablish *>::ID_pointer (0x00454610; VectorClass<Reestablish *> vtable 0x0079d0f0 slot 5).
// DynamicVectorClass<Reestablish *>::Resize (0x00454630).
// RTTI base VectorClass<Reestablish *> at0079D0F0; originalBuilding222 constructs
// the base then installs0079D110 and initializes ActiveCount/GrowthStep.
// Complete original176-byte body, no new shared implementation or header.
// VectorClass<FoggedObjectClass *>::Resize (0x004548A0; VectorClass<FoggedObjectClass *> vtable 0x0079d19c slot 2).
// VectorClass<FoggedObjectClass *>::Clear (0x00454950; VectorClass<FoggedObjectClass *> vtable 0x0079d19c slot 3).
// VectorClass<FoggedObjectClass *>::ID_object (0x00454980; VectorClass<FoggedObjectClass *> vtable 0x0079d19c slot 4).
// VectorClass<FoggedObjectClass *>::ID_pointer (0x004549C0; VectorClass<FoggedObjectClass *> vtable 0x0079d19c slot 5).


extern "C" unsigned char vtable_4549e0[]; extern "C" void __cdecl release_buffer_4549e0(void*); extern "C" void __cdecl release_object_4549e0(void*);
class Leaf_4549e0 { public: void* get(unsigned int flags); };
void* Leaf_4549e0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4549e0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4549e0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4549e0(this); return this; }


extern "C" unsigned char vtable_454a30[]; extern "C" void __cdecl release_buffer_454a30(void*); extern "C" void __cdecl release_object_454a30(void*);
class Leaf_454a30 { public: void* get(unsigned int flags); };
void* Leaf_454a30::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_454a30; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_454a30((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_454a30(this); return this; }


extern "C" unsigned char vtable_454a80[]; extern "C" void __cdecl release_buffer_454a80(void*); extern "C" void __cdecl release_object_454a80(void*);
class Leaf_454a80 { public: void* get(unsigned int flags); };
void* Leaf_454a80::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_454a80; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_454a80((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_454a80(this); return this; }


extern "C" unsigned char table_454ad0[]; extern "C" void* __cdecl storage_454ad0(unsigned int);
class Storage_454ad0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_454ad0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_454ad0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_454ad0(requested << 2); owns=1; } return this; }


// bool DynamicVectorClass<AbstractClass *>::Delete(int index) (0x00454B20)
// Instantiated from shared/vector_class.hpp; called by ~BuildingClass for
// the lists at 0x00AC0BF8 and 0x00AC0C80.
// @symbol ?Delete@?$DynamicVectorClass@PAVAbstractClass@@@@QAE_NH@Z


// bool DynamicVectorClass<BuildingClass *>::Delete(int index) (0x00454B60)
// Instantiated from shared/vector_class.hpp; called by ~BuildingClass for
// the building list (0x00A40AF8).
// @symbol ?Delete@?$DynamicVectorClass@PAVBuildingClass@@@@QAE_NH@Z


extern "C" unsigned char table_454ba0[]; extern "C" void* __cdecl storage_454ba0(unsigned int);
class Storage_454ba0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_454ba0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_454ba0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_454ba0(requested << 2); owns=1; } return this; }


// HRESULT UnknownComPtr<IPersist>::Query(UnknownComPtr<ILocomotion> const &other) (0x00454BF0)
// The converting query from a held locomotor (comip's _QueryInterface, YR twin ra2-decomp 0x0045AEA0
// in building.cpp), out of line in Building.CPP after its last vector instances; written as the
// explicit specialization of the stand-in overload in shared/com_ptr.hpp, defined at the end of the
// unit so its callers there keep calling it (the comip text, not the Query_From wrapper: a
// wrapper call moves the result register).

template <>
HRESULT UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208>::Query(UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> const &other)
{
    HRESULT result;
    if (other) {
        IPersist *pointer;
        result = other->QueryInterface(*iid, (void **)&pointer);
        if (FAILED(result)) {
            Attach(0);
            return result;
        }
        Attach(pointer);
    } else {
        operator=((IPersist *)0);
        result = E_NOINTERFACE;
    }
    return result;
}


// HRESULT UnknownComPtr<IPiggyback>::Query(UnknownComPtr<ILocomotion> const &other) (0x00454C70)
// The converting query from a held locomotor (comip's _QueryInterface; YR twin ra2-decomp 0x0045AEA0
// family in building.cpp), out of line in Building.CPP after its last vector instances. Written as the
// explicit specialization of the stand-in overload in shared/com_ptr.hpp, defined at the end of the unit
// (Mission_Unload 0x00449290 keeps calling it); the body is comip's text as in Query_From (a
// `return Query_From(other);` wrapper moves the result register).

extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;      // 0x007CFC30

template <>
HRESULT UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::Query(UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> const &other)
{
    HRESULT result;
    if (other) {
        IPiggyback *pointer;
        result = other->QueryInterface(*iid, (void **)&pointer);
        if (FAILED(result)) {
            Attach(0);
            return result;
        }
        Attach(pointer);
    } else {
        operator=((IPiggyback *)0);
        result = E_NOINTERFACE;
    }
    return result;
}
