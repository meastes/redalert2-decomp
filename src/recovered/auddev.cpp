// auddev.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/auddev.json carries the addresses and the rows that
// stay in their own files.


#include "shared/audio.hpp"
#include "shared/crt_string.hpp"


// void __fastcall Audio_Device_Add_Channel(AudioDevice *device, AudioChannel *channel) -- 0x00402860
// Purpose: gives the channel to the device: sets its device, counts it and
// links it at the tail of the device's channel list, holding the device's
// busy count (+0x104, floored at 0 on release) meanwhile.

void __fastcall Audio_Device_Add_Channel(AudioDevice *device, AudioChannel *channel)
{
    channel->Device = device;
    device->ChannelCount++;
    device->unknown_busy_104++;
    Audio_List_Add_Tail(&device->ChannelList, channel);
    if (--device->unknown_busy_104 < 0) {
        device->unknown_busy_104 = 0;
    }
}


// void __fastcall Audio_Channel_Detach(AudioChannel *channel) -- 0x004028B0
// Purpose: takes the channel back from its device: unlinks it from the
// device's channel list and uncounts it, then unlinks it again while
// holding the device's busy count (+0x104, floored at 0 on release).

void __fastcall Audio_Channel_Detach(AudioChannel *channel)
{
    AudioDevice *device = channel->Device;
    Audio_List_Remove(channel);
    device->ChannelCount--;
    device->unknown_busy_104++;
    Audio_List_Remove(channel);
    if (--device->unknown_busy_104 < 0) {
        device->unknown_busy_104 = 0;
    }
}


extern "C" unsigned int global_402940_7cc968;
class Call_402940_0 {public: unsigned int Audio_List_Init_004071f0();};
class Call_402940_1 {public: unsigned int Audio_List_Init_004071f0();};
extern "C" unsigned int global_402940_830b50;
class Call_402940_2 {public: unsigned int Init_00401000();};
extern "C" unsigned int global_402940_830b4c;
class Leaf_402940 { public: unsigned int get(); };
unsigned int Leaf_402940::get() { unsigned int unused_list_init_return = ((Call_402940_0*)(global_402940_7cc968))->Audio_List_Init_004071f0(); unsigned int unused_sublist_init_return = ((Call_402940_1*)((global_402940_7cc968) + 0xc))->Audio_List_Init_004071f0(); *(unsigned int*)((global_402940_7cc968) + 0x18) = 0; global_402940_830b50 = (global_402940_7cc968) + 0x20; unsigned int unused_level_init_return = ((Call_402940_2*)((global_402940_7cc968) + 0x20))->Init_00401000(); global_402940_830b4c = 1; return 0; }


extern "C" unsigned int global_402c00_7cc968;
class Call_402c00_0 {public: unsigned int Audio_List_First_004072f0();};
class Leaf_402c00 { public: unsigned int get(); };
unsigned int Leaf_402c00::get() { unsigned int first_node_address = ((Call_402c00_0*)(global_402c00_7cc968))->Audio_List_First_004072f0(); *(unsigned int*)(((unsigned int)this)) = first_node_address; return first_node_address; }


// void __fastcall Audio_Device_Destroy(AudioDevice *device) -- 0x00402EF0
// Purpose: destroys the device's channels, closes it through its driver,
// frees its slot in the driver (the driver's device count floored at 0)
// and deletes it.

void __cdecl operator delete(void *block);

void __fastcall Audio_Device_Destroy(AudioDevice *device)
{
    AudioChannel *node = (AudioChannel *)Audio_List_First(&device->ChannelList);
    AudioChannel *channel = node;
    while (node != 0) {
        node = (AudioChannel *)Audio_List_Next(node);
        Audio_Channel_Destroy(channel);
        channel = node;
    }
    device->Driver->Info->Close(device);
    device->Driver->Devices[device->DriverIndex] = 0;
    if ((device->Driver->DeviceCount -= 1) < 0) {
        device->Driver->DeviceCount = 0;
    }
    operator delete(device);
}


// int Audio_Device_Update(AudioDevice *device) (0x00402F60)
// The device's update function (+0x10C until the device is run): returns
// 0x80120000 while the device is busy; once the level span is due, ticks it,
// updates the device levels (and the optional +0xE8 set) and, for every
// playing channel with a source whose own or linked levels changed (or when
// the device levels did), recombines the channel's settings and has its
// driver apply them; then clears the changed flags of the device levels.

int __fastcall Audio_Device_Update(AudioDevice *device)
{
    if (device->unknown_busy_104 != 0) {
        return 0x80120000;
    }
    __int64 now = Audio_Now();
    if (!device->LevelSpan.Is_Due(now)) {
        return 0;
    }
    device->LevelSpan.Tick(now);
    device->Levels.Update();
    if (device->unknown_levels_e8 != 0) {
        device->unknown_levels_e8->Update();
    }
    int changed;
    if (device->unknown_levels_e8 == 0 || (changed = device->unknown_levels_e8->Changed()) == 0) {
        changed = device->Levels.Changed();
    }
    AudioChannel *channel = (AudioChannel *)device->ChannelList.Next;
    while (channel != (AudioChannel *)&device->ChannelList) {
        AudioChannel *next = (AudioChannel *)channel->Next;
        if ((channel->Flags & AUDIO_CHANNEL_PLAYING) && channel->Source != 0) {
            channel->Levels.Update();
            if (changed || channel->Levels.Changed()
                || (channel->unknown_levels_94 != 0 && channel->unknown_levels_94->Changed())
                || (channel->unknown_levels_98 != 0 && channel->unknown_levels_98->Changed())
                || (channel->unknown_levels_9c != 0 && channel->unknown_levels_9c->Changed())
                || (channel->unknown_levels_90 != 0 && channel->unknown_levels_90->Changed())) {
                Audio_Channel_Update_Settings(channel);
                channel->Driver->Apply_Settings(channel);
            }
        }
        channel = next;
    }
    if (device->unknown_levels_e8 != 0) {
        device->unknown_levels_e8->Clear_Changed();
    }
    device->Levels.Clear_Changed();
    return 0;
}


// void Audio_Device_Set_Format(AudioDevice *device, AudioFormat const *format) (0x004031B0)
// Copies the format into the device's default format (name hypothesis).

void __fastcall Audio_Device_Set_Format(AudioDevice *device, AudioFormat const *format)
{
    device->DefaultFormat = *format;
}


// void Audio_Device_Release(AudioDevice *device) (0x00403310)
// Drops one user of the device (the count never goes below 0); the last
// user removes the device from the system's list and destroys it.

void __fastcall Audio_Device_Release(AudioDevice *device)
{
    if (--device->RefCount < 0) {
        device->RefCount = 0;
    }
    if (device->RefCount == 0) {
        Audio_Device_Remove(device);
    }
}


// void __fastcall Audio_Device_Remove(AudioDevice *device) -- 0x004033C0
// Purpose: unlinks the device from the system's device list (the system
// marked busy meanwhile) and destroys it as 0x00402EF0 does.

void __cdecl operator delete(void *block);

void __fastcall Audio_Device_Remove(AudioDevice *device)
{
    Audio_System_Instance->Busy++;
    Audio_List_Remove(device);
    if (--Audio_System_Instance->Busy < 0) {
        Audio_System_Instance->Busy = 0;
    }
    Audio_Device_Destroy(device);
}


// 0x00403460: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_00403460(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00403460 {

unsigned int  FUN_00403460(void)
{
    return 0x80040000;
}

}  // namespace seed_00403460


// void Audio_Device_Set_Volume(AudioDevice *device, int volume) (0x00403470)
// Sets the device's master volume at once (name hypothesis).

void __fastcall Audio_Device_Set_Volume(AudioDevice *device, int volume)
{
    device->Levels.Volume.Set(volume);
}


// void Audio_Device_Fade_Volume(AudioDevice *device, int volume) (0x00403480)
// Ramps the device's master volume to the value (name hypothesis).

void __fastcall Audio_Device_Fade_Volume(AudioDevice *device, int volume)
{
    device->Levels.Volume.Set_Target(volume);
}


// 0x00403490: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall FUN_00403490(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00403490 {

unsigned int __fastcall FUN_00403490(int object_address)
{
    return *(unsigned int *)(object_address + 0xf4);
}

}  // namespace seed_00403490


// 0x004034a0: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall FUN_004034a0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004034a0 {

unsigned int __fastcall FUN_004034a0(int object_address)
{
    return *(unsigned int *)(object_address + 0xf0);
}

}  // namespace seed_004034a0


// AudioChannel *__fastcall Audio_Device_New_Channel(AudioDevice *device) -- 0x004034B0
// Purpose: makes one more channel for the device unless it already has its
// maximum (+0xF4) and adds it (0x00402860, expanded inline). Returns the
// channel, or 0.

AudioChannel *__fastcall Audio_Device_New_Channel(AudioDevice *device)
{
    AudioChannel *channel;
    if (device->ChannelCount >= device->unknown_f4) {
        goto fail;
    }
    channel = Audio_Channel_Create(device);
    if (channel == 0) {
fail:
        return 0;
    }
    Audio_Device_Add_Channel(device, channel);
    return channel;
}


// int __fastcall Audio_Device_New_Channels(AudioDevice *device, int count) -- 0x00403530
// Purpose: makes up to count channels (0x004034B0, expanded inline),
// stopping when the device cannot take another. Returns how many were
// made.

int __fastcall Audio_Device_New_Channels(AudioDevice *device, int count)
{
    int made = 0;
    do {
        if (Audio_Device_New_Channel(device) == 0) {
            break;
        }
        made++;
    } while (--count != 0);
    return made;
}


// AudioListNode *AudioListIterator::Next() -- 0x004035D0
// Purpose: advances the iterator to the next node of its list (a null
// iterator stays null) and returns it.

AudioListNode *AudioListIterator::Next()
{
    if (Node != 0) {
        Node = Audio_List_Next(Node);
    }
    return Node;
}


// AudioChannel *__fastcall Audio_Device_Find_Channel(AudioDevice *device, int id) -- 0x004035F0
// Purpose: picks a channel of the device (id -1 means 1) to take a new
// sound: the first one that is neither playing, paused nor flagged 0x10, or
// else the lowest priority (+0xA0) one among the others, preferring a
// stopped one over a playing one of equal or higher priority; among playing
// ones of equal priority the quieter (combined volume) is taken, but only
// when it is at least 0x666 quieter.

AudioChannel *__fastcall Audio_Device_Find_Channel(AudioDevice *device, int id)
{
    if (id == -1) {
        id = 1;
    }
    int idle_priority = 0x7f;
    AudioChannel *idle = 0;
    int playing_priority = 0x7f;
    AudioChannel *playing = 0;
    unsigned int playing_volume = (unsigned int)-1;
    AudioListCursor cursor;
    cursor.First(&device->ChannelList);
    AudioChannel *channel;
    for (channel = (AudioChannel *)cursor.Node; channel != 0; channel = (AudioChannel *)cursor.Next()) {
        if (channel->unknown_0c != id) {
            continue;
        }
        if (!(channel->Flags & (AUDIO_CHANNEL_PLAYING | AUDIO_CHANNEL_PAUSED | AUDIO_CHANNEL_FLAG_10))) {
            break;
        }
        if (channel->Flags & AUDIO_CHANNEL_PLAYING) {
            if (channel->unknown_a0 < playing_priority) {
                playing_volume = channel->Settings.Volume.Current;
                playing_priority = channel->unknown_a0;
                playing = channel;
            } else if (channel->unknown_a0 == playing_priority) {
                unsigned int volume = channel->Settings.Volume.Current;
                if (volume < playing_volume && playing_volume - volume >= 0x666) {
                    playing_priority = channel->unknown_a0;
                    playing = channel;
                    playing_volume = volume;
                }
            }
        } else if (channel->unknown_a0 < idle_priority) {
            idle_priority = channel->unknown_a0;
            idle = channel;
        }
    }
    if (channel != 0) {
        return channel;
    }
    AudioChannel *result = idle;
    if (result == 0 || idle_priority > playing_priority) {
        result = playing;
    }
    return result;
}


// AudioChannel *__fastcall Audio_Device_Create_Channel(AudioDevice *device, int flags) -- 0x004036C0
// Purpose: takes a channel for a new sound (0x004035F0 with id 1) and
// releases it with the given value (0x00402440). Returns the channel, or 0.

AudioChannel *__fastcall Audio_Device_Create_Channel(AudioDevice *device, int flags)
{
    AudioChannel *channel = Audio_Device_Find_Channel(device, 1);
    if (channel != 0) {
        Audio_Channel_Release(channel, flags);
    }
    return channel;
}


// int __fastcall Audio_Device_Destroy_Channels(AudioDevice *device, int id) -- 0x004036F0
// Purpose: destroys the device's channels with the given id (all of them
// for -1). Returns 0.

int __fastcall Audio_Device_Destroy_Channels(AudioDevice *device, int id)
{
    AudioChannel *node = (AudioChannel *)Audio_List_First(&device->ChannelList);
    AudioChannel *channel = node;
    while (node != 0) {
        node = (AudioChannel *)Audio_List_Next(node);
        if (id == -1 || channel->unknown_0c == id) {
            Audio_Channel_Destroy(channel);
        }
        channel = node;
    }
    return 0;
}


// void __fastcall Audio_Device_Stop_Channels(AudioDevice *device, int id) -- 0x00403730
// Purpose: stops the device's channels with the given id (all of them for -1).

void __fastcall Audio_Device_Stop_Channels(AudioDevice *device, int id)
{
    AudioChannel *node = (AudioChannel *)Audio_List_First(&device->ChannelList);
    AudioChannel *channel = node;
    while (node != 0) {
        node = (AudioChannel *)Audio_List_Next(node);
        if (id == -1 || channel->unknown_0c == id) {
            Audio_Channel_Stop(channel);
        }
        channel = node;
    }
}


// void __fastcall Audio_Device_Pause_Channels(AudioDevice *device, int id) -- 0x00403770
// Purpose: pauses the device's playing channels with the given id (all of
// them for -1).

void __fastcall Audio_Device_Pause_Channels(AudioDevice *device, int id)
{
    AudioListCursor cursor;
    cursor.First(&device->ChannelList);
    for (AudioChannel *channel = (AudioChannel *)cursor.Node; channel != 0; channel = (AudioChannel *)cursor.Next()) {
        if ((channel->Flags & AUDIO_CHANNEL_PLAYING) && (id == -1 || channel->unknown_0c == id)) {
            Audio_Channel_Pause(channel);
        }
    }
}


// void __fastcall Audio_Device_Resume_Channels(AudioDevice *device, int id) -- 0x004037C0
// Purpose: resumes the device's paused channels with the given id (all of
// them for -1).

void __fastcall Audio_Device_Resume_Channels(AudioDevice *device, int id)
{
    AudioListCursor cursor;
    cursor.First(&device->ChannelList);
    for (AudioChannel *channel = (AudioChannel *)cursor.Node; channel != 0; channel = (AudioChannel *)cursor.Next()) {
        if ((channel->Flags & AUDIO_CHANNEL_PAUSED) && (id == -1 || channel->unknown_0c == id)) {
            Audio_Channel_Resume(channel);
        }
    }
}


// void AudioTimeSpan::Reset() -- 0x00403820
// Purpose: clears the statistics (the peak window stays) and sets +0x40 to 5000.

void AudioTimeSpan::Reset()
{
    unknown_00 = 0;
    LateLimit = 0;
    Elapsed = 0;
    Peak = 0;
    LastTime = 0;
    TickCount = 0;
    unknown_50 = 0;
    unknown_40 = 5000;
    Maximum = 0;
    unknown_38 = 0;
    unknown_54 = 0;
    LateCount = 0;
}


// void AudioTimeSpan::Set(__int64 value) -- 0x00403870
// Purpose: sets the period and the late limit to value and the peak
// window to ten times the period.

void AudioTimeSpan::Set(__int64 value)
{
    unknown_00 = value;
    LateLimit = value;
    PeakWindow = unknown_00 * 10;
}


// void AudioTimeSpan::Set_08(__int64 value) -- 0x004038A0
// Purpose: sets the late limit.

void AudioTimeSpan::Set_08(__int64 value)
{
    LateLimit = value;
}


// void AudioTimeSpan::Set_20(__int64 value) -- 0x004038C0
// Purpose: sets the peak window.

void AudioTimeSpan::Set_20(__int64 value)
{
    PeakWindow = value;
}


// int AudioTimeSpan::Is_Due(__int64 now) (0x004038E0)
// Whether a full period (+0x00) has passed since the last tick (+0x28),
// compared unsigned.

int AudioTimeSpan::Is_Due(__int64 now)
{
    if ((unsigned __int64)(now - LastTime) < (unsigned __int64)unknown_00) {
        return 0;
    }
    return 1;
}


// void AudioTimeSpan::Tick(__int64 now) -- 0x00403920
// Purpose: records one mixer tick at time now: the elapsed time since the
// previous tick, the tick count, the peak elapsed time (restarted when it
// exceeds the peak window), late ticks (elapsed above the late limit) and
// the maximum elapsed time.

void AudioTimeSpan::Tick(__int64 now)
{
    __int64 elapsed = now - LastTime;
    Elapsed = elapsed;
    TickCount++;
    if ((unsigned __int64)Peak > (unsigned __int64)PeakWindow) {
        Peak = 0;
    }
    if ((unsigned __int64)elapsed > (unsigned __int64)Peak) {
        Peak = elapsed;
    }
    if ((unsigned __int64)elapsed > (unsigned __int64)LateLimit) {
        LateCount++;
    }
    if ((unsigned __int64)elapsed > (unsigned __int64)Maximum) {
        Maximum = elapsed;
    }
    LastTime = now;
}


// void __fastcall AudioTimeSpan::Format(char *buffer) -- 0x004039C0
// Purpose: print "period (elapsed,maximum,peak,~late) spinner" for the device dump; the spinner advances once per tick.

extern char *Audio_Time_Span_Spinner;

void __fastcall AudioTimeSpan::Format(char *buffer)
{
    __int64 now = Audio_Now();
    if (TickCount != unknown_50) {
        if ((unsigned)++unknown_54 >= strlen(Audio_Time_Span_Spinner)) {
            unknown_54 = 0;
        }
        unknown_50 = TickCount;
    }
    if ((unsigned __int64)(unknown_40 + unknown_38) < (unsigned __int64)now) {
        Maximum = unknown_00;
        unknown_38 = now;
    }
    sprintf(buffer, "%05dms (%04d,%04d,%04d,~%2d) %c \n", (int)unknown_00, (int)Elapsed, (int)Maximum, (int)Peak,
            LateCount, Audio_Time_Span_Spinner[unknown_54]);
}


// void Audio_Device_Dump(AudioDevice *device, void (__fastcall *print)(char const *), int with_names) (0x00403A80)
// Prints the device (driver name and index), its mixer and level timing, the frame counters and one line per channel
// (index, type, state, format, priority, volume, name) through the print function (name hypothesis).

void __fastcall Audio_Device_Dump(AudioDevice *device, void (__fastcall *print)(char const *), int with_names)
{
    char line[200];
    char span[100];
    char name[200];
    char format[200];

    print("Audio Device Dump ---------------------\n");
    sprintf(line, "%s (%d)\n", device->Driver->Info->Name, device->Id);
    print(line);
    device->Span.Format(span);
    sprintf(line, "mixer:%s", span);
    print(line);
    device->LevelSpan.Format(span);
    sprintf(line, "level:%s", span);
    print(line);
    sprintf(line, "Frames: %d Oversamp: %d Lag: %d", device->unknown_1d8, device->unknown_1dc, device->unknown_1e0);
    print(line);
    if (with_names) {
        sprintf(line, "Chan:TYPE: STATE : FORMAT  :PRI :VOL: NAME\n");
    } else {
        sprintf(line, "Chan:TYPE: STATE : FORMAT  :PRI :VOL:\n");
    }
    print(line);
    AudioListCursor cursor;
    cursor.First(&device->ChannelList);
    int index = 0;
    for (AudioChannel *channel = (AudioChannel *)cursor.Node; channel != 0; channel = (AudioChannel *)cursor.Next()) {
        char const *state;
        if (channel->Flags & AUDIO_CHANNEL_PLAYING) {
            state = "PLAYING";
        } else if (channel->Flags & AUDIO_CHANNEL_PAUSED) {
            state = "PAUSED ";
        } else {
            state = "FREE   ";
        }
        char const *type;
        if (channel->unknown_0c >= 0x1000) {
            type = "USER";
        } else if (channel->unknown_0c == 1) {
            type = "STD ";
        } else {
            type = "??? ";
        }
        name[0] = '\0';
        sprintf(format, "%02d-%2d-%c%c%c", channel->Format.Rate / 1000,
                channel->Format.Type == AUDIO_FORMAT_PCM ? channel->Format.BytesPerSample * 8 : 4,
                channel->Format.Channels == 1 ? 'M' : 'S', channel->FormatChanged ? '*' : ' ',
                channel->unknown_1a0 ? '+' : ' ');
        sprintf(line, "#%2d :%s:%s:%s:%4d:%3d:%.10s\n", index, type, state, format, channel->unknown_a0,
                ((channel->Settings.Volume.Current >> 16) * 100) >> 14, name);
        print(line);
        index++;
    }
    print("--------------------------------------\n");
}
