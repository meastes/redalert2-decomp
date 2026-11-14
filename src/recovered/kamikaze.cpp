// kamikaze.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/kamikaze.json carries the addresses and the rows that
// stay in their own files.


// KamikazeTrackerClass KamikazeTracker (0x00A6E428): its dynamic initializer
// (0x00531550; the constructor expanded, the VectorClass constructor called) and
// atexit destructor (0x005315C0; the destructor expanded) come from this definition.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/kamikaze_tracker.hpp"

KamikazeTrackerClass KamikazeTracker;


// atexit destructor of KamikazeTracker (0x005315C0) emitted by KamikazeTracker_global_init_00531550.cpp; no source of its own.


// KamikazeTrackerClass::KamikazeTrackerClass() (0x00531650)
// An empty list; the timer runs 100 frames.

KamikazeTrackerClass::KamikazeTrackerClass()
{
    Timer = 100;
}


// void KamikazeTrackerClass::Add(AircraftClass *missile, AbstractClass *target) (0x005316A0)
// A spawned missile is tracked flying at the target's cell (or the cell ahead
// of it without a target); any other aircraft gets slot 224 instead.
#include "shared/cell.hpp"
#include "shared/aircraft.hpp"
#include "shared/facing.hpp"
#include "shared/map_view.hpp"

void KamikazeTrackerClass::Add(AircraftClass *missile, AbstractClass *target)
{
    if (!missile->Type->MissileSpawn) {
        missile->unknown_slot224(0);
        return;
    }
    KamikazeControl *node = new KamikazeControl;
    node->Unit = missile;
    if (target == 0) {
        int facing = Dir_To_8(missile->unknown_2e0.Current());
        CellClass *here = (CellClass *)missile->Get_Cell_Ptr();
        node->Cell = &here->Adjacent_Cell(facing);
    } else {
        node->Cell = &Map[Coord_Cell(target->Center_Coord())];
    }
    missile->unknown_5b2 = true;
    missile->unknown_254 = 1;
    Nodes.Add(node);
}


// void KamikazeTrackerClass::AI() (0x005317C0)
// Every 30 frames each tracked missile is sent (again) at its cell, or at the
// cell ahead of it when it has none (each branch makes both calls; VC6
// shares their tail).

void KamikazeTrackerClass::AI()
{
    if (Timer == 0) {
        Timer = 30;
        for (int index = 0; index < Nodes.Count(); index++) {
            KamikazeControl *node = Nodes[index];
            CellClass *cell = node->Cell;
            AircraftClass *missile = node->Unit;
            missile->unknown_254 = 1;
            if (cell == 0) {
                int facing = Dir_To_8(missile->unknown_2e0.Current());
                CellClass *here = (CellClass *)missile->Get_Cell_Ptr();
                CellClass *adjacent = &here->Adjacent_Cell(facing);
                missile->unknown_slot220(adjacent);
                missile->QueueMission(1, false);
            } else {
                missile->unknown_slot220(cell);
                missile->QueueMission(1, false);
            }
        }
    }
}


// void KamikazeTrackerClass::Detach(AbstractClass *target) (0x00531880)
// A missile that is gone is dropped from the list; missiles flying at a cell
// that is detached are sent at the target's cell instead.

void KamikazeTrackerClass::Detach(AbstractClass *target)
{
    for (int index = Nodes.Count() - 1; index >= 0; index--) {
        KamikazeControl *node = Nodes[index];
        if (target == node->Unit) {
            delete Nodes[index];
            Nodes.Delete(index);
            return;
        }
        if (target == node->Cell) {
            CellClass *cell = &Map[Coord_Cell(target->Center_Coord())];
            Nodes[index]->Unit->unknown_slot220(cell);
            Nodes[index]->Unit->QueueMission(1, false);
            Nodes[index]->Cell = cell;
            return;
        }
    }
}


// KamikazeTrackerClass::~KamikazeTrackerClass() (0x00531980)

KamikazeTrackerClass::~KamikazeTrackerClass()
{
    for (int index = Nodes.Count() - 1; index >= 0; index--) {
        delete Nodes[index];
    }
    DynamicVectorClass<KamikazeControl *> &nodes = Nodes;
    int length = nodes.Length();
    nodes.Clear();
    nodes.Resize(length);
}


// void KamikazeTrackerClass::Clear() (0x005319E0)

void KamikazeTrackerClass::Clear()
{
    for (int index = Nodes.Count() - 1; index >= 0; index--) {
        delete Nodes[index];
    }
    DynamicVectorClass<KamikazeControl *> &nodes = Nodes;
    int length = nodes.Length();
    nodes.Clear();
    nodes.Resize(length);
    Timer = 1;
}


// HRESULT KamikazeTrackerClass::Save(IStream *stream) (0x00531A40)

HRESULT KamikazeTrackerClass::Save(IStream *stream)
{
    int count = Nodes.Count();
    HRESULT result = stream->Write(&count, sizeof(count), 0);
    if (SUCCEEDED(result)) {
        for (int index = 0; index < count; index++) {
            result = stream->Write(Nodes[index], sizeof(KamikazeControl), 0);
            if (FAILED(result)) {
                break;
            }
        }
    }
    return result;
}


// HRESULT KamikazeTrackerClass::Load(IStream *stream) (0x00531AA0)
#include "shared/swizzle.hpp"

HRESULT KamikazeTrackerClass::Load(IStream *stream)
{
    int count;
    HRESULT result = stream->Read(&count, sizeof(count), 0);
    if (SUCCEEDED(result)) {
        for (int index = 0; index < count; index++) {
            KamikazeControl *node = new KamikazeControl;
            result = stream->Read(node, sizeof(KamikazeControl), 0);
            if (FAILED(result)) {
                return result;
            }
            Swizzler.Swizzle((void **)&node->Unit);
            Swizzler.Swizzle((void **)&node->Cell);
            Nodes.Add(node);
        }
    }
    return result;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<KamikazeControl *>::Clear (0x00531B80; DynamicVectorClass<KamikazeControl *> vtable 0x007a53b4 slot 3).
// DynamicVectorClass<KamikazeControl *>::ID_pointer (0x00531BB0; DynamicVectorClass<KamikazeControl *> vtable 0x007a53b4 slot 5).
// VectorClass<KamikazeControl *>::Fetch (0x00531BD0; VectorClass<KamikazeControl *> vtable 0x007a53d4 slot 6).
// VectorClass<KamikazeControl *>::operator equal (0x00531BE0; VectorClass<KamikazeControl *> vtable 0x007a53d4 slot 1).
// VectorClass<KamikazeControl *>::Resize (0x00531C20; VectorClass<KamikazeControl *> vtable 0x007a53d4 slot 2).
// VectorClass<KamikazeControl *>::Clear (0x00531CD0; VectorClass<KamikazeControl *> vtable 0x007a53d4 slot 3).
// VectorClass<KamikazeControl *>::ID_object (0x00531D00; VectorClass<KamikazeControl *> vtable 0x007a53d4 slot 4).
// VectorClass<KamikazeControl *>::ID_pointer (0x00531D40; VectorClass<KamikazeControl *> vtable 0x007a53d4 slot 5).
// DynamicVectorClass<KamikazeControl *>::Resize (0x00531D60; DynamicVectorClass<KamikazeControl *> vtable 0x007a53b4 slot 2).
// DynamicVectorClass<KamikazeControl *>::ID_object (0x00531E10; DynamicVectorClass<KamikazeControl *> vtable 0x007a53b4 slot 4).


extern "C" unsigned char vtable_531e40[]; extern "C" void __cdecl release_buffer_531e40(void*); extern "C" void __cdecl release_object_531e40(void*);
class Leaf_531e40 { public: void* get(unsigned int flags); };
void* Leaf_531e40::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_531e40; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_531e40((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_531e40(this); return this; }


extern "C" unsigned char vtable_531e90[]; extern "C" void __cdecl release_buffer_531e90(void*); extern "C" void __cdecl release_object_531e90(void*);
class Leaf_531e90 { public: void* get(unsigned int flags); };
void* Leaf_531e90::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_531e90; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_531e90((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_531e90(this); return this; }


extern "C" unsigned char table_531ee0[]; extern "C" void* __cdecl storage_531ee0(unsigned int);
class Storage_531ee0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_531ee0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_531ee0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_531ee0(requested << 2); owns=1; } return this; }
