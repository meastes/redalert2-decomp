// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Audio_Events_Shutdown() -- 0x00403F80
// Purpose: stops the event system: stops and frees every event, frees any
// stopped event still listed, deletes every event type, destroys the sample
// bank and the event pool, and marks the system closed.
// from ra2-decomp 0x00403FA0 (AudioEventCloseDown, src/wpaudio/audevent.cpp)
#include "shared/audio_event.hpp"

static void Audio_Event_Types_Free()
{
    AudioListNode *type;
    while ((type = Audio_List_First(&Audio_Event_Type_List)) != 0) {
        Audio_List_Remove(type);
        operator delete(type);
    }
}

void Audio_Events_Shutdown()
{
    Audio_Events_Clear();
    Audio_Events_Free_Stopped();
    Audio_Event_Types_Free();
    if (Audio_Event_Bank != 0) {
        Audio_Bank_Destroy(Audio_Event_Bank);
        Audio_Event_Bank = 0;
    }
    if (Audio_Event_Pool != 0) {
        Audio_Pool_Destroy(Audio_Event_Pool);
        Audio_Event_Pool = 0;
    }
    Audio_Events_Open = 0;
    Audio_Events_Ready = 0;
}
