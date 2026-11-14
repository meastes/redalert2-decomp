// audevent.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audevent.json carries the addresses and the rows that
// stay in their own files.


#define RA2_AUDIO_EVENT_LIST_STATIC
#define RA2_AUDIO_EVENT_SERVICE
#include "shared/audio_event.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"
#include "shared/audio.hpp"


// int __fastcall Audio_Events_Init(AudioDevice *device, AudioBag *bag) -- 0x00403EB0
// Purpose: starts the event system: empties the event lists, resets the
// shared levels and, when there is a bag, builds the sample bank and the event
// pool. Returns 1 when everything needed exists.

static AudioListNode Audio_Event_List;                  // 0x00830EA0

int __fastcall Audio_Events_Init(AudioDevice *device, AudioBag *bag)
{
    Audio_List_Init(&Audio_Event_List);
    Audio_List_Init(&Audio_Event_Type_List);
    Audio_Event_Bag = bag;
    Audio_Event_Device = device;
    Audio_Event_Levels.Init();
    Audio_Event_Maximum = 0;
    Audio_Event_Count = 0;
    Audio_Events_Ready = 1;
    Audio_Enabled = 1;
    if (Audio_Event_Bag != 0) {
        Audio_Event_Bank = Audio_Bank_Create(Audio_Event_Bag, 0x100000, 200, 0x2000);
        Audio_Event_Pool = Audio_Pool_Create(100, 0x280);
        Debug_Printf("event mempool size = %d\n", 64000);
    }
    if (Audio_Event_Bank != 0 && Audio_Event_Bag != 0 && Audio_Event_Device != 0 && Audio_Event_Pool != 0) {
        Audio_Events_Open = 1;
        return 1;
    }
    return 0;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Audio_Events_Shutdown() -- 0x00403F80
// Purpose: stops the event system: stops and frees every event, frees any
// stopped event still listed, deletes every event type, destroys the sample
// bank and the event pool, and marks the system closed.
// from ra2-decomp 0x00403FA0 (AudioEventCloseDown, src/wpaudio/audevent.cpp)

static void Audio_Event_Types_Free()
{
    AudioListNode *type;
    while ((type = Audio_List_First(&Audio_Event_Type_List)) != 0) {
        Audio_List_Remove(type);
        operator delete(type);
    }
}

void Audio_Events_Shutdown()
{
    Audio_Events_Clear();
    Audio_Events_Free_Stopped();
    Audio_Event_Types_Free();
    if (Audio_Event_Bank != 0) {
        Audio_Bank_Destroy(Audio_Event_Bank);
        Audio_Event_Bank = 0;
    }
    if (Audio_Event_Pool != 0) {
        Audio_Pool_Destroy(Audio_Event_Pool);
        Audio_Event_Pool = 0;
    }
    Audio_Events_Open = 0;
    Audio_Events_Ready = 0;
}


// void __fastcall Audio_Event_Detach(AudioEvent *event) -- 0x00404120
// Purpose: takes the event off the channel that plays it (when the channel
// still belongs to the event): stops the channel if the event was started,
// clears its flag 0x10, handlers, owner and levels.

void __fastcall Audio_Event_Detach(AudioEvent *event)
{
    if (event->Channel != 0 && event == event->Channel->Owner) {
        if (event->Flags & 2) {
            Audio_Channel_Stop(event->Channel);
        }
        Audio_Channel_Clear_Flag_10(event->Channel);
        event->Channel->Owner = 0;
        event->Channel->unknown_end_callback_bc = 0;
        event->Channel->unknown_loop_b4 = 0;
        event->Channel->unknown_done_b8 = 0;
        event->Channel->unknown_levels_90 = 0;
        event->Channel->unknown_levels_98 = 0;
        event->Channel->unknown_levels_9c = 0;
        event->Channel = 0;
    }
}


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

static int Audio_Event_Frame;                           // 0x00830FC4 counts update passes
static unsigned int Audio_Event_Bucket_Stamp;           // 0x00830FC8 counts Audio_Priority_Lists rebuilds
static int Audio_Events_Updating;                       // 0x00830FCC set while an update pass runs

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
            while (!(event->unknown_148 = Audio_Event_Start_Pass(event))) {
                if (Audio_Event_Type_Sacrifice(event)) {
                    next = (AudioEvent *)event->Next;
                    Audio_Event_Load_Samples(event);
                } else {
                    break;
                }
            }
            if (!(event->unknown_148 = Audio_Event_Start_Pass(event)) || !Audio_Event_Begin(event)) {
                Audio_Event_Stop(event);
                Audio_Event_Free(event);
            }
        }
        event = next;
    }

    Audio_Events_Updating = 0;
}


// AudioSource *__fastcall Audio_Event_Start_Pass(AudioEvent *event) -- 0x004046E0
// Purpose: starts a pass over the event's samples: fills the play order with
// the sample indexes (without the intro of a sequence and the sustained last
// sample) and returns the first source to play: the intro of a sequence, or
// the next sample in order. 0 when there is nothing to play.

AudioSource *__fastcall Audio_Event_Start_Pass(AudioEvent *event)
{
    int index = 0;
    event->unknown_1e0 = event->SampleCount;
    if (event->unknown_1e0 == 0 || (event->Flags & 0x20)) {
        return 0;
    }
    AudioEventType *type = event->Type;
    if (type->unknown_58 < 0x21) {
        if (type->Flags & AUDIO_EVENT_TYPE_SEQUENCE) {
            event->unknown_1e0--;
            index = 1;
            event->unknown_ac = 0;
        } else {
            event->unknown_ac = -1;
        }
        if (type->Flags & AUDIO_EVENT_TYPE_SUSTAIN) {
            event->unknown_1e0--;
        }
        if (event->unknown_1e0 <= 0) {
            type->unknown_0c = 0;
            return 0;
        }
        for (int i = 0; i < event->unknown_1e0; i++, index++) {
            event->unknown_160[i] = index;
        }
        if ((event->Flags & 8) || !(event->Type->Flags & AUDIO_EVENT_TYPE_SEQUENCE)) {
            return Audio_Event_Next_Sample(event);
        }
    }
    return event->Samples[0]->Get_Source();
}


// AudioSource *__fastcall Audio_Event_Next_Sample(AudioEvent *event) -- 0x00404790
// Purpose: moves an event on to its next sample: a random event draws one of the
// entries left in its order, others take the following one; a sustained event
// whose samples are used up repeats, a looping event starts another pass.

AudioSource *__fastcall Audio_Event_Next_Sample(AudioEvent *event)
{
    AudioEventType *type = event->Type;
    unsigned int flags = type->Flags;
    if (type->unknown_58 < 0x21 || (event->Flags & 0x20)) {
        if (event->unknown_1e0 != 0 && !(event->Flags & 0x20)) {
            if (flags & AUDIO_EVENT_TYPE_RANDOM) {
                int index = unknown_random_839218(0, event->unknown_1e0 - 1);
                event->unknown_ac = event->unknown_160[index];
                for (int i = index; i < event->unknown_1e0 - 1; i++) {
                    event->unknown_160[i] = event->unknown_160[i + 1];
                }
            } else {
                event->unknown_ac++;
            }
            event->unknown_1e0--;
            return event->Samples[event->unknown_ac]->Get_Source();
        }
        if ((flags & AUDIO_EVENT_TYPE_LOOPS) && !(event->Flags & 0x20)
            && (type->LoopCount == 0 || event->unknown_1e4 < type->LoopCount - 1)) {
            event->unknown_1e4++;
            return Audio_Event_Start_Pass(event);
        }
        if ((flags & AUDIO_EVENT_TYPE_SUSTAIN) && !(event->Flags & 0x10) && event->SampleCount >= 1) {
            event->Flags |= 0x10;
            return event->Samples[event->SampleCount - 1]->Get_Source();
        }
    }
    return 0;
}


// Acquire the bank samples needed for the event's next playback pass (0x00404890).
static int Audio_Random(int low, int high);
void __fastcall Audio_Event_Load_Samples(AudioEvent *event)
{
    AudioEventType *type = event->Type;
    if (event->SampleCount != 0) {
        if (type->unknown_58 < 33) return;
        for (int i = 0; i < event->SampleCount; ++i) {
            ((AudioCountedList *)event->Samples[i])->Leave();
            event->Samples[i] = 0;
        }
        event->SampleCount = 0;
    }
    int piece;
    if (type->unknown_58 < 33) {
        if (type->Intro != 0) {
            piece = Audio_Random(0, type->Intro - 1);
            event->Samples[event->SampleCount] = Audio_Bank_Load(Audio_Event_Bank, event->Type->Samples[piece]);
            if (event->Samples[event->SampleCount]) {
                ((AudioCountedList *)event->Samples[event->SampleCount])->Enter();
                ++event->SampleCount;
            }
        }
        if (type->Flags & 4) {
            for (piece = type->Intro; piece < type->SampleCount - type->Outro; ++piece) {
                event->Samples[event->SampleCount] = Audio_Bank_Load(Audio_Event_Bank, event->Type->Samples[piece]);
                if (event->Samples[event->SampleCount]) {
                    ((AudioCountedList *)event->Samples[event->SampleCount])->Enter();
                    ++event->SampleCount;
                }
            }
        } else {
            if (type->Flags & AUDIO_EVENT_TYPE_RANDOM) {
                piece = Audio_Random(type->Intro, type->SampleCount - type->Outro - 1);
                event->Samples[event->SampleCount] = Audio_Bank_Load(Audio_Event_Bank, event->Type->Samples[piece]);
                if (event->Samples[event->SampleCount]) {
                    ((AudioCountedList *)event->Samples[event->SampleCount])->Enter();
                    ++event->SampleCount;
                }
            } else {
                piece = type->Intro;
                event->Samples[event->SampleCount] = Audio_Bank_Load(Audio_Event_Bank, event->Type->Samples[piece]);
                if (event->Samples[event->SampleCount]) {
                    ((AudioCountedList *)event->Samples[event->SampleCount])->Enter();
                    ++event->SampleCount;
                }
            }
        }
        if (type->Outro == 0) return;
        piece = Audio_Random(type->SampleCount - type->Outro, type->SampleCount - 1);
        event->Samples[event->SampleCount] = Audio_Bank_Load(Audio_Event_Bank, event->Type->Samples[piece]);
        if (event->Samples[event->SampleCount]) {
            ((AudioCountedList *)event->Samples[event->SampleCount])->Enter();
            ++event->SampleCount;
        }
        return;
    } else {
        int outro = -1;
        if (type->Outro != 0 && (type->Flags & AUDIO_EVENT_TYPE_SUSTAIN)) {
            outro = Audio_Random(type->SampleCount - type->Outro, type->SampleCount - 1);
        }
        piece = Audio_Event_Next_Piece(event);
        if (piece >= 0) {
            event->Samples[event->SampleCount] = Audio_Bank_Load(Audio_Event_Bank, event->Type->Samples[piece]);
            if (event->Samples[event->SampleCount]) {
                ((AudioCountedList *)event->Samples[event->SampleCount])->Enter();
                ++event->SampleCount;
            }
            if (outro != -1) {
                event->Samples[event->SampleCount] = Audio_Bank_Load(Audio_Event_Bank, event->Type->Samples[outro]);
                if (event->Samples[event->SampleCount]) {
                    ((AudioCountedList *)event->Samples[event->SampleCount])->Enter();
                    ++event->SampleCount;
                }
            }
        } else {
            if (outro == -1 || (event->Flags & 0x10)) return;
            event->Samples[event->SampleCount] = Audio_Bank_Load(Audio_Event_Bank, event->Type->Samples[outro]);
            if (event->Samples[event->SampleCount]) {
                ((AudioCountedList *)event->Samples[event->SampleCount])->Enter();
                ++event->SampleCount;
            }
            event->Flags |= 0x10;
        }
    }
}


static int Audio_Random(int low, int high);
int __fastcall Audio_Event_Next_Piece(AudioEvent *event)
{
    AudioEventType *type = event->Type;
    unsigned int flags = type->Flags;
    if (event->unknown_268 == event->unknown_270) {
        int count = 0;
        if (((type->Flags & AUDIO_EVENT_TYPE_LOOPS) == 0 && event->unknown_26c == 1) ||
            ((type->Flags & AUDIO_EVENT_TYPE_LOOPS) != 0 && type->LoopCount != 0 && event->unknown_26c >= type->LoopCount)) return -1;
        if (type->Intro != 0 && (event->Flags & 8) == 0) {
            event->Order[count++] = Audio_Random(0, type->Intro - 1);
        }
        if (type->Flags & 4) {
            for (int piece = type->Intro; piece < type->SampleCount - type->Outro; ++piece) {
                event->Order[count++] = piece;
            }
        } else {
            int piece;
            if (type->Flags & AUDIO_EVENT_TYPE_RANDOM) {
                if (event->unknown_26c == 0) {
                    piece = Audio_Random(type->Intro, type->SampleCount - type->Outro - 1);
                    event->unknown_274 = piece;
                } else piece = event->unknown_274;
            } else piece = type->Intro;
            event->Order[count++] = piece;
        }
        event->unknown_268 = count;
        event->unknown_270 = 0;
        ++event->unknown_26c;
    }
    int piece;
    if (flags & AUDIO_EVENT_TYPE_RANDOM) {
        if (type->Intro != 0 && (event->Flags & 8) == 0 && event->unknown_270 <= 0) {
            goto sequential_piece;
        }
        int low = type->Intro != 0 && (event->Flags & 8) == 0 ? 1 : 0;
        int pick = Audio_Random(low, event->unknown_268 - 1);
        piece = event->Order[pick];
        for (int i = pick; i < event->unknown_268 - 1; ++i) event->Order[i] = event->Order[i + 1];
        --event->unknown_268;
    } else {
sequential_piece:
        piece = event->Order[event->unknown_270++];
    }
    return piece;
}


// void __fastcall Audio_Event_Release_Samples(AudioEvent *event) -- 0x00404D60
// Purpose: lets go of the event's samples (0x00401F90 on each bank entry)
// and forgets them.

void __fastcall Audio_Event_Release_Samples(AudioEvent *event)
{
    for (int i = 0; i < event->SampleCount; i++) {
        ((AudioCountedList *)event->Samples[i])->Leave();
        event->Samples[i] = 0;
    }
    event->SampleCount = 0;
}


// void __fastcall Audio_Event_Free(AudioEvent *event) -- 0x00404DB0
// Purpose: unlinks the event and frees it: deleted when it was allocated,
// else returned to the event pool. One event less.

void __cdecl operator delete(void *block);

void __fastcall Audio_Event_Free(AudioEvent *event)
{
    Audio_List_Remove(event);
    event->unknown_138 = 0;
    if (event->Flags & AUDIO_EVENT_ALLOCATED) {
        operator delete(event);
    } else {
        Audio_Pool_Free(Audio_Event_Pool, event);
    }
    Audio_Event_Count--;
}


// void *__fastcall Audio_Find_Listed(int rows) -- 0x00404E00
// Purpose: returns the object (the list node minus 0xA4) on the first
// non-empty list among the first rows of Audio_Priority_Lists, 0 if all are
// empty. What the lists hold is not known yet.

void *__fastcall Audio_Find_Listed(int rows)
{
    AudioListNode (*list)[10] = Audio_Priority_Lists;
    for (int row = 0; row < rows; row++, list++) {
        for (int column = 0; column < 10; column++) {
            AudioListNode *node = Audio_List_First(&(*list)[column]);
            if (node != 0) {
                return (char *)node - 0xa4;
            }
        }
    }
    return 0;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Audio_Events_Clear() -- 0x00404E50
// Purpose: stops and frees every event.
// Shape from ra2-decomp 0x00404E70 (AudioKillAllEvents, src/wpaudio/audevent.cpp).

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


// void Audio_Events_Pause() -- 0x00404FB0
// Purpose: pauses every event: one more pause on each, and a playing event
// (state 2) with its first pause moves its start time back by the current
// time.

void Audio_Events_Pause()
{
    if (Audio_Events_Ready != 0) {
        for (AudioEvent *event = (AudioEvent *)Audio_Event_List.Next; event != (AudioEvent *)&Audio_Event_List;
             event = (AudioEvent *)event->Next) {
            if (event->PauseCount == 0 && event->State == 2) {
                event->unknown_140 -= Audio_Now();
            }
            event->PauseCount++;
        }
    }
}


// void Audio_Events_Resume() -- 0x00405020
// Purpose: lifts one pause from every paused event; a playing event (state
// 2) whose pauses are all gone gets its start time moved forward by the
// current time.

void Audio_Events_Resume()
{
    if (Audio_Events_Ready != 0) {
        for (AudioEvent *event = (AudioEvent *)Audio_Event_List.Next; event != (AudioEvent *)&Audio_Event_List;
             event = (AudioEvent *)event->Next) {
            if (event->PauseCount != 0) {
                if (--event->PauseCount < 0) {
                    event->PauseCount = 0;
                }
                if (event->PauseCount == 0 && event->State == 2) {
                    event->unknown_140 += Audio_Now();
                }
            }
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Audio_Events_Free_Stopped() -- 0x004050A0
// Purpose: frees every event that has been stopped.
// Shape from ra2-decomp 0x004050C0 (AudioFlushAllDeadEvents, src/wpaudio/audevent.cpp).

void Audio_Events_Free_Stopped()
{
    AudioEvent *event, *next, *head;

    if (!Audio_Events_Ready) {
        return;
    }
    head = (AudioEvent *)&Audio_Event_List;
    event = (AudioEvent *)head->Next;
    while (event != head) {
        next = (AudioEvent *)event->Next;
        if (event->Flags & AUDIO_EVENT_STOPPED) {
            Audio_Event_Free(event);
        }
        event = next;
    }
}


// 0x00405110: function not yet identified
//
// Body: 16 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined4 __fastcall Audio_Event_Type_Range_00405110(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00405110 {

extern "C" unsigned int Audio_Events_Ready_00830fb8;

unsigned int __fastcall Audio_Event_Type_Range_00405110(int type)
{
    if (Audio_Events_Ready_00830fb8 == 0) {
        return 0;
    }
    return *(unsigned int *)(type + 0x50);
}

}  // namespace seed_00405110


// float __fastcall Audio_Event_Type_Get_Rate(AudioEventType *type) -- 0x00405120
// Purpose: returns the type's rate setting (0 while the event system is off).

float __fastcall Audio_Event_Type_Get_Rate(AudioEventType *type)
{
    if (Audio_Events_Ready == 0) {
        return 0.0f;
    }
    return type->unknown_54;
}


// 0x00405140: function not yet identified
//
// Body: 16 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined4 __fastcall Audio_Event_Type_Flags_00405140(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00405140 {

extern "C" unsigned int Audio_Events_Ready_00830fb8;

unsigned int __fastcall Audio_Event_Type_Flags_00405140(int type)
{
    if (Audio_Events_Ready_00830fb8 == 0) {
        return 0;
    }
    return *(unsigned int *)(type + 0x14);
}

}  // namespace seed_00405140


// char *__fastcall Audio_Event_Type_Name(AudioEventType *type) -- 0x00405150
// Purpose: returns the type's name ("no events" while the event system is
// off).

char *__fastcall Audio_Event_Type_Name(AudioEventType *type)
{
    char *name;

    if (Audio_Events_Ready == 0) {
        name = "no events";
    } else {
        name = type->Name;
    }
    return name;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// AudioEvent *__fastcall Audio_Event_Create(AudioEventType *type) -- 0x00405170
// Purpose: a new event of a type whose samples are loaded: taken from the event
// pool (freeing stopped events and trying again when it is empty) or, failing
// that, allocated and flagged so; counted, cleared, given fresh levels, listed
// and numbered. 0 when events are off or nothing could be had.
// from ra2-decomp 0x00405190 (AudioEventCreate, src/wpaudio/audevent.cpp; RA2 adds the allocation fallback)

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


// 0x004052f0: function not yet identified
//
// Body: 9 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall Audio_Event_Set_Flag_8_004052f0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004052f0 {

void __fastcall Audio_Event_Set_Flag_8_004052f0(int event)
{
    *(unsigned int *)(event + 0x18) = *(unsigned int *)(event + 0x18) | 8;
    return;
}

}  // namespace seed_004052f0


// void __fastcall Audio_Event_Stop(AudioEvent *event) -- 0x00405300
// Purpose: stops an event that is not stopped yet: takes it off its channel,
// lets go of its samples, and marks it stopped (state 4, flag 1).

void __fastcall Audio_Event_Stop(AudioEvent *event)
{
    if (!(event->Flags & AUDIO_EVENT_STOPPED)) {
        Audio_Event_Detach(event);
        Audio_Event_Release_Samples(event);
        AUDIO_EVENT_MARK_STOPPED(event)
    }
}


// void __fastcall Audio_Event_Pause(AudioEvent *event) -- 0x004053F0
// Purpose: one more pause on the event; a playing one (state 2) with its
// first pause has its start time moved back by the current time.

void __fastcall Audio_Event_Pause(AudioEvent *event)
{
    if (event->PauseCount == 0 && event->State == 2) {
        event->unknown_140 -= Audio_Now();
    }
    event->PauseCount++;
}


// void __fastcall Audio_Event_Resume(AudioEvent *event) -- 0x00405440
// Purpose: lifts one pause from the event; a playing one (state 2) whose
// pauses are all gone has its start time moved forward by the current time.

void __fastcall Audio_Event_Resume(AudioEvent *event)
{
    if (event->PauseCount != 0) {
        if (--event->PauseCount < 0) {
            event->PauseCount = 0;
        }
        if (event->PauseCount == 0 && event->State == 2) {
            event->unknown_140 += Audio_Now();
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int __fastcall Audio_Event_Is_Paused(AudioEvent *event) -- 0x004054A0
// Purpose: the event's pause count (nonzero while it is paused).
// from ra2-decomp 0x00405490 (AudioEventIsPaused, src/wpaudio/audevent.cpp)

int __fastcall Audio_Event_Is_Paused(AudioEvent *event)
{
    return event->PauseCount;
}


// int __fastcall Audio_Event_Begin(AudioEvent *event) -- 0x004054B0
// Purpose: starts the channel of an event that was waiting (flag 8): puts its
// source on the channel, sets the starting frequency and volume and starts
// it. Returns 1 when the channel runs; otherwise the event goes back to
// state 1 (waiting for its source).

int __fastcall Audio_Event_Begin(AudioEvent *event)
{
    if (event != event->Channel->Owner) {
        return 0;
    }
    Audio_Channel_Lock(event->Channel);
    AudioSource *source = event->unknown_148;
    event->Flags |= 8;
    Audio_Channel_Set_Source(event->Channel, source);
    event->Channel->Levels.Frequency.Set(event->unknown_14c);
    int volume = 0x4000;
    if (event->unknown_150 > 0) {
        volume = 0x4000 - (unsigned int)(event->unknown_150 << 14) / 100;
    }
    event->Channel->Levels.Volume.Set(volume);
    event->State = 3;
    event->Flags |= 2;
    event->unknown_148 = 0;
    if (Audio_Channel_Start(event->Channel) != 0) {
        event->State = 1;
        event->unknown_148 = source;
        event->Flags &= ~2;
        Audio_Channel_Unlock(event->Channel);
        return 0;
    }
    Audio_Channel_Unlock(event->Channel);
    return 1;
}


// void __fastcall Audio_Event_Finish(AudioEvent *event) -- 0x00405590
// Purpose: marks an event that is not finished yet (flag 0x20) finished
// and not to be started (flags 0x60); when its channel is still its own the
// channel is unlocked in the driver.

void __fastcall Audio_Event_Finish(AudioEvent *event)
{
    if (!(event->Flags & 0x20)) {
        if (event->Channel != 0 && event == event->Channel->Owner) {
            Audio_Channel_Lock(event->Channel);
            event->Flags |= 0x60;
            Audio_Channel_Unlock(event->Channel);
        } else {
            event->Flags |= 0x60;
        }
    }
}


// void __fastcall Audio_Event_Update(AudioEvent *event) -- 0x004055D0
// Purpose: steps one event's state machine: 0 asks for a channel and sets
// it up (random frequency and volume, an optional random delay), 2 waits
// out the delay, 3 stops a finished one-shot loop whose handle is gone, a
// pending restart (flag 0x40) stops the channel and plays the next sample,
// and 4 stops the event. Audio off forces state 4.

static int Audio_Random(int low, int high)
{
    return unknown_random_839218(low, high);
}

static void Audio_Event_Start(AudioEvent *event)
{
    AudioEventType *type = event->Type;
    if (type->SampleCount != 0 && type->unknown_0c != 0) {
        event->unknown_1e4 = 0;
        event->Channel = Audio_Event_Start_Channel(type->unknown_40 + event->unknown_154, 0);
        if (event->Channel != 0) {
            event->unknown_14c = Audio_Random(type->unknown_60, type->unknown_64) + 100;
            event->unknown_150 = Audio_Random(0, type->unknown_68);
            Audio_Channel_Set_Flag_10(event->Channel);
            event->Channel->Owner = event;
            event->Channel->unknown_loop_b4 = Audio_Event_Channel_Next;
            event->Channel->unknown_done_b8 = Audio_Event_Channel_Done;
            event->Channel->unknown_end_callback_bc = Audio_Event_Channel_End;
            event->State = 1;
            event->Channel->unknown_levels_90 = &event->Levels;
            event->Channel->unknown_levels_98 = &Audio_Event_Levels;
            event->Channel->unknown_levels_9c = Audio_Global_Levels[7];
            if (type->Flags & 0x88) {
                int delay = Audio_Random((type->Flags & 0x80) ? 0x21 : type->unknown_58, type->unknown_5c);
                if (delay >= 0x21) {
                    __int64 wait = delay;
                    event->unknown_20 = 1;
                    if (event->PauseCount != 0) {
                        event->unknown_140 = wait;
                    } else {
                        event->unknown_140 = Audio_Now() + wait;
                    }
                    event->State = 2;
                }
            }
            return;
        }
    }
    Audio_Event_Stop(event);
    return;
}

void __fastcall Audio_Event_Update(AudioEvent *event)
{
    if (Audio_Enabled == 0) {
        event->State = 4;
    }
again:
    switch (event->State) {
    case 2:
        if (event->Flags & 0x40) {
            goto restart;
        }
        if (event->PauseCount != 0) {
            return;
        }
        if ((unsigned __int64)Audio_Now() <= (unsigned __int64)event->unknown_140) {
            return;
        }
        event->State = event->unknown_20;
        goto again;

    case 0:
        Audio_Event_Start(event);
        return;

    case 3:
        if ((event->Type->Flags & AUDIO_EVENT_TYPE_LOOPS) && event->Type->LoopCount == 0 && !(event->Flags & 0x20)) {
            if (event->unknown_278 == 0 || Audio_Event_Resolve(event->unknown_278) != event) {
                Audio_Event_Stop(event);
            }
        }
        if (event->Flags & 0x40) {
        restart:
            AudioChannel *channel = event->Channel;
            if (channel != 0) {
                Audio_Channel_Lock(channel);
                event->Flags &= ~0x40;
                Audio_Channel_Stop(event->Channel);
                if ((event->Type->Flags & AUDIO_EVENT_TYPE_SUSTAIN) && !(event->Flags & 0x10)) {
                    event->State = 1;
                    event->unknown_148 = Audio_Event_Next_Sample(event);
                    if (event->PauseCount == 0 && Audio_Event_Begin(event) == 0) {
                        Audio_Event_Stop(event);
                    }
                }
                Audio_Channel_Unlock(channel);
            } else {
                event->Flags &= ~0x40;
            }
        }
        break;

    case 4:
        Audio_Event_Stop(event);
        break;

    case 1:
        break;
    }
}


// 0x00405a00: function not yet identified
//
// Body: 16 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __fastcall Audio_Event_Channel_End_00405a00(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00405a00 {

unsigned int __fastcall Audio_Event_Channel_End_00405a00(int event)
{
    *(unsigned int *)(*(int *)(event + 0xc0) + 0x1c) = 4;
    return 0;
}

}  // namespace seed_00405a00


// int __fastcall Audio_Event_Channel_Done(AudioChannel *channel) -- 0x00405A10
// Purpose: channel callback when a sample has finished: clears the playing
// flag and, unless the event is already done, either starts the next sample
// (after the pending delay, if any) or marks the event done.
// Shape: ra2-decomp audioEventSampleDone (src/wpaudio/audevent.cpp, GAMEMD 0x00405A00) with eventWait inlined.

int __fastcall Audio_Event_Channel_Done(AudioChannel *channel)
{
    AudioEvent *event = (AudioEvent *)channel->Owner;
    int state;

    event->Flags &= ~2;
    if (event->State == 4) {
        return 0;
    }
    if (event->unknown_148 != 0 || event->unknown_268 != 0 || event->unknown_13c != 0) {
        state = 1;
    } else {
        state = 4;
        event->unknown_13c = 0;
    }
    if (event->unknown_13c != 0) {
        unsigned __int64 delay = event->unknown_13c;
        event->unknown_20 = state;
        if (event->PauseCount != 0) {
            event->unknown_140 = delay;
        } else {
            event->unknown_140 = Audio_Now() + delay;
        }
        event->State = 2;
        event->unknown_13c = 0;
    } else {
        event->State = state;
    }
    return 0;
}


// int __fastcall Audio_Event_Channel_Next(AudioChannel *channel) -- 0x00405AD0
// Purpose: channel callback of an event: picks the next sample and either
// plays it straight away or, for events with a random delay, parks it until
// the delay has passed.

int __fastcall Audio_Event_Channel_Next(AudioChannel *channel)
{
    AudioEvent *event = (AudioEvent *)channel->Owner;
    AudioSource *source = Audio_Event_Next_Sample(event);

    if (event->Type->unknown_5c < 0x21) {
        Audio_Channel_Set_Source(channel, source);
        return 0;
    }
    Audio_Channel_Set_Source(channel, 0);
    AudioEventType *type = event->Type;
    if ((type->Flags & 0x88) == 0 || source != 0 || event->unknown_268 != 0) {
        unsigned int hi = type->unknown_5c;
        unsigned int lo = type->unknown_58;
        unsigned int delay = unknown_random_839218(lo, hi);
        if (delay < 0x21) {
            Audio_Channel_Set_Source(channel, source);
            return 0;
        }
        event->unknown_13c = delay;
        event->unknown_148 = source;
    }
    return 0;
}


// AudioChannel *__fastcall Audio_Event_Start_Channel(int priority, unsigned int flags) -- 0x00405B60
// Purpose: takes a channel from the event device for a new event, unless the
// channel is busy with something of a higher priority.

AudioChannel *__fastcall Audio_Event_Start_Channel(int priority, unsigned int flags)
{
    AudioChannel *channel = 0;

    if (Audio_Event_Device != 0) {
        channel = Audio_Device_Find_Channel(Audio_Event_Device, 1);
        if (channel != 0) {
            if ((channel->Flags & 0x15) != 0 && channel->unknown_a0 > priority) {
                return 0;
            }
            Audio_Channel_Stop(channel);
            channel->unknown_levels_94 = Audio_Global_Levels[3];
            channel->unknown_levels_9c = Audio_Global_Levels[7];
            channel->unknown_a0 = priority;
            if ((flags & 1) != 0) {
                channel->LoopCount = -1;
                return channel;
            }
            channel->LoopCount = 0;
        }
    }
    return channel;
}


// void __fastcall Audio_Event_Handle_Stop_Clear(AudioEventHandle *handle) -- 0x00405c10
// Purpose: stops the event a handle stands for (when the handle is still
// good) and also clears the handle's token.

void __fastcall Audio_Event_Handle_Stop_Clear(AudioEventHandle *handle)
{
    if (Audio_Events_Ready != 0 && handle->Token == &Audio_Event_Bag && handle->Event != 0) {
        AudioEvent *event = handle->Event;
        if (handle->Type == event->Type && handle->Serial == event->unknown_138
            && !(event->Flags & AUDIO_EVENT_STOPPED)) {
            AUDIO_EVENT_DETACH_INLINE(event)
            AUDIO_EVENT_RELEASE_SAMPLES_INLINE(event)
            AUDIO_EVENT_MARK_STOPPED(event)
        }
        handle->Event = 0;
    }
    handle->Type = 0;
    handle->Token = 0;
}


// void __fastcall Audio_Event_Handle_Stop(AudioEventHandle *handle) -- 0x00405d50
// Purpose: stops the event a handle stands for (when the handle is still
// good) and forgets the event and type.

void __fastcall Audio_Event_Handle_Stop(AudioEventHandle *handle)
{
    if (Audio_Events_Ready != 0 && handle->Token == &Audio_Event_Bag && handle->Event != 0) {
        AudioEvent *event = handle->Event;
        if (handle->Type == event->Type && handle->Serial == event->unknown_138
            && !(event->Flags & AUDIO_EVENT_STOPPED)) {
            AUDIO_EVENT_DETACH_INLINE(event)
            AUDIO_EVENT_RELEASE_SAMPLES_INLINE(event)
            AUDIO_EVENT_MARK_STOPPED(event)
        }
        handle->Event = 0;
    }
    handle->Type = 0;
}


// void __fastcall Audio_Event_Handle_Stop_Loop(AudioEventHandle *handle) -- 0x00405E90
// Purpose: stops the handle's event when its type loops forever (flag 1 and no
// loop count) and forgets the event; other events keep the handle's event.

void __fastcall Audio_Event_Handle_Stop_Loop(AudioEventHandle *handle)
{
    AudioEvent *event = Audio_Event_Resolve(handle);
    if (event != 0 && (event->Type->Flags & AUDIO_EVENT_TYPE_LOOPS) && event->Type->LoopCount == 0) {
        Audio_Event_Stop(event);
        handle->Event = 0;
    }
    handle->Type = 0;
}


// void __fastcall Audio_Event_Handle_Finish(AudioEventHandle *handle) -- 0x00405FE0
// Purpose: lets the handle's event finish (it stops after the sample being
// played) and forgets the event; a stale handle just forgets it.

void __fastcall Audio_Event_Handle_Finish(AudioEventHandle *handle)
{
    AudioEvent *event = Audio_Event_Resolve(handle);
    if (event != 0) {
        Audio_Event_Finish(event);
        handle->Event = 0;
    }
    handle->Type = 0;
}


// void __fastcall Audio_Event_Handle_Finish_Loop(AudioEventHandle *handle) -- 0x00406070
// Purpose: as Audio_Event_Handle_Finish for an event whose type loops forever
// (flag 1 and no loop count); other events keep the handle's event.

void __fastcall Audio_Event_Handle_Finish_Loop(AudioEventHandle *handle)
{
    AudioEvent *event = Audio_Event_Resolve(handle);
    if (event != 0 && (event->Type->Flags & AUDIO_EVENT_TYPE_LOOPS) && event->Type->LoopCount == 0) {
        Audio_Event_Finish(event);
        handle->Event = 0;
    }
    handle->Type = 0;
}


// 0x00406100: function not yet identified
//
// Body: 52 bytes (frozen inventory bound).
// Referenced from 8 sites (unconditional call).
// Ghidra prototype: void __fastcall Audio_Event_Handle_Set_00406100(int *param_1,int param_2,int param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406100 {

extern "C" unsigned int Audio_Event_Bag_00830fac;

void __fastcall Audio_Event_Handle_Set_00406100(int *handle, int event, int type)
{
    if ((unsigned int *)handle[3] == &Audio_Event_Bag_00830fac) {
        *handle = event;
        handle[2] = type;
        if (event != 0) {
            handle[1] = *(int *)(event + 0x138);
            if (type == 0) {
                handle[2] = *(int *)(event + 0x24);
            }
            *(int **)(event + 0x278) = handle;
        }
    }
    return;
}

}  // namespace seed_00406100


// 0x00406140: function not yet identified
//
// Body: 57 bytes (frozen inventory bound).
// Referenced from 9 sites (unconditional call).
// Ghidra prototype: int __fastcall Audio_Event_Handle_Event_00406140(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406140 {

extern "C" unsigned int Audio_Event_Bag_00830fac;
extern "C" unsigned int Audio_Events_Ready_00830fb8;

int __fastcall Audio_Event_Handle_Event_00406140(int *handle)
{
    int event;

    if ((Audio_Events_Ready_00830fb8 != 0) && ((unsigned int *)handle[3] == &Audio_Event_Bag_00830fac)) {
        event = (int)(*handle);
        if ((event != 0) &&
              ((handle[2] != *(int *)(event + 0x24) || (handle[1] != *(int *)(event + 0x138))))) {
            event = (int)(0);
            *handle = 0;
        }
        return event;
    }
    return 0;
}

}  // namespace seed_00406140


// 0x00406180: function not yet identified
//
// Body: 25 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: undefined4 __fastcall Audio_Event_Handle_Type_00406180(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406180 {

extern "C" unsigned int Audio_Event_Bag_00830fac;
extern "C" unsigned int Audio_Events_Ready_00830fb8;

unsigned int __fastcall Audio_Event_Handle_Type_00406180(int handle)
{
    if ((Audio_Events_Ready_00830fb8 != 0) && (*(unsigned int **)(handle + 0xc) == &Audio_Event_Bag_00830fac)) {
        return *(unsigned int *)(handle + 8);
    }
    return 0;
}

}  // namespace seed_00406180


// void __fastcall Audio_Event_Jump_Volume(AudioEvent *event, int volume) -- 0x004061A0
// Purpose: sets the event's volume (0..0x4000, scaled by its type's volume) at
// once instead of ramping to it.

void __fastcall Audio_Event_Jump_Volume(AudioEvent *event, int volume)
{
    int value = volume;
    if (value > 0x4000u) {
        value = 0x4000;
    } else if (value < 0) {
        value = 0;
    }
    event->Levels.Volume.Set((event->Type->VolumeLevel.Current >> 16) * value >> 14);
    event->Levels.Volume.Update();
}


// void __fastcall Audio_Event_Set_Volume(AudioEvent *event, int volume) -- 0x004061E0
// Purpose: sets the volume (0..0x4000) of an event, scaled by the event
// type's own volume: ramped while fading (state 3), at once otherwise.

void __fastcall Audio_Event_Set_Volume(AudioEvent *event, int volume)
{
    if (event->State == 3) {
        int value = volume;
        if (value > 0x4000u) {
            value = 0x4000;
        } else if (value < 0) {
            value = 0;
        }
        event->Levels.Volume.Set_Target((event->Type->VolumeLevel.Current >> 16) * value >> 14);
    } else {
        int value = volume;
        if (value > 0x4000u) {
            value = 0x4000;
        } else if (value < 0) {
            value = 0;
        }
        event->Levels.Volume.Set((event->Type->VolumeLevel.Current >> 16) * value >> 14);
        event->Levels.Volume.Update();
    }
}


// void __fastcall Audio_Event_Jump_Pan(AudioEvent *event, int pan) -- 0x00406260
// Purpose: sets the event's pan (0..0x4000) at once instead of ramping to it.

void __fastcall Audio_Event_Jump_Pan(AudioEvent *event, int pan)
{
    if (pan > 0x4000u) {
        pan = 0x4000;
    } else if (pan < 0) {
        pan = 0;
    }
    event->Levels.Pan.Set(pan);
}


// void __fastcall Audio_Event_Set_Pan(AudioEvent *event, int pan) -- 0x00406280
// Purpose: sets the pan (0..0x4000) of an event: ramped while the event is
// fading (state 3), at once otherwise.

void __fastcall Audio_Event_Set_Pan(AudioEvent *event, int pan)
{
    if (event->State == 3) {
        int value = pan;
        if (value > 0x4000u) {
            value = 0x4000;
        } else if (value < 0) {
            value = 0;
        }
        event->Levels.Pan.Set_Target(value);
    } else {
        int value = pan;
        if (value > 0x4000u) {
            value = 0x4000;
        } else if (value < 0) {
            value = 0;
        }
        event->Levels.Pan.Set(value);
    }
}


// void __fastcall Audio_Event_Set_Level_Rates(AudioEvent *event, __int64 period) -- 0x004062D0
// Purpose: makes the event's volume, frequency and pan levels ramp over a full
// step (0x4000) per period milliseconds.

void __fastcall Audio_Event_Set_Level_Rates(AudioEvent *event, __int64 period)
{
    event->Levels.Volume.Set_Rate(period, 0x4000);
    event->Levels.Frequency.Set_Rate(period, 0x4000);
    event->Levels.Pan.Set_Rate(period, 0x4000);
}


class Leaf_406320 { public: unsigned int get(); };
unsigned int Leaf_406320::get() { return *(unsigned int*)((char*)this + 0x24); }


// 0x00406330: function not yet identified
//
// Body: 25 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall Audio_Event_Loops_Forever_00406330(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406330 {

unsigned int __fastcall Audio_Event_Loops_Forever_00406330(int event)
{
    if (((*(unsigned char *)(*(int *)(event + 0x24) + 0x10) & 1) != 0) &&
          (*(int *)(*(int *)(event + 0x24) + 0x4c) == 0)) {
        return 1;
    }
    return 0;
}

}  // namespace seed_00406330


// 0x00406350: function not yet identified
//
// Body: 32 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: char * __fastcall Audio_Event_Name_00406350(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406350 {

extern "C" unsigned int Audio_Events_Ready_00830fb8;
extern "C" char s_no_events_007cccd4[];
extern "C" char s_unknown_007ccce0[];

char * __fastcall Audio_Event_Name_00406350(int event)
{
    if (Audio_Events_Ready_00830fb8 == 0) {
        return s_no_events_007cccd4;
    }
    if (*(int *)(event + 0x24) == 0) {
        return s_unknown_007ccce0;
    }
    return (char *)(*(int *)(event + 0x24) + 0x6c);
}

}  // namespace seed_00406350


// AudioEventType *__fastcall Audio_Event_Type_Lookup(char const *name) -- 0x00406370
// The event type with this name, or 0 (unreferenced; Audio_Event_Type_Find 0x004063C0 makes a
// missing one instead).

AudioEventType *__fastcall Audio_Event_Type_Lookup(char const *name)
{
    AudioEventType *type;

    if (Audio_Events_Ready == 0) {
        return 0;
    }
    type = (AudioEventType *)Audio_List_First(&Audio_Event_Type_List);
    while (type != 0) {
        if (_stricmp(name, type->Name) == 0) {
            return type;
        }
        type = (AudioEventType *)Audio_List_Next(type);
    }
    return 0;
}


// AudioEventType *__fastcall Audio_Event_Type_Find(char const *name) -- 0x004063C0
// Purpose: finds the event type with this name, or makes a new one with the
// default settings and adds it to the type list.

extern "C" int __cdecl _stricmp(char const *left, char const *right);
extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);
void *__cdecl operator new(unsigned int size);

AudioEventType *__fastcall Audio_Event_Type_Find(char const *name)
{
    AudioEventType *type;

    if (Audio_Events_Ready == 0) {
        return 0;
    }
    type = (AudioEventType *)Audio_List_First(&Audio_Event_Type_List);
    for (;;) {
        if (type == 0) {
        make:
            type = (AudioEventType *)operator new(sizeof(AudioEventType));
            memset(type, 0, sizeof(AudioEventType));
            memset(type, 0, sizeof(AudioEventType));
            Audio_List_Init_Node(type);
            type->unknown_0c = 1;
            type->Flags = 0;
            type->unknown_14 = 0x20;
            type->unknown_48 = 3;
            type->unknown_40 = 2;
            type->unknown_50 = 10;
            type->unknown_54 = 0.2f;
            type->VolumeLevel.Init(0x4000);
            strncpy(type->Name, name, sizeof(type->Name) - 1);
            type->Name[sizeof(type->Name) - 1] = 0;
            Audio_List_Add_Tail(&Audio_Event_Type_List, type);
            return type;
        }
        if (_stricmp(name, type->Name) == 0) {
            if (type != 0) {
                return type;
            }
            goto make;
        }
        type = (AudioEventType *)Audio_List_Next(type);
    }
}


// int __fastcall Audio_Event_Type_Add_Sample(AudioEventType *type, char const *name) -- 0x004064B0
// Purpose: adds the bag entry called name (leading '$' and '#' skipped) to the
// sample list of an event type. A missing sample is reported and makes the
// type unusable. Returns 0 only when the list is full.

int __fastcall Audio_Event_Type_Add_Sample(AudioEventType *type, char const *name)
{
    if (type->SampleCount == 0x20) {
        return 0;
    }
    if (Audio_Events_Ready != 0 && Audio_Event_Bag != 0) {
        while (*name == '$' || *name == '#') {
            name++;
        }
        int index = Audio_Bag_Find(Audio_Event_Bag, name);
        if (index != -1) {
            type->Samples[type->SampleCount] = index;
            type->SampleCount++;
            return 1;
        }
        Debug_Printf("Missing sample %s\n", name);
        type->unknown_0c = 0;
    }
    return 1;
}


// void __fastcall Audio_Event_Type_Set_Intro(AudioEventType *type, int count) -- 0x00406530
// Purpose: stores count in the type's Intro setting.

void __fastcall Audio_Event_Type_Set_Intro(AudioEventType *type, int count)
{
    type->Intro = count;
}


// void __fastcall Audio_Event_Type_Set_Outro(AudioEventType *type, int count) -- 0x00406540
// Purpose: stores count in the type's Outro setting.

void __fastcall Audio_Event_Type_Set_Outro(AudioEventType *type, int count)
{
    type->Outro = count;
}


// void __fastcall Audio_Event_Type_Set_Priority(AudioEventType *type, int priority) -- 0x00406550
// Purpose: stores priority in the type's unknown_40 setting.

void __fastcall Audio_Event_Type_Set_Priority(AudioEventType *type, int priority)
{
    type->unknown_40 = priority;
}


// void __fastcall Audio_Event_Type_Set_Volume(AudioEventType *type, int volume) -- 0x00406560
// Purpose: sets the type's own volume level at once.

void __fastcall Audio_Event_Type_Set_Volume(AudioEventType *type, int volume)
{
    type->VolumeLevel.Set(volume);
    type->VolumeLevel.Update();
}


// void __fastcall Audio_Event_Type_Set_Flags(AudioEventType *type, unsigned int flags) -- 0x00406580
// Purpose: sets the type's flags; the intro and outro counts follow the
// flags that need them (at least 1 when set, 0 when not).

void __fastcall Audio_Event_Type_Set_Flags(AudioEventType *type, unsigned int flags)
{
    type->Flags = flags;
    if (flags & 0x20) {
        if (type->Intro == 0) {
            type->Intro = 1;
        }
    } else {
        type->Intro = 0;
    }
    if (flags & 0x40) {
        if (type->Outro == 0) {
            type->Outro = 1;
        }
    } else {
        type->Outro = 0;
    }
}


// void __fastcall Audio_Event_Type_Set_Type(AudioEventType *type, unsigned int flags) -- 0x004065D0
// Purpose: stores flags in the type's unknown_14 setting.

void __fastcall Audio_Event_Type_Set_Type(AudioEventType *type, unsigned int flags)
{
    type->unknown_14 = flags;
}


// void __fastcall Audio_Event_Type_Set_Limit(AudioEventType *type, int limit) -- 0x004065E0
// Purpose: stores limit in the type's unknown_48 setting.

void __fastcall Audio_Event_Type_Set_Limit(AudioEventType *type, int limit)
{
    type->unknown_48 = limit;
}


// void __fastcall Audio_Event_Type_Set_Range(AudioEventType *type, int range) -- 0x004065F0
// Purpose: stores range in the type's unknown_50 setting.

void __fastcall Audio_Event_Type_Set_Range(AudioEventType *type, int range)
{
    type->unknown_50 = range;
}


// void __fastcall Audio_Event_Type_Set_Min_Volume(AudioEventType *type, float volume) -- 0x00406600
// Purpose: stores volume in the type's unknown_54 setting.

void __fastcall Audio_Event_Type_Set_Min_Volume(AudioEventType *type, float volume)
{
    type->unknown_54 = volume;
}


// void __fastcall Audio_Event_Type_Set_Delay(AudioEventType *type, int low, int high) -- 0x00406610
// Purpose: stores the low and high ends of the type's delay range (unknown_58, unknown_5c).

void __fastcall Audio_Event_Type_Set_Delay(AudioEventType *type, int low, int high)
{
    type->unknown_58 = low;
    type->unknown_5c = high;
}


// void __fastcall Audio_Event_Type_Set_FShift(AudioEventType *type, int low, int high) -- 0x00406620
// Purpose: stores the low and high ends of the type's pause between loops range (unknown_60, unknown_64).

void __fastcall Audio_Event_Type_Set_FShift(AudioEventType *type, int low, int high)
{
    type->unknown_60 = low;
    type->unknown_64 = high;
}


// void __fastcall Audio_Event_Type_Set_Percent(AudioEventType *type, int percent) -- 0x00406630
// Purpose: sets the +0x68 setting of the type, kept within 0..100.

void __fastcall Audio_Event_Type_Set_Percent(AudioEventType *type, int percent)
{
    if (percent > 100) {
        percent = 100;
    } else if (percent < 0) {
        percent = 0;
    }
    type->unknown_68 = percent;
}


// void __fastcall Audio_Event_Type_Set_Loop(AudioEventType *type, int count) -- 0x00406650
// Purpose: stores count in the type's LoopCount setting.

void __fastcall Audio_Event_Type_Set_Loop(AudioEventType *type, int count)
{
    type->LoopCount = count;
}


// 0x00406660: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: undefined4 __fastcall Audio_Event_Type_Loops_Forever_00406660(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406660 {

unsigned int __fastcall Audio_Event_Type_Loops_Forever_00406660(int type)
{
    unsigned int result;

    result = (unsigned int)(1);
    if (((*(unsigned char *)(type + 0x10) & 1) == 0) || (*(int *)(type + 0x4c) != 0)) {
        result = (unsigned int)(0);
    }
    return result;
}

}  // namespace seed_00406660


// void __fastcall Audio_Event_Play_Named(char const *name, int volume) -- 0x00406680
// Purpose: starts an event of the type with this name and sets its volume
// (volume is in 1/256ths of full).

extern "C" int __cdecl _stricmp(char const *left, char const *right);

void __fastcall Audio_Event_Play_Named(char const *name, int volume)
{
    if (Audio_Events_Ready != 0) {
        AudioEventType *type = (AudioEventType *)Audio_List_First(&Audio_Event_Type_List);
        if (type != 0) {
            while (type != 0 && _stricmp(name, type->Name) != 0) {
                type = (AudioEventType *)Audio_List_Next(type);
            }
            AudioEvent *event;
            if (type != 0 && (event = Audio_Event_Create(type)) != 0) {
                int level = (int)((__int64)volume * 0x4000 / 0x100);
                if (level > 0x4000u) {
                    level = 0x4000;
                } else if (level < 0) {
                    level = 0;
                }
                event->Levels.Volume.Set((event->Type->VolumeLevel.Current >> 16) * level >> 14);
                event->Levels.Volume.Update();
            }
        }
    }
}


// 0x00406740: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void Audio_Disable_00406740(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406740 {

extern "C" unsigned int Audio_Enabled_007ccbd8;

void  Audio_Disable_00406740(void)
{
    Audio_Enabled_007ccbd8 = 0;
    return;
}

}  // namespace seed_00406740


// 0x00406750: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void Audio_Enable_00406750(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406750 {

extern "C" unsigned int Audio_Enabled_007ccbd8;

void  Audio_Enable_00406750(void)
{
    Audio_Enabled_007ccbd8 = 1;
    return;
}

}  // namespace seed_00406750


// 0x00406760: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 Audio_Is_Enabled_00406760(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406760 {

extern "C" unsigned int Audio_Enabled_007ccbd8;

unsigned int  Audio_Is_Enabled_00406760(void)
{
    return Audio_Enabled_007ccbd8;
}

}  // namespace seed_00406760


// void __fastcall Audio_Report_Usage(void (__fastcall *report)(char *text)) -- 0x00406770
// Hands the callback two lines: the audio event count against its maximum and
// the static sound count against its maximum (nothing before the event bank exists).

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

void __fastcall Audio_Report_Usage(void (__fastcall *report)(char *text))
{
    char text[200];
    if (Audio_Event_Bank != 0) {
        sprintf(text, "AudioEvents : %05d - Max: %05d", Audio_Event_Count, Audio_Event_Maximum);
        report(text);
        sprintf(text, "StaticSounds: %05d - Max: %05d", Audio_Static_Sound_Count, Audio_Static_Sound_Maximum);
        report(text);
    }
}


// bool __fastcall Audio_Priority_From_Name(char const *name, int *priority) -- 0x004067E0
// Purpose: looks the priority name up (case-insensitive) in the table at
// 0x007CCAE8, which ends with a null name; stores its priority (the
// terminator's when the name is not there) and returns whether it was found.

extern "C" int __cdecl _stricmp(char const *left, char const *right);

struct AudioPriorityName {
    char const *Name;
    int Priority;
};
extern AudioPriorityName Audio_Priority_Names[];        // 0x007CCAE8

bool __fastcall Audio_Priority_From_Name(char const *name, int *priority)
{
    AudioPriorityName *entry = Audio_Priority_Names;

    while (entry->Name != 0 && _stricmp(name, entry->Name) != 0) {
        entry++;
    }
    if (priority != 0) {
        *priority = entry->Priority;
    }
    return entry->Name != 0;
}


// bool __fastcall Audio_Control_Apply_Keyword(unsigned int *flags, char const *name) -- 0x00406830
// Purpose: looks the control keyword up (case-insensitive) in the table at
// 0x007CCB90, which ends with a null name; turns its flags on (the
// terminator's when the keyword is not there) and returns whether it was found.

extern "C" int __cdecl _stricmp(char const *left, char const *right);

extern AudioEventKeyword Audio_Control_Keywords[];      // 0x007CCB90

bool __fastcall Audio_Control_Apply_Keyword(unsigned int *flags, char const *name)
{
    AudioEventKeyword *entry = Audio_Control_Keywords;

    while (entry->Name != 0 && _stricmp(name, entry->Name) != 0) {
        entry++;
    }
    if (flags != 0) {
        *flags |= entry->Flags;
    }
    return entry->Name != 0;
}


// int __fastcall Audio_Event_Apply_Keyword(unsigned int *flags, char const *name) -- 0x00406880
// Purpose: looks the keyword up in the table and, when there are flags to
// change, turns its flags on (dropping the group it replaces: 0x60 or 0xC00).
// Returns whether the keyword exists.

extern "C" int __cdecl _stricmp(char const *left, char const *right);

int __fastcall Audio_Event_Apply_Keyword(unsigned int *flags, char const *name)
{
    AudioEventKeyword *entry = Audio_Event_Keywords;

    while (entry->Name != 0 && _stricmp(name, entry->Name) != 0) {
        entry++;
    }
    if (flags != 0) {
        if (entry->Flags & 0x60) {
            *flags &= ~0x60;
        } else if (entry->Flags & 0xc00) {
            *flags &= ~0xc00;
        }
        *flags |= entry->Flags;
    }
    return entry->Name != 0;
}


class Leaf_4068f0 { public: void get(); };
void Leaf_4068f0::get() {  }
