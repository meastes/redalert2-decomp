// SPDX-License-Identifier: GPL-3.0-or-later
// Test scaffolding (lane C16; NOT game behaviour): RA2_NATIVE_VQA_SELFTEST=1 makes main run this
// before WinMain and exit with its result (ctest native-vqa-selftest). The VQA player's path
// around a .VQA movie that a mod may ship (owner ruling 2026-11-15; the installed game plays only
// Bink movies, so no movie file of the kind is at hand):
//   * VQA_DefaultConfig 0x00777850 gives the library's defaults: its integer fields the ones the
//     user's game.exe stores at 0x00801470 (read here byte by byte to compare), the pointers null;
//   * VQA_Reset 0x0077C7D0 (VQAClass::Reset_VQA after every play) sets the stop frame to the last
//     frame, the loop state to none, the audio block positions and the loop cache window back.
//   * the half-height hi-colour decoders 0x0070BBC0/0x0070C0A0 (VQA option 11): a repeated codeword
//     fills rows 0 and 2 of each block from the codeword's rows 0 and 2, rows 1 and 3 untouched;
//   * the subtitle script (a mod's movie subtitles): the keyword table from game.exe recognises
//     Time, Duration, Position and Text in a small UTF-16 script.
// The hi-colour drawers 0x0077FD80/0x0077FE89 are checked in native-asm-check.
#include "shared/vqa_player.hpp"
#include "shared/movie.hpp"
#include "shared/subtitle.hpp"
#include "shared/buffer_straw.hpp"
#include "shared/vector_class.hpp"
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
    VQA_DefaultConfig(&config);
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
    VQA_Reset((VQAHandle *)vqap);
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

    // The half-height decoders: two blocks per row, one block row, 8 pixels wide; codeword 0's pixel
    // (r, c) is 0x100 * r + c + 1. 0x0070BBC0's 0x2002 and 0x0070C0A0's 0x2000 both repeat codeword 0
    // over the row's two blocks.
    {
        unsigned short codebook[16];
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c) codebook[r * 4 + c] = (unsigned short)(0x100 * r + c + 1);
        for (int which = 0; which < 2; ++which) {
            unsigned short out[4 * 8];
            for (int i = 0; i < 32; ++i) out[i] = 0xEEEE;
            unsigned short command[2] = {(unsigned short)(which ? 0x2000 : 0x2002), 0};
            if (which) VQA_Unknown_0070C0A0((unsigned char *)codebook, (unsigned char *)command, (unsigned char *)out, 2, 1, 8);
            else VQA_Unknown_0070BBC0((unsigned char *)codebook, (unsigned char *)command, (unsigned char *)out, 2, 1, 8);
            bool ok = true;
            for (int y = 0; y < 4; ++y)
                for (int x = 0; x < 8; ++x) {
                    unsigned short want = (y % 2) ? 0xEEEE : (unsigned short)(0x100 * y + x % 4 + 1);
                    ok = ok && out[y * 8 + x] == want;
                }
            check(ok, which ? "half-height 0x0070C0A0: rows 0 and 2 of a repeated codeword" : "half-height 0x0070BBC0: rows 0 and 2 of a repeated codeword");
        }
    }

    // The subtitle script.
    {
        static char const script[] = "BeginMovie TEST\r\n"
                                     "<Time=00:00:02:00, Duration=00:00:03:00, Position=3:Left, Text=\"Hi there\">\r\n"
                                     "EndMovie\r\n";
        static unsigned short wide[256];
        int n = 0;
        wide[n++] = 0xFEFF;
        for (int i = 0; script[i]; ++i) wide[n++] = (unsigned char)script[i];
        BufferStraw straw(wide, n * 2);
        SubTitleParser parser(straw);
        DynamicVectorClass<SubTitle *> *list = parser.Get_Sub_Titles("TEST");
        SubTitle *t = list && list->Count() == 1 ? (*list)[0] : 0;
        static char const want_text[] = "Hi there";
        bool text = t && t->Get_Text();
        for (int i = 0; text && i < (int)sizeof want_text; ++i) text = t->Get_Text()[i] == (unsigned char)want_text[i];
        check(t && t->Get_Display_Time() == 120 && t->Get_Display_Duration() == 180, "subtitles: Time and Duration");
        check(t && t->Get_Line_Position() == 3 && t->Get_Alignment() == 0, "subtitles: Position 3:Left");
        check(text, "subtitles: Text");
    }
    printf("vqa-selftest: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
