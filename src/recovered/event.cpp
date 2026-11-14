// event.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/event.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/building_class.hpp"
#include "shared/unit.hpp"
#include "shared/cell.hpp"
#include "shared/dsurface.hpp"
#include "shared/queue.hpp"
#include "shared/session_state.hpp"
#include "shared/foot.hpp"
#include "shared/building_type_class.hpp"
#include "shared/map_view.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/rules.hpp"
#include "shared/team.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/string_table.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/load_dlg.hpp"
#include "shared/ipx_conn.hpp"
#include "shared/options.hpp"
#include "shared/id_source.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/event.hpp"
#include "shared/special.hpp"
#include "shared/ipx_address.hpp"


// Header-static dynamic initializers of the unit at 0x004B5C90:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004B5C90-0x004B5E76); the other
// drafts of this plan are emitted by these includes.
// The rest of Event.CPP's include list: EventClass::Execute (0x004B65C0) matches only when these
// come before shared/event.hpp (the order changes its inline expansions).


// Dynamic initializer (0x004B5CC0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5CE0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5D00, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5D20, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5D40, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5D60, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5DA0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5DD0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5E00, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5E30, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5E40, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// Dynamic initializer (0x004B5E60, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004b5c90.cpp; no source of its own.


// unsigned char Event_Sequence() (0x004B5E80, name hypothesis)
// The event number within the current frame: 0 on the first call of a frame, then counting up.

extern int unknown_event_frame_7d6684;                  // 0x007D6684
extern unsigned char unknown_event_counter_855b9c;      // 0x00855B9C

unsigned char Event_Sequence()
{
    if (Frame != unknown_event_frame_7d6684) {
        unknown_event_counter_855b9c = 0;
    }
    return unknown_event_counter_855b9c++;
}


// EventClass::EventClass(int house, SpecialClass data) (0x004B5EB0): event type 0x18 (SPECIAL),
// the game options word (Red Alert's EventClass(SpecialClass)).

EventClass::EventClass(int house, SpecialClass data)
{
    if (house >= 0) {
        House = house;
        Type = 0x18;
        SpecialAt(0x07) = data;
        Frame = ::Frame;
        return;
    }
    House = 0xFF;
    Type = 0;
    Frame = ::Frame;
}


// EventClass::EventClass(int house, int type, TargetClass target) (0x004B5EF0)

EventClass::EventClass(int house, int type, TargetClass target)
{
    Debug_Printf("Adding event %s\n", EventNames[(unsigned char)type]);
    if (house >= 0) {
        House = house;
        Type = type;
        TargetAt(0x07) = target;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, unsigned char type, Cell const &cell) (0x004B5F60)
// The cell at +0x07 (TibSun's SellCell shape).

EventClass::EventClass(int house, unsigned char type, Cell const &cell)
{
    Debug_Printf("Adding event %s\n", EventNames[type]);
    if (house >= 0) {
        House = house;
        Type = type;
        ShortAt(0x07) = cell.X;
        ShortAt(0x09) = cell.Y;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, unsigned char type) (0x004B5FD0)
// An event with only a house and a type (Red Alert's EventClass(EventType)).

EventClass::EventClass(int house, unsigned char type)
{
    Debug_Printf("Adding event %s\n", EventNames[type]);
    if (house >= 0) {
        House = house;
        Type = type;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, int type, unsigned long a) (0x004B6030)
// A four-byte payload at +0x07 (TibSun's EventClass(index, type, int val)).

EventClass::EventClass(int house, int type, unsigned long a)
{
    Debug_Printf("Adding event %s\n", EventNames[(unsigned char)type]);
    if (house >= 0) {
        House = house;
        Type = type;
        LongAt(0x07) = a;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, int type, TargetClass target1, TargetClass target2) (0x004B6090)

EventClass::EventClass(int house, int type, TargetClass target1, TargetClass target2)
{
    Debug_Printf("Adding event %s\n", EventNames[(unsigned char)type]);
    if (house >= 0) {
        House = house;
        Type = type;
        TargetAt(0x07) = target1;
        TargetAt(0x0c) = target2;
        Frame = ::Frame;
        return;
    }
    House = 0xFF;
    Type = 0;
    Frame = ::Frame;
}


// EventClass::EventClass(int house, int anim, int owner, CoordStruct const &coord) (0x004B6110)
// ANIMATION (type 0x14): the anim type and owner at +0x07/+0x0B, the coordinate's X and Y at
// +0x0F/+0x13 (Execute takes Z from the map). TibSun's EventClass(index, AnimType, HousesType, Coord).

EventClass::EventClass(int house, int anim, int owner, CoordStruct const &coord)
{
    Debug_Printf("Adding event ANIMATION\n");
    if (house >= 0) {
        House = house;
        Type = 0x14;
        Data.LongsCell.Long1 = anim;
        Data.LongsCell.Long2 = owner;
        Data.LongsCell.Long3 = coord.X;
        LongAt(0x13) = coord.Y;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, TargetClass whom, unsigned char mission, TargetClass target, TargetClass destination, TargetClass follow) (0x004B6170): event type 4.

EventClass::EventClass(int house, TargetClass whom, unsigned char mission, TargetClass target, TargetClass destination, TargetClass follow)
{
    if (house >= 0) {
        House = house;
        Type = 4;
        ByteAt(0x1d) = 0;
        TargetAt(0x07) = whom;
        ByteAt(0x0c) = mission;
        TargetAt(0x0e) = target;
        TargetAt(0x13) = destination;
        TargetAt(0x18) = follow;
        Frame = ::Frame;
        return;
    }
    House = 0xFF;
    Type = 0;
    Frame = ::Frame;
}


// EventClass::EventClass(int house, TargetClass whom, unsigned char mission, TargetClass target, TargetClass destination, long formation_speed, long max_speed) (0x004B61F0): event type 5.

EventClass::EventClass(int house, TargetClass whom, unsigned char mission, TargetClass target, TargetClass destination, long formation_speed, long max_speed)
{
    Debug_Printf("Adding event MEGAMISSION_F\n");
    if (house >= 0) {
        House = house;
        Type = 5;
        Data.MegaMissionF.Whom = whom;
        Data.MegaMissionF.Mission = mission;
        Data.MegaMissionF.Target = target;
        Data.MegaMissionF.Destination = destination;
        Data.MegaMissionF.Speed = formation_speed;
        Data.MegaMissionF.MaxSpeed = max_speed;
        Frame = ::Frame;
        return;
    }
    House = 0xFF;
    Type = 0;
    Frame = ::Frame;
}


// EventClass::EventClass(int house, int type, int rtti, int id, int naval) (0x004B6280)
// A production event: kind, id and naval at +0x07, +0x0B and +0x0F.

EventClass::EventClass(int house, int type, int rtti, int id, int naval)
{
    Debug_Printf("Adding event %s\n", EventNames[(unsigned char)type]);
    if (house >= 0) {
        House = house;
        Type = type;
        Data.LongsCell.Long1 = rtti;
        Data.LongsCell.Long2 = id;
        Data.LongsCell.Long3 = naval;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, int type, long a, Cell const &cell) (0x004B62F0)

EventClass::EventClass(int house, int type, long a, Cell const &cell)
{
    Debug_Printf("Adding event %s\n", EventNames[(unsigned char)type]);
    if (house >= 0) {
        House = house;
        Type = type;
        LongAt(0x07) = a;
        LongAt(0x0b) = -1;
        LongAt(0x0f) = 0;
        ShortAt(0x13) = cell.X;
        ShortAt(0x15) = cell.Y;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, int type, int a, int b, Cell const &cell) (0x004B6370)
// Two longs at +0x07 and +0x0B, 0 at +0x0F and the cell at +0x13.

EventClass::EventClass(int house, int type, int a, int b, Cell const &cell)
{
    Debug_Printf("Adding event %s\n", EventNames[(unsigned char)type]);
    if (house >= 0) {
        House = house;
        Type = type;
        Data.LongsCell.Long1 = a;
        Data.LongsCell.Long2 = b;
        Data.LongsCell.Long3 = 0;
        Data.LongsCell.Location.X = cell.X;
        Data.LongsCell.Location.Y = cell.Y;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, int type, int rtti, int id, int naval, Cell const &cell) (0x004B63F0)
// A production event with a cell (type 0x0B: place): kind, id and naval at
// +0x07, +0x0B and +0x0F, the cell at +0x13.

EventClass::EventClass(int house, int type, int rtti, int id, int naval, Cell const &cell)
{
    Debug_Printf("Adding event %s\n", EventNames[(unsigned char)type]);
    if (house >= 0) {
        House = house;
        Type = type;
        Data.LongsCell.Long1 = rtti;
        Data.LongsCell.Long2 = id;
        Data.LongsCell.Long3 = naval;
        Data.LongsCell.Location.X = cell.X;
        Data.LongsCell.Location.Y = cell.Y;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, unsigned char type, int id, Cell const &cell) (0x004B6470)
// A long at +0x07 and the cell at +0x0B (TibSun's id-and-cell shape).

EventClass::EventClass(int house, unsigned char type, int id, Cell const &cell)
{
    Debug_Printf("Adding event %s\n", EventNames[type]);
    if (house >= 0) {
        House = house;
        Type = type;
        LongAt(0x07) = id;
        ShortAt(0x0B) = cell.X;
        ShortAt(0x0D) = cell.Y;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, unsigned char type, int rtti, int id) (0x004B64F0)
// Two longs at +0x07 and +0x0B (TibSun's object-and-id shape).

EventClass::EventClass(int house, unsigned char type, int rtti, int id)
{
    Debug_Printf("Adding event %s\n", EventNames[type]);
    if (house >= 0) {
        House = house;
        Type = type;
        LongAt(0x07) = rtti;
        LongAt(0x0B) = id;
    } else {
        House = 0xFF;
        Type = 0;
    }
    Frame = ::Frame;
}


// EventClass::EventClass(int house, IPXAddressClass *address, unsigned char unknown) (0x004B6560): event type 0x29
// (ADDRESSCHANGE): the first four node bytes of the address (a UDP address's IP) and a byte.

EventClass::EventClass(int house, IPXAddressClass *address, unsigned char unknown)
{
    if (house >= 0) {
        House = house;
        Type = 0x29;
        NetNumType net;
        NetNodeType node;
        address->Get_Address(net, node);
        LongAt(0x08) = *(long *)node;
        ByteAt(0x07) = unknown;
        Frame = ::Frame;
        return;
    }
    House = 0xFF;
    Type = 0;
    Frame = ::Frame;
}
