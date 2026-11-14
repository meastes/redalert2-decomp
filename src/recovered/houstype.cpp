// houstype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/houstype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/house_type.hpp"
#include "shared/ccini_class.hpp"
#include "shared/side.hpp"
#include "shared/infantry_type.hpp"
#include "shared/unit_type.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/crt_string.hpp"
#define TYPE_LIST_READER(NAME, TYPE, FIND) \
static inline TypeList<TYPE *> NAME(CCINIClass &ini, char const *section, char const *entry, TypeList<TYPE *> const &defvalue) \
{ \
    char buffer[128]; \
    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) { \
        TypeList<TYPE *> list; \
        for (char *name = strtok(buffer, ","); name != 0 && *name != 0; name = strtok(0, ",")) { \
            TYPE *object = FIND(name); \
            if (object != 0) { \
                list.Add(object); \
            } \
        } \
        return list; \
    } \
    return defvalue; \
}


// HouseTypeClass::HouseTypeClass(char const *ini_name) (0x004F90F0)
// Defaults (no side, multipliers 1.0, prefix 'A', owns walls), then
// registration in HouseTypes; House and ArrayIndex are both its index there.

HouseTypeClass::HouseTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    ArrayIndex(-1),
    House(-1),
    Side(-1),
    Color(0),
    FirepowerMult(1.0),
    GroundspeedMult(1.0),
    AirspeedMult(1.0),
    ArmorMult(1.0),
    ROFMult(1.0),
    CostMult(1.0),
    BuildtimeMult(1.0),
    unknown_0x100(1.0f),
    unknown_0x104(1.0f),
    unknown_0x108(1.0f),
    unknown_0x10C(1.0f),
    unknown_0x110(1.0f),
    unknown_0x114(1.0f),
    unknown_0x118(1.0f),
    unknown_0x11C(1.0f),
    unknown_0x120(1.0f),
    unknown_0x124(1.0f),
    unknown_0x128(1.0f),
    unknown_0x12C(1.0f),
    unknown_0x130(1.0f),
    unknown_0x134(1.0f),
    unknown_0x138(1.0f),
    unknown_0x13C(1.0f),
    unknown_0x140(1.0f),
    unknown_0x144(1.0f),
    unknown_0x148(1.0f),
    Prefix('A'),
    Multiplay(false),
    MultiplayPassive(false),
    WallOwner(true),
    SmartAI(false)
{
    AbstractClass::Create_ID();
    Suffix[0] = '\0';
    HouseTypes.Add(this);
    House = HouseTypes.ID(this);
    ArrayIndex = HouseTypes.ID(this);
}


// HouseTypeClass::HouseTypeClass(NoInitClass const &noinit) (0x004F9350)
// Load path: installs the vtables (the lists' too) and keeps the loaded fields.

HouseTypeClass::HouseTypeClass(NoInitClass const &noinit) :
    AbstractTypeClass(noinit),
    ParentCountry(noinit),
    VeteranInfantry(noinit),
    VeteranUnits(noinit),
    VeteranAircraft(noinit)
{
}


// HouseTypeClass::~HouseTypeClass() (0x004F93A0)
// Removes the type from HouseTypes.

HouseTypeClass::~HouseTypeClass()
{
    Unknown_006ECB90(this, true);
    HouseTypes.Delete(this);
}


// int __fastcall HouseTypeClass::Find_Index(char const *name) (0x004F94D0)

int __fastcall HouseTypeClass::Find_Index(char const *name)
{
    if (_stricmp(name, "<random>") == 0) {
        return -2;
    }
    if (name != 0) {
        for (int index = 0; index < HouseTypes.Count(); index++) {
            HouseTypeClass *house = HouseTypes[index];
            if (_stricmp(house->Name, name) == 0 || _stricmp(house->ID, name) == 0) {
                return house->House;
            }
        }
    }
    return -1;
}


// bool HouseTypeClass::Read_INI(CCINIClass &ini) (0x004F9550)
// The abstract type's keys, then the country keys: suffix, parent country
// and colour, prefix letter, the multipliers, multiplayer flags, the
// veteran infantry/unit/aircraft lists, and the side (moving this country
// from its old side's list to the new one's).
// The float multipliers unknown_0x100..unknown_0x148 are, in order,
// ArmorInfantryMult, ArmorUnitsMult, ArmorAircraftMult, ArmorBuildingsMult,
// ArmorDefensesMult, CostInfantryMult, CostUnitsMult, CostAircraftMult,
// CostBuildingsMult, CostDefensesMult, SpeedInfantryMult, SpeedUnitsMult,
// SpeedAircraftMult, BuildTimeInfantryMult, BuildTimeUnitsMult,
// BuildTimeAircraftMult, BuildTimeBuildingsMult, BuildTimeDefensesMult and
// IncomeMult.

extern "C" char *__cdecl strtok(char *string, char const *delimiters);


TYPE_LIST_READER(Get_Infantry_List, InfantryTypeClass, InfantryTypeClass::Find_Or_Make)
TYPE_LIST_READER(Get_Unit_List, UnitTypeClass, UnitTypeClass::Find_Or_Make)
TYPE_LIST_READER(Get_Aircraft_List, AircraftTypeClass, AircraftTypeClass::Find_Or_Make)

bool HouseTypeClass::Read_INI(CCINIClass &ini)
{
    if (AbstractTypeClass::Read_INI(ini)) {
        char buffer[32];
        ini.Get_String(ID, "Suffix", unknown_string_83c55c, buffer, sizeof(Suffix));
        if (strlen(buffer) != 0) {
            strcpy(Suffix, buffer);
        }
        ini.Get_String(ID, "ParentCountry", ID, buffer, sizeof(ParentCountry));
        ParentCountry = FixedString<25>(buffer);
        Color = ini.Get_ColorScheme(ID, "Color", Color);
        char prefix[2];
        buffer[0] = Prefix;
        buffer[1] = '\0';
        ini.Get_String(ID, "Prefix", buffer, prefix, sizeof(prefix));
        Prefix = prefix[0];
        FirepowerMult = ini.Get_Double(ID, "Firepower", FirepowerMult);
        GroundspeedMult = ini.Get_Double(ID, "Groundspeed", GroundspeedMult);
        AirspeedMult = ini.Get_Double(ID, "Airspeed", AirspeedMult);
        ArmorMult = ini.Get_Double(ID, "Armor", ArmorMult);
        ROFMult = ini.Get_Double(ID, "ROF", ROFMult);
        CostMult = ini.Get_Double(ID, "Cost", CostMult);
        BuildtimeMult = ini.Get_Double(ID, "BuildTime", BuildtimeMult);
        Multiplay = ini.Get_Bool(ID, "Multiplay", Multiplay);
        MultiplayPassive = ini.Get_Bool(ID, "MultiplayPassive", MultiplayPassive);
        WallOwner = ini.Get_Bool(ID, "WallOwner", WallOwner);
        SmartAI = ini.Get_Bool(ID, "SmartAI", SmartAI);
        unknown_0x100 = ini.Get_Double(ID, "ArmorInfantryMult", unknown_0x100);
        unknown_0x104 = ini.Get_Double(ID, "ArmorUnitsMult", unknown_0x104);
        unknown_0x108 = ini.Get_Double(ID, "ArmorAircraftMult", unknown_0x108);
        unknown_0x10C = ini.Get_Double(ID, "ArmorBuildingsMult", unknown_0x10C);
        unknown_0x110 = ini.Get_Double(ID, "ArmorDefensesMult", unknown_0x110);
        unknown_0x114 = ini.Get_Double(ID, "CostInfantryMult", unknown_0x114);
        unknown_0x118 = ini.Get_Double(ID, "CostUnitsMult", unknown_0x118);
        unknown_0x11C = ini.Get_Double(ID, "CostAircraftMult", unknown_0x11C);
        unknown_0x120 = ini.Get_Double(ID, "CostBuildingsMult", unknown_0x120);
        unknown_0x124 = ini.Get_Double(ID, "CostDefensesMult", unknown_0x124);
        unknown_0x128 = ini.Get_Double(ID, "SpeedInfantryMult", unknown_0x128);
        unknown_0x12C = ini.Get_Double(ID, "SpeedUnitsMult", unknown_0x12C);
        unknown_0x130 = ini.Get_Double(ID, "SpeedAircraftMult", unknown_0x130);
        unknown_0x134 = ini.Get_Double(ID, "BuildTimeInfantryMult", unknown_0x134);
        unknown_0x138 = ini.Get_Double(ID, "BuildTimeUnitsMult", unknown_0x138);
        unknown_0x13C = ini.Get_Double(ID, "BuildTimeAircraftMult", unknown_0x13C);
        unknown_0x140 = ini.Get_Double(ID, "BuildTimeBuildingsMult", unknown_0x140);
        unknown_0x144 = ini.Get_Double(ID, "BuildTimeDefensesMult", unknown_0x144);
        unknown_0x148 = ini.Get_Double(ID, "IncomeMult", unknown_0x148);
        VeteranInfantry = Get_Infantry_List(ini, ID, "VeteranInfantry", VeteranInfantry);
        VeteranUnits = Get_Unit_List(ini, ID, "VeteranUnits", VeteranUnits);
        VeteranAircraft = Get_Aircraft_List(ini, ID, "VeteranAircraft", VeteranAircraft);
        int side = Side;
        Side = ini.Get_Side(ID, "Side", side);
        if (Side != side) {
            if (side != -1) {
                Sides[side]->Houses.Delete(House);
            }
            if (Side != -1) {
                Sides[Side]->Houses.Add(House);
            }
        }
        return true;
    }
    return false;
}


// void HouseTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x004F9E40)

void HouseTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)House);
    crc((long)Side);
    crc((long)Color);
    crc(FirepowerMult);
    crc(GroundspeedMult);
    crc(AirspeedMult);
    crc(ArmorMult);
    crc(ROFMult);
    crc(CostMult);
    crc(BuildtimeMult);
    crc(Suffix, strlen(Suffix));
    crc(Prefix);
    crc(Multiplay);
}


// HRESULT __stdcall HouseTypeClass::IsDirty() (0x004F9F50)

HRESULT __stdcall HouseTypeClass::IsDirty()
{
    return S_OK;
}


// HRESULT __stdcall HouseTypeClass::Load(IStream *stream) (0x004F9F60)
// Empties the veteran lists, loads the abstract type, restores the house
// type in place, rebuilds the three veteran lists from their saved counts
// and entries and swizzles them.

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

HRESULT __stdcall HouseTypeClass::Load(IStream *stream)
{
    VeteranInfantry.Clear();
    VeteranUnits.Clear();
    VeteranAircraft.Clear();
    HRESULT result = AbstractTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) HouseTypeClass(NoInitClass());
    Load_List(stream, VeteranInfantry);
    Load_List(stream, VeteranUnits);
    Load_List(stream, VeteranAircraft);
    int index;
    for (index = 0; index < VeteranInfantry.Count(); index++) {
        Swizzle_Pointer(VeteranInfantry[index]);
    }
    for (index = 0; index < VeteranUnits.Count(); index++) {
        Swizzle_Pointer(VeteranUnits[index]);
    }
    for (index = 0; index < VeteranAircraft.Count(); index++) {
        Swizzle_Pointer(VeteranAircraft[index]);
    }
    return S_OK;
}


// HRESULT __stdcall HouseTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x004FA150)
// The AbstractClass data, then each veteran list as a count and its entries.

HRESULT __stdcall HouseTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    int count = VeteranInfantry.Count();
    stream->Write(&count, sizeof(count), 0);
    for (int index = 0; index < count; index++) {
        stream->Write(&VeteranInfantry[index], sizeof(InfantryTypeClass *), 0);
    }
    count = VeteranUnits.Count();
    stream->Write(&count, sizeof(count), 0);
    for (index = 0; index < count; index++) {
        stream->Write(&VeteranUnits[index], sizeof(UnitTypeClass *), 0);
    }
    count = VeteranAircraft.Count();
    stream->Write(&count, sizeof(count), 0);
    for (index = 0; index < count; index++) {
        stream->Write(&VeteranAircraft[index], sizeof(AircraftTypeClass *), 0);
    }
    return S_OK;
}


// HRESULT __stdcall HouseTypeClass::GetSizeMax(ULARGE_INTEGER *size) (0x004FA240)

HRESULT __stdcall HouseTypeClass::GetSizeMax(ULARGE_INTEGER *size)
{
    if (size == 0) {
        return E_POINTER;
    }
    size->HighPart = 0;
    size->LowPart = sizeof(HouseTypeClass) + 4;
    return S_OK;
}


// HRESULT __stdcall HouseTypeClass::QueryInterface(GUID const &iid, void **object) (0x004FA270)
// Same answers as AbstractClass::QueryInterface (its own copy).

HRESULT __stdcall HouseTypeClass::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    *object = 0;
    if (iid == IID_IUnknown_7afce0) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_IPersist) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_IPersistStream) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_IRTTITypeInfo) {
        *object = static_cast<IRTTITypeInfo *>(this);
    }
    if (*object == 0) {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}


// HRESULT __stdcall HouseTypeClass::GetClassID(GUID *classid) (0x004FA310)

HRESULT __stdcall HouseTypeClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_HouseTypeClass;
    return S_OK;
}


// HouseTypeClass *__fastcall HouseTypeClass::Find_Or_Make(char const *name) (0x004FA350)
// The type with this ID, a new one when there is none, null for "<none>"/"none".

HouseTypeClass *__fastcall HouseTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < HouseTypes.Count(); index++) {
        if (_stricmp(HouseTypes[index]->ID, name) == 0) {
            return HouseTypes[index];
        }
    }
    return new HouseTypeClass(name);
}


// int HouseTypeClass::Kind_Of() const (0x004FA3E0): the house type RTTI kind.

int HouseTypeClass::Kind_Of() const
{
    return 0x0E;
}


// int HouseTypeClass::Size_Of() const (0x004FA3F0)

int HouseTypeClass::Size_Of() const
{
    return sizeof(HouseTypeClass);
}


// int HouseTypeClass::Fetch_Heap_ID() const (0x004FA400)

int HouseTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// unsigned long __stdcall HouseTypeClass::AddRef() (0x004FA410): types are not counted.

unsigned long __stdcall HouseTypeClass::AddRef()
{
    return 1;
}


// unsigned long __stdcall HouseTypeClass::Release() (0x004FA420)

unsigned long __stdcall HouseTypeClass::Release()
{
    return 1;
}


// HouseTypeClass scalar deleting destructor (0x004FA430): compiler generated
// after the unit's last member (the inlined destructor, then operator delete).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<AircraftTypeClass *>::TypeList, default constructor (0x004FA630; vtable 0x007A30B8).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<InfantryTypeClass *>::Clear (0x004FA570; TypeList<InfantryTypeClass *> vtable 0x007a30f8 slot 3).
// DynamicVectorClass<InfantryTypeClass *>::ID_pointer (0x004FA5A0; TypeList<InfantryTypeClass *> vtable 0x007a30f8 slot 5).
// VectorClass<InfantryTypeClass *>::Fetch (0x004FA5C0; VectorClass<InfantryTypeClass *> vtable 0x007a3118 slot 6).
// DynamicVectorClass<UnitTypeClass *>::Clear (0x004FA5D0; TypeList<UnitTypeClass *> vtable 0x007a30d8 slot 3).
// DynamicVectorClass<UnitTypeClass *>::ID_pointer (0x004FA600; TypeList<UnitTypeClass *> vtable 0x007a30d8 slot 5).
// VectorClass<UnitTypeClass *>::Fetch (0x004FA620; VectorClass<UnitTypeClass *> vtable 0x007a3138 slot 6).
// DynamicVectorClass<AircraftTypeClass *>::Clear (0x004FA660; TypeList<AircraftTypeClass *> vtable 0x007a30b8 slot 3).
// DynamicVectorClass<AircraftTypeClass *>::ID_pointer (0x004FA690; TypeList<AircraftTypeClass *> vtable 0x007a30b8 slot 5).
// VectorClass<AircraftTypeClass *>::Fetch (0x004FA6B0; VectorClass<AircraftTypeClass *> vtable 0x007a3158 slot 6).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<InfantryTypeClass *> scalar deleting destructor (0x004FA6C0; vtable 0x007A30F8 slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x007A3118. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x00652E90.
// TypeList<UnitTypeClass *> scalar deleting destructor (0x004FA710; vtable 0x007A30D8 slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x007A3138. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x00652DC0.
// TypeList<AircraftTypeClass *> scalar deleting destructor (0x004FA760; vtable 0x007A30B8 slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x007A3158. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x004FA630.


// VectorClass<InfantryTypeClass *> scalar deleting destructor (0x004FA7B0): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// VectorClass<UnitTypeClass *> scalar deleting destructor (0x004FA800): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// VectorClass<AircraftTypeClass *> scalar deleting destructor (0x004FA850): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// VectorClass<InfantryTypeClass *> operator assign (0x004FA8A0): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<InfantryTypeClass *>::operator equal (0x004FA910; VectorClass<InfantryTypeClass *> vtable 0x007a3118 slot 1).
// VectorClass<InfantryTypeClass *>::Resize (0x004FA950; VectorClass<InfantryTypeClass *> vtable 0x007a3118 slot 2).
// VectorClass<InfantryTypeClass *>::Clear (0x004FAA00; VectorClass<InfantryTypeClass *> vtable 0x007a3118 slot 3).
// VectorClass<InfantryTypeClass *>::ID_object (0x004FAA30; VectorClass<InfantryTypeClass *> vtable 0x007a3118 slot 4).
// VectorClass<InfantryTypeClass *>::ID_pointer (0x004FAA70; VectorClass<InfantryTypeClass *> vtable 0x007a3118 slot 5).


// DynamicVectorClass<InfantryTypeClass *> scalar deleting destructor (0x004FAAF0): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<InfantryTypeClass *>::Add (0x004FABF0): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<InfantryTypeClass *>::Resize (0x004FAB40; TypeList<InfantryTypeClass *> vtable 0x007a30f8 slot 2).
// DynamicVectorClass<InfantryTypeClass *>::ID_object (0x004FAC40; TypeList<InfantryTypeClass *> vtable 0x007a30f8 slot 4).


// VectorClass<UnitTypeClass *> operator assign (0x004FAC70): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<UnitTypeClass *>::operator equal (0x004FACE0; VectorClass<UnitTypeClass *> vtable 0x007a3138 slot 1).
// VectorClass<UnitTypeClass *>::Resize (0x004FAD20; VectorClass<UnitTypeClass *> vtable 0x007a3138 slot 2).
// VectorClass<UnitTypeClass *>::Clear (0x004FADD0; VectorClass<UnitTypeClass *> vtable 0x007a3138 slot 3).
// VectorClass<UnitTypeClass *>::ID_object (0x004FAE00; VectorClass<UnitTypeClass *> vtable 0x007a3138 slot 4).
// VectorClass<UnitTypeClass *>::ID_pointer (0x004FAE40; VectorClass<UnitTypeClass *> vtable 0x007a3138 slot 5).


// DynamicVectorClass<UnitTypeClass *> scalar deleting destructor (0x004FAEC0): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<UnitTypeClass *>::Add (0x004FAFC0): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<UnitTypeClass *>::Resize (0x004FAF10; TypeList<UnitTypeClass *> vtable 0x007a30d8 slot 2).
// DynamicVectorClass<UnitTypeClass *>::ID_object (0x004FB010; TypeList<UnitTypeClass *> vtable 0x007a30d8 slot 4).


// VectorClass<AircraftTypeClass *> operator assign (0x004FB040): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<AircraftTypeClass *>::operator equal (0x004FB0B0; VectorClass<AircraftTypeClass *> vtable 0x007a3158 slot 1).
// VectorClass<AircraftTypeClass *>::Resize (0x004FB0F0; VectorClass<AircraftTypeClass *> vtable 0x007a3158 slot 2).
// VectorClass<AircraftTypeClass *>::Clear (0x004FB1A0; VectorClass<AircraftTypeClass *> vtable 0x007a3158 slot 3).
// VectorClass<AircraftTypeClass *>::ID_object (0x004FB1D0; VectorClass<AircraftTypeClass *> vtable 0x007a3158 slot 4).
// VectorClass<AircraftTypeClass *>::ID_pointer (0x004FB210; VectorClass<AircraftTypeClass *> vtable 0x007a3158 slot 5).


// DynamicVectorClass<AircraftTypeClass *> scalar deleting destructor (0x004FB290): instantiated in the HouseType unit by HouseTypeClass::Read_INI's
// veteran lists from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AircraftTypeClass *>::Add (0x004FB390): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AircraftTypeClass *>::Resize (0x004FB2E0; TypeList<AircraftTypeClass *> vtable 0x007a30b8 slot 2).
// DynamicVectorClass<AircraftTypeClass *>::ID_object (0x004FB3E0; TypeList<AircraftTypeClass *> vtable 0x007a30b8 slot 4).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<AircraftTypeClass *>::TypeList, copy constructor (0x004FB410; vtable 0x007A30B8).


extern "C" unsigned char table_4fb5c0[]; extern "C" void* __cdecl storage_4fb5c0(unsigned int);
class Storage_4fb5c0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4fb5c0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4fb5c0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4fb5c0(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_4fb660[]; extern "C" void* __cdecl storage_4fb660(unsigned int);
class Storage_4fb660 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4fb660::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4fb660; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4fb660(requested << 2); owns=1; } return this; }


// HouseTypeClass::QueryInterface adjustor thunk for IRTTITypeInfo (0x004FB6B0)
// Compiler generated (this - 4, then QueryInterface) at the end of the unit.


// HouseTypeClass::AddRef adjustor thunk for IRTTITypeInfo (0x004FB6C0)


// HouseTypeClass::Release adjustor thunk for IRTTITypeInfo (0x004FB6D0)
