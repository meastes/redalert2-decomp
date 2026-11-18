/*
**	Command & Conquer Red Alert 2
**	UNHOLYALLIANCE.H: the Unholy Alliance multiplayer game mode (RTTI .?AVUnholyAlliance@@, vtable
**	0x007A6C84).
**
**	Lane R9: declared from the vtable (45 slots; its own AssignTeams, CheckStartConditions,
**	AssignTeamsAtStart and SpawnBaseUnits, the rest MultiplayerGameMode's) and the constructor at
**	0x005AC330, which passes its six arguments to MultiplayerGameMode's constructor unchanged.
*/
#pragma once

#include "multiplayer.h"

class UnholyAlliance : public MultiplayerGameMode
{
	public:
		UnholyAlliance(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex);
		virtual ~UnholyAlliance(void);
		virtual bool AssignTeams(void) override;
		virtual bool CheckStartConditions(WideWstring * message) override;
		virtual bool AssignTeamsAtStart(void) override;
		virtual bool SpawnBaseUnits(HouseClass * house, unsigned unused) override;
};
