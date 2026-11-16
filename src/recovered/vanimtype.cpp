/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/vanimtype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/vanimtype.cpp where both define the function (names only).

// vanimtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vanimtype.json carries the addresses and the rows that
// stay in their own files.

#define VOXEL_ANIM_TYPE_DECLARE_NAME_FROM
// Header-static dynamic initializers of the unit at 0x0070DFB0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0070DFB0-0x0070E196); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x0070DFE0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E000, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E020, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E040, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E060, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E080, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E0C0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E0F0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E120, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E150, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E160, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.


// Dynamic initializer (0x0070E180, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0070dfb0.cpp; no source of its own.




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

VoxelAnimTypeClass::VoxelAnimTypeClass(char const *ininame) :
    ObjectTypeClass(ininame),
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
    AbstractTypePtrTracker.Add(this);
    VoxelAnimTypes.Add(this);
}


/// <summary>
/// Removes this voxel animation type from the game.
/// Anything still referring to this type is detached from it first. A voxel image that
/// was borrowed from another object type is merely let go of, since it belongs to that
/// other type and must not be destroyed here.
/// </summary>
// (0x0070E390)
// Leaves both lists; a borrowed (shared) voxel pair is released without
// being destroyed.

VoxelAnimTypeClass::~VoxelAnimTypeClass()
{
    Detach_This_From_All(this, true);
    AbstractTypePtrTracker.Delete(this);
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

/// <summary>
/// Fetches this voxel animation type's data from the INI database.
/// This routine reads the bouncing, spawning, sound and damage settings that give the
/// voxel animation its character. The voxel image itself either comes from a .VXL file
/// of its own, or is borrowed from the body, turret or barrel of some other object type.
/// </summary>
/// <returns>bool; Was the data read from the database?</returns>
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


/// <summary>
/// Fetches the INI name of the voxel animation type specified.
/// </summary>
/// <returns>Returns with a pointer to the name, or an empty string if the type is not
/// a legal one.</returns>
// VoxelAnimTypeClass Name_From0070EA40: unsigned lookup, preserving the original upper-bound test.
char const *__fastcall VoxelAnimTypeClass::Name_From(unsigned anim)
{
    if (anim > (unsigned)VoxelAnimTypes.Count()) return "";
    return VoxelAnimTypes[anim]->ID;
}


/// <summary>
/// Converts an ASCII name into a voxel animation type number.
/// This routine is used when the rules or a map refer to a voxel animation by name.
/// The comparison ignores case.
/// </summary>
/// <param name="name">The INI name of the voxel animation to search for.</param>
/// <returns>Returns with the type matched, or VANIM_NONE if the name is not known.</returns>
// (0x0070EA60)
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


/// <summary>
/// Submits this voxel animation type's data to the CRC accumulator.
/// This routine is used by the multiplayer sync checking, which verifies that every
/// machine in the game agrees on the rules data it was handed.
/// </summary>
// (0x0070EAB0)

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


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is used by the persistence system to record what kind of object was
/// written, so that the right class can be created when the save game is loaded.
/// </summary>
/// <param name="retval">Pointer to the class identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x0070EBF0)

HRESULT __stdcall VoxelAnimTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_VoxelAnimTypeClass;
    return S_OK;
}


/// <summary>
/// Loads this voxel animation type from the persist stream.
/// The base class reads the raw type data, then the pointers within are converted back
/// from the identifiers they were saved as, and the voxel image is re-fetched.
/// </summary>
/// <returns>Returns with the HRESULT of the load. S_OK if the type was rebuilt.</returns>
// (0x0070EC30)
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


/// <summary>
/// Saves this voxel animation type to the persist stream.
/// Everything a voxel animation type owns is written by the base class, so this routine
/// merely passes the request along.
/// </summary>
/// <returns>Returns with the HRESULT of the save. S_OK if the type was written.</returns>
// (0x0070ECF0)

HRESULT __stdcall VoxelAnimTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Removes the specified object from this voxel animation type.
/// This routine is called when an object is about to leave the game. Every reference
/// this type holds to it must be severed so that nothing is left pointing at it.
/// </summary>
// (0x0070ED10)
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


/// <summary>
/// Fetches the voxel animation type of the name specified, creating it if need be.
/// This routine is used when parsing the rules and the maps, where a reference may name
/// a voxel animation that has not been declared yet.
/// </summary>
/// <returns>Returns with a pointer to the voxel animation type of that name.</returns>
// (0x0070ED80)
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
