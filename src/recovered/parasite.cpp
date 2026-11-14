// parasite.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/parasite.json carries the addresses and the rows that
// stay in their own files.


#define PARASITE_GRAPPLE_COUNTERS
#define RA2_FACING_LEFT90
// Header-static dynamic initializers of the unit at 0x00605F30 and the
// unit's ParasiteClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/parasite.hpp"

// The unit's list of ParasiteClass objects (0x00A76430; name is a hypothesis).
DynamicVectorClass<ParasiteClass *> unknown_ParasiteClass_list_00a76430;


// Dynamic initializer (0x00605F60, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00605F80, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00605FA0, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00605FC0, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00605FE0, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00606000, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00606040, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00606070, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x006060A0, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x006060D0, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x006060E0, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00606100, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00606120, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// Dynamic initializer (0x00606160, _$E) emitted by HeaderStatics_diagonal_init_00605f30.cpp; no source of its own.


// ??0ParasiteClass@@QAE@XZ (0x006061A0)
// Listed in the unit list; only the timers are set.

ParasiteClass::ParasiteClass()
{
    unknown_ParasiteClass_list_00a76430.Add(this);
}


// ??0ParasiteClass@@QAE@PAVAbstractClass@@@Z (0x00606240)
// Parasite of owner, detached; listed in the unit list.

ParasiteClass::ParasiteClass(AbstractClass *owner)
{
    Owner = (FootClass *)owner;
    unknown_28 = 0;
    unknown_38 = CDTimerClass<FrameTimerClass>();
    unknown_2c = CDTimerClass<FrameTimerClass>();
    unknown_54 = false;
    unknown_4c = 0;
    unknown_50 = 0;
    Victim = 0;
    unknown_48 = 0;
    unknown_ParasiteClass_list_00a76430.Add(this);
}


// ??0ParasiteClass@@QAE@ABVNoInitClass@@@Z (0x00606320)
// Load path: the vtables and fresh timers.

ParasiteClass::ParasiteClass(NoInitClass const &noinit) : AbstractClass(noinit)
{
}


// ??1ParasiteClass@@UAE@XZ (0x00606370)
// Releases the victim, leaves the update list and the unit list.



ParasiteClass::~ParasiteClass()
{
    Unknown_006064C0();
    unknown_ParasiteClass_list_00a76430.Delete(this);
}


// ?Compute_CRC@ParasiteClass@@UBEXAAVWWCRCEngine@@@Z (0x00606460)
// AbstractClass part, the victim ID and the state.

void ParasiteClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    if (Victim != 0) {
        crc(Victim->Fetch_ID());
    }
    crc((long)unknown_48);
    crc((long)unknown_4c);
    crc((long)unknown_50);
    crc(unknown_54);
}


// void ParasiteClass::Unknown_006064C0() (0x006064C0)
// Clears the counters, releases the victim and leaves the update list.

void ParasiteClass::Unknown_006064C0()
{
    unknown_4c = 0;
    unknown_50 = 0;
    unknown_48 = 0;
    if (Victim != 0) {
        Victim->unknown_slot60();
        Victim = 0;
    }
    if (unknown_54) {
        unknown_abstracts_ac0bf8.Delete(this);
        unknown_54 = false;
    }
}


// ?Load@ParasiteClass@@UAGJPAUIStream@@@Z (0x00606540)
// AbstractClass load, vtables, rejoins the update list, pointer fixups.

HRESULT __stdcall ParasiteClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) ParasiteClass(NoInitClass());
        if (Victim != 0) {
            unknown_abstracts_ac0bf8.Add(this);
        }
        Swizzler.Swizzle((void **)&Owner);
        Swizzler.Swizzle((void **)&unknown_28);
        Swizzler.Swizzle((void **)&Victim);
    }
    return result;
}


// ?Save@ParasiteClass@@UAGJPAUIStream@@H@Z (0x00606640)
// Only the AbstractClass part.

HRESULT __stdcall ParasiteClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}


// ?GetClassID@ParasiteClass@@UAGJPAUGUID@@@Z (0x00606660)
// The class identifier.

HRESULT __stdcall ParasiteClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_ParasiteClass;
    return S_OK;
}


// Reset the two grapple counters (0x006066A0); existing 9-byte body.
#define PARASITE_DECLARATIONS_ONLY

void ParasiteClass::Reset_Grapple_Counters_006066a0()
{
    unknown_50 = 0;
    unknown_4c = 0;
}


// bool ParasiteClass::Grapple_Frame_006066B0() (0x006066B0)
// Advances the grapple animation one tick: each state shows ten frames per
// victim direction (states 0-1 at three ticks a frame from frame 0, state 2
// from frame 80 and the others from frame 160 at four ticks). True once the
// tenth frame has passed.
#include "shared/foot.hpp"
#include "shared/anim.hpp"

bool ParasiteClass::Grapple_Frame_006066B0()
{
    UnknownFacingDir dir = unknown_28->unknown_2e0.Current();
    int base;
    int delay;
    switch (unknown_48) {
    case 0:
    case 1:
        base = 0;
        delay = 3;
        break;
    case 2:
        base = 0x50;
        delay = 4;
        break;
    default:
        base = 0xA0;
        delay = 4;
        break;
    }
    if (++unknown_50 >= delay) {
        unknown_50 = 0;
        if (++unknown_4c >= 10) {
            return true;
        }
        if (Victim != 0) {
            ((AnimClass *)Victim)->Set_Rate(0x80);
            ((AnimClass *)Victim)->Stage = unknown_4c + Dir_To_8(dir) * 10 + base;
        }
    }
    return false;
}


// Parasite grapple state machine, original 0x00606780 including its five-case dispatch.
#include "shared/cell.hpp"
#include "shared/techno_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/rules.hpp"
#include "shared/id_source.hpp"
#include "shared/fast_math.hpp"

void __fastcall Make_Wakes_00606E10(FootClass *victim, CoordStruct coord);

// Original float angle-scale pool operand used by the grapple oscillation.
extern float const Grapple_Angle_Scale_007a7d00;

void ParasiteClass::Unknown_00606780()
{
    int damage;
    WeaponTypeClass *weapon = Owner->unknown_slot231(0)->WeaponType;
    WarheadTypeClass *warhead = weapon->Warhead;
    CoordStruct coord = unknown_28->Coord;
    CellClass *cell = (CellClass *)unknown_28->Get_Cell_Ptr();
    double sign = 1.0;
    if (!cell || cell->Is_Tile_Water()) {
        unknown_28->unknown_58c = CDTimerClass<FrameTimerClass>(warhead->Paralyzes);
        switch (unknown_48) {
        case 0: {
            unknown_28->unknown_280 = 0.0f;
            int index = AnimTypeClass::From_Name("SQDG");
            if (index != -1) {
                AnimTypeClass *type = AnimTypes[index];
                if (type) {
                    AnimClass *anim = new AnimClass(type, coord, 0, 1, 0x600, 0);
                    if (anim) {
                        Victim = (ParasiteVictim *)anim;
                        unknown_abstracts_ac0bf8.Add(this);
                    }
                }
            }
            unknown_48 = 1;
            Reset_Grapple_Counters_006066a0();
            Grapple_Frame_006066B0();
            break;
        }
        case 1:
            if (Grapple_Frame_006066B0()) {
                unknown_48 = unknown_id_source_a3d290->Random()(0, 1) ? 2 : 3;
                Reset_Grapple_Counters_006066a0();
                Grapple_Frame_006066B0();
                Make_Wakes_00606E10(unknown_28, coord);
            }
            break;
        case 2:
            sign = -1.0;
        case 3: {
            if (Grapple_Frame_006066B0()) {
                unknown_48 = 4;
                Reset_Grapple_Counters_006066a0();
                Grapple_Frame_006066B0();
            }
            double angle = unknown_4c * sign * 0.3141592653589793;
            unknown_28->unknown_280 = Fast_Sin(angle) * Grapple_Angle_Scale_007a7d00;
            break;
        }
        case 4: {
            unknown_28->unknown_280 = 0.0f;
            int count = Rules->SplashList.Count();
            double angle = -unknown_28->unknown_2e0.Current().As_Radian();
            double cosine = Fast_Cos(angle);
            double sine = Fast_Sin(angle);
            for (int index = 0; index < 3; ++index) {
                double across = 64.0;
                double along = index * 128.0 - 128.0;
                if (unknown_id_source_a3d290->Random()(0, 1)) across = -64.0;
                int x = (int)(along * cosine - across * sine);
                int y = (int)(along * sine + across * cosine);
                CoordStruct splash = coord + CoordStruct(x, y, 0);
                new AnimClass(Rules->SplashList[unknown_id_source_a3d290->Random()(0, count - 1)], splash, 2, 1, 0x600, -10);
            }
            if (warhead->Culling && (unknown_28->unknown_func_5d46b0() == 0 ||
                (unknown_28->unknown_func_5d46b0() == 1 && Owner->unknown_118.Is_Elite()))) {
                if (Owner->Techno_Type_Class()->Trainable) {
                    TechnoTypeClass *victim_type = unknown_28->Techno_Type_Class();
                    TechnoTypeClass *owner_type = Owner->Techno_Type_Class();
                    Owner->unknown_118.Made_A_Kill(owner_type->Cost_Of(Owner->House),
                        victim_type->Cost_Of(unknown_28->House));
                }
                FootClass *victim = unknown_28;
                Unknown_00607440();
                victim->unknown_325 = true;
                victim->unknown_slot216((int)Owner);
                victim->unknown_slot210();
                Unknown_006064C0();
                Owner->unknown_58c = CDTimerClass<FrameTimerClass>(0);
                return;
            }
            damage = weapon->unknown_a4;
            unknown_28->Take_Damage(&damage, 0, warhead, Owner, false, true);
            if (unknown_28) {
                unknown_48 = unknown_id_source_a3d290->Random()(0, 1) ? 2 : 3;
                Reset_Grapple_Counters_006066a0();
                Grapple_Frame_006066B0();
                Make_Wakes_00606E10(unknown_28, coord);
            } else {
                Unknown_006064C0();
            }
            break;
        }
        default:
            Unknown_00607440();
            break;
        }
        AnimClass *anim = (AnimClass *)Victim;
        if (anim && Owner && unknown_28) {
            CellClass *cell = (CellClass *)unknown_28->Get_Cell_Ptr();
            anim->Attach_To(unknown_28);
            anim->unknown_bc = Owner->unknown_slot115();
            if (cell) anim->unknown_e4 = cell->unknown_10a;
        }
    } else {
        Unknown_00607440();
        if (Owner) Owner->unknown_58c = CDTimerClass<FrameTimerClass>(0);
    }
}


// void __fastcall Make_Wakes_00606E10(FootClass *victim, CoordStruct coord) (0x00606E10)
// Three wake animations at random spots around coord (up to 180 leptons
// along and 64 across the victim's facing).

extern double const unknown_facing_radians_79b620;
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);

inline double UnknownFacingDir::As_Radian() const
{
    return (short(Raw)-0x3fff)*unknown_facing_radians_79b620;
}

void __fastcall Make_Wakes_00606E10(FootClass *victim, CoordStruct coord)
{
    double angle = -victim->unknown_2e0.Current().As_Radian();
    double cosine = unknown_cos_4ba4d0(angle);
    double sine = unknown_sin_4ba480(angle);
    for (int i = 0; i < 3; i++) {
        double along = unknown_id_source_a3d290->Random()(-180, 180);
        double across = unknown_id_source_a3d290->Random()(-64, 64);
        CoordStruct spot = coord + CoordStruct((int)(along * cosine - across * sine), (int)(along * sine + across * cosine), 0);
        new AnimClass(Rules->unknown_09c, spot, 0, 1, 0x600, 0);
    }
}


#include "shared/particle_system.hpp"

extern double const unknown_facing_radians_79b620;
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);

void ParasiteClass::AI()
{
    if (unknown_28 == 0) {
        return;
    }
    if (unknown_28->Techno_Type_Class()->Naval && !unknown_28->Techno_Type_Class()->Organic
        && Owner->Techno_Type_Class()->Naval && Owner->Techno_Type_Class()->Organic) {
        Unknown_00606780();
        return;
    }
    WeaponTypeClass *weapon = Owner->unknown_slot231(0)->WeaponType;
    WarheadTypeClass *warhead = weapon->Warhead;
    CoordStruct coord = unknown_28->Coord;
    if (unknown_38.Value() != 0) {
        return;
    }
    unknown_38 = CDTimerClass<FrameTimerClass>(weapon->unknown_b0);
    unknown_28->unknown_58c = CDTimerClass<FrameTimerClass>(warhead->Paralyzes);
    int damage;
    if (unknown_28->Kind_Of() == 15) {
        damage = unknown_28->Strength;
        unknown_28->Take_Damage(&damage, 0, warhead, Owner, true, true);
    } else {
        new ParticleSystemClass(Rules->unknown_ce0, coord, 0, 0, unknown_geometry_l);
        UnknownFacingDir dir = unknown_28->unknown_2e0.Current();
        if (weapon->Anim.Count() != 0) {
            AnimTypeClass *anim = weapon->Anim[Dir_To_8(dir)];
            if (anim != 0) {
                new AnimClass(anim, coord, 0, 1, 0x600, 0);
            }
        }
        UnknownFacingDir perpendicular = dir.Left_90();
        int distance = unknown_id_source_a3d290->Random()(0, 1) ? -2 : 2;
        double angle = perpendicular.As_Radian();
        int z = coord.Z;
        int y = (int)(coord.Y - unknown_sin_4ba480(angle) * distance);
        int x = (int)(unknown_cos_4ba4d0(angle) * distance + coord.X);
        CoordStruct push(x, y, z);
        unknown_28->unknown_slot223(push, 1.5f);
        damage = weapon->unknown_a4;
        unknown_28->Take_Damage(&damage, 0, warhead, Owner, false, true);
    }
}


// void ParasiteClass::Detach(AbstractClass *target, bool all) (0x00607200)
// Forgets a removed owner or victim object. When the unit attacked goes away
// while 0x00A40D04 is set, the parasite unit either dies (its +0x2C timer still
// running and slot 203 false) or is put back on the map at the coordinate
// 0x00607C30 picks, facing the way the unit attacked faced; if it cannot be
// placed there it is destroyed.
#include "shared/house.hpp"
#include "shared/team.hpp"
#include "shared/techno_map.hpp"

void ParasiteClass::Detach(AbstractClass *target, bool all)
{
    if (target == Owner) {
        Owner = 0;
        return;
    }
    if (target == unknown_28) {
        if (!unknown_flag_a40d04) {
            unknown_28 = 0;
            return;
        }
        if (unknown_2c.Value() > 0 && !Owner->unknown_slot203()) {
            unknown_28->unknown_580 = 0;
            unknown_28 = 0;
            Owner->Delete_Me();
            return;
        }
        CoordStruct coord = Unknown_00607C30();
        unknown_a40774++;
        if (coord != unknown_geometry_l) {
            if (Owner->Unlimbo(&coord, Dir_To_256(unknown_28->unknown_2e0.Current()))) {
                unknown_a40774--;
                if (Owner->unknown_381 && Owner->House->Is_Player_Control()) {
                    Owner->unknown_381 = false;
                    Owner->unknown_slot80();
                }
                if (Owner->unknown_384 != 0) {
                    ((TeamClass *)Owner->unknown_384)->unknown_func_6b8cc0(Owner);
                }
                if (!Owner->unknown_slot270()) {
                    Owner->unknown_6d6e50(0);
                    Owner->unknown_slot220(0);
                    Owner->Assign_Destination(0, true);
                }
                Owner->unknown_slot261(0, 1);
                Owner->unknown_slot262(0, 0, 0, 0);
                Map.Unknown_0054A550(CoordStruct(Owner->Coord), Owner->unknown_1f4 - 3, Owner->unknown_1f4 + 3, 0);
                unknown_28 = 0;
                return;
            }
        }
        unknown_a40774--;
        Owner->Strength = 0;
        Owner->Delete_Me();
        unknown_28 = 0;
        return;
    }
    if (target == Victim) {
        Victim = 0;
        unknown_54 = true;
    }
}


// Releases the attached unit onto an available cell; original00607440.
#include "shared/map_view.hpp"
#include "shared/house_adjacent.hpp"
void ParasiteClass::Unknown_00607440()
{
    bool naval = Owner->Techno_Type_Class()->Naval;
    int facing = Dir_To_8(unknown_28->unknown_2e0.Current());
    UnknownFacingDir exit_dir;
    if (facing >= 0 && facing <= 2) {
        exit_dir = UnknownFacingDir((unsigned short)(unknown_28->unknown_2e0.Current().Raw + 0x3fff));
    } else {
        exit_dir = UnknownFacingDir((unsigned short)(unknown_28->unknown_2e0.Current().Raw - 0x3fff));
    }
    Cell cell;
    if (naval) {
        cell = Adjacent_Cell(unknown_28->unknown_slot104(), (FacingType)Dir_To_8(exit_dir));
    } else if (unknown_2c.Value() == 0) {
        cell = unknown_28->unknown_slot104();
    } else {
        Owner->Strength = 0;
        unknown_28->unknown_58c = CDTimerClass<FrameTimerClass>(0);
        unknown_28->unknown_580 = 0;
        Owner->Delete_Me();
        unknown_28 = 0;
        return;
    }
    CellClass &target_cell = Map[cell];
    if (!target_cell.Is_Clear_To_Move(Owner->Techno_Type_Class()->SpeedType, false, false,
        Map.Zone(cell, Owner->Techno_Type_Class()->MovementZone, false),
        Owner->Techno_Type_Class()->MovementZone, -1, true)) {
        cell = unknown_28->unknown_6cf130(0);
    }
    CoordStruct coord = unknown_geometry_l;
    if (cell != unknown_geometry_k) {
        coord = Map[cell].Center_Coord();
    }
    bool can_exit = false;
    if (cell != unknown_geometry_k && coord != unknown_geometry_l && unknown_28 != 0 && !unknown_28->In_Air()) {
        if (Owner->Techno_Type_Class()->Naval) {
            can_exit = true;
        } else if (!(Owner != 0 && Owner->Kind_Of() == 1 && ((CellClass *)unknown_28->Get_Cell_Ptr())->Cell_Terrain() != 0)) {
            int land = ((CellClass *)unknown_28->Get_Cell_Ptr())->unknown_ec;
            if (land != 2 && land != 6 && land != 3) can_exit = true;
        }
    }
    if (can_exit && Owner->Unlimbo(&coord, Dir_To_256(exit_dir))) {
        if (Owner->unknown_381 && Owner->House->Is_Player_Control()) {
            Owner->unknown_381 = false;
            Owner->unknown_slot80();
        }
        if (Owner->unknown_384 != 0) {
            ((TeamClass *)Owner->unknown_384)->unknown_func_6b8cc0(Owner);
        }
        if (!Owner->unknown_slot270()) {
            Owner->unknown_6d6e50(0);
            Owner->unknown_slot220(0);
            Owner->Assign_Destination(0, true);
        }
        Owner->unknown_slot261(false, true);
        Owner->unknown_slot262(false, false, false, 0);
        Owner->unknown_58c = CDTimerClass<FrameTimerClass>(Owner->unknown_slot231(0)->WeaponType->unknown_b0 * 3);
        Unknown_006064C0();
    } else {
        Owner->Strength = 0;
        Owner->Delete_Me();
    }
    unknown_28->unknown_280 = 0.0f;
    unknown_28->unknown_58c = CDTimerClass<FrameTimerClass>(0);
    unknown_28->unknown_580 = 0;
    unknown_28 = 0;
}


// bool ParasiteClass::Can_Infect_006078F0(FootClass *target) (0x006078F0)
// A target can take a parasite when it is alive and on the map, has no
// parasite already and its type allows one; a naval parasite also needs the
// target on a water cell (or off the map).

bool ParasiteClass::Can_Infect_006078F0(FootClass *target)
{
    if (target != 0 && !target->unknown_76 && target->unknown_7d && target->Strength != 0
        && target->unknown_580 == 0 && target->Techno_Type_Class()->Parasiteable) {
        if (Owner == 0 || !Owner->Techno_Type_Class()->Naval || target->Get_Cell_Ptr() == 0
            || ((CellClass *)target->Get_Cell_Ptr())->Is_Tile_Water()) {
            return true;
        }
    }
    return false;
}


// void ParasiteClass::Unknown_00607980(FootClass *target) (0x00607980)
// Starts a parasite attachment, or returns the owner to its cell.
// Small accessors for the host and owner fields.
inline int Unknown_1f4_Of_00607980(FootClass *foot) { return foot->unknown_1f4; }
inline void Set_Parasite_00607980(FootClass *host, FootClass *parasite) { host->unknown_580 = parasite; }
void ParasiteClass::Unknown_00607980(FootClass *target)
{
    Unknown_006064C0();
    if (!Can_Infect_006078F0(target)) {
        if (!Owner->Unlimbo(&Map[Owner->unknown_454].Center_Coord(), 0)) {
            Owner->Delete_Me();
            return;
        }
        Owner->unknown_slot263(false, false, false, 0);
        Map.Unknown_0054A550(CoordStruct(Owner->Coord), Unknown_1f4_Of_00607980(Owner) - 3, Unknown_1f4_Of_00607980(Owner) + 2, 0);
        if (!Owner->unknown_slot270()) {
            Owner->unknown_slot220(0);
            Owner->Assign_Destination(0, true);
        }
        Owner->unknown_slot261(false, true);
        return;
    }
    Owner->Locomotor.Checked()->Force_Track(-1, CoordStruct(target->Coord));
    Set_Parasite_00607980(target, Owner);
    unknown_28 = target;
}


// bool ParasiteClass::Can_Exit_00607BA0() (0x00607BA0)
// The parasite unit can be put back where its victim stands: the victim is
// alive and either the parasite is naval or the cell is land it can use (an
// aircraft parasite also needs no terrain object there).

bool ParasiteClass::Can_Exit_00607BA0()
{
    if (unknown_28 == 0) {
        return false;
    }
    if (unknown_28->In_Air()) {
        return false;
    }
    if (!Owner->Techno_Type_Class()->Naval) {
        if (Owner != 0 && Owner->Kind_Of() == 1 && ((CellClass *)unknown_28->Get_Cell_Ptr())->Cell_Terrain() != 0) {
            return false;
        }
        int land = ((CellClass *)unknown_28->Get_Cell_Ptr())->unknown_ec;
        if (land == 2 || land == 6 || land == 3) {
            return false;
        }
    }
    return true;
}


// Finds an exit coordinate for the attached unit; original00607C30.
CoordStruct ParasiteClass::Unknown_00607C30()
{
    if (unknown_28 == 0) return unknown_geometry_l;
    {
        bool can_exit = false;
        if (!unknown_28->In_Air()) {
            if (Owner->Techno_Type_Class()->Naval) {
                can_exit = true;
            } else if (!(Owner != 0 && Owner->Kind_Of() == 1 && ((CellClass *)unknown_28->Get_Cell_Ptr())->Cell_Terrain() != 0)) {
                int land = ((CellClass *)unknown_28->Get_Cell_Ptr())->unknown_ec;
                if (land != 2 && land != 6 && land != 3) can_exit = true;
            }
        }
        if (can_exit) {
            return unknown_28->Coord;
        }
        int facing = Dir_To_8(unknown_28->unknown_2e0.Current());
        CellClass *victim_cell = (CellClass *)unknown_28->Get_Cell_Ptr();
        CellClass &cell = victim_cell->Adjacent_Cell(facing);
        if (!Owner->Techno_Type_Class()->Naval) {
            int land = cell.unknown_ec;
            if (land == 2 || land == 6 || land == 3) {
                return unknown_geometry_l;
            }
        }
        CoordStruct spot = cell.Closest_Free_Spot(cell.Center_Coord(), false, false);
        if (spot != unknown_geometry_l) {
            return spot;
        }
    }
    return unknown_geometry_l;
}


// ?Size_Of@ParasiteClass@@UBEHXZ (0x00607DD0)
// Object size.

int ParasiteClass::Size_Of() const
{
    return sizeof(ParasiteClass);
}


// ?Kind_Of@ParasiteClass@@UBEHXZ (0x00607DE0)
// RTTI kind 0x43.

int ParasiteClass::Kind_Of() const
{
    return 0x43;
}


// void *ParasiteClass::`scalar deleting destructor'(unsigned int flags) (0x00607DF0)
// Compiler-generated with the vtable; it expands ~ParasiteClass (whose body
// is the Unknown_006064C0 release plus the unit-list removal) inline.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<ParasiteClass *>::Clear (0x00607EF0; DynamicVectorClass<ParasiteClass *> vtable 0x007a7c2c slot 3).
// DynamicVectorClass<ParasiteClass *>::ID_pointer (0x00607F20; DynamicVectorClass<ParasiteClass *> vtable 0x007a7c2c slot 5).
// VectorClass<ParasiteClass *>::Fetch (0x00607F40; VectorClass<ParasiteClass *> vtable 0x007a7c4c slot 6).
// VectorClass<ParasiteClass *>::operator equal (0x00607F50; VectorClass<ParasiteClass *> vtable 0x007a7c4c slot 1).
// VectorClass<ParasiteClass *>::Resize (0x00607F90; VectorClass<ParasiteClass *> vtable 0x007a7c4c slot 2).
// VectorClass<ParasiteClass *>::Clear (0x00608040; VectorClass<ParasiteClass *> vtable 0x007a7c4c slot 3).
// VectorClass<ParasiteClass *>::ID_object (0x00608070; VectorClass<ParasiteClass *> vtable 0x007a7c4c slot 4).
// VectorClass<ParasiteClass *>::ID_pointer (0x006080B0; VectorClass<ParasiteClass *> vtable 0x007a7c4c slot 5).
// DynamicVectorClass<ParasiteClass *>::Resize (0x006080D0; DynamicVectorClass<ParasiteClass *> vtable 0x007a7c2c slot 2).
// DynamicVectorClass<ParasiteClass *>::ID_object (0x00608180; DynamicVectorClass<ParasiteClass *> vtable 0x007a7c2c slot 4).


extern "C" unsigned char vtable_6081b0[]; extern "C" void __cdecl release_buffer_6081b0(void*); extern "C" void __cdecl release_object_6081b0(void*);
class Leaf_6081b0 { public: void* get(unsigned int flags); };
void* Leaf_6081b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6081b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6081b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6081b0(this); return this; }


extern "C" unsigned char vtable_608200[]; extern "C" void __cdecl release_buffer_608200(void*); extern "C" void __cdecl release_object_608200(void*);
class Leaf_608200 { public: void* get(unsigned int flags); };
void* Leaf_608200::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_608200; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_608200((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_608200(this); return this; }
