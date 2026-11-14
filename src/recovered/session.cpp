// session.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/session.json carries the addresses and the rows that
// stay in their own files.


#define RA2_VECTOR_DELETE_DIRECT  // migration holdout: unit not yet re-rolled to Westwood Delete(int) through operator[] (docs/HANDOFF.md)
#define RA2_SESSION_STATE_DEFINED
#define RA2_SESSION_CTOR_FIELDS
#define RA2_COORD_AS_CELL
#define RA2_SESSION_MAP_PLAYER_ACCESSORS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_ID_OBJECT
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/vector_class.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/session_state.hpp"
#include "shared/multi_mission.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"
#include "shared/string_table.hpp"
#include "shared/debug_printf.hpp"
#include "shared/house.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/ccini_class.hpp"
#include "shared/rules.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/abstract_type.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/ipx_conn.hpp"
#include "shared/connection.hpp"
#include "shared/owner_talk.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/ipx_address.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/serial_settings.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/random2.hpp"
#include "shared/firewall_settings.hpp"
#include "shared/modem_guest.hpp"
#include "shared/lobby.hpp"
#include "shared/udp_interface.hpp"
#include "shared/campaign.hpp"
#include "shared/mouse.hpp"
#include "shared/gscreen.hpp"
#include "shared/map_view.hpp"
#include "shared/coord.hpp"
#include "shared/crt_stdio.hpp"
#include "shared/buffer_straw.hpp"
#include "shared/ini_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cd.hpp"
#include "shared/win32_find.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_CLEAR
#include "shared/serial_progress.hpp"
#include "shared/queue.hpp"
#include "shared/system_timer.hpp"
#include "shared/cd_timer.hpp"
#include "shared/win32_api.hpp"
#include "shared/keyboard.hpp"
#include "shared/audio.hpp"
#include "shared/special.hpp"
#include "shared/id_source.hpp"
#include "shared/house_type.hpp"
#include "shared/options.hpp"
#include "shared/tool_tip.hpp"
#include "shared/vox_class.hpp"
#include "shared/vox_speech.hpp"


// 0x0066ee40: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0066ee40(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0066ee40 {

void  FUN_0066ee40(void)
{
    return;
}

}  // namespace seed_0066ee40


// Session unit profile: the constructor 0x0066E380 sees the ctor-only fields; Cell compares as the YR twin's
// member operators (Read_MultiPlayer_Settings 0x0066F3A0 with the TargetClass TrapTarget).
// Header-static dynamic initializers of the unit at 0x0066E0A0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0066E0A0-0x0066E281); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0066E0C0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E0F0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E110, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E130, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E150, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E170, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E190, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E1D0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E200, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E230, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E260, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// Dynamic initializer (0x0066E270, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0066e0a0.cpp; no source of its own.


// NodeNameType accessors defined in Session.CPP (0x0066E290-0x0066E320); the session members later in the unit
// expand them inline. The vector accessors this unit uses are implemented here (the first include of
// shared/vector_class.hpp in the unit).

// The country (+0x4B); -2 when +0x4F marks it random.
int NodeNameType::unknown_66e290()
{
    if (unknown_4f == -2) {
        return -2;
    }
    return unknown_4b;
}


// The colour (+0x53); -2 when +0x57 marks it random.
int NodeNameType::unknown_66e2a0()
{
    if (unknown_57 == -2) {
        return -2;
    }
    return unknown_53;
}


// The colour (+0x53); -2 when +0x57 marks it random and no colour is set.
int NodeNameType::Get_Color_0066e2b0()
{
    if (unknown_57 == -2 && unknown_53 == -1) {
        return -2;
    }
    return unknown_53;
}


// Sets the country; -2 (random) stores country 0 and marks +0x4F.
void NodeNameType::unknown_66e2d0(int value)
{
    if (value == -2) {
        unknown_4b = 0;
        unknown_4f = -2;
    } else {
        unknown_4b = value;
        unknown_4f = -1;
    }
}


// Sets the colour; -2 (random) stores colour -1 and marks +0x57.
void NodeNameType::unknown_66e300(int value)
{
    if (value == -2) {
        unknown_53 = -1;
        unknown_57 = -2;
    } else {
        unknown_53 = value;
        unknown_57 = -1;
    }
}


// SessionOptionsClass (the 0x7C option block; session +0x13C, +0x1B8, +0x234).

// The defaults: credits 10000, ten units, the five flags set, the rest 0.
SessionOptionsClass::SessionOptionsClass() :
    GameMode(0),
    ScenIndex(0),
    GameSpeed(0),
    Credits(10000),
    UnitCount(10),
    ShortGame(true),
    SuperWeaponsAllowed(true),
    BuildOffAlly(true),
    MCVRepacks(true),
    CratesAppear(true)
{
    for (int i = 0; i < 8; i++) {
        Slots[i][0] = 0;
        Slots[i][1] = 0;
        Slots[i][2] = 0;
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// SessionClass::SessionClass() (0x0066E380; RA2 SessionClass::SessionClass)
// Statement order from ra2-decomp SessionClass::SessionClass (YR 0x006970A0, src/session.cpp):
// MaxPlayers, NumPlayers, FrameSendRate, MaxAhead = FrameSendRate * 3, MaxMaxAhead = MaxAhead,
// LastMessage[0], WWChat, ModemService and TrapTarget.Invalidate(); TrapTarget is a TargetClass
// member (TibSun/YR), whose inline default constructor clears Kind with the other members.
template <>
inline
VectorClass<MultiMission *>::VectorClass(unsigned size, MultiMission * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) MultiMission *[size];
        } else {
            Vector = new MultiMission *[size];
            IsAllocated = true;
        }
    }
}

template <>
inline
DynamicVectorClass<MultiMission *>::DynamicVectorClass(unsigned size, MultiMission * const *array)
    : VectorClass<MultiMission *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
template <>
inline
VectorClass<UnknownGlobalPacket *>::VectorClass(unsigned size, UnknownGlobalPacket * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) UnknownGlobalPacket *[size];
        } else {
            Vector = new UnknownGlobalPacket *[size];
            IsAllocated = true;
        }
    }
}

template <>
inline
DynamicVectorClass<UnknownGlobalPacket *>::DynamicVectorClass(unsigned size, UnknownGlobalPacket * const *array)
    : VectorClass<UnknownGlobalPacket *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
template <>
inline
VectorClass<NodeNameType *>::VectorClass(unsigned size, NodeNameType * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) NodeNameType *[size];
        } else {
            Vector = new NodeNameType *[size];
            IsAllocated = true;
        }
    }
}

template <>
inline
DynamicVectorClass<NodeNameType *>::DynamicVectorClass(unsigned size, NodeNameType * const *array)
    : VectorClass<NodeNameType *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
template <>
inline
VectorClass<PhoneEntryClass *>::VectorClass(unsigned size, PhoneEntryClass * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) PhoneEntryClass *[size];
        } else {
            Vector = new PhoneEntryClass *[size];
            IsAllocated = true;
        }
    }
}

template <>
inline
DynamicVectorClass<PhoneEntryClass *>::DynamicVectorClass(unsigned size, PhoneEntryClass * const *array)
    : VectorClass<PhoneEntryClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
template <>
inline
VectorClass<char *>::VectorClass(unsigned size, char * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) char *[size];
        } else {
            Vector = new char *[size];
            IsAllocated = true;
        }
    }
}

template <>
inline
DynamicVectorClass<char *>::DynamicVectorClass(unsigned size, char * const *array)
    : VectorClass<char *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
template <>
inline
VectorClass<unsigned short const *>::VectorClass(unsigned size, unsigned short const * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) unsigned short const *[size];
        } else {
            Vector = new unsigned short const *[size];
            IsAllocated = true;
        }
    }
}

template <>
inline
DynamicVectorClass<unsigned short const *>::DynamicVectorClass(unsigned size, unsigned short const * const *array)
    : VectorClass<unsigned short const *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
extern "C" int session_ctor_trace_b26cbc[2048];
extern "C" int session_ctor_trace_b291a4[64];
extern "C" char const session_empty[];
SessionClass::SessionClass() : Mode(0)
{
 Type=0; unknown_14=2; ScenarioIndex=0; unknown_bases_20=true; unknown_24=0;
 unknown_28=false; unknown_crates_29=false; unknown_ghosts_35=false; UnitCount=0;
 unknown_2a=false; unknown_2b=false; unknown_2c=false; GameSpeed_30=0; unknown_34=false;
 unknown_a4=false; unknown_a5=false; unknown_a6=false; unknown_fog_a7=false; unknown_a8=false;
 unknown_difficulty_40=1; UniqueID=0; Handle[0]=0;
 unknown_124=0; unknown_128=-1; unknown_11c=2; unknown_120=-1;
 unknown_134=unknown_124; unknown_138=unknown_128; unknown_12c=unknown_11c; unknown_130=unknown_120;
 WOLTaunts=true; LANTaunts=true; WOLScrollText=true; LANScrollText=true;
 for(int i=0;i<8;++i) unknown_3022.unknown[i]=0;
 unknown_3020=false; unknown_3014=false; unknown_3016=0; unknown_3018=0;
 PortBase=1250; unknown_2f9c=0; unknown_2fc0=true; SendDelay=false; PortNumberOverride=0;
 memset(session_ctor_trace_b26cbc,0,sizeof(session_ctor_trace_b26cbc));
 memset(session_ctor_trace_b291a4,0,sizeof(session_ctor_trace_b291a4));
 unknown_2b0=0; unknown_2b4=0; LastNickSlot=-1; WOLLimitResolution=false; ViaPacketRouter=false;
 MasterPlayerID=-1; memset(MasterPlayerName,0,sizeof(MasterPlayerName));
 unknown_3034=0; unknown_3038=true; unknown_303c=-1; unknown_3040[0]=false;
 unknown_3044=20; unknown_3048=0; unknown_304c=0; unknown_3050=-1; unknown_292c=0;
 unknown_638=false; unknown_639=false; unknown_63a=false;
 unknown_2c0=8; NumPlayers=0; unknown_2cc=5; MaxAhead_2c8=unknown_2cc*3; unknown_2e0=MaxAhead_2c8;
 memset(unknown_2ec,0,sizeof(unknown_2ec)); unknown_2e4=0; unknown_2e8=0;
 unknown_62c=false; unknown_62d=false; LoadGame=0; unknown_634=0;
 LastMessage[0]=0; WWChat=0;
 RecordFile.Set_Name("RECORD.BIN");
 Record=false; Play=false; Attract=false;
 unknown_236c=0; unknown_237c=false; NetProtect=true; NetOpen=false;
 HostName_2380[0]=0; GProductID=0; unknown_2824=0; unknown_290c=1; unknown_27b0=0x22e;
 memset(KickVotes,0,sizeof(KickVotes)); memset(KickVoters,0xff,sizeof(KickVoters));
 ModemService=true; CurPhoneIdx=0;
 SerialDefaults.Port=0x2f8; SerialDefaults.IRQ=3; SerialDefaults.Baud=9600; SerialDefaults.DialMethod=0;
 SerialDefaults.InitStringIndex=-1; SerialDefaults.CallWaitStringIndex=0;
 strcpy(SerialDefaults.CallWaitString,session_empty); unknown_28a0=0;
 TrapFrame=0x7fffffff; TrapObjType=0; unknown_305c=0; unknown_3060=unknown_geometry_l;
 TrapTarget.Invalidate(); TrapCell=0; TrapCheckHeap=0; TrapPrintCRC=0;
 unknown_2820=false; AICount=0;
 for(i=0;i<8;++i){ AICountry[i]=-1; AIColor[i]=-1; }
 unknown_0c=0; unknown_10=0; ScenarioFileName_658[0]=0;
 ScenarioFileLength_87c=0; ScenarioIsOfficial_880=false; unknown_884=0;
}

// The DynamicVectorClass vtables this constructor emits: their slots (read from the original vtables).


// SessionClass (the session object at 0x00A3D298, shared/unknown_globals.hpp SessionClass).

// The call-waiting labels (0x007F3214: TXT_CWAIT_STAR70 and three more).
extern char const *unknown_call_wait_labels_7f3214[4];
extern "C" unsigned short *__cdecl _wcsdup(unsigned short const *string);

void SessionClass::Unknown_0066EAA0()
{
    for (int i = 0; i < 4; i++) {
        CallWaitStrings.Add(_wcsdup(StringTable::Load_String(unknown_call_wait_labels_7f3214[i], 0,
                                                            "D:\\Ra2\\Session.CPP", 536)));
    }
}


extern "C" void __cdecl free(void *block);

SessionClass::~SessionClass()
{
    while (InitStrings.Count()) {
        delete [] InitStrings[0];
        InitStrings.Delete(0);
    }
    while (CallWaitStrings.Count()) {
        free((void *)CallWaitStrings[0]);
        CallWaitStrings.Delete(0);
    }
}


void SessionClass::One_Time()
{
    Read_Scenario_Descriptions();
    UniqueID = Compute_Unique_ID();
    Debug_Printf("Session one time init. UniqueID is %08x\n", UniqueID);
}


int SessionClass::Create_Connections()
{
    NetNumType net;
    NetNodeType node;

    Debug_Printf("Entering Create_Connections\n");
    if (unknown_game_mode_a3d298 != 3 && unknown_game_mode_a3d298 != 4) {
        return 0;
    }

    for (int i = 0; i < Players.Count(); i++) {
        Debug_Printf("Player %d, Name: %s, ID %d\n", i, Players[i]->Name, Players[i]->ID);
        if (_wcsicmp(Players[i]->Name, Houses[Players[i]->ID]->UIName) != 0) {
            return 0;
        }
        if (_wcsicmp(Players[i]->Name, MasterPlayerName) == 0) {
            MasterPlayerID = Players[i]->ID;
            unknown_players_a3fa7c[i]->Address.Get_Address(net, node);
            unknown_forward_address_a3fbac.Set_Address(net, node, 0);
            unknown_ipx_manager_a40978.unknown_5277c0(&unknown_forward_address_a3fbac);
        }
        Houses[Players[i]->ID]->unknown_14c = Players[i]->unknown_6b;
        unknown_players_a3fa7c[i]->Address.Get_Address(net, node);
        unsigned long ip = 0;
        for (int j = 0; j < 4; j++) {
            ip = (ip << 8) | node[j];
        }
        Debug_Printf("House[%d] IP = %X  Clan=%d\n", Players[i]->ID, ip, Players[i]->unknown_6b);
        Houses[Players[i]->ID]->unknown_148 = ip;
        Houses[Players[i]->ID]->unknown_150 = false;
        if (i > 0) {
            unknown_ipx_manager_a40978.Create_Connection(Players[i]->ID, Players[i]->Name, &Players[i]->Address);
            Players[i]->unknown_63 = -1;
        }
    }

    for (int k = 0; k < 8; k++) {
        unknown_session_a3d298.unknown_2ff4[k] = TickCount.Time();
    }

    if (Type == 4 && unknown_3038) {
        NetNumType net2;
        NetNodeType node2;
        net2[0] = 0;
        net2[1] = 0;
        net2[2] = 0;
        net2[3] = 0;
        node2[0] = 0;
        node2[1] = 0;
        node2[2] = 0;
        node2[3] = 0;
        node2[4] = 0;
        node2[5] = 0;
        unknown_forward_address_a3fbb8.Set_Address(net2, node2, 0);
        ViaPacketRouter = true;
        unknown_3034 = 0;
        unknown_3044 = 20;
        unknown_303c = -1;
        unknown_ipx_manager_a40978.unknown_5255b0();
        unknown_OwnerTalkClass_00ab6d18.Unknown_0063F2A0();
        if (unknown_session_a3d298.Am_I_Master(0)) {
            bool connected = unknown_OwnerTalkClass_00ab6d18.Unknown_0063F510(0);
            if (connected) {
                connected = unknown_OwnerTalkClass_00ab6d18.Unknown_0063F130();
            }
            if (!connected) {
                unknown_flag_a3fba8 = false;
            }
        }
    } else {
        unknown_flag_a3fba8 = false;
    }
    Debug_Printf("Session.ViaPacketRouter = %s\n", unknown_flag_a3fba8 ? "true" : "false");
    Debug_Printf("Leaving Create_Connections\n");
    return 1;
}


bool SessionClass::Am_I_Master(HouseClass *house)
{
    if (house == 0) {
        house = unknown_player_a35db4;
    }
    if (unknown_game_mode_a3d298 == 4 && house != 0) {
        if (MasterPlayerID != -1) {
            return house->ArrayIndex == MasterPlayerID;
        }
        if (_wcsicmp(house->UIName, MasterPlayerName) == 0) {
            return true;
        }
    }
    for (int i = 0; i < Houses.Count(); i++) {
        HouseClass *player = Houses[i];
        if (player->unknown_134 && !player->unknown_13d) {
            if (house == player) {
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}


void SessionOptionsClass::Read(CCINIClass &ini, char const *section, int other_slots, int first_slot)
{
    GameMode = ini.Get_Int(section, "GameMode", MultiplayerGameMode::unknown_first_mode_5b6ec0()->Get_ID());
    ScenIndex = ini.Get_Int(section, "ScenIndex", 0);
    GameSpeed = ini.Get_Int(section, "GameSpeed", Rules->GameSpeed);
    Credits = ini.Get_Int(section, "Credits", Rules->Money);
    UnitCount = ini.Get_Int(section, "UnitCount", Rules->UnitCount);
    ShortGame = ini.Get_Bool(section, "ShortGame", Rules->ShortGame);
    SuperWeaponsAllowed = ini.Get_Bool(section, "SuperWeaponsAllowed", Rules->SuperWeaponsAllowed);
    BuildOffAlly = ini.Get_Bool(section, "BuildOffAlly", Rules->BuildOffAlly);
    MCVRepacks = ini.Get_Bool(section, "MCVRepacks", Rules->MCVRedeploys);
    CratesAppear = ini.Get_Bool(section, "CratesAppear", Rules->Crates);
    for (int i = 1; i < 8; i++) {
        char buffer[16];
        sprintf(buffer, "Slot%02d", i);
        int first = (i == 1) ? first_slot : other_slots;
        int second = -2;
        int third = -2;
        ini.Get_Integers(section, buffer, first, second, third);
        Slots[i][0] = first;
        Slots[i][1] = second;
        Slots[i][2] = third;
    }
}


extern "C" unsigned short *__cdecl _wcsupr(unsigned short *string);
// The dial method letters (0x007F320C; also declared by Write_MultiPlayer_Settings 0x00670380).
extern char const *unknown_dial_methods_7f320c[2];
// Set once the port settings are read (0x00AB9178).
extern bool unknown_ports_read_ab9178;
// The firewall detection state (0x00A71B18) and the port pool after it (0x00A71B28).
extern FirewallSettings unknown_firewall_a71b18;
extern int unknown_port_pool_a71b28;
// The "no cell" value the SyncBug cell is compared with (0x00AB71D8).

// Red Alert's SessionClass::Read_MultiPlayer_Settings: reads the player and serial settings,
// the init strings, the phone book and the SyncBug settings from RA2.INI.
void SessionClass::Read_MultiPlayer_Settings()
{
    char *tokenptr;
    PhoneEntryClass *phone;
    char *entry;
    char buf[0x400];
    int i;

    Rules->Countries(*unknown_ini_8396d0);

    for (i = 0; i < InitStrings.Count(); i++) {
        delete [] InitStrings[i];
    }
    InitStrings.Clear();

    for (i = 0; i < PhoneBook.Count(); i++) {
        delete PhoneBook[i];
    }
    PhoneBook.Clear();

    CCFileClass file("RA2.INI");

    if (!unknown_ports_read_ab9178) {
        PortBase = unknown_ini_839740.Get_Int("MultiPlayer", "PortBase", 0x4E2);
        unknown_2f9c = unknown_ini_839740.Get_Int("MultiPlayer", "ForcePortBase", 0);
        unknown_port_pool_a71b28 = unknown_ini_839740.Get_Int("MultiPlayer", "PortPool", unknown_random_839218(0, 0x8000) + 0x1000);
        unknown_ports_read_ab9178 = true;
        unknown_firewall_a71b18.Load_005ba280();
    }
    unknown_session_a3d298.SendDelay = unknown_ini_839740.Get_Bool("MultiPlayer", "SendDelay", false);
    unknown_session_a3d298.PortNumberOverride = unknown_ini_839740.Get_Int("MultiPlayer", "PortNumberOverride", 0);

    buf[0] = 0;
    CurPhoneIdx = unknown_ini_839740.Get_Int("MultiPlayer", "PhoneIndex", -1);
    TrapCheckHeap = unknown_ini_839740.Get_Int("MultiPlayer", "CheckHeap", 0);
    WOLLimitResolution = unknown_ini_839740.Get_Bool("MultiPlayer", "WOLLimitResolution", WOLLimitResolution);
    LastNickSlot = unknown_ini_839740.Get_Int("MultiPlayer", "LastNickSlot", -1);

    unsigned short handle[20];
    unknown_ini_839740.Get_Unicode("MultiPlayer", "Handle",
                                   StringTable::Load_String("TXT_NONAME", 0, "D:\\Ra2\\Session.CPP", 0x4b2), handle, 20);
    WideToCharString_006fa0f0(Handle, handle, -1);

    unknown_12c = unknown_ini_839740.Get_Int("MultiPlayer", "Color", unknown_12c);
    unknown_130 = unknown_ini_839740.Get_Int("MultiPlayer", "ColorEx", unknown_130);
    unknown_134 = unknown_ini_839740.Get_HousesType("MultiPlayer", "Side", unknown_134);
    unknown_138 = unknown_ini_839740.Get_Int("MultiPlayer", "SideEx", unknown_138);
    WOLTaunts = unknown_ini_839740.Get_Bool("MultiPlayer", "WOLTaunts", WOLTaunts);
    LANTaunts = unknown_ini_839740.Get_Bool("MultiPlayer", "LANTaunts", LANTaunts);
    WOLScrollText = unknown_ini_839740.Get_Bool("MultiPlayer", "WOLScrollText", WOLScrollText);
    LANScrollText = unknown_ini_839740.Get_Bool("MultiPlayer", "LANScrollText", LANScrollText);

    unknown_124 = unknown_134;
    if (unknown_124 == -2) {
        unknown_124 = 0;
    }
    unknown_128 = unknown_138;

    Mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_ini_839740.Get_Int("MultiPlayer", "GameMode", 1));
    if (!Mode) {
        Mode = MultiplayerGameMode::unknown_first_mode_5b6ec0();
    }
    SkirmishOptions.Read(unknown_ini_839740, "Skirmish", 1, 6);
    LANOptions.Read(unknown_ini_839740, "LAN", 2, 2);
    WonlineOptions.Read(unknown_ini_839740, "WonlinePref", 2, 2);

    unknown_ini_839740.Get_String("SerialDefaults", "ModemName", buf, SerialDefaults.ModemName, 0x3F);
    if (strcmp(SerialDefaults.ModemName, buf) == 0) {
        SerialDefaults.ModemName[0] = 0;
    }
    SerialDefaults.Port = unknown_ini_839740.Get_Int("SerialDefaults", "Port", 0);
    SerialDefaults.IRQ = unknown_ini_839740.Get_Int("SerialDefaults", "IRQ", -1);
    SerialDefaults.Baud = unknown_ini_839740.Get_Int("SerialDefaults", "Baud", -1);
    SerialDefaults.Compression = unknown_ini_839740.Get_Int("SerialDefaults", "Compression", 0);
    SerialDefaults.ErrorCorrection = unknown_ini_839740.Get_Int("SerialDefaults", "ErrorCorrection", 0);

    unknown_ini_839740.Get_String("SerialDefaults", "DialMethod", "T", buf, 2);
    for (i = 0; i < 2; i++) {
        if (!_stricmp(buf, unknown_dial_methods_7f320c[i])) {
            SerialDefaults.DialMethod = i;
            break;
        }
    }
    if (i == 2) {
        SerialDefaults.DialMethod = 0;
    }

    SerialDefaults.InitStringIndex = unknown_ini_839740.Get_Int("SerialDefaults", "InitStringIndex", -1);
    SerialDefaults.CallWaitStringIndex = unknown_ini_839740.Get_Int("SerialDefaults", "CallWaitStringIndex", 3);
    unknown_ini_839740.Get_String("SerialDefaults", "CallWaitString", "", SerialDefaults.CallWaitString, 0x10);

    if (SerialDefaults.IRQ == 0 || SerialDefaults.Baud == 0) {
        SerialDefaults.Port = 0;
        SerialDefaults.IRQ = -1;
        SerialDefaults.Baud = -1;
    }

    int initcount = unknown_ini_839740.Entry_Count("InitStrings");
    int index;
    for (index = 0; index < initcount; index++) {
        entry = new char[0x29];
        if (entry) {
            entry[0] = 0;
            unknown_ini_839740.Get_String("InitStrings", unknown_ini_839740.Get_Entry("InitStrings", index), 0, entry, 0x29);
            _strupr(entry);
            InitStrings.Add(entry);
        }
    }
    SerialDefaults.InitStringIndex = initcount ? 0 : -1;

    int phonecount = unknown_ini_839740.Entry_Count("PhoneBook");
    for (index = 0; index < phonecount; index++) {
        phone = new PhoneEntryClass;
        unknown_ini_839740.Get_String("PhoneBook", unknown_ini_839740.Get_Entry("PhoneBook", index), 0, buf, sizeof(buf));

        tokenptr = strtok(buf, "|");
        if (tokenptr) {
            Char_To_Wide_006fa0c0(phone->Name, tokenptr, -1);
            _wcsupr(phone->Name);
        } else {
            phone->Name[0] = 0;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            strcpy(phone->Number, tokenptr);
            _strupr(phone->Number);
        } else {
            phone->Number[0] = 0;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            sscanf(tokenptr, "%x", &phone->Settings.Port);
        } else {
            phone->Settings.Port = 0;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            phone->Settings.IRQ = atoi(tokenptr);
        } else {
            phone->Settings.IRQ = -1;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            phone->Settings.Baud = atoi(tokenptr);
        } else {
            phone->Settings.Baud = -1;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            phone->Settings.Compression = atoi(tokenptr);
        } else {
            phone->Settings.Compression = -1;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            phone->Settings.ErrorCorrection = atoi(tokenptr);
        } else {
            phone->Settings.ErrorCorrection = 0;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            strcpy(buf, tokenptr);
            for (i = 0; i < 2; i++) {
                if (!_stricmp(buf, unknown_dial_methods_7f320c[i])) {
                    phone->Settings.DialMethod = i;
                    break;
                }
            }
            if (i == 2) {
                phone->Settings.DialMethod = 0;
            }
        } else {
            phone->Settings.DialMethod = 0;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            phone->Settings.InitStringIndex = atoi(tokenptr);
        } else {
            phone->Settings.InitStringIndex = -1;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            phone->Settings.CallWaitStringIndex = atoi(tokenptr);
        } else {
            phone->Settings.CallWaitStringIndex = 3;
        }

        tokenptr = strtok(0, "|");
        if (tokenptr) {
            strcpy(phone->Settings.CallWaitString, tokenptr);
        } else {
            phone->Settings.CallWaitString[0] = 0;
        }

        PhoneBook.Add(phone);
    }

    if (unknown_session_a3d298.Play) {
        TrapFrame = unknown_ini_839740.Get_Int("SyncBug", "Frame", 0x7FFFFFFF);
        unknown_ini_839740.Get_String("SyncBug", "Type", "NONE", buf, 0x50);
        if (!_stricmp(buf, "AIRCRAFT")) {
            TrapObjType = 2;
        } else if (!_stricmp(buf, "ANIM")) {
            TrapObjType = 4;
        } else if (!_stricmp(buf, "BUILDING")) {
            TrapObjType = 6;
        } else if (!_stricmp(buf, "BULLET")) {
            TrapObjType = 8;
        } else if (!_stricmp(buf, "INFANTRY")) {
            TrapObjType = 15;
        } else if (!_stricmp(buf, "UNIT")) {
            TrapObjType = 1;
        } else {
            TrapObjType = 0;
        }

        unknown_ini_839740.Get_String("SyncBug", "Coord", "0", buf, 0x50);
        int x;
        int y;
        int z;
        sscanf(buf, "%d,%d,%d", &x, &y, &z);
        int target = unknown_ini_839740.Get_Int("SyncBug", "Target", -1);
        TrapTarget.Kind = target >> 24;
        TrapTarget.ID = target & 0xFFFFFF;

        unknown_ini_839740.Get_String("SyncBug", "Cell", "0,0", buf, 0x50);
        sscanf(buf, "%d,%d", &x, &y);
        Cell cell(x, y);
        if (cell != unknown_geometry_k) {
            TrapCell = &Map[cell];
        }

        TrapPrintCRC = unknown_ini_839740.Get_Int("SyncBug", "PrintCRC", 0x7FFFFFFF);
    }
}


bool SessionClass::Log_To_File(_iobuf *out)
{
    int i;
    int j;
    NetNumType net;
    NetNodeType node;

    if (unknown_session_a3d298.ScenarioIndex < 0) {
        fprintf(out, "BOGUS scenario index!?!\n");
        unknown_session_a3d298.ScenarioIndex = 0;
    }

    fprintf(out, "Scenario = %s\n", unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->Description);
    fprintf(out, "Handle = %s\n", Handle);

    for (i = 0; i < NumPlayers; i++) {
        unknown_session_a3d298.Players[i]->Address.Get_Address(net, node);
        fprintf(out, "Player Addr[%d] = ", i);
        for (j = 0; j < sizeof(NetNumType); j++) {
            fprintf(out, "%02X", net[j]);
        }
        fprintf(out, ":");
        for (j = 0; j < sizeof(NetNodeType); j++) {
            fprintf(out, "%02X", node[j]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n");

    unknown_address_a3db2c.Get_Address(net, node);
    fprintf(out, "Address = ");
    for (i = 0; i < sizeof(NetNumType); i++) {
        fprintf(out, "%02X", net[i]);
    }
    fprintf(out, ":");
    for (i = 0; i < sizeof(NetNodeType); i++) {
        fprintf(out, "%02X", node[i]);
    }
    fprintf(out, "\n\n");
    fprintf(out, "MaxAhead = %d\n", MaxAhead_2c8);
    fprintf(out, "LoadGame = %d\n", LoadGame);
    fprintf(out, "PrefColor = %d\n", unknown_12c);
    fprintf(out, "ColorIdx = %d\n", unknown_11c);
    fprintf(out, "House = %d\n", unknown_124);
    fprintf(out, "NumPlayers = %d\n", NumPlayers);
    fprintf(out, "Options.Bases = %d\n", unknown_bases_20);
    fprintf(out, "Options.Credits = %d\n", unknown_24);
    fprintf(out, "Options.BridgeDestruction = %d\n", unknown_28);
    fprintf(out, "Options.Goodies = %d\n", unknown_crates_29);
    fprintf(out, "Options.Ghosts = %d\n", unknown_ghosts_35);
    fprintf(out, "Options.UnitCount = %d\n", UnitCount);
    fprintf(out, "Options.AIPlayers = %d\n", AICount);
    fprintf(out, "Options.AIDifficulty = %d\n", unknown_difficulty_40);
    fprintf(out, "ObiWan = %d\n", unknown_2b0);

    if (unknown_session_a3d298.Type == 3) {
        fprintf(out, "Type = IPX\n");
    } else if (unknown_session_a3d298.Type == 4) {
        fprintf(out, "Type = INTERNET\n");
    } else {
        fprintf(out, "Type = !!Unknown!!\n");
    }
    return true;
}


void SessionOptionsClass::Write(CCINIClass &ini, char const *section)
{
    ini.Put_Int(section, "GameMode", GameMode);
    ini.Put_Int(section, "ScenIndex", ScenIndex);
    ini.Put_Int(section, "GameSpeed", GameSpeed);
    ini.Put_Int(section, "Credits", Credits);
    ini.Put_Int(section, "UnitCount", UnitCount);
    ini.Put_Bool(section, "ShortGame", ShortGame);
    ini.Put_Bool(section, "SuperWeaponsAllowed", SuperWeaponsAllowed);
    ini.Put_Bool(section, "BuildOffAlly", BuildOffAlly);
    ini.Put_Bool(section, "MCVRepacks", MCVRepacks);
    ini.Put_Bool(section, "CratesAppear", CratesAppear);
    for (int i = 1; i < 8; i++) {
        char buffer[16];
        sprintf(buffer, "Slot%02d", i);
        ini.Put_Integers(section, buffer, Slots[i][0], Slots[i][1], Slots[i][2]);
    }
}


// The dial method letters (0x007F320C: "T" tone, "P" pulse; Red Alert's DialMethodCheck).
extern char const *unknown_dial_methods_7f320c[2];

// Red Alert's SessionClass::Write_MultiPlayer_Settings: saves the player and serial
// settings, the init strings and the phone book to RA2.INI.
void SessionClass::Write_MultiPlayer_Settings()
{
    RawFileClass file("RA2.INI");

    unknown_ini_839740.Put_Int("MultiPlayer", "PhoneIndex", CurPhoneIdx);
    unknown_ini_839740.Put_Bool("MultiPlayer", "WOLLimitResolution", WOLLimitResolution);
    unknown_ini_839740.Put_Int("MultiPlayer", "LastNickSlot", LastNickSlot);
    unknown_ini_839740.Put_Unicode("MultiPlayer", "Handle", Narrow_To_Wide_006fa180(Handle));
    unknown_ini_839740.Put_Int("MultiPlayer", "Color", unknown_12c);
    unknown_ini_839740.Put_Int("MultiPlayer", "ColorEx", unknown_130);
    unknown_ini_839740.Put_HousesType("MultiPlayer", "Side", unknown_134);
    unknown_ini_839740.Put_Int("MultiPlayer", "SideEx", unknown_138);
    if (Mode) {
        unknown_ini_839740.Put_Int("MultiPlayer", "GameMode", Mode->Get_ID());
    } else {
        unknown_ini_839740.Put_Int("MultiPlayer", "GameMode", 0);
    }
    SkirmishOptions.Write(unknown_ini_839740, "Skirmish");
    LANOptions.Write(unknown_ini_839740, "LAN");
    WonlineOptions.Write(unknown_ini_839740, "WonlinePref");

    unknown_ini_839740.Clear("SerialDefaults");
    unknown_ini_839740.Put_String("SerialDefaults", "CallWaitString", SerialDefaults.CallWaitString);
    unknown_ini_839740.Put_Int("SerialDefaults", "CallWaitStringIndex", SerialDefaults.CallWaitStringIndex);
    unknown_ini_839740.Put_Int("SerialDefaults", "InitStringIndex", SerialDefaults.InitStringIndex);
    unknown_ini_839740.Put_String("SerialDefaults", "DialMethod", unknown_dial_methods_7f320c[SerialDefaults.DialMethod]);
    unknown_ini_839740.Put_Int("SerialDefaults", "Baud", SerialDefaults.Baud);
    unknown_ini_839740.Put_Int("SerialDefaults", "IRQ", SerialDefaults.IRQ);
    unknown_ini_839740.Put_Int("SerialDefaults", "Port", SerialDefaults.Port, 1);
    unknown_ini_839740.Put_Int("SerialDefaults", "Compression", SerialDefaults.Compression);
    unknown_ini_839740.Put_Int("SerialDefaults", "ErrorCorrection", SerialDefaults.ErrorCorrection);
    unknown_ini_839740.Put_String("SerialDefaults", "ModemName", SerialDefaults.ModemName);

    unknown_ini_839740.Clear("InitStrings");
    for (int index = 0; index < InitStrings.Count(); index++) {
        char buf[10];
        wsprintfA(buf, "%03d", index);
        unknown_ini_839740.Put_String("InitStrings", buf, InitStrings[index]);
    }

    unknown_ini_839740.Clear("PhoneBook");
    for (int i = PhoneBook.Count() - 1; i >= 0; i--) {
        char buf[128];
        char entrytext[10];
        wsprintfA(buf, "%s|%s|%x|%d|%d|%d|%d|%s|%d|%d|%s",
                  PhoneBook[i]->Name,
                  PhoneBook[i]->Number,
                  PhoneBook[i]->Settings.Port,
                  PhoneBook[i]->Settings.IRQ,
                  PhoneBook[i]->Settings.Baud,
                  PhoneBook[i]->Settings.Compression,
                  PhoneBook[i]->Settings.ErrorCorrection,
                  unknown_dial_methods_7f320c[PhoneBook[i]->Settings.DialMethod],
                  PhoneBook[i]->Settings.InitStringIndex,
                  PhoneBook[i]->Settings.CallWaitStringIndex,
                  PhoneBook[i]->Settings.CallWaitString);
        wsprintfA(entrytext, "%03d", i);
        unknown_ini_839740.Put_String("PhoneBook", entrytext, buf);
    }

    unknown_ini_839740.Save(file, false);
}


// void UnknownOptions_00670730::unknown_save_chat_options() -- 0x00670730
// Purpose: writes the WOL/LAN taunt and scroll-text switches (+0x1E89,
// +0x1E88, +0x1F71, +0x1F70) to [MultiPlayer] of the INI object at
// 0x00839740 and saves it to RA2.INI.
// Assumptions: the owner (a large options/session object) and the INI
// object's name are unidentified.


struct UnknownOptions_00670730 {
    char unknown_0000[0x1E88];
    bool LANTaunts;                         // +0x1E88
    bool WOLTaunts;                         // +0x1E89
    char unknown_1e8a[0x1F70 - 0x1E8A];
    bool LANScrollText;                     // +0x1F70
    bool WOLScrollText;                     // +0x1F71
    void unknown_save_chat_options();
};

void UnknownOptions_00670730::unknown_save_chat_options()
{
    RawFileClass file("RA2.INI");
    unknown_ini_839740.Put_Bool("MultiPlayer", "WOLTaunts", WOLTaunts);
    unknown_ini_839740.Put_Bool("MultiPlayer", "LANTaunts", LANTaunts);
    unknown_ini_839740.Put_Bool("MultiPlayer", "WOLScrollText", WOLScrollText);
    unknown_ini_839740.Put_Bool("MultiPlayer", "LANScrollText", LANScrollText);
    unknown_ini_839740.Save(file, false);
}


// Reads a map file's [Basic] section (players, game modes, name, official flag) from the
// first part of the file and its [Digest] from the last part (0x006707D0; Read_Scenario_Descriptions).
void __fastcall Read_Map_Header_006707d0(char const *filename, unsigned short *description, char *digest, char *modes,
                                         bool *official, int *min_players, int *max_players)
{
    CCFileClass file(filename);
    if (!file.Is_Available() || !file.Open(FILE_RIGHTS_READ)) {
        return;
    }

    int percent = 0;
    int size;
    char *head = 0;
    char *end;
    do {
        percent += 5;
        size = file.Size() * percent / 100;
        if (head) {
            delete head;
        }
        head = new char[size + 1];
        if (!head) {
            return;
        }
        file.Seek(0, 0);
        file.Read(head, size);
        head[size] = '\0';
        char *start = strstr(head, "[Basic]");
        if (start) {
            end = strstr(start + 5, "\n[");
            if (end > start) {
                break;
            }
        }
        if (percent >= 100) {
            delete head;
            return;
        }
    } while (true);
    end[1] = '\0';
    Debug_Printf("Read first %d%% of file looking for [Basic] section\n", percent);

    percent = 0;
    char *tail = 0;
    int length;
    do {
        percent++;
        length = file.Size() * percent / 100;
        if (tail) {
            delete tail;
        }
        tail = new char[length + 1];
        if (!tail) {
            return;
        }
        file.Seek(-length, 2);
        file.Read(tail, length);
        tail[length] = '\0';
        if (strstr(tail, "[Digest]")) {
            break;
        }
        if (percent >= 100) {
            delete head;
            delete tail;
            return;
        }
    } while (true);
    Debug_Printf("Read last  %d%% of file looking for [Digest] section\n", percent);

    CCINIClass ini;
    BufferStraw straw(head, end - head);
    ini.Load(straw, false, false);
    CCINIClass digest_ini;
    BufferStraw digest_straw(tail, strlen(tail) + 1);
    digest_ini.Load(digest_straw, false, false);

    int min = ini.Get_Int("Basic", "MinPlayers", *min_players);
    *min_players = min;
    int max = ini.Get_Int("Basic", "MaxPlayers", *max_players);
    *max_players = max;
    ini.Get_String("Basic", "GameMode", "", modes, 0x100);
    char name[0x40];
    ini.Get_String("Basic", "Name", "No Name", name, 0x40);
    Char_To_Wide_006fa0c0(description, name, -1);
    digest_ini.Get_String("Digest", "1", "No Digest", digest, 0x20);
    *official = ini.Get_Bool("Basic", "Official", false);
    Debug_Printf("MPR %s (%d/%d): official: %d, desc: %s\n", filename, *min_players, *max_players, *official, name);
    Debug_Printf("\t\tGameModes: %s\n", modes);
    delete head;
    delete tail;
}


// Read_Scenario_Descriptions00670C50: register maps from packet lists, archives and loose files.
extern "C" wchar_t *__cdecl wcsncat(wchar_t *, wchar_t const *, unsigned int);
void __fastcall Char_To_Wide_006fa0c0(wchar_t *buffer, char const *text, int length);
void __fastcall Read_Map_Header_006707d0(char const *, unsigned short *, char *, char *, bool *, int *, int *);
extern "C" char const session_pkt_ext[];
extern "C" wchar_t const session_range_format[];
extern "C" wchar_t const session_single_format[];
extern "C" wchar_t const session_space[];
extern "C" char const session_empty[];
extern "C" char const session_basic[];
extern "C" char const session_name[];
extern "C" char const session_no_name[];
extern "C" char const session_digest[];
extern "C" char const session_one[];
extern "C" char const session_no_digest[];
extern "C" char const session_official[];
extern "C" char const session_no_description[];
extern "C" char const session_source_file[];

// Copies a description (at most 43 characters; null: MSG:NoDescription) into Description (0x00671FA0).
void MultiMission::Set_Description(unsigned short const *text)
{
    if (text != 0) {
        wcsncpy(Description, text, 44);
        Description[43] = 0;
        return;
    }
    wcscpy(Description, StringTable::Load_String(session_no_description, 0, session_source_file, 2826));
}

void SessionClass::Read_Scenario_Descriptions()
{
    INIClass ini;
    char name_buffer[64];
    char digest_buffer[32];
    wchar_t description[44];
    WIN32_FIND_DATA block;
    wchar_t player_text[64];
    wchar_t wide_name[64];
    char packet_name[260];
    char modes[256];
    int min_players;
    int max_players;

    Scenarios.Clear();
    CCFileClass file("MISSIONS.PKT");
    if (file.Is_Available()) {
        ini.Load(file, false);
        int count = ini.Entry_Count("MultiMaps");
        for (int index = 0; index < count; index++) {
            if (ini.Get_String("MultiMaps", ini.Get_Entry("MultiMaps", index), session_empty, name_buffer, sizeof(name_buffer))) {
                Scenarios.Add(new MultiMission(ini, name_buffer));
            }
        }
    }
    ini.Clear();
    file.Close();
    HANDLE handle = FindFirstFileA("*.PKT", &block);
    if (handle != (HANDLE)-1) {
        do {
            if (!(block.dwFileAttributes & 0x116)) {
                char const *filename = block.cAlternateFileName;
                if (strlen(filename) == 0) filename = block.cFileName;
                if (_stricmp(filename, "MISSIONS.PKT")) {
                    file.Set_Name(filename);
                    ini.Load(file, false);
                    int count = ini.Entry_Count("MultiMaps");
                    for (int index = 0; index < count; index++) {
                        if (ini.Get_String("MultiMaps", ini.Get_Entry("MultiMaps", index), session_empty, name_buffer, sizeof(name_buffer))) {
                            Scenarios.Add(new MultiMission(ini, name_buffer));
                        }
                    }
                }
            }
        } while (FindNextFileA(handle, &block));
        FindClose(handle);
    }
    ini.Clear();
    file.Close();
    handle = FindFirstFileA("*.MMX", &block);
    int required_cd = CD::RequiredCD;
    CD::Set_Required_CD(-2);
    if (handle != (HANDLE)-1) {
        do {
            if (!(block.dwFileAttributes & 0x116)) {
                char const *filename = block.cAlternateFileName;
                if (strlen(filename) == 0) filename = block.cFileName;
                if (_stricmp(filename, "MISSIONS.MMX")) {
                    MixFileClass *archive = new MixFileClass(filename, &FastKey);
                    if (archive != 0) {
                        unknown_mixes_837480.Add(archive);
                        int length = strlen(filename);
                        strncpy(packet_name, filename, length - 3);
                        packet_name[length - 3] = 0;
                        strcat(packet_name, session_pkt_ext);
                        packet_name[length] = 0;
                        file.Set_Name(packet_name);
                        if (file.Is_Available()) {
                            ini.Load(file, false);
                            int count = ini.Entry_Count("MultiMaps");
                            for (int index = 0; index < count; index++) {
                                if (ini.Get_String("MultiMaps", ini.Get_Entry("MultiMaps", index), session_empty, name_buffer, sizeof(name_buffer))) {
                                    MultiMission *mission = new MultiMission(ini, name_buffer);
                                    if (mission != 0) {
                                        if (mission->Get_Min_Players() != mission->Get_Max_Players()) {
                                            swprintf(player_text, session_range_format, '(', mission->Get_Min_Players(), mission->Get_Max_Players(), ')');
                                        } else {
                                            swprintf(player_text, session_single_format, '(', mission->Get_Min_Players(), ')');
                                        }
                                        int used = wcslen(mission->Description);
                                        wcscpy(description, mission->Description);
                                        wcsncat(description, session_space, 44 - used);
                                        int &used_count = used;
                                        ++used_count;
                                        wcsncat(description, player_text, 44 - used);
                                        mission->Set_Description(description);
                                        Scenarios.Add(mission);
                                    }
                                }
                            }
                            ini.Clear();
                        } else {
                            Debug_Printf("Can't see .pkt inside .mmx!\n");
                        }
                    }
                }
            }
        } while (FindNextFileA(handle, &block));
    }
    CD::Set_Required_CD(required_cd);
    if (handle != (HANDLE)-1) FindClose(handle);
    ini.Clear();
    file.Close();
    handle = FindFirstFileA("*.MPR", &block);
    bool official = false;
    if (handle != (HANDLE)-1) {
        do {
            if (!(block.dwFileAttributes & 0x116)) {
                char const *filename = block.cAlternateFileName;
                if (strlen(filename) == 0) filename = block.cFileName;
                ini.Get_String(session_basic, session_name, session_no_name, name_buffer, sizeof(name_buffer));
                ini.Get_String(session_digest, session_one, session_no_digest, digest_buffer, sizeof(digest_buffer));
                Char_To_Wide_006fa0c0(wide_name, name_buffer, -1);
                official = ini.Get_Bool(session_basic, session_official, false);
                min_players = 2;
                max_players = 4;
                Read_Map_Header_006707d0(filename, wide_name, digest_buffer, modes, &official, &min_players, &max_players);
                Scenarios.Add(new MultiMission(filename, wide_name, digest_buffer, official, modes, min_players, max_players));
            }
        } while (FindNextFileA(handle, &block));
        FindClose(handle);
    }
}


// Red Alert's Free_Scenario_Descriptions: deletes the maps, the init strings and the phone book entries.
// Deleting a MultiMission expands its destructor (GameModes, then DiskIDs); the VectorClass<Wstring> and
// VectorClass<DiskID> vtables and their slots are emitted here.
void SessionClass::Free_Scenario_Descriptions()
{
    int i;

    for (int index = 0; index < Scenarios.Count(); index++) {
        delete Scenarios[index];
    }
    Scenarios.Clear();

    for (i = 0; i < InitStrings.Count(); i++) {
        delete [] InitStrings[i];
    }
    InitStrings.Clear();

    for (i = 0; i < PhoneBook.Count(); i++) {
        delete PhoneBook[i];
    }
    PhoneBook.Clear();
}


// The colour scheme of each player colour (0x007F3224, 9 entries).
extern char const unknown_color_schemes_7f3224[9];

int SessionClass::Unknown_006715D0(int color)
{
    if (color == -2) {
        color = 8;
    }
    if ((unsigned)color < 9) {
        return unknown_color_schemes_7f3224[color];
    }
    return color;
}


unsigned int Query_Free_Disk_Kilobytes_00482c80();
extern "C" char *__cdecl getenv(char const *name);

unsigned long SessionClass::Compute_Unique_ID()
{
    unsigned long id = timeGetTime();
    unsigned long free = Query_Free_Disk_Kilobytes_00482c80();
    Add_CRC(&id, free);
    char *path = getenv("PATH");
    if (path != 0) {
        for (unsigned i = 0; i < strlen(path); i++) {
            Add_CRC(&id, path[i]);
        }
    }
    return id;
}


MultiMission::MultiMission(INIClass &ini, char const *section)
{
    Description[0] = 0;
    unknown_58[sizeof(unknown_58) - 1] = '\0';
    strcpy(unknown_15c, "No Digest");
    unknown_17c = true;
    unknown_180 = 2;
    unknown_184 = 4;
    DiskIDs.Clear();
    if (!section) {
        return;
    }
    strcpy(unknown_58, section);
    strcat(unknown_58, ".MAP");
    if (ini.Is_Present(section, "DescriptionText")) {
        char text[0x2C];
        ini.Get_String(section, "DescriptionText", "", text, 0x2C);
        Char_To_Wide_006fa0c0(Description, text, -1);
    } else {
        ini.Get_Wide_String(section, "Description", Description, 0x2C);
    }
    unknown_180 = ini.Get_Int(section, "MinPlayers", unknown_180);
    unknown_184 = ini.Get_Int(section, "MaxPlayers", unknown_184);
    char disks[0x80];
    if (ini.Get_String(section, "CD", "", disks, 0x80)) {
        char *token = strtok(disks, ",");
        while (token && *token) {
            DiskIDs.Add((DiskID)atoi(token));
            token = strtok(0, ",");
        }
    }
    Wstring modes;
    bool found = ini.Get_String(section, "GameMode", "", modes) > 0;
    if (found) {
        Wstring token;
        int position = modes.Get_Token(0, ",", token);
        while (position != -1) {
            token.Trim_Leading(" ");
            token.Trim_Trailing(" ");
            GameModes.Add(token);
            position = modes.Get_Token(position, ",", token);
        }
    }
    CCFileClass file(unknown_58);
    if (!file.Is_Available()) {
        Debug_Printf("Unable to locate scenario %s - No digest info will be loaded\n", unknown_58);
    } else {
        if (!file.Open(FILE_RIGHTS_READ)) {
            return;
        }
        file.Seek(-100, 2);
        char tail[0x80];
        memset(tail, 0, sizeof(tail));
        file.Read(tail, 0x64);
        file.Close();
        if (strstr(tail, "[Digest]")) {
            CCINIClass digest_ini;
            BufferStraw straw(tail, 0x64);
            digest_ini.Load(straw, false, false);
            digest_ini.Get_String("Digest", "1", 0, tail, 0x80);
            if (tail) {
                strncpy(unknown_15c, tail, 0x20);
                unknown_15c[0x1F] = '\0';
            } else {
                strcpy(unknown_15c, "No Digest");
            }
        }
    }
}


extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);

MultiMission::MultiMission(char const *filename, unsigned short const *description, char const *digest, bool official,
                           char const *modes, int unknown_180, int players)
{
    if (filename) {
        strncpy(unknown_58, filename, sizeof(unknown_58));
        unknown_58[sizeof(unknown_58) - 1] = '\0';
    } else {
        strcpy(unknown_58, "No File Name");
    }
    if (description) {
        wcsncpy(Description, description, 0x2C);
        Description[0x2B] = 0;
    } else {
        wcscpy(Description, StringTable::Load_String("MSG:NoDescription", 0, "D:\\Ra2\\Session.CPP", 0xb0a));
    }
    if (digest) {
        strncpy(unknown_15c, digest, sizeof(unknown_15c));
        unknown_15c[sizeof(unknown_15c) - 1] = '\0';
    } else {
        strcpy(unknown_15c, "No Digest");
    }
    unknown_17c = official;
    this->unknown_180 = unknown_180;
    unknown_184 = players;
    DiskIDs.Clear();
    DiskIDs.Add((DiskID)-2);
    if (modes) {
        Wstring list(modes);
        if (list.Length() > 0) {
            Wstring token;
            int position = list.Get_Token(0, ",", token);
            while (position != -1) {
                token.Trim_Leading(" ");
                token.Trim_Trailing(" ");
                GameModes.Add(token);
                position = list.Get_Token(position, ",", token);
            }
        }
    }
}


bool MultiMission::Unknown_00671F00(int cd)
{
    if (cd == -2) {
        return RawFileClass(unknown_58).Is_Available();
    }
    if (Is_On_Disk(DiskIDs, (DiskID)-1)) {
        return true;
    }
    if (Is_On_Disk(DiskIDs, (DiskID)cd)) {
        return true;
    }
    return false;
}


// Red Alert's MultiMission::Set_Filename.
void MultiMission::Set_Filename(char const *filename)
{
    if (filename) {
        strncpy(unknown_58, filename, sizeof(unknown_58));
        unknown_58[sizeof(unknown_58) - 1] = '\0';
    } else {
        strcpy(unknown_58, "No File Name");
    }
}


void MultiMission::Set_Digest(char const *digest)
{
    if (digest) {
        strncpy(unknown_15c, digest, sizeof(unknown_15c));
        unknown_15c[sizeof(unknown_15c) - 1] = '\0';
    } else {
        strcpy(unknown_15c, "No Digest");
    }
}


void MultiMission::Set_Official(bool official)
{
    unknown_17c = official;
}


// Whether the map allows the game mode named: with no GameModes listed only "standard" is allowed.
bool MultiMission::Unknown_006720E0(Wstring &mode)
{
    if (GameModes.Count() == 0) {
        return mode == "standard";
    }
    for (int index = 0; index < GameModes.Count(); index++) {
        if (mode == GameModes[index]) {
            return true;
        }
    }
    return false;
}


// Reports the local loading progress: the progress screen's own bar (half of it while unknown_223b is set), then,
// with other players, a 0x0F global packet (IPX; guaranteed to each player once at 99.95%) or a 0x6C serial packet,
// waiting up to 240 ticks while more than five packets are queued.
void SessionClass::Loading_Progress_00672160(int percent)
{
    if (Debug_Map) {
        return;
    }
    int progress = percent;
    if (unknown_id_source_a3d290->unknown_223b) {
        progress = progress / 2;
    }
    if (ProgressScreen.Get_Progress(0) * 100.0 < progress) {
        ProgressScreen.Set_Progress_006201c0(0, progress, Point2D(-1, -1));
    }
    if (Players.Count() == 1) {
        return;
    }
    CDTimerClass<SystemTimerClass> timeout(240);
    switch (Type) {
    case 3:
    case 4: {
        UnknownGlobalPacket packet;
        memset(&packet, 0, sizeof(packet));
        packet.Command = 0x0F;
        packet.Progress.Percent = (int)(ProgressScreen.Get_Progress(0) * 100.0);
        strcpy(packet.Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name));
        if (packet.Progress.Percent < 99.95) {
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x33, 0, 0, 0, 0);
        } else {
            for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
                Debug_Printf("Sending guarenteed progress message to all players\n");
                unknown_ipx_manager_a40978.unknown_5259a0(&packet, 0x33, 1, unknown_players_a3fa7c[i]->Name);
            }
        }
        unknown_481fb0();
        while (unknown_ipx_manager_a40978.Global_Num_Send() > 5 && timeout > 0) {
            Sleep(20);
            Unknown_005B5EE0();
            unknown_481fb0();
        }
        break;
    }
    case 1:
    case 2: {
        UnknownSerialPacket packet;
        memset(&packet, 0, sizeof(packet));
        packet.Command = 0x6C;
        packet.Progress = (int)(ProgressScreen.Get_Progress(0) * 100.0);
        packet.ID = (unsigned char)serial_local_player_00a3fb38;
        serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 0);
        serial_manager_00a40a28.Service();
        while (serial_manager_00a40a28.Num_Send() > 5 && timeout > 0) {
            Sleep(20);
            WWKeyboard->Check();
            serial_manager_00a40a28.Service();
        }
        break;
    }
    }
    unknown_481fb0();
}


// The squad lists' constructors expand over the out-of-line VectorClass constructor (0x00674BA0).
template <>
VectorClass<HouseClass *>::VectorClass(unsigned size, HouseClass *const *array);
template <>
inline DynamicVectorClass<HouseClass *>::DynamicVectorClass(unsigned size, HouseClass *const *array)
    : VectorClass<HouseClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}

// Makes each squad's members allies of one another (game mode 4, after the load wait of 0x0065C7D0).
void SessionClass::Unknown_00672440()
{
    unknown_638 = false;
    if (Type != 4) {
        return;
    }
    unknown_id_source_a3d290->Special().FixedAlliance = false;
    Debug_Printf("Squad 1 = %s\n", unknown_squad_1_b2a820);
    Debug_Printf("Squad 2 = %s\n", unknown_squad_2_b293d8);
    if (strlen(unknown_squad_1_b2a820) == 0 || strlen(unknown_squad_2_b293d8) == 0) {
        return;
    }
    int i;
    int j;
    char *name;
    DynamicVectorClass<HouseClass *> squad_1;
    DynamicVectorClass<HouseClass *> squad_2;
    char *names = new char[(strlen(unknown_squad_1_b2a820) > strlen(unknown_squad_2_b293d8) ? strlen(unknown_squad_1_b2a820) : strlen(unknown_squad_2_b293d8)) + 0x20];
    strcpy(names, unknown_squad_1_b2a820);
    for (name = strtok(names, ","); name && *name; name = strtok(0, ",")) {
        for (i = 0; i < Houses.Count(); i++) {
            HouseClass *house = Houses[i];
            if (house->unknown_134 && _stricmp(house->IniName, name) == 0) {
                squad_1.Add(house);
            }
        }
    }
    strcpy(names, unknown_squad_2_b293d8);
    for (name = strtok(names, ","); name && *name; name = strtok(0, ",")) {
        for (i = 0; i < Houses.Count(); i++) {
            HouseClass *house = Houses[i];
            if (house->unknown_134 && _stricmp(house->IniName, name) == 0) {
                squad_2.Add(house);
            }
        }
    }
    if (squad_1.Count() > 1) {
        for (i = 0; i < squad_1.Count(); i++) {
            for (j = 0; j < squad_1.Count(); j++) {
                if (squad_1[i]->ArrayIndex != squad_1[j]->ArrayIndex) {
                    squad_1[i]->Make_Ally(squad_1[j], false);
                }
            }
        }
    }
    if (squad_2.Count() > 1) {
        for (i = 0; i < squad_2.Count(); i++) {
            for (j = 0; j < squad_2.Count(); j++) {
                if (squad_2[i]->ArrayIndex != squad_2[j]->ArrayIndex) {
                    squad_2[i]->Make_Ally(squad_2[j], false);
                }
            }
        }
    }
    delete names;
    unknown_638 = true;
    unknown_id_source_a3d290->Special().FixedAlliance = true;
}


// Whether a session player's colour or one of the eight computer colours equals color; -2 (random) never is
// (expanded into 0x00672980).
bool SessionClass::Unknown_006728D0(int color)
{
    if (color != -2) {
        int i;
        for (i = 0; i < Players.Count(); i++) {
            if (Players[i]->Get_Color_0066e2b0() == color) {
                return true;
            }
        }
        for (i = 0; i < 8; i++) {
            if (AIColor[i] == color) {
                return true;
            }
        }
    }
    return false;
}


class Leaf_6729f0 { public: unsigned int get(); };
unsigned int Leaf_6729f0::get() { unsigned int session_address=(unsigned int)this, result=0, sentinel_value=0; unsigned char unused_byte=0; sentinel_value = *(unsigned int*)(session_address + 0x128); result = 0xfffffffe; if (sentinel_value == result) goto L672a05; result = *(unsigned int*)(session_address + 0x124); L672a05:; return result; }


class Leaf_672a10 { public: unsigned int get(); };
unsigned int Leaf_672a10::get() { unsigned int session_address=(unsigned int)this, result=0, sentinel_value=0; unsigned char unused_byte=0; sentinel_value = *(unsigned int*)(session_address + 0x120); result = 0xfffffffe; if (sentinel_value == result) goto L672a25; result = *(unsigned int*)(session_address + 0x11c); L672a25:; return result; }


// A random HouseTypes index of a country that is Multiplay and not MultiplayPassive (expanded into 0x00672A30).
int SessionClass::Unknown_00672940()
{
    int last = HouseTypes.Count() - 1;
    int index;
    do {
        index = unknown_id_source_a3d290->Random()(0, last);
    } while (!HouseTypes[index]->Multiplay || HouseTypes[index]->MultiplayPassive);
    return index;
}


// Draws random colours (0-7) from the scenario's generator until one is not in use and returns it (expanded
// into 0x00672AE0).
int SessionClass::Unknown_00672980()
{
    int color;
    do {
        color = unknown_id_source_a3d290->Random()(0, 7);
    } while (Unknown_006728D0(color));
    return color;
}


// Sets the player's country (+0x134/+0x138; -2: random, a Multiplay country when update is set, else 0) and
// copies it to +0x124/+0x128.
void SessionClass::Set_Value_00672a30(int value, bool update)
{
    if (value == -2) {
        if (update) {
            unknown_134 = Unknown_00672940();
        } else {
            unknown_134 = 0;
        }
        unknown_138 = -2;
    } else {
        unknown_134 = value;
        unknown_138 = -1;
    }
    unknown_124 = unknown_134;
    unknown_128 = unknown_138;
}


// Sets the player's colour (+0x12C/+0x130; -2: random, an unused colour when update is set, else 0) and
// copies it to +0x11C/+0x120.
void SessionClass::Set_Value_00672ae0(int value, bool update)
{
    if (value == -2) {
        if (update) {
            unknown_12c = Unknown_00672980();
        } else {
            unknown_12c = 0;
        }
        unknown_130 = -2;
    } else {
        unknown_12c = value;
        unknown_130 = -1;
    }
    unknown_11c = unknown_12c;
    unknown_120 = unknown_130;
}


class Leaf_6715c0 { public: void get(); };
void Leaf_6715c0::get() {  }


// 0x006720c0: function not yet identified
//
// Body: 23 bytes (frozen inventory bound).
// Referenced from 7 sites (unconditional call).
// Ghidra prototype: bool __fastcall FUN_006720c0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006720c0 {

extern unsigned int  FUN_00781f20(unsigned char *first_buffer, unsigned char *second_buffer);
extern "C" char s_RandMap_Sed_007e16e8[];

bool __fastcall FUN_006720c0(int session_address)
{
    int comparison_result;

    comparison_result = (int)(FUN_00781f20((unsigned char *)(session_address + 0x58), (unsigned char *)(s_RandMap_Sed_007e16e8)));
    return comparison_result == 0;
}

}  // namespace seed_006720c0


// 0x00672140: function not yet identified
//
// Body: 26 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: bool __fastcall FUN_00672140(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00672140 {

extern unsigned int  FUN_00781f20(unsigned char *first_buffer, unsigned char *second_buffer);
extern "C" char s_RandMap_Sed_007e16e8[];

bool __fastcall FUN_00672140(int session_address)
{
    int comparison_result;

    comparison_result = (int)(FUN_00781f20((unsigned char *)(session_address + 0x658), (unsigned char *)(s_RandMap_Sed_007e16e8)));
    return comparison_result == 0;
}

}  // namespace seed_00672140


// 0x00672810: function not yet identified
//
// Body: 30 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void __fastcall FUN_00672810(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00672810 {

void __fastcall FUN_00672810(int object_address)
{
    unsigned int *entry_words;
    int remaining;

    *(unsigned int *)(object_address + 0x24) = 0;
    entry_words = (unsigned int *)(object_address + 0x6c);
    remaining = (int)(8);
    do {
        entry_words[-8] = 0xffffffff;
        *entry_words = 0xffffffff;
        entry_words = (unsigned int *)(entry_words + 1);
        remaining = (int)(remaining + -1);
    } while (remaining != 0);
    return;
}

}  // namespace seed_00672810


// Resolves the random countries and colours (see the declaration); 0x00672940 and 0x00672980 are expanded on the
// global session.
void SessionClass::Unknown_00672BB0()
{
    int count = unknown_players_a3fa7c.Count();
    int i;
    for (i = 0; i < count; i++) {
        NodeNameType *player = unknown_players_a3fa7c[i];
        if (player->unknown_4f == -3 || player->unknown_4b == -3) {
            player->unknown_4b = -3;
            player->unknown_4f = -1;
            player->unknown_53 = 8;
            player->unknown_57 = -1;
        } else {
            if (player->unknown_4f == -2) {
                player->unknown_4f = -1;
                if (Mode) {
                    player->unknown_4b = Mode->RandomHumanCountryIndex();
                } else {
                    player->unknown_4b = unknown_session_a3d298.Unknown_00672940();
                }
            }
            if (player->unknown_57 == -2) {
                player->unknown_57 = -1;
                player->unknown_53 = -1;
                player->unknown_53 = unknown_session_a3d298.Unknown_00672980();
            }
        }
        if (i == 0) {
            unknown_a3d3b4 = player->unknown_53;
        }
    }
    for (i = 0; i < 8; i++) {
        if (unknown_session_a3d298.AICountry[i] == -2) {
            if (Mode) {
                unknown_session_a3d298.AICountry[i] = Mode->RandomAICountryIndex();
            } else {
                unknown_session_a3d298.AICountry[i] = unknown_session_a3d298.Unknown_00672940();
            }
        }
        if (unknown_session_a3d298.AIColor[i] == -2) {
            unknown_session_a3d298.AIColor[i] = unknown_session_a3d298.Unknown_00672980();
        }
    }
}


// Pauses for a dialog once (+0x3080 records it); see the declaration.
void SessionClass::Unknown_00672DE0()
{
    if (!unknown_3080 && unknown_flag_a40958) {
        unknown_3080 = true;
        if (unknown_game_speed_7f3230 == -1) {
            unknown_game_speed_7f3230 = Options.GameSpeed;
        }
        if (Type == 0) {
            Options.GameSpeed = 2;
        }
        if (!unknown_id_source_a3d290->unknown_2306) {
            Pause_Game_0065c3d0();
            unknown_ab9174 = Frame + 3;
        }
        if (!WWMouse->Is_Captured()) {
            WWMouse->Capture_Mouse();
        }
        Audio_Global_Levels[7]->Volume.Set(0x4000);
    }
}


// Undoes 0x00672DE0.
void SessionClass::Unknown_00672E70()
{
    if (unknown_3080) {
        unknown_3080 = false;
        if (unknown_game_speed_7f3230 != -1) {
            Options.GameSpeed = unknown_game_speed_7f3230;
            unknown_game_speed_7f3230 = -1;
        }
        Unknown_005173A0();
        Unknown_005A09E0();
        Audio_Stop();
        Unknown_00714410();
        Speech_Reset();
        Unknown_007160D0();
        Audio_Global_Levels[7]->Volume.Set(0x4000);
        if (ToolTips) {
            ToolTips->Enable(false);
        }
        if (WWMouse->Is_Captured()) {
            WWMouse->Release_Mouse();
        }
        WWMouse->Hide_Mouse();
        unknown_ab9174 = 0;
    }
}


// The unknown_3022 value of the session player with this ID; 0 when none has it.
unsigned short SessionClass::Unknown_00672F50(int id)
{
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        if (unknown_players_a3fa7c[index]->ID == id) {
            return unknown_3022.unknown[index];
        }
    }
    return 0;
}


// The lobby slot whose name equals this session player's name; 0 when none.
int SessionClass::Unknown_00672F90(int player)
{
    for (int slot = 0; slot < 8; slot++) {
        NodeNameType *entry = unknown_player_slots_a3fa98[slot];
        char *name = Wide_To_Narrow_006fa120(unknown_players_a3fa7c[player]->Name);
        if (_stricmp(name, Wide_To_Narrow_006fa120(entry->Name)) == 0) {
            return slot;
        }
    }
    return 0;
}


// The lobby slot whose name equals the text; 0 when none.
int SessionClass::Unknown_00672FF0(char const *name)
{
    for (int slot = 0; slot < 8; slot++) {
        if (_stricmp(name, Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[slot]->Name)) == 0) {
            return slot;
        }
    }
    return 0;
}


// The session player whose name equals this lobby slot's name; 0 when none.
int SessionClass::Unknown_00673030(int slot)
{
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        NodeNameType *entry = unknown_player_slots_a3fa98[slot];
        char *name = Wide_To_Narrow_006fa120(unknown_players_a3fa7c[index]->Name);
        if (_stricmp(name, Wide_To_Narrow_006fa120(entry->Name)) == 0) {
            return index;
        }
    }
    return 0;
}


// Not exact (RA2_NON_MATCHING): FUN_00673090 at 0x00673090, best score 83.07, 2026-11-13; differs: 538 instructions in the original, 543 in the draft (shape 97.87)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x673090 FUN_00673090 (SessionClass::Allocate_Ports)
// entry 0x673090; exact-match score 94.42 (misses_index best); this text 82.90 (codex4 r506); plan "draft, verify"
// origin lane: Scout775 (build/class-work/Scout775/keep/ap_z1_8290_loose9238.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1257 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1257): the eight 0x24-byte name rows at +0x2DD0 (header: char[0x120]) indexed by
// offset, the address port read as its first word; each changed line is marked NATIVE.
#include "shared/wol_sink.hpp"
#include "shared/wstring.hpp"
void SessionClass::Allocate_Ports_00673090()
{
    int bump = 1;
    int index;
    int other;
    int names;
    int slot;
    DynamicVectorClass<NodeNameType *> order;
    char used[8];

    Debug_Printf("Allocating port numbers and local addresses\n");
    if (Players.Count() < 2) {
        return;
    }
    names = 0;
    memset(unknown_2dd0, 0, sizeof(unknown_2dd0));
    unknown_2fc0 = true;
    for (index = 0; index < Players.Count(); index++) {
        slot = Unknown_00672F90(index);
        if (slot >= 0 && slot < 8) {
            Players[index]->Address.Set_Port(unknown_2fa0.unknown[slot]);
            char *name = Wide_To_Narrow_006fa120(Players[index]->Name);
            Debug_Printf("Allocated port for player %s - %s\n", name, Players[index]->Address.As_String());
        }
    }

    memset(used, 0, sizeof(used));
    for (index = 0; index < Players.Count(); index++) {
        int best = 0x7FFFFFFF;
        int lowest = 0x7FFFFFFF;
        for (other = 0; other < Players.Count(); other++) {
            if (!used[other] && Players[other]->unknown_53 < lowest) {
                lowest = Players[other]->unknown_53;
                best = other;
            }
        }
        if (best == 0x7FFFFFFF) {
            break;
        }
        used[best] = true;
        order.Add(Players[best]);
        if (order.Count() == Players.Count()) {
            break;
        }
    }

    memset(used, 0, sizeof(used));
    for (index = 0; index < order.Count(); index++) {
        if (used[index]) {
            continue;
        }
        IPXAddressClass address = order[index]->Address;
        for (other = 0; other < order.Count(); other++) {
            if (other != index && !used[other] && address.Is_Same_Node(order[other]->Address)) {
                int first = Unknown_00672FF0(Wide_To_Narrow_006fa120(order[other]->Name));
                int second = Unknown_00672FF0(Wide_To_Narrow_006fa120(order[index]->Name));
                if (unknown_2fa0.unknown[first] == unknown_2fa0.unknown[second]) {
                    order[other]->Address.Set_Port(unknown_2fa0.unknown[first] + bump++);
                    used[other] = true;
                    Debug_Printf("Adjusted port number to %d for player %s\n", (unsigned short)bump - 1,
                        Wide_To_Narrow_006fa120(order[other]->Name));
                } else {
                    Debug_Printf("Two player behand the same NAT but they are using different port bases anyway\n");
                }
                char *name = Wide_To_Narrow_006fa120(order[index]->Name);
                if (_stricmp(name, unknown_wstring_b26cb8.Get()) == 0) {
                    strcpy(unknown_2dd0 + 0x24 * names++, Wide_To_Narrow_006fa120(order[other]->Name));   // NATIVE: char[8][0x24] rows (codex4 r506)
                }
                name = Wide_To_Narrow_006fa120(order[other]->Name);
                if (_stricmp(name, unknown_wstring_b26cb8.Get()) == 0) {
                    strcpy(unknown_2dd0 + 0x24 * names++, Wide_To_Narrow_006fa120(order[index]->Name));   // NATIVE: char[8][0x24] rows
                }
            }
        }
    }

    if (PortNumberOverride != 0) {
        Debug_Printf("Using %d for local port\n", PortNumberOverride);
        Players[0]->Address.Set_Port(PortNumberOverride);
        PlayerSlots[Unknown_00672F90(0)]->Address.Set_Port(PortNumberOverride);
    }
    for (index = 0; index < Players.Count(); index++) {
        unknown_3022.unknown[index] = *(unsigned short *)Players[index]->Address.NetworkNumber;   // NATIVE: the UDP port word (the header accessor is guarded off in this unit)
        if (unknown_3022.unknown[index] == 0) {
            unknown_3022.unknown[index] = 1234;
        }
    }

    IPXAddressClass local = Players[0]->Address;
    NetNodeType node;
    local.Get_Node(node);
    for (index = 1; index < Players.Count(); index++) {
        if (local.Is_Same_Node(Players[index]->Address)) {
            Debug_Printf("%s has the same IP address as me\n", Wide_To_Narrow_006fa120(Players[index]->Name));
            for (slot = 0; slot < 8; slot++) {
                NodeNameType *entry = PlayerSlots[slot];
                char *name = Wide_To_Narrow_006fa120(Players[index]->Name);
                if (_stricmp(name, Wide_To_Narrow_006fa120(entry->Name)) == 0) {
                    NetNodeType nat;
                    unknown_293c.Addresses[slot].Get_Node(nat);
                    Players[index]->Address.Set_Address(0, nat, *(unsigned short *)Players[index]->Address.NetworkNumber);   // NATIVE: the UDP port word
                    name = Wide_To_Narrow_006fa120(Players[index]->Name);
                    Debug_Printf("Talking to NAT player %s on IP address %s\n", name, Players[index]->Address.As_String());
                }
            }
        }
    }

    if (PacketTransport) {
        ((UDPInterfaceClass *)PacketTransport)->Close_All_Port_Sockets();   // slot 28 (was spelled unknown_slot28)
        // behaviour fix (codex4 r506): the original calls slot 23 OpenSocket(port), not slot 15 Open_Socket.
        ((UDPInterfaceClass *)PacketTransport)->OpenSocket(unknown_3022.unknown[0]);
        PacketTransport->Start_Listening();
    }
}
#endif

// 0x00672f20: function not yet identified
//
// Body: 39 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __thiscall FUN_00672f20(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00672f20 {

struct Owner_00672f20 { void FUN_00672f20(int index_or_remaining); };

void Owner_00672f20::FUN_00672f20(int index_or_remaining)
{
    int session_address = (int)this;
    unsigned short *entry_words;

    if (index_or_remaining < 7) {
        entry_words = (unsigned short *)(session_address + 0x3022 + index_or_remaining * 2);
        index_or_remaining = (int)(7 - index_or_remaining);
        do {
            *entry_words = entry_words[1];
            entry_words = (unsigned short *)(entry_words + 1);
            index_or_remaining = (int)(index_or_remaining + -1);
        } while (index_or_remaining != 0);
    }
    return;
}

}  // namespace seed_00672f20


class Leaf_673820 { public: unsigned int get(unsigned int index); };
unsigned int Leaf_673820::get(unsigned int index) { unsigned int vector_or_index = (unsigned int)this; unsigned int buffer_or_element; unsigned int unused_value; buffer_or_element = *(unsigned int*)(vector_or_index + 4); vector_or_index = index; buffer_or_element = *(unsigned int*)(buffer_or_element + vector_or_index*4); return buffer_or_element; }


extern "C" void __cdecl release_6738d0(void*);
class Cleanup_6738d0 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_6738d0::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_6738d0(pointer); items=0; } owns=0; count=0; }


extern "C" void __cdecl release_673930(void*);
class Cleanup_673930 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_673930::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_673930(pointer); items=0; } owns=0; count=0; }


extern "C" void __cdecl release_673990(void*);
class Cleanup_673990 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_673990::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_673990(pointer); items=0; } owns=0; count=0; }


extern "C" unsigned char vtable_6739f0[]; extern "C" void __cdecl release_buffer_6739f0(void*); extern "C" void __cdecl release_object_6739f0(void*);
class Leaf_6739f0 { public: void* get(unsigned int flags); };
void* Leaf_6739f0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6739f0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6739f0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6739f0(this); return this; }


extern "C" unsigned char vtable_674790[]; extern "C" void __cdecl release_buffer_674790(void*); extern "C" void __cdecl release_object_674790(void*);
class Leaf_674790 { public: void* get(unsigned int flags); };
void* Leaf_674790::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_674790; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_674790((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_674790(this); return this; }


// VectorClass<Wstring> scalar deleting destructor (0x006747E0)
// Compiler generated (slot 0 of the VectorClass<Wstring> vtable 0x007A9644): destroys the vector, then frees.
// Emitted in this unit by Free_Scenario_Descriptions' MultiMission destructor (GameModes).


// DynamicVectorClass<Wstring> scalar deleting destructor (0x00674850)
// Compiler generated (slot 0 of the DynamicVectorClass<Wstring> vtable 0x007A9664): same bytes as VectorClass<Wstring>'s (0x006747E0).
// Emitted in this unit by MultiMission's constructor (GameModes).


extern "C" unsigned char vtable_6748c0[]; extern "C" void __cdecl release_buffer_6748c0(void*); extern "C" void __cdecl release_object_6748c0(void*);
class Leaf_6748c0 { public: void* get(unsigned int flags); };
void* Leaf_6748c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6748c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6748c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6748c0(this); return this; }


extern "C" unsigned char vtable_674910[]; extern "C" void __cdecl release_buffer_674910(void*); extern "C" void __cdecl release_object_674910(void*);
class Leaf_674910 { public: void* get(unsigned int flags); };
void* Leaf_674910::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_674910; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_674910((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_674910(this); return this; }


extern "C" unsigned char vtable_674960[]; extern "C" void __cdecl release_buffer_674960(void*); extern "C" void __cdecl release_object_674960(void*);
class Leaf_674960 { public: void* get(unsigned int flags); };
void* Leaf_674960::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_674960; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_674960((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_674960(this); return this; }


extern "C" unsigned char vtable_6749b0[]; extern "C" void __cdecl release_buffer_6749b0(void*); extern "C" void __cdecl release_object_6749b0(void*);
class Leaf_6749b0 { public: void* get(unsigned int flags); };
void* Leaf_6749b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6749b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6749b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6749b0(this); return this; }


extern "C" unsigned char vtable_674a00[]; extern "C" void __cdecl release_buffer_674a00(void*); extern "C" void __cdecl release_object_674a00(void*);
class Leaf_674a00 { public: void* get(unsigned int flags); };
void* Leaf_674a00::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_674a00; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_674a00((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_674a00(this); return this; }


extern "C" unsigned char vtable_674a50[]; extern "C" void __cdecl release_buffer_674a50(void*); extern "C" void __cdecl release_object_674a50(void*);
class Leaf_674a50 { public: void* get(unsigned int flags); };
void* Leaf_674a50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_674a50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_674a50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_674a50(this); return this; }


// VectorClass<Wstring>::VectorClass(unsigned size, Wstring const *array) (0x00674AF0): the out-of-line copy MultiMission's
// constructor calls for GameModes (the header body, as an explicit specialization declared in that draft).
template <>
VectorClass<Wstring>::VectorClass(unsigned size, Wstring const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) Wstring[size];
        } else {
            Vector = new Wstring[size];
            IsAllocated = true;
        }
    }
}


extern "C" unsigned char table_674ba0[]; extern "C" void* __cdecl storage_674ba0(unsigned int);
class Storage_674ba0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_674ba0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_674ba0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_674ba0(requested << 2); owns=1; } return this; }
