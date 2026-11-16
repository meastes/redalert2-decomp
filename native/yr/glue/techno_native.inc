
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
			converter = ColorSchemes[House->Scheme]->Converter;	// NATIVE: YR has no eight-bit unit compositing
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
	ConvertClass * converter = NULL;
	converter = ColorSchemes[House->Scheme]->Converter;	// NATIVE: YR has no eight-bit unit compositing

	int predoffset = 0;
	if (flags & SHAPE_PREDATOR) {
		predoffset = Get_Predator_Offset();
	}

	Blit_Block(*LogicalSurface, *converter, *VoxelDrawSystem::Get_Surface(), region.Bounds, region.Point + point, cliprect, NULL, converter->Blitter_From_Flags(flags), Get_Z_Adjust(), Get_Z_Gradient(), brightness, predoffset);

	return(region);
}

void TechnoClass::Draw_Health_Bar(Point2D const & xpoint, Rect const & cliprect, bool sensed_underground) const
{
	if (RTTI == RTTI_BUILDING) {	// NATIVE: YR has no Core Defender unit type flag

		/*
		 * Build the screen-space pip bar from the object's lepton dimensions.
		 */
		HeightAGL;

		Point3D dim = TClass->Lepton_Dimensions();

		Point3D half(dim.X / 2, dim.Y / 2, dim.Z / 2);

		/*
		 * The "dim - half" difference is computed for all three components and Z is
		 * then replaced with the full height, so the Z store below is dead.
		 */
		Coord coord = dim - half;
		coord.X = -coord.X;
		coord.Z = dim.Z;

		/*
		 * The X sign flips around the Y flips are redundant -- the paired negations
		 * are dead load/neg/neg/store sequences.
		 */
		Point2D p0 = TacticalMap->Coord_To_Pixel_Absolute(coord);
		coord.X = -coord.X;
		coord.Y = -coord.Y;
		coord.X = -coord.X;
		Point2D p1 = TacticalMap->Coord_To_Pixel_Absolute(coord);
		coord.Z = 0;
		coord.Y = -coord.Y;
		Point2D p2 = TacticalMap->Coord_To_Pixel_Absolute(coord);
		coord.X = -coord.X;
		coord.Y = -coord.Y;
		coord.X = -coord.X;

		int barlen = (p0.Y - p1.Y) / 2;

		int n = (int)(HealthRatio * (double)barlen);
		if (n <= 1) {
			n = 1;
		}
		if (n >= barlen) {
			n = barlen;
		}

		int condcolor = 1;
		if (HealthRatio <= Rule->ConditionYellow) {
			condcolor = 2;
		}
		if (HealthRatio <= Rule->ConditionRed) {
			condcolor = 4;
		}

		int ybase = 2 - 2 * barlen;

		Point2D point;
		int index;

		int yoff = 0;
		int xoff = 0;
		for (index = 0; index < n; index++) {
			point.X = xpoint.X + p0.X + 4 * barlen + 3;
			point.Y = xpoint.Y + p0.Y + ybase + 2;
			point.X -= xoff;
			point.Y -= yoff;
			Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::PipShapes, condcolor, point, cliprect, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER));
			xoff += 4;
			yoff -= 2;
		}

		yoff = -2 * n;
		xoff = 4 * n;
		for (index = n; index < barlen; index++) {
			point.X = xpoint.X + p0.X + 4 * barlen + 3;
			point.Y = xpoint.Y + p0.Y + ybase + 2;
			point.X -= xoff;
			point.Y -= yoff;
			Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::PipShapes, 0, point, cliprect, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER));
			xoff += 4;
			yoff -= 2;
		}

		if (House->Is_Ally(PlayerPtr) || (SpiedBy & (1<<(PlayerPtr->Class->House)))) {
			Draw_Pips(xpoint + p2, xpoint, cliprect);
		}

	} else {

		bool powerup = false;
		if (ArmorBias > 1.0 || FirepowerBias > 1.0 || (Is_Foot() && ((FootClass *)this)->SpeedBias > 1.0)) {
			powerup = true;
		}

		Point2D offset;
		int health_bar_count;

		if (RTTI == RTTI_INFANTRY) {
			if (!sensed_underground || IsSelected) {
				Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::SelectShapes, powerup ? 6 : 2, xpoint, cliprect, ShapeFlags_Type(SHAPE_ALPHA|SHAPE_WIN_REL|SHAPE_CENTER));
			}
			offset = Point2D(-5, -24);
			health_bar_count = 8;
		} else {
			if (!sensed_underground || IsSelected) {
				Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::SelectShapes, (powerup ? 4 : 0) + 3	/* NATIVE: no limpet drones in YR */, xpoint, cliprect, ShapeFlags_Type(SHAPE_ALPHA|SHAPE_WIN_REL|SHAPE_CENTER));
			}
			offset = Point2D(-15, -25);
			health_bar_count = 17;
		}

		int n = (int)(HealthRatio * (double)health_bar_count);
		if (n <= 1) {
			n = 1;
		}
		if (n >= health_bar_count) {
			n = health_bar_count;
		}

		int shapenum = 9;
		if (HealthRatio <= Rule->ConditionYellow) {
			shapenum = 10;
		}
		if (HealthRatio <= Rule->ConditionRed) {
			shapenum = 11;
		}

		Point2D point;
		for (int index = 0; index < n; index++) {
			point.X = xpoint.X + offset.X;
			point.Y = offset.Y + xpoint.Y;
			point.X += 2 * index;
			Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::PipShapes, shapenum, point, cliprect, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER));
		}

		if (House->Is_Ally(PlayerPtr) || (SpiedBy & (1<<(PlayerPtr->Class->House)))) {
			Draw_Pips(xpoint + Point2D(-10, +10), xpoint, cliprect);
		}
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
