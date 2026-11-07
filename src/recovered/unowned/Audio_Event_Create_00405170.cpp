// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// AudioEvent *__fastcall Audio_Event_Create(AudioEventType *type) -- 0x00405170
// @ref ?Audio_Event_Serial@@3HA=007ccbdc
// Purpose: a new event of a type whose samples are loaded: taken from the event
// pool (freeing stopped events and trying again when it is empty) or, failing
// that, allocated and flagged so; counted, cleared, given fresh levels, listed
// and numbered. 0 when events are off or nothing could be had.
// from ra2-decomp 0x00405190 (AudioEventCreate, src/wpaudio/audevent.cpp; RA2 adds the allocation fallback)
#include "shared/audio_event.hpp"
#include "shared/crt_memory.hpp"

void *__cdecl operator new(unsigned int size);

AudioEvent *__fastcall Audio_Event_Create(AudioEventType *type)
{
    AudioEvent *event;
    int allocated = 0;

    if (!Audio_Enabled || !Audio_Events_Open) {
        return 0;
    }
    if (!type->unknown_0c) {
        return 0;
    }
    if (!(event = (AudioEvent *)Audio_Pool_Alloc(Audio_Event_Pool))) {
        Audio_Events_Free_Stopped();
        if (!(event = (AudioEvent *)Audio_Pool_Alloc(Audio_Event_Pool))) {
            event = (AudioEvent *)operator new(sizeof(AudioEvent));
            allocated = 1;
            if (!event) {
                return 0;
            }
        }
    }
    Audio_Event_Count++;
    if (Audio_Event_Count > Audio_Event_Maximum) {
        Audio_Event_Maximum = Audio_Event_Count;
    }
    memset(event, 0, sizeof(AudioEvent));
    event->Type = type;
    Audio_List_Init_Node(event);
    event->State = 0;
    event->unknown_154 = 0;
    event->SampleCount = event->unknown_1e0 = event->unknown_268 = 0;
    event->Levels.Init();
    event->PauseCount = 0;
    Audio_List_Add_Tail(&Audio_Event_List, event);
    event->unknown_138 = Audio_Event_Serial++;
    if (allocated) {
        event->Flags |= AUDIO_EVENT_ALLOCATED;
    }
    return event;
}
