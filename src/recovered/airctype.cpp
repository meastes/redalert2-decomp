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
// Function comment blocks from mischa85/RedAlert2 392e904 src/airctype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/airctype.cpp where both define the function (names only).

// airctype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/airctype.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x0041BAA0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0041BAA0-0x0041BC86); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x0041BAD0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BAF0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BB10, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BB30, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BB50, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BB70, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BBB0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BBE0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BC10, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BC40, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BC50, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.


// Dynamic initializer (0x0041BC70, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0041baa0.cpp; no source of its own.




#include "shared/aircraft_type.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"


/***********************************************************************************************
 * AircraftTypeClass::AircraftTypeClass -- Constructor for aircraft objects.                   *
 *                                                                                             *
 *    This is the constructor for the aircraft object.                                         *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0041BC90)
// Speed type 4; joins AircraftTypes and records its index; aircraft
// defaults for inherited flags.

AircraftTypeClass::AircraftTypeClass(char const *ini_name) :
    TechnoTypeClass(ini_name, 4),
    ArrayIndex(-1),
    Carryall(false),
    Rotors(false),
    CustomRotor(false),
    Landable(false),
    AirportBound(false),
    Fighter(false)
{
    Create_ID();
    AircraftTypes.Add(this);
    ArrayIndex = AircraftTypes.ID(this);
    Parasiteable = true;
    ConsideredAircraft = true;
    MoveToShroud = false;
    RotCount = 32;
    Trailer = 0;
    SpawnDelay = 3;
    ImmuneToPsionics = false;
    Organic = false;
}


/// <summary>
/// Creates an aircraft type without initializing it.
/// This is the constructor used when loading a save game. The data has already been read
/// into the object, so nothing may be disturbed here beyond restoring the virtual table.
/// </summary>
// (0x0041BDB0)
// Load path.

AircraftTypeClass::AircraftTypeClass(NoInitClass const &noinit) :
    TechnoTypeClass(noinit)
{
}


/// <summary>
/// Destroys the aircraft type.
/// Any object still pointing at this type is told to forget it, and the type is removed
/// from the aircraft type heap so that nothing can find it again.
/// </summary>
// (0x0041BDE0)
// Leaves AircraftTypes.

AircraftTypeClass::~AircraftTypeClass()
{
    Detach_This_From_All(this, true);
    AircraftTypes.Delete(this);
}


/***********************************************************************************************
 * AircraftTypeClass::From_Name -- Converts an ASCII name into an aircraft type number.        *
 *                                                                                             *
 *    This routine is used to convert an ASCII representation of an aircraft into the          *
 *    matching aircraft type number. This is used by the scenario INI reader code.             *
 *                                                                                             *
 * INPUT:   name  -- Pointer to ASCII name to translate.                                       *
 *                                                                                             *
 * OUTPUT:  Returns the aircraft type number that matches the ASCII name provided. If no       *
 *          match could be found, then AIRCRAFT_NONE is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0041BE60)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall AircraftTypeClass::From_Name(char const *name)
{
    if (name) {
        for (int index = 0; index < AircraftTypes.Count(); index++) {
            if (_stricmp(AircraftTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// static void AircraftTypeClass::Init() (0x0041BEB0)
// Loads the rotor shapes. Not in the function inventory (code
// 0x0041BEB0-0x0041BECE, 31 bytes, padding to 0x0041BED0).

void AircraftTypeClass::Init()
{
    unknown_left_rotor_83c554 = MixFileClass::Retrieve("LROTOR.SHP");
    unknown_right_rotor_83c558 = MixFileClass::Retrieve("RROTOR.SHP");
}


/***********************************************************************************************
 * AircraftTypeClass::Create_One_Of -- Creates an aircraft object of the appropriate type.     *
 *                                                                                             *
 *    This routine is used to create an aircraft object that matches the aircraft type. It     *
 *    serves as a shortcut to creating an object using the "new" operator and "if" checks.     *
 *                                                                                             *
 * INPUT:   house -- The house owner of the aircraft that is to be created.                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the aircraft created. If the aircraft could not be       *
 *          created, then a NULL is returned.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0041BED0)
// A new aircraft of this type for the house (RA1: Create_One_Of).

ObjectClass *AircraftTypeClass::Create_One_Of(HouseClass *house) const
{
    return (ObjectClass *)new AircraftClass((AircraftTypeClass *)this, house);
}


// bool AircraftTypeClass::CanUseWaypoint() const (0x0041BF00)

bool AircraftTypeClass::CanUseWaypoint() const
{
    return false;
}


// bool AircraftTypeClass::CanAttackMove() const (0x0041BF10)

bool AircraftTypeClass::CanAttackMove() const
{
    return false;
}


/***********************************************************************************************
 * AircraftTypeClass::Occupy_List -- Returns with occupation list for landed aircraft.         *
 *                                                                                             *
 *    This determines the occupation list for the aircraft (if it was landed).                 *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a cell offset occupation list for the aircraft.          *
 *                                                                                             *
 * WARNINGS:   This occupation list is only valid if the aircraft is landed.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0041BF20)
// The occupation list: the origin cell and the end marker.

Cell const *AircraftTypeClass::Occupy_List(int arg1) const
{
    static UnknownStaticCell const unknown_list[] = {Cell(0, 0), Cell(0x7FFF, 0x7FFF)};
    return &unknown_list[0].Cell;
}


// Exit thunk of the function-local static in AircraftTypeClass::Occupy_List (0x0041BF80)
// Compiler generated ($E) after unknown_slot36.
// @symbol _$E47


/***********************************************************************************************
 * AircraftTypeClass::Create_And_Place -- Creates and places aircraft using normal game system *
 *                                                                                             *
 *    This routine is used to create and place an aircraft through the normal game system.     *
 *    Since creation of aircraft in this fashion is prohibited, this routine does nothing.     *
 *                                                                                             *
 * INPUT:   na                                                                                 *
 *                                                                                             *
 * OUTPUT:  Always returns a failure code (false).                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/07/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0041BF90)
// Aircraft are never placed this way (RA1: Create_And_Place).

bool AircraftTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    return false;
}


/***********************************************************************************************
 * AircraftTypeClass::Dimensions -- Fetches the graphic dimensions of the aircraft type.       *
 *                                                                                             *
 *    This routine will fetch the pixel dimensions of this aircraft type. These dimensions     *
 *    are used to control map refresh and select box rendering.                                *
 *                                                                                             *
 * INPUT:   width    -- Reference to variable that will be filled in with aircraft width.      *
 *                                                                                             *
 *          height   -- Reference to variable that will be filled in with aircraft height.     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/07/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0041BFA0)
// One cell square, twice the 0x0083C550 height.

CoordStruct AircraftTypeClass::Lepton_Dimensions() const
{
    return CoordStruct(256, 256, unknown_geometry_g * 2);
}


/***********************************************************************************************
 * AircraftTypeClass::Read_INI -- Fetches aircraft override values from the INI database.      *
 *                                                                                             *
 *    This routine will retrieve the override values for this aircraft type class object from  *
 *    the INI database specified.                                                              *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to retrieve the data from.                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the aircraft section for this type found and data retrieved from it?     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0041BFD0)
// The techno values, then the aircraft keys (rules) and the art keys
// (art INI, image section).

bool AircraftTypeClass::Read_INI(CCINIClass &ini)
{
    if (TechnoTypeClass::Read_INI(ini)) {
        Landable = ini.Get_Bool(ID, "Landable", Landable);
        AirportBound = ini.Get_Bool(ID, "AirportBound", AirportBound);
        Fighter = ini.Get_Bool(ID, "Fighter", Fighter);
        Carryall = ini.Get_Bool(ID, "Carryall", Carryall);
        Rotors = ArtINI.Get_Bool(unknown_1c8, "Rotors", Rotors);
        CustomRotor = ArtINI.Get_Bool(unknown_1c8, "CustomRotor", CustomRotor);
        Trailer = Get_Anim_Type(ArtINI, unknown_1c8, "Trailer", Trailer);
        SpawnDelay = ArtINI.Get_Int(unknown_1c8, "SpawnDelay", SpawnDelay);
        return true;
    }
    return false;
}


/// <summary>
/// Adds the aircraft type to a running checksum.
/// This routine is used by the network code to prove that every machine in a multiplayer
/// game agrees about the rules. Anything that differs between machines must be folded in
/// here or a desynchronization will go unnoticed.
/// </summary>
/// <param name="crc">The checksum engine to submit the type's data to.</param>
// (0x0041C110)

void AircraftTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    TechnoTypeClass::Compute_CRC(crc);
    crc(Rotors);
    crc(CustomRotor);
    crc(Landable);
    crc(AirportBound);
    crc(Fighter);
    crc((long)ArrayIndex);
}


/// <summary>
/// Reads the aircraft type back in from a save game stream.
/// The saved bytes carry no artwork or virtual table, so this routine rebuilds the object
/// in place and then re-fetches the voxel and shape images the aircraft draws with.
/// </summary>
/// <param name="stream">The stream to read the aircraft type from.</param>
/// <returns>Returns with S_OK, or the failure code reported by the base class.</returns>
// (0x0041C180)
// Rebuilds the object in place, swizzles Trailer and reloads the image.

HRESULT __stdcall AircraftTypeClass::Load(IStream *stream)
{
    HRESULT result = TechnoTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) AircraftTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&Trailer);
    Fetch_Voxel_Image();
    Fetch_Normal_Image();
    return S_OK;
}


/// <summary>
/// Writes the aircraft type out to a save game stream.
/// This routine is called by the save game machinery. All of the work is done by the
/// techno type layer; the aircraft type itself has nothing further to record.
/// </summary>
/// <param name="stream">The stream to write the aircraft type to.</param>
/// <param name="cleardirty">Should the object be marked as saved once it is written?</param>
/// <returns>Returns with S_OK, or the failure code reported by the base class.</returns>
// (0x0041C1F0)

HRESULT __stdcall AircraftTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = TechnoTypeClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Fetches the class identifier of the aircraft type.
/// The save game machinery asks each object for this identifier so that it can create an
/// object of the right class again when the game is loaded.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if there was nowhere to put the answer.</returns>
// (0x0041C210)

HRESULT __stdcall AircraftTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = unknown_clsid_aircraft_type_7a1b20;
    return S_OK;
}


/// <summary>
/// Fetches the aircraft type of the name specified, creating it if need be.
/// This routine is used while parsing the rules and the scenario so that a type may be
/// referred to before its own section has been read. A type made this way is added to the
/// aircraft type heap and filled in later.
/// </summary>
/// <param name="name">The INI name of the aircraft type to look for.</param>
/// <returns>Returns with a pointer to the aircraft type. This will never be NULL.</returns>
// (0x0041C250)
// The type with this ID, a new one when unknown, 0 for "<none>"/"none".

AircraftTypeClass *__fastcall AircraftTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < AircraftTypes.Count(); index++) {
        if (_stricmp(AircraftTypes[index]->ID, name) == 0) {
            return AircraftTypes[index];
        }
    }
    return new AircraftTypeClass(name);
}


// CoordStruct ObjectTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x0041C2E0)
// Inline in shared/object_type.hpp; the kept copy is AircraftType.cpp's.
// @symbol ?unknown_slot27@ObjectTypeClass@@


// void *ObjectTypeClass::Get_Image_Data() const (0x0041C300)
// Inline in shared/object_type.hpp; the kept copy is AircraftType.cpp's.
// @symbol ?unknown_slot39@ObjectTypeClass@@


// int AircraftTypeClass::Kind_Of() const (0x0041C310)

int AircraftTypeClass::Kind_Of() const
{
    return 3;
}


// int AircraftTypeClass::Size_Of() const (0x0041C320)

int AircraftTypeClass::Size_Of() const
{
    return sizeof(AircraftTypeClass);
}


// int AircraftTypeClass::Fetch_Heap_ID() const (0x0041C330)
// The type's index in AircraftTypes.

int AircraftTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// AircraftTypeClass scalar deleting destructor (0x0041C340)
// Compiler generated after the unit's last member (destructor inlined).
