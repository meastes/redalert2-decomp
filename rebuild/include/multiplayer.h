// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Changed to RA2 1.006's layout by RedAlert2 contributors, 2026.
#pragma once

#include "always.h"
#include "vector.h"
#include "widewstring.h"
#include "wstring.h"
#include "coord.h"

#include <windows.h>

class CCINIClass;
class NoInitClass;
class HouseClass;
class MultiMission;
struct MPlayerScoreType;

class MultiplayerTeam
{
	public:
		MultiplayerTeam(WideWstring const & title, int index);
		virtual ~MultiplayerTeam(void);
		virtual bool IsTeamIncluded(int index);
		virtual bool SetPlayerTeam(int player);
		int CountMembers(void);
		int AddToList(HWND list);

		WideWstring Title;
		int Index;
};

class MultiplayerGameMode
{
	public:
		class InitializerBase
		{
			public:
				InitializerBase(void);
				virtual ~InitializerBase(void);
				virtual MultiplayerGameMode * Create(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex) = 0; // RA2: named Create by the instances at 0x005B9060..0x005B91A0 (was Allocate)
		};

		template<class T> class Initializer : public InitializerBase // RA2: one for each game mode class; Init_Game_Modes (0x005B8BD0) makes one of each, their Create are the functions at 0x005B9060..0x005B91A0
		{
			public:
				virtual MultiplayerGameMode * Create(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex)
				{
					return(new T(title, tooltip, inifile, mapfilter, aiallowed, modeindex));
				}
		};

		static MultiplayerGameMode * __fastcall Get(int index);
		static int __cdecl GetModeCount(void); // RA2: the number of registered game modes, at 0x005B6F80
		CCINIClass * GetINI(void);
		bool IsMapAllowed(MultiMission const * map);
		MultiplayerTeam * GetTeam(int index);
		void DeleteTeams(void);
		bool GenerateUnits(void); // RA2: 0x005B7CE0, gives every house its base unit and starting units
		int TeamOfHouse(int house); // RA2: 0x005B8430, the team of the player that has the house, -1 when none does

		MultiplayerGameMode(NoInitClass const &) {}
		MultiplayerGameMode(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex);
		virtual ~MultiplayerGameMode(void);
		virtual bool vt_entry_04(void) { return(false); }
		virtual bool vt_entry_08(void) { return(false); }
		virtual bool vt_entry_0C(unsigned a) { return(true); }
		virtual bool AssignTeams(void);
		virtual bool CheckStartConditions(WideWstring * message);
		virtual bool AssignTeamsAtStart(void);
		virtual bool IsCountryAllowed(int country);
		virtual void vt_entry_20(void) {}
		virtual void vt_entry_24(void) {}
		virtual int DefaultCountry(void);
		virtual bool IsAIAllowed(void);
		virtual bool IsSkirmishAllowed(void); // RA2: Fill_Game_Mode_List lists only the modes returning true in a skirmish; FreeForAll does
		virtual int FirstValidMapIndex(void);
		virtual void PopulateTeamDropdown(HWND hwnd, DynamicVectorClass<MultiplayerTeam *> * teams, MultiplayerTeam * team);
		virtual void DrawTeamDropdown(HWND hwnd, DynamicVectorClass<MultiMission const *> * maps, MultiMission const * map);
		virtual void PopulateTeamDropdownForPlayer(HWND hwnd, int index);
		virtual bool vt_entry_44(int a, DynamicVectorClass<int> picks, int b) { return(false); }
		virtual bool vt_entry_48(int a, DynamicVectorClass<int> picks, char const * map, char const * name) { return(false); } // RA2: four arguments (Net2DecodeGameopt pushes the index, the list by value and two names)
		virtual bool vt_entry_4C(unsigned a, unsigned b, struct MPlayerScoreType * scores) { return(true); } // RA2: the third argument points to the two score records Net2DecodeGameopt filled
		virtual bool vt_entry_50(void);
		virtual bool vt_entry_54(void);
		virtual bool vt_entry_58(void) { return(true); }
		virtual int RandomHumanCountryIndex(void);
		virtual int RandomAICountryIndex(void);
		virtual void vt_entry_64(unsigned a, char * text) {} // RA2: Net2DecodeGameopt passes the digit after C and the rest of the text; MPCooperative splits it with strtok, so it is not const
		virtual bool UnfixAlliances(void);
		virtual bool StartingPositionsToHouseBaseCells(char unused);
		virtual bool vt_entry_70(void);
		virtual bool OnGameStart(void);
		virtual void vt_entry_78(void) {}
		virtual void vt_entry_7C(void) {}
		virtual int vt_entry_80(void) { return(-1); }
		virtual void vt_entry_84(void) {}
		virtual void vt_entry_88(bool won) {} // RA2: slot 34; takes whether the player won (Process_Game_End pushes the flag as it is)
		virtual void vt_entry_8C(bool a) {} // RA2: a bool (MPCooperative::vt_entry_8C tests a byte)
		virtual bool vt_entry_90(void) { return(false); }
		virtual void vt_entry_94(void) {}
		virtual void vt_entry_98(void) {}
		virtual bool vt_entry_9C(DynamicVectorClass<int> list, int * result) { return(false); } // RA2: the list and a place for the result (MPCooperative::vt_entry_9C ends with ret 28)
		virtual bool vt_entry_A0(void) { return(true); }
		virtual void CreateMPTeams(DynamicVectorClass<MultiplayerTeam *> * teams);
		virtual Cell * AssignStartingPositionsToHouse(Cell * result, int house, DynamicVectorClass<Cell> * coords, unsigned char * satisfied);
		virtual bool SpawnBaseUnits(HouseClass * house, unsigned unused);
		virtual bool GenerateStartingUnits(HouseClass * house, int & amount);

		bool unknown_4;
		DynamicVectorClass<MultiplayerTeam *> MPTeams;
		WideWstring UIName;
		WideWstring CSFTooltip;
		int MPModeIndex;
		Wstring INIFilename;
		Wstring MapFilter;
		bool AIAllowed;
		CCINIClass * INI;
		int wolTourney; // RA2: +0x3C, an int read from WonlineTournamentAllowed (default 1); no AlliesAllowed, wolClanTourney or MustAlly
};
