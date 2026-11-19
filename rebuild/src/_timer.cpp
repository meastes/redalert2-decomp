/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

#include "always.h"
#include "_timer.h"

/***************************************************************************
**	Game frame timer (this is synced between processes).
*/
CDTimerClass<SystemTimerClass> FrameTimer;
CDTimerClass<MillisecondSystemTimerClass> NetFrameTimer;

/***************************************************************************
**	Tick Count global timer object.
*/
TTimerClass<SystemTimerClass> TickCount = 0;
