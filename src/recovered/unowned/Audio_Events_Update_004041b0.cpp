// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Audio_Events_Update() -- 0x004041B0
// Purpose: one pass of the event system: sorts each event type's events by
// volume and stops those over the type's limit, updates every event, frees
// stopped events while ranking the types by priority and volume and setting
// the shared volume compression, then starts the events waiting to play,
// stopping events of weaker types when a sample cannot be loaded.
// from ra2-decomp 0x004041D0 (AudioServiceAllEvents, src/wpaudio/audevent.cpp)
#include "shared/audio_event.hpp"

static int Audio_Event_Frame;                           // 0x00830FC4 counts update passes
// @ref _Audio_Event_Frame=00830fc4
static unsigned int Audio_Event_Bucket_Stamp;           // 0x00830FC8 counts Audio_Priority_Lists rebuilds
// @ref _Audio_Event_Bucket_Stamp=00830fc8
static int Audio_Events_Updating;                       // 0x00830FCC set while an update pass runs
// @ref _Audio_Events_Updating=00830fcc

static void Audio_Event_Add_Local_Sort(AudioEvent *new_event)
{
    AudioEventType *type;
    AudioListNode *node, *head;
    AudioEvent *event;
    unsigned int volume, new_volume;

    type = new_event->Type;
    new_volume = new_event->Levels.Volume.Current >> 16;
    head = &type->Local;
    node = head->Next;

    while (node != head) {
        int difference, slack;
        unsigned int base_level;
        event = (AudioEvent *)((char *)node - 0x0c);
        volume = event->Levels.Volume.Current >> 16;
        base_level = type->VolumeLevel.Current >> 16;
        slack = base_level / 10;
        difference = volume - new_volume;
        if ((difference >= 0 ? difference : -difference) < slack) {
            if (type->Flags & 0x10) {
                if (event->State != 0) {
                    break;
                }
            } else {
                if (event->State == 0) {
                    break;
                }
            }
        } else if (volume < new_volume) {
            break;
        }
        node = node->Next;
    }

    Audio_List_Init_Node(&new_event->Local);
    Audio_List_Add_Tail(node, &new_event->Local);
    type->Count++;
    if (type->unknown_48 && (type->Count > type->unknown_48)) {
        event = (AudioEvent *)((char *)head->Prev - 0x0c);
        Audio_List_Remove(&event->Local);
        Audio_Event_Stop(event);
        type->Count--;
    }
}

static void Audio_Event_Buckets_Reset()
{
    int i, k;
    AudioListNode (*bucket)[10];
    for (i = 0, bucket = Audio_Priority_Lists; i < 7; i++, bucket++) {
        for (k = 0; k < 10; k++) {
            Audio_List_Init(&(*bucket)[k]);
        }
    }
    Audio_Event_Bucket_Stamp++;
}

static void Audio_Event_Bucket_Add(AudioEvent *event)
{
    AudioListNode (*bucket)[10];
    AudioEventType *type;
    int priority;
    unsigned int volume;

    type = event->Type;
    priority = type->unknown_40 + event->unknown_154;
    volume = (event->Levels.Volume.Current >> 16) / (0x4000 / 10);
    bucket = &Audio_Priority_Lists[priority];

    if (type->LastBucketStamp == Audio_Event_Bucket_Stamp) {
        if (priority > type->MaxPriority) {
            type->MaxPriority = priority;
            type->MaxVolume = volume;
            Audio_List_Remove(&type->BucketNode);
            Audio_List_Add_Tail(&(*bucket)[volume], &type->BucketNode);
        } else if (priority == type->MaxPriority) {
            if (volume > type->MaxVolume) {
                type->MaxVolume = volume;
                Audio_List_Remove(&type->BucketNode);
                Audio_List_Add_Tail(&(*bucket)[volume], &type->BucketNode);
            }
        }
    } else {
        type->LastBucketStamp = Audio_Event_Bucket_Stamp;
        type->MaxPriority = priority;
        type->MaxVolume = volume;
        Audio_List_Init_Node(&type->BucketNode);
        Audio_List_Add_Tail(&(*bucket)[volume], &type->BucketNode);
    }
}

static int Audio_Event_Type_Sacrifice(AudioEvent *new_event)
{
    AudioEvent *event, *next, *head;
    AudioEventType *sacrifice = 0;
    int killed = 0;
    sacrifice = (AudioEventType *)Audio_Find_Listed(new_event->Type->unknown_40 + new_event->unknown_154);
    if (sacrifice == 0) {
        return 0;
    }
    Audio_List_Remove(&sacrifice->BucketNode);
    head = (AudioEvent *)&Audio_Event_List;
    event = (AudioEvent *)head->Next;
    while (event != head) {
        next = (AudioEvent *)event->Next;
        if (event->Type == sacrifice) {
            Audio_Event_Stop(event);
            Audio_Event_Free(event);
            killed = 1;
        }
        event = next;
    }
    return killed;
}

void Audio_Events_Update()
{
    AudioEvent *event, *next, *head;
    enum { AVERAGE_LOUDNESS = 20, FULL_LOUDNESS = 100 };
    int loudness;
    int processed_events = 0;

    if (!Audio_Events_Ready || Audio_Events_Updating) {
        return;
    }
    Audio_Events_Updating = 1;
    Audio_Event_Frame++;
    head = (AudioEvent *)&Audio_Event_List;

    event = (AudioEvent *)head->Next;
    while (event != head) {
        if (event->Type->LastFrame != Audio_Event_Frame) {
            Audio_List_Init(&event->Type->Local);
            event->Type->Count = 0;
            event->Type->LastFrame = Audio_Event_Frame;
        }
        event->Levels.Update();
        if (event->State != 4) {
            Audio_Event_Add_Local_Sort(event);
        }
        event = (AudioEvent *)event->Next;
    }

    event = (AudioEvent *)head->Next;
    while (event != head) {
        if (event->State != 4) {
            processed_events++;
        }
        Audio_Event_Update(event);
        event = (AudioEvent *)event->Next;
    }

    event = (AudioEvent *)head->Next;
    loudness = 0;
    Audio_Event_Buckets_Reset();
    while (event != head) {
        next = (AudioEvent *)event->Next;
        if (!event->Channel || event != (AudioEvent *)event->Channel->Owner) {
            Audio_Event_Stop(event);
        }
        if (event->Flags & AUDIO_EVENT_STOPPED) {
            Audio_Event_Free(event);
        } else {
            if (!Audio_Event_Is_Paused(event)) {
                loudness += (unsigned int)(AVERAGE_LOUDNESS * (event->Type->VolumeLevel.Current >> 16)) >> 14;
            }
            Audio_Event_Bucket_Add(event);
        }
        event = next;
    }

    if (loudness > FULL_LOUDNESS) {
        Audio_Event_Levels.Volume.Set_Target((unsigned int)(0x4000 * FULL_LOUDNESS) / loudness);
    } else {
        Audio_Event_Levels.Volume.Set_Target(0x4000);
    }
    Audio_Event_Levels.Update();

    event = (AudioEvent *)head->Next;
    while (event != head) {
        next = (AudioEvent *)event->Next;
        if (event->State == 1 && !Audio_Event_Is_Paused(event)) {
            Audio_Event_Load_Samples(event);
            while (!(event->unknown_148 = (int)Audio_Event_Start_Pass(event))) {
                if (Audio_Event_Type_Sacrifice(event)) {
                    next = (AudioEvent *)event->Next;
                    Audio_Event_Load_Samples(event);
                } else {
                    break;
                }
            }
            if (!(event->unknown_148 = (int)Audio_Event_Start_Pass(event)) || !Audio_Event_Begin(event)) {
                Audio_Event_Stop(event);
                Audio_Event_Free(event);
            }
        }
        event = next;
    }

    Audio_Events_Updating = 0;
}
