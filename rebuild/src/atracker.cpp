/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the tracker of the aircraft in flight: the map is divided into 20 by 20 regions (bins) and each
**	flying object is filed under the region of its cell, so that the objects near a cell are found without
**	walking the whole object list. TibSun has no such file; the code is the executable's from 0x004123E0.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include "swizzle.h" // RA2: the includes this file needs
#include "_map.h" // RA2: the includes this file needs
#include "cell.h" // RA2: the includes this file needs
#include "foot.h" // RA2: the includes this file needs
#include "targetcell.h" // RA2: Append_Radius_Bins takes the center cell through Target_Cell (ra2-decomp)
#include "tracker.h" // RA2: the includes this file needs


inline AircraftTrackerClass::~AircraftTrackerClass(void) // RA2: the exit-time destructor at 0x00412650 empties the bins first
{
	Clear();
}

AircraftTrackerClass AircraftTracker; // RA2: 0x00839EA0; 400 bins and the working list (its initializer is the one at 0x004125CE)


/// <summary>
/// Passes a text to the debug output; nothing calls it.
/// </summary>
void __fastcall Tracker_Debug_String(char const * text) // RA2: 0x00412710
{
	DebugString(text);
}

/// <summary>
/// Adds the objects of one bin to the working list.
/// </summary>
void AircraftTrackerClass::Append_Bin(int index) // RA2: 0x00412720
{
	DynamicVectorClass<TechnoClass *> & bin = TrackerVectors[index]; // RA2: the bin to copy
	int count = bin.Count();
	for (int i = 0; i < count; i++) {
		CurrentVector.Add(bin.GetItem(i)); // RA2: the element is fetched through the vector's virtual at +0x18 (YR name GetItem)
	}
}

/// <summary>
/// Gives the bin an offset away from a bin on the 20 by 20 grid, kept inside the grid.
/// </summary>
int AircraftTrackerClass::Shift_Bin(int index, Point2D offset) // RA2: 0x004127A0
{
	int x = index % 20;
	int y = index / 20;
	x = MIN(19, MAX(0, x + offset.X)); // RA2: clamped to the grid
	y = MIN(19, MAX(0, y + offset.Y)); // RA2: as above
	return x + y * 20;
}

/// <summary>
/// Gives the bin of a cell: the map rectangle divided into 20 by 20 regions.
/// </summary>
int AircraftTrackerClass::Region_Index(Cell cell) // RA2: 0x00412820
{
	int x = (cell.X < 0) ? 0 : cell.X;
	int y = (cell.Y < 0) ? 0 : cell.Y;
	int qx = x / (Map.MapRect.Width / 20);
	int cx = (qx > 19) ? 19 : qx;
	int qy = y / (Map.MapRect.Height / 20);
	int cy = (qy > 19) ? 19 : qy;
	return cx + cy * 20;
}

/// <summary>
/// Adds the bins around the cell of an object to the working list: its own bin, the bins one radius
/// away to the right, above, below and to the left with those between, then the diagonals.
/// </summary>
void AircraftTrackerClass::Append_Radius_Bins(CellClass * cell, int radius) // RA2: 0x004128A0
{
	Cell center = Target_Cell(cell); // RA2: the cell of the center coordinate, through Target_Cell as in ra2-decomp's twin
	int index = Region_Index(center);
	Append_Bin(index);

	int bin;
	int up = 0;
	int down = 0;
	int right = 0;
	int left = 0;

	Cell edge = center;
	edge.X += radius;
	for (bin = Region_Index(edge); bin != index; bin = Shift_Bin(bin, Point2D(-1, 0))) {
		right++;
		Append_Bin(bin);
	}

	edge = center;
	edge.Y -= radius;
	for (bin = Region_Index(edge); bin != index; bin = Shift_Bin(bin, Point2D(0, 1))) {
		up++;
		Append_Bin(bin);
	}

	edge = center;
	edge.Y += radius;
	for (bin = Region_Index(edge); bin != index; bin = Shift_Bin(bin, Point2D(0, -1))) {
		down++;
		Append_Bin(bin);
	}

	edge = center;
	edge.X -= radius;
	for (bin = Region_Index(edge); bin != index; bin = Shift_Bin(bin, Point2D(1, 0))) {
		left++;
		Append_Bin(bin);
	}

	if (up && left) {
		for (int i = MIN(up, left), bin = index; i > 0; i--) {
			bin = Shift_Bin(bin, Point2D(-1, -1));
			Append_Bin(bin);
		}
		if (up >= 2 && left >= 2) {
			Append_Bin(Shift_Bin(index, Point2D(-2, -1)));
			Append_Bin(Shift_Bin(index, Point2D(-1, -2)));
		}
	}

	if (down && left) {
		for (int i = MIN(down, left), bin = index; i > 0; i--) {
			bin = Shift_Bin(bin, Point2D(-1, 1));
			Append_Bin(bin);
		}
		if (down >= 2 && left >= 2) {
			int side;
			side = Shift_Bin(index, Point2D(-2, 1));
			Append_Bin(side);
			side = Shift_Bin(index, Point2D(-1, 2));
			Append_Bin(side);
		}
	}

	if (up && right) {
		for (int i = MIN(up, right), bin = index; i > 0; i--) {
			bin = Shift_Bin(bin, Point2D(1, -1));
			Append_Bin(bin);
		}
		if (up >= 2 && right >= 2) {
			int side;
			side = Shift_Bin(index, Point2D(2, -1));
			Append_Bin(side);
			side = Shift_Bin(index, Point2D(1, -2));
			Append_Bin(side);
		}
	}

	if (down && right) {
		for (int i = MIN(down, right), bin = index; i > 0; i--) {
			bin = Shift_Bin(bin, Point2D(1, 1));
			Append_Bin(bin);
		}
		if (down >= 2 && right >= 2) {
			int side;
			side = Shift_Bin(index, Point2D(2, 1));
			Append_Bin(side);
			side = Shift_Bin(index, Point2D(1, 2));
			Append_Bin(side);
		}
	}
}

/// <summary>
/// Does nothing: the tracker needs no work when an object it may hold goes away.
/// </summary>
void AircraftTrackerClass::Detach(AbstractClass * target) // RA2: 0x004131D0
{
}

/// <summary>
/// Files an object under the bin of its cell and remembers that cell as its last one.
/// </summary>
void AircraftTrackerClass::Add(TechnoClass * object) // RA2: 0x004131E0
{
	Cell cell = object->Get_Cell();
	object->SetLastFlightMapCoords(cell);
	TrackerVectors[Region_Index(cell)].Add(object);
}

/// <summary>
/// Gives the number 750; nothing calls it, and the executable shows no use of the number here.
/// </summary>
unsigned int Tracker_Return_750(void) // RA2: 0x004132D0
{
	return 750;
}

/// <summary>
/// Tells whether another object holds the jumpjet place of the cell of the object.
/// </summary>
bool AircraftTrackerClass::IsJumpjet(TechnoClass * object) // RA2: 0x004132E0
{
	FootClass * occupier = object->Get_Cell_Ptr()->Jumpjet;
	if (occupier != NULL && occupier != object) {
		return true;
	}
	return false;
}

/// <summary>
/// Takes an object out of the bin of its last cell and forgets that cell.
/// </summary>
void AircraftTrackerClass::Remove(TechnoClass * object) // RA2: 0x00413310
{
	Cell cell = object->GetLastFlightMapCoords();
	object->SetLastFlightMapCoords(CELL_NONE);
	TrackerVectors[Region_Index(cell)].Delete(object);
}

/// <summary>
/// Reads every bin from a stream: the count, then the object pointers, which are swizzled afterwards.
/// </summary>
HRESULT AircraftTrackerClass::Load(IStream * stream) // RA2: 0x00413400
{
	int j;
	for (int i = 0; i < 400; i++) {
		int count;
		HRESULT result = stream->Read(&count, sizeof(count), NULL);
		if (FAILED(result)) return result;
		for (j = 0; j < count; j++) {
			TechnoClass * object;
			result = stream->Read(&object, sizeof(object), NULL);
			if (FAILED(result)) return result;
			TrackerVectors[i].Add(object);
		}
		for (j = 0; j < count; j++) {
			Swizzler.Swizzle((void **)&TrackerVectors[i][j]); // RA2: the pointer is resolved when the whole save is loaded
		}
	}
	return S_OK;
}

/// <summary>
/// Takes the first object off the working list; none when it is empty.
/// </summary>
FootClass * AircraftTrackerClass::Get(void) // RA2: 0x004134E0
{
	if (CurrentVector.Count() == 0) return NULL;
	TechnoClass * object = CurrentVector[0];
	CurrentVector.Delete(object);
	return (FootClass *)object;
}

/// <summary>
/// Empties every bin and the working list.
/// </summary>
void AircraftTrackerClass::Clear(void) // RA2: 0x00413540
{
	for (int i = 0; i < 400; i++) {
		TrackerVectors[i].Delete_All();
	}
	CurrentVector.Delete_All();
}

/// <summary>
/// Writes every bin to a stream: the count, then the object pointers.
/// </summary>
HRESULT AircraftTrackerClass::Save(IStream * stream) // RA2: 0x00413590
{
	int j;
	for (int i = 0; i < 400; i++) {
		int count = TrackerVectors[i].Count();
		HRESULT result = stream->Write(&count, sizeof(count), NULL);
		if (FAILED(result)) return result;
		for (j = 0; j < count; j++) {
			result = stream->Write(&TrackerVectors[i][j], sizeof(TechnoClass *), NULL);
			if (FAILED(result)) return result;
		}
	}
	return S_OK;
}

/// <summary>
/// Remembers the new cell of an object and moves it to the bin of that cell when the bin changed.
/// </summary>
void AircraftTrackerClass::Update(TechnoClass * object, Cell oldcell, Cell newcell) // RA2: 0x00413600
{
	object->SetLastFlightMapCoords(newcell);
	int oldindex = Region_Index(oldcell);
	int newindex = Region_Index(newcell);
	if (oldindex != newindex) {
		TrackerVectors[oldindex].Delete(object);
		TrackerVectors[newindex].Add(object);
	}
}
