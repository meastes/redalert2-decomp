// AITriggerTypeClass: an AI trigger type (AITrigger.cpp, 0x0041C5C0-
// 0x0041E316; RTTI vtables 0x0079B780 primary, 0x0079B764 at +4,
// 0x0079B75C, 0x0079B754). Layout from the constructor 0x0041C5C0, Load
// 0x0041C7B0 (pointer fixups at +0xD8, +0xDC, +0xE0) and Compute_CRC
// 0x0041C850; Size_Of 0x110, Kind_Of 0x3B. Every type is listed in
// AITriggerTypes (defined in the unit at 0x004D2BA0).
// Member names are placeholders.
#ifndef RA2_SHARED_AI_TRIGGER_TYPE_HPP
#define RA2_SHARED_AI_TRIGGER_TYPE_HPP

#include "shared/abstract_type.hpp"
#include "shared/ccini_class.hpp"
#include "shared/crt_memory.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#include "shared/vector_class.hpp"

#ifndef E_POINTER
#define E_POINTER ((HRESULT)0x80004003L)
#endif

extern "C" GUID const CLSID_AITriggerTypeClass;     // 0x007A1EA0
extern char const *NONE_STRING;        // 0x007CEACC -> "<none>"
extern char const *ALL_STRING;         // 0x007CEAD0 -> "<all>"
extern char const *unknown_section_79b748;          // 0x0079B748 -> "AITriggerTypes"
extern char const *unknown_section_79b74c;          // 0x0079B74C -> "AITriggerTypesEnable" (hypothesis)
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
extern "C" char *__cdecl strtok(char *string, char const *delimiters);
extern "C" int __cdecl atoi(char const *string);
extern "C" double __cdecl atof(char const *string);
extern "C" long __cdecl strtol(char const *string, char **end, int base);
extern "C" int __cdecl isspace(int c);
char *__fastcall strtrim(char *buffer);                     // 0x006EEF10

// The team, task force, techno and house types Read_INI and Write_INI look up.
#include "shared/team_type.hpp"
#include "shared/task_force.hpp"
#include "shared/infantry_type.hpp"
#include "shared/unit_type.hpp"
#include "shared/aircraft_type.hpp"
// BuildingTypeClass::From_Name (0x004583D0) by its address: building_type.hpp
// would add the static rectangle initializer this unit does not have, so only
// the list (building_type_list.hpp, which building_type.hpp also includes) is
// taken.
#include "shared/building_type_list.hpp"
int __fastcall BuildingType_From_Name_4583d0(char const *name);  // 0x004583D0
#include "shared/house_type.hpp"

class HouseClass;
class AITriggerTypeClass : public AbstractTypeClass {
public:
    AITriggerTypeClass(char const *name);                   // 0x0041C5C0
    AITriggerTypeClass(NoInitClass const &noinit) : AbstractTypeClass(noinit) {}
    virtual ~AITriggerTypeClass();                              // 0x0041C6F0

    virtual HRESULT __stdcall GetClassID(GUID *retval);        // 0x0041C770
    virtual HRESULT __stdcall Load(IStream *stream);            // 0x0041C7B0
    virtual HRESULT __stdcall Save(IStream *stream, BOOL cleardirty);  // 0x0041C830
    virtual int Kind_Of() const;                                // 0x0041E270
    virtual int Size_Of() const;                                // 0x0041E280
    virtual void Compute_CRC(WWCRCEngine &crc) const;           // 0x0041C850
    virtual bool Read_INI(CCINIClass &ini);                     // 0x0041D7E0
    virtual bool Write_INI(CCINIClass &ini) const;              // 0x0041DD60

    static AITriggerTypeClass *__fastcall Find_Or_Make(char const *name);  // 0x0041DEF0
    void Set_Condition_Object(AbstractTypeClass *obj);       // 0x0041DF80 (unknown_d8; name hypothesis)
    void Set_Team1(TeamTypeClass *team);                        // 0x0041DF90 (unknown_dc; name hypothesis)
    void Set_Team2(TeamTypeClass *team);                        // 0x0041DFA0 (unknown_e0; name hypothesis)
    bool Enemy_Power_Below_100(HouseClass *house, HouseClass *enemy) const;  // 0x0041D290 (name hypothesis)
    bool Enemy_Power_Below_0(HouseClass *house, HouseClass *enemy) const;    // 0x0041D2E0 (name hypothesis)
    bool Iron_Curtain_Ready(HouseClass *house, HouseClass *enemy) const;     // 0x0041D330 (super weapon type 1; name hypothesis)
    bool Chronosphere_Ready(HouseClass *house, HouseClass *enemy) const;     // 0x0041D3E0 (super weapon type 3; name hypothesis)
    bool Enemy_Money_Compare(HouseClass *house, HouseClass *enemy) const;    // 0x0041D490 (name hypothesis)
    bool Enemy_Owns_Compare(HouseClass *house, HouseClass *enemy) const;     // 0x0041CD50 (name hypothesis)
    bool House_Owns_Compare(HouseClass *house, HouseClass *enemy) const;     // 0x0041D0F0 (name hypothesis)
    bool Neutral_Owns_Compare(HouseClass *house, HouseClass *enemy) const;   // 0x0041CEF0 (name hypothesis)
    int Owned_Count(HouseClass *house) const;
    bool Is_Ready(HouseClass *house, HouseClass *enemy, bool unknown_arg3) const;  // 0x0041C990 (name hypothesis)
    bool Team_Zone_Check(TeamTypeClass *team, HouseClass *house, HouseClass *enemy) const;   // 0x0041E130 (name hypothesis)                   // (inline in the conditions) objects of type unknown_d8
    void Register_Success();                                    // 0x0041DFB0 (raises Weight; name hypothesis)
    void Register_Failure();                                    // 0x0041E070 (lowers Weight; name hypothesis)
    static void __fastcall Read_Scenario_INI(CCINIClass &ini, int is_global);     // 0x0041D540
    static void __fastcall Write_Scenario_INI(CCINIClass &ini, int is_global);    // 0x0041D6F0

    int ConditionType;              // +0x098 (-1; selects the comparator in Compute_CRC)
    int Scope;                 // +0x09C
    int TrigHouse;                 // +0x0A0
    bool IsEnabled;                // +0x0A4
    int unknown_a8;                 // +0x0A8 (-1)
    int MultiSide;                 // +0x0AC
    int TechLevelNeeded;                 // +0x0B0
    int unknown_b4;                 // +0x0B4 (not set by the constructor)
    double Weight;                  // +0x0B8 (1.0; hypothesis)
    double MinWeight;               // +0x0C0 (1.0)
    double MaxWeight;               // +0x0C8 (1.0)
    bool IsAvailableInSkirmish;                // +0x0D0
    bool IsForBaseDefense;                // +0x0D1
    bool IsEnabledInEasy;                // +0x0D2 (true)
    bool IsEnabledInMedium;                // +0x0D3 (true)
    bool IsEnabledInHard;                // +0x0D4 (true)
    AbstractTypeClass *ConditionObject;  // +0x0D8
    TeamTypeClass *TeamTypeOne;      // +0x0DC
    TeamTypeClass *TeamTypeTwo;      // +0x0E0
    unsigned char Comparator[0x20]; // +0x0E4 (written as hex; the first two dwords are CRC'd)
    int TimesSucceded;                // +0x104
    int TimesExecuted;                // +0x108
    int unknown_10c;                // +0x10C (size 0x110; not set by the constructor)
};

extern DynamicVectorClass<AITriggerTypeClass *> AITriggerTypes;   // 0x00A3D260

#endif
