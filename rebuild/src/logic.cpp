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

/* $Header: /CounterStrike/LOGIC.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LOGIC.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 27, 1993                                           *
 *                                                                                             *
 *                  Last Update : July 30, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   LogicClass::AI -- Handles AI logic processing for game objects.                           *
 *   LogicClass::Debug_Dump -- Displays logic class status to the mono screen.                 *
 *   LogicClass::Detach -- Detatch the specified target from the logic system.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "logic.h"

#include "_bench.h"
#include "_map.h"
#include "_rules.h"
#include "_tactica.h"
#include "_vanim.h"
#include "aircraft.h"
#include "alphashp.h"
#include "anim.h"
#include "bench.h"
#include "blight.h"
#include "building.h"
#include "bullet.h"
#include "empulse.h"
#include "factory.h"
#include "globals.h"
#include "incdec.h"
#include "infantry.h"
#include "ion.h"
#include "ionblast.h"
#include "laser.h"
#include "light.h"
#include "mono.h"
#include "object.h"
#include "ovrlight.h"
#include "partsys.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "tactical.h"
#include "tag.h"
#include "terrain.h"
#include "tiberium.h"
#include "unit.h"
#include "vanim.h"
#include "vein.h"
#include "wave.h"

#include "bomblist.h"
#include "kamikaze.h"
#include "radsite.h"
#include "detail.h"
#include "fpscounter.h"
#include "bench.hh"




/*
 * Global COM reference count.
 */
ULONG COMRefCount = 0;


unsigned FramesThisSecond=0;
unsigned FPSCounter::CurrentFrameRate = 0; // RA2: TibSun's LastFramesPerSecond
unsigned TotalFrames=0;
unsigned SecondsPassed=0;


/// <summary>
/// RA2
/// </summary>
void __fastcall Set_Frame_Rate_Limit(int value) // RA2: not in TibSun, at 0x53D820
{
	Detail::MinFrameRate = value;
}


/// <summary>
/// RA2
/// </summary>
void __fastcall Set_Buffer_Zone_Width(int value) // RA2: at 0x53D830 (name chosen here)
{
	Detail::BufferZoneWidth = value;
}


/// <summary>
/// RA2
/// </summary>
unsigned Detail::GetMinFrameRate(void) // RA2: not in TibSun, at 0x53D840
{
	if (FPSCounter::ReducedEffects) {
		if (FPSCounter::CurrentFrameRate >= Detail::MinFrameRate + Detail::BufferZoneWidth) {
			FPSCounter::ReducedEffects = false;
			return Detail::MinFrameRate;
		}
		return Detail::MinFrameRate + Detail::BufferZoneWidth;
	}
	if (FPSCounter::CurrentFrameRate < Detail::MinFrameRate) {
		FPSCounter::ReducedEffects = true;
		return Detail::MinFrameRate + Detail::BufferZoneWidth;
	}
	return Detail::MinFrameRate;
}


/***********************************************************************************************
 * LogicClass::AI -- Handles AI logic processing for game objects.                             *
 *                                                                                             *
 *    This routine is used to perform the AI processing for all game objects. This includes    *
 *    all houses, factories, objects, and teams.                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/29/1994 JLB : Created.                                                                 *
 *   12/17/1994 JLB : Must perform one complete pass rather than bailing early.                *
 *   12/23/1994 JLB : Ensures that no object gets skipped if it was deleted.                   *
 *=============================================================================================*/
void LogicClass::AI(void)
{
	FramesThisSecond++;

	/*
	**	Spring the triggers whose events this frame may have satisfied.
	*/
	for (LogicTriggerID = 0; LogicTriggerID < LogicTags.Count(); LogicTriggerID++) {
		TagClass * tag = LogicTags[LogicTriggerID];

		if (Scen->IsCrateBeenPickedUp && tag->Spring(TEVENT_PICKUP_CRATE_ANY)) continue;
		if (Scen->IsGlobalChanged) {
			if (tag->Spring(TEVENT_GLOBAL_SET)) continue;
			if (tag->Spring(TEVENT_GLOBAL_CLEAR)) continue;
			if (tag->Spring(TEVENT_LOCAL_SET)) continue;
			if (tag->Spring(TEVENT_LOCAL_CLEAR)) continue;
		}
		if (Scen->IsAmbientLightChanged) {
			if (tag->Spring(TEVENT_AMBIENT_LESS_THAN)) continue;
			if (tag->Spring(TEVENT_AMBIENT_GREATER_THAN)) continue;
		}
		if (tag->Spring(TEVENT_TIME)) continue;
		if (tag->Spring(TEVENT_RANDOM_TIME)) continue;
		if (Scen->MissionTimer.Is_Active() && Scen->MissionTimer == 0) {
			tag->Spring(TEVENT_MISSION_TIMER_EXPIRED);
		}
	}

	/*
	**	An expired mission timer stops and the tab redraws.
	*/
	if (Scen->MissionTimer.Is_Active() && Scen->MissionTimer == 0) {
		Scen->MissionTimer.Stop();
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
	}

	Scen->IsGlobalChanged = false;
	Scen->IsBridgeChanged = false;
	Scen->IsAmbientLightChanged = false;
	Scen->IsCrateBeenPickedUp = false;

	/*
	**	Shroud regrowth.
	*/
	if (Rule->IsShroudGrow && Rule->ShroudRate != 0 && Scen->ShroudTimer == 0) {
		Scen->ShroudTimer = Rule->ShroudRate * TICKS_PER_MINUTE;
		Map.Encroach_Shadow();
	}

	if (Frame % 120 == 0) {
		Map.Refresh_Shroud();
	}

	/*
	**	Fog regrowth.
	*/
	if (Scen->Special.IsFogOfWar && Rule->FogRate != 0 && Scen->FogTimer == 0) {
		Scen->FogTimer = Rule->FogRate * TICKS_PER_MINUTE;
		Map.Encroach_Fog();
	}

	/*
	**	The ambient light steps toward the level it is changing to.
	*/
	if (Scen->DesiredAmbientLight != Scen->CurrentAmbientLight && Rule->AmbientLightChangeRate != 0 && Scen->AmbientChangeTimer == 0) {
		if (NukeFlash::IsFadingIn() || NukeFlash::IsFadingOut()) {
			Scen->AmbientChangeTimer = Scen->NukeAmbientChangeRate;
		} else {
			Scen->AmbientChangeTimer = Rule->AmbientLightChangeRate * TICKS_PER_MINUTE;
		}
		Scen->DesiredAmbientLight = Scen->DesiredAmbientLight > 0 ? Scen->DesiredAmbientLight : 0;
		int step = Rule->AmbientLightChangeStep * 100;
		if (Scen->DesiredAmbientLight < Scen->CurrentAmbientLight) {
			Scen->CurrentAmbientLight -= step;
			Scen->CurrentAmbientLight = Scen->CurrentAmbientLight > Scen->DesiredAmbientLight ? Scen->CurrentAmbientLight : Scen->DesiredAmbientLight;
		} else {
			Scen->CurrentAmbientLight += step;
			Scen->CurrentAmbientLight = Scen->CurrentAmbientLight < Scen->DesiredAmbientLight ? Scen->CurrentAmbientLight : Scen->DesiredAmbientLight;
		}
		Scen->IsAmbientLightChanged = true;
		Map.Update_Cell_Colors();
		Map.Flag_To_Redraw(GS_REDRAW_TACTICAL);
	}

	TiberiumClass::Tiberium_Growth();
	TiberiumClass::Tiberium_Spread();
	BombList.Update();
	Kamikazes.Update();

	/*
	**	Teams may disband while they think: work from a copy of the list.
	*/
	DynamicVectorClass<TeamClass *> teams;
	int index;
	for (index = 0; index < Teams.Count(); index++) {
		teams.Add(Teams[index]);
	}
	for (index = 0; index < teams.Count(); index++) {
		teams[index]->AI();
	}

	SpotLightClass::Update_All();
	LaserDrawClass::Update_All();
	IonStormClass::AI();

	for (index = RadSites.Count() - 1; index >= 0; index--) {
		RadSites[index]->AI();
	}

	LightSourceClass::Process_Lighting(6);
	EMPulseClass::Update_All();

	/*
	**	Every object in the logic layer gets its turn.
	*/
	for (index = 0; index < Count(); index++) {
		(*this)[index]->AI();
	}

	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
		for (index = 0; index < MoveFlashes.Count(); index++) {
			MoveFlashes[index]->AI();
		}
	}

	IonBlastClass::Update_All();
	AlphaShapeClass::Update_All();
	Map.Logic();
	TacticalMap->AI();

	for (index = 0; index < Factories.Count(); index++) {
		Factories[index]->AI();
	}

	for (index = 0; index < Houses.Count(); index++) {
		HouseClass * house = Houses[index];
		if (house != NULL) {
			house->AI();
		}
	}

	/*
	**	The view follows its object.
	*/
	if (Map.Object_To_Follow() != NULL) {
		TacticalMap->Set_Tactical_Position(Map.Object_To_Follow()->Get_Coord());
	}
}


/// <summary>
/// Handles the environmental side of the game logic frame.
/// This routine paces shroud regrowth, fog of war spread and ice growth against their
/// rule driven timers, lets the map settle its ice and terrain deformation, and then
/// gives each foot object's locomotor its chance to process.
/// </summary>
void LogicClass::Environment_AI(void)
{
	if (Rule->IsShroudGrow && Rule->ShroudRate != 0 && Scen->ShroudTimer == 0) {
		Scen->ShroudTimer = TICKS_PER_MINUTE * Rule->ShroudRate;
		Map.Encroach_Shadow();
	}

	if (Scen->Special.IsFogOfWar && Rule->FogRate != 0 && Scen->FogTimer == 0) {
		Scen->FogTimer = TICKS_PER_MINUTE * Rule->FogRate;
		Map.Encroach_Fog();
	}

	for (int index = 0; index < Count(); index++) { // RA2: Environment_AI at 0x53DFB0 has no Scen, Theater, Rule and 3 more
		FootClass *foot = (FootClass *)(*this)[index];
		if (foot->Is_Foot() && foot->Locomotion != NULL && !foot->IsSinking) {
			foot->Locomotion->Process();
		}
	}
}


/***********************************************************************************************
 * LogicClass::Detach -- Detatch the specified target from the logic system.                   *
 *                                                                                             *
 *    This routine is called when the specified target object is about to be removed from the  *
 *    game system and all references to it must be severed. The only thing that the logic      *
 *    system looks for in this case is to see if the target refers to a trigger and if so,     *
 *    it scans through the trigger list and removes all references to it.                      *
 *                                                                                             *
 * INPUT:   target   -- The target to remove from the sytem.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void LogicClass::Detach(AbstractClass const * target, bool all)
{
	if (Is_Target_Tag(target)) {
		for (int index = 0; index < LogicTags.Count(); index++) {
			if (target == (AbstractClass *)LogicTags[index]) {
				LogicTags.Delete_Index(index);
				index--;
			}
		}
	}
}


#ifdef _DEBUG
/***********************************************************************************************
 * LogicClass::Debug_Dump -- Displays logic class status to the mono screen.                   *
 *                                                                                             *
 *    This is a debugging support routine. It displays the current state of the logic class    *
 *    to the monochrome monitor. It assumes that it is being called once per second.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine only once per second.                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   01/26/1996 JLB : Prints game time value.                                                  *
 *=============================================================================================*/
void LogicClass::Debug_Dump(MonoClass * mono) const
{
	#define RECORDCOUNT	40
	#define RECORDHEIGHT 21
	static int _framecounter = 0;

	static bool first = true;
	if (first) {
		first = false;
		mono->Set_Cursor(0, 0);
	}

	_framecounter++;
	mono->Set_Cursor(0, 0);mono->Printf("%d", AllowVoice);
	mono->Set_Cursor(1, 1);mono->Printf("%ld", (int)Scen->MissionTimer);
	mono->Set_Cursor(10, 1);mono->Printf("%3d", FPSCounter::CurrentFrameRate);
	mono->Set_Cursor(1, 3);mono->Printf("%02d:%02d:%02d", Scen->MissionTimer / TICKS_PER_HOUR, (Scen->MissionTimer % TICKS_PER_HOUR)/TICKS_PER_MINUTE, (Scen->MissionTimer % TICKS_PER_MINUTE)/TICKS_PER_SECOND);

	mono->Set_Cursor(1, 11);mono->Printf("%3d", Units.Count());
	mono->Set_Cursor(1, 12);mono->Printf("%3d", Infantry.Count());
	mono->Set_Cursor(1, 13);mono->Printf("%3d", Aircraft.Count());
	mono->Set_Cursor(1, 15);mono->Printf("%3d", Buildings.Count());
	mono->Set_Cursor(1, 16);mono->Printf("%3d", Terrains.Count());
	mono->Set_Cursor(1, 17);mono->Printf("%3d", Bullets.Count());
	mono->Set_Cursor(1, 18);mono->Printf("%3d", Anims.Count());
	mono->Set_Cursor(1, 19);mono->Printf("%3d", Teams.Count());
	mono->Set_Cursor(1, 20);mono->Printf("%3d", Triggers.Count());
	mono->Set_Cursor(1, 21);mono->Printf("%3d", TriggerTypes.Count());
	mono->Set_Cursor(1, 22);mono->Printf("%3d", Factories.Count());

	SpareTicks = MIN((int)SpareTicks, (int)TIMER_SECOND);

	/*
	**	CPU utilization record.
	*/
	mono->Sub_Window(15, 1, 6, 11);
	mono->Scroll();
	mono->Set_Cursor(0, 10);
	mono->Printf("%3d%%", ((TIMER_SECOND-SpareTicks)*100) / TIMER_SECOND);

	/*
	**	Update the frame rate log.
	*/
	mono->Sub_Window(22, 1, 6, 11);
	mono->Scroll();
	mono->Set_Cursor(0, 10);

	/*
	**	Update the findpath calc record.
	*/
	mono->Sub_Window(50, 1, 6, 11);
	mono->Scroll();
	mono->Set_Cursor(0, 10);
	mono->Printf("%4d", PathCount);
	PathCount = 0;

	/*
	**	Update the cell redraw record.
	*/
	mono->Sub_Window(29, 1, 6, 11);
	mono->Scroll();
	mono->Set_Cursor(0, 10);
	mono->Printf("%5d", CellCount);
	CellCount = 0;

	/*
	**	Update the target scan record.
	*/
	mono->Sub_Window(36, 1, 6, 11);
	mono->Scroll();
	mono->Set_Cursor(0, 10);
	mono->Printf("%5d", TargetScan);
	TargetScan = 0;

	/*
	**	Sidebar redraw record.
	*/
	mono->Sub_Window(43, 1, 6, 11);
	mono->Scroll();
	mono->Set_Cursor(0, 10);
	mono->Printf("%5d", SidebarRedraws);
	SidebarRedraws = 0;

	/*
	**	Update the CPU utilization chart.
	*/
	mono->Sub_Window(15, 13, 63, 10);
	mono->Pan(1);
	mono->Sub_Window(15, 13, 64, 10);
	int graph = 0;//RECORDHEIGHT * fixed(TIMER_SECOND-SpareTicks, TIMER_SECOND);
	for (int row = 1; row < RECORDHEIGHT; row += 2) {
		static char _barchar[4] = {(char)' ', (char)220, (char)0, (char)219};
		char str[2];
		int index = 0;

		index |= (graph >= row) ? 0x01 : 0x00;
		index |= (graph >= row+1) ? 0x02: 0x00;

		str[1] = '\0';
		str[0] = _barchar[index];
		mono->Text_Print(str, 62, 9-(row/2));
	}
	mono->Sub_Window();

	SpareTicks = 0;
}
#endif



/// <summary>
/// Performs the per object thinking for one game logic frame.
/// This routine gives every object that has been submitted to a layer -- animations,
/// lights, bullets, buildings, aircraft, infantry, units, particle systems, terrain,
/// voxel animations and waves -- its chance to act. It is the companion to
/// LogicClass::AI, which handles the trigger and scenario side of the same frame.
/// </summary>
void Logic_Objects_AI(void) // RA2: not in TibSun (name chosen here)
{
	int index;
	for (index = Anims.Count() - 1; index >= 0; index--) {
		AnimClass * anim = Anims[index];
		if (anim->IsSubmittedToLayer) anim->AI();
	}

	for (index = BuildingLights.Count() - 1; index >= 0; index--) {
		BuildingLightClass * blight = BuildingLights[index];
		if (blight->IsSubmittedToLayer) blight->AI();
	}

	for (index = Bullets.Count() - 1; index >= 0; index--) {
		BulletClass * bullet = Bullets[index];
		if (bullet->IsSubmittedToLayer) bullet->AI();
	}

	for (index = Buildings.Count() - 1; index >= 0; index--) {
		BuildingClass * building = Buildings[index];
		if (building->IsSubmittedToLayer) building->AI();
	}

	for (index = Aircraft.Count() - 1; index >= 0; index--) {
		AircraftClass * aircraft = Aircraft[index];
		if (aircraft->IsSubmittedToLayer) aircraft->AI();
	}

	for (index = Infantry.Count() - 1; index >= 0; index--) {
		InfantryClass * infantry = Infantry[index];
		if (infantry->IsSubmittedToLayer) infantry->AI();
	}

	for (index = Units.Count() - 1; index >= 0; index--) {
		UnitClass * unit = Units[index];
		if (unit->IsSubmittedToLayer) unit->AI();
	}

	for (index = ParticleSystems.Count() - 1; index >= 0; index--) {
		ParticleSystemClass * partsys = ParticleSystems[index];
		if (partsys->IsSubmittedToLayer) partsys->AI();
	}

	for (index = Terrains.Count() - 1; index >= 0; index--) {
		TerrainClass * terrain = Terrains[index];
		if (terrain->IsSubmittedToLayer) terrain->AI();
	}

	for (index = VoxelAnims.Count() - 1; index >= 0; index--) {
		VoxelAnimClass * voxelanim = VoxelAnims[index];
		if (voxelanim->IsSubmittedToLayer) voxelanim->AI();
	}

	for (index = Waves.Count() - 1; index >= 0; index--) {
		WaveClass * wave = Waves[index];
		if (wave->IsSubmittedToLayer) wave->AI();
	}
}




/// <summary>
/// Submits the object to the logic system.
/// This routine is used to add an object to the list that the game logic ticks each
/// frame. An object that has already been submitted is accepted without complaint.
/// </summary>
/// <param name="sort">Should the object be inserted in sorted order?</param>
/// <returns>bool; Is the object now submitted to the logic system?</returns>
bool LogicClass::Submit(ObjectClass const * object, bool sort)
{
	if (object->IsSubmittedToLayer) {
		return(true);
	}

	if (BASECLASS::Submit(object, sort)) {
		((ObjectClass *)object)->IsSubmittedToLayer = true;
		return(true);
	}

	return(false);
}


/// <summary>
/// Removes the object from the logic system.
/// This routine is called when an object should no longer be given its chance to think.
/// An object that was never submitted is quietly left alone.
/// </summary>
void LogicClass::Remove(ObjectClass * object)
{
	if (object->IsSubmittedToLayer) {
		Delete(object);
		object->IsSubmittedToLayer = false;
	}
}

