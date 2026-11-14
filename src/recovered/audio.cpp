// audio.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audio.json carries the addresses and the rows that
// stay in their own files.


#include "shared/mix_file_class.hpp"
#include "shared/cd.hpp"
#include "shared/audio.hpp"
#include "shared/vox_class.hpp"
#include "shared/sound_available.hpp"


// int __fastcall Audio_Init(int enable) -- 0x00406AD0
// Purpose: starts the audio library: resets the global level sets, and when
// enabled opens the driver and a 22 kHz 16-bit stereo device with 16 channels,
// AUDIO.MIX and the "audio" bag. Starts the streamer and the event system.
// Anything that fails turns the audio off (Audio_Is_On = 0). Always returns 1.

void unknown_007156d0();    // 0x007156D0
void unknown_00715f10();    // 0x00715F10

int __fastcall Audio_Init(int enable)
{
    Audio_Timer_Init();
    Audio_Level_Set_830fd8.Init();
    Audio_Global_Levels[0] = &Audio_Level_Set_830fd8;
    Audio_Level_Set_8311f8.Init();
    Audio_Global_Levels[2] = &Audio_Level_Set_8311f8;
    Audio_Level_Set_831290.Init();
    Audio_Global_Levels[1] = &Audio_Level_Set_831290;
    Audio_Level_Set_831168.Init();
    Audio_Global_Levels[3] = &Audio_Level_Set_831168;
    Audio_Level_Set_831058.Init();
    Audio_Global_Levels[4] = &Audio_Level_Set_831058;
    Audio_Level_Set_8313b0.Init();
    Audio_Global_Levels[6] = &Audio_Level_Set_8313b0;
    Audio_Level_Set_831310.Init();
    Audio_Global_Levels[5] = &Audio_Level_Set_831310;
    Audio_Level_Set_8310d8.Init();
    Audio_Global_Levels[7] = &Audio_Level_Set_8310d8;
    if (enable != 0) {
        Audio_System_Init();
        Audio_Create_Driver(Audio_Default_Driver);
        AudioFormat format;
        Audio_Format_Init(&format);
        format.Rate = 22050;
        format.Type = AUDIO_FORMAT_PCM;
        format.Channels = 2;
        format.BytesPerSample = 2;
        format.unknown_00 = 4;
        format.Update_Rate();
        Audio_Main_Device = Audio_Create_Device(-1, &format);
        if (Audio_Main_Device != 0 && Audio_Device_New_Channels(Audio_Main_Device, 16) == 16) {
            Audio_Global_Levels[0] = &Audio_Main_Device->Levels;
            AudioMix = new MixFileClass("AUDIO.MIX", &FastKey);
            Audio_Main_Bag = Audio_Bag_Create("audio", 0);
            if (Audio_Main_Bag == 0) {
                enable = 0;
            }
        } else {
            enable = 0;
        }
    }
    Audio_Streamer_Init();
    if (Audio_Events_Init(Audio_Main_Device, Audio_Main_Bag) == 0) {
        enable = 0;
    }
    unknown_007156d0();
    unknown_00715f10();
    Audio_Is_On = enable;
    return 1;
}


// void Audio_Shutdown() -- 0x00406C80
// Purpose: shuts the audio library down: the device's channels, the events and streams, the two sound helpers
// (0x00715780, 0x00715F80), the event system and the streamer; then the bag, AUDIO.MIX, THEME.MIX and the device are
// released and the system itself shut down.

void unknown_00715f80();    // 0x00715F80

void Audio_Shutdown()
{
    if (Audio_Main_Device != 0) {
        Audio_Device_Stop_Channels(Audio_Main_Device, 1);
    }
    Audio_Events_Clear();
    Audio_Streams_Stop();
    Speech_Shutdown();
    unknown_00715f80();
    Audio_Events_Shutdown();
    Audio_Streamer_Shutdown();
    if (Audio_Main_Bag != 0) {
        Audio_Bag_Destroy(Audio_Main_Bag);
        Audio_Main_Bag = 0;
    }
    if (AudioMix != 0) {
        delete AudioMix;
        AudioMix = 0;
    }
    if (ThemeMix != 0) {
        delete ThemeMix;
        ThemeMix = 0;
    }
    if (Audio_Main_Device != 0) {
        Audio_Device_Remove(Audio_Main_Device);
        Audio_Main_Device = 0;
    }
    Audio_System_Shutdown();
}


// int Audio_Open_Main_Device() -- 0x00406D20
// Purpose: creates the default driver and the main device (22050 Hz 16-bit
// stereo PCM) with 16 channels, as Audio_Init does. 1 when it has all 16.

int Audio_Open_Main_Device()
{
    Audio_Create_Driver(Audio_Default_Driver);
    AudioFormat format;
    Audio_Format_Init(&format);
    format.Rate = 22050;
    format.Type = AUDIO_FORMAT_PCM;
    format.Channels = 2;
    format.BytesPerSample = 2;
    format.unknown_00 = 4;
    format.Update_Rate();
    Audio_Main_Device = Audio_Create_Device(-1, &format);
    if (Audio_Main_Device != 0 && Audio_Device_New_Channels(Audio_Main_Device, 16) == 16) {
        return 1;
    }
    return 0;
}


// void Audio_Close_Main_Device() -- 0x00406DA0
// Purpose: removes the main device (Audio_Open_Main_Device's) when there is one.

void Audio_Close_Main_Device()
{
    if (Audio_Main_Device != 0) {
        Audio_Device_Remove(Audio_Main_Device);
        Audio_Main_Device = 0;
    }
}


// void __fastcall Audio_Stop() -- 0x00406DC0
// Purpose: stops the device's event channels and clears the events.

void __fastcall Audio_Stop()
{
    if (Audio_Main_Device != 0) {
        Audio_Device_Stop_Channels(Audio_Main_Device, 1);
    }
    Audio_Events_Clear();
}


// void __fastcall Audio_Pause() -- 0x00406DE0
// Purpose: pauses the device's event channels, the events and the
// music hooks' counters (0x007144B0).

void unknown_007144b0();    // 0x007144B0

void __fastcall Audio_Pause()
{
    if (Audio_Main_Device != 0) {
        Audio_Device_Pause_Channels(Audio_Main_Device, 1);
    }
    Audio_Events_Pause();
    unknown_007144b0();
}


// void Audio_Resume() -- 0x00406E00
// Purpose: undoes Audio_Pause.

void unknown_007144f0();    // 0x007144F0

void Audio_Resume()
{
    unknown_007144f0();
    Audio_Events_Resume();
    if (Audio_Main_Device != 0) {
        Audio_Device_Resume_Channels(Audio_Main_Device, 1);
    }
}


// void Audio_Stop_All() -- 0x00406E20
// Purpose: stops the event channels, the events and the streams.

void Audio_Stop_All()
{
    if (Audio_Main_Device != 0) {
        Audio_Device_Stop_Channels(Audio_Main_Device, 1);
    }
    Audio_Events_Clear();
    Audio_Streams_Stop();
}


// void Audio_Pause_All() -- 0x00406E40
// Purpose: Audio_Pause plus the two 0x007169xx layers.

void unknown_007144b0();    // 0x007144B0
void unknown_00716980();    // 0x00716980
void unknown_007168d0();    // 0x007168D0

void Audio_Pause_All()
{
    if (Audio_Main_Device != 0) {
        Audio_Device_Pause_Channels(Audio_Main_Device, 1);
    }
    Audio_Events_Pause();
    unknown_007144b0();
    unknown_00716980();
    unknown_007168d0();
}


// void Audio_Resume_All() -- 0x00406E70
// Purpose: undoes Audio_Pause_All.

void unknown_007144f0();    // 0x007144F0
void unknown_007168e0();    // 0x007168E0
void unknown_007169f0();    // 0x007169F0

void Audio_Resume_All()
{
    unknown_007168e0();
    unknown_007169f0();
    unknown_007144f0();
    Audio_Events_Resume();
    if (Audio_Main_Device != 0) {
        Audio_Device_Resume_Channels(Audio_Main_Device, 1);
    }
}


// void Audio_Update() -- 0x00406EA0
// Purpose: the audio part of the game's main loop: when more than 0x21 ticks
// have passed since the last time, updates the events and the other layers
// that need it. Does not reenter.

struct unknown_00a35d78_type;
extern unknown_00a35d78_type unknown_00a35d78;      // 0x00A35D78
void unknown_00714330();                            // 0x00714330
void unknown_00715ba0();                            // 0x00715BA0
void __fastcall unknown_006e7cd0(unknown_00a35d78_type *object);        // 0x006E7CD0
void unknown_007717c0();                            // 0x007717C0

void Audio_Update()
{
    if (Audio_Update_Busy == 0 && Audio_Main_Device != 0) {
        Audio_Update_Busy = 1;
        __int64 now = Audio_Now();
        if ((unsigned __int64)(now - Audio_Update_Last) > 0x21) {
            Audio_Events_Update();
            unknown_00714330();
            unknown_00715ba0();
            unknown_006e7cd0(&unknown_00a35d78);
            unknown_007717c0();
            Audio_Update_Last = now;
        }
        Audio_Update_Busy = 0;
    }
}


// int unknown_check_406f30() -- 0x00406F30
// Purpose: whether the audio library has its main device (the sound system can play).

int unknown_check_406f30()
{
    return Audio_Main_Device != 0;
}


// AudioDevice *Audio_Get_Device() -- 0x00406F40
// Purpose: the audio library's main device (0 without one).

AudioDevice *Audio_Get_Device()
{
    return Audio_Main_Device;
}


// 0x00406f50: function not yet identified
//
// Body: 21 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void FUN_00406f50(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406f50 {

extern void  Audio_DS_Stop_Primary_0040a860(void);
extern "C" unsigned int DAT_00831440;

void  FUN_00406f50(void)
{
    if (DAT_00831440 == 0) {
        Audio_DS_Stop_Primary_0040a860();
    }
    DAT_00831440 = DAT_00831440 + 1;
    return;
}

}  // namespace seed_00406f50


// 0x00406f70: function not yet identified
//
// Body: 23 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void FUN_00406f70(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00406f70 {

extern void  Audio_DS_Restart_Primary_0040a870(void);
extern "C" unsigned int DAT_00831440;

void  FUN_00406f70(void)
{
    if ((DAT_00831440 != 0) && (DAT_00831440 = DAT_00831440 + -1, DAT_00831440 == 0)) {
        Audio_DS_Restart_Primary_0040a870();
        return;
    }
    return;
}

}  // namespace seed_00406f70


// void __fastcall Audio_Level_Fade_To_Full(AudioLevel *level) -- 0x00406F90
// Purpose: ramps a level to full (0x4000).

void __fastcall Audio_Level_Fade_To_Full(AudioLevel *level)
{
    level->Set_Target(0x4000);
}


// void __fastcall Audio_Level_Fade_To_Zero(AudioLevel *level) -- 0x00406FA0
// Purpose: ramps a level down to nothing.

void __fastcall Audio_Level_Fade_To_Zero(AudioLevel *level)
{
    level->Set_Target(0);
}


// void __fastcall Audio_Level_Fade_To(AudioLevel *level, int value) -- 0x00406FB0
// Purpose: ramps a level towards value (Set_Target 0x004070A0 as a tail
// call). No callers in the executable, like Fade_To_Full 0x00406F90.

void __fastcall Audio_Level_Fade_To(AudioLevel *level, int value)
{
    level->Set_Target(value);
}


// 0x00406fc0: function not yet identified
//
// Body: 18 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __fastcall FUN_00406fc0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order, return width) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_00406fc0 {

int __fastcall FUN_00406fc0(int object_address)
{
    return *(int *)(object_address + 4) != *(int *)(object_address + 8);
}

}  // namespace seed_00406fc0


// void __fastcall Audio_Wait_Drain(AudioLevel *level) -- 0x00406FE0
// Purpose: calls 0x00481FB0 until the level has reached its target or its
// time limit (now plus level->MaxElapsed) has passed. Do_Lose-like scenario
// ends (0x0065E590) fade the global volume level to zero and wait here.

void unknown_00481fb0();            // 0x00481FB0

void __fastcall Audio_Wait_Drain(AudioLevel *level)
{
    __int64 end = Audio_Now() + level->MaxElapsed;

    while (level->Current != level->Target) {
        if ((unsigned __int64)Audio_Now() >= (unsigned __int64)end) {
            return;
        }
        unknown_00481fb0();
    }
}
