// airctype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/airctype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/aircraft_type.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"


// AircraftTypeClass::AircraftTypeClass(char const *ini_name) (0x0041BC90)
// Speed type 4; joins AircraftTypes and records its index; aircraft
// defaults for inherited flags.

AircraftTypeClass::AircraftTypeClass(char const *ini_name) :
    TechnoTypeClass(ini_name, 4),
    ArrayIndex(-1),
    Carryall(false),
    Rotors(false),
    CustomRotor(false),
    Landable(false),
    AirportBound(false),
    Fighter(false)
{
    Create_ID();
    AircraftTypes.Add(this);
    ArrayIndex = AircraftTypes.ID(this);
    Parasiteable = true;
    ConsideredAircraft = true;
    MoveToShroud = false;
    RotCount = 32;
    Trailer = 0;
    SpawnDelay = 3;
    ImmuneToPsionics = false;
    Organic = false;
}


// AircraftTypeClass::AircraftTypeClass(NoInitClass const &noinit) (0x0041BDB0)
// Load path.

AircraftTypeClass::AircraftTypeClass(NoInitClass const &noinit) :
    TechnoTypeClass(noinit)
{
}


// AircraftTypeClass::~AircraftTypeClass() (0x0041BDE0)
// Leaves AircraftTypes.

AircraftTypeClass::~AircraftTypeClass()
{
    Unknown_006ECB90(this, true);
    AircraftTypes.Delete(this);
}


// static int __fastcall AircraftTypeClass::From_Name(char const *name) (0x0041BE60)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall AircraftTypeClass::From_Name(char const *name)
{
    if (name) {
        for (int index = 0; index < AircraftTypes.Count(); index++) {
            if (_stricmp(AircraftTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// static void AircraftTypeClass::Init() (0x0041BEB0)
// Loads the rotor shapes. Not in the function inventory (code
// 0x0041BEB0-0x0041BECE, 31 bytes, padding to 0x0041BED0).

void AircraftTypeClass::Init()
{
    unknown_left_rotor_83c554 = MixFileClass::Retrieve("LROTOR.SHP");
    unknown_right_rotor_83c558 = MixFileClass::Retrieve("RROTOR.SHP");
}


// ObjectClass *AircraftTypeClass::Create_One_Of(HouseClass *house) const (0x0041BED0)
// A new aircraft of this type for the house (RA1: Create_One_Of).

ObjectClass *AircraftTypeClass::Create_One_Of(HouseClass *house) const
{
    return (ObjectClass *)new AircraftClass((AircraftTypeClass *)this, house);
}


// bool AircraftTypeClass::CanUseWaypoint() const (0x0041BF00)

bool AircraftTypeClass::CanUseWaypoint() const
{
    return false;
}


// bool AircraftTypeClass::CanAttackMove() const (0x0041BF10)

bool AircraftTypeClass::CanAttackMove() const
{
    return false;
}


// Cell const *AircraftTypeClass::Occupy_List(int arg1) const (0x0041BF20)
// The occupation list: the origin cell and the end marker.

Cell const *AircraftTypeClass::Occupy_List(int arg1) const
{
    static UnknownStaticCell const unknown_list[] = {Cell(0, 0), Cell(0x7FFF, 0x7FFF)};
    return &unknown_list[0].Cell;
}


// Exit thunk of the function-local static in AircraftTypeClass::Occupy_List (0x0041BF80)
// Compiler generated ($E) after unknown_slot36.
// @symbol _$E47


// bool AircraftTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x0041BF90)
// Aircraft are never placed this way (RA1: Create_And_Place).

bool AircraftTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    return false;
}


// CoordStruct AircraftTypeClass::Lepton_Dimensions() const (0x0041BFA0)
// One cell square, twice the 0x0083C550 height.

CoordStruct AircraftTypeClass::Lepton_Dimensions() const
{
    return CoordStruct(256, 256, unknown_geometry_g * 2);
}


// bool AircraftTypeClass::Read_INI(CCINIClass &ini) (0x0041BFD0)
// The techno values, then the aircraft keys (rules) and the art keys
// (art INI, image section).

bool AircraftTypeClass::Read_INI(CCINIClass &ini)
{
    if (TechnoTypeClass::Read_INI(ini)) {
        Landable = ini.Get_Bool(ID, "Landable", Landable);
        AirportBound = ini.Get_Bool(ID, "AirportBound", AirportBound);
        Fighter = ini.Get_Bool(ID, "Fighter", Fighter);
        Carryall = ini.Get_Bool(ID, "Carryall", Carryall);
        Rotors = ArtINI.Get_Bool(unknown_1c8, "Rotors", Rotors);
        CustomRotor = ArtINI.Get_Bool(unknown_1c8, "CustomRotor", CustomRotor);
        Trailer = Get_Anim_Type(ArtINI, unknown_1c8, "Trailer", Trailer);
        SpawnDelay = ArtINI.Get_Int(unknown_1c8, "SpawnDelay", SpawnDelay);
        return true;
    }
    return false;
}


// void AircraftTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x0041C110)

void AircraftTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    TechnoTypeClass::Compute_CRC(crc);
    crc(Rotors);
    crc(CustomRotor);
    crc(Landable);
    crc(AirportBound);
    crc(Fighter);
    crc((long)ArrayIndex);
}


// HRESULT __stdcall AircraftTypeClass::Load(IStream *stream) (0x0041C180)
// Rebuilds the object in place, swizzles Trailer and reloads the image.

HRESULT __stdcall AircraftTypeClass::Load(IStream *stream)
{
    HRESULT result = TechnoTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) AircraftTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&Trailer);
    Fetch_Voxel_Image();
    Fetch_Normal_Image();
    return S_OK;
}


// HRESULT __stdcall AircraftTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x0041C1F0)

HRESULT __stdcall AircraftTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = TechnoTypeClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall AircraftTypeClass::GetClassID(GUID *clsid) (0x0041C210)

HRESULT __stdcall AircraftTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = unknown_clsid_aircraft_type_7a1b20;
    return S_OK;
}


// static AircraftTypeClass *__fastcall AircraftTypeClass::Find_Or_Make(char const *name) (0x0041C250)
// The type with this ID, a new one when unknown, 0 for "<none>"/"none".

AircraftTypeClass *__fastcall AircraftTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < AircraftTypes.Count(); index++) {
        if (_stricmp(AircraftTypes[index]->ID, name) == 0) {
            return AircraftTypes[index];
        }
    }
    return new AircraftTypeClass(name);
}


// CoordStruct ObjectTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x0041C2E0)
// Inline in shared/object_type.hpp; the kept copy is AircraftType.cpp's.
// @symbol ?unknown_slot27@ObjectTypeClass@@


// void *ObjectTypeClass::Get_Image_Data() const (0x0041C300)
// Inline in shared/object_type.hpp; the kept copy is AircraftType.cpp's.
// @symbol ?unknown_slot39@ObjectTypeClass@@


// int AircraftTypeClass::Kind_Of() const (0x0041C310)

int AircraftTypeClass::Kind_Of() const
{
    return 3;
}


// int AircraftTypeClass::Size_Of() const (0x0041C320)

int AircraftTypeClass::Size_Of() const
{
    return sizeof(AircraftTypeClass);
}


// int AircraftTypeClass::Fetch_Heap_ID() const (0x0041C330)
// The type's index in AircraftTypes.

int AircraftTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// AircraftTypeClass scalar deleting destructor (0x0041C340)
// Compiler generated after the unit's last member (destructor inlined).
