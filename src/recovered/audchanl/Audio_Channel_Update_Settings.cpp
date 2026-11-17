// void __fastcall Audio_Channel_Update_Settings(AudioChannel *channel) -- 0x00402220
// Purpose: recomputes the channel's combined levels (+0xD8): starts from
// the defaults and combines the channel's own levels (clearing their
// changed flags), the optional level sets at +0x90 (also cleared), +0x94,
// +0x98 and +0x9C, and the device's levels and optional +0xE8 set.
#include "shared/audio.hpp"

void __fastcall Audio_Channel_Update_Settings(AudioChannel *channel)
{
    AudioDevice *device = channel->Device;
    channel->Settings.Init();
    Audio_Level_Set_Combine(&channel->Settings, &channel->Levels);
    channel->Levels.Clear_Changed();
    if (channel->unknown_levels_90 != 0) {
        Audio_Level_Set_Combine(&channel->Settings, channel->unknown_levels_90);
        channel->unknown_levels_90->Clear_Changed();
    }
    if (channel->unknown_levels_94 != 0) {
        Audio_Level_Set_Combine(&channel->Settings, channel->unknown_levels_94);
    }
    if (channel->unknown_levels_98 != 0) {
        Audio_Level_Set_Combine(&channel->Settings, channel->unknown_levels_98);
    }
    if (channel->unknown_levels_9c != 0) {
        Audio_Level_Set_Combine(&channel->Settings, channel->unknown_levels_9c);
    }
    Audio_Level_Set_Combine(&channel->Settings, &device->Levels);
    if (device->unknown_levels_e8 != 0) {
        Audio_Level_Set_Combine(&channel->Settings, device->unknown_levels_e8);
    }
}
