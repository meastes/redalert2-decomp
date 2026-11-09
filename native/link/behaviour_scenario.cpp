// Behaviour-written bodies of unmatched rows the skirmish harness reaches while
// the scenario loads (Scout1089; ra2-native --skirmish, Read_Scenario_INI and
// after). Each follows the evidence named with it (the row's seed, TibSun, the
// accepted callers). NOT EXACT: every row stays unrecovered in the exact tree;
// never original code or bytes.
#include "shared/isometric_tile_type.hpp"
#include "shared/veinhole.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include <cstddef>

// The arm64 offsets unit.hpp's house views are respelled to (port/fixups.json).
static_assert(offsetof(HouseClass, Type) == 0x58, "unit.hpp view: HouseClass::Type");
static_assert(offsetof(HouseClass, unknown_207) == 0x29B, "unit.hpp view: HouseClass::unknown_207");
static_assert(offsetof(HouseTypeClass, VeteranUnits) == 0x188, "unit.hpp view: HouseTypeClass::VeteranUnits");
// The arm64 offsets house_screen.hpp's light HouseClass is respelled to (check_light_views_house.cpp).
static_assert(offsetof(HouseClass, ArrayIndex) == 0x50 && offsetof(HouseClass, Buildings) == 0xA0, "house_screen.hpp view");
static_assert(offsetof(HouseClass, unknown_130) == 0x188 && offsetof(HouseClass, unknown_13d) == 0x195, "house_screen.hpp view");
static_assert(offsetof(HouseClass, unknown_142) == 0x19A && offsetof(HouseClass, WaypointPaths) == 0x1B0, "house_screen.hpp view");
static_assert(offsetof(HouseClass, Supers) == 0x228 && offsetof(HouseClass, CurBuildings) == 0x2C4, "house_screen.hpp view");
static_assert(offsetof(HouseClass, Power) == 0x5364 && offsetof(HouseClass, unknown_5310) == 0x53D4, "house_screen.hpp view");
static_assert(offsetof(HouseClass, BaseCenter) == 0x5480 && offsetof(HouseClass, unknown_5414) == 0x54E0, "house_screen.hpp view");
static_assert(offsetof(HouseClass, unknown_56d0) == 0x58A0 && offsetof(HouseClass, IniName) == 0x160B0, "house_screen.hpp view");
static_assert(offsetof(HouseClass, UIName) == 0x160E6 && offsetof(HouseClass, ColorSchemeIndex) == 0x16110, "house_screen.hpp view");
static_assert(sizeof(AbstractClass) == 0x38, "house_screen.hpp As_IHouse: IHouse, the second base, follows the 0x38-byte AbstractClass");

// 0x0052B6F0 (the row's seed src/recovered/0052b6f0.cpp): the image (slot 39,
// +0x9C) cell record's +0x2A byte for a cell index (taken modulo the image's
// cell count), or 0 without an image or a record at that cell.
char IsometricTileTypeClass::unknown_func_52b6f0(int index)
{
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)unknown_slot39();
    if (image != 0) {
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[index % (image->unknown_00[1] * image->unknown_00[0])];
        if (record != 0) {
            return record->unknown_2a;
        }
    }
    return 0;
}

extern int unknown_int_8325d0;   // 0x008325D0 MapSize.Width (member alias of Map)

// 0x00428A90 (the row's seed src/recovered/00428a90.cpp, __fastcall): the
// linear index of a cell in the map rectangle's packed rows:
// (X - W - 1 + Y) * W + ((X - Y - 1 + W) >> 1), W = the map width.
int __fastcall unknown_cell_index_428a90(Cell const &cell)
{
    int width = unknown_int_8325d0;
    return (cell.X - width - 1 + cell.Y) * width + ((cell.X - cell.Y - 1 + width) >> 1);
}

// MSVC's RTTI comparison, as Process_Deferred_Deletion 0x006ECF10 declares the
// class (the VC6 CRT's type_info::operator==, MSVCRT import in the original):
// natively typeid yields the Itanium type_info objects, so the comparison is
// libc++'s std::type_info equality on the same objects.
#include <typeinfo>
class type_info {
public:
    virtual ~type_info();
    int operator==(type_info const &rhs) const;
    int operator!=(type_info const &rhs) const;
private:
    void *_m_data;
    char _m_d_name[1];
};

int type_info::operator==(type_info const &rhs) const
{
    return *reinterpret_cast<std::type_info const *>(this) == *reinterpret_cast<std::type_info const *>(&rhs);
}

int type_info::operator!=(type_info const &rhs) const
{
    return !(*this == rhs);
}

// --- Rows the scenario's object placement reaches (UnitClass::Read_INI and on) ---
#include "shared/techno.hpp"
#include "shared/bounce.hpp"
#include "shared/object.hpp"
#include "shared/rules.hpp"

// 0x00496B50 (seed src/recovered/00496b50.cpp): neither +0x18 (IsActive) nor
// +0x19 (IsReverse) set.
bool UnknownTimed::unknown_496b50() const
{
    return !IsActive && !IsReverse;
}

// 0x0051FC80 (leaf src/recovered/0051fc80.cpp): returns false.
bool HouseClass::Unknown_0051FC80()
{
    return false;
}

// 0x00622160 UnknownQuaternion(x, y, z, w): the four floats in order (as the
// accepted Quaternion::Quaternion at the same address, src/recovered/classes/
// Quaternion/Quaternion_constructor_00622160.cpp, under bounce.hpp's name).
UnknownQuaternion::UnknownQuaternion(float x, float y, float z, float w)
{
    X = x;
    Y = y;
    Z = z;
    W = w;
}

// 0x00423E00, ObjectClass slot 101 (TibSun object.h Can_Enter_Cell): MOVE_OK (0).
int ObjectClass::unknown_slot101(CellClass *arg1, int arg2, int arg3, intptr_t arg4, bool arg5)   // arg4 pointer-width natively (native_units.py)
{
    return 0;
}

extern unsigned int unknown_detail_frame_rate_7dfb34;   // 0x007DFB34 (LogicClass 0x0053D840 reads it)
extern unsigned int unknown_detail_buffer_zone_7dfb38;  // 0x007DFB38

// 0x0053D820 / 0x0053D830 (leaves src/recovered/0053d820.cpp, 0053d830.cpp,
// __fastcall): store the argument at 0x007DFB34 / 0x007DFB38.
void __fastcall unknown_set_frame_rate_0053d820(int rate)
{
    unknown_detail_frame_rate_7dfb34 = rate;
}

void __fastcall unknown_set_buffer_zone_0053d830(int width)
{
    unknown_detail_buffer_zone_7dfb38 = width;
}

