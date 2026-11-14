// team.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/team.json carries the addresses and the rows that
// stay in their own files.


#define RA2_TEAM_AI_HANDLERS
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/unit.hpp"
#include "shared/foot.hpp"
#include "shared/cell.hpp"
#include "shared/aircraft.hpp"
#include "shared/team.hpp"
#include "shared/team_type.hpp"
#include "shared/house.hpp"
#include "shared/tag.hpp"
#include "shared/map_view.hpp"
#include "shared/crt_memory.hpp"
#include "shared/ai_trigger_type.hpp"
#include "shared/task_force.hpp"
#include "shared/rules.hpp"
#include "shared/scenario_tags.hpp"
#include "shared/theme.hpp"
#include "shared/debug_printf.hpp"
#include "shared/techno_type.hpp"
#include "shared/coord_distance.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/tube.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/id_source.hpp"
#include "shared/unit_type.hpp"
#include "shared/script.hpp"
#include "shared/tactical.hpp"
#include "shared/anim.hpp"
#include "shared/facing.hpp"
#include "shared/fast_math.hpp"
#include "shared/building_light.hpp"
#include "shared/super.hpp"


// Header-static dynamic initializers of the unit at 0x006B7240:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006B7240-0x006B7426); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006B7270, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B7290, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B72B0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B72D0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B72F0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B7310, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B7350, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B7380, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B73B0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B73E0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B73F0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// Dynamic initializer (0x006B7410, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006b7240.cpp; no source of its own.


// TeamClass::TeamClass(TeamTypeClass *type, HouseClass *owner, int arg3) (0x006B7430)
// Creates a team of a type for a house: joins the team list and the abstract
// lists, counts itself in its type (and the owner's team count), makes the
// script and tag, and heads for the type's waypoint.

extern DynamicVectorClass<AbstractClass *> unknown_objects_ac0cb0;
extern DynamicVectorClass<AbstractClass *> unknown_objects_ac0d30;

TeamClass::TeamClass(TeamTypeClass *type, HouseClass *owner, int arg3) :
    Class(type),
    Script(0),
    Owner(owner),
    unknown_30(0),
    unknown_34(0),
    unknown_38(0),
    unknown_3c(0),
    unknown_40(0),
    unknown_44((AbstractClass *)arg3),
    unknown_48(0),
    unknown_4c(0),
    unknown_50(Frame),
    Member(0),
    unknown_58(),
    unknown_64(0L),
    Tag(0),
    IsTransient(false),
    NeedsReGrouping(false),
    GuardSlowerIsNotUnderStrength(false),
    IsForcedActive(false),
    IsHasBeen(false),
    IsFullStrength(false),
    IsUnderStrength(true),
    IsReforming(false),
    IsLagging(false),
    NeedsToDisappear(true),
    JustDisappeared(false),
    IsMoving(false),
    StepCompleted(true),
    TargetNotAssigned(false),
    IsLeavingMap(false),
    IsSuspended(false)
{
    Teams.Add(this);
    unknown_objects_ac0cb0.Add(this);
    unknown_objects_ac0c58.Add(this);
    unknown_AbstractClass_list_00ac0d60.Add(this);
    unknown_objects_ac0d30.Add(this);
    memset(unknown_84, 0, sizeof(unknown_84));
    if (Class != 0) {
        if (Owner != 0 && Class->IsBaseDefense) {
            Owner->unknown_555c++;
        }
        if (Class->Waypoint_Location() != unknown_geometry_k) {
            unknown_34 = (AbstractClass *)&Map[Class->Waypoint_Location()];
        }
        Class->unknown_d4++;
        if (Class->Tag != 0) {
            Tag = new TagClass(Class->Tag);
        }
        Script = new ScriptClass(Class->Script);
    }
}


// TeamClass::~TeamClass() (0x006B7770)
// Reports the team's outcome to the AI trigger types that use its type,
// releases its members and tag, leaves the lists and deletes a transient
// team type.

extern bool unknown_flag_a40958;            // 0x00A40958
extern DynamicVectorClass<AbstractClass *> unknown_objects_ac0cb0;
extern DynamicVectorClass<AbstractClass *> unknown_objects_ac0d30;

TeamClass::~TeamClass()
{
    for (int index = 0; index < AITriggerTypes.Count(); index++) {
        if (AITriggerTypes[index]->unknown_dc == Class) {
            if (IsSuspended) {
                AITriggerTypes[index]->Register_Success();
            } else {
                AITriggerTypes[index]->Register_Failure();
            }
        }
    }
    if (Owner != 0 && Class != 0 && Class->IsBaseDefense) {
        Owner->unknown_555c--;
    }
    if (unknown_flag_a40958 && Class != 0) {
        while (Member != 0) {
            Unknown_006B8FB0(Member, -1, 0);
        }
        Class->unknown_d4--;
        if (Tag != 0) {
            if (Tag->AttachCount == 0) {
                Tag->Mark_To_Delete();
            }
            Tag = 0;
        }
    }
    Unknown_006ECB90(this, true);
    unknown_objects_ac0d30.Delete(this);
    unknown_AbstractClass_list_00ac0d60.Delete(this);
    unknown_objects_ac0cb0.Delete(this);
    unknown_objects_ac0c58.Delete(this);
    Teams.Delete(this);
    if (IsTransient && Class != 0) {
        delete Class;
    }
}


// Sets the team's target (+0x3C, with +0x40 following it when unset or equal), first sending members that were working on the old target back to guard; a cell target (kind 0x0B) not on the map flags +0x81 and sends the members to it.

void TeamClass::unknown_func_6b79e0(AbstractClass *target)
{
    if (target != unknown_3c) {
        FootClass *member = Member;
        if (unknown_3c != 0) {
            for (; member != 0; member = (FootClass *)member->unknown_4c8) {
                bool target_hit = member->unknown_228 == unknown_3c;
                bool destination_hit = member->unknown_494 == unknown_3c;
                if (target_hit || destination_hit) {
                    member->QueueMission(5, false);
                    if (destination_hit) {
                        member->Assign_Destination(0, true);
                    }
                    if (target_hit) {
                        member->unknown_slot220(0);
                    }
                }
            }
        }
    }
    if (unknown_40 == unknown_3c || unknown_40 == 0) {
        unknown_40 = target;
    }
    unknown_3c = target;
    if (target != 0 && target->Kind_Of() == 0xB) {
        if (!Map.Unknown_0055A740((CellClass *)target, 1)) {
            TargetNotAssigned = true;
            for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
                member->Assign_Destination(0, true);
            }
        } else {
            TargetNotAssigned = false;
        }
    }
}


// void TeamClass::AI() (0x006B7AD0; slot 23): Red Alert's/TS TeamClass::AI. Waits out a suspension,
// recalculates the strength flags after the roster changed, regroups or flags the team into action,
// recalculates its center, recruits, disbands an empty team (springing the leaves-map tags first),
// then advances and runs the script step (the handlers below; VC6 expands the small ones) or
// coordinates the regroup or the move.


// CDTimerClass::Value() for the timer type whose class is not recovered.
inline long Timer_Value(UnknownTimer423F60 const &timer)
{
    long remain = timer.DelayTime;
    if (timer.Started != -1) {
        long value = timer.Timer - timer.Started;
        if (value < remain) {
            return remain - value;
        } else {
            return 0;
        }
    }
    return remain;
}

void TeamClass::AI()
{
    if (IsLeavingMap) {
        if (Timer_Value(unknown_64) != 0) {
            return;
        }
        IsLeavingMap = false;
    }
    if (NeedsToDisappear) {
        if (!unknown_func_6b8bb0()) {
            return;
        }
    }
    if (IsMoving && IsUnderStrength) {
        unknown_func_6b88b0();
    }
    if (!IsMoving && (IsFullStrength || IsForcedActive)) {
        unknown_func_6b8860();
    }
    if (IsReforming || IsMoving || unknown_34 == 0 || unknown_38 == 0) {
        unknown_func_6b9610(&unknown_34, &unknown_38);
    }
    if ((!IsMoving || (!IsFullStrength && Class->Reinforce)) && (!Owner->Unknown_004F52F0() || !IsHasBeen)) {
        for (int index = 0; index < Class->TaskForce->CountEntries; index++) {
            if (unknown_84[index] < Class->TaskForce->Entries[index].Amount) {
                unknown_func_6b91c0(index);
            }
        }
    }
    if (Member == 0 && (IsHasBeen || (unknown_game_mode_a3d298 != 0 && Frame - unknown_50 > Rules->DissolveUnfilledTeamDelay))) {
        if (TargetNotAssigned) {
            for (int index = 0; index < unknown_tags_866160.Count(); index++) {
                TagClass *tag = unknown_tags_866160[index];
                if (tag->Spring(0x17, 0, unknown_geometry_k)) {
                    index--;
                    if (unknown_tags_866160.Count() == 0) {
                        break;
                    }
                }
            }
        }
        delete this;
        return;
    }
    bool leaving = Member != 0 && Member->unknown_slot112();
    if (IsMoving && !IsReforming && !IsUnderStrength && !leaving) {
        bool is_next = false;
        if (StepCompleted) {
            is_next = true;
            StepCompleted = false;
            Script->Next_Mission();
            for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
                if (Class->TransportsReturnOnUnload && member->Techno_Type_Class()->Passengers > 0) {
                    if (member->unknown_1b0 != 0) {
                        Debug_Printf("A Transport just recieved orders to go home after unloading\n");
                    }
                } else {
                    member->unknown_6d6e50(0);
                }
            }
            if (Script->Has_Mission()) {
                unknown_func_6b79e0(0);
                unknown_40 = 0;
            } else {
                delete this;
                return;
            }
        }
        if (!is_next) {
            if (unknown_40 == 0) {
                unknown_40 = unknown_3c;
            }
        }
        ScriptMissionClass mission = Script->Get_Current_Mission();
        switch (mission.Mission) {
        case 0x2C: TMission_unknown_6bc5f0(mission, is_next); break;
        case 0x18: TMission_unknown_6bc250(mission, is_next); break;
        case 0x2D: TMission_unknown_6bc660(mission, is_next); break;
        case 0x26: TMission_unknown_6bbe90(mission, is_next); break;
        case 0x25: TMission_unknown_6bc410(mission, is_next); break;
        case 0x16: TMission_unknown_6badc0(mission, is_next); break;
        case 0x04: TMission_unknown_6bae00(mission, is_next); break;
        case 0x03: TMission_unknown_6bae60(mission, is_next); break;
        case 0x01: TMission_unknown_6bb020(mission, is_next); break;
        case 0x10: TMission_unknown_6bb0f0(mission, is_next); break;
        case 0x0F: TMission_unknown_6bb260(mission, is_next); break;
        case 0x15: TMission_unknown_6bb310(mission, is_next); break;
        case 0x14: TMission_unknown_6bb350(mission, is_next); break;
        case 0x12: TMission_unknown_6bb3b0(mission, is_next); break;
        case 0x11: TMission_unknown_6bb420(mission, is_next); break;
        case 0x00: TMission_unknown_6bb480(mission, is_next); break;
        case 0x0E: TMission_unknown_6bb5f0(mission, is_next); break;
        case 0x09: TMission_unknown_6bb8b0(mission, is_next); break;
        case 0x05: TMission_unknown_6bbb40(mission, is_next); break;
        case 0x0B: TMission_unknown_6bbbb0(mission, is_next); break;
        case 0x0C: TMission_unknown_6bbe60(mission, is_next); break;
        case 0x0A: TMission_unknown_6bbf20(mission, is_next); break;
        case 0x13: TMission_unknown_6bc130(mission, is_next); break;
        case 0x0D: TMission_unknown_6bc190(mission, is_next); break;
        case 0x06: TMission_unknown_6bc1e0(mission, is_next); break;
        case 0x07:
            unknown_player_a35db4->Flag_To_Win(false);
            StepCompleted = true;
            break;
        case 0x17:
            unknown_player_a35db4->Flag_To_Lose(false);
            StepCompleted = true;
            break;
        case 0x19: TMission_unknown_6bc260(mission, is_next); break;
        case 0x1A: TMission_unknown_6bc290(mission, is_next); break;
        case 0x1B:
            Theme.Queue_Song((ThemeType)mission.Data);
            StepCompleted = true;
            break;
        case 0x1C: TMission_unknown_6bc2e0(mission, is_next); break;
        case 0x1D: TMission_unknown_6bc360(mission, is_next); break;
        case 0x1E: TMission_unknown_6bc380(mission, is_next); break;
        case 0x1F: TMission_unknown_6bc3a0(mission, is_next); break;
        case 0x20: TMission_unknown_6bc460(mission, is_next); break;
        case 0x21: TMission_unknown_6bc4a0(mission, is_next); break;
        case 0x22: TMission_unknown_6bc4c0(mission, is_next); break;
        case 0x23:
            Map.Unknown_00559FD0();
            StepCompleted = true;
            break;
        case 0x24:
            Map.Unknown_0055A1F0();
            StepCompleted = true;
            break;
        case 0x27: TMission_unknown_6bbec0(mission, is_next); break;
        case 0x28: TMission_unknown_6bbef0(mission, is_next); break;
        case 0x29: TMission_unknown_6bc040(mission, is_next); break;
        case 0x2A: TMission_unknown_6bc070(mission, is_next); break;
        case 0x2B: TMission_unknown_6bc5b0(mission, is_next); break;
        case 0x2E: TMission_unknown_6bc6d0(mission, is_next); break;
        case 0x2F: TMission_unknown_6bc7b0(mission, is_next); break;
        case 0x3A: TMission_unknown_6bc980(mission, is_next); break;
        case 0x30: TMission_unknown_6bcbb0(mission, is_next); break;
        case 0x08: TMission_unknown_6bd4b0(mission, is_next); break;
        case 0x31: TMission_unknown_6bd7f0(mission, is_next); break;
        case 0x32:
            if (is_next) {
                unknown_func_6bd990(mission.Data);
                StepCompleted = true;
            }
            break;
        case 0x33: TMission_unknown_6bd9b0(mission, is_next); break;
        case 0x34: TMission_unknown_6bda70(mission, is_next); break;
        case 0x35: TMission_unknown_6bdaa0(mission, is_next); break;
        case 0x36: TMission_unknown_6bddb0(mission, is_next); break;
        case 0x37: TMission_unknown_6be010(mission, is_next); break;
        case 0x38: TMission_unknown_6be1c0(mission, is_next); break;
        case 0x39: TMission_unknown_6be440(mission, is_next); break;
        default:
            break;
        }
    } else {
        if (IsMoving) {
            IsReforming = !unknown_func_6b9f40();
        } else {
            unknown_func_6ba1a0();
        }
    }
}


// Marks the team moving and "has been", clears under-strength, flags the members when reforming or forced active, restarts the script and completes the step.

void TeamClass::unknown_func_6b8860()
{
    IsMoving = true;
    IsHasBeen = true;
    IsUnderStrength = false;
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        if (IsReforming || IsForcedActive) {
            member->unknown_579 = true;
        }
    }
    Script->Reset();
    StepCompleted = true;
}


// void TeamClass::unknown_func_6b88b0() (0x006B88B0; TS Regroup): restarts the script and sends the team toward the
// owner's unarmed building that rates best (distance from the team's center times one more than the
// house's value of the building's 4 x 4 cell block, halved for repair buildings) with a cell the leader
// can reach on the way (0x004BB3F0, hypothesis: Safety_Point); the target becomes that cell.

void TeamClass::unknown_func_6b88b0()
{
    IsMoving = false;
    Script->Reset();
    if (unknown_48 > 0) {
        unknown_func_6b9610(&unknown_34, &unknown_38);
        if (unknown_34 == 0) {
            if (unknown_38 == 0) {
                return;
            }
            unknown_34 = unknown_38;
        }
        Cell best_cell = Coord_Cell(unknown_34->Center_Coord());
        int best = 0x7FFFFFFF;
        int count = Owner->Buildings.Count();
        for (int index = 0; index < count; index++) {
            BuildingClass *building = Owner->Buildings[index];
            if (building != 0 && !building->unknown_76 && building->unknown_slot231(0)->WeaponType == 0) {
                Cell cell = Coord_Cell(building->Center_Coord());
                int value = Coord_Distance(building->Center_Coord(), unknown_34->Center_Coord()) * (Map.Cell_Threat(cell, Owner) + 1);
                if (building->Type->unknown_1349) {
                    value /= 2;
                }
                if (value < best) {
                    cell = unknown_func_6baa60()->Safety_Point_004bb3f0(Coord_Cell(unknown_34->Center_Coord()), cell, 2, 4);
                    if (cell != unknown_geometry_k) {
                        best = value;
                        best_cell = cell;
                    }
                }
            }
        }
        unknown_40 = &Map[best_cell];
        unknown_func_6ba1a0();
    } else {
        unknown_34 = 0;
    }
}


// bool TeamClass::unknown_func_6b8bb0() (0x006B8BB0)
// Recomputes the strength flags from the member count against the task
// force's total; a team that never had members is deleted when its last
// member is gone.

extern DynamicVectorClass<TagClass *> unknown_tags_866160;      // 0x00866160

bool TeamClass::unknown_func_6b8bb0()
{
    bool was_under_strength = IsUnderStrength;
    int total = Class->TaskForce->Total_Amount();
    if (unknown_48 > 0) {
        IsFullStrength = unknown_48 == total;
        if (IsFullStrength) {
            IsHasBeen = true;
        }
        if (Class->Reinforce) {
            if (total > 2) {
                IsUnderStrength = unknown_48 <= total / 3;
            } else {
                IsUnderStrength = unknown_48 < total;
            }
        } else {
            IsUnderStrength = !IsHasBeen;
        }
        if (Class->GuardSlower) {
            if (IsUnderStrength) {
                GuardSlowerIsNotUnderStrength = false;
            } else {
                GuardSlowerIsNotUnderStrength = true;
            }
        }
        JustDisappeared = false;
        NeedsToDisappear = false;
    } else {
        GuardSlowerIsNotUnderStrength = false;
        IsUnderStrength = true;
        IsFullStrength = false;
        unknown_34 = 0;
        if (IsHasBeen) {
            if (TargetNotAssigned) {
                for (int index = unknown_tags_866160.Count() - 1; index >= 0; index--) {
                    TagClass *tag = unknown_tags_866160[index];
                    if (tag->Spring(0x17, 0, unknown_geometry_k, 0, 0) && unknown_tags_866160.Count() == 0) {
                        break;
                    }
                }
            }
            delete this;
            return false;
        }
    }
    if (was_under_strength != IsUnderStrength) {
        IsReforming = true;
    }
    return true;
}


// Adds a foot object to the team (when it may join): leaves its old team, becomes the first member in the list, takes the team type's group and tag, and counts it.

bool TeamClass::unknown_func_6b8cc0(FootClass *member)
{
    if (member == 0) {
        return false;
    }
    int slot;
    if (!unknown_func_6b8dc0(member, &slot)) {
        return false;
    }
    if (member->Team != 0) {
        member->Team->Unknown_006B8FB0(member, -1, 0);
    }
    unknown_84[slot]++;
    member->unknown_579 = Member == 0;
    member->unknown_4c8 = Member;
    Member = member;
    member->Team = this;
    member->unknown_1ac = Class->Get_Group();
    if (Tag != 0) {
        if (!Class->OnTransOnly || member->Techno_Type_Class()->Passengers > 0) {
            member->Attach_Tag(Tag);
        }
    }
    unknown_48++;
    unknown_4c += member->unknown_slot168();
    if (unknown_34 == 0) {
        unknown_func_6b9610(&unknown_34, &unknown_38);
    }
    JustDisappeared = true;
    NeedsToDisappear = true;
    member->unknown_37a = Class->AreTeamMembersRecruitable;
    return true;
}


// Whether a foot object may join the team, and the task force slot it would fill (written to *slot): it must be a live object of the team's house that is not in radio contact (aircraft only airport-bound ones), recruitable, outranking its current team, and its type must be in the task force with room left.

bool TeamClass::unknown_func_6b8dc0(FootClass *member, int *slot)
{
    if (this != member->Team && unknown_func_6bdd80(member) && member->House == Owner) {
        if (!member->HasAnyLink() || (member->Kind_Of() == 2 && ((AircraftClass *)member)->Type->AirportBound)) {
            if (member->Get_Mission() == -1 || MissionClass::Is_Recruitable_Mission(member->Get_Mission())) {
                if (member->unknown_379 || Class->Autocreate) {
                    bool aircraft_or_waiting = false;
                    if (Script != 0) {
                        if (Script->Get_Current_Mission().Mission == 8) {
                            aircraft_or_waiting = true;
                        }
                    }
                    if (member->Kind_Of() == 2) {
                        aircraft_or_waiting = true;
                    }
                    if (member->unknown_37a || !Class->Autocreate || aircraft_or_waiting) {
                        if (member->Team == 0 || member->Team->Class->Priority < Class->Priority) {
                            if (member->Kind_Of() != 2 || member->unknown_slot231(0)->WeaponType == 0 || member->unknown_254 != 0) {
                                for (*slot = 0; *slot < Class->TaskForce->CountEntries; (*slot)++) {
                                    if (Class->TaskForce->Entries[*slot].Type == member->Class_Of()) {
                                        break;
                                    }
                                }
                                if (*slot != Class->TaskForce->CountEntries) {
                                    return unknown_84[*slot] < Class->TaskForce->Entries[*slot].Amount ? true : false;
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


// Red Alert's Remove: takes a foot object out of the team: clears its tag when it is the team's, counts down the task force slot it filled (found by type when slot is -1), unlinks it from the member list, idles it unless arg3, and when no remaining member is initiated (+0x579) flags the first one.

bool TeamClass::Unknown_006B8FB0(FootClass *member, int slot, bool arg3)
{
    member->unknown_5a2 = true;
    if (this != member->Team) {
        return true;
    }
    if (member->unknown_34 == Tag && member->House != 0 && !member->House->Is_Human_Player()) {
        member->Attach_Tag(0);
    }
    if (slot == -1 && Class != 0 && Class->TaskForce != 0) {
        for (slot = 0; slot < Class->TaskForce->CountEntries; slot++) {
            if (Class->TaskForce->Entries[slot].Type == member->Class_Of()) {
                break;
            }
        }
    }
    if (Class != 0 && Class->TaskForce != 0 && (unsigned)slot < (unsigned)Class->TaskForce->CountEntries) {
        unknown_84[slot]--;
    }
    bool first = false;
    FootClass *previous = 0;
    FootClass *current = Member;
    bool found = false;
    while (current != 0 && (!found || !first)) {
        if (current == member) {
            if (previous != 0) {
                previous->unknown_4c8 = current->unknown_4c8;
            } else {
                Member = (FootClass *)current->unknown_4c8;
            }
            FootClass *next = (FootClass *)current->unknown_4c8;
            current->unknown_4c8 = 0;
            current->Team = 0;
            current->SuspendedMission = -1;
            current->unknown_498 = 0;
            current->unknown_22c = 0;
            current = next;
            unknown_48--;
            found = true;
            unknown_4c -= member->unknown_slot168();
            continue;
        }
        first = first || current->unknown_579;
        previous = current;
        current = (FootClass *)current->unknown_4c8;
    }
    if (member->Team != 0) {
        member->Team = 0;
    }
    if (unknown_a40774 == 0 && member->unknown_7d && !member->unknown_37d && !member->unknown_76 && !arg3) {
        member->unknown_slot261(false, true);
    }
    if (!first && Member != 0) {
        Member->unknown_579 = true;
        unknown_34 = 0;
    }
    JustDisappeared = true;
    NeedsToDisappear = true;
    return true;
}


// int TeamClass::unknown_func_6b91c0(int slot) (0x006B91C0; Red Alert's Recruit)
// Adds the nearest object (squared distance from the team's zone or the type's waypoint, 0x3200 more
// outside the type's group unless the group is -2 or the type recruits) of task force slot's type that
// may join, when the slot is still short: infantry, aircraft, or one of the owner's units with its
// passengers. Returns the number added.


int TeamClass::unknown_func_6b91c0(int slot)
{
    CoordStruct center = unknown_34 != 0 ? unknown_34->Center_Coord() : unknown_geometry_l;
    if (Class->Waypoint_Location() != unknown_geometry_k) {
        center = Cell_Coord(Class->Waypoint_Location());
    }
    int added = 0;
    if (Class->TaskForce->Entries[slot].Amount > unknown_84[slot]) {
        int group = Class->Get_Group();
        switch (Class->TaskForce->Entries[slot].Type->Kind_Of()) {
        case 0xF:
        case 0x10: {
            FootClass *best = 0;
            int best_distance = -1;
            for (int index = 0; index < Infantry.Count(); index++) {
                FootClass *infantry = (FootClass *)Infantry[index];
                if (group == -2 || infantry->unknown_1ac == group || Class->Recruiter) {
                    int distance = infantry->Distance_Squared(center);
                    if (infantry->unknown_1ac != group) {
                        distance += 0x3200;
                    }
                    if ((distance < best_distance || best_distance == -1) && unknown_func_6b8dc0(infantry, &slot)) {
                        best = infantry;
                        best_distance = distance;
                    }
                }
            }
            if (best != 0) {
                best->unknown_slot220(0);
                unknown_func_6b8cc0(best);
                added++;
            }
            break;
        }
        case 2:
        case 3: {
            AircraftClass *best = 0;
            int best_distance = -1;
            for (int index = 0; index < Aircraft.Count(); index++) {
                AircraftClass *aircraft = Aircraft[index];
                if (group == -2 || aircraft->unknown_1ac == group || Class->Recruiter) {
                    int distance = aircraft->Distance_Squared(center);
                    if (aircraft->unknown_1ac != group) {
                        distance += 0x3200;
                    }
                    if ((distance < best_distance || best_distance == -1) && unknown_func_6b8dc0(aircraft, &slot)) {
                        best = aircraft;
                        best_distance = distance;
                    }
                }
            }
            if (best != 0) {
                best->unknown_slot220(0);
                unknown_func_6b8cc0(best);
                added++;
            }
            break;
        }
        case 1:
        case 0x28: {
            UnitClass *best = 0;
            int best_distance = -1;
            for (int index = 0; index < Units.Count(); index++) {
                UnitClass *unit = Units[index];
                if (group == -2 || unit->unknown_1ac == group || Class->Recruiter) {
                    int distance = unit->Distance_Squared(center);
                    if (unit->unknown_1ac != group) {
                        distance += 0x3200;
                    }
                    if (unit->House == Owner && unit->Type == Class->TaskForce->Entries[slot].Type) {
                        if ((distance < best_distance || best_distance == -1) && unknown_func_6b8dc0(unit, &slot)) {
                            best = unit;
                            best_distance = distance;
                        }
                    }
                }
            }
            if (best != 0) {
                best->unknown_slot220(0);
                unknown_func_6b8cc0(best);
                added++;
                FootClass *passenger = (FootClass *)best->Cargo.Attached_Object();
                while (passenger != 0) {
                    unknown_func_6b8cc0(passenger);
                    ObjectClass *next = passenger->unknown_30;
                    passenger = next != 0 && next->AbstractFlags.IsFoot ? (FootClass *)next : 0;
                }
            }
            break;
        }
        }
    }
    return added;
}


// void TeamClass::Detach(AbstractClass *target, bool all) (0x006B9590, slot 10): forgets an
// object that leaves the game: the tag, the first member (only when `all`; the member's
// next team member at FootClass +0x4C8 takes its place), the owner, the script, the
// type and the six swizzled references.

void TeamClass::Detach(AbstractClass *target, bool all)
{
    if (target == Tag) {
        Tag = 0;
    }
    if (target == Member && all) {
        Member = (FootClass *)Member->unknown_4c8;
    }
    if (target == Owner) {
        Owner = 0;
    }
    if (target == Script) {
        Script = 0;
    }
    if (unknown_40 == target) {
        unknown_40 = 0;
    }
    if (unknown_3c == target) {
        unknown_3c = 0;
    }
    if (unknown_34 == target) {
        unknown_34 = 0;
    }
    if (Class == target) {
        Class = 0;
    }
    if (unknown_38 == target) {
        unknown_38 = 0;
    }
    if (unknown_44 == target) {
        unknown_44 = 0;
    }
    if (unknown_30 == target) {
        unknown_30 = 0;
    }
}


// void TeamClass::unknown_func_6b9610(AbstractClass **center, AbstractClass **close_member) (0x006B9610;
// Red Alert's Calc_Center): with a hound-dog script step the nearest allied unit or infantry outside the
// team (its tube's exit cell while it is in a tube), otherwise the average position of the locked
// members that play, and the member nearest the team's target (+0x40).

// The offset from one coordinate to another (hypothesis: Westwood's Coord operator-, whose
// constructor 0x0041B630 stays out of line).
static inline UnknownVector3 Coord_Offset(CoordStruct const &to, CoordStruct const &from)
{
    return UnknownVector3(to.X - from.X, to.Y - from.Y, to.Z - from.Z);
}

// The distance between two coordinates (hypothesis: Westwood's Distance_To).
static inline int Coord_Offset_Distance(CoordStruct const &to, CoordStruct const &from)
{
    UnknownVector3 offset = Coord_Offset(to, from);
    double y = offset.Y;
    double z = offset.Z;
    double x = offset.X;
    return (int)unknown_sqrt_4ba410(x * x + z * z + y * y);
}

void TeamClass::unknown_func_6b9610(AbstractClass **center, AbstractClass **close_member)
{
    *close_member = 0;
    *center = 0;
    FootClass *member = Member;
    if (member == 0) {
        return;
    }
    if (Script->Get_Current_Mission().Mission == 10) {
        FootClass *closest = 0;
        int distance = -1;
        for (int unit_index = 0; unit_index < Units.Count(); unit_index++) {
            FootClass *unit = (FootClass *)Units[unit_index];
            if (unknown_func_6bdd80(unit) && unit->House->Is_Ally(Owner) && this != (TeamClass *)unit->Team) {
                UnknownVector3 offset = Coord_Offset(member->Center_Coord(), unit->Center_Coord());
                int trial = offset.Length();
                if (distance == -1 || trial < distance) {
                    distance = trial;
                    closest = unit;
                }
            }
        }
        for (int infantry_index = 0; infantry_index < Infantry.Count(); infantry_index++) {
            FootClass *infantry = (FootClass *)Infantry[infantry_index];
            if (unknown_func_6bdd80(infantry) && infantry->House->Is_Ally(Owner) && this != (TeamClass *)infantry->Team) {
                int trial = Coord_Offset_Distance(member->Center_Coord(), infantry->Center_Coord());
                if (distance == -1 || trial < distance) {
                    distance = trial;
                    closest = infantry;
                }
            }
        }
        if (closest != 0) {
            if (closest->unknown_574 >= 0) {
                Cell exit = unknown_TubeClass_list_008661c8[closest->unknown_574]->ExitCell;
                *center = &Map[exit];
            } else {
                *center = closest;
            }
            *close_member = Member;
        }
    } else {
        int x = 0;
        int y = 0;
        int dist = 0;
        int quantity = 0;
        FootClass *closest = 0;
        while (member != 0) {
            if (unknown_func_6bdd80(member) && (member->unknown_579 || member->Kind_Of() == 2) && member->unknown_32d) {
                x += member->Position_Coord_004f3f40().X;
                y += member->Position_Coord_004f3f40().Y;
                quantity++;
                if (Class->GuardSlower && member->unknown_slot283()) {
                    x += member->Position_Coord_004f3f40().X;
                    y += member->Position_Coord_004f3f40().Y;
                    quantity++;
                }
                if (member->Techno_Type_Class()->Passengers <= 0 || !member->Techno_Type_Class()->Naval) {
                    int try_dist = member->Distance_Squared((ObjectClass *)unknown_40);
                    if (!dist || try_dist < dist) {
                        dist = try_dist;
                        closest = member;
                    }
                }
            }
            member = (FootClass *)member->unknown_4c8;
        }
        if (quantity) {
            x /= quantity;
            y /= quantity;
            CoordStruct coord = CoordStruct(x, y, 0);
            *center = &Map[coord];
            if (closest == 0) {
                closest = Member;
            }
            if (closest->Can_Enter_Cell((CellClass *)*center, -1, -1, 0, 1) != 0) {
                *center = closest;
            }
        }
        if (closest != 0) {
            *close_member = closest;
        }
    }
}


// void TeamClass::Unknown_006B9AA0(FootClass *member, int result, TechnoClass *source) (0x006B9AA0)
// A member was damaged by a source: a team that is not moving regroups; otherwise
// a team whose first member is not of the excluded kind may turn on the source.

void TeamClass::Unknown_006B9AA0(FootClass *member, int result, TechnoClass *source)
{
    if (source != 0 && result != 0 && !Class->Suicide) {
        if (!IsMoving) {
            unknown_34 = 0;
            NeedsReGrouping = true;
            IsReforming = true;
            return;
        }
        FootClass *first = Member;
        for (FootClass *current = Member; current != 0; current = (FootClass *)current->unknown_4c8) {
            if (current == (FootClass *)source) {
                return;
            }
        }
        if (first != 0 && first->Kind_Of() != 2 && Member->unknown_slot231(0)->WeaponType != 0 && unknown_40 != (AbstractClass *)source) {
            if (Class->Annoyance) {
                unknown_34 = 0;
                NeedsReGrouping = true;
                IsReforming = true;
            }
            if (unknown_40 != 0) {
                TechnoClass *techno = unknown_40->As_Techno();
                if (techno != 0 && techno->unknown_slot231(0)->WeaponType != 0) {
                    if (unknown_34 == 0) {
                        return;
                    }
                    if (techno->unknown_slot214(unknown_34->Center_Coord())) {
                        return;
                    }
                }
            }
            source->Kind_Of();
        }
    }
}


// void TeamClass::unknown_func_6b9bb0() (0x006B9BB0; Red Alert's Coordinate_Attack)
// Takes the mission target when the team has none; a cell target (unless the leader is an aircraft)
// becomes its occupant. Without a target completes the step; otherwise conscripts every member, sends
// infiltrating infantry on a mission-15 line to capture it (mission 8), puts the others to attack
// (mission 1) unless already attacking, entering, capturing, mission 17 or a busy mission 16, gives
// idle members the target, and completes the step when no member still can attack.

void TeamClass::unknown_func_6b9bb0()
{
    if (unknown_40 == 0) {
        unknown_40 = unknown_3c;
    }
    if (unknown_40 != 0 && unknown_40->Kind_Of() == 0xB && Member != 0 && unknown_func_6baa60()->Kind_Of() != 2) {
        CellClass *cell = As_Cell(unknown_40);
        if (cell->Cell_Object(Point2D(0, 0), false) != 0) {
            unknown_40 = cell->Cell_Object(Point2D(0, 0), false);
        }
    }
    if (unknown_40 == 0) {
        StepCompleted = true;
        return;
    }
    ScriptMissionClass mission = Script->Get_Current_Mission();
    bool attacking = false;
    for (FootClass *unit = Member; unit != 0; unit = (FootClass *)unit->unknown_4c8) {
        unknown_func_6ba7d0(unit);
        if (unknown_func_6bdd80(unit) && (unit->unknown_579 || unit->Kind_Of() == 2)) {
            if (mission.Mission == 0xF && unit->Kind_Of() == 0xF && ((InfantryClass *)unit)->Type->Infiltrate) {
                unit->QueueMission(8, false);
                unit->unknown_slot220(unknown_40);
            } else if (unit->Get_Mission() != 1 && unit->Get_Mission() != 7 && unit->Get_Mission() != 8
                && unit->Get_Mission() != 0x11 && (unit->Get_Mission() != 0x10 || !unit->unknown_slot284())) {
                unit->unknown_slot149(3);
                unit->QueueMission(1, false);
                unit->unknown_slot220(0);
                unit->Assign_Destination(0, true);
            }
            if (unit->unknown_228 != unknown_40 && unit->unknown_228 == 0) {
                unit->unknown_slot220(unknown_40);
            }
            if (unit->Kind_Of() != 2 || unit->unknown_slot231(0)->WeaponType == 0 || unit->unknown_254 > 0) {
                attacking = true;
            }
        }
    }
    if (!attacking) {
        StepCompleted = true;
    }
}


// bool TeamClass::unknown_func_6b9f40() (0x006B9F40; Red Alert's Coordinate_Regroup)
// Conscripts every member, then sends the live, initiated (or aircraft) members farther than the stray
// distance from the team's zone (+0x34) toward it (unless guarding with a target, mission 0x0B) and puts
// the near ones back to guard; returns whether none had to move, and then clears NeedsReGrouping.

bool TeamClass::unknown_func_6b9f40()
{
    FootClass *unit = Member;
    bool retval = true;
    while (unit != 0) {
        unknown_func_6ba7d0(unit);
        if (unknown_func_6bdd80(unit) && (unit->unknown_579 || unit->Kind_Of() == 2)) {
            if (unit->unknown_5d4c00(unknown_34) > Unknown_006BE6A0() && (unit->Get_Mission() != 0xB || unit->unknown_228 == 0)) {
                if (unit->unknown_494 == 0) {
                    retval = false;
                    unit->QueueMission(2, false);
                    Cell dest = unit->unknown_4c8af0(Coord_Cell(unknown_34->Center_Coord()));
                    unit->Assign_Destination(&Map[dest], true);
                }
            } else if (unit->Get_Mission() != 0xB) {
                unit->QueueMission(5, false);
                unit->Assign_Destination(0, true);
            }
        }
        unit = (FootClass *)unit->unknown_4c8;
    }
    if (retval == true) {
        NeedsReGrouping = false;
    }
    return retval;
}


// void TeamClass::unknown_func_6ba1a0() (0x006BA1A0; Red Alert's Coordinate_Move)
// Moves the team toward its target (+0x40, else the mission target +0x3C) once no member lags:
// conscripts each member, sends the playing ones farther than the stray distance (doubled for slot 21)
// or still to climb/descend toward it, idles the arrived ones, and completes the step (StepCompleted) when
// every playing member arrived while the team is moving.

void TeamClass::unknown_func_6ba1a0()
{
    FootClass *unit = Member;
    bool finished = true;
    bool found = false;
    if (unit == 0) {
        return;
    }
    if (unknown_40 == 0) {
        unknown_40 = unknown_3c;
    }
    if (unknown_40 != 0) {
        if (!unknown_func_6ba600()) {
            for (; unit != 0; unit = (FootClass *)unit->unknown_4c8) {
                if (unknown_func_6ba7d0(unit)) {
                    finished = false;
                }
                if (unit->Get_Mission() == 0x10 || unit->MissionQueue == 0x10) {
                    finished = false;
                }
                if (unknown_func_6bdd80(unit) && (unit->unknown_579 || unit->Kind_Of() == 2)
                    && unit->Get_Mission() != 0x10 && unit->MissionQueue != 0x10) {
                    int stray = Unknown_006BE6A0();
                    if (unit->In_Air()) {
                        stray *= 2;
                    }
                    found = true;
                    int distance = unit->unknown_5d4c00(unknown_40);
                    if (distance > stray
                        || (unit->Get_Height_AGL() < 0 && Script->Get_Next_Mission().Mission != 3)
                        || (unit->Kind_Of() == 2 && unit->Get_Height() > 0
                            && &Map[unit->Center_Coord()] != unknown_40
                            && Script->Get_Next_Mission().Mission != 3)) {
                        if (Class->Aggressive && unit->unknown_228 != 0) {
                            continue;
                        }
                        if (unit->Get_Mission() != 2) {
                            unit->QueueMission(2, false);
                            if (unit->unknown_slot122()) {
                                unit->Commence();
                            }
                        }
                        if (unit->unknown_494 == 0) {
                            unit->Assign_Destination(unknown_40, true);
                        }
                        AbstractClass *navcom = unit->unknown_494;
                        if (navcom != unknown_40) {
                            if (unit->Techno_Type_Class()->BalloonHover
                                || (navcom != unknown_40 && unit->Kind_Of() == 2 && navcom == unit->Get_Cell_Ptr())) {
                                unit->Assign_Destination(unknown_40, true);
                            }
                        }
                        finished = false;
                    } else {
                        if (unit->Get_Mission() == 2
                            && (unit->unknown_494 == 0
                                || (unit->unknown_5d4c00(unit->unknown_494) <= Rules->unknown_1380 && !unit->Locomotor.Checked()->Is_Moving()))
                            && unit->unknown_228 == 0) {
                            unit->Assign_Destination(0, true);
                            unit->unknown_slot261(false, true);
                        }
                    }
                    bool arrived = unit->Kind_Of() == 2 && unit->unknown_494 == unit->Get_Cell_Ptr();
                    bool hovering = unit->AbstractFlags.IsTechno && unit->Techno_Type_Class()->BalloonHover && distance < stray;
                    if (unit->unknown_494 != 0 && !arrived && !hovering) {
                        finished = false;
                    }
                }
            }
        } else {
            finished = false;
        }
    }
    if (!found) {
        finished = false;
    }
    if (finished && IsMoving) {
        StepCompleted = true;
    }
}


// Red Alert's Lagging_Units: while the team is lagging, every live, placed member farther from the team's object (+0x38) than the allowed distance (tripled for aircraft, a third for non-guarding members of guard-slower teams) is sent toward it, the others are put back to guard (mission 5); stores and returns whether any lags.

bool TeamClass::unknown_func_6ba600()
{
    FootClass *member = Member;
    bool lagging = false;
    if (!IsLagging) {
        return false;
    }
    for (; member != 0; member = (FootClass *)member->unknown_4c8) {
        if (unknown_func_6bdd80(member) && (member->unknown_579 || member->Kind_Of() == 2)) {
            int allowed = Unknown_006BE6A0();
            if (member->Kind_Of() == 2) {
                allowed *= 3;
            }
            if (Class->GuardSlower && !member->unknown_slot283()) {
                allowed /= 3;
            }
            if (Coord_Length(member->Center_Coord() - unknown_38->Center_Coord()) > allowed) {
                if (member->unknown_494 == 0) {
                    member->QueueMission(2, false);
                    member->Assign_Destination(unknown_38, true);
                }
                lagging = true;
            } else if (member->Get_Mission() != 5) {
                member->QueueMission(5, false);
                member->Assign_Destination(0, true);
            }
        }
    }
    IsLagging = lagging;
    return lagging;
}


// For a live member that is not yet in place: when it is farther from the team's object (+0x34) than the allowed distance, sends it toward it (guard mission 2, no target, destination +0x34) and returns true; otherwise flags it in place.

bool TeamClass::unknown_func_6ba7d0(FootClass *member)
{
    if (unknown_func_6bdd80(member) && !member->unknown_579) {
        if (member->unknown_5d4c00(unknown_34) > Unknown_006BE6A0()) {
            if (member->unknown_494 == 0) {
                member->QueueMission(2, false);
                member->unknown_slot220(0);
                member->Assign_Destination(unknown_34, true);
            }
            return true;
        }
        member->unknown_579 = true;
    }
    return false;
}


// Whether a foot object is a member of the team.

bool TeamClass::unknown_func_6ba8b0(FootClass *object)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        if (member == object) {
            return true;
        }
    }
    return false;
}


// Disbands every team of the house whose team type has a lower priority than the given one: removes its members, flags it to disappear and starts its leaving timer.

void __fastcall TeamClass::unknown_func_6ba8e0(int priority, HouseClass *house)
{
    for (int index = 0; index < Teams.Count(); index++) {
        TeamClass *team = Teams[index];
        if (team != 0 && house == team->Owner && team->Class->Priority < priority) {
            while (team->Member != 0) {
                team->Unknown_006B8FB0(team->Member, -1, 0);
            }
            team->JustDisappeared = true;
            team->NeedsToDisappear = true;
            team->unknown_64 = UnknownTimer423F60((long)(Rules->unknown_1150 * 900.0));
            team->IsLeavingMap = true;
        }
    }
}


// Whether the team is moving and its current script line is action 3 toward a waypoint that is not on the map.

bool TeamClass::Unknown_006BA990()
{
    if (IsMoving && Script->Has_Mission()) {
        ScriptMissionClass mission = Script->Get_Current_Mission();
        if (mission.Mission == 3 && !Map.Unknown_0055A660(unknown_id_source_a3d290->Get_Waypoint_Location(mission.Data), true)) {
            return true;
        }
    }
    return false;
}


// Whether every member has its +0x32D flag set.

bool TeamClass::unknown_func_6baa00()
{
    bool all = true;
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        if (!member->unknown_32d) {
            all = false;
            break;
        }
    }
    return all;
}


// Clears the team's target and clears every member's target, flagging it.

void TeamClass::unknown_func_6baa30()
{
    unknown_func_6b79e0(0);
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        member->unknown_slot220(0);
        member->unknown_578 = true;
    }
}


// The member that should lead the team: the live one (on the map or an aircraft) with the highest LeadershipRating; the first member when none qualifies.

// Red Alert's _Is_It_Playing: a member that is alive (0x006BDD80, Red Alert's _Is_It_Breathing) and
// in place or an aircraft. File-local inline of the Team unit; callers after this point may use it.
static inline bool _Is_It_Playing(FootClass *object)
{
    return TeamClass::unknown_func_6bdd80(object) && (object->unknown_579 || object->Kind_Of() == 2);
}

FootClass *TeamClass::unknown_func_6baa60()
{
    FootClass *member = Member;
    FootClass *leader = Member;
    int best = -1;
    for (; member != 0; member = (FootClass *)member->unknown_4c8) {
        int rating = member->Techno_Type_Class()->LeadershipRating;
        if (_Is_It_Playing(member) && rating > best) {
            leader = member;
            best = rating;
        }
    }
    return leader;
}


// HRESULT __stdcall TeamClass::Load(IStream *stream) (0x006BAAE0)
// Restores the team in place and swizzles its pointers.

HRESULT __stdcall TeamClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TeamClass(NoInitClass());
        Swizzler.Swizzle((void **)&Class);
        Swizzler.Swizzle((void **)&Script);
        Swizzler.Swizzle((void **)&Owner);
        Swizzler.Swizzle((void **)&Tag);
        Swizzler.Swizzle((void **)&Member);
        Swizzler.Swizzle((void **)&unknown_38);
        Swizzler.Swizzle((void **)&unknown_40);
        Swizzler.Swizzle((void **)&unknown_34);
        Swizzler.Swizzle((void **)&unknown_3c);
        Swizzler.Swizzle((void **)&unknown_30);
        Swizzler.Swizzle((void **)&unknown_44);
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall TeamClass::Save(IStream *stream, BOOL clear_dirty) (0x006BABD0)

HRESULT __stdcall TeamClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall TeamClass::GetClassID(GUID *clsid) (0x006BABF0)

HRESULT __stdcall TeamClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TeamClass;
    return S_OK;
}


// void TeamClass::Compute_CRC(WWCRCEngine &crc) const (0x006BAC30)

void TeamClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(Class->Fetch_ID());
    crc(((AbstractClass *)Owner)->Fetch_ID());
    if (unknown_38 != 0) {
        crc(unknown_38->Fetch_ID());
    }
    crc((long)unknown_48);
    crc((long)unknown_4c);
    if (Member != 0) {
        crc(((AbstractClass *)Member)->Fetch_ID());
    }
    crc(unknown_58.Value());
    crc(Timer_Value(unknown_64));
    if (Tag != 0) {
        crc(((AbstractClass *)Tag)->Fetch_ID());
    }
    crc(IsForcedActive);
    crc(IsHasBeen);
    crc(IsFullStrength);
    crc(IsUnderStrength);
    crc(IsReforming);
    crc(IsLagging);
    crc(NeedsToDisappear);
    crc(JustDisappeared);
    crc(IsMoving);
    crc(StepCompleted);
    crc(TargetNotAssigned);
    crc(IsLeavingMap);
    crc(unknown_84, sizeof(unknown_84));
}


// void TeamClass::TMission_unknown_6badc0(ScriptMissionClass const &mission, bool unknown) (0x006BADC0)
// When flagged, targets the cell under the team's object (+0x34, else the first member), then runs 0x006BA1A0.

void TeamClass::TMission_unknown_6badc0(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        if (unknown_34 != 0) {
            unknown_func_6b79e0(Map.Unknown_005629D0(unknown_34));
        } else {
            unknown_func_6b79e0(Map.Unknown_005629D0(Member));
        }
    }
    unknown_func_6ba1a0();
}


// void TeamClass::TMission_unknown_6bae00(ScriptMissionClass const &mission, bool unknown) (0x006BAE00)
// When flagged, targets the cell whose coordinates are Data % 128 and Data / 128, then runs 0x006BA1A0.

void TeamClass::TMission_unknown_6bae00(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        unknown_func_6b79e0(&Map[Cell((short)(mission.Data % 128), (short)(mission.Data / 128))]);
    }
    unknown_func_6ba1a0();
}


// void TeamClass::TMission_unknown_6bae60(ScriptMissionClass const &mission, bool unknown) (0x006BAE60)
// When flagged, targets the cell of waypoint Data; unless the team is moving toward a following
// mission-3 line whose waypoint passes 0x0055A660, a cell the leader cannot enter is replaced by a
// reachable cell near it for the leader's speed type (none when there is none); then runs 0x006BA1A0.

void TeamClass::TMission_unknown_6bae60(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        FootClass *leader = unknown_func_6baa60();
        Cell cell = unknown_id_source_a3d290->Get_Waypoint_Location(mission.Data);
        unknown_func_6b79e0(&Map[cell]);
        bool adjust = true;
        if (IsMoving && Script->Has_Mission()) {
            ScriptMissionClass next = Script->Get_Current_Mission();
            if (next.Mission == 3 && !Map.Unknown_0055A660(unknown_id_source_a3d290->Get_Waypoint_Location(next.Data), true)) {
                adjust = false;
            }
        }
        if (adjust) {
            if (leader->Can_Enter_Cell(&Map[cell], -1, -1, 0, 0) != 0) {
                cell = Map.Nearby_Location(cell, leader->Techno_Type_Class()->SpeedType, -1, 0, false,
                    Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
            }
        }
        if (cell != unknown_geometry_k) {
            unknown_func_6b79e0(&Map[cell]);
        } else {
            unknown_func_6b79e0(0);
        }
    }
    unknown_func_6ba1a0();
}


// void TeamClass::TMission_unknown_6bb020(ScriptMissionClass const &mission, bool unknown) (0x006BB020)
// When flagged targets the cell of waypoint Data (or the occupant of that cell); with a target, runs 0x006B9BB0 if any member has ammo to spare, otherwise clears the target and completes the step.

void TeamClass::TMission_unknown_6bb020(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        CellClass *cell = unknown_id_source_a3d290->unknown_6631a0(mission.Data);
        if (cell != 0 && cell->Kind_Of() == 0xB) {
            ObjectClass *occupant = cell->Cell_Object(Point2D(0, 0), cell->unknown_140 >> 8 & 1);
            if (occupant != 0) {
                cell = (CellClass *)occupant;
            }
        }
        unknown_func_6b79e0(cell);
    }
    if (unknown_3c != 0) {
        for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
            if (member->Techno_Type_Class()->Ammo <= 0 || member->unknown_254 > 0) {
                unknown_func_6b9bb0();
                return;
            }
        }
    }
    unknown_func_6b79e0(0);
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bb0f0(ScriptMissionClass const &mission, bool unknown) (0x006BB0F0)
// Targets the cell of waypoint Data (when flagged, or while no second target is set and the script line is below 0x65); every Rules minutes lets the leader pick a better target for the team; then runs 0x006B9BB0 when the target has its +0x14 bit 1 set, else 0x006BA1A0.

void TeamClass::TMission_unknown_6bb0f0(ScriptMissionClass const &mission, bool unknown)
{
    CellClass *cell = unknown_id_source_a3d290->unknown_6631a0(mission.Data);
    if (unknown) {
        unknown_func_6b79e0(cell);
    }
    if (unknown_40 == 0) {
        ScriptMissionClass current = Script->Get_Current_Mission();
        if ((unsigned)current.Data < 0x65) {
            unknown_func_6b79e0(cell);
        }
    }
    if (Frame % (int)(Rules->unknown_e28 * 900.0) == 0) {
        FootClass *leader = unknown_func_6baa60();
        if (leader != 0) {
            AbstractClass *found = leader->unknown_slot219(1, &CoordStruct(leader->Coord), 0);
            if (found != 0) {
                unknown_func_6b79e0(found);
            } else if (unknown_40 != cell) {
                unknown_func_6b79e0(0);
            }
        }
    }
    if (unknown_40 != 0 && unknown_40->AbstractFlags.IsObject) {
        unknown_func_6b9bb0();
    } else {
        unknown_func_6ba1a0();
    }
}


// void TeamClass::TMission_unknown_6bb260(ScriptMissionClass const &mission, bool unknown) (0x006BB260)
// When flagged targets the cell of waypoint Data; a cell target is replaced by the occupant at its centre (if any), then runs 0x006B9BB0 (or clears the target and completes the step when there is none).

void TeamClass::TMission_unknown_6bb260(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        unknown_func_6b79e0(unknown_id_source_a3d290->unknown_6631a0(mission.Data));
    }
    if (unknown_3c != 0 && unknown_3c->Kind_Of() == 0xB) {
        CellClass *cell = As_Cell(unknown_3c);
        ObjectClass *occupant = cell->Cell_Object(Point2D(0, 0), false);
        if (occupant != 0) {
            unknown_func_6b79e0(occupant);
            unknown_func_6b9bb0();
        }
    } else {
        if (unknown_3c == 0) {
            unknown_func_6b79e0(0);
            StepCompleted = true;
        } else {
            unknown_func_6b9bb0();
        }
    }
}


// void TeamClass::TMission_unknown_6bb310(ScriptMissionClass const &mission, bool unknown) (0x006BB310)
// Calls ObjectClass slot 87 (forced) with the zero coordinate on every member, then completes the step.

void TeamClass::TMission_unknown_6bb310(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        member->Scatter(unknown_geometry_l, true, false);
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bb350(ScriptMissionClass const &mission, bool unknown) (0x006BB350)
// Calls FootClass slot 222 on every member with the house of type Data (and 1), then completes the step.

void TeamClass::TMission_unknown_6bb350(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *member = Member;
    while (member != 0) {
        FootClass *next = (FootClass *)member->unknown_4c8;
        member->unknown_slot222(HouseClass::As_Pointer(mission.Data), 1);
        member = next;
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bb3b0(ScriptMissionClass const &mission, bool unknown) (0x006BB3B0)
// Moves every member into a new team of team type Data (same owner) and deletes this team.

void TeamClass::TMission_unknown_6bb3b0(ScriptMissionClass const &mission, bool unknown)
{
    TeamClass *team = new TeamClass(TeamTypes[mission.Data], Owner, 0);
    FootClass *member = Member;
    while (member != 0) {
        Unknown_006B8FB0(member, -1, 0);
        team->unknown_func_6b8cc0(member);
        member = Member;
    }
    delete this;
}


// void TeamClass::TMission_unknown_6bb420(ScriptMissionClass const &mission, bool unknown) (0x006BB420)
// Replaces the team's script with a new one of script type Data and starts it.

void TeamClass::TMission_unknown_6bb420(ScriptMissionClass const &mission, bool unknown)
{
    if (Script != 0) {
        delete Script;
    }
    Script = new ScriptClass(ScriptTypes[mission.Data]);
    Script->Reset();
}


// void TeamClass::TMission_unknown_6bb480(ScriptMissionClass const &mission, bool unknown) (0x006BB480)
// Sends the team's leader to a target found from Data, then completes the
// step once every member has ammo.

void TeamClass::TMission_unknown_6bb480(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown_3c == 0 && Member != 0) {
        FootClass *leader = unknown_func_6baa60();
        int target_data = Unknown_006220E0(mission.Data);
        AbstractClass *target = leader->unknown_slot219(target_data, &CoordStruct(leader->Coord), Class->OnlyTargetHouseEnemy);
        unknown_func_6b79e0(target);
        bool armed = true;
        if (unknown_3c != 0) {
            for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
                if (member->Techno_Type_Class()->Ammo <= 0 || member->unknown_254 > 0) {
                    armed = false;
                    break;
                }
            }
        }
        if (armed) {
            StepCompleted = true;
        }
    }
    bool armed = true;
    if (unknown_3c != 0) {
        for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
            if (member->Techno_Type_Class()->Ammo <= 0 || member->unknown_254 > 0) {
                armed = false;
                break;
            }
        }
    }
    if (armed) {
        StepCompleted = true;
    }
    unknown_func_6b9bb0();
}


// void TeamClass::TMission_unknown_6bb5f0(ScriptMissionClass const &mission, bool unknown) (0x006BB5F0; Red Alert's TMission_Load)

void TeamClass::TMission_unknown_6bb5f0(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *unit = Member;
    FootClass *trans = 0;
    bool aircraft = unknown_func_6bd810();
    while (unit != 0) {
        if (unit->Techno_Type_Class()->Passengers > 0 && unit->Cargo.How_Many() < unit->Techno_Type_Class()->Passengers) {
            if ((!aircraft && unit->Kind_Of() == 1) || (aircraft && unit->Kind_Of() == 2)) {
                trans = unit;
                break;
            }
        }
        unit = (FootClass *)unit->unknown_4c8;
    }
    if (trans == 0) {
        StepCompleted = true;
        return;
    }
    if (trans->HasAnyLink()) {
        return;
    }
    bool finished = true;
    unit = Member;
    while (unit != 0 && unknown_48 > 1) {
        unknown_func_6ba7d0(unit);
        if (unknown_func_6bdd80(unit) && (unit->unknown_579 || unit->Kind_Of() == 2) && unit != trans) {
            finished = false;
            if (unit->Get_Mission() != 7) {
                unit->QueueMission(7, false);
                unit->unknown_slot220(0);
                unit->Assign_Destination(trans, true);
                break;
            }
        }
        unit = (FootClass *)unit->unknown_4c8;
    }
    if (finished) {
        if (Class->TransportsReturnOnUnload) {
            CoordStruct coord = trans->Coord;
            trans->unknown_6d6e50(&Map[coord]);
        }
        StepCompleted = true;
    }
}


// void TeamClass::TMission_unknown_6bb8b0(ScriptMissionClass const &mission, bool unknown) (0x006BB8B0; Red Alert's TMission_Deploy)
// Conscripts every member; a live unit whose type deploys into a building stops the step: unless it is
// already unloading (mission 0x10) it is told to deploy where the building fits (else 0x00458B10 for its
// cell and house); simple deployers and deploying infantry get mission 0x10. Completes when no unit deploys.

void TeamClass::TMission_unknown_6bb8b0(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *unit = Member;
    bool finished = true;
    while (unit != 0) {
        unknown_func_6ba7d0(unit);
        if (unknown_func_6bdd80(unit) && (unit->unknown_579 || unit->Kind_Of() == 2)) {
            bool mcv = unit->Kind_Of() == 1 && ((UnitClass *)unit)->Type->DeploysInto != 0;
            bool deployer = unit->Kind_Of() == 1 && ((UnitClass *)unit)->Type->IsSimpleDeployer;
            bool infantry = unit->Kind_Of() == 0xF && ((InfantryClass *)unit)->unknown_5090a0();
            if (mcv) {
                finished = false;
                if (unit->Get_Mission() != 0x10) {
                    unit->Mark(0);
                    if (!((UnitClass *)unit)->Type->DeploysInto->Legal_Placement(unit->unknown_slot104(), 0)) {
                        ((UnitClass *)unit)->Type->DeploysInto->unknown_458b10(unit->unknown_slot104(), unit->House);
                        unit->Mark(1);
                    } else {
                        unit->Mark(1);
                        unit->Assign_Destination(0, true);
                        unit->unknown_slot220(0);
                        unit->QueueMission(0x10, false);
                    }
                }
            } else if ((deployer || infantry) && unit->Get_Mission() != 0x10) {
                unit->QueueMission(0x10, false);
            }
        }
        unit = (FootClass *)unit->unknown_4c8;
    }
    if (finished) {
        StepCompleted = true;
    }
}


// void TeamClass::TMission_unknown_6bbb40(ScriptMissionClass const &mission, bool unknown) (0x006BBB40)
// Waits Data * 15 frames: when flagged starts the timer, runs 0x006B9F40, and completes the step once the timer has run out.

void TeamClass::TMission_unknown_6bbb40(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        unknown_58 = CDTimerClass<FrameTimerClass>(mission.Data * 15);
    }
    unknown_func_6b9f40();
    if (unknown_58.Value() == 0) {
        StepCompleted = true;
    }
}


// void TeamClass::TMission_unknown_6bbbb0(ScriptMissionClass const &mission, bool unknown) (0x006BBBB0; Red Alert's Coordinate_Do)
// Conscripts every member; a live one without target and destination that strayed beyond twice the
// allowed distance (unless Data is mission 0xB) moves back to the team (+0x34), the cell beside it
// (0x004C8AF0) as destination; otherwise one without target and destination takes mission Data (except
// guard 5 over unloading 0x10).

void TeamClass::TMission_unknown_6bbbb0(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *unit = Member;
    int do_mission = mission.Data;
    while (unit != 0) {
        unknown_func_6ba7d0(unit);
        if (unknown_func_6bdd80(unit) && (unit->unknown_579 || unit->Kind_Of() == 2)) {
            if (unit->unknown_228 == 0 && unit->unknown_494 == 0
                && unit->unknown_5d4c00(unknown_34) > Unknown_006BE6A0() * 2 && do_mission != 0xB) {
                unit->QueueMission(2, false);
                unit->Assign_Destination(unknown_34, true);
                unit->QueueMission(2, false);
                Cell dest = unit->unknown_4c8af0(Coord_Cell(unknown_34->Center_Coord()));
                unit->Assign_Destination(&Map[dest], true);
            } else if (unit->unknown_228 == 0 && unit->unknown_494 == 0 && unit->Get_Mission() != do_mission
                && (do_mission != 5 || unit->Get_Mission() != 0x10)) {
                unit->unknown_6d6e50(0);
                unit->QueueMission(do_mission, false);
                unit->unknown_slot220(0);
                unit->Assign_Destination(0, true);
            }
        }
        unit = (FootClass *)unit->unknown_4c8;
    }
}


// void TeamClass::TMission_unknown_6bbe60(ScriptMissionClass const &mission, bool unknown) (0x006BBE60)
// Sets local variable Data.

void TeamClass::TMission_unknown_6bbe60(ScriptMissionClass const &mission, bool unknown)
{
    unknown_id_source_a3d290->unknown_6612c0(mission.Data, true);
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bbe90(ScriptMissionClass const &mission, bool unknown) (0x006BBE90)
// Clears local variable Data.

void TeamClass::TMission_unknown_6bbe90(ScriptMissionClass const &mission, bool unknown)
{
    unknown_id_source_a3d290->unknown_6612c0(mission.Data, false);
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bbec0(ScriptMissionClass const &mission, bool unknown) (0x006BBEC0)
// Sets global variable Data.

void TeamClass::TMission_unknown_6bbec0(ScriptMissionClass const &mission, bool unknown)
{
    unknown_id_source_a3d290->unknown_661560(mission.Data, true);
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bbef0(ScriptMissionClass const &mission, bool unknown) (0x006BBEF0)
// Clears global variable Data.

void TeamClass::TMission_unknown_6bbef0(ScriptMissionClass const &mission, bool unknown)
{
    unknown_id_source_a3d290->unknown_661560(mission.Data, false);
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bbf20(ScriptMissionClass const &mission, bool unknown) (0x006BBF20)
// Re-picks the team's targets (+0x34/+0x38 through 0x006B9610), copies +0x34 to +0x40, runs 0x006BA1A0, and then points every live, placed member at the new target's own target.

void TeamClass::TMission_unknown_6bbf20(ScriptMissionClass const &mission, bool unknown)
{
    unknown_func_6b9610(&unknown_34, &unknown_38);
    unknown_40 = unknown_34;
    unknown_func_6ba1a0();
    TechnoClass *target = unknown_34->As_Techno();
    if (target != 0) {
        for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
            if (unknown_func_6bdd80(member) && (member->unknown_579 || member->Kind_Of() == 2)) {
                if (member->unknown_228 != 0 && member->unknown_228 != target->unknown_228) {
                    member->unknown_slot220(0);
                }
                if (target->unknown_228 != 0) {
                    if (!member->Locomotor.Checked()->Is_Moving() && member->unknown_slot163() && member->Get_Mission() == 5) {
                        member->QueueMission(1, false);
                        member->unknown_slot220(target->unknown_228);
                    }
                }
            }
        }
    }
}


// void TeamClass::TMission_unknown_6bc040(ScriptMissionClass const &mission, bool unknown) (0x006BC040)
// Calls FootClass slot 296 on every member, then completes the step.

void TeamClass::TMission_unknown_6bc040(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        member->unknown_slot296();
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc070(ScriptMissionClass const &mission, bool unknown) (0x006BC070)
// Waits until no member is moving, turning idle members toward direction Data (Data << 13 in the 16-bit facing).

void TeamClass::TMission_unknown_6bc070(ScriptMissionClass const &mission, bool unknown)
{
    bool done = true;
    FootClass *member = Member;
    if (member != 0) {
        do {
            if (member->Locomotor.Checked()->Is_Moving()) {
                done = false;
            } else {
                int facing = mission.Data;
                if (member->unknown_2e0.Current().Raw != (unsigned short)(facing << 13)) {
                    member->Locomotor.Checked()->Do_Turn(Facing_Dir(mission.Data));
                }
            }
            member = (FootClass *)member->unknown_4c8;
        } while (member != 0);
        if (!done) {
            return;
        }
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc130(ScriptMissionClass const &mission, bool unknown) (0x006BC130)
// Calls FootClass slot 295 on every member, then completes the step.

void TeamClass::TMission_unknown_6bc130(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        member->unknown_slot295();
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc160(ScriptMissionClass const &mission, bool unknown) (0x006BC160)
// Calls FootClass slot 294 on every member, then completes the step.

void TeamClass::TMission_unknown_6bc160(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        member->unknown_slot294();
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc190(ScriptMissionClass const &mission, bool unknown) (0x006BC190)
// Calls FootClass slot 297 with the line's data on every member, then completes the step.

void TeamClass::TMission_unknown_6bc190(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        member->unknown_slot297(mission.Data);
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc1e0(ScriptMissionClass const &mission, bool unknown) (0x006BC1E0)
// Moves the script to line mission.Data - 2 (the step after it advances to
// Data - 1) and completes the step.

void TeamClass::TMission_unknown_6bc1e0(ScriptMissionClass const &mission, bool unknown)
{
    Script->Set_Mission(mission.Data - 2);
    StepCompleted = true;
}


extern "C" unsigned int global_6bc210_a35db4;
class Call_6bc210_0 {public: unsigned int Flag_To_Win_004e7ee0(unsigned int flag_argument);};
class Leaf_6bc210 { public: unsigned int get(unsigned int unused_script_argument_1, unsigned int unused_script_argument_2); };
unsigned int Leaf_6bc210::get(unsigned int unused_script_argument_1, unsigned int unused_script_argument_2) { unsigned int flag_result = ((Call_6bc210_0*)(global_6bc210_a35db4))->Flag_To_Win_004e7ee0((unsigned int)(0)); *(unsigned char*)(((unsigned int)this) + 0x80) = 1; return flag_result; }


// void TeamClass::TMission_unknown_6bc250(ScriptMissionClass const &mission, bool unknown) (0x006BC250)
// Completes the step without doing anything.

void TeamClass::TMission_unknown_6bc250(ScriptMissionClass const &mission, bool unknown)
{
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc260(ScriptMissionClass const &mission, bool unknown) (0x006BC260)
// Plays the sound named by the line at full volume, then completes the step.

void TeamClass::TMission_unknown_6bc260(ScriptMissionClass const &mission, bool unknown)
{
    Voc_Play(mission.Data, 1.0f, 0x2000, 0);
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc290(ScriptMissionClass const &mission, bool unknown) (0x006BC290)
// Calls 0x005A07F0 with the line's data, -1 and three ones, then completes the step.

void TeamClass::TMission_unknown_6bc290(ScriptMissionClass const &mission, bool unknown)
{
    Unknown_005A07F0(mission.Data, -1, 1, 1, 1);
    StepCompleted = true;
}


// 0x006bc2c0: function not yet identified
//
// Body: 32 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall FUN_006bc2c0(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006bc2c0 {

struct Callee_006e7e20 { void Queue_Song_006e7e20(int value); };
extern "C" unsigned int DAT_00a35d78;

struct Owner_006bc2c0 { void FUN_006bc2c0(int argument_address, int unused_1); };

void Owner_006bc2c0::FUN_006bc2c0(int argument_address, int unused_1)
{
    int team_address = (int)this;
    ((Callee_006e7e20*)(&DAT_00a35d78))->Queue_Song_006e7e20((int)(*(unsigned int *)(argument_address + 4)));
    *(unsigned char *)(team_address + 0x80) = 1;
    return;
}

}  // namespace seed_006bc2c0


// void TeamClass::TMission_unknown_6bc2e0(ScriptMissionClass const &mission, bool unknown) (0x006BC2E0)
// Calls 0x0055D990 on the cell of every member, then completes the step.

void TeamClass::TMission_unknown_6bc2e0(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        Map.Unknown_0055D990(Coord_Cell(member->Center_Coord()));
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc360(ScriptMissionClass const &mission, bool unknown) (0x006BC360)
// Sets the team type owner's +0x136 flag.

void TeamClass::TMission_unknown_6bc360(ScriptMissionClass const &mission, bool unknown)
{
    Class->Owner->unknown_136 = true;
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc380(ScriptMissionClass const &mission, bool unknown) (0x006BC380)
// Sets the team type owner's +0x198 to 4.

void TeamClass::TMission_unknown_6bc380(ScriptMissionClass const &mission, bool unknown)
{
    Class->Owner->unknown_198 = 4;
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc3a0(ScriptMissionClass const &mission, bool unknown) (0x006BC3A0)
// Kills every member that is alive and on the map (slot 86 with the rules' warhead), repeating while any was hit, then completes the step.

void TeamClass::TMission_unknown_6bc3a0(ScriptMissionClass const &mission, bool unknown)
{
    bool hit;
    do {
        hit = false;
        for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
            if (member->Strength > 0 && member->unknown_7d && member->unknown_74 && !member->unknown_76) {
                int damage = member->Strength;
                member->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
                hit = true;
            }
        }
    } while (hit);
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc410(ScriptMissionClass const &mission, bool unknown) (0x006BC410)
// Takes every member off the map (slot 52) and removes it (slot 60), then completes the step.

void TeamClass::TMission_unknown_6bc410(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *member = Member;
    while (member != 0) {
        FootClass *next = (FootClass *)member->unknown_4c8;
        member->Limbo();
        member->Delete_Me();
        member = next;
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc460(ScriptMissionClass const &mission, bool unknown) (0x006BC460)
// Unless 0x0051FC50 reports its flag, calls 0x0051F930 with the line's data, a rules value and the zero cell, then completes the step.

void TeamClass::TMission_unknown_6bc460(ScriptMissionClass const &mission, bool unknown)
{
    if (!Unknown_0051FC50()) {
        Unknown_0051F930(mission.Data, Rules->unknown_13e8, unknown_geometry_k, 0);
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc4a0(ScriptMissionClass const &mission, bool unknown) (0x006BC4A0)
// Runs 0x0051FBB0 when 0x0051FC50 reports its flag, then completes the step.

void TeamClass::TMission_unknown_6bc4a0(ScriptMissionClass const &mission, bool unknown)
{
    if (Unknown_0051FC50()) {
        Unknown_0051FBB0();
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc4c0(ScriptMissionClass const &mission, bool unknown) (0x006BC4C0)
// Moves the tactical view to the team's target, raised over the map (and over cells flagged 0x100 or 0x400), using Data.

void TeamClass::TMission_unknown_6bc4c0(ScriptMissionClass const &mission, bool unknown)
{
    CoordStruct coord = unknown_34->Center_Coord();
    coord.Z = Map.Get_Height_GL(coord);
    if ((Map[coord].unknown_140 & 0x100) || (Map[coord].unknown_140 & 0x400)) {
        coord.Z += unknown_geometry_j;
    }
    unknown_tactical_83996c->Setup_Trigger_Scroll(coord, mission.Data);
    StepCompleted = true;
}


extern "C" unsigned char data_6bc570_8324e0[];
class Call_6bc570_0 {public: unsigned int target();};
class Leaf_6bc570 { public: unsigned int get(unsigned int unused_script_argument_1, unsigned int unused_script_argument_2); };
unsigned int Leaf_6bc570::get(unsigned int unused_script_argument_1, unsigned int unused_script_argument_2) { unsigned int call_result = ((Call_6bc570_0*)((unsigned int)data_6bc570_8324e0))->target(); *(unsigned char*)(((unsigned int)this) + 0x80) = 1; return call_result; }


extern "C" unsigned char data_6bc590_8324e0[];
class Call_6bc590_0 {public: unsigned int target();};
class Leaf_6bc590 { public: unsigned int get(unsigned int unused_script_argument_1, unsigned int unused_script_argument_2); };
unsigned int Leaf_6bc590::get(unsigned int unused_script_argument_1, unsigned int unused_script_argument_2) { unsigned int call_result = ((Call_6bc590_0*)((unsigned int)data_6bc590_8324e0))->target(); *(unsigned char*)(((unsigned int)this) + 0x80) = 1; return call_result; }


// void TeamClass::TMission_unknown_6bc5b0(ScriptMissionClass const &mission, bool unknown) (0x006BC5B0)
// Completes the step once every member holds as many passengers as its type carries (leaves it pending while any is short).

void TeamClass::TMission_unknown_6bc5b0(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        if (member->Techno_Type_Class()->Passengers > member->Cargo.Count) {
            return;
        }
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc5f0(ScriptMissionClass const &mission, bool unknown) (0x006BC5F0)
// Turns every TRUCKB unit of the team (the type is the TRUCKA one) and completes the step.

void TeamClass::TMission_unknown_6bc5f0(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        if (member->Kind_Of() == 1 && _stricmp(member->Techno_Type_Class()->ID, "TRUCKB") == 0) {
            ((UnitClass *)member)->Type = UnitTypes[UnitTypeClass::From_Name("TRUCKA")];
        }
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc660(ScriptMissionClass const &mission, bool unknown) (0x006BC660)
// Turns every TRUCKA unit of the team (the type is the TRUCKB one) and completes the step.

void TeamClass::TMission_unknown_6bc660(ScriptMissionClass const &mission, bool unknown)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        if (member->Kind_Of() == 1 && _stricmp(member->Techno_Type_Class()->ID, "TRUCKA") == 0) {
            ((UnitClass *)member)->Type = UnitTypes[UnitTypeClass::From_Name("TRUCKB")];
        }
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc6d0(ScriptMissionClass const &mission, bool unknown) (0x006BC6D0)
// Picks a target of a building type (Data: type index in the low word, a
// value in the high word) for the first member, then waits until every
// member has ammo and completes the step.

extern DynamicVectorClass<BuildingTypeClass *> BuildingTypes;      // 0x00A35CD8

void TeamClass::TMission_unknown_6bc6d0(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown && unknown_3c == 0 && Member != 0) {
        FootClass *first = Member;
        int value = (mission.Data >> 16) & 0xFFFF;
        BuildingTypeClass *type = BuildingTypes[mission.Data & 0xFFFF];
        HouseClass *enemy = 0;
        if (first->House->unknown_54f0 != -1) {
            enemy = Houses[first->House->unknown_54f0];
        }
        AbstractClass *target = Unknown_006BCF70(type, enemy, first, value, Class->OnlyTargetHouseEnemy);
        if (target != 0) {
            unknown_func_6b79e0(target);
        }
    }
    if (unknown_3c != 0) {
        for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
            if (member->Techno_Type_Class()->Ammo <= 0 || member->unknown_254 > 0) {
                unknown_func_6b9bb0();
                return;
            }
        }
    }
    unknown_func_6b79e0(0);
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bc7b0(ScriptMissionClass const &mission, bool unknown) (0x006BC7B0)
// Picks a target of a building type (Data: type index in the low word, a value in the high word) for
// the first member and targets the reachable cell nearest it in the first member's movement zone
// (none when there is none); completes the step when the team has no second target, then runs 0x006BA1A0.

extern DynamicVectorClass<BuildingTypeClass *> BuildingTypes;      // 0x00A35CD8

void TeamClass::TMission_unknown_6bc7b0(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown_3c == 0 && Member != 0) {
        FootClass *first = Member;
        int value = (mission.Data >> 16) & 0xFFFF;
        BuildingTypeClass *type = BuildingTypes[mission.Data & 0xFFFF];
        HouseClass *enemy = 0;
        if (first->House->unknown_54f0 != -1) {
            enemy = Houses[first->House->unknown_54f0];
        }
        AbstractClass *target = Unknown_006BCF70(type, enemy, first, value, Class->OnlyTargetHouseEnemy);
        if (target != 0) {
            int zone_type = first->Techno_Type_Class()->MovementZone;
            CoordStruct coord = ((ObjectClass *)target)->Coord;
            Cell cell = Coord_Cell(coord);
            Cell found = Map.Nearby_Location(cell, first->Techno_Type_Class()->SpeedType,
                Map.Zone(Coord_Cell(first->Center_Coord()), zone_type, first->unknown_79), zone_type, false,
                Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
            if (found != unknown_geometry_k) {
                unknown_func_6b79e0(&Map[found]);
            } else {
                unknown_func_6b79e0(0);
            }
        }
        if (unknown_3c == 0) {
            StepCompleted = true;
        }
    }
    unknown_func_6ba1a0();
}


// void TeamClass::TMission_unknown_6bc980(ScriptMissionClass const &mission, bool unknown) (0x006BC980)
// Picks a target of a building type (Data: type index in the low word, a value in the high word) for
// the first member and targets the reachable cell nearest it in the first member's movement zone
// (none when there is none); completes the step when the team has no second target, then runs 0x006BA1A0.

extern DynamicVectorClass<BuildingTypeClass *> BuildingTypes;      // 0x00A35CD8

void TeamClass::TMission_unknown_6bc980(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown && unknown_3c == 0 && Member != 0) {
        int value = (mission.Data >> 16) & 0xFFFF;
        BuildingTypeClass *type = BuildingTypes[mission.Data & 0xFFFF];
        FootClass *first = unknown_func_6baa60();
        AbstractClass *target = Unknown_006BD240(type, 0, first, value, false);
        if (target != 0) {
            int zone_type = first->Techno_Type_Class()->MovementZone;
            CoordStruct coord = ((ObjectClass *)target)->Coord;
            Cell cell = Coord_Cell(coord);
            Cell found = Map.Nearby_Location(cell, first->Techno_Type_Class()->SpeedType,
                Map.Zone(Coord_Cell(first->Center_Coord()), zone_type, first->unknown_79), zone_type, false,
                Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
            if (found != unknown_geometry_k) {
                unknown_func_6b79e0(&Map[found]);
            } else {
                unknown_func_6b79e0(0);
            }
        }
    }
    if (unknown_3c == 0) {
        StepCompleted = true;
    } else {
        unknown_func_6ba1a0();
    }
}


// void TeamClass::TMission_unknown_6bcbb0(ScriptMissionClass const &mission, bool unknown) (0x006BCBB0)

// Complete original-backed BuildingClass-vector copies; no helper source credit.

void TeamClass::TMission_unknown_6bcbb0(ScriptMissionClass const &mission, bool unknown)
{
    bool found = true;
    if (unknown_30 == 0) {
        int count = 0;
        int index;
        for (index = 0; index < Owner->ScoutNodes.Count(); index++) {
            if (!Owner->ScoutNodes[index].IsPreferred) {
                count++;
            }
        }
        if (count > 0) {
            int pick = unknown_id_source_a3d290->Random()(0, count - 1);
            for (index = 0; index < Owner->ScoutNodes.Count(); index++) {
                if (!Owner->ScoutNodes[index].IsPreferred) {
                    if (pick == 0) {
                        unknown_30 = (AbstractClass *)Owner->ScoutNodes[index].House;
                        break;
                    }
                    pick--;
                }
            }
            DynamicVectorClass<BuildingClass *> list;
            for (index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
                if ((HouseClass *)unknown_30 == unknown_BuildingClass_list_00a40af8[index]->House) {
                    list.Add(unknown_BuildingClass_list_00a40af8[index]);
                }
            }
            if (list.Count() > 0) {
                BuildingClass *building = list[unknown_id_source_a3d290->Random()(0, list.Count() - 1)];
                FootClass *leader = unknown_func_6baa60();
                Point2D radius(1, 1);
                Cell no_cell(0, 0);
                CoordStruct coord = building->Coord;
                Cell dest = Map.Nearby_Location(Coord_Cell(coord), leader->Techno_Type_Class()->SpeedType, -1, 0, false,
                    radius, 0, 0, 0, 1, no_cell, 0);
                if (dest != unknown_geometry_k) {
                    unknown_func_6b79e0(&Map[dest]);
                } else {
                    unknown_func_6b79e0(0);
                }
            } else {
                Owner->unknown_4eec70((HouseClass *)unknown_30);
                unknown_30 = 0;
            }
        } else {
            found = false;
        }
    } else {
        FootClass *leader = unknown_func_6baa60();
        if (leader != 0 && leader->Mission != 2 && leader->MissionQueue != 2) {
            Owner->unknown_4eec70((HouseClass *)unknown_30);
            unknown_30 = 0;
        }
    }
    unknown_func_6ba1a0();
    if (found) {
        StepCompleted = false;
    } else {
        StepCompleted = true;
    }
}


// BuildingClass *__fastcall Unknown_006BCF70(BuildingTypeClass *type, HouseClass *enemy, FootClass *member, int mode, bool enemy_only) (0x006BCF70)
// The building of a type that rates best by mode (0 lowest and 1 highest house value of its cell's
// block, 2 nearest and 3 farthest from the member) among enemy's buildings and those of houses the
// member's house is not allied with (or passive ones): enemy's best when it beats the overall best,
// else the overall best unless enemy_only.

AbstractClass *__fastcall Unknown_006BCF70(BuildingTypeClass *type, HouseClass *enemy, FootClass *member, int mode, bool enemy_only)
{
    BuildingClass *enemy_best = 0;
    BuildingClass *best = 0;
    int enemy_best_value = -1;
    int best_value = -1;
    for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
        bool is_enemy = building->House == enemy;
        if (building->Type == type
            && (is_enemy || !member->House->Is_Ally(building->House) || building->House->Type->MultiplayPassive)) {
            int value = -1;
            switch (mode) {
            case 0: {
                Cell cell = Coord_Cell(building->Center_Coord());
                value = 0x7FFFFFFF - Map.Cell_Threat(cell, member->House);
                break;
            }
            case 1: {
                Cell cell = Coord_Cell(building->Center_Coord());
                value = Map.Cell_Threat(cell, member->House);
                break;
            }
            case 2: {
                CoordStruct here = member->Coord;
                CoordStruct there = building->Coord;
                value = 0x7FFFFFFF - Coord_Distance(there, here);
                break;
            }
            case 3: {
                CoordStruct here = member->Coord;
                CoordStruct there = building->Coord;
                value = Coord_Distance(there, here);
                break;
            }
            }
            if (value > enemy_best_value && is_enemy) {
                enemy_best = building;
                enemy_best_value = value;
            }
            if (value > best_value) {
                best = building;
                best_value = value;
            }
        }
    }
    if (enemy_best != 0 && enemy_best_value > best_value) {
        return enemy_best;
    }
    if (enemy_only) {
        return enemy_best;
    }
    return best;
}


// BuildingClass *__fastcall Unknown_006BD240(BuildingTypeClass *type, HouseClass *enemy, FootClass *member, int mode, bool arg5) (0x006BD240)
// The member's own house's building of a type that rates best by mode: 0 lowest and 1 highest
// house value of its cell's block (0x0054E3A0), 2 nearest and 3 farthest from the member.

AbstractClass *__fastcall Unknown_006BD240(BuildingTypeClass *type, HouseClass *enemy, FootClass *member, int mode, bool arg5)
{
    BuildingClass *best = 0;
    int best_value = -1;
    HouseClass *house = member->House;
    int count = house->Buildings.Count();
    for (int index = 0; index < count; index++) {
        BuildingClass *building = house->Buildings[index];
        if (building->Type == type) {
            int value = -1;
            switch (mode) {
            case 0: {
                Cell cell = Coord_Cell(building->Center_Coord());
                value = 0x7FFFFFFF - Map.Cell_Threat(cell, member->House);
                break;
            }
            case 1: {
                Cell cell = Coord_Cell(building->Center_Coord());
                value = Map.Cell_Threat(cell, member->House);
                break;
            }
            case 2: {
                CoordStruct here = member->Coord;
                CoordStruct there = building->Coord;
                CoordStruct delta(there.X-here.X, there.Y-here.Y, there.Z-here.Z);
                value = 0x7FFFFFFF - (int)unknown_sqrt_4ba410((double)delta.X * delta.X + (double)delta.Z * delta.Z + (double)delta.Y * delta.Y);
                break;
            }
            case 3: {
                CoordStruct here = member->Coord;
                CoordStruct there = building->Coord;
                value = Coord_Distance(there, here);
                break;
            }
            }
            if (value > best_value) {
                best_value = value;
                best = building;
            }
        }
    }
    return best;
}


// void TeamClass::TMission_unknown_6bd4b0(ScriptMissionClass const &mission, bool unknown) (0x006BD4B0; Red Alert's TMission_Unload)
// Members carrying passengers unload (mission 0x10) where their cell has no building; once none carries
// any, Data says who leaves the team: transports go home (with TransportsReturnOnUnload, to +0x1B0) or
// leave for Data 2 and 3, the others (non-transports, or units when the task force has a transport
// aircraft) leave for Data 1 and 3. Then completes the step.

void TeamClass::TMission_unknown_6bd4b0(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *unit = Member;
    bool finished = true;
    while (unit != 0) {
        FootClass *next = (FootClass *)unit->unknown_4c8;
        unknown_func_6ba7d0(unit);
        if (unknown_func_6bdd80(unit) && (unit->unknown_579 || unit->Kind_Of() == 2)) {
            if (unit->Cargo.Object != 0) {
                finished = false;
                if (Map[unit->Center_Coord()].Cell_Building() == 0 && unit->Get_Mission() != 0x10) {
                    unit->Assign_Destination(0, true);
                    unit->unknown_slot220(0);
                    unit->QueueMission(0x10, false);
                    finished = false;
                }
            }
        }
        unit = next;
    }
    if (!finished) {
        return;
    }
    unit = Member;
    int mode = mission.Data;
    bool aircraft = unknown_func_6bd810();
    while (unit != 0) {
        FootClass *next = (FootClass *)unit->unknown_4c8;
        if (unit->Techno_Type_Class()->Passengers == 0 || aircraft && unit->Kind_Of() != 2) {
            if (mode == 1 || mode == 3) {
                Unknown_006B8FB0(unit, -1, false);
                unit->unknown_slot220(0);
                unit->Assign_Destination(0, true);
            }
        } else if (unit->Techno_Type_Class()->Passengers > 0) {
            if (Class->TransportsReturnOnUnload) {
                Unknown_006B8FB0(unit, -1, false);
                unit->unknown_slot220(0);
                unit->Assign_Destination(unit->unknown_1b0, true);
                unit->QueueMission(2, false);
                if (unit->unknown_slot122()) {
                    unit->Commence();
                }
                unit->unknown_6d6e50(0);
            } else if (mode == 3 || mode == 2) {
                Unknown_006B8FB0(unit, -1, false);
                unit->unknown_slot220(0);
                unit->Assign_Destination(0, true);
            }
        }
        unit = next;
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bd7f0(ScriptMissionClass const &mission, bool unknown) (0x006BD7F0)
// Completes the step and sets IsSuspended.

void TeamClass::TMission_unknown_6bd7f0(ScriptMissionClass const &mission, bool unknown)
{
    StepCompleted = true;
    IsSuspended = true;
}


// Whether the team type's task force has a transporter entry (kind 3) that carries passengers.

bool TeamClass::unknown_func_6bd810()
{
    for (int index = 0; index < Class->TaskForce->CountEntries; index++) {
        TechnoTypeClass *type = Class->TaskForce->Entries[index].Type;
        if (type->Kind_Of() == 3 && type->Passengers > 0) {
            return true;
        }
    }
    return false;
}


// Fills a list with the techno types the team type's task force still lacks: every entry's type once per count, minus one for each current member.

void TeamClass::unknown_func_6bd870(DynamicVectorClass<TechnoTypeClass *> &types)
{
    for (int index = 0; index < Class->TaskForce->CountEntries; index++) {
        TechnoTypeClass *type = Class->TaskForce->Entries[index].Type;
        for (int count = Class->TaskForce->Entries[index].Amount; count > 0; count--) {
            types.Add(type);
        }
    }
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        TechnoTypeClass *type = member->Techno_Type_Class();
        types.Delete(type);
    }
}


// 0x006bd960: function not yet identified
//
// Body: 48 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall FUN_006bd960(int param_1,int param_2,char param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006bd960 {

struct Owner_006bd960 { void FUN_006bd960(int argument_address, char apply_to_members); };

void Owner_006bd960::FUN_006bd960(int argument_address, char apply_to_members)
{
    int team_address = (int)this;
    unsigned int member_value;
    int member_address;

    if (apply_to_members != '\0') {
        member_value = (unsigned int)(*(unsigned int *)(argument_address + 4));
        for (member_address = *(int *)(team_address + 0x54); member_address != 0; member_address = *(int *)(member_address + 0x4c8)) {
            *(unsigned int *)(member_address + 0xd8) = member_value;
        }
        *(unsigned char *)(team_address + 0x80) = 1;
    }
    return;
}

}  // namespace seed_006bd960


// void TeamClass::unknown_func_6bd990(int value) (0x006BD990): stores value in every member (+0xD8).

void TeamClass::unknown_func_6bd990(int value)
{
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        member->FlashCount = value;
    }
}


// void TeamClass::TMission_unknown_6bd9b0(ScriptMissionClass const &mission, bool unknown) (0x006BD9B0)
// When flagged, creates the animation (type = low 16 bits of Data, loops = high 16 bits) at every member and attaches it.

void TeamClass::TMission_unknown_6bd9b0(ScriptMissionClass const &mission, bool unknown)
{
    int type = mission.Data & 0xFFFF;
    if (unknown && type >= 0 && type < AnimTypes.Count()) {
        FootClass *member = Member;
        unsigned int loops = mission.Data >> 16 & 0xFFFF;
        AnimTypeClass *anim_type = AnimTypes[type];
        if (member != 0) {
            do {
                AnimClass *anim = new AnimClass(anim_type, member->Center_Coord(), 0, loops, 0x600, 0);
                if (anim != 0) {
                    anim->Attach_To(member);
                }
                member = (FootClass *)member->unknown_4c8;
            } while (member != 0);
            StepCompleted = true;
            return;
        }
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6bda70(ScriptMissionClass const &mission, bool unknown) (0x006BDA70)
// When flagged, calls 0x006D8FD0 for the first member with the line's data, then completes the step.

void TeamClass::TMission_unknown_6bda70(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        Unknown_006D8FD0(Member, mission.Data);
        StepCompleted = true;
    }
}


// Gather near the enemy base, with native RA2 behavior and widths.
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
// Choose a point at the safe distance from the enemy base along the bearing.
// Hypothesis: file-local operation boundary, with no original out-of-line identity.
static inline UnknownVector3 Enemy_Gather_Point(CoordStruct const &there, UnknownFacingDir const &dir, int distance)
{
    double angle = ((short)dir.Raw - 0x3fff) * unknown_facing_radians_79b620;
    double y = there.Y;
    int target_y = (int)(y - unknown_sin_4ba480(angle) * distance);
    int target_x = (int)(unknown_cos_4ba4d0(angle) * distance + there.X);
    UnknownVector3 point(target_x, target_y, there.Z);
    return point;
}

void TeamClass::TMission_unknown_6bdaa0(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        FootClass *leader = unknown_func_6baa60();
        if (leader == 0) { StepCompleted = true; return; }
        HouseClass *house = leader->House;
        HouseClass *enemy = 0;
        if (house->unknown_54f0 != -1) enemy = Houses[house->unknown_54f0];
        if (enemy == 0) { StepCompleted = true; return; }
        CoordStruct there = enemy->BaseCenter;
        if (there == unknown_geometry_l) { StepCompleted = true; return; }
        CoordStruct center = house->BaseCenter;
        if (center == unknown_geometry_l) center = leader->Center_Coord();
        CoordStruct offset = center - there;
        Point2D delta;
        delta.X = offset.X;
        delta.Y = offset.Y;
        double dy = -delta.Y;
        UnknownFacingDir dir((unsigned short)(int)((Fast_Atan2(dy, delta.X) - unknown_facing_origin_79b630)
            * unknown_facing_scale_79b628));
        int distance = Rules->AISafeDistance << 8;
        UnknownVector3 point = Enemy_Gather_Point(there, dir, distance);
        Cell cell = XY_Cell(point.X / 256, point.Y / 256);
        cell = Map.Nearby_Location(cell, leader->Techno_Type_Class()->SpeedType, -1, 0, false,
            Point2D(3,3), false, false, false, true, Cell(0,0), 0);
        unknown_func_6b79e0(&Map[cell]);
        Debug_Printf("A %s Team has chosen ( %d, %d) for its GatherAtEnemy cell.\n", (char const *)Class->Name, cell.X, cell.Y);
    }
    unknown_func_6ba1a0();
}


// Whether an object is alive and may be dealt with: it counts, has strength, and either the editor flag is set or it is not 0x76.

bool __fastcall TeamClass::unknown_func_6bdd80(ObjectClass *object)
{
    if (object != 0 && object->unknown_7d && object->Strength != 0 && (unknown_a40774 != 0 || !object->unknown_76)) {
        return true;
    }
    return false;
}


// void TeamClass::TMission_unknown_6bddb0(ScriptMissionClass const &mission, bool unknown) (0x006BDDB0)
// When unknown is set: from the leader's base center, picks the direction toward the enemy's base
// (a random one without an enemy), goes AISafeDistance cells that way and targets the free 3x3 spot
// nearest there for the leader's speed type (hypothesis: the GatherAtBase action beside 0x006BDAA0's
// GatherAtEnemy). Then runs 0x006BA1A0.

extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;

void TeamClass::TMission_unknown_6bddb0(ScriptMissionClass const &mission, bool unknown)
{
    if (unknown) {
        FootClass *leader = unknown_func_6baa60();
        HouseClass *enemy = 0;
        if (leader != 0 && leader->House->unknown_54f0 != -1) {
            enemy = Houses[leader->House->unknown_54f0];
        }
        CoordStruct center = leader->House->BaseCenter;
        UnknownFacingDir dir;
        if (enemy == 0) {
            dir = UnknownFacingDir((unsigned short)((unsigned char)unknown_id_source_a3d290->Random()(0, 255) << 8));
        } else {
            CoordStruct there = enemy->BaseCenter;
            CoordStruct delta = there - center;
            dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2(-delta.Y, delta.X) - unknown_facing_origin_79b630)
                * unknown_facing_scale_79b628));
        }
        int distance = Rules->AISafeDistance << 8;
        double angle = ((short)dir.Raw - 0x3fff) * unknown_facing_radians_79b620;
        double y = center.Y;
        CoordStruct coord((int)(unknown_cos_4ba4d0(angle) * distance + center.X), (int)(y - unknown_sin_4ba480(angle) * distance), center.Z);
        Cell cell = Coord_Cell(coord);
        cell = Map.Nearby_Location(cell, leader->Techno_Type_Class()->SpeedType, -1, 0, false,
            Point2D(3, 3), 0, 0, 0, 1, Cell(0, 0), 0);
        unknown_func_6b79e0(&Map[cell]);
    }
    unknown_func_6ba1a0();
}


// void TeamClass::TMission_unknown_6be010(ScriptMissionClass const &mission, bool unknown) (0x006BE010)
// Type-1 superweapon handler, twin of 0x006BE1C0: fires it at the team when ready,
// waits while it is charging within the rules' AI fraction, else completes the step.
inline void Complete_Step_006be010(TeamClass *team) { team->StepCompleted=true; }
void TeamClass::TMission_unknown_6be010(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *leader=unknown_func_6baa60();
    if(!leader) { Complete_Step_006be010(this); return; }
    HouseClass *house=leader->House;
    SuperClass *weapon=0;
    int count=house->Supers.Count();
    for(int index=0;index<count;++index) {
        if(house->Supers[index]->Type->Type==1) {
            weapon=house->Supers[index];
            break;
        }
    }
    if(!weapon) { Complete_Step_006be010(this); return; }
    if(!weapon->unknown_0x57) {
        double remaining=weapon->RechargeTimer.Value();
        double total=weapon->Type->RechargeTime;
        if(weapon->unknown_0x55 && remaining/total <= 1.0f-Rules->unknown_b80) return;
        Complete_Step_006be010(this);
        return;
    }
    house->unknown_func_4e68d0(weapon->Type->Type,Coord_Cell(unknown_34->Center_Coord()));
    Complete_Step_006be010(this);
}


// void TeamClass::TMission_unknown_6be1c0(ScriptMissionClass const &mission, bool unknown) (0x006BE1C0)
// Chronoshift: with the leader house's super weapons of types 3 and 4 (hypothesis: ChronoSphere and
// ChronoWarp) and the first ready, picks a building of type Data & 0xFFFF (mode Data >> 16, as 0x006BCF70)
// and fires both, 3 at the team's cell (+0x34) and 4 at the building's, which becomes the team's target;
// completes the step unless the first is still charging below the rules' AI ready fraction.

extern DynamicVectorClass<BuildingTypeClass *> BuildingTypes;      // 0x00A35CD8

void TeamClass::TMission_unknown_6be1c0(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *leader = unknown_func_6baa60();
    if (leader == 0) {
        StepCompleted = true;
        return;
    }
    HouseClass *house = leader->House;
    SuperClass *sphere = 0;
    SuperClass *warp = 0;
    for (int index = 0; index < house->Supers.Count(); index++) {
        if (house->Supers[index]->Type->Type == 3) {
            sphere = house->Supers[index];
        }
        if (house->Supers[index]->Type->Type == 4) {
            warp = house->Supers[index];
        }
    }
    if (sphere != 0 && warp != 0) {
        if (!sphere->unknown_0x57) {
            double remaining = sphere->RechargeTimer.Value();
            double total = sphere->Type->RechargeTime;
            if (sphere->unknown_0x55 && remaining / total <= 1.0f - Rules->unknown_b80) {
                return;
            }
            StepCompleted = true;
            return;
        }
        int value = (mission.Data >> 16) & 0xFFFF;
        BuildingTypeClass *type = BuildingTypes[mission.Data & 0xFFFF];
        HouseClass *enemy = 0;
        if (house->unknown_54f0 != -1) {
            enemy = Houses[house->unknown_54f0];
        }
        AbstractClass *target = Unknown_006BCF70(type, enemy, leader, value, Class->OnlyTargetHouseEnemy);
        if (target != 0) {
            house->unknown_func_4e68d0(sphere->Type->Type, Coord_Cell(unknown_34->Center_Coord()));
            house->unknown_func_4e68d0(warp->Type->Type, Coord_Cell(target->Center_Coord()));
            unknown_func_6b79e0(target);
        }
        StepCompleted = true;
        return;
    }
    StepCompleted = true;
}


// void TeamClass::TMission_unknown_6be440(ScriptMissionClass const &mission, bool unknown) (0x006BE440)
// Chronoshift to a target: with the leader house's super weapons of types 3 and 4 (hypothesis: ChronoSphere
// and ChronoWarp) and the first charged, the leader picks a target of the script argument's kind
// (0x006220E0, slot 219 from its position, the team type's OnlyTargetHouseEnemy); 3 fires at the team's
// cell (+0x34) and 4 at the target's, which becomes the team's target. Completes the step unless the first
// is present and still charging below the rules' AI ready fraction. The early leader exit and the named
// target kind give the original's tail sharing; the inline Position_Coord_004f3f40 site (with the other
// pending sites) keeps 0x006BDD80 out of line in the expanded leader search, as in 0x006BE1C0.

void TeamClass::TMission_unknown_6be440(ScriptMissionClass const &mission, bool unknown)
{
    FootClass *leader = unknown_func_6baa60();
    if (leader == 0) {
        StepCompleted = true;
        return;
    }
    HouseClass *house = leader->House;
    SuperClass *sphere = 0;
    SuperClass *warp = 0;
    for (int index = 0; index < house->Supers.Count(); index++) {
        if (house->Supers[index]->Type->Type == 3) {
            sphere = house->Supers[index];
        }
        if (house->Supers[index]->Type->Type == 4) {
            warp = house->Supers[index];
        }
    }
    if (sphere != 0 && warp != 0) {
        if (!sphere->unknown_0x57) {
            double remaining = sphere->RechargeTimer.Value();
            double total = sphere->Type->RechargeTime;
            if (sphere->unknown_0x55 && remaining / total <= 1.0f - Rules->unknown_b80) {
                return;
            }
            StepCompleted = true;
            return;
        }
        int target_data = Unknown_006220E0(mission.Data);
        AbstractClass *target = leader->unknown_slot219(target_data, &leader->Position_Coord_004f3f40(), Class->OnlyTargetHouseEnemy);
        if (target != 0) {
            house->unknown_func_4e68d0(sphere->Type->Type, Coord_Cell(unknown_34->Center_Coord()));
            house->unknown_func_4e68d0(warp->Type->Type, Coord_Cell(target->Center_Coord()));
            unknown_func_6b79e0(target);
        }
        StepCompleted = true;
        return;
    }
    StepCompleted = true;
}


// How far a member may stray: a rules distance, a different one while the current script line is action 0x35 or 0x36.

int TeamClass::Unknown_006BE6A0()
{
    int action = Script->Get_Current_Mission().Mission;
    if (action != 0x36 && action != 0x35) {
        return Rules->unknown_1384;
    }
    return Rules->unknown_1388;
}


// Whether any member has unlimited ammo or still has some.

bool TeamClass::unknown_func_6be6e0()
{
    bool result = false;
    for (FootClass *member = Member; member != 0; member = (FootClass *)member->unknown_4c8) {
        if (member->Techno_Type_Class()->Ammo <= 0 || member->unknown_254 > 0) {
            result = true;
            break;
        }
    }
    return result;
}


// TeamClass::Size_Of (0x006BE720)
// Compiler-emitted from shared/team.hpp in this unit; no source of its own.


// TeamClass::Kind_Of (0x006BE730)
// Compiler-emitted from shared/team.hpp in this unit; no source of its own.


// TeamClass::scalar_deleting_destructor (0x006BE740)
// Compiler-emitted from shared/team.hpp in this unit; no source of its own.
