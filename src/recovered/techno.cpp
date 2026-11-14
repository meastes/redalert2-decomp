// techno.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/techno.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_DIVIDE_SOURCE
#define RA2_TECHNO_STATIC_TIMERS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/building_type_class.hpp"
#include "shared/cell.hpp"
#include "shared/building_class.hpp"
#include "shared/foot.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/bullet_type.hpp"
#include "shared/house.hpp"
#include "shared/infantry.hpp"
#include "shared/aircraft.hpp"
#include "shared/matrix3d.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/capture_manager.hpp"
#include "shared/parasite.hpp"
#include "shared/temporal.hpp"
#define RA2_INFANTRY_FULL
#include "shared/infantry_type.hpp"
#include "shared/rules.hpp"
#include "shared/tag.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/gscreen.hpp"
#include "shared/techno_map.hpp"
#include "shared/tactical.hpp"
#include "shared/palette.hpp"
#include "shared/gadget.hpp"
#include "shared/options.hpp"
#include "shared/selection_coord.hpp"
#include "shared/convert.hpp"
#include "shared/combat.hpp"
#include "shared/fast_math.hpp"
#include "shared/map_view.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#include "shared/vector_class.hpp"
#include "shared/particle_system.hpp"
#include "shared/anim.hpp"
#include "shared/vox_class.hpp"
#include "shared/team.hpp"
#include "shared/id_source.hpp"
#include "shared/ebolt.hpp"
#include "shared/rad_beam.hpp"
#include "shared/crt_math.hpp"
#include "shared/bullet.hpp"
#include "shared/wave.hpp"
#include "shared/laser_draw.hpp"
#include "shared/sound_play.hpp"
#include "shared/vector3.hpp"
#include "shared/surface.hpp"
#include "shared/event.hpp"
#include "shared/target.hpp"
#include "shared/planning.hpp"
#include "shared/queue.hpp"
#include "shared/random2.hpp"
#include "shared/keyboard.hpp"
#include "shared/house_type.hpp"
#include "shared/voxel_anim.hpp"
#include "shared/sidebar_building.hpp"
#include "shared/dir_type.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tileset_index.hpp"
#include "shared/facing.hpp"
#include "shared/house_adjacent.hpp"
#include "shared/tool_tip.hpp"
#include "shared/hicolor.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/rgb_globals.hpp"
#include "shared/color_scheme.hpp"
#include "shared/light_convert.hpp"
#include "shared/voxel_cache.hpp"
#include "shared/surface_blit.hpp"
#include "shared/blit.hpp"
#include "shared/team_type.hpp"
#include "shared/session_state.hpp"
#include "shared/object_type.hpp"
#include "shared/text_print.hpp"
#include "shared/string_table.hpp"
#include "shared/ilocomotion.hpp"
#include "shared/cell_class.hpp"
#include "shared/system_timer.hpp"
#include "shared/ini_class.hpp"


extern "C" void __cdecl release_6d9750(void*);
class Leaf_6d9750 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_6d9750::get(unsigned int flags) { cleanup(); if (flags & 1) release_6d9750(this); return this; }


// (Declares TechnoClass::ActionLineTimer / TalkBubbleTimer only in this unit: see shared/techno.hpp.)
// (Declares CoordStruct::operator/ only in this unit: its out-of-line copy 0x006D9770 has its own source.)
// Header-static dynamic initializers of the unit at 0x006C0A50: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x006C0A50-0x006C0C31); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006C0A70, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0AA0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0AC0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0AE0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0B00, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0B20, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0B40, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0B80, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0BB0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0BE0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0C10, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Dynamic initializer (0x006C0C20, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_006c0a50.cpp; no source of its own.


// Techno.CPP's two static timers (TibSun TechnoClass::ActionLineTimer and
// TalkBubbleTimer), defined after the unit's header statics: dynamic
// initializers 0x006C0C40 / 0x006C0C80 (_$E) and their guarded atexit
// destructors 0x006C0C60 / 0x006C0CB0 (guard byte 0x00AC0128; the timer's
// destructor is empty), emitted by these definitions.

CDTimerClass<FrameTimerClass> TechnoClass::ActionLineTimer;
CDTimerClass<SystemTimerClass> TechnoClass::TalkBubbleTimer;


// Guarded atexit destructor (0x006C0C60, _$E) of TechnoClass::ActionLineTimer, emitted by TechnoClass_ActionLineTimer_init_006c0c40.cpp; no source of its own.


// Dynamic initializer (0x006C0C80, _$E) of TechnoClass::TalkBubbleTimer, emitted by TechnoClass_ActionLineTimer_init_006c0c40.cpp; no source of its own.


// Guarded atexit destructor (0x006C0CB0, _$E) of TechnoClass::TalkBubbleTimer, emitted by TechnoClass_ActionLineTimer_init_006c0c40.cpp; no source of its own.


// Techno defaults and membership registration; unknown field names retain offsets.
TechnoClass::TechnoClass(HouseClass *house) :
    RadioClass(), FlasherClass(), StageClass(), Cargo(),
    unknown_100(-1), unknown_104(-1), unknown_108(0), unknown_10c(0),
    unknown_110(-1), unknown_114(-1), unknown_118(),
    unknown_120(1.0), unknown_128(1.0),
    unknown_130(0), unknown_13c(0), unknown_148(45), unknown_154(0), unknown_160(0),
    unknown_170(false), unknown_174(0), unknown_178(0),
    unknown_184(0), unknown_190(false), unknown_194(0), unknown_1a0(0,0),
    unknown_1a8(0), unknown_1ac(-1), unknown_1b0(0), House(house), unknown_1b8(0),
    unknown_1bc(), unknown_1d4(0), unknown_1e0(0), unknown_1e4(false),
    unknown_1e8(unknown_geometry_l), unknown_1f4(0), unknown_1f8(0),
    unknown_1fc(false), unknown_1fd(false), unknown_200(0),
    unknown_204(false), unknown_205(false), unknown_206(false),
    unknown_208(0), unknown_20c(0), unknown_210(false), unknown_214(0), unknown_218(0),
    unknown_21c(unknown_geometry_l), unknown_228(0), unknown_22c(0), unknown_230(0),
    unknown_234(0), SpawnManager(0), unknown_23c(0), unknown_240(0), unknown_244(0),
    unknown_250(0), unknown_254(-1), unknown_258(0),
    unknown_27c(0), unknown_280(0), unknown_284(0), unknown_288(0), unknown_28c(0), unknown_290(-1),
    unknown_294(), unknown_2a8(), unknown_2c8(3), unknown_2e0(), unknown_2f8(),
    unknown_310(0), unknown_314(0), unknown_322(0),
    unknown_324(false), unknown_325(false), unknown_326(false), unknown_327(false),
    unknown_328(false), unknown_329(false), unknown_32a(false), unknown_32b(false),
    unknown_32c(false), unknown_32d(false), unknown_330(), unknown_350(),
    unknown_370(false), unknown_371(false), unknown_372(false), unknown_373(false),
    unknown_374(false), unknown_375(false), unknown_376(false), unknown_377(false), unknown_378(0),
    unknown_379(true), unknown_37a(true), unknown_37b(false), unknown_37c(false),
    unknown_37d(false), unknown_37e(false), unknown_37f(false), unknown_380(false), unknown_381(false),
    unknown_384(0), unknown_388(false), unknown_38c(0),
    unknown_390(), unknown_3a8(), unknown_3c0(),
    unknown_3ec(1), unknown_3f0(0), unknown_3f4(Frame), unknown_3f8(0), unknown_3fc(0),
    unknown_408(0), unknown_40c(0), unknown_410(0)
{
    if (house) unknown_114=house->unknown_130;
    unknown_3d8.Init();
    Technos.Add(this);
    unknown_abstracts_ac0d08.Add(this);
    AbstractFlags.IsTechno=true;
    for (int i=0;i<8;++i) unknown_25c[i]=0;
    unknown_320=short(unknown_id_source_a3d290->Random()());
}


// TechnoTypeClass *TechnoClass::Techno_Type_Class() (0x006C12C0, slot 32)
// The type as a techno type (Class_Of).

TechnoTypeClass *TechnoClass::Techno_Type_Class()
{
    return (TechnoTypeClass *)Class_Of();
}


// bool TechnoClass::unknown_slot155() (0x006C12D0, slot 155)
// True for a current mission other than 0, 6 and 16 unless the type sets +0xAA0.

bool TechnoClass::unknown_slot155()
{
    if (Get_Mission() && Get_Mission() != 6 && Get_Mission() != 16 && !Techno_Type_Class()->IsTrain) {
        return true;
    }
    return false;
}


// bool TechnoClass::unknown_slot24() (0x006C1320, slot 24)
// A chain of state tests ending in the owner test 0x004F52B0.

bool TechnoClass::unknown_slot24()
{
    if (Strength > 0 && !unknown_76 && unknown_32d && Kind_Of() != 6 && unknown_373 &&
            Techno_Type_Class()->unknown_200) {
        return House->Is_Player_Control() ? true : false;
    }
    return false;
}


// int TechnoClass::unknown_slot174(AbstractClass *target) (0x006C1380, slot 174)
// Which weapon to use against target: 0 primary, 1 secondary (TibSun's
// What_Weapon_Should_I_Use, with RA2's rules: a turret weapon set keeps the
// current index, occupied buildings follow the occupant's house, ...).
// The armor index is one named local read twice (TibSun's `armor`); read
// in place, this and target trade esi/edi.

int TechnoClass::unknown_slot174(AbstractClass *target)
{
    if (Techno_Type_Class()->Has_Turret_Weapons()) {
        return unknown_10c != -1 ? unknown_10c : 0;
    }
    WeaponTypeClass *secondary = unknown_slot231(1)->WeaponType;
    if (secondary == 0) {
        return 0;
    }
    WeaponTypeClass *primary = unknown_slot231(0)->WeaponType;
    if (primary == 0) {
        return 0;
    }
    if (secondary->unknown_112) {
        return 0;
    }
    if (secondary->unknown_126 && Get_Mission() == 0x10) {
        return 1;
    }
    if (Kind_Of() == 6 && ((BuildingClass *)this)->unknown_531) {
        return 1;
    }
    if (Kind_Of() == 6 && ((BuildingClass *)this)->unknown_4532f0() > 0) {
        return ((BuildingClass *)this)->unknown_554[0]->House->unknown_130 == 1;
    }
    if (target == 0) {
        return 0;
    }
    if (target->Kind_Of() == 6 && House->Is_Ally((ObjectClass const *)target)
        && secondary->Warhead->ElectricAssault && ((BuildingClass *)target)->Type->Overpowerable) {
        return 1;
    }
    if (Kind_Of() == 2 && ((AircraftClass *)this)->unknown_5b2) {
        return 1;
    }
    if (!target->AbstractFlags.IsTechno) {
        return 0;
    }
    ObjectClass *object = (ObjectClass *)target;
    int armor = object->Techno_Type_Class()->unknown_9c;
    if (secondary->Warhead->Verses[armor] == 0.0) {
        return 0;
    }
    armor = object->Techno_Type_Class()->unknown_9c;
    if (primary->Warhead->Verses[armor] == 0.0) {
        return 1;
    }
    bool ground = ((CellClass *)object->Get_Cell_Ptr())->unknown_ec == 2
        || ((CellClass *)object->Get_Cell_Ptr())->unknown_ec == 6;
    if (object->In_Air()) {
        ground = false;
    }
    if (!object->unknown_79 && ground) {
        int which = unknown_slot175(object);
        if (which == -1) {
            which = 0;
        }
        return which;
    } else if (secondary->Projectile->AA && object->In_Air()) {
        return 1;
    }
    return 0;
}


// int TechnoClass::unknown_slot175(ObjectClass *target) (0x006C15D0, slot 175)
// How the techno may target an active object by its type's NavalTargeting
// (-1 never, 0/1 by whether the target is Underwater, Organic or hovering).
// Body 256 bytes: code to 0x006C16B3 and the seven-entry jump table at
// 0x006C16B4. Case 0 returns its 0 itself (not through the shared final
// return): only then is case 6's `return -1` cross-jumped into the guard's
// return block (+0x14), as in the original, instead of case 0's (+0x84).

int TechnoClass::unknown_slot175(ObjectClass *target)
{
    bool active = target != 0 && target->AbstractFlags.IsTechno;
    if (!active) {
        return -1;
    }
    bool underwater = target->Techno_Type_Class()->Underwater;
    bool organic = target->Techno_Type_Class()->Organic;
    bool hover = target->Techno_Type_Class()->SpeedType == 3;
    switch (Techno_Type_Class()->NavalTargeting) {
    case 0:
        if (underwater && ((TechnoClass *)target)->unknown_1b8 != 0) {
            return -1;
        }
        return 0;
    case 1:
        return underwater ? 1 : 0;
    case 2:
        return underwater ? 0 : -1;
    case 3:
        return organic ? 1 : 0;
    case 4:
        if (!hover && !organic) {
            return 1;
        }
        break;
    case 5:
        break;
    case 6:
        return -1;
    }
    return 0;
}


// int TechnoClass::unknown_slot172() (0x006C16D0, slot 172)
// The type flag +0xAD9.

int TechnoClass::unknown_slot172()
{
    return Techno_Type_Class()->Crewed != false;
}


// Weapon damage and ambient damage, averaged for an unspecified weapon.
// Existing nonconst member ABI retained; operation name remains a hypothesis.
int TechnoClass::unknown_6c16f0(int weapon)
{
    int value=0;
    if(weapon==-1) {
        int divisor=0;
        if(Techno_Type_Class()->Has_Turret_Weapons()) {
            WeaponStruct *slot=unknown_slot231(unknown_10c);
            if(!slot) return 0;
            if(!slot->WeaponType) return 0;
            value+=slot->WeaponType->unknown_a4;
            value+=slot->WeaponType->unknown_98;
            divisor=1;
        } else {
            if(unknown_slot231(0)->WeaponType) {
                value+=unknown_slot231(0)->WeaponType->unknown_a4;
                value+=unknown_slot231(0)->WeaponType->unknown_98;
                divisor=1;
            }
            if(unknown_slot231(1)->WeaponType) {
                value+=unknown_slot231(1)->WeaponType->unknown_a4;
                value+=unknown_slot231(1)->WeaponType->unknown_98;
                ++divisor;
            }
        }
        if(divisor>0) return value/divisor;
        return 0;
    } else {
        if(unknown_slot231(weapon)->WeaponType) {
            value+=unknown_slot231(weapon)->WeaponType->unknown_a4;
            value+=unknown_slot231(weapon)->WeaponType->unknown_98;
        }
        return value;
    }
}


// Weapon mounting coordinate with the locomotor transform and turret angle.
extern "C" double const unknown_fire_angle_scale_79d090;
inline unsigned UnknownFacingDir::Round_To_32() const
{
    return (((*(unsigned const *)this>>10)+1)>>1);
}
inline Dir32 UnknownFacingDir::As_Dir32() const
{
    return (Dir32)(Round_To_32()%32U);
}
inline double UnknownFacingDir::As_Radian32() const
{
    return (double)((As_Dir32()-8)*unknown_fire_angle_scale_79d090);
}
CoordStruct TechnoClass::unknown_slot43(int which)
{
    TechnoTypeClass *type=Techno_Type_Class();
    WeaponStruct *weapon=unknown_slot231(which);
    int fire_x=weapon->FLH.X;
    int fire_y=weapon->FLH.Y;
    int fire_z=weapon->FLH.Z;
    Matrix3D matrix;
    double angle;
    FootClass *foot=this && AbstractFlags.IsFoot ? static_cast<FootClass *>(this) : 0;
    if (foot && foot->Locomotor) {
        Matrix3D drawn;
        Matrix3D locomotor_matrix(*foot->Locomotor.Checked()->Draw_Matrix(&drawn,0));
        matrix=locomotor_matrix;
        angle=unknown_slot162().As_Radian32()-unknown_2e0.Current().As_Radian32();
    } else {
        matrix.Make_Identity();
        angle=unknown_slot162().As_Radian32();
    }
    matrix.Translate(type->TurretOffset,0.0f,0.0f);
    matrix.Rotate_Z((float)angle);
    matrix.Translate((float)fire_x,(float)(fire_y*(unknown_310%2!=0 ? -1 : 1)),(float)fire_z);
    Vector3 fire=matrix*Vector3(0.0f,0.0f,0.0f);
    CoordStruct offset((int)fire.X,(int)-fire.Y,(int)fire.Z);
    return offset+unknown_slot42();
}


// Weapon mounting coordinate with the locomotor transform and turret angle.
extern "C" double const unknown_fire_angle_scale_79d090;
CoordStruct TechnoClass::unknown_slot181(int which)
{
    TechnoTypeClass *type=Techno_Type_Class();
    Matrix3D matrix;
    double angle;
    FootClass *foot=this && AbstractFlags.IsFoot ? static_cast<FootClass *>(this) : 0;
    if (foot && foot->Locomotor) {
        Matrix3D drawn;
        Matrix3D locomotor_matrix(*foot->Locomotor.Checked()->Draw_Matrix(&drawn,0));
        matrix=locomotor_matrix;
        angle=unknown_slot162().As_Radian32()-unknown_2e0.Current().As_Radian32();
    } else {
        matrix.Make_Identity();
        angle=unknown_slot162().As_Radian32();
    }
    matrix.Translate(type->TurretOffset,0.0f,0.0f);
    matrix.Rotate_Z((float)angle);
    Vector3 fire=matrix*Vector3(0.0f,0.0f,0.0f);
    CoordStruct offset((int)fire.X,(int)-fire.Y,(int)fire.Z);
    return offset+unknown_slot42();
}


// void TechnoClass::Init() (0x006C1C50, slot 9)
// Records whether the owner is the player (+0x372); with a type, makes the
// owned helpers its type and primary weapon's warhead call for: a spawn
// manager for Spawns (+0x238), a capture manager for a MindControl warhead
// (+0x230), a parasite for a Parasite warhead on non-buildings (FootClass
// +0x588) and a temporal for a Temporal warhead (+0x208, taking the weapon's
// +0xA4); a permanently disguised type takes its owner as the disguise house
// and the rules' disguise (+0xB6C, or +0xB70 when the owner's +0x130 is set);
// copies the turret and barrel recoil timing and resets +0x3EC/+0x3F0.
// The infantry type (for the disguise types): InfantryClass itself comes from shared/infantry.hpp.

// Takes a type's recoil timing (travel and the three frame counts).
inline void Copy_Recoil(RecoilData &block, RecoilControl const &data)
{
    block.unknown_00 = data.Travel;
    block.unknown_04 = data.CompressFrames;
    block.unknown_08 = data.RecoverFrames;
    block.unknown_0c = data.HoldFrames;
}

void TechnoClass::Init()
{
    if (House != 0) {
        if (unknown_player_a35db4 == House) {
            unknown_372 = true;
        } else {
            unknown_372 = false;
        }
    }
    if (Techno_Type_Class() == 0) {
        return;
    }
    if (Techno_Type_Class()->Spawns != 0) {
        SpawnManager = new SpawnManagerClass(this, Techno_Type_Class()->Spawns, Techno_Type_Class()->SpawnsNumber,
            Techno_Type_Class()->SpawnRegenRate, Techno_Type_Class()->SpawnReloadRate);
    }
    if (unknown_slot231(0)->WeaponType != 0 && unknown_slot231(0)->WeaponType->Warhead->MindControl) {
        unknown_230 = new CaptureManagerClass(this);
    }
    if (Kind_Of() != 6 && unknown_slot231(0)->WeaponType != 0 && unknown_slot231(0)->WeaponType->Warhead->Parasite) {
        ((FootClass *)this)->unknown_588 = new ParasiteClass(this);
    }
    if (unknown_slot231(0)->WeaponType != 0 && unknown_slot231(0)->WeaponType->Warhead->Temporal) {
        unknown_208 = (UnknownTechno208 *)new TemporalClass(this);
        ((TemporalClass *)unknown_208)->unknown_4c = unknown_slot231(0)->WeaponType->unknown_a4;
    }
    if (Techno_Type_Class()->CanDisguise && Techno_Type_Class()->PermaDisguise) {
        unknown_170 = true;
        unknown_410 = House;
        if (House->unknown_130 == 0) {
            unknown_40c = Rules->unknown_b6c;
        } else {
            unknown_40c = Rules->unknown_b70;
        }
    }
    Copy_Recoil(unknown_330, Techno_Type_Class()->TurretAnimData);
    Copy_Recoil(unknown_350, Techno_Type_Class()->BarrelAnimData);
    unknown_3ec = 1;
    unknown_3f0 = 0;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// TechnoClass(NoInitClass const &) (0x006C1F10)
// The load constructor: every member with a NoInitClass constructor is built from noinit, the stage at
// +0x1BC and the three target lists default-construct, then the object re-registers in Technos unless it
// is already there. From ra2-decomp (YR 0x006F4300): the target lists' `(0, 0)` initializers. The
// DynamicVectorClass constructors expand inline but their VectorClass base constructors (0x0046D020,
// 0x004CEB90) stay out of line, as in the original, under the unit's RA2_CDTIMER_BASIC_BASE and
// RA2_STAGE_PLAIN_TIMER (first row 0x0049ED90).
TechnoClass::TechnoClass(NoInitClass const &noinit) :
    RadioClass(noinit), FlasherClass(noinit), StageClass(noinit),
    Cargo(noinit), unknown_118(noinit),
    unknown_130(noinit), unknown_13c(noinit), unknown_148(noinit), unknown_154(noinit),
    unknown_160(noinit), unknown_178(noinit), unknown_184(noinit), unknown_194(noinit),
    unknown_1d4(noinit),
    unknown_244(noinit), unknown_294(noinit), unknown_2a8(noinit),
    unknown_2c8(noinit), unknown_2e0(noinit), unknown_2f8(noinit), unknown_314(noinit),
    unknown_390(0, 0), unknown_3a8(0, 0), unknown_3c0(0, 0)
{
    bool found = false;
    for (int i = 0; i < Technos.Count(); i++) {
        if (Technos[i] == this) {
            found = true;
            break;
        }
    }
    unknown_3d8.Init();
    if (!found) {
        Technos.Add(this);
    }
}


// TechnoClass::~TechnoClass() (0x006C20B0)
// Releases the +0x408 object, deletes the owned helpers (+0x230 capture
// manager, +0x238 spawn manager, +0x208 temporal), stops the audio at +0x3D8,
// forgets the owner, leaves Technos and the 0x00AC0D08 list, releases the
// +0x108 object (slot 60) and leaves the 0x00AC0BF8 list; then the members
// and RadioClass/MissionClass/ObjectClass are destroyed.

TechnoClass::~TechnoClass()
{
    if (unknown_408 != 0) {
        unknown_408->unknown_6130b0();
        unknown_408 = 0;
    }
    if (unknown_230 != 0) {
        delete unknown_230;
        unknown_230 = 0;
    }
    if (SpawnManager != 0) {
        delete SpawnManager;
        SpawnManager = 0;
    }
    if (unknown_208 != 0) {
        delete unknown_208;
        unknown_208 = 0;
    }
    unknown_3d8.Unknown_00405FE0();
    unknown_3d8.Unknown_00405C10();
    House = 0;
    unknown_372 = false;
    Technos.Delete(this);
    unknown_abstracts_ac0d08.Delete(this);
    if (unknown_108 != 0) {
        unknown_108->Delete_Me();
        unknown_108 = 0;
    }
    unknown_abstracts_ac0bf8.Delete(this);
}


// int TechnoClass::Unknown_006C22A0() (0x006C22A0)
// The production time (hypothesis: Westwood's Time_To_Build): the type's
// time (ObjectTypeClass slot 34) times the owner's multiplier for its kind,
// slowed by low power (LowPowerPenaltyModifier, clamped to
// MinLowPowerProductionSpeed / MaxLowPowerProductionSpeed), sped up by
// MultipleFactory for each extra factory, and scaled by
// WallBuildSpeedCoefficient for walls.

int TechnoClass::Unknown_006C22A0()
{
    int time = Class_Of()->unknown_slot34();
    time = House->unknown_func_4f5a70(Techno_Type_Class()) * time;
    float power = House->Power_Fraction();
    float speed = 1.0f - (1.0f - power) * Rules->LowPowerPenaltyModifier;
    if ((double)Rules->MinLowPowerProductionSpeed >= speed) {
        speed = Rules->MinLowPowerProductionSpeed;
    }
    if (power < 1.0f && (double)Rules->MaxLowPowerProductionSpeed <= speed) {
        speed = Rules->MaxLowPowerProductionSpeed;
    }
    if (speed == 0.0f) {
        speed = 0.01f;
    }
    time = time / speed;
    bool naval = false;
    if (Kind_Of() == 1) {
        TechnoTypeClass *type = Techno_Type_Class();
        if (type != 0) {
            naval = type->Naval;
        }
    }
    int factories = House->unknown_func_4eb980(Kind_Of(), naval);
    float multiple = Rules->MultipleFactory;
    if (multiple > 0.0f) {
        for (int count = factories - 1; count > 0; count--) {
            time = time * multiple;
        }
    }
    if (Kind_Of() == 6 && ((BuildingClass *)this)->Type->Wall) {
        return time * Rules->WallBuildSpeedCoefficient;
    }
    return time;
}


// bool TechnoClass::Revealed(HouseClass *house) (0x006C2440, slot 96)
// Reveals the object to a house once (+0x373 for the player, +0x374 for the
// others; false when already done or ObjectClass slot 96 refuses). An
// object of a 0x004F52F0-failing house in mission 14 switches to mission 15.
// For the player it also sets the owner's +0x5668/+0x5669 and, unless +0x372,
// springs its tag (event 4) outside game modes 1+ and sets the owner's +0x13C.

bool TechnoClass::Revealed(HouseClass *house)
{
    if (house == unknown_player_a35db4) {
        if (unknown_373) {
            return false;
        }
    } else {
        if (unknown_374) {
            return false;
        }
        unknown_374 = true;
    }
    if (ObjectClass::Revealed(house)) {
        if (!house->Unknown_004F52F0() && Get_Mission() == 14) {
            QueueMission(15, false);
        }
        if (house == unknown_player_a35db4) {
            unknown_373 = true;
            House->unknown_5668 = true;
            House->unknown_5669 = true;
            if (!unknown_372) {
                if (unknown_a40774 == 0 && unknown_34 != 0) {
                    unknown_34->Spring(4, this, unknown_cell_ac0148, 0, 0);
                }
                House->unknown_13c = true;
            }
        } else {
            unknown_374 = true;
        }
        return true;
    }
    return false;
}


// void TechnoClass::Hidden() (0x006C2520, slot 69)
// Clears unknown_373 when the owner test 0x004F52F0 fails.

void TechnoClass::Hidden()
{
    if (unknown_373) {
        if (!House->Unknown_004F52F0()) {
            unknown_373 = false;
        }
    }
}


// bool TechnoClass::Mark(int arg1) (0x006C2550, slot 71)
// The ObjectClass version, then radio message 13 when unknown_370 is set.

bool TechnoClass::Mark(int arg1)
{
    if (ObjectClass::Mark(arg1)) {
        if (unknown_370) {
            unknown_slot149(13);
        }
        return true;
    }
    return false;
}


// int TechnoClass::Receive_Message(RadioClass *from, int message, AbstractClass *&param) (0x006C2590, slot 95)
// The radio message handler (Westwood's Receive_Message; param is the
// message's long argument):
//   3: answers 0x19 to a sender with +0x370 when this has +0x370 too, then RadioClass's;
//   7, 9, 22: answers 0x18, then RadioClass's;   8: answers 0x19 and 3;
//   24: unless an AirportBound aircraft or already +0x370, sets it, answers 0x18;
//   25: clears +0x370, answers 0x19;  26/27: sets/clears +0x371, answers 0x1A/0x1B;
//   28: repairs one step for the type's repair cost (10 when not damaged, 0x20
//       when the owner cannot pay; restarts a riding parasite's timer; fully
//       repaired: 0x21 with Strength reset);
//   30: with a weapon, targets param and commences mission 1;
//   31: 10 at full ammunition, else one more;
// everything else goes to RadioClass. The handled messages answer 1.

int TechnoClass::Receive_Message(RadioClass *from, int message, AbstractClass *&param)
{
    TechnoClass *sender = (TechnoClass *)from;
    switch (message) {
    case 30: {
        WeaponStruct *weapon = unknown_slot230();
        if (weapon == 0 || weapon->WeaponType == 0) {
            break;
        }
        unknown_slot220(param);
        QueueMission(1, false);
        return 1;
    }
    case 24: {
        if (Kind_Of() == 2) {
            AircraftClass *aircraft = this == 0 ? 0 : (Kind_Of() == 2 ? (AircraftClass *)this : 0);
            if (aircraft->Type->AirportBound) {
                break;
            }
        }
        if (unknown_370) {
            break;
        }
        unknown_370 = true;
        unknown_slot150(0x18, sender);
        return 1;
    }
    case 25:
        if (!unknown_370) {
            break;
        }
        unknown_370 = false;
        unknown_slot150(0x19, sender);
        return 1;
    case 26:
        if (unknown_371) {
            break;
        }
        unknown_371 = true;
        unknown_slot150(0x1A, sender);
        return 1;
    case 27:
        if (!unknown_371) {
            break;
        }
        unknown_371 = false;
        unknown_slot150(0x1B, sender);
        return 1;
    case 8:
        unknown_slot150(0x19, sender);
        return unknown_slot150(3, sender);
    case 3:
        if (unknown_370 && sender->unknown_370) {
            unknown_slot150(0x19, sender);
        }
        RadioClass::Receive_Message(from, message, param);
        return 1;
    case 7:
    case 9:
    case 22:
        unknown_slot150(0x18, sender);
        RadioClass::Receive_Message(from, message, param);
        return 1;
    case 31:
        if (unknown_254 == Techno_Type_Class()->Ammo) {
            return 10;
        }
        unknown_254++;
        return 1;
    case 28:
        if (Health_Ratio() < Rules->unknown_1360) {
            int cost = Techno_Type_Class()->Repair_Cost();
            int step = Techno_Type_Class()->Repair_Step();
            if (step <= 1) {
                step = 1;
            }
            if (House->Available_Money() >= cost) {
                if (cost != 0) {
                    House->Unknown_004E5280(cost);
                }
                Strength += step;
                unknown_70 += step;
                bool foot;
                if (this != 0) {
                    foot = AbstractFlags.IsFoot;
                }
                if (this != 0 && foot && ((FootClass *)this)->unknown_580 != 0) {
                    ((FootClass *)this)->unknown_580->unknown_588->unknown_2c = 50;
                    ((FootClass *)this)->unknown_580->unknown_588->Unknown_00607440();
                }
                if (Health_Ratio() > Rules->unknown_1368 || Get_Height_AGL() < -10) {
                    if (unknown_25c[3] != 0) {
                        unknown_25c[3]->Delete_Me();
                    }
                }
                if (Health_Ratio() < Rules->unknown_1360) {
                    return 1;
                }
                int strength = Techno_Type_Class()->MaxStrength;
                Strength = strength;
                unknown_70 = strength;
                return 0x21;
            }
            return 0x20;
        }
        return 10;
    }
    return RadioClass::Receive_Message(from, message, param);
}


// void TechnoClass::unknown_slot238() (0x006C2990, slot 238)
// When this object's cell is seen by its house (0x0047C2C0) and slot 160
// holds: every techno targeting it whose house sees the cell under it
// (0x0047C2E0) or shares its house is collected, slot 253 runs quietly, and
// each collected techno gets this object as its target again (slot 220).

void TechnoClass::unknown_slot238()
{
    Cell cell = Coord_Cell(Center_Coord());
    UnknownCellClass *here = (UnknownCellClass *)&Map[cell];
    if (here->unknown_47c2c0(House->ArrayIndex) && unknown_slot160()) {
        DynamicVectorClass<TechnoClass *> attackers;
        for (int index = Technos.Count() - 1; index >= 0; index--) {
            TechnoClass *techno = Technos[index];
            if (techno->unknown_228 == this) {
                if (((UnknownCellClass *)&Map[Center_Coord()])->Is_Sensed(techno->House->ArrayIndex)
                    || techno->House == House) {
                    attackers.Add(techno);
                }
            }
        }
        unknown_slot253(false);
        for (int i = attackers.Count() - 1; i >= 0; i--) {
            attackers[i]->unknown_slot220(this);
        }
    }
}


// void TechnoClass::unknown_slot93(int arg1) (0x006C2B40, slot 93)

void TechnoClass::unknown_slot93(int arg1)
{
    if (arg1 == 2) {
        if (unknown_208 != 0 && unknown_208->unknown_28 != 0) {
            unknown_208->unknown_6e2500();
        }
        Cell cell = Coord_Cell(Center_Coord());
        unknown_slot238();
        if (unknown_34 != 0) {
            unknown_34->Spring(0x22, this, unknown_cell_ac0148, 0, 0);
        }
        if (!unknown_32d && Map.Unknown_0055A660(cell, true)) {
            unknown_32d = true;
        }
        if (!unknown_373 && (Map_Cell_At(cell)->unknown_12c & 0x10)) {
            Revealed(unknown_player_a35db4);
        }
        Map_Cell_At(cell)->Trigger_Veins();
    }
}


// TechnoClass slot 66 0x006C2C40: Draw_Post_Render (TibSun techno.cpp 1225).
extern void *unknown_convert_8323c8;
extern Rect unknown_rect_839630;
void __fastcall Draw_Double_Selection_Bracket_006c3950(::Coord const &,::Coord const &,int);
// 0x006C3A90: the last infantry site refuses it, so this unit emits the out-of-line copy (TibSun techno.cpp).
inline void __fastcall Draw_Single_Selection_Bracket(::Coord const &from, ::Coord const &to, int color)
{
    ::Coord quarter = (from + from + from + to) / 4;
    if (from.Z > quarter.Z) {
        unknown_tactical_83996c->unknown_slot24(reinterpret_cast<CoordStruct const &>(from), reinterpret_cast<CoordStruct const &>(quarter), color, 0);
    } else {
        unknown_tactical_83996c->unknown_slot24(reinterpret_cast<CoordStruct const &>(quarter), reinterpret_cast<CoordStruct const &>(from), color, 0);
    }
}
// Read accessors of the tail tests (their sites decide the bracket refusals above).
inline bool Flag_380(TechnoClass const *techno) { return techno->unknown_380; }
inline bool Is_Selected(TechnoClass const *techno) { return techno->unknown_77; }
void TechnoClass::unknown_slot66(Point2D const &point, Rect const &window)
{
    bool sensed_underground = false;
    if (!unknown_325) {
        CellClass *cell = static_cast<CellClass *>(Get_Cell_Ptr());
        if (!unknown_77 && Get_Height_AGL() < -20 && cell && cell->Is_Sensed(unknown_player_a35db4->ArrayIndex)) {
            sensed_underground = true;
        }
        if (unknown_68 && unknown_38 && !Map[unknown_slot104()].unknown_47cb50()) {
            Point2D bomb_point;
            unknown_tactical_83996c->Coord_To_Pixel(unknown_slot42(), bomb_point);
            CC_Draw_Shape(unknown_surface_83995c, static_cast<ConvertClass *>(unknown_convert_8323c8),
                static_cast<ShapeFileStruct const *>(Rules->BombCursorShape), unknown_38->Unknown_00436000(),
                bomb_point, unknown_rect_839630, 0xe00, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
        if (this && Kind_Of() == 6) {
            BuildingClass *building = Kind_Of() == 6 ? static_cast<BuildingClass *>(this) : 0;
            if (building->unknown_5b8 && !Map[unknown_slot104()].unknown_47cb50()) {
                Point2D repair_point;
                unknown_tactical_83996c->Coord_To_Pixel(unknown_slot42(), repair_point);
                int delay = Options.Normalize_Delay(14) / 4;
                if (delay < 2) delay = 2;
                CC_Draw_Shape(unknown_surface_83995c, static_cast<ConvertClass *>(unknown_convert_8323c8),
                    static_cast<ShapeFileStruct const *>(unknown_wrench_shape_8502a8),
                    ((Frame % delay) * 6) / (delay - 1), repair_point, window, 0xe00, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        if (!unknown_slot49(unknown_player_a35db4) && unknown_slot25(false, 0) != 5) {
            unknown_slot250(point, window);
        }
        if (unknown_77 || sensed_underground) {
            if (Kind_Of() == 6) {
                int color = 15;
                if (Get_Height_AGL() < -4) {
                    color = 12;
                }
                CoordStruct const &lepton = Techno_Type_Class()->Lepton_Dimensions();
                ::Coord dim = lepton;
                int x = dim.X / 2;
                dim.Y /= 2;
                int y = dim.Y;
                color = unknown_drawer_8323c4->Convert(color);
                CoordStruct const &position = Center_Coord();
                ::Coord center = position;
                if (Kind_Of() != 15) {
                    Draw_Double_Selection_Bracket_006c3950(center + ::Coord(x, y, 0), center + ::Coord(-x, y, 0), color);
                    Draw_Double_Selection_Bracket_006c3950(center + ::Coord(x, y, 0), center + ::Coord(x, -y, 0), color);
                    Draw_Double_Selection_Bracket_006c3950(center + ::Coord(-x, y, 0), center + ::Coord(-x, y, dim.Z), color);
                    Draw_Double_Selection_Bracket_006c3950(center + ::Coord(x, -y, 0), center + ::Coord(x, -y, dim.Z), color);
                }
                if (Strength > 0 && (House->Is_Ally(unknown_player_a35db4) || Rules->EnemyHealth)) {
                    unknown_slot247(reinterpret_cast<int>(&point), reinterpret_cast<int>(&window));
                }
                if (Kind_Of() != 15) {
                    Draw_Single_Selection_Bracket(center + ::Coord(x, y, 0), center + ::Coord(x, y, dim.Z), color);
                    Draw_Single_Selection_Bracket(center + ::Coord(x, -y, dim.Z), center + ::Coord(x, y, dim.Z), color);
                    Draw_Single_Selection_Bracket(center + ::Coord(-x, y, dim.Z), center + ::Coord(x, y, dim.Z), color);
                } else {
                    Draw_Single_Selection_Bracket(center + ::Coord(-x, y, 0), center + ::Coord(x, y, 0), color);
                    Draw_Single_Selection_Bracket(center + ::Coord(-x, y, 0), center + ::Coord(-x, y, dim.Z), color);
                    Draw_Single_Selection_Bracket(center + ::Coord(x, -y, dim.Z), center + ::Coord(x, -y, 0), color);
                    Draw_Single_Selection_Bracket(center + ::Coord(x, -y, dim.Z), center + ::Coord(-x, -y, dim.Z), color);
                }
            }
            unknown_slot248(point, window, sensed_underground);
        }
        if (Flag_380(this) && !Is_Selected(this)) {
            if (unknown_slot49(unknown_player_a35db4)) {
                if (unknown_slot51(true)) {
                    unknown_slot248(point, window, true);
                }
            } else {
                unknown_slot248(point, window, true);
            }
        }




        if (unknown_talk_techno_ac01c4 == this && unknown_talk_value_ac01c8 > 0 && (long)TalkBubbleTimer > 0) {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4,
                static_cast<ShapeFileStruct const *>(unknown_talkbubl_shapes_a72fec), unknown_talk_value_ac01c8 - 1,
                point, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
    }
}


void __fastcall Draw_Double_Selection_Bracket_006c3950(::Coord const &first,::Coord const &second,int color)
{
    ::Coord first_tick=(first+first+first+second)/4;
    ::Coord second_tick=(second+second+second+first)/4;
    if (first.Z>first_tick.Z) {
        unknown_tactical_83996c->unknown_slot24(reinterpret_cast<CoordStruct const &>(first),reinterpret_cast<CoordStruct const &>(first_tick),color,0);
    } else {
        unknown_tactical_83996c->unknown_slot24(reinterpret_cast<CoordStruct const &>(first_tick),reinterpret_cast<CoordStruct const &>(first),color,0);
    }
    if (second.Z>second_tick.Z) {
        unknown_tactical_83996c->unknown_slot24(reinterpret_cast<CoordStruct const &>(second),reinterpret_cast<CoordStruct const &>(second_tick),color,0);
    } else {
        unknown_tactical_83996c->unknown_slot24(reinterpret_cast<CoordStruct const &>(second_tick),reinterpret_cast<CoordStruct const &>(second),color,0);
    }
}


// Draw_Single_Selection_Bracket (0x006C3A90)
// Inline, defined above TechnoClass::unknown_slot66 (0x006C2C40). No source of its own: emitted
// by this unit because the last infantry site in slot 66 refuses it.


// void TechnoClass::unknown_slot247(int arg1, int arg2) (0x006C3B20, slot 247)
// Empty.

void TechnoClass::unknown_slot247(int arg1, int arg2)
{
}


// TechnoClass slot 65 0x006C3B30: Draw_Pre_Render (TibSun techno.cpp). Draws the rear edges of a
// selected building's selection box before the building body.
void TechnoClass::unknown_slot65(int point, int cliprect)
{
    if (Kind_Of() == 6 && unknown_77 && Kind_Of() != 15) {
        int color = 15;
        if (Get_Height_AGL() < -4) {
            color = 12;
        }
        CoordStruct const &lepton = Techno_Type_Class()->Lepton_Dimensions();
        ::Coord dim = lepton;
        dim.X /= 2;
        dim.Y /= 2;
        int x = dim.X;
        int y = dim.Y;
        color = unknown_drawer_8323c4->Convert(color);
        CoordStruct const &position = Center_Coord();
        ::Coord center = position;
        Draw_Double_Selection_Bracket_006c3950(center + ::Coord(-x, -y, 0), center + ::Coord(-x, -y, dim.Z), color);
        Draw_Double_Selection_Bracket_006c3950(center + ::Coord(-x, -y, 0), center + ::Coord(x, -y, 0), color);
        Draw_Double_Selection_Bracket_006c3950(center + ::Coord(-x, -y, 0), center + ::Coord(-x, y, 0), color);
        Draw_Double_Selection_Bracket_006c3950(center + ::Coord(-x, -y, dim.Z), center + ::Coord(-x, y, dim.Z), color);
        Draw_Double_Selection_Bracket_006c3950(center + ::Coord(-x, -y, dim.Z), center + ::Coord(x, -y, dim.Z), color);
    }
}


// Not exact (RA2_NON_MATCHING): TechnoClass::unknown_slot248 at 0x006C3F00, best score 58.26, 2026-11-13; differs: 461 instructions in the original, 459 in the draft (shape 92.83)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x6C3F00 TechnoClass::unknown_slot248 (health bar)
// entry 0x6C3F00; exact-match score 58.26 (misses_index best), this text 58.26; plan "draft as-is"
// origin lane: Scout1059 (build/class-work/Scout1059/keep/hb2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1098 (native/drafts.json). Not original bytes.
// void TechnoClass::unknown_slot248(Point2D const &point, Rect const &window, bool arg3) (0x006C3F00)
// Draws the health bar: buildings get a diagonal pip bar sized from the
// type's lepton dimensions, others the select bracket and a pip row; then the
// pips (slot 249) for allied or spied-on objects.

void TechnoClass::unknown_slot248(Point2D const &point, Rect const &window, bool arg3)
{
    if (Kind_Of() == 6) {
        Get_Height_AGL();
        CoordStruct dim = Techno_Type_Class()->Lepton_Dimensions();
        CoordStruct coord = dim - dim / 2;
        coord.X = -coord.X;
        coord.Z = dim.Z;
        Point2D p0 = unknown_tactical_83996c->Coord_To_Pixel_Absolute(coord);
        coord.X = -coord.X;
        coord.Y = -coord.Y;
        coord.X = -coord.X;
        Point2D p1 = unknown_tactical_83996c->Coord_To_Pixel_Absolute(coord);
        coord.Z = 0;
        coord.Y = -coord.Y;
        Point2D p2 = unknown_tactical_83996c->Coord_To_Pixel_Absolute(coord);
        coord.X = -coord.X;
        coord.Y = -coord.Y;
        coord.X = -coord.X;

        int length = (p0.Y - p1.Y) / 2;
        int count = (int)(Health_Ratio() * length);
        if (count <= 1) {
            count = 1;
        }
        if (count >= length) {
            count = length;
        }
        int frame = 1;
        if ((char)unknown_func_5d4600()) {
            frame = 2;
        } else if ((char)unknown_func_5d45b0()) {
            frame = 4;
        }
        int ybase = 2 - 2 * length;
        Point2D position;
        int index;
        int yoff = 0;
        int xoff = 0;
        for (index = 0; index < count; index++) {
            position.X = point.X + p0.X + 4 * length + 3;
            position.Y = point.Y + p0.Y + ybase + 2;
            position.X -= xoff;
            position.Y -= yoff;
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pips_shapes_a72fe4,
                          frame, position, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            xoff += 4;
            yoff -= 2;
        }
        yoff = -2 * count;
        xoff = 4 * count;
        for (index = count; index < length; index++) {
            position.X = point.X + p0.X + 4 * length + 3;
            position.Y = point.Y + p0.Y + ybase + 2;
            position.X -= xoff;
            position.Y -= yoff;
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pips_shapes_a72fe4,
                          0, position, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            xoff += 4;
            yoff -= 2;
        }
        if (House->Is_Ally(unknown_player_a35db4) || (unknown_1a8 & (1 << unknown_player_a35db4->Type->House))) {
            unknown_slot249(point + p2, point, window);
        }
    } else {
        Point2D offset;
        int length;
        if (Kind_Of() == 15) {
            if (unknown_77) {
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pipbrd_shapes_a72fe0,
                              1, point + Point2D(11, -25), window, 0xE00, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
            offset = Point2D(-5, -24);
            length = 8;
        } else {
            if (unknown_77) {
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pipbrd_shapes_a72fe0,
                              0, point + Point2D(1, -26), window, 0xE00, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
            offset = Point2D(-15, -25);
            length = 17;
        }
        int count = (int)(Health_Ratio() * length);
        if (count <= 1) {
            count = 1;
        }
        if (count >= length) {
            count = length;
        }
        int frame = 15;
        if (Health_Ratio() <= Rules->unknown_1368) {
            frame = 16;
        }
        if (Health_Ratio() <= Rules->unknown_1370) {
            frame = 17;
        }
        Point2D position;
        for (int index = 0; index < count; index++) {
            position.X = point.X + offset.X;
            position.Y = offset.Y + point.Y;
            position.X += 2 * index;
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pips_shapes_a72fe4,
                          frame, position, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
        if (House->Is_Ally(unknown_player_a35db4) || (unknown_1a8 & (1 << unknown_player_a35db4->Type->House))) {
            unknown_slot249(point + Point2D(-10, 10), point, window);
        }
    }
}
#endif

// bool TechnoClass::Limbo() (0x006C4480, slot 52)
// Takes an active techno off the map: slot 263 (revealing to the player for
// RevealToAll types), clears its sight (0x0054A550 around +0x1F4), GapGenerator
// slot 236, BombSight types leave the 0x008322E0 list, the owner forgets it
// (0x004ED440); with slot 168 positive the cell (a foot's +0x454, else slot
// 105) takes back -+0x400 for the owner; slot 265 for +0x37B; the audio at
// +0x3D8 stops; then RadioClass slot 52.

bool TechnoClass::Limbo()
{
    if (!unknown_76 && Techno_Type_Class() != 0) {
        if (Techno_Type_Class()->RevealToAll) {
            unknown_slot263(0, 0, true, unknown_player_a35db4);
        } else {
            unknown_slot263(0, 0, false, 0);
        }
        Map.Unknown_0054A550(CoordStruct(Coord), unknown_1f4 - 3, unknown_1f4 + 3, 0);
        if (Techno_Type_Class()->GapGenerator) {
            unknown_slot236();
        }
        if (Techno_Type_Class()->BombSight != 0) {
            unknown_list_8322e0.Unknown_004366D0(this);
        }
        House->Unknown_004ED440(this, false);
        if (unknown_slot168() > 0) {
            UnknownCellClass *cell;
            if (this != 0 && AbstractFlags.IsFoot) {
                cell = (UnknownCellClass *)&Map[((FootClass *)this)->unknown_454];
                if (cell != 0) {
                    cell->Adjust_Threat(Owner(), -unknown_400);
                    unknown_400 = 0;
                }
            } else {
                cell = (UnknownCellClass *)Get_Cell_Ptr();
                if (cell != 0) {
                    cell->Adjust_Threat(Owner(), -unknown_400);
                    unknown_400 = 0;
                }
            }
        }
        if (unknown_37b) {
            unknown_slot265();
        }
        unknown_3d8.Unknown_00406070();
        unknown_3ec = 1;
        unknown_3f0 = 0;
    }
    return RadioClass::Limbo();
}


// bool TechnoClass::Unlimbo(CoordStruct const *coord, int dir) (0x006C4610, slot 53)
// Puts the techno on the map at coord facing dir (the low byte, as a
// 256-step direction): +0x32D records whether the cell is on the map
// (0x0055A660), then ObjectClass slot 53. An active object then: slot 262,
// sight (0x0054A550 out to +0x1F4 + 3), joins the 0x008322E0 list for
// BombSight types, the owner's 0x004ED610, both facings (the turret pointed
// back by the type's FireAngle), +0x3EC/+0x3F0, slot 261 (with +0x190 set
// for infantry), slot 117 when slot 122 says so, the height level (+0x378),
// +0x373 cleared off the map, the cell's slot 168 weight (+0x400), the map
// position at +0x1A0, and the owner's +0x130 when +0x114 is unset.

bool TechnoClass::Unlimbo(CoordStruct const *coord, int dir)
{
    unknown_32d = Map.Unknown_0055A660(Coord_Cell(*coord), true);
    if (ObjectClass::Unlimbo(coord, dir)) {
        if (!unknown_7d) {
            return true;
        }
        unknown_slot262(0, 0, 0, 0);
        Map.Unknown_0054A550(CoordStruct(Coord), 0, unknown_1f4 + 3, 0);
        if (Techno_Type_Class()->BombSight != 0) {
            unknown_list_8322e0.Unknown_00436680(this);
        }
        House->Unknown_004ED610(this, false);
        unknown_2e0.Set(UnknownFacingDir((unsigned short)((unsigned char)dir << 8)));
        unknown_2c8.Set(UnknownFacingDir(0x4000));
        {
            UnknownFacingDir fire = UnknownFacingDir((unsigned short)(0x4000 - ((unsigned char)Techno_Type_Class()->FireAngle << 8)));
            unknown_2c8.Set_Desired(fire);
        }
        unknown_3ec = 1;
        unknown_3f0 = 0;
        if (Kind_Of() == 15) {
            unknown_190 = true;
        }
        unknown_slot261(true, true);
        unknown_190 = false;
        if (unknown_slot122()) {
            Commence();
        }
        unknown_378 = (char)(coord->Z / Rules->unknown_1324) * 10;
        if (!Map.Unknown_0055A660(Coord_Cell(*coord), true)) {
            unknown_373 = false;
        }
        UnknownCellClass *here = (UnknownCellClass *)Get_Cell_Ptr();
        if (here != 0) {
            unknown_400 = unknown_slot168();
            here->Adjust_Threat(Owner(), unknown_400);
        }
        unknown_1a0 = Map.Coord_To_Radar_Pixel(CoordStruct(Coord), true);
        if (unknown_114 == -1) {
            unknown_114 = House->unknown_130;
        }
        return true;
    }
    return false;
}


// 0x006c48d0: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: int Get_Sight_Bonus_006c48d0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006c48d0 {

extern "C" unsigned int Rule_00839848;

int __stdcall Get_Sight_Bonus_006c48d0(int type)
{
    return (*(int *)(type + 8) / *(int *)(Rule_00839848 + 0x1324)) * 10;
}

}  // namespace seed_006c48d0


// int TechnoClass::unknown_6c48f0(ObjectClass *other) (0x006C48F0)
// The distance to the other object when its height is measured like
// 0x006C4A70: the horizontal part (levels above, scaled and limited, in
// leptons) and the vertical part (the same levels times the geometry
// constant 0x00AC01C0) combined; 0 unless both are on the map (slot 20).

int TechnoClass::unknown_6c48f0(ObjectClass *other)
{
    if (On_Ground() && other->On_Ground()) {
        Cell here = Coord_Cell(Center_Coord());
        Cell there = Coord_Cell(other->Center_Coord());
        UnknownCellClass *source = (UnknownCellClass *)&Map[here];
        UnknownCellClass *target = (UnknownCellClass *)&Map[there];
        int rise;
        if (source->Unknown_0047CF10() - target->Unknown_0047CF10() < 0) {
            rise = 0;
        } else {
            rise = source->Unknown_0047CF10() - target->Unknown_0047CF10();
        }
        double scaled = rise / Rules->unknown_1480 * Rules->unknown_1488;
        double limit = Rules->unknown_1490;
        if (scaled >= limit) {
            scaled = Rules->unknown_1490;
        }
        int vertical = unknown_geometry_g * rise;
        int horizontal = (int)scaled << 8;
        return (int)unknown_sqrt_4ba410((double)(horizontal * horizontal + vertical * vertical));
    }
    return 0;
}


// int TechnoClass::unknown_6c4a70(ObjectClass *other) (0x006C4A70)
// How high this object's cell stands above the other's (levels, never
// negative), divided by the rules' +0x1480, scaled by +0x1488 and limited
// to +0x1490, in leptons (<< 8); 0 unless both are on the map (slot 20).

int TechnoClass::unknown_6c4a70(ObjectClass *other)
{
    if (On_Ground() && other->On_Ground()) {
        Cell here = Coord_Cell(Center_Coord());
        Cell there = Coord_Cell(other->Center_Coord());
        UnknownCellClass *source = (UnknownCellClass *)&Map[here];
        UnknownCellClass *target = (UnknownCellClass *)&Map[there];
        int rise;
        if (source->Unknown_0047CF10() - target->Unknown_0047CF10() < 0) {
            rise = 0;
        } else {
            rise = source->Unknown_0047CF10() - target->Unknown_0047CF10();
        }
        double scaled = rise / Rules->unknown_1480 * Rules->unknown_1488;
        double limit = Rules->unknown_1490;
        if (scaled >= limit) {
            scaled = Rules->unknown_1490;
        }
        return (int)scaled << 8;
    }
    return 0;
}


// bool TechnoClass::unknown_6c4bb0(CoordStruct const *coord, AbstractClass *target, WeaponTypeClass *weapon) (0x006C4BB0):
// whether weapon fired from coord reaches target (TibSun TechnoTypeClass::In_Range, RA2 form with the range bonuses).

CellClass *__fastcall Find_First_Impenetrable_Obstacle_004bbaa0(CoordStruct const &from, CoordStruct const &to, WeaponTypeClass const *weapon, HouseClass const *house);

static inline int Distance_3D(CoordStruct const &a, CoordStruct const &b)
{
    CoordStruct delta = a - b;
    double y = delta.Y;
    double z = delta.Z;
    double x = delta.X;
    return (int)Fast_Sqrt(x * x + z * z + y * y);
}

static inline BuildingClass *As_Building(AbstractClass *target)
{
    if (target->Kind_Of() == 6) {
        return (BuildingClass *)target;
    }
    return 0;
}

static inline int Length_3D(CoordStruct const &delta)
{
    return (int)Fast_Sqrt((double)delta.X * delta.X + ((double)delta.Y * delta.Y + (double)delta.Z * delta.Z));
}

static inline int Length_XY(Point2D const &point)
{
    return (int)Fast_Sqrt((double)point.X * point.X + (double)point.Y * point.Y);
}

static inline Point2D Coord_XY(CoordStruct const &coord)
{
    return Point2D(coord.X, coord.Y);
}

static inline int Length_2D(CoordStruct const &coord)
{
    return (int)Fast_Sqrt((double)coord.X * coord.X + (double)coord.Y * coord.Y);
}

bool TechnoClass::unknown_6c4bb0(CoordStruct const *coord, AbstractClass *target, WeaponTypeClass *weapon)
{
    if (target != 0 && weapon != 0) {
        int range = weapon->unknown_b4;
        if (range == -512) {
            return true;
        }
        if (target->In_Air()) {
            range += Techno_Type_Class()->AirRangeBonus;
        }
        BulletTypeClass *bullet = weapon->Projectile;
        int ground_range = range;
        if (bullet->SubjectToElevation) {
            ground_range = unknown_6c48f0((ObjectClass *)target) + range;
        }
        CoordStruct tcoord = target->Center_Coord();
        int minrange = weapon->unknown_b8;
        if (minrange != 0 && Distance_3D(*coord, tcoord) < minrange) {
            return false;
        }
        if (bullet->Arcing) {
            CoordStruct delta = tcoord - *coord;
            int dist = Length_XY(Coord_XY(delta));
            int arcrange = range;
            if (bullet->SubjectToElevation) {
                arcrange = unknown_6c4a70((ObjectClass *)target) + range;
            }
            if (dist > arcrange) {
                return false;
            }
            double gravity = weapon->Projectile->Floater ? Unknown_0047FCD0() : Rules->unknown_1320;
            int speed = Unknown_0047FB70(arcrange, gravity);
            if (!Is_Projectile_Trajectory_Valid(speed, dist, delta.Z, gravity)) {
                return false;
            }
            if ((Map[tcoord].unknown_140 & 0x100) && tcoord.Z - coord->Z >= unknown_geometry_g * 3) {
                return false;
            }
            bool clear = Find_First_Impenetrable_Obstacle_004bbaa0(*coord, tcoord, weapon, House) == 0;
                return clear;
        } else {
            BuildingClass *building = As_Building(target);
            if (building != 0) {
                ground_range += (building->Type->Foundation_Height(false) + building->Type->Width()) * 64;
            }
            bool inrange;
            if (Kind_Of() != 3) {
                inrange = Length_3D(*coord - tcoord) <= ground_range;
            } else {
                inrange = Length_XY(Coord_XY(*coord) - Coord_XY(tcoord)) <= ground_range;
            }
            if (inrange) {
                if (Map[*coord].unknown_140 & 0x100) {
                    int height = Map.Get_Height_GL(*coord) + unknown_geometry_j;
                    if (coord->Z < height && tcoord.Z >= height) {
                        return false;
                    }
                }
                bool clear = Find_First_Impenetrable_Obstacle_004bbaa0(*coord, tcoord, weapon, House) == 0;
                return clear;
            }
        }
    }
    return false;
}


// bool TechnoClass::unknown_slot211(ObjectClass *target) (0x006C4F50, slot 211)
// Whether target is within the primary weapon's warhead spread (CellSpread
// minus CellInset) of this object, measured in cells.

bool TechnoClass::unknown_slot211(ObjectClass *target)
{
    if (unknown_slot225()) {
        WarheadTypeClass *warhead = unknown_slot229()->WeaponType->Warhead;
        if (warhead != 0) {
            float range = warhead->CellSpread;
            range -= warhead->CellInset;
            Cell here = Coord_Cell(Center_Coord());
            Cell there = Coord_Cell(target->Center_Coord());
            Cell delta = here - there;
            if ((short)Short_Length(delta.X, delta.Y) <= range) {
                return true;
            }
        }
    }
    return false;
}


// void TechnoClass::unknown_slot213(int cell) (0x006C5070, slot 213)
// Slot 212 with slot 174 of the same cell.

bool TechnoClass::unknown_slot213(AbstractClass *target)
{
    return unknown_slot212(target, unknown_slot174(target));
}


// bool TechnoClass::unknown_slot212(AbstractClass *target, int weapon) (0x006C50A0, slot 212)
// Whether the weapon can reach target: slot 21 of this raises the origin to
// the target's height, a weapon with +0x110 fires from the centre of its cell,
// then 0x006C4BB0 tests the range.

bool TechnoClass::unknown_slot212(AbstractClass *target, int weapon)
{
    if (target == 0) {
        return true;
    }
    CoordStruct coord = Center_Coord();
    WeaponTypeClass *type = unknown_slot231(weapon)->WeaponType;
    if (type != 0 && type->unknown_110) {
        coord = ((AbstractClass *)&Map[Coord_Cell(Center_Coord())])->Center_Coord();
    }
    if (In_Air()) {
        coord.Z = target->Center_Coord().Z;
    }
    return unknown_6c4bb0(&coord, target, type);
}


// bool TechnoClass::unknown_slot215(AbstractClass *target) (0x006C51A0, slot 215)
// Slot 212 with slot 227's weapon when that weapon's type has +0x112,
// otherwise slot 213.

bool TechnoClass::unknown_slot215(AbstractClass *target)
{
    int weapon = unknown_slot227();
    WeaponStruct *slot = unknown_slot231(weapon);
    if (slot != 0 && slot->WeaponType != 0 && slot->WeaponType->unknown_112) {
        return unknown_slot212(target, weapon);
    } else {
        return unknown_slot213(target);
    }
}


// bool TechnoClass::unknown_slot214(CoordStruct const &coord) (0x006C5200, slot 214)
// Slot 213 for the cell of a coordinate.

bool TechnoClass::unknown_slot214(CoordStruct const &coord)
{
    AbstractClass *cell = (AbstractClass *)&Map[coord];
    return unknown_slot212(cell, unknown_slot174(cell));
}


// bool TechnoClass::In_Range(CoordStruct const &coord, int weapon) (0x006C5240, slot 84)
// Slot 212 for the cell object at coord.

bool TechnoClass::In_Range(CoordStruct const &coord, int weapon)
{
    return unknown_slot212((AbstractClass *)&Map[coord], weapon);
}


// Evaluate the objects occupying a cell, retaining the last eligible techno.
// Existing seven-argument ABI retained; +418 type and cell views use shared layouts.
bool TechnoClass::unknown_6c6130(int method,int mask,Cell const *cell,int range,ObjectClass **object,int *value,int zone)
{
    *object=0;
    *value=0;
    UnknownCellClass *cellptr=reinterpret_cast<UnknownCellClass *>(&Map[*cell]);
    if(zone!=-1 && Map.Zone(*cell,Techno_Type_Class()->MovementZone,true)!=zone) return false;
    ObjectClass *tentative=cellptr->unknown_e8;
    TechnoClass *tech=0;
    if(!tentative) tentative=cellptr->unknown_e4;
    while(tentative) {
        if(tentative!=this) {
            if(!tentative) tech=0;
            else {
                if(tentative->AbstractFlags.IsTechno) tech=static_cast<TechnoClass *>(tentative);
                else tech=0;
            }
            if(tech) {
                if(unknown_6c16f0(-1)<0) {
                    if(tech->Health_Ratio()<Rules->unknown_1360 && House->Is_Ally(tech)) break;
                } else {
                    if(!House->Is_Ally(tech) || Techno_Type_Class()->AttackFriendlies
                       || (Kind_Of()==15 && (static_cast<InfantryClass *>(this)->unknown_5c0
                       || (static_cast<InfantryClass *>(this)->Type->Engineer && Mission==11
                           && tech->Health_Ratio()<=Rules->unknown_1370 && tech->Kind_Of()==6
                           && reinterpret_cast<TechnoTypeClass *>(static_cast<BuildingClass *>(tech)->Type)->Cost_Of(tech->House)>0)))) break;
                }
            }
        }
        tentative=tentative->unknown_30;
    }
    if(!tech) return false;
    *object=tech;
    bool result=false;
    if(!House->Is_Ally(tech) || unknown_6c16f0(-1)<0
       || (!House->Is_Human_Player() && this && Kind_Of()==15
           && As_Infantry(static_cast<FootClass *>(this))->Type->Engineer)) {
        result=unknown_6c5570(method,mask,range,tech,value,-1,&unknown_geometry_l);
    }
    return result;
}


// Not exact (RA2_NON_MATCHING): TechnoClass::unknown_6c5270 at 0x006C5270, best score 96.38, 2026-11-13; differs: same instruction count (221), shape 97.74: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x6C5270 FUN_006c5270
// entry 0x6C5270; exact-match score 96.38 (misses_index best); plan "draft as-is"
// origin lane: Scout898 (build/class-work/Scout898/w/am0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// double TechnoClass::unknown_6c5270(Cell const &cell) (0x006C5270)
// A fire-suppression factor (hypothesis): 1.0 unless the slot 230 weapon has
// +0x11E set; then halved for each building of an allied house on the square
// rings of radius 1 up to Rules FireSupress cells (in leptons / 256) around
// the cell, cells outside the map skipped.

double TechnoClass::unknown_6c5270(Cell const &cell)
{
    WeaponStruct *weapon = unknown_slot230();
    if (weapon == 0 || weapon->WeaponType == 0 || !weapon->WeaponType->unknown_11e) {
        return 1.0;
    }
    double factor = 1.0;
    int range = Rules->FireSupress / 256;
    for (int radius = 1; radius < range; radius++) {
        for (int x = -radius; x <= radius; x++) {
            Cell spot;
            if (cell.X + x >= Map.unknown_120 && cell.X + x < Map.unknown_120 + Map.unknown_128) {
                if (cell.Y - radius >= Map.unknown_124) {
                    spot = cell + Cell(x, -radius);
                    BuildingClass *building = ((UnknownCellClass *)&Map[spot])->Unknown_004717B0();
                    if (building != 0 && House->Is_Ally(building)) {
                        factor *= 0.5;
                    }
                }
                if (cell.Y + radius < Map.unknown_124 + Map.unknown_12c) {
                    spot = cell + Cell(x, radius);
                    BuildingClass *building = ((UnknownCellClass *)&Map[spot])->Unknown_004717B0();
                    if (building != 0 && House->Is_Ally(building)) {
                        factor *= 0.5;
                    }
                }
            }
        }
        for (int y = -radius + 1; y < radius; y++) {
            Cell spot;
            if (cell.Y + y >= Map.unknown_124 && cell.Y + y < Map.unknown_124 + Map.unknown_12c) {
                if (cell.X - radius >= Map.unknown_120) {
                    spot = cell + Cell(-radius, y);
                    BuildingClass *building = ((UnknownCellClass *)&Map[spot])->Unknown_004717B0();
                    if (building != 0 && House->Is_Ally(building)) {
                        factor *= 0.5;
                    }
                }
                if (cell.X + radius < Map.unknown_120 + Map.unknown_128) {
                    spot = cell + Cell(radius, y);
                    BuildingClass *building = ((UnknownCellClass *)&Map[spot])->Unknown_004717B0();
                    if (building != 0 && House->Is_Ally(building)) {
                        factor *= 0.5;
                    }
                }
            }
        }
    }
    return factor;
}
#endif

// Not exact (RA2_NON_MATCHING): TechnoClass::unknown_6c5570 at 0x006C5570, best score 91.70, 2026-11-13; differs: 920 instructions in the original, 923 in the draft (shape 99.84)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x6C5570 FUN_006c5570
// entry 0x6C5570; exact-match score 91.81 (misses_index best), this text 91.7; plan "draft, verify"
// origin lane: Scout668 (build/class-work/Scout668/w/keep/eo_9170.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// TechnoClass::unknown_6c5570 (TibSun Evaluate_Object).
#include "shared/building.hpp"
static inline int Coord_Size(CoordStruct const &c)
{
    double y=c.Y;
    return (int)Fast_Sqrt((double)c.X*(double)c.X+y*y+(double)c.Z*(double)c.Z);
}
bool TechnoClass::unknown_6c5570(int method,int mask,int range,ObjectClass *object,int *value,int zone,CoordStruct const *coord)
{
    TechnoClass *tech=static_cast<TechnoClass *>(object);
    bool engineer=unknown_slot193();
    int which=unknown_slot174(object);
    WeaponTypeClass *weapon=unknown_slot231(which)->WeaponType;
    if(!(method&0x18200) && unknown_slot217(object,which)==5) return false;
    if(object->unknown_70<=0 && Techno_Type_Class()->VHPScan==2) return false;
    if(weapon && weapon->Warhead) {
        if((int)(weapon->Warhead->Verses[object->Techno_Type_Class()->unknown_9c]*100.0)==1) return false;
    }
    if(object==0 || object->unknown_76 || object->Strength==0) return false;
    if(tech->unknown_1b8==2) {
        if(!Map[object->Center_Coord()].Is_Sensed(House->ArrayIndex) && House!=tech->House) return false;
    }
    if(!tech->unknown_32d) return false;
    if(tech->Control()->unknown_04) return false;
    if(object->Get_Height_AGL()<-20) return false;
    CoordStruct objectcoord=object->Center_Coord();
    if(zone!=-1 && Map.Zone(Coord_Cell(objectcoord),Techno_Type_Class()->MovementZone,object->unknown_79)!=zone) return false;
    if((Kind_Of()!=15 || !static_cast<InfantryClass *>(this)->unknown_5c0) && !Techno_Type_Class()->AttackFriendlies && House->Is_Ally(tech)) {
        if(unknown_6c16f0(-1)>=0 && !engineer) return false;
        if(object->Health_Ratio()==Rules->unknown_1360) return false;
        if(object->Kind_Of()==2 && Kind_Of()==1) {
            if(object->Get_Height_AGL()>0) return false;
            if(Map[object->Center_Coord()].Cell_Building()) return false;
        } else if(Kind_Of()==1 && !object->unknown_slot31()) return false;
    }
    if(unknown_id_source_a3d290->unknown_000&0x800) {
        ObjectTypeClass *type=object->Class_Of();
        for(int i=0;i<Rules->unknown_9ec.Count();i++) {
            if(Rules->unknown_9ec[i]==type) return false;
        }
    }
    int dist;
    if(object->In_Air()) {
        double x=object->Center_Coord().X-Center_Coord().X;
        double y=object->Center_Coord().Y-Center_Coord().Y;
        dist=(int)Fast_Sqrt(y*y+x*x);
        if(range>0 && dist>range) return false;
    } else {
        dist=Coord_Size(Center_Coord()-object->Center_Coord());
        if(range>0 && dist>range) return false;
    }
    if(range==0) {
        if(!unknown_slot163()) {
            if(dist>Techno_Type_Class()->GuardRange.Value) return false;
        } else {
            if(!unknown_slot212(object,which)) return false;
        }
    }
    if(House->Is_Player_Control() && !tech->unknown_372 && !tech->unknown_373 && unknown_game_mode_a3d298==0 && object->Kind_Of()!=2) return false;
    if(object->Kind_Of()==6 && static_cast<BuildingClass *>(object)->Type->InvisibleInGame) return false;
    int otype=object->Kind_Of();
    if(!((1<<otype)&mask) && (!(mask&2) || !object->unknown_slot31())) return false;
    TechnoTypeClass *tclass=object->Techno_Type_Class();
    if(!tclass->unknown_201) return false;
    if(unknown_game_mode_a3d298!=0 && tech->House->Type->MultiplayPassive) {
        if(House->Is_Human_Player() || object->Kind_Of()!=6 || Kind_Of()!=15) return false;
        InfantryClass *inf=static_cast<InfantryClass *>(this);
        bool defense=false;
        if(inf->Team && inf->Team->Class->IsBaseDefense) defense=true;
        bool capture;
        if(!static_cast<BuildingClass *>(object)->Type->NeedsEngineer || !inf->Type->Engineer || tech->House==House) capture=false; else capture=true;
        bool occupy;
        if(!inf->Type->Occupier || !static_cast<BuildingClass *>(object)->Unknown_00452EB0(this)) occupy=false; else occupy=true;
        if(unknown_game_mode_a3d298==0 || defense || (!capture && !occupy)) return false;
    }
    if(tclass->unknown_202 && (object->Kind_Of()!=6 || tech->House->Type->MultiplayPassive)) {
        if(House->Is_Human_Player() || object->Kind_Of()!=6 || Kind_Of()!=15) return false;
        InfantryClass *inf=static_cast<InfantryClass *>(this);
        bool defense=false;
        if(inf->Team && inf->Team->Class->IsBaseDefense) defense=true;
        bool capture;
        if(!static_cast<BuildingClass *>(object)->Type->NeedsEngineer || !inf->Type->Engineer || tech->House==House) capture=false; else capture=true;
        bool occupy;
        if(!inf->Type->Occupier || !static_cast<BuildingClass *>(object)->Unknown_00452EB0(this)) occupy=false; else occupy=true;
        if(unknown_game_mode_a3d298==0 || defense || (!capture && !occupy)) return false;
    }
    if(tclass->IsTrain && Kind_Of()==15 && static_cast<InfantryClass *>(this)->Type->VehicleThief) return false;
    if(object->unknown_slot49(House) && !Techno_Type_Class()->DetectDisguise) {
        if(!tech->unknown_184.Value()) return false;
        if(House->Is_Human_Player()) return false;
        int roll=unknown_id_source_a3d290->Random()(0,99);
        if(roll>Rules->DisabledDisguiseDetectionPercent[House->unknown_c8]) return false;
    }
    if(method&0x100) {
        object->Owning_House();
        return false;
    }
    if((method&0x200) && (otype!=6 || !static_cast<BuildingTypeClass *>(tclass)->Capturable)) return false;
    bool foot=false;
    if(this) foot=AbstractFlags.IsFoot;
    if((!foot || !static_cast<FootClass *>(this)->Team) && House->Is_Human_Player() && !object->unknown_slot31() && otype==6) {
        WeaponStruct *armed=tech->unknown_slot230();
        if((!armed || !armed->WeaponType || !object->Techno_Type_Class()->ThreatPosed) && !engineer) return false;
    }
    if(engineer) {
        if(object->Kind_Of()!=6) return false;
        if(House->Is_Ally(tech->House)) {
            if(object->Health_Ratio()>Rules->unknown_1370) return false;
            if(!static_cast<BuildingClass *>(object)->Type->Cost_Of(House)) return false;
        }
    }
    if((method&0x40) && !tclass->Storage) return false;
    CellClass *here=&Map[Position_Coord_004f3f40()];
    CellClass *there=&Map[object->Position_Coord_004f3f40()];
    if((here->unknown_140&0x100) && (there->unknown_140&0x100) && unknown_79!=object->unknown_79) return false;
    *value=(int)unknown_6d7550(object,*coord);
    if(Techno_Type_Class()->VHPScan==1) {
        if(object->unknown_70<=0) *value=*value/2;
        else if(object->unknown_70<=object->Techno_Type_Class()->MaxStrength/2) *value=*value*2;
    }
    if(House->unknown_190 && House->unknown_54f0!=-1 && tech->House!=Houses[House->unknown_54f0]) *value=1;
    BuildingTypeClass *btype=static_cast<BuildingTypeClass *>(tclass);
    if((method&0x800) && otype==6) {
        if(btype->Power>0) *value=btype->Power*1000+*value;
        else *value=0;
    }
    if(method&0x8000) {
        if(otype!=6 || btype->MaxNumberOccupants<=0) return false;
        *value+=btype->MaxNumberOccupants*1000;
    }
    if(method&0x10000) {
        if(otype!=6 || !btype->NeedsEngineer) return false;
        *value+=1000;
    }
    if((method&0x1000) && otype==6 && btype->Factory==0) *value=0;
    if(method&0x2000) {
        WeaponStruct *w=unknown_slot230();
        bool keep=false;
        if(w && w->WeaponType && otype==6) {
            BuildingClass *b=object->Kind_Of()==6?static_cast<BuildingClass *>(object):0;
            // behaviour fix (codex4 r505): a building that cannot be occupied keeps its score.
            if(!b->Type->CanBeOccupied || b->unknown_4532f0()!=0) keep=true;
        }
        if(!keep) *value=0;
    }
    double areamod=unknown_6c5270(Coord_Cell(object->Center_Coord()));
    if(areamod!=1.0) *value=(int)(areamod*(double)*value);
    if(*value!=0) {
        *value=*value<1?1:*value;
        return true;
    }
    *value=0;
    return false;
}
#endif

// int TechnoClass::unknown_6c6350(Cell const *cell) (0x006C6350)
// How much a computer-owned techno wants to shoot the wall in cell
// (TechnoClass slot 219): 0 for a human owner, a difficulty without
// DestroyWalls, a cell without a wall overlay, a cell out of range of the
// weapon chosen for it, a weapon without a Wall warhead, a projectile not
// AG, or a wall owned by an ally; otherwise slot 85 less the distance to
// the cell's centre.

int TechnoClass::unknown_6c6350(Cell const *cell)
{
    if (House->Unknown_004F52F0()) {
        return 0;
    }
    if (!Rules->Diff[House->unknown_c8].DestroyWalls) {
        return 0;
    }
    UnknownCellClass *wall = (UnknownCellClass *)&Map[*cell];
    if (wall->unknown_44 == -1 || !unknown_overlay_types_a35de8[wall->unknown_44]->unknown_278) {
        return 0;
    }
    int weapon = unknown_slot174((AbstractClass *)&Map[*cell]);
    if (!In_Range(CoordStruct(cell->X * 256 + 128, cell->Y * 256 + 128, 0), weapon)) {
        return 0;
    }
    WeaponStruct *primary = unknown_slot230();
    if (primary == 0 || primary->WeaponType == 0) {
        return 0;
    }
    WarheadTypeClass *warhead = primary->WeaponType->Warhead;
    if (warhead == 0) {
        return 0;
    }
    if (primary->WeaponType->Projectile != 0 && !primary->WeaponType->Projectile->AG) {
        return 0;
    }
    if (!warhead->Wall || wall->unknown_50 == -1 || House->Is_Ally(Houses[wall->unknown_50])) {
        return 0;
    }
    int distance = Offset_Length(Center_Coord() - CoordStruct(cell->X * 256 + 128, cell->Y * 256 + 128, 0));
    return Weapon_Range(0) - distance;
}


// Target scan: original Techno primary slot219. Flags are retained as target values.
extern int unknown_target_scan_a40be4;
AbstractClass *TechnoClass::unknown_slot219(int method, CoordStruct const *coord, bool only_enemy)
{
    ObjectClass *best_object=0;
    int best_value=-1;
    int zone=-1;
    ++unknown_target_scan_a40be4;
    if (Techno_Type_Class()->NoAutoFire && House->Is_Human_Player()) return 0;
    if (!(method&1) && Kind_Of()!=6 && Kind_Of()!=2) {
        zone=Map.Zone(Cell(Center_Coord()),Techno_Type_Class()->MovementZone,true);
    }
    if (Kind_Of()==15) {
        if (unknown_6c16f0(-1)<0) method=(method&3)|0x4008;
        else if (static_cast<InfantryClass *>(this)->Type->Engineer) method&=~0x18;
    } else if (Kind_Of()==1) {
        if (unknown_6c16f0(-1)<0) method=(method&3)|0x4010;
    }
    int mask=0;
    if (method&0x100) mask=0x8042;
    if (method&4) mask|=4;
    if (method&0x1ba60) mask|=0x40;
    if (method&8) mask|=0x8000;
    if (method&0x50) mask|=2;
    if (Techno_Type_Class()->DistributedFire) {
        unknown_3a8.Clear();
        unknown_390.Clear();
    }
    if (method&3) {
        int range=0;
        if (method&1) range=unknown_slot188(0);
        else if (method&2) {
            if (Mission==25) range=unknown_slot188(2);
            else range=unknown_slot188(1);
        }
        if (unknown_6c16f0(-1)<0 && Mission==5) range=512;
        int cell_range=range/256;
        if (!range) {
            if (Techno_Type_Class()->Has_Turret_Weapons()) cell_range=Weapon_Range(unknown_10c)/256;
            else if (Techno_Type_Class()->Underwater && Techno_Type_Class()->Organic && Techno_Type_Class()->SelfHealing) cell_range=Techno_Type_Class()->GuardRange.Value/256;
            else cell_range=((Weapon_Range(0)>Weapon_Range(1)) ? Weapon_Range(0):Weapon_Range(1))/256;
            ++cell_range;
            cell_range+=Techno_Type_Class()->AirRangeBonus/256;
        }
        Cell center(*coord);
        if (method&4) {
            int air_mask=mask|0x8002;
            unknown_tracker_839ea0.Unknown_004128A0(&Map[center],cell_range);
            TechnoClass *object=unknown_tracker_839ea0.Fetch_Next_004134e0();
            while (object) {
                int value=0;
                if (!House->Is_Ally(object) && (!(method&0x4000) || House->Is_Ally(object->House)) && object->unknown_74 && object->In_Which_Layer()!=2 && (!only_enemy || object->House->ArrayIndex==House->unknown_54f0) && unknown_6c5570(method,air_mask,range,object,&value,-1,&unknown_geometry_l)) {
                    if (Techno_Type_Class()->DistributedFire) {
                        unknown_3a8.Add(object);
                        unknown_390.Add(value);
                    }
                    if (value>best_value) { best_object=object;best_value=value; }
                }
                object=unknown_tracker_839ea0.Fetch_Next_004134e0();
            }
        }
        if (method&0x10) mask|=4;
        if (!(method^5)) return best_object;
        {
            Cell best_cell(0,0);
            int best_cell_value=0;
            ObjectClass *object;
            int value;
            for (int radius=0;radius<cell_range;++radius) {
                for (int x=-radius;x<=radius;++x) {
                    Cell scan=center+Cell(x,-radius);
                    if (Map.In_Radar(scan)) {
                        if (unknown_6c6130(method,mask,&scan,range,&object,&value,zone) && object && (!only_enemy || static_cast<TechnoClass *>(object)->House->ArrayIndex==House->unknown_54f0) && (!(method&0x4000) || House->Is_Ally(static_cast<TechnoClass *>(object)->House))) {
                            if (Techno_Type_Class()->DistributedFire) { unknown_3a8.Add(object);unknown_390.Add(value); }
                            if (best_value<value) { best_object=object;best_value=value; }
                        }
                        if (!best_object) {
                            value=unknown_6c6350(&scan);
                            if (best_cell_value<value) {best_cell_value=value;best_cell=scan;}
                        }
                    }
                    scan=center+Cell(x,radius);
                    if (Map.In_Radar(scan)) {
                        if (unknown_6c6130(method,mask,&scan,range,&object,&value,zone) && object && (!only_enemy || static_cast<TechnoClass *>(object)->House->ArrayIndex==House->unknown_54f0) && (!(method&0x4000) || House->Is_Ally(static_cast<TechnoClass *>(object)->House))) {
                            if (Techno_Type_Class()->DistributedFire) { unknown_3a8.Add(object);unknown_390.Add(value); }
                            if (best_value<value) { best_object=object;best_value=value; }
                        }
                        if (!best_object) {
                            value=unknown_6c6350(&scan);
                            if (best_cell_value<value) {best_cell_value=value;best_cell=scan;}
                        }
                    }
                }
                for (int y=1-radius;y<radius;++y) {
                    Cell scan=center+Cell(-radius,y);
                    if (Map.In_Radar(scan)) {
                        if (unknown_6c6130(method,mask,&scan,range,&object,&value,zone) && object && (!only_enemy || static_cast<TechnoClass *>(object)->House->ArrayIndex==House->unknown_54f0) && (!(method&0x4000) || House->Is_Ally(static_cast<TechnoClass *>(object)->House))) {
                            if (Techno_Type_Class()->DistributedFire) { unknown_3a8.Add(object);unknown_390.Add(value); }
                            if (best_value<value) { best_object=object;best_value=value; }
                        }
                        if (!best_object) {
                            value=unknown_6c6350(&scan);
                            if (best_cell_value<value) {best_cell_value=value;best_cell=scan;}
                        }
                    }
                    scan=center+Cell(radius,y);
                    if (Map.In_Radar(scan)) {
                        if (unknown_6c6130(method,mask,&scan,range,&object,&value,zone) && object && (!only_enemy || static_cast<TechnoClass *>(object)->House->ArrayIndex==House->unknown_54f0) && (!(method&0x4000) || House->Is_Ally(static_cast<TechnoClass *>(object)->House))) {
                            if (Techno_Type_Class()->DistributedFire) { unknown_3a8.Add(object);unknown_390.Add(value); }
                            if (best_value<value) { best_object=object;best_value=value; }
                        }
                        if (!best_object) {
                            value=unknown_6c6350(&scan);
                            if (best_cell_value<value) {best_cell_value=value;best_cell=scan;}
                        }
                    }
                }
                if (best_object) {
                    if (radius==cell_range/4 || radius==cell_range/2) return best_object;
                }
                if (best_cell!=unknown_cell_ac0148) return &Map[best_cell];
            }
        }
    } else {
        int index;
        if (mask&4) {
            for (index=0;index<Aircraft.Count();++index) {
                TechnoClass *object=Aircraft[index];
                int value=0;
                if (!House->Is_Ally(object) && (!only_enemy || object->House->ArrayIndex==House->unknown_54f0) && (!(method&0x4000) || House->Is_Ally(object->House)) && unknown_6c5570(method,mask,-1,object,&value,-1,&unknown_geometry_l) && value>best_value) { best_object=object;best_value=value; }
            }
        }
        if (method&0x10) mask|=4;
        for (index=0;index<Technos.Count();++index) {
            TechnoClass *object=Technos[index];
            int value=0;
            if ((!House->Is_Ally(object) || unknown_6c16f0(-1)<0 || (!House->Is_Human_Player() && this && Kind_Of()==15 && (this && Kind_Of()==15 ? static_cast<InfantryClass *>(this):0)->Type->Engineer)) && object->unknown_80==2 && (!only_enemy || object->House->ArrayIndex==House->unknown_54f0) && (!(method&0x4000) || House->Is_Ally(object->House)) && unknown_6c5570(method,mask,-1,object,&value,zone,coord) && value>best_value) {best_object=object;best_value=value;}
        }
    }
    return best_object;
}


// int TechnoClass::Owner() const (0x006C7420, slot 14)
// The owner number (+0x30 of the house).

int TechnoClass::Owner() const
{
    return House->ArrayIndex;
}


// HouseClass *TechnoClass::Owning_House() const (0x006C7430, slot 15)
// The owner.

HouseClass *TechnoClass::Owning_House() const
{
    return House;
}


// void TechnoClass::Clicked_As_Target(int arg1) (0x006C7440, slot 79)
// Stores FlashCount unless slot 49 holds and slot 51 is 0.

void TechnoClass::Clicked_As_Target(int arg1)
{
    if (!unknown_slot49(unknown_player_a35db4) || unknown_slot51(1)) {
        FlashCount = arg1;
    }
}


// bool TechnoClass::unknown_slot158() (0x006C7480, slot 158)
// Whether the type's +0xB0 entry is set and starts with a zero byte.

bool TechnoClass::unknown_slot158()
{
    if (Techno_Type_Class()->unknown_b0.unknown_00 == 0) {
        return false;
    }
    return *(char const *)Techno_Type_Class()->unknown_b0.unknown_00 == 0 ? true : false;
}


// TechnoClass::AI (slot 23, 0x006C74C0; TibSun techno.cpp TechnoClass::AI): per-tick techno logic:
// veterancy promotion, the draw-behind cell test, friendly-target and range checks, the turret
// charge stage, recoil, self-heal, cloaking, flashing, damage sparks and the EMP recovery.
// Unknown names keep their offsets. Is_Foot is a file-local helper (hypothesis): its stored
// result reproduces the original's spilled flag at the first two foot tests; the EMP test keeps
// the same form written in place.
// The local sparks vector holds const ParticleSystemTypeClass pointers (original RTTI);
// const_cast bridges the existing mutable ParticleSystemClass constructor contract only.
namespace seed_00496ce0 { void __fastcall End_Count_00496ce0(int timer); }
extern double const unknown_random_scale_79c280;
static inline bool Is_Foot(AbstractClass const *object)
{
    bool foot = false;
    if (object) foot = object->AbstractFlags.IsFoot;
    return foot;
}

void TechnoClass::AI()
{
    Point2D point;
    Rect window;
    if (unknown_380) unknown_380=false;
    unknown_6d8820();
    if (unknown_70>Strength) unknown_70=Strength;
    if ((Frame&4) && unknown_70<Strength) {
        if (unknown_70+30<0) unknown_70=-30;
        ++unknown_70;
    }
    if (Techno_Type_Class()->Turret) {
        if (unknown_3f0) {
        if (unknown_3ec) {
            unknown_3d8.Unknown_00405D50();
            Voc_Play_At(Techno_Type_Class()->TurretRotateSound,Position_Coord_004f3f40(),&unknown_3d8);
            unknown_3ec=0;
        }
        } else { unknown_3d8.Unknown_00406070(); unknown_3ec=1; }
        Voc_Update_At(Position_Coord_004f3f40(),&unknown_3d8);
    }
    int old_rank=unknown_110;
    if (old_rank!=unknown_118.Remaining_Levels()) {
        if (old_rank!=-1) {
            if (unknown_118.Remaining_Levels()==0) {
                if (House->Is_Player_Control()) { Voc_Play_At(Rules->UpgradeEliteSound,Position_Coord_004f3f40(),0); Speech_Play_Name("EVA_UnitPromoted",-1,-1); }
                FlashCount=Rules->EliteFlashTimer;
            } else if (unknown_118.Remaining_Levels()==1 && House->Is_Player_Control()) {
                Voc_Play_At(Rules->UpgradeVeteranSound,Position_Coord_004f3f40(),0);
                Speech_Play_Name("EVA_UnitPromoted",-1,-1);
            }
        }
        unknown_110=unknown_118.Remaining_Levels();
    }
    if (unknown_slot158()) { unknown_slot237(); if (!unknown_7d) return; }
    Cell where(Center_Coord());
    CellClass &cell=Map[where];
    bool draw_behind;
    if (Kind_Of()==6) {
        if (static_cast<BuildingClass *>(this)->Type->CanBeHidden) draw_behind=cell.unknown_100>1;
        else draw_behind=false;
    } else draw_behind=cell.unknown_100!=0;
    if (draw_behind && !(this && Kind_Of()==2)) {
        unknown_6d9080(point,&window);
    } else if (unknown_108) {
        if (Kind_Of()!=6) unknown_abstracts_ac0bf8.Delete(this);
        unknown_108->Delete_Me();
        unknown_108=0;
    }
    if (!House->Is_Human_Player() && unknown_228 && House->Is_Ally(static_cast<ObjectClass *>(unknown_228))) {
        bool infantry=Kind_Of()==15;
        bool building=unknown_228->Kind_Of()==6;
        bool can_enter=infantry && building && static_cast<BuildingClass *>(unknown_228)->Unknown_00452EB0(this);
        bool engineer=infantry && static_cast<InfantryClass *>(this)->Type->Engineer;
        bool assault=unknown_slot231(1)->WeaponType && unknown_slot231(1)->WeaponType->Warhead->ElectricAssault && building && static_cast<BuildingClass *>(unknown_228)->Type->Overpowerable;
        if (!engineer && !assault && !can_enter) unknown_slot220(0);
    }
    if ((int)Frame%16==0 && Mission!=8 && Mission!=17) {
        int fire_error=unknown_slot217(unknown_228,unknown_slot174(unknown_228));
        if (unknown_228 && (fire_error==5 || fire_error==6)) unknown_slot220(0);
    }
    if (Techno_Type_Class()->TurretRecoil) { unknown_330.unknown_6d8bc0(); unknown_350.unknown_6d8bc0(); }
    if (Techno_Type_Class()->IsChargeTurret && Techno_Type_Class()->Has_Turret_Weapons()) {
        int turret=0;
        if (unknown_250>0) {
            turret=Techno_Type_Class()->TurretCount*unknown_244.Value()/unknown_250;
            if (turret>=Techno_Type_Class()->TurretCount) turret=Techno_Type_Class()->TurretCount-1;
            else if (turret<0) turret=0;
        }
        unknown_100=turret;
    }
    if (unknown_2a8.Has_Ended()) seed_00496ce0::End_Count_00496ce0(reinterpret_cast<int>(&unknown_2a8));
    if (unknown_376) --unknown_376;
    if (unknown_228 && unknown_404) {
        if (Mission==0 || Mission==7 || Mission==13 || Mission==14 || Mission==16 || Mission==18 || Mission==19 || Mission==20 || Mission==22 || Mission==23 || Mission==28 || Mission==24) unknown_slot220(0);
    }
    MissionClass::AI();
    if (unknown_148.Value()==0) {
        if (unknown_slot276()) {
            unknown_slot278();
        } else if ((Mission==2 || Mission==10 || Mission==5) && unknown_6d4180()) {
            unknown_3f4=Frame;
            AbstractClass *target=unknown_228;
            CoordStruct coord=Coord;
            if (unknown_slot209(&coord,1) && unknown_228!=target) unknown_404=true;
        }
    }
    if (unknown_38 && unknown_38->Unknown_00436070()) unknown_38->Unknown_00435D30();
    if (!unknown_7d) return;
    if (unknown_slot157()) {
        ++Strength;
        if ((Health_Ratio()>Rules->unknown_1368 || Get_Height_AGL()<-10) && unknown_25c[3]) unknown_25c[3]->Delete_Me();
    }
    Cloaking_AI(0);
    if (SpawnManager) SpawnManager->AI();
    if (unknown_1b8==0 && Map[Center_Coord()].Is_House_Bit(House->ArrayIndex)) unknown_slot238();
    if (unknown_1b8==2 && !Map[Center_Coord()].Is_House_Bit(House->ArrayIndex)) unknown_slot238();
    if (unknown_game_mode_a3d298==0 && !House->Is_Player_Control() && unknown_228 && !House->Is_Ally(static_cast<ObjectClass *>(unknown_228)) && unknown_6c16f0(-1)<0) unknown_slot220(0);
    if (unknown_228 && unknown_game_mode_a3d298!=0 && House->unknown_134 && unknown_6c16f0(-1)<0 && !House->Is_Ally(static_cast<ObjectClass *>(unknown_228))) unknown_slot220(0);
    if (unknown_228 && unknown_228->Kind_Of()==2) {
        ObjectClass *aircraft=static_cast<ObjectClass *>(unknown_228);
        if (Kind_Of()==1 && unknown_6c16f0(-1)<0 && (aircraft->Get_Height_AGL()>0 || Map[aircraft->Position_Coord_004f3f40()].Cell_Building())) unknown_slot220(0);
    }
    if (Kind_Of()!=2 && unknown_228 && !unknown_slot156()) {
        FootClass *foot=0;
        if (Is_Foot(this)) foot=static_cast<FootClass *>(this);
        if (!foot || !foot->unknown_494) {
            if (!unknown_slot169(unknown_228)) {
                if (Is_Foot(this)) foot->unknown_slot304();
                if (!foot || !foot->unknown_494) {
                    int primary=unknown_slot174(unknown_228);
                    if (!unknown_slot212(unknown_228,primary)) unknown_slot220(0);
                }
            }
        }
    }
    if (Kind_Of()!=6) StageClass::Graphic_Logic();
    unsigned old_flash=FlashCount;
    bool was_flash_on=(old_flash&2)==2;
    if (FlasherClass::Process()) Mark(2);
    if (old_flash && old_flash!=FlashCount && Kind_Of()==6 && was_flash_on!=((FlashCount&2)==2)) {
        unknown_tactical_83996c->Register_Dirty_Area(Get_Render_Rect(),false);
        static_cast<BuildingClass *>(this)->Update_Anim_Appearance_0044d190();
    }
    TechnoTypeClass const *type=Techno_Type_Class();
    if (type->unknown_a9b && Health_Ratio()<Rules->unknown_1368 && Get_Height_AGL()>-10) {
        DynamicVectorClass<ParticleSystemTypeClass const *> sparks;
        for (int i=0;i<type->DamageParticleSystems.Count();++i) {
            if (type->DamageParticleSystems[i]->BehavesLike==3) sparks.Add(type->DamageParticleSystems[i]);
        }
        if (!unknown_25c[1] && sparks.Count()>0) {
            double probability=Health_Ratio()<Rules->unknown_1370 ? Rules->ConditionRedSparkingProbability:Rules->ConditionYellowSparkingProbability;
            if (double(unknown_id_source_a3d290->Random()(0,0x7ffffffe))*unknown_random_scale_79c280<probability) {
                unknown_25c[1]=new ParticleSystemClass(const_cast<ParticleSystemTypeClass *>(sparks[unknown_id_source_a3d290->Random()(0,sparks.Count()-1)]),Techno_Type_Class()->Damage_Smoke_Offset()+Center_Coord(),0,this,unknown_geometry_l);
            }
        }
    }
    unknown_slot267(false);
    if (unknown_3fc>0 && --unknown_3fc==0) {
        if (Kind_Of()==6) {
            BuildingClass *building=static_cast<BuildingClass *>(this);
            if (building && !building->Type->InvisibleInGame) { building->unknown_44d600(); if (building->Type->Radar) House->unknown_5669=true; }
        } else {
            bool is_foot=false;
            if (this) is_foot=AbstractFlags.IsFoot;
            if (is_foot) {
                FootClass *foot=static_cast<FootClass *>(this);
                if (foot->Locomotor) foot->Locomotor.Checked()->Power_On();
                for (int i=0;i<Anims.Count();++i) {
                    AnimClass *anim=Anims[i];
                    if (anim && anim->unknown_b4==this && anim->Class==Rules->unknown_1438) anim->Loops=0;
                }
            }
        }
    }
}


// void TechnoClass::unknown_slot239() (0x006C8290, slot 239)
// Reloads one round (+0x254, up to the type's Ammo) when the +0x194 timer
// has run out, then slot 71 (2) and 0x006C8300.

void TechnoClass::unknown_slot239()
{
    int ammo = Techno_Type_Class()->Ammo;
    if (ammo != -1 && unknown_254 < ammo && unknown_194.Value() == 0) {
        unknown_254++;
        Mark(2);
        unknown_6c8300();
    }
}


// void TechnoClass::unknown_6c8300() (0x006C8300)
// Restarts the reload timer (+0x194) while below the type's Ammo:
// EmptyReload when empty (if set), else Reload plus ReloadIncrement times
// the square of the full PipWrap groups loaded.

void TechnoClass::unknown_6c8300()
{
    int ammo = Techno_Type_Class()->Ammo;
    if (unknown_254 < ammo) {
        if (unknown_254 == 0 && Techno_Type_Class()->EmptyReload != -1) {
            unknown_194 = CDTimerClass<FrameTimerClass>(Techno_Type_Class()->EmptyReload);
        } else {
            int groups = Techno_Type_Class()->PipWrap == 0 ? 1 : unknown_254 / Techno_Type_Class()->PipWrap;
            int delay = Techno_Type_Class()->ReloadIncrement * groups * groups;
            unknown_194 = CDTimerClass<FrameTimerClass>(Techno_Type_Class()->Reload + delay);
        }
    }
}


// Gap-generator registration/removal, with the original void return behavior.
void TechnoClass::unknown_slot235()
{
    if (unknown_player_a35db4 && !unknown_1fd && unknown_slot195()) {
        if (!unknown_200) unknown_200=Techno_Type_Class()->GapRadiusInCells;
        unknown_1fd=true;
        CoordStruct projected=Coord;
        projected.X+=(Unknown_006A2970(projected.Z)/-30)*256;
        projected.Y+=(Unknown_006A2970(projected.Z)/-30)*256;
        Cell center(projected);
        int radius=unknown_200;
        for (int x=-radius-2; x<radius+2; ++x) {
            for (int y=-radius-2; y<radius+2; ++y) {
                if (x*x+y*y<(radius+1)*(radius+1)) {
                    Cell cell=Cell(center.X+x,center.Y+y);
                    if (!Owning_House()->Is_Player_Control() && !House->Is_Ally(unknown_player_a35db4) && !unknown_player_a35db4->unknown_13d) {
                        if (Map[cell].unknown_130!=1 && Map[cell].unknown_130>=0) {
                            ++Map[cell].unknown_130;
                        }
                        ++Map[cell].unknown_134;
                        if (Map[cell].unknown_130>=1) {
                            Map[cell].unknown_12c&=~0x10u;
                            Map[cell].unknown_12c&=~0x08u;
                        }
                    }
                    if (Owning_House()->Is_Player_Control() || House->Is_Ally(unknown_player_a35db4) || unknown_player_a35db4->unknown_13d) {
                        ++Map[cell].unknown_13c;
                    }
                }
            }
        }
        if (unknown_player_a35db4) unknown_player_a35db4->unknown_188=false;
        Map.Complete_Radar_Refresh();
        Map.Flag_To_Redraw(2);
    }
}


// Gap-generator registration/removal, with the original void return behavior.
void TechnoClass::unknown_slot236()
{
    if (unknown_player_a35db4 && unknown_1fd) {
        if (!unknown_200) unknown_200=Techno_Type_Class()->GapRadiusInCells;
        unknown_1fd=false;
        CoordStruct projected=Coord;
        projected.X+=(Unknown_006A2970(projected.Z)/-30)*256;
        projected.Y+=(Unknown_006A2970(projected.Z)/-30)*256;
        Cell center(projected);
        int radius=unknown_200;
        for (int x=-radius-2; x<radius+2; ++x) {
            for (int y=-radius-2; y<radius+2; ++y) {
                if (x*x+y*y<(radius+1)*(radius+1)) {
                    Cell cell=Cell(center.X+x,center.Y+y);
                    if (!Owning_House()->Is_Player_Control() && !House->Is_Ally(unknown_player_a35db4) && !unknown_player_a35db4->unknown_13d) {
                        --Map[cell].unknown_134;
                        if (unknown_player_a35db4->unknown_566a && Map[cell].unknown_134<=0) {
                            --Map[cell].unknown_130;
                            if (Map[cell].unknown_130<=0) {
                                Map[cell].unknown_12c|=0x08u;
                                Map[cell].unknown_12c|=0x10u;
                            }
                        }
                    }
                    if (Owning_House()->Is_Player_Control() || House->Is_Ally(unknown_player_a35db4) || unknown_player_a35db4->unknown_13d) {
                        --Map[cell].unknown_13c;
                    }
                }
            }
        }
        if (unknown_player_a35db4) unknown_player_a35db4->unknown_188=false;
        Map.Complete_Radar_Refresh();
        Map.Flag_To_Redraw(2);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void TechnoClass::Cloaking_AI(int arg1) 0x006C89C0 (slot 234, Cloaking_AI), from ra2-decomp
// TechnoClass::Cloaking_AI (YR 0x006FB740, src/techno.cpp), cases in the twin's order (uncloaking,
// cloaking, cloaked): uncloaked, an object that may cloak (slot 154 and none of slots 201/202/111/112,
// or the rank's ability 6) and is not docked at a weapons factory steps the cloak stage and starts
// cloaking (slot 253) once slot 160 allows, at once above ConditionRed health, else with a 4% chance.
// Uncloaking (3) ends visible: the stage resets and the cloak delay timer (+0x1D4) restarts. Cloaking (1)
// ends hidden: state 2, every techno targeting it that can no longer see it drops the target (slot 220),
// and a computer unit scatters; while still dark a hurt object may give up (slot 252, 10%). Cloaked (2)
// uncloaks when slot 161 says so. arg1 is not read.
// Name map: Cloak = unknown_1b8, CloakingDevice = unknown_1bc, CloakDelay = unknown_1d4,
// Visual_Character = unknown_slot25, Has_Ability = unknown_6d7910,
// Is_Ready_To_Cloak/Should_Uncloak = unknown_slot160/161, Do_Uncloak/Do_Cloak = unknown_slot252/253,
// Detach_All = unknown_slot54, Assign_Target = unknown_slot220, Do_Shimmer = unknown_slot61,
// Scatter = unknown_slot87, ConditionRed = unknown_1370, Array = Technos.
// The stage's timer test calls TibSun's operator long (RA2_CDTIMER_CONVERSION_VALUE_BODY, 0x00453EB0).

void TechnoClass::Cloaking_AI(int arg1)
{
    if (unknown_1b8 == 0) {
        if ((unknown_slot154() && !unknown_slot201() && !unknown_slot202() && !unknown_slot111() && !unknown_slot112())
            || unknown_6d7910(6)) {
            TechnoClass *contact = Contact(0);
            if (contact == 0 || !(contact->Kind_Of() == 6 && ((BuildingClass *)contact)->Type->WeaponsFactory)) {
                unknown_1bc.Graphic_Logic();
                if (unknown_slot160()) {
                    if (Health_Ratio() > Rules->unknown_1370) {
                        unknown_slot253(false);
                    } else {
                        if (unknown_id_source_a3d290->Random()(0, 99) < 4) {
                            unknown_slot253(false);
                        }
                    }
                }
            }
        }
    } else {
        unknown_1bc.Graphic_Logic();
        if (unknown_1bc.Fetch_Stage() < 0) {
            unknown_1bc.Set_Stage(0);
        }
        switch (unknown_1b8) {
        case 3:
            Mark(2);
            switch (unknown_slot25(true, 0)) {
            case 0:
                unknown_1bc.Set_Rate(0);
                unknown_1bc.Set_Stage(0);
                unknown_1b8 = 0;
                unknown_1d4 = CDTimerClass<FrameTimerClass>((int)(Rules->CloakDelay * 900.0));
                Mark(2);
                break;
            case 1:
                if (unknown_slot160()) {
                    unknown_slot253(true);
                }
                break;
            }
            break;
        case 1:
            Mark(2);
            if (!unknown_1bc.Fetch_Rate()) {
                unknown_1bc.Set_Rate(1);
            }
            switch (unknown_slot25(true, 0)) {
            case 2:
                if (Health_Ratio() <= Rules->unknown_1370 && unknown_id_source_a3d290->Random()(0, 99) < 10) {
                    unknown_slot252(true);
                }
                break;
            case 3:
            case 5:
                unknown_1b8 = 2;
                unknown_1bc.Set_Rate(0);
                unknown_1bc.Set_Stage(0);
                Mark(2);
                if (Kind_Of() == 1 && ((UnitClass *)this)->unknown_5b4 != -1) {
                    Do_Shimmer();
                } else {
                    DynamicVectorClass<TechnoClass *> attackers;
                    for (int index = Technos.Count() - 1; index >= 0; index--) {
                        TechnoClass *techno = Technos[index];
                        if (techno->unknown_228 == this) {
                            if (((UnknownCellClass *)&Map[Center_Coord()])->Is_Sensed(techno->House->ArrayIndex)
                                || techno->House == House) {
                                attackers.Add(techno);
                            }
                        }
                    }
                    unknown_slot54(false);
                    for (int i = attackers.Count() - 1; i >= 0; i--) {
                        attackers[i]->unknown_slot220(this);
                    }
                }
                if (Kind_Of() == 1 && !House->Unknown_004F52F0()) {
                    Scatter(unknown_geometry_l, true, false);
                }
                break;
            }
            break;
        case 2:
            if (unknown_slot161()) {
                unknown_slot252(false);
            }
            break;
        }
    }
}


// bool TechnoClass::unknown_slot161() (0x006C8EF0)
// Whether the techno may cloak now (hypothesis: ability 6 is CLOAK): not
// while slot 154 or +0x32A holds unless slot 201, 202, 111 or 112 does, not
// for a veteran or elite whose rank abilities include it, and not in a cell
// flagged for the owner (0x0047C2C0).

bool TechnoClass::unknown_slot161()
{
    if (!(unknown_slot154() || unknown_32a) || unknown_slot201() || unknown_slot202() || unknown_slot111()
        || unknown_slot112()) {
        bool allowed = true;
        if (unknown_118.Is_Veteran() || unknown_118.Is_Elite()) {
            TechnoTypeClass *type = Techno_Type_Class();
            if (unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[6]) {
                allowed = false;
            } else if (unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[6] || type->EliteAbilities.Ability[6])) {
                allowed = false;
            }
        }
        if (allowed && !((UnknownCellClass *)&Map[Coord_Cell(Center_Coord())])->unknown_47c2c0(House->ArrayIndex)) {
            return true;
        }
    }
    return false;
}


// bool TechnoClass::unknown_slot160() (0x006C9020)
// Whether the techno may uncloak now (hypothesis, the counterpart of slot
// 161): not while it should stay cloaked (slot 154 off, no rank ability 6,
// the owner's cell flag 0x0047C2C0 clear and +0x32A off), not in cloak
// state 2, while the +0x244 timer runs, while it may fire at its target
// (slot 213), while a non-building's stage +0x1BC is set, or while the
// +0x1D4 timer runs.

bool TechnoClass::unknown_slot160()
{
    if (!unknown_slot154()) {
        bool allowed = true;
        if (unknown_118.Is_Veteran() || unknown_118.Is_Elite()) {
            TechnoTypeClass *type = Techno_Type_Class();
            if (unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[6]) {
                allowed = false;
            } else if (unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[6] || type->EliteAbilities.Ability[6])) {
                allowed = false;
            }
        }
        if (allowed && !((UnknownCellClass *)&Map[Coord_Cell(Center_Coord())])->unknown_47c2c0(House->ArrayIndex)
            && !unknown_32a) {
            return false;
        }
    }
    if (unknown_1b8 == 2) {
        return false;
    }
    if (unknown_244.Value() != 0) {
        return false;
    }
    if (unknown_228 != 0 && unknown_slot213(unknown_228)) {
        return false;
    }
    if (Kind_Of() != 6 && unknown_1bc.Value != 0) {
        return false;
    }
    if (unknown_1d4.Value() != 0) {
        return false;
    }
    return true;
}


// bool TechnoClass::unknown_slot80() (0x006C91C0, slot 80)
// The ObjectClass version when allowed; then the tag event 0x21, slot 196 and 0x006145E0.

bool TechnoClass::unknown_slot80()
{
    if ((unknown_373 || House->Is_Player_Control() || !unknown_b263d8) && ObjectClass::unknown_slot80()) {
        if (unknown_34) {
            unknown_34->Spring(0x21, this, unknown_geometry_k, 0, 0);
        }
        if (House->Is_Player_Control() && unknown_sound_enabled_7d88d6) {
            unknown_slot196();
        }
        unknown_6145e0();
        return true;
    }
    return false;
}


// int TechnoClass::unknown_slot217(AbstractClass *arg1, int arg2) (0x006C9240, slot 217)
// Slot 218 with a zero third argument.

int TechnoClass::unknown_slot217(AbstractClass *arg1, int arg2)
{
    return unknown_slot218(arg1, arg2, 0);
}


// int TechnoClass::unknown_slot218(AbstractClass *target, int which, bool check_range) (0x006C9260, slot 218)
// TibSun Can_Fire: 0 when the object may fire, else the FireErrorType. The land
// targeting test sits in each arm (the arms cross-jump), and everything after the
// weapon test is nested in it so the CANT block stays last.
extern "C" int __cdecl abs(int);
int TechnoClass::unknown_slot218(AbstractClass *target,int which,bool check_range)
{
    TechnoClass *techno;
    CellClass *cellptr;
    WeaponTypeClass *weapon;
    WeaponTypeClass *other;
    bool water;
    bool check_rearm;
    FootClass *foot;
    bool parasite;
    if(target==0) goto ILLEGAL;
    if(unknown_slot112()) return 3;
    if(unknown_slot111()) goto ILLEGAL;
    if(unknown_325) goto ILLEGAL;
    if(unknown_slot113() && unknown_208 && (int)target==unknown_208->unknown_28) return 3;
    techno=target->AbstractFlags.IsTechno?static_cast<TechnoClass *>(target):0;
    cellptr=reinterpret_cast<CellClass *>(&Map[target->Center_Coord()]);
    if(techno) {
        if(techno->unknown_76) goto ILLEGAL;
        if(techno->unknown_210) goto ILLEGAL;
        if(techno->unknown_slot25(true,(int)House)==5 && !cellptr->Is_Sensed(House->ArrayIndex)) {
            if(unknown_6c16f0(-1)>0) goto CANT;
            if(!techno->House->Is_Ally(House)) goto CANT;
        }
    }
    if(unknown_7a) goto ILLEGAL;
    if(unknown_slot201()) {
        if(Kind_Of()!=1) goto CANT;
        UnitClass *unit=static_cast<UnitClass *>(this);
        if(!unit->Type->LargeVisceroid && !unit->Type->SmallVisceroid) goto CANT;
    }
    weapon=unknown_slot231(which)->WeaponType;
    if(weapon!=0) {
        if(weapon->unknown_125 && House->Unknown_0051FC80()) goto CANT;
        if(techno && weapon->Warhead && techno->unknown_slot111() && !weapon->Warhead->Temporal) goto ILLEGAL;
        if(weapon->unknown_10d) {
            if(unknown_6cf670()) goto CANT;
            if(unknown_slot202()) goto CANT;
            if(SpawnManager->Alive_Count()==0) return 3;
        }
        other=unknown_slot231(which==0)->WeaponType;
        if(other && ((other->unknown_105 && unknown_25c[0]) || (other->unknown_109 && unknown_25c[4]) || (other->unknown_106 && unknown_25c[1]) || (other->unknown_10c && unknown_27c))) goto CANT;
        if(target->In_Air() && !weapon->Projectile->AA) goto ILLEGAL;
        if(techno) {
            water=(reinterpret_cast<CellClass *>(techno->Get_Cell_Ptr())->unknown_ec==2)
                || (reinterpret_cast<CellClass *>(techno->Get_Cell_Ptr())->unknown_ec==6);
            if(techno->In_Air()) water=false;
            if(techno->unknown_79) water=false;
            else if(water && unknown_slot175(static_cast<ObjectClass *>(target))==-1) goto ILLEGAL;
            if(target->On_Ground() && !water && Techno_Type_Class()->LandTargeting==1) return 5;
        } else {
            if(!target->In_Air() && !weapon->Projectile->AG) return 5;
            if(target->Kind_Of()==0xb && As_Cell(target)->unknown_ec!=2 && As_Cell(target)->unknown_ec!=6 && Techno_Type_Class()->LandTargeting==1) goto ILLEGAL;
        }
        check_rearm=true;
        if(which==0 && this && Kind_Of()==1) {
            UnitClass *unit=static_cast<UnitClass *>(this);
            int burst=unknown_310%weapon->Burst;
            if(burst<2 && unit->Type->FiringSyncFrame[burst]!=-1 && unit->unknown_5a8!=-1) {
                if(unit->unknown_5a8!=unit->Type->FiringSyncFrame[burst]) return 3;
                check_rearm=false;
            }
        }
        if(check_rearm && unknown_244.Value()!=0) return 3;
        if(weapon->unknown_105 && unknown_25c[0]) return 3;
        if(weapon->unknown_109 && unknown_25c[4]) return 3;
        if(weapon->unknown_106 && unknown_25c[1]) return 3;
        if(weapon->unknown_10c && unknown_27c) return 3;
        if(unknown_254==0) return 1;
        if(weapon->unknown_10f && unknown_1b8!=0 && (Kind_Of()!=2 || unknown_1b8==2)) return 9;
        if(Techno_Type_Class()->HunterSeeker) return 8;
        if(weapon->Warhead->Parasite && this && AbstractFlags.IsFoot) {
            foot=target->AbstractFlags.IsFoot?static_cast<FootClass *>(target):0;
            if(!static_cast<FootClass *>(this)->unknown_588->Can_Infect_006078F0(foot)) return 5;
        }
        parasite=weapon->Warhead->Parasite;
        if(parasite && target->AbstractFlags.IsFoot && Frame<static_cast<FootClass *>(target)->unknown_584) return 5;
        if(techno) {
            if(parasite && techno->unknown_slot203()) return 5;
            if(weapon->Warhead->MindControl && !reinterpret_cast<CaptureManagerClass *>(unknown_230)->Can_Capture(techno)) return 5;
            WarheadTypeClass *warhead=weapon->Warhead;
            if(warhead->Verses[techno->Techno_Type_Class()->unknown_9c]==0.0) return 5;
            if(warhead->BombDisarm && techno->unknown_38==0) return 5;
            if(warhead->IvanBomb && techno->unknown_38!=0) return 5;
            if(techno->unknown_325) return 5;
            if(unknown_79!=techno->unknown_79) {
                if(Get_Cell_Ptr() && (reinterpret_cast<CellClass *>(Get_Cell_Ptr())->unknown_140&0x100) && !In_Air()
                    && techno->Get_Cell_Ptr() && (reinterpret_cast<CellClass *>(techno->Get_Cell_Ptr())->unknown_140&0x100)) return 5;
                int mine=Position_Coord_004f3f40().Z;
                int theirs=techno->Position_Coord_004f3f40().Z;
                if(weapon->Warhead->Parasite && abs(mine-theirs)>unknown_geometry_g*2) return 5;
            }
        }
        if(Techno_Type_Class()->Organic && unknown_slot202()) return 5;
        if(check_range && !unknown_slot212(target,which)) return 8;
        return 0;
    }
CANT:
    return 6;
ILLEGAL:
    return 5;
}


// void TechnoClass::unknown_slot210() (0x006C9A90, slot 210)
// Resets several states and the +0x238 object.

void TechnoClass::unknown_slot210()
{
    unknown_slot220(0);
    Assign_Destination(0, true);
    SendToEachLink(3);
    if (SpawnManager) {
        SpawnManager->Kill_Nodes();
        SpawnManager->Abandon_Target();
    }
    unknown_slot54(1);
    Unselect();
}


// void TechnoClass::unknown_slot220(AbstractClass *target) (0x006C9AF0, slot 220)
// Sets the target (+0x228), clearing +0x404 first: this object means the
// cell under it; an object (flag bit 1) that is not +0x7D or has no
// strength is dropped, and one with flag bit 2 too when it is +0x325 or
// busy infantry. A dropped target tells +0x238, clears +0x310 and drops
// +0x25C when slot 213 refuses it.

// Red Alert's Target_Legal: any target but none.
inline bool Target_Legal(AbstractClass const *target)
{
    return target != 0;
}

void TechnoClass::unknown_slot220(AbstractClass *target)
{
    unknown_404 = false;
    if (target == unknown_228) {
        return;
    }
    if (!Target_Legal(target)) {
        target = 0;
    } else {
        if (target == this) {
            target = (AbstractClass *)Map_Cell(CoordStruct(Coord));
        } else if (target->AbstractFlags.IsObject) {
            if (!((ObjectClass *)target)->unknown_7d || ((ObjectClass *)target)->Strength == 0) {
                target = 0;
            } else {
                bool active = target->AbstractFlags.IsFoot;
                TechnoClass *techno;
                if (active && (techno = (TechnoClass *)target)->unknown_325) {
                    target = 0;
                }
                if (active && techno->Kind_Of() == 15 && ((InfantryClass *)techno)->Is_Busy_00509760()) {
                    target = 0;
                }
            }
        }
    }
    unknown_228 = target;
    if (SpawnManager != 0 && target == 0) {
        SpawnManager->Set_Target(target);
    }
    if (unknown_228 == 0) {
        unknown_310 = 0;
    }
    if (unknown_25c[0] != 0 && !unknown_slot213(target)) {
        unknown_25c[0]->Delete_Me();
        unknown_25c[0] = 0;
    }
}


// Rearm delay for a selected weapon; existing slot187 ABI is preserved.
int TechnoClass::unknown_slot187(int which)
{
    if (Kind_Of()==6 && unknown_254>1) return 1;
    WeaponTypeClass *weapon=unknown_slot231(which)->WeaponType;
    if (!weapon) return 1;
    if (weapon->unknown_10c || (weapon->unknown_106 && unknown_25c[1]) ||
        (weapon->unknown_105 && unknown_25c[0]) || (weapon->unknown_109 && unknown_25c[4])) {
        return weapon->unknown_b0;
    }
    UnitClass *unit;
    if (!this) unit=0;
    else unit=Kind_Of()==1 ? static_cast<UnitClass *>(this) : 0;
    if (unknown_310<weapon->Burst) {
        int delay=-1;
        if (unknown_310>0 && unknown_310<=4 && unit) delay=unit->Type->BurstDelay[unknown_310-1];
        return delay!=-1 ? delay : Random_Pick(3,5);
    } else {
        int delay=weapon->unknown_b0*House->unknown_f0+Random_Pick(0,2);
        if (unknown_118.Is_Veteran() || unknown_118.Is_Elite()) {
            TechnoTypeClass *type=Techno_Type_Class();
            if ((unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[4]) ||
                (unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[4] || type->EliteAbilities.Ability[4]))) {
                delay=delay*Rules->VeteranROF;
            }
        }
        if (Kind_Of()==6 && static_cast<BuildingClass *>(this)->unknown_4532f0()>0) {
            delay/=static_cast<BuildingClass *>(this)->unknown_4532f0();
        }
        return delay;
    }
}


// Not exact (RA2_NON_MATCHING): TechnoClass::unknown_6c9df0 at 0x006C9DF0, best score 99.46, 2026-11-13; differs: same instructions; registers, stack slots or operands differ
#ifdef RA2_NON_MATCHING
// LaserDrawClass *TechnoClass::unknown_6c9df0(AbstractClass *target, int weapon_index, WeaponTypeClass *weapon, CoordStruct const &source) (0x006C9DF0)
// Draws the weapon's laser from source (or the weapon's fire coordinate when
// source is the zero coordinate; a building then lowers it by the screen
// height of that coordinate above its slot 42 coordinate) to the target's
// coordinate: the weapon's inner/outer colours, or with IsHouseColor (+0x123)
// the owner's laser colour (+0x55EC) and its half as the outer colour.
// Body 560 bytes: ret 10h at 0x006CA01D (the inventory stopped at 368).

LaserDrawClass *TechnoClass::unknown_6c9df0(AbstractClass *target, int weapon_index, WeaponTypeClass *weapon,
                                            CoordStruct const &source)
{
    RGBClass half;
    int duration = weapon->unknown_124;
    RGBClass const *spread = &weapon->unknown_102;
    RGBClass const *inner;
    RGBClass const *outer;
    if (weapon->unknown_123) {
        inner = &House->unknown_55ec;
        half.Red = inner->Red >> 1;
        half.Green = inner->Green >> 1;
        half.Blue = inner->Blue >> 1;
        outer = &half;
    } else {
        inner = &weapon->unknown_fc;
        outer = &weapon->unknown_ff;
    }
    CoordStruct from;
    CoordStruct to;
    int depth = 0;
    if (source != unknown_geometry_l) {
        from = source;
    } else {
        from = unknown_slot43(weapon_index);
        if (Kind_Of() == 6) {
            Point2D a = unknown_tactical_83996c->Coord_To_Pixel_Absolute(from);
            Point2D b = unknown_tactical_83996c->Coord_To_Pixel_Absolute(unknown_slot42());
            depth = a.Y - b.Y;
            if (depth > 0) {
                depth = 0;
            }
        } else {
            depth = 0;
        }
    }
    if (target != 0 && target->AbstractFlags.IsObject) {
        to = ((ObjectClass *)target)->unknown_slot40();
    } else {
        to = target->unknown_slot22();
    }
    LaserDrawClass *laser = new LaserDrawClass(from, to, depth, true, *inner, *outer, *spread, duration,
                                               false, true, 1.0f, 0.0f);
    if (laser != 0 && weapon->unknown_123) {
        laser->IsHouseColor = true;
    }
    return laser;
}
#endif

// EBolt *TechnoClass::unknown_6ca020(AbstractClass *target, WeaponTypeClass *weapon, CoordStruct const &source) (0x006CA020)
// Fires an electric bolt from source to the target: a new EBolt, set up
// from source to the target's coordinate (slot 40 for objects, slot 22
// otherwise); a building raises it by the screen height of the source above
// its own slot 42 coordinate. The weapon's +0x129 flag is copied to the
// bolt's +0x2C.

EBolt *TechnoClass::unknown_6ca020(AbstractClass *target, WeaponTypeClass *weapon, CoordStruct const &source)
{
    int depth = 0;
    EBolt *bolt = new EBolt;
    if (bolt == 0) {
        return 0;
    }
    if (Kind_Of() == 6) {
        Point2D from = unknown_tactical_83996c->Coord_To_Pixel_Absolute(source);
        Point2D to = unknown_tactical_83996c->Coord_To_Pixel_Absolute(unknown_slot42());
        depth = from.Y - to.Y;
        if (depth > 0) {
            depth = 0;
        }
    }
    CoordStruct coord;
    if (target != 0 && target->AbstractFlags.IsObject) {
        coord = ((ObjectClass *)target)->unknown_slot40();
    } else {
        coord = target->unknown_slot22();
    }
    if (weapon != 0) {
        bolt->unknown_2c = weapon->unknown_129;
    }
    bolt->Setup_004B2430(source, coord, depth);
    return bolt;
}


// EBolt *TechnoClass::unknown_6ca130(AbstractClass *target) (0x006CA130)
// Fires the weapon chosen for the target as an electric bolt from its fire
// coordinate (0x006CA020); a unit with +0x7D set and no bolt yet keeps it
// at +0x5C4 and attaches itself (0x004B2580). Never called: inlined at
// every use.

EBolt *TechnoClass::unknown_6ca130(AbstractClass *target)
{
    int weapon = unknown_slot174(target);
    WeaponTypeClass *type = unknown_slot231(weapon)->WeaponType;
    CoordStruct source = unknown_slot43(weapon);
    EBolt *bolt = unknown_6ca020(target, type, source);
    if (bolt != 0 && Kind_Of() == 1 && unknown_7d && ((UnitClass *)this)->unknown_5c4 == 0) {
        ((UnitClass *)this)->unknown_5c4 = bolt;
        bolt->Attach_004B2580(this, weapon);
    }
    return bolt;
}


// void TechnoClass::unknown_6ca1d0(AbstractClass *target, int chrono) (0x006CA1D0)
// Draws a beam (RadBeam type 1) from the fire coordinate of weapon 0 to the
// target's coordinate (slot 40 for objects, slot 22 otherwise) in Rules
// RadColor, or ChronoBeamColor when chrono is 1, period 15, amplitude 40,
// with the screen height of the fire coordinate above slot 42's when they
// differ in Y (TechnoClass 0x006CA890).

void TechnoClass::unknown_6ca1d0(AbstractClass *target, int chrono)
{
    RadBeam *beam = RadBeam::Allocate(1);
    int depth = 0;
    CoordStruct source = unknown_slot43(0);
    CoordStruct center = unknown_slot42();
    if (source.Y != center.Y) {
        Point2D from = unknown_tactical_83996c->Coord_To_Pixel_Absolute(source);
        Point2D to = unknown_tactical_83996c->Coord_To_Pixel_Absolute(center);
        depth = from.Y - to.Y;
    }
    CoordStruct coord;
    bool object = target != 0 && target->AbstractFlags.IsObject;
    if (!object) {
        coord = target->unknown_slot22();
    } else {
        coord = ((ObjectClass *)target)->unknown_slot40();
    }
    RGBClass color;
    switch (chrono) {
    case 1:
        color = Rules->ChronoBeamColor;
        break;
    default:
        color = Rules->RadColor;
        break;
    }
    beam->Set_Source(source);
    beam->Set_Target(coord);
    beam->Set_Period(15);
    beam->Set_Amplitude(40.0);
    beam->Set_Color(color);
    beam->Set_Depth(depth);
}


// void TechnoClass::unknown_6ca340(int spread) (0x006CA340)
// Draws eight beams (RadBeam type 2, Rules RadColor) from the centre to
// random points of the cells spread cells away in each direction, with
// random period, amplitude and +0x48 value (TechnoClass 0x006CA890).

void TechnoClass::unknown_6ca340(int spread)
{
    CoordStruct center = Center_Coord();
    int longest = 0;
    Cell cell = Coord_Cell(center);
    RGBClass color = Rules->RadColor;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x != 0 || y != 0) {
                // Each store and multiply is live; this spelling (not Cell(ox, oy)) gives both add destinations.
                int ox = spread * x;
                Cell offset(ox, spread);
                offset.Y *= y;
                Cell spot = offset + cell;
                CoordStruct to = ((UnknownCellClass *)&Map[spot])->unknown_475d60();
                int dx = unknown_random_839218(-128, 128);
                int dy = unknown_random_839218(-128, 128);
                to.X += dx;
                to.Y += dy;
                RadBeam *beam = RadBeam::Allocate(2);
                beam->Set_Color(color);
                int period = unknown_random_839218(5, 20);
                int amplitude = unknown_random_839218(100, 500);
                int value = unknown_random_839218(0, 20);
                beam->Set_Source(center);
                beam->Set_Target(to);
                beam->Set_Period(period);
                beam->Set_Amplitude(amplitude);
                beam->unknown_6357b0(0.0);
                beam->unknown_6357d0(value);
                if (longest <= period) {
                    longest = period;
                }
            }
        }
    }
}


// UnknownFacingDir TechnoClass::unknown_slot170(AbstractClass *target) (0x006CA540, slot 170)
// The firing arc toward target (hypothesis: the ballistic pitch): the
// type's FireAngle without a target; otherwise from the horizontal distance
// and height difference between the line end (0x006D65B0) and the weapon's
// fire coordinate, the weapon's speed (0x00733F70) and the projectile's
// gravity (Rules +0x1320, or 0x0047FCD0 for floaters) through 0x0047F8B0,
// or 0xE000 when that fails.

UnknownFacingDir TechnoClass::unknown_slot170(AbstractClass *target)
{
    UnknownFacingDir dir((unsigned short)(Techno_Type_Class()->FireAngle << 8));
    if (target != 0) {
        CoordStruct end = unknown_6d65b0();
        int weapon;
        if (Techno_Type_Class()->Has_Turret_Weapons()) {
            weapon = unknown_10c;
        } else {
            weapon = 0;
        }
        CoordStruct delta = end - unknown_slot181(weapon);
        WeaponTypeClass *type = unknown_slot231(weapon)->WeaponType;
        double speed = Rules->unknown_1320;
        if (type->Projectile != 0 && type->Projectile->Floater) {
            speed = Unknown_0047FCD0();
        }
        int distance = (int)unknown_sqrt_4ba410(pow(delta.X, 2.0) + pow(delta.Y, 2.0));
        int velocity = type->Get_Speed(distance);
        if (!Unknown_0047F8B0(unknown_6d7d20(weapon), velocity, distance, delta.Z, speed, &dir)) {
            dir = UnknownFacingDir(0xE000);
        }
    }
    return dir;
}


// int TechnoClass::unknown_6ca6c0(TechnoClass *target, WeaponTypeClass *weapon) (0x006CA6C0)
// The damage a weapon would do to target (0 without a target or for a
// weapon flagged +0x10C or +0x105): Damage times the owner's +0xD0 and the
// techno's +0x128 multipliers (and VeteranCombat for a veteran/elite with
// ability 2), divided by the target owner's armor multiplier against this
// type times +0x120 (and VeteranArmor for a target with ability 1), at
// least 1, then through the warhead against the target's armor
// (0x0047E1C0).

int TechnoClass::unknown_6ca6c0(TechnoClass *target, WeaponTypeClass *weapon)
{
    if (target == 0) {
        return 0;
    }
    int damage = weapon->unknown_a4;
    if (weapon->unknown_10c || weapon->unknown_105) {
        return 0;
    }
    if (damage > 0) {
        damage = House->unknown_d0 * unknown_128 * damage;
        if (unknown_118.Is_Veteran() || unknown_118.Is_Elite()) {
            TechnoTypeClass *type = Techno_Type_Class();
            if ((unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[2])
                || (unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[2] || type->EliteAbilities.Ability[2]))) {
                damage = damage * Rules->VeteranCombat;
            }
        }
        double value = damage;
        damage = value / (target->House->unknown_func_4f58a0(Techno_Type_Class()) * unknown_120);
        if (target->unknown_118.Is_Veteran() || target->unknown_118.Is_Elite()) {
            TechnoTypeClass *type = target->Techno_Type_Class();
            if ((target->unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[1])
                || (target->unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[1] || type->EliteAbilities.Ability[1]))) {
                damage = damage / Rules->VeteranArmor;
            }
        }
        if (damage < 1) {
            damage = 1;
        }
        damage = Unknown_0047E1C0(damage, weapon->Warhead, target->Class_Of()->unknown_9c, 0);
    }
    return damage;
}


// BulletClass *TechnoClass::unknown_slot221(AbstractClass *target, int which) (0x006CA890)
// TibSun TechnoClass::Fire_At.
inline int Length_006ca890(CoordStruct const &c) { return (int)unknown_sqrt_4ba410((double)c.X * (double)c.X + ((double)c.Z * (double)c.Z + (double)c.Y * (double)c.Y)); }
struct Point2F_006ca890 { Point2F_006ca890() {} Point2F_006ca890(float x, float y) : X(x), Y(y) {} float X; float Y; };
struct Point3F_006ca890 : public Point2F_006ca890 {
    Point3F_006ca890(float x, float y, float z) : Point2F_006ca890(x, y), Z(z) {}
    float Length() const
    {
        double sum = (double)Z * (double)Z;
        sum += (double)X * (double)X + (double)Y * (double)Y;
        float length = (float)unknown_sqrt_4ba410(sum);
        return length;
    }
    float Z;
};
inline int Planar_006ca890(CoordStruct const &c) { float y = (float)c.Y; return (int)unknown_sqrt_4ba410((double)y * (double)y + (double)c.X * (double)c.X); }
extern bool unknown_flag_a40d13;
extern CoordStruct const unknown_coord_none_ac0188;
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
extern double const unknown_tau_79d158;
extern double const unknown_twenty_79d150;
struct UnknownUnitView_006ca890 { char unknown_000[0x5C4]; EBolt *unknown_5c4; };
extern double const unknown_facing_radians_79b620;
extern double const unknown_two_pi_79c9b8;
inline double Random_Double_006ca890(double low, double high)
{
    int sample = unknown_id_source_a3d290->Random()(0, 0x7FFFFFFE);
    return (high - low) * ((double)sample * unknown_random_scale_79c280) + low;
}
inline unsigned short Random_Direction_006ca890()
{
    return (unsigned short)(int)((Random_Double_006ca890(0.0, 1.0) * unknown_two_pi_79c9b8 - unknown_facing_origin_79b630) * unknown_facing_scale_79b628);
}
extern double const unknown_small_7a10f8;
float __stdcall Fast_Atan2_Float_004baba0(float y, float x);
struct AircraftVectorXY;
double __fastcall Aircraft_XY_Length_0041B750(AircraftVectorXY const *vector);
inline double Radian_006ca890(UnknownFacingDir const &dir)
{
    return ((short)dir.Raw - 0x3FFF) * unknown_facing_radians_79b620;
}
inline void Set_Yaw_006ca890(TVelocity3D<double> &velocity, UnknownFacingDir const &yaw)
{
    if (velocity.X == 0.0 && velocity.Y == 0.0) {
        velocity.X = 100.0;
    }
    double horizontal = unknown_sqrt_4ba410(velocity.X * velocity.X + velocity.Y * velocity.Y);
    double angle = Radian_006ca890(yaw);
    velocity.X = unknown_cos_4ba4d0(angle) * horizontal;
    velocity.Y = -(unknown_sin_4ba480(angle) * horizontal);
}
inline void Set_Pitch_006ca890(TVelocity3D<double> &velocity, UnknownFacingDir const &pitch)
{
    double current = Radian_006ca890(UnknownFacingDir((unsigned short)((unknown_math_4ba600(velocity.Z, Aircraft_XY_Length_0041B750((AircraftVectorXY const *)&velocity)) - 1.5707963267948966) * -10430.060040584269)));
    double speed = unknown_sqrt_4ba410((velocity.X * velocity.X + velocity.Y * velocity.Y) + velocity.Z * velocity.Z);
    if (current != unknown_double_79b610) {
        velocity.X /= unknown_cos_4ba4d0(current);
        velocity.Y /= unknown_cos_4ba4d0(current);
    }
    velocity.X *= unknown_cos_4ba4d0(Radian_006ca890(pitch));
    velocity.Y *= unknown_cos_4ba4d0(Radian_006ca890(pitch));
    velocity.Z = speed * unknown_sin_4ba480(Radian_006ca890(pitch));
}
inline CoordStruct Move_Coord_006ca890(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
    double radians = Radian_006ca890(dir);
    return CoordStruct((int)(start.X + unknown_cos_4ba4d0(radians) * distance), (int)(start.Y - unknown_sin_4ba480(radians) * distance), start.Z);
}
inline bool Is_Foot_006ca890(ObjectClass const *object)
{
    if (object == 0) {
        return false;
    }
    unsigned char flag = object->AbstractFlags.IsFoot;
    return flag != 0;
}
inline bool Has_Ability_006ca890(TechnoClass *techno, int ability)
{
    if (techno->unknown_118.Is_Veteran() || techno->unknown_118.Is_Elite()) {
        TechnoTypeClass *type = techno->Techno_Type_Class();
        if ((techno->unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[ability])
            || (techno->unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[ability] || type->EliteAbilities.Ability[ability]))) {
            return true;
        }
    }
    return false;
}
template<class T> inline T MIN_006ca890(T a, T b) { return a < b ? a : b; }
inline void Recoil_Start_006ca890(RecoilData &block)
{
    if (block.unknown_00 != 0) {
        block.unknown_18 = 1;
        block.unknown_1c = block.unknown_04;
        if (block.unknown_04 < 1) {
            block.unknown_1c = 1;
        }
        block.unknown_10 = (float)block.unknown_00 / (float)block.unknown_1c;
    }
}


BulletClass *TechnoClass::unknown_slot221(AbstractClass *target, int which)
{
    WeaponTypeClass *weapon = unknown_slot231(which)->WeaponType;
    if (weapon == 0) {
        return 0;
    }
    ObjectClass *object = target == 0 ? 0 : (target->AbstractFlags.IsObject ? static_cast<ObjectClass *>(target) : 0);
    BulletTypeClass *btype = weapon->Projectile;
    if (unknown_flag_a40d13 || target == 0 || (object != 0 && object->unknown_76)) {
        return 0;
    }
    if (weapon->unknown_11c) {
        int damage = Techno_Type_Class()->MaxStrength;
        Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, false);
        return 0;
    }
    if ((weapon->unknown_105 && unknown_25c[0] != 0) || (weapon->unknown_109 && unknown_25c[4] != 0)
        || (weapon->unknown_106 && unknown_25c[1] != 0) || (weapon->unknown_10c && unknown_27c != 0)) {
        return 0;
    }
    if (weapon->unknown_10d) {
        SpawnManager->Set_Target(target);
        if (unknown_372 || unknown_373) {
            if (!Map.Is_Shrouded(Center_Coord()) && !Map.Is_Fogged(Center_Coord())) {
                return 0;
            }
            if (Kind_Of() == 2 && unknown_372) {
                return 0;
            }
        }
        if (object != 0) {
            HouseClass *owner = object->Owning_House();
            if (owner != 0 && owner->Is_Player_Control() && weapon->unknown_113) {
                Map.Unknown_00549C50(Center_Coord(), 3, owner, false, false, false, true, false);
            }
            Map.Unknown_0054A550(Center_Coord(), 0, 4, 0);
        }
        return 0;
    }
    TVelocity3D<double> velocity;
    CoordStruct target_coord;
    if (object != 0) {
        target_coord = object->unknown_slot40();
    } else {
        target_coord = target->unknown_slot22();
    }
    CoordStruct fire_coord = unknown_slot43(which);
    UnknownFacingDir dir;
    if (btype->ROT == 0 && !btype->Dropping) {
        dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)fire_coord.Y - (double)target_coord.Y, (double)target_coord.X - (double)fire_coord.X) - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
    } else {
        dir = unknown_slot183();
        if (btype->Dropping) {
            fire_coord = Center_Coord();
        }
    }
    int attack = weapon->unknown_a4;
    int firepower = attack;
    if (weapon->unknown_10c || weapon->unknown_105) {
        firepower = 0;
    }
    if (firepower > 0) {
        firepower = (int)(House->unknown_d0 * unknown_128 * attack);
        if (Has_Ability_006ca890(this, 2)) {
            firepower = (int)(firepower * Rules->VeteranCombat);
        }
    }
    Point2D delta = Point2D(target_coord.X, target_coord.Y) - Point2D(fire_coord.X, fire_coord.Y);
    int distance = (int)Fast_Sqrt((double)(delta.X * delta.X + delta.Y * delta.Y));
    int speed = weapon->Get_Speed(distance);
    BulletClass *bullet = Create_Bullet(weapon->Projectile, target, this, firepower, weapon->Warhead, speed, weapon->unknown_10b);
    if (bullet != 0) {
        bullet->Set_Weapon_Type(weapon);
        bullet->Limbo();
        if (Is_Foot_006ca890(this)) {
            if (static_cast<FootClass *>(this)->Locomotor.Checked()->Is_Moving() && !Techno_Type_Class()->JumpJet) {
                bullet->unknown_a0 = true;
            }
        }
        if (!bullet->unknown_a0 && !btype->Inaccurate) {
            TechnoClass *techno = unknown_228 == 0 ? 0 : (unknown_228->AbstractFlags.IsTechno ? static_cast<TechnoClass *>(unknown_228) : 0);
            WeaponTypeClass *bullet_weapon = bullet->Get_Weapon_Type();
            if (techno != 0 && bullet_weapon != 0) {
                techno->unknown_70 -= unknown_6ca6c0(techno, bullet_weapon);
            }
        }
        CoordStruct displacement;
        displacement = unknown_6d65b0() - fire_coord;
        if (btype->Inaccurate && btype->Arcing) {
            if (!btype->FlakScatter || btype->Inviso) {
                int spread = unknown_id_source_a3d290->Random()(Rules->unknown_139c / 2, Rules->unknown_139c);
                UnknownFacingDir facing(Random_Direction_006ca890());
                displacement = Move_Coord_006ca890(displacement, facing, spread);
            } else {
                float length = Point3F_006ca890((float)displacement.X, (float)displacement.Y, (float)displacement.Z).Length();
                int spread = Random_Pick(0, Rules->unknown_139c);
                spread *= (int)length;
                spread /= Weapon_Range(which);
                UnknownFacingDir facing(Random_Direction_006ca890());
                displacement = Move_Coord_006ca890(displacement, facing, spread);
            }
        }
        if (btype->ROT == 0 && !btype->Dropping) {
            double angle = Fast_Atan2((double)-displacement.Y, (double)displacement.X);
            dir = UnknownFacingDir((unsigned short)(int)((angle - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
        } else {
            dir = unknown_slot183();
            if (btype->Dropping) {
                fire_coord = Center_Coord();
            }
        }
        if (speed > Length_006ca890(displacement) / 2) {
            speed = Length_006ca890(displacement) / 2;
        }
        int segments = Techno_Type_Class()->RadialFireSegments;
        BulletTypeClass *projectile = weapon->Projectile;
        if (projectile->ROT > 0 || projectile->Vertical) {
            if (segments == 0) {
                speed = 1;
            }
            bullet->Speed = weapon->unknown_a8;
        }
        double full_speed = (double)speed;
        velocity.Y = 0.0;
        velocity.Z = 0.0;
        velocity.X = 100.0;
        double length = unknown_sqrt_4ba410(velocity.X * velocity.X + velocity.Y * velocity.Y + velocity.Z * velocity.Z);
        double ratio = full_speed / length;
        velocity.X *= ratio;
        velocity.Y *= ratio;
        velocity.Z *= ratio;
        if (segments > 0) {
            double angle = unknown_tau_79d158 / (double)segments * unknown_38c - unknown_facing_origin_79b630;
            UnknownFacingDir current = unknown_2e0.Current();
            UnknownFacingDir facing((unsigned short)(int)((((short)current.Raw - 0x3FFF) * unknown_facing_radians_79b620 + angle - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
            velocity.Set_Yaw(facing);
            unknown_38c++;
            if (unknown_38c >= segments) {
                unknown_38c = 0;
            }
        } else {
            velocity.Set_Yaw(dir);
        }
        int height = displacement.Z;
        UnknownFacingDir pitch(0x3FFF);
        bool valid = true;
        if (bullet->Type->Arcing) {
            double gravity = (double)Rules->unknown_1320;
            if (bullet->Type->Floater) {
                gravity = Unknown_0047FCD0();
            }
            int planar = Planar_006ca890(displacement);
            valid = Unknown_0047F8B0(unknown_6d7d20(which), speed, planar, height, gravity, &pitch);
        } else if (bullet->Type->unknown_206) {
            if (target->Center_Coord().Z < Center_Coord().Z) {
                pitch = UnknownFacingDir(0x8000);
            } else {
                pitch = UnknownFacingDir(0x4000);
            }
        } else {
            int abs_z = abs(height);
            if (abs_z > 200) {
                double y = 20.0;
                TechnoClass *tarcom = (TechnoClass *)unknown_228;
                BuildingClass *building = (BuildingClass *)tarcom;
                int kind = building != 0 ? building->Kind_Of() : 0;
                if (kind == 6) {
                    CoordStruct turret = unknown_slot181(which);
                    height = building->Type->Height * 200 - turret.Z;
                    abs_z = abs(height);
                    if ((double)abs_z < unknown_twenty_79d150) {
                        y = 0.0;
                    }
                }
                double planar = (double)Planar_006ca890(displacement);
                if (planar < unknown_small_7a10f8) {
                    planar = unknown_small_7a10f8;
                }
                double angle = Fast_Atan2_Float_004baba0((float)((double)abs_z - y), (float)planar);
                if (height < 0) {
                    angle = -angle;
                }
                pitch = UnknownFacingDir((unsigned short)(int)((angle - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
            }
        }
        Set_Pitch_006ca890(velocity, pitch);
        if (valid) {
          if (!bullet->unknown_slot118(fire_coord, velocity)) {
            bullet->Release();
            bullet = 0;
          } else {
            if (bullet->Type->Inviso && object != 0 && object->unknown_79) {
                bullet->unknown_79 = true;
            }
            if (unknown_slot232() && Techno_Type_Class()->TurretRecoil) {
                Recoil_Start_006ca890(unknown_330);
                Recoil_Start_006ca890(unknown_350);
            }
            if (weapon->unknown_105 && unknown_25c[0] == 0) {
                unknown_25c[0] = new ParticleSystemClass(weapon->unknown_f8, fire_coord, target, this, unknown_coord_none_ac0188);
            }
            if (weapon->unknown_106 && unknown_25c[1] == 0) {
                unknown_25c[1] = new ParticleSystemClass(weapon->unknown_f8, fire_coord, target, this, unknown_coord_none_ac0188);
            }
            if (weapon->unknown_109 && unknown_25c[4] == 0) {
                CoordStruct end = Railgun_Beam_Damage(fire_coord, target, weapon);
                unknown_25c[4] = new ParticleSystemClass(weapon->unknown_f8, fire_coord, 0, this, end);
            }
            unknown_310++;
            int delay = unknown_slot187(which);
            unknown_250 = delay;
            unknown_244 = delay;
            unknown_310 %= weapon->Burst;
            AnimTypeClass *anim_type = 0;
            int anim_count = weapon->Anim.Count();
            if (anim_count == 8) {
                anim_type = weapon->Anim[(Dir_To_8(unknown_slot183()) + 1) % 8];
            } else if (anim_count > 0) {
                anim_type = weapon->Anim[0];
            }
            if (weapon->unknown_bc.Count() > 0) {
                int voc = weapon->unknown_bc.Pick((unsigned short)unknown_320);
                Voc_Play_At(voc, fire_coord, 0);
            }
            if (anim_type != 0) {
                AnimClass *anim = new AnimClass(anim_type, fire_coord, 0, 1, 0x600, 0);
                if (Kind_Of() == 6) {
                    int dy = (fire_coord - unknown_slot42()).Y;
                    int zadjust = dy / -4;
                    anim->ZAdjust = MIN_006ca890(zadjust, 0);
                    BuildingClass *building = static_cast<BuildingClass *>(this);
                    if (building->unknown_4532f0() > 0) {
                        anim->ZAdjust = -200;
                        building->unknown_56c++;
                        building->unknown_56c %= building->unknown_4532f0();
                    }
                }
                if (anim != 0 && Kind_Of() != 6) {
                    anim->Attach_To(this);
                }
            }
            if (weapon->unknown_10c) {
                unknown_27c = new WaveClass(fire_coord, target_coord, this, 0, (ObjectClass *)target);
            }
            if (Techno_Type_Class()->TargetLaser && House->Is_Player_Control()) {
                unknown_314 = 15;
            }
            if (weapon->unknown_121) {
                if (Kind_Of() == 6) {
                    WeaponTypeClass *primary = unknown_slot230()->WeaponType;
                    LaserDrawClass *laser = unknown_6c9df0(target, which, primary, unknown_coord_none_ac0188);
                    BuildingClass *building = static_cast<BuildingClass *>(this);
                    if (laser != 0 && building->Type == Rules->PrismType) {
                        laser->unknown_18 = 3;
                        if (building->unknown_534 > 0) {
                            laser->unknown_1d = true;
                            laser->unknown_18 = 5;
                        }
                    }
                } else {
                    LaserDrawClass *laser = unknown_6c9df0(target, which, weapon, unknown_coord_none_ac0188);
                    if (laser != 0 && weapon->unknown_123) {
                        laser->unknown_18 = 2;
                    }
                }
            } else if (weapon->unknown_127) {
                int index = unknown_slot174(target);
                WeaponTypeClass *bolt_weapon = unknown_slot231(index)->WeaponType;
                CoordStruct source = unknown_slot43(index);
                EBolt *bolt = unknown_6ca020(target, bolt_weapon, source);
                if (bolt != 0 && Kind_Of() == 1 && unknown_7d && reinterpret_cast<UnknownUnitView_006ca890 *>(this)->unknown_5c4 == 0) {
                    reinterpret_cast<UnknownUnitView_006ca890 *>(this)->unknown_5c4 = bolt;
                    bolt->Attach_004B2580(this, index);
                }
            } else if (weapon->unknown_12a) {
                WarheadTypeClass *warhead = weapon->Warhead;
                if (warhead == 0 || !warhead->Temporal) {
                    unknown_6ca1d0(target, 0);
                } else {
                    unknown_6ca1d0(target, 1);
                }
            } else if (weapon->unknown_12b) {
                unknown_6ca340((int)weapon->Warhead->CellSpread);
            }
            unknown_slot207();
            Mark(2);
            if (!(unknown_372 || unknown_373)
                || ((Map.Is_Shrouded(Center_Coord()) || Map.Is_Fogged(Center_Coord())) && !(Kind_Of() == 2 && unknown_372))) {
                if (target->AbstractFlags.IsObject) {
                    HouseClass *owner = static_cast<ObjectClass *>(target)->Owning_House();
                    if (owner != 0 && owner->Is_Player_Control() && weapon->unknown_113) {
                        Map.Unknown_00549C50(Center_Coord(), 3, owner, false, false, false, true, false);
                        Map.Unknown_0054A550(Center_Coord(), 0, 4, 0);
                    }
                }
            }
          }
        } else {
            bullet->Release();
            bullet = 0;
        }
    }
    if (weapon->unknown_10e) {
        if (Techno_Type_Class()->ReselectIfLimboed && unknown_77) {
            HouseClass *house = Owning_House();
            if (house->Is_Player_Control() && target->Kind_Of() == 15) {
                unknown_381 = true;
            }
        }
        if (Techno_Type_Class()->RejoinTeamIfLimboed && this != 0 && AbstractFlags.IsFoot && static_cast<FootClass *>(this)->Team != 0 && target->Kind_Of() == 15) {
            unknown_384 = (this != 0 && AbstractFlags.IsFoot ? static_cast<FootClass *>(this) : 0)->Team;
        }
        Limbo();
        if (weapon->Warhead->Parasite) {
            if (target->AbstractFlags.IsFoot) {
                static_cast<FootClass *>(target)->unknown_584 = Frame + 20;
            }
            bullet->Limbo();
            bullet->Construct(weapon->Projectile, target, this, firepower, weapon->Warhead, weapon->unknown_a8, weapon->unknown_10b);
            bullet->unknown_slot118(fire_coord, velocity);
        }
    }
    if (Techno_Type_Class()->DistributedFire && (Mission != 1 || Kind_Of() == 6)) {
        unknown_3c0.Add(target);
        unknown_slot220(0);
    }
    if (weapon->unknown_111) {
        if (this != 0 && AbstractFlags.IsFoot && static_cast<FootClass *>(this)->Team != 0) {
            static_cast<FootClass *>(this)->Team->unknown_func_6b79e0(0);
            static_cast<FootClass *>(this)->Team->StepCompleted = true;
        }
        unknown_slot220(0);
    }
    return bullet;
}


// Draw a dashed targeting line with a three-pixel marker at each end.
extern Rect unknown_rect_839630;
// Reference rectangle/point operations, with their ordinary C++ copy lifetimes.
inline Rect::Rect(RectPoint2D const &point,int width,int height):X(point.X),Y(point.Y),Width(width),Height(height) {}
inline RectPoint2D Rect::Top_Left() const { return RectPoint2D(X,Y); }
inline RectPoint2D RectPoint2D::operator-(RectPoint2D const &point) const { return RectPoint2D(X-point.X,Y-point.Y); }
inline Rect Rect::operator-(RectPoint2D const &point) const { return Rect(Top_Left()-point,Width,Height); }
static inline Rect Target_Laser_Rect(Point2D const &point)
{
    Point2D offset=point-Point2D(2,2);
    return Rect(RectPoint2D(offset.X,offset.Y),3,3);
}
void TechnoClass::unknown_6cc020()
{
    static bool pattern[16]={true,false,true,false,true,false,true,false,true,false,true,false,true,false,true,false};
    if(unknown_228) {
        CoordStruct source=unknown_slot181(0);
        CoordStruct target=unknown_6d65b0();
        Point2D start;
        Point2D end;
        unknown_tactical_83996c->Coord_To_Pixel(source,start);
        unknown_tactical_83996c->Coord_To_Pixel(target,end);
        start.Y+=unknown_rect_839630.Y;
        end.Y+=unknown_rect_839630.Y;
        unknown_surface_83995c->Fill_Rect(unknown_rect_839630.Intersect(Rect(RectPoint2D(start.X,start.Y),3,3)-RectPoint2D(2,2)),unknown_drawer_8323c4->Convert(8));
        unknown_surface_83995c->Fill_Rect(unknown_rect_839630.Intersect(Target_Laser_Rect(end)),unknown_drawer_8323c4->Convert(8));
        Rect clip=unknown_rect_839630;
        if(Clip_Line(start,end,clip)) {
            unknown_surface_83995c->Draw_Dashed_Line(start,end,unknown_drawer_8323c4->Convert(8),pattern,7*Frame%16);
        }
    }
}


// bool TechnoClass::unknown_slot200(int mission, AbstractClass *target, AbstractClass *destination, AbstractClass *follow) (0x006CC2A0, slot 200)
// The player orders a mission: a move or attack (1, 2) becomes 0x1D while
// 0x006F6670 and slot 275 hold; planning mode records a MEGAMISSION event
// instead; otherwise the mission's voice plays and the MEGAMISSION is queued.

bool Planning_Active();
bool Unknown_006F6670();

bool TechnoClass::unknown_slot200(int mission, AbstractClass *target, AbstractClass *destination, AbstractClass *follow)
{
    bool special = Unknown_006F6670() && unknown_slot275();
    if ((mission == 2 || mission == 1) && special) {
        mission = 0x1D;
    }
    if (Planning_Active()) {
        EventClass event(unknown_player_a35db4->ArrayIndex, TargetClass(this), (unsigned char)mission, target, destination, follow);
        Planning_Send_Event(event);
        return false;
    }
    if (unknown_sound_enabled_7d88d6) {
        if (mission == 1) {
            unknown_slot198();
        } else if (mission == 2) {
            unknown_slot197();
        } else if (mission == 7) {
            if (Techno_Type_Class()->VoiceEnter != -1) {
                Voc_Play(Techno_Type_Class()->VoiceEnter, 1.0f, 0x2000, 0);
            } else {
                unknown_slot197();
            }
        } else if (mission == 8) {
            if (Techno_Type_Class()->VoiceCapture != -1) {
                Voc_Play(Techno_Type_Class()->VoiceCapture, 1.0f, 0x2000, 0);
            } else if (Techno_Type_Class()->VoiceEnter != -1) {
                Voc_Play(Techno_Type_Class()->VoiceEnter, 1.0f, 0x2000, 0);
            } else {
                unknown_slot197();
            }
        } else if (Techno_Type_Class()->VoiceSpecialAttack.Count() > 0) {
            DynamicVectorClass<int> &sounds = Techno_Type_Class()->VoiceSpecialAttack;
            unsigned random = unknown_random_839218();
            Voc_Play(sounds[random % sounds.Count()], 1.0f, 0x2000, 0);
        }
    }
    {
        TargetClass destination_target(destination);
        TargetClass target_target(target);
        Queue_Mission(TargetClass(this), mission, target_target, destination_target);
    }
    return true;
}


// bool TechnoClass::unknown_slot199(int type) (0x006CC510, slot 199)
// Queues an event of the given type about this object for the player's
// house; planning mode refuses it instead.

bool Planning_Active();

bool TechnoClass::unknown_slot199(int type)
{
    EventClass event(unknown_player_a35db4->ArrayIndex, type, TargetClass(this));
    if (Planning_Active()) {
        Planning_Refuse_Event(&event);
        return false;
    }
    OutList.Add(event);
    return true;
}


// int TechnoClass::unknown_slot28(ObjectClass *object, bool arg2) (0x006CC5D0, slot 28)
// The action against an object (Red Alert's What_Action(ObjectClass const *)):
// none for no object; a passenger-style techno (slot 111) only selects; the
// one selected object detonating its own bomb (0x37), self actions (4), the
// force-move (8), force-fire and select-key overrides (0x1A, 1), then the
// attack (5) when the weapon can (slot 218 is not 5, a spy or engineer's
// building target, or force-fire), else select (7) or none.

// A key binding is down (the bindings are ints; WWKeyboardClass takes 16-bit keys).
inline bool Techno_Key_Down(int key) { return WWKeyboard->Down(key); }

int TechnoClass::unknown_slot28(ObjectClass *object, bool arg2)
{
    if (object == 0) {
        return 0;
    }
    if (unknown_slot111()) {
        if (unknown_slot163() && House->Is_Player_Control() && object->Owner() != Owner()) {
            return 0;
        }
        if (unknown_32c && unknown_372) {
            return 0;
        }
        if (!object->CanBeSelected() || object->unknown_77) {
            return 0;
        }
        return 7;
    }
    if (object == this && unknown_objects_a40c60.Count() == 1 && object->unknown_38 != 0 && object->unknown_38->unknown_28->Is_Player_Control()) {
        if (object->unknown_38->Unknown_00435FF0() == 1 && Rules->CanDetonateDeathBomb) {
            return 0x37;
        }
        if (object->unknown_38->Unknown_00435FF0() == 0 && Rules->CanDetonateTimeBomb) {
            return 0x37;
        }
    }
    bool third = !arg2 && (Techno_Key_Down(unknown_key_a40ba4) || Techno_Key_Down(unknown_key_a40ba8));
    bool force_fire = !arg2 && (Techno_Key_Down(KeyForceFire1) || Techno_Key_Down(KeyForceFire2));
    if (!arg2 && (Techno_Key_Down(KeyForceMove1) || Techno_Key_Down(KeyForceMove2))) {
        if (force_fire) {
            force_fire = false;
        } else if (House->Is_Player_Control() && !unknown_32c) {
            return 8;
        }
    }
    if (object == this && unknown_objects_a40c60.Count() == 1 && House->Is_Player_Control()) {
        return 4;
    }
    if (object == this && unknown_objects_a40c60.Count() == 1 && House->Is_Ally(unknown_player_a35db4) && unknown_game_mode_a3d298 == 0) {
        if (this != 0) {
            if (Kind_Of() == 1 && ((UnitClass *)this)->Type->Passengers > 0 && Cargo.Count > 0) {
                return 4;
            }
            if (Kind_Of() == 2 && ((AircraftClass *)this)->unknown_5b1) {
                return 4;
            }
        }
    }
    if (House->Is_Player_Control() && House->Is_Ally((TechnoClass *)object) && Kind_Of() != 2 && force_fire) {
        if (!third) {
            goto attack;
        }
        if (Can_Player_Move()) {
            return 0x1A;
        }
    }
    if (third && House->Is_Player_Control() && Can_Player_Move()) {
        return 1;
    }
attack:
    {
        TechnoTypeClass *type = Techno_Type_Class();
        int weapon = unknown_slot174(object);
        if (object->Not_Underground()) {
            WeaponTypeClass *weapon_type = unknown_slot231(weapon)->WeaponType;
            bool disguised = Rules->AttackCursorOnDisguise
                || (object->AbstractFlags.IsTechno && (object->Techno_Type_Class()->PermaDisguise
                    || (object->AbstractFlags.IsTechno && ((TechnoClass *)object)->unknown_184.Value() != 0)));
            bool owned = object->unknown_slot49(House) && !disguised;
            bool capture = owned && object->unknown_slot51(true) == 0;
            bool can208 = unknown_slot208(object);
            bool usable = !owned || (capture && can208);
            bool ivan = weapon_type != 0 && weapon_type->Warhead->IvanBomb && object->Techno_Type_Class() != 0
                && object->Techno_Type_Class()->Passengers > 0;
            bool foot = false;
            if (weapon_type != 0 && weapon_type->Warhead->Sonic) {
                foot = object->AbstractFlags.IsFoot;
            }
            bool sonic = foot && ((FootClass *)object)->unknown_580 != 0;
            bool friendly = Techno_Type_Class()->AttackFriendlies || (Techno_Type_Class()->AttackCursorOnFriendlies && !ivan) || sonic;
            bool enemy = !House->Is_Ally((TechnoClass *)object) || friendly;
            bool owned_friendly = owned && friendly;
            if (House->Is_Player_Control()) {
                if (force_fire || (enemy && (usable || owned_friendly) && (object->Class_Of()->unknown_201 || can208))) {
                    bool infiltrate = Kind_Of() == 15 && ((InfantryTypeClass *)type)->Infiltrate;
                    if ((unknown_slot163() || infiltrate) && (Can_Player_Move() || unknown_slot212(object, weapon))) {
                        bool special = false;
                        if (infiltrate) {
                            if ((!((InfantryClass *)this)->Type->Agent && object->Kind_Of() == 6 && ((BuildingClass *)object)->Type->Capturable)
                                || (((InfantryClass *)this)->Type->Agent && object->Kind_Of() == 6 && ((BuildingClass *)object)->Type->Spyable)
                                || (((InfantryClass *)this)->Type->C4 && object->Kind_Of() == 6 && ((BuildingClass *)object)->Type->CanC4)) {
                                special = true;
                            }
                        }
                        if (unknown_slot218(object, weapon, 1) != 5 || special || force_fire) {
                            return 5;
                        }
                    }
                }
            }
        }
    }
    if (unknown_slot163() && House->Is_Player_Control() && object->Owner() != Owner()) {
        return 0;
    }
    if (unknown_32c && unknown_372) {
        return 0;
    }
    if (!object->CanBeSelected() || object->unknown_77 || object->unknown_slot111()) {
        return 0;
    }
    return 7;
}


// int TechnoClass::unknown_slot27(Cell const *cell, bool arg2, bool arg3) (0x006CCCE0, slot 27)
// The action for a cell (Westwood's What_Action for a cell). Without arg3,
// the force-fire and force-move key pairs and the 0x00A40BA4/0x00A40BA8 pair
// are read; force fire and force
// move together cancel both. Slots 111/112 refuse (0). For a human-
// controlled owner: force fire with the third key (or +0x388) on a slot 39
// object answers 0x33 (or 0x1A without a waypoint in the cell); an armed
// object attacks (5) a cell its weapon can hit when forced, when the cell's
// overlay (or, with the 0x1000 scenario flag and arg2, a cell entry's) is
// attackable, when the cell burns, or when an enemy wall/parasite case
// allows it; then movement: 1 (move) or 2 (no move).

int TechnoClass::unknown_slot27(Cell const *cell, bool arg2, bool arg3)
{
    UnknownCellClass *cellptr = Map_Cell_At(*cell);
    UnknownOverlayType *overlay = 0;
    bool force_fire = !arg3 && (Techno_Key_Down(KeyForceFire1) || Techno_Key_Down(KeyForceFire2));
    bool force_move = !arg3 && (Techno_Key_Down(KeyForceMove1) || Techno_Key_Down(KeyForceMove2));
    bool third = !arg3 && (Techno_Key_Down(unknown_key_a40ba4) || Techno_Key_Down(unknown_key_a40ba8));
    if (force_move && force_fire) {
        force_move = false;
        force_fire = false;
    }
    int overlay_index = -1;
    bool entry_target = false;
    if (unknown_slot111() || unknown_slot112()) {
        return 0;
    }
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) && arg2 && cellptr->unknown_28 != 0) {
        for (int index = 0; index < cellptr->unknown_28->Count(); index++) {
            UnknownCellEntry28 *entry = (*cellptr->unknown_28)[index];
            if (entry->unknown_74) {
                if (entry->unknown_30 == 0x14) {
                    overlay_index = entry->unknown_24;
                } else if (entry->unknown_30 == 6
                    && (entry->unknown_28 == 0 || !unknown_player_a35db4->Is_Ally(entry->unknown_28))
                    && (Unknown_004C2030(entry) == 0 || Unknown_004C2030(entry)->unknown_201)) {
                    entry_target = true;
                }
            }
        }
    }
    if (overlay_index != -1) {
        overlay = unknown_overlay_types_a35de8[overlay_index];
    } else if (cellptr->unknown_44 != -1) {
        overlay = unknown_overlay_types_a35de8[cellptr->unknown_44];
    }
    bool can_deploy = unknown_slot193();
    if (House->Is_Player_Control() && ((force_fire && third) || unknown_388)
        && Can_Player_Move() && (Can_Player_Fire() || can_deploy)) {
        if (unknown_player_a35db4->Waypoint_At(*cell) == 0 && !unknown_388) {
            return 0x1A;
        }
        return 0x33;
    }
    WeaponStruct *primary;
    if (House->Is_Player_Control() && (primary = unknown_slot230()) != 0 && primary->WeaponType != 0) {
        int weapon = unknown_slot174((AbstractClass *)Map_Cell_At(*cell));
        bool burning = (Map_Cell_At(*cell))->Unknown_0047BB10();
        if (burning) {
            WeaponTypeClass *weapon_type = unknown_slot230()->WeaponType;
            if (weapon_type->Warhead->Fire) {
                burning = false;
            }
        }
        if (force_fire || (overlay != 0 && overlay->unknown_201) || burning || entry_target) {
            WarheadTypeClass *warhead = unknown_slot230()->WeaponType->Warhead;
            bool enemy = cellptr->unknown_50 != -1 ? !Houses[cellptr->unknown_50]->Is_Ally(House) : false;
            UnknownCellObject710160 *object = Unknown_00710160(*cell);
            bool occupied = object != 0 && !object->unknown_e8 && object->unknown_6c > 0;
            if (force_fire || overlay == 0 || occupied
                || (enemy && overlay->unknown_278 && (warhead->Wall || (warhead->Wood && overlay->unknown_9c == 6)))) {
                if (Can_Player_Move() || In_Range(CoordStruct(cell->X * 256 + 128, cell->Y * 256 + 128, 0), weapon)) {
                    return 5;
                }
            }
        }
        if (unknown_slot208((ObjectClass *)cellptr)
            && (Can_Player_Move() || In_Range(CoordStruct(cell->X * 256 + 128, cell->Y * 256 + 128, 0), weapon))) {
            return 5;
        }
    }
    if (House->Is_Player_Control() && (Can_Player_Move() || unknown_slot153())) {
        if (Map.Unknown_0055A660(*cell, true)) {
            if (force_move) {
                return 1;
            }
            if (unknown_slot153() && third) {
                return 1;
            }
            if (Can_Player_Move()) {
                if (Can_Enter_Cell(&Map[*cell], -1, -1, 0, 1) <= 1 || arg2) {
                    return 1;
                }
                if (Techno_Type_Class()->unknown_af5
                    && Can_Enter_Cell(&Map[*cell], -1, -1, 0, 0) <= 1) {
                    return 1;
                }
            }
        }
        return 2;
    }
    return 0;
}


// bool TechnoClass::Can_Player_Move() (0x006CD290, slot 39)
// Owner test, then slots 201/202, the type flag +0xB04 and slot 114.

bool TechnoClass::Can_Player_Move()
{
    if (!House->Is_Player_Control()) {
        return false;
    }
    if (unknown_slot201()) {
        return false;
    }
    if (Techno_Type_Class()->Spawned) {
        return false;
    }
    if (unknown_slot202()) {
        return false;
    }
    return unknown_slot114() ? true : false;
}


// bool TechnoClass::unknown_slot185() (0x006CD300, slot 185)

bool TechnoClass::unknown_slot185()
{
    if (this != 0) {
        if (Kind_Of() != 1) {
            return true;
        }
        FootClass *foot = (FootClass *)this;
        if ((foot->unknown_574 >= 0 && foot->unknown_slot284()) || unknown_slot201()) {
            return false;
        }
    }
    return true;
}


// Not exact (RA2_NON_MATCHING): TechnoClass::unknown_slot186 at 0x006CD340, best score 77.51, 2026-11-13; differs: 224 instructions in the original, 225 in the draft (shape 95.32)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x6CD340 TechnoClass::unknown_slot186 (747 B)
// entry 0x6CD340; exact-match score 81.96 (lane best), this text 81.96; plan "draft, verify"
// origin lane: Scout753 (build/class-work/Scout753/w/keep/s0_8196.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
// bool TechnoClass::unknown_slot186() (0x006CD340, slot 186)
// Whether the techno may deploy (or unload) where it stands.

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
            if (type->DeploysInto == 0 && type->Passengers == 0 && !type->IsSimpleDeployer) {
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
                if (((CellClass *)Get_Cell_Ptr())->Cell_Building()) {
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
            if (!unknown_slot201() && Techno_Type_Class()->Passengers != 0) {
                return true;
            }
            return false;
        }
    }
    return false;
}
#endif

// bool TechnoClass::Can_Player_Fire() (0x006CD630, slot 38)
// Owner test and an armed weapon slot 230, unless slot 201.

bool TechnoClass::Can_Player_Fire()
{
    if (House->Is_Player_Control()) {
        WeaponStruct *weapon = unknown_slot230();
        if (weapon && weapon->WeaponType && !unknown_slot201()) {
            return true;
        }
    }
    return false;
}


// bool TechnoClass::unknown_slot163() (0x006CD670, slot 163)
// Whether slot 230 has a weapon.

bool TechnoClass::unknown_slot163()
{
    WeaponStruct *weapon = unknown_slot230();
    return weapon != 0 && weapon->WeaponType != 0;
}


// bool TechnoClass::Can_Repair() (0x006CD690, slot 36)
// Kind 6 with type flag +0xAD8 and less than full strength.

bool TechnoClass::Can_Repair()
{
    if (Kind_Of() != 6) {
        return false;
    }
    return Techno_Type_Class()->Repairable && Strength != Class_Of()->MaxStrength;
}


// int TechnoClass::unknown_6cd6f0() (0x006CD6F0)
// The distance bonus of this object's height above the map's lowest level
// (0x0056A8A0), measured like 0x006C48F0: the levels divided by the rules'
// +0x1480, scaled by +0x1488 and limited to +0x1490, in leptons, combined
// with the levels times the geometry constant; 0 when slot 21 holds or the
// increment is 0. Never called (inlined everywhere).
int TechnoClass::unknown_6cd6f0()
{
    if (In_Air()) {
        return 0;
    }
    int increment = Rules->unknown_1480;
    if (increment == 0) {
        return 0;
    }
    double bonus = Rules->unknown_1488;
    Cell here = Coord_Cell(Center_Coord());
    UnknownCellClass *cell = (UnknownCellClass *)&Map[here];
    int lowest = Map.Unknown_0056A8A0();
    char level = cell->unknown_11b;
    double scaled = (level - lowest) / increment * bonus;
    double limit = Rules->unknown_1490;
    if (scaled >= limit) {
        scaled = Rules->unknown_1490;
    }
    int vertical = level * unknown_geometry_g;
    int horizontal = (int)scaled << 8;
    return (int)unknown_sqrt_4ba410((double)(horizontal * horizontal + vertical * vertical));
}


// int TechnoClass::Weapon_Range(int arg1) (0x006CD810, slot 85)
// The +0xB4 value of weapon arg1, or 0.

int TechnoClass::Weapon_Range(int arg1)
{
    WeaponTypeClass *weapon = unknown_slot231(arg1)->WeaponType;
    if (weapon) {
        return weapon->unknown_b4;
    }
    return 0;
}


// void TechnoClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination) (0x006CD840, slot 119)
// Remembers +0x228 in +0x22C, then the mission override and slot 220.

void TechnoClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination)
{
    unknown_22c = unknown_228;
    MissionClass::Override_Mission(mission, target, destination);
    unknown_slot220(target);
}


// bool TechnoClass::Restore_Mission() (0x006CD880, slot 120)
// Restores the suspended mission and the +0x22C value.

bool TechnoClass::Restore_Mission()
{
    if (MissionClass::Restore_Mission()) {
        unknown_slot220(unknown_22c);
        return true;
    }
    return false;
}


// void TechnoClass::unknown_slot233() (0x006CD8B0, slot 233)
// Slot 71 (2) and full strength (the type's MaxStrength into +0x6C and
// +0x70); buildings also get slot 97 (0), 0x0044D070 with whether the health
// is at or below the rules' +0x1368, and the rules' +0x1BC sound at their
// centre.

void TechnoClass::unknown_slot233()
{
    Mark(2);
    int strength = Techno_Type_Class()->MaxStrength;
    Strength = strength;
    unknown_70 = strength;
    if (Kind_Of() == 6) {
        Repair(0);
        ((BuildingClass *)this)->unknown_44d070(Health_Ratio() <= Rules->unknown_1368);
        Voc_Play_At(Rules->unknown_1bc, Center_Coord(), 0);
    }
}


// bool TechnoClass::unknown_slot222(HouseClass *house, int arg2) (0x006CD940, slot 222)
// Changes the owner (hypothesis: Westwood's Captured): nothing for the
// current owner; otherwise drops the target and destination, leaves the old
// owner's tracking (0x004ED440, 0x004EA800), credits the new owner with the
// type's cost (+0x5418) and a capture count by kind (+0x5364 buildings,
// +0x5310 units, aircraft and infantry), takes the cell occupation along,
// joins the new owner (0x004EA970, 0x004ED610) and, unless a weapons
// factory building is busy with mission 16 or has one as its first
// contact, resets its missions. Body 699 bytes: code to 0x006CDBDE, a
// three-entry jump table at 0x006CDBE0 and a 15-byte index table at
// 0x006CDBEC.

bool TechnoClass::unknown_slot222(HouseClass *house, int arg2)
{
    if (house != House) {
        unknown_slot220(0);
        Assign_Destination(0, true);
        unknown_6155e0();
        House->Unknown_004ED440(this, false);
        Record_The_Kill(0);
        house->unknown_5418 += Techno_Type_Class()->Cost_Of(House);
        House->unknown_4ea800(this);
        house->unknown_4ea970(this);
        switch (Kind_Of()) {
        case 6:
            house->unknown_5364[Owner()]++;
            break;
        case 1:
        case 2:
        case 15:
            house->unknown_5310[Owner()]++;
            break;
        }
        House->unknown_53b8 = house->Type->House;
        if (!unknown_76) {
            Mark(0);
        }
        Unknown_006D7C80(this);
        if (!unknown_76) {
            Mark(3);
        }
        UnknownCellClass *cell = 0;
        if (!unknown_76) {
            bool foot;
            if (this != 0) {
                foot = AbstractFlags.IsFoot;
            }
            bool moving = this != 0 && foot;
            if (!moving) {
                cell = (UnknownCellClass *)&Map[CoordStruct(Coord)];
            } else {
                cell = (UnknownCellClass *)&Map[((FootClass *)this)->unknown_454];
            }
            unknown_6d95d0(cell);
        }
        unknown_6cdc00(house);
        if (!unknown_76) {
            unknown_6d95a0(cell);
        }
        House->Unknown_004ED610(this, true);
        if (!unknown_76) {
            if (Kind_Of() != 6 || Mission != 16 || !((BuildingClass *)this)->Type->WeaponsFactory) {
                TechnoClass *contact;
                if (Contact(0) == 0 || (contact = Contact(0))->Kind_Of() != 6 || !((BuildingClass *)contact)->Type->WeaponsFactory) {
                    Assign_Destination(0, true);
                    unknown_slot220(0);
                    unknown_slot261(false, true);
                }
            }
            unknown_slot265();
            unknown_slot264();
        }
        return true;
    }
    return false;
}


// void TechnoClass::unknown_6cdc00(HouseClass *house) (0x006CDC00)
// Sets the owner; +0x372 is set exactly when it is the player's house.

void TechnoClass::unknown_6cdc00(HouseClass *house)
{
    House = house;
    unknown_372 = house == unknown_player_a35db4 && unknown_player_a35db4 != 0;
}


// Original smoke-vector RTTI79D0CC proves const ParticleSystemTypeClass pointees.
// Existing mutable ParticleSystem constructor contract is bridged explicitly, not retyped.
// Whole TechnoClass damage response, including the debris, particles and retaliation tail.
// Case 4 as one guarded block and separate `case 0: break; case 3: break;` labels follow the
// ra2-decomp Take_Damage shape (THIRD_PARTY.md); the separate labels give the original's case-1 table entry.

inline FootClass *As_Foot_L(TechnoClass *t) { return (t && t->AbstractFlags.IsFoot) ? static_cast<FootClass *>(t) : 0; }
inline bool Has_Ability_L(TechnoClass *t, int ability)
{
    if (t->unknown_118.Is_Veteran() || t->unknown_118.Is_Elite()) {
        TechnoTypeClass *type=t->Techno_Type_Class();
        if ((t->unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[ability]) ||
            (t->unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[ability] || type->EliteAbilities.Ability[ability]))) return true;
    }
    return false;
}
int TechnoClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *attacker, bool forced, int no_crew)
{
    bool healing=*damage<0;
    if (!forced && !healing) {
        double incoming=*damage;
        *damage=(int)(incoming/(House->unknown_func_4f58a0(Techno_Type_Class())*unknown_120));
        if (Has_Ability_L(this,1)) {
            *damage=(int)(*damage/Rules->VeteranArmor);
        }
        if (*damage<1) *damage=1;
        if (attacker && Techno_Type_Class()->TypeImmune) {
            TechnoTypeClass *attacker_type=attacker->Techno_Type_Class();
            if (Techno_Type_Class()==attacker_type && House==attacker->House) return 0;
        }
    }
    if (unknown_slot203() && !forced && !healing) {
        Unknown_0047F600(Coord,*damage*2,warhead,1,1);
        *damage=0;
        return 0;
    }
    if (unknown_slot111()) {
        *damage=0;
        return 0;
    }
    if (Techno_Type_Class()->DamageReducesReadiness) {
        double fraction=(double)*damage/Techno_Type_Class()->MaxStrength;
        double reduction=Techno_Type_Class()->ReadinessReductionMultiplier*fraction;
        double readiness=unknown_254;
        unknown_254=(int)(readiness-Techno_Type_Class()->Ammo*reduction);
        if (unknown_254<0) unknown_254=0;
        unknown_6c8300();
    }
    if (warhead) {
        if (warhead->Radiation && Techno_Type_Class()->ImmuneToRadiation) { *damage=0; return 0; }
        if (warhead->PsychicDamage && Techno_Type_Class()->ImmuneToPsionics) { *damage=0; return 0; }
    }
    int result=ObjectClass::Take_Damage(damage,distance,warhead,attacker,forced,no_crew);
    if (attacker) {
        TechnoTypeClass *type=Techno_Type_Class();
        double incoming=*damage;
        House->unknown_func_4eeba0((int)(type->Raw_Cost()*(incoming/Techno_Type_Class()->MaxStrength)),attacker->House);
    }
    if (result==4) {
        if (Kind_Of()==6 && warhead && warhead->CausesDelayKill) {
            BuildingClass *building=As_Building(this);
            if (building->Type->EligibleForDelayKill) {
                int delay=warhead->DelayKillFrames;
                int frames=(int)((double)delay+
                    ((warhead->DelayKillAtMax*(double)delay-delay)/(double)((int)warhead->CellSpread*256))*distance);
                if (!building->unknown_5af || frames<building->unknown_420.Value()) {
                    building->unknown_5af=true;
                    building->unknown_420=CDTimerClass<FrameTimerClass>(frames);
                }
                unknown_7d=true;
                Strength=1;
                result=5;
            }
        }
    }
    if (result==5) return 5;
    if (result!=0) {
        unknown_13c=CDTimerClass<FrameTimerClass>(Rules->RadarCombatFlashTime);
    }
    if (result!=0 && result!=4 && Techno_Type_Class()->CanDisguise && !Techno_Type_Class()->PermaDisguise) {
        if (unknown_slot48()) unknown_slot256();
        unknown_178=CDTimerClass<FrameTimerClass>(*damage*2);
    }
    if (Strength==0) result=4;
    switch (result) {
    case 4:
        {
            if (Techno_Type_Class()->VoiceDie.Count()>0 && House->Is_Player_Control()) {
                Voc_Play_At(Techno_Type_Class()->VoiceDie.Pick(unknown_random_839218()),Position_Coord_004f3f40(),0);
            }
            if (Techno_Type_Class()->DieSound.Count()>0) {
                Voc_Play_At(Techno_Type_Class()->DieSound.Pick(unknown_random_839218()),Position_Coord_004f3f40(),0);
            }
            SendToEachLink(3);
            unknown_slot210();
            if (unknown_25c[0]) { unknown_25c[0]->Delete_Me(); unknown_25c[0]=0; }
            if (!(Get_Height_AGL()<=10 && unknown_7c && Map[Position_Coord_004f3f40()].unknown_ec==2)) {
                if (Techno_Type_Class()->MaxDebris>0) {
                    int maximum=Techno_Type_Class()->MaxDebris-1;
                    int debris=Random_Pick(Techno_Type_Class()->MinDebris,maximum);
                    if (Techno_Type_Class()->DebrisTypes.Count()>0) {
                        int index=0;
                        while (debris>0) {
                            int pieces=abs(unknown_id_source_a3d290->RandomNumber)%(Techno_Type_Class()->DebrisMaximums[index]+1);
                            if (pieces>=debris) pieces=debris;
                            for (int n=pieces;n>0;--n) {
                                new VoxelAnimClass(Techno_Type_Class()->DebrisTypes[index],Center_Coord(),House);
                            }
                            debris-=pieces;
                            if (++index>=Techno_Type_Class()->DebrisTypes.Count()) index=0;
                        }
                    }
                    if (Techno_Type_Class()->DebrisAnims.Count()>0) {
                        for (int n=debris;n>0;--n) {
                            new AnimClass(Techno_Type_Class()->DebrisAnims[Random_Pick(0,Techno_Type_Class()->DebrisAnims.Count()-1)],Center_Coord()+CoordStruct(0,0,20),0,1,0x600,0);
                        }
                    } else if (Techno_Type_Class()->DebrisTypes.Count()==0) {
                        for (;debris>0;--debris) {
                            new AnimClass(Rules->MetallicDebris[Random_Pick(0,Rules->MetallicDebris.Count()-1)],Center_Coord()+CoordStruct(0,0,20),0,1,0x600,0);
                        }
                    }
                }
                WeaponTypeClass *weapon=unknown_slot231(unknown_10c)->WeaponType;
                if (Techno_Type_Class()->Explodes || Has_Ability_L(this,10) || (weapon && weapon->unknown_11c)) {
                    unknown_6d3040(attacker);
                    unknown_6d7e20(0);
                }
            }
        }
        if (unknown_38) unknown_38->Unknown_00435D30();
        break;
    case 2:
        {
            TechnoTypeClass *type=Techno_Type_Class();
            if (type->VoiceFeedback.Count()>0 && unknown_random_839218(0,99)<30 && House->Is_Player_Control()) {
                int selection=unknown_random_839218();
                int voice=type->VoiceFeedback.Pick(selection);
                Voc_Play_At(voice,Center_Coord(),0);
            }
        }
        break;
    case 0:
        break;
    case 3:
        break;
    default:
        if (Techno_Type_Class()->DamageSound!=-1) {
            Voc_Static_Add(Techno_Type_Class()->DamageSound,Position_Coord_004f3f40(),0);
        }
        if (Techno_Type_Class()->DamageSound!=-1) {
            Voc_Static_Add(Techno_Type_Class()->DamageSound,Position_Coord_004f3f40(),0);
        }
        if ((Techno_Type_Class()->ToProtect || unknown_327) && !House->Is_Human_Player() && attacker) unknown_6d3400(attacker);
        break;
    }
    if (result!=4) {
    if (attacker && !House->Is_Ally(attacker)) unknown_329=true;
    Do_Shimmer();
    if (Health_Ratio()>Rules->unknown_1368) {
        if (unknown_25c[3]) unknown_25c[3]->Delete_Me();
    } else if (result==2 || result==3) {
            DynamicVectorClass<ParticleSystemTypeClass const *> smoke_types;
            for (int index=Techno_Type_Class()->DamageParticleSystems.Count()-1;index>=0;--index) {
                if (Techno_Type_Class()->DamageParticleSystems[index]->BehavesLike==0) smoke_types.Add(Techno_Type_Class()->DamageParticleSystems[index]);
            }
            if (!unknown_25c[3] && smoke_types.Count()>0 && Get_Height_AGL()>-10) {
                CoordStruct offset=Techno_Type_Class()->Damage_Smoke_Offset();
                unknown_25c[3]=new ParticleSystemClass(const_cast<ParticleSystemTypeClass *>(smoke_types[Random_Pick(0,smoke_types.Count()-1)]),Position_Coord_004f3f40()+offset,0,this,unknown_geometry_l);
            }
    }
    if (!healing) {
    TechnoClass *target=attacker;
    if (unknown_6d3b60(attacker,(int)warhead)) {
        if (attacker==0 && Rules->unknown_1431) target=0;
        if (target) {
            int which=unknown_slot174(target);
            bool retaliate=unknown_slot212(target,which);
            if (!retaliate) {
                if (House->Is_Human_Player()) {
                    TPoint3D<int> difference=Center_Coord()-target->Center_Coord();
                    double range=(int)unknown_sqrt_4ba410((double)difference.X*(double)difference.X+(double)difference.Z*(double)difference.Z+(double)difference.Y*(double)difference.Y);
                    retaliate=range<=(Techno_Type_Class()->Sight+0.5)*256.0;
                } else {
                    retaliate=true;
                }
            }
            if (retaliate) {
                Override_Mission(1,target,0);
            }
        }
        FootClass *foot=As_Foot_L(this);
        if (foot) {
            if (!unknown_228 && !foot->unknown_494) {
                if (Rules->unknown_1431 || Has_Ability_L(this,3)) {
                    Scatter(unknown_geometry_l,true,false);
                }
            }
        }
    } else {
        FootClass *foot=As_Foot_L(this);
        if (foot) {
            if (Control()->unknown_09) {
                if (!unknown_370) {
                    if (!foot->Locomotor.Checked()->Is_Moving() && !unknown_228 && !foot->unknown_494 && Kind_Of()!=2) {
                        if (!House->Is_Human_Player() || Rules->unknown_1431 || Has_Ability_L(this,3)) {
                            Scatter(unknown_geometry_l,true,false);
                        }
                    }
                }
            }
        }
    }
    }
    }
    return result;
}


// void TechnoClass::Record_The_Kill(TechnoClass *killer) (0x006CED90, slot 55)
// Records the object's destruction by the techno killer (may be null): springs
// its tag (events 6 and 4 with a killer; for non-units 7 with a killer, 0x30
// and 0x1D), gives the killer (and its +0x23C parent when the killer's type
// has +0xB18) experience for the object's cost (doubled for veterans,
// tripled for elites, none for an ally), notes the killer's owner on the
// owner (+0x53B8) and adds the points to the killer's owner (+0x5418); then
// counts the loss by kind in the killer owner's trackers and per-house
// tables and asks for a radar redraw.

void TechnoClass::Record_The_Kill(TechnoClass *killer)
{
    int found = 0;
    int points = Techno_Type_Class()->Cost_Of(House);
    if (unknown_7d && unknown_34 != 0 && killer != 0) {
        unknown_34->Spring(6, this, unknown_cell_ac0148, 0, 0);
    }
    if (unknown_7d) {
        if (unknown_34 != 0 && killer != 0) {
            unknown_34->Spring(4, this, unknown_cell_ac0148, 0, 0);
        }
        if (unknown_7d && Kind_Of() != 1) {
            if (unknown_7d && unknown_34 != 0 && killer != 0) {
                unknown_34->Spring(7, this, unknown_cell_ac0148, 0, 0);
            }
            if (unknown_7d && unknown_34 != 0) {
                unknown_34->Spring(0x30, this, unknown_cell_ac0148, 0, 0);
            }
            if (unknown_7d && unknown_34 != 0) {
                unknown_34->Spring(0x1D, this, unknown_cell_ac0148, 0, 0);
            }
        }
    }
    if (killer != 0) {
        if (killer->House->Is_Ally(this)) {
            points = 0;
        } else if (unknown_118.Is_Veteran()) {
            points += points;
        } else if (unknown_118.Is_Elite()) {
            points *= 3;
        }
        if (killer->Techno_Type_Class()->Trainable) {
            killer->unknown_118.Made_A_Kill(killer->Techno_Type_Class()->Cost_Of(House), points);
        }
        if (killer->Techno_Type_Class()->MissileSpawn && killer->unknown_23c != 0
            && ((TechnoClass *)killer->unknown_23c)->Techno_Type_Class()->Trainable) {
            ((TechnoClass *)killer->unknown_23c)->unknown_118.Made_A_Kill(
                ((TechnoClass *)killer->unknown_23c)->Techno_Type_Class()->Cost_Of(House), points);
        }
        House->unknown_53b8 = killer->Owner();
        killer->House->unknown_5418 += points;
    }
    switch (Kind_Of()) {
    case 6:
        if (Techno_Type_Class()->unknown_202) {
            break;
        }
        if (((BuildingClass *)this)->unknown_434 != -1) {
            House->unknown_53b4++;
        }
        if (killer != 0) {
            killer->House->unknown_3aa0.Unknown_0070C490(((BuildingClass *)this)->Type->ArrayIndex);
            killer->House->unknown_5364[Owner()]++;
        }
        if (Map.Is_Player_Names()) {
            Map.Redraw_Radar(false);
        }
        break;
    case 2:
        if (killer != 0) {
            killer->House->unknown_2288.Unknown_0070C490(((AircraftClass *)this)->Type->ArrayIndex);
            found = 1;
        }
    case 15:
        if (killer != 0 && !found) {
            killer->House->unknown_2a90.Unknown_0070C490(((InfantryClass *)this)->Type->ArrayIndex);
            found = 1;
        }
    case 1:
        if (killer != 0 && !found) {
            killer->House->unknown_3298.Unknown_0070C490(((UnitClass *)this)->Type->ArrayIndex);
        }
        House->unknown_5360++;
        if (killer != 0) {
            killer->House->unknown_5310[Owner()]++;
        }
        if (Map.Is_Player_Names()) {
            Map.Redraw_Radar(false);
        }
        break;
    }
}


// Find a nearby reachable cell using this object's speed and movement zone.
Cell TechnoClass::unknown_6cf130(AbstractClass *target)
{
    int speed=Techno_Type_Class()->SpeedType;
    if(speed==4) speed=1;
    Cell cell;
    if(target) cell=Cell(target->Center_Coord());
    else cell=Cell(Center_Coord());
    int movement_zone=Techno_Type_Class()->MovementZone;
    int zone=movement_zone!=-1 ? Map.Zone(cell,movement_zone,unknown_79) : -1;
    return Map.Nearby_Location(cell,speed,zone,movement_zone,unknown_79,Point2D(1,1),0,0,0,1,Cell(0,0),0);
}


// void TechnoClass::unknown_slot252(bool quiet) (0x006CF250, slot 252)
// Starts uncloaking from cloaked (2) or cloaking (1): state 3 counting the
// stage down from the last at the type's CloakingSpeed, with the rules'
// cloak sound unless quiet.

void TechnoClass::unknown_slot252(bool quiet)
{
    if (unknown_1b8 == 2 || unknown_1b8 == 1) {
        unknown_1b8 = 3;
        unknown_1bc.Value = Rules->unknown_548 - 1;
        unknown_1bc.Set_Rate(Techno_Type_Class()->CloakingSpeed);
        unknown_1bc.unknown_14 = -1;
        if (!quiet) {
            Voc_Play_At(Rules->unknown_5c0, CoordStruct(Coord), 0);
        }
    }
}


// void TechnoClass::unknown_slot253(bool quiet) (0x006CF300, slot 253)
// Starts cloaking from visible (0) or uncloaking (3): slot 54 first, then
// state 1 counting the stage up from 0 at the type's CloakingSpeed, with
// the rules' cloak sound unless quiet.

void TechnoClass::unknown_slot253(bool quiet)
{
    if (unknown_1b8 == 0 || unknown_1b8 == 3) {
        unknown_slot54(false);
        unknown_1b8 = 1;
        unknown_1bc.Value = 0;
        unknown_1bc.Set_Rate(Techno_Type_Class()->CloakingSpeed);
        unknown_1bc.unknown_14 = 1;
        if (!quiet) {
            Voc_Play_At(Rules->unknown_5c0, CoordStruct(Coord), 0);
        }
    }
}


// void TechnoClass::Do_Shimmer() (0x006CF3B0, slot 61)
// Slot 252 with 0.

void TechnoClass::Do_Shimmer()
{
    unknown_slot252(0);
}


// int TechnoClass::unknown_slot25(bool arg1, int arg2) (0x006CF3C0, slot 25)
// How the object is drawn (hypothesis: Visual_Character; 0 normal,
// 1 indistinct, 2 darken, 3 shadowy, 4 ripple, 5 hidden): an Invisible type
// is hidden unless +0x372 or the 0x00A40D13 flag; a cloaked object (+0x1B8
// state 2) is shadowy where its cell is revealed to the house (arg2, a
// HouseClass pointer, when arg1 is set; else the player, or mutual allies in
// a multiplayer mode) and hidden otherwise; while cloaking or uncloaking the
// cloak stage scaled to 256 picks 1-5.

int TechnoClass::unknown_slot25(bool arg1, int arg2)
{
    if (Techno_Type_Class()->Invisible && unknown_372) {
        return 0;
    }
    if (Techno_Type_Class()->Invisible && !unknown_372 && !Debug_Map) {
        return 5;
    }
    if (unknown_1b8 == 0 || Debug_Map) {
        return 0;
    }
    if (Kind_Of() == 6) {
        return 0;
    }
    if (unknown_1b8 == 2) {
        if (arg1) {
            HouseClass *house = (HouseClass *)arg2;
            if (house != 0) {
                Cell cell = Coord_Cell(CoordStruct(Coord));
                if (((UnknownCellClass *)&Map[cell])->Is_Sensed(house->ArrayIndex)) {
                    return 3;
                }
            }
        } else {
            if (unknown_b263d8 == 0) {
                return 3;
            }
            if (unknown_372) {
                return 3;
            }
            Cell cell = Coord_Cell(CoordStruct(Coord));
            if (((UnknownCellClass *)&Map[cell])->Is_Sensed(unknown_player_a35db4->ArrayIndex)) {
                return 3;
            }
            if (unknown_game_mode_a3d298 != 0 && House != 0 && unknown_player_a35db4 != 0
                && unknown_player_a35db4->Is_Ally(House) && House->Is_Ally(unknown_player_a35db4)) {
                return 3;
            }
        }
        return 5;
    }
    int stage = unknown_1bc.Value;
    if (stage > 0) {
        int level = (int)((double)stage / Rules->unknown_548 * 256.0);
        if (level < 64) {
            return 1;
        }
        if (level < 128) {
            return 2;
        }
        if (level < 192) {
            return 3;
        }
        if (!arg1 && unknown_372) {
            return 3;
        }
        return level >= 255 ? 5 : 4;
    }
    return 0;
}


// bool TechnoClass::unknown_6cf670() (0x006CF670)
// Whether the object (off a bridge, +0x79) stands where a bridge cell (flag
// 0x100) is under it or next to it: its own cell, the south and west
// neighbours when they carry flag 0x800 too, the east and north ones when
// they do not.

extern Cell const AdjacentCell[8];

bool TechnoClass::unknown_6cf670()
{
    Cell position = unknown_slot104();
    UnknownCellClass *here = (UnknownCellClass *)&Map[position];
    if (here != 0 && !unknown_79) {
        UnknownCellClass *south = (UnknownCellClass *)&Map[(AdjacentCell[4] + position)];
        UnknownCellClass *north = (UnknownCellClass *)&Map[(AdjacentCell[0] + position)];
        UnknownCellClass *east = (UnknownCellClass *)&Map[(AdjacentCell[2] + position)];
        UnknownCellClass *west = (UnknownCellClass *)&Map[(AdjacentCell[6] + position)];
        if ((here->Flags & 0x100) != 0
            || (south != 0 && (south->Flags & 0x100) != 0 && (south->Flags & 0x800) != 0)
            || (west != 0 && (west->Flags & 0x100) != 0 && (west->Flags & 0x800) == 0)
            || (east != 0 && (east->Flags & 0x100) != 0 && (east->Flags & 0x800) == 0)
            || (north != 0 && (north->Flags & 0x100) != 0 && (north->Flags & 0x800) != 0)) {
            return true;
        }
    }
    return false;
}


// bool TechnoClass::unknown_6cf820() (0x006CF820)
// The same test as 0x006CF670 with flag 0x400 in place of 0x100 (a second
// kind of bridge cell; hypothesis): whether the object (off a bridge, +0x79)
// stands on or next to such a cell.

extern Cell const AdjacentCell[8];

bool TechnoClass::unknown_6cf820()
{
    Cell position = unknown_slot104();
    UnknownCellClass *here = (UnknownCellClass *)&Map[position];
    if (here != 0 && !unknown_79) {
        UnknownCellClass *south = (UnknownCellClass *)&Map[(AdjacentCell[4] + position)];
        UnknownCellClass *north = (UnknownCellClass *)&Map[(AdjacentCell[0] + position)];
        UnknownCellClass *east = (UnknownCellClass *)&Map[(AdjacentCell[2] + position)];
        UnknownCellClass *west = (UnknownCellClass *)&Map[(AdjacentCell[6] + position)];
        if ((here->Flags & 0x400) != 0
            || (south != 0 && (south->Flags & 0x400) != 0 && (south->Flags & 0x800) != 0)
            || (west != 0 && (west->Flags & 0x400) != 0 && (west->Flags & 0x800) == 0)
            || (east != 0 && (east->Flags & 0x400) != 0 && (east->Flags & 0x800) == 0)
            || (north != 0 && (north->Flags & 0x400) != 0 && (north->Flags & 0x800) != 0)) {
            return true;
        }
    }
    return false;
}


// int TechnoClass::unknown_6cf9d0() (0x006CF9D0)
// How many of the three neighbouring cells (facings 4, 2 and 3) hold one
// of the cliff or column tiles (tile set 0x00A52C74, positions 7-16); 0
// unless a bridge applies under the object (0x006CF670 or 0x006CF820).

extern Cell const AdjacentCell[8];

extern int unknown_tileset_a52c74;

int TechnoClass::unknown_6cf9d0()
{
    Cell position = unknown_slot104();
    int count = 0;
    UnknownCellClass *here = (UnknownCellClass *)&Map[position];
    if (here != 0 && (unknown_6cf670() || unknown_6cf820())) {
        UnknownCellClass *south = (UnknownCellClass *)&Map[(AdjacentCell[4] + position)];
        UnknownCellClass *east = (UnknownCellClass *)&Map[(AdjacentCell[2] + position)];
        UnknownCellClass *west = (UnknownCellClass *)&Map[(AdjacentCell[3] + position)];
        if (south != 0 && east != 0 && west != 0) {
            int tile = south->unknown_38;
            if (tile != 0xFFFF && tile != 0xFF) {
                if (tile - unknown_tileset_a52c74 + 1 >= 7 && tile - unknown_tileset_a52c74 + 1 <= 16) {
                    count = 1;
                }
            }
            tile = east->unknown_38;
            if (tile != 0xFFFF && tile != 0xFF) {
                if (tile - unknown_tileset_a52c74 + 1 >= 7 && tile - unknown_tileset_a52c74 + 1 <= 16) {
                    count = 1;
                }
            }
            tile = west->unknown_38;
            if (tile != 0xFFFF && tile != 0xFF) {
                if (tile - unknown_tileset_a52c74 + 1 >= 7 && tile - unknown_tileset_a52c74 + 1 <= 16) {
                    count = count + 1;
                }
            }
        }
    }
    return count;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Shape adopted from ra2-decomp (gibbo101/ra2-decomp ca9b8ad, src/techno.cpp TechnoClass::Get_Z_Fudge_Tunnel,
// YR 0x00704000, and include/inline.h's Dir_Facing/Adjacent_Cell): Copyright 2026 ra2-decomp contributors;
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors. Derived from Electronic Arts source code;
// EA's GPL Section 7 terms apply.
// int TechnoClass::unknown_6cfb60() (0x006CFB60)
// Whether a tunnel entrance lies two cells to the north or west of the
// object (1) or not (0), for the drawing depth (times ZFudgeTunnel).


extern Cell const AdjacentCell[8];

// TibSun inline.h's Dir_Facing and the Dir256 Adjacent_Cell (the cell is the sum's left operand).
inline FacingType Dir_Facing(DirType facing) { return facing.As_Facing(); }
inline FacingType Dir_Facing(Dir256 facing) { return Dir_Facing(DirType(facing)); }
inline Cell Adjacent_Cell(Cell const &cell, Dir256 dir) { return cell + AdjacentCell[Dir_Facing(dir)]; }

int TechnoClass::unknown_6cfb60()
{
    int fudge = 0;
    Cell cell = unknown_slot104();
    if (!unknown_79) {
        CellClass *north = &Map[Adjacent_Cell(cell, DIR_N)];
        CellClass *west = &Map[Adjacent_Cell(cell, DIR_W)];
        Cell tuncell = unknown_cell_ac0148;
        CellClass *own = &Map[cell];
        if (north != 0 && west != 0) {
            if (own->Has_Tunnel()) {
                tuncell = own->Fetch_CellID();
            } else if (north->Has_Tunnel()) {
                tuncell = north->Fetch_CellID();
            } else if (west->Has_Tunnel()) {
                tuncell = west->Fetch_CellID();
            }
            if (tuncell != unknown_cell_ac0148) {
                north = &Map[Adjacent_Cell(tuncell, DIR_N)];
                west = &Map[Adjacent_Cell(tuncell, DIR_W)];
                if (north != 0 && west != 0) {
                    north = &Map[Adjacent_Cell(north->Fetch_CellID(), DIR_N)];
                    west = &Map[Adjacent_Cell(west->Fetch_CellID(), DIR_W)];
                    if (north != 0 && west != 0) {
                        if (west->Has_Tunnel() || north->Has_Tunnel()) {
                            fudge = 1;
                        }
                    }
                }
            }
        }
    }
    return fudge;
}


// int TechnoClass::unknown_6cfda0() (0x006CFDA0)
// How a cliff next to the object raises its drawing depth: 2 when the cell
// to the facing-3 side is more than three levels above this one, 1 when
// the cell beyond that is, 0 when the object is on a bridge (+0x79) or
// neither is.

extern Cell const AdjacentCell[8];

int TechnoClass::unknown_6cfda0()
{
    int result = 0;
    Cell position = unknown_slot104();
    UnknownCellClass *here = (UnknownCellClass *)&Map[position];
    if (!unknown_79) {
        UnknownCellClass *next = (UnknownCellClass *)&Map[(AdjacentCell[3] + unknown_slot104())];
        if (next != 0) {
            if (next->unknown_11b - here->unknown_11b >= 4) {
                result = 2;
            }
            Cell cell = next->unknown_24;
            next = (UnknownCellClass *)&Map[(AdjacentCell[3] + cell)];
            if (next != 0 && next->unknown_11b - here->unknown_11b >= 4) {
                return 1;
            }
        }
    }
    return result;
}


// int TechnoClass::unknown_slot176() (0x006CFEA0)
// TibSun TechnoClass::Get_Z_Adjust: the drawing depth adjustment from height, ramps and rocks.
// Shape from the TibSun reconstruction (techno.cpp Get_Z_Adjust, inline.h Adjacent_Cell, facing.h
// Facing_Add/Facing_Sub; 2026 OpenTS contributors, GPL-3.0-or-later, see THIRD_PARTY.md). The YR twin
// (gamemd 0x00704350) compiles to the same instructions.
extern int unknown_techno_zadj_ramp1_7f7124;
extern int unknown_techno_zadj_ramp2_7f7128;
extern int unknown_techno_zadj_rock_7f712c;
extern Cell const AdjacentCell[8];
// The sum or difference of two eight-way facings (TibSun facing.h).
inline FacingType Facing_Add(int first, int second) { return (FacingType)((first + second) & 7); }
inline FacingType Facing_Sub(int first, int second) { return (FacingType)((first - second) & 7); }
// Whether an overlay is a rock (the three rock tests by the ramp).
inline bool Overlay_Is_Rock(int overlay)
{
    OverlayTypeClass *otype = OverlayTypes[overlay];
    return otype->IsARock;
}
int TechnoClass::unknown_slot176()
{
    CellClass *cptrs[3];
    int width;
    int height;
    int z = -Unknown_006A2970(Get_Height());
    int zadjust2 = z;
    int zadjust = z;
    Cell cell = unknown_slot104();
    CellClass *cptr = (CellClass *)&Map[cell];
    UnitClass *unit = (UnitClass *)this;
    if (unit->Kind_Of() == 1) {
        if (unit->unknown_370) {
            BuildingClass *building = (BuildingClass *)unit->Contact_With_Whom(0);
            if (building->Kind_Of() == 6 && building->Get_Mission() == 16) {
                return zadjust2 - 3;
            }
        }
        if (unit->Type->Harvester) {
            BuildingClass *building = (BuildingClass *)((CellClass *)Get_Cell_Ptr())->Cell_Building();
            if (building != 0 && building->Type->Refinery) {
                return zadjust2 - 14;
            }
        }
    }
    if (cptr->unknown_11c) {
        CellClass *cell_s = (CellClass *)&Map[Adjacent_Cell(cell, FACING_S)];
        CellClass *cell_e = (CellClass *)&Map[Adjacent_Cell(cell, FACING_E)];
        CellClass *cell_se = (CellClass *)&Map[Adjacent_Cell(cell, FACING_SE)];
        if (cell_s != 0 && cell_e != 0 && cell_se != 0) {
            bool rock = false;
            if (cell_s->OverlayIndex != -1) {
                if (Overlay_Is_Rock(cell_s->OverlayIndex)) {
                    rock = true;
                }
            } else if (cell_e->OverlayIndex != -1) {
                if (Overlay_Is_Rock(cell_e->OverlayIndex)) {
                    rock = true;
                }
            } else if (cell_se->OverlayIndex != -1) {
                if (Overlay_Is_Rock(cell_se->OverlayIndex)) {
                    rock = true;
                }
            }
            if (rock) {
                return unknown_techno_zadj_rock_7f712c + zadjust2 - 1;
            }
        }
    }
    CellClass *ahead_right = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Add(Dir_To_8(unknown_2e0.Current()), 1))];
    CellClass *ahead_left = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Sub(Dir_To_8(unknown_2e0.Current()), 1))];
    CellClass *ahead = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Add(Dir_To_8(unknown_2e0.Current()), 0))];
    CellClass *behind = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Sub(Dir_To_8(unknown_2e0.Current()), 4))];
    CellClass *behind_right = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Add(Dir_To_8(unknown_2e0.Current()), 3))];
    CellClass *behind_left = (CellClass *)&Map[Adjacent_Cell(cell, Facing_Sub(Dir_To_8(unknown_2e0.Current()), 3))];
    bool ramp = false;
    if (ahead_right != 0 && ahead_left != 0 && ahead != 0) {
        if (ahead_right->unknown_11c == 0 && ahead_left->unknown_11c == 0 && ahead->unknown_11c == 0) {
            if (behind != 0 && behind_right != 0 && behind_left != 0) {
                if (behind->unknown_11c == 0 && behind_right->unknown_11c == 0 && behind_left->unknown_11c == 0) {
                    if (cptr->unknown_11c) {
                        return zadjust + unknown_techno_zadj_ramp1_7f7124;
                    }
                    if (Dir_To_8(unknown_2e0.Current()) == 6 || Dir_To_8(unknown_2e0.Current()) == 0) {
                        cptrs[0] = behind;
                        cptrs[1] = behind_right;
                        cptrs[2] = behind_left;
                    } else if (Dir_To_8(unknown_2e0.Current()) == 4 || Dir_To_8(unknown_2e0.Current()) == 2) {
                        cptrs[0] = ahead_right;
                        cptrs[1] = ahead_left;
                        cptrs[2] = ahead;
                    } else {
                        return zadjust - 1;
                    }
                    if ((cptr->unknown_140 & 0x10000) != 0) {
                        return zadjust - 1;
                    }
                    for (int i = 0; i < 3; i++) {
                        if ((cptrs[i]->unknown_140 & 0x10000) != 0) {
                            return zadjust - 1;
                        }
                    }
                    for (int j = 0; j < 3; j++) {
                        int ittype = cptrs[j]->IsoTileIndex;
                        int subtile;
                        IsometricTileTypeClass *iptr;
                        if (ittype == 0xFFFF || ittype == 0xFF) {
                            iptr = IsoTileTypes[unknown_tileset_a52f00];
                            subtile = 0;
                        } else {
                            iptr = IsoTileTypes[ittype];
                            subtile = (unsigned char)cptrs[j]->unknown_11a;
                        }
                        iptr->Get_Tile_Pixel_Dimensions(subtile, width, height);
                        if (height > 36) {
                            return zadjust - 2;
                        }
                    }
                    return zadjust - 1;
                } else {
                    ramp = true;
                }
            }
        } else {
            ramp = true;
        }
    }
    if (ramp) {
        return unknown_techno_zadj_ramp2_7f7128 + zadjust;
    }
    return zadjust - 1;
}


// TechnoClass 0x006D0510.
extern bool const unknown_dash_7f7130[];
// TibSun's TRect::operator+ with a corner: the rectangle moved by a point.
inline Rect Moved_Rect(Rect const &rect, Point2D const &point)
{
    return Rect(rect.Top_Left()+(RectPoint2D const &)point,rect.Width,rect.Height);
}
void TechnoClass::unknown_6d0510(CoordStruct from, CoordStruct to, RGBClass color, int arg8, int arg9)
{
    Point2D start;
    Point2D end;
    unknown_tactical_83996c->Coord_To_Pixel(from,start);
    unknown_tactical_83996c->Coord_To_Pixel(to,end);
    start.Y+=unknown_rect_839630.Y;
    end.Y+=unknown_rect_839630.Y;
    Point2D shadow_start=start;
    shadow_start.Y+=1;
    Point2D shadow_end=end;
    shadow_end.Y+=1;
    char shadow=(char)arg9;
    if (shadow) {
        int black=unknown_drawer_8323c4->Convert(0);
        unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Moved_Rect(Rect(-2,-2,3,3),shadow_start)),black);
        black=unknown_drawer_8323c4->Convert(0);
        unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Moved_Rect(Rect(-2,-2,3,3),shadow_end)),black);
    }
    Rect clip=unknown_rect_839630;
    if (shadow && Clip_Line(shadow_start,shadow_end,clip)) {
        unknown_surface_839964->Draw_Line(shadow_start,shadow_end,unknown_drawer_8323c4->Convert(0));
    }
    int pixel=((color.Red>>unknown_shift_852fbc)<<unknown_shift_852fb8)|((color.Blue>>unknown_shift_852fc4)<<unknown_shift_852fc0)|((color.Green>>unknown_shift_852fcc)<<unknown_shift_852fc8);
    unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Moved_Rect(Rect(-2,-2,3,3),start)),pixel);
    unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Moved_Rect(Rect(-2,-2,3,3),end)),pixel);
    if ((char)arg8) {
        int offset=(0x7fffffff-Frame)%15;
        if (Clip_Line(start,end,clip)) {
            unknown_surface_839964->Draw_Dashed_Line_Depth_Masked(start,end,pixel,unknown_dash_7f7130,offset,false);
        }
    } else {
        if (Clip_Line(start,end,clip)) {
            unknown_surface_839964->Draw_Line(start,end,pixel);
        }
    }
}


// TechnoClass 0x006D0DC0: a line from `from` to `to` in 64 steps, shaded from the 0x00AB6841 colour at
// `from` to `color` at `to`, with end markers and an optional one-pixel-lower shadow (never called).
// Moved_Rect comes from the 0x006D0510 source earlier in this unit.
// TibSun DSurface::Build_Hicolor_Pixel(RGBClass const &): the three-channel builder on a colour.
static inline int Build_Hicolor_Pixel(RGBClass const &rgb)
{
    return ((rgb.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
         | ((rgb.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
         | ((rgb.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
}
// The 3x3 end marker of a line, its corner two pixels up and left of the point.
static inline Rect Line_End_Rect(Point2D const &point)
{
    Point2D offset=point-Point2D(2,2);
    return Rect(RectPoint2D(offset.X,offset.Y),3,3);
}
void TechnoClass::unknown_6d0dc0(CoordStruct from, CoordStruct to, RGBClass color, bool shadow)
{
    RGBClass shade;
    Point2D start;
    Point2D end;
    unknown_tactical_83996c->Coord_To_Pixel(from,start);
    unknown_tactical_83996c->Coord_To_Pixel(to,end);
    start.Y+=unknown_rect_839630.Y;
    end.Y+=unknown_rect_839630.Y;
    int pixel=Build_Hicolor_Pixel(color);
    int fade=Build_Hicolor_Pixel(unknown_rgb_ab6841);
    Point2D shadow_start=start;
    shadow_start.Y+=1;
    Point2D shadow_end=end;
    shadow_end.Y+=1;
    if (shadow) {
        unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Moved_Rect(Rect(-2,-2,3,3),shadow_start)),pixel);
        unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Moved_Rect(Rect(-2,-2,3,3),shadow_end)),fade);
    }
    Rect clip=unknown_rect_839630;
    if (shadow && Clip_Line(shadow_start,shadow_end,clip)) {
        unknown_surface_839964->Draw_Line(shadow_start,shadow_end,unknown_drawer_8323c4->Convert(0));
    }
    unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Line_End_Rect(start)),fade);
    unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Line_End_Rect(end)),pixel);
    CoordStruct last=from;
    CoordStruct next;
    int i=1;
    int j=63;
    while (i<=64) {
        next.X=(from.X*j+to.X*i)/64;
        next.Y=(from.Y*j+to.Y*i)/64;
        next.Z=(from.Z*j+to.Z*i)/64;
        unknown_tactical_83996c->Coord_To_Pixel(last,start);
        unknown_tactical_83996c->Coord_To_Pixel(next,end);
        start.Y+=unknown_rect_839630.Y;
        end.Y+=unknown_rect_839630.Y;
        if (Clip_Line(start,end,clip)) {
            shade=color;
            int ratio=j*4;
            shade.Adjust(ratio<0xfe?ratio:0xfe,unknown_rgb_ab6841);
            unknown_surface_839964->Draw_Line(start,end,Build_Hicolor_Pixel(shade));
        }
        last=next;
        i++;
        j--;
    }
}


// Not exact (RA2_NON_MATCHING): TechnoClass::unknown_6d0990 at 0x006D0990, best score 86.96, 2026-11-13; differs: 288 instructions in the original, 287 in the draft (shape 93.91)
#ifdef RA2_NON_MATCHING
// TechnoClass 0x006D0990.
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();
static inline void Fill_Line_End_Rect(Point2D const &point, int pixel)
{
    unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Line_End_Rect(point)),pixel);
}
void TechnoClass::unknown_6d0990(CoordStruct from, CoordStruct to, RGBClass color)
{
    RGBClass shade;
    RGBClass white(255,255,255);
    Point2D start;
    Point2D end;
    CoordStruct next;
    {
        unknown_tactical_83996c->Coord_To_Pixel(from,start);
        unknown_tactical_83996c->Coord_To_Pixel(to,end);
        start.Y+=unknown_rect_839630.Y;
        end.Y+=unknown_rect_839630.Y;
        int pixel=Build_Hicolor_Pixel(color.Red,color.Green,color.Blue);
        Fill_Line_End_Rect(start,pixel);
        Fill_Line_End_Rect(end,pixel);
    }
    Rect clip=unknown_rect_839630;
    int phase=timeGetTime()>>4;
    CoordStruct last=from;
    int i=1;
    int j=31;
    while (i<=32) {
        next.X=(to.X*i+from.X*j)/32;
        next.Y=(to.Y*i+from.Y*j)/32;
        double t=(i-16.0)*0.0625;
        int z=(to.Z*i+from.Z*j)/32;
        double arc=-(t*t);
        next.Z=(int)(z+(arc+1.0)*512.0);
        unknown_tactical_83996c->Coord_To_Pixel(last,start);
        unknown_tactical_83996c->Coord_To_Pixel(next,end);
        start.Y+=unknown_rect_839630.Y;
        end.Y+=unknown_rect_839630.Y;
        if (Clip_Line(start,end,clip)) {
            shade=color;
            unsigned ratio=((i-phase)*8)&0x1f8;
            if (ratio<=0xf8 && ratio!=0) {
                shade.Adjust(ratio,white);
            }
            unknown_surface_839964->Draw_Line(start,end,Build_Hicolor_Pixel(shade.Red,shade.Green,shade.Blue));
        }
        last=next;
        i++;
        j--;
    }
}
#endif

// CoordStruct TechnoClass::unknown_slot241() (0x006D13B0, slot 241)
// With mission 1 and a +0x228 object, its slot 22 coordinate; otherwise Coord.

CoordStruct TechnoClass::unknown_slot241()
{
    if (Get_Mission() == 1 && unknown_228) {
        return unknown_228->unknown_slot22();
    }
    return Coord;
}


// void TechnoClass::unknown_6d1420(UnknownTechno408 *value) (0x006D1420)
// Sets unknown_408.

void TechnoClass::unknown_6d1420(UnknownTechno408 *value)
{
    unknown_408 = value;
}


// UnknownTechno408 *TechnoClass::unknown_6d1430() const (0x006D1430)
// Returns unknown_408.

UnknownTechno408 *TechnoClass::unknown_6d1430() const
{
    return unknown_408;
}


// bool TechnoClass::unknown_6d1440() (0x006D1440)
// Whether the object at +0x408 exists and its 0x00613B60 is positive.

bool TechnoClass::unknown_6d1440()
{
    if (unknown_408 == 0) {
        return false;
    }
    return unknown_408->unknown_613b60() > 0 ? true : false;
}


// bool TechnoClass::unknown_slot242() (0x006D1460, slot 242)
// Always true.

bool TechnoClass::unknown_slot242()
{
    return true;
}


// bool TechnoClass::unknown_6d1470() (0x006D1470)
// Forwards to 0x00615360 and returns its result.

bool TechnoClass::unknown_6d1470()
{
    return unknown_615360();
}


// ConvertClass *TechnoClass::unknown_slot115() (0x006D1480, slot 115)
// The colour scheme converter (ColorScheme::Converter of a ColorSchemes entry) to draw with: from
// the type's own palette list when it has one, else the owner's scheme, or
// slot 51's house's when slot 245 rejects the player.

ConvertClass *TechnoClass::unknown_slot115()
{
    TechnoTypeClass *type = Techno_Type_Class();
    int scheme = House->ColorSchemeIndex;
    DynamicVectorClass<ColorScheme *> *palette = (DynamicVectorClass<ColorScheme *> *)type->Palette;
    if (palette != 0) {
        return (*palette)[scheme]->Converter;
    }
    if (!unknown_slot245(unknown_player_a35db4) && unknown_slot51(true) != 0) {
        return ColorSchemes[unknown_slot51(true)->ColorSchemeIndex]->Converter;
    }
    return ColorSchemes[scheme]->Converter;
}


// Draw a techno shape with its visual state, lighting, composite bounds and shadow.

extern Rect unknown_unit_composite_rect_ad0490;
extern ConvertClass *unknown_drawer_ad0608;

void TechnoClass::unknown_6d1510(void const *shape, int frame, Point2D const *point, Rect const *bounds,
    int rotation, int scale, int z, int gradient, int zwrite, int brightness, ShapeFileStruct const *zshape, int zframe,
    Point2D offset, int negative_flags)
{
    if (shape) {
        ShapeFileStruct const *shape_file=static_cast<ShapeFileStruct const *>(shape);
        bool shadow=unknown_flag_7d88d5;
        int visual=unknown_slot25(false,0);
        unsigned int flags=0;
        if (!negative_flags) {
            switch (visual) {
            case 4:
                if (unknown_1bc.Fetch_Stage()) { flags=4; break; }
            case 1: flags=2; break;
            case 2:
            case 3: flags=4; break;
            case 5: return;
            }
        }
        if (unknown_slot111() || unknown_slot112()) flags|=4;
        ConvertClass *converter;
        if (Kind_Of()==6 && static_cast<BuildingClass *>(this)->Type->TerrainPalette) {
            CellClass *cell=&Map[Coord_Cell(Center_Coord())];
            converter=cell->unknown_34;
            if (!converter) {
                cell->Init_Drawer(0,0x10000,0,1000,1000,1000);
                converter=cell->unknown_34;
                if (!converter) return;
            }
            brightness=cell->unknown_10c;
        } else {
            if (Kind_Of()==1 && static_cast<UnitClass *>(this)->unknown_5bb) converter=unknown_drawer_ad0608;
            else converter=unknown_slot115();
        }
        Point2D draw_point=*point;
        int z_adjust=z;
        switch (Kind_Of()) {
        case 1:
            if (static_cast<UnitClass *>(this)->Type->SmallVisceroid || static_cast<UnitClass *>(this)->Type->LargeVisceroid) shadow=false;
        case 15:
            if (Get_Height_AGL()==0) z_adjust+=unknown_slot176();
            else { z_adjust-=Unknown_006A2970(Get_Height()); shadow=false; }
            break;
        case 2:
            draw_point.Y-=Unknown_006A2970(Get_Height());
            z_adjust-=Unknown_006A2970(Get_Height());
            break;
        }
        if (Techno_Type_Class()->NoShadow) shadow=false;
        if (!unknown_slot245(unknown_player_a35db4)) {
            if (unknown_slot50(true)->Kind_Of()!=21 && unknown_slot50(true)->Kind_Of()!=37) converter=ColorSchemes[unknown_slot51(true)->ColorSchemeIndex]->Converter;
            else {
                CellClass *cell=static_cast<CellClass *>(Get_Cell_Ptr());
                converter=cell->unknown_34;
                if (!converter) {
                    cell->Init_Drawer(0,0x10000,0,1000,1000,1000);
                    converter=cell->unknown_34;
                }
                brightness=cell->unknown_10c;
            }
            if (!converter) converter=ColorSchemes[unknown_player_a35db4->ColorSchemeIndex]->Converter;
        }
        if (gradient!=-1) flags|=0x2000;
        if ((unsigned char)zwrite) flags|=0x4000;
        flags|=0x800;
        if (Kind_Of()==1 && static_cast<UnitClass *>(this)->unknown_5bb) { flags|=0x20; shadow=false; }
        flags&=~negative_flags;
        if (Kind_Of()==1 && static_cast<UnitClass *>(this)->unknown_5bb) {
            Rect frame_rect=shape_file->Get_Frame_Rect(frame);
            Rect draw_rect;
            draw_rect.X=frame_rect.X+draw_point.X-shape_file->Get_Width()/2;
            draw_rect.Y=frame_rect.Y+draw_point.Y-shape_file->Get_Height()/2;
            draw_rect.Width=frame_rect.Width;
            draw_rect.Height=frame_rect.Height;
            unknown_unit_composite_rect_ad0490=Union(unknown_unit_composite_rect_ad0490,draw_rect);
        }
        if (House->Is_Player_Control() && unknown_slot48()) flags=unknown_slot244(flags);
        brightness=unknown_slot254(brightness);
        if (unknown_slot203()) brightness=unknown_6d86f0(brightness);
        switch (visual) {
        case 0:
            CC_Draw_Shape(unknown_surface_83995c,converter,shape_file,frame,draw_point,*bounds,flags|0x600,0,z_adjust-2,gradient,brightness,zshape,zframe,offset);
            if (shadow) {
                z_adjust=-2-Unknown_006A2970(Get_Height());
                if (unknown_37c) draw_point.Y-=14;
                flags&=~6;
                CC_Draw_Shape(unknown_surface_83995c,converter,shape_file,frame+shape_file->FrameCount/2,draw_point,*bounds,flags|0x601,0,z_adjust-2,0,1000,0,0,Point2D(0,0));
            }
            break;
        case 1:
            CC_Draw_Shape(unknown_surface_83995c,converter,shape_file,frame,draw_point,*bounds,flags|0x600,0,z_adjust-2,gradient,brightness,zshape,zframe,offset);
            if (shadow) {
                z_adjust=-2-Unknown_006A2970(Get_Height());
                flags&=~6;
                if (unknown_37c) draw_point.Y-=14;
                CC_Draw_Shape(unknown_surface_83995c,converter,shape_file,frame+shape_file->FrameCount/2,draw_point,*bounds,flags|0x601,0,z_adjust-2,0,1000,0,0,Point2D(0,0));
            }
            break;
        case 2:
        case 4:
            CC_Draw_Shape(unknown_surface_83995c,converter,shape_file,frame,draw_point,*bounds,flags|0x600,0,z_adjust-2,gradient,brightness,zshape,zframe,offset);
            break;
        case 3:
            CC_Draw_Shape(unknown_surface_83995c,converter,shape_file,frame,draw_point,*bounds,flags|0x600,0,z_adjust-2,gradient,brightness,zshape,zframe,offset);
            break;
        }
    }
}


extern Rect unknown_unit_rect_ad0490;
void TechnoClass::unknown_slot246(UnknownObjectPair const &voxel,int frame,int key,UnknownBuffer *cache,Rect const &cliprect,Point2D const &draw_point,Matrix3D const &transform,int brightness,unsigned negflags)
{
    unsigned flags=0x2000;
    if (!negflags) {
        switch (unknown_slot25(false,0)) {
        case 4: {
            // Reference distinguishes ripple stages0,1,and later stages.
            int stage=unknown_1bc.Fetch_Stage();
            if (stage==0) flags=0x200a;
            else if (stage==1) flags=0x200c;
            else flags=0x200c;
            break;
        }
        case 1: flags=0x2002; break;
        case 2: case 3: flags=0x2004; break;
        case 5: return;
        }
    }
    if ((unknown_slot111() || unknown_slot112()) && (Kind_Of()!=1 || !((UnitClass *)this)->unknown_5bb)) flags|=0x2004;
    if (House->Is_Player_Control() && unknown_slot48() && (Kind_Of()!=1 || !((UnitClass *)this)->unknown_5bb)) flags=unknown_slot244(flags);
    flags=(flags|0x800)&~negflags;
    Rect window=cliprect;
    brightness=unknown_slot254(brightness);
    if (unknown_slot203()) brightness=unknown_6d86f0(brightness);
    if (key!=-1) {
        if (!cache || Techno_Type_Class()->DisableVoxelCache) key=-1;
        else {
            void *entry=(*cache)[key];
            if (entry) {
                Blit_Voxel_006d29d0(entry,draw_point,window,flags,brightness);
                return;
            }
        }
    }
    if (!voxel.unknown_00 || !voxel.unknown_04) return;
    SurfaceRegion region=Render_Voxel_Object_006d24e0(voxel,transform,draw_point,window,frame,flags,brightness);
    int height=region.Bounds.Height;
    int width=region.Bounds.Width;
    if (Kind_Of()==1) unknown_unit_rect_ad0490=Union(unknown_unit_rect_ad0490,Rect(region.Point.X+draw_point.X,region.Point.Y+draw_point.Y,width,height));
    if (key==-1) return;
    void *data=unknown_voxel_static_buffer_839aa0.Add(*VoxelDrawSystem::Get_Surface(),region);
    if (!data || !cache->Add_Index(key,data)) {
        Class_Of()->Clear_Voxel_Indexes();
        if (!data) data=unknown_voxel_static_buffer_839aa0.Add(*VoxelDrawSystem::Get_Surface(),region);
        cache->Add_Index(key,data);
    }
}


// Complete original shadow draw764body+4padding, eleven stack arguments.
void TechnoClass::Unknown_006D21E0(UnknownObjectPair const &voxel,int layer,int key,UnknownBuffer *cache,Rect const &cliprect,Point2D const &draw_point,Matrix3D const &transform,bool force_cache,Surface *mask,unsigned unknown_2,unsigned unknown_3)
{
    if (unknown_1b8 || Techno_Type_Class()->NoShadow || voxel.unknown_00->LoadFailed) return;
    if (key!=-1) {
        if (!cache || Techno_Type_Class()->DisableShadowCache) key=-1;
        else {
            void *entry=(*cache)[key];
            if (entry) {
                Blit_Voxel_006d29d0(entry,draw_point,cliprect,0x2001,1000);
                return;
            }
            if (!force_cache) return;
        }
    }
    SurfaceRegion region=Render_Voxel_Shadow_006d2810(voxel,transform,draw_point,cliprect,layer,0x2001,key!=-1,mask,unknown_2,unknown_3);
    if (key!=-1) {
        bool added=true;
        void *data=unknown_voxel_static_buffer_839aa0.Add(*VoxelDrawSystem::Get_Surface(),region);
        if (!data || !cache->Add_Index(key,data)) {
            added=false;
            Class_Of()->Clear_Voxel_Indexes();
            if (!data) data=unknown_voxel_static_buffer_839aa0.Add(*VoxelDrawSystem::Get_Surface(),region);
        }
        if (!added) cache->Add_Index(key,data);
        Unknown_006D21E0(voxel,layer,key,cache,cliprect,draw_point,transform,false,0,0,0);
    }
}


// Original complete object renderer: 814 body bytes plus 2 frozen NOPs.
namespace VoxelDrawSystem {
    void Reset();
    void __fastcall Precalculate_Light(VoxelLibrary *voxel,int layer,int info,Matrix3D const &motion,Matrix3D const &camera,Vector3 const &light,float scale);
    void __fastcall Prep_For_Object(VoxelLibrary *voxel,unsigned layer,int info,Matrix3D const &matrix);
    SurfaceRegion __fastcall Render();
}
extern Matrix3D unknown_voxel_transform_839a70;
extern Vector3 unknown_voxel_light_839ab0;
extern BSurface *unknown_surface_ad0604;
extern ConvertClass *unknown_drawer_ad0608;
extern int unknown_int_839638;
SurfaceRegion TechnoClass::Render_Voxel_Object_006d24e0(UnknownObjectPair const &voxel,Matrix3D const &transform,Point2D const &point,Rect const &xcliprect,int frame,unsigned flags,int brightness)
{
    Matrix3D combined;
    combined=transform;
    Matrix3D base;
    base=transform;
    VoxelLibrary *voxlib=voxel.unknown_00;
    UnknownPairSecond *motion=voxel.unknown_04;
    VoxelDrawSystem::Precalculate_Light(voxlib,0,0,base,unknown_voxel_transform_839a70,unknown_voxel_light_839ab0,3.0f);
    VoxelDrawSystem::Reset();
    for (unsigned layer=0;layer<(unsigned)voxlib->LayerCount;++layer) {
        if (motion) {
            Matrix3D section;
            section=motion->unknown_transforms[((unsigned)frame%motion->unknown_frame_count)*motion->unknown_section_count+layer];
            combined=base*section;
        }
        VoxelDrawSystem::Prep_For_Object(voxlib,layer,0,unknown_voxel_transform_839a70*combined);
    }
    SurfaceRegion region=VoxelDrawSystem::Render();
    if (unknown_slot111() || unknown_slot112()) flags|=4;
    if (House->Is_Player_Control() && unknown_slot48()) flags=unknown_slot244(flags);
    Rect cliprect=xcliprect;
    if (unknown_surface_83995c!=unknown_surface_ad0604) {
        if (unknown_322>0) {
            cliprect=Intersect(cliprect,Rect(0,0,unknown_int_839638,unknown_322-unknown_tactical_83996c->TacPixelY));
        } else if (unknown_325) {
            unknown_322=(short)(unknown_tactical_83996c->TacPixelY+point.Y+region.Bounds.Height+region.Point.Y);
        }
    }
    flags&=~0x10U;
    ConvertClass *converter;
    if (Kind_Of()==1 && static_cast<UnitClass *>(this)->unknown_5bb) converter=unknown_drawer_ad0608;
    else converter=unknown_slot115();
    int phase=0;
    if (flags&8) phase=(Fetch_ID()+(int)unknown_1e0)%400;
    Point2D destination=region.Point+point;
    unknown_blit_49f3a0(*unknown_surface_83995c,converter,*VoxelDrawSystem::Get_Surface(),region.Bounds,RectPoint2D(destination.X,destination.Y),cliprect,0,converter->Select_Blitter_00485AA0(flags),unknown_slot176(),unknown_slot177(),brightness,phase);
    return region;
}


// Original shadow renderer: complete440bytes plus8frozenNOPs.
namespace VoxelDrawSystem {
    void Reset();
    void __fastcall Prep_For_Shadow(VoxelLibrary *voxel,int layer,int info,Matrix3D const &camera,Matrix3D const &motion,Vector3 const &light,Surface *mask,int mask_x,int mask_y);
    SurfaceRegion __fastcall Render();
}
extern Matrix3D unknown_voxel_transform_839a70;
extern Vector3 unknown_839a60;
extern ConvertClass *unknown_drawer_8323c4;
SurfaceRegion TechnoClass::Render_Voxel_Shadow_006d2810(UnknownObjectPair const &voxel,Matrix3D const &transform,Point2D const &point,Rect const &cliprect,int layer,unsigned flags,bool cached,Surface *mask,unsigned unknown_2,unsigned unknown_3)
{
    UnknownPairSecond *motion=voxel.unknown_04;
    Matrix3D combined;
    {
        Matrix3D section_matrix;
        section_matrix=motion->unknown_transforms[(0U%motion->unknown_frame_count)*motion->unknown_section_count+layer];
        combined=transform*section_matrix;
    }
    Matrix3D basis;
    basis.Make_Identity();
    if (Techno_Type_Class()->ConsideredAircraft) basis.Scale(0.5f);
    VoxelDrawSystem::Reset();
    VoxelDrawSystem::Prep_For_Shadow(voxel.unknown_00,layer,0,unknown_voxel_transform_839a70*basis,combined,unknown_839a60,mask,unknown_2,unknown_3);
    SurfaceRegion region=VoxelDrawSystem::Render();
    if (!cached) {
        (void)VoxelDrawSystem::Get_Surface()->Get_Rect();
        Point2D destination=region.Point+point;
        unknown_blit_49f3a0(*unknown_surface_83995c,unknown_drawer_8323c4,*VoxelDrawSystem::Get_Surface(),region.Bounds,RectPoint2D(destination.X,destination.Y),cliprect,0,unknown_drawer_8323c4->Select_Blitter_00485AA0(flags),unknown_slot176(),unknown_slot177(),1000,0);
    }
    return region;
}


// Complete cached-voxel blit: 710 body bytes and 10 frozen NOPs.
extern ConvertClass *unknown_drawer_ad0608;
extern Rect unknown_unit_rect_ad0490;
void TechnoClass::Blit_Voxel_006d29d0(void const *image,Point2D const &point,Rect const &cliprect,unsigned flags,int brightness)
{
    StaticBufferClass::Entry const &entry=*static_cast<StaticBufferClass::Entry const *>(image);
    BSurface surface(entry.Width,entry.Height,1,entry.Data);
    flags&=~0x10U;
    ConvertClass *converter;
    if (Kind_Of()==1 && static_cast<UnitClass *>(this)->unknown_5bb) converter=unknown_drawer_ad0608;
    else converter=unknown_slot115();
    RLEBlitter const *blitter=converter->RLEBlitter_From_Flags_00485d60(flags);
    int phase=0;
    if (flags&8) phase=(Fetch_ID()+(int)unknown_1e0)%400;
    if (Kind_Of()==1) {
        unknown_unit_rect_ad0490=Union(unknown_unit_rect_ad0490,Rect(entry.X+point.X,entry.Y+point.Y,entry.Width,entry.Height));
    }
    if (blitter) {
        RLE_Blit(*unknown_surface_83995c,cliprect,Rect(entry.X+point.X,entry.Y+point.Y,entry.Width,entry.Height),surface,surface.Get_Rect(),surface.Get_Rect(),*blitter,unknown_slot176(),unknown_slot177(),brightness,phase,0,BlitPair(0,0));
    }
}


// void TechnoClass::unknown_6d2ca0(int x, int y) (0x006D2CA0)
// Sets the draw depth +0x322 from a screen point and the view's Y, raised
// for turret directions 0, 6 (8 pixels) and 7 (12 pixels). Never called:
// inlined at every use.

void TechnoClass::unknown_6d2ca0(int x, int y)
{
    unknown_322 = unknown_tactical_83996c->TacPixelY + x + y - 10;
    int dir = Dir_To_8(unknown_2e0.Current());
    if (dir == 0 || dir == 6) {
        unknown_322 -= 8;
    } else if (dir == 7) {
        unknown_322 -= 12;
    }
}


// void TechnoClass::Detach(AbstractClass *target, bool all) (0x006D2D10, slot 10)
// Forgets an object that is going away: RadioClass's part, the passenger
// (all), +0x23C and +0x20C; the target (+0x228; kept while the target's cell
// stays visible to the owner unless all, and only when it belongs to another
// house or all) after re-arming the +0x148 timer to 4-8 frames when more than
// 10 remain, clearing it through slot 220 and restoring a suspended mission
// (an aircraft on mission 0x19 also drops +0xA8 and +0x5BA); +0x22C likewise;
// the owner; the slot 51 object when slot 48; the eight +0x25C slots, +0x27C;
// with all, +0x3F8, +0x1B0 and the capture manager's hold; the spawn and
// temporal helpers; +0x384, +0x108; and the target's entries in the +0x390 /
// +0x3A8 / +0x3C0 lists.

void TechnoClass::Detach(AbstractClass *target, bool all)
{
    RadioClass::Detach(target, all);
    if (all) {
        Cargo.Detach((FootClass *)target);
    }
    bool forget = true;
    if (target == unknown_23c) {
        unknown_23c = 0;
    }
    if (target == unknown_20c) {
        unknown_20c = 0;
    }
    bool object;
    if (!all && target != 0) {
        object = target->AbstractFlags.IsTechno;
    }
    if (!all && target != 0 && object) {
        if (Map_Cell(target->Center_Coord())->Is_Sensed(House->ArrayIndex)) {
            forget = false;
        }
    }
    if (unknown_228 == target && forget && (all || target->Owning_House() != House)) {
        if (unknown_148.Value() > 10 && unknown_a40774 == 0) {
            unknown_148 = unknown_id_source_a3d290->Random()(4, 8);
        }
        unknown_slot220(0);
        if (Has_Suspended_Mission()) {
            Restore_Mission();
            if (Kind_Of() == 2 && Mission == 0x19) {
                unknown_a8 = 0;
                ((AircraftClass *)this)->unknown_5ba = false;
            }
        }
    }
    if (unknown_22c == target && forget) {
        unknown_22c = 0;
    }
    if (House == target) {
        House = 0;
        unknown_372 = false;
    }
    if (unknown_slot48() && unknown_slot51(true) == target) {
        unknown_slot256();
    }
    for (int slot = 0; slot < 8; slot++) {
        if (unknown_25c[slot] == target) {
            unknown_25c[slot] = 0;
        }
    }
    if (unknown_27c == target) {
        unknown_27c = 0;
    }
    if (all) {
        if (unknown_3f8 == target) {
            unknown_3f8 = 0;
        }
        if (unknown_1b0 == target) {
            unknown_1b0 = 0;
        }
        if (unknown_230 != 0) {
            ((CaptureManagerClass *)unknown_230)->Detach_Target_468120(target);
        }
    }
    if (SpawnManager != 0) {
        SpawnManager->Unlink(target);
    }
    if (unknown_208 != 0) {
        unknown_208->Unknown_006E24C0(target);
    }
    if (target == unknown_384) {
        unknown_384 = 0;
    }
    if (target == unknown_108) {
        unknown_108 = 0;
    }
    // The list removals take `&*target`, an rvalue: the original passes a
    // temporary copy of the pointer, never the parameter's own slot.
    int index = -1;
    for (int i = 0; i < unknown_3a8.Count(); i++) {
        if (target == unknown_3a8[i]) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        unknown_390.Delete(index);
        unknown_3a8.Delete(&*target);
    }
    unknown_3c0.Delete(&*target);
}


// void TechnoClass::unknown_6d3040(TechnoClass *killer) (0x006D3040)
// Destroys every passenger: each leaves its team, is detached from the
// cargo, gets slot 55 (killer) and is deleted.

void TechnoClass::unknown_6d3040(TechnoClass *killer)
{
    while (Cargo.Object != 0) {
        ObjectClass *passenger = Cargo.Attached_Object();
        if (((UnknownPassengerView *)passenger)->Team != 0) {
            ((UnknownPassengerView *)passenger)->Team->unknown_6b8fb0(passenger, -1, false);
        }
        passenger = Cargo.Detach_Object();
        if (passenger != 0) {
            passenger->Record_The_Kill(killer);
            delete passenger;
        }
    }
}


// InfantryTypeClass *TechnoClass::unknown_slot184() (0x006D30B0, slot 184)
// The crew type a Crewed techno leaves: AlliedCrew or SovietCrew by the
// owner's +0x130 (else Technician); Technician for houses without a side,
// and with a 15% chance for armed technos.

InfantryTypeClass *TechnoClass::unknown_slot184()
{
    if (!Techno_Type_Class()->Crewed) {
        return 0;
    }
    InfantryTypeClass *crew;
    int side = House->unknown_130;
    if (side == 0) {
        crew = Rules->AlliedCrew;
    } else if (side == 1) {
        crew = Rules->SovietCrew;
    } else {
        crew = Rules->Technician;
    }
    if (House->Type->Side != -1) {
        if (unknown_slot163() && unknown_id_source_a3d290->Random()(0, 99) < 15) {
            crew = Rules->Technician;
        }
    } else {
        crew = Rules->Technician;
    }
    return crew;
}


// int TechnoClass::Value() (0x006D3150, slot 90)
// The type's +0x59C plus slot 168, plus slot 90 of every passenger when the
// owner's difficulty has ContentScan= set or the owner's
// +0x194 reaches rules +0x10B8. Names are hypotheses.

int TechnoClass::Value()
{
    int passengers = 0;
    HouseClass *house = House;
    if (Rules->Diff[house->unknown_c8].ContentScan || house->unknown_194 >= Rules->unknown_10b8) {
        if (Cargo.Object != 0) {
            for (ObjectClass *object = Cargo.Attached_Object(); object != 0; object = object->unknown_30) {
                passengers += object->Value();
            }
        }
    }
    return Techno_Type_Class()->unknown_59c + unknown_slot168() + passengers;
}


// int TechnoClass::unknown_slot188(int arg1) (0x006D31E0, slot 188)
// A guard range: -1 for -1; for 0 the type's GuardRange unless slot 193
// holds (else 0); otherwise twice the GuardRange or, without one, twice the
// longer of the two slot 85 ranges, clamped to 0x700..0x1000 for 2 and to
// 0..0x1000 for the rest.

int TechnoClass::unknown_slot188(int arg1)
{
    if (arg1 == -1) {
        return -1;
    }
    if (arg1 == 0) {
        bool flag = unknown_slot193();
        if (Techno_Type_Class()->GuardRange.Value != 0 && !flag) {
            return Techno_Type_Class()->GuardRange.Value;
        }
        return 0;
    }
    int range = Techno_Type_Class()->GuardRange.Value;
    if (range == 0) {
        range = (Weapon_Range(0) > Weapon_Range(1)) ? Weapon_Range(0) : Weapon_Range(1);
    }
    range *= 2;
    if (arg1 == 2) {
        if (range < 0x700) {
            return 0x700;
        }
        if (range > 0x1000) {
            return 0x1000;
        }
    } else {
        if (range < 0) {
            return 0;
        }
        if (range > 0x1000) {
            range = 0x1000;
        }
    }
    return range;
}


// bool TechnoClass::unknown_slot171(CoordStruct const &coord) (0x006D32E0, slot 171)
// Whether the movement zone can reach the cell of a coordinate from the
// object's own cell (the map's path test 0x0054F7C0, with the target cell's
// bridge level); always for zone -1 or the zero coordinate.

bool TechnoClass::unknown_slot171(CoordStruct const &coord)
{
    int zone = Techno_Type_Class()->MovementZone;
    if (zone != -1) {
        if (!(coord == unknown_geometry_l)) {
            Cell cell = Coord_Cell(coord);
            Cell here = Coord_Cell(unknown_slot19(0));
            return Map.Unknown_0054F7C0(here, cell, zone, Is_Moving_Onto_Bridge(0),
                (unsigned char)((((UnknownCellClass *)&Map[cell])->Flags >> 8) & 1), unknown_slot189());
        }
    }
    return true;
}


// Recruit up to six infantry and vehicles to respond to an attack on this base.
void TechnoClass::unknown_6d3400(TechnoClass *attacker)
{
    FootClass *responders[6];
    int scores[6];
    memset(responders,0,sizeof(responders));
    memset(scores,0,sizeof(scores));
    int count=0;
    int minimum=0;
    int required=attacker->unknown_slot168()*Rules->ComputerBaseDefenseResponse;
    int recruited=0;
    if (!attacker || House->Is_Ally(attacker) || House->Is_Human_Player() || attacker->unknown_76) return;
    if (!unknown_game_mode_a3d298 && unknown_slot163()) return;
    if (attacker->Kind_Of()!=15 && attacker->Kind_Of()!=1) return;
    if (Techno_Type_Class()->unknown_202) return;
    bool is_foot;
    if (attacker) is_foot=attacker->AbstractFlags.IsFoot;
    if (attacker && is_foot && static_cast<FootClass *>(attacker)->unknown_540!=0) return;
    TeamClass::unknown_func_6ba8e0(Rules->SuspendPriority,House);
    for (int index=0;index<Infantry.Count();++index) {
        if (required<=0) break;
        InfantryClass *member=Infantry[index];
        if (member && member->unknown_7d && member->Owner()==Owner()) {
            bool base_defense=member->Team && member->Team->Class->IsBaseDefense;
            if ((!member->Team || base_defense) && member->unknown_379 && member->unknown_slot163() && member->unknown_37a && (member->Control()->unknown_06 || unknown_game_mode_a3d298)) {
                int armor=attacker->Techno_Type_Class()->unknown_9c;
                if (member->unknown_slot230()->WeaponType->Warhead->Verses[armor]!=0.0) {
                    Cell from=Coord_Cell(unknown_slot19(0));
                    Cell to=Coord_Cell(member->unknown_slot19(0));
                    if (Map.Unknown_0054F7C0(to,from,member->Type->MovementZone,member->Is_Moving_Onto_Bridge(0),false,false)) {
                        int value=member->unknown_4c82a0(attacker);
                        if (value) {
                            if (unknown_1b0==this) value*=100;
                            if (value<0) required+=value;
                            else if (count<6) { responders[count]=member; scores[count]=value; ++count; }
                            else if (value>minimum) {
                                int lowest=value;
                                for (int check=0;check<count;++check) {
                                    if (scores[check]==minimum) { scores[check]=value;responders[check]=member; }
                                    else if (scores[check]<lowest) lowest=scores[check];
                                }
                                minimum=lowest;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int unit_index=0;unit_index<Units.Count();++unit_index) {
        if (required<=0) break;
        UnitClass *member=Units[unit_index];
        if (member && member->unknown_7d) {
            int member_owner=member->Owner();
            if (member_owner!=Owner()) continue;
            bool base_defense=member->Team && member->Team->Class->IsBaseDefense;
            if ((!member->Team || base_defense) && member->unknown_379 && member->unknown_slot163() && member->unknown_37a && (member->Control()->unknown_06 || unknown_game_mode_a3d298)) {
                int armor=attacker->Techno_Type_Class()->unknown_9c;
                if (member->unknown_slot230()->WeaponType->Warhead->Verses[armor]!=0.0) {
                    Cell from=Coord_Cell(unknown_slot19(0));
                    Cell to=Coord_Cell(member->unknown_slot19(0));
                    if (Map.Unknown_0054F7C0(to,from,member->Type->MovementZone,member->Is_Moving_Onto_Bridge(0),false,false)) {
                        int value=member->unknown_4c82a0(attacker);
                        if (value) {
                            if (value>0 && unknown_1b0==this) value*=10;
                            if (value<0) required+=value;
                            else if (count<6) { responders[count]=member;scores[count]=value;++count; }
                            else if (value>minimum) {
                                int lowest=value;
                                for (int check=0;check<count;++check) {
                                    if (scores[check]==minimum) {scores[check]=value;responders[check]=member;}
                                    else if (scores[check]<lowest) lowest=scores[check];
                                }
                                minimum=lowest;
                            }
                        }
                    }
                }
            }
        }
    }
    if (required>0) {
        for (int first=0;first<count-1;++first) {
            for (int second=first+1;second<count;++second) {
                if (scores[first]<scores[second]) {
                    scores[first]^=scores[second];scores[second]^=scores[first];scores[first]^=scores[second];
                    FootClass *saved=responders[first];responders[first]=responders[second];responders[second]=saved;
                }
            }
        }
        for (int chosen=0;chosen<count;++chosen) {
            FootClass *member=responders[chosen];
            bool base_defense=member->Team && member->Team->Class->IsBaseDefense;
            if (unknown_id_source_a3d290->Random()(0,99)>=66 || base_defense) {
                member->QueueMission(11,false);
                member->unknown_1b0=this;
            } else {
                member->QueueMission(21,false);
                member->unknown_1b0=this;
            }
            member->unknown_slot220(attacker);
            recruited+=member->unknown_slot168();
            if (recruited>required) break;
        }
    }
    if (recruited>required) {
        if (attacker) is_foot=attacker->AbstractFlags.IsFoot;
        if (attacker && is_foot) static_cast<FootClass *>(attacker)->unknown_540=(int)(Rules->BaseDefenseDelay*900.0);
    }
}


// bool TechnoClass::unknown_6d3b60(TechnoClass *source, int arg2) (0x006D3B60)
// Whether the object answers an attack by source (hypothesis: may
// retaliate): the type must allow it (CanRetaliate) and the mission's
// control (Retaliate), the source must be an enemy that can be hit, and the
// player's own units only fire back when Rules PlayerReturnFire allows or
// they are on a mission that does; arg2 is not read.

bool TechnoClass::unknown_6d3b60(TechnoClass *source, int arg2)
{
    if (source == 0) {
        return false;
    }
    if (!Techno_Type_Class()->CanRetaliate) {
        return false;
    }
    if (unknown_230 != 0 && ((UnknownTechno230 *)unknown_230)->unknown_24 != 0) {
        return false;
    }
    if (SpawnManager != 0) {
        return false;
    }
    if (House->Unknown_004F52F0() && unknown_228 != 0) {
        return false;
    }
    if (!Control()->unknown_08) {
        return false;
    }
    if (House->Is_Ally(source)) {
        return false;
    }
    if (source->unknown_slot49(House)) {
        return false;
    }
    if (unknown_6c16f0(-1) <= 0) {
        return false;
    }
    if (!unknown_slot163()) {
        return false;
    }
    TechnoTypeClass *type = Techno_Type_Class();
    int weapon = unknown_slot174(source);
    int result = unknown_slot217(source, weapon);
    if (result == 5 || result == 6) {
        return false;
    }
    if (House->Unknown_004F52F0() && source->Kind_Of() == 6) {
        if (Kind_Of() == 15 && ((InfantryTypeClass *)type)->C4) {
            return false;
        }
        if (unknown_6d7910(14)) {
            return false;
        }
    }
    if (House->Unknown_004F52F0() && Kind_Of() == 1) {
        BuildingTypeClass *building = ((UnitClass *)this)->Type->DeploysInto;
        if (building != 0 && building->Artillary) {
            return false;
        }
    }
    if (House->Unknown_004F52F0() && !Rules->PlayerReturnFire && Kind_Of() != 6
        && Mission != 11 && Mission != 5 && Mission != 25) {
        return false;
    }
    bool foot;
    if (this != 0) {
        foot = AbstractFlags.IsFoot;
    }
    if (this != 0 && foot && ((FootClass *)this)->Team != 0 && ((FootClass *)this)->Team->Class->Suicide) {
        return false;
    }
    if (!House->Unknown_004F52F0()) {
        AbstractClass *target = unknown_228;
        bool techno;
        if (target != 0) {
            techno = target->AbstractFlags.IsObject;
        }
        if (target != 0 && techno
            && unknown_6d7550((ObjectClass *)target, unknown_geometry_l) > unknown_6d7550((ObjectClass *)source, unknown_geometry_l)) {
            return false;
        }
    }
    bool foot2;
    if (this != 0) {
        foot2 = AbstractFlags.IsFoot;
    }
    if (this != 0 && foot2 && source == ((FootClass *)this)->unknown_580) {
        return false;
    }
    return true;
}


// int TechnoClass::Get_Ownable() (0x006D3E40, slot 34)
// Slot 28 of the techno type.

int TechnoClass::Get_Ownable()
{
    return Techno_Type_Class()->unknown_slot28();
}


// int TechnoClass::unknown_slot168() (0x006D3E50, slot 168)
// For a building with a positive 0x004532F0 value, that value times the rules +0xBE8; otherwise the type +0x598.

int TechnoClass::unknown_slot168()
{
    TechnoTypeClass *type = Techno_Type_Class();
    if (type) {
        if (Kind_Of() == 6 && ((BuildingClass *)this)->unknown_4532f0() > 0) {
            int value = Rules->unknown_be8;
            return value * ((BuildingClass *)this)->unknown_4532f0();
        }
        return type->ThreatPosed;
    }
    return 0;
}


// double TechnoClass::unknown_slot165() (0x006D3EA0, slot 165)
// The constant at 0x0079B610 (0.0).

double TechnoClass::unknown_slot165()
{
    return unknown_double_79b610;
}


// int TechnoClass::unknown_slot182(AbstractClass *object, Cell *cell) (0x006D3EB0, slot 182)
// Stores the zero cell; returns 0.

int TechnoClass::unknown_slot182(AbstractClass *object, Cell *cell)
{
    *cell = unknown_geometry_k;
    return 0;
}


// int TechnoClass::unknown_slot166() (0x006D3ED0, slot 166)
// The pips to draw: the type's slot 29 count scaled by ammunition (PipScale
// 1) or passengers (PipScale 3) over the type's maximum, rounded, at least
// one while any remain.

int TechnoClass::unknown_slot166()
{
    double amount;
    double maximum;
    switch (Techno_Type_Class()->PipScale) {
    case 1:
        amount = unknown_254;
        maximum = Techno_Type_Class()->Ammo;
        break;
    case 3:
        amount = Cargo.Count;
        maximum = Techno_Type_Class()->Passengers;
        break;
    default:
        return 0;
    }
    int pips = (int)(Techno_Type_Class()->Max_Pips() * (amount / maximum) + 0.5);
    if (pips == 0 && amount > 0.0) {
        pips = 1;
    }
    return pips;
}


// UnknownFacingDir TechnoClass::unknown_slot183() (0x006D3F80, slot 183)
// Slot 162 by value.

UnknownFacingDir TechnoClass::unknown_slot183()
{
    return unknown_slot162();
}


// void TechnoClass::unknown_slot196() (0x006D3FA0, slot 196)
// Plays a random sound from the type list VoiceSelect when sound is enabled.

void TechnoClass::unknown_slot196()
{
    if (unknown_sound_enabled_7d88d6) {
        if (Techno_Type_Class()->VoiceSelect.Count()) {
            DynamicVectorClass<int> &sounds = Techno_Type_Class()->VoiceSelect;
            unsigned random = unknown_random_839218();
            Voc_Play(sounds[random % sounds.Count()], 1.0f, 0x2000, 0);
        }
    }
}


// void TechnoClass::unknown_slot197() (0x006D4000, slot 197)
// Plays a random sound from the type list VoiceMove when sound is enabled.

void TechnoClass::unknown_slot197()
{
    if (unknown_sound_enabled_7d88d6) {
        if (Techno_Type_Class()->VoiceMove.Count()) {
            DynamicVectorClass<int> &sounds = Techno_Type_Class()->VoiceMove;
            unsigned random = unknown_random_839218();
            Voc_Play(sounds[random % sounds.Count()], 1.0f, 0x2000, 0);
        }
    }
}


// void TechnoClass::unknown_slot198() (0x006D4060, slot 198)
// Plays a random sound from the type list VoiceAttack when sound is enabled.

void TechnoClass::unknown_slot198()
{
    if (unknown_sound_enabled_7d88d6) {
        if (Techno_Type_Class()->VoiceAttack.Count()) {
            DynamicVectorClass<int> &sounds = Techno_Type_Class()->VoiceAttack;
            unsigned random = unknown_random_839218();
            Voc_Play(sounds[random % sounds.Count()], 1.0f, 0x2000, 0);
        }
    }
}


// bool TechnoClass::unknown_6d40c0() (0x006D40C0)
// Whether the techno may scan for a target now: the scan interval
// (0x00AC01CC) has passed since +0x3F4, slot 113 does not hold, the type
// CanPassiveAquire, a building type flagged +0x121E needs 0x004532F0, and
// it is armed (slot 163) without a busy +0x230 or an owner-controlled slot
// 193.

bool TechnoClass::unknown_6d40c0()
{
    if ((unsigned)(Frame - unknown_3f4) >= unknown_ac01cc) {
        if (!unknown_slot113()) {
            if (Techno_Type_Class()->CanPassiveAquire) {
                if (Kind_Of() != 6 || !((UnknownTechnoBuildingType *)((BuildingClass *)this)->Type)->unknown_121e
                    || ((BuildingClass *)this)->unknown_4532f0() != 0) {
                    bool busy = unknown_230 != 0 && ((UnknownTechno230 *)unknown_230)->unknown_24 != 0;
                    if ((!unknown_slot193() || !House->Unknown_004F52F0()) && !busy && unknown_slot163()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


// bool TechnoClass::unknown_6d4180() (0x006D4180)
// Whether the object may take another order now (hypothesis: should keep
// guarding / hunting): false for a while after being constructed or
// asked; a team member of an aggressive, non-suicide team on mission 2
// without a target always may; otherwise it needs 0x006D40C0, and a unit
// on mission 2 of a type with BalloonHover, or on mission 5 whose
// weapon (flag +0x126) already is the one for the target, may not.

bool TechnoClass::unknown_6d4180()
{
    if ((unsigned int)(Frame - unknown_3f4) < unknown_ac01cc) {
        return false;
    }
    bool foot;
    bool foot2;
    if (unknown_228 == 0 && !House->Unknown_004F52F0() && this != 0) {
        if (this != 0) foot = AbstractFlags.IsFoot;
        if (foot && ((FootClass *)this)->Team != 0) {
            TeamTypeClass *team_type = ((FootClass *)this)->Team->Class;
            if (!team_type->Suicide && team_type->Aggressive && Mission == 2) {
                return true;
            }
        }
    }
    if (!unknown_6d40c0()) {
        return false;
    }
    if (Mission == 2 && Techno_Type_Class()->BalloonHover) {
        foot2 = false;
        if (this != 0) foot2 = AbstractFlags.IsFoot;
        if (!foot2) {
            return false;
        }
        if (((FootClass *)this)->unknown_494 == Get_Cell_Ptr()) {
            if (unknown_408 == 0) {
                return true;
            }
            if (unknown_408->unknown_613b60() <= 0) {
                return true;
            }
        }
    }
    if (!Techno_Type_Class()->OpportunityFire) {
        if (Mission != 5) {
            return false;
        }
        int weapon = unknown_slot226();
        WeaponStruct *slot = unknown_slot231(weapon);
        bool flagged;
        if (slot == 0 || slot->WeaponType == 0 || !slot->WeaponType->unknown_126) {
            flagged = false;
        } else {
            flagged = true;
        }
        int best = unknown_slot174(unknown_228);
        bool same = best == weapon;
        if (flagged && same) {
            return false;
        }
    }
    return true;
}


// 0x006d42f0: function not yet identified
//
// Body: 32 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 unknown_is_kind_not_5_6_8_006d42f0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006d42f0 {

unsigned int __stdcall unknown_is_kind_not_5_6_8_006d42f0(int kind)
{
    if (((kind != 5) && (kind != 6)) && (kind != 8)) {
        return 1;
    }
    return 0;
}

}  // namespace seed_006d42f0


// bool TechnoClass::unknown_6d4310() (0x006D4310)
// When 0x006D4180 allows it: stamps +0x3F4 with the frame and asks slot 209
// about the current coordinate; a changed +0x228 then sets +0x404.

bool TechnoClass::unknown_6d4310()
{
    bool result = false;
    if (unknown_6d4180()) {
        unknown_3f4 = Frame;
        AbstractClass *target = unknown_228;
        CoordStruct coord = Coord;
        result = unknown_slot209(&coord, 1);
        if (result && unknown_228 != target) {
            unknown_404 = true;
        }
    }
    return result;
}


// 0x006d4390: function not yet identified
//
// Body: 30 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_mission_2_5_10_006d4390(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006d4390 {

unsigned int __fastcall unknown_mission_2_5_10_006d4390(int self)
{
    int mission;

    mission = (int)(*(int *)(self + 0x98));
    if (((mission != 2) && (mission != 10)) && (mission != 5)) {
        return 0;
    }
    return 1;
}

}  // namespace seed_006d4390


// 0x006d43b0: function not yet identified
//
// Body: 74 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_mission_set_006d43b0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006d43b0 {

unsigned int __fastcall unknown_mission_set_006d43b0(int self)
{
    int mission;

    mission = (int)(*(int *)(self + 0x98));
    if (((((mission != 0) && (mission != 7)) && (mission != 0xd)) &&
            ((((mission != 0xe && (mission != 0x10)) &&
                ((mission != 0x12 && ((mission != 0x13 && (mission != 0x14)))))) && (mission != 0x16)))) &&
          (((mission != 0x17 && (mission != 0x1c)) && (mission != 0x18)))) {
        return 0;
    }
    return 1;
}

}  // namespace seed_006d43b0


// TechnoClass 0x006D4400 (RA2-only DistributedFire target spreading; called from slot 209
// 0x006D46D0): drops spread targets that left the candidate list, collects the candidates not
// yet fired on with their threat values, and assigns the most valuable one (all candidates
// again, after clearing the fired list, once every candidate has been fired on).
// unknown_3a8 = candidate targets, unknown_390 = their values, unknown_3c0 = targets fired on.
// The membership tests are written-out loops (an inlined ID compared with -1 does not thread
// the found branch); the two local vectors' vtables, slots and out-of-line base constructors
// are bound by @ref (they are emitted outside their home population).
void TechnoClass::unknown_6d4400()
{
    if (unknown_3a8.Count() == 0) {
        unknown_3c0.Clear();
        return;
    }
    for (int index = unknown_3c0.Count() - 1; index >= 0; index--) {
        AbstractClass *target = unknown_3c0[index];
        bool found = false;
        for (int k = 0; k < unknown_3a8.Count(); k++) {
            if (target == unknown_3a8[k]) {
                found = true;
                break;
            }
        }
        if (!found) {
            unknown_3c0.Delete(index);
        }
    }
    DynamicVectorClass<AbstractClass *> targets;
    DynamicVectorClass<int> values;
    int fired = unknown_3c0.Count();
    if (fired > 0) {
        int count = unknown_3a8.Count();
        for (int i = 0; i < count; i++) {
            AbstractClass *target = unknown_3a8[i];
            bool found = false;
            for (int k = 0; k < fired; k++) {
                if (target == unknown_3c0[k]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                targets.Add(target);
                values.Add(unknown_390[i]);
            }
        }
    }
    if (targets.Count() == 0) {
        unknown_3c0.Clear();
        int best = 0;
        AbstractClass *best_target = 0;
        for (int i = 0; i < unknown_3a8.Count(); i++) {
            if (unknown_390[i] > best) {
                best = unknown_390[i];
                best_target = unknown_3a8[i];
            }
        }
        unknown_slot220(best_target);
    } else {
        int best = 0;
        AbstractClass *best_target = 0;
        for (int i = 0; i < targets.Count(); i++) {
            AbstractClass *target = targets[i];
            if (values[i] > best) {
                best_target = target;
                best = values[i];
            }
        }
        unknown_slot220(best_target);
    }
}


// int TechnoClass::unknown_6d46b0(int mission) (0x006D46B0)
// Whether a mission is neither 5 nor 6 (hypothesis: neither Guard nor
// Sticky). Never called: inlined at every use; owner and parameter are
// hypotheses.

int TechnoClass::unknown_6d46b0(int mission)
{
    if (mission != 5 && mission != 6) {
        return 1;
    }
    return 0;
}


// bool TechnoClass::unknown_slot209(CoordStruct const *coord, int arg2) (0x006D46D0)
// Scans for a target (hypothesis: Westwood's Target_Something_Nearby):
// records the frame (+0x3F4), restarts the +0x148 timer with
// GuardAreaTargetingDelay (mission 11) or NormalTargetingDelay plus 0-2
// frames, drops an unreachable current target (slot 218 results 6, 5 and
// 8), and without a target takes slot 219's pick: DistributedFire types
// spread (0x006D4400), others assign it and, for a techno and an accurate
// projectile, book the expected damage on its +0x70. Whether it has a
// target now.

bool TechnoClass::unknown_slot209(CoordStruct const *coord, int arg2)
{
    unknown_3f4 = Frame;
    if (Mission == 11) {
        int random = unknown_id_source_a3d290->Random()(0, 2);
        unknown_148 = Rules->GuardAreaTargetingDelay + random;
    } else {
        int random = unknown_id_source_a3d290->Random()(0, 2);
        unknown_148 = Rules->NormalTargetingDelay + random;
    }
    int method = arg2 & 3;
    if (unknown_228 != 0 && unknown_404) {
        int weapon = unknown_slot174(unknown_228);
        int result = unknown_slot218(unknown_228, weapon, 1);
        if (result == 6 && SpawnManager != 0) {
            SpawnManager->Abandon_Target();
        } else if (result == 6 || result == 5 || result == 8) {
            unknown_slot220(0);
        }
    }
    if (unknown_228 == 0) {
        AbstractClass *target = unknown_slot219(method, coord, false);
        if (Techno_Type_Class()->DistributedFire) {
            unknown_6d4400();
        } else {
            if (target != 0) {
                unknown_slot220(target);
            }
            int weapon = unknown_slot174(unknown_228);
            TechnoClass *techno = (TechnoClass *)target;
            if (techno != 0) {
                techno = techno->AbstractFlags.IsTechno ? techno : 0;
            }
            WeaponTypeClass *type = unknown_slot231(weapon)->WeaponType;
            if (techno != 0 && type != 0 && !type->Projectile->Inaccurate) {
                techno->unknown_70 -= unknown_6ca6c0(techno, type);
            }
        }
    }
    return unknown_228 != 0;
}


class Leaf_6d4880 { public: unsigned int get(unsigned int a1); };
unsigned int Leaf_6d4880::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int zero; unsigned int spare; zero = 0; return zero; }


// bool TechnoClass::unknown_slot257() (0x006D4890, slot 257)
// Whether the timer at +0x130 has expired.

bool TechnoClass::unknown_slot257()
{
    return unknown_130.Value() == 0;
}


// void TechnoClass::unknown_slot259(int arg1) (0x006D48D0, slot 259)
// Empty.

void TechnoClass::unknown_slot259(int arg1)
{
}


// void TechnoClass::Assign_Destination(AbstractClass *target, bool immediate) (0x006D48E0, slot 260)
// Empty.

void TechnoClass::Assign_Destination(AbstractClass *target, bool immediate)
{
}


// bool TechnoClass::unknown_slot261(bool arg1, bool arg2) (0x006D48F0, slot 261)
// Calls 0x00615360 when slot 279 is false and slot 242 true; returns false.

bool TechnoClass::unknown_slot261(bool arg1, bool arg2)
{
    if (!unknown_slot279() && unknown_slot242()) {
        unknown_615360();
    }
    return false;
}


// The free Intersect expanded here while every other row of this unit calls 0x0041FDF0 out of line, so
// shared/rect_intersect.hpp stays out of the unit (its definition would expand them too).
static inline Rect Expanded_Intersect(Rect const &rect1, Rect const &rect2, int *x = 0, int *y = 0)
{
    Rect empty(0, 0, 0, 0);
    Rect result = rect2;
    if (!rect1.Is_Valid() || !rect2.Is_Valid()) {
        return empty;
    }
    if (result.X < rect1.X) {
        result.Width += result.X - rect1.X;
        result.X = rect1.X;
    }
    if (result.Width < 1) {
        return empty;
    }
    if (result.Y < rect1.Y) {
        result.Height += result.Y - rect1.Y;
        result.Y = rect1.Y;
    }
    if (result.Height < 1) {
        return empty;
    }
    if (result.X + result.Width > rect1.X + rect1.Width) {
        result.Width = rect1.X - result.X + rect1.Width;
    }
    if (result.Width < 1) {
        return empty;
    }
    if (result.Y + result.Height > rect1.Y + rect1.Height) {
        result.Height = rect1.Y + rect1.Height - result.Y;
    }
    if (result.Height < 1) {
        return empty;
    }
    if (x != 0) {
        *x += rect2.X - result.X;
    }
    if (y != 0) {
        *y += rect2.Y - result.Y;
    }
    return result;
}
void TechnoClass::unknown_slot249(Point2D const &point, Point2D const &center, Rect const &window)
{
    Point2D offset(4, 2);
    Point2D xy = point + Point2D(6, -1);
    Class_Of();
    ShapeFileStruct const *pip_shapes = (ShapeFileStruct const *)unknown_pips_shapes_a72fe4;
    Class_Of();
    ShapeFileStruct const *pips2 = (ShapeFileStruct const *)unknown_pips2_shapes_a72fe8;
    bool building = Kind_Of() == 6;
    if (!building) {
        xy = point + Point2D(-5, 0);
        offset = Point2D(4, 0);
        pip_shapes = pips2;
    }
    if (Kind_Of() == 6 && Techno_Type_Class()->PipScale == 2) return;
    int spawns = Techno_Type_Class()->SpawnsNumber;
    if (spawns > 0) {
        int docked = SpawnManager->Docked_Count();
        for (int i = 0; i < spawns; i++) {
            int full = 1;
            int empty = 0;
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, pip_shapes, i < docked ? full : empty, xy + offset * i, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
    }
    if (building) {
        BuildingClass *self = static_cast<BuildingClass *>(this);
        int occupants = self->Type->MaxNumberOccupants;
        int count = self->unknown_4532f0();
        if (self->Type->ShowOccupantPips && occupants > 0 && count > 0) {
            for (int i = 0; i < occupants; i++) {
                int full = 7;
                int empty = 6;
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, pip_shapes, i < count ? full : empty, xy + offset * i, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
    }
    if (Techno_Type_Class()->Passengers > 0) {
        Point2D shift(0, 0);
        int pips = Class_Of()->Max_Pips();
        Techno_Type_Class();
        int *colors = new int[pips];
        for (int c = 0; c < pips; c++) colors[c] = 0;
        int passengers = Cargo.Count;
        int index = Cargo.GetTotalSize() - 1;
        if (index > pips) index = pips;
        ObjectClass *object = Cargo.Attached_Object();
        for (int i = 0; i < passengers && object; i++) {
            if (object->Kind_Of() == 15) {
                colors[index] = static_cast<InfantryClass *>(object)->Type->Pip;
            } else if (object->Kind_Of() == 1) {
                int size = (int)(static_cast<TechnoClass *>(object)->Techno_Type_Class()->Size - 1.0);
                for (int k = size; k > 0; k--) colors[index--] = 4;
                colors[index] = 5;
            } else {
                colors[index] = 1;
            }
            object = object->unknown_30;
            index--;
        }
        int start = 0;
        if (Techno_Type_Class()->Gunner) {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, pip_shapes, colors[0], xy, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            start = 1;
            shift = offset * 2;
        }
        for (int j = start; j < pips; j++) {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, pip_shapes, colors[j], xy + shift + offset * j, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
        delete colors;
    } else {
        int pips = unknown_slot166();
        if (Kind_Of() == 1 && Techno_Type_Class()->PipScale == 2) {
            float gold = unknown_294.Get_Amount(0);
            float gems = unknown_294.Get_Amount(2);
            float ore = unknown_294.Get_Amount(3);
            float other = unknown_294.Get_Amount(1);
            int greenpips = (int)(Techno_Type_Class()->Max_Pips() * ((double)(ore + gems + gold) / Techno_Type_Class()->Storage) + 0.5);
            int bluepips = (int)(Techno_Type_Class()->Max_Pips() * ((double)other / Techno_Type_Class()->Storage) + 0.5);
            for (int i = 0; i < Class_Of()->Max_Pips(); i++) {
                int shape = 0;
                if (bluepips > 0) {
                    shape = 5;
                    bluepips--;
                } else if (greenpips > 0) {
                    shape = 1;
                    greenpips--;
                }
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, pip_shapes, shape, xy + offset * i, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        } else if (Techno_Type_Class()->PipScale == 1) {
            int wrap = Techno_Type_Class()->PipWrap;
            if (wrap == 0) {
                int left = pips;
                for (int i = 0; i < Class_Of()->Max_Pips(); i++) {
                    if (left > 0) {
                        left--;
                        CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, pips2, 12, xy + offset * i - Point2D(0, 3), window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    }
                }
            } else {
                int rows = Class_Of()->Max_Pips() / wrap;
                for (int i = 0; i < wrap; i++) {
                    int frame = 13;
                    for (int j = rows - 1; j >= 0; j--) {
                        if (i + j * wrap < unknown_254) {
                            frame = j + 14;
                            break;
                        }
                    }
                    CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, pips2, frame, xy + offset * i - Point2D(0, 3), window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                }
            }
        }
    }
    if (Kind_Of() != 6) unknown_slot251(point + Point2D(-10, 10), point, window);
    if (unknown_1ac >= 0 && unknown_1ac < 10) {
        int group = unknown_1ac + 1;
        if (group == 10) group = 0;
        int yval = Kind_Of() == 15 ? -36 : -33;
        wchar_t text[10];
        swprintf(text, L"%d", group < 10 ? group : 0);
        Point2D where = point + Point2D(-4, yval - 3);
        Rect box = Text_Rect(text, where.X, where.Y, 0x49, 2, -2);
        box = Expanded_Intersect(box, window);
        box.X--;
        box.Width++;
        RGBClass color = House->unknown_55e9;
        int pixel = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
        unknown_surface_83995c->Fill_Rect(box, 0);
        unknown_surface_83995c->Draw_Rect(box, pixel);
        unknown_scheme_text_print_498050(text, unknown_surface_83995c, &window, &where, pixel, 0, 0x49, -1, 1);
    }
}


// void TechnoClass::unknown_slot250(Point2D const &point, Rect const &window) (0x006D5440)
// Draws the rank pip (PIPS.SHP frame 13, 14 or 18 by the +0x118 ranks)
// near point; infantry use a smaller offset.

void TechnoClass::unknown_slot250(Point2D const &point, Rect const &window)
{
    ObjectTypeClass *type = Class_Of();
    void const *pips = unknown_pips_shapes_a72fe4;
    int frame = -1;
    if (unknown_118.Is_Veteran()) {
        frame = 13;
    }
    if (unknown_118.Is_Elite()) {
        frame = 14;
    }
    if (unknown_118.Is_Dumbass()) {
        frame = 18;
    }
    if (frame != -1) {
        Point2D position = Point2D(point.X + 5, point.Y + 2);
        if (Kind_Of() != 15) {
            position.X += 5;
            position.Y += 4;
        }
        CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)pips, frame, position, window,
                      0xE00, 0, -2, 0, 1000, 0, 0, Point2D(0, 0));
    }
}


// Draw power figures and the primary-factory label over the object.
void TechnoClass::unknown_slot251(Point2D const &point,Point2D const &center,Rect const &window)
{
    if(Kind_Of()==6 && static_cast<BuildingClass *>(this)->Type->Power>0) {
        if(!unknown_32b) {
            wchar_t text[128];
            swprintf(text,StringTable::Load_String("TXT_POWER_DRAIN2",0,"D:\\Ra2\\Techno.CPP",0x3517),House->Power_Output(),House->Power_Drain());
            Rect background=Text_Rect(text,center.X,center.Y,0x149,4,2);
            background=background.Intersect(window);
            RGBClass const &color=House->unknown_55e9;
            unsigned pixel=Build_Hicolor_Pixel(color.Red,color.Green,color.Blue);
            unknown_surface_83995c->Fill_Rect(background,0);
            unknown_surface_83995c->Draw_Rect(background,pixel);
            Simple_Text_Print(text,unknown_surface_83995c,&window,const_cast<Point2D *>(&center),pixel,0,0x149,1);
        }
    }
    if(unknown_32b) {
        wchar_t const *label=StringTable::Load_String("TXT_PRIMARY",0,"D:\\Ra2\\Techno.CPP",0x352a);
        if(Kind_Of()==6 && static_cast<BuildingClass *>(this)->Type->Width()==1) label=StringTable::Load_String("TXT_PRI",0,"D:\\Ra2\\Techno.CPP",0x352c);
        Rect background=Text_Rect(label,center.X,center.Y,0x149,4,2);
        background=background.Intersect(window);
        RGBClass const &color=House->unknown_55e9;
        unsigned pixel=Build_Hicolor_Pixel(color.Red,color.Green,color.Blue);
        unknown_surface_83995c->Fill_Rect(background,0);
        unknown_surface_83995c->Draw_Rect(background,pixel);
        Simple_Text_Print(label,unknown_surface_83995c,&window,const_cast<Point2D *>(&center),pixel,0,0x149,1);
    }
}


// Cell TechnoClass::unknown_slot180(int arg1) (0x006D5800, slot 180)
// The cell of the slot 41 coordinate.

Cell TechnoClass::unknown_slot180(int arg1)
{
    return Coord_Cell(unknown_slot41(arg1));
}


// int TechnoClass::unknown_slot167() (0x006D5850, slot 167)
// Slot 46 of the techno type for the owner.

int TechnoClass::unknown_slot167()
{
    return Techno_Type_Class()->unknown_slot46(House, 0);
}


// int TechnoClass::unknown_6d5870() (0x006D5870)
// For an anti-air secondary weapon (slot 230): +0xA4 times the warhead's
// Verses[3] times +0xB4 over +0xB0 (doubled for two-shooter types), / 50.

int TechnoClass::unknown_6d5870()
{
    if (unknown_slot163()) {
        WeaponTypeClass *weapon = unknown_slot230()->WeaponType;
        if (weapon->Projectile->AA) {
            double damage = weapon->unknown_a4 * weapon->Warhead->Verses[3];
            int value = (int)(damage * weapon->unknown_b4 / weapon->unknown_b0);
            if (Techno_Type_Class()->Is_Two_Shooter()) {
                value += value;
            }
            return value / 50;
        }
    }
    return 0;
}


// int TechnoClass::unknown_6d5900() (0x006D5900)
// For a ground-capable (AG) secondary weapon (slot 230): +0xA4 times the
// warhead's Verses[4] times the range (+0xB4, at most 1024) over +0xB0,
// doubled for two-shooter types and halved for Inaccurate projectiles, / 50.

int TechnoClass::unknown_6d5900()
{
    if (unknown_slot163() && unknown_slot230()->WeaponType->Projectile->AG) {
        WeaponTypeClass *weapon = unknown_slot230()->WeaponType;
        int range = weapon->unknown_b4;
        BulletTypeClass *projectile = weapon->Projectile;
        WarheadTypeClass *warhead = weapon->Warhead;
        if (range >= 1024) {
            range = 1024;
        }
        double damage = weapon->unknown_a4 * warhead->Verses[4];
        int value = (int)(damage * range / weapon->unknown_b0);
        if (Techno_Type_Class()->Is_Two_Shooter()) {
            value += value;
        }
        if (projectile->Inaccurate) {
            value /= 2;
        }
        return value / 50;
    }
    return 0;
}


// int TechnoClass::unknown_6d59d0() (0x006D59D0)
// For a ground-capable (AG) secondary weapon (slot 230): +0xA4 times the
// warhead's Verses[0] times the range (+0xB4, at most 1024) over +0xB0,
// doubled for two-shooter types and halved for Inaccurate projectiles, / 50.

int TechnoClass::unknown_6d59d0()
{
    if (unknown_slot163() && unknown_slot230()->WeaponType->Projectile->AG) {
        WeaponTypeClass *weapon = unknown_slot230()->WeaponType;
        int range = weapon->unknown_b4;
        BulletTypeClass *projectile = weapon->Projectile;
        WarheadTypeClass *warhead = weapon->Warhead;
        if (range >= 1024) {
            range = 1024;
        }
        double damage = weapon->unknown_a4 * warhead->Verses[0];
        int value = (int)(damage * range / weapon->unknown_b0);
        if (Techno_Type_Class()->Is_Two_Shooter()) {
            value += value;
        }
        if (projectile->Inaccurate) {
            value /= 2;
        }
        return value / 50;
    }
    return 0;
}


// void TechnoClass::Look(bool arg1, bool arg2) (0x006D5AA0, slot 70)
// Reveals the map around the object (0x00549C50) when it has sight (+0x32D),
// except for a MultiplayPassive owner outside game mode 0. The height level
// (Z / LeptonsPerSightIncrease, times 10) is kept in +0x378; rising above the
// last level clears arg1. The radius is the type's Sight scaled by
// 1 + level/100, times VeteranSight for a veteran or elite with the sight
// ability when that factor is set.

extern int unknown_game_mode_a3d298;    // 0x00A3D298

void TechnoClass::Look(bool arg1, bool arg2)
{
    if (!unknown_32d) {
        return;
    }
    if (House->Type->MultiplayPassive && unknown_game_mode_a3d298 != 0) {
        return;
    }
    int level = CoordStruct(Coord).Z / Rules->unknown_1324 * 10;
    if (level > unknown_378) {
        arg1 = false;
    }
    unknown_378 = level;
    int sight = (int)(Techno_Type_Class()->Sight * (unknown_378 * 0.01 + 1.0));
    if (unknown_118.Is_Veteran() || unknown_118.Is_Elite()) {
        TechnoTypeClass *type = Techno_Type_Class();
        if ((unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[5])
            || (unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[5] || type->EliteAbilities.Ability[5]))) {
            if (Rules->unknown_5a0 != 0.0) {
                sight = (int)(sight * Rules->unknown_5a0);
            }
        }
    }
    if (sight != 0) {
        Map.Unknown_00549C50(CoordStruct(Coord), sight, House, arg1, arg2, false, true, true);
    }
}


// void TechnoClass::unknown_slot262(bool arg1, bool arg2, bool arg3, HouseClass *house) (0x006D5C30, slot 262)
// Like slot 70, but records the sight for slot 263: when the object has
// sight (+0x32D) and the owner is not MultiplayPassive, the height level
// (Z / LeptonsPerSightIncrease, times 10) is kept in +0x378 (rising clears
// arg1), the radius is Sight scaled by 1 + level/100 and VeteranSight; the
// first time (+0x1E4 clear) it stores the coordinate and radius in +0x1E8 /
// +0x1F4 and reveals for house (when arg3 asks) or the owner, later only
// for house when arg3 asks.

void TechnoClass::unknown_slot262(bool arg1, bool arg2, bool arg3, HouseClass *house)
{
    if (!unknown_32d || House->Type->MultiplayPassive) {
        return;
    }
    int level = CoordStruct(Coord).Z / Rules->unknown_1324 * 10;
    if (level > unknown_378) {
        arg1 = false;
    }
    unknown_378 = level;
    int sight = (int)(Techno_Type_Class()->Sight * (unknown_378 * 0.01 + 1.0));
    if (unknown_118.Is_Veteran() || unknown_118.Is_Elite()) {
        TechnoTypeClass *type = Techno_Type_Class();
        if ((unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[5])
            || (unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[5] || type->EliteAbilities.Ability[5]))) {
            if (Rules->unknown_5a0 != 0.0) {
                sight = (int)(sight * Rules->unknown_5a0);
            }
        }
    }
    if (!unknown_1e4) {
        unknown_1e4 = true;
        unknown_1e8 = Coord;
        unknown_1f4 = sight;
        if (sight != 0) {
            if (arg3 && house != 0) {
                Map.Sight_From(CoordStruct(Coord), sight, house, arg1, arg2, 0, true, 0);
            } else {
                Map.Sight_From(CoordStruct(Coord), sight, House, arg1, arg2, 0, true, 0);
            }
        }
    } else if (arg3 && sight != 0 && house != 0) {
        Map.Sight_From(CoordStruct(Coord), sight, house, arg1, arg2, 0, true, 0);
    }
}


// void TechnoClass::unknown_slot263(bool arg1, bool arg2, bool arg3, HouseClass *house) (0x006D5E90)
// Reveals the map around the stored sight coordinate +0x1E8 (range +0x1F4)
// when +0x32D is set and the owner is not MultiplayPassive: once for the
// owner while +0x1E4 is pending (clearing it), or for house when arg3 asks.

void TechnoClass::unknown_slot263(bool arg1, bool arg2, bool arg3, HouseClass *house)
{
    if (unknown_32d && !House->Type->MultiplayPassive) {
        if (unknown_1e4) {
            unknown_1e4 = false;
            if (unknown_1f4 != 0 && arg3 && house != 0) {
                Map.Sight_From(unknown_1e8, unknown_1f4, house, arg1, arg2, 0, true, 1);
            } else if (unknown_1f4 != 0) {
                Map.Sight_From(unknown_1e8, unknown_1f4, House, arg1, arg2, 0, true, 1);
            }
        } else if (arg3 && unknown_1f4 != 0 && house != 0) {
            Map.Sight_From(unknown_1e8, unknown_1f4, house, arg1, arg2, 0, true, 1);
        }
    }
}


// Not exact (RA2_NON_MATCHING): TechnoClass::unknown_slot223 at 0x006D5F40, best score 63.62, 2026-11-13; differs: 218 instructions in the original, 219 in the draft (shape 98.86)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x6D5F40 FUN_006d5f40
// entry 0x6D5F40; exact-match score 96.38 (misses_index best), this text 64.37; plan "draft as-is"
// origin lane: Scout767 (build/class-work/Scout767/w/keep/rk5.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): VECTOR3_DECLARE_WWMATH inlines spelled out; each changed line is marked NATIVE.
// Rocking from an impact (TechnoClass slot 223, 0x006D5F40).
// NATIVE: VECTOR3_DECLARE_WWMATH is off in the host group (vector3.hpp is already included); its two inlines are spelled out below.
#include "shared/unknown_pair_first.hpp"
inline Vector3 Normalize(Vector3 const &vector)
{
    float len=Fast_Sqrt(vector.X * vector.X + vector.Y * vector.Y + vector.Z * vector.Z);   // NATIVE: Length2()
    if (len!=0.0) {
        return Vector3(vector.X / len, vector.Y / len, vector.Z / len);   // NATIVE: operator/
    }
    return vector;
}
inline double Rock_Length(Vector3 const &v)
{
    double z = v.Z;
    double y = v.Y;
    double x = v.X;
    return Fast_Sqrt(x * x + y * y + z * z);
}
void TechnoClass::unknown_slot223(CoordStruct const &coord, float force)
{
    if (Techno_Type_Class()->unknown_b0.unknown_00 && !Techno_Type_Class()->unknown_b0.unknown_00->LoadFailed) {
        CoordStruct pos=Coord;
        Vector3 vec1;
        vec1.X=pos.X-coord.X;
        vec1.Y=coord.Y-pos.Y;
        vec1.Z=pos.Z-coord.Z;
        Vector3 vec2=vec1;

        float theta=unknown_2e0.Current().As_Radian();
        float sangle=Fast_Sin(theta);
        float cangle=Fast_Cos(theta);

        float dist1=Rock_Length(vec1);

        float scale=(0.04f-dist1*0.000025f)*force/Techno_Type_Class()->Weight;

        if (fabs(dist1)>=0.00002 && scale>=0.01f) {
            if (scale>0.05f) {
                scale=0.05f;
            }

            vec2.Z=0.0;
            vec1=Normalize(vec2);

            dist1=vec1.X*cangle+vec1.Y*sangle;
            vec2.X=-(vec1.Z*sangle);
            vec2.Y=+(vec1.Z*cangle);
            vec2.Z=(vec1.X*sangle)-(vec1.Y*cangle);
            double dist2=Rock_Length(vec2);
            if (fabs(dist1*cangle-dist2*sangle-vec1.X)>0.0002 || fabs(dist2*cangle+dist1*sangle-vec1.Y)>0.0002) {
                dist2=-dist2;
            }
            unknown_28c=dist1*scale/2.0f;
            unknown_288=-((float)dist2*scale);
        }
    }
}
#endif

// void TechnoClass::unknown_slot237() (0x006D6230)
// Tilt update: the pitch (+0x280) and roll (+0x284) angles follow their
// rates (+0x288, +0x28C), clamped to a quarter turn (a tenth of a turn of
// roll for a foot with +0x59F); the rates decay back towards level and
// an angle that crosses level stops there.

void TechnoClass::unknown_slot237()
{
    bool pitch_up = unknown_280 > 0.00002;
    bool pitch_down = unknown_280 < -0.00002;
    if (unknown_325) {
        float roll = unknown_284 < 0.0f ? -unknown_284 : unknown_284;
        if (roll < 0.7853982f) {
            int facing = Dir_To_8(unknown_2e0.Current());
            if (facing != 0 && (facing <= 5 || facing > 7)) {
                unknown_284 += 0.01;
            } else {
                unknown_284 -= 0.01;
            }
        }
        return;
    }
    if (unknown_37d) {
        unknown_284 += unknown_28c;
        unknown_280 += unknown_288;
        return;
    }

    if (unknown_288 != 0.0f) {
        unknown_280 += unknown_288;
        if (unknown_280 > 0.7853982f) {
            unknown_280 = 0.7853982f;
            unknown_288 = 0.0f;
        } else if (unknown_280 < -0.7853982f) {
            unknown_280 = -0.7853982f;
            unknown_288 = 0.0f;
        }
        if (pitch_up) {
            if (unknown_288 > 0.0f) {
                unknown_288 -= 0.002f;
            } else {
                unknown_288 -= 0.002f * 2.5;
            }
        } else {
            if (unknown_288 < 0.0f) {
                unknown_288 += 0.002f;
            } else {
                unknown_288 += 0.002f * 2.5;
            }
        }
    } else {
        unknown_280 = 0.0f;
    }
    if ((pitch_up && unknown_280 < 0.00002) || (pitch_down && unknown_280 > -0.00002)) {
        unknown_288 = 0.0f;
        unknown_280 = 0.0f;
    }

    bool roll_up = unknown_284 > 0.00002;
    bool roll_down = unknown_284 < -0.00002;
    if (unknown_28c != 0.0f) {
        float roll = unknown_28c + unknown_284;
        unknown_284 = roll;
        float limit = 0.7853982f;
        bool foot;
        if (this != 0) foot = AbstractFlags.IsFoot;
        if (this != 0 && foot && ((FootClass *)this)->unknown_59f) {
            limit = 0.31415927f;
        }
        if (roll > limit) {
            unknown_284 = limit;
            unknown_28c = 0.0f;
        } else if (roll < -limit) {
            unknown_284 = -limit;
            unknown_28c = 0.0f;
        }
        if (roll_up) {
            if (unknown_28c > 0.0f) {
                unknown_28c -= 0.002f;
            } else {
                unknown_28c -= 0.002f * 2.5;
            }
        } else {
            if (unknown_28c < 0.0f) {
                unknown_28c += 0.002f;
            } else {
                unknown_28c += 0.002f * 2.5;
            }
        }
    } else {
        unknown_284 = 0.0f;
    }
    if ((roll_up && unknown_284 < 0.00002) || (roll_down && unknown_284 > -0.00002)) {
        unknown_28c = 0.0f;
        unknown_284 = 0.0f;
    }
}


// CoordStruct TechnoClass::unknown_6d65b0() (0x006D65B0)
// Where to aim at the target (+0x228): its slot 22 coordinate, led ahead
// along a moving unit's facing by its speed times the projectile's flight
// time (distance over 0.9 of the secondary weapon's speed for the range).

inline double UnknownFacingDir::As_Radian() const
{
    return (short(Raw) - 0x3FFF) * -9.587672516830327e-05;
}

// A coordinate moved along a direction (same helper as InfantryClass 0x00504BC0).
inline CoordStruct Move_Coord(CoordStruct const &start, UnknownFacingDir const &direction, int distance)
{
    double angle = direction.As_Radian();
    double scaled = distance;
    return CoordStruct(int(start.X + Fast_Cos(angle) * scaled), int(start.Y - Fast_Sin(angle) * scaled), start.Z);
}
CoordStruct TechnoClass::unknown_6d65b0()
{
    CoordStruct coord = unknown_geometry_l;
    if (unknown_228 != 0) {
        coord = unknown_228->unknown_slot22();
        if (unknown_228 != 0 && unknown_228->Kind_Of() == 1) {
            FootClass *unit = (FootClass *)unknown_228;
            if (unit != 0 && unit->Locomotor.Checked()->Is_Moving()) {
                int speed = unit->unknown_slot303();
                int distance = unknown_5d4c00(unknown_228);
                WeaponStruct *weapon = unknown_slot230();
                if (weapon != 0 && weapon->WeaponType != 0) {
                    double range = distance;
                    int lead = (int)(range / (weapon->WeaponType->Get_Speed(distance) * 0.9) * speed);
                    coord = Move_Coord(coord, unit->unknown_2e0.Current(), lead);
                }
            }
        }
    }
    return coord;
}


// int TechnoClass::unknown_6d6750() (0x006D6750)
// The ID plus unknown_1e0, modulo 400.

int TechnoClass::unknown_6d6750()
{
    return (Fetch_ID() + (int)unknown_1e0) % 400;
}


// bool TechnoClass::unknown_slot157() (0x006D6780, slot 157)
// Whether a self-healing techno (the type's SelfHealing or rank ability 9)
// heals this frame: on every interval of the rules' +0x1348 minutes when
// it is damaged but alive.

bool TechnoClass::unknown_slot157()
{
    if (!Techno_Type_Class()->SelfHealing && !unknown_6d7910(9)) {
        return false;
    }
    if (Frame % (int)(Rules->unknown_1348 * 900.0) != 0) {
        return false;
    }
    int strength = Strength;
    if (strength == Techno_Type_Class()->MaxStrength) {
        return false;
    }
    return strength != 0 ? true : false;
}


// HRESULT __stdcall TechnoClass::Load(IStream *stream) (0x006D6850, slot 5)
// RadioClass::Load, then pointer fixups, the 0x00AC0BF8 list entry and the audio controller.

HRESULT __stdcall TechnoClass::Load(IStream *stream)
{
    HRESULT result = RadioClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    unknown_408 = 0;
    Swizzler.Swizzle((void **)&House);
    for (int index = 0; index < 8; index++) {
        Swizzler.Swizzle((void **)&unknown_25c[index]);
    }
    Swizzler.Swizzle((void **)&unknown_1b0);
    Swizzler.Swizzle((void **)&unknown_3f8);
    Swizzler.Swizzle((void **)&unknown_228);
    Swizzler.Swizzle((void **)&unknown_22c);
    Swizzler.Swizzle((void **)&unknown_230);
    Swizzler.Swizzle((void **)&unknown_384);
    Swizzler.Swizzle((void **)&SpawnManager);
    Swizzler.Swizzle((void **)&unknown_234);
    Swizzler.Swizzle((void **)&unknown_23c);
    Swizzler.Swizzle((void **)&unknown_40c);
    Swizzler.Swizzle((void **)&unknown_410);
    Swizzler.Swizzle((void **)&unknown_208);
    Swizzler.Swizzle((void **)&unknown_20c);
    Swizzler.Swizzle((void **)&Cargo.Object);
    Swizzler.Swizzle((void **)&unknown_27c);
    if (unknown_108) {
        unknown_abstracts_ac0bf8.Delete(this);
        unknown_abstracts_ac0bf8.Add(this);
    }
    Swizzler.Swizzle((void **)&unknown_108);
    unknown_3d8.Init();
    unknown_3ec = 1;
    unknown_3f0 = 0;
    return S_OK;
}


// HRESULT __stdcall TechnoClass::Save(IStream *stream, BOOL clear_dirty) (0x006D6A80, slot 6)
// RadioClass::Save; always S_OK.

HRESULT __stdcall TechnoClass::Save(IStream *stream, BOOL clear_dirty)
{
    RadioClass::Save(stream, clear_dirty);
    return S_OK;
}


// void TechnoClass::Compute_CRC(WWCRCEngine &crc) const (0x006D6AA0, slot 13)
// RadioClass's part, then the techno's state: indices, factors, timers'
// remaining time, flags (the player-visibility flags only in game modes 0 and
// 5), the height level, and the +0x3A8 objects (with their +0x390 values)
// and +0x3C0 objects by ID.

extern int unknown_game_mode_a3d298;    // 0x00A3D298

void TechnoClass::Compute_CRC(WWCRCEngine &crc) const
{
    RadioClass::Compute_CRC(crc);
    crc((long)unknown_1ac);
    crc((long)unknown_114);
    crc(unknown_120);
    crc(unknown_128);
    crc(unknown_130.Value());
    crc((long)unknown_1a8);
    crc((long)unknown_1b8);
    crc(unknown_1d4.Value());
    crc(unknown_1e0);
    crc(unknown_1e4);
    crc((long)unknown_1f4);
    crc(unknown_240);
    crc(unknown_244.Value());
    crc((long)unknown_254);
    crc(unknown_194.Value());
    crc((long)unknown_258);
    crc(unknown_280);
    crc(unknown_284);
    crc(unknown_288);
    crc(unknown_28c);
    crc(unknown_325);
    crc(unknown_327);
    crc(unknown_328);
    crc(unknown_329);
    crc(unknown_32a);
    crc(unknown_32b);
    crc(unknown_32c);
    crc(unknown_32d);
    crc(unknown_370);
    crc(unknown_371);
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
        crc(unknown_372);
        crc(unknown_373);
        crc(unknown_374);
    }
    crc(unknown_375);
    crc((long)unknown_3fc);
    crc((long)unknown_376);
    crc((long)unknown_377);
    crc(unknown_378);
    int count = unknown_3a8.Count();
    crc((long)count);
    for (int index = 0; index < count; index++) {
        AbstractClass *object = unknown_3a8[index];
        int value = unknown_390[index];
        crc(object->Fetch_ID());
        crc((long)value);
    }
    count = unknown_3c0.Count();
    crc((long)count);
    for (index = 0; index < count; index++) {
        crc(unknown_3c0[index]->Fetch_ID());
    }
}


// bool TechnoClass::unknown_slot154() (0x006D6DE0, slot 154)
// unknown_32a.

bool TechnoClass::unknown_slot154()
{
    return unknown_32a ? true : false;
}


// bool TechnoClass::unknown_slot111() (0x006D6DF0, slot 111)
// unknown_204.

bool TechnoClass::unknown_slot111()
{
    return unknown_204;
}


// bool TechnoClass::unknown_slot112() (0x006D6E00, slot 112)
// unknown_205.

bool TechnoClass::unknown_slot112()
{
    return unknown_205;
}


// bool TechnoClass::unknown_slot113() (0x006D6E10, slot 113)
// Whether the +0x208 object has a nonzero +0x28.

bool TechnoClass::unknown_slot113()
{
    if (unknown_208 && unknown_208->unknown_28) {
        return true;
    }
    return false;
}


// bool TechnoClass::unknown_slot114() (0x006D6E30, slot 114)
// Neither unknown_204 nor unknown_205.

bool TechnoClass::unknown_slot114()
{
    if (!unknown_204 && !unknown_205) {
        return true;
    }
    return false;
}


// void TechnoClass::unknown_6d6e50(AbstractClass *value) (0x006D6E50)
// Sets unknown_1b0.

void TechnoClass::unknown_6d6e50(AbstractClass *value)
{
    unknown_1b0 = value;
}


// int TechnoClass::unknown_slot164() (0x006D6E60, slot 164)
// 0 when the tactical view shows the coordinate in the techno's own cell
// (slot 104), else 1.

int TechnoClass::unknown_slot164()
{
    CoordStruct coord = Coord;
    Cell shown = ((UnknownTacticalView *)unknown_tactical_83996c)->Unknown_006A6A50(coord);
    if (shown == unknown_slot104()) {
        return 0;
    }
    return 1;
}


// CoordStruct TechnoClass::Railgun_Beam_Damage(CoordStruct const &coord, AbstractClass *abstract, WeaponTypeClass *weapon) (0x006D6ED0)
// Damages what stands near the beam from coord to the center of abstract (Rules radius, abstract always) and
// returns where the beam stopped: blocking ground, which may collapse a cliff, else the center (TibSun
// Railgun_Beam_Damage). `techno` is left unset on a null object, the shared form of this flag test (0x00471660).

CoordStruct __fastcall Lerp(CoordStruct const &from, CoordStruct const &to, float t);   // 0x007220C0

inline float Vector_Length(Vector3 const &v)
{
    double y = v.Y;
    double z = v.Z;
    double x = v.X;
    return (float)Fast_Sqrt(x * x + y * y + z * z);
}

inline bool Percent_Chance(int percent)
{
    return Random_Pick(0, 99) < percent;
}

inline Vector3 operator-(Vector3 const &a, Vector3 const &b)
{
    return Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}

inline float operator*(Vector3 const &a, Vector3 const &b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

CoordStruct TechnoClass::Railgun_Beam_Damage(CoordStruct const &coord, AbstractClass *abstract, WeaponTypeClass *weapon)
{
    CoordStruct center = abstract->Center_Coord();
    bool target_found = false;
    Cell final_cell = Cell(coord);
    Vector3 start(coord.X, coord.Y, coord.Z);
    Vector3 end(center.X, center.Y, center.Z);
    CoordStruct final_impact = coord;
    Vector3 delta = end - start;
    DynamicVectorClass<ObjectClass *> targets;

    for (int i = 0; i < 50; i++) {
        final_impact = Lerp(coord, center, i * (1.0f / 49));
        if (Cell(final_impact) != final_cell) {
            final_cell = Cell(final_impact);
            CellClass *cptr = &Map[final_cell];
            ObjectClass *optr;
            if (cptr->unknown_140 & 0x100 && final_impact.Z >= (cptr->unknown_11b + 4) * unknown_geometry_g) {
                optr = cptr->AltOccupierPtr;
            } else {
                optr = cptr->OccupierPtr;
            }
            while (optr != 0) {
                if (optr != this) {
                    if (optr == abstract) {
                        target_found = true;
                    }
                    bool techno;
                    if (optr != 0) {
                        techno = optr->AbstractFlags.IsTechno;
                    }
                    if (optr != 0 && techno) {
                        float distance = 0.0f;
                        if (optr->Kind_Of() != 6) {
                            CoordStruct obj_coord = optr->Coord;
                            Vector3 obj(obj_coord.X, obj_coord.Y, obj_coord.Z);
                            Vector3 off = obj - start;
                            Vector3 cross = Vector3::Cross_Product(off, delta);
                            Vector3 perp = Vector3::Cross_Product(cross, delta);
                            distance = (float)fabs((perp * off) / Vector_Length(perp));
                        }
                        if (distance < Rules->RailgunDamageRadius || optr == abstract) {
                            if (targets.Count() == 0 || targets.ID(optr) == -1) {
                                targets.Add(optr);
                            }
                        }
                    }
                }
                optr = optr->unknown_30;
            }
            if (Map.Get_Height_GL(final_impact) > final_impact.Z) {
                if (Map[final_impact].Is_Tile_Destroyable_Cliff()) {
                    if (Percent_Chance(Rules->unknown_140c)) {
                        Map.Collapse_Cliff(&Map[final_impact]);
                    }
                }
                return final_impact;
            }
        }
    }
    if (!target_found && abstract != 0 && abstract->AbstractFlags.IsObject) {
        ObjectClass *obj = (ObjectClass *)abstract;
        if (targets.Count() == 0 || targets.ID(obj) == -1) {
            targets.Add(obj);
        }
    }
    for (int j = 0; j < targets.Count(); j++) {
        int damage = weapon->unknown_98;
        targets[j]->Take_Damage(&damage, 0, weapon->Warhead, this, false, 0);
    }
    if (abstract->Kind_Of() == 11 && ((CellClass *)abstract)->Is_Tile_Destroyable_Cliff()) {
        if (Percent_Chance(Rules->unknown_140c)) {
            Map.Collapse_Cliff((CellClass *)abstract);
        }
    }
    return center;
}


// void TechnoClass::unknown_slot264() (0x006D74D0, slot 264)
// Map call 0x00631910 with +0x1A0/+0x1A4; sets unknown_37b.

void TechnoClass::unknown_slot264()
{
    Map.Radar_Track(this, unknown_1a0);
    unknown_37b = true;
}


// void TechnoClass::unknown_slot265() (0x006D7500, slot 265)
// Map call 0x00631AF0 with +0x1A0/+0x1A4; clears unknown_37b.

void TechnoClass::unknown_slot265()
{
    Map.Radar_Untrack(this, unknown_1a0);
    unknown_37b = false;
}


// void TechnoClass::unknown_slot266() (0x006D7530, slot 266)
// Map call 0x00632620 with &unknown_1a0.

void TechnoClass::unknown_slot266()
{
    Map.Radar_Pixel(unknown_1a0);
}


// double TechnoClass::unknown_6d7550(ObjectClass *target, CoordStruct const &coord) (0x006D7550)
// The threat value of a target (hypothesis: Westwood's Evaluate_Object):
// the type's coefficients (Rules' Dumb* set for a house without +0x143)
// weigh the target's weapon against this object's armor (negated when the
// target is already aiming at this object), its SpecialThreatValue, the
// enemy-house bonus, this object's weapon against the target's armor, the
// target's health and the distance beyond the weapon's range (from coord,
// or from this object when coord is the zero coordinate).

double TechnoClass::unknown_6d7550(ObjectClass *target, CoordStruct const &coord)
{
    TechnoTypeClass *type = Techno_Type_Class();
    if (target->Class_Of() == 0) {
        return unknown_double_79b610;
    }
    double my_effectiveness;
    double target_effectiveness;
    double target_special_threat;
    double target_strength;
    double target_distance;
    if (House->unknown_143) {
        my_effectiveness = type->MyEffectivenessCoefficient;
        target_effectiveness = type->TargetEffectivenessCoefficient;
        target_special_threat = type->TargetSpecialThreatCoefficient;
        target_strength = type->TargetStrengthCoefficient;
        target_distance = type->TargetDistanceCoefficient;
    } else {
        my_effectiveness = Rules->DumbMyEffectivenessCoefficient;
        target_effectiveness = Rules->DumbTargetEffectivenessCoefficient;
        target_special_threat = Rules->DumbTargetSpecialThreatCoefficient;
        target_strength = Rules->DumbTargetStrengthCoefficient;
        target_distance = Rules->DumbTargetDistanceCoefficient;
    }
    int index = unknown_slot174(target);
    WeaponTypeClass *weapon = unknown_slot231(index)->WeaponType;
    double value = 0;
    int kind = target->Kind_Of();
    if ((kind == 6 || kind == 15 || kind == 1 || kind == 2) && target != 0) {
        TechnoClass *techno = (TechnoClass *)target;
        int their_index = techno->unknown_slot174(this);
        WeaponTypeClass *their_weapon = techno->unknown_slot231(their_index)->WeaponType;
        if (their_weapon != 0 && their_weapon->Warhead != 0) {
            if (techno->unknown_228 == this) {
                value = -(target_effectiveness * their_weapon->Warhead->Verses[type->unknown_9c]);
            } else {
                value = target_effectiveness * their_weapon->Warhead->Verses[type->unknown_9c];
            }
        }
        value += target_special_threat * techno->Techno_Type_Class()->SpecialThreatValue;
        if (House->unknown_54f0 != -1 && House->unknown_54f0 == techno->House->ArrayIndex) {
            value += Rules->EnemyHouseThreatBonus;
        }
    }
    if (weapon != 0 && weapon->Warhead != 0) {
        value += my_effectiveness * weapon->Warhead->Verses[target->Class_Of()->unknown_9c];
    }
    value += target->Health_Ratio() * target_strength;
    int range;
    if (weapon != 0) {
        range = weapon->unknown_b4;
    } else {
        range = Techno_Type_Class()->GuardRange.Value;
    }
    range /= 256;
    int distance;
    if (coord != unknown_geometry_l) {
        distance = Offset_Length(coord - target->Center_Coord());
    } else {
        distance = Offset_Length(Center_Coord() - target->Center_Coord()) / 256;
    }
    distance -= range;
    distance = distance < 0 ? 0 : distance;
    value += target_distance * distance;
    return value + 100000.0;
}


// bool TechnoClass::unknown_6d7910(int ability) (0x006D7910)
// Whether the techno's rank grants the type's ability: veterans (0x007133A0)
// the veteran ability, elites (0x00713420) the veteran or elite one.

bool TechnoClass::unknown_6d7910(int ability)
{
    if (unknown_118.Is_Veteran() || unknown_118.Is_Elite()) {
        TechnoTypeClass *type = Techno_Type_Class();
        if (unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[ability]) {
            return true;
        }
        if (unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[ability] || type->EliteAbilities.Ability[ability])) {
            return true;
        }
    }
    return false;
}


// void TechnoClass::unknown_6d7990() (0x006D7990)
// Restarts the timer at 0x00AC0178 with 25 frames.

void TechnoClass::unknown_6d7990()
{
    unknown_timer_ac0178 = 25;
}


// void __fastcall TechnoClass::unknown_6d79c0(bool value) (0x006D79C0)
// Sets the flag at 0x007F7110.

void __fastcall TechnoClass::unknown_6d79c0(bool value)
{
    unknown_bool_7f7110 = value;
}


// int TechnoClass::unknown_slot254(int arg1) (0x006D79D0, slot 254)
// With bit 2 of FlashCount: 2000 up to 1500, else 500; otherwise arg1.

int TechnoClass::unknown_slot254(int arg1)
{
    if ((FlashCount & 2) == 2) {
        return arg1 > 1500 ? 500 : 2000;
    }
    return arg1;
}


// bool TechnoClass::unknown_slot190(int *out_kind) (0x006D7A10, slot 190)
// Whether the object shows on the radar (hypothesis): never for Invisible
// types or while +0x325 is set, always for RadarVisible types; a player-
// controlled object by +0x373. Otherwise visible unless it is hidden in some
// way (a tunnel under shroud, +0x1B8 == 2, low slot 108 value, the rank's
// ability 11, a hidden map position), in which case slot 191 decides and an
// enemy's blip kind is written: 2 below -20, else 1.

bool TechnoClass::unknown_slot190(int *out_kind)
{
    TechnoTypeClass *type = Techno_Type_Class();
    if (type->Invisible || unknown_325) {
        return false;
    }
    if (type->RadarVisible) {
        return true;
    }
    if (House->Is_Player_Control()) {
        return unknown_373 ? true : false;
    }
    int value = Get_Height_AGL();
    bool ability = unknown_6d7910(11);
    bool hidden = Map.Is_Shrouded(CoordStruct(Coord)) && unknown_b263d8 != 0;
    bool tunnel = (unknown_id_source_a3d290->unknown_000 & 0x1000) && Map.Is_Fogged(CoordStruct(Coord));
    if (!tunnel && unknown_1b8 != 2 && value >= -20 && !ability && !hidden) {
        return true;
    }
    if (!unknown_slot191()) {
        return false;
    }
    if (!unknown_player_a35db4->Is_Ally(House) && !ability && !tunnel && !hidden) {
        *out_kind = value < -20 ? 2 : 1;
    }
    return true;
}


// bool TechnoClass::unknown_slot191() (0x006D7C00, slot 191)
// Cell test 0x0047C2E0 for the player at 0x00A35DB4.

bool TechnoClass::unknown_slot191()
{
    if (unknown_player_a35db4) {
        return Map_Cell(Center_Coord())->Is_Sensed(unknown_player_a35db4->ArrayIndex);
    }
    return false;
}


// bool TechnoClass::unknown_slot192(HouseClass *house) (0x006D7C40, slot 192)
// Cell test 0x0047C2E0 for a house.

bool TechnoClass::unknown_slot192(HouseClass *house)
{
    if (house) {
        return Map_Cell(Center_Coord())->Is_Sensed(house->ArrayIndex);
    }
    return false;
}


// void __fastcall Unknown_006D7C80(AbstractClass *target) (0x006D7C80)
// Forgets an object that is going away: every techno that targets it drops
// the target (an aircraft on mission 0x19 also drops +0xA8 and +0x5BA), and
// every team clears its +0x3C and +0x40 references to it.

void __fastcall Unknown_006D7C80(AbstractClass *target)
{
    for (int index = Technos.Count() - 1; index >= 0; index--) {
        TechnoClass *techno = Technos[index];
        if (techno->unknown_228 == target) {
            techno->Restore_Mission();
            if (techno->Kind_Of() == 2 && techno->Mission == 0x19) {
                techno->unknown_a8 = 0;
                ((AircraftClass *)techno)->unknown_5ba = false;
            }
            if (techno->unknown_228 == target) {
                techno->unknown_slot220(0);
            }
        }
    }
    for (int team_index = Teams.Count() - 1; team_index >= 0; team_index--) {
        TeamClass *team = Teams[team_index];
        if (team->unknown_3c == target) {
            team->unknown_3c = 0;
        }
        if (team->unknown_40 == target) {
            team->unknown_40 = 0;
        }
    }
}


// Whether the selected weapon must lob over a steep rise to its target.
// The existing Point2D length implementation, visible inline in this caller;
// any emitted original copy must still pass complete kept-elsewhere closure.
inline int Point2D::Length() const
{
    return (int)Fast_Sqrt((double)X*X+(double)Y*Y);
}

bool TechnoClass::unknown_6d7d20(int weapon)
{
    WeaponTypeClass *type=unknown_slot231(weapon)->WeaponType;
    if(!type) return true;
    bool high_arc=type->unknown_10a;
    if(!high_arc && unknown_228) {
        int height=unknown_228->Center_Coord().Z-Position_Coord_004f3f40().Z;
        if(height>0) {
            // CoordStruct's first two signed32-bit fields are the XY point
            // prefix (reference Coord inherits the point); this storage view
            // keeps the existing shared Point2D constructor/API unchanged.
            if((Point2D(reinterpret_cast<RectPoint2D const &>(static_cast<CoordStruct const &>(Position_Coord_004f3f40())))-Point2D(reinterpret_cast<RectPoint2D const &>(static_cast<CoordStruct const &>(unknown_228->Center_Coord())))).Length()<height) high_arc=true;
        }
    }
    return high_arc;
}


// void TechnoClass::unknown_slot207() (0x006D7E00, slot 207)
// Counts unknown_254 down to 0.

void TechnoClass::unknown_slot207()
{
    if (unknown_254 > 0) {
        unknown_254--;
    }
}


// int TechnoClass::unknown_6d7e20(int extra) (0x006D7E20)
// Fires the death weapon at the techno itself (hypothesis: the explosion
// when it dies): the type's DeathWeapon scaled by DeathWeaponDamageModifier,
// else the primary weapon scaled the same way, else the Rules DeathWeapon
// with half the type's MaxStrength. The damage plus extra goes to a new
// bullet that detonates at the centre; returns that damage, or 0 without a
// weapon.

int TechnoClass::unknown_6d7e20(int extra)
{
    WeaponTypeClass *weapon;
    int damage;
    if (Techno_Type_Class()->DeathWeapon != 0) {
        weapon = Techno_Type_Class()->DeathWeapon;
        damage = weapon->unknown_a4 * Techno_Type_Class()->DeathWeaponDamageModifier;
    } else {
        WeaponStruct *primary = unknown_slot230();
        if (primary != 0 && primary->WeaponType != 0) {
            weapon = unknown_slot230()->WeaponType;
            damage = weapon->unknown_a4 * Techno_Type_Class()->DeathWeaponDamageModifier;
        } else {
            weapon = Rules->DeathWeapon;
            damage = Techno_Type_Class()->MaxStrength * 0.5;
        }
    }
    if (weapon == 0) {
        return 0;
    }
    damage += extra;
    BulletClass *bullet = Create_Bullet(weapon->Projectile, this, this, damage, weapon->Warhead, 0, weapon->unknown_10b);
    if (bullet != 0) {
        bullet->Set_Weapon_Type(weapon);
        bullet->Limbo();
        bullet->unknown_func_462110(Center_Coord());
        bullet->Release();
    }
    return damage;
}


// void TechnoClass::unknown_6d7f30() (0x006D7F30)
// Above the rules' health ratio at +0x1368 (or more than 10 below ground,
// slot 108), slot 60 of the object at +0x268.

void TechnoClass::unknown_6d7f30()
{
    if (Health_Ratio() > Rules->unknown_1368 || Get_Height_AGL() < -10) {
        if (unknown_25c[3] != 0) {
            unknown_25c[3]->Delete_Me();
        }
    }
}


// bool TechnoClass::unknown_6d7f70() (0x006D7F70)
// Follows up the contact at +0x3F8 (hypothesis: a dock or repair partner):
// dropped once it is inactive; when it answers message 2, message 15 decides
// between mission 7 toward it and no mission with message 3, both dropping
// it; an aircraft at a building whose type has UnitReload but not
// UnitRepair sends message 14 instead (FootClass 0x004C984C).

bool TechnoClass::unknown_6d7f70()
{
    if (unknown_3f8 == 0) {
        return false;
    }
    if (((ObjectClass *)unknown_3f8)->unknown_7d) {
        AbstractClass *contact = unknown_3f8;
        if (contact != 0 && contact->AbstractFlags.IsTechno) {
            TechnoClass *techno = (TechnoClass *)contact;
            if (unknown_slot150(2, techno) == 1) {
                if (unknown_slot150(15, techno) == 1) {
                    QueueMission(7, false);
                    Assign_Destination(techno, true);
                    unknown_3f8 = 0;
                    return true;
                }
                QueueMission(-1, false);
                Assign_Destination(0, true);
                unknown_3f8 = 0;
                unknown_slot150(3, techno);
                return true;
            }
            if (techno->Kind_Of() == 6 && Kind_Of() == 2) {
                BuildingTypeClass *type = ((BuildingClass *)techno)->Type;
                if (type->UnitReload && !type->UnitRepair) {
                    unknown_slot150(14, techno);
                }
            }
        }
    } else {
        unknown_3f8 = 0;
    }
    return false;
}


// bool TechnoClass::unknown_6d8080() (0x006D8080)
// With an active, +0x7D object at +0x3F8: heads for the cell 0x006CF130
// picks near it (slot 260, mission 2) and reports true; an inactive one is
// dropped.

bool TechnoClass::unknown_6d8080()
{
    if (unknown_3f8 != 0) {
        if (unknown_3f8->AbstractFlags.IsTechno && ((ObjectClass *)unknown_3f8)->unknown_7d) {
            Cell cell = unknown_6cf130(unknown_3f8);
            if (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y) {
                Assign_Destination((AbstractClass *)&Map[cell], true);
                QueueMission(2, false);
                return true;
            }
        } else {
            unknown_3f8 = 0;
        }
    }
    return false;
}


// void TechnoClass::unknown_slot173() (0x006D8110, slot 173)
// Empty.

void TechnoClass::unknown_slot173()
{
}


// void TechnoClass::unknown_slot267(bool arg1) (0x006D8120, slot 267)
// Keeps the radar point that tracks the object (+0x1A0) current: records
// once whether the map hides the object's position (+0x373), takes the
// radar point of its coordinate (kind 6 keeps the stored point unless
// forced), clips it to the radar surface, and tells slots 264/265 when the
// point appears or moves; while the player's +0x13C timer runs it calls
// slot 266 every Rules FlashFrameTime frames.

void TechnoClass::unknown_slot267(bool arg1)
{
    if (!unknown_373 && unknown_game_mode_a3d298 == 0) {
        unknown_373 = !Map.Is_Shrouded(Center_Coord());
    }
    Point2D point;
    if (Kind_Of() == 6 && !arg1) {
        point = unknown_1a0;
    } else {
        point = Map.Coord_To_Radar_Pixel(CoordStruct(Coord), false);
    }
    int kind = 0;
    bool visible = unknown_slot190(&kind);
    Rect rect = unknown_radar_surface_8336f8->Get_Rect();
    if (kind != 0 && visible) {
        bool foot;
        if (this != 0) {
            foot = AbstractFlags.IsFoot;
        }
        if (this == 0 || !foot || ((FootClass *)this)->unknown_574 < 0) {
            Unknown_0063B5B0(5, Coord_Cell(CoordStruct(Coord)));
        }
    }
    if (!rect.Is_Point_Within(point) && visible) {
        visible = Map.Unknown_0055A740(Get_Target_Cell_Ptr(), true);
        if (visible) {
            point = Map.Coord_To_Radar_Pixel(CoordStruct(Coord), true);
        }
    }
    if (unknown_37b && (point != unknown_1a0 || !visible)) {
        unknown_slot265();
    }
    unknown_1a0 = point;
    if (!unknown_37b && visible) {
        unknown_slot264();
    }
    if (House == unknown_player_a35db4 && unknown_13c.Value() > 0 && unknown_13c.Value() % Rules->FlashFrameTime == 0) {
        unknown_slot266();
    }
}


// bool TechnoClass::unknown_6d83d0() (0x006D83D0)
// The type flag +0x702.

bool TechnoClass::unknown_6d83d0()
{
    return Techno_Type_Class()->HasTurretTooltips;
}


// bool TechnoClass::unknown_6d83e0() (0x006D83E0)
// The type test 0x006DF680.

bool TechnoClass::unknown_6d83e0()
{
    return Techno_Type_Class()->Has_Turret_Weapons();
}


// void TechnoClass::unknown_6d83f0(int index) (0x006D83F0)
// Unless the type has a charge turret, selects turret weapon index (0 when
// outside 0-14): +0x100 the type's turret weapon, +0x10C the index.

void TechnoClass::unknown_6d83f0(int index)
{
    if (!Techno_Type_Class()->IsChargeTurret) {
        if (index >= 0 && index < 15) {
            unknown_100 = Techno_Type_Class()->Get_Turret_Weapon(index);
            unknown_10c = index;
        } else {
            unknown_100 = Techno_Type_Class()->Get_Turret_Weapon(0);
            unknown_10c = 0;
        }
    }
}


// bool TechnoClass::unknown_6d8460() const (0x006D8460)
// Whether unknown_100 is set (not -1).

bool TechnoClass::unknown_6d8460() const
{
    return unknown_100 != -1;
}


// int TechnoClass::unknown_6d8470() const (0x006D8470)
// unknown_100.

int TechnoClass::unknown_6d8470() const
{
    return unknown_100;
}


// int TechnoClass::unknown_6d8480() (0x006D8480)
// Copies +0x100 into +0x104 and reports whether the value changed.

int TechnoClass::unknown_6d8480()
{
    int changed = unknown_100 != unknown_104;
    unknown_104 = unknown_100;
    return changed;
}


// int TechnoClass::unknown_6d84a0() (0x006D84A0)
// unknown_10c when the type test 0x006DF680 holds, else 0.

int TechnoClass::unknown_6d84a0()
{
    if (Techno_Type_Class()->Has_Turret_Weapons()) {
        return unknown_10c;
    }
    return 0;
}


// bool TechnoClass::unknown_slot225() (0x006D84D0, slot 225)
// The +0x126 flag of the slot 229 weapon.

bool TechnoClass::unknown_slot225()
{
    WeaponStruct *weapon = unknown_slot229();
    if (weapon && weapon->WeaponType) {
        return weapon->WeaponType->unknown_126;
    }
    return false;
}


// int TechnoClass::unknown_slot226() (0x006D84F0, slot 226)
// Returns 1.

int TechnoClass::unknown_slot226()
{
    return 1;
}


// int TechnoClass::unknown_slot227() (0x006D8500, slot 227)
// Returns 1.

int TechnoClass::unknown_slot227()
{
    return 1;
}


// int TechnoClass::unknown_slot228() (0x006D8510, slot 228)
// Returns 1.

int TechnoClass::unknown_slot228()
{
    return 1;
}


// WeaponStruct *TechnoClass::unknown_slot229() (0x006D8520, slot 229)
// Slot 231 for the slot 226 index.

WeaponStruct *TechnoClass::unknown_slot229()
{
    return unknown_slot231(unknown_slot226());
}


// WeaponStruct *TechnoClass::unknown_slot231(int index) (0x006D8540, slot 231)
// Weapon entry index of the type (the alternate list 0x006DF5E0 while +0x118 says so).

WeaponStruct *TechnoClass::unknown_slot231(int index)
{
    if (index == -1) {
        return 0;
    }
    if (unknown_118.Is_Elite()) {
        WeaponStruct *weapon = Techno_Type_Class()->Get_Elite_Weapon(index);
        if (weapon && weapon->WeaponType) {
            return weapon;
        }
    }
    return Techno_Type_Class()->Get_Weapon(index);
}


// WeaponStruct *TechnoClass::unknown_slot230() (0x006D85A0, slot 230)
// Slot 231 for unknown_10c when the type test 0x006DF680 holds, else for 0.

WeaponStruct *TechnoClass::unknown_slot230()
{
    return unknown_slot231(unknown_6d84a0());
}


// int TechnoClass::unknown_6d85e0(int index) (0x006D85E0)
// Whether index names a weapon: -1, 0 or 1, or a turret weapon below the
// type's WeaponCount.

int TechnoClass::unknown_6d85e0(int index)
{
    bool basic = index < 2;
    bool turret = Techno_Type_Class()->Has_Turret_Weapons() && index < Techno_Type_Class()->WeaponCount;
    if (index >= -1 && (basic || turret)) {
        return 1;
    }
    return 0;
}


// 0x006d8640: function not yet identified
//
// Body: 26 bytes (frozen inventory bound).
// Referenced from 4 sites (unconditional call).
// Ghidra prototype: undefined4 unknown_is_set_006d8640(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006d8640 {

unsigned int __stdcall unknown_is_set_006d8640(int *value)
{
    if ((value != (int *)0x0) && (*value != 0)) {
        return 1;
    }
    return 0;
}

}  // namespace seed_006d8640


// void TechnoClass::unknown_slot255(int arg1) (0x006D8680, slot 255)
// Sets unknown_170 and its frame when arg1, then slot 266.

void TechnoClass::unknown_slot255(TechnoClass *techno)
{
    if (techno) {
        unknown_170 = true;
        unknown_174 = Frame;
    }
    unknown_slot266();
}


// void TechnoClass::unknown_slot82(int arg1) (0x006D86B0, slot 82)
// Restarts the +0x154 timer with arg1 and clears unknown_16c.

void TechnoClass::unknown_slot82(int arg1)
{
    unknown_154 = CDTimerClass<FrameTimerClass>(arg1);
    unknown_16c = 0;
}


// int TechnoClass::unknown_6d86f0(int value) (0x006D86F0)
// Scales value by a factor in 256ths chosen by the mode at +0x16C from the
// time left on the +0x160 timer, at most 2000; other modes leave it
// unchanged (BuildingClass slot 79). Body 304 bytes: code to 0x006D87F8
// and the nine-entry jump table at 0x006D87FC.

int TechnoClass::unknown_6d86f0(int value)
{
    int time = unknown_160.Value();
    int factor;
    switch (unknown_16c) {
    case 1:
        factor = ((12 - time) << 8) / 6;
        break;
    case 2:
    case 8:
        factor = 512;
        break;
    case 3:
        factor = (461 * time + 1020) / 20;
        break;
    case 4:
        factor = (1024 - 77 * time) / 8;
        break;
    case 5:
        factor = (77 * time + 816) / 16;
        break;
    case 6:
        factor = 51;
        break;
    case 7:
        factor = (3072 - 461 * time) / 6;
        break;
    case 9:
        factor = ((time + 20) << 8) / 20;
        break;
    default:
        return value;
    }
    value = (factor * value) >> 8;
    if (value > 2000) {
        value = 2000;
    }
    return value;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void TechnoClass::unknown_6d8820() (0x006D8820)
// Steps a ten-state sequence (+0x16C) on the +0x160 timer while slot 203
// holds (the +0x154 timer runs), else resets it to 0: states 1-5 and 7-9
// wait out delays of 6, 4, 20, 8, 16 / 6, 4, 20 frames; state 5 returns to
// 4 while the +0x154 timer has 54 or more frames left, state 6 waits until
// it is down to 30; then 10 (TechnoClass::AI; meaning unknown).

// The time left on a timer through its function operator (ra2-decomp's
// Timer_Left, YR twin 0x0070E5A0).
static inline long Timer_Left(CDTimerClass<FrameTimerClass> const &timer)
{
    return timer();
}

void TechnoClass::unknown_6d8820()
{
    if (unknown_slot203()) {
        switch (unknown_16c) {
        case 0:
            unknown_16c = 1;
            unknown_160 = CDTimerClass<FrameTimerClass>(6);
            break;
        case 1:
            if (unknown_160.Value() == 0) {
                unknown_16c = 2;
                unknown_160 = CDTimerClass<FrameTimerClass>(4);
            }
            break;
        case 2:
            if (unknown_160.Value() == 0) {
                unknown_16c = 3;
                unknown_160 = CDTimerClass<FrameTimerClass>(20);
            }
            break;
        case 3:
            if (unknown_160.Value() == 0) {
                unknown_16c = 4;
                unknown_160 = CDTimerClass<FrameTimerClass>(8);
            }
            break;
        case 4:
            if (unknown_160.Value() == 0) {
                unknown_16c = 5;
                unknown_160 = CDTimerClass<FrameTimerClass>(16);
            }
            break;
        case 5:
            if (unknown_160.Value() == 0) {
                if (Timer_Left(unknown_154) < 54) {
                    unknown_16c = 6;
                } else {
                    unknown_16c = 4;
                    unknown_160 = CDTimerClass<FrameTimerClass>(8);
                }
            }
            break;
        case 6:
            if (Timer_Left(unknown_154) <= 30) {
                unknown_16c = 7;
                unknown_160 = CDTimerClass<FrameTimerClass>(6);
            }
            break;
        case 7:
            if (unknown_160.Value() == 0) {
                unknown_16c = 8;
                unknown_160 = CDTimerClass<FrameTimerClass>(4);
            }
            break;
        case 8:
            if (unknown_160.Value() == 0) {
                unknown_16c = 9;
                unknown_160 = CDTimerClass<FrameTimerClass>(20);
            }
            break;
        case 9:
            if (unknown_160.Value() != 0) {
                break;
            }
        default:
            unknown_16c = 10;
            break;
        }
    } else {
        unknown_16c = 0;
    }
}


// void RecoilData::unknown_6d8b90() (0x006D8B90)
// Starts the rise: state 1 for +0x04 frames (at least one), stepping the value by
// amount / frames each frame.

void RecoilData::unknown_6d8b90()
{
    if (unknown_00 != 0) {
        unknown_18 = 1;
        unknown_1c = unknown_04;
        if (unknown_04 < 1) {
            unknown_1c = 1;
        }
        unknown_10 = (float)unknown_00 / unknown_1c;
    }
}


// void RecoilData::unknown_6d8bc0() (0x006D8BC0)
// One frame: adds the step to the value; at the end of a state moves on
// (1: hold +0x0C frames, 2: fall to 0 over +0x08 frames, 3: idle).

void RecoilData::unknown_6d8bc0()
{
    if (unknown_18 != 0) {
        unknown_14 = unknown_10 + unknown_14;
        if (--unknown_1c <= 0) {
            switch (unknown_18) {
            case 1:
                unknown_18 = 2;
                unknown_1c = unknown_0c;
                unknown_10 = 0;
                if (unknown_0c > 1) {
                    break;
                }
            case 2:
                unknown_18 = 3;
                unknown_1c = unknown_08;
                if (unknown_1c < 1) {
                    unknown_1c = 1;
                }
                unknown_10 = -((float)unknown_00 / unknown_1c);
                break;
            case 3:
                unknown_18 = 0;
                unknown_14 = 0;
                break;
            }
        }
    }
}


// int TechnoClass::unknown_slot244(int flags) (0x006D8C30, slot 244)
// Adds blink flags (2, or 4 in the middle of a pulse) by the phase of the
// frame since +0x174, while the +0x184 timer is idle or the owner passes
// HouseClass 0x004F52B0.

int TechnoClass::unknown_slot244(int flags)
{
    int result = flags;
    if (unknown_184.Value() != 0 && !House->Is_Player_Control()) {
        return result;
    }
    int phase = (Frame - unknown_174 + 64) % 256;
    if (phase >= 64) {
        if (phase < 68) {
            result |= 2;
        } else if (phase < 76) {
            result |= 4;
        } else if (phase < 80) {
            result |= 2;
        } else if (phase >= 112) {
            if (phase < 116) {
                result |= 2;
            } else if (phase < 124) {
                result |= 4;
            } else if (phase < 128) {
                result |= 2;
            }
        }
    }
    return result;
}


// bool TechnoClass::unknown_slot245(HouseClass *house) (0x006D8CE0, slot 245)
// Whether house cannot see the techno right now: while the +0x184 timer
// runs for an owner failing HouseClass 0x004F52B0; for owners passing it,
// slot 48 holders outside the visible part of the pulse; otherwise slot 48
// holders on a cell 0x0047C300 refuses for house are visible (false).

bool TechnoClass::unknown_slot245(HouseClass *house)
{
    if (unknown_184.Value() != 0 && !House->Is_Player_Control()) {
        return true;
    }
    if (House->Is_Player_Control() && unknown_slot48()) {
        int phase = (Frame - unknown_174 + 64) % 256;
        if (phase >= 72 && phase <= 119) {
            return false;
        }
        return true;
    }
    UnknownCellClass *cell = (UnknownCellClass *)Get_Cell_Ptr();
    if (unknown_slot48() && cell != 0 && !cell->unknown_47c300(house->ArrayIndex)) {
        return false;
    }
    return true;
}


// bool TechnoClass::unknown_slot208(ObjectClass *target) (0x006D8DB0, slot 208)
// Whether the weapon slot 174 picks for target (flag +0x114) can engage it:
// kind 36 targets by the rules' +0x142D or that flag; with the flag, cells
// whose overlay type has +0x285, and active slot 48 objects without a slot
// 51 house.

bool TechnoClass::unknown_slot208(ObjectClass *target)
{
    WeaponTypeClass *weapon = unknown_slot231(unknown_slot174((AbstractClass *)target))->WeaponType;
    if (weapon == 0) {
        return false;
    }
    if (target->Kind_Of() == 36) {
        if (Rules->unknown_142d || weapon->unknown_114) {
            return true;
        }
        return false;
    }
    if (!weapon->unknown_114) {
        return false;
    }
    if (target->Kind_Of() == 11 && ((UnknownTechnoCellView *)target)->Overlay != -1
        && unknown_overlay_types_a35de8[((UnknownTechnoCellView *)target)->Overlay]->unknown_285) {
        return true;
    }
    if (target != 0 && target->AbstractFlags.IsObject && target->unknown_slot48() && target->unknown_slot51(true) == 0) {
        return true;
    }
    return false;
}


// 0x006d8e80: function not yet identified
//
// Body: 14 bytes (frozen inventory bound).
// Referenced from 6 sites (data, unconditional call).
// Ghidra prototype: bool __fastcall Is_Immobilized_006d8e80(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006d8e80 {

bool __fastcall Is_Immobilized_006d8e80(int self)
{
    return 0 < *(int *)(self + 0x3fc);
}

}  // namespace seed_006d8e80


class Leaf_6d8eb0 { public: unsigned int get(unsigned int a1); };
unsigned int Leaf_6d8eb0::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int value; unsigned int spare; value = a1; value = *(signed char*)(value + 0xc); return value; }


class Leaf_6d8ec0 { public: void get(); };
void Leaf_6d8ec0::get() {  }


class Leaf_6d8ed0 { public: bool get(); };
bool Leaf_6d8ed0::get() { return false; }


class Leaf_6d8ee0 { public: bool get(); };
bool Leaf_6d8ee0::get() { return false; }


// 0x006d8ef0: function not yet identified
//
// Body: 4 bytes (frozen inventory bound).
// Referenced from 2 sites (data).
// Ghidra prototype: undefined4 unknown_return_minus_one_006d8ef0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006d8ef0 {

unsigned int  unknown_return_minus_one_006d8ef0(void)
{
    return 0xffffffff;
}

}  // namespace seed_006d8ef0


// TechnoClass::unknown_slot273 (0x006D8F00): an inline virtual of shared/techno.hpp,
// emitted (and kept) in this unit; no source of its own.


// bool TechnoClass::unknown_slot274() (0x006D8F20, slot 274)
// The type's slot 40, defined out of line (the Infantry and Unit overrides jump to this copy).

bool TechnoClass::unknown_slot274()
{
    return Techno_Type_Class()->CanUseWaypoint();
}


// bool TechnoClass::unknown_slot275() (0x006D8F40, slot 275)
// The type's slot 41, defined out of line (the Infantry and Unit overrides jump to this copy).

bool TechnoClass::unknown_slot275()
{
    return Techno_Type_Class()->CanAttackMove();
}


// int __fastcall Unknown_006D8F60(int kind, int unused_edx, int unused_stack) (0x006D8F60)
// Maps kind 1 to 0x3E and kind 5 to 0x3F while 0x006F6670 holds; other kinds pass through.

bool Unknown_006F6670();

int __fastcall Unknown_006D8F60(int kind, int unused_edx, int unused_stack)
{
    bool pressed = Unknown_006F6670();
    if (kind == 1) {
        if (pressed) {
            return 0x3E;
        }
    } else if (kind == 5 && pressed) {
        return 0x3F;
    }
    return kind;
}


class Leaf_6d8f90 { public: bool get(); };
bool Leaf_6d8f90::get() { return false; }


class Leaf_6d8fa0 { public: bool get(); };
bool Leaf_6d8fa0::get() { return false; }


class Leaf_6d8fb0 { public: void get(); };
void Leaf_6d8fb0::get() {  }


class Leaf_6d8fc0 { public: bool get(); };
bool Leaf_6d8fc0::get() { return false; }


// void __fastcall TechnoClass::unknown_6d8fd0(TechnoClass *techno, int value) (0x006D8FD0)
// Sets the talk-bubble techno (0x00AC01C4) and its value (0x00AC01C8),
// restarts the talk-bubble timer (0x00AC0168) with Rules TalkBubbleTime and
// reveals around the techno for the player (0x00549C50); without both, it
// clears them and stops the timer at 0 (hypotheses: team script talk
// bubbles, 0x006B2F50).

void __fastcall TechnoClass::unknown_6d8fd0(TechnoClass *techno, int value)
{
    if (techno != 0 && value != 0) {
        unknown_talk_value_ac01c8 = value;
        unknown_talk_techno_ac01c4 = techno;
        unknown_timer_ac0168 = Rules->TalkBubbleTime;
        Map.Unknown_00549C50(techno->Center_Coord(), 2, unknown_player_a35db4, false, false, false, true, true);
    } else {
        unknown_talk_value_ac01c8 = 0;
        unknown_talk_techno_ac01c4 = 0;
        unknown_timer_ac0168 = 0;
    }
}


// Behind animation and fallback targeting reticle, original 006D9080.
// Arg1 is independently observed as two dwords (Point2D); arg2 is an
// address passed by the original caller but never read by this body.
// Its nominal pointee type is unknown; keep the unused pointer opaque.
extern ConvertClass *unknown_drawer_8323c4;
extern DynamicVectorClass<AbstractClass *> unknown_abstracts_ac0bf8;
extern bool unknown_a40b33;
void TechnoClass::unknown_6d9080(Point2D const &center, void const *unused)
{
    if (unknown_slot25(false,0)==0 && !unknown_108) {
        if (Rules->Behind) {
            unknown_108=new AnimClass(Rules->Behind,Center_Coord(),0,1,0x600,0);
            if (unknown_108) {
                if (Kind_Of()!=6) {
                    unknown_abstracts_ac0bf8.Delete((AbstractClass *)this);
                    unknown_abstracts_ac0bf8.Add(this);
                }
                ((AnimClass *)unknown_108)->Attach_To(this);
                if (!unknown_a40b33) ((AnimClass *)unknown_108)->unknown_17d=true;
            }
        } else {
            int color=unknown_drawer_8323c4->Convert(12);
            Point2D origin=center;
            int outside,inside;
            if (Frame&8) { outside=12; inside=8; }
            else { outside=10; inside=6; }
            for (int pass=0;pass<2;++pass) {
                unknown_surface_83995c->Draw_Line(origin+Point2D(-outside,-outside),origin+Point2D(-inside,-outside),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(inside,-outside),origin+Point2D(outside,-outside),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(outside,-outside),origin+Point2D(outside,-inside),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(outside,inside),origin+Point2D(outside,outside),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(outside,outside),origin+Point2D(inside,outside),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(-inside,outside),origin+Point2D(-outside,outside),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(-outside,outside),origin+Point2D(-outside,inside),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(-outside,-inside),origin+Point2D(-outside,-outside),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(-4,0),origin+Point2D(4,0),color);
                unknown_surface_83995c->Draw_Line(origin+Point2D(0,-4),origin+Point2D(0,4),color);
                ++origin.X;
                ++origin.Y;
                RGBClass rgb=ColorSchemes[unknown_player_a35db4->ColorSchemeIndex]->BaseColor.To_RGB();
                color=Build_Hicolor_Pixel(rgb.Red,rgb.Green,rgb.Blue);
            }
        }
    }
}


// void TechnoClass::unknown_6d95a0(UnknownCellClass *cell) (0x006D95A0)
// Records slot 168's value in +0x400 and adds it for the owner to the cell
// (0x00476B20); 0x006D95D0 takes it back.

void TechnoClass::unknown_6d95a0(UnknownCellClass *cell)
{
    if (cell != 0) {
        int value = unknown_slot168();
        unknown_400 = value;
        cell->Adjust_Threat(Owner(), value);
    }
}


// void TechnoClass::unknown_6d95d0(UnknownCellClass *cell) (0x006D95D0)
// Removes the owner's +0x400 from the cell (0x00476B20) and clears it.

void TechnoClass::unknown_6d95d0(UnknownCellClass *cell)
{
    if (cell != 0) {
        cell->Adjust_Threat(Owner(), -unknown_400);
        unknown_400 = 0;
    }
}


// void TechnoClass::Unknown_006D9610(AbstractClass *cell) (0x006D9610)
// Re-registers the techno with a cell: takes the old +0x400 value back
// (0x006D95D0) and records slot 168's current value (0x006D95A0).

void TechnoClass::Unknown_006D9610(AbstractClass *cell)
{
    if (cell != 0) {
        unknown_6d95d0((UnknownCellClass *)cell);
        unknown_6d95a0((UnknownCellClass *)cell);
    }
}


// void TechnoClass::unknown_6d9670(long delay) (0x006D9670)
// Restarts the +0x148 timer to run for delay frames.

void TechnoClass::unknown_6d9670(long delay)
{
    unknown_148 = CDTimerClass<FrameTimerClass>(delay);
}


// void TechnoClass::unknown_6d96a0() (0x006D96A0)
// With more than 10 frames left on the +0x148 timer (and 0x00A40774
// clear), restarts it at 4-8 frames from the scenario's generator.

void TechnoClass::unknown_6d96a0()
{
    if (unknown_148.Value() > 10 && unknown_a40774 == 0) {
        unknown_148 = CDTimerClass<FrameTimerClass>(unknown_id_source_a3d290->Random()(4, 8));
    }
}


// bool TechnoClass::unknown_6d9710() const (0x006D9710)
// Whether the timer at +0x148 has no time left.

bool TechnoClass::unknown_6d9710() const
{
    return unknown_148.Value() == 0;
}


// CoordStruct CoordStruct::operator/(int divisor) const (0x006D9770)
// Member-wise quotient; a zero divisor gives (0, 0, 0).

CoordStruct CoordStruct::operator/(int divisor) const
{
    if (divisor == 0) {
        return CoordStruct(0, 0, 0);
    }
    return CoordStruct(X / divisor, Y / divisor, Z / divisor);
}


// Grow the existing signed-key voxel-cache node table.
inline bool UnknownBuffer::Increase_Size_006d97c0(int amount)
{
    if(amount>=0) {
        NodeElement *table=new NodeElement[unknown_08+amount];
        if(table) {
            for(int index=0;index<unknown_04;++index) table[index]=static_cast<NodeElement *>(unknown_00)[index];
            delete[] static_cast<NodeElement *>(unknown_00);
            unknown_00=table;
            unknown_08+=amount;
            unknown_10=0;
            return true;
        }
    }
    return false;
}


// Explicit instance of the existing signed cache-node binary search.
template UnknownBuffer::NodeElement *__fastcall Binary_Search<UnknownBuffer::NodeElement>(UnknownBuffer::NodeElement *,int,UnknownBuffer::NodeElement const &);
