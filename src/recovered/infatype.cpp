// infatype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/infatype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/infantry_type.hpp"
#include "shared/map_view.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/crt_string.hpp"
#include "shared/ccini_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/voc_class.hpp"


// Header-static dynamic initializers of the unit at 0x00509930:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00509930-0x00509B16); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00509960, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x00509980, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x005099A0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x005099C0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x005099E0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x00509A00, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x00509A40, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x00509A70, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x00509AA0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x00509AD0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x00509AE0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// Dynamic initializer (0x00509B00, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00509930.cpp; no source of its own.


// InfantryTypeClass::InfantryTypeClass(char const *ini_name) (0x00509B20)
// The infantry defaults (crawls, no sounds), a new ID, registration in the
// infantry type list (its index), the techno-type values infantry override,
// and the 38 empty animation sequences.

// An unused animation sequence.
static inline void Clear_Sequence(DoInfoStruct *&sequences, int index)
{
    sequences[index].Frame = 0;
    sequences[index].Count = 0;
    sequences[index].Jump = 0;
    sequences[index].Facing = -1;
}

InfantryTypeClass::InfantryTypeClass(char const *ini_name) :
    TechnoTypeClass(ini_name, 0),
    ArrayIndex(-1),
    Pip(1),
    DoControls(0),
    FireUp(0),
    FireProne(0),
    EnterWaterSound(-1),
    LeaveWaterSound(-1),
    Cyborg(false),
    NotHuman(false),
    Ivan(false),
    DetectionDistance(0),
    Occupier(false),
    Assaulter(false),
    Fearless(false),
    Crawls(true),
    Infiltrate(false),
    Fraidycat(false),
    TiberiumProof(false),
    Civilian(false),
    C4(false),
    Engineer(false),
    Agent(false),
    Thief(false),
    VehicleThief(false),
    Doggie(false),
    Deployer(false)
{
    Create_ID();
    InfantryTypes.Add(this);
    ArrayIndex = InfantryTypes.ID(this);
    RotCount = 8;
    RadarVisible = false;
    unknown_1fd = true;
    Repairable = false;
    Crewed = false;
    ImmuneToPsionics = false;
    Parasiteable = true;
    Organic = true;
    ConsideredAircraft = false;
    DoControls = new DoInfoStruct[38];
    for (int index = 0; index < 38; index++) {
        Clear_Sequence(DoControls, index);
    }
}


// InfantryTypeClass::InfantryTypeClass(NoInitClass const &noinit) (0x00509D60)
// Load path.

InfantryTypeClass::InfantryTypeClass(NoInitClass const &noinit) :
    TechnoTypeClass(noinit),
    DeadBodies(noinit),
    DeathAnims(noinit),
    VoiceComment(noinit)
{
}


// InfantryTypeClass::~InfantryTypeClass() (0x00509DB0)
// Leaves InfantryTypes and frees the sequences.

InfantryTypeClass::~InfantryTypeClass()
{
    Unknown_006ECB90(this, true);
    InfantryTypes.Delete(this);
    operator delete(DoControls);
    DoControls = 0;
}


// ObjectClass *InfantryTypeClass::Create_One_Of(HouseClass *house) const (0x00509EF0)
// A new infantry unit of this type for the house (RA1: Create_One_Of).

ObjectClass *InfantryTypeClass::Create_One_Of(HouseClass *house) const
{
    return (ObjectClass *)new InfantryClass((InfantryTypeClass *)this, house);
}


// bool InfantryTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x00509F20)
// Places a new infantry of this type for the house at the nearest free spot
// of the cell, facing 0x40; deletes it when the cell has no room.

// The "no coordinate" value (0x00A41170; hypothesis).

// Whether a coordinate is not the "no coordinate" value.
static inline bool Is_Valid(CoordStruct const &coord)
{
    return coord.X != unknown_geometry_l.X || coord.Y != unknown_geometry_l.Y
        || coord.Z != unknown_geometry_l.Z;
}

bool InfantryTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    InfantryClass *infantry = new InfantryClass(const_cast<InfantryTypeClass *>(this), house);
    if (infantry != 0) {
        CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
        CoordStruct spot = ((UnknownPlacementCellView &)Map[coord]).Closest_Free_Spot(coord);
        if (Is_Valid(spot)) {
            return infantry->Unlimbo(&spot, 0x40);
        }
        delete infantry;
    }
    return false;
}


// Cell const *InfantryTypeClass::Occupy_List(int arg1) const (0x0050A000)
// The occupation list: the origin cell and the end marker.

Cell const *InfantryTypeClass::Occupy_List(int arg1) const
{
    static UnknownStaticCell const unknown_list[] = {Cell(0, 0), Cell(0x7FFF, 0x7FFF)};
    return &unknown_list[0].Cell;
}


// Exit thunk of the function-local static in InfantryTypeClass::Occupy_List (0x0050A060)
// Compiler generated ($E) after unknown_slot36.
// @symbol _$E41


// static int __fastcall InfantryTypeClass::From_Name(char const *name) (0x0050A070)
// Index of the type with this ID (case-insensitive), or -1 (also for
// "<none>" and "none").

int __fastcall InfantryTypeClass::From_Name(char const *name)
{
    if (_stricmp(name, "<none>") == 0 || _stricmp(name, "none") == 0) {
        return -1;
    }
    for (int index = 0; index < InfantryTypes.Count(); index++) {
        if (_stricmp(InfantryTypes[index]->ID, name) == 0) {
            return index;
        }
    }
    return -1;
}


// void InfantryTypeClass::Read_Sequence() (0x0050A0E0)
// Reads the art INI's Sequence= section of this infantry's image: each named
// sequence is "frame,count,jump,facing" with the facing as a compass name
// (N, NE, E, SE, S, SW, W, NW); an unknown facing name keeps the old one.

extern "C" int __cdecl sscanf(char const *buffer, char const *format, ...);

// The sequence entry names (0x007DAF60: Ready, Guard, Prone, ..., Panic).
extern char const *unknown_sequence_names_7daf60[38];

void InfantryTypeClass::Read_Sequence()
{
    char section[32];
    if (ArtINI.Get_String(unknown_1c8, "Sequence", unknown_string_83c55c, section, sizeof(section)) > 0) {
        DoInfoStruct *controls = DoControls;
        for (int index = 0; index < 38; index++) {
            char facing[10] = "";
            char value[32];
            if (ArtINI.Get_String(section, unknown_sequence_names_7daf60[index], unknown_string_83c55c, value, sizeof(value)) > 0) {
                sscanf(value, "%d,%d,%d,%s", &controls[index].Frame, &controls[index].Count, &controls[index].Jump, facing);
                if (strcmp("N", facing) == 0) {
                    controls[index].Facing = 0;
                } else if (strcmp("NE", facing) == 0) {
                    controls[index].Facing = 1;
                } else if (strcmp("E", facing) == 0) {
                    controls[index].Facing = 2;
                } else if (strcmp("SE", facing) == 0) {
                    controls[index].Facing = 3;
                } else if (strcmp("S", facing) == 0) {
                    controls[index].Facing = 4;
                } else if (strcmp("SW", facing) == 0) {
                    controls[index].Facing = 5;
                } else if (strcmp("W", facing) == 0) {
                    controls[index].Facing = 6;
                } else if (strcmp("NW", facing) == 0) {
                    controls[index].Facing = 7;
                }
            }
        }
    }
}


// bool InfantryTypeClass::Read_INI(CCINIClass &ini) (0x0050A3C0)
// The techno keys, then the infantry keys (rules section) and the art
// keys (image section); cyborgs are flagged (unknown_a9b) and C4,
// engineers and agents can infiltrate; finally the sequence table.

extern "C" char *__cdecl strtok(char *string, char const *delimiters);

// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<AnimTypeClass *> scalar deleting destructor (0x0050B200; vtable 0x007A3B3C slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x007A3B5C. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x0050B100.


class Leaf_50b250 { public: unsigned int get(unsigned int unused_argument); };
unsigned int Leaf_50b250::get(unsigned int unused_argument) { unsigned int this_address = (unsigned int)this; unsigned int object_address; unsigned int unused_value; object_address = this_address; *(unsigned int*)(object_address) = 0x7a3b7c; return object_address; }


extern "C" unsigned char vtable_50b260[]; extern "C" void __cdecl release_buffer_50b260(void*); extern "C" void __cdecl release_object_50b260(void*);
class Leaf_50b260 { public: void* get(unsigned int flags); };
void* Leaf_50b260::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_50b260; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_50b260((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_50b260(this); return this; }


extern "C" unsigned char vtable_50b2b0[]; extern "C" void __cdecl release_buffer_50b2b0(void*); extern "C" void __cdecl release_object_50b2b0(void*);
class Leaf_50b2b0 { public: void* get(unsigned int flags); };
void* Leaf_50b2b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_50b2b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_50b2b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_50b2b0(this); return this; }


// Original region: 0x0050b480 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_0050b480(unsigned int byte_count);
extern "C" unsigned char base_dispatch_0050b480[];
extern "C" unsigned char derived_dispatch_0050b480[];

class WordStorage_0050b480 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_0050b480* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_0050b480_size_check[(sizeof(WordStorage_0050b480) == 24) ? 1 : -1];

WordStorage_0050b480* WordStorage_0050b480::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_0050b480;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_0050b480(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_0050b480;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AnimTypeClass *>::Add (0x0050B590): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<AnimTypeClass *>::Fetch (0x0050B1F0; VectorClass<AnimTypeClass *> vtable 0x007a3b5c slot 6).
// VectorClass<AnimTypeClass *>::operator equal (0x0050B300; VectorClass<AnimTypeClass *> vtable 0x007a3b5c slot 1).
// VectorClass<AnimTypeClass *>::Resize (0x0050B340; VectorClass<AnimTypeClass *> vtable 0x007a3b5c slot 2).
// VectorClass<AnimTypeClass *>::Clear (0x0050B3F0; VectorClass<AnimTypeClass *> vtable 0x007a3b5c slot 3).
// VectorClass<AnimTypeClass *>::ID_object (0x0050B420; VectorClass<AnimTypeClass *> vtable 0x007a3b5c slot 4).
// VectorClass<AnimTypeClass *>::ID_pointer (0x0050B460; VectorClass<AnimTypeClass *> vtable 0x007a3b5c slot 5).
// DynamicVectorClass<AnimTypeClass *>::Resize (0x0050B4E0; TypeList<AnimTypeClass *> vtable 0x007a3b3c slot 2).
// DynamicVectorClass<AnimTypeClass *>::ID_object (0x0050B5E0; TypeList<AnimTypeClass *> vtable 0x007a3b3c slot 4).


// The voice and sound name lists (0x0050B610; unknown names skipped).
TypeList<int> __fastcall Get_Voc_List(CCINIClass &ini, char const *section, char const *entry, TypeList<int> defvalue);

// Get_Sound: shared/ccini_class.hpp.

// Animation types named by the entry (comma separated), else the default.
static inline TypeList<AnimTypeClass *> Get_Anim_List(CCINIClass &ini, char const *section, char const *entry,
                                                      TypeList<AnimTypeClass *> const &defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) {
        TypeList<AnimTypeClass *> list;
        for (char *name = strtok(buffer, ","); name != 0 && *name != 0; name = strtok(0, ",")) {
            AnimTypeClass *anim = AnimTypeClass::Find_Or_Make(name);
            if (anim != 0) {
                list.Add(anim);
            }
        }
        return list;
    }
    return defvalue;
}

bool InfantryTypeClass::Read_INI(CCINIClass &ini)
{
    unknown_a9b = false;
    if (TechnoTypeClass::Read_INI(ini)) {
        Pip = ini.Get_PipType(ID, "Pip", Pip);
        VoiceComment = Get_Voc_List(ini, ID, "VoiceComment", VoiceComment);
        DeadBodies = Get_Anim_List(ini, ID, "DeadBodies", DeadBodies);
        DeathAnims = Get_Anim_List(ini, ID, "DeathAnims", DeathAnims);
        Cyborg = ini.Get_Bool(ID, "Cyborg", Cyborg);
        NotHuman = ini.Get_Bool(ID, "NotHuman", NotHuman);
        if (Cyborg) {
            unknown_a9b = true;
        }
        EnterWaterSound = Get_Sound(ini, ID, "EnterWaterSound", EnterWaterSound);
        LeaveWaterSound = Get_Sound(ini, ID, "LeaveWaterSound", LeaveWaterSound);
        Fearless = ini.Get_Bool(ID, "Fearless", Fearless);
        Fraidycat = ini.Get_Bool(ID, "Fraidycat", Fraidycat);
        Infiltrate = ini.Get_Bool(ID, "Infiltrate", Infiltrate);
        Ivan = ini.Get_Bool(ID, "Ivan", Ivan);
        Occupier = ini.Get_Bool(ID, "Occupier", Occupier);
        Assaulter = ini.Get_Bool(ID, "Assaulter", Assaulter);
        DetectionDistance = ini.Get_Int(ID, "DetectionDistance", DetectionDistance);
        C4 = ini.Get_Bool(ID, "C4", C4);
        Civilian = ini.Get_Bool(ID, "Civilian", Civilian);
        Engineer = ini.Get_Bool(ID, "Engineer", Engineer);
        TiberiumProof = ini.Get_Bool(ID, "TiberiumProof", TiberiumProof);
        Agent = ini.Get_Bool(ID, "Agent", Agent);
        Thief = ini.Get_Bool(ID, "Thief", Thief);
        VehicleThief = ini.Get_Bool(ID, "VehicleThief", VehicleThief);
        Doggie = ini.Get_Bool(ID, "Doggie", Doggie);
        Deployer = ini.Get_Bool(ID, "Deployer", Deployer);
        if (C4) {
            Infiltrate = true;
        }
        if (Engineer) {
            Infiltrate = true;
        }
        if (Agent) {
            Infiltrate = true;
        }
        Crawls = ArtINI.Get_Bool(unknown_1c8, "Crawls", Crawls);
        FireUp = ArtINI.Get_Int(unknown_1c8, "FireUp", FireUp);
        FireProne = ArtINI.Get_Int(unknown_1c8, "FireProne", FireProne);
        Read_Sequence();
        return true;
    }
    return false;
}


// CoordStruct InfantryTypeClass::Lepton_Dimensions() const (0x0050A940)
// 0x55 leptons square, twice the 0x00A411B0 height.

CoordStruct InfantryTypeClass::Lepton_Dimensions() const
{
    return CoordStruct(0x55, 0x55, unknown_geometry_g * 2);
}


// int InfantryTypeClass::Repair_Step() const (0x0050A970)
// The rules value at +0x1340.

int InfantryTypeClass::Repair_Step() const
{
    // Rules +0x1340 is not yet a named field of shared/object.hpp's rules view.
    return Rules->unknown_1340;
}


// int InfantryTypeClass::Repair_Cost() const (0x0050A980)
// Infantry are not repaired.

int InfantryTypeClass::Repair_Cost() const
{
    return 0;
}


// bool InfantryTypeClass::CanUseWaypoint() const (0x0050A990)

bool InfantryTypeClass::CanUseWaypoint() const
{
    return TechnoTypeClass::CanUseWaypoint();
}


// bool InfantryTypeClass::CanAttackMove() const (0x0050A9A0)

bool InfantryTypeClass::CanAttackMove() const
{
    return TechnoTypeClass::CanAttackMove();
}


// CoordStruct InfantryTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x0050A9B0)
// The coordinate, raised to the ground height when below it.

CoordStruct InfantryTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    if (result.Z < Map.Get_Height_GL(result)) {
        result.Z = Map.Get_Height_GL(result);
    }
    return result;
}


// void InfantryTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x0050AA20)

void InfantryTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    TechnoTypeClass::Compute_CRC(crc);
    crc((long)Pip);
    crc(DoControls, 38 * sizeof(DoInfoStruct));
    crc((long)FireUp);
    crc((long)FireProne);
    crc((long)VoiceComment.Count());
    crc((long)DeadBodies.Count());
    crc((long)DeathAnims.Count());
    crc(Fearless);
    crc(Crawls);
    crc(Infiltrate);
    crc(Fraidycat);
    crc(TiberiumProof);
    crc(Civilian);
    crc(C4);
    crc(Engineer);
    crc(Agent);
    crc(Thief);
    crc(VehicleThief);
}


// HRESULT __stdcall InfantryTypeClass::Load(IStream *stream) (0x0050AB40)
// Frees the sequence table, loads the techno type, restores the type in
// place, reloads its images, rebuilds the comment voice and death
// animation lists (swizzling the animations) and reads a fresh sequence
// table (38 entries).

// Rebuilds a list in place from its saved count and elements.
template <class T>
static inline void Load_List(IStream *stream, TypeList<T> &list)
{
    new (&list) TypeList<T>;
    int count;
    stream->Read(&count, sizeof(count), 0);
    for (int index = 0; index < count; index++) {
        T value;
        stream->Read(&value, sizeof(value), 0);
        list.Add(value);
    }
}

HRESULT __stdcall InfantryTypeClass::Load(IStream *stream)
{
    if (DoControls != 0) {
        operator delete(DoControls);
        DoControls = 0;
    }
    HRESULT result = TechnoTypeClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) InfantryTypeClass(NoInitClass());
        Fetch_Voxel_Image();
        Fetch_Normal_Image();
        Load_List(stream, VoiceComment);
        Load_List(stream, DeadBodies);
        int index;
        for (index = 0; index < DeadBodies.Count(); index++) {
            Swizzle_Pointer(DeadBodies[index]);
        }
        Load_List(stream, DeathAnims);
        for (index = 0; index < DeathAnims.Count(); index++) {
            Swizzle_Pointer(DeathAnims[index]);
        }
        DoControls = (DoInfoStruct *)operator new(38 * sizeof(DoInfoStruct));
        stream->Read(DoControls, 38 * sizeof(DoInfoStruct), 0);
    }
    return result;
}


// HRESULT __stdcall InfantryTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x0050AD80)
// The techno part, the three lists and the sequences.

HRESULT __stdcall InfantryTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = TechnoTypeClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        VoiceComment.Save(stream);
        DeadBodies.Save(stream);
        DeathAnims.Save(stream);
        stream->Write(DoControls, 38 * sizeof(DoInfoStruct), 0);
    }
    return result;
}


// HRESULT __stdcall InfantryTypeClass::GetClassID(GUID *clsid) (0x0050AE90)

HRESULT __stdcall InfantryTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = unknown_clsid_infantry_type_7a1b10;
    return S_OK;
}


// static InfantryTypeClass *__fastcall InfantryTypeClass::Find_Or_Make(char const *name) (0x0050AED0)
// The type with this ID, a new one when unknown, 0 for "<none>"/"none".

InfantryTypeClass *__fastcall InfantryTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < InfantryTypes.Count(); index++) {
        if (_stricmp(InfantryTypes[index]->ID, name) == 0) {
            return InfantryTypes[index];
        }
    }
    return new InfantryTypeClass(name);
}


// int InfantryTypeClass::Kind_Of() const (0x0050AF60)

int InfantryTypeClass::Kind_Of() const
{
    return 0x10;
}


// int InfantryTypeClass::Size_Of() const (0x0050AF70)

int InfantryTypeClass::Size_Of() const
{
    return sizeof(InfantryTypeClass);
}


// int InfantryTypeClass::Fetch_Heap_ID() const (0x0050AF80)
// The type's index in InfantryTypes.

int InfantryTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// InfantryTypeClass scalar deleting destructor (0x0050AF90)
// Compiler generated after the unit's last member (destructor inlined).


class Leaf_50b0e0 { public: void* get(); };
void* Leaf_50b0e0::get() { return this; }


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<int>::TypeList, noinit constructor (0x0050B0F0; vtable 0x0079D9F4).
// TypeList<AnimTypeClass *>::TypeList, noinit constructor (0x0050B100; vtable 0x007A3B3C).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AnimTypeClass *>::Clear (0x0050B110; TypeList<AnimTypeClass *> vtable 0x007a3b3c slot 3).
// DynamicVectorClass<AnimTypeClass *>::ID_pointer (0x0050B140; TypeList<AnimTypeClass *> vtable 0x007a3b3c slot 5).


// DynamicVectorClass<AnimTypeClass *>::operator= (0x0050B160)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (population-2466).


// TypeList<int> __fastcall Get_Voc_List(CCINIClass &ini, char const *section, char const *entry, TypeList<int> defvalue) (0x0050B610)
// The sounds named by the entry (comma separated; unknown names skipped),
// else the default. The out-of-line copy InfantryType.cpp's readers call.

extern "C" char *__cdecl strtok(char *string, char const *delimiters);

TypeList<int> __fastcall Get_Voc_List(CCINIClass &ini, char const *section, char const *entry, TypeList<int> defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *name = strtok(buffer, ","); name != 0 && *name != 0; name = strtok(0, ",")) {
            VocClass *voc = VocClass::Find(name);
            if (voc != 0) {
                list.Add(VocClass::Index_Of(voc));
            }
        }
        return list;
    }
    return defvalue;
}


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<AnimTypeClass *>::TypeList, copy constructor (0x0050B7D0; vtable 0x007A3B3C).


extern "C" unsigned char table_50b860[]; extern "C" void* __cdecl storage_50b860(unsigned int);
class Storage_50b860 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_50b860::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_50b860; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_50b860(requested << 2); owns=1; } return this; }
