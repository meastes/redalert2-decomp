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

/* $Header: /CounterStrike/INFANTRY.CPP 2     3/03/97 10:35p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INFANTRY.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 15, 1994                                              *
 *                                                                                             *
 *                  Last Update : October 28, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   InfantryClass::AI -- Handles the infantry non-graphic related AI processing.              *
 *   InfantryClass::Active_Click_With -- Handles action when clicking with infantry soldier.   *
 *   InfantryClass::Assign_Destination -- Gives the infantry a movement destination.           *
 *   InfantryClass::Assign_Target -- Gives the infantry a combat target.                       *
 *   InfantryClass::Can_Enter_Cell -- Determines if the infantry can enter the cell specified. *
 *   InfantryClass::Can_Fire -- Can the infantry fire its weapon?                              *
 *   InfantryClass::Class_Of -- Returns the class reference for this object.                   *
 *   InfantryClass::Clear_Occupy_Bit -- Clears occupy bit and given cell                       *
 *   InfantryClass::Debug_Dump -- Displays debug information about infantry unit.              *
 *   InfantryClass::Detach -- Removes the specified target from targeting computer.            *
 *   InfantryClass::Do_Action -- Launches the infantry into an animation sequence.             *
 *   InfantryClass::Doing_AI -- Handles the animation AI processing.                           *
 *   InfantryClass::Draw_It -- Draws a unit object.                                            *
 *   InfantryClass::Edge_Of_World_AI -- Detects when infantry has left the map.                *
 *   InfantryClass::Enter_Idle_Mode -- The infantry unit enters idle mode by this routine.     *
 *   InfantryClass::Fear_AI -- Process any fear related affects on this infantry.              *
 *   InfantryClass::Fire_At -- Fires projectile from infantry unit.                            *
 *   InfantryClass::Firing_AI -- Handles firing and combat AI for the infantry.                *
 *   InfantryClass::Full_Name -- Fetches the full name of the infantry unit.                   *
 *   InfantryClass::Get_Image_Data -- Fetches the image data for this infantry unit.           *
 *   InfantryClass::Greatest_Threat -- Determines greatest threat (target) for infantry unit.  *
 *   InfantryClass::InfantryClass -- The constructor for infantry objects.                     *
 *   InfantryClass::Init -- Initialize the infantry object system.                             *
 *   InfantryClass::Is_Ready_To_Random_Anima -- Checks to see if it is ready to perform an idle*
 *   InfantryClass::Limbo -- Performs cleanup operations needed when limboing.                 *
 *   InfantryClass::Mission_Attack -- Intercept attack mission for special handling.           *
 *   InfantryClass::Movement_AI -- This routine handles all infantry movement logic.           *
 *   InfantryClass::Overlap_List -- The list of cells that the infantry overlaps, but doesn't o*
 *   InfantryClass::Paradrop -- Handles paradropping infantry.                                 *
 *   InfantryClass::Per_Cell_Process -- Handles special operations that occur once per cell.   *
 *   InfantryClass::Random_Animate -- Randomly animate the infantry (maybe)                    *
 *   InfantryClass::Read_INI -- Reads units from scenario INI file.                            *
 *   InfantryClass::Response_Attack -- Plays infantry audio response to attack order.          *
 *   InfantryClass::Response_Move -- Plays infantry response to movement order.                *
 *   InfantryClass::Response_Select -- Plays infantry audio response due to being selected.    *
 *   InfantryClass::Scatter -- Causes the infantry to scatter to nearby cell.                  *
 *   InfantryClass::Set_Occupy_Bit -- Sets the occupy bit cell and bit pos                     *
 *   InfantryClass::Set_Primary_Facing -- Change infantry primary facing -- always and instantl*
 *   InfantryClass::Shape_Number -- Fetch the shape number for this infantry.                  *
 *   InfantryClass::Start_Driver -- Handles giving immediate destination and move orders.      *
 *   InfantryClass::Stop_Driver -- Stops the infantry from moving any further.                 *
 *   InfantryClass::Take_Damage -- Applies damage to the infantry unit.                        *
 *   InfantryClass::Unlimbo -- Unlimbo infantry unit in legal sub-location.                    *
 *   InfantryClass::What_Action -- Determines what action to perform for the cell specified.   *
 *   InfantryClass::What_Action -- Infantry units might be able to capture -- check.           *
 *   InfantryClass::Write_INI -- Store the infantry to the INI database.                       *
 *   InfantryClass::operator delete -- Returns the infantry object back to the free pool       *
 *   InfantryClass::operator new -- Allocates an infantry object from the free pool.           *
 *   InfantryClass::~InfantryClass -- Default destructor for infantry units.                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "infantry.h"

#include "_bench.h"
#include "_convert.h"
#include "_keyboar.h" // RA2: What_Action reads the force-move and force-fire keys
#include "_mixfile.h"
#include "_rtti.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "aircraft.h"
#include "anim.h"
#include "astar.h"
#include "bench.h"
#include "building.h"
#include "builtype.h"
#include "cast.h" // RA2: Assign_Destination's checked casts (ra2-decomp)
#include "ccrand.h"
#include "cell.h"
#include "combat.h"
#include "data.h"
#include "draw.h"
#include "fly.h"
#include "fog.h"
#include "goptions.h"
#include "house.h"
#include "houstype.h"
#include "ilocos.h"
#include "incdec.h"
#include "infatype.h"
#include "inline.h"
#include "ion.h"
#include "isotype.h"
#include "language\language.h"
#include "lightcon.h"
#include "mixfile.h"
#include "mono.h"
#include "overlay.h"
#include "overtype.h"
#include "piggyback.h" // RA2: Assign_Destination hands the locomotor over (ra2-decomp)
#include "parasite.h" // RA2: Can_Enter_Cell reads the parasite victim
#include "planning.h" // RA2: Planning_Allows_Event_Type
#include "radsite.h" // RA2
#include "revent.h"
#include "rules.h"
#include "scheme.h"
#include "temporal.h" // RA2: AI calls TemporalClass::AI
#include "session.h"
#include "sun.h"
#include "swizzle.h"
#include "tactical.h"
#include "tag.h"
#include "tagtype.h"
#include "team.h"
#include "tiberium.h"
#include "tracker.h"
#include "tube.h"
#include "tube.hh"
#include "unit.h"
#include "vox.h"
#include "warhead.h"
#include "weapon.h"

#include "bench.hh"
#include "stringtable.h" // RA2


static ShapeSet const * InfantryShadowShape = NULL; // RA2: 0x00A41134; INFSHDW.SHP, loaded on first use


char const * const InfantryClass::INI_Name = "Infantry";

int const InfantryClass::HumanShape[32] = { 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 7, 7 };


/***********************************************************************************************
 * InfantryClass::InfantryClass -- The constructor for infantry objects.                       *
 *                                                                                             *
 *    This is the constructor used when creating an infantry unit. All values are required     *
 *    except for facing and position. If these are absent, then the infantry is created in     *
 *    a state of limbo -- not placed upon the map.                                             *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryClass::InfantryClass(InfantryTypeClass const * type, HouseClass * house) :
	BASECLASS(house),
	Class((InfantryTypeClass *)type),
	Doing(DO_NOTHING),
	Comment(0),
	IsTechnician(false), // RA2: InfantryClass at 0x4FF640 has no ProneStruggleTimer, LookTimer
	IsStoked(false),
	IsProne(false),
	IsBerzerk(false),
	IsZoneCheat(false),
	WasSelected(false),
	Fear(FEAR_NONE), // RA2: InfantryClass at 0x4FF640 matches only so
	Unused_5C8(0), // RA2: as RA2 initializes it
	ShouldDeploy(false), // RA2: as above
	LastSurface(2) // RA2: as above
{
	Create_ID();
	Infantry.Add(this);

	Init();

	if (Class != NULL) {
		Locomotion.CreateInstance(Class->Locomotor, NULL, CLSCTX_ALL);
		Locomotion->Link_To_Object(this);
	}

	PrimaryFacing.Set_ROT(127);
	TargetTracker.Add_Index(Fetch_ID(), this);
}


/***********************************************************************************************
 * InfantryClass::Init -- Initialize the infantry object system.                               *
 *                                                                                             *
 *    This routine will force the infantry object system into its empty initial state. It      *
 *    is called when the scenario needs to be cleared in preparation for a scenario load.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Init(void)
{
	BASECLASS::Init();

	if (House != NULL) {
		House->Tracking_Add(this);
	}

	if (Class != NULL) {
		if (House != NULL) { // RA2: Init at 0x4FF8B0 matches only so
			TypeList<InfantryTypeClass const *> & list = House->Class->VeteranInfantry; // RA2: as above
			for (int index = 0; index < list.Count(); index++) { // RA2: as above
				if (list[index] == Class) { // RA2: as above
					Veterancy.Set_Veteran(true); // RA2: as above
					break;
				}
			}
		}

		if (House != NULL && House->BarracksInfiltrated && Class->IsTrainable) { // RA2: as above
			Veterancy.Set_Veteran(true); // RA2: as above
		}

		Strength = Class->MaxStrength;
		EstimatedHealth = Class->MaxStrength; // RA2: as above
		if (Class->InitialAmmo == -1) { // RA2: as above
			Ammo = Class->MaxAmmo; // RA2: as above
		} else {
			Ammo = Class->InitialAmmo; // RA2: as above
		}
		IsCloakable = Class->IsCloakable;
	}
}


/***********************************************************************************************
 * InfantryClass::~InfantryClass -- Default destructor for infantry units.                     *
 *                                                                                             *
 *    This is the default destructor for infantry type units. It will put the infantry into    *
 *    a limbo state if it isn't already in that state and the game is still active.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryClass::~InfantryClass(void)
{
	if (GameActive && Class != NULL) {

		if (House->Can_Build(Class, false, false) == -1) {
			House->IsRecalcNeeded = true;
		}

		/*
		**	Remove this member from any team it may be associated with. This must occur at the
		**	top most level of the inheritance hierarchy because it may call virtual functions.
		*/
		if (Team != NULL) {
			Team->Remove(this);
			Team = NULL;
		}

		House->Tracking_Remove(this);
		Limbo();
	}
	Detach_This_From_All(this);
	Infantry.Delete(this);
	TargetTracker.Remove_Index(Fetch_ID());
	IsActive = false;
}


/***********************************************************************************************
 * InfantryClass::Take_Damage -- Applies damage to the infantry unit.                          *
 *                                                                                             *
 *    This routine applies the damage specified to the infantry object. It is possible that    *
 *    this routine will DESTROY the infantry unit in the process.                              *
 *                                                                                             *
 * INPUT:   damage   -- The damage points to inflict.                                          *
 *                                                                                             *
 *          distance -- The distance from the damage center point to the object's center point.*
 *                                                                                             *
 *          warhead  -- The warhead type that is inflicting the damage.                        *
 *                                                                                             *
 *          source   -- Who is responsible for inflicting the damage.                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry unit destroyed by this damage?                              *
 *                                                                                             *
 * WARNINGS:   Since the infantry unit could be destroyed by this routine, be sure to check    *
 *             for this in the code that follows the call to Take_Damage().                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   11/22/1994 JLB : Shares base damage handler for techno objects.                           *
 *   03/31/1995 JLB : Revenge factor.                                                          *
 *=============================================================================================*/
ResultType InfantryClass::Take_Damage(int & damage, int distance, WarheadTypeClass const * warhead, TechnoClass * source, bool forced, bool no_crew)
{
	ResultType res = RESULT_NONE;

	/*
	**	Prone infantry take only half damage, but never below one damage point.
	*/
	if (IsProne && damage > 0 && !forced) {
		damage = (int)(damage * warhead->ProneDamage);

		if (damage < 1) {
			damage = 1;
		}
	}

	res = BASECLASS::Take_Damage(damage, distance, warhead, source, forced, no_crew); // RA2: no webby paralysis

	if (res == RESULT_ALREADY_DESTROYED) return(res);

	if (res == RESULT_NONE) return(res);

	if (res == RESULT_DESTROYED) {
		Death_Announcement(source);
		Stop_Driver();
		Stun();
		QueueMission(MISSION_NONE, false); // RA2: the mission is queued, not assigned
		QueueMission(MISSION_GUARD, false); // RA2: as above
		Commence();
		Kill_Cargo(source);

		bool delthis = false;
		if (forced && Class->IsCyborg) {
			delthis = true;
			if (IsToExplode) {
				new AnimClass(Rule->InfantryExplode, PositionCoord);
			}
		}

		if (HeightAGL <= 10 && Map[(Coord const &)PositionCoord].Land_Type() == LAND_WATER && IsToExplode) {
			new AnimClass(Rule->Wake, PositionCoord);
			new AnimClass(Rule->SplashList[0], PositionCoord + Coord(0,0,3));
			delthis = true;
		} else if (Class->IsCyborg && IsProne) {
			new AnimClass(Rule->InfantryExplode, PositionCoord);
			delthis = true;
		} else if (Class->JumpJet) { // RA2: a jumpjet soldier explodes
			new AnimClass(Rule->InfantryExplode, PositionCoord);
			delthis = true;
		} else {

			/*
			**	The type of warhead determines the animation the infantry
			**	will perform when killed.
			*/
			int infdeath = warhead->InfantryDeath;
			if (Doing == DO_PARADROP) { // RA2: a paradropping soldier explodes
				infdeath = 3; // RA2: as above
			}
			if (source != NULL && source->RTTI == RTTI_BUILDING && reinterpret_cast<BuildingClass*>(source)->Class->IsLaserFence) {
				infdeath = 5;
			}

			if (Class->DeathAnims.Count() > 0) { // RA2: the type's own death animations
				if (infdeath < 0 || infdeath >= Class->DeathAnims.Count() || Class->DeathAnims[infdeath] == NULL) { // RA2: as above
					infdeath = 0; // RA2: as above
				}
				if (Class->DeathAnims[infdeath] != NULL) { // RA2: as above
					new AnimClass(Class->DeathAnims[infdeath], PositionCoord); // RA2: as above
				}
				delthis = true;
			} else if (Class->NotHuman) { // RA2: animals simply fall
				Do_Action(DO_GUN_DEATH, true);
			} else {
				switch (infdeath) {
					default:
					case 0:
						delthis = true;
						break;

					case 1:
						Do_Action(DO_GUN_DEATH, true);
						break;

					case 2:
						Do_Action(DO_EXPLOSION_DEATH, true);
						break;

					case 3:
						new AnimClass(Rule->InfantryExplode, PositionCoord);
						delthis = true;
						break;

					case 4:
						new AnimClass(Rule->FlamingInfantry, PositionCoord); // RA2: the executable's death animations and fear values
						delthis = true;
						break;

					case 5:
						new AnimClass(AnimTypes[ANIM_ELECT_DIE], PositionCoord); // RA2: the executable's death animations and fear values
						delthis = true;
						break;

					case 6: // RA2: the head pop
						new AnimClass(Rule->InfantryHeadPop, PositionCoord); // RA2: the executable's death animations and fear values
						delthis = true;
						break;

					case 7: // RA2: nuked
						new AnimClass(Rule->InfantryNuked, PositionCoord); // RA2: the executable's death animations and fear values
						delthis = true;
						break;
				}
			}
		}

		if (delthis) {
			if (Class->Crashable) { // RA2: a crashable soldier may crash instead of vanishing
				if (!Crash(NULL)) { // RA2: as above
					Delete_Me();
					return(res);
				}
			} else {
				Delete_Me();
			}
		}
		return(res);
	}

	/*
	**	When infantry gets hit, it gets scared.
	*/
	if (res != RESULT_DESTROYED) {
		Coord source_coord = (source) ? source->PositionCoord : COORD_NONE;

		/*
		**	If an engineer is damaged and it is just sitting there, then tell it
		**	to go do something since it will definitely die if it doesn't.
		*/
		if (!House->Is_Human_Player() && Class->IsEngineer && (Get_Mission() == MISSION_GUARD || Get_Mission() == MISSION_GUARD_AREA)) { // RA2: the executable's death animations and fear values
			QueueMission(MISSION_HUNT, false); // RA2: as above
		}

		if (source != NULL) {
			Scatter(source_coord, false, false); // RA2: the executable's death animations and fear values
		}

		if (source != NULL && Fear < FEAR_SCARED) {
			if (Class->IsFraidyCat) {
				Fear = (FearType)300; // RA2: panic is 300
			} else if (!Class->IsFearless && !Has_Ability(ABILITY_FEARLESS)) {
				Fear = FEAR_SCARED;
			}
		} else {

			if (!Class->IsFearless && !Has_Ability(ABILITY_FEARLESS)) {
				/*
				**	Increase the fear of the infantry by a bit. The fear increases more
				**	quickly if the infantry is damaged.
				*/
				int morefear = 50; // RA2: as above
				if (HealthRatio > Rule->ConditionRed) morefear /= 2;
				if (HealthRatio > Rule->ConditionYellow) morefear /= 2;
				Fear = FearType(MIN((int)Fear + morefear, 300)); // RA2: as above
			}
		}
	}

	return(res);
}


/***********************************************************************************************
 * InfantryClass::Shape_Number -- Fetch the shape number for this infantry.                    *
 *                                                                                             *
 *    This will determine the shape number to use for this infantry soldier. The shape number  *
 *    is relative to the shape file associated with this infantry unit.                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number for this infantry object to be used when drawing.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int InfantryClass::Shape_Number(void) const
{
	/*
	**	Fetch the shape pointer to use for the infantry. This is controlled by what
	**	choreograph sequence the infantry is performing, it's facing, and whether it
	**	is prone.
	*/
	DoType doit = Doing;
	if (doit == DO_NOTHING) {
		if (Map[Get_Cell()].Land_Type() == LAND_WATER && !IsOnBridge) { // RA2: treading water
			doit = DO_TREAD; // RA2: the Tread sequence
		} else {
			doit = DO_STAND_READY;
		}
	}

	InfantryTypeClass const * type = Class; // RA2: the disguise's sequences for those who cannot see through it
	if (!IsClearlyVisibleTo(PlayerPtr)) { // RA2
		type = (InfantryTypeClass const *)GetDisguise(true); // RA2
	}

	/*
	**	The infantry shape is always modulo the number of animation frames
	**	of the action stage that the infantry is doing.
	*/
	int shapenum = Fetch_Stage() % MAX(type->DoControls[doit].Count, 1); // RA2: type

	if (Is_JumpJet() && TarCom != NULL) {

		/*
		**	If facing makes a difference, then the shape number will be incremented
		**	by the facing accordingly.
		*/
		if (type->DoControls[doit].Jump > 0) { // RA2: type
			shapenum += HumanShape[Direction(TarCom).As_Dir32()] * type->DoControls[doit].Jump; // RA2: type
		}

	} else {

		/*
		**	If facing makes a difference, then the shape number will be incremented
		**	by the facing accordingly.
		*/
		if (type->DoControls[doit].Jump > 0) { // RA2: type
			shapenum += HumanShape[PrimaryFacing.Current().As_Dir32()] * type->DoControls[doit].Jump; // RA2: type; no DirType copy
		}
	}

	/*
	**	Finally, the shape number is biased according to the starting frame number for
	**	that action in the infantry shape file.
	*/
	shapenum += type->DoControls[doit].Frame; // RA2: type

	/*
	**	Return with the final infantry shape number.
	*/
	return(shapenum);
}


/***********************************************************************************************
 * InfantryClass::Draw_It -- Draws a unit object.                                              *
 *                                                                                             *
 *    This routine is the one that actually draws a unit object. It displays the unit          *
 *    according to its current state flags and centered at the location specified.             *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y coordinate of where to draw the unit.                         *
 *                                                                                             *
 *          window   -- The clipping window to use.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   06/27/1994 JLB : Takes a window parameter.                                                *
 *   08/15/1994 JLB : Converted to infantry support.                                           *
 *   08/14/1996 JLB : Simplified.                                                              *
 *=============================================================================================*/
void InfantryClass::Draw_It(Point2D const & xpoint, Rect const & cliprect) const
{
	static int _zadj = -10;

	Point2D point = xpoint;
	Cell cell = Get_Target_Cell();

	if (CurrentTube == -1) {
		IPersistPtr persist = Locomotion;
		CLSID clsid;
		persist->GetClassID(&clsid);

		if (HeightAGL > 0 && clsid == CLSID_BallisticLocomotion) {
			ShapeSet const * shapefile = (ShapeSet const *)MFCD::Retrieve("POD.SHP");
			Point2D spoint = xpoint + Point2D(Locomotion->Shadow_Point());
			Draw_Shape(
				*LogicalSurface,
				*NormalDrawer,
				shapefile,
				Locomotion->Drawing_Code(),
				spoint,
				cliprect,
				ShapeFlags_Type(SHAPE_ZGRAD|SHAPE_ALPHA|SHAPE_WIN_REL|SHAPE_CENTER|SHAPE_DARKEN),
				NULL,
				Get_Z_Adjust() - 2
			);

			Map[cell];
			Map[cell];

			Draw_Object(shapefile, Locomotion->Drawing_Code(), xpoint, cliprect, DIR_N, 256, 0, ZGRAD_90DEG, false, NORMAL_LIGHT);
		} else {

			/*
			**	Verify the legality of the unit class by seeing if there is shape imagery for it. If
			**	there is no shape image, then it certainly can't be drawn -- bail.
			*/
			ShapeSet const * shapefile = (ShapeSet const *)Get_Image_Data();

			if (shapefile == NULL) return;

			Cell tcell = Get_Target_Cell();

			TacticalMap->Add_To_Selectables((ObjectClass *)this, point);

			int level_light; // RA2: the ion storm and the nuke flash have their own level lighting
			if (IonStormClass::Is_Ion_Storm_Active()) {
				level_light = Scen->IonLevelLight; // RA2: as above
			} else if (NukeFlash::IsFadingIn()) { // RA2: as above
				level_light = Scen->NukeLevelLight; // RA2: as above
			} else {
				level_light = Scen->LevelLight; // RA2: as above
			}

			int brightness;
			if (IsOnBridge || (Map[(Coord const &)PositionCoord].IsOvershadowed && PositionCoord.Z > Map.Get_Height_GL(PositionCoord) + (BRIDGE_LEPTON_HEIGHT / 2))) {
				int light = level_light * (HeightAGL / (2 * LEVEL_LEPTON_H)); // RA2: the executable's level light and shadow shape
				brightness = Map[tcell].Brightness + light;
			} else {
				brightness = Map[tcell].Brightness + (Map[(Coord const &)PositionCoord].IsOvershadowed ? -500 : 0);
			}
			brightness += Rule->ExtraInfantryLight;

			int height = HeightAGL;
			CellClass *cellptr = &Map[(Coord const &)PositionCoord];

			if (cellptr->IsUnderBridge && height >= BRIDGE_LEPTON_HEIGHT) {
				if ((cellptr->IsBridgeEastWest && cellptr->Adjacent_Cell(FACING_N).IsUnderBridge) ||
					(!cellptr->IsBridgeEastWest && cellptr->Adjacent_Cell(FACING_W).IsUnderBridge)) {
					height -= BRIDGE_LEPTON_HEIGHT;
				}
			}

			if (height > 0 && (InfantryShadowShape != NULL || (InfantryShadowShape = (ShapeSet const *)MFCD::Retrieve("INFSHDW.SHP")) != NULL)) { // RA2: the shadow is a shape of its own
				Draw_Shape(
					*LogicalSurface,
					*NormalDrawer,
					InfantryShadowShape, // RA2: the executable's level light and shadow shape
					1, // RA2: as above
					point + Point2D(0, -IonBlastYDrawOffset) + Point2D(0, TacticalMap->Z_Lepton_To_Pixel(height)),
					cliprect,
					ShapeFlags_Type(SHAPE_ZGRAD|SHAPE_WIN_REL|SHAPE_CENTER|SHAPE_DARKEN),
					NULL,
					-5 - TacticalMap->Z_Lepton_To_Pixel(PositionCoord.Z - height)
				);
			}

			Draw_Object(shapefile, Shape_Number(), point + Point2D(0, -IonBlastYDrawOffset), cliprect, DIR_N, 256, _zadj, ZGRAD_90DEG, false, brightness);
		}
	} else {
		TacticalMap->Add_To_Selectables((ObjectClass *)this, point);
	}

	BASECLASS::Draw_It(point, cliprect);
}



/***************************************************************************
**	This is the array of constant data associated with infantry maneuvers. It
**	specifies the frame rate as well as if the animation can be aborted.
*/
// interruptible, mobile, randomstart, rate
DoStruct const InfantryClass::MasterDoControls[] = { // RA2: one entry per art sequence (infatype.cpp, SequenceName)
	{true,	false,	false,	0},	// DO_STAND_READY
	{true,	false,	false,	0},	// DO_STAND_GUARD
	{true,	false,	false,	6},	// DO_PRONE
	{true,	true,	true,	3},	// DO_WALK
	{true,	false,	false,	1},	// DO_FIRE_WEAPON
	{false,	true,	false,	1},	// DO_LIE_DOWN
	{true,	true,	true,	1},	// DO_CRAWL
	{false,	false,	false,	1},	// DO_GET_UP
	{true,	false,	false,	1},	// DO_FIRE_PRONE
	{ true, false, false, 3 },	// DO_IDLE1
	{ true, false, false, 3 },	// DO_IDLE2
	{false,	false,	false,	1},	// DO_GUN_DEATH
	{false,	false,	false,	1},	// DO_EXPLOSION_DEATH
	{false,	false,	false,	1},	// DO_EXPLOSION2_DEATH
	{false,	false,	false,	1},	// DO_GRENADE_DEATH
	{false,	false,	false,	1},	// DO_FIRE_DEATH
	{ true, false, false, 3 },	// DO_TREAD
	{ true, true, true, 1 },	// DO_SWIM
	{ true, false, false, 3 },	// DO_WET_IDLE1
	{ true, false, false, 3 },	// DO_WET_IDLE2
	{ false, false, false, 1 },	// DO_WET_DIE1
	{ false, false, false, 1 },	// DO_WET_DIE2
	{ true, false, false, 1 },	// DO_WET_ATTACK
	{ true, true, false, 2 },	// DO_HOVER
	{ true, true, false, 1 },	// DO_FLY
	{ true, true, false, 1 },	// DO_TUMBLE
	{ true, true, false, 1 },	// DO_FIREFLY
	{ false, false, false, 1 },	// DO_DEPLOY
	{ true, false, false, 1 },	// DO_DEPLOYED
	{ true, false, false, 1 },	// DO_DEPLOYED_FIRE
	{ true, false, false, 1 },	// DO_DEPLOYED_IDLE
	{ false, false, false, 1 },	// DO_UNDEPLOY
	{ true, false, false, 3 },	// DO_CHEER
	{ true, false, false, 1 },	// DO_PARADROP
	{ false, false, false, 3 },	// DO_AIR_DEATH_START
	{ false, false, false, 1 },	// DO_AIR_DEATH_FALLING
	{ false, false, false, 3 },	// DO_AIR_DEATH_FINISH
	{ true, true, true, 4 },	// DO_PANIC
};


#ifdef _DEBUG
/***********************************************************************************************
 * InfantryClass::Debug_Dump -- Displays debug information about infantry unit.                *
 *                                                                                             *
 *    This routine is used by the debug version to display pertinent information about the     *
 *    infantry unit.                                                                           *
 *                                                                                             *
 * INPUT:   mono  -- The monochrome screen to display the debug information to.                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(0, 0);

	mono->Set_Cursor(1, 11);mono->Printf("%3d", Doing);
	mono->Set_Cursor(8, 11);mono->Printf("%3d", Fear);

	mono->Fill_Attrib(66, 13, 12, 1, IsTechnician ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 14, 12, 1, IsStoked ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 15, 12, 1, IsProne ? MonoClass::INVERSE : MonoClass::NORMAL);

	BASECLASS::Debug_Dump(mono);
}
#endif



/***********************************************************************************************
 * InfantryClass::Per_Cell_Process -- Handles special operations that occur once per cell.     *
 *                                                                                             *
 *    This routine will handle any special operations that need to be performed once each      *
 *    cell travelled. This includes radioing a transport that it is now clear and the          *
 *    transport is free to leave.                                                              *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   03/01/1995 JLB : Capture building options.                                                *
 *   05/31/1995 JLB : Capture is always successful now.                                        *
 *=============================================================================================*/
void InfantryClass::Per_Cell_Process(PCPType why)
{
	BStart(BENCH_PCP);
	CellClass * cellptr = &Map[Get_Coord()];

	if (why == PCP_END) {
		if (Mission == MISSION_CAPTURE && (Class->Assaulter || Class->Occupier) &&
			NavCom && NavCom->RTTI == RTTI_BUILDING) {
			BuildingClass * building = (BuildingClass *)NavCom;
			if (building == cellptr->Cell_Building()) {
				if (!building->CanBeOccupiedBy(this)) {
					Assign_Destination(NULL, true);
					Scatter(building->Center_Coord(), true, true);
					return;
				}
				if (building->Tag) building->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
				Enter_Occupiable(building);
				return;
			}
		}
		if (Mission == MISSION_EATEN) {
			BuildingClass * building = cellptr->Cell_Building();
			if (building && (building == NavCom || building == TarCom)) {
				if (building->Tag) building->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
				if (TClass->VoiceDie.Count() > 0 && House->Is_Player_Control()) {
					VocClass::PlayAt(TClass->VoiceDie.Pick(NonCriticalRandomNumber()), PositionCoord, 0);
				}
				if (TClass->DieSound.Count() > 0) {
					VocClass::PlayAt(TClass->DieSound.Pick(NonCriticalRandomNumber()), PositionCoord, 0);
				}
				building->House->Refund_Money(Refund_Amount());
				if (Tag) Tag->Spring(TEVENT_DESTROYED_ANY, this);
				Delete_Me();
				return;
			}
		}


		/*
		**	If the infantry unit is entering a cell that contains the building it is trying to
		**	capture, then capture it.
		*/
		if (Mission == MISSION_CAPTURE || Mission == MISSION_GUARD_AREA || Mission == MISSION_PATROL) {
			if (NavCom != NULL && NavCom->Is_Techno() && ((TechnoClass*)NavCom)->Considered_Vehicle()) {
				TechnoClass * tech = (TechnoClass *)NavCom;
				if (tech != NULL && tech->RTTI == RTTI_BUILDING) {
					if (PositionCell == tech->PositionCell) {
						if (tech->Tag) {
							tech->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
						}
						tech->Detach_All(false);
						tech->Captured(House);
						tech->EnteredByInfType = Class->HeapID;
						tech->Scatter_Incoming_Infantry();
						if (Tag && Tag->Is_To_Inherit()) {
							tech->Attach_Tag(Tag);
						}
						if (Tag) {
							Tag->Spring(TEVENT_DESTROYED_ANY, this);
						}
						Delete_Me();
						BEnd(BENCH_PCP);
						return;
					} else {
						Coord destination = Locomotion->Destination();
						if (destination == COORD_NONE || PositionCell == Cell(destination)) {
							Assign_Destination(NavCom);
							BEnd(BENCH_PCP);
							return;
						}
					}
				}
			}

			BuildingClass * building = cellptr->Cell_Building();

			if (building != NULL && (building == NavCom || building == TarCom)) {
				if (building->Tag) {
					building->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
				}

				if (Class->IsEngineer) {
					// are we trying to repair a bridge?
					if (building->RTTI == RTTI_BUILDING && building->Class->IsBridgeRepairHut) {
						if (House->Is_Player_Control() && Submit_Radar_Event(RADAREVENT_BRIDGE_REPAIRED, Get_Cell())) Queue_Speech("EVA_BridgeRepaired", -1, -1);

						bool train = false;
						for (int y = -2; y < 3; y++) {
							for (int x = -2; x < 3; x++) {
								Cell cell = Cell(Cell(x, y) + PositionCell);
								IsometricTileType ittype = Map[cell].ITType;
								OverlayType overlay = Map[Cell(Cell(x, y) + PositionCell)].Overlay;
								if ((ittype >= IsometricTileTypeClass::TrainBridgeSet && ittype < IsometricTileTypeClass::TrainBridgeSet + TRAIN_BRIDGE_COUNT) || (overlay >= OVERLAY_LOWBRIDGE_FIRST && overlay <= OVERLAY_LOWBRIDGE_LAST)) {
									train = true;
								}
							}
						}
						if (train) {
							Map.Repair_Train_Bridge(PositionCell);
						} else {
							Map.Repair_Bridge(PositionCell);
						}
						for (int i = Infantry.Count() - 1; i >= 0; i--) {
							Infantry[i]->Detach(building, false);
						}
						building->Scatter_Incoming_Infantry();
					}

					/*
					**	An engineer will either mega-repair a friendly or allied
					**	building or it will damage/capture an enemy building. Whether
					**	it damages or captures depends on how badly damaged the
					**	enemy building is.
					*/
					else if (House->Is_Ally(building) || (building->House->Class->IsMultiplayPassive && building->Class->CanBeOccupied)) {
						TechnoTypeClass const * ttype = building->TClass;
						if (building->Strength == ttype->MaxStrength) {
							Assign_Destination(NULL, true);
							Scatter(building->Center_Coord(), true, true);
							return;
						}
						building->Renovate();
					} else {
						bool iscapturable = false;
						if (building->RTTI == RTTI_BUILDING) {
							iscapturable = building->Class->IsCaptureable;
						}

						if (Session.Type != GAME_NORMAL && Session.Options.CrapEngineers && building->HealthRatio > Rule->ConditionRed) {
							int maxdamage = building->Strength - int(building->TClass->MaxStrength * Rule->ConditionRed / 2);
							int damage = MIN((building->TClass->MaxStrength) * ((1 - Rule->ConditionRed / 2) / 2), maxdamage);
							building->Take_Damage(damage, 0, Rule->C4Warhead, this, true);
						} else if (iscapturable) {
							if (building->CurrentMission == MISSION_DECONSTRUCTION) {
								Assign_Destination(NULL, true);
								Scatter(building->Center_Coord(), true, true);
								return;
							}
							if (building->Tag) {
								building->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
							}
							building->House->IsThieved = true;
							if (Tag && Tag->Is_To_Inherit()) {
								building->Attach_Tag(Tag);
							}
							building->Captured(House);
							building->EnteredByInfType = Class->HeapID;
							building->Scatter_Incoming_Infantry();
						}
					}
				} else {
					if (Class->IsAgent) {

						building->Spied_By(House);
					}
				}
				if (Tag) {
					Tag->Spring(TEVENT_DESTROYED_ANY, this);
				}
				Delete_Me();
				BEnd(BENCH_PCP);
				return;

			} else {
				if (NavCom == NULL) {
					if (CurrentMission != MISSION_GUARD_AREA) {
						Enter_Idle_Mode();
					}
					if (Map[Get_Coord()].Cell_Building()) {
						Scatter(COORD_NONE, true);
					}
				}
			}
		}

		/*
		**	Infantry entering a transport vehicle will break radio contact
		**	at attach itself to the transporter.
		*/
		TechnoClass * techno;
		if (Class->IsVehicleThief) {
			techno = Get_Cell_Ptr()->Cell_Unit(IsOnBridge);
			if (techno == NULL) techno = Get_Cell_Ptr()->Cell_Aircraft(IsOnBridge);
			if (techno == NULL) techno = Get_Cell_Ptr()->Cell_Building();
		} else {
			techno = Get_Cell_Ptr()->Cell_Building();
			if (techno == NULL) techno = Get_Cell_Ptr()->Cell_Aircraft(IsOnBridge);
			if (techno == NULL) techno = Get_Cell_Ptr()->Cell_Unit(IsOnBridge);
		}

		TechnoClass * on_target_cell = NULL;
		if (NavCom != NULL && NavCom->RTTI == RTTI_CELL) {
			CellClass * cellptr = &Map[NavCom->Center_Coord()];
			if (Class->IsVehicleThief) {
				on_target_cell = cellptr->Cell_Unit(true);
				if (on_target_cell == NULL) on_target_cell = cellptr->Cell_Aircraft(true);
				if (on_target_cell == NULL) on_target_cell = cellptr->Cell_Building();
			} else {
				on_target_cell = cellptr->Cell_Building();
				if (on_target_cell == NULL) on_target_cell = cellptr->Cell_Aircraft(true);
				if (on_target_cell == NULL) on_target_cell = cellptr->Cell_Unit(true);
			}
		}

		if (Mission == MISSION_ENTER && techno != NULL && (NavCom == techno || TarCom == techno || on_target_cell != NULL && techno == on_target_cell)) {
			if (techno->Tag) {
				techno->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
			}
			if (techno->RTTI == RTTI_BUILDING) {
				if (techno == Get_Cell_Ptr()->Cell_Building() && Transmit_Message(RADIO_IM_IN) == RADIO_ROGER) {
					Limbo();
					Transmit_Message(RADIO_HELLO, techno);
					techno->Cargo.Attach(this);
					Abort_Motion();
					BEnd(BENCH_PCP);
					return;
				}
			} else {
				if (Get_Cell() == techno->Get_Cell() && techno == NavCom) {
					if (Transmit_Message(RADIO_CAN_LOAD, techno) == RADIO_ROGER) {
						Assign_Archive_Target(NULL);
						IsOnBridge = false;
						Limbo();
						((FootClass *)techno)->Load_Passenger(this);
						Hidden();
					} else {
						Assign_Destination(NULL);
						QueueMission(MISSION_GUARD, false);
						Scatter(COORD_NONE, true, true);
					}
					BEnd(BENCH_PCP);
					return;
				}
			}
		}

		/*
		**	If the infantry unit is entering a cell that contains the building it is trying to
		**	sabotage, then sabotage it.
		*/
		if (Mission == MISSION_SABOTAGE && Class->IsBomber) {
			BuildingClass * building = cellptr->Cell_Building();
			if (building != NULL && building == NavCom) {
				if (building->Tag) {
					building->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
				}
				if (building->Mission != MISSION_DECONSTRUCTION && !building->Unknown_Virtual_32C()) {
					if (building->IsGoingToBlow) {
						Assign_Destination(NULL, true);
						Arm = Rearm_Delay(true);
						Scatter(building->Center_Coord(), true, true);
						return;
					}
					building->IsGoingToBlow = true;
					building->Clicked_As_Target((Rule->C4Delay * TICKS_PER_MINUTE) / 2);
					building->CountDown = Rule->C4Delay * TICKS_PER_MINUTE;
					building->WhomToRepay = this;
				}
				Abort_Motion();
				Do_Uncloak();
				Arm = Rearm_Delay(true);
				Scatter(building->Center_Coord(), true, true);	// RUN AWAY!
				BEnd(BENCH_PCP);
				return;
			} else {
				if (&Map[Center_Coord()] == NavCom) {
					Explosion_Damage(PositionCoord, Rule->BridgeStrength, this, Rule->C4Warhead);

					Stop_Driver();
					Scatter(Adjacent_Cell(PositionCoord, (FacingType)PrimaryFacing.Current().As_Dir8()), true, true);
					QueueMission(MISSION_MOVE, false);

					if (NavCom == NULL || Map[NavCom->Center_Coord()].Land_Type() == LAND_WATER) {
						Mark(MARK_DOWN);		// Needed only so that Tanya will get destroyed by the explosion.
					}
					Explosion_Damage(PositionCoord, Rule->BridgeStrength, NULL, Rule->C4Warhead);
					Explosion_Damage(PositionCoord, Rule->BridgeStrength, NULL, Rule->C4Warhead);
					if (!IsActive) {
						BEnd(BENCH_PCP);
						return;
					}

					Mark(MARK_DOWN);
				}
			}
		}

		/*
		**	If this unit is on a teather, then cut it at this time so that
		**	the "parent" unit is free to proceed. Note that the parent
		**	unit might actually be a building.
		*/
		if (IsTethered) {
			Transmit_Message(RADIO_UNLOADED);

			/*
			**	Special voice play.
			*/
			if (Class->VoiceComment.Count() > 1) {
				VocClass::PlayAt(Class->VoiceComment[1], PositionCoord, 0);
			}

			/*
			**	If the cell is now full of infantry, tell them all to scatter
			**	in order to make room for more.
			*/
			if (IsOnBridge) {
				if ((cellptr->BridgeFlag.Composite & 0x01C) == 0x01C) {
					cellptr->Incoming(COORD_NONE, true, true, true);
				}
			} else {
				if ((cellptr->Flag.Composite & 0x01C) == 0x01C) {
					cellptr->Incoming(COORD_NONE, true, true, false);
				}
			}
		}

		/*
		**	When the infantry reaches the center of the cell, it may begin a new mission.
		*/
		if (MissionQueue == MISSION_NONE && NavCom == NULL && TarCom == NULL && !HasAnyLink()) {
			Enter_Idle_Mode();
		}
		Commence();

		RemoveSight(false, false, false, NULL);
		UpdateSight(false, false, false, NULL);
		Map.RevealArea3(Coord(PositionCoord), 0, LastSightRange + 3, 0);

		/*
		**	If after all is said and done, the unit finishes its move on an impassable cell, then
		**	it must presume that it is in the case of a unit driving onto a bridge that blows up
		**	before the unit completes it's move. In such a case the unit should have been destroyed
		**	anyway, so blow it up now.
		*/
		CellClass * cellptr = &Map[Get_Coord()];
		LandType land = cellptr->Land_Type();
		if (!Locomotion->Is_Moving() && !Class->IsBomber && !Has_Ability(ABILITY_C4) && (land == LAND_ROCK || land == LAND_WATER) && (!IsOnBridge || !cellptr->IsUnderBridge)) {
			int damage = Strength;
			Take_Damage(damage, 0, Rule->C4Warhead, NULL, true);
			BEnd(BENCH_PCP);
			return;
		}


	}

	if (IsActive) {
		BASECLASS::Per_Cell_Process(why);
	}
	BEnd(BENCH_PCP);
}


/***********************************************************************************************
 * InfantryClass::Detach -- Removes the specified target from targeting computer.              *
 *                                                                                             *
 *    This is a support routine that removes the target specified from any targeting or        *
 *    navigation computers. When a target is destroyed or removed from the game system,        *
 *    the target must be removed from any tracking systems of the other units. This routine    *
 *    handles removal for infantry units.                                                      *
 *                                                                                             *
 * INPUT:   target   -- The target to remove from the infantry unit's tracking systems.        *
 *                                                                                             *
 *          all      -- Is the target going away for good as opposed to just cloaking/hiding?  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Detach(AbstractClass const * target, bool all)
{
	BASECLASS::Detach(target, all);
	if (Class == target) {
		Class = NULL;
	}
}


/***********************************************************************************************
 * InfantryClass::Assign_Destination -- Gives the infantry a movement destination.             *
 *                                                                                             *
 *    This routine updates the infantry's navigation computer so that the infantry will        *
 *    travel to the destination target specified.                                              *
 *                                                                                             *
 * INPUT:   target   -- The target to have the infantry unit move to.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Assign_Destination(AbstractClass * target, bool immediate)
{
	if (Owner_HouseClass()->Is_Human_Player()) { // RA2: a deploying human soldier ignores new destinations
		if (Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE) { // RA2: as above
			return;
		}
	}

	if (Class->JumpJet && target != NULL) { // RA2: a jumpjet already in the target cell stays
		if (Cell(target->Center_Coord()) == Cell(Center_Coord())) { // RA2: as above
			return;
		}
	}

	/*
	**	Special flag so that infantry will start heading in the right direction immediately.
	*/
	if (Locomotion->Is_Moving() && target != NULL && Map[Center_Coord()].Is_Clear_To_Move(Class->Speed, true, false, -1, MZONE_NORMAL, -1, true)) { // RA2: stops the driver unless it is attacking its target
		if (Class->JumpJet || CurrentMission != MISSION_ATTACK || NavCom != target) { // RA2: as above
			Stop_Driver();
		}
	}

	/*
	**	When telling an infantry soldier to move to a location twice, then this
	**	means that movement is more important than safety. Get up and run!
	*/
	if (House->Is_Human_Player() && target != NULL && NavCom == target && IsProne && !Class->IsFraidyCat && !Class->IsCyborg) {
		Do_Action(DO_GET_UP);
	}

	/*
	**	Handle entry logic here.
	*/
	if (Get_Mission() == MISSION_ENTER || MissionQueue == MISSION_ENTER) {

		/*
		**	If not already in radio contact (presumed with the transport), then
		**	either try to establish contact if allowed, or just move close and
		**	wait until radio contact can be established.
		*/
		if (!HasAnyLink()) {
			TechnoClass * techno = Dynamic_Cast<TechnoClass *>(target);
			if (techno != NULL) {

				/*
				**	Determine if the transport is already in radio contact. If so, then just move
				**	toward the transport and try to establish contact at a later time.
				*/
				if (techno->HasAnyLink()) {
					if (techno->Fetch_RTTI() == RTTI_UNIT) { // RA2: a busy vehicle transport is queued up for
						QueueUpToEnter = As_Unit(target); // RA2: as above
						ArchiveTarget = NULL;
						target = NULL;
					} else if (techno->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
						QueueUpToEnter = As_Building(target); // RA2: as above
						ArchiveTarget = NULL;
						target = NULL;
					} else {
						ArchiveTarget = target;
					}
				} else {
					if (Transmit_Message(RADIO_HELLO, techno) == RADIO_ROGER) {
						RadioMessageType reply = Transmit_Message(RADIO_DOCKING); // RA2: leave at once when docking begins
						if (reply == RADIO_DOCKING) { // RA2: as above
							return;
						}
						if (reply != RADIO_ROGER) { // RA2: as above
							Transmit_Message(RADIO_OVER_OUT);
						}
					}
				}
			}
		} else {
			Path[0] = FACING_NONE;
		}
	} else {
		Path[0] = FACING_NONE;
	}

	if (target != NULL && Class->JumpJet && Locomotion->Is_Moving()) { // RA2: a walking jumpjet queues the target and heads for the target cell
		IPersistPtr persist(Locomotion);
		CLSID clsid;
		persist->GetClassID(&clsid);
		if (clsid == CLSID_WalkLocomotion) {
			NavQueue.Add_Head(target);
			target = Get_Target_Cell_Ptr();
			if (target != NULL && ((CellClass *)target)->IsUnderBridge) {
				target = NULL;
			}
		}
	}

	if (Class->JumpJet && target != NULL && !Locomotion->Is_Moving()) { // RA2: jumpjets switch between walking and flying here
		bool should_fly = Should_JumpJet_Fly(Destination_Coord().As_Cell(), target->Center_Coord().As_Cell());
		bool is_jumpjet = Is_JumpJet(); // RA2: as above
		if (Class->HoverAttack) { // RA2: as above
			should_fly = true; // RA2: as above
		}
		if (is_jumpjet) { // RA2: as above
			if (!should_fly) {
				IPiggybackPtr piggy(Locomotion);
				if (piggy != NULL && piggy->Is_Piggybacking() && piggy->Is_Ok_To_End()) { // RA2: as above
					piggy->End_Piggyback(&Locomotion);
				}
				ILocomotionPtr walk(CLSID_WalkLocomotion);
				walk->Link_To_Object(this);
				piggy = IPiggybackPtr(walk);
				if (piggy != NULL) {
					Begin_Piggyback_Locomotion(Locomotion, piggy, walk); // RA2: as in ra2-decomp's twin
				}
			}
		} else if (should_fly) { // RA2: the executable's destination assignment (jumpjet, queue-up transports)
			IPiggybackPtr piggy(Locomotion);
			if (piggy != NULL && piggy->Is_Piggybacking() && piggy->Is_Ok_To_End()) { // RA2: as above
				piggy->End_Piggyback(&Locomotion);
			}
		}
	}

	BASECLASS::Assign_Destination(target, immediate);
}


/***********************************************************************************************
 * InfantryClass::Assign_Target -- Gives the infantry a combat target.                         *
 *                                                                                             *
 *    This routine will update the infantry's targeting computer so that it will try to        *
 *    attack the target specified. This might result in it moving to be within range and thus  *
 *    also cause adjustment of the navigation computer.                                        *
 *                                                                                             *
 * INPUT:   target   -- The target that this infantry should attack.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   06/30/1995 JLB : Tries to capture target if possible.                                     *
 *=============================================================================================*/
void InfantryClass::Assign_Target(AbstractClass * target)
{
	if (target != TarCom && Strength > 0) {
		IsFiring = false;
		if (Is_Deploying()) {
			Do_Action(DO_DEPLOYED, 0, 0);
		} else if (IsProne) {
			Do_Action(DO_PRONE, 0, 0);
		} else {
			Do_Action(DO_STAND_READY, 0, 0);
		}
	}
	if (!Is_Deploying() || Class->DeployFire) {
		Path[0] = FACING_NONE;
		TechnoClass::Assign_Target(target);
		if (NavCom == 0 && Class->IsCapture) {
			AbstractClass *object = target;
			BuildingClass *building = (object != 0 && object->Fetch_RTTI() == RTTI_BUILDING) ? (BuildingClass *)object : 0;
			if (!Class->IsAgent && building != 0 && building->Class->IsCaptureable) {
				Assign_Destination(object, true);
			} else if (Class->IsAgent && building != 0 && building->Class->Spyable) {
				Assign_Destination(object, true);
			}
		}
		IsOverpowering = false;
	}
}



/// <summary>
/// Handles the infantry traveling through a subterranean tunnel.
/// This routine walks the soldier along the tube it entered, following the tube's own
/// path and sinking or rising with it along the way. When the far mouth is reached the
/// infantry is set back down on the map, marked down into its cell, and given a per
/// cell process so that it rejoins the world properly. Anything standing in the way of
/// the exit is told to scatter first.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 1759 bytes differ against the image-derived bound 005025c0 (1876 B; ours 1920 B), 2026-11-23 (lane F43): the height step through ra2-decomp's tube accessors and the step statement in UnitClass::Tunnel_AI's form (the adjacent cell a temporary of the `Coord delta` statement), which give the image's frame 0x80, its slot sharing and its called operator-; left: the first distance's Length is a call in the image and expanded here. Scratch: six empty inline sites at the end of the root give the row exact (560 = 560 instructions, every slot); one to four do not, and sites at the top turn operator- back. So the image's root has about six more sites late or lighter text; ra2-decomp reads the same on the Yuri's Revenge twin 0x51B350 (five sites after the Distance statement; its Get_Enter/Get_Dir accessors are invented). Both directions through Red Alert's inline ObjectClass::Direction(Coord const &) (lane F35)
void InfantryClass::Tunnel_AI(void)
{
	bool finished = Tubes[CurrentTube]->Dirs[CurrentTubeDir] == FACING_NONE;
	if (!finished) {
		int distance = Distance(LastTubeCoord);
		TubeClass * tube = Tubes[CurrentTube];
		DirType direction = Direction(LastTubeCoord);
		int speed = Class->MaxSpeed;

		int height_step = (Map.Get_Height_GL(tube->Get_Exit()) - Map.Get_Height_GL(Cell(tube->Enter))) / tube->Get_Count();

		if (distance <= speed) {
			CurrentTubeDir++;
			FacingType tube_dir = tube->Dirs[CurrentTubeDir];
			finished = tube_dir == FACING_NONE;
			PositionCoord = LastTubeCoord;

			if (!finished) {
				Coord delta = Map[Adjacent_Cell(LastTubeCoord, tube->Dirs[CurrentTubeDir])].Cell_Coord() - Map[LastTubeCoord].Cell_Coord();
				LastTubeCoord = LastTubeCoord + Coord(delta.X, delta.Y, 0);
				LastTubeCoord.Z += height_step;

				DirType dir = Direction(LastTubeCoord);
				Coord new_coord = Move_Coord(PositionCoord, dir, speed - distance);
				double divisor;
				if (tube_dir % 2) {
					divisor = CELL_LEPTON_H * M_SQRT2;
				} else {
					divisor = CELL_LEPTON_H;
				}
				new_coord.Z += (speed - distance) / divisor * height_step;
				PositionCoord = new_coord;
				return;
			}
		} else {
			FacingType tube_dir = tube->Dirs[CurrentTubeDir];
			Coord new_coord = Move_Coord(Get_Coord(), direction, speed);
			double divisor;
			if (tube_dir % 2) {
				divisor = CELL_LEPTON_H * M_SQRT2;
			} else {
				divisor = CELL_LEPTON_H;
			}
			new_coord.Z += speed / divisor * height_step;
			PositionCoord = new_coord;
			return;
		}
	}

	if (finished) {
		CellClass * cellptr = &Map[Get_Coord()];
		ObjectClass * occupier = cellptr->Cell_Occupier();
		if (Can_Enter_Cell(cellptr, FACING_NONE, 0) != MOVE_OK) {

			/*
			**	RA2: the exit cell is blocked: every idle vehicle or infantryman there is shooed
			**	away (from a copy of the occupier list, since scattering relinks it) and the
			**	infantry waits underground.
			*/
			DynamicVectorClass<ObjectClass *> occupiers;
			while (occupier != NULL) {
				occupiers.Add(occupier);
				occupier = occupier->Next;
			}
			for (int index = 0; index < occupiers.Count(); index++) {
				ObjectClass * object = occupiers[index];
				if (object->RTTI == RTTI_UNIT || object->RTTI == RTTI_INFANTRY) {
					if (!((FootClass *)object)->Locomotion->Is_Moving()) {
						object->Scatter(Coord(0, 0, 0), true, true);
					}
				}
			}
			Set_Speed(0);
		} else {
			Coord spot = cellptr->Closest_Free_Spot(Get_Coord());
			spot.Z = Map.Get_Height_GL(spot);
			PositionCoord = spot;

			Locomotion->Force_Immediate_Destination(COORD_NONE);
			Locomotion->Stop_Movement_Animation();
			Mark(MARK_DOWN);
			Set_Occupy_Bit(Get_Coord());

			if (NavCom == cellptr) {
				Scatter(Coord(0, 0, 0), true, true);
			} else {
				Set_Speed(1);
			}
			CurrentTube = TUBE_NONE;
			IsPlanningToLook = true;
			Per_Cell_Process(PCP_END);
		}
	}
}
#endif


/***********************************************************************************************
 * InfantryClass::AI -- Handles the infantry non-graphic related AI processing.                *
 *                                                                                             *
 *    This routine is used to handle the non-graphic AI processing the infantry requires.      *
 *    Call this routine ONCE per game frame.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   08/14/1996 JLB : Simplified.                                                              *
 *=============================================================================================*/
void InfantryClass::AI(void)
{
	if (CurrentTube >= 0) {
		Tunnel_AI();
		Update_Radar_Position();
		return;
	}

	/*
	**	RA2: a chronoshifting soldier sparkles every 24th frame, and a chrono-warping one
	**	cannot do anything else.
	*/
	if ((IsBeingWarpedOut() || IsWarpingIn()) && Frame % 24 == 0 && Rule->ChronoSparkle1 != NULL) { // RA2: the executable's AI order (chrono warp, guard area, inline Edge_Of_World_AI)
		new AnimClass(Rule->ChronoSparkle1, PositionCoord + Coord(120, 120, 0), 0, 1, (ShapeFlags_Type)(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
	}
	if (TemporalTargetingMe != NULL) { // RA2: as above
		TemporalTargetingMe->AI(); // RA2: as above
	}
	if (IsWarpingIn() || (IsBeingWarpedOut() && IsImmobilized)) { // RA2: as above
		Locomotion->Process(); // RA2: as above
	}
	if (IsBeingWarpedOut()) { // RA2: as above
		if (TarCom != NULL) {
			Assign_Target(NULL);
		}
		if (NavCom != NULL) {
			Assign_Destination(NULL, true); // RA2: the executable's AI order (chrono warp, guard area, inline Edge_Of_World_AI)
		}
		return;
	}

	/*
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (Ready_To_Commence()) {
		if (Mission == MISSION_NONE && MissionQueue == MISSION_NONE) Enter_Idle_Mode();
		Commence();
	}

	if (Strength <= 0) {
		if (!Is_Dying()) { // RA2: Is_Dying() instead of the death sequences and Tumble
			Strength = 1;
		}
	}

	BASECLASS::AI();

	if (!IsActive) {
		return;
	}

	if (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA) { // RA2: guard area as well
		BuildingClass *building = Map[(Coord const &)PositionCoord].Cell_Building();
		if (building != NULL) {

			if (!building->Class->IsInvisibleInGame) {
				bool scatter = false;
				if (building->Class->IsLaserFence) {
					scatter = !(building->LaserFenceFrame == 12 || building->LaserFenceFrame == 8);
				} else if (building->Class->IsFirestormWall) {
					if (building->House->FirestormDefenseActivated) {
						scatter = true;
					}
				} else {
					if (!(building->Class->IsGate && building->Is_Gate_Open())) {
						scatter = true;
					}
				}

				if (scatter) {
					Scatter(COORD_NONE, true, true);
				}
			}
		}
	}

	/*
	**	Infantry that are not on the ground should always be redrawn. Such is
	**	the case when they are parachuting to the ground.
	*/
	if (In_Which_Layer() != LAYER_GROUND) {
		Mark(MARK_CHANGE);
	}

	/*
	**	Special hack to make sure that if this infantry is in firing animation, but the
	**	stage class isn't set, then abort the firing flag.
	*/
	if (IsFiring && Fetch_Rate() == 0) {
		IsFiring = false;
		if (Is_Deploying()) { // RA2: a deployed soldier returns to the deployed sequence
			Do_Action(DO_DEPLOYED); // RA2: as above
		} else {
			Do_Action(DO_STAND_READY);
		}
	}

	/*
	**	Delete this unit if it finds itself off the edge of the map and it is in
	**	guard or other static mission mode.
	*/
	if (Edge_Of_World_AI()) {
		return;
	}

	if (Theft_AI()){
		return;
	}

	/*
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (Ready_To_Commence()) {
		if (Mission == MISSION_NONE && MissionQueue == MISSION_NONE) Enter_Idle_Mode();
		Commence();
	}

	/*
	**	Handle any infantry fear logic or related actions.
	*/
	Fear_AI();

	/*
	**	Special victory dance action.
	*/
	if (NavCom == NULL && !IsProne && IsStoked && Comment == 0) {
		IsStoked = false;
	}

	/*
	**	Determine if this infantry unit should fire off an
	**	attack or not.
	*/
	Firing_AI();

	if (IsActive) { // RA2: the animation and movement steps nest on IsActive
		Doing_AI();
		if (IsActive) {
			Movement_AI();
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Can_Enter_Cell -- Determines if the infantry can enter the cell specified.   *
 *                                                                                             *
 *    This routine is used to examine the cell specified and determine if the infantry is      *
 *    allowed to enter it. It is used by the path finding algorithm.                           *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns the type of blockage in the cell.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType InfantryClass::Can_Enter_Cell(CellClass const * cellptr, FacingType dir, int cell_height, CellClass const * adj_cellptr, bool) const
{
	/*
	**	If we are moving into an illegal cell, then we can't do that.
	*/
	//if ((unsigned)cell >= MAP_CELL_TOTAL) return(MOVE_NO);

	bool isbridge = cellptr->IsUnderBridge && (cell_height == -1 || abs(cell_height - cellptr->Height) > 1);
	unsigned char flag = cellptr->Flag.Composite;
	HousesType inftype = cellptr->InfType;
	bool vehicleflag = cellptr->Flag.Occupy.Vehicle;

	TubeClass *tube = cellptr->Get_Tunnel();

	if (dir == FACING_COUNT) {
		if (tube != NULL && Cell(tube->Exit) != Cell(tube->Enter)) {
			return(MOVE_OK);
		}
		return(MOVE_NO);
	}

	if (tube != NULL) {
		int a = abs(dir - tube->EnterDir);
		if (a > 2 && a < 6 && dir != FACING_NONE) {
			return(MOVE_NO);
		}
	}

	TubeClass *tube_adj = cellptr->Adjacent_Cell(Facing_Sub(dir, FACING_180)).Get_Tunnel();
	if (tube_adj != NULL) {
		int a = abs(Facing_Sub(dir, FACING_180) - tube_adj->EnterDir);
		if (a > FACING_90 && a < FACING_270 && dir != FACING_NONE) {
			return(MOVE_NO);
		}
	}

	if (cell_height - cellptr->Height > 4) {
		return(MOVE_OK);
	}

	if (Can_Reach(cellptr, dir, cell_height, isbridge, adj_cellptr) == MOVE_NO) {
		return(MOVE_NO);
	}

	if (cell_height != -1 && cellptr->IsUnderBridge && cell_height == cellptr->Height + BRIDGE_CELL_HEIGHT) {
		flag = cellptr->BridgeFlag.Composite;
		inftype = cellptr->BridgeInfType;
		vehicleflag = cellptr->BridgeFlag.Occupy.Vehicle;
	}

	/*
	**	If moving off the edge of the map, then consider that an illegal move.
	*/
	if (IsLocked && !ScenarioInit && !Map.In_Local_Radar(cellptr) && !Is_Allowed_To_Leave_Map()) {
		return(MOVE_NO);
	}

	MoveType retval = MOVE_OK;

	/*
	**	Walls are considered impassable for infantry UNLESS the wall has a hole
	**	in it.
	*/
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const & otype = *OverlayTypes[cellptr->Overlay];

		if (otype.IsCrate && !House->Is_Human_Player()) {
			return(MOVE_NO);
		}

		if (otype.IsWall) {
			if ((cellptr->OverlayData / 16) != otype.DamageLevels) {

				/*
				**	If the wall can be destroyed, then return this fact instead of
				**	a complete failure to enter.
				*/
				if (Is_Weapon_Equipped() && PrimaryWeapon->Is_Wall_Destroyer()) {
					if (House->Is_Ally(cellptr->Owner)) {
						retval = MOVE_FRIENDLY_DESTROYABLE;
					} else {
						retval = MOVE_DESTROYABLE;
					}
				} else {
					return(MOVE_NO);
				}
			}
		}
	}

	/*
	**	Loop through all of the objects in the square setting a bit
	**	for how they affect movement.
	*/
	int trynum = 0;
	ObjectClass * obj = cellptr->Cell_Occupier(isbridge);
	while (obj != NULL) {

		if (obj != (ObjectClass *)this) {

			/*
			**	RA2: a parasite that has its victim in this very cell may enter it.
			*/
			if (ParasiteImUsing != NULL && ParasiteImUsing->Victim != NULL) { // RA2: the executable's occupant checks (parasite, iron curtain, eaten mission)
				if (obj->Get_Cell() == ParasiteImUsing->Victim->Get_Cell()) { // RA2: as above
					return(MOVE_OK);
				}
			}

			/*
			**	Always allow movement if the cell is the object to be captured or sabotaged.
			*/
			if (Get_Mission() == MISSION_ENTER || Get_Mission() == MISSION_CAPTURE || Get_Mission() == MISSION_EATEN || // RA2: the executable's occupant checks (parasite, iron curtain, eaten mission)
				(Get_Mission() == MISSION_SABOTAGE && Class->IsBomber) || // RA2: as above
				((Get_Mission() == MISSION_GUARD_AREA || Get_Mission() == MISSION_PATROL || Get_Mission() == MISSION_GUARD) && Class->IsEngineer)) { // RA2: Get_Mission(), Eaten, and sabotage only for bombers
				if (obj == NavCom || (&Map[(Coord const &)obj->PositionCoord] == NavCom || obj == TarCom)) {
					if (!obj->IsIronCurtained()) { // RA2: an invulnerable object is not entered
						float cost = Ground[cellptr->Land_Type()].Cost[Class->Speed]; // RA2: the cost is read first
						bool unrestricted = IsTethered || isbridge; // RA2: as above
						if (!unrestricted && cost == 0) { // RA2: as above
							return(MOVE_NO);
						}
						return(MOVE_OK);
					}
					return(MOVE_NO);
				}
				BuildingClass *building = cellptr->Cell_Building();
				if (building != NULL && obj != building) {
					if (building == NavCom || &Map[building->Center_Coord()] == NavCom || building == TarCom) { // RA2: one test
						obj = obj->Next;
						continue;
					}
				}
			}

			/*
			**	Guard area should not allow the guarding unit to enter the cell with the
			**	guarded unit.
			*/
			//if (Get_Mission() == MISSION_GUARD_AREA && ArchiveTarget == obj && Is_Target_Unit(ArchiveTarget)) {
			if (Mission == MISSION_GUARD_AREA && ArchiveTarget == obj && ArchiveTarget->RTTI == RTTI_UNIT) {
				return(MOVE_NO);
			}

			if (Class->IsVehicleThief && obj->Considered_Vehicle() && NavCom == obj && !obj->TClass->IsTrain) {
				return(MOVE_OK);
			}

			/*
			**	If object is a land mine, allow movement
			*/
			if (obj->RTTI == RTTI_BUILDING) {
				BuildingClass *building = (BuildingClass *)obj;
				if (building->Class->IsInvisibleInGame) {
					obj = obj->Next;
					continue;
				}

				if (building->Class->IsLaserFence && (building->LaserFenceFrame == 12 || building->LaserFenceFrame == 8)) {
					obj = obj->Next;
					continue;
				}

				if (building->Class->IsFirestormWall) {
					if (building->House->FirestormDefenseActivated) {
						return(MOVE_NO);
					}
					obj = obj->Next;
					continue;
				}

				if (building->Class->IsGate) {
					if (!building->Is_Gate_Open()) {
						if (building->House->Is_Ally(House)) {
							if (retval < MOVE_CLOSED_GATE) {
								retval = MOVE_CLOSED_GATE;
							}
						} else {
							if (!Is_Weapon_Equipped()) {
								return(MOVE_NO);
							}
							if (retval < MOVE_DESTROYABLE) {
								retval = MOVE_DESTROYABLE;
							}
						}
					}
					obj = obj->Next;
					continue;
				}
			}

			/*
			**	Special case check so that a landed aircraft that is in radio contact, will not block
			**	a capture attempt. It is presumed that this case happens when a helicopter is landed
			**	at a helipad.
			*/
//			if ((Mission != MISSION_CAPTURE && Mission != MISSION_SABOTAGE) || obj->What_Am_I() != RTTI_AIRCRAFT || !((AircraftClass *)obj)->In_Radio_Contact()) {

				/*
				**	Special check to always allow entry into the building that this infantry
				**	is trying to capture.
				*/
//				if (obj->What_Am_I() == RTTI_BUILDING || obj->What_Am_I() == RTTI_AIRCRAFT || obj->What_Am_I() == RTTI_UNIT) {
//					if ((Mission == MISSION_CAPTURE || Mission == MISSION_SABOTAGE) && (obj == NavCom || obj == TarCom)) {
//						return(MOVE_OK);
//					}
//				}

				/*
				**	Special check to always allow entry into the building that this infantry
				**	is trying to capture.
				*/
				if (Get_Mission() == MISSION_ENTER && obj == NavCom && IsTethered) { // RA2: the executable's occupant checks (parasite, iron curtain, eaten mission)
					return(MOVE_OK);
				}

				/*
				**	Allied objects block movement using different rules than for enemy
				**	objects.
				*/
				if (House->Is_Ally(obj) || ScenarioInit) {
					switch ((RTTIType)obj->RTTI) {

						/*
						**	A unit blocks as either a moving blockage or a stationary temp blockage.
						**	This depends on whether the unit is currently moving or not.
						*/
						case RTTI_UNIT:
							if (((UnitClass *)obj)->Locomotion->Is_Moving() || ((UnitClass *)obj)->NavCom != NULL) {
								if (((UnitClass *)obj)->IsOccupyingCell || ((UnitClass *)obj)->Locomotion->Will_Jump_Tracks()) {
									if (retval < MOVE_MOVING_BLOCK) retval = MOVE_MOVING_BLOCK;
								}
							} else {
								if (retval < MOVE_TEMP) retval = MOVE_TEMP;
							}
							break;

						/*
						**	Aircraft and buildings always block movement. If for some reason there is an
						**	allied terrain object, that blocks movement as well.
						*/
						//case RTTI_TERRAIN:
						case RTTI_AIRCRAFT:
						case RTTI_BUILDING:
							return(MOVE_NO);

						case RTTI_INFANTRY:
							if (!((InfantryClass *)obj)->Locomotion->Is_Moving()) {
								trynum++;
							}
							break;

						case RTTI_TERRAIN:
						default:
							break;
					}

				} else {

					/*
					**	Cloaked enemy objects are not considered if this is a Find_Path()
					**	call.
					*/
					TechnoClass *tech = Dynamic_Cast<TechnoClass *>(obj);
					if (tech == NULL || tech->Cloak != CLOAKED) {

						RTTIType rtti = obj->RTTI;

						/*
						**	Any non-allied blockage is considered impassible if the infantry
						**	is not equipped with a weapon.
						*/
						if (Combat_Damage() <= 0 && rtti != RTTI_TERRAIN) return(MOVE_NO);

						/*
						**	Some kinds of terrain are considered destroyable if the infantry is equipped
						**	with the weapon that can destroy it. Otherwise, the terrain is considered
						**	impassable.
						*/
						switch (rtti) {
							case RTTI_TERRAIN:
#ifdef OBSOLETE
								if (((TerrainClass *)obj)->Class->Armor == ARMOR_WOOD &&
										Class->PrimaryWeapon->WarheadPtr->IsWoodDestroyer) {

									if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
								} else {
									return(MOVE_NO);
								}
								break;
#else
								break;
#endif
							case RTTI_INFANTRY:
								if (obj->IsDisguisedAs(House)) { // RA2: a disguised enemy infantryman is passable
									retval = MOVE_TEMP;
								}
								break;

							case RTTI_BUILDING:
								if (((BuildingClass *)obj)->Class->IsBridgeRepairHut) {
									return(MOVE_NO);
								}
								if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
								break;

							default:
								if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
								break;
						}
					} else {
						if (retval < MOVE_CLOAK) retval = MOVE_CLOAK;
					}
				}
//			}
		}

		/*
		**	Move to next object in chain.
		*/
		obj = obj->Next;
	}

	/*
	**	If foot soldiers cannot travel on the cell -- consider it impassable.
	*/
	if (/*retval == MOVE_OK &&*/ !IsTethered && !isbridge && Ground[cellptr->Land_Type()].Cost[Class->Speed] == 0) {

#ifdef OBSOLETE
		/*
		** Special case - if it's an engineer, and the cell under consideration
		** is his NavCom, and his mission is mission_capture, then he's most
		** likely moving to his final destination to repair a bridge, so we
		** should let him.
		*/
		if (*this == INFANTRY_RENOVATOR && Is_Target_Cell(TarCom) && (cell == ::As_Cell(NavCom)) && (cellptr->TType == TEMPLATE_BRIDGE1D || cellptr->TType == TEMPLATE_BRIDGE2D || (cellptr->TType >= TEMPLATE_BRIDGE_1C && cellptr->TType <= TEMPLATE_BRIDGE_3E) ) ) {
			return(MOVE_OK);
		}
#endif
		return(MOVE_NO);
	}

	/*
	**	if a unit has the cell reserved then we just can't go in there.
	*/
	if (retval == MOVE_OK && vehicleflag) {
		return(MOVE_MOVING_BLOCK);
	}

	/*
	**	if a block of infantry has the cell reserved then there are two
	**	possibilities...
	*/
	if (inftype != HOUSE_NONE) {
		if (House->Is_Ally(inftype)) {
			if ((flag & 0x1C) == 0x1C) {
				if (retval < MOVE_MOVING_BLOCK) {
					if (trynum == 3) {
						retval = MOVE_TEMP;
					} else {
						retval = MOVE_MOVING_BLOCK;
					}
				}
			}
		} else {
			if (Combat_Damage() > 0) {
				if (retval < MOVE_DESTROYABLE) {
					retval = MOVE_DESTROYABLE;
				}
			} else {
				return(MOVE_NO);
			}
		}
	}

	/*
	**	If it is still ok to move the infantry, then perform the last check
	**	to see if the cell is already full of infantry.
	*/
	if (retval == MOVE_OK && (flag & 0x1C) == 0x1C) {
		return(MOVE_NO);
	}

	/*
	**	Return with the most severe reason why this cell would be impassable.
	*/
	return(retval);
}


/***********************************************************************************************
 * InfantryClass::Can_Fire -- Can the infantry fire its weapon?                                *
 *                                                                                             *
 *    Determines if the infantry unit can fire on the target. If it can't fire, then the       *
 *    reason why is returned.                                                                  *
 *                                                                                             *
 * INPUT:   target   -- The target to determine if the infantry can fire upon.                 *
 *                                                                                             *
 * OUTPUT:  Returns the fire error type that indicates if the infantry can fire and if it      *
 *          can't, why not.                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   06/27/1995 JLB : Flame thrower can fire while prone now.                                  *
 *=============================================================================================*/
FireErrorType InfantryClass::Can_Fire(AbstractClass * target, int which, bool ignoreRange) const // RA2: ignoreRange
{
	if (Is_Dying()) { // RA2: a dying soldier cannot fire
		return(FIRE_CANT); // RA2: as above
	}

	/*
	**	If a medic is shooting at a healed target, let's declare the target
	**	illegal so he won't be constantly healing healed infantrymen.
	*/
	if (Combat_Damage() < 0) {
		if (target == NULL || target->Fetch_RTTI() != RTTI_INFANTRY || !(((ObjectClass *)target)->Get_Health_Ratio() < Rule->ConditionGreen)) { // RA2: Get_Health_Ratio() instead of the member
			return(FIRE_ILLEGAL);
		}
	}

	FireErrorType result = BASECLASS::Can_Fire(target, which, ignoreRange); // RA2: the base check comes first; the rest only refines FIRE_OK
	if (result == FIRE_OK) { // RA2: as above
		/*
		**	If this unit cannot fire while moving, then bail.
		*/
		if (Speed > 0.1 || (NavCom != NULL && Doing != DO_NOTHING && !MasterDoControls[Doing].Interrupt)) {
			return(FIRE_MOVING);
		}

		WeaponTypeClass const * weapon = Get_Class_Weapon_Data(which)->Weapon;
		if (weapon != NULL) { // RA2: the executable's checks, in its order
			if (weapon->UseFireParticles && NavCom != NULL) { // RA2: as above
				return(FIRE_MOVING);
			}
			if (weapon->AreaFire && target != Get_Cell_Ptr()) { // RA2: an area weapon needs the cell as its target
				return(FIRE_ILLEGAL);
			}
			WarheadTypeClass const * warhead = weapon->WarheadPtr; // RA2: no Ivan bomb on a target that already carries one
			if (warhead != NULL && warhead->IvanBomb && target != NULL && (target->AbstractFlags & ABSTRACT_OBJECT) && ((ObjectClass *)target)->AttachedBomb != NULL) { // RA2: as above
				return(FIRE_ILLEGAL);
			}
		}

		FireErrorType error = Locomotion->Can_Fire();
		if (error) {
			return(error);
		}
	}
	return(result);
}


/***********************************************************************************************
 * InfantryClass::Enter_Idle_Mode -- The infantry unit enters idle mode by this routine.       *
 *                                                                                             *
 *    Use this routine when the infantry unit as accomplished its task and needs to find       *
 *    something to do. The default behavior is to enter some idle state such as guarding.      *
 *                                                                                             *
 * INPUT:   initial  -- Is this called when the unit just leaves a factory or is initially     *
 *                      or is initially placed on the map?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Enter_Idle_Mode(bool initial, bool resume_waypoint)
{
	bool result = BASECLASS::Enter_Idle_Mode(initial, resume_waypoint);
	if (HaveMegaMission()) {
		return result;
	}
	if (Mission == MISSION_WAIT) {
		return result;
	}
	MissionType mission;
	if (TarCom != 0) {
		mission = MISSION_ATTACK;
		if (Mission == MISSION_SABOTAGE) {
			mission = MISSION_SABOTAGE;
		}
		if (Mission == MISSION_CAPTURE) {
			mission = MISSION_CAPTURE;
		}
	} else {
		Handle_Navigation_List();
		if (NavCom != 0) {
			mission = MISSION_MOVE;
			if (Mission == MISSION_CAPTURE) {
				mission = MISSION_CAPTURE;
			}
			if (Mission == MISSION_SABOTAGE) {
				mission = MISSION_SABOTAGE;
			}
		} else {
			if (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA) {
				return(false);
			}
			if (Mission != MISSION_NONE && (Current_Mission_Control().IsZombie || Current_Mission_Control().IsParalyzed)) {
				return(false);
			}
			if (!House->Is_Human_Player() && Team == 0) {
				if (House->IQ < Rule->IQGuardArea && CurrentMission != MISSION_GUARD_AREA) {
					mission = MISSION_GUARD;
				} else if (!Is_Weapon_Equipped() && !Class->IsEngineer && !Class->IsVehicleThief) {
					mission = MISSION_GUARD;
				} else {
					mission = MISSION_GUARD_AREA;
				}
			} else if (CurrentMission != MISSION_GUARD_AREA && ((!Has_Ability(ABILITY_GUARD_AREA) && !Class->DefaultToGuardArea) || Team != 0)) {
				mission = MISSION_GUARD;
			} else {
				mission = MISSION_GUARD_AREA;
			}
		}
	}
	if (CurrentMission != MISSION_PATROL && CurrentMission != MISSION_GUARD_AREA && mission != MISSION_NONE) {
		QueueMission(mission, false);
	}
	return result;
}



/***********************************************************************************************
 * InfantryClass::Random_Animate -- Randomly animate the infantry (maybe)                      *
 *                                                                                             *
 *    This routine is the random animator initiator for infantry units. This routine should    *
 *    be called regularly. On occasion, it will cause the infantry to go into an idle          *
 *    animation.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   12/13/1994 JLB : Does random facing change.                                               *
 *   07/02/1995 JLB : Nikoomba special effects.                                                *
 *=============================================================================================*/
bool InfantryClass::Random_Animate(void)
{
	if (Is_Ready_To_Random_Animate()) {
		IdleTimer = (int)Random_Double(Rule->RandomAnimateTime * (TICKS_PER_MINUTE/2), Rule->RandomAnimateTime * (TICKS_PER_MINUTE*2));

		/*
		**	Scared infantry will always follow the golden rule of civilians;
		**		"When in darkness or in doubt, run in circles, scream, and shout!"
		*/
		if (Class->IsFraidyCat && !House->Is_Human_Player() && Fear > 50) { // RA2: scared from 50 up (TibSun's FEAR_ANXIOUS is 10)
			Scatter(COORD_NONE, true);
			return(true);
		}

		int pick = Random_Pick(0, 10); // RA2: the pick is a local; cows scatter a lot more often
		bool cow = Is_Type_Named("COW"); // RA2: as above
		if (cow && Random_Pick(0, 10) < 5) { // RA2: as above
			pick = 8; // RA2: as above
		}

		switch (pick) { // RA2: as above
			case 0:
				break;

			case 3:
			case 4:
			case 5:
				Do_Action(DO_IDLE1);
				break;

			case 6:
				PrimaryFacing.Set(DirType(DirType(Facing_Dir(Random_Pick(FACING_N, FACING_NW))).As_Dir256())); // RA2: the facing goes through As_Dir256
				break;

			case 1:
			case 2:
			case 7:
				Do_Action(DO_IDLE2);
				if (!IsSelected && House->Is_Player_Control() && Class->VoiceComment.Count() > 0 && Sim_Random_Pick(0, 2) == 0) { // RA2: House->Is_Player_Control(), VocClass::PlayAt
					VocClass::PlayAt(Class->VoiceComment[0], PositionCoord); // RA2: as above
				}
				break;

			/*
			**	On occasion, civilian types will wander about.
			*/
			case 8:
				PrimaryFacing.Set(DirType(DirType(Facing_Dir(Random_Pick(FACING_N, FACING_NW))).As_Dir256())); // RA2: the facing goes through As_Dir256
				if (cow || (!House->Is_Human_Player() && Class->IsFraidyCat)) { // RA2: cows wander too
					Scatter(COORD_NONE, true);
				}
				break;

			case 9:
			case 10:
				PrimaryFacing.Set(DirType(DirType(Facing_Dir(Random_Pick(FACING_N, FACING_NW))).As_Dir256())); // RA2: the facing goes through As_Dir256
				break;

		}
		return(true);
	}
	return(false);
}



/// <summary>
/// RA2
/// </summary>
static inline Point2D Cell_Fraction(Coord const & x, Coord const & y)
{
	return Point2D(x.X & (CELL_LEPTON_W - 1), y.Y & (CELL_LEPTON_H - 1));
}


/// <summary>
/// RA2
/// </summary>
static inline DirType Coord_Direction(Coord const & coord1, Coord const & coord2)
{
	return(fastmath::atan2((double)coord1.Y - (double)coord2.Y, (double)coord2.X - (double)coord1.X));
}


/***********************************************************************************************
 * InfantryClass::Scatter -- Causes the infantry to scatter to nearby cell.                    *
 *                                                                                             *
 *    This routine is used when the infantry should scatter to a nearby cell. Scattering       *
 *    occurs as an occasional consequence of being fired upon. It is one of the features       *
 *    that makes infantry so "charming".                                                       *
 *                                                                                             *
 * INPUT:   threat   -- The coordinate source of the threat that is causing the infantry to    *
 *                      scatter. If the threat isn't from a particular direction, then this    *
 *                      parameter will be NULL.                                                *
 *                                                                                             *
 *          forced   -- The threat is real and a serious effort to scatter should be made.     *
 *                                                                                             *
 *          nokidding-- The scatter should affect the player's infantry even if it otherwise   *
 *                      wouldn't have.                                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   12/12/1994 JLB : Flame thrower infantry always scatter.                                   *
 *   08/02/1996 JLB : Added the nokidding parameter                                            *
 *=============================================================================================*/
void InfantryClass::Scatter(Coord const & threat, bool forced, bool nokidding)
{
	if ((Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE) && forced && nokidding) { // RA2: a deployed soldier undeploys when forced to scatter
		Do_Action(DO_UNDEPLOY, false, false); // RA2: as above
	} else if (House->Is_Human_Player()) { // RA2: as above
		if (Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE) return; // RA2: as above
	}

	/*
	**	A unit that is in the process of going somewhere will never scatter.
	*/
	if (Locomotion->Is_Moving()) forced = false;

	/*
	**	Certain missions prevent scattering regardless of whether it would be
	**	a good idea or not.
	*/
	if (!Current_Mission_Control().IsScatter && !forced) return;

	/*
	**	If the infantry is currently engaged in legitimate combat, then don't
	**	scatter unless forced to.
	*/
	if (!Class->IsFraidyCat && TarCom != NULL && !forced) return;

	/*
	**	Don't scatter if performing an action that can't be interrupted.
	*/
	if (Doing != DO_NOTHING && Doing != DO_UNDEPLOY && !MasterDoControls[Doing].Interrupt) return; // RA2: undeploying may be interrupted

	/*
	**	For human players, don't scatter the infantry, if the special
	**	flag has not been enabled that allows infantry scatter.
	*/
	if (!Rule->IsScatter && !Has_Ability(ABILITY_SCATTER) && !nokidding && House->Is_Human_Player() && !forced && Team == NULL) return;

	if (forced || Class->IsFraidyCat) {
		FacingType toface;

		if (threat != COORD_NONE) {
			toface = Dir_Facing(Coord_Direction(threat, PositionCoord)); // RA2: the executable's scatter (deploy handling, bridge height, queued mission)
			toface = (FacingType)(toface + Random_Pick(FACING_FIRST, FACING_180) - FACING_90);
		} else {
			Point2D coord = Cell_Fraction(Center_Coord(), Center_Coord()); // RA2: as above

			if (coord != Point2D(CELL_LEPTON_W / 2, CELL_LEPTON_H / 2)) {
				toface = ::Direction(Point2D(CELL_LEPTON_W / 2, CELL_LEPTON_H / 2), coord).As_Dir8();
			} else {
				toface = PrimaryFacing.Current().As_Dir8();
			}
			toface = (FacingType)(toface + FacingType(Random_Pick(FACING_FIRST, FACING_180) - FACING_90));
			Cell nearbyloc = Map.Nearby_Location(Destination_Coord().As_Cell(), Class->Speed, -1, MZONE_NORMAL, IsOnBridge, Point2D(1, 1), false, true);

			if (nearbyloc != CELL_NONE) {
				Assign_Destination(&Map[nearbyloc], true); // RA2: the executable's scatter (deploy handling, bridge height, queued mission)
				Locomotion->Process(); // RA2: moves on at once
				return;
			}
		}

		Cell altcell(0, 0);
		Cell newcell(0, 0);
		Cell _cell(Destination_Coord());

		CellClass *cellptr = &Map[_cell];

		/// The parentheses are missing on purpose: the whole sum is the ternary
		/// condition, not just the bridge term.
		int z = cellptr->Height + Is_Moving_Onto_Bridge() ? BRIDGE_CELL_HEIGHT : 0;
		Coord destcoord = Destination_Coord();
		destcoord.Z = z * LEVEL_LEPTON_H;

		FacingType face;
		for (face = FACING_N; face < FACING_COUNT; face++) {
			FacingType newface = Facing_Add(toface, face);
			Cell checkcell = Adjacent_Cell(_cell, newface);
			CellClass *cptr = &Map[checkcell];

			if (Map.In_Local_Radar(checkcell) && Can_Enter_Cell(cptr, newface, Get_Cell_Height()) == MOVE_OK) {
				if (altcell == CELL_NONE) altcell = checkcell;
				if (CELL_NONE == Cell(0,0)) {
					Coord checkcoord = checkcell.As_Coord();
					checkcoord.Z = destcoord.Z;
					if (checkcell == TacticalMap->Coord_To_Cell(checkcoord) && !Map[checkcell].IsUnderBridge) {
						newcell = checkcell;
						break;
					}
				}
			}
		}

		Cell destination = newcell;
		if (destination == CELL_NONE) {
			destination = altcell;
		}

		if (destination != CELL_NONE) {
			QueueMission(MISSION_MOVE, false); // RA2: the mission is queued, not assigned
			Assign_Destination(&Map[destination], true); // RA2: as above
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Do_Action -- Launches the infantry into an animation sequence.               *
 *                                                                                             *
 *    This starts the infantry into a choreographed animation sequence. These sequences can    *
 *    be as simple as standing up or lying down, but can also be complex, such as dying or     *
 *    performing some idle animation.                                                          *
 *                                                                                             *
 * INPUT:   todo     -- The choreographed sequence to start.                                   *
 *                                                                                             *
 *          force    -- Force starting this animation even if the current animation is flagged *
 *                      as uninterruptible. This is necessary for death animations.            *
 *                                                                                             *
 * OUTPUT:  bool; Was the animation started?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Do_Action(DoType todo, bool force, bool randomize)
{
	if (todo == DO_NOTHING || Class->DoControls[todo].Count == 0) {
		return(false);
	}

	if (Doing == DO_PARADROP && IsFalling) { // RA2: nothing interrupts the paradrop fall
		return(false);
	}

	/*
	**	RA2: amphibious infantry swap to the water sequences in water and play the splash sounds
	**	when they enter or leave it.
	*/
	if (Class->MZone == MZONE_AMPHIBIOUS_DESTROYER) { // RA2: the executable's sequence mapping (water, hover, panic, deploy sounds)
		LandType land = Map[Get_Cell()].Land_Type(); // RA2: as above
		int onland = 1; // RA2: as above
		if ((land == LAND_BEACH || land == LAND_WATER) && !IsOnBridge) { // RA2: as above
			onland = 0; // RA2: as above
			if (todo == DO_WALK || todo == DO_CRAWL) { // RA2: as above
				todo = DO_SWIM; // RA2: as above
			} else if (todo == DO_PRONE || todo == DO_STAND_READY) { // RA2: as above
				todo = DO_TREAD; // RA2: as above
			} else if (todo == DO_IDLE1) { // RA2: as above
				todo = DO_WET_IDLE1; // RA2: as above
			} else if (todo == DO_IDLE2) { // RA2: as above
				todo = DO_WET_IDLE2; // RA2: as above
			} else if (todo == DO_GUN_DEATH) { // RA2: as above
				todo = DO_WET_DIE1; // RA2: as above
			} else if (todo == DO_EXPLOSION_DEATH) { // RA2: as above
				todo = DO_WET_DIE2; // RA2: as above
			} else if (todo == DO_FIRE_WEAPON || todo == DO_FIRE_PRONE) { // RA2: as above
				todo = DO_WET_ATTACK; // RA2: as above
			}
		}
		if (LastSurface == 0 && onland == 1) { // RA2: as above
			VocClass::PlayAt(Class->LeaveWaterSound, PositionCoord); // RA2: as above
		} else if (LastSurface == 1 && onland == 0) { // RA2: as above
			VocClass::PlayAt(Class->EnterWaterSound, PositionCoord); // RA2: as above
		}
		LastSurface = onland; // RA2: as above
	}

	if (In_Air() && !IsOnBridge && Class->DoControls[23].Frame > 0 && todo == DO_STAND_READY) { // RA2: a flying soldier hovers
		todo = DO_HOVER; // RA2: as above
	} else if (todo == DO_WALK && Fear >= FEAR_PANIC) { // RA2: panicked infantry run
		todo = DO_PANIC; // RA2: as above
	}

	if (todo != Doing && (Doing == DO_NOTHING || force || MasterDoControls[Doing].Interrupt)) {
		if (todo == DO_DEPLOY) { // RA2: the deploy and undeploy sounds
			if (TClass->DeploySound != VOC_NONE) { // RA2: as above
				VocClass::PlayAt(TClass->DeploySound, PositionCoord); // RA2: as above
			}
		} else if (todo == DO_UNDEPLOY) { // RA2: as above
			if (TClass->UndeploySound != VOC_NONE) { // RA2: as above
				VocClass::PlayAt(TClass->UndeploySound, PositionCoord); // RA2: as above
			}
		}

		Doing = todo;
		if (Is_Idle_Doing(todo)) { // RA2: all the idle sequences
			Set_Rate(Options.Normalize_Delay(MasterDoControls[Doing].Rate));
		} else {
			Set_Rate(MasterDoControls[Doing].Rate);
		}
		if (randomize) {
			Set_Stage(Random_Pick(0,MAX((int)Class->DoControls[todo].Count, 1) - 1)); // RA2: the synced random, not the sim one
		} else {
			Set_Stage(0);
		}

		/*
		**	Kludge to make sure that if infantry is in the dying animation, it isn't still
		**	moving as well.
		*/
		if (Strength == 0) {
			Stop_Driver();
		}

		/*
		**	Since the animation sequence might be interrupted. Set any flags
		**	necessary so that if interrupted, the affect on the infantry is
		**	still accomplished.
		*/
		switch (todo) {
			case DO_LIE_DOWN:
				IsProne = true;
				break;

			case DO_GET_UP:
			case DO_DEPLOY: // RA2: deploying stands up as well
				IsProne = false;
				break;

			default:
				break;
		}

		return(true);
	}

	return(false);
}


/***********************************************************************************************
 * InfantryClass::Stop_Driver -- Stops the infantry from moving any further.                   *
 *                                                                                             *
 *    This is used to stop the infantry from animating in movement. This function will stop    *
 *    the infantry moving and revert it to either a prone or standing.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the driving stopped?                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Stop_Driver(void)
{
	if (Doing == DO_DEPLOY) goto deploy_fire; // RA2: Stop_Driver at 0x504AE0 matches only so
	if (Doing == DO_DEPLOYED) goto deploy_fire; // RA2: as above
	if (Doing == DO_DEPLOYED_FIRE) goto deploy_fire; // RA2: as above
	if (Doing == DO_DEPLOYED_IDLE) { // RA2: as above
deploy_fire: // RA2: as above
	Do_Action(DO_DEPLOYED); // RA2: as above
	} else if (IsProne) { // RA2: as above
		Do_Action(DO_PRONE);
	} else {
		Do_Action(DO_STAND_READY);
	}

	if (Can_Enter_Cell(&Map[(Coord const &)PositionCoord], PrimaryFacing.Current().As_Dir8(), Get_Cell_Height()) == MOVE_OK) {
		IsZoneCheat = false;
	} else {
		IsZoneCheat = true;
	}

	return(BASECLASS::Stop_Driver());
}


/***********************************************************************************************
 * InfantryClass::Start_Driver -- Handles giving immediate destination and move orders.        *
 *                                                                                             *
 *    Use this routine to being the infantry moving toward the destination specified. The      *
 *    destination is first checked to see if there is a free spot available. Then the infantry *
 *    reserves that spot and begins movement toward it.                                        *
 *                                                                                             *
 * INPUT:   headto   -- The coordinate location desired for the infantry to head to.           *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry successfully started on its journey? Failure may be because *
 *                the specified destination could not contain the infantry unit.               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/21/1994 JLB : Created.                                                                 *
 *   05/14/1995 JLB : Tries to move to closest spot possible.                                  *
 *   05/15/1995 JLB : Uses closest spot if moving onto transport.                              *
 *=============================================================================================*/
bool InfantryClass::Start_Driver(Coord & headto)
{
	Coord old = headto;

	/*
	**	Convert the head to coordinate to a legal sub-position location.
	*/
	headto = Map[headto].Closest_Free_Spot(Move_Coord(headto, Direction(headto) + ((DIR_S << 8) - 1), CELL_LEPTON / 2 - 4), false, Map[headto].IsUnderBridge); // RA2: Red Alert's Direction(headto), ObjectClass::Direction
	if (headto == COORD_NONE && Can_Enter_Cell(&Map[old]) == MOVE_OK) {
		headto = Map[old].Closest_Free_Spot(Move_Coord(old, Direction(headto) + ((DIR_S << 8) - 1), CELL_LEPTON / 2), true, Map[old].IsUnderBridge);
	}

	/*
	**	If the infantry started moving, then fixup the occupation bits.
	*/
	if (headto != COORD_NONE && BASECLASS::Start_Driver(headto)) {
		if (!IsActive) return(false);

		/*
		**	Remove the occupation bit from the infantry's current location.
		*/
		Clear_Occupy_Bit(Coord(Position)); // RA2: a copy of the position, as ra2-decomp's twin has it

		/*
		**	Set the occupation bit for the new headto location.
		*/
		Set_Occupy_Bit(headto);
		return(true);
	}

	return(false);
}


/***********************************************************************************************
 * InfantryClass::Limbo -- Performs cleanup operations needed when limboing.                   *
 *                                                                                             *
 *    This routine will clean up the infantry occupation bits (as necessary) as well as stop   *
 *    the infantry movement process when it gets limboed.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry unit limboed?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Limbo(void)
{
	Locomotion->Stop_Movement_Animation();
	LastSurface = 2; // RA2: the surface is not known again
	IsProne = false; // RA2: standing up again
	Doing = DO_STAND_READY; // RA2: as above
	return(BASECLASS::Limbo());
}


/***********************************************************************************************
 * InfantryClass::Fire_At -- Fires projectile from infantry unit.                              *
 *                                                                                             *
 *    Use this routine when the infantry unit wishes to fire a projectile. This routine        *
 *    will launch the projectile and perform any other necessary infantry specific operations. *
 *                                                                                             *
 * INPUT:   target   -- The target of the attack.                                              *
 *                                                                                             *
 *          which    -- Which weapon to use for firing. 0=primary, 1=secondary.                *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to the projectile launched. If none could be launched, then   *
 *          NULL is returned. If there is already the maximum bullet objects in play, then     *
 *          this could happen.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass * InfantryClass::Fire_At(AbstractClass * target, int which)
{
	IsFiring = false;

	BulletClass * bullet = BASECLASS::Fire_At(target, which);
	if (bullet != NULL && !IsInLimbo) {
		if (Class->IsFraidyCat && !Ammo) {
			Fear = (FearType)0x12c; // RA2: Fire_At at 0x504F50 matches only so
			if (Get_Mission() == MISSION_ATTACK || Get_Mission() == MISSION_HUNT) { // RA2: as above
				QueueMission(MISSION_GUARD, false); // RA2: as above
			}
		}
	}
	return(bullet);
}


/***********************************************************************************************
 * InfantryClass::Unlimbo -- Unlimbo infantry unit in legal sub-location.                      *
 *                                                                                             *
 *    This will attempt to unlimbo the infantry unit at the designated coordinate, but will    *
 *    ensure that the coordinate is a legal subposition.                                       *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to unlimbo the infantry at.                             *
 *                                                                                             *
 *          facing   -- The desired initial facing for the infantry unit.                      *
 *                                                                                             *
 *          strength -- The desired initial strength for the infantry unit.                    *
 *                                                                                             *
 *          mission  -- The desired initial mission for the infantry unit.                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry unlimboed successfully?                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Unlimbo(Coord const & xcoord, Dir256 facing)
{
	int height = Map.Get_Height_GL(xcoord);
	Coord coord = xcoord;

	if (coord.Z == height) {

		/*
		**	Make sure that the infantry start in a legal position on the map.
		*/
		coord = Map[xcoord].Closest_Free_Spot(xcoord, ScenarioInit || !Map.In_Local_Radar(xcoord.As_Cell()));
		if (coord == COORD_NONE) {
			return(false);
		}
		coord.Z = height;
	}

	if (BASECLASS::Unlimbo(coord, facing)) {

		/*
		**	If there is no sight range, then this object isn't discovered by the player unless
		**	it actually appears in a cell mapped by the player.
		*/
		if (Class->SightRange == 0) {
			IsDiscoveredByPlayer = false;
		}

		if (coord.Z <= height + BRIDGE_LEPTON_HEIGHT) {
			Set_Occupy_Bit(coord);
		}

		LastSurface = 2;
		return(true);
	}
	return(false);
}



/***********************************************************************************************
 * InfantryClass::Greatest_Threat -- Determines greatest threat (target) for infantry unit.    *
 *                                                                                             *
 *    This routine intercepts the Greatest_Threat request and adds the appropriate target      *
 *    types to search for. For regular infantry, this consists of all the ground types. For    *
 *    rocket launching infantry, this also includes aircraft.                                  *
 *                                                                                             *
 * INPUT:   threat   -- The basic threat control value.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the best target for this infantry unit to attack. If no suitable      *
 *          target could be found, then TARGET_NONE is returned.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *   09/28/1995 JLB : Engineers try to recapture buildings first.                              *
 *=============================================================================================*/
AbstractClass * InfantryClass::Greatest_Threat(ThreatType threat, Coord const & coord, bool onlyenemy) const
{
	/*
	**	Engineers consider only buildings that can be captured as being a threat. All others
	**	are ignored. If there is a building that needs to be recaptured and it is nearby
	**	then automatically head toward it to recapture it.
	*/
	if (!House->Is_Human_Player() && Class->IsEngineer) { // RA2: IsEngineer instead of IsCapture, and no weapon test
		ObjectClass * object = (ObjectClass *)House->ToCapture; // RA2: as above
		if (object != NULL) { // RA2: as above
			if (Distance(object->Center_Coord()) < 15 * CELL_LEPTON) { // RA2: the distance is measured to the center of the object
				return(House->ToCapture);
			}
		}
		threat = ThreatType(threat | THREAT_CAPTURE);
	}

	if (Class->IsVehicleThief) {
		AbstractClass * victim = NavCom; // RA2: the flag test goes through a local that is only set for a non-NULL target
		bool active; // RA2: as above
		if (victim != NULL) { // RA2: as above
			active = (victim->AbstractFlags & ABSTRACT_TECHNO); // RA2: as above
		}
		if (victim != NULL && active && ((TechnoClass *)NavCom)->Considered_Vehicle() && !((TechnoClass *)NavCom)->TClass->IsTrain && Distance(NavCom) < 15 * CELL_LEPTON) { // RA2: the flags test instead of Is_Techno
			return(NavCom);
		}
	}

	if (!Is_Weapon_Equipped()) {
		if (!Class->IsCapture && !Class->IsVehicleThief) {
			return(NULL);
		}
		if (Class->IsVehicleThief) {
			threat = ThreatType((threat & ~(THREAT_BUILDINGS|THREAT_VEHICLES|THREAT_BOATS|THREAT_AIR)) | THREAT_VEHICLES);
		}
	}

	if ((threat & (THREAT_INFANTRY|THREAT_VEHICLES|THREAT_BUILDINGS|THREAT_TIBERIUM|THREAT_CIVILIANS|THREAT_POWER|THREAT_FACTORIES|THREAT_BASE_DEFENSE|THREAT_OCCUPIABLE|THREAT_TECH_BUILDINGS)) == 0) { // RA2: the mask also holds the occupiable and tech building threats
		if (Get_Class_Weapon_Data(0)->Weapon != NULL) { // RA2: through the virtual weapon lookup
			threat = ThreatType(threat | Get_Class_Weapon_Data(0)->Weapon->Allowed_Threats()); // RA2: as above
		}
		if (Get_Class_Weapon_Data(1)->Weapon != NULL) { // RA2: as above
			threat = ThreatType(threat | Get_Class_Weapon_Data(1)->Weapon->Allowed_Threats()); // RA2: as above
		}
	}

	/*
	**	Organic weapon types don't consider anything but infantry to be a threat. Such
	**	weapon types would be the dog jaw and the medic first aid kit.
	*/
	if (Is_Weapon_Equipped() && Get_Class_Weapon_Data(0)->Weapon->WarheadPtr->IsOrganic) { // RA2: the executable's threat selection for engineers, vehicle thieves and thieves
		threat = ThreatType(threat & ~(THREAT_BUILDINGS|THREAT_VEHICLES|THREAT_BOATS|THREAT_AIR));
	}

	/*
	**	If this is a bomber type, then allow buildings to be considered a threat.
	*/
	if ((Class->IsBomber || Has_Ability(ABILITY_C4)) && !House->Is_Human_Player()) {
		threat = ThreatType(threat | THREAT_BUILDINGS);
	}

	/*
	**	Special hack: if it's a thief, then the only possible objects to
	**	consider are tiberium-processing objects (silos & refineries).
	*/
	if (Class->IsThief) {
		threat = ThreatType(threat | THREAT_CAPTURE | THREAT_TIBERIUM);
	}
	return(BASECLASS::Greatest_Threat(threat, coord, onlyenemy));
}


/***********************************************************************************************
 * InfantryClass::What_Action -- Infantry units might be able to capture -- check.             *
 *                                                                                             *
 *    This routine checks to see if the infantry unit can capture the specified object rather  *
 *    than merely attacking it. If this is the case, then ACTION_CAPTURE will be returned.     *
 *                                                                                             *
 * INPUT:   object   -- The object that the mouse is currently over.                           *
 *                                                                                             *
 * OUTPUT:  Returns the action that will be performed if the mouse were clicked over the       *
 *          object specified.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType InfantryClass::What_Action(ObjectClass const * object, bool disallow_force) const
{
	bool shift = !disallow_force && (Keyboard->Down(Options.KeySelect1) || Keyboard->Down(Options.KeySelect2)); // RA2: the select (shift) keys, then the force-attack keys (opcheck: Options+156, +160, +148, +152)
	bool force_fire = !disallow_force && (Keyboard->Down(Options.KeyForceAttack1) || Keyboard->Down(Options.KeyForceAttack2)); // RA2: as above
	if (shift && force_fire) { // RA2: as above
		force_fire = false; // RA2: as above
	}

	ActionType action = BASECLASS::What_Action(object, disallow_force);

	if (action == ACTION_TOGGLE_SELECT) { // RA2: a selection toggle stays
		return(action);
	}

	if (Class->IsEngineer && House->Is_Player_Control() && object->AttachedBomb != NULL && object->BombVisible) { // RA2: engineers defuse visible bombs
		return(ACTION_DISARM_BOMB); // RA2: as above
	}

	/*
	**	If this is an engineer/renovator, we have to make some adjustments.
	*/
	if (Class->IsEngineer && object->RTTI == RTTI_BUILDING && House->Is_Player_Control() && !object->Considered_Vehicle() && ((BuildingClass const *)object)->Class->IsRepairable) { // RA2: as above
		BuildingClass const * bldg = (BuildingClass const *)object; // RA2: as above
		if (bldg->Class->IsBridgeRepairHut) {
			return(Map.Can_Repair_Bridge(object->Center_Coord()) ? ACTION_GREPAIR : ACTION_NO_GREPAIR); // RA2: as above
		}
		if (House->Is_Ally(object) || (bldg->House->Class->IsMultiplayPassive && bldg->Class->CanBeOccupied)) { // RA2: as above
			if (bldg->Class->IsHospital && HealthRatio < Rule->ConditionGreen) { // RA2: as above
				return(ACTION_ENTER); // RA2: as above
			}
			if (object->HealthRatio == Rule->ConditionGreen) { // RA2: as above
				return(ACTION_NO_GREPAIR);
			}
			return(ACTION_GREPAIR);
		}
		if (bldg->Class->IsCaptureable) {
			if (object->HealthRatio <= Rule->EngineerCaptureLevel) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
				goto capture; // RA2: shares the return of the garrison test below; the exe jumps there
			}
			return(ACTION_DAMAGE);
		}
	}

	if (object->RTTI == RTTI_BUILDING && House->Is_Player_Control() && ((BuildingClass const *)object)->CanBeOccupiedBy((InfantryClass *)this) && !force_fire) { // RA2: garrisoning
capture: // RA2: structured forms do not match; the exe shares this return between the engineer capture-level test and the garrison test, and the goto form matches
		return(ACTION_CAPTURE);
	}

	if (Class->IsEngineer && action == ACTION_ATTACK) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
		return(ACTION_NOMOVE); // RA2: as above
	}

	/*
	**	If this is a medic, and the cursor's over a friendly infantryman,
	**	execute an action-attack.
	*/
	if (Combat_Damage() < 0 && House->Is_Player_Control()) {
		if (House->Is_Ally(object)) {
			if (object->RTTI == RTTI_INFANTRY) {
				if (object != this) {
					if (object->HealthRatio < Rule->ConditionGreen) {
						return(ACTION_HEAL);
					}
				} else {
					return(ACTION_GUARD_AREA);
				}
			}
			const TechnoClass *tech = ::Dynamic_Cast<TechnoClass const *>(object);
			if (tech == NULL || !tech->TClass->Max_Passengers()) {
				if (action == ACTION_GUARD_AREA || action == ACTION_MOVE) {
					return(action);
				}
				return(ACTION_SELECT);
			}
		} else {
			return(ACTION_ATTACK_SUPPORT);
		}
	}

	/*
	**	See if it's a thief attacking an enemy vehicle, let him CAPTURE it.
	*/
	if (House->Is_Player_Control() && Class->IsVehicleThief && object->Considered_Vehicle()) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
		if (object->TClass->IsTrain) {
			return(ACTION_SELECT);
		}
		if (((UnitClass *)object)->House != House) {
			if (Scen->Special.IsHarvesterImmune && Rule->HarvesterUnit.Is_In_List((UnitTypeClass const *)object)) {
				return(ACTION_SELECT);
			}
			return(ACTION_CAPTURE);
		}
	}

	if (object->RTTI == RTTI_BUILDING && House->Is_Ally(object) && House->Is_Player_Control() && ((BuildingClass const *)object)->Class->IsHospital && !force_fire) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
		if (Strength < Class->MaxStrength) {
			if (!Class->Deployer || (Doing != DO_DEPLOY && Doing != DO_DEPLOYED && Doing != DO_DEPLOYED_FIRE && Doing != DO_DEPLOYED_IDLE)) { // RA2: as above
				switch (((InfantryClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (RadioClass *)object)) {
					case RADIO_ROGER:
						action = ACTION_ENTER;
						break;

					case RADIO_NEGATIVE:
						action = ACTION_NO_ENTER;
						break;
				}
			}
		} else {
			action = ACTION_NO_ENTER;
		}
	}

	if (object->RTTI == RTTI_BUILDING && House->Is_Ally(object) && House->Is_Player_Control() && ((BuildingClass const *)object)->Class->IsArmory) { // RA2: armories train veterans
		if (!Veterancy.Is_Elite()) {
			if (!Class->Deployer || (Doing != DO_DEPLOY && Doing != DO_DEPLOYED && Doing != DO_DEPLOYED_FIRE && Doing != DO_DEPLOYED_IDLE)) { // RA2: as above
				switch (((InfantryClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (RadioClass *)object)) {
					case RADIO_ROGER:
						action = ACTION_ENTER;
						break;

					case RADIO_NEGATIVE:
						action = ACTION_NO_ENTER;
						break;
				}
			}
		} else {
			action = ACTION_NO_ENTER;
		}
	}

	if (House->Is_Player_Control() && (Class->IsBomber || Has_Ability(ABILITY_C4)) && action == ACTION_ATTACK && object->RTTI == RTTI_BUILDING && !object->Considered_Vehicle()) {
		if (((BuildingClass const *)object)->Class->CanC4 && !((BuildingClass const *)object)->Class->IsInvisibleInGame) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
			return(ACTION_SABOTAGE);
		}
		return(ACTION_ATTACK);
	}

	WeaponTypeClass const * weapon = Get_Class_Weapon_Data(What_Weapon_Should_I_Use((AbstractClass *)object))->Weapon; // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
	if (action == ACTION_ATTACK && weapon != NULL && weapon->SabotageCursor) { // RA2: a sabotage weapon has its own cursor
		return((ActionType)ACTION_DEMOLISH); // RA2: as above
	}

	if (House->Is_Player_Control() && action == ACTION_ATTACK && Class->Ivan) { // RA2: Ivan's bombs
		if (object->Class_Of()->IsBombable && object->AttachedBomb == NULL) { // RA2: as above
			return(ACTION_IVAN_BOMB); // RA2: as above
		}
		return(ACTION_NO_IVAN_BOMB); // RA2: as above
	}

	if (House->Is_Player_Control() && Class->Deployer && (Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE)) { // RA2: as above
		if (action == ACTION_MOVE) { // RA2: as above
			return(ACTION_NOMOVE); // RA2: as above
		}
		if (action == ACTION_ATTACK) {
			bool in_range = false; // RA2: as above
			if (Class->DeployFire) { // RA2: as above
				int widx = What_Weapon_Should_I_Use(NULL); // RA2: as above
				in_range = In_Range((AbstractClass *)object, widx); // RA2: as above
			}
			if (!in_range) { // RA2: as above
				return(ACTION_NOMOVE); // RA2: as above
			}
		} else if (action == ACTION_SELF) { // RA2: as above
			if (!Planning_Allows_Event_Type(EventClass::MEGAMISSION)) { // RA2: as above
				return(ACTION_NO_DEPLOY); // RA2: as above
			}
			if (Class->UndeployDelay > -1) { // RA2: as above
				return(ACTION_NO_DEPLOY); // RA2: as above
			}
		}
	}

	if (House->Is_Player_Control() && action == ACTION_SELF && Class->Deployer && Class->UndeployDelay > -1 && Doing == DO_UNDEPLOY) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
		return(ACTION_NO_DEPLOY); // RA2: as above
	}

	if (action == ACTION_SELF) {
		if (Planning_Allows_Event_Type(EventClass::MEGAMISSION)) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
			if (!House->Is_Player_Control()) {
				action = ACTION_NONE;
			} else if (GetDeployWeapon()->Weapon != NULL && GetDeployWeapon()->Weapon->AreaFire) { // RA2: as above
				action = ACTION_AREA_ATTACK; // RA2: as above
			} else if (!Class->Deployer) { // RA2: as above
				action = ACTION_NONE;
			}
		} else {
			action = ACTION_NO_DEPLOY; // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
		}
	}

	if (action != ACTION_NO_ENTER && House->Is_Ally(object) && House->Is_Player_Control() && ::Dynamic_Cast<TechnoClass const *>(object) != NULL && action != ACTION_ATTACK && action != ACTION_GUARD_AREA) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
		TechnoTypeClass const * tclass = object->Techno_Type_Class(); // RA2: as above
		if (tclass != NULL && tclass->MaxPassengers > 0) { // RA2: as above
			if (Class->BalloonHover) { // RA2: as above
				action = ACTION_NO_ENTER;
			} else {
				FootClass const * foot = (FootClass const *)object; // RA2: as above
				bool transport_ready; // RA2: the foot flag is read through a local that is only set for a non-NULL object
				if (object != NULL) { // RA2: as above
					transport_ready = (object->AbstractFlags & 4) != 0; // RA2: as above
				}
				if (object != NULL && transport_ready && ((foot->Team == NULL || foot->Team->Class->IsLoadable) ? foot->Locomotion->Is_Moving() : true)) { // RA2: as above
					action = ACTION_NO_ENTER;
				} else {
					switch (((InfantryClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (RadioClass *)object)) {
						case RADIO_ROGER:
							action = ACTION_ENTER;
							break;

						case RADIO_NEGATIVE:
							action = ACTION_NO_ENTER;
							break;
					}
				}
			}
		}
	}

	if (House->Is_Player_Control() && Class->IsCapture && action == ACTION_ATTACK) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
		if ((!Class->IsAgent && !House->Is_Ally(object) && object->RTTI == RTTI_BUILDING && ((BuildingClass const *)object)->Class->IsCaptureable) || // RA2: as above
			(Class->IsAgent && !House->Is_Ally(object) && object->RTTI == RTTI_BUILDING && ((BuildingClass const *)object)->Class->Spyable)) { // RA2: as above
			action = ACTION_CAPTURE;
			if (object->RTTI == RTTI_BUILDING) {
				Cell cell = object->Center_Coord().As_Cell();
				int targzone = Map.Get_Cell_Zone(Get_Target_Cell(), Class->MZone, IsOnBridge);
				Cell const * list = ((BuildingClass const *)object)->Class->Occupy_List(false); // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
				bool found = false;
				while (*list != REFRESH_EOL && !found) {
					Cell newcell = *list++ + cell; // RA2: as above
					for (int i = 0; i < 8; i++) { // RA2: as above
						Cell adjcell = AdjacentCell[i & 7] + newcell; // RA2: as above
						if (Map.Get_Cell_Zone(adjcell, Class->MZone, false) == targzone) {
							found = true;
							break;
						}
					}
				}
			}
			goto finish; // RA2: the exe skips the tests below here; the gotos at "finish" match, nested ifs do not
		}
		if (!Is_Weapon_Equipped()) {
			action = ACTION_NONE;
			goto finish; // RA2: as above
		}
	}

	if (action == ACTION_ATTACK) {
		if (!Is_Weapon_Equipped()) {
			action = ACTION_ATTACK_SUPPORT;
		}
	} else if (action == ACTION_SELECT && object->RTTI == RTTI_BUILDING && House->Is_Player_Control() && !object->Considered_Vehicle() && ((BuildingClass const *)object)->Class->Cloning) { // RA2: cloning vats
		return(object->Owner_HouseClass() != PlayerPtr ? ACTION_NO_ENTER : ACTION_EATEN); // RA2: as above
	}

finish: // RA2: structured forms do not match; the exe skips the tests above for the capture cases, and the goto form matches
	if (House->Is_Player_Control() && action == ACTION_NONE) { // RA2: as above
		if (IsBeingWarpedOut() || IsWarpingIn()) { // RA2: as above
			goto done; // RA2: the exe returns through the shared tail with the action in its register; a plain return of the constant does not match
		}
		action = ACTION_NOMOVE;
	}

	if (action == ACTION_NOMOVE) { // RA2: the executable's object action (engineer, medic, thief, deploy, capture, cloning)
		if (object->IsDisguised() && !object->GetDisguiseHouse(true) && object->GetDisguise(true)->RTTI == RTTI_OVERLAYTYPE) { // RA2: a disguised soldier standing for an overlay is moved over
			action = ACTION_MOVE; // RA2: as above
		}
	} else if (action == ACTION_ATTACK && object->Class_Of()->IsImmune) { // RA2: as above
		action = ACTION_NOMOVE;
	}

done: // RA2: structured forms do not match; the exe returns the action register through this shared tail, and the goto form matches
	return(action);
}



/***********************************************************************************************
 * InfantryClass::Active_Click_With -- Handles action when clicking with infantry soldier.     *
 *                                                                                             *
 *    This routine is called when the player clicks over an object while this infantry soldier *
 *    is selected. Capture attempts are prohibited if the infantry cannot capture. The         *
 *    command might respond if told to sabotage something.                                     *
 *                                                                                             *
 * INPUT:   action   -- The action that is nominally to be performed.                          *
 *                                                                                             *
 *          object   -- The object over which the mouse was clicked.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Active_Click_With(ActionType action, ObjectClass * object, bool is_waypoint)
{
	action = What_Action(object, is_waypoint);

	switch (action) {
		case ACTION_DAMAGE: // RA2: DAMAGE and GREPAIR (and CAPTURE) all become CAPTURE
		case ACTION_GREPAIR:
		case ACTION_CAPTURE:
			action = ACTION_CAPTURE;
			break;

		case ACTION_HEAL: // RA2: HEAL and DISARM_BOMB become ATTACK
		case ACTION_DISARM_BOMB: // RA2: the executable cancels a click on itself
			action = ACTION_ATTACK;
			break;

		case ACTION_SABOTAGE:
		case ACTION_ATTACK:
		case ACTION_GUARD_AREA:
		case ACTION_MOVE:
			action = action;
			break;

		default:
			break;
	}

	if (object == this && action == ACTION_ATTACK) { // RA2: an infantryman cannot attack itself
		return(false);
	}
	return(BASECLASS::Active_Click_With(action, object, is_waypoint));
}


/// <summary>
/// Handles a player click on a map cell while this infantry is selected.
/// This routine defers to the FootClass handler; infantry have no cell click
/// behavior of their own beyond the normal movement and attack orders.
/// </summary>
/// <returns>bool; Was the click acted upon?</returns>
bool InfantryClass::Active_Click_With(ActionType action, Cell const & cell, Cell const & cell2, bool is_waypoint) // RA2: a second cell
{
	if (Is_Deploying() && What_Action(cell, false, is_waypoint) == ACTION_NOMOVE) { // RA2: a deploying soldier ignores a click that cannot be moved to
		return(false);
	}
	return(BASECLASS::Active_Click_With(action, cell, cell2, is_waypoint)); // RA2: as above
}


/***********************************************************************************************
 * InfantryClass::Full_Name -- Fetches the full name of the infantry unit.                     *
 *                                                                                             *
 *    This routine will return with the full name (as a text number) for this infantry         *
 *    unit. Typically, this is the normal name, but in cases of civilian type survivors from   *
 *    a building explosion, it might be a technician instead. In such a case, the special      *
 *    technician name number is returned instead.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the full name to use for this infantry unit.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/30/1995 JLB : Created.                                                                 *
 *   10/28/1996 JLB : Spy returns "enemy soldier" text name.                                   *
 *=============================================================================================*/
wchar_t const * InfantryClass::Full_Name(void) const // RA2: wide
{
	if (IsTechnician) {
		return(StringTable::LoadString("TXT_TECHNICIAN", NULL, "D:\\Ra2\\Infantry.CPP", 4221)); // RA2
	}

	if (IsDisguised() && !House->Is_Ally(PlayerPtr)) { // RA2
		return(GetDisguise(true)->UIName); // RA2
	}

	return(Class->UIName); // RA2
}



/// <summary>
/// RA2: not in TibSun, at 0x5061D0. Keeps an idle soldier looking for something to shoot.
/// While the current target is still in range nothing changes; otherwise the soldier takes
/// the greatest threat around it (none while it is mind controlled) as its new target, and
/// one left without a target drops back to idle unless it is guarding or a jumpjet.
/// </summary>
void InfantryClass::Reacquire_Target(void)
{
	int which = What_Weapon_Should_I_Use(NULL);
	if (TarCom != NULL && In_Range(TarCom, which)) {
		return;
	}

	AbstractClass * target = NULL;
	if (MindControlledBy == NULL) {
		Coord coord = Position;
		target = Greatest_Threat(THREAT_RANGE, coord, false);
	}
	Assign_Target(target);

	if (target == NULL && CurrentMission != MISSION_GUARD && !TClass->JumpJet) {
		Enter_Idle_Mode(false, true);
	}
}




/***********************************************************************************************
 * InfantryClass::Mission_Attack -- Intercept attack mission for special handling.             *
 *                                                                                             *
 *    This routine intercepts the normal attack mission and if an engineer is detected and the *
 *    target is a building, then the engineer will be automatically assigned the capture       *
 *    mission. In other cases, the normal attack logic will proceed.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/07/1995 JLB : Created.                                                                 *
 *   04/15/1996 BWG : Engineers can only attack their own house's buildings now.               *
 *   05/29/1996 JLB : Engineers can now damage/capture enemy buildings.                        *
 *=============================================================================================*/
int InfantryClass::Do_MISSION_ATTACK(void)
{
	if ((Class->IsBomber || Has_Ability(ABILITY_C4)) && TarCom != NULL && TarCom->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)TarCom)->Class->CanC4 && !((BuildingClass *)TarCom)->Class->IsInvisibleInGame) { // RA2: Do_MISSION_ATTACK at 0x506280 matches only so
		Assign_Destination(TarCom, true); // RA2: as above
		QueueMission(MISSION_SABOTAGE, false); // RA2: as above
		return(1);
	}

	if (!House->Is_Human_Player() && TarCom != NULL && TarCom->Fetch_RTTI() == RTTI_BUILDING && (Class->IsCapture || ((Class->Occupier || Class->Assaulter) && ((BuildingClass *)TarCom)->CanBeOccupiedBy(this)))) { // RA2: as above
		Assign_Destination(TarCom, true); // RA2: as above
		Set_Mission(MISSION_CAPTURE); // RA2: as above
		return(1);
	}

	if (House->Is_Human_Player()) { // RA2: as above
		if (Doing == DO_DEPLOY) goto deploy_fire; // RA2: as above
		if (Doing == DO_DEPLOYED) goto deploy_fire; // RA2: as above
		if (Doing == DO_DEPLOYED_FIRE) goto deploy_fire; // RA2: as above
		if (Doing == DO_DEPLOYED_IDLE) { // RA2: as above
deploy_fire: // RA2: as above
			Reacquire_Target(); // RA2: as above
			return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2)); // RA2: as above
		}
	}

	return(BASECLASS::Do_MISSION_ATTACK());
}




/// <summary>
/// Handles the guard mission for this infantry.
/// Attack dogs left standing about on tiberium will turn to face east and then lie
/// down, which is how they come to be found sleeping in the fields. Everything else
/// about guard duty is left to the normal FootClass handler.
/// </summary>
/// <returns>The delay in game frames before this mission should be processed again.</returns>
/// <summary>
/// RA2: sends a computer-owned capturer or occupier into the building it is hunting.
/// </summary>
int InfantryClass::Do_MISSION_HUNT(void)
{
	if (!House->Is_Human_Player() && TarCom != NULL && TarCom->Fetch_RTTI() == RTTI_BUILDING) {
		if (Class->IsCapture || ((Class->Occupier || Class->Assaulter) && ((BuildingClass *)TarCom)->CanBeOccupiedBy(this))) {
			Assign_Destination(TarCom, true);
			Set_Mission(MISSION_CAPTURE);
			return(1);
		}
	}
	return(BASECLASS::Do_MISSION_HUNT());
}


/// <summary>
/// RA2: a deployer may deploy or fire from its deployed state before guarding.
/// </summary>
int InfantryClass::Do_MISSION_GUARD(void)
{
	int delay = Update_Deployed_Attack(); // RA2: Do_MISSION_GUARD at 0x506470 matches only so (its bytes equal Do_MISSION_GUARD_AREA's)
	if (delay == -1) { // RA2: as above
		return(BASECLASS::Do_MISSION_GUARD()); // RA2: as above
	}
	return(delay); // RA2: as above
}


/// <summary>
/// RA2: as Do_MISSION_GUARD, for area guard.
/// </summary>
int InfantryClass::Do_MISSION_GUARD_AREA(void)
{
	int delay = Update_Deployed_Attack();
	if (delay == -1) {
		return(BASECLASS::Do_MISSION_GUARD_AREA());
	}
	return(delay);
}


/// <summary>
/// RA2: a deployed soldier undeploys before it moves; a human player's just stops.
/// </summary>
int InfantryClass::Do_MISSION_MOVE(void)
{
	if (Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE) {
		if (Owner_HouseClass()->Is_Human_Player()) {
			Assign_Destination(NULL, true);
			return(1);
		}
		if (Class->UndeployDelay <= -1) {
			Do_Action(DO_UNDEPLOY, false, false);
			return(Class->DoControls[31].Count);
		}
	}
	return(BASECLASS::Do_MISSION_MOVE());
}


/// <summary>
/// RA2: the deploy command for a Deployer: deploys (aiming an area-fire deploy weapon at its own cell) or undeploys.
/// </summary>
int InfantryClass::Do_MISSION_UNLOAD(void)
{
	if (Class->Deployer) {
		int delay = -1;
		if (Is_Deploying()) {
			if (Class->UndeployDelay <= -1) {
				Do_Action(DO_UNDEPLOY, true, false);
			}
		} else {
			Do_Action(DO_DEPLOY, true, false);
			WeaponDataStruct const *weapon = GetDeployWeapon();
			if (Is_Weapon_Valid(weapon) && weapon->Weapon->AreaFire) {
				if (Is_Type_Named("DESO")) {
					delay = Class->DoControls[27].Count + 1;
				} else {
					Assign_Target((AbstractClass *)&Map[Get_Cell()]);
				}
			}
			if (Class->UndeployDelay > -1) {
				delay = Class->UndeployDelay;
			}
		}
		Set_Mission(MISSION_GUARD);
		Assign_Destination(0, true);
		if (delay > -1) {
			return delay;
		}
	}
	return BASECLASS::Do_MISSION_UNLOAD();
}












/***********************************************************************************************
 * InfantryClass::What_Action -- Determines what action to perform for the cell specified.     *
 *                                                                                             *
 *    This routine will determine what action to perform if the mouse was clicked on the cell  *
 *    specified. This is just a courier function since the lower level classes actually        *
 *    perform the work. The need for this routine at this level is due to the existence of     *
 *    a similarly named function at this level as well.  C++ namespace rules require this      *
 *    function courier to be in place or an error will result.                                 *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the mouse might be clicked upon.                            *
 *                                                                                             *
 * OUTPUT:  Returns with the action that would be given to this infantry unit if the mouse     *
 *          were clicked at the cell specified.                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType InfantryClass::What_Action(Cell const & cell, bool check_fog, bool disallow_force) const
{
	if (!House->Is_Player_Control()) {
		return(ACTION_NONE);
	}

	ActionType action = BASECLASS::What_Action(cell, check_fog, disallow_force);

	/*
	**	If this is a medic, and the cursor's over a friendly infantryman,
	**	execute an action-attack.
	*/
	if (Combat_Damage() < 0 && House->Is_Player_Control() && action == ACTION_ATTACK) { // RA2: the executable's cell action (tunnels, deployed soldiers, fogged buildings)
		action = ACTION_GUARD_AREA;
	}

	if (Class->JumpJet && (action == ACTION_MOVE || action == ACTION_NOMOVE)) { // RA2: jumpjets cannot enter the tunnel mouths
		if (Map[cell].Is_Near_Tunnel_NW()) { // RA2: as above
			action = ACTION_NONE;
		} else if (Map[cell].Is_Near_Tunnel_ES()) { // RA2: as above
			action = ACTION_NONE;
		}
	}

	if (House->Is_Player_Control() && Class->Deployer && (Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE) && (action == ACTION_MOVE || action == ACTION_ATTACK)) { // RA2: a deployed soldier cannot move, and attacks only what it can reach
		if (action == ACTION_MOVE) { // RA2: as above
			action = ACTION_NOMOVE;
		} else {
			bool inrange = false; // RA2: as above
			if (Class->DeployFire) { // RA2: as above
				int weapon = What_Weapon_Should_I_Use(NULL); // RA2: as above
				Coord coord = cell; // RA2: as above
				inrange = In_Range(coord, weapon); // RA2: as above
			}
			if (!inrange) { // RA2: as above
				action = ACTION_NOMOVE;
			}
		}
	} else if (action == ACTION_MOVE && Map[cell].Has_Tunnel()) { // RA2: tunnel entrances
		return(Map[cell].Can_Enter_Tunnel(this) ? ACTION_ENTER_TUNNEL : ACTION_NO_ENTER_TUNNEL);
	}

	CellClass * cellptr = &Map[cell]; // RA2: no IsJumpJet (not in RA2's layout)
	BuildingTypeClass * fogged_building = NULL;
	FoggedObjectClass * fogged_object = NULL;

	if (Scen->Special.IsFogOfWar && check_fog && cellptr->FoggedObjects != NULL) {
		for (int i = 0; i < cellptr->FoggedObjects->Count(); i++) {
			fogged_object = (*cellptr->FoggedObjects)[i];
			if (fogged_object->CanDraw && fogged_object->RTTI == RTTI_BUILDING) {
				if (fogged_object->RTTI == RTTI_BUILDING) {
					fogged_building = (BuildingTypeClass *)fogged_object->Records[0].TypeClass;
				} else {
					fogged_building = NULL;
				}
				break;
			}
		}
	}

	if (Class->IsEngineer && fogged_building != NULL && House->Is_Player_Control()) {

		/*
		**	Engineers may repair a destroyed bridge.
		*/
		if (fogged_building->IsBridgeRepairHut) {
			action = Map.Can_Repair_Bridge(fogged_object->Center_Coord()) ? ACTION_GREPAIR : ACTION_NO_GREPAIR; // RA2: the executable's cell action (tunnels, deployed soldiers, fogged buildings)
			goto done; // RA2: as above
		}
		if (fogged_building->IsRepairable) {
			action = ACTION_CAPTURE;
			goto done; // RA2: the executable's cell action (tunnels, deployed soldiers, fogged buildings)
		}
	}

	if (action == ACTION_ATTACK && !Is_Weapon_Equipped()) {
		action = ACTION_ATTACK_SUPPORT;
	}
done: // RA2: the structured if/else forms do not match; the exe shares this tail between the engineer cases and the fall-through
	if (House->Is_Player_Control() && action == ACTION_NONE && !Class->JumpJet && !IsBeingWarpedOut() && !IsWarpingIn()) { // RA2: a soldier that is not warping may move
		action = ACTION_NOMOVE;
	}

	return(action);
}


/***********************************************************************************************
 * InfantryClass::Class_Of -- Returns the class reference for this object.                     *
 *                                                                                             *
 *    This routine will return a reference to the infantry type class object that describes    *
 *    this infantry's characteristics.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the InfantryTypeClass object associated with this      *
 *          infantry object.                                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass const * InfantryClass::Class_Of(void) const
{
	return(Class);
}


/***********************************************************************************************
 * InfantryClass::Read_INI -- Reads units from scenario INI file.                              *
 *                                                                                             *
 *    This routine is used to read all the starting units from the                             *
 *    scenario control INI file. The units are created and placed on the                       *
 *    map by this routine.                                                                     *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Housename, Typename, Strength, Cellnum, CellSublocation, Missionname,                  *
 *         Facingnum, Triggername                                                              *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the loaded scenario INI file.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Read_INI(CCINIClass const & ini)
{
	InfantryClass	* infantry;			// Working infantry pointer.
	InfantryType	classid;			// Infantry class.
	char			buf[128];
	char			* validation;
	Dir256 			dir;
	TagTypeClass	* tp;

	int len = ini.Entry_Count(INI_Name);
	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_Name, index);

		/*
		**	Get an infantry entry
		*/
		ini.Get_String(INI_Name, entry, NULL, buf, sizeof(buf));

		/*
		**	1st token: house name.
		*/
		int house_index = House_Index_From_Name(strtok(buf, ",")); // RA2: the house is found by index in Houses
		if (house_index != -1) { // RA2: as above

			HouseClass * inhousep = Houses[house_index]; // RA2: as above

			/*
			**	2nd token: infantry type name.
			*/
			classid = InfantryTypeClass::From_Name(strtok(NULL, ","));

			if (classid != INFANTRY_NONE) {

				infantry = new InfantryClass(InfantryTypes[classid], inhousep);
				if (infantry != NULL) {

					/*
					**	3rd token: strength.
					*/
					int strength = atoi(strtok(NULL, ","));

					/*
					**	4th token: cell #.
					*/
					Coord coord; // RA2: as above
					if (NewINIFormat >= 4) {
						int x = atoi(strtok(NULL, ",")); // RA2: as above
						int y = atoi(strtok(NULL, ",")); // RA2: as above
						coord = Cell((short)x, y).As_Coord(); // RA2: the cell is built and converted inside each branch, x narrowed to a short
					} else {
						int cellnum = atoi(strtok(NULL, ","));
						coord = Cell((short)(cellnum % 128), (short)(cellnum / 128)).As_Coord(); // RA2: as above
					}

					/*
					**	5th token: cell sub-location.
					*/
					int sub = atoi(strtok(NULL, ","));
					coord = Coord_Whole(coord) + StoppingCoordAbs[sub];

					/*
					**	Fetch the mission and facing.
					*/
					MissionType mission = MissionClass::Mission_From_Name(strtok(NULL, ","));
					validation = strtok(NULL, ",");
					if (validation) {
						dir = (Dir256)atoi(validation);
					} else {
						dir = DIR_N;
					}

					validation = strtok(NULL, ",");
					if (validation) {
						tp = TagTypeClass::From_Name(validation);
						if (tp != NULL) {
							TagClass * tt = Find_Or_Make(tp);
							if (tt != NULL) {
								infantry->Attach_Tag(tt);
							}
						}
					}

					validation = strtok(NULL, ",");
					if (validation) {
						infantry->Veterancy.From_Integer(atoi(validation));
					}

					validation = strtok(NULL, ",");
					if (validation) {
						infantry->Group = atoi(validation);
					}

					validation = strtok(NULL, ",");
					if (validation) {
						infantry->IsOnBridge = atoi(validation) != 0;
						if (infantry->IsOnBridge) {
							coord.Z = Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT;
						} else {
							coord.Z = Map.Get_Height_GL(coord);
						}
					}

					validation = strtok(NULL, ",");
					if (validation) {
						infantry->IsTeamRecruitable = atoi(validation) != 0;
					}

					validation = strtok(NULL, ",");
					if (validation) {
						infantry->IsAutocreateRecruitable = atoi(validation) != 0;
					}

					if (&Map[coord] != &BlubCell && infantry->Unlimbo(coord, dir)) {
						infantry->Strength = infantry->Class->MaxStrength * (strength / 256.0);
						if (infantry->Strength > infantry->Class->MaxStrength-3) infantry->Strength = infantry->Class->MaxStrength;
						if (infantry->Strength < 1) infantry->Strength = 1;
						infantry->EstimatedHealth = infantry->Strength; // RA2: the health the owner last saw
						if (Session.Type == GAME_NORMAL || infantry->House->Is_Human_Player()) {
							infantry->QueueMission(mission, false); // RA2: the mission is queued
							infantry->Commence();
						} else {
							infantry->Enter_Idle_Mode();
						}
					} else {

						/*
						**	If the infantry could not be unlimboed, then this is a big error.
						**	Delete the infantry.
						*/
						delete infantry;
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Write_INI -- Store the infantry to the INI database.                         *
 *                                                                                             *
 *    This will store all the infantry objects to the INI database specified.                  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to store the infantry data to.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Write_INI(CCINIClass & ini)
{
	ini.Clear(INI_Name);

	for (int index = 0; index < Infantry.Count(); index++) {
		InfantryClass * infantry = Infantry[index];
		if (!infantry->IsInLimbo) {
			char uname[10]; // RA2: Write_INI at 0x506D70 matches only so
			char buf[128]; // RA2: as above

			sprintf(uname, "%d", index);
			sprintf(buf, "%s,%s,%d,%d,%d,%d,%s,%d,%s,%d,%d,%d,%d,%d",
					(char const *)infantry->House->IniName, // RA2: as above
					(char const *)infantry->Class->IniName,
					int(infantry->HealthRatio * 256), // RA2: as above
					infantry->PositionCell.X,
					infantry->PositionCell.Y,
					CellClass::Spot_Index(infantry->PositionCoord),
					MissionClass::Mission_Name((infantry->Get_Mission() == MISSION_NONE) ? // RA2: as above
						infantry->MissionQueue : infantry->Get_Mission()), // RA2: as above
					infantry->PrimaryFacing.Current().As_Dir256(),
					infantry->Tag != NULL ? (char const *)infantry->Tag->Class->IniName : "None",
					infantry->Veterancy.To_Integer(),
					infantry->Group,
					infantry->IsOnBridge,
					infantry->IsTeamRecruitable,
					infantry->IsAutocreateRecruitable
				);
			ini.Put_String(INI_Name, uname, buf);
		}
	}
}


/// <summary>
/// RA2
/// </summary>
static inline void Reduce_Fear(int & fear)
{
	fear--;
}


/***********************************************************************************************
 * InfantryClass::Fear_AI -- Process any fear related affects on this infantry.                *
 *                                                                                             *
 *    Use this routine to handle the fear logic for this infantry. It will slowly increase     *
 *    the bravery of the infantry as well as cause it to stand up or lie down as appropriate.  *
 *    It will even handle the special fraidy cat logic for civilian infantry.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this once per game logic loop per infantry unit.                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Fear_AI(void)
{
	/*
	**	After a time, the infantry will gain courage.
	*/
	if (Fear > FEAR_NONE) {
		if (!Class->IsFearless) {
			Fear--;
		}

		/*
		**	When an armed civilian becomes unafraid, he will then reload
		**	another clip into his pistol.
		*/
		if (Fear == FEAR_NONE && Ammo == 0 && Is_Weapon_Equipped()) {
			Ammo = Class->MaxAmmo;
		}

		/*
		**	Stand up if brave and lie down if afraid.
		*/
		if (IsProne) {
			if (Fear < 50 && !Is_Deploying()) {
				Do_Action(DO_GET_UP, false, false);
			}
		} else if (Fear >= 50 && !Is_Deploying()) {
			if (!(House->Is_Human_Player() && (NavCom != 0 || Locomotion->Is_Moving()))
				&& !Class->IsFraidyCat) {
				Do_Action(DO_LIE_DOWN, false, false);
			}
		}
	}
	if (Class->IsFraidyCat && Fear > 50 && !Is_Deploying() && !IsFalling) {
		if (!Locomotion->Is_Moving() && NavCom == 0) {
			Scatter(COORD_NONE, true, false);
		}
	}
}



/***********************************************************************************************
 * InfantryClass::Edge_Of_World_AI -- Detects when infantry has left the map.                  *
 *                                                                                             *
 *    This routine will detect when the infantry has left the edge of the world and will       *
 *    delete it as necessary.                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry unit deleted by this routine?                               *
 *                                                                                             *
 * WARNINGS:   Be sure the check the return value and if true, abort any further processing    *
 *             for this infantry unit.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Edge_Of_World_AI(void)
{
	/*
	**	Delete this unit if it finds itself off the edge of the map and it is in
	**	guard or other static mission mode.
	*/
	if (Team != NULL && IsLocked) Team->IsLeaveMap = true;

	if (Team == NULL && Mission == MISSION_GUARD && !Map.In_Radar(PositionCoord)) {
		Stun();
		Delete_Me();
		return(true);
	}
	return(false);
}


/// <summary>
/// Handles a thief stealing the vehicle it has been sent after.
/// A thief that gets close enough to an enemy vehicle on its own level captures the
/// vehicle outright and is consumed in the process. Failing that, the thief keeps
/// chasing, re-aiming itself whenever the vehicle wanders away from where it was
/// headed.
/// </summary>
/// <returns>bool; Was the vehicle stolen, and this infantry consumed doing it?</returns>
bool InfantryClass::Theft_AI(void)
{
	if (!Class->IsThief) return(false);
	if (NavCom == NULL) return(false);
	if (NavCom->Fetch_RTTI() != RTTI_UNIT) return(false); // RA2: Theft_AI at 0x507170 matches only so

	UnitClass * unit = (UnitClass *)NavCom;
	if (House->Is_Ally(unit)) return(false);

	Coord coord = unit->Destination_Coord(this); // RA2: as above
	int z = coord.Z;
	coord = Center_Coord();

	if (abs(z - coord.Z) < LEVEL_LEPTON_H) {
		if (Center_Coord().Distance_To(unit->Center_Coord()) < CELL_LEPTON / 2) { // RA2: as above
			if (unit->Tag != NULL) {
				unit->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
			}
			unit->Transmit_Message(RADIO_OVER_OUT);
			unit->Detach_All(false);
			if (Tag != NULL && Tag->Is_To_Inherit()) {
				unit->Attach_Tag(Tag);
			}
			unit->Captured(House, true); // RA2: as above
			unit->EnteredByInfType = Class->HeapID;
			Delete_Me();
			return(true);
		}
	}

	if (Center_Coord().Distance_To(unit->Center_Coord()) < 2 * CELL_LEPTON) { // RA2: as above
		if (Coord(Locomotion->Destination()) != COORD_NONE) {
			if (unit->Distance(Coord(Locomotion->Destination())) <= CELL_LEPTON / 2) {
				return(false);
			}
		}
		Assign_Destination(NavCom);
	} else {
		if (Coord(Locomotion->Destination()) != COORD_NONE) {
			if (unit->Destination_Coord(this).As_Cell() == Coord(Locomotion->Destination()).As_Cell()) { // RA2: as above
				return(false);
			}
		}
	}
	Assign_Destination(NavCom);
	return(false);
}


/***********************************************************************************************
 * InfantryClass::Firing_AI -- Handles firing and combat AI for the infantry.                  *
 *                                                                                             *
 *    This will examine the infantry and determine what firing action is required. It will     *
 *    search for targets, starting firing animations, and launch bullets as necessary.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per infantry per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Firing_AI(void)
{
	if (TarCom != NULL) {
		int primary = What_Weapon_Should_I_Use(TarCom);

		if (!IsFiring) {
			switch (Can_Fire(TarCom, primary, true)) { // RA2: ignoreRange
				case FIRE_ILLEGAL:
					if (Combat_Damage(primary) < 0) {
						AbstractClass * targ = TarCom; // RA2: the target is tested for infantry by its flags and type
						if (targ != NULL && (targ->AbstractFlags & ABSTRACT_OBJECT) && targ->Fetch_RTTI() == RTTI_INFANTRY) { // RA2: as above
							if (!(((ObjectClass *)targ)->Get_Health_Ratio() < Rule->ConditionGreen)) { // RA2: as above
								Assign_Target(NULL);
							}
						} else {
							Assign_Target(NULL);
						}
					}
					break;

				case FIRE_CLOAKED:
					Do_Uncloak();
					break;

				case FIRE_OK:
					/*
					**	Start firing animation.
					*/
					if (Is_JumpJet()) {
						Do_Action(DO_FIREFLY); // RA2: FireFly
					} else {
						if (Is_Deploying()) { // RA2: a deployed soldier fires in its deployed sequence
							Do_Action(DO_DEPLOYED_FIRE); // RA2: as above
						} else if (IsProne) { // RA2: as above
							Do_Action(DO_FIRE_PRONE);
						} else {
							Do_Action(DO_FIRE_WEAPON);
						}
					}

					IsFiring = true;
					PrimaryFacing.Set(Direction(TarCom));

					/*
					**	If the target is in range, and the NavCom is the same, then just
					**	stop and keep firing.
					*/
					if (TarCom == NavCom) {
						Abort_Motion(); // RA2: Abort_Motion and Stop_Driver instead of clearing the NavCom and path
						Stop_Driver();
					}
					break;
			}
		}

		/*
		**	If in the middle of firing animation, then only
		**	process that. Infantry cannot fire and move simultaneously.
		**	At some point in the firing animation process, a projectile
		**	will be launched. When the required animation frames have
		**	been completed, the firing animation stops.
		*/
		int firestage = Class->FireLaunch;
		if (IsProne) firestage = Class->ProneLaunch;

		if (IsFiring && Fetch_Stage() == firestage) {

			primary = What_Weapon_Should_I_Use(TarCom);
			if (Can_Fire(TarCom, primary, true) == FIRE_OK) { // RA2: the executable's firing AI (jumpjet, deploy, abort motion)
				Fire_At(TarCom, primary);
			} else {
				IsFiring = false;
				if (IsProne) {
					Do_Action(DO_PRONE);
				} else if (Is_Deploying()) { // RA2: back to the deployed sequence
					Do_Action(DO_DEPLOYED); // RA2: as above
				} else {
					Do_Action(DO_STAND_READY);
				}
			}

			if (TarCom != NULL) { // RA2: the target may have gone
				const WeaponDataStruct * wdata = Get_Class_Weapon_Data(0);
				if (wdata->Weapon->MaxSpeed < Rule->Incoming) {
					Map[TarCom->Center_Coord()].Incoming(PositionCoord, true);
				}
			}
		}
	} else {
		IsFiring = false;
	}
}


/***********************************************************************************************
 * InfantryClass::Doing_AI -- Handles the animation AI processing.                             *
 *                                                                                             *
 *    Infantry can be in one of many different animation sequences. At the conclusion of each  *
 *    sequence, the infantry will quite likely transition to a new animation state. This       *
 *    routine handles detecting when that trasition should occur and starting the infantry     *
 *    into its new state.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per infantry unit per game logic loop.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
inline AnimTypeClass const * Pick_Dead_Body(unsigned draw, TypeList<AnimTypeClass const *> const & list) // RA2: a file-local inline (meastes' Pick_Animation): list[draw % Count]
{
	return(list[draw % (unsigned)list.Count()]); // RA2: as above
}

void InfantryClass::Doing_AI(void)
{
	if (Doing == DO_NOTHING || Fetch_Stage() >= Class->DoControls[Doing].Count) {
		switch (Doing) {
			case DO_DEPLOY: // RA2: the end of the deploy sequence starts the deployed one
				Do_Action(DO_DEPLOYED, true, false); // RA2: as above
				Shorten_Targeting_Timer(); // RA2: as above
				break;

			case DO_UNDEPLOY: // RA2: stand ready again
				Do_Action(DO_STAND_READY, true, false); // RA2: as above
				break;

			default:
				if (Doing != DO_NOTHING) { // RA2: face the sequence's facing
					int facing = Class->DoControls[Doing].Facing; // RA2: as above
					if (facing != FACING_NONE) { // RA2: as above
						PrimaryFacing.Set(DirType(facing << 13)); // RA2: as above
					}
				}
				if (Locomotion->Is_Moving() && Speed > 0.1) { // RA2: still moving
					if (IsProne) {
						Do_Action(DO_CRAWL, true, false); // RA2: the executable's sequence end handling
					} else {
						Do_Action(DO_WALK, true, false); // RA2: as above
					}
				} else {
					if (Is_Deploying()) { // RA2: as above
						Do_Action(DO_DEPLOYED, true, false); // RA2: as above
					} else if (IsProne) { // RA2: as above
						Do_Action(DO_PRONE, true, false); // RA2: as above
					} else {
						Do_Action(DO_STAND_READY, true, false); // RA2: as above
					}
				}
				break;

			case DO_PARADROP: // RA2: the paradrop sequence has an empty case of its own (jump-table entry to the end); the landing check is below
				break; // RA2: as above

			case DO_AIR_DEATH_START: // RA2: a sequence that leads on into the next one
				Do_Action(DO_AIR_DEATH_FALLING, true, false); // RA2: as above
				break;

			case DO_GUN_DEATH:
			case DO_EXPLOSION_DEATH:
			case DO_EXPLOSION2_DEATH:
			case DO_GRENADE_DEATH:
			case DO_FIRE_DEATH:
				if (Fetch_Stage() >= Class->DoControls[Doing].Count) {
					if (Class->DeadBodies.Count() > 0) { // RA2: the type's own dead bodies first
						new AnimClass(Pick_Dead_Body(Scen->RandomNumber(), Class->DeadBodies), // RA2: the draw is made before the list is read (as meastes' Pick_Dead_Body)
							Center_Coord(), 0, 1, (ShapeFlags_Type)(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
					} else if (!Class->NotHuman) { // RA2: else the rules' for humans
						new AnimClass(Pick_Dead_Body(Scen->RandomNumber(), Rule->DeadBodies), // RA2: as above
							Center_Coord(), 0, 1, (ShapeFlags_Type)(SHAPE_CENTER | SHAPE_WIN_REL), 0); // RA2: as above
					}
					Delete_Me();
				}
				break;

			case DO_WET_DIE1: // RA2: the executable's sequence end handling
			case DO_WET_DIE2: // RA2: as above
			case DO_AIR_DEATH_FINISH: // RA2: as above
				if (Fetch_Stage() >= Class->DoControls[Doing].Count) {
					Delete_Me();
				}
				break;
		}
	}
	if (Doing == DO_PARADROP && !IsFalling) { // RA2: the paradrop sequence ends when landed
		Do_Action(DO_STAND_READY, true, false); // RA2: as above
	}
}


/***********************************************************************************************
 * InfantryClass::Movement_AI -- This routine handles all infantry movement logic.             *
 *                                                                                             *
 *    It examines the infantry state and determines what movement action should be initiated   *
 *    or processed. It handles the actual movement of the infantry as well as any path finding *
 *    or infantry startup logic.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per infantry unit per game logic loop.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Movement_AI(void)
{
	if (Get_Mission() == MISSION_MOVE && !Locomotion->Is_Moving()) {
		if (NavCom == NULL) {
			Enter_Idle_Mode(false, true);
		} else {
			Assign_Destination(NavCom, true);
			Set_Speed(1.0);
		}
	}

	if (NavCom != NULL) {
		Cell target_cell = NavCom->Destination_Coord(this).As_Cell();
		/*
		**	Double check to make sure it doesn't have a movement destination into a zone
		**	that it can't travel to. In such a case, abort the movement process by clearing
		**	the navigation computer.
		*/
		if (!IsZoneCheat || Can_Enter_Cell(Get_Target_Cell_Ptr(), FACING_NONE, -1, 0, 1) != MOVE_NO) {
			if (!Locomotion->Is_Moving() && !IsTethered && NavCom != NULL && IsLocked) {
				Cell current_cell = Destination_Coord(0).As_Cell();
				if (!Map.Is_Same_Cell_Zone(current_cell, target_cell, Class->MZone,
				Is_Moving_Onto_Bridge(0), Map[target_cell].IsUnderBridge, Is_Allowed_To_Leave_Map())
				&& !Class->IsCapture && Get_Mission() != MISSION_ENTER) {
					Assign_Destination(NULL, true);
				}
			}
		}
	}

	if (Locomotion->Is_Really_Moving_Now()) {
		if (Is_JumpJet()) {
			if (IsFiring) {
				return;
			}
			if (Speed > 0.8) {
				Do_Action(DO_FLY, false, false);
			} else {
				Do_Action(DO_HOVER, false, false);
			}
			return;
		}
		if (IsProne) {
			Do_Action(DO_CRAWL, false, false);
		} else {
			Do_Action(DO_WALK, false, false);
		}
	} else {
		if (Doing == DO_WALK || Doing == DO_FLY || Doing == DO_HOVER) {
			Do_Action(DO_STAND_READY, false, false);
		} else if (Doing == DO_CRAWL) {
			Do_Action(DO_PRONE, false, false);
		} else if (Doing == DO_SWIM) {
			Do_Action(DO_TREAD, false, false);
		}
		return;
	}
}



/// <summary>
/// RA2: a guarding deployer's turn. Deployed, it undeploys (UndeployDelay), fires its deploy weapon at its own cell
/// while the radiation there is low (radiation-immune types) or rocks; undeployed, a computer-owned DeployFire type
/// deploys once the difficulty's AIAutoDeployFrameDelay has passed. Returns -1 when there is nothing to do.
/// </summary>
int InfantryClass::Update_Deployed_Attack(void) // RA2: the executable's deployed attack update
{
	if (Is_Deploying()) { // RA2: as above
		if (Class->UndeployDelay > -1) { // RA2: as above
			Do_Action(DO_UNDEPLOY, false, false); // RA2: as above
			return(Class->DoControls[31].Count); // RA2: as above
		}
		if (Class->DeployFire) { // RA2: as above
			if (Class->ImmuneToRadiation) { // RA2: as above
				RadSiteClass * site = Map[Get_Cell()].Get_Rad_Site(); // RA2: as above
				WeaponTypeClass const * weapon = Get_Class_Weapon_Data(1)->Weapon; // RA2: as above
				int level = weapon->RadLevel / 3; // RA2: as above
				if (site == NULL || site->GetRadLevel() < level) { // RA2: as above
					Assign_Target(&Map[Get_Cell()]); // RA2: as above
					if (Can_Fire(TarCom, 1, true) == FIRE_OK) { // RA2: as above
						Fire_At(TarCom, 1); // RA2: as above
						Assign_Target(NULL);
						Do_Action(DO_DEPLOYED_FIRE, false, false); // RA2: as above
						return(Class->DoControls[29].Count); // RA2: as above
					}
					Assign_Target(NULL);
				}
				return(Current_Mission_Control().Normal_Delay() + Random_Pick(10, 20)); // RA2: the executable's deployed attack update
			}
			Reacquire_Target(); // RA2 // RA2: as above
			return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2)); // RA2: as above
		}
	} else {
		if (!Owner_HouseClass()->Is_Human_Player() && Class->Deployer && Class->DeployFire && Class->UndeployDelay <= -1 && NavCom == NULL && CurrentMissionStartTime + Rule->AIAutoDeployFrameDelay[House->Difficulty] < Frame) { // RA2: as above
			if (ArchiveTarget != NULL) { // RA2: as above
				if (ArchiveTarget->Center_Coord().As_Cell() != Center_Coord().As_Cell()) { // RA2: as above
					return(-1); // RA2: as above
				}
			}
			if (!Class->ImmuneToRadiation) { // RA2: as above
				if (!Locomotion->Is_Moving()) { // RA2: as above
					Do_Action(DO_DEPLOY, false, false); // RA2: as above
					return(Class->DoControls[27].Count); // RA2: as above
				}
				Locomotion->Stop_Moving(); // RA2: as above
				ShouldDeploy = true; // RA2: as above
				return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2)); // RA2: as above
			}
		}
	}
	return(-1); // RA2: the executable's deployed attack update
}






/***********************************************************************************************
 * InfantryClass::Get_Image_Data -- Fetches the image data for this infantry unit.             *
 *                                                                                             *
 *    The image data for the infantry differs from normal if this is a spy. A spy always       *
 *    appears like a minigunner to the non-owning players.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the image data to use for this infantry soldier.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * InfantryClass::Get_Image_Data(void) const
{
	return(BASECLASS::Get_Image_Data()); // RA2: no Doing, Rule, WebbedInfantry and 5 more (TibSun's use does not compile against RA2's declaration)
}


/***********************************************************************************************
 * InfantryClass::Is_Ready_To_Random_Anima -- Checks to see if it is ready to perform an idle  *
 *                                                                                             *
 *    This routine will examine this infantry and determine if it is allowed and ready to      *
 *    perform an idle animation. The conditions under which idle animations can be performed   *
 *    are restrictive. Hence this routine.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this infantry ready to do an idle animation?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Is_Ready_To_Random_Animate(void) const
{
	if (!BASECLASS::Is_Ready_To_Random_Animate()) {
		return(false);
	}
	if (Locomotion->Is_Moving()) {
		return(false);
	}
	if (IsProne) {
		return(false);
	}
	if (IsFiring) {
		return(false);
	}
	if (Doing == DO_DEPLOY) goto not_ready; // RA2: Is_Ready_To_Random_Animate at 0x508320 matches only so
	if (Doing == DO_DEPLOYED) goto not_ready; // RA2: as above
	if (Doing == DO_DEPLOYED_FIRE) goto not_ready; // RA2: as above
	if (Doing == DO_DEPLOYED_IDLE) { // RA2: as above
not_ready: // RA2: as above
		return(false);
	}
	if (Doing == DO_STAND_GUARD || Doing == DO_STAND_READY || Doing == DO_TREAD) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}


/***********************************************************************************************
 * InfantryClass::Paradrop -- Handles paradropping infantry.                                   *
 *                                                                                             *
 *    This routine will paradrop this soldier at the location specified. It will cause the     *
 *    soldier to hunt if controlled by the computer and to guard if controlledy by the         *
 *    human.                                                                                   *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to paradrop the soldier to.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the paradrop successful?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Paradrop(Coord const & coord)
{
	if (ObjectClass::Paradrop(coord)) {
		if (House->Is_Human_Player()) {
			QueueMission(MISSION_GUARD, false);
		} else {
			QueueMission(MISSION_HUNT, false);
		}
		Do_Action(DO_PARADROP, 1, 0);
		return(true);
	}
	return(false);
}



/***********************************************************************************************
 * InfantryClass::Set_Occupy_Bit -- Sets the occupy bit cell and bit pos                       *
 *                                                                                             *
 * INPUT:      CELL      - the cell we are setting the bit in                                  *
 *                                                                                             *
 *               int      - the spot index we are setting the bit for                          *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/08/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Set_Occupy_Bit(Coord const & coord)
{
	int spot_index = CellClass::Spot_Index(coord);
	CellClass & cell = Map[coord];
	if (Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT <= coord.Z && cell.IsUnderBridge) {
		cell.BridgeFlag.Composite |= (1 << spot_index);
		cell.BridgeInfType = Owner();
	} else {
		cell.Flag.Composite |= (1 << spot_index);
		cell.InfType = Owner();
	}
}


/***************************************************************************
 * InfantryClass::Clear_Occupy_Bit -- Clears occupy bit and given cell     *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/08/1995 PWG : Created.                                             *
 *=========================================================================*/
void InfantryClass::Clear_Occupy_Bit(Coord const & coord)
{
	int spot_index = CellClass::Spot_Index(coord);
	CellClass & cell = Map[coord]; // RA2: Clear_Occupy_Bit at 0x5084A0 matches only so
	int height = Map.Get_Height_GL(coord);

	if (height + BRIDGE_LEPTON_HEIGHT <= coord.Z) {
		cell.BridgeFlag.Composite &= ~(1 << spot_index);
		if (!(cell.BridgeFlag.Composite & 0x1C)) {
			cell.BridgeInfType = HOUSE_NONE;
		}
	} else {
		cell.Flag.Composite &= ~(1 << spot_index);
		if (!(cell.Flag.Composite & 0x1C)) {
			cell.InfType = HOUSE_NONE;
		}
	}
}

/// <summary>
/// RA2: a soldier in its deploy sequences fires its type's DeployFireWeapon when it has DeployFire.
/// </summary>
int InfantryClass::What_Weapon_Should_I_Use(AbstractClass * target) const
{
	if ((Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE) && Class->DeployFire) {
		return(Class->DeployFireWeapon);
	}
	return(BASECLASS::What_Weapon_Should_I_Use(target));
}





/// <summary>
/// Reads this infantry back in from the save game stream.
/// The object is re-registered with the target tracker, its virtual table pointers are
/// rebuilt, and its class pointer is swizzled back into a live one. Save games written
/// by older versions are shifted to make room for members that did not exist then.
/// </summary>
/// <returns>Returns with S_OK if the object was read successfully.</returns>
HRESULT STDMETHODCALLTYPE InfantryClass::Load(IStream * stream)
{
	TargetTracker.Remove_Index(Fetch_ID()); // RA2: Load at 0x508570 has no int size = Fetch_Object_Size(false);
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		ILocomotion * locomotion = Locomotion; // RA2: Load at 0x508570 has no IsOldSaveGame, LookTimer, ProneStruggleTimer and 1 more

		new (this) InfantryClass(NoInitClass());

		Locomotion = locomotion;
		locomotion->Release();

		TargetTracker.Add_Index(Fetch_ID(), this);
		Swizzle_Pointer(&Class);
		Swizzle_Pointer(&Disguise); // RA2: Load at 0x508570 matches only so
		Swizzle_Pointer(&DisguisedAsHouse); // RA2: as above

		result = S_OK;
	}
	return(result);
}


/// <summary>
/// Writes this infantry out to the save game stream.
/// </summary>
/// <returns>Returns with S_OK if the object was written successfully.</returns>
HRESULT STDMETHODCALLTYPE InfantryClass::Save(IStream * stream, BOOL cleardirty)
{
	return(BASECLASS::Save(stream, cleardirty));
}


/// <summary>
/// Stops the walking or crawling animation.
/// The locomotion code calls this routine when the infantry comes to rest, so that the
/// soldier does not carry on marching in place.
/// </summary>
void InfantryClass::Stop_Movement_Animation(void)
{
	if (Doing == DO_CRAWL || Doing == DO_WALK || Doing == DO_SWIM) { // RA2: Stop_Movement_Animation at 0x508730 matches only so
		Doing = DO_NOTHING; // RA2: as above
	}
}

/// <summary>
/// RA2: starts the deploy that Update_Deployed_Attack put off while the soldier was still moving.
/// </summary>
void InfantryClass::Unknown_Virtual_4D0(void)
{
	if (ShouldDeploy) {
		ShouldDeploy = false;
		Do_Action(DO_DEPLOY, false, false);
	}
}


/// <summary>
/// Is the infantry ready to be given something new to do?
/// The mission logic uses this routine to find a safe moment to change the infantry's
/// orders. A soldier that is firing, falling, or partway through an animation that
/// must not be cut short will refuse until it is finished.
/// </summary>
/// <returns>bool; Can the infantry be handed a new order right now?</returns>
bool InfantryClass::Ready_To_Commence(void)
{
	if (CurrentMission != MISSION_STICKY && CurrentMission != MISSION_RESCUE) {
		if (!IsFiring && !IsFalling) {
			if (Locomotion->Is_Moving_Now() && Mission != MISSION_GUARD && Mission != MISSION_HUNT && (Mission != MISSION_ATTACK || TarCom)) {
				return(false);
			}
			if (Doing == DO_NOTHING || InfantryClass::MasterDoControls[Doing].Interrupt) {
				return(true);
			}
		}
	}
	return(false);
}








/// <summary>
/// Frightens this infantry as much as it can be frightened.
/// Types flagged as fearless, and veterans who have earned the fearless ability, pay
/// this routine no attention at all.
/// </summary>
void InfantryClass::Start_Fear(void)
{
	if (!Class->IsFearless && !Has_Ability(ABILITY_FEARLESS)) {
		Fear = (FearType)0x12c; // RA2: Start_Fear at 0x508820 matches only so
	}
}


/// <summary>
/// Clears any fear this infantry is feeling.
/// Use this routine to bring a cowering soldier back to its senses immediately, rather
/// than waiting for its nerve to recover on its own.
/// </summary>
void InfantryClass::Stop_Fear(void)
{
	if (!Class->IsFearless) {
		Fear = FEAR_NONE;
	}
}


/// <summary>
/// Performs one of the infantry's idle animations.
/// This routine is used by the random animation logic to give a soldier that has been
/// standing around too long something to do with itself.
/// </summary>
/// <param name="which">The idle animation variation to perform.</param>
void InfantryClass::Do_Idle(int which)
{
	switch (which) {
		case 0:
		default:
			Do_Action(DO_IDLE1);
			break;
		case 1:
			Do_Action(DO_IDLE2);
			break;
	}
}


/// <summary>
/// Submits this infantry's state to the game checksum.
/// The multiplayer sync check uses this routine to prove that every machine agrees
/// about the object. Only the members that steer the game logic are submitted.
/// </summary>
/// <param name="crc">The checksum engine to submit the values to.</param>
void InfantryClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
	crc(Doing);
	crc((int)Comment);
	crc(Fear);
	crc(IsTechnician);
	crc(IsStoked);
	crc(IsProne);
	crc(IsZoneCheat);
	crc(WasSelected);
}


/// <summary>
/// Fetches the coordinate that this infantry's weapon is mounted at.
/// Infantry carry their weapon rather than mount it, so this routine just hands back the
/// same spot the weapon is fired from.
/// </summary>
/// <param name="which">The weapon slot to fetch the mounting for.</param>
/// <returns>Returns with the coordinate the projectile should first appear at.</returns>
Coord InfantryClass::Turret_Coord(int which) const
{
	return(Fire_Coord(which)); // RA2: no offset argument
}


/// <summary>
/// Fetches the speed this infantry should be moving at.
/// Going prone changes the pace. The types that crawl are slowed down by it, while the
/// types that merely duck actually cover ground faster than they do upright.
/// </summary>
/// <returns>Returns with the movement speed to use.</returns>
int InfantryClass::Current_Speed(void)
{
	int speed = BASECLASS::Current_Speed();
	if (IsProne) {
		if (Class->IsCrawling) {
			speed -= speed / 3;
		} else {
			speed += speed / 2;
		}
	}
	return(speed);
}


/// <summary>
/// Handles the infantry discovering that its way is blocked.
/// The locomotion code calls this routine when the infantry cannot proceed along its
/// path, so that the object gets a chance to react to the obstruction.
/// </summary>
void InfantryClass::On_Movement_Blocked(void)
{
	BASECLASS::On_Movement_Blocked();
	if (NavCom != NULL && Distance(NavCom) < CELL_LEPTON_W * 4 && Is_JumpJet()) {
	}
}


/// <summary>
/// Switches a grounded jumpjet over to walking.
/// This routine is used when the remaining journey is too short to be worth taking off
/// for. A walk locomotor is piggybacked onto the jumpjet one and carries the infantry
/// the rest of the way to its navigation target.
/// </summary>
/// <returns>bool; Was the walk locomotor engaged?</returns>
bool InfantryClass::JumpJet_To_Walk(void)
{
	int path_length = 0;
	if (NavCom != NULL) {
		for (int i = 0; i < CONQUER_PATH_MAX; i++) {
			if (Path[i] == FACING_NONE) break;
			if (Path[i] == FACING_COUNT) break;
			path_length++;
		}
	}
	if (path_length >= 4) return(false);

	if (Is_JumpJet()) {
		IPiggybackPtr piggy(Locomotion);
		if (piggy != NULL && !piggy->Is_Piggybacking()) {
			ILocomotionPtr walk(CLSID_WalkLocomotion);
			walk->Link_To_Object(this);
			piggy = IPiggybackPtr(walk);
			if (piggy != NULL) {
				Path[0] = FACING_NONE;
				piggy->Begin_Piggyback(Locomotion);
				Locomotion = walk;
				Locomotion->Move_To(NavCom->Center_Coord());
				return(true);
			}
		}
	}

	return(false);
}


/// <summary>
/// Sends this infantry berzerk.
/// Use this routine to drive a soldier out of its mind. A berzerk soldier will attack
/// whatever happens to be nearby, with no regard for who owns it.
/// </summary>
void InfantryClass::Berzerk(void)
{
	IsBerzerk = true;
}


/// <summary>
/// RA2
/// </summary>
static inline bool Is_Deploy_Doing(int doing)
{
	return(doing == 0x1b || doing == 0x1c || doing == 0x1d || doing == 0x1e);
}


/// <summary>
/// Is this infantry flying under jumpjet power?
/// A jumpjet type can have a walk locomotor piggybacked onto it while it is down on
/// the ground, so the type alone is not enough -- the locomotor actually in control
/// is examined as well.
/// </summary>
/// <returns>bool; Is the jumpjet locomotor the one in control?</returns>
bool InfantryClass::Is_JumpJet(void) const
{
	if (!Class->JumpJet) { // RA2: Is_JumpJet at 0x508D00 matches only so
		return(false);
	}

	IPersistPtr persist(Locomotion);
	CLSID clsid;
	persist->GetClassID(&clsid);
	return((clsid == CLSID_JumpjetLocomotion) ? true : false);
}


/// <summary>
/// Should the jumpjet infantry fly in order to reach the destination cell?
/// This routine is used to choose between flying and walking for the next leg of a
/// journey. A trip that ground movement can manage easily is walked, so that jumpjets
/// do not take off for every step, but anything ground movement cannot reach -- or
/// would take too long over -- is flown instead. An ion storm keeps the unit down.
/// </summary>
/// <param name="from">The cell the infantry is starting out from.</param>
/// <param name="to">The cell the infantry wants to reach.</param>
/// <returns>bool; Should the trip be made by air?</returns>
bool InfantryClass::Should_JumpJet_Fly(Cell const & from, Cell const & to)
{
	int height = HeightAGL;
	height = height; /// dead code

	if (House->Ion_Sensitives_Should_Be_Offline()) { // RA2: per house, not the ion storm
		return(false);
	}

	if (HeightAGL > 0) {
		return(true);
	}

	if (House->Is_Human_Player()) {
		CellClass * startptr = &Map[from];
		if (startptr != NULL && startptr->Is_Near_Tunnel_NW()) {
			return(false);
		}
	}

	if (Map.Is_Same_Cell_Zone(from, to, MZONE_INFANTRY, IsOnBridge, Map[to].IsUnderBridge, false)) {
		if (from == to) {
			return(false);
		}
		int dist = MAX(abs(to.X - from.X), abs(to.Y - from.Y));
		if (dist == 1) {
			return(false);
		}
		if (dist >= 4 || !Map.In_Local_Radar(from) || !Map.In_Local_Radar(to)) { // RA2: 4, not 12
			return(true);
		}
		return(Search.Test_Cell_Walk(from, to, this, IsOnBridge, Map[to].IsUnderBridge, MZONE_INFANTRY) > 7); // RA2: 7, not 15
	}

	return(true);
}


/// <summary>
/// Moves the infantry closer to whatever it is attacking.
/// Engineers are the special case here -- they must physically reach the building they
/// have been aimed at, so they are given it as a movement destination rather than
/// merely closing to weapon range.
/// </summary>
void InfantryClass::Approach_Target(void)
{
	if (TarCom == NULL) { // RA2: the engineer branch is gone; a deployer fires from where it stands
		return;
	}
	if (!Is_Deploying()) { // RA2: as above
		BASECLASS::Approach_Target();
	}
	if (Owner_HouseClass()->Is_Human_Player()) { // RA2: AI deployers only
		return;
	}
	if (!Class->Deployer || !Class->DeployFire) { // RA2: as above
		return;
	}
	int weapon = Class->DeployFireWeapon; // RA2: as above
	bool in_range = In_Range(TarCom, weapon); // RA2: as above
	WeaponTypeClass * weapon_type = Get_Class_Weapon_Data(weapon)->Weapon; // RA2: as above
	if (weapon_type == NULL || weapon_type->AreaFire) { // RA2: as above
		return;
	}
	if (Is_Deploying()) { // RA2: undeploy again when the target is out of range
		if (!in_range) { // RA2: as above
			Do_Action(DO_UNDEPLOY, false, false); // RA2: as above
		}
		return;
	}
	if (!in_range) { // RA2: as above
		return;
	}
	if (!Locomotion->Is_Moving()) { // RA2: standing still: start deploying
		Do_Action(DO_DEPLOY, false, false); // RA2: as above
		return;
	}
	Locomotion->Stop_Moving(); // RA2: moving: stop and deploy later
	ShouldDeploy = true; // RA2: as above
	(void)(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2)); // RA2: the exe evaluates both and drops the sum
}


/// <summary>
/// Can this infantry repair and capture buildings?
/// </summary>
/// <returns>bool; Is this soldier an engineer?</returns>
bool InfantryClass::Is_Renovator(void) const
{
	return(Class->IsEngineer);
}

/// <summary>
/// RA2: whether this soldier's type deploys.
/// </summary>
bool InfantryClass::Is_Deployer(void) const
{
	return(Class->Deployer);
}


/// <summary>
/// RA2: whether this soldier's type undeploys by itself after UndeployDelay.
/// </summary>
bool InfantryClass::Has_Undeploy_Delay(void) const
{
	return(Class->UndeployDelay > -1);
}


/// <summary>
/// RA2: whether this soldier is in one of the deploy sequences (deploy, deployed, deployed fire, deployed idle).
/// </summary>
bool InfantryClass::Is_Deploying(void) const
{
	return(Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE);
}


/// <summary>
/// RA2: whether this soldier's type fires when deployed.
/// </summary>
bool InfantryClass::Is_Deploy_Fire(void) const
{
	return(Class->DeployFire);
}


/// <summary>
/// RA2: deploys a standing soldier, or stops a moving one to deploy once it has stopped.
/// </summary>
int InfantryClass::Deploy_Or_Stop(void)
{
	if (!Locomotion->Is_Moving()) {
		Do_Action(DO_DEPLOY, false, false);
		return(Class->DoControls[27].Count);
	}
	Locomotion->Stop_Moving();
	ShouldDeploy = true;
	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}


/// <summary>
/// RA2: the iron curtain kills infantry outright.
/// </summary>
void InfantryClass::IronCurtain(int duration)
{
	int damage = Class->MaxStrength;
	Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false);
}


/// <summary>
/// RA2: the type an enemy sees: the disguise, or the default spy disguise for the player's side.
/// </summary>
ObjectTypeClass * InfantryClass::GetDisguise(bool against_allies) const
{
	if (House->Is_Ally(PlayerPtr) && !against_allies) {
		return(Class);
	}
	if (Disguise == NULL) {
		if (PlayerPtr != NULL && PlayerPtr->ActLike == 0) {
			return((ObjectTypeClass *)Rule->AlliedDisguise);
		}
		return((ObjectTypeClass *)Rule->SovietDisguise);
	}
	return(Disguise);
}


/// <summary>
/// RA2: the house an enemy sees: the disguise's house, or the player's.
/// </summary>
HouseClass * InfantryClass::GetDisguiseHouse(bool against_allies) const
{
	if (House->Is_Ally(PlayerPtr) && !against_allies) {
		return(House);
	}
	if (DisguisedAsHouse == NULL) {
		return(PlayerPtr);
	}
	return(DisguisedAsHouse);
}


#pragma warning(push)
#pragma warning(disable: 4716)
/// <summary>
/// RA2: a spy takes the look of another soldier, or of that soldier's own disguise.
/// </summary>
unsigned InfantryClass::DisguiseAs(AbstractClass * target)
{
	if (target != NULL && target->Fetch_RTTI() == RTTI_INFANTRY) {
		BASECLASS::DisguiseAs(target);
		ObjectClass * object = (ObjectClass *)target;
		if (object->IsDisguised()) {
			Disguise = object->GetDisguise(true);
			DisguisedAsHouse = object->GetDisguiseHouse(true);
		} else {
			Disguise = (ObjectTypeClass *)object->Class_Of();
			DisguisedAsHouse = object->Owner_HouseClass();
		}
	}
}
#pragma warning(pop)


/// <summary>
/// RA2: a PermaDisguise soldier goes back to its side's default disguise; others drop theirs.
/// </summary>
void InfantryClass::ClearDisguise(void)
{
	if (Class->PermaDisguise) {
		Disguised = true;
		DisguisedAsHouse = House;
		if (House->ActLike == 0) {
			Disguise = (ObjectTypeClass *)Rule->AlliedDisguise;
		} else {
			Disguise = (ObjectTypeClass *)Rule->SovietDisguise;
		}
	} else {
		Disguised = false;
	}
}


/// <summary>
/// RA2: whether the disguise fools this house (not allied, no disguise sensor on the cell).
/// </summary>
bool InfantryClass::IsDisguisedAs(HouseClass * house) const
{
	if (IsDisguised()) {
		CellClass * cell = &Map[Center_Coord().As_Cell()];
		if (!House->Is_Ally(house) && !cell->Is_Disguise_Sensed(house->HeapID)) {
			HouseClass * disguiser = DisguisedAsHouse;
			if (disguiser == house || disguiser == NULL || house->Is_Ally(disguiser)) {
				return(true);
			}
		}
	}
	return(false);
}


/// <summary>
/// RA2: TechnoClass's.
/// </summary>
bool InfantryClass::CanUseWaypoint(void) const
{
	return(TechnoClass::CanUseWaypoint());
}


/// <summary>
/// RA2: TechnoClass's.
/// </summary>
bool InfantryClass::CanAttackOnTheMove(void) const
{
	return(TechnoClass::CanAttackOnTheMove());
}


/// <summary>
/// RA2: whether a Deployer is in its deploy sequences.
/// </summary>
bool InfantryClass::IsDeployed(void) const
{
	return(Class->Deployer && (Doing == DO_DEPLOY || Doing == DO_DEPLOYED || Doing == DO_DEPLOYED_FIRE || Doing == DO_DEPLOYED_IDLE));
}


/// <summary>
/// RA2: an occupier garrisons the building; an assaulter clears it and steps aside.
/// </summary>
void InfantryClass::Enter_Occupiable(BuildingClass * building)
{
	if (Class->Occupier) {
		Limbo();
		building->Occupants.Add(this);
		building->UpdateThreatInCell(building->Get_Cell_Ptr());
	} else if (Class->Assaulter) {
		building->KillOccupants(this);
		Assign_Destination(NULL, true);
		Scatter(building->Center_Coord(), true, true);
	}
}


/// <summary>
/// RA2: the notice sink. A balloon or jumpjet soldier in radio contact answers notice 0x117B by attacking its
/// archived target or scattering; a Crashable one lands on notice 0x117C.
/// </summary>
bool InfantryClass::Unknown_Virtual_0(int code, int)
{
	if ((Class->BalloonHover || Class->JumpJet) && code == 0x117B && HasAnyLink()) {
		Transmit_Message(RADIO_UNLOADED);
		if (ArchiveTarget != NULL && ArchiveTarget != NavCom) {
			Assign_Destination(ArchiveTarget, true);
			return(true);
		}
		Abort_Motion();
		Scatter(COORD_NONE, true, false);
		return(true);
	}
	if (Class->Crashable && code == 0x117C) {
		Set_Height_AGL(0);
		Do_Action(DO_AIR_DEATH_FINISH, true, false);
		return(true);
	}
	return(false);
}


/// <summary>
/// RA2: a guarding soldier standing or idling may cheer.
/// </summary>
bool InfantryClass::CanCheer(void) const
{
	if (Get_Mission() == MISSION_GUARD || Get_Mission() == MISSION_GUARD_AREA) {
		if (Doing == DO_STAND_READY || Doing == DO_IDLE1 || Doing == DO_IDLE2 || Doing == DO_CHEER) {
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// RA2: cheers when it can.
/// </summary>
void InfantryClass::Cheer(void)
{
	if (CanCheer()) {
		Do_Action(DO_CHEER, false, false);
	}
}


/// <summary>
/// RA2: whether a Do type is an idle or cheer sequence.
/// </summary>
bool InfantryClass::Is_Idle_Doing(DoType doing) const
{
	if (doing == DO_IDLE1 || doing == DO_IDLE2 || doing == DO_WET_IDLE1 || doing == DO_WET_IDLE2 || doing == DO_HOVER || doing == DO_CHEER) {
		return(true);
	}
	return(false);
}


/// <summary>
/// RA2: whether a Do type is a death sequence.
/// </summary>
bool InfantryClass::Is_Death_Doing(DoType doing) const
{
	return(doing == DO_GUN_DEATH || doing == DO_EXPLOSION_DEATH || doing == DO_EXPLOSION2_DEATH || doing == DO_GRENADE_DEATH || doing == DO_FIRE_DEATH || doing == DO_AIR_DEATH_START || doing == DO_AIR_DEATH_FALLING || doing == DO_AIR_DEATH_FINISH || doing == DO_WET_DIE1 || doing == DO_WET_DIE2);
}


/// <summary>
/// RA2: whether this soldier is playing a death sequence.
/// </summary>
bool InfantryClass::Is_Dying(void) const
{
	return(Doing == DO_GUN_DEATH || Doing == DO_EXPLOSION_DEATH || Doing == DO_EXPLOSION2_DEATH || Doing == DO_GRENADE_DEATH || Doing == DO_FIRE_DEATH || Doing == DO_AIR_DEATH_START || Doing == DO_AIR_DEATH_FALLING || Doing == DO_AIR_DEATH_FINISH || Doing == DO_WET_DIE1 || Doing == DO_WET_DIE2);
}




























































































/// <summary>
/// Fetches the byte size of this object as it appears in a save game.
/// Save games written by earlier versions of the game predate a couple of the members,
/// so the loader asks for the legacy size when it has to interpret one of those files.
/// </summary>
/// <param name="oldsave">Is the size wanted for an old format save game?</param>
/// <returns>Returns with the number of bytes this object occupies in the save file.</returns>
int InfantryClass::Fetch_Object_Size(void) const // RA2: no argument
{
	return(sizeof(*this)); // RA2: no old-save delta
}


/// <summary>
/// Fetches the class identifier used to persist this object.
/// The save system records this identifier alongside the object data so that the
/// correct kind of object can be created again when the stream is read back.
/// </summary>
/// <param name="retval">Pointer to the buffer to fill in with the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no buffer was supplied.</returns>
HRESULT STDMETHODCALLTYPE InfantryClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_InfantryClass;
	return(S_OK);
}


/// <summary>
/// Fetches the RTTI type of this object.
/// </summary>
/// <returns>Returns with RTTI_INFANTRY.</returns>
RTTIType InfantryClass::Fetch_RTTI(void) const
{
	return(RTTI_INFANTRY);
}


/// <summary>
/// Is the infantry unable to move at the moment?
/// A soldier struggling back to its feet after being knocked prone is pinned in place
/// until it has finished doing so.
/// </summary>
/// <returns>bool; Is the infantry immobilized?</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
bool InfantryClass::Is_Immobilized(void) const
{
	return(BASECLASS::IsUnderEMP()); // RA2: no ProneStruggleTimer
}
#endif

