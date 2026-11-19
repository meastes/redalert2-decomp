// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

#include "audtimer.h"
#include <windows.h>

struct AudioThread
{
		volatile int quit;
		volatile int count;
		volatile int leaving;
		volatile TimeStamp interval;
		volatile int running;
		HANDLE handle;
		void * data;
		DWORD id;
};
