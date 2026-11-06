// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int __fastcall Audio_Queue_Update(AudioQueue *queue, int limit) -- 0x004090E0
// From ra2-decomp STM_AccessAdvance (YR 0x004091C0).
// Purpose: advances the queue by up to limit of its available bytes, node by
// node (the at-head flag is set on wrapping round to the commit node),
// refreshing the contiguous window; counts them pending, moves the ring
// position, and commits at once unless the queue leaves commits to its
// caller. Returns the bytes advanced.
// @ref ?Audio_Queue_Available@@YIHPAUAudioQueue@@@Z=00409240
// @ref ?Audio_Queue_Commit@@YIHPAUAudioQueue@@@Z=00409020
#include "shared/audio.hpp"

static AudioStreamNode *Audio_Ring_Next_Node(AudioStreamNode *node)
{
    AudioStream *ring;
    AudioStreamNode *head;

    ring = node->Ring;
    head = (AudioStreamNode *)&ring->Nodes;
    node = (AudioStreamNode *)node->Next;
    if (node == head) {
        node = (AudioStreamNode *)node->Next;
        if (node == head) {
            return 0;
        }
    }
    return node;
}

int __fastcall Audio_Queue_Update(AudioQueue *queue, int limit)
{
    int advanced = 0;
    int available = Audio_Queue_Available(queue) - queue->Pending;

    if (!available) {
        queue->Available = 0;
        return 0;
    }
    if (available < limit) {
        limit = available;
    }
    for (;;) {
        AudioStreamWindow *window = &queue->Node->Windows[queue->Index];
        if (queue->Flags & AUDIO_QUEUE_AT_HEAD) {
            queue->Available = available;
        } else {
            int bytes = window->Length - (queue->Pointer - window->Pointer);
            if (bytes < available) {
                queue->Available = bytes;
            } else {
                queue->Available = available;
            }
        }
        int advance = limit;
        if (!advance) {
            break;
        }
        char *pointer;
        if (advance >= queue->Available) {
            advance = queue->Available;
            queue->Node = Audio_Ring_Next_Node(queue->Node);
            if (queue->Node == queue->CommitNode) {
                queue->Flags |= AUDIO_QUEUE_AT_HEAD;
                pointer = queue->Node->Base;
            } else {
                pointer = queue->Node->Windows[queue->Index].Pointer;
            }
        } else {
            pointer = queue->Pointer + advance;
        }
        queue->Pointer = pointer;
        limit -= advance;
        advanced += advance;
        available -= advance;
    }

    queue->Pending += advanced;
    queue->Position += advanced;
    if ((unsigned int)queue->Position >= (unsigned int)queue->Stream->Size) {
        queue->Position %= (unsigned int)queue->Stream->Size;
    }
    if (queue->Pending && queue->unknown_18 == 0) {
        Audio_Queue_Commit(queue);
    }
    return advanced;
}
