
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/cell.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

Coord CellClass::Closest_Free_Spot(Coord const & xcoord, bool any, bool bridge, bool use_cell_coord) const
{
	int spot_index = Spot_Index(xcoord);

	/*
	**	This precalculated sequence table records the closest spots to any given spot. Sequential
	**	examination of these spots for availability ensures that the closest available one is
	**	discovered first.
	*/
	static unsigned char _sequence[5][4] = {
		{1,2,3,4},
		{0,2,3,4},
		{0,1,4,3},
		{0,1,4,2},
		{0,2,3,1}
	};

	/*
	**	In the case of the center coordinate being requested, but is occupied, then all other
	**	sublocations are equidistant. Instead of picking a static sequence of examination, the
	**	order is mixed up by way of this table.
	*/
	static unsigned char _alternate[4][4] = {
		{1,2,3,4},
		{2,3,4,1},
		{3,4,1,2},
		{4,1,2,3},
	};

	Coord coord = Coord_Whole(use_cell_coord ? Center_Coord() : xcoord);	// NATIVE: YR's fourth flag (gamemd 0x00481180) starts from the cell's centre

	/*
	**	Cells occupied by buildings or vehicles don't have any free spots.
	*/

	bool is_vehicle_occupied;
	if (bridge) {
		is_vehicle_occupied = BridgeFlag.Occupy.Vehicle;
	} else {
		is_vehicle_occupied = Flag.Occupy.Vehicle;
	}

	if (!any) {
		if (is_vehicle_occupied || (Flag.Occupy.Monolith && (!Get_Gate() || !Get_Gate()->Is_Gate_Open()))) {
			return(COORD_NONE);
		}

	}

	/*
	**	If just the nearest position is desired regardless of whether occupied or not,
	**	then just return with the stopping coordinate value.
	*/
	if (any || Is_Spot_Free(spot_index, bridge)) {
		coord += StoppingCoordAbs[spot_index];
		coord.Z = Map.Get_Height_GL(xcoord);
		if (bridge) {
			coord.Z += BRIDGE_LEPTON_HEIGHT;
		}
		return(coord);
	}

	/*
	**	Scan through all available sub-locations in the cell in order to determine
	**	the closest one to the coordinate requested. Use precalculated table so that
	**	when the first free position is found, bail.
	*/
	unsigned char * sequence;
	if (spot_index == 0) {
		sequence = &_alternate[Random_Pick(0, 3)][0];
	} else {
		sequence = &_sequence[spot_index][0];
	}
	for (int index = 0; index < 4; index++) {
		int pos = *sequence++;

		if (Is_Spot_Free(pos, bridge)) {
			coord += StoppingCoordAbs[pos];
			coord.Z = Map.Get_Height_GL(xcoord);
			if (bridge) {
				coord.Z += BRIDGE_LEPTON_HEIGHT;
			}
			return(coord);
		}
	}

	/*
	**	No free spot could be found so return a NULL coordinate.
	*/
	return(COORD_NONE);
}

// NON-EXACT NATIVE DRAFT (lane Y8): rewritten from gamemd.exe 0x0047EFE0. FileSystem::LoadFile
// returns an SHPReference for a .SHP name (gamemd 0x005B40B0), not the shape file itself, so the
// frame's rectangle and pixels come through SHPStruct::GetFrameBounds (0x0069E7E0) and GetPixels
// (0x0069E740), which load the file behind the reference. The lane Y3 draft (TibSun's text) read
// the reference object as a ShapeSet: its rectangles came from the reference's own pointers and
// heap bytes past it, and its pixel pointer was that object plus a garbage offset (the SIGBUS at a
// game's start, 1 run in 10 on the iOS Simulator; the shroud's edge shapes were never drawn).
// Order of calls and the two loops as gamemd.exe: the ring-wrapping path tests before each row
// and pixel, the path that fits before the ring's end draws in do-while loops.
void CellClass::Draw_Shroud_Or_Fog_Shape(Point2D const & drawpoint, Rect const & cliprect, int shapenum)
{
	static bool shapes_loaded = false;
	static SHPStruct * shadow_shapes = NULL;
	static SHPStruct * fog_shapes = NULL;

	if (!shapes_loaded) {
		shapes_loaded = true;
		shadow_shapes = (SHPStruct *)FileSystem::LoadFile("SHROUD.SHP", false);
		fog_shapes = (SHPStruct *)FileSystem::LoadFile("FOG.SHP", false);
	}

	SHPStruct * shapes;
	if (Scen->Special.IsFogOfWar) {
		shapes = fog_shapes;
	} else {
		shapes = shadow_shapes;
	}
	if (shapes == NULL) return;	// NATIVE: gamemd.exe reads through a missing file's NULL

	Rect shaperect = shapes->GetFrameBounds(shapenum);

	if (drawpoint.Y + shaperect.Y + shaperect.Height < cliprect.Y + 1) return;
	if (drawpoint.Y >= cliprect.Height + cliprect.Y) return;
	if (drawpoint.X + shaperect.X + shaperect.Width < cliprect.X + 1) return;
	if (drawpoint.X >= cliprect.X + cliprect.Width) return;

	int inter_top = MAX(cliprect.Y, drawpoint.Y + shaperect.Y);
	int src_y = inter_top - drawpoint.Y - shaperect.Y;
	int inter_bottom = MIN(cliprect.Y + cliprect.Height, drawpoint.Y + shaperect.Y + shaperect.Height);

	int inter_left = MAX(cliprect.X, drawpoint.X + shaperect.X);
	int src_x = inter_left - drawpoint.X - shaperect.X;
	int inter_right = MIN(drawpoint.X + shaperect.X + shaperect.Width, cliprect.X + cliprect.Width);

	int shape_skip = drawpoint.X - inter_right + shaperect.X + shaperect.Width + src_x;
	int alpha_skip = inter_left - inter_right + AlphaBuffer->Get_Buffer_Width();

	unsigned char * shapedata = shapes->GetPixels(shapenum);
	if (shapedata == NULL) return;	// NATIVE: gamemd.exe would read from address 0 (a frame without pixels)
	unsigned short * alphaptr = (unsigned short *)AlphaBuffer->Get_Buffer_Offset(Point2D(inter_left, inter_top - TacticalRect.Y));

	unsigned char * shapeptr = &shapedata[src_x + src_y * shaperect.Width];
	int rows = inter_bottom - inter_top;
	int cols = inter_right - inter_left;
	if ((uintptr_t)&alphaptr[cols + rows * AlphaBuffer->Get_Buffer_Width() + 2] >= AlphaBuffer->Get_Buffer_End()) {
		for (int i = inter_top; i < inter_bottom; i++) {
			for (int j = inter_left; j < inter_right; j++) {
				unsigned char pixel = *shapeptr++;
				if (pixel != 0xFE) {
					*alphaptr = pixel;
				}
				alphaptr++;
				alphaptr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)alphaptr);
			}
			shapeptr += shape_skip;
			alphaptr += alpha_skip;
			alphaptr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)alphaptr);
		}
	} else {
		do {
			int c = cols;
			do {
				unsigned char pixel = *shapeptr++;
				if (pixel != 0xFE) {
					*alphaptr = pixel;
				}
				++alphaptr;
			} while (--c > 0);
			shapeptr += shape_skip;
			alphaptr += alpha_skip;
		} while (--rows > 0);
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4): CellClass::Reduce_Tiberium, unmatched in ra2-decomp. TibSun's
// text (TibSun archive code/cell.cpp; same licence notice as above). Behaviour code for the native
// build only; gamemd.exe's version is not compared yet.
// ---------------------------------------------------------------------------------------------
int CellClass::Reduce_Tiberium(int levels)
{
	Rect dirty = Union(Overlay_Render_Rect(), Overlay_Shadow_Render_Rect());
	dirty.Y -= TacticalRect.Y;

	TiberiumType tibtype = Tiberium_Type_Here();
	int reducer = levels;

	if (levels > 0 && tibtype != TIBERIUM_NONE) {
		TiberiumClass * tiberium = Tiberiums[tibtype];
		if (OverlayData == OVERLAYDATA_TIBERIUM_MAX) {
			tiberium->Queue_Growth(CellID);
		}
		if (OverlayData+1 > levels) {
			OverlayData -= levels;
			reducer = levels;
		} else {
			PassabilityType passability = Passability;
			Overlay = OVERLAY_NONE;
			reducer = OverlayData;
			OverlayData = 0;
			Recalc_Attributes();
			if (passability != Passability) {
				Map.Update_Cell_Zone(CellID);
				Map.Update_Cell_Subzones(CellID);
			}
			Map.Flag_Background_Update(CellID);
			tiberium->Clear_Spread_State(CellID);
			for (int facing = FACING_FIRST; facing < FACING_COUNT; facing++) {
				Cell adjacent = (Cell)::Adjacent_Cell(CellID, (FacingType)facing);
				if (Map.In_Radar(adjacent) && !tiberium->SpreadState[Map_Cell_Index(adjacent)]) {
					tiberium->Queue_Spread(adjacent);
				}
			}
		}
		TacticalMap->Register_Dirty_Area(dirty);
		return(reducer);
	}
	return(0);
}
