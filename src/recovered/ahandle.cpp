// ahandle.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ahandle.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ahandle.hpp"


// unsigned long __cdecl Timer_Callback_Audio_Handler(VQAHandle *vqa) (0x00411420): the movie clock from the audio play
// position (the static Get_Playback_Position expands inline); the paused clock while paused.

static unsigned long Get_Playback_Position(VQAHandle *vqa, Ahandle *audio, VQAConfig *config)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;

    unsigned long s = audio->SecondaryBufferSize;

    unsigned long dma_diff;
    unsigned long totalbytes;
    DWORD play_cursor;
    DWORD write_cursor;

    EnterCriticalSection(&audio->CriticalSection);

    long r = vqap->RepeatedBuffers;
    long l = audio->LastChunkPosition;
    long m = audio->ChunksMovedToAudioBuffer;

    totalbytes = config->HMIBufSize * m;

    if (audio->SecondaryBufferPtr &&
        audio->SecondaryBufferPtr->GetCurrentPosition(&play_cursor, &write_cursor) == 0) {
        if (l) {
            totalbytes += play_cursor;
        } else {
            if (play_cursor >= (s / 2)) {
                totalbytes += play_cursor - (s / 2);
            } else {
                if (totalbytes > 0) {
                    totalbytes += play_cursor + (s / 2);
                }
            }
        }
    }

    LeaveCriticalSection(&audio->CriticalSection);

    dma_diff = totalbytes - config->HMIBufSize * r;
    if (dma_diff > totalbytes) {
        dma_diff = 0;
    }
    return dma_diff;
}

unsigned long __cdecl Timer_Callback_Audio_Handler(VQAHandle *vqa)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    VQAConfig *config = &vqap->Config;
    Ahandle *handle = &_handles[vqap->AudioHandleIndex];
    unsigned long d;
    unsigned long t;

    if (handle->Flags & AHANDLEF_IS_PAUSED) {
        Debug_Printf("Ahandle: Paused %ld\n", handle->TickCount);
        return handle->TickCount;
    }

    t = Simple_Timer_Callback_Audio_Handler(0) - handle->PauseAdjust;

    d = Get_Playback_Position(vqa, handle, config);

    if (d > 0 && d <= handle->LastPlaybackPosition) {
        if (t > handle->LastTimerTick) {
            d = (t - handle->LastTimerTick);
            handle->LastTimerTick = t;
            handle->TickCount += d;
        }
    } else {
        handle->LastPlaybackPosition = d;
        handle->LastTimerTick = t;
        handle->TickCount = (60 * (d / (vqap->Channels * (vqap->BitsPerSample >> 3))) / vqap->SampleRate);

        if (handle->TickCount >= (unsigned long)config->LatencyAdjustment) {
            handle->TickCount -= config->LatencyAdjustment;
        } else {
            handle->TickCount = 0;
        }
    }
    return handle->TickCount;
}


// long __cdecl Stream_Audio_Handler(VQAHandle *, long action, void *buffer, long nbytes) (0x004115A0): the player's
// VQAAUDIO_* dispatch (Close, Pause, Play and Stop expand inline).

long __cdecl Stream_Audio_Handler(VQAHandle *vqa, long action, void *buffer, long nbytes)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    VQAConfig *config;

    long error = AHANDLE_OK;

    switch (action) {
        case VQAAUDIO_INIT:
            config = &vqap->Config;
            config->TimerCallback = Simple_Timer_Callback_Audio_Handler;
            config->RefreshRate = 60;
            break;

        case VQAAUDIO_OPEN:
            error = Open_Audio_Handler((VQAHandleP *)vqa, (AhandleInitParams *)buffer, nbytes);
            break;

        case VQAAUDIO_CLOSE:
            error = Close_Audio_Handler((VQAHandleP *)vqa);
            break;

        case VQAAUDIO_START:
            error = Start_Audio_Handler((VQAHandleP *)vqa);
            break;

        case VQAAUDIO_LOAD:
            error = Load_Audio_Handler((VQAHandleP *)vqa, buffer, nbytes);
            break;

        case VQAAUDIO_PAUSE:
            error = Pause_Audio_Handler((VQAHandleP *)vqa);
            break;

        case VQAAUDIO_PLAY:
            error = Play_Audio_Handler((VQAHandleP *)vqa);
            break;

        case VQAAUDIO_STOP:
            error = Stop_Audio_Handler((VQAHandleP *)vqa);
            break;
    }

    return error;
}


// long __cdecl Open_Audio_Handler(VQAHandleP *, AhandleInitParams *, long) (0x004118A0): claims the handle, reads the
// format and starts the 16 ms audio timer (RA2 drops TibSun's primary-buffer format change).

long __cdecl Open_Audio_Handler(VQAHandleP *vqap, AhandleInitParams *params, long b)
{
    IDirectSound *sound = (IDirectSound *)unknown_directsound_40a6c0();
    DSCAPS dscaps;

    Debug_Printf("Opening VQ audio handler\n");
    Debug_Printf("Current thread ID is %08x\n", GetCurrentThreadId());

    if (!unknown_check_406f30()) {
        return AHANDLE_ERROR;
    }

    int index = 0;
    while (index < Ahandle::MAX_HANDLES) {
        if (!_handles[index].Used) {
            break;
        }
        index++;
    }

    if (index != 1 && b == 16) {

        VQAConfig *config = &vqap->Config;

        vqap->AudioHandleIndex = index;

        Ahandle *handle = &_handles[index];
        memset(handle, 0, sizeof(*handle));
        handle->Used = true;
        handle->Volume = config->Volume;

        InitializeCriticalSection(&handle->CriticalSection);

        if (config->AudioRate != -1) {
            handle->SampleRate = config->AudioRate;
        } else {
            if (config->FrameRate != vqap->FrameRate) {
                handle->SampleRate = params->SampleRate * (unsigned)config->FrameRate / vqap->FrameRate;
            } else {
                handle->SampleRate = params->SampleRate;
            }
        }

        memset(&dscaps, 0, sizeof(dscaps));
        dscaps.dwSize = sizeof(DSCAPS);

        sound->GetCaps(&dscaps);

        if (dscaps.dwFlags & 0x20) {
            Debug_Printf("Ahandle detected emulated sound driver. LatencyAdjustment = %ld ticks\n", config->LatencyAdjustment);
        } else {
            config->LatencyAdjustment = 0;
        }

        handle->Channels = params->Channels;
        handle->BitsPerSample = params->BitsPerSample;
        handle->InitFlags = params->Flags;
        _AHandleCallbackFunc1 = (AHANDLE_CALLBACK_1)params->Callback1;
        _AHandleCallbackFunc2 = (AHANDLE_CALLBACK_2)params->Callback2;

        Debug_Printf("Calling timeBeginPeriod\n");
        if (timeBeginPeriod(1000 / 60) != 97) {
            Debug_Printf("Creating VQ audio timer thread\n");
            handle->TimerHandle = timeSetEvent(1000 / 60, 1, AudioCallback, (DWORD)vqap, 1);
            Debug_Printf("VQ audio handler opened OK\n");
            if (handle->TimerHandle != 0) {
                return AHANDLE_OK;
            }
            return AHANDLE_ERROR;
        }
    }
    return AHANDLE_ERROR;
}


// long __cdecl Close_Audio_Handler(VQAHandleP *) (0x00411A60): stops the buffer and the timer, frees the handle.

long __cdecl Close_Audio_Handler(VQAHandleP *vqap)
{
    Debug_Printf("Closing VQ audio handler\n");

    Ahandle *handle = &_handles[vqap->AudioHandleIndex];

    if (handle->Used == 1) {

        if (handle->SecondaryBufferPtr != 0) {
            Debug_Printf("Stop_Audio_Handler\n");
            Stop_Audio_Handler(vqap);
        }

        Debug_Printf("Calling timeKillEvent\n");
        timeKillEvent(handle->TimerHandle);
        handle->TimerHandle = 0;

        Debug_Printf("Calling timeEndPeriod\n");
        timeEndPeriod(1000 / 60);

        handle->Used = false;

        Debug_Printf("Deleting the critical section object\n");
        DeleteCriticalSection(&handle->CriticalSection);
    }

    Debug_Printf("VQ audio handler closed OK\n");
    return AHANDLE_OK;
}


// long __cdecl Start_Audio_Handler(VQAHandleP *) (0x00411B70): makes the looping secondary buffer (two HMI blocks),
// primes it and plays it at the movie volume (0x00A40B48); resumes a paused handle instead.

long __cdecl Start_Audio_Handler(VQAHandleP *vqap)
{
    VQAConfig *config = &vqap->Config;
    Ahandle *audio = &_handles[vqap->AudioHandleIndex];

    if (audio->Flags & AHANDLEF_IS_PAUSED) {
        return Play_Audio_Handler(vqap);
    }

    EnterCriticalSection(&audio->CriticalSection);

    if (audio->SecondaryBufferPtr != 0) {
        audio->SecondaryBufferPtr->Stop();
        audio->SecondaryBufferPtr->Release();
        audio->SecondaryBufferPtr = 0;
    }

    audio->SecondaryBufferSize = config->HMIBufSize * 2;

    memset(&audio->BufferDesc, 0, sizeof(DSBUFFERDESC));
    audio->BufferDesc.dwSize = sizeof(DSBUFFERDESC);
    audio->BufferDesc.dwFlags = 0x000100E0;
    audio->BufferDesc.dwBufferBytes = audio->SecondaryBufferSize;
    audio->BufferDesc.lpwfxFormat = &audio->DsBuffFormat;
    memset(&audio->DsBuffFormat, 0, sizeof(WAVEFORMATEX));
    audio->DsBuffFormat.wFormatTag = WAVE_FORMAT_PCM;
    audio->DsBuffFormat.nSamplesPerSec = audio->SampleRate;
    audio->DsBuffFormat.nChannels = audio->Channels;
    audio->DsBuffFormat.wBitsPerSample = audio->BitsPerSample;
    audio->DsBuffFormat.nBlockAlign = (short)((audio->DsBuffFormat.wBitsPerSample / 8) * audio->DsBuffFormat.nChannels);
    audio->DsBuffFormat.nAvgBytesPerSec = audio->DsBuffFormat.nSamplesPerSec * audio->DsBuffFormat.nBlockAlign;

    ((IDirectSound *)unknown_directsound_40a6c0())->CreateSoundBuffer(&audio->BufferDesc, &audio->SecondaryBufferPtr, 0);

    if (audio->SecondaryBufferPtr == 0) {
        LeaveCriticalSection(&audio->CriticalSection);
        return AHANDLE_ERROR;
    }

    _AHandleCallbackFunc1((VQAHandle *)vqap);
    _AHandleCallbackFunc1((VQAHandle *)vqap);
    audio->AudioBufWriteIndex = 0;
    audio->EndLastAudioChunk = 0;
    audio->ChunksMovedToAudioBuffer = 0;
    audio->SecondaryBufferPtr->SetCurrentPosition(0);

    audio->Volume = (int)(unknown_volume_a40b48 * 255.0f);
    audio->SecondaryBufferPtr->SetVolume(Audio_DS_Volume_Attenuation(audio->Volume & 255));

    HRESULT return_code = audio->SecondaryBufferPtr->Play(0, 0, 1);
    LeaveCriticalSection(&audio->CriticalSection);
    return return_code == 0 ? AHANDLE_OK : AHANDLE_ERROR;
}


// long __cdecl Load_Audio_Handler(VQAHandleP *, void *buffer, long nbytes) (0x00411D80): follows a movie volume
// change, queues the block and copies it into the DirectSound buffer (Move_HMI_Audio_Block_To_Direct_Sound_Buffer
// expands inline).

static BOOL Move_HMI_Audio_Block_To_Direct_Sound_Buffer(VQAHandleP *vqap)
{
    Ahandle *audio;

    void *play_buffer_ptr1;
    void *play_buffer_ptr2;
    DWORD lock_length1;
    DWORD lock_length2;
    unsigned next_fill_pos;
    HRESULT return_code;

    audio = &_handles[vqap->AudioHandleIndex];
    VQAConfig *config = &vqap->Config;

    if (audio->SecondaryBufferPtr == 0) {
        return 0;
    }

    next_fill_pos = audio->EndLastAudioChunk;

    return_code = audio->SecondaryBufferPtr->Lock((DWORD)next_fill_pos, (DWORD)config->HMIBufSize,
        &play_buffer_ptr1, &lock_length1, &play_buffer_ptr2, &lock_length2, 0);

    if (return_code != 0) return FALSE;

    int index = audio->AudioBufReadIndex;

    if (audio->AudioBufSize[index] > lock_length1) {

        memcpy((char *)play_buffer_ptr1, audio->AudioBuf[index], lock_length1);

        if (audio->AudioBufSize[index] - lock_length1 > lock_length2) {
            memcpy(play_buffer_ptr2, audio->AudioBuf[index], lock_length2);
        } else {
            memcpy(play_buffer_ptr2, audio->AudioBuf[index], audio->AudioBufSize[index] - lock_length1);
        }

    } else {
        memcpy(play_buffer_ptr1, audio->AudioBuf[index], audio->AudioBufSize[index]);
    }

    audio->SecondaryBufferPtr->Unlock(play_buffer_ptr1, lock_length1, play_buffer_ptr2, lock_length2);

    audio->LastChunkPosition = next_fill_pos;
    audio->EndLastAudioChunk = next_fill_pos + audio->AudioBufSize[index];
    if (audio->EndLastAudioChunk >= audio->SecondaryBufferSize) {
        audio->EndLastAudioChunk = 0;
    }

    return TRUE;
}

long __cdecl Load_Audio_Handler(VQAHandleP *vqap, void *buffer, long nbytes)
{
    Ahandle *handle = &_handles[vqap->AudioHandleIndex];

    if (buffer != 0 && nbytes != 0) {
        EnterCriticalSection(&handle->CriticalSection);
        if (handle != 0 && handle->SecondaryBufferPtr != 0) {
            if (handle->Volume != unknown_volume_a40b48 * 256.0f) {
                handle->Volume = (int)(unknown_volume_a40b48 * 255.0f);
                handle->SecondaryBufferPtr->SetVolume(Audio_DS_Volume_Attenuation(handle->Volume & 255));
            }
        }
        int readindex = handle->AudioBufReadIndex;
        if (handle->AudioBufInUse[readindex] == TRUE) {
            LeaveCriticalSection(&handle->CriticalSection);
            return AHANDLE_ERROR;
        }
        handle->AudioBuf[readindex] = buffer;
        handle->AudioBufSize[readindex] = nbytes;
        handle->AudioBufInUse[readindex] = TRUE;
        Move_HMI_Audio_Block_To_Direct_Sound_Buffer(vqap);
        int index = readindex + 1;
        if (index >= Ahandle::MAX_BUFFERS) {
            index = 0;
        }
        handle->AudioBufReadIndex = index;
        LeaveCriticalSection(&handle->CriticalSection);
        return AHANDLE_OK;
    }
    return AHANDLE_ERROR;
}


// long __cdecl Pause_Audio_Handler(VQAHandleP *) (0x00411FB0)

long __cdecl Pause_Audio_Handler(VQAHandleP *vqap)
{
    Ahandle *handle = &_handles[vqap->AudioHandleIndex];

    EnterCriticalSection(&handle->CriticalSection);

    if (handle->Used == true && handle->SecondaryBufferPtr != 0) {
        handle->SecondaryBufferPtr->Stop();
    }
    handle->Flags |= AHANDLEF_IS_PAUSED;

    LeaveCriticalSection(&handle->CriticalSection);

    return AHANDLE_OK;
}


// long __cdecl Play_Audio_Handler(VQAHandleP *) (0x00412000): resumes a paused buffer; the clock skips the pause.

long __cdecl Play_Audio_Handler(VQAHandleP *vqap)
{
    Ahandle *handle = &_handles[vqap->AudioHandleIndex];

    long rc;
    if (!handle->Used || handle->SecondaryBufferPtr == 0) {
        return AHANDLE_ERROR;
    }

    EnterCriticalSection(&handle->CriticalSection);

    rc = handle->SecondaryBufferPtr->Play(0, 0, 1);
    if (rc == 0) {
        handle->PauseAdjust = Simple_Timer_Callback_Audio_Handler(0) - handle->LastTimerTick;
        Debug_Printf("Ahandle: PauseAdjust %ld\n", handle->PauseAdjust);
        handle->Flags &= ~AHANDLEF_IS_PAUSED;
        rc = AHANDLE_OK;
    } else {
        rc = AHANDLE_ERROR;
    }
    LeaveCriticalSection(&handle->CriticalSection);

    return rc;
}


// long __cdecl Stop_Audio_Handler(VQAHandleP *) (0x004120A0): releases the secondary buffer and the queued blocks.

long __cdecl Stop_Audio_Handler(VQAHandleP *vqap)
{
    Ahandle *handle = &_handles[vqap->AudioHandleIndex];

    if (handle->SecondaryBufferPtr != 0) {

        EnterCriticalSection(&handle->CriticalSection);
        handle->SecondaryBufferPtr->Stop();
        handle->SecondaryBufferPtr->Release();
        handle->SecondaryBufferPtr = 0;
        handle->AudioBufReadIndex = 0;
        handle->AudioBufWriteIndex = 0;
        for (int i = 0; i < Ahandle::MAX_BUFFERS; i++) {
            handle->AudioBufInUse[i] = false;
        }

        LeaveCriticalSection(&handle->CriticalSection);
    }
    return AHANDLE_OK;
}


// void __stdcall AudioCallback(UINT timer, UINT, DWORD vqap, DWORD, DWORD) (0x00412120): the timer callback that
// refills the buffer half the play cursor left (and restores a lost buffer).

void __stdcall AudioCallback(unsigned int uTimerID, unsigned int, DWORD dwUser, DWORD, DWORD)
{
    Ahandle *audio;
    DWORD play_cursor;
    DWORD write_cursor;
    HRESULT return_code;
    bool buffer_stopped = false;
    DWORD status;

    VQAHandle *vqa = (VQAHandle *)dwUser;
    VQAHandleP *vqap = (VQAHandleP *)dwUser;

    audio = &_handles[vqap->AudioHandleIndex];

    if (InterlockedIncrement(&audio->SuspendAudioCallback) != TRUE || (audio->Flags & AHANDLEF_IS_PAUSED)) {
        InterlockedDecrement(&audio->SuspendAudioCallback);
        return;
    }

    EnterCriticalSection(&audio->CriticalSection);

    if (!audio->SecondaryBufferPtr || audio->TimerHandle != uTimerID) {
        LeaveCriticalSection(&audio->CriticalSection);
        InterlockedDecrement(&audio->SuspendAudioCallback);
        return;
    }

    return_code = audio->SecondaryBufferPtr->GetStatus(&status);
    if (!(status & 1) && !(status & 4)) {
        LeaveCriticalSection(&audio->CriticalSection);
        InterlockedDecrement(&audio->SuspendAudioCallback);
        return;
    }

    return_code = audio->SecondaryBufferPtr->GetCurrentPosition(&play_cursor, &write_cursor);

    bool write_more = false;

    if (return_code == (HRESULT)0x88780096) {
        audio->SecondaryBufferPtr->Restore();
        audio->SecondaryBufferPtr->Stop();
        buffer_stopped = true;
        audio->SecondaryBufferPtr->SetCurrentPosition(0);
        audio->LastChunkPosition = 0;
        audio->EndLastAudioChunk = 0;
        write_more = true;
    }

    if (play_cursor < audio->EndLastAudioChunk) {
        write_more = true;
    } else {
        if ((play_cursor >= audio->SecondaryBufferSize / 2) && audio->EndLastAudioChunk == 0) {
            write_more = true;
        }
    }

    if (write_more == true) {
        _AHandleCallbackFunc2((VQAHandle *)vqa, audio->AudioBuf[audio->AudioBufWriteIndex]);
        audio->ChunksMovedToAudioBuffer++;
        int index = audio->AudioBufWriteIndex + 1;
        if (index >= Ahandle::MAX_BUFFERS) {
            index = 0;
        }
        audio->AudioBufInUse[audio->AudioBufWriteIndex] = FALSE;
        _AHandleCallbackFunc1((VQAHandle *)vqa);

        if (audio->AudioBufInUse[index]) {
            audio->AudioBufWriteIndex = index;
        }

        if (buffer_stopped == true) {
            audio->SecondaryBufferPtr->Play(0, 0, 1);
        }
    }
    LeaveCriticalSection(&audio->CriticalSection);
    InterlockedDecrement(&audio->SuspendAudioCallback);
}


// unsigned long __cdecl Simple_Timer_Callback_Audio_Handler(VQAHandle *) (0x004122C0): the game clock (a tail jump).

unsigned long __cdecl Simple_Timer_Callback_Audio_Handler(VQAHandle *vqa)
{
    return unknown_ticks_004d0090();
}


// void __fastcall Suspend_Movie_Audio_004122d0() (0x004122D0)
// Stops the movie's secondary buffer and marks the handle paused.

void __fastcall Suspend_Movie_Audio_004122d0()
{
    Ahandle *handle = &_handles[0];

    if (handle->Used == true && handle->SecondaryBufferPtr != 0) {
        EnterCriticalSection(&handle->CriticalSection);
        handle->SecondaryBufferPtr->Stop();
        handle->Flags |= AHANDLEF_IS_PAUSED;
        LeaveCriticalSection(&handle->CriticalSection);
    }
}


// void __fastcall Resume_Movie_Audio_00412320() (0x00412320)
// Restarts the movie's looping secondary buffer and clears the paused flag.

void __fastcall Resume_Movie_Audio_00412320()
{
    Ahandle *handle = &_handles[0];

    if (handle->Used == true && handle->SecondaryBufferPtr != 0) {
        EnterCriticalSection(&handle->CriticalSection);
        handle->SecondaryBufferPtr->Play(0, 0, DSBPLAY_LOOPING);
        handle->Flags &= ~AHANDLEF_IS_PAUSED;
        LeaveCriticalSection(&handle->CriticalSection);
    }
}


// int Lock_Audio_Handler() (0x00412370)

int Lock_Audio_Handler()
{
    return 1;
}


// int Unlock_Audio_Handler() (0x00412380)

int Unlock_Audio_Handler()
{
    return 1;
}
