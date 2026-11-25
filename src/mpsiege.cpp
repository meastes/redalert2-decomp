/*
**	Command & Conquer Red Alert 2
**	MPSIEGE.CPP: the Siege game mode.
**
**	One player defends (team 1) and starts with a construction yard on the first start
**	waypoint; every other player attacks (team 2) from the waypoints the houses are given
**	after it.
*/

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"
#include "mpsiege.h"
#include "session.h"
#include "house.h"
#include "houstype.h"
#include "building.h"
#include "builtype.h"
#include "_rules.h"
#include "rules.h"
#include "scenario.h"
#include "dbgprint.h"
#include <windows.h>
#undef LoadString
#include "stringtable.h"


MultiplayerSiege::MultiplayerSiege(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex) : // RA2: 0x005AB5A0
	MultiplayerGameMode(title, tooltip, inifile, mapfilter, aiallowed, modeindex)
{
}


MultiplayerSiege::~MultiplayerSiege(void) // RA2: 0x005AB5E0
{
}


/// <summary>
/// RA2: the first player defends, the others attack.
/// </summary>
bool MultiplayerSiege::AssignTeams(void) // RA2: 0x005AB5F0
{
	if (!MultiplayerGameMode::AssignTeams()) {
		return(false);
	}
	if (Session.Players.Count() > 0) {
		Session.Players[0]->Slot.SpectatorFlag = 1;
		for (int i = 1; i < Session.Players.Count(); i++) {
			Session.Players[1]->Slot.SpectatorFlag = 2;
		}
	}
	return(true);
}


/// <summary>
/// RA2: the game may start with exactly one defender and at least one attacker.
/// </summary>
bool MultiplayerSiege::CheckStartConditions(WideWstring * message) // RA2: 0x005AB640
{
	if (!MultiplayerGameMode::CheckStartConditions(message)) {
		return(false);
	}

	bool defender = false;
	int attackers = 0;
	for (int i = 0; i < Session.Players.Count(); i++) {
		switch (Session.Players[i]->Slot.SpectatorFlag) {
			case 0:
				break;

			case 1:
				if (defender) {
					*message = StringTable::LoadString("MP:OnlyOneBeseiged", NULL, "D:\\Ra2\\MPSiege.cpp", 51);
					return(false);
				}
				defender = true;
				break;

			case 2:
				attackers++;
				break;

			default:
				*message = StringTable::LoadString("MP:IllegalTeam", NULL, "D:\\Ra2\\MPSiege.cpp", 60);
				return(false);
		}
	}

	if (!defender) {
		*message = StringTable::LoadString("MP:NoDefender", NULL, "D:\\Ra2\\MPSiege.cpp", 65);
		return(false);
	}

	if (attackers < 1) {
		*message = StringTable::LoadString("MP:NoAttackers", NULL, "D:\\Ra2\\MPSiege.cpp", 70);
		return(false);
	}

	return(true);
}


bool MultiplayerSiege::AssignTeamsAtStart(void) // RA2: 0x005AB740
{
	return(MultiplayerGameMode::AssignTeamsAtStart());
}


bool MultiplayerSiege::IsAIAllowed(void) // RA2: 0x005AB750
{
	return(false);
}


bool MultiplayerSiege::UnfixAlliances(void) // RA2: 0x005AB760
{
	return(MultiplayerGameMode::UnfixAlliances());
}


/// <summary>
/// RA2: the defender's house gets the first start waypoint, the other houses are placed as
/// usual on the rest.
/// </summary>
/// <returns>Returns false when there is no defender or its house cannot be found.</returns>
bool MultiplayerSiege::StartingPositionsToHouseBaseCells(char unused) // RA2: 0x005AB770
{
	DynamicVectorClass<Cell> cells = Build_Start_Waypoint_List(unused);
	unsigned char taken[16];
	memset(taken, 0, sizeof(taken));

	int index;
	for (index = 0; index < Session.Players.Count(); index++) {
		if (Session.Players[index]->Slot.SpectatorFlag == 1) {
			break;
		}
	}
	if (index >= Session.Players.Count()) {
		return(false);
	}

	int id = Session.Players[index]->Player.ID;
	if (id < 0 || id >= Houses.Count()) {
		return(false);
	}

	HouseClass * defender = Houses[id];
	if (defender->Class->IsMultiplayPassive) {
		return(false);
	}

	DebugString("Assigning Starting Points for Defending house %d (%s)\n", id, defender->Class->Name());
	Cell first = cells[0];
	taken[0] = true;
	defender->Center = Coord(first);

	for (int i = 0; i < Houses.Count(); i++) {
		HouseClass * house = Houses[i];
		if (!house->Class->IsMultiplayPassive && house != defender) {
			DebugString("Assigning Starting Points for Attacking house %d (%s)\n", i, house->Class->Name());
			Cell cell;
			Cell chosen = *AssignStartingPositionsToHouse(&cell, i, &cells, taken);
			house->Center = Coord(chosen);
		}
	}
	return(true);
}


void MultiplayerSiege::CreateMPTeams(DynamicVectorClass<MultiplayerTeam *> * teams) // RA2: 0x005AB970
{
	teams->Add(new MultiplayerObserverTeam);
	teams->Add(new MultiplayerSiegeDefenderTeam);
	teams->Add(new MultiplayerSiegeAttackerTeam);
}


/// <summary>
/// RA2: the defender starts with a construction yard on its base center (or as near as it
/// fits); the other houses get the usual base unit.
/// </summary>
bool MultiplayerSiege::SpawnBaseUnits(HouseClass * house, unsigned unused) // RA2: 0x005ABA80
{
	int index;
	for (index = 0; index < Session.Players.Count(); index++) {
		if (Session.Players[index]->Player.ID == house->HeapID) {
			break;
		}
	}
	if (index >= Session.Players.Count()) {
		MultiplayerGameMode::SpawnBaseUnits(house, unused);
	} else if (Session.Players[index]->Slot.SpectatorFlag != 1) {
		return(MultiplayerGameMode::SpawnBaseUnits(house, unused));
	} else {
		BuildingTypeClass const * type = house->Get_First_Ownable(Rule->BuildConst);
		if (type != NULL) {
			BuildingClass * building = new BuildingClass(type, house);
			if (building != NULL) {
				if (!building->Unlimbo(house->Center, DIR_N)) {
					if (!Scan_Place_Object(building, Cell(house->Center), 1)) {
						delete building;
						return(false);
					}
				}
				return(true);
			}
		}
	}
	return(false);
}
