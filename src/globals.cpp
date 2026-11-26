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

/*
**	Command & Conquer Red Alert 2
**	GLOBALS.CPP: the game's global objects that have constructors, in the order the original
**	constructs them (its .CRT$XCU initializer table, 0x004D2D90-0x004D4920). The plain globals
**	are not defined here yet.
**
**	Text adapted from TibSun's globals.cpp and ra2-decomp's src/globals.cpp (the same objects
**	in the same order for Yuri's Revenge); Red Alert 2 has no RGB colour globals here, no
**	second building list before MoveFlashes, and TimedSuperWeapons where they have ShowTimers.
*/

#define INCLUDE_COM
#include "always.h"

/// create all com interfaces here
#include "iblowfish.h"
#include "iblowfish_i.c"
#include "sun.h"
#include "isun_i.c"
#include "ilocos.h"
#include "ilocos_i.c"
#include "irtti.h"
#include "irtti_i.c"
#include "iaihouse.h"
#include "iaihouse_i.c"
#include "ipublichouse.h"
#include "ipublichouse_i.c"
#include "ipiggy.h"
#include "ipiggy_i.c"
#include "iswizzle.h"
#include "iswizzle_i.c"
#include "iblockci.h"
#include "iblockci_i.c"
#include "iflyctrl.h"
#include "iflyctrl_i.c"
#undef INCLUDE_COM

#include "geometry.h"
#include "rect.h"
#include "vector.h"
#include "globals.h"

#include "_voxel.h"
#include "app.h"
#include "credits.h"
#include "event.h"
#include "goptions.h"
#include "index.h"
#include "ipxmgr.h"
#include "mission.h"
#include "nullmgr.h"
#include "playcd.h"
#include "queue.h"
#include "rndstraw.h"
#include "session.h"
#include "special.h"
#include "super.h"
#include "tag.h"
#include "theme.h"
#include "version.h"

DynamicVectorClass<AITriggerTypeClass *> AITriggerTypes;
DynamicVectorClass<TriggerTypeClass *> TriggerTypes;
DynamicVectorClass<TriggerClass *> Triggers;
DynamicVectorClass<BuildingLightClass *> BuildingLights;
DynamicVectorClass<ParticleSystemClass *> ParticleSystems;
DynamicVectorClass<TubeClass *> Tubes;
DynamicVectorClass<WaveClass *> Waves;
DynamicVectorClass<BulletClass *> Bullets;
DynamicVectorClass<OverlayClass *> Overlays;
DynamicVectorClass<ParticleClass *> Particles;
DynamicVectorClass<SmudgeClass *> Smudges;
DynamicVectorClass<TerrainClass *> Terrains;
DynamicVectorClass<InfantryClass *> Infantry;
DynamicVectorClass<BuildingClass *> Buildings;
DynamicVectorClass<AircraftClass *> Aircraft;
DynamicVectorClass<UnitClass *> Units;
DynamicVectorClass<HouseClass *> Houses;
DynamicVectorClass<AnimClass *> Anims;
DynamicVectorClass<TeamClass *> Teams;
DynamicVectorClass<FactoryClass *> Factories;
DynamicVectorClass<CampaignClass *> Campaigns;
DynamicVectorClass<SideClass *> Sides;
DynamicVectorClass<HouseTypeClass *> HouseTypes;
DynamicVectorClass<TaskForceClass *> TaskForces;
DynamicVectorClass<TeamTypeClass *> TeamTypes;
DynamicVectorClass<ScriptTypeClass *> ScriptTypes;
DynamicVectorClass<OverlayTypeClass *> OverlayTypes;
DynamicVectorClass<SmudgeTypeClass *> SmudgeTypes;
DynamicVectorClass<TerrainTypeClass *> TerrainTypes;
DynamicVectorClass<BuildingTypeClass *> BuildingTypes;
DynamicVectorClass<AircraftTypeClass *> AircraftTypes;
DynamicVectorClass<UnitTypeClass *> UnitTypes;
DynamicVectorClass<AnimTypeClass *> AnimTypes;
DynamicVectorClass<InfantryTypeClass *> InfantryTypes;
DynamicVectorClass<BulletTypeClass *> BulletTypes;
DynamicVectorClass<VoxelAnimTypeClass *> VoxelAnimTypes;
DynamicVectorClass<IsometricTileTypeClass *> IsometricTileTypes;
DynamicVectorClass<ParticleTypeClass *> ParticleTypes;
DynamicVectorClass<ParticleSystemTypeClass *> ParticleSystemTypes;
DynamicVectorClass<SuperWeaponTypeClass *> SuperWeaponTypes;
DynamicVectorClass<SuperClass *> SuperWeapons;
DynamicVectorClass<SuperClass *> TimedSuperWeapons;
DynamicVectorClass<TechnoClass *> Technos;
DynamicVectorClass<TechnoTypeClass *> TechnoTypes;
DynamicVectorClass<ObjectClass *> Objects;
DynamicVectorClass<AbstractTypeClass *> AbstractTypes;
DynamicVectorClass<AnimClass *> MoveFlashes;

TiberianSunClassFactory TibSunFactory;
MissionControlClass MissionControl[MISSION_COUNT];
RandomStraw CryptRandom;
DynamicVectorClass<ObjectClass *> CurrentObject;
VersionClass VerNum;
VoxelDataStruct DropPodVoxel;
GameOptionsClass Options;
ThemeClass Theme;
CreditClass CreditDisplay;
QueueClass<EventClass, MAX_EVENTS> OutList;
QueueClass<EventClass, (MAX_EVENTS * 128)> DoList;
QueueClass<EventClass, (MAX_EVENTS * 2)> MirrorList;
DynamicVectorClass<TagClass *> MapTags;
DynamicVectorClass<TagClass *> LogicTags;
SessionClass Session;
NullModemClass NullModem(16, 16, 0xEC, 0x1234);
IPXManagerClass Ipx(0x1C7, 0x22E, 0xA0, 0x20, 0x8813, 0xAA03);
GetCDClass CDList;
