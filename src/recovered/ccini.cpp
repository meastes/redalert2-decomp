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
// Function comment blocks from mischa85/RedAlert2 392e904 src/ccini.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/ccini.cpp where both define the function (names only).

// ccini.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ccini.json carries the addresses and the rows that
// stay in their own files.


#define RA2_CCINI_UNIT_MEMBERS
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/ccini_class.hpp"
#include "shared/ccini_extra.hpp"
#include "shared/crt_memory.hpp"
#include "shared/sha_pipe.hpp"


// CCINI.CPP declares CCINIClass::Get_Buildings 0x0046AEA0 and Put_House_List 0x0046BE60 (shared/ccini_class.hpp).
// Header-static dynamic initializers of the unit at 0x004694C0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004694C0-0x004696A6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004694F0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469510, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469530, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469550, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469570, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469590, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x004695D0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469600, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469630, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469660, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469670, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469690, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


/***********************************************************************************************
 * CCINIClass::Load -- Load the INI database from the file specified.                          *
 *                                                                                             *
 *    This routine will load the database from the file specified in much the same manner      *
 *    that the INIClass load function works. However, this class will examine the message      *
 *    digest (if present) and compare it to the actual digest. If they differ, a special       *
 *    return value is used. This will allow verification of the integrity of the ini data.     *
 *                                                                                             *
 * INPUT:   file  -- Reference to the file that will be read from.                             *
 *                                                                                             *
 *          withdigest  -- Should a message digest be examined when loaded. If there is a      *
 *                         mismatch detected, then an error will be returned.                  *
 *                                                                                             *
 * OUTPUT:  If the file was not read, returns 0. If the file was read ok, returns 1. If the    *
 *          file was read ok, but the digest doesn't verify, returns 2.                        *
 *                                                                                             *
 * WARNINGS:   If no message digest was present in the INI file, then no verification can      *
 *             be performed.                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Handles digest control.                                                  *
 *=============================================================================================*/
// (0x004696B0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Load(FileClass &file, bool withdigest, bool keep_comments)
{
    FileStraw straw(file);
    return Load(straw, withdigest, keep_comments);
}


/***********************************************************************************************
 * CCINIClass::Load -- Load the INI database from the data stream specified.                   *
 *                                                                                             *
 *    This will load the INI database and in the process, it will fetch and verify any         *
 *    message digest present.                                                                  *
 *                                                                                             *
 * INPUT:   straw -- The data stream to fetch the INI data from.                               *
 *                                                                                             *
 *          withdigest  -- Should a message digest be examined when loaded. If there is a      *
 *                         mismatch detected, then an error will be returned.                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the database loaded ok? (hack: returns "2" if digest doesn't match).     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Handles message digest control.                                          *
 *=============================================================================================*/
// (0x004697E0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Load(Straw &file, bool withdigest, bool keep_comments)
{
    int result = INIClass::Load(file, keep_comments);
    Invalidate_Message_Digest();
    if (result && withdigest) {
        unsigned char digest[20];
        int length = Get_UUBlock("Digest", digest, sizeof(digest));
        if (length == 0) {
            return 2;
        }
        if (length > 0) {
            Clear("Digest");
            Calculate_Message_Digest();
            if (memcmp(digest, Digest, sizeof(digest)) != 0) {
                return 2;
            }
        }
    }
    return result;
}


/***********************************************************************************************
 * CCINIClass::Save -- Save the INI data to the file specified.                                *
 *                                                                                             *
 *    This routine will save the INI data to the file. It will add a message digest so that    *
 *    validity check can be performed when the INI data is subsequently read.                  *
 *                                                                                             *
 * INPUT:   file  -- Reference to the file to write the INI data to.                           *
 *                                                                                             *
 *          withdigest  -- Should a message digest be generated and saved with the INI         *
 *                         data file?                                                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the INI data saved?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Handles message digest control.                                          *
 *=============================================================================================*/
// (0x004698F0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Save(FileClass &file, bool withdigest)
{
    FilePipe pipe(file);
    return Save(pipe, withdigest);
}


/***********************************************************************************************
 * CCINIClass::Save -- Pipes the INI database to the pipe specified.                           *
 *                                                                                             *
 *    This routine will pipe the INI data to the pipe segment specified. It is functionally    *
 *    the same as the save operation. A message digest is added to the output data so that     *
 *    validity check can occur during a subsequent read.                                       *
 *                                                                                             *
 * INPUT:   straw -- Reference to the pipe that will receive the output ini data stream.       *
 *                                                                                             *
 *          withdigest  -- Should a message digest be generated and saved with the INI         *
 *                         data file?                                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes output to the pipe.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *   08/21/1996 JLB : Handles message digest control.                                          *
 *=============================================================================================*/
// (0x00469A00)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Save(Pipe &pipe, bool withdigest)
{
    if (!withdigest) {
        return INIClass::Save(pipe);
    }
    Clear("Digest");
    Calculate_Message_Digest();
    Put_UUBlock("Digest", Digest, sizeof(Digest));
    int result = INIClass::Save(pipe);
    Clear("Digest");
    return result;
}


/***********************************************************************************************
 * CCINIClass::Get_Lepton -- Fetches a lepton value from the INI database.                     *
 *                                                                                             *
 *    This routine will fetch the lepton value as if it were expressed as cells. Example;      *
 *    a value of 1 would mean 256 in leptons.                                                  *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to look under.                                  *
 *                                                                                             *
 *          entry    -- The entry identifier to find.                                          *
 *                                                                                             *
 *          defvalue -- The default value to use if the specified section and entry could      *
 *                      not be located.                                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the lepton value of the section & entry specified. If not found, then *
 *          the default value is returned.                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00469AE0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Lepton(char const *section, char const *entry, int defvalue) const
{
    double value = Get_Double(section, entry, -1.0);
    if (value == -1.0) {
        return defvalue;
    }
    return value * 256.0;
}


/***********************************************************************************************
 * CCINIClass::Put_Lepton -- Stores a lepton value to the INI database.                        *
 *                                                                                             *
 *    This routine will store the lepton value as if it were expressed in cells. Example;      *
 *    A lepton of 128 will be stored as ".5".                                                  *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the value under.                       *
 *                                                                                             *
 *          entry    -- The entry to store the lepton value at.                                *
 *                                                                                             *
 *          value    -- The lepton value to store.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the lepton value stored?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00469B20)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Lepton(char const *section, char const *entry, int value)
{
    return Put_Double(section, entry, value / 256.0);
}


// CCINIClass::Put_Facing (0x00469B50).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Facing(char const *section, char const *entry, int facing)
{
    char buffer[32];
    sprintf(buffer, "%d", (facing * 360) / 256);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Facing (0x00469BA0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Facing(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return (atoi(buffer) * 256) / 360;
    }
    return defvalue;
}


/// <summary>
/// Fetches a cell location from the INI database.
/// The cell is expressed as an "x,y" pair.
/// </summary>
/// <returns>Returns with the cell found, or the default cell if the entry is absent.</returns>
// (0x00469C00)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

Cell CCINIClass::Get_Cell(char const *section, char const *entry, Cell const &defvalue) const
{
    char buffer[64];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        int x;
        int y;
        sscanf(buffer, "%d,%d", &x, &y);
        return Cell(x, y);
    }
    return defvalue;
}


/// <summary>
/// Stores a cell location to the INI database.
/// The cell is recorded as an "x,y" pair.
/// </summary>
/// <returns>bool; Was the cell stored?</returns>
// (0x00469C80)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Cell(char const *section, char const *entry, Cell const &value)
{
    char buffer[64];
    sprintf(buffer, "%d,%d", value.X, value.Y);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Percent (0x00469CD0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Percent(char const *section, char const *entry, int defvalue) const
{
    int value = Get_Int(section, entry, -1);
    if (value == -1) {
        return defvalue;
    }
    if (value >= 100) {
        value = 100;
    }
    int result = ((value > 0 ? value : 0) * 256) / 100;
    if (result >= 255) {
        result = 255;
    }
    return result;
}


// CCINIClass::Put_Percent (0x00469D30).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Percent(char const *section, char const *entry, int value)
{
    return Put_Int(section, entry, (value * 100) / 256);
}


// CCINIClass::Get_PipType (0x00469D60).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_PipType(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, PipNames[defvalue].Name, buffer, sizeof(buffer));
    for (int index = 0; index < 5; index++) {
        if (_stricmp(buffer, PipNames[index].Name) == 0) {
            return PipNames[index].Value;
        }
    }
    return 1;
}


// CCINIClass::Put_PipType (0x00469DD0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_PipType(char const *section, char const *entry, int pip)
{
    if (pip != 0) {
        return Put_String(section, entry, PipNames[pip].Name);
    }
    return false;
}


// CCINIClass::Get_PipScale (0x00469E00).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_PipScale(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    if (Get_String(section, entry, "", buffer, sizeof(buffer)) > 0) {
        for (int index = 0; index < 4; index++) {
            if (_stricmp(buffer, PipScaleNames[index].Name) == 0) {
                return PipScaleNames[index].Value;
            }
        }
        return 0;
    }
    return defvalue;
}


// CCINIClass::Put_PipScale (0x00469E70): writes a pip scale by name; none (0) is not written
// (TibSun Put_PipScaleType).

bool CCINIClass::Put_PipScale(char const *section, char const *entry, int scale)
{
    if (scale != 0) {
        return Put_String(section, entry, PipScaleNames[scale].Name);
    }
    return false;
}


/// <summary>
/// Fetches an object category from the INI database.
/// The category is what the AI production logic uses to tell a soldier from a transport or
/// a power plant.
/// </summary>
/// <returns>Returns with the category that matches the name recorded, or CATEGORY_NONE if
/// the name is not recognized.</returns>
// (0x00469EA0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

CategoryType CCINIClass::Get_CategoryType(char const *section, char const *entry, CategoryType defvalue) const
{
    char buffer[32];
    Get_String(section, entry, Name_From_Category(defvalue), buffer, sizeof(buffer));
    return Category_From_Name(buffer);
}


/// <summary>
/// Stores an object category to the INI database.
/// </summary>
/// <returns>bool; Was the category stored? A value of CATEGORY_SOLDIER is never
/// stored.</returns>
// (0x00469EE0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_CategoryType(char const *section, char const *entry, CategoryType value)
{
    if (value != CATEGORY_SOLDIER) {
        return Put_String(section, entry, Name_From_Category(value));
    }
    return false;
}


// CCINIClass::Get_UnknownHex_469f10 (0x00469F10): the entry read with Get_Hex
// as an UnknownHexValue. Unreferenced in the original.

UnknownHexValue CCINIClass::Get_UnknownHex_469f10(char const *section, char const *entry, int defvalue) const
{
    return UnknownHexValue(Get_Hex(section, entry, defvalue));
}


// CCINIClass::Put_UnknownHex_469f30 (0x00469F30): writes the value with
// Put_Hex. Unreferenced in the original.

bool CCINIClass::Put_UnknownHex_469f30(char const *section, char const *entry, int value)
{
    return Put_Hex(section, entry, value);
}


// CCINIClass::Get_ColorScheme (0x00469F50).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_ColorScheme(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, ColorSchemes[defvalue]->Name, buffer, sizeof(buffer));
    for (int index = 0; index < ColorSchemes.Count(); index++) {
        if (_stricmp(ColorSchemes[index]->Name, buffer) == 0 && ColorSchemes[index]->unknown_0x310 != 1) {
            return index;
        }
    }
    return defvalue;
}


// CCINIClass::Put_ColorScheme (0x00469FE0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_ColorScheme(char const *section, char const *entry, int scheme)
{
    return Put_String(section, entry, ColorSchemes[scheme]->Name);
}


// CCINIClass::Get_RGB (0x0046A010).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

RGBClass CCINIClass::Get_RGB(char const *section, char const *entry, RGBClass const &defvalue) const
{
    char defbuffer[64];
    char buffer[64];
    sprintf(defbuffer, "%d,%d,%d", defvalue.Red, defvalue.Green, defvalue.Blue);
    if (Get_String(section, entry, defbuffer, buffer, sizeof(buffer))) {
        int red;
        int green;
        int blue;
        sscanf(buffer, "%d,%d,%d", &red, &green, &blue);
        return RGBClass(red, green, blue);
    }
    return defvalue;
}


// CCINIClass::Put_RGB (0x0046A0E0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_RGB(char const *section, char const *entry, RGBClass const &rgb)
{
    char buffer[64];
    sprintf(buffer, "%d,%d,%d", rgb.Red, rgb.Green, rgb.Blue);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_HSV (0x0046A130).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

HSVClass CCINIClass::Get_HSV(char const *section, char const *entry, HSVClass const &defvalue) const
{
    char defbuffer[64];
    char buffer[64];
    sprintf(defbuffer, "%d,%d,%d", defvalue.Hue, defvalue.Saturation, defvalue.Value);
    if (Get_String(section, entry, defbuffer, buffer, sizeof(buffer))) {
        int hue = 0;
        int saturation = 0;
        int value = 0;
        sscanf(buffer, "%d,%d,%d", &hue, &saturation, &value);
        return HSVClass(hue, saturation, value);
    }
    return defvalue;
}


// CCINIClass::Put_HSV (0x0046A210).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_HSV(char const *section, char const *entry, HSVClass const &hsv)
{
    char buffer[64];
    sprintf(buffer, "%d,%d,%d", hsv.Hue, hsv.Saturation, hsv.Value);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Foundation (0x0046A260).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Foundation(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, FoundationNames[defvalue].Name, buffer, sizeof(buffer));
    for (int index = 0; index < 22; index++) {
        if (_stricmp(buffer, FoundationNames[index].Name) == 0) {
            return FoundationNames[index].Value;
        }
    }
    return 0;
}


// CCINIClass::Put_Foundation (0x0046A2D0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Foundation(char const *section, char const *entry, int foundation)
{
    if (foundation != -1) {
        return Put_String(section, entry, FoundationNames[foundation].Name);
    }
    return false;
}


// CCINIClass::Get_MovementZone (0x0046A300).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_MovementZone(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, MovementZoneNames[defvalue], buffer, sizeof(buffer));
    for (int index = 0; index < 12; index++) {
        if (_stricmp(buffer, MovementZoneNames[index]) == 0) {
            return index;
        }
    }
    return -1;
}


// CCINIClass::Put_MovementZone (0x0046A370).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_MovementZone(char const *section, char const *entry, int zone)
{
    if (zone != -1) {
        return Put_String(section, entry, MovementZoneNames[zone]);
    }
    return false;
}


/// <summary>
/// Fetches a trigger action identifier from the INI database.
/// </summary>
/// <returns>Returns with the action that matches the name recorded, or ACTION_NONE if the
/// name is not recognized.</returns>
// (0x0046A3A0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_ActionType(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, ActionNames[defvalue], buffer, sizeof(buffer));
    for (int index = 0; index < 66; index++) {
        if (_stricmp(buffer, ActionNames[index]) == 0) {
            return index;
        }
    }
    return 0;
}


/// <summary>
/// Fetches a super weapon identifier from the INI database.
/// </summary>
/// <returns>Returns with the super weapon that matches the name recorded, or SUPER_NONE if
/// the name is not recognized.</returns>
// (0x0046A410)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_SuperWeaponType(char const *section, char const *entry, int defvalue) const
{
    char empty[28];
    char buffer[32];
    char const *defname;
    if (defvalue != -1) {
        defname = SuperWeaponTypes[defvalue]->ID;
    } else {
        empty[0] = '\0';
        defname = empty;
    }
    Get_String(section, entry, defname, buffer, sizeof(buffer));
    return SuperWeaponTypeClass::Find_Index(buffer);
}


/// <summary>
/// Fetches a speech identifier from the INI database.
/// </summary>
/// <returns>Returns with the speech identifier that matches the name recorded, or VOX_NONE
/// if the name is not recognized.</returns>
// (0x0046A460)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_VoxType(char const *section, char const *entry, int defvalue) const
{
    char buffer[100];
    Get_String(section, entry, "", buffer, sizeof(buffer));
    if (strlen(buffer) != 0) {
        return VoxClass::Find_Index(buffer);
    }
    return defvalue;
}


/// <summary>
/// Fetches an object category identifier from the INI database.
/// This routine is used where a rules entry names the kind of object it refers to, rather
/// than an individual object type.
/// </summary>
/// <returns>Returns with the object category that matches the name recorded.</returns>
// (0x0046A4B0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

RTTIType CCINIClass::Get_RTTIType(char const *section, char const *entry, RTTIType defvalue) const
{
    char buffer[32];
    Get_String(section, entry, Name_From_RTTI(defvalue), buffer, sizeof(buffer));
    return RTTI_From_Name(buffer);
}


/// <summary>
/// Stores an object category identifier to the INI database.
/// </summary>
/// <returns>bool; Was the object category stored?</returns>
// (0x0046A4F0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_RTTIType(char const *section, char const *entry, RTTIType value)
{
    return Put_String(section, entry, Name_From_RTTI(value));
}


/// <summary>
/// RA2: fetches a build category from the INI database.
/// </summary>
// (0x0046A520)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

BuildCat CCINIClass::Get_BuildCat(char const *section, char const *entry, BuildCat defvalue) const
{
    char buffer[32];
    Get_String(section, entry, Name_From_BuildCat(defvalue), buffer, sizeof(buffer));
    return BuildCat_From_Name(buffer);
}


/// <summary>
/// RA2: stores a build category to the INI database.
/// </summary>
// (0x0046A560)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_BuildCat(char const *section, char const *entry, BuildCat value)
{
    return Put_String(section, entry, Name_From_BuildCat(value));
}


/***********************************************************************************************
 * CCINIClass::Get_Owners -- Fetch the owners (list of house bits).                            *
 *                                                                                             *
 *    Use this to fetch a house bit array value from the INI database. This value will be      *
 *    various bit positions set (1 << house#) for each house specified in the database.        *
 *    Houses can be specified in series by the house name separated by commas or by the        *
 *    special group names of "soviet", and "allies" to cover the houses that are members of    *
 *    these groups.                                                                            *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to search for the entry under.                  *
 *                                                                                             *
 *          entry    -- The entry identifier to search for.                                    *
 *                                                                                             *
 *          defvalue -- The default house bitfield to use if the entry could not be found.     *
 *                                                                                             *
 * OUTPUT:  Returns with the house bitfield value. If the entry could not be found, then the   *
 *          default value is returned.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046A590)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Owners(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    int bits = defvalue;
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        bits = 0;
        for (char const *name = strtok(buffer, ","); name != 0; name = strtok(0, ",")) {
            bits |= Owner_From_Name(name);
        }
    }
    return bits;
}


/***********************************************************************************************
 * CCINIClass::Put_Owners -- Store the house bitfield to the INI database.                     *
 *                                                                                             *
 *    Use this routine to store the house bitfield data into the database. The bitfield format *
 *    matches the format used by the Get_Owners function. Example; if both England and         *
 *    Spain were specified in the bitfield, the entry would be stored as "England,Spain".      *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry identifier that is assigned the value.                       *
 *                                                                                             *
 *          value    -- The value to assign to the entry.                                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the entry stored in the INI database?                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046A610)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Owners(char const *section, char const *entry, int value)
{
    char buffer[128];
    buffer[0] = '\0';
    if (value != 0) {
        for (int index = 0; index < HouseTypes.Count(); index++) {
            HouseTypeClass *house = HouseTypes[index];
            if ((value & (1 << house->House)) != 0) {
                if (buffer[0] != '\0') {
                    strcat(buffer, ",");
                }
                strcat(buffer, house->ID);
            }
        }
        if (buffer[0] != '\0') {
            return Put_String(section, entry, buffer);
        }
    }
    return true;
}


/// <summary>
/// RA2: fetches a set of houses, by their INI names, as a bit field of house indexes.
/// </summary>
// (0x0046A720)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Houses(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    int bits = defvalue;
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        bits = 0;
        for (char const *name = strtok(buffer, ","); name != 0; name = strtok(0, ",")) {
            bits |= 1 << HouseClass::Find_Index(name);
        }
    }
    return bits;
}


/// <summary>
/// RA2: stores a bit field of house indexes as their INI names.
/// </summary>
// (0x0046A7A0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Houses(char const *section, char const *entry, int value)
{
    char buffer[128];
    buffer[0] = '\0';
    if (value != 0) {
        for (int index = 0; index < Houses.Count(); index++) {
            HouseClass *house = Houses[index];
            if ((value & (1 << house->ArrayIndex)) != 0) {
                if (buffer[0] != '\0') {
                    strcat(buffer, ",");
                }
                strcat(buffer, house->IniName);
            }
        }
        if (buffer[0] != '\0') {
            return Put_String(section, entry, buffer);
        }
    }
    return true;
}


/***********************************************************************************************
 * CCINIClass::Get_ArmorType -- Fetches the armor type from the INI database.                  *
 *                                                                                             *
 *    This routine will fetch the armor type from the database.                                *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- Th identifier for the entry to search for.                             *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the armor type specified in the INI database. If it could not be      *
 *          found, then the default value is returned instead.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046A8B0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

ArmorType CCINIClass::Get_ArmorType(char const *section, char const *entry, ArmorType defvalue) const
{
    char buffer[128];
    Get_String(section, entry, ArmorNames[defvalue], buffer, sizeof(buffer));
    return Armor_From_Name(buffer);
}


/***********************************************************************************************
 * CCINIClass::Put_ArmorType -- Store the armor type to the INI database.                      *
 *                                                                                             *
 *    Use this routine to store the specified armor type to the INI database.                  *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry to store the value at.                                       *
 *                                                                                             *
 *          value    -- The value to store in the database.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the entry stored in the database?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046A900)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_ArmorType(char const *section, char const *entry, ArmorType value)
{
    return Put_String(section, entry, ArmorNames[value]);
}


/***********************************************************************************************
 * CCINIClass::Put_VocType -- Store a sound effect identifier into the INI database.           *
 *                                                                                             *
 *    Use this routine to store a voc identifier (stored a the text name of the sound) into    *
 *    the INI database.                                                                        *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to store the entry under.                   *
 *                                                                                             *
 *          entry    -- The entry to assign the value to.                                      *
 *                                                                                             *
 *          value    -- The sound effect to store to the entry.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the sound effect entry stored?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046A920)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_VocType(char const *section, char const *entry, int value)
{
    if ((char)value == -1) {
        return Put_String(section, entry, "<none>");
    }
    return Put_String(section, entry, Voc_Name(value));
}


/// <summary>
/// Fetches a land type from the INI database.
/// </summary>
/// <returns>Returns with the land type named by the entry. If the entry could not be found,
/// then the default value is returned.</returns>
// (0x0046A970)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

LandType CCINIClass::Get_LandType(char const *section, char const *entry, LandType defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, Name_From_Land(defvalue), buffer, sizeof(buffer))) {
        return Land_From_Name(buffer);
    }
    return defvalue;
}


/// <summary>
/// Stores a land type to the INI database.
/// </summary>
/// <returns>bool; Was the land type stored?</returns>
// (0x0046A9D0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_LandType(char const *section, char const *entry, LandType value)
{
    return Put_String(section, entry, Name_From_Land(value));
}


/***********************************************************************************************
 * CCINIClass::Get_HousesType -- Fetch a house identifier from the INI database.               *
 *                                                                                             *
 *    Use this routine to fetch an individual house identifier from the INI database. This is  *
 *    somewhat similar to the Get_Owners function but is limited to a single house.            *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- Identifier for the entry to search for.                                *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the house identifier if it was found. If not found, then the default  *
 *          value is returned.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AA00)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_HousesType(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        int index = HouseTypeClass::Find_Index(buffer);
        if (index == -1) {
            // An unknown name creates the house type.
            return HouseTypes.ID(new HouseTypeClass(buffer));
        }
        return index;
    }
    return defvalue;
}


/***********************************************************************************************
 * CCINIClass::Put_HousesType -- Store a house identifier to the INI database.                 *
 *                                                                                             *
 *    Use this routine to store the specified house identifier to the INI database.            *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- Identifier for the entry to search for.                                *
 *                                                                                             *
 *          value    -- The house identifier to store in the database.                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the house identifier stored?                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AAB0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_HousesType(char const *section, char const *entry, int value)
{
    char const *name = "<none>";
    if (value == -2) {
        name = "<random>";
    } else if (value == -3) {
        name = "<random>";
    } else if (value != -1 && value >= 0 && value < HouseTypes.Count()) {
        name = HouseTypes[value]->ID;
    }
    return Put_String(section, entry, name);
}


/// <summary>
/// Fetches a side identifier from the INI database.
/// This routine is used while the rules are being read, where a side may well be named
/// before it has been declared.
/// </summary>
/// <returns>Returns with the side identifier that matches the name recorded. If the entry
/// could not be found, then the default value is returned.</returns>
/// <remarks>A name that is not already known will add a new side to the side list.</remarks>
// (0x0046AB30)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Side(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        int index = SideClass::Find_Index(buffer);
        if (index == -1) {
            // An unknown name creates the side.
            return Sides.ID(new SideClass(buffer));
        }
        return index;
    }
    return defvalue;
}


/// <summary>
/// Stores a side identifier to the INI database.
/// The side is recorded by name. A side identifier of SIDE_NONE is recorded as no side at all.
/// </summary>
/// <returns>bool; Was the side identifier stored?</returns>
// (0x0046ABE0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Side(char const *section, char const *entry, int value)
{
    char const *name = "<none>";
    if (value != -1) {
        name = Sides[value]->ID;
    }
    return Put_String(section, entry, name);
}


// CCINIClass::Get_Listed_Name (0x0046AC10).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Listed_Name(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        int index = VQ_From_Name(buffer);
        if (index != -1) {
            return index;
        }
    }
    return defvalue;
}


// CCINIClass::Put_Listed_Name (0x0046AC60).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Listed_Name(char const *section, char const *entry, unsigned index)
{
    if (index >= (unsigned)Movies.Count()) {
        return Put_String(section, entry, "<none>");
    }
    return Put_String(section, entry, Movies[index]);
}


/***********************************************************************************************
 * CCINIClass::Get_TheaterType -- Fetch the theater type from the INI database.                *
 *                                                                                             *
 *    This will fetch the theater identifier from the INI database.                            *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search for the entry under.              *
 *                                                                                             *
 *          entry    -- Identifier for the entry to search for.                                *
 *                                                                                             *
 *          defvalue -- The default value to use if the entry could not be located.            *
 *                                                                                             *
 * OUTPUT:  Returns with the theater type found. If the entry could not be found, then the     *
 *          default value is returned.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046ACB0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

TheaterType CCINIClass::Get_TheaterType(char const *section, char const *entry, TheaterType defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Theater_From_Name(buffer);
    }
    return defvalue;
}


/***********************************************************************************************
 * CCINIClass::Put_TheaterType -- Store the theater identifier to the INI database.            *
 *                                                                                             *
 *    Use this routine to store the theater name to the INI database.                          *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- Identifier for the entry to store.                                     *
 *                                                                                             *
 *          value    -- The theater identifier to store.                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the theater identifier stored?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AD00)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_TheaterType(char const *section, char const *entry, TheaterType value)
{
    return Put_String(section, entry, Theaters[value].Name);
}


/***********************************************************************************************
 * CCINIClass::Get_ThemeType -- Fetch the theme identifier.                                    *
 *                                                                                             *
 *    This routine will fetch the theme identifier from the INI database.                      *
 *                                                                                             *
 * INPUT:   section  -- The section to search for the entry under.                             *
 *                                                                                             *
 *          entry    -- Identifier of the entry to search for.                                 *
 *                                                                                             *
 *          defvalue -- The default theme identifier to return if the entry could not be found.*
 *                                                                                             *
 * OUTPUT:  Returns with the theme identifier if it was found. If not found, then the default  *
 *          value is returned instead.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AD30)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_ThemeType(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Theme.From_Name(buffer);
    }
    return defvalue;
}


/***********************************************************************************************
 * CCINIClass::Put_ThemeType -- Store the theme identifier to the INI database.                *
 *                                                                                             *
 *    This routine will store the specified theme identifier to the INI database.              *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to store the entry under.                   *
 *                                                                                             *
 *          entry    -- Identifier for the entry to store the value to.                        *
 *                                                                                             *
 *          value    -- The theme identifier to store.                                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the theme identifier stored.                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AD90)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_ThemeType(char const *section, char const *entry, int value)
{
    return Put_String(section, entry, Theme.Base_Name(value));
}


/***********************************************************************************************
 * CCINIClass::Get_SourceType -- Fetch the source (edge) type from the INI database.           *
 *                                                                                             *
 *    This routine will fetch the source (reinforcement edge) identifier from the INI          *
 *    database.                                                                                *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section that the entry will be searched under.      *
 *                                                                                             *
 *          entry    -- Identifier for the entry that will be searched for.                    *
 *                                                                                             *
 *          defvalue -- The default value to return if the entry could not be located.         *
 *                                                                                             *
 * OUTPUT:  Returns with the source type of the entry if found. If not found, then the         *
 *          default value is returned.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046ADC0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

SourceType CCINIClass::Get_SourceType(char const *section, char const *entry, SourceType defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Source_From_Name(buffer);
    }
    return defvalue;
}


/***********************************************************************************************
 * CCINIClass::Put_SourceType -- Store the source (edge) identifier to the INI database.       *
 *                                                                                             *
 *    This will store the source type (reinforcement edge) to the INI database.                *
 *                                                                                             *
 * INPUT:   section  -- The section to store the entry under.                                  *
 *                                                                                             *
 *          entry    -- Identifier of the entry to store the source identifier to.             *
 *                                                                                             *
 *          value    -- The source (edge) value to store.                                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the source identifier stored?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AE10)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_SourceType(char const *section, char const *entry, SourceType value)
{
    return Put_String(section, entry, SourceNames[value]);
}


/***********************************************************************************************
 * CCINIClass::Get_CrateType -- Fetches a crate type value from the INI database.              *
 *                                                                                             *
 *    This will return with the crate type specified in the INI database.                      *
 *                                                                                             *
 * INPUT:   section  -- Identifier for the section to search under.                            *
 *                                                                                             *
 *          entry    -- The entry to find the matching crate value for.                        *
 *                                                                                             *
 *          defvalue -- The default crate value to return if the entry could not be found.     *
 *                                                                                             *
 * OUTPUT:  Returns with the crate type identified with the specified entry. If the entry      *
 *          could not be located, then the default value is returned.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AE30)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

CrateType CCINIClass::Get_CrateType(char const *section, char const *entry, CrateType defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Crate_From_Name(buffer);
    }
    return defvalue;
}


/***********************************************************************************************
 * CCINIClass::Put_CrateType -- Stores the crate value in the section and entry specified.     *
 *                                                                                             *
 *    This will store the specified crate value to the section and entry specified.            *
 *                                                                                             *
 * INPUT:   section  -- The section identifier to store the entry under.                       *
 *                                                                                             *
 *          entry    -- The entry identifier to store the crate value with.                    *
 *                                                                                             *
 *          value    -- The crate value to store.                                              *
 *                                                                                             *
 * OUTPUT:  bool; Was the crate value stored to the INI database?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AE80)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_CrateType(char const *section, char const *entry, CrateType value)
{
    return Put_String(section, entry, CrateNames[value]);
}


/***********************************************************************************************
 * CCINIClass::Get_Buildings -- Fetch a building bitfield from the INI database.               *
 *                                                                                             *
 *    This routing will fetch the a list of buildings from the INI database. The buildings     *
 *    are expressed as a comma separated list of building identifiers. The return value is     *
 *    a composite of bits that represent these buildings -- one bit per building type.         *
 *                                                                                             *
 * INPUT:   section  -- The section to search for the entry under.                             *
 *                                                                                             *
 *          entry    -- The entry to fetch the building list from.                             *
 *                                                                                             *
 *          defvalue -- The default value to return if the section and entry could not be      *
 *                      located.                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the building list (as a bitfield). If the entry could not be          *
 *          found, the the default value is returned instead.                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AEA0; TibSun ccini.cpp)
// separated list: POWER, FACTORY, BARRACKS and RADAR set the top four bits, a building type ID
// its own bit (BuildingTypeClass::From_Name); defvalue when the entry is missing.

int CCINIClass::Get_Buildings(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    int pre;

    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        pre = 0;
        char *token = strtok(buffer, ",");
        while (token != 0 && *token != '\0') {
            if (!_stricmp(token, "POWER")) {
                pre |= 1L << (32 - 1);
            }
            if (!_stricmp(token, "FACTORY")) {
                pre |= 1L << (32 - 2);
            }
            if (!_stricmp(token, "BARRACKS")) {
                pre |= 1L << (32 - 3);
            }
            if (!_stricmp(token, "RADAR")) {
                pre |= 1L << (32 - 4);
            }
            int building = BuildingTypeClass::From_Name(token);
            if (building != -1) {
                pre |= (1L << building);
            }
            token = strtok(0, ",");
        }
    } else {
        pre = defvalue;
    }

    return pre;
}


/***********************************************************************************************
 * CCINIClass::Put_Buildings -- Store a building list to the INI database.                     *
 *                                                                                             *
 *    This will store a list of buildings to the INI database. The buildings are listed by     *
 *    their identifier names separated by commas.                                              *
 *                                                                                             *
 * INPUT:   section  -- The identifier for the section to store the entry under.               *
 *                                                                                             *
 *          entry    -- The entry to store the building list to.                               *
 *                                                                                             *
 *          value    -- A list of buildings (in the form of a bit field -- one bit per         *
 *                      building type).                                                        *
 *                                                                                             *
 * OUTPUT:  Was the building list stored to the INI file?                                      *
 *                                                                                             *
 * WARNINGS:   This is limited to the buildings that can be expressed in a bitfield long.      *
 *             Which means, there can be only a maximum of 32 building types listed and        *
 *             even then, the total line length generated must not exceed 128 bytes.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046AFB0)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Buildings(char const *section, char const *entry, int value)
{
    char buffer[128] = "";
    int count = BuildingTypes.Count() > 32 ? 32 : BuildingTypes.Count();
    for (int index = 0; index < count; index++) {
        if ((value & (1 << index)) != 0) {
            if (buffer[0] != '\0') {
                strcat(buffer, ",");
            }
            strcat(buffer, BuildingTypes[index]->ID);
        }
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Put_Voc_List (0x0046B0B0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Voc_List(char const *section, char const *entry, TypeList<int> list)
{
    char buffer[128] = "";
    for (int index = 0; index < list.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        strcat(buffer, Voc_Name(list[index]));
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Integer_List (0x0046B1B0).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<int> CCINIClass::Get_Integer_List(char const *section, char const *entry, TypeList<int> defvalue)
{
    char buffer[512];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            list.Add(atoi(token));
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Put_Integer_List (0x0046B370).
// Layout and behaviour: shared/ccini_class.hpp.
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
extern "C" char *__cdecl strcat(char *destination, char const *source);
#pragma intrinsic(strcat)

bool CCINIClass::Put_Integer_List(char const *section, char const *entry, TypeList<int> const &list)
{
    char buffer[512] = "";
    for (int index = 0; index < list.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        char number[12];
        sprintf(number, "%d", list[index]);
        strcat(buffer, number);
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Anim_Target_List (0x0046B460).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<int> CCINIClass::Get_Anim_Target_List(char const *section, char const *entry, TypeList<int> defvalue)
{
    char buffer[512];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            TargetClass target;
            int index = InfantryTypeClass::From_Name(token);
            if (index != -1) {
                target = TargetClass(InfantryTypes[index]);
            } else {
                index = UnitTypeClass::From_Name(token);
                if (index == -1) {
                    continue;
                }
                target = TargetClass(UnitTypes[index]);
            }
            if (target.Is_Valid()) {
                list.Add(target.As_Target());
            }
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Put_Anim_Target_List (0x0046B680).
// Layout and behaviour: shared/ccini_class.hpp; TargetClass in
// CCINIClass_Get_Anim_Target_List_0046b460.cpp.

bool CCINIClass::Put_Anim_Target_List(char const *section, char const *entry, TypeList<int> const &list)
{
    char buffer[512] = "";
    for (int index = 0; index < list.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        TargetClass target;
        target = TargetClass(list[index]);
        AbstractTypeClass *type = target.As_TechnoType();
        strcat(buffer, type->ID);
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Float_Triple (0x0046B780).
// Layout and behaviour: shared/ccini_class.hpp.
extern "C" double __cdecl atof(char const *string);

INIVector3 CCINIClass::Get_Float_Triple(char const *section, char const *entry, INIVector3 const &defvalue)
{
    char buffer[512];
    INIVector3 value = defvalue;
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        value.X = atof(strtok(buffer, ","));
        value.Y = atof(strtok(0, ","));
        value.Z = atof(strtok(0, ","));
        return value;
    }
    return defvalue;
}


// CCINIClass::Get_Integer_Triple (0x0046B860).
// Layout and behaviour: shared/ccini_class.hpp.

INIPoint3D CCINIClass::Get_Integer_Triple(char const *section, char const *entry, INIPoint3D const &defvalue)
{
    char buffer[512];
    INIPoint3D value = defvalue;
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        value.X = atoi(strtok(buffer, ","));
        value.Y = atoi(strtok(0, ","));
        value.Z = atoi(strtok(0, ","));
        return value;
    }
    return defvalue;
}


// CCINIClass::Get_Techno_List (0x0046B930).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<TechnoTypeClass *> CCINIClass::Get_Techno_List(char const *section, char const *entry,
                                                        TypeList<TechnoTypeClass *> defvalue)
{
    char buffer[512];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<TechnoTypeClass *> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            for (int index = 0; index < TechnoTypes.Count(); index++) {
                if (_stricmp(token, TechnoTypes[index]->ID) == 0) {
                    list.Add(TechnoTypes[index]);
                    break;
                }
            }
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Put_Techno_List (0x0046BB30).
// Layout and behaviour: shared/ccini_class.hpp; TechnoTypeClass in
// CCINIClass_Get_Techno_List_0046b930.cpp.

bool CCINIClass::Put_Techno_List(char const *section, char const *entry, TypeList<TechnoTypeClass *> const &list)
{
    char buffer[512] = "";
    for (int index = 0; index < list.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        strcat(buffer, list[index]->ID);
    }
    return Put_String(section, entry, buffer);
}


/// <summary>
/// Fetches a house list from the INI database.
/// This routine will read a comma separated list of house names. A side may be named in
/// place of a house, in which case every house belonging to that side is added to the list.
/// </summary>
/// <returns>Returns with the list of house identifiers specified. If the entry could not be
/// found, then the default value is returned.</returns>
// (0x0046BC00)
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<int> CCINIClass::Get_House_List(char const *section, char const *entry, TypeList<int> defvalue)
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            int index = HouseTypeClass::Find_Index(token);
            if (index != -1) {
                list.Add(index);
            } else {
                int side = SideClass::Find_Index(token);
                if (side != -1) {
                    SideClass *sideptr = Sides[side];
                    for (int house = 0; house < sideptr->Houses.Count(); house++) {
                        list.Add(sideptr->Houses[house]);
                    }
                }
            }
        }
        return list;
    }
    return defvalue;
}


/// <summary>
/// Stores a house list to the INI database.
/// The houses are recorded as a comma separated list of house names.
/// </summary>
/// <returns>bool; Was the house list stored?</returns>
// (0x0046BE60; TibSun ccini.cpp)
// comma separated list of their IDs.

bool CCINIClass::Put_House_List(char const *section, char const *entry, TypeList<int> value)
{
    char buffer[128] = "";

    for (int index = 0; index < value.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        strcat(buffer, HouseTypes[value[index]]->ID);
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Color_List (0x0046BF60).
// Layout and behaviour: shared/ccini_class.hpp, shared/rgb_class.hpp.

TypeList<RGBClass> CCINIClass::Get_Color_List(char const *section, char const *entry, TypeList<RGBClass> defvalue)
{
    char buffer[512];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<RGBClass> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            RGBClass color;
            bool valid = true;
            if (token != 0) {
                color.Red = atoi(token + 1);      // after the '('
            } else {
                valid = false;
            }
            token = strtok(0, ",");
            if (token != 0 && *token != '\0') {
                color.Green = atoi(token);
            } else {
                valid = false;
            }
            token = strtok(0, ",");
            if (token != 0 && *token != '\0') {
                token[strlen(token) - 1] = '\0';  // drops the ')'
                color.Blue = atoi(token);
                if (valid) {
                    list.Add(color);
                }
            }
        }
        return list;
    }
    return defvalue;
}


/***********************************************************************************************
 * CCINIClass::Get_Unique_ID -- Fetch a unique identifier number for the INI file.             *
 *                                                                                             *
 *    This is a shorthand version of the message digest. It calculates the ID number from the  *
 *    message digest itself.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a 32 bit unique identifier number for the INI database.               *
 *                                                                                             *
 * WARNINGS:   Since the return value is only 32 bits, it is much less secure than the         *
 *             complete message digest.                                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046C1C0)
// Layout and behaviour: shared/ccini_class.hpp (Calculate_Message_Digest,
// defined later in the unit, is expanded here).

int CCINIClass::Get_Unique_ID() const
{
    if (!IsDigestValid) {
        ((CCINIClass *)this)->Calculate_Message_Digest();
    }
    return CRCEngine()(&Digest[0], sizeof(Digest));
}


/***********************************************************************************************
 * CCINIClass::Calculate_Message_Digest -- Calculate a message digest for the current database *
 *                                                                                             *
 *    This will calculate a new message digest according to the current state of the INI       *
 *    database.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   If the database is changed in any fashion, this message digest will be rendered *
 *             obsolete.                                                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0046C260)
// Layout and behaviour: shared/ccini_class.hpp, shared/sha_pipe.hpp.

void CCINIClass::Calculate_Message_Digest()
{
    SHAPipe sha;
    INIClass::Save(sha);
    sha.Result(Digest);
    IsDigestValid = true;
}


// 0x0046c2e0: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_0046c2e0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0046c2e0 {

void __fastcall FUN_0046c2e0(int object_address)
{
    *(unsigned char *)(object_address + 0x38) = 0;
    return;
}

}  // namespace seed_0046c2e0


// CCINIClass::Get_Object_Type (0x0046C2F0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

ObjectTypeClass *CCINIClass::Get_Object_Type(char const *section, char const *entry)
{
    char buffer[128];
    ObjectTypeClass *type = 0;
    Get_String(section, entry, "<none>", buffer, sizeof(buffer));
    int index = InfantryTypeClass::From_Name(buffer);
    if (index != -1) {
        type = InfantryTypes[index];
    } else {
        index = UnitTypeClass::From_Name(buffer);
        if (index != -1) {
            type = UnitTypes[index];
        } else {
            index = AircraftTypeClass::From_Name(buffer);
            if (index != -1) {
                type = AircraftTypes[index];
            } else {
                index = BuildingTypeClass::From_Name(buffer);
                if (index != -1) {
                    type = BuildingTypes[index];
                }
            }
        }
    }
    return type;
}


// CCINIClass::Put_Target_Type (0x0046C3B0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Target_Type(char const *section, char const *entry, TargetClass const &target)
{
    AbstractTypeClass *type = target.As_TechnoType();
    if (type != 0) {
        return Put_String(section, entry, type->ID);
    }
    return Put_String(section, entry, "<none>");
}


/// <summary>
/// Fetches a speed type from the INI database.
/// </summary>
/// <returns>Returns with the speed type named by the entry. If the entry could not be
/// found, then the default value is returned.</returns>
// (0x0046C400)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

SpeedType CCINIClass::Get_SpeedType(char const *section, char const *entry, SpeedType defvalue)
{
    char buffer[128];
    if (Get_String(section, entry, Name_From_Speed(defvalue), buffer, sizeof(buffer))) {
        return Speed_From_Name(buffer);
    }
    return defvalue;
}


class Call_46c460_0 {public: unsigned int Name_From_Speed_00482f60();};
class Call_46c460_1 {public: unsigned int Put_String_0050e5f0(unsigned int target_section, unsigned int target_entry, unsigned int target_value);};
class Leaf_46c460 { public: unsigned int get(unsigned int section_name, unsigned int entry_name, unsigned int speed); };
unsigned int Leaf_46c460::get(unsigned int section_name, unsigned int entry_name, unsigned int speed) { unsigned int speed_name_address = ((Call_46c460_0*)(speed))->Name_From_Speed_00482f60(); unsigned int write_result = ((Call_46c460_1*)((unsigned int)this))->Put_String_0050e5f0((unsigned int)(section_name), (unsigned int)(entry_name), (unsigned int)(speed_name_address)); return write_result; }


/// <summary>
/// RA2: fetches a layer from the INI database.
/// </summary>
// (0x0046C490)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

LayerType CCINIClass::Get_LayerType(char const *section, char const *entry, LayerType defvalue)
{
    char buffer[128];
    if (Get_String(section, entry, Name_From_Layer(defvalue), buffer, sizeof(buffer))) {
        return Layer_From_Name(buffer);
    }
    return defvalue;
}


class Call_46c4f0_0 {public: unsigned int Name_From_Layer_00482fc0();};
class Call_46c4f0_1 {public: unsigned int Put_String_0050e5f0(unsigned int target_section, unsigned int target_entry, unsigned int target_value);};
class Leaf_46c4f0 { public: unsigned int get(unsigned int section_name, unsigned int entry_name, unsigned int layer); };
unsigned int Leaf_46c4f0::get(unsigned int section_name, unsigned int entry_name, unsigned int layer) { unsigned int layer_name_address = ((Call_46c4f0_0*)(layer))->Name_From_Layer_00482fc0(); unsigned int write_result = ((Call_46c4f0_1*)((unsigned int)this))->Put_String_0050e5f0((unsigned int)(section_name), (unsigned int)(entry_name), (unsigned int)(layer_name_address)); return write_result; }


// Get_Prerequisite_List (0x0046C520).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<int> __fastcall Get_Prerequisite_List(CCINIClass &ini, char const *section, char const *entry,
                                               TypeList<int> defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            bool found = false;
            if (_stricmp(token, "POWER") == 0) {
                list.Add(-1);
                found = true;
            }
            if (_stricmp(token, "FACTORY") == 0) {
                list.Add(-2);
                found = true;
            }
            if (_stricmp(token, "BARRACKS") == 0) {
                list.Add(-3);
                found = true;
            }
            if (_stricmp(token, "RADAR") == 0) {
                list.Add(-4);
                found = true;
            }
            if (_stricmp(token, "TECH") == 0) {
                list.Add(-5);
                found = true;
            }
            if (_stricmp(token, "PROC") == 0) {
                list.Add(-6);
                found = true;
            }
            if (!found) {
                int index = BuildingTypeClass::From_Name(token);
                if (index != -1) {
                    list.Add(index);
                }
            }
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Get_Integers (0x0046C880).
// Layout and behaviour: shared/ccini_class.hpp.

void CCINIClass::Get_Integers(char const *section, char const *entry, int &first, int &second, int &third)
{
    char buffer[512] = "";
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        char *token = strtok(buffer, ",");
        if (token != 0) {
            first = atoi(token);
        }
        token = strtok(0, ",");
        if (token != 0) {
            second = atoi(token);
        }
        token = strtok(0, ",");
        if (token != 0) {
            third = atoi(token);
        }
    }
}


// CCINIClass::Put_Integers (0x0046C950).
// Layout and behaviour: shared/ccini_class.hpp.

bool CCINIClass::Put_Integers(char const *section, char const *entry, int first, int second, int third)
{
    char buffer[512] = "";
    sprintf(buffer, "%d,%d,%d", first, second, third);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Level (0x0046C9D0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

LevelType CCINIClass::Get_Level(char const *section, char const *entry, LevelType defvalue)
{
    char buffer[128];
    Get_String(section, entry, LevelNames[defvalue], buffer, sizeof(buffer));
    char *token = strtok(buffer, ",");
    if (_stricmp(token, "None") == 0) {
        return LEVEL_NONE;
    }
    if (_stricmp(token, "Normal") == 0) {
        return LEVEL_NORMAL;
    }
    if (_stricmp(token, "Strong") == 0) {
        return LEVEL_STRONG;
    }
    return defvalue;
}


/// <summary>
/// Fetches a veterancy ability set from the INI database.
/// This routine will read a comma separated list of ability names, as used by the elite
/// ability lists of the object types. Any name that is not recognized is ignored.
/// </summary>
/// <returns>Returns with the abilities specified. If the entry could not be found, then the
/// default value is returned.</returns>
// (0x0046CA80)
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

AbilitiesStruct CCINIClass::Get_Abilities(char const *section, char const *entry, AbilitiesStruct const &defvalue)
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        AbilitiesStruct abilities;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            int ability = Ability_From_Name(token);
            if (ability != -1) {
                abilities.Ability[ability] = true;
            }
        }
        return abilities;
    }
    return defvalue;
}


// SHAPipe scalar deleting destructor (0x0046CB90): emitted in the CCINI unit
// with SHAPipe's vtable by CCINIClass::Calculate_Message_Digest; no source of
// its own (shared/sha_pipe.hpp).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<int>::TypeList, copy constructor (0x0046CBF0; vtable 0x0079D9F4).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<int>::Clear (0x0046CC80; TypeList<int> vtable 0x0079d9f4 slot 3).
// DynamicVectorClass<int>::ID_pointer (0x0046CCB0; TypeList<int> vtable 0x0079d9f4 slot 5).
// VectorClass<int>::Fetch (0x0046CCD0; VectorClass<int> vtable 0x0079d9d4 slot 6).
// DynamicVectorClass<TechnoTypeClass *>::Clear (0x0046CCE0; TypeList<TechnoTypeClass *> vtable 0x0079da34 slot 3).
// DynamicVectorClass<TechnoTypeClass *>::ID_pointer (0x0046CD10; TypeList<TechnoTypeClass *> vtable 0x0079da34 slot 5).
// VectorClass<TechnoTypeClass *>::Fetch (0x0046CD30; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 6).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<int> scalar deleting destructor (0x0046CDD0; vtable 0x0079D9F4 slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x0079D9D4. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x0050B0F0.


extern "C" unsigned char vtable_46ceb0[]; extern "C" void __cdecl release_buffer_46ceb0(void*); extern "C" void __cdecl release_object_46ceb0(void*);
class Leaf_46ceb0 { public: void* get(unsigned int flags); };
void* Leaf_46ceb0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46ceb0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46ceb0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46ceb0(this); return this; }


extern "C" unsigned char vtable_46cf00[]; extern "C" void __cdecl release_buffer_46cf00(void*); extern "C" void __cdecl release_object_46cf00(void*);
class Leaf_46cf00 { public: void* get(unsigned int flags); };
void* Leaf_46cf00::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46cf00; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46cf00((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46cf00(this); return this; }


extern "C" unsigned char vtable_46cf50[]; extern "C" void __cdecl release_buffer_46cf50(void*); extern "C" void __cdecl release_object_46cf50(void*);
class Leaf_46cf50 { public: void* get(unsigned int flags); };
void* Leaf_46cf50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46cf50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46cf50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46cf50(this); return this; }


// VectorClass<int> copy constructor (0x0046CFA0): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<int>::Add (0x0046D300): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<int>::VectorClass(unsigned size, int const *array) (0x0046D020; stores the VectorClass<int> vtable 0x0079d9d4).
// VectorClass<int>::operator equal (0x0046D070; VectorClass<int> vtable 0x0079d9d4 slot 1).
// VectorClass<int>::Resize (0x0046D0B0; VectorClass<int> vtable 0x0079d9d4 slot 2).
// VectorClass<int>::Clear (0x0046D160; VectorClass<int> vtable 0x0079d9d4 slot 3).
// VectorClass<int>::ID_object (0x0046D190; VectorClass<int> vtable 0x0079d9d4 slot 4).
// VectorClass<int>::ID_pointer (0x0046D1D0; VectorClass<int> vtable 0x0079d9d4 slot 5).
// DynamicVectorClass<int>::Resize (0x0046D250; TypeList<int> vtable 0x0079d9f4 slot 2).
// DynamicVectorClass<int>::ID_object (0x0046D350; TypeList<int> vtable 0x0079d9f4 slot 4).


// VectorClass<TechnoTypeClass *> copy constructor (0x0046D380): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<TechnoTypeClass *>::operator equal (0x0046D400; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 1).
// VectorClass<TechnoTypeClass *>::Resize (0x0046D440; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 2).
// VectorClass<TechnoTypeClass *>::Clear (0x0046D4F0; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 3).
// VectorClass<TechnoTypeClass *>::ID_object (0x0046D520; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 4).
// VectorClass<TechnoTypeClass *>::ID_pointer (0x0046D560; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 5).
// DynamicVectorClass<TechnoTypeClass *>::Resize (0x0046D580; TypeList<TechnoTypeClass *> vtable 0x0079da34 slot 2).
// DynamicVectorClass<TechnoTypeClass *>::ID_object (0x0046D630; TypeList<TechnoTypeClass *> vtable 0x0079da34 slot 4).


// VectorClass<RGBClass> copy constructor (0x0046D660): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// VectorClass<RGBClass> operator equal (0x0046D720): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// VectorClass<RGBClass> Resize (0x0046D780): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<RGBClass>::ID_object (0x0046D8B0; VectorClass<RGBClass> vtable 0x0079da54 slot 4).
// Layout and behaviour: shared/vector_class.hpp; RGBClass::operator== compares the three bytes (shared/rgb_class.hpp).


// DynamicVectorClass<RGBClass> Resize (0x0046D930): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<RGBClass>::ID_object (0x0046DA30; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; RGBClass::operator== compares the three bytes (shared/rgb_class.hpp).


extern "C" unsigned char vtable_46da70[]; extern "C" void __cdecl release_buffer_46da70(void*); extern "C" void __cdecl release_object_46da70(void*);
class Leaf_46da70 { public: void* get(unsigned int flags); };
void* Leaf_46da70::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46da70; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46da70((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46da70(this); return this; }


extern "C" unsigned char vtable_46dac0[]; extern "C" void __cdecl release_buffer_46dac0(void*); extern "C" void __cdecl release_object_46dac0(void*);
class Leaf_46dac0 { public: void* get(unsigned int flags); };
void* Leaf_46dac0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46dac0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46dac0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46dac0(this); return this; }


extern "C" unsigned char vtable_46db10[]; extern "C" void __cdecl release_buffer_46db10(void*); extern "C" void __cdecl release_object_46db10(void*);
class Leaf_46db10 { public: void* get(unsigned int flags); };
void* Leaf_46db10::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46db10; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46db10((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46db10(this); return this; }


// VectorClass<int> operator assign (0x0046DB60): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


extern "C" unsigned char table_46dbd0[]; extern "C" void* __cdecl storage_46dbd0(unsigned int);
class Storage_46dbd0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_46dbd0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_46dbd0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_46dbd0(requested << 2); owns=1; } return this; }


// VectorClass<TechnoTypeClass *> operator assign (0x0046DC20): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// VectorClass<RGBClass> operator assign (0x0046DD20): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// ??0?$DynamicVectorClass@H@@QAE@ABV0@@Z (0x0046CE20): the compiler-generated DynamicVectorClass<int> copy constructor (shared/vector_class.hpp declares none,
// as TibSun vector.h); emitted by this unit's copy of the list. No source of its own.
