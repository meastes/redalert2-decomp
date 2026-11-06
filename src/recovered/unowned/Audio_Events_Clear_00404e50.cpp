// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Audio_Events_Clear() -- 0x00404E50
// Purpose: stops and frees every event.
// Shape from ra2-decomp 0x00404E70 (AudioKillAllEvents, src/wpaudio/audevent.cpp).
#include "shared/audio_event.hpp"

void Audio_Events_Clear()
{
    AudioEvent *event, *next, *head;

    if (!Audio_Events_Ready) {
        return;
    }
    head = (AudioEvent *)&Audio_Event_List;
    event = (AudioEvent *)head->Next;
    while (event != head) {
        next = (AudioEvent *)event->Next;
        Audio_Event_Stop(event);
        Audio_Event_Free(event);
        event = next;
    }
}
