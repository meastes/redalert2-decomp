// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

typedef unsigned __int64 TimeStamp;

#define SECONDS(n) ((n) * 1000)

void InitAudioTimer(void);
TimeStamp AudioGetTime(void);

// Declarations moved from the units that used them (one declaration per name).
extern TimeStamp LoadingScreenHoldUntil; // RA2: 0x00A41668, as below
extern TimeStamp SideSoundTime; // RA2: 0x00A41680, when the hovered side's sound may play
