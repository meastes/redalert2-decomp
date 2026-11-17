/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

#pragma once

#include "multiplayer.h"
#include "coopcamp.h"
#include "ipxaddr.h"
#include "campscor.h" // RA2: the score screens of the cooperative campaign (src/campscor.cpp)

struct NodeNameType;
struct MPlayerScoreType;

/*
**	RA2: the cooperative game mode: two human players play the campaign maps of CoopCamp.ini
**	together against the computer. The mode keeps the list of campaigns, the campaign that is
**	being played, and (while a map is played) a copy of the lobby's settings that it puts back
**	when the map is over.
*/
class MPCooperative : public MultiplayerGameMode // RA2: vtable 0x007A678C, 0x348 bytes (the constructor 0x005A2960 passes false for the computer players allowed)
{
	public:
		MPCooperative(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex); // RA2: 0x005A2960
		virtual ~MPCooperative(void); // RA2: 0x005A2B50
		virtual bool AssignTeams(void) override; // RA2: 0x005A2D70, the base's slot 4 is used here to read the campaigns again
		virtual bool IsCountryAllowed(int country) override; // RA2: 0x005A2CD0, whether the campaign lets a player pick the house (the base returns true)
		virtual void vt_entry_20(void) override; // RA2: 0x005A2EF0, called when the lobby opens: selects the campaign and its first map and tells the host (O8)
		virtual void vt_entry_24(void) override; // RA2: 0x005A31F0, marks the lobby's player section as changed (+0x344)
		virtual int FirstValidMapIndex(void) override; // RA2: 0x005A3200
		virtual int DefaultCountry(void) override; // RA2: 0x005A34B0
		virtual bool vt_entry_48(int index, DynamicVectorClass<int> picks, char const * map, char const * name) override; // RA2: 0x005A3500
		virtual bool vt_entry_4C(unsigned index, unsigned time, struct MPlayerScoreType * scores) override; // RA2: 0x005A35D0
		virtual bool vt_entry_50(void) override; // RA2: 0x005A38D0
		virtual bool vt_entry_54(void) override; // RA2: 0x005A3960
		virtual void vt_entry_64(unsigned kind, char * text) override; // RA2: 0x005A3CA0
		virtual bool UnfixAlliances(void) override; // RA2: 0x005A3EE0
		virtual void PopulateTeamDropdownForPlayer(HWND hwnd, int index) override; // RA2: 0x005A3F70
		virtual Cell * AssignStartingPositionsToHouse(Cell * result, int house, DynamicVectorClass<Cell> * coords, unsigned char * satisfied) override; // RA2: 0x005A3F90
		virtual bool StartingPositionsToHouseBaseCells(char unused) override; // RA2: 0x005A4170
		virtual bool vt_entry_58(void) override; // RA2: 0x005A4250
		virtual int RandomHumanCountryIndex(void) override; // RA2: 0x005A4540
		virtual int RandomAICountryIndex(void) override; // RA2: 0x005A45F0
		virtual bool vt_entry_70(void) override; // RA2: 0x005A43C0
		virtual bool OnGameStart(void) override; // RA2: 0x005A46A0
		virtual bool IsAIAllowed(void) override; // RA2: 0x005A46B0
		virtual void vt_entry_88(bool player_wins) override; // RA2: 0x005A46C0
		virtual void DrawTeamDropdown(HWND list, DynamicVectorClass<MultiMission const *> * maps, MultiMission const * map) override; // RA2: 0x005A4B70
		virtual void vt_entry_78(void) override; // RA2: 0x005A5000
		virtual void vt_entry_7C(void) override; // RA2: 0x005A5190
		virtual int vt_entry_80(void) override; // RA2: 0x005A5340
		virtual void vt_entry_84(void) override; // RA2: 0x005A4F90
		virtual bool vt_entry_90(void) override; // RA2: 0x005A5350
		virtual void vt_entry_94(void) override; // RA2: 0x005A5720
		virtual void vt_entry_98(void) override; // RA2: 0x005A6040
		virtual bool vt_entry_9C(DynamicVectorClass<int> list, int * result) override; // RA2: 0x005A6230
		virtual bool vt_entry_04(void) override; // RA2: 0x005A6280
		virtual bool vt_entry_08(void) override; // RA2: 0x005A6290
		virtual bool vt_entry_A0(void) override; // RA2: 0x005A62A0
		virtual void vt_entry_8C(bool show) override; // RA2: 0x005A5570
		virtual bool CheckStartConditions(WideWstring * message) override; // RA2: 0x005A32E0
		virtual bool AssignTeamsAtStart(void) override; // RA2: 0x005A3320

		void Fill_House_Combo(HWND dialog, int id); // RA2: 0x005A3650, fills a side combo box with the campaign's houses
		void Refresh_Player_Sides(HWND dialog); // RA2: 0x005A3760, the side boxes of the player slots and the description of the campaign
		static void Fill_Player_Colors(void); // RA2: 0x005A3920
		bool Recreate_Campaign(void); // RA2: 0x005A4F20, makes the campaign object again
		int Find_Campaign_Index(void); // RA2: 0x005A2E50, the campaign whose map is the selected scenario, -1 when none is

		CoopCampListClass * Campaigns; // RA2: +0x40, the campaigns of CoopCamp.ini
		CoopCampaignClass * Campaign; // RA2: +0x44, the campaign that is being played
		int Field48; // RA2: +0x48, zero from the constructor, not used elsewhere
		bool SavedCampaignChosen; // RA2: +0x4C, set when the saved campaign is the one picked
		bool SaveListed; // RA2: +0x4D, the map list has the "use the saved campaign" row
		int LoadedRecord; // RA2: +0x50, the record the campaign was loaded from last
		CampaignScoreClass * ScoreScreen; // RA2: +0x54
		int LocalPlayer; // RA2: +0x58, the index of the local player in SavedPlayers
		DynamicVectorClass<NodeNameType *> SavedPlayers; // RA2: +0x5C, copies of the lobby's players while a map is played
		int SavedField74; // RA2: +0x74, the lobby settings kept while a map is played start here (0x005A46C0 saves them, 0x005A5720 puts them back)
		wchar_t HostName[20]; // RA2: +0x78
		int Seed; // RA2: +0xA0
		int TechLevel; // RA2: +0xA4
		int UnitCount; // RA2: +0xA8
		int Credits; // RA2: +0xAC
		bool FogOfWar; // RA2: +0xB0
		bool BridgeDestruction; // RA2: +0xB1
		bool Goodies; // RA2: +0xB2
		bool MCVRedeploy; // RA2: +0xB3
		bool AlliesAllowed; // RA2: +0xB4
		bool HarvTruce; // RA2: +0xB5
		bool Bases; // RA2: +0xB6
		bool CTF; // RA2: +0xB7
		int AIPlayers; // RA2: +0xB8
		int AIDifficulty; // RA2: +0xBC
		bool ShortGame; // RA2: +0xC0
		bool SWAllowed; // RA2: +0xC1
		bool BuildOffAlly; // RA2: +0xC2
		bool CrapEngineers; // RA2: +0xC3
		int GameSpeed; // RA2: +0xC4
		IPXAddressClass Addresses[8]; // RA2: +0xC8, constructed by the constructor
		int PlayerStates[8]; // RA2: +0x128
		int PlayerPortBases[8]; // RA2: +0x148
		wchar_t MasterName[21]; // RA2: +0x168
		int MasterID; // RA2: +0x194
		unsigned short PlayerPorts[8]; // RA2: +0x198
		DynamicVectorClass<int> HouseChoices[8]; // RA2: +0x1A8, one list per player slot: the house each map's player plays
		DynamicVectorClass<int> ColorChoices[8]; // RA2: +0x268, the same for the colors
		DynamicVectorClass<CoopCampaignClass *> CampaignStates; // RA2: +0x328, one campaign object per campaign of the list
		int CampaignIndex; // RA2: +0x340, the selected campaign
		bool PlayersChanged; // RA2: +0x344, the lobby's player section has to be filled again
};
