// vqalib_buffer_.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_buffer_.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"
#include "shared/crt_memory.hpp"


// long __fastcall AllocBuffers(VQAHandleP *vqap) -- 0x007768A0
// Purpose: sizes and allocates the player buffers: the codebook buffer and
// node ring (count from the codebook group size, plus one or two nodes for
// loops that do not start or end on a group), the pointer buffer and frame
// node ring, the audio buffers when the movie and options have audio, and
// the frame offset table. Everything is locked and counted in MemUsed.
// -8 out of memory (all freed), -3 codebook larger than the buffer, -18 no
// audio buffer size.

// Whether the loops' start and end frames begin a codebook group (inlined).
static void VQA_BufferPerpareLoop(VQAHandleP *vqap, long *needs_start_cb, long *needs_end_cb)
{
    int lcount = vqap->LoopInfo.Header.Count;
    VQALoopInfo::DATA *ldata = vqap->LoopInfo.Data;
    int groupsize = vqap->Header.Groupsize;
    int lindex;

    if (groupsize > 0) {
        *needs_start_cb = 0;
        *needs_end_cb = 0;
        for (lindex = 0; lindex < lcount; lindex++) {
            if (ldata[lindex].StartFrame % groupsize) {
                *needs_start_cb = 1;
                break;
            }
        }
        for (lindex = 0; lindex < lcount; lindex++) {
            if (ldata[lindex].EndFrame % groupsize) {
                *needs_end_cb = 1;
                break;
            }
        }
    } else {
        int ccount = vqap->CodebookInfo.Header.Count;
        VQACodebookInfo::DATA *cdata = vqap->CodebookInfo.Data;
        *needs_start_cb = 1;
        *needs_end_cb = 1;
        for (lindex = 0; lindex < lcount; lindex++) {
            int cindex;
            if (*needs_start_cb) {
                for (cindex = 0; cindex < ccount; cindex++) {
                    if (ldata[lindex].StartFrame == cdata[cindex].Frame) {
                        *needs_start_cb = 0;
                        break;
                    }
                }
            }
            if (*needs_end_cb) {
                for (cindex = 0; cindex < ccount; cindex++) {
                    if (ldata[lindex].EndFrame == cdata[cindex].Frame) {
                        *needs_end_cb = 0;
                        break;
                    }
                }
            }
            if (!*needs_start_cb && !*needs_end_cb) {
                break;
            }
        }
    }
}

long __fastcall AllocBuffers(VQAHandleP *vqap)
{
    long needs_start_cb;
    long needs_end_cb;
    VQACBNode *cbnode;
    VQACBNode *this_cb;
    VQAFrameNode *framenode;
    VQAFrameNode *this_frame;
    long i;
    VQAConfig *config = &vqap->Config;
    VQAHeader *header = &vqap->Header;

    if (config->NumFrameBufs == 0) {
        return -8;
    }
    vqap->CBBufferSize = (header->CBentries * header->BlockWidth * header->BlockHeight + 4) & ~3;
    if (header->ColorMode == 1 || header->ColorMode == 4) {
        vqap->CBBufferSize *= 2;
    }
    if (header->MaxCBSize > (unsigned long)vqap->CBBufferSize) {
        return -3;
    }
    if (header->MaxCBSize == 0) {
        header->MaxCBSize = vqap->CBBufferSize;
    }
    if (config->NumCBBufs <= 0) {
        long groupsize;
        if (vqap->CodebookInfo.Header.Count > 0) {
            groupsize = vqap->CodebookInfo.Header.Groupsize;
        } else {
            groupsize = header->Groupsize;
        }
        if (groupsize == vqap->NumFrames) {
            config->NumCBBufs = 1;
        } else {
            int newcbcount = 0;
            if (config->NumFrameBufs > 1) {
                newcbcount = (config->NumFrameBufs - 2) / groupsize + 1;
                if (config->OptionFlags & 0x20) {
                    if (vqap->LoopInfo.Header.Count == 0) {
                        newcbcount++;
                        newcbcount++;
                    } else {
                        VQA_BufferPerpareLoop(vqap, &needs_start_cb, &needs_end_cb);
                        if (needs_start_cb != 0) {
                            newcbcount++;
                        }
                        if (needs_end_cb != 0) {
                            newcbcount++;
                        }
                    }
                }
            }
            config->NumCBBufs = newcbcount + 2;
        }
    }

    vqap->AltBufferFlags |= VQAABUFF_ALTCB;
    vqap->Max_CB_Size = (header->MaxCBSize + 16) & ~3;
    vqap->AltCBBuffer = (void *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, vqap->CBBufferSize);
    if (vqap->AltCBBuffer == 0) {
        FreeBuffers(vqap);
        return -8;
    }
    config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)vqap->AltCBBuffer, vqap->CBBufferSize);
    vqap->MemUsed += vqap->CBBufferSize;

    cbnode = 0;
    this_cb = 0;
    for (i = 0; i < config->NumCBBufs; i++) {
        cbnode = (VQACBNode *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, sizeof(VQACBNode) + vqap->Max_CB_Size);
        if (cbnode == 0) {
            FreeBuffers(vqap);
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)cbnode, sizeof(VQACBNode) + vqap->Max_CB_Size);
        vqap->MemUsed += sizeof(VQACBNode) + vqap->Max_CB_Size;
        memset(cbnode, 0, sizeof(VQACBNode));
        cbnode->Buffer = (unsigned char *)cbnode + sizeof(VQACBNode);
        if (this_cb == 0) {
            vqap->CBData = cbnode;
        } else {
            this_cb->Next = cbnode;
            cbnode->Prev = this_cb;
        }
        this_cb = cbnode;
    }
    if (cbnode != 0) {
        cbnode->Next = vqap->CBData;
        vqap->CBData->Prev = cbnode;
    }
    vqap->Loader.CurCB = vqap->CBData;
    vqap->Loader.FullCB = vqap->CBData;
    vqap->Max_CB_Size -= 4;

    vqap->Max_Pal_Size = 0x3FC;
    vqap->PtrBufferSize = ((header->ImageWidth / header->BlockWidth) * (header->ImageHeight / header->BlockHeight) * 2 + 4) & ~3;
    unsigned long maxptr;
    if (header->MaxFramesize == 0) {
        maxptr = vqap->PtrBufferSize;
    } else {
        maxptr = header->MaxFramesize;
        if (header->Version > 2) {
            maxptr <<= 8;
        }
    }
    vqap->AltBufferFlags |= VQAABUFF_ALTPTR;
    vqap->Max_Ptr_Size = (maxptr + 8) & ~3;
    vqap->AltPtrBuffer = (void *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, vqap->PtrBufferSize);
    if (vqap->AltPtrBuffer == 0) {
        FreeBuffers(vqap);
        return -8;
    }
    config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)vqap->AltPtrBuffer, vqap->PtrBufferSize);
    vqap->MemUsed += vqap->PtrBufferSize;

    framenode = 0;
    this_frame = 0;
    for (i = 0; i < config->NumFrameBufs; i++) {
        framenode = (VQAFrameNode *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0,
                                                          sizeof(VQAFrameNode) + vqap->Max_Ptr_Size + vqap->Max_Pal_Size);
        if (framenode == 0) {
            FreeBuffers(vqap);
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)framenode,
                              sizeof(VQAFrameNode) + vqap->Max_Ptr_Size + vqap->Max_Pal_Size);
        vqap->MemUsed += sizeof(VQAFrameNode) + vqap->Max_Ptr_Size + vqap->Max_Pal_Size;
        memset(framenode, 0, sizeof(VQAFrameNode));
        framenode->Pointers = (unsigned char *)framenode + sizeof(VQAFrameNode);
        framenode->Palette = (unsigned char *)framenode + sizeof(VQAFrameNode) + vqap->Max_Ptr_Size;
        framenode->Codebook = 0;
        if (this_frame == 0) {
            vqap->FrameData = framenode;
        } else {
            this_frame->Next = framenode;
            framenode->Prev = this_frame;
        }
        this_frame = framenode;
    }
    if (framenode != 0) {
        framenode->Next = vqap->FrameData;
        vqap->FrameData->Prev = framenode;
    }
    vqap->Loader.CurFrame = vqap->FrameData;
    vqap->Drawer.CurFrame = vqap->FrameData;
    vqap->Flipper.CurFrame = vqap->FrameData;
    vqap->Max_Pal_Size -= 4;
    vqap->Max_Ptr_Size -= 4;

    if ((header->Flags & 1) && (config->OptionFlags & 1)) {
        VQAAudio *audio = &vqap->Audio;
        if (header->Version < 2) {
            vqap->SampleRate = 22050;
            vqap->Channels = 1;
            vqap->BitsPerSample = 8;
            audio->BytesPerSec = 22050;
        } else {
            if ((config->OptionFlags & 0x40) && (header->Flags & 2)) {
                vqap->SampleRate = header->AltSampleRate;
                vqap->Channels = header->AltChannels;
                vqap->BitsPerSample = header->AltBitsPerSample;
            } else {
                vqap->SampleRate = header->SampleRate;
                vqap->Channels = header->Channels;
                vqap->BitsPerSample = header->BitsPerSample;
            }
            audio->BytesPerSec = (vqap->SampleRate * vqap->Channels) * (vqap->BitsPerSample >> 3);
        }
        if (config->AudioBufSize == -1) {
            i = (audio->BytesPerSec + audio->BytesPerSec / 2) / config->HMIBufSize;
            config->AudioBufSize = config->HMIBufSize * i;
        }
        if (config->AudioBufSize > 0) {
        if (config->AudioBuf == 0) {
            audio->Buffer = (unsigned char *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, config->AudioBufSize);
            if (audio->Buffer == 0) {
                FreeBuffers(vqap);
                return -8;
            }
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)audio->Buffer, config->AudioBufSize);
            vqap->MemUsed += config->AudioBufSize;
        } else {
            audio->Buffer = (unsigned char *)config->AudioBuf;
        }
        audio->NumAudBlocks = config->AudioBufSize / config->HMIBufSize;
        audio->IsLoaded = (bool *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, audio->NumAudBlocks * sizeof(*audio->IsLoaded));
        if (audio->IsLoaded == 0) {
            FreeBuffers(vqap);
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)audio->IsLoaded, audio->NumAudBlocks * sizeof(*audio->IsLoaded));
        vqap->MemUsed += audio->NumAudBlocks * sizeof(*audio->IsLoaded);
        memset(audio->IsLoaded, 0, audio->NumAudBlocks * sizeof(*audio->IsLoaded));
        audio->BlockRepeats = (short *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, audio->NumAudBlocks * sizeof(*audio->BlockRepeats));
        if (audio->BlockRepeats == 0) {
            FreeBuffers(vqap);
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)audio->BlockRepeats, audio->NumAudBlocks * sizeof(*audio->BlockRepeats));
        vqap->MemUsed += audio->NumAudBlocks * sizeof(*audio->BlockRepeats);
        memset(audio->BlockRepeats, 0, audio->NumAudBlocks * sizeof(*audio->BlockRepeats));
        audio->TempBufSize = audio->BytesPerSec / header->FPS * 2 + 100;
        audio->TempBuf = (unsigned char *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, audio->TempBufSize);
        if (audio->TempBuf == 0) {
            FreeBuffers(vqap);
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)audio->TempBuf, audio->TempBufSize);
        vqap->MemUsed += audio->TempBufSize;
        audio->HMIBuffer = (void *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, config->HMIBufSize);
        if (audio->HMIBuffer == 0) {
            FreeBuffers(vqap);
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)audio->HMIBuffer, config->HMIBufSize);
        memset(audio->HMIBuffer, 0, config->HMIBufSize);
        vqap->MemUsed += config->HMIBufSize;
        } else {
            FreeBuffers(vqap);
            return -18;
        }
    }

    vqap->Foff = (long *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, header->Frames * sizeof(*vqap->Foff));
    if (vqap->Foff == 0) {
        FreeBuffers(vqap);
        return -8;
    }
    config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)vqap->Foff, header->Frames * sizeof(*vqap->Foff));
    vqap->MemUsed += header->Frames * sizeof(long);
    return -1;
}


// void __fastcall FreeBuffers(VQAHandleP *vqap) -- 0x00777190
// Purpose: unlocks and frees every buffer the player allocated: the loop
// cache, the frame offset table, the audio buffers, the frame and codebook
// node lists (and their alternative buffers), the palette and codebook
// info tables and the MSCI/MFCI chunk caches and tables.

void __fastcall FreeBuffers(VQAHandleP *vqap)
{
    VQAConfig *config = &vqap->Config;
    VQAHeader *header = &vqap->Header;
    VQAAudio *audio = &vqap->Audio;

    if (vqap->LoopCache.Ptr != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->LoopCache.Ptr, vqap->LoopCache.Size);
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->LoopCache.Ptr, 0);
    }
    if (vqap->Foff != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->Foff, header->Frames * sizeof(*vqap->Foff));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->Foff, 0);
    }
    if (config->AudioBuf == 0 && audio->Buffer != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)audio->Buffer, config->AudioBufSize);
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)audio->Buffer, 0);
    }
    if (audio->IsLoaded != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)audio->IsLoaded, audio->NumAudBlocks * sizeof(*audio->IsLoaded));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)audio->IsLoaded, 0);
    }
    if (audio->BlockRepeats != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)audio->BlockRepeats, audio->NumAudBlocks * sizeof(*audio->BlockRepeats));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)audio->BlockRepeats, 0);
    }
    if (audio->TempBuf != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)audio->TempBuf, audio->TempBufSize);
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)audio->TempBuf, 0);
    }
    if (audio->HMIBuffer != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)audio->HMIBuffer, config->HMIBufSize);
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)audio->HMIBuffer, 0);
    }

    VQAFrameNode *frame_this = vqap->FrameData;
    for (int i = 0; i < config->NumFrameBufs; i++) {
        if (frame_this == 0) {
            break;
        }
        VQAFrameNode *frame_next = frame_this->Next;
        vqap->Max_Pal_Size += 4;
        vqap->Max_Ptr_Size += 4;
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)frame_this,
                              sizeof(VQAFrameNode) + vqap->Max_Ptr_Size + vqap->Max_Pal_Size);
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)frame_this, 0);
        frame_this = frame_next;
    }
    if (vqap->AltBufferFlags & VQAABUFF_ALTPTR) {
        if (vqap->AltPtrBuffer != 0) {
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->AltPtrBuffer, vqap->PtrBufferSize);
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->AltPtrBuffer, 0);
            vqap->AltPtrBuffer = 0;
        }
    }

    VQACBNode *cb_this = vqap->CBData;
    for (int j = 0; j < config->NumCBBufs; j++) {
        if (cb_this == 0) {
            break;
        }
        VQACBNode *cb_next = cb_this->Next;
        vqap->Max_CB_Size += 4;
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)cb_this, sizeof(VQACBNode) + vqap->Max_CB_Size);
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)cb_this, 0);
        cb_this = cb_next;
    }
    if (vqap->AltBufferFlags & VQAABUFF_ALTCB) {
        if (vqap->AltCBBuffer != 0) {
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->AltCBBuffer, vqap->CBBufferSize);
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->AltCBBuffer, 0);
            vqap->AltCBBuffer = 0;
        }
    }

    if (vqap->PaletteInfo.Data != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->PaletteInfo.Data,
                              vqap->PaletteInfo.Header.Count * sizeof(*vqap->PaletteInfo.Data));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->PaletteInfo.Data, 0);
    }
    if (vqap->CodebookInfo.Data != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->CodebookInfo.Data,
                              vqap->CodebookInfo.Header.Count * sizeof(*vqap->CodebookInfo.Data));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->CodebookInfo.Data, 0);
    }

    {
    unsigned long tablecount = vqap->MSCInfo.Header.Count;
    unsigned long k;
    for (k = 0; k < tablecount; k++) {
        unsigned long count = vqap->MSCInfo.Data2[k].Count;
        VQAChunkCache::DATA *data = vqap->MSCInfo.Data2[k].Data;
        char *buffer = data->Buffer;
        if (buffer != 0) {
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)buffer, count * PADSIZE(vqap->MSCInfo.Table[k].EntrySize));
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)buffer, 0);
        }
        if (data != 0) {
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)data, count * sizeof(*data));
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)data, 0);
        }
    }
    if (vqap->MSCInfo.Data2 != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->MSCInfo.Data2, tablecount * sizeof(*vqap->MSCInfo.Data2));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->MSCInfo.Data2, 0);
    }
    if (vqap->MSCInfo.Table != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->MSCInfo.Table, tablecount * sizeof(*vqap->MSCInfo.Table));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->MSCInfo.Table, 0);
    }

    }
    {
    unsigned long tablecount = vqap->MFCInfo.Header.Count;
    unsigned long k;
    for (k = 0; k < tablecount; k++) {
        unsigned long count = vqap->MFCInfo.Data2[k].Count;
        VQAChunkCache::DATA *data = vqap->MFCInfo.Data2[k].Data;
        char *buffer = data->Buffer;
        if (buffer != 0) {
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)buffer, count * PADSIZE(vqap->MFCInfo.Table[k].EntrySize));
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)buffer, 0);
        }
        if (data != 0) {
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)data, count * sizeof(*data));
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)data, 0);
        }
    }
    if (vqap->MFCInfo.Data2 != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->MFCInfo.Data2, tablecount * sizeof(*vqap->MFCInfo.Data2));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->MFCInfo.Data2, 0);
    }
    if (vqap->MFCInfo.Table != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->MFCInfo.Table, tablecount * sizeof(*vqap->MFCInfo.Table));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->MFCInfo.Table, 0);
    }
    }
    if (vqap->MFCInfo.StaticData != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->MFCInfo.StaticData,
                              vqap->MFCInfo.Header.StaticCount * sizeof(*vqap->MFCInfo.StaticData));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->MFCInfo.StaticData, 0);
    }
    if (vqap->LoopInfo.Data != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->LoopInfo.Data,
                              vqap->LoopInfo.Header.Count * sizeof(*vqap->LoopInfo.Data));
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->LoopInfo.Data, 0);
    }
}


// long __fastcall VQA_Configure_Buffer(VQAHandleP *vqap) -- 0x007776B0
// Purpose: chooses the image size (the configured one, else the movie's
// plus its offset with DrawFlags bit 3), allocates and clears a private
// image buffer when buffering is on and none was given, then installs the
// buffer with VQA_Set_DrawBuffer. -9 without a buffer, -8 when out of memory.

long __fastcall VQA_Configure_Buffer(VQAHandleP *vqap)
{
    unsigned char *buffer;
    int width;
    int height;
    VQAConfig *config = &vqap->Config;
    VQAHeader *header = &vqap->Header;

    if (config->ImageWidth == -1 || config->ImageHeight == -1) {
        width = header->ImageWidth;
        height = header->ImageHeight;
        if (config->DrawFlags & 8) {
            width += header->Xpos;
            height += header->Ypos;
        }
    } else {
        width = config->ImageWidth;
        height = config->ImageHeight;
    }
    if (config->DrawFlags & 1) {
        if (config->ImageBuf == 0) {
            int size = width * height;
            if (header->ColorMode == 1 || header->ColorMode == 4) {
                size *= 2;
            }
            buffer = (unsigned char *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, size);
            if (buffer == 0) {
                return -8;
            }
            vqap->MemUsed += size;
            config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)buffer, size);
            memset(buffer, 0, size);
            vqap->AltImageBuf = buffer;
            vqap->AltImageWidth = width;
            vqap->AltImageHeight = height;
            vqap->Flags |= 0x20;
        } else {
            buffer = (unsigned char *)config->ImageBuf;
        }
    } else {
        if (config->ImageBuf != 0) {
            buffer = (unsigned char *)config->ImageBuf;
        } else {
            return -9;
        }
    }
    VQA_Unknown_0077C480((VQAHandle *)vqap, (int)buffer, width, height, config->X1, config->Y1);
    vqap->Flags |= 0x10;
    return -1;
}


// int __cdecl VQA_Memory_Handler(VQAHandle *vqa, int action, int buffer, int nbytes) -- 0x007777E0
// Purpose: the default memory handler: VQAMEM_ALLOC mallocs, VQAMEM_FREE
// frees, LOCK and UNLOCK return the buffer, action 6 (loop cache budget
// query) stores -1 (no limit) through the buffer.

int __cdecl VQA_Memory_Handler(VQAHandle *vqa, int action, int buffer, int nbytes)
{
    int result = 0;
    switch (action) {
    case VQAMEM_ALLOC:
        result = (int)malloc(nbytes);
        break;
    case VQAMEM_FREE:
        free((void *)buffer);
        break;
    case VQAMEM_LOCK:
    case VQAMEM_UNLOCK:
        result = buffer;
        break;
    case 6:
        *(int *)buffer = -1;
        break;
    }
    return result;
}
