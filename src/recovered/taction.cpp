// taction.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/taction.json carries the addresses and the rows that
// stay in their own files.


#define RA2_VECTOR_UNSIGNED_INDEX  // migration holdout: unit not yet re-rolled to VectorClass::operator[](int) (docs/HANDOFF.md)
#define RA2_TACTION_DECLARE_DAMAGE
#define RA2_TACTION_EXECUTE_CONTEXT
#define RA2_COORD_CELL_BASE_INIT
#define RA2_COORD_AS_CELL_NO_CASTS
#define RA2_BUILDING_TYPED_SPOTLIGHT
#define RA2_TPOINT3D_SUM_X_LOCAL
#define RA2_TACTION_TYPED_DATA
#define RA2_CELL_EQ_RETURNS_INT
#define RA2_COORD_AS_CELL
#define RA2_COORD_TPOINT3D
#define RA2_CDTIMER_NAMED_DURATION
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_FIXEDSTRING_COPY_ASSIGN_SOURCE
#define RA2_FIXEDSTRING_ASSIGN_VALUE
#define RA2_CDTIMER_STOP_SOURCE
// Header-static dynamic initializers of the unit at 0x006AD100 and the
// unit's TActionClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/object.hpp"

// The unit's list of TActionClass objects (0x00ABFD88; name is a hypothesis).
DynamicVectorClass<TActionClass *> unknown_TActionClass_list_00abfd88;



// Independently proved native copies used by this translation unit.


// Dynamic initializer (0x006AD130, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD150, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD170, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD190, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD1B0, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD1D0, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD210, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD240, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD270, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD2A0, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD2B0, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD2D0, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD2F0, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// Dynamic initializer (0x006AD330, _$E) emitted by HeaderStatics_diagonal_init_006ad100.cpp; no source of its own.


// TActionClass::TActionClass() (0x006AD370)
// A new action, numbered by its place in TActions.

// The abstract list 0x00AC0C98 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0c98;

TActionClass::TActionClass() :
    unknown_24(-1),
    Next(0),
    Type(0),
    unknown_30(0),
    unknown_34(0),
    unknown_38(0),
    unknown_3c(0),
    unknown_40(0),
    unknown_44(0),
    unknown_48(0),
    unknown_4c(0),
    unknown_50(0)
{
    unknown_TActionClass_list_00abfd88.Add(this);
    unknown_24 = unknown_TActionClass_list_00abfd88.ID(this);
    unknown_objects_ac0cb0.Add((ObjectClass *)this);
    unknown_AbstractClass_list_00ac0c98.Add(this);
    Text[0] = 0;
    Data.Value = 0;
}


// TActionClass::~TActionClass() (0x006AD4F0)

// The abstract list 0x00AC0C98 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0c98;

TActionClass::~TActionClass()
{
    Unknown_006ECB90(this, true);
    unknown_objects_ac0cb0.Delete((ObjectClass *)this);
    unknown_AbstractClass_list_00ac0c98.Delete(this);
    unknown_TActionClass_list_00abfd88.Delete(this);
    Text[0] = 0;
}


// void TActionClass::Detach(AbstractClass *target, bool all) (0x006AD600)

void TActionClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Next == target) {
        Next = Next->Next;
    }
    if ((AbstractClass *)unknown_30 == target) {
        unknown_30 = 0;
    }
    if (unknown_50 == target) {
        unknown_50 = 0;
    }
    if (unknown_4c == target) {
        unknown_4c = 0;
    }
}


// void TActionClass::Build_INI_Entry(char *buffer) const (0x006AD640)
// Appends "type,kind,value,a,b,c,d,last": the value is the number, or the
// name of the team/trigger/tag type, text, voice, sound or theme its type
// takes; the last field is the waypoint (kinds 5 and 9 write a number there).
#include "shared/taction.hpp"
#include "shared/team_type.hpp"
#include "shared/trigger_type.hpp"
#include "shared/tag_type.hpp"
#include "shared/voc_class.hpp"
#include "shared/vox_class.hpp"
#include "shared/theme.hpp"
#include "shared/waypoint_path.hpp"

void TActionClass::Build_INI_Entry(char *buffer) const
{
    int value_kind = Value_Kind(Type);
    int value = Data.Value;
    int number = 0;
    int last = 0;
    int kind = 0;
    char const *name = 0;
    if (value_kind == 0x2B) {
        kind = 9;
        number = value;
        last = unknown_48;
    }
    if (unknown_30 != 0 && value_kind == 0x28) {
        kind = 5;
        value = ((TeamTypeClass *)unknown_30)->ArrayIndex;
        name = unknown_30->ID;
    }
    if (unknown_30 != 0 && (value_kind == 0x0C || value_kind == 0x22)) {
        value = ((TeamTypeClass *)unknown_30)->ArrayIndex;
        kind = 1;
        name = unknown_30->ID;
    } else if (unknown_50 != 0 && value_kind == 0x0B) {
        value = ((TriggerTypeClass *)unknown_50)->ArrayIndex;
        kind = 2;
        name = ((TriggerTypeClass *)unknown_50)->ID;
    } else if (unknown_4c != 0 && value_kind == 0x1A) {
        value = ((TagTypeClass *)unknown_4c)->ArrayIndex;
        kind = 3;
        name = ((TagTypeClass *)unknown_4c)->ID;
    } else if (value_kind == 0x27) {
        kind = 4;
        name = Get_Text();
        if (name == 0) {
            name = "-1";
        }
    } else if (value_kind == 4) {
        kind = 6;
        name = VoxClass::Name_From(Data.Value);
    } else if (value_kind == 3 || value_kind == 0x26) {
        kind = 7;
        name = Voc_Name(Data.Value);
    } else if (value_kind == 1) {
        kind = 8;
        name = Theme.Name(Data.Value);
    }

    buffer += strlen(buffer);
    if (kind == 9) {
        sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d", Type, kind, number, unknown_34, unknown_38, unknown_3c, unknown_40, last);
    } else if (kind == 5) {
        sprintf(buffer, "%d,%d,%s,%d,%d,%d,%d,%d", Type, kind, name, unknown_34, unknown_38, unknown_3c, unknown_40, Data.Value);
    } else if (kind != 0) {
        sprintf(buffer, "%d,%d,%s,%d,%d,%d,%d,%s", Type, kind, name, unknown_34, unknown_38, unknown_3c, unknown_40,
                Waypoint_As_String(unknown_44));
    } else {
        sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%s", Type, 0, value, unknown_34, unknown_38, unknown_3c, unknown_40,
                Waypoint_As_String(unknown_44));
    }
}


// void TActionClass::Read_INI() (0x006AD860)
// Reads "type,kind,value,a,b,c,d,last" from the current strtok position.
// The kind says what the value names: a number, a team type, a trigger type,
// a tag type, a text, an EVA voice, a sound or a theme (a short token is an
// index into the type list, a longer one a name); the last field is the
// waypoint, except for kinds 5 and 9.

void TActionClass::Read_INI()
{
    Data.Value = 0;
    Type = atoi(strtok(0, ","));
    int kind = atoi(strtok(0, ","));
    char *token = strtok(0, ",");
    int value = atoi(token);
    switch (kind) {
        case 0:
        case 9:
            Data.Value = value;
            break;

        case 1:
        case 5:
            if (value == -1) {
                unknown_30 = 0;
            } else if (strlen(token) < 3) {
                unknown_30 = TeamTypes[value];
            } else {
                unknown_30 = TeamTypeClass::Find_Or_Make(token);
            }
            break;

        case 2:
            if (value == -1) {
                unknown_50 = 0;
            } else if (strlen(token) < 3) {
                unknown_50 = TriggerTypes[value];
            } else {
                unknown_50 = TriggerTypeClass::Find_Or_Make(token);
            }
            break;

        case 3:
            if (value == -1) {
                unknown_4c = 0;
            } else if (strlen(token) < 3) {
                unknown_4c = TagTypes[value];
            } else {
                unknown_4c = TagTypeClass::Find_Or_Make(token);
            }
            break;

        case 4:
            if (value == -1) {
                Text[0] = '\0';
            } else {
                Set_Text(token);
            }
            break;

        case 6:
            Data.Value = VoxClass::Find_Index(token);
            break;

        case 7:
            Data.Value = VocClass::From_Name(token);
            break;

        case 8:
            Data.Value = Theme.From_Name(token);
            break;
    }

    unknown_34 = atoi(strtok(0, ","));
    unknown_38 = atoi(strtok(0, ","));
    unknown_3c = atoi(strtok(0, ","));
    unknown_40 = atoi(strtok(0, ","));
    token = strtok(0, ",");
    if (token != 0) {
        if (kind == 5) {
            Data.Value = atoi(token);
        } else if (kind == 9) {
            unknown_48 = atoi(token);
        } else {
            unknown_44 = Waypoint_From_String(token);
        }
    }
}


// bool TActionClass::Execute(...) (0x006ADAE0)
// Performs the action: a switch over its type, most types handled by their own member (an inactive object is passed as none); types without a handler succeed.
bool TActionClass::Execute(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool success = true;
    if (object != 0 && !object->unknown_7d) object = 0;
    switch (Type) {
    case 97: success = unknown_toggle_session_flag_2229(house, object, trigger, cell); break;
    case 94: success = unknown_ion_blast_at_waypoint(house, object, trigger, cell); break;
    case 95: success = unknown_nuke_at_waypoint(house, object, trigger, cell); break;
    case 102: success = unknown_func_6b25e0(house, object, trigger, cell); break;
    case 96: success = unknown_chem_missile_at_waypoint(house, object, trigger, cell); break;
    case 91: success = unknown_infantry_with_tag_slot294(house, object, trigger, cell); break;
    case 81: success = unknown_hunt_technos_with_tag(house, object, trigger, cell); break;
    case 85: success = unknown_set_session_flag_222b(house, object, trigger, cell); break;
    case 86: success = unknown_set_session_flag_222a(house, object, trigger, cell); break;
    case 87: success = unknown_set_session_flag_222c(house, object, trigger, cell); break;
    case 82: success = unknown_hunt_idle_feet_not_of_house(house, object, trigger, cell); break;
    case 83: success = unknown_hunt_feet_mission_23_of_house(house, object, trigger, cell); break;
    case 84: success = unknown_hunt_technos_of_house_type(house, object, trigger, cell); break;
    case 67: success = unknown_player_4e7ee0(house, object, trigger, cell); break;
    case 68: success = unknown_player_4e80d0(house, object, trigger, cell); break;
    case 69: success = unknown_player_4e82c0(house, object, trigger, cell); break;
    case 63: success = unknown_damage_006afd80(house, object, trigger, cell); break;
    case 64: success = unknown_explosion_50_at_waypoint(house, object, trigger, cell); break;
    case 65: success = unknown_explosion_100_at_waypoint(house, object, trigger, cell); break;
    case 66: success = unknown_explosion_300_at_waypoint(house, object, trigger, cell); break;
    case 60: success = unknown_buildings_with_tag_slot98(house, object, trigger, cell); break;
    case 61: success = unknown_buildings_with_tag_power_down(house, object, trigger, cell); break;
    case 62: success = unknown_buildings_with_tag_power_up(house, object, trigger, cell); break;
    case 14: success = unknown_technos_with_tag_to_house(house, object, trigger, cell); break;
    case 36: success = unknown_give_technos_to_house(house, object, trigger, cell); break;
    case 11: success = unknown_show_text(house, object, trigger, cell); break;
    case 37: success = unknown_make_ally(house, object, trigger, cell); break;
    case 38: success = unknown_make_enemy(house, object, trigger, cell); break;
    case 35: success = unknown_action_006b0540(house, object, trigger, cell); break;
    case 30: success = unknown_base_building(house, object, trigger, cell); break;
    case 31: success = unknown_map_49cc20(house, object, trigger, cell); break;
    case 28: success = unknown_set_local_variable(house, object, trigger, cell); break;
    case 29: success = unknown_clear_local_variable(house, object, trigger, cell); break;
    case 59: success = unknown_map_55d990_at_waypoint(house, object, trigger, cell); break;
    case 17: success = unknown_reveal_around_waypoint(house, object, trigger, cell); break;
    case 18: success = unknown_reveal_zone_of_waypoint(house, object, trigger, cell); break;
    case 16: success = unknown_reveal_whole_map(house, object, trigger, cell); break;
    case 23: success = unknown_start_mission_timer(house, object, trigger, cell); break;
    case 24: success = unknown_stop_mission_timer(house, object, trigger, cell); break;
    case 25: success = unknown_func_6b0ac0(house, object, trigger, cell); break;
    case 26: success = unknown_func_6b0b20(house, object, trigger, cell); break;
    case 27: success = unknown_func_6b0ba0(house, object, trigger, cell); break;
    case 103: success = unknown_set_mission_timer_text(house, object, trigger, cell); break;
    case 10: success = unknown_func_6b0d30(house, object, trigger, cell); break;
    case 19: success = unknown_func_6b0d70(house, object, trigger, cell); break;
    case 98: success = unknown_sound_at_random_waypoint(house, object, trigger, cell); break;
    case 99: success = unknown_waypoint_sound_6b0ec0(house, object, trigger, cell); break;
    case 101: success = unknown_reveal_6b1050(house, object, trigger, cell); break;
    case 20: success = unknown_queue_theme(house, object, trigger, cell); break;
    case 21: success = unknown_play_voice(house, object, trigger, cell); break;
    case 33: success = unknown_grant_super_6b11a0(house, object, trigger, cell); break;
    case 34: success = unknown_grant_super_6b1200(house, object, trigger, cell); break;
    case 8: success = unknown_func_6b1270(house, object, trigger, cell); break;
    case 1: success = unknown_win_or_lose_6b1360(house, object, trigger, cell); break;
    case 2: success = unknown_lose_or_win_6b1390(house, object, trigger, cell); break;
    case 3: success = unknown_set_house_136(house, object, trigger, cell); break;
    case 9: success = unknown_set_house_198(house, object, trigger, cell); break;
    case 13: success = unknown_set_house_137(house, object, trigger, cell); break;
    case 4: success = unknown_create_team(house, object, trigger, cell); break;
    case 5: success = unknown_destroy_teams(house, object, trigger, cell); break;
    case 7: success = unknown_func_6b1470(house, object, trigger, cell); break;
    case 80: success = unknown_func_6b1490(house, object, trigger, cell); break;
    case 6: success = unknown_func_6b14b0(house, object, trigger, cell); break;
    case 32: success = unknown_func_6b14d0(house, object, trigger, cell); break;
    case 39: success = unknown_action_006b1640(house, object, trigger, cell); break;
    case 40: success = unknown_func_6b1650(house, object, trigger, cell); break;
    case 41: success = unknown_func_6b1710(house, object, trigger, cell); break;
    case 42: success = unknown_weapon_explosion_at_waypoint(house, object, trigger, cell); break;
    case 43: success = unknown_func_6b19a0(house, object, trigger, cell); break;
    case 44: success = unknown_func_6b1a80(house, object, trigger, cell); break;
    case 45: success = unknown_action_006b1ac0(house, object, trigger, cell); break;
    case 46: success = unknown_action_006b1ae0(house, object, trigger, cell); break;
    case 47: success = unknown_action_006b1af0(house, object, trigger, cell); break;
    case 48: success = unknown_look_at_waypoint(house, object, trigger, cell); break;
    case 49: success = unknown_func_6b1c80(house, object, trigger, cell); break;
    case 50: success = unknown_func_6b1d00(house, object, trigger, cell); break;
    case 51: success = unknown_map_559fd0(house, object, trigger, cell); break;
    case 52: success = unknown_spotlight_behavior_6b1d80(house, object, trigger, cell); break;
    case 12: success = unknown_func_6b1e20(house, object, trigger, cell); break;
    case 70: success = unknown_func_6b1e70(house, object, trigger, cell); break;
    case 22: success = unknown_func_6b1ec0(house, object, trigger, cell); break;
    case 53: success = unknown_func_6b1f10(house, object, trigger, cell); break;
    case 54: success = unknown_func_6b1f90(house, object, trigger, cell); break;
    case 55: success = unknown_63b5b0_at_waypoint(house, object, trigger, cell); break;
    case 56: success = unknown_set_global_variable(house, object, trigger, cell); break;
    case 57: success = unknown_clear_global_variable(house, object, trigger, cell); break;
    case 58: success = unknown_meteor_shower_at_waypoint(house, object, trigger, cell); break;
    case 71: success = unknown_func_6b2220(house, object, trigger, cell); break;
    case 72: success = unknown_func_6b2240(house, object, trigger, cell); break;
    case 73: success = unknown_set_session_22a8(house, object, trigger, cell); break;
    case 74: success = unknown_func_6b2290(house, object, trigger, cell); break;
    case 75: success = unknown_func_6b22b0(house, object, trigger, cell); break;
    case 76: success = unknown_action_006b22f0(house, object, trigger, cell); break;
    case 77: success = unknown_action_006b2310(house, object, trigger, cell); break;
    case 78: success = unknown_action_006b2330(house, object, trigger, cell); break;
    case 79: success = unknown_action_006b2350(house, object, trigger, cell); break;
    case 88: success = unknown_particle_system_at_waypoint(house, object, trigger, cell); break;
    case 89: success = unknown_func_6af980(house, object, trigger, cell); break;
    case 90: success = unknown_func_6af950(house, object, trigger, cell); break;
    case 100: success = unknown_func_6b0d50(house, object, trigger, cell); break;
    case 92: success = unknown_action_006af930(house, object, trigger, cell); break;
    case 93: success = unknown_action_006af940(house, object, trigger, cell); break;
    case 104: success = unknown_func_6b2f30(house, object, trigger, cell); break;
    case 105: success = unknown_func_6b2f50(house, object, trigger, cell); break;
    case 106: success = Set_Type_Tech_Level_006b27e0(house, object, trigger, cell); break;
    case 107: success = unknown_func_6b2810(house, object, trigger, cell); break;
    case 108: success = unknown_map_54e580_at_waypoint(house, object, trigger, cell); break;
    case 109: success = unknown_iron_curtain_at_waypoint(house, object, trigger, cell); break;
    case 110: success = unknown_func_6b2f90(house, object, trigger, cell); break;
    case 111: success = unknown_func_6b2fa0(house, object, trigger, cell); break;
    case 112: success = unknown_center_on_waypoint(house, object, trigger, cell); break;
    case 113: success = unknown_func_6b22d0(house, object, trigger, cell); break;
    case 114: success = unknown_select_sidebar_tab(house, object, trigger, cell); break;
    case 115: success = unknown_sidebar_6822b0(house, object, trigger, cell); break;
    case 116: success = unknown_waypoint_sound_6b0f90(house, object, trigger, cell); break;
    case 117: success = unknown_func_6b0d60(house, object, trigger, cell); break;
    default: break;
    }
    return success;
}


// bool TActionClass::unknown_action_006af930(...) (0x006AF930)
bool TActionClass::unknown_action_006af930(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    return true;
}


// bool TActionClass::unknown_action_006af940(...) (0x006AF940)
bool TActionClass::unknown_action_006af940(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    return true;
}


// bool TActionClass::unknown_func_6af950(...) (0x006AF950)
// Calls 0x0051FC90 with the cell of waypoint +0x44; always true.
#include "shared/id_source.hpp"
void __stdcall Unknown_0051FC90(Cell cell);   // 0x0051FC90

bool TActionClass::unknown_func_6af950(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_0051FC90(unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44));
    return true;
}


// bool TActionClass::unknown_func_6af980(...) (0x006AF980)
// Calls slot 60 on every active particle system that stands on the cell of waypoint +0x44; always true.
#include "shared/particle_system.hpp"

bool TActionClass::unknown_func_6af980(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    for (int index = 0; index < ParticleSystems.Count(); index++) {
        ParticleSystemClass *system = ParticleSystems[index];
        if (system->unknown_7d && system->unknown_slot104() == waypoint) {
            system->Delete_Me();
        }
    }
    return true;
}


// bool TActionClass::unknown_particle_system_at_waypoint(...) (0x006AFA00)
// Creates the particle system of the type at index Data.Value over the cell centre of waypoint +0x44 (ground height); always true.
#include "shared/map_view.hpp"

bool TActionClass::unknown_particle_system_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(CoordStruct(waypoint));
    new ParticleSystemClass(ParticleSystemTypes[Data.Value], coord, 0, 0, unknown_geometry_l);
    return true;
}


// bool TActionClass::unknown_hunt_technos_with_tag(...) (0x006AFAB0)
// Sends every living, active non-building techno that has the action's trigger attached through its tag and is idle or on mission 23 to mission 5; always true.
#include "shared/building.hpp"   // first: the unit also sees the full BuildingClass
#include "shared/techno.hpp"
#include "shared/tag.hpp"

bool TActionClass::unknown_hunt_technos_with_tag(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno->Kind_Of() != 6 && techno->unknown_7d && techno->unknown_74 && techno->unknown_34 != 0
            && techno->unknown_34->Is_Trigger_Attached(trigger)) {
            if (techno->Get_Mission() == 0 || techno->Get_Mission() == 0x17) {
                techno->QueueMission(5, false);
            }
        }
    }
    return true;
}


// bool TActionClass::unknown_set_session_flag_222b(...) (0x006AFB40)
// Copies the low byte of Data.Value into the session byte +0x222B; always true.

bool TActionClass::unknown_set_session_flag_222b(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_222b = Data.Boolean;
    return true;
}


// bool TActionClass::unknown_set_session_flag_222a(...) (0x006AFB60)
// Copies the low byte of Data.Value into the session byte +0x222B; always true.

bool TActionClass::unknown_set_session_flag_222a(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_222a = Data.Boolean;
    return true;
}


// bool TActionClass::unknown_set_session_flag_222c(...) (0x006AFB80)
// Copies the low byte of Data.Value into the session byte +0x222B; always true.

bool TActionClass::unknown_set_session_flag_222c(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_222c = Data.Boolean;
    return true;
}


// bool TActionClass::unknown_hunt_idle_feet_not_of_house(...) (0x006AFBA0)
// Sends every idle (mission 0), living, active, non-limbo foot not owned by the action's house to mission 5; always true.
#include "shared/foot.hpp"
extern DynamicVectorClass<FootClass *> unknown_FootClass_list_00865f38;

bool TActionClass::unknown_hunt_idle_feet_not_of_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    for (int index = 0; index < unknown_FootClass_list_00865f38.Count(); index++) {
        FootClass *foot = unknown_FootClass_list_00865f38[index];
        if (foot->Get_Mission() == 0 && foot->Strength > 0 && foot->unknown_7d && foot->unknown_74
            && !foot->unknown_76 && house != foot->House) {
            foot->QueueMission(5, false);
        }
    }
    return true;
}


// bool TActionClass::unknown_hunt_feet_mission_23_of_house(...) (0x006AFC20)
// Sends every mission-23, living, active, non-limbo foot owned by the action's house to mission 5; always true.
extern DynamicVectorClass<FootClass *> unknown_FootClass_list_00865f38;

bool TActionClass::unknown_hunt_feet_mission_23_of_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    for (int index = 0; index < unknown_FootClass_list_00865f38.Count(); index++) {
        FootClass *foot = unknown_FootClass_list_00865f38[index];
        if (foot->Get_Mission() == 0x17 && foot->Strength > 0 && foot->unknown_7d && foot->unknown_74
            && !foot->unknown_76 && foot->House == house) {
            foot->QueueMission(5, false);
        }
    }
    return true;
}


// bool TActionClass::unknown_hunt_technos_of_house_type(...) (0x006AFCA0)
// Sends every living, active techno whose +0x1AC equals Data.Value and that is idle or on mission 23 to mission 5; always true.

bool TActionClass::unknown_hunt_technos_of_house_type(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno->unknown_1ac == Data.Value && (techno->Get_Mission() == 0 || techno->Get_Mission() == 0x17)
            && techno->Strength > 0 && techno->unknown_7d && techno->unknown_74 && !techno->unknown_76) {
            techno->QueueMission(5, false);
        }
    }
    return true;
}


// bool TActionClass::unknown_player_4e7ee0(...) (0x006AFD30)
// Calls HouseClass::Flag_To_Win(true) on the player's house; always true.
#include "shared/house.hpp"

bool TActionClass::unknown_player_4e7ee0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_player_a35db4->Flag_To_Win(true);
    return true;
}


// bool TActionClass::unknown_player_4e80d0(...) (0x006AFD50)
// Calls HouseClass::Flag_To_Lose(true) on the player's house; always true.

bool TActionClass::unknown_player_4e80d0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_player_a35db4->Flag_To_Lose(true);
    return true;
}


// bool TActionClass::unknown_player_4e82c0(...) (0x006AFD70)
// Calls HouseClass::unknown_func_4e82c0() on the player's house; always true.

bool TActionClass::unknown_player_4e82c0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_player_a35db4->unknown_func_4e82c0();
    return true;
}


// TAction damage at006AFD80, independently755+13. Native five ordered blasts and bridge redraw.
#include "shared/tactical.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
bool TActionClass::unknown_damage_006afd80(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    int damage = 100;
    Unknown_0047E2C0(unknown_6b2e80(), damage, 0, Rules->unknown_c74, 1);
    damage = 100;
    Unknown_0047E2C0(unknown_6b2e80() + CoordStruct(85, 85, 0), damage, 0, Rules->unknown_c74, 1);
    damage = 100;
    Unknown_0047E2C0(unknown_6b2e80() + CoordStruct(-85, 85, 0), damage, 0, Rules->unknown_c74, 1);
    damage = 100;
    Unknown_0047E2C0(unknown_6b2e80() + CoordStruct(85, -85, 0), damage, 0, Rules->unknown_c74, 1);
    damage = 100;
    Unknown_0047E2C0(unknown_6b2e80() + CoordStruct(-85, -85, 0), damage, 0, Rules->unknown_c74, 1);
    bool valid_waypoint = unknown_id_source_a3d290->Unknown_00663240(Data.Value);
    if (valid_waypoint) {
        Cell spot = unknown_id_source_a3d290->Get_Waypoint_Location(Data.Value);
        int tries = 3;
        while (!Map.Unknown_00569090(&spot) && tries-- > 0) {}
        Point2D point;
        CoordStruct coord = CoordStruct(spot);
        coord.Z += unknown_geometry_j;
        unknown_tactical_83996c->Coord_To_Pixel(coord, point);
        Rect dirty(point.X - 128, point.Y - 128, 256, 256);
        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
    }
    return true;
}


// bool TActionClass::unknown_explosion_50_at_waypoint(...) (0x006B0080)
// Explosion effects of 50 with the rules warhead at +0xC74 at the coordinate of waypoint Data.Value; always true.

bool TActionClass::unknown_explosion_50_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_0047F600(unknown_id_source_a3d290->Waypoint_Coord(Data.Value), 50, Rules->unknown_c74, 0, 0);
    return true;
}


// bool TActionClass::unknown_explosion_100_at_waypoint(...) (0x006B00e0)
// Explosion effects of 100 with the rules warhead at +0xC74 at the coordinate of waypoint Data.Value; always true.

bool TActionClass::unknown_explosion_100_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_0047F600(unknown_id_source_a3d290->Waypoint_Coord(Data.Value), 100, Rules->unknown_c74, 0, 0);
    return true;
}


// bool TActionClass::unknown_explosion_300_at_waypoint(...) (0x006B0140)
// Explosion effects of 300 with the rules warhead at +0xC74 at the coordinate of waypoint Data.Value; always true.

bool TActionClass::unknown_explosion_300_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_0047F600(unknown_id_source_a3d290->Waypoint_Coord(Data.Value), 300, Rules->unknown_c74, 0, 0);
    return true;
}


// bool TActionClass::unknown_buildings_with_tag_slot98(...) (0x006B01A0)
// Calls ObjectClass slot 98 (1) on every living, active building whose tag has the action's trigger attached; true when any was handled.
extern DynamicVectorClass<BuildingClass *> unknown_BuildingClass_list_00a40af8;

bool TActionClass::unknown_buildings_with_tag_slot98(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool handled = false;
    for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
        if (building->unknown_7d && building->unknown_74 && !building->unknown_76 && building->unknown_34 != 0
            && building->unknown_34->Is_Trigger_Attached(trigger)) {
            building->Sell_Back(1);
            handled = true;
        }
    }
    return handled;
}


// bool TActionClass::unknown_infantry_with_tag_slot294(...) (0x006B0210)
// Calls InfantryClass slot 294 on every living, active infantry whose tag has the action's trigger attached; true when any was handled.
#include "shared/infantry.hpp"
extern DynamicVectorClass<InfantryClass *> Infantry;

bool TActionClass::unknown_infantry_with_tag_slot294(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool handled = false;
    for (int index = 0; index < Infantry.Count(); index++) {
        InfantryClass *infantry = Infantry[index];
        if (infantry->unknown_7d && infantry->unknown_74 && !infantry->unknown_76 && infantry->unknown_34 != 0
            && infantry->unknown_34->Is_Trigger_Attached(trigger)) {
            infantry->Berzerk();
            handled = true;
        }
    }
    return handled;
}


// bool TActionClass::unknown_buildings_with_tag_power_down(...) (0x006B0280)
// Runs BuildingClass::unknown_44d530 on every living, active building flagged at +0x530 whose tag has the action's trigger attached; true when any was handled.
extern DynamicVectorClass<BuildingClass *> unknown_BuildingClass_list_00a40af8;

bool TActionClass::unknown_buildings_with_tag_power_down(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool handled = false;
    for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
        if (building->unknown_7d && building->unknown_74 && !building->unknown_76 && building->unknown_34 != 0
            && building->unknown_34->Is_Trigger_Attached(trigger) && building->unknown_530) {
            building->unknown_44d530();
            handled = true;
        }
    }
    return handled;
}


// bool TActionClass::unknown_buildings_with_tag_power_up(...) (0x006B02F0)
// Runs BuildingClass::unknown_44d3e0 on every living, active building not flagged at +0x530 whose tag has the action's trigger attached; true when any was handled.
extern DynamicVectorClass<BuildingClass *> unknown_BuildingClass_list_00a40af8;

bool TActionClass::unknown_buildings_with_tag_power_up(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool handled = false;
    for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
        if (building->unknown_7d && building->unknown_74 && !building->unknown_76 && building->unknown_34 != 0
            && building->unknown_34->Is_Trigger_Attached(trigger) && !building->unknown_530) {
            building->unknown_44d3e0();
            handled = true;
        }
    }
    return handled;
}


// bool TActionClass::unknown_technos_with_tag_to_house(...) (0x006B0360)
// Passes the house of type Data.Value to TechnoClass slot 222 of every living, active techno whose tag has the action's trigger attached; true when any was handled.

bool TActionClass::unknown_technos_with_tag_to_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool handled = false;
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno->unknown_7d && techno->unknown_74 && !techno->unknown_76 && techno->unknown_34 != 0
            && techno->unknown_34->Is_Trigger_Attached(trigger)) {
            techno->unknown_slot222(HouseClass::As_Pointer(Data.Value), 0);
            handled = true;
        }
    }
    return handled;
}


// bool TActionClass::unknown_give_technos_to_house(...) (0x006B03E0)
// Passes the house of type Data.Value to TechnoClass slot 222 of every techno owned by the action's house; true when any was handled.

bool TActionClass::unknown_give_technos_to_house(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool handled = false;
    HouseClass *new_house = HouseClass::As_Pointer(Data.Value);
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno->House == house) {
            techno->unknown_slot222(new_house, 0);
            handled = true;
        }
    }
    return handled;
}


// Text style type/bitwise operation hypothesis from Westwood headers; native caller passes0x4046.
enum TextPrintType { TPF_6PT_GRAD = 6, TPF_FULLSHADOW = 0x40, TPF_USE_GRAD_PAL = 0x4000 };
inline TextPrintType operator|(TextPrintType left, TextPrintType right)
{
    return static_cast<TextPrintType>(static_cast<int>(left) | static_cast<int>(right));
}
// bool TActionClass::unknown_show_text(...) (0x006B0440)
// Shows the action's text (when it has one) in the message list in the color slot 1 (2 when +0x2239 is set)
// for the rules' MessageDelay minutes; always true. The named rules pointer gives the original's eax load.
#include "shared/string_table.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/uicmnds.hpp"
#include "shared/session_state.hpp"
#include "shared/message_list.hpp"

bool TActionClass::unknown_show_text(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    char const *text = Get_Text();
    ScenarioClass *scenario = unknown_id_source_a3d290;
    int color_scheme = scenario->unknown_2239 ? 2 : 1;
    int color = unknown_session_a3d298.Unknown_006715D0(color_scheme);
    if (text != 0) {
        unknown_messages_a3dc70.Add_Message(0, 0, StringTable::Load_String(text, 0, "D:\\Ra2\\TAction.CPP", 0x62E), color, TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), (int)(Rules->MessageDelay * 900.0), 0);
    }
    return true;
}


// bool TActionClass::unknown_make_ally(...) (0x006B04C0)
// Makes the action's house and the house of type Data.Value allies of each other (both directions, silent); always true.

bool TActionClass::unknown_make_ally(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != -1) {
        HouseClass *other = HouseClass::As_Pointer(Data.Value);
        house->Make_Ally(other, false);
        other->Make_Ally(house, false);
    }
    return true;
}


// bool TActionClass::unknown_make_enemy(...) (0x006B0500)
// Makes the action's house and the house of type Data.Value enemies of each other (both directions, silent); always true.

bool TActionClass::unknown_make_enemy(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != -1) {
        HouseClass *other = HouseClass::As_Pointer(Data.Value);
        house->Make_Enemy(other, false);
        other->Make_Enemy(house, false);
    }
    return true;
}


// bool TActionClass::unknown_action_006b0540(...) (0x006B0540)
bool TActionClass::unknown_action_006b0540(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (house != 0) house->unknown_541c = Data.Value;
    return true;
}


// bool TActionClass::unknown_base_building(...) (0x006B0560)
// Red Alert's TACTION_BASE_BUILDING: sets or clears the house's base-building flag (+0x13B); when set, the base
// centre and the base plan's cell move to the cell of the house's first building of the list at +0x50, and an
// empty base plan is filled (0x004F5BE0); always true.

bool TActionClass::unknown_base_building(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Boolean) {
        house->unknown_13b = true;
        if (house->unknown_50.Count() > 0) {
            CoordStruct coord = house->unknown_50[0]->Coord;
            Cell center = coord.As_Cell();
            house->BaseCenter = CoordStruct(center);
            house->unknown_55f0.unknown_50 = center;
        }
        if (house->unknown_55f0.Nodes.Count() == 0) {
            house->unknown_func_4f5be0();
        }
    } else {
        house->unknown_13b = false;
    }
    return true;
}


// bool TActionClass::unknown_map_49cc20(...) (0x006B0620)
// Calls DisplayClass::Encroach_Shadow on the map; always true.

bool TActionClass::unknown_map_49cc20(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Map.Encroach_Shadow();
    return true;
}


// bool TActionClass::unknown_set_local_variable(...) (0x006B0630)
// Sets local variable Data.Value (ScenarioClass::unknown_6612c0); always true.

bool TActionClass::unknown_set_local_variable(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_6612c0(Data.Value, true);
    return true;
}


// bool TActionClass::unknown_clear_local_variable(...) (0x006B0650)
// Clears local variable Data.Value (ScenarioClass::unknown_6612c0); always true.

bool TActionClass::unknown_clear_local_variable(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_6612c0(Data.Value, false);
    return true;
}


// bool TActionClass::unknown_reveal_around_waypoint(...) (0x006B0670)
// Unless the player's house flag +0x188 is set: Sight_From the player twice (last argument 0, then 1) around waypoint Data.Value, moved back by half its cell's +0x11B level (plus 4 on a bridge), raised by that level, with the rules RevealTriggerRadius (Red Alert's TACTION_REVEAL_SOME); always true.
#include "shared/map_cell.hpp"


bool TActionClass::unknown_reveal_around_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (!unknown_player_a35db4->unknown_188) {
        Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(Data.Value);
        int extra;
        if ((Map[waypoint].unknown_140 & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
            extra = 4;
        } else {
            extra = 0;
        }
        int size = Map[waypoint].unknown_11b + extra;
        Cell spot = waypoint - Cell(size / 2, size / 2);
        Map.Sight_From(XYZ_Coord(spot.X * 256 + 128, spot.Y * 256 + 128, unknown_geometry_g * size), Rules->RevealTriggerRadius, unknown_player_a35db4, false, false, 0, true, 0);
        spot = waypoint - Cell(size / 2, size / 2);
        Map.Sight_From(XYZ_Coord(spot.X * 256 + 128, spot.Y * 256 + 128, unknown_geometry_g * size), Rules->RevealTriggerRadius, unknown_player_a35db4, false, false, 0, true, 1);
    }
    return true;
}


// bool TActionClass::unknown_map_55d990_at_waypoint(...) (0x006B0800)
// Calls Map.Unknown_0055D990 with the cell of waypoint Data.Value; always true.


bool TActionClass::unknown_map_55d990_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(Data.Value);
    Map.Unknown_0055D990(waypoint);
    return true;
}


// bool TActionClass::unknown_reveal_zone_of_waypoint(...) (0x006B0840)
// Unless the player's house flag +0x188 is set: Sight_From the player (radius 2, twice) at every cell of the local-map walk in the zone of waypoint Data.Value (Red Alert's TACTION_REVEAL_ZONE); always true.
// bool TActionClass::unknown_reveal_zone_of_waypoint(...) (0x006B0840)
// Red Alert's TACTION_REVEAL_ZONE: unless the player sees the whole map (+0x188), every cell of the local map
// in the movement zone of waypoint Data.Value is sighted twice for the player (radius 2, from the cell centre moved
// back by half its bridge height and raised to it); always true.


bool TActionClass::unknown_reveal_zone_of_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (!unknown_player_a35db4->unknown_188) {
        int zone = Map.Zone(unknown_id_source_a3d290->Get_Waypoint_Location(Data.Value), 1, false);
        Map.Unknown_0055AA50();
        CellClass *cellptr;
        while ((cellptr = Map.Unknown_0055A910()) != 0) {
            Cell const &where = cellptr->MapCoords;
            if (Map.Zone(where, 1, false) == zone) {
                int level = cellptr->unknown_11b / 2;
                Cell spot = where - Cell(level / 2, level / 2);
                Map.Sight_From(XYZ_Coord(spot.X * 256 + 128, spot.Y * 256 + 128, unknown_geometry_g * level), 2, unknown_player_a35db4, false, false, 0, true, 0);
                spot = where - Cell(level / 2, level / 2);
                Map.Sight_From(XYZ_Coord(spot.X * 256 + 128, spot.Y * 256 + 128, unknown_geometry_g * level), 2, unknown_player_a35db4, false, false, 0, true, 1);
            }
        }
    }
    return true;
}


// bool TActionClass::unknown_reveal_whole_map(...) (0x006B09B0)
// Once per player (house +0x188): walks every cell of the map, lets the radar mark each as seen for the player's house, and flags a full redraw; always true.


bool TActionClass::unknown_reveal_whole_map(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (!unknown_player_a35db4->unknown_188) {
        unknown_player_a35db4->unknown_188 = true;
        Map.Reset_Iterator();
        CellClass *cell;
        while ((cell = Map.Iterate()) != 0) {
            Map.Map_Cell(cell->MapCoords, unknown_player_a35db4);
        }
        Map.Flag_To_Redraw(1);
    }
    return true;
}


// bool TActionClass::unknown_start_mission_timer(...) (0x006B0A20)
// If the session mission timer is stopped, starts it and flags the tab bar for a redraw; always true.


bool TActionClass::unknown_start_mission_timer(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_id_source_a3d290->MissionTimer.Started == -1) {
        unknown_id_source_a3d290->MissionTimer.Start();
        Map.Unknown_006A17B0();
    }
    return true;
}


// bool TActionClass::unknown_stop_mission_timer(...) (0x006B0A60)
// If the mission timer runs, stops it and flags the tab bar for a redraw; always true (Red Alert's TACTION_STOP_TIMER).

bool TActionClass::unknown_stop_mission_timer(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_id_source_a3d290->MissionTimer.Started != -1) {
        unknown_id_source_a3d290->MissionTimer.Stop();
        Map.Unknown_006A17B0();
    }
    return true;
}


// bool TActionClass::unknown_func_6b0ac0(...) (0x006B0AC0)
// Adds Data.Value quarter minutes (15 frames each) to the mission timer, restarting
// it from the time left, and has the tab redraw it; always true.

bool TActionClass::unknown_func_6b0ac0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->MissionTimer = unknown_id_source_a3d290->MissionTimer + Data.Value * 15;
    Map.Unknown_006A17B0();
    return true;
}


// bool TActionClass::unknown_func_6b0b20(...) (0x006B0B20)
// Takes Data.Value quarter minutes (15 frames each) from the mission timer (not
// below zero), restarting it, and has the tab redraw it; always true.

bool TActionClass::unknown_func_6b0b20(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_id_source_a3d290->MissionTimer <= Data.Value * 15) {
        unknown_id_source_a3d290->MissionTimer = 0;
    } else {
        unknown_id_source_a3d290->MissionTimer = unknown_id_source_a3d290->MissionTimer - Data.Value * 15;
    }
    Map.Unknown_006A17B0();
    return true;
}


// bool TActionClass::unknown_func_6b0ba0(...) (0x006B0BA0)
// Sets the mission timer to Data.Value quarter minutes (15 frames each) and starts
// it, clears its text and label, and has the tab redraw it; always true.
#include "shared/abstract_type.hpp"

bool TActionClass::unknown_func_6b0ba0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->MissionTimer = Data.Value * 15;
    unknown_id_source_a3d290->MissionTimer.Start();
    unknown_id_source_a3d290->MissionTimerText = unknown_empty_wide_839d4c;
    FixedString<32> label(unknown_string_83c55c);
    unknown_id_source_a3d290->MissionTimerLabel = label;
    Map.Unknown_006A17B0();
    return true;
}


// bool TActionClass::unknown_set_mission_timer_text(...) (0x006B0C50)
// Sets the mission timer text from the action's text (the string table entry and its label; the empty text and label without one) and has the tab redraw it; always true.

bool TActionClass::unknown_set_mission_timer_text(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    char const *text = Get_Text();
    FixedString<32> label(text);
    if (text != 0) {
        unknown_id_source_a3d290->MissionTimerText = StringTable::Load_String(text, 0, "D:\\Ra2\\TAction.CPP", 0x802);
        unknown_id_source_a3d290->MissionTimerLabel = label;
    } else {
        unknown_id_source_a3d290->MissionTimerText = unknown_empty_wide_839d4c;
        FixedString<32> empty_label(unknown_string_83c55c);
        unknown_id_source_a3d290->MissionTimerLabel = empty_label;
    }
    Map.Unknown_006A17B0();
    return true;
}


// bool TActionClass::unknown_func_6b0d30(...) (0x006B0D30)
// Passes Data.Value to 0x005A07F0 with -1, 1, 1, 1; always true.
#include "shared/team.hpp"

bool TActionClass::unknown_func_6b0d30(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_005A07F0(Data.Value, -1, 1, 1, 1);
    return true;
}


// bool TActionClass::unknown_func_6b0d50(...) (0x006B0D50)
// Unknown_005A0920(Data.Value, false); always true.

bool TActionClass::unknown_func_6b0d50(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_005A0920(Data.Value, false);
    return true;
}


// bool TActionClass::unknown_func_6b0d60(...) (0x006B0D60)
// Unknown_005A0920(Data.Value, true); always true.

bool TActionClass::unknown_func_6b0d60(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_005A0920(Data.Value, true);
    return true;
}


// bool TActionClass::unknown_func_6b0d70(...) (0x006B0D70)
// Plays sound Data.Value at full volume (priority 0x2000); always true.

bool TActionClass::unknown_func_6b0d70(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Voc_Play(Data.Value, 1.0f, 0x2000, 0);
    return true;
}


// bool TActionClass::unknown_sound_at_random_waypoint(...) (0x006B0D90)
// Plays the static sound Data.Value at a random one of the set waypoints 0..100 (the cell list stops at sizeof(list) entries, as the original); always true.
#include "shared/sound_play.hpp"

bool TActionClass::unknown_sound_at_random_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell cells[100];
    int count = 0;
    for (int waypoint = 0; waypoint < 101; waypoint++) {
        if (unknown_id_source_a3d290->Unknown_00663240(waypoint)) {
            cells[count++] = unknown_id_source_a3d290->Get_Waypoint_Location(waypoint);
            if (count == sizeof(cells)) {
                break;
            }
        }
    }
    Voc_Static_Add(Data.Value, CoordStruct(cells[Random_Pick(0, count - 1)]), 0);
    return true;
}


// ObjectClass *__fastcall Building_Or_Terrain_At_006B0E40(CoordStruct const &coord) (0x006B0E40)
// The object in the cell of a coordinate: its building, else its terrain object; none for the unset cell (name hypothesis).
// Execute calls it out of line; the waypoint sound actions after it inline it.
ObjectClass *__fastcall Building_Or_Terrain_At_006B0E40(CoordStruct const &coord)
{
    Cell cell = Coord_Cell(coord);
    ObjectClass *found = 0;
    if (cell != unknown_geometry_k) {
        CellClass &cellobj = Map[cell];
        found = cellobj.Cell_Building();
        if (found == 0) {
            found = cellobj.Cell_Terrain(false);
        }
    }
    return found;
}


// bool TActionClass::unknown_waypoint_sound_6b0ec0(...) (0x006B0EC0)
// Plays sound Data.Value on the object at waypoint +0x44, else starts a static sound there; always true.
#include "shared/voc_static.hpp"

bool TActionClass::unknown_waypoint_sound_6b0ec0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    CoordStruct coord = unknown_id_source_a3d290->Waypoint_Coord(unknown_44);
    ObjectClass *target = Building_Or_Terrain_At_006B0E40(coord);
    if (target != 0) {
        target->unknown_func_5d5520(Data.Value);
    } else {
        Voc_Static_Add(Data.Value, coord, 1);
    }
    return true;
}


// bool TActionClass::unknown_waypoint_sound_6b0f90(...) (0x006B0F90)
// Stops the sound of the building or terrain object at waypoint +0x44 (0x005D5520 with -1), else lets the static sounds there end; always true.

bool TActionClass::unknown_waypoint_sound_6b0f90(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    CoordStruct coord = unknown_id_source_a3d290->Waypoint_Coord(unknown_44);
    ObjectClass *target = Building_Or_Terrain_At_006B0E40(coord);
    if (target != 0) {
        target->unknown_func_5d5520(-1);
    } else {
        Voc_Static_Finish_At(coord, 1);
    }
    return true;
}


// bool TActionClass::unknown_reveal_6b1050(...) (0x006B1050)
// TibSun TAction_REVEAL_SOME shape: unless the player sees everything, Map 0x0054A720 reveals the rules
// RevealTriggerRadius around waypoint Data.Value (raised by half the cell height, bridges +4), then 0x0055A300; always true.


bool TActionClass::unknown_reveal_6b1050(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (!unknown_player_a35db4->unknown_188) {
        Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(Data.Value);
        int extra;
        if ((Map[waypoint].unknown_140 & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
            extra = 4;
        } else {
            extra = 0;
        }
        int size = Map[waypoint].unknown_11b + extra;
        Cell spot = waypoint - Cell(size / 2, size / 2);
        Map.Unknown_0054A720(Cell_Coord(spot) + CoordStruct(0, 0, unknown_geometry_g * size), Rules->RevealTriggerRadius);
        Map.Unknown_0055A300();
    }
    return true;
}


// bool TActionClass::unknown_queue_theme(...) (0x006B1160)
// Queues theme Data.Value on the music player; always true.

bool TActionClass::unknown_queue_theme(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Theme.Queue_Song(Data.Value);
    return true;
}


// bool TActionClass::unknown_play_voice(...) (0x006B1180)
// Plays the VoxClass Data.Value through Speech_Play (type 2, its own priority); always true.


bool TActionClass::unknown_play_voice(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    int speech = Data.Value;
    Speech_Play(speech, 2, -1);
    return true;
}


// bool TActionClass::unknown_grant_super_6b11a0(...) (0x006B11A0)
// Grants the house's super weapon Data.Value one-time; the player's house adds it to the sidebar and redraws that strip; always true.
#include "shared/super.hpp"

bool TActionClass::unknown_grant_super_6b11a0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    house->Supers[Data.Value]->Grant(true, false, false);
    if (unknown_player_a35db4 == house) {
        Map.Unknown_0067C530(31, Data.Value);
        Map.Unknown_0067C2D0(Unknown_006818D0(31, Data.Value, 0));
    }
    return true;
}


// bool TActionClass::unknown_grant_super_6b1200(...) (0x006B1200)
// Grants the house's super weapon Data.Value and clears its +0x48; the player's house adds it to the sidebar and redraws that strip; always true.

bool TActionClass::unknown_grant_super_6b1200(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    house->Supers[Data.Value]->Grant(false, false, false);
    house->Supers[Data.Value]->unknown_0x48 = false;
    if (unknown_player_a35db4 == house) {
        Map.Unknown_0067C530(31, Data.Value);
        Map.Unknown_0067C2D0(Unknown_006818D0(31, Data.Value, 0));
    }
    return true;
}


// bool TActionClass::unknown_func_6b1270(...) (0x006B1270)
// Creates the animation of the rules DropZoneAnim type at the waypoint +0x44 (ground height, bridge level counted), flagged +0x178; always true.
#include "shared/anim_type.hpp"

bool TActionClass::unknown_func_6b1270(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    unsigned int bridge_flags = Map[waypoint].unknown_140;
    if ((bridge_flags & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
        int bridge_height = unknown_geometry_j;
        coord.Z += bridge_height;
    }
    (new AnimClass(Rules->DropZoneAnim, coord, 0, 1, 0x600, 0))->unknown_178 = true;
    return true;
}


// bool TActionClass::unknown_win_or_lose_6b1360(...) (0x006B1360)
// The player wins (quietly no) when its house type is Data.Value, else loses; always true.
#include "shared/house_type.hpp"

bool TActionClass::unknown_win_or_lose_6b1360(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value == unknown_player_a35db4->Type->House) {
        unknown_player_a35db4->Flag_To_Win(false);
        return true;
    }
    unknown_player_a35db4->Flag_To_Lose(false);
    return true;
}


// bool TActionClass::unknown_lose_or_win_6b1390(...) (0x006B1390)
// The player loses when its house type is Data.Value, else wins; always true.

bool TActionClass::unknown_lose_or_win_6b1390(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != unknown_player_a35db4->Type->House) {
        unknown_player_a35db4->Flag_To_Win(false);
        return true;
    }
    unknown_player_a35db4->Flag_To_Lose(false);
    return true;
}


// bool TActionClass::unknown_set_house_136(...) (0x006B13C0)
// Sets +0x136 of the house of type Data.Value (none for -1); always true.

bool TActionClass::unknown_set_house_136(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != -1) {
        HouseClass::As_Pointer(Data.Value)->unknown_136 = true;
    }
    return true;
}


// bool TActionClass::unknown_set_house_198(...) (0x006B13E0)
// Sets HouseClass +0x198 of the house of type Data.Value to 4; always true.

bool TActionClass::unknown_set_house_198(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != -1) {
        HouseClass::As_Pointer(Data.Value)->unknown_198 = 4;
    }
    return true;
}


// bool TActionClass::unknown_set_house_137(...) (0x006B1400)
// Sets HouseClass +0x137 of the house of type Data.Value; always true.

bool TActionClass::unknown_set_house_137(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != -1) {
        HouseClass::As_Pointer(Data.Value)->unknown_137 = true;
    }
    return true;
}


// bool TActionClass::unknown_create_team(...) (0x006B1420)
// Creates one team of the team type at +0x30 with the global counter at 0x00A40774 raised around the call; always true.
extern int unknown_team_creating_a40774;

bool TActionClass::unknown_create_team(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_team_creating_a40774++;
    TeamTypeClass *team_type = (TeamTypeClass *)unknown_30;
    if (team_type != 0) {
        team_type->Create_One_Of(0);
    }
    unknown_team_creating_a40774--;
    return true;
}


// bool TActionClass::unknown_destroy_teams(...) (0x006B1450)
// Destroys every team of the team type at +0x30; always true.

bool TActionClass::unknown_destroy_teams(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    TeamTypeClass *team_type = (TeamTypeClass *)unknown_30;
    if (team_type != 0) {
        team_type->Destroy_All_Of();
    }
    return true;
}


// bool TActionClass::unknown_func_6b1470(...) (0x006B1470)
// Reinforcements of the team type at +0x30 at its own waypoint; false without one.

bool TActionClass::unknown_func_6b1470(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_30 != 0) {
        return Do_Reinforcements((TeamTypeClass *)unknown_30, -1);
    }
    return false;
}


// bool TActionClass::unknown_func_6b1490(...) (0x006B1490)
// Reinforcements of the team type at +0x30 at the waypoint +0x44; false without either.

bool TActionClass::unknown_func_6b1490(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_30 != 0 && unknown_44 != -1) {
        return Do_Reinforcements((TeamTypeClass *)unknown_30, unknown_44);
    }
    return false;
}


// bool TActionClass::unknown_func_6b14b0(...) (0x006B14B0)
// Sends every unit of the house of type Data.Value to hunt (IHouse::All_To_Hunt); always true.

bool TActionClass::unknown_func_6b14b0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    HouseClass::As_Pointer(Data.Value)->All_To_Hunt();
    return true;
}


// bool TActionClass::unknown_func_6b14d0(...) (0x006B14D0)
// Kills every living, active techno whose tag has the trigger attached (repeating while any was hit), then scrolls the tactical view to the cell unless it is the unset cell; true when any was killed.

bool TActionClass::unknown_func_6b14d0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool killed = false;
    bool again;
    do {
        again = false;
        for (int index = 0; index < Technos.Count(); index++) {
            TechnoClass *techno = Technos[index];
            if (techno->Strength > 0 && techno->unknown_7d && techno->unknown_74 && !techno->unknown_76 && techno->unknown_34 != 0
                && techno->unknown_34->Is_Trigger_Attached(trigger)) {
                int damage = techno->Strength;
                techno->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
                killed = true;
                again = true;
                index--;
            }
        }
    } while (again);
    if (cell->X != unknown_geometry_k.X || cell->Y != unknown_geometry_k.Y) {
        bool visible = Map.Unknown_00569090(cell);
        int tries = 3;
        while (!visible && tries-- > 0) {
            visible = Map.Unknown_00569090(cell);
        }
        CoordStruct coord = CoordStruct(cell->X * 256 + 128, cell->Y * 256 + 128, 0);
        coord.Z = unknown_geometry_j;
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
        unknown_tactical_83996c->Register_Dirty_Area(Rect(pixel.X - 0x80, pixel.Y - 0x80, 0x100, 0x100), 0);
    }
    return killed;
}


// bool TActionClass::unknown_action_006b1640(...) (0x006B1640)
bool TActionClass::unknown_action_006b1640(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    return true;
}


// bool TActionClass::unknown_func_6b1650(...) (0x006B1650)
// Sets the map's local size from +0x34..+0x40, refits the radar and the subzones, clears every cell's overlay, shows the buildings and announces the new terrain; always true.
#include "shared/building_class.hpp"

bool TActionClass::unknown_func_6b1650(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Map.MapLocalSize = *(Rect const *)&unknown_34;
    Map.Reset_Radar();
    Map.Reset_Iterator();
    CellClass *walk = Map.Iterate();
    while (walk != 0) {
        walk->Recalc_Attributes(-1);
        walk = Map.Iterate();
    }
    Map.Zone_Reset();
    Map.Reset_All_Subzones();
    Map.Complete_Radar_Refresh();
    for (int index = unknown_BuildingClass_list_00a40af8.Count() - 1; index >= 0; index--) {
        unknown_BuildingClass_list_00a40af8[index]->unknown_slot267(true);
    }
    Speech_Play_Name("EVA_NewTerrainDiscovered", -1, -1);
    return true;
}


// bool TActionClass::unknown_func_6b1710(...) (0x006B1710)
// Creates the animation of type index Data.Value at the waypoint +0x44 (ground height, bridge level counted), flagged +0x178; always true.

bool TActionClass::unknown_func_6b1710(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    if ((Map[waypoint].unknown_140 & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
        coord.Z += unknown_geometry_j;
    }
    (new AnimClass(AnimTypes[Data.Value], coord, 0, 1, 0x600, 0))->unknown_178 = true;
    return true;
}


// bool TActionClass::unknown_weapon_explosion_at_waypoint(...) (0x006B1810)
// The explosion of weapon type Data.Value at waypoint +0x44 (ground height, raised on a bridge): its warhead's damage animation, Unknown_0047F600 and Unknown_0047E2C0 with the weapon's +0xA4 damage; always true.
#include "shared/weapon_type.hpp"

bool TActionClass::unknown_weapon_explosion_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    if ((Map[waypoint].unknown_140 & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
        coord.Z += unknown_geometry_j;
    }
    int index = Data.Value;
    WeaponTypeClass *weapon = unknown_WeaponTypeClass_list_00839b90[index];
    int damage = weapon->unknown_a4;
    int land = Map[coord].unknown_ec;
    AnimTypeClass *anim = Select_Damage_Anim(damage, weapon->Warhead, land, &coord);
    if (anim != 0) {
        new AnimClass(anim, coord, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(coord));
    }
    Unknown_0047F600(coord, damage, unknown_WeaponTypeClass_list_00839b90[index]->Warhead, 0, 0);
    Unknown_0047E2C0(coord, damage, 0, unknown_WeaponTypeClass_list_00839b90[index]->Warhead, 1);
    return true;
}


// bool TActionClass::unknown_func_6b19a0(...) (0x006B19A0)
// Creates a voxel animation of the type at index Data.Value at the waypoint +0x44 (ground height, bridge level counted); always true.
#include "shared/voxel_anim.hpp"
#include "shared/voxel_anim_type.hpp"

bool TActionClass::unknown_func_6b19a0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    unsigned int bridge_flags = Map[waypoint].unknown_140;
    if ((bridge_flags & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
        int bridge_height = unknown_geometry_j;
        coord.Z += bridge_height;
    }
    new VoxelAnimClass(VoxelAnimTypes[Data.Value], coord, 0);
    return true;
}


// bool TActionClass::unknown_func_6b1a80(...) (0x006B1A80)
// Unless Unknown_0051FC50 holds (then false), calls 0x0051F930 with Data.Value,
// the rules value at +0x13E8 and the zero cell (as TeamClass 0x006BC460);
// true.

bool TActionClass::unknown_func_6b1a80(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Unknown_0051FC50()) {
        return false;
    }
    Unknown_0051F930(Data.Value, Rules->unknown_13e8, unknown_geometry_k, 0);
    return true;
}


// bool TActionClass::unknown_action_006b1ac0(...) (0x006B1AC0)
#include "shared/lightning_storm.hpp"
bool TActionClass::unknown_action_006b1ac0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (!Unknown_0051FC50()) return false;
    Unknown_0051FBB0();
    return true;
}


// bool TActionClass::unknown_action_006b1ae0(...) (0x006B1AE0)
bool TActionClass::unknown_action_006b1ae0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Pause_Game_0065c3d0();
    return true;
}


// bool TActionClass::unknown_action_006b1af0(...) (0x006B1AF0)
bool TActionClass::unknown_action_006b1af0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Resume_Game_0065c420();
    return true;
}


// bool TActionClass::unknown_look_at_waypoint(...) (0x006B1B00)
// Tells the tactical view to look at the waypoint +0x44 (its cell centre at ground height, raised on a bridge) with Data.Value; always true.

bool TActionClass::unknown_look_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    UnknownCoord434820 coord = UnknownCoord434820(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    if ((Map[waypoint].unknown_140 & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
        coord.Z += unknown_geometry_j;
    }
    unknown_tactical_83996c->Setup_Trigger_Scroll(coord, Data.Value);
    return true;
}


// bool TActionClass::unknown_center_on_waypoint(...) (0x006B1BC0)
// Moves the tactical view to waypoint +0x44 (its cell centre at ground height, raised on a bridge); always true.

bool TActionClass::unknown_center_on_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    if ((Map[waypoint].unknown_140 & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
        coord.Z += unknown_geometry_j;
    }
    unknown_tactical_83996c->Set_Tactical_Position_006a66b0(coord);
    return true;
}


// bool TActionClass::unknown_func_6b1c80(...) (0x006B1C80)
// Resets the tactical zoom to the rules ZoomInFactor, clears keyboard input, hides the mouse, aborts a drag select, redraws and pauses a second; always true.
#include "shared/keyboard.hpp"
#include "shared/mouse.hpp"
#include "shared/win32_api.hpp"

bool TActionClass::unknown_func_6b1c80(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_tactical_83996c->unknown_c0 = Rules->ZoomInFactor;
    unknown_flag_a40d44 = true;
    WWKeyboard->Clear();
    WWMouse->Hide_Mouse();
    Map.Abort_Drag_Select();
    Map.Flag_To_Redraw(0);
    Map.Render();
    Sleep(1000);
    return true;
}


// bool TActionClass::unknown_func_6b1d00(...) (0x006B1D00)
// Undoes 0x006B1C80: tactical zoom 1.0, keyboard cleared, the input flag from the pause flag, mouse shown, redraw; always true.

bool TActionClass::unknown_func_6b1d00(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_tactical_83996c->unknown_c0 = 1.0;
    WWKeyboard->Clear();
    unknown_flag_a40d44 = unknown_id_source_a3d290->unknown_2306;
    WWMouse->Show_Mouse();
    Map.Flag_To_Redraw(0);
    Map.Render();
    return true;
}


// bool TActionClass::unknown_map_559fd0(...) (0x006B1D70)
// Calls Map.Unknown_00559FD0; always true.


bool TActionClass::unknown_map_559fd0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Map.Unknown_00559FD0();
    return true;
}


// bool TActionClass::unknown_spotlight_behavior_6b1d80(...) (0x006B1D80)
// Sets spotlight behavior Data.Value on every living, active spotlight building whose tag has the trigger attached; true when any.
#include "shared/building_light.hpp"
extern DynamicVectorClass<BuildingClass *> unknown_BuildingClass_list_00a40af8;

bool TActionClass::unknown_spotlight_behavior_6b1d80(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool handled = false;
    for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
        if (building->Strength > 0 && building->unknown_7d && building->unknown_74 && !building->unknown_76
            && building->Type->HasSpotlight && building->unknown_4d4 != 0
            && building->unknown_34 != 0 && building->unknown_34->Is_Trigger_Attached(trigger)) {
            building->unknown_4d4->Set_Behavior(Data.Value);
            handled = true;
        }
    }
    return handled;
}


// bool TActionClass::unknown_func_6b1e20(...) (0x006B1E20)
// Deletes every trigger whose type is the one at +0x50 (the count is read
// once; the index steps back after each deletion); always true.
#include "shared/trigger.hpp"

bool TActionClass::unknown_func_6b1e20(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_50 != 0) {
        int count = Triggers.Count();
        for (int index = 0; index < count; index++) {
            if (Triggers[index]->Class == (TriggerTypeClass *)unknown_50) {
                delete Triggers[index];
                index--;
            }
        }
    }
    return true;
}


// bool TActionClass::unknown_func_6b1e70(...) (0x006B1E70)
// Deletes every tag whose type is the one at +0x4C (the count is read
// once; the index steps back after each deletion); always true.

bool TActionClass::unknown_func_6b1e70(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_4c != 0) {
        int count = Tags.Count();
        for (int index = 0; index < count; index++) {
            if (Tags[index]->Class == (TagTypeClass *)unknown_4c) {
                delete Tags[index];
                index--;
            }
        }
    }
    return true;
}


// bool TActionClass::unknown_func_6b1ec0(house, object, trigger, cell) (0x006B1EC0)
// Springs (forced, no object) every trigger whose type is the one at +0x50; always true.

bool TActionClass::unknown_func_6b1ec0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_50 != 0) {
        for (int index = 0; index < Triggers.Count(); index++) {
            if ((AbstractClass *)Triggers[index]->Class == unknown_50) {
                Triggers[index]->Spring(0, unknown_geometry_k);
            }
        }
    }
    return true;
}


// bool TActionClass::unknown_func_6b1f10(...) (0x006B1F10)
// Enables every trigger whose type is the one at +0x50 and is enabled for
// the scenario's difficulty (as 0x006B1F90 disables them); always true.

bool TActionClass::unknown_func_6b1f10(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_50 != 0) {
        for (int index = 0; index < Triggers.Count(); index++) {
            TriggerTypeClass *type = Triggers[index]->Class;
            if ((AbstractClass *)type == unknown_50) {
                if ((unknown_id_source_a3d290->Difficulty != 0 || type->IsEnabledOnEasy)
                        && (unknown_id_source_a3d290->Difficulty != 1 || type->IsEnabledOnMedium)
                        && (unknown_id_source_a3d290->Difficulty != 2 || type->IsEnabledOnHard)) {
                    Triggers[index]->Enable();
                }
            }
        }
    }
    return true;
}


// bool TActionClass::unknown_func_6b1f90(house, object, trigger, cell) (0x006B1F90)
// Disables every trigger whose type is the one at +0x50; always true.

bool TActionClass::unknown_func_6b1f90(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_50 != 0) {
        for (int index = 0; index < Triggers.Count(); index++) {
            if ((AbstractClass *)Triggers[index]->Class == unknown_50) {
                Triggers[index]->Disable();
            }
        }
    }
    return true;
}


// bool TActionClass::unknown_63b5b0_at_waypoint(...) (0x006B1FD0)
// Calls Unknown_0063B5B0(Data.Value, cell of waypoint +0x44); always true.
bool __fastcall Unknown_0063B5B0(int kind, Cell cell);   // 0x0063B5B0

bool TActionClass::unknown_63b5b0_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_0063B5B0(Data.Value, unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44));
    return true;
}


// bool TActionClass::unknown_set_global_variable(...) (0x006B2000)
// Sets global variable Data.Value (ScenarioClass::unknown_661560); always true.

bool TActionClass::unknown_set_global_variable(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_661560(Data.Value, true);
    return true;
}


// bool TActionClass::unknown_clear_global_variable(...) (0x006B2020)
// Clears global variable Data.Value (ScenarioClass::unknown_661560); always true.

bool TActionClass::unknown_clear_global_variable(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_661560(Data.Value, false);
    return true;
}


// bool TActionClass::unknown_meteor_shower_at_waypoint(...) (0x006B2040)
// METLARGE or METSMALL animations (random) at random offsets around waypoint +0x44 (ground height); their count is abs(random) % 3 plus the size table entry Data.Value, the radius 70 leptons per meteor; always true.
#include "shared/crt_memory.hpp"
extern int const unknown_meteor_counts_7f6ad8[4];    // 0x007F6AD8 (1, 5, 9, 15: the meteors of each shower size)

// A random offset of up to radius leptons in X and Y (inline; name hypothesis).
inline CoordStruct Random_Offset(int radius)
{
    Random2Class &random = unknown_id_source_a3d290->Random();
    int x = random() % radius;
    int y = random() % radius;
    return CoordStruct(x, y, 0);
}

bool TActionClass::unknown_meteor_shower_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    if ((Map[waypoint].unknown_140 & 0x100) || (Map[waypoint].unknown_140 & 0x400)) {
        coord.Z += unknown_geometry_j;
    }
    int count = abs(unknown_id_source_a3d290->Random()()) % 3 + unknown_meteor_counts_7f6ad8[Data.Value];
    int large = Anim_From_Name("METLARGE");
    int small = Anim_From_Name("METSMALL");
    int radius = count * 70;
    for (int index = 0; index < count; index++) {
        CoordStruct spot = coord + Random_Offset(radius);
        spot.Z = Map.Get_Height_GL(spot);
        new AnimClass(AnimTypes[(abs(unknown_id_source_a3d290->Random()()) % 2) ? large : small], spot, 0, 1, 0x600, 0);
    }
    return true;
}


// bool TActionClass::unknown_func_6b2220(...) (0x006B2220)
// Sets the rules AmbientChangeStep to the float Data.Value; always true.

bool TActionClass::unknown_func_6b2220(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    float change_step = Data.Float;
    Rules->AmbientChangeStep = change_step;
    return true;
}


// bool TActionClass::unknown_func_6b2240(...) (0x006B2240)
// Sets the rules AmbientChangeRate to the float Data.Value; always true.

bool TActionClass::unknown_func_6b2240(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    float change_rate = Data.Float;
    Rules->AmbientChangeRate = change_rate;
    return true;
}


// bool TActionClass::unknown_set_session_22a8(...) (0x006B2260)
// Sets the ID source's +0x22A8 (the lighting ambient) to Data.Value and copies it
// to +0x22B0 unless 0x0051FC50 holds; always true.

bool TActionClass::unknown_set_session_22a8(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_22a8 = Data.Value;
    if (!Unknown_0051FC50()) {
        unknown_id_source_a3d290->unknown_22b0 = unknown_id_source_a3d290->unknown_22a8;
    }
    return true;
}


// bool TActionClass::unknown_func_6b2290(...) (0x006B2290)
// Sets +0x13A of the house of type Data.Value (none for -1); always true.

bool TActionClass::unknown_func_6b2290(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != -1) {
        HouseClass::As_Pointer(Data.Value)->unknown_13a = true;
    }
    return true;
}


// bool TActionClass::unknown_func_6b22b0(...) (0x006B22B0)
// Clears +0x13A of the house of type Data.Value (none for -1); always true.

bool TActionClass::unknown_func_6b22b0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != -1) {
        HouseClass::As_Pointer(Data.Value)->unknown_13a = false;
    }
    return true;
}


// bool TActionClass::unknown_func_6b22d0(...) (0x006B22D0)
// Every techno of the house of type Data.Value (none for -1) cheers; always true.

bool TActionClass::unknown_func_6b22d0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value != -1) {
        HouseClass::As_Pointer(Data.Value)->Cheer();
    }
    return true;
}


// bool TActionClass::unknown_action_006b22f0(...) (0x006B22F0)
bool TActionClass::unknown_action_006b22f0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (house != 0) house->unknown_554c = Data.Value;
    return true;
}


// bool TActionClass::unknown_action_006b2310(...) (0x006B2310)
bool TActionClass::unknown_action_006b2310(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (house != 0) house->unknown_5550 = Data.Value;
    return true;
}


// bool TActionClass::unknown_action_006b2330(...) (0x006B2330)
bool TActionClass::unknown_action_006b2330(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (house != 0) house->unknown_5554 = Data.Value;
    return true;
}


// bool TActionClass::unknown_action_006b2350(...) (0x006B2350)
bool TActionClass::unknown_action_006b2350(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (house != 0) house->unknown_5558 = Data.Value;
    return true;
}


// bool TActionClass::unknown_ion_blast_at_waypoint(...) (0x006B2370)
// Starts a new ion blast at waypoint +0x44 (cell centre, ground height); always true.
#include "shared/ion_blast.hpp"

bool TActionClass::unknown_ion_blast_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    new IonBlastClass(coord);
    return true;
}


// bool TActionClass::unknown_nuke_at_waypoint(...) (0x006B2400)
// A NukePayload bullet (speed 50) aimed at the cell of waypoint +0x44 (ground height, raised on a bridge), given the weapon, placed 20000 leptons above it and launched straight down at speed 100 (the pitch of 0x00463C50); false when the bullet cannot be created. Like 0x00463C50 the weapon is set before the null test.
#include "shared/bullet.hpp"

bool TActionClass::unknown_nuke_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    if (Map[waypoint].unknown_140 & 0x100) {
        coord.Z += unknown_geometry_j;
    }
    WeaponTypeClass *payload = unknown_WeaponTypeClass_list_00839b90[WeaponTypeClass::Index_From_Name("NukePayload")];
    BulletTypeClass *projectile = payload->Projectile;
    BulletClass *bullet = Create_Bullet(projectile, (AbstractClass *)&Map[coord], 0, payload->unknown_a4, payload->Warhead, 50, false);
    bullet->Set_Weapon_Type(payload);
    CoordStruct launch = coord + CoordStruct(0, 0, 20000);
    if (bullet != 0) {
        double y = unknown_sin_4ba480(1.5707483884323126);
        y *= unknown_cos_4ba4d0(1.5707483884323126);
        y *= -100.0;
        TVelocity2D<double> planar(unknown_cos_4ba4d0(1.5707483884323126) * unknown_cos_4ba4d0(1.5707483884323126) * -100.0, y);
        TVelocity3D<double> velocity;
        velocity.XY() = planar;
        velocity.Z = unknown_sin_4ba480(1.5707483884323126) * -100.0;
        bullet->unknown_slot118(launch, velocity);
        return true;
    }
    return false;
}


// bool TActionClass::unknown_func_6b25e0(...) (0x006B25E0)
// Starts the lightning storm (0x0051F930) over the cell of waypoint +0x44 (bridge level counted) with the rules'
// LightningStormDuration and LightningDeferment for house, unless Unknown_0051FC50 holds (then false); true otherwise.

bool TActionClass::unknown_func_6b25e0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    if (Map[waypoint].unknown_140 & 0x100) {
        coord.Z += unknown_geometry_j;
    }
    Cell target = coord.As_Cell();
    if (!Unknown_0051FC50()) {
        Unknown_0051F930(Rules->LightningStormDuration, Rules->unknown_13e8, target, house);
        return true;
    }
    return false;
}


// Applies Iron Curtain to occupants around the waypoint and starts its invocation animation.
extern int const unknown_radius_count_7a5908[];   // 0x007A5908 cells within each radius (MapClass::Sight_From 0x0054A110)
extern Cell const unknown_cell_offsets_a6f248[];

bool TActionClass::unknown_iron_curtain_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell target_cell;
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct position = Map[waypoint].Center_Coord();
    new AnimClass(Rules->IronCurtainInvokeAnim, position + CoordStruct(0,0,5), 0,1,0x600,0);
    int count = unknown_radius_count_7a5908[1];
    for (int index = 0; index < count; ++index) {
        Cell const &offset = unknown_cell_offsets_a6f248[index];
        target_cell = TPoint2D<short>((short)(waypoint.X + offset.X), (short)(waypoint.Y + offset.Y));
        CellClass &target = Map[target_cell];
        ObjectClass *occupier = target.OccupierPtr;
        while (occupier != 0) {
            occupier->unknown_slot82(Rules->IronCurtainDuration);
            occupier = occupier->unknown_30;
        }
    }
    return true;
}


// Changes the type indexed by Data.Value and refreshes the house tech state, native006B27E0.
#include "shared/techno_type.hpp"
bool TActionClass::Set_Type_Tech_Level_006b27e0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    TechnoTypes[Data.Value]->TechLevel = unknown_48;
    if (house != 0) house->unknown_144 = true;
    return true;
}


// bool TActionClass::unknown_func_6b2810(...) (0x006B2810)
// Unknown_0063A880 with the team type at +0x30 and the waypoint +0x44; false without either.

bool TActionClass::unknown_func_6b2810(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_30 != 0 && unknown_44 != -1) {
        return Unknown_0063A880((TeamTypeClass *)unknown_30, unknown_44);
    }
    return false;
}


// bool TActionClass::unknown_map_54e580_at_waypoint(...) (0x006B2830)
// Calls Map.Unknown_0054E580 with the cell of waypoint +0x44 and Data.Value; returns its result.


bool TActionClass::unknown_map_54e580_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    return Map.Unknown_0054E580(waypoint, Data.Value);
}


// bool TActionClass::unknown_chem_missile_at_waypoint(...) (0x006B2860)
// A ChemLauncher bullet (speed 20) aimed at the cell of waypoint +0x44 (ground height, raised on a bridge), launched at speed 100 from the ground centre of the cell Map 0x005689D0 picks near it; false when the bullet cannot be created.

bool TActionClass::unknown_chem_missile_at_waypoint(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Cell waypoint = unknown_id_source_a3d290->Get_Waypoint_Location(unknown_44);
    CoordStruct coord = CoordStruct(waypoint);
    coord.Z = Map.Get_Height_GL(coord);
    if (Map[waypoint].unknown_140 & 0x100) {
        coord.Z += unknown_geometry_j;
    }
    Cell from = Map.Unknown_005689D0(Coord_Cell(coord), 0);
    WeaponTypeClass *launcher = unknown_WeaponTypeClass_list_00839b90[WeaponTypeClass::Index_From_Name("ChemLauncher")];
    BulletTypeClass *projectile = launcher->Projectile;
    BulletClass *bullet = Create_Bullet(projectile, (AbstractClass *)&Map[coord], 0, launcher->unknown_a4, launcher->Warhead, 20, false);
    if (bullet != 0) {
        double y = unknown_cos_4ba4d0(-9.587672516830327e-05);
        y *= unknown_sin_4ba480(1.5707483884323126);
        y *= 100.0;
        TVelocity2D<double> planar(unknown_cos_4ba4d0(-9.587672516830327e-05) * unknown_cos_4ba4d0(1.5707483884323126) * 100.0, y);
        TVelocity3D<double> velocity;
        velocity.XY() = planar;
        velocity.Z = unknown_sin_4ba480(-9.587672516830327e-05) * 100.0;
        bullet->unknown_slot118(CoordStruct(from.X * 256 + 128, from.Y * 256 + 128, 0), velocity);
        return true;
    }
    return false;
}


// bool TActionClass::unknown_toggle_session_flag_2229(...) (0x006B2A90)
// Toggles the session byte +0x2229; always true.


bool TActionClass::unknown_toggle_session_flag_2229(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    unknown_id_source_a3d290->unknown_2229 = !unknown_id_source_a3d290->unknown_2229;
    return true;
}


// static int __fastcall TActionClass::Value_Kind(int type) (0x006B2AB0)
// What the value of an action type names (the kinds Build_INI_Entry and the
// trigger editor use); 0 for a plain number (name hypothesis).

int __fastcall TActionClass::Value_Kind(int type)
{
    switch (type) {
        case 11:
        case 103:
            return 39;
        case 33:
        case 34:
            return 17;
        case 1:
        case 2:
        case 3:
        case 6:
        case 9:
        case 13:
        case 14:
        case 36:
        case 37:
        case 38:
        case 74:
        case 75:
        case 113:
            return 13;
        case 30:
        case 85:
        case 86:
        case 87:
            return 16;
        case 4:
        case 5:
        case 7:
        case 105:
            return 12;
        case 104:
            return 40;
        case 80:
        case 107:
            return 34;
        case 88:
            return 35;
        case 12:
        case 22:
        case 53:
        case 54:
            return 11;
        case 20:
            return 1;
        case 10:
        case 100:
        case 117:
            return 2;
        case 19:
        case 98:
            return 3;
        case 21:
            return 4;
        case 70:
            return 26;
        case 106:
        case 115:
            return 43;
        case 28:
        case 29:
            return 22;
        case 56:
        case 57:
            return 29;
        case 35:
            return 15;
        case 40:
            return 21;
        case 41:
            return 23;
        case 99:
            return 38;
        case 8:
        case 17:
        case 18:
        case 59:
        case 63:
        case 64:
        case 65:
        case 66:
        case 89:
        case 90:
        case 94:
        case 95:
        case 96:
        case 101:
        case 102:
        case 109:
        case 112:
        case 116:
            return 9;
        case 42:
            return 24;
        case 43:
            return 27;
        case 48:
            return 30;
        case 25:
        case 26:
        case 27:
        case 39:
        case 44:
        case 73:
        case 76:
        case 77:
        case 78:
        case 79:
        case 84:
        case 110:
        case 114:
            return 10;
        case 52:
            return 25;
        case 55:
            return 28;
        case 58:
            return 31;
        case 71:
        case 72:
            return 33;
        case 108:
            return 44;
    }
    return 0;
}


// HRESULT __stdcall TActionClass::GetClassID(GUID *clsid) (0x006B2C80)

HRESULT __stdcall TActionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TActionClass;
    return S_OK;
}


// HRESULT __stdcall TActionClass::Load(IStream *stream) (0x006B2CC0)

HRESULT __stdcall TActionClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TActionClass(NoInitClass());
    Swizzler.Swizzle((void **)&Next);
    Swizzler.Swizzle((void **)&unknown_30);
    Swizzler.Swizzle((void **)&unknown_4c);
    Swizzler.Swizzle((void **)&unknown_50);
    return S_OK;
}


// HRESULT __stdcall TActionClass::Save(IStream *stream, BOOL clear_dirty) (0x006B2D40)

HRESULT __stdcall TActionClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// void TActionClass::Compute_CRC(WWCRCEngine &crc) const (0x006B2D60)

void TActionClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    if (Next != 0) {
        crc(Next->Fetch_ID());
    }
    crc((long)Type);
    if (unknown_30 != 0) {
        crc(unknown_30->Fetch_ID());
    }
    if (unknown_4c != 0) {
        crc(unknown_4c->Fetch_ID());
    }
    if (unknown_50 != 0) {
        crc(unknown_50->Fetch_ID());
    }
    crc((long)Data.Value);
}


// static int __fastcall TActionClass::Action_Flags(int type) (0x006B2DF0)
// 2 for the action types that need a waypoint (hypothesis), else 0.

int __fastcall TActionClass::Action_Flags(int type)
{
    int flags = 0;
    switch (type) {
        case 14:
        case 32:
        case 60:
        case 61:
        case 62:
        case 91:
        case 111:
            flags = 2;
            break;
    }
    return flags;
}


// CoordStruct TActionClass::unknown_6b2e80() const (0x006B2E80)
// The coordinate of the waypoint Data.Value (ScenarioClass 0x006631C0).

CoordStruct TActionClass::unknown_6b2e80() const
{
    return unknown_id_source_a3d290->Waypoint_Coord(Data.Value);
}


// void TActionClass::Set_Text(char const *text) (0x006B2EC0)
// Keeps text without its leading and trailing white space (31 characters).

void TActionClass::Set_Text(char const *text)
{
    Text[0] = 0;
    if (text != 0) {
        while (*text != 0 && (unsigned char)*text <= ' ') {
            text++;
        }
        if (*text != 0) {
            strncpy(Text, text, sizeof(Text) - 1);
            Text[sizeof(Text) - 1] = 0;
            int length = strlen(Text);
            while (length != 0 && (unsigned char)Text[length - 1] <= ' ') {
                length--;
            }
            Text[length] = 0;
        }
    }
}


// char const *TActionClass::Get_Text() const (0x006B2F20)

char const *TActionClass::Get_Text() const
{
    return Text[0] ? Text : 0;
}


// bool TActionClass::unknown_func_6b2f30(...) (0x006B2F30)
// Passes Data.Value to every team of the team type at +0x30 (0x006C0110); always true.

bool TActionClass::unknown_func_6b2f30(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_30 != 0) {
        ((TeamTypeClass *)unknown_30)->unknown_func_6c0110(Data.Value);
    }
    return true;
}


// bool TActionClass::unknown_func_6b2f50(...) (0x006B2F50)
// The first member of the team type's first team gets the talk bubble with Data.Value
// (TechnoClass 0x006D8FD0); without a team type it is cleared. Always true.

bool TActionClass::unknown_func_6b2f50(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_30 != 0) {
        TeamClass *team = ((TeamTypeClass *)unknown_30)->Find_Team();
        if (team != 0) {
            TechnoClass::unknown_6d8fd0(team->Member, Data.Value);
        }
    } else {
        TechnoClass::unknown_6d8fd0(0, 0);
    }
    return true;
}


// bool TActionClass::unknown_func_6b2f90(...) (0x006B2F90)
// Passes Data.Value to 0x0065C1F0 (the radar movie's flag handler); always true.

bool TActionClass::unknown_func_6b2f90(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    Unknown_0065C1F0(Data.Value);
    return true;
}


// bool TActionClass::unknown_func_6b2fa0(...) (0x006B2FA0)
// Runs BuildingClass::unknown_452f50(0) on the first living, active building whose tag has the action's trigger attached; true when one was found.
extern DynamicVectorClass<BuildingClass *> unknown_BuildingClass_list_00a40af8;

bool TActionClass::unknown_func_6b2fa0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    bool handled = false;
    for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
        if (building->unknown_7d && building->unknown_74 && !building->unknown_76 && building->unknown_34 != 0
            && building->unknown_34->Is_Trigger_Attached(trigger)) {
            building->unknown_452f50(0);
            handled = true;
            break;
        }
    }
    return handled;
}


// bool TActionClass::unknown_select_sidebar_tab(...) (0x006B3010)
// Selects sidebar tab Data.Value (0..3) when its strip has buildables; true then.

bool TActionClass::unknown_select_sidebar_tab(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    int tab = Data.Value;
    if (tab < 4 && tab >= 0 && Map.Column[tab].Unknown_00681730()) {
        Map.Select_Tab(tab);
        return true;
    }
    return false;
}


// bool TActionClass::unknown_sidebar_6822b0(...) (0x006B3060)
// Map.Unknown_006822B0 with techno type Data.Value (when valid) and +0x48; true then.

bool TActionClass::unknown_sidebar_6822b0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (Data.Value < TechnoTypes.Count() && Data.Value >= 0) {
        Map.Unknown_006822B0(TechnoTypes[Data.Value], unknown_48);
        return true;
    }
    return false;
}


// int TActionClass::Size_Of() const (0x006B30A0)

int TActionClass::Size_Of() const
{
    return sizeof(TActionClass);
}


// int TActionClass::Kind_Of() const (0x006B30B0)

int TActionClass::Kind_Of() const
{
    return 47;
}


// int TActionClass::Fetch_Heap_ID() const (0x006B30C0)

int TActionClass::Fetch_Heap_ID() const
{
    return unknown_24;
}


// TActionClass scalar deleting destructor (0x006B30D0): compiler generated (the destructor expanded inline); no source of its own.


#include "shared/fixed_string.hpp"


// FixedString<32> assignment returns the copied value, original006B3230 complete41+7/RET8.
template<> FixedString<32> FixedString<32>::operator=(FixedString<32> const &other)
{
    if (&other != this) memcpy(Buffer, other.Buffer, 32);
    return *this;
}


// 0x006b3200: function not yet identified
//
// Body: 41 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: char * __thiscall FUN_006b3200(char *param_1,char *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006b3200 {

extern "C" unsigned int __cdecl _strncpy(...);  // @ref __strncpy=007823d0

struct Owner_006b3200 { char * FUN_006b3200(char *source_text); };

char * Owner_006b3200::FUN_006b3200(char *source_text)
{
    char * destination = (char *)this;
    if (source_text == (char *)0x0) {
        *destination = '\0';
        return destination;
    }
    _strncpy(destination,source_text,0x1f);
    destination[0x1f] = '\0';
    return destination;
}

}  // namespace seed_006b3200


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TActionClass *>::Clear (0x006B3260; DynamicVectorClass<TActionClass *> vtable 0x007ac5f0 slot 3).
// DynamicVectorClass<TActionClass *>::ID_pointer (0x006B3290; DynamicVectorClass<TActionClass *> vtable 0x007ac5f0 slot 5).
// VectorClass<TActionClass *>::Fetch (0x006B32B0; VectorClass<TActionClass *> vtable 0x007ac610 slot 6).
// VectorClass<TActionClass *>::operator equal (0x006B3300; VectorClass<TActionClass *> vtable 0x007ac610 slot 1).
// VectorClass<TActionClass *>::Resize (0x006B3340; VectorClass<TActionClass *> vtable 0x007ac610 slot 2).
// VectorClass<TActionClass *>::Clear (0x006B33F0; VectorClass<TActionClass *> vtable 0x007ac610 slot 3).
// VectorClass<TActionClass *>::ID_object (0x006B3420; VectorClass<TActionClass *> vtable 0x007ac610 slot 4).
// VectorClass<TActionClass *>::ID_pointer (0x006B3460; VectorClass<TActionClass *> vtable 0x007ac610 slot 5).
// DynamicVectorClass<TActionClass *>::Resize (0x006B3480; DynamicVectorClass<TActionClass *> vtable 0x007ac5f0 slot 2).
// DynamicVectorClass<TActionClass *>::ID_object (0x006B3530; DynamicVectorClass<TActionClass *> vtable 0x007ac5f0 slot 4).


extern "C" unsigned char vtable_6b3560[]; extern "C" void __cdecl release_buffer_6b3560(void*); extern "C" void __cdecl release_object_6b3560(void*);
class Leaf_6b3560 { public: void* get(unsigned int flags); };
void* Leaf_6b3560::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6b3560; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6b3560((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6b3560(this); return this; }


extern "C" unsigned char vtable_6b35b0[]; extern "C" void __cdecl release_buffer_6b35b0(void*); extern "C" void __cdecl release_object_6b35b0(void*);
class Leaf_6b35b0 { public: void* get(unsigned int flags); };
void* Leaf_6b35b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6b35b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6b35b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6b35b0(this); return this; }
