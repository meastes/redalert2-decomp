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

/* $Header: /CounterStrike/EVENT.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : EVENT.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/09/94                                                     *
 *                                                                                             *
 *                  Last Update : November 10, 1995 [BRR]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   EventClass::EventClass -- Construct an id and cell based event.                           *
 *   EventClass::EventClass -- Construct simple target type event.                             *
 *   EventClass::EventClass -- Constructor for mission change events.                          *
 *   EventClass::EventClass -- Constructor for navigation computer events.                     *
 *   EventClass::EventClass -- Constructor for object types affecting cells event.             *
 *   EventClass::EventClass -- Constructor for sidebar build events.                           *
 *   EventClass::EventClass -- Constructs event to transfer special flags.                     *
 *   EventClass::EventClass -- Default constructor for event objects.                          *
 *   EventClass::EventClass -- Event for sequencing animations.                                *
 *   EventClass::EventClass -- Megamission assigned to unit.                                   *
 *   EventClass::Execute -- Execute a queued command.                                          *
 *   EventClass::EventClass -- construct a variable-sized event                                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "geometry.h"
#include "rect.h"

#include "event.h"
#include "ipxaddr.h" // RA2

#include "_map.h"
#include "_rules.h"
#include "anim.h"
#include "bomb.h" // RA2
#include "building.h"
#include "cell.h"
#include "conquer.h"
#include "coord.h"
#include "data.h"
#include "dbgprint.h"
#include "foot.h"
#include "ftimer.h"
#include "goptions.h"
#include "ipxmgr.h" // RA2
#include "house.h"
#include "language\language.h"
#include "mouse.h"
#include "msgbox.h"
#include "queue.h" // RA2
#include "planning.h" // RA2
#include "planmgr.h"
#include "ownertalk.h" // RA2
#include "rules.h"
#include "saveload.h"
#include "scenario.h"
#include "session.h"
#include "spawnmanager.h"
#include "stringtable.h" // RA2
#include "surface.h"
#include "techno.h"
#include "unit.h"
#include "unittype.h"
#include "wonline.h"





#include "color.hh"
#include "house.hh"
#include "ramp.hh"
#include "special.hh"





/***************************************************************************
**	Table of what data is really used in the EventClass struct for different
**	events.  This table must be kept current with the EventType enum.
*/
unsigned char EventClass::EventLength[EventClass::LAST_EVENT] = {
	0,												// EMPTY
	size_of(EventClass, Data.Target ),				/// POWERON
	size_of(EventClass, Data.Target ),				/// POWEROFF
	size_of(EventClass, Data.General ),				// ALLY
	size_of(EventClass, Data.MegaMission ),			// MEGAMISSION
	size_of(EventClass, Data.MegaMission_F ),		// MEGAMISSION_F
	size_of(EventClass, Data.Target ),				// IDLE
	size_of(EventClass, Data.Target ),				// SCATTER
	0,												// DESTRUCT
	size_of(EventClass, Data.Target ),				// DEPLOY
	size_of(EventClass, Data.Target ),				// DETONATE
	size_of(EventClass, Data.Place ),				// PLACE
	0,												// OPTIONS
	size_of(EventClass, Data.General ),				// GAMESPEED
	size_of(EventClass, Data.Specific ),			// PRODUCE
	size_of(EventClass, Data.Specific ),			// SUSPEND
	size_of(EventClass, Data.Specific ),			// ABANDON
	size_of(EventClass, Data.Target ),				// PRIMARY
	size_of(EventClass, Data.Special ),				// SPECIAL_PLACE
	0,												// EXIT
	size_of(EventClass, Data.Anim ),				// ANIMATION
	size_of(EventClass, Data.Target ),				// REPAIR
	size_of(EventClass, Data.Target ),				// SELL
	size_of(EventClass, Data.SellCell),				// SELLCELL
	size_of(EventClass, Data.Options ),				// SPECIAL
	0,												// FRAMESYNC
	0,												// MESSAGE
	size_of(EventClass, Data.FrameInfo.Delay ),		// RESPONSE_TIME
	size_of(EventClass, Data.FrameInfo ),			// FRAMEINFO
	0,												// SAVEGAME
	size_of(EventClass, Data.NavCom ),				// ARCHIVE
	size_of(EventClass, Data.Variable.Size),		// ADDPLAYER
	size_of(EventClass, Data.Timing ),				// TIMING
	size_of(EventClass, Data.ProcessTime ),			// PROCESS_TIME
	0,												/// PAGEUSER
	size_of(EventClass, Data.General ),				/// REMOVEPLAYER
	size_of(EventClass, Data.General ),				/// LATENCYFUDGE
	104,											// MEGAFRAMEINFO (RA2)
	64,												// PACKET_TIMING (RA2)
	0,												// ABOUT_TO_EXIT (RA2)
	size_of(EventClass, Data.General ),				// FALLBACK_HOST (RA2)
	size_of(EventClass, Data.Address ),				// ADDRESS_CHANGE (RA2)
	size_of(EventClass, Data.NavCom ),				// PLAN_CONNECT (RA2)
	0,												// PLAN_COMMIT (RA2)
	size_of(EventClass, Data.Target ),				// PLAN_NODE_DELETE (RA2)
	size_of(EventClass, Data.General ),				// ALL_CHEER (RA2)
};

char const * EventClass::EventNames[EventClass::LAST_EVENT] = {
	"EMPTY",
	"POWERON",
	"POWEROFF",
	"ALLY",
	"MEGAMISSION",
	"MEGAMISSION_F",
	"IDLE",
	"SCATTER",
	"DESTRUCT",
	"DEPLOY",
	"DETONATE",
	"PLACE",
	"OPTIONS",
	"GAMESPEED",
	"PRODUCE",
	"SUSPEND",
	"ABANDON",
	"PRIMARY",
	"SPECIAL_PLACE",
	"EXIT",
	"ANIMATION",
	"REPAIR",
	"SELL",
	"SELLCELL",
	"SPECIAL",
	"FRAMESYNC",
	"MESSAGE",
	"RESPONSE_TIME",
	"FRAMEINFO",
	"SAVEGAME",
	"ARCHIVE",
	"ADDPLAYER",
	"TIMING",
	"PROCESS_TIME",
	"PAGEUSER",
	"REMOVEPLAYER",
	"LATENCYFUDGE",
	"MEGAFRAMEINFO",
	"PACKETTIMING",
	"ABOUTTOEXIT",
	"FALLBACKHOST",
	"ADDRESSCHANGE",
	"PLANCONNECT",
	"PLANCOMMIT",
	"PLANNODEDELETE",
	"ALLCHEER",
};


/*
**	RA2: numbers the events made within one frame, counting up from zero; a frame other than the
**	one recorded starts the count again. The recorded frame is never updated here.
*/
unsigned char Event_Sequence(void)
{
	static int frame = -1;
	static unsigned char sequence;

	if (::Frame != frame) {
		sequence = 0;
	}
	return(sequence++);
}


/***********************************************************************************************
 * EventClass::EventClass -- Constructs event to transfer special flags.                       *
 *                                                                                             *
 *    This constructs an event that will transfer the special flags.                           *
 *                                                                                             *
 * INPUT:   data  -- The special flags to be transported to all linked computers.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(int index, SpecialClass data)
{
	if (index >= 0) {
		ID = index;
		Type = SPECIAL;
		Data.Options.Data = data;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/***********************************************************************************************
 * EventClass::EventClass -- Construct simple target type event.                               *
 *                                                                                             *
 *    This will construct a generic event that needs only a target parameter. The actual       *
 *    event and target values are specified as parameters.                                     *
 *                                                                                             *
 * INPUT:   type  -- The event type to construct.                                              *
 *                                                                                             *
 *          target-- The target value that this event is to apply to.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(int index, unsigned char type, TargetClass target)
{
	DebugString("Adding event %s\n", EventNames[type]);

	if (index >= 0) {
		ID = index;
		Type = type;
		Data.Target.Whom = target;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/// <summary>
/// Constructs a simple cell type event.
/// This will construct a generic event that needs only a cell parameter. The actual
/// event and cell values are specified as parameters.
/// </summary>
/// <param name="index">The house index of the player originating this event.</param>
/// <param name="type">The event type to construct.</param>
/// <param name="cell">The cell that this event is to apply to.</param>
EventClass::EventClass(int index, unsigned char type, Cell const & cell)
{
	DebugString("Adding event %s\n", EventNames[type]);

	if (index >= 0) {
		ID = index;
		Type = type;
		Data.SellCell.Where = cell;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/***********************************************************************************************
 * EventClass::EventClass -- Default constructor for event objects.                            *
 *                                                                                             *
 *    This constructs a simple event object that requires no parameters other than the         *
 *    type of event it is.                                                                     *
 *                                                                                             *
 * INPUT:   type  -- The type of event to construct.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(int index, unsigned char type)
{
	DebugString("Adding event %s\n", EventNames[type]);

	if (index >= 0) {
		ID = index;
		Type = type;
	} else {
		ID = -1;
		Type = EMPTY;
	}
	Frame = ::Frame;
}


/***********************************************************************************************
 * EventClass::EventClass -- Constructor for general-purpose-data events.                      *
 *                                                                                             *
 * INPUT:   type  -- The type of event to construct.                                           *
 *            val   -- data value                                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(int index, unsigned char type, int val)
{
	DebugString("Adding event %s\n", EventNames[type]);
	if (index >= 0) {
		ID = index;
		Type = type;
		Data.General.Value = val;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;

	}
}


/***********************************************************************************************
 * EventClass::EventClass -- Constructor for navigation computer events.                       *
 *                                                                                             *
 *    Constructor for events that are used to assign the navigation computer.                  *
 *                                                                                             *
 * INPUT:   type     -- The type of event (this constructor can be used by other navigation    *
 *                      type events).                                                          *
 *                                                                                             *
 *          src      -- The object that the event should apply to.                             *
 *                                                                                             *
 *          dest     -- The destination (or target) that the event needs to complete.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(int index, unsigned char type, TargetClass src, TargetClass dest)
{
	DebugString("Adding event %s\n", EventNames[type]);

	if (index >= 0) {
		ID = index;
		Type = type;
		Data.NavCom.Whom = src;
		Data.NavCom.Where = dest;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/***********************************************************************************************
 * EventClass::EventClass -- Event for sequencing animations.                                  *
 *                                                                                             *
 *    This constructor is used for animations that must be created through the event system.   *
 *                                                                                             *
 * INPUT:   anim  -- The animation that will be created.                                       *
 *                                                                                             *
 *          coord -- The location where the animation is to be created.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(int index, AnimType anim, HousesType owner, Coord const & coord)
{
	DebugString("Adding event ANIMATION\n");

	if (index >= 0) {
		ID = index;
		Type = ANIMATION;
		Data.Anim.What = anim;
		Data.Anim.Owner = owner;
		Data.Anim.Where = coord;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/// <summary>
/// RA2
/// </summary>
EventClass::EventClass(int index, TargetClass src, MissionType mission, TargetClass target, TargetClass destination, TargetClass follow)
{
	if (index >= 0) {
		ID = index;
		Type = MEGAMISSION;
		Data.MegaMission.IsPlanningEvent = false;
		Data.MegaMission.Whom = src;
		Data.MegaMission.Mission = mission;
		Data.MegaMission.Target = target;
		Data.MegaMission.Destination = destination;
		Data.MegaMission.Follow = follow;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/***********************************************************************************************
 * EventClass::EventClass -- Megamission assigned to unit.                                     *
 *                                                                                             *
 *    This is the event that is used to assign most missions to units. It combines both the    *
 *    mission and the target (navcom and tarcom).                                              *
 *                                                                                             *
 * INPUT:   src      -- The object that this mission is to apply to.                           *
 *                                                                                             *
 *          mission  -- The mission to assign to this object.                                  *
 *                                                                                             *
 *          target   -- The target to assign to this object's TarCom.                          *
 *                                                                                             *
 *          destination -- The destination to assign to this object's NavCom.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/



/***********************************************************************************************
 * EventClass::EventClass -- Megamission assigned to unit.                                     *
 *                                                                                             *
 *    This is the event that is used to assign most missions to units. It combines both the    *
 *    mission and the target (navcom and tarcom).  This variation is used for formation moves. *
 *                                                                                             *
 * INPUT:   src      -- The object that this mission is to apply to.                           *
 *                                                                                             *
 *          mission  -- The mission to assign to this object.                                  *
 *                                                                                             *
 *          target   -- The target to assign to this object's TarCom.                          *
 *                                                                                             *
 *          destination -- The destination to assign to this object's NavCom.                  *
 *                                                                                             *
 *          speed    -- The formation override speed for this move.                            *
 *                                                                                             *
 *          maxspeed -- The formation override maximum speed for this move.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(int index, TargetClass src, MissionType mission, TargetClass target, TargetClass destination, SpeedType speed, MPHType maxspeed)
{
	DebugString("Adding event MEGAMISSION_F\n");

	if (index >= 0) {
		ID = index;
		Type = MEGAMISSION_F;
		Data.MegaMission_F.Whom = src;
		Data.MegaMission_F.Mission = mission;
		Data.MegaMission_F.Target = TargetClass(target);
		Data.MegaMission_F.Destination = TargetClass(destination);
		Data.MegaMission_F.Speed = speed;
		Data.MegaMission_F.MaxSpeed = maxspeed;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


EventClass::EventClass(int index, unsigned char type, RTTIType object, int id, BOOL naval)
{
	DebugString("Adding event %s\n", EventNames[type]);
	if (index >= 0) {
		ID = index;
		Type = type;
		Data.Specific.Type = object;
		Data.Specific.ID = id;
		Data.Specific.IsNaval = naval;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/***********************************************************************************************
 * EventClass::EventClass -- Constructor for sidebar build events.                             *
 *                                                                                             *
 *    This constructor is used for events that deal with an object type and an object ID.      *
 *    Typically, this is used exclusively by the sidebar.                                      *
 *                                                                                             *
 * INPUT:   type     -- The event type of this object.                                         *
 *                                                                                             *
 *          object   -- The object type number.                                                *
 *                                                                                             *
 *          id       -- The object sub-type number.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/



/***********************************************************************************************
 * EventClass::EventClass -- Constructor for object types affecting cells event.               *
 *                                                                                             *
 *    This constructor is used for those events that have an object type and associated cell.  *
 *    Typically, this is for building placement after construction has completed.              *
 *                                                                                             *
 * INPUT:   type     -- The event type for this object.                                        *
 *                                                                                             *
 *          object   -- The object type number (actual object is probably inferred from the    *
 *                      sidebar data).                                                         *
 *                                                                                             *
 *          cell     -- The cell location where this event is to occur.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
EventClass::EventClass(int index, unsigned char type, RTTIType object, Cell const & cell)
{
	DebugString("Adding event %s\n", EventNames[type]);

	if (index >= 0) {
		ID = index;
		Type = type;
		Data.Place.Type = object;
		Data.Place.ID = -1;
		Data.Place.IsNaval = 0;
		Data.Place.Where = cell;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


EventClass::EventClass(int index, unsigned char type, RTTIType object, int id, Cell const & cell)
{
	DebugString("Adding event %s\n", EventNames[type]);
	if (index >= 0) {
		ID = index;
		Type = type;
		Data.Place.Type = object;
		Data.Place.ID = id;
		Data.Place.IsNaval = 0;
		Data.Place.Where = cell;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


EventClass::EventClass(int index, unsigned char type, RTTIType object, int id, BOOL naval, Cell const & cell)
{
	DebugString("Adding event %s\n", EventNames[type]);
	if (index >= 0) {
		ID = index;
		Type = type;
		Data.Place.Type = object;
		Data.Place.ID = id;
		Data.Place.IsNaval = naval;
		Data.Place.Where = cell;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/***********************************************************************************************
 * EventClass::EventClass -- Construct an id and cell based event.                             *
 *                                                                                             *
 *    This constructor is used for those events that require an ID number and a cell location. *
 *                                                                                             *
 * INPUT:   type  -- The event type this will be.                                              *
 *                                                                                             *
 *          id    -- The arbitrary id number to assign.                                        *
 *                                                                                             *
 *          cell  -- The location for this event.                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/



/// <summary>
/// RA2
/// </summary>
EventClass::EventClass(int index, unsigned char type, int id, Cell const & cell)
{
	DebugString("Adding event %s\n", EventNames[type]);

	if (index >= 0) {
		ID = index;
		Type = type;
		Data.Special.ID 	= id;
		Data.Special.Where = cell;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/// <summary>
/// RA2
/// </summary>
EventClass::EventClass(int index, unsigned char type, RTTIType object, int id)
{
	DebugString("Adding event %s\n", EventNames[type]);

	if (index >= 0) {
		ID = index;
		Type = type;
		Data.Specific.Type = object;
		Data.Specific.ID = id;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


EventClass::EventClass(int index, IPXAddressClass & addr, signed char value)
{
	NetNumType net;
	NetNodeType node;

	if (index >= 0) {
		ID = index;
		Type = ADDRESS_CHANGE;
		addr.Get_Address(net, node);
		Data.Address.Node = *(unsigned int *)node;
		Data.Address.Value = value;
		Frame = ::Frame;
	} else {
		ID = -1;
		Type = EMPTY;
		Frame = ::Frame;
	}
}


/***********************************************************************************************
 * EventClass::Execute -- Execute a queued command.                                            *
 *                                                                                             *
 *    This routine executes an event. The even must already have been confirmed by any         *
 *    remote machine before calling this routine.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 264 bytes differ (adopted text)
void EventClass::Execute(void)
{
	TechnoClass * techno = NULL;
	BuildingClass * building = NULL;
	AnimClass * anim = NULL;
	HouseClass * house = Houses[ID];
	HouseClass * hptr = NULL;
	wchar_t const * str = NULL;
//	Cell cell;
//	Coord coord;
	wchar_t txt[80];
//	bool formation = false;
	int i;
	int index;
//	RTTIType rt;

	//if (Debug_Print_Events) {
	//	printf("(%d) Executing %s ID:%d Frame:%d ",
	//		::Frame, EventNames[Type], ID, Frame);
	//}

	switch (Type) {

		case PLAN_CONNECT:
		case PLAN_COMMIT:
		case PLAN_NODE_DELETE:
			Planning_Execute(this);
			return;

		case POWERON:
			building = Data.Target.Whom.As_Building();
			Planning_Clear_Techno(building, NULL);
			if (building && building->IsActive && !building->IsInLimbo && !building->Class->IsFirestormWall) {
				building->Turn_On();
			}
			break;

		case POWEROFF:
			building = Data.Target.Whom.As_Building();
			Planning_Clear_Techno(building, NULL);
			if (building && building->IsActive && !building->IsInLimbo && !building->Class->IsFirestormWall) {
				building->Turn_Off();
			}
			break;

		/*
		**	Update the archive target for this building.
		*/
		case ARCHIVE:
			techno = Data.NavCom.Whom.As_Techno();
			Planning_Clear_Techno(techno, NULL);
			if (techno && techno->IsActive) {
				techno->Assign_Archive_Target(Data.NavCom.Where.As_Abstract());
			}
			break;

		/*
		**	Make or break alliance.
		*/
		case ALLY:
			hptr = Houses[Data.General.Value];
			if (house->Is_Ally(hptr)) {
				house->Make_Enemy((HousesType)Data.General.Value, true);
			} else {
				house->Make_Ally((HousesType)Data.General.Value, true);
			}
			break;

		/*
		**	The player's infantry cheer.
		*/
		case ALL_CHEER: // RA2
			if (house != NULL) {
				house->Cheer();
			}
			break;

		/*
		**	Update the special control flags. This is necessary so that in a multiplay
		**	game, all machines will agree on the rules. If these options change during
		**	game play, then all players are informed that options have changed.
		*/
		case SPECIAL:
			if (house != NULL) {
				Special = Data.Options.Data;
				Scen->Special = Data.Options.Data;

				swprintf(txt, StringTable::LoadString("TXT_SPECIAL_WARNING", NULL, "D:\\Ra2\\Event.CPP", 950), house->UIName);
				Session.Messages.Add_Message(NULL, 0, txt, house->Scheme, TextPrintType(0x4046), 1200, false);
				Map.Flag_To_Redraw(GScreenRedrawFlags(0));
			}
			break;

		/*
		**	Starts or stops repair on the specified object. This event is triggered by the
		**	player clicking the repair wrench on a building.
		*/
		case REPAIR:
			techno = Data.Target.Whom.As_Techno();
			Planning_Clear_Techno(techno, NULL);
			if (techno && techno->IsActive) {
				techno->Repair(-1);
			}
			break;

		/*
		**	Tells a building/unit to sell. This event is triggered by the player clicking the
		**	sell animating cursor over the building or unit.
		*/
		case SELL:
			techno = Data.Target.Whom.As_Techno();
			Planning_Clear_Techno(techno, NULL);
			if (techno && techno->IsActive && techno->House->HeapID == ID) {
				if (techno->RTTI == RTTI_BUILDING || ((techno->RTTI == RTTI_UNIT || techno->RTTI == RTTI_AIRCRAFT) && Map[techno->Center_Coord()].Cell_Building() != NULL)) {
					techno->Sell_Back(-1);
				}
			}
			break;

		/*
		**	Tells the wall at the specified location to sell off.
		*/
		case SELLCELL:
//			cell = Data.SellCell.Cell;
			house->Sell_Wall(Cell(Data.SellCell.Where.X, Data.SellCell.Where.Y), false);
			break;

		/*
		**	This even is used to trigger an animation that is generated as a direct
		**	result of player intervention.
		*/
		case ANIMATION:
		{
			Coord coord(Data.Anim.Where.X, Data.Anim.Where.Y);
			coord.Z = Map.Get_Height_GL(coord);
			if (Map[coord].IsUnderBridge) {
				coord.Z += BRIDGE_LEPTON_HEIGHT;
			}
			if (Data.Anim.What == ANIM_NONE) {
				anim = new AnimClass(Rule->MoveFlash, coord, 0, 1, ShapeFlags_Type(0x600), 0);
			} else {
				anim = new AnimClass(AnimTypes[Data.Anim.What], coord, 0, 1, ShapeFlags_Type(0x600), 0);
			}
			if (anim) {
				if (Data.Anim.Owner != HOUSE_NONE && !Houses[Data.Anim.Owner]->Is_Player_Control()) {
					anim->Make_Invisible();
				}
			}
			break;
		}

		/*
		**	This event will place the specified object at the specified location.
		**	The event is used to place newly constructed buildings down on the map. The
		**	object type is specified. From this object type, the house can determine the
		**	exact factory and real object pointer to use.
		*/
		case PLACE:
			house->Place_Object(Data.Place.Type, Data.Place.ID, Data.Place.IsNaval != 0, Cell(Data.Place.Where.X, Data.Place.Where.Y));
			break;

		/*
		**	This event starts production of the specified object type. The house can
		**	determine from the type and ID value, what object to begin production on and
		**	what factory to use.
		*/
		case PRODUCE:
			house->Begin_Production(Data.Specific.Type, Data.Specific.ID, Data.Specific.IsNaval != 0, false);
			break;

		/*
		**	This event is generated when the player puts production on hold. From the
		**	object type, the factory can be inferred.
		*/
		case SUSPEND:
			house->Suspend_Production(Data.Specific.Type, Data.Specific.ID, Data.Specific.IsNaval != 0);
			break;

		/*
		**	This event is generated when the player cancels production of the specified
		**	object type. From the object type, the exact factory can be inferred.
		*/
		case ABANDON:
			house->Abandon_Production(Data.Specific.Type, Data.Specific.ID, Data.Specific.IsNaval != 0);
			break;

		/*
		**	Toggles the primary factory state of the specified building.
		*/
		case PRIMARY:
			{
				BuildingClass * building = Data.Target.Whom.As_Building();
				Planning_Clear_Techno(building, NULL);
				if (building && building->IsActive) {
					building->Toggle_Primary();
				}
			}
			break;

		/*
		**	This is the general purpose mission control event. Most player
		**	action routes through this event. It sets a unit's mission, TarCom,
		**	and NavCom to the values specified.
		*/
		case MEGAMISSION_F:
			// Fall thru to next case...

		case MEGAMISSION:
			techno = Data.MegaMission.Whom.As_Techno();
			if (Data.MegaMission.IsPlanningEvent == 1) {
				Planning_Execute(this);
				break;
			}
			Planning_Clear_Techno(techno, this);
			if (techno != NULL && techno->IsActive && techno->Strength > 0 && !techno->IsInLimbo) {

				/*
				**	Fetch a pointer to the object of the mission. If there is an error with
				**	this object, such as it is dead, then bail.
				*/
				ObjectClass * object = NULL;
				if (Data.MegaMission.Target.Is_Valid()) {
					object = Data.MegaMission.Target.As_Object();
					if (object != NULL && (!object->IsActive || object->Strength == 0 || object->IsInLimbo)) {
						break;
					}
				}

				/*
				**	If the destination target is invalid because the object is dead, then
				**	bail from processing this mega mission.
				*/
				if (Data.MegaMission.Destination.Is_Valid()) {
					object = Data.MegaMission.Destination.As_Object();
					if (object != NULL && (!object->IsActive || object->Strength == 0 || object->IsInLimbo)) {
						break;
					}
				}

				if (Data.MegaMission.Mission == MISSION_ENTER && techno->CurrentMission == MISSION_ENTER && techno->Is_Foot()) {
					AbstractClass * target = Data.MegaMission.Destination.As_Abstract();
					if (target != NULL && target->RTTI == RTTI_BUILDING && techno->Radio_Link(0) == target) {
						break;
					}
				}

				/*
				**	Break any existing tether or team contact, since it is now invalid.
				*/
				if (!techno->IsTethered) {
					techno->Transmit_Message(RADIO_OVER_OUT);
				} else {
					TechnoClass * radio = techno->Radio_Link(0);
					if (radio != NULL && radio->IsActive && radio->RTTI == RTTI_BUILDING && ((BuildingClass *)radio)->Class->IsDockUnload) {
						techno->Transmit_Message(RADIO_OVER_OUT);
						techno->IsTethered = false;
					}
				}
				if (techno->QueueUpToEnter != NULL) {
					techno->QueueUpToEnter = NULL;
				}
				if (techno->Is_Foot()) {
					if (((FootClass *)techno)->Team && Data.MegaMission.Mission != MISSION_UNLOAD) {
						((FootClass *)techno)->Team->Remove((FootClass *)techno, -1, true);
					}
				}

				if (object != NULL) {
					if (PlayerPtr->Is_Ally(techno)) {
						object->Clicked_As_Target(7);
					}
				}

				/*
				**	Test to see if the navigation target should really be queued rather
				**	than assigned to the object.
				*/
				int mission = techno->RespondMegaEventMission(this);
				techno->QueueMission((MissionType)mission, false);

				FootClass * foot;
				if (techno->AbstractFlags & 4) {
					foot = (FootClass *)techno;
				} else {
					foot = NULL;
				}
				if (foot != NULL) {
					foot->SuspendedNavCom = NULL;
				}
				techno->SuspendedTarCom = NULL;

				/*
				**	Guard area mode is handled with care. The specified target is actually
				**	assigned as the location that should be guarded. In addition, the
				**	movement destination is immediately set to this new location.
				*/
				if (Data.MegaMission.Mission == MISSION_GUARD_AREA && techno->Is_Foot()) {
					techno->Assign_Target(NULL);
					techno->Assign_Destination(Data.MegaMission.Target.As_Abstract(), true);
					techno->Assign_Archive_Target(Data.MegaMission.Target.As_Abstract());
				} else {
					if (foot != NULL) {
						foot->Assign_Archive_Target(NULL);
						foot->Clear_Navigation_List();
					}
					techno->Assign_Target(Data.MegaMission.Target.As_Abstract());
					techno->Assign_Destination(Data.MegaMission.Destination.As_Abstract(), true);
					if (Data.MegaMission.Follow.Is_Valid()) {
						techno->Set_Follow_Target(Data.MegaMission.Follow.As_Abstract());
					}
				}
			}
			break;

		/*
		**	Request that the unit/infantry/aircraft go into idle mode.
		*/
		case IDLE:
			techno = Data.Target.Whom.As_Techno();
			Planning_Clear_Techno(techno, NULL);
			if (techno != NULL && techno->IsActive && !techno->IsInLimbo && !techno->IsTethered && techno->CurrentMission != MISSION_CONSTRUCTION && techno->CurrentMission != MISSION_DECONSTRUCTION) {

				if (!techno->IsOnBridge && Map[Coord(techno->Position)].Ramp == RAMP_NONE && techno->Is_On_Elevation()) {
					break;
				}

				if (techno->Is_Foot()) {
					((FootClass *)techno)->NavQueue.Clear();
					((FootClass *)techno)->Clear_Navigation_List();
					((FootClass *)techno)->CurrentPath = PATH_NONE;
					((FootClass *)techno)->NextWaypoint = 0;
					((FootClass *)techno)->WaypointOffsetCell = Cell(0, 0);
					((FootClass *)techno)->WaypointTargetCell = Cell(0, 0);
				}
				techno->SendToEachLink(RADIO_OVER_OUT);
				techno->Assign_Destination(NULL, true);
				techno->Assign_Target(NULL);
				if (techno->SpawnManager) {
					techno->SpawnManager->ResetTarget();
				}
				if (techno->RTTI == RTTI_UNIT && ((UnitClass *)techno)->Class->IsToHarvest && (techno->CurrentMission == MISSION_HARVEST || techno->CurrentMission == MISSION_RETURN)) {
					techno->QueueMission(MISSION_GUARD, false);
					techno->Commence();
				}
			}
			break;

		case DEPLOY:
			techno = Data.Target.Whom.As_Techno();
			Planning_Clear_Techno(techno, NULL);
			if (techno != NULL && techno->IsActive && !techno->IsInLimbo && !techno->IsTethered && techno->StunDuration == 0) {

				if (!techno->IsOnBridge) {
					if (!Map[Coord(techno->Position)].Ramp != 0 && techno->Is_On_Elevation()) {
						break;
					}
				}

				if (techno->CurrentMission != MISSION_CONSTRUCTION && techno->CurrentMission != MISSION_DECONSTRUCTION && techno->RTTI != RTTI_AIRCRAFT) {
					Cell cell = techno->Get_Cell();
					if (cell != CELL_NONE) {
						building = Map[cell].Cell_Building();
						if (building && building->Class->IsWeaponsFactory) {
							break;
						}
					}
					techno->Transmit_Message(RADIO_OVER_OUT);
					techno->Assign_Destination(NULL, true);
					techno->Assign_Target(NULL);
					techno->QueueMission(MISSION_UNLOAD, false);
				}
			}
			break;

		/*
		**	Detonate the bomb attached to the object. RA2
		*/
		case DETONATE:
			techno = Data.Target.Whom.As_Techno();
			Planning_Clear_Techno(techno, NULL);
			if (techno && techno->AttachedBomb) {
				techno->AttachedBomb->Detonate();
			}
			break;

		/*
		**	Request that the unit/infantry/aircraft scatter from its current location.
		*/
		case SCATTER:
			techno = Data.Target.Whom.As_Techno();
			Planning_Clear_Techno(techno, NULL);
			if (techno != NULL && techno->Is_Foot() && techno->IsActive && !techno->IsInLimbo && !techno->IsTethered) {
				((FootClass *)techno)->IsScattering = true;
				techno->Scatter(COORD_NONE, true, false);
			}
			break;

		/*
		**	If we are placing down the ion cannon blast then lets take
		**	care of it.
		*/
		case SPECIAL_PLACE:
			house->Place_Special_Blast((SuperWeaponType)Data.Special.ID, Cell(Data.Special.Where.X, Data.Special.Where.Y));
			break;

		/*
		**	Exit the game.
		**	Give parting message while palette is fading to black.
		*/
		case EXIT:
			DebugString("Processing EXIT event on frame %d\n", ::Frame);
			PlayerAborts = true;
			break;

		/*
		**	Process the options menu, unless we're playing back a recording.
		*/
		case OPTIONS:
			if (!Session.Play) {
				SpecialDialog = SDLG_OPTIONS;
			}
			break;

		/*
		**	Process the options Game Speed
		*/
		case GAMESPEED:
			Options.GameSpeed = Data.General.Value;

			house = Houses[ID];
			if (house != PlayerPtr && house != NULL) {
				str = StringTable::LoadString("TXT_PLAYER_CHANGED_SPEED", NULL, "D:\\Ra2\\Event.CPP", 1401);
				if (str != NULL && wcslen(str) != 0) {
					wchar_t buffer[256];
					swprintf(buffer, str, house->UIName);
					Session.Messages.Add_Message(NULL, 0, buffer, house->Scheme, TextPrintType(0x4046), int(Rule->MessageDelay * TICKS_PER_MINUTE), false);
				}
			}
			break;

		/*
		**	Adjust connection timing for multiplayer games
		*/
		case RESPONSE_TIME:
			Session.MaxAhead = Data.FrameInfo.Delay;
			break;

		/*
		**	Save a multiplayer game (this event is only generated in multiplayer mode)
		*/
		case SAVEGAME:
			/*
			**	Show the user what's going on with a message box (but only if
			**	we're not already inside a dialog box routine!)
			*/
			if (SpecialDialog == SDLG_NONE) {
				CDTimerClass<SystemTimerClass> timer;
				timer = TICKS_PER_SECOND * 4;

				WWMessageBox().Process(StringTable::LoadString("TXT_SAVING_GAME", NULL, "D:\\Ra2\\Event.CPP", 1434), StringTable::LoadString("TXT_NONE", NULL, "D:\\Ra2\\Event.CPP", 1434), NULL, NULL, false);
				Save_Game("SAVEGAME.NET", StringTable::LoadString("TXT_MULTIPLAYER_GAME", NULL, "D:\\Ra2\\Event.CPP", 1436), false);

				while (timer > 0) {
					Call_Back();
				}

				HiddenSurface->Fill(0);
				Map.Flag_To_Redraw(GS_REDRAW_ALL);
				Map.Render();
			}
			else {
				Save_Game("SAVEGAME.NET", StringTable::LoadString("TXT_MULTIPLAYER_GAME", NULL, "D:\\Ra2\\Event.CPP", 1447), false);
			}
			break;

		/*
		**	Add a new player to the game:
		**	- Form a network connection to him
		**	- Add his name, ID, House etc to our list of players
		**	- Re-sort the ID array
		**	- Place his units on the map
		*/
		case ADDPLAYER:
			if (ID != PlayerPtr->HeapID) {
				delete [] Data.Variable.Pointer;
			}
			break;

		case REMOVEPLAYER:
			DebugString("Executing REMOVEPLAYER event. Frame is %d\n", ::Frame);
			index = Data.General.Value;

			house = Houses[index];
			if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) {
				if (house->Is_Human_Player()) {
					OwnerTalk.Send_Remove_Player(index);
				}
			}
			if ((Session.Type == GAME_INTERNET && WestwoodOnline_Tournament) || house->Is_Human_Player()) {
				house->Flag_To_Die();
			}
			break;

		/*
		**	Special self destruct action requested. This is active in the multiplayer mode.
		*/
		case DESTRUCT:
			house->Flag_To_Die();
			break;

		case FALLBACK_HOST: // RA2
			DebugString("Received FALLBACKHOST event\n");
			Session.LastTimingID = Data.General.Value;
			break;

		case ADDRESS_CHANGE: // RA2
			{
				DebugString("Executing ADDRESSCHANGE event for player %d\n", Data.Address.Value);
				unsigned char node[6];
				unsigned char network[4];
				memset(node, 0, sizeof(node));
				memset(network, 0, sizeof(network));
				*(long *)node = Data.Address.Node;
				IPXAddressClass address(network, node, 0);
				DebugString("New address is %s\n", address.As_String(&address));
				Ipx.Set_Connection_Address(Data.Address.Value, &address);
				OwnerTalk.Set_Address(Data.Address.Value, &address);
				int count = Session.Players.Count();
				for (i = 0; i < count; i++) {
					if (Session.Players[i]->Player.ID == Data.Address.Value) {
						break;
					}
				}
				if (i < count) {
					Session.Players[i]->Address = address;
				}
			}
			break;

		case ABOUT_TO_EXIT: // RA2
			{
				DebugString("Processing ABOUTTOEXIT event on frame %d\n", ::Frame);
				if (Session.Am_I_Master(NULL)) {
					OutList.Add(EventClass(PlayerPtr->HeapID, EXIT));
					Session.RouterProbeActive = true;
				}
				Session.AboutToExitActive = true;
				int host = OwnerTalk.Update_Master();
				if (host != -1) {
					HouseClass * hostp = Houses[host];
					if (hostp) {
						if (Session.Players.Count() > 2) {
							IPXAddressClass * address = NULL;
							for (i = 0; i < Session.Players.Count(); i++) {
								if (Session.Players[i]->Player.ID == host) {
									address = &Session.Players[i]->Address;
									Session.PacketRouterPort = Session.PlayerPorts[i];
									break;
								}
							}
							unsigned char network[4];
							unsigned char node[6];
							address->Get_Address(network, node);
							Session.PacketRouterAddress.Set_Address(network, node, 0);
							DebugString("Session.PacketRouterAddress = %s\n", Session.PacketRouterAddress.As_String(NULL));
							DebugString("Session.PacketRouterPort = %d\n", Session.PacketRouterPort);
							if (!Session.Am_I_Master(NULL)) {
								Ipx.Set_Lobby_Address(&Session.PacketRouterAddress);
							}
						}
						wcscpy(Session.MasterPlayerName, hostp->UIName);
						DebugString("New game host is %ls\n", Session.MasterPlayerName);
						Session.MasterPlayerID = host;
						if (Session.Am_I_Master(NULL)) {
							OwnerTalk.Send_Player_List();
						}
					}
					Session.ExitEventPending = ::Frame;
					Session.ExitEventPlayer = host;
					Session.ExitEventFrame = ((Session.MaxAhead + ::Frame + 10) / 10) * 10;
					Session.LastTimingID = -1;
				}
				Session.AboutToExitActive = false;
			}
			break;

		case LATENCYFUDGE:
			DebugString("Executing LATENCYFUDGE event. Frame is %d\n", ::Frame);
			Session.LatencyFudge = Data.General.Value;
			DebugString("LatencyFudge is %d\n", Session.LatencyFudge);

			house = Houses[ID];
			if (house != PlayerPtr && house != NULL) {
				str = StringTable::LoadString("TXT_PLAYER_CHANGED_LATENCY", NULL, "D:\\Ra2\\Event.CPP", 1718);
				if (str != NULL && wcslen(str) != 0) {
					wchar_t buffer[256];
					swprintf(buffer, str, house->UIName);
					Session.Messages.Add_Message(NULL, 0, buffer, house->Scheme, TextPrintType(0x4046), int(Rule->MessageDelay * TICKS_PER_MINUTE), false);
				}
			}
			break;

		//
		// This event tells all systems to use new timing values.  It's like
		// RESPONSE_TIME, only it works.  It's only used with the
		// COMM_MULTI_E_COMP protocol.
		//
		case TIMING:
			Data.Timing.MaxAhead -= (Scen->Special.IsFogOfWar) ? 10 : 0;

			if (Data.Timing.MaxAhead > Session.MaxAhead || Data.Timing.FrameSendRate > Session.FrameSendRate) {
				NewMaxAheadFrame1 = Frame;
				NewMaxAheadFrame2 = Data.Timing.FrameSendRate * ((Frame + Data.Timing.MaxAhead + Data.Timing.FrameSendRate - 1) / Data.Timing.FrameSendRate);
			} else {
				NewMaxAheadFrame1 = 0;
				NewMaxAheadFrame2 = 0;
			}

			Session.DesiredFrameRate = Data.Timing.DesiredFrameRate;
			Session.MaxAhead = Data.Timing.MaxAhead;

			if ((unsigned int)Session.MaxMaxAhead <= Session.MaxAhead) {
				Session.MaxMaxAhead = Session.MaxAhead;
			}

			Session.FrameSendRate = Data.Timing.FrameSendRate;

			break;

		//
		// This event tells all systems what the other systems' process
		// timing requirements are; it's used to compute a desired frame rate
		// for the game.
		//
		case PROCESS_TIME:
			for (i = 0; i < Session.Players.Count(); i++) {
				if (ID == Session.Players[i]->Player.ID) {
					Session.Players[i]->Player.ProcessTime = Data.ProcessTime.AverageTicks;
					break;
				}
			}
			break;

		case PAGEUSER:
			if (!Session.Play) {
				SpecialDialog = SDLG_WOL_OPTIONS;
			}
			break;

		/*
		**	Default: do nothing.
		*/
		default:
			break;
	}

	//if (Debug_Print_Events) {
	//	printf("\n");
	//}

}
#endif


