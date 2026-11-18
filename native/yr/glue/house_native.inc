
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
// TibSun's GDI/Nod factory groups become YR's refinery group. Lane Y6 compared the ownership tests
// with gamemd.exe 0x004F7870 and added YR's secret-lab grant and the super-weapons-off test; the
// rest is not compared. Behaviour code for the native build only.
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
		**	YR (gamemd.exe 0x004F78E4, lane Y6): what one of the house's secret labs gives is
		**	allowed without further tests.
		*/
		for (int i = 0; i < SecretLabs.Count(); i++) {
			if (SecretLabs[i]->FUN_00459840() == ttype) {
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

		/*
		**	YR (gamemd.exe 0x004F7A9A, lane Y6): with super weapons off, a building whose super weapon
		**	can be switched off from the setup is not built, unless it is one of the rules' BuildTech.
		*/
		if (!Unsorted::SWAllowed && (RTTIType)type->RTTI == RTTI_BUILDINGTYPE) {
			BuildingTypeClass const * btype = (BuildingTypeClass const *)type;
			if (btype->SuperWeapon != SUPER_NONE && !Rule->BuildTech.Is_In_List(btype)
					&& SuperWeapon[btype->SuperWeapon]->Class->DisableableFromShell) {
				return(0);
			}
		}

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

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y6): HouseClass::AI_Build_Defense (gamemd.exe 0x00506EF0, 3,216 B),
// unmatched in ra2-decomp. The text is the RA2 twin's (src/recovered/house.cpp, RA2 0x004F1190,
// exact there) in the YR tree's names, with YR's changes read from gamemd.exe's code: the facing
// inlined as DirType(atan2), the candidate weights are the defense values alone (RA2 adds
// 10000 / cost), the cells ranked through BasePlanningCellContainer (IndexClass here). Not compared
// byte for byte; behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
static inline int AI_Defense_Quadrant(Cell const & offset)
{
	DirType dir = DirType(fastmath::atan2((double)-offset.Y, (double)offset.X));
	return(dir.As_Dir4());
}

bool HouseClass::AI_Build_Defense(int nodeid, DynamicVectorClass<Cell> * cells)
{
	int i;
	int best = -1;
	int best_value = 999999;
	int air_values[4] = {0, 0, 0, 0};
	int armor_values[4] = {0, 0, 0, 0};
	int ground_values[4] = {0, 0, 0, 0};
	DynamicVectorClass<Cell> quadrants[4];

	int size = Base.BaseAreaRect.Width * Base.BaseAreaRect.Height;
	int * air_map = new int[size];
	int * armor_map = new int[size];
	int * ground_map = new int[size];
	memset(air_map, 0, size * sizeof(int));
	memset(armor_map, 0, size * sizeof(int));
	memset(ground_map, 0, size * sizeof(int));

	if (cells != NULL) {
		for (i = 0; i < cells->Count(); i++) {
			Cell offset = (*cells)[i] - Base.PlacementCenter;
			quadrants[AI_Defense_Quadrant(offset)].Add((*cells)[i]);
		}
	}

	for (i = 0; i < Buildings.Count(); i++) {
		BuildingClass * building = Buildings[i];
		Cell offset = building->Get_Cell() - Base.PlacementCenter;
		if (offset.X != 0 || offset.Y != 0) {
			int quadrant = AI_Defense_Quadrant(offset);
			int air = building->Anti_Air();
			int armor = building->Anti_Armor();
			int infantry = building->Anti_Infantry();
			if (air + armor + infantry > 0) {
				air_values[quadrant] += air;
				armor_values[quadrant] += armor;
				ground_values[quadrant] += infantry;
				Calculate_Defense_Values(building, air, air_map, Base.BaseAreaRect);
				Calculate_Defense_Values(building, armor, armor_map, Base.BaseAreaRect);
				Calculate_Defense_Values(building, infantry, ground_map, Base.BaseAreaRect);
			}
		}
	}

	for (i = 0; i < 4; i++) {
		int total = air_values[i] + armor_values[i] + ground_values[i];
		if (total < best_value && (cells == NULL || quadrants[i].Count() > 0)) {
			best_value = total;
			best = i;
		}
	}
	if (best < 0) best = 0;		// NATIVE: the original reads index -1 when no quadrant qualifies

	int air_value = air_values[best];
	int armor_value = armor_values[best];
	int ground_value = ground_values[best];

	Calculate_Enemy_Predictions();

	DynamicVectorClass<BuildingTypeClass const *> owned;
	for (i = 0; i < Buildings.Count(); i++) {
		BuildingTypeClass * type = Buildings[i]->Class;
		if (!type->IsBaseDefense && type != Rule->WallTower) {
			owned.Add(type);
		}
	}

	int type_id = Base.Nodes[nodeid].Type;
	BuildingTypeClass * pending = NULL;

	float air_ratio = best_value != 0 ? air_value / (float)best_value : 0.0f;
	float armor_ratio = best_value != 0 ? armor_value / (float)best_value : 0.0f;
	float ground_ratio = best_value != 0 ? ground_value / (float)best_value : 0.0f;
	float air_deficit = EnemyAirForcePrediction - air_ratio;
	float armor_deficit = EnemyArmorForcePrediction - armor_ratio;
	float ground_deficit = EnemyInfantryForcePrediction - ground_ratio;

	DynamicVectorClass<BuildingTypeClass *> air_defenses = Get_Anti_Air_Defense_Buildings(owned);
	DynamicVectorClass<BuildingTypeClass *> armor_defenses = Get_Anti_Armor_Defense_Buildings(owned);
	DynamicVectorClass<BuildingTypeClass *> ground_defenses = Get_Anti_Ground_Defense_Buildings(owned);

	DynamicVectorClass<BuildingTypeClass *> * defenses;
	int category;
	if (air_deficit <= armor_deficit || air_deficit <= ground_deficit) {
		if (armor_deficit > ground_deficit) {
			defenses = &armor_defenses;
			BaseAreaMap = armor_map;
			category = 1;
		} else {
			defenses = &ground_defenses;
			BaseAreaMap = ground_map;
			category = 2;
		}
	} else {
		defenses = &air_defenses;
		BaseAreaMap = air_map;
		category = 0;
	}

	bool success = true;
	if (defenses->Count() == 0) {
		if (armor_defenses.Count() > 0) {
			defenses = &armor_defenses;
			category = 1;
			BaseAreaMap = armor_map;
		} else if (ground_defenses.Count() > 0) {
			defenses = &ground_defenses;
			category = 2;
			BaseAreaMap = ground_map;
		} else if (air_defenses.Count() > 0) {
			defenses = &air_defenses;
			category = 0;
			BaseAreaMap = air_map;
		} else {
			success = false;
		}
	}

	if (success) {
		BuildingTypeClass * choice;
		if (defenses->Count() == 1) {
			choice = (*defenses)[0];
		} else {
			DiscreteDistributionClass<BuildingTypeClass> distribution;
			for (i = 0; i < defenses->Count(); i++) {
				BuildingTypeClass * type = (*defenses)[i];
				int weight;
				if (category == 2) {
					weight = type->AntiInfantryValue;
				} else if (category == 1) {
					weight = type->AntiArmorValue;
				} else {
					weight = type->AntiAirValue;
				}
				distribution.Add(type, weight);
			}
			choice = distribution.Sample();
		}

		BuildingTypeClass * build_type;
		if (type_id >= 0) {
			build_type = BuildingTypes[type_id];
			pending = choice;
		} else {
			build_type = choice;
		}

		Cell cell(0, 0);
		if (choice == NULL || build_type == NULL) {
			// NATIVE: a distribution with no weight samples nothing; the original goes on with NULL
		} else if (cells != NULL) {
			IndexClass<int, Cell> ranking;
			for (i = 0; i < quadrants[best].Count(); i++) {
				Cell threat = quadrants[best][i];
				ranking.Add_Index(Base_Cell_Weight_By_Defense_Coverage(*this, threat, i, -1), threat);
			}
			if (ranking.Count() > 0) {
				cell = ranking.Fetch_By_Position(0);
				cells->Delete_Index(cells->ID(cell));
			}
		} else {
			cell = Where_To_Place_Building(build_type, Base_Cell_Weight_By_Defense_Coverage, 2 * best);
		}

		if (cell == Cell(0, 0)) {
			success = false;
		} else if (pending != NULL) {
			Base.Nodes[nodeid].CellID = cell;
			if (nodeid + 1 < Base.Nodes.Count() && Base.Nodes[nodeid + 1].Type == -1) {	// NATIVE: bounds
				Base.Nodes[nodeid + 1].Type = (StructType)pending->HeapID;
				Base.Nodes[nodeid + 1].CellID = cell;
				if (cells != NULL) {
					BuildingTypeClass const * wall = (BuildingTypeClass const *)FirstBuildableFromArray(Rule->ConcreteWalls);
					if (wall != NULL) {
						for (i = Base.Nodes.Count() - 1; i >= 0; i--) {
							if (Base.Nodes[i].CellID == cell && Base.Nodes[i].Type == wall->HeapID) {
								Base.Nodes.Delete_Index(i);
								break;
							}
						}
					}
				}
			}
		} else {
			Base.Nodes[nodeid].Type = (StructType)build_type->HeapID;
			Base.Nodes[nodeid].CellID = cell;
		}
	}

	delete [] air_map;
	delete [] armor_map;
	delete [] ground_map;
	BaseAreaMap = NULL;

	return(success);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y7): HouseClass::Random_Cell_In_Zone, YR 0x00501AC0 (1,596 bytes,
// ra2-decomp's PickRandomCellInZone, unmatched; a link-closure trap reached through Where_To_Go
// when a computer player's unit looked for somewhere to go). TibSun's text (code/house.cpp) with
// gamemd.exe's changes read from its code: the base's centre is the coordinate of the base centre
// cell (FUN_0050DF30: COORD_NONE without one) instead of a Center member, the random numbers are
// the scenario's (Scen->RandomNumber), and the legal-area test is In_Local_Radar(cell, true)
// (MapClass::IsWithinUsableArea 0x00578460). The quadrant rotation is written out on the raw
// facing as gamemd.exe does it (the upper three bits from the previous eighth, the rest kept).
// ---------------------------------------------------------------------------------------------
static inline DirType Zone_Rotate_Back_One_Facing(DirType const & dir)
{
	unsigned short raw = dir.Raw;
	DirType back;
	back.Raw = (unsigned short)((((raw & 0xE000) - 1) & 0xE000) | (raw & 0x1FFF));
	return back;
}

Cell HouseClass::Random_Cell_In_Zone(ZoneType zone) const
{
	Coord coord = COORD_NONE;
	int radius = MAX(3 * CELL_LEPTON, MIN(Radius, 8 * CELL_LEPTON));
	int maxdist = radius * 2;

	switch (zone) {
		case ZONE_CORE: {
			int distance = Scen->RandomNumber(0, radius);
			coord = Coord_Scatter(FUN_0050DF30(), distance, true);
			break;
		}

		case ZONE_NORTH:
		case ZONE_EAST:
		case ZONE_SOUTH:
		case ZONE_WEST: {
			int distance = Scen->RandomNumber(radius, maxdist);
			DirType dir;
			dir.Raw = (unsigned short)Scen->RandomNumber();
			switch (zone) {
				case ZONE_NORTH: dir.Raw &= ~0xC000; break;
				case ZONE_EAST: dir.Raw &= ~0x8000; dir.Raw |= 0x4000; break;
				case ZONE_SOUTH: dir.Raw &= ~0x4000; dir.Raw |= 0x8000; break;
				default: dir.Raw |= 0xC000; break;
			}
			dir = Zone_Rotate_Back_One_Facing(dir);
			coord = Move_Coord(FUN_0050DF30(), dir, distance);
			break;
		}

		default:
			break;
	}

	Cell cell;
	if (coord == COORD_NONE) {
		if (zone == ZONE_CORE) {
			cell = Get_Base_Center();
		} else {
			cell = Random_Cell_In_Zone(ZONE_CORE);
		}
	} else {
		cell = coord.As_Cell();
	}

	if (!Map.In_Local_Radar(cell, true)) {
		cell = Map.Clip_To_Map(cell);
	}
	return(cell);
}

// NON-EXACT NATIVE DRAFT (lane Y14): HouseClass::Fire_PsyDom, gamemd.exe 0x0050A150 (497 bytes, unmatched in
// ra2-decomp; a link-closure trap on the play paths: an AI house firing its Psychic Dominator). Written from
// gamemd.exe's code in the shape of its matched siblings Fire_GenMutator (0x00509F60) and FUN_0050A350: with
// no dominator already active (FUN_0053B400, PsyDom::Active) and an enemy chosen, and no preferred target
// cell, every foot unit out of limbo scores the cells of its spread (CellSpreadTable up to entry
// DAT_007ED3D0[3]) by the foot units there that are not allied, not in the air and can be permanently mind
// controlled; the best cell, if it is in the usable map, is the target of Place_Special_Blast.
bool FUN_0053B400(void);

void HouseClass::Fire_PsyDom(SuperClass * super)
{
	if (FUN_0053B400()) {
		return;
	}
	if (Enemy == HOUSE_NONE) {
		return;
	}

	Cell target = CELL_NONE;
	int bestcount = 0;

	if (PreferredTargetCell == CELL_NONE) {
		for (int i = Feet.Count() - 1; i >= 0; i--) {
			FootClass * foot = Feet[i];
			if (foot->IsInLimbo) {
				continue;
			}
			CellClass * cellptr = foot->Get_Cell_Ptr();
			int count = 0;
			for (int j = 0; j <= DAT_007ED3D0[3]; j++) {
				Cell cell = cellptr->CellID + Unsorted::CellSpreadTable[j];
				CellClass * cc = &Map[cell];
				for (ObjectClass * object = cc->Cell_Occupier(); object != NULL && object->Is_Foot(); object = object->Next) {
					if (!Is_Ally(object) && !object->In_Air() && ((TechnoClass *)object)->CanBePermaMindControlled()) {
						count++;
					}
				}
			}
			if (count > bestcount) {
				bestcount = count;
				target = cellptr->CellID;
			}
		}

		if (bestcount != 0) {
			if (target != CELL_NONE && Map.In_Local_Radar(target, true)) {
				Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(super), target);
			}
		}
	}
}
