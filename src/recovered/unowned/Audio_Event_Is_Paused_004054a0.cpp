// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int __fastcall Audio_Event_Is_Paused(AudioEvent *event) -- 0x004054A0
// Purpose: the event's pause count (nonzero while it is paused).
// from ra2-decomp 0x00405490 (AudioEventIsPaused, src/wpaudio/audevent.cpp)
#include "shared/audio_event.hpp"

int __fastcall Audio_Event_Is_Paused(AudioEvent *event)
{
    return event->PauseCount;
}
