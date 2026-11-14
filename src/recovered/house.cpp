// house.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/house.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/house.hpp"
#define RA2_HOUSE_SCREEN_AI
#include "shared/map.hpp"
#include "shared/building_class.hpp"
#include "shared/techno.hpp"
#include "shared/house_views.hpp"
#include "shared/building_type_list.hpp"
#include "shared/unit.hpp"
#include "shared/building_type_class.hpp"
#include "shared/rules.hpp"
#include "shared/waypoint_path.hpp"
#include "shared/infantry.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/super.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/house_type.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/aircraft.hpp"
#include "shared/infantry_type.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/tag.hpp"
#include "shared/team_type.hpp"
#include "shared/type_list_decl.hpp"
#include "shared/options.hpp"
#include "shared/session_state.hpp"
#include "shared/special.hpp"
#include "shared/vox_class.hpp"
#include "shared/system_timer.hpp"
#include "shared/radar_event.hpp"
#include "shared/tiberium.hpp"
#include "shared/string_table.hpp"
#include "shared/uicmnds.hpp"
#include "shared/target.hpp"
#include "shared/unknown_list_832470.hpp"
#include "shared/factory_class.hpp"
#include "shared/id_source.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tactical.hpp"
#include "shared/mouse.hpp"
#include "shared/sound_play.hpp"
#include "shared/debug_printf.hpp"
#include "shared/trigger.hpp"
#include "shared/lobby.hpp"
#include "shared/planning.hpp"
#include "shared/dsurface.hpp"
#include "shared/crt_memory.hpp"
#include "shared/coord_distance.hpp"
#include "shared/team.hpp"
#include "shared/crt_string.hpp"
#include "shared/techno_type.hpp"
#include "shared/facing.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/foot.hpp"
#include "shared/color_scheme.hpp"
#include "shared/convert.hpp"
#include "shared/hicolor.hpp"
#include "shared/com_persist.hpp"
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#define LOAD_LIST(stream, list, type)                                       \
    {                                                                       \
        count = 0;                                                          \
        result = (stream)->Read(&count, sizeof(count), 0);                  \
        if (FAILED(result)) {                                               \
            return result;                                                  \
        }                                                                   \
        while (count--) {                                                   \
            type value = 0;                                                 \
            result = (stream)->Read(&value, sizeof(value), 0);              \
            if (FAILED(result)) {                                           \
                return result;                                              \
            }                                                               \
            (list).Add(value);                                              \
        }                                                                   \
    }
#define SAVE_LIST(stream, list, type)                                       \
    {                                                                       \
        count = (list).Count();                                             \
        HRESULT list_result = (stream)->Write(&count, sizeof(count), 0);    \
        if (FAILED(list_result)) {                                          \
            return list_result;                                             \
        }                                                                   \
        for (int index = 0; index < count; index++) {                       \
            list_result = (stream)->Write(&(list)[index], sizeof(type), 0); \
            if (FAILED(list_result)) {                                      \
                return list_result;                                         \
            }                                                               \
        }                                                                   \
    }
#include "shared/cell_class.hpp"
#include "shared/path_estimator.hpp"
#include "shared/unit_type.hpp"
#include "shared/ini_class.hpp"
#include "shared/house_adjacent.hpp"
#include "shared/base.hpp"
#include "shared/vector_class.hpp"
#include "shared/rect.hpp"
#include "shared/discrete_distribution.hpp"
#include "shared/task_force.hpp"
#include "shared/object.hpp"
#include "shared/house_nodes.hpp"
#include "shared/unknown_grow_array.hpp"


// Header-static dynamic initializers of the unit at 0x004E13F0 and its
// list globals (HouseClass_BuildChoiceClass); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis.


DynamicVectorClass<HouseClass::BuildChoiceClass *> HouseClass::BuildChoiceClass::Array;


// Dynamic initializer (0x004E1410, _$E5) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E1440, _$E8) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E1460, _$E11) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E1480, _$E14) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E14A0, _$E17) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E14C0, _$E20) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E14E0, _$E23) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E1520, _$E26) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E1550, _$E29) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E1580, _$E32) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E15B0, _$E35) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E15C0, _$E38) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// Dynamic initializer (0x004E15E0, _$E45) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// atexit destructor (0x004E1620, _$E42) emitted by HeaderStatics_empty_rect_init_004e13f0.cpp; no source of its own.


// House screen operations used by this population; full screen providers live in their own units.
// HouseClass::HouseClass(NoInitClass const &noinit) (0x004E1680)
// The constructor used when loading: the lists, the +0x1C8 timer, the
// counters and the base records are constructed empty; everything else is
// restored by Load.

HouseClass::HouseClass(NoInitClass const &noinit)
    : AbstractClass(noinit),
      unknown_1d4(noinit),
      unknown_1e0(noinit),
      unknown_1ec(noinit),
      unknown_1f8(noinit),
      Tiberium(noinit),
      unknown_254(noinit),
      unknown_268(noinit),
      unknown_a70(noinit),
      unknown_1278(noinit),
      unknown_1a80(noinit),
      unknown_2288(noinit),
      unknown_2a90(noinit),
      unknown_3298(noinit),
      unknown_3aa0(noinit),
      unknown_42a8(noinit),
      unknown_4ab0(noinit),
      unknown_54e0(noinit),
      unknown_5524(noinit),
      unknown_5530(noinit),
      unknown_55e9(noinit),
      unknown_55ec(noinit),
      unknown_55f0(noinit),
      unknown_5678(noinit),
      unknown_5684(noinit),
      unknown_5690(noinit),
      unknown_569c(noinit),
      unknown_56a8(noinit),
      unknown_56b4(noinit),
      unknown_56c0(noinit),
      IniName(noinit),
      unknown_15ef5(noinit)
{
}


// HouseClass normal constructor004E18C0; complete3860-byte body plus12 frozen NOP bytes.
// Layout names remain hypotheses where current shared headers do not identify them.
extern DynamicVectorClass<AircraftTypeClass *> AircraftTypes;
extern DynamicVectorClass<InfantryTypeClass *> InfantryTypes;
extern DynamicVectorClass<UnitTypeClass *> UnitTypes;
extern "C" wchar_t *__cdecl wcscpy(wchar_t *, wchar_t const *);

HouseClass::HouseClass(HouseTypeClass *type)
    : AbstractClass(),
      ArrayIndex(-1),
      Type(type),
      unknown_c8(unknown_id_source_a3d290->unknown_610),
      unknown_d0(1.0),
      unknown_d8(1.0),
      unknown_e0(1.0),
      unknown_e8(1.0),
      unknown_f0(1.0),
      unknown_f8(1.0),
      unknown_100(1.0),
      unknown_108(0.0),
      unknown_110(0.0),
      unknown_118(0),
      unknown_11c(1),
      unknown_120(0),
      unknown_124(0),
      unknown_128(0),
      unknown_12c(0),
      unknown_134(0),
      unknown_135(0),
      unknown_136(0),
      unknown_137(0),
      unknown_138(1),
      unknown_139(0),
      unknown_13a(0),
      unknown_13b(0),
      unknown_13c(0),
      unknown_13d(0),
      unknown_13e(0),
      unknown_13f(0),
      unknown_140(0),
      unknown_141(0),
      unknown_142(0),
      unknown_143(0),
      unknown_144(1),
      unknown_148(0),
      unknown_14c(0),
      unknown_150(0),
      unknown_154(-1),
      unknown_188(0),
      unknown_189(0),
      unknown_18a(0),
      unknown_18b(0),
      unknown_18c(0),
      unknown_18d(0),
      unknown_18e(0),
      unknown_18f(0),
      unknown_190(0),
      unknown_191(0),
      unknown_192(1),
      unknown_194(unknown_118),
      unknown_198(0),
      unknown_1b4(-1),
      unknown_1b8(-1),
      unknown_1bc(-1),
      unknown_1c0(-1),
      unknown_1c4(0),
      unknown_204(0),
      unknown_205(0),
      unknown_206(0),
      unknown_207(0),
      unknown_208(0),
      unknown_20c(0),
      unknown_210(0),
      unknown_214(0),
      unknown_218(0),
      unknown_21c(0),
      unknown_220(0),
      unknown_224(0),
      unknown_228(0),
      unknown_22c(0),
      CurBuildings(0),
      unknown_234(0),
      unknown_238(0),
      Balance(0),
      Capacity(0),
      unknown_52b8(0),
      unknown_52bc(0),
      unknown_52c0(0),
      unknown_52c4(0),
      unknown_52c8(0),
      unknown_52cc(0),
      Power(0),
      Drain(0),
      unknown_52d8(0),
      unknown_52dc(0),
      unknown_52e0(0),
      unknown_52e4(0),
      unknown_52e8(0),
      unknown_52ec(0),
      unknown_52f0(0),
      unknown_52f4(0),
      unknown_52f8(0),
      unknown_52fc(0),
      unknown_52fd(0),
      unknown_52fe(0),
      unknown_52ff(0),
      unknown_5300(0),
      unknown_5301(0),
      unknown_5302(0),
      unknown_5303(0),
      unknown_5304(0),
      unknown_5308(0),
      unknown_530c(0, 0),
      unknown_5360(0),
      unknown_53b4(0),
      unknown_53b8(-1),
      BaseCenter(0, 0, 0),
      unknown_53c8(0),
      unknown_5408(0),
      unknown_540c(-1),
      unknown_5410(0),
      unknown_5414(0),
      unknown_5418(0),
      unknown_541c(1),
      unknown_54ec(0),
      unknown_54f0(-1),
      unknown_553c(-1),
      unknown_5540(-1),
      unknown_5544(-1),
      unknown_5548(-1),
      unknown_554c(100),
      unknown_5550(75),
      unknown_5554(75),
      unknown_5558(75),
      unknown_555c(0),
      unknown_55e4(0),
      unknown_55e8(0),
      unknown_55e9(0, 0, 0),
      unknown_55ec(255, 255, 255),
      unknown_5668(1),
      unknown_5669(1),
      unknown_566a(0),
      EMPDest(0, 0),
      NukeDest(0, 0),
      Allies(0),
      unknown_5678(int(900 * Rules->DamageDelay)),
      unknown_5684(1),
      unknown_5690(0),
      unknown_569c(1),
      unknown_56a8(1),
      unknown_56b4(1),
      unknown_56c0(1),
      unknown_56cc(0),
      ColorSchemeIndex(0),
      unknown_15f44(0),
      unknown_15f60(0),
      unknown_15f64(0),
      unknown_15f68(1.0),
      unknown_15f70(1.0),
      unknown_15f7c(0.33f),
      unknown_15f80(0.33f),
      unknown_15f84(0.34f),
      unknown_15f88(0)
{
    unknown_15ef5 = FixedString<32>(unknown_string_83c55c);
    UIName[0] = 0;
    Create_ID();
    if (Type != 0) {
        ColorSchemeIndex = Type->Color;
    }
    unknown_objects_ac0cb0.Add(this);
    unknown_abstracts_ac0c80.Add(this);
    unknown_objects_ac0c30.Add(this);
    unknown_objects_ac0c58.Add(this);
    unknown_AbstractClass_list_00ac0d60.Add(this);
    ArrayIndex = Houses.Count();
    if (type != 0) {
        for (int index = 0; index < Houses.Count(); index++) {
            Houses[index]->AngerNodes.Add(AngerStruct(this, 0));
            AngerNodes.Add(AngerStruct(Houses[index], 0));
            Houses[index]->ScoutNodes.Add(ScoutStruct(this, false));
            ScoutNodes.Add(ScoutStruct(Houses[index], false));
        }
    }
    Houses.Add(this);
    memset(unknown_56d0, 0, sizeof(unknown_56d0));
    for (int index = 0; index < SuperWeaponTypes.Count(); index++) {
        Supers.Add(new SuperClass(SuperWeaponTypes[index], this));
    }
    memset(unknown_5310, 0, sizeof(unknown_5310));
    memset(unknown_5364, 0, sizeof(unknown_5364));
    if (type != 0) {
        IniName = (char const *)type->ID;
        wcscpy(UIName, type->UIName);
    }
    memset(unknown_56d0, 0, sizeof(unknown_56d0));
    unknown_120 |= 1 << ArrayIndex;
    int attack = Random_Pick(450, 1800);
    attack = int(attack * Rules->AttackDelay);
    unknown_54e0 = attack;
    unknown_54ec = attack;
    unknown_268.unknown_func_70c4d0(AircraftTypes.Count());
    unknown_a70.unknown_func_70c4d0(InfantryTypes.Count());
    unknown_1278.unknown_func_70c4d0(UnitTypes.Count());
    unknown_1a80.unknown_func_70c4d0(BuildingTypes.Count());
    unknown_2288.unknown_func_70c4d0(AircraftTypes.Count());
    unknown_2a90.unknown_func_70c4d0(InfantryTypes.Count());
    unknown_3298.unknown_func_70c4d0(UnitTypes.Count());
    unknown_3aa0.unknown_func_70c4d0(BuildingTypes.Count());
    unknown_42a8.unknown_func_70c4d0(BuildingTypes.Count());
    unknown_4ab0.unknown_func_70c4d0(19);
    for (index = 0; index < 12; index++) {
        WaypointPaths[index] = 0;
    }
    if (type != 0) {
        unknown_143 = true;
    }
    unknown_130 = -1;
    if (Type != 0) {
        if (Type->Side == 0) {
            unknown_130 = 0;
        } else if (Type->Side == 1) {
            unknown_130 = 1;
        }
    }
    IUnknown *unknown = 0;
    QueryInterface(IID_IUnknown_7afce0, (void **)&unknown);
    unknown_15f60 = new ConnectionPointClass(unknown_iid_7a1ac0, unknown);
    IConnectionPoint *connection;
    unknown_15f60->QueryInterface(IID_IConnectionPoint, (void **)&connection);
    ConnectionPoints.Add(connection);
    unknown->Release();
}


// House income capability, original004E27E0;481body+15padding.

bool HouseClass::Can_Make_Money_004e27e0()
{
    int credits=Available_Money();
    int refinery_cost=unknown_func_4ef600(Rules->BuildRefinery)->Cost_Of(this);
    int harvester_cost=Unknown_004EF6F0(Rules->unknown_9ec)->Cost_Of(this);
    bool has_refinery=unknown_5460.unknown_func_491d10(Rules->BuildRefinery[0]->ArrayIndex)>0 ||
                      unknown_5460.unknown_func_491d10(Rules->BuildRefinery[1]->ArrayIndex)>0;
    bool has_harvester=unknown_5470.unknown_func_491d10(Rules->unknown_9ec[0]->ArrayIndex)>0 ||
                       unknown_5470.unknown_func_491d10(Rules->unknown_9ec[1]->ArrayIndex)>0;
    if (has_refinery) {
        if (has_harvester) return true;
        bool has_factory=false;
        for (int index=0;index<Rules->BuildWeapons.Count();++index) {
            has_factory=has_factory || unknown_5460.unknown_func_491d10(Rules->BuildWeapons[index]->ArrayIndex)>0;
        }
        int factory_cost=Rules->BuildWeapons[0]->Cost_Of(this);
        if ((has_factory && credits>=harvester_cost) || credits>=harvester_cost+factory_cost ||
            credits>=refinery_cost) return true;
        return false;
    }
    if (credits>refinery_cost) return true;
    return false;
}


// HRESULT __stdcall HouseClass::QueryInterface(GUID const &iid, void **object) (0x004E29D0)
// IUnknown, IPersist and IPersistStream give the object; IHouse,
// IPublicHouse and IConnectionPointContainer their interfaces.

HRESULT __stdcall HouseClass::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    *object = 0;
    if (iid == IID_IUnknown_7afce0) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_IHouse) {
        *object = static_cast<IHouse *>(this);
    }
    if (iid == IID_IPublicHouse) {
        *object = static_cast<IPublicHouse *>(this);
    }
    if (iid == IID_IConnectionPointContainer) {
        *object = static_cast<IConnectionPointContainer *>(this);
    }
    if (iid == IID_IPersist) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_IPersistStream) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (*object == 0) {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}


// bool __stdcall HouseClass::Is_Powered() (0x004E2AB0)

bool __stdcall HouseClass::Is_Powered()
{
    return Power >= Drain;
}


// IApplication *__stdcall HouseClass::Get_Application() (0x004E2AD0)

IApplication *__stdcall HouseClass::Get_Application()
{
    IApplication *application;
    Unknown_00426C60(&unknown_application_84c7dc, IID_IApplication_79c3d0, (void **)&application);
    return application;
}


// BSTR __stdcall HouseClass::Name() (0x004E2AF0)

BSTR __stdcall HouseClass::Name()
{
    wchar_t buffer[32];
    MultiByteToWideChar(0, 1, Type->Name, -1, buffer, 32);
    return SysAllocString(buffer);
}


// long __stdcall HouseClass::Available_Money() (0x004E2B30)

long __stdcall HouseClass::Available_Money()
{
    float scale = Type->unknown_0x148;
    return (long)(Tiberium.Get_Total_Value() * scale + Balance);
}


// long __stdcall HouseClass::Available_Storage() (0x004E2B70)

long __stdcall HouseClass::Available_Storage()
{
    return Capacity - (int)Tiberium.Get_Total_Amount();
}


// long __stdcall HouseClass::Power_Output() (0x004E2BA0)

long __stdcall HouseClass::Power_Output()
{
    return Power;
}


// long __stdcall HouseClass::Power_Drain() (0x004E2BB0)

long __stdcall HouseClass::Power_Drain()
{
    return Drain;
}


// long __stdcall HouseClass::Category_Quantity(int category) (0x004E2BC0)
// The number of this house's technos of a category.

long __stdcall HouseClass::Category_Quantity(int category)
{
    int count = 0;
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno->House == this && techno->Techno_Type_Class()->Category == category) {
            count++;
        }
    }
    return count;
}


// long __stdcall HouseClass::Public_Category_Quantity(int category) (0x004E2C20)
// The number of this house's technos of a category.

long __stdcall HouseClass::Public_Category_Quantity(int category)
{
    int count = 0;
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno->House == this && techno->Techno_Type_Class()->Category == category) {
            count++;
        }
    }
    return count;
}


// long __stdcall HouseClass::Category_Power(int category) (0x004E2C80)
// The summed power values of this house's technos of a category.

long __stdcall HouseClass::Category_Power(int category)
{
    int power = 0;
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        TechnoTypeClass *type = techno->Techno_Type_Class();
        if (techno->House == this && type->Category == category) {
            power += type->unknown_59c + type->ThreatPosed;
        }
    }
    return power;
}


// long __stdcall HouseClass::Public_Category_Power(int category) (0x004E2CF0)
// The summed power values of this house's technos of a category.

long __stdcall HouseClass::Public_Category_Power(int category)
{
    int power = 0;
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        TechnoTypeClass *type = techno->Techno_Type_Class();
        if (techno->House == this && type->Category == category) {
            power += type->unknown_59c + type->ThreatPosed;
        }
    }
    return power;
}


// Cell __stdcall HouseClass::Base_Center() (0x004E2D60)

Cell __stdcall HouseClass::Base_Center()
{
    return Coord_Cell(BaseCenter);
}


// Cell __stdcall HouseClass::Public_Base_Center() (0x004E2DA0)

Cell __stdcall HouseClass::Public_Base_Center()
{
    return Coord_Cell(BaseCenter);
}


// long __stdcall HouseClass::ID_Number() (0x004E2DE0)

long __stdcall HouseClass::ID_Number()
{
    return ArrayIndex;
}


// double HouseClass::Tiberium_Fraction() const (0x004E2DF0)

double HouseClass::Tiberium_Fraction() const
{
    if ((int)Tiberium.Get_Total_Amount() == 0) {
        return 0.0;
    }
    return Tiberium.Get_Total_Amount() / Capacity;
}


// void Unknown_004E2E30() (0x004E2E30)
// Empty in this build (Init_Bulk_Data calls it in the One_Time sequence; TibSun's
// HouseClass One_Time position).

void Unknown_004E2E30()
{
}


// int HouseClass::Assign_Handicap(int difficulty) (0x004E2E40)
// Takes the difficulty's modifiers from Rules (scaled by the house type's in
// multiplayer; speeds and build time by the game speed bias), restarts the
// team timer and returns the previous difficulty (name after TS).

int HouseClass::Assign_Handicap(int difficulty)
{
    int old = unknown_c8;
    unknown_c8 = difficulty;
    if (unknown_game_mode_a3d298) {
        HouseTypeClass *type = Type;
        unknown_d0 = Rules->Diff[difficulty].FirePower * type->FirepowerMult;
        unknown_d8 = Rules->Diff[difficulty].GroundSpeed * Rules->GameSpeedBias * type->GroundspeedMult;
        unknown_e0 = Rules->Diff[difficulty].AirSpeed * Rules->GameSpeedBias * type->AirspeedMult;
        unknown_e8 = Rules->Diff[difficulty].Armor * type->ArmorMult;
        unknown_f0 = Rules->Diff[difficulty].ROF * type->ROFMult;
        unknown_f8 = Rules->Diff[difficulty].Cost * type->CostMult;
        unknown_108 = Rules->Diff[difficulty].RepairDelay;
        unknown_110 = Rules->Diff[difficulty].BuildDelay;
        unknown_100 = Rules->Diff[difficulty].BuildTime * Rules->GameSpeedBias * type->BuildtimeMult;
    } else {
        unknown_d0 = Rules->Diff[difficulty].FirePower;
        unknown_d8 = Rules->Diff[difficulty].GroundSpeed * Rules->GameSpeedBias;
        unknown_e0 = Rules->Diff[difficulty].AirSpeed * Rules->GameSpeedBias;
        unknown_e8 = Rules->Diff[difficulty].Armor;
        unknown_f0 = Rules->Diff[difficulty].ROF;
        unknown_f8 = Rules->Diff[difficulty].Cost;
        unknown_108 = Rules->Diff[difficulty].RepairDelay;
        unknown_110 = Rules->Diff[difficulty].BuildDelay;
        unknown_100 = Rules->Diff[difficulty].BuildTime * Rules->GameSpeedBias;
    }
    unknown_5684 = CDTimerClass<FrameTimerClass>(Rules->TeamDelays[difficulty] + ArrayIndex * 175);
    return old;
}


// HouseClass::~HouseClass() (0x004E30C0)
// Deletes the house's waypoint paths, super weapons and tags, releases its
// connection points and removes it from the global lists and Houses.

extern bool unknown_flag_a40958;            // 0x00A40958
extern HouseClass *unknown_house_84e894;    // 0x0084E894

HouseClass::~HouseClass()
{
    Unknown_006ECB90((ObjectClass *)this, true);
    Type = 0;
    for (int index = 0; index < 12; index++) {
        delete WaypointPaths[index];
        WaypointPaths[index] = 0;
    }
    for (index = 0; index < Supers.Count(); index++) {
        delete Supers[index];
    }
    Supers.Clear();
    if (unknown_56cc != 0 && unknown_flag_a40958) {
        unknown_56cc->Release();
        unknown_56cc = 0;
    }
    while (unknown_38.Count() > 0) {
        delete unknown_38[0];
    }
    for (index = 0; index < ConnectionPoints.Count(); index++) {
        while (ConnectionPoints[index]->Release() > 0) {
        }
        ConnectionPoints[index] = 0;
    }
    ConnectionPoints.Clear();
    unknown_15f60 = 0;
    unknown_objects_ac0cb0.Delete((ObjectClass *)this);
    unknown_abstracts_ac0c80.Delete(this);
    unknown_objects_ac0c30.Delete(this);
    unknown_objects_ac0c58.Delete(this);
    unknown_AbstractClass_list_00ac0d60.Delete(this);
    if (unknown_house_84e894 == this) {
        unknown_house_84e894 = 0;
    }
    Houses.Delete(this);
}


// Original004E3640: initializes five32-bit state words and returns their base pointer.
// ABI hypothesis: ECX points at writable state, EAX returns that pointer, no stack arguments.
// No original incoming references are known; class ownership/constructor identity remains unknown.
extern "C" int *__fastcall Initialize_Five_Word_State_004e3640(int *state)
{
    state[0]=0;
    state[1]=1;
    state[2]=0;
    state[3]=0;
    state[4]=0;
    return state;
}


// int HouseClass::Can_Build(TechnoTypeClass const *type, bool forced, bool include_in_progress) -- 0x004E3660 (TibSun House.CPP)
// Whether the type can be built: 1 yes, 0 no, -1 at its build limit. The progress test is Factory_Producing_This expanded.

// The house type's bit in a house set (RequiredHouses, ForbiddenHouses).
static inline int House_Owner_Bit(HouseTypeClass const *type)
{
    return 1 << type->House;
}

int HouseClass::Can_Build(TechnoTypeClass const *type, bool forced, bool include_in_progress)
{
    if (!forced) {
        if (type->unknown_aa4) {
            return 0;
        }
        TypeList<int> over = type->PrerequisiteOverride;
        for (int i = 0; i < over.Count(); i++) {
            int id = over[i];
            if (unknown_5460.unknown_func_491d10(id) > 0) {
                goto skip;
            }
        }
        if (type->TechLevel == -1) {
            return 0;
        }
        TypeList<int> pre = type->Prerequisite;

        int level = unknown_11c;
        if (type->RequiresStolenAlliedTech && !unknown_205) {
            return 0;
        }
        if (type->RequiresStolenSovietTech && !unknown_204) {
            return 0;
        }
        if (type->RequiredHouses != -1 && (type->RequiredHouses & House_Owner_Bit(Type)) == 0) {
            if ((type->RequiredHouses & unknown_208) == 0 || type->Kind_Of() != 0x10) {
                if ((type->RequiredHouses & unknown_20c) == 0 || type->Kind_Of() != 0x28) {
                    if ((type->RequiredHouses & unknown_210) == 0 || type->Kind_Of() != 3) {
                        if ((type->RequiredHouses & unknown_214) == 0 || type->Kind_Of() != 7) {
                            return 0;
                        }
                    }
                }
            }
        }
        if (type->ForbiddenHouses != -1 && (type->ForbiddenHouses & House_Owner_Bit(Type)) != 0) {
            return 0;
        }
        if (unknown_session_a3d2c3 == 0 && type->Kind_Of() == 7) {
            BuildingTypeClass const *btype = (BuildingTypeClass const *)type;
            if (btype->SuperWeapon != -1 && Supers[btype->SuperWeapon]->Type->DisableableFromShell) {
                return 0;
            }
        }
        if (type->TechLevel > level) {
            return 0;
        }
        if (!Is_Human_Control()) {
            return 1;
        }
        for (int index = 0; index < pre.Count(); index++) {
            switch (pre[index]) {
            case -1: {
                bool found = false;
                for (int j = 0; j < Rules->PrerequisitePower.Count(); j++) {
                    if (unknown_5460.unknown_func_491d10(Rules->PrerequisitePower[j]) > 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) return 0;
            } break;
            case -2: {
                bool found = false;
                for (int j = 0; j < Rules->PrerequisiteFactory.Count(); j++) {
                    if (unknown_5460.unknown_func_491d10(Rules->PrerequisiteFactory[j]) > 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) return 0;
            } break;
            case -3: {
                bool found = false;
                for (int j = 0; j < Rules->PrerequisiteBarracks.Count(); j++) {
                    if (unknown_5460.unknown_func_491d10(Rules->PrerequisiteBarracks[j]) > 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) return 0;
            } break;
            case -4: {
                bool found = false;
                for (int j = 0; j < Rules->PrerequisiteRadar.Count(); j++) {
                    if (unknown_5460.unknown_func_491d10(Rules->PrerequisiteRadar[j]) > 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) return 0;
            } break;
            case -5: {
                bool found = false;
                for (int j = 0; j < Rules->PrerequisiteTech.Count(); j++) {
                    if (unknown_5460.unknown_func_491d10(Rules->PrerequisiteTech[j]) > 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) return 0;
            } break;
            case -6: {
                bool found = false;
                for (int j = 0; j < Rules->PrerequisiteProc.Count(); j++) {
                    if (unknown_5460.unknown_func_491d10(Rules->PrerequisiteProc[j]) > 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) return 0;
            } break;
            default: {
                BuildingTypeClass *btype = BuildingTypes[pre[index]];
                if (btype->PowersUpBuilding[0] != '\0') {
                    BuildingClass *bptr;
                    bool found = false;
                    for (int j = Buildings.Count() - 1; j >= 0; j--) {
                        bptr = Buildings[j];
                        if (!bptr->unknown_76 && bptr->unknown_530) {
                            if (bptr->Get_Mission() != 0x13 && bptr->MissionQueue != 0x13) {
                                found = true;
                                break;
                            }
                        }
                    }
                    if (found) {
                        int k = 0;
                        while (true) {
                            if (bptr->unknown_4c0[k] == btype) {
                                goto breakout;
                            }
                            k++;
                            if (k >= 3) break;
                        }
                    }
                    return 0;
                }
                if (unknown_5460.unknown_func_491d10(pre[index]) == 0) return 0;
                breakout:;
            } break;
            }
        }
    }
skip:
    switch (type->Kind_Of()) {
    case 0x28: {
        int limit = type->BuildLimit;
        if (type->BuildLimit <= 0) {
            if (unknown_54b0.unknown_func_491d10(type->Fetch_Heap_ID()) >= abs(limit)) return 0;
        }
        if (limit <= 0) return 1;
        if (unknown_5430.unknown_func_491d10(type->Fetch_Heap_ID()) < type->BuildLimit) {
            return 1;
        }
        if (include_in_progress) {
            if (Factory_Producing_This_004e4090(type) != 0) return 1;
        }
        return -1;
    }
    case 0x10: {
        if (type->BuildLimit <= 0) {
            if (unknown_54c0.unknown_func_491d10(type->Fetch_Heap_ID()) >= abs(type->BuildLimit)) return 0;
        }
        int count = unknown_5440.unknown_func_491d10(type->Fetch_Heap_ID());
        if (((InfantryTypeClass const *)type)->VehicleThief) {
            for (int i = 0; i < Units.Count(); i++) {
                UnitClass *unit = Units[i];
                if (unit->House == this && unit->unknown_290 == type->Fetch_Heap_ID()) {
                    count++;
                }
            }
        }
        if (type->BuildLimit <= 0) {
            return 1;
        }
        if (count < type->BuildLimit) {
            return 1;
        }
        if (include_in_progress && count == type->BuildLimit) {
            if (Factory_Producing_This_004e4090(type) != 0) return 1;
        }
        return -1;
    }
    case 7: {
        int limit = type->BuildLimit;
        if (type->BuildLimit <= 0) {
            if (unknown_54a0.unknown_func_491d10(type->Fetch_Heap_ID()) >= abs(limit)) return 0;
        }
        if (limit <= 0) return 1;
        if (unknown_5420.unknown_func_491d10(type->Fetch_Heap_ID()) < type->BuildLimit) {
            return 1;
        }
        if (include_in_progress) {
            if (Factory_Producing_This_004e4090(type) != 0) return 1;
        }
        return -1;
    }
    case 3: {
        int limit = type->BuildLimit;
        if (type->BuildLimit <= 0) {
            if (unknown_54d0.unknown_func_491d10(type->Fetch_Heap_ID()) >= abs(limit)) return 0;
        }
        if (limit <= 0) return 1;
        if (unknown_5450.unknown_func_491d10(type->Fetch_Heap_ID()) < type->BuildLimit) {
            return 1;
        }
        if (include_in_progress) {
            if (Factory_Producing_This_004e4090(type) != 0) return 1;
        }
        return -1;
    }
    }
    return 1;
}


// Original004E4090:113body+15padding; inventory004E4094 is inside the first MOV.

FactoryClass *HouseClass::Factory_Producing_This_004e4090(TechnoTypeClass const *type) const
{
    FactoryClass *factory=0;
    for (int index=0;index<Factories.Count();++index) {
        if (Factories[index]->Get_House_004e4090()==this) {
            if (Factories[index]->Get_Object()!=0 &&
                Factories[index]->Get_Object()->Techno_Type_Class()==type) {
                factory=Factories[index];
                break;
            }
        }
    }
    return factory;
}


// House per-frame logic, original004E4110:3544 body bytes plus8 frozenNOP.
// Complete original1034-instruction CFG; inventory55-byte prefix/interior414C are not separate bodies.

// Incoming interface at House+56CC: its saved IID007A2030 equals the IAIHouse IID.
// Slot4's stdcall/long* call is verified on this target; names are supported by the same-IID TLB reference.
struct __declspec(novtable) IAIHouse : public IUnknown {
    virtual long __stdcall Link_House(IHouse *house) = 0;
    virtual long __stdcall AI(long *frame_delay) = 0;
};
extern bool player_wins_a35db1;
extern bool player_loses_a40c78;
extern int low_power_notice_a40fc8;
extern bool unknown_sell_mode_83368d;
extern CoordStruct const unknown_coord_a40f48;
extern char const eva_insufficient_funds_007cf9e8[];
extern char const txt_low_power_007da260[];
extern char const house_cpp_path_007da270[];

// Checked Foot view: target Foot constructor sets Abstract+14 bit2.
// Primary Abstract/Object base adjustment is zero; this performs the actual type test.
static inline FootClass *House_Foot_View(AbstractClass *object)
{
    if (object && object->AbstractFlags.IsFoot) return reinterpret_cast<FootClass *>(object);
    return 0;
}

void HouseClass::AI()
{
    if (unknown_1ec.Value()==1) {
        unknown_1ec = CDTimerClass<FrameTimerClass>(0);
        unknown_5668 = true;
    }
    if (unknown_1f8.Value()==1) {
        unknown_1f8 = CDTimerClass<FrameTimerClass>(0);
        unknown_5669 = true;
    }
    if (unknown_5668 != false) {
        Recalc_Power_004f2cc0();
        unknown_5669 = true;
    }
    if ((unknown_5669 != false) && (unknown_int_a3d548 == 0)) {
        unknown_func_4f2e50();
        unknown_func_4f2fc0();
    }
    if (Frame%100==0) {
        for (int index=0;index<AngerNodes.Count();++index) {
            if (AngerNodes[index].Value>1) --AngerNodes[index].Value;
        }
    }
    if (unknown_56cc) {
        long frame_delay=0;
        static_cast<IAIHouse *>(unknown_56cc)->AI(&frame_delay);
    }
    if (!Is_Human_Control() && (unknown_13b || unknown_194>=Rules->IQProduction)) {
        unknown_13b=true;
        unknown_136=true;
        unknown_137=true;
    }
    if (unknown_13f && unknown_1e0.Value()==0 && (unknown_game_mode_a3d298!=0 || unknown_1c4<=0)) {
        CDTimerClass<SystemTimerClass> speech_wait(120);
        while (Speech_Is_Playing() && speech_wait.Value()!=0) Service_Game_IO_00481fb0();
        unknown_13f=false;
        if (Is_Player_Control()) player_wins_a35db1=true;
        else if ((unknown_game_mode_a3d298==3 || unknown_game_mode_a3d298==4) && unknown_session_a3d29c &&
                 unknown_session_a3d29c->unknown_slot1() && unknown_session_a3d29c->unknown_slot2()) {
            if (Is_Human_Control()) player_wins_a35db1=true;
            else player_loses_a40c78=true;
        } else player_loses_a40c78=true;
    }
    if (unknown_140 && unknown_1e0.Value()==0) {
        CDTimerClass<SystemTimerClass> speech_wait(120);
        while (Speech_Is_Playing() && speech_wait.Value()!=0) Service_Game_IO_00481fb0();
        if (unknown_game_mode_a3d298==0) {
            if (Is_Player_Control()) player_loses_a40c78=true;
            else player_wins_a35db1=true;
        } else if (Is_Player_Control()) player_loses_a40c78=true;
        else player_wins_a35db1=true;
    }
    if (unknown_13e && unknown_1e0.Value()==0) {
        unknown_13e=false;
        unknown_func_4e7df0();
    }
    Power=Power>0 ? Power : 0;
    Drain=Drain>0 ? Drain : 0;
    if (unknown_530c!=unknown_cell_a40f10 && Frame%15==0) {
        FootClass *foot=0;
        ObjectClass *occupant=Map[unknown_530c].Cell_Techno(Point2D(0,0),false,0);
        if (occupant) {
            bool moving=false;
            foot=House_Foot_View(occupant);
            if (foot && foot->unknown_494) moving=true;
            if (!moving) occupant->Scatter(unknown_coord_a40f48,true,true);
            if (foot && foot->unknown_494) moving=true;
            if (!moving) {
                int count=0;
                while (!occupant->unknown_76 && count++<5) {
                    int damage=occupant->Strength;
                    occupant->Take_Damage(&damage,0,Rules->unknown_c74,0,true,0);
                }
            }
        }
    }
    if (unknown_5684.Value()==0 && !Is_Human_Control() && !Type->MultiplayPassive) {
        TypeList<TeamTypeClass const *> teams=Suggested_New_Team(false);
        if (teams.Count()>0) {
            for (int index=0;index<teams.Count();++index) {
                // The underlying recipes belong to mutable TeamTypes; the list's pointer qualification does not make those objects const.
                const_cast<TeamTypeClass *>(teams[index])->Create_One_Of(this);
            }
        }
        unknown_5684=CDTimerClass<FrameTimerClass>(Rules->TeamDelays[unknown_c8]);
    }
    if (unknown_player_a35db4==this) {
        if (!CurBuildings && unknown_sell_mode_83368d) Map.Unknown_0049C7C0(0);
        if (unknown_player_a35db4==this) {
            if (unknown_56c0.Value()==0 && Available_Money()<100 &&
                unknown_52c8+unknown_52c4+unknown_52c0+unknown_52bc>0) {
                Speech_Play_Name(eva_insufficient_funds_007cf9e8,-1,-1);
                Map.Unknown_006A1770();
                unknown_56c0=CDTimerClass<FrameTimerClass>(Options.Normalize_Delay((int)(Rules->SpeakDelay*900.0)));
            }
            if (unknown_56c0.Value()==0) {
                int capacity=Capacity;
                if (capacity-(int)Tiberium.Get_Total_Amount()<30 && capacity>50) {
                    unknown_56c0=CDTimerClass<FrameTimerClass>(Options.Normalize_Delay((int)(Rules->SpeakDelay*900.0)));
                }
            }
            if (Power_Fraction()<1.0) {
                if (unknown_5460.unknown_func_491d10(Rules->unknown_7c8[0]->ArrayIndex)>0 ||
                    unknown_5460.unknown_func_491d10(Rules->unknown_7c8[1]->ArrayIndex)>0) {
                    if (!low_power_notice_a40fc8) {
                        Speech_Play_Name("EVA_LowPower",-1,-1);
                        unknown_session_a3d298.Messages.Add_Message(0,0,StringTable::Load_String(txt_low_power_007da260,0,house_cpp_path_007da270,0x892),
                            ColorSchemeIndex,0x4046,(int)(Rules->MessageDelay*900.0),false);
                        low_power_notice_a40fc8=1;
                    }
                    unknown_56a8=CDTimerClass<FrameTimerClass>(Options.Normalize_Delay((int)(Rules->SpeakDelay*900.0)));
                }
            } else low_power_notice_a40fc8=0;
        }
    }
    ObjectClass *flag=static_cast<ObjectClass *>(unknown_5308);
    if (flag && flag->Kind_Of()==1) flag->Mark(2);
    if (unknown_5690.Value()==0 || unknown_18d) {
        unknown_5690=CDTimerClass<FrameTimerClass>(90);
        unknown_18d=false;
    }
    for (int super_index=0;super_index<Supers.Count();++super_index) {
        SuperClass *super=Supers[super_index];
        if (super->Is_Present() && super->unknown_69d390(this==unknown_player_a35db4) && this==unknown_player_a35db4) {
            int type_index=super->Type->Fetch_Heap_ID();
            Map.Unknown_0067C2D0(Unknown_006818D0(0x39,type_index,0));
        }
    }
    if (unknown_game_mode_a3d298!=0 && !unknown_13d && Frame>0 && !Type->MultiplayPassive) {
        bool defeated=false;
        if (unknown_session_a3d298.unknown_2a) {
            int units=unknown_5430.unknown_func_491d10(Rules->BaseUnit[0]->ArrayIndex)
                + unknown_5430.unknown_func_491d10(Rules->BaseUnit[1]->ArrayIndex);
            if (!CurBuildings && units<=0) defeated=true;
        } else if (!CurBuildings && !unknown_238 && !unknown_234) {
            int units=unknown_228;
            if (units && unknown_id_source_a3d290->Special().HarvesterImmune) {
                int immune=unknown_5430.unknown_func_491d10(Rules->unknown_9ec[0]->ArrayIndex)
                    + unknown_5430.unknown_func_491d10(Rules->unknown_9ec[1]->ArrayIndex);
                units-=immune;
            }
            if (units<=0) defeated=true;
        }
        if (defeated) {
            unknown_func_4e7df0();
            MPlayer_Defeated_004e77f0();
        }
    }
defeat_checked:;
    if (Frame%8==0) {
        for (int tag_index=unknown_38.Count()-1;tag_index>=0;--tag_index) {
            unknown_38[tag_index]->Spring(8,0,unknown_cell_a40f10,false,0);
        }
    }
    if (unknown_5524.Value()==0 && !Is_Human_Control() && !Type->MultiplayPassive) {
        unknown_5524=CDTimerClass<FrameTimerClass>(Expert_AI());
    }
    if (!Is_Human_Control() && !Type->MultiplayPassive) {
        int mode=unknown_12c;
        if (mode==0 || unknown_game_mode_a3d298==0) {
            AI_Building();
            AI_Unit();
            AI_Infantry();
            AI_Aircraft();
        } else if (mode==1) {
            AI_Building();
            if (unknown_553c==-1 || !BuildingTypes[unknown_553c]->Who_Can_Build_Me(true,true,true,this)) {
                AI_Unit();
                AI_Infantry();
                AI_Aircraft();
            }
        } else if (mode==2) {
            AI_Unit();
            UnitTypeClass *harvester=Unknown_004EF6F0(Rules->unknown_9ec);
            if (unknown_5540!=harvester->ArrayIndex) {
                AI_Infantry();
                AI_Aircraft();
            }
            bool none=unknown_5540==-1 && unknown_5544==-1 && unknown_5548==-1;
            if (unknown_5540!=-1 && !UnitTypes[unknown_5540]->Who_Can_Build_Me(true,true,true,this)) none=true;
            if (unknown_5544!=-1 && !InfantryTypes[unknown_5544]->Who_Can_Build_Me(true,true,true,this)) none=true;
            if ((unknown_5548!=-1 && !AircraftTypes[unknown_5548]->Who_Can_Build_Me(true,true,true,this)) || none) {
                AI_Building();
            }
        }
    }
    if (unknown_player_a35db4 == this) {
        if (unknown_144 != false) {
            unknown_144 = false;
            {
            int building_count=Buildings.Count();
            for (int index=0;index<building_count;++index) {
                BuildingClass *building=Buildings[index];
                if (building && building->unknown_7d && building->Strength>0 && building->Get_Mission()!=0x13 &&
                    building->MissionQueue!=0x13 && unknown_player_a35db4==building->House) {
                    building->unknown_slot283();
                }
            }
            Map.Unknown_0067E1C0();
            }
update_super_weapons:;
            Update_Present_Super_Weapons();
            Enable_Available_Super_Weapons_004f4db0();
        }
    }
    else if (unknown_144 != false) {
        unknown_144 = false;
        goto update_super_weapons;
    }
    if (unknown_18c && unknown_1c8.Value()==0) unknown_18c=false;
    if (this==unknown_player_a35db4 && unknown_192) {
        unknown_192=false;
        Map.Unknown_0049DE00(false,false);
    }
}


// void HouseClass::unknown_func_4e4ef0() (0x004E4EF0)
// Updates the super weapons; for the player's house, refreshes the sidebar
// entry of each one that changed (hypothesis).

void HouseClass::unknown_func_4e4ef0()
{
    for (int index = 0; index < Supers.Count(); index++) {
        UnknownSuperView *super = (UnknownSuperView *)Supers[index];
        if (super->unknown_55 && super->unknown_func_69d390(this == unknown_player_a35db4) &&
            this == unknown_player_a35db4) {
            int id = super->Type->Fetch_Heap_ID();
            Map.Unknown_0067C2D0(Unknown_006818D0(0x39, id, 0));
        }
    }
}


// void HouseClass::Attacked(TechnoClass *source) (0x004E4F60)
// An object of the house was attacked (TS's HouseClass::Attacked; name
// hypothesis). Unless the source declines (slot 31), the player's own house
// gets the base radar event and EVA warning, a house allied to the player the
// ally ones (except, outside the campaign, against a passive source house);
// every attached tag is then offered event 6.

void HouseClass::Attacked(TechnoClass *source)
{
    if (source != 0 && source->unknown_slot31()) {
        return;
    }
    bool is_player = Is_Player_Control();
    bool is_ally = unknown_player_a35db4 != 0 && unknown_player_a35db4 != this && Is_Ally(unknown_player_a35db4)
        && (unknown_game_mode_a3d298 == 0 || !source->House->Type->MultiplayPassive);
    if (is_player) {
        if (Unknown_0063B5B0(3, Coord_Cell(source->Center_Coord()))) {
            Speech_Play_Name("EVA_OurBaseIsUnderAttack", -1, -1);
            Voc_Play(Rules->BaseUnderAttackSound, 1.0f, 0x2000, 0);
        }
    } else if (is_ally) {
        if (Unknown_0063B5B0(16, Coord_Cell(source->Center_Coord()))) {
            Speech_Play_Name("EVA_OurAllyIsUnderAttack", -1, -1);
            Voc_Play(Rules->BaseUnderAttackSound, 1.0f, 0x2000, 0);
        }
    }
    for (int index = 0; index < unknown_38.Count(); index++) {
        unknown_38[index]->Spring(6, 0, unknown_cell_a40f10);
    }
}


// float HouseClass::Harvested(float amount, int type) (0x004E5100)

float HouseClass::Harvested(float amount, int type)
{
    unknown_5418 = (int)(amount * 5.0f + unknown_5418);
    Balance = (int)(((UnknownTiberiumView *)unknown_TiberiumClass_list_00ac0b30[type])->Value * Type->unknown_0x148 * amount + Balance);
    return amount;
}


// BuildingClass *HouseClass::unknown_func_4e5160() (0x004E5160)
// The building with tiberium storage that has the most free room.

BuildingClass *HouseClass::unknown_func_4e5160()
{
    UnknownBuildingView *best = 0;
    float best_room = 0;
    int count = Buildings.Count();
    for (int index = 0; index < count; index++) {
        UnknownBuildingView *building = (UnknownBuildingView *)Buildings[index];
        if (building != 0 && building->unknown_74) {
            int storage = ((UnknownBuildingTypeView *)building->Type)->Storage;
            if (storage > 0) {
                float capacity = (float)storage;
                float room = capacity - building->unknown_294.Get_Total_Amount();
                if (best == 0 || room > best_room) {
                    best_room = room;
                    best = building;
                }
            }
        }
    }
    return (BuildingClass *)best;
}


// void HouseClass::Place_Tiberium(int count, int type) (0x004E51F0)

void HouseClass::Place_Tiberium(int count, int type)
{
    while (count > 0) {
        float limit = (float)Rules->unknown_1410;
        if (!(unknown_254.Get_Total_Amount() < limit)) {
            break;
        }
        unknown_254.Add_Amount(1.0f, type);
        count--;
    }
}


// double HouseClass::unknown_func_4e5240() const (0x004E5240)

double HouseClass::unknown_func_4e5240() const
{
    if (unknown_254.Get_Total_Amount() == 0.0f) {
        return 0.0;
    }
    return unknown_254.Get_Total_Amount() / Rules->unknown_1410;
}


// House money spending, 0x004E5280; 436-byte body, 12-byte tail.

void HouseClass::Spend_Money_004e5280(int money)
{
    float old_amount = Tiberium.Get_Total_Amount();
    int old_capacity = Capacity;
    int spent;
    int remaining = money;
    if (money > Balance) {
        remaining -= Balance;
        spent = Balance;
        Balance = 0;
    } else {
        Balance -= money;
        spent = money;
        remaining = 0;
    }
    if (remaining > 0 && Tiberium.Get_Total_Amount() > 0.0f) {
        int count = Buildings.Count();
        for (int index = 0; index < count; ++index) {
            TechnoClass *building = (TechnoClass *)Buildings[index];
            if (building) {
                StorageClass &storage = building->unknown_294;
                while (storage.Get_Total_Amount() > 0.0f && remaining > 0) {
                    int type = storage.First_Used_Slot();
                    while (remaining > 0 && storage.Get_Amount(type) > 0.0f) {
                        float removed = storage.Remove_Amount(1.0f, type);
                        Tiberium.Remove_Amount(removed, type);
                        int value = (int)(unknown_TiberiumClass_list_00ac0b30[type]->Value *
                            Type->unknown_0x148 * removed);
                        remaining -= value;
                        spent += value;
                        if (remaining < 0) {
                            Balance -= remaining;
                            spent += remaining;
                            remaining = 0;
                            break;
                        }
                        if (remaining <= 0) break;
                    }
                }
            }
            if (!remaining) break;
        }
    }
    Silo_Redraw_Check((int)old_amount, old_capacity);
    unknown_21c += spent;
}


// void HouseClass::Refund_Money(int amount) (0x004E5440)

void HouseClass::Refund_Money(int amount)
{
    Balance += amount;
}


// void HouseClass::Silo_Redraw_Check(int tiberium, int capacity) (0x004E5460)
// Redraws the storage buildings when the fill level (in steps) changed.

void HouseClass::Silo_Redraw_Check(int tiberium, int capacity)
{
    int old_level = 0;
    if (capacity != 0) {
        old_level = (int)((double)tiberium / capacity * 4.0 + 0.5);
    }
    int level = 0;
    if (Capacity != 0) {
        level = (int)(Tiberium.Get_Total_Amount() / Capacity * 4.0 + 0.5);
    }
    if (old_level != level) {
        int count = Buildings.Count();
        for (int index = 0; index < count; index++) {
            UnknownBuildingView *building = (UnknownBuildingView *)Buildings[index];
            if (building != 0 && !building->unknown_76 &&
                ((UnknownBuildingTypeView *)building->Type)->unknown_1348) {
                building->Mark(2);
            }
        }
    }
}


// bool HouseClass::Is_Ally(int house) const (0x004E5500)

bool HouseClass::Is_Ally(int house) const
{
    if (house == ArrayIndex) {
        return true;
    }
    if (house != -1) {
        return (Allies & (1 << house)) != 0;
    }
    return false;
}


// bool HouseClass::Is_Ally(HouseClass const *house) const (0x004E5540)

bool HouseClass::Is_Ally(HouseClass const *house) const
{
    if (house != 0) {
        if (house == this) {
            return true;
        }
        return Is_Ally(house->ArrayIndex);
    }
    return false;
}


// bool HouseClass::Is_Ally(TechnoClass const *techno) const (0x004E5580)

bool HouseClass::Is_Ally(TechnoClass const *techno) const
{
    if (techno != 0) {
        return Is_Ally(techno->Owning_House());
    }
    return false;
}


// bool HouseClass::Is_Ally(ObjectClass const *object) const (0x004E55E0)

bool HouseClass::Is_Ally(ObjectClass const *object) const
{
    if (object != 0 && object->AbstractFlags.IsObject) {
        HouseClass *house = object->Owning_House();
        if (house != 0) {
            if (house == this) {
                return true;
            }
            return Is_Ally(house->ArrayIndex);
        }
    }
    return false;
}


// void HouseClass::Make_Ally(int house, bool announce) (0x004E5640)

void HouseClass::Make_Ally(int house, bool announce)
{
    Make_Ally(Houses[house], announce);
}


// void HouseClass::Make_Ally(HouseClass *house, bool announce) (0x004E5660)
// RA's Make_Ally: when the alliance is allowed the house's bit is set, the
// threat map rebuilt and the anger at the house forgotten (0x004F33E0); it
// stops being the enemy. Outside a network replay (or in game mode 4) a
// paranoid human house makes the computers paranoid, this house's objects drop
// targets that are now allies, and a human house outside the campaign shows
// "TXT_HAS_ALLIED" and, with announce, plays the EVA alliance speech.

void HouseClass::Make_Ally(HouseClass *house, bool announce)
{
    if (Is_Allowed_To_Ally(house)) {
        Map.Unknown_0049E000(false, false);
        Allies |= 1 << house->ArrayIndex;
        unknown_func_4f3420();
        unknown_func_4f33e0(house);
        if (unknown_54f0 == house->ArrayIndex) {
            unknown_54f0 = -1;
        }
        if (unknown_a40774) {
            unknown_120 |= 1 << house->ArrayIndex;
        }
        if (unknown_game_mode_a3d298 == 4 || !unknown_a40774) {
            if (Is_Human_Control() && Rules->Paranoid && !house->Type->MultiplayPassive && unknown_game_mode_a3d298 != 0) {
                unknown_func_4ec5e0();
            }
            for (int index = 0; index < unknown_list_832470.Count(); index++) {
                ObjectClass *object = unknown_list_832470[index];
                if (object != 0 && object->As_Techno() != 0 && !object->unknown_76 && object->Owner() == ArrayIndex) {
                    TargetClass target(((TechnoClass *)object)->unknown_228);
                    if (target.Is_Valid() && target.As_Techno() != 0) {
                        if (Is_Ally(target.As_Techno())) {
                            ((TechnoClass *)object)->unknown_slot220(0);
                        }
                    }
                }
            }
            if (Is_Human_Control() && unknown_game_mode_a3d298 != 0 && !house->Type->MultiplayPassive) {
                wchar_t buffer[80];
                swprintf(buffer, StringTable::Load_String("TXT_HAS_ALLIED", 0, "D:\\Ra2\\House.CPP", 3149), UIName, house->UIName);
                unknown_messages_a3dc70.Add_Message(0, 0, buffer, Type->Color, 0x4046, (int)(Rules->MessageDelay * 900.0), 0);
                if (announce) {
                    if (house->Is_Ally(this)) {
                        if (!unknown_int_a3d548) {
                            if (unknown_player_a35db4->Is_Ally(house) && unknown_player_a35db4->Is_Ally(this)) {
                                Speech_Play_Name("EVA_AllianceFormed", -1, -1);
                            } else {
                                Speech_Play_Name("EVA_EnemyAllianceFormed", -1, -1);
                            }
                        }
                    } else if (Is_Player_Control()) {
                        Speech_Play_Name("EVA_RequestingAlliance", -1, -1);
                    } else if (house->Is_Player_Control()) {
                        Speech_Play_Name("EVA_AllianceRequested", -1, -1);
                    }
                }
            }
            Map.Flag_To_Redraw(false);
        }
        Map.Unknown_0049DE00(false, false);
    }
}


// void HouseClass::Make_Enemy(int house, bool announce) (0x004E5A50)

void HouseClass::Make_Enemy(int house, bool announce)
{
    Make_Enemy(Houses[house], announce);
}


// void HouseClass::Make_Enemy(HouseClass *house, bool announce) (0x004E5A70)
// Ends an alliance (TS's Make_Enemy). Outside the campaign passive houses
// never change. The anger at the house rises; when it was an ally the
// alliance is dropped on both sides (each side's bits and +0x4F3420), the
// player's own house in a multiplayer game shows "TXT_AT_WAR", and with
// announce EVA reports the broken alliance when the player controls a side.
// The allied view is refreshed around it (0x0049E000 / 0x0049DE00).

void HouseClass::Make_Enemy(HouseClass *house, bool announce)
{
    bool broken = false;
    if (unknown_game_mode_a3d298 != 0 && (house->Type->MultiplayPassive || Type->MultiplayPassive)) {
        return;
    }
    Map.Unknown_0049E000(false, false);
    unknown_func_4eeba0(1, house);
    if (Is_Ally(house)) {
        Allies &= ~(1 << house->ArrayIndex);
        if (unknown_a40774) {
            unknown_120 &= !(1 << house->ArrayIndex);
        }
        unknown_func_4f3420();
        if (house->Is_Ally(this)) {
            house->Allies &= ~(1 << ArrayIndex);
            broken = true;
            if (unknown_a40774) {
                unknown_120 &= ~(1 << ArrayIndex);
            }
            house->unknown_func_4f3420();
            house->unknown_func_4eeba0(1, house);
        }
        if (unknown_game_mode_a3d298 != 0 && !unknown_a40774 && unknown_134) {
            wchar_t buffer[80];
            swprintf(buffer, StringTable::Load_String("TXT_AT_WAR", 0, "D:\\Ra2\\House.CPP", 3272), UIName, house->UIName);
            unknown_messages_a3dc70.Add_Message(0, 0, buffer, Type->Color, 0x4046, (int)(Rules->MessageDelay * 900.0), 0);
            Map.Flag_To_Redraw(false);
            if (announce && broken && (Is_Player_Control() || house->Is_Player_Control())) {
                Speech_Play_Name("EVA_AllianceBroken", -1, -1);
            }
        }
    }
    Map.Unknown_0049DE00(false, false);
}


// TypeList<TeamTypeClass const *> HouseClass::Suggested_New_Team(bool alerted) (0x004E5CF0)
// The team types this house may make now (RA's HouseClass::Suggested_New_Team,
// which returned one type; the list comes from TeamTypeClass 0x006BED50; AI
// expands it).

TypeList<TeamTypeClass const *> HouseClass::Suggested_New_Team(bool alerted)
{
    TypeList<TeamTypeClass const *> list = TeamTypeClass::Suggested_New_Teams(this, alerted);
    return list;
}


// void HouseClass::unknown_func_4e5dc0(int cell, int amount) (0x004E5DC0)
// Adds an amount (or removes a negative one, clamped at zero) to the
// per-cell values around a cell, halved or quartered by distance.

extern int const unknown_adjacent_offset_7d9fa0[9];   // 0x007D9FA0 cell offsets of the 3x3 block (map width 130? hypothesis)
extern int const unknown_adjacent_shift_7d9fc4[9];    // 0x007D9FC4 shifts per block position

void HouseClass::unknown_func_4e5dc0(int cell, int amount)
{
    int negative;
    if (amount < 0) {
        amount = -amount;
        negative = 1;
    } else {
        negative = 0;
    }
    int index = 0;
    for (int count = 9; count != 0; count--, index++) {
        int value = amount >> unknown_adjacent_shift_7d9fc4[index];
        int *entry = &unknown_56d0[unknown_adjacent_offset_7d9fa0[index] + cell].Threat;
        if (negative) {
            *entry -= value;
        } else {
            *entry += value;
        }
        if (*entry < 0) {
            *entry = 0;
        }
    }
}


// int HouseClass::unknown_func_4e63f0(int kind, int index, bool naval) (0x004E63F0)
// Suspends the house's factory for a type (3 when it has none); the
// player's house also refreshes the type's sidebar entry (hypothesis).

int HouseClass::unknown_func_4e63f0(int kind, int index, bool naval)
{
    UnknownFactoryView *factory = (UnknownFactoryView *)unknown_func_4eb580(kind, naval, unknown_func_4eb550(kind, index));
    if (factory == 0) {
        return 3;
    }
    factory->unknown_func_4b9630(true);
    if (unknown_player_a35db4 == this) {
        Map.Unknown_0067C2D0(Unknown_006818D0(kind, index, 0));
    }
    return 0;
}


// int HouseClass::Abandon_Production(int kind, int index, bool naval) (0x004E64F0)
// RA's Abandon_Production: a type still queued is only removed from the
// queue; otherwise the factory abandons its object (the player's house
// updates the sidebar and drops a pending building) and, with nothing
// queued, is cleared from the house and deleted. 3 without a factory.

int HouseClass::Abandon_Production(int kind, int index, bool naval)
{
    int buildcat = unknown_func_4eb550(kind, index);
    FactoryClass *factory = unknown_func_4eb580(kind, naval, buildcat);
    if (factory == 0) {
        return 3;
    }
    if (factory->Queue.Count() > 0 && index >= 0 && buildcat == 0 && factory->Delete_Queued_004B9DF0(Unknown_00482C00(kind, index))) {
        Map.Unknown_0067C2D0(Unknown_006818D0(kind, index, 0));
        return 0;
    }
    if (buildcat == 0 && index != -1) {
        if (factory->Object == 0 || index != factory->Object->Class_Of()->Fetch_Heap_ID()) {
            return 0;
        }
    }
    if (unknown_player_a35db4 == this) {
        Map.Unknown_00681740(kind, index, factory);
        if (kind == 7 || kind == 6) {
            Map.unknown_11a0 = 0;
            Map.unknown_11a4 = 0;
            Map.unknown_11a8 = -1;
            Map.Unknown_00498F10(0);
        }
    }
    factory->Abandon_Production_004B97C0();
    if (factory->Queue.Count() == 0) {
        Set_Factory(0, kind, naval, buildcat);
        delete factory;
    } else {
        factory->Start_Next_Queued_004B9D70();
    }
    return 0;
}


// void HouseClass::unknown_func_4e67a0(int index) (0x004E67A0)
// Picks the building of another, non-allied house with the highest slot 90
// value (each candidate passes a 90% roll; buildings with no strength or the
// +0x76 flag are skipped) and hands its cell to 0x004E68D0.

void HouseClass::unknown_func_4e67a0(int index)
{
    BuildingClass *best = 0;
    int value = -1;
    for (int i = 0; i < unknown_BuildingClass_list_00a40af8.Count(); i++) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[i];
        if (building != 0 && !building->unknown_76 && building->Strength != 0 && !Is_Ally(building)
                && Random_Pick(0, 99) < 90) {
            if (building->Value() > value || value == -1) {
                value = building->Value();
                best = building;
            }
        }
    }
    if (best != 0) {
        Cell cell = Coord_Cell(best->Center_Coord());
        unknown_func_4e68d0(index, cell);
    }
}


// bool HouseClass::unknown_func_4e68d0(int index, Cell const &cell) (0x004E68D0)
// Acts on a super weapon (0x0069D090); a linked super weapon takes over its
// +0x4A cell first and is updated after it.

// Original House retains all nine calls out of line. The sole direct
// launch69D090 call is within4E68D0; no inline copy occurs in this unit.
// Original-never-inlined pragma is permitted by WORKER source rules.
#pragma auto_inline(off)
bool HouseClass::unknown_func_4e68d0(int index, Cell const &cell)
{
    UnknownSuperView *super = (UnknownSuperView *)Supers[index];
    UnknownSuperView *linked = 0;
    if (super->Type->unknown_e1) {
        linked = (UnknownSuperView *)Supers[super->Type->unknown_e4];
        if (linked != 0) {
            super->unknown_4a = linked->unknown_4a;
        }
    }
    super->unknown_func_69d090(this == unknown_player_a35db4, &cell);
    if (super->Type->unknown_e1 && linked != 0) {
        linked->unknown_func_69cfb0(0);
        linked->unknown_func_69cfc0(this == unknown_player_a35db4);
    }
    return true;
}
#pragma auto_inline(on)


// House004E6950: completed production placement, 1460 body bytes and 12 frozen NOPs.
// RA2 additions and call/storage order checked against the exact target; TS is shape evidence only.

extern Rect unknown_rect_839630; // Existing shared/display.hpp view rectangle, 00839630.

bool HouseClass::Unknown_004E6950(int kind, int index, bool naval, Cell const &cell)
{
    bool placed = false;
    TechnoClass *tech = 0;
    int category = unknown_func_4eb550(kind, index);
    FactoryClass *factory = unknown_func_4eb580(kind, naval, category);
    if (factory && factory->Is_Complete_004B9900()) {
        tech = factory->Get_Object();
        if (cell == unknown_cell_a40f10 || cell == Cell(-1, -1)) {
            if (tech) {
                BuildingClass *builder = tech->Who_Can_Build_Me(false, true);
                if (!builder && tech->Kind_Of() == 2) {
                    builder = tech->Who_Can_Build_Me(true, true);
                }
                if (builder) {
                    int exit = builder->Exit_Object(tech);
                    if (exit == 2 || (exit == 1 && builder->Kind_Of() == 6 && builder->unknown_41c != 0)) {
                        if (Is_Player_Control()) {
                            if (Unknown_0063B5B0(6, Coord_Cell(builder->Center_Coord()))) {
                                Speech_Play_Name("EVA_UnitReady", -1, -1);
                            }
                        }
                        factory->Release_Completed_004B9970();
                        Abandon_Production(kind, index, naval);
                        placed = true;
                    } else {
                        if (tech->Kind_Of() != 6) {
                            Debug_Printf("Failed to exit object from factory - refunding money\n");
                            Abandon_Production(kind, index, naval);
                        }
                        return placed;
                    }
                }
            }
        } else {
            Cell location = cell;
            if (tech) {
                BuildingClass *builder = tech->Who_Can_Build_Me(false, false);
                if (builder) {
                    builder->unknown_slot150(2, tech);
                    if (tech->Unlimbo(&UnknownCoord434820(location.X * 256 + 128, location.Y * 256 + 128, 0), 0)) {
                        if (tech->Kind_Of() == 6) {
                            BuildingTypeClass *type = ((BuildingClass *)tech)->Type;
                            if (type->FirestormWall) {
                                Map.Unknown_0056A480(location, this, type);
                            } else if (type->ToOverlay && type->ToOverlay->Wall) {
                                Map.Unknown_0056A660(location, this, type);
                            }
                        }
                        factory->Release_Completed_004B9970();
                        tech->unknown_slot150(12, builder);
                        Abandon_Production(kind, index, naval);
                        placed = true;
                        if (unknown_player_a35db4 == this) {
                            if (tech->unknown_7d && !tech->unknown_373) tech->Revealed(this);
                            Voc_Play(Rules->BuildingSlam, 1.0f, 0x2000, 0);
                            if (!Debug_Map) {
                                Map.Set_Cursor_Shape(0);
                                Map.unknown_118c = 0;
                                Map.unknown_1190 = 0;
                            }
                            Map.Unknown_00498F10(0);
                            Map.unknown_11a0 = 0;
                            Map.unknown_11a4 = 0;
                            Map.unknown_11a8 = -1;
                        }
                    } else {
                        placed = false;
                        if (this == unknown_player_a35db4) Speech_Play_Name("EVA_CannotDeployHere", -1, -1);
                        if (!Debug_Map && this == unknown_player_a35db4) {
                            Map.unknown_11a0 = Map.unknown_118c;
                            Map.unknown_11a4 = Map.unknown_1190;
                            Map.unknown_11a8 = Map.unknown_1194;
                            Map.unknown_1170 = Map.unknown_117e;
                            Map.unknown_1174 = Map.unknown_1182;
                            Map.unknown_1178 = Map.unknown_1188;
                            if (Map.unknown_1190) {
                                Map.Unknown_00498F10(((ObjectTypeClass *)Map.unknown_1190)->Occupy_List(1));
                                Point2D xy = WWMouse->Get_Mouse_XY();
                                unknown_tactical_83996c->Pixel_To_Coord(xy);
                                xy -= Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);
                                Cell mouse_cell;
                                CoordStruct mouse_coord;
                                ObjectClass *object = 0;
                                bool shadow, small;
                                Map.Resolve_Point(xy, mouse_cell, mouse_coord, object, shadow, small);
                                Map.Unknown_004994D0(mouse_cell);
                            }
                            Map.Set_Cursor_Shape(0);
                            Map.unknown_118c = 0;
                            Map.unknown_1190 = 0;
                        }
                    }
                    builder->unknown_slot149(3);
                }
            }
        }
        if (placed) {
            Just_Built(tech);
            BuildingClass *builder = tech->Who_Can_Build_Me(false, true);
            if (builder && builder->Kind_Of() == 6) {
                int sound = builder->Type->CreateUnitSound;
                if (sound != -1) Voc_Static_Add(sound, tech->Position_Coord_004f3f40(), 0);
            }
        }
    }
    return placed;
}


// void HouseClass::Just_Built(TechnoClass *techno) (0x004E6F10)
// Records the type an object just built (+0x1B4..+0x1C0 by kind), counts it
// (+0x54A0..+0x54D0) and plays the Rules creation sound of its kind (the
// type's CreateSound overrides it) at the object (TS name).

void HouseClass::Just_Built(TechnoClass *techno)
{
    int sound = -1;
    unknown_18d = true;
    TechnoTypeClass *type = techno->Techno_Type_Class();
    switch (techno->Kind_Of()) {
    case 1:
        unknown_1bc = type->Fetch_Heap_ID();
        unknown_54b0.Increment(type->Fetch_Heap_ID());
        sound = Rules->CreateUnitSound;
        break;
    case 15:
        unknown_1b8 = type->Fetch_Heap_ID();
        unknown_54c0.Increment(type->Fetch_Heap_ID());
        sound = Rules->CreateInfantrySound;
        break;
    case 6:
        unknown_1b4 = type->Fetch_Heap_ID();
        unknown_54a0.Increment(type->Fetch_Heap_ID());
        break;
    case 2:
        unknown_1c0 = type->Fetch_Heap_ID();
        unknown_54d0.Increment(type->Fetch_Heap_ID());
        sound = Rules->CreateAircraftSound;
        break;
    }
    if (type->CreateSound != -1) {
        sound = type->CreateSound;
    }
    if (sound != -1) {
        CoordStruct coord = techno->Coord;
        Voc_Play_At(sound, coord, 0);
    }
    unknown_144 = true;
}


// bool HouseClass::unknown_func_4e7070(TechnoClass *builder, BuildingClass *building) (0x004E7070)
// RA's Manual_Place: the player's house, with nothing pending, starts placing
// a finished building: the selection is cleared, the repair and sell modes
// end, the building, its type and the house become the map's pending
// placement, the cursor takes the building's foundation list and moves to
// the builder's cell, and the builder is marked (slot 71).
void __cdecl Clear_Selected_Objects_00482bc0();

bool HouseClass::unknown_func_4e7070(TechnoClass *builder, BuildingClass *building)
{
    if (this == unknown_player_a35db4 && Map.unknown_11a4 == 0 && builder != 0 && building != 0) {
        Clear_Selected_Objects_00482bc0();
        Map.Unknown_0049CA20(0);
        Map.Unknown_0049C7C0(0);
        Map.unknown_11ae = false;
        Map.unknown_11af = false;
        Map.unknown_11b0 = false;
        Map.unknown_11b4 = -1;
        Map.unknown_11a4 = building->Type;
        Map.unknown_11a0 = building;
        Map.unknown_11a8 = ArrayIndex;
        Map.Unknown_00498F10(building->Occupy_List(1));
        Map.Unknown_004994D0(builder->unknown_slot104());
        builder->Mark(2);
        return true;
    }
    return false;
}


// void HouseClass::unknown_func_4e7150() (0x004E7150)
// Deletes the house's technos, the triggers of its house type and the
// house itself.

void HouseClass::unknown_func_4e7150()
{
    for (int index = 0; index < Technos.Count(); index++) {
        if (Technos[index]->House == this) {
            delete Technos[index];
            index--;
        }
    }
    for (int i = 0; i < Triggers.Count(); i++) {
        if ((AbstractTypeClass *)Triggers[i]->Class->House == Type) {
            delete Triggers[i];
            i--;
        }
    }
    delete this;
}


// void HouseClass::Detach(AbstractClass *target, bool all) (0x004E71E0, slot 10)
// Forgets a destroyed object: the tag and building lists, the waypoint paths, the factories and the +0x5410 slot.

void HouseClass::Detach(AbstractClass *target, bool all)
{
    if (unknown_5410 == target) {
        unknown_5410 = 0;
    }
    TagClass *tag = dynamic_cast<TagClass *>(target);
    if (tag != 0) {
        unknown_38.Delete(tag);
    }
    if (target->Kind_Of() == 6 && all) {
        unknown_50.Delete((BuildingClass *)target);
        unknown_80.Delete((BuildingClass *)target);
        unknown_98.Delete((BuildingClass *)target);
        unknown_b0.Delete((BuildingClass *)target);
        Buildings.Delete((BuildingClass *)target);
    }
    for (int index = 0; index < 12; index++) {
        if (WaypointPaths[index] == target) {
            WaypointPaths[index] = 0;
        }
    }
    if (unknown_52d8 == target) {
        unknown_52d8 = 0;
    }
    if (unknown_52dc == target) {
        unknown_52dc = 0;
    }
    if (unknown_52e0 == target) {
        unknown_52e0 = 0;
    }
    if (unknown_52e4 == target) {
        unknown_52e4 = 0;
    }
    if (unknown_52e8 == target) {
        unknown_52e8 = 0;
    }
    if (unknown_52ec == target) {
        unknown_52ec = 0;
    }
    if (unknown_52f0 == target) {
        unknown_52f0 = 0;
    }
    if (unknown_52f4 == target) {
        unknown_52f4 = 0;
    }
    if (unknown_52f8 == target) {
        unknown_52f8 = 0;
    }
}


// bool HouseClass::unknown_func_4e7440(int type) (0x004E7440)
// Whether any house that is not an ally counts one or more of a type
// (its +0x5460 counter).

bool HouseClass::unknown_func_4e7440(int type)
{
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *house = Houses[index];
        if (house != 0 && !Is_Ally(house) && house->unknown_5460.unknown_func_491d10(type) > 0) {
            return true;
        }
    }
    return false;
}


// TechnoTypeClass *HouseClass::unknown_func_4e74c0(int kind, bool naval) (0x004E74C0)
// The type kept per object kind at +0x553C..+0x5548 (an index into the
// type list of that kind; -1 for none).

// The type lists (declared by shared/aircraft_type.hpp, unit_type.hpp,
// infantry_type.hpp and building_type.hpp, which this unit's views of the
// object classes keep it from including).
class AircraftTypeClass;
class UnitTypeClass;
class InfantryTypeClass;
class BuildingTypeClass;
extern DynamicVectorClass<AircraftTypeClass *> AircraftTypes;       // 0x00A3D278
extern DynamicVectorClass<UnitTypeClass *> UnitTypes;               // 0x00A35D48
extern DynamicVectorClass<InfantryTypeClass *> InfantryTypes;       // 0x00A40350
extern DynamicVectorClass<BuildingTypeClass *> BuildingTypes;       // 0x00A35CD8

TechnoTypeClass *HouseClass::unknown_func_4e74c0(int kind, bool naval)
{
    switch (kind) {
    case 2:
    case 3:
        if (unknown_5548 != -1) {
            return (TechnoTypeClass *)AircraftTypes[unknown_5548];
        }
        break;
    case 1:
    case 40:
        if (unknown_5540 != -1) {
            return (TechnoTypeClass *)UnitTypes[unknown_5540];
        }
        break;
    case 15:
    case 16:
        if (unknown_5544 != -1) {
            return (TechnoTypeClass *)InfantryTypes[unknown_5544];
        }
        break;
    case 6:
    case 7:
        if (unknown_553c != -1) {
            return (TechnoTypeClass *)BuildingTypes[unknown_553c];
        }
        break;
    }
    return 0;
}


// bool HouseClass::unknown_func_4e7580(AbstractClass *object, bool arg2) (0x004E7580)
// Releases an object (a unit through 0x007049F0, anything else through its
// cell); forgets it at +0x5308 when it was kept there. With arg2 the marked
// cell at +0x530C is cleared too.

// UnitClass and the cell object as far as this unit calls them (views).
struct UnknownUnitReleaseView {
    bool unknown_func_7049f0();             // 0x007049F0
};
struct UnknownCellView {
    char unknown_00[0x44];
    int unknown_44;                         // +0x44

    bool unknown_func_478720();             // 0x00478720
};


bool HouseClass::unknown_func_4e7580(AbstractClass *object, bool arg2)
{
    bool result = false;
    if (object != 0) {
        if (object->Kind_Of() == 1) {
            result = ((UnknownUnitReleaseView *)object)->unknown_func_7049f0();
            if (result && unknown_5308 == object) {
                unknown_5308 = 0;
            }
        } else {
            Cell cell = Coord_Cell(object->Center_Coord());
            if (Map.In_Radar(cell)) {
                result = ((UnknownCellView *)&Map[cell])->unknown_func_478720();
                if (result && unknown_5308 == object) {
                    unknown_5308 = 0;
                }
            }
        }
        if (arg2 && unknown_530c != unknown_geometry_k) {
            ((UnknownCellView *)&Map[unknown_530c])->unknown_44 = -1;
            unknown_530c = unknown_geometry_k;
        }
        return result;
    }
    return false;
}


// bool HouseClass::unknown_4e76a0(Cell const &cell, bool arg2) (0x004E76A0)
// Keeps the cell as the house's held object (+0x5308): releases the old one
// (0x004E7580), then marks the cell, or the nearest cell that takes the
// house, and stores that cell; false when none does.

bool HouseClass::unknown_4e76a0(Cell const &cell, bool arg2)
{
    if (Map.In_Radar(cell)) {
        unknown_func_4e7580(unknown_5308, arg2);
        Cell target = cell;
        bool ok = Map[target].unknown_4786d0(Type->House);
        if (!ok) {
            target = Map.Nearby_Location(cell, 1, -1, 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
            if (target != unknown_cell_a40f10) {
                ok = Map[target].unknown_4786d0(Type->House);
            }
        }
        if (ok) {
            unknown_5308 = &Map[target];
        }
        return ok;
    }
    return false;
}


// bool HouseClass::unknown_func_4e77a0(ObjectClass *object, bool arg2) (0x004E77A0)
// Replaces the object kept at +0x5308 (releasing the old one through
// 0x004E7580) and marks the new one with the house type.

// UnitClass as far as this unit calls it (view).
struct UnknownUnitView {
    bool unknown_func_7049c0(int house_type);   // 0x007049C0
};

bool HouseClass::unknown_func_4e77a0(ObjectClass *object, bool arg2)
{
    if (object != 0 && !object->unknown_76) {
        unknown_func_4e7580(unknown_5308, arg2);
        ((UnknownUnitView *)object)->unknown_func_7049c0(Type->House);
        unknown_5308 = object;
        return true;
    }
    return false;
}


// House defeat handling, original004E77F0; complete1535body+1NOP.
extern bool unknown_flag_8373f4;
extern bool unknown_flag_7d9fe8;
extern bool unknown_flag_a3d8d1;
void HouseClass::MPlayer_Defeated_004e77f0()
{
    wchar_t text[80];
    int i,j;
    HouseClass *house,*other;
    int alive,humans;
    bool all_allies;
    unknown_13d=true;
    if (unknown_194==Rules->MaxIQ && !Is_Human_Control() && Rules->Paranoid && unknown_game_mode_a3d298!=0) unknown_func_4ec5e0();
    if (unknown_id_source_a3d290->unknown_000 & 0x10) {
        if (unknown_5308) unknown_func_4e7580(unknown_5308,true);
        else if (unknown_530c!=unknown_cell_a40f10) unknown_func_4e7580((AbstractClass *)&Map[unknown_530c],true);
    }
    if (unknown_game_mode_a3d298!=0 && (unknown_id_source_a3d290->unknown_000 & 0x800)) {
        for (i=0;i<Units.Count();++i) {
            UnitClass *unit=Units[i];
            if (unit->House==this && unit->unknown_7d) unit->Delete_Me();
        }
    }
    if (unknown_player_a35db4==this) {
        unknown_object_84e8d0.Unknown_0042EBE0(unknown_player_a35db4->ArrayIndex);
        if (unknown_flag_8373f4) {
            Map.Unknown_006A1F10();
            unknown_flag_7d9fe8=true;
        } else unknown_flag_7d9fe8=false;
        Planning_End();
        Map.Unknown_0049CAC0(0);
        unknown_int_a3d548=1;
        Map.Unknown_0055A1F0();
        Map.Toggle_Radar(true);
        unknown_surface_839954->Fill(0);
        Map.Flag_To_Redraw(2);
        memset(unknown_flags_a3f118,1,sizeof(unknown_flags_a3f118));
        if (Current_Dialog_Id_00735b20()==0x73) Map.Unknown_00634970((int)Unknown_00735B10());
        if (unknown_house_a72d28!=unknown_player_a35db4) {
            swprintf(text,StringTable::Load_String("TXT_PLAYER_DEFEATED",0,"D:\\Ra2\\House.CPP",0x125f),UIName);
            unknown_session_a3d298.Messages.Add_Message(0,0,text,unknown_session_a3d298.Unknown_006715D0(unknown_a3d3b4),0x4046,(int)(Rules->MessageDelay*900),false);
            Speech_Play_Name("EVA_YouHaveLost",-1,-1);
        }
        Map.Flag_To_Redraw(0);
        Debug_Printf("MPlayer_Defeated() - Player %s has been defeated (OBIWAN MODE)\n",(char const *)IniName);
    } else {
        if (!Type->MultiplayPassive) {
            if (this!=unknown_house_a72d28) {
                swprintf(text,StringTable::Load_String("TXT_PLAYER_DEFEATED",0,"D:\\Ra2\\House.CPP",0x126f),UIName);
                unknown_session_a3d298.Messages.Add_Message(0,0,text,ColorSchemeIndex,0x4046,(int)(Rules->MessageDelay*900),false);
                Speech_Play_Name("EVA_PlayerDefeated",-1,-1);
            }
            Map.Flag_To_Redraw(0);
        }
        Debug_Printf("MPlayer_Defeated() - Opponent %s has been defeated\n",(char const *)IniName);
    }
    alive=0;
    humans=0;
    for (i=0;i<Houses.Count();++i) {
        house=Houses[i];
        if (house && !house->unknown_13d && !house->Type->MultiplayPassive) {
            if (house->Is_Human_Control()) ++humans;
            ++alive;
        }
    }
    Debug_Printf("MPlayer_Defeated() - Alive = %d, Humans = %d\n",alive,humans);
    all_allies=true;
    for (i=0;i<Houses.Count();++i) {
        house=Houses[i];
        if (!house || house->unknown_13d || (unknown_game_mode_a3d298!=5 && house->Type->MultiplayPassive)) continue;
        for (j=0;j<Houses.Count();++j) {
            other=Houses[j];
            if (other && !other->unknown_13d && (unknown_game_mode_a3d298==5 || !other->Type->MultiplayPassive) && (!house->Is_Ally(other) || !other->Is_Ally(house))) {
                all_allies=false;
                break;
            }
        }
        if (!all_allies) break;
    }
    if (all_allies) {
        unknown_flag_a3d8d1=true;
        Debug_Printf("Saw game completion due to player defeat\n");
        alive=1;
        Debug_Printf("MPlayer_Defeated() - All remaining players are allied\n");
    }
    if (alive==1 || humans==0) {
        if (unknown_flag_7d9fe8) Map.Unknown_006A1EC0();
        if (unknown_player_a35db4->unknown_13d) {
            UnknownSessionA3D29C *mode=unknown_session_a3d29c;
            bool present=mode!=0;
            bool cooperative=(unknown_game_mode_a3d298==3 || unknown_game_mode_a3d298==4) && present && mode->unknown_slot1() && mode->unknown_slot2();
            if (alive==1 && cooperative) {
                bool human_alive=false;
                for (i=0;i<Houses.Count();++i) {
                    house=Houses[i];
                    if (house && !house->unknown_13d && house->Is_Human_Control()) human_alive=true;
                }
                if (human_alive) {
                    Debug_Printf("MPlayer_Defeated() - Allies win, Flag_To_Win\n");
                    unknown_player_a35db4->Flag_To_Win(false);
                } else {
                    Debug_Printf("MPlayer_Defeated() - Allies lost, Flag_To_Lose\n");
                    unknown_player_a35db4->Flag_To_Lose(false);
                }
            } else {
                Debug_Printf("MPlayer_Defeated() - Flag_To_Lose\n");
                unknown_player_a35db4->Flag_To_Lose(false);
            }
        } else {
            Debug_Printf("MPlayer_Defeated() - Flag_To_Win\n");
            unknown_player_a35db4->Flag_To_Win(false);
        }
    }
}


// void HouseClass::unknown_func_4e7df0() (0x004E7DF0)
// Destroys every techno the house owns (through the rules' warhead at
// +0xC74); a techno taken over by another owner counts for that owner.


// The object a techno keeps at +0x230, and the one at +0x20C (views).
struct UnknownTechno230View {
    char unknown_00[0x28];
    HouseClass *unknown_28;                 // +0x28
};
struct UnknownTechno20cView {
    void unknown_func_6e2680();             // 0x006E2680 (releases its techno)
};

void HouseClass::unknown_func_4e7df0()
{
    int index = 0;
    TechnoClass *last = 0;
    int damage;
    while (index < Technos.Count()) {
        TechnoClass *techno = Technos[index];
        HouseClass *owner = techno->unknown_234 != 0
            ? ((UnknownTechno230View *)((TechnoClass *)techno->unknown_234)->unknown_230)->unknown_28
            : techno->House;
        if (owner == this && techno != last) {
            damage = techno->Strength;
            if (techno->unknown_20c != 0) {
                ((UnknownTechno20cView *)techno->unknown_20c)->unknown_func_6e2680();
            }
            techno->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 1);
            last = techno;
        } else {
            index++;
        }
    }
}


// bool HouseClass::unknown_func_4e7e80() (0x004E7E80)
// Sets the +0x13E flag and restarts the +0x1E0 timer at zero unless one of
// the +0x13E..+0x140 flags is already set; returns the +0x13E flag.

bool HouseClass::unknown_func_4e7e80()
{
    if (!unknown_13f && !unknown_13e && !unknown_140) {
        unknown_13e = true;
        unknown_1e0 = CDTimerClass<FrameTimerClass>(0);
    }
    return unknown_13e;
}


// bool HouseClass::Flag_To_Win(bool quiet) (0x004E7EE0)
// RA's Flag_To_Win: unless the house is already winning, dying or losing
// (+0x13F, +0x13E, +0x140), marks it as winning and, unless quiet, starts the
// borrowed time (+0x1E0, Rules SavourDelay minutes; in a network game at
// least the frames ahead and ending on a multiple of ten frames). The
// player's house is told (campaign: the game pauses).
extern int unknown_max_ahead_a3d560;

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

bool HouseClass::Flag_To_Win(bool quiet)
{
    if (!unknown_13f && !unknown_13e && !unknown_140) {
        unknown_13f = true;
        if (!quiet) {
            unknown_1e0 = (int)(Rules->SavourDelay * 900.0);
            if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
                unknown_1e0 = ((Frame + (unknown_1e0.Value() > unknown_max_ahead_a3d560 ? unknown_1e0 : CDTimerClass<FrameTimerClass>(unknown_max_ahead_a3d560)).Value() + 9) / 10) * 10 - Frame;
            }
            Debug_Printf("Frame %d, BorrowedTime == %d\n", Frame, unknown_1e0.Value());
        }
        if (unknown_game_mode_a3d298 == 0) {
            if (Is_Player_Control()) {
                Pause_Game_0065c3d0();
                unknown_tactical_83996c->Set_Caption_Text_006a53f0(StringTable::Load_String("TXT_SCENARIO_WON", 0, "D:\\Ra2\\House.CPP", 5304), 0);
                Speech_Play_Name("EVA_MissionAccomplished", -1, -1);
                Map.Flag_To_Redraw(false);
            }
        } else if (Is_Player_Control()) {
            unknown_tactical_83996c->Set_Caption_Text_006a53f0(StringTable::Load_String("TXT_VICTORIOUS", 0, "D:\\Ra2\\House.CPP", 5307), 2);
            Speech_Play_Name("EVA_YouAreVictorious", -1, -1);
            Map.Flag_To_Redraw(false);
        }
    }
    return unknown_13f;
}


// bool HouseClass::Flag_To_Lose(bool quiet) (0x004E80D0)
// RA's Flag_To_Lose: clears winning (+0x13F); unless the house is already
// dying or losing (+0x13E, +0x140), marks it as losing and, unless quiet, starts the
// borrowed time (+0x1E0, Rules SavourDelay minutes; in a network game at
// least the frames ahead and ending on a multiple of ten frames). The
// player's house is told (campaign: the game pauses; the defeat speech is
// left out when the player is unknown_house_a72d28).
extern int unknown_max_ahead_a3d560;

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

bool HouseClass::Flag_To_Lose(bool quiet)
{
    unknown_13f = false;
    if (!unknown_13e && !unknown_140) {
        unknown_140 = true;
        if (!quiet) {
            unknown_1e0 = (int)(Rules->SavourDelay * 900.0);
            if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
                unknown_1e0 = ((Frame + (unknown_1e0.Value() > unknown_max_ahead_a3d560 ? unknown_1e0 : CDTimerClass<FrameTimerClass>(unknown_max_ahead_a3d560)).Value() + 9) / 10) * 10 - Frame;
            }
            Debug_Printf("Frame %d, BorrowedTime == %d\n", Frame, unknown_1e0.Value());
        }
        if (unknown_game_mode_a3d298 == 0) {
            if (Is_Player_Control()) {
                Pause_Game_0065c3d0();
                unknown_tactical_83996c->Set_Caption_Text_006a53f0(StringTable::Load_String("TXT_SCENARIO_LOST", 0, "D:\\Ra2\\House.CPP", 5359), 1);
                Speech_Play_Name("EVA_MissionFailed", -1, -1);
                Map.Flag_To_Redraw(false);
            }
        } else if (Is_Player_Control()) {
            unknown_tactical_83996c->Set_Caption_Text_006a53f0(StringTable::Load_String("TXT_LOST", 0, "D:\\Ra2\\House.CPP", 5362), 3);
            if (unknown_player_a35db4 != unknown_house_a72d28) {
                Speech_Play_Name("EVA_YouHaveLost", -1, -1);
            }
            Map.Flag_To_Redraw(false);
        }
    }
    return unknown_140;
}


// void HouseClass::unknown_func_4e82c0() (0x004E82C0)

void HouseClass::unknown_func_4e82c0()
{
    if (!unknown_140 && !unknown_13f) {
        Flag_To_Win(false);
        return;
    }
    unknown_1e0.Start();
}


// void HouseClass::unknown_func_4e8300(int color, int arg2, int balance) (0x004E8300)

void HouseClass::unknown_func_4e8300(int color, int arg2, int balance)
{
    unknown_124 = balance;
    Balance = balance;
    Type->Color = color;
    ColorSchemeIndex = color;
}


// double HouseClass::Power_Fraction() const (0x004E8330)

double HouseClass::Power_Fraction() const
{
    if (Power < Drain && Drain != 0) {
        if (Power != 0) {
            return (double)Power / Drain;
        }
        return 0.0;
    }
    return 1.0;
}


// void HouseClass::Sell_Wall(Cell const &cell, bool quiet) (0x004E8380)
// RA's Sell_Wall: a wall overlay whose owner (+0x50) a human controls is
// sold through the building type that makes it (ToOverlay, not Unsellable):
// the sell sound for the player unless quiet, the type's slot 33 for this
// house, then the overlay and its owner are cleared and the cell, radar and
// map are refreshed.

void HouseClass::Sell_Wall(Cell const &cell, bool quiet)
{
    if (cell != unknown_cell_a40f10) {
        CellClass &cellptr = Map[cell];
        int overlay = cellptr.OverlayIndex;
        if (overlay != -1) {
            int owner = cellptr.unknown_50;
            if (owner != -1 && Houses[owner]->Is_Human_Control()) {
                OverlayTypeClass *otype = OverlayTypes[overlay];
                if (otype->Wall) {
                    BuildingTypeClass *btype = 0;
                    for (int index = 0; index < BuildingTypes.Count(); index++) {
                        if (BuildingTypes[index]->ToOverlay == otype) {
                            btype = BuildingTypes[index];
                            break;
                        }
                    }
                    if (btype != 0 && !btype->Unsellable) {
                        if (unknown_player_a35db4 == this && !quiet) {
                            Voc_Play(Rules->unknown_5c4, 1.0f, 0x2000, 0);
                        }
                        btype->Cost_Of(this);
                        Map[cell].OverlayIndex = -1;
                        Map[cell].OverlayData = 0;
                        Map[cell].unknown_50 = -1;
                        Map[cell].Recalc_Attributes(-1);
                        Map[cell].Wall_Update(false);
                        Map.Radar_Background(cell);
                        Unknown_006D7C80(&Map[cell]);
                        Map.Update_Cell_Zone(cell);
                        Map.Unknown_00566340(cell);
                    }
                }
            }
        }
    }
}


// 0x004e8540: function not yet identified
//
// Body: 24 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_lookup_553c_004e8540(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004e8540 {

extern "C" unsigned int unknown_list_00a35cdc;

unsigned int __fastcall unknown_lookup_553c_004e8540(int house)
{
    if (*(int *)(house + 0x553c) != -1) {
        return *(unsigned int *)(unknown_list_00a35cdc + *(int *)(house + 0x553c) * 4);
    }
    return 0;
}

}  // namespace seed_004e8540


// BuildingClass *HouseClass::Find_Building(int type, int zone) (0x004E8560)
// A building of the house with this building type index, in a zone (any
// zone for -1); 0 when the house has none.

// BuildingTypes (shared/building_type.hpp, which this unit's views keep it from including).
extern DynamicVectorClass<BuildingTypeClass *> BuildingTypes;       // 0x00A35CD8

// (UnknownBuildingView: HouseClass_Silo_Redraw_Check_004e5460.cpp.)

BuildingClass *HouseClass::Find_Building(int type, int zone)
{
    if (unknown_5420.unknown_func_491d10(type) > 0) {
        int count = Buildings.Count();
        for (int index = 0; index < count; index++) {
            UnknownBuildingView *building = (UnknownBuildingView *)Buildings[index];
            if (building != 0 && !building->unknown_76 &&
                BuildingTypes.ID(building->Type) == type) {
                if (zone == -1) {
                    return (BuildingClass *)building;
                }
                if (Which_Zone(building->Center_Coord()) == zone) {
                    return (BuildingClass *)building;
                }
            }
        }
    }
    return 0;
}


// CoordStruct HouseClass::unknown_func_4e8610(int arg) const (0x004E8610)
// Returns the zero coordinate (this unit's copy of unknown_geometry_l, 0x00A40F48);
// the argument is unused. No caller in the image; owner and name are hypotheses.

CoordStruct HouseClass::unknown_func_4e8610(int arg) const
{
    return unknown_geometry_l;
}


// void HouseClass::unknown_func_4e8640() (0x004E8640)
// RA's Recalc_Center: the base centre is the cost-weighted average position
// of the house's live buildings, moved to the centre of a reachable cell
// near it; the radius their mean distance from it (at least 0x200); each
// zone's defence the sum of its buildings' anti-air, anti-armour and
// anti-infantry strengths.

// The distance between two coordinates (as shared/coord_distance.hpp's
// Coord_Distance, with the last two squares summed first).
inline int Base_Distance(CoordStruct const &a, CoordStruct const &b)
{
    CoordStruct d(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    return (int)unknown_sqrt_4ba410((double)d.X * d.X + ((double)d.Z * d.Z + (double)d.Y * d.Y));
}

void HouseClass::unknown_func_4e8640()
{
    BaseCenter = unknown_geometry_l;
    unknown_53c8 = 0;
    for (int zone = 0; zone < 5; zone++) {
        ZoneInfo[zone].AirDefense = 0;
        ZoneInfo[zone].ArmorDefense = 0;
        ZoneInfo[zone].InfantryDefense = 0;
    }
    if (CurBuildings > 0) {
        int x = 0;
        int y = 0;
        int count = 0;
        int buildings = Buildings.Count();
        for (int index = 0; index < buildings; index++) {
            UnknownBuildingView *building = (UnknownBuildingView *)Buildings[index];
            if (building != 0 && !building->unknown_76 && building->Strength > 0) {
                int weight = building->Type->Cost_Of(this) / 1000 + 1;
                for (int i = 0; i < weight; i++) {
                    x += building->Center_Coord().X;
                    y += building->Center_Coord().Y;
                    count++;
                }
            }
        }
        if (count > 0) {
            x /= count;
            y /= count;
            BaseCenter = CoordStruct(x, y, 0);
            BaseCenter = Cell_Coord(Map.Nearby_Location(Coord_Cell(BaseCenter), 0, -1, 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0));
        }
        if (count > 1) {
            int radius = 0;
            int buildings = Buildings.Count();
            for (int index = 0; index < buildings; index++) {
                UnknownBuildingView *building = (UnknownBuildingView *)Buildings[index];
                if (building != 0 && !building->unknown_76 && building->Strength > 0) {
                    radius += Base_Distance(BaseCenter, building->Center_Coord());
                }
            }
            radius /= count;
            if (radius <= 0x200) {
                radius = 0x200;
            }
            unknown_53c8 = radius;
            buildings = Buildings.Count();
            for (index = 0; index < buildings; index++) {
                UnknownBuildingView *building = (UnknownBuildingView *)Buildings[index];
                if (building != 0 && !building->unknown_76 && building->Strength > 0) {
                    int zone = Which_Zone(building->Center_Coord());
                    if (zone != -1) {
                        ZoneInfo[zone].ArmorDefense += building->unknown_6d5900();
                        ZoneInfo[zone].AirDefense += building->unknown_6d5870();
                        ZoneInfo[zone].InfantryDefense += building->unknown_6d59d0();
                    }
                }
            }
        } else {
            unknown_53c8 = 0x200;
        }
    }
}


// Expert-system house decisions, original 0x004E8990.

int HouseClass::Expert_AI()
{
    if(unknown_5530==0) {
        if(unknown_54f0==-1 && unknown_game_mode_a3d298!=0 && !Type->MultiplayPassive) {
            CoordStruct center=BaseCenter;
            if(center!=unknown_geometry_l) {
                int closest=0x7fffffff;
                HouseClass *enemy=0;
                for(int index=0;index<Houses.Count();++index) {
                    HouseClass *house=Houses[index];
                    if(house!=this && !house->Type->MultiplayPassive && !house->unknown_13d) {
                        CoordStruct offset=house->BaseCenter;
                        offset-=center;
                        int distance=(int)unknown_sqrt_4ba410((double)offset.X*offset.X+(double)offset.Z*offset.Z+(double)offset.Y*offset.Y);
                        if(distance<closest) {enemy=house;closest=distance;}
                    }
                }
                if(enemy) unknown_func_4eeba0(1,enemy);
            }
        }
    }
    if(unknown_54f0!=-1) {
        HouseClass *enemy=Houses[unknown_54f0];
        if(enemy->unknown_13d) {
            for(int index=0;index<AngerNodes.Count();++index) {
                if(AngerNodes[index].House==enemy) {
                    unknown_func_4eeba0(-AngerNodes[index].Value,enemy);
                    break;
                }
            }
            unknown_54f0=-1;
        }
    }
    if(unknown_game_mode_a3d298!=0 || unknown_194>=Rules->IQSuperWeapons) AI_Supers_004f3910();
    if(unknown_198==4) {
        Fire_Sale();
        All_To_Hunt();
    } else {
        if(unknown_198==0 && Available_Money()<25) unknown_198=1;
        if(unknown_198==1 && Available_Money()>=25) unknown_198=0;
        if(unknown_198==3 && unknown_5408+900<Frame) unknown_198=0;
        if(unknown_198!=3 && unknown_5408+900>Frame) unknown_198=3;
    }
    if(unknown_game_mode_a3d298!=0) {
        int urgency[2];
        int strategy;
        for(strategy=0;strategy<2;++strategy) {
            urgency[strategy]=0;
            switch(strategy) {
            case 0:
                urgency[0]=unknown_func_4e8d20();
                break;
            case 1: urgency[1]=Check_Raise_Money_004e8d80();break;
            default: urgency[strategy]=0;break;
            }
        }
        for(int level=4;level>=1;--level) {
            for(strategy=0;strategy<2;++strategy) {
                if(urgency[strategy]==level) {
                    switch(strategy) {
                    case 0: if(CurBuildings>0 && level==4) {Fire_Sale();All_To_Hunt();}break;
                    case 1: AI_Raise_Money_004e90f0(level);break;
                    }
                }
            }
        }
    }
    return 105+Random_Pick(1,7);
}


// int HouseClass::unknown_func_4e8d20() (0x004E8D20)
// 4 when the house has buildings but none whose type sets +0xC44; else 0.

int HouseClass::unknown_func_4e8d20()
{
    if (unknown_198 != 3 && CurBuildings > 0) {
        int count = Buildings.Count();
        for (int index = 0; index < count; index++) {
            UnknownBuildingView *building = (UnknownBuildingView *)Buildings[index];
            if (building != 0 && building->unknown_7d && !building->unknown_76 &&
                ((UnknownBuildingTypeView *)building->Type)->unknown_c44 != 0) {
                return 0;
            }
        }
        return 4;
    }
    return 0;
}


// Money urgency, original004E8D80;827body bytes. RA2 checks two alternatives.

int HouseClass::Check_Raise_Money_004e8d80()
{
    int urgency=0;
    if (Is_Human_Control()) return 0;
    if (!Can_Make_Money_004e27e0()) {
        bool has_refinery=unknown_5460.unknown_func_491d10(Rules->BuildRefinery[0]->ArrayIndex)>0 ||
                          unknown_5460.unknown_func_491d10(Rules->BuildRefinery[1]->ArrayIndex)>0;
        if (has_refinery) {
            for (int index=0;index<unknown_BuildingClass_list_00a40af8.Count();++index) {
                BuildingClass *building=unknown_BuildingClass_list_00a40af8[index];
                if (building->House==this &&
                    (building->Type==Rules->BuildRefinery[0] || building->Type==Rules->BuildRefinery[1]) &&
                    building->Mission==18) return 0;
            }
            if (unknown_5540!=Rules->unknown_9ec[0]->ArrayIndex &&
                unknown_5540!=Rules->unknown_9ec[1]->ArrayIndex) {
                if (Available_Money()<Rules->unknown_9ec[0]->Cost_Of(this) &&
                    Available_Money()<Rules->unknown_9ec[1]->Cost_Of(this)) urgency=1;
            } else {
            FactoryClass *pending=0;
            for (int factory_index=0;factory_index<Factories.Count();++factory_index) {
                if (Factories[factory_index]->Get_House_004e4090()==this && Factories[factory_index]->Get_Object()!=0) {
                    UnitClass *unit=(UnitClass *)Factories[factory_index]->Get_Object();
                    if (unit->Kind_Of()==1 &&
                        (unit->Type==Rules->unknown_9ec[0] || unit->Type==Rules->unknown_9ec[1])) {
                        pending=Factories[factory_index];
                        break;
                    }
                }
            }
            if (pending) {
                int owed=pending->UnspentCost;
                if (Available_Money()<owed) urgency=1;
            } else urgency=1;
            }
        } else {
            if (unknown_553c!=Rules->BuildRefinery[0]->ArrayIndex &&
                unknown_553c!=Rules->BuildRefinery[1]->ArrayIndex) {
                if (Available_Money()<Rules->BuildRefinery[0]->Cost_Of(this) &&
                    Available_Money()<Rules->BuildRefinery[1]->Cost_Of(this)) urgency=1;
            } else {
            FactoryClass *pending=0;
            for (int factory_index=0;factory_index<Factories.Count();++factory_index) {
                if (Factories[factory_index]->Get_House_004e4090()==this && Factories[factory_index]->Get_Object()!=0) {
                    BuildingClass *building=(BuildingClass *)Factories[factory_index]->Get_Object();
                    if (building->Kind_Of()==6 &&
                        (building->Type==Rules->BuildRefinery[0] || building->Type==Rules->BuildRefinery[1])) {
                        pending=Factories[factory_index];
                        break;
                    }
                }
            }
            if (pending) {
                int owed=pending->UnspentCost;
                if (Available_Money()<owed) urgency=1;
            } else urgency=1;
            }
        }
    }
    return urgency;
}


// bool HouseClass::unknown_func_4e90c0(int arg1) (0x004E90C0)
// For 4 with buildings left: sells every building and sends every unit to
// hunt (through the IHouse slots).

bool HouseClass::unknown_func_4e90c0(int arg1)
{
    if (CurBuildings > 0 && arg1 == 4) {
        Fire_Sale();
        All_To_Hunt();
        return true;
    }
    return false;
}


// Sell base assets to restore a refinery/harvester; original004E90F0.
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house);
bool HouseClass::AI_Raise_Money_004e90f0(int urgency)
{
    int refund=0;
    int needed;
    bool can_build_harvester=(unknown_5460.unknown_func_491d10(Rules->BuildRefinery[0]->ArrayIndex)>0 ||
                              unknown_5460.unknown_func_491d10(Rules->BuildRefinery[1]->ArrayIndex)>0) &&
                             (unknown_5460.unknown_func_491d10(Rules->BuildWeapons[0]->ArrayIndex)>0 ||
                              unknown_5460.unknown_func_491d10(Rules->BuildWeapons[1]->ArrayIndex)>0);
    if (can_build_harvester) needed=Unknown_004EF6F0(Rules->unknown_9ec)->Cost_Of(this);
    else needed=unknown_func_4ef600(Rules->BuildRefinery)->Cost_Of(this);
    int i=unknown_55f0.Nodes.Count()-1;
    for (;i>=0;--i) {
        BuildingClass *building=unknown_55f0.Get_Building(i);
        if (building) {
            char level=building->unknown_5c6;
            if (level!=0) {
                int cost=((BuildingTypeClass *)building->unknown_4c0[level-1])->unknown_slot46(this,true);
                Balance+=cost;
                building->unknown_44c860();
                unknown_5668=true;
                unknown_5669=true;
                Building_Refresh_House_Buildings_0044fa40(this);
            } else {
                building->Sell_Back(1);
                if (building->Type->Gate) unknown_55f0.Nodes.Delete(i);
                refund+=building->unknown_slot167();
            }
        } else if (unknown_55f0.Is_Built(i)) {
            Sell_Wall(unknown_55f0.Nodes[i].Cell,false);
            unknown_55f0.Nodes.Delete(i);
        }
        if (refund+Balance>needed) {
            for (int j=Factories.Count()-1;j>=0;--j) {
                FactoryClass *factory=Factories[j];
                if (factory->Owner==this) {
                    Factories[j]->Suspend_004B9630(true);
                    Factories[j]->Abandon_Production_004B97C0();
                    delete Factories[j];
                }
            }
            unknown_5540=-1;
            unknown_5544=-1;
            unknown_5548=-1;
            unknown_553c=-1;
            if (can_build_harvester) {
                unknown_5540=Unknown_004EF6F0(Rules->unknown_9ec)->ArrayIndex;
                unknown_12c=2;
            } else {
                int next=unknown_55f0.Next_Buildable_Index();
                if (next!=0) {
                    if (next==-1) {
                        if (unknown_55f0.Nodes.Length()<2) goto GO_HUNT;
                        next=1;
                    }
                    BaseNodeClass node(unknown_func_4ef600(Rules->BuildRefinery)->ArrayIndex,Cell(0,0));
                    unknown_55f0.Nodes.Insert_After(node,next-1);
                    for (int j=unknown_55f0.Nodes.Count()-1;j>next;--j) {
                        if (unknown_55f0.Nodes[j].Type==unknown_func_4ef600(Rules->BuildRefinery)->ArrayIndex)
                            unknown_55f0.Nodes.Delete(j);
                    }
                    unknown_12c=1;
                    AI_Building();
                } else goto GO_HUNT;
            }
            break;
        }
    }
    if (i<=0) {
GO_HUNT:
        All_To_Hunt();
    }
    return false;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int HouseClass::AI_Building() (0x004E9660)
// Picks the next base node to build. Block structure from ra2-decomp (YR 0x004FE3E0): everything
// after the naval-node retry sits inside `if (node != NULL) { ... }` (an early return there
// leaves the node copy's stores in source order: 99.42).
int HouseClass::AI_Building()
{
    if (unknown_553c != -1) return 15;
    if (unknown_50.Count() == 0) return 15;

    BaseNodeClass *node = unknown_55f0.Next_Buildable();
    if (node == 0) return 15;

    if (node->Type >= 0 && BuildingTypes[node->Type]->Naval && !unknown_138) {
        unknown_55f0.Nodes.Delete(unknown_55f0.Nodes.ID(node));
        node = unknown_55f0.Next_Buildable();
    }
    if (node != 0) {
        if (node->Type == -3) {
            unknown_55f0.Nodes.Delete(unknown_55f0.Nodes.ID(node));
            AI_Build_Wall();
            return 1;
        }

        BaseNodeClass copy = *node;
        if (copy.Type == -1 || (BuildingTypes[copy.Type] == Rules->unknown_78c && node->Cell == Cell(0, 0))) {
            int index = unknown_55f0.Nodes.ID(node);
            if (Random_Pick(0, 99) < Rules->AIPickWallDefensePercent[unknown_c8] && unknown_func_4f5d30(index)) {
                unknown_55f0.Nodes.Delete(unknown_55f0.Nodes.ID(copy));
                return 1;
            }
            DynamicVectorClass<Cell> *cells = 0;
            if (unknown_55f0.unknown_38.Count() > 0) cells = &unknown_55f0.unknown_38;
            if (!AI_Build_Defense(index, cells)) {
                if (node->Type == Rules->unknown_78c->ArrayIndex) {
                    unknown_55f0.Nodes.Delete(index);
                }
                unknown_55f0.Nodes.Delete(index);
                return 1;
            }
        }

        if (node->Type == -2) return 15;

        BuildingTypeClass *type = BuildingTypes[node->Type];
        if (Drain + type->PowerDrain_c6c > Power - unknown_15f88) {
            bool found = false;
            for (int index = 0; index < Rules->unknown_7c8.Count(); index++) {
                if (Rules->unknown_7c8[index] == type) {
                    found = true;
                    break;
                }
            }
            if (!found && type->PowerDrain_c6c > 0) {
                BuildingTypeClass *power;
                if (unknown_130 == 0) {
                    power = Rules->GDIPowerPlant;
                } else {
                    DynamicVectorClass<BuildingTypeClass const *> owned;
                    for (int bindex = Buildings.Count() - 1; bindex >= 0; bindex--) {
                        owned.Add((BuildingTypeClass const *)Buildings[bindex]);
                    }
                    if (unknown_func_4ef740(Rules->NodAdvancedPower, owned, owned.Count())) {
                        power = Rules->NodAdvancedPower;
                    } else {
                        power = Rules->NodRegularPower;
                    }
                }
                int at = unknown_55f0.Nodes.ID(node);
                unknown_55f0.Nodes.Insert_After(BaseNodeClass(power->ArrayIndex, Cell(0, 0), false, 0), at - 1);
                return 1;
            }
        }

        if (type->PowersUpToLevel <= 0 && type->PowersUpToLevel == -1 && node->Cell != unknown_cell_a40f10 && type->PowersUpBuilding[0] != 0) {
            BuildingClass *building = (BuildingClass *)Map[node->Cell].Cell_Building();
            BuildingTypeClass *upgrade = BuildingTypes[BuildingTypeClass::From_Name(type->PowersUpBuilding)];
            if (building == 0) {
                node->Cell = Cell(0, 0);
            } else if (building->Type != upgrade) {
                node->Cell = Cell(0, 0);
            } else if ((building->Type->PowersUpToLevel == -1 && building->unknown_5c6 >= building->Type->Upgrades)
                    || (building->Type->PowersUpToLevel > 0 && building->unknown_5c6 > 0)) {
                node->Cell = Cell(0, 0);
            }
        }
        unknown_553c = node->Type;
    }
    return 15;
}


// int HouseClass::AI_Unit() (0x004E9C90)
// Picks the unit type to build next: a harvester while refineries lack them for the teams (RA's AI_Unit).

int HouseClass::AI_Unit()
{
    if (unknown_5540 != -1) return 15;

    int harvesters = unknown_5470.unknown_func_491d10(Unknown_004EF6F0(Rules->unknown_9ec)->ArrayIndex);
    int refineries = unknown_5460.unknown_func_491d10(Rules->BuildRefinery[0]->ArrayIndex)
        + unknown_5460.unknown_func_491d10(Rules->BuildRefinery[1]->ArrayIndex);
    int per_refinery = Rules->HarvestersPerRefinery;
    if (unknown_194 >= Rules->IQHarvester && !unknown_189 && !Is_Human_Control()
            && per_refinery * refineries > harvesters) {
        if ((unsigned)Unknown_004EF6F0(Rules->unknown_9ec)->TechLevel <= (unsigned)unknown_11c) {
            unknown_5540 = Unknown_004EF6F0(Rules->unknown_9ec)->ArrayIndex;
            return 15;
        }
    }

    int counter[100];
    int earliest[100];
    int bestlist[100];
    memset(counter, 0, sizeof(counter));
    for (int x = 0; x < 100; x++) earliest[x] = 0x7FFFFFFF;

    for (int index = 0; index < Teams.Count(); index++) {
        TeamClass *team = Teams[index];
        if (team != 0) {
            TeamTypeClass *type = team->Class;
            int age = team->unknown_50;
            if (((type->Reinforce && !team->IsFullStrength) || (!team->IsForcedActive && !team->IsHasBeen)) && team->Owner == this) {
                DynamicVectorClass<TechnoTypeClass *> needed;
                team->unknown_func_6bd870(needed);
                for (int subindex = 0; subindex < needed.Count(); subindex++) {
                    TechnoTypeClass *member = needed[subindex];
                    if (member->Kind_Of() == 0x28) {
                        counter[((UnitTypeClass *)member)->ArrayIndex]++;
                        if (age < earliest[((UnitTypeClass *)member)->ArrayIndex]) earliest[((UnitTypeClass *)member)->ArrayIndex] = age;
                    }
                }
            }
        }
    }

    for (int uindex = 0; uindex < Units.Count(); uindex++) {
        UnitClass *unit = Units[uindex];
        if (unit != 0 && unit->unknown_4c8d00(this) && counter[unit->Type->ArrayIndex] > 0) {
            counter[unit->Type->ArrayIndex]--;
        }
    }

    int bestval = -1;
    int bestcount = 0;
    int best = -1;
    int bestearliest = 0x7FFFFFFF;
    for (int utype = 0; utype < UnitTypes.Count(); utype++) {
        int count = counter[utype];
        if (count > 0 && Can_Build(UnitTypes[utype], 0, 0)
                && UnitTypes[utype]->Cost_Of(this) <= Available_Money()) {
            if (bestval == -1 || bestval < count) {
                bestval = count;
                bestcount = 0;
            }
            bestlist[bestcount++] = utype;
            if (best == -1 || earliest[utype] < bestearliest) {
                best = utype;
                bestearliest = earliest[utype];
            }
        }
    }

    double chance = Random_Pick(0, 0x7FFFFFFE) * (1.0 / 2147483646.0);
    if (chance < Rules->FillEarliestTeamProbability[unknown_c8] * 0.01) {
        unknown_5540 = best;
    } else if (bestcount) {
        unknown_5540 = bestlist[Random_Pick(0, bestcount - 1)];
    }
    return 15;
}


// int HouseClass::AI_Infantry() (0x004EA1B0)
// Picks the infantry type to build next for the teams (RA's AI_Infantry).

int HouseClass::AI_Infantry()
{
    if (unknown_5544 != -1) return 15;

    int counter[100];
    int earliest[100];
    int bestlist[100];
    memset(counter, 0, sizeof(counter));
    for (int x = 0; x < 100; x++) earliest[x] = 0x7FFFFFFF;

    for (int index = 0; index < Teams.Count(); index++) {
        TeamClass *team = Teams[index];
        if (team != 0) {
            TeamTypeClass *type = team->Class;
            int age = team->unknown_50;
            if (((type->Reinforce && !team->IsFullStrength) || (!team->IsForcedActive && !team->IsHasBeen)) && team->Owner == this) {
                DynamicVectorClass<TechnoTypeClass *> needed;
                team->unknown_func_6bd870(needed);
                for (int subindex = 0; subindex < needed.Count(); subindex++) {
                    TechnoTypeClass *member = needed[subindex];
                    if (member->Kind_Of() == 0x10) {
                        counter[((InfantryTypeClass *)member)->ArrayIndex]++;
                        if (age < earliest[((InfantryTypeClass *)member)->ArrayIndex]) earliest[((InfantryTypeClass *)member)->ArrayIndex] = age;
                    }
                }
            }
        }
    }

    for (int iindex = 0; iindex < Infantry.Count(); iindex++) {
        InfantryClass *infantry = Infantry[iindex];
        if (infantry != 0 && infantry->unknown_4c8d00(this) && counter[infantry->Type->ArrayIndex] > 0) {
            counter[infantry->Type->ArrayIndex]--;
        }
    }

    int bestval = -1;
    int bestcount = 0;
    int best = -1;
    int bestearliest = 0x7FFFFFFF;
    for (int itype = 0; itype < InfantryTypes.Count(); itype++) {
        int count = counter[itype];
        if (count > 0 && Can_Build(InfantryTypes[itype], 0, 0)
                && InfantryTypes[itype]->Cost_Of(this) <= Available_Money()) {
            if (bestval == -1 || bestval < count) {
                bestval = count;
                bestcount = 0;
            }
            bestlist[bestcount++] = itype;
            if (best == -1 || earliest[itype] < bestearliest) {
                best = itype;
                bestearliest = earliest[itype];
            }
        }
    }

    double chance = Random_Pick(0, 0x7FFFFFFE) * (1.0 / 2147483646.0);
    if (chance < Rules->FillEarliestTeamProbability[unknown_c8] * 0.01) {
        unknown_5544 = best;
    } else if (bestcount) {
        unknown_5544 = bestlist[Random_Pick(0, bestcount - 1)];
    }
    return 15;
}


// int HouseClass::AI_Aircraft() (0x004EA4D0)
// Picks the aircraft type to build next for the teams (RA's AI_Aircraft).

int HouseClass::AI_Aircraft()
{
    if (unknown_5548 != -1) return 15;

    int counter[100];
    int earliest[100];
    int bestlist[100];
    memset(counter, 0, sizeof(counter));
    for (int x = 0; x < 100; x++) earliest[x] = 0x7FFFFFFF;

    for (int index = 0; index < Teams.Count(); index++) {
        TeamClass *team = Teams[index];
        if (team != 0) {
            TeamTypeClass *type = team->Class;
            int age = team->unknown_50;
            if (((type->Reinforce && !team->IsFullStrength) || (!team->IsForcedActive && !team->IsHasBeen)) && team->Owner == this) {
                DynamicVectorClass<TechnoTypeClass *> needed;
                team->unknown_func_6bd870(needed);
                for (int subindex = 0; subindex < needed.Count(); subindex++) {
                    TechnoTypeClass *member = needed[subindex];
                    if (member->Kind_Of() == 3) {
                        counter[((AircraftTypeClass *)member)->ArrayIndex]++;
                        if (age < earliest[((AircraftTypeClass *)member)->ArrayIndex]) earliest[((AircraftTypeClass *)member)->ArrayIndex] = age;
                    }
                }
            }
        }
    }

    for (int aindex = 0; aindex < Aircraft.Count(); aindex++) {
        AircraftClass *aircraft = Aircraft[aindex];
        if (aircraft != 0 && aircraft->unknown_4c8d00(this) && counter[aircraft->Type->ArrayIndex] > 0) {
            counter[aircraft->Type->ArrayIndex]--;
        }
    }

    int bestval = -1;
    int bestcount = 0;
    int best = -1;
    int bestearliest = 0x7FFFFFFF;
    for (int atype = 0; atype < AircraftTypes.Count(); atype++) {
        int count = counter[atype];
        if (count > 0 && Can_Build(AircraftTypes[atype], 0, 0)
                && AircraftTypes[atype]->Cost_Of(this) <= Available_Money()) {
            if (bestval == -1 || bestval < count) {
                bestval = count;
                bestcount = 0;
            }
            bestlist[bestcount++] = atype;
            if (best == -1 || earliest[atype] < bestearliest) {
                best = atype;
                bestearliest = earliest[atype];
            }
        }
    }

    double chance = Random_Pick(0, 0x7FFFFFFE) * (1.0 / 2147483646.0);
    if (chance < Rules->FillEarliestTeamProbability[unknown_c8] * 0.01) {
        unknown_5548 = best;
    } else if (bestcount) {
        unknown_5548 = bestlist[Random_Pick(0, bestcount - 1)];
    }
    return 15;
}


// void HouseClass::unknown_4ea800(TechnoClass *techno) (0x004EA800)
// A techno leaves the house's counts (AircraftClass/InfantryClass destructors):
// clears its +0x324 flag and, unless its type is flagged (+0x202), lowers the
// count of its kind (+0x228 units/infantry, +0x238, +0x230, +0x234; an
// unoccupied naval vehicle also lowers +0x22C) and the per-type counter
// (+0x5420..+0x5450) of its type's array index.

void HouseClass::unknown_4ea800(TechnoClass *techno)
{
    techno->unknown_324 = false;
    if (!techno->Techno_Type_Class()->unknown_202) {
        switch (techno->Kind_Of()) {
        case 6:
            if (techno->unknown_slot31()) {
                unknown_228--;
            } else {
                CurBuildings--;
            }
            unknown_5420.Decrement(techno->Techno_Type_Class()->Fetch_Heap_ID());
            break;
        case 2:
            unknown_238--;
            unknown_5450.Decrement(techno->Techno_Type_Class()->Fetch_Heap_ID());
            break;
        case 15:
            unknown_234--;
            unknown_5440.Decrement(techno->Techno_Type_Class()->Fetch_Heap_ID());
            break;
        case 1:
            unknown_228--;
            unknown_5430.Decrement(techno->Techno_Type_Class()->Fetch_Heap_ID());
            if (techno->Techno_Type_Class()->Naval && techno->Techno_Type_Class()->Passengers == 0) {
                unknown_22c--;
            }
            break;
        }
    }
}


// void HouseClass::unknown_4ea970(TechnoClass *techno) (0x004EA970)
// A techno joins the house's counts (AircraftClass::Init; the counterpart of
// 0x004EA800): sets its +0x324 flag and, unless its type is flagged (+0x202),
// raises the count of its kind (+0x228 units, +0x230 buildings, +0x238,
// +0x234; an unoccupied naval vehicle also +0x22C), the per-type counter
// (+0x5420..+0x5450) and the kind's tracker (+0x268..+0x1A80) of its type's
// array index.

void HouseClass::unknown_4ea970(TechnoClass *techno)
{
    techno->unknown_324 = true;
    if (!techno->Techno_Type_Class()->unknown_202) {
        switch (techno->Kind_Of()) {
        case 6: {
            if (techno->unknown_slot31()) {
                unknown_228++;
            } else {
                CurBuildings++;
            }
            int index = techno->Techno_Type_Class()->Fetch_Heap_ID();
            unknown_5420.Increment(index);
            unknown_1a80.Unknown_0070C490(index);
            break;
        }
        case 2: {
            unknown_238++;
            int index = techno->Techno_Type_Class()->Fetch_Heap_ID();
            unknown_5450.Increment(index);
            unknown_268.Unknown_0070C490(index);
            break;
        }
        case 15: {
            int index = techno->Techno_Type_Class()->Fetch_Heap_ID();
            unknown_234++;
            unknown_5440.Increment(index);
            unknown_a70.Unknown_0070C490(index);
            break;
        }
        case 1: {
            unknown_228++;
            int index = techno->Techno_Type_Class()->Fetch_Heap_ID();
            unknown_5430.Increment(index);
            unknown_1278.Unknown_0070C490(index);
            if (techno->Techno_Type_Class()->Naval && techno->Techno_Type_Class()->Passengers == 0) {
                unknown_22c++;
            }
            break;
        }
        }
    }
}


// int *HouseClass::unknown_func_4eab10(int kind, bool naval) (0x004EAB10)
// The counter per object kind at +0x52B8..+0x52C8 (0 for other kinds).

int *HouseClass::unknown_func_4eab10(int kind, bool naval)
{
    switch (kind) {
    case 1:
    case 40:
        if (naval) {
            return &unknown_52c8;
        }
        return &unknown_52c0;
    case 2:
    case 3:
        return &unknown_52b8;
    case 15:
    case 16:
        return &unknown_52bc;
    case 6:
    case 7:
        return &unknown_52c4;
    }
    return 0;
}


// void HouseClass::unknown_func_4eaba0(TechnoClass *techno) (0x004EABA0)
// Lowers the counter of 0x004EAB10 for the kind a building's type names
// at +0xC44.

void HouseClass::unknown_func_4eaba0(TechnoClass *techno)
{
    if (techno->Kind_Of() == 6) {
        TechnoTypeClass *type = techno->Techno_Type_Class();
        int *value = unknown_func_4eab10(((UnknownBuildingTypeView *)((UnknownBuildingView *)techno)->Type)->unknown_c44,
                                         type != 0 && type->Naval);
        if (value != 0) {
            (*value)--;
        }
    }
}


// void HouseClass::unknown_func_4eac70(TechnoClass *techno) (0x004EAC70)
// Raises the counter of 0x004EAB10 for the kind a building's type names
// at +0xC44.

void HouseClass::unknown_func_4eac70(TechnoClass *techno)
{
    if (techno->Kind_Of() == 6) {
        TechnoTypeClass *type = techno->Techno_Type_Class();
        int *value = unknown_func_4eab10(((UnknownBuildingTypeView *)((UnknownBuildingView *)techno)->Type)->unknown_c44,
                                         type != 0 && type->Naval);
        if (value != 0) {
            (*value)++;
        }
    }
}


// int HouseClass::Which_Zone(CoordStruct const &coord) const (0x004EAD40)
// The base zone of a coordinate (RA's Which_Zone): -1 for no coordinate or
// beyond four radii of the base centre, 0 within the radius (+0x53C8), else
// by the direction from the centre (1-4; the compares are the original's).
// The direction from one coordinate to another, the X values read into
// locals (hypothesis: Westwood's Direction; Which_Zone expands it).
inline UnknownFacingDir Coord_Direction(CoordStruct const &from, CoordStruct const &to)
{
    int from_x = from.X;
    int to_x = to.X;
    return UnknownFacingDir((unsigned short)((Fast_Atan2((double)from.Y - to.Y, (double)to_x - from_x)
        - 1.5707963267948966) * -10430.060040584269));
}

int HouseClass::Which_Zone(CoordStruct const &coord) const
{
    if (coord == unknown_geometry_l) {
        return -1;
    }
    int distance = Coord_Distance(BaseCenter, coord);
    if (distance <= unknown_53c8) {
        return 0;
    }
    if (distance > unknown_53c8 * 4) {
        return -1;
    }
    int facing = Dir_To_256(Coord_Direction(BaseCenter, coord));
    if (facing > 0x20 && facing < 0xE0) {
        return 1;
    }
    if (facing > 0xA0 && facing < 0x60) {
        return 3;
    }
    if (facing <= 0x20 && facing < 0x60) {
        return 2;
    }
    return 4;
}


// int HouseClass::Which_Zone(ObjectClass const *object) const (0x004EAEB0)
// The base zone of an object (-1 for none).

int HouseClass::Which_Zone(ObjectClass const *object) const
{
    if (object == 0) {
        return -1;
    }
    return Which_Zone(object->Center_Coord());
}


// int HouseClass::Which_Zone(Cell const &cell) const (0x004EAEF0)
// The base zone of a cell (its center coordinate).

int HouseClass::Which_Zone(Cell const &cell) const
{
    return Which_Zone(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0));
}


// Cell HouseClass::Zone_Cell(int zone) const (0x004EAF30)
// The cell at the centre of a base zone (RA's Zone_Cell): the base centre
// for zone 0, else three radii (+0x53C8) from it to the north (1), east (2),
// south (3) or west (4); unknown_cell_a40f10 for any other zone.

// A coordinate moved a distance along a direction (raw 16-bit, 0 north,
// clockwise; folded to its angle in radians).
inline UnknownVector3 Zone_Coord_Move(CoordStruct const &coord, short dir, int distance)
{
    double radians = (dir - 0x3FFF) * -(2 * 3.141592653589793 / 65534);
    int x, y, z;
    z = coord.Z;
    y = (int)(coord.Y - (double)Fast_Sin(radians) * distance);
    x = (int)(coord.X + (double)Fast_Cos(radians) * distance);
    UnknownVector3 result(x, y, z);
    return result;
}

Cell HouseClass::Zone_Cell(int zone) const
{
    switch (zone) {
    case 0:
        return Coord_Cell(BaseCenter);
    case 1: {
        UnknownVector3 coord = Zone_Coord_Move(BaseCenter, (short)0x0000, unknown_53c8 * 3);
        return Coord_Cell(CoordStruct(coord.X, coord.Y, coord.Z));
    }
    case 2: {
        UnknownVector3 coord = Zone_Coord_Move(BaseCenter, (short)0x4000, unknown_53c8 * 3);
        return Coord_Cell(CoordStruct(coord.X, coord.Y, coord.Z));
    }
    case 4: {
        UnknownVector3 coord = Zone_Coord_Move(BaseCenter, (short)0xC000, unknown_53c8 * 3);
        return Coord_Cell(CoordStruct(coord.X, coord.Y, coord.Z));
    }
    case 3: {
        UnknownVector3 coord = Zone_Coord_Move(BaseCenter, (short)0x8000, unknown_53c8 * 3);
        return Coord_Cell(CoordStruct(coord.X, coord.Y, coord.Z));
    }
    }
    return unknown_cell_a40f10;
}


// Cell HouseClass::Unknown_004EB270(FootClass *foot) (0x004EB270)
// RA's Where_To_Go: a unit without weapons goes to the core zone, others to
// a random outer zone; the cell is the reachable one (tracked) nearest a
// random cell of that zone, in the zone of the unit's own cell.

inline Cell Coord_Cell_Value(CoordStruct coord)
{
    return Coord_Cell(coord);
}

Cell HouseClass::Unknown_004EB270(FootClass *foot)
{
    int zone;
    if (foot->unknown_6d59d0() + foot->unknown_6d5900() + foot->unknown_6d5870() == 0) {
        zone = 0;
    } else {
        zone = Random_Pick(1, 4);
    }
    Cell cell = Random_Cell_In_Zone(zone);
    return Map.Nearby_Location(cell, 1, Map.Zone(Coord_Cell_Value(foot->Coord), 0, false), 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
}


// AbstractClass *HouseClass::unknown_func_4eb360(CoordStruct const &coord) (0x004EB360)
// RA's Find_Juicy_Target, around a coordinate: of the units in the 369 cells
// nearest its cell (rings of unknown_cells_a6f248) that are not allied, stand
// outside their owner's zones, have slot 108 at least -20 and +0x1B8 not 2,
// the nearest (anti-air units count double distance).

extern Cell unknown_cells_a6f248[369];      // 0x00A6F248 (cell offsets in rings around 0, 0)
extern int unknown_cell_count_7a5934;       // 0x007A5934 (369, their count)

AbstractClass *HouseClass::unknown_func_4eb360(CoordStruct const &coord)
{
    TechnoClass *best = 0;
    int value = 0;
    Cell center = coord;
    int count = unknown_cell_count_7a5934;
    for (int index = 0; index < count; index++) {
        TechnoClass *unit = (TechnoClass *)Map[center + unknown_cells_a6f248[index]].Cell_Unit(true);
        if (unit != 0 && !unit->unknown_76 && !Is_Ally(unit) && unit->House->Which_Zone(unit) == -1
                && unit->Get_Height_AGL() >= -20 && unit->unknown_1b8 != 2) {
            int distance = Coord_Distance(coord, unit->Center_Coord());
            if (unit->unknown_6d5870() != 0) {
                distance *= 2;
            }
            if (value == 0 || distance < value) {
                value = distance;
                best = unit;
            }
        }
    }
    if (best != 0) {
        return best;
    }
    return 0;
}


// static int __fastcall HouseClass::unknown_func_4eb550(int kind, int index) (0x004EB550)
// A building type's +0xBA0 value (0 for other kinds and for no index).

int __fastcall HouseClass::unknown_func_4eb550(int kind, int index)
{
    if (index < 0) {
        return 0;
    }
    TechnoTypeClass *type = Unknown_00482C00(kind, index);
    if (kind == 6 || kind == 7) {
        return ((UnknownBuildingTypeBA0View *)type)->unknown_ba0;
    }
    return 0;
}


// FactoryClass *HouseClass::unknown_func_4eb580(int kind, bool naval, int arg3) (0x004EB580)
// The house's factory for an object kind (+0x52D8..+0x52F8).
// Cases are object kinds: 1 unit, 2/3 aircraft, 6/7 building, 15/16
// infantry, 40 (all hypotheses); arg3 5 picks the second building entry.

FactoryClass *HouseClass::unknown_func_4eb580(int kind, bool naval, int arg3)
{
    FactoryClass *factory;
    switch (kind) {
    case 15:
    case 16:
        factory=unknown_52dc;
        break;
    case 1:
    case 40:
        if (naval) factory=unknown_52e4;
        else factory=unknown_52e0;
        break;
    case 6:
    case 7:
        if (arg3==5) factory=unknown_52f8;
        else factory=unknown_52e8;
        break;
    case 2:
    case 3:
        factory=unknown_52d8;
        break;
    default:
        factory=0;
        break;
    }
    return factory;
}


// void HouseClass::unknown_func_4eb620(int kind, bool naval, int arg3) (0x004EB620)
// Sets the flag per object kind.
// Cases are object kinds: 1 unit, 2/3 aircraft, 6/7 building, 15/16
// infantry, 40 (all hypotheses); arg3 5 picks the second building entry.

void HouseClass::unknown_func_4eb620(int kind, bool naval, int arg3)
{
    switch (kind) {
    case 15:
    case 16:
        unknown_52fd = true;
        // fall through
    case 1:
    case 40:
        if (naval) {
            unknown_52ff = true;
        } else {
            unknown_52fe = true;
        }
        break;
    case 6:
    case 7:
        if (arg3 == 5) {
            unknown_5304 = true;
        } else {
            unknown_5300 = true;
        }
        break;
    case 2:
    case 3:
        unknown_52fc = true;
        break;
    }
}


// void HouseClass::unknown_func_4eb6d0(int kind, bool naval, int arg3) (0x004EB6D0)
// Clears the flag per object kind.
// Cases are object kinds: 1 unit, 2/3 aircraft, 6/7 building, 15/16
// infantry, 40 (all hypotheses); arg3 5 picks the second building entry.

void HouseClass::unknown_func_4eb6d0(int kind, bool naval, int arg3)
{
    switch (kind) {
    case 15:
    case 16:
        unknown_52fd = false;
        // fall through
    case 1:
    case 40:
        if (naval) {
            unknown_52ff = false;
        } else {
            unknown_52fe = false;
        }
        break;
    case 6:
    case 7:
        if (arg3 == 5) {
            unknown_5304 = false;
        } else {
            unknown_5300 = false;
        }
        break;
    case 2:
    case 3:
        unknown_52fc = false;
        break;
    }
}


// bool HouseClass::unknown_func_4eb770(int kind, bool naval, int arg3) (0x004EB770)
// The flag per object kind.
// Cases are object kinds: 1 unit, 2/3 aircraft, 6/7 building, 15/16
// infantry, 40 (all hypotheses); arg3 5 picks the second building entry.

bool HouseClass::unknown_func_4eb770(int kind, bool naval, int arg3)
{
    switch (kind) {
    case 15:
    case 16:
        return unknown_52fd;
    case 1:
    case 40:
        if (naval) {
            return unknown_52ff;
        }
        return unknown_52fe;
    case 6:
    case 7:
        if (arg3 == 5) {
            return unknown_5304;
        }
        return unknown_5300;
    case 2:
    case 3:
        return unknown_52fc;
    }
    return false;
}


// Original House unit keeps the only flag-setter call at004E5EFC; exhaustive flag-store audit finds no inlined copy.
#pragma auto_inline(off)
// void HouseClass::unknown_func_4eb810(int kind, bool naval, int arg3, bool value) (0x004EB810)
// Sets the flag per object kind to a value.
// Cases are object kinds: 1 unit, 2/3 aircraft, 6/7 building, 15/16
// infantry, 40 (all hypotheses); arg3 5 picks the second building entry.

void HouseClass::unknown_func_4eb810(int kind, bool naval, int arg3, bool value)
{
    switch (kind) {
    case 15:
    case 16:
        unknown_52fd = value;
        // fall through
    case 1:
    case 40:
        if (naval) {
            unknown_52ff = value;
        } else {
            unknown_52fe = value;
        }
        break;
    case 6:
    case 7:
        if (arg3 == 5) {
            unknown_5304 = value;
        } else {
            unknown_5300 = value;
        }
        break;
    case 2:
    case 3:
        unknown_52fc = value;
        break;
    }
}

#pragma auto_inline(on)


// void HouseClass::Set_Factory(FactoryClass *factory, int kind, bool naval, int buildcat) (0x004EB8C0)
// Stores the house's factory for an object kind (the slots unknown_func_4eb580
// reads; name after TS's HouseClass::Set_Factory).

void HouseClass::Set_Factory(FactoryClass *factory, int kind, bool naval, int buildcat)
{
    switch (kind) {
    case 1:
    case 40:
        if (naval) {
            unknown_52e4 = factory;
        } else {
            unknown_52e0 = factory;
        }
        break;
    case 15:
    case 16:
        unknown_52dc = factory;
        break;
    case 6:
    case 7:
        if (buildcat == 5) {
            unknown_52f8 = factory;
        } else {
            unknown_52e8 = factory;
        }
        break;
    case 2:
    case 3:
        unknown_52d8 = factory;
        break;
    }
}


// int HouseClass::unknown_func_4eb980(int kind, bool naval) (0x004EB980)
// The counter of 0x004EAB10 for an object kind (0 for other kinds).

int HouseClass::unknown_func_4eb980(int kind, bool naval)
{
    int *value = unknown_func_4eab10(kind, naval);
    if (value != 0) {
        return *value;
    }
    return 0;
}


// void __fastcall HouseClass::Read_Scenario_INI(CCINIClass &ini) (0x004EBA20)
// Makes a house for every [Houses] entry (its Country= type, the first type
// when unknown; the entry value becomes its INI name), reads each house's
// section and, outside multiplayer, assigns the scenario difficulty (the
// computer difficulty to houses that are neither human nor player
// controlled). Without [Houses], one house per house type is made and read.

void __fastcall HouseClass::Read_Scenario_INI(CCINIClass &ini)
{
    int count = ini.Entry_Count("Houses");
    for (int index = 0; index < count; index++) {
        char name[20];
        ini.Get_String("Houses", ini.Get_Entry("Houses", index), "", name, sizeof(name));
        int type = ini.Get_HousesType(name, "Country", -1);
        if (type == -1) {
            type = 0;
        }
        HouseClass *house = new HouseClass(HouseTypes[type]);
        house->IniName = FixedString<21>(name);
    }

    for (int i = 0; i < Houses.Count(); i++) {
        HouseClass *house = Houses[i];
        house->Read_INI(ini);
        if (!unknown_game_mode_a3d298 && (house->unknown_134 || house->unknown_135)) {
            house->Assign_Handicap(unknown_id_source_a3d290->Difficulty);
        } else if (!unknown_game_mode_a3d298) {
            house->Assign_Handicap(unknown_id_source_a3d290->unknown_610);
        }
    }

    if (Houses.Count() == 0) {
        for (int type = 0; type < HouseTypes.Count(); type++) {
            HouseClass *house = new HouseClass(HouseTypes[type]);
            house->Read_INI(ini);
        }
    }
}


// void HouseClass::Read_INI(CCINIClass &ini) (0x004EBBB0)
// Reads the house's own section (named by its IniName): tech level, credits
// (the campaign money delta for a player-controlled house outside
// multiplayer), UIName (a string-table label, else the type's name), the
// team ratios, IQ, edge, colour (and the laser colour derived from it),
// allies, the first team delay and the base plan.

void HouseClass::Read_INI(CCINIClass &ini)
{
    char section[20];
    strcpy(section, IniName);

    unknown_11c = ini.Get_Int(section, "TechLevel", unknown_id_source_a3d290->TechLevel);
    unknown_124 = ini.Get_Int(section, "Credits", 0) * 100;
    unknown_135 = ini.Get_Bool(section, "PlayerControl", false);
    if (unknown_135 && !unknown_game_mode_a3d298) {
        if (Options.Difficulty == 0) {
            Balance = Rules->CampaignMoneyDeltaEasy + unknown_124;
        } else if (Options.Difficulty == 2) {
            Balance = Rules->CampaignMoneyDeltaHard + unknown_124;
        } else {
            Balance = unknown_124;
        }
        Balance = Balance > 0 ? Balance : 0;
    } else {
        Balance = unknown_124;
    }

    char buffer[100];
    ini.Get_String(section, "UIName", unknown_15ef5, buffer, 32);
    unknown_15ef5 = buffer;
    if (unknown_15ef5[0] != 0) {
        wcsncpy(UIName, StringTable::Load_String(unknown_15ef5, 0, "D:\\Ra2\\House.CPP", 7898), 32);
        UIName[20] = 0;
    } else {
        wcscpy(UIName, Type->UIName);
    }

    unknown_554c = ini.Get_Int(section, "RatioAITriggerTeam", unknown_554c);
    unknown_5550 = ini.Get_Int(section, "RatioTeamAircraft", 75);
    unknown_5554 = ini.Get_Int(section, "RatioTeamInfantry", 75);
    unknown_5558 = ini.Get_Int(section, "RatioTeamUnits", 75);

    unknown_130 = Type->Side;
    if (unknown_130 == -1) {
        unknown_130 = 0;
    }

    int iq = ini.Get_Int(section, "IQ", 0);
    if (iq > Rules->MaxIQ) {
        iq = 1;
    }
    unknown_118 = iq;
    unknown_194 = iq;

    unknown_128 = ini.Get_SourceType(section, "Edge", (SourceType)0);
    ColorSchemeIndex = ini.Get_ColorScheme(section, "Color", ColorSchemeIndex);
    Update_Color();
    Update_Laser_Color();

    unknown_56cc = 0;
    Make_Ally(Houses[ArrayIndex], false);
    int allies = ini.Get_Houses(section, "Allies", 0);
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *house = Houses[index];
        if (allies & (1 << house->ArrayIndex)) {
            Make_Ally(house, false);
        }
    }

    unknown_5684 = CDTimerClass<FrameTimerClass>(Rules->TeamDelays[unknown_c8] + ArrayIndex * 175);
    unknown_55f0.Read_INI(ini, section);
    unknown_55f0.House = this;
}


// void __fastcall HouseClass::Write_Scenario_INI(CCINIClass &ini) (0x004EC180)
// The [Houses] list (index = name), then each house's own section.

void __fastcall HouseClass::Write_Scenario_INI(CCINIClass &ini)
{
    ini.Clear("Houses", 0);
    for (int index = 0; index < Houses.Count(); index++) {
        char name[32];
        char entry[32];
        strcpy(name, Houses[index]->IniName);
        sprintf(entry, "%d", index);
        ini.Put_String("Houses", entry, name);
    }
    for (int i = 0; i < Houses.Count(); i++) {
        Houses[i]->Write_INI(ini);
    }
}


// void HouseClass::Write_INI(CCINIClass &ini) (0x004EC230)
// Writes the house's own section (cleared first): Country, TechLevel,
// Credits, IQ, Edge, PlayerControl, Color, Allies (from +0x120), UIName when
// set, and the base plan.

void HouseClass::Write_INI(CCINIClass &ini)
{
    char section[20];
    strcpy(section, IniName);

    ini.Clear(section);
    ini.Put_String(section, "Country", Type->ID);
    ini.Put_Int(section, "TechLevel", unknown_11c);
    ini.Put_Int(section, "Credits", unknown_124 / 100);
    ini.Put_Int(section, "IQ", unknown_118);
    ini.Put_SourceType(section, "Edge", (SourceType)unknown_128);
    ini.Put_Bool(section, "PlayerControl", unknown_135);
    ini.Put_ColorScheme(section, "Color", ColorSchemeIndex);

    int allies = 0;
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *house = Houses[index];
        if (unknown_120 & (1 << house->ArrayIndex)) {
            allies |= 1 << house->ArrayIndex;
        }
    }
    ini.Put_Houses(section, "Allies", allies);

    if (unknown_15ef5[0] != 0) {
        ini.Put_String(section, "UIName", unknown_15ef5);
    }
    unknown_55f0.Write_INI(ini, section);
}


// HRESULT __stdcall HouseClass::Fire_Sale() (0x004EC3C0)
// Sells every building of the house (IHouse).

HRESULT __stdcall HouseClass::Fire_Sale()
{
    if (CurBuildings > 0) {
        int count = Buildings.Count();
        for (int index = 0; index < count; index++) {
            UnknownBuildingView *building = (UnknownBuildingView *)Buildings[index];
            if (building != 0 && !building->unknown_76 && building->Strength > 0) {
                building->Sell_Back(1);
            }
        }
        return S_OK;
    }
    return 1;   // S_FALSE: nothing to sell
}


// HRESULT __stdcall HouseClass::All_To_Hunt() (0x004EC420)
// Sends every active unit of the house to hunt (out of its team) and lets
// the buildings that can do so act on it (IHouse).


// TeamClass and FootClass as far as this unit calls them (views).
struct UnknownTeamView {
    void unknown_func_6b8fb0(TechnoClass *techno, int arg2, int arg3);   // 0x006B8FB0 (removes a member; hypothesis)
};
struct UnknownFootView : public TechnoClass {
    char unknown_0418[0x4C4 - 0x418];
    UnknownTeamView *Team;                  // +0x4C4
};

// AbstractClass flag bit 2: a FootClass (hypothesis).
inline bool Is_Foot_4ec420(AbstractClass const *object)
{
    return object->AbstractFlags.IsFoot;
}

HRESULT __stdcall HouseClass::All_To_Hunt()
{
    for (int index = Technos.Count() - 1; index >= 0; index--) {
        TechnoClass *techno = Technos[index];
        if (this == techno->House && techno->unknown_74 && !techno->unknown_76 && techno != 0) {
            bool is_foot = Is_Foot_4ec420(techno);
            if (is_foot) {
                UnknownFootView *foot = (UnknownFootView *)techno;
                if (foot->Team != 0) {
                    foot->Team->unknown_func_6b8fb0(foot, -1, 0);
                }
                foot->QueueMission(15, false);   // hunt (hypothesis)
            }
            if (!is_foot && techno->Kind_Of() == 6) {
                UnknownBuildingView *building = (UnknownBuildingView *)techno;
                if (building->unknown_func_4532f0() > 0) {
                    building->unknown_func_452f50(1);
                }
            }
        }
    }
    unknown_190 = true;
    return S_OK;
}


// bool HouseClass::Is_Allowed_To_Ally(HouseClass *house) (0x004EC4E0)
// Whether the house may ally with another: not already allied, not the
// civilian side in a multiplayer game, and not the last unallied player.

// SideClass as far as this unit calls it.
class SideClass {
public:
    static int __fastcall Find_Index(char const *name);   // 0x0067AF90
};
extern int unknown_a40774;                  // 0x00A40774

bool HouseClass::Is_Allowed_To_Ally(HouseClass *house)
{
    if (Is_Ally(house)) {
        return false;
    }
    if (house->Type->Side == SideClass::Find_Index("Civilian") && unknown_game_mode_a3d298 != 0) {
        return false;
    }
    if (unknown_a40774 != 0) {
        return true;
    }
    if (unknown_13d) {
        return false;
    }
    int players = 0;
    int allies = 0;
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *other = Houses[index];
        if (!other->unknown_13d && !other->Type->MultiplayPassive) {
            players++;
            if (Is_Ally(other)) {
                allies++;
            }
        }
    }
    if (players == allies + 1) {
        return false;
    }
    return true;
}


// void HouseClass::unknown_func_4ec5e0() (0x004EC5E0)
// When the game object at 0x00A3D29C asks for it (slot 1) or the rules set
// +0x111D: every computer house allies with every other computer house and
// becomes the enemy of every human one (hypothesis).

// The object at 0x00A3D29C (view).
class UnknownA3D29CView {
public:
    virtual void unknown_slot0();
    virtual bool unknown_slot1();
};
extern UnknownA3D29CView *unknown_a3d29c;   // 0x00A3D29C

inline bool Is_Controlled_4ec5e0(HouseClass const *house)
{
    if (house->unknown_134 || house->unknown_135) {
        return true;
    }
    return false;
}

// A house a player controls (+0x134; outside multiplayer also +0x135).
inline bool Is_Human_4ec5e0(HouseClass const *house)
{
    if (unknown_game_mode_a3d298 != 0) {
        return house->unknown_134;
    }
    return Is_Controlled_4ec5e0(house);
}

void HouseClass::unknown_func_4ec5e0()
{
    bool forced = Rules->unknown_111d;
    if ((unknown_a3d29c != 0 && unknown_a3d29c->unknown_slot1()) || forced) {
        for (int index = 0; index < Houses.Count(); index++) {
            HouseClass *house = Houses[index];
            if (house != 0 && !house->unknown_13d && !Is_Human_4ec5e0(house)) {
                house->unknown_191 = true;
                for (int other_index = 0; other_index < Houses.Count(); other_index++) {
                    HouseClass *other = Houses[other_index];
                    if (other != 0 && !other->unknown_13d) {
                        if (Is_Human_4ec5e0(other)) {
                            house->Make_Enemy(other, false);
                        } else {
                            house->Make_Ally(other, false);
                        }
                    }
                }
            }
        }
    }
}


// void HouseClass::Adjust_Power(int amount) (0x004EC6E0)
// Adds to the power output (only while 0x00A40958 is set); when the house
// turns under- or re-powered, every sink of its outgoing interface is told.

void HouseClass::Adjust_Power(int amount)
{
    if (unknown_flag_a40958) {
        bool was_low = Power < Drain;
        Power += amount;
        unknown_func_4ec9b0();
        bool low = Power < Drain;
        if (!unknown_a40774 && was_low != low) {
            IConnectionPointContainer *container;
            QueryInterface(IID_IConnectionPointContainer, (void **)&container);
            IEnumConnectionPoints *points;
            container->EnumConnectionPoints(&points);
            IConnectionPoint *point;
            while (points->Next(1, &point, 0) >= 0) {
                GUID iid;
                point->GetConnectionInterface(&iid);
                if (iid == unknown_iid_7a1ac0) {
                    IEnumConnections *connections;
                    point->EnumConnections(&connections);
                    tagCONNECTDATA data;
                    while (connections->Next(1, &data, 0) >= 0) {
                        if (low) {
                            ((IUnknownHouseSink *)data.pUnk)->Power_Low();
                        } else {
                            ((IUnknownHouseSink *)data.pUnk)->Power_Restored();
                        }
                        data.pUnk->Release();
                    }
                    connections->Release();
                    break;
                }
                point->Release();
            }
            container->Release();
            points->Release();
        }
    }
}


// void HouseClass::Adjust_Drain(int amount) (0x004EC850)
// Adds to the power drain; when the house turns under- or re-powered, every
// sink of its outgoing interface is told.

void HouseClass::Adjust_Drain(int amount)
{
    bool was_low = Power < Drain;
    Drain += amount;
    unknown_func_4ec9b0();
    bool low = Power < Drain;
    if (!unknown_a40774 && was_low != low) {
        IConnectionPointContainer *container;
        QueryInterface(IID_IConnectionPointContainer, (void **)&container);
        IEnumConnectionPoints *points;
        container->EnumConnectionPoints(&points);
        IConnectionPoint *point;
        while (points->Next(1, &point, 0) >= 0) {
            GUID iid;
            point->GetConnectionInterface(&iid);
            if (iid == unknown_iid_7a1ac0) {
                IEnumConnections *connections;
                point->EnumConnections(&connections);
                tagCONNECTDATA data;
                while (connections->Next(1, &data, 0) >= 0) {
                    IUnknownHouseSink *sink = (IUnknownHouseSink *)data.pUnk;
                    if (low) {
                        sink->Power_Low();
                    } else {
                        sink->Power_Restored();
                    }
                    sink->Release();
                }
                connections->Release();
                break;
            }
            point->Release();
        }
        container->Release();
        points->Release();
    }
}


// void HouseClass::unknown_func_4ec9b0() (0x004EC9B0)
// Redraws (slot 71) every building whose type sets +0xC68 and whose +0x1A8
// mask includes the player's house type.

// Never expanded inline: Adjust_Power and Adjust_Drain, its only callers, call it.
#pragma auto_inline(off)
void HouseClass::unknown_func_4ec9b0()
{
    int count = unknown_objects_a40c60.Count();
    if (count != 0) {
        for (int index = 0; index < count; index++) {
            UnknownBuildingView *building = (UnknownBuildingView *)unknown_objects_a40c60[index];
            if (building != 0 && building->Kind_Of() == 6 && !building->unknown_372 &&
                ((UnknownBuildingTypeView *)building->Type)->unknown_c68 > 0 &&
                (building->unknown_1a8 & (1 << unknown_player_a35db4->Type->House)) != 0) {
                building->Mark(2);
            }
        }
    }
}
#pragma auto_inline(on)


// Cell HouseClass::unknown_func_4eca30(int, int) (0x004ECA30)
// Returns cell 0, 0 (no caller references it; its arguments are unused).

Cell HouseClass::unknown_func_4eca30(int, int)
{
    return Cell(0, 0);
}


// Not exact (RA2_NON_MATCHING): HouseClass::Random_Cell_In_Zone at 0x004ECA50, best score 54.00, 2026-11-13; differs: 345 instructions in the original, 355 in the draft (shape 94.00)
#ifdef RA2_NON_MATCHING
// Cell HouseClass::Random_Cell_In_Zone(int zone) (0x004ECA50)
// RA's Random_Cell_In_Zone: a random cell of a base zone. Core zone 0 is
// scattered around the base centre; zones 1 to 4 pick a random direction in
// their quadrant (north-east, south-east, south-west, north-west) and a
// distance of one to two radii (+0x53C8, between 0x300 and 0x800 leptons)
// from the base centre. A cell off the map is replaced by the nearest valid
// one; a zone without a coordinate falls back to the core zone.
#include "shared/bullet.hpp"
#include "shared/dir_type.hpp"
#include "shared/building_light.hpp"

#define LESS_MAX(a, b) (((a) < (b)) ? (b) : (a))
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
static inline DirType Rotate_Back_One_Facing(DirType const &dir)
{
    int fraction = dir.Facing & 0x1FFF;
    DirType sector(dir);
    DirType back(sector.Snap_To_8().Raw - 1);
    return DirType(back.Snap_To_8().Facing ^ fraction);
}
inline UnknownVector3 Move_Coord(CoordStruct const &start, DirType const &dir, int distance)
{
    double radians = (dir.Facing - 0x3FFF) * unknown_facing_radians_79b620;
    int z = start.Z;
    int y = (int)(start.Y - (double)Fast_Sin(radians) * distance);
    int x = (int)(start.X + (double)Fast_Cos(radians) * distance);
    UnknownVector3 result(x, y, z);
    return result;
}

Cell HouseClass::Random_Cell_In_Zone(int zone)
{
    int radius = LESS_MAX(MIN(unknown_53c8, 0x800), 0x300);
    int maxdist = radius * 2;
    UnknownVector3 coord;
    switch (zone) {
    case 0:
        coord = (UnknownVector3 const &)Coord_Scatter(BaseCenter, Random_Pick(0, radius), true);
        break;
    case 1: {
        int distance = Random_Pick(radius, maxdist);
        DirType dir(unknown_id_source_a3d290->Random()());
        dir.Raw &= ~0xC000;
        coord = Move_Coord(BaseCenter, Rotate_Back_One_Facing(dir), distance);
        break;
    }
    case 2: {
        int distance = Random_Pick(radius, maxdist);
        DirType dir(unknown_id_source_a3d290->Random()());
        dir.Raw &= ~0x8000;
        dir.Raw |= 0x4000;
        coord = Move_Coord(BaseCenter, Rotate_Back_One_Facing(dir), distance);
        break;
    }
    case 3: {
        int distance = Random_Pick(radius, maxdist);
        DirType dir(unknown_id_source_a3d290->Random()());
        dir.Raw &= ~0x4000;
        dir.Raw |= 0x8000;
        coord = Move_Coord(BaseCenter, Rotate_Back_One_Facing(dir), distance);
        break;
    }
    case 4: {
        int distance = Random_Pick(radius, maxdist);
        DirType dir(unknown_id_source_a3d290->Random()());
        dir.Raw |= 0xC000;
        coord = Move_Coord(BaseCenter, Rotate_Back_One_Facing(dir), distance);
        break;
    }
    default:
        (CoordStruct &)coord = unknown_coord_a40f48;
        break;
    }
    Cell cell;
    if ((CoordStruct const &)coord == unknown_coord_a40f48) {
        if (zone == 0) {
            cell = Coord_Cell(BaseCenter);
        } else {
            cell = Random_Cell_In_Zone(0);
        }
    } else {
        cell = Cell(coord.X / 256, coord.Y / 256);
    }
    if (!Map.Unknown_0055A660(cell, true)) {
        cell = Map.Clip_To_Map(cell);
    }
    return cell;
}
#endif

// void HouseClass::Make_Ally(ObjectClass *object, bool announce) (0x004ECF50)

void HouseClass::Make_Ally(ObjectClass *object, bool announce)
{
    if (object != 0) {
        Make_Ally(object->Owning_House(), announce);
    }
}


// void HouseClass::Make_Enemy(ObjectClass *object, bool announce) (0x004ECF80)

void HouseClass::Make_Enemy(ObjectClass *object, bool announce)
{
    if (object != 0) {
        Make_Enemy(object->Owning_House(), announce);
    }
}


// void HouseClass::unknown_func_4ecfb0(CoordStruct const &coord) (0x004ECFB0)
// Adds a waypoint at a coordinate to the current path (+0x154) unless
// another path already has a waypoint in that cell (the test of
// 0x004ED140, written out here).

void HouseClass::unknown_func_4ecfb0(CoordStruct const &coord)
{
    Cell cell = Coord_Cell(coord);
    for (int index = 0; index < 12; index++) {
        if (WaypointPaths[index] == 0) {
            WaypointPaths[index] = new WaypointPathClass(index);
        }
        for (int point = 0; point < WaypointPaths[index]->Waypoints.Count(); point++) {
            WaypointClass *waypoint = WaypointPaths[index]->Get_Waypoint(point);
            if (Coord_Cell(waypoint->Coord) == cell && index != unknown_154) {
                return;
            }
        }
    }
    Make_Waypoint_Path(unknown_154);
    WaypointPaths[unknown_154]->Add_Waypoint(coord);
}


// void HouseClass::Add_Waypoint(CoordStruct const &coord) (0x004ED0E0)
// Makes the current path (+0x154) and selects the waypoint at the
// coordinate in it (WaypointPathClass::Select_Waypoint 0x00724E30).

void HouseClass::Add_Waypoint(CoordStruct const &coord)
{
    Make_Waypoint_Path(unknown_154);
    WaypointPaths[unknown_154]->Select_Waypoint(coord);
}


// bool HouseClass::unknown_func_4ed140(Cell const &cell) (0x004ED140)
// False when a waypoint path other than the current one (+0x154) has a
// waypoint in the cell (making missing paths).

bool HouseClass::unknown_func_4ed140(Cell const &cell)
{
    for (int index = 0; index < 12; index++) {
        if (WaypointPaths[index] == 0) {
            WaypointPaths[index] = new WaypointPathClass(index);
        }
        for (int point = 0; point < WaypointPaths[index]->Waypoints.Count(); point++) {
            WaypointClass *waypoint = WaypointPaths[index]->Get_Waypoint(point);
            if (Coord_Cell(waypoint->Coord) == cell && index != unknown_154) {
                return false;
            }
        }
    }
    return true;
}


// WaypointClass *HouseClass::Waypoint_At(Cell const &cell) (0x004ED200)
// The waypoint of any path in the cell, else 0 (making missing
// paths).

WaypointClass *HouseClass::Waypoint_At(Cell const &cell)
{
    for (int index = 0; index < 12; index++) {
        if (WaypointPaths[index] == 0) {
            WaypointPaths[index] = new WaypointPathClass(index);
        }
        for (int point = 0; point < WaypointPaths[index]->Waypoints.Count(); point++) {
            WaypointClass *waypoint = WaypointPaths[index]->Get_Waypoint(point);
            if (Coord_Cell(waypoint->Coord) == cell) {
                return waypoint;
            }
        }
    }
    return 0;
}


// bool HouseClass::Fetch_Waypoint_Data(WaypointClass *waypoint, int *path, char *position) (0x004ED2B0)
// Finds a waypoint in the house's waypoint paths (making missing paths).

bool HouseClass::Fetch_Waypoint_Data(WaypointClass *waypoint, int *path, char *position)
{
    for (int index = 0; index < 12; index++) {
        if (WaypointPaths[index] == 0) {
            WaypointPaths[index] = new WaypointPathClass(index);
        }
        for (int point = 0; point < WaypointPaths[index]->Waypoints.Count(); point++) {
            if (WaypointPaths[index]->Get_Waypoint(point) == waypoint) {
                *path = index;
                *position = (char)point;
                return true;
            }
        }
    }
    *path = -1;
    *position = 0;
    return false;
}


// HRESULT __stdcall HouseClass::EnumConnectionPoints(IEnumConnectionPoints **points) (0x004ED340)
// A new enumerator over the house's connection points
// (IConnectionPointContainer).


// The connection point enumerator (0x24 bytes; constructor 0x00492E20).
class UnknownEnumConnectionPoints : public IUnknown {
public:
    UnknownEnumConnectionPoints(DynamicVectorClass<IConnectionPoint *> &points);   // 0x00492E20
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    char unknown_04[0x24 - 4];
};

HRESULT __stdcall HouseClass::EnumConnectionPoints(IEnumConnectionPoints **points)
{
    if (points == 0) {
        return E_POINTER;
    }
    UnknownEnumConnectionPoints *enumerator = new UnknownEnumConnectionPoints(ConnectionPoints);
    if (enumerator == 0) {
        return E_OUTOFMEMORY;
    }
    enumerator->AddRef();
    *points = (IEnumConnectionPoints *)enumerator;
    return S_OK;
}


// HRESULT __stdcall HouseClass::FindConnectionPoint(GUID const &iid, IConnectionPoint **point) (0x004ED3A0)
// The connection point for an outgoing interface (IConnectionPointContainer).

HRESULT __stdcall HouseClass::FindConnectionPoint(GUID const &iid, IConnectionPoint **point)
{
    if (point == 0) {
        return E_POINTER;
    }
    *point = 0;
    for (int index = 0; index < ConnectionPoints.Count(); index++) {
        IConnectionPoint *connection = ConnectionPoints[index];
        GUID id;
        connection->GetConnectionInterface(&id);
        if (id == iid) {
            *point = connection;
            connection->AddRef();
            return S_OK;
        }
    }
    return CONNECT_E_CANNOTCONNECT;
}


// void HouseClass::Unknown_004ED440(TechnoClass *techno, bool sold) (0x004ED440)
// An object of the house leaves the map (TechnoClass slot 52): the sidebar
// strip is refreshed and its type uncounted by kind; a building also gives
// back its type's Storage and either pays out its stored tiberium (sold: the
// value times the house type's factor to the balance, five per unit to
// +0x5418) or takes it off the house's tiberium.

void HouseClass::Unknown_004ED440(TechnoClass *techno, bool sold)
{
    Map.Unknown_0067C150(techno);
    switch (techno->Kind_Of()) {
    case 6:
        unknown_5460.Decrement(techno->Techno_Type_Class()->Fetch_Heap_ID());
        if (techno != 0) {
            BuildingClass *building = (BuildingClass *)techno;
            unknown_5668 = true;
            Capacity -= building->Type->Storage;
            if (sold) {
                StorageClass &storage = building->unknown_294;
                int type = storage.First_Used_Slot();
                while (type != -1) {
                    int amount = (int)storage.Remove_Amount(2147483648.0f, type);
                    unknown_5418 += amount * 5.0f;
                    Balance += unknown_TiberiumClass_list_00ac0b30[type]->Value * Type->unknown_0x148 * amount;
                    type = storage.First_Used_Slot();
                }
            } else {
                Tiberium -= building->unknown_294;
            }
        }
        break;
    case 2:
        unknown_5490.Decrement(techno->Techno_Type_Class()->Fetch_Heap_ID());
        break;
    case 15:
        unknown_5480.Decrement(techno->Techno_Type_Class()->Fetch_Heap_ID());
        break;
    case 1:
        unknown_5470.Decrement(techno->Techno_Type_Class()->Fetch_Heap_ID());
        break;
    }
}


// void HouseClass::Unknown_004ED610(TechnoClass *techno, bool arg2) (0x004ED610)
// An object of the house enters the map (TechnoClass slot 53): its type is
// counted by kind (buildings +0x5460, aircraft +0x5490, infantry +0x5480
// unless +0x5C1, units +0x5470); a building also adds its drain, its type's
// Storage to the capacity and its stored tiberium.

void HouseClass::Unknown_004ED610(TechnoClass *techno, bool arg2)
{
    switch (techno->Kind_Of()) {
    case 6:
        unknown_5460.Increment(techno->Techno_Type_Class()->Fetch_Heap_ID());
        if (techno != 0 && techno->Kind_Of() == 6) {
            BuildingClass *building = (BuildingClass *)techno;
            Adjust_Drain(building->unknown_449dd0());
            Capacity += building->Type->Storage;
            Tiberium += building->unknown_294;
        }
        break;
    case 2:
        unknown_5490.Increment(techno->Techno_Type_Class()->Fetch_Heap_ID());
        break;
    case 15:
        if (!((InfantryClass *)techno)->unknown_5c1) {
            unknown_5480.Increment(techno->Techno_Type_Class()->Fetch_Heap_ID());
        }
        break;
    case 1:
        unknown_5470.Increment(techno->Techno_Type_Class()->Fetch_Heap_ID());
        break;
    }
}


// HouseClass *__fastcall HouseClass::As_Pointer(int type) (0x004ED760)
// The house whose type has this index, or 0.

HouseClass *__fastcall HouseClass::As_Pointer(int type)
{
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *house = Houses[index];
        if (house->Type->House == type) {
            return house;
        }
    }
    return 0;
}


// void HouseClass::Compute_CRC(WWCRCEngine &crc) const (0x004ED790)
// Checksums the abstract part and the house's counters, doubles, flags and timers, then the base plan.

void HouseClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc((long)unknown_c8);
    crc((long)Type->House);
    crc(unknown_d0);
    crc(unknown_d8);
    crc(unknown_e0);
    crc(unknown_e8);
    crc(unknown_f0);
    crc(unknown_f8);
    crc(unknown_100);
    crc(unknown_108);
    crc(unknown_110);
    crc(unknown_134);
    crc(unknown_136);
    crc(unknown_137);
    crc(unknown_13a);
    crc(unknown_13d);
    crc((long)unknown_194);
    crc((long)unknown_198);
    crc((long)unknown_1c4);
    crc(unknown_1c8.Value());
    crc(unknown_1d4.Value());
    crc(unknown_1e0.Value());
    crc((long)unknown_228);
    crc((long)CurBuildings);
    crc((long)unknown_234);
    crc((long)unknown_238);
    crc((long)Balance);
    crc((long)Capacity);
    crc((long)unknown_52b8);
    crc((long)unknown_52bc);
    crc((long)unknown_52c0);
    crc((long)unknown_52c8);
    crc((long)unknown_52c4);
    crc((long)unknown_52cc);
    crc((long)Power);
    crc((long)Drain);
    crc((long)unknown_53b8);
    crc((long)unknown_54f0);
    unknown_55f0.Compute_CRC(crc);
}


// HRESULT __stdcall HouseClass::Load(IStream *stream) (0x004EDA60, slot 5)
// The counterpart of Save 0x004EE440: empties the lists (releasing the
// connection points and deleting the super weapons), restores the house in
// place, loads the base plan and counters, fixes up the pointers, reads the
// lists back (count, then elements) and finally the +0x56CC object.
// The scout swizzle loop fixes up the last node's house each pass, as the
// original does. The body after the base Load is the SUCCEEDED block with
// one shared count (as in BuildingClass::Load): count's scope then covers
// the placement new, whose NoInitClass temporary holds the dead this slot,
// so count takes the stream slot and each list element the this slot.

extern "C" GUID const unknown_iid_7a2030;   // 0x007A2030 the +0x56CC object's interface

// Reads a list saved by SAVE_LIST: its count, then each element; returns a failure.

HRESULT __stdcall HouseClass::Load(IStream *stream)
{
    unknown_50.Clear();
    Buildings.Clear();
    unknown_80.Clear();
    unknown_98.Clear();
    unknown_b0.Clear();
    for (int point = 0; point < ConnectionPoints.Count(); point++) {
        while (ConnectionPoints[point]->Release() > 0) {
        }
        ConnectionPoints[point] = 0;
    }
    ConnectionPoints.Clear();
    while (Supers.Count()) {
        delete Supers[0];
        Supers.Delete(0);
    }

    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        int count;
        new (this) HouseClass(NoInitClass());
        unknown_55f0.Load(stream);
        unknown_5420.Load(stream);
        unknown_5430.Load(stream);
        unknown_5440.Load(stream);
        unknown_5450.Load(stream);
        unknown_5460.Load(stream);
        unknown_5470.Load(stream);
        unknown_5480.Load(stream);
        unknown_5490.Load(stream);
        unknown_54a0.Load(stream);
        unknown_54b0.Load(stream);
        unknown_54c0.Load(stream);
        unknown_54d0.Load(stream);

        Swizzler.Swizzle((void **)&Type);
        Swizzler.Swizzle((void **)&unknown_52dc);
        Swizzler.Swizzle((void **)&unknown_52e0);
        Swizzler.Swizzle((void **)&unknown_52d8);
        Swizzler.Swizzle((void **)&unknown_52e4);
        Swizzler.Swizzle((void **)&unknown_52e8);
        Swizzler.Swizzle((void **)&unknown_52ec);
        Swizzler.Swizzle((void **)&unknown_52f0);
        Swizzler.Swizzle((void **)&unknown_52f4);
        Swizzler.Swizzle((void **)&unknown_52f8);
        Swizzler.Swizzle((void **)&unknown_5410);
        int index;
        for (index = 0; index < 12; index++) {
            Swizzler.Swizzle((void **)&WaypointPaths[index]);
        }
        for (index = 0; index < 3; index++) {
            unknown_5560[index].unknown_func_4a6b10();
        }

        LOAD_LIST(stream, Supers, SuperClass *);
        LOAD_LIST(stream, unknown_38, TagClass *);
        {
            count = 0;
            result = stream->Read(&count, sizeof(count), 0);
            if (FAILED(result)) {
                return result;
            }
            while (count--) {
                AngerStruct anger;
                result = stream->Read(&anger, sizeof(anger), 0);
                if (FAILED(result)) {
                    return result;
                }
                AngerNodes.Add(anger);
            }
        }
        for (index = 0; index < AngerNodes.Count(); index++) {
            Swizzler.Swizzle((void **)&AngerNodes[index].House);
        }
        {
            count = 0;
            result = stream->Read(&count, sizeof(count), 0);
            if (FAILED(result)) {
                return result;
            }
            while (count--) {
                ScoutStruct scout;
                result = stream->Read(&scout, sizeof(scout), 0);
                if (FAILED(result)) {
                    return result;
                }
                ScoutNodes.Add(scout);
            }
        }
        for (index = 0; index < ScoutNodes.Count(); index++) {
            Swizzler.Swizzle((void **)&ScoutNodes[ScoutNodes.Count() - 1].House);
        }
        LOAD_LIST(stream, unknown_50, BuildingClass *);
        LOAD_LIST(stream, Buildings, BuildingClass *);
        LOAD_LIST(stream, unknown_80, BuildingClass *);
        LOAD_LIST(stream, unknown_98, BuildingClass *);
        LOAD_LIST(stream, unknown_b0, BuildingClass *);

        for (index = 0; index < Supers.Count(); index++) {
            Swizzler.Swizzle((void **)&Supers[index]);
        }
        for (index = 0; index < unknown_38.Count(); index++) {
            Swizzler.Swizzle((void **)&unknown_38[index]);
        }
        for (index = 0; index < unknown_50.Count(); index++) {
            Swizzler.Swizzle((void **)&unknown_50[index]);
        }
        for (index = 0; index < Buildings.Count(); index++) {
            Swizzler.Swizzle((void **)&Buildings[index]);
        }
        for (index = 0; index < unknown_80.Count(); index++) {
            Swizzler.Swizzle((void **)&unknown_80[index]);
        }
        for (index = 0; index < unknown_98.Count(); index++) {
            Swizzler.Swizzle((void **)&unknown_98[index]);
        }
        for (index = 0; index < unknown_b0.Count(); index++) {
            Swizzler.Swizzle((void **)&unknown_b0[index]);
        }

        BOOL present = 0;
        result = stream->Read(&present, sizeof(present), 0);
        if (SUCCEEDED(result) && present) {
            result = OleLoadFromStream(stream, unknown_iid_7a2030, (void **)&unknown_56cc);
        }
    }
    return result;
}


// HRESULT __stdcall HouseClass::Save(IStream *stream, BOOL clear_dirty) (0x004EE440)
// Writes the abstract part, the base plan, the counters, the lists (count
// then elements) and the +0x56CC object (through OleSaveToStream).

extern "C" __declspec(dllimport) HRESULT __stdcall OleSaveToStream(IPersistStream *persist, IStream *stream);
extern "C" GUID const IID_IPersistStream_7d5f38;    // 0x007D5F38 (__uuidof(IPersistStream))

// An IPersistStream smart pointer queried from an IUnknown (comip.h
// _com_ptr_t semantics: a failed query other than E_NOINTERFACE raises).
class PersistStreamPtr {
public:
    PersistStreamPtr(IUnknown *unknown) : Interface(0)
    {
        if (unknown != 0) {
            IPersistStream *persist;
            HRESULT result = unknown->QueryInterface(IID_IPersistStream_7d5f38, (void **)&persist);
            Interface = SUCCEEDED(result) ? persist : 0;
            if (FAILED(result) && result != E_NOINTERFACE) {
                _com_issue_error(result);
            }
        }
    }
    ~PersistStreamPtr()
    {
        if (Interface != 0) {
            Interface->Release();
        }
    }
    operator IPersistStream *() const { return Interface; }

    IPersistStream *Interface;
};

// Writes a list: its count, then each element; returns a failure.

HRESULT __stdcall HouseClass::Save(IStream *stream, BOOL clear_dirty)
{
    int count;
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
    unknown_55f0.Save(stream);
    unknown_5420.Save(stream);
    unknown_5430.Save(stream);
    unknown_5440.Save(stream);
    unknown_5450.Save(stream);
    unknown_5460.Save(stream);
    unknown_5470.Save(stream);
    unknown_5480.Save(stream);
    unknown_5490.Save(stream);
    unknown_54a0.Save(stream);
    unknown_54b0.Save(stream);
    unknown_54c0.Save(stream);
    unknown_54d0.Save(stream);

    SAVE_LIST(stream, Supers, SuperClass *);
    SAVE_LIST(stream, unknown_38, TagClass *);
    SAVE_LIST(stream, AngerNodes, AngerStruct);
    SAVE_LIST(stream, ScoutNodes, ScoutStruct);
    SAVE_LIST(stream, unknown_50, BuildingClass *);
    SAVE_LIST(stream, Buildings, BuildingClass *);
    SAVE_LIST(stream, unknown_80, BuildingClass *);
    SAVE_LIST(stream, unknown_98, BuildingClass *);
    SAVE_LIST(stream, unknown_b0, BuildingClass *);

    BOOL present = unknown_56cc != 0;
    result = stream->Write(&present, sizeof(present), 0);
    if (SUCCEEDED(result) && present) {
        PersistStreamPtr persist(unknown_56cc);
        result = OleSaveToStream(persist, stream);
    }
    }
    return result;
}


// HRESULT __stdcall HouseClass::GetClassID(GUID *clsid) (0x004EE850)

HRESULT __stdcall HouseClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_HouseClass;
    return S_OK;
}


// int HouseClass::Size_Of() const (0x004EE890)

int HouseClass::Size_Of() const
{
    return sizeof(HouseClass);
}


// void HouseClass::Make_Waypoint_Path(int index) (0x004EE8A0)

void HouseClass::Make_Waypoint_Path(int index)
{
    if (WaypointPaths[index] == 0) {
        WaypointPaths[index] = new WaypointPathClass(index);
    }
}


// void HouseClass::unknown_func_4ee8f0() (0x004EE8F0)
// Turns the house's firestorm on (+0x142): every FirestormWall building it
// owns switches (0x00450950) and the map rebuilds the zones of their cells.

void HouseClass::unknown_func_4ee8f0()
{
    if (!unknown_142) {
        DynamicVectorClass<Cell> cells;
        unknown_142 = true;
        for (int index = unknown_BuildingClass_list_00a40af8.Count() - 1; index >= 0; index--) {
            BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
            if (building->Type->FirestormWall && building->House == this) {
                building->Update_Firestorm_Wall_00450950();
                cells.Add(Coord_Cell(building->Center_Coord()));
            }
        }
        Map.Zone_Reset();
        Map.Unknown_005688A0(cells);
    }
}


// void HouseClass::unknown_func_4eea40() (0x004EEA40)
// Turns the house's firestorm off (+0x142): every FirestormWall building it
// owns switches (0x00450950) and the map rebuilds the zones of their cells.

void HouseClass::unknown_func_4eea40()
{
    if (unknown_142) {
        unknown_142 = false;
        DynamicVectorClass<Cell> cells;
        for (int index = unknown_BuildingClass_list_00a40af8.Count() - 1; index >= 0; index--) {
            BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
            if (building->Type->FirestormWall && building->House == this) {
                building->Update_Firestorm_Wall_00450950();
                cells.Add(Coord_Cell(building->Center_Coord()));
            }
        }
        Map.Zone_Reset();
        Map.Unknown_005688A0(cells);
    }
}


class Leaf_4eeb90 { public: void get(); };
void Leaf_4eeb90::get() {  }


// void HouseClass::unknown_func_4eeba0(int amount, HouseClass *house) (0x004EEBA0)
// Adds to the anger at a house, then picks the house it is angriest at
// among the remaining non-allied houses (+0x54F0: its index, -1 for none).

void HouseClass::unknown_func_4eeba0(int amount, HouseClass *house)
{
    for (int index = 0; index < AngerNodes.Count(); index++) {
        if (AngerNodes[index].House == house) {
            AngerNodes[index].Value += amount;
        }
    }
    int best = 0;
    HouseClass *angriest = 0;
    for (index = 0; index < AngerNodes.Count(); index++) {
        int value = AngerNodes[index].Value;
        HouseClass *other = AngerNodes[index].House;
        if (value > best && !other->unknown_13d && !Is_Ally(other)) {
            best = value;
            angriest = other;
        }
    }
    if (angriest != 0) {
        unknown_54f0 = angriest->ArrayIndex;
    } else {
        unknown_54f0 = -1;
    }
}


// 0x004eec70: function not yet identified
//
// Body: 54 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void __thiscall Mark_Scouted_004eec70(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004eec70 {

struct Owner_004eec70 { void Mark_Scouted_004eec70(int id); };

void Owner_004eec70::Mark_Scouted_004eec70(int id)
{
    int self = (int)this;
    int index;

    index = (int)(0);
    if (0 < *(int *)(self + 0x551c)) {
        do {
            if (*(int *)(*(int *)(self + 0x5510) + index * 8) == id) {
                *(unsigned char *)(*(int *)(self + 0x5510) + index * 8 + 4) = 1;
            }
            index = (int)(index + 1);
        } while (index < *(int *)(self + 0x551c));
    }
    return;
}

}  // namespace seed_004eec70


// void HouseClass::Plan_Naval_Yard_Path_004eecb0(int node) (0x004EECB0)
// The naval yard node `node` of the base plan: needs a large water zone, picks the shore cell nearest
// the base by path for a test harvester, then puts BuildDummy nodes every third step of the path
// before the yard node and gives the yard node that cell. Names are hypotheses (no caller, RA2 only).

void HouseClass::Plan_Naval_Yard_Path_004eecb0(int node)
{
    Cell center(BaseCenter);
    int zone = Map.Zone(center, 0, false);
    Cell start = Map.Nearby_Location(center, 0, zone, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
    int water_zone = -1;
    if (Map.Unknown_005658C0(water_zone) < 3000) {
        unknown_55f0.Nodes.Delete(node);
        unknown_138 = false;
        return;
    }

    int width = 1;
    int height = 1;
    for (int i = 0; i < Rules->BuildNavalYard.Count(); i++) {
        if (Rules->BuildNavalYard[i]->AIBasePlanningSide == Type->Side || Rules->BuildNavalYard[i]->AIBasePlanningSide == -1) {
            width = Rules->BuildNavalYard[i]->Width() + 1;
            height = Rules->BuildNavalYard[i]->Foundation_Height(false) + 1;
            break;
        }
    }

    DynamicVectorClass<Cell> cells;
    Map.Unknown_00565A50(center, 25, 30, 10, water_zone, cells, width, height);
    if (cells.Count() == 0) {
        unknown_55f0.Nodes.Delete(node);
        unknown_138 = false;
        return;
    }

    IndexClass<int, Cell> sorted;
    for (int c = 0; c < cells.Count(); c++) {
        Cell spot = Map.Nearby_Location(cells[c], 0, zone, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
        int distance = Search.Test_Cell_Walk(center, spot, 0, false, false, 0);
        sorted.Add_Index(cells.Count() * distance + c, cells[c]);
    }
    cells.Clear();
    for (int s = 0; s < sorted.Count(); s++) {
        cells.Add(sorted.Fetch_By_Position(s));
    }

    bool found = false;
    PathStruct *path = 0;
    Cell target = unknown_cell_a40f10;
    Cell spot = unknown_cell_a40f10;
    int moves[2048];
    UnitClass *unit = new UnitClass(UnitTypes[UnitTypeClass::From_Name("HARV")], this);
    unit->Unlimbo(&UnknownCoord434820(start.X * 256 + 128, start.Y * 256 + 128, 0), 0);
    while (!found && cells.Count() > 0) {
        target = cells[0];
        spot = Map.Nearby_Location(cells[0], 0, zone, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
        if (spot != unknown_cell_a40f10) {
            path = Search.Find_Path(start, spot, unit, moves, -1, 0, 0);
        } else {
            path = 0;
        }
        if (path != 0 && path->Length <= 25) {
            found = true;
        } else {
            cells.Delete(0);
        }
    }
    unit->Limbo();
    delete unit;
    if (!found) {
        unknown_55f0.Nodes.Delete(node);
        unknown_138 = false;
        return;
    }

    int bit = 1 << ArrayIndex;
    Cell cell = start;
    int first = 0;
    Cell last = start;
    for (int m = 0; m < path->Length; m++) {
        cell = AdjacentCell[moves[m] & 7] + cell;
        if (Map[cell].unknown_dc & bit) {
            first = m + 1;
            last = cell;
        }
    }

    cell = last;
    int counter = 2;
    int at = node - 1;
    int type = -1;
    for (int d = 0; d < Rules->BuildDummy.Count(); d++) {
        if (Rules->BuildDummy[d]->AIBasePlanningSide == Type->Side || Rules->BuildDummy[d]->AIBasePlanningSide == -1) {
            type = Rules->BuildDummy[d]->ArrayIndex;
            break;
        }
    }
    for (int step = first; step < path->Length; step++) {
        if (counter == 0) {
            counter = 3;
            unknown_55f0.Nodes.Insert_After(BaseNodeClass(type, cell, false, 0), at);
            at++;
        } else {
            counter--;
        }
        cell = cell + AdjacentCell[moves[step] & 7];
    }
    unknown_55f0.Nodes[at + 1].Cell = target;
}


// void HouseClass::Unknown_004EF5A0() (0x004EF5A0)
// After the MCV deploys (UnitClass 0x006FDFC0): a computer house in a
// multiplayer game runs 0x004EC5E0; a house without base nodes plans its
// base (0x004EF890) with unknown_a40774 cleared.

void HouseClass::Unknown_004EF5A0()
{
    if (!Is_Human_Control() && unknown_game_mode_a3d298 != 0) {
        unknown_func_4ec5e0();
    }
    if (unknown_55f0.Nodes.Count() == 0) {
        int saved = unknown_a40774;
        unknown_a40774 = 0;
        unknown_func_4ef890();
        unknown_a40774 = saved;
    }
}


// BuildingTypeClass *HouseClass::unknown_func_4ef600(DynamicVectorClass<BuildingTypeClass *> const &types) (0x004EF600)
// The first building type of the list that this house may build: its owner
// set holds the parent country's house index, its required/forbidden house
// sets allow this house type, its side matches (or is -1) and its super
// weapon, when it has one, is not disableable (always accepted in the session
// flag 0x00A3D2C3).

BuildingTypeClass *HouseClass::unknown_func_4ef600(DynamicVectorClass<BuildingTypeClass *> const &types)
{
    int owner_bit = 1 << HouseTypeClass::Find_Index(Type->ParentCountry);
    for (int index = 0; index < types.Count(); index++) {
        BuildingTypeClass *building = types[index];
        if ((building->Owner & owner_bit) != 0
            && (building->RequiredHouses == -1 || (building->RequiredHouses & (1 << Type->House)) != 0)
            && (building->ForbiddenHouses == -1 || (building->ForbiddenHouses & (1 << Type->House)) == 0)
            && (building->AIBasePlanningSide == -1 || building->AIBasePlanningSide == Type->Side)) {
            if (unknown_session_a3d2c3 || building->SuperWeapon == -1 || !Supers[building->SuperWeapon]->Type->DisableableFromShell) {
                return building;
            }
        }
    }
    return 0;
}


// UnitTypeClass *HouseClass::Unknown_004EF6F0(TypeList<UnitTypeClass *> const &list) (0x004EF6F0)
// The first unit type of the list this house's country may own (its owner bit
// is the index of the parent country's house type), else 0 (AI expands it for
// Rules HarvesterUnit; Create_Units 0x00660320 calls it for BaseUnit).

UnitTypeClass *HouseClass::Unknown_004EF6F0(TypeList<UnitTypeClass *> const &list)
{
    int owner = 1 << HouseTypeClass::Find_Index(Type->ParentCountry);
    for (int index = 0; index < list.Count(); index++) {
        if (list[index]->Owner & owner) {
            return list[index];
        }
    }
    return 0;
}


// 0x004EF740: checks each prerequisite against available building types.
// Original code 004EF740..004EF869; alignment through004EF86C;
// six switch targets end at004EF884. Full strict audit still required.

bool HouseClass::unknown_func_4ef740(BuildingTypeClass *type,
    DynamicVectorClass<BuildingTypeClass const *> &available, int count)
{
    int prerequisite = 0;
    if (type->Prerequisite.Count() > 0) {
        do {
            int index = type->Prerequisite[prerequisite];
            if (index >= 0) {
                int item = 0;
                if (count <= 0) return false;
                while (true) {
                    if (available[item] == BuildingTypes[index]) break;
                    ++item;
                    if (item >= count) return false;
                }
            } else {
                TypeList<BuildingTypeClass *> *group = 0;
                switch (index) {
                case -1: group = &Rules->BuildPower; break;
                case -2: group = &Rules->BuildWeapons; break;
                case -3: group = &Rules->BuildBarracks; break;
                case -4: group = &Rules->BuildRadar; break;
                case -5: group = &Rules->BuildTech; break;
                case -6: group = &Rules->BuildRefinery; break;
                }
                int member = 0;
                if (group->Count() > 0) {
                    do {
                        BuildingTypeClass *candidate = group->Fetch(member);
                        int item = 0;
                        if (count > 0) {
                            do {
                                if (available[item] == candidate) goto found;
                                ++item;
                            } while (item < count);
                        }
                        ++member;
                    } while (member < group->Count());
                }
                return false;
            }
found:
            ++prerequisite;
        } while (prerequisite < type->Prerequisite.Count());
    }
    return true;
}


// House base-plan construction, original004EF890;2545 body+15 NOP.
// Negative queue pointers represent the original special base-node kinds.
void HouseClass::unknown_func_4ef890()
{
    bool soviet=unknown_130==1;
    unknown_55f0.Init();
    int owner=1 << HouseTypeClass::Find_Index(Type->ParentCountry);
    DynamicVectorClass<BuildingTypeClass const *> eligible;
    DynamicVectorClass<bool> included;
    int i;
    for (i=0;i<BuildingTypes.Count();++i) {
        BuildingTypeClass const *type=BuildingTypes[i];
        if ((type->Owner & owner) && (type->AIBasePlanningSide==Type->Side || type->AIBasePlanningSide==-1)
            && type->AIBuildThis && type->TechLevel<=unknown_11c
            && (type->RequiredHouses==-1 || (type->RequiredHouses & (1 << Type->House)))
            && (type->ForbiddenHouses==-1 || !(type->ForbiddenHouses & (1 << Type->House)))
            && (unknown_session_a3d2c3 || type->SuperWeapon==-1 || !Supers[type->SuperWeapon]->Type->DisableableFromShell)) {
            eligible.Add(type);
            included.Add(false);
        }
    }
    int eligible_count=eligible.Count();
    DynamicVectorClass<BuildingTypeClass const *> ordered;
    BuildingTypeClass const *yard=unknown_func_4ef600(Rules->unknown_7c8);
    for (i=0;i<eligible_count;++i) {
        if (yard==eligible[i]) { included[i]=true; ordered.Add(yard); break; }
    }
    ordered.Add(unknown_func_4ef600(Rules->BuildPower));
    BuildingTypeClass const *barracks=unknown_func_4ef600(Rules->BuildBarracks);
    for (i=0;i<eligible.Count();++i) {
        if (eligible[i]==barracks) {
            BuildingTypeClass const *first=eligible[0]; eligible[0]=barracks;
            included[i]=included[0]; included[0]=false; eligible[i]=first; break;
        }
    }
    BuildingTypeClass const *weapons=unknown_func_4ef600(Rules->BuildWeapons);
    for (i=0;i<eligible.Count();++i) {
        if (eligible[i]==weapons) {
            BuildingTypeClass const *second=eligible[1]; eligible[1]=weapons;
            included[i]=included[1]; included[1]=false; eligible[i]=second; break;
        }
    }
    int remaining=eligible_count-1;
    while (remaining>0) {
        int plug=-1;
        bool added=false;
        int queued=ordered.Count();
        for (i=0;i<eligible_count;++i) {
            if (included[i]) continue;
            BuildingTypeClass const *type=eligible[i];
            if (_stricmp(type->ID,"GAPLUG")==0) { plug=i; continue; }
            if (unknown_func_4ef740((BuildingTypeClass *)type,ordered,queued)) {
                included[i]=true; ordered.Add(type); added=true; --remaining;
            }
        }
        if (!added) {
            if (plug==-1) break;
            included[plug]=true; ordered.Add(eligible[plug]); --remaining;
        }
    }
    int extra_refineries=2-unknown_c8;
    BuildingTypeClass const *refinery=unknown_func_4ef600(Rules->BuildRefinery);
    int refinery_position=0;
    for (i=0;i<ordered.Count()-1;++i) {
        if (ordered[i]==refinery) {
            if (refinery_position!=-1 && extra_refineries>0) {
                for (int n=0;n<extra_refineries;++n) ordered.Insert_After(refinery,Random_Pick(refinery_position,ordered.Count()-1));
            }
            break;
        }
        ++refinery_position;
    }
    DynamicVectorClass<BuildingTypeClass const *> final_queue;
    final_queue.Add(ordered[0]); final_queue.Add(ordered[1]); final_queue.Add(ordered[2]);
    for (i=3;i<ordered.Count();++i) final_queue.Add(ordered[i]);
    int defenses=soviet ? Rules->SovietBaseDefenseCounts[unknown_c8] : Rules->AlliedBaseDefenseCounts[unknown_c8];
    for (;defenses>0;--defenses) final_queue.Insert_After((BuildingTypeClass const *)-1,Random_Pick(3,final_queue.Count()-1));
    for (i=0;i<final_queue.Count();++i) {
        BuildingTypeClass const *type=final_queue[i];
        if ((int)type<0 && (int)type>=-4) unknown_55f0.Nodes.Add(BaseNodeClass((int)type,Cell(0,0),false,0));
        else unknown_55f0.Nodes.Add(BaseNodeClass(type->ArrayIndex,Cell(0,0),false,0));
    }
}


// int __fastcall House_Base_Distance_Score(HouseClass *, Cell const &, int, int) (0x004F0290)
// A placement score callback that HouseClass 0x004F03C0 calls for each
// candidate cell (BuildingClass 0x00440520 passes it): the score plus 1000
// per cell of the larger axis distance from the base plan's cell (+0x50 of
// the house's BaseClass). The fourth argument is unused. Name hypothesis.

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

int __fastcall House_Base_Distance_Score(HouseClass *house, Cell const &cell, int score, int)
{
    int dx = cell.X - house->unknown_55f0.unknown_50.X;
    int dy = cell.Y - house->unknown_55f0.unknown_50.Y;
    return score + MAX(abs(dx), abs(dy)) * 1000;
}


// int __fastcall House_Facing_Score(HouseClass *, Cell const &, int, int) (0x004F02E0)
// A placement score callback that HouseClass 0x004F03C0 calls (0x004F1190
// passes it): the score plus 1000 per point of the cell's value (+0x15F44,
// over the base plan's area) times 128, plus, for a facing other than -1,
// how far the direction from the base plan's cell is from that 8-way facing
// (0..128 of 256). Name hypothesis.


inline UnknownFacingDir Offset_Direction(int x, int y)
{
    return UnknownFacingDir((unsigned short)((Fast_Atan2(y, x) - 1.5707963267948966) * -10430.060040584269));
}

int __fastcall House_Facing_Score(HouseClass *house, Cell const &cell, int score, int facing)
{
    BaseClass &base = house->unknown_55f0;
    int value = house->unknown_15f44[cell.X - base.unknown_54.X + (cell.Y - base.unknown_54.Y) * base.unknown_54.Width];
    int diff = 0;
    if (facing != -1) {
        Point2D d;
        d.X = cell.X - base.unknown_50.X;
        d.Y = cell.Y - base.unknown_50.Y;
        UnknownFacingDir dir = Offset_Direction(d.X, -d.Y);
        diff = abs(facing * 32 - Dir_To_256(dir));
        if (diff >= 128) {
            diff = 255 - diff;
        }
    }
    return score + (value * 128 + diff) * 1000;
}


// HouseClass::Where_To_Place_Building, original 0x004F03C0, 2,629 B body (TibSun twin). Facing_Offset, Coord_Span and
// Coord_Span2 stay file-local until the real table element and distance helpers are identified.
struct Facing_Offset : Cell {
    Facing_Offset(Cell const &cell) : Cell(cell) {}
    ~Facing_Offset() {}
};
inline int Coord_Span(CoordStruct const &a, CoordStruct const &b) { return ((UnknownVector3 const &)(a - b)).Length(); }
inline int Coord_Span2(CoordStruct const &a, CoordStruct const &b)
{
    CoordStruct d(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    double x = d.X;
    double y = d.Y;
    double z = d.Z;
    return (int)unknown_sqrt_4ba410(z * z + y * y + x * x);
}
Cell HouseClass::Where_To_Place_Building(BuildingTypeClass *type, int (__fastcall *score)(HouseClass *, Cell const &, int, int), int context)
{
    if (type->Naval) {
        Cell no_cell(0,0);
        int width = unknown_func_4ef600(Rules->Shipyard)->Width() + 2;
        int height = unknown_func_4ef600(Rules->Shipyard)->Foundation_Height(false) + 2;
        Cell center = Coord_Cell(BaseCenter);
        Cell spot = Map.Nearby_Location(center, 5, -1, 0, false, Point2D(width, height), false, false, false, true, no_cell, 0);
        if (spot != unknown_cell_a40f10) {
            if (unknown_50[0] != 0) {
                if (Coord_Span(Map[spot].Center_Coord(), unknown_50[0]->Center_Coord()) > Rules->AINavalYardAdjacency * 256) {
                    return unknown_cell_a40f10;
                }
            }
        }
        return spot;
    } else {
    DynamicVectorClass<Cell> sorted;
    if (unknown_55f0.unknown_50 == Cell(0,0)) {
        return Coord_Cell(BaseCenter);
    }
    IndexClass<int, Cell> ranking;
    sorted.Clear();
    int i;
    for (i = 0; i < unknown_55f0.unknown_20.Count(); ++i) {
        Cell cell = unknown_55f0.unknown_20[i];
        int value;
        if (!type->CloakGenerator) {
            value = score(this, cell, i, context);
        } else {
            CoordStruct cell_coord = CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
            value = i + 1000 * Coord_Span2(BaseCenter, cell_coord);
        }
        ranking.Add_Index(value, cell);
    }
    for (i = 0; i < ranking.Count(); ++i) {
        sorted.Add(ranking.Fetch_By_Position(i));
    }
    int base_height = Map[unknown_55f0.unknown_50].unknown_11b;
    int house_mask = 1 << ArrayIndex;
    Cell base_cell;
    for (int pass = 0; pass < 2; ++pass) {
        for (i = 0; i < sorted.Count(); ++i) {
            base_cell = sorted[i];
            Cell occupied(0,0);
            for (int face = 0; face < 8; ++face) {
                CellClass *cptr = &Map[Adjacent_Cell(base_cell, (FacingType)face)];
                if (cptr->unknown_dc & house_mask) {
                    occupied = Adjacent_Cell(occupied, (FacingType)face);
                }
            }
            if (occupied == Cell(0,0)) {
                continue;
            }
            Point2D delta;
            delta.X = -(int)occupied.X;
            delta.Y = -(int)occupied.Y;
            double angle = Fast_Atan2((double)-delta.Y, (double)delta.X);
            UnknownFacingDir dir = UnknownFacingDir((unsigned short)((angle - 1.5707963267948966) * -10430.060040584269));
            int facing = Dir_To_8(dir);
            Cell offset(0,0);
            int width = type->Width();
            int height = type->Foundation_Height(false);
            bool extra = type->ProtectWithWall || type->WantsExtraSpace;
            int spacing = Rules->AIBaseSpacing;
            if (extra) ++spacing;
            if (facing >= 1 && facing <= 3) {
                offset.X = (short)spacing;
            } else if (facing >= 5 && facing <= 7) {
                offset.X = (short)(1 - width - spacing);
            }
            if (facing >= 3 && facing <= 5) {
                offset.Y = (short)(spacing + 1);
            } else if (facing >= 7 || facing <= 1) {
                offset.Y = (short)(1 - height - spacing);
            }
            for (int trial = 0; trial < 2; ++trial) {
                static Facing_Offset scan_offsets[8] = {
                    Cell(-1,0), Cell(-1,-1), Cell(0,-1), Cell(1,-1),
                    Cell(1,0), Cell(1,1), Cell(0,1), Cell(-1,1)
                };
                static Facing_Offset step_offsets[8] = {
                    Cell(1,0), Cell(1,1), Cell(0,1), Cell(-1,1),
                    Cell(-1,0), Cell(-1,-1), Cell(0,-1), Cell(1,-1)
                };
                Cell pos = base_cell + offset + scan_offsets[facing];
                if (trial == 1) {
                    pos = Adjacent_Cell(pos, facing - 4);
                    spacing -= Rules->AIBaseSpacing;
                }
                for (int attempt = 0; attempt < 3; ++attempt) {
                    Rect rect(pos.X - spacing, pos.Y - spacing, width + spacing * 2, height + spacing * 2);
                    if (Map.Unknown_005686B0(rect, ArrayIndex)) {
                        if (type->Legal_Placement(pos, this)) {
                            if (abs(base_height - Map[pos].unknown_11b) < 3 && unknown_func_4f5320(type, pos)) {
                                return pos;
                            }
                        }
                    }
                    pos.X += step_offsets[facing].X;
                    pos.Y += step_offsets[facing].Y;
                }
            }
        }
    }
    return Cell(0,0);
    }
}


// Empty atexit destructor (0x004F0E10) of HouseClass::Where_To_Place_Building's static step_offsets table, emitted by HouseClass_Where_To_Place_Building_004f03c0.cpp; no source of its own.


// Empty atexit destructor (0x004F0E20) of HouseClass::Where_To_Place_Building's static scan_offsets table, emitted by HouseClass_Where_To_Place_Building_004f03c0.cpp; no source of its own.


// Cell HouseClass::unknown_func_4f0e30(BuildingTypeClass *upgrade) (0x004F0E30)
// The building an upgrade goes on: this house's buildings of the type the
// upgrade powers up (PowersUpBuilding) that take it (0x0044D900); with
// PowersUpToLevel -1 the least upgraded one, else one without upgrades.

Cell HouseClass::unknown_func_4f0e30(BuildingTypeClass *upgrade)
{
    int index = BuildingTypeClass::From_Name(upgrade->PowersUpBuilding);
    if (index == -1) {
        return Cell(0, 0);
    }
    BuildingTypeClass *type = BuildingTypes[index];
    int best = type->Upgrades + 10;
    BuildingClass *chosen = 0;
    bool any_level = upgrade->PowersUpToLevel == -1;
    for (int at = 0; at < unknown_BuildingClass_list_00a40af8.Count(); at++) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[at];
        if (building->House == this && building->Type == type && building->unknown_44d900(upgrade, this)) {
            if ((any_level && best > building->unknown_5c6) || (!any_level && building->unknown_5c6 == 0)) {
                best = building->unknown_5c6;
                chosen = building;
            }
        }
    }
    if (chosen != 0) {
        CoordStruct coord = chosen->Coord;
        return Coord_Cell(coord);
    }
    return Cell(0, 0);
}


// Cell Clamp_Cell_004F0F70(Rect const &rect, Cell const &cell) (0x004F0F70)
// The cell moved inside the cell rectangle (each coordinate clamped to
// [X, X + Width - 1]). No caller; name, convention and owner are hypotheses.

Cell __fastcall Clamp_Cell_004F0F70(Rect const &rect, Cell const &cell)
{
    Cell result = cell;
    if (result.X < rect.X) result.X = rect.X;
    if (result.X >= rect.X + rect.Width) result.X = rect.X + rect.Width - 1;
    if (result.Y < rect.Y) result.Y = rect.Y;
    if (result.Y >= rect.Y + rect.Height) result.Y = rect.Y + rect.Height - 1;
    return result;
}


// void HouseClass::unknown_func_4f0ff0(ObjectClass const *object, int strength, int *grid, Rect const &area) (0x004F0FF0)
// Adds an object's influence to a grid covering an area of cells: each cell
// within six of the object's (by Fast_Sqrt) gains strength over
// (distance - 1) * 0.1 + 1, distances under one counting as one.

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

void HouseClass::unknown_func_4f0ff0(ObjectClass const *object, int strength, int *grid, Rect const &area)
{
    if (strength <= 0) {
        strength = 1;
    }
    CoordStruct coord = object->Coord;
    Cell cell = Coord_Cell(coord);
    int left = MAX(cell.X - 6, area.X);
    int right = MIN(area.X + area.Width, cell.X + 6);
    int top = MAX(cell.Y - 6, area.Y);
    int bottom = MIN(area.Y + area.Height, cell.Y + 6);
    for (int y = top; y < bottom; y++) {
        for (int x = left; x < right; x++) {
            int dx = x - cell.X;
            int dy = y - cell.Y;
            double distance = Fast_Sqrt(dx * dx + dy * dy);
            if (distance < 6.0) {
                int column = x - area.X;
                int row = y - area.Y;
                double scale = distance;
                if (scale < 1.0) {
                    scale = 1.0;
                }
                int width = area.Width;
                grid[width * row + column] += strength / ((scale - 1.0) * 0.1 + 1.0);
            }
        }
    }
}


// bool HouseClass::AI_Build_Defense(int node, DynamicVectorClass<Cell> *cells) (0x004F1190, 3,264 B, no padding)
// TibSun AI_Build_Defense: sums the defenses of the base by quadrant, picks the weakest quadrant
// (one holding threat cells when a list is given), then the category the defenses lack most, and
// puts a defense of that category on the node, or queues it on the next node when the node
// already has a type. `lowest` is not in TibSun: an alias that names best_value before best in
// the IL, which sets the reload order ahead of the quadrant loop. Each named `dy` is needed in
// both loops for the add order in the first inlined Add.
// The local distribution's vector instantiation (COMDAT copies of the accepted members and vtables):

extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;

inline int Dir_To_4(UnknownFacingDir const &dir)
{
    return (((*(unsigned const *)&dir >> 13) + 1) >> 1) & 3;
}

bool HouseClass::AI_Build_Defense(int node, DynamicVectorClass<Cell> *cells)
{
    int i;
    int best_value;
    int const &lowest = best_value;
    int best = -1;
    best_value = 999999;
    int air_values[4];
    air_values[0] = 0;
    air_values[1] = 0;
    air_values[2] = 0;
    air_values[3] = 0;
    int armor_values[4];
    armor_values[0] = 0;
    armor_values[1] = 0;
    armor_values[2] = 0;
    armor_values[3] = 0;
    int ground_values[4];
    ground_values[0] = 0;
    ground_values[1] = 0;
    ground_values[2] = 0;
    ground_values[3] = 0;
    int totals[4];
    DynamicVectorClass<Cell> quadrants[4];

    int size = unknown_55f0.unknown_54.Width * unknown_55f0.unknown_54.Height;
    int *air_map = new int[size];
    int *armor_map = new int[size];
    int *ground_map = new int[size];
    memset(air_map, 0, size * sizeof(int));
    memset(armor_map, 0, size * sizeof(int));
    memset(ground_map, 0, size * sizeof(int));

    if (cells != 0) {
        for (i = 0; i < cells->Count(); i++) {
            Cell offset = (*cells)[i] - unknown_55f0.unknown_50;
            Point2D delta;
            delta.X = offset.X;
            delta.Y = offset.Y;
            int dy = -delta.Y;
            UnknownFacingDir dir = Offset_Direction(delta.X, dy);
            quadrants[Dir_To_4(dir)].Add((*cells)[i]);
        }
    }

    int count = Buildings.Count();
    for (i = 0; i < count; i++) {
        BuildingClass *building = Buildings[i];
        Cell offset = building->unknown_slot104() - unknown_55f0.unknown_50;
        if (offset.X != 0 || offset.Y != 0) {
            Point2D delta;
            delta.X = offset.X;
            delta.Y = offset.Y;
            int dy = -delta.Y;
            UnknownFacingDir dir = Offset_Direction(delta.X, dy);
            int quadrant = Dir_To_4(dir);
            int air = building->unknown_451710();
            int armor = building->unknown_451760();
            int infantry = building->unknown_4517b0();
            if (air + armor + infantry > 0) {
                air_values[quadrant] += air;
                armor_values[quadrant] += armor;
                ground_values[quadrant] += infantry;
                unknown_func_4f0ff0(building, air, air_map, unknown_55f0.unknown_54);
                unknown_func_4f0ff0(building, armor, armor_map, unknown_55f0.unknown_54);
                unknown_func_4f0ff0(building, infantry, ground_map, unknown_55f0.unknown_54);
            }
        }
    }

    for (i = 0; i < 4; i++) {
        totals[i] = air_values[i] + armor_values[i] + ground_values[i];
        if (totals[i] < best_value && (cells == 0 || quadrants[i].Count() > 0)) {
            best_value = totals[i];
            best = i;
        }
    }

    int air_value = air_values[best];
    int armor_value = armor_values[best];
    int ground_value = ground_values[best];

    Calculate_Enemy_Predictions();

    DynamicVectorClass<BuildingTypeClass const *> owned;
    count = Buildings.Count();
    for (i = 0; i < count; i++) {
        BuildingClass *building = Buildings[i];
        BuildingTypeClass *type = building->Type;
        if (!type->IsBaseDefense && type != Rules->unknown_78c) {
            owned.Add(building->Type);
        }
    }

    int type_id = unknown_55f0.Nodes[node].Type;
    BuildingTypeClass *pending = 0;

    float air_ratio = lowest != 0 ? air_value / (float)best_value : 0.0f;
    float armor_ratio = lowest != 0 ? armor_value / (float)best_value : 0.0f;
    float ground_ratio = lowest != 0 ? ground_value / (float)best_value : 0.0f;
    float air_deficit = unknown_15f80 - air_ratio;
    float armor_deficit = unknown_15f7c - armor_ratio;
    float ground_deficit = unknown_15f84 - ground_ratio;

    DynamicVectorClass<BuildingTypeClass *> air_defenses = Get_Anti_Air_Defense_Buildings_004f1e50(owned);
    DynamicVectorClass<BuildingTypeClass *> armor_defenses = Get_Anti_Armor_Defense_Buildings_004f1ff0(owned);
    DynamicVectorClass<BuildingTypeClass *> ground_defenses = Get_Anti_Ground_Defense_Buildings_004f2190(owned);

    DynamicVectorClass<BuildingTypeClass *> *defenses;
    int category;
    if (air_deficit <= armor_deficit || air_deficit <= ground_deficit) {
        if (armor_deficit > ground_deficit) {
            defenses = &armor_defenses;
            unknown_15f44 = armor_map;
            category = 1;
        } else {
            defenses = &ground_defenses;
            unknown_15f44 = ground_map;
            category = 2;
        }
    } else {
        defenses = &air_defenses;
        unknown_15f44 = air_map;
        category = 0;
    }

    bool success = true;
    if (defenses->Count() == 0) {
        if (armor_defenses.Count() > 0) {
            defenses = &armor_defenses;
            category = 1;
            unknown_15f44 = armor_map;
        } else if (ground_defenses.Count() > 0) {
            defenses = &ground_defenses;
            category = 2;
            unknown_15f44 = ground_map;
        } else if (air_defenses.Count() > 0) {
            defenses = &air_defenses;
            category = 0;
            unknown_15f44 = air_map;
        } else {
            success = false;
        }
    }

    if (success) {
        BuildingTypeClass *choice;
        if (defenses->Count() == 1) {
            choice = (*defenses)[0];
        } else {
            DiscreteDistributionClass<BuildingTypeClass> distribution;
            for (i = 0; i < defenses->Count(); i++) {
                BuildingTypeClass *type = (*defenses)[i];
                int weight = 10000 / type->Cost_Of(this);
                if (category == 2) {
                    weight += type->unknown_11e0;
                } else if (category == 1) {
                    weight += type->unknown_11dc;
                } else {
                    weight += type->unknown_11d8;
                }
                distribution.Add(type, weight);
            }
            choice = distribution.Sample();
        }

        BuildingTypeClass *build_type;
        if (type_id >= 0) {
            build_type = BuildingTypes[type_id];
            pending = choice;
        } else {
            build_type = choice;
        }

        Cell cell(0, 0);
        if (cells != 0) {
            IndexClass<int, Cell> ranking;
            DynamicVectorClass<Cell> sorted;
            sorted.Clear();
            for (i = 0; i < quadrants[best].Count(); i++) {
                Cell threat = quadrants[best][i];
                ranking.Add_Index(House_Facing_Score(this, threat, i, -1), threat);
            }
            for (i = 0; i < ranking.Count(); i++) {
                sorted.Add(ranking.Fetch_By_Position(i));
            }
            cell = sorted[0];
            cells->Delete(cells->ID(cell));
        } else {
            cell = Where_To_Place_Building(build_type, House_Facing_Score, 2 * best);
        }

        if (cell == Cell(0, 0)) {
            success = false;
        } else if (pending != 0) {
            unknown_55f0.Nodes[node].Cell = cell;
            if (unknown_55f0.Nodes[node + 1].Type == -1) {
                unknown_55f0.Nodes[node + 1].Type = pending->ArrayIndex;
                unknown_55f0.Nodes[node + 1].Cell = cell;
                if (cells != 0) {
                    BuildingTypeClass const *wall = unknown_func_4ef600(Rules->ConcreteWalls);
                    for (i = unknown_55f0.Nodes.Count() - 1; i >= 0; i--) {
                        if (unknown_55f0.Nodes[i].Cell == cell && unknown_55f0.Nodes[i].Type == wall->ArrayIndex) {
                            unknown_55f0.Nodes.Delete(i);
                            break;
                        }
                    }
                }
            }
        } else {
            unknown_55f0.Nodes[node].Type = build_type->ArrayIndex;
            unknown_55f0.Nodes[node].Cell = cell;
        }
    }

    delete[] air_map;
    delete[] armor_map;
    delete[] ground_map;
    unknown_15f44 = 0;

    return success;
}


// House defense candidates, original 004f1e50; independently bounded 405+11.
DynamicVectorClass<BuildingTypeClass *> HouseClass::Get_Anti_Air_Defense_Buildings_004f1e50(DynamicVectorClass<BuildingTypeClass const *> &owned)
{
    unsigned ownable = 1 << HouseTypeClass::Find_Index(Type->ParentCountry);
    DynamicVectorClass<BuildingTypeClass *> defenses;
    for (int i = 0; i < BuildingTypes.Count(); ++i) {
        BuildingTypeClass *building = BuildingTypes[i];
        if ((building->Owner & ownable) && building->unknown_11d8 > 0 && building->TechLevel <= unknown_11c && unknown_func_4ef740(building, owned, owned.Count())) {
            defenses.Add(building);
        }
    }
    return defenses;
}


// House defense candidates, original 004f1ff0; independently bounded 405+11.
DynamicVectorClass<BuildingTypeClass *> HouseClass::Get_Anti_Armor_Defense_Buildings_004f1ff0(DynamicVectorClass<BuildingTypeClass const *> &owned)
{
    unsigned ownable = 1 << HouseTypeClass::Find_Index(Type->ParentCountry);
    DynamicVectorClass<BuildingTypeClass *> defenses;
    for (int i = 0; i < BuildingTypes.Count(); ++i) {
        BuildingTypeClass *building = BuildingTypes[i];
        if ((building->Owner & ownable) && building->unknown_11dc > 0 && building->TechLevel <= unknown_11c && unknown_func_4ef740(building, owned, owned.Count())) {
            defenses.Add(building);
        }
    }
    return defenses;
}


// House defense candidates, original 004f2190; independently bounded 405+11.
DynamicVectorClass<BuildingTypeClass *> HouseClass::Get_Anti_Ground_Defense_Buildings_004f2190(DynamicVectorClass<BuildingTypeClass const *> &owned)
{
    unsigned ownable = 1 << HouseTypeClass::Find_Index(Type->ParentCountry);
    DynamicVectorClass<BuildingTypeClass *> defenses;
    for (int i = 0; i < BuildingTypes.Count(); ++i) {
        BuildingTypeClass *building = BuildingTypes[i];
        if ((building->Owner & ownable) && building->unknown_11e0 > 0 && building->TechLevel <= unknown_11c && unknown_func_4ef740(building, owned, owned.Count())) {
            defenses.Add(building);
        }
    }
    return defenses;
}


// 0x004f2330: function not yet identified
//
// Body: 24 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_reset_ratios_004f2330(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004f2330 {

void __fastcall unknown_reset_ratios_004f2330(int house)
{
    *(unsigned int *)(house + 0x15f84) = 0x3ea8f5c3;
    *(unsigned int *)(house + 0x15f80) = 0x3ea8f5c3;
    *(unsigned int *)(house + 0x15f7c) = 0x3ea8f5c3;
    return;
}

}  // namespace seed_004f2330


// AI_Build_Wall (TibSun twin): plan a perimeter of walls and gates around the base area, original 0x004F2350,
// 2,405 B body. The `buildings` check is reused as the soft-block flag (one byte slot) and the node initialization is an inline hypothesis.
static FacingType const wall_step_facings[4] = {FACING_E,FACING_E,FACING_S,FACING_S};
static FacingType const wall_adjacent_facings[4] = {FACING_N,FACING_S,FACING_W,FACING_E};
void HouseClass::AI_Build_Wall()
{
    Rect &area = unknown_55f0.unknown_54;
    Rect perimeter=area;
    --perimeter.X; --perimeter.Y; perimeter.Width+=2; perimeter.Height+=2;
    unknown_55f0.unknown_64=perimeter;
    DynamicVectorClass<BaseNodeClass> walls;
    DynamicVectorClass<BaseNodeClass> gates;
    Cell starts[4] = {
        Cell(perimeter.X,perimeter.Y),
        Cell(perimeter.X,perimeter.Y+perimeter.Height-1),
        Cell(perimeter.X,perimeter.Y+1),
        Cell(perimeter.X+perimeter.Width-1,perimeter.Y+1)
    };
    Cell ends[4] = {
        Cell(perimeter.X+perimeter.Width,perimeter.Y),
        Cell(perimeter.X+perimeter.Width,perimeter.Y+perimeter.Height-1),
        Cell(perimeter.X,perimeter.Y+perimeter.Height-1),
        Cell(perimeter.X+perimeter.Width-1,perimeter.Y+perimeter.Height-1)
    };
    BuildingTypeClass *wall = unknown_func_4ef600(Rules->ConcreteWalls);
    BuildingTypeClass *east_west_gate = unknown_func_4ef600(Rules->EWGates);
    BuildingTypeClass *north_south_gate = unknown_func_4ef600(Rules->NSGates);
    BuildingTypeClass *gate_types[4] = {east_west_gate,east_west_gate,north_south_gate,north_south_gate};
    Cell beginning(0,0);
    int elevation = Map[unknown_55f0.unknown_50].unknown_11b;
    int side;
    for (side=0;side<4;++side) {
        FacingType direction=wall_step_facings[side];
        Cell cell=starts[side];
        int length=0;
        bool running=false;
        int soft_blocks=0;
        while (cell != ends[side]) {
            CellClass *current=&Map[cell];
            Cell adjacent=Adjacent_Cell(cell,wall_adjacent_facings[side]);
            CellClass *neighbor=&Map[adjacent];
            int land=current->unknown_ec;
            int adjacent_land=neighbor->unknown_ec;
            bool level=abs(current->unknown_11b-elevation)<=2;
            bool ground=land!=3 && land!=2 && land!=8 && adjacent_land!=3 && adjacent_land!=2 && adjacent_land!=8;
            bool overlay=current->OverlayIndex==-1;
            bool buildings=!current->Cell_Building() && !neighbor->Cell_Building();
            bool terrain=!current->Cell_Terrain() && !neighbor->Cell_Terrain();
            bool flat=current->unknown_11c==0;
            bool visible=Map.Unknown_0055A660(cell,true);
            if (level && ground && overlay && buildings && terrain && flat && visible) {
                if (!running) { running=true; length=1; beginning=cell; }
                else ++length;
            } else {
buildings=!(level && ground && buildings && terrain && visible);
 if (buildings) { if (soft_blocks>0) { buildings=false; soft_blocks=0; } } else ++soft_blocks;
                if (length>=5 || !buildings) {
                    Cell position=beginning;
                    int placed=0;
                    int middle=length/2-1;
                    while (placed<length) {
                        if (placed==middle && buildings) {
                            gates.Add(BaseNodeClass(gate_types[side]->ArrayIndex,position));
                            placed+=3;
                            position=Adjacent_Cell(Adjacent_Cell(Adjacent_Cell(position,direction),direction),direction);
                        } else {
                            walls.Add(BaseNodeClass(wall->ArrayIndex,position));
                            ++placed;
                            position=Adjacent_Cell(position,direction);
                        }
                    }
                }
                length=0;
                running=false;
            }
            cell=Adjacent_Cell(cell,direction);
        }
        if (length>=5) {
            Cell position=beginning;
            int placed=0;
            int middle=length/2-1;
            while (placed<length) {
                if (placed==middle) {
                    gates.Add(BaseNodeClass(gate_types[side]->ArrayIndex,position));
                    placed+=3;
                    position=Adjacent_Cell(Adjacent_Cell(Adjacent_Cell(position,direction),direction),direction);
                } else {
                    walls.Add(BaseNodeClass(wall->ArrayIndex,position));
                    ++placed;
                    position=Adjacent_Cell(position,direction);
                }
            }
        }
    }
    for (int w=0;w<walls.Count();++w) unknown_55f0.Nodes.Add(walls[w]);
    for (int g=0;g<gates.Count();++g) unknown_55f0.Nodes.Add(gates[g]);
    unknown_55f0.unknown_54=unknown_55f0.unknown_64;
}


// Recounts active buildings' power and drain (0x004F2CC0,393bytes).
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house);

void HouseClass::Recalc_Power_004f2cc0()
{
    bool was_low = Power_Fraction() < 1.0;
    unknown_5668 = false;
    Power = 0;
    Drain = 0;
    int count = Buildings.Count();
    for (int index = 0; index < count; ++index) {
        BuildingClass *building = Buildings[index];
        if (building && !building->unknown_76 && building->unknown_74) {
            if (!Is_Player_Control() || building->unknown_373 || unknown_game_mode_a3d298 != 0) {
                Power += building->unknown_449d40();
                Drain += building->unknown_449dd0();
            }
        }
    }
    if (unknown_1ec.Value() != 0) Power = 0;
    Building_Refresh_House_Buildings_0044fa40(this);
    FactoryClass::Update_Rates(this);
    if (was_low != (Power_Fraction() < 1.0)) Update_Present_Super_Weapons();
    unknown_5669 = true;
}


// void HouseClass::unknown_func_4f2e50() (0x004F2E50)
// HouseClass::AI for the player's house: once the +0x1F8 timer has run out
// the radar is on with the scenario's FreeRadar, or with full power and an
// active radar building on the map (+0x530 set, not being sold; outside the
// campaign or for a house the player does not control any, otherwise only one
// with +0x373) that has no +0x3FC and does not refuse (slot 111). The map's
// radar is switched when it differs. +0x5669 is cleared first.

void HouseClass::unknown_func_4f2e50()
{
    unknown_5669 = false;
    if (this == unknown_player_a35db4) {
        bool radar = false;
        if (unknown_1f8 == 0) {
            if (unknown_id_source_a3d290->FreeRadar) {
                radar = true;
            } else if (Power_Fraction() >= 1.0) {
                for (int index = 0; index < Buildings.Count(); index++) {
                    BuildingClass *building = Buildings[index];
                    if (building != 0 && building->unknown_530 && building->Type->Radar && !building->unknown_76
                        && building->unknown_74
                        && ((unknown_game_mode_a3d298 == 0 && !unknown_134 && !unknown_135) || building->unknown_373
                            || unknown_game_mode_a3d298 != 0)
                        && building->Mission != 0x13 && building->MissionQueue != 0x13) {
                        if (building->unknown_3fc == 0 && !building->unknown_slot111()) {
                            radar = true;
                        }
                        break;
                    }
                }
            }
        }
        if (Map.Is_Radar_Existing() != radar) {
            Map.Toggle_Radar(radar);
        }
    }
}


// void HouseClass::unknown_func_4f2fc0() (0x004F2FC0)
// HouseClass::AI for the player's house: the spy satellite view is on while
// one of its buildings is an active spy satellite on the map that is not
// being sold (mission 0x13, current or queued); outside the campaign or for a
// house the player does not control any such building counts, otherwise only
// one with +0x373. Turning it on or off plays the Rules sound and reveals or
// covers the map. +0x5669 is cleared first.

void HouseClass::unknown_func_4f2fc0()
{
    unknown_5669 = false;
    if (this == unknown_player_a35db4) {
        for (int index = 0; index < Buildings.Count(); index++) {
            BuildingClass *building = Buildings[index];
            if (building != 0 && building->Type->SpySat && !building->unknown_76 && building->unknown_74
                && ((unknown_game_mode_a3d298 == 0 && !unknown_134 && !unknown_135) || building->unknown_373
                    || unknown_game_mode_a3d298 != 0)
                && building->Mission != 0x13 && building->MissionQueue != 0x13) {
                if (!unknown_566a) {
                    Voc_Play(Rules->SpySatActivationSound, 1.0f, 0x2000, 0);
                    Map.Unknown_0055A120();
                    unknown_566a = true;
                }
                return;
            }
        }
        if (unknown_566a) {
            Map.Unknown_00559F10();
            Voc_Play(Rules->SpySatDeactivationSound, 1.0f, 0x2000, 0);
            unknown_566a = false;
        }
    }
}


// int HouseClass::unknown_func_4f30d0() (0x004F30D0)
// The index of the first waypoint path of the house that has no waypoints, else -1; a path that was not
// made yet is made empty for the test (and not kept).

int HouseClass::unknown_func_4f30d0()
{
    for (int index = 0; index < 12; index++) {
        WaypointPathClass *path = WaypointPaths[index];
        if (path == 0) {
            path = new WaypointPathClass;
        }
        if (path->Waypoints.Count() == 0) {
            return index;
        }
    }
    return -1;
}


// bool __fastcall unknown_slot_ready(UnknownSlotOwner_004f3120 const *) -- 0x004F3120
// Purpose: for the current slot index (+0x154, valid 0..11) of the owner's
// twelve record pointers (+0x158), true when the record's +0x38 is below the
// rules value at +0x98 (rules object at 0x00839848) and its +0x24 is -1.
// No direct callers. Types not identified (rules view only).

struct UnknownSlotRecord_004f3120 {
    char unknown_00[0x24];
    int unknown_24;
    char unknown_28[0x38 - 0x28];
    int unknown_38;
};

struct UnknownSlotOwner_004f3120 {
    char unknown_000[0x154];
    int current;                                 // +0x154
    UnknownSlotRecord_004f3120 *records[12];     // +0x158
};

bool __fastcall unknown_slot_ready_004f3120(UnknownSlotOwner_004f3120 const *owner)
{
    if (owner->current < 0 || owner->current >= 12) {
        return false;
    }
    UnknownSlotRecord_004f3120 const *record = owner->records[owner->current];
    return record->unknown_38 < Rules->unknown_98 && record->unknown_24 == -1;
}


class Leaf_4f3160 { public: void get(); };
void Leaf_4f3160::get() { *(unsigned char*)((char*)this + 0x143) = 1; }


// void HouseClass::unknown_func_4f3170(int kind, bool naval, int arg3) (0x004F3170)
// A factory building left (BuildingClass slot 52): the house's factory for
// the type kind drops the queued types it can no longer build; the object in
// production is abandoned when it cannot be built at all, suspended (and the
// player's sidebar strip redrawn) when only the limits forbid it, or resumed
// when it was waiting; an idle empty factory is deleted.

void HouseClass::unknown_func_4f3170(int kind, bool naval, int arg3)
{
    FactoryClass *factory;
    switch (kind) {
    case 7:
        if (arg3 == 5) {
            factory = unknown_52f8;
        } else {
            factory = unknown_52e8;
        }
        break;
    case 40:
        if (naval) {
            factory = unknown_52e4;
        } else {
            factory = unknown_52e0;
        }
        break;
    case 16:
        factory = unknown_52dc;
        break;
    case 3:
        factory = unknown_52d8;
        break;
    default:
        return;
    }
    if (factory == 0) {
        return;
    }
    for (int index = factory->Queue.Count() - 1; index >= 0; index--) {
        if (factory->Queue[index]->Who_Can_Build_Me(true, false, true, this) == 0) {
            factory->Queue.Delete(index);
        }
    }
    if (factory->Object != 0) {
        if (factory->Object->Techno_Type_Class()->Who_Can_Build_Me(true, false, true, this) == 0) {
            factory->Abandon_Production_004B97C0();
            factory->Start_Next_Queued_004B9D70();
        } else if (factory->Object->Techno_Type_Class()->Who_Can_Build_Me(true, true, true, this) == 0) {
            factory->Suspend_004B9630(false);
            if (unknown_player_a35db4 == this) {
                Map.Unknown_0067C2D0(SidebarClass::Which_Column(kind, arg3, naval));
            }
        } else if (factory->unknown_6c && !factory->unknown_6d) {
            factory->Start_Production_004B9670(false);
        }
    }
    if (factory->Object == 0 && factory->Queue.Count() == 0) {
        delete factory;
    }
}


// void HouseClass::unknown_func_4f3320(HouseClass *house) (0x004F3320)
// Marks the house's country in +0x5414; when that house is the player's, every
// techno of this house reveals its sight around it and the map redraws.

void HouseClass::unknown_func_4f3320(HouseClass *house)
{
    unknown_5414 |= 1 << house->Type->House;
    if (house == unknown_player_a35db4) {
        for (int index = 0; index < Technos.Count(); index++) {
            TechnoClass *techno = Technos[index];
            if (techno != 0 && !techno->unknown_76 && techno->House == this) {
                CoordStruct coord = techno->Coord;
                Map.Unknown_00549C50(coord, techno->Techno_Type_Class()->Sight, this, false, false, false, true, true);
            }
        }
        Map.Flag_To_Redraw(2);
    }
}


// void HouseClass::unknown_func_4f33e0(HouseClass *house) (0x004F33E0)
// Forgets the anger at a house (adds the negated anger; name hypothesis).

void HouseClass::unknown_func_4f33e0(HouseClass *house)
{
    for (int index = 0; index < AngerNodes.Count(); index++) {
        if (AngerNodes[index].House == house) {
            unknown_func_4eeba0(-AngerNodes[index].Value, house);
            return;
        }
    }
}


// void HouseClass::unknown_func_4f3420() (0x004F3420)
// Rebuilds the house's threat map (+0x56D0): every techno on the map adds its
// threat (slot 168) to the regions around its cell; foot objects of this
// house, or of its allies when a human controls it, are left out. Called when
// an alliance changes. 0x004E5DC0 (RA's Adjust_Threat) expands in both branches.

void HouseClass::unknown_func_4f3420()
{
    for (int region = 0; region < 0x4204; region++) {
        unknown_56d0[region].Threat = 0;
    }
    for (int index = Technos.Count() - 1; index >= 0; index--) {
        TechnoClass *techno = Technos[index];
        if (techno->unknown_76 || !techno->unknown_7d) {
            continue;
        }
        int threat = techno->unknown_slot168();
        if (threat <= 0) {
            continue;
        }
        FootClass *foot = (techno->Kind_Of() == 15 || techno->Kind_Of() == 1 || techno->Kind_Of() == 2) ? (FootClass *)techno : 0;
        if (foot != 0) {
            if (foot->unknown_454 == unknown_cell_a40f10) {
                continue;
            }
            int region = Map.Unknown_0054E370(foot->unknown_454);
            if (foot->House == this) {
                continue;
            }
            if (Is_Human_Control() && Is_Ally(foot->House)) {
                continue;
            }
            unknown_func_4e5dc0(region, threat);
        } else {
            unknown_func_4e5dc0(Map.Unknown_0054E370(((CellClass *)techno->Get_Cell_Ptr())->MapCoords), threat);
        }
    }
}


// int HouseClass::Team_Count(TeamTypeClass *team) (0x004F35F0)
// The number of teams of the house made from a team type (counted from the newest team back).

int HouseClass::Team_Count(TeamTypeClass *team)
{
    int count = 0;
    for (int index = Teams.Count() - 1; index >= 0; index--) {
        TeamClass *candidate = Teams[index];
        if (candidate != 0 && candidate->Owner == this && team == candidate->Class) {
            count++;
        }
    }
    return count;
}


// bool HouseClass::Can_Build_Team(TeamTypeClass *team) (0x004F3630)
// Whether the house can field every member type of the team's task force:
// each type must be buildable now (Who_Can_Build_Me); in the campaign a
// member type it cannot build is also accepted when one of the house's own
// foot objects of that type may join the team (0x006C0010).

bool HouseClass::Can_Build_Team(TeamTypeClass *team)
{
    if (team == 0 || team->TaskForce == 0) {
        return false;
    }
    for (int index = 0; index < team->TaskForce->CountEntries; index++) {
        TechnoTypeClass *type = team->TaskForce->Entries[index].Type;
        if (type == 0 || type->Who_Can_Build_Me(true, true, false, this) == 0) {
            if (unknown_game_mode_a3d298 != 0) {
                return false;
            }
            bool found = false;
            for (int foot_index = unknown_FootClass_list_00865f38.Count() - 1; foot_index >= 0; foot_index--) {
                FootClass *foot = unknown_FootClass_list_00865f38[foot_index];
                if (foot->House == this && type == foot->Techno_Type_Class() && team->unknown_func_6c0010(foot, this)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    return true;
}


// void HouseClass::unknown_func_4f3720(int arg1) (0x004F3720)
// The money mode (+0x12C) of a computer house outside the campaign, by its
// Available_Money against Rules AIAlternateProductionCreditCutoff; mode 1
// may end when the house owns a BuildBarracks and a BuildWeapons type, has
// the power, and a coin flip says so; arg1 6 picks mode 2 (meaning not recovered).

void HouseClass::unknown_func_4f3720(int arg1)
{
    if (Is_Human_Control() || unknown_game_mode_a3d298 == 0) {
        return;
    }
    switch (unknown_12c) {
    case 1:
        if (Available_Money() < Rules->AIAlternateProductionCreditCutoff) {
            if (arg1 == 6) {
                unknown_12c = 2;
            }
        } else {
            unknown_12c = 0;
        }
        break;
    case 2:
        if (Available_Money() < Rules->AIAlternateProductionCreditCutoff) {
            bool barracks = false;
            for (int index = 0; index < Rules->BuildBarracks.Count(); index++) {
                barracks = barracks || unknown_5460.unknown_func_491d10(Rules->BuildBarracks[index]->ArrayIndex) > 0;
            }
            bool factory = false;
            for (int index2 = 0; index2 < Rules->BuildWeapons.Count(); index2++) {
                factory = factory || unknown_5460.unknown_func_491d10(Rules->BuildWeapons[index2]->ArrayIndex) > 0;
            }
            if (!(factory && barracks && Drain <= Power && Random_Pick(0, 1) != 0) && arg1 != 6) {
                unknown_12c = 1;
            }
        } else {
            unknown_12c = 0;
        }
        break;
    case 3:
        if (Available_Money() > Rules->AIAlternateProductionCreditCutoff) {
            unknown_12c = 0;
        }
        break;
    case 0:
        if (Available_Money() < Rules->AIAlternateProductionCreditCutoff) {
            unknown_12c = (arg1 == 6) + 1;
        }
        break;
    }
}


// AI use of ready super weapons (0x004F3910;427originalbytes).

void HouseClass::AI_Supers_004f3910()
{
    Cell target_0;
    Cell target_2;
    if (!Is_Human_Control()) {
        for (int index = 0; index < Supers.Count(); ++index) {
            SuperClass *super = Supers[index];
            if (super && super->unknown_0x57) {
                switch (super->Type->Type) {
                case 0: {
                    SuperClass *pending = super;
                    if (unknown_54f0 != -1) {
                        target_0 = unknown_cell_a40f10;
                        if (unknown_541c == 1) target_0 = unknown_func_4f65a0();
                        else target_0 = unknown_func_4f6a80(unknown_541c);
                        if (target_0 != unknown_cell_a40f10)
                            unknown_func_4e68d0(Supers.ID(pending), target_0);
                    }
                    break;
                }
                case 2: {
                    SuperClass *pending = super;
                    if (!Unknown_0051FC50() && unknown_54f0 != -1) {
                        target_2 = unknown_cell_a40f10;
                        if (unknown_541c == 1) target_2 = unknown_func_4f65a0();
                        else target_2 = unknown_func_4f6a80(unknown_541c);
                        if (target_2 != unknown_cell_a40f10)
                            unknown_func_4e68d0(Supers.ID(pending), target_2);
                    }
                    break;
                }
                case 5:
                case 6: unknown_func_4f3c50(super); break;
                }
            }
        }
    }
}


// void HouseClass::unknown_func_4f3ae0(SuperClass *super) (0x004F3AE0)
// While the house has an enemy (+0x54F0), picks a target cell (0x004F65A0
// for target mode 1, else 0x004F6A80 by mode) and fires the super weapon at it.

void HouseClass::unknown_func_4f3ae0(SuperClass *super)
{
    if (unknown_54f0 != -1) {
        Cell cell = unknown_cell_a40f10;
        if (unknown_541c == 1) {
            cell = unknown_func_4f65a0();
        } else {
            cell = unknown_func_4f6a80(unknown_541c);
        }
        if (cell != unknown_cell_a40f10) {
            unknown_func_4e68d0(Supers.ID(super), cell);
        }
    }
}


// void HouseClass::unknown_func_4f3b70(SuperClass *super) (0x004F3B70)
// While the house has an enemy (+0x54F0), fires the super weapon at the
// empty cell 0x00A40F10.

void HouseClass::unknown_func_4f3b70(SuperClass *super)
{
    if (unknown_54f0 != -1) {
        unknown_func_4e68d0(Supers.ID(super), unknown_cell_a40f10);
    }
}


// void HouseClass::unknown_func_4f3bb0(SuperClass *super) (0x004F3BB0)
// While the house has an enemy (+0x54F0), picks a target cell (0x004F65A0
// for target mode 1, else 0x004F6A80 by mode) and fires the super weapon at it.

void HouseClass::unknown_func_4f3bb0(SuperClass *super)
{
    if (unknown_54f0 != -1) {
        Cell cell = unknown_cell_a40f10;
        if (unknown_541c == 1) {
            cell = unknown_func_4f65a0();
        } else {
            cell = unknown_func_4f6a80(unknown_541c);
        }
        if (cell != unknown_cell_a40f10) {
            unknown_func_4e68d0(Supers.ID(super), cell);
        }
    }
}


// 0x004f3c40: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_004f3c40(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004f3c40 {

void __stdcall unknown_empty_004f3c40(int unused_0)
{
    return;
}

}  // namespace seed_004f3c40


// void HouseClass::unknown_func_4f3c50(SuperClass *super) (0x004F3C50)
// Fires a super weapon (0x004E68D0) at a cell: with target mode +0x541C 1, two
// cells beyond the reachable cell nearest the base centre of the house the
// computer is angry at (+0x54F0, else this house); otherwise at the mode's
// cell (0x004F6A80). Nothing happens when no cell is found.

void HouseClass::unknown_func_4f3c50(SuperClass *super)
{
    HouseClass *house = unknown_54f0 != -1 ? Houses[unknown_54f0] : this;
    Cell cell = unknown_cell_a40f10;
    if (unknown_541c != 1) {
        cell = unknown_func_4f6a80(unknown_541c);
    } else {
        cell = Coord_Cell(house->BaseCenter);
        cell = Map.Nearby_Location(cell, 0, -1, 0, false, Point2D(5, 5), 0, 0, 0, 1, Cell(0, 0), 0);
        cell.X += 2;
        cell.Y += 2;
    }
    if (cell != unknown_cell_a40f10) {
        unknown_func_4e68d0(Supers.ID(super), cell);
    }
}


// void HouseClass::unknown_func_4f3d70(SuperClass *super) (0x004F3D70)
// As 0x004F3AE0, unless 0x0051FC50's flag is set.

void HouseClass::unknown_func_4f3d70(SuperClass *super)
{
    if (!Unknown_0051FC50() && unknown_54f0 != -1) {
        Cell cell = unknown_cell_a40f10;
        if (unknown_541c == 1) {
            cell = unknown_func_4f65a0();
        } else {
            cell = unknown_func_4f6a80(unknown_541c);
        }
        if (cell != unknown_cell_a40f10) {
            unknown_func_4e68d0(Supers.ID(super), cell);
        }
    }
}


// void HouseClass::unknown_func_4f3e00(SuperClass *super) (0x004F3E00)
// Fires a super weapon (0x004E68D0) at the building of the house the
// computer is angry at (+0x54F0) whose cell rates highest for this house
// (0x0054E3A0; a building with +0x5BD 15 rates at random up to 100).

void HouseClass::unknown_func_4f3e00(SuperClass *super)
{
    if (unknown_54f0 != -1) {
        BuildingClass *best = 0;
        int value = -1;
        HouseClass *house = Houses[unknown_54f0];
        for (int i = unknown_BuildingClass_list_00a40af8.Count() - 1; i >= 0; i--) {
            BuildingClass *building = unknown_BuildingClass_list_00a40af8[i];
            if (building->House == house) {
                int rating = Map.Cell_Threat(Coord_Cell(building->Center_Coord()), this);
                if (building->unknown_5bd == 15) {
                    rating = Random_Pick(0, 100);
                }
                if (rating > value) {
                    value = rating;
                    best = building;
                }
            }
        }
        if (best != 0) {
            unknown_func_4e68d0(Supers.ID(super), Coord_Cell(best->Center_Coord()));
        }
    }
}


// Clear overlapping planned cells when a base building leaves (0x004F3F40).

void HouseClass::unknown_func_4f3f40(BuildingClass *building)
{
    if (Debug_Map) return;
    CoordStruct position=building->Position_Coord_004f3f40();
    Cell location=Coord_Cell(position);
    int count = unknown_55f0.Nodes.Count();
    for (int index = 0; index < count; ++index) {
        if (unknown_55f0.Nodes[index].Type == building->Type->ArrayIndex &&
            unknown_55f0.Nodes[index].Cell == location) {
            Cell cell = unknown_55f0.Nodes[index].Cell;
            for (int other = 0; other < unknown_55f0.Nodes.Count(); ++other) {
                if (index != other && unknown_55f0.Nodes[other].Cell == cell)
                    unknown_55f0.Nodes[other].Cell = unknown_cell_a40f10;
            }
            if (building->Type->IsBaseDefense && unknown_game_mode_a3d298 != 0) {
                unknown_55f0.Nodes[index].Type = -1;
                unknown_55f0.Nodes[index].Cell = unknown_cell_a40f10;
            }
            return;
        }
    }
}


// void HouseClass::AI_Takeover() (0x004F4070): the AI takes over this house (TibSun AI_Takeover):
// name, factories, handicap, base centre, then rebuilds the base nodes from the buildings it owns.
// A computer house in a network game. The query is one level down here: the original calls
// Is_Player_Control out of line at this site and expands it at the first one.
inline bool Is_Network_Computer(HouseClass *house)
{
    return !house->Is_Human_Player() && unknown_game_mode_a3d298 != 0;
}
void HouseClass::AI_Takeover()
{
    if (!Is_Human_Player()) return;
    unknown_134=false;
    unknown_135=false;
    unknown_194=Rules->MaxIQ;
    IniName=FixedString<21>("Computer");
    wcscpy(UIName,StringTable::Load_String("TXT_COMPUTER",0,"D:\\Ra2\\House.CPP",0x3130));
    int index;
    for (index=Factories.Count()-1;index>=0;--index) {
        FactoryClass *factory=Factories[index];
        if (factory->Owner==this) {
            Factories[index]->Suspend_004B9630(true);
            Factories[index]->Abandon_Production_004B97C0();
            delete Factories[index];
        }
    }
    Assign_Handicap(2-unknown_c8);
    BuildingClass *conyard=0;
    for (index=Buildings.Count()-1;index>=0;--index) {
        BuildingClass *building=Buildings[index];
        bool owned_type=false;
        for (int candidate=0;candidate<Rules->unknown_7c8.Count();++candidate) {
            if (Rules->unknown_7c8[candidate]==building->Type) { owned_type=true; break; }
        }
        if (!owned_type) continue;
        if (!building->unknown_76) { conyard=building; break; }
    }
    if (!conyard) return;
    Cell center=Coord_Cell(conyard->Position_Coord_004f3f40());
    BaseCenter=Cell_Coord(center);
    if (Is_Network_Computer(this)) unknown_func_4ec5e0();
    if (unknown_55f0.Nodes.Count()==0) {
        int saved_init=unknown_a40774;
        unknown_a40774=0;
        unknown_func_4ef890();
        unknown_a40774=saved_init;
    }
    unknown_55f0.Nodes[0].Cell=center;
    unknown_55f0.unknown_50=center;
    unknown_136=true;
    unknown_13a=true;
    unknown_13b=true;
    for (int node=unknown_55f0.Nodes.Count()-1;node>=0;--node) {
        int type=unknown_55f0.Nodes[node].Type;
        if (type>=0 && unknown_5460.unknown_func_491d10(type)>0) {
            BuildingTypeClass *building_type=BuildingTypes[type];
            for (int owned=Buildings.Count()-1;owned>=0;--owned) {
                BuildingClass *building=Buildings[owned];
                if (!building->unknown_76 && (building->Type==building_type
                    || building->unknown_4c0[0]==building_type || building->unknown_4c0[1]==building_type
                    || building->unknown_4c0[2]==building_type)) {
                    unknown_55f0.Nodes[node].Cell=Coord_Cell(building->Position_Coord_004f3f40());
                }
            }
        }
    }
    Map.Reset_Iterator();
    CellClass *cell=Map.Iterate();
    while (cell) {
        if (cell->unknown_50==ArrayIndex && cell->OverlayIndex!=-1 && OverlayTypes[cell->OverlayIndex]->Wall) {
            Sell_Wall(cell->MapCoords,true);
        }
        cell=Map.Iterate();
    }
    int building_count=Buildings.Count();
    int defense_node=0;
    for (index=0;index<building_count;++index) {
        BuildingClass *building=Buildings[index];
        if (building->unknown_76) continue;
        BuildingTypeClass *building_type=building->Type;
        if (!building_type->IsBaseDefense && !(building_type==Rules->unknown_78c && building->unknown_5c6>0)) continue;
        if (!building_type->Who_Can_Build_Me(true,false,true,this)) continue;
        while (defense_node<unknown_55f0.Nodes.Count()) {
            if (unknown_55f0.Nodes[defense_node].Type==-1) {
                if (building->Type==Rules->unknown_78c) {
                    unknown_55f0.Nodes.Insert_After(defense_node,BaseNodeClass(
                        ((BuildingTypeClass *)building->unknown_4c0[building->unknown_5c6-1])->ArrayIndex,
                        Coord_Cell(building->Position_Coord_004f3f40())));
                    unknown_55f0.Nodes[defense_node].Type=building->Type->ArrayIndex;
                    unknown_55f0.Nodes[defense_node].Cell=Coord_Cell(building->Position_Coord_004f3f40());
                    ++defense_node;
                } else {
                    unknown_55f0.Nodes[defense_node].Type=building->Type->ArrayIndex;
                    unknown_55f0.Nodes[defense_node].Cell=Coord_Cell(building->Position_Coord_004f3f40());
                }
                ++defense_node;
                break;
            }
            ++defense_node;
        }
    }
    int power_node=0;
    int power=0;
    int drain=0;
    building_count=Buildings.Count();
    for (index=0;index<building_count;++index) {
        BuildingClass *building=Buildings[index];
        if (building->unknown_76) continue;
        BuildingTypeClass *building_type=building->Type;
        if (building_type!=Rules->GDIPowerPlant && building_type!=Rules->NodRegularPower
            && building_type!=Rules->NodAdvancedPower) continue;
        if (Coord_Cell(building->Position_Coord_004f3f40())==unknown_55f0.Nodes[1].Cell) continue;
        if (!building_type->Who_Can_Build_Me(true,false,true,this)) continue;
        while (power_node<unknown_55f0.Nodes.Count()) {
            if (unknown_55f0.Nodes[power_node].Type>=0) {
                BuildingTypeClass *planned_type=BuildingTypes[unknown_55f0.Nodes[power_node].Type];
                power+=planned_type->Power;
                drain+=planned_type->PowerDrain_c6c;
                if (power_node>0 && power<drain+unknown_15f88) {
                    unknown_55f0.Nodes.Insert_After(power_node-1,BaseNodeClass(building->Type->ArrayIndex,
                        Coord_Cell(building->Position_Coord_004f3f40())));
                    power+=building->Type->Power;
                    drain-=planned_type->PowerDrain_c6c;
                    ++power_node;
                    break;
                }
            }
            ++power_node;
        }
    }
    if (_stricmp(Type->ID,"GDI")==0) {
        power_node=0;
        power=0;
        drain=0;
        for (index=0;index<unknown_BuildingClass_list_00a40af8.Count();++index) {
            BuildingClass *building=unknown_BuildingClass_list_00a40af8[index];
            if (building->House!=this || building->unknown_76 || building->Type!=Rules->GDIPowerPlant
                || building->unknown_5c6<=0) continue;
            for (int upgrade=0;upgrade<building->unknown_5c6;++upgrade) {
                while (power_node<unknown_55f0.Nodes.Count()) {
                    if (unknown_55f0.Nodes[power_node].Type>=0) {
                        BuildingTypeClass *planned_type=BuildingTypes[unknown_55f0.Nodes[power_node].Type];
                        power+=planned_type->Power;
                        drain+=planned_type->PowerDrain_c6c;
                        if (power_node>0 && power<drain+unknown_15f88) {
                            unknown_55f0.Nodes.Insert_After(power_node-1,BaseNodeClass(Rules->GDIPowerTurbine->ArrayIndex,
                                Coord_Cell(building->Position_Coord_004f3f40())));
                            power+=Rules->GDIPowerTurbine->Power;
                            drain-=planned_type->PowerDrain_c6c;
                            ++power_node;
                            break;
                        }
                    }
                    ++power_node;
                }
            }
        }
    }
}


// void HouseClass::Update_Present_Super_Weapons() (0x004F4B20)
// TibSun Update_Present_Super_Weapons: for each super weapon present that needs a building (or
// when the house is defeated), find a building or upgrade that grants it, drop it when power is
// short, suspend or resume it, or remove it when nothing grants it any more. A change clears
// the pending target and redraws the sidebar for the player's house.
// `on` and `alive` are not in TibSun. `on` stops known-value threading of the flag, so the
// second test stays; the second named negation sets the compare order of the Fetch_ID pair.
void HouseClass::Update_Present_Super_Weapons()
{
    if (!unknown_flag_a40958) return;
    bool is_player = unknown_player_a35db4 == this;
    for (int s = 0; s < Supers.Count(); ++s) {
        if (Supers[s]->Is_Present() && ((Supers[s]->unknown_0x48 && !Supers[s]->Is_One_Time()) || unknown_13d)) {
            bool present = false;
            bool enabled = false;
            bool alive = !unknown_13d;
            if (alive) {
                for (int j = 0; j < unknown_BuildingClass_list_00a40af8.Count(); ++j) {
                    BuildingClass *b = unknown_BuildingClass_list_00a40af8[j];
                    if (!b->unknown_76 && b->unknown_7d && b->House->Fetch_ID() == Fetch_ID()) {
                        for (int k = 0; k < 3; ++k) {
                            BuildingTypeClass *upgrade = (BuildingTypeClass *)b->unknown_4c0[k];
                            if (upgrade && (upgrade->SuperWeapon == s || upgrade->SuperWeapon2 == s)) {
                                present = true;
                                if (!enabled) enabled = b->unknown_530;
                            }
                        }
                        if (b->unknown_452800() == s || b->unknown_452860() == s) {
                            present = true;
                            if (enabled) break;
                            enabled = b->unknown_530;
                        }
                        if (enabled && present) break;
                    }
                }
            }
            int power = Power;
            int drain = Drain;
            if (power < drain && drain != 0) {
                if (power == 0 || (double)power / (double)drain < 1.0) {
                    enabled = false;
                }
            }
            int type_id = Supers[s]->Type->Fetch_Heap_ID();
            int type_index = Unknown_006818D0(57, type_id, 0);
            if (present && !unknown_13d) {
                bool const &on = enabled;
                bool off = !on;
                if ((off && Supers[s]->Is_Powered() && Supers[s]->Suspend(true)) || (enabled && Supers[s]->Suspend(false))) {
                    if (is_player) {
                        if (unknown_pending_super_833694 == s) unknown_pending_super_833694 = -1;
                        Map.Unknown_0067C2D0(type_index);
                    }
                    unknown_144 = true;
                }
            } else {
                if (Supers[s]->Remove() && unknown_player_a35db4 != 0) {
                    if (is_player) {
                        if (unknown_pending_super_833694 == s) unknown_pending_super_833694 = -1;
                        Map.Unknown_0067C2D0(type_index);
                    }
                    unknown_144 = true;
                }
            }
        }
    }
}


// Grant building-provided super weapons; original004F4DB0.
void HouseClass::Enable_Available_Super_Weapons_004f4db0()
{
    if (unknown_13d) return;
    for (int super_index=0;super_index<Supers.Count();++super_index) {
        if (!Supers[super_index]->Is_Present() || Supers[super_index]->Is_One_Time()) {
            bool from_upgrade=false;
            int building_index=Buildings.Count()-1;
            if (building_index>=0) while (true) {
                BuildingClass *building=Buildings[building_index];
                if (building->unknown_7d && !building->unknown_76) {
                    for (int upgrade_index=0;upgrade_index<3;++upgrade_index) {
                        BuildingTypeClass *upgrade=(BuildingTypeClass *)building->unknown_4c0[upgrade_index];
                        if (upgrade && (upgrade->SuperWeapon==super_index || upgrade->SuperWeapon2==super_index)) {
                            from_upgrade=true;
                            break;
                        }
                    }
                    if (building->unknown_452800()==super_index || building->unknown_452860()==super_index || from_upgrade) {
                        Supers[super_index]->Grant(false,this==unknown_player_a35db4,Power_Fraction()<1.0);
                        if (this==unknown_player_a35db4) {
                            Map.Unknown_0067C530(31,super_index);
                            int type_index=Supers[super_index]->Type->Fetch_Heap_ID();
                            Map.Unknown_0067C2D0(Unknown_006818D0(57,type_index,0));
                        }
                        break;
                    }
                }
                --building_index;
                if (building_index<0) break;
            }
        }
    }
}


// Build-limit exhaustion, original004F4F30; reference names are hypotheses.
bool HouseClass::Is_Build_Limited_004F4F30(TechnoTypeClass *type)
{
    if (!type) return true;
    int queued=0;
    bool naval=type->Naval;
    FactoryClass *factory=unknown_func_4eb580(type->Kind_Of(),naval,0);
    if (factory) queued=factory->Count_Type_004B9E40(type);
    switch (type->Kind_Of()) {
    case 40:
        if (type->BuildLimit<=0) {
            if (queued+unknown_54b0.unknown_func_491d10(type->Fetch_Heap_ID())>=abs(type->BuildLimit)) return true;
        }
        if (type->BuildLimit>0) {
            if (queued+unknown_5430.unknown_func_491d10(type->Fetch_Heap_ID())>=type->BuildLimit) return true;
        }
        break;
    case 16: {
        if (type->BuildLimit<=0) {
            if (queued+unknown_54c0.unknown_func_491d10(type->Fetch_Heap_ID())>=abs(type->BuildLimit)) return true;
        }
        int owned=unknown_5440.unknown_func_491d10(type->Fetch_Heap_ID());
        if (((InfantryTypeClass *)type)->VehicleThief) {
            for (int i=0;i<Units.Count();++i) {
                UnitClass *unit=Units[i];
                if (unit->House==this && unit->unknown_290==type->Fetch_Heap_ID()) ++owned;
            }
        }
        if (type->BuildLimit>0) {
            if (queued+owned>=type->BuildLimit) return true;
        }
        break;
    }
    case 7:
        if (type->BuildLimit<=0) {
            if (queued+unknown_54a0.unknown_func_491d10(type->Fetch_Heap_ID())>=abs(type->BuildLimit)) return true;
        }
        if (type->BuildLimit>0) {
            if (queued+unknown_5420.unknown_func_491d10(type->Fetch_Heap_ID())>=type->BuildLimit) return true;
        }
        break;
    case 3:
        if (((AircraftTypeClass *)type)->AirportBound) {
            int owned=0,queued_aircraft=0;
            for (int i=0;i<Rules->PadAircraft.Count();++i) {
                owned+=unknown_5490.unknown_func_491d10(Rules->PadAircraft[i]->Fetch_Heap_ID());
                if (factory) queued_aircraft+=factory->Count_Type_004B9E40(Rules->PadAircraft[i]);
            }
            if (queued_aircraft+owned>=unknown_218) return true;
            break;
        }
        if (type->BuildLimit<=0) {
            if (queued+unknown_54d0.unknown_func_491d10(type->Fetch_Heap_ID())>=abs(type->BuildLimit)) return true;
        }
        if (type->BuildLimit>0) {
            if (queued+unknown_5450.unknown_func_491d10(type->Fetch_Heap_ID())>=type->BuildLimit) return true;
        }
        break;
    }
    return false;
}


// bool HouseClass::Is_Player_Control() (0x004F52B0)
// In a multiplayer game whether this is the local player's house, otherwise whether a human
// flag is set (hypothesis).

extern int unknown_game_mode_a3d298;        // 0x00A3D298

bool HouseClass::Is_Player_Control()
{
    if (!unknown_game_mode_a3d298) {
        return unknown_134 || unknown_135;
    }
    return this == unknown_player_a35db4;
}


// Existing query004F52F0; nested auto-inlining structure from reference and takeover call evidence.

bool HouseClass::Is_Human_Player()
{
    if (unknown_game_mode_a3d298==0) return Is_Player_Control();
    return unknown_134;
}


// bool HouseClass::unknown_func_4f5320(BuildingTypeClass *type, Cell const &cell) (0x004F5320)
// Outside the campaign: whether any cell of the type's foundation at the cell,
// widened by Rules AIBaseSpacing on every side, has this house's bit in its
// +0xDC house mask. The campaign always answers true.

bool HouseClass::unknown_func_4f5320(BuildingTypeClass *type, Cell const &cell)
{
    if (unknown_game_mode_a3d298 == 0) {
        return true;
    }
    int spacing = Rules->AIBaseSpacing;
    int width = type->Width() + spacing * 2;
    int height = type->Foundation_Height(false) + spacing * 2;
    int bit = 1 << ArrayIndex;
    for (int x = cell.X - spacing - 1; x < cell.X + width + 1; x++) {
        for (int y = cell.Y - spacing - 1; y < cell.Y + height + 1; y++) {
            if (Map[Cell(x, y)].unknown_dc & bit) {
                return true;
            }
        }
    }
    return false;
}


// void HouseClass::Update_Color() (0x004F5400)
// The house colour (+0x55E9) from its colour scheme's palette entry, unpacked
// from the screen pixel the scheme's converter gives it (name hypothesis).

void HouseClass::Update_Color()
{
    ColorScheme *scheme = ColorSchemes[ColorSchemeIndex];
    unknown_55e9 = Unpack_Hicolor_Pixel(scheme->Converter->Convert(scheme->unknown_color_0x330));
}


// void __fastcall Normalize_Color(double &red, double &green, double &blue) (0x004F5490)

void __fastcall Normalize_Color(double &red, double &green, double &blue)
{
    double length = unknown_sqrt_4ba410(red * red + green * green + blue * blue);
    if (length == 0.0) {
        blue = 255.0;
        green = 255.0;
        red = 255.0;
    } else {
        red = red * 240.0 / length;
        if (red > 255.0) {
            red = 255.0;
        }
        green = green * 240.0 / length;
        if (green > 255.0) {
            green = 255.0;
        }
        blue = blue * 240.0 / length;
        if (blue > 255.0) {
            blue = 255.0;
        }
    }
}


// void HouseClass::Update_Laser_Color() (0x004F5570)

void HouseClass::Update_Laser_Color()
{
    double red = unknown_55e9.Red;
    double green = unknown_55e9.Green;
    double blue = unknown_55e9.Blue;
    Normalize_Color(red, green, blue);
    if (red < 96.0) {
        red = 0;
    }
    if (green < 96.0) {
        green = 0;
    }
    if (blue < 96.0) {
        blue = 0;
    }
    Normalize_Color(red, green, blue);
    unknown_55ec.Set(red, green, blue);
}


// void HouseClass::unknown_func_4f5800(long delay) (0x004F5800)

void HouseClass::unknown_func_4f5800(long delay)
{
    unknown_1ec = CDTimerClass<FrameTimerClass>(delay);
    unknown_5668 = true;
}


// void HouseClass::unknown_func_4f5840(long delay) (0x004F5840)

void HouseClass::unknown_func_4f5840(long delay)
{
    unknown_1f8 = CDTimerClass<FrameTimerClass>(delay);
    unknown_5669 = true;
}


// void HouseClass::unknown_func_4f5880() (0x004F5880)
// The local player's house tells the map (0x00559F10) unless the +0x566A flag is set.

void HouseClass::unknown_func_4f5880()
{
    if (this == unknown_player_a35db4 && !unknown_566a) {
        Map.Unknown_00559F10();
    }
}


// float HouseClass::unknown_func_4f58a0(TechnoTypeClass *type) (0x004F58A0)
// The owner country's armor multiplier for the type's kind (infantry, vehicle,
// aircraft, building or, for a defence building (+0xBA0 == 5), defence); 1 otherwise.

float HouseClass::unknown_func_4f58a0(TechnoTypeClass *type)
{
    switch (type->Kind_Of()) {
    case 16:
        return Type->unknown_0x100;
    case 40:
        return Type->unknown_0x104;
    case 3:
        return Type->unknown_0x108;
    case 7:
        if (((UnknownBuildingTypeBA0View const *)type)->unknown_ba0 == 5) {
            return Type->unknown_0x110;
        }
        return Type->unknown_0x10C;
    }
    return 1.0f;
}


// float HouseClass::Cost_Multiplier(TechnoTypeClass const *type) (0x004F5960)
// The owner country's cost multiplier for the type's kind (infantry, vehicle,
// aircraft, building or, for a defence building (+0xBA0 == 5), defence); 1 otherwise.

float HouseClass::Cost_Multiplier(TechnoTypeClass const *type)
{
    switch (type->Kind_Of()) {
    case 16:
        return Type->unknown_0x114;
    case 40:
        return Type->unknown_0x118;
    case 3:
        return Type->unknown_0x11C;
    case 7:
        if (((UnknownBuildingTypeBA0View const *)type)->unknown_ba0 == 5) {
            return Type->unknown_0x124;
        }
        return Type->unknown_0x120;
    }
    return 1.0f;
}


// float HouseClass::unknown_func_4f5a20(TechnoTypeClass *type) (0x004F5A20)
// The owner country's multiplier for the type's kind (infantry, vehicle or
// aircraft; FootClass 0x004C9B10 reads it); 1 otherwise.

float HouseClass::unknown_func_4f5a20(TechnoTypeClass *type)
{
    switch (type->Kind_Of()) {
    case 16:
        return Type->unknown_0x128;
    case 40:
        return Type->unknown_0x12C;
    case 3:
        return Type->unknown_0x130;
    }
    return 1.0f;
}


// float HouseClass::unknown_func_4f5a70(TechnoTypeClass *type) (0x004F5A70)
// The owner country's build time multiplier for the type's kind (infantry, vehicle,
// aircraft, building or, for a defence building (+0xBA0 == 5), defence); 1 otherwise.

float HouseClass::unknown_func_4f5a70(TechnoTypeClass *type)
{
    switch (type->Kind_Of()) {
    case 16:
        return Type->unknown_0x134;
    case 40:
        return Type->unknown_0x138;
    case 3:
        return Type->unknown_0x13C;
    case 7:
        if (((UnknownBuildingTypeBA0View const *)type)->unknown_ba0 == 5) {
            return Type->unknown_0x144;
        }
        return Type->unknown_0x140;
    }
    return 1.0f;
}


// 0x004f5b30: function not yet identified
//
// Body: 10 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: float10 __fastcall unknown_type_float_148_004f5b30(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004f5b30 {

long double __fastcall unknown_type_float_148_004f5b30(int house)
{
    return (long double)*(float *)(*(int *)(house + 0x34) + 0x148);
}

}  // namespace seed_004f5b30


// int __fastcall HouseClass::Find_Index(char const *name) (0x004F5B40)
// The index in Houses of the house whose INI name is name, or -1.

int __fastcall HouseClass::Find_Index(char const *name)
{
    for (int index = 0; index < Houses.Count(); index++) {
        char buffer[20];
        strcpy(buffer, Houses[index]->IniName);
        if (strcmp(buffer, name) == 0) {
            return index;
        }
    }
    return -1;
}


// Begin construction from an existing construction yard, original004F5BE0;328body+8padding.

void HouseClass::unknown_func_4f5be0()
{
    BuildingClass *construction_yard=0;
    for (int index=Buildings.Count()-1;index>=0;--index) {
        BuildingClass *building=Buildings[index];
        bool is_construction_yard=false;
        for (int type_index=0;type_index<Rules->unknown_7c8.Count();++type_index) {
            if (Rules->unknown_7c8[type_index]==building->Type) {
                is_construction_yard=true;
                break;
            }
        }
        if (is_construction_yard && !building->unknown_76) {
            construction_yard=building;
            break;
        }
    }
    if (!construction_yard) return;
    CoordStruct position=construction_yard->Position_Coord_004f3f40();
    Cell center=Coord_Cell(position);
    BaseCenter=Cell_Coord(center);
    if (!Is_Human_Control() && unknown_game_mode_a3d298!=0) unknown_func_4ec5e0();
    if (unknown_55f0.Nodes.Count()==0) {
        int saved_initialization=unknown_a40774;
        unknown_a40774=0;
        unknown_func_4ef890();
        unknown_a40774=saved_initialization;
    }
    unknown_55f0.Nodes[0].Cell=center;
    unknown_55f0.unknown_50=center;
}


// bool HouseClass::unknown_func_4f5d30(int node) (0x004F5D30)
// Walls in a planned building: finds the house side's concrete wall type (Rules
// ConcreteWalls, AIBasePlanningSide), walks the base nodes back from `node` to the
// last built building that wants a wall (ProtectWithWall) and is not already followed
// by a wall node, then inserts wall nodes after it around its foundation: both long
// sides, both short sides, then the four corners. Answers whether it planned any.

bool HouseClass::unknown_func_4f5d30(int node)
{
    int wall = -1;
    for (int i = 0; i < Rules->ConcreteWalls.Count(); i++) {
        if (Rules->ConcreteWalls[i]->AIBasePlanningSide == Type->Side || Rules->ConcreteWalls[i]->AIBasePlanningSide == -1) {
            wall = Rules->ConcreteWalls[i]->ArrayIndex;
            break;
        }
    }

    BuildingClass *building = 0;
    int index;
    for (index = node - 1; index >= 0; index--) {
        building = unknown_55f0.Get_Building(index);
        if (building && building->Type->ProtectWithWall && unknown_55f0.Nodes[index + 1].Type != wall) break;
    }

    if (index == -1 || !building || !building->Type->ProtectWithWall) {
        return false;
    }

    CoordStruct coord = building->Coord;
    Cell cell(coord.X / 256, coord.Y / 256);
    BuildingTypeClass *type = building->Type;
    int width = type->Width();
    int height = type->Foundation_Height(false);
    Cell corner = cell - Cell(1, 1);

    for (node = 1; node <= width; node++) {
        unknown_55f0.Nodes.Insert_After(BaseNodeClass(wall, corner + Cell(node, 0)), index);
        unknown_55f0.Nodes.Insert_After(BaseNodeClass(wall, corner + Cell(node, height + 1)), index);
    }
    for (node = 1; node <= height; node++) {
        unknown_55f0.Nodes.Insert_After(BaseNodeClass(wall, corner + Cell(0, node)), index);
        unknown_55f0.Nodes.Insert_After(BaseNodeClass(wall, corner + Cell(width + 1, node)), index);
    }

    unknown_55f0.Nodes.Insert_After(BaseNodeClass(wall, corner + Cell(0, 0)), index);
    unknown_55f0.Nodes.Insert_After(BaseNodeClass(wall, corner + Cell(width + 1, 0)), index);
    unknown_55f0.Nodes.Insert_After(BaseNodeClass(wall, corner + Cell(0, height + 1)), index);
    unknown_55f0.Nodes.Insert_After(BaseNodeClass(wall, corner + Cell(width + 1, height + 1)), index);
    return true;
}


// void HouseClass::Cheer() (0x004F62B0)
// Every techno of the house cheers (TechnoClass slot 205), then Rules'
// CheerSound plays (name hypothesis).

void HouseClass::Cheer()
{
    int count = Technos.Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && techno->House == this) {
            techno->unknown_slot205();
        }
    }
    Voc_Play(Rules->CheerSound, 1.0f, 0x2000, 0);
}


// Rally eligible owned technos near the base, original004F6310;419body+13padding.
// Inventory129-byte extent ends at a returning Map call; original CFG proves the complete419-byte body.

void HouseClass::Unknown_004F6310()
{
    int count=Technos.Count();
    Cell destination=Coord_Cell(BaseCenter);
    destination=Map.Nearby_Location(destination,0,-1,0,false,Point2D(4,4),
                                         0,0,0,1,Cell(0,0),0);
    if (destination!=unknown_cell_a40f10) {
        ++destination.X;
        ++destination.Y;
        for (int index=0;index<count;++index) {
            TechnoClass *techno=Technos[index];
            if (techno && techno->House==this && techno->Kind_Of()!=6 && techno->Kind_Of()!=2 &&
                (!techno->Techno_Type_Class()->DeploysInto ||
                 !techno->Techno_Type_Class()->DeploysInto->ConstructionYard) &&
                Coord_Cell(BaseCenter)!=unknown_cell_a40f10) {
                techno->Assign_Destination(&Map[destination],true);
                techno->unknown_6d6e50(0);
                techno->QueueMission(2,false);
            }
        }
    }
}


// House node readiness, original004F64C0. Original source-independent extent224+0.
bool HouseClass::unknown_func_4f64c0(BaseNodeClass const &node)
{
    int type_index=node.Type;
    if (unknown_game_mode_a3d298==0) return true;
    if (type_index<0) return true;
    if (type_index>=BuildingTypes.Count()) return false;
    BuildingTypeClass *type=BuildingTypes[type_index];
    if (!type) return false;
    if (type->Get_Weapon(0)->WeaponType) return true;
    if (type->Wall) {
        for (int facing=0;facing<8;++facing) {
            CellClass &cell=Map[node.Cell];
            TechnoClass *building=(TechnoClass *)cell.Adjacent_Cell(facing).Cell_Building();
            if (building && building->House==this) return true;
        }
        return false;
    }
    if (type->Power>0) return true;
    if (unknown_5408+Rules->AIRestrictReplaceTime<=Frame) return true;
    return false;
}


// House super-weapon target cell, original004F65A0. Independently authored from target flow.
Cell HouseClass::unknown_func_4f65a0()
{
    HouseClass *enemy=Houses[unknown_54f0];
    DynamicVectorClass<AbstractClass *> targets;
    int best=0;
    for (int i=0;i<Technos.Count();++i) {
        int value=0;
        bool valid=false;
        TechnoClass *techno=Technos[i];
        if (techno->House==enemy) {
            value=1;
            if (techno->In_Which_Layer()==2 && techno->unknown_7d && !techno->unknown_76) valid=true;
            else if (unknown_c8==0) {
                for (int j=0;j<Factories.Count();++j) {
                    FactoryClass *factory=Factories[j];
                    if (factory->Get_Object()==techno && factory->Rate && !factory->unknown_6c) valid=true;
                }
            }
            switch (techno->Kind_Of()) {
            case 15: {
                InfantryTypeClass *type=((InfantryClass *)techno)->Type;
                if (type->Engineer) value=Rules->AIIonCannonEngineerValue[unknown_c8];
                else if (type->VehicleThief) value=Rules->AIIonCannonThiefValue[unknown_c8];
                else value=2;
                break;
            }
            case 6: {
                BuildingTypeClass *type=((BuildingClass *)techno)->Type;
                if (type->Factory==7) value=Rules->AIIonCannonConYardValue[unknown_c8];
                else if (type->Factory==40) value=Rules->AIIonCannonWarFactoryValue[unknown_c8];
                else if (type->Power>type->PowerDrain_c6c) value=Rules->AIIonCannonPowerValue[unknown_c8];
                else if (type->IsBaseDefense) value=Rules->AIIonCannonBaseDefenseValue[unknown_c8];
                else if (type->IsPlug) value=Rules->AIIonCannonPlugValue[unknown_c8];
                else if (type->IsTemple) value=Rules->AIIonCannonTempleValue[unknown_c8];
                else if (type->HoverPad) value=Rules->AIIonCannonHelipadValue[unknown_c8];
                else value=4;
                break;
            }
            case 1: {
                UnitTypeClass *type=((UnitClass *)techno)->Type;
                if (type->Harvester) value=Rules->AIIonCannonHarvesterValue[unknown_c8];
                else {
                    for (int j=0;j<Rules->unknown_7c8.Count();++j) {
                        if (Rules->unknown_7c8[j]==type->DeploysInto) {
                            value=Rules->AIIonCannonMCVValue[unknown_c8];
                            goto CLASSIFIED;
                        }
                    }
                    if (type->Passengers>0) value=Rules->AIIonCannonAPCValue[unknown_c8];
                    else value=2;
                }
                break;
            }
            }
        }
CLASSIFIED:
        if (techno->unknown_1b8==2 || (techno->Kind_Of()==6 && ((BuildingClass *)techno)->unknown_5bd==15)) value=Random_Pick(0,best+10);
        if (valid) {
            if (value>best) {
                targets.Clear();
                targets.Add(techno);
                best=value;
            } else if (value==best) targets.Add(techno);
        }
    }
    if (targets.Count()>0) {
        AbstractClass *target=targets[Random_Pick(0,targets.Count()-1)];
        if (target) return Coord_Cell(target->Center_Coord());
    }
    return unknown_cell_a40f10;
}


// Cell HouseClass::unknown_func_4f6a80(int mode) (0x004F6A80)
// The cell under what the leader of the house's first team would attack for a
// target mode (0x006220E0 gives the threat flags): slot 219 of the leader with its
// own center coordinate; the invalid cell without a team or a target.

int __fastcall unknown_threat_flags_6220e0(int mode);   // 0x006220E0

Cell HouseClass::unknown_func_4f6a80(int mode)
{
    int flags = unknown_threat_flags_6220e0(mode);
    TeamClass *team = 0;
    for (int index = 0; index < Teams.Count(); index++) {
        team = Teams[index];
        if (team->Owner == this) {
            break;
        }
    }
    if (team != 0 && team->Owner == this) {
        FootClass *leader = team->unknown_func_6baa60();
        AbstractClass *target = leader->unknown_slot219(flags, &leader->Center_Coord(), true);
        if (target != 0) {
            return Coord_Cell(target->Center_Coord());
        }
        return unknown_cell_a40f10;
    }
    return unknown_cell_a40f10;
}


// unsigned long __stdcall HouseClass::AddRef() (0x004F6B60)

unsigned long __stdcall HouseClass::AddRef()
{
    return 1;
}


// unsigned long __stdcall HouseClass::Release() (0x004F6B70)

unsigned long __stdcall HouseClass::Release()
{
    return 1;
}


// int HouseClass::Kind_Of() const (0x004F6B80)

int HouseClass::Kind_Of() const
{
    return 13;
}


// int HouseClass::Fetch_Heap_ID() const (0x004F6B90)

int HouseClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// HouseClass scalar deleting destructor (0x004F6BA0): compiler generated; no source of its own.


// void RGBClass::Set(unsigned char red, unsigned char green, unsigned char blue) (0x004F6BC0)
// An inline member of shared/rgb_class.hpp: House.cpp emits its out-of-line copy (called by
// HouseClass::Read_INI) after its own functions; no source of its own.


// BaseNodeClass::BaseNodeClass(int type, Cell const &cell) (0x004F6BE0)
// A base plan node for building type `type` at `cell`, not yet built and with no attempts. The base
// planner 0x004F5D30 inlines it until its /Ob2 budget runs out and calls this copy for the last three
// corners of a wall ring.
BaseNodeClass::BaseNodeClass(int type, ::Cell const &cell)
{
    Type = type;
    Cell = cell;
    unknown_08 = false;
    unknown_0c = 0;
}


// VectorClass_BuildingTypeDistribution_scalar_deleting_destructor, original0x004f8800.
// Compiler generated by the ordinary weighted-building vector constructor and element lifetime in this unit.


// BuildingTypeDistribution_vector_deleting_destructor, original0x004f8860.
// Compiler generated by the ordinary weighted-building vector constructor and element lifetime in this unit.


// DynamicVectorClass<DiscreteDistributionClass<BuildingTypeClass>::DistributionObject<BuildingTypeClass *> > deleting destructor (original0x004F88A0).
// Compiler generated by the accepted weighted-building distribution lifetime in HouseClass::AI_Build_Defense; no separate implementation.


extern "C" void __cdecl release_4f6e00(void*);
class Cleanup_4f6e00 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_4f6e00::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_4f6e00(pointer); items=0; } owns=0; count=0; }


// 0x004f6e30: function not yet identified
//
// Owner: DynamicVectorClass<bool>, virtual slot 5 (vtable +0x14) (from RTTI/vtables).
// Body: 24 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int __thiscall unknown_offset_of_004f6e30(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004f6e30 {

struct Owner_004f6e30 { int unknown_offset_of_004f6e30(int item); };

int Owner_004f6e30::unknown_offset_of_004f6e30(int item)
{
    int self = (int)this;
    if (*(char *)(self + 0xc) == '\0') {
        return 0;
    }
    return item - *(int *)(self + 4);
}

}  // namespace seed_004f6e30


extern "C" unsigned char vtable_4f6ee0[]; extern "C" void __cdecl release_buffer_4f6ee0(void*); extern "C" void __cdecl release_object_4f6ee0(void*);
class Leaf_4f6ee0 { public: void* get(unsigned int flags); };
void* Leaf_4f6ee0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f6ee0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f6ee0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f6ee0(this); return this; }


// 0x004f6fc0: function not yet identified
//
// Owner: DynamicVectorClass<TeamTypeClass *>, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 73 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_004f6fc0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004f6fc0 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* VectorClass_TeamTypeClass_vtable_007a2ee4;

struct Owner_004f6fc0 { unsigned int * scalar_deleting_destructor_004f6fc0(unsigned char flags); };

unsigned int * Owner_004f6fc0::scalar_deleting_destructor_004f6fc0(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&VectorClass_TeamTypeClass_vtable_007a2ee4);
    if ((self[1] != 0) && (*(char *)((int)self + 0xd) != '\0')) {
        operator_delete_00781d3b((unsigned int)(self[1]));
        self[1] = 0;
    }
    *(unsigned char *)((int)self + 0xd) = 0;
    self[2] = 0;
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_004f6fc0


// 0x004f83e0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_empty_004f83e0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004f83e0 {

void  unknown_empty_004f83e0(void)
{
    return;
}

}  // namespace seed_004f83e0


// 0x004f8450: function not yet identified
//
// Owner: DynamicVectorClass<DiscreteDistributionClass<BuildingTypeClass>::DistributionObject<BuildingTypeClass *>>, virtual slot 4 (vtable +0x10) (from RTTI/vtables).
// Body: 54 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int __thiscall ID_004f8450(int param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004f8450 {

struct Owner_004f8450 { int ID_004f8450(int *item); };

int Owner_004f8450::ID_004f8450(int *item)
{
    int self = (int)this;
    int index;
    int *entry;

    index = (int)(0);
    if (0 < *(int *)(self + 0x10)) {
        entry = (int *)(*(int **)(self + 4));
        do {
            if ((*entry == *item) && (entry[1] == item[1])) {
                return index;
            }
            index = (int)(index + 1);
            entry = (int *)(entry + 2);
        } while (index < *(int *)(self + 0x10));
    }
    return -1;
}

}  // namespace seed_004f8450


extern "C" unsigned char vtable_4f8490[]; extern "C" void __cdecl release_buffer_4f8490(void*); extern "C" void __cdecl release_object_4f8490(void*);
class Leaf_4f8490 { public: void* get(unsigned int flags); };
void* Leaf_4f8490::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f8490; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f8490((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f8490(this); return this; }


extern "C" unsigned char vtable_4f84e0[]; extern "C" void __cdecl release_buffer_4f84e0(void*); extern "C" void __cdecl release_object_4f84e0(void*);
class Leaf_4f84e0 { public: void* get(unsigned int flags); };
void* Leaf_4f84e0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f84e0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f84e0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f84e0(this); return this; }


extern "C" unsigned char vtable_4f8530[]; extern "C" void __cdecl release_buffer_4f8530(void*); extern "C" void __cdecl release_object_4f8530(void*);
class Leaf_4f8530 { public: void* get(unsigned int flags); };
void* Leaf_4f8530::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f8530; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f8530((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f8530(this); return this; }


extern "C" unsigned char vtable_4f8580[]; extern "C" void __cdecl release_buffer_4f8580(void*); extern "C" void __cdecl release_object_4f8580(void*);
class Leaf_4f8580 { public: void* get(unsigned int flags); };
void* Leaf_4f8580::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f8580; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f8580((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f8580(this); return this; }


extern "C" unsigned char vtable_4f85d0[]; extern "C" void __cdecl release_buffer_4f85d0(void*); extern "C" void __cdecl release_object_4f85d0(void*);
class Leaf_4f85d0 { public: void* get(unsigned int flags); };
void* Leaf_4f85d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f85d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f85d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f85d0(this); return this; }


extern "C" unsigned char vtable_4f8620[]; extern "C" void __cdecl release_buffer_4f8620(void*); extern "C" void __cdecl release_object_4f8620(void*);
class Leaf_4f8620 { public: void* get(unsigned int flags); };
void* Leaf_4f8620::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f8620; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f8620((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f8620(this); return this; }


extern "C" unsigned char vtable_4f8670[]; extern "C" void __cdecl release_buffer_4f8670(void*); extern "C" void __cdecl release_object_4f8670(void*);
class Leaf_4f8670 { public: void* get(unsigned int flags); };
void* Leaf_4f8670::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f8670; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f8670((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f8670(this); return this; }


extern "C" unsigned char vtable_4f86c0[]; extern "C" void __cdecl release_buffer_4f86c0(void*); extern "C" void __cdecl release_object_4f86c0(void*);
class Leaf_4f86c0 { public: void* get(unsigned int flags); };
void* Leaf_4f86c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f86c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f86c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f86c0(this); return this; }


extern "C" unsigned char vtable_4f8710[]; extern "C" void __cdecl release_buffer_4f8710(void*); extern "C" void __cdecl release_object_4f8710(void*);
class Leaf_4f8710 { public: void* get(unsigned int flags); };
void* Leaf_4f8710::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f8710; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f8710((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f8710(this); return this; }


extern "C" unsigned char vtable_4f8760[]; extern "C" void __cdecl release_buffer_4f8760(void*); extern "C" void __cdecl release_object_4f8760(void*);
class Leaf_4f8760 { public: void* get(unsigned int flags); };
void* Leaf_4f8760::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f8760; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f8760((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f8760(this); return this; }


extern "C" unsigned char vtable_4f87b0[]; extern "C" void __cdecl release_buffer_4f87b0(void*); extern "C" void __cdecl release_object_4f87b0(void*);
class Leaf_4f87b0 { public: void* get(unsigned int flags); };
void* Leaf_4f87b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4f87b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4f87b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4f87b0(this); return this; }


extern "C" unsigned char table_4f8900[]; extern "C" void* __cdecl storage_4f8900(unsigned int);
class Storage_4f8900 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4f8900::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4f8900; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4f8900(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_4f8950[]; extern "C" void* __cdecl storage_4f8950(unsigned int);
class Storage_4f8950 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4f8950::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4f8950; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4f8950(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_4f89f0[]; extern "C" void* __cdecl storage_4f89f0(unsigned int);
class Storage_4f89f0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4f89f0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4f89f0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4f89f0(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_4f8a40[]; extern "C" void* __cdecl storage_4f8a40(unsigned int);
class Storage_4f8a40 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4f8a40::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4f8a40; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4f8a40(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_4f8a90[]; extern "C" void* __cdecl storage_4f8a90(unsigned int);
class Storage_4f8a90 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4f8a90::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4f8a90; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4f8a90(requested << 2); owns=1; } return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<BuildingTypeClass *>::Clear (0x004F6C00; DynamicVectorClass<BuildingTypeClass *> vtable 0x007a2f28 slot 3).
// DynamicVectorClass<BuildingTypeClass *>::ID_pointer (0x004F6C30; DynamicVectorClass<BuildingTypeClass *> vtable 0x007a2f28 slot 5).
// VectorClass<BuildingTypeClass *>::Fetch (0x004F6C50; VectorClass<BuildingTypeClass *> vtable 0x007a2f08 slot 6).
// DynamicVectorClass<AngerStruct>::Clear (0x004F6C60; DynamicVectorClass<AngerStruct> vtable 0x007a2e64 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<AngerStruct>::ID_pointer (0x004F6C90; DynamicVectorClass<AngerStruct> vtable 0x007a2e64 slot 5).
// AngerStruct is a size-only stand-in (8 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<AngerStruct>::Fetch (0x004F6CB0; VectorClass<AngerStruct> vtable 0x007a2e84 slot 6).
// AngerStruct: shared/house_nodes.hpp.
// DynamicVectorClass<ScoutStruct>::Clear (0x004F6CD0; DynamicVectorClass<ScoutStruct> vtable 0x007a2e44 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<ScoutStruct>::ID_pointer (0x004F6D00; DynamicVectorClass<ScoutStruct> vtable 0x007a2e44 slot 5).
// ScoutStruct is a size-only stand-in (8 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<ScoutStruct>::Fetch (0x004F6D20; VectorClass<ScoutStruct> vtable 0x007a2e64 slot 6).
// ScoutStruct: shared/house_nodes.hpp.
// VectorClass<bool>::Fetch (0x004F6E50; VectorClass<bool> vtable 0x007a2f64 slot 6).
// VectorClass<BuildingTypeClass *>::operator equal (0x004F7010; VectorClass<BuildingTypeClass *> vtable 0x007a2f08 slot 1).
// VectorClass<BuildingTypeClass *>::Resize (0x004F7050; VectorClass<BuildingTypeClass *> vtable 0x007a2f08 slot 2).
// VectorClass<BuildingTypeClass *>::Clear (0x004F7100; VectorClass<BuildingTypeClass *> vtable 0x007a2f08 slot 3).
// VectorClass<BuildingTypeClass *>::ID_object (0x004F7130; VectorClass<BuildingTypeClass *> vtable 0x007a2f08 slot 4).
// VectorClass<BuildingTypeClass *>::ID_pointer (0x004F7170; VectorClass<BuildingTypeClass *> vtable 0x007a2f08 slot 5).
// DynamicVectorClass<BuildingTypeClass *>::Resize (0x004F7190; DynamicVectorClass<BuildingTypeClass *> vtable 0x007a2f28 slot 2).
// DynamicVectorClass<BuildingTypeClass *>::ID_object (0x004F7240; DynamicVectorClass<BuildingTypeClass *> vtable 0x007a2f28 slot 4).
// VectorClass<AngerStruct>::operator equal (0x004F7320).
// AngerStruct: shared/house_nodes.hpp.
// VectorClass<AngerStruct>::Resize (0x004F7380).
// AngerStruct: shared/house_nodes.hpp.
// VectorClass<AngerStruct>::Clear (0x004F7470; VectorClass<AngerStruct> vtable 0x007a2e84 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<AngerStruct>::ID_object (0x004F74A0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; AngerStruct comparison: shared/house_nodes.hpp.
// VectorClass<AngerStruct>::ID_pointer (0x004F74F0; VectorClass<AngerStruct> vtable 0x007a2e84 slot 5).
// AngerStruct is a size-only stand-in (8 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// DynamicVectorClass<AngerStruct>::Resize (0x004F7510).
// AngerStruct: shared/house_nodes.hpp.
// DynamicVectorClass<AngerStruct>::ID_object (0x004F7610; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; AngerStruct comparison: shared/house_nodes.hpp.
// VectorClass<ScoutStruct>::operator equal (0x004F7650).
// ScoutStruct: shared/house_nodes.hpp.
// VectorClass<ScoutStruct>::Resize (0x004F76B0).
// ScoutStruct: shared/house_nodes.hpp.
// VectorClass<ScoutStruct>::Clear (0x004F77A0; VectorClass<ScoutStruct> vtable 0x007a2ea4 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<ScoutStruct>::ID_object (0x004F77D0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; ScoutStruct comparison: shared/house_nodes.hpp.
// VectorClass<ScoutStruct>::ID_pointer (0x004F7820; VectorClass<ScoutStruct> vtable 0x007a2ea4 slot 5).
// ScoutStruct is a size-only stand-in (8 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// DynamicVectorClass<ScoutStruct>::Resize (0x004F7840).
// ScoutStruct: shared/house_nodes.hpp.
// DynamicVectorClass<ScoutStruct>::ID_object (0x004F7940; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; ScoutStruct comparison: shared/house_nodes.hpp.
// VectorClass<bool>::operator equal (0x004F7E40; VectorClass<bool> vtable 0x007a2f64 slot 1).
// VectorClass<bool>::Resize (0x004F7E80; VectorClass<bool> vtable 0x007a2f64 slot 2).
// VectorClass<bool>::Clear (0x004F7F30; VectorClass<bool> vtable 0x007a2f64 slot 3).
// VectorClass<bool>::ID_object (0x004F7F60; VectorClass<bool> vtable 0x007a2f64 slot 4).
// VectorClass<bool>::ID_pointer (0x004F7FA0; VectorClass<bool> vtable 0x007a2f64 slot 5).
// DynamicVectorClass<bool>::Resize (0x004F7FC0).
// DynamicVectorClass<bool>::ID object (0x004F8070).
// Real original RTTI: VectorClass<BuildingTypeClass const *> at007A2F08.
// DynamicVectorClass<HouseClass *>::Delete (index; 0x004F8C20).


// bool UnknownGrowArray<UnknownGrowElement8>::Grow(int amount) (0x004F8C60)
// See shared/unknown_grow_array.hpp.

template struct UnknownGrowArray<UnknownGrowElement8>;


// Weighted-building candidate storage; original004F8D70, RTTI007A2FAC.
class BuildingTypeClass;
typedef DiscreteDistributionClass<BuildingTypeClass>::DistributionObject<BuildingTypeClass *> HouseBuildingDistributionElement;
template <> VectorClass<HouseBuildingDistributionElement>::VectorClass(unsigned size,HouseBuildingDistributionElement const *array)
    : Vector(0),VectorMax(size),IsValid(true),IsAllocated(false)
{
    if (size) {
        if (array) Vector=new ((void *)array) HouseBuildingDistributionElement[size];
        else {
            Vector=new HouseBuildingDistributionElement[size];
            IsAllocated=true;
        }
    }
}


// 0x004f8cf0: function not yet identified
//
// Body: 34 bytes (frozen inventory bound).
// Referenced from 2 sites (data).
// Ghidra prototype: int unknown_compare_004f8cf0(int *param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_004f8cf0 {

int  unknown_compare_004f8cf0(int *a, int *b)
{
    if (*a == *b) {
        return 0;
    }
    return ((*a >= *b) - 1 & 0xfffffffe) + 1;
}

}  // namespace seed_004f8cf0


// HouseClass::QueryInterface adjustor thunk for IRTTITypeInfo (0x004F8E20)
// Compiler generated with the HouseClass vtables (this - 4, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::AddRef adjustor thunk for IRTTITypeInfo (0x004F8E30)
// Compiler generated with the HouseClass vtables (this - 4, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::Release adjustor thunk for IRTTITypeInfo (0x004F8E40)
// Compiler generated with the HouseClass vtables (this - 4, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::QueryInterface adjustor thunk for IHouse (0x004F8E50)
// Compiler generated with the HouseClass vtables (this - 0x24, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::AddRef adjustor thunk for IHouse (0x004F8E60)
// Compiler generated with the HouseClass vtables (this - 0x24, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::Release adjustor thunk for IHouse (0x004F8E70)
// Compiler generated with the HouseClass vtables (this - 0x24, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::QueryInterface adjustor thunk for IPublicHouse (0x004F8E80)
// Compiler generated with the HouseClass vtables (this - 0x28, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::AddRef adjustor thunk for IPublicHouse (0x004F8E90)
// Compiler generated with the HouseClass vtables (this - 0x28, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::Release adjustor thunk for IPublicHouse (0x004F8EA0)
// Compiler generated with the HouseClass vtables (this - 0x28, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::ID_Number adjustor thunk for IPublicHouse vtable (this - 4) (0x004F8EB0)
// Compiler generated with the HouseClass vtables (this - 4, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::Name adjustor thunk for IPublicHouse vtable (this - 4) (0x004F8EC0)
// Compiler generated with the HouseClass vtables (this - 4, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::QueryInterface adjustor thunk for IConnectionPointContainer (0x004F8ED0)
// Compiler generated with the HouseClass vtables (this - 0x2C, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::AddRef adjustor thunk for IConnectionPointContainer (0x004F8EE0)
// Compiler generated with the HouseClass vtables (this - 0x2C, then jump to the member).
// No source of its own: emitted by this unit (population-2537).


// HouseClass::Release adjustor thunk for IConnectionPointContainer (0x004F8EF0)
// Compiler generated with the HouseClass vtables (this - 0x2C, then jump to the member).
// No source of its own: emitted by this unit (population-2537).
