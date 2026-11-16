/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/terrtype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/terrtype.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/terrtype.cpp where both functions declare the same local types (names only).

// terrtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/terrtype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/terrain_type.hpp"
#include "shared/anim_type.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/terrain.hpp"
#include "shared/shape_file.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"


// The foundation (occupy) lists of the terrain types, one per foundation size, each ended by
// Cell(0x7FFF, 0x7FFF) (TibSun terrtype.cpp's _OccupyLists, BSIZE_COUNT 22 rows of 10 cells; the
// rows after BSIZE_35 are empty). Its dynamic initializer is 0x006E4D60.

Cell _OccupyLists[22][10] = {
    { Cell(0, 0), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(0, 1), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(0, 1), Cell(1, 1), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(0, 1), Cell(1, 1), Cell(0, 2), Cell(1, 2), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(2, 0), Cell(0, 1), Cell(1, 1), Cell(2, 1), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(2, 0), Cell(0, 1), Cell(1, 1), Cell(2, 1), Cell(0, 2), Cell(1, 2), Cell(2, 2), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(2, 0), Cell(3, 0), Cell(0, 1), Cell(1, 1), Cell(2, 1), Cell(3, 1), Cell(0x7FFF, 0x7FFF) },
};


// TerrainTypeClass::TerrainTypeClass(char const *ini_name) (0x006E5260)
// Default terrain values, registration in TerrainTypes, then the default
// strength (-1: the rules value, see Read_INI) and armor (6; hypothesis).

TerrainTypeClass::TerrainTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name),
    ArrayIndex(-1),
    Foundation(0),
    TemperateOccupationBits(7),
    SnowOccupationBits(7),
    AnimationRate(0),
    AnimationProbability(0),
    WaterBound(false),
    SpawnsTiberium(false),
    IsFlammable(false),
    IsAnimated(false),
    IsVeinhole(false),
    OccupyList(0)
{
    AbstractClass::Create_ID();
    IsSelectable = false;
    IsSentient = true;
    IsStealthy = true;
    IsLegalTarget = false;
    IsInsignificant = true;
    TerrainTypes.Add(this);
    ArrayIndex = TerrainTypes.ID(this);
    MaxStrength = -1;
    Armor = 6;
}


/***********************************************************************************************
 * TerrainTypeClass::TerrainTypeClass -- The general constructor for the terrain type objects. *
 *                                                                                             *
 *    This is the constructor for terrain type objects. It is only used to construct the       *
 *    static (constant) terrain type objects.                                                  *
 *                                                                                             *
 * INPUT:   see below..                                                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006E53B0)
// Load path: installs the vtables only.

TerrainTypeClass::TerrainTypeClass(NoInitClass const &noinit) :
    ObjectTypeClass(noinit),
    RadarColor(noinit)
{
}


/// <summary>
/// Removes this terrain type from the game.
/// This routine will detach the type from everything that refers to it before it
/// drops out of the terrain type heap.
/// </summary>
// (0x006E53E0)
// Leaves the notice lists and TerrainTypes.

TerrainTypeClass::~TerrainTypeClass()
{
    Detach_This_From_All((ObjectClass *)(AbstractClass *)this, true);
    TerrainTypes.Delete(this);
}


/***********************************************************************************************
 * TerrainTypeClass::Init -- Loads terrain object shape files.                                 *
 *                                                                                             *
 *    This routine is used to load up the terrain object shape files.                          *
 *    The shape files loaded depends on theater.                                               *
 *                                                                                             *
 * INPUT:   theater  -- The theater to load the terrain shape data for.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/16/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006E5460)
// Reloads the theater images of theater-specific terrain types when the
// theater changes (or a reload is forced), freeing owned images first.
// Assumptions: unknown_1fc is "Theater="; fullname size 512.

void __fastcall TerrainTypeClass::Init(int theater)
{
    if (Debug_Map || theater != LastTheater) {
        for (int index = 0; index < TerrainTypes.Count(); index++) {
            TerrainTypeClass *terrain = TerrainTypes[index];
            if (terrain->IsTheater) {
                char fullname[512];
                _makepath(fullname, 0, 0, terrain->ID, Theaters[theater].Suffix);
                if (terrain->ImageAllocated && terrain->ImageData != 0) {
                    operator delete(terrain->ImageData);
                }
                terrain->ImageData = 0;
                terrain->ImageAllocated = false;
                terrain->ImageData = (void *)MixFileClass::Retrieve(fullname);
            }
        }
    }
}


/***********************************************************************************************
 * TerrainTypeClass::From_Name -- Convert name to terrain type.                                *
 *                                                                                             *
 *    This routine is used to convert a text name into the matching                            *
 *    terrain type number. This is used during scenario initialization.                        *
 *                                                                                             *
 * INPUT:   name  -- The name to convert.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns the TerrainType that matches the name specified. If                        *
 *          no match was found, then TERRAIN_NONE is returned.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/16/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006E5510)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall TerrainTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TerrainTypes.Count(); index++) {
            if (_stricmp(name, TerrainTypes[index]->ID) == 0) {
                return index;
            }
        }
    }
    return -1;
}


/***********************************************************************************************
 * TerrainTypeClass::Create_And_Place -- Creates and places terrain object on map.             *
 *                                                                                             *
 *    This support routine is used by the scenario editor to add a terrain object to the map.  *
 *                                                                                             *
 * INPUT:   cell  -- The cell to place the terrain object in.                                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the placement successful?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006E5560)
// Creates a terrain object of this type at the cell.

bool TerrainTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    if (new TerrainClass((TerrainTypeClass *)this, cell) != 0) {
        return true;
    }
    return false;
}


/***********************************************************************************************
 * TerrainTypeClass::Create_One_Of -- Creates a terrain object from type.                      *
 *                                                                                             *
 *    This is used to create a terrain object by using the terrain type as a guide. This       *
 *    routine is typically used by the scenario editor in order to place a terrain object      *
 *    onto the map.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the created terrain object or NULL if one couldn't be    *
 *          created.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006E55A0)
// Creates an unplaced terrain object of this type (at the zero cell).

ObjectClass *TerrainTypeClass::Create_One_Of(HouseClass *house) const
{
    return new TerrainClass((TerrainTypeClass *)this, unknown_geometry_k);
}


/***********************************************************************************************
 * TerrainTypeClass::Occupy_List -- Returns with the occupy list for the terrain object type.  *
 *                                                                                             *
 *    This routine will return with the occupy list for the terrain object type. If there is   *
 *    no occupy list for this terrain object type, then a special zero length occupy list      *
 *    pointer is returned.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the terrain object's occupy list. A zero length list is  *
 *          returned in the case of no occupy list.                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006E55D0)
// The foundation's occupation list, else an empty list (only the 0x7FFF
// end marker; a function-local static registered with atexit on first use).

Cell const *TerrainTypeClass::Occupy_List(int arg1) const
{
    if (OccupyList != 0) {
        return OccupyList;
    }
    static UnknownStaticCell const unknown_list[] = {Cell(0x7FFF, 0x7FFF)};
    return &unknown_list[0].Cell;
}


// TerrainTypeClass::Occupy_List static exit thunk (0x006E5620)
// Compiler-emitted atexit destructor of the function-local list in
// TerrainTypeClass_unknown_slot36_006e55d0.cpp; no source of its own.
// @symbol _$E57


// bool TerrainTypeClass::Read_INI(CCINIClass &ini) (0x006E5630)
// Reads the terrain keys; the foundation comes from the art INI (image
// section) and selects the occupation list; the radar colour is the first
// image frame's colour. Veinholes clear unknown_204 and set unknown_201.
// A strength of -1 takes the rules default (+0xE1C).

// The colour stored in a shape frame header (+0x0C; hypothesis: the radar
// colour of the frame).
static inline RGBClass Frame_Color(ShapeFileStruct const *shape, int frame)
{
    ShapeFileFrameStruct const *header = shape->Get_Frame(frame);
    if (header != 0) {
        unsigned char const *color = (unsigned char const *)&header->unknown_0C;
        return RGBClass(color[0], color[1], color[2]);
    }
    return RGBClass();
}

/// <summary>
/// Fetches the terrain type data from the INI database specified.
/// This routine reads the rules that make a terrain object what it is -- whether it is a
/// veinhole, whether it must sit on water, whether it burns or spawns tiberium -- along
/// with the artwork, foundation and animation settings it draws with.
/// </summary>
/// <returns>bool; Was the terrain type data read?</returns>
bool TerrainTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        if (MaxStrength == -1) {
            MaxStrength = Rules->TreeStrength;
        }
        char const *section = ID;
        IsVeinhole = ini.Get_Bool(section, "IsVeinhole", IsVeinhole);
        if (IsVeinhole) {
            IsSentient = false;
            IsLegalTarget = true;
        }
        WaterBound = ini.Get_Bool(section, "WaterBound", WaterBound);
        SpawnsTiberium = ini.Get_Bool(section, "SpawnsTiberium", SpawnsTiberium);
        IsFlammable = ini.Get_Bool(section, "IsFlammable", IsFlammable);
        Foundation = ArtINI.Get_Foundation(unknown_1c8, "Foundation", Foundation);
        OccupyList = _OccupyLists[Foundation];
        if (!IsTheater) {
            char filename[512];
            _makepath(filename, 0, 0, unknown_1c8, ".SHP");
            ImageData = (void *)MixFileClass::Retrieve(filename);
        }
        ShapeFileStruct const *shape = (ShapeFileStruct const *)Get_Image_Data();
        if (shape != 0) {
            RadarColor = Frame_Color(shape, 0);
        }
        IsAnimated = ini.Get_Bool(section, "IsAnimated", IsAnimated);
        AnimationRate = ini.Get_Int(section, "AnimationRate", AnimationRate);
        AnimationProbability = ini.Get_Double(section, "AnimationProbability", AnimationProbability);
        TemperateOccupationBits = ini.Get_Int(section, "TemperateOccupationBits", TemperateOccupationBits);
        SnowOccupationBits = ini.Get_Int(section, "SnowOccupationBits", SnowOccupationBits);
        return true;
    }
    return false;
}


/// <summary>
/// Adjusts the coordinate so that the object will rest on the ground.
/// This routine is used when a terrain object is placed, so that it never ends up
/// buried in the terrain at that location.
/// </summary>
/// <returns>Returns with the coordinate raised to ground level if it was below it.</returns>
// (0x006E5850)
// The coordinate, raised to the ground height when below it.

CoordStruct TerrainTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    if (result.Z < Map.Get_Height_GL(result)) {
        result.Z = Map.Get_Height_GL(result);
    }
    return result;
}


/// <summary>
/// Adds this terrain type's data to the CRC engine specified.
/// This routine is used by the multiplayer sync checking logic to prove that every
/// machine in the game loaded the same terrain rules.
/// </summary>
// (0x006E58C0)
// Note: the base called is AbstractTypeClass's (0x00410960).

void TerrainTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)ArrayIndex);
    crc(WaterBound);
    crc(SpawnsTiberium);
    crc(IsFlammable);
    crc((long)Foundation);
    crc(IsAnimated);
    crc((long)AnimationRate);
    crc(AnimationProbability);
}


/// <summary>
/// Loads this terrain type from the stream specified.
/// Save games written by an older version of the game lack a couple of the members, so
/// their data is shuffled into place before the artwork is fetched again.
/// </summary>
/// <returns>Returns with S_OK if the terrain type was read.</returns>
// (0x006E5950)
// Restores the type in place, reloads its voxel and image and points the
// occupation list at the foundation's list again.

HRESULT __stdcall TerrainTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TerrainTypeClass(NoInitClass());
        Fetch_Voxel_Image();
        Fetch_Normal_Image();
        OccupyList = _OccupyLists[Foundation];
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Saves this terrain type to the stream specified.
/// </summary>
/// <param name="cleardirty">Should the dirty flag be cleared by this save?</param>
/// <returns>Returns with S_OK if the terrain type was written.</returns>
// (0x006E59C0)

HRESULT __stdcall TerrainTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Fetches the class identifier of this object.
/// The save system uses this identifier to know what kind of object to create when the
/// save file is loaded back in.
/// </summary>
/// <param name="retval">Pointer to the buffer to fill in with the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no buffer was supplied.</returns>
// (0x006E59E0)

HRESULT __stdcall TerrainTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_TerrainTypeClass;
    return S_OK;
}


/// <summary>
/// Fetches the terrain type of the name specified, creating it if necessary.
/// This routine is used while the rules are being parsed, so that a terrain type may be
/// referred to before its own section has been reached.
/// </summary>
/// <returns>Returns with a pointer to the terrain type found or created.</returns>
// (0x006E5A20)
// The type with this ID, else a new one; null for "<none>" and "none".

TerrainTypeClass *__fastcall TerrainTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < TerrainTypes.Count(); index++) {
        if (_stricmp(TerrainTypes[index]->ID, name) == 0) {
            return TerrainTypes[index];
        }
    }
    return new TerrainTypeClass(name);
}


// int TerrainTypeClass::Kind_Of() const (0x006E5AB0)
// Compiler-emitted from shared/terrain_type.hpp in this unit; no source of its own.


// int TerrainTypeClass::Size_Of() const (0x006E5AC0)
// Compiler-emitted from shared/terrain_type.hpp in this unit; no source of its own.
