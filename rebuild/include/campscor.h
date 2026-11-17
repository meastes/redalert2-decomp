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

#include "vector.h"

struct MPlayerScoreType;

/*
**	RA2: the score screens of the cooperative campaign (RTTI CampaignScoreClass, 8 bytes, vtable 0x0079D724;
**	CampaignEndScoreClass, 4 bytes, vtable 0x0079D730; src/campscor.cpp). Each has a virtual destructor
**	and one virtual that shows the screen for the scores of the players.
*/
class CampaignScoreClass
{
	public:
		CampaignScoreClass(void); // RA2: 0x00465BF0
		virtual ~CampaignScoreClass(void); // RA2: 0x00465C10
		virtual void Presentation(DynamicVectorClass<MPlayerScoreType *> scores, int time, bool more, int difficulty, wchar_t const * name); // RA2: 0x00466A50, shows the scores of a map: the game time, whether more maps follow, the difficulty (0 to 2, -1 unknown) and the name of the campaign

		void Tally_Scores(void); // RA2: 0x00465D10, fills the session's score table from the houses

		int Field4; // RA2: +0x04
};

class CampaignEndScoreClass
{
	public:
		CampaignEndScoreClass(void); // RA2: 0x00467200
		virtual ~CampaignEndScoreClass(void); // RA2: 0x00467230
		virtual void Presentation(DynamicVectorClass<MPlayerScoreType *> scores, int time, int difficulty, int campaign, wchar_t const * name); // RA2: 0x00467A20, shows the totals of the campaign
};

extern bool CampaignScoreDone; // RA2: 0x00850504, set by the score screens when the players are done with them (MPCooperative::vt_entry_90 reads and clears it)
