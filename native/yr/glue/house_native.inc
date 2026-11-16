
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/house.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

void HouseClass::Update_Present_Super_Weapons(void)
{
	if (GameActive) {
		for (int s = 0; s < SuperWeapon.Count(); s++) {
			if (SuperWeapon[s]->Is_Present() && ((SuperWeapon[s]->NeedsBuilding && !SuperWeapon[s]->Is_One_Time()) || IsDefeated)) {
				bool present = false;
				bool enabled = false;
				if (!IsDefeated) {
					for (int j = 0; j < Buildings.Count(); j++) {
						BuildingClass * b = Buildings[j];
						if (!b->IsInLimbo && b->IsActive && b->House->Fetch_ID() == Fetch_ID()) {
							for (int k = 0; k < 3; k++) {
								BuildingTypeClass const * upgrade = b->Upgrades[k];
								if (upgrade != NULL && (upgrade->SuperWeapon == s || upgrade->SuperWeapon2 == s)) {
									present = true;
									if (!enabled) enabled = b->IsOn;
								}
							}
							if (b->Fetch_Super_Weapon() == s || b->Fetch_Super_Weapon2() == s) {
								present = true;
								if (enabled) break;
								enabled = b->IsOn;
							}
							if (enabled && present) break;
						}
					}
				}

				int power = Power;
				int drain = Drain;
				if (power < drain && drain != 0) {
					if (power == 0 || (double)power / (double)drain < 1.0) {
						enabled = false;
					}
				}

				if (present && !IsDefeated) {
					if (!enabled && SuperWeapon[s]->Is_Powered() && SuperWeapon[s]->SetOnHold(true)) {
						if (Fetch_ID() == PlayerPtr->Fetch_ID()) {
							if (Map.IsTargettingMode == s) {
								Map.IsTargettingMode = SUPER_NONE;
							}
							Map.Column[1].Flag_To_Redraw();
						}
						IsRecalcNeeded = true;
					}

					if (enabled && SuperWeapon[s]->SetOnHold(false)) {
						if (Fetch_ID() == PlayerPtr->Fetch_ID()) {
							if (s == Map.IsTargettingMode) {
								Map.IsTargettingMode = SUPER_NONE;
							}
							Map.Column[1].Flag_To_Redraw();
						}
						IsRecalcNeeded = true;
					}
				} else {
					if (SuperWeapon[s]->Lose() && PlayerPtr != NULL) {
						if (Fetch_ID() == PlayerPtr->Fetch_ID()) {
							if (Map.IsTargettingMode == s) {
								Map.IsTargettingMode = SUPER_NONE;
							}
							Map.Column[1].Flag_To_Redraw();
						}
						IsRecalcNeeded = true;
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4): HouseClass::Can_Build, unmatched in ra2-decomp. TibSun's text
// (TibSun archive code/house.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md) with the RA2 twin's YR-era tests
// (src/recovered/house.cpp, RA2 0x004F7870's shape: override prerequisites, stolen technology,
// required and forbidden houses) in place of TibSun's ownable/construction-yard test, marked YR;
// TibSun's GDI/Nod factory groups become YR's refinery group. Behaviour code for the native build
// only; gamemd.exe's version is not compared yet.
// ---------------------------------------------------------------------------------------------
int HouseClass::Can_Build(ObjectTypeClass const * type, bool forced, bool include_in_progress) const
{
	assert(type != NULL);

	if (!forced) {

		TechnoTypeClass const * ttype = (TechnoTypeClass const *)type;
		if (ttype->IsUnbuildable) return(0);

		/*
		**	YR (as the RA2 twin): one owned override prerequisite skips every test below.
		*/
		for (int i = 0; i < ttype->PrerequisiteOverride.Count(); i++) {
			if (ABQuantity.Value(ttype->PrerequisiteOverride[i]) > 0) {
				goto skip;
			}
		}

		/*
		**	An object with a prohibited tech level availability will never be allowed, regardless
		**	of who requests it.
		*/
		if (((TechnoTypeClass const *)type)->Level == -1) return(0);

		TypeList<int> pre = ((TechnoTypeClass const *)type)->Prerequisite;

		int level = Control.TechLevel;
#ifdef _DEBUG
		if (Debug_Cheat) {
			level = 98;
			pre.Clear();
		}
#endif

		/*
		**	YR (as the RA2 twin): stolen technology, and the houses allowed or forbidden to own it
		**	(an alternative owner per kind, from captured factories, also qualifies).
		*/
		if (ttype->RequiresStolenAlliedTech && !Side0TechInfiltrated) return(0);
		if (ttype->RequiresStolenSovietTech && !Side1TechInfiltrated) return(0);
		if (ttype->RequiresStolenThirdTech && !Side2TechInfiltrated) return(0);
		if (ttype->RequiredHouses != (DWORD)-1 && (ttype->RequiredHouses & (1 << Class->House)) == 0) {
			unsigned alt = 0;
			switch ((RTTIType)type->RTTI) {
				case RTTI_INFANTRYTYPE: alt = InfantryAltOwner; break;
				case RTTI_UNITTYPE: alt = UnitAltOwner; break;
				case RTTI_AIRCRAFTTYPE: alt = AircraftAltOwner; break;
				case RTTI_BUILDINGTYPE: alt = BuildingAltOwner; break;
				default: break;
			}
			if ((ttype->RequiredHouses & alt) == 0) return(0);
		}
		if (ttype->ForbiddenHouses != (DWORD)-1 && (ttype->ForbiddenHouses & (1 << Class->House)) != 0) return(0);

		if (((TechnoTypeClass const *)type)->Level > level) return(0);

		/*
		**	The computer can always build everything.
		*/
		if (!Is_Human_Player()) return(1);

		/*
		**	See if the prerequisite requirements have been met.
		*/
		for (int i = 0; i < pre.Count(); i++) {
			switch (pre[i]) {
				case STRUCT_G_POWER: {
					bool found = false;
					for (int j = 0; j < Rule->PrerequisitePower.Count(); j++) {
						if (ABQuantity.Value(Rule->PrerequisitePower[j]) > 0) {
							found = true;
							break;
						}
					}
					if (!found) return(0);
				}
				break;

				case STRUCT_G_FACTORY: {
					bool found = false;
					for (int j = 0; j < Rule->PrerequisiteFactory.Count(); j++) {
						if (ABQuantity.Value(Rule->PrerequisiteFactory[j]) > 0) {
							found = true;
							break;
						}
					}
					if (!found) return(0);
				}
				break;

				case STRUCT_G_BARRACKS: {
					bool found = false;
					for (int j = 0; j < Rule->PrerequisiteBarracks.Count(); j++) {
						if (ABQuantity.Value(Rule->PrerequisiteBarracks[j]) > 0) {
							found = true;
							break;
						}
					}
					if (!found) return(0);
				}
				break;

				case STRUCT_G_RADAR: {
					bool found = false;
					for (int j = 0; j < Rule->PrerequisiteRadar.Count(); j++) {
						if (ABQuantity.Value(Rule->PrerequisiteRadar[j]) > 0) {
							found = true;
							break;
						}
					}
					if (!found) return(0);
				}
				break;

				case STRUCT_G_TECH: {
					bool found = false;
					for (int j = 0; j < Rule->PrerequisiteTech.Count(); j++) {
						if (ABQuantity.Value(Rule->PrerequisiteTech[j]) > 0) {
							found = true;
							break;
						}
					}
					if (!found) return(0);
				}
				break;

				case STRUCT_G_GDIFACTORY: {	// YR: the refinery group (PrerequisiteProc)
					bool found = false;
					for (int j = 0; j < Rule->PrerequisiteProc.Count(); j++) {
						if (ABQuantity.Value(Rule->PrerequisiteProc[j]) > 0) {
							found = true;
							break;
						}
					}
					if (!found) return(0);
				}
				break;

				default: {
					BuildingTypeClass * btype = BuildingTypes[pre[i]];
					if (btype->PowersUpBuilding[0] != '\0') {
						BuildingClass * bptr;
						bool found = false;
						for (int j = Buildings.Count() - 1; j >= 0; j--) {
							bptr = Buildings[j];
							if (!bptr->IsInLimbo && bptr->House == this && bptr->IsOn) {
								if (bptr->Mission != MISSION_DECONSTRUCTION && bptr->MissionQueue != MISSION_DECONSTRUCTION) {
									found = true;
									break;
								}
							}
						}
						if (found) {
							for (int k = 0; k < ARRAY_SIZE(bptr->Upgrades); k++) {
								if (bptr->Upgrades[k] == btype) {
									goto breakout;
								}
							}
						}
						return(0);
					}
					if (ABQuantity.Value(pre[i]) == 0) return(0);
					breakout:;
				}
				break;
			}
		}

	}

skip:
	switch ((RTTIType)type->RTTI) {
		case RTTI_UNITTYPE: {
			UnitTypeClass const * utype = (UnitTypeClass const *)type;
			if (utype->BuildLimit <= 0) {
				return(PUQuantity.Value(type->Fetch_Heap_ID()) < abs(utype->BuildLimit));
			}
			int count = UQuantity.Value(type->Fetch_Heap_ID());
			if (utype->DeploysInto != NULL) {
				count += BQuantity.Value(utype->DeploysInto->Fetch_Heap_ID());
			}
			if (count < utype->BuildLimit) {
				return(1);
			}
			if (include_in_progress) {
				for (int i = 0; i < Factories.Count(); i++) {
					FactoryClass * fptr = Factories[i];
					if (fptr->House == this && fptr->Get_Object() != NULL && fptr->Get_Object()->TClass == type) {
						if (Factories[i] != NULL) {
							return(1);
						}
						break;
					}
				}
			}
			return(-1);
		}
		break;

		case RTTI_INFANTRYTYPE: {
			InfantryTypeClass const * itype = (InfantryTypeClass const *)type;
			if (itype->BuildLimit <= 0) {
				return(PIQuantity.Value(type->Fetch_Heap_ID()) < abs(itype->BuildLimit));
			}
			int count = IQuantity.Value(type->Fetch_Heap_ID());
			if (itype->IsVehicleThief) {
				for (int i = 0; i < Units.Count(); i++) {
					UnitClass * unit = Units[i];
					if (unit->House == this && unit->EnteredByInfType == type->Fetch_Heap_ID()) {
						count++;
					}
				}
			}
			if (itype->BuildLimit <= 0 || count < itype->BuildLimit) {
				return(1);
			}
			if (include_in_progress && count == itype->BuildLimit) {
				for (int i = 0; i < Factories.Count(); i++) {
					FactoryClass * fptr = Factories[i];
					if (fptr->House == this && fptr->Get_Object() != NULL && fptr->Get_Object()->TClass == type) {
						if (Factories[i] != NULL) {
							return(1);
						}
						break;
					}
				}
			}
			return(-1);
		}
		break;

		case RTTI_BUILDINGTYPE: {
			BuildingTypeClass const * btype = (BuildingTypeClass const *)type;
			if (btype->BuildLimit <= 0) {
				return(PBQuantity.Value(type->Fetch_Heap_ID()) < abs(btype->BuildLimit));
			}
			int count = BQuantity.Value(type->Fetch_Heap_ID());
			if (count < btype->BuildLimit) {
				return(1);
			}
			if (include_in_progress) {
				for (int i = 0; i < Factories.Count(); i++) {
					FactoryClass * fptr = Factories[i];
					if (fptr->House == this && fptr->Get_Object() != NULL && fptr->Get_Object()->TClass == type) {
						if (Factories[i] != NULL) {
							return(1);
						}
						break;
					}
				}
			}
			return(-1);
		}
		break;

		case RTTI_AIRCRAFTTYPE: {
			AircraftTypeClass const * atype = (AircraftTypeClass const *)type;
			if (atype->BuildLimit <= 0) {
				return(PAQuantity.Value(type->Fetch_Heap_ID()) < abs(atype->BuildLimit));
			}
			int count = AQuantity.Value(type->Fetch_Heap_ID());
			if (count < atype->BuildLimit) {
				return(1);
			}
			if (include_in_progress) {
				for (int i = 0; i < Factories.Count(); i++) {
					FactoryClass * fptr = Factories[i];
					if (fptr->House == this && fptr->Get_Object() != NULL && fptr->Get_Object()->TClass == type) {
						if (Factories[i] != NULL) {
							return(1);
						}
						break;
					}
				}
			}
			return(-1);
		}
		break;
	}
	return(1);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4): HouseClass::Where_To_Place_Building, unmatched in ra2-decomp.
// TibSun's text (TibSun archive code/house.cpp; same licence notice as above); native changes are
// marked NATIVE. Behaviour code for the native build only; gamemd.exe's version is not compared.
// ---------------------------------------------------------------------------------------------
Cell HouseClass::Where_To_Place_Building(BuildingTypeClass *buildingtype, int (*func)(HouseClass const &, Cell const &, int, int), int context)
{
	int i;
	DynamicVectorClass<Cell> sorted_cells;

	if (Base.PlacementCenter == Cell(0, 0)) {
		return(BaseCenter);		// NATIVE: YR keeps the base centre as a cell (BaseCenter)
	}

	/*
	 * Rank the cells the base occupies by the weight the caller's function gives them.
	 */
	IndexClass<int, Cell> ranking;
	sorted_cells.Clear();

	for (i = 0; i < Base.InnerCells.Count(); i++) {
		Cell cell = Base.InnerCells[i];
		int score;
		if (!buildingtype->IsCloakGenerator) {
			score = func(*this, cell, i, context);
		} else {
			score = i + 1000 * (Coord(BaseCenter) - Coord(cell)).Length();	// NATIVE: BaseCenter
		}
		ranking.Add_Index(score, cell);
	}

	/// Pull the ranked cells out in ascending weight order.
	for (i = 0; i < ranking.Count(); i++) {
		sorted_cells.Add(ranking.Fetch_By_Position(i));
	}

	/// Height at base placement center.
	int base_height = Map[Base.PlacementCenter].Height;
	int house_mask  = 1 << HeapID;

	/// Declared out here deliberately -- scoped to the loop, MSVC6 pools its frame slot with
	/// the cloak generator distance temporary and the whole slot map shifts.
	Cell base_cell;

	for (int pass = 0; pass < 2; pass++) {
		for (int index = 0; index < sorted_cells.Count(); index++) {
			base_cell = sorted_cells[index];
			Cell occupied_dir(0, 0);

			/// Sum the directions of the neighbors this house already occupies.
			for (int face = 0; face < FACING_COUNT; face++) {
				CellClass *cptr = &Map[Adjacent_Cell(base_cell, (FacingType)face)];
				if (cptr->OccupiedBy & house_mask) {
					occupied_dir = Adjacent_Cell(occupied_dir, (FacingType)face);
				}
			}

			if (occupied_dir == Cell(0, 0)) {
				continue;
			}

			/// Face away from the mass of the base, so the structure lands on its outside.
			int x = -(int)occupied_dir.X;
			int y = -(int)occupied_dir.Y;
			double angle = fastmath::atan2((double)-y, (double)x);
			DirType dir = DirType(angle);
			FacingType facing = dir.As_Facing();

			/*
			 * Offset the placement so the structure's own footprint and the base
			 * spacing fall on the side the facing points away from.
			 */
			Cell place_offset(0, 0);
			if (facing >= FACING_NE && facing <= FACING_SE) {
				place_offset.X = (short)Rule->AIBaseSpacing;
			} else if (facing >= FACING_SW && facing <= FACING_NW) {
				place_offset.X = (short)(1 - Rule->AIBaseSpacing - buildingtype->Width());
			}
			if (facing >= FACING_SE && facing <= FACING_SW) {
				place_offset.Y = (short)Rule->AIBaseSpacing;
			} else if (facing >= FACING_NW || facing <= FACING_NE) {
				place_offset.Y = (short)(1 - Rule->AIBaseSpacing - buildingtype->Height(false));
			}

			static Cell scan_offsets[FACING_COUNT] = {
				Cell(-1,  0), Cell(-1, -1), Cell( 0, -1), Cell( 1, -1),
				Cell( 1,  0), Cell( 1,  1), Cell( 0,  1), Cell(-1,  1)
			};
			static Cell step_offsets[FACING_COUNT] = {
				Cell( 1,  0), Cell( 1,  1), Cell( 0,  1), Cell(-1,  1),
				Cell(-1,  0), Cell(-1, -1), Cell( 0, -1), Cell( 1, -1)
			};

			int trial = 0;

			do {
				/*
				 * Compute start position for placement search.
				 */
				Cell pos = base_cell + place_offset + scan_offsets[facing];

				/*
				 * On the second placement try, start one cell away from the
				 * occupied mass (opposite facing).
				 */
				if (trial == 1) {
					pos = Adjacent_Cell(pos, Facing_Sub(facing, FACING_COUNT / 2));
				}

				/// The spacing border applies only on the first placement try.
				int use_spacing = (trial != 1);
				int attempt = 0;

				while (attempt < 3) {
					int total_height = buildingtype->Height(false);
					total_height += 2 * (Rule->AIBaseSpacing * use_spacing);
					int spacing      = Rule->AIBaseSpacing * use_spacing;
					int total_width  = buildingtype->Width() + 2 * spacing;

					Rect rect;
					rect.X      = pos.X - spacing;
					rect.Y      = pos.Y - spacing;
					rect.Width  = total_width;
					rect.Height = total_height;

					/// The second pass relaxes the area-availability requirement.
					if (pass == 1 || Map.IsAreaFree(rect, HeapID) /* NATIVE: YR's name */) {
						if (buildingtype->Legal_Placement(pos, this)) {
							if (abs(base_height - Map[pos].Height) < 3 && Can_Build_Here(buildingtype, pos)) {
								return(pos);
							}
						}
					}

					pos += step_offsets[facing];
					attempt++;
				}
				trial++;
			} while (trial < 2);
		}
	}

	return(Cell(0, 0));
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4): HouseClass::Can_Build_Here, unmatched in ra2-decomp.
// TibSun's text (TibSun archive code/house.cpp; same licence notice as above); native changes are
// marked NATIVE. Behaviour code for the native build only; gamemd.exe's version is not compared.
// ---------------------------------------------------------------------------------------------
bool HouseClass::Can_Build_Here(BuildingTypeClass *building, Cell const & cell)
{
	if (Session.Type == GAME_NORMAL) {
		return(true);
	}

	int spacing = Rule->AIBaseSpacing;
	int width = (2 * spacing) + building->Width();
	int height = (2 * spacing) + building->Height();

	int mask = 1 << HeapID;

	for (int x = cell.X - spacing - 1; x < cell.X + width + 1; x++) {
		for (int y = cell.Y - spacing - 1; y < cell.Y + height + 1; y++) {
			if (Map[Cell(x, y)].OccupiedBy & mask) {
				return(true);
			}
		}
	}

	return(false);
}
