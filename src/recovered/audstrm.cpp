// audstrm.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audstrm.json carries the addresses and the rows that
// stay in their own files.


#include "shared/crt_memory.hpp"
#include "shared/audio.hpp"
#include "shared/debug_printf.hpp"
#include "shared/raw_file_class.hpp"


// AudioStream *Audio_Ring_Create() -- 0x00408710
// Purpose: makes an empty, zero-filled ring with its two queues (0 fills,
// 1 drains).

void *__cdecl operator new(unsigned int size);

AudioStream *Audio_Ring_Create()
{
    AudioStream *ring = (AudioStream *)operator new(sizeof(AudioStream));
    memset(ring, 0, sizeof(AudioStream));
    Audio_List_Init(&ring->Nodes);
    ring->Users = 0;
    for (int index = 0; index < 2; index++) {
        AudioQueue *queue = &ring->Queues[index];
        queue->Stream = ring;
        queue->Users = 0;
        queue->Error = 0;
        queue->Index = index;
        queue->unknown_18 = 0;
    }
    return ring;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Audio_Ring_Destroy(AudioStream *ring) -- 0x00408760
// From ra2-decomp STM_StreamDestroy (YR 0x00408840).
// Purpose: frees every node of the ring, then the ring itself
// (Audio_Ring_Clear expanded inline).

void __cdecl operator delete(void *block);

void __fastcall Audio_Ring_Destroy(AudioStream *ring)
{
    Audio_Ring_Clear(ring);
    operator delete(ring);
}


// void __fastcall Audio_Queue_Reset(AudioQueue *queue) -- 0x00408850
// Purpose: moves the queue back to its commit node, dropping what was
// pending.

void __fastcall Audio_Queue_Reset(AudioQueue *queue)
{
    queue->Pending = 0;
    queue->Node = queue->CommitNode;
    if (queue->Node != 0) {
        queue->Pointer = queue->Node->Windows[queue->Index].Pointer;
    } else {
        queue->Pointer = 0;
    }
    queue->Available = 0;
    queue->Flags &= ~AUDIO_QUEUE_AT_HEAD;
}


// void __fastcall Audio_Ring_Clear(AudioStream *ring) -- 0x00408890
// Purpose: removes and frees every node of the ring, then resets it
// (Audio_Ring_Node_Remove, Audio_Ring_Node_Destroy and Audio_Ring_Reset
// expanded inline).

void __fastcall Audio_Ring_Clear(AudioStream *ring)
{
    AudioStreamNode *node;
    while ((node = (AudioStreamNode *)ring->Nodes.Next) != (AudioStreamNode *)&ring->Nodes) {
        Audio_Ring_Node_Remove(node);
        Audio_Ring_Node_Destroy(node);
    }
    Audio_Ring_Reset(ring);
}


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


// void __fastcall Audio_Ring_Add_Node(AudioStream *ring, AudioStreamNode *node) -- 0x00408A30
// Purpose: links node in at the head of the ring and counts its size.

void __fastcall Audio_Ring_Add_Node(AudioStream *ring, AudioStreamNode *node)
{
    Audio_List_Add_Head(&ring->Nodes, node);
    ring->NodeCount++;
    ring->Size += node->Size;
    node->Ring = ring;
    ring->Flags &= ~AUDIO_RING_FLAG_2;
}


// int __fastcall Audio_Ring_Add_Nodes(AudioStream *ring, int count, int size, int shift) -- 0x00408A60
// Purpose: adds count nodes of size bytes aligned to 1 << shift
// (Audio_Ring_Node_Create and Audio_Ring_Add_Node expanded inline).
// Returns how many were added.

int __fastcall Audio_Ring_Add_Nodes(AudioStream *ring, int count, int size, int shift)
{
    int added = 0;
    while (count-- != 0) {
        AudioStreamNode *node = Audio_Ring_Node_Create(size, shift);
        if (node == 0) {
            break;
        }
        Audio_Ring_Add_Node(ring, node);
        added++;
    }
    return added;
}


// AudioQueue *__fastcall Audio_Ring_Acquire(AudioStream *ring, int index) -- 0x00408B50
// Purpose: acquires queue index of the ring (0 "Write", 1 "Read"); the
// first acquisition resets the ring. Returns 0 (reported) when that queue
// is already acquired.


AudioQueue *__fastcall Audio_Ring_Acquire(AudioStream *ring, int index)
{
    AudioQueue *queue = &ring->Queues[index];
    if (queue->Users != 0) {
        Debug_Printf("%s access to stream failed. Access already acquired.\n", queue->Index == 0 ? "Write" : "Read");
        return 0;
    }
    if (ring->Users == 0) {
        Audio_Ring_Reset(ring);
    }
    queue->Users++;
    ring->Users++;
    return queue;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int __fastcall Audio_Ring_Drain_Available(AudioStream *ring) -- 0x00408C50
// From ra2-decomp STM_StreamTotalBytesIn (YR 0x00408D30).
// Purpose: bytes queued for draining that are not yet consumed.

int __fastcall Audio_Ring_Drain_Available(AudioStream *ring)
{
    return Audio_Queue_Available(&ring->Queues[1]);
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int __fastcall Audio_Ring_Fill_Available(AudioStream *ring) -- 0x00408C60
// From ra2-decomp STM_StreamTotalBytesTillFull (YR 0x00408D40).
// Purpose: free bytes queued for filling that are not yet consumed.

int __fastcall Audio_Ring_Fill_Available(AudioStream *ring)
{
    return Audio_Queue_Available(&ring->Queues[0]);
}


// int __fastcall Audio_Ring_Size(AudioStream *ring) -- 0x00408C70
// Purpose: the ring's size in bytes.

int __fastcall Audio_Ring_Size(AudioStream *ring)
{
    return ring->Size;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int __fastcall Audio_Ring_Is_Full(AudioStream *ring) -- 0x00408C80
// From ra2-decomp STM_StreamFull (YR 0x00408D60).
// Purpose: whether the fill queue has no free bytes left.

int __fastcall Audio_Ring_Is_Full(AudioStream *ring)
{
    return Audio_Ring_Fill_Available(ring) == 0;
}


// AudioStreamNode *__fastcall Audio_Ring_Node_Create(int size, int shift) -- 0x00408CA0
// Purpose: makes an unlinked node with a buffer of size bytes aligned to
// 1 << shift. Returns 0 when the buffer cannot be allocated.

void *__cdecl operator new(unsigned int size);
void __cdecl operator delete(void *block);

AudioStreamNode *__fastcall Audio_Ring_Node_Create(int size, int shift)
{
    AudioStreamNode *node = (AudioStreamNode *)operator new(sizeof(AudioStreamNode));
    memset(node, 0, sizeof(AudioStreamNode));
    Audio_List_Init_Node(node);
    node->Data = operator new((1 << shift) + size);
    if (node->Data == 0) {
        if (node != 0) {
            operator delete(node);
        }
        return 0;
    }
    node->Shift = shift;
    node->Base = (char *)(((1 << shift) + (unsigned long)node->Data - 1) & (-1 << shift));
    node->Size = size;
    return node;
}


// void __fastcall Audio_Ring_Node_Destroy(AudioStreamNode *node) -- 0x00408D20
// Purpose: frees the node's buffer and the node.

void __cdecl operator delete(void *block);

void __fastcall Audio_Ring_Node_Destroy(AudioStreamNode *node)
{
    if (node->Data != 0) {
        operator delete(node->Data);
    }
    operator delete(node);
}


// void __fastcall Audio_Ring_Node_Remove(AudioStreamNode *node) -- 0x00408D40
// Purpose: unlinks the node from its ring and uncounts its size.

void __fastcall Audio_Ring_Node_Remove(AudioStreamNode *node)
{
    Audio_List_Remove(node);
    AudioStream *ring = node->Ring;
    node->Ring = 0;
    ring->NodeCount--;
    ring->Size -= node->Size;
    ring->Flags &= ~AUDIO_RING_FLAG_2;
}


// AudioStreamNode *__fastcall Audio_Ring_Node_Next(AudioStreamNode *node) -- 0x00408D70
// Purpose: the next node round the ring (AudioStreamNode::Ring_Next).

AudioStreamNode *__fastcall Audio_Ring_Node_Next(AudioStreamNode *node)
{
    return node->Ring_Next();
}


// void __fastcall Audio_Queue_Release(AudioQueue *queue) -- 0x00408D90
// Purpose: drops an acquisition of the queue and of its ring (each
// floored at 0).

void __fastcall Audio_Queue_Release(AudioQueue *queue)
{
    if (--queue->Users < 0) {
        queue->Users = 0;
    }
    if (--queue->Stream->Users < 0) {
        queue->Stream->Users = 0;
    }
}


// int __fastcall Audio_Queue_Set_18(AudioQueue *queue, int value) -- 0x00408DD0
// Purpose: sets +0x18 (nonzero: the caller commits). Returns 0.

int __fastcall Audio_Queue_Set_18(AudioQueue *queue, int value)
{
    queue->unknown_18 = value;
    return 0;
}


// int __fastcall Audio_Queue_Get_18(AudioQueue *queue) -- 0x00408DE0
// Purpose: returns +0x18.

int __fastcall Audio_Queue_Get_18(AudioQueue *queue)
{
    return queue->unknown_18;
}


// int __fastcall Audio_Queue_Index(AudioQueue *queue) -- 0x00408DF0
// Purpose: the queue's side (0 fill, 1 drain).

int __fastcall Audio_Queue_Index(AudioQueue *queue)
{
    return queue->Index;
}


// int __fastcall Audio_Queue_Copy(AudioQueue *queue, void *buffer, int bytes) -- 0x00408E00
// Purpose: moves up to bytes between buffer and the ring through the queue
// (into the ring for the fill side, out of it for the drain side),
// advancing the queue. Returns the bytes moved.

int __fastcall Audio_Queue_Copy(AudioQueue *queue, void *buffer, int bytes)
{
    int moved = 0;
    char *pointer = (char *)buffer;
    Audio_Queue_Update(queue, 0);
    for (;;) {
        int count = queue->Available;
        if (count > bytes) {
            count = bytes;
        }
        if (count == 0) {
            break;
        }
        if (queue->Index != 0) {
            memcpy(pointer, queue->Pointer, count);
        } else {
            memcpy(queue->Pointer, pointer, count);
        }
        Audio_Queue_Update(queue, count);
        pointer += count;
        bytes -= count;
        moved += count;
        if (bytes == 0) {
            break;
        }
    }
    return moved;
}


// int __fastcall Audio_Queue_File(AudioQueue *queue, FileClass *file, int bytes, int *done) -- 0x00408EA0
// Purpose: moves up to bytes between file and the ring through the queue
// (reading the file for the fill side, writing it for the drain side),
// adding the bytes moved to *done. Returns 1 when all moved or the queue
// has no room, 2 at the end of the file, 0 on an error (a negative result
// also sets the queue's error).

int __fastcall Audio_Queue_File(AudioQueue *queue, FileClass *file, int bytes, int *done)
{
    int ignored;
    Audio_Queue_Update(queue, 0);
    if (done == 0) {
        done = &ignored;
    }
    *done = 0;
    for (;;) {
        int count = queue->Available;
        if (count > bytes) {
            count = bytes;
        }
        if (count == 0) {
            return 1;
        }
        int moved;
        if (queue->Index != 0) {
            moved = file->Write(queue->Pointer, count);
            if (moved < 0) {
                queue->Error = 0x80040000;
            }
        } else {
            moved = file->Read(queue->Pointer, count);
            if (moved < 0) {
                queue->Error = 0x80040000;
            }
        }
        if (moved == 0) {
            return 2;
        }
        if (moved == -1) {
            return 0;
        }
        Audio_Queue_Update(queue, moved);
        bytes -= moved;
        *done += moved;
        if (bytes == 0) {
            return 1;
        }
    }
}


// int __fastcall Audio_Queue_Stdio(AudioQueue *queue, void *file, int bytes, int *done) -- 0x00408F50
// Purpose: Audio_Queue_File for a C stdio file (fread/fwrite).

extern "C" unsigned int __cdecl fread(void *buffer, unsigned int size, unsigned int count, void *file);
extern "C" unsigned int __cdecl fwrite(void const *buffer, unsigned int size, unsigned int count, void *file);

int __fastcall Audio_Queue_Stdio(AudioQueue *queue, void *file, int bytes, int *done)
{
    int ignored;
    Audio_Queue_Update(queue, 0);
    if (done == 0) {
        done = &ignored;
    }
    *done = 0;
    for (;;) {
        int count = queue->Available;
        if (count > bytes) {
            count = bytes;
        }
        if (count == 0) {
            return 1;
        }
        int moved;
        if (queue->Index != 0) {
            moved = fwrite(queue->Pointer, 1, count, file);
            if (moved < 0) {
                queue->Error = 0x80040000;
            }
        } else {
            moved = fread(queue->Pointer, 1, count, file);
            if (moved < 0) {
                queue->Error = 0x80040000;
            }
        }
        if (moved == 0) {
            return 2;
        }
        if (moved == -1) {
            return 0;
        }
        Audio_Queue_Update(queue, moved);
        bytes -= moved;
        *done += moved;
        if (bytes == 0) {
            return 1;
        }
    }
}


// HRESULT __fastcall Audio_Queue_Error(AudioQueue *queue) -- 0x00409010
// Purpose: the queue's last transfer error.

HRESULT __fastcall Audio_Queue_Error(AudioQueue *queue)
{
    return queue->Error;
}


// int __fastcall Audio_Queue_Commit(AudioQueue *queue) -- 0x00409020
// Purpose: commits the queue's pending bytes: counts them consumed here
// and queued on the other queue, releases them from the node windows
// starting at the commit node (a fully used window is reset to its node's
// whole buffer), makes the reached node the new commit and current node,
// clears the at-head flag and refreshes the queue (0x004090E0). Returns
// the bytes committed.

int __fastcall Audio_Queue_Commit(AudioQueue *queue)
{
    int pending = queue->Pending;
    if (pending == 0) {
        return 0;
    }
    AudioQueue *partner = Audio_Queue_Partner(queue);
    queue->Consumed += pending;
    partner->Queued += pending;
    int committed = pending;
    AudioStreamNode *node = queue->CommitNode;
    while (pending != 0) {
        AudioStreamWindow *window = &node->Windows[queue->Index];
        if (window->Length <= pending) {
            pending -= window->Length;
            window->Pointer = node->Base;
            window->Length = node->Size;
            node = node->Ring_Next();
        } else {
            window->Pointer += pending;
            window->Length -= pending;
            pending = 0;
        }
    }
    queue->CommitNode = node;
    Audio_Queue_Rewind(queue);
    return committed;
}


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


// void __fastcall Audio_Queue_Rewind(AudioQueue *queue) -- 0x004091D0
// Purpose: resets the queue to its commit node (Audio_Queue_Reset expanded
// inline) and refreshes it.

void __fastcall Audio_Queue_Rewind(AudioQueue *queue)
{
    Audio_Queue_Reset(queue);
    Audio_Queue_Update(queue, 0);
}


// HRESULT __fastcall Audio_Queue_Flush(AudioQueue *queue) -- 0x00409210
// Purpose: updates the queue (0x004090E0) with a zero limit, then once more
// limited to its ready count when that is non-zero. Returns 0x80040000.

HRESULT __fastcall Audio_Queue_Flush(AudioQueue *queue)
{
    Audio_Queue_Update(queue, 0);
    if (queue->Available != 0) {
        Audio_Queue_Update(queue, queue->Available);
    }
    return 0x80040000;
}


// int __fastcall Audio_Queue_Ready(AudioQueue *queue) -- 0x00409230
// Purpose: updates the queue (0x004090E0) with a zero limit and returns
// its ready count.

int __fastcall Audio_Queue_Ready(AudioQueue *queue)
{
    Audio_Queue_Update(queue, 0);
    return queue->Available;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int __fastcall Audio_Queue_Available(AudioQueue *queue) -- 0x00409240
// From ra2-decomp STM_AccessTotalBytes (YR 0x00409320).
// Purpose: the bytes queued to this queue that it has not committed yet
// (Queued minus Consumed, written with the twin's unsigned wrap test, which folds).

int __fastcall Audio_Queue_Available(AudioQueue *queue)
{
    unsigned int queued = queue->Queued;
    unsigned int consumed = queue->Consumed;
    if (queued < consumed) {
        return (0xFFFFFFFFu - consumed) + 1 + queued;
    }
    return queued - consumed;
}


// int __fastcall Audio_Queue_Position(AudioQueue *queue) -- 0x00409250
// Purpose: the queue's offset in the ring.

int __fastcall Audio_Queue_Position(AudioQueue *queue)
{
    return queue->Position;
}


// AudioQueue *__fastcall Audio_Queue_Partner(AudioQueue *queue) -- 0x00409260
// Purpose: the ring's other queue.

AudioQueue *__fastcall Audio_Queue_Partner(AudioQueue *queue)
{
    int other = queue->Index + 1;
    if (other == 2) {
        other = 0;
    }
    return &queue->Stream->Queues[other];
}
