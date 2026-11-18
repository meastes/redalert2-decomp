
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/techno.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

void TechnoClass::Techno_Draw_Object(ShapeSet const * shapefile, int shapenum, Point2D const & xdrawpoint, Rect const & rect, Dir256 rotation, int scale, int xzadjust, ZGradientType zgrad, bool zwrite, int brightness, unsigned short tint, ShapeSet const * zshapefile, int zshapenum, Point2D zoff, ShapeFlags_Type negflags) const
{
	if (shapefile != NULL) {
		bool shadow = DrawShapeShadows;
		VisualType visual = Visual_Character();

		ShapeFlags_Type flags = SHAPE_NORMAL;
		if (negflags == SHAPE_NORMAL) {
			bool visible = false;
			bool shadowy = true;
			switch (visual) {
				case VISUAL_DARKEN:
				case VISUAL_SHADOWY:
					visible = true;
					shadowy = false;
					break;

				case VISUAL_HIDDEN:
					return;

				case VISUAL_RIPPLE:
					visible = true;
					if (CloakingDevice.Fetch_Stage()) {
						shadowy = false;
					}
					break;

				case VISUAL_INDISTINCT:
					visible = true;
					break;
			}
			if (visible) {
				if (shadowy) {
					flags = ShapeFlags_Type(flags|SHAPE_TRANSLUCENT25);
				} else {
					flags = ShapeFlags_Type(flags|SHAPE_TRANSLUCENT50);
				}
			}
		}

		ConvertClass * converter;
		if (RTTI == RTTI_BUILDING && ((BuildingClass *)this)->Class->IsTerrainPalette) {
			CellClass * cptr = &Map[Center_Coord().As_Cell()];
			converter = cptr->Drawer;
			if (converter == NULL) {
				cptr->Init_Drawer();
				converter = cptr->Drawer;
				if (converter == NULL) return;
			}
			brightness = cptr->TileBrightness;
		} else {
			// TibSun's text: a vehicle compositing on the eight-bit surface draws through EightBitDrawer
			// (YR's tree calls the flag TerrainPalette; lane Y14: the draft had dropped it).
			if (RTTI == RTTI_UNIT && ((UnitClass const *)this)->TerrainPalette) {
				converter = EightBitDrawer;
			} else {
				converter = ColorSchemes[House->Scheme]->Converter;
			}
		}

		Point2D drawpoint = xdrawpoint;
		int zadjust = xzadjust;

		switch ((RTTIType)RTTI) {
			case RTTI_UNIT:
				if (((UnitClass *)this)->Class->SmallVisceroid || ((UnitClass *)this)->Class->LargeVisceroid) {
					shadow = false;
				}
				if (HeightAGL == 0) {
					zadjust += Get_Z_Adjust();
				} else {
					zadjust -= TacticalMap->Z_Lepton_To_Pixel(Height);
				}
				break;

			case RTTI_AIRCRAFT:
				drawpoint.Y -= TacticalMap->Z_Lepton_To_Pixel(Height);
				zadjust -= TacticalMap->Z_Lepton_To_Pixel(Height);
				break;

			case RTTI_INFANTRY:
				if (HeightAGL == 0) {
					zadjust += Get_Z_Adjust();
				} else {
					zadjust -= TacticalMap->Z_Lepton_To_Pixel(Height);
					shadow = false;
				}
				break;
		}

		// NATIVE: gamemd.exe picks a disguised object's colours through its disguise (YR 0x00706061..0x00706129); not written.

		if (zgrad != ZGRAD_NONE) flags = ShapeFlags_Type(flags|SHAPE_ZGRAD);
		if (zwrite) flags = ShapeFlags_Type(flags|SHAPE_ZWRITE);
		flags = ShapeFlags_Type(flags|SHAPE_ALPHA);

		flags = ShapeFlags_Type(flags & ~negflags);


		brightness = Apparent_Brightness(brightness);

		switch (visual) {
			case VISUAL_NORMAL:
				Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, zgrad, brightness, tint, zshapefile, zshapenum, zoff);
				if (shadow) {
					zadjust = -2 - TacticalMap->Z_Lepton_To_Pixel(Height);
					if (IsInTransport) {
						drawpoint.Y -= 14;
					}
					Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum + shapefile->Get_Count() / 2, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_DARKEN|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2);
				}
				break;

			case VISUAL_INDISTINCT:
				Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, zgrad, brightness, tint, zshapefile, zshapenum, zoff);
				if (DrawShapeShadows) {
					zadjust = -2 - TacticalMap->Z_Lepton_To_Pixel(Height);
					flags = ShapeFlags_Type(flags & ~SHAPE_TRANSLUCENT75);
					if (IsInTransport) {
						drawpoint.Y -= 14;
					}
					Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum + shapefile->Get_Count() / 2, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_DARKEN|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2);
				}
				break;

			case VISUAL_DARKEN:
			case VISUAL_RIPPLE:
				Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, zgrad, brightness, tint, zshapefile, zshapenum, zoff);
				break;

			case VISUAL_SHADOWY:
				Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, zgrad, brightness, tint, zshapefile, zshapenum, zoff);
				break;
		}
	}
}

void TechnoClass::Draw_Pre_Render(Point2D const & point, Rect const & cliprect) const
{
	if (RTTI == RTTI_BUILDING && IsSelected && RTTI != RTTI_INFANTRY) {
		int color = WHITE;	// NATIVE: YR has no limpet colour (gamemd 0x006F60D0)
		if (HeightAGL < -4) {
			color = BLACK;
		}
		Coord dim = TClass->Lepton_Dimensions();
		dim.X /= 2;
		dim.Y /= 2;
		color = NormalDrawer->Convert_Pixel(color);
		Coord center = Center_Coord();
		Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, 0), center + Coord(-dim.X, -dim.Y, dim.Z), color);
		Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, 0), center + Coord(dim.X, -dim.Y, 0), color);
		Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, 0), center + Coord(-dim.X, dim.Y, 0), color);
		Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, dim.Z), center + Coord(-dim.X, dim.Y, dim.Z), color);
		Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, dim.Z), center + Coord(dim.X, -dim.Y, dim.Z), color);
	}
}

SurfaceRegion TechnoClass::Techno_Render_Voxel_Object(VoxelDataStruct const & voxeldata, Matrix3D const & matrix, Point2D const & point, Rect const & xcliprect, int frame, ShapeFlags_Type flags, int brightness, unsigned short tint) const
{
	Matrix3D mtx = matrix;
	Matrix3D mtx2 = matrix;

	VoxelLibrary * voxlib = voxeldata.VoxLib;
	MotionLibrary * motlib = voxeldata.MotLib;

	VoxelDrawSystem::Precalculate_Light(voxlib, 0, 0, mtx2, VoxelLightSource);
	VoxelDrawSystem::Reset();

	for (unsigned layer = 0; layer < voxlib->Get_Layer_Count(); layer++) {
		if (motlib != NULL) {
			Matrix3D mtx3 = motlib->Get_Layer_Matrix(layer, frame);
			mtx = mtx2 * mtx3;
		}
		VoxelDrawSystem::Prep_For_Object(voxlib, layer, 0, VoxelCameraMatrix * mtx);
	}

	SurfaceRegion region = VoxelDrawSystem::Render();
	Rect cliprect = xcliprect;

	if (LogicalSurface != EightBitSurface) {
		if (SinkingYOffset > 0) {
			cliprect = Intersect(cliprect, Rect(0, 0, TacticalRect.Width, SinkingYOffset - TacticalMap->TacPixelY));
		} else if (IsSinking) {
			((TechnoClass *)this)->Calculate_Sinking_Offset(region.Bounds.Height, point.Y);
		}
	}

	flags = ShapeFlags_Type(flags & ~SHAPE_REMAP);
	// A vehicle composites its voxels on the eight-bit surface (UnitClass::Unit_Draw_Voxel swaps
	// LogicalSurface for it and Unit_Blit_Voxel draws the result): drawn there through
	// EightBitDrawer, as Techno_Blit_Voxel and gamemd.exe 0x0070717D do. Lane Y14: the draft drew
	// through the house's 16-bit converter, writing 16-bit pixels into the 8-bit surface: every
	// vehicle's first picture at a new facing or frame (before the voxel cache held it) was noise
	// (the user's MCV deploy and a Grizzly's first move).
	ConvertClass * converter = NULL;
	if (RTTI == RTTI_UNIT && ((UnitClass const *)this)->TerrainPalette) {
		converter = EightBitDrawer;
	} else {
		converter = GetRemapColour();
	}

	int predoffset = 0;
	if (flags & SHAPE_PREDATOR) {
		predoffset = Get_Predator_Offset();
	}

	Blit_Block(*LogicalSurface, *converter, *VoxelDrawSystem::Get_Surface(), region.Bounds, region.Point + point, cliprect, NULL, converter->Blitter_From_Flags(flags), Get_Z_Adjust(), Get_Z_Gradient(), brightness, predoffset);

	return(region);
}

// NON-EXACT NATIVE DRAFT (lane Y6): TechnoClass::Draw_Health_Bar, gamemd.exe 0x006F64A0 (1,568 B,
// unmatched in ra2-decomp), written from gamemd.exe's code (it replaces lane Y3's TibSun text).
// YR draws from the shape files objtype.cpp loads (FileSystem::PIPS_SHP, PIPBRD_SHP); TibSun's
// ObjectTypeClass::SelectShapes and PipShapes are never loaded in YR, so the TibSun text drew no
// selection brackets. A building's pip bar runs along its foundation; infantry and vehicles get the
// selection bracket (pipbrd.shp frame 1 / 0, raised by the type's PixelSelectionBracketDelta)
// while selected and a pip bar of 8 / 17 pips (pips.shp 16 green, 17 yellow, 18 red). The
// building's cargo pips show to allies, spies, every player when the type says so, and on
// buildings that can be occupied. Behaviour code for the native build only.
namespace FileSystem {	// objtype.cpp's statics (struct FileSystem there; the same symbols)
	extern void const * PIPBRD_SHP;
	extern ShapeSet * PIPS_SHP;	// as techno.cpp declares it
}

void TechnoClass::Draw_Health_Bar(Point2D const & xpoint, Rect const & cliprect, bool) const
{
	ShapeSet const * pips = (ShapeSet const *)FileSystem::PIPS_SHP;
	ShapeSet const * brackets = (ShapeSet const *)FileSystem::PIPBRD_SHP;
	ShapeFlags_Type const flags = ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER);
	ShapeFlags_Type const bracket_flags = ShapeFlags_Type(SHAPE_ALPHA|SHAPE_WIN_REL|SHAPE_CENTER);

	if (RTTI == RTTI_BUILDING) {
		Point3D dim = TClass->Lepton_Dimensions();
		Point3D half(dim.X / 2, dim.Y / 2, dim.Z / 2);
		Coord coord(-(dim.X - half.X), dim.Y - half.Y, dim.Z);
		Point2D p0 = TacticalMap->Coord_To_Pixel_Absolute(coord);	// the top corner
		coord.Y = -coord.Y;
		Point2D p1 = TacticalMap->Coord_To_Pixel_Absolute(coord);	// the left corner
		coord.Z = 0;
		coord.Y = -coord.Y;
		Point2D p2 = TacticalMap->Coord_To_Pixel_Absolute(coord);	// the top corner on the ground

		int barlen = (p0.Y - p1.Y) / 2;
		int n = (int)(HealthRatio * (double)barlen);
		if (n <= 1) n = 1;
		if (n >= barlen) n = barlen;

		int condcolor = 1;
		if (Is_Yellow_HP()) {
			condcolor = 2;
		} else if (Is_Red_HP()) {
			condcolor = 4;
		}

		int ybase = 2 - 2 * barlen;
		Point2D point;
		int index;
		for (index = 0; index < n; index++) {
			point.X = xpoint.X + p0.X + 4 * barlen + 3 - 4 * index;
			point.Y = xpoint.Y + p0.Y + ybase + 2 + 2 * index;
			Draw_Shape(*LogicalSurface, *NormalDrawer, pips, condcolor, point, cliprect, flags);
		}
		for (index = n; index < barlen; index++) {
			point.X = xpoint.X + p0.X + 4 * barlen + 3 - 4 * index;
			point.Y = xpoint.Y + p0.Y + ybase + 2 + 2 * index;
			Draw_Shape(*LogicalSurface, *NormalDrawer, pips, 0, point, cliprect, flags);
		}

		bool show = House->Is_Ally(PlayerPtr) || (SpiedBy & (1 << PlayerPtr->Class->House));
		if (((BuildingClass const *)this)->Class->CanBeOccupied) show = true;
		if (TClass->PipsDrawForAll || show) {
			Draw_Pips(xpoint + p2, xpoint, cliprect);
		}
		return;
	}

	Point2D offset;
	int count;
	int delta = TClass->PixelSelectionBracketDelta;
	if (RTTI == RTTI_INFANTRY) {
		if (IsSelected) {
			Draw_Shape(*LogicalSurface, *NormalDrawer, brackets, 1, Point2D(xpoint.X + 11, xpoint.Y + delta - 25), cliprect, bracket_flags);
		}
		offset = Point2D(-5, delta - 24);
		count = 8;
	} else {
		if (IsSelected) {
			Draw_Shape(*LogicalSurface, *NormalDrawer, brackets, 0, Point2D(xpoint.X + 1, xpoint.Y + delta - 26), cliprect, bracket_flags);
		}
		offset = Point2D(-15, delta - 25);
		count = 17;
	}

	int n = (int)(HealthRatio * (double)count);
	if (n <= 1) n = 1;
	if (n >= count) n = count;

	int shapenum = 16;
	if (HealthRatio <= Rule->ConditionYellow) shapenum = 17;
	if (HealthRatio <= Rule->ConditionRed) shapenum = 18;

	for (int index = 0; index < n; index++) {
		Draw_Shape(*LogicalSurface, *NormalDrawer, pips, shapenum, Point2D(xpoint.X + offset.X + 2 * index, xpoint.Y + offset.Y), cliprect, flags);
	}

	if (House->Is_Ally(PlayerPtr) || (SpiedBy & (1 << PlayerPtr->Class->House))) {
		Draw_Pips(xpoint + Point2D(-10, +10), xpoint, cliprect);
	}
}

// NON-EXACT NATIVE DRAFT (lane Y4): TechnoClass::Can_Deploy_Now, YR 0x00700D50 (891 bytes,
// unmatched in ra2-decomp), written from gamemd.exe's code (it replaces lane Y3's TibSun text).
// After the tunnel test the answer depends on the object's kind: an infantry soldier deploys
// when its type is a deployer and its undeploy delay has run out; a building asks its type
// (BuildingTypeClass 0x00465D30); a vehicle may not deploy while under EMP or in a tube, with
// nothing to deploy into, while eaten by a parasite, or (a construction yard's vehicle) while
// mind controlled, and a transport not on a bridge, next to a bridge's underside, a human's on
// water, or in a building that is not a bunker; anything else deploys when it carries passengers.
// Behaviour code for the native build only.
bool TechnoClass::Can_Deploy_Now(void) const
{
	Cell cell = PositionCell;
	CellClass * cellptr = &Map[cell];
	if (cellptr != NULL && cellptr->Is_Near_Tunnel_NW()) {
		return(false);
	}

	switch (Fetch_RTTI()) {
		case RTTI_INFANTRY: {
			InfantryClass const * infantry = (InfantryClass const *)this;
			if (infantry->FUN_005224F0()) {
				if (infantry->FUN_00522510() || infantry->Doing == (DoType)31) {
					return(false);
				}
			}
			if (infantry->FUN_005224F0() && Arm != 0) {
				return(false);
			}
			return(infantry->FUN_005224E0());
		}

		case RTTI_BUILDING:
			return(((BuildingClass const *)this)->Class->FUN_00465D30());

		case RTTI_UNIT: {
			UnitClass const * unit = (UnitClass const *)this;
			if (unit->IsUnderEMP() || unit->CurrentTube >= 0) {
				return(false);
			}
			UnitTypeClass const * type = unit->Class;
			if (type->DeploysInto == NULL && type->MaxPassengers == 0 && !type->IsSimpleDeployer
				&& BunkerLinkedItem == NULL && !unit->FUN_00746D00()) {
				return(false);
			}
			if (type->DeploysInto != NULL) {
				if (unit->ParasiteEatingMe != NULL) {
					return(false);
				}
				if (type->DeploysInto->IsConstructionYard && MindControlledBy != NULL) {
					return(false);
				}
			}
			if (type->MaxPassengers <= 0) {
				return(true);
			}
			if (IsOnBridge) {
				return(false);
			}
			cellptr = &Map[cell];
			if (cellptr != NULL) {
				CellClass * cellptr_s = &Map[Adjacent_Cell(cell, FACING_S)];
				CellClass * cellptr_n = &Map[Adjacent_Cell(cell, FACING_N)];
				CellClass * cellptr_e = &Map[Adjacent_Cell(cell, FACING_E)];
				CellClass * cellptr_w = &Map[Adjacent_Cell(cell, FACING_W)];
				if (cellptr->IsUnderBridge ||
					(cellptr_s != NULL && cellptr_s->IsUnderBridge) ||
					(cellptr_n != NULL && cellptr_n->IsUnderBridge) ||
					(cellptr_e != NULL && cellptr_e->IsUnderBridge) ||
					(cellptr_w != NULL && cellptr_w->IsUnderBridge)) {
					return(false);
				}
				if (House->IsHuman && cellptr->Land_Type() == LAND_WATER) {
					return(false);
				}
			}
			BuildingClass * building = Get_Cell_Ptr()->Cell_Building();
			if (building != NULL && !building->Class->Bunker) {
				return(false);
			}
			return(true);
		}

		default:
			if (IsUnderEMP()) {
				return(false);
			}
			return(Techno_Type_Class()->Max_Passengers() != 0);
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y12): the rocking from an impact, TechnoClass vtable slot 0x3D8, gamemd.exe
// 0x0070B280 (752 bytes; the tree's vt_entry_3D8, unmatched: a link-closure trap that the area damage of
// a rocker warhead reached in the first Soviet mission; Explosion_Damage, BulletClass and the parasite
// call it). Red Alert 2's draft of the same function (src/recovered/techno.cpp, game.exe 0x006D5F40,
// TechnoClass slot 223) in the YR tree's names, with gamemd.exe's change: the third argument keeps the full
// forward rocking (false halves it, as Red Alert 2 always does). Only voxel units rock. Behaviour code for
// the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
void TechnoClass::vt_entry_3D8(Coord const & coord, float force, bool full)
{
	TechnoTypeClass const * type = Techno_Type_Class();
	if (type->Voxel.VoxLib == NULL || type->Voxel.VoxLib->Load_Failed()) {
		return;
	}
	float vx = (float)(Position.X - coord.X);
	float vy = (float)(coord.Y - Position.Y);
	float vz = (float)(Position.Z - coord.Z);
	double theta = PrimaryFacing.Current().As_Radian();
	float sangle = (float)fastmath::sin(theta);
	float cangle = (float)fastmath::cos(theta);
	float distance = (float)fastmath::sqrt((double)vx * vx + (double)vy * vy + (double)vz * vz);
	float scale = (float)((0.04f - distance * 0.000025f) * force / type->Weight);
	if (fabs(distance) < 0.00002 || scale < 0.01f) {
		return;
	}
	if (scale > 0.05f) {
		scale = 0.05f;
	}
	// The direction in the ground plane, normalised.
	float length = (float)fastmath::sqrt((double)vx * vx + (double)vy * vy);
	float nx = vx, ny = vy, nz = 0.0f;
	if (length != 0.0f) {
		nx = vx / length;
		ny = vy / length;
	}
	float forwards = nx * cangle + ny * sangle;
	float cx = -(nz * sangle);
	float cy = nz * cangle;
	float cz = nx * sangle - ny * cangle;
	double sideways = fastmath::sqrt((double)cx * cx + (double)cy * cy + (double)cz * cz);
	if (fabs(forwards * cangle - sideways * sangle - nx) > 0.0002 || fabs(sideways * cangle + forwards * sangle - ny) > 0.0002) {
		sideways = -sideways;
	}
	RockingForwardsPerFrame = full ? forwards * scale : forwards * scale / 2.0f;
	RockingSidewaysPerFrame = -((float)sideways * scale);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y15): TechnoClass::Draw_A_Link_To (gamemd.exe 0x00704E40,
// ObjectClass::DrawALinkTo, 1,059 bytes; the mind control link the capture manager draws), unmatched in
// ra2-decomp. Written from gamemd.exe's code read instruction by instruction: a 3x3 dot at each end, then
// 31 segments along a raised arc (the height 512 * (1 - ((i - 16) / 16)^2) leptons over the straight
// line), each segment lightened toward white by a band that runs along the link with the clock
// (timeGetTime). Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
extern bool __fastcall Line_In_Bounds(Point2D & start, Point2D & end, Rect const & bounds);

void TechnoClass::Draw_A_Link_To(Coord src, Coord dst, RGBClass color)
{
	RGBClass const white(255, 255, 255);
	Point2D from, to;
	TacticalMap->Coord_To_Pixel(src, from);
	TacticalMap->Coord_To_Pixel(dst, to);
	from.Y += TacticalRect.Y;
	to.Y += TacticalRect.Y;
	int pixel = DSurface::Build_Hicolor_Pixel(color);
	Rect dot = Intersect(TacticalRect, Rect(to.X - 2, to.Y - 2, 3, 3));
	DSurface::Composite->Fill_Rect(dot, pixel);
	dot = Intersect(TacticalRect, Rect(from.X - 2, from.Y - 2, 3, 3));
	DSurface::Composite->Fill_Rect(dot, pixel);

	Rect clip = TacticalRect;
	int band = (int)((timeGetTime() >> 4) << 3);
	Coord prev = src;
	for (int i = 1, j = 31, phase = 8; phase <= 256; i++, j--, phase += 8) {
		Coord cur;
		cur.X = (dst.X * i + src.X * j) / 32;
		cur.Y = (dst.Y * i + src.Y * j) / 32;
		double f = (i - 16.0) * 0.0625;
		cur.Z = (int)((dst.Z * i + src.Z * j) / 32 + (1.0 - f * f) * 512.0);

		Point2D p1, p2;
		TacticalMap->Coord_To_Pixel(prev, p1);
		TacticalMap->Coord_To_Pixel(cur, p2);
		p1.Y += TacticalRect.Y;
		p2.Y += TacticalRect.Y;
		if (Line_In_Bounds(p1, p2, clip)) {
			RGBClass shade = color;
			int ratio = (phase - band) & 0x1F8;
			if (ratio <= 0xF8 && ratio != 0) {
				shade.Adjust(ratio, white);
			}
			DSurface::Composite->Draw_Line(p1, p2, DSurface::Build_Hicolor_Pixel(shade));
		}
		prev = cur;
	}
}
