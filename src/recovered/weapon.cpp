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
      AmbientDamage(0),
      Burst(1),
      Projectile(0),
      Attack(0),
      MaxSpeed(0),
      Warhead(0),
      ROF(0),
      Range(0),
      MinimumRange(0),
      AssaultAnim(0),
      AttachedParticleSystem(0),
      UseFireParticles(false),
      UseSparkParticles(false),
      OmniFire(false),
      DistributedWeaponFire(false),
      IsRailgun(false),
      IsLobber(false),
      IsBright(false),
      IsSonic(false),
      Spawner(false),
      LimboLaunch(false),
      DecloakToFire(true),
      CellRangefinding(false),
      FireOnce(false),
      NeverUse(false),
      RevealOnFire(true),
      TerrainFire(false),
      SabotageCursor(false),
      DisguiseFireOnly(false),
      DisguiseFakeBlinkTime(0),
      Suicide(false),
      IsTurboBoosted(false),
      IsSupressed(false),
      IsCamera(false),
      IsElectric(false),
      IsLaser(false),
      IsBigLaser(false),
      IsHouseColor(false),
      LaserDuration(10),
      IsIonSensitive(false),
      AreaFire(false),
      IsElectricBolt(false),
      DrawBoltAsLaser(false),
      IsAlternateColor(false),
      IsRadBeam(false),
      IsRadEruption(false),
      RadLevel(0)
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
    LaserInnerColor(noinit),
    LaserOuterColor(noinit),
    LaserOuterSpread(noinit)
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
        AmbientDamage = ini.Get_Int(ID, "AmbientDamage", AmbientDamage);
        IsSonic = ini.Get_Bool(ID, "IsSonic", IsSonic);
        Spawner = ini.Get_Bool(ID, "Spawner", Spawner);
        LimboLaunch = ini.Get_Bool(ID, "LimboLaunch", LimboLaunch);
        DecloakToFire = ini.Get_Bool(ID, "DecloakToFire", DecloakToFire);
        CellRangefinding = ini.Get_Bool(ID, "CellRangefinding", CellRangefinding);
        FireOnce = ini.Get_Bool(ID, "FireOnce", FireOnce);
        NeverUse = ini.Get_Bool(ID, "NeverUse", NeverUse);
        RevealOnFire = ini.Get_Bool(ID, "RevealOnFire", RevealOnFire);
        TerrainFire = ini.Get_Bool(ID, "TerrainFire", TerrainFire);
        SabotageCursor = ini.Get_Bool(ID, "SabotageCursor", SabotageCursor);
        DisguiseFireOnly = ini.Get_Bool(ID, "DisguiseFireOnly", DisguiseFireOnly);
        DisguiseFakeBlinkTime = ini.Get_Int(ID, "DisguiseFakeBlinkTime", DisguiseFakeBlinkTime);
        Suicide = ini.Get_Bool(ID, "Suicide", Suicide);
        IsSupressed = ini.Get_Bool(ID, "Supress", IsSupressed);
        Burst = ini.Get_Int(ID, "Burst", Burst);
        Attack = ini.Get_Int(ID, "Damage", Attack);
        MaxSpeed = ini.Get_Percent(ID, "Speed", MaxSpeed);
        ROF = ini.Get_Int(ID, "ROF", ROF);
        Range = ini.Get_Lepton(ID, "Range", Range);
        MinimumRange = ini.Get_Lepton(ID, "MinimumRange", MinimumRange);
        unknown_bc = Get_Voc_List(ini, ID, "Report", unknown_bc);
        Anim = TypeListReader<AnimTypeClass>::Get(ini, ID, "Anim", Anim);
        AssaultAnim = Get_Type(ini, ID, "AssaultAnim", AssaultAnim);
        IsCamera = ini.Get_Bool(ID, "Camera", IsCamera);
        IsLaser = ini.Get_Bool(ID, "IsLaser", IsLaser);
        IsHouseColor = ini.Get_Bool(ID, "IsHouseColor", IsHouseColor);
        IsElectric = ini.Get_Bool(ID, "Charges", IsElectric);
        IsTurboBoosted = ini.Get_Bool(ID, "TurboBoost", IsTurboBoosted);
        UseFireParticles = ini.Get_Bool(ID, "UseFireParticles", UseFireParticles);
        UseSparkParticles = ini.Get_Bool(ID, "UseSparkParticles", UseSparkParticles);
        OmniFire = ini.Get_Bool(ID, "OmniFire", OmniFire);
        DistributedWeaponFire = ini.Get_Bool(ID, "DistributedWeaponFire", DistributedWeaponFire);
        IsRailgun = ini.Get_Bool(ID, "IsRailgun", IsRailgun);
        IsLobber = ini.Get_Bool(ID, "Lobber", IsLobber);
        LaserInnerColor = ini.Get_RGB(ID, "LaserInnerColor", LaserInnerColor);
        LaserOuterColor = ini.Get_RGB(ID, "LaserOuterColor", LaserOuterColor);
        LaserOuterSpread = ini.Get_RGB(ID, "LaserOuterSpread", LaserOuterSpread);
        LaserDuration = ini.Get_Int(ID, "LaserDuration", LaserDuration);
        IsBigLaser = ini.Get_Bool(ID, "IsBigLaser", IsBigLaser);
        IsBright = ini.Get_Bool(ID, "Bright", IsBright);
        IsIonSensitive = ini.Get_Bool(ID, "IonSensitive", IsIonSensitive);
        AreaFire = ini.Get_Bool(ID, "AreaFire", AreaFire);
        IsElectricBolt = ini.Get_Bool(ID, "IsElectricBolt", IsElectricBolt);
        DrawBoltAsLaser = ini.Get_Bool(ID, "DrawBoltAsLaser", DrawBoltAsLaser);
        IsAlternateColor = ini.Get_Bool(ID, "IsAlternateColor", IsAlternateColor);
        IsRadBeam = ini.Get_Bool(ID, "IsRadBeam", IsRadBeam);
        IsRadEruption = ini.Get_Bool(ID, "IsRadEruption", IsRadEruption);
        RadLevel = ini.Get_Int(ID, "RadLevel", RadLevel);
        char system[20];
        system[0] = 0;
        ini.Get_String(ID, "AttachedParticleSystem", system, system, sizeof(system));
        int index = ParticleSystemTypeClass::Index_Or_Make(system);
        if (index != -1) {
            AttachedParticleSystem = ParticleSystemTypes[index];
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
        double gravity = Rules->Gravity;
        if (Projectile->Floater) {
            gravity = Get_Floater_Gravity();
        }
        MaxSpeed = Calculate_Projectile_Speed(Range, gravity);
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
    crc((long)AmbientDamage);
    crc(IsSonic);
    crc(IsTurboBoosted);
    crc(IsSupressed);
    crc(IsCamera);
    crc(IsLaser);
    crc(IsHouseColor);
    crc((long)Burst);
    if (Projectile != 0) {
        crc(Projectile->Fetch_ID());
    }
    crc((long)Attack);
    crc((long)MaxSpeed);
    if (Warhead != 0) {
        crc(Warhead->Fetch_ID());
    }
    crc((long)ROF);
    crc((long)Range);
    crc((long)unknown_bc.Count());
    crc((long)Anim.Count());
    crc(UseFireParticles);
    crc(IsLobber);
    crc((char)LaserDuration);
    crc(IsBigLaser);
    crc(IsRailgun);
    crc(IsElectric);
    crc(IsBright);
    crc(UseSparkParticles);
    crc(OmniFire);
    crc(DistributedWeaponFire);
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
    Swizzle_Pointer(AttachedParticleSystem);
    Swizzle_Pointer(AssaultAnim);
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
        double gravity = Projectile->Floater ? Get_Floater_Gravity() : Rules->Gravity;
        return Calculate_Projectile_Speed(range, gravity);
    }
    return MaxSpeed;
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
