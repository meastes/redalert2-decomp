// void Audio_Channel_Lock(AudioChannel *channel) (0x004026A0): the channel driver's Lock (+0x28).
#include "shared/audio.hpp"

void __fastcall Audio_Channel_Lock(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
}
