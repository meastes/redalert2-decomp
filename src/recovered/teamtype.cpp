// teamtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/teamtype.json carries the addresses and the rows that
// stay in their own files.


#define TYPELIST_IMPLEMENT_DEFAULT_CONSTRUCTOR
#define TYPELIST_IMPLEMENT_COPY_CONSTRUCTOR
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/team_type.hpp"
#include "shared/team.hpp"
#include "shared/house.hpp"
#include "shared/anim_type.hpp"
#include "shared/rules.hpp"
#include "shared/unit.hpp"
#include "shared/ai_trigger_type.hpp"
#include "shared/discrete_distribution.hpp"
#include "shared/id_source.hpp"
#include "shared/debug_printf.hpp"
#include "shared/script.hpp"
#include "shared/house_type.hpp"
#include "shared/tag_type.hpp"
#include "shared/task_force.hpp"
#include "shared/ccini_class.hpp"
#include "shared/coord.hpp"
#include "shared/foot.hpp"
#include "shared/mission.hpp"
#include "shared/techno_type.hpp"
#include "shared/zone_table.hpp"
#include "shared/unknown_globals.hpp"


// The container members TeamTypeClass::Suggested_New_Teams (0x006BED50) expands: TypeList construction and copy,
// DynamicVectorClass Add and Clear (the unit's first source sets the guards before any include).
// Header-static dynamic initializers of the unit at 0x006BE760:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006BE760-0x006BE946); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006BE790, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE7B0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE7D0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE7F0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE810, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE830, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE870, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE8A0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE8D0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE900, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE910, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// Dynamic initializer (0x006BE930, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006be760.cpp; no source of its own.


// static int Hex_To_Int(char const *string) (0x006BE950)
// Parses leading hexadecimal digits (either case); stops at the first other
// character. File-local in the original (no callers found; name hypothesis).

int __fastcall TeamTypeClass_unknown_hex_6be950(char const *string)
{
    int value = 0;
    while (isxdigit(*string)) {
        value *= 16;
        if (isdigit(*string)) {
            value += *string - '0';
        } else {
            value += toupper(*string) - 'A' + 10;
        }
        string++;
    }
    return value;
}


// TeamTypeClass::TeamTypeClass(char const *ini_name) (0x006BE9B0)
// Default team type, registered in TeamTypes and the abstract list
// 0x00AC0CB0; ArrayIndex is its index in TeamTypes.

TeamTypeClass::TeamTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    ArrayIndex(-1),
    Group(-1),
    VeteranLevel(1),
    Loadable(false),
    Full(false),
    Annoyance(false),
    GuardSlower(false),
    Recruiter(false),
    Autocreate(false),
    Prebuild(false),
    Reinforce(false),
    Whiner(false),
    Aggressive(false),
    LooseRecruit(false),
    Suicide(false),
    Droppod(false),
    UseTransportOrigin(false),
    unknown_b2(false),
    OnTransOnly(false),
    Priority(7),
    Max(-1),
    unknown_bc(0),
    Owner(0),
    TechLevel(0),
    Tag(0),
    Waypoint(-1),
    TransportWaypoint(-1),
    unknown_d4(0),
    Script(0),
    TaskForce(0),
    IsGlobal(0),
    unknown_e4(9),
    unknown_e8(true),
    unknown_e9(false),
    AvoidThreats(false),
    IonImmune(false),
    TransportsReturnOnUnload(false),
    AreTeamMembersRecruitable(true),
    IsBaseDefense(false),
    OnlyTargetHouseEnemy(false)
{
    TeamTypes.Add(this);
    ArrayIndex = TeamTypes.ID(this);
    unknown_objects_ac0cb0.Add(this);
}


// TeamTypeClass::~TeamTypeClass() (0x006BEBB0)
// Detaches the type from everything and leaves the team type and abstract
// lists. The scalar deleting destructor (0x006C0280) expands this body.

TeamTypeClass::~TeamTypeClass()
{
    Unknown_006ECB90(this, true);
    TeamTypes.Delete(this);
    unknown_objects_ac0cb0.Delete(this);
}


// TeamClass *TeamTypeClass::Create_One_Of(HouseClass *house) (0x006BEC80)
// Creates a team of this type for the house (the type's owner when none is
// given), unless the type already has its maximum number of teams.

extern int unknown_a40774;                  // 0x00A40774
extern int unknown_game_mode_a3d298;        // 0x00A3D298

TeamClass *TeamTypeClass::Create_One_Of(HouseClass *house)
{
    Debug_Printf("Create_One_Of...\n");
    if (house == 0) {
        house = Owner;
    }
    if (!unknown_a40774 && Max >= 0) {
        if (!unknown_game_mode_a3d298) {
            if (unknown_d4 >= Max) {
                return 0;
            }
        } else if (house->Team_Count(this) >= Max) {
            return 0;
        }
    }
    Debug_Printf("Creating a new team named '%s'.\n", (char const *)Name);
    return new TeamClass(this, house, 0);
}


// void TeamTypeClass::Destroy_All_Of() const (0x006BED10)
// Deletes every team of this type.

void TeamTypeClass::Destroy_All_Of() const
{
    for (int index = 0; index < Teams.Count(); index++) {
        TeamClass *team = Teams[index];
        if (this == team->Class) {
            delete team;
            index--;
        }
    }
}


// TypeList<TeamTypeClass const *> TeamTypeClass::Suggested_New_Teams(HouseClass *house, bool alerted) (0x006BED50)
// The team types a house may make now: when its AI triggers are on and a 1-100 roll is within its ratio, enforce the
// team caps (dropping the oldest base defense team when over the cap), pick one ready AI trigger by weight (weight 5000
// triggers exclude all others) and take its one or two team types; nothing when a team of a suggested type is
// already idle or reforming. The suggested types are marked autocreate.

TypeList<TeamTypeClass const *> __fastcall TeamTypeClass::Suggested_New_Teams(HouseClass *house, bool alerted)
{
    int index;
    bool skip_base_defense;

    TypeList<TeamTypeClass const *> best;
    HouseClass *enemy = 0;

    if (house->unknown_54f0 != -1) {
        enemy = Houses[house->unknown_54f0];
    }
    if (Random_Pick(1, 100) <= house->unknown_554c && house->unknown_13a) {
        int team_count = 0;
        int defense_team_count = 0;
        for (index = 0; index < Teams.Count(); index++) {
            if (Teams[index]->Owner == house) {
                team_count++;
                if (Teams[index]->Class->IsBaseDefense) {
                    defense_team_count++;
                }
            }
        }

        skip_base_defense = false;
        if (team_count < Rules->TotalAITeamCap[house->unknown_c8] || defense_team_count < team_count / 2) {
            if (defense_team_count > Rules->MaximumAIDefensiveTeams[house->unknown_c8]) {
                skip_base_defense = true;
            }
        } else {
            TeamClass *team = 0;
            int frame = 0x7FFFFFFF;
            for (index = 0; index < Teams.Count(); index++) {
                TeamClass *t = Teams[index];
                if (t->Owner == house) {
                    if (t->Class->IsBaseDefense) {
                        if (t->unknown_50 < frame) {
                            team = t;
                            frame = t->unknown_50;
                        }
                    }
                }
            }
            if (team != 0) {
                team_count--;
                skip_base_defense = true;
                delete team;
            }
        }

        if (team_count < Rules->TotalAITeamCap[house->unknown_c8]) {
            DiscreteDistributionClass<AITriggerTypeClass> trigdist;
            bool exclusive = false;
            for (index = 0; index < AITriggerTypes.Count(); index++) {
                if (AITriggerTypes[index] != 0) {
                    AITriggerTypeClass *trig = AITriggerTypes[index];
                    if (trig->Is_Ready(house, enemy, skip_base_defense) == true) {
                        int weight = (int)trig->Weight;
                        if (weight == 5000) {
                            if (!exclusive) {
                                exclusive = true;
                                trigdist.Clear();
                                Debug_Printf("Infinite weight AIT hit; clearing distribution.\n");
                            }
                        } else if (exclusive) {
                            continue;
                        }
                        trigdist.Add(trig, weight);
                    }
                }
            }
            AITriggerTypeClass *trig = trigdist.Sample();
            if (trig != 0) {
                TeamTypeClass const *first = trig->unknown_dc;
                if (first != 0) {
                    best.Add(first);
                }
            }
            if (trig != 0) {
                TeamTypeClass const *second = trig->unknown_e0;
                if (second != 0) {
                    best.Add(second);
                }
            }
        }
    }

    for (index = 0; index < Teams.Count(); index++) {
        if (Teams[index]->Owner == house && (Teams[index]->IsReforming || !Teams[index]->IsMoving)) {
            int j = 0;
            if (j < best.Count()) {
                TeamTypeClass *cls = Teams[index]->Class;
                while (true) {
                    if (cls == best[j]) {
                        best.Clear();
                        return best;
                    }
                    j++;
                    if (j >= best.Count()) {
                        break;
                    }
                }
            }
        }
    }

    for (index = 0; index < best.Count(); index++) {
        TeamTypeClass *ttype = (TeamTypeClass *)best[index];
        ttype->Autocreate = true;
    }

    return best;
}


// TeamTypeClass *TeamTypeClass::Find_By_ID_Or_Name(char const *name) (0x006BF260)
// The first team type whose ID or Name matches name (case-insensitive).

TeamTypeClass *__fastcall TeamTypeClass::Find_By_ID_Or_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TeamTypes.Count(); index++) {
            if (_stricmp(name, TeamTypes[index]->ID) == 0 || _stricmp(name, TeamTypes[index]->Name) == 0) {
                return TeamTypes[index];
            }
        }
    }
    return 0;
}


// void TeamTypeClass::Detach(AbstractClass *target, bool all) (0x006BF2D0)
// Forgets target as tag, owner, task force or script, then lets the script
// type forget it too.

void TeamTypeClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Tag == target) {
        Tag = 0;
    }
    if ((AbstractClass *)Owner == target) {
        Owner = 0;
    }
    if ((AbstractClass *)TaskForce == target) {
        TaskForce = 0;
    }
    if ((AbstractClass *)Script == target) {
        Script = 0;
    }
    if (Script != 0) {
        Script->Detach(target, all);
    }
}


// bool TeamTypeClass::Read_INI(CCINIClass &ini) (0x006BF330, slot 25)
// The abstract type's keys, then the team keys under the team's ID: the
// owner (by house type index), the flags and limits, the tag, waypoints
// (by name), script and task force (made when missing).


// The name of a waypoint index ("A".."ZZ") and back (0x007249F0,
// 0x00724A70, __fastcall; names hypotheses).
char *__fastcall Waypoint_Name_7249f0(int waypoint);
int __fastcall Waypoint_From_Name_724a70(char *name);

static inline TagTypeClass *Get_Tag(CCINIClass &ini, char const *section, char const *entry, TagTypeClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return TagTypeClass::Find_Or_Make(name);
    }
    return defvalue;
}

static inline ScriptTypeClass *Get_Script(CCINIClass &ini, char const *section, char const *entry, ScriptTypeClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return ScriptTypeClass::Find_Or_Make(name);
    }
    return defvalue;
}

static inline TaskForceClass *Get_Task_Force(CCINIClass &ini, char const *section, char const *entry, TaskForceClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return TaskForceClass::Find_Or_Make(name);
    }
    return defvalue;
}

bool TeamTypeClass::Read_INI(CCINIClass &ini)
{
    if (AbstractTypeClass::Read_INI(ini)) {
        int house = -1;
        if (Owner != 0) {
            house = Owner->Type->House;
        }
        house = ini.Get_HousesType(ID, "House", house);
        if (house != -1) {
            Owner = HouseClass::As_Pointer(house);
        }
        VeteranLevel = ini.Get_Int(ID, "VeteranLevel", VeteranLevel);
        Loadable = ini.Get_Bool(ID, "Loadable", Loadable);
        Full = ini.Get_Bool(ID, "Full", Full);
        Annoyance = ini.Get_Bool(ID, "Annoyance", Annoyance);
        GuardSlower = ini.Get_Bool(ID, "GuardSlower", GuardSlower);
        Autocreate = ini.Get_Bool(ID, "Autocreate", Autocreate);
        Prebuild = ini.Get_Bool(ID, "Prebuild", Prebuild);
        Reinforce = ini.Get_Bool(ID, "Reinforce", Reinforce);
        Droppod = ini.Get_Bool(ID, "Droppod", Droppod);
        UseTransportOrigin = ini.Get_Bool(ID, "UseTransportOrigin", UseTransportOrigin);
        Recruiter = ini.Get_Bool(ID, "Recruiter", Recruiter);
        Whiner = ini.Get_Bool(ID, "Whiner", Whiner);
        Suicide = ini.Get_Bool(ID, "Suicide", Suicide);
        LooseRecruit = ini.Get_Bool(ID, "LooseRecruit", LooseRecruit);
        Aggressive = ini.Get_Bool(ID, "Aggressive", Aggressive);
        OnTransOnly = ini.Get_Bool(ID, "OnTransOnly", OnTransOnly);
        Priority = ini.Get_Int(ID, "Priority", Priority);
        Max = ini.Get_Int(ID, "Max", Max);
        TechLevel = ini.Get_Int(ID, "TechLevel", TechLevel);
        Tag = Get_Tag(ini, ID, "Tag", Tag);
        Group = ini.Get_Int(ID, "Group", Group);
        AvoidThreats = ini.Get_Bool(ID, "AvoidThreats", AvoidThreats);
        IonImmune = ini.Get_Bool(ID, "IonImmune", IonImmune);
        IsBaseDefense = ini.Get_Bool(ID, "IsBaseDefense", IsBaseDefense);
        OnlyTargetHouseEnemy = ini.Get_Bool(ID, "OnlyTargetHouseEnemy", OnlyTargetHouseEnemy);
        TransportsReturnOnUnload = ini.Get_Bool(ID, "TransportsReturnOnUnload", TransportsReturnOnUnload);
        AreTeamMembersRecruitable = ini.Get_Bool(ID, "AreTeamMembersRecruitable", AreTeamMembersRecruitable);
        char buffer[64];
        if (ini.Get_String(ID, "Waypoint", Waypoint_Name_7249f0(Waypoint), buffer, sizeof(buffer)) > 0) {
            Waypoint = Waypoint_From_Name_724a70(buffer);
        }
        if (ini.Get_String(ID, "TransportWaypoint", Waypoint_Name_7249f0(TransportWaypoint), buffer, sizeof(buffer)) > 0) {
            TransportWaypoint = Waypoint_From_Name_724a70(buffer);
        }
        Script = Get_Script(ini, ID, "Script", Script);
        TaskForce = Get_Task_Force(ini, ID, "TaskForce", TaskForce);
        return true;
    }
    return false;
}


// bool TeamTypeClass::Write_INI(CCINIClass &ini) const (0x006BF770, slot 26)
// The abstract type's keys, then every team key under the team's ID; the
// house is written by its type's index (-1 for none), waypoints by name,
// and the tag, script and task force by ID when set.

// The name of a waypoint index ("A".."ZZ"; 0x007249F0, __fastcall; name
// hypothesis).
char *__fastcall Waypoint_Name_7249f0(int waypoint);

bool TeamTypeClass::Write_INI(CCINIClass &ini) const
{
    if (AbstractTypeClass::Write_INI(ini)) {
        int house = -1;
        if (Owner != 0) {
            house = Owner->Type->House;
        }
        ini.Put_Int(ID, "VeteranLevel", VeteranLevel);
        ini.Put_Bool(ID, "Loadable", Loadable);
        ini.Put_Bool(ID, "Full", Full);
        ini.Put_Bool(ID, "Annoyance", Annoyance);
        ini.Put_Bool(ID, "GuardSlower", GuardSlower);
        ini.Put_HousesType(ID, "House", house);
        ini.Put_Bool(ID, "Recruiter", Recruiter);
        ini.Put_Bool(ID, "Autocreate", Autocreate);
        ini.Put_Bool(ID, "Prebuild", Prebuild);
        ini.Put_Bool(ID, "Reinforce", Reinforce);
        ini.Put_Bool(ID, "Droppod", Droppod);
        ini.Put_Bool(ID, "UseTransportOrigin", UseTransportOrigin);
        ini.Put_Bool(ID, "Whiner", Whiner);
        ini.Put_Bool(ID, "LooseRecruit", LooseRecruit);
        ini.Put_Bool(ID, "Aggressive", Aggressive);
        ini.Put_Bool(ID, "Suicide", Suicide);
        ini.Put_Int(ID, "Priority", Priority);
        ini.Put_Int(ID, "Max", Max);
        ini.Put_Int(ID, "TechLevel", TechLevel);
        ini.Put_Int(ID, "Group", Group);
        ini.Put_Bool(ID, "OnTransOnly", OnTransOnly);
        ini.Put_Bool(ID, "AvoidThreats", AvoidThreats);
        ini.Put_Bool(ID, "IonImmune", IonImmune);
        ini.Put_Bool(ID, "TransportsReturnOnUnload", TransportsReturnOnUnload);
        ini.Put_Bool(ID, "AreTeamMembersRecruitable", AreTeamMembersRecruitable);
        ini.Put_Bool(ID, "IsBaseDefense", IsBaseDefense);
        ini.Put_Bool(ID, "OnlyTargetHouseEnemy", OnlyTargetHouseEnemy);
        if (Tag != 0) {
            ini.Put_String(ID, "Tag", Tag->ID);
        }
        ini.Put_String(ID, "Waypoint", Waypoint_Name_7249f0(Waypoint));
        ini.Put_String(ID, "TransportWaypoint", Waypoint_Name_7249f0(TransportWaypoint));
        if (Script != 0) {
            ini.Put_String(ID, "Script", Script->ID);
        }
        if (TaskForce != 0) {
            ini.Put_String(ID, "TaskForce", TaskForce->ID);
        }
        return true;
    }
    return false;
}


// int TeamTypeClass::Get_Group() const (0x006BFA60)
// Group, or the task force's group when Group is -1.

int TeamTypeClass::Get_Group() const
{
    int group = Group;
    if (group != -2 && group == -1 && TaskForce != 0) {
        group = TaskForce->Group;
    }
    return group;
}


// Cell TeamTypeClass::Waypoint_Location() const (0x006BFA90)
// The cell of the team's waypoint; the "no cell" value without one.

// The "no cell" position (0x00AC00C8; hypothesis).

Cell TeamTypeClass::Waypoint_Location() const
{
    if (Waypoint == -1) {
        return unknown_geometry_k;
    }
    return unknown_id_source_a3d290->Get_Waypoint_Location(Waypoint);
}


// Cell TeamTypeClass::Transport_Waypoint_Location() const (0x006BFAD0)
// The cell of the team's transport waypoint; the "no cell" value without one.

// The "no cell" position (0x00AC00C8; hypothesis).

Cell TeamTypeClass::Transport_Waypoint_Location() const
{
    if (TransportWaypoint == -1) {
        return unknown_geometry_k;
    }
    return unknown_id_source_a3d290->Get_Waypoint_Location(TransportWaypoint);
}


// TeamTypeClass *TeamTypeClass::Find_Or_Make(char const *name) (0x006BFB10)
// The team type with this ID, created when missing; null for "<none>" and
// "none".

TeamTypeClass *__fastcall TeamTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < TeamTypes.Count(); index++) {
        if (_stricmp(TeamTypes[index]->ID, name) == 0) {
            return TeamTypes[index];
        }
    }
    return new TeamTypeClass(name);
}


// void TeamTypeClass::Read_Scenario_INI(CCINIClass &ini, int is_global) (0x006BFBA0)
// Creates (or finds) every team type listed in [TeamTypes] and reads its
// own section.

void __fastcall TeamTypeClass::Read_Scenario_INI(CCINIClass &ini, int is_global)
{
    char buffer[32];

    int count = ini.Entry_Count("TeamTypes");
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry("TeamTypes", index);
        ini.Get_String("TeamTypes", entry, "", buffer, sizeof(buffer));
        TeamTypeClass *teamtype = Find_Or_Make(buffer);
        teamtype->Read_INI(ini);
        teamtype->IsGlobal = is_global;
    }
}


// void TeamTypeClass::Write_Scenario_INI(CCINIClass &ini, int is_global) (0x006BFCA0)
// Clears the old [TeamTypes] list and its sections, then lists and writes
// every team type whose IsGlobal matches.

void __fastcall TeamTypeClass::Write_Scenario_INI(CCINIClass &ini, int is_global)
{
    char buffer[32];

    int count = ini.Entry_Count("TeamTypes");
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry("TeamTypes", index);
        ini.Get_String("TeamTypes", entry, "", buffer, sizeof(buffer));
        ini.Clear(buffer);
    }
    ini.Clear("TeamTypes");

    int written = 0;
    for (int tt = 0; tt < TeamTypes.Count(); tt++) {
        if (TeamTypes[tt]->IsGlobal == is_global) {
            sprintf(buffer, "%d", written++);
            ini.Put_String("TeamTypes", buffer, TeamTypes[tt]->ID);
            TeamTypes[tt]->Write_INI(ini);
        }
    }
}


// HRESULT __stdcall TeamTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x006BFD80)

HRESULT __stdcall TeamTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall TeamTypeClass::Load(IStream *stream) (0x006BFDA0)
// Restores the type in place and swizzles its owner, tag, script and task
// force.

HRESULT __stdcall TeamTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TeamTypeClass(NoInitClass());
        Swizzler.Swizzle((void **)&Owner);
        Swizzler.Swizzle((void **)&Tag);
        Swizzler.Swizzle((void **)&Script);
        Swizzler.Swizzle((void **)&TaskForce);
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall TeamTypeClass::GetClassID(GUID *clsid) (0x006BFE30)

HRESULT __stdcall TeamTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TeamTypeClass;
    return S_OK;
}


// void TeamTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x006BFE70)

void TeamTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)Group);
    crc(Recruiter);
    crc(Autocreate);
    crc(Prebuild);
    crc(Reinforce);
    crc(Whiner);
    crc(Aggressive);
    crc(LooseRecruit);
    crc(Suicide);
    crc(Droppod);
    crc(UseTransportOrigin);
    crc((long)Priority);
    crc((long)Max);
    crc((long)unknown_bc);
    if (Owner != 0) {
        crc((long)((AbstractClass *)Owner)->Kind_Of());
    }
    if (Owner != 0) {
        crc(((AbstractClass *)Owner)->Fetch_ID());
    }
    crc((long)TechLevel);
    if (Tag != 0) {
        crc(((AbstractClass *)Tag)->Fetch_ID());
    }
    crc((long)Waypoint);
    crc((long)TransportWaypoint);
    crc((long)unknown_d4);
    if (Script != 0) {
        crc(((AbstractClass *)Script)->Fetch_ID());
    }
    if (TaskForce != 0) {
        crc(((AbstractClass *)TaskForce)->Fetch_ID());
    }
}


// bool TeamTypeClass::unknown_func_6c0010(FootClass *foot, HouseClass *house) (0x006C0010)
// Whether the foot may join a team of this type (HouseClass::Can_Build_Team): it belongs to house, is alive
// and not in radio contact, in no team or in one of a lower priority, in an idle mission when it has one,
// recruitable by the team type's Autocreate flag (+0x379/+0x37A), and not a unit with a weapon and no
// +0x254 state (kind 2).

bool TeamTypeClass::unknown_func_6c0010(FootClass *foot, HouseClass *house)
{
    if ((foot->Team == 0 || this != foot->Team->Class) && foot->Strength > 0 && foot->unknown_7d && !foot->unknown_76
        && !foot->HasAnyLink() && foot->House == house) {
        if (foot->Get_Mission() != -1) {
            if (!MissionClass::Is_Recruitable_Mission(foot->Get_Mission())) {
                return false;
            }
        }
        if ((foot->unknown_379 || Autocreate) && (foot->unknown_37a || !Autocreate)
            && (foot->Team == 0 || foot->Team->Class->Priority < Priority)) {
            if (foot->Kind_Of() == 2 && foot->unknown_slot231(0)->WeaponType != 0 && foot->unknown_254 == 0) {
                return false;
            }
            return true;
        }
    }
    return false;
}


// void TeamTypeClass::unknown_func_6c0110(int value) const (0x006C0110)
// Passes value to TeamClass::unknown_func_6bd990 of every team of this type.

void TeamTypeClass::unknown_func_6c0110(int value) const
{
    for (int index = 0; index < Teams.Count(); index++) {
        if (Teams[index]->Class == this) {
            Teams[index]->unknown_func_6bd990(value);
        }
    }
}


// TeamClass *TeamTypeClass::Find_Team() const (0x006C0150)
// The first team of this type, or null.

TeamClass *TeamTypeClass::Find_Team() const
{
    for (int index = 0; index < Teams.Count(); index++) {
        TeamClass *team = Teams[index];
        if (team->Class == this) {
            return team;
        }
    }
    return 0;
}


// void TeamTypeClass::unknown_func_6c0180() (0x006C0180)
// Recomputes +0xE4 (the movement zones of the task force's types combined
// through 0x0056A900, starting at 9), +0xE8 (false once a naval type
// without passengers is found, or for base defense) and +0xE9 (a naval
// type carries passengers).

void TeamTypeClass::unknown_func_6c0180()
{
    unknown_e8 = true;
    unknown_e9 = false;
    unknown_e4 = 9;
    for (int index = 0; index < TaskForce->CountEntries; index++) {
        TechnoTypeClass *type = TaskForce->Entries[index].Type;
        if (type->Naval) {
            if (type->Passengers == 0) {
                unknown_e8 = false;
            } else {
                unknown_e9 = true;
            }
        }
        unknown_e4 = Unknown_0056A900(type->MovementZone, unknown_e4);
    }
    if (IsBaseDefense) {
        unknown_e8 = false;
    }
}


// void Unknown_006c0220() (0x006C0220)
// Recomputes the task-force flags of every team type (TeamTypeClass 0x006C0180), after the scenario's team types are read.

void __fastcall Unknown_006c0220()
{
    for (int index = 0; index < TeamTypes.Count(); index++) {
        TeamTypes[index]->unknown_func_6c0180();
    }
}


// TeamTypeClass::Kind_Of (0x006C0250)
// Compiler-emitted from shared/team_type.hpp in this unit; no source of its own.


// TeamTypeClass::Fetch_Heap_ID (0x006C0270)
// Compiler-emitted from shared/team_type.hpp in this unit; no source of its own.


// TeamTypeClass::scalar_deleting_destructor (0x006C0280)
// Compiler-emitted from shared/team_type.hpp in this unit; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>>::Fetch (0x006C03C0).
// VectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>>::operator== (0x006C0440).
// VectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>>::Resize (0x006C04A0).
// VectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>>::Clear (0x006C05B0).


// 0x006c0780: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void FUN_006c0780(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006c0780 {

void  FUN_006c0780(void)
{
    return;
}

}  // namespace seed_006c0780


// 0x006c07f0: function not yet identified
//
// Owner: DynamicVectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>>, virtual slot 4 (vtable +0x10) (from RTTI/vtables).
// Body: 54 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int __thiscall FUN_006c07f0(int param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006c07f0 {

struct Owner_006c07f0 { int ID_006c07f0(int *object); };

int Owner_006c07f0::ID_006c07f0(int *object)
{
    int vector_address = (int)this;
    int index;
    int *element_words;

    index = (int)(0);
    if (0 < *(int *)(vector_address + 0x10)) {
        element_words = (int *)(*(int **)(vector_address + 4));
        do {
            if ((*element_words == *object) && (element_words[1] == object[1])) {
                return index;
            }
            index = (int)(index + 1);
            element_words = (int *)(element_words + 2);
        } while (index < *(int *)(vector_address + 0x10));
    }
    return -1;
}

}  // namespace seed_006c07f0


// DynamicVectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>> scalar deleting destructor (0x006C08D0; vtable 0x007ACA80 slot 0):
// compiler generated in this unit with the vtable TeamTypeClass::Suggested_New_Teams (0x006BED50) constructs (shared/vector_class.hpp); no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<TeamTypeClass const *>::operator= (0x006C0930).
// Called at 0x006BF12F, where a VectorClass<TeamTypeClass const *> (vtable
// 0x007A2EE4, named by its RTTI) is copy-constructed inline.
