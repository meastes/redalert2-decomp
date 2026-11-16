/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/bullettype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/bullettype.cpp where both define the function (names only).

// bullettype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bullettype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/bullet_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/map_view.hpp"


// Header-static dynamic initializers of the unit at 0x004642F0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x004642F0-0x004644D1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00464310, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x00464340, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x00464360, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x00464380, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x004643A0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x004643C0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x004643E0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x00464420, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x00464450, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x00464480, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x004644B0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// Dynamic initializer (0x004644C0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_004642f0.cpp; no source of its own.


// BulletTypeClass::BulletTypeClass(char const *ini_name) (0x004644E0)
// The projectile defaults (shadowed, rotating, anti-ground; one cluster,
// elasticity 0.75, acceleration and spawn delay 3), the object-type flags a
// projectile overrides, a new ID, and both type lists.

BulletTypeClass::BulletTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name),
    Airburst(false),
    Floater(false),
    SubjectToCliffs(false),
    SubjectToElevation(false),
    SubjectToWalls(false),
    VeryHigh(false),
    Shadow(true),
    Arcing(false),
    Dropping(false),
    Level(false),
    Inviso(false),
    Proximity(false),
    Ranged(false),
    Rotates(true),
    Inaccurate(false),
    FlakScatter(false),
    AA(false),
    AG(true),
    Degenerates(false),
    Bouncy(false),
    AnimPalette(false),
    FirersPalette(false),
    Cluster(1),
    AirburstWeapon(0),
    ShrapnelWeapon(0),
    ShrapnelCount(0),
    DetonationAltitude(0),
    Vertical(false),
    Elasticity(0.75),
    Acceleration(3),
    Color(0),
    Trailer(0),
    ROT(0),
    CourseLockDuration(0),
    SpawnDelay(3),
    unknown_2b8(0),
    Scalable(false),
    Arm(0),
    AnimLow(0),
    AnimHigh(0),
    AnimRate(0),
    Flat(false)
{
    Create_ID();
    unknown_204 = true;
    unknown_1ff = true;
    unknown_200 = false;
    unknown_201 = false;
    unknown_202 = true;
    unknown_203 = true;
    unknown_205 = false;
    BulletTypes.Add(this);
    AbstractTypePtrTracker.Add(this);
}


/***********************************************************************************************
 * BulletTypeClass::BulletTypeClass -- Constructor for bullet type objects.                    *
 *                                                                                             *
 *    This is basically a constructor for static type objects used by bullets. All bullets     *
 *    are of a type constructed by this routine at game initialization time.                   *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   07/17/1996 JLB : Uses correct default values.                                             *
 *=============================================================================================*/
// (0x00464700)

BulletTypeClass::BulletTypeClass(NoInitClass const &noinit) :
    ObjectTypeClass(noinit)
{
}


/// <summary>
/// Removes this bullet type from the game.
/// This routine severs any references other objects hold to this bullet type before
/// dropping it from the type databases.
/// </summary>
// (0x00464730)

BulletTypeClass::~BulletTypeClass()
{
    Detach_This_From_All((ObjectClass *)this, true);
    AbstractTypePtrTracker.Delete((ObjectClass *)this);
    BulletTypes.Delete(this);
}


// bool BulletTypeClass::Read_INI(CCINIClass &ini) (0x00464800)
// The rules keys, then (when the type names an image) the art keys of the
// image section in ArtINI.

static inline AnimTypeClass *Get_AnimType(CCINIClass &ini, char const *section, char const *entry, AnimTypeClass *defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) {
        return Find_Or_Make_AnimType_004264a0(buffer);
    }
    return defvalue;
}

static inline WeaponTypeClass *Get_WeaponType(CCINIClass &ini, char const *section, char const *entry, WeaponTypeClass *defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) {
        return WeaponTypeClass::Find_Or_Make(buffer);
    }
    return defvalue;
}

/***********************************************************************************************
 * BulletTypeClass::Read_INI -- Fetch the bullet type data from the INI database.              *
 *                                                                                             *
 *    Use this routine to fetch override information about this bullet type class object       *
 *    from the INI database specified.                                                         *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to examine.                                 *
 *                                                                                             *
 * OUTPUT:  bool; Was the section for this bullet found and the data extracted?                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BulletTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        char const *section = ID;
        Arm = ini.Get_Int(section, "Arm", Arm);
        ROT = ini.Get_Int(section, "ROT", ROT);
        CourseLockDuration = ini.Get_Int(section, "CourseLockDuration", CourseLockDuration);
        Elasticity = ini.Get_Double(section, "Elasticity", Elasticity);
        Acceleration = ini.Get_Int(section, "Acceleration", Acceleration);
        Color = ini.Get_ColorScheme(section, "Color", Color);
        Arcing = ini.Get_Bool(section, "Arcing", Arcing);
        Floater = ini.Get_Bool(section, "Floater", Floater);
        SubjectToCliffs = ini.Get_Bool(section, "SubjectToCliffs", SubjectToCliffs);
        SubjectToElevation = ini.Get_Bool(section, "SubjectToElevation", SubjectToElevation);
        SubjectToWalls = ini.Get_Bool(section, "SubjectToWalls", SubjectToWalls);
        VeryHigh = ini.Get_Bool(section, "VeryHigh", VeryHigh);
        Shadow = ini.Get_Bool(section, "Shadow", Shadow);
        Dropping = ini.Get_Bool(section, "Dropping", Dropping);
        Level = ini.Get_Bool(section, "Level", Level);
        Inviso = ini.Get_Bool(section, "Inviso", Inviso);
        Proximity = ini.Get_Bool(section, "Proximity", Proximity);
        Ranged = ini.Get_Bool(section, "Ranged", Ranged);
        Inaccurate = ini.Get_Bool(section, "Inaccurate", Inaccurate);
        FlakScatter = ini.Get_Bool(section, "FlakScatter", FlakScatter);
        AA = ini.Get_Bool(section, "AA", AA);
        AG = ini.Get_Bool(section, "AG", AG);
        Degenerates = ini.Get_Bool(section, "Degenerates", Degenerates);
        Bouncy = ini.Get_Bool(section, "Bouncy", Bouncy);
        Airburst = ini.Get_Bool(section, "Airburst", Airburst);
        Cluster = ini.Get_Int(section, "Cluster", Cluster);
        Scalable = ini.Get_Bool(section, "Scalable", Scalable);

        char *graphic = unknown_1c8.Buffer;
        if (ini.Get_String(section, "Image", unknown_string_83c55c, graphic, sizeof(unknown_1c8.Buffer)) > 0) {
            Trailer = Get_AnimType(ArtINI, graphic, "Trailer", Trailer);
            SpawnDelay = ArtINI.Get_Int(graphic, "SpawnDelay", SpawnDelay);
            Rotates = !ArtINI.Get_Bool(graphic, "Rotates", !Rotates);
            Flat = ArtINI.Get_Bool(graphic, "Flat", Flat);
        }
        AirburstWeapon = Get_WeaponType(ini, section, "AirburstWeapon", AirburstWeapon);
        ShrapnelWeapon = Get_WeaponType(ini, section, "ShrapnelWeapon", ShrapnelWeapon);
        ShrapnelCount = ini.Get_Int(section, "ShrapnelCount", ShrapnelCount);
        DetonationAltitude = ini.Get_Int(section, "DetonationAltitude", DetonationAltitude);
        Vertical = ini.Get_Bool(section, "Vertical", Vertical);
        FirersPalette = ini.Get_Bool(section, "FirersPalette", FirersPalette);
        AnimLow = ArtINI.Get_Int(graphic, "AnimLow", AnimLow);
        AnimHigh = ArtINI.Get_Int(graphic, "AnimHigh", AnimHigh);
        AnimRate = ArtINI.Get_Int(graphic, "AnimRate", AnimRate);
        AnimPalette = ArtINI.Get_Bool(graphic, "AnimPalette", AnimPalette);
        if (!Inviso) {
            Fetch_Normal_Image();
        }
        if (unknown_206) {
            Fetch_Voxel_Image();
        }
        return true;
    }
    return false;
}


// int __fastcall BulletTypeClass::Index_Or_Make(char const *name) (0x00464D50)
// The index of the named bullet type in BulletTypes, created when missing;
// -1 for "<none>".

int __fastcall BulletTypeClass::Index_Or_Make(char const *name)
{
    if (_stricmp(name, "<none>") == 0) {
        return -1;
    }
    for (int index = 0; index < BulletTypes.Count(); index++) {
        if (_stricmp(name, BulletTypes[index]->ID) == 0) {
            return index;
        }
    }
    BulletTypeClass *type = new BulletTypeClass(name);
    return BulletTypes.ID(type);
}


/// <summary>
/// Converts a bullet type number into its ASCII name.
/// </summary>
/// <param name="bullet">The bullet type to fetch the name of.</param>
/// <returns>Returns with the name of the bullet type. If the type is not a legal one,
/// then NULL is returned.</returns>
// (0x00464DE0)

char const *__fastcall BulletTypeClass::Name_From(int bullet)
{
    if ((unsigned)bullet < (unsigned)BulletTypes.Count()) {
        return BulletTypes[bullet]->ID;
    }
    return 0;
}


/// <summary>
/// Adjusts a coordinate so that it is legal for a bullet of this type.
/// This routine lifts the coordinate clear of the terrain, so that a bullet is never
/// created or moved to a spot buried in the landscape.
/// </summary>
/// <param name="coord">The coordinate to be adjusted.</param>
/// <returns>Returns with the adjusted coordinate.</returns>
// (0x00464E00)
// The coordinate, raised above the ground height when it is not above it.

CoordStruct BulletTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    if (result.Z <= Map.Get_Height_GL(result)) {
        result.Z = Map.Get_Height_GL(result) + 1;
    }
    return result;
}


/// <summary>
/// Submits this bullet type's data to the running checksum.
/// This routine is used by the multiplayer sync check to prove that every machine is
/// playing by the same bullet rules.
/// </summary>
/// <param name="crc">The checksum engine to submit this object's data to.</param>
// (0x00464E70, slot 13)
// The base CRC, then the flight flags and values (Westwood's selection).
// Extent: the inventory stops at 0x00464F52; a linear sweep reaches the ret 4
// at 0x00464F9F, followed by nop padding to the Load at 0x00464FB0.

void BulletTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc(Airburst);
    crc(Shadow);
    crc(Arcing);
    crc(Dropping);
    crc(Level);
    crc(Inviso);
    crc(Proximity);
    crc(Ranged);
    crc(Rotates);
    crc(Inaccurate);
    crc(FlakScatter);
    crc(AA);
    crc(AG);
    crc(Degenerates);
    crc(Bouncy);
    crc(Elasticity);
    crc((long)Acceleration);
    crc((long)ROT);
    crc((long)Arm);
    crc(Flat);
}


/// <summary>
/// Reads this bullet type back in from the save game stream.
/// This routine copes with save games written before the later members existed, and then
/// restores the parts of the object that a save file cannot carry -- the artwork and the
/// pointers to other type objects.
/// </summary>
/// <returns>Returns with S_OK if the object was read successfully.</returns>
// (0x00464FB0)
// Restores the type in place, reloads its voxel and image and swizzles its
// trailer and weapon pointers.

HRESULT __stdcall BulletTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) BulletTypeClass(NoInitClass());
    Fetch_Voxel_Image();
    Fetch_Normal_Image();
    Swizzler.Swizzle((void **)&Trailer);
    Swizzler.Swizzle((void **)&AirburstWeapon);
    Swizzler.Swizzle((void **)&ShrapnelWeapon);
    return S_OK;
}


/// <summary>
/// Writes this bullet type out to the save game stream.
/// </summary>
/// <param name="cleardirty">Should the object be marked as clean once written?</param>
/// <returns>Returns with S_OK if the object was written successfully.</returns>
// (0x00465040)

HRESULT __stdcall BulletTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Fetches the class identifier that the save game code stores for this object.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x00465060)

HRESULT __stdcall BulletTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_BulletTypeClass;
    return S_OK;
}


/// <summary>
/// Fetches the bullet type of the name specified, creating it if need be.
/// This routine is used by the rules parser when it meets a bullet name that has not been
/// declared yet, so that bullets may be mentioned in any order.
/// </summary>
/// <param name="name">Pointer to the ASCII name of the bullet type wanted.</param>
/// <returns>Returns with a pointer to the bullet type of that name.</returns>
// (0x004650A0)
// The named bullet type, created when missing; 0 for "<none>" and "none".

BulletTypeClass *__fastcall BulletTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name) || !_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < BulletTypes.Count(); index++) {
        if (!_stricmp(BulletTypes[index]->ID, name)) {
            return BulletTypes[index];
        }
    }
    return new BulletTypeClass(name);
}


/// <summary>
/// Removes any reference this bullet type holds to the object specified.
/// This routine is called when an object is about to leave the game, so that no bullet
/// type is left pointing at it.
/// </summary>
/// <param name="target">Pointer to the object that is going away.</param>
// (0x00465130)

void BulletTypeClass::Detach(AbstractClass *target, bool all)
{
    if (target == (AbstractClass *)Trailer) {
        Trailer = 0;
    }
}


// void BulletTypeClass::Set_unknown_2b8(int value) (0x00465150)

void BulletTypeClass::Set_unknown_2b8(int value)
{
    unknown_2b8 = value;
}


// int BulletTypeClass::Kind_Of() const (0x00465160)

int BulletTypeClass::Kind_Of() const
{
    return 9;
}


// int BulletTypeClass::Size_Of() const (0x00465170)

int BulletTypeClass::Size_Of() const
{
    return sizeof(BulletTypeClass);
}


// bool BulletTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x00465180)
// Never places a bullet type directly (RA1: Create_And_Place).

bool BulletTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    return false;
}


// ObjectClass *BulletTypeClass::Create_One_Of(HouseClass *house) const (0x00465190)
// Creates nothing (RA1: Create_One_Of).

ObjectClass *BulletTypeClass::Create_One_Of(HouseClass *house) const
{
    return 0;
}


// BulletTypeClass scalar deleting destructor (0x004651A0): compiler generated (the destructor expanded inline); no source of its own.
