// Behaviour row (Scout1103, native M10): reached with audio once units answer
// orders (their voices end). NOT EXACT; never original code or bytes.
#include "shared/audio.hpp"

// 0x00405A00 Audio_Event_Channel_End (seed decompile): the channel's end
// callback (Audio_Event_Update 0x004055D0 installs it) marks the event that
// fed the channel (+0xC0 Owner) as ended: State (+0x1C) = 4.
int __fastcall Audio_Event_Channel_End(AudioChannel *channel)
{
    ((AudioEvent *)channel->Owner)->State = 4;
    return 0;
}
