/*
**	Command & Conquer Red Alert 2
**	MPUNHOLYALLIANCE.CPP: the Unholy Alliance game mode.
**
**	Every player starts with a base, and with both of the rules' base units: the first stands
**	three cells down and right of the base centre, the second (and the first, when that cell is
**	taken) goes to the nearest free place.
*/

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"
#include "unholyalliance.h"
#include "session.h"
#include "house.h"
#include "_rules.h"
#include "rules.h"
#include "unit.h"
#include "scenario.h"


UnholyAlliance::UnholyAlliance(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex) : // RA2: 0x005AC330
	MultiplayerGameMode(title, tooltip, inifile, mapfilter, aiallowed, modeindex)
{
}


UnholyAlliance::~UnholyAlliance(void) // RA2: 0x005AC370
{
}


bool UnholyAlliance::AssignTeams(void) // RA2: 0x005AC380, bases are always on
{
	Session.Options.Bases = true;
	return(true);
}


bool UnholyAlliance::CheckStartConditions(WideWstring * message) // RA2: 0x005AC390
{
	return(Session.Options.Bases && MultiplayerGameMode::CheckStartConditions(message));
}


bool UnholyAlliance::AssignTeamsAtStart(void) // RA2: 0x005AC3C0
{
	Session.Options.Bases = true;
	return(true);
}


/// <summary>
/// RA2: places both base units of the rules for the house.
/// </summary>
/// <returns>Returns false when one of them is missing or could not be placed.</returns>
bool UnholyAlliance::SpawnBaseUnits(HouseClass * house, unsigned unused) // RA2: 0x005AC3D0
{
	bool failed = false;

	for (int index = 0; index < 2; index++) {
		UnitTypeClass const * type = Rule->BaseUnit[index];
		if (type == NULL) {
			failed = true;
		} else {
			UnitClass * unit = new UnitClass(type, house);
			if (unit == NULL) {
				failed = true;
			} else {
				Cell center = Cell(house->Center);
				Cell cell(center.X + 3, center.Y + 3);
				if (index != 0 || !unit->Unlimbo(Coord(cell, 0))) {
					if (!Scan_Place_Object(unit, Cell(house->Center), 3)) {
						failed = true;
						delete unit;
					}
				}
			}
		}
	}
	return(!failed);
}
