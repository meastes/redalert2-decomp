
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4, redone by lane Y6), appended to the combat unit: the area damage YR
// calls Explosion_Damage, MapClass::DamageArea and FUN_00489280 (one function, 0x00489280, 4,710
// bytes, unmatched in ra2-decomp; the other names are bound to this one in data/name_aliases.tsv).
// Lane Y6 compared lane Y4's TibSun text with gamemd.exe: YR damages everything within the
// warhead's spread (SpreadFactor, the cells within that radius, flying objects through the aircraft
// tracker), with the RA2 twin's iron-curtain block, and passes the house on; TibSun hit one cell
// and its neighbours at a fixed 1.5 cells. The text is now the RA2 twin's (src/recovered/combat.cpp,
// RA2 0x00489280's twin, exact there) in the YR tree's names, keeping lane Y4's YR-named bridge
// and explosive-overlay parts; TibSun's terrain deforming, cliff collapse and fire spread (not in
// gamemd.exe's version) are left out. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
struct DamageGroup {
	ObjectClass * Target;
	int Distance;
};

// MapClass's ring tables (protected there): the cells within each radius, in rings.
struct Damage_Radius : MapClass {
	using MapClass::RadiusCount;
	using MapClass::RadiusOffset;
};

static inline bool Damage_Protected(ObjectClass * object)
{
	return(object != NULL && (object->AbstractFlags & 1) && ((TechnoClass *)object)->IsIronCurtained());
}

int Explosion_Damage(Coord const & coord, int strength, TechnoClass * source, WarheadTypeClass const * warhead, bool dochainreaction, HouseClass * house)
{
	int i;
	int distance;
	ObjectClass * object;
	DynamicVectorClass<DamageGroup *> groups;

	if (!strength || Scen->Special.IsInert || warhead == NULL) return(1);

	int range = (int)(warhead->SpreadFactor * 256.0f);
	Cell cell = coord.As_Cell();
	CellClass * cellptr = &Map[cell];
	bool isbridge = false;
	bool hit = false;
	bool blocked = false;
	bool close = warhead->SpreadFactor <= 0.5f;

	if (Map.Get_Height_GL(Coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0)) < coord.Z) {
		AircraftTracker.FillCurrentVector(cellptr, (int)warhead->SpreadFactor);
		FootClass * flier;
		while ((flier = AircraftTracker.Get()) != NULL) {
			if (flier->IsActive && flier->IsDown && flier->Strength > 0) {
				distance = (coord - flier->Get_Coord()).Length();
				if (distance <= range) {
					if (close && distance < 0x55 && flier->IsIronCurtained()) {
						blocked = true;
					}
					DamageGroup * group = new DamageGroup;
					group->Target = flier;
					group->Distance = distance;
					groups.Add(group);
				}
			}
		}
	}

	if (cellptr->IsUnderBridge && coord.Z > Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT / 2) {
		isbridge = true;
	}

	int rings = (int)(warhead->SpreadFactor + 0.99);
	if (rings > 10) rings = 10;	// NATIVE: RadiusCount's bound
	for (int m = 0; m < Damage_Radius::RadiusCount[rings]; m++) {
		Cell tcell = cell + Damage_Radius::RadiusOffset[m];
		CellClass * cptr = &Map[tcell];
		if (cptr->Overlay != OVERLAY_NONE) {
			OverlayTypeClass const * overlay = OverlayTypes[cptr->Overlay];
			if (overlay->IsChainReaction && (!overlay->IsTiberium || warhead->IsTiberiumDestroyer) && dochainreaction) {
				cptr->Reduce_Tiberium(strength / 10);
			}
			if (overlay->IsWall) {
				if (warhead->WallAbsoluteDestroyer) {
					cptr->Reduce_Wall(-1);
				} else if (warhead->IsWallDestroyer || (warhead->IsWoodDestroyer && overlay->Armor == ARMOR_WOOD)) {
					cptr->Reduce_Wall(strength);
				}
			}
			if (cptr->Overlay == OVERLAY_NONE) {
				TechnoClass::Remove_Target(cptr);
			}
		}

		for (object = cptr->Cell_Occupier(isbridge); object != NULL; object = object->Next) {
			if (object == source || !object->IsActive) continue;
			if (object->RTTI == RTTI_UNIT && Scen->Special.IsHarvesterImmune && Rule->HarvesterUnit.Is_In_List((UnitTypeClass *)object->Class_Of())) continue;
			DamageGroup * group = new DamageGroup;
			group->Target = object;
			if (object->RTTI == RTTI_BUILDING) {
				Coord center = cptr->Center_Coord();
				if (m == 0) {
					if (coord.Z - center.Z > LEVEL_LEPTON_H * 2) {
						group->Distance = (coord - center).Length() - LEVEL_LEPTON_H * 2;
					} else {
						group->Distance = 0;
					}
				} else {
					group->Distance = (coord - center).Length();
				}
			} else {
				group->Distance = (coord - object->Target_Coord()).Length();
			}
			if (close && m == 0 && Damage_Protected(object) && group->Distance < 0x55) {
				blocked = true;
			}
			groups.Add(group);
		}
	}

	for (int j = 0; j < groups.Count(); j++) {
		object = groups[j]->Target;
		distance = groups[j]->Distance;
		if (object->IsActive && (object->RTTI != RTTI_BUILDING || !((BuildingClass *)object)->Class->IsInvisibleInGame)) {
			if (!blocked || Damage_Protected(object)) {
				if (object->RTTI == RTTI_AIRCRAFT && object->In_Air()) {
					distance /= 2;
				}
				if (object->Strength > 0 && object->IsDown && !object->IsInLimbo && distance <= range) {
					int damage = strength;
					object->Take_Damage(damage, distance, warhead, source, false, false, house);
					hit = true;
				}
			}
		}
	}

	for (int k = 0; k < groups.Count(); k++) {
		delete groups[k];
	}
	groups.Clear();

	if (blocked) return(2);

	double rocking_force = MIN(strength * 0.01, 4.0);
	if (warhead->IsRocker && rocking_force > 0.3) {
		for (int x = cell.X - 3; x <= cell.X + 3; x++) {
			for (int y = cell.Y - 3; y <= cell.Y + 3; y++) {
				for (object = Map[Cell(x, y)].Cell_Occupier(isbridge); object != NULL; object = object->Next) {
					TechnoClass * techno = Dynamic_Cast<TechnoClass *>(object);
					if (techno == NULL) continue;
					if ((short)x == cell.X && (short)y == cell.Y && source) {
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
			}
		}
	}

	cellptr = &Map[cell];
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

	return(!hit);
}
