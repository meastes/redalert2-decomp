// SPDX-License-Identifier: GPL-3.0-or-later
// Test scaffolding (lane C16; NOT game behaviour): RA2_NATIVE_VQA_SELFTEST=1 makes main run this
// before WinMain and exit with its result (ctest native-vqa-selftest). The VQA player's path
// around a .VQA movie that a mod may ship (owner ruling 2026-11-15; the installed game plays only
// Bink movies, so no movie file of the kind is at hand):
//   * VQA_DefaultConfig 0x00777850 gives the library's defaults: its integer fields the ones the
//     user's game.exe stores at 0x00801470 (read here byte by byte to compare), the pointers null;
//   * VQA_Reset 0x0077C7D0 (VQAClass::Reset_VQA after every play) sets the stop frame to the last
//     frame, the loop state to none, the audio block positions and the loop cache window back.
// The hi-colour drawers 0x0077FD80/0x0077FE89 are checked in native-asm-check.
#include "shared/vqa_player.hpp"
#include <stdint.h>

extern "C" int printf(char const *format, ...);
extern "C" void *calloc(RA2_SIZE_T count, RA2_SIZE_T size);
extern "C" void free(void *p);
namespace ra2port { bool original_bytes(uint32_t va, void *out, RA2_SIZE_T n); }

static int failures = 0;
static void check(bool ok, char const *what)
{
    printf("vqa-selftest: %s: %s\n", what, ok ? "ok" : "FAILED");
    if (!ok) ++failures;
}

extern "C" int ra2_native_vqa_selftest(void)
{
    unsigned char raw[0x88];
    if (!ra2port::original_bytes(0x00801470u, raw, sizeof raw)) {
        printf("vqa-selftest: game.exe not available (set RA2_GAME_EXE): skipped\n");
        return 77;
    }
    auto at = [&](int offset) {
        return (int)((uint32_t)raw[offset] | (uint32_t)raw[offset + 1] << 8 | (uint32_t)raw[offset + 2] << 16 |
                     (uint32_t)raw[offset + 3] << 24);
    };
    VQAConfig config;
    VQA_Unknown_00777850(&config);
    check(config.ImageWidth == at(0x20) && config.ImageHeight == at(0x24) && config.X1 == at(0x28) && config.Y1 == at(0x2C) &&
              config.FrameRate == at(0x30) && config.DrawRate == at(0x34) && config.OptionFlags == at(0x44) &&
              config.NumFrameBufs == at(0x48) && config.NumCBBufs == at(0x4C) && config.AudioBufSize == at(0x58) &&
              config.HMIBufSize == at(0x5C) && config.Volume == at(0x64) && config.InitialLoopID == at(0x70),
          "default configuration as game.exe stores it");
    check(config.ImageWidth != 0 || config.NumFrameBufs != 0, "default configuration not all zero");
    check(!config.StreamHandler && !config.EventHandler && !config.DrawerCallback && !config.ImageBuf && !config.AudioBuf &&
              !config.Owner,
          "default configuration's pointers null");

    VQAHandleP *vqap = (VQAHandleP *)calloc(1, sizeof(VQAHandleP));
    vqap->Header.Frames = 40;
    vqap->LoadedFrames = 17;
    vqap->DrawnFrames = 16;
    vqap->Flags = 0x55;
    vqap->LoopID = 3;
    vqap->LoopIterations = 2;
    vqap->MaxCallbackDelta = 9;
    vqap->RepeatedBuffers = 4;
    vqap->Audio.Flags = VQAAUDF_ISREPEATING | VQAAUDF_ISDONE | 1;
    vqap->Audio.Block1 = 5;
    vqap->Audio.Block2 = 6;
    vqap->Audio.BufferPosition = 7;
    vqap->LoopCache.ID = 2;
    VQA_Unknown_0077C7D0((VQAHandle *)vqap);
    check(vqap->LoadedFrames == 0 && vqap->DrawnFrames == 0 && vqap->StopFrame == 39 && vqap->LoopEndFrameMode2 == 39 &&
              vqap->LoopEndFrame2 == 39,
          "reset: counters and stop frame");
    check(vqap->Flags == 0 && vqap->LoopID == -1 && vqap->LoopIterations == 0 && vqap->LoopStartFrame0 == -1 &&
              vqap->LastCallbackCount == -1 && vqap->MaxCallbackDelta == 0 && vqap->RepeatedBuffers == 0,
          "reset: loop state");
    check(vqap->Audio.Flags == 1 && vqap->Audio.Block1 == 0 && (int)vqap->Audio.Block2 == -1 && vqap->Audio.BufferPosition == 0,
          "reset: audio blocks");
    check(vqap->LoopCache.ID == -1 && vqap->LoopCache.Min == -1 && vqap->LoopCache.Max == -1, "reset: loop cache");
    free(vqap);
    printf("vqa-selftest: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
