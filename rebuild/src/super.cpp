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

/* $Header: /CounterStrike/SUPER.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SUPER.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/28/95                                                     *
 *                                                                                             *
 *                  Last Update : October 11, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   SuperClass::AI -- Process the super weapon AI.                                            *
 *   SuperClass::Anim_Stage -- Fetches the animation stage for this super weapon.              *
 *   SuperClass::Discharged -- Handles discharged action for special super weapon.             *
 *   SuperClass::Enable -- Enable this super special weapon.                                   *
 *   SuperClass::Forced_Charge -- Force the super weapon to full charge state.                 *
 *   SuperClass::Impatient_Click -- Called when player clicks on unfinished super weapon.      *
 *   SuperClass::Recharge -- Starts the special super weapon recharging.                       *
 *   SuperClass::Remove -- Removes super weapon availability.                                  *
 *   SuperClass::SuperClass -- Constructor for special super weapon objects.                   *
 *   SuperClass::Suspend -- Suspend the charging of the super weapon.                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "super.h"

#include "_map.h"
#include "_rules.h"
#include "_weapon.h"
#include "building.h"
#include "builtype.h"
#include "bullet.h"
#include "ccrand.h"
#include "cell.h"
#include "crc.h"
#include "data.h"
#include "globals.h"
#include "house.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "inline.h"
#include "ionblast.h"
#include "language\language.h"
#include "mouse.h"
#include "rules.h"
#include "sun.h"
#include "suprtype.h"
#include "swizzle.h"
#include "tracker.h"
#include "unit.h"
#include "vector.h"
#include "vox.h"
#include "weapon.h"
#include "revent.h"	// RA2
#include "stringtable.h"	// RA2
#include "ion.h"	// RA2
#include "houstype.h"	// RA2
#include "ilocos.h"	// RA2
#include "loco.h"	// RA2
#include "ipiggy.h"	// RA2
#include "airctype.h"	// RA2
#include "reinf.h"	// RA2
#include "bullettype.h"	// RA2
#include "anim.h"	// RA2

// RA2: SuperClass in RA2's form, which shares none of YR's and few of TibSun's parts


Cell const CellAndNeighbors[9] = { // RA2: 0x00ABD7A0, built by the initializer at 0x0069C5A0: the cell itself, then its eight neighbours clockwise from east
	Cell(0, 0), // RA2: as above
	Cell(1, 0), // RA2: as above
	Cell(1, -1), // RA2: as above
	Cell(0, -1), // RA2: as above
	Cell(-1, -1), // RA2: as above
	Cell(-1, 0), // RA2: as above
	Cell(-1, 1), // RA2: as above
	Cell(0, 1), // RA2: as above
	Cell(1, 1) // RA2: as above
};


/// <summary>
/// Default constructor for the super weapon objects.
/// This routine creates a blank super weapon with no type and no owning house, and adds
/// it to the global super weapon list.
/// </summary>
SuperClass::SuperClass(void) :
	Class(NULL), // RA2: the timer and the old stage get no initializer
	House(NULL),
	NeedsBuilding(true),
	ChronoMapCoords(CELL_NONE), // RA2: copied from the constant at 0x00ABD768
	Animation(NULL), // RA2: as above
	AnimationGotInvalid(false), // RA2: as above
	IsPresent(false),
	IsOneTime(false),
	IsReady(false),
	IsSuspended(false),
	ChargeDrainState(SUSPENDED)
{
	UIName = L""; // RA2: the empty wide text at 0x00839D4C
	SuperWeapons.Add(this);
}


/***********************************************************************************************
 * SuperClass::SuperClass -- Constructor for special super weapon objects.                     *
 *                                                                                             *
 *    This is the constructor for the super weapons.                                           *
 *                                                                                             *
 * INPUT:   recharge    -- The recharge delay time (in game frames).                           *
 *                                                                                             *
 *          charging    -- Voice to announce that the weapon is charging.                      *
 *                                                                                             *
 *          ready       -- Voice to announce that the weapon is fully charged.                 *
 *                                                                                             *
 *          impatient   -- Voice to announce current charging state.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
SuperClass::SuperClass(SuperWeaponTypeClass * type, HouseClass * owner) :
	Class(type),
	House(owner),
	NeedsBuilding(true), // RA2: as above
	ChronoMapCoords(CELL_NONE), // RA2: as above
	Animation(NULL), // RA2: as above
	AnimationGotInvalid(false), // RA2: as above
	IsPresent(false),
	IsOneTime(false),
	IsReady(false),
	IsSuspended(false),
	OldStage(-1),
	ChargeDrainState(SUSPENDED)
{
	Create_ID();
	SuperWeapons.Add(this);
	AbstractTypePtrTracker.Add(this);
	HousePtrTracker.Add(this);
}


/// <summary>
/// Destructor for the super weapon objects.
/// This routine unhooks the super weapon from the global super weapon list and from the
/// trackers that watch its weapon type and its owning house.
/// </summary>
SuperClass::~SuperClass(void)
{
	Detach_Animation(); // RA2: as above
	TimedSuperWeapons.Delete(this); // RA2: as above
	SuperWeapons.Delete(this);
	AbstractTypePtrTracker.Delete(this);
	HousePtrTracker.Delete(this);
}


/***********************************************************************************************
 * SuperClass::Suspend -- Suspend the charging of the super weapon.                            *
 *                                                                                             *
 *    This will temporarily put on hold the charging of the special weapon. This might be the  *
 *    result of insufficient power.                                                            *
 *                                                                                             *
 * INPUT:   on -- Should the weapon charging be suspended? Else, it will unsuspend.            *
 *                                                                                             *
 * OUTPUT:  Was the weapon suspend state changed?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void SuperClass::Detach_Animation() // RA2: drops the animation and its tracker entries (0x0069CA70)
{
	if (Animation != NULL) { // RA2: as above
		Animation->Loops = 0; // RA2: as above
		Animation = NULL; // RA2: as above
		AnimPtrTracker.Delete(this); // RA2: as above
	}
	if (AnimationGotInvalid) { // RA2: as above
		AnimPtrTracker.Delete(this); // RA2: as above
		AnimationGotInvalid = false; // RA2: as above
	}
}


void SuperClass::Create_Chrono_Anim(Coord coord) // RA2: puts the chrono placement marker 5 leptons above coord (0x0069CB30)
{
	Detach_Animation(); // RA2: as above
	AnimClass *anim = new AnimClass(Rule->ChronoPlacement, coord + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
	if (anim != NULL) { // RA2: as above
		Animation = anim; // RA2: as above
		AnimPtrTracker.Add(this); // RA2: as above
	}
}


bool SuperClass::Suspend(bool on)
{
	if (IsPresent && !IsOneTime && on != IsSuspended && NeedsBuilding) {
		if (on || Class->IsManualControl) { // RA2: as above
			Control.Stop();
		} else {
			Control.Start();
		}
		IsSuspended = on;
		return(true);
	}
	return(false);
}


bool SuperClass::Enable(bool one_time, bool announce, bool on_hold) // RA2: grants the weapon and starts its charge (0x0069CCF0)
{
	if (!IsPresent) {
		IsPresent = true;
		IsOneTime = one_time; // RA2: as above
		BlinkInfo.State = false; // RA2: as above
		bool result = false; // RA2: as above
		UIName = Class->UIName; // RA2: as above
		if (!one_time) Suspend(false); // RA2: as above
		if (Class->ShowTimer && !House->Class->IsMultiplayPassive) { // RA2: as above
			TimedSuperWeapons.Add(this); // RA2: as above
		}
		if (Class->IsManualControl) {
			OldStage = -1;
			Control = Class->RechargeTime;
			Control.Stop();
		} else {
			result = Recharge(announce && !on_hold); // RA2: as above
		}
		if (on_hold) Suspend(true); // RA2: as above
		if (IsOneTime) Forced_Charge(announce); // RA2: as above
		return(result);
	}
	return(false);
}


bool SuperClass::Remove() // RA2: takes the weapon away and leaves the timer list (0x0069CF40)
{
	if (IsPresent) {
		IsReady = false;
		IsPresent = false;
		TimedSuperWeapons.Delete(this); // RA2: as above
		return(true);
	}
	return(false);
}


void SuperClass::Set_Ready(bool ready) // RA2: has this one-line setter out of line (0x0069CFB0)
{
	IsReady = ready; // RA2: as above
}


bool SuperClass::Recharge(bool player)
{
	Detach_Animation(); // RA2: as above
	if (IsPresent && !IsReady && !IsSuspended) {
		OldStage = -1;
		Control.Start();
		Control = Class->RechargeTime;
		if (Class->UseChargeDrain) {
			ChargeDrainState = CHARGING;
		}
		return(true);
	}
	return(false);
}


bool SuperClass::Discharged(bool player, Cell const & cell)
{
	if (Class->UseChargeDrain) {
		if (House->Is_Human_Player()) {
			if (ChargeDrainState == FIRESTORM_ON) {
				ChargeDrainState = READY;
				Control = Class->RechargeTime - Control.Value() / Rule->ChargeToDrainRatio;
				Deactivate_Firestorm(0, player);
				return(false);
			}
			if (ChargeDrainState == READY) {
				ChargeDrainState = FIRESTORM_ON;
				Control = (Class->RechargeTime - Control.Value()) * Rule->ChargeToDrainRatio;
				Place(cell, player);
			}
		} else {
			if (House->FirestormDefenseActivated) {
				Deactivate_Firestorm(0, player);
			} else {
				Place(cell, player);
			}
		}
		return(false);
	}

	if ((Control.Is_Active() && IsPresent && IsReady) || Class->PostClick) { // RA2: as above
		if (Class->Type == SUPER_LIGHTNING_STORM && IonStormClass::Is_Ion_Storm_Pending()) { // RA2: as above
			if (player) {
				IonStormClass::Ion_Storm_Active_Message(); // RA2: as above
			}
			return(false);
		}
		Place(cell, player);
		if (!Class->PostClick) { // RA2: as above
			IsReady = false;
		}
		if (IsOneTime) {
			IsOneTime = false;
			return(Remove());
		}
		if (Class->IsManualControl) {
			Control = Class->RechargeTime;
			OldStage = -1;
			Control.Stop();
		} else if (!Class->PreClick && !Class->PostClick) { // RA2: as above
			Recharge(player);
		}
	}
	return(false);
}


/***********************************************************************************************
 * SuperClass::AI -- Process the super weapon AI.                                              *
 *                                                                                             *
 *    This routine will process the charge up AI for this super weapon object. If the weapon   *
 *    has advanced far enough to change any sidebar graphic that might represent it, then      *
 *    "true" will be returned. Use this return value to intelligently update the sidebar.      *
 *                                                                                             *
 * INPUT:   player   -- Is this for the player? If it is and the weapon is now fully charged,  *
 *                      then this fully charged state will be announced to the player.         *
 *                                                                                             *
 * OUTPUT:  Was the weapon's state changed such that a sidebar graphic update will be          *
 *          necessary?                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SuperClass::AI(bool player)
{
	if (CurrentSuperWeaponType != SUPER_CHRONO_WARP && Animation != NULL) { // RA2: as above
		Animation->IsInvisible = true; // RA2: as above
		Animation->StartSoundAudioController.End(); // RA2: as above
	}
	if (Is_Present() && (!IsReady || Class->UseChargeDrain) && !IsSuspended) { // RA2: as above
		if (!Control.Is_Active()) {
			if (OldStage != -1) {
				OldStage = -1;
				return(true);
			}
		} else {
			if (Control == 0) {
				if (Class->UseChargeDrain) {
					if (ChargeDrainState == FIRESTORM_ON) {
						ChargeDrainState = CHARGING;
						Control = Class->RechargeTime;
					} else {
						ChargeDrainState = READY;
						IsReady = true;
					}
					return(true);
				} else {
					IsReady = true;
					if (player) {
						switch (Class->Type) {
						case SUPER_MULTI_MISSILE:
							Queue_Speech("EVA_NuclearMissileReady", -1, -1); // RA2: as above
							break;
						case SUPER_IRON_CURTAIN: // RA2: as above
							Queue_Speech("EVA_IronCurtainReady", -1, -1); // RA2: as above
							break;
						case SUPER_LIGHTNING_STORM: // RA2: as above
							Queue_Speech("EVA_LightningStormReady", -1, -1); // RA2: as above
							break;
						case SUPER_CHRONO_SPHERE: // RA2: as above
							Queue_Speech("EVA_ChronosphereReady", -1, -1); // RA2: as above
							break;
						case SUPER_PARA_DROP: // RA2: as above
						case SUPER_AMER_PARA_DROP: // RA2: as above
							Queue_Speech("EVA_ReinforcementsReady", -1, -1); // RA2: as above
							break;
						}
					}
					return(true);
				}
			} else {
				if (Anim_Stage() != OldStage) {
					OldStage = Anim_Stage();
					return(true);
				}
			}
		}
	}
	return(false);
}


/***********************************************************************************************
 * SuperClass::Anim_Stage -- Fetches the animation stage for this super weapon.                *
 *                                                                                             *
 *    This will return the current animation stage for this super weapon. The value will be    *
 *    between zero (uncharged) to ANIMATION_STAGES (fully charged). Use this value to render   *
 *    the appropriate graphic on the sidebar.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current animation stage for this special super weapon powerup.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/28/1995 JLB : Created.                                                                 *
 *   10/11/1996 JLB : Doesn't show complete until really complete.                             *
 *=============================================================================================*/
int SuperClass::Anim_Stage() const // RA2: sidebar stage with the charge drain ratio (0x0069D560)
{
	if (IsPresent) {
		int stage;
		if (Class->UseChargeDrain) {
			if (ChargeDrainState == FIRESTORM_ON) {
				stage = 54 * (1 - double(Class->RechargeTime * Rule->ChargeToDrainRatio - Control.Value()) / (Class->RechargeTime * Rule->ChargeToDrainRatio)); // RA2: as above
			} else {
				stage = 54 * (double(Class->RechargeTime - Control.Value()) / Class->RechargeTime); // RA2: as above
			}
		} else {
			if (IsReady) {
				return 54; // RA2: as above
			}
			stage = 54 * (double(Class->RechargeTime - Control.Value()) / Class->RechargeTime); // RA2: as above
		}
		stage = stage < 54 - 1 ? stage : 54 - 1; // RA2: as above
		return(stage);
	}
	return(0);
}


/***********************************************************************************************
 * SuperClass::Impatient_Click -- Called when player clicks on unfinished super weapon.        *
 *                                                                                             *
 *    This routine is called when the player clicks on the super weapon icon on the sidebar    *
 *    when the super weapon is not ready yet. This results in a voice message feedback to the  *
 *    player.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void SuperClass::Impatient_Click(void) const
{
	// RA2: does nothing (0x0069D680)
}


/***********************************************************************************************
 * SuperClass::Forced_Charge -- Force the super weapon to full charge state.                   *
 *                                                                                             *
 *    This routine will force the special weapon to full charge state. Call it when the weapon *
 *    needs to be instantly charged. The airstrike (when it first becomes available) is a      *
 *    good example.                                                                            *
 *                                                                                             *
 * INPUT:   player   -- Is this for the player? If true, then the full charge state will be    *
 *                      announced.                                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void SuperClass::Forced_Charge(bool player)
{
	if (IsPresent) {
		IsReady = true;
		Control.Start();
		Control = 0;
//		IsSuspended = false;
		if (player) {
			switch (Class->Type) {
			case SUPER_MULTI_MISSILE:
				Queue_Speech("EVA_NuclearMissileReady", -1, -1); // RA2: as above
				break;
			case SUPER_IRON_CURTAIN: // RA2: as above
				Queue_Speech("EVA_IronCurtainReady", -1, -1); // RA2: as above
				break;
			case SUPER_LIGHTNING_STORM: // RA2: as above
				Queue_Speech("EVA_LightningStormReady", -1, -1); // RA2: as above
				break;
			case SUPER_CHRONO_SPHERE: // RA2: as above
				Queue_Speech("EVA_ChronosphereReady", -1, -1); // RA2: as above
				break;
			case SUPER_PARA_DROP: // RA2: as above
			case SUPER_AMER_PARA_DROP: // RA2: as above
				Queue_Speech("EVA_ReinforcementsReady", -1, -1); // RA2: as above
				break;
			}
		}
		if (Class->UseChargeDrain) {
			ChargeDrainState = READY;
		}
	}
}


/// <summary>
/// Does the super weapon need power in order to charge?
/// </summary>
/// <returns>bool; Is this super weapon dependent upon the owner's power supply?</returns>
bool SuperClass::Is_Powered(void) const
{
	return(Class->IsPowered);
}


/// <summary>
/// Fetches the status text for the super weapon.
/// This routine is used by the sidebar to caption the super weapon's cameo with a
/// description of what it is currently up to.
/// </summary>
/// <returns>Returns with a pointer to the text to display, or NULL if the weapon has
/// nothing worth saying.</returns>
wchar_t const *SuperClass::State_String() const // RA2: returns wide texts from the string table (0x0069D770)
{
	if (!IsSuspended) {
		if (Class->UseChargeDrain) {
			switch (ChargeDrainState) {
			case CHARGING:
				return StringTable::LoadString("TXT_CHARGING", NULL, "D:\\Ra2\\Super.CPP", 0x32B); // RA2: as above
			case READY:
				return StringTable::LoadString("TXT_READY", NULL, "D:\\Ra2\\Super.CPP", 0x32E); // RA2: as above
			case FIRESTORM_ON:
				return StringTable::LoadString("TXT_FIRESTORM_ON", NULL, "D:\\Ra2\\Super.CPP", 0x331); // RA2: as above
			default:
				return(0);
			}
		}
		if (IsReady) {
			return StringTable::LoadString("TXT_READY", NULL, "D:\\Ra2\\Super.CPP", 0x344); // RA2: as above
		} else {
			return(0);
		}
	} else {
		return StringTable::LoadString("TXT_HOLD", NULL, "D:\\Ra2\\Super.CPP", 0x34A); // RA2: as above
	}
}


/// <summary>
/// Can the super weapon be fired right now?
/// This routine is used to decide whether clicking the cameo should arm the targeting
/// cursor. A suspended weapon can never be fired, no matter how charged it is.
/// </summary>
/// <returns>bool; Is the super weapon ready to be used?</returns>
bool SuperClass::Can_Place() // RA2: is not const (0x0069D820)
{
	if (!IsSuspended) {
		if (Class->UseChargeDrain) {
			return ChargeDrainState != NULL ? true : false; // RA2: as above
		}
		return(IsReady);
	}
	return(false);
}


#include "cast.h" // RA2: As_Unit, included here as in ra2-decomp's super.cpp


/// <summary>
/// Clears the targeting cursor and silences the "ready" announcement.
/// </summary>
/// <param name="name">The name of the speech to cancel.</param>
static inline void Ready_Voice(char const * name) // RA2: ra2-decomp's file helper (YR SuperClass::Launch)
{
	CurrentSuperWeaponType = SUPER_NONE;
	Speech_Cancel(Vox_From_Name(name));
}


/// <summary>
/// Unleashes the super weapon upon the cell specified.
/// This routine is called once the target has been chosen, either by the player
/// clicking on the map or by the computer deciding where to strike. Each kind of super
/// weapon does its own thing here -- some order a launch site to fire, others create
/// their effect outright, and the hunter seeker simply walks out of the nearest
/// suitable building.
/// </summary>
/// <param name="cell">The cell that was designated as the target.</param>
/// <param name="player">Is this for a player owned super weapon? If so, then the
/// targeting cursor gets cleared.</param>
void SuperClass::Place(Cell const & cell, bool player) // RA2: does each weapon type's effect here (0x0069D850)
{
	switch (Class->Type) {
	case SUPER_CHRONO_SPHERE: // RA2: as above
		if (Is_Ready()) {
			ChronoMapCoords = cell; // RA2: as above
			Coord coord = Map[cell].Center_Coord(); // RA2: as above
			if (Map[cell].IsUnderBridge) { // RA2: as above
				coord.Z += BRIDGE_LEPTON_HEIGHT; // RA2: as above
			}
			Create_Chrono_Anim(coord + Coord(0, 0, 5)); // RA2: as above
			if (player) {
				CurrentSuperWeaponType = SUPER_CHRONO_WARP; // RA2: as above
				return; // RA2: as above
			}
			if (Animation != 0) { // RA2: as above
				Animation->IsInvisible = true; // RA2: as above
				Animation->StartSoundAudioController.Stop(); // RA2: as above
				return; // RA2: as above
			}
		}
		break;

	case SUPER_CHRONO_WARP: { // RA2: as above
		Submit_Radar_Event(RADAREVENT_SUPERWEAPON_ACTIVATED, ChronoMapCoords); // RA2: as above
		Submit_Radar_Event(RADAREVENT_SUPERWEAPON_ACTIVATED, cell); // RA2: as above
		Coord dest = Map[cell].Center_Coord(); // RA2: as above
		Coord from = Map[ChronoMapCoords].Center_Coord(); // RA2: as above
		if (Map[cell].IsUnderBridge) { // RA2: as above
			dest.Z += BRIDGE_LEPTON_HEIGHT; // RA2: as above
		}
		if (Map[ChronoMapCoords].IsUnderBridge) { // RA2: as above
			from.Z += BRIDGE_LEPTON_HEIGHT; // RA2: as above
		}
		Detach_Animation(); // RA2: as above
		new AnimClass(Rule->ChronoBlastDest, dest + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
		new AnimClass(Rule->ChronoBlast, from + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
		for (int index = 0; index < 9; index++) { // RA2: as above
			Cell const & offset = CellAndNeighbors[index]; // RA2: as above
			ObjectClass *object; // RA2: as above
			if (Map[offset + ChronoMapCoords].IsUnderBridge) { // RA2: the offset first here (YR's image has the other order at this test)
				object = Map[ChronoMapCoords + offset].Cell_Bridge_Occupier(); // RA2: as above
			} else {
				object = Map[ChronoMapCoords + offset].Cell_Occupier(); // RA2: as above
			}
			while (object != 0) { // RA2: as above
				if (object->Is_Foot() && !object->In_Air()) { // RA2: as above
					FootClass * foot = (FootClass *)object; // RA2: as above
					if (foot->Techno_Type_Class()->Organic && !foot->Techno_Type_Class()->Teleporter && !foot->In_Air()) { // RA2: as above
						int damage = foot->Techno_Type_Class()->MaxStrength; // RA2: as above
						foot->Take_Damage(damage, 0, Rule->C4Warhead, 0, true, 0); // RA2: as above
					} else if (!foot->Unknown_Virtual_32C() && !foot->IsImmobilized && !foot->IsBeingWarpedOut() && !foot->IsWarpingIn()) { // RA2: as above
						IPiggybackPtr piggyback(foot->Locomotion); // RA2: as above
						ILocomotionPtr teleport(CLSID_TeleportLocomotion); // RA2: as above
						UnitClass * unit = As_Unit(foot); // RA2: as above
						Coord coord = Map[offset + cell].Center_Coord(); // RA2: as above
						if (Map[coord].IsUnderBridge) { // RA2: as above
							coord.Z += BRIDGE_LEPTON_HEIGHT; // RA2: as above
						}
						if (unit != 0) { // RA2: as above
							foot->Locomotion->Mark_All_Occupation_Bits(0); // RA2: as above
							foot->Locomotion->Force_Track(-1, coord); // RA2: as above
							foot->Clear_Occupy_Bit(coord); // RA2: as above
							foot->IsOccupyingCell = true; // RA2: as above
						}
						teleport->Link_To_Object(foot); // RA2: as above
						piggyback = IPiggybackPtr(teleport); // RA2: as above
						if (piggyback != NULL) { // RA2: as above
							piggyback->Begin_Piggyback(foot->Locomotion); // RA2: as above
							foot->Locomotion = teleport; // RA2: as above
						}
						Coord delta = foot->Center_Coord() - Map[ChronoMapCoords].Center_Coord(); // RA2: as above
						if (unit == 0) { // RA2: as above
							coord = Map[cell].Center_Coord() + delta; // RA2: as above
							if (Map[coord].IsUnderBridge) { // RA2: as above
								coord.Z += BRIDGE_LEPTON_HEIGHT; // RA2: as above
							}
						}
						foot->IsImmobilized = true; // RA2: as above
						foot->ChronoDestCoords = coord; // RA2: as above
						foot->Assign_Destination(&Map[offset + cell], true); // RA2: as above
					}
				}
				object = object->Next; // RA2: as above
			}
		}
		if (!MuteSWLaunches) { // RA2: as above
			Queue_Speech("EVA_ChronosphereActivated", -1, -1); // RA2: as above
		}
		if (player) {
			Ready_Voice("EVA_ChronosphereReady"); // RA2: as above
			return; // RA2: as above
		}
		break;
	}

	case SUPER_LIGHTNING_STORM: // RA2: as above
		if (Is_Ready()) {
			IonStormClass::Ion_Storm_Begin(Rule->LightningStormDuration, Rule->LightningDeferment, cell, House); // RA2: as above
			if (!MuteSWLaunches) { // RA2: as above
				Queue_Speech("EVA_LightningStormCreated", -1, -1); // RA2: as above
			}
			if (player) {
				Ready_Voice("EVA_LightningStormReady"); // RA2: as above
				return; // RA2: as above
			}
		}
		break;

	case SUPER_IRON_CURTAIN: // RA2: as above
		if (Is_Ready()) {
			Coord coord = Map[cell].Center_Coord(); // RA2: as above
			new AnimClass(Rule->IronCurtainInvokeAnim, coord + Coord(0, 0, 5), 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
			if (!MuteSWLaunches) { // RA2: as above
				Queue_Speech("EVA_IronCurtainActivated", -1, -1); // RA2: as above
			}
			Submit_Radar_Event(RADAREVENT_SUPERWEAPON_ACTIVATED, cell); // RA2: as above
			for (int index = 0; index < 9; index++) { // RA2: as above
				Cell const & offset = CellAndNeighbors[index]; // RA2: as above
				for (ObjectClass * object = Map[cell + offset].Cell_Occupier(); object != 0; object = object->Next) { // RA2: as above
					object->IronCurtain(Rule->IronCurtainDuration); // RA2: as above
				}
			}
			if (player) {
				Ready_Voice("EVA_IronCurtainReady"); // RA2: as above
				return; // RA2: as above
			}
		}
		break;

	case SUPER_PARA_DROP: // RA2: as above
		if (Is_Ready()) {
			AircraftType plane = AircraftTypeClass::From_Name("PDPLANE"); // RA2: as above
			CellClass *cellptr = &Map[cell]; // RA2: as above
			if (cellptr != 0 && cellptr != &BlubCell) { // RA2: as above
				if (cellptr->Is_Tile_Water()) { // RA2: as above
					Cell spot = Map.Nearby_Location(cell, SPEED_FOOT); // RA2: as above
					if (spot != CELL_NONE) { // RA2: as above
						CellClass *nearptr = &Map[spot]; // RA2: as above
						if (nearptr != 0 && nearptr != &BlubCell && !nearptr->Is_Tile_Water()) { // RA2: as above
							cellptr = nearptr; // RA2: as above
						}
					}
				}
				if (House->ActLike == 0) { // RA2: as above
					if (Rule->AllyParaDropInf.Count() == Rule->AllyParaDropNum.Count()) { // RA2: as above
						for (int i = 0; i < Rule->AllyParaDropInf.Count(); i++) { // RA2: as above
							int type = Rule->AllyParaDropInf[i]->HeapID; // RA2: as above
							if (plane != -1 && type != -1) { // RA2: as above
								Create_Air_Reinforcement(House, plane, 1, MISSION_PARADROP_APPROACH, cellptr, NULL, (InfantryType)type, Rule->AllyParaDropNum[i]); // RA2: as above
							}
						}
					}
				} else {
					for (int i = 0; i < Rule->SovParaDropInf.Count(); i++) { // RA2: as above
						int type = Rule->SovParaDropInf[i]->HeapID; // RA2: as above
						if (plane != -1 && type != -1) { // RA2: as above
							Create_Air_Reinforcement(House, plane, 1, MISSION_PARADROP_APPROACH, cellptr, NULL, (InfantryType)type, Rule->SovParaDropNum[i]); // RA2: as above
						}
					}
				}
			}
			if (player) {
				Ready_Voice("EVA_ReinforcementsReady"); // RA2: as above
				return; // RA2: as above
			}
		}
		break;

	case SUPER_AMER_PARA_DROP: // RA2: as above
		if (Is_Ready()) {
			AircraftType plane = AircraftTypeClass::From_Name("PDPLANE"); // RA2: as above
			CellClass *cellptr = &Map[cell]; // RA2: as above
			if (cellptr != 0 && cellptr != &BlubCell) { // RA2: as above
				if (cellptr->Is_Tile_Water()) { // RA2: as above
					Cell spot = Map.Nearby_Location(cell, SPEED_FOOT); // RA2: as above
					if (spot != CELL_NONE) { // RA2: as above
						CellClass *nearptr = &Map[spot]; // RA2: as above
						if (nearptr != 0 && nearptr != &BlubCell && !nearptr->Is_Tile_Water()) { // RA2: as above
							cellptr = nearptr; // RA2: as above
						}
					}
				}
				if (Rule->AmerParaDropInf.Count() == Rule->AmerParaDropNum.Count()) { // RA2: as above
					for (int i = 0; i < Rule->AmerParaDropInf.Count(); i++) { // RA2: as above
						int type = Rule->AmerParaDropInf[i]->HeapID; // RA2: as above
						if (plane != -1 && type != -1) { // RA2: as above
							Create_Air_Reinforcement(House, plane, 1, MISSION_PARADROP_APPROACH, cellptr, NULL, (InfantryType)type, Rule->AmerParaDropNum[i]); // RA2: as above
						}
					}
				}
			}
			if (player) {
				Ready_Voice("EVA_ReinforcementsReady"); // RA2: as above
			}
		}
		break;

	case SUPER_MULTI_MISSILE:
		if (Is_Ready()) {
			if (Is_One_Time()) { // RA2: as above
				Cell spot = cell; // RA2: as above
				Coord coord = spot.As_Coord(); // RA2: as above
				coord.Z = Map.Get_Height_GL(coord); // RA2: as above
				if (Map[spot].IsUnderBridge) { // RA2: as above
					coord.Z += BRIDGE_LEPTON_HEIGHT; // RA2: as above
				}
				WeaponTypeClass *payload = Weapons[WeaponTypeClass::From_Name("NukePayload")]; // RA2: as above
				CellClass *target = &Map[coord]; // RA2: as above
				Coord sound = coord; // RA2: as above
				Coord launch(sound.X, sound.Y, sound.Z + 20000); // RA2: as above
				BulletTypeClass const * bullet_type = payload->Bullet; // RA2: as above
				BulletClass * bullet = bullet_type->Create_Bullet(target, NULL, payload->Attack, payload->WarheadPtr, payload->MaxSpeed, false); // RA2: as above
				bullet->Set_Weapon_Type(payload); // RA2: as above
				Create_Attached_Anim(House, target, bullet, "PSIWARN"); // RA2: the bullet comes before the name
				if (bullet != 0) { // RA2: as above
					TVelocity3D<double> velocity(Dir256(DIR_N), Dir256(DIR_N), -100.0); // RA2: the same downward motion as TAction_MULTI_MISSILE
					bullet->Unlimbo(launch, velocity); // RA2: as above
					if (!MuteSWLaunches) { // RA2: as above
						Queue_Speech("EVA_NuclearMissileLaunched", -1, -1); // RA2: as above
					}
					VocClass::PlayIndexAtPos(Rule->DigSound, sound, 0); // RA2: as above
				}
				if (player) {
					Ready_Voice("EVA_NuclearMissileReady"); // RA2: as above
				}
				House->Production_Status_Changed();
			} else {
				for (int index = 0; index < BuildingTypes.Count(); index++) {
					BuildingTypeClass * btype = BuildingTypes[index]; // RA2: as above
					if (btype->IsNukeSilo && (btype->SuperWeapon == Class->HeapID || btype->SuperWeapon2 == Class->HeapID)) {
						BuildingClass * site = House->Find_Building((StructType)index, ZONE_NONE); // RA2: as above
						if (site != 0) { // RA2: as above
							Cell spot = cell; // RA2: as above
							Coord coord = spot.As_Coord(); // RA2: as above
							coord.Z = Map.Get_Height_GL(coord); // RA2: as above
							Coord sound = coord; // RA2: as above
							site->QueueMission(MISSION_MISSILE, false); // RA2: as above
							site->Commence(); // RA2: as above
							House->NukeDest = cell;
							site->LastSuperWeaponIndex = Class->Type; // RA2: as above
							VocClass::PlayIndexAtPos(Rule->DigSound, sound, 0); // RA2: as above
							if (!MuteSWLaunches) { // RA2: as above
								Queue_Speech("EVA_NuclearMissileLaunched", -1, -1); // RA2: as above
							}
						}
						break;
					}
				}
				if (player) {
					Ready_Voice("EVA_NuclearMissileReady"); // RA2: as above
				}
				House->Production_Status_Changed();
			}
		}
		break;
	}
}


/// <summary>
/// Shuts the firestorm defense back down.
/// This routine is called when the firestorm wall is toggled off or its charge runs
/// out. Any other kind of super weapon ignores the request.
/// </summary>
/// <param name="player">Is this for a player owned super weapon? If so, then the sidebar
/// will want redrawing.</param>
void SuperClass::Deactivate_Firestorm(int, bool player) const // RA2: the body is empty (0x0069E980)
{
}


/// <summary>
/// Is the super weapon building up toward its next use?
/// This routine is used by the sidebar to decide whether the charge-up clock should be
/// drawn over the cameo. A charge-drain weapon is always considered to be charging,
/// since it never simply sits at the ready.
/// </summary>
/// <returns>bool; Is the super weapon charging?</returns>
bool SuperClass::Is_Charging() const // RA2: (0x0069E990)
{
	if (Class->UseChargeDrain) {
		return(true);
	}
	return !IsReady; // RA2: as above
}


/// <summary>
/// Fetches the persistent class identifier for the super weapon.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT STDMETHODCALLTYPE SuperClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_SuperWeaponClass;
	return(S_OK);
}


/// <summary>
/// Reads the super weapon back in from the save game stream.
/// This routine rebuilds the object in place and then hands the weapon type and owning
/// house pointers to the swizzler so they can be remapped to their new addresses.
/// </summary>
/// <returns>Returns with S_OK if the super weapon was read successfully.</returns>
HRESULT STDMETHODCALLTYPE SuperClass::Load(IStream * stream)
{
	HRESULT result = BASECLASS::Load(stream);
	if (FAILED(result)) { // RA2: as above
		return(result);
	}
	new (this) SuperClass(NoInitClass());
	if (Animation != NULL) { // RA2: as above
		AnimPtrTracker.Add(this); // RA2: as above
	}
	Swizzle_Pointer(&Class);
	Swizzle_Pointer(&House);
	Swizzle_Pointer(&Animation); // RA2: as above
	BlinkInfo.Data0 = 0; // RA2: as above
	BlinkInfo.Data1 = 0; // RA2: as above
	return(S_OK);
}


HRESULT STDMETHODCALLTYPE SuperClass::Save(IStream * stream, BOOL cleardirty)
{
	HRESULT result = BASECLASS::Save(stream, cleardirty);
	if (SUCCEEDED(result)) {
		result = S_OK;
	}
	return(result);
}


void SuperClass::Detach(AbstractClass const * target, bool all)
{
	if (target == House) {
		House = NULL;
	}
	if (target == Class) {
		Class = NULL;
	}
	if (target == Animation) { // RA2: as above
		Animation = NULL; // RA2: as above
		AnimationGotInvalid = true; // RA2: as above
	}
}


/// <summary>
/// Submits the super weapon's state to the game CRC.
/// This routine is used by the multiplayer synchronization check to prove that every
/// machine agrees about the charge state of this super weapon.
/// </summary>
void SuperClass::Compute_CRC(CRCEngine & crc) const // RA2: sums the flags, the stage, the drain state and the animation (0x0069EB20)
{
	BASECLASS::Compute_CRC(crc);
	crc(IsPresent);
	crc(IsOneTime);
	crc(IsReady);
	crc((long)OldStage); // RA2: as above
	crc((long)ChargeDrainState); // RA2: as above
	crc(NeedsBuilding);
	crc(AnimationGotInvalid); // RA2: as above
	if (Animation != NULL) { // RA2: as above
		crc((long)Animation->Loops); // RA2: as above
		crc(Animation->Fetch_ID()); // RA2: as above
	}
}


/// <summary>
/// Delivers a squad of elite infantry by drop pod.
/// This routine is used by the drop pod super weapon to land a handful of veteran
/// soldiers on the map. Each pod aims for a cell adjacent to the one before it, so the
/// squad arrives as a cluster rather than stacked on one spot, and any soldier that
/// cannot find room to land is quietly thrown away.
/// </summary>
/// <param name="cell">The cell to begin dropping the squad around.</param>
void SuperClass::Reset() // RA2: drops the animation and resets the charge (0x0069EBB0)
{
	Detach_Animation(); // RA2: as above
	IsReady = false;
	OldStage = -1;
	bool active = Control.Is_Active(); // RA2: as above
	Control = Class->RechargeTime;
	if (!active) { // RA2: as above
		Control.Stop();
	}
}
