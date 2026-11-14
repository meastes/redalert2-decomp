// vqalib_loader.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_loader.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"
#include "shared/vqa_iff.hpp"
#include "shared/crt_memory.hpp"


// long __fastcall VQA_LoadFrame(VQAHandleP *vqap, long flags) -- 0x00778680
// Purpose: loads the next frame; while a loop is cached (OptionFlags 0x20, AltBufferFlags
// ALTLOOP) it fills the loop cache and reads the frame through the memory stream
// handler, and at a loop start it marks the current codebook full.

long __fastcall VQA_LoadFrame(VQAHandleP *vqap, long flags)
{
    VQAConfig *config;
    VQALoader *loader;
    VQALoopCache *cache;
    int frame;
    int loop_bytes;
    int scan_frame;
    int tocache;
    long rc;
    int fsize;
    VQACallback oldhandler;
    bool restore_handler;

    oldhandler = 0;
    long foffset = 0;
    config = &vqap->Config;
    loader = &vqap->Loader;
    long *foff = vqap->Foff;
    frame = loader->CurFrameNum;
    restore_handler = false;

    if ((config->OptionFlags & 0x20) && (vqap->AltBufferFlags & VQAABUFF_ALTLOOP) && vqap->LoopID >= 0) {
        if (frame < vqap->NumFrames) {
            foffset = (foff[frame] & 0x0FFFFFFF) << 1;
        }
        cache = &vqap->LoopCache;
        if (frame == vqap->LoopStartFrame0 && vqap->LoopID != cache->ID) {
            if (frame != cache->Min) {
                cache->Buffer = (char *)foffset;
                cache->Bytes = 0;
                cache->Offset = 0;
            }
            cache->ID = vqap->LoopID;
            cache->Min = vqap->LoopStartFrame0;
            cache->Max = -1;
            loop_bytes = 0;
            for (scan_frame = vqap->LoopStartFrame0; scan_frame <= vqap->LoopEndFrameMode2; scan_frame++) {
                if (scan_frame < vqap->NumFrames - 1) {
                    loop_bytes += ((foff[scan_frame + 1] & 0x0FFFFFFF) << 1) - ((foff[scan_frame] & 0x0FFFFFFF) << 1);
                } else {
                    if (config->StreamHandler((VQAHandle *)vqap, 9, (int)&fsize, 0)) {
                        cache->Max = scan_frame - 1;
                        break;
                    }
                    loop_bytes += fsize - ((foff[vqap->LoopEndFrameMode2] & 0x0FFFFFFF) << 1);
                }
                if (loop_bytes >= cache->Size) {
                    cache->Max = scan_frame - 1;
                    break;
                }
            }
            if (loop_bytes < cache->Size && cache->Max == -1) {
                cache->Max = vqap->LoopEndFrameMode2;
            }
        }
        if (frame >= cache->Min && frame <= cache->Max) {
            if (foffset == (long)cache->Buffer + cache->Bytes) {
                if (frame < vqap->NumFrames - 1) {
                    tocache = ((foff[frame + 1] & 0x0FFFFFFF) << 1) - foffset;
                } else {
                    if (!config->StreamHandler((VQAHandle *)vqap, 9, (int)&fsize, 0)) {
                        tocache = fsize - foffset;
                    } else {
                        tocache = 0;
                    }
                }
                if (tocache > 0) {
                    if (config->StreamHandler != Memory_VQA_Stream_Handler) {
                        config->StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 0, foffset);
                    }
                    if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&cache->Ptr[cache->Offset], tocache)) {
                        return -4;
                    }
                    cache->Bytes += tocache;
                }
            }
            if (foffset < (long)cache->Buffer + cache->Bytes) {
                restore_handler = true;
                cache->Offset = foffset - (long)cache->Buffer;
                oldhandler = config->StreamHandler;
                config->StreamHandler = Memory_VQA_Stream_Handler;
            }
        }
    }

    if (VQA_IsFrameStartOfLoop(vqap, frame)) {
        if ((flags & 0x20) == 0) {
            vqap->Loader.FullCB = vqap->Loader.CurCB;
        }
    }

    rc = VQA_LoadFrame_Internal(vqap, flags);

    if (restore_handler) {
        config->StreamHandler = oldhandler;
    }
    return rc;
}


// VQA_LoadFrame_Internal00778950: stream chunks into the current frame, including loop rewinds.
static long Load_CBF0(VQAHandleP *, unsigned long);
static long Load_CPL0(VQAHandleP *, unsigned long);
static long Load_VPT0(VQAHandleP *, unsigned long);
static long Load_VPTZ(VQAHandleP *, unsigned long);
long __fastcall Load_VQF(VQAHandleP *, unsigned long, char);
static long Load_SN2J(VQAHandleP *, unsigned long);

static long VQA_LoadLoop(VQAHandleP *vqap, long frame)
{
    long result=-1;
    bool skipcodebook=false;
    VQALoader *loader=&vqap->Loader;
    VQAHeader *header=&vqap->Header;
    VQAConfig *config=&vqap->Config;
    if(loader->CurFrame->Flags&VQAFRMF_LOADED) return -9;
    if(frame>=vqap->StopFrame || vqap->Foff==0) return -6;
    if((config->OptionFlags&VQAOPTF_AUDIO) && !(header->Flags&8)) return -6;
    if(header->ColorMode==0 && !VQA_IsFrameStartOfLoop(vqap,frame)) result=VQA_ReloadPalette(vqap,frame,1);
    if(result==-1) result=VQA_SeekGroup(vqap,frame,header->Groupsize,false,false,skipcodebook);
    loader->CurFrameNum=frame;
    if(result==-1) {
        long flags=0x20;
        if(skipcodebook) flags|=0x10;
        result=VQA_LoadFrame(vqap,flags);
    }
    return result;
}

static long VQA_MFCIIndexFromChunkID(VQAHandleP *vqap,unsigned long id)
{
    VQAMFCInfo::TABLE *table=vqap->MFCInfo.Table;
    unsigned long count=vqap->MFCInfo.Header.Count;
    for(unsigned long i=0;i<count;i++) { if(id==table[i].ChunkID) return i; }
    return -1;
}
static long VQA_MFCIReadData(VQAHandleP *vqap,unsigned long size,long index)
{
    VQALoader *loader=&vqap->Loader;
    VQAConfig *config=&vqap->Config;
    unsigned long padded=PADSIZE(size);
    VQAChunkCache *cache=&vqap->MFCInfo.Data2[index];
    unsigned long write_index=cache->WriteIndex;
    void *buffer=cache->Data[write_index].Buffer;
    cache->Data[write_index].Size=size;
    cache->Data[write_index].Frame=loader->CurFrameNum;
    ++write_index;
    if(write_index>=cache->Count) write_index=0;
    cache->WriteIndex=write_index;
    if(config->StreamHandler((VQAHandle *)vqap,VQACMD_READ,(int)buffer,padded)) return -4;
    loader->CurFrame->Flags|=VQAFRMF_CHUNKS;
    return -1;
}

static long VQA_MSCIIndexFromChunkID(VQAHandleP *vqap,unsigned long id)
{
    VQAMSCInfo::TABLE *table=vqap->MSCInfo.Table;
    unsigned long count=vqap->MSCInfo.Header.Count;
    for(unsigned long i=0;i<count;i++) { if(id==table[i].ChunkID) return i; }
    return -1;
}
static long VQA_MSCIReadData(VQAHandleP *vqap,unsigned long size,long index)
{
    VQALoader *loader=&vqap->Loader;
    VQAConfig *config=&vqap->Config;
    unsigned long padded=PADSIZE(size);
    VQAChunkCache *cache=&vqap->MSCInfo.Data2[index];
    unsigned long write_index=cache->WriteIndex;
    void *buffer=cache->Data[write_index].Buffer;
    cache->Data[write_index].Size=size;
    cache->Data[write_index].Frame=loader->CurFrameNum;
    ++write_index;
    if(write_index>=cache->Count) write_index=0;
    cache->WriteIndex=write_index;
    if(config->StreamHandler((VQAHandle *)vqap,VQACMD_READ,(int)buffer,padded)) return -4;
    loader->CurFrame->Flags|=VQAFRMF_CHUNKS;
    return -1;
}

long __fastcall VQA_LoadFrame_Internal(VQAHandleP *vqap,long flags)
{
    VQAConfig *config=&vqap->Config;
    VQALoader *loader=&vqap->Loader;
    VQADrawer *drawer=&vqap->Drawer;
    VQAFrameNode *curframe;
    VQAChunkHeader *chunk=&loader->CurChunkHdr;
    static unsigned long iffsize;
    int frame_loaded=0;
    bool loop_loaded=false;
    curframe=loader->CurFrame;
    long result;
    if(config->OptionFlags&VQAOPTF_LOOPS) {
        if(vqap->LoopIterations>0 || vqap->LoopIterations==-1) {
            if(loader->CurFrameNum>vqap->LoopEndFrameMode2) {
                long jump=vqap->LoopEndFrameJump;
                long old_end=vqap->LoopEndFrameMode2;
                long stopframe=vqap->StopFrame;
                if(jump!=-1) {
                    vqap->LoopEndFrameMode2=jump;
                    vqap->LoopEndFrameJump=-1;
                    vqap->StopFrame=jump;
                }
                if(loader->CurFrameNum!=vqap->LoopStartFrame0) {
                    loader->CurFrameNum=vqap->LoopStartFrame0;
                    result=VQA_LoadLoop(vqap,vqap->LoopStartFrame0);
                } else result=-1;
                if(result==-1 || result==-11) {
                    vqap->Flags|=VQADATF_LOOPED;
                    if(jump==-1) {
                        curframe->Flags|=VQAFRMF_LOOPED;
                        if(vqap->LoopIterations!=-1) vqap->LoopIterations--;
                    } else {
                        curframe->Flags|=VQAFRMF_LOOPJMP;
                        vqap->LoopEndFrame2=old_end;
                        vqap->LoopIterations=vqap->LoopIterationsJump;
                        vqap->LoopIterationsJump=-1;
                        vqap->Flags|=VQADATF_LOOPJMP;
                    }
                } else {
                    vqap->LoopEndFrameMode2=old_end;
                    vqap->LoopEndFrameJump=jump;
                    vqap->StopFrame=stopframe;
                    loader->CurFrameNum=vqap->LoopEndFrameMode2+1;
                }
                return result;
            }
        }
    }
    if(loader->CurFrameNum>vqap->StopFrame) {
        if(!(flags&2) && (config->OptionFlags&VQAOPTF_AUDIO) && !(config->OptionFlags&VQAOPTF_ALTAUDIO)) {
            if(CopyAudio(vqap)==-11) { vqap->Flags|=1; return -11; }
            vqap->Flags&=~1;
            vqap->Audio.Flags|=VQAAUDF_ISENDOFFILE;
        }
        return -2;
    }
    int skip;
    if((curframe->Flags&VQAFRMF_LOADED) && !(curframe->Flags&VQAFRMF_11)) { loader->WaitsOnDrawer++; return -9; }
    if(!(vqap->Flags&1)) {
        loader->FrameSize=0;
        curframe->Codebook=loader->FullCB;
    }
    while(frame_loaded==0) {
        if(!(vqap->Flags&1)) {
            if(config->StreamHandler((VQAHandle *)vqap,VQACMD_READ,(int)chunk,8)) return -2;
            iffsize=REVERSE_LONG(chunk->Size);
            loader->FrameSize+=iffsize;
        }
        skip=true;
        switch(chunk->ID) {
        case ID_VQFR:
            if((flags&0x1d)!=0x1d) {
                if(Load_VQF(vqap,iffsize,flags)!=-1) return -4;
                skip=false;
            }
            frame_loaded=true;
            break;
        case ID_VQFK:
            if((flags&0x1d)!=0x1d) {
                if(Load_VQF(vqap,iffsize,flags)!=-1) return -4;
                curframe->Flags|=VQAFRMF_KEY;
                skip=false;
            }
            frame_loaded=true;
            break;
        case ID_VQFL:
            if(!(flags&0x20)) {
                if(Load_VQF(vqap,iffsize,flags)!=-1) return -4;
                skip=false;
                loop_loaded=true;
                if((vqap->Header.Flags&0x10) && (flags&4)) frame_loaded=true;
            }
            break;
        case ID_CBF0:
            if(!(flags&16)) {
                if(Load_CBF0(vqap,iffsize)!=-1) return -4;
                
                skip=false;
            }
            break;
        case ID_CBFZ:
            if(!(flags&16)) {
                if(Load_CBFZ(vqap,iffsize)!=-1) return -4;
                
                skip=false;
            }
            break;
        case ID_CBP0:
            if(!(flags&8)) {
                if(Load_CBP0(vqap,iffsize)!=-1) return -4;
                
                skip=false;
            }
            break;
        case ID_CBPZ:
            if(!(flags&8)) {
                if(Load_CBPZ(vqap,iffsize)!=-1) return -4;
                
                skip=false;
            }
            break;
        case ID_CPL0:
            if(!(flags&1)) {
                if(Load_CPL0(vqap,iffsize)!=-1) return -4;
                curframe->Flags|=VQAFRMF_PALETTE;
                skip=false;
            }
            break;
        case ID_CPLZ:
            if(!(flags&1)) {
                if(Load_CPLZ(vqap,iffsize)!=-1) return -4;
                curframe->Flags|=VQAFRMF_PALETTE;
                skip=false;
            }
            break;
        case ID_VPT0:
        case ID_VPTD:
            if(!(flags&4)) {
                if(Load_VPT0(vqap,iffsize)!=-1) return -4;
                skip=false;
                
            }
            frame_loaded=true;
            break;
        case ID_VPKZ:
            if(!(flags&4)) {
                if(Load_VPTZ(vqap,iffsize)!=-1) return -4;
                skip=false;
                curframe->Flags|=VQAFRMF_KEY;
            }
            frame_loaded=true;
            break;
        case ID_VPTK:
            if(!(flags&4)) {
                if(Load_VPT0(vqap,iffsize)!=-1) return -4;
                skip=false;
                curframe->Flags|=VQAFRMF_KEY;
            }
            frame_loaded=true;
            break;
        case ID_VPTR:
            if(!(flags&4)) {
                if(Load_VPT0(vqap,iffsize)!=-1) return -4;
                skip=false;
                curframe->Flags|=VQAFRMF_RSDCOMP;
            }
            frame_loaded=true;
            break;
        case ID_VPRZ:
            if(!(flags&4)) {
                if(Load_VPTZ(vqap,iffsize)!=-1) return -4;
                skip=false;
                curframe->Flags|=VQAFRMF_RSDCOMP;
            }
            frame_loaded=true;
            break;
        case ID_VPTZ:
        case ID_VPDZ:
            if(!(flags&4)) {
                if(Load_VPTZ(vqap,iffsize)!=-1) return -4;
                skip=false;
                
            }
            frame_loaded=true;
            break;
        case ID_SND0:
            if(!(flags&2)) {
                if((config->OptionFlags&VQAOPTF_AUDIO) && !(config->OptionFlags&VQAOPTF_ALTAUDIO)) {
                    if(CopyAudio(vqap)==-11) { vqap->Flags|=1; return -11; }
                    else vqap->Flags&=~1;
                    if(Load_SND0(vqap,iffsize)!=-1) return -4;
                    skip=false;
                }
            }
            break;
        case ID_SNA0:
            if(!(flags&2)) {
                if((config->OptionFlags&VQAOPTF_AUDIO) && (config->OptionFlags&VQAOPTF_ALTAUDIO)) {
                    if(CopyAudio(vqap)==-11) { vqap->Flags|=1; return -11; }
                    else vqap->Flags&=~1;
                    if(Load_SND0(vqap,iffsize)!=-1) return -4;
                    skip=false;
                }
            }
            break;
        case ID_SND1:
            if(!(flags&2)) {
                if((config->OptionFlags&VQAOPTF_AUDIO) && !(config->OptionFlags&VQAOPTF_ALTAUDIO)) {
                    if(CopyAudio(vqap)==-11) { vqap->Flags|=1; return -11; }
                    else vqap->Flags&=~1;
                    if(Load_SND1(vqap,iffsize)!=-1) return -4;
                    skip=false;
                }
            }
            break;
        case ID_SNA1:
            if(!(flags&2)) {
                if((config->OptionFlags&VQAOPTF_AUDIO) && (config->OptionFlags&VQAOPTF_ALTAUDIO)) {
                    if(CopyAudio(vqap)==-11) { vqap->Flags|=1; return -11; }
                    else vqap->Flags&=~1;
                    if(Load_SND1(vqap,iffsize)!=-1) return -4;
                    skip=false;
                }
            }
            break;
        case ID_SND2:
            if(!(flags&2)) {
                if((config->OptionFlags&VQAOPTF_AUDIO) && !(config->OptionFlags&VQAOPTF_ALTAUDIO)) {
                    if(CopyAudio(vqap)==-11) { vqap->Flags|=1; return -11; }
                    else vqap->Flags&=~1;
                    if(Load_SND2(vqap,iffsize)!=-1) return -4;
                    skip=false;
                }
            }
            break;
        case ID_SNA2:
            if(!(flags&2)) {
                if((config->OptionFlags&VQAOPTF_AUDIO) && (config->OptionFlags&VQAOPTF_ALTAUDIO)) {
                    if(CopyAudio(vqap)==-11) { vqap->Flags|=1; return -11; }
                    else vqap->Flags&=~1;
                    if(Load_SND2(vqap,iffsize)!=-1) return -4;
                    skip=false;
                }
            }
            break;
        case ID_SN2J:
            if(!(flags&2)) {
                if((config->OptionFlags&VQAOPTF_AUDIO) && !(config->OptionFlags&VQAOPTF_ALTAUDIO)) {
                    if(Load_SN2J(vqap,iffsize)!=-1) return -4;
                    skip=false;
                }
            }
            break;
        default: {
            long index=VQA_MFCIIndexFromChunkID(vqap,chunk->ID);
            if(index>=0) {
                if(VQA_MFCIReadData(vqap,iffsize,index)!=-1) return -4;
                skip=false;
                break;
            }
            index=VQA_MSCIIndexFromChunkID(vqap,chunk->ID);
            if(index>=0) {
                if(VQA_MSCIReadData(vqap,iffsize,index)!=-1) return -4;
                skip=false;
                break;
            }
            break;
        }
        }
        if(skip==true) {
            if(config->StreamHandler((VQAHandle *)vqap,VQACMD_SEEK,1,PADSIZE(iffsize))) return -6;
        }
    }
    if(loader->CurFrameNum>0 && loader->FrameSize>loader->MaxFrameSize) loader->MaxFrameSize=loader->FrameSize;
    if(VQA_IsFrameStartOfLoop(vqap,loader->CurFrameNum) && !(flags&0x20) && !loop_loaded) {
        loader->FullCB=loader->CurCB->Prev;
        curframe->Codebook=loader->FullCB;
    }
    if(!(flags&4)) {
        curframe->FrameNum=loader->CurFrameNum;
        loader->LastFrameNum=loader->CurFrameNum;
        loader->PrevCB=curframe->Codebook;
        curframe->Flags|=VQAFRMF_LOADED;
        loader->CurFrame=curframe->Next;
    }
    loader->CurFrameNum++;
    return -1;
}

static long Load_SN2J(VQAHandleP *vqap,unsigned long size)
{
    VQAAudio *audio=&vqap->Audio;
    VQAConfig *config=&vqap->Config;
    unsigned long padded=PADSIZE(size);
#pragma pack(push,1)
    struct JumpState { unsigned short index; unsigned long predicted; unsigned short index2; unsigned long predicted2; } data;
#pragma pack(pop)
    if(!(config->OptionFlags&VQAOPTF_AUDIO) || audio->Buffer==0) {
        if(config->StreamHandler((VQAHandle *)vqap,VQACMD_SEEK,1,padded)) return -6;
        return -1;
    }
    if(padded<=sizeof(data)) {
        if(config->StreamHandler((VQAHandle *)vqap,VQACMD_READ,(int)&data,padded)) return -4;
    } else return -4;
    audio->ADPCM_Info.Predicted=data.predicted;
    audio->ADPCM_Info.Index=data.index;
    if(vqap->Channels==2) {
        audio->ADPCM_Info.Predicted2=data.predicted2;
        audio->ADPCM_Info.Index2=data.index2;
    }
    return -1;
}


// long __fastcall PrimeBuffers(VQAHandleP *vqap) -- 0x007796A0
// Purpose: fills the frame buffers before playback; -9/-11 are not errors.

long __fastcall PrimeBuffers(VQAHandleP *vqap)
{
    VQAConfig *config;
    long rc;
    long i;

    config = &vqap->Config;
    for (i = 0; i < config->NumFrameBufs; i++) {
        if ((rc = VQA_LoadFrame(vqap, 0x20)) == -1) {
            vqap->LoadedFrames++;
        } else if (rc != -9 && rc != -11) {
            return rc;
        }
    }
    return -1;
}


// bool __fastcall VQA_IsFrameStartOfLoop(VQAHandleP *vqap, long framenum) -- 0x007796F0
// Purpose: frame 0 or the start frame of a loop in the LINF table.

bool __fastcall VQA_IsFrameStartOfLoop(VQAHandleP *vqap, long framenum)
{
    int i;
    int count;
    VQALoopInfo::DATA *data;

    if (framenum == 0) {
        return true;
    }
    count = vqap->LoopInfo.Header.Count;
    if (count > 0 && ((vqap->LoopInfo.Header.Flags & 1) || (vqap->LoopInfo.Header.Flags & 2))) {
        data = vqap->LoopInfo.Data;
        for (i = 0; i < count; i++) {
            if (framenum == data[i].StartFrame) {
                return true;
            }
        }
    }
    return false;
}


// long __fastcall VQA_ReloadPalette(VQAHandleP *vqap, long framenum, int force) -- 0x00779740
// Purpose: loads the palette in effect at a frame (PINF table or FINF palette flags).

long __fastcall VQA_ReloadPalette(VQAHandleP *vqap, long framenum, int force)
{
    VQALoader *loader;
    VQAFrameNode *curframe;
    VQAConfig *config;
    long rc = -1;
    long pal_frame;
    long last_pal_frame;
    int i;

    loader = &vqap->Loader;
    curframe = loader->CurFrame;
    config = &vqap->Config;

    if (framenum > vqap->StopFrame || vqap->Foff == 0) {
        return -6;
    }
    if (vqap->PaletteInfo.Data != 0) {
        VQA_GetPaletteFrameRange(vqap, framenum, &pal_frame, 0);
        if (!force) {
            VQA_GetPaletteFrameRange(vqap, loader->LastFrameNum, &last_pal_frame, 0);
            if (pal_frame == last_pal_frame) {
                pal_frame = -1;
            }
        }
    } else {
        pal_frame = -1;
        for (i = framenum; i >= 0; i--) {
            if (vqap->Foff[i] & 0x40000000) {
                pal_frame = i;
                break;
            }
        }
        if (!force) {
            last_pal_frame = -1;
            for (i = loader->LastFrameNum; i >= 0; i--) {
                if (vqap->Foff[i] & 0x40000000) {
                    last_pal_frame = i;
                    break;
                }
            }
            if (pal_frame == last_pal_frame) {
                pal_frame = -1;
            }
        }
    }
    if (pal_frame >= 0) {
        if (!config->StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 0, (vqap->Foff[pal_frame] & 0x0FFFFFFF) << 1)) {
            if (force) {
                loader->NumPartialCB = 0;
                loader->PartialCBSize = 0;
                loader->FullCB = vqap->CBData;
                loader->CurCB = vqap->CBData;
                loader->CurFrameNum = 0;
                curframe->Flags = 0;
            }
            rc = VQA_LoadFrame(vqap, 0x1E);
        } else {
            rc = -6;
        }
    }
    return rc;
}


// long __fastcall VQA_GetPaletteFrameRange(VQAHandleP *vqap, long framenum, long *first_frame, long *last_frame) -- 0x007798C0
// Purpose: index and frame span of the PINF entry covering a frame.

long __fastcall VQA_GetPaletteFrameRange(VQAHandleP *vqap, long framenum, long *first_frame, long *last_frame)
{
    VQAPaletteInfo::DATA *data = vqap->PaletteInfo.Data;
    int count = vqap->PaletteInfo.Header.Count;
    int findex = 0;
    bool found = false;

    for (int i = 1; i < count; i++) {
        if (framenum >= data[i - 1].Frame) {
            if (framenum < data[i].Frame) {
                found = true;
                break;
            }
        }
        findex++;
    }
    if (!found) {
        findex = count - 1;
    }
    if (first_frame != 0) {
        *first_frame = data[findex].Frame;
    }
    if (last_frame != 0) {
        if (findex < count - 1) {
            *last_frame = data[findex + 1].Frame - 1;
        } else {
            *last_frame = vqap->NumFrames;
        }
    }
    return findex;
}


// long __fastcall VQA_SeekGroup(VQAHandleP *vqap, long framenum, long groupsize, bool preloadaudio, bool reset_state, bool &skipcodebook) -- 0x00779950
// Purpose: positions the loader at the codebook group holding a frame, rewinds the
// audio buffer for the preload and loads the frames up to the target
// (VQA_SeekLoop, a static helper expanded here, handles loop starts).

static long VQA_SeekLoop(VQAHandleP *vqap, long framenum, long flags)
{
    long rc = -1;
    VQALoopCache *cache;
    bool needs_seek = false;
    long *foff;

    cache = &vqap->LoopCache;
    foff = vqap->Foff;
    if ((vqap->AltBufferFlags & VQAABUFF_ALTLOOP) && framenum == cache->Min && cache->Bytes != 0) {
        if ((long)cache->Buffer + cache->Bytes <= ((foff[vqap->LoopEndFrameMode2] & 0x0FFFFFFF) << 1)) {
            needs_seek = true;
        }
        cache->Offset = 0;
    } else if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 0, (foff[framenum] & 0x0FFFFFFF) << 1) != 0) {
        return -6;
    }
    if (framenum > 0 && (!(flags & 0x10) || !(vqap->Header.Flags & 0x10))) {
        rc = VQA_LoadFrame(vqap, flags | 2 | 4);
    }
    if (rc == -1) {
        if (needs_seek && vqap->Config.StreamHandler((VQAHandle *)vqap, 8, 0, (long)cache->Buffer + cache->Bytes) != 0) {
            return -6;
        }
    }
    return rc;
}

long __fastcall VQA_SeekGroup(VQAHandleP *vqap, long framenum, long groupsize, bool preloadaudio, bool reset_state, bool &skipcodebook)
{
    VQALoader *loader;
    VQADrawer *drawer;
    VQAHeader *header;
    VQAFrameNode *frame;
    VQACBNode *curcb;
    VQAConfig *config;
    long group;
    long i;
    VQAAudio *audio;
    int audpreload;
    int bytes_per_frame;
    int preload_frames;
    int covered_bytes;
    long rc;
    int loadflags;
    long group_end;
    long group_start;
    int group_index;
    int last_group_index;
    bool cb_valid;
    long audio_start;

    loader = &vqap->Loader;
    drawer = &vqap->Drawer;
    header = &vqap->Header;
    config = &vqap->Config;
    audio = &vqap->Audio;
    group_index = 0;
    frame = drawer->CurFrame;

    if (vqap->Foff) {
        group = framenum;
        if (config->NumCBBufs > 1) {
            cb_valid = false;
            if (groupsize > 0) {
                group_index = framenum / groupsize;
                group_start = group_index * groupsize;
                if (group_start >= groupsize) {
                    group_start -= groupsize;
                }
                if (group_index == 0) {
                    group_end = -1;
                } else {
                    group_end = group_start + groupsize - 1;
                }
                last_group_index = loader->LastFrameNum / groupsize;
            } else {
                group_index = VQA_GetCodebookFrameRange(vqap, framenum, &group_start, &group_end);
                last_group_index = VQA_GetCodebookFrameRange(vqap, loader->LastFrameNum, 0, 0);
            }
            group = group_start;
            if (!reset_state) {
                if (last_group_index == group_index) {
                    group = group_end + 1;
                    loader->FullCB = loader->PrevCB;
                    loader->CurCB = loader->FullCB->Next;
                    cb_valid = true;
                } else if (loader->NumPartialCB == 0) {
                    bool dont_set_cur = false;
                    curcb = loader->FullCB;
                    while (true) {
                        if (curcb == frame->Codebook && (frame->Flags & 1) != 0) {
                            dont_set_cur = true;
                            break;
                        }
                        frame = frame->Next;
                        if (drawer->CurFrame == frame) {
                            break;
                        }
                    }
                    if (!dont_set_cur) {
                        loader->CurCB = curcb;
                    }
                }
            } else {
                loader->FullCB = vqap->CBData;
                loader->CurCB = vqap->CBData;
            }
            loader->NumPartialCB = 0;
            loader->PartialCBSize = 0;
        } else {
            cb_valid = true;
            group_start = framenum;
            group_end = framenum;
        }
        skipcodebook = cb_valid;

        if ((config->OptionFlags & 1) && (audio->Buffer != 0)) {
            int bytes_per_sec = ((vqap->SampleRate * vqap->Channels) * (vqap->BitsPerSample >> 3));
            bytes_per_frame = bytes_per_sec / config->FrameRate;
            audpreload = header->AudioPreload;
            if ((header->Flags & 8) == 0 && audpreload == 0) {
                audpreload = (long)(bytes_per_sec / 2);
            }
            if (preloadaudio && audpreload > 0) {
                preload_frames = audpreload / bytes_per_frame;
                covered_bytes = bytes_per_frame * preload_frames;
                if (audpreload > covered_bytes) {
                    ++preload_frames;
                }
                audio_start = framenum - preload_frames;
                if (audio_start < 0) {
                    audio_start = 0;
                    audio->BufferOffset = framenum * bytes_per_frame;
                } else {
                    audio->BufferOffset = bytes_per_frame + covered_bytes - audpreload;
                }
                if (audio_start < group) {
                    group = audio_start;
                }
            } else {
                audio_start = framenum;
                audio->BufferOffset = 0;
            }
            if (reset_state) {
                memset(audio->IsLoaded, 0, audio->NumAudBlocks * sizeof(*audio->IsLoaded));
                memset(audio->Buffer, 0, config->AudioBufSize);
                audio->TempBufLen = 0;
                if (audio_start == 0) {
                    audio->AudBufPos = 0;
                } else {
                    audio->AudBufPos = audpreload;
                    int blocks = int(audio->AudBufPos / config->HMIBufSize);
                    for (i = 0; i < blocks; i++) {
                        audio->IsLoaded[i] = 1;
                    }
                }
            }
        } else {
            preloadaudio = false;
            audio_start = framenum;
            audio->BufferOffset = 0;
        }

        if (VQA_IsFrameStartOfLoop(vqap, framenum)) {
            if (reset_state) {
                loader->CurFrame->Flags = 0;
            }
            if (cb_valid) {
                loadflags = 0x10;
            } else {
                loadflags = 0;
                loader->FullCB = loader->CurCB;
            }
            rc = VQA_SeekLoop(vqap, framenum, loadflags);
            if (reset_state) {
                if (rc == -9 || rc == -11) {
                    rc = -1;
                }
            }
            return rc;
        }

        if (!config->StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 0, (vqap->Foff[group] & 0x0FFFFFFF) << 1)) {
            loader->CurFrameNum = group;
            rc = -1;
            loadflags = 1 | 4;
            if (group_index) {
                loadflags |= 0x10;
            }
            if (!preloadaudio) {
                loadflags |= 2;
            }
            long loadframe = loader->CurFrameNum;
            for (i = 0; i < (framenum - group); i++) {
                if (reset_state) {
                    loader->CurFrame->Flags &= ~1;
                }
                if (preloadaudio) {
                    if (reset_state) {
                        if (loadframe == audio_start) {
                            audio->TempBufLen = 0;
                        }
                    }
                    if (loadframe < audio_start) {
                        loadflags |= 2;
                    } else {
                        loadflags &= ~2;
                    }
                }
                if (cb_valid && loadframe <= group_end) {
                    loadflags |= 8;
                } else {
                    loadflags &= ~8;
                }
                rc = VQA_LoadFrame(vqap, loadflags);
                if (rc != -1) {
                    if (!reset_state || ((rc != -9) && (rc != -11))) {
                        break;
                    } else {
                        rc = -1;
                    }
                }
                loadframe++;
            }
        } else {
            return -6;
        }
        return rc;
    }
    return -6;
}


// long __fastcall Load_VQF(VQAHandleP *vqap, unsigned long frame_iffsize, char flags) -- 0x00779E90
// Purpose: reads the chunks of one VQFR/VQFK frame container. The CBF0, CPL0, VPT0 and
// VPTZ readers are static helpers expanded here. VPKZ goes through its own
// copy of the VPTZ reader (Load_VPKZ): the expanded VPKZ case evaluates the
// buffer offset before the frame node, unlike the VPTZ/VPDZ/VPRZ cases
// (hypothesis that reproduces both orders).

static long Load_CBF0(VQAHandleP *vqap, unsigned long iffsize)
{
    VQALoader *loader;
    VQACBNode *curcb;

    loader = &vqap->Loader;
    curcb = loader->CurCB;
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)curcb->Buffer, PADSIZE(iffsize))) {
        return -4;
    }
    loader->NumPartialCB = 0;
    curcb->Flags = 0;
    curcb->CBOffset = 0;
    curcb->CodebookSize = iffsize;
    curcb->Flags |= 8;
    loader->FullCB = curcb;
    loader->CurCB = curcb->Next;
    return -1;
}
static long Load_CPL0(VQAHandleP *vqap, unsigned long iffsize)
{
    VQADrawer *drawer;
    VQAFrameNode *curframe;

    drawer = &vqap->Drawer;
    curframe = vqap->Loader.CurFrame;
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)curframe->Palette, PADSIZE(iffsize))) {
        return -4;
    }
    curframe->Flags &= ~8;
    curframe->PalOffset = 0;
    curframe->PaletteSize = iffsize;
    if (vqap->PaletteInfo.Header.Count == 1) {
        memcpy(drawer->Palette_24, curframe->Palette, curframe->PaletteSize);
        drawer->CurPalSize = curframe->PaletteSize;
        drawer->Flags |= 1;
    }
    return -1;
}
static long Load_VPT0(VQAHandleP *vqap, unsigned long iffsize)
{
    VQAFrameNode *curframe;

    curframe = vqap->Loader.CurFrame;
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)curframe->Pointers, PADSIZE(iffsize))) {
        return -4;
    }
    curframe->Flags &= ~0x10;
    curframe->PtrOffset = 0;
    return -1;
}
static long Load_VPTZ(VQAHandleP *vqap, unsigned long iffsize)
{
    VQAFrameNode *curframe;
    unsigned char *buffer;
    unsigned long padsize;
    unsigned long lcwoffset;

    curframe = vqap->Loader.CurFrame;
    padsize = PADSIZE(iffsize);
    lcwoffset = vqap->Max_Ptr_Size - padsize;
    buffer = curframe->Pointers + lcwoffset;
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)buffer, padsize)) {
        return -4;
    }
    curframe->Flags |= 0x10;
    curframe->PtrOffset = lcwoffset;
    if (vqap->Header.Version < 3) {
        buffer[iffsize + 1] = 0x80;
    }
    return -1;
}
static long Load_VPKZ(VQAHandleP *vqap, unsigned long iffsize)
{
    VQAFrameNode *curframe;
    unsigned char *buffer;
    unsigned long padsize;
    unsigned long lcwoffset;

    padsize = PADSIZE(iffsize);
    lcwoffset = vqap->Max_Ptr_Size - padsize;
    curframe = vqap->Loader.CurFrame;
    buffer = curframe->Pointers + lcwoffset;
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)buffer, padsize)) {
        return -4;
    }
    curframe->Flags |= 0x10;
    curframe->PtrOffset = lcwoffset;
    if (vqap->Header.Version < 3) {
        buffer[iffsize + 1] = 0x80;
    }
    return -1;
}

long __fastcall Load_VQF(VQAHandleP *vqap, unsigned long frame_iffsize, char flags)
{
    VQAFrameNode *curframe;
    VQAChunkHeader *chunk;
    unsigned long iffsize;
    unsigned long framesize;
    unsigned long bytes_loaded = 0;
    VQADrawer *drawer;
    VQAConfig *config;
    int skip;

    curframe = vqap->Loader.CurFrame;
    drawer = &(vqap->Drawer);
    chunk = &vqap->Loader.CurChunkHdr;
    config = &(vqap->Config);
    framesize = PADSIZE(frame_iffsize);

    while (bytes_loaded < framesize) {
        if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)chunk, sizeof(*chunk))) {
            return -2;
        }
        iffsize = REVERSE_LONG(chunk->Size);
        bytes_loaded += sizeof(*chunk);
        bytes_loaded += PADSIZE(iffsize);
        skip = true;
        switch (chunk->ID) {
        case ID_CBF0:
            if (flags & 0x10) {
                break;
            }
            if (Load_CBF0(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            break;
        case ID_CBFZ:
            if (flags & 0x10) {
                break;
            }
            if (Load_CBFZ(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            break;
        case ID_CBP0:
            if (flags & 8) {
                break;
            }
            if (Load_CBP0(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            break;
        case ID_CBPZ:
            if (flags & 8) {
                break;
            }
            if (Load_CBPZ(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            break;
        case ID_CPL0:
            if (flags & 1) {
                break;
            }
            if (Load_CPL0(vqap, iffsize) != -1) {
                return -4;
            }
            curframe->Flags |= 4;
            skip = false;
            break;
        case ID_CPLZ:
            if (flags & 1) {
                break;
            }
            if (Load_CPLZ(vqap, iffsize) != -1) {
                return -4;
            }
            curframe->Flags |= 4;
            skip = false;
            break;
        case ID_VPT0:
        case ID_VPTD:
            if (flags & 4) {
                break;
            }
            if (Load_VPT0(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            break;
        case ID_VPTZ:
        case ID_VPDZ:
            if (flags & 4) {
                break;
            }
            if (Load_VPTZ(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            break;
        case ID_VPTK:
            if (flags & 4) {
                break;
            }
            if (Load_VPT0(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            curframe->Flags |= 2;
            break;
        case ID_VPTR:
            if (flags & 4) {
                break;
            }
            if (Load_VPT0(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            curframe->Flags |= 0x20;
            break;
        case ID_VPKZ:
            if (flags & 4) {
                break;
            }
            if (Load_VPKZ(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            curframe->Flags |= 2;
            break;
        case ID_VPRZ:
            if (flags & 4) {
                break;
            }
            if (Load_VPTZ(vqap, iffsize) != -1) {
                return -4;
            }
            skip = false;
            curframe->Flags |= 0x20;
            break;
        default:
            return -4;
        }
        if (skip == true) {
            if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 1, PADSIZE(iffsize))) {
                return -6;
            }
        }
    }
    return -1;
}


// long __fastcall Load_CLIP(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077A340
// Purpose: reads the CLIP chunk into ClipWidth/ClipHeight.

long __fastcall Load_CLIP(VQAHandleP *vqap, unsigned long iffsize)
{
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&vqap->ClipWidth, PADSIZE(iffsize))) {
        return -4;
    }
    return -1;
}


// long __fastcall Load_LINF(VQAHandleP *vqap) -- 0x0077A360
// Purpose: reads the LINH loop header and the LIND loop table.

static long Load_LINH(VQAHandleP *vqap, unsigned long iffsize)
{
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&vqap->LoopInfo.Header, PADSIZE(iffsize))) {
        return -4;
    }
    return -1;
}
static long Load_LIND(VQAHandleP *vqap, unsigned long iffsize)
{
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)vqap->LoopInfo.Data, PADSIZE(iffsize))) {
        return -4;
    }
    return -1;
}

long __fastcall Load_LINF(VQAHandleP *vqap)
{
    unsigned long iffsize;
    VQAChunkHeader chunk;
    VQAConfig *config;

    config = &vqap->Config;
    if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&chunk, sizeof(chunk))) {
        return -2;
    }
    if (chunk.ID != ID_LINH) {
        return -4;
    }
    iffsize = REVERSE_LONG(chunk.Size);
    if (Load_LINH(vqap, iffsize) != -1) {
        return -4;
    }
    if (vqap->LoopInfo.Header.Count > 0) {
        long size = vqap->LoopInfo.Header.Count * sizeof(VQALoopInfo::DATA);
        vqap->LoopInfo.Data = (VQALoopInfo::DATA *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, size);
        if (vqap->LoopInfo.Data == 0) {
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)vqap->LoopInfo.Data, size);
        vqap->MemUsed += size;
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&chunk, sizeof(chunk))) {
            return -2;
        }
        if (chunk.ID != ID_LIND) {
            return -4;
        }
        size = REVERSE_LONG(chunk.Size);
        if (Load_LIND(vqap, size) != -1) {
            return -4;
        }
    }
    return -1;
}


static long Load_MFCH(VQAHandleP *player, unsigned long size)
{
    if (player->Config.StreamHandler((VQAHandle *)player, VQACMD_READ,
            (int)&player->MFCInfo.Header, PADSIZE(size))) return -4;
    return -1;
}
static long Load_MFCD(VQAHandleP *player, unsigned long size)
{
    if (player->Config.StreamHandler((VQAHandle *)player, VQACMD_READ,
            (int)player->MFCInfo.StaticData, PADSIZE(size))) return -4;
    return -1;
}
static long Load_MFCT(VQAHandleP *player, unsigned long size)
{
    if (player->Config.StreamHandler((VQAHandle *)player, VQACMD_READ,
            (int)player->MFCInfo.Table, PADSIZE(size))) return -4;
    return -1;
}

static int VQA_MFCISpansLoop(VQAHandleP *player, unsigned long chunk_id, unsigned long interval)
{
    int distance_to_end = 0;
    int distance_from_start = 0;
    unsigned loop_count = player->LoopInfo.Header.Count;
    VQALoopInfo::DATA *loops = player->LoopInfo.Data;
    unsigned key_count = player->MFCInfo.Header.StaticCount;
    VQAMFCInfo::DATA *keys = player->MFCInfo.StaticData;
    for (unsigned loop = 0; loop < loop_count; ++loop) {
        unsigned short *loop_end = &loops[loop].EndFrame;
        distance_from_start = -1;
        for (unsigned key = 0; key < key_count; ++key) {
            if (keys[key].ChunkID == chunk_id) {
                unsigned frame = keys[key].KeyFrame;
                unsigned start = loop_end[-1];
                if (frame >= start) {
                    unsigned end = *loop_end;
                    if (frame > end) break;
                    if (distance_from_start < 0) {
                        distance_from_start = frame - start;
                        distance_to_end = end - frame;
                    } else {
                        distance_to_end = end - frame;
                    }
                }
            }
        }
        if (distance_from_start > 0 &&
                distance_from_start + distance_to_end + 1 < interval) return 1;
    }
    return 0;
}

static long VQA_MFCICalcCount(VQAHandleP *player, unsigned long chunk_id,
    long buffered_frames, unsigned long interval)
{
    if (!interval) return 0;
    long slots = (buffered_frames - 1) / interval + 1;
    if (VQA_MFCISpansLoop(player, chunk_id, interval)) ++slots;
    return slots;
}

static long VQA_MFCIPrepare(VQAHandleP *player, unsigned long index)
{
    VQAConfig *config = &player->Config;
    VQAMFCInfo *info = &player->MFCInfo;
    VQAMFCInfo::TABLE *table = player->MFCInfo.Table;
    VQAChunkCache *cache = info->Data2;
    int entry_size = table[index].EntrySize;
    int slots = VQA_MFCICalcCount(player, table[index].ChunkID, config->NumFrameBufs, table[index].FrameInterval);
    if (slots > 0) {
        cache[index].Count = slots;
        cache[index].WriteIndex = 0;
        unsigned long allocation_size = sizeof(VQAChunkCache::DATA) * slots;
        VQAChunkCache::DATA *records = (VQAChunkCache::DATA *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, allocation_size);
        if (!records) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)records, allocation_size);
        player->MemUsed += allocation_size;
        cache[index].Data = records;
        allocation_size = slots * PADSIZE(entry_size);
        char *storage = (char *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, allocation_size);
        if (!storage) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)storage, allocation_size);
        player->MemUsed += allocation_size;
        for (int slot = 0; slot < slots; ++slot) {
            cache[index].Data[slot].Buffer = storage;
            cache[index].Data[slot].Frame = -1;
            storage += entry_size;
        }
    }
    return -1;
}

long __fastcall Load_MFCI(VQAHandleP *player)
{
    VQAChunkHeader chunk;
    VQAConfig *config = &player->Config;
    long size;
    unsigned long count, iffsize;
    if (config->StreamHandler((VQAHandle *)player, VQACMD_READ, (int)&chunk, sizeof(chunk))) return -2;
    if (chunk.ID != ID_MFCH) return -4;
    iffsize = REVERSE_LONG(chunk.Size);
    if (Load_MFCH(player, iffsize) != -1) return -4;
    count = player->MFCInfo.Header.StaticCount;
    if (count > 0) {
        size = count * sizeof(VQAMFCInfo::DATA);
        player->MFCInfo.StaticData = (VQAMFCInfo::DATA *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, size);
        if (!player->MFCInfo.StaticData) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)player->MFCInfo.StaticData, size);
        player->MemUsed += size;
        if (config->StreamHandler((VQAHandle *)player, VQACMD_READ, (int)&chunk, sizeof(chunk))) return -2;
        if (chunk.ID != ID_MFCD) return -4;
        size = REVERSE_LONG(chunk.Size);
        if (Load_MFCD(player, size) != -1) return -4;
    }
    count = player->MFCInfo.Header.Count;
    if (count > 0) {
        size = count * sizeof(VQAMFCInfo::TABLE);
        player->MFCInfo.Table = (VQAMFCInfo::TABLE *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, size);
        if (!player->MFCInfo.Table) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)player->MFCInfo.Table, size);
        player->MemUsed += size;
        if (config->StreamHandler((VQAHandle *)player, VQACMD_READ, (int)&chunk, sizeof(chunk))) return -2;
        if (chunk.ID != ID_MFCT) return -4;
        size = REVERSE_LONG(chunk.Size);
        if (Load_MFCT(player, size) != -1) return -4;
    }
    count = player->MFCInfo.Header.Count;
    if (count > 0) {
        size = count * sizeof(VQAChunkCache);
        player->MFCInfo.Data2 = (VQAChunkCache *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, size);
        if (!player->MFCInfo.Data2) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)player->MFCInfo.Data2, size);
        player->MemUsed += size;
        for (unsigned long entry = 0; entry < count; ++entry) {
            long result = VQA_MFCIPrepare(player, entry);
            if (result != -1) return result;
        }
    }
    return -1;
}


static long Load_MSCH(VQAHandleP *player, unsigned long size)
{
    if (player->Config.StreamHandler((VQAHandle *)player, VQACMD_READ,
            (int)&player->MSCInfo.Header, PADSIZE(size))) return -4;
    return -1;
}
static long Load_MSCT(VQAHandleP *player, unsigned long size)
{
    if (player->Config.StreamHandler((VQAHandle *)player, VQACMD_READ,
            (int)player->MSCInfo.Table, PADSIZE(size))) return -4;
    return -1;
}

static long VQA_MSCIPrepare(VQAHandleP *player, unsigned long index)
{
    VQAConfig *config = &player->Config;
    int slots = config->NumFrameBufs;
    VQAMSCInfo *info = &player->MSCInfo;
    VQAChunkCache *cache = info->Data2;
    VQAMSCInfo::TABLE *table = player->MSCInfo.Table;
    int entry_size = table[index].EntrySize;
    if (slots > 0) {
        cache[index].Count = slots;
        cache[index].WriteIndex = 0;
        unsigned long allocation_size = sizeof(VQAChunkCache::DATA) * slots;
        VQAChunkCache::DATA *records = (VQAChunkCache::DATA *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, allocation_size);
        if (!records) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)records, allocation_size);
        player->MemUsed += allocation_size;
        cache[index].Data = records;
        allocation_size = PADSIZE(entry_size) * slots;
        char *storage = (char *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, allocation_size);
        if (!storage) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)storage, allocation_size);
        player->MemUsed += allocation_size;
        for (int slot = 0; slot < slots; ++slot) {
            cache[index].Data[slot].Buffer = storage;
            cache[index].Data[slot].Frame = -1;
            storage += entry_size;
        }
    }
    return -1;
}

long __fastcall Load_MSCI(VQAHandleP *player)
{
    VQAChunkHeader chunk;
    VQAConfig *config = &player->Config;
    long size;
    unsigned long count, iffsize;
    if (config->StreamHandler((VQAHandle *)player, VQACMD_READ, (int)&chunk, sizeof(chunk))) return -2;
    if (chunk.ID != ID_MSCH) return -4;
    iffsize = REVERSE_LONG(chunk.Size);
    if (Load_MSCH(player, iffsize) != -1) return -4;
    VQAMSCInfo::HEADER *header = &player->MSCInfo.Header;
    count = header->Count;
    if (count > 0) {
        size = count * sizeof(VQAMSCInfo::TABLE);
        player->MSCInfo.Table = (VQAMSCInfo::TABLE *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, size);
        if (!player->MSCInfo.Table) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)player->MSCInfo.Table, size);
        player->MemUsed += size;
        if (config->StreamHandler((VQAHandle *)player, VQACMD_READ, (int)&chunk, sizeof(chunk))) return -2;
        if (chunk.ID != ID_MSCT) return -4;
        size = REVERSE_LONG(chunk.Size);
        if (Load_MSCT(player, size) != -1) return -4;
    }
    count = header->Count;
    if (count > 0) {
        size = count * sizeof(VQAChunkCache);
        player->MSCInfo.Data2 = (VQAChunkCache *)config->MemoryHandler(
            (VQAHandle *)player, VQAMEM_ALLOC, 0, size);
        if (!player->MSCInfo.Data2) return -8;
        config->MemoryHandler((VQAHandle *)player, VQAMEM_LOCK, (int)player->MSCInfo.Data2, size);
        player->MemUsed += size;
        for (unsigned long entry = 0; entry < count; ++entry) {
            long result = VQA_MSCIPrepare(player, entry);
            if (result != -1) return result;
        }
    }
    return -1;
}


// long __fastcall VQA_GetCodebookFrameRange(VQAHandleP *vqap, long framenum, long *first_frame, long *last_frame) -- 0x0077AC00
// Purpose: codebook group index of a frame from the CINF table (VQA_GetCodebookSize, the
// CINF size lookup of the partial codebook readers, is a static helper here).

static long VQA_GetCodebookSize(VQAHandleP *vqap, long framenum)
{
    VQACodebookInfo *info = &vqap->CodebookInfo;
    VQACodebookInfo::DATA *data = info->Data;
    int count = info->Header.Count;
    bool found = false;
    int findex;

    for (int i = 0; i < count; i++) {
        if (data[i].Frame > framenum) {
            findex = i;
            found = true;
            break;
        }
    }
    if (found == false) {
        return 0;
    }
    return data[findex].Size;
}

long __fastcall VQA_GetCodebookFrameRange(VQAHandleP *vqap, long framenum, long *first_frame, long *last_frame)
{
    VQACodebookInfo::DATA *data = vqap->CodebookInfo.Data;
    int count = vqap->CodebookInfo.Header.Count;
    unsigned int start_index;
    unsigned int end_index;
    int findex = 0;
    bool found = false;

    for (int i = 1; i < count; i++) {
        if (framenum >= data[i - 1].Frame) {
            if (framenum < data[i].Frame) {
                found = true;
                break;
            }
        }
        findex++;
    }
    if (!found) {
        findex = count - 1;
    }
    if (findex == 0) {
        start_index = 0;
        end_index = 0;
    } else {
        start_index = findex - 1;
        end_index = findex;
    }
    if (first_frame != 0) {
        *first_frame = data[start_index].Frame;
    }
    if (last_frame != 0) {
        *last_frame = data[end_index].Frame - 1;
    }
    return findex;
}


// long __fastcall Load_CINF(VQAHandleP *vqap) -- 0x0077AC80
// Purpose: reads the CINH codebook header and, without a group size, the CIND table.

static long Load_CINH(VQAHandleP *vqap, unsigned long iffsize)
{
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&vqap->CodebookInfo.Header, PADSIZE(iffsize))) {
        return -4;
    }
    return -1;
}
static long Load_CIND(VQAHandleP *vqap, unsigned long iffsize)
{
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)vqap->CodebookInfo.Data, PADSIZE(iffsize))) {
        return -4;
    }
    return -1;
}

long __fastcall Load_CINF(VQAHandleP *vqap)
{
    int groupsize;
    long size;
    unsigned long iffsize;
    VQAChunkHeader chunk;
    VQAConfig *config;

    config = &vqap->Config;
    if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&chunk, sizeof(chunk))) {
        return -2;
    }
    if (chunk.ID != ID_CINH) {
        return -4;
    }
    iffsize = REVERSE_LONG(chunk.Size);
    if (Load_CINH(vqap, iffsize) != -1) {
        return -4;
    }
    groupsize = vqap->Header.Groupsize;
    if (groupsize == 0 && vqap->CodebookInfo.Header.Count > 0) {
        size = vqap->CodebookInfo.Header.Count * sizeof(VQACodebookInfo::DATA);
        vqap->CodebookInfo.Data = (VQACodebookInfo::DATA *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, size);
        if (vqap->CodebookInfo.Data == 0) {
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)vqap->CodebookInfo.Data, size);
        vqap->MemUsed += size;
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&chunk, sizeof(chunk))) {
            return -2;
        }
        if (chunk.ID != ID_CIND) {
            return -4;
        }
        iffsize = REVERSE_LONG(chunk.Size);
        if (Load_CIND(vqap, iffsize) != -1) {
            return -4;
        }
    }
    return -1;
}


// long __fastcall Load_PINF(VQAHandleP *vqap) -- 0x0077ADF0
// Purpose: reads the PINH palette header and the PIND table.

static long Load_PINH(VQAHandleP *vqap, unsigned long iffsize)
{
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&vqap->PaletteInfo.Header, PADSIZE(iffsize))) {
        return -4;
    }
    return -1;
}
static long Load_PIND(VQAHandleP *vqap, unsigned long iffsize)
{
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)vqap->PaletteInfo.Data, PADSIZE(iffsize))) {
        return -4;
    }
    return -1;
}

long __fastcall Load_PINF(VQAHandleP *vqap)
{
    unsigned long iffsize;
    VQAChunkHeader chunk;
    VQAConfig *config;

    config = &vqap->Config;
    if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&chunk, sizeof(chunk))) {
        return -2;
    }
    if (chunk.ID != ID_PINH) {
        return -4;
    }
    iffsize = REVERSE_LONG(chunk.Size);
    if (Load_PINH(vqap, iffsize) != -1) {
        return -4;
    }
    if (vqap->PaletteInfo.Header.Count > 0) {
        long size = vqap->PaletteInfo.Header.Count * sizeof(VQAPaletteInfo::DATA);
        vqap->PaletteInfo.Data = (VQAPaletteInfo::DATA *)config->MemoryHandler((VQAHandle *)vqap, VQAMEM_ALLOC, 0, size);
        if (vqap->PaletteInfo.Data == 0) {
            return -8;
        }
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_LOCK, (int)vqap->PaletteInfo.Data, size);
        vqap->MemUsed += size;
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&chunk, sizeof(chunk))) {
            return -2;
        }
        if (chunk.ID != ID_PIND) {
            return -4;
        }
        iffsize = REVERSE_LONG(chunk.Size);
        if (Load_PIND(vqap, iffsize) != -1) {
            return -4;
        }
    }
    return -1;
}


// long __fastcall Load_FINF(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077AF50
// Purpose: reads the frame offset table, or skips it without a table.

long __fastcall Load_FINF(VQAHandleP *vqap, unsigned long iffsize)
{
    if (vqap->Foff != 0) {
        if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)vqap->Foff, PADSIZE(iffsize))) {
            return -4;
        }
    } else {
        if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 1, PADSIZE(iffsize))) {
            return -6;
        }
    }
    return -1;
}


// long __fastcall Load_CBFZ(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077AFA0
// Purpose: reads a compressed full codebook into the end of the codebook buffer.

long __fastcall Load_CBFZ(VQAHandleP *vqap, unsigned long iffsize)
{
    VQALoader *loader;
    VQACBNode *curcb;
    unsigned char *buffer;
    unsigned long padsize;
    unsigned long lcwoffset;

    loader = &vqap->Loader;
    curcb = loader->CurCB;
    padsize = PADSIZE(iffsize);
    lcwoffset = vqap->Max_CB_Size - padsize;
    buffer = curcb->Buffer + lcwoffset;
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)buffer, padsize)) {
        return -4;
    }
    loader->NumPartialCB = 0;
    curcb->Flags = 0;
    curcb->Flags |= 8 | 2;
    curcb->CBOffset = lcwoffset;
    loader->FullCB = curcb;
    loader->CurCB = curcb->Next;
    if (vqap->Header.Version < 3) {
        buffer[iffsize + 1] = 0x80;
    }
    return -1;
}


// long __fastcall Load_CBP0(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077B020
// Purpose: appends an uncompressed partial codebook; completes the codebook at the group size.

long __fastcall Load_CBP0(VQAHandleP *vqap, unsigned long iffsize)
{
    VQALoader *loader;
    VQACBNode *curcb;
    VQAHeader *header;
    unsigned char *buffer;
    int cbfull;

    loader = &vqap->Loader;
    curcb = loader->CurCB;
    header = &vqap->Header;
    if (header->Groupsize == 0 && loader->PartialCBSize == 0) {
        loader->CBSize = VQA_GetCodebookSize(vqap, loader->CurFrameNum);
    }
    buffer = curcb->Buffer + loader->PartialCBSize;
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)buffer, PADSIZE(iffsize))) {
        return -4;
    }
    loader->PartialCBSize += iffsize;
    loader->NumPartialCB++;
    cbfull = false;
    if (header->Groupsize != 0) {
        if (loader->NumPartialCB == vqap->Header.Groupsize) {
            cbfull = true;
        }
    } else {
        if (loader->PartialCBSize == loader->CBSize) {
            cbfull = true;
        } else {
            if (loader->PartialCBSize > loader->CBSize) {
                return -4;
            }
        }
    }
    if (cbfull) {
        curcb->CodebookSize = loader->PartialCBSize;
        loader->NumPartialCB = 0;
        loader->PartialCBSize = 0;
        curcb->Flags = 0;
        curcb->CBOffset = 0;
        curcb->Flags |= 8;
        loader->FullCB = curcb;
        loader->CurCB = curcb->Next;
    }
    return -1;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// long __fastcall Load_CBPZ(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077B140
// From ra2-decomp (YR 0x007C1F30).
// Purpose: appends a compressed partial codebook at the top of the codebook buffer; completes it at the group size.

long __fastcall Load_CBPZ(VQAHandleP *vqap, unsigned long iffsize)
{
    VQAConfig *config;
    VQALoader *loader;
    VQACBNode *curcb;
    void *buffer;
    unsigned long padsize;
    long groupsize;
    int cbfull;

    padsize = PADSIZE(iffsize);
    config = &vqap->Config;
    loader = &vqap->Loader;
    curcb = loader->CurCB;
    groupsize = vqap->Header.Groupsize;

    if (loader->PartialCBSize == 0) {
        if (groupsize != 0) {
            loader->CBSize = PADSIZE(groupsize) + groupsize * padsize;
        } else {
            loader->CBSize = VQA_GetCodebookSize(vqap, loader->CurFrameNum);
        }
        if (!(vqap->AltBufferFlags & VQAABUFF_ALTCB)) {
            curcb->CBOffset = vqap->Max_CB_Size - loader->CBSize;
        } else {
            curcb->CBOffset = 0;
        }
    }

    buffer = curcb->Buffer + curcb->CBOffset;
    buffer = (char *)buffer + loader->PartialCBSize;

    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)buffer, padsize)) {
        return -4;
    }

    loader->PartialCBSize += iffsize;
    loader->NumPartialCB++;
    cbfull = false;

    if (groupsize != 0) {
        if (loader->NumPartialCB == groupsize) {
            cbfull = true;
        }
    } else {
        if (loader->PartialCBSize == loader->CBSize) {
            cbfull = true;
        } else {
            if (loader->PartialCBSize > loader->CBSize) {
                return -4;
            }
        }
    }

    if (cbfull) {
        loader->NumPartialCB = 0;
        loader->PartialCBSize = 0;

        curcb->Flags = 0;
        curcb->Flags |= VQACBF_CBFULL | VQACBF_CBCOMP;

        loader->FullCB = curcb;
        loader->CurCB = curcb->Next;

        if (vqap->Header.Version < 3) {
            curcb->Buffer[loader->CBSize + curcb->CBOffset + 1] = 0x80;
        }
    }

    return -1;
}


// long __fastcall Load_CPLZ(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077B2C0
// Purpose: reads a compressed palette; a single-palette movie unpacks it at once.

long __fastcall Load_CPLZ(VQAHandleP *vqap, unsigned long iffsize)
{
    VQADrawer *drawer;
    VQAFrameNode *curframe;
    unsigned char *buffer;
    unsigned long padsize;
    unsigned long lcwoffset;

    curframe = vqap->Loader.CurFrame;
    padsize = PADSIZE(iffsize);
    drawer = &vqap->Drawer;
    lcwoffset = vqap->Max_Pal_Size - padsize;
    buffer = curframe->Palette + lcwoffset;
    if (vqap->Config.StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)buffer, padsize)) {
        return -4;
    }
    curframe->Flags |= 8;
    curframe->PalOffset = lcwoffset;
    curframe->PaletteSize = iffsize;
    if (vqap->Header.Version < 3) {
        buffer[iffsize + 1] = 0x80;
    }
    if (vqap->PaletteInfo.Header.Count == 1) {
        curframe->PaletteSize = VQA_LCW_Uncompress((char *)(curframe->Palette + curframe->PalOffset), (char *)curframe->Palette, vqap->Max_Pal_Size);
        curframe->Flags &= ~8;
        memcpy(drawer->Palette_24, curframe->Palette, curframe->PaletteSize);
        drawer->CurPalSize = curframe->PaletteSize;
        drawer->Flags |= 1;
    }
    return -1;
}


// long __fastcall Load_SND0(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077B390
// Purpose: reads raw audio into the audio buffer (startup chunk) or the temporary buffer.

long __fastcall Load_SND0(VQAHandleP *vqap, unsigned long iffsize)
{
    VQAAudio *audio;
    VQAConfig *config;
    unsigned long padsize;
    unsigned long i;

    audio = &vqap->Audio;
    config = &vqap->Config;
    padsize = PADSIZE(iffsize);
    if (((config->OptionFlags & 1) == 0) || (audio->Buffer == 0)) {
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 1, padsize)) {
            return -6;
        } else {
            return -1;
        }
    }
    if ((padsize > audio->TempBufSize) && (audio->AudBufPos == 0)) {
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)audio->Buffer, padsize)) {
            return -4;
        }
        audio->AudBufPos += iffsize;
        for (i = 0; i < (iffsize / config->HMIBufSize); i++) {
            audio->IsLoaded[i] = 1;
        }
        return -1;
    } else {
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)audio->TempBuf, padsize)) {
            return -4;
        }
    }
    audio->TempBufLen = iffsize;
    return -1;
}


// long __fastcall Load_SND1(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077B460
// Purpose: reads a ZAP-compressed audio frame and unpacks it.

long __fastcall Load_SND1(VQAHandleP *vqap, unsigned long iffsize)
{
    VQAAudio *audio;
    VQAConfig *config;
    unsigned char *loadbuf;
    unsigned long padsize;
    ZAPHeader zap;
    long i;

    audio = &vqap->Audio;
    config = &vqap->Config;
    padsize = PADSIZE(iffsize);
    if (((config->OptionFlags & 1) == 0) || (audio->Buffer == 0)) {
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 1, padsize)) {
            return -6;
        } else {
            return -1;
        }
    }
    if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)&zap, sizeof(ZAPHeader))) {
        return -4;
    }
    padsize -= sizeof(ZAPHeader);
    if ((zap.UnCompSize > audio->TempBufSize) && (audio->AudBufPos == 0)) {
        if (zap.UnCompSize == zap.CompSize) {
            if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)audio->Buffer, padsize)) {
                return -4;
            }
        } else {
            loadbuf = (audio->Buffer + config->AudioBufSize) - padsize;
            if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)loadbuf, padsize)) {
                return -4;
            }
            AudioUnzap(loadbuf, audio->Buffer, zap.UnCompSize);
        }
        audio->AudBufPos += zap.UnCompSize;
        for (i = 0; i < (zap.UnCompSize / config->HMIBufSize); i++) {
            audio->IsLoaded[i] = 1;
        }
        return -1;
    }
    if (zap.UnCompSize == zap.CompSize) {
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)audio->TempBuf, padsize)) {
            return -4;
        }
    } else {
        loadbuf = (audio->TempBuf + audio->TempBufSize) - padsize;
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)loadbuf, padsize)) {
            return -4;
        }
        AudioUnzap(loadbuf, audio->TempBuf, zap.UnCompSize);
    }
    audio->TempBufLen = zap.UnCompSize;
    return -1;
}


// long __fastcall Load_SND2(VQAHandleP *vqap, unsigned long iffsize) -- 0x0077B620
// Purpose: reads an IMA ADPCM audio frame and decodes it.

long __fastcall Load_SND2(VQAHandleP *vqap, unsigned long iffsize)
{
    VQAAudio *audio;
    VQAConfig *config;
    unsigned char *loadbuf;
    unsigned long padsize;
    unsigned long uncomp_size;
    long i;

    audio = &vqap->Audio;
    config = &vqap->Config;
    padsize = PADSIZE(iffsize);
    if (((config->OptionFlags & 1) == 0) || (audio->Buffer == 0)) {
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_SEEK, 1, padsize)) {
            return -6;
        } else {
            return -1;
        }
    }
    uncomp_size = iffsize * (vqap->BitsPerSample / 4);
    if ((uncomp_size > audio->TempBufSize) && (audio->AudBufPos == 0)) {
        loadbuf = (audio->Buffer + config->AudioBufSize) - padsize;
        if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)loadbuf, padsize)) {
            return -4;
        }
        VQA_sosCODECDecompressData(loadbuf, (char *)audio->Buffer, vqap->BitsPerSample, vqap->Channels, uncomp_size, &audio->ADPCM_Info);
        if (audio->BufferOffset) {
            uncomp_size -= audio->BufferOffset;
            memcpy(audio->Buffer, audio->Buffer + audio->BufferOffset, uncomp_size);
            audio->BufferOffset = 0;
        }
        audio->AudBufPos += uncomp_size;
        for (i = 0; i < (uncomp_size / config->HMIBufSize); i++) {
            audio->IsLoaded[i] = true;
        }
        return -1;
    }
    loadbuf = (audio->TempBuf + audio->TempBufSize) - padsize;
    if (config->StreamHandler((VQAHandle *)vqap, VQACMD_READ, (int)loadbuf, padsize)) {
        return -4;
    }
    VQA_sosCODECDecompressData(loadbuf, (char *)audio->TempBuf, vqap->BitsPerSample, vqap->Channels, uncomp_size, &audio->ADPCM_Info);
    audio->TempBufLen = uncomp_size;
    return -1;
}
