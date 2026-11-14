// wdtcampaign.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtcampaign.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_ALL
#include "shared/wdt_campaign.hpp"
#include "shared/crt_string.hpp"


// Header-static dynamic initializers of the unit at 0x00725BF0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00725BF0-0x00725DD1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00725C10, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725C40, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725C60, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725C80, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725CA0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725CC0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725CE0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725D20, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725D50, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725D80, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725DB0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// Dynamic initializer (0x00725DC0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00725bf0.cpp; no source of its own.


// int __fastcall WorldDominationTour::Campaign::Saved_ID(Wstring const &name) (0x00725DE0)
// The CampaignID saved in WDTHist.INI under the campaign's name; -1 without
// the file (or the entry). No callers in the executable.

namespace WorldDominationTour {

int __fastcall Campaign::Saved_ID(Wstring const &name)
{
    CDFileClass file("WDTHist.INI");
    if (file.Is_Available()) {
        INIClass ini;
        ini.Load(file, false);
        file.Close();
        Wstring key("CampaignID");
        return ini.Get_Int(name, key, -1);
    }
    return -1;
}

}


// rc_ptr<Campaign> __fastcall WorldDominationTour::Campaign::Create(Wstring const &name, rc_ptr<CampaignProperties> const &properties) (0x00725EE0)
// A new campaign, returned counted; empty without properties.

namespace WorldDominationTour {

rc_ptr<Campaign> __fastcall Campaign::Create(Wstring const &name, rc_ptr<CampaignProperties> const &properties)
{
    if (!properties.Get()) {
        return rc_ptr<Campaign>();
    }
    return rc_ptr<Campaign>(new Campaign(name, properties));
}

}


// Construct the campaign's disputed territories and restore progress for the current cycle (0x00725F40).
namespace WorldDominationTour {
Campaign::Campaign(Wstring const &name, rc_ptr<CampaignProperties> const &properties)
    : Properties(properties), TheHistory(History::Create(properties->TheWDTState)),
      Conflicts(4), PlayerFaction(0), Name(name)
{
    memset(LastDay, 0, sizeof(LastDay));
    if (properties->TheWDTState.Get() != 0) {
        rc_ptr<State> state = TheHistory->Get_State(properties->Last_Conflict());
        unsigned index = properties->TheWDTState->unknown_20;
        while (index--) {
            if (state->Owner_Of(index) == 1) {
                DynamicVectorClass<rc_ptr<WDTTerritory> > &territories = properties->TheWDTState->Territories;
                rc_ptr<WDTTerritory> territory = index < (unsigned)territories.Count()
                    ? territories[index] : rc_ptr<WDTTerritory>();
                if (territory.Get() != 0) {
                    rc_ptr<Conflict> conflict = Conflict::Create(territory);
                    Conflicts.Add(conflict);
                }
            }
        }
    }
    CDFileClass file("WDTHist.INI");
    if (file.Is_Available()) {
        INIClass ini;
        ini.Load(file, false);
        file.Close();
        int saved_id;
        {
            Wstring key("CampaignID");
            saved_id = ini.Get_Int(Name, key, -1);
        }
        ID = saved_id < 0 ? 0 : saved_id;
        if (ID == properties->unknown_18()) {
            for (int faction = 0; faction < 4; faction++) {
                Wstring key = Wstring("LastDay") + Faction_Code(faction);
                int day = ini.Get_Int(Name, key, 0);
                LastDay[faction] = day < 0 ? 0 : day;
            }
            {
                Wstring key("PlayerFaction");
                PlayerFaction = ini.Get_Int(Name, key, PlayerFaction);
            }
        } else {
            memset(LastDay, 0, sizeof(LastDay));
        }
    }
}
}


// WorldDominationTour::Campaign::~Campaign() (0x007262E0)

namespace WorldDominationTour {

Campaign::~Campaign()
{
}

}


// bool WorldDominationTour::Campaign::ID_Differs() const (0x00726350)
// Whether the saved CampaignID differs from the properties' (unknown_18).

namespace WorldDominationTour {

bool Campaign::ID_Differs() const
{
    int value = Properties->unknown_18();
    return ID != value;
}

}


// bool WorldDominationTour::Campaign::Progress_Differs(int faction) const (0x00726370)
// Whether the saved CampaignID differs from the properties', or else the
// faction's saved LastDay from the properties' last conflict.

namespace WorldDominationTour {

bool Campaign::Progress_Differs(int faction) const
{
    int value = Properties->unknown_18();
    if (ID != value) {
        return true;
    }
    value = Properties->Last_Conflict();
    return LastDay[faction] != value ? true : false;
}

}


// int WorldDominationTour::Campaign::Last_Day(int faction) const (0x007263A0)

namespace WorldDominationTour {

int Campaign::Last_Day(int faction) const
{
    return LastDay[faction];
}

}


// void WorldDominationTour::Campaign::Save_Progress(int faction) (0x007263B0)
// Takes the properties' CampaignID and last conflict as the saved progress
// of the faction and writes both to WDTHist.ini.

namespace WorldDominationTour {

void Campaign::Save_Progress(int faction)
{
    ID = Properties->unknown_18();
    LastDay[faction] = Properties->Last_Conflict();
    INIClass ini;
    Load_History(ini);
    Wstring key = Wstring("LastDay") + Faction_Code(faction);
    {
        Wstring id_key("CampaignID");
        ini.Put_Int(Name, id_key, ID);
    }
    ini.Put_Int(Name, key, LastDay[faction]);
    CDFileClass file;
    file.Open("WDTHist.ini", FILE_RIGHTS_WRITE);
    ini.Save(file);
}

}


// void WorldDominationTour::Campaign::Reset_Progress() (0x00726580)
// Writes CampaignID 0 and LastDay 0 for every faction to WDTHist.ini.
// No callers in the executable.

namespace WorldDominationTour {

void Campaign::Reset_Progress()
{
    INIClass ini;
    Load_History(ini);
    {
        Wstring key("CampaignID");
        ini.Put_Int(Name, key, 0);
    }
    for (int faction = 0; faction < 4; faction++) {
        Wstring key = Wstring("LastDay") + Faction_Code(faction);
        ini.Put_Int(Name, key, 0);
    }
    CDFileClass file;
    file.Open("WDTHist.ini", FILE_RIGHTS_WRITE);
    ini.Save(file);
}

}


// ConflictCursor WorldDominationTour::Campaign::Find_Conflict(int index) (0x00726730)
// A cursor on the first conflict whose territory index matches.

namespace WorldDominationTour {

ConflictCursor Campaign::Find_Conflict(int index)
{
    ConflictCursor cursor(Conflicts);
    while (cursor && (*cursor)->unknown_7273f0() != index) {
        ++cursor;
    }
    return cursor;
}

}


// void WorldDominationTour::Campaign::Set_Player_Faction(int faction) (0x007267C0)
// Keeps a changed faction and writes it to WDTHist.ini as PlayerFaction.

namespace WorldDominationTour {

void Campaign::Set_Player_Faction(int faction)
{
    if (PlayerFaction != faction) {
        PlayerFaction = faction;
        INIClass ini;
        Load_History(ini);
        {
            Wstring key("PlayerFaction");
            ini.Put_Int(Name, key, PlayerFaction);
        }
        CDFileClass file;
    file.Open("WDTHist.ini", FILE_RIGHTS_WRITE);
    ini.Save(file);
    }
}

}


// void WorldDominationTour::Campaign::Load_History(INIClass &ini) (0x00726930)
// Loads WDTHist.ini into ini, creating the file first when it is missing.
// No callers: the unit's callers expand it inline.

namespace WorldDominationTour {

void Campaign::Load_History(INIClass &ini)
{
    CDFileClass file;
    file.Open("WDTHist.ini", FILE_RIGHTS_READ);
    if (!file.Is_Available()) {
        file.Create();
        file.Close();
        file.Open("WDTHist.ini", FILE_RIGHTS_READ);
    }
    ini.Load(file, false);
    file.Close();
}

}


// WorldDominationTour::Campaign scalar deleting destructor
// (0x007269B0): vtable 0x007AEFF4 slot 0; compiler generated from the virtual destructor.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<rc_ptr<WorldDominationTour::Conflict>>::Clear (0x00726A30).
// Pointer index in the campaign conflict list (0x00726A60; vtable007AF004 slot5).
// Use the recovered eight-byte rc_ptr class shared by the complete campaign unit.


// VectorClass<rc_ptr<WorldDominationTour::Conflict>>::Fetch
// (0x00726A80): compiler generated with the unit's DynamicVectorClass<rc_ptr<Conflict>> (shared/vector_class.hpp); no source of its own.


// 0x00726ab0: function not yet identified
//
// Owner: VectorCursor<rc_ptr<WorldDominationTour::Conflict>, DynamicVectorClass<rc_ptr<WorldDominationTour::Conflict>>>, virtual slot 1 (vtable +0x4) (from RTTI/vtables).
// Body: 14 bytes (frozen inventory bound).
// Referenced from 6 sites (computed call, data).
// Ghidra prototype: bool __fastcall unknown_in_range_00726ab0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00726ab0 {

bool __fastcall unknown_in_range_00726ab0(int self)
{
    return *(unsigned int *)(self + 8) < *(unsigned int *)(*(int *)(self + 4) + 0x10);
}

}  // namespace seed_00726ab0


// 0x00726ac0: function not yet identified
//
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_00726ac0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00726ac0 {

extern void __fastcall rc_ptr_base_dtor_00639510(void);
extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* rc_ptr_vtable_007af024;

struct Owner_00726ac0 { unsigned int * scalar_deleting_destructor_00726ac0(unsigned char flags); };

unsigned int * Owner_00726ac0::scalar_deleting_destructor_00726ac0(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&rc_ptr_vtable_007af024);
    rc_ptr_base_dtor_00639510();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_00726ac0


// 0x00726af0: function not yet identified
//
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_00726af0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00726af0 {

extern void __fastcall rc_ptr_base_dtor_00639510(void);
extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* rc_ptr_vtable_007af02c;

struct Owner_00726af0 { unsigned int * scalar_deleting_destructor_00726af0(unsigned char flags); };

unsigned int * Owner_00726af0::scalar_deleting_destructor_00726af0(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&rc_ptr_vtable_007af02c);
    rc_ptr_base_dtor_00639510();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_00726af0


// 0x00726b20: function not yet identified
//
// Owner: VectorCursor<rc_ptr<WorldDominationTour::Conflict>, DynamicVectorClass<rc_ptr<WorldDominationTour::Conflict>>>, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_00726b20(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00726b20 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* VectorCursor_vtable_007af054;

struct Owner_00726b20 { unsigned int * scalar_deleting_destructor_00726b20(unsigned char flags); };

unsigned int * Owner_00726b20::scalar_deleting_destructor_00726b20(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&VectorCursor_vtable_007af054);
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_00726b20


// 0x00726b40: function not yet identified
//
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_00726b40(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00726b40 {

extern void __fastcall rc_ptr_base_dtor_00639510(void);
extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* rc_ptr_vtable_007aefdc;

struct Owner_00726b40 { unsigned int * scalar_deleting_destructor_00726b40(unsigned char flags); };

unsigned int * Owner_00726b40::scalar_deleting_destructor_00726b40(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&rc_ptr_vtable_007aefdc);
    rc_ptr_base_dtor_00639510();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_00726b40


// 0x00726b70: function not yet identified
//
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_00726b70(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00726b70 {

extern void __fastcall rc_ptr_base_dtor_00639510(void);
extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* rc_ptr_vtable_007aefe4;

struct Owner_00726b70 { unsigned int * scalar_deleting_destructor_00726b70(unsigned char flags); };

unsigned int * Owner_00726b70::scalar_deleting_destructor_00726b70(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&rc_ptr_vtable_007aefe4);
    rc_ptr_base_dtor_00639510();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_00726b70


// VectorClass<rc_ptr<WorldDominationTour::Conflict>>::operator==
// (0x00726BA0): compiler generated with the unit's DynamicVectorClass<rc_ptr<Conflict>> (shared/vector_class.hpp); no source of its own.


// VectorClass<rc_ptr<WorldDominationTour::Conflict>>::Resize
// (0x00726BF0): compiler generated with the unit's DynamicVectorClass<rc_ptr<Conflict>> (shared/vector_class.hpp); no source of its own.


// VectorClass<rc_ptr<WorldDominationTour::Conflict>>::Clear
// (0x00726D10): compiler generated with the unit's DynamicVectorClass<rc_ptr<Conflict>> (shared/vector_class.hpp); no source of its own.


// VectorClass<rc_ptr<WorldDominationTour::Conflict>>::ID(T const &)
// (0x00726D40): compiler generated with the unit's DynamicVectorClass<rc_ptr<Conflict>> (shared/vector_class.hpp); no source of its own.


// VectorClass<rc_ptr<WorldDominationTour::Conflict>>::ID(T const *)
// (0x00726D80): compiler generated with the unit's DynamicVectorClass<rc_ptr<Conflict>> (shared/vector_class.hpp); no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<rc_ptr<WorldDominationTour::Conflict>>::Resize (0x00726DA0).
// DynamicVectorClass<rc_ptr<WorldDominationTour::Conflict>>::ID_object (0x00726EC0).


class Leaf_726ef0 { public: unsigned int get(); };
unsigned int Leaf_726ef0::get() { unsigned int self = (unsigned int)this; unsigned int cursor; unsigned int spare; cursor = self; *(unsigned int*)(cursor + 8) = (*(unsigned int*)(cursor + 8)) + 1; return cursor; }


class Leaf_726f00 { public: unsigned int get(); };
unsigned int Leaf_726f00::get() { unsigned int self = (unsigned int)this; unsigned int cursor; unsigned int spare; cursor = self; *(unsigned int*)(cursor + 8) = (*(unsigned int*)(cursor + 8)) - 1; return cursor; }


// VectorClass<rc_ptr<WorldDominationTour::Conflict>> scalar deleting destructor
// (0x00726F10): compiler generated with the unit's DynamicVectorClass<rc_ptr<Conflict>> (shared/vector_class.hpp); no source of its own.


// rc_ptr<WorldDominationTour::Conflict> vector deleting destructor
// (0x00726F60): compiler generated with the unit's DynamicVectorClass<rc_ptr<Conflict>> (shared/vector_class.hpp); no source of its own.


// Compiler-generated scalar deleting destructor of VectorClass<rc_ptr<WorldDominationTour::Conflict>>.
// Emitted by the complete actual campaign translation unit; no helper call is hand-written.


class Call_727020_0 {public: unsigned int target();};
extern "C" unsigned char data_727020_7aefec[];
class Leaf_727020 { public: unsigned int get(); };
unsigned int Leaf_727020::get() { unsigned int r0 = ((Call_727020_0*)((unsigned int)this))->target(); *(unsigned int*)(((unsigned int)this)) = (unsigned int)data_727020_7aefec; return (unsigned int)this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<rc_ptr<WorldDominationTour::Conflict>>::VectorClass(unsigned, rc_ptr<Conflict> const *) (0x00727040).
// The generic body of shared/vector_class.hpp written as an explicit specialization (VC6 cannot
// explicitly instantiate a constructor alone, and the whole-class instantiation would emit an
// out-of-line ~VectorClass the original does not have). The Campaign unit's copy (the
// Campaign constructor 0x00725F40 calls it); its vtable 0x007AF034 comes with it.
