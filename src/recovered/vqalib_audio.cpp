// vqalib_audio.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_audio.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"
#include "shared/crt_memory.hpp"


// long __fastcall VQA_OpenAudio(VQAHandleP *vqap) -- 0x00776380
// Purpose: -13 without an audio buffer; opens the audio handler (action 2) with the
// format and the fill/done callbacks and marks the module locked. -1 on success,
// else the handler's error.

// The audio handler's open parameters (0x10 bytes on the stack).
struct VQAAudioOpenParams {
    unsigned short SampleRate;
    unsigned char Channels;
    unsigned char BitsPerSample;
    int unknown_04;
    int (__cdecl *FillCallback)(VQAHandleP *vqap);
    int (__cdecl *DoneCallback)(VQAHandleP *vqap, int buffer);
};

long __fastcall VQA_OpenAudio(VQAHandleP *vqap)
{
    VQAAudio *audio = &vqap->Audio;
    long rc;

    if (audio->Buffer == 0) {
        return -13;
    }
    audio->Block1 = 0;
    audio->Block2 = -1;
    audio->BufferPosition = 0;
    vqap->RepeatedBuffers = 0;
    VQAAudioOpenParams params;
    params.SampleRate = vqap->SampleRate;
    params.Channels = vqap->Channels;
    params.BitsPerSample = vqap->BitsPerSample;
    params.FillCallback = VQA_AudioFillCallback;
    params.DoneCallback = VQA_AudioDoneCallback;
    rc = vqap->Config.AudioHandler((VQAHandle *)vqap, VQAAUDIO_OPEN, (int)&params, sizeof(params));
    if (rc >= 0 || rc == -1) {
        if ((audio->Flags & VQAAUDF_MODLOCKED) == 0) {
            audio->Flags |= VQAAUDF_MODLOCKED;
        }
        return -1;
    }
    return rc;
}


// void __fastcall VQA_CloseAudio(VQAHandleP *vqap) -- 0x00776420
// Purpose: stops playback if playing, closes the audio handler (action 3).

void __fastcall VQA_CloseAudio(VQAHandleP *vqap)
{
    VQAAudio *audio = &vqap->Audio;

    if (audio->Flags & VQAAUDF_ISPLAYING) {
        VQA_StopAudio(vqap);
    }
    vqap->Config.AudioHandler((VQAHandle *)vqap, VQAAUDIO_CLOSE, 0, 0);
    if ((audio->Flags & VQAAUDF_MODLOCKED) == 1) {
        audio->Flags &= ~VQAAUDF_MODLOCKED;
    }
}


// void __fastcall VQA_StartAudio(VQAHandleP *vqap) -- 0x00776480
// Purpose: starts the audio handler (action 4) and marks audio playing.

void __fastcall VQA_StartAudio(VQAHandleP *vqap)
{
    VQAAudio *audio = &vqap->Audio;

    vqap->Config.AudioHandler((VQAHandle *)vqap, VQAAUDIO_START, 0, 0);
    audio->Flags |= VQAAUDF_ISPLAYING;
}


// void __fastcall VQA_PauseAudio(VQAHandleP *vqap) -- 0x007764A0
// Purpose: pauses the audio handler (action 6), audio no longer playing.

void __fastcall VQA_PauseAudio(VQAHandleP *vqap)
{
    vqap->Config.AudioHandler((VQAHandle *)vqap, VQAAUDIO_PAUSE, 0, 0);
    vqap->Audio.Flags &= ~VQAAUDF_ISPLAYING;
}


// void __fastcall VQA_StopAudio(VQAHandleP *vqap) -- 0x007764C0
// Purpose: stops the audio handler (action 7), audio no longer playing.

void __fastcall VQA_StopAudio(VQAHandleP *vqap)
{
    vqap->Config.AudioHandler((VQAHandle *)vqap, VQAAUDIO_STOP, 0, 0);
    vqap->Audio.Flags &= ~VQAAUDF_ISPLAYING;
}


// long __fastcall CopyAudio(VQAHandleP *vqap) -- 0x007764E0
// Purpose: moves the decoded audio waiting in TempBuf into the audio ring buffer
// (wrapping at its end) and marks the blocks it fills loaded. -11 (and a loader
// wait) when the last block is still loaded; -1 otherwise.

long __fastcall CopyAudio(VQAHandleP *vqap)
{
    VQAAudio *audio = &vqap->Audio;
    VQAConfig *config = &vqap->Config;
    VQALoader *loader = &vqap->Loader;
    unsigned long startblock;
    unsigned long endblock;
    unsigned long len1;
    unsigned long len2;
    unsigned long i;
    unsigned char *tempbuf;
    unsigned long tempbuflen;

    if ((config->OptionFlags & 1) == 0 || audio->Buffer == 0 || audio->TempBufLen == 0) {
        return -1;
    }
    tempbuf = audio->TempBuf + audio->BufferOffset;
    tempbuflen = audio->TempBufLen - audio->BufferOffset;
    startblock = audio->AudBufPos / config->HMIBufSize;
    endblock = (audio->AudBufPos + tempbuflen) / config->HMIBufSize;
    if (endblock >= audio->NumAudBlocks) {
        endblock -= audio->NumAudBlocks;
    }
    if (audio->IsLoaded[endblock] == 1) {
        loader->WaitsOnAudio++;
        return -11;
    }
    if (startblock <= endblock) {
        memcpy(audio->Buffer + audio->AudBufPos, tempbuf, tempbuflen);
        audio->AudBufPos += tempbuflen;
        audio->TempBufLen = 0;
        audio->BufferOffset = 0;
        for (i = startblock; i < endblock; i++) {
            audio->IsLoaded[i] = 1;
        }
    } else {
        len1 = config->AudioBufSize - audio->AudBufPos;
        len2 = tempbuflen - len1;
        memcpy(audio->Buffer + audio->AudBufPos, tempbuf, len1);
        memcpy(audio->Buffer, tempbuf + len1, len2);
        audio->AudBufPos = len2;
        audio->TempBufLen = 0;
        audio->BufferOffset = 0;
        for (i = startblock; i < audio->NumAudBlocks; i++) {
            audio->IsLoaded[i] = 1;
        }
        for (i = 0; i < endblock; i++) {
            audio->IsLoaded[i] = 1;
        }
    }
    return -1;
}


// int __cdecl VQA_AudioFillCallback(VQAHandleP *vqap) -- 0x00776680
// Purpose: the audio handler's request for the next HMIBufSize block: hands the
// next loaded block to the handler (action 5), or repeats the previous block
// (HMIBuffer) while the loader is behind, or the tail at end of file; returns its size.

int __cdecl VQA_AudioFillCallback(VQAHandleP *vqap)
{
    VQAAudio *audio = &vqap->Audio;
    VQAConfig *config = &vqap->Config;
    long size = config->HMIBufSize;

    if (audio->Flags & 0x200) {
        return 0;
    }
    long pos = audio->BufferPosition;
    unsigned long block = audio->Block1;
    if (config->OptionFlags & 0x200) {
        long nblock = block + 1;
        if ((unsigned)nblock >= audio->NumAudBlocks) {
            nblock = 0;
        }
        if (!(audio->Flags & 0x100)) {
            if (audio->IsLoaded[nblock] == 0) {
                audio->Flags |= 0x400;
            }
        }
    }
    bool repeating = false;
    if (audio->IsLoaded[block] == 1) {
        if (audio->Block2 == -1) {
            audio->Block2 = block;
            audio->PlayPosition = pos;
        }
        audio->BlockRepeats[block] = 0;
        block++;
        long npos = pos + size;
        if (npos >= config->AudioBufSize) {
            npos = 0;
            block = 0;
        }
        audio->BufferPosition = npos;
        audio->Block1 = block;
    } else {
        if (audio->Flags & 0x100) {
            audio->Flags |= 0x200;
            if ((unsigned)pos < audio->AudBufPos) {
                size = audio->AudBufPos - pos;
            } else {
                size = 0;
            }
        } else {
            audio->Flags |= 0x80;
            repeating = true;
            vqap->RepeatedBuffers++;
            pos -= size;
            block--;
            if (pos < 0) {
                pos = config->AudioBufSize - size;
                block = audio->NumAudBlocks - 1;
            }
            audio->BlockRepeats[block]++;
        }
    }
    if (size > 0) {
        if (repeating == true) {
            config->AudioHandler((VQAHandle *)vqap, 5, (int)audio->HMIBuffer, size);
        } else {
            config->AudioHandler((VQAHandle *)vqap, 5, (int)(audio->Buffer + pos), size);
        }
    }
    return size;
}


// int __cdecl VQA_AudioDoneCallback(VQAHandleP *vqap, int buffer) -- 0x007767D0
// Purpose: the audio handler finished a buffer: frees its block (unless it was
// repeated) and advances PlayPosition/Block2. -20 without a playing block, -19
// for a buffer out of sync.

int __cdecl VQA_AudioDoneCallback(VQAHandleP *vqap, int buffer)
{
    VQAAudio *audio = &vqap->Audio;
    VQAConfig *config = &vqap->Config;
    unsigned long block;

    if ((void *)buffer == audio->Buffer + audio->PlayPosition || (void *)buffer == audio->HMIBuffer) {
        block = audio->Block2;
        if (block != -1) {
            if (!audio->BlockRepeats[block]) {
                audio->IsLoaded[block] = 0;
                block++;
                if (block >= audio->NumAudBlocks) {
                    block = 0;
                }
                if (audio->IsLoaded[block] == 1) {
                    audio->PlayPosition += config->HMIBufSize;
                    if (audio->PlayPosition >= (unsigned)config->AudioBufSize) {
                        audio->PlayPosition = 0;
                    }
                    audio->Block2 = block;
                } else {
                    audio->Block2 = -1;
                }
            } else {
                audio->BlockRepeats[block]--;
            }
            return -1;
        }
        return -20;
    }
    return -19;
}
