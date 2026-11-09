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
#include "shared/audio.hpp"
#include "shared/crt_memory.hpp"
#include "shared/debug_printf.hpp"
// @ref ??_C@_0BK@FIAO@Failed?5to?5prefill?5buffer?6?$AA@=007cd2f0
// @ref ??_C@_0BN@ELOL@Sound?5frame?5size?5?$DN?5?$CFd?5bytes?6?$AA@=007cd2d0
// @ref ??_C@_0CC@HKBA@Failed?5to?5create?5playback?5buffer@=007cd2ac
// @ref ??_C@_0BO@KHOK@Sound?5buffer?5size?5?$DN?5?$CFd?5bytes?6?$AA@=007cd28c
// @ref ??_C@_0BK@HNLL@Failed?5to?5start?5playback?6?$AA@=007cd270

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
