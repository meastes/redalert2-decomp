/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

class HouseClass;

bool Single_Score_Presentation(HouseClass * house);
void Multi_Score_Presentation(void);

class MPScoreScreen // RA2: the screen (YR: MPScoreScreen, ctor 0x5AAA20)
{
	public:
		MPScoreScreen(void);
		void Presentation(void); // RA2: 0x5AAA40, shows the screen (named after TibSun's ScoreClass::Presentation)
};


