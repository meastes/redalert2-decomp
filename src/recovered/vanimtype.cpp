// vanimtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vanimtype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/voxel_anim_type.hpp"
#include "shared/ccini_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/cc_file_class.hpp"
#define VECTOR_IMPLEMENT_ALL
#define VOXEL_ANIM_TYPE_DECLARE_NAME_FROM


// VoxelAnimTypeClass::VoxelAnimTypeClass(char const *ini_name) (0x0070E1A0)
// Default voxel animation values, then registration in the 0x00AC0CB0 list
// and the voxel animation type list.
// Assumptions: the 0x00AC0CB0 list is declared as an ObjectClass list in
// shared/object.hpp (element type unknown; it also holds types).

VoxelAnimTypeClass::VoxelAnimTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name),
    Normalized(false),
    Translucent(false),
    unknown_266(false),
    VoxelIndex(0),
    Duration(30),
    Elasticity(.8f),
    MinAngularVelocity(0),
    MaxAngularVelocity(10.0 * 3.1415 / 180.0),
    MinZVel(3.5),
    MaxZVel(5.0),
    MaxXYVel(15.0),
    IsMeteor(false),
    Spawns(0),
    SpawnCount(0),
    StartSound(-1),
    StopSound(-1),
    BounceAnim(0),
    ExpireAnim(0),
    TrailerAnim(0),
    Damage(0),
    DamageRadius(0),
    Warhead(0),
    AttachedSystem(0),
    IsTiberium(false)
{
    AbstractClass::Create_ID();
    unknown_201 = false;
    unknown_204 = true;
    unknown_1ff = true;
    unknown_200 = true;
    unknown_202 = true;
    unknown_203 = true;
    unknown_205 = false;
    unknown_objects_ac0cb0.Add(this);
    VoxelAnimTypes.Add(this);
}


// VoxelAnimTypeClass::~VoxelAnimTypeClass() (0x0070E390)
// Leaves both lists; a borrowed (shared) voxel pair is released without
// being destroyed.

VoxelAnimTypeClass::~VoxelAnimTypeClass()
{
    Unknown_006ECB90(this, true);
    unknown_objects_ac0cb0.Delete(this);
    VoxelAnimTypes.Delete(this);
    if (unknown_266) {
        unknown_b0.unknown_00 = 0;
        unknown_b0.unknown_04 = 0;
    }
}


// bool VoxelAnimTypeClass::Read_INI(CCINIClass &ini) (0x0070E470)
// The object keys, then the voxel animation keys; angular velocities are
// given in degrees. A type that shares the body, turret or barrel voxel of
// another (ShareSource=) borrows that voxel pair; otherwise <image>.VXL is
// loaded as its own voxel.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

// Get_Sound: shared/ccini_class.hpp.

static inline AnimTypeClass *Get_Anim(CCINIClass &ini, char const *section, char const *entry, AnimTypeClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return AnimTypeClass::Find_Or_Make(name);
    }
    return defvalue;
}

static inline VoxelAnimTypeClass *Get_Voxel_Anim(CCINIClass &ini, char const *section, char const *entry, VoxelAnimTypeClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return Find_Or_Make_Type(name, VoxelAnimTypes);
    }
    return defvalue;
}

static inline WarheadTypeClass *Get_Warhead(CCINIClass &ini, char const *section, char const *entry, WarheadTypeClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return WarheadTypeClass::Find_Or_Make(name);
    }
    return defvalue;
}

static inline ParticleSystemTypeClass *Get_Particle_System(CCINIClass &ini, char const *section, char const *entry, ParticleSystemTypeClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return Find_Or_Make_ParticleSystemType_00620df0(name);
    }
    return defvalue;
}

bool VoxelAnimTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        Normalized = ini.Get_Bool(ID, "Normalized", Normalized);
        Translucent = ini.Get_Bool(ID, "Translucent", Translucent);
        IsTiberium = ini.Get_Bool(ID, "IsTiberium", IsTiberium);
        IsMeteor = ini.Get_Bool(ID, "IsMeteor", IsMeteor);
        Elasticity = ini.Get_Double(ID, "Elasticity", Elasticity);
        double velocity = ini.Get_Double(ID, "MinAngularVelocity", -1.0);
        if (velocity != 0.0) {
            MinAngularVelocity = velocity * 0.01745277777777778;
        }
        velocity = ini.Get_Double(ID, "MaxAngularVelocity", -1.0);
        if (velocity != 0.0) {
            MaxAngularVelocity = velocity * 0.01745277777777778;
        }
        Duration = ini.Get_Int(ID, "Duration", Duration);
        MinZVel = ini.Get_Double(ID, "MinZVel", MinZVel);
        MaxZVel = ini.Get_Double(ID, "MaxZVel", MaxZVel);
        MaxXYVel = ini.Get_Double(ID, "MaxXYVel", MaxXYVel);

        Spawns = Get_Voxel_Anim(ini, ID, "Spawns", Spawns);
        SpawnCount = ini.Get_Int(ID, "SpawnCount", SpawnCount);
        bool share_body = ini.Get_Bool(ID, "ShareBodyData", false);
        bool share_turret = ini.Get_Bool(ID, "ShareTurretData", false);
        bool share_barrel = ini.Get_Bool(ID, "ShareBarrelData", false);
        VoxelIndex = ini.Get_Int(ID, "VoxelIndex", VoxelIndex);
        StartSound = Get_Sound(ini, ID, "StartSound", StartSound);
        StopSound = Get_Sound(ini, ID, "StopSound", StopSound);
        BounceAnim = Get_Anim(ini, ID, "BounceAnim", BounceAnim);
        ExpireAnim = Get_Anim(ini, ID, "ExpireAnim", ExpireAnim);
        TrailerAnim = Get_Anim(ini, ID, "TrailerAnim", TrailerAnim);
        Damage = ini.Get_Int(ID, "Damage", Damage);
        DamageRadius = ini.Get_Int(ID, "DamageRadius", DamageRadius);
        Warhead = Get_Warhead(ini, ID, "Warhead", Warhead);
        AttachedSystem = Get_Particle_System(ini, ID, "AttachedSystem", AttachedSystem);

        unknown_266 = share_body || share_barrel || share_turret;
        if (unknown_266) {
            ObjectTypeClass *source = ini.Get_Object_Type(ID, "ShareSource");
            if (source != 0) {
                if (share_body) {
                    unknown_b0.unknown_00 = source->unknown_b0.unknown_00;
                    unknown_b0.unknown_04 = source->unknown_b0.unknown_04;
                    return true;
                }
                if (share_turret) {
                    unknown_b0.unknown_00 = source->unknown_b8.unknown_00;
                    unknown_b0.unknown_04 = source->unknown_b8.unknown_04;
                    return true;
                }
                if (share_barrel) {
                    unknown_b0.unknown_00 = source->unknown_c0.unknown_00;
                    unknown_b0.unknown_04 = source->unknown_c0.unknown_04;
                    return true;
                }
            } else {
                unknown_b0.unknown_00 = 0;
                unknown_b0.unknown_04 = 0;
                return true;
            }
        } else {
            char fullname[512];
            _makepath(fullname, 0, 0, unknown_1c8, ".VXL");
            CCFileClass file(fullname);
            delete unknown_b0.unknown_00;
            unknown_b0.unknown_00 = new VoxelLibrary(&file);
        }
        return true;
    }
    return false;
}


// VoxelAnimTypeClass Name_From0070EA40: unsigned lookup, preserving the original upper-bound test.
char const *__fastcall VoxelAnimTypeClass::Name_From(unsigned index)
{
    if (index > (unsigned)VoxelAnimTypes.Count()) return "";
    return VoxelAnimTypes[index]->ID;
}


// static int __fastcall VoxelAnimTypeClass::From_Name(char const *name) (0x0070EA60)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall VoxelAnimTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < VoxelAnimTypes.Count(); index++) {
            if (_stricmp(VoxelAnimTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// void VoxelAnimTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x0070EAB0)

void VoxelAnimTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectTypeClass::Compute_CRC(crc);
    crc(Normalized);
    crc(Translucent);
    crc(unknown_266);
    crc((long)VoxelIndex);
    crc((long)Duration);
    crc(Elasticity);
    crc(MinAngularVelocity);
    crc(MaxAngularVelocity);
    crc(MinZVel);
    crc(MaxZVel);
    crc(MaxXYVel);
    crc(IsMeteor);
    crc((long)SpawnCount);
    crc((long)StartSound);
    crc((long)StopSound);
    crc((long)Damage);
    crc((long)DamageRadius);
    crc(IsTiberium);
}


// HRESULT __stdcall VoxelAnimTypeClass::GetClassID(GUID *clsid) (0x0070EBF0)

HRESULT __stdcall VoxelAnimTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_VoxelAnimTypeClass;
    return S_OK;
}


// HRESULT __stdcall VoxelAnimTypeClass::Load(IStream *stream) (0x0070EC30)
// Restores the type in place, swizzles its type pointers and reloads its
// voxel.

HRESULT __stdcall VoxelAnimTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) VoxelAnimTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&Warhead);
    Swizzler.Swizzle((void **)&BounceAnim);
    Swizzler.Swizzle((void **)&ExpireAnim);
    Swizzler.Swizzle((void **)&TrailerAnim);
    Swizzler.Swizzle((void **)&Spawns);
    Swizzler.Swizzle((void **)&AttachedSystem);
    Fetch_Voxel_Image();
    return S_OK;
}


// HRESULT __stdcall VoxelAnimTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x0070ECF0)

HRESULT __stdcall VoxelAnimTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// void VoxelAnimTypeClass::Detach(AbstractClass *target, bool all) (0x0070ED10)
// Clears every type pointer that refers to a removed object.

void VoxelAnimTypeClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Warhead == target) {
        Warhead = 0;
    }
    if ((AbstractClass *)BounceAnim == target) {
        BounceAnim = 0;
    }
    if ((AbstractClass *)ExpireAnim == target) {
        ExpireAnim = 0;
    }
    if ((AbstractClass *)TrailerAnim == target) {
        TrailerAnim = 0;
    }
    if ((AbstractClass *)Spawns == target) {
        Spawns = 0;
    }
    if ((AbstractClass *)AttachedSystem == target) {
        AttachedSystem = 0;
    }
}


// static VoxelAnimTypeClass *__fastcall VoxelAnimTypeClass::Find_Or_Make(char const *name) (0x0070ED80)
// The type with this ID, else a new one; null for "<none>" and "none".

VoxelAnimTypeClass *__fastcall VoxelAnimTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < VoxelAnimTypes.Count(); index++) {
        if (_stricmp(VoxelAnimTypes[index]->ID, name) == 0) {
            return VoxelAnimTypes[index];
        }
    }
    return new VoxelAnimTypeClass(name);
}


// int VoxelAnimTypeClass::Kind_Of() const (0x0070EE10)
// Compiler-emitted from shared/voxel_anim_type.hpp in this unit; no source of its own.


// int VoxelAnimTypeClass::Size_Of() const (0x0070EE20)
// Compiler-emitted from shared/voxel_anim_type.hpp in this unit; no source of its own.


// bool VoxelAnimTypeClass::Create_And_Place(Cell const *cell, HouseClass *house) (0x0070EE30)
// Compiler-emitted from shared/voxel_anim_type.hpp in this unit; no source of its own.
