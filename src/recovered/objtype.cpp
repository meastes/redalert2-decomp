// objtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/objtype.json carries the addresses and the rows that
// stay in their own files.


#define OBJECT_TYPE_DECLARE_OBJECT_TYPES
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/object_type.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/techno.hpp"
#include "shared/building_type_class.hpp"
#include "shared/crt_string.hpp"
#define SHAPE_FILE_EMPTY_RECT Rect(0, 0, 0, 0)
#include "shared/shape_file.hpp"
#include "shared/ccini_class.hpp"
#include "shared/rgb_class.hpp"


// Header-static dynamic initializers of the unit at 0x005D5560:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x005D5560-0x005D5746); the other
// drafts of this plan are emitted by these includes.
// The unit declares ObjectTypeClass::ObjectTypes (shared/object_type.hpp).


// Dynamic initializer (0x005D5590, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D55B0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D55D0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D55F0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D5610, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D5630, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D5670, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D56A0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D56D0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D5700, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D5710, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer (0x005D5730, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_005d5560.cpp; no source of its own.


// Dynamic initializer of ObjectTypeClass::ObjectTypes (0x005D5750, _$E) and its guarded atexit
// destructor (0x005D5790), emitted by the definition below: an empty DynamicVectorClass (growth
// step 10). The destructor's guard is bit 0 of 0x00A72FD9.

DynamicVectorClass<ObjectTypeClass *> ObjectTypeClass::ObjectTypes;


// Guarded atexit destructor (0x005D5790, _$E) of ObjectTypeClass::ObjectTypes: emitted by the
// definition in ObjectTypeClass_ObjectTypes_init_005d5750.cpp; no source of its own.


// ObjectTypeClass existing constructor API/layout; real Particle-type base dependency.
// Per-unit form of the real container append: observed growth operand order.
// No new layout or alias; same existing template member and protected fields.
template <> inline bool DynamicVectorClass<ObjectTypeClass *>::Add(ObjectTypeClass *const &object)
{
    if (ActiveCount >= Length()) {
        if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
            if (!Resize(GrowthStep + VectorMax)) return false;
        } else return false;
    }
    int index = ActiveCount++;
    Vector[index] = object;
    return true;
}
ObjectTypeClass::ObjectTypeClass(char const *ini_name)
    : AbstractTypeClass(ini_name),
      unknown_9c(0), MaxStrength(0), unknown_a4(0), unknown_a8(false), unknown_ac(0),
      unknown_1b8(false), unknown_1bc(0), unknown_1c0(-1), unknown_1c4(-1),
      unknown_1e1(false), unknown_1e2(false),
      unknown_1fc(false), unknown_1fd(false), unknown_1fe(true), unknown_1ff(false),
      unknown_200(true), unknown_201(true), unknown_202(false), unknown_203(false),
      unknown_204(false), unknown_205(true), unknown_206(false), unknown_207(false),
      unknown_208(false), unknown_209(false), unknown_20a(false),
      LineTrailColor(128,128,128), unknown_210(16)
{
    unknown_1c8 = ID;
    unknown_1e3 = unknown_string_83c55c;
    ObjectTypes.Add(this);
}


// ObjectTypeClass::ObjectTypeClass(NoInitClass const &noinit) (0x005D5A80)
// Load path: only the members with constructors of their own are set up.

ObjectTypeClass::ObjectTypeClass(NoInitClass const &noinit) :
    AbstractTypeClass(noinit),
    RadialColor(noinit),
    unknown_a4(0),
    unknown_b0(noinit),
    unknown_b8(noinit),
    unknown_c0(noinit),
    unknown_1c8(noinit),
    unknown_1e3(noinit),
    LineTrailColor(noinit)
{
}


// ObjectTypeClass::~ObjectTypeClass() (0x005D5B60)
// Removes the type from ObjectTypes, frees an owned unknown_a4; the member
// destructors release the buffers and object pairs.

ObjectTypeClass::~ObjectTypeClass()
{
    ObjectTypes.Delete(this);
    if (unknown_a8 && unknown_a4 != 0) {
        operator delete(unknown_a4);
        unknown_a4 = 0;
        unknown_a8 = false;
    }
}


// int ObjectTypeClass::Max_Pips() const (0x005D5D00)

int ObjectTypeClass::Max_Pips() const
{
    return 0;
}


// CoordStruct ObjectTypeClass::Pixel_Dimensions() const (0x005D5D10)

CoordStruct ObjectTypeClass::Pixel_Dimensions() const
{
    CoordStruct coord;
    coord.X = 10;
    coord.Y = 10;
    coord.Z = 10;
    return coord;
}


// CoordStruct ObjectTypeClass::Lepton_Dimensions() const (0x005D5D30)

CoordStruct ObjectTypeClass::Lepton_Dimensions() const
{
    CoordStruct coord;
    coord.X = 0x100;
    coord.Y = 0x100;
    coord.Z = unknown_a72fdc * 2;
    return coord;
}


// int ObjectTypeClass::Cost_Of(HouseClass *house) const (0x005D5D60)

int ObjectTypeClass::Cost_Of(HouseClass *house) const
{
    return 0;
}


// int ObjectTypeClass::unknown_slot34() const (0x005D5D70)

int ObjectTypeClass::unknown_slot34() const
{
    return 0;
}


// void const *ObjectTypeClass::unknown_slot38() const (0x005D5D80)

void const *ObjectTypeClass::unknown_slot38() const
{
    return 0;
}


// Cell const *ObjectTypeClass::Occupy_List(int arg1) const (0x005D5D90)
// The default occupation list: the origin cell, then the 0x7FFF end marker
// (a function-local static array registered with atexit on first use).

Cell const *ObjectTypeClass::Occupy_List(int arg1) const
{
    static UnknownStaticCell const unknown_list[] = {Cell(0, 0), Cell(0x7FFF, 0x7FFF)};
    return &unknown_list[0].Cell;
}


// Exit thunk of the function-local static in ObjectTypeClass::Occupy_List
// (0x005D5DF0)
// Compiler generated ($E) right after unknown_slot36: UnknownStaticCell's empty
// destructor over the array, registered with atexit on first use.


// void ObjectTypeClass::One_Time() (0x005D5E00)
// Loads the shared pip and talk-bubble shapes once: from a loose file when
// one exists (then owned), else from the mixfiles.

extern void const *unknown_pipbrd_shapes_a72fe0;
extern void const *unknown_pips_shapes_a72fe4;
extern void const *unknown_pips2_shapes_a72fe8;
extern void const *unknown_talkbubl_shapes_a72fec;
extern bool unknown_pipbrd_owned_a72ff0;
extern bool unknown_pips_owned_a72ff1;
extern bool unknown_pips2_owned_a72ff2;
extern bool unknown_talkbubl_owned_a72ff3;

void ObjectTypeClass::One_Time()
{
    if (unknown_pipbrd_shapes_a72fe0 == 0) {
        RawFileClass file("PIPBRD.SHP");
        unknown_pipbrd_shapes_a72fe0 = Load_Alloc_Data(file);
        unknown_pipbrd_owned_a72ff0 = unknown_pipbrd_shapes_a72fe0 != 0;
        if (unknown_pipbrd_shapes_a72fe0 == 0) {
            unknown_pipbrd_shapes_a72fe0 = MixFileClass::Retrieve("PIPBRD.SHP");
        }
    }
    if (unknown_pips_shapes_a72fe4 == 0) {
        RawFileClass file("PIPS.SHP");
        unknown_pips_shapes_a72fe4 = Load_Alloc_Data(file);
        unknown_pips_owned_a72ff1 = unknown_pips_shapes_a72fe4 != 0;
        if (unknown_pips_shapes_a72fe4 == 0) {
            unknown_pips_shapes_a72fe4 = MixFileClass::Retrieve("PIPS.SHP");
        }
    }
    if (unknown_pips2_shapes_a72fe8 == 0) {
        RawFileClass file("PIPS2.SHP");
        unknown_pips2_shapes_a72fe8 = Load_Alloc_Data(file);
        unknown_pips2_owned_a72ff2 = unknown_pips2_shapes_a72fe8 != 0;
        if (unknown_pips2_shapes_a72fe8 == 0) {
            unknown_pips2_shapes_a72fe8 = MixFileClass::Retrieve("PIPS2.SHP");
        }
    }
    if (unknown_talkbubl_shapes_a72fec == 0) {
        RawFileClass file("TALKBUBL.SHP");
        unknown_talkbubl_shapes_a72fec = Load_Alloc_Data(file);
        unknown_talkbubl_owned_a72ff3 = unknown_talkbubl_shapes_a72fec != 0;
        if (unknown_talkbubl_shapes_a72fec == 0) {
            unknown_talkbubl_shapes_a72fec = MixFileClass::Retrieve("TALKBUBL.SHP");
        }
    }
}


// 0x005d5f40: function not yet identified
//
// Body: 120 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void FUN_005d5f40(void)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005d5f40 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" unsigned int DAT_00a72fe0;
extern "C" unsigned int DAT_00a72fe4;
extern "C" unsigned int DAT_00a72fe8;
extern "C" unsigned int DAT_00a72fec;
extern "C" char DAT_00a72ff0;
extern "C" char DAT_00a72ff1;
extern "C" char DAT_00a72ff2;
extern "C" char DAT_00a72ff3;

void  FUN_005d5f40(void)
{
    if (DAT_00a72ff0 != '\0') {
        operator_delete_00781d3b((unsigned int)(DAT_00a72fe0));
        DAT_00a72ff0 = '\0';
    }
    if (DAT_00a72ff1 != '\0') {
        operator_delete_00781d3b((unsigned int)(DAT_00a72fe4));
        DAT_00a72ff1 = '\0';
    }
    if (DAT_00a72ff2 != '\0') {
        operator_delete_00781d3b((unsigned int)(DAT_00a72fe8));
        DAT_00a72ff2 = '\0';
    }
    if (DAT_00a72ff3 != '\0') {
        operator_delete_00781d3b((unsigned int)(DAT_00a72fec));
        DAT_00a72ff3 = '\0';
    }
    return;
}

}  // namespace seed_005d5f40


// BuildingClass *ObjectTypeClass::Who_Can_Build_Me(bool in_theory, bool legal, bool check_limit, HouseClass *house) const (0x005D5FC0, slot 37)
// Scans the house's buildings for a factory of this type's kind that is
// out of limbo, not being sold (mission 0x13), owned by an allowed house
// and, as asked, powered (legal) and below the build limit (check_limit).
// Aircraft (kind 3) not built "in theory" need a helipad with a free radio
// slot; a naval unit type (kind 0x28) needs a naval yard and others a land
// factory. A primary factory (+0x32B) wins at once, else the last match.

BuildingClass *ObjectTypeClass::Who_Can_Build_Me(bool in_theory, bool legal, bool check_limit, HouseClass *house) const
{
    BuildingClass *anybuilding = 0;
    int ownable = unknown_slot28();
    int count = house->Buildings.Count();
    for (int index = 0; index < count; index++) {
        BuildingClass *building = house->Buildings[index];
        if (building->unknown_76) {
            continue;
        }
        if (building->Type->Factory != Kind_Of()) {
            continue;
        }
        if (legal && !building->unknown_530) {
            continue;
        }
        if (building->Get_Mission() == 0x13 || building->MissionQueue == 0x13) {
            continue;
        }
        if (check_limit && building->House->Can_Build((TechnoTypeClass const *)this, 1, 1) <= 0) {
            continue;
        }
        if ((building->Type->unknown_slot28() & ownable) == 0) {
            continue;
        }
        if (!in_theory && building->HasAnyLink() && Kind_Of() == 3) {
            if (Kind_Of() == 3 && building->Unknown_00636EA0()) {
                if (building->unknown_32b) {
                    return building;
                }
                anybuilding = building;
            }
            continue;
        }
        if (Kind_Of() == 0x28 && ((TechnoTypeClass const *)this)->Naval) {
            if (!building->Type->Naval) {
                continue;
            }
            if (building->unknown_32b) {
                return building;
            }
        } else if (building->Type->Naval) {
            continue;
        }
        if (building->unknown_32b) {
            return building;
        }
        anybuilding = building;
    }
    if (anybuilding != 0) {
        return anybuilding;
    }
    return 0;
}


// bool ObjectTypeClass::unknown_func_5d6150(char const *image, int index) (0x005D6150)
// Loads turret voxel `index` (<image>TUR.VXL for 0, else <image>TUR<n>.VXL,
// with the matching .HVA) into unknown_c8[index]; the motion is scaled by
// the voxel's first section multiplier. When either fails both are freed.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

bool ObjectTypeClass::unknown_func_5d6150(char const *image, int index)
{
    char name[256];
    char fullname[260];

    if (index == 0) {
        sprintf(name, "%sTUR", image);
    } else {
        sprintf(name, "%sTUR%d", image, index);
    }
    _makepath(fullname, 0, 0, name, ".VXL");
    CCFileClass voxel(fullname);
    if (!voxel.Is_Available()) {
        return false;
    }

    bool voxel_loaded = false;
    unknown_c8[index].unknown_00 = new VoxelLibrary(&voxel);
    if (unknown_c8[index].unknown_00 != 0 && !unknown_c8[index].unknown_00->LoadFailed) {
        voxel_loaded = true;
    }

    if (index == 0) {
        sprintf(name, "%sTUR", image);
    } else {
        sprintf(name, "%sTUR%d", image, index);
    }
    _makepath(fullname, 0, 0, name, ".HVA");
    CCFileClass motion(fullname);
    bool motion_loaded = false;
    unknown_c8[index].unknown_04 = new UnknownPairSecond(&motion);
    if (unknown_c8[index].unknown_04 != 0 && !unknown_c8[index].unknown_04->unknown_00) {
        motion_loaded = true;
    }

    if (voxel_loaded && motion_loaded) {
        unknown_c8[index].unknown_04->Scale_0059ed30(unknown_c8[index].unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
        return true;
    }
    delete unknown_c8[index].unknown_00;
    delete unknown_c8[index].unknown_04;
    return false;
}


// bool ObjectTypeClass::unknown_func_5d63f0(char const *image, int index) (0x005D63F0)
// Loads turret voxel `index` (<image>TUR.VXL for 0, else <image>TUR<n>.VXL,
// with the matching .HVA) into unknown_140[index]; the motion is scaled by
// the voxel's first section multiplier. When either fails both are freed.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

bool ObjectTypeClass::unknown_func_5d63f0(char const *image, int index)
{
    char name[256];
    char fullname[260];

    if (index == 0) {
        sprintf(name, "%sBARL", image);
    } else {
        sprintf(name, "%sBARL%d", image, index);
    }
    _makepath(fullname, 0, 0, name, ".VXL");
    CCFileClass voxel(fullname);
    if (!voxel.Is_Available()) {
        return true;
    }

    bool voxel_loaded = false;
    unknown_140[index].unknown_00 = new VoxelLibrary(&voxel);
    if (unknown_140[index].unknown_00 != 0 && !unknown_140[index].unknown_00->LoadFailed) {
        voxel_loaded = true;
    }

    if (index == 0) {
        sprintf(name, "%sBARL", image);
    } else {
        sprintf(name, "%sBARL%d", image, index);
    }
    _makepath(fullname, 0, 0, name, ".HVA");
    CCFileClass motion(fullname);
    bool motion_loaded = false;
    unknown_140[index].unknown_04 = new UnknownPairSecond(&motion);
    if (unknown_140[index].unknown_04 != 0 && !unknown_140[index].unknown_04->unknown_00) {
        motion_loaded = true;
    }

    if (voxel_loaded && motion_loaded) {
        unknown_140[index].unknown_04->Scale_0059ed30(unknown_140[index].unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
        return true;
    }
    delete unknown_140[index].unknown_00;
    delete unknown_140[index].unknown_04;
    return false;
}


// void ObjectTypeClass::unknown_func_5d66a0() (0x005D66A0)
// Frees both voxel-pair tables (unknown_c8 and unknown_140, 15 each).

void ObjectTypeClass::unknown_func_5d66a0()
{
    for (int index = 0; index < 15; index++) {
        if (unknown_c8[index].unknown_00 != 0) {
            delete unknown_c8[index].unknown_00;
            unknown_c8[index].unknown_00 = 0;
        }
        if (unknown_c8[index].unknown_04 != 0) {
            delete unknown_c8[index].unknown_04;
            unknown_c8[index].unknown_04 = 0;
        }
        if (unknown_140[index].unknown_00 != 0) {
            delete unknown_140[index].unknown_00;
            unknown_140[index].unknown_00 = 0;
        }
        if (unknown_140[index].unknown_04 != 0) {
            delete unknown_140[index].unknown_04;
            unknown_140[index].unknown_04 = 0;
        }
    }
}


// void ObjectTypeClass::Fetch_Voxel_Image() (0x005D6730)
// Loads the type's voxel: <image>.VXL/.HVA into unknown_b0; for unit
// types without a turret the water (<image>W, the APC only) and
// no-spawn-alternate (<image>WO, NoSpawnAlt=) images into unknown_b8; else
// the turret voxels (<image>TUR, or one per turret weapon) and barrel
// voxels (<image>BARL, or one per turret weapon). On any failure the main
// and secondary voxels are freed; otherwise unknown_1bc becomes the largest
// section dimension (at least 8) and the cached buffers are released.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

void ObjectTypeClass::Fetch_Voxel_Image()
{
    char name[256];
    char fullname[260];
    bool failed = false;

    _makepath(fullname, 0, 0, unknown_1c8, ".VXL");
    CCFileClass voxel(fullname);
    if (voxel.Is_Available()) {
        delete unknown_b0.unknown_00;
        unknown_b0.unknown_00 = new VoxelLibrary(&voxel);
        if (unknown_b0.unknown_00 == 0 || unknown_b0.unknown_00->LoadFailed) {
            failed = true;
        }
        _makepath(fullname, 0, 0, unknown_1c8, ".HVA");
        CCFileClass motion(fullname);
        delete unknown_b0.unknown_04;
        unknown_b0.unknown_04 = new UnknownPairSecond(&motion);
        if (unknown_b0.unknown_04 != 0 && !unknown_b0.unknown_04->unknown_00) {
            unknown_b0.unknown_04->Scale_0059ed30(unknown_b0.unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
        } else {
            failed = true;
        }
    } else {
        failed = true;
    }

    if (Kind_Of() == 0x28 && !((TechnoTypeClass *)this)->Turret) {
        if (strcmp(ID, "APC") == 0) {
            sprintf(name, "%sW", (char const *)unknown_1c8);
            _makepath(fullname, 0, 0, name, ".VXL");
            CCFileClass voxel(fullname);
            if (voxel.Is_Available()) {
                delete unknown_b8.unknown_00;
                unknown_b8.unknown_00 = new VoxelLibrary(&voxel);
                if (unknown_b8.unknown_00 == 0 || unknown_b8.unknown_00->LoadFailed) {
                    failed = true;
                }
                sprintf(name, "%sW", (char const *)unknown_1c8);
                _makepath(fullname, 0, 0, name, ".HVA");
                CCFileClass motion(fullname);
                delete unknown_b8.unknown_04;
                unknown_b8.unknown_04 = new UnknownPairSecond(&motion);
                if (unknown_b8.unknown_04 != 0 && !unknown_b8.unknown_04->unknown_00) {
                    unknown_b8.unknown_04->Scale_0059ed30(unknown_b8.unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
                } else {
                    failed = true;
                }
            }
        }
        if (unknown_1b8) {
            sprintf(name, "%sWO", (char const *)unknown_1c8);
            _makepath(fullname, 0, 0, name, ".VXL");
            CCFileClass voxel(fullname);
            if (voxel.Is_Available()) {
                delete unknown_b8.unknown_00;
                unknown_b8.unknown_00 = new VoxelLibrary(&voxel);
                if (unknown_b8.unknown_00 == 0 || unknown_b8.unknown_00->LoadFailed) {
                    failed = true;
                }
                sprintf(name, "%sWO", (char const *)unknown_1c8);
                _makepath(fullname, 0, 0, name, ".HVA");
                CCFileClass motion(fullname);
                delete unknown_b8.unknown_04;
                unknown_b8.unknown_04 = new UnknownPairSecond(&motion);
                if (unknown_b8.unknown_04 != 0 && !unknown_b8.unknown_04->unknown_00) {
                    unknown_b8.unknown_04->Scale_0059ed30(unknown_b8.unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
                } else {
                    failed = true;
                }
            }
        }
    } else if (Kind_Of() == 0x28 && ((TechnoTypeClass *)this)->Has_Turret_Weapons()) {
        int count = ((TechnoTypeClass *)this)->TurretCount;
        for (int index = 0; index < count; index++) {
            if (!unknown_func_5d6150(unknown_1c8, index)) {
                failed = true;
                break;
            }
        }
    } else {
        sprintf(name, "%sTUR", (char const *)unknown_1c8);
        _makepath(fullname, 0, 0, name, ".VXL");
        CCFileClass voxel(fullname);
        if (voxel.Is_Available()) {
            delete unknown_b8.unknown_00;
            unknown_b8.unknown_00 = new VoxelLibrary(&voxel);
            if (unknown_b8.unknown_00 == 0 || unknown_b8.unknown_00->LoadFailed) {
                failed = true;
            }
            sprintf(name, "%sTUR", (char const *)unknown_1c8);
            _makepath(fullname, 0, 0, name, ".HVA");
            CCFileClass motion(fullname);
            delete unknown_b8.unknown_04;
            unknown_b8.unknown_04 = new UnknownPairSecond(&motion);
            if (unknown_b8.unknown_04 != 0 && !unknown_b8.unknown_04->unknown_00) {
                unknown_b8.unknown_04->Scale_0059ed30(unknown_b8.unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
            } else {
                failed = true;
            }
        }
    }

    if (Kind_Of() != 0x28 || ((TechnoTypeClass *)this)->Turret) {
        if (Kind_Of() == 0x28 && ((TechnoTypeClass *)this)->Has_Turret_Weapons()) {
            int count = ((TechnoTypeClass *)this)->TurretCount;
            for (int index = 0; index < count; index++) {
                if (!unknown_func_5d63f0(unknown_1c8, index)) {
                    failed = true;
                    break;
                }
            }
        } else {
            sprintf(name, "%sBARL", (char const *)unknown_1c8);
            _makepath(fullname, 0, 0, name, ".VXL");
            CCFileClass voxel(fullname);
            if (voxel.Is_Available()) {
                delete unknown_c0.unknown_00;
                unknown_c0.unknown_00 = new VoxelLibrary(&voxel);
                if (unknown_c0.unknown_00 == 0 || unknown_c0.unknown_00->LoadFailed) {
                    failed = true;
                }
                sprintf(name, "%sBARL", (char const *)unknown_1c8);
                _makepath(fullname, 0, 0, name, ".HVA");
                CCFileClass motion(fullname);
                delete unknown_c0.unknown_04;
                unknown_c0.unknown_04 = new UnknownPairSecond(&motion);
                if (unknown_c0.unknown_04 != 0 && !unknown_c0.unknown_04->unknown_00) {
                    unknown_c0.unknown_04->Scale_0059ed30(unknown_c0.unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
                } else {
                    failed = true;
                }
            }
        }
    }

    if (failed) {
        if (unknown_b0.unknown_00 != 0) {
            delete unknown_b0.unknown_00;
            unknown_b0.unknown_00 = 0;
        }
        if (unknown_b0.unknown_04 != 0) {
            delete unknown_b0.unknown_04;
            unknown_b0.unknown_04 = 0;
        }
        if (Kind_Of() == 0x28 && ((TechnoTypeClass *)this)->Has_Turret_Weapons()) {
            unknown_func_5d66a0();
        } else {
            if (unknown_b8.unknown_00 != 0) {
                delete unknown_b8.unknown_00;
                unknown_b8.unknown_00 = 0;
            }
            if (unknown_b8.unknown_04 != 0) {
                delete unknown_b8.unknown_04;
                unknown_b8.unknown_04 = 0;
            }
        }
    } else {
        int size = unknown_b0.unknown_00->Get_Layer_Info(0, 0)->unknown_a0;
        for (int index = 0; index < unknown_b0.unknown_00->LayerCount; index++) {
            if (size <= unknown_b0.unknown_00->Get_Layer_Info(index, 0)->unknown_a0) {
                size = unknown_b0.unknown_00->Get_Layer_Info(index, 0)->unknown_a0;
            }
            if (size <= unknown_b0.unknown_00->Get_Layer_Info(index, 0)->unknown_a1) {
                size = unknown_b0.unknown_00->Get_Layer_Info(index, 0)->unknown_a1;
            }
            if (size <= unknown_b0.unknown_00->Get_Layer_Info(index, 0)->unknown_a2) {
                size = unknown_b0.unknown_00->Get_Layer_Info(index, 0)->unknown_a2;
            }
        }
        unknown_1bc = size > 8 ? size : 8;
        unknown_214.~UnknownBuffer();
        unknown_228.~UnknownBuffer();
        unknown_23c.~UnknownBuffer();
        unknown_250.~UnknownBuffer();
    }
}


// void ObjectTypeClass::Fetch_Aux_Voxel_Image() (0x005D72B0)
// For a turreted unit type with an image: loads <image>TUR.VXL and
// <image>BARL.VXL into unknown_b8 / unknown_c0, each with its .HVA motion
// scaled by the voxel's first section multiplier.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

void ObjectTypeClass::Fetch_Aux_Voxel_Image()
{
    if (Kind_Of() == 0x28 && Get_Image_Data() != 0 && ((TechnoTypeClass *)this)->Turret) {
        char name[256];
        char fullname[260];

        sprintf(name, "%sTUR", (char const *)unknown_1c8);
        _makepath(fullname, 0, 0, name, ".VXL");
        CCFileClass turret(fullname);
        if (turret.Is_Available()) {
            delete unknown_b8.unknown_00;
            unknown_b8.unknown_00 = new VoxelLibrary(&turret);
            if (unknown_b8.unknown_00 != 0 && !unknown_b8.unknown_00->LoadFailed) {
                sprintf(name, "%sTUR", (char const *)unknown_1c8);
                _makepath(fullname, 0, 0, name, ".HVA");
                CCFileClass motion(fullname);
                delete unknown_b8.unknown_04;
                unknown_b8.unknown_04 = new UnknownPairSecond(&motion);
                unknown_b8.unknown_04->Scale_0059ed30(unknown_b8.unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
            }
        }

        sprintf(name, "%sBARL", (char const *)unknown_1c8);
        _makepath(fullname, 0, 0, name, ".VXL");
        CCFileClass barrel(fullname);
        if (barrel.Is_Available()) {
            delete unknown_c0.unknown_00;
            unknown_c0.unknown_00 = new VoxelLibrary(&barrel);
            if (unknown_c0.unknown_00 != 0 && !unknown_c0.unknown_00->LoadFailed) {
                sprintf(name, "%sBARL", (char const *)unknown_1c8);
                _makepath(fullname, 0, 0, name, ".HVA");
                CCFileClass motion(fullname);
                delete unknown_c0.unknown_04;
                unknown_c0.unknown_04 = new UnknownPairSecond(&motion);
                unknown_c0.unknown_04->Scale_0059ed30(unknown_c0.unknown_00->Get_Layer_Info(0, 0)->unknown_0c);
            }
        }
    }
}


// void ObjectTypeClass::Fetch_Normal_Image() (0x005D7630)
// (Re)loads the image for the current theater: arctic-art types switch to
// their "A" image once in the snow theater (theater 1); the file name gets
// the theater suffix (Theater=) or theater letter (NewTheater=); any owned
// image is freed and the mixfile copy taken; unknown_1bc becomes the larger
// shape dimension, at least 8.
// ObjectType.cpp has no empty-rectangle static of its own (no dynamic
// initializer for it in the unit).

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

void ObjectTypeClass::Fetch_Normal_Image()
{
    char fullname[260];
    if (unknown_1e1 && unknown_id_source_a3d290->Theater == 1 && !unknown_a8) {
        if (!unknown_1e2) {
            char buffer[256];
            sprintf(buffer, "%sA", (char const *)unknown_1c8);
            unknown_1c8 = buffer;
            unknown_1e2 = true;
        }
    } else {
        unknown_1e2 = false;
    }

    _makepath(fullname, 0, 0, unknown_1c8, ".SHP");
    if (unknown_1fc) {
        _makepath(fullname, 0, 0, unknown_1c8, Theaters[unknown_id_source_a3d290->Theater].Suffix);
    } else if (unknown_207) {
        Theater_Naming_Convention(fullname, unknown_id_source_a3d290->Theater);
    }

    if (unknown_a8 && unknown_a4 != 0) {
        operator delete(unknown_a4);
    }
    unknown_a4 = 0;
    unknown_a8 = false;
    unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
    if (unknown_a4 != 0) {
        ShapeFileStruct const *shape = (ShapeFileStruct const *)unknown_a4;
        int size;
        if (shape->Width > shape->Height) {
            size = shape->Width;
        } else {
            size = shape->Height;
        }
        unknown_1bc = size > 8 ? size : 8;
    }
}


// bool ObjectTypeClass::Read_INI(CCINIClass &ini) (0x005D7810)
// The abstract type's keys, then the object keys from the rules section and
// the art keys from the image's art INI section; non-voxel types load their
// theater image, and an AlphaImage= is looked up in the mixfiles.
// Members by INI key: unknown_1c8 Image, unknown_1e3 AlphaImage,
// unknown_1c0 CrushSound, unknown_1c4 AmbientSound, unknown_1fd Crushable,
// unknown_1fe Bombable, unknown_1b8 NoSpawnAlt, unknown_1e1
// AlternateArcticArt, unknown_1ff RadarInvisible, unknown_200 Selectable,
// unknown_201 LegalTarget, unknown_9c Armor, MaxStrength Strength,
// unknown_203 Immune, unknown_202 Insignificant, unknown_208
// HasRadialIndicator, unknown_209 IgnoresFirestorm;
// art: unknown_20a UseLineTrail, unknown_210
// LineTrailColorDecrement, unknown_1fc Theater, unknown_207 NewTheater,
// unknown_206 Voxel.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

bool ObjectTypeClass::Read_INI(CCINIClass &ini)
{
    if (AbstractTypeClass::Read_INI(ini)) {
        char buffer[25];
        strcpy(buffer, unknown_1c8);
        ini.Get_String(ID, "Image", buffer, unknown_1c8.Buffer, sizeof(unknown_1c8.Buffer));
        strcpy(buffer, unknown_1e3);
        ini.Get_String(ID, "AlphaImage", buffer, unknown_1e3.Buffer, sizeof(unknown_1e3.Buffer));

        char name[128];
        int sound = unknown_1c0;
        if (ini.Get_String(ID, "CrushSound", unknown_string_83c55c, name, sizeof(name))) {
            int found = VocClass::From_Name(name);
            if (found != -1) {
                sound = found;
            }
        }
        unknown_1c0 = sound;
        sound = unknown_1c4;
        int found;
        if (!ini.Get_String(ID, "AmbientSound", unknown_string_83c55c, name, sizeof(name)) || (found = VocClass::From_Name(name)) == -1) {
            found = sound;
        }
        unknown_1c4 = found;

        unknown_1fd = ini.Get_Bool(ID, "Crushable", unknown_1fd);
        unknown_1fe = ini.Get_Bool(ID, "Bombable", unknown_1fe);
        unknown_1b8 = ini.Get_Bool(ID, "NoSpawnAlt", unknown_1b8);
        unknown_1e1 = ini.Get_Bool(ID, "AlternateArcticArt", unknown_1e1);
        unknown_1ff = ini.Get_Bool(ID, "RadarInvisible", unknown_1ff);
        unknown_200 = ini.Get_Bool(ID, "Selectable", unknown_200);
        unknown_201 = ini.Get_Bool(ID, "LegalTarget", unknown_201);
        unknown_9c = ini.Get_ArmorType(ID, "Armor", (ArmorType)unknown_9c);
        MaxStrength = ini.Get_Int(ID, "Strength", MaxStrength);
        unknown_203 = ini.Get_Bool(ID, "Immune", unknown_203);
        unknown_202 = ini.Get_Bool(ID, "Insignificant", unknown_202);
        unknown_208 = ini.Get_Bool(ID, "HasRadialIndicator", unknown_208);
        RadialColor = ini.Get_RGB(ID, "RadialColor", RadialColor);
        unknown_209 = ini.Get_Bool(ID, "IgnoresFirestorm", unknown_209);

        unknown_20a = ArtINI.Get_Bool(unknown_1c8, "UseLineTrail", unknown_20a);
        LineTrailColor = ArtINI.Get_RGB(unknown_1c8, "LineTrailColor", LineTrailColor);
        unknown_210 = ArtINI.Get_Int(unknown_1c8, "LineTrailColorDecrement", unknown_210);
        unknown_1fc = ArtINI.Get_Bool(unknown_1c8, "Theater", unknown_1fc);
        unknown_207 = ArtINI.Get_Bool(unknown_1c8, "NewTheater", unknown_207);
        unknown_206 = ArtINI.Get_Bool(unknown_1c8, "Voxel", unknown_206);
        if (!unknown_206) {
            Fetch_Normal_Image();
        }

        if (unknown_1e3[0] != 0) {
            char filename[260];
            _makepath(filename, 0, 0, unknown_1e3, ".SHP");
            unknown_ac = (int)MixFileClass::Retrieve(filename);
        }
        return true;
    }
    return false;
}


// void __fastcall ObjectTypeClass::Theater_Naming_Convention(char *buffer, int theater) (0x005D7BD0)
// A theater-generic image name ("G", "N" or "C" first, "A" or "T" second,
// either case) gets the theater's letter as its second character.

void __fastcall ObjectTypeClass::Theater_Naming_Convention(char *buffer, int theater)
{
    if (theater != -1) {
        char first = buffer[0];
        char second = buffer[1];
        if (first >= 'A' && first <= 'Z') {
            first += 'a' - 'A';
        }
        if (second >= 'A' && second <= 'Z') {
            second += 'a' - 'A';
        }
        if ((first == 'g' || first == 'n' || first == 'c') && (second == 'a' || second == 't')) {
            buffer[1] = Theaters[theater].Letter[0];
        }
    }
}


// void ObjectTypeClass::Generic_Naming_Convention(char *buffer) (0x005D7C20)
// The theater letter of a file name becomes the generic one ('G').

void __fastcall ObjectTypeClass::Generic_Naming_Convention(char *buffer)
{
    buffer[1] = 'G';
}


// HRESULT __stdcall ObjectTypeClass::Load(IStream *stream) (0x005D7C30)
// Drops the cached buffers and voxel pairs, loads the base, then rebuilds
// the cleared members and looks the alternate image up in the mixfiles.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

// Empties a pair without freeing (the loaded pointers are stale). Used
// for unknown_b0 and unknown_b8 only: that inline-site count is what keeps
// exactly the first buffer destructor inline (one more or one fewer site
// changes which ones VC6 expands).
static inline void Clear_Pair(UnknownObjectPair &pair)
{
    pair.unknown_00 = 0;
    pair.unknown_04 = 0;
}

HRESULT __stdcall ObjectTypeClass::Load(IStream *stream)
{
    unknown_214.~UnknownBuffer();
    unknown_228.~UnknownBuffer();
    unknown_23c.~UnknownBuffer();
    unknown_250.~UnknownBuffer();
    delete unknown_b0.unknown_00;
    unknown_b0.unknown_00 = 0;
    delete unknown_b0.unknown_04;
    unknown_b0.unknown_04 = 0;
    delete unknown_b8.unknown_00;
    unknown_b8.unknown_00 = 0;
    delete unknown_b8.unknown_04;
    unknown_b8.unknown_04 = 0;
    delete unknown_c0.unknown_00;
    unknown_c0.unknown_00 = 0;
    delete unknown_c0.unknown_04;
    unknown_c0.unknown_04 = 0;
    unknown_func_5d66a0();

    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }

    unknown_a4 = 0;
    unknown_a8 = false;
    unknown_ac = 0;
    new (&unknown_214) UnknownBuffer;
    new (&unknown_228) UnknownBuffer;
    new (&unknown_23c) UnknownBuffer;
    new (&unknown_250) UnknownBuffer;
    Clear_Pair(unknown_b0);
    Clear_Pair(unknown_b8);
    unknown_c0.unknown_00 = 0;
    unknown_c0.unknown_04 = 0;
    for (int index = 0; index < 15; index++) {
        unknown_c8[index].unknown_00 = 0;
        unknown_c8[index].unknown_04 = 0;
        unknown_140[index].unknown_00 = 0;
        unknown_140[index].unknown_04 = 0;
    }
    if (unknown_1e3.Buffer[0] != 0) {
        char filename[260];
        _makepath(filename, 0, 0, unknown_1e3.Buffer, ".SHP");
        unknown_ac = (int)MixFileClass::Retrieve(filename);
    }
    return S_OK;
}


// HRESULT __stdcall ObjectTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x005D7E60)

HRESULT __stdcall ObjectTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall ObjectTypeClass::GetSizeMax(ULARGE_INTEGER *size) (0x005D7E80)

HRESULT __stdcall ObjectTypeClass::GetSizeMax(ULARGE_INTEGER *size)
{
    HRESULT result = AbstractClass::GetSizeMax(size);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// ObjectTypeClass *__fastcall ObjectTypeClass::Find_From_Name(char const *name) (0x005D7EA0)
// The object type whose ID matches name (case-insensitive), else null.
// (The inventory splits this body at 0x005D7EB3, inside the loop's first
// instruction bytes; the function runs from 0x005D7EA0 to the ret at
// 0x005D7EE2.)

extern "C" int __cdecl _stricmp(char const *string1, char const *string2);

ObjectTypeClass *__fastcall ObjectTypeClass::Find_From_Name(char const *name)
{
    for (int index = 0; index < ObjectTypes.Count(); index++) {
        if (_stricmp(ObjectTypes[index]->ID, name) == 0) {
            return ObjectTypes[index];
        }
    }
    return 0;
}


// void ObjectTypeClass::Clear_Voxel_Indexes() (0x005D7EF0)
// Releases the four cached buffers of every object type, then resets
// unknown_839aa8 from unknown_839aa4 (name and meaning hypotheses).

extern int unknown_839aa4;
extern int unknown_839aa8;

void ObjectTypeClass::Clear_Voxel_Indexes()
{
    for (int index = 0; index < ObjectTypes.Count(); index++) {
        ObjectTypeClass *type = ObjectTypes[index];
        type->unknown_214.~UnknownBuffer();
        type->unknown_228.~UnknownBuffer();
        type->unknown_23c.~UnknownBuffer();
        type->unknown_250.~UnknownBuffer();
    }
    unknown_839aa8 = unknown_839aa4;
}


// ObjectTypeClass scalar deleting destructor (0x005D7FF0)
// Compiler generated after the unit's last member: ~ObjectTypeClass,
// then operator delete when bit 0 of the flags is set.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<ObjectTypeClass *>::Clear (0x005D8010; DynamicVectorClass<ObjectTypeClass *> vtable 0x007a768c slot 3).
// DynamicVectorClass<ObjectTypeClass *>::ID_pointer (0x005D8040; DynamicVectorClass<ObjectTypeClass *> vtable 0x007a768c slot 5).
// VectorClass<ObjectTypeClass *>::Fetch (0x005D8060; VectorClass<ObjectTypeClass *> vtable 0x007a76ac slot 6).


// UnknownBuffer::~UnknownBuffer() (0x005D8070)
// The header's inline destructor, kept out of line by ObjectTypeClass::Load
// (0x005D7C30), which destroys the cached buffers explicitly.
// @symbol ??1UnknownBuffer@@QAE@XZ


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<ObjectTypeClass *>::operator equal (0x005D8090; VectorClass<ObjectTypeClass *> vtable 0x007a76ac slot 1).
// VectorClass<ObjectTypeClass *>::Resize (0x005D80D0; VectorClass<ObjectTypeClass *> vtable 0x007a76ac slot 2).
// VectorClass<ObjectTypeClass *>::Clear (0x005D8180; VectorClass<ObjectTypeClass *> vtable 0x007a76ac slot 3).
// VectorClass<ObjectTypeClass *>::ID_object (0x005D81B0; VectorClass<ObjectTypeClass *> vtable 0x007a76ac slot 4).
// VectorClass<ObjectTypeClass *>::ID_pointer (0x005D81F0; VectorClass<ObjectTypeClass *> vtable 0x007a76ac slot 5).
// DynamicVectorClass<ObjectTypeClass *>::Resize (0x005D8210; DynamicVectorClass<ObjectTypeClass *> vtable 0x007a768c slot 2).
// DynamicVectorClass<ObjectTypeClass *>::ID_object (0x005D82C0; DynamicVectorClass<ObjectTypeClass *> vtable 0x007a768c slot 4).


extern "C" unsigned char vtable_5d82f0[]; extern "C" void __cdecl release_buffer_5d82f0(void*); extern "C" void __cdecl release_object_5d82f0(void*);
class Leaf_5d82f0 { public: void* get(unsigned int flags); };
void* Leaf_5d82f0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5d82f0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5d82f0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5d82f0(this); return this; }


extern "C" unsigned char vtable_5d8340[]; extern "C" void __cdecl release_buffer_5d8340(void*); extern "C" void __cdecl release_object_5d8340(void*);
class Leaf_5d8340 { public: void* get(unsigned int flags); };
void* Leaf_5d8340::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5d8340; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5d8340((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5d8340(this); return this; }
