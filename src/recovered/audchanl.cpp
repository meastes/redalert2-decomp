// audchanl.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audchanl.json carries the addresses and the rows that
// stay in their own files.


#include "shared/audio.hpp"
#include "shared/crt_memory.hpp"


// AudioChannel *__fastcall Audio_Channel_Create(AudioDevice *device) -- 0x00402040
// Purpose: makes a channel for the device: a zero-filled block with its own
// level sets (Levels, Settings) at neutral, the library's next-piece,
// source-ended and finished handlers, the master levels (0x00830B50) and
// the device's default format, then lets the device's channel driver open
// it. Returns 0 (closing and deleting the channel) when the driver refuses.

void *__cdecl operator new(unsigned int size);
void __cdecl operator delete(void *block);

AudioChannel *__fastcall Audio_Channel_Create(AudioDevice *device)
{
    AudioChannel *channel = (AudioChannel *)operator new(sizeof(AudioChannel));
    memset(channel, 0, sizeof(AudioChannel));
    Audio_List_Init_Node(channel);
    channel->LoopCount = 0;
    channel->Flags = 0;
    channel->Settings.Init();
    channel->unknown_next_block_15c = Audio_Channel_Next_Piece;
    channel->unknown_end_of_source_160 = Audio_Channel_Source_Ended;
    channel->unknown_stop_callback_164 = Audio_Channel_Finished;
    channel->unknown_next_b0 = 0;
    channel->unknown_loop_b4 = 0;
    channel->unknown_done_b8 = 0;
    channel->unknown_end_callback_bc = 0;
    channel->Owner = 0;
    channel->unknown_0c = 1;
    channel->LoopCount = 0;
    channel->Flags = 0;
    channel->unknown_a0 = 0;
    channel->unknown_levels_94 = Audio_Master_Levels;
    channel->unknown_levels_90 = 0;
    channel->unknown_levels_98 = 0;
    channel->unknown_levels_9c = 0;
    channel->Levels.Init();
    channel->Source = 0;
    channel->Device = device;
    channel->Driver = device->ChannelDriver;
    channel->Format = device->DefaultFormat;
    channel->FormatChanged = 0;
    channel->unknown_1a0 = 0;
    if (device->ChannelDriver->Open(channel) != 0) {
        if (channel != 0) {
            channel->Driver->Close(channel);
            operator delete(channel);
        }
        return 0;
    }
    return channel;
}


// void __fastcall unknown_release_and_delete(UnknownComHolder_00402170 *) -- 0x00402170
// Purpose: calls the third entry of the function table that +0xD4 points
// to, then frees the holder with operator delete. No direct callers.

struct UnknownFunctionTable_00402170 {
    void (*unknown_00)();
    void (*unknown_04)();
    void (*unknown_08)();
};

struct UnknownComHolder_00402170 {
    char unknown_00[0xd4];
    UnknownFunctionTable_00402170 *unknown_d4;
};

void __fastcall unknown_release_and_delete_00402170(UnknownComHolder_00402170 *holder)
{
    holder->unknown_d4->unknown_08();
    operator delete(holder);
}


extern "C" unsigned char data_402190_4022c0[];
extern "C" unsigned char data_402190_402330[];
extern "C" unsigned char data_402190_4023f0[];
extern "C" unsigned int global_402190_830b50;
class Call_402190_0 {public: unsigned int Init_00401000();};
class Leaf_402190 { public: unsigned int get(); };
unsigned int Leaf_402190::get() { *(unsigned int*)(((unsigned int)this) + 0x15c) = (unsigned int)data_402190_4022c0; *(unsigned int*)(((unsigned int)this) + 0x160) = (unsigned int)data_402190_402330; *(unsigned int*)(((unsigned int)this) + 0x164) = (unsigned int)data_402190_4023f0; *(unsigned int*)(((unsigned int)this) + 0xb0) = 0; *(unsigned int*)(((unsigned int)this) + 0xb4) = 0; *(unsigned int*)(((unsigned int)this) + 0xb8) = 0; *(unsigned int*)(((unsigned int)this) + 0xbc) = 0; *(unsigned int*)(((unsigned int)this) + 0xc0) = 0; *(unsigned int*)(((unsigned int)this) + 0xc) = 1; *(unsigned int*)(((unsigned int)this) + 0xa8) = 0; *(unsigned int*)(((unsigned int)this) + 0xa4) = 0; *(unsigned int*)(((unsigned int)this) + 0xa0) = 0; *(unsigned int*)(((unsigned int)this) + 0x94) = global_402190_830b50; *(unsigned int*)(((unsigned int)this) + 0x90) = 0; *(unsigned int*)(((unsigned int)this) + 0x98) = 0; *(unsigned int*)(((unsigned int)this) + 0x9c) = 0; unsigned int initialization_result = ((Call_402190_0*)(((unsigned int)this) + 0x10))->Init_00401000(); *(unsigned int*)(((unsigned int)this) + 0x168) = 0; return initialization_result; }


// 0x00402430: function not yet identified
//
// Body: 10 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: uint __fastcall FUN_00402430(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00402430 {

unsigned int __fastcall FUN_00402430(int channel_address)
{
    return *(unsigned int *)(channel_address + 0xa4) & 8;
}

}  // namespace seed_00402430


// void __fastcall Audio_Channel_Reuse(AudioChannel *channel) -- 0x004024A0
// Purpose: gives a free channel (flag 8) a fresh start: clears the flag and
// puts the handlers, counters, levels and source back as 0x00402040 left a
// new channel. Does nothing for a channel that is not free.

void __fastcall Audio_Channel_Reuse(AudioChannel *channel)
{
    if (channel->Flags & AUDIO_CHANNEL_FREE) {
        channel->Flags &= ~AUDIO_CHANNEL_FREE;
        channel->unknown_next_block_15c = Audio_Channel_Next_Piece;
        channel->unknown_end_of_source_160 = Audio_Channel_Source_Ended;
        channel->unknown_stop_callback_164 = Audio_Channel_Finished;
        channel->unknown_next_b0 = 0;
        channel->unknown_loop_b4 = 0;
        channel->unknown_done_b8 = 0;
        channel->unknown_end_callback_bc = 0;
        channel->Owner = 0;
        channel->unknown_0c = 1;
        channel->LoopCount = 0;
        channel->Flags = 0;
        channel->unknown_a0 = 0;
        channel->unknown_levels_94 = Audio_Master_Levels;
        channel->unknown_levels_90 = 0;
        channel->unknown_levels_98 = 0;
        channel->unknown_levels_9c = 0;
        channel->Levels.Init();
        channel->Source = 0;
    }
}


// void __fastcall Audio_Channel_Destroy(AudioChannel *channel) -- 0x00402550
// Purpose: stops the channel as Audio_Channel_Stop does, detaches it
// from its device (0x004028B0), closes it in the driver and deletes it.

void __cdecl operator delete(void *block);

void __fastcall Audio_Channel_Destroy(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
    if (channel->Flags & (AUDIO_CHANNEL_PLAYING | AUDIO_CHANNEL_PAUSED)) {
        channel->Driver->Stop(channel);
    }
    channel->Driver->Unlock(channel);
    if (channel->unknown_end_callback_bc != 0) {
        channel->unknown_end_callback_bc(channel);
    }
    channel->Source = 0;
    Audio_Channel_Detach(channel);
    channel->Driver->Close(channel);
    operator delete(channel);
}


// void __fastcall Audio_Channel_Stop(AudioChannel *channel) -- 0x004025B0
// Purpose: under the driver lock, stops a playing or paused channel in the
// driver; then runs the channel's end callback (+0xBC) and detaches its
// source.

void __fastcall Audio_Channel_Stop(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
    if (channel->Flags & (AUDIO_CHANNEL_PLAYING | AUDIO_CHANNEL_PAUSED)) {
        channel->Driver->Stop(channel);
    }
    channel->Driver->Unlock(channel);
    if (channel->unknown_end_callback_bc != 0) {
        channel->unknown_end_callback_bc(channel);
    }
    channel->Source = 0;
}


// void __fastcall Audio_Channel_Pause(AudioChannel *channel) -- 0x00402600
// Purpose: under the driver lock, pauses a playing channel in the driver
// and marks it paused.

void __fastcall Audio_Channel_Pause(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
    if (channel->Flags & AUDIO_CHANNEL_PLAYING) {
        channel->Flags &= ~AUDIO_CHANNEL_PLAYING;
        channel->Driver->Pause(channel);
        channel->Flags |= AUDIO_CHANNEL_PAUSED;
    }
    channel->Driver->Unlock(channel);
}


// void __fastcall Audio_Channel_Resume(AudioChannel *channel) -- 0x00402650
// Purpose: under the driver lock, resumes a paused channel in the driver
// and marks it playing.

void __fastcall Audio_Channel_Resume(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
    if (channel->Flags & AUDIO_CHANNEL_PAUSED) {
        channel->Driver->Resume(channel);
        channel->Flags &= ~AUDIO_CHANNEL_PAUSED;
        channel->Flags |= AUDIO_CHANNEL_PLAYING;
    }
    channel->Driver->Unlock(channel);
}


// void Audio_Channel_Lock(AudioChannel *channel) (0x004026A0): the channel driver's Lock (+0x28).

void __fastcall Audio_Channel_Lock(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
}


// void Audio_Channel_Unlock(AudioChannel *channel) (0x004026B0): the channel driver's Unlock (+0x2C).

void __fastcall Audio_Channel_Unlock(AudioChannel *channel)
{
    channel->Driver->Unlock(channel);
}


// void __fastcall Audio_Channel_Set_Flag_10(AudioChannel *channel) -- 0x004026C0
// Purpose: sets channel flag 0x10 under the driver lock.

void __fastcall Audio_Channel_Set_Flag_10(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
    channel->Flags |= AUDIO_CHANNEL_FLAG_10;
    channel->Driver->Unlock(channel);
}


// void __fastcall Audio_Channel_Clear_Flag_10(AudioChannel *channel) -- 0x004026F0
// Purpose: clears channel flag 0x10 under the driver lock.

void __fastcall Audio_Channel_Clear_Flag_10(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
    channel->Flags &= ~AUDIO_CHANNEL_FLAG_10;
    channel->Driver->Unlock(channel);
}


// HRESULT __fastcall Audio_Channel_Start(AudioChannel *channel) -- 0x00402720
// Purpose: starts a channel that is neither playing nor paused and has a
// source: updates its levels and combined settings, and under the driver lock starts
// it in the driver, marking it playing on success. Returns the driver's
// result, 0x800F0000 when the channel is already active or 0x80070000
// without a source.

HRESULT __fastcall Audio_Channel_Start(AudioChannel *channel)
{
    if (channel->Flags & (AUDIO_CHANNEL_PLAYING | AUDIO_CHANNEL_PAUSED)) {
        return 0x800f0000;
    }
    if (channel->Source == 0) {
        return 0x80070000;
    }
    channel->Levels.Update();
    Audio_Channel_Update_Settings(channel);
    channel->Driver->Lock(channel);
    HRESULT result = channel->Driver->Start(channel);
    if (result == DS_OK) {
        channel->Flags |= AUDIO_CHANNEL_PLAYING;
    }
    channel->Driver->Unlock(channel);
    return result;
}


// void __fastcall Audio_Channel_Set_Source(AudioChannel *channel, AudioSource *source) -- 0x00402790
// Purpose: attaches a source to the channel. When the source has info
// (0x00408440) the channel keeps it and copies its +0x0C and +0x10 words
// into +0x170 and +0x174/+0x178, clearing the source's first word;
// otherwise the source's own two words are copied there.

void __fastcall Audio_Channel_Set_Source(AudioChannel *channel, AudioSource *source)
{
    channel->Source = source;
    if (source != 0) {
        AudioSourceInfo *info = source->Info();
        channel->SourceInfo = info;
        if (info != 0) {
            channel->unknown_170 = info->unknown_0c;
            channel->unknown_178 = channel->unknown_174 = info->unknown_10;
            source->unknown_00 = 0;
            return;
        }
        channel->unknown_170 = channel->Source->unknown_00;
        channel->unknown_178 = channel->unknown_174 = channel->Source->unknown_04;
    }
}


// int __fastcall Audio_Channel_Set_Format(AudioChannel *channel, AudioFormat const *format) -- 0x00402800
// Purpose: records the channel's format (+0x17C); FormatChanged (+0x19C)
// tells whether it differed. Returns that flag.

int __fastcall Audio_Channel_Set_Format(AudioChannel *channel, AudioFormat const *format)
{
    if (memcmp(format, &channel->Format, sizeof(AudioFormat)) == 0) {
        channel->FormatChanged = 0;
    } else {
        channel->FormatChanged = 1;
        channel->Format = *format;
    }
    return channel->FormatChanged;
}


// 0x00402850: function not yet identified
//
// Body: 9 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_00402850(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00402850 {

void __fastcall FUN_00402850(int object_address)
{
    *(unsigned int *)(object_address + 8) = 0;
    *(unsigned int *)(object_address + 4) = 0;
    return;
}

}  // namespace seed_00402850
