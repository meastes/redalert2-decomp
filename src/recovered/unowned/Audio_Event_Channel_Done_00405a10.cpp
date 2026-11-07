// int __fastcall Audio_Event_Channel_Done(AudioChannel *channel) -- 0x00405A10
// Purpose: channel callback when a sample has finished: clears the playing
// flag and, unless the event is already done, either starts the next sample
// (after the pending delay, if any) or marks the event done.
// Shape: ra2-decomp audioEventSampleDone (src/wpaudio/audevent.cpp, GAMEMD 0x00405A00) with eventWait inlined.
#include "shared/audio_event.hpp"

int __fastcall Audio_Event_Channel_Done(AudioChannel *channel)
{
    AudioEvent *event = (AudioEvent *)channel->Owner;
    int state;

    event->Flags &= ~2;
    if (event->State == 4) {
        return 0;
    }
    if (event->unknown_148 != 0 || event->unknown_268 != 0 || event->unknown_13c != 0) {
        state = 1;
    } else {
        state = 4;
        event->unknown_13c = 0;
    }
    if (event->unknown_13c != 0) {
        unsigned __int64 delay = event->unknown_13c;
        event->unknown_20 = state;
        if (event->PauseCount != 0) {
            event->unknown_140 = delay;
        } else {
            event->unknown_140 = Audio_Now() + delay;
        }
        event->State = 2;
        event->unknown_13c = 0;
    } else {
        event->State = state;
    }
    return 0;
}
