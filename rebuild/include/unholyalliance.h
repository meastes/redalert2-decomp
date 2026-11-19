/*
**	Command & Conquer Red Alert 2
**	UNHOLYALLIANCE.H: the Unholy Alliance multiplayer game mode (RTTI .?AVUnholyAlliance@@, vtable
**	0x007A6C84).
**
**	AssignTeams, CheckStartConditions, AssignTeamsAtStart and SpawnBaseUnits are its own; the
**	other slots are MultiplayerGameMode's. The constructor (0x005AC330) passes its six arguments
**	to MultiplayerGameMode's unchanged.
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
