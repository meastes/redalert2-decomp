// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// UnknownSessionState::UnknownSessionState() (0x0066E380; RA2 SessionClass::SessionClass)
// Statement order from ra2-decomp SessionClass::SessionClass (YR 0x006970A0, src/session.cpp):
// MaxPlayers, NumPlayers, FrameSendRate, MaxAhead = FrameSendRate * 3, MaxMaxAhead = MaxAhead,
// LastMessage[0], WWChat, ModemService and TrapTarget.Invalidate(); TrapTarget is a TargetClass
// member (TibSun/YR), whose inline default constructor clears Kind with the other members.
#include "shared/multi_mission.hpp"
#include "shared/session_state.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"
#include "shared/unknown_geometry.hpp"
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
// @ref _session_ctor_trace_b26cbc=00b26cbc
extern "C" int session_ctor_trace_b291a4[64];
// @ref _session_ctor_trace_b291a4=00b291a4
extern "C" char const session_record_bin[];
// @ref _session_record_bin=007f33bc
extern "C" char const session_empty[];
// @ref _session_empty=0083c55c
UnknownSessionState::UnknownSessionState() : Mode(0)
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
 RecordFile.Set_Name(session_record_bin);
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

// @ref ??_7?$DynamicVectorClass@PAVMultiMission@@@@6B@=007a95a4
// @ref ??_7?$DynamicVectorClass@PAUUnknownGlobalPacket@@@@6B@=007a9584
// @ref ??_7?$DynamicVectorClass@PAVPhoneEntryClass@@@@6B@=007a9564
// @ref ??_7?$DynamicVectorClass@PBG@@6B@=007a5714
// The DynamicVectorClass vtables this constructor emits: their slots (read from the original vtables).
// @ref ??_7?$DynamicVectorClass@PAD@@6B@=0079e55c
// @ref ??_7?$DynamicVectorClass@PAUNodeNameType@@@@6B@=007a6864
// @ref ??_E?$DynamicVectorClass@PAD@@UAEPAXI@Z=00491390
// @ref ??_E?$DynamicVectorClass@PAUNodeNameType@@@@UAEPAXI@Z=005a6c90
// @ref ??_E?$DynamicVectorClass@PAUUnknownGlobalPacket@@@@UAEPAXI@Z=006749b0
// @ref ??_E?$DynamicVectorClass@PAVMultiMission@@@@UAEPAXI@Z=00674910
// @ref ??_E?$DynamicVectorClass@PAVPhoneEntryClass@@@@UAEPAXI@Z=00674a50
// @ref ??_E?$DynamicVectorClass@PBG@@UAEPAXI@Z=0053aaf0
// @ref ?Clear@?$DynamicVectorClass@PAD@@UAEXXZ=00491060
// @ref ?Clear@?$DynamicVectorClass@PAUNodeNameType@@@@UAEXXZ=005a6410
// @ref ?Clear@?$DynamicVectorClass@PAUUnknownGlobalPacket@@@@UAEXXZ=00673930
// @ref ?Clear@?$DynamicVectorClass@PAVMultiMission@@@@UAEXXZ=006738d0
// @ref ?Clear@?$DynamicVectorClass@PAVPhoneEntryClass@@@@UAEXXZ=00673990
// @ref ?Clear@?$DynamicVectorClass@PBG@@UAEXXZ=0053aba0
// @ref ?ID@?$DynamicVectorClass@PAD@@UAEHABQAD@Z=00491310
// @ref ?ID@?$DynamicVectorClass@PAD@@UAEHPBQAD@Z=00491090
// @ref ?ID@?$DynamicVectorClass@PAUNodeNameType@@@@UAEHABQAUNodeNameType@@@Z=005a6960
// @ref ?ID@?$DynamicVectorClass@PAUNodeNameType@@@@UAEHPBQAUNodeNameType@@@Z=005a6440
// @ref ?ID@?$DynamicVectorClass@PAUUnknownGlobalPacket@@@@UAEHABQAUUnknownGlobalPacket@@@Z=00674500
// @ref ?ID@?$DynamicVectorClass@PAUUnknownGlobalPacket@@@@UAEHPBQAUUnknownGlobalPacket@@@Z=00673960
// @ref ?ID@?$DynamicVectorClass@PAVMultiMission@@@@UAEHABQAVMultiMission@@@Z=006742a0
// @ref ?ID@?$DynamicVectorClass@PAVMultiMission@@@@UAEHPBQAVMultiMission@@@Z=00673900
// @ref ?ID@?$DynamicVectorClass@PAVPhoneEntryClass@@@@UAEHABQAVPhoneEntryClass@@@Z=00674760
// @ref ?ID@?$DynamicVectorClass@PAVPhoneEntryClass@@@@UAEHPBQAVPhoneEntryClass@@@Z=006739c0
// @ref ?ID@?$DynamicVectorClass@PBG@@UAEHABQBG@Z=0053ae30
// @ref ?ID@?$DynamicVectorClass@PBG@@UAEHPBQBG@Z=0053abd0
// @ref ?Resize@?$DynamicVectorClass@PAD@@UAE_NHPBQAD@Z=00491260
// @ref ?Resize@?$DynamicVectorClass@PAUNodeNameType@@@@UAE_NHPBQAUNodeNameType@@@Z=005a68b0
// @ref ?Resize@?$DynamicVectorClass@PAUUnknownGlobalPacket@@@@UAE_NHPBQAUUnknownGlobalPacket@@@Z=00674450
// @ref ?Resize@?$DynamicVectorClass@PAVMultiMission@@@@UAE_NHPBQAVMultiMission@@@Z=006741f0
// @ref ?Resize@?$DynamicVectorClass@PAVPhoneEntryClass@@@@UAE_NHPBQAVPhoneEntryClass@@@Z=006746b0
// @ref ?Resize@?$DynamicVectorClass@PBG@@UAE_NHPBQBG@Z=0053ad80
