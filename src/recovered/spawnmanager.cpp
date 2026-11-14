// spawnmanager.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/spawnmanager.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x006894A0 and the
// unit's SpawnManagerClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class SpawnManagerClass;

// The unit's list of SpawnManagerClass objects (0x00ABD028; name is a hypothesis).
DynamicVectorClass<SpawnManagerClass *> unknown_SpawnManagerClass_list_00abd028;


// Dynamic initializer (0x006894D0, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x006894F0, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689510, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689530, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689550, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689570, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x006895B0, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x006895E0, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689610, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689640, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689650, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689670, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x00689690, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// Dynamic initializer (0x006896D0, _$E) emitted by HeaderStatics_diagonal_init_006894a0.cpp; no source of its own.


// SpawnManagerClass::SpawnManagerClass(TechnoClass *owner, AircraftTypeClass *type, int count, int regen, int reload) (0x00689710)
// Creates count units of the type for the owner's house, each in its own
// record, and joins the unit's list.
#include "shared/spawn_manager.hpp"
#include "shared/aircraft.hpp"
#include "shared/rules.hpp"

SpawnManagerClass::SpawnManagerClass(TechnoClass *owner, AircraftTypeClass *type, int count, int regen, int reload)
{
    Owner = owner;
    SpawnType = type;
    SpawnCount = count;
    RegenRate = regen;
    ReloadRate = reload;
    for (int i = 0; i < SpawnCount; i++) {
        SpawnControl *node = new SpawnControl;
        if (!node) {
            break;
        }
        node->Unit = (AircraftClass *)SpawnType->Create_One_Of(Owner->Owning_House());
        if (node->Unit) {
            if (SpawnType == Rules->V3Rocket.Type || SpawnType == Rules->DMisl.Type) {
                node->IsMissile = 1;
            } else {
                node->IsMissile = 0;
            }
            node->Unit->Limbo();
            node->Unit->unknown_23c = Owner;
            node->Status = 0;
            node->Timer = 0;
            SpawnedNodes.Add(node);
        }
    }
    unknown_50 = 20;
    Destination = 0;
    Target = 0;
    Status = 0;
    unknown_5c = 0;
    unknown_SpawnManagerClass_list_00abd028.Add(this);
}


// SpawnManagerClass::SpawnManagerClass() (0x00689910)
// The class factory's empty manager.

SpawnManagerClass::SpawnManagerClass()
{
    Owner = 0;
    SpawnType = 0;
    SpawnCount = 0;
    unknown_50 = 0;
    Destination = 0;
    Target = 0;
    Status = 0;
    unknown_5c = 0;
    unknown_SpawnManagerClass_list_00abd028.Add(this);
}


// SpawnManagerClass::SpawnManagerClass(NoInitClass const &noinit) (0x00689A10)

SpawnManagerClass::SpawnManagerClass(NoInitClass const &noinit) :
    AbstractClass(noinit),
    unknown_50(0),
    unknown_5c(0)
{
}


// SpawnManagerClass::~SpawnManagerClass() (0x00689A80)
// Kills the spawned units unless the game is shutting down, frees the
// records and leaves the unit's list.
#include "shared/unknown_globals.hpp"

SpawnManagerClass::~SpawnManagerClass()
{
    if (unknown_flag_a40958) {
        Kill_Nodes();
    }
    for (int i = SpawnedNodes.Count() - 1; i >= 0; i--) {
        delete SpawnedNodes[i];
    }
    DynamicVectorClass<SpawnControl *> &nodes = SpawnedNodes;
    int length = nodes.Length();
    nodes.Clear();
    nodes.Resize(length);
    unknown_SpawnManagerClass_list_00abd028.Delete(this);
}


// void SpawnManagerClass::Kill_Nodes() (0x00689B70)
// Every live record loses its unit: docked units are removed, launching ones
// leave the missile tracker first, and flying ones are handed to the tracker
// to finish at the destination. Each record then waits RegenRate frames.
#include "shared/kamikaze_tracker.hpp"

void SpawnManagerClass::Kill_Nodes()
{
    for (int i = SpawnedNodes.Count() - 1; i >= 0; i--) {
        int status = SpawnedNodes[i]->Status;
        if (status == 7) {
            continue;
        }
        if (status == 0 || status == 6) {
            SpawnedNodes[i]->Status = 7;
            SpawnedNodes[i]->Unit->Delete_Me();
            SpawnedNodes[i]->Unit = 0;
            SpawnedNodes[i]->Timer = RegenRate;
        } else if (status == 1) {
            KamikazeTracker.Detach(SpawnedNodes[i]->Unit);
            SpawnedNodes[i]->Status = 7;
            SpawnedNodes[i]->Unit->Delete_Me();
            SpawnedNodes[i]->Unit = 0;
            SpawnedNodes[i]->Timer = RegenRate;
        } else {
            SpawnedNodes[i]->Status = 7;
            KamikazeTracker.Add(SpawnedNodes[i]->Unit, Destination);
            SpawnedNodes[i]->Unit = 0;
            SpawnedNodes[i]->Timer = RegenRate;
        }
    }
}


// void SpawnManagerClass::AI() (0x00689C90)
// The spawner's frame update (YR's SpawnManagerClass::AI, name hypothesis): every
// 10 frames walks the records. Docked (0): launches one toward the destination
// when the launch timer allows (9 frames for MissileSpawn owners, else 20), the
// owner is not jammed (0x0051FC80) and a missile launcher is not moving; missiles
// alternate the owner's burst index and fly at the destination, others leave
// for the cell north of the owner. Launching (1): unlinked when its timer runs
// out. Flying (2) and attacking (3): attack the destination while armed, else
// return to the owner (4). Returning (4): docks when it is over the owner's cell
// and less than 20 leptons above it, reloading for ReloadRate frames (6), which
// refills ammo and strength. Dead (7): a new unit is made when the timer runs out.
// Then the spawner's own state: idle (0) takes a target in range, launching (1)
// sends every flying record at the destination once all are out (MissileSpawn
// units go to the kamikaze tracker), and attacking (2) goes idle when no record
// is attacking or returning.
#include "shared/aircraft_type.hpp"
#include "shared/house.hpp"
#include "shared/foot.hpp"
#include "shared/facing.hpp"
#include "shared/map_view.hpp"
#include "shared/weapon_type.hpp"

void SpawnManagerClass::AI()
{
    if (unknown_50.Value()) {
        return;
    }
    unknown_50 = 10;

    for (int i = 0; i < SpawnedNodes.Count(); i++) {
        bool burst = false;
        AircraftClass *unit = SpawnedNodes[i]->Unit;
        switch (SpawnedNodes[i]->Status) {
        case 0:
            if (Destination == 0 || unknown_5c.Value() || Status == 2 || Owner->House->Unknown_0051FC80()) {
                break;
            }
            if (SpawnedNodes[i]->IsMissile == 1) {
                if (((FootClass *)Owner)->Locomotor.Checked()->Is_Moving()) {
                    break;
                }
                if (((FootClass *)Owner)->Locomotor.Checked()->Is_Moving_Now()) {
                    break;
                }
            }
            if (Owner->Techno_Type_Class()->MissileSpawn) {
                unknown_5c = 9;
            } else {
                unknown_5c = 20;
            }
            if (SpawnedNodes[i]->IsMissile == 1) {
                WeaponTypeClass *weapon = Owner->unknown_slot231(0)->WeaponType;
                if (weapon != 0 && weapon->Burst > 1) {
                    burst = true;
                    Owner->unknown_310 = i & 1;
                }
            }
            SpawnedNodes[i]->Status = 2;
            {
                CoordStruct coord = Owner->unknown_slot43(0);
                coord.Z += 10;
                unit->Unlimbo(&coord, Dir_To_256(Owner->unknown_2e0.Current()));
            }
            if (burst) {
                Owner->unknown_310 = 0;
            }
            if (SpawnedNodes[i]->IsMissile == 1) {
                if (Target != 0) {
                    Destination = Target;
                    Target = 0;
                }
                unit->Assign_Destination(Destination, true);
            } else {
                CellClass &cell = Map[Owner->unknown_slot104()];
                unit->Assign_Destination(&cell.Adjacent_Cell(0), true);
            }
            unit->QueueMission(2, false);
            break;

        case 1:
            if (!SpawnedNodes[i]->Timer.Value()) {
                Unlink(unit);
            }
            break;

        case 2:
            if (SpawnedNodes[i]->IsMissile == 1) {
                break;
            }
            if (Target != 0) {
                Destination = Target;
                Target = 0;
            }
            if (Destination == 0) {
                unit->Assign_Destination(Owner, true);
                unit->unknown_slot220(0);
                unit->QueueMission(2, false);
                SpawnedNodes[i]->Status = 4;
            } else {
                CellClass &cell = Map[Owner->unknown_slot104()];
                unit->Assign_Destination(&cell.Adjacent_Cell(4), true);
                unit->QueueMission(2, false);
            }
            break;

        case 3:
            if (Target != 0) {
                Destination = Target;
                Target = 0;
            }
            if (unit->unknown_254 == 0 || Destination == 0) {
                unit->Assign_Destination(Owner, true);
                unit->unknown_slot220(0);
                unit->QueueMission(2, false);
                SpawnedNodes[i]->Status = 4;
            } else {
                SpawnedNodes[i]->Unit->unknown_slot220(Destination);
                SpawnedNodes[i]->Unit->QueueMission(1, false);
            }
            break;

        case 4:
            if (Target != 0) {
                Destination = Target;
                Target = 0;
            }
            if (unit->unknown_254 > 0 && Destination != 0) {
                SpawnedNodes[i]->Status = 3;
                SpawnedNodes[i]->Unit->unknown_slot220(Destination);
                SpawnedNodes[i]->Unit->QueueMission(1, false);
            } else if (unit->unknown_slot104() == Owner->unknown_slot104() && CoordStruct(unit->Coord).Z - CoordStruct(Owner->Coord).Z < 20) {
                unit->Limbo();
                SpawnedNodes[i]->Status = 6;
                SpawnedNodes[i]->Timer = ReloadRate;
            } else {
                unit->Assign_Destination(Owner, true);
                unit->unknown_slot220(0);
                unit->QueueMission(2, false);
            }
            break;

        case 6:
            if (!SpawnedNodes[i]->Timer.Value()) {
                SpawnedNodes[i]->Status = 0;
                unit->unknown_254 = unit->Type->Ammo;
                unit->unknown_70 = unit->Strength = unit->Type->MaxStrength;
            }
            break;

        case 7:
            if (!SpawnedNodes[i]->Timer.Value()) {
                SpawnedNodes[i]->Unit = (AircraftClass *)SpawnType->Create_One_Of(Owner->Owning_House());
                if (SpawnType == Rules->V3Rocket.Type || SpawnType == Rules->DMisl.Type) {
                    SpawnedNodes[i]->IsMissile = 1;
                } else {
                    SpawnedNodes[i]->IsMissile = 0;
                }
                SpawnedNodes[i]->Unit->Limbo();
                SpawnedNodes[i]->Unit->unknown_23c = Owner;
                SpawnedNodes[i]->Status = 0;
            }
            break;
        }
    }

    bool flag = false;
    switch (Status) {
    case 0:
        if (Target != 0) {
            Destination = Target;
            Target = 0;
        }
        if (Destination != 0) {
            if (!Owner->unknown_slot213(Destination)) {
                Abandon_Target();
                return;
            }
            Status = 1;
        }
        break;

    case 1: {
        if (Destination == 0) {
            Abandon_Target();
            return;
        }
        int index;
        for (index = 0; index < SpawnedNodes.Count(); index++) {
            int status = SpawnedNodes[index]->Status;
            if (status != 2 && status != 7) {
                return;
            }
        }
        for (index = 0; index < SpawnedNodes.Count(); index++) {
            if (SpawnedNodes[index]->Status == 2) {
                if (SpawnedNodes[index]->Unit->Type->MissileSpawn) {
                    flag = true;
                    KamikazeTracker.Add(SpawnedNodes[index]->Unit, Destination);
                    KamikazeTracker.Timer = 2;
                    if (SpawnedNodes[index]->IsMissile == 1) {
                        SpawnedNodes[index]->Status = 1;
                        if (SpawnType == Rules->V3Rocket.Type) {
                            SpawnedNodes[index]->Timer = Rules->V3Rocket.PauseFrames + Rules->V3Rocket.TiltFrames;
                        } else {
                            SpawnedNodes[index]->Timer = Rules->DMisl.PauseFrames + Rules->DMisl.TiltFrames;
                        }
                    } else {
                        Unlink(SpawnedNodes[index]->Unit);
                    }
                } else {
                    SpawnedNodes[index]->Status = 3;
                    SpawnedNodes[index]->Unit->unknown_slot220(Destination);
                    SpawnedNodes[index]->Unit->QueueMission(1, false);
                }
            }
        }
        if (flag) {
            Abandon_Target();
        }
        Status = 2;
        break;
    }

    case 2: {
        flag = true;
        for (int index = 0; index < SpawnedNodes.Count(); index++) {
            int status = SpawnedNodes[index]->Status;
            if (status == 3 || status == 4) {
                flag = false;
            }
        }
        if (flag) {
            Status = 0;
        }
        break;
    }
    }
}


// void SpawnManagerClass::Set_Target(AbstractClass *target) (0x0068A490)
// Queues a new target; the current destination is not queued again.

void SpawnManagerClass::Set_Target(AbstractClass *target)
{
    if (target != Destination) {
        Target = target;
    }
}


// void SpawnManagerClass::Abandon_Target() (0x0068A4B0)
// Flying missile spawns go to the missile tracker to finish at the
// destination and are unlinked; then the manager forgets its targets.

void SpawnManagerClass::Abandon_Target()
{
    for (int i = 0; i < SpawnedNodes.Count(); i++) {
        if (SpawnedNodes[i]->Status == 2 && SpawnedNodes[i]->Unit->Type->MissileSpawn) {
            KamikazeTracker.Add(SpawnedNodes[i]->Unit, Destination);
            KamikazeTracker.Timer = 2;
            Unlink(SpawnedNodes[i]->Unit);
        }
    }
    Status = 0;
    Target = 0;
    Destination = 0;
}


// bool SpawnManagerClass::Next_Target() (0x0068A540)

bool SpawnManagerClass::Next_Target()
{
    if (Target) {
        Destination = Target;
        Target = 0;
        return true;
    }
    return false;
}


// void SpawnManagerClass::Unlink(AbstractClass *object) (0x0068A560)
// Forgets a dying object: the destination (then the queued target takes
// over, if any), the queued target, one of its units (whose record then
// waits RegenRate frames, unless a healthy non-missile unit is only being
// detached) or the owner (every unit dies).

void SpawnManagerClass::Unlink(AbstractClass *object)
{
    if (object == Destination) {
        Destination = 0;
        if (!Target) {
            Abandon_Target();
        }
        return;
    }
    if (object == Target) {
        Target = 0;
        return;
    }
    for (int i = SpawnedNodes.Count() - 1; i >= 0; i--) {
        if (object == SpawnedNodes[i]->Unit) {
            AircraftClass *unit = SpawnedNodes[i]->Unit;
            if (unit->Strength > 0 && !unit->unknown_5b2 && SpawnedNodes[i]->IsMissile != 1) {
                return;
            }
            SpawnedNodes[i]->Unit = 0;
            SpawnedNodes[i]->Status = 7;
            SpawnedNodes[i]->Timer = RegenRate;
            return;
        }
    }
    if (object == Owner) {
        Kill_Nodes();
        Abandon_Target();
    }
}


// int SpawnManagerClass::Alive_Count() const (0x0068A630)

int SpawnManagerClass::Alive_Count() const
{
    int count = 0;
    for (int i = 0; i < SpawnedNodes.Count(); i++) {
        if (SpawnedNodes[i]->Status != 7) {
            count++;
        }
    }
    return count;
}


// int SpawnManagerClass::Docked_Count() const (0x0068A650)

int SpawnManagerClass::Docked_Count() const
{
    int count = 0;
    for (int i = 0; i < SpawnedNodes.Count(); i++) {
        int status = SpawnedNodes[i]->Status;
        if (status == 6 || status == 0) {
            count++;
        }
    }
    return count;
}


// int SpawnManagerClass::Launching_Count() const (0x0068A680)

int SpawnManagerClass::Launching_Count() const
{
    int count = 0;
    for (int i = 0; i < SpawnedNodes.Count(); i++) {
        if (SpawnedNodes[i]->Status == 1) {
            count++;
        }
    }
    return count;
}


// ?Compute_CRC@SpawnManagerClass@@UBEXAAVWWCRCEngine@@@Z (0x0068A6A0)
// AbstractClass part, state, linked object IDs and timer values.

void SpawnManagerClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc((long)Status);
    if (Target != 0) {
        crc(Target->Fetch_ID());
    }
    if (Destination != 0) {
        crc(Destination->Fetch_ID());
    }
    crc(unknown_5c.Value());
    crc(unknown_50.Value());
    crc((long)SpawnedNodes.Count());
    crc((long)SpawnCount);
    if (SpawnType != 0) {
        crc(SpawnType->Fetch_ID());
    }
    if (Owner != 0) {
        crc(Owner->Fetch_ID());
    }
}


// ?GetClassID@SpawnManagerClass@@UAGJPAUGUID@@@Z (0x0068A790)
// The class identifier.

HRESULT __stdcall SpawnManagerClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_SpawnManagerClass;
    return S_OK;
}


// HRESULT SpawnManagerClass::Load(IStream *stream) (0x0068A7D0, slot 5)
// The record count, then each record whole.

HRESULT __stdcall SpawnManagerClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) SpawnManagerClass(NoInitClass());
        int count;
        result = stream->Read(&count, sizeof(count), 0);
        if (SUCCEEDED(result)) {
            new (&SpawnedNodes) DynamicVectorClass<SpawnControl *>;
            for (int i = 0; i < count; i++) {
                SpawnControl *node = new SpawnControl;
                result = stream->Read(node, sizeof(SpawnControl), 0);
                if (FAILED(result)) {
                    return result;
                }
                Swizzle_Pointer(node->Unit);
                SpawnedNodes.Add(node);
            }
            Swizzle_Pointer(Owner);
            Swizzle_Pointer(SpawnType);
            Swizzle_Pointer(Destination);
            Swizzle_Pointer(Target);
        }
    }
    return result;
}


// HRESULT SpawnManagerClass::Save(IStream *stream, BOOL clear_dirty) (0x0068A970, slot 6)

HRESULT __stdcall SpawnManagerClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    int count = SpawnedNodes.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int i = 0; i < count; i++) {
        result = stream->Write(SpawnedNodes[i], sizeof(SpawnControl), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    return result;
}


// ?Size_Of@SpawnManagerClass@@UBEHXZ (0x0068A9E0)
// Object size.

int SpawnManagerClass::Size_Of() const
{
    return sizeof(SpawnManagerClass);
}


// ?Kind_Of@SpawnManagerClass@@UBEHXZ (0x0068A9F0)
// RTTI kind 0x41.

int SpawnManagerClass::Kind_Of() const
{
    return 0x41;
}


// void *SpawnManagerClass::`scalar deleting destructor'(unsigned int flags) (0x0068AA00)
// Compiler-generated with the vtable. No source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<SpawnManagerClass *>::Clear (0x0068AA80; DynamicVectorClass<SpawnManagerClass *> vtable 0x007ab85c slot 3).
// DynamicVectorClass<SpawnManagerClass *>::ID_pointer (0x0068AAB0; DynamicVectorClass<SpawnManagerClass *> vtable 0x007ab85c slot 5).
// VectorClass<SpawnManagerClass *>::Fetch (0x0068AAD0; VectorClass<SpawnManagerClass *> vtable 0x007ab87c slot 6).
// VectorClass<SpawnManagerClass *>::operator equal (0x0068AD40; VectorClass<SpawnManagerClass *> vtable 0x007ab87c slot 1).
// VectorClass<SpawnManagerClass *>::Resize (0x0068AD80; VectorClass<SpawnManagerClass *> vtable 0x007ab87c slot 2).
// VectorClass<SpawnManagerClass *>::Clear (0x0068AE30; VectorClass<SpawnManagerClass *> vtable 0x007ab87c slot 3).
// VectorClass<SpawnManagerClass *>::ID_object (0x0068AE60; VectorClass<SpawnManagerClass *> vtable 0x007ab87c slot 4).
// VectorClass<SpawnManagerClass *>::ID_pointer (0x0068AEA0; VectorClass<SpawnManagerClass *> vtable 0x007ab87c slot 5).
// DynamicVectorClass<SpawnManagerClass *>::Resize (0x0068AEC0; DynamicVectorClass<SpawnManagerClass *> vtable 0x007ab85c slot 2).
// DynamicVectorClass<SpawnManagerClass *>::ID_object (0x0068AF70; DynamicVectorClass<SpawnManagerClass *> vtable 0x007ab85c slot 4).


extern "C" unsigned char vtable_68afa0[]; extern "C" void __cdecl release_buffer_68afa0(void*); extern "C" void __cdecl release_object_68afa0(void*);
class Leaf_68afa0 { public: void* get(unsigned int flags); };
void* Leaf_68afa0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_68afa0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_68afa0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_68afa0(this); return this; }


extern "C" unsigned char vtable_68aff0[]; extern "C" void __cdecl release_buffer_68aff0(void*); extern "C" void __cdecl release_object_68aff0(void*);
class Leaf_68aff0 { public: void* get(unsigned int flags); };
void* Leaf_68aff0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_68aff0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_68aff0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_68aff0(this); return this; }


extern "C" unsigned char vtable_68b040[]; extern "C" void __cdecl release_buffer_68b040(void*); extern "C" void __cdecl release_object_68b040(void*);
class Leaf_68b040 { public: void* get(unsigned int flags); };
void* Leaf_68b040::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_68b040; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_68b040((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_68b040(this); return this; }


extern "C" unsigned char vtable_68b090[]; extern "C" void __cdecl release_buffer_68b090(void*); extern "C" void __cdecl release_object_68b090(void*);
class Leaf_68b090 { public: void* get(unsigned int flags); };
void* Leaf_68b090::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_68b090; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_68b090((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_68b090(this); return this; }


extern "C" unsigned char table_68b0e0[]; extern "C" void* __cdecl storage_68b0e0(unsigned int);
class Storage_68b0e0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_68b0e0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_68b0e0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_68b0e0(requested << 2); owns=1; } return this; }
