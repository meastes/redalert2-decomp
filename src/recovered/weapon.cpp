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
// Function comment blocks from mischa85/RedAlert2 392e904 src/weapon.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/weapon.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/weapon.cpp where both functions declare the same local types (names only).

// weapon.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/weapon.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/weapon_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/anim_type.hpp"


// Header-static dynamic initializers of the unit at 0x00732CB0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00732CB0-0x00732E96); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00732CE0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732D00, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732D20, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732D40, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732D60, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732D80, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732DC0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732DF0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732E20, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732E50, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732E60, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// Dynamic initializer (0x00732E80, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00732cb0.cpp; no source of its own.


// WeaponTypeClass::WeaponTypeClass(char const *ini_name) (0x00732EA0)

WeaponTypeClass::WeaponTypeClass(char const *ini_name)
    : AbstractTypeClass(ini_name),
      unknown_98(0),
      Burst(1),
      Projectile(0),
      unknown_a4(0),
      unknown_a8(0),
      Warhead(0),
      unknown_b0(0),
      unknown_b4(0),
      unknown_b8(0),
      unknown_f4(0),
      unknown_f8(0),
      unknown_105(false),
      unknown_106(false),
      unknown_107(false),
      unknown_108(false),
      unknown_109(false),
      unknown_10a(false),
      unknown_10b(false),
      unknown_10c(false),
      unknown_10d(false),
      unknown_10e(false),
      unknown_10f(true),
      unknown_110(false),
      unknown_111(false),
      unknown_112(false),
      unknown_113(true),
      unknown_114(false),
      unknown_115(false),
      unknown_116(false),
      unknown_118(0),
      unknown_11c(false),
      unknown_11d(false),
      unknown_11e(false),
      unknown_11f(false),
      unknown_120(false),
      unknown_121(false),
      unknown_122(false),
      unknown_123(false),
      unknown_124(10),
      unknown_125(false),
      AreaFire(false),
      unknown_127(false),
      unknown_128(false),
      unknown_129(false),
      unknown_12a(false),
      unknown_12b(false),
      unknown_12c(0)
{
    AbstractClass::Create_ID();
    Weapons.Add(this);
}


/***********************************************************************************************
 * WeaponTypeClass::WeaponTypeClass -- Default constructor for weapon type objects.            *
 *                                                                                             *
 *    This default constructor will initialize all the values of the weapon to the default     *
 *    state. Thus, if any of these settings are not specifically overridden by the rules.ini   *
 *    file, they will remain this value in the game.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x007330D0)

WeaponTypeClass::WeaponTypeClass(NoInitClass const &noinit) :
    AbstractTypeClass(noinit),
    unknown_bc(noinit),
    Anim(noinit),
    unknown_fc(noinit),
    unknown_ff(noinit),
    unknown_102(noinit)
{
}


/***********************************************************************************************
 * WeaponTypeClass::~WeaponTypeClass -- Destructor for weapon type class objects.              *
 *                                                                                             *
 *    This destructor really doesn't do anything but set the pointers to NULL. This is a       *
 *    general purposes safety tactic but is otherwise useless.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00733120)
// Detaches the weapon type everywhere, forgets its projectile and warhead and
// leaves the weapon type list.

WeaponTypeClass::~WeaponTypeClass()
{
    Detach_This_From_All(this, true);
    Projectile = 0;
    Warhead = 0;
    Weapons.Delete(this);
}


/***********************************************************************************************
 * WeaponTypeClass::Read_INI -- Fetch the weapon data from the INI database.                   *
 *                                                                                             *
 *    This routine will fetch the weapon data for this weapon type object from the INI         *
 *    database specified.                                                                      *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that the weapon data will be fetched        *
 *                   from.                                                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was this weapon type described in the database and the values retrieved?     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00733220)
// Reads the weapon's rules.ini section; the section must exist.

bool WeaponTypeClass::Read_INI(CCINIClass &ini)
{
    if (ini.Is_Present(ID)) {
        unknown_98 = ini.Get_Int(ID, "AmbientDamage", unknown_98);
        unknown_10c = ini.Get_Bool(ID, "IsSonic", unknown_10c);
        unknown_10d = ini.Get_Bool(ID, "Spawner", unknown_10d);
        unknown_10e = ini.Get_Bool(ID, "LimboLaunch", unknown_10e);
        unknown_10f = ini.Get_Bool(ID, "DecloakToFire", unknown_10f);
        unknown_110 = ini.Get_Bool(ID, "CellRangefinding", unknown_110);
        unknown_111 = ini.Get_Bool(ID, "FireOnce", unknown_111);
        unknown_112 = ini.Get_Bool(ID, "NeverUse", unknown_112);
        unknown_113 = ini.Get_Bool(ID, "RevealOnFire", unknown_113);
        unknown_114 = ini.Get_Bool(ID, "TerrainFire", unknown_114);
        unknown_115 = ini.Get_Bool(ID, "SabotageCursor", unknown_115);
        unknown_116 = ini.Get_Bool(ID, "DisguiseFireOnly", unknown_116);
        unknown_118 = ini.Get_Int(ID, "DisguiseFakeBlinkTime", unknown_118);
        unknown_11c = ini.Get_Bool(ID, "Suicide", unknown_11c);
        unknown_11e = ini.Get_Bool(ID, "Supress", unknown_11e);
        Burst = ini.Get_Int(ID, "Burst", Burst);
        unknown_a4 = ini.Get_Int(ID, "Damage", unknown_a4);
        unknown_a8 = ini.Get_Percent(ID, "Speed", unknown_a8);
        unknown_b0 = ini.Get_Int(ID, "ROF", unknown_b0);
        unknown_b4 = ini.Get_Lepton(ID, "Range", unknown_b4);
        unknown_b8 = ini.Get_Lepton(ID, "MinimumRange", unknown_b8);
        unknown_bc = Get_Voc_List(ini, ID, "Report", unknown_bc);
        Anim = TypeListReader<AnimTypeClass>::Get(ini, ID, "Anim", Anim);
        unknown_f4 = Get_Type(ini, ID, "AssaultAnim", unknown_f4);
        unknown_11f = ini.Get_Bool(ID, "Camera", unknown_11f);
        unknown_121 = ini.Get_Bool(ID, "IsLaser", unknown_121);
        unknown_123 = ini.Get_Bool(ID, "IsHouseColor", unknown_123);
        unknown_120 = ini.Get_Bool(ID, "Charges", unknown_120);
        unknown_11d = ini.Get_Bool(ID, "TurboBoost", unknown_11d);
        unknown_105 = ini.Get_Bool(ID, "UseFireParticles", unknown_105);
        unknown_106 = ini.Get_Bool(ID, "UseSparkParticles", unknown_106);
        unknown_107 = ini.Get_Bool(ID, "OmniFire", unknown_107);
        unknown_108 = ini.Get_Bool(ID, "DistributedWeaponFire", unknown_108);
        unknown_109 = ini.Get_Bool(ID, "IsRailgun", unknown_109);
        unknown_10a = ini.Get_Bool(ID, "Lobber", unknown_10a);
        unknown_fc = ini.Get_RGB(ID, "LaserInnerColor", unknown_fc);
        unknown_ff = ini.Get_RGB(ID, "LaserOuterColor", unknown_ff);
        unknown_102 = ini.Get_RGB(ID, "LaserOuterSpread", unknown_102);
        unknown_124 = ini.Get_Int(ID, "LaserDuration", unknown_124);
        unknown_122 = ini.Get_Bool(ID, "IsBigLaser", unknown_122);
        unknown_10b = ini.Get_Bool(ID, "Bright", unknown_10b);
        unknown_125 = ini.Get_Bool(ID, "IonSensitive", unknown_125);
        AreaFire = ini.Get_Bool(ID, "AreaFire", AreaFire);
        unknown_127 = ini.Get_Bool(ID, "IsElectricBolt", unknown_127);
        unknown_128 = ini.Get_Bool(ID, "DrawBoltAsLaser", unknown_128);
        unknown_129 = ini.Get_Bool(ID, "IsAlternateColor", unknown_129);
        unknown_12a = ini.Get_Bool(ID, "IsRadBeam", unknown_12a);
        unknown_12b = ini.Get_Bool(ID, "IsRadEruption", unknown_12b);
        unknown_12c = ini.Get_Int(ID, "RadLevel", unknown_12c);
        char system[20];
        system[0] = 0;
        ini.Get_String(ID, "AttachedParticleSystem", system, system, sizeof(system));
        int index = ParticleSystemTypeClass::Index_Or_Make(system);
        if (index != -1) {
            unknown_f8 = ParticleSystemTypes[index];
        }
        Warhead = Get_Type(ini, ID, "Warhead", Warhead);
        Projectile = Get_Type(ini, ID, "Projectile", Projectile);
        return true;
    }
    return false;
}


/// <summary>
/// Sets the launch speed for a ballistic weapon.
/// A weapon that throws an unguided projectile must launch it hard enough to carry it the
/// weapon's full range, so the speed is worked out from that range and the gravity the
/// projectile falls under. A guided projectile steers itself and is left alone.
/// </summary>
/// <remarks>Only call this routine once the rules have been read, since it leans on them.</remarks>
// (0x007339C0)
// Derives unknown_a8 from the range and the projectile's speed (rules
// default, or 0x0047FCD0 for floaters); nothing for homing (ROT) projectiles.

void WeaponTypeClass::Init_Max_Speed()
{
    if (Projectile != 0 && Projectile->ROT == 0) {
        double gravity = Rules->unknown_1320;
        if (Projectile->Floater) {
            gravity = Get_Floater_Gravity();
        }
        unknown_a8 = Calculate_Projectile_Speed(unknown_b4, gravity);
    }
}


/***********************************************************************************************
 * Armor_From_Name -- Convert ASCII name into armor type number.                               *
 *                                                                                             *
 *    This will find the armor type that matches the ASCII name specified and then will return *
 *    the armor ID number of it.                                                               *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the armor to find.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the armor ID number of the armor that matches the name specified. If  *
 *          no match could be found, then ARMOR_NONE is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00733A20)

ArmorType __fastcall Armor_From_Name(char const *name)
{
    if (name != 0) {
        for (int armor = 0; armor < 11; armor++) {
            if (!_stricmp(ArmorNames[armor], name)) {
                return (ArmorType)armor;
            }
        }
    }
    return ARMOR_NONE;
}


/***********************************************************************************************
 * WeaponTypeClass::Allowed_Threats -- Determine what threats this weapon can address.         *
 *                                                                                             *
 *    This routine will examine the capabilities of this weapon and return with the threat     *
 *    types that it can address.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the threat types that this weapon can address.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00733A60)
// Flags from the projectile type (4 for +0x274, 0xB8 added for +0x275).

int WeaponTypeClass::Allowed_Threats() const
{
    int flags = 0;
    if (Projectile->AA) {
        flags = 4;
    }
    if (Projectile->AG) {
        flags |= 0xB8;
    }
    return flags;
}


/// <summary>
/// Can this weapon knock down a wall?
/// The answer comes from the warhead the weapon fires, and it is used when deciding whether
/// an object should bother attacking a wall that is in its way.
/// </summary>
/// <returns>bool; Will this weapon destroy walls?</returns>
// (0x00733A90)

bool WeaponTypeClass::Is_Wall_Destroyer() const
{
    if (Warhead && Warhead->Wall) {
        return true;
    }
    return false;
}


/// <summary>
/// Adds this weapon type's data to the CRC calculation.
/// This routine is used by the multiplayer sync checker to prove that every machine agrees
/// about the weapon, so the projectile and warhead it refers to are submitted by identifier
/// rather than by pointer.
/// </summary>
/// <param name="crc">The checksum engine to submit the data to.</param>
// (0x00733AB0)

void WeaponTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)unknown_98);
    crc(unknown_10c);
    crc(unknown_11d);
    crc(unknown_11e);
    crc(unknown_11f);
    crc(unknown_121);
    crc(unknown_123);
    crc((long)Burst);
    if (Projectile != 0) {
        crc(Projectile->Fetch_ID());
    }
    crc((long)unknown_a4);
    crc((long)unknown_a8);
    if (Warhead != 0) {
        crc(Warhead->Fetch_ID());
    }
    crc((long)unknown_b0);
    crc((long)unknown_b4);
    crc((long)unknown_bc.Count());
    crc((long)Anim.Count());
    crc(unknown_105);
    crc(unknown_10a);
    crc((char)unknown_124);
    crc(unknown_122);
    crc(unknown_109);
    crc(unknown_120);
    crc(unknown_10b);
    crc(unknown_106);
    crc(unknown_107);
    crc(unknown_108);
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is used by the persistence machinery to recognize what kind of object it
/// is about to load back.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x00733C50)

HRESULT __stdcall WeaponTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_WeaponTypeClass;
    return S_OK;
}


/// <summary>
/// Loads this weapon type from the save game stream.
/// A weapon type saved by an older version of the game lacks the projectile range and
/// burst delay data, so such a save is patched up to the current layout as it is read. The
/// object is then rebuilt in place, its sound and animation lists are read back, and the
/// saved pointers are remapped to the objects of this session.
/// </summary>
/// <returns>Returns with S_OK if the weapon type was loaded, otherwise the failure code.</returns>
// (0x00733C90)
// Clears the sound list, loads the base object, restores the weapon type in
// place, rebuilds both lists from their saved counts and entries, and swizzles
// the projectile, warhead, attached particle system, assault animation and the
// animation list entries.

HRESULT __stdcall WeaponTypeClass::Load(IStream *stream)
{
    unknown_bc.Clear();
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) WeaponTypeClass(NoInitClass());
    Anim.Load(stream);
    unknown_bc.Load(stream);
    Swizzle_Pointer(Warhead);
    Swizzle_Pointer(Projectile);
    Swizzle_Pointer(unknown_f8);
    Swizzle_Pointer(unknown_f4);
    for (int index = 0; index < Anim.Count(); index++) {
        Swizzle_Pointer(Anim[index]);
    }
    return S_OK;
}


/// <summary>
/// Writes this weapon type out to the save game stream.
/// The base class writes the raw object image, and this routine then writes out the
/// report sound and firing animation lists that hang off it.
/// </summary>
/// <param name="cleardirty">Should the object be marked as clean once it has been written?</param>
/// <returns>Returns with S_OK if the weapon type was written, otherwise the failure code.</returns>
// (0x00733DF0)
// The base object, then the two lists as counts and elements.

HRESULT __stdcall WeaponTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = AbstractTypeClass::Save(stream, cleardirty);
    if (FAILED(result)) {
        return result;
    }
    int count = Anim.Count();
    stream->Write(&count, sizeof(count), 0);
    for (int i = 0; i < count; i++) {
        stream->Write(&Anim[i], sizeof(AnimTypeClass *), 0);
    }
    count = unknown_bc.Count();
    stream->Write(&count, sizeof(count), 0);
    for (i = 0; i < count; i++) {
        stream->Write(&unknown_bc[i], sizeof(int), 0);
    }
    return S_OK;
}


/// <summary>
/// Fetches the weapon of the name specified, creating it if it does not exist yet.
/// Use this routine while processing the rules so that a weapon mentioned before its own
/// section has been read will still resolve to a real object.
/// </summary>
/// <param name="name">The INI name of the weapon desired.</param>
/// <returns>Returns with a pointer to the weapon type of that name.</returns>
// (0x00733EA0)
// The named weapon type, created when missing; 0 for "<none>" and "none".

WeaponTypeClass *__fastcall WeaponTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name) || !_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < Weapons.Count(); index++) {
        if (!_stricmp(Weapons[index]->ID, name)) {
            return Weapons[index];
        }
    }
    return new WeaponTypeClass(name);
}


// int __fastcall WeaponTypeClass::Index_From_Name(char const *name) (0x00733F30)

int __fastcall WeaponTypeClass::Index_From_Name(char const *name)
{
    for (int index = 0; index < Weapons.Count(); index++) {
        if (!_stricmp(Weapons[index]->ID, name)) {
            return index;
        }
    }
    return -1;
}


/**
 *  RA2: The speed of the projectile for the range given. Ballistic projectiles (no rate of
 *  turn) are given the speed needed to cover the range under gravity.
 */
// (0x00733F70)
// Init_Max_Speed's value for another range.

int WeaponTypeClass::Get_Speed(int range) const
{
    if (Projectile != 0 && Projectile->ROT == 0) {
        double gravity = Projectile->Floater ? Get_Floater_Gravity() : Rules->unknown_1320;
        return Calculate_Projectile_Speed(range, gravity);
    }
    return unknown_a8;
}


// int WeaponTypeClass::Size_Of() const (0x00733FD0)

int WeaponTypeClass::Size_Of() const
{
    return sizeof(WeaponTypeClass);
}


// int WeaponTypeClass::Kind_Of() const (0x00733FE0)

int WeaponTypeClass::Kind_Of() const
{
    return 49;
}
