// SPDX-License-Identifier: GPL-3.0-or-later
// The VQA player's path after a movie (lane C16, owner ruling 2026-11-15: a mod's .VQA movie plays
// and returns). NOT EXACT: native bodies for the native tree only.
//
// VQA_Reset 0x0077C7D0: the best parked draft (src/recovered/vqalib_task.cpp, RA2_NON_MATCHING,
// score 66.67; TS VQA_Reset): clears the playback counters, loop state, timing and audio block
// positions, and the loop cache window. VQAClass::Reset_VQA calls it after every play.
//
// The library's default configuration 0x00801470 (VQA_DefaultConfig 0x00777850 copies it into
// every VQAClass's): the original stores it; its integer fields are read from the user's game.exe
// at the first copy (AGENTS.md "Native phase", user decision 2026-11-08), its pointer fields
// (handlers, buffers, owner) are null there and stay null (native fixup in
// vqalib_config/VQA_Unknown.cpp calls Native_VQA_Default_Config).
#include "shared/vqa_player.hpp"
#include <stdint.h>
#include <stdio.h>

namespace ra2port { bool original_bytes(uint32_t va, void *out, size_t n); }

void __fastcall VQA_Unknown_0077C7D0(VQAHandle *vqa)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    VQAAudio *audio = &vqap->Audio;
    VQAHeader *header = &vqap->Header;

    int stop = header->Frames - 1;
    vqap->LoadedFrames = 0;
    vqap->DrawnFrames = 0;
    vqap->StopFrame = stop;
    vqap->LoopEndFrameMode2 = stop;
    vqap->LoopEndFrame2 = stop;
    RA2_ULONG audio_flags = audio->Flags & ~(VQAAUDF_ISREPEATING | VQAAUDF_ISENDOFFILE | VQAAUDF_ISDONE | VQAAUDF_ISSTARVED);
    vqap->StartTime = 0;
    audio->Flags = audio_flags;
    vqap->EndTime = 0;
    vqap->Flags = 0;
    vqap->LoopIterations = 0;
    vqap->LoopID = -1;
    vqap->LoopStartFrame0 = -1;
    vqap->LoopEndFrameJump = -1;
    vqap->LoopIterationsJump = -1;
    vqap->LoopStartFrame1 = -1;
    vqap->LoopEndFrameNormal = -1;
    vqap->LoopStartFrame2 = -1;
    vqap->LastCallbackCount = -1;
    vqap->MaxCallbackDelta = 0;
    audio->Block1 = 0;
    audio->Block2 = -1;
    audio->BufferPosition = 0;
    vqap->RepeatedBuffers = 0;
    VQA_ResetCache(vqap);
}

VQAConfig unknown_vqa_default_config_801470;

void Native_VQA_Default_Config()
{
    static bool filled = false;
    if (filled) return;
    filled = true;
    unsigned char raw[0x88];
    if (!ra2port::original_bytes(0x00801470u, raw, sizeof raw)) {
        fprintf(stderr, "vqa: the default configuration (0x00801470) is not in game.exe; zeros used\n");
        return;
    }
    auto at = [&](int offset) {
        return (int)((uint32_t)raw[offset] | (uint32_t)raw[offset + 1] << 8 | (uint32_t)raw[offset + 2] << 16 |
                     (uint32_t)raw[offset + 3] << 24);
    };
    VQAConfig &c = unknown_vqa_default_config_801470;
    c.ImageWidth = at(0x20);
    c.ImageHeight = at(0x24);
    c.X1 = at(0x28);
    c.Y1 = at(0x2C);
    c.FrameRate = at(0x30);
    c.DrawRate = at(0x34);
    c.RefreshRate = at(0x38);
    c.unknown_3c = at(0x3C);
    c.DrawFlags = at(0x40);
    c.OptionFlags = at(0x44);
    c.NumFrameBufs = at(0x48);
    c.NumCBBufs = at(0x4C);
    c.unknown_50 = at(0x50);
    c.AudioBufSize = at(0x58);
    c.HMIBufSize = at(0x5C);
    c.AudioRate = at(0x60);
    c.Volume = at(0x64);
    c.unknown_68 = at(0x68);
    c.InitialLoopID = at(0x70);
    c.InitialLoopIterations = at(0x74);
    c.StreamFileHandle = at(0x78);
    c.unknown_7c = at(0x7C);
    c.unknown_80 = at(0x80);
    c.LatencyAdjustment = at(0x84);
}
