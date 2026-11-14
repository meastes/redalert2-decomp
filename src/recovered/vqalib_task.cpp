// vqalib_task.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_task.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"
#include "shared/vqa_iff.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"


// int __fastcall VQA_Unknown_0077B7A0(char const *filename, VQAConfig *_config, VQAHandle **handle) -- 0x0077B7A0
// Purpose: VQA_Open (TS): allocates and clears a private handle, copies the
// configuration (default memory and disk stream handlers), opens the
// stream, keeps the last 31 characters of the name, reads the FORM/WVQA
// header chunks up to FINF (VQHD, CINF, PINF, LINF, MFCI, MSCI, CLIP; others
// skipped), then resets playback, opens the audio, sizes the loop cache,
// sets the initial loop, configures the drawer and primes the buffers.
// Stores the handle and returns -1, else an error after closing.

int __fastcall VQA_Unknown_0077B7A0(char const *filename, VQAConfig *_config, VQAHandle **handle)
{
    VQAHandle *vqa;
    VQAHandleP *vqap;
    VQAHeader *header;
    VQAConfig *config;
    VQAChunkHeader chunk;
    bool done;

    *handle = 0;

    if (!_config) {
        return -17;
    }
    if (!_config->TimerCallback) {
        return -15;
    }
    if (_config->RefreshRate == 0) {
        return -16;
    }

    if (_config->MemoryHandler == 0) {
        vqa = (VQAHandle *)VQA_Memory_Handler(0, VQAMEM_ALLOC, 0, sizeof(VQAHandleP));
        if (vqa != 0) {
            VQA_Memory_Handler(vqa, VQAMEM_LOCK, (int)vqa, sizeof(VQAHandleP));
        } else {
            return -8;
        }
    } else {
        vqa = (VQAHandle *)_config->MemoryHandler(0, VQAMEM_ALLOC, 0, sizeof(VQAHandleP));
        if (vqa != 0) {
            _config->MemoryHandler(vqa, VQAMEM_LOCK, (int)vqa, sizeof(VQAHandleP));
        } else {
            return -8;
        }
    }

    vqap = (VQAHandleP *)vqa;
    memset(vqap, 0, sizeof(VQAHandleP));
    vqap->MemUsed = sizeof(VQAHandleP);
    memcpy(&vqap->Config, _config, sizeof(VQAConfig));
    config = &vqap->Config;

    if (config->MemoryHandler == 0) {
        config->MemoryHandler = VQA_Memory_Handler;
    }
    if (config->StreamHandler == 0) {
        config->StreamHandler = Disk_VQA_Stream_Handler;
    }

    header = &vqap->Header;

    if (config->StreamHandler(vqa, VQACMD_OPEN, (int)filename, 0)) {
        config->MemoryHandler(vqa, VQAMEM_UNLOCK, (int)vqa, sizeof(VQAHandleP));
        config->MemoryHandler(vqa, VQAMEM_FREE, (int)vqa, 0);
        return -3;
    }

    int pos = strlen(filename) - 31;
    if (pos < 0) pos = 0;
    strcpy(vqap->Filename, filename + pos);

    if (config->StreamHandler(vqa, VQACMD_READ, (int)&chunk, sizeof(chunk))) {
        VQA_Unknown_0077BE70(vqa);
        return -4;
    }
    if ((chunk.ID != ID_FORM) || (chunk.Size == 0)) {
        VQA_Unknown_0077BE70(vqa);
        return -7;
    }
    if (config->StreamHandler(vqa, VQACMD_READ, (int)&chunk, sizeof(chunk.ID))) {
        VQA_Unknown_0077BE70(vqa);
        return -4;
    }
    if (chunk.ID != ID_WVQA) {
        VQA_Unknown_0077BE70(vqa);
        return -7;
    }

    done = 0;
    while (!done) {
        if (config->StreamHandler(vqa, VQACMD_READ, (int)&chunk, sizeof(chunk))) {
            VQA_Unknown_0077BE70(vqa);
            return -4;
        }
        chunk.Size = REVERSE_LONG(chunk.Size);

        switch (chunk.ID) {
        case ID_VQHD:
            if (chunk.Size != sizeof(VQAHeader)) {
                VQA_Unknown_0077BE70(vqa);
                return -7;
            }
            if (config->StreamHandler(vqa, VQACMD_READ, (int)header, PADSIZE(chunk.Size))) {
                VQA_Unknown_0077BE70(vqa);
                return -4;
            }
            vqap->Version = header->Version;
            vqap->NumFrames = header->Frames;
            vqap->ImageWidth = header->ImageWidth;
            vqap->ImageHeight = header->ImageHeight;
            vqap->ColorMode = header->ColorMode;
            vqap->FrameRate = header->FPS;
            if (config->FrameRate == -1) {
                config->FrameRate = header->FPS;
            }
            if (config->DrawRate == -1) {
                config->DrawRate = header->FPS;
            }
            if ((config->DrawRate == -1) || (config->DrawRate == 0)) {
                config->DrawRate = header->FPS;
            }
            if ((header->Version > VQAHD_VER1) && !(header->Flags & VQAHDF_ALTAUDIO)) {
                config->OptionFlags &= ~VQAOPTF_ALTAUDIO;
            }
            break;

        case ID_CLIP:
            if (Load_CLIP(vqap, chunk.Size) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -4;
            }
            break;

        case ID_LINF:
            if (Load_LINF(vqap) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -4;
            }
            break;

        case ID_MFCI:
            if (Load_MFCI(vqap) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -4;
            }
            break;

        case ID_CINF:
            if (Load_CINF(vqap) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -4;
            }
            break;

        case ID_PINF:
            if (Load_PINF(vqap) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -4;
            }
            break;

        case ID_FINF:
            if (AllocBuffers(vqap) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -8;
            }
            if (Load_FINF(vqap, chunk.Size) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -4;
            }
            done = true;
            break;

        case ID_MSCI:
            if (Load_MSCI(vqap) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -4;
            }
            break;

        default:
            if (config->StreamHandler(vqa, VQACMD_SEEK, 1, PADSIZE(chunk.Size))) {
                VQA_Unknown_0077BE70(vqa);
                return -6;
            }
            break;
        }
    }

    if (header->ColorMode == 0 && vqap->PaletteInfo.Header.Count == 0) {
        vqap->PaletteInfo.Header.Count = (unsigned short)VQA_NumFramesWithPalettes(vqap);
    }

    VQA_Unknown_0077C7D0((VQAHandle *)vqap);

    if ((header->Flags & VQAHDF_AUDIO) == 0) {
        config->OptionFlags &= ~VQAOPTF_AUDIO;
    }

    if (config->OptionFlags & VQAOPTF_AUDIO) {
        VQAAudio *audio;
        if (config->AudioHandler == 0) {
            VQA_Unknown_0077BE70(vqa);
            return -21;
        }
        audio = &vqap->Audio;
        if (config->OptionFlags & VQAOPTF_ALTAUDIO) {
            vqap->SampleRate = header->AltSampleRate;
            vqap->Channels = header->AltChannels;
            vqap->BitsPerSample = header->AltBitsPerSample;
        } else {
            vqap->SampleRate = header->SampleRate;
            vqap->Channels = header->Channels;
            vqap->BitsPerSample = header->BitsPerSample;
        }
        if (VQA_OpenAudio(vqap) != -1) {
            VQA_Unknown_0077BE70(vqa);
            return -13;
        }
        Lock_Audio_Handler();
        audio->Flags |= VQAAUDF_MEMLOCKED;
        VQA_sosCODECInitStream(&audio->ADPCM_Info);
    } else {
        if (config->AudioHandler != 0) {
            vqap->Config.AudioHandler(vqa, VQAAUDIO_INIT, 0, 0);
        }
    }

    if (config->OptionFlags & VQAOPTF_LOOPS) {
        if (config->OptionFlags & VQAOPTF_LOOPCACHE) {
            VQALoopInfo *info = &vqap->LoopInfo;
            if (info->Header.Count > 0) {
                long cached;
                long size;
                if (config->StreamHandler(vqa, VQACMD_SIZE, (int)&size, 0) != 0) {
                    size = 0;
                }
                VQALoopCache *cache = &vqap->LoopCache;
                cache->Size = VQA_LargestLoop(vqap, size);
                config->MemoryHandler(vqa, VQAMEM_QUERYSIZE, (int)&cached, 0);
                if (cached < 0) {
                    if (size > 0) {
                        cached = size;
                    } else {
                        cached = 0;
                    }
                }
                if (cache->Size > cached) {
                    cache->Size = cached;
                }
                if (cache->Size > 0) {
                    void *ptr = (void *)config->MemoryHandler(vqa, VQAMEM_ALLOC, 0, vqap->LoopCache.Size);
                    cache->Ptr = (char *)ptr;
                    if (ptr == 0) {
                        VQA_Unknown_0077BE70(vqa);
                        return -8;
                    }
                    config->MemoryHandler(vqa, VQAMEM_LOCK, (int)vqap->LoopCache.Ptr, vqap->LoopCache.Size);
                    vqap->AltBufferFlags |= VQAABUFF_ALTLOOP;
                    vqap->MemUsed += vqap->LoopCache.Size;
                    VQA_ResetCache(vqap);
                }
            }
        }

        if (config->InitialLoopID >= 0) {
            if (VQA_Unknown_0077C640(vqa, config->InitialLoopID, config->InitialLoopIterations, 2) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -23;
            }
        } else {
            if (VQA_Unknown_0077C6C0(vqa, 0, vqap->StopFrame, 0, 0) != -1) {
                VQA_Unknown_0077BE70(vqa);
                return -23;
            }
        }
    } else {
        vqap->LoopEndFrameMode2 = vqap->StopFrame;
    }

    if (VQA_Configure_Drawer(vqap) != -1) {
        return -3;
    }

    if (!(config->OptionFlags & VQAOPTF_LOOPS) || config->InitialLoopID < 0) {
        if (PrimeBuffers(vqap) != -1) {
            VQA_Unknown_0077BE70(vqa);
            return -4;
        }
    }

    *handle = vqa;
    return -1;
}


// void __fastcall VQA_Unknown_0077BE70(VQAHandle *vqa) -- 0x0077BE70
// Purpose: VQA_Close (TS): closes the audio (and releases the game's audio
// lock), frees the private image buffer and all player buffers, closes the
// stream and frees the handle.

void __fastcall VQA_Unknown_0077BE70(VQAHandle *vqa)
{
    VQAHandleP *vqap;
    VQAConfig *config;
    VQAAudio *audio;

    vqap = (VQAHandleP *)vqa;
    config = &vqap->Config;
    audio = &vqap->Audio;

    if (config->OptionFlags & VQAOPTF_AUDIO && vqap->Header.Flags & (VQAHDF_AUDIO | VQAHDF_ALTAUDIO)) {
        VQA_CloseAudio(vqap);
        if (audio->Flags & VQAAUDF_MEMLOCKED) {
            Unlock_Audio_Handler();
            audio->Flags &= ~VQAAUDF_MEMLOCKED;
        }
    }

    if (vqap->Flags & VQADATF_ALTIMG && vqap->AltImageBuf != 0) {
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap->AltImageBuf,
                              vqap->AltImageWidth * vqap->AltImageHeight);
        config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap->AltImageBuf, 0);
    }

    FreeBuffers(vqap);
    config->StreamHandler((VQAHandle *)vqap, VQACMD_CLOSE, 0, 0);
    config->MemoryHandler((VQAHandle *)vqap, VQAMEM_UNLOCK, (int)vqap, sizeof(VQAHandleP));
    config->MemoryHandler((VQAHandle *)vqap, VQAMEM_FREE, (int)vqap, 0);
}


// int __fastcall VQA_Unknown_0077BF10(VQAHandle *vqa, int mode, int flags) -- 0x0077BF10
// Purpose: VQA_Play (TS): primes playback on the first call (clock, image
// buffer, audio), tracks the client callback count, then pauses (drawing
// one forced frame with VQAPLAYF_REPAINT) or runs the load/draw loop until
// both loader and drawer are done (one frame in walk mode), honoring the
// wait-for-refill audio sync. Returns the last drawn frame, -2 at the end.

int __fastcall VQA_Unknown_0077BF10(VQAHandle *vqa, int mode, int flags)
{
    VQAHandleP *vqabuf;
    VQAConfig *config;
    VQADrawer *drawer;
    VQAAudio *audio;
    int rc;
    long curtime;

    rc = -1;
    vqabuf = ((VQAHandleP *)vqa);
    drawer = &vqabuf->Drawer;
    config = &vqabuf->Config;
    audio = &vqabuf->Audio;

    if (!(vqabuf->Flags & VQADATF_PRIMED)) {
        rc = VQA_ResetLastFrameNum(vqa);
        if (rc != -1) {
            return rc;
        }
        if (!(vqabuf->Flags & VQADATF_BUFCONFIG)) {
            VQA_Configure_Buffer((VQAHandleP *)vqa);
        }
        if ((config->OptionFlags & VQAOPTF_AUDIO) && vqabuf->Audio.IsLoaded[0]) {
            VQA_StartAudio((VQAHandleP *)vqabuf);
        }
        curtime = ((drawer->CurFrame->FrameNum * VQA_TIMETICKS) / config->DrawRate);
        VQA_SetTimer((VQAHandleP *)vqabuf, curtime);
        ((VQAHandleP *)vqa)->StartTime = VQA_GetTime((VQAHandleP *)vqabuf);
        vqabuf->Flags |= VQADATF_PRIMED;
    }

    int count;
    if (config->UnusedCallback != 0) {
        count = config->UnusedCallback(vqa);
    } else {
        count = -1;
    }
    int lastcount = vqabuf->LastCallbackCount;
    if (lastcount == -1) {
        vqabuf->LastCallbackCount = count;
        vqabuf->MaxCallbackDelta = 0;
    } else {
        int delta = count - lastcount;
        vqabuf->LastCallbackCount = count;
        if (delta > vqabuf->MaxCallbackDelta) {
            vqabuf->MaxCallbackDelta = delta;
        }
    }

    switch (mode) {
    case VQAMODE_PAUSE:
        if ((vqabuf->Flags & VQADATF_PAUSED) == 0) {
            vqabuf->Flags |= VQADATF_PAUSED;
            ((VQAHandleP *)vqa)->EndTime = VQA_GetTime((VQAHandleP *)vqabuf);
            if (vqabuf->Audio.Flags & VQAAUDF_ISPLAYING) {
                VQA_PauseAudio((VQAHandleP *)vqabuf);
            }
        }
        if (flags & VQAPLAYF_REPAINT) {
            drawer->Flags |= VQADRWF_REPAINT;
            drawer->Flags |= VQADRWF_FORCEDRAW;
            if ((rc = (*(vqabuf->Draw_Frame))(vqa)) == -1) {
                if ((rc = User_Update(vqa)) != -1) {
                    vqabuf->Flags |= (VQADATF_DDONE | VQADATF_LDONE);
                } else {
                    rc = drawer->LastFrameNum;
                }
            } else {
                if (rc == -9 && (vqabuf->Flags & VQADATF_LDONE)) {
                    vqabuf->Flags |= (VQADATF_DDONE);
                }
            }
            drawer->Flags &= ~VQADRWF_STEP;
            drawer->Flags &= ~VQADRWF_REPAINT;
            drawer->Flags &= ~VQADRWF_FORCEDRAW;
        } else {
            rc = -14;
        }
        break;

    case VQAMODE_RUN:
    case VQAMODE_WALK:
    default:
        if (vqabuf->Flags & VQADATF_PAUSED) {
            vqabuf->Flags &= ~VQADATF_PAUSED;
            if (config->OptionFlags & VQAOPTF_AUDIO) {
                VQA_StartAudio((VQAHandleP *)vqabuf);
            }
            VQA_SetTimer((VQAHandleP *)vqabuf, ((VQAHandleP *)vqa)->EndTime);
        }

        while ((vqabuf->Flags & (VQADATF_DDONE | VQADATF_LDONE)) != (VQADATF_DDONE | VQADATF_LDONE)) {
            if (!(vqabuf->Flags & VQADATF_LDONE)) {
                if ((rc = VQA_LoadFrame(vqabuf, 0)) == -1) {
                    ((VQAHandleP *)vqa)->LoadedFrames++;
                } else {
                    if ((rc != -9) && (rc != -11)) {
                        vqabuf->Flags |= VQADATF_LDONE;
                    }
                }

                if (config->OptionFlags & VQAOPTF_WAITFILL) {
                    if (rc == -9 || (vqabuf->Flags & VQADATF_LDONE)) {
                        if (vqabuf->Flags & VQADATF_AUDIOSYNC) {
                            if (!(vqabuf->Flags & VQADATF_FRAMESTALL)) {
                                vqabuf->Flags |= VQADATF_REFILLED;
                                if (config->OptionFlags & VQAOPTF_AUDIO) {
                                    VQA_StartAudio(vqabuf);
                                }
                            }
                        }
                    } else if (!(vqabuf->Flags & VQADATF_LDONE)) {
                        if (vqabuf->Audio.Flags & VQAAUDF_ISSTARVED) {
                            if (vqabuf->Audio.Flags & VQAAUDF_ISPLAYING) {
                                VQA_PauseAudio(vqabuf);
                            }
                            vqabuf->Flags |= VQADATF_AUDIOSYNC;
                            audio->Flags &= ~VQAAUDF_ISSTARVED;
                        }
                    }
                }
            }

            if ((config->DrawFlags & VQACFGF_NODRAW) == 0) {
                if (vqabuf->Config.OptionFlags & VQAOPTF_WAITFILL) {
                    if (vqabuf->Flags & VQADATF_AUDIOSYNC) {
                        if (vqabuf->Flags & VQADATF_REFILLED) {
                            vqabuf->Flags &= ~VQADATF_AUDIOSYNC;
                            vqabuf->Flags &= ~VQADATF_REFILLED;
                        } else if ((vqabuf->Flags & VQADATF_FRAMESTALL) == 0) {
                            rc = -10;
                            break;
                        }
                    }
                }

                if (flags & VQAPLAYF_STEP) {
                    drawer->Flags |= VQADRWF_STEP;
                } else if (flags & VQAPLAYF_REPAINT) {
                    drawer->Flags |= VQADRWF_REPAINT;
                }

                if ((rc = ((VQAHandleP *)vqabuf)->Draw_Frame((VQAHandle *)vqabuf)) == -1) {
                    if (!(drawer->Flags & VQADRWF_HOLD)) {
                        ((VQAHandleP *)vqa)->DrawnFrames++;
                    }
                    if ((rc = User_Update((VQAHandle *)vqabuf)) != -1) {
                        vqabuf->Flags |= (VQADATF_DDONE | VQADATF_LDONE);
                    } else {
                        rc = drawer->LastFrameNum;
                    }
                } else {
                    if ((rc == -9) && (vqabuf->Flags & VQADATF_LDONE)) {
                        vqabuf->Flags |= VQADATF_DDONE;
                    }
                }
                drawer->Flags &= ~VQADRWF_STEP;
                drawer->Flags &= ~VQADRWF_REPAINT;
            } else {
                vqabuf->Flags |= VQADATF_DDONE;
                drawer->CurFrame->Flags = 0;
                drawer->CurFrame = drawer->CurFrame->Next;
            }

            if (mode == VQAMODE_WALK) {
                break;
            }
        }
        break;
    }

    if (((vqabuf->Flags & (VQADATF_DDONE | VQADATF_LDONE)) == (VQADATF_DDONE | VQADATF_LDONE))
        || (mode == VQAMODE_STOP)) {
        ((VQAHandleP *)vqa)->EndTime = VQA_GetTime((VQAHandleP *)vqabuf);
        if (vqabuf->Audio.Flags & VQAAUDF_ISPLAYING) {
            VQA_StopAudio((VQAHandleP *)vqabuf);
        }
        rc = -2;
    }
    return rc;
}


// int __fastcall VQA_Unknown_0077C2E0(VQAHandle *vqa, int framenum, int fromwhere) -- 0x0077C2E0
// Purpose: VQA_SeekFrame (TS): repositions playback at a frame up to the
// stop frame: stops the audio, empties the loop cache, reloads the
// palette in effect there (color mode 0, not a loop start), seeks the
// loader to the frame's group, clears the frame ring, primes the buffers
// and sets the clock. Returns the frame, or -6 past the stop frame.

int __fastcall VQA_Unknown_0077C2E0(VQAHandle *vqa, int framenum, int fromwhere)
{
    VQAHandleP *vqap;
    VQALoader *loader;
    VQAHeader *header;
    VQAFrameNode *frame;
    VQAConfig *config;
    int rc = -1;
    VQAAudio *audio;
    long audio_on;

    vqap = (VQAHandleP *)vqa;
    loader = &vqap->Loader;
    header = &vqap->Header;
    config = &vqap->Config;
    frame = loader->CurFrame;

    audio = &vqap->Audio;
    audio_on = (audio->Flags & VQAAUDF_ISPLAYING);
    if (audio_on) {
        VQA_StopAudio(vqap);
    }

    if ((framenum <= vqap->StopFrame) && (vqap->Foff != 0)) {
        VQA_ResetCache(vqap);
        VQA_ResetLastFrameNum(vqa);

        bool has_palette = false;
        if (header->ColorMode == 0 && !VQA_IsFrameStartOfLoop(vqap, framenum)) {
            rc = VQA_ReloadPalette(vqap, framenum, true);
            if (rc == -1) {
                if (frame->Flags & VQAFRMF_PALETTE) {
                    has_palette = true;
                }
            }
        }

        if (rc == -1) {
            loader->CurFrameNum = framenum;
            bool skipcodebook;
            bool preloadaudio = (((unsigned char)header->Flags) & VQAHDF_SNDJUMP) == 0;
            rc = VQA_SeekGroup(vqap, framenum, header->Groupsize, preloadaudio, 1, skipcodebook);
            if (rc == -1) {
                loader->CurFrameNum = framenum;
                loader->CurFrame->Flags = 0;
                if (has_palette) {
                    loader->CurFrame->Flags |= VQAFRMF_PALETTE;
                }
                frame = loader->CurFrame->Next;
                while (frame != loader->CurFrame) {
                    frame->Flags = 0;
                    frame = frame->Next;
                }
                vqap->Drawer.CurFrame = loader->CurFrame;
                rc = PrimeBuffers(vqap);
                if ((rc == -1) || (rc == -2)) {
                    rc = framenum;
                    VQA_SetTimer(vqap, VQA_TIMETICKS * framenum / config->DrawRate);
                }
            }
        }
        return rc;
    } else {
        rc = -6;
    }
    return rc;
}


// int __fastcall VQA_Unknown_0077C440(VQAHandle *vqa, UNVQ_FUNC unvq1, UNVQ_FUNC unvq2) -- 0x0077C440
// Purpose: VQA_SetUnVQ (TS): installs caller-supplied UnVQ decoders; a null
// pointer installs UnVQ_Nop.

int __fastcall VQA_Unknown_0077C440(VQAHandle *vqa, UNVQ_FUNC unvq1, UNVQ_FUNC unvq2)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;

    if (unvq1 != 0) {
        vqap->UnVQ1 = unvq1;
    } else {
        vqap->UnVQ1 = UnVQ_Nop;
    }
    if (unvq2 != 0) {
        vqap->UnVQ2 = unvq2;
    } else {
        vqap->UnVQ2 = UnVQ_Nop;
    }
    return -1;
}


// int __fastcall VQA_Unknown_0077C480(VQAHandle *vqa, int buffer, int width, int height, int xpos, int ypos) -- 0x0077C480
// Purpose: VQA_Set_DrawBuffer (TS): sets the image buffer and its size
// (0 keeps the movie's) and places the movie in it: centered (or at the
// movie's offset with DrawFlags bit 3) without a position, else at the
// position relative to the configured origin corner, halved for half-size
// playback. -24 when the origin falls outside the buffer.

int __fastcall VQA_Unknown_0077C480(VQAHandle *vqa, int buffer, int width, int height, int xpos, int ypos)
{
    long origin;
    VQAHeader *header;
    VQADrawer *drawer;
    VQAConfig *config;

    header = &((VQAHandleP *)vqa)->Header;
    drawer = &((VQAHandleP *)vqa)->Drawer;
    config = &((VQAHandleP *)vqa)->Config;

    origin = (config->DrawFlags & VQACFGF_ORIGIN);

    if (width == 0) {
        width = header->ImageWidth;
    }
    if (height == 0) {
        height = header->ImageHeight;
    }

    if (xpos < 0 && ypos < 0) {
        if (config->DrawFlags & VQACFGF_OFFSET) {
            if (config->DrawFlags & VQACFGF_HALFSIZE) {
                drawer->X1 = (header->Xpos / 2);
                drawer->Y1 = (header->Ypos / 2);
            } else {
                drawer->X1 = header->Xpos;
                drawer->Y1 = header->Ypos;
            }
        } else {
            if (config->DrawFlags & VQACFGF_HALFSIZE) {
                drawer->X1 = ((unsigned)width - (header->ImageWidth / 2)) / 2;
                drawer->Y1 = ((unsigned)height - (header->ImageHeight / 2)) / 2;
            } else {
                drawer->X1 = ((unsigned)width - header->ImageWidth) / 2;
                drawer->Y1 = ((unsigned)height - header->ImageHeight) / 2;
            }
        }
    } else {
        if (config->DrawFlags & VQACFGF_HALFSIZE) {
            xpos /= 2;
            ypos /= 2;
        }
        switch (origin) {
        default:
        case VQACFGF_TOPLEFT:
            drawer->X1 = xpos;
            drawer->Y1 = ypos;
            break;

        case VQACFGF_BOTLEFT:
            drawer->X1 = xpos;
            drawer->Y1 = (height - ypos);
            break;

        case VQACFGF_BOTRIGHT:
            drawer->X1 = (width - xpos);
            drawer->Y1 = (height - ypos);
            break;
        }
    }

    if (drawer->X1 >= 0 && drawer->Y1 >= 0 && (unsigned)drawer->X1 < (unsigned)width
        && (unsigned)drawer->Y1 < (unsigned)height) {
        long x = drawer->X1;
        long y = drawer->Y1;
        if (header->ColorMode == 1 || header->ColorMode == 4) {
            x *= 2;
            y *= 2;
        }
        drawer->ImageWidth = width;
        drawer->ImageHeight = height;
        drawer->ScreenOffset = ((width * y) + x);
        if (buffer != 0) {
            drawer->ImageBuf = (unsigned char *)buffer;
            ((VQAHandleP *)vqa)->ImageBuf = (void *)buffer;
        }
        ((VQAHandleP *)vqa)->Flags |= VQADATF_BUFCONFIG;
        return -1;
    }
    return -24;
}


// int __fastcall VQA_Unknown_0077C640(VQAHandle *vqa, int id, int iterations, int mode) -- 0x0077C640
// Purpose: VQA_SetLoop (TS): plays loop `id` of the LINF table
// (VQA_SetLoop_Internal with its frame range) and records it as the
// current loop. -23 for an unknown loop.

int __fastcall VQA_Unknown_0077C640(VQAHandle *vqa, int id, int iterations, int mode)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    VQAConfig *config = &vqap->Config;

    if (id >= vqap->LoopInfo.Header.Count || id < 0) {
        return -23;
    }
    if ((config->OptionFlags & VQAOPTF_AUDIO) && vqap->LoopInfo.Header.Count == 0) {
        return -23;
    }

    VQALoopInfo::DATA *data = &vqap->LoopInfo.Data[id];
    int start = data->StartFrame;
    int end = data->EndFrame;
    vqap->LoopID = id;
    int rc = VQA_Unknown_0077C6C0(vqa, start, end, iterations, mode);
    if (rc == -1) {
        vqap->LoopID = id;
    }
    return rc;
}


// int __fastcall VQA_Unknown_0077C6C0(VQAHandle *vqa, int start, int end, int iterations, int mode) -- 0x0077C6C0
// Purpose: VQA_SetLoop_Internal (TS): queues a loop over frames start..end.
// Mode 0 loops after the current loop, mode 1 jumps at the current loop's
// end (mode 2 when no loop is running), mode 2 seeks to the loop now.
// A negative iteration count loops forever. -23 for a bad range or mode.

int __fastcall VQA_Unknown_0077C6C0(VQAHandle *vqa, int start, int end, int iterations, int mode)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    int rc = -1;

    if (start < vqap->NumFrames && end < vqap->NumFrames && start < end && mode >= VQALOOP_NORMAL
        && mode < VQALOOP_3) {
        if (iterations < 0) {
            iterations = -1;
        }
        if (vqap->LoopIterations == 0 && mode == VQALOOP_JUMP) {
            mode = VQALOOP_2;
        }
        if (mode != VQALOOP_2) {
            if (vqap->Flags & VQADATF_LOOPED) {
                if (mode == VQALOOP_NORMAL) {
                    vqap->LoopStartFrame1 = start;
                    vqap->LoopEndFrameNormal = end;
                    end = vqap->LoopEndFrameMode2;
                } else {
                    vqap->LoopStartFrame1 = start;
                    start = vqap->LoopStartFrame0;
                    vqap->LoopEndFrameNormal = vqap->LoopEndFrameMode2;
                }
            } else {
                vqap->LoopStartFrame2 = start;
                vqap->LoopEndFrame2 = end;
            }
        }
        vqap->LoopStartFrame0 = start;
        if (mode == VQALOOP_JUMP) {
            vqap->LoopIterationsJump = iterations;
            vqap->LoopEndFrameJump = end;
        } else {
            vqap->LoopIterations = iterations;
            if (mode == VQALOOP_2) {
                vqap->LoopEndFrameMode2 = end;
                vqap->StopFrame = end;
                if (VQA_Unknown_0077C2E0(vqa, start, 0) >= 0) {
                    rc = -1;
                } else {
                    rc = -23;
                }
            }
        }
    } else {
        rc = -23;
    }
    return rc;
}


// int __fastcall VQA_GetLoopCount(VQAHandleP *vqap) -- 0x0077C7C0
// Purpose: the number of loops in the LINF table.

int __fastcall VQA_GetLoopCount(VQAHandleP *vqap)
{
    return vqap->LoopInfo.Header.Count;
}


// void __fastcall VQA_ResetCache(VQAHandleP *vqap) -- 0x0077C880
// Purpose: empties the loop cache window (ID, Min and Max -1).

void __fastcall VQA_ResetCache(VQAHandleP *vqap)
{
    vqap->LoopCache.ID = -1;
    vqap->LoopCache.Min = -1;
    vqap->LoopCache.Max = -1;
}


// int __fastcall VQA_Unknown_0077C8A0(VQAHandle *vqa, int stop) -- 0x0077C8A0
// Purpose: VQA_SetStop (TS): sets the frame playback stops at (1..Frames)
// and returns the previous one, else -1.

int __fastcall VQA_Unknown_0077C8A0(VQAHandle *vqa, int stop)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    int oldstop = -1;
    VQAHeader *header;

    header = &vqap->Header;
    if ((stop > 0) && (header->Frames >= stop)) {
        oldstop = vqap->StopFrame;
        vqap->StopFrame = (unsigned short)stop;
    }
    return oldstop;
}


// char *__fastcall VQA_Version() -- 0x0077C8D0
// Purpose: the library version string.


char *__fastcall VQA_Version()
{
    return "5.0";
}


// int __fastcall VQA_GetClipping(VQAHandleP *vqap, int &clipw, long &cliph) -- 0x0077C8E0
// Purpose: the clip size from the CLIP chunk, else the image size.

int __fastcall VQA_GetClipping(VQAHandleP *vqap, int &clipw, long &cliph)
{
    if (vqap->ClipWidth > 0) {
        clipw = vqap->ClipWidth;
        cliph = vqap->ClipHeight;
    } else {
        clipw = vqap->ImageWidth;
        cliph = vqap->ImageHeight;
    }
    return -1;
}


// int __fastcall VQA_Unknown_0077C920(VQAHandle *vqa, int *blockw, int *blockh, int *clrmode) -- 0x0077C920
// Purpose: VQA_GetBlockInfo (TS): the block width, height and color mode.

int __fastcall VQA_Unknown_0077C920(VQAHandle *vqa, int *blockw, int *blockh, int *clrmode)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    VQAHeader *header;

    header = &vqap->Header;
    *blockw = header->BlockWidth;
    *blockh = header->BlockHeight;
    *clrmode = header->ColorMode;
    return -1;
}


// char *__fastcall VQA_IDString() -- 0x0077C950
// Purpose: the library identification string.


char *__fastcall VQA_IDString()
{
    return "VQA playback 5.0 (Jun  6 2001 16:25:28)";
}


// long __fastcall User_Update(VQAHandle *vqa) -- 0x0077C960
// Purpose: shows the drawn frame: reports its loop events and chunks, runs
// the page flipper, and clears the frame's flags (or ends a held redraw).

long __fastcall User_Update(VQAHandle *vqa)
{
    VQAHandleP *vqap;
    VQAFrameNode *curframe;
    VQADrawer *drawer;
    VQAConfig *config;
    long rc = 0;

    vqap = (VQAHandleP *)vqa;
    drawer = &vqap->Drawer;
    config = &vqap->Config;
    curframe = vqap->Flipper.CurFrame;

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

    rc = vqap->Page_Flip((VQAHandle *)vqap);
    vqap->Flipper.LastFrameNum = curframe->FrameNum;

    if (!(drawer->Flags & VQADRWF_HOLD)) {
        curframe->PrevFlags = curframe->Flags;
        curframe->Flags = 0;
    } else {
        if (drawer->Flags & VQADRWF_FORCEDRAW) {
            curframe->Flags |= VQAFRMF_11;
            drawer->Flags &= ~VQADRWF_FORCEDRAW;
        }
        drawer->Flags &= ~VQADRWF_HOLD;
    }
    return rc;
}


// long __fastcall VQA_NumFramesWithPalettes(VQAHandleP *vqap) -- 0x0077CA30
// Purpose: counts the frames whose FINF entry carries a palette.

long __fastcall VQA_NumFramesWithPalettes(VQAHandleP *vqap)
{
    long num = 0;

    for (int i = 0; i < vqap->NumFrames; i++) {
        if (vqap->Foff[i] & VQAFINF_PAL) {
            num++;
        }
    }
    return num;
}


// int __fastcall VQA_GetXYPos(VQAHandleP *vqap, int &x, long &y) -- 0x0077CA50
// Purpose: the movie's position from its header.

int __fastcall VQA_GetXYPos(VQAHandleP *vqap, int &x, long &y)
{
    VQAHeader *header;

    header = &vqap->Header;
    x = header->Xpos;
    y = header->Ypos;
    return -1;
}


// long __fastcall VQA_LargestLoop(VQAHandleP *vqap, long streamsize) -- 0x0077CA70
// Purpose: the size in bytes of the largest loop of the LINF table (from
// the FINF frame offsets; a loop ending at the last frame ends at
// streamsize when that is known).

long __fastcall VQA_LargestLoop(VQAHandleP *vqap, long streamsize)
{
    VQAInfoHeader *infohdr = &vqap->LoopInfo.Header;
    long largest = 0;
    int count = infohdr->Count;

    for (int i = 0; i < count; i++) {
        VQALoopInfo::DATA *data = &vqap->LoopInfo.Data[i];
        int start = data->StartFrame;
        int stop = data->EndFrame;
        long start_offset;
        long end_offset;
        long loopsize = 0;

        if (stop != vqap->NumFrames - 1) {
            end_offset = VQAFRAME_OFFSET(vqap->Foff[stop + 1]);
            start_offset = VQAFRAME_OFFSET(vqap->Foff[start]);
            loopsize = end_offset - start_offset;
        } else if (streamsize > 0) {
            end_offset = streamsize;
            start_offset = VQAFRAME_OFFSET(vqap->Foff[start]);
            loopsize = end_offset - start_offset;
        } else {
            end_offset = VQAFRAME_OFFSET(vqap->Foff[stop]);
            start_offset = VQAFRAME_OFFSET(vqap->Foff[start]);
            loopsize = end_offset - start_offset;
        }
        if (loopsize > largest) {
            largest = loopsize;
        }
    }
    return largest;
}

// Not exact (RA2_NON_MATCHING): VQA_Reset at 0x0077C7D0, best score 66.67, 2026-11-13; differs: 36 instructions in the original, 39 in the draft (shape 77.33)
#ifdef RA2_NON_MATCHING
// void __fastcall VQA_Unknown_0077C7D0(VQAHandle *vqa) -- 0x0077C7D0
// Purpose: VQA_Reset (TS): clears the playback counters, loop state, timing
// and audio block positions, and the loop cache window (VQA_ResetCache,
// defined after it in the unit and expanded inline here).

void __fastcall VQA_Unknown_0077C7D0(VQAHandle *vqa)
{
    VQAHandleP *vqap;
    VQAAudio *audio;
    VQAHeader *header;

    vqap = (VQAHandleP *)vqa;
    audio = &vqap->Audio;
    header = &vqap->Header;

    int stop = header->Frames - 1;
    vqap->LoadedFrames = 0;
    vqap->DrawnFrames = 0;
    vqap->StopFrame = stop;
    vqap->LoopEndFrameMode2 = stop;
    vqap->LoopEndFrame2 = stop;
    unsigned long audio_flags = audio->Flags & ~(VQAAUDF_ISREPEATING | VQAAUDF_ISENDOFFILE | VQAAUDF_ISDONE | VQAAUDF_ISSTARVED);
    vqap->StartTime = 0;
    audio->Flags = audio_flags;
    vqap->EndTime = 0;
    vqap->Flags = 0;
    vqap->LoopIterations = 0;
    vqap->LoopID = -1;
    vqap->LoopStartFrame0 = -1;
    vqap->LoopEndFrameJump = -1;
    vqap->LoopIterationsJump = -1;
    vqap->LoopStartFrame1 = -1;
    vqap->LoopEndFrameNormal = -1;
    vqap->LoopStartFrame2 = -1;
    vqap->LastCallbackCount = -1;
    vqap->MaxCallbackDelta = 0;
    audio->Block1 = 0;
    audio->Block2 = -1;
    audio->BufferPosition = 0;
    vqap->RepeatedBuffers = 0;
    VQA_ResetCache(vqap);
}
#endif

