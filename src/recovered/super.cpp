// super.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/super.json carries the addresses and the rows that
// stay in their own files.


#define RA2_VECTOR_DELETE_DIRECT  // migration holdout: unit not yet re-rolled to Westwood Delete(int) through operator[] (docs/HANDOFF.md)
#define RA2_BUILDING_LOCOMOTOR_ADDREF_DECL
#define RA2_COORD_TPOINT3D
#define RA2_OBJECT_IS_FOOT
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/super.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/lightning_storm.hpp"
#include "shared/vox_class.hpp"
#include "shared/rules.hpp"
#include "shared/string_table.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/foot.hpp"
#include "shared/locomotion.hpp"
#include "shared/vox_speech.hpp"
#include "shared/building_type_list.hpp"
#include "shared/building_type_class.hpp"
#include "shared/teleport_locomotion.hpp"
#include "shared/infantry_type.hpp"
#include "shared/bullet.hpp"
#include "shared/weapon_type.hpp"
#include "shared/radar_event.hpp"
#include "shared/sound_play.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/aircraft_type.hpp"


// Header-static dynamic initializers of the unit at 0x0069C3B0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0069C3B0-0x0069C591); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0069C3D0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C400, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C420, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C440, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C460, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C480, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C4A0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C4E0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C510, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C540, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C570, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C580, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0069c3b0.cpp; no source of its own.


// Dynamic initializer (0x0069C5A0) of the cell offset table at 0x00ABD7A0:
// the cell itself, then its eight neighbours clockwise from east.

Cell const unknown_cells_abd7a0[9] = {
    Cell(0, 0),
    Cell(1, 0),
    Cell(1, -1),
    Cell(0, -1),
    Cell(-1, -1),
    Cell(-1, 0),
    Cell(-1, 1),
    Cell(0, 1),
    Cell(1, 1)
};


// SuperClass::SuperClass() (0x0069C660)
// An unowned, untyped super; joins Supers.

SuperClass::SuperClass() :
    Type(0),
    House(0),
    unknown_0x48(true),
    unknown_0x4A(unknown_cell_abd768),
    Anim(0),
    AnimDetached(false),
    unknown_0x55(false),
    unknown_0x56(false),
    unknown_0x57(false),
    unknown_0x58(false),
    unknown_0x60(-1)
{
    unknown_0x38 = unknown_empty_wide_839d4c;
    Supers.Add(this);
}


// SuperClass::SuperClass(SuperWeaponTypeClass *type, HouseClass *house) (0x0069C730)

SuperClass::SuperClass(SuperWeaponTypeClass *type, HouseClass *house) :
    Type(type),
    House(house),
    unknown_0x48(true),
    unknown_0x4A(unknown_cell_abd768),
    Anim(0),
    AnimDetached(false),
    unknown_0x55(false),
    unknown_0x56(false),
    unknown_0x57(false),
    unknown_0x58(false),
    unknown_0x5C(-1),
    unknown_0x60(-1)
{
    Create_ID();
    Supers.Add(this);
    unknown_objects_ac0cb0.Add(this);
    unknown_abstracts_ac0d08.Add(this);
}


// SuperClass::~SuperClass() (0x0069C8B0)
// Drops the animation and leaves every list.

// The scalar deleting destructor (0x0069ECD0) calls this out of line.
#pragma auto_inline(off)
SuperClass::~SuperClass()
{
    unknown_69ca70();
    unknown_supers_a35db8.Delete(this);
    Supers.Delete(this);
    unknown_objects_ac0cb0.Delete(this);
    unknown_abstracts_ac0d08.Delete(this);
}
#pragma auto_inline(on)


// void SuperClass::unknown_69ca70() (0x0069CA70)
// Drops the animation and its entry in the animation list. The explicit
// upcasts in the two Delete calls are the spelling hypothesis that gives this
// body the original inline size (0x0069CB30 then keeps its second
// Delete(int) out of line, and Recharge needs no Is_Present() call).

void SuperClass::unknown_69ca70()
{
    if (Anim != 0) {
        Anim->Loops = 0;
        Anim = 0;
        unknown_abstracts_ac0bf8.Delete((AbstractClass *)this);
    }
    if (AnimDetached) {
        unknown_abstracts_ac0bf8.Delete((AbstractClass *)this);
        AnimDetached = false;
    }
}


// void SuperClass::unknown_place_anim_69cb30(CoordStruct coord) (0x0069CB30)
// Replaces the super weapon's animation with Rules->ChronoPlacement 5 leptons
// above coord and joins the animated supers (hypothesis: the chronosphere's
// placement marker).

void SuperClass::unknown_place_anim_69cb30(CoordStruct coord)
{
    unknown_69ca70();
    AnimClass *anim = new AnimClass(Rules->ChronoPlacement, coord + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
    if (anim != 0) {
        Anim = anim;
        unknown_abstracts_ac0bf8.Add(this);
    }
}


// bool SuperClass::Suspend(bool on) (0x0069CC60)
// Puts the charging on hold or resumes it (TS SuperClass::Suspend without the charge-drain part).

bool SuperClass::Suspend(bool on)
{
    if (unknown_0x55 && !unknown_0x56 && on != unknown_0x58 && unknown_0x48) {
        if (on || Type->ManualControl) {
            RechargeTimer.Stop();
        } else {
            RechargeTimer.Start();
        }
        unknown_0x58 = on;
        return true;
    }
    return false;
}


// Grants this house a super weapon and applies its initial charging state.
// Existing AL-return/three-bool member contract retained, original0069CCF0.

bool SuperClass::Grant(bool one_time, bool announce, bool on_hold)
{
    if (!unknown_0x55) {
        unknown_0x55 = true;
        unknown_0x56 = one_time;
        unknown_0x3C.unknown_0 = false;
        bool result = false;
        unknown_0x38 = Type->UIName;
        if (!one_time) Suspend(false);
        if (Type->ShowTimer && !House->Type->MultiplayPassive) {
            unknown_supers_a35db8.Add(this);
        }
        if (Type->ManualControl) {
            unknown_0x5C = -1;
            RechargeTimer = Type->RechargeTime;
            RechargeTimer.Stop();
        } else {
            result = Recharge(announce && !on_hold);
        }
        if (on_hold) Suspend(true);
        if (unknown_0x56) Forced_Charge(announce);
        return result;
    }
    return false;
}


// bool SuperClass::Remove() (0x0069CF40)
// Takes the super weapon away from its house.

bool SuperClass::Remove()
{
    if (unknown_0x55) {
        unknown_0x57 = false;
        unknown_0x55 = false;
        unknown_supers_a35db8.Delete(this);
        return true;
    }
    return false;
}


// 0x0069cfb0: function not yet identified
//
// Body: 10 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __thiscall FUN_0069cfb0(int param_1,undefined1 param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0069cfb0 {

struct Owner_0069cfb0 { void FUN_0069cfb0(unsigned char unknown_0x57); };

void Owner_0069cfb0::FUN_0069cfb0(unsigned char unknown_0x57)
{
    int object_address = (int)this;
    *(unsigned char *)(object_address + 0x57) = unknown_0x57;
    return;
}

}  // namespace seed_0069cfb0


// bool SuperClass::Recharge(bool player) (0x0069CFC0)
// Drops the animation and starts charging anew (player is unused).

bool SuperClass::Recharge(bool player)
{
    unknown_69ca70();
    if (unknown_0x55 && !unknown_0x57 && !unknown_0x58) {
        unknown_0x5C = -1;
        RechargeTimer.Start();
        RechargeTimer = Type->RechargeTime;
        if (Type->UseChargeDrain) {
            unknown_0x60 = 0;
        }
        return true;
    }
    return false;
}


// bool SuperClass::Discharged(bool player, Cell const &cell) (0x0069D090)
// Fires the super weapon at cell and starts the next charge, or removes a
// one-time weapon (TS SuperClass::Discharged; the charge-drain weapon toggles).

// This unit tests only AL of Is_Storm_Pending_0051FC30 (whose own unit returns an
// int in EAX): the unit's declaration returns bool (explicit hypothesis).
bool __fastcall Is_Storm_Pending_Bool_0051FC30();

bool SuperClass::Discharged(bool player, Cell const &cell)
{
    if (Type->UseChargeDrain) {
        if (House->Unknown_004F52F0()) {
            if (unknown_0x60 == 2) {
                unknown_0x60 = 1;
                RechargeTimer = Type->RechargeTime - RechargeTimer.Value() / Rules->ChargeToDrainRatio;
                return false;
            }
            if (unknown_0x60 == 1) {
                unknown_0x60 = 2;
                RechargeTimer = (Type->RechargeTime - RechargeTimer.Value()) * Rules->ChargeToDrainRatio;
                Place(cell, player);
                return false;
            }
        } else {
            if (!House->unknown_142) {
                Place(cell, player);
            }
        }
        return false;
    }

    if ((RechargeTimer.Is_Active() && unknown_0x55 && unknown_0x57) || Type->PostClick) {
        if (Type->Type == 2 && Is_Storm_Pending_Bool_0051FC30()) {
            if (player) {
                Storm_Active_Message_00520BF0();
                return false;
            }
        } else {
            Place(cell, player);
            if (!Type->PostClick) {
                unknown_0x57 = false;
            }
            if (unknown_0x56) {
                unknown_0x56 = false;
                return Remove();
            } else {
                if (Type->ManualControl) {
                    RechargeTimer = Type->RechargeTime;
                    unknown_0x5C = -1;
                    RechargeTimer.Stop();
                } else if (!Type->PreClick && !Type->PostClick) {
                    Recharge(player);
                }
            }
        }
    }
    return false;
}


// bool SuperClass::unknown_69d390(bool local_player) (0x0069D390)
// The charge-up AI (TS SuperClass::AI): true when the sidebar graphic must be redrawn.

bool SuperClass::unknown_69d390(bool local_player)
{
    if (unknown_pending_super_833694 != 4 && Anim != 0) {
        Anim->unknown_17d = true;
        Anim->unknown_180.Unknown_00405FE0();
    }
    if (Is_Present() && (!unknown_0x57 || Type->UseChargeDrain) && !unknown_0x58) {
        if (!RechargeTimer.Is_Active()) {
            if (unknown_0x5C != -1) {
                unknown_0x5C = -1;
                return true;
            }
        } else {
            if (RechargeTimer == 0) {
                if (Type->UseChargeDrain) {
                    if (unknown_0x60 == 2) {
                        unknown_0x60 = 0;
                        RechargeTimer = Type->RechargeTime;
                    } else {
                        unknown_0x60 = 1;
                        unknown_0x57 = true;
                    }
                    return true;
                } else {
                    unknown_0x57 = true;
                    if (local_player) {
                        switch (Type->Type) {
                        case 0:
                            Speech_Play_Name("EVA_NuclearMissileReady", -1, -1);
                            break;
                        case 1:
                            Speech_Play_Name("EVA_IronCurtainReady", -1, -1);
                            break;
                        case 2:
                            Speech_Play_Name("EVA_LightningStormReady", -1, -1);
                            break;
                        case 3:
                            Speech_Play_Name("EVA_ChronosphereReady", -1, -1);
                            break;
                        case 5:
                        case 6:
                            Speech_Play_Name("EVA_ReinforcementsReady", -1, -1);
                            break;
                        }
                    }
                    return true;
                }
            } else {
                if (Anim_Stage() != unknown_0x5C) {
                    unknown_0x5C = Anim_Stage();
                    return true;
                }
            }
        }
    }
    return false;
}


// int SuperClass::Anim_Stage() const (0x0069D560)
// The sidebar charge stage, 0 (uncharged) to 54 (charged).


int SuperClass::Anim_Stage() const
{
    if (unknown_0x55) {
        int stage;
        if (Type->UseChargeDrain) {
            if (unknown_0x60 == 2) {
                stage = 54 * (1 - double(Type->RechargeTime * Rules->ChargeToDrainRatio - RechargeTimer.Value()) / (Type->RechargeTime * Rules->ChargeToDrainRatio));
            } else {
                stage = 54 * (double(Type->RechargeTime - RechargeTimer.Value()) / Type->RechargeTime);
            }
        } else {
            if (unknown_0x57) {
                return 54;
            }
            stage = 54 * (double(Type->RechargeTime - RechargeTimer.Value()) / Type->RechargeTime);
        }
        stage = stage < 54 - 1 ? stage : 54 - 1;
        return stage;
    }
    return 0;
}


class Leaf_69d680 { public: void get(); };
void Leaf_69d680::get() {  }


// void SuperClass::Forced_Charge(bool player) (0x0069D690)
// Charges the super weapon at once, announcing it for the player.


void SuperClass::Forced_Charge(bool player)
{
    if (unknown_0x55) {
        unknown_0x57 = true;
        RechargeTimer.Start();
        RechargeTimer = 0;
        if (player) {
            switch (Type->Type) {
            case 0:
                Speech_Play_Name("EVA_NuclearMissileReady", -1, -1);
                break;
            case 1:
                Speech_Play_Name("EVA_IronCurtainReady", -1, -1);
                break;
            case 2:
                Speech_Play_Name("EVA_LightningStormReady", -1, -1);
                break;
            case 3:
                Speech_Play_Name("EVA_ChronosphereReady", -1, -1);
                break;
            case 5:
            case 6:
                Speech_Play_Name("EVA_ReinforcementsReady", -1, -1);
                break;
            }
        }
        if (Type->UseChargeDrain) {
            unknown_0x60 = 1;
        }
    }
}


// bool SuperClass::Is_Powered() const (0x0069D760)

bool SuperClass::Is_Powered() const
{
    return Type->IsPowered;
}


// wchar_t const *SuperClass::State_String() const (0x0069D770)
// The sidebar caption for the super weapon's state, or null.


wchar_t const *SuperClass::State_String() const
{
    if (!unknown_0x58) {
        if (Type->UseChargeDrain) {
            switch (unknown_0x60) {
            case 0:
                return StringTable::Load_String("TXT_CHARGING", 0, "D:\\Ra2\\Super.CPP", 0x32B);
            case 1:
                return StringTable::Load_String("TXT_READY", 0, "D:\\Ra2\\Super.CPP", 0x32E);
            case 2:
                return StringTable::Load_String("TXT_FIRESTORM_ON", 0, "D:\\Ra2\\Super.CPP", 0x331);
            default:
                return 0;
            }
        }
        if (unknown_0x57) {
            return StringTable::Load_String("TXT_READY", 0, "D:\\Ra2\\Super.CPP", 0x344);
        } else {
            return 0;
        }
    } else {
        return StringTable::Load_String("TXT_HOLD", 0, "D:\\Ra2\\Super.CPP", 0x34A);
    }
}


// bool SuperClass::Unknown_0069D820() (0x0069D820)
// Whether the super weapon can be fired now (TS SuperClass::Can_Place).

bool SuperClass::Unknown_0069D820()
{
    if (!unknown_0x58) {
        if (Type->UseChargeDrain) {
            return unknown_0x60 != 0 ? true : false;
        }
        return unknown_0x57;
    }
    return false;
}


// void SuperClass::Place(Cell const &cell, bool player) (0x0069D850)
// Fires the super weapon at cell (TS SuperClass::Place; RA2 types 0 nuke,
// 1 iron curtain, 2 lightning storm, 3 chronosphere, 4 chrono warp,
// 5/6 paradrops; source case order follows the code layout). The chrono warp's
// piggyback swap has TibSun's and the YR twin's comip shape (TibSun building.cpp
// Mission_Unload, ra2-decomp reinf.cpp: `piggy = IPiggybackPtr(teleport);
// if (piggy != NULL) piggy->Begin_Piggyback(...)`, operator-> checked).

extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;
// The cell returned for coordinates outside the map (map.hpp's declaration; map.hpp cannot follow map_view.hpp).
extern CellClass unknown_dummy_cell_a6f9f8;                       // 0x00A6F9F8
// The psychic-warning radar note for a launched bullet (0x00438B80; name and types hypotheses).
void __fastcall Unknown_00438B80(HouseClass *house, CellClass *cell, BulletClass *bullet, char const *name);
// Drops a paradrop of count infantry of a type from a plane (0x0063A690; hypothesis).
int __fastcall Unknown_0063A690(HouseClass *house, int plane, int arg3, int arg4, CellClass *cell, AbstractClass *arg6, int type, int count);   // Create_Air_Reinforcement

// comip's operator=: the new reference through _AddRef, which stays out of line
// (0x0069ED20 for IPiggyback, 0x00453EA0 for ILocomotion; as BuildingClass 0x00449290).
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

void SuperClass::Place(Cell const &cell, bool player)
{
    switch (Type->Type) {
    case 3:
        if (unknown_0x57) {
            unknown_0x4A = cell;
            CoordStruct coord = Map[cell].Center_Coord();
            if (Map[cell].unknown_140 & 0x100) {
                coord.Z += unknown_geometry_j;
            }
            unknown_place_anim_69cb30(coord + CoordStruct(0, 0, 5));
            if (player) {
                unknown_pending_super_833694 = 4;
                return;
            }
            if (Anim != 0) {
                Anim->unknown_17d = true;
                Anim->unknown_180.Unknown_00405D50();
                return;
            }
        }
        break;

    case 4: {
        Unknown_0063B5B0(13, unknown_0x4A);
        Unknown_0063B5B0(13, cell);
        CoordStruct dest = Map[cell].Center_Coord();
        CoordStruct from = Map[unknown_0x4A].Center_Coord();
        if (Map[cell].unknown_140 & 0x100) {
            dest.Z += unknown_geometry_j;
        }
        if (Map[unknown_0x4A].unknown_140 & 0x100) {
            from.Z += unknown_geometry_j;
        }
        unknown_69ca70();
        new AnimClass(Rules->ChronoBlastDest, dest + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
        new AnimClass(Rules->ChronoBlast, from + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
        for (int index = 0; index < 9; index++) {
            Cell const *offset = &unknown_cells_abd7a0[index];
            ObjectClass *object;
            if (Map[*offset + unknown_0x4A].unknown_140 & 0x100) {
                object = Map[unknown_0x4A + *offset].AltOccupierPtr;
            } else {
                object = Map[*offset + unknown_0x4A].OccupierPtr;
            }
            while (object != 0) {
                FootClass *foot = (FootClass *)object;
                if (object->Is_Foot() && !object->In_Air()) {
                    TechnoTypeClass *ttype = object->Techno_Type_Class();
                    if (ttype->Organic && !object->Techno_Type_Class()->Teleporter && !object->In_Air()) {
                        int damage = object->Techno_Type_Class()->MaxStrength;
                        object->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
                    } else if (!foot->unknown_slot203() && !foot->unknown_210 && !object->unknown_slot111() && !object->unknown_slot112()) {
                        UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggyback(foot->Locomotor);
                        UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> teleport(CLSID_TeleportLocomotionClass);
                        UnitClass *unit = object->Kind_Of() == 1 ? (UnitClass *)object : 0;
                        CoordStruct coord = Map[*offset + cell].Center_Coord();
                        if (Map[coord].unknown_140 & 0x100) {
                            coord.Z += unknown_geometry_j;
                        }
                        if (unit != 0) {
                            foot->Locomotor.Checked()->Mark_All_Occupation_Bits(0);
                            foot->Locomotor.Checked()->Force_Track(-1, CoordStruct(coord));
                            object->Clear_Occupy_Bit(coord);
                            foot->unknown_5a0 = true;
                        }
                        teleport.Checked()->Link_To_Object(object);
                        piggyback = UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>(teleport);
                        if (piggyback != 0) {
                            piggyback.Checked()->Begin_Piggyback(foot->Locomotor);
                            foot->Locomotor = teleport;
                        }
                        CoordStruct delta = object->Center_Coord() - Map[unknown_0x4A].Center_Coord();
                        if (unit == 0) {
                            coord = delta + Map[cell].Center_Coord();
                            if (Map[coord].unknown_140 & 0x100) {
                                coord.Z += unknown_geometry_j;
                            }
                        }
                        foot->unknown_210 = true;
                        foot->unknown_21c = coord;
                        foot->Assign_Destination(&Map[*offset + cell], true);
                    }
                }
                object = object->unknown_30;
            }
        }
        if (!unknown_int_a3d548) {
            Speech_Play_Name("EVA_ChronosphereActivated", -1, -1);
        }
        if (player) {
            unknown_pending_super_833694 = -1;
            Speech_Cancel(VoxClass::Find_Index("EVA_ChronosphereReady"));
            return;
        }
        break;
    }

    case 2:
        if (unknown_0x57) {
            Unknown_0051F930(Rules->LightningStormDuration, Rules->unknown_13e8, cell, House);
            if (!unknown_int_a3d548) {
                Speech_Play_Name("EVA_LightningStormCreated", -1, -1);
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_LightningStormReady"));
                return;
            }
        }
        break;

    case 1:
        if (unknown_0x57) {
            CoordStruct coord = Map[cell].Center_Coord();
            new AnimClass(Rules->IronCurtainInvokeAnim, coord + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
            if (!unknown_int_a3d548) {
                Speech_Play_Name("EVA_IronCurtainActivated", -1, -1);
            }
            Unknown_0063B5B0(13, cell);
            for (int index = 0; index < 9; index++) {
                Cell const *offset = &unknown_cells_abd7a0[index];
                for (ObjectClass *object = Map[cell + *offset].OccupierPtr; object != 0; object = object->unknown_30) {
                    object->unknown_slot82(Rules->IronCurtainDuration);
                }
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_IronCurtainReady"));
                return;
            }
        }
        break;

    case 5:
        if (unknown_0x57) {
            int plane = AircraftTypeClass::From_Name("PDPLANE");
            CellClass *cellptr = &Map[cell];
            if (cellptr != 0 && cellptr != &unknown_dummy_cell_a6f9f8) {
                if (cellptr->Is_Tile_Water()) {
                    Cell spot = Map.Nearby_Location(cell, 0, -1, 0, 0, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
                    if (spot != unknown_cell_abd768) {
                        CellClass *nearptr = &Map[spot];
                        if (nearptr != 0 && nearptr != &unknown_dummy_cell_a6f9f8 && !nearptr->Is_Tile_Water()) {
                            cellptr = nearptr;
                        }
                    }
                }
                if (House->unknown_130 == 0) {
                    if (Rules->AllyParaDropInf.Count() == Rules->AllyParaDropNum.Count()) {
                        for (int i = 0; i < Rules->AllyParaDropInf.Count(); i++) {
                            int type = Rules->AllyParaDropInf[i]->ArrayIndex;
                            int count = Rules->AllyParaDropNum[i];
                            if (plane != -1 && type != -1) {
                                Unknown_0063A690(House, plane, 1, 0x1a, cellptr, 0, type, count);
                            }
                        }
                    }
                } else {
                    for (int i = 0; i < Rules->SovParaDropInf.Count(); i++) {
                        int type = Rules->SovParaDropInf[i]->ArrayIndex;
                        int count = Rules->SovParaDropNum[i];
                        if (plane != -1 && type != -1) {
                            Unknown_0063A690(House, plane, 1, 0x1a, cellptr, 0, type, count);
                        }
                    }
                }
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_ReinforcementsReady"));
                return;
            }
        }
        break;

    case 6:
        if (unknown_0x57) {
            int plane = AircraftTypeClass::From_Name("PDPLANE");
            CellClass *cellptr = &Map[cell];
            if (cellptr != 0 && cellptr != &unknown_dummy_cell_a6f9f8) {
                if (cellptr->Is_Tile_Water()) {
                    Cell spot = Map.Nearby_Location(cell, 0, -1, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
                    if (spot != unknown_cell_abd768) {
                        CellClass *nearptr = &Map[spot];
                        if (nearptr != 0 && nearptr != &unknown_dummy_cell_a6f9f8 && !nearptr->Is_Tile_Water()) {
                            cellptr = nearptr;
                        }
                    }
                }
                if (Rules->AmerParaDropInf.Count() == Rules->AmerParaDropNum.Count()) {
                    for (int i = 0; i < Rules->AmerParaDropInf.Count(); i++) {
                        int type = Rules->AmerParaDropInf[i]->ArrayIndex;
                        int count = Rules->AmerParaDropNum[i];
                        if (plane != -1 && type != -1) {
                            Unknown_0063A690(House, plane, 1, 0x1a, cellptr, 0, type, count);
                        }
                    }
                }
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_ReinforcementsReady"));
            }
        }
        break;

    case 0:
        if (unknown_0x57) {
            if (Is_One_Time()) {
                Cell spot = cell;
                CoordStruct coord = Cell_Coord(spot);
                coord.Z = Map.Get_Height_GL(coord);
                if (Map[spot].unknown_140 & 0x100) {
                    coord.Z += unknown_geometry_j;
                }
                WeaponTypeClass *payload = unknown_WeaponTypeClass_list_00839b90[WeaponTypeClass::Index_From_Name("NukePayload")];
                CellClass *target = &Map[coord];
                CoordStruct sound = coord;
                CoordStruct launch(sound.X, sound.Y, sound.Z + 20000);
                BulletTypeClass *btype = payload->Projectile;
                BulletClass *bullet = Create_Bullet(btype, target, 0, payload->unknown_a4, payload->Warhead, payload->unknown_a8, false);
                bullet->Set_Weapon_Type(payload);
                Unknown_00438B80(House, target, bullet, "PSIWARN");
                if (bullet != 0) {
                    double y = unknown_sin_4ba480(1.5707483884323126);
                    y *= unknown_cos_4ba4d0(1.5707483884323126);
                    y *= -100.0;
                    TVelocity2D<double> planar(unknown_cos_4ba4d0(1.5707483884323126) * unknown_cos_4ba4d0(1.5707483884323126) * -100.0, y);
                    TVelocity3D<double> velocity;
                    velocity.XY() = planar;
                    velocity.Z = unknown_sin_4ba480(1.5707483884323126) * -100.0;
                    bullet->unknown_slot118(launch, velocity);
                    if (!unknown_int_a3d548) {
                        Speech_Play_Name("EVA_NuclearMissileLaunched", -1, -1);
                    }
                    Voc_Static_Add(Rules->unknown_16c, sound, 0);
                }
                if (player) {
                    unknown_pending_super_833694 = -1;
                    Speech_Cancel(VoxClass::Find_Index("EVA_NuclearMissileReady"));
                }
                House->unknown_144 = true;
                return;
            }
            for (int index = 0; index < BuildingTypes.Count(); index++) {
                BuildingTypeClass *btype = BuildingTypes[index];
                if (btype->NukeSilo && (btype->SuperWeapon == Type->ArrayIndex || btype->SuperWeapon2 == Type->ArrayIndex)) {
                    BuildingClass *site = House->Find_Building(index, -1);
                    if (site != 0) {
                        Cell spot = cell;
                        CoordStruct coord = Cell_Coord(spot);
                        coord.Z = Map.Get_Height_GL(coord);
                        CoordStruct sound = coord;
                        site->QueueMission(0x16, false);
                        site->Commence();
                        House->NukeDest = cell;
                        *(int *)((char *)site + 0x4CC) = Type->Type;   // TODO: BuildingClass +0x4CC (building_class.hpp unknown_4cc; the BuildingClass in scope differs)
                        Voc_Static_Add(Rules->unknown_16c, sound, 0);
                        if (!unknown_int_a3d548) {
                            Speech_Play_Name("EVA_NuclearMissileLaunched", -1, -1);
                        }
                    }
                    break;
                }
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_NuclearMissileReady"));
            }
            House->unknown_144 = true;
        }
        break;
    }
}


// 0x0069e980: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0069e980(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0069e980 {

void __stdcall FUN_0069e980(int unused_0, int unused_1)
{
    return;
}

}  // namespace seed_0069e980


// bool SuperClass::unknown_69e990() const (0x0069E990)
// Whether the super weapon is still charging, or a charge-drain weapon (hypothesis).

bool SuperClass::unknown_69e990() const
{
    if (Type->UseChargeDrain) {
        return true;
    }
    return !unknown_0x57;
}


// HRESULT __stdcall SuperClass::GetClassID(GUID *classid) (0x0069E9B0)

HRESULT __stdcall SuperClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_SuperClass;
    return S_OK;
}


// HRESULT __stdcall SuperClass::Load(IStream *stream) (0x0069E9F0)
// Rebuilds the super in place (AbstractClass part only), rejoins the
// animation list while it has an animation, and fixes up the pointers.

HRESULT __stdcall SuperClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) SuperClass(NoInitClass());
    if (Anim != 0) {
        unknown_abstracts_ac0bf8.Add(this);
    }
    Swizzler.Swizzle((void **)&Type);
    Swizzler.Swizzle((void **)&House);
    Swizzler.Swizzle((void **)&Anim);
    unknown_0x3C.unknown_4 = 0;
    unknown_0x3C.unknown_8 = 0;
    return S_OK;
}


// HRESULT __stdcall SuperClass::Save(IStream *stream, BOOL clear_dirty) (0x0069EAD0)

HRESULT __stdcall SuperClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// void SuperClass::Detach(AbstractClass *target, bool all) (0x0069EAF0)
// Forgets the house, the type or the animation when it goes away.

void SuperClass::Detach(AbstractClass *target, bool)
{
    if (target == (AbstractClass *)House) {
        House = 0;
    }
    if (target == (AbstractClass *)Type) {
        Type = 0;
    }
    if (target == Anim) {
        Anim = 0;
        AnimDetached = true;
    }
}


// void SuperClass::Compute_CRC(WWCRCEngine &crc) const (0x0069EB20)

void SuperClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(unknown_0x55);
    crc(unknown_0x56);
    crc(unknown_0x57);
    crc((long)unknown_0x5C);
    crc((long)unknown_0x60);
    crc(unknown_0x48);
    crc(AnimDetached);
    if (Anim != 0) {
        crc((long)Anim->Loops);
        crc(Anim->Fetch_ID());
    }
}


// void SuperClass::unknown_69ebb0() (0x0069EBB0)
// Drops the animation and restarts the recharge timer from the full recharge time.

void SuperClass::unknown_69ebb0()
{
    unknown_69ca70();
    unknown_0x57 = false;
    unknown_0x5C = -1;
    bool active = RechargeTimer.Is_Active();
    RechargeTimer = Type->RechargeTime;
    if (!active) {
        RechargeTimer.Stop();
    }
}


// int __stdcall SuperClass::What_Am_I() (0x0069ECA0): IRTTITypeInfo, the super RTTI kind.

int __stdcall SuperClass::What_Am_I()
{
    return 0x39;
}


// int SuperClass::Kind_Of() const (0x0069ECB0)

int SuperClass::Kind_Of() const
{
    return 0x39;
}


// int SuperClass::Size_Of() const (0x0069ECC0)

int SuperClass::Size_Of() const
{
    return sizeof(SuperClass);
}


// SuperClass scalar deleting destructor (0x0069ECD0): compiler generated,
// calls ~SuperClass (0x0069C8B0) out of line.


// 0x0069ed30: function not yet identified
//
// Body: 13 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void __fastcall FUN_0069ed30(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0069ed30 {

extern "C" unsigned int Frame_00a40d2c;

void __fastcall FUN_0069ed30(int *timer_words)
{
    if (*timer_words == -1) {
        *timer_words = Frame_00a40d2c;
    }
    return;
}

}  // namespace seed_0069ed30


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// bool DynamicVectorClass<AbstractClass *>::Delete(AbstractClass * const &object) (0x0069ED40).
// Instantiated from shared/vector_class.hpp; called by the SuperClass code (0x0069D333 and three more) for the AbstractClass list at 0x00AC0BF8 (vtable 0x007A177C).
// Expands Delete(int); this unit's own copy of Delete(int) is the one at 0x00454B20.
// bool DynamicVectorClass<SuperClass *>::Delete(int index) (0x0069ED90).
// Instantiated from shared/vector_class.hpp; called at 0x0069D25E for the SuperClass list (0x00A35DB8).
