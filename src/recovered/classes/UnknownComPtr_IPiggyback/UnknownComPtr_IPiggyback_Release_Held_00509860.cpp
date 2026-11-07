// void UnknownComPtr<IPiggyback>::Release_Held() -- 0x00509860
// comip's _Release for the IPiggyback pointer: releases what is held, if anything. The destructor is the inline
// level above it; JumpJet_To_Walk 0x00508AB0 calls this copy for its converted temporary.
#include "shared/infantry_piggyback_ptr.hpp"
void UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::Release_Held()
{
    if (Pointer != 0) {
        Pointer->Release();
    }
}
