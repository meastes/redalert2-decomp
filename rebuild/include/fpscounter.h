// SPDX-License-Identifier: GPL-3.0-or-later
// RA2: not in TibSun. The frame rate the game measured over the last second (YRpp names it
// FPSCounter::CurrentFrameRate); the detail checks and the beam effects compare it with the rate the
// session wants.
#pragma once

namespace FPSCounter {
	extern unsigned CurrentFrameRate; // RA2: 0xA6EB1C
	extern bool ReducedEffects; // RA2: set while the rate is under the detail minimum (logic.cpp)
}
