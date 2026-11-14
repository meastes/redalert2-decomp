// audplay.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audplay.json carries the addresses and the rows that
// stay in their own files.


#include "shared/audio.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_string.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/cc_file_class.hpp"


// int Audio_Streamer_Init() -- 0x00407480
// Purpose: sets up the stream list and lock, the streamer thread's block (one pass a second, the timing
// limits four times that) and starts the streamer thread (4 KB stack, above normal priority); a failed
// priority change is reported. 0 when the thread cannot be created.

int __fastcall Audio_Streamer_Init()
{
    char message[0x400];
    Audio_List_Init(&Audio_Stream_List);
    InitializeCriticalSection(&Audio_Stream_Lock);
    Audio_Streamer.Quit = 0;
    Audio_Streamer.Done = 0;
    Audio_Streamer.Started = 0;
    Audio_Streamer.Interval = 1000;
    Audio_Streamer.PassCount = 0;
    Audio_Streamer.Timing.Reset();
    Audio_Streamer.Timing.Set(Audio_Streamer.Interval);
    Audio_Streamer.Timing.Set_08(Audio_Streamer.Interval * 4);
    Audio_Streamer.Timing.Set_20(Audio_Streamer.Interval * 4);
    Audio_Streamer_Handle = CreateThread(0, 0x1000, Audio_Streamer_Proc, &Audio_Streamer, 0, &Audio_Streamer_Id);
    if (Audio_Streamer_Handle == 0) {
        return 0;
    }
    if (!SetThreadPriority(Audio_Streamer_Handle, 2)) {
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, message, 0x400, 0);
        Debug_Printf("Unable to change the priority of the Streamer Thread - %s\n", message);
    }
    Audio_Streamer_Running = 1;
    Audio_Streamer_Initialized = 1;
    return 1;
}


// DWORD __stdcall Audio_Streamer_Proc(void *parameter) -- 0x004075B0
// Purpose: the streamer thread (its parameter is the thread block
// 0x00831480). Until asked to quit, every interval it ticks its timing
// statistics and, under the stream lock, reads the next part of every
// filling stream's file into its ring (0x00408EA0, flagged by
// 0x00AB663C); a stream stops filling when its file is exhausted or the
// read reports an error or the end. Marks the block done when it ends.


DWORD __stdcall Audio_Streamer_Proc(void *parameter)
{
    AudioStreamerThread *block = (AudioStreamerThread *)parameter;
    block->Started = 1;
    block->Done = 0;
    while (block->Quit == 0) {
        block->Timing.Tick(Audio_Now());
        EnterCriticalSection(&Audio_Stream_Lock);
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            if (stream->Flags & AUDIO_STREAM_FILLING) {
                EnterCriticalSection(&Audio_Stream_Lock);
                int read;
                int bytes = stream->Remaining;
                Audio_Stream_In_Read = true;
                int result = Audio_Queue_File(stream->Fill, stream->File, bytes, &read);
                Audio_Stream_In_Read = false;
                stream->Remaining -= read;
                if (stream->Remaining <= 0 || result == 2 || result == 0) {
                    stream->Flags &= ~AUDIO_STREAM_FILLING;
                }
                LeaveCriticalSection(&Audio_Stream_Lock);
            }
        }
        LeaveCriticalSection(&Audio_Stream_Lock);
        Sleep((DWORD)block->Interval);
        block->PassCount++;
    }
    block->Done = 1;
    return 0;
}


// void Audio_Streamer_Shutdown() -- 0x004076B0
// Purpose: when the streamer is initialized: stops its thread if it runs
// (asks it to quit, waits until it reports done and for the handle up to
// 5 s, closes it), destroys every stream (stopping each that is not held)
// and deletes the stream lock.

void Audio_Streamer_Shutdown()
{
    if (Audio_Streamer_Initialized == 0) {
        return;
    }
    if (Audio_Streamer_Running != 0) {
        Audio_Streamer.Quit = 1;
        while (Audio_Streamer.Done == 0) {
            Sleep(0);
        }
        WaitForSingleObject(Audio_Streamer_Handle, 5000);
        CloseHandle(Audio_Streamer_Handle);
        Audio_Streamer_Running = 0;
    }
    AudioStreamPlayer *stream;
    while ((stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List)) != 0) {
        Audio_Stream_Stop(stream);
        Audio_Stream_Destroy(stream);
    }
    DeleteCriticalSection(&Audio_Stream_Lock);
    Audio_Streamer_Initialized = 0;
}


// AudioStreamPlayer *__fastcall Audio_Stream_Create(AudioDevice *device, __int64 buffer_time) -- 0x00407790
// Purpose: makes a stream on the device buffering buffer_time milliseconds
// (at least 7000): a zero-filled player named "Audio Stream" with its own
// channel (0x004036C0, 0x1000) whose callbacks feed it from the ring
// (0x00408710). Links it into the stream list and returns it, or returns 0
// without a device, channel or ring.

void *__cdecl operator new(unsigned int size);

AudioStreamPlayer *__fastcall Audio_Stream_Create(AudioDevice *device, __int64 buffer_time)
{
    if (device == 0) {
        return 0;
    }
    if ((unsigned __int64)buffer_time <= 0 || (unsigned __int64)buffer_time < 7000) {
        buffer_time = 7000;
    }
    AudioStreamPlayer *stream = (AudioStreamPlayer *)operator new(sizeof(AudioStreamPlayer));
    memset(stream, 0, sizeof(AudioStreamPlayer));
    Audio_List_Init_Node(stream);
    stream->Device = device;
    stream->Channel = Audio_Device_Create_Channel(device, 0x1000);
    stream->Channel->unknown_levels_94 = 0;
    stream->Channel->unknown_levels_90 = 0;
    stream->Channel->unknown_levels_98 = 0;
    stream->Channel->unknown_levels_9c = 0;
    stream->BufferTime = buffer_time;
    Audio_Source_Reset(&stream->Source);
    Audio_Format_Init(&stream->Format);
    stream->PauseCount = 0;
    stream->HoldCount = 0;
    strcpy(stream->Name, "Audio Stream");
    AudioStream *ring;
    if (stream->Channel == 0) {
        goto fail;
    }
    stream->Channel->Owner = stream;
    stream->Channel->unknown_next_b0 = Audio_Stream_Next_Block;
    stream->Channel->unknown_done_b8 = Audio_Stream_Channel_Done;
    stream->Channel->unknown_end_callback_bc = Audio_Stream_Channel_End;
    ring = Audio_Ring_Create();
    stream->Ring = ring;
    if (ring == 0) {
fail:
        Audio_Stream_Destroy(stream);
        return 0;
    }
    EnterCriticalSection(&Audio_Stream_Lock);
    Audio_List_Add_Head(&Audio_Stream_List, stream);
    LeaveCriticalSection(&Audio_Stream_Lock);
    return stream;
}


// int __fastcall Audio_Stream_Next_Block(AudioChannel *channel) -- 0x004078F0
// Purpose: the stream channel's next-block callback (+0xB0): advances the
// stream's play queue by the last block, refreshes it, and gives the
// channel the next block (at most the stream's block size, +0xA0) at the
// queue's position. Returns 0.

int __fastcall Audio_Stream_Next_Block(AudioChannel *channel)
{
    AudioStreamPlayer *stream = (AudioStreamPlayer *)channel->Owner;
    Audio_Queue_Update(stream->Drain, stream->unknown_90);
    Audio_Queue_Ready(stream->Drain);
    if (stream->unknown_a0 > stream->Drain->Available) {
        stream->unknown_90 = stream->Drain->Available;
    } else {
        stream->unknown_90 = stream->unknown_a0;
    }
    channel->unknown_178 = channel->unknown_174 = stream->unknown_90;
    channel->unknown_170 = stream->Drain->Pointer;
    return 0;
}


// int __fastcall Audio_Stream_Channel_Done(AudioChannel *channel) -- 0x00407960
// Purpose: the stream channel's done callback (+0xB8): when the stream was
// playing or paused notes the time, clears its playing, paused and
// filling flags and its pause count. Returns 0.

int __fastcall Audio_Stream_Channel_Done(AudioChannel *channel)
{
    AudioStreamPlayer *stream = (AudioStreamPlayer *)channel->Owner;
    if (stream->Flags & (AUDIO_STREAM_PLAYING | AUDIO_STREAM_PAUSED)) {
        stream->unknown_20 = Audio_Now();
    }
    stream->Flags &= ~(AUDIO_STREAM_PLAYING | AUDIO_STREAM_PAUSED | AUDIO_STREAM_FILLING);
    stream->PauseCount = 0;
    return 0;
}


// int __fastcall Audio_Stream_Channel_End(AudioChannel *channel) -- 0x00407990
// Purpose: the stream channel's end callback (+0xBC): when the stream was
// playing or paused notes the time, clears its playing, paused and
// filling flags and its pause count. Returns 0.

int __fastcall Audio_Stream_Channel_End(AudioChannel *channel)
{
    AudioStreamPlayer *stream = (AudioStreamPlayer *)channel->Owner;
    if (stream->Flags & (AUDIO_STREAM_PLAYING | AUDIO_STREAM_PAUSED)) {
        stream->unknown_20 = Audio_Now();
    }
    stream->Flags &= ~(AUDIO_STREAM_PLAYING | AUDIO_STREAM_PAUSED | AUDIO_STREAM_FILLING);
    stream->PauseCount = 0;
    return 0;
}


// void __fastcall Audio_Stream_Destroy(AudioStreamPlayer *stream) -- 0x004079C0
// Purpose: unlinks the stream (under the stream lock), stops it, releases
// its queues and ring, frees its channel (0x004024A0), deletes its file
// and deletes it.

void __cdecl operator delete(void *block);

void __fastcall Audio_Stream_Destroy(AudioStreamPlayer *stream)
{
    if (stream->Prev != stream) {
        EnterCriticalSection(&Audio_Stream_Lock);
        Audio_List_Remove(stream);
        LeaveCriticalSection(&Audio_Stream_Lock);
    }
    Audio_Stream_Stop(stream);
    if (stream->Fill != 0) {
        Audio_Queue_Release(stream->Fill);
        stream->Fill = 0;
    }
    if (stream->Drain != 0) {
        Audio_Queue_Release(stream->Drain);
        stream->Drain = 0;
    }
    if (stream->Ring != 0) {
        Audio_Ring_Destroy(stream->Ring);
    }
    if (stream->Channel != 0) {
        Audio_Channel_Reset(stream->Channel);
    }
    if (stream->File != 0) {
        delete stream->File;
    }
    operator delete(stream);
}


// void __fastcall Audio_Stream_Set_Levels_94(AudioStreamPlayer *stream, AudioLevelSet *levels) -- 0x00407A70
// Purpose: sets the level set the stream's channel combines at +0x94.

void __fastcall Audio_Stream_Set_Levels_94(AudioStreamPlayer *stream, AudioLevelSet *levels)
{
    stream->Channel->unknown_levels_94 = levels;
}


// void __fastcall Audio_Stream_Set_Levels_9c(AudioStreamPlayer *stream, AudioLevelSet *levels) -- 0x00407A80
// Purpose: sets the level set the stream's channel combines at +0x9C.

void __fastcall Audio_Stream_Set_Levels_9c(AudioStreamPlayer *stream, AudioLevelSet *levels)
{
    stream->Channel->unknown_levels_9c = levels;
}


// int __fastcall Audio_Stream_Open(AudioStreamPlayer *stream, char const *name, int raw) -- 0x00407A90
// Purpose: starts the stream playing the sound file name (a stream that is
// held is left alone): stops its channel, opens the file (a RawFileClass
// first unless raw, otherwise the mixfile-aware CCFileClass), reads its
// wave format, sizes the ring for the buffered time (0x00408460), reads
// the first block into it (the byte count of the read reuses raw's
// slot), points the channel's source at what was read, sets the volume
// to full and starts the channel. Returns 1 when playing, 0 on any
// failure (queues released).

void *__cdecl operator new(unsigned int size);
void __cdecl operator delete(void *block);

int __fastcall Audio_Stream_Open(AudioStreamPlayer *stream, char const *name, int raw)
{
    if (stream->HoldCount == 0) {
        if (stream->Channel != 0) {
            EnterCriticalSection(&Audio_Stream_Lock);
            Audio_Channel_Stop(stream->Channel);
            LeaveCriticalSection(&Audio_Stream_Lock);
        }
        if (stream->File != 0) {
            delete stream->File;
            stream->File = 0;
        }
        if (raw == 0) {
            stream->File = new RawFileClass(name);
            if (stream->File != 0 && !stream->File->Is_Available(0)) {
                if (stream->File != 0) {
                    delete stream->File;
                }
                stream->File = 0;
            }
        }
        if (stream->File == 0) {
            stream->File = new CCFileClass(name);
        }
        if (stream->File != 0) {
            if (!stream->File->Is_Available(0) || !stream->File->Open(FILE_RIGHTS_READ)) {
                if (stream->File != 0) {
                    delete stream->File;
                }
                stream->File = 0;
            } else if (Audio_Format_Read_Wave(stream->File, &stream->Format, &stream->DataSize)) {
                stream->Remaining = stream->DataSize;
                stream->DataStart = stream->File->Seek(0, 1);
                if (stream->Fill != 0) {
                    Audio_Queue_Release(stream->Fill);
                    stream->Fill = 0;
                }
                if (stream->Drain != 0) {
                    Audio_Queue_Release(stream->Drain);
                    stream->Drain = 0;
                }
                AudioStream *ring = stream->Ring;
                __int64 buffered = stream->BufferTime;
                int ring_bytes = stream->Format.Bytes_For_Time(buffered);
                stream->unknown_a0 = stream->Format.Bytes_For_Time(stream->Channel->unknown_1a8 * 2);
                if (Audio_Ring_Size(ring) != ring_bytes) {
                    Audio_Ring_Clear(ring);
                    AudioStreamNode *node = Audio_Ring_Node_Create(ring_bytes, 0);
                    if (node != 0) {
                        Audio_Ring_Add_Node(ring, node);
                    }
                }
                if (Audio_Ring_Size(ring) == ring_bytes) {
                    Audio_Ring_Reset(stream->Ring);
                    stream->Fill = Audio_Ring_Acquire(stream->Ring, 0);
                    stream->Drain = Audio_Ring_Acquire(stream->Ring, 1);
                    if (stream->Fill != 0 && stream->Drain != 0) {
                        unsigned __int64 time = stream->Channel->unknown_1b0 * 2;
                        unsigned __int64 first = time > (unsigned __int64)(Audio_Streamer.Interval * 4) ? time : Audio_Streamer.Interval * 4;
                        if (first <= 4000) {
                            first = 4000;
                        }
                        EnterCriticalSection(&Audio_Stream_Lock);
                        int bytes;
                        if (first != 0) {
                            bytes = stream->Format.Bytes_For_Time(first);
                            if (bytes > stream->Remaining) {
                                bytes = stream->Remaining;
                            }
                        } else {
                            bytes = stream->Remaining;
                        }
                        Audio_Stream_In_Read = true;
                        int result = Audio_Queue_File(stream->Fill, stream->File, bytes, &raw);
                        Audio_Stream_In_Read = false;
                        stream->Remaining -= raw;
                        if (stream->Remaining <= 0 || result == 2 || result == 0) {
                            stream->Flags &= ~AUDIO_STREAM_FILLING;
                        }
                        LeaveCriticalSection(&Audio_Stream_Lock);
                        if (raw != 0) {
                            Audio_Queue_Ready(stream->Drain);
                            stream->Source.unknown_00 = stream->Drain->Pointer;
                            stream->Source.unknown_04 = stream->Format.Bytes_For_Time(stream->Channel->unknown_1a8);
                            if (stream->Source.unknown_04 > stream->Drain->Available) {
                                stream->Source.unknown_04 = stream->Drain->Available;
                            }
                            stream->unknown_90 = stream->Source.unknown_04;
                            stream->Source.Format = &stream->Format;
                            stream->Channel->Levels.Volume.Set(0x4000);
                            Audio_Channel_Set_Source(stream->Channel, &stream->Source);
                            stream->Flags |= AUDIO_STREAM_PLAYING;
                            if (Audio_Channel_Start(stream->Channel) != 0) {
                                stream->Flags &= ~AUDIO_STREAM_PLAYING;
                                Audio_Queue_Release(stream->Fill);
                                stream->Fill = 0;
                                Audio_Queue_Release(stream->Drain);
                                stream->Drain = 0;
                            } else {
                                stream->StartTime = Audio_Now();
                                stream->unknown_20 = stream->StartTime;
                                stream->Flags |= AUDIO_STREAM_FILLING;
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        if (stream->Fill != 0) {
            Audio_Queue_Release(stream->Fill);
            stream->Fill = 0;
        }
        if (stream->Drain != 0) {
            Audio_Queue_Release(stream->Drain);
            stream->Drain = 0;
        }
    }
    return 0;
}


// void __fastcall Audio_Stream_Stop(AudioStreamPlayer *stream) -- 0x00407E60
// Purpose: unless the stream is held, stops its channel under the stream
// lock.

void __fastcall Audio_Stream_Stop(AudioStreamPlayer *stream)
{
    if (stream->HoldCount == 0 && stream->Channel != 0) {
        EnterCriticalSection(&Audio_Stream_Lock);
        Audio_Channel_Stop(stream->Channel);
        LeaveCriticalSection(&Audio_Stream_Lock);
    }
}


// void __fastcall Audio_Stream_Hold(AudioStreamPlayer *stream) -- 0x00407EA0
// Purpose: counts one more hold on the stream.

void __fastcall Audio_Stream_Hold(AudioStreamPlayer *stream)
{
    stream->HoldCount++;
}


// void __fastcall Audio_Stream_Release(AudioStreamPlayer *stream) -- 0x00407EB0
// Purpose: drops one hold on the stream (floored at 0).

void __fastcall Audio_Stream_Release(AudioStreamPlayer *stream)
{
    if (--stream->HoldCount < 0) {
        stream->HoldCount = 0;
    }
}


// void __fastcall Audio_Stream_Pause(AudioStreamPlayer *stream) -- 0x00407ED0
// Purpose: counts a pause; the first pauses the channel under the stream
// lock and turns a playing stream into a paused one.

void __fastcall Audio_Stream_Pause(AudioStreamPlayer *stream)
{
    if (stream->PauseCount == 0 && stream->Channel != 0) {
        EnterCriticalSection(&Audio_Stream_Lock);
        Audio_Channel_Pause(stream->Channel);
        if (stream->Flags & AUDIO_STREAM_PLAYING) {
            stream->Flags &= ~AUDIO_STREAM_PLAYING;
            stream->Flags |= AUDIO_STREAM_PAUSED;
        }
        LeaveCriticalSection(&Audio_Stream_Lock);
    }
    stream->PauseCount++;
}


// void __fastcall Audio_Stream_Resume(AudioStreamPlayer *stream) -- 0x00407F20
// Purpose: drops a pause (floored at 0); the last one turns a paused
// stream back into a playing one and resumes its channel under the stream
// lock.

void __fastcall Audio_Stream_Resume(AudioStreamPlayer *stream)
{
    if (stream->PauseCount != 0) {
        if (--stream->PauseCount < 0) {
            stream->PauseCount = 0;
        }
        if (stream->PauseCount == 0 && stream->Channel != 0) {
            EnterCriticalSection(&Audio_Stream_Lock);
            if (stream->Flags & AUDIO_STREAM_PAUSED) {
                stream->Flags &= ~AUDIO_STREAM_PAUSED;
                stream->Flags |= AUDIO_STREAM_PLAYING;
            }
            Audio_Channel_Resume(stream->Channel);
            LeaveCriticalSection(&Audio_Stream_Lock);
        }
    }
}


// int __fastcall Audio_Stream_Is_Playing(AudioStreamPlayer *stream) -- 0x00407F90
// Purpose: the stream's playing flag.

int __fastcall Audio_Stream_Is_Playing(AudioStreamPlayer *stream)
{
    return stream->Flags & AUDIO_STREAM_PLAYING;
}


// void __fastcall Audio_Stream_Set_Name(AudioStreamPlayer *stream, char const *name) -- 0x00407FA0
// Purpose: copies the stream's name (at most 0x4F characters).

extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);

void __fastcall Audio_Stream_Set_Name(AudioStreamPlayer *stream, char const *name)
{
    strncpy(stream->Name, name, sizeof(stream->Name));
    stream->Name[sizeof(stream->Name) - 1] = 0;
}


// char *__fastcall Audio_Stream_Name(AudioStreamPlayer *stream) -- 0x00407FC0
// Purpose: the stream's name.

char *__fastcall Audio_Stream_Name(AudioStreamPlayer *stream)
{
    return stream->Name;
}


// void __fastcall Audio_Stream_Fade_In(AudioStreamPlayer *stream) -- 0x00407FD0
// Purpose: ramps the stream channel's volume to full.

void __fastcall Audio_Stream_Fade_In(AudioStreamPlayer *stream)
{
    stream->Channel->Levels.Volume.Set_Target(0x4000);
}


// void __fastcall Audio_Stream_Fade_Out(AudioStreamPlayer *stream) -- 0x00407FE0
// Purpose: ramps the stream channel's volume to silence.

void __fastcall Audio_Stream_Fade_Out(AudioStreamPlayer *stream)
{
    stream->Channel->Levels.Volume.Set_Target(0);
}


// int __fastcall Audio_Stream_Is_Fading(AudioStreamPlayer *stream) -- 0x00407FF0
// Purpose: 1 while the stream plays and its channel's volume is still
// moving towards its target.

int __fastcall Audio_Stream_Is_Fading(AudioStreamPlayer *stream)
{
    if ((stream->Flags & AUDIO_STREAM_PLAYING)
        && stream->Channel->Levels.Volume.Current != stream->Channel->Levels.Volume.Target) {
        return 1;
    }
    return 0;
}


// void __fastcall Audio_Stream_Wait(AudioStreamPlayer *stream) -- 0x00408010
// Purpose: keeps the game running (0x00481FB0) while the stream plays and
// its channel's volume is still moving (Audio_Stream_Is_Fading), for at
// most the volume level's MaxElapsed milliseconds.

void __fastcall Audio_Stream_Wait(AudioStreamPlayer *stream)
{
    __int64 end = Audio_Now() + stream->Channel->Levels.Volume.MaxElapsed;
    while (Audio_Stream_Is_Fading(stream) && (unsigned __int64)Audio_Now() < (unsigned __int64)end) {
        unknown_481fb0();
    }
}


// __int64 __fastcall Audio_Stream_Stop_Time(AudioStreamPlayer *stream) -- 0x00408060
// Purpose: the time the stream last stopped playing (+0x20).

__int64 __fastcall Audio_Stream_Stop_Time(AudioStreamPlayer *stream)
{
    return stream->unknown_20;
}


// void Audio_Streams_Stop_Filling() -- 0x00408070
// Purpose: clears AUDIO_STREAM_FILLING on every stream, under the stream lock.

void __fastcall Audio_Streams_Stop_Filling()
{
    if (Audio_Streamer_Initialized != 0) {
        AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List);
        EnterCriticalSection(&Audio_Stream_Lock);
        for (; stream != 0; stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            stream->Flags &= ~AUDIO_STREAM_FILLING;
        }
        LeaveCriticalSection(&Audio_Stream_Lock);
    }
}


// void Audio_Streams_Stop() -- 0x004080C0
// Purpose: stops every stream that is not held.

void Audio_Streams_Stop()
{
    if (Audio_Streamer_Initialized != 0) {
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            Audio_Stream_Stop(stream);
        }
    }
}


// void Audio_Streams_Hold() -- 0x00408120
// Purpose: holds every stream.

void Audio_Streams_Hold()
{
    if (Audio_Streamer_Initialized != 0) {
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            Audio_Stream_Hold(stream);
        }
    }
}


// void Audio_Streams_Release() -- 0x00408150
// Purpose: releases a hold on every stream.

void Audio_Streams_Release()
{
    if (Audio_Streamer_Initialized != 0) {
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            Audio_Stream_Release(stream);
        }
    }
}


// void Audio_Streams_Pause() -- 0x00408190
// Purpose: pauses every stream.

void Audio_Streams_Pause()
{
    if (Audio_Streamer_Initialized != 0) {
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            Audio_Stream_Pause(stream);
        }
    }
}


// void Audio_Streams_Resume() -- 0x00408210
// Purpose: resumes every stream.

void Audio_Streams_Resume()
{
    if (Audio_Streamer_Initialized != 0) {
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            Audio_Stream_Resume(stream);
        }
    }
}


// void Audio_Streams_Fade_Out() -- 0x004082B0
// Purpose: fades every stream out.

void Audio_Streams_Fade_Out()
{
    if (Audio_Streamer_Initialized != 0) {
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            Audio_Stream_Fade_Out(stream);
        }
    }
}


// void Audio_Streams_Fade_In() -- 0x004082F0
// Purpose: fades every stream in.

void Audio_Streams_Fade_In()
{
    if (Audio_Streamer_Initialized != 0) {
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            Audio_Stream_Fade_In(stream);
        }
    }
}


// int Audio_Streams_Done_Fading() -- 0x00408330
// Purpose: 0 while some stream is still fading, else 1.

int Audio_Streams_Done_Fading()
{
    if (Audio_Streamer_Initialized != 0) {
        for (AudioStreamPlayer *stream = (AudioStreamPlayer *)Audio_List_First(&Audio_Stream_List); stream != 0;
             stream = (AudioStreamPlayer *)Audio_List_Next(stream)) {
            if (Audio_Stream_Is_Fading(stream)) {
                return 0;
            }
        }
    }
    return 1;
}
