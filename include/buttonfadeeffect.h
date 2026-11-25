/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

// RA2: the fade record of the dropship loadout screen (TibSun's dropship.cpp text); the struct tag is the
// RTTI's: VectorClass<ButtonFadeEffect *> (vtable 0x7A0B10) names .?AV?$VectorClass@PAUButtonFadeEffect@@@@.
#pragma once


struct ButtonFadeEffect
{
		unsigned int StartTime;
		int CandidateIndex;
		float Alpha;
		float Direction;
		bool StopAtLow;

		ButtonFadeEffect(unsigned int start_time, int candidate_index, float alpha, float direction, bool stop_at_low) :
			StartTime(start_time),
			CandidateIndex(candidate_index),
			Alpha(alpha),
			Direction(direction),
			StopAtLow(stop_at_low)
		{
		}
};
