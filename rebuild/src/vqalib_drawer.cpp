// SPDX-License-Identifier: GPL-3.0-or-later

#include <stddef.h>
#include "vqalib/vqaplayp.h"
#include "vqalib/unvq.h"
#include <string.h>

void __cdecl UnVQ_Nop(unsigned char *, unsigned char *, unsigned char *, unsigned long, unsigned long, unsigned long);
long PageFlip_Nop(VQAHandle *);
long DrawFrame_MCGA(VQAHandle *);
long DrawFrame_MCGABuf(VQAHandle *);
long PageFlip_MCGA(VQAHandle *);
long PageFlip_MCGABuf(VQAHandle *);
long Select_Frame(VQAHandleP *);
void Prepare_Frame(VQAHandleP *);
long VQA_ComputeDesiredFrame(VQAHandleP *, VQAConfig *, VQADrawer *, VQAFrameNode *, unsigned long);
void VQA_UnVQFrame(VQAHandleP *, VQAFrameNode *);
void VQA_DispatchFrameChunks(VQAHandleP *, long);

long VQA_ResetLastFrameNum(VQAHandle * vqa)
{
    return ((VQAHandleP *)vqa)->Drawer.LastFrameNum = -1;
}

long VQA_Configure_Drawer(VQAHandleP * vqap)
{
    VQADrawer * drawer = &vqap->Drawer;
    VQAHeader * header = &vqap->Header;
    VQAConfig * config = &vqap->Config;
    drawer->BlocksPerRow = header->ImageWidth / header->BlockWidth;
    drawer->NumRows = header->ImageHeight / header->BlockHeight;
    drawer->NumBlocks = drawer->NumRows * drawer->BlocksPerRow;
    long dimensions = BLOCK_DIM(header->BlockWidth, header->BlockHeight);
    vqap->UnVQ1 = UnVQ_Nop;
    vqap->UnVQ2 = UnVQ_Nop;
    long flags = config->DrawFlags;
    vqap->Page_Flip = PageFlip_Nop;
    if (!(flags & VQACFGF_BUFFER)) {
        vqap->Draw_Frame = DrawFrame_MCGA;
        vqap->Page_Flip = PageFlip_MCGA;
    } else {
        vqap->Draw_Frame = DrawFrame_MCGABuf;
        vqap->Page_Flip = PageFlip_MCGABuf;
    }
    switch (dimensions) {
    case BLOCK_4X4:
        switch (header->ColorMode) {
        case 0:
            vqap->UnVQ1 = ASM_UnVQ_4x4;
            if (header->Flags & VQAHDF_TRANS) {
                if (flags & VQACFGF_NOTRANS) vqap->UnVQ2 = UnVQ2_C0_4x4_KEY;
                else if (flags & VQACFGF_HALFSIZE) vqap->UnVQ2 = UnVQ2_C0_4x4_TRANS_HALF;
                else vqap->UnVQ2 = UnVQ2_C0_4x4_TRANS;
            } else if (flags & VQACFGF_HALFSIZE) vqap->UnVQ1 = ASM_UnVQ_4x4_HALF;
            break;
        case 1:
            vqap->UnVQ1 = ASM_UnVQ1_C1_4x4;
            vqap->UnVQ2 = UnVQ2_C1_4x4;
            break;
        case 4:
            vqap->UnVQ1 = UnVQ1_C4_4x4;
            vqap->UnVQ2 = UnVQ2_C4_4x4;
            break;
        }
        break;
    case BLOCK_4X2:
        switch (header->ColorMode) {
        case 0:
            vqap->UnVQ1 = ASM_UnVQ_4x2;
            if (header->Flags & VQAHDF_TRANS) {
                if (flags & VQACFGF_NOTRANS) vqap->UnVQ2 = UnVQ2_C0_4x2_KEY;
                else vqap->UnVQ2 = UnVQ2_C0_4x2_TRANS;
            }
            break;
        case 4:
            vqap->UnVQ1 = UnVQ1_C4_4x2;
            vqap->UnVQ2 = UnVQ2_C4_4x2;
            break;
        }
        break;
    }
    return VQAERR_NONE;
}

void VQA_DispatchFrameChunks(VQAHandleP * vqap, long frame)
{
    VQAMFCInfo * mfc = &vqap->MFCInfo;
    unsigned long count = mfc->Header.Count;
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long id = mfc->Table[i].ChunkID;
        VQAMFCInfo::DATA2::DATA * base = mfc->Data2[i].Data;
        unsigned long n = mfc->Data2[i].Count;
        for (unsigned long j = 0; j < n; ++j) {
            VQAMFCInfo::DATA2::DATA * data = &base[j];
            if (data->Frame == frame) vqap->Config.EventHandler((VQAHandle *)vqap, id, data->Buffer, data->Size);
        }
    }
    VQAMSCInfo * msc = &vqap->MSCInfo;
    count = msc->Header.Count;
    for (i = 0; i < count; ++i) {
        unsigned long id = msc->Table[i].ChunkID;
        VQAMSCInfo::DATA2::DATA * base = msc->Data2[i].Data;
        unsigned long n = msc->Data2[i].Count;
        for (unsigned long j = 0; j < n; ++j) {
            VQAMSCInfo::DATA2::DATA * data = &base[j];
            if (data->Frame == frame) vqap->Config.EventHandler((VQAHandle *)vqap, id, data->Buffer, data->Size);
        }
    }
}

void VQA_UnVQFrame(VQAHandleP * vqap, VQAFrameNode * frame)
{
    VQADrawer * drawer = &vqap->Drawer;
    VQAConfig * config = &vqap->Config;
    VQACBNode * node = frame->Codebook;
    if (drawer->Flags & VQACFGF_NODRAW) return;
    unsigned char * buffer;
    if ((!config->EventHandler || !(buffer = (unsigned char *)config->EventHandler((VQAHandle *)vqap, VQAEVENT_LOCK, 0, 0))) && !(buffer = drawer->ImageBuf)) return;
    long blocks = drawer->BlocksPerRow;
    long rows = drawer->NumRows;
    unsigned char * dest = buffer + drawer->ScreenOffset;
    unsigned char * pointers = (frame->Flags & VQAFRMF_ALTPTR) ? (unsigned char *)vqap->AltPtrBuffer : frame->Pointers;
    unsigned char * codebook = (node->Flags & VQACBF_ALTPTR) ? (unsigned char *)vqap->AltCBBuffer : node->Buffer;
    if (config->EventHandler && (node->Flags & VQACBF_CBFULL)) {
        node->Flags &= ~VQACBF_CBFULL;
        config->EventHandler((VQAHandle *)vqap, VQAEVENT_CODEBOOK, codebook, node->CodebookSize);
    }
    if (!(frame->Flags & VQAFRMF_RSDCOMP)) vqap->UnVQ1(codebook, pointers, dest, blocks, rows, drawer->ImageWidth);
    else vqap->UnVQ2(codebook, pointers, dest, blocks, rows, drawer->ImageWidth);
    if (config->EventHandler) config->EventHandler((VQAHandle *)vqap, VQAEVENT_UNLOCK, 0, 0);
}

long DrawFrame_MCGA(VQAHandle * vqa)
{
    VQAHandleP * vqap = (VQAHandleP *)vqa;
    VQADrawer * drawer = &vqap->Drawer;
    long result = Select_Frame(vqap);
    if (result == VQAERR_NONE) {
        Prepare_Frame(vqap);
        VQAFrameNode * frame = drawer->CurFrame;
        vqap->Flipper.CurFrame = frame;
        if (!(drawer->Flags & VQADRWF_HOLD)) drawer->CurFrame = frame->Next;
        result = VQAERR_NONE;
    }
    return result;
}

#if 0
long Select_Frame(VQAHandleP * vqap)
{
    VQAConfig * config;
    VQADrawer * drawer;
    VQAFrameNode * frame;
    long desired;
    config = &vqap->Config;
    drawer = &vqap->Drawer;
    frame = drawer->CurFrame;
    if (!(frame->Flags & VQAFRMF_LOADED)) {
        drawer->WaitsOnLoader++;
        if (!(drawer->Flags & VQADRWF_REPAINT) || (vqap->Flags & VQADATF_LDONE)) return VQAERR_NOBUFFER;
        drawer->CurFrame = frame->Prev;
        drawer->CurFrame->Flags = drawer->CurFrame->PrevFlags;
        if (drawer->Flags & VQADRWF_FORCEDRAW) {
            drawer->Flags |= VQADRWF_HOLD;
            frame->Flags |= VQAFRMF_HOLD;
            return VQAERR_NONE;
        }
    } else {
        if (drawer->Flags & VQADRWF_FORCEDRAW) {
            drawer->LastFrameNum = drawer->CurFrame->FrameNum;
            drawer->Flags |= VQADRWF_HOLD;
            frame->Flags |= VQAFRMF_HOLD;
            return VQAERR_NONE;
        }
        if (vqap->Config.OptionFlags & VQAOPTF_STEP) {
            drawer->LastFrameNum = frame->FrameNum;
            VQA_SetTimer(vqap, (frame->FrameNum * VQA_TIMETICKS) / vqap->Config.DrawRate);
            return VQAERR_NONE;
        }
        long time = VQA_GetTime(vqap);
        long next;
        if (!(drawer->Flags & VQADRWF_STEP)) next = drawer->LastFrameNum + 1;
        else next = drawer->LastFrameNum + 2;
        if (time < (next * VQA_TIMETICKS) / vqap->Config.DrawRate) {
            if (drawer->Flags & VQADRWF_REPAINT) {
            desired = VQA_ComputeDesiredFrame(vqap, config, drawer, frame, time);
            if (desired == drawer->LastFrameNum && (frame->Flags & VQAFRMF_HOLD)) {
                drawer->LastFrameNum = frame->FrameNum;
                return VQAERR_NONE;
            }
            drawer->LastFrameNum = frame->FrameNum;
            drawer->Flags |= VQADRWF_HOLD;
            frame->Flags |= VQAFRMF_HOLD;
            return VQAERR_NONE;
            }
            return VQAERR_NOT_TIME;
        }
        desired = VQA_ComputeDesiredFrame(vqap, config, drawer, frame, time);
        if (vqap->Config.DrawFlags & VQACFGF_NOSKIP) {
            drawer->LastFrameNum = drawer->CurFrame->FrameNum;
            return VQAERR_NONE;
        } else {
        long limit = vqap->Config.FrameRate / 3 - 1;
        if (limit < 0) limit = 0;
        while (frame->FrameNum != desired && !(frame->Flags & VQAFRMF_KEY)) {
            long current = drawer->CurFrame->FrameNum;
            long distance;
            if (current >= drawer->LastFrameNum && current <= vqap->LoopEndFrame2) {
                distance = current - drawer->LastFrameNum;
            } else {
                long fallback = vqap->Config.FrameRate / 3;
                long tail = vqap->LoopEndFrame2 - drawer->LastFrameNum;
                if (tail < 0) distance = fallback;
                else {
                    long head = current - vqap->LoopStartFrame0;
                    if (head < 0) distance = fallback;
                    else distance = head + tail + 1;
                }
            }
            if (distance > limit) break;
            if (frame->Flags & VQAFRMF_PALETTE) {
                if (frame->Flags & VQAFRMF_PALCOMP) {
                    frame->PaletteSize = VQA_LCW_Uncompress((char *)frame->Palette + frame->PalOffset, (char *)frame->Palette, vqap->Max_Pal_Size);
                    frame->Flags &= ~VQAFRMF_PALCOMP;
                }
                memcpy(drawer->Palette_24, frame->Palette, frame->PaletteSize);
                drawer->CurPalSize = frame->PaletteSize;
                drawer->Flags |= VQADRWF_SETPAL;
            }
            if (vqap->Config.EventHandler) {
                if (frame->Flags & VQAFRMF_LOOPED) {
                    vqap->Config.EventHandler((VQAHandle *)vqap, VQAEVENT_LOOPED, (void *)frame->FrameNum, vqap->LoopID);
                    frame->Flags &= ~VQAFRMF_LOOPED;
                }
                if (frame->Flags & VQAFRMF_LOOPJMP) {
                    vqap->Config.EventHandler((VQAHandle *)vqap, VQAEVENT_LOOPJUMP, (void *)frame->FrameNum, vqap->LoopID);
                    frame->Flags &= ~VQAFRMF_LOOPJMP;
                }
                if (frame->Flags & VQAFRMF_CHUNKS) {
                    VQA_DispatchFrameChunks(vqap, frame->FrameNum);
                    frame->Flags &= ~VQAFRMF_CHUNKS;
                }
            }
            if (vqap->Config.DrawerCallback && vqap->Config.DrawerCallback(0, frame->FrameNum)) return VQAERR_EOF;
            unsigned long previous_flags = frame->Flags;
            frame->Flags = 0;
            frame->PrevFlags = previous_flags;
            frame = frame->Next;
            drawer->CurFrame = frame;
            vqap->SkippedFrames++;
            if (!(frame->Flags & VQAFRMF_LOADED)) {
                drawer->WaitsOnLoader++;
                if (!(drawer->Flags & VQADRWF_REPAINT) || (vqap->Flags & VQADATF_LDONE)) return VQAERR_NOBUFFER;
                frame = frame->Prev;
                drawer->CurFrame = frame;
                frame->Flags = frame->PrevFlags;
                return VQAERR_NONE;
            }
        }
        drawer->LastFrameNum = drawer->CurFrame->FrameNum;
        }
    }
    return VQAERR_NONE;
}
#endif




long VQA_ComputeDesiredFrame(VQAHandleP * vqap, VQAConfig * config, VQADrawer * drawer, VQAFrameNode * frame, unsigned long time)
{
    long rate = config->FrameRate;
    long current = frame->FrameNum;
    long desired = (rate * time) / VQA_TIMETICKS;
    if (drawer->Flags & VQADRWF_STEP) --desired;
    long end = vqap->LoopEndFrameMode2;
    bool wrapped = false;
    if (vqap->Flags & VQADATF_LOOPED) {
        if (vqap->Flags & VQADATF_LOOPJMP) end = vqap->LoopEndFrame2;
        long stepframes;
        do {
            if (((config->DrawFlags & VQACFGF_NOSKIP) || desired <= end) && (!(config->DrawFlags & VQACFGF_NOSKIP) || current != vqap->LoopStartFrame0 || wrapped)) break;
            stepframes = vqap->LoopStartFrame0 - end - 1;
            VQA_StepTimer(vqap, (stepframes * VQA_TIMETICKS) / rate);
            long now = VQA_GetTime(vqap);
            desired = (now * rate) / VQA_TIMETICKS;
            wrapped = true;
        } while (stepframes);
    }
    if (config->DrawFlags & VQACFGF_NOSKIP) {
        if (desired > current) {
            long behind = current - desired;
            if (!(vqap->Header.Flags & VQAHDF_AUDIO) || !(config->OptionFlags & VQAOPTF_AUDIO)) VQA_StepTimer(vqap, (behind * VQA_TIMETICKS) / rate);
            desired = current;
            vqap->Flags |= VQADATF_FRAMESTALL;
        } else vqap->Flags &= ~VQADATF_FRAMESTALL;
    }
    if (wrapped) {
        if ((drawer->Flags & VQADRWF_STEP) && !(config->DrawFlags & VQACFGF_NOSKIP) && --desired < vqap->LoopStartFrame0) desired = end;
        if (vqap->LoopEndFrameNormal != -1) {
            vqap->LoopStartFrame0 = vqap->LoopStartFrame1;
            vqap->LoopStartFrame1 = -1;
            vqap->LoopEndFrameMode2 = vqap->LoopEndFrameNormal;
            vqap->LoopEndFrameNormal = -1;
        }
        if (!(vqap->Flags & VQADATF_LOOPJMP)) {
            vqap->LoopStartFrame2 = vqap->LoopStartFrame0;
            vqap->LoopEndFrame2 = vqap->LoopEndFrameMode2;
        }
        vqap->Flags &= ~(VQADATF_LOOPED | VQADATF_LOOPJMP);
    }
    return desired;
}

void Prepare_Frame(VQAHandleP * vqap)
{
    VQAFrameNode * frame = vqap->Drawer.CurFrame;
    VQACBNode * node = frame->Codebook;
    if (!(vqap->Drawer.Flags & VQACFGF_NODRAW)) {
        if (node->Flags & VQACBF_CBCOMP) {
            unsigned char * dest;
            if (vqap->AltBufferFlags & VQAABUFF_ALTCB) {
                dest = (unsigned char *)vqap->AltCBBuffer;
                node->Flags |= VQACBF_ALTPTR;
            } else dest = node->Buffer;
            node->CodebookSize = VQA_LCW_Uncompress((char *)node->Buffer + node->CBOffset, (char *)dest, vqap->CBBufferSize);
            node->Flags &= ~VQACBF_CBCOMP;
        }
        if (frame->Flags & VQAFRMF_PALCOMP) {
            frame->PaletteSize = VQA_LCW_Uncompress((char *)frame->Palette + frame->PalOffset, (char *)frame->Palette, vqap->Max_Pal_Size);
            frame->Flags &= ~VQAFRMF_PALCOMP;
        }
        if (frame->Flags & VQAFRMF_PTRCOMP) {
            unsigned char * dest;
            if (vqap->AltBufferFlags & VQAABUFF_ALTPTR) {
                dest = (unsigned char *)vqap->AltPtrBuffer;
                frame->Flags |= VQAFRMF_ALTPTR;
            } else dest = frame->Pointers;
            VQA_LCW_Uncompress((char *)frame->Pointers + frame->PtrOffset, (char *)dest, vqap->PtrBufferSize);
            frame->Flags &= ~VQAFRMF_PTRCOMP;
        }
    }
}

long PageFlip_MCGA(VQAHandle * vqa)
{
    VQAHandleP * vqap = (VQAHandleP *)vqa;
    VQAFrameNode * frame = vqap->Flipper.CurFrame;
    if ((frame->Flags & VQAFRMF_PALETTE) || (vqap->Drawer.Flags & VQADRWF_SETPAL)) {
    if (frame->Flags & VQAFRMF_PALETTE) {
        if (vqap->Config.EventHandler) vqap->Config.EventHandler(vqa, VQAEVENT_PALETTE, frame->Palette, frame->PaletteSize);
    } else if (vqap->Drawer.Flags & VQADRWF_SETPAL) {
        vqap->Drawer.Flags &= ~VQADRWF_SETPAL;
        if (vqap->Config.EventHandler) vqap->Config.EventHandler(vqa, VQAEVENT_PALETTE, vqap->Drawer.Palette_24, vqap->Drawer.CurPalSize);
    }
    }
    VQA_UnVQFrame(vqap, frame);
    if (vqap->Config.DrawerCallback && vqap->Config.DrawerCallback(vqa, frame->FrameNum)) return VQAERR_EOF;
    return VQAERR_NONE;
}

long DrawFrame_MCGABuf(VQAHandle * vqa)
{
    VQAHandleP * vqap = (VQAHandleP *)vqa;
    VQADrawer * drawer = &vqap->Drawer;
    long result = Select_Frame(vqap);
    if (result == VQAERR_NONE) {
        Prepare_Frame(vqap);
        VQAFrameNode * frame = drawer->CurFrame;
        VQA_UnVQFrame(vqap, frame);
        vqap->Flipper.CurFrame = frame;
        if (!(drawer->Flags & VQADRWF_HOLD)) drawer->CurFrame = frame->Next;
        result = VQAERR_NONE;
    }
    return result;
}

long PageFlip_MCGABuf(VQAHandle * vqa)
{
    VQAHandleP * vqap = (VQAHandleP *)vqa;
    VQAFrameNode * frame = vqap->Flipper.CurFrame;
    if ((frame->Flags & VQAFRMF_PALETTE) || (vqap->Drawer.Flags & VQADRWF_SETPAL)) {
    if (frame->Flags & VQAFRMF_PALETTE) {
        if (vqap->Config.EventHandler) vqap->Config.EventHandler(vqa, VQAEVENT_PALETTE, frame->Palette, frame->PaletteSize);
    } else if (vqap->Drawer.Flags & VQADRWF_SETPAL) {
        vqap->Drawer.Flags &= ~VQADRWF_SETPAL;
        if (vqap->Config.EventHandler) vqap->Config.EventHandler(vqa, VQAEVENT_PALETTE, vqap->Drawer.Palette_24, vqap->Drawer.CurPalSize);
    }
    }
    if (vqap->Config.DrawerCallback && vqap->Config.DrawerCallback(vqa, frame->FrameNum)) return VQAERR_EOF;
    return VQAERR_NONE;
}

void __cdecl UnVQ_Nop(unsigned char *, unsigned char *, unsigned char *, unsigned long, unsigned long, unsigned long)
{
}

long PageFlip_Nop(VQAHandle *)
{
    return VQAERR_NONE;
}
