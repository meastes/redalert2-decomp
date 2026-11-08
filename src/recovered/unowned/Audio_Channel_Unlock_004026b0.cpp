// void Audio_Channel_Unlock(AudioChannel *channel) (0x004026B0): the channel driver's Unlock (+0x2C).
#include "shared/audio.hpp"

void __fastcall Audio_Channel_Unlock(AudioChannel *channel)
{
    channel->Driver->Unlock(channel);
}
