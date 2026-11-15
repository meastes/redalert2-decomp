/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/suprtype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/suprtype.cpp where both define the function (names only).

// suprtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/suprtype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/super_weapon_type.hpp"


// Header-static dynamic initializers of the unit at 0x0069EDD0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0069EDD0-0x0069EFB1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0069EDF0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EE20, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EE40, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EE60, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EE80, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EEA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EEC0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EF00, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EF30, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EF60, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EF90, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// Dynamic initializer (0x0069EFA0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0069edd0.cpp; no source of its own.


// int __fastcall SuperWeaponTypeClass::Type_From_Name(char const *name) (0x0069EFC0)

int __fastcall SuperWeaponTypeClass::Type_From_Name(char const *name)
{
    for (int index = 0; index < 7; index++) {
        if (_stricmp(SuperWeaponTypeNames[index], name) == 0) {
            return index;
        }
    }
    return -1;
}


// SuperWeaponTypeClass::SuperWeaponTypeClass(char const *ini_name) (0x0069F000)
// Defaults (4500-frame recharge, powered); registers the type in
// SuperWeaponTypes; the sidebar image starts as the ID.

SuperWeaponTypeClass::SuperWeaponTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    WeaponType(0),
    unknown_0xA0(-1),
    unknown_0xA4(-1),
    unknown_0xA8(-1),
    unknown_0xAC(-1),
    RechargeTime(4500),
    Type(-1),
    SidebarIcon(0),
    Action(0),
    AuxBuilding(0),
    UseChargeDrain(false),
    IsPowered(true),
    DisableableFromShell(false),
    PreClick(false),
    PostClick(false),
    PreDependent(-1),
    ShowTimer(false),
    ManualControl(false)
{
    Create_ID();
    ArrayIndex = SuperWeaponTypes.Count();
    SuperWeaponTypes.Add(this);
    SidebarImage = ID;
}


/// <summary>
/// Constructs a super weapon type of the rules name specified.
/// The new type is given a heap identifier and added to the super weapon type heap. Only
/// harmless defaults are installed here; Read_INI supplies the real characteristics.
/// </summary>
/// <param name="ininame">The rules section this super weapon is declared by.</param>
// (0x0069F130)

SuperWeaponTypeClass::SuperWeaponTypeClass(NoInitClass const &noinit) :
    AbstractTypeClass(noinit),
    SidebarIcon(0),
    SidebarImage(noinit)
{
}


/// <summary>
/// Removes this super weapon type from the game.
/// The type is unlinked from the super weapon type heap so that nothing can reach it
/// afterwards.
/// </summary>
// (0x0069F170)

SuperWeaponTypeClass::~SuperWeaponTypeClass()
{
    SuperWeaponTypes.Delete(this);
}


/// <summary>
/// Fetches the class identifier of this object.
/// The save game system uses this to know which class to construct when the object is
/// read back in.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x0069F1F0)

HRESULT __stdcall SuperWeaponTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_SuperWeaponTypeClass;
    return S_OK;
}


/// <summary>
/// Reads this super weapon type back from the save game stream.
/// The weapon and auxiliary building references are remapped to their new addresses, and
/// the sidebar cameo is fetched from the mix files again since artwork is never written
/// to a save game.
/// </summary>
/// <returns>Returns with S_OK if the object was read successfully.</returns>
// (0x0069F230)
// Rebuilds the type in place, fixes up the pointers and reloads the sidebar icon.

HRESULT __stdcall SuperWeaponTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) SuperWeaponTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&WeaponType);
    Swizzler.Swizzle((void **)&AuxBuilding);
    char fullname[512];
    _makepath(fullname, 0, 0, SidebarImage, ".SHP");
    SidebarIcon = MixFileClass::Retrieve(fullname);
    if (SidebarIcon == 0) {
        SidebarIcon = MixFileClass::Retrieve("XXICON.SHP");
    }
    return S_OK;
}


/// <summary>
/// Writes this super weapon type to the save game stream.
/// </summary>
/// <param name="cleardirty">Should the object be marked as clean once written?</param>
/// <returns>Returns with S_OK if the object was written successfully.</returns>
// (0x0069F300)

HRESULT __stdcall SuperWeaponTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = AbstractClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// int SuperWeaponTypeClass::Kind_Of() const (0x0069F320): the super weapon type RTTI kind.

int SuperWeaponTypeClass::Kind_Of() const
{
    return 0x20;
}


// int SuperWeaponTypeClass::Size_Of() const (0x0069F330)

int SuperWeaponTypeClass::Size_Of() const
{
    return sizeof(SuperWeaponTypeClass);
}


/// <summary>
/// Submits this super weapon type to the game state checksum.
/// This routine is used by the multiplayer sync check to prove that every machine is
/// playing with the same rules.
/// </summary>
// (0x0069F340)

void SuperWeaponTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)ArrayIndex);
    crc((long)Action);
    crc((long)unknown_0xA0);
    crc((long)unknown_0xA4);
    crc((long)unknown_0xA8);
    crc((long)unknown_0xAC);
    crc(IsPowered);
    crc(DisableableFromShell);
    crc(PreClick);
    crc(PostClick);
    crc((long)PreDependent);
    crc(ShowTimer);
    crc((long)RechargeTime);
    crc((long)Type);
    crc(UseChargeDrain);
    crc(ManualControl);
}


/// <summary>
/// Fetches the heap identifier of this super weapon type.
/// </summary>
/// <returns>Returns with the index of this object within the super weapon type heap.</returns>
// (0x0069F440)

int SuperWeaponTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


/// <summary>
/// Fetches this super weapon's characteristics from the rules.
/// This routine picks up the weapon, the announcement voices, the recharge delay, and
/// the sidebar cameo artwork. A missing cameo falls back on the generic icon so that the
/// sidebar always has something to draw.
/// </summary>
/// <returns>bool; Was the database entry found and read?</returns>
// (0x0069F450)

bool SuperWeaponTypeClass::Read_INI(CCINIClass &ini)
{
    if (AbstractTypeClass::Read_INI(ini)) {
    char buffer[128];
    WeaponTypeClass *weapon = WeaponType;
    WeaponType = ini.Get_String(ID, "WeaponType", "", buffer, sizeof(buffer)) ? WeaponTypeClass::Find_Or_Make(buffer) : weapon;
    Action = ini.Get_ActionType(ID, "Action", Action);
    IsPowered = ini.Get_Bool(ID, "IsPowered", IsPowered);
    DisableableFromShell = ini.Get_Bool(ID, "DisableableFromShell", DisableableFromShell);
    PreClick = ini.Get_Bool(ID, "PreClick", PreClick);
    PostClick = ini.Get_Bool(ID, "PostClick", PostClick);
    ShowTimer = ini.Get_Bool(ID, "ShowTimer", ShowTimer);

    char name[40];
    ini.Get_String(ID, "Type", "", name, sizeof(name));
    if (strlen(name) != 0) {
        int type = Type_From_Name(name);
        if (type != -1) {
            Type = type;
        }
    }
    char dependent[40];
    ini.Get_String(ID, "PreDependent", "", dependent, sizeof(dependent));
    if (strlen(dependent) != 0) {
        int type = Type_From_Name(dependent);
        if (type != -1) {
            PreDependent = type;
        }
    }

    BuildingTypeClass *building = AuxBuilding;
    AuxBuilding = ini.Get_String(ID, "AuxBuilding", "", buffer, sizeof(buffer)) ? BuildingTypeClass_Find_Or_Make(buffer) : building;
    UseChargeDrain = ini.Get_Bool(ID, "UseChargeDrain", UseChargeDrain);
    ManualControl = ini.Get_Bool(ID, "ManualControl", ManualControl);
    double minutes = ini.Get_Double(ID, "RechargeTime", 0.0);
    if (minutes != 0.0) {
        RechargeTime = (float)minutes * 900.0f;
    }

    char image[256];
    ini.Get_String(ID, "SidebarImage", SidebarImage, image, sizeof(image));
    SidebarImage = image;
    char fullname[512];
    _makepath(fullname, 0, 0, SidebarImage, ".SHP");
    SidebarIcon = MixFileClass::Retrieve(fullname);
    if (SidebarIcon == 0) {
        SidebarIcon = MixFileClass::Retrieve("XXICON.SHP");
    }
    return true;
    }
    return false;
}


// int __fastcall SuperWeaponTypeClass::Find_Index(char const *name) (0x0069F790)

int __fastcall SuperWeaponTypeClass::Find_Index(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < SuperWeaponTypes.Count(); index++) {
            if (_stricmp(SuperWeaponTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


/// <summary>
/// Fetches the super weapon type that performs the action specified.
/// This routine is used to work back from a mouse action to the super weapon that
/// offers it.
/// </summary>
/// <returns>Returns with a pointer to the matching super weapon type. Otherwise, NULL is
/// returned.</returns>
// (0x0069F7E0)

SuperWeaponTypeClass *__fastcall SuperWeaponTypeClass::From_Action(int action)
{
    for (int index = 0; index < SuperWeaponTypes.Count(); index++) {
        if (SuperWeaponTypes[index]->Action == action) {
            return SuperWeaponTypes[index];
        }
    }
    return 0;
}


/// <summary>
/// Fetches the super weapon type of the name specified, creating it if need be.
/// This routine is used while the rules are being parsed so that a super weapon may be
/// referred to before its own section has been reached.
/// </summary>
/// <returns>Returns with a pointer to the super weapon type object.</returns>
// (0x0069F820)

SuperWeaponTypeClass *__fastcall SuperWeaponTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < SuperWeaponTypes.Count(); index++) {
        if (_stricmp(SuperWeaponTypes[index]->ID, name) == 0) {
            return SuperWeaponTypes[index];
        }
    }
    return new SuperWeaponTypeClass(name);
}


// int SuperWeaponTypeClass::unknown_slot27(void const *, void const *) const (0x0069F8B0)

int SuperWeaponTypeClass::unknown_slot27(void const *, void const *) const
{
    return Action;
}


// SuperWeaponTypeClass scalar deleting destructor (0x0069F8C0): compiler
// generated after the unit's last member.
