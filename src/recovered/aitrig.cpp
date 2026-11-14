// aitrig.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/aitrig.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ai_trigger_type.hpp"
#include "shared/house.hpp"
#include "shared/object.hpp"
#include "shared/side.hpp"
#include "shared/debug_printf.hpp"
#include "shared/super.hpp"
#include "shared/super_weapon_type.hpp"
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#include "shared/map_view.hpp"


// ??0AITriggerTypeClass@@QAE@PBD@Z (0x0041C5C0)
// Defaults, listed in the AI trigger type list.

AITriggerTypeClass::AITriggerTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    ConditionType(-1),
    unknown_9c(0),
    unknown_a0(0),
    unknown_a4(false),
    unknown_a8(-1),
    unknown_ac(0),
    unknown_b0(0),
    Weight(1.0),
    MinWeight(1.0),
    MaxWeight(1.0),
    unknown_d0(false),
    unknown_d1(false),
    unknown_d2(true),
    unknown_d3(true),
    unknown_d4(true),
    unknown_d8(0),
    unknown_dc(0),
    unknown_e0(0),
    unknown_104(0),
    unknown_108(0)
{
    memset(Comparator, 0, sizeof(Comparator));
    AITriggerTypes.Add(this);
}


// ??1AITriggerTypeClass@@UAE@XZ (0x0041C6F0)
// Unlists the type.

AITriggerTypeClass::~AITriggerTypeClass()
{
    AITriggerTypes.Delete(this);
}


// ?GetClassID@AITriggerTypeClass@@UAGJPAUGUID@@@Z (0x0041C770)
// The class identifier.

HRESULT __stdcall AITriggerTypeClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_AITriggerTypeClass;
    return S_OK;
}


// ?Load@AITriggerTypeClass@@UAGJPAUIStream@@@Z (0x0041C7B0)
// AbstractClass load, vtables, pointer fixups.

HRESULT __stdcall AITriggerTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) AITriggerTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&unknown_d8);
    Swizzler.Swizzle((void **)&unknown_dc);
    Swizzler.Swizzle((void **)&unknown_e0);
    return S_OK;
}


// ?Save@AITriggerTypeClass@@UAGJPAUIStream@@H@Z (0x0041C830)
// Only the AbstractClass part.

HRESULT __stdcall AITriggerTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// ?Compute_CRC@AITriggerTypeClass@@UBEXAAVWWCRCEngine@@@Z (0x0041C850)
// AbstractTypeClass part and the trigger fields; the comparator only for condition types 0, 1, 4 and 7.

void AITriggerTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)ConditionType);
    crc((long)unknown_9c);
    crc((long)unknown_a0);
    crc((long)unknown_a8);
    crc((long)unknown_b0);
    switch (ConditionType) {
    case 0:
    case 1:
    case 4:
    case 7:
        crc(((long const *)Comparator)[0]);
        crc(((long const *)Comparator)[1]);
        break;
    }
    crc(Weight);
    crc(MinWeight);
    crc(MaxWeight);
    crc(unknown_d0);
    crc(unknown_d2);
    crc(unknown_d3);
    crc(unknown_d4);
    crc((long)unknown_ac);
    crc(unknown_d1);
}


// bool AITriggerTypeClass::Is_Ready(HouseClass *house, HouseClass *enemy, bool unknown_arg3) const (0x0041C990)
// Whether the trigger may fire for the house against the enemy: team
// availability, the minimum the rules require, global/enabled state,
// difficulty flags, owner side and house, tech level, the condition, the
// teams' zones, buildability and team limits.

bool AITriggerTypeClass::Is_Ready(HouseClass *house, HouseClass *enemy, bool unknown_arg3) const
{
    bool teams_ok = unknown_dc != 0 && unknown_dc->IsBaseDefense && (unknown_e0 == 0 || unknown_e0->IsBaseDefense);
    if (unknown_dc == 0) {
        return false;
    }
    if (enemy == 0 || (Rules->unknown_1437 && house->unknown_555c < Rules->MinimumAIDefensiveTeams[house->unknown_c8])) {
        if (!teams_ok) {
            return false;
        }
    }
    if (teams_ok && unknown_arg3) {
        return false;
    }
    if (unknown_9c == 1 && unknown_id_source_a3d290->unknown_2238 == true) {
        return false;
    }
    if (!unknown_a4) {
        return false;
    }
    if (unknown_game_mode_a3d298 != 0 && !unknown_d0) {
        return false;
    }
    if (unknown_game_mode_a3d298 == 0) {
        if (unknown_id_source_a3d290->Difficulty == 0) {
            if (!unknown_d2) {
                return false;
            }
        } else if (unknown_id_source_a3d290->Difficulty == 1) {
            if (!unknown_d3) {
                return false;
            }
        } else if (unknown_id_source_a3d290->Difficulty == 2) {
            if (!unknown_d4) {
                return false;
            }
        }
    } else {
        if (house->unknown_c8 == 0) {
            if (!unknown_d4) {
                return false;
            }
        } else if (house->unknown_c8 == 1) {
            if (!unknown_d3) {
                return false;
            }
        } else if (house->unknown_c8 == 2) {
            if (!unknown_d2) {
                return false;
            }
        }
    }
    if (unknown_a0 == 0) {
        return false;
    }
    if (unknown_a0 != 2 && unknown_a0 == 1 && house->Type->House != unknown_a8) {
        return false;
    }
    if (unknown_ac == 1) {
        if (house->unknown_130 != 0) {
            return false;
        }
    } else if (unknown_ac == 2) {
        if (house->unknown_130 != 1) {
            return false;
        }
    }
    if (unknown_b0 > house->unknown_11c) {
        return false;
    }

    bool result;
    if (enemy != 0) {
        switch (ConditionType) {
        case -1:
            result = true;
            break;
        case 0:
            result = Enemy_Owns_Compare(house, enemy);
            break;
        case 1:
            result = House_Owns_Compare(house, enemy);
            break;
        case 2:
            result = Enemy_Power_Below_100(house, enemy);
            break;
        case 3:
            result = Enemy_Power_Below_0(house, enemy);
            break;
        case 4:
            result = Enemy_Money_Compare(house, enemy);
            break;
        case 5:
            result = Iron_Curtain_Ready(house, enemy);
            break;
        case 6:
            result = Chronosphere_Ready(house, enemy);
            break;
        case 7:
            result = Neutral_Owns_Compare(house, enemy);
            break;
        default:
            result = false;
            break;
        }
    } else if (ConditionType == 1) {
        result = House_Owns_Compare(house, 0);
    } else {
        result = ConditionType == -1;
    }
    if (!result) {
        return false;
    }

    if (!Team_Zone_Check(unknown_dc, house, enemy)) {
        return false;
    }
    if (unknown_e0 != 0 && !Team_Zone_Check(unknown_e0, house, enemy)) {
        return false;
    }
    if (!house->Can_Build_Team(unknown_dc)) {
        return false;
    }
    if (unknown_e0 != 0 && !house->Can_Build_Team(unknown_e0)) {
        return false;
    }
    if (unknown_dc != 0 && unknown_dc->Max >= 0 && house->Team_Count(unknown_dc) >= unknown_dc->Max) {
        return false;
    }
    if (unknown_e0 != 0 && unknown_e0->Max >= 0 && house->Team_Count(unknown_e0) >= unknown_e0->Max) {
        return false;
    }
    return true;
}


// bool AITriggerTypeClass::Enemy_Owns_Compare(HouseClass *house, HouseClass *enemy) const (0x0041CD50)
// Compares how many objects of the condition type (unknown_d8) the enemy owns
// with the comparator's value using its operator (0 <, 1 <=, 2 ==, 3 >=,
// 4 >, 5 !=). The count comes from the enemy's per-kind counters: building
// (kind 7), unit (0x28), infantry (0x10) and aircraft (3) types.

// How many objects of the condition type the house owns (its per-kind counters).
inline int AITriggerTypeClass::Owned_Count(HouseClass *house) const
{
    AbstractTypeClass *type = unknown_d8;
    if (type != 0) {
        int index = type->Fetch_Heap_ID();
        int count = 0;
        switch (type->Kind_Of()) {
        case 7:
            count = house->unknown_5460.unknown_func_491d10(index);
            break;
        case 0x28:
            count = house->unknown_5470.unknown_func_491d10(index);
            break;
        case 0x10:
            count = house->unknown_5480.unknown_func_491d10(index);
            break;
        case 3:
            count = house->unknown_5490.unknown_func_491d10(index);
            break;
        }
        return count;
    }
    return 0;
}

bool AITriggerTypeClass::Enemy_Owns_Compare(HouseClass *house, HouseClass *enemy) const
{
    bool result = false;
    if (enemy != 0) {
        int count = Owned_Count(enemy);
        switch (((int const *)Comparator)[1]) {
        case 0:
            result = count < ((int const *)Comparator)[0];
            break;
        case 1:
            result = count <= ((int const *)Comparator)[0];
            break;
        case 2:
            result = count == ((int const *)Comparator)[0];
            break;
        case 3:
            result = count >= ((int const *)Comparator)[0];
            break;
        case 4:
            result = count > ((int const *)Comparator)[0];
            break;
        case 5:
            result = count != ((int const *)Comparator)[0];
            break;
        }
    }
    return result;
}


// bool AITriggerTypeClass::Neutral_Owns_Compare(HouseClass *house, HouseClass *enemy) const (0x0041CEF0)
// Finds the neutral house (the last house of the "Civilian" side) and
// compares how many objects of the condition type it owns with the
// comparator; false without a neutral house.


bool AITriggerTypeClass::Neutral_Owns_Compare(HouseClass *house, HouseClass *enemy) const
{
    bool result = false;
    HouseClass *neutral = 0;
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *candidate = Houses[index];
        if (candidate->Type->Side == SideClass::Find_Index("Civilian")) {
            neutral = candidate;
        }
    }
    if (neutral != 0) {
        Debug_Printf("Neutral house is %s", (char const *)neutral->Type->ID);
        int count = Owned_Count(neutral);
        switch (((int const *)Comparator)[1]) {
        case 0:
            result = count < ((int const *)Comparator)[0];
            break;
        case 1:
            result = count <= ((int const *)Comparator)[0];
            break;
        case 2:
            result = count == ((int const *)Comparator)[0];
            break;
        case 3:
            result = count >= ((int const *)Comparator)[0];
            break;
        case 4:
            result = count > ((int const *)Comparator)[0];
            break;
        case 5:
            result = count != ((int const *)Comparator)[0];
            break;
        }
    }
    return result;
}


// bool AITriggerTypeClass::House_Owns_Compare(HouseClass *house, HouseClass *enemy) const (0x0041D0F0)
// Compares how many objects of the condition type the house itself owns
// with the comparator (Owned_Count is defined with 0x0041CD50 in this unit).

bool AITriggerTypeClass::House_Owns_Compare(HouseClass *house, HouseClass *enemy) const
{
    bool result = false;
    if (house != 0) {
        int count = Owned_Count(house);
        switch (((int const *)Comparator)[1]) {
        case 0:
            result = count < ((int const *)Comparator)[0];
            break;
        case 1:
            result = count <= ((int const *)Comparator)[0];
            break;
        case 2:
            result = count == ((int const *)Comparator)[0];
            break;
        case 3:
            result = count >= ((int const *)Comparator)[0];
            break;
        case 4:
            result = count > ((int const *)Comparator)[0];
            break;
        case 5:
            result = count != ((int const *)Comparator)[0];
            break;
        }
    }
    return result;
}


// bool AITriggerTypeClass::Enemy_Power_Below_100(HouseClass *house, HouseClass *enemy) const (0x0041D290)
// The enemy's power surplus (output minus drain) is below 100 (a condition the trigger check 0x0041C990 expands in place).

bool AITriggerTypeClass::Enemy_Power_Below_100(HouseClass *house, HouseClass *enemy) const
{
    bool result = false;
    if (enemy != 0) {
        result = enemy->Power_Output() - enemy->Power_Drain() < 100.0;
    }
    return result;
}


// bool AITriggerTypeClass::Enemy_Power_Below_0(HouseClass *house, HouseClass *enemy) const (0x0041D2E0)
// Extent: the inventory splits it at 0x0041D31D; a linear sweep reaches the
// ret 8 at 0x0041D31E, followed by padding to 0x0041D330.
// The enemy's power surplus is negative (a condition the trigger check 0x0041C990 expands in place).

bool AITriggerTypeClass::Enemy_Power_Below_0(HouseClass *house, HouseClass *enemy) const
{
    bool result = false;
    if (enemy != 0) {
        result = enemy->Power_Output() - enemy->Power_Drain() < 0.0;
    }
    return result;
}


// bool AITriggerTypeClass::Iron_Curtain_Ready(HouseClass *house, HouseClass *enemy) const (0x0041D330)
// Whether the house's first super weapon of type 1 is present and has
// recharged far enough: the remaining fraction of its recharge time is at
// most 1 - the rules fraction (+0xB80).

bool AITriggerTypeClass::Iron_Curtain_Ready(HouseClass *house, HouseClass *enemy) const
{
    if (house != 0) {
        for (int index = 0; index < house->Supers.Count(); index++) {
            if (house->Supers[index]->Type->Type == 1) {
                SuperClass *super = house->Supers[index];
                if (super != 0 && super->unknown_0x55) {
                    double remaining = super->RechargeTimer.Value();
                    double total = super->Type->RechargeTime;
                    if (remaining / total > 1.0f - Rules->unknown_b80) {
                        return false;
                    }
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}


// bool AITriggerTypeClass::Chronosphere_Ready(HouseClass *house, HouseClass *enemy) const (0x0041D3E0)
// Whether the house's first super weapon of type 3 is present and has
// recharged far enough: the remaining fraction of its recharge time is at
// most 1 - the rules fraction (+0xB80).

bool AITriggerTypeClass::Chronosphere_Ready(HouseClass *house, HouseClass *enemy) const
{
    if (house != 0) {
        for (int index = 0; index < house->Supers.Count(); index++) {
            if (house->Supers[index]->Type->Type == 3) {
                SuperClass *super = house->Supers[index];
                if (super != 0 && super->unknown_0x55) {
                    double remaining = super->RechargeTimer.Value();
                    double total = super->Type->RechargeTime;
                    if (remaining / total > 1.0f - Rules->unknown_b80) {
                        return false;
                    }
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}


// bool AITriggerTypeClass::Enemy_Money_Compare(HouseClass *house, HouseClass *enemy) const (0x0041D490)
// Compares the enemy's available money with the comparator's value using its
// operator (0 <, 1 <=, 2 ==, 3 >=, 4 >, 5 !=).

bool AITriggerTypeClass::Enemy_Money_Compare(HouseClass *house, HouseClass *enemy) const
{
    bool result = false;
    if (enemy != 0) {
        IHouse *ihouse = enemy;
        long money = ihouse->Available_Money();
        switch (((int const *)Comparator)[1]) {
        case 0:
            result = money < ((int const *)Comparator)[0];
            break;
        case 1:
            result = money <= ((int const *)Comparator)[0];
            break;
        case 2:
            result = money == ((int const *)Comparator)[0];
            break;
        case 3:
            result = money >= ((int const *)Comparator)[0];
            break;
        case 4:
            result = money > ((int const *)Comparator)[0];
            break;
        case 5:
            result = money != ((int const *)Comparator)[0];
            break;
        }
    }
    return result;
}


// void __fastcall AITriggerTypeClass::Read_Scenario_INI(CCINIClass &ini, int is_global) (0x0041D540)
// Makes (or finds) and reads the AI trigger type of every entry, marks it
// global or not (enabled outright when global); for the scenario list the
// enable section then switches the existing types on or off (all on when the
// game mode is not 0).

// The named AI trigger type, or 0 (no type is made).
static inline AITriggerTypeClass *Find_AI_Trigger(char const *name)
{
    for (int index = 0; index < AITriggerTypes.Count(); index++) {
        if (!_stricmp(AITriggerTypes[index]->ID, name)) {
            return AITriggerTypes[index];
        }
    }
    return 0;
}

void __fastcall AITriggerTypeClass::Read_Scenario_INI(CCINIClass &ini, int is_global)
{
    int count = ini.Entry_Count(unknown_section_79b748);
    int index;
    for (index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_section_79b748, index);
        AITriggerTypeClass *type = Find_Or_Make(entry);
        if (type != 0) {
            type->Read_INI(ini);
            type->unknown_9c = is_global;
        }
        if (is_global == 1) {
            type->unknown_a4 = true;
        }
    }

    if (is_global == 0) {
        count = ini.Entry_Count(unknown_section_79b74c);
        for (index = 0; index < count; index++) {
            char const *entry = ini.Get_Entry(unknown_section_79b74c, index);
            AITriggerTypeClass *type = Find_AI_Trigger(entry);
            if (type != 0) {
                if (ini.Get_Bool(unknown_section_79b74c, entry, false) || unknown_game_mode_a3d298 != 0) {
                    type->unknown_a4 = true;
                } else {
                    type->unknown_a4 = false;
                }
            }
        }
    }
}


// void __fastcall AITriggerTypeClass::Write_Scenario_INI(CCINIClass &ini, int is_global) (0x0041D6F0)
// Clears the AI trigger section (and every section its entries name), writes
// the types whose global flag (unknown_9c) matches, and for the scenario
// (not global) list rewrites the enable section from each type's flag.

void __fastcall AITriggerTypeClass::Write_Scenario_INI(CCINIClass &ini, int is_global)
{
    char buffer[32];
    int count = ini.Entry_Count(unknown_section_79b748);
    int index;
    for (index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_section_79b748, index);
        ini.Get_String(unknown_section_79b748, entry, unknown_string_83c55c, buffer, sizeof(buffer));
        ini.Clear(buffer, 0);
    }
    ini.Clear(unknown_section_79b748, 0);

    for (index = 0; index < AITriggerTypes.Count(); index++) {
        AITriggerTypeClass *type = AITriggerTypes[index];
        if (type->unknown_9c == is_global) {
            type->Write_INI(ini);
        }
    }

    if (!is_global) {
        ini.Clear(unknown_section_79b74c, 0);
        for (index = 0; index < AITriggerTypes.Count(); index++) {
            AITriggerTypeClass *type = AITriggerTypes[index];
            ini.Put_Bool(unknown_section_79b74c, type->ID, type->unknown_a4);
        }
    }
}


// bool AITriggerTypeClass::Read_INI(CCINIClass &ini) (0x0041D7E0, slot 25)
// Parses this type's comma-separated line under [AITriggerTypes] in the
// order Write_INI writes it: name, team 1, house ("<all>", "<none>" or a
// house type), an ignored field, condition type, the object type
// (infantry, unit, aircraft or building), the comparator as hex digit
// pairs, the three weights (whole numbers), flags and team 2. Missing
// fields up to the comparator fail the read; later ones keep their value.
// unknown_b0 ends as the highest tech level of either team's task force.


bool AITriggerTypeClass::Read_INI(CCINIClass &ini)
{
    char buffer[0x200];
    char name[0x18];

    if (ini.Get_String(unknown_section_79b748, ID, "", buffer, sizeof(buffer))) {
        char *token = strtok(buffer, ",");
        if (token != 0) {
            FixedString<49> name49(token);
            Name = name49;
            token = strtok(0, ",");
            if (token != 0) {
                strncpy(name, token, sizeof(name));
                name[sizeof(name) - 1] = '\0';
                strtrim(name);
                unknown_dc = 0;
                if (_stricmp(name, unknown_none_name_7ceacc) != 0) {
                    unknown_dc = TeamTypeClass::Find_By_ID_Or_Name(name);
                }
                token = strtok(0, ",");
                if (token != 0) {
                    strncpy(name, token, sizeof(name));
                    name[sizeof(name) - 1] = '\0';
                    strtrim(name);
                    unknown_a0 = 0;
                    unknown_a8 = -1;
                    if (_stricmp(name, unknown_all_name_7cead0) == 0) {
                        unknown_a0 = 2;
                    } else if (_stricmp(name, unknown_none_name_7ceacc) != 0) {
                        unknown_a8 = HouseTypeClass::Find_Index(name);
                        if (unknown_a8 != -1) {
                            unknown_a0 = 1;
                        }
                    }
                    if (strtok(0, ",") != 0) {
                        unknown_b0 = 0;
                        token = strtok(0, ",");
                        if (token != 0) {
                            ConditionType = atoi(token);
                            token = strtok(0, ",");
                            if (token != 0) {
                                strncpy(name, token, sizeof(name));
                                name[sizeof(name) - 1] = '\0';
                                strtrim(name);
                                AbstractTypeClass *type = 0;
                                int index = InfantryTypeClass::From_Name(name);
                                if (index != -1) {
                                    type = (AbstractTypeClass *)InfantryTypes[index];
                                } else {
                                    index = UnitTypeClass::From_Name(name);
                                    if (index != -1) {
                                        type = (AbstractTypeClass *)UnitTypes[index];
                                    } else {
                                        index = AircraftTypeClass::From_Name(name);
                                        if (index != -1) {
                                            type = (AbstractTypeClass *)AircraftTypes[index];
                                        } else {
                                            index = BuildingType_From_Name_4583d0(name);
                                            if (index != -1) {
                                                type = (AbstractTypeClass *)BuildingTypes[index];
                                            }
                                        }
                                    }
                                }
                                unknown_d8 = type;

                                token = strtok(0, ",");
                                if (token != 0) {
                                    {
                                    char *digit = token;
                                    char digits[3] = "00";
                                    char *end = 0;
                                    unsigned count = 0;
                                    while (*digit != '\0') {
                                        while (isspace(*digit)) {
                                            digit++;
                                        }
                                        digits[0] = *digit++;
                                        if (*digit != '\0') {
                                            digits[1] = *digit++;
                                        } else {
                                            digits[1] = '\0';
                                        }
                                        if (count >= sizeof(Comparator)) break;
                                        Comparator[count++] = (unsigned char)strtol(digits, &end, 16);
                                    }
                                    }

                                    token = strtok(0, ",");
                                    if (token != 0) Weight = (unsigned)atof(token);
                                    token = strtok(0, ",");
                                    if (token != 0) MinWeight = (unsigned)atof(token);
                                    token = strtok(0, ",");
                                    if (token != 0) MaxWeight = (unsigned)atof(token);
                                    token = strtok(0, ",");
                                    if (token != 0) unknown_d0 = atoi(token) != 0;
                                    strtok(0, ",");
                                    token = strtok(0, ",");
                                    if (token != 0) unknown_ac = atoi(token);
                                    token = strtok(0, ",");
                                    if (token != 0) unknown_d1 = atoi(token) != 0;

                                    token = strtok(0, ",");
                                    if (token != 0) {
                                        char team2[0x18];
                                        strncpy(team2, token, sizeof(team2));
                                        team2[sizeof(team2) - 1] = '\0';
                                        strtrim(team2);
                                        unknown_e0 = 0;
                                        if (_stricmp(team2, unknown_none_name_7ceacc) != 0) {
                                            unknown_e0 = TeamTypeClass::Find_By_ID_Or_Name(team2);
                                        }
                                    }
                                    token = strtok(0, ",");
                                    if (token != 0) unknown_d2 = atoi(token) != 0;
                                    token = strtok(0, ",");
                                    if (token != 0) unknown_d3 = atoi(token) != 0;
                                    token = strtok(0, ",");
                                    if (token != 0) unknown_d4 = atoi(token) != 0;

                                    if (unknown_dc != 0) {
                                        unknown_b0 = MAX(unknown_b0, unknown_dc->TaskForce->Max_Tech_Level());
                                    }
                                    if (unknown_e0 != 0) {
                                        unknown_b0 = MAX(unknown_b0, unknown_e0->TaskForce->Max_Tech_Level());
                                    }
                                    return true;
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


// bool AITriggerTypeClass::Write_INI(CCINIClass &ini) const (0x0041DD60)
// Writes this type's line under [AITriggerTypes] in the order Read_INI
// parses it: name, team 1, house ("<all>", "<none>" or the house type),
// tech level, condition type, the object type, the comparator as hex digit
// pairs, the three weights, flags (one field always 0) and team 2.



bool AITriggerTypeClass::Write_INI(CCINIClass &ini) const
{
    char const *team1 = unknown_none_name_7ceacc;
    char const *team2 = unknown_none_name_7ceacc;
    char const *house = unknown_none_name_7ceacc;
    char const *object = unknown_none_name_7ceacc;
    if (unknown_dc != 0) {
        team1 = unknown_dc->ID;
    }
    if (unknown_e0 != 0) {
        team2 = unknown_e0->ID;
    }
    switch (unknown_a0) {
    case 1:
        if (unknown_a8 != -1) {
            house = HouseTypes[unknown_a8]->ID;
        }
        break;
    case 2:
        house = unknown_all_name_7cead0;
        break;
    }
    if (unknown_d8 != 0) {
        object = unknown_d8->ID;
    }

    char comparator[0x200];
    char *digit;
    unsigned index;
    for (index = 0, digit = comparator; index < sizeof(Comparator); index++, digit += 2) {
        sprintf(digit, "%02x", Comparator[index]);
    }
    *digit = '\0';

    char buffer[0x200];
    sprintf(buffer, "%s,%s,%s,%d,%d,%s,%s,%lf,%lf,%lf,%d,%d,%d,%d,%s,%d,%d,%d",
            (char const *)Name, team1, house, unknown_b0, ConditionType, object, comparator,
            Weight, MinWeight, MaxWeight, unknown_d0 != 0, 0, unknown_ac, unknown_d1 != 0,
            team2, unknown_d2 != 0, unknown_d3 != 0, unknown_d4 != 0);
    return ini.Put_String(unknown_section_79b748, ID, buffer);
}


// AITriggerTypeClass *__fastcall AITriggerTypeClass::Find_Or_Make(char const *name) (0x0041DEF0)
// The named AI trigger type, created when missing; 0 for "<none>" and "none".

AITriggerTypeClass *__fastcall AITriggerTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name)) {
        return 0;
    }
    if (!_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < AITriggerTypes.Count(); index++) {
        if (!_stricmp(AITriggerTypes[index]->ID, name)) {
            return AITriggerTypes[index];
        }
    }
    return new AITriggerTypeClass(name);
}


// void AITriggerTypeClass::Set_Condition_Object(AbstractTypeClass *object) (0x0041DF80)

void AITriggerTypeClass::Set_Condition_Object(AbstractTypeClass *object)
{
    unknown_d8 = object;
}


// void AITriggerTypeClass::Set_Team1(TeamTypeClass *team) (0x0041DF90)

void AITriggerTypeClass::Set_Team1(TeamTypeClass *team)
{
    unknown_dc = team;
}


// void AITriggerTypeClass::Set_Team2(TeamTypeClass *team) (0x0041DFA0)

void AITriggerTypeClass::Set_Team2(TeamTypeClass *team)
{
    unknown_e0 = team;
}


// void AITriggerTypeClass::Register_Success() (0x0041DFB0)
// Raises the weight by the rules' success bonus plus a share for a better than
// even record (successes unknown_104 out of attempts unknown_108), clamped to
// MinWeight..MaxWeight, and counts the attempt and the success.

void AITriggerTypeClass::Register_Success()
{
    double bonus = 0.0;
    if (unknown_108 > 0) {
        bonus = (unknown_104 / (double)unknown_108 - 0.5) * unknown_108;
        if (bonus < 0.0) {
            bonus = 0.0;
        }
    }
    Weight = Rules->unknown_b8 + Weight + bonus;
    if (Weight < MinWeight) {
        Weight = MinWeight;
    }
    if (Weight > MaxWeight) {
        Weight = MaxWeight;
    }
    unknown_104++;
    unknown_108++;
}


// void AITriggerTypeClass::Register_Failure() (0x0041E070)
// Changes the weight by the rules' failure value plus a scaled share for a
// worse than even record, clamped to MinWeight..MaxWeight, and counts the
// attempt.

void AITriggerTypeClass::Register_Failure()
{
    double bonus = 0.0;
    if (unknown_108 > 0) {
        bonus = (unknown_104 / (double)unknown_108 - 0.5) * Rules->unknown_c8 * unknown_108;
        if (bonus > 0.0) {
            bonus = 0.0;
        }
    }
    Weight = Rules->unknown_c0 + Weight + bonus;
    if (Weight < MinWeight) {
        Weight = MinWeight;
    }
    if (Weight > MaxWeight) {
        Weight = MaxWeight;
    }
    unknown_108++;
}


// bool AITriggerTypeClass::Team_Zone_Check(TeamTypeClass *team, HouseClass *house, HouseClass *enemy) const (0x0041E130)
// For a team that cares (unknown_e8): whether the house's and the enemy's base
// centres lie in the same movement zone of the team's zone type
// (unknown_e4); when the team allows it (unknown_e9), zones that differ for
// its type but match for zone type 5 also pass. True otherwise.

bool AITriggerTypeClass::Team_Zone_Check(TeamTypeClass *team, HouseClass *house, HouseClass *enemy) const
{
    if (!team->unknown_e8 || enemy == 0) {
        return true;
    }
    Cell house_cell = Coord_Cell(house->BaseCenter);
    int zone_type = team->unknown_e4;
    Cell enemy_cell = Coord_Cell(enemy->BaseCenter);
    if (team->unknown_e9) {
        int enemy_zone = Map.Zone(enemy_cell, zone_type, false);
        if (Map.Zone(house_cell, zone_type, false) != enemy_zone) {
            enemy_zone = Map.Zone(enemy_cell, 5, false);
            if (Map.Zone(house_cell, 5, false) == enemy_zone) {
                return true;
            }
        }
        return false;
    }
    int enemy_zone = Map.Zone(enemy_cell, zone_type, false);
    bool same = Map.Zone(house_cell, zone_type, false) == enemy_zone;
    return same;
}


// ?Kind_Of@AITriggerTypeClass@@UBEHXZ (0x0041E270)
// RTTI kind 0x3B.

int AITriggerTypeClass::Kind_Of() const
{
    return 0x3B;
}


// ?Size_Of@AITriggerTypeClass@@UBEHXZ (0x0041E280)
// Object size.

int AITriggerTypeClass::Size_Of() const
{
    return sizeof(AITriggerTypeClass);
}


// ??_GAITriggerTypeClass@@UAEPAXI@Z (0x0041E290)
// Compiler generated (the destructor is expanded inline).
