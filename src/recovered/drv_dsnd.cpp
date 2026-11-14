// drv_dsnd.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/drv_dsnd.json carries the addresses and the rows that
// stay in their own files.


#define RA2_AUDIO_DS_START
#include "shared/audio.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"


// HRESULT __fastcall Audio_DS_Init(AudioDriver *driver) -- 0x00409390
// Purpose: DirectSound driver start-up: initializes the driver lock,
// creates the DirectSound object with priority co-operation on the main
// window, marks the driver ready, and starts the mixing thread (4 KB
// stack, time-critical priority) unless it is running, which only counts
// another user. Returns 0, or 0x80040000 when DirectSound or the thread
// cannot be created. A failed priority change is reported.

extern HWND MainWindow;                                             // 0x00B263D8

HRESULT __fastcall Audio_DS_Init(AudioDriver *driver)
{
    char message[0x400];
    InitializeCriticalSection(&Audio_Lock);
    if (DirectSoundCreate(0, &Audio_DirectSound, 0) != DS_OK) {
        return 0x80040000;
    }
    Audio_DirectSound->SetCooperativeLevel(MainWindow, DSSCL_PRIORITY);
    driver->unknown_ready_18 = 1;
    if (Audio_Thread_Running == 0) {
        Audio_Thread_Users = 0;
        Audio_Thread.Quit = 0;
        Audio_Thread.Done = 0;
        Audio_Thread.Started = 0;
        Audio_Thread.Interval = 33;
        Audio_Thread.PassCount = 0;
        Audio_Thread.Handle = CreateThread(0, 0x1000, Audio_Thread_Proc, &Audio_Thread, 0, &Audio_Thread.ThreadId);
        if (Audio_Thread.Handle == 0) {
            return 0x80040000;
        }
        Audio_Thread_Users++;
        Audio_Thread_Running = 1;
        if (!SetThreadPriority(Audio_Thread.Handle, THREAD_PRIORITY_TIME_CRITICAL)) {
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, message, 0x400, 0);
            Debug_Printf("Unable to change the priority of the Audio Thread - %s\n", message);
        }
    } else {
        Audio_Thread_Users++;
    }
    return DS_OK;
}


// DWORD __stdcall Audio_Thread_Proc(void *parameter) -- 0x004094D0
// Purpose: the DirectSound mixing thread. Until asked to quit, each pass
// (unless the device is busy, which waits 5 ms) ticks the device's mixer
// timing, refills every playing channel's buffer by the frames played since
// the last pass (stopping channels whose buffer stopped or ran out) and
// updates the global levels and the device; then it sleeps the interval.
// The level block is Audio_Run_With_Levels_Updated 0x0040A8C0 written out
// (the original keeps its device test; VC6 does not inline that function).

DWORD __stdcall Audio_Thread_Proc(void *parameter)
{
    AudioThreadBlock *block = (AudioThreadBlock *)parameter;
    block->Started = 1;
    block->Done = 0;
    while (block->Quit == 0) {
        if (block->Device != 0) {
            if (block->Device->unknown_busy_104 == 0) {
                AudioDevice *device = block->Device;
                EnterCriticalSection(&Audio_Lock);
                __int64 now = Audio_Now();
                device->Span.Tick(now);
                AudioListIterator cursor;
                for (AudioChannel *channel = (AudioChannel *)Audio_Device_First_Channel(device, &cursor); channel != 0;
                     channel = (AudioChannel *)cursor.Next()) {
                    if (channel->Flags & AUDIO_CHANNEL_PLAYING) {
                        if (Audio_DS_Channel_Is_Playing(channel)) {
                            channel->DriverBuffer->unknown_18 = now - channel->DriverBuffer->unknown_10;
                            channel->DriverBuffer->unknown_10 = now;
                            AudioChannelBuffer *record = channel->DriverBuffer;
                            DWORD play;
                            record->Buffer->GetCurrentPosition(&play, &record->WriteCursor);
                            unsigned int position = play / record->FrameBytes;
                            int frames = position - record->unknown_0c;
                            if (frames != 0) {
                                if (frames < 0) {
                                    frames = position - record->unknown_0c + 4;
                                }
                                if (frames > 0) {
                                    if ((unsigned int)(record->unknown_0c += frames) >= 4) {
                                        record->unknown_0c &= 3;
                                    }
                                    record->unknown_d4 += frames;
                                    frames *= record->FrameBytes;
                                    int filled = Audio_DS_Fill(record, frames);
                                    if (record->unknown_40 > frames) {
                                        record->unknown_40 = frames;
                                    }
                                    if ((record->unknown_08 -= frames) <= 0 || filled == 0) {
                                        record->Buffer->Stop();
                                    }
                                }
                            }
                            record->unknown_d0++;
                        } else {
                            if (channel->DriverBuffer->Buffer != 0) {
                                channel->DriverBuffer->Buffer->Stop();
                            }
                            if (channel->unknown_stop_callback_164 != 0) {
                                channel->unknown_stop_callback_164(channel);
                            }
                        }
                    }
                }
                if (device != 0) {
                    Audio_Global_Levels[2]->Update();
                    Audio_Global_Levels[3]->Update();
                    Audio_Global_Levels[1]->Update();
                    Audio_Global_Levels[0]->Update();
                    Audio_Global_Levels[6]->Update();
                    Audio_Global_Levels[7]->Update();
                    Audio_Global_Levels[5]->Update();
                    Audio_Global_Levels[4]->Update();
                    device->Run_Update();
                    Audio_Global_Levels[3]->Clear_Changed();
                    Audio_Global_Levels[2]->Clear_Changed();
                    Audio_Global_Levels[1]->Clear_Changed();
                    Audio_Global_Levels[0]->Clear_Changed();
                    Audio_Global_Levels[6]->Clear_Changed();
                    Audio_Global_Levels[7]->Clear_Changed();
                    Audio_Global_Levels[5]->Clear_Changed();
                    Audio_Global_Levels[4]->Clear_Changed();
                }
                LeaveCriticalSection(&Audio_Lock);
            } else {
                Sleep(5);
                goto next;
            }
        }
        Sleep((DWORD)block->Interval);
next:
        block->PassCount++;
    }
    block->Done = 1;
    return 0;
}


// int __fastcall Audio_DS_Fill(AudioChannelBuffer *record, int bytes) -- 0x004097A0
// Purpose: writes bytes of decoded sound into the channel's DirectSound
// buffer at the fill position (+0x30), wrapping at the end of the buffer.
// Each pass locks the free space (restoring a lost buffer up to three
// times), then fills it piece by piece: asks the channel for its next
// block and its end-of-source handling when the current piece (+0x28
// source pointer, +0x2C bytes left) runs out, writes silence (0 for 16-bit,
// 0x80 for 8-bit) when there is nothing left, and otherwise runs the
// decoder. Returns 1 when everything was written, 0 on a lock, restore,
// buffer-management or decoder failure (each but the last reported).


int __fastcall Audio_DS_Fill(AudioChannelBuffer *record, int bytes)
{
    void *audio1 = 0;
    void *audio2 = 0;
    DWORD bytes1 = 0;
    DWORD bytes2 = 0;
    int locked = 0;
    int result = 0;
    while (bytes > 0) {
        if (record->unknown_38 <= 0) {
            record->PlayCursor = 0;
            record->unknown_38 = record->BufferBytes;
        }
        int written = 0;
        int size = bytes;
        if ((DWORD)size > (DWORD)record->unknown_38) {
            size = record->unknown_38;
        }
        if (locked) {
            record->Buffer->Unlock(audio1, bytes1, audio2, bytes2);
            locked = 0;
        }
        DWORD offset = record->PlayCursor;
        int retries = 3;
        while (record->Buffer->Lock(offset, size, &audio1, &bytes1, &audio2, &bytes2, 0) != DS_OK) {
            if (record->Buffer->Restore() != DS_OK) {
                Debug_Printf("Failed to restore buffer\n");
                goto finish;
            }
            record->unknown_2c = 0;
            record->unknown_08 = 0;
            record->unknown_3c = record->FrameBytes * 4;
            record->Buffer->SetCurrentPosition(0);
            DWORD play;
            record->Buffer->GetCurrentPosition(&play, &record->WriteCursor);
            record->unknown_0c = play / record->FrameBytes;
            record->unknown_10 = Audio_Now();
            record->unknown_40 = record->BufferBytes;
            record->Buffer->GetCurrentPosition((DWORD *)&record->PlayCursor, &record->WriteCursor);
            record->unknown_38 = record->BufferBytes - record->PlayCursor;
            record->unknown_d0 = 0;
            record->unknown_d8 = record->unknown_0c;
            record->unknown_d4 = 0;
            record->unknown_dc = record->PlayCursor;
            Debug_Printf("Restored buffer\n");
            if (retries-- == 0) {
                Debug_Printf("Failed to lock buffer\n");
                goto finish;
            }
        }
        locked = 1;
        if (audio2 != 0 || bytes1 != (DWORD)size || bytes2 != 0) {
            Debug_Printf("Buffer management out by %d bytes\n", bytes2);
            goto unlock;
        }
        {
            char *destination = (char *)audio1;
            while (size != 0) {
                int chunk = size;
                if (record->unknown_2c == 0) {
                    record->unknown_2c = -1;
                    if (record->unknown_cc == 0) {
                        record->Channel->unknown_next_block_15c(record->Channel);
                        record->unknown_cc = 1;
                    }
                    if (record->Channel->unknown_174 == 0 && record->unknown_a8 == 0) {
                        record->Channel->unknown_end_of_source_160(record->Channel);
                        if (record->Channel->unknown_174 != 0) {
                            Audio_DS_Setup_Decoder(record);
                        }
                    }
                    if (record->Channel->unknown_174 != 0) {
                        record->unknown_cc = 0;
                        record->unknown_28 = (unsigned char *)record->Channel->unknown_170;
                        record->unknown_20 = (unsigned char *)record->Channel->unknown_170;
                        record->unknown_2c = record->unknown_24 = record->Channel->unknown_174;
                    }
                    if (record->unknown_2c == 0) {
                        record->unknown_2c = -1;
                    }
                }
                if (record->unknown_2c <= 0 && record->unknown_a8 == 0) {
                    if (record->Channel->Format.BytesPerSample == 2) {
                        memset(destination, 0, chunk);
                    } else {
                        memset(destination, 0x80, chunk);
                    }
                    record->unknown_3c -= chunk;
                } else {
                    int consumed = record->unknown_2c;
                    if (consumed <= 0) {
                        if (!record->Decode(record, destination, &chunk, 0, 0)) {
                            goto unlock;
                        }
                        consumed = 0;
                        record->unknown_2c = 0;
                    } else {
                        if (!record->Decode(record, destination, &chunk, record->unknown_28, &consumed)) {
                            goto unlock;
                        }
                    }
                    record->unknown_28 += consumed;
                    record->unknown_2c -= consumed;
                    record->unknown_08 += chunk;
                }
                destination += chunk;
                record->PlayCursor += chunk;
                record->unknown_38 -= chunk;
                size -= chunk;
                written += chunk;
            }
        }
        bytes -= written;
        if (bytes <= 0) {
            break;
        }
    }
    result = 1;
finish:
    if (!locked) {
        return result;
    }
unlock:
    record->Buffer->Unlock(audio1, bytes1, audio2, bytes2);
    return result;
}


// void __fastcall Audio_DS_Setup_Decoder(AudioChannelBuffer *record) -- 0x00409B60
// Purpose: prepares the buffer record's decoder for its channel's source
// format (the device default when the source has none): PCM copies
// (0x00409CB0); IMA ADPCM (type 1) runs the block decoder 0x00409D00 with
// 0x0040A990 and needs a block-sized input buffer and a 4 * block + 0x80
// output buffer; type 2 runs 0x00409D00 alone. Grows the two work buffers
// (+0x7C and +0x90) when they are too small; a failed allocation leaves
// the capacity 0. Does nothing without a source.

extern "C" void __cdecl free(void *block);
extern "C" void *__cdecl malloc(unsigned int size);

void __fastcall Audio_DS_Setup_Decoder(AudioChannelBuffer *record)
{
    AudioSource *source = record->Channel->Source;
    if (source == 0) {
        return;
    }
    AudioFormat *format = source->Format;
    if (format == 0) {
        format = &record->Channel->Device->DefaultFormat;
    }
    record->unknown_98 = 0;
    record->unknown_84 = 0;
    record->unknown_80 = 0;
    record->DecodeState = 3;
    record->InputNeeded = 0;
    record->OutputNeeded = 0;
    record->DecodeChannels = format->Channels;
    record->unknown_a8 = 0;
    switch (format->Type) {
    case AUDIO_FORMAT_PCM:
        record->Decode = Audio_Copy_Block;
        break;
    case AUDIO_FORMAT_IMA_ADPCM:
        record->Decode = Audio_DS_Decode_Blocks;
        record->Decode_Block = Audio_DS_Decode_Adpcm_Block;
        record->InputNeeded = format->BlockSize;
        record->BlockSize = format->BlockSize;
        record->OutputNeeded = format->BlockSize * 4 + 0x80;
        break;
    case 2:
        record->Decode = Audio_DS_Decode_Blocks;
        break;
    }
    if (record->InputCapacity < record->InputNeeded) {
        if (record->Input != 0) {
            free(record->Input);
        }
        record->Input = malloc(record->InputNeeded);
        if (record->Input == 0) {
            record->InputCapacity = 0;
        } else {
            record->InputCapacity = record->InputNeeded;
        }
    }
    if (record->OutputCapacity < record->OutputNeeded) {
        if (record->Output != 0) {
            free(record->Output);
        }
        record->Output = malloc(record->OutputNeeded);
        if (record->Output == 0) {
            record->OutputCapacity = 0;
        } else {
            record->OutputCapacity = record->OutputNeeded;
        }
    }
}


// int __fastcall Audio_Copy_Block(AudioChannelBuffer *record, void *destination, int *destination_size,
//                                 void const *source, int *source_size) -- 0x00409CB0
// Purpose: the PCM decoder: copies the smaller of the two sizes from
// source to destination and stores that count into both sizes (nothing
// when it is 0). Always returns 1. The record is not used.

int __fastcall Audio_Copy_Block(AudioChannelBuffer *record, void *destination, int *destination_size,
                                void const *source, int *source_size)
{
    int count = *destination_size;
    if (count > *source_size) {
        count = *source_size;
    }
    if (count != 0) {
        memcpy(destination, source, count);
        *destination_size = count;
        *source_size = count;
    }
    return 1;
}


// void Audio_DS_Release() -- 0x00409F10
// Purpose: DirectSound driver shutdown: releases the DirectSound object;
// when the mixing thread is running and its user count (floored at 0)
// drops to zero, asks it to quit, waits until it reports done and for its
// handle (up to 5 s), closes the handle and clears the running flag.
// Finally deletes the driver lock.

void Audio_DS_Release()
{
    if (Audio_DirectSound != 0) {
        Audio_DirectSound->Release();
        Audio_DirectSound = 0;
    }
    if (Audio_Thread_Running != 0) {
        if (--Audio_Thread_Users < 0) {
            Audio_Thread_Users = 0;
        }
        if (Audio_Thread_Users == 0) {
            Audio_Thread.Quit = 1;
            while (Audio_Thread.Done == 0) {
            }
            WaitForSingleObject(Audio_Thread.Handle, 5000);
            CloseHandle(Audio_Thread.Handle);
            Audio_Thread_Running = 0;
        }
    }
    DeleteCriticalSection(&Audio_Lock);
}


// HRESULT __fastcall Audio_DS_Open(AudioDevice *device) -- 0x00409FB0
// Purpose: opens DirectSound output for a device: resets its mixer timing
// (33 ms passes), hands it to the mixing thread, creates the primary buffer
// in the device's default format (reported when the format is refused),
// reads the DirectSound caps and starts the primary buffer looping. Returns
// 0, or 0x80040000 when the primary buffer cannot be created.

HRESULT __fastcall Audio_DS_Open(AudioDevice *device)
{
    device->unknown_f4 = 100;
    device->Span.Set(33);
    device->Span.Set_08(1056);
    device->Span.Set_20(3168);
    Audio_Thread.Device = device;
    AudioFormat *format = &device->DefaultFormat;
    WAVEFORMATEX wave;
    memset(&wave, 0, sizeof(wave));
    wave.wFormatTag = WAVE_FORMAT_PCM;
    wave.nChannels = (WORD)format->Channels;
    wave.nSamplesPerSec = format->Rate;
    wave.nBlockAlign = (WORD)(format->BytesPerSample * format->Channels);
    wave.nAvgBytesPerSec = wave.nBlockAlign * wave.nSamplesPerSec;
    wave.wBitsPerSample = (WORD)(format->BytesPerSample * 8);
    wave.cbSize = 0;
    DSBUFFERDESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    desc.dwBufferBytes = 0;
    if (Audio_DirectSound->CreateSoundBuffer(&desc, &Audio_Primary_Buffer, 0) != DS_OK) {
        Audio_Primary_Buffer = 0;
        return 0x80040000;
    }
    if (Audio_Primary_Buffer->SetFormat(&wave) != DS_OK) {
        Debug_Printf("Unable to set the desired primary format\n");
    }
    DSCAPS caps = { 0 };
    caps.dwSize = sizeof(caps);
    Audio_DirectSound->GetCaps(&caps);
    Audio_Primary_Buffer->Play(0, 0, DSBPLAY_LOOPING);
    device->unknown_1d8 = 4;
    device->unknown_1dc = 8;
    device->unknown_1e0 = 0;
    return 0;
}


// void __fastcall Audio_DS_Close() -- 0x0040A130
// Purpose: detaches the device from the mixing thread under the driver
// lock, then stops and releases the primary buffer.

void __fastcall Audio_DS_Close()
{
    EnterCriticalSection(&Audio_Lock);
    Audio_Thread.Device = 0;
    LeaveCriticalSection(&Audio_Lock);
    if (Audio_Primary_Buffer != 0) {
        Audio_Primary_Buffer->Stop();
        Audio_Primary_Buffer->Release();
        Audio_Primary_Buffer = 0;
    }
}


// Opens the DirectSound buffer record for a channel, 0040A180.

int __fastcall Audio_DS_Channel_Open(AudioChannel *channel)
{
    AudioChannelBuffer *record = new AudioChannelBuffer;
    memset(record, 0, sizeof(*record));
    channel->DriverBuffer = record;
    record->Channel = channel;
    channel->unknown_1a8 = 264;
    channel->unknown_1b0 = 1056;
    record->Decode_Block = 0;
    record->Input = 0;
    record->Output = 0;
    int &reset_state = record->unknown_a8;
    reset_state = 0;
    Audio_Channel_Set_Format(record->Channel, &record->Channel->Device->DefaultFormat);
    return 0;
}


// int __fastcall Audio_DS_Channel_Close(AudioChannel *channel) -- 0x0040A1F0
// Purpose: releases the channel's DirectSound buffer record: frees its two
// heap blocks (clearing their sizes), releases the buffer and deletes the
// record. Returns 0.

extern "C" void __cdecl free(void *block);
void __cdecl operator delete(void *block);

int __fastcall Audio_DS_Channel_Close(AudioChannel *channel)
{
    AudioChannelBuffer *record = channel->DriverBuffer;
    if (record != 0) {
        if (record->Input != 0) {
            free(record->Input);
            record->Input = 0;
            record->InputCapacity = 0;
        }
        if (record->Output != 0) {
            free(record->Output);
            record->Output = 0;
            record->OutputCapacity = 0;
        }
        if (record->Buffer != 0) {
            record->Buffer->Release();
            record->Buffer = 0;
        }
        operator delete(record);
        channel->DriverBuffer = 0;
    }
    return 0;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// HRESULT __fastcall Audio_DS_Channel_Start(AudioChannel *channel) -- 0x0040A260
// Purpose: starts a channel on the DirectSound driver: makes (or keeps) the
// playback buffer for the source's format, rewinds it, sets up the decoder,
// prefills it, applies the channel settings and starts looping playback.
// Returns 0, or 0x80040000 with a debug line when any step fails.
// from ra2-decomp 0x0040A340 (audioStart, src/wpaudio/drv_dsnd.cpp)

// AUD_set_wave_format: the PCM wave format of an audio format.
static void Audio_DS_Set_Wave_Format(WAVEFORMATEX *wave, AudioFormat *format)
{
    memset(wave, 0, sizeof(WAVEFORMATEX));
    wave->wFormatTag = WAVE_FORMAT_PCM;
    wave->nChannels = (unsigned short)format->Channels;
    wave->nSamplesPerSec = format->Rate;
    wave->nBlockAlign = (unsigned short)format->Channels * format->BytesPerSample;
    wave->nAvgBytesPerSec = wave->nSamplesPerSec * wave->nBlockAlign;
    wave->wBitsPerSample = (unsigned short)format->BytesPerSample << 3;
    wave->cbSize = 0;
}

// AUD_same_format: whether two formats agree in channels, rate and sample width.
static int Audio_DS_Same_Format(AudioFormat *f1, AudioFormat *f2)
{
    return (f1->Channels == f2->Channels && f1->Rate == f2->Rate && f1->BytesPerSample == f2->BytesPerSample);
}

// AUD_create_pcm_buffer: keeps the playback buffer when the format is
// unchanged, else makes a new one of four frames.
static int Audio_DS_Create_Buffer(AudioChannelBuffer *record, AudioFormat *format)
{
    DSBUFFERDESC desc;
    WAVEFORMATEX wave;

    Audio_Channel_Set_Format(record->Channel, format);
    if (record->Buffer != 0) {
        if (Audio_DS_Same_Format(&record->Format, format)) {
            record->Channel->unknown_1a0 = 0;
            return 0;
        }
        record->Channel->unknown_1a0 = 1;
    }
    if (record->Buffer != 0) {
        record->Buffer->Release();
        record->Buffer = 0;
    }
    Audio_DS_Set_Wave_Format(&wave, format);
    record->Format = *format;
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE;
    record->FrameBytes = wave.nAvgBytesPerSec * 3 / 100 * 8;
    record->FrameBytes = (record->FrameBytes + 1024 - 1) / 1024 * 1024;
    Debug_Printf("Sound frame size = %d bytes\n", record->FrameBytes);
    desc.dwBufferBytes = 4 * record->FrameBytes;
    desc.lpwfxFormat = &wave;
    if (Audio_DirectSound->CreateSoundBuffer(&desc, &record->Buffer, 0) != 0) {
        record->Buffer = 0;
        return 0x80040000;
    }
    record->BufferBytes = desc.dwBufferBytes;
    Debug_Printf("Sound buffer size = %d bytes\n", record->BufferBytes);
    return 0;
}

// AUD_get_current_frame: the frame the play cursor is in.
static unsigned Audio_DS_Current_Frame(AudioChannelBuffer *record)
{
    DWORD play;
    record->Buffer->GetCurrentPosition(&play, &record->WriteCursor);
    return play / record->FrameBytes;
}

// AUD_init_buffer: rewinds the buffer and resets the fill state.
static void __fastcall Audio_DS_Init_Buffer(AudioChannelBuffer *record)
{
    record->unknown_2c = 0;
    record->unknown_08 = 0;
    record->unknown_3c = record->FrameBytes * 4;
    record->Buffer->SetCurrentPosition(0);
    record->unknown_0c = Audio_DS_Current_Frame(record);
    record->unknown_10 = Audio_Now();
    record->unknown_40 = record->BufferBytes;
    record->Buffer->GetCurrentPosition((DWORD *)&record->PlayCursor, &record->WriteCursor);
    record->unknown_38 = record->BufferBytes - record->PlayCursor;
    record->unknown_d0 = 0;
    record->unknown_d8 = record->unknown_0c;
    record->unknown_d4 = 0;
    record->unknown_dc = record->PlayCursor;
}

// AUD_set_buffer_src: points the buffer at the channel's current frame.
static void __fastcall Audio_DS_Set_Buffer_Source(AudioChannelBuffer *record)
{
    record->unknown_cc = 0;
    record->unknown_28 = (unsigned char *)record->Channel->unknown_170;
    record->unknown_20 = (unsigned char *)record->Channel->unknown_170;
    record->unknown_2c = record->unknown_24 = record->Channel->unknown_174;
}

HRESULT __fastcall Audio_DS_Channel_Start(AudioChannel *channel)
{
    AudioChannelBuffer *record = channel->DriverBuffer;
    AudioFormat *format = channel->Source->Format;
    if (format == 0) {
        format = &channel->Device->DefaultFormat;
    }
    if (Audio_DS_Create_Buffer(record, format) != 0) {
        Debug_Printf("Failed to create playback buffer\n");
        return 0x80040000;
    }
    Audio_DS_Init_Buffer(record);
    Audio_DS_Setup_Decoder(record);
    Audio_DS_Set_Buffer_Source(record);
    if (Audio_DS_Fill(record, record->BufferBytes) == 0) {
        Debug_Printf("Failed to prefill buffer\n");
        return 0x80040000;
    }
    Audio_DS_Channel_Apply_Settings(channel);
    if (record->Buffer->Play(0, 0, DSBPLAY_LOOPING) != 0) {
        Debug_Printf("Failed to start playback\n");
        return 0x80040000;
    }
    return 0;
}


// int __fastcall Audio_DS_Channel_Stop(AudioChannel *channel) -- 0x0040A530
// Purpose: for a playing or paused channel, stops its buffer and runs the
// channel's stop callback (+0x164). Returns 0.

int __fastcall Audio_DS_Channel_Stop(AudioChannel *channel)
{
    if (channel->Flags & (AUDIO_CHANNEL_PLAYING | AUDIO_CHANNEL_PAUSED)) {
        if (channel->DriverBuffer->Buffer != 0) {
            channel->DriverBuffer->Buffer->Stop();
        }
        if (channel->unknown_stop_callback_164 != 0) {
            channel->unknown_stop_callback_164(channel);
        }
    }
    return 0;
}


// int __fastcall Audio_DS_Channel_Pause(AudioChannel *channel) -- 0x0040A570
// Purpose: stops the channel's buffer, if it has one. Returns 0.

int __fastcall Audio_DS_Channel_Pause(AudioChannel *channel)
{
    AudioChannelBuffer *record = channel->DriverBuffer;
    if (record->Buffer != 0) {
        record->Buffer->Stop();
    }
    return 0;
}


// HRESULT __fastcall Audio_DS_Channel_Resume(AudioChannel *channel) -- 0x0040A590
// Purpose: plays the channel's buffer looping again. Returns 0, or
// 0x80040000 without a buffer or when Play fails.

HRESULT __fastcall Audio_DS_Channel_Resume(AudioChannel *channel)
{
    AudioChannelBuffer *record = channel->DriverBuffer;
    if (record->Buffer != 0 && record->Buffer->Play(0, 0, DSBPLAY_LOOPING) == DS_OK) {
        return DS_OK;
    }
    return 0x80040000;
}


// int __fastcall Audio_DS_Channel_Is_Playing(AudioChannel *channel) -- 0x0040A5C0
// Purpose: the playing bit of the buffer status, or 0 without a buffer or
// when GetStatus fails.

int __fastcall Audio_DS_Channel_Is_Playing(AudioChannel *channel)
{
    AudioChannelBuffer *record = channel->DriverBuffer;
    DWORD status;
    if (record->Buffer != 0 && record->Buffer->GetStatus(&status) == DS_OK) {
        return status & DSBSTATUS_PLAYING;
    }
    return 0;
}


// int __fastcall Audio_DS_Channel_Apply_Settings(AudioChannel *channel) -- 0x0040A5F0
// Purpose: pushes the channel's settings to its buffer: volume (0..100%)
// through the attenuation table, pan (-100..100) through the same table
// mirrored, and the source rate scaled by the frequency level.
// Returns 0.

int __fastcall Audio_DS_Channel_Apply_Settings(AudioChannel *channel)
{
    AudioChannelBuffer *record = channel->DriverBuffer;
    record->Buffer->SetVolume(Audio_Attenuation[((channel->Settings.Volume.Current >> 16) * 100) >> 14]);
    int pan = (int)((((channel->Settings.Pan.Current >> 16) * 200) >> 14) - 100);
    if (pan < 0) {
        record->Buffer->SetPan(Audio_Attenuation[100 + pan]);
    } else {
        record->Buffer->SetPan(-Audio_Attenuation[100 - pan]);
    }
    DWORD frequency = channel->Settings.Scale_Frequency(record->Channel->Format.Rate);
    record->Buffer->SetFrequency(frequency);
    return 0;
}


class Leaf_40a690 { public: unsigned int get(); };
unsigned int Leaf_40a690::get() { return 0x80040000; }


class Leaf_40a6c0 { public: unsigned int get(); };
unsigned int Leaf_40a6c0::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int global_value; unsigned int unused_value; global_value = *(unsigned int*)(0x8315ac); return global_value; }


// 0x0040a6d0: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_0040a6d0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0040a6d0 {

extern "C" unsigned int DAT_008315b0;

unsigned int  FUN_0040a6d0(void)
{
    return DAT_008315b0;
}

}  // namespace seed_0040a6d0


// void __fastcall Audio_DS_Set_Primary_Format(int rate, int bits, int channels) -- 0x0040A6E0
// Purpose: stops the primary buffer, sets it to PCM with the given rate,
// sample width and channel count, and plays it looping again.

void __fastcall Audio_DS_Set_Primary_Format(int rate, int bits, int channels)
{
    WAVEFORMATEX format;
    if (Audio_Primary_Buffer != 0) {
        format.wFormatTag = WAVE_FORMAT_PCM;
        format.nChannels = (WORD)channels;
        format.nSamplesPerSec = rate;
        format.nBlockAlign = (WORD)(bits / 8 * channels);
        format.nAvgBytesPerSec = format.nBlockAlign * rate;
        format.wBitsPerSample = (WORD)bits;
        format.cbSize = 0;
        Audio_Primary_Buffer->Stop();
        Audio_Primary_Buffer->SetFormat(&format);
        Audio_Primary_Buffer->Play(0, 0, DSBPLAY_LOOPING);
    }
}


// void Audio_DS_Apply_Primary_Format() -- 0x0040A770
// Purpose: gives the primary DirectSound buffer the main device's default
// format (PCM) and restarts it looping.

void __fastcall Audio_DS_Apply_Primary_Format()
{
    AudioDevice *device = Audio_Get_Device();
    if (Audio_Primary_Buffer != 0 && device != 0) {
        Audio_Primary_Buffer->Stop();
        WAVEFORMATEX format;
        memset(&format, 0, sizeof(format));
        format.wFormatTag = WAVE_FORMAT_PCM;
        format.nChannels = (WORD)device->DefaultFormat.Channels;
        format.nSamplesPerSec = device->DefaultFormat.Rate;
        format.nBlockAlign = (WORD)(device->DefaultFormat.BytesPerSample * device->DefaultFormat.Channels);
        format.nAvgBytesPerSec = format.nBlockAlign * format.nSamplesPerSec;
        format.wBitsPerSample = (WORD)(device->DefaultFormat.BytesPerSample * 8);
        format.cbSize = 0;
        Audio_Primary_Buffer->SetFormat(&format);
        Audio_Primary_Buffer->Play(0, 0, DSBPLAY_LOOPING);
    }
}


// long __fastcall Audio_DS_Volume_Attenuation(int volume) -- 0x0040A820
// Purpose: maps a 0..255 volume to percent (volume * 100 / 256, clamped to
// 0..100) and returns its attenuation.

long __fastcall Audio_DS_Volume_Attenuation(int volume)
{
    int percent = volume * 100 / 256;
    if (percent > 100) {
        percent = 100;
    } else if (percent < 0) {
        percent = 0;
    }
    return Audio_Attenuation[percent];
}


// void Audio_DS_Stop_Primary() -- 0x0040A860
// Purpose: stops the primary buffer, if any.

void Audio_DS_Stop_Primary()
{
    if (Audio_Primary_Buffer != 0) {
        Audio_Primary_Buffer->Stop();
    }
}


// void Audio_DS_Restart_Primary() -- 0x0040A870
// Purpose: keeps the primary buffer running: restores it when lost and
// plays it looping when not playing.

void Audio_DS_Restart_Primary()
{
    DWORD status;
    if (Audio_Primary_Buffer != 0) {
        Audio_Primary_Buffer->GetStatus(&status);
        if (status & DSBSTATUS_BUFFERLOST) {
            Audio_Primary_Buffer->Restore();
        }
        if (!(status & DSBSTATUS_PLAYING)) {
            Audio_Primary_Buffer->Play(0, 0, DSBPLAY_LOOPING);
        }
    }
}


// void __fastcall Audio_Run_With_Levels_Updated(AudioDevice *device) -- 0x0040A8C0
// Purpose: with a device, updates the eight global level sets, runs the
// device's update (0x00403810) and clears the sets' changed flags, in the
// original's order.

void __fastcall Audio_Run_With_Levels_Updated(AudioDevice *device)
{
    if (device != 0) {
        Audio_Global_Levels[2]->Update();
        Audio_Global_Levels[3]->Update();
        Audio_Global_Levels[1]->Update();
        Audio_Global_Levels[0]->Update();
        Audio_Global_Levels[6]->Update();
        Audio_Global_Levels[7]->Update();
        Audio_Global_Levels[5]->Update();
        Audio_Global_Levels[4]->Update();
        device->Run_Update();
        Audio_Global_Levels[3]->Clear_Changed();
        Audio_Global_Levels[2]->Clear_Changed();
        Audio_Global_Levels[1]->Clear_Changed();
        Audio_Global_Levels[0]->Clear_Changed();
        Audio_Global_Levels[6]->Clear_Changed();
        Audio_Global_Levels[7]->Clear_Changed();
        Audio_Global_Levels[5]->Clear_Changed();
        Audio_Global_Levels[4]->Clear_Changed();
    }
}


// short __fastcall Audio_Adpcm_Decode(unsigned char code, AudioAdpcmState *state) -- 0x0040ABF0
// Purpose: IMA ADPCM decoding of one 4-bit code: adds the step-scaled
// difference to the predicted sample, clamps it to 16 bits, advances the
// step index (clamped to 0..88) and returns the new sample.

short __fastcall Audio_Adpcm_Decode(unsigned char code, AudioAdpcmState *state)
{
    int step = Audio_Adpcm_Steps[state->Index];
    int difference = step >> 3;
    if (code & 1) {
        difference += step >> 2;
    }
    if (code & 2) {
        difference += step >> 1;
    }
    if (code & 4) {
        difference += step;
    }
    if (code & 8) {
        difference = -difference;
    }
    state->Sample += difference;
    if (state->Sample > 32767) {
        state->Sample = 32767;
    } else if (state->Sample < -32768) {
        state->Sample = -32768;
    }
    state->Index += Audio_Adpcm_Index[code];
    if (state->Index < 0) {
        state->Index = 0;
    } else if (state->Index > 88) {
        state->Index = 88;
    }
    return (short)state->Sample;
}
