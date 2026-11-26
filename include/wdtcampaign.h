/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "wdtnet.h"

/*
 * RA2: WorldDominationTour::Campaign in a header of its own (lane O99), out of wdtnet.h. A unit that sees this class
 * instantiates DynamicVectorClass<rc_ptr<Conflict> > for its Conflicts member, and VC6 then gives rc_ptr<Conflict>'s
 * vtable the vector deleting destructor; wdtconflict's object carries the scalar one (0x00727CC0), so its unit never
 * saw the class.
 */
namespace WorldDominationTour
{

	/*
	 * RA2: the progress of one campaign, kept in WDTHist.INI under its name.
	 */
	class Campaign : public ReferenceCounted {
		public:
			Campaign(Wstring const & name, rc_ptr<CampaignProperties> const & props); // RA2: 0x00725F40
			virtual ~Campaign(void); // RA2: 0x007262E0
			static rc_ptr<Campaign> __fastcall Create(Wstring const & name, rc_ptr<CampaignProperties> const & props); // RA2: 0x00725EE0
			bool Is_Different_Cycle(void) const; // RA2: 0x00726350
			bool Is_Different(int faction) const; // RA2: 0x00726370; the saved progress of the faction differs
			int Get_Last_Day(int faction) const; // RA2: 0x007263A0
			void Save_Progress(int faction); // RA2: 0x007263B0
			void Reset_Progress(void); // RA2: 0x00726580; zeroes the cycle and every faction's last day in WDTHist.INI
			CONFLICT_ITER Get_Conflict_Iterator(int index); // RA2: 0x00726730; the first conflict over that territory
			void Set_Faction(int faction); // RA2: 0x007267C0; keeps the side and writes PlayerFaction
			void Load_INI(INIClass & ini); // RA2: 0x00726930; loads WDTHist.INI, creating it when missing
			int Get_Player_Faction(void) const { return(PlayerFaction); }

		public:
			rc_ptr<CampaignProperties> Properties; // RA2: +0x08
			rc_ptr<History> CycleHistory; // RA2: +0x10
			CONFLICT_LIST Conflicts; // RA2: +0x18
			int CycleID; // RA2: +0x30
			int LastDay[4]; // RA2: +0x34
			int PlayerFaction; // RA2: +0x44
			Wstring Name; // RA2: +0x48
	};

};
