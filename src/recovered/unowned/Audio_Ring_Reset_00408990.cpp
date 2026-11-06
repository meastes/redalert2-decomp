// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Audio_Ring_Reset(AudioStream *ring) -- 0x00408990
// From ra2-decomp STM_StreamReset and its static helpers (YR 0x00408A70).
// Purpose: gives every node its whole buffer as both windows and numbers
// the nodes, then starts both queues at the first node (Queued and
// Consumed -1001, nothing pending) with the whole ring queued for
// filling. The helper chain is the twin's: its depth keeps
// Audio_Queue_Reset out of line in Audio_Ring_Destroy 0x00408760.
#include "shared/audio.hpp"

static void Audio_Queue_Reset_Block(AudioQueue *queue)
{
    Audio_Queue_Reset(queue);
}

static void Audio_Queue_Reset_Position(AudioQueue *queue)
{
    queue->Position = 0;
    Audio_Queue_Reset_Block(queue);
}

static void Audio_Queue_Reset_Counts(AudioQueue *queue)
{
    queue->Queued = -1001;
    queue->Consumed = queue->Queued;
    Audio_Queue_Reset_Position(queue);
}

static void Audio_Queue_Restart(AudioQueue *queue, AudioStreamNode *first)
{
    queue->CommitNode = first;
    Audio_Queue_Reset_Counts(queue);
}

static void Audio_Ring_Restart_Queues(AudioStream *ring, AudioStreamNode *first)
{
    for (int side = 0; side < 2; side++) {
        Audio_Queue_Restart(&ring->Queues[side], first);
    }
    ring->Queues[0].Queued += ring->Size;
}

static void Audio_Ring_Restart(AudioStream *ring)
{
    AudioStreamNode *first = Audio_Ring_Number_Nodes(ring);
    Audio_Ring_Restart_Queues(ring, first);
}

void __fastcall Audio_Ring_Reset(AudioStream *ring)
{
    Audio_Ring_Restart(ring);
}
