// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y6): SuperClass::Launch (gamemd.exe 0x006CC390, 6,884 B), unmatched in
// ra2-decomp. The RA2 twin's SuperClass::Place (src/recovered/super.cpp, RA2 0x0069D850, exact
// there: the nuke, iron curtain, lightning storm, chronosphere, chrono warp and paradrops) in the YR
// tree's names, with YR's changes and its five new super weapons read from gamemd.exe's code: the
// chrono warp also lets parasites go, empties bunkers and spares units inside a war factory; the
// iron curtain spares immobilized feet; paradrops by side (Allied, Soviet, Yuri); the psychic
// dominator, spy plane, genetic mutator, force shield and psychic reveal call the tree's matched
// helpers. Not compared byte for byte; behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
#include "airctype.h"
#include "revent.h"
#include "parasite.h"
#include "foot.h"
#include "ilocos.h"
#include "ipiggy.h"
#include "loco.h"
#include "building.h"
#include "builtype.h"
#include "weapon.h"
#include "bullet.h"
#include "voc.h"
#include "combat.h"

namespace Unsorted { extern int MuteSWLaunches; }
void __fastcall FUN_00539EB0(int duration, int deferment, Cell cell, HouseClass * owner);	// LightningStorm::Start
void __fastcall FUN_0053AE50(HouseClass * owner, Cell cell);	// PsyDom::Start
int __fastcall FUN_00753250(char const * name);	// the speech index of a name
int __fastcall Create_Paradrop_Planes(HouseClass * house, int air, int number, MissionType mission, AbstractClass * tarcom, AbstractClass * navcom, int passenger, int passengers);
int __fastcall Create_Spy_Planes(HouseClass * house, int air, int number, MissionType mission, AbstractClass * tarcom, AbstractClass * navcom);

static inline Coord Launch_Cell_Coord(Cell const & cell)
{
	Coord coord = Map[cell].Center_Coord();
	if (Map[cell].IsUnderBridge) {
		coord.Z += BRIDGE_LEPTON_HEIGHT;
	}
	return(coord);
}

static inline ObjectClass * Launch_Occupier(Cell const & cell)
{
	CellClass & cellptr = Map[cell];
	return(cellptr.Cell_Occupier(cellptr.IsUnderBridge));
}

static inline void Launch_Ready_Silence(bool player, char const * ready)
{
	if (player) {
		DAT_008809A0 = -1;
		VoxClass::SilenceIndex(FUN_00753250(ready));
	}
}

// A paradrop of each listed infantry type (with its count) to the cell, or the nearest land cell.
static void Launch_Paradrop(HouseClass * house, Cell const & cell, TypeList<InfantryTypeClass const *> const & types, TypeList<int> const & counts, bool check_counts)
{
	int plane = AircraftTypeClass::From_Name("PDPLANE");
	CellClass * cellptr = &Map[cell];
	if (cellptr == NULL || cellptr == &BlubCell) return;
	if (cellptr->Is_Tile_Water()) {
		Cell spot = Map.Nearby_Location(cell, SPEED_FOOT, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false, false);
		if (spot != CELL_NONE) {
			CellClass * nearptr = &Map[spot];
			if (nearptr != NULL && nearptr != &BlubCell && !nearptr->Is_Tile_Water()) {
				cellptr = nearptr;
			}
		}
	}
	if (check_counts && types.Count() != counts.Count()) return;
	for (int i = 0; i < types.Count() && i < counts.Count(); i++) {	// NATIVE: the count list's bound (the Soviet list is not checked)
		int type = types[i]->HeapID;
		if (plane != -1 && type != -1) {
			Create_Paradrop_Planes(house, plane, 1, (MissionType)0x1A, cellptr, NULL, type, counts[i]);
		}
	}
}

void SuperClass::Launch(Cell const & cell, bool player)
{
	int i;

	switch (Class->Type) {

		case SUPER_CHRONO_SPHERE:
			if (!IsReady) break;
			ChronoMapCoords = cell;
			CreateChronoAnim(Launch_Cell_Coord(cell) + Coord(0, 0, 5));
			if (player) {
				DAT_008809A0 = SUPER_CHRONO_WARP;
				return;
			}
			if (Animation != NULL) {
				Animation->Invisible = true;	// gamemd.exe: AnimClass +0x19D
				Animation->StartSoundAudioController.Stop();
			}
			break;

		case SUPER_CHRONO_WARP: {
			RadarEventClass::Create(RADAREVENT_SUPERWEAPON_ACTIVATED, ChronoMapCoords);
			RadarEventClass::Create(RADAREVENT_SUPERWEAPON_ACTIVATED, cell);
			Coord dest = Launch_Cell_Coord(cell);
			Coord from = Launch_Cell_Coord(ChronoMapCoords);
			DetachAnimation();
			new AnimClass(Rule->ChronoBlastDest, dest + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), 0);
			new AnimClass(Rule->ChronoBlast, from + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), 0);

			for (i = 0; i < 9; i++) {
				Cell source = ChronoMapCoords + DAT_00B0C038[i];
				ObjectClass * next;
				for (ObjectClass * object = Launch_Occupier(source); object != NULL; object = next) {
					next = object->Next;
					if (!(object->AbstractFlags & 4) || object->In_Air()) continue;
					FootClass * foot = (FootClass *)object;

					if (foot->ParasiteEatingMe != NULL && foot->ParasiteEatingMe->TClass->Organic && foot->ParasiteEatingMe->ParasiteImUsing != NULL) {
						foot->ParasiteEatingMe->ParasiteImUsing->ExitUnit();	// NATIVE: the 500-frame grapple timer gamemd.exe sets first is left out
					}

					bool in_factory = false;
					if (object->RTTI == RTTI_UNIT) {
						TechnoClass * contact = foot->FUN_0065AD30(0);
						if (contact != NULL && contact->RTTI == RTTI_BUILDING && ((BuildingClass *)contact)->Class->IsWeaponsFactory
								&& Map[object->Get_Coord()].Cell_Building() == contact) {
							in_factory = true;
						}
					}

					if (foot->TClass->Organic && !foot->TClass->Teleporter && !object->In_Air()) {
						int damage = foot->TClass->MaxStrength;
						object->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false, NULL);
					} else if (!foot->IsIronCurtained() && !foot->IsImmobilized && !foot->IsBeingWarpedOut() && !foot->IsWarpingIn() && !in_factory) {
						if (foot->BunkerLinkedItem != NULL && foot->BunkerLinkedItem->RTTI == RTTI_BUILDING) {
							((BuildingClass *)foot->BunkerLinkedItem)->ClearBunker();
						}
						IPiggybackPtr piggy(foot->Locomotion);
						ILocomotionPtr teleport(LocomotionClass::CLSIDs::Teleport);
						bool unit = object->RTTI == RTTI_UNIT;
						Coord coord = Launch_Cell_Coord(cell + DAT_00B0C038[i]);
						if (unit) {
							foot->Locomotion->Mark_All_Occupation_Bits(0);
							foot->Locomotion->Force_Track(-1, coord);
							foot->Clear_Occupy_Bit(coord);
							foot->IsOccupyingCell = true;
						}
						teleport->Link_To_Object(object);
						piggy = teleport;
						if (piggy != NULL) {
							piggy->Begin_Piggyback(foot->Locomotion);
							foot->Locomotion = teleport;
						}
						Coord delta = object->Center_Coord() - Map[ChronoMapCoords].Center_Coord();
						if (!unit) {
							coord = delta + Map[cell].Center_Coord();
							if (Map[coord].IsUnderBridge) {
								coord.Z += BRIDGE_LEPTON_HEIGHT;
							}
						}
						foot->IsImmobilized = true;
						(Coord &)foot->ChronoDestCoords = coord;
						foot->Transmit_Message(RADIO_OVER_OUT);
						foot->ChronoWarpedByHouse = House;
						foot->Assign_Destination(&Map[cell + DAT_00B0C038[i]], true);
					}
				}
			}
			if (!Unsorted::MuteSWLaunches) {
				VoxClass::Play("EVA_ChronosphereActivated");
			}
			Launch_Ready_Silence(player, "EVA_ChronosphereReady");
			break;
		}

		case SUPER_LIGHTNING_STORM:
			if (!IsReady) break;
			FUN_00539EB0(Rule->LightningStormDuration, Rule->LightningDeferment, cell, House);
			if (!Unsorted::MuteSWLaunches) {
				VoxClass::Play("EVA_LightningStormCreated");
			}
			Launch_Ready_Silence(player, "EVA_LightningStormReady");
			break;

		case SUPER_PSYCHIC_DOMINATOR:
			if (!IsReady) break;
			RadarEventClass::Create(RADAREVENT_SUPERWEAPON_ACTIVATED, cell);
			FUN_0053AE50(House, cell);
			if (!Unsorted::MuteSWLaunches) {
				VoxClass::Play("EVA_PsychicDominatorActivated");
			}
			VocClass::PlayIndexAtPos(Rule->PsychicDominatorActivateSound, Map[cell].Center_Coord(), 0);
			Launch_Ready_Silence(player, "EVA_PsychicDominatorReady");
			break;

		case SUPER_IRON_CURTAIN: {
			if (!IsReady) break;
			Coord coord = Launch_Cell_Coord(cell);
			new AnimClass(Rule->IronCurtainInvokeAnim, coord + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), 0);
			if (!Unsorted::MuteSWLaunches) {
				VoxClass::Play("EVA_IronCurtainActivated");
			}
			RadarEventClass::Create(RADAREVENT_SUPERWEAPON_ACTIVATED, cell);
			for (i = 0; i < 9; i++) {
				for (ObjectClass * object = Launch_Occupier(cell + DAT_00B0C038[i]); object != NULL; object = object->Next) {
					if (!(object->AbstractFlags & 4) || !((TechnoClass *)object)->IsImmobilized) {
						object->IronCurtain(Rule->IronCurtainDuration, House, false);
					}
				}
			}
			Launch_Ready_Silence(player, "EVA_IronCurtainReady");
			break;
		}

		case SUPER_FORCE_SHIELD: {
			if (!IsReady) break;
			Coord coord = Launch_Cell_Coord(cell);
			new AnimClass(Rule->ForceShieldInvokeAnim, coord + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), 0);
			SpecialSoundDuration = Rule->ForceShieldDuration - Rule->ForceShieldPlayFadeSoundTime;
			SpecialSoundLocation[0] = coord.X;
			SpecialSoundLocation[1] = coord.Y;
			SpecialSoundLocation[2] = coord.Z;
			if (Class->StartSound != -1) {
				VocClass::PlayAt(Class->StartSound, coord, NULL);
			}
			House->CreatePowerOutage(Rule->ForceShieldBlackoutDuration);
			Coord center = Map[cell].Center_Coord();
			int radius = Rule->ForceShieldRadius * CELL_LEPTON_W;
			if (center != COORD_NONE) {
				for (i = Buildings.Count() - 1; i >= 0; i--) {
					BuildingClass * building = Buildings[i];
					if (building->House->Is_Ally(House) && (center - building->Center_Coord()).Length() < radius) {
						building->IronCurtain(Rule->ForceShieldDuration, House, true);
					}
				}
			}
			Launch_Ready_Silence(player, "EVA_ForceShieldReady");
			break;
		}

		case SUPER_PARA_DROP:
			if (!IsReady) break;
			if (House->ActLike == 0) {
				Launch_Paradrop(House, cell, Rule->AllyParaDropInf, Rule->AllyParaDropNum, true);
			} else if (House->ActLike == 2) {
				Launch_Paradrop(House, cell, Rule->YuriParaDropInf, Rule->YuriParaDropNum, true);
			} else {
				Launch_Paradrop(House, cell, Rule->SovParaDropInf, Rule->SovParaDropNum, false);
			}
			Launch_Ready_Silence(player, "EVA_ReinforcementsReady");
			break;

		case SUPER_AMER_PARA_DROP:
			if (!IsReady) break;
			Launch_Paradrop(House, cell, Rule->AmerParaDropInf, Rule->AmerParaDropNum, true);
			Launch_Ready_Silence(player, "EVA_ReinforcementsReady");
			break;

		case SUPER_SPY_PLANE: {
			if (!IsReady) break;
			int plane = AircraftTypeClass::From_Name("SPYP");
			CellClass * cellptr = &Map[cell];
			if (cellptr != NULL && cellptr != &BlubCell && Rule->AllyParaDropInf.Count() == Rule->AllyParaDropNum.Count()) {
				for (i = 0; i < Rule->AllyParaDropInf.Count(); i++) {	// gamemd.exe: one spy plane per Allied paradrop entry
					if (plane != -1) {
						Create_Spy_Planes(House, plane, 1, (MissionType)0x1E, cellptr, NULL);
					}
				}
			}
			Launch_Ready_Silence(player, "EVA_SpyPlaneReady");
			break;
		}

		case SUPER_PSYCHIC_REVEAL: {
			if (!IsReady) break;
			Coord coord = Map[cell].Center_Coord();
			Map.RevealArea2(&coord, Rule->PsychicRevealRadius, House, false, false, false, false, false);
			Map.RevealArea2(&coord, Rule->PsychicRevealRadius, House, false, false, false, false, true);
			VocClass::PlayIndexAtPos(Rule->PsychicRevealActivateSound, Map[cell].Center_Coord(), 0);
			Launch_Ready_Silence(player, "EVA_PsychicRevealReady");
			break;
		}

		case SUPER_GENETIC_CONVERTER: {
			if (!IsReady) break;
			Coord coord = Launch_Cell_Coord(cell);
			new AnimClass(Rule->IonBlast, coord + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), 0);	// gamemd.exe: Rule +0x298
			if (!Unsorted::MuteSWLaunches) {
				VoxClass::Play("EVA_GeneticMutatorActivated");
			}
			VocClass::PlayIndexAtPos(Rule->GeneticMutatorActivateSound, coord, 0);
			RadarEventClass::Create(RADAREVENT_SUPERWEAPON_ACTIVATED, cell);
			if (Rule->MutateExplosion) {
				Explosion_Damage(coord, 10000, NULL, Rule->MutateExplosionWarhead, false, House);
			} else {
				for (i = 0; i < 9; i++) {
					ObjectClass * next;
					for (ObjectClass * object = Launch_Occupier(cell + DAT_00B0C038[i]); object != NULL; object = next) {
						next = object->Next;
						if (object->RTTI == RTTI_INFANTRY) {
							int damage = ((TechnoClass *)object)->TClass->MaxStrength;
							object->Take_Damage(damage, 0, Rule->MutateWarhead, NULL, true, false, House);
						}
					}
				}
			}
			Launch_Ready_Silence(player, "EVA_GeneticMutatorReady");
			break;
		}

		case SUPER_MULTI_MISSILE: {
			if (!IsReady) break;
			if (IsOneTime && IsPresent) {
				Coord coord(cell.X * CELL_LEPTON_W + CELL_LEPTON_W / 2, cell.Y * CELL_LEPTON_H + CELL_LEPTON_H / 2, 0);
				coord.Z = Map.Get_Height_GL(coord);
				if (Map[cell].IsUnderBridge) {
					coord.Z += BRIDGE_LEPTON_HEIGHT;
				}
				WeaponTypeClass * payload = Weapons[WeaponTypeClass::From_Name("NukePayload")];
				CellClass * target = &Map[coord];
				Coord launch(coord.X, coord.Y, coord.Z + 20000);
				BulletClass * bullet = Create_Bullet(payload->Bullet, target, NULL, payload->Attack, payload->WarheadPtr, payload->MaxSpeed, false);
				if (bullet != NULL) {
					bullet->Set_Weapon_Type(payload);
				}
				HouseClass::PsiWarn(House, target, bullet, "PSIWARN");
				if (bullet != NULL) {
					double angle = 1.5707483884323126;
					double vz = fastmath::sin(angle) * -100.0;
					double vx = fastmath::cos(angle) * fastmath::cos(angle) * -100.0;
					double vy = fastmath::sin(angle) * fastmath::cos(angle) * -100.0;
					TVelocity3D<double> velocity;
					velocity.X = vx;
					velocity.Y = vy;
					velocity.Z = vz;
					bullet->Unlimbo(launch, velocity);
					if (!Unsorted::MuteSWLaunches) {
						VoxClass::Play("EVA_NuclearMissileLaunched");
					}
					VocClass::PlayIndexAtPos(Rule->DigSound, coord, 0);	// gamemd.exe: Rule +0x174
				}
				Launch_Ready_Silence(player, "EVA_NuclearMissileReady");
				House->IsRecalcNeeded = true;
				return;
			}
			for (i = 0; i < BuildingTypes.Count(); i++) {
				BuildingTypeClass * btype = BuildingTypes[i];
				if (btype->IsNukeSilo && (btype->SuperWeapon == Class->HeapID || btype->SuperWeapon2 == Class->HeapID)) {
					BuildingClass * site = House->Find_Building((StructType)i, (ZoneType)-1);
					if (site != NULL) {
						Coord coord(cell.X * CELL_LEPTON_W + CELL_LEPTON_W / 2, cell.Y * CELL_LEPTON_H + CELL_LEPTON_H / 2, 0);
						coord.Z = Map.Get_Height_GL(coord);
						site->QueueMission((MissionType)0x16, false);
						site->Commence();
						House->NukeDest = cell;
						site->LastSuperWeaponIndex = Class->Type;	// gamemd.exe: BuildingClass +0x5F8
						VocClass::PlayIndexAtPos(Rule->DigSound, coord, 0);
						if (!Unsorted::MuteSWLaunches) {
							VoxClass::Play("EVA_NuclearMissileLaunched");
						}
					}
					break;
				}
			}
			Launch_Ready_Silence(player, "EVA_NuclearMissileReady");
			House->IsRecalcNeeded = true;
			break;
		}

		default:
			break;
	}
}

// SuperClass::Impatient_Click, YR 0x006CC070 (lane Y7): an empty body in gamemd.exe; adopted from
// ra2-decomp (commit 0bafad686c6ac1f7cf29777cb291d6002ebbcfd7, src/super.cpp, an exact match). Called by
// SelectClass::Action (sidebar_native.inc) when a super weapon's icon is tapped before it is ready.
// FUNCTION: GAMEMD 0x006CC070
void SuperClass::Impatient_Click(void) const
{
}
