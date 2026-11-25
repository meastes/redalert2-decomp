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

#pragma once

#include "vector.h" // RA2: the bins are dynamic vectors
#include "point.h" // RA2: Shift_Bin takes a Point2D

class AbstractClass;
template<class T> class DynamicVectorClass;

void Detach_This_From_All(AbstractClass const * target, bool all = true);
void Process_Deferred_Deletion(void);

/*
 * These trackers exist for speed optimization reasons.
 * For example:
 * BuildingClass and HouseClass have a pointer to FactoryClass.
 * Their constructors add them to FactoryPtrTracker.
 * If you pass a FactoryClass pointer to Detach_This_From_All,
 * it will Detach that pointer from BuildingClass and HouseClass instances
 * tracked by FactoryPtrTracker.
 */
extern DynamicVectorClass<AbstractClass *> ObjectPtrTracker;
extern DynamicVectorClass<AbstractClass *> AbstractTypePtrTracker;
extern DynamicVectorClass<AbstractClass *> TagPtrTracker;
extern DynamicVectorClass<AbstractClass *> TriggerPtrTracker;
extern DynamicVectorClass<AbstractClass *> AnimPtrTracker;
extern DynamicVectorClass<AbstractClass *> FactoryPtrTracker;
extern DynamicVectorClass<AbstractClass *> EventActionPtrTracker;
extern DynamicVectorClass<AbstractClass *> WaypointPathPtrTracker;
extern DynamicVectorClass<AbstractClass *> TeamPtrTracker;
extern DynamicVectorClass<AbstractClass *> HousePtrTracker;
extern DynamicVectorClass<AbstractClass *> NeuronPtrTracker;

/*
 * This vector holds all objects marked by ObjectClass::Delete_Me.
 */
extern DynamicVectorClass<AbstractClass *> ObjectsToDelete;

/*
**	RA2: the tracker of the aircraft in flight, and the selection list of the user
**	interface commands; both forget an object when it goes away.
*/
class FootClass;
class TechnoClass; // RA2: the bins hold techno pointers
class CellClass; // RA2: Append_Radius_Bins takes the cell whose surroundings are wanted
class Cell; // RA2: AircraftTrackerClass::Update takes cells by value
struct IStream; // RA2: Load and Save take a stream
typedef long HRESULT; // RA2: the result type of Load and Save, for files that do not see the Windows headers

class AircraftTrackerClass
{
	public:
		~AircraftTrackerClass(void); // RA2: no code of its own at an address; the exit-time destructor at 0x412650 has it inlined (YR source: it calls Clear)
		void Append_Bin(int index); // RA2: 0x412720 (name proposed from the code); adds the objects of one bin to the working list
		int Shift_Bin(int index, Point2D offset); // RA2: 0x4127A0 (name proposed from the code); the bin an offset away on the 20 by 20 grid, clamped at its edges
		int Region_Index(Cell cell); // RA2: 0x412820 (name proposed from the code); the bin of a cell: the map divided into 20 by 20 regions
		void Append_Radius_Bins(CellClass * cell, int radius); // RA2: 0x4128A0 (name proposed from the code); adds the bins around the object's cell to the working list
		void Detach(AbstractClass * target);
		void Add(TechnoClass * object); // RA2: 0x4131E0 (YR name); files the object under its cell
		bool IsJumpjet(TechnoClass * object); // RA2: 0x4132E0 (YR name); whether another object holds the jumpjet place of the cell
		void Remove(TechnoClass * object); // RA2: 0x413310 (YR name)
		HRESULT Load(IStream * stream); // RA2: 0x413400 (YR name)
		FootClass * Get(void); // RA2: 0x4134E0 (YR name); takes the first object off the working list
		void Clear(void); // RA2: 0x413540 (YR name)
		HRESULT Save(IStream * stream); // RA2: 0x413590 (YR name)
		void Update(TechnoClass * object, Cell oldcell, Cell newcell); // RA2: 0x413600 (YR name); moves the object's filing from one cell to the next

		DynamicVectorClass<TechnoClass *> TrackerVectors[400]; // RA2: +0x0000; the objects of each region, indexed x + 20 * y (meastes: Bins)
		DynamicVectorClass<TechnoClass *> CurrentVector; // RA2: +0x2580; the working list Append_Radius_Bins fills and Get empties
};

extern AircraftTrackerClass AircraftTracker;
void Detach_From_Command_List(AbstractClass * object);
