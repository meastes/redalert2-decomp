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

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table

#include "objtype.h"

#include "sun.h"
#include "_mixfile.h"
#include "_rules.h"
#include "_theater.h"
#include "aircraft.h"
#include "airctype.h"
#include "building.h"
#include "builtype.h"
#include "ccfile.h"
#include "coord.h"
#include "data.h"
#include "globals.h"
#include "house.h"
#include "mixfile.h"
#include "rawfile.h"
#include "rules.h"
#include "scenario.h"
#include "shapeset.h"
#include "unittype.h"
#include "vector.h"
#include "voc.h"

/*
**	Selected objects have a special marking box around them. This is the shapes that are
**	used for this purpose.
*/
void const * ObjectTypeClass::SelectShapes = 0;

void const * ObjectTypeClass::PipShapes = 0;
void const * ObjectTypeClass::Pip2Shapes = 0;

void const * ObjectTypeClass::TalkBubbleShapes = 0;

bool ObjectTypeClass::SelectShapesAllocated = false; // RA2: whether One_Time allocated the shapes
bool ObjectTypeClass::PipShapesAllocated = false; // RA2: as above
bool ObjectTypeClass::Pip2ShapesAllocated = false; // RA2: as above
bool ObjectTypeClass::TalkBubbleShapesAllocated = false; // RA2: as above

DynamicVectorClass<ObjectTypeClass *> ObjectTypeClass::ObjectTypes;


template<> inline bool DynamicVectorClass<ObjectTypeClass *>::Add(ObjectTypeClass * const & object) // RA2: this unit's Add holds the new length in a variable before the Resize call (the sum is formed in the register that held Length), as in link2.cpp
{
	if (ActiveCount >= Length()) { // RA2: as above
		if ((IsAllocated || !VectorMax) && GrowthStep > 0) { // RA2: as above
			int new_length = Length(); // RA2: as above
			new_length += GrowthStep; // RA2: as above
			if (!Resize(new_length)) { // RA2: as above
				return(false); // RA2: as above
			}
		} else {
			return(false); // RA2: as above
		}
	}
	(*this)[ActiveCount++] = object; // RA2: as above
	return(true); // RA2: as above
}


/***********************************************************************************************
 * ObjectTypeClass::ObjectTypeClass -- Normal constructor for object type class objects.       *
 *                                                                                             *
 *    This is the base constructor that is used when constructing the object type classes.     *
 *    Every tangible game piece type calls this constructor for the ObjectTypeClass. This      *
 *    class holds static information that is common to objects in general.                     *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass::ObjectTypeClass(char const * ininame) :
	BASECLASS(ininame),
	MaxSize(0),
	CrushSound(VOC_NONE),
	AmbientSound(VOC_NONE), // RA2: 0x1C4 starts at -1, like CrushSound
	GraphicName(),
	AlternateArcticArt(false), // RA2: 0x1E1 starts clear
	ArcticArtInUse(false), // RA2: 0x1E2 starts clear
	AlphaGraphicName(),
	IsTheater(false),
	IsCrushable(false),
	IsBombable(true), // RA2: 0x1FE starts set
	IsStealthy(false),
	IsSelectable(true),
	IsLegalTarget(true),
	IsInsignificant(false),
	IsImmune(false),
	IsSentient(false),
	IsFootprint(true),
	IsVoxel(false),
	IsNewTheater(false),
	IsHasRadialIndicator(false),
	IsIgnoresFirestorm(false),
	UseLineTrail(false), // RA2: 0x20A starts clear
	LineTrailColor(128, 128, 128), // RA2: 0x20B..0x20D are stored as 0x80
	LineTrailColorDecrement(16), // RA2: 0x210 is stored as 0x10
	RadialColor(0,0,0),
	Armor(ARMOR_NONE),
	MaxStrength(0),
	ImageData(0),
	ImageAllocated(false), // RA2: 0xA8 starts clear
	AlphaImageData(0), // RA2: as above
	NoSpawnAlt(false) // RA2: 0x1B8 starts clear
{
	GraphicName = IniName;

	AlphaGraphicName = ""; // RA2: copied from the string at 0x83C55C

	ObjectTypes.Add(this);
}


/// <summary>
/// Constructor for the object type object.
/// This is the no-initialization constructor. It is used when the object type is about to
/// be overwritten wholesale by a load from a save game stream, so the members are
/// deliberately left as they are.
/// </summary>
ObjectTypeClass::ObjectTypeClass(NoInitClass const & x) :
	BASECLASS(x),
	RadialColor(x),
	GraphicName(x),
	AlphaGraphicName(x),
	Voxel(x),
	AuxVoxel(x),
	AuxVoxel2(x),
	LineTrailColor(x), // RA2: the line trail colour is restored too
	ImageData(NULL)
{

}


/// <summary>
/// Destructor for the object type object.
/// This routine will detach the object type from the master list of object types, so that
/// no further code can find it by name or by index.
/// </summary>
ObjectTypeClass::~ObjectTypeClass(void)
{
	ObjectTypes.Delete(this);
	if (ImageAllocated && ImageData != NULL) { // RA2: 0x5D5B83 frees the loaded image (+0xA4) when the flag at +0xA8 is set
		operator delete((void *)ImageData); // RA2: as above
		ImageData = NULL; // RA2: as above
		ImageAllocated = false; // RA2: as above
	}
}



/***********************************************************************************************
 * ObjectTypeClass::Max_Pips -- Fetches the maximum pips allowed for this object.              *
 *                                                                                             *
 *    This routine will return the maximum number of pips that can be displayed for this       *
 *    object. When dealing with generic objects, this value is always zero.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pip boxes (empty or otherwise) to display.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectTypeClass::Max_Pips(void) const
{
	return(0);
}


/***********************************************************************************************
 * ObjectTypeClass::Dimensions -- Gets the dimensions of the object in pixels.                 *
 *                                                                                             *
 *    This routine will fetch the dimensions of this object expressed as pixels width and      *
 *    pixels height. This information can be used to intelligently update the clipping         *
 *    rectangles.                                                                              *
 *                                                                                             *
 * INPUT:   width    -- Reference to the width variable that will be filled in.                *
 *                                                                                             *
 *          height   -- Reference to the height variable that will be filled in.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Point3D ObjectTypeClass::Pixel_Dimensions(void) const
{
	return(Point3D(10, 10, 10));
}


/***********************************************************************************************
 * ObjectTypeClass::Dimensions -- Gets the dimensions of the object in leptons.                *
 *                                                                                             *
 *    This routine will fetch the dimensions of this object expressed as leptons width and     *
 *    leptons height. This information can be used to intelligently update the clipping        *
 *    rectangles.                                                                              *
 *                                                                                             *
 * INPUT:   width    -- Reference to the width variable that will be filled in.                *
 *                                                                                             *
 *          height   -- Reference to the height variable that will be filled in.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Point3D ObjectTypeClass::Lepton_Dimensions(void) const
{
	Point3D coord; // RA2: RA2's form
	coord.X = 0x100; // RA2: as above
	coord.Y = 0x100; // RA2: as above
	coord.Z = LEVEL_LEPTON_H * 2; // RA2: as above
	return coord; // RA2: as above
}



/***********************************************************************************************
 * ObjectTypeClass::Cost_Of -- Returns the cost to buy this unit.                              *
 *                                                                                             *
 *    This routine will return the cost to purchase this unit. This routine is expected to be  *
 *    overridden by the objects that can actually be purchased. All other object types can     *
 *    simply return zero since this value won't be used.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the cost of the object.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectTypeClass::Cost_Of(HouseClass * house) const
{
	return(0);
}


/***********************************************************************************************
 * ObjectTypeClass::Time_To_Build -- Fetches the time to construct this object.                *
 *                                                                                             *
 *    This routine will fetch the time in takes to construct this object. Objects that can     *
 *    be constructed will override this routine in order to return a useful value.             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time units (arbitrary) that it takes to construct this object.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ObjectTypeClass::Time_To_Build(void) const
{
	return(0);
}


/***********************************************************************************************
 * ObjectTypeClass::Get_Cameo_Data -- Fetches pointer to cameo data for this object type.      *
 *                                                                                             *
 *    This routine will return with the cameo data pointer for this object type. It is         *
 *    expected that objects that can appear on the sidebar will override this routine in order *
 *    to provide proper cameo data pointer.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cameo shape data.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * ObjectTypeClass::Get_Cameo_Data(void) const
{
	return(NULL);
}


/***********************************************************************************************
 * ObjectTypeClass::Occupy_List -- Returns with simple occupation list for object.             *
 *                                                                                             *
 *    This routine returns a pointer to a simple occupation list for this object. Since at     *
 *    this tier of the object class chain, the exact shape of the object is indeterminate,     *
 *    this function merely returns a single cell occupation list. This actually works for      *
 *    most vehicles.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to a simple occupation list.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
Cell const * ObjectTypeClass::Occupy_List(bool) const
{
	static Cell const _list[] = {Cell(0, 0), REFRESH_EOL};
	return(_list);
}


/***********************************************************************************************
 * ObjectTypeClass::One_Time -- Handles one time processing for object types.                  *
 *                                                                                             *
 *    This routine is used to handle the once per game processing required for object types.   *
 *    This consists of loading any data and initializing any data tables the game requires.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine goes to disk.                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void ObjectTypeClass::One_Time(void)
{
	/*
	**	RA2: each shape file is read from a loose file first, and only taken from the mix
	**	files when there is none. A shape read from a loose file was allocated here, so it
	**	is remembered to be freed again by Free_One_Time.
	*/
	if (SelectShapes == NULL) { // RA2: loaded once
		RawFileClass file("PIPBRD.SHP"); // RA2: the selection box art is PIPBRD.SHP
		SelectShapes = Load_Alloc_Data(file); // RA2: from a loose file
		SelectShapesAllocated = (SelectShapes != NULL); // RA2: remembered for freeing
		if (SelectShapes == NULL) { // RA2: else from the mix files
			SelectShapes = MFCD::Retrieve("PIPBRD.SHP"); // RA2: as above
		}
	}
	if (PipShapes == NULL) { // RA2: as above
		RawFileClass file("PIPS.SHP"); // RA2: as above
		PipShapes = Load_Alloc_Data(file); // RA2: as above
		PipShapesAllocated = (PipShapes != NULL); // RA2: as above
		if (PipShapes == NULL) { // RA2: as above
			PipShapes = MFCD::Retrieve("PIPS.SHP"); // RA2: as above
		}
	}
	if (Pip2Shapes == NULL) { // RA2: as above
		RawFileClass file("PIPS2.SHP"); // RA2: as above
		Pip2Shapes = Load_Alloc_Data(file); // RA2: as above
		Pip2ShapesAllocated = (Pip2Shapes != NULL); // RA2: as above
		if (Pip2Shapes == NULL) { // RA2: as above
			Pip2Shapes = MFCD::Retrieve("PIPS2.SHP"); // RA2: as above
		}
	}
	if (TalkBubbleShapes == NULL) { // RA2: as above
		RawFileClass file("TALKBUBL.SHP"); // RA2: as above
		TalkBubbleShapes = Load_Alloc_Data(file); // RA2: as above
		TalkBubbleShapesAllocated = (TalkBubbleShapes != NULL); // RA2: as above
		if (TalkBubbleShapes == NULL) { // RA2: as above
			TalkBubbleShapes = MFCD::Retrieve("TALKBUBL.SHP"); // RA2: as above
		}
	}
}


/// <summary>
/// Frees the shapes that One_Time read from loose files.
/// This routine is called when the game shuts down. Shapes that One_Time took from the mix
/// files belong to the mix files and are left alone.
/// </summary>
void ObjectTypeClass::Free_One_Time(void) // RA2: not in TibSun, at 0x005D5F40
{
	if (SelectShapesAllocated) {
		delete (void *)SelectShapes;
		SelectShapesAllocated = false;
	}
	if (PipShapesAllocated) {
		delete (void *)PipShapes;
		PipShapesAllocated = false;
	}
	if (Pip2ShapesAllocated) {
		delete (void *)Pip2Shapes;
		Pip2ShapesAllocated = false;
	}
	if (TalkBubbleShapesAllocated) {
		delete (void *)TalkBubbleShapes;
		TalkBubbleShapesAllocated = false;
	}
}






/***********************************************************************************************
 * ObjectTypeClass::Who_Can_Build_Me -- Determine what building can build this object type.    *
 *                                                                                             *
 *    This routine will scan through all available factory buildings and determine which       *
 *    is capable of building this object type. The scan can be controlled to scan for only     *
 *    factory buildings that are free to produce now or those that could produce this          *
 *    object type if conditions permit.                                                        *
 *                                                                                             *
 * INPUT:   intheory -- Should the general (when conditions permit) case be examined to see    *
 *                      if a building could build this object type "in theory" even though it  *
 *                      might currently be otherwise occupied?                                 *
 *                                                                                             *
 *          legal    -- Check for building prerequisite and technology level rules? Usually    *
 *                      this would be 'true' for human controlled requests and 'false' for     *
 *                      the computer. This is because the computer is usually not under        *
 *                      the normal restrictions that the player is under.                      *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building that can produce the object of this         *
 *          type. If no suitable factory building could be found, then NULL is returned.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * ObjectTypeClass::Who_Can_Build_Me(bool intheory, bool needsnopower, bool legal, HouseClass * house) const
{
	BuildingClass * freebuilding = NULL;
	int ownable = Get_Ownable();

	int count = house->Buildings.Count(); // RA2: the house's buildings walked by index
	for (int index = 0; index < count; index++) { // RA2: by index
		BuildingClass * building = house->Buildings[index]; // RA2: each building

		if (!building->IsInLimbo &&
			building->Class->ToBuild == RTTI &&
			(!needsnopower || building->IsOn) &&
			building->Mission != MISSION_DECONSTRUCTION && building->MissionQueue != MISSION_DECONSTRUCTION &&
			(!legal || building->House->Can_Build(this, true, true) > 0) &&
			(building->Class->Get_Ownable() & ownable)) { // RA2: ownable by this type's owners

			/*
			**	HACK ALERT: Helipads can build aircraft and airstrips can build
			**	fixed wing craft only.
			*/
			if (!intheory && building->HasAnyLink() && RTTI == RTTI_AIRCRAFTTYPE) { // RA2: an aircraft looks for a linked factory
				if (RTTI == RTTI_AIRCRAFTTYPE && building->HasFreeLink()) { // RA2: with a free dock
					if (building->IsLeader) return(building); // RA2: the leader first
					freebuilding = building; // RA2: else any free one
				}
			} else {
				if (RTTI == RTTI_UNITTYPE && ((TechnoTypeClass const *)this)->Naval) { // RA2: a naval unit needs a naval factory
					if (building->Class->Naval) { // RA2: naval factory
						if (building->IsLeader) return(building); // RA2: the leader first
						goto leader; // RA2: else as a leader candidate
					}
				} else {
					if (!building->Class->Naval) { // RA2: a land unit needs a land factory
						leader: // RA2: leader candidate
						if (building->IsLeader) return(building); // RA2: the leader first
						freebuilding = building; // RA2: else any one
					}
				}
			}
		}
	}

	if (freebuilding != NULL) {
		return(freebuilding);
	}
	return(NULL); // RA2: none
}


/// <summary>
/// Loads the turret voxel of one of a charger's weapons.
/// An object type with several weapons can give each weapon its own turret, named after the
/// type's image with "TUR" and the weapon index (no index for the first weapon). The voxel and
/// its motion data both have to load, or neither is kept.
/// </summary>
/// <param name="image">The image name the turret's file names are built from.</param>
/// <param name="index">The weapon whose turret to load.</param>
/// <returns>Whether the turret loaded; false when there is no turret file.</returns>
bool ObjectTypeClass::Load_Charger_Turret(char const * image, int index) // RA2: not in TibSun, at 0x005D6150
{
	char buffer[256];
	char name[260];

	if (index == 0) {
		sprintf(buffer, "%sTUR", image);
	} else {
		sprintf(buffer, "%sTUR%d", image, index);
	}
	_makepath(name, NULL, NULL, buffer, ".VXL");
	CCFileClass vxl(name);
	if (!vxl.Is_Available()) {
		return(false);
	}

	bool voxelok = false;
	ChargerTurrets[index].VoxLib = new VoxelLibrary(vxl);
	if (ChargerTurrets[index].VoxLib != NULL && !ChargerTurrets[index].VoxLib->Load_Failed()) {
		voxelok = true;
	}

	if (index == 0) {
		sprintf(buffer, "%sTUR", image);
	} else {
		sprintf(buffer, "%sTUR%d", image, index);
	}
	_makepath(name, NULL, NULL, buffer, ".HVA");
	CCFileClass hva(name);
	bool motionok = false;
	ChargerTurrets[index].MotLib = new MotionLibrary(hva);
	if (ChargerTurrets[index].MotLib != NULL && !ChargerTurrets[index].MotLib->Load_Failed()) {
		motionok = true;
	}

	if (voxelok && motionok) {
		ChargerTurrets[index].MotLib->Scale(ChargerTurrets[index].VoxLib->Get_Layer_Info(0, 0).Scale);
		return(true);
	}
	delete ChargerTurrets[index].VoxLib;
	delete ChargerTurrets[index].MotLib;
	return(false);
}


/// <summary>
/// Loads the barrel voxel of one of a charger's weapons.
/// This works as Load_Charger_Turret does, with "BARL" in place of "TUR", except that a
/// missing barrel file is no failure: a turret need not have a barrel.
/// </summary>
/// <param name="image">The image name the barrel's file names are built from.</param>
/// <param name="index">The weapon whose barrel to load.</param>
/// <returns>Whether the barrel loaded or there is none; false when its files would not load.</returns>
bool ObjectTypeClass::Load_Charger_Barrel(char const * image, int index) // RA2: not in TibSun, at 0x005D63F0
{
	char buffer[256];
	char name[260];

	if (index == 0) {
		sprintf(buffer, "%sBARL", image);
	} else {
		sprintf(buffer, "%sBARL%d", image, index);
	}
	_makepath(name, NULL, NULL, buffer, ".VXL");
	CCFileClass vxl(name);
	if (!vxl.Is_Available()) {
		return(true);
	}

	bool voxelok = false;
	ChargerBarrels[index].VoxLib = new VoxelLibrary(vxl);
	if (ChargerBarrels[index].VoxLib != NULL && !ChargerBarrels[index].VoxLib->Load_Failed()) {
		voxelok = true;
	}

	if (index == 0) {
		sprintf(buffer, "%sBARL", image);
	} else {
		sprintf(buffer, "%sBARL%d", image, index);
	}
	_makepath(name, NULL, NULL, buffer, ".HVA");
	CCFileClass hva(name);
	bool motionok = false;
	ChargerBarrels[index].MotLib = new MotionLibrary(hva);
	if (ChargerBarrels[index].MotLib != NULL && !ChargerBarrels[index].MotLib->Load_Failed()) {
		motionok = true;
	}

	if (voxelok && motionok) {
		ChargerBarrels[index].MotLib->Scale(ChargerBarrels[index].VoxLib->Get_Layer_Info(0, 0).Scale);
		return(true);
	}
	delete ChargerBarrels[index].VoxLib;
	delete ChargerBarrels[index].MotLib;
	return(false);
}


/// <summary>
/// Frees the turret and barrel voxels of every charger weapon.
/// </summary>
void ObjectTypeClass::Free_Charger_Voxels(void) // RA2: not in TibSun, at 0x005D66A0
{
	for (int index = 0; index < ARRAY_SIZE(ChargerTurrets); index++) {
		if (ChargerTurrets[index].VoxLib != NULL) {
			delete ChargerTurrets[index].VoxLib;
			ChargerTurrets[index].VoxLib = NULL;
		}
		if (ChargerTurrets[index].MotLib != NULL) {
			delete ChargerTurrets[index].MotLib;
			ChargerTurrets[index].MotLib = NULL;
		}
		if (ChargerBarrels[index].VoxLib != NULL) {
			delete ChargerBarrels[index].VoxLib;
			ChargerBarrels[index].VoxLib = NULL;
		}
		if (ChargerBarrels[index].MotLib != NULL) {
			delete ChargerBarrels[index].MotLib;
			ChargerBarrels[index].MotLib = NULL;
		}
	}
}











/// <summary>
/// Fetches the voxel artwork for this object type.
/// This routine will load the voxel model and the motion data that animates it, along
/// with the turret, barrel, or weapon models that belong with it. If any piece of the set
/// is missing or refuses to load, the whole set is thrown away so that the object type is
/// left with no voxel artwork at all.
/// </summary>
void ObjectTypeClass::Fetch_Voxel_Image(void)
{
	char name[256]; // RA2: RA2's form
	char fullname[260]; // RA2: as above
	bool failed = false;

	_makepath(fullname, 0, 0, GraphicName, ".VXL"); // RA2: as above
	CCFileClass voxel(fullname); // RA2: as above
	if (voxel.Is_Available()) { // RA2: as above
		delete Voxel.VoxLib;
		Voxel.VoxLib = new VoxelLibrary(voxel); // RA2: as above
		if (Voxel.VoxLib == NULL || Voxel.VoxLib->Load_Failed()) {
			failed = true;
		}
		_makepath(fullname, 0, 0, GraphicName, ".HVA"); // RA2: as above
		CCFileClass motion(fullname); // RA2: as above

		//if (hva.Is_Available()) {
		delete Voxel.MotLib;
		Voxel.MotLib = new MotionLibrary(motion); // RA2: as above
		if (Voxel.MotLib != NULL && !Voxel.MotLib->Load_Failed()) { // RA2: as above
			Voxel.MotLib->Scale(Voxel.VoxLib->Get_Layer_Info(0, 0).Scale); // RA2: as above
		} else { // RA2: as above
			failed = true;
		}
	} else {
		failed = true;
	}

	UnitTypeClass *utype = (UnitTypeClass *)this;
	TechnoTypeClass *ttype = (TechnoTypeClass *)this; // RA2: as above

	if (Fetch_RTTI() == RTTI_UNITTYPE && !utype->IsTurretEquipped) { // RA2: as above
		if (strcmp((const char *)utype->IniName, "APC") == 0) { // RA2: as above
			sprintf(name, "%sW", (char const *)GraphicName); // RA2: as above
			_makepath(fullname, 0, 0, name, ".VXL"); // RA2: as above
			CCFileClass voxel(fullname); // RA2: as above
			if (voxel.Is_Available()) { // RA2: as above
				delete AuxVoxel.VoxLib; // RA2: as above
				AuxVoxel.VoxLib = new VoxelLibrary(voxel); // RA2: as above
				if (AuxVoxel.VoxLib == NULL || AuxVoxel.VoxLib->Load_Failed()) { // RA2: as above
					failed = true;
				}
				sprintf(name, "%sW", (char const *)GraphicName); // RA2: as above
				_makepath(fullname, 0, 0, name, ".HVA"); // RA2: as above
				CCFileClass motion(fullname); // RA2: as above
				delete AuxVoxel.MotLib; // RA2: as above
				AuxVoxel.MotLib = new MotionLibrary(motion); // RA2: as above
				if (AuxVoxel.MotLib != NULL && !AuxVoxel.MotLib->Load_Failed()) { // RA2: as above
					AuxVoxel.MotLib->Scale(AuxVoxel.VoxLib->Get_Layer_Info(0, 0).Scale);
				} else {
					failed = true;
				}
			}
		}
		if (NoSpawnAlt) { // RA2: as above
			sprintf(name, "%sWO", (char const *)GraphicName); // RA2: as above
			_makepath(fullname, 0, 0, name, ".VXL"); // RA2: as above
			CCFileClass voxel(fullname); // RA2: as above
			if (voxel.Is_Available()) { // RA2: as above
				delete AuxVoxel.VoxLib; // RA2: as above
				AuxVoxel.VoxLib = new VoxelLibrary(voxel); // RA2: as above
				if (AuxVoxel.VoxLib == NULL || AuxVoxel.VoxLib->Load_Failed()) { // RA2: as above
					failed = true;
				}
				sprintf(name, "%sWO", (char const *)GraphicName); // RA2: as above
				_makepath(fullname, 0, 0, name, ".HVA"); // RA2: as above
				CCFileClass motion(fullname); // RA2: as above
				delete AuxVoxel.MotLib; // RA2: as above
				AuxVoxel.MotLib = new MotionLibrary(motion); // RA2: as above
				if (AuxVoxel.MotLib != NULL && !AuxVoxel.MotLib->Load_Failed()) { // RA2: as above
					AuxVoxel.MotLib->Scale(AuxVoxel.VoxLib->Get_Layer_Info(0, 0).Scale);
				} else {
					failed = true;
				}
			}
		}
	} else if (Fetch_RTTI() == RTTI_UNITTYPE && ttype->Has_Turret_Weapons()) { // RA2: as above
		int count = ttype->TurretCount; // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: as above
			if (!Load_Charger_Turret(GraphicName, index)) { // RA2: as above
				failed = true;
				break; // RA2: as above
			}
		}
	} else {
		sprintf(name, "%sTUR", (char const *)GraphicName); // RA2: as above
		_makepath(fullname, 0, 0, name, ".VXL"); // RA2: as above
		CCFileClass voxel(fullname); // RA2: as above
		if (voxel.Is_Available()) { // RA2: as above
			delete AuxVoxel.VoxLib; // RA2: as above
			AuxVoxel.VoxLib = new VoxelLibrary(voxel); // RA2: as above
			if (AuxVoxel.VoxLib == NULL || AuxVoxel.VoxLib->Load_Failed()) {
				failed = true;
			}
			sprintf(name, "%sTUR", (char const *)GraphicName); // RA2: as above
			_makepath(fullname, 0, 0, name, ".HVA"); // RA2: as above
			CCFileClass motion(fullname); // RA2: as above
			delete AuxVoxel.MotLib; // RA2: as above
			AuxVoxel.MotLib = new MotionLibrary(motion); // RA2: as above
			if (AuxVoxel.MotLib != NULL && !AuxVoxel.MotLib->Load_Failed()) { // RA2: as above
				AuxVoxel.MotLib->Scale(AuxVoxel.VoxLib->Get_Layer_Info(0, 0).Scale); // RA2: as above
			} else {
				failed = true;
			}
		}
	}

	if (Fetch_RTTI() != RTTI_UNITTYPE || utype->IsTurretEquipped) { // RA2: as above
		if (Fetch_RTTI() == RTTI_UNITTYPE && ttype->Has_Turret_Weapons()) { // RA2: as above
			int count = ttype->TurretCount; // RA2: as above
			for (int index = 0; index < count; index++) { // RA2: as above
				if (!Load_Charger_Barrel(GraphicName, index)) { // RA2: as above
					failed = true;
					break; // RA2: as above
				}
			}
		} else { // RA2: as above
			sprintf(name, "%sBARL", (char const *)GraphicName); // RA2: as above
			_makepath(fullname, 0, 0, name, ".VXL"); // RA2: as above
			CCFileClass voxel(fullname); // RA2: as above
			if (voxel.Is_Available()) { // RA2: as above
				delete AuxVoxel2.VoxLib; // RA2: as above
				AuxVoxel2.VoxLib = new VoxelLibrary(voxel); // RA2: as above
				if (AuxVoxel2.VoxLib == NULL || AuxVoxel2.VoxLib->Load_Failed()) { // RA2: as above
					failed = true;
				}
				sprintf(name, "%sBARL", (char const *)GraphicName); // RA2: as above
				_makepath(fullname, 0, 0, name, ".HVA"); // RA2: as above
				CCFileClass motion(fullname); // RA2: as above
				delete AuxVoxel2.MotLib; // RA2: as above
				AuxVoxel2.MotLib = new MotionLibrary(motion); // RA2: as above
				if (AuxVoxel2.MotLib != NULL && !AuxVoxel2.MotLib->Load_Failed()) { // RA2: as above
					AuxVoxel2.MotLib->Scale(AuxVoxel2.VoxLib->Get_Layer_Info(0, 0).Scale);
				} else {
					failed = true;
				}
			}
		}
	}

	if (failed) { // RA2: as above
		if (Voxel.VoxLib != NULL) { // RA2: as above
			delete Voxel.VoxLib; // RA2: as above
			Voxel.VoxLib = NULL; // RA2: as above
		}
		if (Voxel.MotLib != NULL) { // RA2: as above
			delete Voxel.MotLib;
			Voxel.MotLib = NULL; // RA2: as above
		}
		if (Fetch_RTTI() == RTTI_UNITTYPE && ttype->Has_Turret_Weapons()) { // RA2: as above
			Free_Charger_Voxels(); // RA2: as above
		} else { // RA2: as above
			if (AuxVoxel.VoxLib != NULL) { // RA2: as above
				delete AuxVoxel.VoxLib; // RA2: as above
				AuxVoxel.VoxLib = NULL; // RA2: as above
			}
			if (AuxVoxel.MotLib != NULL) { // RA2: as above
				delete AuxVoxel.MotLib; // RA2: as above
				AuxVoxel.MotLib = NULL; // RA2: as above
			}
		}
	} else {
		int size = Voxel.VoxLib->Get_Layer_Info(0, 0).XSize; // RA2: as above
		for (int index = 0; index < (int)Voxel.VoxLib->Get_Layer_Count(); index++) { // RA2: as above
			if (size <= Voxel.VoxLib->Get_Layer_Info(index, 0).XSize) { // RA2: as above
				size = Voxel.VoxLib->Get_Layer_Info(index, 0).XSize; // RA2: as above
			}
			if (size <= Voxel.VoxLib->Get_Layer_Info(index, 0).YSize) { // RA2: as above
				size = Voxel.VoxLib->Get_Layer_Info(index, 0).YSize; // RA2: as above
			}
			if (size <= Voxel.VoxLib->Get_Layer_Info(index, 0).ZSize) { // RA2: as above
				size = Voxel.VoxLib->Get_Layer_Info(index, 0).ZSize; // RA2: as above
			}
		}
		MaxSize = size > 8 ? size : 8; // RA2: as above
		Clear_Voxel_Index();
	}
}



/// <summary>
/// Fetches the turret and barrel voxels for this object type.
/// This routine is used to attach the auxiliary artwork to a unit that already has its
/// main voxel loaded. Object types that carry no turret have nothing to fetch and are
/// left alone.
/// </summary>
void ObjectTypeClass::Fetch_Aux_Voxel_Image(void)
{
	char buffer[256];
	char name[260];

	if (RTTI == RTTI_UNITTYPE && Get_Image_Data() != NULL && ((TechnoTypeClass *)this)->IsTurretEquipped) { // RA2: a turreted unit's turret voxel
		sprintf(buffer, "%sTUR", (char const *)GraphicName); // RA2: NAMETUR
		_makepath(name, NULL, NULL, buffer, ".VXL"); // RA2: its VXL
		CCFileClass tvxl(name); // RA2: opened

		if (tvxl.Is_Available()) { // RA2: if present
			delete AuxVoxel.VoxLib; // RA2: replaces the aux voxel
			AuxVoxel.VoxLib = new VoxelLibrary(tvxl); // RA2: loaded

			if (AuxVoxel.VoxLib != NULL && !AuxVoxel.VoxLib->Load_Failed()) { // RA2: with its motion
				sprintf(buffer, "%sTUR", (char const *)GraphicName); // RA2: NAMETUR
				_makepath(name, NULL, NULL, buffer, ".HVA"); // RA2: its HVA
				CCFileClass thva(name); // RA2: opened

				delete AuxVoxel.MotLib; // RA2: replaces the motion library
				AuxVoxel.MotLib = new MotionLibrary(thva); // RA2: loaded
				AuxVoxel.MotLib->Scale(AuxVoxel.VoxLib->Get_Layer_Info(0, 0).Scale); // RA2: scaled to the voxel
			}
		}

		sprintf(buffer, "%sBARL", (char const *)GraphicName); // RA2: and the barrel voxel
		_makepath(name, NULL, NULL, buffer, ".VXL"); // RA2: NAMEBARL
		CCFileClass bvxl(name); // RA2: its VXL

					//if (thva.Is_Available()) {
		if (bvxl.Is_Available()) { // RA2: if present
			delete AuxVoxel2.VoxLib; // RA2: replaces the second aux voxel
			AuxVoxel2.VoxLib = new VoxelLibrary(bvxl); // RA2: loaded

			if (AuxVoxel2.VoxLib != NULL && !AuxVoxel2.VoxLib->Load_Failed()) { // RA2: with its motion
				sprintf(buffer, "%sBARL", (char const *)GraphicName); // RA2: NAMEBARL
				_makepath(name, NULL, NULL, buffer, ".HVA"); // RA2: its HVA
				CCFileClass bhva(name); // RA2: opened

				delete AuxVoxel2.MotLib; // RA2: replaces the motion library
				AuxVoxel2.MotLib = new MotionLibrary(bhva); // RA2: loaded
				AuxVoxel2.MotLib->Scale(AuxVoxel2.VoxLib->Get_Layer_Info(0, 0).Scale); // RA2: scaled to the voxel
			}
		}
	}
}


void Theater_Name_Fixup(char * name, TheaterType theater); // RA2: defined below at 0x5D7BD0


/// <summary>
/// Fetches the shape artwork for this object type.
/// This routine will work out the artwork name that suits the current theater and then
/// attach the matching shape data to the object type. A type whose artwork is not present
/// is left with no image, which the drawing code copes with.
/// </summary>
void ObjectTypeClass::Fetch_Normal_Image(void)
{
	char fullname[260];
	if (AlternateArcticArt && Scen->Theater == THEATER_SNOW && !ImageAllocated) { // RA2: snow maps use the "A" art variant of the graphic name (flag +0x1E1, state +0x1E2)
		if (!ArcticArtInUse) { // RA2: as above
			char buffer[256]; // RA2: as above
			sprintf(buffer, "%sA", (char const *)GraphicName); // RA2: as above
			GraphicName = buffer; // RA2: as above
			ArcticArtInUse = true; // RA2: as above
		}
	} else {
		ArcticArtInUse = false; // RA2: as above
	}

	_makepath(fullname, NULL, NULL, GraphicName, ".SHP");

	if (IsTheater) {
		_makepath(fullname, NULL, NULL, GraphicName, Theaters[Scen->Theater].Suffix);
	} else if (IsNewTheater) {
		Theater_Name_Fixup(fullname, Scen->Theater); // RA2: the free function at 0x5D7BD0, expanded here
	}

	if (ImageAllocated && ImageData != NULL) { // RA2: frees the previous image first
		operator delete((void *)ImageData); // RA2: as above
	}
	ImageData = NULL; // RA2: as above
	ImageAllocated = false; // RA2: as above

	ShapeSet const * image = (ShapeSet const *)MFCD::Retrieve(fullname);
	ImageData = image; // RA2: stored before the test
	if (image) {
		int maxsize = MAX(image->Get_Width(), image->Get_Height());
		maxsize = MAX(maxsize, 8);
		MaxSize = maxsize;
	}
}



/// <summary>
/// Fetches the object type's data from the INI database.
/// This routine will read the attributes common to every object -- armor, strength,
/// crushability, and the targeting flags -- from the rules database, and the artwork
/// settings from the art database. Types that are not voxels have their shape artwork
/// attached as part of the process.
/// </summary>
/// <returns>bool; Was the object type's section found and read?</returns>
bool ObjectTypeClass::Read_INI(CCINIClass const & ini)
{
	if (AbstractTypeClass::Read_INI(ini)) { // RA2: RA2's form
		char buffer[25]; // RA2: as above
		strcpy(buffer, GraphicName); // RA2: as above
		ini.Get_String(IniName, "Image", buffer, GraphicName, sizeof(GraphicName)); // RA2: as above
		strcpy(buffer, AlphaGraphicName); // RA2: as above
		ini.Get_String(IniName, "AlphaImage", buffer, AlphaGraphicName, sizeof(AlphaGraphicName)); // RA2: as above
		char name[128]; // RA2: as above
		int sound = CrushSound; // RA2: as above
		if (ini.Get_String(IniName, "CrushSound", "", name, sizeof(name))) { // RA2: as above
			int found = VocClass::From_Name(name); // RA2: as above
			if (found != VOC_NONE) { // RA2: as above
				sound = found; // RA2: as above
			}
		}
		CrushSound = (VocType)sound; // RA2: as above
		sound = AmbientSound; // RA2: as above
		int found; // RA2: as above
		if (!ini.Get_String(IniName, "AmbientSound", "", name, sizeof(name)) || (found = VocClass::From_Name(name)) == VOC_NONE) { // RA2: as above
			found = sound; // RA2: as above
		}
		AmbientSound = (VocType)found; // RA2: as above
		IsCrushable = ini.Get_Bool(IniName, "Crushable", IsCrushable);
		IsBombable = ini.Get_Bool(IniName, "Bombable", IsBombable); // RA2: as above
		NoSpawnAlt = ini.Get_Bool(IniName, "NoSpawnAlt", NoSpawnAlt); // RA2: as above
		AlternateArcticArt = ini.Get_Bool(IniName, "AlternateArcticArt", AlternateArcticArt); // RA2: as above
		IsStealthy = ini.Get_Bool(IniName, "RadarInvisible", IsStealthy);
		IsSelectable = ini.Get_Bool(IniName, "Selectable", IsSelectable);
		IsLegalTarget = ini.Get_Bool(IniName, "LegalTarget", IsLegalTarget);
		Armor = ini.Get_ArmorType(IniName, "Armor", (ArmorType)Armor); // RA2: as above
		MaxStrength = ini.Get_Int(IniName, "Strength", MaxStrength);
		IsImmune = ini.Get_Bool(IniName, "Immune", IsImmune);
		IsInsignificant = ini.Get_Bool(IniName, "Insignificant", IsInsignificant);
		IsHasRadialIndicator = ini.Get_Bool(IniName, "HasRadialIndicator", IsHasRadialIndicator);
		RadialColor = ini.Get_RGBClass(IniName, "RadialColor", RadialColor);
		IsIgnoresFirestorm = ini.Get_Bool(IniName, "IgnoresFirestorm", IsIgnoresFirestorm);

		UseLineTrail = ArtINI.Get_Bool(GraphicName, "UseLineTrail", UseLineTrail); // RA2: as above
		LineTrailColor = ArtINI.Get_RGBClass(GraphicName, "LineTrailColor", LineTrailColor); // RA2: as above
		LineTrailColorDecrement = ArtINI.Get_Int(GraphicName, "LineTrailColorDecrement", LineTrailColorDecrement); // RA2: as above
		IsTheater = ArtINI.Get_Bool(GraphicName, "Theater", IsTheater);
		IsNewTheater = ArtINI.Get_Bool(GraphicName, "NewTheater", IsNewTheater);
		IsVoxel = ArtINI.Get_Bool(GraphicName, "Voxel", IsVoxel);
		if (!IsVoxel) {
			Fetch_Normal_Image();
		}

		if (AlphaGraphicName[0] != 0) { // RA2: as above
			char filename[260]; // RA2: as above
			_makepath(filename, 0, 0, AlphaGraphicName, ".SHP"); // RA2: as above
			AlphaImageData = (const void *)MixFileClass::Retrieve(filename); // RA2: as above
		}
		return(true);
	}
	return(false);
}




/// <summary>
/// Converts an artwork name to the current theater's spelling.
/// This routine handles the new theater naming convention, where the second letter of a
/// structure's artwork name selects the temperate or the snow version of the art. Names
/// that do not follow the convention are left untouched.
/// </summary>
/// <param name="name">The artwork name to adjust in place.</param>
void Theater_Name_Fixup(char * name, TheaterType theater) // RA2: not in TibSun, at 0x005D7BD0
{
	if (theater != -1) { // RA2: as above
		char first = name[0];
		char second = name[1];
		if (first >= 'A' && first <= 'Z') { // RA2: as above
			first += 'a' - 'A'; // RA2: as above
		}
		if (second >= 'A' && second <= 'Z') { // RA2: as above
			second += 'a' - 'A'; // RA2: as above
		}
		if ((first == 'g' || first == 'n' || first == 'c') && (second == 'a' || second == 't')) { // RA2: as above
			name[1] = Theaters[theater].Letter[0]; // RA2: as above
		}
	}
}



/// <summary>
/// Puts the generic theater letter into an art name.
/// </summary>
void Generic_Theater_Name(char * name) // RA2: not in TibSun, at 0x005D7C20
{
	name[1] = 'G';
}


/// <summary>
/// Loads the object type from a save game stream.
/// This routine will discard whatever artwork this type was holding, read the persistent
/// members back through the base class, and then re-attach the artwork that the restored
/// data calls for. The voxel libraries are deliberately left empty here, since they are
/// fetched again on demand.
/// </summary>
/// <returns>Returns with S_OK if the object type was restored, otherwise the error
/// result from the base class.</returns>
HRESULT STDMETHODCALLTYPE ObjectTypeClass::Load(IStream *stream)
{
	char filename[260];

	VoxelIndex.Clear(); // RA2: the four index tables are destroyed in place (the first Clear is expanded, the rest are calls to 0x5D8070)
	AuxVoxelIndex.~VoxelIndexClass(); // RA2: as above
	ShadowVoxelIndex.~VoxelIndexClass(); // RA2: as above
	AuxVoxel2Index.~VoxelIndexClass(); // RA2: as above

	delete Voxel.VoxLib;
	Voxel.VoxLib = NULL;

	delete Voxel.MotLib;
	Voxel.MotLib = NULL;

	delete AuxVoxel.VoxLib;
	AuxVoxel.VoxLib = NULL;

	delete AuxVoxel.MotLib;
	AuxVoxel.MotLib = NULL;

	delete AuxVoxel2.VoxLib;
	AuxVoxel2.VoxLib = NULL;

	delete AuxVoxel2.MotLib;
	AuxVoxel2.MotLib = NULL;

	Free_Charger_Voxels(); // RA2: 0x5D66A0 frees the charger voxels as well

	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		ImageData = NULL;
		ImageAllocated = false; // RA2: the flag at +0xA8 is cleared too
		AlphaImageData = NULL;

		new (&VoxelIndex) VoxelIndexClass;
		new (&AuxVoxelIndex) VoxelIndexClass;
		new (&ShadowVoxelIndex) VoxelIndexClass;
		new (&AuxVoxel2Index) VoxelIndexClass;

		Voxel.VoxLib = NULL;
		Voxel.MotLib = NULL;
		AuxVoxel.VoxLib = NULL;
		AuxVoxel.MotLib = NULL;
		AuxVoxel2.VoxLib = NULL;
		AuxVoxel2.MotLib = NULL;
		for (int index = 0; index < ARRAY_SIZE(ChargerTurrets); index++) { // RA2: the charger voxel pointers are cleared as well
			ChargerTurrets[index].VoxLib = NULL; // RA2: as above
			ChargerTurrets[index].MotLib = NULL; // RA2: as above
			ChargerBarrels[index].VoxLib = NULL; // RA2: as above
			ChargerBarrels[index].MotLib = NULL; // RA2: as above
		}

		if (*AlphaGraphicName != NULL) {
			_makepath(filename, NULL, NULL, AlphaGraphicName, ".SHP");
			AlphaImageData = MFCD::Retrieve(filename);
		}

		result = S_OK;
	}
	return(result);
}



/// <summary>
/// Saves the object type to a save game stream.
/// </summary>
/// <param name="cleardirty">Should the object type be marked clean once it is written?</param>
/// <returns>Returns with S_OK if the object type was written, otherwise the error result
/// from the base class.</returns>
HRESULT STDMETHODCALLTYPE ObjectTypeClass::Save(IStream *stream, int cleardirty)
{
	HRESULT result = BASECLASS::Save(stream, cleardirty);
	if (SUCCEEDED(result)) {

		result = S_OK;
	}
	return(result);
}


/// <summary>
/// Fetches the largest size this object type could need in a save game stream.
/// </summary>
/// <param name="pcbSize">Pointer to the value to fill in with the size required.</param>
/// <returns>Returns with S_OK if the size was reported, otherwise the error result from
/// the base class.</returns>
HRESULT STDMETHODCALLTYPE ObjectTypeClass::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
	HRESULT res = BASECLASS::GetSizeMax(pcbSize);
	if (SUCCEEDED(res)) {
		res = S_OK;
	}
	return(res);
}


/***********************************************************************************************
 * ObjectTypeClass::From_Name -- Converts an ASCII name into an object type pointer.           *
 *                                                                                             *
 *    This routine is used to convert an ASCII representation of an object into the            *
 *    matching object type pointer. This is used by the scenario INI reader code.              *
 *                                                                                             *
 * INPUT:   name  -- Pointer to ASCII name to translate.                                       *
 *                                                                                             *
 * OUTPUT:  Returns the object type poiner that matches the ASCII name provided. If no         *
 *          match could be found, then NULL is returned.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass const * ObjectTypeClass::From_Name(char const * name)
{
	for (int classid = 0; classid < ObjectTypes.Count(); classid++) {
		if (stricmp(ObjectTypes[classid]->IniName, name) == 0) {
			return(ObjectTypes[classid]);
		}
	}
	return(NULL);
}


/// <summary>
/// Discards the cached voxel renderings of every object type.
/// This routine is used by the voxel drawing code when the shared render buffer can no
/// longer take another entry. Throwing away every type's cached images frees the buffer
/// so that drawing can carry on.
/// </summary>
void ObjectTypeClass::Clear_Voxel_Indexes(void)
{
	for (int classid = 0; classid < ObjectTypes.Count(); classid++) {
		ObjectTypes[classid]->Clear_Voxel_Index();
	}

	VoxelStaticBuffer.Reset();
}


/// <summary>
/// Converts an artwork name to the current theater's spelling.
/// This routine handles the new theater naming convention, where the second letter of a
/// structure's artwork name selects the temperate or the snow version of the art. Names
/// that do not follow the convention are left untouched.
/// </summary>
/// <param name="name">The artwork name to adjust in place.</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void ObjectTypeClass::Theater_Naming_Convention(char * name, TheaterType theater) const // RA2: defined here
{
	char prefix[4];

	if (theater != THEATER_NONE) {
		strncpy(prefix, name, 2);
		prefix[2] = 0;
		if (stricmp(prefix, "ga") == 0 ||
			stricmp(prefix, "na") == 0 ||
			stricmp(prefix, "gt") == 0 ||
			stricmp(prefix, "nt") == 0 ||
			stricmp(prefix, "ca") == 0 ||
			stricmp(prefix, "ct") == 0)
		{
			switch (theater) {
				case THEATER_TEMPERATE:
					name[1] = 'T';
					break;
				case THEATER_SNOW:
					name[1] = 'A';
					break;
				default:
					break;
			}
		}
	}
}
#endif

