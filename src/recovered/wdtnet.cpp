// wdtnet.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtnet.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_ASSIGN
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CLEAR
#define VECTOR_IMPLEMENT_VECTOR_CLEAR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define WDT_STATE_DECLARE_CONSTRUCTORS
#define RA2_VECTOR_SIGNED_SIZE
#include "shared/wdt_territory.hpp"
#include "shared/wdt_state.hpp"
#include "shared/packet.hpp"
#include "shared/crt_memory.hpp"


// rc_ptr<WDTTerritory> __fastcall WDTTerritory::Create() (0x0072BDC0): a new territory, counted.
// First source of the unit: the vector members the WDTState constructor and destructor expand, and
// the guarded WDTState declarations (shared/wdt_state.hpp), are defined here before any include.
// TibSun vector.h signed VectorClass size (the WDTState() 0x0072BEF0 temporary expands inline).

rc_ptr<WDTTerritory> __fastcall WDTTerritory::Create()
{
    return rc_ptr<WDTTerritory>(new WDTTerritory());
}


// WDTTerritory::WDTTerritory() (0x0072BE00): the default battle settings (10 units, tech level
// 10, two players, eight 0/100/50 byte triples from +0x1F with the one at +0x2B enabled, no
// option flags chosen). The bytes from +0x15 keep the shared header's unknown_15 view (the map
// seed reads +0x1C-+0x24 through it, MapSeedClass::Apply_WDT_Territory 0x00578BC0).

WDTTerritory::WDTTerritory()
{
    unknown_08 = 0;
    UnitCount = 10;
    TechLevel = 10;
    unknown_10 = 10000;
    *(unsigned short *)unknown_0c = 0;
    unknown_12 = 1;
    unknown_13 = 1;
    PlayerCount = 2;
    unknown_15[0x1C - 0x15] = 1;
    unknown_15[0x1D - 0x15] = 0;
    unknown_15[0x1E - 0x15] = 0;
    unknown_15[0x1F - 0x15] = 0;
    unknown_15[0x20 - 0x15] = 100;
    unknown_15[0x21 - 0x15] = 50;
    unknown_15[0x22 - 0x15] = 0;
    unknown_15[0x23 - 0x15] = 100;
    unknown_15[0x24 - 0x15] = 50;
    unknown_15[0x25 - 0x15] = 0;
    unknown_15[0x26 - 0x15] = 100;
    unknown_15[0x27 - 0x15] = 50;
    unknown_15[0x28 - 0x15] = 0;
    unknown_15[0x29 - 0x15] = 100;
    unknown_15[0x2A - 0x15] = 50;
    unknown_15[0x2B - 0x15] = 1;
    unknown_15[0x2C - 0x15] = 100;
    unknown_15[0x2D - 0x15] = 50;
    unknown_15[0x2E - 0x15] = 0;
    unknown_15[0x2F - 0x15] = 100;
    unknown_15[0x30 - 0x15] = 50;
    unknown_15[0x31 - 0x15] = 0;
    unknown_15[0x32 - 0x15] = 100;
    unknown_15[0x33 - 0x15] = 50;
    unknown_15[0x34 - 0x15] = 0;
    unknown_15[0x35 - 0x15] = 100;
    unknown_15[0x36 - 0x15] = 50;
    OptionFlags = -1;
    unknown_3c = -1;
}


// 0x0072bea0: function not yet identified
//
// Owner: WDTTerritory, stores the WDTTerritory vtable (constructor or destructor) (from RTTI/vtables).
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_0072bea0(undefined4 *param_1)
// Callees with accepted source:
//   ReferenceCounted_Destructor_00639710 -> src/recovered/classes/ReferenceCounted/00639710.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072bea0 {

extern void __fastcall ReferenceCounted_Destructor_00639710(void);
extern "C" void* PTR_FUN_007af2d8;

void __fastcall FUN_0072bea0(unsigned int *object_words)
{
    *object_words = (unsigned int)(&PTR_FUN_007af2d8);
    ReferenceCounted_Destructor_00639710();
    return;
}

}  // namespace seed_0072bea0


// rc_ptr<WDTState> __fastcall WDTState::Create(unsigned char *data) (0x0072BEB0): a new state
// read from the WOL state block, counted.

rc_ptr<WDTState> __fastcall WDTState::Create(unsigned char *data)
{
    return rc_ptr<WDTState>(new WDTState(data));
}


// WDTState::WDTState() (0x0072BEF0): an empty state (map 0, cycle 0, a 1,000,000 tick time, no
// territories). The territory list is reset by assigning an empty temporary: the vector's
// assignment is expanded and calls VectorClass::operator= 0x0072CAC0, which needs the unit's
// RA2_VECTOR_SIGNED_SIZE profile (defined in WDTTerritory::Create 0x0072BDC0).
// From ra2-decomp 0x0076ABE0 (YR identical twin, src/wdtnet.cpp WDTState::WDTState):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

WDTState::WDTState()
{
    unknown_14 = 0;
    unknown_18 = 0;
    unknown_20 = 0;
    ConflictCount = 0;
    unknown_28 = 1000000;
    Territories = DynamicVectorClass<rc_ptr<WDTTerritory> >();
    unknown_44 = 0;
}


// WDTState::WDTState(unsigned char *data) (0x0072BFA0): a state read from the WOL state packet:
// NACK leaves it empty; TOTK (or CRTK/CYLN) gives the conflict counts, SDSC/LDSC/WURL the label
// strings, CMID/CYID/TICC the numbers; MPVC (current cycles only) and HSVC are parsed by
// unknown_72c490 / unknown_72c3b0. The field names are the packet tags.

WDTState::WDTState(unsigned char *data)
{
    PacketClass *packet = new PacketClass((char *)data);
    int mpvc_length = 0;
    int hsvc_length = 0;
    int sdsc_length = 0;
    int ldsc_length = 0;
    int wurl_length = 0;
    int nack_length = 0;
    ShortLabel = 0;
    LongLabel = 0;
    unknown_10 = 0;
    unknown_14 = 0;
    unknown_18 = 0;
    unknown_1c = 0;
    unknown_20 = 0;
    ConflictCount = 0;
    unknown_28 = 0;
    unknown_44 = 0;
    if (packet->Get_Field("NACK", 0, nack_length)) {
        delete packet;
        return;
    }
    if (packet->Get_Field("TOTK", unknown_1c)) {
        unknown_48 = true;
        unknown_1c++;
        ConflictCount = unknown_1c;
    } else {
        unknown_48 = false;
        packet->Get_Field("CRTK", ConflictCount);
        ConflictCount++;
        packet->Get_Field("CYLN", unknown_1c);
        unknown_1c++;
    }
    packet->Get_Field("WURL", 0, wurl_length);
    packet->Get_Field("SDSC", 0, sdsc_length);
    packet->Get_Field("LDSC", 0, ldsc_length);
    if (!unknown_48) {
        packet->Get_Field("MPVC", 0, mpvc_length);
    }
    packet->Get_Field("HSVC", 0, hsvc_length);
    ShortLabel = new char[sdsc_length];
    LongLabel = new char[ldsc_length];
    unknown_10 = new char[wurl_length];
    char *hsvc = new char[hsvc_length];
    char *mpvc = 0;
    if (!unknown_48) {
        mpvc = new char[mpvc_length];
        memset(mpvc, 0, mpvc_length);
    }
    memset(ShortLabel, 0, sdsc_length);
    memset(LongLabel, 0, ldsc_length);
    memset(unknown_10, 0, wurl_length);
    memset(hsvc, 0, hsvc_length);
    packet->Get_Field("CMID", unknown_14);
    packet->Get_Field("CYID", unknown_18);
    if (unknown_48) {
        unknown_28 = 0;
    } else {
        packet->Get_Field("TICC", unknown_28);
    }
    packet->Get_Field("SDSC", ShortLabel, sdsc_length);
    packet->Get_Field("LDSC", LongLabel, ldsc_length);
    packet->Get_Field("WURL", unknown_10, wurl_length);
    if (!unknown_48) {
        packet->Get_Field("MPVC", mpvc, mpvc_length);
    }
    packet->Get_Field("HSVC", hsvc, hsvc_length);
    if (!unknown_48) {
        unknown_72c490(mpvc, mpvc_length);
    } else {
        unknown_20 = 30;
    }
    unknown_72c3b0(hsvc, hsvc_length);
    delete hsvc;
    if (!unknown_48) {
        delete mpvc;
    }
    delete packet;
}


// WDTState::~WDTState() (0x0072C2D0, slot 0 body)
// Clears the territory list, frees the three label strings and the owned string table (ConflictCount entries), then the list storage and the ReferenceCounted base.

WDTState::~WDTState()
{
    Territories.Clear();
    if (LongLabel != 0) {
        delete LongLabel;
    }
    if (ShortLabel != 0) {
        delete ShortLabel;
    }
    if (unknown_10 != 0) {
        delete unknown_10;
    }
    if (unknown_44 != 0) {
        for (unsigned i = 0; (int)i < (int)ConflictCount; i++) {
            if (unknown_44[i] != 0) {
                delete (char *)unknown_44[i];
            }
        }
        delete unknown_44;
    }
}


// bool UnknownOwner_0072C390::unknown_has_44() const -- 0x0072C390
// Purpose: whether the word at +0x44 is set.
// Assumptions: owner unidentified.

struct UnknownOwner_0072c390 {
    char unknown_00[0x44];
    int unknown_44;                         // +0x44
    bool unknown_has_44() const;
};

bool UnknownOwner_0072c390::unknown_has_44() const
{
    return unknown_44 != 0 ? true : false;
}


class Leaf_72c3a0 { public: unsigned char get(); };
unsigned char Leaf_72c3a0::get() { return *(unsigned char*)((char*)this + 0x48); }


// bool WDTState::unknown_72c3b0(void *data, int length) (0x0072C3B0): the HSVC field of the WOL
// state packet, four 2-bit values per byte (low bits first): one byte table of unknown_20 values
// per conflict in unknown_44 (ConflictCount rows), each row starting on a fresh byte. Fails when
// the field holds fewer than unknown_20 * ConflictCount values. Each switch case stores its value
// itself (a shared value local enregisters the bit counter, which the original keeps in memory).

bool WDTState::unknown_72c3b0(void *data, int length)
{
    if ((unsigned)(length * 4) < unknown_20 * ConflictCount) {
        return false;
    }
    unknown_44 = new void *[ConflictCount];
    for (unsigned i = 0; i < ConflictCount; i++) {
        unknown_44[i] = new unsigned char[unknown_20];
    }
    unsigned char *bytes = (unsigned char *)data;
    int index = 0;
    int pair = 0;
    for (unsigned conflict = 0; conflict < ConflictCount; conflict++) {
        for (unsigned item = 0; item < unknown_20; item++) {
            switch (pair) {
                case 0:
                    ((unsigned char *)unknown_44[conflict])[item] = bytes[index] & 3;
                    break;
                case 1:
                    ((unsigned char *)unknown_44[conflict])[item] = (bytes[index] >> 2) & 3;
                    break;
                case 2:
                    ((unsigned char *)unknown_44[conflict])[item] = (bytes[index] >> 4) & 3;
                    break;
                default:
                    ((unsigned char *)unknown_44[conflict])[item] = bytes[index] >> 6;
                    break;
            }
            pair++;
            if (pair == 4) {
                pair = 0;
                index++;
            }
        }
        if (pair != 0) {
            pair = 0;
            index++;
        }
    }
    return true;
}


// bool WDTState::unknown_72c490(void *data, int length) (0x0072C490): the MPVC field of the WOL
// state packet, one territory per record (44 bytes read, 0x1C checked). Each record makes a counted
// WDTTerritory through the factory WDTTerritory::Create 0x0072BDC0 (expanded here; its rc_ptr
// constructor stays out of line at 0x0072CAA0), numbers it with unknown_20 and appends it to
// Territories. Most bytes carry a +0x80 bias; the battle-setting triples from +0x1F only drop
// the high bit (Unbias_72c490, read in the packet's 0,1,2,4,5,3,6,7 triple order).

inline unsigned char Unbias_72c490(unsigned char const &value)
{
    return value > 0x7F ? value - 0x80 : value;
}

bool WDTState::unknown_72c490(void *data, int length)
{
    unsigned char *bytes = (unsigned char *)data;
    unknown_20 = 0;
    if (length < 0x1C) {
        return false;
    }
    for (int i = 0; i < length; ) {
        if (i + 0x1C > length) {
            return false;
        }
        rc_ptr<WDTTerritory> territory = WDTTerritory::Create();
        territory->unknown_08 = unknown_20;
        *(unsigned short *)territory->unknown_0c = bytes[i++] << 8;
        *(unsigned short *)territory->unknown_0c += bytes[i++];
        territory->UnitCount = bytes[i++] + 0x80;
        territory->TechLevel = bytes[i++] + 0x80;
        territory->unknown_10 = bytes[i++] << 8;
        territory->unknown_10 += bytes[i++];
        unsigned kind = bytes[i++] - 0x80;
        territory->unknown_15[0x1C - 0x15] = kind & 0x0F;
        territory->unknown_15[0x1D - 0x15] = (kind >> 4) & 0x0F;
        territory->unknown_15[0x1E - 0x15] = bytes[i++] + 0x80;
        territory->unknown_12 = bytes[i++] + 0x80;
        territory->unknown_13 = bytes[i++] + 0x80;
        territory->PlayerCount = bytes[i++] + 0x80;
        *(int *)&territory->unknown_15[0x18 - 0x15] = bytes[i++] - 0x80;
        territory->unknown_15[0x1F - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x20 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x21 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x22 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x23 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x24 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x25 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x26 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x27 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x2B - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x2C - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x2D - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x2E - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x2F - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x30 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x28 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x29 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x2A - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x31 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x32 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x33 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x34 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x35 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_15[0x36 - 0x15] = Unbias_72c490(bytes[i]); i++;
        territory->unknown_3c = bytes[i++] << 24;
        territory->unknown_3c += bytes[i++] << 16;
        territory->unknown_3c += bytes[i++] << 8;
        territory->unknown_3c += bytes[i++];
        territory->OptionFlags = bytes[i++] << 24;
        territory->OptionFlags += bytes[i++] << 16;
        territory->OptionFlags += bytes[i++] << 8;
        territory->OptionFlags += bytes[i++];
        Territories.Add(territory);
        unknown_20++;
    }
    return true;
}


// 0x0072c9d0: function not yet identified
//
// Owner: WDTTerritory, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_0072c9d0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   ReferenceCounted_Destructor_00639710 -> src/recovered/classes/ReferenceCounted/00639710.cpp
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072c9d0 {

extern void __fastcall ReferenceCounted_Destructor_00639710(void);
extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007af2d8;

struct Owner_0072c9d0 { unsigned int * FUN_0072c9d0(unsigned char deletion_flags); };

unsigned int * Owner_0072c9d0::FUN_0072c9d0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007af2d8);
    ReferenceCounted_Destructor_00639710();
    if ((deletion_flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_0072c9d0


extern "C" void __cdecl release_72ca00(void*);
class Leaf_72ca00 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_72ca00::get(unsigned int flags) { cleanup(); if (flags & 1) release_72ca00(this); return this; }


class Call_72caa0_0 {public: unsigned int target();};
extern "C" unsigned char data_72caa0_7a452c[];
class Call_72caa0_1 {public: unsigned int Set_00639530(unsigned int target_object);};
class Leaf_72caa0 { public: unsigned int get(unsigned int referenced_object_address); };
unsigned int Leaf_72caa0::get(unsigned int referenced_object_address) { unsigned int unused_base_return = ((Call_72caa0_0*)((unsigned int)this))->target(); *(unsigned int*)(((unsigned int)this)) = (unsigned int)data_72caa0_7a452c; unsigned int unused_set_return = ((Call_72caa0_1*)((unsigned int)this))->Set_00639530((unsigned int)(referenced_object_address)); return (unsigned int)this; }


// VectorClass<rc_ptr<WDTTerritory>> scalar deleting destructor (0x0072CED0; vtable 0x007AF2E8 slot 0):
// compiler generated with the unit's VectorClass<rc_ptr<WDTTerritory>> (shared/vector_class.hpp); no source of its own.


// DynamicVectorClass<rc_ptr<WDTTerritory>> scalar deleting destructor (0x0072CF20; vtable 0x007AF318 slot 0):
// compiler generated with the unit's DynamicVectorClass<rc_ptr<WDTTerritory>> (WDTState's constructors,
// shared/vector_class.hpp); no source of its own.


class Call_72cf70_0 {public: unsigned int target();};
extern "C" unsigned char data_72cf70_7a452c[];
class Leaf_72cf70 { public: unsigned int get(); };
unsigned int Leaf_72cf70::get() { unsigned int unused_base_return = ((Call_72cf70_0*)((unsigned int)this))->target(); *(unsigned int*)(((unsigned int)this)) = (unsigned int)data_72cf70_7a452c; return (unsigned int)this; }
