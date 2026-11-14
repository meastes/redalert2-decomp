// vqalib_drawer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_drawer.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"
#include "shared/crt_memory.hpp"


// long __fastcall VQA_ResetLastFrameNum(VQAHandle *vqa) -- 0x00777870
// Purpose: forgets the last drawn frame (-1) so playback progress after a
// seek is not measured against a frame from before it.

long __fastcall VQA_ResetLastFrameNum(VQAHandle *vqa)
{
    VQAHandleP *vqap;
    VQADrawer *drawer;

    vqap = (VQAHandleP *)vqa;
    drawer = &vqap->Drawer;
    drawer->LastFrameNum = -1;
    return -1;
}


// long __fastcall VQA_Configure_Drawer(VQAHandleP *vqap) -- 0x00777880
// Purpose: computes the block grid of the movie and selects the frame
// drawer, page flipper and the two UnVQ decoders for the block size, color
// mode, transparency and draw flags.

long __fastcall VQA_Configure_Drawer(VQAHandleP *vqap)
{
    VQAHeader *header;
    VQADrawer *drawer;
    long blkdim;

    drawer = &vqap->Drawer;
    header = &vqap->Header;

    drawer->BlocksPerRow = header->ImageWidth / header->BlockWidth;
    drawer->NumRows = header->ImageHeight / header->BlockHeight;
    drawer->NumBlocks = drawer->BlocksPerRow * drawer->NumRows;

    blkdim = BLOCK_DIM(header->BlockWidth, header->BlockHeight);

    vqap->UnVQ1 = UnVQ_Nop;
    vqap->UnVQ2 = UnVQ_Nop;
    vqap->Page_Flip = PageFlip_Nop;

    if (!(vqap->Config.DrawFlags & VQACFGF_BUFFER)) {
        vqap->Draw_Frame = DrawFrame_MCGA;
        vqap->Page_Flip = PageFlip_MCGA;
    } else {
        vqap->Draw_Frame = DrawFrame_MCGABuf;
        vqap->Page_Flip = PageFlip_MCGABuf;
    }

    switch (blkdim) {
    case BLOCK_4X2:
        switch (header->ColorMode) {
        case 0:
            vqap->UnVQ1 = ASM_UnVQ_4x2;
            if (header->Flags & VQAHDF_TRANS) {
                if (vqap->Config.DrawFlags & VQACFGF_NOTRANS) {
                    vqap->UnVQ2 = UnVQ2_C0_4x2_KEY;
                } else {
                    vqap->UnVQ2 = UnVQ2_C0_4x2_TRANS;
                }
            }
            break;

        case 4:
            vqap->UnVQ1 = UnVQ1_C4_4x2;
            vqap->UnVQ2 = UnVQ2_C4_4x2;
            break;
        }
        break;

    case BLOCK_4X4:
        switch (header->ColorMode) {
        case 4:
            vqap->UnVQ1 = UnVQ1_C4_4x4;
            vqap->UnVQ2 = UnVQ2_C4_4x4;
            break;

        case 1:
            vqap->UnVQ1 = ASM_UnVQ1_C1_4x4;
            vqap->UnVQ2 = UnVQ2_C1_4x4;
            break;

        case 0:
            vqap->UnVQ1 = ASM_UnVQ_4x4;
            if (header->Flags & VQAHDF_TRANS) {
                if (vqap->Config.DrawFlags & VQACFGF_NOTRANS) {
                    vqap->UnVQ2 = UnVQ2_C0_4x4_KEY;
                } else if (vqap->Config.DrawFlags & VQACFGF_HALFSIZE) {
                    vqap->UnVQ2 = UnVQ2_C0_4x4_TRANS_HALF;
                } else {
                    vqap->UnVQ2 = UnVQ2_C0_4x4_TRANS;
                }
            } else {
                if (vqap->Config.DrawFlags & VQACFGF_HALFSIZE) {
                    vqap->UnVQ1 = ASM_UnVQ_4x4_HALF;
                }
            }
            break;
        }
        break;
    }

    return -1;
}


// void __fastcall VQA_DispatchFrameChunks(VQAHandleP *vqap, long frame) -- 0x00777A40
// Purpose: hands every cached MFCI and MSCI chunk recorded for the frame to
// the client's event handler (chunk ID, buffer, size).

void __fastcall VQA_DispatchFrameChunks(VQAHandleP *vqap, long frame)
{
    unsigned long chunkid;
    unsigned long count1;
    unsigned long count2;
    unsigned long i;
    unsigned long j;

    VQAMFCInfo *mfci_info = &vqap->MFCInfo;
    count1 = mfci_info->Header.Count;
    for (i = 0; i < count1; i++) {
        chunkid = mfci_info->Table[i].ChunkID;
        VQAChunkCache::DATA *mfci_data = mfci_info->Data2[i].Data;
        count2 = mfci_info->Data2[i].Count;
        for (j = 0; j < count2; j++) {
            if (mfci_data[j].Frame == frame) {
                vqap->Config.EventHandler((VQAHandle *)vqap, chunkid, (int)mfci_data[j].Buffer, mfci_data[j].Size);
            }
        }
    }

    VQAMSCInfo *msci_info = &vqap->MSCInfo;
    count1 = msci_info->Header.Count;
    for (i = 0; i < count1; i++) {
        chunkid = msci_info->Table[i].ChunkID;
        VQAChunkCache::DATA *msci_data = msci_info->Data2[i].Data;
        count2 = msci_info->Data2[i].Count;
        for (j = 0; j < count2; j++) {
            if (msci_data[j].Frame == frame) {
                vqap->Config.EventHandler((VQAHandle *)vqap, chunkid, (int)msci_data[j].Buffer, msci_data[j].Size);
            }
        }
    }
}


// void __fastcall VQA_UnVQFrame(VQAHandleP *vqap, VQAFrameNode *frame) -- 0x00777B50
// Purpose: decodes a frame into the image buffer (or the buffer the event
// handler's LOCK returns) with UnVQ1, or UnVQ2 for frames with residual
// data; reports a newly completed codebook first and unlocks after.

void __fastcall VQA_UnVQFrame(VQAHandleP *vqap, VQAFrameNode *frame)
{
    VQACBNode *codebk;
    VQAConfig *config;
    VQADrawer *drawer;

    config = &vqap->Config;
    drawer = &vqap->Drawer;
    codebk = frame->Codebook;

    if (!(drawer->Flags & VQADRWF_STEP)) {
        unsigned char *buf = 0;
        if (config->EventHandler != 0) {
            buf = (unsigned char *)config->EventHandler((VQAHandle *)vqap, VQAEVENT_LOCK, 0, 0);
        }
        if (buf == 0) {
            buf = drawer->ImageBuf;
        }
        if (buf == 0) {
            return;
        }

        unsigned char *buffer = buf + drawer->ScreenOffset;
        long blocksperrow = drawer->BlocksPerRow;
        long numrows = drawer->NumRows;
        unsigned char *codebook;
        unsigned char *pointers;

        if (frame->Flags & VQAFRMF_ALTPTR) {
            pointers = (unsigned char *)vqap->AltPtrBuffer;
        } else {
            pointers = frame->Pointers;
        }
        if (codebk->Flags & VQACBF_ALTPTR) {
            codebook = (unsigned char *)vqap->AltCBBuffer;
        } else {
            codebook = codebk->Buffer;
        }

        if (config->EventHandler != 0) {
            if (codebk->Flags & VQACBF_CBFULL) {
                codebk->Flags &= ~VQACBF_CBFULL;
                config->EventHandler((VQAHandle *)vqap, VQAEVENT_CODEBOOK, (int)codebook, codebk->CodebookSize);
            }
        }

        if (!(frame->Flags & VQAFRMF_RSDCOMP)) {
            vqap->UnVQ1(codebook, pointers, buffer, blocksperrow, numrows, drawer->ImageWidth);
        } else {
            vqap->UnVQ2(codebook, pointers, buffer, blocksperrow, numrows, drawer->ImageWidth);
        }

        if (config->EventHandler != 0) {
            config->EventHandler((VQAHandle *)vqap, VQAEVENT_UNLOCK, 0, 0);
        }
    }
}


// long __fastcall DrawFrame_MCGA(VQAHandle *vqa) -- 0x00777C50
// Purpose: the unbuffered frame drawer: selects and unpacks the next frame
// and hands it to the flipper (which decodes it), advancing unless held.

long __fastcall DrawFrame_MCGA(VQAHandle *vqa)
{
    VQAHandleP *vqap;
    VQADrawer *drawer;
    VQAFrameNode *curframe;
    long rc;

    vqap = (VQAHandleP *)vqa;
    drawer = &vqap->Drawer;

    rc = Select_Frame(vqap);
    if (rc != -1) {
        return rc;
    }

    Prepare_Frame(vqap);
    curframe = drawer->CurFrame;
    vqap->Flipper.CurFrame = curframe;
    if (!(drawer->Flags & VQADRWF_HOLD)) {
        drawer->CurFrame = curframe->Next;
    }
    return -1;
}


// long __fastcall Select_Frame(VQAHandleP *vqap) -- 0x00777C90
// Purpose: chooses the frame to draw now: waits for the loader, honors
// forced redraws and single stepping, compares the clock with the last
// drawn frame and skips late non-key frames (dispatching their palettes,
// loop events and chunks) up to a third of the frame rate.
// -1 draw, -9 frame not loaded, -10 not time yet, -2 stopped by the callback.

static long VQA_CalcFramesSinceDrawn(VQAHandleP *vqap)
{
    VQAConfig *config;
    VQADrawer *drawer;
    VQAFrameNode *curframe;
    long cur;
    long loop_end;
    int to_loop_end;
    int from_loop_start;
    int fallback;

    drawer = &vqap->Drawer;
    curframe = drawer->CurFrame;
    config = &vqap->Config;
    cur = curframe->FrameNum;
    loop_end = vqap->LoopEndFrame2;

    if (cur >= drawer->LastFrameNum && cur <= loop_end) {
        return cur - drawer->LastFrameNum;
    }

    fallback = config->FrameRate / 3;
    to_loop_end = loop_end - drawer->LastFrameNum;
    if (to_loop_end < 0) {
        return fallback;
    }
    from_loop_start = curframe->FrameNum - vqap->LoopStartFrame0;
    if (from_loop_start < 0) {
        return fallback;
    }
    return to_loop_end + from_loop_start + 1;
}

static long VQA_SetCurrentFrameAsLast(VQAHandleP *vqap)
{
    VQADrawer *drawer;

    drawer = &vqap->Drawer;
    drawer->LastFrameNum = drawer->CurFrame->FrameNum;
    return -1;
}

static void VQA_SetPreviousFrameNode(VQAHandleP *vqap)
{
    VQADrawer *drawer;

    drawer = &vqap->Drawer;
    drawer->CurFrame = drawer->CurFrame->Prev;
    drawer->CurFrame->Flags = drawer->CurFrame->PrevFlags;
}

long __fastcall Select_Frame(VQAHandleP *vqap)
{
    VQADrawer *drawer;
    VQAConfig *config;
    VQAFrameNode *curframe;
    long desiredframe;
    unsigned long curtime;

    config = &vqap->Config;
    drawer = &vqap->Drawer;
    curframe = drawer->CurFrame;

    if ((curframe->Flags & VQAFRMF_LOADED) == 0) {
        drawer->WaitsOnLoader++;
        if ((drawer->Flags & VQADRWF_REPAINT) && !(vqap->Flags & VQADATF_LDONE)) {
            VQA_SetPreviousFrameNode(vqap);
            if (drawer->Flags & VQADRWF_FORCEDRAW) {
                drawer->Flags |= VQADRWF_HOLD;
                curframe->Flags |= VQAFRMF_HOLD;
            }
            return -1;
        } else {
            return -9;
        }
    }

    if (drawer->Flags & VQADRWF_FORCEDRAW) {
        VQA_SetCurrentFrameAsLast(vqap);
        drawer->Flags |= VQADRWF_HOLD;
        curframe->Flags |= VQAFRMF_HOLD;
        return -1;
    }

    if (config->OptionFlags & VQAOPTF_STEP) {
        drawer->LastFrameNum = curframe->FrameNum;
        curtime = ((curframe->FrameNum * VQA_TIMETICKS) / config->DrawRate);
        VQA_SetTimer(vqap, curtime);
        return -1;
    }

    curtime = VQA_GetTime(vqap);

    long endtime = 0;
    if (!(drawer->Flags & VQADRWF_STEP)) {
        endtime = drawer->LastFrameNum + 1;
    } else {
        endtime = drawer->LastFrameNum + 2;
    }

    if ((int)curtime < ((endtime * VQA_TIMETICKS) / config->DrawRate)) {
        if (drawer->Flags & VQADRWF_REPAINT) {
            if (VQA_ComputeDesiredFrame(vqap, config, drawer, curframe, curtime) == drawer->LastFrameNum
                && (curframe->Flags & VQAFRMF_HOLD)) {
                drawer->LastFrameNum = curframe->FrameNum;
                return -1;
            } else {
                drawer->LastFrameNum = curframe->FrameNum;
                drawer->Flags |= VQADRWF_HOLD;
                curframe->Flags |= VQAFRMF_HOLD;
                return -1;
            }
        } else {
            return -10;
        }
    }

    desiredframe = VQA_ComputeDesiredFrame(vqap, config, drawer, curframe, curtime);

    if (config->DrawFlags & VQACFGF_NOSKIP) {
        return VQA_SetCurrentFrameAsLast(vqap);
    }

    long maxskip = (config->FrameRate / 3) - 1;
    if (maxskip < 0) {
        maxskip = 0;
    }

    while (curframe->FrameNum != desiredframe) {
        if (curframe->Flags & VQAFRMF_KEY) {
            break;
        }
        if (VQA_CalcFramesSinceDrawn(vqap) > maxskip) {
            break;
        }

        if (curframe->Flags & VQAFRMF_PALETTE) {
            if (curframe->Flags & VQAFRMF_PALCOMP) {
                curframe->PaletteSize = VQA_LCW_Uncompress((char *)curframe->Palette + curframe->PalOffset,
                                                           (char *)curframe->Palette, vqap->Max_Pal_Size);
                curframe->Flags &= ~VQAFRMF_PALCOMP;
            }
            memcpy(drawer->Palette_24, curframe->Palette, curframe->PaletteSize);
            drawer->CurPalSize = curframe->PaletteSize;
            drawer->Flags |= VQADRWF_SETPAL;
        }

        if (config->EventHandler != 0) {
            if (curframe->Flags & VQAFRMF_LOOPED) {
                config->EventHandler((VQAHandle *)vqap, VQAEVENT_LOOPED, curframe->FrameNum, vqap->LoopID);
                curframe->Flags &= ~VQAFRMF_LOOPED;
            }
            if (curframe->Flags & VQAFRMF_LOOPJMP) {
                config->EventHandler((VQAHandle *)vqap, VQAEVENT_LOOPJUMP, curframe->FrameNum, vqap->LoopID);
                curframe->Flags &= ~VQAFRMF_LOOPJMP;
            }
            if (curframe->Flags & VQAFRMF_CHUNKS) {
                VQA_DispatchFrameChunks(vqap, curframe->FrameNum);
                curframe->Flags &= ~VQAFRMF_CHUNKS;
            }
        }

        if (config->DrawerCallback != 0) {
            if (config->DrawerCallback(0, curframe->FrameNum) != 0) {
                return -2;
            }
        }

        curframe->PrevFlags = curframe->Flags;
        curframe->Flags = 0;
        curframe = curframe->Next;
        drawer->CurFrame = curframe;
        vqap->SkippedFrames++;

        if (!(curframe->Flags & VQAFRMF_LOADED)) {
            drawer->WaitsOnLoader++;
            if ((drawer->Flags & VQADRWF_REPAINT) && !(vqap->Flags & VQADATF_LDONE)) {
                VQA_SetPreviousFrameNode(vqap);
                return -1;
            }
            return -9;
        }
    }

    return VQA_SetCurrentFrameAsLast(vqap);
}


// long __fastcall VQA_ComputeDesiredFrame(VQAHandleP *vqap, VQAConfig *config, VQADrawer *drawer,
//                                         VQAFrameNode *frame, unsigned long time) -- 0x00778030
// Purpose: converts the clock into the frame that should be on screen,
// stepping the clock back over a pending loop wrap, holding it on the
// current frame with NOSKIP, and committing the queued loop bounds once the
// drawer has crossed the wrap.

long __fastcall VQA_ComputeDesiredFrame(VQAHandleP *vqap, VQAConfig *config, VQADrawer *drawer,
                                        VQAFrameNode *frame, unsigned long time)
{
    VQAHeader *header;
    long frame_rate;
    long frame_num;
    long result;
    bool crossed_loop;
    long loop_end;

    header = &vqap->Header;
    frame_rate = config->FrameRate;
    frame_num = frame->FrameNum;
    result = (time * frame_rate) / (int)VQA_TIMETICKS;
    if (drawer->Flags & VQADRWF_STEP) {
        result--;
    }

    crossed_loop = false;
    loop_end = vqap->LoopEndFrameMode2;
    if (vqap->Flags & VQADATF_LOOPED) {
        if (vqap->Flags & VQADATF_LOOPJMP) {
            loop_end = vqap->LoopEndFrame2;
        }
        while (true) {
            if ((config->DrawFlags & VQACFGF_NOSKIP) || result <= loop_end) {
                if (!(config->DrawFlags & VQACFGF_NOSKIP) || frame_num != vqap->LoopStartFrame0 || crossed_loop) {
                    break;
                }
            }
            int step_frames = vqap->LoopStartFrame0 - loop_end;
            step_frames--;
            VQA_StepTimer(vqap, VQA_TIMETICKS * step_frames / frame_rate);
            result = ((int)VQA_GetTime(vqap) * (int)frame_rate) / (int)VQA_TIMETICKS;
            crossed_loop = true;
            if (step_frames == 0) {
                break;
            }
        }
    }

    if (config->DrawFlags & VQACFGF_NOSKIP) {
        if (result > frame_num) {
            int step_frames = (frame_num - result);
            if (!(header->Flags & VQAHDF_AUDIO) || !(config->OptionFlags & VQAOPTF_AUDIO)) {
                VQA_StepTimer(vqap, VQA_TIMETICKS * step_frames / frame_rate);
            }
            result = frame_num;
            vqap->Flags |= VQADATF_FRAMESTALL;
        } else {
            vqap->Flags &= ~VQADATF_FRAMESTALL;
        }
    }

    if (crossed_loop) {
        if ((drawer->Flags & VQADRWF_STEP) && !(config->DrawFlags & VQACFGF_NOSKIP)) {
            result--;
            if (result < vqap->LoopStartFrame0) {
                result = loop_end;
            }
        }
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
        vqap->Flags &= ~(VQADATF_LOOPED);
        vqap->Flags &= ~(VQADATF_LOOPJMP);
    }

    return result;
}


// void __fastcall Prepare_Frame(VQAHandleP *vqap) -- 0x007781F0
// Purpose: unpacks the current frame's compressed codebook, palette and
// vector pointers (into the alternate buffers when those are in use).

void __fastcall Prepare_Frame(VQAHandleP *vqap)
{
    VQADrawer *drawer;
    VQAFrameNode *curframe;
    VQACBNode *codebook;

    drawer = &vqap->Drawer;
    curframe = drawer->CurFrame;
    codebook = curframe->Codebook;

    if (drawer->Flags & VQADRWF_STEP) {
        return;
    }

    if (codebook->Flags & VQACBF_CBCOMP) {
        void *cbbuffer = 0;
        if (vqap->AltBufferFlags & VQAABUFF_ALTCB) {
            cbbuffer = vqap->AltCBBuffer;
            codebook->Flags |= VQACBF_ALTPTR;
        } else {
            cbbuffer = codebook->Buffer;
        }
        codebook->CodebookSize = VQA_LCW_Uncompress((char *)(codebook->Buffer + codebook->CBOffset), (char *)cbbuffer,
                                                    vqap->CBBufferSize);
        codebook->Flags &= ~VQACBF_CBCOMP;
    }

    if (curframe->Flags & VQAFRMF_PALCOMP) {
        curframe->PaletteSize = VQA_LCW_Uncompress((char *)curframe->Palette + curframe->PalOffset, (char *)curframe->Palette,
                                                   vqap->Max_Pal_Size);
        curframe->Flags &= ~VQAFRMF_PALCOMP;
    }

    if (curframe->Flags & VQAFRMF_PTRCOMP) {
        void *ptrbuffer = 0;
        if (vqap->AltBufferFlags & VQAABUFF_ALTPTR) {
            ptrbuffer = vqap->AltPtrBuffer;
            curframe->Flags |= VQAFRMF_ALTPTR;
        } else {
            ptrbuffer = curframe->Pointers;
        }
        VQA_LCW_Uncompress((char *)curframe->Pointers + curframe->PtrOffset, (char *)ptrbuffer, vqap->PtrBufferSize);
        curframe->Flags &= ~VQAFRMF_PTRCOMP;
    }
}


// long __fastcall PageFlip_MCGA(VQAHandle *vqa) -- 0x007782C0
// Purpose: the unbuffered page flipper: reports a new palette to the event
// handler, decodes the frame and runs the drawer callback (-2 stops).

long __fastcall PageFlip_MCGA(VQAHandle *vqa)
{
    VQAHandleP *vqap;
    VQADrawer *drawer;
    VQAFrameNode *curframe;
    VQAConfig *config;
    unsigned char *pal;
    long palsize;

    vqap = (VQAHandleP *)vqa;
    config = &vqap->Config;
    drawer = &vqap->Drawer;
    curframe = vqap->Flipper.CurFrame;

    if ((curframe->Flags & VQAFRMF_PALETTE) || (drawer->Flags & VQADRWF_SETPAL)) {
        pal = curframe->Palette;
        palsize = curframe->PaletteSize;
        if (curframe->Flags & VQAFRMF_PALETTE) {
            if (config->EventHandler != 0) {
                config->EventHandler(vqa, VQAEVENT_PALETTE, (int)pal, palsize);
            }
        } else if (drawer->Flags & VQADRWF_SETPAL) {
            drawer->Flags &= ~VQADRWF_SETPAL;
            if (config->EventHandler != 0) {
                config->EventHandler(vqa, VQAEVENT_PALETTE, (int)drawer->Palette_24, drawer->CurPalSize);
            }
        }
    }

    VQA_UnVQFrame((VQAHandleP *)vqa, curframe);

    if (config->DrawerCallback != 0) {
        if (config->DrawerCallback(vqa, curframe->FrameNum) != 0) {
            return -2;
        }
    }
    return -1;
}


// long __fastcall DrawFrame_MCGABuf(VQAHandle *vqa) -- 0x00778350
// Purpose: the buffered frame drawer: selects, unpacks and decodes the next
// frame into the image buffer and hands it to the flipper.

long __fastcall DrawFrame_MCGABuf(VQAHandle *vqa)
{
    long rc;
    VQAHandleP *vqap;
    VQADrawer *drawer;
    VQAFrameNode *curframe;

    vqap = (VQAHandleP *)vqa;
    drawer = &vqap->Drawer;

    rc = Select_Frame(vqap);
    if (rc != -1) {
        return rc;
    }

    Prepare_Frame(vqap);
    curframe = drawer->CurFrame;
    VQA_UnVQFrame(vqap, curframe);
    vqap->Flipper.CurFrame = curframe;
    if ((drawer->Flags & VQADRWF_HOLD) == 0) {
        drawer->CurFrame = curframe->Next;
    }
    return -1;
}


// long __fastcall PageFlip_MCGABuf(VQAHandle *vqa) -- 0x007783A0
// Purpose: the buffered page flipper: reports a new palette to the event
// handler and runs the drawer callback (-2 stops); the drawer decoded it.

long __fastcall PageFlip_MCGABuf(VQAHandle *vqa)
{
    VQAHandleP *vqap;
    VQADrawer *drawer;
    VQAFrameNode *curframe;
    VQAConfig *config;
    unsigned char *pal;
    long palsize;

    vqap = (VQAHandleP *)vqa;
    config = &vqap->Config;
    drawer = &vqap->Drawer;
    curframe = vqap->Flipper.CurFrame;

    if ((curframe->Flags & VQAFRMF_PALETTE) || (drawer->Flags & VQADRWF_SETPAL)) {
        pal = curframe->Palette;
        palsize = curframe->PaletteSize;
        if (curframe->Flags & VQAFRMF_PALETTE) {
            if (config->EventHandler != 0) {
                config->EventHandler(vqa, VQAEVENT_PALETTE, (int)pal, palsize);
            }
        } else if (drawer->Flags & VQADRWF_SETPAL) {
            drawer->Flags &= ~VQADRWF_SETPAL;
            if (config->EventHandler != 0) {
                config->EventHandler(vqa, VQAEVENT_PALETTE, (int)drawer->Palette_24, drawer->CurPalSize);
            }
        }
    }

    if (config->DrawerCallback != 0) {
        if (config->DrawerCallback(vqa, curframe->FrameNum) != 0) {
            return -2;
        }
    }
    return -1;
}
