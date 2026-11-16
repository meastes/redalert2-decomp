
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the combat unit: the area damage YR calls
// Explosion_Damage, MapClass::DamageArea and FUN_00489280 (one function, 0x00489280, 4,710 bytes,
// unmatched in ra2-decomp; the other names are bound to this one in data/name_aliases.tsv).
// TibSun's text of Explosion_Damage (TibSun archive code/combat.cpp; SPDX GPL-3.0-or-later,
// Copyright 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL section 7 terms;
// THIRD_PARTY.md); native changes are marked NATIVE. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
inline static int Explosion_Distance(Coord const & coord1, Coord const & coord2)
{
	int z1 = coord1.Z;
	int z2 = coord2.Z;
	if (abs(z2 - z1) < LEVEL_LEPTON_H) {
		z2 = coord1.Z;
	}
	return(Coord(coord1.X - coord2.X, coord1.Y - coord2.Y, z1 - z2).Length());
}

int Explosion_Damage(Coord const & coord, int strength, TechnoClass * source, WarheadTypeClass const * warhead, bool dochainreaction, HouseClass * house)	// NATIVE: YR's signature (house unused, result 0)
{
	Cell								cell;		// Cell number under explosion.
	ObjectClass *						object;		// Working object pointer.
	DynamicVectorClass<ObjectClass *>	objects;	// Maximum number of objects that can be damaged.
	int									distance;	// Distance to unit.
	int									range;		// Damage effect radius.

	if (Scen->Special.IsInert || warhead == NULL) return(0);

	if (!strength) return(0);	// NATIVE: TibSun's webby warheads have no YR counterpart

	range = CELL_LEPTON_W + (CELL_LEPTON_W >> 1);
	cell = coord.As_Cell();

	CellClass * cellptr = &Map[cell];

	if (Map.Get_Height_GL(cell) < coord.Z) {
		int index;

		for (index = 0; index < Aircraft.Count(); index++) {
			AircraftClass * aircraft = Aircraft[index];

			if (aircraft->IsActive) {
				if (aircraft->IsDown && aircraft->Strength > 0) {
					distance = coord.Distance_To(aircraft->PositionCoord);
					if (distance < CELL_LEPTON_W) {
						objects.Delete(aircraft);
						objects.Add(aircraft);
					}
				}
			}
		}

		for (index = 0; index < Infantry.Count(); index++) {
			InfantryClass * infantry = Infantry[index];

			if (infantry->IsActive && infantry->Class->JumpJet) {
				if (infantry->IsDown && infantry->Strength > 0) {
					distance = coord.Distance_To(infantry->PositionCoord);
					if (distance < CELL_LEPTON_W) {
						objects.Delete(infantry);
						objects.Add(infantry);
					}
				}
			}
		}

		for (index = 0; index < Units.Count(); index++) {
			UnitClass * unit = Units[index];

			if (unit->IsActive && false /* NATIVE: no jellyfish in YR */) {
				if (unit->IsDown && unit->Strength > 0) {
					distance = coord.Distance_To(unit->PositionCoord);
					if (distance < CELL_LEPTON_W) {
						objects.Delete(unit);
						objects.Add(unit);
					}
				}
			}
		}

	}

	CellClass *cptr = cellptr;
	ObjectClass *impacto;
	bool isbridge = false;
	if (cptr->IsUnderBridge && coord.Z > Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT / 2) {
		impacto = cptr->Cell_Bridge_Occupier();
		isbridge = true;
	} else {
		impacto = cptr->Cell_Occupier(false);
	}

	/*
	**	Fill the list of unit IDs that will have damage
	**	assessed upon them. The units can be lifted from
	**	the cell data directly.
	*/
	for (FacingType i = FACING_NONE; i < FACING_COUNT; i++) {

		/*
		**	Fetch a pointer to the cell to examine. This is either
		**	an adjacent cell or the center cell. Damage never spills
		**	further than one cell away.
		*/
		if (i != FACING_NONE) {
			cptr = &Map[cell].Adjacent_Cell(i);
		}

		/*
		**	Add all objects in this cell to the list of objects to possibly apply
		**	damage to. The list stops building when the object pointer list becomes
		**	full.  Do not include overlapping objects; selection state can affect
		**	the overlappers, and this causes multiplayer games to go out of sync.
		*/
		object = cptr->Cell_Occupier(isbridge);
		while (object) {
			if (object != source) {
				if (object->RTTI != RTTI_UNIT || !Scen->Special.IsHarvesterImmune || !Rule->HarvesterUnit.Is_In_List((UnitTypeClass *)object->Class_Of())) {
					objects.Delete(object);
					objects.Add(object);
				}
			}
			object = object->Next;
		}

		if (cptr->Overlay != OVERLAY_NONE) {
			if (OverlayTypes[cptr->Overlay]->IsVeinholeMonster) {
				VeinholeMonsterClass * veinhole = VeinholeMonsterClass::Get_Monster_At(cell);
				if (veinhole != NULL) {
					objects.Add(veinhole);
				}
			}
		}
	}

	/*
	**	Sweep through the units to be damaged and damage them. When damaging
	**	buildings, consider a hit on any cell the building occupies as if it
	**	were a direct hit on the building's center.
	*/
	for (int index = 0; index < objects.Count(); index++) {
		object = objects[index];

		object->IsToDamage = false;
		if (object->IsActive && (object->RTTI != RTTI_BUILDING || !((BuildingClass *)object)->Class->IsInvisibleInGame)) {
			if (object->RTTI == RTTI_BUILDING && impacto == object) {
				distance = 0;
			} else {
				distance = Explosion_Distance(coord, object->Target_Coord());
				if (object->RTTI == RTTI_AIRCRAFT) {
					distance /= 2;
				}
			}
			if (object->Strength > 0 && object->IsDown && !object->IsInLimbo && distance < range) {
				int damage = strength;
				if (true /* NATIVE: TibSun's ion storm warhead exemption */ || !object->Is_Foot() || ((FootClass *)object)->Team == NULL || !((FootClass *)object)->Team->Class->IsIonImmune) {
					object->Take_Damage(damage, distance, warhead, source);
				}
			}
		}
	}

	double rocking_force = MIN(strength * 0.01, 4.0);
	if (warhead->IsRocker && rocking_force > 0.3) {
		for (int x = cell.X - 3; x <= cell.X + 3; x++) {
			for (int y = cell.Y - 3; y <= cell.Y + 3; y++) {
				object = isbridge ? Map[Cell(x, y)].Cell_Occupier(true) : Map[Cell(x, y)].Cell_Occupier(false);

				while (object) {
					TechnoClass * techno = Dynamic_Cast<TechnoClass *>(object);
					if (techno != NULL) {
						if (Cell(x, y) == cell && source) {
							Coord tcoord = techno->PositionCoord;

							Coord scoord = source->PositionCoord;
							TPoint3D<float> rockdirf((float)(scoord.X - tcoord.X), (float)(scoord.Y - tcoord.Y), (float)(scoord.Z - tcoord.Z));
							rockdirf = rockdirf.Normalize() * 10;

							tcoord += Coord(rockdirf.X, rockdirf.Y, rockdirf.Z);
							techno->Rock(tcoord, rocking_force);
						} else {
							techno->Rock(coord, rocking_force);
						}
					}
					object = object->Next;
				}
			}
		}
	}

	/*
	**	If there is a wall present at this location, it may be destroyed. Check to
	**	make sure that the warhead is of the kind that can destroy walls.
	*/
	cellptr = &Map[cell];
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = OverlayTypes[cellptr->Overlay];

		if (optr->IsChainReaction && (!optr->IsTiberium || warhead->IsTiberiumDestroyer) && dochainreaction) {
			Chain_Reaction_Damage(cell);
			cellptr->Reduce_Tiberium(strength / 10);
		}

		if (optr->IsWall && (warhead->IsWallDestroyer || (warhead->IsWoodDestroyer && optr->Armor == ARMOR_WOOD))) {
			cellptr->Reduce_Wall(strength);
		}

		if (cellptr->Overlay == OVERLAY_NONE) {
			TechnoClass::Remove_Target(cellptr);
		}
	}

	/*
	**	If there is a bridge at this location, then it may be destroyed by the
	**	combat damage.
	*/
	bool ion_cannon = warhead == Rule->IonCannonWarhead;
	if (Scen->Special.IsDestroyBridges && warhead->IsWallDestroyer) {

		IsometricTileType ittype1 = IsometricTileType(cellptr->ITType - IsometricTileTypeClass::BridgeSet + 1);
		CellClass * bridge_deck = cellptr->Get_Bridge_Deck_CellClass();

		if (bridge_deck && bridge_deck->Is_Overlay_Bridge() ||
			ittype1 == IsometricTileTypeClass::BridgeMiddle1 + 0 || ittype1 == IsometricTileTypeClass::BridgeMiddle1 + 1 ||
			ittype1 == IsometricTileTypeClass::BridgeMiddle1 + 2 || ittype1 == IsometricTileTypeClass::BridgeMiddle1 + 3 ||
			ittype1 == IsometricTileTypeClass::BridgeMiddle2 + 0 || ittype1 == IsometricTileTypeClass::BridgeMiddle2 + 1 ||
			ittype1 == IsometricTileTypeClass::BridgeMiddle2 + 2 || ittype1 == IsometricTileTypeClass::BridgeMiddle2 + 3) {
			if (!cellptr->IsUnderBridge || coord.Z <= BRIDGE_LEPTON_HEIGHT + LEVEL_LEPTON_H * (cellptr->Height + 1) && coord.Z > BRIDGE_LEPTON_HEIGHT + LEVEL_LEPTON_H * (cellptr->Height - 2)) {
				if (warhead->IsWallDestroyer && (warhead == Rule->IonCannonWarhead || Random_Pick(1, Rule->BridgeStrength) < strength)) {
					bool bridge_damaged = false;
					int tries=4;
					do {
						bridge_damaged = Map.Damage_Bridge(cell);
						tries--;
						if (bridge_damaged) {
							break;
						}
					} while (ion_cannon && tries > 0);

					if (bridge_damaged) {
						TechnoClass::Remove_Target(cellptr);
					}
					Point2D point;
					TacticalMap->Coord_To_Pixel(coord, point);
					TacticalMap->Register_Dirty_Area(Rect(point.X - 128, point.Y - 128, 256, 256), false);
				}
			}
		}

		IsometricTileType ittype2 = IsometricTileType(cellptr->ITType - IsometricTileTypeClass::TrainBridgeSet + 1);

		if (bridge_deck && bridge_deck->Is_Overlay_Rail_Bridge() ||
			ittype2 == IsometricTileTypeClass::BridgeMiddle1 + 0 || ittype2 == IsometricTileTypeClass::BridgeMiddle1 + 1 ||
			ittype2 == IsometricTileTypeClass::BridgeMiddle1 + 2 || ittype2 == IsometricTileTypeClass::BridgeMiddle1 + 3 ||
			ittype2 == IsometricTileTypeClass::BridgeMiddle2 + 0 || ittype2 == IsometricTileTypeClass::BridgeMiddle2 + 1 ||
			ittype2 == IsometricTileTypeClass::BridgeMiddle2 + 2 || ittype2 == IsometricTileTypeClass::BridgeMiddle2 + 3) {
			if (!cellptr->IsUnderBridge || coord.Z <= BRIDGE_LEPTON_HEIGHT + LEVEL_LEPTON_H * (cellptr->Height + 1) && coord.Z > BRIDGE_LEPTON_HEIGHT + LEVEL_LEPTON_H * (cellptr->Height - 2)) {
				if (warhead->IsWallDestroyer && (warhead == Rule->IonCannonWarhead || Random_Pick(1, Rule->BridgeStrength) < strength)) {
					bool bridge_damaged = false;
					int tries=4;
					do {
						bridge_damaged = Map.Damage_Bridge(cell);
						tries--;
						if (bridge_damaged) {
							break;
						}
					} while (ion_cannon && tries > 0);

					if (bridge_damaged) {
						TechnoClass::Remove_Target(cellptr);
					}
					Point2D point;
					TacticalMap->Coord_To_Pixel(coord, point);
					TacticalMap->Register_Dirty_Area(Rect(point.X - 96, point.Y - 96, 192, 192), false);
				}
			}
		}

		if (cellptr->Overlay >= OVERLAY_LOWBRIDGE_01 && cellptr->Overlay <= OVERLAY_LOWBRIDGE_26) {
			if (warhead == Rule->IonCannonWarhead || Random_Pick(1, Rule->BridgeStrength) < strength) {
				bool destroyed = Map.Damage_Low_Bridge(cell);
				Map.Damage_Low_Bridge(cell);
				if (destroyed) {
					TechnoClass::Remove_Target(cellptr);
				}
			}
		}
	}

	if (cellptr->Overlay != OVERLAY_NONE && OverlayTypes[cellptr->Overlay]->IsExplosive) {
		cellptr->Register_For_Redraw();
		cellptr->Overlay = OVERLAY_NONE;
		cellptr->Recalc_Attributes();
		Map.Update_Cell_Zone(cellptr->CellID);
		Map.Update_Cell_Subzones(cellptr->CellID);
		TechnoClass::Remove_Target(cellptr);

		new AnimClass(Rule->BarrelExplode, coord);
		Explosion_Damage(coord, Rule->AmmoCrateDamage, NULL, Rule->C4Warhead, true);
		for (int i = 0; i < Rule->BarrelDebris.Count(); i++) {
			if (Percent_Chance(15)) {
				new VoxelAnimClass(Rule->BarrelDebris[i], coord);
				break;
			}
		}
		if (Percent_Chance(25)) {
			ParticleSystemClass * psys = new ParticleSystemClass(Rule->BarrelParticle, coord);
			psys->Spawn_Held_Particle(coord, coord);
		}

		static FacingType _part_facings[] = { FACING_N, FACING_E, FACING_S, FACING_W };

		for (int f = 0; f < ARRAY_SIZE(_part_facings); f++) {
			Coord adjacent = Adjacent_Coord_With_Height(coord, _part_facings[f]);
			if (Map[adjacent].Overlay != OVERLAY_NONE && OverlayTypes[Map[adjacent].Overlay]->IsExplosive) {
				new AnimClass(AnimTypes[AnimTypeClass::From_Name("FIRE3")], adjacent, Random_Pick(1, 3) + 3);
			}
		}
	}

	if (strength > warhead->DeformThreshhold) {
		if (Percent_Chance((strength * 0.01) * warhead->Deform * 100.0) && !(Map[coord].IsUnderBridge && coord.Z >= BRIDGE_LEPTON_HEIGHT + Map.Get_Height_GL(coord))) {
			Map.Deform_Terrain(coord.As_Cell(), false);
		}
	}

	if (cellptr->Is_Tile_Destroyable_Cliff()) {
		if (Percent_Chance(Rule->CollapseChance)) {
			Map.Collapse_Cliff(cellptr);
		}
	}

	if (warhead->Particle) {
		ParticleSystemClass * psys;
		if (warhead->Particle->BehavesLike == PSYS_BEHAVIOR_GAS) {
			psys = GasSystem;
		} else {
			psys = new ParticleSystemClass(warhead->Particle, coord);
		}
		psys->Spawn_Held_Particle(coord, coord);
	}

	// NATIVE: TibSun's ice cracking left out (no ice in YR).
	return(0);
}
