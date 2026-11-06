// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Audio_Ring_Destroy(AudioStream *ring) -- 0x00408760
// From ra2-decomp STM_StreamDestroy (YR 0x00408840).
// Purpose: frees every node of the ring, then the ring itself
// (Audio_Ring_Clear expanded inline).
#include "shared/audio.hpp"

void __cdecl operator delete(void *block);

void __fastcall Audio_Ring_Destroy(AudioStream *ring)
{
    Audio_Ring_Clear(ring);
    operator delete(ring);
}
